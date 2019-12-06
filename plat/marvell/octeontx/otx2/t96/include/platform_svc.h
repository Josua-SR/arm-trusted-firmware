/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __PLATFORM_SVC_H__
#define __PLATFORM_SVC_H__

#include <plat_svc.h>

/* OcteonTX Service Calls version numbers */
#define PLAT_OCTEONTX_VERSION_MAJOR	0x1
#define PLAT_OCTEONTX_VERSION_MINOR	0x2

#define OCTEONTX_ATTESTATION_QUERY		0xc2000d03

/* X1 - if <= 0, query for attestation buffer address.
 *      if > 0, specifies nonce len
 */
#define OCTEONTX_ATTESTATION_QUERY_SUBCMD_LEGACY 0

/* no args */
#define OCTEONTX_ATTESTATION_QUERY_SUBCMD_BUFFER 1

/* no args */
#define OCTEONTX_ATTESTATION_QUERY_SUBCMD_INFO   2

/* Number of platform specific SMCs */
#define PLAT_OCTEONTX_NUM_SMC_CALLS	(1 + OTX2_NUM_SMC_CALLS)

#endif /* __PLATFORM_SVC_H__ */
