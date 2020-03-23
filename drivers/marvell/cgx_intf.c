/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

/* CGX driver for OcteonTX2 */

#include <arch.h>
#include <cassert.h>
#include <stdio.h>
#include <debug.h>
#include <string.h>
#include <platform_def.h>
#include <timers.h>
#include <octeontx_common.h>
#include <drivers/delay_timer.h>
#include <plat_board_cfg.h>
#include <cgx_intf.h>
#include <cgx.h>
#include <qlm/qlm.h>
#include <plat_scfg.h>
#include <sh_fwdata.h>
#include <gser_internal.h>
#include <platform_setup.h>
#include <gsern/gsern.h>
#include <smi.h>

#ifdef NT_FW_CONFIG
#include <plat_npc_mcam_profile.h>
#endif

#include "cavm-csrs-cgx.h"

/* define DEBUG_ATF_CGX_INTF to enable debug logs */
#undef DEBUG_ATF_CGX_INTF

#ifdef DEBUG_ATF_CGX_INTF
#define debug_cgx_intf printf
#else
#define debug_cgx_intf(...) ((void) (0))
#endif

#define MAX_CGX_TIMERS 2

static int cgx_timers[MAX_CGX_TIMERS];

static cgx_lmac_context_t
		lmac_context[MAX_CGX][MAX_LMAC_PER_CGX];

static cgx_lmac_timers_t
		lmac_timers[MAX_CGX][MAX_LMAC_PER_CGX];

