/*
 * Copyright (C) 2016-2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <platform_def.h>
#include <octeontx_common.h>
#include <string.h>
#include <debug.h>
#include <plat_scfg.h>
#include <octeontx_trace.h>
#include <runtime_svc.h>
#include <octeontx_svc.h>
#include <octeontx_mmap_utils.h>
#include <plat_board_cfg.h>

#include "cavm-csrs-dab_etr.h"
#include "cavm-csrs-dab_trc.h"

#define ARM_CPU_TRACEBUFSIZE_MIN 0x100000 /* 1M */
#define PAGE_SIZE_MAX PAGE_SIZE_64KB

/* Check if addr <--> (addr + size) lies within
 * start <--------------------------> end
 */
#define IS_ADDR_OUTSIDE_BOUNDARY(addr, size, start, end)	\
	(((addr) < (start)) ||			\
	 ((addr) > (end)) ||			\
	 ((addr) + (size) - 1 < (start)) ||	\
	 (addr) + (size) - 1 > (end))

/* Secure trace buffer partitioned equally among all cpus */
uint64_t arm_tracebuf; /* Start address */
uint64_t arm_tracebuf_end;
uint64_t arm_tracebuf_size;/* total trace buffer size */
uint64_t cpu_tracebufsize; /* cpu trace buffer size */

/* Hints about non secure - non preserved region */
uint64_t nsec_npres;
uint64_t nsec_npres_end;

/* Check if an address region is non secure, non preserve */
static inline bool is_address_region_nsec_npres(uint64_t addr, uint64_t size)
{
	bool outside;

	outside = IS_ADDR_OUTSIDE_BOUNDARY(addr, size,
					nsec_npres, nsec_npres_end);
	if (outside) {
		INFO("%s: address 0x%llx, size 0x%llx outside nsec_npres\n",
		     __func__, addr, size);
		return false;
	} else
		return true;
}

/* Check if an address region is  arm_tracebuf */
static inline bool is_address_region_arm_tracebuf(uint64_t addr, uint64_t size)
{
	bool outside;

	outside = IS_ADDR_OUTSIDE_BOUNDARY(addr, size,
					arm_tracebuf, arm_tracebuf_end);
	if (outside) {
		INFO("%s: address 0x%llx, size 0x%llx outside tracebuffer\n",
		     __func__, addr, size);
		return false;
	} else
		return true;
}

void plat_armtrace_stop(void)
{
	int i;
	uint64_t base1, base2;
	uint64_t size = PAGE_SIZE;
	unsigned int attr = MT_DEVICE | MT_RW | MT_NS;

	VERBOSE("Disabling ETM/ETR for %d cores\n", PLATFORM_CORE_COUNT);

	for (i = 0; i < PLATFORM_CORE_COUNT; i++) {
		base1 = ROUND_DOWN(CAVM_TRCX_TRCPRGCTLR(i), PAGE_SIZE);
		base2 = ROUND_DOWN(CAVM_ETRX_CONTROL(i), PAGE_SIZE);
		if (octeontx_mmap_add_dynamic_region_with_sync(base1, base1,
							    size, attr))
			goto err;
		if (octeontx_mmap_add_dynamic_region_with_sync(base2, base2,
							   size, attr))
			goto err;

		/* Unlock ETM */
		CSR_WRITE(CAVM_TRCX_TRCLAR(i), 0xc5acce55);

		/* Disable ETM */
		CSR_WRITE(CAVM_TRCX_TRCPRGCTLR(i), 0x0);

		/* Unlock ETR */
		CSR_WRITE(CAVM_ETRX_LAR(i), 0xc5acce55);

		/* Disable ETR */
		CSR_WRITE(CAVM_ETRX_CONTROL(i), 0x0);

		octeontx_mmap_remove_dynamic_region_with_sync(base1, size);
		octeontx_mmap_remove_dynamic_region_with_sync(base2, size);
	}

	return;

err:
	ERROR("Failed to disable ETM/ETR\n");
}

