/*
 * Copyright (C) 2016-2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __PLAT_BOARD_CFG_H__
#define __PLAT_BOARD_CFG_H__

#include <platform_dt.h>
#include <cgx.h>
#include <qlm/qlm.h>
#include <sfp_mgmt.h>
#include <phy_mgmt.h>
#include <platform_scfg.h>
#include <octeontx_board_cfg.h>

typedef enum ccs_region_index {
	SECURE_NONPRESERVE,
	NSECURE_NONPRESERVE,
	SECURE_PRESERVE, /* used as ARM Trace Buffer */
	NSECURE_PRESERVE,
	CCS_REGION_IDX_MAX,
} ccs_region_index_t;

typedef enum {
	SW_RVU_MAP_NONE,
	SW_RVU_MAP_LEGACY,
	SW_RVU_MAP_AVAILABLE,
	SW_RVU_MAP_FORCE,
} sw_rvu_mapping;

typedef struct rvu_sw_rvu_pf {
	int num_rvu_vfs;
	int num_msix_vec;
	sw_rvu_mapping mapping;
} rvu_sw_rvu_pf_t;

/* default, if not defined by platform */
#ifndef SW_RVU_SSO_TIM_NUM_PF
#define SW_RVU_SSO_TIM_NUM_PF 1
#endif

/* default, if not defined by platform */
#ifndef SW_RVU_NPA_NUM_PF
#define SW_RVU_NPA_NUM_PF     1
#endif

/* default, if not defined by platform */
#ifndef SW_RVU_SDP_NUM_PF
#define SW_RVU_SDP_NUM_PF     0
#endif

/* default, if not defined by platform */
#ifndef SW_RVU_CPT_NUM_PF
#define SW_RVU_CPT_NUM_PF     1
#endif

typedef enum {
	SW_RVU_NODEV = 0, /* 0 is reserved */
	SW_RVU_SSO_TIM_BASE,
	SW_RVU_NPA_BASE = SW_RVU_SSO_TIM_BASE + SW_RVU_SSO_TIM_NUM_PF,
	SW_RVU_SDP_BASE = SW_RVU_NPA_BASE + SW_RVU_NPA_NUM_PF,
	SW_RVU_CPT_BASE = SW_RVU_SDP_BASE + SW_RVU_SDP_NUM_PF,
} sw_rvu_pfs;

/*
 * Implementation note: the 'SW_RVU_xxx_PF' values do not correspond directly
 * to RVU PF IDs; rather, they are simply an identifier (ID).
 * If a platform defines 0 instances of a device, the ID == 0.
 * See 'find_sw_rvu_dev()'.
 */
#define SW_RVU_SSO_TIM_PF(n) (!SW_RVU_SSO_TIM_NUM_PF ? SW_RVU_NODEV : \
			      SW_RVU_SSO_TIM_BASE + (n))
#define SW_RVU_NPA_PF(n)     (!SW_RVU_NPA_NUM_PF ? SW_RVU_NODEV : \
			      SW_RVU_NPA_BASE + (n))
#define SW_RVU_SDP_PF(n)     (!SW_RVU_SDP_NUM_PF ? SW_RVU_NODEV : \
			      SW_RVU_SDP_BASE + (n))
#define SW_RVU_CPT_PF(n)     (!SW_RVU_CPT_NUM_PF ? SW_RVU_NODEV : \
			      SW_RVU_CPT_BASE + (n))

#define SW_RVU_NUM_PF        (SW_RVU_SSO_TIM_NUM_PF \
			      + SW_RVU_NPA_NUM_PF \
			      + SW_RVU_SDP_NUM_PF \
			      + SW_RVU_CPT_NUM_PF \
			      + 1 /* for SW_RVU_NODEV */)

typedef struct rvu_config {
	int valid;
	int cpt_dis;	/* to indicate if CPT block is not present */
	rvu_sw_rvu_pf_t admin_pf;
	rvu_sw_rvu_pf_t sw_pf[SW_RVU_NUM_PF];
} rvu_config_t;

