/*
 * This file was copied from BDK using gsern-bdk-to-atf.sh script.
 * Do not edit it manually!
 */

/***********************license start***********************************
* Copyright (C) 2018-2020 Marvell International Ltd.
* SPDX-License-Identifier: BSD-3-Clause
* https://spdx.org/licenses
***********************license end**************************************/
#include <gser_internal.h>
#include <qlm/qlm.h>
#include <qlm/qlm_gserj.h>

/**
 * Define to enable or disable lanes 2-3. Used to keep differences between
 * GSERR, GSERC, and GSERJ minimal
 */
#define QUAD_LANE 0

static int mailbox_command(int qlm, uint8_t cmd, uint64_t args);
static int mailbox_response(int qlm, uint64_t *arg0, uint64_t *arg1);

/* Indexed by QLM number and lane */
static uint64_t prbs_errors[8][2];

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
	GSER_CSR_INIT(gserjx_const, CAVM_GSERJX_CONST(module));
	return gserjx_const.s.nr_lanes;
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
qlm_state_lane_t qlm_gserj_get_state(int qlm, int lane)
{
	qlm_state_lane_t state = {.u = GSER_CSR_READ(CAVM_GSERJX_SCRATCHX(qlm, lane))};
	return state;
}

/**
 * Measure the reference clock of a QLM
 *
 * @param qlm	QLM to measure
 *
 * @return Clock rate in Hz
 */
int qlm_gserj_measure_refclock(int qlm)
{
	if (gser_is_platform(GSER_PLATFORM_ASIM))
		return REF_122MHZ;

	GSER_CSR_INIT(ctr_start, CAVM_GSERJX_REFCLK_CTR(qlm));
	uint64_t start = gser_clock_get_count(GSER_CLOCK_TIME);

	/* Wait for a short time to get a number of counts */
	gser_wait_usec(1000); /* 1ms */

	GSER_CSR_INIT(ctr_stop, CAVM_GSERJX_REFCLK_CTR(qlm));
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
int qlm_gserj_reset(int qlm)
{
	GSER_CSR_MODIFY(c, CAVM_GSERJX_COMMON_PHY_CTRL_BCFG(qlm),
		c.s.cm0_rst = 1;
		c.s.por = 1;
		c.s.cpu_reset = 1;
		c.s.apb_reset = 1);
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
int _qlm_gserj_enable_prbs(int qlm, int prbs, qlm_direction_t dir, int qlm_lane)
{
	int pattern = 1;
	uint64_t user_defined[4] = { 0, 0, 0, 0 };
	int user_defined_length = 0;
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
		case 100: /* an 8 + 8 clock like generator test defined by SFF8418 */
			pattern = 7; /* User defined pattern */
			user_defined[0] = 0xff00ff00ff00ff00ul; /* Bits 63:0 */
			user_defined[1] = 0xff00ff00ff00ff00ul; /* Bits 127:64 */
			user_defined[2] = 0xff00ff00ff00ff00ul; /* Bits 191:128 */
			user_defined[3] = 0x00000000000000fful; /* Bits 199:192 */
			/* Determines the length of the UDP. Must be set to d160 modulus udp_length. */
			user_defined_length = 160 % 16;
			break;
	}

	int num_lanes = get_num_lanes(qlm);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		if ((qlm_lane != -1) && (qlm_lane != lane))
			continue;

		if (dir & QLM_DIRECTION_TX)
		{
			/* No error injection */
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL0(qlm, lane), 0);
			/* Seed PRBS */
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_PRBS_CTRL0(qlm, lane), gser_rng_get_random8());
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_PRBS_CTRL1(qlm, lane), gser_rng_get_random8());
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_PRBS_CTRL2(qlm, lane), gser_rng_get_random8());
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_PRBS_CTRL3(qlm, lane), gser_rng_get_random8());
			/* Write the user defined pattern */
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_7_0(qlm, lane), user_defined[0]);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_15_8(qlm, lane), user_defined[0] >> 8);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_23_16(qlm, lane), user_defined[0] >> 16);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_31_24(qlm, lane), user_defined[0] >> 24);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_39_32(qlm, lane), user_defined[0] >> 32);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_47_40(qlm, lane), user_defined[0] >> 40);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_55_48(qlm, lane), user_defined[0] >> 48);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_63_56(qlm, lane), user_defined[0] >> 56);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_71_64(qlm, lane), user_defined[1]);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_79_72(qlm, lane), user_defined[1] >> 8);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_87_80(qlm, lane), user_defined[1] >> 16);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_95_88(qlm, lane), user_defined[1] >> 24);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_103_96(qlm, lane), user_defined[1] >> 32);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_111_104(qlm, lane), user_defined[1] >> 40);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_119_112(qlm, lane), user_defined[1] >> 48);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_127_120(qlm, lane), user_defined[1] >> 56);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_135_128(qlm, lane), user_defined[2]);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_143_136(qlm, lane), user_defined[2] >> 8);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_151_144(qlm, lane), user_defined[2] >> 16);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_159_152(qlm, lane), user_defined[2] >> 24);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_167_160(qlm, lane), user_defined[2] >> 32);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_175_168(qlm, lane), user_defined[2] >> 40);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_183_176(qlm, lane), user_defined[2] >> 48);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_191_184(qlm, lane), user_defined[2] >> 56);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_199_192(qlm, lane), user_defined[3]);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_UDP_SHIFT_AMOUNT(qlm, lane), user_defined_length);
			/* Start transmit */
			GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_BIST_TX_CTRL(qlm, lane),
				c.s.pattern_sel = pattern;
				c.s.en = 1);
			/* Mux data from BIST generator */
			GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_TOP_DPL_TXDP_CTRL1(qlm, lane),
				c.s.tx_dmux2_sel = 1);

		}
		if (dir & QLM_DIRECTION_RX)
		{
			/* Write the user defined pattern */
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_7_0(qlm, lane), user_defined[0]);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_15_8(qlm, lane), user_defined[0] >> 8);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_23_16(qlm, lane), user_defined[0] >> 16);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_31_24(qlm, lane), user_defined[0] >> 24);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_39_32(qlm, lane), user_defined[0] >> 32);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_47_40(qlm, lane), user_defined[0] >> 40);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_55_48(qlm, lane), user_defined[0] >> 48);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_63_56(qlm, lane), user_defined[0] >> 56);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_71_64(qlm, lane), user_defined[1]);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_79_72(qlm, lane), user_defined[1] >> 8);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_87_80(qlm, lane), user_defined[1] >> 16);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_95_88(qlm, lane), user_defined[1] >> 24);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_103_96(qlm, lane), user_defined[1] >> 32);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_111_104(qlm, lane), user_defined[1] >> 40);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_119_112(qlm, lane), user_defined[1] >> 48);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_127_120(qlm, lane), user_defined[1] >> 56);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_135_128(qlm, lane), user_defined[2]);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_143_136(qlm, lane), user_defined[2] >> 8);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_151_144(qlm, lane), user_defined[2] >> 16);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_159_152(qlm, lane), user_defined[2] >> 24);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_167_160(qlm, lane), user_defined[2] >> 32);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_175_168(qlm, lane), user_defined[2] >> 40);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_183_176(qlm, lane), user_defined[2] >> 48);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_191_184(qlm, lane), user_defined[2] >> 56);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_199_192(qlm, lane), user_defined[3]);
			GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_RX_UDP_SHIFT_AMOUNT(qlm, lane), user_defined_length);
			/* Require 63 cycles to match before getting lock */
			GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_BIST_RX_LOCK_CTRL0(qlm, lane),
				c.s.num_cycles_7_0 = 0x3f);
			/* Require 63 cycles to not match before losing lock */
			GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_BIST_RX_LOSS_LOCK_CTRL0(qlm, lane),
				c.s.num_cycles_7_0 = 0x3f);
			/* Clear error counter */
			GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_BIST_RX_CTRL(qlm, lane),
				c.s.stop_error_count = 1;
				c.s.clear_ber = 1);
			/* Allow re-lock */
			GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_BIST_RX_LOSS_LOCK_CTRL4(qlm, lane),
				c.s.stop_on_loss_lock = 0);
			/* Start receive */
			GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_BIST_RX_CTRL(qlm, lane),
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
int _qlm_gserj_disable_prbs(int qlm, int qlm_lane)
{
	int num_lanes = get_num_lanes(qlm);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		if ((qlm_lane != -1) && (qlm_lane != lane))
			continue;
		/* Stop receive */
		GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_BIST_RX_CTRL(qlm, lane),
			c.s.stop_error_count = 1;
			c.s.clear_ber = 1;
			c.s.en = 0);
		/* Stop transmit */
		GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_TOP_DPL_TXDP_CTRL1(qlm, lane),
			c.s.tx_dmux2_sel = 0);
		GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_BIST_TX_CTRL(qlm, lane),
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
uint64_t _qlm_gserj_get_prbs_errors(int qlm, int lane, int clear)
{
	GSER_CSR_INIT(status, CAVM_GSERJX_LNX_BIST_RX_STATUS(qlm, lane));
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
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_BIST_RX_CTRL(qlm, lane),
		c.s.stop_error_count = 1);

	/* Read the 24 bit counter */
	GSER_CSR_INIT(status0, CAVM_GSERJX_LNX_BIST_RX_BER_STATUS0(qlm, lane));
	GSER_CSR_INIT(status1, CAVM_GSERJX_LNX_BIST_RX_BER_STATUS1(qlm, lane));
	GSER_CSR_INIT(status2, CAVM_GSERJX_LNX_BIST_RX_BER_STATUS2(qlm, lane));

	/* Clear the counter */
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_BIST_RX_CTRL(qlm, lane),
		c.s.clear_ber = 1);
	/* Restart the counter */
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_BIST_RX_CTRL(qlm, lane),
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
void qlm_gserj_inject_prbs_error(int qlm, int lane)
{
	GSER_CSR_DEFINE(ctrl0, CAVM_GSERJX_LNX_BIST_TX_BER_CTRL0(0, 0));
	GSER_CSR_DEFINE(ctrl1, CAVM_GSERJX_LNX_BIST_TX_BER_CTRL1(0, 0));
	GSER_CSR_DEFINE(ctrl2, CAVM_GSERJX_LNX_BIST_TX_BER_CTRL2(0, 0));
	GSER_CSR_DEFINE(ctrl3, CAVM_GSERJX_LNX_BIST_TX_BER_CTRL3(0, 0));
	GSER_CSR_DEFINE(ctrl4, CAVM_GSERJX_LNX_BIST_TX_BER_CTRL4(0, 0));
	GSER_CSR_DEFINE(ctrl5, CAVM_GSERJX_LNX_BIST_TX_BER_CTRL5(0, 0));
	GSER_CSR_DEFINE(ctrl6, CAVM_GSERJX_LNX_BIST_TX_BER_CTRL6(0, 0));

	ctrl0.u = 0; /* Control */
	ctrl1.u = 0; /* Timer bits 7:0 */
	ctrl2.u = 0; /* Timer bits 15:8 */
	ctrl3.u = 0; /* Error bits 7:0 */
	ctrl4.u = 0; /* Error bits 15:8 */
	ctrl5.u = 0; /* Error bits 23:16 */
	ctrl6.u = 0; /* Error bits 31:24 */

	/* Clear any previous state */
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL6(qlm, lane), ctrl6.u);
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL5(qlm, lane), ctrl5.u);
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL4(qlm, lane), ctrl4.u);
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL3(qlm, lane), ctrl3.u);
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL2(qlm, lane), ctrl2.u);
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL1(qlm, lane), ctrl1.u);
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL0(qlm, lane), ctrl0.u);

	/* LSB bit is where we inject the error */
	ctrl3.s.bit_error_field_7_0 = 1;

	/* Inject single error without the timer */
	ctrl0.s.mode = 1;

	/* Write ctrl0 last as it starts injection */
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL6(qlm, lane), ctrl6.u);
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL5(qlm, lane), ctrl5.u);
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL4(qlm, lane), ctrl4.u);
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL3(qlm, lane), ctrl3.u);
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL2(qlm, lane), ctrl2.u);
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL1(qlm, lane), ctrl1.u);
	GSER_CSR_WRITE(CAVM_GSERJX_LNX_BIST_TX_BER_CTRL0(qlm, lane), ctrl0.u);
}

