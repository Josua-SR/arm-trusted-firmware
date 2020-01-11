/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

/* EDAC/RAS SMCs for OcteonTX2 */

#include <arch.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <platform_def.h>
#include <octeontx_irqs_def.h>
#include <octeontx_common.h>
#include <plat_otx2_configuration.h>
#include <debug.h>
#include <bl31/interrupt_mgmt.h>
#include <plat_board_cfg.h>
#include <octeontx_security.h>
#include <octeontx_dram.h>
#include <octeontx_utils.h>
#include <tools_share/uuid.h>
#include <plat/common/platform.h>
#include <plat_ras.h>
#include <timers.h>
#include <drivers/delay_timer.h>
#include <cavm-csrs-ccu.h>
#include <cavm-csrs-ccs.h>
#include <cavm-csrs-lmc.h>
#include <cavm-csrs-mdc.h>

#include <cavm-csrs-ccs.h>

int64_t plat_ras_smc_op(u_register_t x1, u_register_t x2,
			u_register_t x3, u_register_t x4)
{
	/* log the interesting calls */
	if (x1 != OCTEONTX_EDAC_MDC_RW)
		debug_ras("%s(op:%lx x2:%lx x3:%lx x4:%lx)\n",
			__func__, x1, x2, x3, x4);

	switch (x1) {
	case OCTEONTX_EDAC_VER:
		return 0;
	case OCTEONTX_EDAC_INJECT:
		return plat_ras_lmc_inject(x2, x3, x4);
	case OCTEONTX_EDAC_MDC_CONST:
		return plat_ras_mdc_limits();
	case OCTEONTX_EDAC_MDC_RW:
		return plat_ras_mdc_rw(x2, x3, x4);
	case OCTEONTX_EDAC_MDC_ROM:
		return plat_ras_mdc_rom(x2);
	default:
		return -EINVAL;
	}
}
