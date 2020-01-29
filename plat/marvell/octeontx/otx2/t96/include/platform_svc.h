/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __PLATFORM_SVC_H__
#define __PLATFORM_SVC_H__

/* OcteonTX Service Calls version numbers */
#define PLAT_OCTEONTX_VERSION_MAJOR	0x1
#define PLAT_OCTEONTX_VERSION_MINOR	0x2

/* x1 - node number */
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
 * X1 - CGX_LMAC_ID [0:3 LMAC, 4:7 CGX], X2 - CLAUSE MODE, X3 - phy address,
 * X4 - device address, X5 - register address
 */
#define PLAT_OCTEONTX_MDIO_DBG_READ		0xc2000d01

/*
 * X1 - CGX_LMAC_ID [0:3 LMAC, 4:7 CGX], X2 - CLAUSE MODE, X3 - phy address,
 * X4 - device address, X5 - register address, X6 - register value
 */
#define PLAT_OCTEONTX_MDIO_DBG_WRITE		0xc2000d02

#define OCTEONTX_ATTESTATION_QUERY		0xc2000d03

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

/* X1 - if <= 0, query for attestation buffer address.
 *      if > 0, specifies nonce len
 */
#define OCTEONTX_ATTESTATION_QUERY_SUBCMD_LEGACY 0

/* no args */
#define OCTEONTX_ATTESTATION_QUERY_SUBCMD_BUFFER 1

/* no args */
#define OCTEONTX_ATTESTATION_QUERY_SUBCMD_INFO   2

/* Number of platform specific SMCs */
#define PLAT_OCTEONTX_NUM_SMC_CALLS	8

int octeontx2_configure_ooo(int x1);

int mdio_debug_read(int cgxlmac_id, int mode, int phyaddr, int devad,
		    int regnum);
int mdio_debug_write(int cgxlmac_id, int mode, int phyaddr, int devad,
		     int regnum, uint16_t value);
#endif /* __PLATFORM_SVC_H__ */