/**
 * Implementation of NED Loopback with Internal BIST PRBS Generator/Checker.
 * Based on CNF95XX_B0_GSERJ_Programming_v1p2.pdf
 *
 * @param node
 * @param module
 * @param lane
 * @param enable
 *
 * @return Zero on success, negative on failure
 */
static int qlm_gserj_ned_loopback(int module, int lane, bool enable)
{
	/* Skip running if we aren't changing anything */
	GSER_CSR_INIT(dpl_rxdp_ctrl1, CAVM_GSERJX_LNX_TOP_DPL_RXDP_CTRL1(module, lane));
	if (enable && dpl_rxdp_ctrl1.s.rx_dmux_sel)
		return 0;
	if (!enable && !dpl_rxdp_ctrl1.s.rx_dmux_sel)
		return 0;

	/* The steps to enable GSERJ NED loopback with the PHY internal BIST PRBS
		geneator and checker are described below. */
	/* 1. Disable the firmware receive control state machine.
		Write GSERJ(0..2)_LN(0..3)_FEATURE_TEST_CFG0
			RX_CTRL_DIS=1 */
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_FEATURE_TEST_CFG0(module, lane),
		c.s.rx_ctrl_dis = enable);

	/* 2. Bring-up the lane to the active power state, refer to the steps in
		Section 1.4 GSERJ Software Initialization. */
	/* Already done */

	/* 3. If performing the NED loopback at the PHY_RATE3 data rate of
		1.25Gbps, 1000BASE-X, or SGMII rate program the following register
		to configure the Rate 3 clocks for the receive gearbox. For all other
		data rates skip this step.
		Write GSERJ(0..2)_CM0_TOP_AFE_CMCP_CTRL4
			CMCP_CLKDIV_PLL3DIV=4âh1 //cm0_clk_rate3div set divider to 10
			CMCP_CLKDIV_PLL3=4âh1 //cm0_clk_rate3 set divider to 8 */
	GSER_CSR_INIT(bcfg, CAVM_GSERJX_LANEX_CONTROL_BCFG(module, lane));
	if (bcfg.s.ln_ctrl_tx_rate == 2)
	{
		GSER_CSR_MODIFY(c, CAVM_GSERJX_CM0_TOP_AFE_CMCP_CTRL4(module),
			c.s.cmcp_clkdiv_pll3div = 1;
			c.s.cmcp_clkdiv_pll3 = 1);
	}

	/* 4. Read GSERJ(0..2)_LANE(0..3)_CONTROL_BCFG[LN_CTRL_TX_RATE]
		Use the LN_CTRL_TX_RATE value as an index into Table 6 below
		Write the corresponding lane GSERJ(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL]
		field with the value shown in Table 6:
		GSERJ(0..2)_LANE(0..3)_CONTROL_BCFG[LN_TX_CTRL_RATE] 0 1 2 4 5 6
		GSERJ(0..2)_PHY0_TOP_CLOCK_LN0_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL] 2âh3 2âh2 2âh1 2âh3 2âh2 2âh1
		GSERJ(0..2)_PHY0_TOP_CLOCK_LN1_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL] 2âh3 2âh2 2âh1 2âh3 2âh2 2âh1
		GSERJ(0..2)_PHY0_TOP_CLOCK_LN2_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL] 2âh3 2âh2 2âh1 2âh3 2âh2 2âh1
		GSERJ(0..2)_PHY0_TOP_CLOCK_LN3_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL] 2âh3 2âh2 2âh1 2âh3 2âh2 2âh1
		Table 6 â Clock override values for NED loopback with BIST Enabled */
	int ctrl_src_ovr_val;
	switch (bcfg.s.ln_ctrl_tx_rate)
	{
		case 0:
		case 4:
			ctrl_src_ovr_val = enable ? 3 : 0;
			break;
		case 1:
		case 5:
			ctrl_src_ovr_val = enable ? 2 : 0;
			break;
		case 2:
		case 6:
		default:
			ctrl_src_ovr_val = enable ? 1 : 0;
			break;
	}
	switch (lane)
	{
		case 0:
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_CLOCK_LN0_CM0_CLK_GS_MODE_CTRL0_RSVD(module),
				c.s.ctrl_src_ovr_en = enable;
				c.s.ctrl_src_ovr_val = ctrl_src_ovr_val);
			break;
		case 1:
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_CLOCK_LN1_CM0_CLK_GS_MODE_CTRL0_RSVD(module),
				c.s.ctrl_src_ovr_en = enable;
				c.s.ctrl_src_ovr_val = ctrl_src_ovr_val);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_CLOCK_LN2_CM0_CLK_GS_MODE_CTRL0_RSVD(module),
				c.s.ctrl_src_ovr_en = enable;
				c.s.ctrl_src_ovr_val = ctrl_src_ovr_val);
			break;
		case 3:
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_CLOCK_LN3_CM0_CLK_GS_MODE_CTRL0_RSVD(module),
				c.s.ctrl_src_ovr_en = enable;
				c.s.ctrl_src_ovr_val = ctrl_src_ovr_val);
			break;
