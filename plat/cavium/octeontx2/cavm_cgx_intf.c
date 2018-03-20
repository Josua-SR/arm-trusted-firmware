/** @file

  Copyright (c) 2018, Cavium Inc. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <arch.h>
#include <stdio.h>
#include <debug.h>
#include <string.h>
#include <platform_def.h>
#include <timers.h>
#include <cavm_common.h>
#include <delay_timer.h>
#include <cavm_dt.h>
#include <cavm_cgx_intf.h>
#include <cavm_cgx.h>

#define MAX_CGX_TIMERS 2

static int cgx_timers[MAX_CGX_TIMERS];

static cgx_lmac_context_t
		lmac_context[PLATFORM_MAX_NODES][MAX_CGX][MAX_LMAC_PER_CGX];

static uint64_t cgx_get_error_type(int node, int cgx_id, int lmac_id)
{
	cgx_lmac_context_t *lmac_ctx;

	lmac_ctx = &lmac_context[node][cgx_id][lmac_id];
	INFO("%s: %d:%d:%d error_type 0x%x\n", __func__, node, cgx_id, lmac_id,
				lmac_ctx->s.error_type);
	return lmac_ctx->s.error_type;
}

static int cgx_trigger_interrupt(int node, int cgx_id, int lmac_id)
{
	INFO("%s %d:%d:%d\n", __func__, node, cgx_id, lmac_id);

	/* Enable the interrupt bit each time before triggering
	 * an interrupt. In case of Link down request, enable bit for
	 * LMAC(CGXX_CMRX_CONFIG(enable)) is disabled
	 * which clears all the dedicated CGX context
	 * state for the LMAC. Hence, need to explicitly enable
	 * the interrupt every time.
	 */
	CAVM_MODIFY_CGX_CSR(node, cavm_cgxx_cmrx_int_ena_w1s_t,
				CAVM_CGXX_CMRX_INT_ENA_W1S(cgx_id, lmac_id),
				overflw, 1);

	/* FIXME: trigger an interrupt to kernel
	 * via CMRX overflow int bit - Hardware-opaque vector
	 * for software driver (37th MSIX vector) doesn't have interrupt
	 * support yet. this is a workaround for the 1st pass
	 */
	CAVM_MODIFY_CGX_CSR(node, cavm_cgxx_cmrx_int_w1s_t,
				CAVM_CGXX_CMRX_INT_W1S(cgx_id, lmac_id),
				overflw, 1);
	return 0;
}

static int cgx_acquire_csr_lock(int node, int cgx_id, int lmac_id)
{
	int timeout = 10; /* loop for few times but not infinitely */
	cgx_lmac_context_t *lmac_ctx;

	INFO("%s %d:%d:%d\n", __func__, node, cgx_id, lmac_id);

	lmac_ctx = &lmac_context[node][cgx_id][lmac_id];

	do {
		if (!lmac_ctx->s.lock) {
			lmac_ctx->s.lock = 1;
			return 0;
		}
		udelay(1);
	} while (timeout--);

	return -1;
}

static void cgx_release_csr_lock(int node, int cgx_id, int lmac_id)
{
	INFO("%s %d:%d:%d\n", __func__, node, cgx_id, lmac_id);
	lmac_context[node][cgx_id][lmac_id].s.lock = 0;
}

static void cgx_release_own_status(int node, int cgx_id, int lmac_id)
{
	CAVM_MODIFY_CGX_CSR(node, union cgx_scratchx1,
			CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 1),
			own_status, 0); /* released the ownership */
}

static void cgx_set_link_state(int node, int cgx_id, int lmac_id,
					link_state_t *link, uint64_t err_type)
{
	union cgx_scratchx0 scratchx0;

	INFO("%s %d:%d:%d link_up %d speed %d duplex %d err_type %ld\n",
			__func__, node, cgx_id, lmac_id,
			link->s.link_up, link->s.speed,
			link->s.full_duplex, err_type);

	scratchx0.u = CSR_READ(node, CAVM_CGXX_CMRX_SCRATCHX(
					cgx_id, lmac_id, 0));
	scratchx0.s.link_sts.link_up = link->s.link_up;
	scratchx0.s.link_sts.speed = link->s.speed;
	scratchx0.s.link_sts.full_duplex = link->s.full_duplex;
	scratchx0.s.link_sts.err_type = err_type;
	CSR_WRITE(node, CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 0),
				scratchx0.u);
}

