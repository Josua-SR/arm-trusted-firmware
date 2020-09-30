/*
 * This file was copied from BDK using gsern-bdk-to-atf.sh script.
 * Do not edit it manually!
 */

/***********************license start***********************************
* Copyright (C) 2018-2020 Marvell International Ltd.
* SPDX-License-Identifier: BSD-3-Clause
* https://spdx.org/licenses
***********************license end**************************************/
#include <gsern/gsern.h>
#include <gser_internal.h>
#include <cgx.h>

#ifdef IMAGE_BL31
#include <bakery_lock.h>
DEFINE_BAKERY_LOCK(gsern_eye_lock);
#endif /* IMAGE_BL31 */

/* GSERN requires a short hold time between most changes to time or
   voltage. Theser are defined as 1.6us, but the BDK uses integers,
   so round up to 2us */
#define SETTLE_TIME 2

struct eye_capture_state{
	int ongoing;
	int qlm;
	int lane;
	int v_height;
	int delta_eye;
	uint64_t cycles;
	int t;
	cavm_gsernx_lanex_cdrfsm_bcfg_t cdrfsm_bcfg_original;
};

/**
 * Move the eye sample location. The hardware may lose lock if we move
 * the sampler in large steps. This code reads the current location and
 * moves the sampler one step at a time.
 *
 * @param node   Node to access
 * @param qlm	QLM to access
 * @param lane   Lane to access
 * @param t	  Location along the X axis (time) (0-63)
 * @param v	  Location along Y axis (voltage) (0-31)
 */
static void eye_move_location(int qlm, int lane, int t, int v)
{
	/* Move along the X axis (time) */
	GSER_CSR_INIT(cdrfsm_bcfg, CAVM_GSERNX_LANEX_CDRFSM_BCFG(qlm, lane));
	while (cdrfsm_bcfg.s.eoffs != t)
	{
		if (cdrfsm_bcfg.s.eoffs > t)
			cdrfsm_bcfg.s.eoffs--;
		else
			cdrfsm_bcfg.s.eoffs++;
		GSER_CSR_WRITE(CAVM_GSERNX_LANEX_CDRFSM_BCFG(qlm, lane), cdrfsm_bcfg.u);
		gser_wait_usec(SETTLE_TIME); /* 1.6 us */
		GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_RX_QAC_BCFG(qlm, lane),
			c.s.cdr_qac_sele = 0);
		gser_wait_usec(SETTLE_TIME); /* 1.6 us */
	}

	/* Move along the Y axis (voltage) */
	GSER_CSR_INIT(rx_os_5_bcfg, CAVM_GSERNX_LANEX_RX_OS_5_BCFG(qlm, lane));
	while (rx_os_5_bcfg.s.c1_e_adjust != v)
	{
		if (rx_os_5_bcfg.s.c1_e_adjust > v)
			rx_os_5_bcfg.s.c1_e_adjust--;
		else
			rx_os_5_bcfg.s.c1_e_adjust++;
		GSER_CSR_WRITE(CAVM_GSERNX_LANEX_RX_OS_5_BCFG(qlm, lane), rx_os_5_bcfg.u);
		gser_wait_usec(SETTLE_TIME); /* 1.6 us */
	}
}

/**
 * Measure the number of errors over a number of GSERN cycles
 *
 * @param node   Node to measure
 * @param qlm	DLM/QLM to measure
 * @param lane   Lane in QLM/DLM
 * @param cycles Cycles to measure over
 *
 * @return Number of errors, max uint64 if failure
 */
static uint64_t eye_measure_errors(int qlm, int lane, uint64_t cycles)
{
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_EYE_CTL(qlm, lane),
		c.s.rst_n = 0);
	/* Load Cycle Count */
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_EYE_CTL(qlm, lane),
		c.s.cycle_cnt_en = 1;
		c.s.cycle_cnt = cycles);
	/* Start Eye Measurement */
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_EYE_CTL(qlm, lane),
		c.s.eye_en = 1);
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_EYE_CTL(qlm, lane),
		c.s.rst_n = 1);
	/* Wait for eye measurement done */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERNX_LANEX_EYE_DAT(qlm, lane), GSERN_LANE_EYE_DAT_CYCLE_CNT_DONE, ==, 1, 10000000))
	{
		gser_warn("Timeout waiting for cycle to complete, aborting eye\n");
		return UINT64_MAX;
	}
	/* Stop the eye */
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_EYE_CTL(qlm, lane),
		c.s.eye_en = 0);
	/* Read Error count */
	GSER_CSR_INIT(eye_dat, CAVM_GSERNX_LANEX_EYE_DAT(qlm, lane));
	if (eye_dat.s.err_cnt_ovf || (eye_dat.s.err_cnt > UINT32_MAX))
		return UINT32_MAX;
	else
		return eye_dat.s.err_cnt;
}

