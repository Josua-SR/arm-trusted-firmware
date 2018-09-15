/** @file

    Copyright (c) 2016-2018, Marvell International Ltd. All rights reserved.<BR>
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution.  The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.php

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __F95_PLAT_SETUP_H__
#define __F95_PLAT_SETUP_H__

/*
 * Defines used for CNF95xx to enable particular
 * LMC access to particular ASC_REGION
 */

#define CAVM_CCS_LMC_MASK_LMC0	0x1
#define CAVM_CCS_LMC_MASK_LMC2	0x4

/* 2 LMCs per node */
#define CAVM_CCS_LMC_MODE	CAVM_CCS_LMC_MODE_E_STRIPE_2
#define CAVM_CCS_LMC_MASK	(CAVM_CCS_LMC_MASK_LMC0 | CAVM_CCS_LMC_MASK_LMC2)

/*
 * The array mapping platform core position (implemented by plat_my_core_pos())
 * to the SCMI power domain ID implemented by SCP.
 *
 * Right now it's pretty dummy, ATF defines PLAT_NUM_PWR_DOMAINS
 * as CORES+CLUSTERS+NODES (for f95, it's 8).
 * Right now use one PWR_DOMAIN per core.
 */
static const uint32_t plat_cavm_core_pos_to_scmi_dmn_id_map[PLATFORM_CORE_COUNT] = {
				0, 1, 2, 3, 4, 5};

/* CPU topology tree description for F95 */
static const unsigned char cavm_power_domain_tree_desc[] = {
	/* No of root nodes */
	PLATFORM_MAX_NODES,
	/* No of clusters */
	PLATFORM_MAX_CLUSTERS_PER_NODE,
	/* No of CPU cores */
	PLATFORM_MAX_CPUS_PER_CLUSTER,
};

/* plat APIs specific to Octeon TX2 family */
int plat_get_max_lane_num(int qlm);
int plat_get_cgx_idx(int qlm);

#endif /* __F95_PLAT_SETUP_H__ */
