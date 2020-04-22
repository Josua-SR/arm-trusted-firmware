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
 * Structure to define default tuning parameters for GSERR, GSERC, and GSERJ.
 */
typedef struct
{
	qlm_modes_t mode : 16;
	uint16_t baud_mhz;
	uint8_t tx_swing;
	uint8_t tx_cpre;
	uint8_t tx_cpost;
} default_tuning_t;

static const default_tuning_t DEF_TUNING[] = {
	{QLM_MODE_SGMII,		 1250,  3,  0,  0 },
	{QLM_MODE_1G_X,		  1250,  7,  0,  0 },
	{QLM_MODE_QSGMII,		5000,  3,  0,  0 },
	{QLM_MODE_XAUI,		  3125,  7,  0,  0 },
	{QLM_MODE_XAUI,		  6250,  7,  0,  0 },
	{QLM_MODE_RXAUI,		 6250,  7,  0,  0 },
	{QLM_MODE_XFI,		  10312,  3,  0,  0 },
	{QLM_MODE_SFI,		  10312,  7,  3,  3 },
	{QLM_MODE_XLAUI,		10312,  7,  0, 10 },
	{QLM_MODE_XLAUI_C2M,	10312,  7,  0,  7 },
	//{QLM_MODE_10G_KR,	   10312,  7,  0,  0 },
	//{QLM_MODE_40G_CR4,	  10312,  7,  0,  0 },
	//{QLM_MODE_40G_KR4,	  10312,  7,  0,  0 },
	{QLM_MODE_20GAUI_C2C,   20625,  7,  3,  0 },
	{QLM_MODE_25GAUI_C2C,   25781,  7,  3,  0 },
	{QLM_MODE_25GAUI_C2M,   25781,  7,  3,  0 },
	//{QLM_MODE_25G_CR,	   25781,  7,  3,  0 },
	//{QLM_MODE_25G_KR,	   25781,  7,  3,  0 },
	{QLM_MODE_40GAUI_2_C2C, 20625,  7,  3,  0 },
	{QLM_MODE_50GAUI_2_C2C, 25781,  7,  3,  0 },
	{QLM_MODE_50GAUI_2_C2M, 25781,  7,  3,  0 },
	//{QLM_MODE_50G_CR2,	  25781,  7,  3,  0 },
	//{QLM_MODE_50G_KR2,	  25781,  7,  3,  0 },
	{QLM_MODE_80GAUI_4_C2C, 20625,  7,  3,  0 },
	{QLM_MODE_CAUI_4_C2C,   25781,  7,  3,  0 },
	{QLM_MODE_CAUI_4_C2M,   25781,  7,  3,  0 },
	//{QLM_MODE_100G_CR4,	 25781,  7,  3,  0 },
	//{QLM_MODE_100G_KR4,	 25781,  7,  3,  0 },
	{QLM_MODE_USXGMII_4X1,  20625,  7,  3,  0 },
	{QLM_MODE_USXGMII_2X1,  20625,  7,  3,  0 },
	{QLM_MODE_USXGMII_1X1,  10312,  7,  0,  0 },
	{QLM_MODE_DISABLED,		 0,  0,  0,  0 }
};

/**
 * Return the default tuning parameters for the associated mode and speed when
 * programmed on GSERR, GSERC, or GSERJ. If there are no defaults, the passed
 * parameters are not updated.
 *
 * @param mode	 QLM mode to be found
 * @param baud_mhz Speed of the mode
 * @param tx_swing Recommended TX swing
 * @param tx_cpre  Recommended TX PRE
 * @param tx_cpost Recommended TX POST
 */
void qlm_gserx_default_tuning(qlm_modes_t mode, int baud_mhz, int *tx_swing, int *tx_cpre, int *tx_cpost)
{
	const default_tuning_t *tuning = DEF_TUNING;
	while (tuning->mode)
	{
		if ((tuning->mode == mode) && (tuning->baud_mhz == baud_mhz))
			break;
		tuning++;
	}
	if (tuning->mode)
	{
		*tx_swing = tuning->tx_swing;
		*tx_cpre = tuning->tx_cpre;
		*tx_cpost = tuning->tx_cpost;
	}
}

