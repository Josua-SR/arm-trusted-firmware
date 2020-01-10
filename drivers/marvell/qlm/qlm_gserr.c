/*
 * This file was copied from BDK using gsern-bdk-to-atf.sh script.
 * Do not edit it manually!
 */

/***********************license start***********************************
* Copyright (C) 2018 Marvell International Ltd.
* SPDX-License-Identifier: BSD-3-Clause
* https://spdx.org/licenses
***********************license end**************************************/
#include <gser_internal.h>
#include <qlm/qlm.h>
#include <qlm/qlm_gsern.h>
#include <qlm/qlm_gserc.h>
#include <qlm/qlm_gserr.h>

/**
 * This define controls whether VCO_DOSC_TEMP_SKEW is set in GSERR, enabling the
 * correction of calibration based on temperature settings. This improves the
 * temperature range the GSERR SERDES operate over.
 */
#define GSER_GSERR_ENABLE_DOSC_TEMP_SKEW 0 // FIXME: Disabled for current firmware

static cavm_gserrx_common_phy_ctrl_bcfg_t qlm_gserr_get_clock_mode(int module);
static cavm_gserrx_lanex_control_bcfg_t qlm_gserr_get_lane_mode(int module, int lane);
static int qlm_gserr_change_lane_rate(int module, int lane);
static int qlm_gserr_change_phy_rate(int module);

/* Indexed by QLM number and lane */
static uint64_t prbs_errors[5][4];

/**
 * Function to return the number of lanes in the SERDES group
 *
 * @param node   Node to query
 * @param module Index into GSER* group
 *
 * @return Number of lanes (2,4)
 */
static int get_num_lanes(int module)
{
	GSER_CSR_INIT(gserrx_const, CAVM_GSERRX_CONST(module));
	return gserrx_const.s.nr_lanes;
}

/**
 * Get the SERDES state
 *
 * @param node   Node to query
 * @param qlm	Index into GSER* group
 * @param lane   Lane in GSER*
 *
 * @return State data
 */
qlm_state_lane_t qlm_gserr_get_state(int qlm, int lane)
{
	qlm_state_lane_t state = {.u = GSER_CSR_READ(CAVM_GSERRX_SCRATCHX(qlm, lane))};
	return state;
}

/**
 * For chips that don't use pin strapping, this function programs
 * the QLM to the specified mode
 *
 * @param node	 Node to use in a Numa setup
 * @param qlm	  QLM to configure
 * @param lane	 Lane inside the QLM
 * @param mode	 Desired mode
 * @param baud_mhz Desired speed
 * @param flags	Flags to specify mode specific options
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_set_mode(int qlm, int lane, qlm_modes_t mode, int baud_mhz, qlm_mode_flags_t flags)
{
	qlm_state_lane_t state = qlm_build_state(mode, baud_mhz, flags);
	int num_lanes = get_num_lanes(qlm);

	int start_lane = (lane == -1) ? 0 : lane;
	int end_lane = (lane == -1) ? num_lanes - 1 : lane;

	/* Loop through all lanes */
	for (int l = start_lane; l <= end_lane; l++)
		GSER_CSR_WRITE(CAVM_GSERRX_SCRATCHX(qlm, l), state.u);

	/* Return without changing the SERDES is the microcontroller isn't
	   running. Setup will be completed when it is brought up.*/
	GSER_CSR_INIT(gserrx_init_ctl, CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(0));
	if (gserrx_init_ctl.s.cpu_reset)
		return 0;

	/* Check if clocking needs to change */
	GSER_CSR_INIT(bcfg_old, CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(qlm));
	cavm_gserrx_common_phy_ctrl_bcfg_t bcfg_new = qlm_gserr_get_clock_mode(qlm);
	if (bcfg_old.u != bcfg_new.u)
		qlm_gserr_change_phy_rate(qlm);
	else
		GSER_TRACE(QLM, "GSERP%d: PHY rates already correct\n", qlm);

	gser_wait_usec(1000);

	/* Check if lane speed needs to change */
	for (int l = start_lane; l <= end_lane; l++)
	{
		bool need_change = (bcfg_old.u != bcfg_new.u);
		if (!need_change)
		{
			GSER_CSR_INIT(lane_old, CAVM_GSERRX_LANEX_CONTROL_BCFG(qlm, l));
			cavm_gserrx_lanex_control_bcfg_t lane_new = qlm_gserr_get_lane_mode(qlm, l);
			need_change = (lane_old.u != lane_new.u);
		}
		if (need_change)
		{
			gser_wait_usec(1000);
			qlm_gserr_change_lane_rate(qlm, l);
			gser_wait_usec(1000);
		}
		else
			GSER_TRACE(QLM, "GSERP%d.%d: Lane mode already correct\n", qlm, l);
	}

	return 0;
}

/**
 * Measure the reference clock of a QLM
 *
 * @param qlm	QLM to measure
 *
 * @return Clock rate in Hz
 */
int qlm_gserr_measure_refclock(int qlm)
{
	if (gser_is_platform(GSER_PLATFORM_ASIM))
		return REF_156MHZ;

	GSER_CSR_INIT(ctr_start, CAVM_GSERRX_REFCLK_CTR(qlm));
	uint64_t start = gser_clock_get_count(GSER_CLOCK_TIME);

	/* Wait for a short time to get a number of counts */
	gser_wait_usec(1000); /* 1ms */

	GSER_CSR_INIT(ctr_stop, CAVM_GSERRX_REFCLK_CTR(qlm));
	uint64_t stop = gser_clock_get_count(GSER_CLOCK_TIME);

	/* Calculate the rate */
	uint64_t count = ctr_stop.u - ctr_start.u;
	count *= gser_clock_get_rate(gser_numa_local(), GSER_CLOCK_TIME);
	count /= stop - start;
	return count;
}

/**
 * Put a QLM into hardware reset
 *
 * @param node   Node to use in a numa setup
 * @param qlm	QLM to use
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_reset(int qlm)
{
	int num_lanes = get_num_lanes(qlm);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(qlm, lane),
			c.s.ln_rst = 1);
	}
	gser_wait_usec(1000); /* Stay in reset of 1ms */
	for (int lane = 0; lane < num_lanes; lane++)
	{
		GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(qlm, lane),
			c.s.ln_rst = 0);
	}
	return 0;
}

/**
 * Enable PRBS on a QLM
 *
 * @param node   Node to use in a numa setup
 * @param qlm	QLM to use
 * @param prbs   PRBS mode (31, etc)
 * @param dir	Directions to enable. This is so you can enable TX and later
 *			   enable RX after TX has run for a time
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_enable_prbs(int qlm, int prbs, qlm_direction_t dir)
{
	int pattern = 1;
	switch (prbs)
	{
		case 7:
			pattern = 1;
			break;
		case 9:
			pattern = 2;
			break;
		case 11:
			pattern = 3;
			break;
		case 15:
			pattern = 4;
			break;
		case 23:
			pattern = 5;
			break;
		case 31:
			pattern = 6;
			break;
	}

	int num_lanes = get_num_lanes(qlm);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		if (dir & QLM_DIRECTION_TX)
		{
			/* No error injection */
			GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL0(qlm, lane), 0);
			/* Seed PRBS */
			GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_PRBS_CTRL0(qlm, lane), gser_rng_get_random8());
			GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_PRBS_CTRL1(qlm, lane), gser_rng_get_random8());
			GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_PRBS_CTRL2(qlm, lane), gser_rng_get_random8());
			GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_PRBS_CTRL3(qlm, lane), gser_rng_get_random8());
			/* Start transmit */
			GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_BIST_TX_CTRL(qlm, lane),
				c.s.pattern_sel = pattern;
				c.s.en = 1);
			/* Mux data from BIST generator */
			GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_TOP_DPL_TXDP_CTRL1(qlm, lane),
				c.s.tx_dmux2_sel = 1);

		}
		if (dir & QLM_DIRECTION_RX)
		{
			/* Clear error counter */
			GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_BIST_RX_CTRL(qlm, lane),
				c.s.stop_error_count = 1;
				c.s.clear_ber = 1);
			/* Allow re-lock */
			GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_BIST_RX_LOSS_LOCK_CTRL4(qlm, lane),
				c.s.stop_on_loss_lock = 0);
			/* Start receive */
			GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_BIST_RX_CTRL(qlm, lane),
				c.s.stop_error_count = 0;
				c.s.clear_ber = 0;
				c.s.pattern_sel = pattern;
				c.s.en = 1);
			prbs_errors[qlm][lane] = 0;
		}
	}
	return 0;
}

/**
 * Disable PRBS on a QLM
 *
 * @param node   Node to use in a numa setup
 * @param qlm	QLM to use
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_disable_prbs(int qlm)
{
	int num_lanes = get_num_lanes(qlm);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		/* Stop receive */
		GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_BIST_RX_CTRL(qlm, lane),
			c.s.stop_error_count = 1;
			c.s.clear_ber = 1;
			c.s.en = 0);
		/* Stop transmit */
		GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_TOP_DPL_TXDP_CTRL1(qlm, lane),
			c.s.tx_dmux2_sel = 0);
		GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_BIST_TX_CTRL(qlm, lane),
			c.s.pattern_sel = 9;
			c.s.en = 0);
		prbs_errors[qlm][lane] = 0;
	}
	return 0;
}

/**
 * Return the number of PRBS errors since PRBS started running
 *
 * @param node   Node to use in numa setup
 * @param qlm	QLM to use
 * @param lane   Which lane
 * @param clear  Clear counter after return the current value
 *
 * @return Number of errors
 */
uint64_t qlm_gserr_get_prbs_errors(int qlm, int lane, int clear)
{
	GSER_CSR_INIT(status, CAVM_GSERRX_LNX_BIST_RX_STATUS(qlm, lane));
	switch (status.s.state)
	{
		case 3: /* Lock acquired */
			/* Continue below getting counters */
			break;
		case 0: /* Off */
		case 1: /* Searching for pattern */
		case 2: /* Waiting for lock */
		case 4: /* Lock loss */
		default:
			if (clear)
				prbs_errors[qlm][lane] = 0;
			return -1; /* No lock */
	}

	/* Stop the counter so we coherent results */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_BIST_RX_CTRL(qlm, lane),
		c.s.stop_error_count = 1);

	/* Read the 24 bit counter */
	GSER_CSR_INIT(status0, CAVM_GSERRX_LNX_BIST_RX_BER_STATUS0(qlm, lane));
	GSER_CSR_INIT(status1, CAVM_GSERRX_LNX_BIST_RX_BER_STATUS1(qlm, lane));
	GSER_CSR_INIT(status2, CAVM_GSERRX_LNX_BIST_RX_BER_STATUS2(qlm, lane));

	/* Clear the counter */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_BIST_RX_CTRL(qlm, lane),
		c.s.clear_ber = 1);
	/* Restart the counter */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_BIST_RX_CTRL(qlm, lane),
		c.s.stop_error_count = 0;
		c.s.clear_ber = 0);

	/* Add up the errors */
	uint64_t errors = status0.s.bit_error_count_7_0;
	errors += status1.s.bit_error_count_15_8 << 8;
	errors += status2.s.bit_error_count_23_16 << 16;
	errors += prbs_errors[qlm][lane];

	prbs_errors[qlm][lane] = (clear) ? 0 : errors;

	return errors;
}

/**
 * Inject an error into PRBS
 *
 * @param node   Node to use in numa setup
 * @param qlm	QLM to use
 * @param lane   Which lane
 */
void qlm_gserr_inject_prbs_error(int qlm, int lane)
{
	GSER_CSR_DEFINE(ctrl0, CAVM_GSERRX_LNX_BIST_TX_BER_CTRL0(0, 0));
	GSER_CSR_DEFINE(ctrl1, CAVM_GSERRX_LNX_BIST_TX_BER_CTRL1(0, 0));
	GSER_CSR_DEFINE(ctrl2, CAVM_GSERRX_LNX_BIST_TX_BER_CTRL2(0, 0));
	GSER_CSR_DEFINE(ctrl3, CAVM_GSERRX_LNX_BIST_TX_BER_CTRL3(0, 0));
	GSER_CSR_DEFINE(ctrl4, CAVM_GSERRX_LNX_BIST_TX_BER_CTRL4(0, 0));
	GSER_CSR_DEFINE(ctrl5, CAVM_GSERRX_LNX_BIST_TX_BER_CTRL5(0, 0));
	GSER_CSR_DEFINE(ctrl6, CAVM_GSERRX_LNX_BIST_TX_BER_CTRL6(0, 0));

	ctrl0.u = 0; /* Control */
	ctrl1.u = 0; /* Timer bits 7:0 */
	ctrl2.u = 0; /* Timer bits 15:8 */
	ctrl3.u = 0; /* Error bits 7:0 */
	ctrl4.u = 0; /* Error bits 15:8 */
	ctrl5.u = 0; /* Error bits 23:16 */
	ctrl6.u = 0; /* Error bits 31:24 */

	/* Clear any previous state */
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL6(qlm, lane), ctrl6.u);
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL5(qlm, lane), ctrl5.u);
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL4(qlm, lane), ctrl4.u);
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL3(qlm, lane), ctrl3.u);
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL2(qlm, lane), ctrl2.u);
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL1(qlm, lane), ctrl1.u);
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL0(qlm, lane), ctrl0.u);

	/* LSB bit is where we inject the error */
	ctrl3.s.bit_error_field_7_0 = 1;

	/* Inject single error without the timer */
	ctrl0.s.mode = 1;

	/* Write ctrl0 last as it starts injection */
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL6(qlm, lane), ctrl6.u);
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL5(qlm, lane), ctrl5.u);
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL4(qlm, lane), ctrl4.u);
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL3(qlm, lane), ctrl3.u);
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL2(qlm, lane), ctrl2.u);
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL1(qlm, lane), ctrl1.u);
	GSER_CSR_WRITE(CAVM_GSERRX_LNX_BIST_TX_BER_CTRL0(qlm, lane), ctrl0.u);
}

