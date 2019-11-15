/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

/* CGX Flash helpers for OcteonTX2 */

#include <arch.h>
#include <stdio.h>
#include <string.h>
#include <libfdt.h>
#include <debug.h>
#include <spi.h>
#include <drivers/delay_timer.h>
#include <platform_def.h>
#include <platform_dt.h>
#include <octeontx_common.h>
#include <plat_board_cfg.h>
#include <cgx.h>
#include <octeontx_utils.h>

/* define DEBUG_ATF_CGX_FLASH to enable debug logs */
#undef DEBUG_ATF_CGX_FLASH

#ifdef DEBUG_ATF_CGX_FLASH
#define debug_cgx_flash printf
#else
#define debug_cgx_flash(...) ((void) (0))
#endif

/* Fixed flash offset for storage of lmac params */
#define	SPI_NVDATA_OFFSET 0xFD0000

enum cmd_param {
	IGNORE = 0,
	LMAC_MODE,
	FEC,
	PHY_MOD,
};

extern uint32_t spi_mode;

static void cgx_fdt_get_spi_bus_cs(int *bus, int *cs)
{
	const void *fdt = fdt_ptr;
	const uint32_t *reg, *preg;
	int node, addr;

	*bus = -1;
	*cs = -1;
	if (fdt_check_header(fdt))
		return;
	node = fdt_node_offset_by_compatible(fdt, -1, "spi-flash");
	while (node > 0) {
		if (fdt_getprop(fdt, node, "u-boot,env", NULL)) {
			reg = fdt_getprop(fdt, node, "reg", NULL);
			if (reg)
				*cs = fdt32_to_cpu(*reg);
			preg = fdt_getprop(fdt, fdt_parent_offset(fdt, node),
					  "reg", NULL);
			if (preg) {
				addr = fdt32_to_cpu(*preg);
				/* SPI node will have PCI addr, so map it */
				if (addr == 0x3000)
					*bus = 0;
				if (addr == 0x3800)
					*bus = 1;
			}
			debug_cgx_flash("\n Env SPI [bus:cs] [%d:%d]\n",
					*bus, *cs);
			break;
		}
		node = fdt_node_offset_by_compatible(fdt, node, "spi-flash");
	}
}

static void cgx_fdt_get_persist_offset(int *offset)
{
	const void *fdt = fdt_ptr;
	const uint32_t *prop;
	int node;
	int poff = 0;

	if (fdt_check_header(fdt))
		return;
	node = fdt_node_offset_by_compatible(fdt, -1, "spi-flash");
	while (node > 0) {
		if (fdt_getprop(fdt, node, "u-boot,env", NULL)) {
			prop = fdt_getprop(fdt, node, "persist-offset", NULL);
			if (prop)
				poff = fdt32_to_cpu(*prop);
			/* Need offset above 8MB after firmware image
			 * and atleast 512 bytes below 256MB max flash size.
			 * Also should not be in two 64K sectors reserved for
			 * manufacturing data.
			 */
			if ((poff > 0x7FFFFF && poff <= 0xFDFF00) ||
			    (poff > 0xFFFFFF && poff <= 0xFFFFF00))
				*offset = poff;
			debug_cgx_flash("\n persist offset %x\n",
					*offset);
			break;
		}
		node = fdt_node_offset_by_compatible(fdt, node, "spi-flash");
	}
}

static int cgx_read_flash_lmac_params(uint8_t *buf, int buflen)
{
	int err = 0, spi_con, cs;
	int offset = SPI_NVDATA_OFFSET, mode = SPI_ADDRESSING_24BIT;

	cgx_fdt_get_spi_bus_cs(&spi_con, &cs);
	if (spi_con == -1 || cs == -1)
		return -1;
	err = spi_config(CONFIG_SPI_FREQUENCY, 0, 0, 0, spi_con, cs);
	if (err < 0) {
		debug_cgx_flash("Config flash failed for lmac params\n");
		return -1;
	}
	spi_mode |= SPI_FORCE_X1_READ;

	cgx_fdt_get_persist_offset(&offset);
	if (offset > 0xFFFFFF) {
		mode = SPI_ADDRESSING_32BIT;
		spi_mode |= SPI_FORCE_4B_OPCODE;
	}
	err = spi_nor_read(buf, buflen, offset, mode, spi_con, cs);
	if (err < 0)
		debug_cgx_flash("Read flash failed for lmac params\n");
	spi_mode &= ~(SPI_FORCE_X1_READ | SPI_FORCE_4B_OPCODE);
	return err;
}