#endif
	}

	/* 5. Configure the clocks for the Transmit FIFO and Receiver gearbox clocks.
		Write GSERJ(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXB
			CTRL_SRC_OVR_VAL=2âh2 //Select cmu clock
		Write GSERJ(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXF
			CTRL_SRC_OVR_VAL=2âh2 //Select cmu clock
		Write GSERJ(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXB
			CTRL_SRC_OVR_VAL=2âh2 //Select cmu clock
		Write GSERJ(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXF
			CTRL_SRC_SEL=2âh2 //Select cmu clock */
	cavm_gserjx_phy0_top_clock_ln0_clk_txb_t clk_txb = {.u = 0};
	cavm_gserjx_phy0_top_clock_ln0_clk_txf_t clk_txf = {.u = 0};
	cavm_gserjx_phy0_top_clock_ln0_clk_rxb_t clk_rxb = {.u = 0};
	cavm_gserjx_phy0_top_clock_ln0_clk_rxf_t clk_rxf = {.u = 0};
	clk_txb.s.ctrl_src_ovr_val = enable ? 3 : 0;
	clk_txf.s.ctrl_src_ovr_val = enable ? 3 : 0;
	clk_rxb.s.ctrl_src_ovr_val = enable ? 2 : 0;
	clk_rxf.s.ctrl_src_sel = enable ? 0 : 3;
	switch (lane)
	{
		case 0:
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN0_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN0_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN0_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN0_CLK_RXF(module), clk_rxf.u);
			break;
		case 1:
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN1_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN1_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN1_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN1_CLK_RXF(module), clk_rxf.u);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN2_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN2_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN2_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN2_CLK_RXF(module), clk_rxf.u);
			break;
		case 3:
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN3_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN3_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN3_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN3_CLK_RXF(module), clk_rxf.u);
			break;
#endif
	}

	/* 6. Enable the clocks for the Transmit FIFO and Receiver gearbox clocks.
		Write GSERJ(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXB
			CTRL_SRC_OVR_EN=1
		Write GSERJ(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXF
			CTRL_SRC_OVR_EN=1
		Write GSERJ(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXB
			CTRL_SRC_OVR_EN=1
		Note there is no CTRL_SRC_OVR_EN bit for the CLK_RXF gearbox clock, it
		is controlled only by the GSERJ(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXF[CTRL_SRC_SEL]
		field in Step 5 above. */
	clk_txb.s.ctrl_src_ovr_en = enable;
	clk_txf.s.ctrl_src_ovr_en = enable;
	clk_rxb.s.ctrl_src_ovr_en = enable;
	switch (lane)
	{
		case 0:
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN0_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN0_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN0_CLK_RXB(module), clk_rxb.u);
			break;
		case 1:
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN1_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN1_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN1_CLK_RXB(module), clk_rxb.u);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN2_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN2_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN2_CLK_RXB(module), clk_rxb.u);
			break;
		case 3:
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN3_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN3_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_TOP_CLOCK_LN3_CLK_RXB(module), clk_rxb.u);
			break;
#endif
	}

	/* 7. Wait 1 microsecond for the clocks to settle. */
	gser_wait_usec(1);

	/* 8. Enable NED loopback on the selected lane.
		Write GSERJ(0..2)_LN(0..3)_TOP_DPL_RXDP_CTRL1
			RX_DMUX_SEL=1 */
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_TOP_DPL_RXDP_CTRL1(module, lane),
		c.s.rx_dmux_sel = enable);

	/* 9. Enable the clock gaters for NED loopback. One per lane.
		Write GSERJ(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CG_CTRL
			CLK_RX=1 */
	switch (lane)
	{
		case 0:
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_CLOCK_LN0_CG_CTRL(module),
				c.s.clk_rx = enable);
			break;
		case 1:
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_CLOCK_LN1_CG_CTRL(module),
				c.s.clk_rx = enable);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_CLOCK_LN2_CG_CTRL(module),
				c.s.clk_rx = enable);
			break;
		case 3:
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_CLOCK_LN3_CG_CTRL(module),
				c.s.clk_rx = enable);
			break;
