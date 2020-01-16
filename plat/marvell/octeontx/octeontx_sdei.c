/*
 * Copyright (C) 2016-2020 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

/* SDEI configuration for OcteonTX platforms */

#include <platform_def.h>
#include <platform_irqs_def.h>
#include <octeontx_sdei.h>

#include <bl31/ehf.h>
#include <services/sdei.h>

/* Private event mappings */
static sdei_ev_map_t octeontx_sdei_private[] = {
	OCTEONTX_SDEI_PRIVATE_EVENTS
};

/* Shared event mappings */
static sdei_ev_map_t octeontx_sdei_shared[] = {
	OCTEONTX_SDEI_SHARED_EVENTS
};

/* Export ARM SDEI events */
REGISTER_SDEI_MAP(octeontx_sdei_private, octeontx_sdei_shared);
