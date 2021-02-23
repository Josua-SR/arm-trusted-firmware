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
	/* CPRI, not eCPRI (CN9XXX BPHY) */
	QLM_MODE_CPRI,
	/* JESD204B (CN9XXX BPHY) */
	QLM_MODE_JESD204B,
	/* JESD204C (CN9XXX BPHY) */
	QLM_MODE_JESD204C,
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

/*
 * The following structure is used to store the 802.3 autonegotiation advertisement data.
 * This includes the advertised Technology/protocol, FEC ability and FEC request
 */
typedef union {
	uint64_t u;
	struct {
		uint32_t  an_1000base_kx : 1;      /* Advertise 1000BASE-KX support */
		uint32_t  an_10gbase_kx4 : 1;      /* Advertise 10GBASE-KR support */
		uint32_t  an_10gbase_kr : 1;       /* Advertise 10GBASE-KR support */
		uint32_t  an_40gbase_kr4 : 1;      /* Advertise 40GBASE-KR4 support */
		uint32_t  an_40gbase_cr4 : 1;      /* Advertise 40GBASE-CR4 support */
		uint32_t  an_100gbase_cr10 : 1;    /* Advertise 100GBASE-CR10 support */
		uint32_t  an_100gbase_kp4 : 1;     /* Advertise 100GBASE-KP4 support */
		uint32_t  an_100gbase_kr4 : 1;     /* Advertise 100GBASE-KR4 support */
		uint32_t  an_100gbase_cr4 : 1;     /* Advertise 100GBASE-CR4 support */
		uint32_t  an_25gbase_kcrs : 1;     /* Advertise 25GBASE-KR/CR-S(short) support */
		uint32_t  an_25gbase_kcr : 1;      /* Advertise 25GBASE-KR/CR support */
		uint32_t  an_2_5gbase_kx : 1;      /* Advertise 2.5GBASE-KX support */
		uint32_t  an_5gbase_kr : 1;        /* Advertise 5GBASE-KR support */
		uint32_t  an_50gbase_kcr : 1;      /* Advertise 50GBASE-KR/CR support */
		uint32_t  an_100gbase_kcr2 : 1;    /* Advertise 100GBASE-KR2/CR2 support */
		uint32_t  an_200gbase_kcr4 : 1;    /* Advertise 200GBASE-KR4/CR4 support */
		uint32_t  an_25gbase_kr_cons : 1;  /* Advertise 25GBASE-KR Consortium support */
		uint32_t  an_25gbase_cr_cons : 1;  /* Advertise 25GBASE-CR Consortium support */
		uint32_t  an_50gbase_kr2_cons : 1; /* Advertise 50GBASE-KR2 Consortium support */
		uint32_t  an_50gbase_cr2_cons : 1; /* Advertise 50GBASE-CR2 Consortium support */
		uint32_t  fec_10g_abil : 1;        /* Advertise FEC ability (10Gb/s per lane only) */
		uint32_t  fec_25g_rs_abil : 1;     /* Advertise 25Gb/s per lane RS-FEC ability (Consortium Only) */
		uint32_t  fec_25g_baser_abil : 1;  /* Advertise 25Gb/s per lane BASE-R ability (Consortium Only) */
		uint32_t  fec_10g_req : 1;         /* Advertise 10Gb/s per lane BASE-R FEC requested */
		uint32_t  fec_25g_rs : 1;          /* Advertise 25Gb/s per lane RS-FEC requested */
		uint32_t  fec_25g_baser : 1;       /* Advertise 25Gb/s per lane BASE-R FEC requested */
		uint32_t  fec_25g_rs_cons : 1;     /* Advertise 25Gb/s per lane RS-FEC requested (Consortium Only) */
		uint32_t  fec_25g_baser_cons : 1;  /* Advertise 25Gb/s per lane BASE-R FEC requested (Consortium Only) */
		uint32_t  fc_pause : 1;            /* Advertise Symmetric pause capability */
		uint32_t  fc_asm_dir : 1;          /* Advertise support for Asymmetric pause */
		uint64_t  reserved: 34;		   /* Reserved for future use */
	} s;
} ap_802_3_adv_t;

typedef enum {
	QLM_802_3AP_FEC_DISABLED = 0,     /* 802.3AP No FEC requested */
	QLM_802_3AP_FEC_BASER = 1,        /* 802.3AP BASE-R FEC requested */
	QLM_802_3AP_FEC_RSFEC = 2,        /* 802.3AP RS-FEC requested */
	QLM_802_3AP_FEC_BASER_RSFEC = 3,  /* 802.3AP BASE-R and RS-FEC requested */
} qlm_802_3ap_fec_t;