/**
 * Implementation of NED Loopback with Internal BIST PRBS Generator/Checker.
 * Based on CNF95XX_B0_GSERR_Programming_v1p2.pdf
 *
 * @param node
 * @param module
 * @param lane
 *
 * @return Zero on success, negative on failure
 */
static int qlm_gserr_ned_loopback(int module, int lane, bool is_prbs)
{
	/* The steps to enable GSERR NED loopback with the PHY internal BIST PRBS
		geneator and checker are described below. */
	/* 1. Disable the firmware receive control state machine.
		Write GSERR(0..2)_LN(0..3)_FEATURE_TEST_CFG0
			RX_CTRL_DIS=1 */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_FEATURE_TEST_CFG0(module, lane),
		c.s.rx_ctrl_dis = 1);

	/* 2. Bring-up the lane to the active power state, refer to the steps in
		Section 1.4 GSERR Software Initialization. */
	/* Already done */

	/* 3. If performing the NED loopback at the PHY_RATE3 data rate of
		1.25Gbps, 1000BASE-X, or SGMII rate program the following register
		to configure the Rate 3 clocks for the receive gearbox. For all other
		data rates skip this step.
		Write GSERR(0..2)_CM0_TOP_AFE_CMCP_CTRL4
			CMCP_CLKDIV_PLL3DIV=4’h1 //cm0_clk_rate3div set divider to 10
			CMCP_CLKDIV_PLL3=4’h1 //cm0_clk_rate3 set divider to 8 */
	GSER_CSR_INIT(bcfg, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane));
	if (bcfg.s.ln_ctrl_tx_rate == 2)
	{
		GSER_CSR_MODIFY(c, CAVM_GSERRX_CM0_TOP_AFE_CMCP_CTRL4(module),
			c.s.cmcp_clkdiv_pll3div = 1;
			c.s.cmcp_clkdiv_pll3 = 1);
	}

	/* 4. Read GSERR(0..2)_LANE(0..3)_CONTROL_BCFG[LN_CTRL_TX_RATE]
		Use the LN_CTRL_TX_RATE value as an index into Table 6 below
		Write the corresponding lane GSERR(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL]
		field with the value shown in Table 6:
		GSERR(0..2)_LANE(0..3)_CONTROL_BCFG[LN_TX_CTRL_RATE] 0 1 2 4 5 6
		GSERR(0..2)_PHY0_TOP_CLOCK_LN0_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL] 2’h3 2’h2 2’h1 2’h3 2’h2 2’h1
		GSERR(0..2)_PHY0_TOP_CLOCK_LN1_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL] 2’h3 2’h2 2’h1 2’h3 2’h2 2’h1
		GSERR(0..2)_PHY0_TOP_CLOCK_LN2_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL] 2’h3 2’h2 2’h1 2’h3 2’h2 2’h1
		GSERR(0..2)_PHY0_TOP_CLOCK_LN3_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL] 2’h3 2’h2 2’h1 2’h3 2’h2 2’h1
		Table 6 – Clock override values for NED loopback with BIST Enabled */
	int ctrl_src_ovr_val;
	switch (bcfg.s.ln_ctrl_tx_rate)
	{
		case 0:
		case 4:
			ctrl_src_ovr_val = 3;
			break;
		case 1:
		case 5:
			ctrl_src_ovr_val = 2;
			break;
		case 2:
		case 6:
		default:
			ctrl_src_ovr_val = 1;
			break;
	}
	switch (lane)
	{
		case 0:
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_CLOCK_LN0_CM0_CLK_GS_MODE_CTRL0_RSVD(module),
				c.s.ctrl_src_ovr_en = 1;
				c.s.ctrl_src_ovr_val = ctrl_src_ovr_val);
			break;
		case 1:
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_CLOCK_LN1_CM0_CLK_GS_MODE_CTRL0_RSVD(module),
				c.s.ctrl_src_ovr_en = 1;
				c.s.ctrl_src_ovr_val = ctrl_src_ovr_val);
			break;
		case 2:
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_CLOCK_LN2_CM0_CLK_GS_MODE_CTRL0_RSVD(module),
				c.s.ctrl_src_ovr_en = 1;
				c.s.ctrl_src_ovr_val = ctrl_src_ovr_val);
			break;
		case 3:
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_CLOCK_LN3_CM0_CLK_GS_MODE_CTRL0_RSVD(module),
				c.s.ctrl_src_ovr_en = 1;
				c.s.ctrl_src_ovr_val = ctrl_src_ovr_val);
			break;
	}

	/* 5. Configure the clocks for the Transmit FIFO and Receiver gearbox clocks.
		Write GSERR(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXB
			CTRL_SRC_OVR_VAL=2’h2 //Select cmu clock
		Write GSERR(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXF
			CTRL_SRC_OVR_VAL=2’h2 //Select cmu clock
		Write GSERR(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXB
			CTRL_SRC_OVR_VAL=2’h2 //Select cmu clock
		Write GSERR(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXF
			CTRL_SRC_SEL=2’h2 //Select cmu clock */
	cavm_gserrx_phy0_top_clock_ln0_clk_txb_t clk_txb = {.u = 0};
	cavm_gserrx_phy0_top_clock_ln0_clk_txf_t clk_txf = {.u = 0};
	cavm_gserrx_phy0_top_clock_ln0_clk_rxb_t clk_rxb = {.u = 0};
	cavm_gserrx_phy0_top_clock_ln0_clk_rxf_t clk_rxf = {.u = 0};
	clk_txb.s.ctrl_src_ovr_val = (is_prbs) ? 2 : 3;
	clk_txf.s.ctrl_src_ovr_val = (is_prbs) ? 2 : 3;
	clk_rxb.s.ctrl_src_ovr_val = 2;
	clk_rxf.s.ctrl_src_sel = (is_prbs) ? 2 : 0;
	switch (lane)
	{
		case 0:
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN0_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN0_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN0_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN0_CLK_RXF(module), clk_rxf.u);
			break;
		case 1:
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN1_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN1_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN1_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN1_CLK_RXF(module), clk_rxf.u);
			break;
		case 2:
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN2_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN2_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN2_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN2_CLK_RXF(module), clk_rxf.u);
			break;
		case 3:
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN3_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN3_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN3_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN3_CLK_RXF(module), clk_rxf.u);
			break;
	}

	/* 6. Enable the clocks for the Transmit FIFO and Receiver gearbox clocks.
		Write GSERR(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXB
			CTRL_SRC_OVR_EN=1
		Write GSERR(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXF
			CTRL_SRC_OVR_EN=1
		Write GSERR(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXB
			CTRL_SRC_OVR_EN=1
		Note there is no CTRL_SRC_OVR_EN bit for the CLK_RXF gearbox clock, it
		is controlled only by the GSERR(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXF[CTRL_SRC_SEL]
		field in Step 5 above. */
	clk_txb.s.ctrl_src_ovr_en = 1;
	clk_txf.s.ctrl_src_ovr_en = 1;
	clk_rxb.s.ctrl_src_ovr_en = 1;
	switch (lane)
	{
		case 0:
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN0_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN0_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN0_CLK_RXB(module), clk_rxb.u);
			break;
		case 1:
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN1_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN1_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN1_CLK_RXB(module), clk_rxb.u);
			break;
		case 2:
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN2_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN2_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN2_CLK_RXB(module), clk_rxb.u);
			break;
		case 3:
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN3_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN3_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERRX_PHY0_TOP_CLOCK_LN3_CLK_RXB(module), clk_rxb.u);
			break;
	}

	/* 7. Wait 1 microsecond for the clocks to settle. */
	gser_wait_usec(1);

	/* 8. Enable NED loopback on the selected lane.
		Write GSERR(0..2)_LN(0..3)_TOP_DPL_RXDP_CTRL1
			RX_DMUX_SEL=1 */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_TOP_DPL_RXDP_CTRL1(module, lane),
		c.s.rx_dmux_sel = 1);

	/* 9. Enable the clock gaters for NED loopback. One per lane.
		Write GSERR(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CG_CTRL
			CLK_RX=1 */
	switch (lane)
	{
		case 0:
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_CLOCK_LN0_CG_CTRL(module),
				c.s.clk_rx = 1);
			break;
		case 1:
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_CLOCK_LN1_CG_CTRL(module),
				c.s.clk_rx = 1);
			break;
		case 2:
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_CLOCK_LN2_CG_CTRL(module),
				c.s.clk_rx = 1);
			break;
		case 3:
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_CLOCK_LN3_CG_CTRL(module),
				c.s.clk_rx = 1);
			break;
	}

	/* 10. Wait 1 microsecond for the clocks to settle. */
	gser_wait_usec(1);

	/* 11. Start the CGX or PHY BIST PRBS Generator */
	if (is_prbs)
	{
		if (qlm_gserr_enable_prbs(module, 32, QLM_DIRECTION_TX))
			return -1;
		GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane),
			c.s.ln_ctrl_tx_en = 1);
	}
	else
	{
		// FIXME
	}

	/* 12. Drive the LN_STAT_RXVALID signal output on the selected lane.
		Write GSERR(0..2)_LN(0..3)_TOP_LN_STAT_CTRL0
			RXVALID=1 */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_TOP_LN_STAT_CTRL0(module, lane),
		c.s.rxvalid = 1);

	/* 13. Read/Poll for LN_STAT_RXVALID=1.
		GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STAT_RXVALID=1 Rx CDR is locked
			LN_STAT_LOS Ignore the LN_STAT_LOS flag in NED loopback. */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_STAT_RXVALID, ==, 1, 10000))
		gser_error("GSERR%d.%d: Wait for GSERRX_LANEX_STATUS_BSTS[LN_STAT_RXVALID]=1 timeout\n", module, lane);

	/* 14. Complete the bring-up of the Lane in NED loopback mode. */
	if (is_prbs)
	{
		if (qlm_gserr_enable_prbs(module, 32, QLM_DIRECTION_RX))
			return -1;
	}
	else
	{
		/* Nothing needed for CGX */
	}

	/* 15. Reset the Receiver Gearbox FIFO in each lane configured for NED
		loopback. This ensures the Rx Gearbox FIFO read and write pointers are
		properly initialized.
		Step 1. For each GSERR lane configured for NED loopback first check
			that the PHY firmware has released the Rx gearbox FIFO from reset.
			i. Lane 0 Read/poll
				GSERR(0..2)_PHY0_TOP_RESET_CTRL_LN0_RSVD.RXDP_SW_RESET=1’b0
			ii. Lane 1 Read/poll
				GSERR(0..2)_PHY0_TOP_RESET_CTRL_LN1_RSVD.RXDP_SW_RESET=1’b0
			iii. Lane 2 Read/poll
				GSERR(0..2)_PHY0_TOP_RESET_CTRL_LN2_RSVD.RXDP_SW_RESET=1’b0
			iv. Lane 3 Read/poll
				GSERR(0..2)_PHY0_TOP_RESET_CTRL_LN3_RSVD.RXDP_SW_RESET=1’b0 */
	switch (lane)
	{
		case 0:
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_PHY0_TOP_RESET_CTRL_LN0_RSVD(module), GSERRX_PHY0_TOP_RESET_CTRL_LN0_RSVD_RXDP_SW_RESET, ==, 0, 10000))
				gser_error("GSERR%d.%d: Wait for GSERRX_PHY0_TOP_RESET_CTRL_LN0_RSVD[rxdp_sw_reset]=0 timeout\n", module, lane);
			break;
		case 1:
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module), GSERRX_PHY0_TOP_RESET_CTRL_LN1_RSVD_RXDP_SW_RESET, ==, 0, 10000))
				gser_error("GSERR%d.%d: Wait for GSERRX_PHY0_TOP_RESET_CTRL_LN1_RSVD[rxdp_sw_reset]=0 timeout\n", module, lane);
			break;
		case 2:
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module), GSERRX_PHY0_TOP_RESET_CTRL_LN2_RSVD_RXDP_SW_RESET, ==, 0, 10000))
				gser_error("GSERR%d.%d: Wait for GSERRX_PHY0_TOP_RESET_CTRL_LN2_RSVD[rxdp_sw_reset]=0 timeout\n", module, lane);
			break;
		case 3:
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module), GSERRX_PHY0_TOP_RESET_CTRL_LN3_RSVD_RXDP_SW_RESET, ==, 0, 10000))
				gser_error("GSERR%d.%d: Wait for GSERRX_PHY0_TOP_RESET_CTRL_LN3_RSVD[rxdp_sw_reset]=0 timeout\n", module, lane);
			break;
	}

	/*  Step 2. For each GSERR lane configured for NED loopback disable the Rx
			gearbox FIFO, then reset the FIFO, then re-enable the FIFO.
			i. Lane 0:
				Write GSERR(0..2)_LN(0)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=0
				Write GSERR(0..2)_PHY0_TOP_RESET_CTRL_LN0_RSVD.RXDP_SWRESET=1
				Write GSERR(0..2)_PHY0_TOP_RESET_CTRL_LN0_RSVD.RXDP_SWRESET=0
				Write GSERR(0..2)_LN(0)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=1
			ii. Lane 1:
				Write GSERR(0..2)_LN(1)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=0
				Write GSERR(0..2)_PHY0_TOP_RESET_CTRL_LN1_RSVD.RXDP_SWRESET=1
				Write GSERR(0..2)_PHY0_TOP_RESET_CTRL_LN1_RSVD.RXDP_SWRESET=0
				Write GSERR(0..2)_LN(1)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=1
			iii. Lane 2:
				Write GSERR(0..2)_LN(2)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=0
				Write GSERR(0..2)_PHY0_TOP_RESET_CTRL_LN2_RSVD.RXDP_SWRESET=1
				Write GSERR(0..2)_PHY0_TOP_RESET_CTRL_LN2_RSVD.RXDP_SWRESET=0
				Write GSERR(0..2)_LN(2)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=1
			iv. Lane 3:
				Write GSERR(0..2)_LN(3)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=0
				Write GSERR(0..2)_PHY0_TOP_RESET_CTRL_LN3_RSVD.RXDP_SWRESET=1
				Write GSERR(0..2)_PHY0_TOP_RESET_CTRL_LN3_RSVD.RXDP_SWRESET=0
				Write GSERR(0..2)_LN(3)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=1 */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_TOP_DPL_RXDP_CTRL1(module, lane),
		c.s.rx_fifo_en = 0);
	switch (lane)
	{
		case 0:
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_RESET_CTRL_LN0_RSVD(module),
				c.s.rxdp_sw_reset = 1);
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_RESET_CTRL_LN0_RSVD(module),
				c.s.rxdp_sw_reset = 0);
			break;
		case 1:
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module),
				c.s.rxdp_sw_reset = 1);
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module),
				c.s.rxdp_sw_reset = 0);
			break;
		case 2:
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_RESET_CTRL_LN2_RSVD(module),
				c.s.rxdp_sw_reset = 1);
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_RESET_CTRL_LN2_RSVD(module),
				c.s.rxdp_sw_reset = 0);
			break;
		case 3:
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_RESET_CTRL_LN3_RSVD(module),
				c.s.rxdp_sw_reset = 1);
			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_RESET_CTRL_LN3_RSVD(module),
				c.s.rxdp_sw_reset = 0);
			break;
	}
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_TOP_DPL_RXDP_CTRL1(module, lane),
		c.s.rx_fifo_en = 1);

	/* 16. Start the BIST PRBS checker, refer to Section 1.8.2 PRBS Checker.
		The BIST checker should now be receiving the BIST PRBS generator
		transmit data in NED loopback. Clear the PRBS Pattern Checker after
		resetting the Rx gearbox FIFO in step 15, as the Rx gearbox FIFO reset
		has probably caused bit errors while the FIFO was reset and the BIST
		test pattern was interrupted.
		Write GSERR(0..2)_LN(0..3)_BIST_RX_CTRL
			CLEAR_BER=1
		Write GSERR(0..2)_LN(0..3)_BIST_RX_CTRL
			CLEAR_BER=0 */
	if (is_prbs)
	{
		GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_BIST_RX_CTRL(module, lane),
			c.s.clear_ber = 1);
		GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_BIST_RX_CTRL(module, lane),
			c.s.clear_ber = 0);
	}
	return 0;
}