static int cgx_link_change(int node, int cgx_id, int lmac_id,
			link_state_t *link)
{
	int ret = 0;
	cgx_lmac_config_t *lmac_cfg;

	/* get the lmac type and based on lmac
	 * type, change the link settings
	 */
	lmac_cfg = &bfdt->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	INFO("%s %d:%d:%d lmac_type %d\n", __func__, node, cgx_id,
			lmac_id, lmac_cfg->mode);

	if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
		ret = cgx_sgmii_set_link_speed(node, cgx_id,
						lmac_id, link);
	} else if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_XAUI) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_RXAUI) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R)) {
		ret = cgx_xaui_set_link_up(node, cgx_id, lmac_id);
	} else {
		/* FIXME: handle new modes as well */
	}

	/* update the current link status along with any error type set */
	cgx_set_link_state(node, cgx_id, lmac_id, link,
		cgx_get_error_type(node, cgx_id, lmac_id));

	return ret;
}

static int cgx_link_bringup(int node, int cgx_id, int lmac_id)
{
	cgx_lmac_config_t *lmac_cfg;
	cgx_lmac_context_t *lmac_ctx;
	link_state_t link = {0};
	int count = 0;

	/* get the lmac type and based on lmac
	 * type, initialize SGMII/XAUI link
	 */
	lmac_cfg = &bfdt->cgx_cfg[cgx_id].lmac_cfg[lmac_id];

	INFO("%s %d:%d:%d lmac_type %d\n", __func__, node, cgx_id,
			lmac_id, lmac_cfg->mode);

	lmac_ctx = &lmac_context[node][cgx_id][lmac_id];
	/* link_enable will be set when the LINK UP req is processed.
	 * To avoid processing duplication of requests, check for it
	 * and return the previous link status
	 */
	if (lmac_ctx->s.link_enable == 1) {
		INFO("%s: Link status for %d:%d is already up\n",
			__func__, cgx_id, lmac_id);
		link.s.link_up = lmac_ctx->s.link_up;
		link.s.full_duplex = lmac_ctx->s.full_duplex;
		link.s.speed = lmac_ctx->s.speed;
		cgx_set_link_state(node, cgx_id, lmac_id, &link,
			cgx_get_error_type(node, cgx_id, lmac_id));
		return 0;
	}

	if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
		/* sequence if PHY is present:
		 *	- get PHY status,
		 *	- If link is up, init link and set speed
		 *	- If link is not up, return error
		 * sequence if PHY is not present(same if internal lbk enabled):
		 *	- assign default link status,
		 *	- init link and set speed
		 */
		if (lmac_ctx->s.lbk1_enable || !lmac_cfg->phy_present) {
			link.s.link_up = 1;
			link.s.full_duplex = 1;
			link.s.speed = CGX_LINK_1G;
		} else {
			/* FIXME: interface with PHY management to get
			 * the link status if phy is present. PHY management
			 * firmware will provide link status info. Currently
			 * not supported
			 */
		}
		if (link.s.link_up == 1) {	/* link is up */
			if (cgx_sgmii_set_link_up(node, cgx_id, lmac_id) != 0)
				/* error occurred, skip further */
				goto cgx_err;

			if (cgx_sgmii_set_link_speed(node, cgx_id,
						lmac_id, &link) != 0)
				goto cgx_err;

			/* SUCCESS case : update the link status and indicate
			 * poll timer to start polling for the link
			 */
			lmac_ctx->s.link_up = link.s.link_up;
			lmac_ctx->s.full_duplex = link.s.full_duplex;
			lmac_ctx->s.speed = link.s.speed;
			lmac_ctx->s.link_enable = 1;
			cgx_set_link_state(node, cgx_id, lmac_id, &link, 0);

			return 0;

		} else {
			/* if the link is not up, return error */
			ERROR("%s : PHY link status is down for LMAC%d\n",
				__func__, lmac_id);
			cgx_set_error_type(node, cgx_id, lmac_id,
				CGX_ERR_PHY_LINK_DOWN);
			return -1;
		}
	} else if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_XAUI) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_RXAUI) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R)) {

		/* initialize the link, get the link status,
		 * If link is up, check AN and perform training
		 */
		if (cgx_xaui_init_link(node, cgx_id, lmac_id) != 0) {
			/* This API will fail only if there is PCS reset
			 * error. Hence set error type with the LINK status
			 * as down and return
			 */
			cgx_set_link_state(node, cgx_id, lmac_id, &link,
				cgx_get_error_type(node, cgx_id, lmac_id));
			return -1;
		}

		/* PHY not present, internal loopback case */
		if (lmac_ctx->s.lbk1_enable || !lmac_cfg->phy_present) {
			link.s.link_up = 1;
			link.s.full_duplex = 1;
			if (lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R)
				link.s.speed = CGX_LINK_40G;
			else
				link.s.speed = CGX_LINK_10G;
		}
retry_link:
		if (cgx_xaui_set_link_up(node, cgx_id, lmac_id) == -1) {
			/* if init link fails, retry */
			if (count++ < 5) {
				WARN("%s retrying link\n", __func__);
				/* clear the error when retrying */
				cgx_set_error_type(node, cgx_id, lmac_id, 0);
				goto retry_link;
			}
		}

		cgx_xaui_get_link(node, cgx_id, lmac_id, &link);
		if (link.s.link_up == 1) {	/* link is up */
			if (cgx_get_error_type(node, cgx_id, lmac_id) &
						CGX_ERR_MASK)
				goto cgx_err;
			/* SUCCESS case : update the link status and indicate
			 * poll timer to start polling for link
			 */
			lmac_ctx->s.link_up = link.s.link_up;
			lmac_ctx->s.full_duplex = link.s.full_duplex;
			lmac_ctx->s.speed = link.s.speed;
			lmac_ctx->s.link_enable = 1;
			cgx_set_link_state(node, cgx_id, lmac_id, &link, 0);
			return 0;
		} else {
			/* link is down */
			ERROR("%s: link status is down for LMAC%d\n",
				__func__, lmac_id);
			cgx_set_error_type(node, cgx_id, lmac_id,
					CGX_ERR_PHY_LINK_DOWN);
			return -1;
		}
	} else { /* FIXME : add support for new modes */
		ERROR("%s LMAC%d mode %d not configured correctly,"
			" cannot initialize link\n",
			__func__, lmac_id, lmac_cfg->mode);
		cgx_set_error_type(node, cgx_id, lmac_id,
			CGX_ERR_LMAC_MODE_INVALID);
		return -1;
	}

