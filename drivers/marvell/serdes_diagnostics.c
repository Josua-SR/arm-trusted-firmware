/*
 * Copyright (C) 2020 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <stdio.h>
#include <string.h>
#include <platform_def.h>
#include <plat_board_cfg.h>
#include <qlm/qlm.h>
#include <plat_scfg.h>
#include <gser_internal.h>
#include <platform_setup.h>

static int start_prbs(int cgx_id, int qlm, int mode, int *show_phy_host,
		int *show_phy_line, int qlm_lane)
{
	int gserx;
	cgx_config_t *cgx_cfg;

	cgx_cfg = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);

	if (mode == 0) {
		WARN("PRBS with pattern memory not implemented yet.\n");
		return -1;
	}

	gserx = plat_otx2_get_gserx(qlm, NULL);

	/* Start PRBS */
	cgx_cfg->qlm_ops->qlm_enable_prbs(gserx, mode, QLM_DIRECTION_TX, qlm_lane);

	/* BDK use here bdk_netphy_get_handle function */
	if (cgx_cfg->lmac_cfg[qlm_lane].phy_present) {
		if (*show_phy_host) {
			if (phy_enable_prbs(cgx_id, qlm_lane, 1, mode, 1))
				*show_phy_host = 0;
		}
		if (*show_phy_host) {
			if (phy_enable_prbs(cgx_id, qlm_lane, 0, mode, 1))
				*show_phy_line = 0;
		}
	}
	udelay(1000);  /* Let TX run for 1ms before starting RX */
	cgx_cfg->qlm_ops->qlm_enable_prbs(gserx, mode, QLM_DIRECTION_RX, qlm_lane);

	/* BDK use here bdk_netphy_get_handle function */
	if (cgx_cfg->lmac_cfg[qlm_lane].phy_present) {
		if (*show_phy_host) {
			if (phy_enable_prbs(cgx_id, qlm_lane, 1, mode, 1))
				*show_phy_host = 0;
		}
		if (*show_phy_line) {
			if (phy_enable_prbs(cgx_id, qlm_lane, 0, mode, 1))
				*show_phy_line = 0;
		}
	}

	return 0;
}

static int stop_prbs(int cgx_id, int qlm, int mode, int show_phy_host, int qlm_lane)
{
	int gserx;
	cgx_config_t *cgx_cfg;

	cgx_cfg = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);

	gserx = plat_otx2_get_gserx(qlm, NULL);

	/* Stop PRBS */
	cgx_cfg->qlm_ops->qlm_disable_prbs(gserx, qlm_lane);

	/* BDK use here bdk_netphy_get_handle function */
	if (cgx_cfg->lmac_cfg[qlm_lane].phy_present && show_phy_host) {
		phy_disable_prbs(cgx_id, qlm_lane, 1, mode);
		phy_disable_prbs(cgx_id, qlm_lane, 0, mode);
	}

	return 0;
}

static void clear_prbs_errors(int cgx_id, int qlm, int qlm_lane, int mode)
{
	int gserx;
	cgx_config_t *cgx_cfg;

	cgx_cfg = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);

	/* gserx index is the same for every lane */
	gserx = plat_otx2_get_gserx(qlm, NULL);

	cgx_cfg->qlm_ops->qlm_get_prbs_errors(gserx, qlm_lane, 1);

	if (cgx_cfg->lmac_cfg[qlm_lane].phy_present)
		phy_get_prbs_errors(cgx_id, qlm_lane, 1, 1, mode);
}

static int get_prbs_errors(int cgx_id, int qlm, int mode,
		int show_phy_host, int show_phy_line,
		cgx_prbs_errors_t *errors, int qlm_lane)
{
	int gserx;
	cgx_config_t *cgx_cfg;

	cgx_cfg = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);

	/* gserx index is the same for every lane */
	gserx = plat_otx2_get_gserx(qlm, NULL);

	errors[qlm_lane].err = cgx_cfg->qlm_ops->qlm_get_prbs_errors(gserx,
			qlm_lane, 0);

	/* BDK use here bdk_netphy_get_handle */
	if (!show_phy_host) {
		errors[qlm_lane].phy_host = -2;
	} else if (cgx_cfg->lmac_cfg[qlm_lane].phy_present) {
		errors[qlm_lane].phy_host = phy_get_prbs_errors(cgx_id,
						qlm_lane, 1, 0, mode);
	} else {
		errors[qlm_lane].phy_host = -1;
	}
	/* BDK use here bdk_netphy_get_handle */
	if (!show_phy_line) {
		errors[qlm_lane].phy_line = -2;
	} else if (cgx_cfg->lmac_cfg[qlm_lane].phy_present) {
		errors[qlm_lane].phy_line = phy_get_prbs_errors(cgx_id,
						qlm_lane, 0, 0, mode);
	} else {
		errors[qlm_lane].phy_line = -1;
	}

	return 0;
}