/**
 * Enable shallow loopback on a QLM
 *
 * @param node   Node to use in a numa setup
 * @param qlm	QLM to use
 * @param loop   Type of loopback. Not all QLMs support all modes
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_enable_loop(int qlm, qlm_loop_t loop)
{
	int near_end_analog = 0;
	int far_end_analog = 0;
	int near_end_digital = 0;

	switch (loop)
	{
		case QLM_LOOP_DISABLED:  /* No loopback */
			break;
		case QLM_LOOP_SHALLOW: /* Loop external data RX->TX (Not supported on CN8XXX) */
			far_end_analog = 1;
			break;
		case QLM_LOOP_NEAR_END: /* Loop internal data TX->RX in analog domain (Not supported on CN8XXX) */
			near_end_analog = 1;
			break;
		case QLM_LOOP_CORE: /* Loop internal data TX->RX in digital domain */
			near_end_digital = 1;
			break;
	}

	int num_lanes = get_num_lanes(qlm);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_TOP_AFE_LOOPBACK_CTRL(qlm, lane),
			c.s.loopback_nea_en = near_end_analog;
			c.s.loopback_fea_en = far_end_analog);
		if (near_end_digital)
			qlm_gserr_ned_loopback(qlm, lane, false);
	}
	return 0;
}

/**
 * Configure the TX tuning parameters for a QLM lane
 *
 * @param node	  Node to configure
 * @param qlm	   QLM to configure
 * @param lane	  Lane to configure
 * @param tx_cmain  Transmitter Main (C0) equalizer tap coefficient value.
 *				  Programs the SerDes transmitter Main tap. Valid range is
 *				  0x30 to 0x18.
 * @param tx_cpre   Transmitter Pre (C-1) equalizer tap coefficient value.
 *				  Programs the transmitter Pre tap. Valid range is 0 to 0x10.
 * @param tx_cpost  Transmitter Post (C+1) equalizer tap coefficient value.
 *				  Programs the transmitter Post tap. Valid range is 0 to
 *				  0x10.
 * @param tx_bs	 TX bias/swing selection. This setting only takes effect if
 *				  [TX_CSPD] is deasserted; with [TX_CSPD] asserted the
 *				  bias/swing control setting seen in the analog bias generator
 *				  is zero.
 *				  Typical override values would be:
 *				  42 = Nominal 1.0V p-p transmit amplitude.
 *				  52 = Nominal 1.2V p-p transmit amplitude.
 *				  The maximum usable value without transmitted waveform
 *				  distortion depends primarily on voltage, secondarily on
 *				  process corner and temperature, but is at least 52. There is
 *				  no minimum setting based on transmitter distortion, only
 *				  that set by the receiver.
 * @param tx_unused Currently unused
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_tune_lane_tx(int qlm, int lane, int tx_cmain, int tx_cpre, int tx_cpost, int tx_unused1, int tx_unused2)
{
	GSER_CSR_INIT(ctrl0, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_CTRL0(qlm, lane));
	GSER_CSR_INIT(status0, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_STATUS0(qlm, lane));
	if (ctrl0.s.req || status0.s.ack)
	{
		gser_error("GSERR%d.%d: TXEQ coefficient request in progress\n", qlm, lane);
		return -1;
	}
	int orig_tx_cmain = -1;
	int orig_tx_cpre = -1;
	int orig_tx_cpost = -1;
	if (qlm_gserr_get_tune_lane_tx(qlm, lane, &orig_tx_cmain, &orig_tx_cpre, &orig_tx_cpost, &tx_unused1, &tx_unused2))
	{
		gser_error("GSERR%d.%d: Failed to read previous settings\n", qlm, lane);
		return -1;
	}

	if (tx_cmain == -1)
		tx_cmain = orig_tx_cmain;
	if (tx_cpre == -1)
		tx_cpre = orig_tx_cpre;
	if (tx_cpost == -1)
		tx_cpost = orig_tx_cpost;

	/* Load the new value */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_CTRL1(qlm, lane),
		c.s.txeq_c1 = tx_cpost);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_CTRL3(qlm, lane),
		c.s.txeq_cm1 = tx_cpre);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_CTRL5(qlm, lane),
		c.s.drv_swing = tx_cmain);
	/* Request an update */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_CTRL0(qlm, lane),
		c.s.req = 1);
	/* Wait for ACK */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_STATUS0(qlm, lane), GSERRX_LNX_DRV_REFCLK_TXEQ_STATUS0_ACK, ==, 1, 10000))
	{
		gser_error("GSERR%d.%d: TXEQ coefficient request timeout\n", qlm, lane);
		return -1;
	}
	/* Clear update request */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_CTRL0(qlm, lane),
		c.s.req = 0);

	return 0;
}

/**
 * Get the TX tuning parameters for a QLM lane
 *
 * @param node	  Node to configure
 * @param qlm	   QLM to configure
 * @param lane	  Lane to configure
 * @param tx_cmain  Transmitter Main (C0) equalizer tap coefficient value.
 *				  Programs the SerDes transmitter Main tap. Valid range is
 *				  0x30 to 0x18.
 * @param tx_cpre   Transmitter Pre (C-1) equalizer tap coefficient value.
 *				  Programs the transmitter Pre tap. Valid range is 0 to 0x10.
 * @param tx_cpost  Transmitter Post (C+1) equalizer tap coefficient value.
 *				  Programs the transmitter Post tap. Valid range is 0 to
 *				  0x10.
 * @param tx_bs	 TX bias/swing selection. This setting only takes effect if
 *				  [TX_CSPD] is deasserted; with [TX_CSPD] asserted the
 *				  bias/swing control setting seen in the analog bias generator
 *				  is zero.
 *				  Typical override values would be:
 *				  42 = Nominal 1.0V p-p transmit amplitude.
 *				  52 = Nominal 1.2V p-p transmit amplitude.
 *				  The maximum usable value without transmitted waveform
 *				  distortion depends primarily on voltage, secondarily on
 *				  process corner and temperature, but is at least 52. There is
 *				  no minimum setting based on transmitter distortion, only
 *				  that set by the receiver.
 * @param tx_unused Currently unused
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_get_tune_lane_tx(int qlm, int lane, int *tx_cmain, int *tx_cpre, int *tx_cpost, int *tx_bs, int *tx_unused)
{
	GSER_CSR_INIT(status1, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_STATUS1(qlm, lane));
	GSER_CSR_INIT(status2, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_STATUS2(qlm, lane));
	GSER_CSR_INIT(status3, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_STATUS3(qlm, lane));
	*tx_cmain = status3.s.txeq_c0;
	*tx_cpre = status2.s.txeq_cm1;
	*tx_cpost = status1.s.txeq_c1;
	*tx_bs = -1;
	*tx_unused = -1;
	return 0;
}

/**
 * Display the current settings of a QLM lane
 *
 * @param node	 Node the QLM is on
 * @param qlm	  QLM to display
 * @param qlm_lane Lane to use
 * @param show_tx  Display TX parameters
 * @param show_rx  Display RX parameters
 */
/*
 * Modified in qlm-gserr.patch applied by gsern-update script in SDK
 */
//void qlm_gserr_display_settings(int qlm, int qlm_lane, bool show_tx, bool show_rx)
//{
void qlm_gserr_display_settings(int qlm, int qlm_lane, bool show_tx, bool show_rx, char *buf, int size)
{
#define printf(...) \
	do {								\
		if (buf == NULL) {					\
			printf(__VA_ARGS__);				\
		} else {						\
			int _chars = snprintf(buf, size, __VA_ARGS__);	\
			buf += _chars;					\
			size -= _chars;					\
		}							\
	} while(0)

	printf("N0.GSERR%d Lane %d:\n", qlm, qlm_lane);
	if (show_tx)
	{
		printf("  CMU Status:\n");
		GSER_CSR_INIT(cm0_top_phy_if_status, CAVM_GSERRX_CM0_TOP_PHY_IF_STATUS(qlm));
		GSER_CSR_INIT(bcfg, CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(qlm));
		printf("	CMU OK=%d, PHY Rate1=0x%x, Rate2=0x%x\n",
			cm0_top_phy_if_status.s.cmu_ok, bcfg.s.phy_ctrl_rate1, bcfg.s.phy_ctrl_rate2);
		GSER_CSR_INIT(status1, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_STATUS1(qlm, qlm_lane));
		GSER_CSR_INIT(status2, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_STATUS2(qlm, qlm_lane));
		GSER_CSR_INIT(status3, CAVM_GSERRX_LNX_DRV_REFCLK_TXEQ_STATUS3(qlm, qlm_lane));
		printf("  Transmitter Status:\n");
		printf("	PRE=%d, MAIN=%d, POST=%d\n", status2.s.txeq_cm1, status3.s.txeq_c0, status1.s.txeq_c1);
	}
	if (show_rx)
	{
		/* Read LEQ */
		int att = -1;
		int apg = -1;
		int lfg = -1;
		int hfg = -1;
		int mbf = -1;
		int mbg = -1;
		switch (qlm_lane)
		{
			case 0:
				att = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_PLE_ATT_O_READ_VAL_2_0_RSVD(qlm));
				apg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_GN_APG_O_READ_VAL_2_0_RSVD(qlm));
				lfg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_LFG_O_READ_VAL_4_0_RSVD(qlm));
				hfg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_HFG_O_READ_VAL_4_0_RSVD(qlm));
				mbf = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_MBF_O_READ_VAL_3_0_RSVD(qlm));
				mbg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_MBG_O_READ_VAL_3_0_RSVD(qlm));
				break;
			case 1:
				att = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_PLE_ATT_O_READ_VAL_2_0_RSVD(qlm));
				apg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_GN_APG_O_READ_VAL_2_0_RSVD(qlm));
				lfg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_LFG_O_READ_VAL_4_0_RSVD(qlm));
				hfg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_HFG_O_READ_VAL_4_0_RSVD(qlm));
				mbf = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_MBF_O_READ_VAL_3_0_RSVD(qlm));
				mbg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_MBG_O_READ_VAL_3_0_RSVD(qlm));
				break;
			case 2:
				att = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_PLE_ATT_O_READ_VAL_2_0_RSVD(qlm));
				apg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_GN_APG_O_READ_VAL_2_0_RSVD(qlm));
				lfg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_LFG_O_READ_VAL_4_0_RSVD(qlm));
				hfg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_HFG_O_READ_VAL_4_0_RSVD(qlm));
				mbf = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_MBF_O_READ_VAL_3_0_RSVD(qlm));
				mbg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_MBG_O_READ_VAL_3_0_RSVD(qlm));
				break;
			case 3:
				att = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_PLE_ATT_O_READ_VAL_2_0_RSVD(qlm));
				apg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_GN_APG_O_READ_VAL_2_0_RSVD(qlm));
				lfg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_LFG_O_READ_VAL_4_0_RSVD(qlm));
				hfg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_HFG_O_READ_VAL_4_0_RSVD(qlm));
				mbf = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_MBF_O_READ_VAL_3_0_RSVD(qlm));
				mbg = GSER_CSR_READ(CAVM_GSERRX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_MBG_O_READ_VAL_3_0_RSVD(qlm));
				break;
		}
		printf("  Receiver Status:\n");
		printf("	ATT=%d, APG=%d, LFG=%d, HFG=%d, MBF=%d, MBG=%d\n",
			att, apg, lfg, hfg, mbf, mbg);

		/* Read DFE taps */
		GSER_CSR_INIT(tap1_even, CAVM_GSERRX_LNX_DFE_REFCLK_TAP_VAL_STATUS0(qlm, qlm_lane));
		GSER_CSR_INIT(tap1_odd, CAVM_GSERRX_LNX_DFE_REFCLK_TAP_VAL_STATUS1(qlm, qlm_lane));
		GSER_CSR_INIT(tap2, CAVM_GSERRX_LNX_DFE_REFCLK_TAP_VAL_STATUS2(qlm, qlm_lane));
		GSER_CSR_INIT(tap3, CAVM_GSERRX_LNX_DFE_REFCLK_TAP_VAL_STATUS3(qlm, qlm_lane));
		GSER_CSR_INIT(tap4, CAVM_GSERRX_LNX_DFE_REFCLK_TAP_VAL_STATUS4(qlm, qlm_lane));
		GSER_CSR_INIT(tap5, CAVM_GSERRX_LNX_DFE_REFCLK_TAP_VAL_STATUS5(qlm, qlm_lane));
		GSER_CSR_INIT(tap6, CAVM_GSERRX_LNX_DFE_REFCLK_TAP_VAL_STATUS6(qlm, qlm_lane));
		GSER_CSR_INIT(tap7, CAVM_GSERRX_LNX_DFE_REFCLK_TAP_VAL_STATUS7(qlm, qlm_lane));
		GSER_CSR_INIT(tap8, CAVM_GSERRX_LNX_DFE_REFCLK_TAP_VAL_STATUS8(qlm, qlm_lane));
		int tap1_even_val = (tap1_even.s.tap1_even_polarity) ? tap1_even.s.tap1_even : -tap1_even.s.tap1_even;
		int tap1_odd_val = (tap1_odd.s.tap1_odd_polarity) ? tap1_odd.s.tap1_odd : -tap1_odd.s.tap1_odd;
		int tap2_val = (tap2.s.tap2_polarity) ? tap2.s.tap2 : -tap2.s.tap2;
		int tap3_val = (tap3.s.tap3_polarity) ? tap3.s.tap3 : -tap3.s.tap3;
		int tap4_val = (tap4.s.tap4_polarity) ? tap4.s.tap4 : -tap4.s.tap4;
		int tap5_val = (tap5.s.tap5_polarity) ? tap5.s.tap5 : -tap5.s.tap5;
		int tap6_val = (tap6.s.tap6_polarity) ? tap6.s.tap6 : -tap6.s.tap6;
		int tap7_val = (tap7.s.tap7_polarity) ? tap7.s.tap7 : -tap7.s.tap7;
		int tap8_val = (tap8.s.tap8_polarity) ? tap8.s.tap8 : -tap8.s.tap8;
		printf("	TAP1(EVEN)=%d, TAP1(ODD)=%d, TAP2=%d, TAP3=%d, TAP4=%d, TAP5=%d, TAP6=%d, TAP7=%d, TAP8=%d\n",
			tap1_even_val, tap1_odd_val, tap2_val, tap3_val, tap4_val, tap5_val, tap6_val, tap7_val, tap8_val);
	}