#endif
	}

	/* 10. Wait 1 microsecond for the clocks to settle. */
	gser_wait_usec(1);

	/* 11. Start the JESD or PHY BIST PRBS Generator */
	/* Not needed */

	/* 12. Drive the LN_STAT_RXVALID signal output on the selected lane.
		Write GSERJ(0..2)_LN(0..3)_TOP_LN_STAT_CTRL0
			RXVALID=1 */
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_TOP_LN_STAT_CTRL0(module, lane),
		c.s.rxvalid = 1);

	/* 13. Read/Poll for LN_STAT_RXVALID=1.
		GSERJ(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STAT_RXVALID=1 Rx CDR is locked
			LN_STAT_LOS Ignore the LN_STAT_LOS flag in NED loopback. */
	if (enable && GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERJX_LANEX_STATUS_BSTS(module, lane), GSERJX_STATUS_BSTS_LN_STAT_RXVALID, ==, 1, 10000))
		gser_warn("GSERJ%d.%d: Wait for GSERJX_LANEX_STATUS_BSTS[LN_STAT_RXVALID]=1 timeout\n", module, lane);

	/* 14. Complete the bring-up of the Lane in NED loopback mode. */
	/* Not needed */

	/* 15. Reset the Receiver Gearbox FIFO in each lane configured for NED
		loopback. This ensures the Rx Gearbox FIFO read and write pointers are
		properly initialized.
		Step 1. For each GSERJ lane configured for NED loopback first check
			that the PHY firmware has released the Rx gearbox FIFO from reset.
			i. Lane 0 Read/poll
				GSERJ(0..2)_PHY0_TOP_RESET_CTRL_LN0_RSVD.RXDP_SW_RESET=1âb0
			ii. Lane 1 Read/poll
				GSERJ(0..2)_PHY0_TOP_RESET_CTRL_LN1_RSVD.RXDP_SW_RESET=1âb0
			iii. Lane 2 Read/poll
				GSERJ(0..2)_PHY0_TOP_RESET_CTRL_LN2_RSVD.RXDP_SW_RESET=1âb0
			iv. Lane 3 Read/poll
				GSERJ(0..2)_PHY0_TOP_RESET_CTRL_LN3_RSVD.RXDP_SW_RESET=1âb0 */
	switch (lane)
	{
		case 0:
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERJX_PHY0_TOP_RESET_CTRL_LN0_RSVD(module), GSERJX_PHY0_TOP_RESET_CTRL_LN0_RSVD_RXDP_SW_RESET, ==, 0, 10000))
				gser_error("GSERJ%d.%d: Wait for GSERJX_PHY0_TOP_RESET_CTRL_LN0_RSVD[rxdp_sw_reset]=0 timeout\n", module, lane);
			break;
		case 1:
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERJX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module), GSERJX_PHY0_TOP_RESET_CTRL_LN1_RSVD_RXDP_SW_RESET, ==, 0, 10000))
				gser_error("GSERJ%d.%d: Wait for GSERJX_PHY0_TOP_RESET_CTRL_LN1_RSVD[rxdp_sw_reset]=0 timeout\n", module, lane);
			break;
#if QUAD_LANE
		case 2:
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERJX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module), GSERJX_PHY0_TOP_RESET_CTRL_LN2_RSVD_RXDP_SW_RESET, ==, 0, 10000))
				gser_error("GSERJ%d.%d: Wait for GSERJX_PHY0_TOP_RESET_CTRL_LN2_RSVD[rxdp_sw_reset]=0 timeout\n", module, lane);
			break;
		case 3:
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERJX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module), GSERJX_PHY0_TOP_RESET_CTRL_LN3_RSVD_RXDP_SW_RESET, ==, 0, 10000))
				gser_error("GSERJ%d.%d: Wait for GSERJX_PHY0_TOP_RESET_CTRL_LN3_RSVD[rxdp_sw_reset]=0 timeout\n", module, lane);
			break;
#endif
	}

	/*  Step 2. For each GSERJ lane configured for NED loopback disable the Rx
			gearbox FIFO, then reset the FIFO, then re-enable the FIFO.
			i. Lane 0:
				Write GSERJ(0..2)_LN(0)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=0
				Write GSERJ(0..2)_PHY0_TOP_RESET_CTRL_LN0_RSVD.RXDP_SWRESET=1
				Write GSERJ(0..2)_PHY0_TOP_RESET_CTRL_LN0_RSVD.RXDP_SWRESET=0
				Write GSERJ(0..2)_LN(0)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=1
			ii. Lane 1:
				Write GSERJ(0..2)_LN(1)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=0
				Write GSERJ(0..2)_PHY0_TOP_RESET_CTRL_LN1_RSVD.RXDP_SWRESET=1
				Write GSERJ(0..2)_PHY0_TOP_RESET_CTRL_LN1_RSVD.RXDP_SWRESET=0
				Write GSERJ(0..2)_LN(1)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=1
			iii. Lane 2:
				Write GSERJ(0..2)_LN(2)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=0
				Write GSERJ(0..2)_PHY0_TOP_RESET_CTRL_LN2_RSVD.RXDP_SWRESET=1
				Write GSERJ(0..2)_PHY0_TOP_RESET_CTRL_LN2_RSVD.RXDP_SWRESET=0
				Write GSERJ(0..2)_LN(2)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=1
			iv. Lane 3:
				Write GSERJ(0..2)_LN(3)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=0
				Write GSERJ(0..2)_PHY0_TOP_RESET_CTRL_LN3_RSVD.RXDP_SWRESET=1
				Write GSERJ(0..2)_PHY0_TOP_RESET_CTRL_LN3_RSVD.RXDP_SWRESET=0
				Write GSERJ(0..2)_LN(3)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=1 */
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_TOP_DPL_RXDP_CTRL1(module, lane),
		c.s.rx_fifo_en = 0);
	switch (lane)
	{
		case 0:
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_RESET_CTRL_LN0_RSVD(module),
				c.s.rxdp_sw_reset = 1);
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_RESET_CTRL_LN0_RSVD(module),
				c.s.rxdp_sw_reset = 0);
			break;
		case 1:
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module),
				c.s.rxdp_sw_reset = 1);
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module),
				c.s.rxdp_sw_reset = 0);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_RESET_CTRL_LN2_RSVD(module),
				c.s.rxdp_sw_reset = 1);
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_RESET_CTRL_LN2_RSVD(module),
				c.s.rxdp_sw_reset = 0);
			break;
		case 3:
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_RESET_CTRL_LN3_RSVD(module),
				c.s.rxdp_sw_reset = 1);
			GSER_CSR_MODIFY(c, CAVM_GSERJX_PHY0_TOP_RESET_CTRL_LN3_RSVD(module),
				c.s.rxdp_sw_reset = 0);
			break;
#endif
	}
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_TOP_DPL_RXDP_CTRL1(module, lane),
		c.s.rx_fifo_en = 1);
	return 0;
}

/**
 * Implementation of Far-End Analog (FEA) Loopback
 *
 * @param node
 * @param module
 * @param lane
 *
 * @return Zero on success, negative on failure
 */