static const cgx_speed_mode_map speed_mode_map[] = {
	{CAVM_CGX_LMAC_TYPES_E_SGMII, 0, QLM_MODE_SGMII, CGX_FEC_NONE, 1250, (1 << CGX_MODE_SGMII_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_SGMII, 0, QLM_MODE_SGMII, CGX_FEC_NONE, 1250, (1 << CGX_MODE_SGMII_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_SGMII, 0, QLM_MODE_SGMII, CGX_FEC_NONE, 1250, (1 << CGX_MODE_SGMII_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_SGMII, 0, QLM_MODE_1G_X, CGX_FEC_NONE, 1250, (1 << CGX_MODE_1000_BASEX_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_QSGMII, 0, QLM_MODE_QSGMII, CGX_FEC_NONE, 1250, (1 << CGX_MODE_QSGMII_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_TENG_R, 0, QLM_MODE_SFI, CGX_FEC_BASE_R, 10312, (1 << CGX_MODE_10G_C2C_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_TENG_R, 0, QLM_MODE_XFI, CGX_FEC_BASE_R, 10312, (1 << CGX_MODE_10G_C2M_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_TENG_R, 1, QLM_MODE_10G_KR, CGX_FEC_BASE_R, 10312, (1 << CGX_MODE_10G_KR_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R, 0, QLM_MODE_20GAUI_C2C, (CGX_FEC_BASE_R | CGX_FEC_RS), 20625, (1 << CGX_MODE_20G_C2C_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_FIFTYG_R, 0, QLM_MODE_25GAUI_2_C2C, (CGX_FEC_BASE_R | CGX_FEC_RS), 12890, (1 << CGX_MODE_25G_2_C2C_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R, 0, QLM_MODE_25GAUI_C2C, (CGX_FEC_BASE_R | CGX_FEC_RS), 25781, (1 << CGX_MODE_25G_C2C_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R, 0, QLM_MODE_25GAUI_C2M, (CGX_FEC_BASE_R | CGX_FEC_RS), 25781, (1 << CGX_MODE_25G_C2M_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R, 0, QLM_MODE_25G_CR, (CGX_FEC_BASE_R | CGX_FEC_RS), 25781, (1 << CGX_MODE_25G_CR_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R, 1, QLM_MODE_25G_KR, (CGX_FEC_BASE_R | CGX_FEC_RS), 25781, (1 << CGX_MODE_25G_KR_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_FORTYG_R, 0, QLM_MODE_XLAUI, (CGX_FEC_BASE_R), 10312, (1 << CGX_MODE_40G_C2C_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_FORTYG_R, 0, QLM_MODE_XLAUI_C2M, (CGX_FEC_BASE_R), 10312, (1 << CGX_MODE_40G_C2M_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_FORTYG_R, 0, QLM_MODE_40G_CR4, (CGX_FEC_BASE_R), 10312, (1 << CGX_MODE_40G_CR4_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_FORTYG_R, 1, QLM_MODE_40G_KR4, (CGX_FEC_BASE_R), 10312, (1 << CGX_MODE_40G_KR4_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_FIFTYG_R, 0, QLM_MODE_40GAUI_2_C2C, (CGX_FEC_BASE_R), 20625, (1 << CGX_MODE_40GAUI_C2C_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_FIFTYG_R, 0, QLM_MODE_50GAUI_2_C2C, (CGX_FEC_BASE_R | CGX_FEC_RS), 25781, (1 << CGX_MODE_50G_C2C_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_FIFTYG_R, 0, QLM_MODE_50GAUI_2_C2M, (CGX_FEC_BASE_R | CGX_FEC_RS), 25781, (1 << CGX_MODE_50G_C2M_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_FIFTYG_R, 0, QLM_MODE_50G_CR2, (CGX_FEC_BASE_R | CGX_FEC_RS), 25781, (1 << CGX_MODE_50G_C2M_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_FIFTYG_R, 1, QLM_MODE_50G_KR2, (CGX_FEC_BASE_R | CGX_FEC_RS), 25781, (1 << CGX_MODE_50G_C2M_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_FORTYG_R, 0, QLM_MODE_50GAUI_4_C2C, (CGX_FEC_BASE_R | CGX_FEC_RS), 12890, (1 << CGX_MODE_50G_4_C2C_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R, 0, QLM_MODE_80GAUI_4_C2C, (CGX_FEC_RS), 20625, (1 << CGX_MODE_80GAUI_C2C_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R, 0, QLM_MODE_CAUI_4_C2C, (CGX_FEC_RS), 25781, (1 << CGX_MODE_100G_C2C_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R, 0, QLM_MODE_CAUI_4_C2M, (CGX_FEC_RS), 25781, (1 << CGX_MODE_100G_C2M_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R, 0, QLM_MODE_100G_CR4, (CGX_FEC_RS), 25781, (1 << CGX_MODE_100G_CR4_BIT)},
	{CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R, 0, QLM_MODE_100G_KR4, (CGX_FEC_RS), 25781, (1 << CGX_MODE_100G_KR4_BIT)},
	/* add new modes here */
	{CGX_LINK_MAX, 0, 0, QLM_MODE_DISABLED, 0, CGX_FEC_NONE},
};

static int cgx_get_error_type(int cgx_id, int lmac_id)
{
	cgx_lmac_context_t *lmac_ctx;

	lmac_ctx = &lmac_context[cgx_id][lmac_id];
	return lmac_ctx->s.error_type;
}

static int cgx_trigger_interrupt(int cgx_id, int lmac_id)
{
	debug_cgx_intf("%s %d:%d\n", __func__, cgx_id, lmac_id);

	/* Enable the interrupt bit each time before triggering
	 * an interrupt. In case of Link down request, enable bit for
	 * LMAC(CGXX_CMRX_CONFIG(enable)) is disabled
	 * which clears all the dedicated CGX context
	 * state for the LMAC. Hence, need to explicitly enable
	 * the interrupt every time.
	 */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_int_ena_w1s_t,
				CAVM_CGXX_CMRX_INT_ENA_W1S(cgx_id, lmac_id),
				overflw, 1);

	/* FIXME: trigger an interrupt to kernel
	 * via CMRX overflow int bit - Hardware-opaque vector
	 * for software driver (37th MSIX vector) doesn't have interrupt
	 * support yet. this is a workaround for the 1st pass
	 */
	CAVM_MODIFY_CGX_CSR(cavm_cgxx_cmrx_int_w1s_t,
				CAVM_CGXX_CMRX_INT_W1S(cgx_id, lmac_id),
				overflw, 1);
	return 0;
}

static int cgx_acquire_csr_lock(int cgx_id, int lmac_id)
{
	int timeout = 10; /* loop for few times but not infinitely */
	cgx_lmac_context_t *lmac_ctx;

	lmac_ctx = &lmac_context[cgx_id][lmac_id];

	do {
		if (!lmac_ctx->s.lock) {
			lmac_ctx->s.lock = 1;
			return 0;
		}
		udelay(1);
	} while (timeout--);

	return -1;
}

static void cgx_release_csr_lock(int cgx_id, int lmac_id)
{
	lmac_context[cgx_id][lmac_id].s.lock = 0;
}

static void cgx_release_own_status(int cgx_id, int lmac_id)
{
	CAVM_MODIFY_CGX_CSR(union cgx_scratchx1,
			CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 1),
			own_status, CGX_OWN_NON_SECURE_SW); /* released the ownership */
}

static int cgx_sfp_obtain_capabilities(int cgx_id, int lmac_id)
{
	int trans_type;

	/* Read the EEPROM to determine new module capabilities */
	trans_type = sfp_parse_eeprom_data(cgx_id, lmac_id);

	if (trans_type != SFP_TRANS_TYPE_NONE) {
		/* If Valid transceiver found */
		debug_cgx_intf("%s: %d:%d trans_type %d\n",
				__func__, cgx_id, lmac_id,
				trans_type);
		return sfp_validate_user_options(cgx_id, lmac_id);
	}

	debug_cgx_intf("%s: %d:%d Valid transceiver not identified\n",
		__func__, cgx_id, lmac_id);
	cgx_set_error_type(cgx_id, lmac_id, CGX_ERR_MODULE_INVALID);
	return 0;
}

static int cgx_get_mode_for_qlm_mode(int qlm_mode)
{
	int mode = -1;
	uint64_t bitmask = 0;

	for (int i = 0; i < ARRAY_SIZE(speed_mode_map); i++) {
		if (qlm_mode == speed_mode_map[i].qlm_mode) {
			bitmask = speed_mode_map[i].mode_bitmask;
			break;
		}
	}
	debug_cgx_intf("%s: mode 0x%llx qlm_mode %d\n", __func__,
		       bitmask, qlm_mode);
	if (bitmask)
		mode = __builtin_ffsl(bitmask) - 1; /* enum starts at 0 */
	return mode;
}

static void cgx_set_link_mode(int cgx_id, int lmac_id, int qlm_mode)
{
	union cgx_scratchx0 scratchx0;

	scratchx0.u = CSR_READ(CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 0));
	scratchx0.s.link_sts.mode = cgx_get_mode_for_qlm_mode(qlm_mode);
	CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 0), scratchx0.u);

}

static void cgx_set_link_state(int cgx_id, int lmac_id,
					link_state_t *link, int err_type)
{
	union cgx_scratchx0 scratchx0;

	debug_cgx_intf("%s %d:%d link_up %d speed %d duplex %d\t"
			"err_type %d fec %d\n",
			__func__, cgx_id, lmac_id,
			link->s.link_up, link->s.speed,
			link->s.full_duplex, err_type,
			link->s.fec);

	scratchx0.u = CSR_READ(CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 0));
	scratchx0.s.link_sts.link_up = link->s.link_up;
	scratchx0.s.link_sts.speed = link->s.speed;
	scratchx0.s.link_sts.full_duplex = link->s.full_duplex;
	scratchx0.s.link_sts.err_type = err_type;
	scratchx0.s.link_sts.fec = link->s.fec;
	CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 0), scratchx0.u);

	/* Update supported FEC to SM when updating link status */
	sh_fwdata_update_supported_fec(cgx_id, lmac_id);
}

static int cgx_link_change_req(int cgx_id, int lmac_id)

{
	int ret = 0, err_type = 0;
	union cgx_scratchx0 scratchx0;
	link_state_t link;
	cgx_lmac_config_t *lmac_cfg;
	cgx_lmac_context_t *lmac_ctx;

	/* get the lmac type and based on lmac
	 * type, change the link settings
	 */
	lmac_cfg = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];
	lmac_ctx = &lmac_context[cgx_id][lmac_id];

	debug_cgx_intf("%s %d:%d lmac_type %d\n", __func__, cgx_id,
			lmac_id, lmac_cfg->mode);

	/* In case of link change from down -> up, do the necessary
	 * HW configuration to initialize the link again. For >=10G interfaces,
	 * if the link is down, set up link is already called once. If the link
	 * has changed from up -> down, just update the status. no HW
	 * re-configuration is required.
	 */
	link.s.link_up = lmac_ctx->s.link_up;
	link.s.speed = lmac_ctx->s.speed;
	link.s.full_duplex = lmac_ctx->s.full_duplex;
	link.s.fec = lmac_ctx->s.fec;

	/* clear the previous errors before changing the link */
	cgx_set_error_type(cgx_id, lmac_id, 0);

	if (link.s.link_up) {
		if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) ||
			(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
			if (cgx_sgmii_set_link_speed(cgx_id, lmac_id, &link) != 0)
				ret = -1;
			/* Check for AN CPT */
			if (cgx_sgmii_check_an_cpt(cgx_id, lmac_id) != 0)
				ret = -1;
			/* Check for link status */
			if (cgx_sgmii_check_link(cgx_id, lmac_id) != 0)
				ret = -1;
		}
	}

	if (ret == -1) {
		link.s.link_up = 0;
		link.s.full_duplex = 0;
		link.s.speed = CGX_LINK_NONE;
		err_type = cgx_get_error_type(cgx_id, lmac_id);
		debug_cgx_intf("%s: %d:%d CGX error %d\n", __func__,
					cgx_id, lmac_id, err_type);
	}

	/* update the current link status along with any error type set */
	cgx_set_link_state(cgx_id, lmac_id, &link, err_type);

	lmac_ctx->s.link_up = link.s.link_up;
	lmac_ctx->s.full_duplex = link.s.full_duplex;
	lmac_ctx->s.speed = link.s.speed;

	/* update the event status to evt_sts struct to notify kernel */
	scratchx0.u = CSR_READ(CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 0));
	if (err_type & CGX_ERR_MASK)
		scratchx0.s.evt_sts.stat = CGX_STAT_FAIL;
	else
		scratchx0.s.evt_sts.stat = CGX_STAT_SUCCESS;

	scratchx0.s.evt_sts.id = CGX_EVT_LINK_CHANGE;
	scratchx0.s.evt_sts.evt_type = CGX_EVT_ASYNC;
	scratchx0.s.evt_sts.ack = 1; /* set ack */
	CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 0), scratchx0.u);

	/* clear the link_change_req after handling it */
	lmac_ctx->s.link_change_req = 0;

	if (link.s.link_up) {
		lmac_ctx->s.link_req = 0;
		lmac_ctx->s.link_enable = 1;
	}
	return ret;
}

static int cgx_link_bringup(int cgx_id, int lmac_id)
{
	int mod_status = 0;
	int sfp_count = 0;
	int trans_type = 0;
	int valid;
	cgx_lmac_config_t *lmac_cfg;
	cgx_lmac_context_t *lmac_ctx;
	cgx_lmac_timers_t *lmac_tmr;
	link_state_t link;
	int sig_fail_count = 0, an_lt_fail_count = 0;
	int lp_fail_count = 0, phy_fail_count = 0;
	int fail_count = 0;

	/* get the lmac type and based on lmac
	 * type, initialize SGMII/XAUI link
	 */
	lmac_cfg = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	debug_cgx_intf("%s %d:%d lmac_type %d\n", __func__, cgx_id,
			lmac_id, lmac_cfg->mode);

	lmac_ctx = &lmac_context[cgx_id][lmac_id];
	lmac_tmr = &lmac_timers[cgx_id][lmac_id];

	link.u64 = 0;

	/* link_enable will be set when the LINK UP req is processed.
	 * To avoid processing duplication of requests, check for it
	 * and return the previous link status
	 */
	if (lmac_ctx->s.link_enable == 1) {
		debug_cgx_intf("%s: Link status for %d:%d is already up\n",
			__func__, cgx_id, lmac_id);
		link.s.link_up = lmac_ctx->s.link_up;
		link.s.full_duplex = lmac_ctx->s.full_duplex;
		link.s.speed = lmac_ctx->s.speed;
		link.s.fec = lmac_ctx->s.fec;
		cgx_set_link_state(cgx_id, lmac_id, &link,
			cgx_get_error_type(cgx_id, lmac_id));
		return 0;
	}

	if (!lmac_ctx->s.init_link) {
		debug_cgx_intf("%s: %d:%d initialize link\n", __func__,
					cgx_id, lmac_id);
		cgx_lmac_init_link(cgx_id, lmac_id);
		lmac_ctx->s.init_link = 1;

		/* If PHY re-config is set, change the OP mode */
		if (lmac_cfg->phy_config.forceconfig) {
			debug_cgx_intf("%s: %d:%d change the OP mode for PHY\n",
				__func__, cgx_id, lmac_id);
			phy_config(cgx_id, lmac_id);
		}
	}

	/* Save the current FEC what is configured by the user */
	link.s.fec = lmac_ctx->s.fec = lmac_cfg->fec;

	if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
		/* sequence if PHY is present:
		 *	- get PHY status,
		 *	- If link is up, init link and set speed
		 *	- If link is not up, return error
		 * sequence if PHY is not present(same when
		 * 			internal lbk enabled/PHY mode):
		 *	- assign default link status,
		 *	- init link and set speed
		 */
		if ((lmac_ctx->s.lbk1_enable) || (!lmac_cfg->phy_present) ||
				(lmac_cfg->phy_mode)) {
			link.s.link_up = 1;
			link.s.full_duplex = 1;
			link.s.speed = CGX_LINK_1G;
		} else {
			/* Get the link status */
			phy_get_link_status(cgx_id, lmac_id, &link);
		}

		if (link.s.link_up == 1) {	/* PHY link is up */
			/* Set up the link for the negotiated speed */
			if (cgx_sgmii_set_link_speed(cgx_id, lmac_id, &link) != 0) {
				link.s.link_up = 0;
				link.s.full_duplex = 0;
				link.s.speed = CGX_LINK_NONE;
				goto cgx_err;
			}

			/* Check for AN CPT */
			if (cgx_sgmii_check_an_cpt(cgx_id, lmac_id) != 0) {
				link.s.link_up = 0;
				link.s.full_duplex = 0;
				link.s.speed = CGX_LINK_NONE;
				goto cgx_err;	/* Poll timer to retry */
			}

			/* Check for link status */
			if (cgx_sgmii_check_link(cgx_id, lmac_id) != 0) {
				link.s.link_up = 0;
				link.s.full_duplex = 0;
				link.s.speed = CGX_LINK_NONE;
				goto cgx_err;	/* Poll timer to retry */
			}

			/* SUCCESS case : update the link status and indicate
			 * poll timer to start polling for the link
			 */
			lmac_ctx->s.link_up = link.s.link_up;
			lmac_ctx->s.full_duplex = link.s.full_duplex;
			lmac_ctx->s.speed = link.s.speed;
			lmac_ctx->s.link_enable = 1;
			cgx_set_link_state(cgx_id, lmac_id, &link, 0);

			return 0;

		} else {
			/* if the link is not up, return success with
			 * LINK as down
			 */
			debug_cgx_intf("%s : %d:%d PHY link status is down\n",
				__func__, cgx_id, lmac_id);
			goto cgx_err; /* To poll for the link again */
		}
	} else if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_XAUI) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_RXAUI) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_FIFTYG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_USXGMII)) {

		if (lmac_cfg->sfp_slot == 1) {
retry_mod_stat:
			/* Obtain SFP module status */
			mod_status = sfp_get_mod_status(cgx_id, lmac_id);
			if ((mod_status == SFP_MOD_STATE_PRESENT) ||
				(mod_status == SFP_MOD_STATE_EEPROM_UPDATED)) {
				/* Read EEPROM to determine module capability */
				trans_type = sfp_parse_eeprom_data(cgx_id,
								lmac_id);
				if (trans_type == SFP_TRANS_TYPE_NONE) {
					cgx_set_error_type(cgx_id, lmac_id,
						CGX_ERR_MODULE_INVALID);
					goto sfp_err;
				}
				/* Save the new mod status */
				lmac_ctx->s.mod_stats = mod_status;

				valid = sfp_validate_user_options(cgx_id,
								lmac_id);
				if (valid == 1) {
					debug_cgx_intf("%s: %d:%d\t"
					"trans_type %d\n",
					__func__, cgx_id, lmac_id,
					trans_type);
					goto retry_link;
				} else {
					ERROR("%s: %d:%d User config\t"
						"doesn't match EEPROM\n",
						__func__, cgx_id, lmac_id);
					cgx_set_error_type(cgx_id, lmac_id,
						CGX_ERR_MODULE_INVALID);
					goto sfp_err;
				}
			} else {
				if (sfp_count++ < 5) {
					mdelay(1);
					goto retry_mod_stat;
				}

				debug_cgx_intf("%s: %d:%d Module not present\n",
					__func__, cgx_id, lmac_id);
				cgx_set_error_type(cgx_id, lmac_id,
						CGX_ERR_MODULE_NOT_PRESENT);
				goto sfp_err;
			}
sfp_err:
			/* set link_req to 1 to indicate poll timer */
			lmac_ctx->s.link_req = 1;
			cgx_set_link_state(cgx_id, lmac_id, &link,
					cgx_get_error_type(cgx_id, lmac_id));
			return -1;
		}

retry_link:
		if (lmac_cfg->phy_present) {
			/* Get the link status */
			phy_get_link_status(cgx_id, lmac_id, &link);

			if (!link.s.link_up) {
				if (phy_fail_count++ < PHY_FAIL_RETRIES)
					goto retry_link;

				debug_cgx_intf("%s:%d:%d link status is down\n",
					__func__, cgx_id, lmac_id);
				/* PHY link is down */
				link.s.link_up = 0;
				link.s.full_duplex = 0;
				link.s.speed = CGX_LINK_NONE;
				link.s.fec = lmac_cfg->fec;
				phy_fail_count = 0; /* reset the counter */
				goto cgx_err; /* To poll for the link */
			}
			cgx_set_link_state(cgx_id, lmac_id, &link, 0);
		}
		/* Check if Rx link is down */
		if (!lmac_ctx->s.rx_link_up) {
			if (cgx_xaui_set_link_up(cgx_id, lmac_id) == -1) {
				/* Need to retry on a Rx Signal detect
				 * failure a min of 80ms to account
				 * for an AN restart
				 */
				if (cgx_get_error_type(cgx_id, lmac_id) ==
					CGX_ERR_SERDES_RX_NO_SIGNAL) {
					if (sig_fail_count++ < SIG_FAIL_RETRIES) {
						printf("%s: %d:%d Signal Detect failed,\t"
							"retrying link\n", __func__,
							cgx_id, lmac_id);
				/* if init link fails, retry */
						cgx_set_error_type(cgx_id, lmac_id, 0);
						goto retry_link;
					}
				} else if (cgx_get_error_type(cgx_id, lmac_id) ==
					CGX_ERR_AN_CPT_FAIL) {
					if (an_lt_fail_count++ < AN_LT_FAIL_RETRIES) {
						printf("%s: %d:%d AN Init failed,\t"
						"retrying link\n", __func__,
						cgx_id, lmac_id);
					/* clear the error when retrying */
					cgx_set_error_type(cgx_id, lmac_id, 0);
					goto retry_link;
				}
				} else if (cgx_get_error_type(cgx_id, lmac_id) ==
					CGX_ERR_TRAINING_FAIL) {
					if (an_lt_fail_count++ < AN_LT_FAIL_RETRIES) {
						printf("%s: %d:%d Link Training failed,\t"
							"retrying link\n", __func__,
							cgx_id, lmac_id);
						cgx_set_error_type(cgx_id, lmac_id, 0);
						goto retry_link;
					}
				} else {
					if (fail_count++ < ETH_FAIL_RETRIES) {
						printf("%s: %d:%d Ethernet Init failed,\t"
							"retrying link\n", __func__,
							cgx_id, lmac_id);
						cgx_set_error_type(cgx_id, lmac_id, 0);
						goto retry_link;
					}
				}
			} else
				lmac_ctx->s.rx_link_up = 1;
		}

		/* Only check full link (Tx and Rx) is up after verifying Rx is up */
		if (lmac_ctx->s.rx_link_up) {
			if (cgx_xaui_get_link(cgx_id, lmac_id, &link, lmac_ctx, lmac_tmr) == -1) {
				if (lp_fail_count++ < LP_FAIL_RETRIES) {
					debug_cgx_intf("%s %d:%d Link down,\t"
						"retrying link\n", __func__,
						cgx_id, lmac_id);
					/* clear the error when retrying */
					cgx_set_error_type(cgx_id, lmac_id, 0);
					goto retry_link;
				}
			}
		}

		if (link.s.link_up == 1) {	/* link is up */
			if (cgx_get_error_type(cgx_id, lmac_id) &
						CGX_ERR_MASK)
				goto cgx_err;
			/* SUCCESS case : update the link status and indicate
			 * poll timer to start polling for link
			 */
			lmac_ctx->s.link_up = link.s.link_up;
			lmac_ctx->s.full_duplex = link.s.full_duplex;
			lmac_ctx->s.speed = link.s.speed;
			lmac_ctx->s.link_enable = 1;
			cgx_set_link_state(cgx_id, lmac_id, &link, 0);
			return 0;
		} else {
			/* link is down */
			debug_cgx_intf("%s: %d:%d link status is down\n",
				__func__, cgx_id, lmac_id);
			goto cgx_err; /* Poll timer to poll for the link */
		}
	} else {
		debug_cgx_intf("%s %d:%d mode %d not configured correctly,"
			" cannot initialize link\n",
			__func__, cgx_id, lmac_id, lmac_cfg->mode);
		cgx_set_error_type(cgx_id, lmac_id,
			CGX_ERR_LMAC_MODE_INVALID);
		return -1;
	}

cgx_err:
	/* If the link is up/down, or the CGX configuration failed, reach here
	 * In that case, update the link status along with the error type
	 * in lmac_context structure and the SCRATCHX CSRs and notify
	 * poll timer to start polling for the link
	 */
	lmac_ctx->s.link_up = link.s.link_up;
	lmac_ctx->s.full_duplex = link.s.full_duplex;
	lmac_ctx->s.speed = link.s.speed;
	cgx_set_link_state(cgx_id, lmac_id, &link,
			cgx_get_error_type(cgx_id, lmac_id));

	lmac_ctx->s.link_enable = 1;

	return -1;
}

static int cgx_link_bringdown(int cgx_id, int lmac_id)
{
	cgx_lmac_config_t *lmac_cfg;
	cgx_lmac_context_t *lmac_ctx;
	link_state_t link;

	/* get the lmac type and based on lmac
	 * type, bring down SGMII/XAUI link
	 */
	lmac_cfg = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];
	debug_cgx_intf("%s %d:%d lmac_type %d\n", __func__,
				cgx_id, lmac_id, lmac_cfg->mode);

	lmac_ctx = &lmac_context[cgx_id][lmac_id];

	/* link_enable will be cleared when the LINK DOWN req is processed
	 * To avoid processing duplication of requests, check for it
	 * and return the previous link status.
	 * In case of QSGMII, always bring down the link even
	 * if the link was not brought up
	 */
	if ((lmac_ctx->s.init_link == 0) && (lmac_ctx->s.link_enable == 0) &&
	    (lmac_cfg->mode != CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
		debug_cgx_intf("%s: Link status for %d:%d is already down\n",
			__func__, cgx_id, lmac_id);
		link.s.link_up = lmac_ctx->s.link_up;
		link.s.full_duplex = lmac_ctx->s.full_duplex;
		link.s.speed = lmac_ctx->s.speed;
		cgx_set_link_state(cgx_id, lmac_id, &link,
			cgx_get_error_type(cgx_id, lmac_id));
		return 0;
	}

	if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
		if (cgx_sgmii_set_link_down(cgx_id, lmac_id) != 0)
			return -1;

	} else if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_XAUI) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_RXAUI) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_FIFTYG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_USXGMII)) {
		if (cgx_xaui_set_link_down(cgx_id, lmac_id) != 0)
			return -1;

	} else {
		debug_cgx_intf("%s LMAC%d mode %d not configured correctly"
			" cannot bring down the link\n",
			__func__, lmac_id, lmac_cfg->mode);
		cgx_set_error_type(cgx_id, lmac_id,
			CGX_ERR_LMAC_MODE_INVALID);
		return -1;
	}

	/* link is brought down successfully. update the link
	 * status and indicate poll timer to stop polling
	 * for the link
	 */
	lmac_ctx->s.link_up = link.s.link_up = 0;
	lmac_ctx->s.full_duplex = link.s.full_duplex = 0;
	lmac_ctx->s.speed = link.s.speed = 0;
	cgx_set_link_state(cgx_id, lmac_id, &link,
			cgx_get_error_type(cgx_id, lmac_id));

	lmac_ctx->s.link_enable = 0;
	lmac_ctx->s.init_link = 0;

	return 0;
}

static int cgx_get_lmac_type_for_req_mode(uint64_t req_mode)
{
	int lmac_mode = 0;

	for (int i = 0; i < ARRAY_SIZE(speed_mode_map); i++) {
		if (req_mode == speed_mode_map[i].mode_bitmask) {
			lmac_mode = speed_mode_map[i].lmac_mode;
			break;
		}
	}
	debug_cgx_intf("%s: req_mode %lld mode %d\n", __func__,
			req_mode, lmac_mode);
	return lmac_mode;
}

static int cgx_get_training_for_mode(uint64_t req_mode)
{
	int use_training = 0;

	for (int i = 0; i < ARRAY_SIZE(speed_mode_map); i++) {
		if (req_mode == speed_mode_map[i].mode_bitmask) {
			use_training = speed_mode_map[i].train_en;
			break;
		}
	}
	debug_cgx_intf("%s: req_mode %lld use_training %d\n", __func__,
			req_mode, use_training);
	return use_training;
}

static int cgx_get_qlm_mode_for_req_mode(uint64_t req_mode)
{
	int qlm_mode = 0;

	for (int i = 0; i < ARRAY_SIZE(speed_mode_map); i++) {
		if (req_mode == speed_mode_map[i].mode_bitmask) {
			qlm_mode = speed_mode_map[i].qlm_mode;
			break;
		}
	}
	debug_cgx_intf("%s: req_mode 0x%llx qlm_mode %d\n", __func__,
			req_mode, qlm_mode);
	return qlm_mode;
}

static int cgx_get_fec_for_speed(int qlm_mode)
{
	int fec = 0;

	for (int i = 0; i < ARRAY_SIZE(speed_mode_map); i++) {
		if (qlm_mode == speed_mode_map[i].qlm_mode) {
			fec = speed_mode_map[i].fec_cap;
			break;
		}
	}
	debug_cgx_intf("%s: qlm_mode %d fec %d\n", __func__, qlm_mode, fec);

	return fec;
}

int qlm_get_baud_rate_for_mode(int qlm_mode)
{
	int baud_rate = 0;

	for (int i = 0; i < ARRAY_SIZE(speed_mode_map); i++) {
		if (qlm_mode == speed_mode_map[i].qlm_mode) {
			baud_rate = speed_mode_map[i].baud_rate;
			break;
		}
	}
	debug_cgx_intf("%s: qlm_mode %d baud rate %d\n", __func__, qlm_mode,
					baud_rate);

	return baud_rate;
}

static int cgx_check_speed_change_allowed(int cgx_id, int lmac_id, int new_mode,
						uint64_t mode_bitmask)
{
	int change = 0;
	cgx_lmac_config_t *lmac_cfg;
	int new_lc;

	debug_cgx_intf("%s: %d:%d mode_bitmask 0x%llx\n",
		__func__, cgx_id, lmac_id, mode_bitmask);
	lmac_cfg = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	/* Check if mode is in the supported link modes */
	if (!(mode_bitmask & lmac_cfg->supported_link_modes)) {
		debug_cgx_intf("%s: bitmask 0x%llx link_mode 0x%llx\n",
			__func__, mode_bitmask,
			lmac_cfg->supported_link_modes);
		return 0;
	}

	new_lc = cgx_get_lane_count(new_mode);
	if (new_lc && lmac_cfg->max_lane_count >= new_lc)
		change = 1;
	debug_cgx_intf("%s: %d:%d change %d new_lc %d max lane count %d\n",
				__func__, cgx_id, lmac_id, change,
				new_lc, lmac_cfg->max_lane_count);
	return change;
}

static int cgx_set_phy_mod_type(int cgx_id, int lmac_id, int phy_mod_type)
{
	cgx_lmac_config_t *lmac;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	if (lmac->phy_present && lmac->phy_config.init &&
	    lmac->phy_config.valid) {
		if (!phy_set_mod_type(cgx_id, lmac_id, phy_mod_type)) {
			if (cgx_update_flash_phy_mod_param(cgx_id,
							   lmac_id,
							   phy_mod_type)) {
				debug_cgx_intf("%s: %d:%d failed to update phy mod in flash\n",
					       __func__, cgx_id, lmac_id);
				return -1;
			}
		} else {
			return -1; /* phy_set_mod_type() failed */
		}
	} else {
		debug_cgx_intf("%s: %d:%d phy not present\n",
			       __func__, cgx_id, lmac_id);
		return -1;
	}
	return 0;
}

int cgx_handle_mode_change(int cgx_id, int lmac_id,
				struct cgx_mode_change_args *args)
{
	int req_speed, valid, lmac_type, ret = 0;
	cgx_lmac_config_t *lmac;
	cgx_lmac_context_t *lmac_ctx;
	cgx_config_t *cgx;
	int qlm_mode = 0, baud_mhz = 0, req_an = 0, flags = 0;
	int req_duplex = 0;
	int qlm, gserx = 0, an = 0, invalid_req = 0, num_lanes;
	link_state_t link;
	int lane, lane_count;
	uint64_t req_mode;
	uint32_t lane_mask;

	lmac_ctx = &lmac_context[cgx_id][lmac_id];
	cgx = &plat_octeontx_bcfg->cgx_cfg[cgx_id];
	lmac = &cgx->lmac_cfg[lmac_id];
	req_speed = args->speed;
	req_an = args->an;
	req_mode = args->mode;
	req_duplex = args->duplex;
	an = !lmac->autoneg_dis;

	debug_cgx_intf("%s: %d:%d speed %d req_speed %d req_an %d req_duplex %d req_mode 0x%llx\n",
				__func__, cgx_id, lmac_id, lmac_ctx->s.speed,
					req_speed, req_an, req_duplex, req_mode);

	if ((!req_mode) && (req_speed == CGX_LINK_NONE)) {
		invalid_req = 1;
		/* If speed change is not requested, check for
		 * other parameters
		 */
		if ((req_an != 0) && (req_an != 1)) {
			debug_cgx_intf("%s: %d:%d Invalid AN request\n",
					 __func__, cgx_id, lmac_id);
			invalid_req = 1;
		}
		if ((req_duplex != 0) && (req_duplex != 1)) {
			debug_cgx_intf("%s: %d: %d Invalid duplex request\n",
					 __func__, cgx_id, lmac_id);
			invalid_req = 1;
		}
		if (invalid_req == 1) {
			debug_cgx_intf("%s: %d: %d Invalid speed/AN/mode request\n",
					 __func__, cgx_id, lmac_id);
			cgx_set_error_type(cgx_id, lmac_id,
					CGX_ERR_SPEED_CHANGE_INVALID);
			goto mode_err;
		}
	}

	/* User is requesting for a different speed. Check if
	 * it is valid to change to user requested speed
	 */
	qlm_mode = cgx_get_qlm_mode_for_req_mode(req_mode);
	debug_cgx_intf("%s: req_an %d an %d qlm_mode %d\n", __func__,
					req_an, an, qlm_mode);

	if ((lmac->mode_idx != qlm_mode) || (req_an != an)) {
		lmac_type = cgx_get_lmac_type_for_req_mode(req_mode);
		valid = cgx_check_speed_change_allowed(cgx_id, lmac_id,
						lmac_type, req_mode);
		if (valid) {
			/* Bring down the CGX link */
			ret = cgx_link_bringdown(cgx_id, lmac_id);

			baud_mhz = qlm_get_baud_rate_for_mode(qlm_mode);

			/* Update the new mode info to board config structure
			 */
			lmac->autoneg_dis = !req_an;
			lmac->mode_idx = qlm_mode;

			/* Clear the attributes related to mode and set
			 * the corresponding attributes for the new mode
			 */
			lmac->use_training = 0;
			lmac->sgmii_1000x_mode = 0;
			lmac->use_training = cgx_get_training_for_mode(req_mode);

			/* Update attributes for 1000 BASE-X */
			if (lmac->mode_idx == QLM_MODE_1G_X)
				lmac->sgmii_1000x_mode = 1;

			/* Check FEC capability and update FEC for mode.
			 * If current LMAC's FEC doesn't match the mode's
			 * capability, disable FEC
			 */
			if (!(lmac->fec & cgx_get_fec_for_speed(qlm_mode)))
				lmac->fec = CGX_FEC_NONE;

			lmac->mode = lmac_type;
			qlm = lmac->qlm + lmac->shift_from_first;
			gserx = lmac->gserx + lmac->shift_from_first;

			debug_cgx_intf("%s: Re-configuring serdes for mode %d, baud rate %d, lmac type %d\n",
						__func__, qlm_mode, baud_mhz,
						lmac->mode);

			/* Configure SerDes for new QLM mode */
			qlm_state_lane_t state = qlm_build_state(qlm_mode, baud_mhz, flags);

			lane_count = cgx_get_lane_count(lmac_type);
			lane_mask = 0;
			for (int i = 0; i < lane_count; i++) {
				lane = (lmac->lane_to_sds >> (i*2)) & 3;
				lane_mask |= 1 << lane;
			}
			lmac->lane_mask = lane_mask;

			while (lane_mask) {
				/* Get the number of lanes on this QLM/DLM */
				num_lanes = qlm_get_lanes(qlm);
				for (int lane = 0; lane < num_lanes; lane++) {
					if (!(lane_mask & (1 << lane)))
						continue;
					/* Change the SERDES speed */
					cgx->qlm_ops->qlm_set_mode(gserx, lane,
							qlm_mode, baud_mhz, 0);
				}
				lane_mask >>= num_lanes;
				qlm++;
				gserx++;
			}

			/* Update the SCRATCHX register with the new link info to the
			 * original lane
			 */
			if (cgx->qlm_ops->type == QLM_GSERN_TYPE)
				cgx->qlm_ops->qlm_set_state(lmac->gserx,
							lmac->lane,
							state);

			/* Wait 5ms before bringing UP the CGX link */
			mdelay(5);

			if ((lmac->phy_present) && (lmac->phy_config.init)) {
				lmac->phy_config.forceconfig = 1;
				if (lmac->phy_config.mod_type
					== PHY_MOD_TYPE_PAM4 &&
				    qlm_mode != QLM_MODE_50GAUI_4_C2C &&
				    qlm_mode != QLM_MODE_50GAUI_2_C2C) {
					WARN("%s: %d:%d Setting PHY modulation type to NRZ because new speed does not support PAM4.\n",
					     __func__, cgx_id, lmac_id);
					cgx_set_phy_mod_type(cgx_id, lmac_id,
							     PHY_MOD_TYPE_NRZ);
				}
			}

			cgx_lmac_init(cgx_id, lmac_id);

			/* Bring UP the CGX link */
			ret = cgx_link_bringup(cgx_id, lmac_id);

			/* Clear any errors set during LINK bring up as the mode
			 * is changed now successfully and link may come up
			 * later. In this case, still return SUCCESS for MODE
			 * change command
			 */
			cgx_set_error_type(cgx_id, lmac_id, 0);
			return ret;
		} else {
			debug_cgx_intf("%s: %d: %d Invalid speed change request\n", __func__,
				cgx_id, lmac_id);
			cgx_set_error_type(cgx_id, lmac_id,
					CGX_ERR_SPEED_CHANGE_INVALID);
			goto mode_err;
		}
	}
mode_err:
	link.s.fec = lmac_ctx->s.fec;
	link.s.link_up = lmac_ctx->s.link_up;
	link.s.full_duplex = lmac_ctx->s.full_duplex;
	link.s.speed = lmac_ctx->s.speed;
	cgx_set_link_state(cgx_id, lmac_id, &link,
			cgx_get_error_type(cgx_id, lmac_id));
	return -1;
}

int cgx_set_fec_type(int cgx_id, int lmac_id, int req_fec)
{
	int ret = 0;
	link_state_t link;
	cgx_lmac_config_t *lmac;
	cgx_lmac_context_t *lmac_ctx;

	lmac_ctx = &lmac_context[cgx_id][lmac_id];
	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	debug_cgx_intf("%s: %d:%d fec %d request_fec %d\n", __func__, cgx_id,
				lmac_id, lmac->fec, req_fec);

	if (lmac->phy_present && lmac->phy_config.init &&
	    lmac->phy_config.mod_type == PHY_MOD_TYPE_PAM4 &&
	    req_fec != CGX_FEC_RS) {
		ERROR("%s: %d:%d PAM4 requires RS-FEC.  No other FEC setting is allowed.\n",
		      __func__, cgx_id, lmac_id);
		cgx_set_error_type(cgx_id, lmac_id, CGX_ERR_SET_FEC_INVALID);
		return -1;
	}

	if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) ||
			(lmac->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
		ERROR("%s: %d: %d FEC is not applicable for this mode %d\n",
				__func__, cgx_id, lmac_id, lmac->mode);
		cgx_set_error_type(cgx_id, lmac_id, CGX_ERR_SET_FEC_INVALID);
		return -1;
	}

	if (req_fec == lmac->fec) {
		WARN("%s: %d:%d FEC requested is same as current FEC state\n",
				__func__, cgx_id, lmac_id);
		return 0;
	}

	/* Validate FEC based on LMAC mode, QLM mode, and PHY mod type */
	ret = cgx_validate_fec_config(cgx_id, lmac_id, req_fec);
	if (ret == -1) {
		cgx_set_error_type(cgx_id, lmac_id, CGX_ERR_SET_FEC_INVALID);
		return -1;
	}

	/* FIXME: Validate FEC based on transceiver */

	/* Update the new FEC requested by user in the board config */
	lmac->fec = req_fec;

	/* Configure PHY to new mode based on FEC requested
	 * if PHY is present
	 */
	if ((lmac->phy_present) && (lmac->phy_config.init)) {
		phy_config(cgx_id, lmac_id);
		mdelay(1);
	}

	/* Change CGX configuration to new FEC */
	ret = cgx_fec_change(cgx_id, lmac_id, req_fec);
	if (ret == -1) {
		ERROR("%s: FEC type could not be changed\n", __func__);
		cgx_set_error_type(cgx_id, lmac_id, CGX_ERR_SET_FEC_FAIL);
		return -1;
	}

	/* Just update the new FEC type but use the existing link status */
	link.s.fec = lmac_ctx->s.fec = req_fec;
	link.s.link_up = lmac_ctx->s.link_up;
	link.s.full_duplex = lmac_ctx->s.full_duplex;
	link.s.speed = lmac_ctx->s.speed;

	cgx_set_link_state(cgx_id, lmac_id, &link,
			cgx_get_error_type(cgx_id, lmac_id));

	return 0;
}

static int cgx_control_higig2(int cgx_id, int lmac_id, int enable)
{
	struct val {
		int ifg1, ifg2, len, fcssel, skip_after_term, hg2tx_en,
		    hg2rx_en, phys_en, logl_en, pre_chk, pre_strp, hg_en,
		    preamble, tx_en, rx_en;
	};
	static const struct val values[2] = {
		[0] = { /* for disabling HiGig2 */
			.ifg1		 = 8,
			.ifg2		 = 4,
			.len		 = 0,
			.fcssel		 = 0,
			.skip_after_term = 0,
			.hg2tx_en	 = 0,
			.hg2rx_en	 = 0,
			.phys_en	 = 1,
			.logl_en	 = 0xffff,
			.pre_chk	 = 1,
			.pre_strp	 = 1,
			.hg_en		 = 0,
			.preamble	 = 1,
			.tx_en		 = 0,
			.rx_en		 = 0
		},
		[1] = { /* for enabling HiGig2 */
			.ifg1		 = 4,
			.ifg2		 = 4,
			.len		 = 16,
			.fcssel		 = 0,
			.skip_after_term = 1,
			.hg2tx_en	 = 1,
			.hg2rx_en	 = 1,
			.phys_en	 = 1,
			.logl_en	 = 1,
			.pre_chk	 = 0,
			.pre_strp	 = 0,
			.hg_en		 = 1,
			.preamble	 = 0,
			.tx_en		 = 0,
			.rx_en		 = 0
		}
	};
	cavm_cgxx_spux_misc_control_t misc_control;
	cavm_cgxx_smux_hg2_control_t hg2_control;
	cavm_cgxx_smux_rx_frm_ctl_t rx_frm_ctl;
	cavm_cgxx_smux_tx_append_t tx_append;
	cavm_cgxx_smux_rx_udd_skp_t udd_skp;
	cavm_cgxx_smux_cbfc_ctl_t cbfc_ctl;
	cavm_cgxx_smux_tx_ctl_t tx_ctl;
	cavm_cgxx_smux_tx_ifg_t ifg;
	cgx_lmac_config_t *lmac;
	const struct val *v;

	debug_cgx_intf("%s %d:%d\n", __func__, cgx_id, lmac_id);

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	if (lmac->mode == CAVM_CGX_LMAC_TYPES_E_SGMII ||
	    lmac->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII) {
		ERROR("%s: %d:%d HiGig2 is not supported by %s\n",
		      __func__, cgx_id, lmac_id,
		      qlm_get_mode_strmap(lmac->mode_idx).bdk_str);
		return -1;
        }

	if (enable)
		v = &values[1];
	else
		v = &values[0];

	ifg.u = CSR_READ(CAVM_CGXX_SMUX_TX_IFG(cgx_id, lmac_id));
	ifg.s.ifg1 = v->ifg1;
	ifg.s.ifg2 = v->ifg2;
	CSR_WRITE(CAVM_CGXX_SMUX_TX_IFG(cgx_id, lmac_id), ifg.u);

	udd_skp.u = CSR_READ(CAVM_CGXX_SMUX_RX_UDD_SKP(cgx_id, lmac_id));
	udd_skp.s.len = v->len;
	udd_skp.s.fcssel = v->fcssel;
	CSR_WRITE(CAVM_CGXX_SMUX_RX_UDD_SKP(cgx_id, lmac_id), udd_skp.u);

	misc_control.u = CSR_READ(CAVM_CGXX_SPUX_MISC_CONTROL(cgx_id, lmac_id));
	misc_control.s.skip_after_term = v->skip_after_term;
	CSR_WRITE(CAVM_CGXX_SPUX_MISC_CONTROL(cgx_id, lmac_id), misc_control.u);

	hg2_control.u = CSR_READ(CAVM_CGXX_SMUX_HG2_CONTROL(cgx_id, lmac_id));
	hg2_control.s.hg2tx_en = v->hg2tx_en;
	hg2_control.s.hg2rx_en = v->hg2rx_en;
	hg2_control.s.phys_en  = v->phys_en;
	hg2_control.s.logl_en  = v->logl_en;
	CSR_WRITE(CAVM_CGXX_SMUX_HG2_CONTROL(cgx_id, lmac_id), hg2_control.u);

	rx_frm_ctl.u = CSR_READ(CAVM_CGXX_SMUX_RX_FRM_CTL(cgx_id, lmac_id));
	rx_frm_ctl.s.pre_chk  = v->pre_chk;
	rx_frm_ctl.s.pre_strp = v->pre_strp;
	CSR_WRITE(CAVM_CGXX_SMUX_RX_FRM_CTL(cgx_id, lmac_id), rx_frm_ctl.u);

	tx_ctl.u = CSR_READ(CAVM_CGXX_SMUX_TX_CTL(cgx_id, lmac_id));
	tx_ctl.s.hg_en = v->hg_en;
	CSR_WRITE(CAVM_CGXX_SMUX_TX_CTL(cgx_id, lmac_id), tx_ctl.u);

	tx_append.u = CSR_READ(CAVM_CGXX_SMUX_TX_APPEND(cgx_id, lmac_id));
	tx_append.s.preamble = v->preamble;
	CSR_WRITE(CAVM_CGXX_SMUX_TX_APPEND(cgx_id, lmac_id), tx_append.u);

	cbfc_ctl.u = CSR_READ(CAVM_CGXX_SMUX_CBFC_CTL(cgx_id, lmac_id));
	cbfc_ctl.s.tx_en = v->tx_en;
	cbfc_ctl.s.rx_en = v->rx_en;
	CSR_WRITE(CAVM_CGXX_SMUX_CBFC_CTL(cgx_id, lmac_id), cbfc_ctl.u);

	return 0;
}

#ifdef DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS
static int start_prbs(int cgx_id, int num_lanes, int mode, int *show_phy_host,
		int *show_phy_line)
{
	int lane;
	int gserx;
	cgx_config_t *cgx_cfg;

	cgx_cfg = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);

	if (mode == 0) {
		WARN("PRBS with pattern memory not implemented yet.\n");
		return -1;
	}

	/* gserx index is the same for every lane */
	gserx = cgx_cfg->lmac_cfg[0].gserx;

	/* Start PRBS */
	cgx_cfg->qlm_ops->qlm_enable_prbs(gserx, mode, QLM_DIRECTION_TX);

	for (lane = 0; lane < num_lanes; lane++) {
		/* BDK use here bdk_netphy_get_handle function */
		if (cgx_cfg->lmac_cfg[lane].phy_present) {
			if (*show_phy_host) {
				if (phy_enable_prbs(cgx_id, lane, 1, mode, 1))
					*show_phy_host = 0;
			}
			if (*show_phy_host) {
				if (phy_enable_prbs(cgx_id, lane, 0, mode, 1))
					*show_phy_line = 0;
			}
		}
	}
	udelay(1000);  /* Let TX run for 1ms before starting RX */
	cgx_cfg->qlm_ops->qlm_enable_prbs(gserx, mode, QLM_DIRECTION_RX);

	for (lane = 0; lane < num_lanes; lane++) {
		/* BDK use here bdk_netphy_get_handle function */
		if (cgx_cfg->lmac_cfg[lane].phy_present) {
			if (*show_phy_host) {
				if (phy_enable_prbs(cgx_id, lane, 1, mode, 1))
					*show_phy_host = 0;
			}
			if (*show_phy_host) {
				if (phy_enable_prbs(cgx_id, lane, 0, mode, 1))
					*show_phy_line = 0;
			}
		}
	}

	return 0;
}

static int stop_prbs(int cgx_id, int num_lanes, int mode, int show_phy_host)
{
	int lane;
	int gserx;
	cgx_config_t *cgx_cfg;

	cgx_cfg = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);

	/* gserx index is the same for every lane */
	gserx = cgx_cfg->lmac_cfg[0].gserx;

	/* Stop PRBS */
	cgx_cfg->qlm_ops->qlm_disable_prbs(gserx);

	for (lane = 0; lane < num_lanes; lane++) {
		/* BDK use here bdk_netphy_get_handle function */
		if (cgx_cfg->lmac_cfg[lane].phy_present && show_phy_host) {
			phy_disable_prbs(cgx_id, lane, 1, mode);
			phy_disable_prbs(cgx_id, lane, 0, mode);
		}
	}

	return 0;
}

static int get_prbs_errors(int cgx_id, int num_lanes, int mode,
		int show_phy_host, int show_phy_line,
		cgx_prbs_errors_t *errors)
{
	int lane;
	int gserx;
	cgx_config_t *cgx_cfg;

	cgx_cfg = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);

	/* gserx index is the same for every lane */
	gserx = cgx_cfg->lmac_cfg[0].gserx;

	for (lane = 0; lane < num_lanes; lane++) {
		errors[lane].err = cgx_cfg->qlm_ops->qlm_get_prbs_errors(
							gserx, lane, 0);

		/* BDK use here bdk_netphy_get_handle */
		if (!show_phy_host) {
			errors[lane].phy_host = -2;
		} else if (cgx_cfg->lmac_cfg[lane].phy_present) {
			errors[lane].phy_host = phy_get_prbs_errors(cgx_id,
							lane, 1, 0, mode);
		} else {
			errors[lane].phy_host = -1;
		}
		/* BDK use here bdk_netphy_get_handle */
		if (!show_phy_line) {
			errors[lane].phy_line = -2;
		} else if (cgx_cfg->lmac_cfg[lane].phy_present) {
			errors[lane].phy_line = phy_get_prbs_errors(cgx_id,
							lane, 0, 0, mode);
		} else {
			errors[lane].phy_line = -1;
		}
	}

	return 0;
}