#undef printf
}

/**
 * Issue a mailbox command to the serdes firmware
 *
 * @param node   Node to access
 * @param qlm	QLM to access
 * @param cmd	Command to issue
 * @param args   Arguments to command. LSB = arg0, MSB = arg7
 *
 * @return Command status, or -1 on failure
 */
static int mailbox_command(int qlm, uint8_t cmd, uint64_t args)
{
	GSER_CSR_INIT(flag, CAVM_GSERRX_PHY0_MB_CMD_FLAG(qlm));
	if (flag.s.data)
	{
		gser_error("GSERR%d: Mailbox already has a command\n", qlm);
		return -1;
	}

	GSER_CSR_INIT(rsp_flag, CAVM_GSERRX_PHY0_MB_RSP_FLAG(qlm));
	if (rsp_flag.s.data)
	{
		gser_error("GSERR%d: Mailbox already has a response\n", qlm);
		return -1;
	}

	GSER_CSR_WRITE(CAVM_GSERRX_PHY0_MB_CMD_DATA0(qlm), gser_extract(args, 0, 8));
	GSER_CSR_WRITE(CAVM_GSERRX_PHY0_MB_CMD_DATA1(qlm), gser_extract(args, 8, 8));
	GSER_CSR_WRITE(CAVM_GSERRX_PHY0_MB_CMD_DATA2(qlm), gser_extract(args, 16, 8));
	GSER_CSR_WRITE(CAVM_GSERRX_PHY0_MB_CMD_DATA3(qlm), gser_extract(args, 24, 8));
	GSER_CSR_WRITE(CAVM_GSERRX_PHY0_MB_CMD_DATA4(qlm), gser_extract(args, 32, 8));
	GSER_CSR_WRITE(CAVM_GSERRX_PHY0_MB_CMD_DATA5(qlm), gser_extract(args, 40, 8));
	GSER_CSR_WRITE(CAVM_GSERRX_PHY0_MB_CMD_DATA6(qlm), gser_extract(args, 48, 8));
	GSER_CSR_WRITE(CAVM_GSERRX_PHY0_MB_CMD_DATA7(qlm), gser_extract(args, 56, 8));
	GSER_CSR_WRITE(CAVM_GSERRX_PHY0_MB_CMD(qlm), cmd);

	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_PHY0_MB_CMD_FLAG(qlm), GSERRX_PHY0_MB_CMD_FLAG_DATA, ==, 0, 10000))
	{
		gser_error("GSERR%d: Mailbox stuck on command\n", qlm);
		return -1;
	}
	return 0;
}

/**
 * Read a mailbox response from the serdes firmware
 *
 * @param node   Node to access
 * @param qlm	QLM to access
 * @param arg0   First 8 arguments of the response. LSB = data0, MSB = data7
 * @param arg1   Second 8 arguments of the response. LSB = data8, MSB = data15
 *
 * @return Response status, or -1 on failure
 */
static int mailbox_response(int qlm, uint64_t *arg0, uint64_t *arg1)
{
	/* Wait for a response */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_PHY0_MB_RSP_FLAG(qlm), GSERRX_PHY0_MB_RSP_FLAG_DATA, ==, 1, 10000))
	{
		gser_error("GSERR%d: Mailbox has no response\n", qlm);
		return -1;
	}

	/* Read the response */
	GSER_CSR_INIT(rsp, CAVM_GSERRX_PHY0_MB_RSP(qlm));
	GSER_CSR_INIT(data0, CAVM_GSERRX_PHY0_MB_RSP_DATA0(qlm));
	GSER_CSR_INIT(data1, CAVM_GSERRX_PHY0_MB_RSP_DATA1(qlm));
	GSER_CSR_INIT(data2, CAVM_GSERRX_PHY0_MB_RSP_DATA2(qlm));
	GSER_CSR_INIT(data3, CAVM_GSERRX_PHY0_MB_RSP_DATA3(qlm));
	GSER_CSR_INIT(data4, CAVM_GSERRX_PHY0_MB_RSP_DATA4(qlm));
	GSER_CSR_INIT(data5, CAVM_GSERRX_PHY0_MB_RSP_DATA5(qlm));
	GSER_CSR_INIT(data6, CAVM_GSERRX_PHY0_MB_RSP_DATA6(qlm));
	GSER_CSR_INIT(data7, CAVM_GSERRX_PHY0_MB_RSP_DATA7(qlm));
	GSER_CSR_INIT(data8, CAVM_GSERRX_PHY0_MB_RSP_DATA8(qlm));
	GSER_CSR_INIT(data9, CAVM_GSERRX_PHY0_MB_RSP_DATA9(qlm));
	GSER_CSR_INIT(data10, CAVM_GSERRX_PHY0_MB_RSP_DATA10(qlm));
	GSER_CSR_INIT(data11, CAVM_GSERRX_PHY0_MB_RSP_DATA11(qlm));
	GSER_CSR_INIT(data12, CAVM_GSERRX_PHY0_MB_RSP_DATA12(qlm));
	GSER_CSR_INIT(data13, CAVM_GSERRX_PHY0_MB_RSP_DATA13(qlm));
	GSER_CSR_INIT(data14, CAVM_GSERRX_PHY0_MB_RSP_DATA14(qlm));
	GSER_CSR_INIT(data15, CAVM_GSERRX_PHY0_MB_RSP_DATA15(qlm));

	/* Tell firmware we read the response */
	GSER_CSR_WRITE(CAVM_GSERRX_PHY0_MB_RSP_FLAG(qlm), 1);

	/* Package and return the response */
	*arg0 = (uint64_t)data0.s.data;
	*arg0 |= (uint64_t)data1.s.data << 8;
	*arg0 |= (uint64_t)data2.s.data << 16;
	*arg0 |= (uint64_t)data3.s.data << 24;
	*arg0 |= (uint64_t)data4.s.data << 32;
	*arg0 |= (uint64_t)data5.s.data << 40;
	*arg0 |= (uint64_t)data6.s.data << 48;
	*arg0 |= (uint64_t)data7.s.data << 56;
	*arg1 = (uint64_t)data8.s.data;
	*arg1 |= (uint64_t)data9.s.data << 8;
	*arg1 |= (uint64_t)data10.s.data << 16;
	*arg1 |= (uint64_t)data11.s.data << 24;
	*arg1 |= (uint64_t)data12.s.data << 32;
	*arg1 |= (uint64_t)data13.s.data << 40;
	*arg1 |= (uint64_t)data14.s.data << 48;
	*arg1 |= (uint64_t)data15.s.data << 56;
	return rsp.s.data;
}

/**
 * Perform RX equalization on a QLM
 *
 * @param node	 Node the QLM is on
 * @param qlm	  QLM to perform RX equalization on
 * @param qlm_lane Lane to use, or -1 for all lanes
 *
 * @return Zero on success, negative if any lane failed RX equalization
 */
int qlm__gserr_rx_equalization(int qlm, int qlm_lane)
{
	if (gser_is_platform(GSER_PLATFORM_ASIM))
		return 0;

	int result = 0;
	int num_lanes = get_num_lanes(qlm);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		if ((qlm_lane != -1) && (qlm_lane != lane))
			continue;
		GSER_CSR_INIT(rxdp_ctrl1, CAVM_GSERRX_LNX_TOP_DPL_RXDP_CTRL1(qlm, lane));
		GSER_CSR_INIT(bsts, CAVM_GSERRX_LANEX_STATUS_BSTS(qlm, lane));
		if (bsts.s.ln_stat_los)
		{
			/* No signal is OK in NED loopback */
			if (!rxdp_ctrl1.s.rx_dmux_sel)
			{
				GSER_TRACE(QLM, "N0.GSERR%d.%d: Loss of signal\n", qlm, lane);
				result = -1;
				continue;
			}
		}
		else
			GSER_TRACE(QLM, "N0.GSERR%d.%d: RX signal detected\n", qlm, lane);

		if (!bsts.s.ln_stat_rxvalid)
		{
			GSER_TRACE(QLM, "N0.GSERR%d.%d: RX invalid, CDR not locked\n", qlm, lane);
			result = -1;
			continue;
		}
		else
			GSER_TRACE(QLM, "N0.GSERR%d.%d: RX valid, CDR locked\n", qlm, lane);

		if (rxdp_ctrl1.s.rx_dmux_sel)
		{
			GSER_TRACE(QLM, "N0.GSERR%d.%d: NED loopback, skipping equalization\n", qlm, lane);
			continue;
		}

		GSER_TRACE(QLM, "N0.GSERR%d.%d: Lane TXDP Clock Phase Calibration starting\n", qlm, lane);
		if (mailbox_command(qlm, 0x81, qlm_lane))
		{
			gser_error("N0.GSERR%d.%d: Lane TXDP Clock Phase Calibration failed to issue\n", qlm, lane);
			result = -1;
			continue;
		}
		uint64_t arg0 = 0;
		uint64_t arg1 = 0;
		int status = mailbox_response(qlm, &arg0, &arg1);
		switch (status)
		{
			case 0x00: /* Calibration OK */
				GSER_TRACE(QLM, "N0.GSERR%d.%d: Lane TXDP Clock Phase Calibration: %lu\n", qlm, lane, arg0 & 0xff);
				break;
			case 0x01: /* Calibration Railed Low */
				gser_error("N0.GSERR%d.%d: Lane TXDP Clock Phase Calibration Railed Low\n", qlm, lane);
				result = -1;
				break;
			case 0x02: /* Calibration Railed High */
				gser_error("N0.GSERR%d.%d: Lane TXDP Clock Phase Calibration Railed High\n", qlm, lane);
				result = -1;
				break;
			default:
				gser_error("N0.GSERR%d.%d: Lane TXDP Clock Phase Calibration illegal response %d\n", qlm, lane, status);
				result = -1;
				break;
		}
	}
	return result;
}

