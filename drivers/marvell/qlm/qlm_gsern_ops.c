/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <octeontx_common.h>
#include <octeontx_utils.h>
#include <qlm/qlm.h>
#include <qlm/qlm_gsern.h>

static qlm_state_lane_t qlm_get_state_gsern(int qlm, int lane)
{
	qlm_state_lane_t state;

	state.u = CSR_READ(CAVM_GSERNX_LANEX_SCRATCHX(qlm, lane, 0));

	return state;
}

static void qlm_set_state_gsern(int qlm, int lane, qlm_state_lane_t state)
{
	CSR_WRITE(CAVM_GSERNX_LANEX_SCRATCHX(qlm, lane, 0), state.u);
}

const qlm_ops_t qlm_gsern_ops = {
	.type = QLM_GSERN_TYPE,
	.qlm_get_state = qlm_get_state_gsern,
	.qlm_set_state = qlm_set_state_gsern,
	.qlm_set_mode = qlm_set_mode_gsern,
	.qlm_measure_refclock = qlm_measure_refclock_gsern,
	.qlm_reset = qlm_reset_gsern,
	.qlm_enable_prbs = qlm_enable_prbs_gsern,
	.qlm_disable_prbs = qlm_disable_prbs_gsern,
	.qlm_get_prbs_errors = qlm_get_prbs_errors_gsern,
	.qlm_inject_prbs_error = qlm_inject_prbs_error_gsern,
	.qlm_enable_loop = qlm_enable_loop_gsern,
	.qlm_tune_lane_tx = qlm_tune_lane_tx_gsern,
	.qlm_get_tune_lane_tx = qlm_get_tune_lane_tx_gsern,
	.qlm_rx_equalization = qlm_rx_equalization_gsern,
	.qlm_display_settings = qlm_display_settings_gsern,
	.qlm_eye_capture = gsern_eye_capture,
	.qlm_tx_control = qlm_tx_control_gsern,
	.qlm_tx_sm_rst_control = qlm_tx_sm_rst_control_gsern,
	.qlm_rx_signal_detect = qlm_rx_signal_detect_gsern,
	.qlm_get_lmac_phy_lane = qlm_gsern_get_lmac_phy_lane,
	.qlm_get_lmac_first_qlm = qlm_gsern_get_lmac_first_qlm,
};
