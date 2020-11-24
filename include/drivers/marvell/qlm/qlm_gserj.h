/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */
#ifndef _QLM_GSERJ_H_
#define _QLM_GSERJ_H_

#include "cavm-csrs-gserj.h"

#define GSERJX_LNX_DRV_REFCLK_TXEQ_STATUS0_ACK		BIT(0)
#define GSERJX_PHY0_MB_CMD_FLAG_DATA			BIT(0)
#define GSERJX_PHY0_MB_RSP_FLAG_DATA			BIT(0)

#define GSERJX_STATUS_BSTS_LN_STAT_RXVALID		BIT(4)
#define GSERJX_STATUS_BSTS_LN_RX_RDY			BIT(2)
#define GSERJX_STATUS_BSTS_LN_TX_RDY			BIT(1)
#define GSERJX_STATUS_BSTS_LN_STATE_CHNG_RDY		BIT(0)

#define GSERJX_LNX_TOP_LN_STAT_CTRL0_RXVALID            BIT(0)

#define GSERJX_PHY0_TOP_RESET_CTRL_LN0_RSVD_RXDP_SW_RESET	BIT(4)

#define GSERJX_PHY0_TOP_RESET_CTRL_LN1_RSVD_RXDP_SW_RESET	BIT(4)

#define GSERJX_COMMON_PHY_STATUS_BSTS_CM0_OK			BIT(2)
#define GSERJX_COMMON_PHY_STATUS_BSTS_CM0_STATE_CHNG_RDY	BIT(1)

/**
 * Get the SERDES state
 *
 * @param qlm    Index into GSER* group
 * @param lane   Lane in GSER*
 *
 * @return State data
 */
qlm_state_lane_t qlm_gserj_get_state(int qlm, int lane);

/**
 * For chips that don't use pin strapping, this function programs
 * the QLM to the specified mode
 *
 * @param qlm      QLM to configure
 * @param lane     Lane inside the QLM
 * @param mode     Desired mode
 * @param baud_mhz Desired speed
 * @param flags    Flags to specify mode specific options
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_set_mode(int qlm, int lane, qlm_modes_t mode, int baud_mhz, qlm_mode_flags_t flags);

/**
 * Measure the reference clock of a QLM
 *
 * @param qlm    QLM to measure
 *
 * @return Clock rate in Hz
 */
int qlm_gserj_measure_refclock(int qlm);

/**
 * Put a QLM into hardware reset
 *
 * @param qlm    QLM to use
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_reset(int qlm);

/**
 * Enable PRBS on a QLM
 *
 * @param qlm    QLM to use
 * @param prbs   PRBS mode (31, etc)
 * @param dir    Directions to enable. This is so you can enable TX and later
 *               enable RX after TX has run for a time
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_enable_prbs(int qlm, int prbs, qlm_direction_t dir, int qlm_lane);

/**
 * Disable PRBS on a QLM
 *
 * @param qlm    QLM to use
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_disable_prbs(int qlm, int qlm_lane);

/**
 * Return the number of PRBS errors since PRBS started running
 *
 * @param qlm    QLM to use
 * @param lane   Which lane
 * @param clear  Clear counter after return the current value
 *
 * @return Number of errors
 */
uint64_t qlm_gserj_get_prbs_errors(int qlm, int lane, int clear);

/**
 * Inject an error into PRBS
 *
 * @param qlm    QLM to use
 * @param lane   Which lane
 */
void qlm_gserj_inject_prbs_error(int qlm, int lane);

/**
 * Configure the TX tuning parameters for a QLM lane
 *
 * @param qlm        QLM to configure
 * @param lane       Lane to configure
 * @param tx_swing   Transmitter Main (C0) equalizer tap coefficient value.
 *                   Programs the SerDes transmitter Main tap. Valid range is
 *                   0(0% swing) to 7(100% swing).
 * @param tx_cpre    Transmitter Pre (C-1) equalizer tap coefficient value.
 *                   Programs the transmitter Pre tap. Valid range is 0(min)
 *                   to 15(max).
 * @param tx_cpost   Transmitter Post (C+1) equalizer tap coefficient value.
 *                   Programs the transmitter Post tap. Valid range is 0(min)
 *                   to 31(max).
 * @param tx_unused1
 * @param tx_unused2
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_tune_lane_tx(int qlm, int lane, int tx_swing, int tx_cpre, int tx_cpost, int tx_unused1, int tx_unused2);

/**
 * Display the current settings of a QLM lane
 *
 * @param qlm      QLM to display
 * @param qlm_lane Lane to use
 * @param show_tx  Display TX parameters
 * @param show_rx  Display RX parameters
 */
void qlm_gserj_display_settings(int qlm, int qlm_lane, bool show_tx, bool show_rx, char *buf, int size);

/**
 * Capture an eye diagram for the given QLM lane. The output data is written
 * to "eye".
 *
 * @param qlm       QLM to use
 * @param qlm_lane  Which lane
 * @param show_data Set to non-zero to have the eye capture dump the raw eye data as
 *                  it is captured
 * @param eye       Output eye data
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_eye_capture(int qlm, int lane, int show_data, gser_qlm_eye_t *eye_data);

/**
 * Some SERDES can display extra tracing about AN, training, etc. This function
 * displays those traces if available. If the SERDES doesn't support tracing, a
 * informational message tells the user.
 *
 * @param module GSERJ to trace
 * @param lane   Lane to trace
 * @param unused Unused argument. Present so a number of QLM functions have the same signature
 *               for easy calling in the network driver
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_display_trace(int module, int lane, int unused);

/**
 * Implementation of NED Loopback with Internal BIST PRBS Generator/Checker.
 * Based on CNF95XX_B0_GSERJ_Programming_v1p2.pdf
 *
 * @param module
 * @param lane
 * @param enable
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_ned_loopback(int module, int lane, bool enable);

/**
 * Implementation of Far-End Analog (FEA) Loopback
 *
 * @param module
 * @param lane
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_fea_loopback(int module, int lane, bool enable);

/**
 * Implementation of Far-End Digital (FED) Loopback
 *
 * @param module	which GSERJ instance
 * @param lane	 which lane
 * @param enable
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_fed_loopback(int module, int lane, bool enable);

/**
 * Implementation of Near-End Analog (NEA) Loopback
 *
 * @param module	which GSERJ instance
 * @param lane	 which lane
 * @param enable
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserj_nea_loopback(int module, int lane, bool enable);

#endif /* _QLM_GSERJ_H_ */
