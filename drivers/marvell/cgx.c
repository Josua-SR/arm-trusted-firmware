/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

/* CGX driver for OcteonTX2 */

#include <arch.h>
#include <stdio.h>
#include <string.h>
#include <debug.h>
#include <drivers/delay_timer.h>
#include <platform_def.h>
#include <platform_dt.h>
#include <octeontx_common.h>
#include <plat_board_cfg.h>
#include <cgx.h>
#include <cgx_intf.h>
#include <qlm.h>
#include <octeontx_utils.h>
#include <gsern/gsern_internal.h>

/* define DEBUG_ATF_CGX to enable debug logs */
#undef DEBUG_ATF_CGX

#ifdef DEBUG_ATF_CGX
#define debug_cgx printf
#else
#define debug_cgx(...) ((void) (0))
#endif

/* table to map speed in Mbps with cgx_link_speed enum */
static int cgx_link_speed_mbps[CGX_LINK_MAX] = {
		0, 10, 100, 1000, 2500, 5000, 10000, 20000, 25000,
		40000, 50000, 80000, 100000 };

/* table to map USXGMII sub types with baud rate and number of LMACs */
static usxgmii_type_t usxgmii_type[MAX_USXGMII_TYPES] = {
	{2578, 1, CAVM_CGX_USXGMII_TYPE_E_SXGMII_2G, CAVM_CGX_USXGMII_RATE_E_RATE_2HG},
	{5156, 1, CAVM_CGX_USXGMII_TYPE_E_SXGMII_5G, CAVM_CGX_USXGMII_RATE_E_RATE_5G},
	{5156, 2, CAVM_CGX_USXGMII_TYPE_E_DXGMII_5G, CAVM_CGX_USXGMII_RATE_E_RATE_2HG},
	{10312, 1, CAVM_CGX_USXGMII_TYPE_E_SXGMII_10G, CAVM_CGX_USXGMII_RATE_E_RATE_10G},
	{10312, 2, CAVM_CGX_USXGMII_TYPE_E_DXGMII_10G, CAVM_CGX_USXGMII_RATE_E_RATE_5G},
	{10312, 4, CAVM_CGX_USXGMII_TYPE_E_QXGMII_10G, CAVM_CGX_USXGMII_RATE_E_RATE_2HG},
	{20625, 2, CAVM_CGX_USXGMII_TYPE_E_DXGMII_20G, CAVM_CGX_USXGMII_RATE_E_RATE_10G},
	{20625, 4, CAVM_CGX_USXGMII_TYPE_E_QXGMII_20G, CAVM_CGX_USXGMII_RATE_E_RATE_5G},
	};

/* table to map the LINK_SPEED with USXGMII rate */
static usxgmii_rate_map_t usxgmii_rate_map[MAX_USXGMII_RATE_TYPES] = {
	{CGX_LINK_10M, CAVM_CGX_USXGMII_RATE_E_RATE_10M, 10},
	{CGX_LINK_100M, CAVM_CGX_USXGMII_RATE_E_RATE_100M, 100},
	{CGX_LINK_1G, CAVM_CGX_USXGMII_RATE_E_RATE_1G, 1000},
	{CGX_LINK_2HG, CAVM_CGX_USXGMII_RATE_E_RATE_2HG, 2500},
	{CGX_LINK_5G, CAVM_CGX_USXGMII_RATE_E_RATE_5G, 5000},
	{CGX_LINK_10G, CAVM_CGX_USXGMII_RATE_E_RATE_10G, 10000},
	{CGX_LINK_20G, CAVM_CGX_USXGMII_RATE_E_RATE_20G, 20000},
	{CGX_LINK_NONE, CAVM_CGX_USXGMII_RATE_E_RSV_RATE, 0}
};

static int cgx_poll_for_csr(uint64_t addr, uint64_t mask,
					int poll_val, int timeout)
{
	int tries;
	uint64_t val;

	/* Default poll for CGX status registers is 1 ms. If any
	 * specific case like AN, training needs different
	 * timeout, it can be specified via the API. If not, -1
	 * needs to be passed as a argument.
	 */
	if (timeout != -1)
		tries = timeout;
	else
		tries = 100;
	do {
		/* NOTE : directly read from addr instead of using
		 * CSR_READ in this case to avoid dependencies
		 */
		val = cavm_le64_to_cpu(*(volatile uint64_t *)addr);
		/* argument poll_val indicates if the bit should
		 * be polled for value of 1 or 0
		 */
		if ((poll_val && (val & mask)) || (!poll_val && !(val & mask)))
			break;
		udelay(CGX_POLL_FOR_CSR_DELAY);
	} while (--tries > 0);

	if (!tries)
		return -1;

	return 0;
}

/* This function initializes CGX in the SGMII/QSGMII modes
 * called one time either during boot or mode change
 */
static int cgx_sgmii_hw_init(int cgx_id, int lmac_id)
{
	cgx_lmac_config_t *lmac;
	cavm_cgxx_gmp_gmi_txx_append_t tx_append;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	debug_cgx("%s: cgx_id = %d, lmac_id = %d\n", __func__, cgx_id, lmac_id);

	/* set TX threshold */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_gmi_txx_thresh_t,
			CAVM_CGXX_GMP_GMI_TXX_THRESH(cgx_id, lmac_id),
			cnt, CGX_GMP_TX_FIFO_WORDS); /* 128-bit words FIFO */
	/* Max packet size */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_gmi_rxx_jabber_t,
			CAVM_CGXX_GMP_GMI_RXX_JABBER(cgx_id, lmac_id),
			cnt, CGX_MAX_FRAME_SIZE); /* Max Frame size */

	/* Disable frame alignment if using preamble */
	tx_append.u = CSR_READ(CAVM_CGXX_GMP_GMI_TXX_APPEND(
					cgx_id, lmac_id));
	CSR_WRITE(CAVM_CGXX_GMP_GMI_TXX_SGMII_CTL(cgx_id, lmac_id),
				!tx_append.s.preamble);
	tx_append.s.fcs = 1;
	tx_append.s.pad = 1;
	CSR_WRITE(CAVM_CGXX_GMP_GMI_TXX_APPEND(cgx_id,
					lmac_id), tx_append.u);

	/* Min packet size */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_gmi_txx_min_pkt_t,
			CAVM_CGXX_GMP_GMI_TXX_MIN_PKT(cgx_id, lmac_id),
			min_size, CGX_GMP_MIN_PACKET_SIZE);

	/* Disable disparity for QSGMII mode, to prevent
	 * propagation across ports
	 */
	if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII) {
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_pcs_miscx_ctl_t,
				CAVM_CGXX_GMP_PCS_MISCX_CTL(
				cgx_id, lmac_id), disp_en, 0);
	}

	return 0;
}

/* This function initializes CGX in the XFI/XAUI/RXAUI/XLAUI/10G_R/40G_R
 * modes called one time either during boot or mode change
 */
static int cgx_xaui_hw_init(int cgx_id, int lmac_id)
{
	int jabber;
	cgx_lmac_config_t *lmac;
	cavm_cgxx_smux_tx_append_t smux_tx_append;
	cavm_cgxx_cmrx_rx_bp_on_t rx_bp_on;
	cavm_cgxx_const_t cgx_const;
	cavm_cgxx_cmr_rx_lmacs_t cmr_rx_lmacs;
	cavm_cgxx_spux_rx_lpi_timing_t rx_lpi_timing;
	cavm_cgxx_spux_tx_lpi_timing_t tx_lpi_timing;

	debug_cgx("%s %d:%d\n", __func__, cgx_id, lmac_id);

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	/* Jabber : Max packet size + 8 bytes for preamble + 4 bytes for FCS
	 * and should be multiple of 16 bytes
	 */
	jabber = CGX_MAX_FRAME_SIZE + CGX_FCS_BYTES + CGX_PREAMBLE_BYTES;

	CAVM_MODIFY_CGX_CSR(cavm_cgxx_smux_rx_jabber_t,
		CAVM_CGXX_SMUX_RX_JABBER(cgx_id, lmac_id), cnt,
		((((jabber + CGX_JABBER_ALIGN - 1)/CGX_JABBER_ALIGN) * CGX_JABBER_ALIGN)));

	/* Tx threshold */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_smux_tx_thresh_t,
		CAVM_CGXX_SMUX_TX_THRESH(cgx_id, lmac_id),
		cnt, CGX_SMUX_TX_FIFO_WORDS);

	/* Disable forward error correction by default. will enable
	 * it later based on transceiver capability
	 */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_fec_control_t,
			CAVM_CGXX_SPUX_FEC_CONTROL(cgx_id, lmac_id),
			fec_en, 0);

	/* Set interleaved running disparity for RXAUI */
	if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_RXAUI) {
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_misc_control_t,
			CAVM_CGXX_SPUX_MISC_CONTROL(cgx_id, lmac_id),
			intlv_rdisp, 1);
	}
	/* Append FCS & pad to each packet */
	smux_tx_append.u = CSR_READ(CAVM_CGXX_SMUX_TX_APPEND(
						cgx_id, lmac_id));
	smux_tx_append.s.pad = 1;
	smux_tx_append.s.fcs_d = 1;
	CSR_WRITE(CAVM_CGXX_SMUX_TX_APPEND(cgx_id, lmac_id),
					smux_tx_append.u);
	/* Min packet size */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_smux_tx_min_pkt_t,
			CAVM_CGXX_SMUX_TX_MIN_PKT(cgx_id, lmac_id),
			min_size, CGX_SMUX_MIN_PACKET_SIZE);

	/* Disable EEE as there is no support in HW */
	rx_lpi_timing.u = CSR_READ(CAVM_CGXX_SPUX_RX_LPI_TIMING(cgx_id,
						lmac_id));
	rx_lpi_timing.s.rx_lpi_en = 0;
	rx_lpi_timing.s.rx_lpi_fw = 0;
	CSR_WRITE(CAVM_CGXX_SPUX_RX_LPI_TIMING(cgx_id, lmac_id),
				rx_lpi_timing.u);

	tx_lpi_timing.u = CSR_READ(CAVM_CGXX_SPUX_TX_LPI_TIMING(cgx_id,
						lmac_id));
	tx_lpi_timing.s.tx_lpi_en = 0;
	tx_lpi_timing.s.tx_lpi_fw = 0;
	CSR_WRITE(CAVM_CGXX_SPUX_TX_LPI_TIMING(cgx_id, lmac_id),
				tx_lpi_timing.u);

	/* Program receive backpressure as recommended by HRM
	 * The recommended value is 1/4th the size of the per-LMAC RX FIFO
	 * size as determined by CGX()_CMR_RX_LMACS[LMACS].
	 * Also, mark to be configured in mulitple of 16 bytes
	 */
	cgx_const.u = CSR_READ(CAVM_CGXX_CONST(cgx_id));
	cmr_rx_lmacs.u = CSR_READ(CAVM_CGXX_CMR_RX_LMACS(cgx_id));
	rx_bp_on.u = CSR_READ(CAVM_CGXX_CMRX_RX_BP_ON(cgx_id, lmac_id));
	rx_bp_on.s.mark = (cgx_const.s.rx_fifosz/(cmr_rx_lmacs.s.lmacs *
				CGX_BP_ON_MARK_SIZE_DIV * CGX_BP_PACKET_DATA_DEPTH));
	CSR_WRITE(CAVM_CGXX_CMRX_RX_BP_ON(cgx_id, lmac_id),
			rx_bp_on.u);
	return 0;
}

static void cgx_restart_an(int cgx_id, int lmac_id)
{
	cgx_lmac_config_t *lmac;

	debug_cgx("%s %d:%d\n", __func__, cgx_id, lmac_id);

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	if (lmac->autoneg_dis)
		return;

	CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_an_control_t,
			CAVM_CGXX_SPUX_AN_CONTROL(cgx_id, lmac_id),
			an_restart, 1);
}

static void cgx_restart_training(int cgx_id, int lmac_id)
{
	debug_cgx("%s %d:%d\n", __func__, cgx_id, lmac_id);

	/* clear the training interrupts by writing 1 - W1C CSR */
	CSR_WRITE(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id),
			CGX_SPUX_TRAINING_DONE_MASK);
	CSR_WRITE(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id),
			CGX_SPUX_TRAINING_FAIL_MASK);

	CSR_WRITE(CAVM_CGXX_SPUX_BR_PMD_LD_CUP(cgx_id, lmac_id), 0);
	CSR_WRITE(CAVM_CGXX_SPUX_BR_PMD_LP_CUP(cgx_id, lmac_id), 0);
	CSR_WRITE(CAVM_CGXX_SPUX_BR_PMD_LP_REP(cgx_id, lmac_id), 0);

	/* wait */
	udelay(CGX_SPUX_TRAINING_RESTART_DELAY);

	/* restart the training */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_br_pmd_control_t,
		CAVM_CGXX_SPUX_BR_PMD_CONTROL(cgx_id, lmac_id),
		train_restart, 1);
}

