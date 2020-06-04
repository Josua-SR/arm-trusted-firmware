/*
 * Copyright (C) 2016-2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef _PLATFORM_DT_H_
#define _PLATFORM_DT_H_

#define MAX_CGX			5

#define MAX_LMAC_PER_CGX	4

/* Total number of Hardware VFs */
#define MAX_RVU_HWVFS		256

/* Maximum number of RVU VFs per PF */
#define MAX_VFS_PER_PF		128

/* Global, default number of VFs */
#define DEFAULT_VFS		3

/* Default number of VFs for PF0 (admin PF) */
#define DEFAULT_AF_PF0_VFS	0

/* Default number of VFs for PF15 (CPT PF) */
#define DEFAULT_CPT_PF15_VFS	64

/* Default number of MSIX vectors for software
 * provisioned (SSO_TIM/NPA) RVU PFs */
#define DEFAULT_MSIX_SW		133

/*
 * Default number of MSIX vectors for LMAC RVU PFs, based on
 * NPA_LF (66) + NIX_LF (131) + RVU_PF (13) = 210, which addresses
 * the bare minimum configuration of NIX + NPA.
 */
#define DEFAULT_MSIX_LMAC	210

/*
 * Default number of MSIX vectors for AF PF (must match DTS rvu-admin node).
 * 5 CAVM_RVU_AF_INT_VEC_E_xxx
 * 7 CAVM_RVU_PF_INT_VEC_E_xxx
 * 2 * 5 CAVM_NIX_AF_INT_VEC_E_xxx
 * 5 CAVM_NPA_AF_INT_VEC_E_xxx
 * 3 CAVM_SSO_AF_INT_VEC_E_xxx
 * 5 CAVM_TIM_AF_INT_VEC_E_xxx
 * 3 CAVM_NDC_AF_INT_VEC_E_xxx
 * 4 CAVM_CPT_AF_INT_VEC_E_xxx
 * 2 * 4 CAVM_REE_AF_INT_VEC_E_xxx
 */
#define DEFAULT_MSIX_AF		50

/* RVU node names definitions */
#define RVU_ADMIN_FDT_NODE	"rvu-admin@0"
#define RVU_SSO_TIM_FDT_NODE	"rvu-sso-tim@0"
#define RVU_NPA_FDT_NODE	"rvu-npa@0"
#define RVU_CPT_FDT_NODE	"rvu-cpt@0"
#define RVU_SDP_FDT_NODE	"rvu-sdp@0"
#define RVU_REE_FDT_NODE	"rvu-ree@0"

#endif