/* This structure is used to keep information about ongoing PRBS commands */
struct {
	int show_phy_host;
	int show_phy_line;
	int mode;
	int started;
} prbs_status[MAX_CGX];

/* Check if memory region for PRBS data is big enough */
CASSERT(sizeof(cgx_prbs_data) <= SERDES_PRBS_DATA_SIZE,
	assert_serdes_prbs_data_size_to_small);

int cgx_smc_do_prbs(int cmd, int qlm, int x3)
{
	int rc;
	int num_lanes;
	int cgx_id;
	cgx_prbs_data *prbs_data;

	if (qlm >= MAX_QLM || qlm < 0) {
		WARN("%d not in range, available QLM0-%d\n", qlm, MAX_QLM - 1);
		return -1;
	}
	num_lanes = plat_octeontx_scfg->qlm_max_lane_num[qlm];
	cgx_id = plat_get_cgx_idx(qlm);
	if (cgx_id == -1) {
		WARN("To QLM%d any CGX cannot by wired.\n", qlm);
		return -1;
	}

	switch (cmd) {
	case CGX_PRBS_START_CMD:
		if (prbs_status[cgx_id].started) {
			stop_prbs(cgx_id, num_lanes,
				prbs_status[cgx_id].mode,
				prbs_status[cgx_id].show_phy_host);
			prbs_status[cgx_id].started = 0;
		}

		prbs_status[cgx_id].mode = x3;
		rc = start_prbs(cgx_id, num_lanes,
				prbs_status[cgx_id].mode,
				&prbs_status[cgx_id].show_phy_host,
				&prbs_status[cgx_id].show_phy_line);
		if (rc)
			return -1;

		prbs_status[cgx_id].started = 1;
		break;

	case CGX_PRBS_STOP_CMD:
		if (prbs_status[cgx_id].started) {
			stop_prbs(cgx_id, num_lanes,
				prbs_status[cgx_id].mode,
				prbs_status[cgx_id].show_phy_host);
			prbs_status[cgx_id].started = 0;
		}
		break;

	case CGX_PRBS_GET_DATA_CMD:
		if (!prbs_status[cgx_id].started) {
			WARN("PRBS for QLM%d is not started\n", qlm);
			return -1;
		}

		prbs_data = (cgx_prbs_data *)(SERDES_PRBS_DATA_BASE);
		prbs_data->num_lanes = num_lanes;

		get_prbs_errors(cgx_id, num_lanes,
				prbs_status[cgx_id].mode,
				prbs_status[cgx_id].show_phy_host,
				prbs_status[cgx_id].show_phy_line,
				prbs_data->errors);
		break;

	default:
		WARN("Unknown PRBS cmd (%d)\n", cmd);
		return -1;
	}

	return CGX_DISPLAY_OK;
}