static int qlm_gserj_fea_loopback(int module, int lane, bool enable)
{
	/* Skip running if we aren't changing anything */
	GSER_CSR_INIT(afe_loopback_ctrl, CAVM_GSERJX_LNX_TOP_AFE_LOOPBACK_CTRL(module, lane));
	if (enable && afe_loopback_ctrl.s.loopback_fea_en)
		return 0;
	if (!enable && !afe_loopback_ctrl.s.loopback_fea_en)
		return 0;

	/* These steps are from the Salina platform programming guide */
	/* 1. Bring up the lane to the ACTIVE power state as in the normal operating
	   mode (i.e. lnX_tx_rdy_o and lnX_rx_rdy_o asserted). Valid data must be
	   transmitted into RX pins (lnX_rxp/m_i). */
	/* Already been done when the SERDES was configured */

	/* 2. Set the lane top register field AFE_LOOPBACK_CTRL.LOOPBACK_RXCLK_EN.
	   (Note: This is a required step for simulation testing, but is generally
	   not used in silicon testing. This step will use the recovered clock to
	   drive the transmit driver, therefore it will result in much higher jitter
	   content on the TX signal in silicon). */
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_TOP_AFE_LOOPBACK_CTRL(module, lane),
		c.s.loopback_rxclk_en = enable);

	/* 3. Wait until lnX_stat_rxvalid_o PHY top-level interface signal to be set
	   to 1. If desired, a lane top register field LN_STAT_CTRL0.RXVALID can be
	   polled. By this time, CDR is locked and received data are valid. */
	if (enable && GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERJX_LANEX_STATUS_BSTS(module, lane), GSERJX_LNX_TOP_LN_STAT_CTRL0_RXVALID, ==, 1, 10000))
		gser_warn("GSERJ%d.%d: No receive signal detected\n", module, lane);

	/* 4. Enable FEA loopback by setting a lane top register field
	   AFE_LOOPBACK_CTRL.LOOPBACK_FEA_EN to 0x1. */
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_TOP_AFE_LOOPBACK_CTRL(module, lane),
		c.s.loopback_fea_en = enable);

	/* 5. Set interface data width to 32 bit as this is required for AFE
	   interface by programming lane top registers:
			a. Program register field AFE_TXDP_CTRL0.TXDP_DATA_WIDTH to 0x1
			b. Program register field AFE_RXDP_CTRL1.RXDP_DATA_WIDTH to 0x1 */
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_TOP_AFE_TXDP_CTRL0(module, lane),
		c.s.txdp_data_width = 1);
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_TOP_AFE_RXDP_CTRL1(module, lane),
		c.s.rxdp_data_width = 1);

	/* 6. Recalibrate TXDP clock using the corresponding mailbox command.
			a. Read top mailbox register CMD_FLAG. It should be 0.
			b. Read top mailbox register RSP_FLAG. If it is 1, write a 1 to
				clear it.
			c. Write the lane number (0 to NUM_LANES) to top mailbox register
				CMD_DATA0.
			d. Write 0x81 to top mailbox register CMD.
			e. Wait for top mailbox register RSP_FLAG to read 1.
			f. Read top mailbox register RSP to get the calibrationâs error
				code (0 == no error). Read top mailbox register RSP_DATA0 to
				get the calibrationâs result.
			g. Write 1 to RSP_FLAG to clear it. */
	if (mailbox_command(module, 0x81, lane) != 0)
		return -1;
	uint64_t resp0 = 0;
	uint64_t resp1 = 0;
	if (mailbox_response(module, &resp0, &resp1) != 0)
		gser_warn("GSERJ%d.%d: TXDP recalibrate failed\n", module, lane);
	/* 7. Override tx_en indicator by programming a lane top register
	   LN_CTRL_OVR0. Note that all of its fields must be programmed
	   appropriately.
			a. TX_EN = 0x1
			b. DATA_RXPOLARITY = 0x0
			c.  EDGE_RXPOLARITY = 0x0
			d. EYE_RXPOLARITY = 0x0
			e. OVR_EN = 0x1 (should be programmed last or at the same time as
				all other fields) */
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_TOP_LN_CTRL_OVR0(module, lane),
		c.s.tx_en = 1;
		c.s.eye_rxpolarity = 0;
		c.s.edge_rxpolarity = 0;
		c.s.data_rxpolarity = 0;
		c.s.ovr_en = enable);
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
int qlm_gserj_enable_loop(int qlm, qlm_loop_t loop)
{
	int use_fea = (loop == QLM_LOOP_SHALLOW);
	int use_ned = (loop == QLM_LOOP_CORE);
	/* We haven't implemented near-end analog loopback, so use the digital one
	   instead. I doubt anyone will notice */
	use_ned |= (loop == QLM_LOOP_NEAR_END);

	int num_lanes = get_num_lanes(qlm);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		qlm_gserj_fea_loopback(qlm, lane, use_fea);
		qlm_gserj_ned_loopback(qlm, lane, use_ned);
	}
	return 0;
}

/**
 * Configure the TX tuning parameters for a QLM lane
 *
 * @param node	   Node to configure
 * @param qlm		QLM to configure
 * @param lane	   Lane to configure
 * @param tx_swing   Transmitter Main (C0) equalizer tap coefficient value.
 *				   Programs the SerDes transmitter Main tap. Valid range is
 *				   0(0% swing) to 7(100% swing).
 * @param tx_cpre	Transmitter Pre (C-1) equalizer tap coefficient value.
 *				   Programs the transmitter Pre tap. Valid range is 0(min)
 *				   to 15(max).
 * @param tx_cpost   Transmitter Post (C+1) equalizer tap coefficient value.
 *				   Programs the transmitter Post tap. Valid range is 0(min)
 *				   to 31(max).
 * @param tx_unused1
 * @param tx_unused2
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_tune_lane_tx(int qlm, int lane, int tx_swing, int tx_cpre, int tx_cpost, int tx_unused1, int tx_unused2)
{
	if ((tx_swing < 0) || (tx_swing > 7))
	{
		gser_error("GSERJ%d.%d: Invalid tuning, TX Swing(%d) must be 0 to 7\n",
			qlm, lane, tx_swing);
		return -1;
	}
	if ((tx_cpre < 0) || (tx_cpre > 15))
	{
		gser_error("GSERJ%d.%d: Invalid tuning, TX PRE(%d) must be 0 to 15\n",
			qlm, lane, tx_cpre);
		return -1;
	}
	if ((tx_cpost < 0) || (tx_cpost > 31))
	{
		gser_error("GSERJ%d.%d: Invalid tuning, TX POST(%d) must be 0 to 31\n",
			qlm, lane, tx_cpost);
		return -1;
	}

	GSER_CSR_INIT(ctrl0, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_CTRL0(qlm, lane));
	GSER_CSR_INIT(status0, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_STATUS0(qlm, lane));
	if (ctrl0.s.req || status0.s.ack)
	{
		gser_error("GSERJ%d.%d: TXEQ coefficient request in progress\n", qlm, lane);
		return -1;
	}

	/* Load the new value */
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_CTRL1(qlm, lane),
		c.s.txeq_c1 = tx_cpost);
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_CTRL3(qlm, lane),
		c.s.txeq_cm1 = tx_cpre);
	GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_CTRL5(qlm, lane),
		c.s.drv_swing = tx_swing);

	/* Only request Tx coefficient update if CPU is out of reset */
	GSER_CSR_INIT(gserjx_init_ctl, CAVM_GSERJX_COMMON_PHY_CTRL_BCFG(qlm));
	if (!gserjx_init_ctl.s.cpu_reset)
	{
		/* Request an update */
		GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_CTRL0(qlm, lane),
			c.s.req = 1);
		/* Wait for ACK */
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_STATUS0(qlm, lane), GSERJX_LNX_DRV_REFCLK_TXEQ_STATUS0_ACK, ==, 1, 10000))
		{
			gser_error("GSERJ%d.%d: TXEQ coefficient request timeout\n", qlm, lane);
			return -1;
		}
		/* Clear update request */
		GSER_CSR_MODIFY(c, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_CTRL0(qlm, lane),
			c.s.req = 0);
	}

	return 0;
}

