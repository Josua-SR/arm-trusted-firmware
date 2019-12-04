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

#if REPORT_ERRATA
/*
 * Errata printing function for octeontx2. Must follow AAPCS.
 */
func octeontx2_errata_report
	ret
endfunc octeontx2_errata_report
#endif

declare_cpu_ops octeontx2, OCTEONTX2_CN96XX_MIDR, \
	octeontx2_reset_func, \
	octeontx2_core_pwr_dwn, \
	octeontx2_cluster_pwr_dwn

declare_cpu_ops octeontx2, OCTEONTX2_CNF95XX_MIDR, \
	octeontx2_reset_func, \
	octeontx2_core_pwr_dwn, \
	octeontx2_cluster_pwr_dwn

declare_cpu_ops octeontx2, OCTEONTX2_LOKI_MIDR, \
	octeontx2_reset_func, \
	octeontx2_core_pwr_dwn, \
	octeontx2_cluster_pwr_dwn

declare_cpu_ops octeontx2, OCTEONTX2_CN98XX_MIDR, \
	octeontx2_reset_func, \
	octeontx2_core_pwr_dwn, \
	octeontx2_cluster_pwr_dwn

declare_cpu_ops octeontx2, OCTEONTX2_F95MM_MIDR, \
	octeontx2_reset_func, \
	octeontx2_core_pwr_dwn, \
	octeontx2_cluster_pwr_dwn
