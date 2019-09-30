/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <octeontx_common.h>
#include <octeontx_utils.h>
#include <qlm/qlm.h>
#include <qlm/qlm_gserr.h>

static void qlm_gserr_set_state(int qlm, int lane, qlm_state_lane_t state)
{
	CSR_WRITE(CAVM_GSERRX_SCRATCHX(qlm, lane), state.u);
}

const qlm_ops_t qlm_gserr_ops = {
	.type = QLM_GSERR_TYPE,
	.qlm_get_state = qlm_gserr_get_state,
	.qlm_set_state = qlm_gserr_set_state,
	.qlm_set_mode = qlm_gserr_set_mode,
	.qlm_measure_refclock = qlm_gserr_measure_refclock,
	.qlm_reset = qlm_gserr_reset,
	.qlm_enable_prbs = qlm_gserr_enable_prbs,
	.qlm_disable_prbs = qlm_gserr_disable_prbs,
	.qlm_get_prbs_errors = qlm_gserr_get_prbs_errors,
	.qlm_inject_prbs_error = qlm_gserr_inject_prbs_error,
	.qlm_enable_loop = qlm_gserr_enable_loop,
	.qlm_tune_lane_tx = qlm_gserr_tune_lane_tx,
	.qlm_get_tune_lane_tx = qlm_gserr_get_tune_lane_tx,
	.qlm_rx_equalization = qlm__gserr_rx_equalization,
	.qlm_display_settings = qlm_gserr_display_settings,
	.qlm_eye_capture = qlm_gserr_eye_capture
};
