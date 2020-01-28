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
	return NULL;
}

struct otx2_ghes_err_record *otx2_begin_ghes(const char *name,
			    struct otx2_ghes_err_ring **ringp)
{
	struct otx2_ghes_err_record *err_rec;
	struct otx2_ghes_err_ring *err_ring;
	uint32_t total_ring_len, tail, head;
	struct fdt_ghes *gh;

	gh = otx2_find_ghes("mcc");
	if (!gh)
		return NULL;

	err_ring = gh->base[GHES_PTR_RING];
	total_ring_len = gh->size[GHES_PTR_RING];
	/* TODO: fix me - head/tail/size need one-time init */
	err_ring->size =
		(total_ring_len -
		 offsetof(struct otx2_ghes_err_ring, records[0])) /
		sizeof(err_ring->records[0]);

	tail = err_ring->tail;
	/* TODO: fix me - need to check for 'full' condition */
	(void)tail;
	head = err_ring->head;
	err_rec = &err_ring->records[head];
	memset(err_rec, 0, sizeof(*err_rec));

	if (ringp)
		*ringp = err_ring;
	return err_rec;
}

void otx2_send_ghes(struct otx2_ghes_err_record *rec,
		    struct otx2_ghes_err_ring *err_ring,
		    int event)
{
	uint32_t head = err_ring->head;

	/* Ensure that error record is written fully prior to advancing
	 * the head (which indicates availability to consumer).
	 */
	dsbsy();

	if (++head >= err_ring->size)
		head = 0;
	err_ring->head = head;
	dsbsy();

#if SDEI_SUPPORT
	sdei_dispatch_event(event);
#endif
}

void otx2_map_ghes(void)
{
	ras_config_t *rc = &plat_octeontx_bcfg->ras_config;
	uint64_t lo = ~0ULL;
	uint64_t hi = 0;
	int i, j;

	for (i = 0; i < rc->nr_ghes; i++) {
		struct fdt_ghes *g = &rc->fdt_ghes[i];

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

	if (lo >= hi)
		return;
	lo &= ~PAGE_SIZE_MASK;
	hi |= PAGE_SIZE_MASK;
	debug_ras("%s map %llx..%llx\n", __func__, lo, hi + 1 - lo);
	mmap_add_region(lo, lo, hi + 1 - lo,
		MT_MEMORY | MT_RW | MT_NS);
}

int otx2_ras_init(void)
{
	plat_ras_initialize_interrupt_array();
	ras_init();

	return 0;
}
