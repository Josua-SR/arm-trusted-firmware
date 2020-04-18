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

#define OCTEONTX2_CN98XX_MIDR		0x430F0B10
#define OCTEONTX2_CN96XX_MIDR		0x430F0B20
#define OCTEONTX2_CNF95XX_MIDR		0x430F0B30
#define OCTEONTX2_LOKI_MIDR		0x430F0B40
#define OCTEONTX2_F95MM_MIDR		0x430F0B50

#define EC_CAVIUM_IO_TRAP		U(0x1f)

func octeontx2_reset_func
	ret
endfunc octeontx2_reset_func

func octeontx2_core_pwr_dwn
	ret
endfunc octeontx2_core_pwr_dwn

func octeontx2_cluster_pwr_dwn
	ret
endfunc octeontx2_cluster_pwr_dwn

func octeontx2_cpu_reg_dump
	ret
endfunc octeontx2_cpu_reg_dump

	/* ----------------------------------------------
	 * void octeontx2_exception_handler(int)
	 * Function for handling sync exceptions specific
	 * for this platform. x1 argument contains EC
	 * from el3_esr.
	 * ----------------------------------------------
	 */
func octeontx2_exception_handler
/* These errors are handled only in runtime (in BL31) */
#if defined(IMAGE_BL31)
	/* -----------------------------------------------------
	 * Handle Cavium-specific trap - synchronous data abort
	 * with (as per ARM's ARM DDI0487B) EC = 0x1f, which
	 * is IMPLEMENTATION_DEFINED
	 * -----------------------------------------------------
	 */
	cmp	x1, #EC_CAVIUM_IO_TRAP
	b.eq	octeontx_io_trap_handler

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
endfunc octeontx2_exception_handler

#if REPORT_ERRATA
/*
 * Errata printing function for octeontx2. Must follow AAPCS.
 */
func octeontx2_errata_report
	ret
endfunc octeontx2_errata_report
#endif

declare_cpu_ops_eh octeontx2, OCTEONTX2_CN96XX_MIDR, \
	octeontx2_reset_func, \
	octeontx2_exception_handler, \
	octeontx2_core_pwr_dwn, \
	octeontx2_cluster_pwr_dwn

declare_cpu_ops_eh octeontx2, OCTEONTX2_CNF95XX_MIDR, \
	octeontx2_reset_func, \
	octeontx2_exception_handler, \
	octeontx2_core_pwr_dwn, \
	octeontx2_cluster_pwr_dwn

declare_cpu_ops_eh octeontx2, OCTEONTX2_LOKI_MIDR, \
	octeontx2_reset_func, \
	octeontx2_exception_handler, \
	octeontx2_core_pwr_dwn, \
	octeontx2_cluster_pwr_dwn

declare_cpu_ops_eh octeontx2, OCTEONTX2_CN98XX_MIDR, \
	octeontx2_reset_func, \
	octeontx2_exception_handler, \
	octeontx2_core_pwr_dwn, \
	octeontx2_cluster_pwr_dwn

declare_cpu_ops_eh octeontx2, OCTEONTX2_F95MM_MIDR, \
	octeontx2_reset_func, \
	octeontx2_exception_handler, \
	octeontx2_core_pwr_dwn, \
	octeontx2_cluster_pwr_dwn