/**
 * Capture an eye diagram for the given QLM lane. The output data is written
 * to "eye".
 *
 * @param node	  Node to use in numa setup
 * @param qlm	   QLM to use
 * @param qlm_lane  Which lane
 * @param show_data Set to non-zero to have the eye capture dump the raw eye data as
 *				  it is captured
 * @param eye	   Output eye data
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_eye_capture(int qlm, int lane, int show_data, qlm_eye_t *eye_data)
{
	int x_min = 0;
	int x_max = 127;
	int x_step = 1;
	int y_min = -255;
	int y_max = 255;
	int y_step = 8;
	int ber = 16; /* Base 2, not base 10. Must be 8, 16, or 32 */
	uint64_t resp0;
	uint64_t resp1;

	eye_data->width = 0;
	eye_data->height = 0;

	int y_points = (y_max - y_min + 1) / y_step;
	int x_points = (x_max - x_min + 1) / x_step;
	if (y_points > 64) /* Limit from static struct */
	{
		gser_error("GSERR%d: Eye too high\n", qlm);
		return -1;
	}
	if (x_points > 128) /* Limit from static struct */
	{
		gser_error("GSERR%d: Eye too high\n", qlm);
		return -1;
	}
	x_max = x_min + x_step * (x_points - 1) - 1;
	y_max = y_min + y_step * (y_points - 1);

	if (show_data)
	{
		printf("# Extensive 1D Rx Eye on GSERR%d Lane %d\n", qlm, lane);
		printf("# Cycle Bits: %lu\n", 1ul << ber);
		printf("# Start Time Offset: %d; End Time Offset: %d; Time Step: %d\n", x_min, x_max, x_step);
		printf("# Start Voltage Offset: %d; End Voltage Offset: %d; Voltage Step: %d\n", y_min, y_max, y_step);
		printf("%5s %5s %s\n", "V", "T", "Errors");
	}
	else
		printf("Capturing eye...\n");

	uint64_t cmd_arg = (uint64_t)lane; /* data 0 */
	cmd_arg |= (uint64_t)x_min << 8; /* data 1 */
	cmd_arg |= (uint64_t)x_max << 16; /* data 2 */
	cmd_arg |= (uint64_t)x_step << 24; /* data 3 */
	cmd_arg |= (uint64_t)-y_min << 32; /* data 4 */
	cmd_arg |= (uint64_t)y_max << 40; /* data 5 */
	int signs = ((y_min < 0) ? 1 : 0) | ((y_max < 0) ? 2 : 0);
	cmd_arg |= (uint64_t)signs << 48; /* data 6 */
	cmd_arg |= (uint64_t)y_step << 56; /* data 7 */

	if (mailbox_command(qlm, 0x83, cmd_arg) != 0)
		return -1;
	if (mailbox_response(qlm, &resp0, &resp1) != 0)
	{
		gser_error("GSERR%d: Eye setup 1 failed\n", qlm);
		return -1;
	}

	cmd_arg = lane; /* data 0 */
	uint64_t ber_limit = gser_build_mask(ber);
	cmd_arg |= gser_extract(ber_limit, 32, 8) << 8; /* data 1 */
	cmd_arg |= gser_extract(ber_limit, 24, 8) << 16; /* data 2 */
	cmd_arg |= gser_extract(ber_limit, 16, 8) << 24; /* data 3 */
	cmd_arg |= gser_extract(ber_limit, 8, 8) << 32; /* data 4 */
	cmd_arg |= gser_extract(ber_limit, 0, 8) << 40; /* data 5 */

	if (mailbox_command(qlm, 0x84, cmd_arg))
		return -1;
	if (mailbox_response(qlm, &resp0, &resp1) != 0)
	{
		gser_error("GSERR%d: Eye setup 2 failed\n", qlm);
		return -1;
	}

	cmd_arg = lane; /* data 0 */
	cmd_arg = 0 << 16; /* data 2, sweep mode */

	if (mailbox_command(qlm, 0x82, cmd_arg))
		return -1;

	int x = 0;
	int y = 0;
	int rsp = 0;
	while (rsp != 1)
	{
		/* read 128 bits of response */
		rsp = mailbox_response(qlm, &resp0, &resp1);
		/* 0 = more data, 1 = done, others are errors */
		if ((rsp != 0) && (rsp != 1))
		{
			gser_error("GSERR%d: Eye failed (%d)\n", qlm, rsp);
			return -1;
		}
		/* Make a 128 bit number from response for easy processing */
		__uint128_t r = resp0;
		r |= (__uint128_t)resp1 << 64;
		/* Loop through all response bits. This assumes 128 divides evenly by BER */
		for (int bits = 0; bits < 128; bits += ber)
		{
			/* Extract this sample. It is encoded BE */
			uint64_t sample = r & gser_build_mask(ber);
			/* Move to the next BER bits */
			r >>= ber;
			/* Convert and store the sample */
			sample = gser_be16_to_cpu(sample);
			eye_data->data[y][x] = sample;
			if (show_data)
				printf("%5d %5d %llu\n", y * y_step + y_min, x * x_step + x_min, sample);
			/* Move to next */
			y++;
			if (y >= y_points)
			{
				y = 0;
				x++;
				if (x >= x_points)
				{
					if (rsp != 1)
					{
						gser_error("GSERR%d: Too many response\n", qlm);
						return -1;
					}
					break; /* Bit loop */
				}
			}
		}
	}
	eye_data->width = x_points;
	eye_data->height = y_points;
	return 0;
}

/**
 * Manually turn on or off the SERDES transmitter
 *
 * @param node	  Node to use in numa setup
 * @param qlm	   QLM to use
 * @param lane	  Which lane
 * @param enable_tx True to enable transmitter, false to disable
 */
void qlm_gserr_tx_control(int qlm, int lane, bool enable_tx)
{
	GSER_TRACE(QLM, "GSERR%d.%d: %s TX\n", qlm, lane, (enable_tx) ? "Enable" : "Disable");
	int en = (enable_tx) ? 1 : 0;
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(qlm, lane),
		c.s.ln_ctrl_tx_en = en);
}

/**
 * Poll serdes for errors removed by gser-update script (qlm.sed)
 *
 * @param node   Node to poll
 * @param module  SERDES group to poll
 */
//static void qlm_gserr_poll_error(int module)
//{
//	/* Return if no PHY error is detected */
//	GSER_CSR_INIT(phy_status_bsts, CAVM_GSERRX_COMMON_PHY_STATUS_BSTS(module));
//	if (!phy_status_bsts.s.phy_error)
//		return;
//	gser_warn("GSERR%d: COMMON_PHY_STATUS_BSTS[PHY_ERROR] is set\n", module);
//
//	/* PHY Errors */
//	GSER_CSR_INIT(top_err_ctrl0, CAVM_GSERRX_PHY0_TOP_ERR_CTRL0(module));
//	if (top_err_ctrl0.s.err_o)
//	{
//		GSER_CSR_INIT(top_err_ctrl1, CAVM_GSERRX_PHY0_TOP_ERR_CTRL1(module));
//		GSER_CSR_INIT(top_err_ctrl2, CAVM_GSERRX_PHY0_TOP_ERR_CTRL2(module));
//		gser_warn("GSERR%d: PHY0_TOP_ERR 0x%02x%02x\n",
//			module, top_err_ctrl2.s.err_code_15_8, top_err_ctrl1.s.err_code_7_0);
//		return;
//	}
//
//	/* CM0 Errors */
//	GSER_CSR_INIT(cm0_top_phy_if_status, CAVM_GSERRX_CM0_TOP_PHY_IF_STATUS(module));
//	if (!cm0_top_phy_if_status.s.cmu_ok)
//	{
//		GSER_CSR_INIT(top_err_ctrl1, CAVM_GSERRX_CM0_TOP_ERR_CTRL1(module));
//		GSER_CSR_INIT(top_err_ctrl2, CAVM_GSERRX_CM0_TOP_ERR_CTRL2(module));
//		GSER_CSR_INIT(top_err_ctrl3, CAVM_GSERRX_CM0_TOP_ERR_CTRL3(module));
//		gser_warn("GSERR%d: CM0_TOP_ERR 0x%02x%02x CRITICAL=%d\n",
//			module, top_err_ctrl2.s.err_code_15_8, top_err_ctrl1.s.err_code_7_0,
//			top_err_ctrl3.s.critical_err);
//		return;
//	}
//
//	/* Lane Errors */
//	int num_lanes = get_num_lanes(module);
//	for (int lane = 0; lane < num_lanes; lane++)
//	{
//		GSER_CSR_INIT(ctrl, CAVM_GSERRX_LNX_TOP_PHY_IF_CTRL_RSVD(module, lane));
//		if (!ctrl.s.ln_rx_rdy || !ctrl.s.ln_tx_rdy)
//		{
//			GSER_CSR_INIT(top_err_ctrl1, CAVM_GSERRX_LNX_TOP_ERR_CTRL1(module, lane));
//			GSER_CSR_INIT(top_err_ctrl2, CAVM_GSERRX_LNX_TOP_ERR_CTRL2(module, lane));
//			GSER_CSR_INIT(top_err_ctrl3, CAVM_GSERRX_LNX_TOP_ERR_CTRL3(module, lane));
//			gser_warn("GSERR%d.%d: LN_RX_RSY=%d, LN_TX_RDY=%d, Error 0x%02x%02x CRITICAL_ERR_RX=%d CRITICAL_ERR_TX=%d\n",
//				module, lane, ctrl.s.ln_rx_rdy, ctrl.s.ln_tx_rdy,
//				top_err_ctrl2.s.err_code_15_8, top_err_ctrl1.s.err_code_7_0,
//				top_err_ctrl3.s.critical_err_rx, top_err_ctrl3.s.critical_err_tx);
//		}
//	}
//}

/**
 * Determine the GSERR clocking needed based on the current config. No updates
 * are done, just determines what is needed. This will be input to the code that
 * actually makes changes.
 *
 * @param node
 * @param module
 *
 * @return SERDES clocking mode
 */
static cavm_gserrx_common_phy_ctrl_bcfg_t qlm_gserr_get_clock_mode(int module)
{
	/* Find all needed clocks */
	bool need_25g = false;
	bool need_20g = false;
	bool need_10g = false;
	bool need_6g = false;
	bool need_5g = false;
	bool need_1g = false;

	int num_lanes = get_num_lanes(module);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		qlm_state_lane_t state = qlm_gserr_get_state(module, lane);
		need_25g |= (state.s.baud_mhz == 25781);
		need_20g |= (state.s.baud_mhz == 20625);
		need_10g |= (state.s.baud_mhz == 10312);
		need_6g |= (state.s.baud_mhz == 6250) || (state.s.baud_mhz == 3125);
		need_5g |= (state.s.baud_mhz == 5000) || (state.s.baud_mhz == 2500);
		need_1g |= (state.s.baud_mhz == 1250);
	}

	int phy_ctrl_rate1;
	int phy_ctrl_rate2;

	if (need_1g)
	{
		/* All PHY rates are fixed when we support 1G. Only 25G, 10G, and 1G
		   are available */
		if (need_20g)
			phy_ctrl_rate1 = 0x05; /* 20G */
		else
			phy_ctrl_rate1 = 0x03; /* 25G */
		phy_ctrl_rate2 = 0x23; /* 10G */
		/* rate 3 is auto enabled at 1G */
	}
	else if (need_25g)
	{
		phy_ctrl_rate1 = 0x03; /* 25G */
		if (need_20g)
			phy_ctrl_rate2 = 0x05; /* 20G */
		else if (need_6g)
			phy_ctrl_rate2 = 0x28; /* 6G */
		else if (need_5g)
			phy_ctrl_rate2 = 0x2a; /* 5G */
		else
			phy_ctrl_rate2 = 0x23; /* 10G */
	}
	else if (need_20g)
	{
		phy_ctrl_rate1 = 0x05; /* 20G */
		if (need_6g)
			phy_ctrl_rate2 = 0x28; /* 6G */
		else if (need_5g)
			phy_ctrl_rate2 = 0x2a; /* 5G */
		else
			phy_ctrl_rate2 = 0x23; /* 10G */
	}
	else if (need_6g)
	{
		phy_ctrl_rate1 = 0x23; /* 10G */
		phy_ctrl_rate2 = 0x28; /* 6G */
	}
	else if (need_5g)
	{
		phy_ctrl_rate1 = 0x23; /* 10G */
		phy_ctrl_rate2 = 0x2a; /* 5G */
	}
	else /* No special speeds are needed, setup for 25G, 10G, 1G for flexibility */
	{
		phy_ctrl_rate1 = 0x03; /* 25G */
		phy_ctrl_rate2 = 0x23; /* 10G */
		/* rate 3 is auto enabled at 1G */
	}

	GSER_TRACE(QLM, "GSERR%d: phy_ctrl_rate1=0x%02x, phy_ctrl_rate2=0x%02x\n",
		module, phy_ctrl_rate1, phy_ctrl_rate2);

	GSER_CSR_INIT(bcfg, CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(module));
	bcfg.s.refclk_input_sel = (module == 0) ? 0 : 2; /* From CNF95XX_B0_GSERR_Programming_v1p0.pdf */
	bcfg.s.phy_ctrl_refclk = 0x0e; /* From CNF95XX_B0_GSERR_Programming_v1p0.pdf */
	bcfg.s.phy_ctrl_rate1 = phy_ctrl_rate1;
	bcfg.s.phy_ctrl_rate2 = phy_ctrl_rate2;

	return bcfg;
}

/**
 * Determine the lane settings to support the current protocol. No actual GSERR
 * changes are made.
 *
 * @param node
 * @param module
 * @param lane
 *
 * @return Lane settings that need to be applied
 */
