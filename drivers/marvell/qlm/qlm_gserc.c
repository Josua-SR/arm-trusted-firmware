/*
 * This file was copied from BDK using gsern-bdk-to-atf.sh script.
 * Do not edit it manually!
 */

/***********************license start***********************************
* Copyright (C) 2018-2020 Marvell
* SPDX-License-Identifier: BSD-3-Clause
* https://spdx.org/licenses
***********************license end**************************************/
#include <gser_internal.h>
#include <qlm/qlm.h>
#include <qlm/qlm_gserc.h>

/**
 * Define to enable or disable lanes 2-3. Used to keep differences between
 * GSERR, GSERC, and GSERJ minimal
 */
#define QUAD_LANE 0

/**
 * This define controls whether VCO_DOSC_TEMP_SKEW is set in GSERC, enabling the
 * correction of calibration based on temperature settings. This improves the
 * temperature range the GSERC SERDES operate over.
 */
#define GSER_GSERC_ENABLE_DOSC_TEMP_SKEW 1

/**
 * DOSC skew threshold. The default of 10% is way too large. Encoded at percent
 * times 10. 8 represents 0.8%
 */
#define GSER_GSERC_DOSC_SKEW_THRESHOLD 8 /* 0.8% */

static cavm_gsercx_common_phy_ctrl_bcfg_t qlm_gserc_get_clock_mode(int module);
static cavm_gsercx_lanex_control_bcfg_t qlm_gserc_get_lane_mode(int module, int lane);
static int qlm_gserc_change_lane_rate(int module, int lane);
static int qlm_gserc_change_phy_rate(int module);
static int mailbox_command(int qlm, uint8_t cmd, uint64_t args);
static int mailbox_response(int qlm, uint64_t *arg0, uint64_t *arg1);
static void qlm_gserc_lane_rst(int module, int lane, bool reset);

typedef enum
{
	LANE_STATE_TX_OFF,
	LANE_STATE_TX_ON,
	LANE_STATE_AN,
	LANE_STATE_TRAINING,
	LANE_STATE_CGX,
} lane_state_t;

/* Indexed by QLM number and lane */
static int num_gserc = 0;
static uint64_t prbs_errors[5][2];
static lane_state_t lane_state[5][2];

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
	GSER_CSR_INIT(gsercx_const, CAVM_GSERCX_CONST(module));
	return gsercx_const.s.nr_lanes;
}

/**
 * Map a GSERC module number to the QLM global number tied to it
 *
 * @param module Module to map
 *
 * @return Global QLM number
 */
static int map_module_to_qlm(int module)
{
	int qlm_offset = 0;

	if (gser_is_model(OCTEONTX_LOKI))
		qlm_offset = 2; /* From GSERC_MIN in bdk-qlm-loki.c */
	else
		gser_fatal("GSERC: map_module_to_qlm(): Unrecognized chip\n");

	return module + qlm_offset;
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
qlm_state_lane_t qlm_gserc_get_state(int qlm, int lane)
{
	qlm_state_lane_t state = {.u = GSER_CSR_READ(CAVM_GSERCX_SCRATCHX(qlm, lane))};
	return state;
}

/**
 * Return true if the given lane is CPRI. CGX returns false
 *
 * @param node   Node to query
 * @param qlm	Index into GSER* group
 * @param lane   Lane in GSER
 *
 * @return True if using CPRI
 */
static bool gserc_is_cpri(int qlm, int lane)
{
	qlm_state_lane_t state = {.u = GSER_CSR_READ(CAVM_GSERCX_SCRATCHX(qlm, lane))};
	return state.s.mode == QLM_MODE_CPRI;
}

/**
 * Apply any custom tuning from the device tree
 *
 * @param node
 * @param module
 */
static void apply_tuning(int module, int lane)
{
	if (!gser_is_platform(GSER_PLATFORM_HW))
		return;

	int qlm = map_module_to_qlm(module);
	{
		/* Get settings from the device tree */
		qlm_state_lane_t qlm_state = qlm_gserc_get_state(module, lane);
		const char *smode = qlm_mode_to_cfg_str(qlm_state.s.mode);
		int tx_swing = gser_config_get_int(GSER_CONFIG_QLM_TUNING_TX_MAIN, smode, qlm_state.s.baud_mhz, qlm, lane);
		int tx_cpre = gser_config_get_int(GSER_CONFIG_QLM_TUNING_TX_PRE, smode, qlm_state.s.baud_mhz, qlm, lane);
		int tx_cpost = gser_config_get_int(GSER_CONFIG_QLM_TUNING_TX_POST, smode, qlm_state.s.baud_mhz, qlm, lane);

		/* Get the defaults from the lookup table */
		int default_swing = -1;
		int default_cpre = -1;
		int default_cpost = -1;
		qlm_gserx_default_tuning(qlm_state.s.mode, qlm_state.s.baud_mhz, &default_swing, &default_cpre, &default_cpost);

		/* If the device tree didn't supply a setting, use the default. Note that
		   there may not be a default in the table, so the parameter will continue
		   to be -1 */
		if (tx_swing == -1)
			tx_swing = default_swing;
		if (tx_cpre == -1)
			tx_cpre = default_cpre;
		if (tx_cpost == -1)
			tx_cpost = default_cpost;

		/* If no parameters were changed skip to next lane */
		if ((tx_swing == -1) && (tx_cpre == -1) && (tx_cpost == -1))
			return;

		/* At least one parameter changed, but maybe not all. Get the original
		   values to use as a default for any parameter not filled in yet */
		int orig_swing;
		int orig_cpre;
		int orig_cpost;
		int orig_unused1;
		int orig_unused2;
		qlm_gserc_get_tune_lane_tx(module, lane, &orig_swing, &orig_cpre, &orig_cpost, &orig_unused1, &orig_unused2);

		/* Fill in any parameter not specified yet */
		if (tx_swing == -1)
			tx_swing = orig_swing;
		if (tx_cpre == -1)
			tx_cpre = orig_cpre;
		if (tx_cpost == -1)
			tx_cpost = orig_cpost;

		GSER_TRACE(QLM, "GSERC%d.%d: SWING=%d, PRE=%d, POST=%d\n", module, lane, tx_swing, tx_cpre, tx_cpost);
		qlm_gserc_tune_lane_tx(module, lane, tx_swing, tx_cpre, tx_cpost, -1, -1);
	}
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
int qlm_gserc_set_mode(int qlm, int lane, qlm_modes_t mode, int baud_mhz, qlm_mode_flags_t flags)
{
	qlm_state_lane_t state = qlm_build_state(mode, baud_mhz, flags);
	int num_lanes = get_num_lanes(qlm);

	int start_lane = (lane == -1) ? 0 : lane;
	int end_lane = (lane == -1) ? num_lanes - 1 : lane;

	/* Loop through all lanes */
	bool need_full_reset = false;
	for (int l = start_lane; l <= end_lane; l++)
	{
		need_full_reset |= gserc_is_cpri(qlm, l);
		GSER_CSR_WRITE(CAVM_GSERCX_SCRATCHX(qlm, l), state.u);
	}

	/* Return without changing the SERDES is the microcontroller isn't
	   running. Setup will be completed when it is brought up.*/
	GSER_CSR_INIT(gsercx_init_ctl, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(0));
	if (gsercx_init_ctl.s.cpu_reset || (mode == QLM_MODE_DISABLED))
		return 0;

	for (int l = start_lane; l <= end_lane; l++)
	{
		GSER_CSR_INIT(lane_old, CAVM_GSERCX_LANEX_CONTROL_BCFG(qlm, l));
		need_full_reset |= lane_old.s.ln_an_cfg;
	}

	/* Check if clocking needs to change */
	GSER_CSR_INIT(bcfg_old, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(qlm));
	cavm_gsercx_common_phy_ctrl_bcfg_t bcfg_new = qlm_gserc_get_clock_mode(qlm);
	if (need_full_reset)
		GSER_TRACE(QLM, "GSERC%d: PHY rates require a full reset\n", qlm);
	else if (bcfg_old.u != bcfg_new.u)
		qlm_gserc_change_phy_rate(qlm);
	else
		GSER_TRACE(QLM, "GSERC%d: PHY rates already correct\n", qlm);

	gser_wait_usec(1000);

	/* Check if lane speed needs to change */
	for (int l = start_lane; l <= end_lane; l++)
	{
		GSER_CSR_INIT(lane_old, CAVM_GSERCX_LANEX_CONTROL_BCFG(qlm, l));
		cavm_gsercx_lanex_control_bcfg_t lane_new = qlm_gserc_get_lane_mode(qlm, l);
		bool need_change = (bcfg_old.u != bcfg_new.u) || (lane_old.u != lane_new.u);
		if (need_change)
		{
			need_full_reset |= lane_new.s.ln_an_cfg;
			gser_wait_usec(1000);
#if 0
			if (need_full_reset)
			{
				qlm_gserc_init_reset();
				qlm_gserc_init();
				break; /* Full reset finishes reconfig, so exit loop */
			}
			else
#endif
				qlm_gserc_change_lane_rate(qlm, l);
			gser_wait_usec(1000);
#if defined(PLAT_loki)
			if (mode == QLM_MODE_CPRI) {
				if ((flags >> 3) & 0x1) {
extern void qlm_gserc_rx_dfe_adaptation(int qlm, int lane);
					qlm_gserc_rx_dfe_adaptation(qlm, l);
				}
				if ((flags >> 2) & 0x1) {
extern void qlm_gserc_rx_leq_adaptation(int qlm, int lane,
	int leq_lfg_start, int leq_hfg_sql_start, int leq_mbf_start,
	int leq_mbg_start, int gn_apg_start);
					qlm_gserc_rx_leq_adaptation(qlm, l, 2, 8, 0, 8, 3);
				}
			}
#endif
	                apply_tuning(qlm, l);
			/* Perform lane reset */
			if (mode == QLM_MODE_CPRI) {
				qlm_gserc_lane_rst(qlm, l, 1);
				qlm_gserc_lane_rst(qlm, l, 0);
			}
		}
		else
			GSER_TRACE(QLM, "GSERC%d.%d: Lane mode already correct\n", qlm, l);
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
int qlm_gserc_measure_refclock(int qlm)
{
	if (gser_is_platform(GSER_PLATFORM_ASIM))
	{
		if (gserc_is_cpri(qlm, 0))
			return REF_122MHZ;
		else
			return REF_156MHZ;
	}

	GSER_CSR_INIT(ctr_start, CAVM_GSERCX_REFCLK_CTR(qlm));
	uint64_t start = gser_clock_get_count(GSER_CLOCK_TIME);

	/* Wait for a short time to get a number of counts */
	gser_wait_usec(1000); /* 1ms */

	GSER_CSR_INIT(ctr_stop, CAVM_GSERCX_REFCLK_CTR(qlm));
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
int qlm_gserc_reset(int qlm)
{
	GSER_CSR_MODIFY(c, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(qlm),
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
int qlm_gserc_enable_prbs(int qlm, int prbs, qlm_direction_t dir, int qlm_lane)
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
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL0(qlm, lane), 0);
			/* Seed PRBS */
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_PRBS_CTRL0(qlm, lane), gser_rng_get_random8());
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_PRBS_CTRL1(qlm, lane), gser_rng_get_random8());
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_PRBS_CTRL2(qlm, lane), gser_rng_get_random8());
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_PRBS_CTRL3(qlm, lane), gser_rng_get_random8());
			/* Write the user defined pattern */
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_7_0(qlm, lane), user_defined[0]);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_15_8(qlm, lane), user_defined[0] >> 8);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_23_16(qlm, lane), user_defined[0] >> 16);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_31_24(qlm, lane), user_defined[0] >> 24);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_39_32(qlm, lane), user_defined[0] >> 32);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_47_40(qlm, lane), user_defined[0] >> 40);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_55_48(qlm, lane), user_defined[0] >> 48);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_63_56(qlm, lane), user_defined[0] >> 56);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_71_64(qlm, lane), user_defined[1]);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_79_72(qlm, lane), user_defined[1] >> 8);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_87_80(qlm, lane), user_defined[1] >> 16);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_95_88(qlm, lane), user_defined[1] >> 24);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_103_96(qlm, lane), user_defined[1] >> 32);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_111_104(qlm, lane), user_defined[1] >> 40);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_119_112(qlm, lane), user_defined[1] >> 48);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_127_120(qlm, lane), user_defined[1] >> 56);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_135_128(qlm, lane), user_defined[2]);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_143_136(qlm, lane), user_defined[2] >> 8);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_151_144(qlm, lane), user_defined[2] >> 16);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_159_152(qlm, lane), user_defined[2] >> 24);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_167_160(qlm, lane), user_defined[2] >> 32);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_175_168(qlm, lane), user_defined[2] >> 40);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_183_176(qlm, lane), user_defined[2] >> 48);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_191_184(qlm, lane), user_defined[2] >> 56);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_199_192(qlm, lane), user_defined[3]);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_UDP_SHIFT_AMOUNT(qlm, lane), user_defined_length);
			/* Start transmit */
			GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_BIST_TX_CTRL(qlm, lane),
				c.s.pattern_sel = pattern;
				c.s.en = 1);
			/* Mux data from BIST generator */
			GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_DPL_TXDP_CTRL1(qlm, lane),
				c.s.tx_dmux2_sel = 1);

		}
		if (dir & QLM_DIRECTION_RX)
		{
			/* Write the user defined pattern */
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_7_0(qlm, lane), user_defined[0]);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_15_8(qlm, lane), user_defined[0] >> 8);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_23_16(qlm, lane), user_defined[0] >> 16);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_31_24(qlm, lane), user_defined[0] >> 24);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_39_32(qlm, lane), user_defined[0] >> 32);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_47_40(qlm, lane), user_defined[0] >> 40);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_55_48(qlm, lane), user_defined[0] >> 48);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_63_56(qlm, lane), user_defined[0] >> 56);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_71_64(qlm, lane), user_defined[1]);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_79_72(qlm, lane), user_defined[1] >> 8);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_87_80(qlm, lane), user_defined[1] >> 16);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_95_88(qlm, lane), user_defined[1] >> 24);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_103_96(qlm, lane), user_defined[1] >> 32);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_111_104(qlm, lane), user_defined[1] >> 40);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_119_112(qlm, lane), user_defined[1] >> 48);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_127_120(qlm, lane), user_defined[1] >> 56);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_135_128(qlm, lane), user_defined[2]);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_143_136(qlm, lane), user_defined[2] >> 8);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_151_144(qlm, lane), user_defined[2] >> 16);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_159_152(qlm, lane), user_defined[2] >> 24);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_167_160(qlm, lane), user_defined[2] >> 32);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_175_168(qlm, lane), user_defined[2] >> 40);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_183_176(qlm, lane), user_defined[2] >> 48);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_191_184(qlm, lane), user_defined[2] >> 56);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_199_192(qlm, lane), user_defined[3]);
			GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_RX_UDP_SHIFT_AMOUNT(qlm, lane), user_defined_length);
			/* Require 63 cycles to match before getting lock */
			GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_BIST_RX_LOCK_CTRL0(qlm, lane),
				c.s.num_cycles_7_0 = 0x3f);
			/* Require 63 cycles to not match before losing lock */
			GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_BIST_RX_LOSS_LOCK_CTRL0(qlm, lane),
				c.s.num_cycles_7_0 = 0x3f);
			/* Clear error counter */
			GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_BIST_RX_CTRL(qlm, lane),
				c.s.stop_error_count = 1;
				c.s.clear_ber = 1);
			/* Allow re-lock */
			GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_BIST_RX_LOSS_LOCK_CTRL4(qlm, lane),
				c.s.stop_on_loss_lock = 0);
			/* Start receive */
			GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_BIST_RX_CTRL(qlm, lane),
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
int qlm_gserc_disable_prbs(int qlm, int qlm_lane)
{
	int num_lanes = get_num_lanes(qlm);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		if ((qlm_lane != -1) && (qlm_lane != lane))
			continue;

		/* Stop receive */
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_BIST_RX_CTRL(qlm, lane),
			c.s.stop_error_count = 1;
			c.s.clear_ber = 1;
			c.s.en = 0);
		/* Stop transmit */
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_DPL_TXDP_CTRL1(qlm, lane),
			c.s.tx_dmux2_sel = 0);
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_BIST_TX_CTRL(qlm, lane),
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
uint64_t qlm_gserc_get_prbs_errors(int qlm, int lane, int clear)
{
	GSER_CSR_INIT(status, CAVM_GSERCX_LNX_BIST_RX_STATUS(qlm, lane));
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
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_BIST_RX_CTRL(qlm, lane),
		c.s.stop_error_count = 1);

	/* Read the 24 bit counter */
	GSER_CSR_INIT(status0, CAVM_GSERCX_LNX_BIST_RX_BER_STATUS0(qlm, lane));
	GSER_CSR_INIT(status1, CAVM_GSERCX_LNX_BIST_RX_BER_STATUS1(qlm, lane));
	GSER_CSR_INIT(status2, CAVM_GSERCX_LNX_BIST_RX_BER_STATUS2(qlm, lane));

	/* Clear the counter */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_BIST_RX_CTRL(qlm, lane),
		c.s.clear_ber = 1);
	/* Restart the counter */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_BIST_RX_CTRL(qlm, lane),
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
void qlm_gserc_inject_prbs_error(int qlm, int lane)
{
	GSER_CSR_DEFINE(ctrl0, CAVM_GSERCX_LNX_BIST_TX_BER_CTRL0(0, 0));
	GSER_CSR_DEFINE(ctrl1, CAVM_GSERCX_LNX_BIST_TX_BER_CTRL1(0, 0));
	GSER_CSR_DEFINE(ctrl2, CAVM_GSERCX_LNX_BIST_TX_BER_CTRL2(0, 0));
	GSER_CSR_DEFINE(ctrl3, CAVM_GSERCX_LNX_BIST_TX_BER_CTRL3(0, 0));
	GSER_CSR_DEFINE(ctrl4, CAVM_GSERCX_LNX_BIST_TX_BER_CTRL4(0, 0));
	GSER_CSR_DEFINE(ctrl5, CAVM_GSERCX_LNX_BIST_TX_BER_CTRL5(0, 0));
	GSER_CSR_DEFINE(ctrl6, CAVM_GSERCX_LNX_BIST_TX_BER_CTRL6(0, 0));

	ctrl0.u = 0; /* Control */
	ctrl1.u = 0; /* Timer bits 7:0 */
	ctrl2.u = 0; /* Timer bits 15:8 */
	ctrl3.u = 0; /* Error bits 7:0 */
	ctrl4.u = 0; /* Error bits 15:8 */
	ctrl5.u = 0; /* Error bits 23:16 */
	ctrl6.u = 0; /* Error bits 31:24 */

	/* Clear any previous state */
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL6(qlm, lane), ctrl6.u);
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL5(qlm, lane), ctrl5.u);
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL4(qlm, lane), ctrl4.u);
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL3(qlm, lane), ctrl3.u);
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL2(qlm, lane), ctrl2.u);
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL1(qlm, lane), ctrl1.u);
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL0(qlm, lane), ctrl0.u);

	/* LSB bit is where we inject the error */
	ctrl3.s.bit_error_field_7_0 = 1;

	/* Inject single error without the timer */
	ctrl0.s.mode = 1;

	/* Write ctrl0 last as it starts injection */
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL6(qlm, lane), ctrl6.u);
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL5(qlm, lane), ctrl5.u);
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL4(qlm, lane), ctrl4.u);
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL3(qlm, lane), ctrl3.u);
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL2(qlm, lane), ctrl2.u);
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL1(qlm, lane), ctrl1.u);
	GSER_CSR_WRITE(CAVM_GSERCX_LNX_BIST_TX_BER_CTRL0(qlm, lane), ctrl0.u);
}

/**
 * Reset the GSER lane.
 *
 * @param  qlm	   QLM to use
 * @param  lane	  Which lane
 * @param  reset 1) Set reset 0) Clear reset
 * @return Returns the LMAC first GSER
 */
static void qlm_gserc_lane_rst(int module, int lane, bool reset)
{
	if (reset)
		GSER_TRACE(QLM, "GSERC%d.%d: Setting Lane Reset\n", module, lane);
	else
		GSER_TRACE(QLM, "GSERC%d.%d: Clearing Lane Reset\n", module, lane);

	if (reset)
	{
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
			c.s.ln_ctrl_tx_en = 0);
		gser_wait_usec(1);
	}

	/* Assert or deassert Lane reset */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
		c.s.ln_rst = reset);

	if (reset)
	{
		/* Wait for the PHY firmware to signal that the Lane is in the Reset
		   power state which is signaled by the lane Tx and Rx blocks negating
			the Tx/Rx ready signals.
			Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
				LN_TX_RDY=0 Lane Tx is not ready
				LN_RX_RDY=0 Lane Rx is not ready
				LN_STATE_CHNG_RDY = 0 Lane is transitioning states */
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_TX_RDY, ==, 0, 500))
			gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_tx_rdy]=0 (lane is reset)\n", module, lane);
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_RX_RDY, ==, 0, 500))
			gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_rx_rdy]=0 (lane is reset)\n", module, lane);
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 0, 10000))
		{
			/* This happens fast, so sometimes we miss it */
			//gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=0\n (lane is reset)", module, lane);
		}
	}
	else
	{
		/* Read/Poll for the GSERC to set the Lane State Change Ready flag and
		drive the Lane Tx and Rx ready flags to signal that the lane as
		returned to the ACTIVE state.
		Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_TX_RDY=1 Lane Tx is ready
			LN_RX_RDY=1 Lane Rx is ready
			LN_STATE_CHNG_RDY = 1 Lane is in the “Active” power state */
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_TX_RDY, ==, 1, 5000))
			gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_tx_rdy]=1 (reset done)\n", module, lane);
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_RX_RDY, ==, 1, 5000))
			gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_rx_rdy]=1 (reset done)\n", module, lane);
	}

	/* Wait for the “Lane State Change Ready” to signal that the lane has
	   transitioned to the “Reset” state.
	   Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 1 Lane is in the “Reset” power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 10000)) {
		gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1 (reset done)\n", module, lane);
		return;
	}

	/* set SerDes lane tx_en on */
	if (!reset)
	{
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
					   c.s.ln_ctrl_tx_en = 1);
		gser_wait_usec(1);
	}
}