/**
 * Get the TX tuning parameters for a QLM lane
 *
 * @param node	   Node to configure
 * @param qlm		QLM to configure
 * @param lane	   Lane to configure
 * @param tx_swing   Transmitter Main (C0) equalizer tap coefficient value.
 *				   Programs the SerDes transmitter Main tap. Valid range is
 *				   0(0% swing) to 7(100% swing).
 * @param tx_cpre	Transmitter Pre (C-1) equalizer tap coefficient value.
 *				   Programs the transmitter Pre tap. Valid range is 0(min)
 *				   to 15(max).
 * @param tx_cpost   Transmitter Post (C+1) equalizer tap coefficient value.
 *				   Programs the transmitter Post tap. Valid range is 0(min)
 *				   to 31(max).
 * @param tx_unused1
 * @param tx_unused2
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_get_tune_lane_tx(int qlm, int lane, int *tx_swing, int *tx_cpre, int *tx_cpost, int *tx_unused1, int *tx_unused2)
{
	GSER_CSR_INIT(ctrl1, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_CTRL1(qlm, lane));
	GSER_CSR_INIT(ctrl3, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_CTRL3(qlm, lane));
	GSER_CSR_INIT(ctrl5, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_CTRL5(qlm, lane));
	*tx_swing = ctrl5.s.drv_swing;
	*tx_cpre = ctrl3.s.txeq_cm1;
	*tx_cpost = ctrl1.s.txeq_c1;
	*tx_unused1 = -1;
	*tx_unused2 = -1;
	return 0;
}

/**
 * This function converts a reflected binary
 * Gray code number to a binary number.
 * Each Gray code bit is exclusive-ored with all
 * more significant bits.
 *
 * @param num
 *
 * @return
 */
static unsigned int gray2binary(unsigned int num)
{
	unsigned int mask = num >> 1;
	while (mask != 0)
	{
		num = num ^ mask;
		mask = mask >> 1;
	}
	return num;
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
void _qlm_gserj_display_settings(int qlm, int qlm_lane, bool show_tx, bool show_rx, char *buf, int size)
{
	#define printf(...) \
	do {                                                            \
		if (buf == NULL) {                                      \
			printf(__VA_ARGS__);                            \
		} else {                                                \
		int _chars = snprintf(buf, size, __VA_ARGS__);  \
		buf += _chars;                                  \
		size -= _chars;                                 \
		}                                                       \
	} while(0)


	printf("N0.GSERJ%d Lane %d:\n", qlm, qlm_lane);
	if (show_tx)
	{
		printf("  CMU Status:\n");
		GSER_CSR_INIT(cm0_top_phy_if_status, CAVM_GSERJX_CM0_TOP_PHY_IF_STATUS(qlm));
		GSER_CSR_INIT(bcfg, CAVM_GSERJX_COMMON_PHY_CTRL_BCFG(qlm));
		printf("	CMU OK=%d, PHY Rate1=0x%x, Rate2=0x%x\n",
			cm0_top_phy_if_status.s.cmu_ok, bcfg.s.phy_ctrl_rate1, bcfg.s.phy_ctrl_rate2);
		GSER_CSR_INIT(status1, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_STATUS1(qlm, qlm_lane));
		GSER_CSR_INIT(status2, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_STATUS2(qlm, qlm_lane));
		GSER_CSR_INIT(ctrl5, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_CTRL5(qlm, qlm_lane));
		GSER_CSR_INIT(status3, CAVM_GSERJX_LNX_DRV_REFCLK_TXEQ_STATUS3(qlm, qlm_lane));
		printf("  Transmitter Status:\n");
		printf("	SWING=%d, PRE=%d, MAIN=%d, POST=%d\n", ctrl5.s.drv_swing,
			status2.s.txeq_cm1, status3.s.txeq_c0, status1.s.txeq_c1);
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
				att = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_PLE_ATT_O_READ_VAL_2_0_RSVD(qlm));
				apg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_GN_APG_O_READ_VAL_2_0_RSVD(qlm));
				lfg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_LFG_O_READ_VAL_4_0_RSVD(qlm));
				hfg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_HFG_O_READ_VAL_4_0_RSVD(qlm));
				mbf = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_MBF_O_READ_VAL_3_0_RSVD(qlm));
				mbg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_MBG_O_READ_VAL_3_0_RSVD(qlm));
				break;
			case 1:
				att = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_PLE_ATT_O_READ_VAL_2_0_RSVD(qlm));
				apg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_GN_APG_O_READ_VAL_2_0_RSVD(qlm));
				lfg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_LFG_O_READ_VAL_4_0_RSVD(qlm));
				hfg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_HFG_O_READ_VAL_4_0_RSVD(qlm));
				mbf = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_MBF_O_READ_VAL_3_0_RSVD(qlm));
				mbg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_MBG_O_READ_VAL_3_0_RSVD(qlm));
				break;
#if QUAD_LANE
			case 2:
				att = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_PLE_ATT_O_READ_VAL_2_0_RSVD(qlm));
				apg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_GN_APG_O_READ_VAL_2_0_RSVD(qlm));
				lfg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_LFG_O_READ_VAL_4_0_RSVD(qlm));
				hfg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_HFG_O_READ_VAL_4_0_RSVD(qlm));
				mbf = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_MBF_O_READ_VAL_3_0_RSVD(qlm));
				mbg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_MBG_O_READ_VAL_3_0_RSVD(qlm));
				break;
			case 3:
				att = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_PLE_ATT_O_READ_VAL_2_0_RSVD(qlm));
				apg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_GN_APG_O_READ_VAL_2_0_RSVD(qlm));
				lfg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_LFG_O_READ_VAL_4_0_RSVD(qlm));
				hfg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_HFG_O_READ_VAL_4_0_RSVD(qlm));
				mbf = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_MBF_O_READ_VAL_3_0_RSVD(qlm));
				mbg = GSER_CSR_READ(CAVM_GSERJX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_MBG_O_READ_VAL_3_0_RSVD(qlm));
				break;