static cavm_gserrx_lanex_control_bcfg_t qlm_gserr_get_lane_mode(int module, int lane)
{
	qlm_state_lane_t state = qlm_gserr_get_state(module, lane);
	int ln_ctrl_rate = 0;
	int tx_clk_mux_sel = 0;
	int ln_ctrl_tx_width = 0x4;
	int ln_ctrl_rx_width = 0x5;

	switch (state.s.baud_mhz)
	{
		case 1250:
			ln_ctrl_rate = 2; /* PHY rate 3 */
			tx_clk_mux_sel = 0x6; /* PHY rate 3 */
			ln_ctrl_tx_width = 0x2; /* 16 bit */
			ln_ctrl_rx_width = 0x3; /* 20 bit */
			break;
		case 2500:
		case 3125:
			ln_ctrl_rate = 0x5; /* PHY rate 2 divide by 2 */
			tx_clk_mux_sel = 0x4; /* PHY rate 2 divide by 2 */
			ln_ctrl_tx_width = 0x2; /* 16 bit */
			ln_ctrl_rx_width = 0x3; /* 20 bit */
			break;
		case 5000:
		case 6250:
			ln_ctrl_rate = 0x1; /* PHY rate 2 */
			tx_clk_mux_sel = 0x1; /* PHY rate 2 */
			ln_ctrl_tx_width = 0x2; /* 16 bit */
			ln_ctrl_rx_width = 0x3; /* 20 bit */
			break;
		case 10312:
		{
			GSER_CSR_INIT(phy_bcfg, CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(module));
			if (phy_bcfg.s.phy_ctrl_rate1 == 0x23) /* 10G */
			{
				ln_ctrl_rate = 0x0; /* PHY rate 1 */
				tx_clk_mux_sel = 0x0; /* PHY rate 1 */
			}
			else
			{
				ln_ctrl_rate = 0x1; /* PHY rate 2 */
				tx_clk_mux_sel = 0x1; /* PHY rate 2 */
			}
			ln_ctrl_tx_width = 0x2; /* 16 bit */
			ln_ctrl_rx_width = 0x3; /* 20 bit */
			break;
		}
		case 20625:
		case 25781:
		default:
			ln_ctrl_rate = 0x0; /* PHY rate 1 */
			tx_clk_mux_sel = 0x0; /* PHY rate 1 */
			ln_ctrl_tx_width = 0x4; /* 32 bit */
			ln_ctrl_rx_width = 0x5; /* 40 bit */
			break;
	}

	bool is_dual;
	bool is_quad;
	switch (state.s.mode)
	{
		case QLM_MODE_RXAUI: /* Lane sync across two lanes */
			is_dual = true;
			is_quad = false;
			break;
		case QLM_MODE_XAUI: /* Lane sync across four lanes */
			is_dual = false;
			is_quad = true;
			break;
		default: /* No other protocols require lane sync */
			is_dual = false;
			is_quad = false;
			break;
	}

	GSER_TRACE(QLM, "GSERR%d.%d: %s %dMHz, rate=%d, tx_width=%d, rx_width=%d, clk_mux=%d, quad=%d, dual=%d\n",
		module, lane, qlm_mode_to_cfg_str(state.s.mode), state.s.baud_mhz,
		ln_ctrl_rate, ln_ctrl_tx_width, ln_ctrl_rx_width, tx_clk_mux_sel,
		is_quad, is_dual);

	GSER_CSR_INIT(bcfg, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane));
	bcfg.s.ln_ctrl_tx_rate = ln_ctrl_rate;
	bcfg.s.ln_ctrl_rx_rate = ln_ctrl_rate;
	bcfg.s.ln_ctrl_tx_width = ln_ctrl_tx_width;
	bcfg.s.ln_ctrl_rx_width = ln_ctrl_rx_width;
	bcfg.s.tx_clk_mux_sel = tx_clk_mux_sel;
	bcfg.s.cgx_quad = is_quad;
	bcfg.s.cgx_dual = is_dual;
	bcfg.s.ln_link_stat = 0; /* No AN from PHY */
	bcfg.s.ln_an_cfg = 0; /* No AN from PHY */

	return bcfg;
}

/**
 * Change the lane speed of a single lane. Note that the PHY base rates must
 * already be setup
 *
 * @param node
 * @param module
 * @param lane
 *
 * @return Zero on success, negative on error
 */
static int qlm_gserr_change_lane_rate(int module, int lane)
{
	GSER_TRACE(QLM, "GSERP%d.%d: Changing lane mode\n", module, lane);
	/* Lane Rate Change Sequence
		1. If the Lane is inactive, proceed to Step 5, else if the Lane is
		already up and connected bring down the CGX LMAC {SPU or GMP}
		connection to the lane and ensure CGX has asserted the CGX-to-GSERR
		reset signal to place the GSERR Lane Tx/Rx async FIFOs in reset. This
		is required to ensure the "credit" flow control signaling between the
		GSERR lane async Tx/Rx FIFOs and the CGX LMAC cleanly enter the reset
		state.
		Refer to CGX(0..2)_SPU(0..3)_CONTROL1[RESET] //LMAC is SPU
		Refer to CGX(0..2)_GMP_PCS_MR(0..3)_CONTROL[RESET] //LMAC is GMP */
	// FIXME: CGX stop

	/* 2. Disable the PHY transmitter driver and put the GSERR lane in reset by
		writing
		Write GSERR(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_CTRL_TX_EN=0 Disable the transmitter
		Wait at leaset 100 nanoseconds to allow the Transmitter to turn off
		Write GSERR(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_RST = 1 Put the PHY lane in Reset */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane),
		c.s.ln_ctrl_tx_en = 0);
	gser_wait_usec(1);
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane),
		c.s.ln_rst = 1);

	/* 3. Wait for the PHY firmware to signal that the Lane is in the Reset
		power state which is signaled by the lane Tx and Rx blocks negating
		the Tx/Rx ready signals.
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_TX_RDY=0 Lane Tx is not ready
			LN_RX_RDY=0 Lane Rx is not ready
			LN_STATE_CHNG_RDY = 0 Lane is transitioning states */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_TX_RDY, ==, 0, 500))
		gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_tx_rdy]=0 (lane is reset)\n", module, lane);
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_RX_RDY, ==, 0, 500))
		gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_rx_rdy]=0 (lane is reset)\n", module, lane);
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 0, 500))
	{
		/* This happens fast, so sometimes we miss it */
		//gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=0\n (lane is reset)", module, lane);
	}

	/* 4. Wait for the “Lane State Change Ready” to signal that the lane has
		transitioned to the “Reset” state.
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 1 Lane is in the “Reset” power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 5000))
		gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1 (lane is reset)\n", module, lane);

	/* 5. Program the new Lane rate, data width, and transmitter clock parameters
		using the values shown in Table 2
		Write GSERR(0..2)_LANE(0..3)_CONTROL_BCFG
			CFG_CGX=0 Hold the Tx/Rx Async FIFOs in reset
			LN_CTRL_RX_WIDTH Table 2
			LN_CTRL_TX_WIDTH Table 2
			LN_CTRL_RX_RATE Table 2
			LN_CTRL_TX_RATE Table 2
			TX_CLK_MUX_SEL Table 2
			LN_LINK_STAT Set to 0, no AN from the PHY
			LN_AN_CFG Set to 0, no AN from thePHY
		Wait one microsecond for the PHY firmware to receive the lane rate
		change parameters and for the transmitter clock mux to switch to the
		new Tx clock rate. */
	cavm_gserrx_lanex_control_bcfg_t bcfg = qlm_gserr_get_lane_mode(module, lane);
	GSER_CSR_WRITE(CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane), bcfg.u);
	gser_wait_usec(1);

	/* 6. Trigger the lane rate change by writing the LN_RATE_CHNG field to
		load the *new* lane rate settings from Step 5 above.
		Write GSERR(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_RATE_CHNG=1
		Wait at least 200 nanoseconds for the PHY firmware to receive the lane
		rate change request before moving to the next step. Note the
		LN_STATE_CHNG_RDY will toggle from 1->0->1 when the LN_RATE_CHNG bit is
		set above. */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane),
		c.s.ln_rate_chng = 1);
	gser_wait_usec(1);

	/* 7. Wait for the “Lane State Change Ready” status bit to deassert
		indicating the lane is transitioning to the “Rate Change” state.
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 0 Lane is transitioning power states */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 0, 500))
		gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=0 (rate change)\n", module, lane);

	/* 8. Program the lane x2/x4 bonding (used with RXAUI,XAUI,DXAUI) fields if
		applicable to the new lane rate or link topology
		Write GSERR(0..2)_LANE(0..3)_CONTROL_BCFG
			CGX_QUAD = 1 If the lane is part of a x4 link (XAUI,DXAUI); else 0
			CGX_DUAL = 1 If lanes 0&1 or 2&3 are part of a x2 link (RXAUI); else 0
			CFG_CGX = 0 Keep the Tx/Rx FIFOs to CGX in reset */
	/* Done at step 6 */

	/* 9. Wait for the PHY “Lane State Change Ready” to signal that the lane has
		transitioned to the “Rate Change” state.
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 1 Lane is in the “Rate Change” power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 500))
		gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1 (rate change)\n", module, lane);

	/* 10. Deassert the LN_STATE_CHNG signal to complete the lane
		reconfiguration
		Write GSERR(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_RATE_CHNG=0
		Wait 200 nanoseconds for the PHY firmware to receive the lane rate
		change deassertion request. */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane),
		c.s.ln_rate_chng = 0);
	gser_wait_usec(1);

	/* 11. Wait for the “Lane State Change Ready” status bit to deassert
		indicating the lane is transitioning states.
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 0 Lane is transitioning states */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 0, 500))
		gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=0 (change done)\n", module, lane);

	/* 12. Wait for the PHY “Lane State Change Ready” to signal that the lane
		has transitioned back to the “Reset” state.
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 1 Lane is in the “Reset” power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 500))
		gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1 (change done)\n", module, lane);

	/* 13. Release the lane from reset.
		Write GSERR(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_RST=0 Release the lane reset */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane),
		c.s.ln_rst = 0);

	/* 14. Read/Poll for the GSERR to set the Lane State Change Ready flag and
		drive the Lane Tx and Rx ready flags to signal that the lane as
		returned to the ACTIVE state.
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_TX_RDY=1 Lane Tx is ready
			LN_RX_RDY=1 Lane Rx is ready
			LN_STATE_CHNG_RDY = 1 Lane is in the “Active” power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_TX_RDY, ==, 1, 5000))
		gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_tx_rdy]=1 (reset done)\n", module, lane);
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_RX_RDY, ==, 1, 5000))
		gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_rx_rdy]=1 (reset done)\n", module, lane);
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 5000))
		gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1 (reset done)\n", module, lane);

	/* 15. Enable the Tx/Rx FIFOs between CGX and GSERR
		Write GSERR(0..2)_LANE(0..3)_CONTROL_BCFG
			CFG_CGX = 1 Enable Tx and Rx Async FIFOs to CGX */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane),
		c.s.cfg_cgx = 1);

	/* 16. CGX should start transmitting now, refer to the CGX initialization
		and link bring-up section in the CNF95XX B0 HRM. */
	// FIXME: Enable CGX transmit

	/* 17. Poll the lane receiver looking for the Lane Status Rx Valid flag to
		be set indicating the Lane Rx CDR is locked and the lane is receiving
		data
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STAT_LOS=0 Rx loss-of-signal is deasserted
			LN_STAT_RXVALID=1 Rx CDR has locked */
	// FIXME: Should be done in CGX

	/* 18. Now bring up the CGX LMAC receiver for the associated lane, or
		lanes for a x4 or x2 link. Refer to the CGX initialization and link
		bring-up section in the CNF95XX B0 HRM. */
	// FIXME: Should be done in CGX

	/* 19. Once CGX starts transmitting set the SerDes Transmitter Enable on
		each lane that is active. This prevents transmitting garbage on the Tx
		serial lanes prior to CGX starting up.
		Write GSERR(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_CTRL_TX_EN = 1 Enable the Lane SerDes transmitter driver */
	// FIXME: Should be done in CGX

	/* 20. Poll the lane receiver looking for a valid receive signal and the
		indication that the PHY Lane Rx CDR is locked. Note that if the
		Ethernet link-partner is not up and driving valid Ethernet data into
		the GSERR PHY serial receiver input then LN_STAT_LOS=1 and
		LN_STAT_RXVALID=0 until the link partner starts to drive valid data. So
		do not watch-dog time-out this polling routine. Once LN_STAT_LOS
		transitions 1->0 then LN_STAT_RXVALID should transition 0->1 within
		500 microseconds so a watch-dog timer can be used to guardband the
		Rx CDR lock process.
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STAT_LOS=0 Rx signal carrier detected (Loss-of-signal is negated)
			LN_STAT_RXVALID=1 The receive CDR has locked and Rx data is valid */
	// FIXME: Should be done in CGX

	/* 21. Bring up the CGX receive side. Refer to the CGX initialization and
	   link bring-up steps in the CNF95XX B0 HRM. */
	// FIXME: Should be done in CGX
	GSER_TRACE(QLM, "GSERP%d.%d: Lane mode change complete\n", module, lane);
	return 0;
}

/**
 * Change the phy speed. Note that all lanes go down
 *
 * @param node
 * @param module
 *
 * @return Zero on success, negative on error
 */
