/*
 * Copyright (C) 2016-2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __PLAT_BOARD_CFG_H__
#define __PLAT_BOARD_CFG_H__

#include <octeontx_board_cfg.h>

typedef enum l2c_region_index {
	SECURE_NONPRESERVE,
	NSECURE_NONPRESERVE,
	SECURE_PRESERVE, /* used as ARM Trace Buffer */
	NSECURE_PRESERVE,
	L2C_REGION_IDX_MAX,
} l2c_region_index_t;

typedef struct plat_octeontx_board_cfg {
	board_cfg_t bcfg;
	int qlm_auto_config;

	int gpio_intercept_intr;
} plat_octeontx_board_cfg_t;

extern plat_octeontx_board_cfg_t * const plat_octeontx_bcfg;

#endif /* __PLAT_BOARD_CFG__ */