static int cgx_get_usxgmii_type(int cgx_id, int lmac_id)
{
	int type = -1;
	cgx_config_t *cgx;
	cgx_lmac_config_t *lmac;
	qlm_state_lane_t qlm_state;

	cgx = &plat_octeontx_bcfg->cgx_cfg[cgx_id];
	lmac = &cgx->lmac_cfg[lmac_id];

	debug_cgx("%s: cgx %d qlm %d lane %d mode %d\n", __func__, cgx_id,
					lmac->qlm, lmac->lane, lmac->mode);

	qlm_state = plat_otx2_get_qlm_state_lane(lmac->qlm, lmac->lane);
	/* get the USXGMII type based on baud rate
	 * and LMACs per CGX
	 */
	for (int i = 0; i < MAX_USXGMII_TYPES; i++) {
		if ((usxgmii_type[i].baud_rate == qlm_state.s.baud_mhz) &&
			(usxgmii_type[i].lmacs_used == cgx->lmacs_used)) {
			type = usxgmii_type[i].type;
			break;
		}
	}
	debug_cgx("%s: USXGMII sub type %d\n", __func__, type);

	if ((type < CAVM_CGX_USXGMII_TYPE_E_SXGMII_10G) &&
			(type > CAVM_CGX_USXGMII_TYPE_E_QXGMII_10G)) {
		/* FIXME: set to default 10G single port sub type */
		type = CAVM_CGX_USXGMII_TYPE_E_SXGMII_10G;
		debug_cgx("%s: invalid USXGMII sub type %d\n", __func__, type);
	}
	return type;
}

static int cgx_get_usxgmii_rate_from_link_speed(int link_speed)
{
	int rate = CAVM_CGX_USXGMII_RATE_E_RSV_RATE;

	for (int i = 0; i < MAX_USXGMII_RATE_TYPES; i++) {
		if (usxgmii_rate_map[i].speed == link_speed)
			rate = usxgmii_rate_map[i].rate;
	}
	debug_cgx("%s: speed %d, rate %d\n", __func__, link_speed, rate);
	return rate;
}

static int cgx_get_usxgmii_speed_mbps_from_rate(int rate)
{
	int speed = CGX_LINK_NONE;

	for (int i = 0; i < MAX_USXGMII_RATE_TYPES; i++) {
		if (usxgmii_rate_map[i].rate == rate)
			speed = usxgmii_rate_map[i].speed_mbps;
	}
	debug_cgx("%s: rate %d speed %d\n", __func__, rate, speed);
	return speed;
}

/* This function initializes the CGX LMAC for
 * a particular mode
 */
void cgx_lmac_init(int cgx_id, int lmac_id)
{
	cgx_lmac_config_t *lmac;
	cavm_cgxx_cmrx_config_t cmr_config;
	cavm_cgxx_spu_usxgmii_control_t usxgmii_ctrl;
	cavm_cgxx_cmr_chan_msk_and_t chan_msk_and;

	debug_cgx("%s %d:%d\n", __func__, cgx_id, lmac_id);

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	/* FDT parser should have updated the lmac mode by now.
	 * check if mode is programmed, if not, return
	 */
	if ((lmac->mode == -1) || (lmac->lane_to_sds == -1)) {
		debug_cgx("%s: %d:%d mode/lane_to_sds not programmed\n",
					__func__, cgx_id, lmac_id);
		return;
	}

	/* force on CGX clocks to ensure successful propagation of reset
	 * when disabling the link
	 */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmr_global_config_t,
		CAVM_CGXX_CMR_GLOBAL_CONFIG(cgx_id), cgx_clk_enable, 1);

	/* disable LMAC */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
		CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id), enable, 0);

	/* release the force of clocks */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmr_global_config_t,
		CAVM_CGXX_CMR_GLOBAL_CONFIG(cgx_id), cgx_clk_enable, 0);

	/* program CGX_CMRX_CONFIG register with
	 * lmac_type, lane_to_sds
	 */
	debug_cgx("%s mode %d lane_to_sds %d\n", __func__, lmac->mode,
					lmac->lane_to_sds);
	cmr_config.u = CSR_READ( CAVM_CGXX_CMRX_CONFIG(
				cgx_id, lmac_id));
	cmr_config.s.lmac_type = lmac->mode;
	cmr_config.s.lane_to_sds = lmac->lane_to_sds;
	CSR_WRITE(CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
			cmr_config.u);

	/* For USXGMII, lane_to_sds is
	 * ignored. Instead SPU_USXGMII_CONTROL[SDS_ID]
	 * should be programmed
	 */
	if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_USXGMII) {
		/* Program the lane and sub type */
		usxgmii_ctrl.u = CSR_READ(
					CAVM_CGXX_SPU_USXGMII_CONTROL(cgx_id));
		usxgmii_ctrl.s.sds_id = lmac->lane;
		usxgmii_ctrl.s.usxgmii_type = cgx_get_usxgmii_type(cgx_id, lmac_id);
		CSR_WRITE(CAVM_CGXX_SPU_USXGMII_CONTROL(cgx_id),
					usxgmii_ctrl.u);
	}
	/* do one time initialization of CGX based on
	 * the LMAC type. this function will be called
	 * once during boot and whenever mode change
	 * happens
	 */
	switch (lmac->mode) {
	case CAVM_CGX_LMAC_TYPES_E_SGMII:
	case CAVM_CGX_LMAC_TYPES_E_QSGMII:
		cgx_sgmii_hw_init(cgx_id, lmac_id);
		break;
	case CAVM_CGX_LMAC_TYPES_E_XAUI:
	case CAVM_CGX_LMAC_TYPES_E_RXAUI:
	case CAVM_CGX_LMAC_TYPES_E_TENG_R:
	case CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R:
	case CAVM_CGX_LMAC_TYPES_E_FORTYG_R:
	case CAVM_CGX_LMAC_TYPES_E_FIFTYG_R:
	case CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R:
	case CAVM_CGX_LMAC_TYPES_E_USXGMII:
		cgx_xaui_hw_init(cgx_id, lmac_id);
		break;
	default:
		debug_cgx("%s invalid mode %d\n", __func__, lmac->mode);
		break;
	}
	/* Set the channel back pressure mask */
	chan_msk_and.u = CSR_READ(CAVM_CGXX_CMR_CHAN_MSK_AND(cgx_id));
	CSR_WRITE(CAVM_CGXX_CMR_CHAN_MSK_AND(cgx_id),
			(chan_msk_and.s.msk_and |
			(((1ull << 1) - 1ull) << (lmac_id * MAX_CHAN_PER_LMAC))));
}

int cgx_get_lane_speed(int cgx_id, int lmac_id)
{
	int qlm, lane_id, lanes = 0, speed = 0;
	cgx_lmac_config_t *lmac;
	qlm_state_lane_t qlm_state;
	cavm_cgxx_spux_control1_t spux_ctrl1;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];
	qlm = lmac->qlm;
	lane_id = lmac->lane;

	debug_cgx("%s: cgx %d qlm %d lane %d mode %d\n", __func__, cgx_id,
					qlm, lane_id, lmac->mode);

	qlm_state = plat_otx2_get_qlm_state_lane(qlm, lane_id);

	/* Ref : Table 38-1 of T9X HRM for encoding, lanes
	 * baud rate based on LMAC type
	 */
	switch (lmac->mode) {
	case CAVM_CGX_LMAC_TYPES_E_SGMII:
	case CAVM_CGX_LMAC_TYPES_E_QSGMII:
		/* Using 8b10b symbol encoding */
		speed = (qlm_state.s.baud_mhz * 8 + 5) / 10;
		lanes = 1;
		break;
	case CAVM_CGX_LMAC_TYPES_E_XAUI:
		/* Using 8b10b symbol encoding */
		speed = (qlm_state.s.baud_mhz * 8 + 5) / 10;
		lanes = 4;
		break;
	case CAVM_CGX_LMAC_TYPES_E_RXAUI:
		/* Using 8b10b symbol encoding */
		speed = (qlm_state.s.baud_mhz * 8 + 5) / 10;
		lanes = 2;
		break;
	case CAVM_CGX_LMAC_TYPES_E_TENG_R:
	case CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R:
		/* Using 64b66b symbol encoding */
		speed = (qlm_state.s.baud_mhz * 64 + 33) / 66;
		lanes = 1;
		break;
	case CAVM_CGX_LMAC_TYPES_E_FIFTYG_R:
		if (lmac->mode_idx == QLM_MODE_25GAUI_2_C2C) {
			speed = 12500;
		} else {
			/* Using 64b66b symbol encoding */
			speed = (qlm_state.s.baud_mhz * 64 + 33) / 66;
		}
		lanes = 2;
		break;
	case CAVM_CGX_LMAC_TYPES_E_FORTYG_R:
	case CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R:
		if (lmac->mode_idx == QLM_MODE_50GAUI_4_C2C) {
			speed = 12500;
		} else {
			/* Using 64b66b symbol encoding */
			speed = (qlm_state.s.baud_mhz * 64 + 33) / 66;
		}
		lanes = 4;
		break;
	case CAVM_CGX_LMAC_TYPES_E_USXGMII:
		/* for USXGMII, get the rate and return the corresponding
		 * link speed.
		 * FIXME : This is as per the info that CGX will
		 * automatically autonegotiate if PHY's link speed will
		 * be different. If this is not the case, PHY's link
		 * speed needs to be read here
		 */
		spux_ctrl1.u = CSR_READ(CAVM_CGXX_SPUX_CONTROL1(
					cgx_id, lmac_id));
		speed = cgx_get_usxgmii_speed_mbps_from_rate(
					spux_ctrl1.s.usxgmii_rate);
		lanes = 1;
		break;
	default:
		break;
	};
	speed = speed * lanes;
	return speed;
}

int cgx_sgmii_check_an_cpt(int cgx_id, int lmac_id)
{
	cgx_lmac_config_t *lmac;

	debug_cgx("%s %d:%d\n", __func__, cgx_id, lmac_id);

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	/* Check if AN is complete for SGMII only */
	if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) &&
			(!lmac->autoneg_dis)) {
		if (cgx_poll_for_csr(CAVM_CGXX_GMP_PCS_MRX_STATUS(
			cgx_id, lmac_id), CGX_GMP_PCS_AN_CPT_MASK, 1,
				CGX_POLL_AN_STATUS)) {
			debug_cgx("%s: %d:%d SGMII AN not complete 0x%llx\n",
				__func__, cgx_id, lmac_id,
			CSR_READ(CAVM_CGXX_GMP_PCS_MRX_STATUS(
					cgx_id, lmac_id)));
			/* Reset AN */
			CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_pcs_mrx_control_t,
				CAVM_CGXX_GMP_PCS_MRX_CONTROL(cgx_id, lmac_id),
				rst_an, 1);
			cgx_set_error_type(cgx_id, lmac_id,
				CGX_ERR_AN_CPT_FAIL);
				return -1;
		}
	}
	return 0;
}