/**
 * Implementation of NED Loopback with Internal BIST PRBS Generator/Checker.
 * Based on CNF95XX_B0_GSERC_Programming_v1p2.pdf
 *
 * @param node
 * @param module
 * @param lane
 * @param enable
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserc_ned_loopback(int module, int lane, bool enable)
{
	/* Skip running if we aren't changing anything */
	GSER_CSR_INIT(dpl_rxdp_ctrl1, CAVM_GSERCX_LNX_TOP_DPL_RXDP_CTRL1(module, lane));
	if (enable && dpl_rxdp_ctrl1.s.rx_dmux_sel)
		return 0;
	if (!enable && !dpl_rxdp_ctrl1.s.rx_dmux_sel)
		return 0;

	/* The steps to enable GSERC NED loopback with the PHY internal BIST PRBS
		geneator and checker are described below. */
	/* 1. Disable the firmware receive control state machine.
		Write GSERC(0..2)_LN(0..3)_FEATURE_TEST_CFG0
			RX_CTRL_DIS=1 */
	if (enable)
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_TEST_CFG0(module, lane),
			c.s.rx_ctrl_dis = enable);

	/* 2. Bring-up the lane to the active power state, refer to the steps in
		Section 1.4 GSERC Software Initialization. */
	/* Already done */

	/* 3. If performing the NED loopback at the PHY_RATE3 data rate of
		1.25Gbps, 1000BASE-X, or SGMII rate program the following register
		to configure the Rate 3 clocks for the receive gearbox. For all other
		data rates skip this step.
		Write GSERC(0..2)_CM0_TOP_AFE_CMCP_CTRL4
			CMCP_CLKDIV_PLL3DIV=4’h1 //cm0_clk_rate3div set divider to 10
			CMCP_CLKDIV_PLL3=4’h1 //cm0_clk_rate3 set divider to 8 */
	GSER_CSR_INIT(bcfg, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane));
	if (bcfg.s.ln_ctrl_tx_rate == 2)
	{
		GSER_CSR_MODIFY(c, CAVM_GSERCX_CM0_TOP_AFE_CMCP_CTRL4(module),
			c.s.cmcp_clkdiv_pll3div = 1;
			c.s.cmcp_clkdiv_pll3 = 1);
	}

	/* 4. Read GSERC(0..2)_LANE(0..3)_CONTROL_BCFG[LN_CTRL_TX_RATE]
		Use the LN_CTRL_TX_RATE value as an index into Table 6 below
		Write the corresponding lane GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL]
		field with the value shown in Table 6:
		GSERC(0..2)_LANE(0..3)_CONTROL_BCFG[LN_TX_CTRL_RATE] 0 1 2 4 5 6
		GSERC(0..2)_PHY0_TOP_CLOCK_LN0_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL] 2’h3 2’h2 2’h1 2’h3 2’h2 2’h1
		GSERC(0..2)_PHY0_TOP_CLOCK_LN1_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL] 2’h3 2’h2 2’h1 2’h3 2’h2 2’h1
		GSERC(0..2)_PHY0_TOP_CLOCK_LN2_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL] 2’h3 2’h2 2’h1 2’h3 2’h2 2’h1
		GSERC(0..2)_PHY0_TOP_CLOCK_LN3_CM0_CLK_GS_MODE_CTRL0_RSVD[CTRL_SRC_OVR_VAL] 2’h3 2’h2 2’h1 2’h3 2’h2 2’h1
		Table 6 – Clock override values for NED loopback with BIST Enabled */
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
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CM0_CLK_GS_MODE_CTRL0_RSVD(module),
				c.s.ctrl_src_ovr_en = enable;
				c.s.ctrl_src_ovr_val = ctrl_src_ovr_val);
			break;
		case 1:
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CM0_CLK_GS_MODE_CTRL0_RSVD(module),
				c.s.ctrl_src_ovr_en = enable;
				c.s.ctrl_src_ovr_val = ctrl_src_ovr_val);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CM0_CLK_GS_MODE_CTRL0_RSVD(module),
				c.s.ctrl_src_ovr_en = enable;
				c.s.ctrl_src_ovr_val = ctrl_src_ovr_val);
			break;
		case 3:
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CM0_CLK_GS_MODE_CTRL0_RSVD(module),
				c.s.ctrl_src_ovr_en = enable;
				c.s.ctrl_src_ovr_val = ctrl_src_ovr_val);
			break;
#endif
	}

	/* 5. Configure the clocks for the Transmit FIFO and Receiver gearbox clocks.
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXB
			CTRL_SRC_OVR_VAL=2’h2 //Select cmu clock
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXF
			CTRL_SRC_OVR_VAL=2’h2 //Select cmu clock
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXB
			CTRL_SRC_OVR_VAL=2’h2 //Select cmu clock
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXF
			CTRL_SRC_SEL=2’h2 //Select cmu clock */
	cavm_gsercx_phy0_top_clock_ln0_clk_txb_t clk_txb = {.u = 0};
	cavm_gsercx_phy0_top_clock_ln0_clk_txf_t clk_txf = {.u = 0};
	cavm_gsercx_phy0_top_clock_ln0_clk_rxb_t clk_rxb = {.u = 0};
	cavm_gsercx_phy0_top_clock_ln0_clk_rxf_t clk_rxf = {.u = 0};
	clk_txb.s.ctrl_src_ovr_val = enable ? 3 : 0;
	clk_txf.s.ctrl_src_ovr_val = enable ? 3 : 0;
	clk_rxb.s.ctrl_src_ovr_val = enable ? 2 : 0;
	clk_rxf.s.ctrl_src_sel = enable ? 0 : 3;
	switch (lane)
	{
		case 0:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_RXF(module), clk_rxf.u);
			break;
		case 1:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_RXF(module), clk_rxf.u);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_RXF(module), clk_rxf.u);
			break;
		case 3:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_RXF(module), clk_rxf.u);
			break;
#endif
	}

	/* 6. Enable the clocks for the Transmit FIFO and Receiver gearbox clocks.
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXB
			CTRL_SRC_OVR_EN=1
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXF
			CTRL_SRC_OVR_EN=1
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXB
			CTRL_SRC_OVR_EN=1
		Note there is no CTRL_SRC_OVR_EN bit for the CLK_RXF gearbox clock, it
		is controlled only by the GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXF[CTRL_SRC_SEL]
		field in Step 5 above. */
	clk_txb.s.ctrl_src_ovr_en = enable;
	clk_txf.s.ctrl_src_ovr_en = enable;
	clk_rxb.s.ctrl_src_ovr_en = enable;
	switch (lane)
	{
		case 0:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_RXB(module), clk_rxb.u);
			break;
		case 1:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_RXB(module), clk_rxb.u);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_RXB(module), clk_rxb.u);
			break;
		case 3:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_RXB(module), clk_rxb.u);
			break;
#endif
	}

	/* 7. Wait 1 microsecond for the clocks to settle. */
	gser_wait_usec(1);

	/* 8. Enable NED loopback on the selected lane.
		Write GSERC(0..2)_LN(0..3)_TOP_DPL_RXDP_CTRL1
			RX_DMUX_SEL=1 */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_DPL_RXDP_CTRL1(module, lane),
		c.s.rx_dmux_sel = enable);

	/* 9. Enable the clock gaters for NED loopback. One per lane.
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CG_CTRL
			CLK_RX=1 */
	switch (lane)
	{
		case 0:
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CG_CTRL(module),
				c.s.clk_rx = enable);
			break;
		case 1:
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CG_CTRL(module),
				c.s.clk_rx = enable);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CG_CTRL(module),
				c.s.clk_rx = enable);
			break;
		case 3:
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CG_CTRL(module),
				c.s.clk_rx = enable);
			break;
#endif
	}

	/* 10. Wait 1 microsecond for the clocks to settle. */
	gser_wait_usec(1);

	/* 11. Start the CGX or PHY BIST PRBS Generator */
	/* Not needed */

	/* 12. Drive the LN_STAT_RXVALID signal output on the selected lane.
		Write GSERC(0..2)_LN(0..3)_TOP_LN_STAT_CTRL0
			RXVALID=1 */
#if 0
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_LN_STAT_CTRL0(module, lane),
		c.s.rxvalid = 1);
#endif
	/* Skip step; Not needed, may cause issues exiting */

	/* 13. Read/Poll for LN_STAT_RXVALID=1.
		GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STAT_RXVALID=1 Rx CDR is locked
			LN_STAT_LOS Ignore the LN_STAT_LOS flag in NED loopback. */
	if (enable && GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STAT_RXVALID, ==, 1, 10000))
		gser_warn("GSERC%d.%d: Wait for GSERCX_LANEX_STATUS_BSTS[LN_STAT_RXVALID]=1 timeout\n", module, lane);

	/* 14. Complete the bring-up of the Lane in NED loopback mode. */
	/* Not needed */

	/* 15. Reset the Receiver Gearbox FIFO in each lane configured for NED
		loopback. This ensures the Rx Gearbox FIFO read and write pointers are
		properly initialized.
		Step 1. For each GSERC lane configured for NED loopback first check
			that the PHY firmware has released the Rx gearbox FIFO from reset.
			i. Lane 0 Read/poll
				GSERC(0..2)_PHY0_TOP_RESET_CTRL_LN0_RSVD.RXDP_SW_RESET=1’b0
			ii. Lane 1 Read/poll
				GSERC(0..2)_PHY0_TOP_RESET_CTRL_LN1_RSVD.RXDP_SW_RESET=1’b0
			iii. Lane 2 Read/poll
				GSERC(0..2)_PHY0_TOP_RESET_CTRL_LN2_RSVD.RXDP_SW_RESET=1’b0
			iv. Lane 3 Read/poll
				GSERC(0..2)_PHY0_TOP_RESET_CTRL_LN3_RSVD.RXDP_SW_RESET=1’b0 */
	switch (lane)
	{
		case 0:
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_PHY0_TOP_RESET_CTRL_LN0_RSVD(module), GSERCX_PHY0_TOP_RESET_CTRL_LN0_RSVD_RXDP_SW_RESET, ==, 0, 10000))
				gser_error("GSERC%d.%d: Wait for GSERCX_PHY0_TOP_RESET_CTRL_LN0_RSVD[rxdp_sw_reset]=0 timeout\n", module, lane);
			break;
		case 1:
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module), GSERCX_PHY0_TOP_RESET_CTRL_LN1_RSVD_RXDP_SW_RESET, ==, 0, 10000))
				gser_error("GSERC%d.%d: Wait for GSERCX_PHY0_TOP_RESET_CTRL_LN1_RSVD[rxdp_sw_reset]=0 timeout\n", module, lane);
			break;
#if QUAD_LANE
		case 2:
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module), GSERCX_PHY0_TOP_RESET_CTRL_LN2_RSVD_RXDP_SW_RESET, ==, 0, 10000))
				gser_error("GSERC%d.%d: Wait for GSERCX_PHY0_TOP_RESET_CTRL_LN2_RSVD[rxdp_sw_reset]=0 timeout\n", module, lane);
			break;
		case 3:
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module), GSERCX_PHY0_TOP_RESET_CTRL_LN3_RSVD_RXDP_SW_RESET, ==, 0, 10000))
				gser_error("GSERC%d.%d: Wait for GSERCX_PHY0_TOP_RESET_CTRL_LN3_RSVD[rxdp_sw_reset]=0 timeout\n", module, lane);
			break;
#endif
	}

	/*  Step 2. For each GSERC lane configured for NED loopback disable the Rx
			gearbox FIFO, then reset the FIFO, then re-enable the FIFO.
			i. Lane 0:
				Write GSERC(0..2)_LN(0)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=0
				Write GSERC(0..2)_PHY0_TOP_RESET_CTRL_LN0_RSVD.RXDP_SWRESET=1
				Write GSERC(0..2)_PHY0_TOP_RESET_CTRL_LN0_RSVD.RXDP_SWRESET=0
				Write GSERC(0..2)_LN(0)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=1
			ii. Lane 1:
				Write GSERC(0..2)_LN(1)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=0
				Write GSERC(0..2)_PHY0_TOP_RESET_CTRL_LN1_RSVD.RXDP_SWRESET=1
				Write GSERC(0..2)_PHY0_TOP_RESET_CTRL_LN1_RSVD.RXDP_SWRESET=0
				Write GSERC(0..2)_LN(1)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=1
			iii. Lane 2:
				Write GSERC(0..2)_LN(2)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=0
				Write GSERC(0..2)_PHY0_TOP_RESET_CTRL_LN2_RSVD.RXDP_SWRESET=1
				Write GSERC(0..2)_PHY0_TOP_RESET_CTRL_LN2_RSVD.RXDP_SWRESET=0
				Write GSERC(0..2)_LN(2)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=1
			iv. Lane 3:
				Write GSERC(0..2)_LN(3)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=0
				Write GSERC(0..2)_PHY0_TOP_RESET_CTRL_LN3_RSVD.RXDP_SWRESET=1
				Write GSERC(0..2)_PHY0_TOP_RESET_CTRL_LN3_RSVD.RXDP_SWRESET=0
				Write GSERC(0..2)_LN(3)_TOP_DPL_RXDP_CTRL1[RX_FIFO_EN]=1 */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_DPL_RXDP_CTRL1(module, lane),
		c.s.rx_fifo_en = 0);
	switch (lane)
	{
		case 0:
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_RESET_CTRL_LN0_RSVD(module),
				c.s.rxdp_sw_reset = 1);
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_RESET_CTRL_LN0_RSVD(module),
				c.s.rxdp_sw_reset = 0);
			break;
		case 1:
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module),
				c.s.rxdp_sw_reset = 1);
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_RESET_CTRL_LN1_RSVD(module),
				c.s.rxdp_sw_reset = 0);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_RESET_CTRL_LN2_RSVD(module),
				c.s.rxdp_sw_reset = 1);
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_RESET_CTRL_LN2_RSVD(module),
				c.s.rxdp_sw_reset = 0);
			break;
		case 3:
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_RESET_CTRL_LN3_RSVD(module),
				c.s.rxdp_sw_reset = 1);
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_RESET_CTRL_LN3_RSVD(module),
				c.s.rxdp_sw_reset = 0);
			break;
#endif
	}
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_DPL_RXDP_CTRL1(module, lane),
		c.s.rx_fifo_en = 1);

	/* Additional Step. Override tx_en indicator by programming a lane top register
	   LN_CTRL_OVR0, and modify rxpolarity to  Note that all of its fields must be programmed
	   appropriately.
			a. TX_EN = 0x1
			b. DATA_RXPOLARITY = 0x1
			c.  EDGE_RXPOLARITY = 0x0
			d. EYE_RXPOLARITY = 0x0
			e. OVR_EN = 0x1 (should be programmed last or at the same time as
				all other fields) */
	GSER_CSR_INIT(tx_pol, CAVM_GSERCX_LNX_TOP_DPL_TXDP_CTRL1(module, lane));
	GSER_CSR_INIT(rx_pol, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane));
	if (tx_pol.s.txpolarity != rx_pol.s.ln_ctrl_rxpolarity)
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_LN_CTRL_OVR0(module, lane),
			c.s.tx_en = enable;
			c.s.eye_rxpolarity = 0;
			c.s.edge_rxpolarity = 0;
			c.s.data_rxpolarity = tx_pol.s.txpolarity;
			c.s.ovr_en = enable);

	if (!enable)
	{
		/* 2. Disable the firmware receive control state machine. */
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_TEST_CFG0(module, lane),
			c.s.rx_ctrl_dis = enable);
		/* Toggle Reset */
		qlm_gserc_lane_rst(module, lane, 1);
		qlm_gserc_lane_rst(module, lane, 0);
	}
	return 0;
}

/**
 * Implementation of Far-End Analog (FEA) Loopback
 *
 * @param node
 * @param module
 * @param lane
 * @param enable
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserc_fea_loopback(int module, int lane, bool enable)
{
	/* Skip running if we aren't changing anything */
	GSER_CSR_INIT(afe_loopback_ctrl, CAVM_GSERCX_LNX_TOP_AFE_LOOPBACK_CTRL(module, lane));
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

	/* Reset lane if disabling FEA loopback */
	if (!enable)
		qlm_gserc_lane_rst(module, lane, 1);

	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_AFE_LOOPBACK_CTRL(module, lane),
		c.s.loopback_rxclk_en = enable);

	/* 3. Wait until lnX_stat_rxvalid_o PHY top-level interface signal to be set
	   to 1. If desired, a lane top register field LN_STAT_CTRL0.RXVALID can be
	   polled. By this time, CDR is locked and received data are valid. */
	if (enable && GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LNX_TOP_LN_STAT_CTRL0(module, lane), GSERCX_LNX_TOP_LN_STAT_CTRL0_RXVALID, ==, 1, 10000))
		gser_warn("GSERC%d.%d: No receive signal detected\n", module, lane);

	/* 4. Enable FEA loopback by setting a lane top register field
	   AFE_LOOPBACK_CTRL.LOOPBACK_FEA_EN to 0x1. */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_AFE_LOOPBACK_CTRL(module, lane),
		c.s.loopback_fea_en = enable);

	/* 5. Set interface data width to 32 bit as this is required for AFE
	   interface by programming lane top registers:
			a. Program register field AFE_TXDP_CTRL0.TXDP_DATA_WIDTH to 0x1
			b. Program register field AFE_RXDP_CTRL1.RXDP_DATA_WIDTH to 0x1 */
	if (enable)
	{
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_AFE_TXDP_CTRL0(module, lane),
			c.s.txdp_data_width = 1);
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_AFE_RXDP_CTRL1(module, lane),
			c.s.rxdp_data_width = 1);
	}
	else
	{
		/* Program AFE txdp_data_width */
		int txdp_width;
		int rxdp_width;

		/* Bit-stuffed/stripped modes set width to 1 */
		GSER_CSR_INIT(bcfg, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane));
		if (bcfg.s.ln_ctrl_tx_rate == 2)
		{
			txdp_width = 1;
		}
		else
		{
			switch (bcfg.s.ln_ctrl_tx_width)
			{
				case 0x1: /* 10-bit Reserved */
				case 0x2: /* 16-bit 40G,10G,DXAUI,RXAUI,XAUI,QSGMII,SGMII */
					txdp_width = 0;
					break;
				case 0x3: /* 20-bit Reserved */
					txdp_width = 1;
					break;
				case 0x4: /* 32-bit 25G,50G,100G data rate (default) */
				case 0x5: /* 40-bit Reserved */
				default:
					txdp_width = 2;
					break;
			}
		}

		/* Program AFE rxdp_data_width set width to 1 */
		/* Bit-stuffed/stripped modes */
		if (bcfg.s.ln_ctrl_rx_rate == 2)
		{
			rxdp_width = 1;
		}
		else
		{
			switch (bcfg.s.ln_ctrl_rx_width)
			{
				case 0x1: /* 10-bit Reserved */
				case 0x2: /* 16-bit 40G,10G,DXAUI,RXAUI,XAUI,QSGMII,SGMII */
					rxdp_width = 0;
					break;
				case 0x3: /* 20-bit Reserved */
					rxdp_width = 1;
					break;
				case 0x4: /* 32-bit 25G,50G,100G data rate (default) */
				case 0x5: /* 40-bit Reserved */
				default:
					rxdp_width = 2;
					break;
			}
		}
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_AFE_TXDP_CTRL0(module, lane),
			c.s.txdp_data_width = txdp_width);

		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_AFE_RXDP_CTRL1(module, lane),
			c.s.rxdp_data_width = rxdp_width);
	}

	/* 6. Recalibrate TXDP clock using the corresponding mailbox command.
			a. Read top mailbox register CMD_FLAG. It should be 0.
			b. Read top mailbox register RSP_FLAG. If it is 1, write a 1 to
				clear it.
			c. Write the lane number (0 to NUM_LANES) to top mailbox register
				CMD_DATA0.
			d. Write 0x81 to top mailbox register CMD.
			e. Wait for top mailbox register RSP_FLAG to read 1.
			f. Read top mailbox register RSP to get the calibration’s error
				code (0 == no error). Read top mailbox register RSP_DATA0 to
				get the calibration’s result.
			g. Write 1 to RSP_FLAG to clear it. */
	if (enable)
	{
		if (mailbox_command(module, 0x81, lane) != 0)
			return -1;
		uint64_t resp0 = 0;
		uint64_t resp1 = 0;
		if (mailbox_response(module, &resp0, &resp1) != 0)
			gser_warn("GSERC%d.%d: TXDP recalibrate failed\n", module, lane);
	}
	/* 7. Override tx_en indicator by programming a lane top register
	   LN_CTRL_OVR0. Note that all of its fields must be programmed
	   appropriately.
			a. TX_EN = 0x1
			b. DATA_RXPOLARITY = 0x0
			c.  EDGE_RXPOLARITY = 0x0
			d. EYE_RXPOLARITY = 0x0
			e. OVR_EN = 0x1 (should be programmed last or at the same time as
				all other fields) */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_LN_CTRL_OVR0(module, lane),
		c.s.tx_en = enable;
		c.s.eye_rxpolarity = 0;
		c.s.edge_rxpolarity = 0;
		c.s.data_rxpolarity = 0;
		c.s.ovr_en = enable);

	/* Complete lane reset if disabling FEA loopback */
	if (!enable)
		qlm_gserc_lane_rst(module, lane, 0);

	return 0;
}

