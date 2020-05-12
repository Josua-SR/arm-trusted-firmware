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
#include <gser_internal.h>

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
static int qlm_gserr_get_lmac_phy_lane(int qlm, int lane, int lane_to_sds)
{
	int phy_lane;

	phy_lane = ((lane_to_sds >> (lane * 2)) & 0x3);
	return phy_lane;
}

/**
 * Get the LMAC's first GSER associated with the specified GSER.
 * Required for LMAC's that use DLM's
 *
 * @param  qlm	   QLM to use
 * @param  lane	   Which lane
 * @param  disable Disable Rx adaption and fix CDR
 * @param  is_10g  true if speed is 10G
 */
static void qlm_gserr_rx_adaption_cdr_control(int qlm, int lane, bool disable, bool is_10g)
{
	int leq_lfg_start, leq_hfg_sql_start;
	int leq_mbf_start, leq_mbg_start;
	int gn_apg_start, rxcdr_bbstep;

	if (disable) {
		GSER_TRACE(QLM, "GSERR%d.%d: Disabling LEQ and DFE adaptation\n", qlm, lane);
		leq_lfg_start = 0;
		leq_hfg_sql_start = 9;
		leq_mbf_start = 5;
		leq_mbg_start = 7;
		gn_apg_start = 3;
		rxcdr_bbstep = 10;
	} else {
		GSER_TRACE(QLM, "GSERR%d.%d: Enabling LEQ and DFE adaptation\n", qlm, lane);
		if (is_10g) {
			leq_lfg_start = 0;
			leq_hfg_sql_start = 20;
			leq_mbf_start = 0;
			leq_mbg_start = 0;
			gn_apg_start = 7;
		} else { /* 25Gb/s Settings */
			leq_lfg_start = 6;
			leq_hfg_sql_start = 24;
			leq_mbf_start = 8;
			leq_mbg_start = 8;
			gn_apg_start = 4;
		}
		rxcdr_bbstep = 20;
	}
	/* Adjust CDR phase gain */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_CDR_REFCLK_AFE_CTRL0(qlm, lane),
		c.s.rxcdr_bbstep = rxcdr_bbstep);

	/* Configure LEQ adaptation */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_ADAPT_CONT_CFG0(qlm, lane),
		c.s.en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_CTLE_ADAPT_LFG_CFG(qlm, lane),
		c.s.init1_en = !disable;
		c.s.init0_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_LEQ_REFCLK_EQ_LFG_CTRL0(qlm, lane),
		c.s.eq_lfg_start = leq_lfg_start);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_CTLE_ADAPT_APG_CFG(qlm, lane),
		c.s.init1_en = !disable;
		c.s.init0_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_LEQ_REFCLK_GN_APG_CTRL0(qlm, lane),
		c.s.gn_apg_start = gn_apg_start);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_CTLE_ADAPT_HFG_CFG0(qlm, lane),
		c.s.init1_en = !disable;
		c.s.init0_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_CTLE_ADAPT_HFG_CFG1(qlm, lane),
		c.s.init1_en = !disable;
		c.s.init0_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_LEQ_REFCLK_EQ_HFG_SQL_CTRL0(qlm, lane),
		c.s.eq_hfg_sql_start = leq_hfg_sql_start);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_CTLE_ADAPT_MBS_CFG0(qlm, lane),
		c.s.init1_en = !disable;
		c.s.init0_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_CTLE_ADAPT_MBS_CFG1(qlm, lane),
		c.s.init1_en = !disable;
		c.s.init0_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_LEQ_REFCLK_EQ_MB_CTRL1(qlm, lane),
		c.s.eq_mbg_start = leq_mbg_start;
		c.s.eq_mbf_start = leq_mbf_start);
	/* Configure DFE adaptation */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_DFE_ADAPT_TAP1_CFG(qlm, lane),
		c.s.tap1_start_val_sel = 0;
		c.s.tap1_cont_en = !disable;
		c.s.tap1_eie_en = 0;
		c.s.tap1_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_DFE_ADAPT_TAP2_CFG(qlm, lane),
		c.s.tap2_start_val_sel = 0;
		c.s.tap2_cont_en = !disable;
		c.s.tap2_eie_en = 0;
		c.s.tap2_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_DFE_ADAPT_TAP3_CFG(qlm, lane),
		c.s.tap3_start_val_sel = 0;
		c.s.tap3_cont_en = !disable;
		c.s.tap3_eie_en = 0;
		c.s.tap3_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_DFE_ADAPT_TAP4_CFG(qlm, lane),
		c.s.tap4_start_val_sel = 0;
		c.s.tap4_cont_en = !disable;
		c.s.tap4_eie_en = 0;
		c.s.tap4_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_DFE_ADAPT_TAP5_CFG(qlm, lane),
		c.s.tap5_start_val_sel = 0;
		c.s.tap5_cont_en = !disable;
		c.s.tap5_eie_en = 0;
		c.s.tap5_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_DFE_ADAPT_TAP6_CFG(qlm, lane),
		c.s.tap6_start_val_sel = 0;
		c.s.tap6_cont_en = !disable;
		c.s.tap6_eie_en = 0;
		c.s.tap6_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_DFE_ADAPT_TAP7_CFG(qlm, lane),
		c.s.tap7_start_val_sel = 0;
		c.s.tap7_cont_en = !disable;
		c.s.tap7_eie_en = 0;
		c.s.tap7_init_en = !disable);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_DFE_ADAPT_TAP8_CFG(qlm, lane),
		c.s.tap8_start_val_sel = 0;
		c.s.tap8_cont_en = !disable;
		c.s.tap8_eie_en = 0;
		c.s.tap8_init_en = !disable);
}