/* This API sets the speed for the link based on PHY status */
int cgx_sgmii_set_link_speed(int cgx_id, int lmac_id,
			 link_state_t *link)
{
	cavm_cgxx_cmrx_config_t cmr_config;
	cavm_cgxx_gmp_gmi_prtx_cfg_t prtx_cfg;
	cavm_cgxx_gmp_pcs_miscx_ctl_t pcs_miscx_ctl;
	cavm_cgxx_gmp_gmi_txx_slot_t txx_slot;
	cavm_cgxx_gmp_gmi_txx_burst_t txx_burst;

	debug_cgx("%s %d:%d\n", __func__, cgx_id, lmac_id);

	/* disable Rx/Tx */
	cmr_config.u = CSR_READ(CAVM_CGXX_CMRX_CONFIG(
				cgx_id, lmac_id));
	cmr_config.s.data_pkt_rx_en = 0;
	cmr_config.s.data_pkt_tx_en = 0;
	CSR_WRITE(CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
				cmr_config.u);

	/* Wait for GMX Tx/Rx to be idle */
	if (cgx_poll_for_csr(CAVM_CGXX_GMP_GMI_PRTX_CFG(cgx_id, lmac_id),
				CGX_GMP_TX_IDLE_MASK, 1, -1)) {
		debug_cgx("%s: %d:%d CGX GMP Tx not Idle\n",
				__func__, cgx_id, lmac_id);
		cgx_set_error_type(cgx_id, lmac_id, CGX_ERR_TX_NOT_IDLE);
		return -1;
	}
	if (cgx_poll_for_csr(CAVM_CGXX_GMP_GMI_PRTX_CFG(
				cgx_id, lmac_id),
				CGX_GMP_RX_IDLE_MASK, 1, -1)) {
		debug_cgx("%s: %d:%d CGX GMP Rx not Idle\n",
				__func__, cgx_id, lmac_id);
		cgx_set_error_type(cgx_id, lmac_id, CGX_ERR_RX_NOT_IDLE);
		return -1;
	}

	/* Read GMX CFG */
	prtx_cfg.u = CSR_READ(CAVM_CGXX_GMP_GMI_PRTX_CFG(
			cgx_id, lmac_id));

	/* Read PCS MISCS CTL */
	pcs_miscx_ctl.u = CSR_READ(CAVM_CGXX_GMP_PCS_MISCX_CTL(
				cgx_id, lmac_id));

	debug_cgx("%s: link %d duplex %d speed %d\n", __func__,
				link->s.link_up, link->s.full_duplex,
				link->s.speed);

	/* Use GMXENO to force the link down */
	if (link->s.link_up) {
		pcs_miscx_ctl.s.gmxeno = 0;
		/* change the duplex setting if the link is up */
		prtx_cfg.s.duplex = 1;
	} else
		pcs_miscx_ctl.s.gmxeno = 1;

	/* speed based setting for GMP */
	txx_burst.s.burst = 0;

	switch (link->s.speed) {
	case CGX_LINK_10M:
		prtx_cfg.s.speed = 0;
		prtx_cfg.s.speed_msb = 1;
		prtx_cfg.s.slottime = 0;
		pcs_miscx_ctl.s.samp_pt = 50; /* sampling point */
		txx_slot.s.slot = 0x40;
		CSR_WRITE(CAVM_CGXX_GMP_GMI_TXX_SLOT(cgx_id, lmac_id),
						txx_slot.u);
		CSR_WRITE(CAVM_CGXX_GMP_GMI_TXX_BURST(cgx_id, lmac_id),
					txx_burst.u);
		break;
	case CGX_LINK_100M:
		prtx_cfg.s.speed = 0;
		prtx_cfg.s.speed_msb = 0;
		prtx_cfg.s.slottime = 0;
		pcs_miscx_ctl.s.samp_pt = 5; /* sampling point */
		txx_slot.s.slot = 0x40;
		CSR_WRITE(CAVM_CGXX_GMP_GMI_TXX_SLOT(cgx_id, lmac_id),
						txx_slot.u);
		CSR_WRITE(CAVM_CGXX_GMP_GMI_TXX_BURST(cgx_id, lmac_id),
					txx_burst.u);
		break;
	case CGX_LINK_1G:
		prtx_cfg.s.speed = 1;
		prtx_cfg.s.speed_msb = 0;
		prtx_cfg.s.slottime = 1;
		pcs_miscx_ctl.s.samp_pt = 1; /* sampling point */
		txx_slot.s.slot = 0x200;
		CSR_WRITE(CAVM_CGXX_GMP_GMI_TXX_SLOT(cgx_id, lmac_id),
						txx_slot.u);
		if (!link->s.full_duplex)
			txx_burst.s.burst = 0x2000;
		CSR_WRITE(CAVM_CGXX_GMP_GMI_TXX_BURST(cgx_id, lmac_id),
					txx_burst.u);
		break;
	default:
		break;
	}

	/* write back the new PCS misc and GMX settings */
	CSR_WRITE(CAVM_CGXX_GMP_PCS_MISCX_CTL(cgx_id, lmac_id),
				pcs_miscx_ctl.u);
	CSR_WRITE(CAVM_CGXX_GMP_GMI_PRTX_CFG(cgx_id, lmac_id),
						prtx_cfg.u);

	/* read back GMX CFG again to check config completion */
	prtx_cfg.u = CSR_READ(
			CAVM_CGXX_GMP_GMI_PRTX_CFG(cgx_id, lmac_id));

	return 0;
}

static void cgx_set_autoneg(int cgx_id, int lmac_id)
{
	cgx_lmac_config_t *lmac;
	cavm_cgxx_gmp_pcs_anx_adv_t pcs_anx_adv;
	cavm_cgxx_gmp_pcs_sgmx_an_adv_t sgmx_an_adv;
	cavm_cgxx_spux_an_adv_t spux_an_adv;
	cavm_cgxx_gmp_pcs_mrx_control_t pcs_mrx_ctl;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	debug_cgx("%s: %d:%d, mode=%d an=%d\n", __func__,
				cgx_id, lmac_id, lmac->mode,
				!lmac->autoneg_dis);
	switch (lmac->mode) {
	case CAVM_CGX_LMAC_TYPES_E_SGMII:
	case CAVM_CGX_LMAC_TYPES_E_QSGMII:
		/* Write the Advertisement register to be used */
		if (lmac->sgmii_1000x_mode) { /* 1000 BASE-X mode */
			pcs_anx_adv.u = 0;
			pcs_anx_adv.s.rem_flt = 0; /* Link OK */
			/* both symmetric & asymmetric pause to local device */
			pcs_anx_adv.s.pause = 3;
			/* both half-duplex and full-duplex capable */
			pcs_anx_adv.s.hfd = 1;
			pcs_anx_adv.s.fd = 1;
			CSR_WRITE(CAVM_CGXX_GMP_PCS_ANX_ADV(
					cgx_id, lmac_id), pcs_anx_adv.u);
		} else { /* SGMII/QSGMII mode */
			if (lmac->phy_mode) {	/* PHY mode */
				sgmx_an_adv.u = 0;
				sgmx_an_adv.s.dup = 1; /* full duplex */
				sgmx_an_adv.s.speed = 1; /* 1000Mb/s */
				CSR_WRITE(CAVM_CGXX_GMP_PCS_SGMX_AN_ADV(
					cgx_id, lmac_id), sgmx_an_adv.u);
			} /* nothing to do in MAC mode */
		}

		pcs_mrx_ctl.u = CSR_READ(CAVM_CGXX_GMP_PCS_MRX_CONTROL(
						cgx_id, lmac_id));
		pcs_mrx_ctl.s.an_en = 1;
		pcs_mrx_ctl.s.rst_an = 1;
		CSR_WRITE(CAVM_CGXX_GMP_PCS_MRX_CONTROL(
				cgx_id, lmac_id), pcs_mrx_ctl.u);

		break;
	case CAVM_CGX_LMAC_TYPES_E_TENG_R:
	case CAVM_CGX_LMAC_TYPES_E_FORTYG_R:
		/* enable AN */
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_an_control_t,
				CAVM_CGXX_SPUX_AN_CONTROL(cgx_id, lmac_id),
				an_en, 1);
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_an_control_t,
				CAVM_CGXX_SPUX_AN_CONTROL(cgx_id, lmac_id),
				xnp_en, 1);

		/* program the negotiation parameters to be advertised */
		spux_an_adv.u = CSR_READ(CAVM_CGXX_SPUX_AN_ADV(
						cgx_id, lmac_id));
		spux_an_adv.u = 0;
		/* set the default values */
		spux_an_adv.s.xnp_able = 1;
		spux_an_adv.s.pause = 1;
		spux_an_adv.s.s	= 1;
		if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) {
			spux_an_adv.s.np = 0;
			if (lmac->use_training)
				spux_an_adv.s.a10g_kr = 1;
			/* only BASE-R ability */
			if (lmac->fec == CGX_FEC_BASE_R) {
				spux_an_adv.s.fec_able = 1;
				spux_an_adv.s.fec_req = 1;
			} else {
				spux_an_adv.s.fec_able = 0;
				spux_an_adv.s.fec_req = 0;
			}
		} else if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R) {
			spux_an_adv.s.np = 0;
			if (lmac->use_training)
				spux_an_adv.s.a40g_kr4 = 1;
			else
				spux_an_adv.s.a40g_cr4 = 1;
			if (lmac->fec == CGX_FEC_BASE_R) {
				/* only BASE-R ability */
				spux_an_adv.s.fec_able = 1;
				spux_an_adv.s.fec_req = 1;
			} else {
				spux_an_adv.s.fec_able = 0;
				spux_an_adv.s.fec_req = 0;
			}
		}
		CSR_WRITE(CAVM_CGXX_SPUX_AN_ADV(cgx_id, lmac_id),
				spux_an_adv.u);

		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spu_dbg_control_t,
			CAVM_CGXX_SPU_DBG_CONTROL(cgx_id),
			an_nonce_match_dis, 0);
		/* optionally set an_arb_link_chk_en. For CN9XXX pass 1, better
		 * not to set it due to errata #34763 for LMAC types with
		 * RS-FEC enabled
		 */
		if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) ||
			(lmac->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R))
			CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_an_control_t,
				CAVM_CGXX_SPUX_AN_CONTROL(cgx_id, lmac_id),
				an_arb_link_chk_en, 1);
		/* Restart AN */
		cgx_restart_an(cgx_id, lmac_id);
		break;
	/* FIXME: new modes to be supported */
	default:
		break;
	}
}

static void cgx_set_fec(int cgx_id, int lmac_id, int req_fec)
{
	int val = 0;
	cgx_lmac_config_t *lmac;
	cavm_cgxx_spux_rx_mrk_cnt_t rx_mrk_cnt;
	cavm_cgxx_spux_tx_mrk_cnt_t tx_mrk_cnt;
	int mrk_cnt = 0x3FFF;
	int ram_mrk_cnt;
	int by_mrk_100g = 1;

	debug_cgx("%s %d:%d fec type %d\n", __func__, cgx_id,
							lmac_id, req_fec);

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	if (req_fec == -1) {
		/* In this case, User did not specify any FEC type.
		 * By default enable FEC for
		 * 10G, 25G, 40G, 50G, 100G BASE-R & USXGMII modes
		 *
		 * Below table from HRM describes how to enable "fec_en" for each mode.
		 * fec_en is 2-bits
		 *
		 * Bit 0 enables BASE-R FEC. Bit 1 enables RS-FEC (Reed-Solomon FEC).
		 *
		 *	Value     LMAC_TYPE         Comment
		 *	-------   ---------------   ------------
		 *	0x0       All BASE-R        No FEC
		 *	0x1       25G_R, 50G_R,     BASE-R FEC enabled
		 *	0x2       25G_R, 50G_R,     RS-FEC enabled
		 *	0x3       25G_R, 50G_R,     UNDEFINED
		 *	0x2,0x3   100G_R, USXGMII   RS-FEC enabled
		 *	0x2       10G_R, 40G_R      No FEC. 10G_R, 40G_R may only use BASE-R FEC
		 *	0x1,0x3   10G_R, 40G_R      BASE-R FEC
		 *	0x1       100G_R            No FEC. 100G_R  may only use RS-FEC
		 *
		 */
		switch (lmac->mode) {
		case CAVM_CGX_LMAC_TYPES_E_TENG_R:
		case CAVM_CGX_LMAC_TYPES_E_FORTYG_R:
			val = CGX_FEC_BASE_R;
			break;
		case CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R:
		case CAVM_CGX_LMAC_TYPES_E_FIFTYG_R:
			/* RS-FEC considered the default for now */
			val = CGX_FEC_RS;
			break;
		case CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R:
		case CAVM_CGX_LMAC_TYPES_E_USXGMII:
			val = CGX_FEC_RS;
			break;
		default:
			val = CGX_FEC_NONE;
			break;
		}

		/* FIXME: for now disable FEC by default */
		val = 0;
		/* update the board config structure with FEC set */
		lmac->fec = val;
	} else {
		/* If user has requested specific FEC type,
		 * configure the FEC as such
		 */
		val = req_fec;

		/* Workaround for errata GSER-35489
		 * For T9X pass 1.0 and 1.1, always enable RS-FEC
		 * for 25G
		 */
		if ((IS_OCTEONTX_PASS(read_midr(), T96PARTNUM, 1, 0)) ||
			(IS_OCTEONTX_PASS(read_midr(), T96PARTNUM, 1, 1)) ||
			(IS_OCTEONTX_PASS(read_midr(), F95PARTNUM, 1, 0))) {
			switch (lmac->mode_idx) {
			case QLM_MODE_25GAUI_C2C:
			case QLM_MODE_25GAUI_C2M:
			case QLM_MODE_25G_CR:
			case QLM_MODE_25G_KR:
			case QLM_MODE_50GAUI_2_C2C:
			case QLM_MODE_50GAUI_2_C2M:
			case QLM_MODE_50G_CR2:
			case QLM_MODE_50G_KR2:
			case QLM_MODE_CAUI_4_C2C:
			case QLM_MODE_CAUI_4_C2M:
			case QLM_MODE_100G_CR4:
			case QLM_MODE_100G_KR4:
				val = CGX_FEC_RS;
			break;
			}
		}

		if (lmac->mode_idx == QLM_MODE_25GAUI_2_C2C ||
		    lmac->mode_idx == QLM_MODE_50GAUI_4_C2C) {
			/* Our proprietary 25GBASE-R2 and 50GBASE-R4 do not
			 * support FEC.
			 */
			val = CGX_FEC_NONE;
		}
	}
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_fec_control_t,
				CAVM_CGXX_SPUX_FEC_CONTROL(cgx_id, lmac_id),
				fec_en, val);
	/* Program Tx/Rx marker count based on LMAC/FEC settings */
	switch (lmac->mode) {
	case CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R:
		if (val == CGX_FEC_RS)
			mrk_cnt = 0x13FFC;
		else
			mrk_cnt = 0x3FFF;
	break;
	case CAVM_CGX_LMAC_TYPES_E_FIFTYG_R:
		if (val == CGX_FEC_RS) {
			by_mrk_100g = 0;
			mrk_cnt = 0x4FFF;
		} else
			mrk_cnt = 0x3FFF;
		break;
	case CAVM_CGX_LMAC_TYPES_E_USXGMII:
		if (val == CGX_FEC_RS)
			mrk_cnt = 0x13FFC;
		else
			mrk_cnt = 0x4010;
		break;
	default:
		/* All other modes with/wo FEC */
		mrk_cnt = 0x3FFF;
	break;
	}

	/* Rapid marker alignment, default is 15: for 100G, it should be 7 */
	ram_mrk_cnt = (lmac->mode == CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R) ? 7 : 15;
	debug_cgx("%s: mode %d fec %d program mrk_cnt 0x%x\t"
		"ram_mrk_cnt 0x%x\n", __func__, lmac->mode, val, mrk_cnt,
			ram_mrk_cnt);

	rx_mrk_cnt.u = CSR_READ(CAVM_CGXX_SPUX_RX_MRK_CNT(cgx_id,
						lmac_id));
	rx_mrk_cnt.s.ram_mrk_cnt = ram_mrk_cnt;
	rx_mrk_cnt.s.by_mrk_100g = by_mrk_100g;
	rx_mrk_cnt.s.mrk_cnt = mrk_cnt;

	CSR_WRITE(CAVM_CGXX_SPUX_RX_MRK_CNT(cgx_id, lmac_id),
				rx_mrk_cnt.u);

	tx_mrk_cnt.u = CSR_READ(CAVM_CGXX_SPUX_TX_MRK_CNT(cgx_id,
						lmac_id));
	tx_mrk_cnt.s.ram_mrk_cnt = ram_mrk_cnt;
	tx_mrk_cnt.s.by_mrk_100g = by_mrk_100g;
	tx_mrk_cnt.s.mrk_cnt = mrk_cnt;
	CSR_WRITE(CAVM_CGXX_SPUX_TX_MRK_CNT(cgx_id, lmac_id),
			tx_mrk_cnt.u);
}

