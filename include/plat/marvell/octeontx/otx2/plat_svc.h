/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __PLAT_SVC_H__
#define __PLAT_SVC_H__

/*
 * No input
 * Return:
 *	0x0 -- Success
 */
#define PLAT_OCTEONTX_DISABLE_RVU_LFS	0xc2000b01

#define PLAT_OCTEONTX_FLSF_FW_BOOTED	0xc2000b02
/*
 * No input
 * Return:
 *	0x0 -- Success
 */
#define PLAT_OCTEONTX_FLSF_CLEAR_FORCE_2NDRY	0xc2000b03
/*
 * x1 - ooo
 * Return:
 *	0x0 -- Success
 *	0x2 -- Fail
 */
#define PLAT_OCTEONTX_OOO_CONFIG	0xc2000b04

/*
 * No input
 * Return:
 *	x0:
 *		0x0 -- Success
 *		0x2 -- Fail
 *	x1 - in case of success - address to eye cmd data
 *	x2 - in case of success - address to serdes cmd data
 */
#define PLAT_OCTEONTX_SERDES_DBG_GET_MEM	0xc2000d04

/*
 * x1 - qlm
 * x2 - lane
 * x3 - display data to console
 * Return:
 *	x0:
 *		0x0 -- Success
 *		0x1 -- Pending
 *		0x2 -- Fail
 */
#define PLAT_OCTEONTX_SERDES_DBG_GET_EYE	0xc2000d05

/*
 * x1 - qlm
 * x2 - lane
 * x3 - display data to console
 * Return:
 *	x0:
 *		0x0 -- Success
 *		0x2 -- Fail
 */
#define PLAT_OCTEONTX_SERDES_DBG_GET_CONF	0xc2000d06

/*
 * x1 - cmd
 *     1 - CGX_PRBS_START_CMD - start prbs for qlm(x2) in mode(x3)
 *     2 - CGX_PRBS_STOP_CMD  - stop prbs for qlm(x2)
 *     3 - CGX_PRBS_GET_DATA_CMD - save errors for running prbs for qlm(x2)
 *                                 under SERDES_PRBS_DATA_BASE address.
 * x2 - qlm
 * x3 - if cmd == 1 then mode else ignored
 * Return:
 *	x0:
 *		0x0 -- Success
 *		0x2 -- Fail
 */
#define PLAT_OCTEONTX_SERDES_DBG_PRBS		0xc2000d07

/*
 * x1 - qlm
 * x2 - lane
 * x3 - tx_swing
 * x4 - tx_pre
 * x5 - tx_post
 * Return:
 *	x0:
 *		0x0 -- Success
 *		0x2 -- Fail
 */
#define PLAT_OCTEONTX_SERDES_DBG_SET_CONF	0xc2000d08

/*
 * x1 - qlm
 * x2 - lane
 * x3 - flags
 * Return:
 *	x0:
 *		0x0 -- Success
 *		0x2 -- Fail
 */
#define PLAT_OCTEONTX_SERDES_DBG_SET_LOOP	0xc2000d09

/* Number of family specific SMCs */
#define OTX2_NUM_SMC_CALLS			13

/* API that allows to define platform specific SMC CALLS */
uintptr_t otx2_svc_smc_handler(uint32_t smc_fid,
			       u_register_t x1,
			       u_register_t x2,
			       u_register_t x3,
			       u_register_t x4,
			       void *cookie,
			       void *handle,
			       u_register_t flags);

int octeontx2_configure_ooo(uint64_t x1);

#endif /* __PLAT_SVC_H__ */

