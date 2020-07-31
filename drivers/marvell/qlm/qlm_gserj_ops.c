/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <octeontx_common.h>
#include <octeontx_utils.h>
#include <qlm/qlm.h>
#include <qlm/qlm_gserj.h>
#include <plat_otx2_configuration.h>
#include <gser_internal.h>

/* Each chip has its own QLM operation table */
const qlm_ops_t qlm_ops_f95mm = {
	.type = QLM_GSERJ_TYPE,
	.qlm_enable_prbs = qlm_gserj_enable_prbs,
	.qlm_disable_prbs = qlm_gserj_disable_prbs,
	.qlm_get_prbs_errors = qlm_gserj_get_prbs_errors,
	.qlm_eye_capture = qlm_gserj_eye_capture,
	.qlm_display_settings = qlm_gserj_display_settings,
	.qlm_tune_lane_tx = qlm_gserj_tune_lane_tx,
	.qlm_ned_loopback = qlm_gserj_ned_loopback,
	.qlm_fea_loopback = qlm_gserj_fea_loopback,
};