static int do_prbs(int qlm, int mode, int time)
{
	int ec;
	int num_lanes;
	int lane;
	int show_phy_host = 1;
	int show_phy_line = 1;
	int time_left, delay;
	int cgx_id;
	int qlm_idx;
	cgx_config_t *cgx_cfg;
	cgx_prbs_errors_t errors[MAX_LMAC_PER_CGX];
	const int DISPLAY_INTERVAL = 5;

	if (qlm >= MAX_QLM || qlm < 0) {
		WARN("%d not in range, available QLM0-%d\n", qlm, MAX_QLM - 1);
		return -1;
	}
	num_lanes = plat_octeontx_scfg->qlm_max_lane_num[qlm];
	cgx_id = plat_get_cgx_idx(qlm);
	if (cgx_id == -1) {
		WARN("To QLM%d any CGX cannot by wired.\n", qlm);
		return -1;
	}
	cgx_cfg = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);

	printf("Start PRBS-%d on QLM%d (CGX %d), end in %d sec\n",
			mode, qlm, cgx_id, time);

	ec = start_prbs(cgx_id, num_lanes, mode, &show_phy_host,
			&show_phy_line);
	if (ec)
		return -1;

	/* qlm index is the same for every lane */
	qlm_idx = cgx_cfg->lmac_cfg[0].qlm;

	/* Wait/display */
	time_left = time;
	while (time_left > 0) {
		delay = time_left > DISPLAY_INTERVAL ?
				DISPLAY_INTERVAL : time_left;
		time_left -= delay;
		mdelay(delay * 1000);

		get_prbs_errors(cgx_id, num_lanes, mode, show_phy_host,
				show_phy_line, errors);

		for (lane = 0; lane < num_lanes; lane++) {
			printf("Time: %d seconds QLM%d.Lane%d: errors: ",
				time - time_left, qlm_idx, lane);
			if (errors[lane].err != -1)
				printf("%lld", errors[lane].err);
			else
				printf("No lock");

			if (show_phy_host) {
				printf(", PHY Host errors: ");
				if (errors[lane].phy_host >= 0)
					printf("%lld", errors[lane].phy_host);
				else
					printf("No lock");
			}
			if (show_phy_line) {
				printf(", PHY Line errors: ");
				if (errors[lane].phy_line >= 0)
					printf("%lld", errors[lane].phy_line);
				else
					printf("No lock");
			}
			printf("\n");
		}
	}

	printf("Stopping pattern generator\n");
	stop_prbs(cgx_id, num_lanes, mode, show_phy_host);

	return 0;
}