static int qlm_gserr_change_phy_rate(int module)
{
	gser_wait_usec(10000); /* No idea why this is needed. 1ms isn't long enough */
	GSER_TRACE(QLM, "GSERP%d: Changing PHY rates\n", module);
	int num_lanes = get_num_lanes(module);
	/* PHY Rate Change Sequence
		The PHY Rate 1 and Rate 2 values can be changed without driving the
		GSERR(0..2)_COMMON_PHY_CTRL_BCFG[POR] power on reset. This is necessary
		to prevent disruption of the reference clock distribution network
		between GSERR blocks, refer to Figure 3. The steps required to program
		new PHY Rate 1 and Rate 2 values is described below. */

	/* 1. If the Lanes are all inactive, proceed to Step 5, else bring-down all
		the lanes if the lanes are already up and connected to the CGX LMAC
		{SPU or GMP} and ensure CGX has asserted the CGX-to-GSERR reset signal
		to place the GSERR Lane Tx/Rx async FIFOs in reset. This is required
		to ensure the "credit" flow control signaling between the GSERR lane
		async Tx/Rx FIFOs and the CGX LMAC cleanly enter the reset state.
		Refer to CGX(0..2)_SPU(0..3)_CONTROL1[RESET] //LMAC is SPU
		Refer to CGX(0..2)_GMP_PCS_MR(0..3)_CONTROL[RESET] //LMAC is GMP */
	// FIXME: CGX stop

	/* 2. Disable the PHY transmitter driver and put the GSERR lane in reset by
		writing
		Write GSERR(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_CTRL_TX_EN=0 Disable the transmitter
		Wait at lease 100 nanoseconds to allow the Transmitter to turn off
		Write GSERR(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_RST = 1 Put the PHY lane in Reset */
	for (int lane = 0; lane < num_lanes; lane++)
	{
		GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane),
			c.s.ln_ctrl_tx_en = 0);
		gser_wait_usec(1);
		GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane),
			c.s.ln_rst = 1);
	}

	/* 3. Wait for the PHY firmware to signal that the Lane is in the Reset
		power state which is signaled by the lane Tx and Rx blocks negating
		the Tx/Rx ready signals.
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_TX_RDY=0 Lane Tx is not ready
			LN_RX_RDY=0 Lane Rx is not ready
			LN_STATE_CHNG_RDY = 0 Lane is transitioning states */
	for (int lane = 0; lane < num_lanes; lane++)
	{
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_TX_RDY, ==, 0, 500))
			gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_tx_rdy]=0\n", module, lane);
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_RX_RDY, ==, 0, 500))
			gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_rx_rdy]=0\n", module, lane);
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 0, 500))
			gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=0\n", module, lane);
	}

	/* 4. Wait for the “Lane State Change Ready” to signal that the lane has
		transitioned to the “Reset” state.
		Read/Poll GSERR(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 1 Lane is in the “Reset” power state */
	for (int lane = 0; lane < num_lanes; lane++)
	{
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 5000))
			gser_error("GSERR%d.%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1\n", module, lane);
	}

	/* 5. Write the new PHY Rate 1 and Rate 2 settings from Table 2
		Write GSERR(0..2)_COMMON_PHY_CTRL_BCFG
			PHY_CTRL_RATE1 New PHY Rate 1 value from Table 2
			PHY_CTRL_RATE1 New PHY Rate 2 value from Table 2
		Wait 1 microsecond for the PHY firmware to pickup the new rates */
	cavm_gserrx_common_phy_ctrl_bcfg_t bcfg = qlm_gserr_get_clock_mode(module);
	GSER_CSR_WRITE(CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(module), bcfg.u);
	gser_wait_usec(1);

	/* 6. Reset the Clock Management Unit
		Write GSERR(0..2)_COMMON_PHY_CTRL_BCFG
			CM0_RST=1 Reset CMU */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(module),
		c.s.cm0_rst = 1);

	/* 7. Read/Poll for the CM0 State Change Ready to deassert
		Read/Poll GSERR(0..2)_COMMON_PHY_STATUS_BSTS
			CM0_STATE_CHNG_RDY=0 //CM0 is transitioning power states */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_COMMON_PHY_STATUS_BSTS(module), GSERRX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 0, 500))
		gser_error("GSERR%d: Timeout waiting for GSERRX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=0\n", module);

	/* 8. Read/Poll for the CM0 State Change Ready to reassert
		Read/Poll GSERR(0..2)_COMMON_PHY_STATUS_BSTS
			CM0_STATE_CHNG_RDY=1 //CM0 is in the Reset power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_COMMON_PHY_STATUS_BSTS(module), GSERRX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 1, 5000))
		gser_error("GSERR%d: Timeout waiting for GSERRX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=1\n", module);

	/* 9. Release the Clock Management Unit reset
		Write GSERR(0..2)_COMMON_PHY_CTRL_BCFG
			CM0_RST=0 Release CMU reset */
	GSER_CSR_MODIFY(c, CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(module),
		c.s.cm0_rst = 0);

	/* 10. Read/Poll for the CM0 State Change Ready to deassert
	   Read/Poll GSERR(0..2)_COMMON_PHY_STATUS_BSTS
	   CM0_STATE_CHNG_RDY=0 //CM0 is transitioning power states */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_COMMON_PHY_STATUS_BSTS(module), GSERRX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 0, 500))
		gser_error("GSERR%d: Timeout waiting for GSERRX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=0\n", module);

	/* 11. Read/Poll for the CM0 State Change Ready to reassert
	   Read/Poll GSERR(0..2)_COMMON_PHY_STATUS_BSTS
		CM0_STATE_CHNG_RDY=1 //CM0 has completed power state transtion */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_COMMON_PHY_STATUS_BSTS(module), GSERRX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 1, 5000))
		gser_error("GSERR%d: Timeout waiting for GSERRX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=1\n", module);

	/* 12. Read/Poll for the CM0 OK flag set
		Read/Poll GSERR(0..2)_COMMON_PHY_STATUS_BSTS
			CM0_OK=1 //CM0 status is Active power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_COMMON_PHY_STATUS_BSTS(module), GSERRX_COMMON_PHY_STATUS_BSTS_CM0_OK, ==, 1, 500))
		gser_error("GSERR%d: Timeout waiting for GSERRX_COMMON_PHY_STATUS_BSTS[cm0_ok]=1\n", module);

	/* Program the new Lane Rates to the new PHY rates, refer to the steps in
	   Section 1.4 Lane Rate Change. */
	GSER_TRACE(QLM, "GSERP%d: PHY rate change complete\n", module);
	return 0;
}

/**
 * Initialize function removed by gser-update script (qlm.sed)
 */
