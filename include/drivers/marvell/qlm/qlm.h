/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef _QLM_H_
#define _QLM_H_

#include <cassert.h>

/* Default value of QLM-VOLTAGE.N0 property */
#define QLM_DEFAULT_VOLTAGE	900

/* Perform RX adaptation by default */
#define QLM_DEFAULT_RX_ADAPTATION 1

/*
 * Define different QLM-MODE variants. These definitions based on the BDK code.
 */
typedef enum {
	/* QLM is disabled (all chips) */
	QLM_MODE_DISABLED,
	/* PCIe modes  */
	/* 1 PCIe, 1 lane. Other lanes unused */
	QLM_MODE_PCIE_X1,
	/* 1 PCIe, 2 lanes */
	QLM_MODE_PCIE_X2,
	/* 1 PCIe, 4 lanes */
	QLM_MODE_PCIE_X4,
	/* 1 PCIe, 8 lanes */
	QLM_MODE_PCIE_X8,
	/* 1 PCIe, 16 lanes (CN93XX) */
	QLM_MODE_PCIE_X16,
	/* SATA modes  */
	/* SATA, each lane independent (CN81xx) */
	QLM_MODE_SATA,
	/* BGX / CGX modes */
	/* SGMII, each lane independent (CN81xx) */
	QLM_MODE_SGMII,
	/* 1000BASE-X, each lane independent */
	QLM_MODE_1G_X,
	/* QSGMII is 4 SGMII on one lane (CN81xx) */
	QLM_MODE_QSGMII,
	/* 1 XAUI or DXAUI, 4 lanes (CN81xx), use gbaud to tell difference */
	QLM_MODE_XAUI,
	/* 2 RXAUI, 2 lanes each (CN81xx) */
	QLM_MODE_RXAUI,
	/* 4 XFI, 1 lane each (CN81xx) */
	QLM_MODE_XFI,
	/* 4 SFI, 1 lane each (CN93xx) */
	QLM_MODE_SFI,
	/* 1 XLAUI chip-to-chip, 4 lanes each (CN81xx) */
	QLM_MODE_XLAUI,
	/* 1 XLAUI chip-to-module, 4 lanes each (CN93xx) */
	QLM_MODE_XLAUI_C2M,
	/* 4 10GBASE-KR, 1 lane each */
	QLM_MODE_10G_KR,
	/* 1 40GBASE-CR4, 4 lanes each */
	QLM_MODE_40G_CR4,
	/* 1 40GBASE-KR4, 4 lanes each */
	QLM_MODE_40G_KR4,
	/* 20GAUI chip to chip, 1 lane each (CN9XXX) */
	QLM_MODE_20GAUI_C2C,
	/* 25G chip-to-chip, 1 lane each (CN93XX QLMs) */
	QLM_MODE_25GAUI_C2C,
	/* 25G chip-to-module, 1 lane each (CN93XX QLMs) */
	QLM_MODE_25GAUI_C2M,
	/* 25GBASE-CR, 1 lane each */
	QLM_MODE_25G_CR,
	/* 25GBASE-KR, 1 lane each */
	QLM_MODE_25G_KR,
	/* 25GAUI-2 chip to chip, 2 lanes each (CN9XXX) */
	QLM_MODE_25GAUI_2_C2C,
	/* 40GAUI-2 chip to chip, 2 lanes each (CN9XXX) */
	QLM_MODE_40GAUI_2_C2C,
	/* 50G chip to chip, 2 lanes each (CN93XX QLMs) */
	QLM_MODE_50GAUI_2_C2C,
	/* 50G chip to module, 2 lanes each (CN93XX QLMs) */
	QLM_MODE_50GAUI_2_C2M,
	/* 50GBASE-CR2, 2 lanes each */
	QLM_MODE_50G_CR2,
	/* 50GBASE-KR2, 2 lanes each */
	QLM_MODE_50G_KR2,
	/* 50GAUI-2 chip to chip, 4 lanes each (CN9XXX) */
	QLM_MODE_50GAUI_4_C2C,
	/* 80GAUI-2 chip to chip, 4 lanes each (CN9XXX) */
	QLM_MODE_80GAUI_4_C2C,
	/* 100G chip-to-chip, 4 lanes each (CN93XX) */
	QLM_MODE_CAUI_4_C2C,
	/* 100G chip-to-module, 4 lanes each (CN93XX) */
	QLM_MODE_CAUI_4_C2M,
	/* 100GBASE-CR4, 4 lanes each */
	QLM_MODE_100G_CR4,
	/* 1 100GBASE-KR4, 4 lanes each */
	QLM_MODE_100G_KR4,
	/* USXGMII, 4 over 1 lane, 10M, 100M, 1G, 2.5G, 5G (CN93XX QLMs) */
	QLM_MODE_USXGMII_4X1,
	/* USXGMII, 2 over 1 lane, 10M, 100M, 1G, 2.5G, 5G, 10G (CN93XX QLMs) */
	QLM_MODE_USXGMII_2X1,
	/*
	 * USXGMII, 1 over 1 lane, 10M, 100M, 1G, 2.5G, 5G, 10G,
	 * 20G (CN93XX QLMs)
	 */
	QLM_MODE_USXGMII_1X1,
	QLM_MODE_LAST,
} qlm_modes_t;