/*
 * Square root by abacus algorithm, Martin Guy @ UKC, June 1985.
 * From a book on programming abaci by Mr C. Woo.
 */
static uint64_t isqrt(uint64_t num)
{
	uint64_t result = 0;
	/* The second-to-top bit is set: 1 << 62 for 64 bits */
	uint64_t bit = 1ull << 62;

	/* "bit" starts at the highest power of four <= the argument. */
	while (bit > num)
		bit >>= 2;

	while (bit != 0) {
		if (num >= result + bit) {
			num -= result + bit;
			result = (result >> 1) + bit;
		} else {
			result >>= 1;
		}
		bit >>= 2;
	}

	return result;
}

static uint64_t log10(uint64_t num)
{
	uint64_t result = 0;

	while (num > 10) {
		num /= 10;
		result++;
	}
	if (num >= 5)
		result++;

	return result;
}

/* This structure has over 32KiB and cannot be stored on stack */
static gser_qlm_eye_t eye;

int cgx_display_eye(int qlm, int qlm_lane, int show_data)
{
	int x, y, width, height, last_color, level, deltay, deltax, dy, dx;
	int dist, color, max_lane;
	int eye_area = 0;
	int eye_width = 0;
	int eye_height = 0;
	int gserx;
	int cgx_id;
	int ec;
	cgx_config_t *cgx_cfg;
	char color_str[] = "\33[40m"; /* Note: This is modified, not constant */

	if (qlm >= MAX_QLM || qlm < 0) {
		WARN("%d not in range, available QLM0-%d\n", qlm, MAX_QLM - 1);
		return -1;
	}

	max_lane = plat_octeontx_scfg->qlm_max_lane_num[qlm];
	if (qlm_lane >= max_lane || qlm_lane < 0) {
		WARN("%d not in range, available for QLM%d lanes are 0-%d\n",
				qlm_lane, qlm, max_lane - 1);
		return -1;
	}

	cgx_id = plat_get_cgx_idx(qlm);
	if (cgx_id == -1) {
		WARN("To QLM%d any CGX cannot by wired.\n", qlm);
		return -1;
	}
	cgx_cfg = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);

	gserx = cgx_cfg->lmac_cfg[qlm_lane].gserx;

	ec = cgx_cfg->qlm_ops->qlm_eye_capture(gserx, qlm_lane,
						show_data, &eye);
	if (ec)
		return ec;

	if (!show_data) {
		eye.type = cgx_cfg->qlm_ops->type;
		memcpy((void *)(SERDES_EYE_DATA_BASE), &eye,
				sizeof(gser_qlm_eye_t));

		return CGX_DISPLAY_OK;
	}

	if (cgx_cfg->qlm_ops->type == QLM_GSERN_TYPE) {
		uint64_t data;
		for (y = 0; y < eye.height; y++) {
			for (x = 0; x < eye.width; x++) {
				data = eye.data[y][x] + eye.data[y][x + 64];
				if (data > UINT32_MAX)
					data = UINT32_MAX;
				eye.data[y][x] = data;
			}
		}
	}

	/* Calculate the max eye width */
	for (y = 0; y < eye.height; y++) {
		width = 0;
		for (x = 0; x < eye.width; x++) {
			if (eye.data[y][x] == 0) {
				width++;
				eye_area++;
			}
		}
		if (width > eye_width)
			eye_width = width;
	}

	/* Calculate the max eye height */
	for (x = 0; x < eye.width; x++) {
		height = 0;
		for (y = 0; y < eye.height; y++) {
			if (eye.data[y][x] == 0) {
				height++;
				eye_area++;
			}
		}
		if (height > eye_height)
			eye_height = height;
	}

	printf("\nEye Diagram for QLM %d, Lane %d\n", qlm, qlm_lane);

	last_color = -1;
	for (y = 0; y < eye.height; y++) {
		for (x = 0; x < eye.width; x++) {
			level = log10(eye.data[y][x] + 1);
			if (level > 9)
				level = 9;
			#define DIFF(a, b) (a < b) ? b-a : a-b
			deltay = (y == (eye.height - 1)) ? -1 : 1;
			deltax = (x == (eye.width - 1)) ? -1 : 1;
			dy = DIFF(eye.data[y][x], eye.data[y + deltay][x]);
			dx = DIFF(eye.data[y][x], eye.data[y][x + deltax]);
			#undef DIFF
			dist = dx * dx + dy * dy;
			color = log10(isqrt(dist) + 1);
			if (color > 6)
				color = 6;
			if (level == 0)
				color = 0;
			if (color != last_color) {
				color_str[3] = '0' + color;
				printf("%s", color_str);
				last_color = color;
			}
			printf("%c", '0' + level);
		}
		printf("\33[0m\n");
		last_color = -1;
	}
	printf("\nEye Width %d, Height %d, Area %d\n",
		eye_width, eye_height, eye_area);

	return CGX_DISPLAY_OK;
}

int cgx_display_serdes_settings(int qlm, int qlm_lane, int show_data)
{
	int max_lane;
	int gserx;
	int cgx_id;
	cgx_config_t *cgx_cfg;

	if (qlm >= MAX_QLM || qlm < 0) {
		WARN("%d not in range, available QLM0-%d\n", qlm, MAX_QLM - 1);
		return -1;
	}

	max_lane = plat_octeontx_scfg->qlm_max_lane_num[qlm];
	if (qlm_lane >= max_lane || qlm_lane < 0) {
		WARN("%d not in range, available for QLM%d lanes are 0-%d\n",
				qlm_lane, qlm, max_lane - 1);
		return -1;
	}

	cgx_id = plat_get_cgx_idx(qlm);
	if (cgx_id == -1) {
		WARN("To QLM%d any CGX cannot by wired.\n", qlm);
		return -1;
	}
	cgx_cfg = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);

	gserx = cgx_cfg->lmac_cfg[qlm_lane].gserx;

	if (show_data) {
		cgx_cfg->qlm_ops->qlm_display_settings(gserx, qlm_lane, 1, 1,
				NULL, 0);
	} else {
		cgx_cfg->qlm_ops->qlm_display_settings(gserx, qlm_lane, 1, 1,
				(char *)(SERDES_SETTINGS_DATA_BASE),
				SERDES_SETTINGS_DATA_SIZE);
	}

	return CGX_DISPLAY_OK;
}

#endif /* DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS */

/* Note : this function executes with lock acquired */
static int cgx_process_requests(int cgx_id, int lmac_id)
{
	int ret = 0, val = 0;
	int enable = 0; /* read from scratch1 - cmd_args */
	int request_id = 0, err_type = 0, req_fec, phy_mod_type, ignore;
#if defined(PLAT_t96)
	int mode;
#endif
	union cgx_scratchx0 scratchx0;
	union cgx_scratchx1 scratchx1;
	link_state_t link;
	cgx_lmac_context_t *lmac_ctx;
	cgx_lmac_config_t *lmac;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];
	lmac_ctx = &lmac_context[cgx_id][lmac_id];

	/* Read the command arguments from SCRATCHX(1) */
	scratchx1.u = CSR_READ(CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 1));
	request_id = scratchx1.s.cmd.id;
	enable = scratchx1.s.cmd_args.enable;
	debug_cgx_intf("%s: %d:%d request_id %d\n", __func__, cgx_id,
				lmac_id, request_id);

	/* always reset the error bits when processing new
	 * command except when obtaining current status
	 */
	if (request_id != CGX_CMD_GET_LINK_STS)
		cgx_set_error_type(cgx_id, lmac_id, 0);

	/* some of the commands like below should be handled independent
	 * of whether LMAC is enabled or not
	 */
	if ((request_id == CGX_CMD_INTF_SHUTDOWN) ||
#ifdef NT_FW_CONFIG
		(request_id == CGX_CMD_GET_MKEX_SIZE) ||
		(request_id == CGX_CMD_GET_MKEX_PROFILE) ||
#endif
		(request_id == CGX_CMD_SET_MAC_ADDR) ||
		(request_id == CGX_CMD_GET_FWD_BASE) ||
#ifdef DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS
		(request_id == CGX_CMD_PRBS) ||
		(request_id == CGX_CMD_DISPLAY_EYE) ||
		(request_id == CGX_CMD_DISPLAY_SERDES) ||
#endif /* DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS */
		(request_id == CGX_CMD_GET_FW_VER)) {
		switch (request_id) {
		case CGX_CMD_INTF_SHUTDOWN:
			cgx_fw_intf_shutdown();
			/* in case of shutdown, clear all other
			 * bits and set only ack bit to indicate
			 * to user request is processed (this bit
			 * will be cleared by user)
			 */
			scratchx0.u = 0;
			scratchx0.s.evt_sts.ack = 1; /* set ack */
			CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(
				cgx_id, lmac_id, 0), scratchx0.u);
			return 0;
		case CGX_CMD_GET_FW_VER:
			scratchx0.u = 0;
			scratchx0.s.ver.major_ver = CGX_FIRMWARE_MAJOR_VER;
			scratchx0.s.ver.minor_ver = CGX_FIRMWARE_MINOR_VER;
			CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(
				cgx_id, lmac_id, 0), scratchx0.u);
			break;
#ifdef NT_FW_CONFIG
		case CGX_CMD_GET_MKEX_PROFILE:
			scratchx0.u = 0;
			scratchx0.s.prfl_addr.mcam_addr = otx2_get_npc_profile_addr(0);
			CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(
				cgx_id, lmac_id, 0), scratchx0.u);

			debug_cgx_intf("%s: MKEX_PROFILE %u\n", __func__,
				(unsigned int)scratchx0.s.prfl_addr.mcam_addr);
			break;

		case CGX_CMD_GET_MKEX_SIZE:
			scratchx0.u = 0;
			scratchx0.s.prfl_sz.mcam_sz = otx2_get_npc_profile_size(0);
			CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(
				cgx_id, lmac_id, 0), scratchx0.u);

			debug_cgx_intf("%s: MKEX_SIZE %u\n", __func__,
				(unsigned int)scratchx0.s.prfl_sz.mcam_sz);
			break;
#endif
		case CGX_CMD_GET_FWD_BASE:
			scratchx0.u = 0;
			scratchx0.s.fwd_base_s.addr = get_sh_fwdata_base();
			CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 0),
				scratchx0.u);
			break;

		case CGX_CMD_SET_MAC_ADDR:
			sh_fwdata_update_mac_addr(scratchx1.s.mac_args.addr,
						  scratchx1.s.mac_args.pf_id);
			break;
#ifdef DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS
		case CGX_CMD_PRBS:
			do_prbs(scratchx1.s.prbs_args.qlm,
				scratchx1.s.prbs_args.mode,
				scratchx1.s.prbs_args.time);
			break;

		case CGX_CMD_DISPLAY_EYE:
			cgx_display_eye(scratchx1.s.dsp_eye_args.qlm,
				scratchx1.s.dsp_eye_args.lane,
				1 /* = show_data */);
			break;

		case CGX_CMD_DISPLAY_SERDES:
			cgx_display_serdes_settings(
				scratchx1.s.dsp_serdes_args.qlm,
				scratchx1.s.dsp_serdes_args.lane,
				1 /* = show_data */);
			break;
#endif /* DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS */
		}
	} else {
		/* all the below commands should be processed only
		 * when LMAC is enabled
		 */
		if (lmac->lmac_enable) {
			switch (request_id) {
			case CGX_CMD_LINK_BRING_UP:
				ret = cgx_link_bringup(cgx_id, lmac_id);
				break;
			case CGX_CMD_LINK_BRING_DOWN:
				ret = cgx_link_bringdown(cgx_id, lmac_id);
				break;
			case CGX_CMD_INTERNAL_LBK:
				lmac_ctx->s.lbk1_enable = enable;
				cgx_set_internal_loopback(cgx_id, lmac_id, enable);
				break;
			case CGX_CMD_EXTERNAL_LBK:
				cgx_set_external_loopback(cgx_id, lmac_id,
							enable);
				break;
			case CGX_CMD_AN_LOOPBACK:
				if (lmac->autoneg_dis) {
					WARN("%s: %d:%d command not applicable for AN disabled LMAC,ignoring enabling loopback mode\n",
						__func__, cgx_id, lmac_id);
					lmac->an_loopback = 0;
				} else
					lmac->an_loopback = enable;
				break;
			case CGX_CMD_GET_LINK_STS:
				CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(
						cgx_id, lmac_id, 0), 0); /* reset */
				link.s.link_up = lmac_ctx->s.link_up;
				link.s.full_duplex = lmac_ctx->s.full_duplex;
				link.s.speed = lmac_ctx->s.speed;
				link.s.fec = lmac->fec;
				cgx_set_link_state(cgx_id, lmac_id, &link,
					lmac_ctx->s.error_type);
				cgx_set_link_mode(cgx_id, lmac_id,
						  lmac->mode_idx);
				break;
			case CGX_CMD_GET_MAC_ADDR:
				scratchx0.u = 0;
				scratchx0.s.mac_s.addr_0 = lmac->local_mac_address[0];
				scratchx0.s.mac_s.addr_1 = lmac->local_mac_address[1];
				scratchx0.s.mac_s.addr_2 = lmac->local_mac_address[2];
				scratchx0.s.mac_s.addr_3 = lmac->local_mac_address[3];
				scratchx0.s.mac_s.addr_4 = lmac->local_mac_address[4];
				scratchx0.s.mac_s.addr_5 = lmac->local_mac_address[5];
				debug_cgx_intf("%s mac_addr %x:%x:%x:%x:%x:%x\n", __func__,
						scratchx0.s.mac_s.addr_0,
						scratchx0.s.mac_s.addr_1,
						scratchx0.s.mac_s.addr_2,
						scratchx0.s.mac_s.addr_3,
						scratchx0.s.mac_s.addr_4,
						scratchx0.s.mac_s.addr_5);
				CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(
						cgx_id, lmac_id, 0), scratchx0.u);
				break;
			case CGX_CMD_GET_SUPPORTED_FEC:
				scratchx0.u = 0;
				/* SFP EEPROM info will be available only when
				 * link is brought UP. If the link_enable is set
				 * in case of SFP slot, supported FEC should
				 * be returned based on transceiver capabilities
				 * If not, return PCS supported FEC types
				 */
				if ((lmac_ctx->s.link_enable) &&
						(lmac->sfp_slot))
					val = sfp_get_fec_capability(cgx_id,
								lmac_id);
				else
					val = cgx_get_supported_fec_type(
							cgx_id, lmac_id);
				scratchx0.s.supported_fec.fec = val;
				debug_cgx_intf("%s: %d:%d supported FEC %d\n",
					__func__, cgx_id, lmac_id,
					scratchx0.s.supported_fec.fec);
				CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(
						cgx_id, lmac_id, 0),
						scratchx0.u);
			break;
			case CGX_CMD_SET_FEC:
				/* Read the command arguments from SCRATCH(1) */
				scratchx1.u = CSR_READ(CAVM_CGXX_CMRX_SCRATCHX(
							cgx_id, lmac_id, 1));
				req_fec = scratchx1.s.fec_args.fec;
				debug_cgx_intf("%s: %d:%d requested FEC %d\n",
						__func__,
						cgx_id, lmac_id, req_fec);
				ret = cgx_set_fec_type(cgx_id, lmac_id,
								req_fec);
				if (!cgx_get_error_type(cgx_id, lmac_id)) {
					if (cgx_update_flash_fec_param(cgx_id,
						lmac_id, req_fec))
						debug_cgx_intf(
						"Flash update fec failed\n");
				}
			break;
			case CGX_CMD_SET_PHY_MOD_TYPE:
				phy_mod_type = scratchx1.s.phy_mod_args.mod;
				ret = cgx_set_phy_mod_type(cgx_id, lmac_id,
							   phy_mod_type);
			break;
			case CGX_CMD_GET_PHY_MOD_TYPE:
				scratchx0.u = 0;
				scratchx0.s.phy_mod_type.mod =
					lmac->phy_config.mod_type;
				CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(
						cgx_id, lmac_id, 0),
						scratchx0.u);
			break;
			case CGX_CMD_SET_PERSIST_IGNORE:
				ignore = scratchx1.s.persist_args.ignore;
				ret = cgx_update_flash_ignore_param(cgx_id,
								    lmac_id,
								    ignore);
			break;
			case CGX_CMD_GET_PERSIST_IGNORE:
				scratchx0.u = 0;
				ret = cgx_read_flash_ignore(cgx_id, lmac_id,
							    &ignore);
				if (!ret)
					scratchx0.s.persist.ignore = ignore;
				CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(
						cgx_id, lmac_id, 0),
						scratchx0.u);
			break;
			case CGX_CMD_GET_PHY_FEC_STATS:
				ret = phy_get_fec_stats(cgx_id, lmac_id);
				if (!ret)
					sh_fwdata_update_phy_fec_stats(cgx_id,
								       lmac_id);
			break;
#if defined(PLAT_t96)
			case CGX_CMD_MODE_CHANGE:
				/* Read the command arguments from SCRATCH(1) */
				scratchx1.u = CSR_READ(CAVM_CGXX_CMRX_SCRATCHX(
							cgx_id, lmac_id, 1));
				ret = cgx_handle_mode_change(cgx_id, lmac_id,
						&scratchx1.s.mode_change_args);
				mode = cgx_get_lmac_type_for_req_mode(scratchx1.s.mode_change_args.mode);
				if (!cgx_get_error_type(cgx_id, lmac_id)) {
					if (cgx_update_flash_mode_param(cgx_id,
					    lmac_id, mode))
						debug_cgx_intf(
						"Flash update mode failed\n");
				}
			break;
#endif
			case CGX_CMD_HIGIG:
				ret = cgx_control_higig2(cgx_id, lmac_id, enable);
			break;
			case CGX_CMD_SET_PTP_MODE:
				ret = cgx_set_ptp_mode(cgx_id, lmac_id, enable);
			break;
			/* FIXME: add support for other commands */
			default:
				debug_cgx_intf("%s: %d:%d Invalid request %d\n",
					__func__, cgx_id, lmac_id, request_id);
				cgx_set_error_type(cgx_id, lmac_id,
					CGX_ERR_REQUEST_ID_INVALID);
				break;
			}
		} else {
			debug_cgx_intf("%s: CGX%d LMAC%d is not enabled.\t"
					"Req %d ignored\n", __func__, cgx_id,
					lmac_id, request_id);
			cgx_set_error_type(cgx_id, lmac_id,
					CGX_ERR_LMAC_NOT_ENABLED);
		}
	}

	/* update the event status either async or resp
	 * to command by writing to evt_sts struct
	 */
	scratchx0.u = CSR_READ(CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 0));
	err_type = cgx_get_error_type(cgx_id, lmac_id);
	if (err_type & CGX_ERR_MASK)
		scratchx0.s.evt_sts.stat = CGX_STAT_FAIL;
	else
		scratchx0.s.evt_sts.stat = CGX_STAT_SUCCESS;

	/* For all requests, update the command status, ID and
	 * set event type
	 */
	scratchx0.s.evt_sts.id = request_id;
	scratchx0.s.evt_sts.evt_type = CGX_EVT_CMD_RESP;
	if ((request_id != CGX_CMD_LINK_BRING_UP) &&
		(request_id != CGX_CMD_LINK_BRING_DOWN) &&
		(request_id != CGX_CMD_SET_FEC) &&
		(request_id != CGX_CMD_MODE_CHANGE)) {
		/* in case of LINK_UP/DOWN, error type is updated
		 * as part of link status struct
		 */
		if (err_type & CGX_ERR_MASK)
			scratchx0.s.err.type = err_type;
	}

	scratchx0.s.evt_sts.ack = 1; /* set ack */
	CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(
				cgx_id, lmac_id, 0), scratchx0.u);

	return ret;
}