/**
 * Implementation of Far-End Digital (FED) Loopback
 *
 * @param module   which GSERC instance
 * @param lane	 which lane
 * @param enable
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserc_fed_loopback(int module, int lane, bool enable)
{
	/* Skip running if we aren't changing anything */
	GSER_CSR_INIT(dpl_txdp_ctrl1, CAVM_GSERCX_LNX_TOP_DPL_TXDP_CTRL1(module, lane));
	if (enable && dpl_txdp_ctrl1.s.lb_fed_tx_en)
		return 0;
	if (!enable && !dpl_txdp_ctrl1.s.lb_fed_tx_en)
		return 0;

	/* These steps are from the Salina platform programming guide */
	/* 1. Bring up the lane to the ACTIVE power state as in the normal operating
	   mode (i.e. lnX_tx_rdy_o and lnX_rx_rdy_o asserted). Valid data must be
	   transmitted into RX pins (lnX_rxp/m_i). */
	/* Already been done when the SERDES was configured */

	qlm_gserc_lane_rst(module, lane, 1);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_LN_CTRL_OVR3(module, lane),
		c.s.rx_data_width = enable ? 4 : 0;  // 32-bit
		c.s.tx_data_width = enable ? 4 : 0;  // 32-bit
		c.s.ovr_en = enable);

	/* assert rate chng */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_LN_CTRL_OVR1(module, lane),
		c.s.rate_chng = 1;
		c.s.ovr_en = 1);
 
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 10000))
		{
			gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=0\n (lane is reset)", module, lane);
		}
	/* de-assert rate chng */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_LN_CTRL_OVR1(module, lane),
		c.s.rate_chng = 0;
		c.s.ovr_en = 1);

	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 10000))
		{
			gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1\n (lane is reset)", module, lane);
		}
	/* de-assert ovr_en */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_LN_CTRL_OVR1(module, lane),
		c.s.ovr_en = 0);

	if (enable)
		qlm_gserc_lane_rst(module, lane, 0);

	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_AFE_TXCP_CTRL2(module, lane),
		c.s.txcp_clkdiv = 0x0);

	/* 2. Set the lane top register field AFE_LOOPBACK_CTRL.LOOPBACK_RXCLK_EN.
	   (Note: This is a required step for simulation testing, but is generally
	   not used in silicon testing. This step will use the recovered clock to
	   drive the transmit driver, therefore it will result in much higher jitter
	   content on the TX signal in silicon). */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_AFE_LOOPBACK_CTRL(module, lane),
		c.s.loopback_rxclk_en = enable);

	/* 3. Wait until lnX_stat_rxvalid_o PHY top-level interface signal to be set
	   to 1. If desired, a lane top register field LN_STAT_CTRL0.RXVALID can be
	   polled. By this time, CDR is locked and received data are valid. */
	if (enable && GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LNX_TOP_LN_STAT_CTRL0(module, lane), GSERCX_LNX_TOP_LN_STAT_CTRL0_RXVALID, ==, 1, 10000))
		gser_warn("GSERC%d.%d: [1] No receive signal detected\n", module, lane);

	/* 4. Configure the clocks for the Transmit FIFO and Receiver gearbox clocks.
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXB
			CTRL_SRC_OVR_VAL=2’h0 //Select lnX_clk_rx TX_GBX off
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXF
			CTRL_SRC_OVR_VAL=2’h0 //Select lnX_clk_rx TX_GBX off
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXB
			CTRL_SRC_OVR_VAL=2’h3 //lnX_clk_rx RX_GBX off
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXF
			CTRL_SRC_SEL=2’h3 //lnX_clk_rx RX_GBX off */
	cavm_gsercx_phy0_top_clock_ln0_clk_txb_t clk_txb = {.u = 0};
	cavm_gsercx_phy0_top_clock_ln0_clk_txf_t clk_txf = {.u = 0};
	cavm_gsercx_phy0_top_clock_ln0_clk_rxb_t clk_rxb = {.u = 0};
	cavm_gsercx_phy0_top_clock_ln0_clk_rxf_t clk_rxf = {.u = 0};
	clk_txb.s.ctrl_src_ovr_val = enable ? 0 : 0;
	clk_txf.s.ctrl_src_ovr_val = enable ? 0 : 0;
	clk_rxb.s.ctrl_src_ovr_val = enable ? 3 : 0;
	clk_rxf.s.ctrl_src_sel = enable ? 3 : 3;

	switch (lane)
	{
		case 0:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_RXF(module), clk_rxf.u);
			break;
		case 1:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_RXF(module), clk_rxf.u);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_RXF(module), clk_rxf.u);
			break;
		case 3:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_RXB(module), clk_rxb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_RXF(module), clk_rxf.u);
			break;
#endif
	}

	/* 5. Enable the clocks for the Transmit FIFO and Receiver gearbox clocks.
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXB
			CTRL_SRC_OVR_EN=1
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXF
			CTRL_SRC_OVR_EN=1
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXB
			CTRL_SRC_OVR_EN=1
		Note there is no CTRL_SRC_OVR_EN bit for the CLK_RXF gearbox clock, it
		is controlled only by the GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXF[CTRL_SRC_SEL]
		field in Step 5 above. */
	clk_txb.s.ctrl_src_ovr_en = enable;
	clk_txf.s.ctrl_src_ovr_en = enable;
	clk_rxb.s.ctrl_src_ovr_en = enable;
	switch (lane)
	{
		case 0:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_RXB(module), clk_rxb.u);
			break;
		case 1:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_RXB(module), clk_rxb.u);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_RXB(module), clk_rxb.u);
			break;
		case 3:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_TXF(module), clk_txf.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_RXB(module), clk_rxb.u);
			break;
#endif
	}

	/* 6. Wait 1 usecond for the clocks to settle */
	gser_wait_usec(1);

	/* 7. Enable FED loopback on the selected lane */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_DPL_TXDP_CTRL1(module, lane),
		c.s.dmux_txa_sel_ovr_val = 0x1;  // Rx data for Far-End-Digital FED loopback
		c.s.dmux_txa_sel_ovr_en = enable);   // Enables register control of Tx data path mux in DPL

	/* 8. Recalibrate TXDP clock using the corresponding mailbox command.
			a. Read top mailbox register CMD_FLAG. It should be 0.
			b. Read top mailbox register RSP_FLAG. If it is 1, write a 1 to
				clear it.
			c. Write the lane number (0 to NUM_LANES) to top mailbox register
				CMD_DATA0.
			d. Write 0x81 to top mailbox register CMD.
			e. Wait for top mailbox register RSP_FLAG to read 1.
			f. Read top mailbox register RSP to get the calibration’s error
				code (0 == no error). Read top mailbox register RSP_DATA0 to
				get the calibration’s result.
			g. Write 1 to RSP_FLAG to clear it. */
	if (enable) {
		if (mailbox_command(module, 0x81, lane) != 0)
			return -1;
		uint64_t resp0 = 0;
		uint64_t resp1 = 0;
		if (mailbox_response(module, &resp0, &resp1) != 0)
			gser_warn("GSERC%d.%d: TXDP recalibrate failed\n", module, lane);
	}

	if (enable && GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LNX_TOP_LN_STAT_CTRL0(module, lane), GSERCX_LNX_TOP_LN_STAT_CTRL0_RXVALID, ==, 1, 10000))
		gser_warn("GSERC%d.%d: [2] No receive signal detected\n", module, lane);

	/* 9. Enable the transmitter for the LB_FED path */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_DPL_TXDP_CTRL1(module, lane),
		c.s.lb_fed_tx_en = enable);

	/* Complete lane reset if disabling FED loopback */
	if (!enable) {
		qlm_gserc_lane_rst(module, lane, 0);
	}

	return 0;
}

/**
 * Implementation of Near-End Analog (NEA) Loopback
 *
 * @param module   which GSERC instance
 * @param lane	 which lane
 * @param enable
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserc_nea_loopback(int module, int lane, bool enable)
{
	GSER_CSR_INIT(afe_loopback_ctrl, CAVM_GSERCX_LNX_TOP_AFE_LOOPBACK_CTRL(module, lane));
	if (enable && afe_loopback_ctrl.s.loopback_nea_en)
		return 0;
	if (!enable && !afe_loopback_ctrl.s.loopback_nea_en)
		return 0;
	if (enable)
	{
		/* 1. Assert Reset. */
		qlm_gserc_lane_rst(module, lane, 1);
		/* 2. Disable the firmware receive control state machine. */
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_TEST_CFG0(module, lane),
			c.s.rx_ctrl_dis = enable);
	}

	/* Additional Step. Override tx_en indicator by programming a lane top register
		LN_CTRL_OVR0, and modify rxpolarity to  Note that all of its fields must be programmed
		appropriately.
		a. TX_EN = 0x1
		b. DATA_RXPOLARITY = 0x1
		c.  EDGE_RXPOLARITY = 0x1
		d. EYE_RXPOLARITY = 0x1
		e. OVR_EN = 0x1 (should be programmed last or at the same time as
		all other fields) */
	GSER_CSR_INIT(tx_pol, CAVM_GSERCX_LNX_TOP_DPL_TXDP_CTRL1(module, lane));
	GSER_CSR_INIT(rx_pol, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane));
	if (tx_pol.s.txpolarity != rx_pol.s.ln_ctrl_rxpolarity)
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_LN_CTRL_OVR0(module, lane),
			c.s.tx_en = enable;
			c.s.eye_rxpolarity = tx_pol.s.txpolarity;
			c.s.edge_rxpolarity = tx_pol.s.txpolarity;
			c.s.data_rxpolarity = tx_pol.s.txpolarity;
			c.s.ovr_en = enable);

	/* 3. De-Assert Reset */
	if (enable)
		qlm_gserc_lane_rst(module, lane, 0);

	/* 4. Configure the clocks for the Transmit FIFO and Receiver gearbox clocks.
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXB
			CTRL_SRC_OVR_VAL=2’h2 //Select cmu clock
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXF
			CTRL_SRC_OVR_VAL=2’h2 //Select cmu clock
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXB
			CTRL_SRC_OVR_VAL=2’h2 //Select cmu clock
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXF
			CTRL_SRC_SEL=2’h2 //Select cmu clock */
	cavm_gsercx_phy0_top_clock_ln0_clk_txb_t clk_txb = {.u = 0};
	cavm_gsercx_phy0_top_clock_ln0_clk_txf_t clk_txf = {.u = 0};
	//cavm_gsercx_phy0_top_clock_ln0_clk_rxb_t clk_rxb = {.u = 0};
	//cavm_gsercx_phy0_top_clock_ln0_clk_rxf_t clk_rxf = {.u = 0};
	clk_txb.s.ctrl_src_ovr_val = enable ? 3 : 0;
	GSER_CSR_INIT(ln_bcfg, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane));
	if (ln_bcfg.s.ln_ctrl_tx_width == 5)
		clk_txf.s.ctrl_src_ovr_val = enable ? 2 : 0;
	else
		clk_txf.s.ctrl_src_ovr_val = enable ? 3 : 0;
	//clk_rxb.s.ctrl_src_ovr_val = enable ? 1 : 0;
	//clk_rxf.s.ctrl_src_sel = enable ? 3 : 0;
	switch (lane)
	{
		case 0:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_TXF(module), clk_txf.u);
			//GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_RXB(module), clk_rxb.u);
			//GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_RXF(module), clk_rxf.u);
			break;
		case 1:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_TXF(module), clk_txf.u);
			//GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_RXB(module), clk_rxb.u);
			//GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_RXF(module), clk_rxf.u);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_TXF(module), clk_txf.u);
			//GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_RXB(module), clk_rxb.u);
			//GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_RXF(module), clk_rxf.u);
			break;
		case 3:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_TXF(module), clk_txf.u);
			//GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_RXB(module), clk_rxb.u);
			//GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_RXF(module), clk_rxf.u);
			break;
#endif
	}

	/* 5. Enable the clocks for the Transmit FIFO and Receiver gearbox clocks.
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXB
			CTRL_SRC_OVR_EN=1
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_TXF
			CTRL_SRC_OVR_EN=1
		Write GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXB
			CTRL_SRC_OVR_EN=1
		Note there is no CTRL_SRC_OVR_EN bit for the CLK_RXF gearbox clock, it
		is controlled only by the GSERC(0..2)_PHY0_TOP_CLOCK_LN[0,1,2,3]_CLK_RXF[CTRL_SRC_SEL]
		field in Step 5 above. */
	clk_txb.s.ctrl_src_ovr_en = enable;
	clk_txf.s.ctrl_src_ovr_en = enable;
	//clk_rxb.s.ctrl_src_ovr_en = enable;
	switch (lane)
	{
		case 0:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_TXF(module), clk_txf.u);
			//GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CLK_RXB(module), clk_rxb.u);
			break;
		case 1:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_TXF(module), clk_txf.u);
			//GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CLK_RXB(module), clk_rxb.u);
			break;
#if QUAD_LANE
		case 2:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_TXF(module), clk_txf.u);
			//GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CLK_RXB(module), clk_rxb.u);
			break;
		case 3:
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_TXB(module), clk_txb.u);
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_TXF(module), clk_txf.u);
			//GSER_CSR_WRITE(CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CLK_RXB(module), clk_rxb.u);
			break;
#endif
	}


	/* 6. Wait 1 usecond for the clocks to settle */
	gser_wait_usec(1);

	/* 7. Recalibrate TXDP clock using the corresponding mailbox command.
			a. Read top mailbox register CMD_FLAG. It should be 0.
			b. Read top mailbox register RSP_FLAG. If it is 1, write a 1 to
				clear it.
			c. Write the lane number (0 to NUM_LANES) to top mailbox register
				CMD_DATA0.
			d. Write 0x81 to top mailbox register CMD.
			e. Wait for top mailbox register RSP_FLAG to read 1.
			f. Read top mailbox register RSP to get the calibration’s error
				code (0 == no error). Read top mailbox register RSP_DATA0 to
				get the calibration’s result.
			g. Write 1 to RSP_FLAG to clear it. */
	if (enable) {
		if (mailbox_command(module, 0x81, lane) != 0)
			return -1;
		uint64_t resp0 = 0;
		uint64_t resp1 = 0;
		if (mailbox_response(module, &resp0, &resp1) != 0)
			gser_warn("GSERC%d.%d: TXDP recalibrate failed\n", module, lane);
	}

	/* 8. Enable NEA loopback on the selected lane. */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_AFE_LOOPBACK_CTRL(module, lane),
		c.s.loopback_nea_en = enable);

	/* 9. Enable the lane recieve clock. */
	switch(lane)
	{
		case 0 :
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CG_CTRL(module),
				c.s.clk_rx = enable);
			break;
		case 1 :
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CG_CTRL(module),
				c.s.clk_rx = enable);
			break;
#if QUAD_LANE
		case 2 :
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CG_CTRL(module),
				c.s.clk_rx = enable);
			break;
		case 3 :
			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CG_CTRL(module),
				c.s.clk_rx = enable);
			break;
#endif
	}

	/* 10. Bring-up the CGX MAC and start the MAC transmitter. */
	// TODO

	/* 11. Wait 1 usecond for looped Tx data to fully propegate to the receive data path. */
	gser_wait_usec(1);

	/* 13. Drive the RXVALID on the selected lane. */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_LN_STAT_CTRL0(module, lane),
		c.s.rxvalid = enable);

	if (!enable)
	{
		/* 2. Disable the firmware receive control state machine. */
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_TEST_CFG0(module, lane),
			c.s.rx_ctrl_dis = enable); // maybe use enable?
		qlm_gserc_lane_rst(module, lane, 1);
		/* 3. De-Assert Reset */
		qlm_gserc_lane_rst(module, lane, 0);
	}

	/* 14. Poll for Rx CDR locked.
	   Ignore the LN_STAT_LOS flag in NEA loopback. */
	if (enable && GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STAT_RXVALID, ==, 1, 10000))
		gser_warn("GSERC%d.%d: Wait for GSERCX_LANEX_STATUS_BSTS[LN_STAT_RXVALID]=1 timeout\n", module, lane);

	/* 15. Start the CGX MAC Receiver, CGX should now be receiving all transmit data to the GSER* lane in NEA loopback. */
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
int qlm_gserc_enable_loop(int qlm, qlm_loop_t loop)
{
	int use_fea = (loop == QLM_LOOP_FEA);
	int use_ned = (loop == QLM_LOOP_NED);
	int use_fed = (loop == QLM_LOOP_FED);
	int use_nea = (loop == QLM_LOOP_NEA);

	int num_lanes = get_num_lanes(qlm);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		qlm_gserc_fea_loopback(qlm, lane, use_fea);
		qlm_gserc_ned_loopback(qlm, lane, use_ned);
		qlm_gserc_fed_loopback(qlm, lane, use_fed);
		qlm_gserc_nea_loopback(qlm, lane, use_nea);
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
int qlm_gserc_tune_lane_tx(int qlm, int lane, int tx_swing, int tx_cpre, int tx_cpost, int tx_unused1, int tx_unused2)
{
	if ((tx_swing < 0) || (tx_swing > 7))
	{
		gser_error("GSERC%d.%d: Invalid tuning, TX Swing(%d) must be 0 to 7\n",
			qlm, lane, tx_swing);
		return -1;
	}
	if ((tx_cpre < 0) || (tx_cpre > 15))
	{
		gser_error("GSERC%d.%d: Invalid tuning, TX PRE(%d) must be 0 to 15\n",
			qlm, lane, tx_cpre);
		return -1;
	}
	if ((tx_cpost < 0) || (tx_cpost > 31))
	{
		gser_error("GSERC%d.%d: Invalid tuning, TX POST(%d) must be 0 to 31\n",
			qlm, lane, tx_cpost);
		return -1;
	}

	GSER_CSR_INIT(ctrl0, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_CTRL0(qlm, lane));
	GSER_CSR_INIT(status0, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_STATUS0(qlm, lane));
	if (ctrl0.s.req || status0.s.ack)
	{
		gser_error("GSERC%d.%d: TXEQ coefficient request in progress\n", qlm, lane);
		return -1;
	}

	/* Load the new value */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_CTRL1(qlm, lane),
		c.s.txeq_c1 = tx_cpost);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_CTRL3(qlm, lane),
		c.s.txeq_cm1 = tx_cpre);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_CTRL5(qlm, lane),
		c.s.drv_swing = tx_swing);

	/* Only request Tx coefficient update if CPU is out of reset */
	GSER_CSR_INIT(gsercx_init_ctl, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(qlm));
	if (!gsercx_init_ctl.s.cpu_reset)
	{
		/* Request an update */
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_CTRL0(qlm, lane),
			c.s.req = 1);
		/* Wait for ACK */
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_STATUS0(qlm, lane), GSERCX_LNX_DRV_REFCLK_TXEQ_STATUS0_ACK, ==, 1, 10000))
		{
			gser_error("GSERC%d.%d: TXEQ coefficient request timeout\n", qlm, lane);
			return -1;
		}
		/* Clear update request */
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_CTRL0(qlm, lane),
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
int qlm_gserc_get_tune_lane_tx(int qlm, int lane, int *tx_swing, int *tx_cpre, int *tx_cpost, int *tx_unused1, int *tx_unused2)
{
	GSER_CSR_INIT(ctrl1, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_CTRL1(qlm, lane));
	GSER_CSR_INIT(ctrl3, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_CTRL3(qlm, lane));
	GSER_CSR_INIT(ctrl5, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_CTRL5(qlm, lane));
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
/*
 * Modified in qlm-gserc.patch applied by gser-update script in SDK
 */
//void qlm_gserc_display_settings(int qlm, int qlm_lane, bool show_tx, bool show_rx)
//{
void qlm_gserc_display_settings(int qlm, int qlm_lane, bool show_tx, bool show_rx, char *buf, int size)
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

	printf("N0.GSERC%d Lane %d:\n", qlm, qlm_lane);
	if (show_tx)
	{
		printf("  CMU Status:\n");
		GSER_CSR_INIT(cm0_top_phy_if_status, CAVM_GSERCX_CM0_TOP_PHY_IF_STATUS(qlm));
		GSER_CSR_INIT(bcfg, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(qlm));
		printf("	CMU OK=%d, PHY Rate1=0x%x, Rate2=0x%x\n",
			cm0_top_phy_if_status.s.cmu_ok, bcfg.s.phy_ctrl_rate1, bcfg.s.phy_ctrl_rate2);
		GSER_CSR_INIT(status1, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_STATUS1(qlm, qlm_lane));
		GSER_CSR_INIT(status2, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_STATUS2(qlm, qlm_lane));
		GSER_CSR_INIT(ctrl5, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_CTRL5(qlm, qlm_lane));
		GSER_CSR_INIT(status3, CAVM_GSERCX_LNX_DRV_REFCLK_TXEQ_STATUS3(qlm, qlm_lane));
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
		int sql = -1;
		int mbf = -1;
		int mbg = -1;
		switch (qlm_lane)
		{
			case 0:
				att = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_PLE_ATT_O_READ_VAL_2_0_RSVD(qlm));
				apg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_GN_APG_O_READ_VAL_2_0_RSVD(qlm));
				lfg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_LFG_O_READ_VAL_4_0_RSVD(qlm));
				hfg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_HFG_O_READ_VAL_4_0_RSVD(qlm));
				sql = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_SQL_O_READ_VAL_2_0_RSVD(qlm));
				mbf = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_MBF_O_READ_VAL_3_0_RSVD(qlm));
				mbg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN0_RXLEQ_EQ_MBG_O_READ_VAL_3_0_RSVD(qlm));
				break;
			case 1:
				att = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_PLE_ATT_O_READ_VAL_2_0_RSVD(qlm));
				apg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_GN_APG_O_READ_VAL_2_0_RSVD(qlm));
				lfg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_LFG_O_READ_VAL_4_0_RSVD(qlm));
				hfg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_HFG_O_READ_VAL_4_0_RSVD(qlm));
				sql = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_SQL_O_READ_VAL_2_0_RSVD(qlm));
				mbf = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_MBF_O_READ_VAL_3_0_RSVD(qlm));
				mbg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN1_RXLEQ_EQ_MBG_O_READ_VAL_3_0_RSVD(qlm));
				break;
