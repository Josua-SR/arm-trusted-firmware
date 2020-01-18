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
#include <plat_otx2_configuration.h>

static void qlm_gserr_set_state(int qlm, int lane, qlm_state_lane_t state)
{
	CSR_WRITE(CAVM_GSERRX_SCRATCHX(qlm, lane), state.u);
}

/**
 * Check whether SERDES Rx lane is detecting a signal
 *
 * @param qlm	  QLM to use
 * @param lane	  Which lane
 * @return 0 on successful signal detected, 1 on no signal detected
 */
static int qlm_gserr_rx_signal_detect(int qlm, int lane)
{
	CSR_INIT(bsts, CAVM_GSERRX_LANEX_STATUS_BSTS(qlm, lane));
	if (bsts.s.ln_stat_los)
		return 1;
	else
		return 0;
}

/**
 * Get the LMAC physical lane for the GSER physical lane
 *
 * @param  qlm	  QLM to use
 * @param  lane	  Which lane
 * @return Returns the physical lane
 */
static int qlm_gserr_get_lmac_phy_lane(int qlm, int lane)
{
	return lane;
}

/**
 * Get the LMAC's first GSER associated with the specified GSER.
 * Required for LMAC's that use DLM's
 *
 * @param  qlm	   QLM to use
 * @return Returns the LMAC first GSER
 */
static int qlm_gserr_get_lmac_first_qlm(int qlm)
{
	return qlm + plat_octeontx_get_gserp_count();
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
	.qlm_eye_capture = qlm_gserr_eye_capture,
	.qlm_tx_control = qlm_gserr_tx_control,
	.qlm_rx_signal_detect = qlm_gserr_rx_signal_detect,
	.qlm_get_lmac_phy_lane = qlm_gserr_get_lmac_phy_lane,
	.qlm_get_lmac_first_qlm = qlm_gserr_get_lmac_first_qlm,
	.qlm_rx_adaptation_cdr_control = qlm_gserr_rx_adaption_cdr_control,
	.qlm_lane_rst = qlm_gserr_lane_rst,
	.qlm_link_training_start = qlm_gserr_link_training_start,
	.qlm_link_training_fail = qlm_gserr_link_training_fail,
	.qlm_link_training_complete = qlm_gserr_link_training_complete,
};
