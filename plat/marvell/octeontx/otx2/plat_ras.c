/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <lib/extensions/ras.h>
#include <octeontx_irqs_def.h>
#include <plat_ras.h>
#include <plat_ghes.h>
#include <plat/common/platform.h>
#include <octeontx_sdei.h>
#include <lib/el3_runtime/context_mgmt.h>

#if SDEI_SUPPORT
#include <services/sdei.h>
#endif

/*
 * It is number of all RAS interrupts.
 */
#define NUMBER_OF_RAS_INTERRUPTS	(RAS_PPI_IRQS + \
					MDC_SPI_IRQS + \
					MCC_SPI_IRQS + \
					LMC_SPI_IRQS)

#define RAS_PPI_HANDLER			0
#define RAS_MDC_HANDLER			1
#define RAS_MCC_HANDLER			2
#define RAS_LMC_HANDLER			3
#define RAS_HANDLERS			4

static int plat_ras_mdc_handler(const struct err_record_info *info,
		int probe_data, const struct err_handler_data *const data)
{
	int ret;

	cm_el1_sysregs_context_save(NON_SECURE);

	ret = otx2_mdc_isr(data->interrupt, data->flags, data->cookie);

	/* issue EOI to controller */
	if (data->interrupt)
		plat_ic_end_of_interrupt(data->interrupt);

	return ret;
}

static int plat_ras_mcc_handler(const struct err_record_info *info,
		int probe_data, const struct err_handler_data *const data)
{
	int ret;

	cm_el1_sysregs_context_save(NON_SECURE);

	ret = otx2_mcc_isr(data->interrupt, data->flags, data->cookie);

	/* issue EOI to controller */
	if (data->interrupt)
		plat_ic_end_of_interrupt(data->interrupt);

	return ret;
}

static int plat_ras_lmc_handler(const struct err_record_info *info,
		int probe_data, const struct err_handler_data *const data)
{
	int ret;

	cm_el1_sysregs_context_save(NON_SECURE);

	ret = otx2_lmc_isr(data->interrupt, data->flags, data->cookie);

	/* issue EOI to controller */
	if (data->interrupt)
		plat_ic_end_of_interrupt(data->interrupt);

	return ret;
}

struct ras_interrupt otx2_ras_interrupts[NUMBER_OF_RAS_INTERRUPTS];

struct err_record_info otx2_err_records[RAS_HANDLERS] = {
#ifdef notyet
	[RAS_PPI_HANDLER] = {
		.probe = otx2_ppi_probe,
		.handler = plat_ras_ppi_handler,
	},
#endif
	[RAS_MDC_HANDLER] = {
		.probe = otx2_mdc_probe,
		.handler = plat_ras_mdc_handler,
	},
	[RAS_MCC_HANDLER] = {
		.probe = otx2_mcc_probe,
		.handler = plat_ras_mcc_handler,
	},
	[RAS_LMC_HANDLER] = {
		.probe = otx2_lmc_probe,
		.handler = plat_ras_lmc_handler,
	},
};

REGISTER_ERR_RECORD_INFO(otx2_err_records);
REGISTER_RAS_INTERRUPTS(otx2_ras_interrupts);

static void plat_ras_initialize_interrupt_array(void)
{
	int i, idx = 0;

	for (i = 0; i < RAS_PPI_IRQS; i++) {
		otx2_ras_interrupts[idx].intr_number = RAS_PPI_IRQ(i);
		otx2_ras_interrupts[idx].err_record =
				&otx2_err_records[RAS_PPI_HANDLER];
		idx++;
	}

	for (i = 0; i < MDC_SPI_IRQS; i++) {
		otx2_ras_interrupts[idx].intr_number = MDC_SPI_IRQ(i);
		otx2_ras_interrupts[idx].err_record =
				&otx2_err_records[RAS_MDC_HANDLER];
		idx++;
	}

	for (i = 0; i < MCC_SPI_IRQS; i++) {
		otx2_ras_interrupts[idx].intr_number = MCC_SPI_IRQ(i);
		otx2_ras_interrupts[idx].err_record =
				&otx2_err_records[RAS_MCC_HANDLER];
		idx++;
	}

	for (i = 0; i < LMC_SPI_IRQS; i++) {
		otx2_ras_interrupts[idx].intr_number = LMC_SPI_IRQ(i);
		otx2_ras_interrupts[idx].err_record =
				&otx2_err_records[RAS_LMC_HANDLER];
		idx++;
	}
}

struct fdt_ghes *otx2_find_ghes(const char *name)
{
	int i;
	ras_config_t *rc = &plat_octeontx_bcfg->ras_config;