/* This structure is used to keep information about ongoing PRBS commands */
struct {
	int show_phy_host;
	int show_phy_line;
	int mode;
	int started;
} prbs_status[MAX_CGX];

/* Check if memory region for PRBS data is big enough */
CASSERT(sizeof(cgx_prbs_data) <= SERDES_PRBS_DATA_SIZE,
	assert_serdes_prbs_data_size_to_small);

int cgx_smc_do_prbs(int cmd, int qlm, int x3, int lane)
{
	int rc;
	int cgx_id;
	cgx_prbs_data *prbs_data;

	if (qlm >= MAX_QLM || qlm < 0) {
		WARN("%d not in range, available QLM0-%d\n", qlm, MAX_QLM - 1);
		return -1;
	}
	cgx_id = plat_get_cgx_idx(qlm);
	if (cgx_id == -1) {
		WARN("To QLM%d any CGX cannot by wired.\n", qlm);
		return -1;
	}

	switch (cmd) {
	case CGX_PRBS_START_CMD:
		if (prbs_status[cgx_id].started) {
			stop_prbs(cgx_id, qlm,
				prbs_status[cgx_id].mode,
				prbs_status[cgx_id].show_phy_host, lane);
			prbs_status[cgx_id].started = 0;
		}

		prbs_status[cgx_id].mode = x3;
		rc = start_prbs(cgx_id, qlm,
				prbs_status[cgx_id].mode,
				&prbs_status[cgx_id].show_phy_host,
				&prbs_status[cgx_id].show_phy_line, lane);
		if (rc)
			return -1;

		prbs_status[cgx_id].started = 1;
		break;

	case CGX_PRBS_STOP_CMD:
		if (prbs_status[cgx_id].started) {
			stop_prbs(cgx_id, qlm,
				prbs_status[cgx_id].mode,
				prbs_status[cgx_id].show_phy_host, lane);
			prbs_status[cgx_id].started = 0;
		}
		break;

	case CGX_PRBS_CLEAR_CMD:
		if (prbs_status[cgx_id].started) {
			clear_prbs_errors(cgx_id, qlm, lane,
				prbs_status[cgx_id].mode);
		}
		break;

	case CGX_PRBS_GET_DATA_CMD:
		if (!prbs_status[cgx_id].started) {
			WARN("PRBS for QLM%d is not started\n", qlm);
			return -1;
		}

		prbs_data = (cgx_prbs_data *)(SERDES_PRBS_DATA_BASE);

		get_prbs_errors(cgx_id, qlm,
				prbs_status[cgx_id].mode,
				prbs_status[cgx_id].show_phy_host,
				prbs_status[cgx_id].show_phy_line,
				prbs_data->errors, lane);

		if (x3) {
			printf("QLM%d.Lane%d: errors:", qlm, lane);
			if (prbs_data->errors[lane].err != -1)
				printf("%lld", prbs_data->errors[lane].err);
			else
				printf("No lock");

			if (prbs_data->errors[lane].phy_host != -2) {
				printf(", PHY Host errors: ");
				if (prbs_data->errors[lane].phy_host != -1)
					printf("%lld", prbs_data->errors[lane].phy_host);
				else
					printf("No lock");
			}

			if (prbs_data->errors[lane].phy_line != -2) {
				printf(", PHY Line errors: ");
			if (prbs_data->errors[lane].phy_line != -1)
				printf("%lld", prbs_data->errors[lane].phy_line);
			else
				printf("No lock");
			}
			printf("\n");
		}
		break;

	default:
		WARN("Unknown PRBS cmd (%d)\n", cmd);
		return -1;
	}

	return CGX_DISPLAY_OK;
}

/*
 * Square root by abacus algorithm, Martin Guy @ UKC, June 1985.
 * From a book on programming abaci by Mr C. Woo.
 */
static uint64_t isqrt(uint64_t num)
{
	uint64_t result = 0;
	/* The second-to-top bit is set: 1 << 62 for 64 bits */
	uint64_t bit = 1ull << 62;

	/* "bit" starts at the highest power of four <= the argument. */
	while (bit > num)
		bit >>= 2;

	while (bit != 0) {
		if (num >= result + bit) {
			num -= result + bit;
			result = (result >> 1) + bit;
		} else {
			result >>= 1;
		}
		bit >>= 2;
	}

	return result;
}

static uint64_t log10(uint64_t num)
{
	uint64_t result = 0;

	while (num > 10) {
		num /= 10;
		result++;
	}
	if (num >= 5)
		result++;

	return result;
}

/* This structure has over 32KiB and cannot be stored on stack */
static gser_qlm_eye_t eye;