typedef enum {
	QLM_MODE_FLAG_NONE = 0,     /* No flags */
	QLM_MODE_FLAG_ENDPOINT = 1, /* PCIe in EP instead of RC */
#if 0
	QLM_MODE_FLAG_LEQ = 2,      /* Used for disabling LEQ adaptation */
	QLM_MODE_FLAG_DFE = 3,      /* Used for disabling DFE adaptation */
#endif
} qlm_mode_flags_t;

typedef enum {
	QLM_DIRECTION_TX = 1,
	QLM_DIRECTION_RX = 2,
	QLM_DIRECTION_BOTH = 3,
} qlm_direction_t;

typedef enum
{
    QLM_LOOP_DISABLED,  /* No loopback */
    QLM_LOOP_FEA,  /* Loop Far End Analog (Not supported on CN8XXX) */
    QLM_LOOP_NEA,  /* Loop Near End Analog (Not supported on CN8XXX) */
    QLM_LOOP_NED,  /* Loop Near End Digital (Not supported on CN8XXX) */
    QLM_LOOP_FED,  /* Loop Far End Digital (Not supported on CN8XXX) */
} qlm_loop_t;

typedef enum {
	QLM_GSERC_TYPE = 0,
	QLM_GSERR_TYPE,
	QLM_GSERN_TYPE,
	QLM_GSERJ_TYPE,
	QLM_TYPE_END
} qlm_type_t;

typedef enum {
	QLM_LANE_AN_DIS = 0,
	QLM_LANE_AN_SLAVE = 1,
	QLM_LANE_AN_MASTER = 2,
} qlm_lane_an_mode_t;

typedef enum {
	LT_INIT_STATE = 0,
	LT_PRESET_STATE = 1,
	LT_INIT_UNCHANGED = 2,
} qlm_lt_init_state_t;

typedef enum {
	AN_CFG_CMD_START = 0x91,
	AN_CFG_CMD_CONT = 0x92,
} qlm_an_cfg_cmd_type_t;

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
	int eth_link_speed; /* in units of Gbps */
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
	int (*qlm_cfg_mode)(int module, uint8_t lane_mask, qlm_modes_t mode, int baud_mhz,
			    qlm_mode_flags_t flags, int lane_an_master,
			    qlm_802_3ap_fec_t fec_types, qlm_lt_init_state_t lt_init,
			    int ignore_mode_chk);
	int (*qlm_measure_refclock)(int qlm);
	int (*qlm_reset)(int qlm);
	int (*qlm_enable_prbs)(int qlm, int prbs, qlm_direction_t dir, int lane);
	int (*qlm_disable_prbs)(int qlm, int lane);
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
	int (*qlm_tx_control)(int qlm, int lane, int enable_tx);
	int (*qlm_tx_sm_rst_control)(int qlm, uint64_t lane_mask, int reset_tx);
	int (*qlm_rx_signal_detect)(int qlm, int lane);
	int (*qlm_get_lmac_phy_lane)(int qlm, int lane, int lane_to_sds);
	int (*qlm_link_training_fail)(int qlm, int lane);
	int (*qlm_link_training_complete)(int qlm, int lane);
	void (*qlm_set_phy_strap)(int qlm, int lane);
	int (*qlm_an_complete)(int qlm, int lane);
	int (*qlm_prbs_chk)(int qlm, int lane);
	int (*qlm_farend_lpbk_chk)(int qlm, int lane);
	int (*qlm_display_trace)(int qlm, int lane, int unused);
	void (*qlm_rx_leq_adapt)(int qlm, int lane, int disable, int leq_lfg_start,
		int leq_hfg_sql_start, int leq_mbf_start, int leq_mbg_start,
		int gn_apg_start);
	void (*qlm_rx_dfe_adapt)(int qlm, int lane, int disable);
	int (*qlm_ned_loopback)(int qlm, int lane, bool enable);
	int (*qlm_fea_loopback)(int qlm, int lane, bool enable);
	int (*qlm_nea_loopback)(int qlm, int lane, bool enable);
	int (*qlm_fed_loopback)(int qlm, int lane, bool enable);
	int (*qlm_mode_chg_full_reset)(int module, int baud_mhz);
	void (*qlm_cmu_reset)(int module);
} qlm_ops_t;

/* QLM platform specific API */

/*
 * Return struct qlm_ops_t with pointer to functions for specific cgx.
 */
const qlm_ops_t *plat_otx2_get_qlm_ops(int cgx_idx);
/*
 * Return gserx idx from qlm idx. If shift_from_first != NULL it returns
 * shift value to get first gserx (useful for DLM).
 * first_qlm = qlm_idx + *shift_from_first
 * first_gserx = gserx_idx + *shift_from_first
 */
int plat_otx2_get_gserx(int qlm, int *shift_from_first);

#endif /* _QLM_H_ */
