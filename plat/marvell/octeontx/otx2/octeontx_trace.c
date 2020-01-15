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


/* Secure trace buffer partitioned among all cpus */
char arm_tracebuf[ARM_TRACEBUF_SIZE] __aligned(CACHE_WRITEBACK_GRANULE);

void plat_armtrace_init(void)
{
	int i;
	uint64_t base1, base2;
	uint64_t size = PAGE_SIZE;
	unsigned int attr = MT_DEVICE | MT_RW | MT_NS;

	printf("Disabling ETM/ETR for %d cores\n", PLATFORM_CORE_COUNT);

	/* Ensure trace is stopped before we enter non secure world */
	for (i = 0; i < PLATFORM_CORE_COUNT; i++) {
		base1 = ROUND_DOWN(CAVM_TRCX_TRCPRGCTLR(i), PAGE_SIZE);
		base2 = ROUND_DOWN(CAVM_ETRX_CONTROL(i), PAGE_SIZE);
		mmap_add_dynamic_region(base1, base1, size, attr);
		mmap_add_dynamic_region(base2, base2, size, attr);

		/* Unlock ETM */
		CSR_WRITE(CAVM_TRCX_TRCLAR(i), 0xc5acce55);

		/* Disable ETM */
		CSR_WRITE(CAVM_TRCX_TRCPRGCTLR(i), 0x0);

		/* Unlock ETR */
		CSR_WRITE(CAVM_ETRX_LAR(i), 0xc5acce55);

		/* Disable ETR */
		CSR_WRITE(CAVM_ETRX_CONTROL(i), 0x0);


		mmap_remove_dynamic_region(base1, size);
		mmap_remove_dynamic_region(base2, size);
	}
}

uint64_t arm_trace_alloc_sbuf(uint64_t size, uint64_t cpu, int llc_lock_req,
			      uint64_t *addr)
{
	static uint64_t cpu_size_init;
	int err = 0;

	if (!cpu_size_init)
		cpu_size_init = size;

	/* We assume buffer size is same for all cpus */
	if (cpu_size_init != size)
		return SMC_UNK;

	/* Ensure end offset doesn't overflow the tracebuffer */
	if (cpu * size + size > ARM_TRACEBUF_SIZE)
		return SMC_UNK;

	*addr = (uint64_t)arm_tracebuf + cpu * size;

	if (llc_lock_req)
		err = octeontx_llc_lock(*addr, size);

	if (err)
		return SMC_UNK;

	return SMC_OK;
}

/* API to map non secure buffer for copying trace data */
int arm_trace_register_drvbuf(uint64_t address, uint64_t size)
{
	if (mmap_add_dynamic_region(address, address, size, MT_MEMORY
			| MT_RW | MT_NS))
		return SMC_UNK;
	return SMC_OK;
}

int arm_trace_unregister_drvbuf(uint64_t address, uint64_t size)
{
	if (mmap_remove_dynamic_region(address, size))
		return SMC_UNK;

	return SMC_OK;
}

int arm_trace_copy_to_drvbuf(uint64_t dst,  uint64_t src, uint64_t size)
{

	/* Ensure src is within the tracebuffer area */
	if (((char *)src < arm_tracebuf) &&
	    ((char *)src > arm_tracebuf + ARM_TRACEBUF_SIZE - 1))
		return SMC_UNK;

	/* Ensure dst is a non secure region */
	if (dst < TZDRAM_BASE + TZDRAM_SIZE)
		return SMC_UNK;

	memcpy((void *)(dst), (void *)(src), size);
	return 0;
}

