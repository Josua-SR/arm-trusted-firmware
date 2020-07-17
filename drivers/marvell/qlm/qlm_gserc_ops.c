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
#include <gser_internal.h>

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
	/* Skip check if nea or ned loopbacks enabled */
	GSER_CSR_INIT(dpl_rxdp_ctrl1, CAVM_GSERCX_LNX_TOP_DPL_RXDP_CTRL1(qlm, lane));
	GSER_CSR_INIT(afe_loopback_ctrl, CAVM_GSERCX_LNX_TOP_AFE_LOOPBACK_CTRL(qlm, lane));

	if (dpl_rxdp_ctrl1.s.rx_dmux_sel ||
		afe_loopback_ctrl.s.loopback_nea_en)
		return 0;

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
static int qlm_gserc_get_lmac_phy_lane(int qlm, int lane, int lane_to_sds)
{
	int phy_lane;
	if (IS_OCTEONTX_PN(read_midr(), LOKIPARTNUM)) {
		switch (qlm) {
		case 1:
		case 3:
			lane += 2;
		break;
		}
	}
	phy_lane = ((lane_to_sds >> (lane * 2)) & 0x3);
	return phy_lane;
}

void qlm_gserc_rx_leq_adaptation(int qlm, int lane, int leq_lfg_start,
	int leq_hfg_sql_start, int leq_mbf_start, int leq_mbg_start,
	int gn_apg_start)
{
	int disable = 1;
	/* Configure LEQ adaptation */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_ADAPT_CONT_CFG0(qlm, lane),
		c.s.en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_CTLE_ADAPT_LFG_CFG(qlm, lane),
		c.s.init1_en = !disable;
		c.s.init0_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_LEQ_REFCLK_EQ_LFG_CTRL0(qlm, lane),
		c.s.eq_lfg_start = leq_lfg_start);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_CTLE_ADAPT_APG_CFG(qlm, lane),
		c.s.init1_en = !disable;
		c.s.init0_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_LEQ_REFCLK_GN_APG_CTRL0(qlm, lane),
		c.s.gn_apg_start = gn_apg_start);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_CTLE_ADAPT_HFG_CFG0(qlm, lane),
		c.s.init1_en = !disable;
		c.s.init0_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_CTLE_ADAPT_HFG_CFG1(qlm, lane),
		c.s.init1_en = !disable;
		c.s.init0_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_LEQ_REFCLK_EQ_HFG_SQL_CTRL0(qlm, lane),
		c.s.eq_hfg_sql_start = leq_hfg_sql_start);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_CTLE_ADAPT_MBS_CFG0(qlm, lane),
		c.s.init1_en = !disable;
		c.s.init0_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_CTLE_ADAPT_MBS_CFG1(qlm, lane),
		c.s.init1_en = !disable;
		c.s.init0_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_LEQ_REFCLK_EQ_MB_CTRL1(qlm, lane),
		c.s.eq_mbg_start = leq_mbg_start;
		c.s.eq_mbf_start = leq_mbf_start);
}

void qlm_gserc_rx_dfe_adaptation(int qlm, int lane)
{
	int disable = 1;
	/* Configure DFE adaptation */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_DFE_ADAPT_TAP1_CFG(qlm, lane),
		c.s.tap1_start_val_sel = 0;
		c.s.tap1_cont_en = !disable;
		c.s.tap1_eie_en = 0;
		c.s.tap1_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_DFE_ADAPT_TAP2_CFG(qlm, lane),
		c.s.tap2_start_val_sel = 0;
		c.s.tap2_cont_en = !disable;
		c.s.tap2_eie_en = 0;
		c.s.tap2_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_DFE_ADAPT_TAP3_CFG(qlm, lane),
		c.s.tap3_start_val_sel = 0;
		c.s.tap3_cont_en = !disable;
		c.s.tap3_eie_en = 0;
		c.s.tap3_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_DFE_ADAPT_TAP4_CFG(qlm, lane),
		c.s.tap4_start_val_sel = 0;
		c.s.tap4_cont_en = !disable;
		c.s.tap4_eie_en = 0;
		c.s.tap4_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_DFE_ADAPT_TAP5_CFG(qlm, lane),
		c.s.tap5_start_val_sel = 0;
		c.s.tap5_cont_en = !disable;
		c.s.tap5_eie_en = 0;
		c.s.tap5_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_DFE_ADAPT_TAP6_CFG(qlm, lane),
		c.s.tap6_start_val_sel = 0;
		c.s.tap6_cont_en = !disable;
		c.s.tap6_eie_en = 0;
		c.s.tap6_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_DFE_ADAPT_TAP7_CFG(qlm, lane),
		c.s.tap7_start_val_sel = 0;
		c.s.tap7_cont_en = !disable;
		c.s.tap7_eie_en = 0;
		c.s.tap7_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_DFE_ADAPT_TAP8_CFG(qlm, lane),
		c.s.tap8_start_val_sel = 0;
		c.s.tap8_cont_en = !disable;
		c.s.tap8_eie_en = 0;
		c.s.tap8_init_en = !disable);
}