static int cgx_usxgmii_spux_reset(int cgx_id, int lmac_id, int an_en)
{
	debug_cgx("%s %d:%d an_en %d\n", __func__, cgx_id, lmac_id, an_en);

	if (an_en) { /* AN enabled case */
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_usx_an_control_t,
			CAVM_CGXX_SPUX_USX_AN_CONTROL(cgx_id, lmac_id),
			an_reset, 1);
		if (cgx_poll_for_csr(CAVM_CGXX_SPUX_USX_AN_CONTROL(
					cgx_id, lmac_id),
					CGX_SPUX_USX_AN_RESET_MASK, 0, -1)) {
			debug_cgx("%s: %d:%d SPUX USXGMII AN reset not\t"
					"complete\n",
					__func__, cgx_id, lmac_id);
			cgx_set_error_type(cgx_id, lmac_id,
				CGX_ERR_SPUX_USX_AN_RESET_FAIL);
			return -1;
		}
	} else { /* AN disabled */
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_control1_t,
			CAVM_CGXX_SPUX_CONTROL1(cgx_id, lmac_id),
			reset, 1);

		if (cgx_poll_for_csr(CAVM_CGXX_SPUX_CONTROL1(cgx_id,
				lmac_id), CGX_SPUX_RESET_MASK, 0, -1)) {
			debug_cgx("%s: %d:%d SPUX reset not complete\n",
					__func__, cgx_id, lmac_id);
			cgx_set_error_type(cgx_id, lmac_id,
				CGX_ERR_SPUX_RESET_FAIL);
			return -1;
		}
	}
	return 0;
}

/* This function sets MAC/PHY mode.
 * Also 1000base-x/SGMII mode. Accordingly, programs the GMP timer
 */
static int cgx_sgmii_set_mode(int cgx_id, int lmac_id)
{
	int timer_val = 0;
	cgx_lmac_config_t *lmac;
	cavm_cgxx_gmp_pcs_miscx_ctl_t pcs_miscx_ctl;
	cavm_cgxx_gmp_pcs_mrx_control_t pcs_mrx_ctl;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	debug_cgx("%s: %d:%d mode %d AN enable %d\n",
			__func__, cgx_id, lmac_id, lmac->mode,
			!lmac->autoneg_dis);

	/* If phy is not present, force to 1G */
	if (!lmac->phy_present)	{
		pcs_mrx_ctl.u = CSR_READ(CAVM_CGXX_GMP_PCS_MRX_CONTROL(
						cgx_id, lmac_id));
		pcs_mrx_ctl.s.spdlsb = 0;
		pcs_mrx_ctl.s.spdmsb = 1;
		pcs_mrx_ctl.s.dup = 1;
		CSR_WRITE(CAVM_CGXX_GMP_PCS_MRX_CONTROL(
				cgx_id, lmac_id), pcs_mrx_ctl.u);
	}

	/* Adjust the MAC mode based on the info from DT */
	pcs_miscx_ctl.u = CSR_READ(CAVM_CGXX_GMP_PCS_MISCX_CTL(
					cgx_id, lmac_id));

	debug_cgx("%s phy mode %d 1000x-base mode %d\n", __func__,
			lmac->phy_mode, lmac->sgmii_1000x_mode);

	/* set to 1 for PHY mode. default is MAC mode */
	pcs_miscx_ctl.s.mac_phy = lmac->phy_mode;

	/* set to 1 for 1000base-x mode. default is SGMII/QSGMII mode */
	pcs_miscx_ctl.s.mode = lmac->sgmii_1000x_mode;

	CSR_WRITE(CAVM_CGXX_GMP_PCS_MISCX_CTL(cgx_id, lmac_id),
						pcs_miscx_ctl.u);

	/* Timer value should be 1.6 ms for SGMII/QSGMII and 10 ms for
	 * 1000 base-x mode
	 */
	if (pcs_miscx_ctl.s.mode) /* 1000 base-x mode */
		timer_val = 10000;
	else /* SGMII or QSGMII mode */
		timer_val = 1600; /* reset value */

	CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_pcs_linkx_timer_t,
			CAVM_CGXX_GMP_PCS_LINKX_TIMER(cgx_id, lmac_id),
			count, timer_val);
	return 0;
}

static void cgx_mode_change(int cgx_id, int lmac_id, int new_mode,
					int new_fec)
{
	/* FIXME */
	debug_cgx("%s: %d:%d\n", __func__, cgx_id, lmac_id);
}

int cgx_validate_fec_config(int cgx_id, int lmac_id, int req_fec)
{
	cgx_lmac_config_t *lmac;
	int fec = req_fec;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	debug_cgx("%s: %d:%d mode %d, req_fec %d\n", __func__, cgx_id, lmac_id,
		  lmac->mode, req_fec);

	/* FEC can be disabled by user. In that case, no need
	 * to validate against any PCS supported FEC option.
	 */
	if (fec == CGX_FEC_NONE) {
		debug_cgx("%s: FEC disabled\n", __func__);
		return fec;
	}

	/* Validate FEC configuration against PCS supported FEC option.
	 * If the type is not correct, set FEC to be -1 so default
	 * FEC type can be configured
	 */
	switch (lmac->mode) {
	case CAVM_CGX_LMAC_TYPES_E_TENG_R:
		if (fec != CGX_FEC_BASE_R) {
			WARN("%s: 10G PCS doesn't support FEC type %d\t"
				"default FEC type will be set\n",
				__func__, req_fec);
			fec = -1;
		}
	break;
	case CAVM_CGX_LMAC_TYPES_E_FORTYG_R:
		if (lmac->mode_idx == QLM_MODE_50GAUI_4_C2C) {
			/* The T9X's proprietary 50GBASE-R4 PCS (which is based
			 * on 40GBASE-R4) does not support FEC, but the PHY
			 * line-side PCS (50GBASE-R2 or 50GBASE-R1) does.
			 */
			if (lmac->phy_config.mod_type == PHY_MOD_TYPE_PAM4) {
				if (fec != CGX_FEC_RS) {
					WARN("%s: PAM4 does not support FEC type %d\t"
					     "default FEC type will be set\n",
					      __func__, req_fec);
					fec = -1;
				}
			} else {
				if (fec != CGX_FEC_BASE_R && fec != CGX_FEC_RS)
					fec = -1;
			}
		} else if (fec != CGX_FEC_BASE_R) {
			WARN("%s: 40G PCS doesn't support FEC type %d\t"
				"default FEC type will be set\n",
				__func__, req_fec);
			fec = -1;
		}
	break;
	case CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R:
	case CAVM_CGX_LMAC_TYPES_E_FIFTYG_R:
		if ((fec != CGX_FEC_BASE_R) && (fec != CGX_FEC_RS)) {
			WARN("%s: 25G/50G PCS doesn't support FEC type %d\t"
				"default FEC type will be set\n",
				__func__, req_fec);
			fec = -1;
		}
	break;
	case CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R:
		if (fec != CGX_FEC_RS) {
			WARN("%s: 100G PCS doesn't support FEC type %d\t"
				"default FEC type will be set\n",
				__func__, req_fec);
			fec = -1;
		}
	break;
	case CAVM_CGX_LMAC_TYPES_E_USXGMII:
		if (fec != CGX_FEC_RS) {
			WARN("%s: USXGMII PCS doesn't support FEC type %d\t"
				"default FEC type will be set\n",
				__func__, req_fec);
			fec = -1;
		}
	break;
	default:
		fec = 0;
	break;
	}

	return fec;
}


int cgx_get_supported_fec_type(int cgx_id, int lmac_id)
{
	int val = 0;
	cgx_lmac_config_t *lmac;

	debug_cgx("%s %d:%d\n", __func__, cgx_id, lmac_id);

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	switch (lmac->mode) {
	case CAVM_CGX_LMAC_TYPES_E_TENG_R:
	case CAVM_CGX_LMAC_TYPES_E_FORTYG_R:
		val = CGX_FEC_BASE_R;
		break;
	case CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R:
	case CAVM_CGX_LMAC_TYPES_E_FIFTYG_R:
		val = CGX_FEC_RS | CGX_FEC_BASE_R;
		break;
	case CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R:
	case CAVM_CGX_LMAC_TYPES_E_USXGMII:
		val = CGX_FEC_RS;
		break;
	default:
		val = CGX_FEC_NONE;
		break;
	}
	return val;
}

int cgx_fec_change(int cgx_id, int lmac_id, int new_fec)
{
	cgx_lmac_config_t *lmac;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	debug_cgx("%s: %d:%d fec %d\n", __func__, cgx_id, lmac_id, new_fec);

	/* Handle FEC configuration change, follow the
	 * below steps as mentioned in HRM
	 */

	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmr_global_config_t,
			CAVM_CGXX_CMR_GLOBAL_CONFIG(cgx_id), cgx_clk_enable, 1);

	/* disable LMAC */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
			CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id), enable, 0);

	if (!lmac->autoneg_dis)
		/* disable AN */
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_an_control_t,
				CAVM_CGXX_SPUX_AN_CONTROL(cgx_id, lmac_id),
				an_en, 0);
	/* Update the board LMAC config structure with the new
	 * configuration
	 */
	lmac->fec = new_fec;

	/* set the new FEC */
	cgx_set_fec(cgx_id, lmac_id, new_fec);

	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmr_global_config_t,
			CAVM_CGXX_CMR_GLOBAL_CONFIG(cgx_id), cgx_clk_enable, 0);

	/* optionally enable training */
	if (lmac->use_training) {
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_br_pmd_control_t,
			CAVM_CGXX_SPUX_BR_PMD_CONTROL(cgx_id, lmac_id),
				train_en, 1);
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_br_pmd_control_t,
				CAVM_CGXX_SPUX_BR_PMD_CONTROL(cgx_id, lmac_id),
				train_restart, 1);
	}

	if (!lmac->autoneg_dis) {
		/* AN reset */
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_an_control_t,
				CAVM_CGXX_SPUX_AN_CONTROL(cgx_id, lmac_id),
				an_reset, 1);

		if (cgx_poll_for_csr(CAVM_CGXX_SPUX_AN_CONTROL(
					cgx_id, lmac_id),
					CGX_SPUX_AN_RESET_MASK, 0, -1)) {
			ERROR("%s: %d:%d SPUX AN reset not complete\n",
						__func__, cgx_id, lmac_id);
			cgx_set_error_type(cgx_id, lmac_id,
						CGX_ERR_SPUX_AN_RESET_FAIL);
			return -1;
		}

		/* Enable AN and program AN related CSRs as AN reset
		 * will clear the corresponding CSRs
		 */
		cgx_set_autoneg(cgx_id, lmac_id);
	}

	/* enable LMAC */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
			CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id), enable, 1);

	return 0;
}

