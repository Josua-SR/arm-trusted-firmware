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
#define PLAT_OCTEONTX_VERSION_MINOR	0x3

/* x1 - node number */
#define PLAT_OCTEONTX_DISABLE_RVU_LFS		0xc2000b01

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
#define PLAT_OCTEONTX_OOO_CONFIG		0xc2000b04


#define PLAT_OCTEONTX_INSTALL_BPHY_PSM_ERRINT	0xc2000803
#define PLAT_OCTEONTX_REMOVE_BPHY_PSM_ERRINT	0xc2000804

/* X1 - bus_num, X2 - phy address, X3 - register address */
#define PLAT_OCTEONTX_MDIO_DBG_READ		0xc2000d01

/* X1 - bus_num, X2 - phy address, X3 - register address, X4 - data */
#define PLAT_OCTEONTX_MDIO_DBG_WRITE		0xc2000d02

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

/* Number of platform specific SMCs */
#define PLAT_OCTEONTX_NUM_SMC_CALLS	11

int octeontx2_configure_ooo(int x1);

int mdio_debug_read(int cgxlmac_id, int mode, int phyaddr, int devad,
		    int regnum);
int mdio_debug_write(int cgxlmac_id, int mode, int phyaddr, int devad,
		     int regnum, uint16_t value);
#endif /* __PLATFORM_SVC_H__ */