#if QUAD_LANE
			case 2:
				att = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_PLE_ATT_O_READ_VAL_2_0_RSVD(qlm));
				apg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_GN_APG_O_READ_VAL_2_0_RSVD(qlm));
				lfg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_LFG_O_READ_VAL_4_0_RSVD(qlm));
				hfg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_HFG_O_READ_VAL_4_0_RSVD(qlm));
				sql = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_SQL_O_READ_VAL_2_0_RSVD(qlm));
				mbf = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_MBF_O_READ_VAL_3_0_RSVD(qlm));
				mbg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN2_RXLEQ_EQ_MBG_O_READ_VAL_3_0_RSVD(qlm));
				break;
			case 3:
				att = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_PLE_ATT_O_READ_VAL_2_0_RSVD(qlm));
				apg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_GN_APG_O_READ_VAL_2_0_RSVD(qlm));
				lfg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_LFG_O_READ_VAL_4_0_RSVD(qlm));
				hfg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_HFG_O_READ_VAL_4_0_RSVD(qlm));
				sql = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_SQL_O_READ_VAL_2_0_RSVD(qlm));
				mbf = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_MBF_O_READ_VAL_3_0_RSVD(qlm));
				mbg = GSER_CSR_READ(CAVM_GSERCX_PHY0_AFE_OVR_AFE_LN3_RXLEQ_EQ_MBG_O_READ_VAL_3_0_RSVD(qlm));
				break;
#endif
		}
		/* These values are encoded as gray codes https://en.wikipedia.org/wiki/Gray_code */
		att = gray2binary(att);
		apg = gray2binary(apg);
		lfg = gray2binary(lfg);
		hfg = gray2binary(hfg) + 7 - gray2binary(sql);
		mbf = gray2binary(mbf);
		mbg = gray2binary(mbg);

		printf("  Receiver Status:\n");
		printf("	ATT=%d, APG=%d, LFG=%d, HFG=%d, MBF=%d, MBG=%d\n",
			att, apg, lfg, hfg, mbf, mbg);

		/* Read DFE taps */
		GSER_CSR_INIT(tap1_even, CAVM_GSERCX_LNX_DFE_REFCLK_TAP_VAL_STATUS0(qlm, qlm_lane));
		GSER_CSR_INIT(tap1_odd, CAVM_GSERCX_LNX_DFE_REFCLK_TAP_VAL_STATUS1(qlm, qlm_lane));
		GSER_CSR_INIT(tap2, CAVM_GSERCX_LNX_DFE_REFCLK_TAP_VAL_STATUS2(qlm, qlm_lane));
		GSER_CSR_INIT(tap3, CAVM_GSERCX_LNX_DFE_REFCLK_TAP_VAL_STATUS3(qlm, qlm_lane));
		GSER_CSR_INIT(tap4, CAVM_GSERCX_LNX_DFE_REFCLK_TAP_VAL_STATUS4(qlm, qlm_lane));
		GSER_CSR_INIT(tap5, CAVM_GSERCX_LNX_DFE_REFCLK_TAP_VAL_STATUS5(qlm, qlm_lane));
		GSER_CSR_INIT(tap6, CAVM_GSERCX_LNX_DFE_REFCLK_TAP_VAL_STATUS6(qlm, qlm_lane));
		GSER_CSR_INIT(tap7, CAVM_GSERCX_LNX_DFE_REFCLK_TAP_VAL_STATUS7(qlm, qlm_lane));
		GSER_CSR_INIT(tap8, CAVM_GSERCX_LNX_DFE_REFCLK_TAP_VAL_STATUS8(qlm, qlm_lane));
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
	GSER_CSR_INIT(flag, CAVM_GSERCX_PHY0_MB_CMD_FLAG(qlm));
	if ((flag.s.data) && (cmd != 0))
	{
		gser_error("GSERC%d: Mailbox already has a command\n", qlm);
		return -1;
	}

	GSER_CSR_INIT(rsp_flag, CAVM_GSERCX_PHY0_MB_RSP_FLAG(qlm));
	if ((rsp_flag.s.data) && (cmd != 0))
	{
		gser_error("GSERC%d: Mailbox already has a response\n", qlm);
		return -1;
	}

	GSER_CSR_WRITE(CAVM_GSERCX_PHY0_MB_CMD_DATA0(qlm), gser_extract(args, 0, 8));
	GSER_CSR_WRITE(CAVM_GSERCX_PHY0_MB_CMD_DATA1(qlm), gser_extract(args, 8, 8));
	GSER_CSR_WRITE(CAVM_GSERCX_PHY0_MB_CMD_DATA2(qlm), gser_extract(args, 16, 8));
	GSER_CSR_WRITE(CAVM_GSERCX_PHY0_MB_CMD_DATA3(qlm), gser_extract(args, 24, 8));
	GSER_CSR_WRITE(CAVM_GSERCX_PHY0_MB_CMD_DATA4(qlm), gser_extract(args, 32, 8));
	GSER_CSR_WRITE(CAVM_GSERCX_PHY0_MB_CMD_DATA5(qlm), gser_extract(args, 40, 8));
	GSER_CSR_WRITE(CAVM_GSERCX_PHY0_MB_CMD_DATA6(qlm), gser_extract(args, 48, 8));
	GSER_CSR_WRITE(CAVM_GSERCX_PHY0_MB_CMD_DATA7(qlm), gser_extract(args, 56, 8));
	GSER_CSR_WRITE(CAVM_GSERCX_PHY0_MB_CMD(qlm), cmd);

	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_PHY0_MB_CMD_FLAG(qlm), GSERCX_PHY0_MB_CMD_FLAG_DATA, ==, 0, 1000000))
	{
		gser_error("GSERC%d: Mailbox stuck on command\n", qlm);
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
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_PHY0_MB_RSP_FLAG(qlm), GSERCX_PHY0_MB_RSP_FLAG_DATA, ==, 1, 500000))
	{
		gser_error("GSERC%d: Mailbox has no response\n", qlm);
		return -1;
	}

	/* Read the response */
	GSER_CSR_INIT(rsp, CAVM_GSERCX_PHY0_MB_RSP(qlm));
	GSER_CSR_INIT(data0, CAVM_GSERCX_PHY0_MB_RSP_DATA0(qlm));
	GSER_CSR_INIT(data1, CAVM_GSERCX_PHY0_MB_RSP_DATA1(qlm));
	GSER_CSR_INIT(data2, CAVM_GSERCX_PHY0_MB_RSP_DATA2(qlm));
	GSER_CSR_INIT(data3, CAVM_GSERCX_PHY0_MB_RSP_DATA3(qlm));
	GSER_CSR_INIT(data4, CAVM_GSERCX_PHY0_MB_RSP_DATA4(qlm));
	GSER_CSR_INIT(data5, CAVM_GSERCX_PHY0_MB_RSP_DATA5(qlm));
	GSER_CSR_INIT(data6, CAVM_GSERCX_PHY0_MB_RSP_DATA6(qlm));
	GSER_CSR_INIT(data7, CAVM_GSERCX_PHY0_MB_RSP_DATA7(qlm));
	GSER_CSR_INIT(data8, CAVM_GSERCX_PHY0_MB_RSP_DATA8(qlm));
	GSER_CSR_INIT(data9, CAVM_GSERCX_PHY0_MB_RSP_DATA9(qlm));
	GSER_CSR_INIT(data10, CAVM_GSERCX_PHY0_MB_RSP_DATA10(qlm));
	GSER_CSR_INIT(data11, CAVM_GSERCX_PHY0_MB_RSP_DATA11(qlm));
	GSER_CSR_INIT(data12, CAVM_GSERCX_PHY0_MB_RSP_DATA12(qlm));
	GSER_CSR_INIT(data13, CAVM_GSERCX_PHY0_MB_RSP_DATA13(qlm));
	GSER_CSR_INIT(data14, CAVM_GSERCX_PHY0_MB_RSP_DATA14(qlm));
	GSER_CSR_INIT(data15, CAVM_GSERCX_PHY0_MB_RSP_DATA15(qlm));

	/* Tell firmware we read the response */
	GSER_CSR_WRITE(CAVM_GSERCX_PHY0_MB_RSP_FLAG(qlm), 1);

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
			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_PHY0_MB_RSP_FLAG(qlm), GSERCX_PHY0_MB_RSP_FLAG_DATA, ==, 1, 500000))
				break;
			/* Tell firmware we read the response */
			GSER_CSR_WRITE(CAVM_GSERCX_PHY0_MB_RSP_FLAG(qlm), 1);
			gser_wait_usec(1000);
		}
		/* Issue a clear command on first loop */
		if (count == 0)
			mailbox_command(qlm, 0x00, 0);
	}
	return 0;
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
int qlm__gserc_rx_equalization(int qlm, int qlm_lane)
{
	/* Update Forcing RX FSM Out of Lock:
		By setting these fields, the FW will re-calibrate and RX CTRL state
		machine will go back to the top (idle loop), LOS will drop,
		calibration and adaptation will happen again and CDR will re-lock:
		- lane_feature.spare_cfg6[1:1] to use initial adaptation sequence after
			CDR has relocked.
		- lane_feature.spare_cfg6[2:2] to use eie adaptation sequence after CDR
			has relocked.
		FW will clear these bits when it reads either of them as 1 while in LOCKED
		state. Allows for forced RX re-calibration/re-adaptation and re-locking
		of RX CDR without interfering with transmitter operation. */
	//const int ADAPTION_TYPE_INITIAL = 2;
	const int ADAPTION_TYPE_RUNNING = 4;

	if (gser_is_platform(GSER_PLATFORM_ASIM))
		return 0;

	int result = 0;
	int num_lanes = get_num_lanes(qlm);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		if ((qlm_lane != -1) && (qlm_lane != lane))
			continue;

		GSER_CSR_INIT(rxdp_ctrl1, CAVM_GSERCX_LNX_TOP_DPL_RXDP_CTRL1(qlm, lane));
		GSER_CSR_INIT(afe_loopback_ctrl, CAVM_GSERCX_LNX_TOP_AFE_LOOPBACK_CTRL(qlm, lane));
		if (rxdp_ctrl1.s.rx_dmux_sel ||
			afe_loopback_ctrl.s.loopback_nea_en)
		{
			GSER_TRACE(QLM, "N0.GSERC%d.%d: NED or NEA loopback, skipping adaption\n", qlm, lane);
			continue;
		}

		GSER_CSR_INIT(bsts, CAVM_GSERCX_LANEX_STATUS_BSTS(qlm, lane));
		if (bsts.s.ln_stat_los)
		{
			GSER_TRACE(QLM, "N0.GSERC%d.%d: Loss of signal\n", qlm, lane);
			result = -1;
			continue;
		}
		if (!bsts.s.ln_stat_rxvalid)
		{
			GSER_TRACE(QLM, "N0.GSERC%d.%d: RX invalid, CDR not locked\n", qlm, lane);
			result = -1;
			continue;
		}

		GSER_TRACE(QLM, "N0.GSERC%d.%d: RX signal, CDR locked, running adaption\n", qlm, lane);
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_SPARE_CFG6_RSVD(qlm, lane),
			c.u |= ADAPTION_TYPE_RUNNING);
	}
	return result;
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
int qlm_gserc_eye_capture(int qlm, int lane, int show_data, qlm_eye_t *eye_data)
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
//	eye_state.node = node;
	eye_state.qlm = qlm;
	eye_state.lane = lane;
	eye_state.ber = ber;

	eye_data->width = 0;
	eye_data->height = 0;

	int y_points = (y_max - y_min) / y_step + 1;
	int x_points = (x_max - x_min) / x_step + 1;
	if (y_points > 64) /* Limit from static struct */
	{
		gser_error("GSERC%d: Eye too high (%d>%d)\n", qlm, y_points, 64);
		return -1;
	}
	if (x_points > 128) /* Limit from static struct */
	{
		gser_error("GSERC%d: Eye too long (%d>%d)\n", qlm, x_points, 64);
		return -1;
	}
	x_max = x_min + x_step * (x_points - 1);
	y_max = y_min + y_step * (y_points - 1);

	uint64_t start = gser_clock_get_count(GSER_CLOCK_TIME);
	uint64_t count1 = GSER_CSR_READ(CAVM_GSERCX_LANEX_TXCLK_CTR(qlm, lane));
	gser_wait_usec(500000);
	uint64_t count2 = GSER_CSR_READ(CAVM_GSERCX_LANEX_TXCLK_CTR(qlm, lane));
	uint64_t stop = gser_clock_get_count(GSER_CLOCK_TIME);
	uint64_t hz = (count2 - count1) * gser_clock_get_rate(GSER_CLOCK_TIME) / (stop - start);

	GSER_CSR_INIT(bcfg, CAVM_GSERCX_LANEX_CONTROL_BCFG(qlm, lane));
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

	printf("GSERC%d.%d: Measured TX bit rate %llu.%llu MHz\n", qlm, lane,
		hz / 1000000, ((hz + 500) / 1000) % 1000);

	GSER_CSR_INIT(bsts, CAVM_GSERCX_LANEX_STATUS_BSTS(qlm, lane));
	if (bsts.s.ln_stat_los)
	{
		gser_error("GSERC%d.%d: No signal detected, skipping eye\n", qlm, lane);
		return -1;
	}

	if (show_data)
	{
		printf("# Extensive 1D Rx Eye on GSERC%d Lane %d\n", qlm, lane);
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
		gser_error("GSERC%d: Eye setup 1 failed\n", qlm);
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
		gser_error("GSERC%d: Eye setup 2 failed\n", qlm);
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

/**
 * Manually turn on or off the SERDES transmitter
 *
 * @param node	  Node to use in numa setup
 * @param qlm	   QLM to use
 * @param lane	  Which lane
 * @param enable_tx True to enable transmitter, false to disable
 */
int qlm_gserc_tx_control(int qlm, int lane, int enable_tx)
{
	int en = (enable_tx) ? 1 : 0;
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(qlm, lane),
		c.s.ln_ctrl_tx_en = en);
	lane_state[qlm][lane] = en ? LANE_STATE_TX_ON : LANE_STATE_TX_OFF;
	return 0;
}

/**
 * Poll serdes for errors removed by gser-update script (qlm.sed)
 *
 * @param node   Node to poll
 * @param module  SERDES group to poll
 */
//static void qlm_gserc_poll_error(int module)
//{
//	/* Return if no PHY error is detected */
//	GSER_CSR_INIT(phy_status_bsts, CAVM_GSERCX_COMMON_PHY_STATUS_BSTS(module));
//	if (!phy_status_bsts.s.phy_error)
//		return;
//	gser_warn("GSERC%d: COMMON_PHY_STATUS_BSTS[PHY_ERROR] is set\n", module);
//
//	/* PHY Errors */
//	GSER_CSR_INIT(top_err_ctrl0, CAVM_GSERCX_PHY0_TOP_ERR_CTRL0(module));
//	if (top_err_ctrl0.s.err_o)
//	{
//		GSER_CSR_INIT(top_err_ctrl1, CAVM_GSERCX_PHY0_TOP_ERR_CTRL1(module));
//		GSER_CSR_INIT(top_err_ctrl2, CAVM_GSERCX_PHY0_TOP_ERR_CTRL2(module));
//		gser_warn("GSERC%d: PHY0_TOP_ERR 0x%02x%02x\n",
//			module, top_err_ctrl2.s.err_code_15_8, top_err_ctrl1.s.err_code_7_0);
//		return;
//	}
//
//	/* CM0 Errors */
//	GSER_CSR_INIT(cm0_top_phy_if_status, CAVM_GSERCX_CM0_TOP_PHY_IF_STATUS(module));
//	if (!cm0_top_phy_if_status.s.cmu_ok)
//	{
//		GSER_CSR_INIT(top_err_ctrl1, CAVM_GSERCX_CM0_TOP_ERR_CTRL1(module));
//		GSER_CSR_INIT(top_err_ctrl2, CAVM_GSERCX_CM0_TOP_ERR_CTRL2(module));
//		GSER_CSR_INIT(top_err_ctrl3, CAVM_GSERCX_CM0_TOP_ERR_CTRL3(module));
//		gser_warn("GSERC%d: CM0_TOP_ERR 0x%02x%02x CRITICAL=%d\n",
//			module, top_err_ctrl2.s.err_code_15_8, top_err_ctrl1.s.err_code_7_0,
//			top_err_ctrl3.s.critical_err);
//		return;
//	}
//
//	/* Lane Errors */
//	int num_lanes = get_num_lanes(module);
//	for (int lane = 0; lane < num_lanes; lane++)
//	{
//		GSER_CSR_INIT(ctrl, CAVM_GSERCX_LNX_TOP_PHY_IF_CTRL_RSVD(module, lane));
//		if (!ctrl.s.ln_rx_rdy || !ctrl.s.ln_tx_rdy)
//		{
//			GSER_CSR_INIT(top_err_ctrl1, CAVM_GSERCX_LNX_TOP_ERR_CTRL1(module, lane));
//			GSER_CSR_INIT(top_err_ctrl2, CAVM_GSERCX_LNX_TOP_ERR_CTRL2(module, lane));
//			GSER_CSR_INIT(top_err_ctrl3, CAVM_GSERCX_LNX_TOP_ERR_CTRL3(module, lane));
//			gser_warn("GSERC%d.%d: LN_RX_RSY=%d, LN_TX_RDY=%d, Error 0x%02x%02x CRITICAL_ERR_RX=%d CRITICAL_ERR_TX=%d\n",
//				module, lane, ctrl.s.ln_rx_rdy, ctrl.s.ln_tx_rdy,
//				top_err_ctrl2.s.err_code_15_8, top_err_ctrl1.s.err_code_7_0,
//				top_err_ctrl3.s.critical_err_rx, top_err_ctrl3.s.critical_err_tx);
//		}
//	}
//}

/**
 * Updated IKVCO Settings Based on Speed
 *
 * @param module
 *
 */
static void gserc_qlm_set_ikvco_override(int module)
{
	/* Find all needed clocks */
	bool need_25g = false;
	bool need_10g = false;
	bool need_cpri_9g = false;
	bool need_cpri_6g = false;
	int cmpll_ikvco = 10;

	int num_lanes = get_num_lanes(module);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		qlm_state_lane_t state = qlm_gserc_get_state(module, lane);
		need_25g |= (state.s.baud_mhz == 25781);
		need_10g |= (state.s.baud_mhz == 10312);
		need_cpri_9g |= (state.s.baud_mhz == 9830) || (state.s.baud_mhz == 4915) || (state.s.baud_mhz == 2458);
		need_cpri_6g |= (state.s.baud_mhz == 6144) || (state.s.baud_mhz == 3072);
	}

	if(need_25g || need_10g || need_cpri_9g || need_cpri_6g)
	{
		if (need_25g || need_10g)
			cmpll_ikvco = 8;
		else if (need_cpri_9g || need_cpri_6g)
			cmpll_ikvco = 12;
		printf("GSERC%d Setting IKVCO Override to %d\n", module, cmpll_ikvco);
		//GSER_TRACE(QLM, "GSERC%d Setting IKVCO Override to %d\n", module, cmpll_ikvco);
	}

	GSER_CSR_MODIFY(c, CAVM_GSERCX_CM0_PLL_AFE_INT_CTRL3_RSVD(module),
			c.s.cmpll_ikvco = cmpll_ikvco);
}

/**
 * Determine the GSERC clocking needed based on the current config. No updates
 * are done, just determines what is needed. This will be input to the code that
 * actually makes changes.
 *
 * @param node
 * @param module
 *
 * @return SERDES clocking mode
 */