static int cgx_handle_link_reqs(int cgx_id, int lmac_id,
			link_state_t *new_link)
{
	int timeout = 10; /* check the moderate val */;
	union cgx_scratchx0 cgx_scratch0;
	cgx_lmac_context_t *lmac_ctx;

	debug_cgx_intf("%s: %d:%d\n", __func__, cgx_id, lmac_id);

	/* check for the ack bit to be clear and post the command */
	do {
		cgx_scratch0.u = CSR_READ(CAVM_CGXX_CMRX_SCRATCHX(
					cgx_id, lmac_id, 0));
		debug_cgx_intf("%s, waiting for prev ack %d to be clear\n",
				__func__, cgx_scratch0.s.evt_sts.ack);
		if (!cgx_scratch0.s.evt_sts.ack)
			break;
		udelay(1);
	} while (--timeout);

	if (!timeout) {
		/* prev ack not clear */
		debug_cgx_intf("%s %d:%d Prev ack not clear to post\t"
			"LINK CHANGE req\n", __func__, cgx_id, lmac_id);
		return -1;
	}

	/* acquire the internal lock */
	if (cgx_acquire_csr_lock(cgx_id, lmac_id) == -1) {
		debug_cgx_intf("%s %d:%d lock not obtained to post\t"
				"LINK CHANGE req\n", __func__, cgx_id, lmac_id);
		return -1;
	}

	lmac_ctx = &lmac_context[cgx_id][lmac_id];

	/* post the command by setting link_change_req */
	lmac_ctx->s.link_up = new_link->s.link_up;
	lmac_ctx->s.speed = new_link->s.speed;
	lmac_ctx->s.full_duplex = new_link->s.full_duplex;
	lmac_ctx->s.link_change_req = 1;

	/* release the internal lock */
	cgx_release_csr_lock(cgx_id, lmac_id);

	return 0;
}

static int cgx_check_sfp_mod_stat(int cgx_id, int lmac_id)
{
	int ret = 0, mod_status = 0;
	cgx_lmac_context_t *lmac_ctx;

	debug_cgx_intf("%s: %d:%d\n", __func__, cgx_id, lmac_id);

	lmac_ctx = &lmac_context[cgx_id][lmac_id];

	/* Obtain the module status */
	mod_status = sfp_get_mod_status(cgx_id, lmac_id);

	if (mod_status == -1)
		return -1;

	/* MCP updates EEPROM buffer every 5s if the user
	 * hasn't un-plugged/plugged the transceiver. In this
	 * case, just update the SH memory with the
	 * data and do not handle any link change
	 */
	if (mod_status == SFP_MOD_STATE_EEPROM_UPDATED) {
		lmac_ctx->s.mod_stats = mod_status;
		sfp_parse_eeprom_data(cgx_id, lmac_id);
		return 0;
	}

	if (mod_status != lmac_ctx->s.mod_stats) {
		/* Update the new status */
		lmac_ctx->s.mod_stats = mod_status;
		if (mod_status == SFP_MOD_STATE_PRESENT) {
			/* User has unplug and plug the module.
			 * In this case, read the EEPROM capabilities
			 * and configure CGX accordingly if there is
			 * a change in capabilities.
			 */
			debug_cgx_intf("%s: %d:%d User has plugged module\n",
				 __func__, cgx_id, lmac_id);

			ret = cgx_sfp_obtain_capabilities(cgx_id, lmac_id);
			if (ret != 1)
				return -1;
			return 1; /* Valid */
		}
		debug_cgx_intf("%s: %d:%d user has un-plugged cable\n",
				__func__, cgx_id, lmac_id);
		/* If Module is absent, clear the EEPROM data */
		sh_fwdata_clear_eeprom_data(cgx_id, lmac_id, 0);
	}
	return 0;
}

static int cgx_get_link_status(int cgx_id, int lmac_id,
				link_state_t *link)
{
	cgx_lmac_config_t *lmac = NULL;
	phy_config_t *phy = NULL;
	int ret = 0;
	cgx_lmac_context_t *lmac_ctx;
	cgx_lmac_timers_t *lmac_tmr;

	/* get the lmac type and based on lmac
	 * type, initialize SGMII/XAUI link
	 */

	lmac_ctx = &lmac_context[cgx_id][lmac_id];
	lmac_tmr = &lmac_timers[cgx_id][lmac_id];

	debug_cgx_intf("%s: %d:%d\n", __func__, cgx_id, lmac_id);

	/* Get the LMAC type for each LMAC */
	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];
	phy = &lmac->phy_config;

	debug_cgx_intf("%s: mode %d\n", __func__, lmac->mode);

	/* Append the current FEC to new link status */
	link->s.fec = lmac->fec;

	if (lmac->phy_present) {
		/* Get the PHY link status */
		if (lmac->mdio_bus_dbg != phy->mdio_bus) {
			if (phy_get_link_status(cgx_id, lmac_id, link) == -1) {
				debug_cgx_intf("%s: %d:%d phy_get_link_status failed\n",
					__func__, cgx_id, lmac_id);
				return -1;
			}
		}
		/* For non-SGMII cases, still continue to read the
		 * CGX registers to know the link status if the
		 * link is UP
		 */
		if ((!link->s.link_up) ||
			((lmac->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) ||
			(lmac->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII))) {
			debug_cgx_intf("%s %d:%d link %d speed %d duplex %d\n",
					__func__, cgx_id, lmac_id,
					link->s.link_up, link->s.speed,
					link->s.full_duplex);
			return 0;
		}
	}

	/* In case of SGMII/QSGMII/1000 BASE-X, with PHY not present,
	 * (even loopback module) return the link as UP based on
	 * PCS_RXX_SYNC with default speed as 1G
	 */
	if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
		if (cgx_sgmii_check_link(cgx_id, lmac_id) != -1) {
			link->s.link_up = 1;
			link->s.full_duplex = 1;
			link->s.speed = CGX_LINK_1G;
		}
		return 0;
	}

	if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_XAUI) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_RXAUI) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_FIFTYG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R) ||
		(lmac->mode == CAVM_CGX_LMAC_TYPES_E_USXGMII)) {
		/* Obtain the link status from CGX CSRs */
		if (lmac_ctx->s.rx_link_up) {
			if (cgx_xaui_get_link(cgx_id, lmac_id, link, lmac_ctx, lmac_tmr) == -1) {
				/* When the rx link is up but link is still not UP, wait for remote fault to clear */
				debug_cgx_intf("%s: %d:%d link down\n",
						   __func__, cgx_id, lmac_id);
			}
		} else {
			debug_cgx_intf("%s: %d:%d link down, re-initialize Rx link\n",
						   __func__, cgx_id, lmac_id);
			if (cgx_xaui_set_link_up(cgx_id, lmac_id))
				lmac_ctx->s.rx_link_up = 0;
			else
				lmac_ctx->s.rx_link_up = 1;
		}
		debug_cgx_intf("%s: %d:%d link %d speed %d duplex %d\n",
			__func__, cgx_id, lmac_id,
			link->s.link_up,
			link->s.speed, link->s.full_duplex);
		return ret;
	}

	/* Other cases should not reach here */
	ERROR("%s: %d:%d Invalid reach\n", __func__, cgx_id, lmac_id);
	return -1;
}

/* Timer callback to periodically poll for link */
static int cgx_poll_for_link_cb(int timer)
{
	int valid = 0;
	cgx_lmac_config_t *lmac_cfg;
	cgx_lmac_context_t *lmac_ctx;
	link_state_t link;

	for (int cgx = 0; cgx < plat_octeontx_scfg->cgx_count; cgx++) {
		for (int lmac = 0; lmac < MAX_LMAC_PER_CGX; lmac++) {
			lmac_cfg = &plat_octeontx_bcfg->cgx_cfg[cgx].lmac_cfg[lmac];
			lmac_ctx = &lmac_context[cgx][lmac];

			link.u64 = 0;
			valid = 0;

			if ((lmac_ctx->s.link_enable) ||
				(lmac_ctx->s.link_req)) {
				if (lmac_cfg->sfp_slot)
					valid = cgx_check_sfp_mod_stat(cgx,
							lmac);
				/* Get the link status */
				cgx_get_link_status(cgx, lmac, &link);

				/* If the prev link change is not handled
				 * wait until it is handled as the reqs
				 * are handled one at a time
				 */
				if ((!lmac_ctx->s.link_change_req) &&
				((valid == 1) || ((lmac_ctx->s.link_up !=
					link.s.link_up) ||
					(lmac_ctx->s.full_duplex !=
					link.s.full_duplex) ||
					(lmac_ctx->s.speed !=
					link.s.speed)))) {
					debug_cgx_intf("%d:%d link changed%d\n",
							cgx, lmac,
							link.s.link_up);
					cgx_handle_link_reqs(cgx, lmac,
								&link);
				}
			}
		}
	}

	return 0;
}

/* Timer callback to process CGX requests */
static int cgx_handle_requests_cb(int timer)
{
	union cgx_scratchx1 scratch1;
	union cgx_scratchx0 scratch0;
	cgx_lmac_context_t *lmac_ctx;

	/* Go through all active LMACs and check
	 * if there are any new message requests by reading
	 * command register of each LMAC(SCRATCHX(1)
	 */
	for (int cgx = 0; cgx < plat_octeontx_scfg->cgx_count; cgx++) {
		for (int lmac = 0; lmac < MAX_LMAC_PER_CGX; lmac++) {
			scratch1.u = CSR_READ(CAVM_CGXX_CMRX_SCRATCHX(cgx, lmac, 1));
			scratch0.u = CSR_READ(CAVM_CGXX_CMRX_SCRATCHX(cgx, lmac, 0));
			lmac_ctx = &lmac_context[cgx][lmac];

			/* acquire firmware internal lock */
			if (cgx_acquire_csr_lock(cgx, lmac) == -1) {
				debug_cgx_intf("%s %d:%d lock not"
				" obtained to process command,"
				" wait for now\n",
				 __func__, cgx, lmac);
				/* skip to next LMAC */
				continue;
			}

			/* first, check if link change event is
			 * posted by timer #2 before handling
			 * new requests post by non-secure SW
			 */
			if (lmac_ctx->s.link_change_req) {
				debug_cgx_intf("%s: %d:%d link chng req\n",
					__func__, cgx, lmac);
				cgx_link_change_req(cgx, lmac);

				/* trigger an interrupt to notify the event */
				cgx_trigger_interrupt(cgx, lmac);
			}
			/* poll on ownership to be set as OWN_FW to
			 * process any new requests
			 */
			if (scratch1.s.own_status == CGX_OWN_FIRMWARE) {
				if (scratch0.s.evt_sts.ack) {
					debug_cgx_intf("%s Req ignored,"
						" status not cleared\n",
						__func__);
					cgx_set_error_type(cgx, lmac,
					CGX_ERR_PREV_ACK_NOT_CLEAR);
					cgx_release_own_status(cgx, lmac);
					cgx_release_csr_lock(cgx, lmac);
					/* skip to next LMAC */
					continue;
				}

				cgx_process_requests(cgx, lmac);

				cgx_release_own_status(cgx, lmac);

				/* trigger an interrupt before ret */
				cgx_trigger_interrupt(cgx, lmac);
			}

			/* release firmware internal lock */
			cgx_release_csr_lock(cgx, lmac);
		}
	}
	return 0;
}

void cgx_get_link_state(int cgx_id, int lmac_id, link_state_t *link)
{
	union cgx_scratchx0 scratchx0;

	debug_cgx_intf("%s:%d:%d\n", __func__, cgx_id, lmac_id);

	scratchx0.u = CSR_READ(CAVM_CGXX_CMRX_SCRATCHX(
					cgx_id, lmac_id, 0));
	link->s.link_up  = scratchx0.s.link_sts.link_up;
	link->s.full_duplex = scratchx0.s.link_sts.speed;
	link->s.full_duplex = scratchx0.s.link_sts.full_duplex;
}

/* this function to be called from any CGX function when major
 * error type is encountered
 */
void cgx_set_error_type(int cgx_id, int lmac_id, uint64_t type)
{
	cgx_lmac_context_t *lmac_ctx;

	lmac_ctx = &lmac_context[cgx_id][lmac_id];
	lmac_ctx->s.error_type = type;
}