//void qlm_gserr_init()
//{
//	const char *firmware_file = NULL;
//	int num_gserr = -1;
//	int sensor;
//
//	/* Figure out which firmware file to use */
//	if (gser_is_model(OCTEONTX_CN96XX))
//	{
//		firmware_file = "/fatfs/serdes/gserr-cn96xx.frm.lzma";
//		num_gserr = 3;
//		sensor = 10; /* TSN10 is nearest to GSERR */
//	}
//	else if (gser_is_model(OCTEONTX_CNF95XX))
//	{
//		firmware_file = "/fatfs/serdes/gserr-cnf95xx.frm.lzma";
//		num_gserr = 3;
//		sensor = 2;  /* TSN2 is nearest to GSERR */
//	}
//	else if (gser_is_model(OCTEONTX_LOKI))
//	{
//		firmware_file = "/fatfs/serdes/gserr-loki.frm.lzma";
//		num_gserr = 1;
//		sensor = 2;  /* TSN2 is nearest to GSERR */
//	}
//	else if (gser_is_model(OCTEONTX_CN98XX))
//	{
//		firmware_file = "/fatfs/serdes/gserr-cn98xx.frm.lzma";
//		num_gserr = 5;
//		sensor = 15; /* TSN15 is nearest to GSERR */
//	}
//	else
//	{
//		gser_error("GSERR: Unrecognized chip\n");
//		return;
//	}
//
//	GSER_CSR_INIT(gserrx_init_ctl, CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(0));
//	if (!gserrx_init_ctl.s.cpu_reset)
//	{
//		GSER_TRACE(QLM, "GSERR: Already running, forcing a reset\n");
//		for (int module = 0; module < num_gserr; module++)
//			GSER_CSR_MODIFY(c, CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(module),
//				c.s.cm0_rst = 1;
//				c.s.por = 1;
//				c.s.cpu_reset = 1;
//				c.s.apb_reset = 1);
//		gser_wait_usec(1000);
//	}
//
//	/* Open the firmware file */
//	GSER_TRACE(QLM, "GSERR: Loading firmware %s\n", firmware_file);
//	uint64_t filesize = 0;
//	uint64_t *firmware = gser_signed_load(firmware_file, 0,GSER_SIGNED_IMAGE, GSER_SIGNED_FLAG_NOT_ENCRYPTED, &filesize);
//	if (!firmware)
//	{
//		gser_error("GSERR: Failed to load firmware %s\n", firmware_file);
//		return;
//	}
//
//	/* 1) Load the PHY Firmware - While not EOF read the FW file and write the
//	   FW image to the Program Memory (96KB, 12288*8Bytes) */
//	/* Load the firmware */
//	for (int module = 0; module < num_gserr; module++)
//	{
//		for (int index = 0; index < (int)filesize / 8; index++)
//		{
//			uint64_t data = gser_cpu_to_be64(firmware[index]);
//			GSER_CSR_WRITE(CAVM_GSERRX_PMEMX(module, index), data);
//		}
//
//		/* Write protect program memory now that the firmware is loaded */
//		GSER_CSR_MODIFY(c, CAVM_GSERRX_COMMON_PHY_CTRL_PROT(module),
//			c.s.pmem_wr_prot_stky = 1);
//
//		/* Verify the firmware matches what we loaded */
//		for (int index = 0; index < (int)filesize / 8; index++)
//		{
//			uint64_t data = gser_cpu_to_be64(firmware[index]);
//			uint64_t tmp = GSER_CSR_READ(CAVM_GSERRX_PMEMX(module, index));
//			if (tmp != data)
//			{
//				gser_error("GSERR%d: Mismatch loading firmware[%d], wrote 0x%016lx, read 0x%016lx\n", module, index, data, tmp);
//				return;
//			}
//		}
//	}
//	free(firmware);
//	GSER_TRACE(QLM, "GSERR: Loaded firmware of %lu bytes\n", filesize);
//
//	/* 2. For each “Active” QLM Program the Lane Rate and Width parameters for
//	   each lane.
//		Write GSERR()_LANE()_CONTROL_BCFG
//			LN_CTRL_TX_RATE=3’h0 //PHY Rate 1 used here, but other PHY rates available
//			LN_CTRL_RX_RATE=3’h0 //PHY Rate 1 used here, but other PHY rates available
//			LN_CTRL_TX_WIDTH=3’h4 //32-bit Tx data width @25G & 20.625G, or 3’h2 for 16-bit width at 10G and lower data rates
//			LN_CTRL_RX_WIDTH=3’h5 //40-bit Rx data width @25G & 20.625G, or 3’h3 for 20-bit width at 10G and lower data rates
//			TX_CLK_MUX_SEL=5’h0 //Select cm0_clk_rate1_o as the transmitter clock source, or appropriate clock source for other PHY rates
//			CGX_QUAD=0 //Set the CGX_QUAD=1 in all lanes which are part of a x4 link (Lanes 0,1,2,3).
//			CGX_DUAL=0 //Set the CGX_DUAL=1 in all lanes which are part of a x2 link (Lane 0&1, or Lanes 2&3) */
//	/* 3) For each QLM, Active and not Active Program the PHY Rate 1 and Rate 2
//		settings, also enable the reference clock buffer to the next downstream QLM
//		Write GSERR()_COMMON_PHY_CTRL_BCFG //Repeat for Slave QLMs
//			REFCLK_A_OE_R = 1’h1 //Enable reference clock A right side clock buffer to pass “clk_ref_a_r_o” to adjacent QLM
//			REFCLK_INPUT_SEL = 3’h0 if refclk from the “refclkp/m” pads, else 3’h2 if refclk is from the “clk_ref_a_r_i” inputs from adjacent QLM
//			PHY_CTRL_RATE1 = 6’h03 //Set PHY Rate 1 25.78125Gbps
//			PHY_CTRL_RATE2 = 6’h23 //Set PHY Rate 2 10.3125Gbps */
//	for (int module = 0; module < num_gserr; module++)
//	{
//		cavm_gserrx_common_phy_ctrl_bcfg_t bcfg = qlm_gserr_get_clock_mode(module);
//		GSER_CSR_WRITE(CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(module), bcfg.u);
//
//		int num_lanes = get_num_lanes(module);
//		for (int lane = 0; lane < num_lanes; lane++)
//		{
//			cavm_gserrx_lanex_control_bcfg_t bcfg = qlm_gserr_get_lane_mode(module, lane);
//			GSER_CSR_WRITE(CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane), bcfg.u);
//		}
//	}
//
//	/* Wait 10us for the above configuration to propagate through the SERDES block */
//	gser_wait_usec(10);
//
//	/* For most chips each serdes needs to be brought up in sequential order.
//		CN96XX requires reverse order
//			CN96XX GSERR2 is REXT Master and must be released from POR reset first.
//				Wait for GSERR2 cm0_state_chng_rdy=1, then release GSERR1 and
//				GSERR0 from POR reset. This ensures that GSERR2 completes the
//				REXT calibration and then sends the REXT trim code (8-bit value)
//				and REXT redy to GSERR0 and GSERR1.
//				Next GSERR2 must wait for the 156.25 MHz reference clock to
//				propagated from GSERR0->GSERR1->GSERR2 before we release the
//				GSERR2 cm0_rst.
//				So in summary
//				CN96XX C0:
//					Release POR,APB,CPU Reset Order:  GSERR2 first-> GSERR1 second ->GSERR0 last.
//					Release CM0_RST Order:  GSERR0 first-> GSERR1 second ->GSERR2 last.
//				CNF95XX B0:
//					Release POR,APB,CPU Reset Order:  GSERR0 first-> GSERR1 second ->GSERR2 last.
//					Release CM0_RST Order:  GSERR0 first-> GSERR1 second ->GSERR2 last. */
//	int module = 0;
//	int module_last = num_gserr - 1;
//	int module_inc = 1;
//	if (gser_is_model(OCTEONTX_CN96XX))
//	{
//		module = num_gserr - 1;
//		module_last = 0;
//		module_inc = -1;
//	}
//
//	/* Steps 4-6 are done sequentially across all GSERR blocks */
//	while (1)
//	{
//		/* 4) Take the PHY out of reset and release the APB reset
//			Write GSERR()_COMMON_PHY_CTRL_BCFG
//			POR = 1’h0 //Release PHY Power-on Reset (POR)
//			APB_RESET = 1’h0 //Release APB reset */
//		GSER_TRACE(QLM, "GSERR%d: Release POR and APB reset\n", module);
//		GSER_CSR_MODIFY(c, CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(module),
//			c.s.por = 0;
//			c.s.apb_reset = 0);
//		/* The emulator requires enabling the AFE shell */
//		if (gser_is_platform(GSER_PLATFORM_EMULATOR))
//		{
//			/* Write GSERR(0..2) _PHY0_TOP_SIM_CTRL
//					SIM_1B_MODEL = 1
//					FAST_SIM = 1 */
//			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_SIM_CTRL(module),
//				c.s.sim_1b_model = 1;
//				c.s.fast_sim = 1);
//		}
//
//		/* Reference clock should be running now */
//		int hz = qlm_gserr_measure_refclock(module);
//		GSER_TRACE(QLM, "GSERR%d: Reference clock at %dHz\n", module, hz);
//
//		/* Program the temperature into the GSERR */
//		GSER_CSR_INIT(temp_result, CAVM_TSNX_TS_TEMP_CONV_RESULT(sensor));
//		int temp = gser_extracts(temp_result.s.temp_corrected, 2, 8);
//		temp += 40;
//		if (temp < 0)
//			temp = 0;
//		if (temp > 255)
//			temp = 255;
//		/* We program the temperature regrdless of GSER_GSERR_ENABLE_DOSC_TEMP_SKEW.
//		   This allows checking that the correct value is programmed without
//		   performing DOSC_TEMP_SKEW */
//		GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_DOSC_TEMP_DURING_CAL(module),
//			c.s.temp_value = temp);
//		GSER_CSR_MODIFY(c, CAVM_GSERRX_CM0_FEATURE_PLL_CAL_EN_CFG1_RSVD(module),
//			c.s.vco_dosc_temp_skew = GSER_GSERR_ENABLE_DOSC_TEMP_SKEW);
//
//		/* 5) Take the CPU out of reset so it will boot and run the PHY firmware
//			Write GSERR()_COMMON_PHY_CTRL_BCFG
//			CPU_RESET = 1’h0 //Release CPU reset */
//		GSER_TRACE(QLM, "GSERR%d: Release CPU reset\n", module);
//		GSER_CSR_MODIFY(c, CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(module),
//			c.s.cpu_reset = 0);
//
//		/* 6) Poll for “CM0_STATE_CHNG_RDY=1” This signal indicates that the CMU
//			has transitioned to the RESET state
//			// transition into the RESET power state.
//			Poll GSERR()_COMMON_PHY_STATUS_BSTS
//				CM0_STATE_CHNG_RDY = 1'b1
//				PHY_ERROR = 1'b1 //PHY_ERROR (signal err_o) should always be 1'b0 unless there is a startup problem with the PHY
//								// If PHY_ERROR=1'b1 then throw an Error Message else continue to poll for CM0_STATE_CHNG_RDY=1. */
//		if (!gser_is_platform(GSER_PLATFORM_ASIM))
//		{
//			GSER_TRACE(QLM, "GSERR%d: Waiting for cm0_state_chng_rdy=1\n", module);
//			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_COMMON_PHY_STATUS_BSTS(module), GSERRX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 1, 1000))
//				gser_error("GSERR%d: Timeout waiting for GSERRX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=1 (after cpu_reset=0)\n", module);
//			qlm_gserr_poll_error(module);
//		}
//		if (module == module_last)
//			break;
//		module += module_inc;
//	}
//
//	/* Steps 7-9 are done sequentially across all GSERR blocks
//		For 156.25MHz Reference Clock bring-up and proper refclk propagation between GSERR block:
//		CN96XX C0 & CNF95XX B0 start with GSERR0 first, GSERR1 second, GSERR2 last
//			GSERR0 receives the 156.25MHz refclk from the BGA bumps so start CMU first,
//			GSERR1 gets refclk from GSERR0 and forwards to GSERR2, so start CMU second,
//			GSERR2 gets refclk from GSERR1 so start CMU last. */
//	module = 0;
//	module_last = num_gserr - 1;
//	module_inc = 1;
//
//	while (1)
//	{
//		/* 7) Take the GSERR PHY clock management unit (CM0) out of reset.
//			Write GSERR(0..2)_COMMON_PHY_CTRL_BCFG
//			CM0_RST=0 - Release CMU from reset */
//		GSER_TRACE(QLM, "GSERR%d: Release CMU reset\n", module);
//		GSER_CSR_MODIFY(c, CAVM_GSERRX_COMMON_PHY_CTRL_BCFG(module),
//			c.s.cm0_rst = 0);
//
//		/* 8) Poll for “CM0_STATE_CHNG_RDY=0” These signals indicate that the CMU
//			is transitioning between states
//			// transition into the RESET power state.
//			Poll GSERR()_COMMON_PHY_STATUS_BSTS
//				CM0_STATE_CHNG_RDY = 1’b0
//				PHY_ERROR = 1’b0 //PHY_ERROR (signal err_o) should always be 1'b0 unless there is a startup problem with the PHY
//								// If PHY_ERROR=1'b1 then throw an Error Message, else continue to poll for CM0_STATE_CHNG_RDY=0. */
//		if (!gser_is_platform(GSER_PLATFORM_ASIM))
//		{
//			GSER_TRACE(QLM, "GSERR%d: Waiting for cm0_state_chng_rdy=0\n", module);
//			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_COMMON_PHY_STATUS_BSTS(module), GSERRX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 0, 500))
//				GSER_TRACE(QLM, "GSERR%d: Timeout waiting for GSERRX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=0\n", module);
//			qlm_gserr_poll_error(module);
//		}
//
//		/* 9) Poll for “CM0_STATE_CHNG_RDY=1” and “CM0_OK=1” These signals indicate
//			that the CMU is in the ACTIVE state
//			// transition into the RESET power state.
//			Poll GSERR()_COMMON_PHY_STATUS_BSTS
//				CM0_STATE_CHNG_RDY = 1’b1
//				CM0_OK = 1’b1
//				PHY_ERROR = 1’b0 //PHY_ERROR (signal err_o) should always be 1'b0 unless there is a startup problem with the PHY
//								// If PHY_ERROR=1'b1 then throw an Error Message, else continue to poll for CM0_STATE_CHNG_RDY=1. */
//		if (!gser_is_platform(GSER_PLATFORM_ASIM))
//		{
//			GSER_TRACE(QLM, "GSERR%d: Waiting for cm0_ok=1, cm0_state_chng_rdy=1\n", module);
//			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_COMMON_PHY_STATUS_BSTS(module), GSERRX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 1, 5000))
//				gser_error("GSERR%d: Timeout waiting for GSERRX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=1 (after cm0_rst=0)\n", module);
//			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_COMMON_PHY_STATUS_BSTS(module), GSERRX_COMMON_PHY_STATUS_BSTS_CM0_OK, ==, 1, 10))
//				gser_error("GSERR%d: Timeout waiting for GSERRX_COMMON_PHY_STATUS_BSTS[cm0_ok]=1\n", module);
//			qlm_gserr_poll_error(module);
//		}
//		if (module == module_last)
//			break;
//		module += module_inc;
//	}
//
//	/* 10) Poll for all lanes “Ready” to indicate the LANE is in the “RESET” state
//		For i=0 to 3, i++
//			Poll GSERR()_LANE(i)_STATUS_BSTS
//				LN_STATE_CHNG_RDY = 1'b1 //Lane is in RESET power state
//		next i;
//		// Use a watchdog timer on “LN_STATE_CHNG_RDY” in case this signal never goes ready. */
//	if (!gser_is_platform(GSER_PLATFORM_ASIM))
//	{
//		for (int module = 0; module < num_gserr; module++)
//		{
//			GSER_TRACE(QLM, "GSERR%d: Waiting for ln_state_chng_rdy=1\n", module);
//			int num_lanes = get_num_lanes(module);
//			for (int lane = 0; lane < num_lanes; lane++)
//			{
//				if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 500))
//					gser_error("GSERR%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1\n", module);
//			}
//			/* Enable the BIST/PRBS clocks before starting each lane */
//			GSER_TRACE(QLM, "GSERR%d: Enabling BIST/PRBS logic\n", module);
//			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_CLOCK_LN0_CG_CTRL(module),
//				c.s.bist_rx = 1;
//				c.s.bist_tx = 1);
//			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_CLOCK_LN1_CG_CTRL(module),
//				c.s.bist_rx = 1;
//				c.s.bist_tx = 1);
//			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_CLOCK_LN2_CG_CTRL(module),
//				c.s.bist_rx = 1;
//				c.s.bist_tx = 1);
//			GSER_CSR_MODIFY(c, CAVM_GSERRX_PHY0_TOP_CLOCK_LN3_CG_CTRL(module),
//				c.s.bist_rx = 1;
//				c.s.bist_tx = 1);
//		}
//	}
//	/* 11) Take the lanes out of reset
//		For i=0 to 3, i++
//			Write GSERR()_LANE(i)_CONTROL_BCFG
//				LN_RST_N = 1'b1
//		next i */
//	for (int module = 0; module < num_gserr; module++)
//	{
//		GSER_TRACE(QLM, "GSERR%d: Taking lanes out of reset\n", module);
//		int num_lanes = get_num_lanes(module);
//		for (int lane = 0; lane < num_lanes; lane++)
//		{
//			if (gser_is_platform(GSER_PLATFORM_EMULATOR))
//			{
//				GSER_TRACE(QLM, "GSERR%d.%d: Disable AFE calibration for emulator\n", module, lane);
//				GSER_CSR_WRITE(CAVM_GSERRX_LNX_FEATURE_TX_CAL_CFG0_RSVD(module, lane), 0);
//				GSER_CSR_WRITE(CAVM_GSERRX_LNX_FEATURE_REXT_CAL_RESULT_CFG0_RSVD(module, lane), 0);
//				GSER_CSR_WRITE(CAVM_GSERRX_LNX_FEATURE_LEQ_OFFSET_CAL_CFG0_RSVD(module, lane), 0);
//				GSER_CSR_WRITE(CAVM_GSERRX_LNX_FEATURE_DFE_OFFSET_CAL_CFG0_RSVD(module, lane), 0);
//				GSER_CSR_WRITE(CAVM_GSERRX_LNX_FEATURE_CDR_CAL_CFG0(module, lane), 0);
//				GSER_CSR_WRITE(CAVM_GSERRX_LNX_FEATURE_DFE_OFFSET_CAL_CFG1_RSVD(module, lane), 0);
//			}
//			GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane),
//				c.s.ln_rst = 0);
//		}
//	}
//	/* 12) Poll for “LN_STATE_CHNG_RDY = 1” & “LN_TX_RDY=1” & “LN_RX_RDY=1” to
//		indicate the Lane is in the “Active” power state
//		For i=0 to 3, i++
//			Poll GSERR()_LANE(i)_STATUS_BSTS
//				LN_TX_RDY=1’b1 //Lane Tx is up
//				LN_RX_RDY=1’b1 //Lane Rx is up
//				LN_STATE_CHNG_RDY = 1'b1 //Lane is in RESET power state
//		next i */
//	if (!gser_is_platform(GSER_PLATFORM_ASIM))
//	{
//		for (int module = 0; module < num_gserr; module++)
//		{
//			GSER_TRACE(QLM, "GSERR%d: Waiting for ln_tx_rdy=1\n", module);
//			int num_lanes = get_num_lanes(module);
//			for (int lane = 0; lane < num_lanes; lane++)
//			{
//				if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_TX_RDY, ==, 1, 5000))
//					gser_error("GSERR%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_tx_rdy]=1\n", module);
//				if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_RX_RDY, ==, 1, 500))
//					gser_error("GSERR%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_rx_rdy]=1\n", module);
//				if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERRX_LANEX_STATUS_BSTS(module, lane), GSERRX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 500))
//					gser_error("GSERR%d: Timeout waiting for GSERRX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1\n", module);
//			}
//		}
//	}
//	/* 13) Once Lane is Tx and Rx is Ready set the CFG_CGX=1 to enable the Tx/Rx
//		FIFOs between CGX and PHY.
//		Write in reverse order, Lane 3 down to Lane 0
//		For i=3 to 0, i--
//			Write GSERR()_LANE(i)_CONTROL_BCFG
//				CFG_CGX = 1 //Enable Tx and Rx FIFOs to CGX
//		next i */
//	for (int module = num_gserr - 1; module >= 0; module--)
//	{
//		GSER_TRACE(QLM, "GSERR%d: Enable CGX connection\n", module);
//		int num_lanes = get_num_lanes(module);
//		for (int lane = 0; lane < num_lanes; lane++)
//		{
//			GSER_CSR_MODIFY(c, CAVM_GSERRX_LANEX_CONTROL_BCFG(module, lane),
//				c.s.cfg_cgx = 1);
//		}
//	}
//	/* 14) The CGX MAC transmitter can be started at this point */
//	/* Done later */
//	GSER_TRACE(QLM, "GSERR: End of init\n");
//}