/**
 * Reset the GSER lane.
 *
 * @param  qlm	     QLM to use
 * @param  lane	     Which lane
 * @param  reset_en  1) Enable reset 0) Clear reset
 * @return Returns the LMAC first GSER
 */
static void qlm_gserr_lane_rst(int qlm, int lane, bool reset)
{
	if (reset)
		GSER_TRACE(QLM, "GSERR%d.%d: Setting Lane Reset\n", qlm, lane);
	else
		GSER_TRACE(QLM, "GSERR%d.%d: Clearing Lane Reset\n", qlm, lane);

	if (reset) {
		GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(qlm, lane),
			c.s.ln_ctrl_tx_en = 0);
		gser_wait_usec(1);
	}

	/* Assert or deassert Lane reset */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(qlm, lane),
		c.s.ln_rst = reset);

	if (reset) {
		/* Wait for the PHY firmware to signal that the Lane is in the Reset
			power state which is signaled by the lane Tx and Rx blocks negating
			the Tx/Rx ready signals.
			Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
				LN_TX_RDY=0 Lane Tx is not ready
				LN_RX_RDY=0 Lane Rx is not ready
				LN_STATE_CHNG_RDY = 0 Lane is transitioning states */
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(qlm, lane), GSERRX_STATUS_BSTS_LN_TX_RDY, ==, 0, 500))
			gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_tx_rdy]=0 (lane is reset)\n", qlm, lane);
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(qlm, lane), GSERRX_STATUS_BSTS_LN_RX_RDY, ==, 0, 500))
			gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_rx_rdy]=0 (lane is reset)\n", qlm, lane);
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(qlm, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 0, 500))
		{
			/* This happens fast, so sometimes we miss it */
			//gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=0\n (lane is reset)", module, lane);
		}
	} 
	else 
	{
		/* Read/Poll for the GSERR to set the Lane State Change Ready flag and
		drive the Lane Tx and Rx ready flags to signal that the lane as
		returned to the ACTIVE state.
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_TX_RDY=1 Lane Tx is ready
			LN_RX_RDY=1 Lane Rx is ready
			LN_STATE_CHNG_RDY = 1 Lane is in the “Active” power state */
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(qlm, lane), GSERRX_STATUS_BSTS_LN_TX_RDY, ==, 1, 5000))
			gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_tx_rdy]=1 (reset done)\n", qlm, lane);
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(qlm, lane), GSERRX_STATUS_BSTS_LN_RX_RDY, ==, 1, 5000))
			gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_rx_rdy]=1 (reset done)\n", qlm, lane);
	}

	/* 4. Wait for the “Lane State Change Ready” to signal that the lane has
		transitioned to the “Reset” state.
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 1 Lane is in the “Reset” power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(qlm, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 10000))
		gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1 (lane is reset)\n", qlm, lane);
}

/**
 * Configure SERDES for Link Training
 *
 * @param qlm	  QLM to use
 * @param lane	  Which lane
 * @param en      0=Disable Training, 1=Enable Training
 */
