/*
 * Copyright (C) 2021 Marvell
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __PLATFORM_SVC_H__
#define __PLATFORM_SVC_H__

#include <plat_svc.h>

/* OcteonTX Service Calls version numbers */
#define PLAT_OCTEONTX_VERSION_MAJOR	0x2
#define PLAT_OCTEONTX_VERSION_MINOR	0x0

#define PLAT_OCTEONTX_INSTALL_BPHY_PSM_ERRINT	0xc2000803
#define PLAT_OCTEONTX_REMOVE_BPHY_PSM_ERRINT	0xc2000804

/* Number of platform specific SMCs */
#define PLAT_OCTEONTX_NUM_SMC_CALLS	(2 + OTX2_NUM_SMC_CALLS)

#endif /* __PLATFORM_SVC_H__ */
