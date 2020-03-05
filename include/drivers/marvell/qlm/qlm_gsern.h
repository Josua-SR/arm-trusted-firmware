/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */
#ifndef _QLM_GSERN_H_
#define _QLM_GSERN_H_

#include "cavm-csrs-gsern.h"
#include "cavm-csrs-pem.h"
#include "cavm-csrs-rst.h"
#include "cavm-csrs-tsn.h"

/*
 * Setup the PEM to either driver or receive reset from PRST based on RC or EP
 *
 * @param pem	Which PEM to setuo
 * @param is_endpoint  Non zero if PEM is a EP
 */
void qlm_setup_pem_reset_gsern(int pem, int is_endpoint);

/*
 * Given a valid PEM number, return its speed in Gbaud
 *
 * @param pem	PEM to get speed of
 *
 * @return Speed in Gbaud. Zero if disabled
 */
int qlm_get_gbaud_mhz_pem_gsern(int pem);

/**
 * Measure the reference clock of a QLM
 *
 * @param qlm    QLM to measure
 *
 * @return Clock rate in Hz
 */
int qlm_measure_refclock_gsern(int qlm);

/*
 * Put a QLM into hardware reset
 *
 * @param qlm	QLM to use
 *
 * @return Zero on success, negative on failure
 */
int qlm_reset_gsern(int qlm);

/*
 * Enable PRBS on a QLM
 *
 * @param qlm	QLM to use
 * @param prbs   PRBS mode (31, etc)
 * @param dir	Directions to enable. This is so you can enable TX and later
 *			   enable RX after TX has run for a time
 *
 * @return Zero on success, negative on failure
 */
int qlm_enable_prbs_gsern(int qlm, int prbs, qlm_direction_t dir);

/*
 * Disable PRBS on a QLM
 *
 * @param qlm	QLM to use
 *
 * @return Zero on success, negative on failure
 */
int qlm_disable_prbs_gsern(int qlm);

/*
 * Return the number of PRBS errors since PRBS started running
 *
 * @param qlm	QLM to use
 * @param lane   Which lane
 * @param clear  Clear counter after return the current value
 *
 * @return Number of errors
 */
uint64_t qlm_get_prbs_errors_gsern(int qlm, int lane, int clear);

/*
 * Inject an error into PRBS
 *
 * @param qlm	QLM to use
 * @param lane   Which lane
 */
void qlm_inject_prbs_error_gsern(int qlm, int lane);

/*
 * Enable shallow loopback on a QLM
 *
 * @param qlm	QLM to use
 * @param loop	Type of loopback. Not all QLMs support all modes
 *
 * @return Zero on success, negative on failure
 */
int qlm_enable_loop_gsern(int qlm, qlm_loop_t loop);

/*
 * Perform RX equalization on a QLM
 *
 * @param qlm	  QLM to perform RX equalization on
 * @param qlm_lane Lane to use, or -1 for all lanes
 *
 * @return Zero on success, negative if any lane failed RX equalization
 */
int qlm_rx_equalization_gsern(int qlm, int qlm_lane);

/*
 * Configure the TX tuning parameters for a QLM lane
 *
 * @param qlm	   QLM to configure
 * @param lane	  Lane to configure
 * @param tx_cmain  Transmitter Main (C0) equalizer tap coefficient value.
 *                  Programs the SerDes transmitter Main tap. Valid range is
 *                  0x30 to 0x18.
 * @param tx_cpre   Transmitter Pre (C-1) equalizer tap coefficient value.
 *                  Programs the transmitter Pre tap. Valid range is 0 to 0x10.
 * @param tx_cpost  Transmitter Post (C+1) equalizer tap coefficient value.
 *                  Programs the transmitter Post tap. Valid range is 0 to
 *                  0x10.
 * @param tx_bs	 TX bias/swing selection. This setting only takes effect if
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
int qlm_tune_lane_tx_gsern(int qlm, int lane, int tx_cmain, int tx_cpre,
	int tx_cpost, int tx_bs, int tx_unused);

/*
 * Get the TX tuning parameters for a QLM lane
 *
 * @param qlm	   QLM to configure
 * @param lane	  Lane to configure
 * @param tx_cmain  Transmitter Main (C0) equalizer tap coefficient value.
 *                  Programs the SerDes transmitter Main tap. Valid range is
 *                  0x30 to 0x18.
 * @param tx_cpre   Transmitter Pre (C-1) equalizer tap coefficient value.
 *                  Programs the transmitter Pre tap. Valid range is 0 to 0x10.
 * @param tx_cpost  Transmitter Post (C+1) equalizer tap coefficient value.
 *                  Programs the transmitter Post tap. Valid range is 0 to
 *                  0x10.
 * @param tx_bs	 TX bias/swing selection. This setting only takes effect if
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
int qlm_get_tune_lane_tx_gsern(int qlm, int lane, int *tx_cmain, int *tx_cpre,
	int *tx_cpost, int *tx_bs, int *tx_unused);

/*
 * Display the current settings of a QLM lane
 *
 * @param qlm	  QLM to display
 * @param qlm_lane Lane to use
 * @param show_tx  Display TX parameters
 * @param show_rx  Display RX parameters
 */
void qlm_display_settings_gsern(int qlm, int qlm_lane, bool show_tx,
				bool show_rx, char *buf, int size);

/*
 * Some QLM speeds need to override the default tuning parameters
 *
 * @param qlm	  QLM to configure
 * @param lane	 Lane to tune, or -1 for all
 * @param mode	 Desired mode
 * @param baud_mhz Desired speed
 */
void qlm_tune_gsern(int qlm, int lane, qlm_modes_t mode, int baud_mhz);

/*
 * Called to initialize the GSERN programming API
 */
void qlm_init_gsern(void);

/*
 * Call GSERN APIs for CN9XXX chips
 *
 * @param qlm	  QLM to configure
 * @param lane	 Lane inside the QLM
 * @param mode	 Desired mode
 * @param baud_mhz Desired speed
 * @param flags	Flags to specify mode specific options
 *
 * @return Zero on success, negative on failure
 */
int qlm_set_mode_gsern(int qlm, int lane, qlm_modes_t mode, int baud_mhz,
	qlm_mode_flags_t flags);

/**
 * Capture an eye diagram
 *
 * @param node	 Node to access
 * @param qlm	  QLM to access
 * @param lane	 Lane to access
 * @param show_data Set to non-zero to have the eye capture dump the raw eye data as
 *				  it is captured
 * @param eye_data Location where the eye data will be stored
 *
 * @return Zero on success, negative on failure
 */
int gsern_eye_capture(int qlm, int lane, int show_data, gser_qlm_eye_t *eye_data);

/**
 * Manually turn on or off the SERDES transmitter
 *
 * @param node	  Node to use in numa setup
 * @param qlm	   QLM to use
 * @param lane	  Which lane
 * @param enable_tx True to enable transmitter, false to disable
 */
int qlm_tx_control_gsern(int qlm, int lane, int enable_tx);

#endif /* _QLM_GSERN_H_ */