/**
 * Check whether SERDES Link Training Failed
 *
 * @param qlm	  QLM to use
 * @param lane	  Which lane
 * @return 0 on no failure, 1 on fail
 */
static int qlm_gserc_link_training_fail(int qlm, int lane)
{
	GSER_CSR_INIT(fsm_status, CAVM_GSERCX_LNX_LT_TX_FSM_STATUS(qlm, lane));
	if (fsm_status.s.training_fail)
		return 1;
	else
		return 0;
}

/**
 * Check whether SERDES Link Training Completed
 *
 * @param qlm	  QLM to use
 * @param lane	  Which lane
 * @return 0 on completion, 1 not complete
 */
static int qlm_gserc_link_training_complete(int qlm, int lane)
{
	GSER_CSR_INIT(fsm_ctrl0, CAVM_GSERCX_LNX_LT_TX_FSM_CTRL0(qlm, lane));
	if (fsm_ctrl0.s.signal_detect)
		return 0;
	else
		return 1;
}

/**
 * Set Phy Strap
 *
 * @param qlm    Index into GSER* group
 * @param lane      Which lane
 */
static void qlm_gserc_set_phy_strap(int qlm, int lane)
{
	/* Read GSERCX_LANEX_CONTROL_BCFG Fields. Read initially
	 * from GSERCX_LANEX_STATUS_BSTS [ln_an_link_sel]
	 */
	GSER_CSR_INIT(status_bsts, CAVM_GSERCX_LANEX_STATUS_BSTS(qlm, lane));
	/* Write value from above into GSERCX_LANEX_CONTROL_BCFG [ln_link_stat] */
	GSER_CSR_MODIFY(control_bcfg, CAVM_GSERCX_LANEX_CONTROL_BCFG(qlm, lane),
		control_bcfg.s.ln_link_stat = status_bsts.s.ln_an_link_sel);
}

/**
 * Check whether SERDES AN Completed
 *
 * @param qlm	  QLM to use
 * @param lane	  Which lane
 * @return 0 on completion, 1 not complete
 */
static int qlm_gserc_an_complete(int qlm, int lane)
{
	GSER_CSR_INIT(lane_status, CAVM_GSERCX_LANEX_STATUS_BSTS(qlm, lane));
	if (lane_status.s.ln_an_stat_resolved)
		return 0;
	else
		return 1;
}

/**
 * Check if PRBS Rx or Tx is enabled
 *
 * @param  qlm        Index into GSER* group
 * @param lane	  Which lane
 * @return 1 if PRBS enabled on lane, 0 if no PRBS
 */
static int qlm_gserc_prbs_chk(int qlm, int lane)
{
	/* Check if any lane has Rx or Tx PRBS enabled */
	GSER_CSR_INIT(rx_ctrl, CAVM_GSERCX_LNX_BIST_RX_CTRL(qlm, lane));
	GSER_CSR_INIT(tx_ctrl, CAVM_GSERCX_LNX_BIST_TX_CTRL(qlm, lane));
	if (rx_ctrl.s.en || tx_ctrl.s.en)
		return 1;

	return 0;
}

/**
 * Check if farend digital or analog loopbacks are enabled
 *
 * @param qlm    Index into GSER* group
 * @param lane	 Which lane
 * @return 1 if farend loopback enabled on lane, 0 if not enabled
 */
static int qlm_gserc_farend_lpbk_chk(int qlm, int lane)
{
	/* Farend Analog */
	GSER_CSR_INIT(loopback_ctrl, CAVM_GSERCX_LNX_TOP_AFE_LOOPBACK_CTRL(qlm, lane));
	/* Farend Digital */
	GSER_CSR_INIT(txdp_ctrl1, CAVM_GSERCX_LNX_TOP_DPL_TXDP_CTRL1(qlm, lane));

	if (loopback_ctrl.s.loopback_fea_en || txdp_ctrl1.s.lb_fed_tx_en)
		return 1;

	return 0;
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
	.qlm_link_training_fail = qlm_gserc_link_training_fail,
	.qlm_link_training_complete = qlm_gserc_link_training_complete,
	.qlm_set_phy_strap = qlm_gserc_set_phy_strap,
	.qlm_an_complete = qlm_gserc_an_complete,
	.qlm_prbs_chk = qlm_gserc_prbs_chk,
	.qlm_farend_lpbk_chk = qlm_gserc_farend_lpbk_chk,
	.qlm_display_trace = qlm_gserc_display_trace,
	.qlm_rx_leq_adapt = qlm_gserc_rx_leq_adaptation,
	.qlm_rx_dfe_adapt = qlm_gserc_rx_dfe_adaptation,
	.qlm_ned_loopback = qlm_gserc_ned_loopback,
	.qlm_fea_loopback = qlm_gserc_fea_loopback,
};