static cavm_gsercx_common_phy_ctrl_bcfg_t qlm_gserc_get_clock_mode(int module)
{
	/* Find all needed clocks */
	bool need_25g = false;
	bool need_20g = false;
	bool need_10g = false;
	bool need_6g = false;
	bool need_5g = false;
	bool need_1g = false;
	bool need_cpri_9g = false;
	bool need_cpri_6g = false;

	int num_lanes = get_num_lanes(module);
	for (int lane = 0; lane < num_lanes; lane++)
	{
		qlm_state_lane_t state = qlm_gserc_get_state(module, lane);
		need_25g |= (state.s.baud_mhz == 25781);
		need_20g |= (state.s.baud_mhz == 20625);
		need_10g |= (state.s.baud_mhz == 10312);
		need_6g |= (state.s.baud_mhz == 6250) || (state.s.baud_mhz == 3125);
		need_5g |= (state.s.baud_mhz == 5000) || (state.s.baud_mhz == 2500);
		need_1g |= (state.s.baud_mhz == 1250);
		need_cpri_9g |= (state.s.baud_mhz == 9830) || (state.s.baud_mhz == 4915) || (state.s.baud_mhz == 2458);
		need_cpri_6g |= (state.s.baud_mhz == 6144) || (state.s.baud_mhz == 3072);
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
	else if (need_cpri_9g || need_cpri_6g)
	{
		phy_ctrl_rate1 = 0x0d;
		phy_ctrl_rate2 = need_cpri_6g ? 0x11 : 0x12;
	}
	else /* No special speeds are needed, setup for 25G, 10G, 1G for flexibility */
	{
		phy_ctrl_rate1 = 0x03; /* 25G */
		phy_ctrl_rate2 = 0x23; /* 10G */
		/* rate 3 is auto enabled at 1G */
	}

	GSER_TRACE(QLM, "GSERC%d: phy_ctrl_rate1=0x%02x, phy_ctrl_rate2=0x%02x\n",
		module, phy_ctrl_rate1, phy_ctrl_rate2);

	/* Refclk routing, from F95N_detailed_refclk_distribution_diagram_r3.pdf
		1) GSERC_REF_CLK2, 156.25MHz for etherent, comes into GSERR0 refclk
			bumps on GSERR0. This is then forwarded out ref_a_r_o to GSERC0.
			GSERC0 receives etherent clk on ref_a_r_i and forwards it out
			ref_a_r_o. GSERC1-3 do the same, then GSERC4 receives it on
			ref_a_r_i. REFCLK_PAD_ENA must be 1 on GSERR0.
		2) GSERC_REF_CLK3, 156.25MHz for SyncEtherent, comes into GSERC4 refclk
			bumps on GSERC4. This is then forwarded out ref_b_r_o to GSERC3.
			GSERC3 receives SyncEtherent clk on ref_b_r_i and forwards it out
			ref_b_r_o. GSERC2-1 do the same, then GSERC0 receives it on
			ref_b_r_i and forwards it out ref_b_r_o to GSERR0. REFCLK_PAD_ENA
			must be 1 on GSERC3.
		3) GSERC_REF_CLK4, 122.88MHz for CPRI, comes into GSERC0 refclk
			bumps on GSERC0. This is then forwarded out ref_a_l_o to GSERC1.
			GSERC1 receives CPRI clk on ref_a_l_i and forwards it out
			ref_a_l_o. GSERC1-3 do the same, then GSERC4 receives it on
			ref_a_l_i. REFCLK_PAD_ENA must be 1 on GSERC0. */
	bool use_gserc_ref_clk2 = !need_cpri_9g && !need_cpri_6g;
	bool use_gserc_ref_clk3 = false;
	bool use_gserc_ref_clk4 = need_cpri_9g || need_cpri_6g;

	GSER_CSR_INIT(bcfg, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(module));
	if (use_gserc_ref_clk2)
	{
		bcfg.s.refclk_input_sel = 2; /* ref_a_r_i */
		bcfg.s.phy_ctrl_refclk = 0x0e; /* Assumes 156.25MHz */
		bcfg.s.refclk_pad_ena = (module == 0) ? 1 : 0; /* Pad on GSERC0 */
	}
	else if (use_gserc_ref_clk3)
	{
		/* module 3 uses pad, others use ref_b_r_i */
		bcfg.s.refclk_input_sel = (module == 4) ? 0 : 6;
		bcfg.s.phy_ctrl_refclk = 0x0e; /* Assumes 156.25MHz */
		bcfg.s.refclk_pad_ena = (module == 4) ? 1 : 0;; /* Pad on GSERC4 */
	}
	else if (use_gserc_ref_clk4)
	{
		/* module 0 uses pad, others use ref_a_l_i */
		bcfg.s.refclk_input_sel = (module == 0) ? 0 : 1;
		bcfg.s.phy_ctrl_refclk = 0x14; /* Assumes 122.88MHz */
		bcfg.s.refclk_pad_ena = (module == 0) ? 1 : 0;; /* Pad on GSERC0 */
	}
	else
		gser_error("GSERC%d: No refclk selected\n", module);

	bcfg.s.phy_ctrl_rate1 = phy_ctrl_rate1;
	bcfg.s.phy_ctrl_rate2 = phy_ctrl_rate2;

	return bcfg;
}

/**
 * Determine the lane settings to support the current protocol. No actual GSERC
 * changes are made.
 *
 * @param node
 * @param module
 * @param lane
 *
 * @return Lane settings that need to be applied
 */
static cavm_gsercx_lanex_control_bcfg_t qlm_gserc_get_lane_mode(int module, int lane)
{
	qlm_state_lane_t state = qlm_gserc_get_state(module, lane);
	int ln_ctrl_tx_rate = 0;
	int ln_ctrl_rx_rate = 0;
	int tx_clk_mux_sel = 0;
	int ln_ctrl_tx_width = 0x4;
	int ln_ctrl_rx_width = 0x5;

	switch (state.s.baud_mhz)
	{
		case 1250:
			ln_ctrl_tx_rate = 2; /* PHY rate 3 */
			ln_ctrl_rx_rate = 2; /* PHY rate 3 */
			tx_clk_mux_sel = 0x6; /* PHY rate 3 */
			ln_ctrl_tx_width = 0x2; /* 16 bit */
			ln_ctrl_rx_width = 0x3; /* 20 bit */
			break;
		case 2458: /* CPRI */
		case 6144: /* CPRI */
		case 9830: /* CPRI */
			ln_ctrl_tx_rate = 0x1; /* PHY rate 2 */
			ln_ctrl_rx_rate = 0x1; /* PHY rate 2 */
			tx_clk_mux_sel = 0x0; /* PHY rate 1 */
			ln_ctrl_tx_width = 0x3; /* 20 bit */
			ln_ctrl_rx_width = 0x3; /* 20 bit */
			break;
		case 3072: /* CPRI */
		case 4915: /* CPRI */
			ln_ctrl_tx_rate = 0x1; /* PHY rate 2 */
			ln_ctrl_rx_rate = 0x5; /* PHY rate 2 divide by 2 */
			tx_clk_mux_sel = 0x0; /* PHY rate 1 */
			ln_ctrl_tx_width = 0x3; /* 20 bit */
			ln_ctrl_rx_width = 0x3; /* 20 bit */
			break;
		case 2500:
		case 3125:
			ln_ctrl_tx_rate = 0x5; /* PHY rate 2 divide by 2 */
			ln_ctrl_rx_rate = 0x5; /* PHY rate 2 divide by 2 */
			tx_clk_mux_sel = 0x4; /* PHY rate 2 divide by 2 */
			ln_ctrl_tx_width = 0x2; /* 16 bit */
			ln_ctrl_rx_width = 0x3; /* 20 bit */
			break;
		case 5000:
		case 6250:
			ln_ctrl_tx_rate = 0x1; /* PHY rate 2 */
			ln_ctrl_rx_rate = 0x1; /* PHY rate 2 */
			tx_clk_mux_sel = 0x1; /* PHY rate 2 */
			ln_ctrl_tx_width = 0x2; /* 16 bit */
			ln_ctrl_rx_width = 0x3; /* 20 bit */
			break;
		case 10312:
		{
			GSER_CSR_INIT(phy_bcfg, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(module));
			if (phy_bcfg.s.phy_ctrl_rate1 == 0x23) /* 10G */
			{
				ln_ctrl_tx_rate = 0x0; /* PHY rate 1 */
				ln_ctrl_rx_rate = 0x0; /* PHY rate 1 */
				tx_clk_mux_sel = 0x0; /* PHY rate 1 */
			}
			else
			{
				ln_ctrl_tx_rate = 0x1; /* PHY rate 2 */
				ln_ctrl_rx_rate = 0x1; /* PHY rate 2 */
				tx_clk_mux_sel = 0x1; /* PHY rate 2 */
			}
			ln_ctrl_tx_width = 0x2; /* 16 bit */
			ln_ctrl_rx_width = 0x3; /* 20 bit */
			break;
		}
		case 20625:
		case 25781:
		default:
			ln_ctrl_tx_rate = 0x0; /* PHY rate 1 */
			ln_ctrl_rx_rate = 0x0; /* PHY rate 1 */
			tx_clk_mux_sel = 0x0; /* PHY rate 1 */
			ln_ctrl_tx_width = 0x4; /* 32 bit */
			ln_ctrl_rx_width = 0x5; /* 40 bit */
			break;
	}

	bool is_dual;
	bool is_quad;
	int ln_an_cfg = 0;
	switch (state.s.mode)
	{
		case QLM_MODE_10G_KR:
		case QLM_MODE_40G_CR4:
		case QLM_MODE_40G_KR4:
		case QLM_MODE_25G_CR:
		case QLM_MODE_25G_KR:
		case QLM_MODE_50G_CR2:
		case QLM_MODE_50G_KR2:
		case QLM_MODE_100G_CR4:
		case QLM_MODE_100G_KR4:
			/* 3 is not a valid AN value. We use it to signal that the entire
			   GSERC complex needs refresh to apply AN changes */
			ln_an_cfg = 3;
			is_dual = false;
			is_quad = false;
			break;
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

	GSER_TRACE(QLM, "GSERC%d.%d: %s %dMHz, tx rate=%d, rx rate=%d, tx_width=%d, rx_width=%d, clk_mux=%d, quad=%d, dual=%d\n",
		module, lane, qlm_mode_to_cfg_str(state.s.mode), state.s.baud_mhz,
		ln_ctrl_tx_rate, ln_ctrl_rx_rate, ln_ctrl_tx_width, ln_ctrl_rx_width,
		tx_clk_mux_sel, is_quad, is_dual);

	GSER_CSR_INIT(bcfg, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane));
	bcfg.s.ln_ctrl_tx_rate = ln_ctrl_tx_rate;
	bcfg.s.ln_ctrl_rx_rate = ln_ctrl_rx_rate;
	bcfg.s.ln_ctrl_tx_width = ln_ctrl_tx_width;
	bcfg.s.ln_ctrl_rx_width = ln_ctrl_rx_width;
	bcfg.s.tx_clk_mux_sel = tx_clk_mux_sel;
	bcfg.s.cgx_quad = is_quad;
	bcfg.s.cgx_dual = is_dual;
	bcfg.s.ln_link_stat = 0;
	bcfg.s.ln_an_cfg = ln_an_cfg;
	bcfg.s.rx_bitstrip_en = (state.s.baud_mhz == 2458);
	/* Program RX polarity. The TX polarity is programemd in qlm_gserc_init() */
	bcfg.s.ln_ctrl_rxpolarity = gser_config_get_int(GSER_CONFIG_QLM_LANE_RX_POLARITY,
		map_module_to_qlm(module), lane);

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
static int qlm_gserc_change_lane_rate(int module, int lane)
{
	GSER_TRACE(QLM, "GSERC%d.%d: Changing lane mode\n", module, lane);
	/* Lane Rate Change Sequence
		1. If the Lane is inactive, proceed to Step 5, else if the Lane is
		already up and connected bring down the CGX LMAC {SPU or GMP}
		connection to the lane and ensure CGX has asserted the CGX-to-GSERC
		reset signal to place the GSERC Lane Tx/Rx async FIFOs in reset. This
		is required to ensure the "credit" flow control signaling between the
		GSERC lane async Tx/Rx FIFOs and the CGX LMAC cleanly enter the reset
		state.
		Refer to CGX(0..2)_SPU(0..3)_CONTROL1[RESET] //LMAC is SPU
		Refer to CGX(0..2)_GMP_PCS_MR(0..3)_CONTROL[RESET] //LMAC is GMP */
	// FIXME: CGX stop

	/* 2. Disable the PHY transmitter driver and put the GSERC lane in reset by
		writing
		Write GSERC(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_CTRL_TX_EN=0 Disable the transmitter
		Wait at least 100 nanoseconds to allow the Transmitter to turn off
		Write GSERC(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_RST = 1 Put the PHY lane in Reset */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
		c.s.ln_ctrl_tx_en = 0);
	gser_wait_usec(1);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
		c.s.ln_rst = 1);

	/* 3. Wait for the PHY firmware to signal that the Lane is in the Reset
		power state which is signaled by the lane Tx and Rx blocks negating
		the Tx/Rx ready signals.
		Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_TX_RDY=0 Lane Tx is not ready
			LN_RX_RDY=0 Lane Rx is not ready
			LN_STATE_CHNG_RDY = 0 Lane is transitioning states */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_TX_RDY, ==, 0, 500))
		gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_tx_rdy]=0 (lane is reset)\n", module, lane);
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_RX_RDY, ==, 0, 500))
		gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_rx_rdy]=0 (lane is reset)\n", module, lane);
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 0, 10000))
	{
		/* This happens fast, so sometimes we miss it */
		//gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=0\n (lane is reset)", module, lane);
	}

	/* 4. Wait for the “Lane State Change Ready” to signal that the lane has
		transitioned to the “Reset” state.
		Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 1 Lane is in the “Reset” power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 10000))
		gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1 (lane is reset)\n", module, lane);

	/* 5. Program the new Lane rate, data width, and transmitter clock parameters
		using the values shown in Table 2
		Write GSERC(0..2)_LANE(0..3)_CONTROL_BCFG
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
	cavm_gsercx_lanex_control_bcfg_t bcfg = qlm_gserc_get_lane_mode(module, lane);
	GSER_CSR_WRITE(CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane), bcfg.u);
	gser_wait_usec(1);
	qlm_state_lane_t state = qlm_gserc_get_state(module, lane);
	bool ena_8b10b = (state.s.baud_mhz <= 6250) && (state.s.baud_mhz != 2458) && (state.s.baud_mhz != 3072);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_ADAPT_CFG0(module, lane),
		c.s.ena_8b10b = ena_8b10b);

	/* 6. Trigger the lane rate change by writing the LN_RATE_CHNG field to
		load the *new* lane rate settings from Step 5 above.
		Write GSERC(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_RATE_CHNG=1
		Wait at least 200 nanoseconds for the PHY firmware to receive the lane
		rate change request before moving to the next step. Note the
		LN_STATE_CHNG_RDY will toggle from 1->0->1 when the LN_RATE_CHNG bit is
		set above. */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
		c.s.ln_rate_chng = 1);
	gser_wait_usec(1);

	/* 7. Wait for the “Lane State Change Ready” status bit to deassert
		indicating the lane is transitioning to the “Rate Change” state.
		Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 0 Lane is transitioning power states */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 0, 10000))
		gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=0 (rate change)\n", module, lane);

	/* 8. Program the lane x2/x4 bonding (used with RXAUI,XAUI,DXAUI) fields if
		applicable to the new lane rate or link topology
		Write GSERC(0..2)_LANE(0..3)_CONTROL_BCFG
			CGX_QUAD = 1 If the lane is part of a x4 link (XAUI,DXAUI); else 0
			CGX_DUAL = 1 If lanes 0&1 or 2&3 are part of a x2 link (RXAUI); else 0
			CFG_CGX = 0 Keep the Tx/Rx FIFOs to CGX in reset */
	/* Done at step 6 */

	/* 9. Wait for the PHY “Lane State Change Ready” to signal that the lane has
		transitioned to the “Rate Change” state.
		Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 1 Lane is in the “Rate Change” power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 10000))
		gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1 (rate change)\n", module, lane);

	/* 10. Deassert the LN_STATE_CHNG signal to complete the lane
		reconfiguration
		Write GSERC(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_RATE_CHNG=0
		Wait 200 nanoseconds for the PHY firmware to receive the lane rate
		change deassertion request. */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
		c.s.ln_rate_chng = 0);
	gser_wait_usec(1);

	/* 11. Wait for the “Lane State Change Ready” status bit to deassert
		indicating the lane is transitioning states.
		Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 0 Lane is transitioning states */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 0, 10000))
		gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=0 (change done)\n", module, lane);

	/* 12. Wait for the PHY “Lane State Change Ready” to signal that the lane
		has transitioned back to the “Reset” state.
		Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 1 Lane is in the “Reset” power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 10000))
		gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1 (change done)\n", module, lane);

	/* 13. Release the lane from reset.
		Write GSERC(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_RST=0 Release the lane reset */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
		c.s.ln_rst = 0);

	/* 14. Read/Poll for the GSERC to set the Lane State Change Ready flag and
		drive the Lane Tx and Rx ready flags to signal that the lane as
		returned to the ACTIVE state.
		Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_TX_RDY=1 Lane Tx is ready
			LN_RX_RDY=1 Lane Rx is ready
			LN_STATE_CHNG_RDY = 1 Lane is in the “Active” power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_TX_RDY, ==, 1, 5000))
		gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_tx_rdy]=1 (reset done)\n", module, lane);
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_RX_RDY, ==, 1, 5000))
		gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_rx_rdy]=1 (reset done)\n", module, lane);
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 10000))
		gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1 (reset done)\n", module, lane);

	/* 15. Enable the Tx/Rx FIFOs between CGX and GSERC
		Write GSERC(0..2)_LANE(0..3)_CONTROL_BCFG
			CFG_CGX = 1 Enable Tx and Rx Async FIFOs to CGX */
	bool is_cpri = gserc_is_cpri(module, lane);
	GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
		c.s.cfg_cpri = is_cpri;
		c.s.cfg_cgx = !is_cpri);

	/* 16. CGX should start transmitting now, refer to the CGX initialization
		and link bring-up section in the CNF95XX B0 HRM. */
	// FIXME: Enable CGX transmit

	/* 17. Poll the lane receiver looking for the Lane Status Rx Valid flag to
		be set indicating the Lane Rx CDR is locked and the lane is receiving
		data
		Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
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
		Write GSERC(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_CTRL_TX_EN = 1 Enable the Lane SerDes transmitter driver */
	// FIXME: Should be done in CGX

	/* 20. Poll the lane receiver looking for a valid receive signal and the
		indication that the PHY Lane Rx CDR is locked. Note that if the
		Ethernet link-partner is not up and driving valid Ethernet data into
		the GSERC PHY serial receiver input then LN_STAT_LOS=1 and
		LN_STAT_RXVALID=0 until the link partner starts to drive valid data. So
		do not watch-dog time-out this polling routine. Once LN_STAT_LOS
		transitions 1->0 then LN_STAT_RXVALID should transition 0->1 within
		500 microseconds so a watch-dog timer can be used to guardband the
		Rx CDR lock process.
		Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STAT_LOS=0 Rx signal carrier detected (Loss-of-signal is negated)
			LN_STAT_RXVALID=1 The receive CDR has locked and Rx data is valid */
	// FIXME: Should be done in CGX

	/* 21. Bring up the CGX receive side. Refer to the CGX initialization and
	   link bring-up steps in the CNF95XX B0 HRM. */
	// FIXME: Should be done in CGX
	GSER_TRACE(QLM, "GSERC%d.%d: Lane mode change complete\n", module, lane);
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
static int qlm_gserc_change_phy_rate(int module)
{
	gser_wait_usec(10000); /* No idea why this is needed. 1ms isn't long enough */
	GSER_TRACE(QLM, "GSERC%d: Changing PHY rates\n", module);
	int num_lanes = get_num_lanes(module);
	/* PHY Rate Change Sequence
		The PHY Rate 1 and Rate 2 values can be changed without driving the
		GSERC(0..2)_COMMON_PHY_CTRL_BCFG[POR] power on reset. This is necessary
		to prevent disruption of the reference clock distribution network
		between GSERC blocks, refer to Figure 3. The steps required to program
		new PHY Rate 1 and Rate 2 values is described below. */

	/* 1. If the Lanes are all inactive, proceed to Step 5, else bring-down all
		the lanes if the lanes are already up and connected to the CGX LMAC
		{SPU or GMP} and ensure CGX has asserted the CGX-to-GSERC reset signal
		to place the GSERC Lane Tx/Rx async FIFOs in reset. This is required
		to ensure the "credit" flow control signaling between the GSERC lane
		async Tx/Rx FIFOs and the CGX LMAC cleanly enter the reset state.
		Refer to CGX(0..2)_SPU(0..3)_CONTROL1[RESET] //LMAC is SPU
		Refer to CGX(0..2)_GMP_PCS_MR(0..3)_CONTROL[RESET] //LMAC is GMP */
	// FIXME: CGX stop

	/* 2. Disable the PHY transmitter driver and put the GSERC lane in reset by
		writing
		Write GSERC(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_CTRL_TX_EN=0 Disable the transmitter
		Wait at lease 100 nanoseconds to allow the Transmitter to turn off
		Write GSERC(0..2)_LANE(0..3)_CONTROL_BCFG
			LN_RST = 1 Put the PHY lane in Reset */
	for (int lane = 0; lane < num_lanes; lane++)
	{
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
			c.s.ln_ctrl_tx_en = 0);
		gser_wait_usec(1);
		GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
			c.s.ln_rst = 1);
	}

	/* 3. Wait for the PHY firmware to signal that the Lane is in the Reset
		power state which is signaled by the lane Tx and Rx blocks negating
		the Tx/Rx ready signals.
		Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_TX_RDY=0 Lane Tx is not ready
			LN_RX_RDY=0 Lane Rx is not ready
			LN_STATE_CHNG_RDY = 0 Lane is transitioning states */
	for (int lane = 0; lane < num_lanes; lane++)
	{
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_TX_RDY, ==, 0, 500))
			gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_tx_rdy]=0 (change rate)\n", module, lane);
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_RX_RDY, ==, 0, 500))
			gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_rx_rdy]=0 (change rate)\n", module, lane);
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 0, 10000))
			GSER_TRACE(QLM, "GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=0 (change rate)\n", module, lane);
	}

	/* 4. Wait for the “Lane State Change Ready” to signal that the lane has
		transitioned to the “Reset” state.
		Read/Poll GSERC(0..2)_LANE(0..3)_STATUS_BSTS
			LN_STATE_CHNG_RDY = 1 Lane is in the “Reset” power state */
	for (int lane = 0; lane < num_lanes; lane++)
	{
		if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 10000))
			gser_error("GSERC%d.%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1 (change rate)\n", module, lane);
	}

	/* 5. Write the new PHY Rate 1 and Rate 2 settings from Table 2
		Write GSERC(0..2)_COMMON_PHY_CTRL_BCFG
			PHY_CTRL_RATE1 New PHY Rate 1 value from Table 2
			PHY_CTRL_RATE1 New PHY Rate 2 value from Table 2
		Wait 1 microsecond for the PHY firmware to pickup the new rates */
	cavm_gsercx_common_phy_ctrl_bcfg_t bcfg = qlm_gserc_get_clock_mode(module);
	GSER_CSR_WRITE(CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(module), bcfg.u);
	gser_wait_usec(1);

	/* 6. Reset the Clock Management Unit
		Write GSERC(0..2)_COMMON_PHY_CTRL_BCFG
			CM0_RST=1 Reset CMU */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(module),
		c.s.cm0_rst = 1);

	/* 7. Read/Poll for the CM0 State Change Ready to deassert
		Read/Poll GSERC(0..2)_COMMON_PHY_STATUS_BSTS
			CM0_STATE_CHNG_RDY=0 //CM0 is transitioning power states */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_COMMON_PHY_STATUS_BSTS(module), GSERCX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 0, 500))
		GSER_TRACE(QLM, "GSERC%d: Timeout waiting for GSERCX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=0 (change rate rst)\n", module);

	/* 8. Read/Poll for the CM0 State Change Ready to reassert
		Read/Poll GSERC(0..2)_COMMON_PHY_STATUS_BSTS
			CM0_STATE_CHNG_RDY=1 //CM0 is in the Reset power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_COMMON_PHY_STATUS_BSTS(module), GSERCX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 1, 10000))
		gser_error("GSERC%d: Timeout waiting for GSERCX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=1 (change rate rst)\n", module);

	/* 9. Release the Clock Management Unit reset
		Write GSERC(0..2)_COMMON_PHY_CTRL_BCFG
			CM0_RST=0 Release CMU reset */
	GSER_CSR_MODIFY(c, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(module),
		c.s.cm0_rst = 0);

	/* 10. Read/Poll for the CM0 State Change Ready to deassert
	   Read/Poll GSERC(0..2)_COMMON_PHY_STATUS_BSTS
	   CM0_STATE_CHNG_RDY=0 //CM0 is transitioning power states */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_COMMON_PHY_STATUS_BSTS(module), GSERCX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 0, 500))
		GSER_TRACE(QLM, "GSERC%d: Timeout waiting for GSERCX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=0 (change rate)\n", module);

	/* 11. Read/Poll for the CM0 State Change Ready to reassert
	   Read/Poll GSERC(0..2)_COMMON_PHY_STATUS_BSTS
		CM0_STATE_CHNG_RDY=1 //CM0 has completed power state transition */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_COMMON_PHY_STATUS_BSTS(module), GSERCX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 1, 50000))
		gser_error("GSERC%d: Timeout waiting for GSERCX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=1 (change rate)\n", module);

	/* 12. Read/Poll for the CM0 OK flag set
		Read/Poll GSERC(0..2)_COMMON_PHY_STATUS_BSTS
			CM0_OK=1 //CM0 status is Active power state */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_COMMON_PHY_STATUS_BSTS(module), GSERCX_COMMON_PHY_STATUS_BSTS_CM0_OK, ==, 1, 50000))
		gser_error("GSERC%d: Timeout waiting for GSERCX_COMMON_PHY_STATUS_BSTS[cm0_ok]=1 (change rate)\n", module);

	/* Update IKVCO */
	gserc_qlm_set_ikvco_override(module);

	/* Program the new Lane Rates to the new PHY rates, refer to the steps in
	   Section 1.4 Lane Rate Change. */
	GSER_TRACE(QLM, "GSERC%d: PHY rate change complete\n", module);
	return 0;
}

