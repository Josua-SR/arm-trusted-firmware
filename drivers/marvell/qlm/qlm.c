/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <stdlib.h>
#include <octeontx_common.h>
#include <qlm/qlm.h>

#include "cavm-csrs-cgx.h"

static const struct qlm_mode_strmap_s qlmmode_strmap[] = {
	/* These modes are defined to have a 1 to 1 reflection from the
	 * QLM/LANE mode to the array index.
	 */
	{-1, 0, "DISABLED", NULL},
	{-1, 0, "PCIE_X1", NULL},
	{-1, 0, "PCIE_X2", NULL},
	{-1, 0, "PCIE_X4", NULL},
	{-1, 0, "PCIE_X8", NULL},
	{-1, 0, "PCIE_X16", NULL},
	{-1, 0, "SATA", NULL},
	/* CGX/LMAC types. */
	{CAVM_CGX_LMAC_TYPES_E_SGMII, 1250, "SGMII", "sgmii"},
	{CAVM_CGX_LMAC_TYPES_E_SGMII, 1250, "1G_X", "sgmii"},
	{CAVM_CGX_LMAC_TYPES_E_QSGMII, 1250, "QSGMII", "qsgmii"},
	{CAVM_CGX_LMAC_TYPES_E_XAUI, 10312, "XAUI", "xaui"},
	{CAVM_CGX_LMAC_TYPES_E_RXAUI, 10312, "RXAUI", "rxaui"},
	{CAVM_CGX_LMAC_TYPES_E_TENG_R, 10312, "XFI", "10g"},
	{CAVM_CGX_LMAC_TYPES_E_TENG_R, 10312, "SFI", "10g"},
	{CAVM_CGX_LMAC_TYPES_E_FORTYG_R, 10312, "XLAUI", "40g"},
	{CAVM_CGX_LMAC_TYPES_E_FORTYG_R, 10312, "XLAUI_C2M", "40g"},
	{CAVM_CGX_LMAC_TYPES_E_TENG_R, 10312, "10G_KR", "10g"},
	{CAVM_CGX_LMAC_TYPES_E_FORTYG_R, 10312, "40G_CR4", "40g"},
	{CAVM_CGX_LMAC_TYPES_E_FORTYG_R, 10312, "40G_KR4", "40g"},
	{CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R, 20625, "20GAUI_C2C", "20gaui"},
	{CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R, 25781, "25GAUI_C2C", "25g"},
	{CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R, 25781, "25GAUI_C2M", "25g"},
	{CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R, 25781, "25G_CR", "25g"},
	{CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R, 25781, "25G_KR", "25g"},
	/* 25GAUI_2_C2C uses a non-standard PCS:  25GBASE-R2, which is a scaled
	 * down version of 50GBASE-R2.  T9x was not designed to support
	 * 25GBASE-R2, but T9x can be hacked to support it by making CGX use
	 * LMAC_TYPES_E_FIFTYG_R and setting GSER speed to 12.890 Gbd.
	 */
	{CAVM_CGX_LMAC_TYPES_E_FIFTYG_R, 12890, "25GAUI_2_C2C", "25g"},
	{CAVM_CGX_LMAC_TYPES_E_FIFTYG_R, 20625, "40GAUI_2_C2C", "40gaui"},
	{CAVM_CGX_LMAC_TYPES_E_FIFTYG_R, 25781, "50GAUI_2_C2C", "50g"},
	{CAVM_CGX_LMAC_TYPES_E_FIFTYG_R, 25781, "50GAUI_2_C2M", "50g"},
	{CAVM_CGX_LMAC_TYPES_E_FIFTYG_R, 25781, "50G_CR2", "50g"},
	{CAVM_CGX_LMAC_TYPES_E_FIFTYG_R, 25781, "50G_KR2", "50g"},
	/* 50GAUI_4_C2C uses a non-standard PCS:  50GBASE-R4, which is a scaled
	 * up version of 40GBASE-R4.  T9x was not designed to support
	 * 50GBASE-R4, but T9x can be hacked to support it by making CGX use
	 * LMAC_TYPES_E_FORTYG_R and setting GSER speed to 12.890 Gbd.
	 */
	{CAVM_CGX_LMAC_TYPES_E_FORTYG_R, 12890, "50GAUI_4_C2C", "50g"},
	{CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R, 20625, "80GAUI_4_C2C", "80gaui"},
	{CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R, 25781, "CAUI_4_C2C", "100g"},
	{CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R, 25781, "CAUI_4_C2M", "100g"},
	{CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R, 25781, "100G_CR4", "100g"},
	{CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R, 25781, "100G_KR4", "100g"},
	{CAVM_CGX_LMAC_TYPES_E_USXGMII, 20625, "USXGMII_4X1", "usxgmii"},
	{CAVM_CGX_LMAC_TYPES_E_USXGMII, 20625, "USXGMII_2X1", "usxgmii"},
	{CAVM_CGX_LMAC_TYPES_E_USXGMII, 10312, "USXGMII_1X1", "usxgmii"},
	{-1, 0, NULL, NULL}
};

const struct qlm_mode_strmap_s qlm_get_mode_strmap(int qlm_mode)
{
	return qlmmode_strmap[qlm_mode];
}