#endif
		}
		/* These values are encoded as gray codes https://en.wikipedia.org/wiki/Gray_code */
		att = gray2binary(att);
		apg = gray2binary(apg);
		lfg = gray2binary(lfg);
		hfg = gray2binary(hfg);
		mbf = gray2binary(mbf);
		mbg = gray2binary(mbg);

		printf("  Receiver Status:\n");
		printf("	ATT=%d, APG=%d, LFG=%d, HFG=%d, MBF=%d, MBG=%d\n",
			att, apg, lfg, hfg, mbf, mbg);

		/* Read DFE taps */
		GSER_CSR_INIT(tap1_even, CAVM_GSERJX_LNX_DFE_REFCLK_TAP_VAL_STATUS0(qlm, qlm_lane));
		GSER_CSR_INIT(tap1_odd, CAVM_GSERJX_LNX_DFE_REFCLK_TAP_VAL_STATUS1(qlm, qlm_lane));
		GSER_CSR_INIT(tap2, CAVM_GSERJX_LNX_DFE_REFCLK_TAP_VAL_STATUS2(qlm, qlm_lane));
		GSER_CSR_INIT(tap3, CAVM_GSERJX_LNX_DFE_REFCLK_TAP_VAL_STATUS3(qlm, qlm_lane));
		GSER_CSR_INIT(tap4, CAVM_GSERJX_LNX_DFE_REFCLK_TAP_VAL_STATUS4(qlm, qlm_lane));
		GSER_CSR_INIT(tap5, CAVM_GSERJX_LNX_DFE_REFCLK_TAP_VAL_STATUS5(qlm, qlm_lane));
		GSER_CSR_INIT(tap6, CAVM_GSERJX_LNX_DFE_REFCLK_TAP_VAL_STATUS6(qlm, qlm_lane));
		GSER_CSR_INIT(tap7, CAVM_GSERJX_LNX_DFE_REFCLK_TAP_VAL_STATUS7(qlm, qlm_lane));
		GSER_CSR_INIT(tap8, CAVM_GSERJX_LNX_DFE_REFCLK_TAP_VAL_STATUS8(qlm, qlm_lane));
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
	GSER_CSR_INIT(flag, CAVM_GSERJX_PHY0_MB_CMD_FLAG(qlm));
	if (flag.s.data)
	{
		gser_error("GSERJ%d: Mailbox already has a command\n", qlm);
		return -1;
	}

	GSER_CSR_INIT(rsp_flag, CAVM_GSERJX_PHY0_MB_RSP_FLAG(qlm));
	if (rsp_flag.s.data)
	{
		gser_error("GSERJ%d: Mailbox already has a response\n", qlm);
		return -1;
	}

	GSER_CSR_WRITE(CAVM_GSERJX_PHY0_MB_CMD_DATA0(qlm), gser_extract(args, 0, 8));
	GSER_CSR_WRITE(CAVM_GSERJX_PHY0_MB_CMD_DATA1(qlm), gser_extract(args, 8, 8));
	GSER_CSR_WRITE(CAVM_GSERJX_PHY0_MB_CMD_DATA2(qlm), gser_extract(args, 16, 8));
	GSER_CSR_WRITE(CAVM_GSERJX_PHY0_MB_CMD_DATA3(qlm), gser_extract(args, 24, 8));
	GSER_CSR_WRITE(CAVM_GSERJX_PHY0_MB_CMD_DATA4(qlm), gser_extract(args, 32, 8));
	GSER_CSR_WRITE(CAVM_GSERJX_PHY0_MB_CMD_DATA5(qlm), gser_extract(args, 40, 8));
	GSER_CSR_WRITE(CAVM_GSERJX_PHY0_MB_CMD_DATA6(qlm), gser_extract(args, 48, 8));
	GSER_CSR_WRITE(CAVM_GSERJX_PHY0_MB_CMD_DATA7(qlm), gser_extract(args, 56, 8));
	GSER_CSR_WRITE(CAVM_GSERJX_PHY0_MB_CMD(qlm), cmd);

	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERJX_PHY0_MB_CMD_FLAG(qlm), GSERJX_PHY0_MB_CMD_FLAG_DATA, ==, 0, 1000000))
	{
		gser_error("GSERJ%d: Mailbox stuck on command\n", qlm);
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
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERJX_PHY0_MB_RSP_FLAG(qlm), GSERJX_PHY0_MB_RSP_FLAG_DATA, ==, 1, 500000))
	{
		gser_error("GSERJ%d: Mailbox has no response\n", qlm);
		return -1;
	}

	/* Read the response */
	GSER_CSR_INIT(rsp, CAVM_GSERJX_PHY0_MB_RSP(qlm));
	GSER_CSR_INIT(data0, CAVM_GSERJX_PHY0_MB_RSP_DATA0(qlm));
	GSER_CSR_INIT(data1, CAVM_GSERJX_PHY0_MB_RSP_DATA1(qlm));
	GSER_CSR_INIT(data2, CAVM_GSERJX_PHY0_MB_RSP_DATA2(qlm));
	GSER_CSR_INIT(data3, CAVM_GSERJX_PHY0_MB_RSP_DATA3(qlm));
	GSER_CSR_INIT(data4, CAVM_GSERJX_PHY0_MB_RSP_DATA4(qlm));
	GSER_CSR_INIT(data5, CAVM_GSERJX_PHY0_MB_RSP_DATA5(qlm));
	GSER_CSR_INIT(data6, CAVM_GSERJX_PHY0_MB_RSP_DATA6(qlm));
	GSER_CSR_INIT(data7, CAVM_GSERJX_PHY0_MB_RSP_DATA7(qlm));
	GSER_CSR_INIT(data8, CAVM_GSERJX_PHY0_MB_RSP_DATA8(qlm));
	GSER_CSR_INIT(data9, CAVM_GSERJX_PHY0_MB_RSP_DATA9(qlm));
	GSER_CSR_INIT(data10, CAVM_GSERJX_PHY0_MB_RSP_DATA10(qlm));
	GSER_CSR_INIT(data11, CAVM_GSERJX_PHY0_MB_RSP_DATA11(qlm));
	GSER_CSR_INIT(data12, CAVM_GSERJX_PHY0_MB_RSP_DATA12(qlm));
	GSER_CSR_INIT(data13, CAVM_GSERJX_PHY0_MB_RSP_DATA13(qlm));
	GSER_CSR_INIT(data14, CAVM_GSERJX_PHY0_MB_RSP_DATA14(qlm));
	GSER_CSR_INIT(data15, CAVM_GSERJX_PHY0_MB_RSP_DATA15(qlm));

	/* Tell firmware we read the response */
	GSER_CSR_WRITE(CAVM_GSERJX_PHY0_MB_RSP_FLAG(qlm), 1);

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
 * Clean the mailbox to the serdes firmware
 *
 * @param node   Node to access
 * @param qlm	QLM to access
 *
 * @return Zero on success, negative on failure
 */