//write_dmem is used only in initialize function
//which is removed by gser-update script (qlm.sed)
//static void write_dmem(int module, int offset, uint8_t data)
//{
//	int bit = 24 - (offset & 3) * 8;
//	int word = offset >> 2;
//	uint32_t tmp = GSER_CSR_READ(CAVM_GSERCX_DMEMX(module, word));
//	tmp = gser_insert(tmp, data, bit, 8);
//	GSER_CSR_WRITE(CAVM_GSERCX_DMEMX(module, word), tmp);
//}

//static void qlm_gserc_config_an(int module, int lane)
//{
//	qlm_state_lane_t qlm_state = qlm_gserc_get_state(module, lane);
//	bool use_training;
//	int an_lane_mode;
//	int qlm = map_module_to_qlm(module);
//
//	/* Determine which CGX maps to this GSERC module and lane */
//	bool is_split = false;
//	bool is_upper = false;
//	int cgx = qlm_to_network(qlm, &is_split, &is_upper);
//	/* Lane order can change mapping for CGX lmac to lane */
//	int lane_order = gser_config_get_int(GSER_CONFIG_NETWORK_LANE_ORDER, cgx);
//	/* Find which lmac corresponds to this lane */
//	int lmac;
//	for (lmac = 0; lmac < 4; lmac++)
//	{
//		int lmac_lane = (lane_order >> (lmac * 4)) & 0xf;
//		if (lane == lmac_lane)
//			break;
//	}
//	gser_netphy_flags_t net_flags = gser_config_get_int(GSER_CONFIG_NETWORK_FLAGS, cgx, lmac);
//	GSER_TRACE(QLM, "GSERC%d.%d: Maps to CGX%d, LMAC%d, with flags 0x%x\n", module, lane, cgx, lmac, net_flags);
//
//	switch (qlm_state.s.mode)
//	{
//		case QLM_MODE_10G_KR:
//		case QLM_MODE_25G_CR:
//		case QLM_MODE_25G_KR:
//			use_training = true;
//			an_lane_mode = 2; /* AN master on all lanes */
//			break;
//		case QLM_MODE_50G_CR2:
//		case QLM_MODE_50G_KR2:
//			use_training = true;
//			/* AN master on all even lanes, slave on odd */
//			an_lane_mode = 1;
//			if (lmac == 0)
//				an_lane_mode = 2;
//			if (lmac == 2)
//				an_lane_mode = 2;
//			break;
//		case QLM_MODE_40G_CR4:
//		case QLM_MODE_40G_KR4:
//		case QLM_MODE_100G_CR4:
//		case QLM_MODE_100G_KR4:
//			use_training = true;
//			/* AN master on first lane, slave on all others */
//			an_lane_mode = 1;
//			if (lmac == 0)
//				an_lane_mode = 2;
//			break;
//		default:
//			lmac = 0;
//			use_training = false;
//			an_lane_mode = 0; /* Skipping AN as default */
//			break;
//	}
//
//	/* Configure the link training to request PRESET or INITIALIZE */
//	if (use_training && (an_lane_mode != 0))
//	{
//		if (gser_config_get_int(GSER_CONFIG_QLM_LINK_TRAINING_INITIAL_STATE, qlm, lane))
//		{
//			/* Configure LT to request Preset */
//			GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_SPARE_CFG0_RSVD(module, lane),
//				c.s.data |= 0x4);
//		}
//		else
//		{
//			/* Configure LT to request Init */
//			GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_SPARE_CFG0_RSVD(module, lane),
//				c.s.data |= 0x8);
//		}
//	}
//
//	/* Program the AN config structure for this lane */
//	const int AN_CFG_DATA_ADDR = 0x0;
//	const int AN_CFG_DATA_STRUCT_SIZE = 0x6b;
//	/* First 4 bytes are indexes into AN_CFG_DATA_STRUCT for each lane. These
//	   are always sequential in our setup */
//	write_dmem(module, AN_CFG_DATA_ADDR+0, 0x00);
//	write_dmem(module, AN_CFG_DATA_ADDR+1, 0x01);
//	write_dmem(module, AN_CFG_DATA_ADDR+2, 0x02);
//	write_dmem(module, AN_CFG_DATA_ADDR+3, 0x03);
//	int lane_offset = AN_CFG_DATA_ADDR + 4 + AN_CFG_DATA_STRUCT_SIZE * lane;
//	/* 0x0 nonce_seed 7:0 Seed value for TX Nonce Generator
//	   The lane number (0-3) will be added to this value to guarantee that each
//	   lane has a unique TX nonce */
//	uint8_t nonce = 0;
//	while ((nonce < 1) || (nonce >= 0xfd))
//		nonce = gser_rng_get_random8();
//	write_dmem(module, lane_offset++, nonce);
//	/* 0x1 priority_1g_kx 7:0 Resolution priority for 1000BASE-KX */
//	write_dmem(module, lane_offset++, 15);
//	/* 0x2 priority_10g_kx4 7:0 Resolution priority for 10GBASE-KX
//	   Not supported by the PHY */
//	write_dmem(module, lane_offset++, 14);
//	/* 0x3 priority_10g_kr 7:0 Resolution priority for 10GBASE-KR */
//	write_dmem(module, lane_offset++, 13);
//	/* 0x4 priority_40g_kr4 7:0 Resolution priority for 40GBASE-KR4 */
//	write_dmem(module, lane_offset++, 8);
//	/* 0x5 priority_40g_cr4 7:0 Resolution priority for 40GBASE-CR4 */
//	write_dmem(module, lane_offset++, 7);
//	/* 0x6 priority_100g_cr10 7:0 Resolution priority for 100GBASE-CR10
//	   Not supported by the PHY */
//	write_dmem(module, lane_offset++, 4);
//	/* 0x7 priority_100g_kp4 7:0 Resolution priority for 100GBASE-KP4
//	   Not supported by the PHY */
//	write_dmem(module, lane_offset++, 3);
//	/* 0x8 priority_100g_kr4 7:0 Resolution priority for 100GBASE-KR4 */
//	write_dmem(module, lane_offset++, 2);
//	/* 0x9 priority_100g_cr4 7:0 Resolution priority for 100GBASE-CR4 */
//	write_dmem(module, lane_offset++, 1);
//	/* 0xA priority_25g_gr_s 7:0 Resolution priority for 802.3by 25GBASE-CR-S / 25GBASE-KR-S */
//	write_dmem(module, lane_offset++, 10);
//	/* 0xB priority_25g_gr 7:0 Resolution priority for 802.3by 25GBASE-CR / 25GBASE-KR */
//	write_dmem(module, lane_offset++, 9);
//	/* 0xC priority_25g_kr 7:0 Resolution priority for 25G/50G Consortium 25GBASE-KR */
//	write_dmem(module, lane_offset++, 12);
//	/* 0xD priority_25g_cr 7:0 Resolution priority for 25G/50G Consortium 25GBASE-CR */
//	write_dmem(module, lane_offset++, 11);
//	/* 0xE priority_50g_kr2 7:0 Resolution priority for 25G/50G Consortium 50GBASE-KR2 */
//	write_dmem(module, lane_offset++, 6);
//	/* 0xF priority_50g_cr2 7:0 Resolution priority for 25G/50G Consortium 50GBASE-CR2 */
//	write_dmem(module, lane_offset++, 5);
//	/* 0x10 tech_abilities_15_8
//	   0:0 Advertise 100GBASE-CR4 in Base Page bit A8
//	   1:1 Advertise 802.3by 25GBASE-KR-S or 25GBASE-CR-S in Base Page bit A9
//	   2:2 Advertise 802.3by 25GBASE-KR or 25GBASE-CR in Base Page bit A10
//	   3:3 Advertise 25G/50G Consortium 25GBASE-KR in 25G/50G Consortium OUI Next Page
//	   4:4 Advertise 25G/50G Consortium 50GBASE-CR in 25G/50G Consortium OUI Next Page
//	   5:5 Advertise 25G/50G Consortium 25GBASE-KR2 in 25G/50G Consortium OUI Next Page
//	   6:6 Advertise 25G/50G Consortium 50GBASE-CR2 in 25G/50G Consortium OUI Next Page
//	   7:7 Reserved */
//	uint8_t tech_abilities_15_8 = 0;
//	if (qlm_state.s.mode == QLM_MODE_100G_CR4)
//		tech_abilities_15_8 |= 1 << 0;
//	if ((qlm_state.s.mode == QLM_MODE_25G_CR) || (qlm_state.s.mode == QLM_MODE_25G_KR))
//	{
//		tech_abilities_15_8 |= 1 << 1;
//		tech_abilities_15_8 |= 1 << 2;
//		tech_abilities_15_8 |= 1 << 3;
//		tech_abilities_15_8 |= 1 << 4;
//	}
//	if ((qlm_state.s.mode == QLM_MODE_50G_CR2) || (qlm_state.s.mode == QLM_MODE_50G_KR2))
//	{
//		tech_abilities_15_8 |= 1 << 5;
//		tech_abilities_15_8 |= 1 << 6;
//	}
//	/* 0x11 tech_abilities_7_0
//	   0:0 Advertise 1000BASE-KX in Base Page bit A0
//	   1:1 Advertise 10GBASE-KX4 in Base Page bit A1
//			Not supported by the PHY
//	   2:2 Advertise 10GBASE-KR in Base Page bit A2
//	   3:3 Advertise 40GBASE-KR4 in Base Page bit A3
//	   4:4 Advertise 40GBASE-CR4 in Base Page bit A4
//	   5:5 Advertise 100GBASE-CR10 in Base Page bit A5
//			Not supported by the PHY
//	   6:6 Advertise 100GBASE-KP4 in Base Page bit A6
//			Not supported by the PHY
//	   7:7 Advertise 100GBASE-KR4 in Base Page bit A7 */
//	uint8_t tech_abilities_7_0 = 0;
//	if (qlm_state.s.mode == QLM_MODE_10G_KR)
//		tech_abilities_7_0 |= 1 << 2;
//	if (qlm_state.s.mode == QLM_MODE_40G_KR4)
//		tech_abilities_7_0 |= 1 << 3;
//	if (qlm_state.s.mode == QLM_MODE_40G_CR4)
//		tech_abilities_7_0 |= 1 << 4;
//	if (qlm_state.s.mode == QLM_MODE_100G_KR4)
//		tech_abilities_7_0 |= 1 << 7;
//
//	int64_t tech_abilities_override = gser_config_get_int(GSER_CONFIG_QLM_OVERRIDE_AN_ABILITIES, qlm, lane);
//	if (tech_abilities_override)
//	{
//		GSER_TRACE(QLM, "GSERC%d.%d: Override AN tech_abilites: Default: 0x%02x%02x, Override: 0x%04x\n",
//			module, lane, tech_abilities_15_8, tech_abilities_7_0, (uint32_t)(tech_abilities_override & 0xffff));
//		tech_abilities_15_8 = (tech_abilities_override >> 8) & 0xff;
//		tech_abilities_7_0 = tech_abilities_override & 0xff;
//	}
//	write_dmem(module, lane_offset++, tech_abilities_15_8);
//	write_dmem(module, lane_offset++, tech_abilities_7_0);
//	/* 0x12 eee_tech_abilities_15_8 (RESERVED)
//	   0:0 Advertise 100GBASE-CR4 has EEE deep sleep capability
//	   1:1 Advertise 802.3by 25GBASE-KR-S or 25GBASE-CR-S has EEE deep sleep capability
//	   2:2 Advertise 802.3by 25GBASE-KR or 25GBASE-CR has EEE deep sleep capability
//	   3:3 Advertise 25G/50G Consortium 25GBASE-KR has EEE deep sleep capability
//	   4:4 Advertise 25G/50G Consortium 50GBASE-CR has EEE deep sleep capability
//	   5:5 Advertise 25G/50G Consortium 25GBASE-KR2 has EEE deep sleep capability
//	   6:6 Advertise 25G/50G Consortium 50GBASE-CR2 has EEE deep sleep capability
//	   7:7 Reserved */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x13 eee_tech_abilities_7_0 (RESERVED)
//	   0:0 Advertise 1000BASE-KX has EEE capability
//	   1:1 Advertise 10GBASE-KX4 has EEE capability
//			Not supported by the PHY
//	   2:2 Advertise 10GBASE-KR has EEE capability
//	   3:3 Advertise 40GBASE-KR4 has EEE deep sleep capability
//	   4:4 Advertise 40GBASE-CR4 has EEE deep sleep capability
//	   5:5 Advertise 100GBASE-CR10 has EEE deep sleep capability
//			Not supported by the PHY
//	   6:6 Advertise 100GBASE-KP4 has EEE deep sleep capability
//			Not supported by the PHY
//	   7:7 Advertise 100GBASE-KR4 has EEE deep sleep capability */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x14 fec
//	   0:0 Advertise Clause 74 FEC (Fire Code) Ability in Base Page F0 bit
//	   1:1 Request Clause 74 FEC (Fire Code) in Base Page F1 bit
//	   2:2 Request Clause 91 FEC (Reed Solomon) for 8023.by 25GBASE-KR or
//			25GBASE-CR in Base Page F2 bit
//	   3:3 Request Clause 74 FEC (Fire Code) for 8023.by 25GBASE-KR or
//			25GBASE-CR-S or 25GBASE-KR-S or 25GBASE-CR-S in Base Page F3 bit
//	   4:4 Advertise Clause 91 FEC (Reed Solomon) Ability in 25G/50G Consortium
//			OUI Next Pages
//	   5:5 Advertise Clause 74 FEC (Fire Code) Ability in 25G/50G Consortium
//			OUI Next Pages
//	   6:6 Request Clause 91 FEC (Reed Solomon) in 25G/50G Consortium OUI Next Pages
//	   7:7 Request Clause 74 FEC (Fire Code) in 25G/50G Consortium OUI Next Page */
//	uint8_t fec = 0;
//	fec |= 1 << 0; /* Advertise FEC in base page */
//	fec |= 1 << 4; /* Advertise RS-FEC for 25G/50G Consortium */
//	fec |= 1 << 5; /* Advertise FEC for 25G/50G Consortium */
//	if (net_flags & GSER_NETPHY_FLAG_FEC) /* Fire Code */
//	{
//		fec |= 1 << 1;
//		fec |= 1 << 3;
//		fec |= 1 << 7;
//	}
//	if (net_flags & GSER_NETPHY_FLAG_RS_FEC) /* Reed Solomon */
//	{
//		fec |= 1 << 2;
//		fec |= 1 << 6;
//	}
//	write_dmem(module, lane_offset++, fec);
//	/* 0x15 pause
//	   0:0 Advertise Pause Ability in Base Page bit C0
//	   1:1 Advertise Pause ASM_DIR Ability in Base Page C1 */
//	write_dmem(module, lane_offset++, 0x03);
//	/* 0x16 np_cfg
//	   0:0 Send 25G/50G Consortium OUI tagged Next Pages to advertise 25G/50G
//			abilities during negotiation
//	   1:1 Send a Device ID Next Pages during negotiation
//	   2:2 Send an EEE technology Next Page during negotiation
//			(RESERVED setting – leave at 0, as the AFE does support EEE functionality). */
//	uint8_t np_cfg = 0;
//	if ((qlm_state.s.mode == QLM_MODE_50G_CR2) || (qlm_state.s.mode == QLM_MODE_50G_KR2) ||
//		(qlm_state.s.mode == QLM_MODE_25G_CR) || (qlm_state.s.mode == QLM_MODE_25G_KR))
//		np_cfg |= 1 << 0;
//	write_dmem(module, lane_offset++, np_cfg);
//	/* 0x17 reserved 7:0 N/A */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x18 consortium_oui_23_16 7:0 OUI to use for the OUI field in 25G/50G Consortium OUI tagged Next Pages (Bits 23:16) */
//	write_dmem(module, lane_offset++, 0x6a);
//	/* 0x19 consortium_oui_15_8 7:0 OUI to use for the OUI field in 25G/50G Consortium OUI tagged Next Pages (Bits 15:8) */
//	write_dmem(module, lane_offset++, 0x73);
//	/* 0x1A consortium_oui_7_0 7:0 OUI to use for the OUI field in 25G/50G Consortium OUI tagged Next Pages (Bits 7:0) */
//	write_dmem(module, lane_offset++, 0x7d);
//	/* 0x1B device_id_31_24 7:0 32-bit PHY Device ID to send in Device ID Next Pages (Bits 32:24) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x1C device_id_31_25 7:0 32-bit PHY Device ID to send in Device ID Next Pages (Bits 23:16) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x1D device_id_31_26 7:0 32-bit PHY Device ID to send in Device ID Next Pages (Bits 15:8) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x1E device_id_31_27 7:0 32-bit PHY Device ID to send in Device ID Next Pages (Bits 7:0) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x1F num_custom_tx_nps 7:0 How many custom Next Pages to send (Maximum 10)
//	   These are in addition to Device ID, EEE technology, and 25G/50G Consortium Next Pages */
//	write_dmem(module, lane_offset++, 0x00);
//
//	/* 0x20 custom_tx_nps0_47_41 7:0 1st Custom Next Page to send (Bits 47:41) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x21 custom_tx_nps0_40_32 7:0 1st Custom Next Page to send (Bits 40:32) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x22 custom_tx_nps0_31_24 7:0 1st Custom Next Page to send (Bits 31:24) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x23 custom_tx_nps0_23_16 7:0 1st Custom Next Page to send (Bits 23:16) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x24 custom_tx_nps0_15_8 7:0 1st Custom Next Page to send (Bits 15:8) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x25 custom_tx_nps0_7_0 7:0 1st Custom Next Page to send (Bits 7:0) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x26 custom_tx_nps1_47_41 7:0 2nd Custom Next Page to send (Bits 47:41) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x27 custom_tx_nps1_40_32 7:0 2nd Custom Next Page to send (Bits 40:32) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x28 custom_tx_nps1_31_24 7:0 2nd Custom Next Page to send (Bits 31:24) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x29 custom_tx_nps1_23_16 7:0 2nd Custom Next Page to send (Bits 23:16) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x2A custom_tx_nps1_15_8 7:0 2nd Custom Next Page to send (Bits 15:8) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x2B custom_tx_nps1_7_0 7:0 2nd Custom Next Page to send (Bits 7:0) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x2C custom_tx_nps2_47_41 7:0 3rd Custom Next Page to send (Bits 47:41) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x2D custom_tx_nps2_40_32 7:0 3rd Custom Next Page to send (Bits 40:32) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x2E custom_tx_nps2_31_24 7:0 3rd Custom Next Page to send (Bits 31:24) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x2F custom_tx_nps2_23_16 7:0 3rd Custom Next Page to send (Bits 23:16) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x30 custom_tx_nps2_15_8 7:0 3rd Custom Next Page to send (Bits 15:8) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x31 custom_tx_nps2_7_0 7:0 3rd Custom Next Page to send (Bits 7:0) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x32 custom_tx_nps3_47_41 7:0 4th Custom Next Page to send (Bits 47:41) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x33 custom_tx_nps3_40_32 7:0 4th Custom Next Page to send (Bits 40:32) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x34 custom_tx_nps3_31_24 7:0 4th Custom Next Page to send (Bits 31:24) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x35 custom_tx_nps3_23_16 7:0 4th Custom Next Page to send (Bits 23:16) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x36 custom_tx_nps3_15_8 7:0 4th Custom Next Page to send (Bits 15:8) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x37 custom_tx_nps3_7_0 7:0 4th Custom Next Page to send (Bits 7:0) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x38 custom_tx_nps4_47_41 7:0 5th Custom Next Page to send (Bits 47:41) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x39 custom_tx_nps4_40_32 7:0 5th Custom Next Page to send (Bits 40:32) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x3A custom_tx_nps4_31_24 7:0 5th Custom Next Page to send (Bits 31:24) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x3B custom_tx_nps4_23_16 7:0 5th Custom Next Page to send (Bits 23:16) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x3C custom_tx_nps4_15_8 7:0 5th Custom Next Page to send (Bits 15:8) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x3D custom_tx_nps4_7_0 7:0 5th Custom Next Page to send (Bits 7:0) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x3E custom_tx_nps5_47_41 7:0 6th Custom Next Page to send (Bits 47:41) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x3F custom_tx_nps5_40_32 7:0 6th Custom Next Page to send (Bits 40:32) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x40 custom_tx_nps5_31_24 7:0 6th Custom Next Page to send (Bits 31:24) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x41 custom_tx_nps5_23_16 7:0 6th Custom Next Page to send (Bits 23:16) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x42 custom_tx_nps5_15_8 7:0 6th Custom Next Page to send (Bits 15:8) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x43 custom_tx_nps5_7_0 7:0 6th Custom Next Page to send (Bits 7:0) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x44 custom_tx_nps6_47_41 7:0 7th Custom Next Page to send (Bits 47:41) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x45 custom_tx_nps6_40_32 7:0 7th Custom Next Page to send (Bits 40:32) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x46 custom_tx_nps6_31_24 7:0 7th Custom Next Page to send (Bits 31:24) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x47 custom_tx_nps6_23_16 7:0 7th Custom Next Page to send (Bits 23:16) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x48 custom_tx_nps6_15_8 7:0 7th Custom Next Page to send (Bits 15:8) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x49 custom_tx_nps6_7_0 7:0 7th Custom Next Page to send (Bits 7:0) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x4A custom_tx_nps7_47_41 7:0 8th Custom Next Page to send (Bits 47:41) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x4B custom_tx_nps7_40_32 7:0 8th Custom Next Page to send (Bits 40:32) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x4C custom_tx_nps7_31_24 7:0 8th Custom Next Page to send (Bits 31:24) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x4D custom_tx_nps7_23_16 7:0 8th Custom Next Page to send (Bits 23:16) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x4E custom_tx_nps7_15_8 7:0 8th Custom Next Page to send (Bits 15:8) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x4F custom_tx_nps7_7_0 7:0 8th Custom Next Page to send (Bits 7:0) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x50 custom_tx_nps8_47_41 7:0 9th Custom Next Page to send (Bits 47:41) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x51 custom_tx_nps8_40_32 7:0 9th Custom Next Page to send (Bits 40:32) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x52 custom_tx_nps8_31_24 7:0 9th Custom Next Page to send (Bits 31:24) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x53 custom_tx_nps8_23_16 7:0 9th Custom Next Page to send (Bits 23:16) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x54 custom_tx_nps8_15_8 7:0 9th Custom Next Page to send (Bits 15:8) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x55 custom_tx_nps8_7_0 7:0 9th Custom Next Page to send (Bits 7:0) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x56 custom_tx_nps9_47_41 7:0 10th Custom Next Page to send (Bits 47:41) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x57 custom_tx_nps9_40_32 7:0 10th Custom Next Page to send (Bits 40:32) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x58 custom_tx_nps9_31_24 7:0 10th Custom Next Page to send (Bits 31:24) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x59 custom_tx_nps9_23_16 7:0 10th Custom Next Page to send (Bits 23:16) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x5A custom_tx_nps9_15_8 7:0 10th Custom Next Page to send (Bits 15:8) */
//	write_dmem(module, lane_offset++, 0x00);
//	/* 0x5B custom_tx_nps9_7_0 7:0 10th Custom Next Page to send (Bits 7:0) */
//	write_dmem(module, lane_offset++, 0x00);
//
//	/* 0x5C width_1g_kx 7:0 Parallel width to use for 1000BASE-KX */
//	write_dmem(module, lane_offset++, 0x02);
//	/* 0x5D width_10g_kx4 7:0 Parallel width to use for 10GBASE-KX
//	   Not supported by the PHY */
//	write_dmem(module, lane_offset++, 0x02);
//	/* 0x5E width_10g_kr
//		7:4 Rx Parallel interface width to use for 10GBASE-KR
//		3:0 Tx Parallel interface width to use for 10GBASE-KR */
//	write_dmem(module, lane_offset++, 0x32);
//	/* 0x5F width_40g_kr4
//		7:4 Rx Parallel interface width to use for 40GBASE-KR4
//		* 3:0 Tx Parallel interface width to use for 40GBASE-KR4 */
//	write_dmem(module, lane_offset++, 0x32);
//	/* 0x60 width_40g_cr4
//		7:4 Rx Parallel interface width to use for 40GBASE-CR4
//		3:0 Tx Parallel interface width to use for 40GBASE-CR4 */
//	write_dmem(module, lane_offset++, 0x32);
//	/* 0x61 width_100g_cr10 7:0 Parallel interface width to use for 100GBASE-CR10
//	   Not supported by the PHY */
//	write_dmem(module, lane_offset++, 0x04);
//	/* 0x62 width_100g_kp4 7:0 Parallel interface width to use for 100GBASE-KP4
//	   Not supported by the PHY */
//	write_dmem(module, lane_offset++, 0x04);
//	/* 0x63 width_100g_kr4
//		7:4 Rx Parallel interface width to use for 100GBASE-KR4
//		3:0 Tx Parallel interface width to use for 100GBASE-KR4 */
//	write_dmem(module, lane_offset++, 0x54);
//	/* 0x64 width_100g_cr4
//		7:4 Rx Parallel interface width to use for 100GBASE-CR4
//		3:0 Tx Parallel interface width to use for 100GBASE-CR4 */
//	write_dmem(module, lane_offset++, 0x54);
//	/* 0x65 width_25g_gr_s
//		7:4 Rx Parallel interface width to use for 802.3by 25GBASE-CR-S / 25GBASE-KR-S
//		3:0 Tx Parallel interface width to use for 802.3by 25GBASE-CR-S / 25GBASE-KR-S */
//	write_dmem(module, lane_offset++, 0x54);
//	/* 0x66 width_25g_gr
//		7:4 Rx Parallel interface width to use for 802.3by 25GBASE-CR / 25GBASE-KR
//		3:0 Tx Parallel interface width to use for 802.3by 25GBASE-CR / 25GBASE-KR */
//	write_dmem(module, lane_offset++, 0x54);
//	/* 0x67 width_25g_kr
//		7:4 Rx Parallel interface width to use for 25G/50G Consortium 25GBASE-KR
//		3:0 Tx Parallel interface width to use for 25G/50G Consortium 25GBASE-KR */
//	write_dmem(module, lane_offset++, 0x54);
//	/* 0x68 width_25g_cr
//		7:4 Rx Parallel interface width to use for 25G/50G Consortium 25GBASE-CR
//		3:0 Tx Parallel interface width to use for 25G/50G Consortium 25GBASE-CR */
//	write_dmem(module, lane_offset++, 0x54);
//	/* 0x69 width_50g_kr2
//		7:4 Rx Parallel interface width to use for 25G/50G Consortium 50GBASE-KR2
//		3:0 Tx Parallel interface width to use for 25G/50G Consortium 50GBASE-KR2 */
//	write_dmem(module, lane_offset++, 0x54);
//	/* 0x6A width_50g_cr2
//		7:4 Rx Parallel interface width to use for 25G/50G Consortium 50GBASE-CR2
//		3:0 Tx Parallel interface width to use for 25G/50G Consortium 50GBASE-CR2 */
//	write_dmem(module, lane_offset++, 0x54);
//	if (lane_offset != AN_CFG_DATA_ADDR + 4 + AN_CFG_DATA_STRUCT_SIZE * (lane + 1))
//		gser_fatal("GSERC%d.%d: Incorrect offset into DMEM %d\n", module, lane, lane_offset);
//
//	/* Enable or disable training after AN */
//	GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_LT_TX_FSM_CTRL0(module, lane),
//		c.s.mr_training_enable = (use_training) ? 1 : 0);
//	/* Configure how the lane handles AN */
//	/* 0 = No AN, 1 = AN Slave, 2 = AN Master */
//	GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
//		c.s.ln_an_cfg = an_lane_mode);
//}