uint64_t cgx_get_lane_mask(int lane, int mode)
{
	uint64_t lane_mask = 0;

	switch (mode) {
	case CAVM_CGX_LMAC_TYPES_E_TENG_R:
	case CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R:
	case CAVM_CGX_LMAC_TYPES_E_USXGMII:
		lane_mask = 1ULL << lane;
	break;
	case CAVM_CGX_LMAC_TYPES_E_RXAUI:
	case CAVM_CGX_LMAC_TYPES_E_FIFTYG_R:
		if (lane & 1)
			lane_mask = 3ULL << (lane - 1);
		else
			lane_mask = 3ULL << lane;
	break;
	case CAVM_CGX_LMAC_TYPES_E_XAUI:
	case CAVM_CGX_LMAC_TYPES_E_FORTYG_R:
	case CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R:
		lane_mask = 0xF;
	break;
	default:
		lane_mask = 0;
	break;
	}

	return lane_mask;
}

static int cgx_complete_sw_an(int cgx_id, int lmac_id)
{
	int mode_req = 0, fec_req = 0;
	cgx_lmac_config_t *lmac;
	cavm_cgxx_spux_an_lp_base_t an_lp_base;
	cavm_cgxx_spux_an_control_t spux_an_ctl;
	cavm_cgxx_spux_an_adv_t an_adv;
	cavm_cgxx_spux_int_t spux_int;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	if (lmac->autoneg_dis)
		return 1;

	debug_cgx("%s: %d:%d mode %d\n", __func__, cgx_id, lmac_id, lmac->mode);

	if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_USXGMII)
		return 1;

	spux_int.u = CSR_READ(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id));
	spux_an_ctl.u = CSR_READ(CAVM_CGXX_SPUX_AN_CONTROL(cgx_id, lmac_id));

	if (spux_an_ctl.s.an_arb_link_chk_en) {
		if (spux_int.s.an_complete) {
			debug_cgx("%s:%d:%d: Skipping AN, AN compelete\n",
				__func__, cgx_id, lmac_id);
			return 0;
		}
	} else {
		if (spux_int.s.an_link_good) {
			debug_cgx("%s:%d:%d: Skipping AN, AN link good\n",
				__func__, cgx_id, lmac_id);
			return 0;
		}
	}

	/* Poll for reception of new page */
	if (cgx_poll_for_csr(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id),
		CGX_SPUX_AN_RX_PAGE_MASK, 1, CGX_POLL_AN_STATUS)) {
		debug_cgx("%s:%d:%d Timeout for AN base page\n",
				__func__, cgx_id, lmac_id);
		// FIXME: need to restart autoneg
		cgx_set_error_type(cgx_id, lmac_id, CGX_ERR_AN_CPT_FAIL);
		return -1;
	}

	/* Clear AN page RX - W1C */
	CSR_WRITE(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id), CGX_SPUX_AN_RX_PAGE_MASK);

	/* Read the link partner's base page ability */
	an_lp_base.u = CSR_READ(CAVM_CGXX_SPUX_AN_LP_BASE(cgx_id, lmac_id));
	debug_cgx("%s: an_lp_base.u 0x%llx\n", __func__, an_lp_base.u);
	if (an_lp_base.s.fec_able && an_lp_base.s.fec_req) {
		/* FEC requested */
		if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) ||
			(lmac->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R)) {
			fec_req = CGX_FEC_BASE_R;
		} else
			fec_req = CGX_FEC_NONE;
	} else
		fec_req = CGX_FEC_NONE; /* FEC not requested */

	/* Check if the partner or us has extended pages */
	an_adv.u = CSR_READ(CAVM_CGXX_SPUX_AN_ADV(cgx_id, lmac_id));
	if (an_lp_base.s.np || an_adv.s.np) {
		int num_pages = 0;
		cavm_cgxx_spux_an_xnp_tx_t xnp_tx;
		cavm_cgxx_spux_an_lp_xnp_t lp_xnp;
		cavm_cgxx_spux_int_t spu_int;

		spu_int.u = 0;
		spu_int.s.an_page_rx = 1;
		do {
			xnp_tx.u = 0;
			xnp_tx.s.mp = 1;
			xnp_tx.s.m_u = 1;
			switch (num_pages) {
			case 0: /* Send ethernet consortium header */
				if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R) {
					xnp_tx.s.u = 0x4df0353; //0x3530fd40;
					xnp_tx.s.np = 1;
					xnp_tx.s.mp = 1;
					xnp_tx.s.m_u = 0x005;
				}
				break;
			case 1: /* Send ethernet consortium data */
				if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R) {
					xnp_tx.s.u |= 1 << 4;  /* 25GBASE-KR */
					xnp_tx.s.u |= 1 << 5;  /* 25GBASE-CR */
					xnp_tx.s.u |= 1 << 8;  /* 50GBASE-KR2 */
					xnp_tx.s.u |= 1 << 9;  /* 50GBASE-CR2 */
					xnp_tx.s.u |= 1 << 24;  /* F1 - Clause 91 FEC ability */
					xnp_tx.s.u |= 1 << 25;  /* F2 - Clause 74 FEC ability */
					if (fec_req != CGX_FEC_NONE) {
						xnp_tx.s.u |= 1 << 26; /* F3 - Claue 91 FEC request */
						xnp_tx.s.u |= 1 << 27; /* F3 - Claue 74 FEC request */
					}
					xnp_tx.s.np = 0;
					xnp_tx.s.mp = 0;
					xnp_tx.s.m_u = 0x203;
				}
				break;
			default: /* Send empty page to finish protocol */
				break;
			}
			CSR_WRITE(CAVM_CGXX_SPUX_AN_XNP_TX(cgx_id, lmac_id), xnp_tx.u);

			if (cgx_poll_for_csr(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id),
				CGX_SPUX_AN_RX_PAGE_MASK, 1, CGX_POLL_AN_STATUS)) {
				debug_cgx("%s:%d:%d Timeout waiting for AN extended page 0x%llx\n",
						__func__, cgx_id, lmac_id,
						CSR_READ(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id)));
				cgx_restart_an(cgx_id, lmac_id);
				cgx_set_error_type(cgx_id, lmac_id, CGX_ERR_AN_CPT_FAIL);
				return -1;
			}

			lp_xnp.u = CSR_READ(CAVM_CGXX_SPUX_AN_LP_XNP(cgx_id, lmac_id));
			CSR_WRITE(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id), spu_int.u);
			debug_cgx("%s:%d:%d AN extended page %d sent 0x%llx, received 0x%llx\n",
					__func__, cgx_id, lmac_id, num_pages, xnp_tx.u, lp_xnp.u);
			num_pages++;
		} while (lp_xnp.s.np || xnp_tx.s.np);
	}

	/* Based on speed ability bit set, set LMAC type to compare
	* with the CGX configuration
	*/
	if (an_lp_base.s.a10g_kr)
		mode_req = CAVM_CGX_LMAC_TYPES_E_TENG_R;
	else if ((an_lp_base.s.a40g_cr4) || (an_lp_base.s.a40g_kr4))
		mode_req = CAVM_CGX_LMAC_TYPES_E_FORTYG_R;
	debug_cgx("%s %d:%d LMAC type configured %d, LP's ability %d\n",
			__func__, cgx_id, lmac_id, lmac->mode, mode_req);

	if (spux_an_ctl.s.an_arb_link_chk_en) {
		if (cgx_poll_for_csr(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id),
			CGX_SPUX_AN_CMP_MASK, 1, CGX_POLL_AN_STATUS)) {
			debug_cgx("%s: %d:%d Timeout waiting for AN not complete\n",
				__func__, cgx_id, lmac_id);
			cgx_set_error_type(cgx_id, lmac_id,
					CGX_ERR_AN_CPT_FAIL);
			return -1;
		}
	} else {
		/* if arb_link_chk_en not enabled, SW need to intervene AN
		 * process. poll for AN_LINK_GOOD state. It will be set when
		 * HCD technology has negotiated with link partner
		 * and perform HCD match
		 * This is FIXME
		 */
		/* Poll for AN_LINK_GOOD state */
		if (cgx_poll_for_csr(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id),
			CGX_SPUX_AN_LNK_MASK, 1, CGX_POLL_AN_STATUS)) {
			debug_cgx("%s: %d:%d Timeout waiting for AN link good\n",
						__func__, cgx_id, lmac_id);
			cgx_set_error_type(cgx_id, lmac_id,
					CGX_ERR_AN_CPT_FAIL);
			return -1;
		}

		if (lmac->mode != mode_req) {
			/* Handle mode change - reconfigure LMAC and GSER
			 * to new mode
			 */
			debug_cgx("%s: %d:%d LMAC type %d, LP's ability %d\n",
					__func__, cgx_id, lmac_id,
					lmac->mode, mode_req);
			cgx_mode_change(cgx_id, lmac_id, mode_req, fec_req);
			return 0;
		}
		debug_cgx("%s:No change in LMAC mode %d mode_req %d\n",
				__func__, lmac->mode, mode_req);
		//hcd_match = 1;

		if (lmac->fec != fec_req) {
			debug_cgx("%s %d:%d FEC type %d, LP's FEC ability %d\n",
				__func__, cgx_id, lmac_id, lmac->fec, fec_req);
			/* FIXME */
			return 0;
		}
		debug_cgx("%s: No change in FEC type %d fec_req %d\n",
			__func__, lmac->fec, fec_req);
		//hcd_match = 1; /* Both LMAC and FEC type match */
		/* If there is no change in LMAC mode/FEC configuration,
		 * disable AN and proceed
		 */
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_an_control_t,
				CAVM_CGXX_SPUX_AN_CONTROL(cgx_id, lmac_id),
				an_en, 0);
	}
	return 0;
}

static void cgx_fill_lmac_attributes(int cgx_idx, int lmac_idx)
{
	cgx_config_t *cgx;
	cgx_lmac_config_t *lmac;

	cgx = &(plat_octeontx_bcfg->cgx_cfg[cgx_idx]);
	lmac = &cgx->lmac_cfg[lmac_idx];

	/* Reset the attributes before updating for each mode */
	lmac->use_training = 0;
	cgx->usxgmii_mode = 0;
	lmac->sgmii_1000x_mode = 0;
	lmac->autoneg_dis = 0;

	switch (lmac->mode_idx) {
	case QLM_MODE_10G_KR:
	case QLM_MODE_40G_KR4:
		lmac->use_training = 1;
		break;
	case QLM_MODE_USXGMII_1X1:
	case QLM_MODE_USXGMII_2X1:
	case QLM_MODE_USXGMII_4X1:
		cgx->usxgmii_mode = 1;	/* set USXGMII for this CGX */
	case QLM_MODE_XFI:
	case QLM_MODE_SFI:
	case QLM_MODE_XLAUI:
	case QLM_MODE_XLAUI_C2M:
	case QLM_MODE_RXAUI:
	case QLM_MODE_XAUI:
	/* Fixed speed option. consider as AN disabled cases */
	case QLM_MODE_20GAUI_C2C:
	case QLM_MODE_25GAUI_C2C:
	case QLM_MODE_25GAUI_C2M:
	case QLM_MODE_25GAUI_2_C2C:
	case QLM_MODE_40GAUI_2_C2C:
	case QLM_MODE_50GAUI_2_C2C:
	case QLM_MODE_50GAUI_2_C2M:
	case QLM_MODE_50GAUI_4_C2C:
	case QLM_MODE_80GAUI_4_C2C:
	case QLM_MODE_CAUI_4_C2C:
	case QLM_MODE_CAUI_4_C2M:
		/* FIXME : always disable AN for USXGMII for now */
		lmac->autoneg_dis = 1;
	break;
	case QLM_MODE_1G_X:
		lmac->sgmii_1000x_mode = 1;
		lmac->autoneg_dis = 1;
	break;
	}
}

/* This API brings up SGMII/QSGMII link. This function to be called
 * after the PHY is up (only when link is up).
 */