	for (i = 0; rc && i < rc->nr_ghes; i++) {
		if (strcmp(name, rc->fdt_ghes[i].name))
			continue;
		debug_ras("%s(%s) finds entry %d: %p %p %p\n",
			__func__, name, i,
			rc->fdt_ghes[i].base[0],
			rc->fdt_ghes[i].base[1],
			rc->fdt_ghes[i].base[GHES_PTRS - 1]);
		return &rc->fdt_ghes[i];
	}

	if (!strcmp(name, rc->fdt_bert.name)) {
		debug_ras("%s(%s) finds entry: %p %p %p\n",
			__func__, name,
			rc->fdt_bert.base[0],
			rc->fdt_bert.base[1],
			rc->fdt_bert.base[GHES_PTRS - 1]);
		return &rc->fdt_bert;
	}

	return NULL;
}

/*
 * Used to retrieve an error record pointer for a GHES source.
 * The error record can subsequently be sent to the consumer
 * via 'otx2_send_ghes()'.
 *
 * On entry,
 *   name:  specifies GHES source (i.e. 'mdc', 'mcc')
 *   ringp: pointer by which error ring value is returned
 *          (see error conditions below)
 *
 * Returns,
 *   Pointer to next available error record for GHES source OR
 *   NULL if:
 *     1. GHES source is full
 *     2. GHES source not found
 *
 *   Note: if NULL is returned, the error condition can be discerned by
 *   examining the value returned in 'ringp'.  If the GHES source is present,
 *   'ringp' will contain a valid pointer, thus describing error #1 (full).
 *   If the specified GHES source was not located, 'ringp' will also
 *   contain NULL, thus describing error #2 (not found).
 */
struct otx2_ghes_err_record *otx2_begin_ghes(const char *name,
			    struct otx2_ghes_err_ring **ringp)
{
	struct otx2_ghes_err_record *err_rec;
	struct otx2_ghes_err_ring *err_ring;
	uint32_t tail, head;
	struct fdt_ghes *gh;

	if (ringp)
		*ringp = NULL;

	gh = otx2_find_ghes(name);
	if (!gh)
		return NULL;

	err_ring = gh->base[GHES_PTR_RING];
	if (!err_ring->size) {
		ERROR("error ring '%s' size is uninitialized\n", name);
		return NULL;
	}

	/* if consumer not registered */
	if (err_ring->reg != OTX2_GHES_ERR_RING_SIG) {
		debug2ras("%s unset reg\n", __func__);
		return NULL;
	}

	tail = err_ring->tail;
	head = err_ring->head;
	if (((head + 1) % err_ring->size) != tail) {
		err_rec = &err_ring->records[head];
		memset(err_rec, 0, sizeof(*err_rec));
	} else {
		err_rec = NULL;
		ERROR("GHES error ring '%s' is full\n", name);
	}

	if (ringp)
		*ringp = err_ring;
	return err_rec;
}

void otx2_send_ghes(struct otx2_ghes_err_record *rec,
		    struct otx2_ghes_err_ring *err_ring,
		    int event)
{
	int ret = 0;
	uint32_t head = err_ring->head;

	/* Ensure that error record is written fully prior to advancing
	 * the head (which indicates availability to consumer).
	 */
	dmbsy();

	if (++head >= err_ring->size)
		head = 0;
	err_ring->head = head;
	dsbsy();

#if SDEI_SUPPORT
	ret = sdei_dispatch_event(event);
	if (ret != 0) {
		/*
		 * sdei_dispatch_event() may return failing result in some cases,
		 * for example kernel may not have registered a handler or RAS event
		 * may happen early during boot. We restore the NS context when
		 * sdei_dispatch_event() returns failing result.
		 */
		ERROR("SDEI dispatch failed: %d", ret);
		cm_el1_sysregs_context_restore(NON_SECURE);
		cm_set_next_eret_context(NON_SECURE);
	}
#endif
}

