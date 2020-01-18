/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <octeontx_common.h>
#include <octeontx_utils.h>
#include <qlm/qlm.h>
#include <qlm/qlm_gserc.h>
#include <plat_otx2_configuration.h>

static void qlm_gserc_set_state(int qlm, int lane, qlm_state_lane_t state)
{
	CSR_WRITE(CAVM_GSERCX_SCRATCHX(qlm, lane), state.u);
}

/**
 * Check whether SERDES Rx lane is detecting a signal
 *
 * @param qlm	  QLM to use
 * @param lane	  Which lane
 * @return 0 on successful signal detected, 1 on no signal detected
 */
static int qlm_gserc_rx_signal_detect(int qlm, int lane)
{
	CSR_INIT(bsts, CAVM_GSERCX_LANEX_STATUS_BSTS(qlm, lane));
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
static int qlm_gserc_get_lmac_phy_lane(int qlm, int lane)
{
	if (IS_OCTEONTX_PN(read_midr(), LOKIPARTNUM)) {
		switch (qlm) {
		case 1:
		case 3:
			return (lane + 2);
		default:
			return lane;
		}
	}

	return lane;
}

/**
 * Get the LMAC's first GSER associated with the specified GSER.
 * Required for LMAC's that use DLM's
 *
 * @param  qlm	   QLM to use
 * @return Returns the LMAC first GSER
 */
static int qlm_gserc_get_lmac_first_qlm(int qlm)
{
	/* Need to add GSERP and GSERR count
	 * for LOKI
	 */
	if (IS_OCTEONTX_PN(read_midr(), LOKIPARTNUM)) {
		int gserp_gserr_count = plat_octeontx_get_gserp_count() +
					plat_octeontx_get_gserr_count();

		switch (qlm) {
		case 1:
		case 3:
			return qlm - 1 + gserp_gserr_count;
		default:
			return qlm + gserp_gserr_count;
		}
	}

	return qlm;
}

const qlm_ops_t qlm_gserc_ops = {
	.type = QLM_GSERC_TYPE,
	.qlm_get_state = qlm_gserc_get_state,
	.qlm_set_state = qlm_gserc_set_state,
	.qlm_set_mode = qlm_gserc_set_mode,
	.qlm_measure_refclock = qlm_gserc_measure_refclock,
	.qlm_reset = qlm_gserc_reset,
	.qlm_enable_prbs = qlm_gserc_enable_prbs,
	.qlm_disable_prbs = qlm_gserc_disable_prbs,
	.qlm_get_prbs_errors = qlm_gserc_get_prbs_errors,
	.qlm_inject_prbs_error = qlm_gserc_inject_prbs_error,
	.qlm_enable_loop = qlm_gserc_enable_loop,
	.qlm_tune_lane_tx = qlm_gserc_tune_lane_tx,
	.qlm_get_tune_lane_tx = qlm_gserc_get_tune_lane_tx,
	.qlm_rx_equalization = qlm__gserc_rx_equalization,
	.qlm_display_settings = qlm_gserc_display_settings,
	.qlm_eye_capture = qlm_gserc_eye_capture,
	.qlm_tx_control = qlm_gserc_tx_control,
	.qlm_rx_signal_detect = qlm_gserc_rx_signal_detect,
	.qlm_get_lmac_phy_lane = qlm_gserc_get_lmac_phy_lane,
	.qlm_get_lmac_first_qlm = qlm_gserc_get_lmac_first_qlm,
	.qlm_rx_adaptation_cdr_control = qlm_gserc_rx_adaption_cdr_control,
	.qlm_lane_rst = qlm_gserc_lane_rst,
	.qlm_link_training_start = qlm_gserc_link_training_start,
	.qlm_link_training_fail = qlm_gserc_link_training_fail,
	.qlm_link_training_complete = qlm_gserc_link_training_complete,
};
