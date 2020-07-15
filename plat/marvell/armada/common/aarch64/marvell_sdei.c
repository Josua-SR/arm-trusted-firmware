/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* SDEI configuration for ARM platforms */

#include <platform_def.h>

#include <bl31/ehf.h>
#include <services/sdei.h>

/* Private event mappings */
static sdei_ev_map_t marvell_sdei_private[] = {
	PLAT_ARMADA_PRIVATE_SDEI_EVENTS
};

/* Shared event mappings */
static sdei_ev_map_t marvell_sdei_shared[] = {
	PLAT_ARMADA_SHARED_SDEI_EVENTS
};

/* Export ARM SDEI events */
REGISTER_SDEI_MAP(marvell_sdei_private, marvell_sdei_shared);