static int gsern_eye_capture_setup(struct eye_capture_state *state,
				   int show_data, gser_qlm_eye_t *eye_data)
{
	const int TIMEOUT = 10000; /* 10ms */
	int qlm = state->qlm;
	int lane = state->lane;

	GSER_CSR_INIT(cdrfsm_bcfg_original, CAVM_GSERNX_LANEX_CDRFSM_BCFG(qlm, lane));
	state->cdrfsm_bcfg_original = cdrfsm_bcfg_original;

	/* Disable DFE Periodic Offset update */
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_RX_24_BCFG(qlm, lane),
		c.s.dfe_oscomp_timer_en = 0);

	/* Power-up the eye monitor */
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_RST2_BCFG(qlm, lane),
		c.s.rst_eye_rst_sm = 0);
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_RST2_BCFG(qlm, lane),
		c.s.ln_reset_use_eye = 1);

	/* Poll for eye monitor ready */
	if (GSER_CSR_WAIT_FOR_FIELD(CAVM_GSERNX_LANEX_INIT_BSTS(qlm, lane), GSERN_LANE_INIT_BSTS_EYE_READY, ==, 1, TIMEOUT))
	{
		gser_error("N0.QLM%d.Lane%d: Eye monitor not ready\n", qlm, lane);
		return -1;
	}

	/* Calculate the number of cycles to sample over */
	const int BER = 6;
	state->cycles = 1;
	for (int b = 0; b < BER; b++) /* Loop avoids math library for power */
		state->cycles *= 10;

	state->v_height = 32;
	eye_data->width = 0x3b;
	eye_data->height = state->v_height * 2 - 1;

	/* Determine if we're in 20 bit or 40 bit mode */
	GSER_CSR_INIT(bcfg, CAVM_GSERNX_LANEX_PCS_802P3_BCFG(qlm, lane));
	int bus_width = bcfg.s.rx_wpk_20b40b ? 20 : 40;

	if (show_data)
	{
		printf("# Extensive 1D Rx Eye on GSER%d Lane %d\n", qlm, lane);
		printf("# Start Time Offset: 0; End Time Offset: %d\n", eye_data->width - 1);
		printf("# eyemon %d %d %d %d %d %d %d %d %d %d %s %s\n",
			qlm, lane,
			0 /* vmin */, eye_data->height-1 /* vmax */, 1 /* vstep */,
			0 /* tmin */, eye_data->width-1 /* tmax */, 1 /* tstep */,
			BER /* BER */, 0 /* OffsetOverrideEn */, "DOUTQ" /* PTYPE */, "NO_SHIFT" /* SHIFT */);
		printf("# cycle count = %llu, bus width = %d\n", state->cycles, bus_width);
		printf("V  T  %-20s %-20s %-20s %-20s\n", "TRANS_ONE_ECNT",
			"NON_TRANS_ONE_ECNT", "TRANS_ZEROS_ECNT", "NON_TRANS_ZEROS_ECNT");
	}
	else
		printf("Capturing eye...\n");

	/* Initialize eyeEoffs from eyeQacEoffs */
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_RX_QAC_BCFG(qlm, lane),
		c.s.en_qac_e = 0);
	gser_wait_usec(SETTLE_TIME); /* 1.6 us */
	GSER_CSR_INIT(rx_qac_bsts, CAVM_GSERNX_LANEX_RX_QAC_BSTS(qlm, lane));
	int eye_eoffs = rx_qac_bsts.s.qac_eoffs;
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_CDRFSM_BCFG(qlm, lane),
		c.s.eoffs = eye_eoffs);
	state->delta_eye = eye_eoffs - 0x20;
	if (state->delta_eye < 0)
		state->delta_eye = 0;

	return 0;
}