/*
 * This structure is stored in GSERNX_LANEX_SCRATCHX to remember the mode of a
 * single lane. Based on the BDK structure;
 */
typedef union {
	uint64_t u;
	struct {
		/* Baudrate of the lane in MHz */
		uint16_t baud_mhz;
		qlm_modes_t mode: 8;	/* Mode of the lane */
		uint32_t flags: 8;		/* Mode flags */
		/* Mode is PCIE RC or endpoint, see flags */
		uint32_t pcie: 1;
		uint32_t sata: 1;		/* Mode is SATA */
		/* Mode is supported by CGX, see mode for details */
		uint32_t cgx: 1;
		uint32_t reserved: 29;		/* Reserved for future use */
	} s;
} qlm_state_lane_t;

typedef enum {
	QLM_MODE_FLAG_NONE = 0,     /* No flags */
	QLM_MODE_FLAG_ENDPOINT = 1, /* PCIe in EP instead of RC */
} qlm_mode_flags_t;

typedef enum {
	QLM_DIRECTION_TX = 1,
	QLM_DIRECTION_RX = 2,
	QLM_DIRECTION_BOTH = 3,
} qlm_direction_t;

typedef enum {
	/* No loopback */
	QLM_LOOP_DISABLED,
	/* Loop external data RX->TX (Not supported on CN8XXX) */
	QLM_LOOP_SHALLOW,
	/*
	 * Loop internal data TX->RX in analog domain
	 * (Not supported on CN8XXX)
	 */
	QLM_LOOP_NEAR_END,
	/* Loop internal data TX->RX in digital domain */
	QLM_LOOP_CORE,
} qlm_loop_t;

typedef enum {
	QLM_GSERC_TYPE,
	QLM_GSERR_TYPE,
	QLM_GSERN_TYPE,
} qlm_type_t;

/*
 * Eye diagram captures are stored in the following structure
 */
typedef struct {
	int width;              /* Width in the x direction (time) */
	int height;             /* Height in the y direction (voltage) */
	uint32_t data[64][128]; /* Error count at location, saturates as max */
	qlm_type_t type;
} gser_qlm_eye_t;

#ifdef DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS
CASSERT(SERDES_EYE_DATA_SIZE >= sizeof(gser_qlm_eye_t),
	assert_serdes_eye_data_size_to_small);
#endif /* DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS */

struct qlm_mode_strmap_s {
	int mode;
	int baud_rate;
	char *bdk_str;
	char *linux_str;
};

/* QLM APIs */

/*
 * Setup the PEM to either driver or receive reset from PRST based on RC or EP
 *
 * @param pem	Which PEM to setup
 * @param is_endpoint
 *			   Non zero if PEM is a EP
 */
static inline qlm_state_lane_t qlm_build_state(qlm_modes_t mode, int baud_mhz,
	qlm_mode_flags_t flags)
{
	qlm_state_lane_t state;

	state.u = 0;
	state.s.mode = mode;
	state.s.baud_mhz = baud_mhz;
	state.s.flags = flags;
	state.s.pcie =
		(mode > QLM_MODE_DISABLED) && (mode <= QLM_MODE_PCIE_X16);
	state.s.sata = (mode == QLM_MODE_SATA);
	state.s.cgx = (mode >= QLM_MODE_SGMII) && (mode < QLM_MODE_LAST);
	return state;
}

const struct qlm_mode_strmap_s qlm_get_mode_strmap(int qlm_mode);

typedef struct {
	qlm_type_t type;
	qlm_state_lane_t (*qlm_get_state)(int qlm, int lane);
	void (*qlm_set_state)(int qlm, int lane, qlm_state_lane_t state);
	int (*qlm_set_mode)(int qlm, int lane, qlm_modes_t mode, int baud_mhz,
			qlm_mode_flags_t flags);
	int (*qlm_measure_refclock)(int qlm);
	int (*qlm_reset)(int qlm);
	int (*qlm_enable_prbs)(int qlm, int prbs, qlm_direction_t dir);
	int (*qlm_disable_prbs)(int qlm);
	uint64_t (*qlm_get_prbs_errors)(int qlm, int lane, int clear);
	void (*qlm_inject_prbs_error)(int qlm, int lane);
	int (*qlm_enable_loop)(int qlm, qlm_loop_t loop);
	int (*qlm_tune_lane_tx)(int qlm, int lane, int tx_cmain, int tx_cpre,
			int tx_cpost, int tx_bs, int tx_unused);
	int (*qlm_get_tune_lane_tx)(int qlm, int lane, int *tx_cmain,
			int *tx_cpre, int *tx_cpost, int *tx_bs,
			int *tx_unused);
	int (*qlm_rx_equalization)(int qlm, int qlm_lane);
	void (*qlm_display_settings)(int qlm, int qlm_lane, bool show_tx,
			bool show_rx, char *buf, int size);
	int (*qlm_eye_capture)(int qlm, int lane, int show_data,
			gser_qlm_eye_t *eye_data);
	void (*qlm_tx_control)(int qlm, int lane, bool enable_tx);
} qlm_ops_t;

/* QLM platform specific API */

/*
 * Return struct qlm_ops_t with pointer to functions for specific qlm.
 * It may modify qlm index, and this new qlm value should be use as argument
 * for functions from qlm_ops_t.
 */
const qlm_ops_t *plat_otx2_get_qlm_ops(int *qlm);

#endif /* _QLM_H_ */
