/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <debug.h>
#include <runtime_svc.h>
#include <octeontx_svc.h>
#include <platform_svc.h>
#include <octeontx_common.h>
#include <stdint.h>
#include <tools_share/uuid.h>
#if ENABLE_ATTESTATION_SERVICE
#include <octeontx_attestation.h>
#endif

uintptr_t otx2_svc_smc_handler(uint32_t smc_fid,
			       u_register_t x1,
			       u_register_t x2,
			       u_register_t x3,
			       u_register_t x4,
			       void *cookie,
			       void *handle,
			       u_register_t flags)
{
#if ENABLE_ATTESTATION_SERVICE
	uint64_t ret = 0;
#endif

	switch (smc_fid) {
#if ENABLE_ATTESTATION_SERVICE
	case OCTEONTX_ATTESTATION_QUERY:
		if (x2 == OCTEONTX_ATTESTATION_QUERY_SUBCMD_LEGACY) {
			/* deprecated subcmd */
			ret = SMC_UNK;
		} else if (x2 == OCTEONTX_ATTESTATION_QUERY_SUBCMD_BUFFER) {
			ret = (uintptr_t)attestation_info_base();
		} else if (x2 == OCTEONTX_ATTESTATION_QUERY_SUBCMD_INFO) {
			ret = generate_attestation_info(x1);
		} else {
			/* Unknown subcmd */
			ret = SMC_UNK;
		}

		SMC_RET1(handle, ret);
		break;
#endif

	default:
		WARN("Unimplemented OcteonTX Service Call: 0x%x\n", smc_fid);
		SMC_RET1(handle, SMC_UNK);
	}
}