int cgx_sgmii_set_link_up(int cgx_id, int lmac_id)
{
	cgx_lmac_config_t *lmac;
	cavm_cgxx_gmp_pcs_mrx_control_t pcs_mrx_ctl;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	debug_cgx("%s: %d:%d mode %d AN enable %d\n",
			__func__, cgx_id, lmac_id, lmac->mode,
			!lmac->autoneg_dis);

	/* FEC is not applicable in case of SGMII */
	lmac->fec = CGX_FEC_NONE;

	/* set MAC/PHY/1000 base-x/SGMII mode */
	cgx_sgmii_set_mode(cgx_id, lmac_id);

	/* enable LMAC */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
		CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
		enable, 1);

	/* disable GMI */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_pcs_miscx_ctl_t,
			CAVM_CGXX_GMP_PCS_MISCX_CTL(cgx_id, lmac_id),
			gmxeno, 1);

	/* Bring the PCS out of reset */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_pcs_mrx_control_t,
		CAVM_CGXX_GMP_PCS_MRX_CONTROL(cgx_id, lmac_id),
		reset, 1);

	/* Wait till reset done */
	if (cgx_poll_for_csr(CAVM_CGXX_GMP_PCS_MRX_CONTROL(cgx_id,
				lmac_id), CGX_GMP_PCS_RESET_MASK, 0, -1)) {
		debug_cgx("%s: %d:%d PCS reset not completed\n",
				__func__, cgx_id, lmac_id);
		cgx_set_error_type(cgx_id, lmac_id,
				CGX_ERR_PCS_RESET_FAIL);
		return -1;
	}

	/* Normal operation - power up */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_pcs_mrx_control_t,
			CAVM_CGXX_GMP_PCS_MRX_CONTROL(cgx_id, lmac_id),
			pwr_dn, 0);

	/* check if AN is enabled. If not enabled, just
	 * configure MRX_CONTROL CSR with default speed
	 */
	if (lmac->autoneg_dis) {
		pcs_mrx_ctl.u = CSR_READ(CAVM_CGXX_GMP_PCS_MRX_CONTROL(
						cgx_id, lmac_id));
		pcs_mrx_ctl.s.an_en = 0;
		pcs_mrx_ctl.s.spdlsb = 0;
		pcs_mrx_ctl.s.spdmsb = 1;
		CSR_WRITE(CAVM_CGXX_GMP_PCS_MRX_CONTROL(
			cgx_id, lmac_id), pcs_mrx_ctl.u);
	} else
		/* AN is desired. sequence from AN execution section(HRM) */
		cgx_set_autoneg(cgx_id, lmac_id);

	return 0;
}

int cgx_sgmii_check_link(int cgx_id, int lmac_id)
{
	cavm_cgxx_cmrx_config_t cmr_config;
	cgx_lmac_config_t *lmac;

	debug_cgx("%s %d:%d\n", __func__, cgx_id, lmac_id);

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	if ((lmac->mode != CAVM_CGX_LMAC_TYPES_E_SGMII) &&
		(lmac->mode != CAVM_CGX_LMAC_TYPES_E_QSGMII))
		return 0;

	if (!lmac->autoneg_dis) {
		if (cgx_poll_for_csr(CAVM_CGXX_GMP_PCS_MRX_STATUS(
			cgx_id, lmac_id), CGX_GMP_PCS_LNK_ST_MASK, 1, -1)) {
			debug_cgx("%s: %d:%d SGMII/QSGMII Link is not up 0x%llx\n",
				__func__, cgx_id, lmac_id,
			CSR_READ(CAVM_CGXX_GMP_PCS_MRX_STATUS(
					cgx_id, lmac_id)));
			/* Reset AN */
			CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_pcs_mrx_control_t,
					CAVM_CGXX_GMP_PCS_MRX_CONTROL(cgx_id, lmac_id),
					rst_an, 1);
			cgx_set_error_type(cgx_id, lmac_id,
					CGX_ERR_PCS_LINK_FAIL);
			return -1;
		}
	} else {
		if (cgx_poll_for_csr(CAVM_CGXX_GMP_PCS_RXX_SYNC(
			cgx_id, lmac_id), CGX_GMP_PCS_RXX_SYNC_MASK, 1, -1)) {
			debug_cgx("%s: %d:%d SGMII Link is not up 0x%llx\n",
				__func__, cgx_id, lmac_id,
			CSR_READ(CAVM_CGXX_GMP_PCS_RXX_SYNC(
					cgx_id, lmac_id)));
			cgx_set_error_type(cgx_id, lmac_id,
						CGX_ERR_PCS_LINK_FAIL);
			return -1;
		}
	}

	/* If the link is UP, enable Rx/Tx */
	cmr_config.u = CSR_READ(CAVM_CGXX_CMRX_CONFIG(
				cgx_id, lmac_id));
	cmr_config.s.data_pkt_rx_en = 1;
	cmr_config.s.data_pkt_tx_en = 1;
	CSR_WRITE(CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
				cmr_config.u);

	return 0;
}

/* This API brings down the link, typically called
 * when user invokes ifconfig down
 */
int cgx_sgmii_set_link_down(int cgx_id, int lmac_id)
{
	cavm_cgxx_cmrx_rx_fifo_len_t rx_fifo_len;
	cavm_cgxx_cmrx_tx_fifo_len_t tx_fifo_len;

	debug_cgx("%s %d:%d\n", __func__, cgx_id, lmac_id);

	/* Rx disable */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
			CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
			data_pkt_rx_en, 0);

	/* Wait until Rx buffer is empty and Tx path is Idle */
	do {
		rx_fifo_len.u = CSR_READ(CAVM_CGXX_CMRX_RX_FIFO_LEN(
				cgx_id, lmac_id));
		tx_fifo_len.u = CSR_READ(CAVM_CGXX_CMRX_TX_FIFO_LEN(
				cgx_id, lmac_id));
	} while ((rx_fifo_len.s.fifo_len > 0) &&
				(tx_fifo_len.s.lmac_idle != 1));

	/* Tx disable */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
			CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
			data_pkt_tx_en, 0);

	/* disable GMI */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_pcs_miscx_ctl_t,
			CAVM_CGXX_GMP_PCS_MISCX_CTL(cgx_id, lmac_id),
			gmxeno, 1);

	/* power down PCS */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_pcs_mrx_control_t,
			CAVM_CGXX_GMP_PCS_MRX_CONTROL(cgx_id, lmac_id),
			pwr_dn, 1);

	/* force on CGX clocks to ensure successful propagation of reset
	 * when disabling the link
	 */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmr_global_config_t,
		CAVM_CGXX_CMR_GLOBAL_CONFIG(cgx_id), cgx_clk_enable, 1);

	/* disable LMAC */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
		CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id), enable, 0);

	/* release the force of clocks */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmr_global_config_t,
		CAVM_CGXX_CMR_GLOBAL_CONFIG(cgx_id), cgx_clk_enable, 0);

	return 0;
}

int cgx_xaui_init_link(int cgx_id, int lmac_id)
{
	int rate = 0, i;
	cgx_config_t *cgx;
	cgx_lmac_config_t *lmac;
	cavm_cgxx_smux_tx_ctl_t smux_tx_ctl;
	cavm_cgxx_spu_usxgmii_control_t usxgmii_ctrl;
	link_state_t link;

	cgx = &plat_octeontx_bcfg->cgx_cfg[cgx_id];
	lmac = &cgx->lmac_cfg[lmac_id];

	debug_cgx("%s: %d:%d mode %d AN enable %d training %d\n",
			__func__, cgx_id, lmac_id, lmac->mode,
			!lmac->autoneg_dis, lmac->use_training);

	/* reset SPU */
	if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_USXGMII) {
		/* For USXGMII mode, reset of LMACs should be done
		 * for all LMACs in the CGX even if not used
		 */
		cgx_usxgmii_spux_reset(cgx_id, lmac_id,
						!lmac->autoneg_dis);

		/* if LMAC index matches the LMAC count in that CGX, the
		 * remaining LMACs will be disabled and hence perform reset here.
		 * cases : USXGMII_1X1 or USXGMII_2X1
		 */
		if (lmac_id == (cgx->lmac_count - 1)) {
			for (i = (lmac_id + 1); i < MAX_LMAC_PER_CGX; i++)
				cgx_usxgmii_spux_reset(cgx_id, i, !lmac->autoneg_dis);
		}
	} else { /* all other modes */
		if (lmac->autoneg_dis)	{
			CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_control1_t,
				CAVM_CGXX_SPUX_CONTROL1(cgx_id, lmac_id),
				reset, 1);
			if (cgx_poll_for_csr(CAVM_CGXX_SPUX_CONTROL1(
					cgx_id, lmac_id),
					CGX_SPUX_RESET_MASK, 0, -1)) {
				debug_cgx("%s: %d:%d SPUX reset not complete\n",
					__func__, cgx_id, lmac_id);
				cgx_set_error_type(cgx_id, lmac_id,
					CGX_ERR_SPUX_RESET_FAIL);
				return -1;
			} else {
				debug_cgx("%s: %d:%d Training not done\n",
					__func__, cgx_id, lmac_id);
			}
		} else { /* AN enabled case, reset AN_CONTROL bit */
			CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_an_control_t,
				CAVM_CGXX_SPUX_AN_CONTROL(cgx_id, lmac_id),
				an_reset, 1);
			if (cgx_poll_for_csr(CAVM_CGXX_SPUX_AN_CONTROL(
					cgx_id, lmac_id),
					CGX_SPUX_AN_RESET_MASK, 0, -1)) {
				debug_cgx("%s: %d:%d SPUX AN reset not\t"
					"complete\n",
					__func__, cgx_id, lmac_id);
				cgx_set_error_type(cgx_id, lmac_id,
						CGX_ERR_SPUX_AN_RESET_FAIL);
				return -1;
			}
		}
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spu_usxgmii_control_t,
			CAVM_CGXX_SPU_USXGMII_CONTROL(cgx_id),
			enable, 0); /* disable USXGMII */
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_usx_an_control_t,
			CAVM_CGXX_SPUX_USX_AN_CONTROL(cgx_id, lmac_id),
			an_en, 0); /* disable AN for USXGMII */
	}

	/* disable LMAC */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
			CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
			enable, 0);

	/* low power enable */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_control1_t,
			CAVM_CGXX_SPUX_CONTROL1(cgx_id, lmac_id),
			lo_pwr, 1);

	CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_misc_control_t,
			CAVM_CGXX_SPUX_MISC_CONTROL(cgx_id, lmac_id),
			rx_packet_dis, 0);

	/* disable SMU/SPU interrupts as number of them
	 * will occur during bring up of a link
	 * first zero XXX_INT and disable interrupts via ENA_W1C per HRM
	 */
	CSR_WRITE(CAVM_CGXX_SMUX_RX_INT(cgx_id, lmac_id),
		CSR_READ(CAVM_CGXX_SMUX_RX_INT(cgx_id, lmac_id)));
	CSR_WRITE(CAVM_CGXX_SMUX_TX_INT(cgx_id, lmac_id),
		CSR_READ(CAVM_CGXX_SMUX_TX_INT(cgx_id, lmac_id)));
	CSR_WRITE(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id),
		CSR_READ(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id)));
	CSR_WRITE(CAVM_CGXX_SMUX_RX_INT_ENA_W1C(cgx_id, lmac_id), ~0ULL);
	CSR_WRITE(CAVM_CGXX_SMUX_TX_INT_ENA_W1C(cgx_id, lmac_id), ~0ULL);
	CSR_WRITE(CAVM_CGXX_SPUX_INT_ENA_W1C(cgx_id, lmac_id), ~0ULL);

	/* enable link training, if applicable */
	if (lmac->use_training) {
		if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_USXGMII) {
			debug_cgx("%s: %d:%d UXSGMII doesn't support\t"
					"training\n", __func__, cgx_id,
					lmac_id);
			cgx_set_error_type(cgx_id, lmac_id,
					CGX_ERR_TRAINING_FAIL);
			return -1;
		}
		/* FIXME : not mentioned in HRM. Is this HW errata?
		 * As per HRM, these registers will be cleared
		 * before start of the traning. Why to explicitly clear them?
		 */
		CSR_WRITE(CAVM_CGXX_SPUX_BR_PMD_LD_CUP(cgx_id, lmac_id),
					0);
		CSR_WRITE(CAVM_CGXX_SPUX_BR_PMD_LP_CUP(cgx_id, lmac_id),
					0);
		CSR_WRITE(CAVM_CGXX_SPUX_BR_PMD_LP_REP(cgx_id, lmac_id),
					0);
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_br_pmd_control_t,
			CAVM_CGXX_SPUX_BR_PMD_CONTROL(cgx_id, lmac_id),
			train_en, 1);
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_br_pmd_control_t,
			CAVM_CGXX_SPUX_BR_PMD_CONTROL(cgx_id, lmac_id),
			train_restart, 1);
	} else
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_br_pmd_control_t,
			CAVM_CGXX_SPUX_BR_PMD_CONTROL(cgx_id, lmac_id),
			train_en, 0);

	/* Configure FEC type */
	cgx_set_fec(cgx_id, lmac_id, lmac->fec);

	/* configure and enable AN, if AN is desired */
	if (!lmac->autoneg_dis)
		cgx_set_autoneg(cgx_id, lmac_id);
	else { /* disable AN */
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_an_control_t,
				CAVM_CGXX_SPUX_AN_CONTROL(cgx_id, lmac_id),
				an_en, 0);
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_usx_an_control_t,
				CAVM_CGXX_SPUX_USX_AN_CONTROL(cgx_id, lmac_id),
				an_en, 0);
	}

	/* Program the rate type in case of USXGMII. rate type to be
	 * obtained from PHY's link speed
	 */
	if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_USXGMII) {
		/* No PHY is not an option in case
		 * of USXGMII. But this is for ASIM environment
		 */
		if (!lmac->phy_present) {
			/* get the sub type to retrieve the MAX allowed rate */
			usxgmii_ctrl.u = CSR_READ(
					CAVM_CGXX_SPU_USXGMII_CONTROL(cgx_id));
			/* configure the max allowed rate for the sub type */
			for (i = 0; i < MAX_USXGMII_TYPES; i++) {
				if (usxgmii_ctrl.s.usxgmii_type ==
						usxgmii_type[i].type) {
					rate = usxgmii_type[i].max_rate;
					break;
				}
			}
		} else {
			/* PHY present case : get link speed to get the rate */
			cgx_get_link_state(cgx_id, lmac_id, &link);
			rate = cgx_get_usxgmii_rate_from_link_speed(link.s.speed);
		}
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_control1_t,
			CAVM_CGXX_SPUX_CONTROL1(cgx_id, lmac_id),
			usxgmii_rate, rate);
	}

	/* enable LMAC, bringup the SMU/SPU */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_control1_t,
			CAVM_CGXX_SPUX_CONTROL1(cgx_id, lmac_id),
			lo_pwr, 0);

	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
			CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
			enable, 1);

	if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_USXGMII)
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spu_usxgmii_control_t,
			CAVM_CGXX_SPU_USXGMII_CONTROL(cgx_id),
			enable, 1);

	/* keep the reset values for lane polarity. select deficit
	 * idle count mode and unidirectional enable/disable
	 */
	smux_tx_ctl.u = CSR_READ(CAVM_CGXX_SMUX_TX_CTL(cgx_id, lmac_id));
	smux_tx_ctl.s.dic_en = 1;
	smux_tx_ctl.s.mia_en = 1;
	smux_tx_ctl.s.uni_en = 0;
	CSR_WRITE(CAVM_CGXX_SMUX_TX_CTL(cgx_id, lmac_id),
			smux_tx_ctl.u);

	return 0;
}