int cgx_read_flash_fec(int cgx_id, int lmac_id, int *fec)
{
	cgx_lmac_flash_ctx_t *ptr;
	cgx_lmac_flash_ctx_t fctx[MAX_CGX * MAX_LMAC_PER_CGX];
	int ret;

	ret = cgx_read_flash_lmac_params((uint8_t *)fctx, sizeof(fctx));
	if (ret < 0)
		return -1;
	ptr = &fctx[cgx_id * MAX_LMAC_PER_CGX + lmac_id];

	if ((ptr->s.status != 0x2) || ptr->s.fec_invalid ||
	    ptr->s.cgx_id != cgx_id || ptr->s.lmac_id != lmac_id)
		return -1;
	if (!ptr->s.ignore)
		return -1;

	*fec = ptr->s.fec_type;
	return 0;
}

int cgx_read_flash_phy_mod(int cgx_id, int lmac_id, int *phy_mod)
{
	cgx_lmac_flash_ctx_t *ptr;
	cgx_lmac_flash_ctx_t fctx[MAX_CGX * MAX_LMAC_PER_CGX];
	int ret;

	ret = cgx_read_flash_lmac_params((uint8_t *)fctx, sizeof(fctx));
	if (ret < 0)
		return -1;
	ptr = &fctx[cgx_id * MAX_LMAC_PER_CGX + lmac_id];

	if ((ptr->s.status != 0x2) || ptr->s.mod_invalid ||
	    ptr->s.cgx_id != cgx_id || ptr->s.lmac_id != lmac_id)
		return -1;
	if (!ptr->s.ignore)
		return -1;
	*phy_mod = ptr->s.mod_type;
	return 0;
}

int cgx_read_flash_ignore(int cgx_id, int lmac_id, int *ignore)
{
	cgx_config_t *cgx;
	cgx_lmac_config_t *lmac;
	cgx_lmac_flash_ctx_t *ptr;
	cgx_lmac_flash_ctx_t fctx[MAX_CGX * MAX_LMAC_PER_CGX];
	int ret;

	cgx = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);
	lmac = &cgx->lmac_cfg[lmac_id];

	ret = cgx_read_flash_lmac_params((uint8_t *)fctx, sizeof(fctx));
	if (ret < 0)
		return -1;
	ptr = &fctx[cgx_id * MAX_LMAC_PER_CGX + lmac_id];

	if ((ptr->s.status != 0x2) || ptr->s.cgx_id != cgx_id ||
	    ptr->s.lmac_id != lmac_id || ptr->s.qlm_mode != lmac->mode_idx)
		return -1;
	*ignore = ptr->s.ignore ? 0 : 1;
	return 0;
}

int cgx_read_flash_mode_param(int cgx_id, int lmac_id, int *qlm_mode,
			      int *lmac_mode)
{
	cgx_lmac_flash_ctx_t *ptr;
	cgx_lmac_flash_ctx_t fctx[MAX_CGX * MAX_LMAC_PER_CGX];
	int ret;

	ret = cgx_read_flash_lmac_params((uint8_t *)fctx, sizeof(fctx));
	if (ret < 0)
		return -1;
	ptr = &fctx[cgx_id * MAX_LMAC_PER_CGX + lmac_id];

	if ((ptr->s.status != 0x2) || ptr->s.cgx_id != cgx_id ||
	    ptr->s.lmac_id != lmac_id) {
		debug_cgx_flash("%s: %d:%d invalid param\n", __func__, cgx_id, lmac_id);
		return -1;
	}

	*qlm_mode = ptr->s.qlm_mode;
	*lmac_mode = ptr->s.lmac_mode;
	return 0;
}