void otx2_map_ghes(void)
{
	struct fdt_ghes *g;
	ras_config_t *rc;
	uint64_t lo, hi;
	int i, j;

	rc = &plat_octeontx_bcfg->ras_config;
	lo = ~0ULL;
	hi = 0;

	/* add mapping for GHES memory */
	for (i = 0; i < rc->nr_ghes; i++) {
		g = &rc->fdt_ghes[i];

		for (j = 0; j < GHES_PTRS; j++) {
			debug_ras("(%s) %d.%d %x@%p\n",
				g->name, i, j, g->size[j], g->base[j]);
			if (!g->size[j])
				break;
			if (lo > (uint64_t) g->base[j])
				lo = (uint64_t) g->base[j];
			if (hi < (uint64_t) g->base[j] + g->size[j])
				hi = (uint64_t) g->base[j] + g->size[j];
			debug_ras("range %llx..%llx\n", lo, hi);
		}
	}

	if (lo < hi) {
		lo &= ~(uint64_t)PAGE_SIZE_MASK;
		hi += PAGE_SIZE_MASK;
		hi &= ~(uint64_t)PAGE_SIZE_MASK;
		hi--;
		debug_ras("%s map %llx..%llx\n", __func__, lo, hi + 1 - lo);
		mmap_add_region(lo, lo, hi + 1 - lo,
			MT_MEMORY | MT_RW | MT_NS);
	}

	/* add mapping for BERT memory */
	lo = ~0ULL;
	hi = 0;
	g = &rc->fdt_bert;
	for (j = 0; j < GHES_PTRS; j++) {
		debug_ras("(%s) %d.%d %x@%p\n",
			g->name, i, j, g->size[j], g->base[j]);
		if (!g->size[j])
			break;
		if (lo > (uint64_t) g->base[j])
			lo = (uint64_t) g->base[j];
		if (hi < (uint64_t) g->base[j] + g->size[j])
			hi = (uint64_t) g->base[j] + g->size[j];
		debug_ras("range %llx..%llx\n", lo, hi);
	}

	if (lo < hi) {
		lo &= ~(uint64_t)PAGE_SIZE_MASK;
		hi += PAGE_SIZE_MASK;
		hi &= ~(uint64_t)PAGE_SIZE_MASK;
		hi--;
		debug_ras("%s map %llx..%llx\n", __func__, lo, hi + 1 - lo);
		mmap_add_region(lo, lo, hi + 1 - lo,
			MT_MEMORY | MT_RW | MT_NS);
	}
}

/*
 * err_ring_init()
 *
 * Used to initialilze an error ring.
 *
 * on entry,
 *   err_ring: error ring
 *   len:      allocated length for entirety of ring (including header)
 *   entries:  if non-zero, # of entries to use in ring
 *             if zero, # of entries to use in ring is calculated dynamically
 *   reinit:   force reinitialize buffer accidentally memory can stay in previous state
 *
 * Returns,
 *   false if ring was NOT already initialized
 *   !false if ring WAS already initialized
 */
static bool
err_ring_init(struct otx2_ghes_err_ring *err_ring, int len, int entries, bool reinit)
{
	bool init = false;

	if (err_ring && len) {
		err_ring->reg = 0;
		init = (err_ring->sig == OTX2_GHES_ERR_RING_SIG);
		if (!init || reinit) {
			err_ring->sig = OTX2_GHES_ERR_RING_SIG;
			err_ring->head = err_ring->tail = 0;
			err_ring->size = entries ? entries :
			   (len -
			    offsetof(struct otx2_ghes_err_ring, records[0])) /
			   sizeof(err_ring->records[0]);
		}
		if (!reinit) {
			/* BERT buffer must be registered at boot */
			debug2ras("%s setup reg\n", __func__);
			err_ring->reg = OTX2_GHES_ERR_RING_SIG;
		}
	}

	return init;
}

int otx2_ras_init(void)
{
	struct otx2_ghes_err_ring *err_ring;
	uint32_t ring_len;
	ras_config_t *cfg;
	int i;

	plat_ras_initialize_interrupt_array();
	ras_init();

	/* set default GHES ring size (based on total ring mem length) */
	cfg = &plat_octeontx_bcfg->ras_config;
	for (i = 0; i < ARRAY_SIZE(cfg->fdt_ghes); i++) {
		err_ring = cfg->fdt_ghes[i].base[GHES_PTR_RING];
		ring_len = cfg->fdt_ghes[i].size[GHES_PTR_RING];
		err_ring_init(err_ring, ring_len, 0, 1);
	}

	/* set default BERT ring size */
	err_ring = cfg->fdt_bert.base[GHES_PTR_RING];
	ring_len = cfg->fdt_bert.size[GHES_PTR_RING];
	err_ring_init(err_ring, ring_len, BERT_RAS_RING_SIZE, 0);

#if DEBUG_RAS
	otx2_begin_ghes("bert", &err_ring);
	if (err_ring) {
		debug_ras("BERT ring head/tail/size %u/%u/%u\n",
			  err_ring->head, err_ring->tail,
			  err_ring->size);
	}
#endif

	return 0;
}