static void qlm_gserr_link_training_config(int qlm, int lane, bool en)
{
	/* Set/Clear Training enable */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_LT_TX_FSM_CTRL0(qlm, lane),
		c.s.mr_training_enable = en);
}

/**
 * Check whether SERDES Link Training Failed
 *
 * @param qlm	  QLM to use
 * @param lane	  Which lane
 * @return 0 on no failure, 1 on fail
 */
static int qlm_gserr_link_training_fail(int qlm, int lane)
{
	GSER_CSR_INIT(fsm_status, CAVM_GSERRX_LNX_LT_TX_FSM_STATUS(qlm, lane));
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
static int qlm_gserr_link_training_complete(int qlm, int lane)
{
	GSER_CSR_INIT(fsm_ctrl0, CAVM_GSERRX_LNX_LT_TX_FSM_CTRL0(qlm, lane));
	if (fsm_ctrl0.s.signal_detect)
		return 0;
	else
		return 1;
}

/**
 * Set GSERR handshake bit to start AN
 *
 * @param  qlm    Index into GSER* group
 * @param lane       Which lane
 */
static void qlm_gserr_start_an(int qlm, int lane)
{
	/* AN Handshaking bit assignments
	 * GSERRX_LNX_FEATURE_SPARE_CFG6_RSVD[data]
	 * data[3] = Handshake enable
	 * data[4] = Handshake ready
	 */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_SPARE_CFG6_RSVD(qlm, lane),
				c.s.data &= 0xf7);
}

/**
 * Set Phy Strap
 *
 * @param qlm    Index into GSER* group
 * @param lane      Which lane
 */
static void qlm_gserr_set_phy_strap(int qlm, int lane)
{
	/* Read GSERRX_LANEX_CONTROL_BCFG Fields. Read initially
	 * from GSERRX_LANEX_STATUS_BSTS [ln_an_link_sel]
	 */
	GSER_CSR_INIT(status_bsts, CAVM_GSERRX_LANEX_STATUS_BSTS(qlm, lane));
	/* Write value from above into GSERRX_LANEX_CONTROL_BCFG [ln_link_stat] */
	GSER_CSR_MODIFY(control_bcfg, CAVM_GSERRX_LANEX_CONTROL_BCFG(qlm, lane),
		control_bcfg.s.ln_link_stat = status_bsts.s.ln_an_link_sel);
}

/**
 * Check whether SERDES AN Completed
 *
 * @param qlm	  QLM to use
 * @param lane	  Which lane
 * @return 0 on completion, 1 not complete
 */
static int qlm_gserr_an_complete(int qlm, int lane)
{
	GSER_CSR_INIT(lane_status, CAVM_GSERRX_LANEX_STATUS_BSTS(qlm, lane));
	if (lane_status.s.ln_an_stat_resolved)
		return 0;
	else
		return 1;
}

/**
 * Read CSRs to determine Link Training Status
 *
 * @param  qlm   Index into GSER* group
 * @param  lane    Which lane
 */
