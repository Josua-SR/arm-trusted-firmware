/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __OCTEONTX_DRAM_H__
#define __OCTEONTX_DRAM_H__

#include <plat_board_cfg.h>

uint64_t octeontx_dram_size();
uint64_t memory_region_get_info(int index, uint64_t *start);
uint64_t octeontx_dram_reserve(uint64_t size, int index);

#endif /* __OCTEONTX_DRAM_H__ */