int cgx_xaui_set_link_up(int cgx_id, int lmac_id)
{
	int qlm, lane, num_lanes;
	uint64_t lane_mask;
	uint64_t timeout_usec = 150000; /* 150ms */
	uint64_t timeout;
	uint64_t debounce_usec = 100000; /* 100ms */
	uint64_t debounce_time;
	int link_up = 1;
	cgx_lmac_config_t *lmac;
	cavm_cgxx_spux_int_t spux_int;
	cavm_cgxx_spux_status1_t spux_status1;
	cavm_cgxx_spux_status2_t spux_status2;
	cavm_cgxx_cmrx_config_t cmr_config;
	cavm_cgxx_spux_br_status2_t br_status2;
	cavm_cgxx_spux_control1_t spux_control1;
	cavm_cgxx_smux_rx_ctl_t	smux_rx_ctl;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	debug_cgx("%s: %d:%d mode %d AN enable %d training %d\n",
			__func__, cgx_id, lmac_id, lmac->mode,
			!lmac->autoneg_dis, lmac->use_training);

	/* check whether AN is complete, if AN is enabled */
	if (!lmac->autoneg_dis) {
		if (cgx_complete_sw_an(cgx_id, lmac_id) != 0) {
			/* restart AN */
			cgx_restart_an(cgx_id, lmac_id);
			cgx_set_error_type(cgx_id, lmac_id,
				CGX_ERR_AN_CPT_FAIL);
			return -1;
		}
	}

	/* check training is complete if enabled */
	if (lmac->use_training) {
		if (cgx_poll_for_csr(CAVM_CGXX_SPUX_INT(cgx_id, lmac_id),
			CGX_SPUX_TRAINING_DONE_MASK, 1,
			CGX_POLL_TRAINING_STATUS)) {
			spux_int.u = CSR_READ(CAVM_CGXX_SPUX_INT(
					cgx_id, lmac_id));
			if (spux_int.s.training_done && !spux_int.s.training_failure) {
				debug_cgx("%s: %d:%d No Training failure\n",
					__func__, cgx_id, lmac_id);
			} else if (spux_int.s.training_failure) {
				/* Training failed, restart */
				debug_cgx("%s: %d:%d Restarting Training\t"
					"0x%llx\n", __func__, cgx_id, lmac_id,
					CSR_READ(CAVM_CGXX_SPUX_INT(
						cgx_id, lmac_id)));
				cgx_restart_training(cgx_id, lmac_id);
				cgx_set_error_type(cgx_id, lmac_id,
						CGX_ERR_TRAINING_FAIL);
				return -1;
			} else {
				debug_cgx("%s: %d:%d Training not done\n",
					__func__, cgx_id, lmac_id);
			}
		}
	} else {
		/* Perform RX EQU for non-KR interfaces and for the link
		 * speed >= 10Gbaud - XAUI/XLAUI/XFI
		 */
		qlm = lmac->qlm;
		/* For some boards, lanes are swizzled and the lane
		 * info in LMAC config structure might not have the
		 * swapped lane info and hence read it from lane_to_sds
		 */
		lane = lmac->rev_lane;
		/* Special case for EBB9604 for DLM4/5. Lanes are
		 * swizzled on EBB9604 and hence for DLM 4/5 case,
		 * even the QLM is different
		 */
		if (!strncmp(plat_octeontx_bcfg->bcfg.board_model,
					"ebb96", 5)) {
			if (qlm_get_lanes(lmac->qlm) == 2) {
				if (lmac->qlm == 4)
					qlm = 5;
				else if (lmac->qlm == 5)
					qlm = 4;
			}
		}
		lane_mask = cgx_get_lane_mask(lane, lmac->mode);

		/* Skip RX adaptation in internal loopback mode */
		spux_control1.u = CSR_READ(CAVM_CGXX_SPUX_CONTROL1(
					cgx_id, lmac_id));
		if (spux_control1.s.loopbck)
			lane_mask = 0;

		while (lane_mask) {
			num_lanes = qlm_get_lanes(qlm);
			for (int lane = 0; lane < num_lanes; lane++) {
				if (!(lane_mask & (1 << lane)))
					continue;
				if (!plat_octeontx_bcfg->qlm_cfg[qlm].
							rx_adaptation[lane])
					continue;
				if (qlm_rx_equalization_gsern(qlm, lane)
							== -1) {
					debug_cgx("%s:RX EQU failed %d:%d\n",
						__func__, qlm, lane);
					cgx_set_error_type(cgx_id, lmac_id,
						CGX_ERR_RX_EQU_FAIL);
					return -1;
				}
			}
			lane_mask >>= num_lanes;
		}
	}

	mdelay(1);

	/* POLL for the status of the link by checking lane
	 * alignment/block lock
	 */
	if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_XAUI) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_RXAUI)) {
		if (cgx_poll_for_csr(CAVM_CGXX_SPUX_BX_STATUS(cgx_id,
				lmac_id), CGX_SPUX_RX_ALIGN_MASK, 1, -1)) {
			debug_cgx("%s: %d:%d SPUX RX ALIGN not completed\n",
					__func__, cgx_id, lmac_id);
			cgx_set_error_type(cgx_id, lmac_id,
					CGX_ERR_SPUX_RX_ALIGN_FAIL);
			return -1;
		}
	}

	/* Clear latching bits */
	spux_status1.u = CSR_READ(CAVM_CGXX_SPUX_STATUS1(
					cgx_id, lmac_id));
	spux_status1.s.rcv_lnk = 1;
	CSR_WRITE(CAVM_CGXX_SPUX_STATUS1(cgx_id, lmac_id),
			spux_status1.u);

	spux_status2.u = CSR_READ(CAVM_CGXX_SPUX_STATUS2(
					cgx_id, lmac_id));
	spux_status2.s.rcvflt = 1;
	CSR_WRITE(CAVM_CGXX_SPUX_STATUS2(cgx_id, lmac_id),
			spux_status2.u);

	/* For BASE-R modes, for stable link to be established
	 * check if the link is error free upto 50ms
	 * FIXME for USXGMII
	 */
	if (((lmac->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_FIFTYG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R)) &&
		(strncmp(plat_octeontx_bcfg->bcfg.board_model, "asim-", 5))) {
		/* Wait up to 50ms for an error-free link */
		timeout = gser_clock_get_count(GSER_CLOCK_TIME) + timeout_usec *
			gser_clock_get_rate(GSER_CLOCK_TIME)/1000000;

		while (1) {
			link_up = 1;
			debounce_time = gser_clock_get_count(GSER_CLOCK_TIME) + debounce_usec *
					gser_clock_get_rate(GSER_CLOCK_TIME)/1000000;
			/* Clear the ERR_BLKS and BER_CNT */
			CSR_WRITE(CAVM_CGXX_SPUX_BR_STATUS2(cgx_id, lmac_id),
								0);
			/* Verify link is up and error-free for the debounce
			 * time
			 */
			while (link_up && (gser_clock_get_count(GSERN_CLOCK_TIME)
					< debounce_time)) {
				udelay(1);
				spux_status1.u = CSR_READ(
						CAVM_CGXX_SPUX_STATUS1(
						cgx_id, lmac_id));
				br_status2.u = CSR_READ(
						CAVM_CGXX_SPUX_BR_STATUS2(
						cgx_id, lmac_id));
				smux_rx_ctl.u = CSR_READ(CAVM_CGXX_SMUX_RX_CTL(
						cgx_id, lmac_id));

				if ((spux_status1.s.rcv_lnk == 1) &&
					(smux_rx_ctl.s.status == 0) &&
					(br_status2.s.err_blks == 0) &&
					(br_status2.s.ber_cnt == 0))
					link_up = 1;
				else
					link_up = 0;

			}

			if (link_up)
				break;

			if (gser_clock_get_count(GSER_CLOCK_TIME) >= timeout) {
				debug_cgx("%s: %d:%d Link error timeout %lld\n",
					__func__, cgx_id, lmac_id, timeout);
				spux_status1.u = CSR_READ(
						CAVM_CGXX_SPUX_STATUS1(
							cgx_id, lmac_id));
				br_status2.u = CSR_READ(
						CAVM_CGXX_SPUX_BR_STATUS2(
						cgx_id, lmac_id));
				smux_rx_ctl.u = CSR_READ(CAVM_CGXX_SMUX_RX_CTL(
						cgx_id, lmac_id));

				if (spux_status1.s.rcv_lnk != 1)
					debug_cgx("%s: %d:%d RCV_LNK timeout\n",
						__func__, cgx_id, lmac_id);
				else if (smux_rx_ctl.s.status == 1)
					debug_cgx("%s: %d:%d Local fault\n",
						__func__, cgx_id, lmac_id);
				else if (smux_rx_ctl.s.status == 2)
					debug_cgx("%s: %d:%d Remote fault\n",
						__func__, cgx_id, lmac_id);
				else if (br_status2.s.err_blks)
					debug_cgx("%s: %d:%d error blocks %d\n",
					__func__, cgx_id, lmac_id,
						br_status2.s.err_blks);
				else if (br_status2.s.ber_cnt)
					debug_cgx("%s: %d:%d ber count %d\n",
						__func__, cgx_id, lmac_id,
						br_status2.s.ber_cnt);
				else
					debug_cgx("%s: %d:%d timeout waiting\t"
						"for RX link to be stable %d\n",
						__func__, cgx_id, lmac_id,
						smux_rx_ctl.s.status);
				return -1;
			}

			/* Clear the block error counters */
			CSR_WRITE(CAVM_CGXX_SPUX_BR_STATUS2(
						cgx_id, lmac_id), 0);

			/* RCV_LNK is latching bit, so set it again */
			spux_status1.u = CSR_READ(CAVM_CGXX_SPUX_STATUS1(
						cgx_id, lmac_id));
			spux_status1.s.rcv_lnk = 1;
			CSR_WRITE(CAVM_CGXX_SPUX_STATUS1(cgx_id, lmac_id),
						spux_status1.u);
		}
		/* Write 1 to latched BER */
		br_status2.u = CSR_READ(CAVM_CGXX_SPUX_BR_STATUS2(
					cgx_id, lmac_id));
		br_status2.s.latched_ber = 1;
		CSR_WRITE(CAVM_CGXX_SPUX_BR_STATUS2(cgx_id, lmac_id),
					br_status2.u);
	}

	/* enable Data Tx/Rx */
	cmr_config.u = CSR_READ(CAVM_CGXX_CMRX_CONFIG(
				cgx_id, lmac_id));
	cmr_config.s.data_pkt_rx_en = 1;
	cmr_config.s.data_pkt_tx_en = 1;
	CSR_WRITE(CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
				cmr_config.u);
	return 0;
}

