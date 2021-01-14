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
#include <plat_board_cfg.h>
#include <octeontx_dram.h>

#include "cavm-csrs-lmc.h"

static inline uint32_t popcnt(uint64_t val)
{
	uint64_t x, x2 = val;

	x2 = x2 - ((x2 >> 1) & 0x5555555555555555ull);
	/* Every 2 bits holds the sum of every pair of bits (32) */
	x2 = ((x2 >> 2) & 0x3333333333333333ull) + (x2 & 0x3333333333333333ull);
	/* Every 4 bits holds the sum of every 4-set of bits (3 significant bits) (16) */
	x2 = (x2 + (x2 >> 4)) & 0x0F0F0F0F0F0F0F0Full;
	/* Every 8 bits holds the sum of every 8-set of bits (4 significant bits) (8) */

	x = x2 * 0x0101010101010101ull >> 56;

	/* The lower 8 bits hold the popcnt */
	return x;  /* (7 significant bits) */
}

uint64_t octeontx_dram_size()
{
	uint64_t addr = 0, size;

	size = memory_region_get_info(NSECURE_NONPRESERVE, &addr);
	size -= plat_octeontx_bcfg->reserved_os_memory_size;
	return size;
}

/*
 * This reserves memory from area used by OS (U-Boot/Linux).
 * This is done by reducing the size of available memory that is reported by
 * the API 'octeontx_dram_size()'.
 *
 * on entry,
 *   size:    size in bytes to reserve
 *            NOTE: this will be rounded UP to achieve 64K alignment
 *   index:   must be NSECURE_NONPRESERVE
 *            NOTE: This parameter exists solely for compatibility with other
 *                  implementations of this API.
 *
 * returns,
 *   Base address of OS memory.
 */
uint64_t octeontx_dram_reserve(uint64_t size, int index)
{
	/* Support memory reservation from NSECURE_NONPRESERVE only */
	if (index != NSECURE_NONPRESERVE) {
		ERROR("%s: Unsupported memory reservation type %d\n",
		      __func__, index);
		return 0;
	}

	/* convert size to integer multiple of 64K */
	if (size & 0xffff) {
		size += 0x10000;
		size &= ~0xffff;
	}

	plat_octeontx_bcfg->reserved_os_memory_size += size;

	/*
	 * This API will evaluate 'reserved_os_memory_size', which
	 * was just adjusted with the request size.
	 */
	size = octeontx_dram_size();

	return size; /* assume base address = 0 for OS memory */
}
