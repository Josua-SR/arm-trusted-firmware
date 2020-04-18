/*
 * Copyright (C) 2016-2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <arch.h>
#include <asm_macros.S>
#include <bl_common.h>
#include <cpu_macros.S>
#include <plat_macros.S>

#define OCTEONTX_CN81XX_MIDR            0x430F0A20
#define OCTEONTX_CN83XX_MIDR            0x430F0A30

func octeontx_reset_func
	ret
endfunc octeontx_reset_func

func octeontx_core_pwr_dwn
	ret
endfunc octeontx_core_pwr_dwn

func octeontx_cluster_pwr_dwn
	ret
endfunc octeontx_cluster_pwr_dwn

	/* ----------------------------------------------
	 * void octeontx2_exception_handler(int)
	 * Function for handling sync exceptions specific
	 * for this platform. x1 argument contains EC
	 * from el3_esr.
	 * ----------------------------------------------
	 */
func octeontx_exception_handler
/* These errors are handled only in runtime (in BL31) */
#if defined(IMAGE_BL31)
	/* ------------------------------------------------------
	 * The following handles WFI trapped in EL0.
	 * Right now this only happens when EL0 ISR is returning.
	 * ------------------------------------------------------
	 */
	cmp	x1, #EC_WFE_WFI
	b.eq	el3_exit_wfi
#endif /* BL31 */

	/* ---------------------------------------------------
	 * Other errors are unhandled, return to generic code.
	 * ---------------------------------------------------
	 */
	ret
endfunc octeontx_exception_handler

func octeontx_cpu_reg_dump
	ret
endfunc octeontx_cpu_reg_dump

#if REPORT_ERRATA
/*
 * Errata printing function for octeontx. Must follow AAPCS.
 */
func octeontx_errata_report
	ret
endfunc octeontx_errata_report
#endif

declare_cpu_ops_eh octeontx, OCTEONTX_CN81XX_MIDR, \
	octeontx_reset_func, \
	octeontx_exception_handler, \
	octeontx_core_pwr_dwn, \
	octeontx_cluster_pwr_dwn

declare_cpu_ops_eh octeontx, OCTEONTX_CN83XX_MIDR, \
	octeontx_reset_func, \
	octeontx_exception_handler, \
	octeontx_core_pwr_dwn, \
	octeontx_cluster_pwr_dwn