int cgx_xaui_get_link(int cgx_id, int lmac_id,
		link_state_t *result)
{
	cavm_cgxx_spux_status1_t spux_status1;
	cavm_cgxx_smux_tx_ctl_t smux_tx_ctl;
	cavm_cgxx_smux_rx_ctl_t	smux_rx_ctl;
	int speed = 0;

	spux_status1.u = CSR_READ(CAVM_CGXX_SPUX_STATUS1(
					cgx_id, lmac_id));
	smux_tx_ctl.u = CSR_READ(CAVM_CGXX_SMUX_TX_CTL(
					cgx_id, lmac_id));
	smux_rx_ctl.u = CSR_READ(CAVM_CGXX_SMUX_RX_CTL(
					cgx_id, lmac_id));

	if ((smux_tx_ctl.s.ls == 0) && (smux_rx_ctl.s.status ==
			0) && (spux_status1.s.rcv_lnk)) {
		/* link is up */
		result->s.link_up = 1;
		result->s.full_duplex = 1;
		speed = cgx_get_lane_speed(cgx_id, lmac_id);
		debug_cgx("%s: speed obtained %d\n", __func__, speed);
		result->s.speed = CGX_LINK_NONE;
		/* obtain the speed enum based on the speed in Mbps */
		for (int i = CGX_LINK_NONE; i < CGX_LINK_MAX; i++) {
			if (speed == cgx_link_speed_mbps[i]) {
				result->s.speed = i;
				break;
			}
		}
	} else {
		debug_cgx("%s: spux_status1 0x%llx, smux_tx_ctl 0x%llx smux_rx_ctl 0x%llx\n",
			__func__, spux_status1.u, smux_tx_ctl.u, smux_rx_ctl.u);

		/* When the link is down, try to re-initialize the CGX link */
		result->s.link_up = 0;
		result->s.full_duplex = 0;
		result->s.speed = 0;
		return -1;
	}

	return 0;
}

int cgx_xaui_set_link_down(int cgx_id, int lmac_id)
{
	int usx_en = 0;
	cgx_lmac_config_t *lmac;
	cavm_cgxx_cmrx_rx_fifo_len_t rx_fifo_len;
	cavm_cgxx_cmrx_tx_fifo_len_t tx_fifo_len;
	cavm_cgxx_cmrx_config_t cmrx_config;

	debug_cgx("%s %d:%d\n", __func__, cgx_id, lmac_id);

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	/* Rx disable */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
			CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
			data_pkt_rx_en, 0);

	/* Wait until Rx buffer is empty and Tx path is Idle */
	do {
		rx_fifo_len.u = CSR_READ(CAVM_CGXX_CMRX_RX_FIFO_LEN(
				cgx_id, lmac_id));
		tx_fifo_len.u = CSR_READ(CAVM_CGXX_CMRX_TX_FIFO_LEN(
				cgx_id, lmac_id));
	} while ((rx_fifo_len.s.fifo_len > 0) &&
				(tx_fifo_len.s.lmac_idle != 1));

	/* Tx disable */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
			CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
			data_pkt_tx_en, 0);

	/* FIXME: if flow control is enabled, disable appropriate
	 * flow control packets
	 */

	/* disable SerDes lane */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_control1_t,
			CAVM_CGXX_SPUX_CONTROL1(cgx_id, lmac_id),
			lo_pwr, 1);

	/* force on CGX clocks to ensure successful propagation of reset
	 * when disabling the link
	 */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmr_global_config_t,
		CAVM_CGXX_CMR_GLOBAL_CONFIG(cgx_id), cgx_clk_enable, 1);

	/* disable LMAC */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
		CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id), enable, 0);

	/* In case of USXGMII mode, also clear USXGMII enable only when
	 * all LMACs configured in USXGMII mode are disabled
	 */
	if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_USXGMII) {
		for (int i = 0; i < MAX_LMAC_PER_CGX; i++) {
			cmrx_config.u = CSR_READ(CAVM_CGXX_CMRX_CONFIG(
						cgx_id, lmac_id));
			/* if any of the LMACs are enabled, set usx_en */
			if (cmrx_config.s.enable) {
				usx_en = 1;
				break;
			}
		}
		if (!usx_en) {
			debug_cgx("%s %d %d disable usxgmii\n", __func__, cgx_id, lmac_id);
			CAVM_MODIFY_CGX_CSR(cavm_cgxx_spu_usxgmii_control_t,
				CAVM_CGXX_SPU_USXGMII_CONTROL(cgx_id),
				enable, 0);
		}
	}

	/* release the force of clocks */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmr_global_config_t,
		CAVM_CGXX_CMR_GLOBAL_CONFIG(cgx_id), cgx_clk_enable, 0);

	return 0;
}

void cgx_set_internal_loopback(int cgx_id, int lmac_id, int enable)
{
	uint64_t midr;
	cgx_lmac_config_t *lmac;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	debug_cgx("%s: %d:%d mode %d enable %d\n", __func__, cgx_id,
				lmac_id, lmac->mode, enable);

	/* Enable internal beat generation as GSERN's credit pulses are ignored
	 * in internal loopback mode
	 */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_config_t,
		CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
		int_beat_gen, enable);

	if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) || (lmac->mode ==
			CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_pcs_mrx_control_t,
			CAVM_CGXX_GMP_PCS_MRX_CONTROL(cgx_id, lmac_id),
			loopbck1, enable);
	} else if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_XAUI) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_RXAUI) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_FIFTYG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_USXGMII)) {
		midr = read_midr();
		/* SW workaround for errata #35544:
		 * SPU RX link doesn't come up in loopback mode
		 * if it doesn't get valids from SERDES. Hence
		 * set RX_EDET_SIGNAL_OK bit to use SERDES energy
		 * detect as signal_OK.
		 */
		if (IS_OCTEONTX_PASS(midr, T96PARTNUM, 1, 0))
			CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_misc_control_t,
				CAVM_CGXX_SPUX_MISC_CONTROL(cgx_id, lmac_id),
				rx_edet_signal_ok, enable);

		CAVM_MODIFY_CGX_CSR(cavm_cgxx_spux_control1_t,
			CAVM_CGXX_SPUX_CONTROL1(cgx_id, lmac_id),
			loopbck, enable);
	}
}

void cgx_set_external_loopback(int cgx_id, int lmac_id, int enable)
{
	cgx_lmac_config_t *lmac;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	debug_cgx("%s: %d:%d mode %d enable %d\n", __func__, cgx_id,
				lmac_id, lmac->mode, enable);

	if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) || (lmac->mode ==
			CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_gmp_pcs_miscx_ctl_t,
				CAVM_CGXX_GMP_PCS_MISCX_CTL(cgx_id, lmac_id),
				loopbck2, enable);
	} else if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_XAUI) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_RXAUI) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_FIFTYG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_USXGMII)) {
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_smux_ext_loopback_t,
			CAVM_CGXX_SMUX_EXT_LOOPBACK(cgx_id, lmac_id),
			en, enable);
	}
}

void cgx_lmac_init_link(int cgx_id, int lmac_id)
{
	cgx_lmac_config_t *lmac;

	debug_cgx("%s %d:%d\n", __func__, cgx_id, lmac_id);

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	switch (lmac->mode) {
	case CAVM_CGX_LMAC_TYPES_E_SGMII:
	case CAVM_CGX_LMAC_TYPES_E_QSGMII:
		if (cgx_sgmii_set_link_up(cgx_id, lmac_id) != 0) {
			debug_cgx("%s: %d:%d SGMII link initialization\t"
				"failed\n", __func__, cgx_id, lmac_id);
			break;
		}
		break;
	case CAVM_CGX_LMAC_TYPES_E_XAUI:
	case CAVM_CGX_LMAC_TYPES_E_RXAUI:
	case CAVM_CGX_LMAC_TYPES_E_TENG_R:
	case CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R:
	case CAVM_CGX_LMAC_TYPES_E_FORTYG_R:
	case CAVM_CGX_LMAC_TYPES_E_FIFTYG_R:
	case CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R:
	case CAVM_CGX_LMAC_TYPES_E_USXGMII:
		if (cgx_xaui_init_link(cgx_id, lmac_id) != 0) {
			debug_cgx("%s: %d:%d XAUI link initialization failed\n",
				__func__, cgx_id, lmac_id);
			break;
		}
		break;
	default:
		debug_cgx("%s invalid mode %d\n", __func__, lmac->mode);
		break;
	}
}

/* this function to be called for every CGX either from
 * PCI scanning (CGX device enumeration) or
 * during INTF initialization
 */
void cgx_hw_init(int cgx_id)
{
	int lmac_id, qlm_mode, lmac_type;
	int baud_rate, flags = 0, lane, ret;
	qlm_state_lane_t state;
	cgx_config_t *cgx;
	cgx_lmac_config_t *lmac;
	cavm_cgxx_cmrx_config_t cmr_config;

	debug_cgx("%s: %d\n", __func__, cgx_id);

	cgx = &plat_octeontx_bcfg->cgx_cfg[cgx_id];

	if (cgx->enable) {
		/* program the LMAC count (already updated by FDT parser)
		 * for each CGX
		 */
		debug_cgx("%s lmac_count %d\n", __func__, cgx->lmac_count);
		CSR_WRITE(CAVM_CGXX_CMR_TX_LMACS(cgx_id),
					cgx->lmac_count);
		CSR_WRITE(CAVM_CGXX_CMR_RX_LMACS(cgx_id),
					cgx->lmac_count);

		/* FCS setting */
		CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmr_global_config_t,
			CAVM_CGXX_CMR_GLOBAL_CONFIG(cgx_id), fcs_strip, 1);

		/* Retrieve the LMAC config from plat_octeontx_bcfg structure and loop
		 * through them for lmac_count times and program the HW
		 * for each LMAC
		 */
		for (lmac_id = 0; lmac_id < MAX_LMAC_PER_CGX; lmac_id++) {
			lmac = &cgx->lmac_cfg[lmac_id];
			if (lmac->lmac_enable)	{
				ret = cgx_read_flash_mode_param(cgx_id, lmac_id, &qlm_mode, &lmac_type);
				if (!ret) {
					/* Update the board config structure with QLM/LMAC
					 * mode from flash if ignoreflash parameter
					 * not set by user
					 */
					lmac->mode_idx = qlm_mode;
					lmac->mode = lmac_type;

					/* Update PCS attributes based on each mode */
					cgx_fill_lmac_attributes(cgx_id, lmac_id);
					/* Configure SerDes for new QLM mode */
					baud_rate = plat_octeontx_get_baud_rate_qlm_mode(qlm_mode);
					state = qlm_build_state(qlm_mode, baud_rate, flags);
					if (qlm_mode == QLM_MODE_50GAUI_4_C2C ||
						qlm_mode == QLM_MODE_25GAUI_2_C2C)
						lane = -1;
					else
						lane = lmac->rev_lane;
					qlm_set_mode_gsern(lmac->qlm, lane,
								qlm_mode, baud_rate, flags);
					GSERN_CSR_WRITE(CAVM_GSERNX_LANEX_SCRATCHX(
								lmac->qlm, lmac->lane, 0), state.u);
				}
				cgx_lmac_init(cgx_id, lmac_id);
			} else
				debug_cgx("%s LMAC%d not enabled\n", __func__, lmac_id);

			/* enable CMR INT for notifications to kernel cmds/evt */
			CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_int_ena_w1s_t,
				CAVM_CGXX_CMRX_INT_ENA_W1S(cgx_id, lmac_id),
				overflw, 1);
		}

	} else {
		/* if CGX not enabled, configure the number of LMACs
		 * in CGX to be zero. configure the LMAC type as 0
		 * and lane_to_sds with the reset value of 0xE4
		 */
		CSR_WRITE(CAVM_CGXX_CMR_TX_LMACS(cgx_id), 0);
		CSR_WRITE(CAVM_CGXX_CMR_RX_LMACS(cgx_id), 0);

		for (lmac_id = 0; lmac_id < MAX_LMAC_PER_CGX; lmac_id++) {
			cmr_config.u = CSR_READ( CAVM_CGXX_CMRX_CONFIG(
					cgx_id, lmac_id));
			cmr_config.s.lmac_type = 0;
			cmr_config.s.lane_to_sds = 0xE4; /* reset value */
			CSR_WRITE(CAVM_CGXX_CMRX_CONFIG(cgx_id, lmac_id),
					cmr_config.u);
		}
	}
}