/* Define LMAC structure. */
typedef struct cgx_lmac_config {
	/* for RVU */
	int num_rvu_vfs;
	int num_msix_vec;
	/* below info to be filled by
	 * FDT parser during boot time or mode
	 * change
	 */
	int lane_to_sds;
	int use_training;
	int mode;		/* octeontx_qlm_mode enum type */
	int mode_idx;		/* helper field for the mode mapping */
	int qlm;		/* from BDK DT */
	int lane;		/* from BDK DT */
	int first_phy_lane;	/* 1st Physical lane */
	/* NOTE: when this bit is set, it doesn't necessarily
	 * mean the link is up until the user sends LINK UP command
	 */
	int lmac_enable;
	int phy_mode;		/* MAC or PHY mode for SGMII */
	int sgmii_1000x_mode;	/* SGMII or 1000x mode for SGMII */
	int phy_present;
	int sfp_slot;
	int autoneg_dis;
	int fec;
#ifdef DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS
	int prbs_errors_host;
	int prbs_errors_line;
#endif /* DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS */
	uint8_t local_mac_address[6];
	phy_config_t phy_config;
	sfp_slot_info_t sfp_info;
	uint64_t supported_link_modes;	/* cgx_mode_t enum */
	uint32_t lane_mask;
	/*
	 * Shift from first qlm and gser is always the same it can be easily
	 * calculated by adding shift value to qlm/gser.
	 */
	int shift_from_first;
	int gserx;
	int max_lane_count;
	int an_loopback;
	int lane_an_master;
} cgx_lmac_config_t;

typedef enum {
	NIX0 = 1,
	NIX1 = 2
} nix_block_t;

typedef struct cgx_config {
	uint32_t lmac_count:4;
	uint32_t lmacs_used:4;
	uint32_t usxgmii_mode:1; /* global - per CGX */
	/* for RVU */
	uint32_t enable:1;
	uint32_t is_rfoe:1;
	uint16_t lanes_used_mask;
	uint16_t network_lane_order;
	cgx_lmac_config_t lmac_cfg[MAX_LMAC_PER_CGX];
	const qlm_ops_t *qlm_ops;
	int load;
	nix_block_t nix_block;
} cgx_config_t;

typedef struct qlm_config {
	int lane_rx_polarity[MAX_LANES_PER_QLM];
	int lane_tx_polarity[MAX_LANES_PER_QLM];
	int lane_rx_prevga_gn_adapt[MAX_LANES_PER_QLM];
	int lane_rx_prevga_gn_ovrd[MAX_LANES_PER_QLM];
	int rx_adaptation[MAX_LANES_PER_QLM];
	int lane_idle_refset[MAX_LANES_PER_QLM];
	int is_cpri;
	int cpri_baud_rate[MAX_LANES_PER_QLM];
	int clk_term;
} qlm_config_t;

typedef struct mcc_ras_config {
	uint32_t lmcoe_count:8;
	uint32_t enabled:1;
} mcc_ras_config_t;

#define MAX_GHES_OBJ		16
#define GHES_PTR_STAT_ADDR	0
#define GHES_PTR_STATUS		1
#define GHES_PTR_RING		2
#define GHES_PTRS		3
typedef struct ras_config {
	mcc_ras_config_t mcc[MAX_MCC];

	struct fdt_ghes {
		uint32_t id;
		uint32_t size[GHES_PTRS];
		void *base[GHES_PTRS];
		char name[8];
	} fdt_ghes[MAX_GHES_OBJ];
	int nr_ghes;
} ras_config_t;

typedef struct plat_octeontx_board_cfg {
	board_cfg_t bcfg;
	rvu_config_t rvu_config;
	cgx_config_t cgx_cfg[MAX_CGX];
	qlm_config_t qlm_cfg[MAX_QLM];
	int qlm_voltage;
	ras_config_t ras_config;
	uint64_t pf_mac_base; /* PF MAC Address */
	int pf_mac_num;
	int qlm_auto_config;
	int show_smi_in_nsw; /* flag to show or hide SMI in non-secure world */
} plat_octeontx_board_cfg_t;

extern plat_octeontx_board_cfg_t * const plat_octeontx_bcfg;
uint64_t ccs_region_get_info(ccs_region_index_t index, uint64_t *start);

#endif /* __PLAT_BOARD_CFG_H__ */
