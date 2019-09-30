/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */
#ifndef _QLM_GSERR_H__
#define _QLM_GSERR_H__

#define GSERRX_LNX_DRV_REFCLK_TXEQ_STATUS0_ACK		BIT(0)
#define GSERRX_PHY0_MB_CMD_FLAG_DATA			BIT(0)
#define GSERRX_PHY0_MB_RSP_FLAG_DATA			BIT(0)

/**
 * Get the SERDES state
 *
 * @param qlm    Index into GSER* group
 * @param lane   Lane in GSER*
 *
 * @return State data
 */
qlm_state_lane_t qlm_gserr_get_state(int qlm, int lane);

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
int qlm_gserr_set_mode(int qlm, int lane, qlm_modes_t mode, int baud_mhz, qlm_mode_flags_t flags);

/**
 * Measure the reference clock of a QLM
 *
 * @param qlm    QLM to measure
 *
 * @return Clock rate in Hz
 */
int qlm_gserr_measure_refclock(int qlm);

/**
 * Put a QLM into hardware reset
 *
 * @param qlm    QLM to use
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_reset(int qlm);

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
int qlm_gserr_enable_prbs(int qlm, int prbs, qlm_direction_t dir);

/**
 * Disable PRBS on a QLM
 *
 * @param qlm    QLM to use
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_disable_prbs(int qlm);

/**
 * Return the number of PRBS errors since PRBS started running
 *
 * @param qlm    QLM to use
 * @param lane   Which lane
 * @param clear  Clear counter after return the current value
 *
 * @return Number of errors
 */
uint64_t qlm_gserr_get_prbs_errors(int qlm, int lane, int clear);

/**
 * Inject an error into PRBS
 *
 * @param qlm    QLM to use
 * @param lane   Which lane
 */
void qlm_gserr_inject_prbs_error(int qlm, int lane);

/**
 * Enable shallow loopback on a QLM
 *
 * @param qlm    QLM to use
 * @param loop   Type of loopback. Not all QLMs support all modes
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_enable_loop(int qlm, qlm_loop_t loop);

/**
 * Configure the TX tuning parameters for a QLM lane
 *
 * @param qlm       QLM to configure
 * @param lane      Lane to configure
 * @param tx_cmain  Transmitter Main (C0) equalizer tap coefficient value.
 *                  Programs the SerDes transmitter Main tap. Valid range is
 *                  0x30 to 0x18.
 * @param tx_cpre   Transmitter Pre (C-1) equalizer tap coefficient value.
 *                  Programs the transmitter Pre tap. Valid range is 0 to 0x10.
 * @param tx_cpost  Transmitter Post (C+1) equalizer tap coefficient value.
 *                  Programs the transmitter Post tap. Valid range is 0 to
 *                  0x10.
 * @param tx_bs     TX bias/swing selection. This setting only takes effect if
 *                  [TX_CSPD] is deasserted; with [TX_CSPD] asserted the
 *                  bias/swing control setting seen in the analog bias generator
 *                  is zero.
 *                  Typical override values would be:
 *                  42 = Nominal 1.0V p-p transmit amplitude.
 *                  52 = Nominal 1.2V p-p transmit amplitude.
 *                  The maximum usable value without transmitted waveform
 *                  distortion depends primarily on voltage, secondarily on
 *                  process corner and temperature, but is at least 52. There is
 *                  no minimum setting based on transmitter distortion, only
 *                  that set by the receiver.
 * @param tx_unused Currently unused
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_tune_lane_tx(int qlm, int lane, int tx_cmain, int tx_cpre, int tx_cpost, int tx_bs, int tx_unused);

/**
 * Get the TX tuning parameters for a QLM lane
 *
 * @param qlm       QLM to configure
 * @param lane      Lane to configure
 * @param tx_cmain  Transmitter Main (C0) equalizer tap coefficient value.
 *                  Programs the SerDes transmitter Main tap. Valid range is
 *                  0x30 to 0x18.
 * @param tx_cpre   Transmitter Pre (C-1) equalizer tap coefficient value.
 *                  Programs the transmitter Pre tap. Valid range is 0 to 0x10.
 * @param tx_cpost  Transmitter Post (C+1) equalizer tap coefficient value.
 *                  Programs the transmitter Post tap. Valid range is 0 to
 *                  0x10.
 * @param tx_bs     TX bias/swing selection. This setting only takes effect if
 *                  [TX_CSPD] is deasserted; with [TX_CSPD] asserted the
 *                  bias/swing control setting seen in the analog bias generator
 *                  is zero.
 *                  Typical override values would be:
 *                  42 = Nominal 1.0V p-p transmit amplitude.
 *                  52 = Nominal 1.2V p-p transmit amplitude.
 *                  The maximum usable value without transmitted waveform
 *                  distortion depends primarily on voltage, secondarily on
 *                  process corner and temperature, but is at least 52. There is
 *                  no minimum setting based on transmitter distortion, only
 *                  that set by the receiver.
 * @param tx_unused Currently unused
 *
 * @return Zero on success, negative on failure
 */
int qlm_gserr_get_tune_lane_tx(int qlm, int lane, int *tx_cmain, int *tx_cpre, int *tx_cpost, int *tx_bs, int *tx_unused);

/**
 * Perform RX equalization on a QLM
 *
 * @param qlm      QLM to perform RX equalization on
 * @param qlm_lane Lane to use, or -1 for all lanes
 *
 * @return Zero on success, negative if any lane failed RX equalization
 */
int qlm__gserr_rx_equalization(int qlm, int qlm_lane);

/**
 * Display the current settings of a QLM lane
 *
 * @param qlm      QLM to display
 * @param qlm_lane Lane to use
 * @param show_tx  Display TX parameters
 * @param show_rx  Display RX parameters
 */
void qlm_gserr_display_settings(int qlm, int qlm_lane, bool show_tx, bool show_rx);

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
int qlm_gserr_eye_capture(int qlm, int lane, int show_data, gser_qlm_eye_t *eye_data);

#endif /* _QLM_GSERR_H_ */