static int mailbox_cleanup(int qlm)
{
	/* The following sequence cleans the mailbox:
	   1) Read responses until we don't get one for 500ms
	   2) Issue a clear command
	   3) Read responses until we don't get one for 500ms */
	for (int count = 0; count < 2; count++)
	{
		/* Read responses until we don't get one for 500ms */
		while (1)
		{
			/* Wait for a response */
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERJX_PHY0_MB_RSP_FLAG(qlm), GSERJX_PHY0_MB_RSP_FLAG_DATA, ==, 1, 500000))
				break;
			/* Tell firmware we read the response */
			GSER_CSR_WRITE(CAVM_GSERJX_PHY0_MB_RSP_FLAG(qlm), 1);
			gser_wait_usec(1000);
		}
		/* Issue a clear command on first loop */
		if (count == 0)
			mailbox_command(qlm, 0x00, 0);
	}
	return 0;
}

typedef struct
{
	gser_node_t node;
	int qlm;
	int lane;
	int ber;
	int buffered_bits;
	int last_resp;
	uint64_t buffer[3]; /* 2 is latest MSB data, 0 is future samples */
} eye_temp_state_t;

static uint64_t get_eye_sample(eye_temp_state_t *s)
{
	if ((s->buffered_bits < s->ber) && (s->last_resp == 0))
	{
		s->last_resp = mailbox_response(s->qlm, &s->buffer[1], &s->buffer[0]);
		s->buffer[1] = gser_be64_to_cpu(s->buffer[1]);
		s->buffer[0] = gser_be64_to_cpu(s->buffer[0]);
		if (s->buffered_bits)
		{
			int bits = 64 - s->buffered_bits;
			s->buffer[2] |= s->buffer[1] >> s->buffered_bits;
			s->buffer[1] = (s->buffer[1] << bits) | (s->buffer[0] >> s->buffered_bits);
			s->buffer[0] <<= bits;
		}
		else
		{
			s->buffer[2] = s->buffer[1];
			s->buffer[1] = s->buffer[0];
			s->buffer[0] = 0;
		}
		s->buffered_bits += 128;
	}

	int bits = 64 - s->ber;
	uint64_t result = s->buffer[2] >> bits;
	s->buffer[2] = (s->buffer[2] << s->ber) | (s->buffer[1] >> bits);
	s->buffer[1] = (s->buffer[1] << s->ber) | (s->buffer[0] >> bits);
	s->buffer[0] <<= s->ber;
	s->buffered_bits -= s->ber;
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
int _qlm_gserj_eye_capture(int qlm, int lane, int show_data, qlm_eye_t *eye_data)
{
	int x_min = 0;
	int x_max = 127;
	int x_step = 1;
	int y_min = -248;
	int y_max = 248;
	int y_step = 8;
	int ber = show_data ? 20 : 16;
	uint64_t resp0;
	uint64_t resp1;

	eye_temp_state_t eye_state;
	memset(&eye_state, 0, sizeof(eye_state));
	//eye_state.node = node;
	eye_state.qlm = qlm;
	eye_state.lane = lane;
	eye_state.ber = ber;

	eye_data->width = 0;
	eye_data->height = 0;

	int y_points = (y_max - y_min) / y_step + 1;
	int x_points = (x_max - x_min) / x_step + 1;
	if (y_points > 64) /* Limit from static struct */
	{
		gser_error("GSERJ%d: Eye too high (%d>%d)\n", qlm, y_points, 64);
		return -1;
	}
	if (x_points > 128) /* Limit from static struct */
	{
		gser_error("GSERJ%d: Eye too long (%d>%d)\n", qlm, x_points, 64);
		return -1;
	}
	x_max = x_min + x_step * (x_points - 1);
	y_max = y_min + y_step * (y_points - 1);

	uint64_t start = gser_clock_get_count(GSER_CLOCK_TIME);
	uint64_t count1 = GSER_CSR_READ(CAVM_GSERJX_LANEX_TXCLK_CTR(qlm, lane));
	gser_wait_usec(500000);
	uint64_t count2 = GSER_CSR_READ(CAVM_GSERJX_LANEX_TXCLK_CTR(qlm, lane));
	uint64_t stop = gser_clock_get_count(GSER_CLOCK_TIME);
	uint64_t hz = (count2 - count1) * gser_clock_get_rate(GSER_CLOCK_TIME) / (stop - start);

	GSER_CSR_INIT(bcfg, CAVM_GSERJX_LANEX_CONTROL_BCFG(qlm, lane));
	switch (bcfg.s.ln_ctrl_tx_width)
	{
		case 0x1: /* 10-bit Reserved */
			hz *= 10;
			break;
		case 0x2: /* 16-bit 40G,10G,DXAUI,RXAUI,XAUI,QSGMII,SGMII */
			hz *= 16;
			break;
		case 0x3: /* 20-bit Reserved */
			hz *= 20;
			break;
		case 0x4: /* 32-bit 25G,50G,100G data rate (default) */
			hz *= 32;
			break;
		case 0x5: /* 40-bit Reserved */
			hz *= 40;
			break;
	}

	printf("GSERJ%d.%d: Measured TX bit rate %llu.%llu MHz\n", qlm, lane,
		hz / 1000000, ((hz + 500) / 1000) % 1000);

	GSER_CSR_INIT(bsts, CAVM_GSERJX_LANEX_STATUS_BSTS(qlm, lane));
	if (bsts.s.ln_stat_los)
	{
		gser_error("GSERJ%d.%d: No signal detected, skipping eye\n", qlm, lane);
		return -1;
	}

	if (show_data)
	{
		printf("# Extensive 1D Rx Eye on GSERJ%d Lane %d\n", qlm, lane);
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
	{
		/* Clean any stale data out of the mailbox */
		mailbox_cleanup(qlm);
		return -1;
	}
	if (mailbox_response(qlm, &resp0, &resp1) != 0)
	{
		gser_error("GSERJ%d: Eye setup 1 failed\n", qlm);
		/* Clean any stale data out of the mailbox */
		mailbox_cleanup(qlm);
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
	{
		/* Clean any stale data out of the mailbox */
		mailbox_cleanup(qlm);
		return -1;
	}
	if (mailbox_response(qlm, &resp0, &resp1) != 0)
	{
		gser_error("GSERJ%d: Eye setup 2 failed\n", qlm);
		/* Clean any stale data out of the mailbox */
		mailbox_cleanup(qlm);
		return -1;
	}

	cmd_arg = lane; /* data 0 */
	cmd_arg = 0 << 16; /* data 2, sweep mode */

	if (mailbox_command(qlm, 0x82, cmd_arg))
	{
		/* Clean any stale data out of the mailbox */
		mailbox_cleanup(qlm);
		return -1;
	}

	for (int x = 0; x < x_points; x++)
	{
		for (int y = 0; y < y_points; y++)
		{
			uint64_t sample = get_eye_sample(&eye_state);
			eye_data->data[y][x] = sample;
			if (show_data)
				printf("%5d %5d %llu\n", y * y_step + y_min, x * x_step + x_min, sample);
		}
	}
	/* Need one more data fetch */
	if (eye_state.last_resp == 0)
		eye_state.last_resp = mailbox_response(qlm, &eye_state.buffer[1], &eye_state.buffer[2]);
	eye_data->width = x_points;
	eye_data->height = y_points;
	return 0;
}