static int gsern_eye_capture_line(struct eye_capture_state *state,
				  int show_data, gser_qlm_eye_t *eye_data)
{
	int t = state->t;
	int qlm = state->qlm;
	int lane = state->lane;
	int cycles = state->cycles;
	int v_height = state->v_height;

	for (int v = 0; v < v_height; v++)
	{
		eye_move_location(qlm, lane, t + state->delta_eye, v);

		/* Configure Eye Monitor to capture Non-Transitions Ones */
		GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_EYE_CTL_2(qlm, lane),
				c.s.capture_ones_en = 1;
				c.s.capture_ones = 1;
				c.s.capture_edgemode = 0;
				c.s.capture_trans = 0;
				c.s.eye_adapt_en = 1);
		gser_wait_usec(SETTLE_TIME); /* 1.6 us */
		uint64_t errors_nt_ones = eye_measure_errors(qlm, lane, cycles);
		if (errors_nt_ones == UINT64_MAX)
			return -1;
		/* Configure Eye Monitor to Capture Transitions Ones */
		GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_EYE_CTL_2(qlm, lane),
				c.s.capture_ones_en = 1;
				c.s.capture_ones = 1;
				c.s.capture_edgemode = 1;
				c.s.capture_trans = 1;
				c.s.eye_adapt_en = 1);
		gser_wait_usec(SETTLE_TIME); /* 1.6 us */
		uint64_t errors_tr_ones = eye_measure_errors(qlm, lane, cycles);
		if (errors_tr_ones == UINT64_MAX)
			return -1;
		/* Configure Eye Monitor to capture Non-Transitions Zeros */
		GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_EYE_CTL_2(qlm, lane),
				c.s.capture_ones_en = 0;
				c.s.capture_ones = 0;
				c.s.capture_edgemode = 0;
				c.s.capture_trans = 0;
				c.s.eye_adapt_en = 1);
		gser_wait_usec(SETTLE_TIME); /* 1.6 us */
		uint64_t errors_nt_zeros = eye_measure_errors(qlm, lane, cycles);
		if (errors_nt_zeros == UINT64_MAX)
			return -1;
		/* Configure Eye Monitor to capture Transitions Zeros */
		GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_EYE_CTL_2(qlm, lane),
				c.s.capture_ones_en = 0;
				c.s.capture_ones = 0;
				c.s.capture_edgemode = 1;
				c.s.capture_trans = 1;
				c.s.eye_adapt_en = 1);
		gser_wait_usec(SETTLE_TIME); /* 1.6 us */
		uint64_t errors_tr_zeros = eye_measure_errors(qlm, lane, cycles);
		if (errors_tr_zeros == UINT64_MAX)
			return -1;

		if (show_data)
		{
			printf("%02x %02x %020llx %020llx %020llx %020llx\n",
				v, t, errors_tr_ones, errors_nt_ones,
				errors_tr_zeros, errors_nt_zeros);
		}
		eye_data->data[v_height - v - 1][t] = errors_nt_ones;
		eye_data->data[v_height - v - 1][t + 64] = errors_tr_ones;
		eye_data->data[v_height + v - 1][t] = errors_nt_zeros;
		eye_data->data[v_height + v - 1][t + 64] = errors_tr_zeros;
	}

	return CGX_DISPLAY_OK;
}

void gsern_eye_capture_stop(struct eye_capture_state *state)
{
	int qlm = state->qlm;
	int lane = state->lane;

	/* Power-down the eye monitor */
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_RST2_BCFG(qlm, lane),
		c.s.ln_reset_use_eye = 0);

	/* Re-Enable DFE Periodic Offset update */
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_RX_24_BCFG(qlm, lane),
		c.s.dfe_oscomp_timer_en = 1);

	/* Reset the eye monitor  */
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_CDRFSM_BCFG(qlm, lane),
		c.s.eoffs = state->cdrfsm_bcfg_original.s.eoffs);
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_RX_QAC_BCFG(qlm, lane),
		c.s.en_qac_e = 1;
		c.s.en_qac_q = 1;
		c.s.cdr_qac_sele = 1);
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_EYE_CTL_2(qlm, lane),
		c.s.capture_edgemode = 0;
		c.s.capture_trans = 0;
		c.s.eye_adapt_en = 0);
	GSER_CSR_MODIFY(c, CAVM_GSERNX_LANEX_EYE_CTL(qlm, lane),
		c.s.cycle_cnt_en = 0;
		c.s.rst_n = 0);

	state->ongoing = 0;
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
int gsern_eye_capture(int qlm, int lane, int show_data, gser_qlm_eye_t *eye_data)
{
	int ec = 0;

	static struct eye_capture_state state = { .ongoing = 0 };

#ifdef IMAGE_BL31
	bakery_lock_get(&gsern_eye_lock);
#endif /* IMAGE_BL31 */
	if (!(state.qlm == qlm && state.lane == lane) && state.ongoing == 1)
		gsern_eye_capture_stop(&state);

	if (!state.ongoing) {
		state.qlm = qlm;
		state.lane = lane;
		ec = gsern_eye_capture_setup(&state, show_data, eye_data);
		if (ec) {
#ifdef IMAGE_BL31
			bakery_lock_release(&gsern_eye_lock);
#endif /* IMAGE_BL31 */
			return ec;
		}

		state.t = 0;
		state.ongoing = 1;
	}

	/* Scan the eye monitor across the area */
	while (state.t < eye_data->width) {
		ec = gsern_eye_capture_line(&state, show_data, eye_data);
		if (ec)
			break;

		state.t++;
		/*
		 * Operation didn't end yet. If not showing data give
		 * chance to execute for non-secure world.
		 */
		if (!show_data) {
#ifdef IMAGE_BL31
			bakery_lock_release(&gsern_eye_lock);
#endif /* IMAGE_BL31 */
			return CGX_DISPLAY_PENDING;
		}
	}

	gsern_eye_capture_stop(&state);
#ifdef IMAGE_BL31
	bakery_lock_release(&gsern_eye_lock);
#endif /* IMAGE_BL31 */
	return ec;
}