void cgx_set_supported_link_modes(int cgx_id, int lmac_id)
{
	cgx_lmac_config_t *lmac_cfg;

	lmac_cfg = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	if (!(lmac_cfg->phy_present) && !(lmac_cfg->sfp_slot)) {
		/* If PHY not present and not SFP/QSFP slot, return
		 * default all modes as supported (loopback module
		 * or internally connected)
		 */
		lmac_cfg->supported_link_modes = CGX_ALL_SUPPORTED_MODES;
	}

	if (lmac_cfg->phy_present)
		lmac_cfg->supported_link_modes =
			lmac_cfg->phy_config.supported_link_modes;

	/* If PHY is not present, check whether it is SFP/QSFP slot */
	if (lmac_cfg->sfp_slot) {
		if (lmac_cfg->sfp_info.is_sfp) {
			lmac_cfg->supported_link_modes |=
				((1 << CGX_MODE_1000_BASEX_BIT) |
				(1 << CGX_MODE_SGMII_BIT) |
				(1 << CGX_MODE_10G_C2C_BIT) |
				(1 << CGX_MODE_10G_C2M_BIT) |
				(1 << CGX_MODE_10G_KR_BIT) |
				(1 << CGX_MODE_25G_C2C_BIT) |
				(1 << CGX_MODE_25G_C2M_BIT) |
				(1 << CGX_MODE_20G_C2C_BIT));
		} else if (lmac_cfg->sfp_info.is_qsfp) {
			lmac_cfg->supported_link_modes = CGX_ALL_SUPPORTED_MODES;
		}
	}

	/* Restrict speed change only for modes that are
	 * supported now.Also need to obtain transceiver cap to
	 * determine the supported modes
	 */
	if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_TWENTYFIVEG_R)) {
		uint64_t modes_allowed = ((1 << CGX_MODE_1000_BASEX_BIT) |
					 (1 << CGX_MODE_SGMII_BIT) |
					 (1 << CGX_MODE_10G_C2C_BIT) |
					 (1 << CGX_MODE_10G_C2M_BIT) |
					 (1 << CGX_MODE_10G_KR_BIT) |
					 (1 << CGX_MODE_20G_C2C_BIT) |
					(1 << CGX_MODE_25G_C2C_BIT) |
					(1 << CGX_MODE_25G_C2M_BIT));

		if (!strncmp(plat_octeontx_bcfg->bcfg.board_model, "cn33", 4))
			modes_allowed |= (1 << CGX_MODE_25G_2_C2C_BIT) |
					 (1 << CGX_MODE_50G_C2C_BIT)   |
					 (1 << CGX_MODE_50G_4_C2C_BIT);

		lmac_cfg->supported_link_modes &= modes_allowed;

	} else if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_FIFTYG_R) ||
		 (lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R)) {
		switch (lmac_cfg->mode_idx) {
		case QLM_MODE_XLAUI:
		case QLM_MODE_XLAUI_C2M:
		case QLM_MODE_40G_CR4:
		case QLM_MODE_40G_KR4:
			lmac_cfg->supported_link_modes &=
				((1 << CGX_MODE_40G_C2C_BIT) |
				(1 << CGX_MODE_40G_C2M_BIT) |
				(1 << CGX_MODE_40G_CR4_BIT) |
				(1 << CGX_MODE_40G_KR4_BIT) |
				(1 << CGX_MODE_80GAUI_C2C_BIT) |
				(1 << CGX_MODE_100G_C2C_BIT) |
				(1 << CGX_MODE_100G_C2M_BIT) |
				(1 << CGX_MODE_100G_CR4_BIT) |
				(1 << CGX_MODE_100G_KR4_BIT));
		break;
		case QLM_MODE_50GAUI_4_C2C:
		case QLM_MODE_25GAUI_2_C2C:
			lmac_cfg->supported_link_modes &=
				((1 << CGX_MODE_25G_2_C2C_BIT) |
			(1 << CGX_MODE_50G_4_C2C_BIT) |
			(1 << CGX_MODE_50G_C2C_BIT) |
			(1 << CGX_MODE_10G_C2C_BIT) |
			(1 << CGX_MODE_10G_C2M_BIT) |
			(1 << CGX_MODE_10G_KR_BIT));
		break;
		default:
		break;
		}
	} else if (lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII)
		lmac_cfg->supported_link_modes &=
			(1 << CGX_MODE_QSGMII_BIT);
	else if (lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R)
		lmac_cfg->supported_link_modes &=
				((1 << CGX_MODE_40G_C2C_BIT) |
				(1 << CGX_MODE_40G_C2M_BIT) |
				(1 << CGX_MODE_40G_CR4_BIT) |
				(1 << CGX_MODE_40G_KR4_BIT) |
				(1 << CGX_MODE_80GAUI_C2C_BIT) |
				(1 << CGX_MODE_100G_C2C_BIT) |
				(1 << CGX_MODE_100G_C2M_BIT) |
				(1 << CGX_MODE_100G_CR4_BIT) |
				(1 << CGX_MODE_100G_KR4_BIT));
	else
		lmac_cfg->supported_link_modes = 0;

	debug_cgx_intf("%s: %d:%d link_modes 0x%llx\n",
			__func__, cgx_id, lmac_id,
			lmac_cfg->supported_link_modes);

	/* Update the supported link modes to SH FW data mem */
	sh_fwdata_set_supported_link_modes(cgx_id, lmac_id);
}

static void cgx_check_for_presence_of_phy(int cgx_id, int lmac_id)
{
	int mode, dev, reg, phy_id, phy_id_mask, val;
	cgx_lmac_config_t *lmac;
	phy_config_t *phy;

	if (strncmp(plat_octeontx_bcfg->bcfg.board_model, "ebb9", 4))
		return; /* if board is not ebb9x, then do nothing */

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_id].lmac_cfg[lmac_id];
	phy = &lmac->phy_config;

	if (!lmac->phy_present)
		return;

	switch (phy->type) {
	case PHY_MARVELL_5113:
		mode = CLAUSE45;
		dev = PHY_88X5113_ID_DEV;
		reg = PHY_88X5113_ID_REG;
		phy_id = PHY_88X5113_ID,
		phy_id_mask = PHY_88X5113_ID_MASK;
		break;

	case PHY_VITESSE_8574:
		mode = CLAUSE22;
		dev = -1;
		reg = MII_PHY_ID1_REG;
		phy_id = PHY_VSC8574_ID1;
		phy_id_mask = PHY_VSC8574_ID_MASK;
		break;

	default:
		return;
	}

	val = phy_mdio_read(phy, mode, dev, reg) & phy_id_mask;
	if (val != phy_id) {
		/* PHY that was specified in device tree is not detected on the
		 * I/O module.
		 */
		switch (lmac->mode_idx) {
		case QLM_MODE_SGMII:
		case QLM_MODE_QSGMII:
			/* These QLM modes require a PHY.
			 * Keep the lmac->phy_present flag at 1 so that the PHY
			 * driver will run and report errors (because the PHY is
			 * missing).
			 */
			break;

		default:
			/* Mark the PHY as not present so that the PHY driver
			 * will not run; otherwise it will report errors.
			 * For most Ethernet QLM modes, a PHY is not required.
			 */
			printf("%s: %d:%d PHY type %d not detected in I/O module, disabling PHY present in config\n",
				       __func__, cgx_id, lmac_id, phy->type);
			/* In case of 1000 BASE-X without PHY, AN needs
			 * to be disabled
			 */
			if (lmac->mode_idx == QLM_MODE_1G_X)
				lmac->autoneg_dis = 1;
			lmac->phy_present = 0;
			memset(phy, 0, sizeof(*phy));
			break;
		}
	}
}

/* This function should be called once during boot time */
void cgx_fw_intf_init(void)
{
	cgx_config_t *cgx_cfg;
	cgx_lmac_config_t *lmac_cfg;
	cgx_lmac_context_t *lmac_ctx;
	int cgx_idx;
	const qlm_ops_t *qlm_ops;

	debug_cgx_intf("%s\n", __func__);

	/*
	 * CGX will be used for first time on BL31.
	 * Pointers to qlm_ops needs to be refreashed.
	 */
	for (cgx_idx = 0; cgx_idx < plat_octeontx_scfg->cgx_count; cgx_idx++) {
		qlm_ops = plat_otx2_get_qlm_ops(cgx_idx);
		if (qlm_ops == NULL) {
			WARN("%s:get_qlm_ops failed for CGX %d\n",
				__func__, cgx_idx);
		}
		plat_octeontx_bcfg->cgx_cfg[cgx_idx].qlm_ops = qlm_ops;
	}

	for (int cgx = 0; cgx < plat_octeontx_scfg->cgx_count; cgx++) {
		cgx_cfg = &plat_octeontx_bcfg->cgx_cfg[cgx];

		if (cgx_cfg->enable) {
			for (int lmac = 0; lmac < MAX_LMAC_PER_CGX; lmac++) {
				lmac_cfg = &plat_octeontx_bcfg->cgx_cfg[cgx].lmac_cfg[lmac];
				lmac_ctx = &lmac_context[cgx][lmac];
				if (lmac_cfg->lmac_enable) {
					cgx_check_for_presence_of_phy(cgx, lmac);
					if (lmac_cfg->phy_present) {
						/* If PHY is present, look up for PHY
						 * driver and init
						 */
						phy_lookup(cgx, lmac, lmac_cfg->phy_config.type);
						if ((lmac_cfg->phy_config.valid) &&
							(!lmac_cfg->phy_config.init)) {
							debug_cgx_intf("%s: init PHY\n", __func__);
							phy_probe(cgx, lmac);
							lmac_cfg->phy_config.init = 1;
						}
					}
					/* Enable LMAC */
					cgx_lmac_init_link(cgx, lmac);
					lmac_ctx->s.init_link = 1;
					/* If PHY is initialized, configure
					 * the PHY. For ex: to set in
					 * particular mode
					 */
					if (lmac_cfg->phy_config.init) {
						phy_config(cgx, lmac);
						phy_set_supported_link_modes(
							cgx, lmac);
					}
					cgx_set_supported_link_modes(cgx, lmac);
				}
			}
		} else {
			/* for CGXs that are not configured by BDK to any mode,
			 * CGX config CSRs needs to be configured correctly
			 * as init callback will not be triggered for these
			 * CGXs
			 */
			cgx_hw_init(cgx);
		}
	}

	/* start with 1 timer to handle & process CGX requests */
	cgx_timers[0] = timer_create(TM_PERIODIC, 1000, cgx_handle_requests_cb);
	timer_start(cgx_timers[0]);

	/* start 2nd timer to peridically poll for link status */
	cgx_timers[1] = timer_create(TM_PERIODIC, 1000, cgx_poll_for_link_cb);
	timer_start(cgx_timers[1]);
}

/* this function required to be called when booting to kernel
 * from uefi/u-boot. Timer will still be running,
 * Brings down the link for which ever link is enabled and clear
 * the SCRATCHx register info so
 * when kernel is booting, it can start new.
 */
void cgx_fw_intf_shutdown(void)
{
	int init = 0;
	cgx_lmac_context_t *lmac_ctx;
	cgx_lmac_config_t *lmac_cfg;

	cavm_cgxx_cmrx_int_t cmrx_int;

	debug_cgx_intf("%s\n", __func__);

	/* bring down all the links and clear all SCRATCHX
	 * registers/context
	 */
	for (int cgx = 0; cgx < plat_octeontx_scfg->cgx_count; cgx++) {
		for (int lmac = 0; lmac < MAX_LMAC_PER_CGX; lmac++) {
			lmac_ctx = &lmac_context[cgx][lmac];
			lmac_cfg = &plat_octeontx_bcfg->cgx_cfg[cgx]
							.lmac_cfg[lmac];
			/* Bring down the link if link_enable is set.
			 * After bringing down the links, do one time
			 * initialization of the LMACs that are enabled
			 * so kernel can bring up the link.
			 * In case of QSGMII, always bring down the link
			 * for all the LMACs associated with the CGX
			 */
			init = 0;
			if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII) ||
				(lmac_ctx->s.link_enable)) {
				cgx_link_bringdown(cgx, lmac);
				mdelay(1);
				/* Set to indicate the link is
				 * initialized as the LMAC
				 * context structure will be reset
				 */
				init = 1;
			}
			CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(
					cgx, lmac, 0), 0);
			CSR_WRITE(CAVM_CGXX_CMRX_SCRATCHX(
					cgx, lmac, 1), 0);
			lmac_ctx->u64 = 0;
			if (init)
				lmac_ctx->s.init_link = 1;
			/* Clear the interrupt during shutdown for all
			 * LMACs as there might be a possibility that
			 * interrupts are not cleared by u-boot
			 * as it doesn't handle asynchronous events
			 */
			cmrx_int.u = CSR_READ(CAVM_CGXX_CMRX_INT(cgx, lmac));
			cmrx_int.s.overflw = 1;
			CSR_WRITE(CAVM_CGXX_CMRX_INT(cgx, lmac),
					cmrx_int.u);
		}
		for (int lmac = 0; lmac < MAX_LMAC_PER_CGX; lmac++) {
			lmac_ctx = &lmac_context[cgx][lmac];
			lmac_cfg = &plat_octeontx_bcfg->cgx_cfg[cgx]
							.lmac_cfg[lmac];
			/* Now for each CGX, initialize the link for
			 * each LMAC if the link was brought down
			 */
			if (lmac_ctx->s.init_link)
				cgx_lmac_init_link(cgx, lmac);
			/* Set init_link = 1 for all enabled LMACs as
			 * the link is already initialized but never
			 * brought up/down
			 */
			if (lmac_cfg->lmac_enable)
				lmac_ctx->s.init_link = 1;
		}
	}
}
