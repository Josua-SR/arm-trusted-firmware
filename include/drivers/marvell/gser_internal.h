/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef _GSER_INTERNAL_H_
#define _GSER_INTERNAL_H_

#include <gsern/gsern.h>
#include <octeontx_common.h>
#include <debug.h>
#include <drivers/delay_timer.h>
#include <plat_scfg.h>
#include <plat_board_cfg.h>
#include <octeontx_utils.h>

/* define DEBUG_ATF_GSER to enable debug logs */
#undef DEBUG_ATF_GSER

#ifdef DEBUG_ATF_GSER
#define debug_gser printf
/*
 * Define values to evaluate following expression to true:
 * gser_trace_enables & (1ull << GSER_TRACE_ENABLE_QLM)
 */
#define gser_trace_enables	2
#define GSER_TRACE_ENABLE_QLM	1
#else
#define debug_gser(...) ((void) (0))
/*
 * Define values to evaluate following expression to false:
 * gser_trace_enables & (1ull << GSER_TRACE_ENABLE_QLM)
 */
#define gser_trace_enables	1
#define GSER_TRACE_ENABLE_QLM	1
#endif /* DEBUG_ATF_GSER */

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/* These constants represent the possible QLM reference clock speeds in Hz */
#define REF_100MHZ 100000000
#define REF_122MHZ 122800000
#define REF_125MHZ 125000000
#define REF_156MHZ 156250000

/* From bdk/libbdk-hal/bdk-clock.h */
#define GTI_RATE 100000000ull

/* Argument for gser_is_platform */
#define GSER_PLATFORM_ASIM		0
#define GSER_PLATFORM_EMULATOR		1
#define GSER_PLATFORM_HW		2

/* Argument for gsern_config_get_int */
#define GSER_CONFIG_QLM_LANE_RX_POLARITY		0
#define GSER_CONFIG_QLM_LANE_TX_POLARITY		1
#define GSER_CONFIG_QLM_TUNING_TX_MAIN			2
#define GSER_CONFIG_QLM_TUNING_TX_PRE			3
#define GSER_CONFIG_QLM_TUNING_TX_POST			4
#define GSER_CONFIG_QLM_TUNING_TX_BS			5
#define GSER_CONFIG_QLM_VOLTAGE				6
#define GSER_CONFIG_QLM_TUNING_RX_PREVGA_GN_OVRD	7
#define GSER_CONFIG_QLM_TUNING_RX_PREVGA_GN_ADAPT	8
#define GSER_CONFIG_QLM_LANE_IDLE_REFSET_VALUE		9
#define GSER_CONFIG_QLM_CLK_TERM		       10
#define GSER_CONFIG_QLM_RX_HFG		               11

#define GSER_CONFIG_QLM_TUNING_TX_MAIN_NAME	"MAIN"
#define GSER_CONFIG_QLM_TUNING_TX_PRE_NAME	"PRE"
#define GSER_CONFIG_QLM_TUNING_TX_POST_NAME	"POST"
#define GSER_CONFIG_QLM_TUNING_TX_BS_NAME	"BS"

#define GSER_CSR_WAIT_FOR_FIELD(addr, mask, ignore, val, timeout)	\
	gser_poll_for_csr(addr, mask, val, timeout)

/* Defines for simple functions */
#define gser_qlm_get_lanes(q)	plat_octeontx_scfg->qlm_max_lane_num[q]
#define qlm_get_lanes(q)	gser_qlm_get_lanes(q)

#define GSER_TRACE(ignore, format, ...)	debug_gser(format, ##__VA_ARGS__)
#define GSER_CSR_MODIFY			CSR_MODIFY
#define GSER_CSR_INIT			CSR_INIT
#define GSER_CSR_WRITE			CSR_WRITE
#define GSER_CSR_READ			CSR_READ
#define GSER_CSR_DEFINE			CSR_DEFINE
#define false				0
#define true				1
#define gser_fatal			ERROR
#define gser_error			WARN
#define gser_warn			WARN
#define gser_wait_usec			udelay
#define gser_is_model			cavm_is_model
#define gser_clock_get_count(...)	read_cntpct_el0()
#define gser_clock_get_rate(...)	GTI_RATE
#define gser_extracts			octeontx_bit_extracts
#define gser_extract			octeontx_bit_extract
#define gser_insert			octeontx_bit_insert
#define qlm_eye_t			gser_qlm_eye_t
#define gser_build_mask			octeontx_build_mask
#define gser_be16_to_cpu		cavm_be16_to_cpu
#define gser_be32_to_cpu		cavm_be32_to_cpu
#define gser_be64_to_cpu		cavm_be64_to_cpu
#define gser_node_t			int

static inline int64_t gser_extract_smag(uint64_t v, int lsb, int msb)
{
	int64_t r = octeontx_bit_extract(v, lsb, msb - lsb);

	if (v & (1ull << msb))
		r = -r;
	return r;
}

static inline int qlm_get_gbaud_mhz(int qlm, int lane)
{
	qlm_state_lane_t state;
	cgx_config_t *cgx;
	int gserx, cgx_idx;

	cgx_idx = plat_get_cgx_idx(qlm);
	cgx = &(plat_octeontx_bcfg->cgx_cfg[cgx_idx]);
	if (cgx->qlm_ops == NULL) {
		debug_gser("%s:CGX%d: has no qlm_ops\n",  __func__, cgx_idx);
		return 0;
	}

	gserx = plat_otx2_get_gserx(qlm, NULL);

	state = cgx->qlm_ops->qlm_get_state(gserx, lane);

	return state.s.baud_mhz;
}

/* Internal functions */
extern int gsern_voltage;

int gser_is_platform(int plat);
int gser_config_get_int(int prop, ...);
int gser_poll_for_csr(uint64_t addr, uint64_t mask, int poll_val, int timeout);
const char *qlm_mode_to_cfg_str(qlm_modes_t mode);
uint8_t gser_rng_get_random8(void);
void qlm_gserx_default_tuning(qlm_modes_t mode, int baud_mhz, int *tx_swing,
				int *tx_cpre, int *tx_cpost);

/* Implemented in drivers/marvell/gsern/gsern_init_common.c */
int gsern_init_wait_for_sm_complete(int qlm);
int gsern_init_wait_for_sm_ready(int qlm, int qlm_lane);
int gsern_init_wait_for_rx_ready(int qlm, int qlm_lane);
int gsern_init_wait_for_tx_ready(int qlm, int qlm_lane);

/* Stub implemented in drivers/marvell/gsern/gsern_internal.c */
int gsern_init_pcie(int qlm, bool is_first_qlm, enum gsern_flags flags,
	enum gsern_lane_modes mode);
int gsern_init_sata_no_scc(int qlm, enum gsern_lane_modes mode);
int gsern_init_sata_scc(int qlm, enum gsern_lane_modes mode);

/* Implemented in drivers/marvell/gsern/gsern_init_network.c */
int gsern_init_network(int qlm, int qlm_lane, enum gsern_flags flags,
	enum gsern_lane_modes mode);

#endif /* _GSER_INTERNAL_H_ */
