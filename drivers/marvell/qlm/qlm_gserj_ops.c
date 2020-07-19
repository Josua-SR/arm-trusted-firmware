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

#define GSERR_MIN 0 /* QLM0 => GSERR */
#define GSERR_MAX 1 /* QLM1 => GSERR */
#define GSERJ_MIN 2 /* QLM2 => GSERJ */
#define GSERJ_MAX 9 /* QLM9 => GSERJ */

/**
 * Enable PRBS on a QLM
 *
 * @param qlm    QLM to use
 * @param prbs   PRBS mode (31, etc)
 * @param dir    Directions to enable. This is so you can enable TX and later
 *               enable RX after TX has run for a time
 *
 * @return Zero on success, negative on failure
 */
static int qlm_gserj_enable_prbs(int qlm, int prbs, qlm_direction_t dir, int qlm_lane)
{
    switch (qlm)
    {
        case GSERJ_MIN ... GSERJ_MAX: /* GSERJ */
            return _qlm_gserj_enable_prbs(qlm - GSERJ_MIN, prbs, dir, qlm_lane);
        default:
            return -1;
    }
}

/**
 * Disable PRBS on a QLM
 *
 * @param qlm    QLM to use
 *
 * @return Zero on success, negative on failure
 */
static int qlm_gserj_disable_prbs(int qlm, int qlm_lane)
{
    switch (qlm)
    {
        case GSERJ_MIN ... GSERJ_MAX: /* GSERJ */
            return _qlm_gserj_disable_prbs(qlm - GSERJ_MIN, qlm_lane);
        default:
            return -1;
    }
}

/**
 * Return the number of PRBS errors since PRBS started running
 *
 * @param qlm    QLM to use
 * @param lane   Which lane
 * @param clear  Clear counter after return the current value
 *
 * @return Number of errors
 */
static uint64_t qlm_gserj_get_prbs_errors(int qlm, int lane, int clear)
{
    switch (qlm)
    {
        case GSERJ_MIN ... GSERJ_MAX: /* GSERJ */
            return _qlm_gserj_get_prbs_errors(qlm - GSERJ_MIN, lane, clear);
        default:
            return -1;
    }
}


/**
 * Display the current settings of a QLM lane
 *
 * @param qlm      QLM to display
 * @param qlm_lane Lane to use
 * @param show_tx  Display TX parameters
 * @param show_rx  Display RX parameters
 */
static void qlm_gserj_display_settings(int qlm, int qlm_lane, bool show_tx, bool show_rx, char *buf, int size)
{
    switch (qlm)
    {
        case GSERJ_MIN ... GSERJ_MAX: /* GSERJ */
            _qlm_gserj_display_settings(qlm - GSERJ_MIN, qlm_lane, show_tx, show_rx, buf, size);
            break;
    }
}

/**
 * Capture an eye diagram for the given QLM lane. The output data is written
 * to "eye".
 *
 * @param qlm       QLM to use
 * @param qlm_lane  Which lane
 * @param show_data Set to non-zero to have the eye capture dump the raw eye data as
 *                  it is captured
 * @param eye       Output eye data
 *
 * @return Zero on success, negative on failure
 */
static int qlm_gserj_eye_capture(int qlm, int lane, int show_data, qlm_eye_t *eye_data)
{
    switch (qlm)
    {
        case GSERJ_MIN ... GSERJ_MAX: /* GSERJ */
            return _qlm_gserj_eye_capture(qlm - GSERJ_MIN, lane, show_data, eye_data);
    }
    return -1;
}

/* Each chip has its own QLM operation table */
const qlm_ops_t qlm_ops_f95mm = {
	.type = QLM_GSERJ_TYPE,
	.qlm_enable_prbs = qlm_gserj_enable_prbs,
	.qlm_disable_prbs = qlm_gserj_disable_prbs,
	.qlm_get_prbs_errors = qlm_gserj_get_prbs_errors,
	.qlm_eye_capture = qlm_gserj_eye_capture,
	.qlm_display_settings = qlm_gserj_display_settings,
};