/**
 * Read trace data from the serdes
 *
 * @param node	   Node to read from
 * @param module	 SERDES module to read from
 * @param lane	   Lane to read from
 * @param trace_type Type of data to read:
 *				   1 for AN trace
 *				   2 for LT training state machine trace
 *				   3 for LT coefficient update state machine trace
 * @param data	   Array to contain data
 * @param data_size  Size of the data in bytes
 *
 * @return Length of data read in bytes, or negative on failure
 */
static int qlm_gserc_get_trace(int module, int lane, int trace_type, uint8_t *data, int data_size)
{
	uint64_t args = (trace_type << 8) | lane;
	if (mailbox_command(module, 0x8a, args))
		return -1;
	int status = -1;
	int loc = 0;
	do
	{
		if (loc + 16 >= data_size)
		{
			uint64_t dummy;
			while (status == 0)
			{
				status = mailbox_response(module, &dummy, &dummy);
				loc += 16;
			}
			gser_error("GSERC%d.%d: Not enough room for trace data, need %d\n", module, lane, loc);
			return -1;
		}
		status = mailbox_response(module, (uint64_t*)&data[loc], (uint64_t*)&data[loc + 8]);
		loc += 16;
	} while (status == 0);
	return loc;
}

static int decode_header(uint64_t data, int length, int max_entries)
{
	int trace_bytes = gser_be32_to_cpu(gser_extract(data, 0, 32));
	int total_entries = gser_be16_to_cpu(gser_extract(data, 32, 16));
	int actual_entries = (total_entries > max_entries) ? max_entries : total_entries;
	int trace_idx = gser_be16_to_cpu(gser_extract(data, 48, 16));
	printf("	Dumped Bytes:   %d\n", length);
	printf("	Trace Size:	 %d\n", trace_bytes);
	printf("	Total Entries:  %d\n", total_entries);
	printf("	Actual Entries: %d\n", actual_entries);
	printf("	Idx:			%d\n", trace_idx);
	return actual_entries;
}

static void decode_an(uint8_t *data, int length)
{
	const char *EVENTS[] = {
		"AN_CFG_DROPPED_IN_RESET",
		"AN_CFG_SET_TO_MASTER",
		"ALL_SLAVES_REACHED_RESET",
		"LANE_RCHNG_RST",
		"LANE_RCHNG_COMPLETE",
		"LANE_RESET_MISSION",
		"SLAVE_MASTER_RESOLVED",
		"DISABLED",
		"AN_CFG_DROPPED_IN_ACTIVE",
		"STARTING_NEGOTIATION",
		"NEGOTIATION_COMPLETE",
		"RESOLVED_1G",
		"NEW_INIT_REQUEST",
		"SLAVE_COMMANDED_TO_RESET",
		"STARTING_DISABLE",
		"DISABLING_RCHNG_RST",
		"DISABLING_RESETTING",
		"DISABLING_REACHED_RESET",
		"DISABLING_START_DISABLING_AN",
		"DISABLING_RCHNG_RST_FIXED",
		"DISABLING_REACHED_RESET_FIXED",
		"DISABLING_SLAVES_SYNCHED",
		"DISABLING_COMPLETE",
		"DISABLING_ALREADY_DONE",
		"HANDSHAKE_COMPLETE",
		"HANDSHAKE_DISABLED",
		"HANDSHAKE_SLAVE_COMPLETE"
	};

	const char *STATES[] = {
		"OFF",
		"RESET",
		"RESET_MISSION",
		"SLAVE_WAIT",
		"RATE_CHANGE_DONE",
		"WAIT_RATE_CHANGE_HANDSHAKE",
		"WAIT_RATE_CHANGE",
		"START_AUTO_NEG",
		"AUTO_NEG",
		"MASTER_ACTIVE",
		"SLAVE_ACTIVE"
	};

	const int RECORD_SIZE = 3;
	int entries = decode_header(*(uint64_t *)data, length, 48);
	printf("		Event							  State						   Flags\n");
	for (int e = 0; e < entries; e++)
	{
		int i = e * RECORD_SIZE + 8;
		uint8_t event = data[i];
		uint8_t state = data[i + 1];
		uint8_t flags = data[i + 2];
		printf("	%2d: (%2d) %-29s (%2d) %-26s (0x%02x)%s%s%s%s%s%s\n",
			e, event, EVENTS[event], state, STATES[state], flags,
			(flags & 0x01) ? " FLAG_ENABLED" : "",
			(flags & 0x02) ? " FLAG_MASTER" : "",
			(flags & 0x04) ? " FLAG_RESOLVED" : "",
			(flags & 0x08) ? " FLAG_BASE_PAGE_DONE" : "",
			(flags & 0x10) ? " FLAG_DISABLING" : "",
			(flags & 0x20) ? " HANDSHAKE_DONE" : "");
	}
}

static void decode_training_coef(uint8_t *data, int length)
{
	const char *EVENTS[] = {
		"STOPPING",
		"STARTING",
		"INITIALIZING",
		"FRAME_LOCKED",
		"NEW_REQUEST",
		"HOLD",
		"PRESET_APPLIED",
		"INIT_APPLIED",
		"UPDATED",
		"ABORTED"
	};

	const char *STATES[] = {
		"IDLE",
		"WAIT_FRAME_LOCK",
		"LOCKED"
	};

	const int RECORD_SIZE = 5;
	int entries = decode_header(*(uint64_t *)data, length, 64);
	printf("		Event			   State				Request						  Mask						  Flags\n");
	for (int e = 0; e < entries; e++)
	{
		int i = e * RECORD_SIZE + 8;
		uint8_t event = data[i];
		uint8_t state = data[i + 1];
		uint8_t req = data[i + 2];
		uint8_t mask = data[i + 3];
		uint8_t flags = data[i + 4];
		const char *REQ_NAMES[4] = { "Hold", "Inc ", "Dec ", "????" };
		const char *req_cm1 = REQ_NAMES[(req >> 4) & 3];
		const char *req_c0 = REQ_NAMES[(req >> 2) & 3];
		const char *req_c1 = REQ_NAMES[req & 3];
		const char *req_init = (req & 0x40) ? "INIT" : "----";
		const char *req_prst = (req & 0x80) ? "PRST" : "----";
		const char *cm1;
		const char *c0;
		const char *c1;
		const char *init = (mask & 0x40) ? "INIT" : "----";
		const char *prst = (mask & 0x80) ? "PRST" : "----";
		switch ((mask >> 4) & 3)
		{
			case 0:
				cm1 = "---";
				break;
			case 3:
				cm1 = "Cm1";
				break;
			default:
				cm1 = "???";
				break;
		}
		switch ((mask >> 2) & 3)
		{
			case 0:
				c0 = "---";
				break;
			case 3:
				c0 = "C0 ";
				break;
			default:
				c0 = "???";
				break;
		}
		switch (mask & 3)
		{
			case 0:
				c1 = "---";
				break;
			case 3:
				c1 = "C1 ";
				break;
			default:
				c1 = "???";
				break;
		}
		printf("	%2d: (%2d) %-14s (%2d) %-15s (0x%02x) %s|%s|%s|%s|%s  (0x%02x) %s|%s|%s|%s|%s  (0x%02x)%s%s\n",
			e, event, EVENTS[event], state, STATES[state],
			req, req_cm1, req_c0, req_c1, req_init, req_prst,
			mask, cm1, c0, c1, init, prst, flags,
			(flags & 0x01) ? " FLAG_RESTART" : "",
			(flags & 0x02) ? " FLAG_STOP" : "");
	}
}

static void decode_training_state(uint8_t *data, int length)
{
	const char *EVENTS[] = {
		"INITIALIZING",
		"RX_FRAME_LOCKED",
		"NEW_TX_REQ",
		"COEFFS_NOT_UPDATED",
		"COEFFS_APPLIED",
		"RX_TRAINED",
		"RX_TRAINED_DEASSERTED",
		"REMOTE_RX_READY",
		"REMOTE_RX_READY_DEASSERTED",
		"SEND_DATA",
		"RESTART_TRAINING_ASSERTED",
		"TRAINING_FAIL_ASSERTED",
		"RX_FRAME_LOCK_LOST",
		"ABORTED"
	};

	const char *STATES[] = {
		"INIT",
		"WAIT_FRAME_LOCK",
		"FRAME_LOCKED",
		"WAIT_COEFF_NOT_UPDATED",
		"WAIT_COEFF_UPDATED",
		"WAIT_COEFF_REQ",
		"RX_ADAPT",
		"TX_EVAL",
		"TRAIN_REMOTE",
		"LINK_READY",
		"SEND_DATA",
		"TRAINING_FAILURE"
	};

	const int RECORD_SIZE = 5;
	int entries = decode_header(*(uint64_t *)data, length, 64);
	printf("		Event						   State					   Update Request				   Coeff Status		Flags\n");
	for (int e = 0; e < entries; e++)
	{
		int i = e * RECORD_SIZE + 8;
		uint8_t event = data[i];
		uint8_t state = data[i + 1];
		uint8_t req = data[i + 2];
		uint8_t coeff = data[i + 3];
		uint8_t flags = data[i + 4];
		const char *REQ_NAMES[4] = { "Hold", "Inc ", "Dec ", "????" };
		const char *req_cm1 = REQ_NAMES[(req >> 4) & 3];
		const char *req_c0 = REQ_NAMES[(req >> 2) & 3];
		const char *req_c1 = REQ_NAMES[req & 3];
		const char *req_init = (req & 0x40) ? "INIT" : "----";
		const char *req_prst = (req & 0x80) ? "PRST" : "----";
		const char *COEFF_NAMES[4] = { "---", "Upd", "Min", "Max" };
		const char *coeff_cm1 = COEFF_NAMES[(coeff >> 4) & 3];
		const char *coeff_c0 = COEFF_NAMES[(coeff >> 2) & 3];
		const char *coeff_c1 = COEFF_NAMES[coeff & 3];
		printf("	%2d: (%2d) %-26s (%2d) %-22s (0x%02x) %s|%s|%s|%s|%s  (0x%02x) %s|%s|%s  (0x%02x)%s%s%s\n",
			e, event, EVENTS[event], state, STATES[state],
			req, req_cm1, req_c0, req_c1, req_init, req_prst,
			coeff, coeff_cm1, coeff_c0, coeff_c1, flags,
			(flags & 0x01) ? " FLAG_RX_TRAINED" : "",
			(flags & 0x02) ? " FLAG_RESTARTED" : "",
			(flags & 0x04) ? " FLAG_TX_REQ" : "");
	}
}

/**
 * Some SERDES can display extra tracing about AN, training, etc. This function
 * displays those traces if available. If the SERDES doesn't support tracing, a
 * informational message tells the user.
 *
 * @param node   Node to trace
 * @param module GSERC to trace
 * @param lane   Lane to trace
 * @param unused Unused argument. Present so a number of QLM functions have the same signature
 *			   for easy calling in the network driver
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserc_display_trace(int module, int lane, int unused)
{
	uint8_t data[64 * 8];
	printf("GSERC%d.%d: AN trace\n", module, lane);
	int len = qlm_gserc_get_trace(module, lane, 1, data, sizeof(data));
	decode_an(data, len);
	printf("GSERC%d.%d: Training state machine trace\n", module, lane);
	len = qlm_gserc_get_trace(module, lane, 2, data, sizeof(data));
	decode_training_state(data, len);
	printf("GSERC%d.%d: Training coefficient trace\n", module, lane);
	len = qlm_gserc_get_trace(module, lane, 3, data, sizeof(data));
	decode_training_coef(data, len);
	return 0;
}

/**
 * Reset the QLM layer
 */
void qlm_gserc_init_reset()
{
	GSER_CSR_INIT(gsercx_init_ctl, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(0));
	if (!gsercx_init_ctl.s.cpu_reset)
	{
		GSER_TRACE(QLM, "GSERC: Already running, forcing a reset\n");
		for (int module = 0; module < num_gserc; module++)
			GSER_CSR_MODIFY(c, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(module),
				c.s.cm0_rst = 1;
				c.s.por = 1;
				c.s.cpu_reset = 1;
				c.s.apb_reset = 1);
		gser_wait_usec(1000);
	}

	/* Program the GSERC reference clock termination */
	for (int module = 0; module < num_gserc; module++)
	{
		int qlm = map_module_to_qlm(module);
		int clk_term = gser_config_get_int(GSER_CONFIG_QLM_CLK_TERM, qlm);
		switch (clk_term)
		{
			case 1: /* 100 ohm termination */
				GSER_TRACE(QLM, "GSERC%d: Using 100 Ohm termination\n", module);
				GSER_CSR_MODIFY(c, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(module),
					c.s.refclk_hiz_ena = 0);
				break;
			case 0: /* Unterminated */
				GSER_TRACE(QLM, "GSERC%d: Using no termination\n", module);
				GSER_CSR_MODIFY(c, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(module),
					c.s.refclk_hiz_ena = 1);
				break;
			default:
				gser_error("GSERC%d: Invalid termination %d\n", module, clk_term);
				break;
		}
	}
}

/**
 * Initialize function removed by gser-update script (qlm.sed)
 */