int cgx_display_eye(int qlm, int qlm_lane, int show_data)
{
	int x, y, width, height, last_color, level, deltay, deltax, dy, dx;
	int dist, color, max_lane;
	int eye_area = 0;
	int eye_width = 0;
	int eye_height = 0;
	int gserx;
	int cgx_id;
	int ec;
	cgx_config_t *cgx_cfg;
	char color_str[] = "\33[40m"; /* Note: This is modified, not constant */

	if (qlm >= MAX_QLM || qlm < 0) {
		WARN("%d not in range, available QLM0-%d\n", qlm, MAX_QLM - 1);
		return -1;
	}

	max_lane = plat_octeontx_scfg->qlm_max_lane_num[qlm];
	if (qlm_lane >= max_lane || qlm_lane < 0) {
		WARN("%d not in range, available for QLM%d lanes are 0-%d\n",
				qlm_lane, qlm, max_lane - 1);
		return -1;
	}

	cgx_id = plat_get_cgx_idx(qlm);
	if (cgx_id == -1) {
		WARN("To QLM%d any CGX cannot by wired.\n", qlm);
		return -1;
	}
	cgx_cfg = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);

	gserx = cgx_cfg->lmac_cfg[qlm_lane].gserx;

	ec = cgx_cfg->qlm_ops->qlm_eye_capture(gserx, qlm_lane,
						show_data, &eye);
	if (ec)
		return ec;

	if (!show_data) {
		eye.type = cgx_cfg->qlm_ops->type;
		memcpy((void *)(SERDES_EYE_DATA_BASE), &eye,
				sizeof(gser_qlm_eye_t));

		return CGX_DISPLAY_OK;
	}

	if (cgx_cfg->qlm_ops->type == QLM_GSERN_TYPE) {
		uint64_t data;
		for (y = 0; y < eye.height; y++) {
			for (x = 0; x < eye.width; x++) {
				data = eye.data[y][x] + eye.data[y][x + 64];
				if (data > UINT32_MAX)
					data = UINT32_MAX;
				eye.data[y][x] = data;
			}
		}
	}

	/* Calculate the max eye width */
	for (y = 0; y < eye.height; y++) {
		width = 0;
		for (x = 0; x < eye.width; x++) {
			if (eye.data[y][x] == 0) {
				width++;
				eye_area++;
			}
		}
		if (width > eye_width)
			eye_width = width;
	}

	/* Calculate the max eye height */
	for (x = 0; x < eye.width; x++) {
		height = 0;
		for (y = 0; y < eye.height; y++) {
			if (eye.data[y][x] == 0) {
				height++;
				eye_area++;
			}
		}
		if (height > eye_height)
			eye_height = height;
	}

	printf("\nEye Diagram for QLM %d, Lane %d\n", qlm, qlm_lane);

	last_color = -1;
	for (y = 0; y < eye.height; y++) {
		for (x = 0; x < eye.width; x++) {
			level = log10(eye.data[y][x] + 1);
			if (level > 9)
				level = 9;
			#define DIFF(a, b) (a < b) ? b-a : a-b
			deltay = (y == (eye.height - 1)) ? -1 : 1;
			deltax = (x == (eye.width - 1)) ? -1 : 1;
			dy = DIFF(eye.data[y][x], eye.data[y + deltay][x]);
			dx = DIFF(eye.data[y][x], eye.data[y][x + deltax]);
			#undef DIFF
			dist = dx * dx + dy * dy;
			color = log10(isqrt(dist) + 1);
			if (color > 6)
				color = 6;
			if (level == 0)
				color = 0;
			if (color != last_color) {
				color_str[3] = '0' + color;
				printf("%s", color_str);
				last_color = color;
			}
			printf("%c", '0' + level);
		}
		printf("\33[0m\n");
		last_color = -1;
	}
	printf("\nEye Width %d, Height %d, Area %d\n",
		eye_width, eye_height, eye_area);

	return CGX_DISPLAY_OK;
}

int cgx_display_serdes_settings(int qlm, int qlm_lane, int show_data)
{
	int max_lane;
	int gserx;
	int cgx_id;
	cgx_config_t *cgx_cfg;

	if (qlm >= MAX_QLM || qlm < 0) {
		WARN("%s: %d not in range, available QLM0-%d\n", __func__, qlm,
				MAX_QLM - 1);
		return -1;
	}

	max_lane = plat_octeontx_scfg->qlm_max_lane_num[qlm];
	if (qlm_lane >= max_lane || qlm_lane < 0) {
		WARN("%s: %d not in range, available for QLM%d lanes are 0-%d\n",
				__func__, qlm_lane, qlm, max_lane - 1);
		return -1;
	}

	cgx_id = plat_get_cgx_idx(qlm);
	if (cgx_id == -1) {
		WARN("%s: QLM%d is not mapped to CGX.\n", __func__, qlm);
		return -1;
	}

	cgx_cfg = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);

	gserx = plat_otx2_get_gserx(qlm, NULL);

	if (show_data) {
		cgx_cfg->qlm_ops->qlm_display_settings(gserx, qlm_lane, 1, 1,
				NULL, 0);
	} else {
		cgx_cfg->qlm_ops->qlm_display_settings(gserx, qlm_lane, 1, 1,
				(char *)(SERDES_SETTINGS_DATA_BASE),
				SERDES_SETTINGS_DATA_SIZE);
	}

	return CGX_DISPLAY_OK;
}