void plat_armtrace_init(void)
{
	uint64_t size;

	/* Ensure trace is stopped before we enter non secure world */
	plat_armtrace_stop();

	/* Initialize the arm trace buffer base/size */
	arm_tracebuf_size = ccs_region_get_info(SECURE_PRESERVE, &arm_tracebuf);
	if (!arm_tracebuf_size) {
		INFO("ARM trace buffer not configured\n");
		goto err;
	} else {
		/* Ensure per cpu buffer size is sufficient */
		cpu_tracebufsize = arm_tracebuf_size / PLATFORM_CORE_COUNT;
		cpu_tracebufsize = ROUND_DOWN(cpu_tracebufsize,
					      PAGE_SIZE_MAX);
		if (cpu_tracebufsize < ARM_CPU_TRACEBUFSIZE_MIN) {
			ERROR("ARM trace buffer size not sufficient\n");
			goto err;
		}

		/* Keep the whole tracebuffer mapped as RO */
		if (!IS_PAGE_ALIGNED(arm_tracebuf)) {
			ERROR("ARM trace buffer not page aligned\n");
			goto err;
		}
		if (octeontx_mmap_add_dynamic_region_with_sync(arm_tracebuf,
			arm_tracebuf, arm_tracebuf_size, MT_MEMORY | MT_RO)) {
			ERROR("ARM trace buffer map failure in secure world\n");
			goto err;
		}

		arm_tracebuf_end = arm_tracebuf + arm_tracebuf_size - 1;
		INFO("ARM trace buffer start = 0x%llx, size = 0x%llx\n",
		       arm_tracebuf, arm_tracebuf_size);
	}

	/* Update hints on the non secure non preserve area */
	size = ccs_region_get_info(NSECURE_NONPRESERVE, &nsec_npres);
	if (!size)
		goto err;
	nsec_npres_end = nsec_npres + size - 1;
	INFO("NS non preserve start = 0x%llx, size = 0x%llx\n",
		       nsec_npres, size);

	return;

err:
	cpu_tracebufsize = 0;
}

uint64_t arm_trace_get_cpu_tracebufsize(uint64_t *size)
{
	*size = cpu_tracebufsize;
	return SMC_OK;
}

uint64_t arm_trace_alloc_sbuf(uint64_t size, uint64_t cpu, int llc_lock_req,
			      uint64_t *addr)
{
	int err = 0;

	/* Verify if arm trace buffer is available and size is valid */
	if (!cpu_tracebufsize || size != cpu_tracebufsize)
		return SMC_UNK;

	/* Ensure end offset doesn't overflow the tracebuffer */
	if (cpu * size + size > arm_tracebuf_size)
		return SMC_UNK;

	*addr = arm_tracebuf + cpu * size;

	if (llc_lock_req)
		err = octeontx_llc_lock(*addr, size);

	if (err)
		return SMC_UNK;

	return SMC_OK;
}

/* API to map non secure buffer for copying trace data */
int arm_trace_register_drvbuf(uint64_t address, uint64_t size)
{
	if (!is_address_region_nsec_npres(address, size))
		return SMC_UNK;

	if (octeontx_mmap_add_dynamic_region_with_sync(address, address, size,
						MT_MEMORY | MT_RW | MT_NS)) {
		INFO("%s: NS address 0x%llx, size 0x%llx map failure\n",
			__func__, address, size);
		return SMC_UNK;
	}

	return SMC_OK;
}

int arm_trace_unregister_drvbuf(uint64_t address, uint64_t size)
{
	if (!is_address_region_nsec_npres(address, size))
		return SMC_UNK;

	if (octeontx_mmap_remove_dynamic_region_with_sync(address, size)) {
		INFO("%s: NS address 0x%llx, size 0x%llx unmap failure\n",
			__func__, address, size);
		return SMC_UNK;
	}

	return SMC_OK;
}

int arm_trace_copy_to_drvbuf(uint64_t dst,  uint64_t src, uint64_t size)
{

	/* Verify if arm trace buffer is available and size is valid */
	if (!cpu_tracebufsize || size > cpu_tracebufsize)
		return SMC_UNK;

	/* Ensure src is within the tracebuffer area */
	if (!is_address_region_arm_tracebuf(src, size))
		return SMC_UNK;

	/* Ensure dst is a non secure non preserve region */
	if (!is_address_region_nsec_npres(dst, size))
		return SMC_UNK;

	memcpy((void *)(dst), (void *)(src), size);
	return 0;
}