//void qlm_gserc_init()
//{
//	const char *firmware_file = "/fatfs/serdes/gserc-loki.frm.lzma";
//	int sensor;
//
//	/* Figure out which firmware file to use */
//	if (gser_is_model(OCTEONTX_LOKI))
//		sensor = 2;  /* TSN2 is nearest to GSERC */
//	else
//	{
//		gser_error("GSERC: Unrecognized chip\n");
//		return;
//	}
//
//	/* Open the firmware file */
//	GSER_TRACE(QLM, "GSERC: Loading firmware %s\n", firmware_file);
//	uint64_t filesize = 0;
//	uint64_t *firmware = gser_signed_load(firmware_file, 0,GSER_SIGNED_IMAGE, GSER_SIGNED_FLAG_NOT_ENCRYPTED, &filesize);
//	if (!firmware)
//	{
//		gser_error("GSERC: Failed to load firmware %s\n", firmware_file);
//		return;
//	}
//
//	/* 1) Load the PHY Firmware - While not EOF read the FW file and write the
//	   FW image to the Program Memory (96KB, 12288*8Bytes) */
//	/* Load the firmware */
//	for (int module = 0; module < num_gserc; module++)
//	{
//		for (int index = 0; index < (int)filesize / 8; index++)
//		{
//			uint64_t data = gser_cpu_to_be64(firmware[index]);
//			GSER_CSR_WRITE(CAVM_GSERCX_PMEMX(module, index), data);
//		}
//
//		/* Write protect program memory now that the firmware is loaded */
//		GSER_CSR_MODIFY(c, CAVM_GSERCX_COMMON_PHY_CTRL_PROT(module),
//			c.s.pmem_wr_prot_stky = 1);
//
//		/* Verify the firmware matches what we loaded */
//		for (int index = 0; index < (int)filesize / 8; index++)
//		{
//			uint64_t data = gser_cpu_to_be64(firmware[index]);
//			uint64_t tmp = GSER_CSR_READ(CAVM_GSERCX_PMEMX(module, index));
//			if (tmp != data)
//			{
//				gser_error("GSERC%d: Mismatch loading firmware[%d], wrote 0x%016lx, read 0x%016lx\n", module, index, data, tmp);
//				return;
//			}
//		}
//	}
//	free(firmware);
//	GSER_TRACE(QLM, "GSERC: Loaded firmware of %lu bytes\n", filesize);
//
//	/* Extract the firmware version. The firmware contains 32 bytes of version
//	   data embedded inside the program image at memory address 0x50.
//		Bytes Data						   Format
//		0-15  PHY Digital Architecture	   Null-terminated ASCII string
//		16	Revision Number				Binary
//		17	Revision Suffix				Binary
//		18-19 PHY H/W Platform			   2 ASCII characters
//		20-21 Development Branch			 Binary
//		22-23 PHY H/W Rev					2 ASCII characters
//		24-27 Reserved					   Binary
//		28-31 F/W Source Repository Revision 32-bit binary number (Big Endian) */
//	union
//	{
//		uint64_t u[2];
//		char text[16];
//	} phy_arch;
//	phy_arch.u[0] = gser_be64_to_cpu(GSER_CSR_READ(CAVM_GSERCX_PMEMX(0, 0x50 / 8)));
//	phy_arch.u[1] = gser_be64_to_cpu(GSER_CSR_READ(CAVM_GSERCX_PMEMX(0, 0x50 / 8 + 1)));
//	uint64_t phy_rev = gser_be64_to_cpu(GSER_CSR_READ(CAVM_GSERCX_PMEMX(0, 0x50 / 8 + 2)));
//	uint64_t phy_svn_rev = gser_be64_to_cpu(GSER_CSR_READ(CAVM_GSERCX_PMEMX(0, 0x50 / 8 + 3)));
//	int rev_number = gser_extract(phy_rev, 0, 8);
//	int rev_suffix = gser_extract(phy_rev, 8, 8);
//	char hw_plat0 = gser_extract(phy_rev, 16, 8);
//	char hw_plat1 = gser_extract(phy_rev, 24, 8);
//	int branch = gser_extract(phy_rev, 32, 16);
//	char hw_rev0 = gser_extract(phy_rev, 48, 8);
//	char hw_rev1 = gser_extract(phy_rev, 56, 8);
//	uint32_t svn_rev = gser_be32_to_cpu(gser_extract(phy_svn_rev, 32, 32));
//	GSER_TRACE(QLM, "GSERC: Firmware Arch:%s Rev:%d.%d Plat:%c%c Branch:%d HW:%c%c SVN:0x%x\n",
//		phy_arch.text, rev_number, rev_suffix, hw_plat0, hw_plat1, branch, hw_rev0, hw_rev1, svn_rev);
//
//	/* 2. For each “Active” QLM Program the Lane Rate and Width parameters for
//	   each lane.
//		Write GSERC()_LANE()_CONTROL_BCFG
//			LN_CTRL_TX_RATE=3’h0 //PHY Rate 1 used here, but other PHY rates available
//			LN_CTRL_RX_RATE=3’h0 //PHY Rate 1 used here, but other PHY rates available
//			LN_CTRL_TX_WIDTH=3’h4 //32-bit Tx data width @25G & 20.625G, or 3’h2 for 16-bit width at 10G and lower data rates
//			LN_CTRL_RX_WIDTH=3’h5 //40-bit Rx data width @25G & 20.625G, or 3’h3 for 20-bit width at 10G and lower data rates
//			TX_CLK_MUX_SEL=5’h0 //Select cm0_clk_rate1_o as the transmitter clock source, or appropriate clock source for other PHY rates
//			CGX_QUAD=0 //Set the CGX_QUAD=1 in all lanes which are part of a x4 link (Lanes 0,1,2,3).
//			CGX_DUAL=0 //Set the CGX_DUAL=1 in all lanes which are part of a x2 link (Lane 0&1, or Lanes 2&3) */
//	/* 3) For each QLM, Active and not Active Program the PHY Rate 1 and Rate 2
//		settings, also enable the reference clock buffer to the next downstream QLM
//		Write GSERC()_COMMON_PHY_CTRL_BCFG //Repeat for Slave QLMs
//			REFCLK_A_OE_R = 1’h1 //Enable reference clock A right side clock buffer to pass “clk_ref_a_r_o” to adjacent QLM
//			REFCLK_INPUT_SEL = 3’h0 if refclk from the “refclkp/m” pads, else 3’h2 if refclk is from the “clk_ref_a_r_i” inputs from adjacent QLM
//			PHY_CTRL_RATE1 = 6’h03 //Set PHY Rate 1 25.78125Gbps
//			PHY_CTRL_RATE2 = 6’h23 //Set PHY Rate 2 10.3125Gbps */
//	for (int module = 0; module < num_gserc; module++)
//	{
//		cavm_gsercx_common_phy_ctrl_bcfg_t bcfg = qlm_gserc_get_clock_mode(module);
//		GSER_CSR_WRITE(CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(module), bcfg.u);
//
//		int num_lanes = get_num_lanes(module);
//		for (int lane = 0; lane < num_lanes; lane++)
//		{
//			cavm_gsercx_lanex_control_bcfg_t bcfg = qlm_gserc_get_lane_mode(module, lane);
//			GSER_CSR_WRITE(CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane), bcfg.u);
//		}
//	}
//
//	/* Wait 10us for the above configuration to propagate through the SERDES block */
//	gser_wait_usec(10);
//
//	/* For most chips each serdes needs to be brought up in sequential order.
//	   CN96XX requires reverse order */
//	int module = 0;
//	int module_last = num_gserc - 1;
//	int module_inc = 1;
//
//	/* Steps 4-6 are done sequentially across all GSERC blocks */
//	while (1)
//	{
//		/* 4) Take the PHY out of reset and release the APB reset
//			Write GSERC()_COMMON_PHY_CTRL_BCFG
//			POR = 1’h0 //Release PHY Power-on Reset (POR)
//			APB_RESET = 1’h0 //Release APB reset */
//		GSER_TRACE(QLM, "GSERC%d: Release POR and APB reset\n", module);
//		GSER_CSR_MODIFY(c, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(module),
//			c.s.por = 0;
//			c.s.apb_reset = 0);
//		/* The emulator requires enabling the AFE shell */
//		if (gser_is_platform(GSER_PLATFORM_EMULATOR))
//		{
//			/* Write GSERC(0..2) _PHY0_TOP_SIM_CTRL
//					SIM_1B_MODEL = 1
//					FAST_SIM = 1 */
//			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_SIM_CTRL(module),
//				c.s.sim_1b_model = 1;
//				c.s.fast_sim = 1);
//		}
//
//		/* Program the temperature into the GSERC */
//		GSER_CSR_INIT(temp_result, CAVM_TSNX_TS_TEMP_CONV_RESULT(sensor));
//		while (!temp_result.s.temp_valid && !gser_is_platform(GSER_PLATFORM_ASIM))
//			temp_result.u = GSER_CSR_READ(CAVM_TSNX_TS_TEMP_CONV_RESULT(sensor));
//		int temp = gser_extracts(temp_result.s.temp_corrected, 2, 8);
//		temp += 40;
//		if (temp < 0)
//			temp = 0;
//		if (temp > 125 + 40)
//			temp = 125 + 40;
//		/* We program the temperature regrdless of GSER_GSERC_ENABLE_DOSC_TEMP_SKEW.
//		   This allows checking that the correct value is programmed without
//		   performing DOSC_TEMP_SKEW */
//		GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_DOSC_TEMP_DURING_CAL(module),
//			c.s.temp_value = temp);
//		GSER_CSR_MODIFY(c, CAVM_GSERCX_CM0_FEATURE_PLL_CAL_EN_CFG1_RSVD(module),
//			c.s.vco_dosc_temp_skew = GSER_GSERC_ENABLE_DOSC_TEMP_SKEW);
//
//		/* Program AN structures */
//		int num_lanes = get_num_lanes(module);
//		for (int lane = 0; lane < num_lanes; lane++)
//			qlm_gserc_config_an(module, lane);
//
//		GSER_TRACE(QLM, "GSERC: Checking for custom tuning\n");
//		apply_tuning(module);
//
//		/* 5) Take the CPU out of reset so it will boot and run the PHY firmware
//			Write GSERC()_COMMON_PHY_CTRL_BCFG
//			CPU_RESET = 1’h0 //Release CPU reset */
//		GSER_TRACE(QLM, "GSERC%d: Release CPU reset\n", module);
//		GSER_CSR_MODIFY(c, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(module),
//			c.s.cpu_reset = 0);
//
//		/* 6) Poll for “CM0_STATE_CHNG_RDY=1” This signal indicates that the CMU
//			has transitioned to the RESET state
//			// transition into the RESET power state.
//			Poll GSERC()_COMMON_PHY_STATUS_BSTS
//				CM0_STATE_CHNG_RDY = 1'b1
//				PHY_ERROR = 1'b1 //PHY_ERROR (signal err_o) should always be 1'b0 unless there is a startup problem with the PHY
//								// If PHY_ERROR=1'b1 then throw an Error Message else continue to poll for CM0_STATE_CHNG_RDY=1. */
//		if (!gser_is_platform(GSER_PLATFORM_ASIM))
//		{
//			/* This wait may be required to avoid false sampling of
//			   cm0_state_chng_rdy (its a negative pulse according to ScottM) */
//			gser_wait_usec(10);
//			GSER_TRACE(QLM, "GSERC%d: Waiting for cm0_state_chng_rdy=1\n", module);
//			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_COMMON_PHY_STATUS_BSTS(module), GSERCX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 1, 10000))
//				gser_error("GSERC%d: Timeout waiting for GSERCX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=1 (after cpu_reset=0)\n", module);
//			qlm_gserc_poll_error(module);
//		}
//		if (module == module_last)
//			break;
//		module += module_inc;
//	}
//
//	/* Steps 7-9 are done sequentially across all GSERC blocks
//		For 156.25MHz Reference Clock bring-up and proper refclk propagation between GSERC block:
//		CN96XX C0 & CNF95XX B0 start with GSERC0 first, GSERC1 second, GSERC2 last
//			GSERC0 receives the 156.25MHz refclk from the BGA bumps so start CMU first,
//			GSERC1 gets refclk from GSERC0 and forwards to GSERC2, so start CMU second,
//			GSERC2 gets refclk from GSERC1 so start CMU last. */
//	module = 0;
//	module_last = num_gserc - 1;
//	module_inc = 1;
//
//	while (1)
//	{
//		/* the DOSC threshold must be written before CMU_RESET is deasserted.
//		   Adjust the DOSC skew threshold. The default of 10% is way too large */
//		GSER_CSR_MODIFY(c, CAVM_GSERCX_CM0_FEATURE_SPARE_CFG5_RSVD(module),
//			c.s.data = GSER_GSERC_DOSC_SKEW_THRESHOLD);
//		/* This wait may be required to avoid racing with CMU_reset */
//		gser_wait_usec(50);
//
//		/* 7) Take the GSERC PHY clock management unit (CM0) out of reset.
//			Write GSERC(0..2)_COMMON_PHY_CTRL_BCFG
//			CM0_RST=0 - Release CMU from reset */
//		GSER_TRACE(QLM, "GSERC%d: Release CMU reset\n", module);
//		GSER_CSR_MODIFY(c, CAVM_GSERCX_COMMON_PHY_CTRL_BCFG(module),
//			c.s.cm0_rst = 0);
//
//		/* 8) Poll for “CM0_STATE_CHNG_RDY=0” These signals indicate that the CMU
//			is transitioning between states
//			// transition into the RESET power state.
//			Poll GSERC()_COMMON_PHY_STATUS_BSTS
//				CM0_STATE_CHNG_RDY = 1’b0
//				PHY_ERROR = 1’b0 //PHY_ERROR (signal err_o) should always be 1'b0 unless there is a startup problem with the PHY
//								// If PHY_ERROR=1'b1 then throw an Error Message, else continue to poll for CM0_STATE_CHNG_RDY=0. */
//		if (!gser_is_platform(GSER_PLATFORM_ASIM))
//		{
//			GSER_TRACE(QLM, "GSERC%d: Waiting for cm0_state_chng_rdy=0\n", module);
//			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_COMMON_PHY_STATUS_BSTS(module), GSERCX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 0, 500))
//				GSER_TRACE(QLM, "GSERC%d: Timeout waiting for GSERCX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=0\n", module);
//			qlm_gserc_poll_error(module);
//		}
//
//		/* 9) Poll for “CM0_STATE_CHNG_RDY=1” and “CM0_OK=1” These signals indicate
//			that the CMU is in the ACTIVE state
//			// transition into the RESET power state.
//			Poll GSERC()_COMMON_PHY_STATUS_BSTS
//				CM0_STATE_CHNG_RDY = 1’b1
//				CM0_OK = 1’b1
//				PHY_ERROR = 1’b0 //PHY_ERROR (signal err_o) should always be 1'b0 unless there is a startup problem with the PHY
//								// If PHY_ERROR=1'b1 then throw an Error Message, else continue to poll for CM0_STATE_CHNG_RDY=1. */
//		if (!gser_is_platform(GSER_PLATFORM_ASIM))
//		{
//			GSER_TRACE(QLM, "GSERC%d: Waiting for cm0_ok=1, cm0_state_chng_rdy=1\n", module);
//			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_COMMON_PHY_STATUS_BSTS(module), GSERCX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY, ==, 1, 50000))
//				gser_error("GSERC%d: Timeout waiting for GSERCX_COMMON_PHY_STATUS_BSTS[cm0_state_chng_rdy]=1 (after cm0_rst=0)\n", module);
//			if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_COMMON_PHY_STATUS_BSTS(module), GSERCX_COMMON_PHY_STATUS_BSTS_CM0_OK, ==, 1, 10))
//				gser_error("GSERC%d: Timeout waiting for GSERCX_COMMON_PHY_STATUS_BSTS[cm0_ok]=1\n", module);
//			qlm_gserc_poll_error(module);
//		}
//
//		/* Reference clock is now measurable */
//		int hz = qlm_gserc_measure_refclock(module);
//		GSER_TRACE(QLM, "GSERC%d: Reference clock at %dHz\n", module, hz);
//
//		if (module == module_last)
//			break;
//		module += module_inc;
//	}
//
//	/* 10) Poll for all lanes “Ready” to indicate the LANE is in the “RESET” state
//		For i=0 to 3, i++
//			Poll GSERC()_LANE(i)_STATUS_BSTS
//				LN_STATE_CHNG_RDY = 1'b1 //Lane is in RESET power state
//		next i;
//		// Use a watchdog timer on “LN_STATE_CHNG_RDY” in case this signal never goes ready. */
//	if (!gser_is_platform(GSER_PLATFORM_ASIM))
//	{
//		for (int module = 0; module < num_gserc; module++)
//		{
//			int num_lanes = get_num_lanes(module);
//			for (int lane = 0; lane < num_lanes; lane++)
//			{
//				GSER_CSR_INIT(bcfg, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane));
//				if (bcfg.s.ln_an_cfg == 0)
//				{
//					GSER_TRACE(QLM, "GSERC%d.%d: Waiting for ln_state_chng_rdy=1\n", module, lane);
//					if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 5000))
//						gser_error("GSERC%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1\n", module);
//				}
//				qlm_state_lane_t state = qlm_gserc_get_state(module, lane);
//				bool ena_8b10b = (state.s.baud_mhz <= 6250) && (state.s.baud_mhz != 2458) && (state.s.baud_mhz != 3072);
//				GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_FEATURE_ADAPT_CFG0(module, lane),
//					c.s.ena_8b10b = ena_8b10b);
//				/* Program TX polarity. RX polarity is programmed in the call to
//				   qlm_gserc_get_lane_mode() */
//				GSER_CSR_MODIFY(c, CAVM_GSERCX_LNX_TOP_DPL_TXDP_CTRL1(module, lane),
//					c.s.txpolarity = gser_config_get_int(GSER_CONFIG_QLM_LANE_TX_POLARITY,
//						map_module_to_qlm(module), lane));
//			}
//			/* Enable the BIST/PRBS clocks before starting each lane */
//			GSER_TRACE(QLM, "GSERC%d: Enabling BIST/PRBS logic\n", module);
//			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN0_CG_CTRL(module),
//				c.s.bist_rx = 1;
//				c.s.bist_tx = 1);
//			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN1_CG_CTRL(module),
//				c.s.bist_rx = 1;
//				c.s.bist_tx = 1);
//#if QUAD_LANE
//			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN2_CG_CTRL(module),
//				c.s.bist_rx = 1;
//				c.s.bist_tx = 1);
//			GSER_CSR_MODIFY(c, CAVM_GSERCX_PHY0_TOP_CLOCK_LN3_CG_CTRL(module),
//				c.s.bist_rx = 1;
//				c.s.bist_tx = 1);
//#endif
//		}
//	}
//	/* 11) Take the lanes out of reset
//		For i=0 to 3, i++
//			Write GSERC()_LANE(i)_CONTROL_BCFG
//				LN_RST_N = 1'b1
//		next i */
//	for (int module = 0; module < num_gserc; module++)
//	{
//		GSER_TRACE(QLM, "GSERC%d: Taking lanes out of reset\n", module);
//		int num_lanes = get_num_lanes(module);
//		for (int lane = 0; lane < num_lanes; lane++)
//		{
//			if (gser_is_platform(GSER_PLATFORM_EMULATOR))
//			{
//				GSER_TRACE(QLM, "GSERC%d.%d: Disable AFE calibration for emulator\n", module, lane);
//				GSER_CSR_WRITE(CAVM_GSERCX_LNX_FEATURE_TX_CAL_CFG0_RSVD(module, lane), 0);
//				GSER_CSR_WRITE(CAVM_GSERCX_LNX_FEATURE_REXT_CAL_RESULT_CFG0_RSVD(module, lane), 0);
//				GSER_CSR_WRITE(CAVM_GSERCX_LNX_FEATURE_LEQ_OFFSET_CAL_CFG0_RSVD(module, lane), 0);
//				GSER_CSR_WRITE(CAVM_GSERCX_LNX_FEATURE_DFE_OFFSET_CAL_CFG0_RSVD(module, lane), 0);
//				GSER_CSR_WRITE(CAVM_GSERCX_LNX_FEATURE_CDR_CAL_CFG0(module, lane), 0);
//				GSER_CSR_WRITE(CAVM_GSERCX_LNX_FEATURE_DFE_OFFSET_CAL_CFG1_RSVD(module, lane), 0);
//			}
//			GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
//				c.s.ln_rst = 0);
//		}
//	}
//	/* 12) Poll for “LN_STATE_CHNG_RDY = 1” & “LN_TX_RDY=1” & “LN_RX_RDY=1” to
//		indicate the Lane is in the “Active” power state
//		For i=0 to 3, i++
//			Poll GSERC()_LANE(i)_STATUS_BSTS
//				LN_TX_RDY=1’b1 //Lane Tx is up
//				LN_RX_RDY=1’b1 //Lane Rx is up
//				LN_STATE_CHNG_RDY = 1'b1 //Lane is in RESET power state
//		next i */
//	if (!gser_is_platform(GSER_PLATFORM_ASIM))
//	{
//		for (int module = 0; module < num_gserc; module++)
//		{
//			int num_lanes = get_num_lanes(module);
//			for (int lane = 0; lane < num_lanes; lane++)
//			{
//				GSER_CSR_INIT(bcfg, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane));
//				if (bcfg.s.ln_an_cfg == 0)
//				{
//					GSER_TRACE(QLM, "GSERC%d.%d: Waiting for ln_tx_rdy=1\n", module, lane);
//					if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_TX_RDY, ==, 1, 5000))
//						gser_error("GSERC%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_tx_rdy]=1\n", module);
//					if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_RX_RDY, ==, 1, 500))
//						gser_error("GSERC%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_rx_rdy]=1\n", module);
//					if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERCX_LANEX_STATUS_BSTS(module, lane), GSERCX_STATUS_BSTS_LN_STATE_CHNG_RDY, ==, 1, 500))
//						gser_error("GSERC%d: Timeout waiting for GSERCX_LANEX_STATUS_BSTS[ln_state_chng_rdy]=1\n", module);
//				}
//			}
//		}
//	}
//	/* 13) Once Lane is Tx and Rx is Ready set the CFG_CGX=1 to enable the Tx/Rx
//		FIFOs between CGX and PHY.
//		Write in reverse order, Lane 3 down to Lane 0
//		For i=3 to 0, i--
//			Write GSERC()_LANE(i)_CONTROL_BCFG
//				CFG_CGX = 1 //Enable Tx and Rx FIFOs to CGX
//		next i */
//	for (int module = num_gserc - 1; module >= 0; module--)
//	{
//		GSER_TRACE(QLM, "GSERC%d: Enable CGX connection\n", module);
//		int num_lanes = get_num_lanes(module);
//		for (int lane = 0; lane < num_lanes; lane++)
//		{
//			bool is_cpri = gserc_is_cpri(module, lane);
//			GSER_CSR_MODIFY(c, CAVM_GSERCX_LANEX_CONTROL_BCFG(module, lane),
//				c.s.cfg_cpri = is_cpri;
//				c.s.cfg_cgx = !is_cpri);
//			lane_state[module][lane] = LANE_STATE_TX_OFF;
//			show_lane_state(module, lane);
//		}
//	}
//	/* 14) The CGX MAC transmitter can be started at this point */
//	/* Done later */
//
//	GSER_TRACE(QLM, "GSERC: End of init\n");
//}
