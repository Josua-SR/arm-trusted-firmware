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
#include <rvu.h>
#include <plat_board_cfg.h>
#include <plat_scmi.h>
#include <cgx.h>
#if ENABLE_ATTESTATION_SERVICE
#include <octeontx_attestation.h>
#endif

extern void *scmi_handle;

uintptr_t plat_octeontx_svc_smc_handler(uint32_t smc_fid,
					u_register_t x1,
					u_register_t x2,
					u_register_t x3,
					u_register_t x4,
					void *cookie,
					void *handle,
					u_register_t flags)
{
	uint64_t ret = 0;
	const gp_regs_t *sregs = get_gpregs_ctx(handle);
	u_register_t x5, x6;

	switch (smc_fid) {
	case PLAT_OCTEONTX_DISABLE_RVU_LFS:
		ret = octeontx2_clear_lf_to_pf_mapping(x1);
		SMC_RET1(handle, ret);
		break;

	case PLAT_OCTEONTX_FLSF_FW_BOOTED:
		ret = scmi_octeontx_flsf_fw_booted(scmi_handle);
		SMC_RET1(handle, ret);
		break;

	case PLAT_OCTEONTX_FLSF_CLEAR_FORCE_2NDRY:
		ret = scmi_octeontx_flsf_clear_force_2ndry(scmi_handle);
		SMC_RET1(handle, ret);
		break;

	case PLAT_OCTEONTX_OOO_CONFIG:
		INFO("SVC OOO CONFIG: x1 = 0x%lx\n", x1);
		ret = octeontx2_configure_ooo(x1);
		SMC_RET1(handle, ret);
		break;

	case PLAT_OCTEONTX_MDIO_DBG_READ:
		x5 = read_ctx_reg(sregs, CTX_GPREG_X5);
		ret = mdio_debug_read(x1, x2, x3, x4, x5);
		SMC_RET1(handle, ret);
		break;

	case PLAT_OCTEONTX_MDIO_DBG_WRITE:
		x5 = read_ctx_reg(sregs, CTX_GPREG_X5);
		x6 = read_ctx_reg(sregs, CTX_GPREG_X6);
		ret = mdio_debug_write(x1, x2, x3, x4, x5, x6);
		SMC_RET1(handle, ret);
		break;

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

#ifdef DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS
	case PLAT_OCTEONTX_SERDES_DBG_GET_EYE:
		ret = cgx_display_eye(x1, x2, x3);
		if (ret == CGX_DISPLAY_OK)
			ret = SMC_OK;
		else if (ret == CGX_DISPLAY_PENDING)
			ret = OCTEONTX_SMC_PENDING;
		else
			ret = OCTEONTX_SMC_FAIL;

		SMC_RET1(handle, ret);
		break;

	case PLAT_OCTEONTX_SERDES_DBG_GET_CONF:
		ret = cgx_display_serdes_settings(x1, x2, x3);
		ret = ret == CGX_DISPLAY_OK ? SMC_OK : OCTEONTX_SMC_FAIL;

		SMC_RET1(handle, ret);
		break;
#endif /* DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS */

	default:
		WARN("Unimplemented OcteonTX Service Call: 0x%x\n", smc_fid);
		SMC_RET1(handle, SMC_UNK);
	}
}