static int cgx_update_flash_lmac_params(int cgx_id, int lmac_id, int cmd,
					int arg)
{
	cgx_config_t *cgx;
	cgx_lmac_config_t *lmac;
	cgx_lmac_flash_ctx_t fctx[MAX_CGX * MAX_LMAC_PER_CGX];
	cgx_lmac_flash_ctx_t *ptr;
	int err, spi_con, cs;
	int offset = SPI_NVDATA_OFFSET, mode = SPI_ADDRESSING_24BIT;

	cgx = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);
	lmac = &cgx->lmac_cfg[lmac_id];
	cgx_fdt_get_spi_bus_cs(&spi_con, &cs);
	if (spi_con == -1 || cs == -1)
		return -1;

	err = cgx_read_flash_lmac_params((uint8_t *)fctx, sizeof(fctx));
	if (err < 0) {
		debug_cgx_flash("Read flash failed for lmac params\n");
		return -1;
	}
	ptr = &fctx[cgx_id * MAX_LMAC_PER_CGX + lmac_id];
	/* As flash erase sets all bits to 1, use 0x2 to mark
	 * param as valid, using 0 makes lmac mode read success for
	 * CGX0 LMAC0 if previous status of flash has all 0's. To avoid
	 * such corner cases, change name from invalid to status with
	 * size increase 1-bit to 2-bit and use only 0x2 as valid and
	 * others as invalid.
	 */
	ptr->s.status = 0x2;
	ptr->s.cgx_id = cgx_id;
	ptr->s.lmac_id = lmac_id;
	ptr->s.qlm_mode = lmac->mode_idx;
	/* Now that flash lmac mode is in use while booting whenever entry
	 * is made valid so store it along with qlm mode otherwise lmac init
	 * will fail on next reboot [reads 0xff for lmac mode]
	 * if only fec or phy_mod is changed.
	 */
	ptr->s.lmac_mode = lmac->mode;
	if (cmd == FEC) {
		ptr->s.fec_type = arg & 0x3;
		/* As flash erase sets all bits to 1, use 0 to mark
		 * param fec_invalid as valid.
		 */
		ptr->s.fec_invalid = 0;
	}
	if (cmd == PHY_MOD) {
		ptr->s.mod_type = arg & 0x1;
		/* As flash erase sets all bits to 1, use 0 to mark
		 * param mod_invalid as valid.
		 */
		ptr->s.mod_invalid = 0;
	}
	if (cmd == LMAC_MODE) {
		ptr->s.lmac_mode = arg & 0xff;
		if (lmac->phy_present && lmac->phy_config.init &&
		    lmac->phy_config.valid) {
			ptr->s.mod_invalid = 0;
			ptr->s.mod_type = lmac->phy_config.mod_type;
		}
		ptr->s.fec_invalid = 0;
		ptr->s.fec_type = lmac->fec;
	}
	if (cmd == IGNORE)
		ptr->s.ignore = (arg & 0x1) ? 0 : 1;

	debug_cgx_flash("%s flash status %d cgx%d lmac%d qmode %x mode %x\n",
			__func__, ptr->s.status, ptr->s.cgx_id,
			ptr->s.lmac_id, ptr->s.qlm_mode, ptr->s.lmac_mode);
	debug_cgx_flash("%s fec invalid %d type%x mod invalid%d type%x\n",
			__func__, ptr->s.fec_invalid, ptr->s.fec_type,
			ptr->s.mod_invalid, ptr->s.mod_type);
	cgx_fdt_get_persist_offset(&offset);
	if (offset > 0xFFFFFF) {
		mode = SPI_ADDRESSING_32BIT;
		spi_mode |= SPI_FORCE_4B_OPCODE;
	}
	err = spi_nor_erase(offset, mode, spi_con, cs);
	if (err < 0) {
		debug_cgx_flash("Erase flash failed for lmac params\n");
		return -1;
	}
	err = spi_nor_write((uint8_t *)fctx, sizeof(fctx), offset,
			    mode, spi_con, cs);
	if (err < 0) {
		debug_cgx_flash("Write flash failed for lmac params\n");
		return -1;
	}
	spi_mode &= ~SPI_FORCE_4B_OPCODE;
	return 0;
}

int cgx_update_flash_mode_param(int cgx_id, int lmac_id, int lmac_mode)
{
	return cgx_update_flash_lmac_params(cgx_id, lmac_id, LMAC_MODE,
					    lmac_mode);
}

int cgx_update_flash_ignore_param(int cgx_id, int lmac_id, int ignore)
{
	return cgx_update_flash_lmac_params(cgx_id, lmac_id, IGNORE, ignore);
}

int cgx_update_flash_fec_param(int cgx_id, int lmac_id, int fec)
{
	return cgx_update_flash_lmac_params(cgx_id, lmac_id, FEC, fec);
}

int cgx_update_flash_phy_mod_param(int cgx_id, int lmac_id, int phy_mod)
{
	return cgx_update_flash_lmac_params(cgx_id, lmac_id, PHY_MOD, phy_mod);
}