static void qlm_gserr_get_link_training_status(int qlm, int lane)
{
#ifdef DEBUG_ATF_GSER
	GSER_CSR_INIT(fsm_ctrl5, CAVM_GSERRX_LNX_LT_TX_FSM_CTRL5(qlm, lane));
	GSER_TRACE(QLM, "GSERR%d_LN%d_LT_TX_FSM_CTRL5\n", qlm, lane);
			GSER_TRACE(QLM, "remote_rx_ready: %d\trx_trained: %d\n",
			fsm_ctrl5.s.remote_rx_ready,
			fsm_ctrl5.s.rx_trained);

	GSER_CSR_INIT(fsm_status, CAVM_GSERRX_LNX_LT_TX_FSM_STATUS(qlm, lane));
	GSER_TRACE(QLM, "GSERR%d_LN%d_LT_TX_FSM_STATUS\n", qlm, lane);
	GSER_TRACE(QLM, "fsm_local_rx_ready: %d\tsignal_detect: %d\ttraining_fail: %d\n",
			fsm_status.s.fsm_local_rx_ready,
			fsm_status.s.signal_detect,
			fsm_status.s.training_fail);

	GSER_CSR_INIT(fsm_state_status0, CAVM_GSERRX_LNX_LT_TX_FSM_STATE_STATUS0(qlm, lane));
	GSER_TRACE(QLM, "GSERR%d_LN%d_LT_TX_FSM_STATE_STATUS0\n", qlm, lane);
	GSER_TRACE(QLM, "prev1: %d\tcurrent: %d\n",
			fsm_state_status0.s.prev1,
			fsm_state_status0.s.current);

	GSER_CSR_INIT(fsm_state_status1, CAVM_GSERRX_LNX_LT_TX_FSM_STATE_STATUS1(qlm, lane));
	GSER_TRACE(QLM, "GSERR%d_LN%d_LT_TX_FSM_STATE_STATUS1\n", qlm, lane);
	GSER_TRACE(QLM, "prev2: %d\tprev3: %d\n",
			fsm_state_status1.s.prev2,
			fsm_state_status1.s.prev3);
#endif
}

static void qlm_gserr_clear_link_stat(int qlm, int lane)
{
	GSER_CSR_MODIFY(control_bcfg, CAVM_GSERRX_LANEX_CONTROL_BCFG(
		qlm, lane),
		control_bcfg.s.ln_link_stat = 0);
}

/**
 * Manually turn on or off the SERDES transmitter
 *
 * @param node	  Node to use in numa setup
 * @param qlm	   QLM to use
 * @param lane	  Which lane
 * @param enable_tx True to enable transmitter, false to disable
 */
static int qlm_gserr_tx_control(int qlm, int lane, int enable_tx)
{
	int en = (enable_tx) ? 1 : 0;

	GSER_TRACE(QLM, "GSERR%d.%d: %s TX\n", qlm, lane, (enable_tx) ? "Enable" : "Disable");
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(qlm, lane),
		c.s.ln_ctrl_tx_en = en);
	return 0;
}

/**
 * Check if PRBS Rx or Tx is enabled
 *
 * @param  qlm        Index into GSER* group
 * @param lane	  Which lane
 * @return 1 if PRBS enabled on lane, 0 if no PRBS
 */
static int qlm_gserr_prbs_chk(int qlm, int lane)
{
	/* Check if any lane has Rx or Tx PRBS enabled */
	GSER_CSR_INIT(rx_ctrl, CAVM_GSERRX_LNX_BIST_RX_CTRL(qlm, lane));
	GSER_CSR_INIT(tx_ctrl, CAVM_GSERRX_LNX_BIST_TX_CTRL(qlm, lane));
	if (rx_ctrl.s.en || tx_ctrl.s.en)
		return 1;

	return 0;
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
	.qlm_rx_adaptation_cdr_control = qlm_gserr_rx_adaption_cdr_control,
	.qlm_lane_rst = qlm_gserr_lane_rst,
	.qlm_link_training_config = qlm_gserr_link_training_config,
	.qlm_link_training_fail = qlm_gserr_link_training_fail,
	.qlm_link_training_complete = qlm_gserr_link_training_complete,
	.qlm_start_an = qlm_gserr_start_an,
	.qlm_set_phy_strap = qlm_gserr_set_phy_strap,
	.qlm_an_complete = qlm_gserr_an_complete,
	.qlm_get_link_training_status = qlm_gserr_get_link_training_status,
	.qlm_clear_link_stat = qlm_gserr_clear_link_stat,
	.qlm_prbs_chk = qlm_gserr_prbs_chk,
	.qlm_display_trace = qlm_gserr_display_trace,
};