cgx_err:
	/* If the link is up, but the CGX configuration failed, reach here
	 * In that case, update the link status along with the error type
	 * in lmac_context structure and the SCRATCHX CSRs and notify
	 * poll timer to start polling for the link
	 */
	lmac_ctx->s.link_up = link.s.link_up;
	lmac_ctx->s.full_duplex = link.s.full_duplex;
	lmac_ctx->s.speed = link.s.speed;
	cgx_set_link_state(node, cgx_id, lmac_id, &link,
			cgx_get_error_type(node, cgx_id, lmac_id));

	lmac_ctx->s.link_enable = 1;

	return -1;
}

static int cgx_link_bringdown(int node, int cgx_id, int lmac_id)
{
	cgx_lmac_config_t *lmac_cfg;
	cgx_lmac_context_t *lmac_ctx;
	link_state_t link;

	/* get the lmac type and based on lmac
	 * type, bring down SGMII/XAUI link
	 */
	lmac_cfg = &bfdt->cgx_cfg[cgx_id].lmac_cfg[lmac_id];
	INFO("%s %d:%d:%d lmac_type %d\n", __func__, node,
				cgx_id, lmac_id, lmac_cfg->mode);

	lmac_ctx = &lmac_context[node][cgx_id][lmac_id];

	/* link_enable will be cleared when the LINK DOWN req is processed
	 * To avoid processing duplication of requests, check for it
	 * and return the previous link status
	 */
	if (lmac_ctx->s.link_enable == 0) {
		INFO("%s: Link status for %d:%d is already down\n",
			__func__, cgx_id, lmac_id);
		link.s.link_up = lmac_ctx->s.link_up;
		link.s.full_duplex = lmac_ctx->s.full_duplex;
		link.s.speed = lmac_ctx->s.speed;
		cgx_set_link_state(node, cgx_id, lmac_id, &link,
			cgx_get_error_type(node, cgx_id, lmac_id));
		return 0;
	}

	if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
		if (cgx_sgmii_set_link_down(node, cgx_id, lmac_id) != 0)
			return -1;

	} else if ((lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_XAUI) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_RXAUI) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_TENG_R) ||
		(lmac_cfg->mode == CAVM_CGX_LMAC_TYPES_E_FORTYG_R)) {
		if (cgx_xaui_set_link_down(node, cgx_id, lmac_id) != 0)
			return -1;

	} else { /* FIXME : add support for new modes */
		ERROR("%s LMAC%d mode %d not configured correctly"
			" cannot bring down the link\n",
			__func__, lmac_id, lmac_cfg->mode);
		cgx_set_error_type(node, cgx_id, lmac_id,
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
	cgx_set_link_state(node, cgx_id, lmac_id, &link,
			cgx_get_error_type(node, cgx_id, lmac_id));

	lmac_ctx->s.link_enable = 0;

	return 0;
}

/* Note : this function executes with lock acquired */
static int cgx_process_requests(int node, int cgx_id, int lmac_id)
{
	int ret = 0;
	int enable = 0; /* read from scratch1 - cmd_args */
	int request_id = 0;
	union cgx_scratchx0 scratchx0;
	union cgx_scratchx1 scratchx1;
	link_state_t new_link, link;
	cgx_lmac_context_t *lmac_ctx;
	cgx_lmac_config_t *lmac;

	lmac = &bfdt->cgx_cfg[cgx_id].lmac_cfg[lmac_id];
	lmac_ctx = &lmac_context[node][cgx_id][lmac_id];

	/* Read the command arguments from SCRATCHX(1) */
	scratchx1.u = CSR_READ(node, CAVM_CGXX_CMRX_SCRATCHX(
					cgx_id, lmac_id, 1));
	request_id = scratchx1.s.cmd.id;
	enable = scratchx1.s.cmd_args.enable;
	new_link.s.link_up = scratchx1.s.lnk_args.link_up;
	new_link.s.full_duplex = scratchx1.s.lnk_args.full_duplex;
	new_link.s.speed = scratchx1.s.lnk_args.speed;

	INFO("%s: %d:%d:%d request_id %d\n", __func__, node, cgx_id,
				lmac_id, request_id);

	/* always reset the error bits when processing new
	 * command except when obtaining current status
	 */
	if (request_id != CGX_CMD_GET_LINK_STS)
		cgx_set_error_type(node, cgx_id, lmac_id, 0);

	if (lmac->lmac_enable) {
		switch (request_id) {
		case CGX_CMD_LINK_BRING_UP:
			ret = cgx_link_bringup(node, cgx_id, lmac_id);
			break;
		case CGX_CMD_LINK_BRING_DOWN:
			ret = cgx_link_bringdown(node, cgx_id, lmac_id);
			break;
		case CGX_CMD_LINK_STAT_CHANGE:
			ret = cgx_link_change(node, cgx_id, lmac_id, &new_link);
			break;
		case CGX_CMD_INTF_SHUTDOWN:
			cgx_fw_intf_shutdown();
			break;
		case CGX_CMD_INTERNAL_LBK:
			lmac_ctx->s.lbk1_enable = enable;
			cgx_set_internal_loopback(node, cgx_id, lmac_id,
						enable);
			break;
		case CGX_CMD_EXTERNAL_LBK:
			cgx_set_external_loopback(node, cgx_id, lmac_id,
						enable);
			break;
		case CGX_CMD_GET_FW_VER:
			scratchx0.u = 0;
			scratchx0.s.ver.major_ver = CGX_FIRWARE_MAJOR_VER;
			scratchx0.s.ver.minor_ver = CGX_FIRWARE_MINOR_VER;
			CSR_WRITE(node, CAVM_CGXX_CMRX_SCRATCHX(
					cgx_id, lmac_id, 0), scratchx0.u);
			break;
		case CGX_CMD_GET_LINK_STS:
			CSR_WRITE(node, CAVM_CGXX_CMRX_SCRATCHX(
					cgx_id, lmac_id, 0), 0); /* reset */
			link.s.link_up = lmac_ctx->s.link_up;
			link.s.full_duplex = lmac_ctx->s.full_duplex;
			link.s.speed = lmac_ctx->s.speed;
			cgx_set_link_state(node, cgx_id, lmac_id, &link,
				lmac_ctx->s.error_type);
			break;
		case CGX_CMD_GET_MAC_ADDR:
			scratchx0.u = 0;
			scratchx0.s.mac_s.addr_0 = lmac->local_mac_address[0];
			scratchx0.s.mac_s.addr_1 = lmac->local_mac_address[1];
			scratchx0.s.mac_s.addr_2 = lmac->local_mac_address[2];
			scratchx0.s.mac_s.addr_3 = lmac->local_mac_address[3];
			scratchx0.s.mac_s.addr_4 = lmac->local_mac_address[4];
			scratchx0.s.mac_s.addr_5 = lmac->local_mac_address[5];
			INFO("%s mac_addr %x:%x:%x:%x:%x:%x\n",	__func__,
					scratchx0.s.mac_s.addr_0,
					scratchx0.s.mac_s.addr_1,
					scratchx0.s.mac_s.addr_2,
					scratchx0.s.mac_s.addr_3,
					scratchx0.s.mac_s.addr_4,
					scratchx0.s.mac_s.addr_5);
			CSR_WRITE(node, CAVM_CGXX_CMRX_SCRATCHX(
					cgx_id, lmac_id, 0), scratchx0.u);
			break;
		/* FIXME: add support for other commands */
		default:
			ERROR("%s: Invalid ID %d\n", __func__, request_id);
			cgx_set_error_type(node, cgx_id, lmac_id,
				CGX_ERR_REQUEST_ID_INVALID);
			break;
		}
	} else {
		ERROR("%s: CGX%d LMAC%d is not enabled. Req %d ignored\n",
			__func__, cgx_id, lmac_id, request_id);
		cgx_set_error_type(node, cgx_id, lmac_id,
				CGX_ERR_LMAC_NOT_ENABLED);
	}

	/* update the event status either async or resp
	 * to command by writing to evt_sts struct
	 */
	scratchx0.u = CSR_READ(node, CAVM_CGXX_CMRX_SCRATCHX(
					cgx_id, lmac_id, 0));
	if ((cgx_get_error_type(node, cgx_id, lmac_id)) &
				CGX_ERR_MASK)
		scratchx0.s.evt_sts.stat = CGX_STAT_FAIL;
	else
		scratchx0.s.evt_sts.stat = CGX_STAT_SUCCESS;

	if (request_id == CGX_CMD_LINK_STAT_CHANGE) {
		/* handle this case differently as this is not user submitted
		 * request. It is from PHY obtained periodically
		 * polling the link status. Notify to user there is change
		 * in link status by setting event type as CGX_EVT_ASYNC
		 */
		scratchx0.s.evt_sts.id = CGX_EVT_LINK_CHANGE;
		scratchx0.s.evt_sts.evt_type = CGX_EVT_ASYNC;
	} else {
		/* For all other requests, update the command status, ID and
		 * set event type
		 */
		scratchx0.s.evt_sts.id = request_id;
		scratchx0.s.evt_sts.evt_type = CGX_EVT_CMD_RESP;
		if ((request_id != CGX_CMD_LINK_BRING_UP) &&
			(request_id != CGX_CMD_LINK_BRING_DOWN)) {
			/* in case of LINK_UP/DOWN, error type is updated
			 * as part of link status struct
			 */
			if ((cgx_get_error_type(node, cgx_id, lmac_id)) &
							CGX_ERR_MASK)
				scratchx0.s.err.type = cgx_get_error_type(
					node, cgx_id, lmac_id);
		}
	}
	scratchx0.s.evt_sts.ack = 1; /* set ack */
	CSR_WRITE(node, CAVM_CGXX_CMRX_SCRATCHX(
				cgx_id, lmac_id, 0), scratchx0.u);

	return ret;
}

static int cgx_handle_link_change(int node, int cgx_id, int lmac_id,
			link_state_t *new_link)
{
	int timeout = 10; /* check the moderate val */;
	union cgx_scratchx1 cgx_scratch1;
	union cgx_scratchx0 cgx_scratch0;
	cgx_lmac_context_t *lmac_ctx;

	INFO("%s: %d:%d:%d\n", __func__, node, cgx_id, lmac_id);

	/* if the owner ship is available and lock is free, check
	 * for the ack bit to be clear and post the command.
	 */
	do {
		cgx_scratch1.u = CSR_READ(node, CAVM_CGXX_CMRX_SCRATCHX(
					cgx_id, lmac_id, 1));
		cgx_scratch0.u = CSR_READ(node, CAVM_CGXX_CMRX_SCRATCHX(
					cgx_id, lmac_id, 0));
		INFO("%s, waiting for own %d and prev ack %d to be clear\n",
				__func__, cgx_scratch1.s.own_status,
				cgx_scratch0.s.evt_sts.ack);
		if ((!cgx_scratch1.s.own_status) &&
				(!cgx_scratch0.s.evt_sts.ack))
			break;
		udelay(1);
	} while (--timeout);

	if (!timeout) {
		/* not able to obtain the ownership of CSRs */
		WARN("%s %d:%d Owner ship not free to post LINK CHANGE req\n",
			__func__, cgx_id, lmac_id);
		return -1;
	}

	/* acquire the internal lock */
	if (cgx_acquire_csr_lock(node, cgx_id, lmac_id) == -1) {
		WARN("%s %d:%d lock not obtained to post LINK CHANGE req\n",
			 __func__, cgx_id, lmac_id);
		return -1;
	}

	lmac_ctx = &lmac_context[node][cgx_id][lmac_id];
	/* post the command */
	cgx_scratch1.u = CSR_READ(node, CAVM_CGXX_CMRX_SCRATCHX(
				cgx_id, lmac_id, 1));

	/* set the ownership to firmware and update the request ID */
	cgx_scratch1.s.own_status = CGX_OWN_FIRMWARE;
	cgx_scratch1.s.cmd.id = CGX_CMD_LINK_STAT_CHANGE;

	/* pass the new link state as an argument and update the
	 * link status in context structure when the request
	 * is being submitted
	 */
	lmac_ctx->s.link_up = cgx_scratch1.s.lnk_args.link_up =
				new_link->s.link_up;
	lmac_ctx->s.speed = cgx_scratch1.s.lnk_args.speed =
				new_link->s.speed;
	lmac_ctx->s.full_duplex = cgx_scratch1.s.lnk_args.full_duplex =
				new_link->s.full_duplex;

	CSR_WRITE(node, CAVM_CGXX_CMRX_SCRATCHX(cgx_id, lmac_id, 1),
					cgx_scratch1.u);
	/* release the internal lock, but still maintain the own status
	 * as FIRMWARE so that timer #2 can process the link
	 * change command
	 */
	cgx_release_csr_lock(node, cgx_id, lmac_id);

	return 0;
}

/* Timer callback to periodically poll for link */
static int cgx_poll_for_link_cb(int timer)
{
	cgx_lmac_config_t *lmac_cfg;
	cgx_lmac_context_t *lmac_ctx;
	link_state_t link = {0};

	for (int node = 0; node < PLATFORM_MAX_NODES; node++) {
		for (int cgx = 0; cgx < MAX_CGX; cgx++) {
			for (int lmac = 0; lmac < MAX_LMAC_PER_CGX; lmac++) {
				lmac_cfg = &bfdt->cgx_cfg[cgx].lmac_cfg[lmac];
				lmac_ctx = &lmac_context[node][cgx][lmac];
				if (lmac_ctx->s.link_enable) {
					/* check if PHY is present, if not
					 * return the default link status
					 */
					if (!lmac_cfg->phy_present) {
						INFO("%s:%d:%d:%d PHY not present\t",
							__func__, node, cgx, lmac);
						INFO("link %d speed %d duplex %d\n",
							lmac_ctx->s.link_up,
							lmac_ctx->s.speed,
						lmac_ctx->s.full_duplex);
						continue;
					}
					/* FIXME: to interface with PHY
					 * management and get the link status
					 */
					INFO("%s:%d:%d:%d poll for link status\n",
						__func__, node, cgx, lmac);
					if ((lmac_ctx->s.link_up !=
						link.s.link_up) ||
						(lmac_ctx->s.full_duplex !=
						link.s.full_duplex) ||
						(lmac_ctx->s.speed !=
						link.s.speed)) {
						INFO("%s:%d:%d link changed\n",
							__func__, cgx, lmac);
						cgx_handle_link_change(node,
							cgx, lmac, &link);
					}
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

	/* Go through all active LMACs and check
	 * if there are any new message requests by reading
	 * command register of each LMAC(SCRATCHX(1)
	 */
	for (int node = 0; node < PLATFORM_MAX_NODES; node++) {
		for (int cgx = 0; cgx < MAX_CGX; cgx++) {
			for (int lmac = 0; lmac < MAX_LMAC_PER_CGX; lmac++) {
				scratch1.u = CSR_READ(node,
					CAVM_CGXX_CMRX_SCRATCHX(cgx, lmac, 1));
				scratch0.u = CSR_READ(node,
					CAVM_CGXX_CMRX_SCRATCHX(cgx, lmac, 0));
				if (scratch1.s.own_status == CGX_OWN_FIRMWARE) {
					/* acquire firmware internal lock */
					if (cgx_acquire_csr_lock(node, cgx, lmac)
							== -1) {
						WARN("%s %d:%d lock not"
						" obtained to process command,"
						" wait for now\n",
						 __func__, cgx, lmac);
						/* skip to next LMAC */
						continue;
					}

					if (scratch0.s.evt_sts.ack) {
						ERROR("%s Req ignored,"
							" status not cleared\n",
							__func__);
						cgx_set_error_type(node, cgx, lmac,
						CGX_ERR_PREV_ACK_NOT_CLEAR);
						cgx_release_own_status(
								node, cgx, lmac);
						cgx_release_csr_lock(
								node, cgx, lmac);
						/* skip to next LMAC */
						continue;
					}

					cgx_process_requests(node, cgx, lmac);

					cgx_release_own_status(node, cgx, lmac);

					/* trigger an interrupt before ret */
					cgx_trigger_interrupt(node, cgx, lmac);

					/* release firmware internal lock */
					cgx_release_csr_lock(node, cgx, lmac);
				}
			}
		}
	}
	return 0;
}

/* this function to be called from any CGX function when major
 * error type is encountered
 */
void cgx_set_error_type(int node, int cgx_id, int lmac_id, uint64_t type)
{
	cgx_lmac_context_t *lmac_ctx;

	lmac_ctx = &lmac_context[node][cgx_id][lmac_id];
	INFO("%s:%d:%d:%d type 0x%lx\n", __func__, node, cgx_id, lmac_id, type);

	lmac_ctx->s.error_type = type;
}


/* This function should be called once during boot time */
void cgx_fw_intf_init(void)
{
	INFO("%s\n", __func__);

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
	cgx_lmac_context_t *lmac_ctx;

	INFO("%s\n", __func__);

	/* bring down all the links and clear all SCRATCHX
	 * registers/context
	 */
	for (int node = 0; node < PLATFORM_MAX_NODES; node++) {
		for (int cgx = 0; cgx < MAX_CGX; cgx++) {
			for (int lmac = 0; lmac < MAX_LMAC_PER_CGX; lmac++) {
				lmac_ctx = &lmac_context[node][cgx][lmac];
				/* bring down the link if link_enable is set */
				if (lmac_ctx->s.link_enable)
					cgx_link_bringdown(node, cgx, lmac);
				CSR_WRITE(node, CAVM_CGXX_CMRX_SCRATCHX(
						cgx, lmac, 0), 0);
				CSR_WRITE(node, CAVM_CGXX_CMRX_SCRATCHX(
						cgx, lmac, 1), 0);
				lmac_ctx->u64 = 0;
			}
		}
		/* RVU specific: enable LF to PF mapping done
		 * for NIX/NPA PFs. This is required for
		 * networking support in u-boot/uefi.
		 * clear them before booting to kernel
		 */
		octeontx2_clear_lf_to_pf_mapping(node);
	}
}
