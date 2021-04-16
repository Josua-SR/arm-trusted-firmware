/*
 * Copyright (C) 2018 Marvell International Ltd.
 * This program is provided "as is" without any warranty of any kind,
 * and is distributed under the applicable Marvell proprietary limited use
 * license agreement.
 */
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <platform_def.h>
#include <platform_setup.h>
#include <debug.h>
#include <libfdt.h>
#include <stdlib.h>
#include <assert.h>
#include <plat_board_cfg.h>
#include <octeontx_common.h>
#include <octeontx_board_cfg_setup.h>
#include <plat_scfg.h>
#include <plat_otx2_configuration.h>
#include <octeontx_utils.h>
#include <qlm/qlm.h>
#include <rvu.h>
#include <strtol.h>
#include <plat_ghes.h>

#include "cavm-csrs-cgx.h"
#include "cavm-csrs-ecam.h"
#include "cavm-csrs-gpio.h"
#include "cavm-csrs-rst.h"

/* define DEBUG_ATF_DTS to enable debug logs */
#undef DEBUG_ATF_DTS

#ifdef DEBUG_ATF_DTS
#define debug_dts printf
#else
#define debug_dts(...) ((void) (0))
#endif

#define MHZ_TICKS_PER_SEC 1000000

/* List of GPIO types - used as expanders in case of SFP/QSFP/PHY */
static gpio_compat_t gpio_compat_list[] = {
	{ "cavium,thunder-8890-gpio", GPIO_PIN_DEFAULT, 64 },	/* 64 pins for T9x */
	{ "nxp,pca9505",	GPIO_PIN_PCA953X, 40 },
	{ "nxp,pca9698",	GPIO_PIN_PCA953X, 40 },
	{ "nxp,pca9534",	GPIO_PIN_PCA953X, 8 },
	{ "nxp,pca9535",	GPIO_PIN_PCA953X, 16 },
	{ "nxp,pca9536",	GPIO_PIN_PCA953X, 4 },
	{ "nxp,pca9537",	GPIO_PIN_PCA953X, 4 },
	{ "nxp,pca9538",	GPIO_PIN_PCA953X, 8 },
	{ "nxp,pca9539",	GPIO_PIN_PCA953X, 16 },
	{ "nxp,pca9554",	GPIO_PIN_PCA953X, 8 },
	{ "nxp,pca9554a",	GPIO_PIN_PCA953X, 8 },
	{ "nxp,pca9555",	GPIO_PIN_PCA953X, 16 },
	{ "nxp,pca9555a",	GPIO_PIN_PCA953X, 16 },
	{ "nxp,pca9556",	GPIO_PIN_PCA953X, 8 },
	{ "nxp,pca9557",	GPIO_PIN_PCA953X, 8 },
	{ "nxp,pca9574",	GPIO_PIN_PCA957X, 8 },
	{ "nxp,pca9575",	GPIO_PIN_PCA957X, 16 },
	{ "maxim,max7310",	GPIO_PIN_PCA953X, 8 },
	{ "maxim,max7312",	GPIO_PIN_PCA953X, 16 },
	{ "maxim,max7313",	GPIO_PIN_PCA953X, 16 },
	{ "maxim,max7315",	GPIO_PIN_PCA953X, 8 },
	{ "ti,pca6107",		GPIO_PIN_PCA953X, 8 },
	{ "ti,tca6408",		GPIO_PIN_PCA953X, 8 },
	{ "ti,tca6416",		GPIO_PIN_PCA953X, 16 },
	{ "ti,tca9554",		GPIO_PIN_PCA953X, 8 },
	{ "nxp,pcf8574",	GPIO_PIN_PCF857X, 8 },
	{ "nxp,pcf8574a",	GPIO_PIN_PCF857X, 8 },
	{ "nxp,pca8574",	GPIO_PIN_PCF857X, 8 },
	{ "nxp,pca9670",	GPIO_PIN_PCF857X, 8 },
	{ "nxp,pca9672",	GPIO_PIN_PCF857X, 8 },
	{ "nxp,pca9674",	GPIO_PIN_PCF857X, 8 },
	{ "nxp,pca8575",	GPIO_PIN_PCF857X, 16 },
	{ "nxp,pcf8575",	GPIO_PIN_PCF857X, 16 },
	{ "nxp,pca9671",	GPIO_PIN_PCF857X, 16 },
	{ "nxp,pca9673",	GPIO_PIN_PCF857X, 16 },
	{ "nxp,pca9675",	GPIO_PIN_PCF857X, 16 },
	{ "maxim,max7328",	GPIO_PIN_PCF857X, 8 },
	{ "maxim,max7329",	GPIO_PIN_PCF857X, 8 },
	{ "cavium,cpld96xx",	GPIO_PIN_CPLD,	8},
};

/* List of I2C Mux/Switch types */
static i2c_compat_t i2c_compat_list[] = {
	{ "cavium,thunder-8890-twsi", I2C_BUS_DEFAULT, I2C_OTHER,  0, 6},
	{ "cavium,thunderx-i2c", I2C_BUS_DEFAULT, I2C_OTHER,  0, 6},
	{ "nxp,pca9540", I2C_BUS_PCA9540, I2C_MUX,    4, 2 },
	{ "nxp,pca9542", I2C_BUS_PCA9542, I2C_MUX,    4, 2 },
	{ "nxp,pca9543", I2C_BUS_PCA9543, I2C_SWITCH, 0, 2 },
	{ "nxp,pca9544", I2C_BUS_PCA9544, I2C_MUX,    4, 4 },
	{ "nxp,pca9545", I2C_BUS_PCA9545, I2C_SWITCH, 0, 4 },
	{ "nxp,pca9546", I2C_BUS_PCA9546, I2C_SWITCH, 0, 4 },
	{ "nxp,pca9547", I2C_BUS_PCA9547, I2C_MUX,    8, 8 },
	{ "nxp,pca9548", I2C_BUS_PCA9548, I2C_SWITCH, 0, 8 },
};

/* List of PHY compatible strings/types */
static const phy_compatible_type_t phy_compat_list[] = {
	{ "marvell,88x5123", PHY_MARVELL_5123},
	{ "marvell,88x5113", PHY_MARVELL_5113},
#ifdef MARVELL_PHY_6141
	{ "marvell,88x6141", PHY_MARVELL_6141},
#endif /* MARVELL_PHY_6141 */
#ifdef MARVELL_PHY_1548
	{ "marvell,88e1548", PHY_MARVELL_88E1548},
#endif
	{ "marvell,88e1514", PHY_MARVELL_88E1514},
	{ "marvell,88e1512", PHY_MARVELL_88E1514},
	{ "vitesse,vsc8574", PHY_VITESSE_8574},
	{ "ethernet-phy-ieee802.3-c22", PHY_GENERIC_8023_C22},
	{ "ethernet-phy-ieee802.3-c45", PHY_GENERIC_8023_C45},
};

static int twsi_trim_list[TWSI_NUM];
static int mdio_trim_list[MDIO_NUM];

extern int cgx_read_flash_fec(int cgx_id, int lmac_id, int *fec);
extern int cgx_read_flash_phy_mod(int cgx_id, int lmac_id, int *phy_mod);


static int fdt_check_compatible_new_old_fmt(const void *fdt, int nodeoffset,
		char *compatible)
{
	int ret;
	char *p;

	if (!fdt_node_check_compatible(fdt, nodeoffset, compatible))
		return 0;

	/* try with the 'old' format ... */
	p = strchr(compatible, ',');
	if (!p)
		return 1;

	*p = '_';
	ret = fdt_node_check_compatible(fdt, nodeoffset, compatible);

	/* Reverting change in compatible string before return */
	*p = ',';

	return ret;
}

/* Output information specific for OCTEONTX2, for now only CGX. */
void plat_octeontx_print_board_variables(void)
{
	int i, j;
	cgx_config_t *cgx;
	cgx_lmac_config_t *lmac;
	phy_config_t *phy;

	for (i = 0; i < plat_octeontx_scfg->cgx_count; i++) {
		cgx = &(plat_octeontx_bcfg->cgx_cfg[i]);
		debug_dts("CGX%d: lmac_count = %d\n", i, cgx->lmac_count);
		for (j = 0; j < cgx->lmac_count; j++) {
			lmac = &cgx->lmac_cfg[j];
			debug_dts("CGX%d.LMAC%d: mode = %s:%d, qlm = %d, lane = %d, lane_to_sds=0x%x first_phy_lane %d\n",
					i,
					j,
					qlm_get_mode_strmap(lmac->mode_idx).bdk_str,
					lmac->mode,
					lmac->qlm,
					lmac->lane,
					lmac->lane_to_sds,
					lmac->first_phy_lane);
			debug_dts("\tnum_rvu_vfs=%d, num_msix_vec=%d, use_training=%d\n",
					lmac->num_rvu_vfs,
					lmac->num_msix_vec,
					lmac->use_training);
			debug_dts("\tMAC=%x:%x:%x:%x:%x:%x\n",
					lmac->local_mac_address[0],
					lmac->local_mac_address[1],
					lmac->local_mac_address[2],
					lmac->local_mac_address[3],
					lmac->local_mac_address[4],
					lmac->local_mac_address[5]);
			if (lmac->phy_present) {
				phy = &lmac->phy_config;
				if (phy->type != PHY_NONE) {
					debug_dts("\tPHY: mdio_bus=%d\t"
							"phy_addr=0x%x\t"
							"type=%d switch=%d\t"
							"port=%d\t"
							"host_order=0x%x\t"
							"line_order=0x%x\n",
							phy->mdio_bus,
							phy->addr,
							phy->type,
							phy->mux_switch,
							phy->port,
							phy->host_order,
							phy->line_order);
				}
			} else {
				debug_dts("\tPHY: NONE\n");
			}
			if (lmac->sfp_slot) {
				debug_dts("\tSFP: present\n");
				debug_dts("\tis_sfp=%d\n", lmac->sfp_info.is_sfp);
				debug_dts("\tis_qsfp=%d\n", lmac->sfp_info.is_qsfp);
				debug_dts("\tmax_power=%dmW\n", lmac->sfp_info.max_power);
			}
			if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) ||
				(lmac->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
				debug_dts("\tphy_mode=%d, sgmii_1000x_mode=%d, autoneg_dis=%d\n",
						lmac->phy_mode,
						lmac->sgmii_1000x_mode,
						lmac->autoneg_dis);
			}
			debug_dts("\tfec_type=%d line fec type=%d\n",
					lmac->fec, lmac->line_fec);
			debug_dts("\tLMAC enable=%d\n", lmac->lmac_enable);
		}
	}
}

static int octeontx2_fdt_get_int32(const void *fdt, const char *prop,
					int offset)
{
	const uint32_t *reg;
	int val = 0;

	reg = fdt_getprop(fdt, offset, prop, NULL);
	if (!reg) {
		debug_dts("%s: cannot find property %s\n",
				 __func__, prop);
		return -1;
	}
	val = fdt32_to_cpu(*reg);

	return val;
}

static uint64_t octeontx2_fdt_get_uint64(const void *fdt, const char *prop,
					int offset)
{
	const uint32_t *reg;
	uint64_t val = 0;

	reg = fdt_getprop(fdt, offset, prop, NULL);
	if (!reg) {
		WARN("%s: Cannot find property for prop %s\n",
				 __func__, prop);
		return -1;
	}
	/* To read the 64-bit property from DT, 8-byte aligned
	 * address is required as SCTLR_EL1/EL3(aa) - alignment
	 * check enable bit is set. Ex:MDIO address is 64-bit and
	 * fdt_getprop() might not return 8 byte aligned addr.
	 * to avoid alignment fault, the below code does 2 32-bit
	 * reads to obtain 64-bit addr.
	 */
	val = (uint64_t)fdt32_to_cpu(reg[0]) << 32;
	val |= fdt32_to_cpu(reg[1]);

	return val;
}

static int octeontx2_fdt_lookup_phandle(const void *fdt_addr, int offset,
		const char *prop_name)
{
	const uint32_t *phandle;

	phandle = fdt_getprop(fdt_addr, offset, prop_name, NULL);
	if (phandle)
		return fdt_node_offset_by_phandle(fdt_addr,
					fdt32_to_cpu(*phandle));
	else
		return -FDT_ERR_NOTFOUND;
}

/*
 * Finds first matching substring str2 in str1 and returns that offset @str1
 */
static const char *octeontx2_first_substring(const char *str1,
	 const char *str2)
{
	char temp_str[1024];

	if (!str1 || !str2)
		return NULL;

	while (*str1 != '\0') {
		if (*str1 == *str2) {
			strlcpy(temp_str, str1, sizeof(temp_str));
			temp_str[strlen(str2)] = '\0';
			if (strcmp(temp_str, str2) == 0)
				return str1;
		}
		str1++;
	}
	return NULL;
}

/*
 * Converts ascii string str to integer
 */
static int octeontx2_atoi(const char *str)
{
	int val = 0;
	int len = 0;
	int mul = 1;
	int index = 0;

	if (!str)
		return -1;

	len = strlen(str);
	while (len) {
		if (((str[index] < '0') || (str[index] > '9')))
			return -1;

		val = ((str[index] - '0') + (val * mul));
		mul = mul * 10;
		len--;
		index++;
	}
	return val;
}

/* Return string pointer of the BDK field required. Return NULL, if such
 * field isn't defined.
 */
static const char *octeontx2_fdtbdk_get_str(const void *fdt_addr,
	 const char *prop)
{
	int offset;
	const char *buf;
	int len;

	offset = fdt_path_offset(fdt_addr, "/cavium,bdk");
	buf = fdt_getprop(fdt_addr, offset, prop, &len);
	if (!buf) {
		debug_dts("No %s option is set in BDK.\n", prop);
		return NULL;
	}
	return buf;
}

/* Return numeric representation of the BDK field required. Return -1, if such
 * field isn't defined. Note that -1 can be value for the field.
 */
static long octeontx2_fdtbdk_get_num(const void *fdt_addr, const char *prop,
		int base)
{
	long ret;
	int offset;
	const char *buf;
	int len;

	offset = fdt_path_offset(fdt_addr, "/cavium,bdk");
	buf = fdt_getprop(fdt_addr, offset, prop, &len);
	if (!buf) {
		debug_dts("No %s option is set in BDK.\n", prop);
		return -1;
	}
	ret = strtol(buf, NULL, base);

	return ret;
}

/**
 * octeontx2_handle_num_rvu_vfs - handle errors and report user about
 * @req_vfs: requested (via FDT) number of VFs
 * @default_vfs: default number of VFs
 * @sum_vfs: already allocated HWVFs
 * @name: node name
 *
 * returns:
 * 	Valid number (req_vfs/default_vfs/0) of VFs that can be configured
 */
static int octeontx2_handle_num_rvu_vfs(int req_vfs, int default_vfs,
					int *sum_vfs, const char *name)
{
	int hwvfs_left, ret = 0;

	hwvfs_left = MAX_RVU_HWVFS - *sum_vfs;
	/* Check if requested num_rvu_vfs does not exceed
	 * maximum number of VFs per PF and number of left HWVFs */
	if ((req_vfs <= MAX_VFS_PER_PF) && (hwvfs_left >= req_vfs)) {
		/* Valid config */
		ret = req_vfs;
	} else {
		/* Print error, try fallback to default value */
		WARN("RVU: Request to allocate more VFs (%d) than allowed\n"
		     "              for node %s, trying fallback\n"
		     "              to default (%d)\n",
		     req_vfs, name, default_vfs);
		/* Repeat check for default value */
		if ((default_vfs <= MAX_VFS_PER_PF) && (hwvfs_left >= default_vfs)) {
			WARN("RVU: Successful fallback to default VFs (%d)\n"
			     "              for node %s\n",
			     default_vfs, name);
			ret = default_vfs;
		} else {
			/* Can't do more here, zeroing
			 * number of VFs for this PF */
			ERROR("RVU: Zeroing number of VFs for node %s\n", name);
			ret = 0;
		}
	}

	return ret;
}

/**
 * octeontx2_parse_rvu_admin - fill rvu_admin_pf_t structure of rvu_config
 * @fdt: pointer to the device tree blob
 * @parentoffset: offset to parent node (ecam2)
 * @node: node name
 * @sum_vfs: already allocated HWVFs
 *
 * returns:
 * 	0 on success, -1 otherwise
 */
static int octeontx2_parse_rvu_admin(const void *fdt, int parentoffset,
				    const char *name, int *sum_vfs)
{
	int offset, len, req_vfs;
	rvu_sw_rvu_pf_t *sw_pf;
	const int *val;

	/* Find offset of *name node */
	offset = fdt_subnode_offset(fdt, parentoffset, name);
	if (offset < 0) {
		WARN("RVU: No %s node in FDT\n", name);
		return -1;
	}

	sw_pf = &(plat_octeontx_bcfg->rvu_config.admin_pf);
	/* Get number of MSIX */
	val = fdt_getprop(fdt, offset, "num-msix-vec", &len);
	if (!val) {
		VERBOSE("RVU: No num-msix-vec, using %d number of MSIX\n"
		        "              for node %s\n", DEFAULT_MSIX_AF, name);
		sw_pf->num_msix_vec = DEFAULT_MSIX_AF;
	} else {
		sw_pf->num_msix_vec = fdt32_to_cpu(*val);
	}

	/* Get number of VFs from FDT */
	val = fdt_getprop(fdt, offset, "num-rvu-vfs", &len);
	if (!val) {
		/* If there's no such property in FDT
		 * try to assign default VFS */
		VERBOSE("RVU: No num-rvu-vfs property for node %s\n", name);
		sw_pf->num_rvu_vfs = octeontx2_handle_num_rvu_vfs(DEFAULT_AF_PF0_VFS,
					DEFAULT_AF_PF0_VFS, sum_vfs, name);
	} else {
		/* We've got that property, handle any errors with config */
		req_vfs = fdt32_to_cpu(*val);
		sw_pf->num_rvu_vfs = octeontx2_handle_num_rvu_vfs(req_vfs,
					DEFAULT_AF_PF0_VFS, sum_vfs, name);
	}

	/* Allocate additional VF for AF_PF0 to handle
	 * workaround for NIX-36681
	 */
	if ((IS_OCTEONTX_PASS(read_midr(), T96PARTNUM, 1, 0)) ||
		(IS_OCTEONTX_VAR(read_midr(), F95PARTNUM, 1)))
		sw_pf->num_rvu_vfs += 1;

	/* Increment number of allocated HWVFs */
	*sum_vfs += sw_pf->num_rvu_vfs;

	return 0;
}

/**
 * octeontx2_parse_sw_rvu - fill rvu_sw_pf_t structure of rvu_config
 * @fdt: pointer to the device tree blob
 * @parentoffset: offset to parent node (ecam2)
 * @node: node name
 * @sw_rvu_pf: index enumerated by sw_rvu_pfs
 * @sum_vfs: already allocated HWVFs
 *
 * returns:
 * 	0 on success, -1 otherwise
 */
static int octeontx2_parse_sw_rvu(const void *fdt, int parentoffset,
				 const char *name, int sw_rvu_pf, int *sum_vfs)
{
	int offset, len, req_vfs;
	const char *str;
	const int *val;
	rvu_sw_rvu_pf_t *sw_pf;

	assert(sw_rvu_pf >= 0 && sw_rvu_pf < SW_RVU_NUM_PF);

	sw_pf = &(plat_octeontx_bcfg->rvu_config.sw_pf[sw_rvu_pf]);

	/* Find offset of *name node */
	offset = fdt_subnode_offset(fdt, parentoffset, name);
	if (offset < 0) {
		WARN("RVU: No %s node in FDT\n", name);
		return -1;
	}

	/* Get number of MSIX */
	val = fdt_getprop(fdt, offset, "num-msix-vec", &len);
	if (!val) {
		VERBOSE("RVU: No num-msix-vec, using %d number of MSIX\n"
		        "              for node %s\n", DEFAULT_MSIX_SW, name);
		sw_pf->num_msix_vec = DEFAULT_MSIX_SW;
	} else {
		sw_pf->num_msix_vec = fdt32_to_cpu(*val);
	}

	if ((IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 1)) &&
		(sw_rvu_pf == SW_RVU_SSO_TIM_PF(0))) {
		if ((sw_pf->num_msix_vec != 128) ||
		     (sw_pf->num_msix_vec != DEFAULT_MSIX_SW)) {
			sw_pf->num_msix_vec = 128;
			VERBOSE("RVU: Defaulting MSI-X RVU PF count to 128 for Ax pass\n");
		}
	}

	/* Get number of VFs from FDT */
	val = fdt_getprop(fdt, offset, "num-rvu-vfs", &len);
	if (!val) {
		/* If there's no such property in FDT
		 * try to assign default VFS */
		VERBOSE("RVU: No num-rvu-vfs property for node %s\n", name);
		sw_pf->num_rvu_vfs = octeontx2_handle_num_rvu_vfs(DEFAULT_VFS,
					DEFAULT_VFS, sum_vfs, name);
	} else {
		/* We've got that property, handle any errors with config */
		req_vfs = fdt32_to_cpu(*val);
		sw_pf->num_rvu_vfs = octeontx2_handle_num_rvu_vfs(req_vfs,
					DEFAULT_VFS, sum_vfs, name);
	}

	/* Get [mandatory for SDP] provisioning mode from FDT */
	str = fdt_getprop(fdt, offset, "provision-mode", &len);
	if (!str) {
		/*
		 * The provision-mode property is required for SDP.
		 * If missing, issue [DEBUG] warning message, default to legacy.
		 * All others simply default to legacy (no [DEBUG] warning).
		 */
#if DEBUG
		if ((sw_rvu_pf >= SW_RVU_SDP_PF(0)) &&
		    (sw_rvu_pf - SW_RVU_SDP_PF(0) < SW_RVU_SDP_NUM_PF))
			WARN("RVU: node %s, no provision-mode, using LEGACY\n",
			     name);
#endif
		sw_pf->mapping = SW_RVU_MAP_LEGACY;
	} else if (!strncmp(str, "LEGACY", 6))
		sw_pf->mapping = SW_RVU_MAP_LEGACY;
	else if (!strncmp(str, "AVAILABLE", 9))
		sw_pf->mapping = SW_RVU_MAP_AVAILABLE;
	else if (!strncmp(str, "FORCE", 5))
		sw_pf->mapping = SW_RVU_MAP_FORCE;
	else if (!strncmp(str, "NONE", 4))
		sw_pf->mapping = SW_RVU_MAP_NONE;
	else {
		ERROR("RVU: node %s, invalid provision-mode %s, using NONE.\n",
		      name, str);
		sw_pf->mapping = SW_RVU_MAP_NONE;
	}

	/* Increment number of allocated HWVFs */
	*sum_vfs += sw_pf->num_rvu_vfs;

	return 0;
}

static void octeontx2_parse_rvu_config(const void *fdt, int *fdt_vfs)
{
	int offset, rc, soc_offset, cpt, i;
	char node_name[32];

	/* Initialize all SW_RVU_PF mappings to NONE */
	for (i = 0; i < SW_RVU_NUM_PF; i++)
		plat_octeontx_bcfg->rvu_config.sw_pf[i].mapping =
			SW_RVU_MAP_NONE;

	/* CGX configuration is already done on this step,
	 * perform initial setup for other RVU-related nodes */
	plat_octeontx_bcfg->rvu_config.valid = 0;
	soc_offset = offset = fdt_path_offset(fdt, "/soc@0");
	if (soc_offset < 0) {
		ERROR("RVU: Unable to find soc@0 node\n");
		return;
	}

	/* Parse all subnodes of ECAM0, Domain2 */
	snprintf(node_name, sizeof(node_name), "pci@%llx",
		(ECAM_PF_BAR2(0) | (2 << 28)));
	offset = fdt_subnode_offset(fdt, soc_offset, node_name);
	if (offset < 0) {
		ERROR("RVU: Unable to find ecam2 node: %s\n", node_name);
		return;
	}

	/* Fill rvu_admin_pf_t structure */
	rc = octeontx2_parse_rvu_admin(fdt, offset, RVU_ADMIN_FDT_NODE, fdt_vfs);
	if (rc < 0) {
		WARN("RVU: Unable to fill PF%d-ADMIN structure\n", RVU_AF);
		return;
	}

	/* Fill rvu_sw_rvu_pf_t structure, start with SSO_TIM (PF13) */
	rc = octeontx2_parse_sw_rvu(fdt, offset, RVU_SSO_TIM_FDT_NODE,
				    SW_RVU_SSO_TIM_PF(0), fdt_vfs);
	if (rc < 0) {
		WARN("RVU: Unable to fill PF%d-SSO_TIM structure\n",
			FIXED_RVU_SSO_TIM);
		return;
	} else if (!SW_RVU_SSO_TIM_NUM_PF) {
		/* Platform DTS should not contain entry for non-existent dev */
		ERROR("RVU: SSO FDT entry found but SW_RVU_SSO_TIM_NUM_PF=0\n");
	}

	/* Now parse NPA (PF14) */
	rc = octeontx2_parse_sw_rvu(fdt, offset, RVU_NPA_FDT_NODE,
				    SW_RVU_NPA_PF(0), fdt_vfs);
	if (rc < 0) {
		WARN("RVU: Unable to fill PF%d-NPA structure\n", FIXED_RVU_NPA);
		return;
	} else if (!SW_RVU_NPA_NUM_PF) {
		/* Platform DTS should not contain entry for non-existent dev */
		ERROR("RVU: NPA FDT entry found but SW_RVU_NPA_NUM_PF=0\n");
	}

#ifdef RVU_SDP_FDT_NODE
	/*
	 * Implementation note: we parse all the SDP devices using the same
	 * node name as they are all identical.  If they need to be different,
	 * this loop needs to change to specify the instance-specific node name.
	 */
	for (i = 0; i < SW_RVU_SDP_NUM_PF; i++) {
		rc = octeontx2_parse_sw_rvu(fdt, offset, RVU_SDP_FDT_NODE,
					    SW_RVU_SDP_PF(i), fdt_vfs);
		/* Not an error if SDP is absent from FDT. */
		(void)rc;
	}
#endif /* RVU_SDP_FDT_NODE */

#ifdef RVU_REE_FDT_NODE
	/*
	 * Implementation note: we parse all the REE devices using the same
	 * node name as they are all identical.  If they need to be different,
	 * this loop needs to change to specify the instance-specific node name.
	 */
	for (i = 0; i < SW_RVU_REE_NUM_PF; i++) {
		rc = octeontx2_parse_sw_rvu(fdt, offset, RVU_REE_FDT_NODE,
					    SW_RVU_REE_PF(i), fdt_vfs);
		/* Not an error if REE is absent from FDT. */
		(void)rc;
	}
#endif /* RVU_REE_FDT_NODE */

	/* Find if CPT node is available */
	if (plat_octeontx_get_cpt_count()) {
		/* if CPT block is available, check if node is
		 * present before configuring RVU for CPT
		 */
		cpt = fdt_subnode_offset(fdt, offset, RVU_CPT_FDT_NODE);
		if (cpt < 0) {
			/* If node is not present, return error */
			WARN("RVU: CPT node is not available\n");
			return;
		}
		rc = octeontx2_parse_sw_rvu(fdt, offset, RVU_CPT_FDT_NODE,
					    SW_RVU_CPT_PF(0), fdt_vfs);
		if (rc < 0) {
			WARN("RVU: Unable to fill PF%d structure\n", RVU_LAST);
			return;
		}
	} else { /* CPT not available */
		debug_dts("RVU: CPT is disabled\n");
		plat_octeontx_bcfg->rvu_config.cpt_dis = 1;
	}

	/* Here we can mark FDT RVU config as valid */
	plat_octeontx_bcfg->rvu_config.valid = 1;
}


static void octeontx2_boot_device_from_strapx()
{
	cavm_gpio_strap_t gpio_strap;
	int boot_medium;

	gpio_strap.u = CSR_READ(CAVM_GPIO_STRAP);
	boot_medium = (gpio_strap.u) & 0x7;

	switch (boot_medium) {
		case CAVM_RST_BOOT_METHOD_E_REMOTE_CN9:
			plat_octeontx_bcfg->bcfg.boot_dev.boot_type = OCTEONTX_BOOT_REMOTE;
			break;
		case CAVM_RST_BOOT_METHOD_E_SPI0_CS0:
			plat_octeontx_bcfg->bcfg.boot_dev.boot_type = OCTEONTX_BOOT_SPI;
			plat_octeontx_bcfg->bcfg.boot_dev.controller = 0;
			plat_octeontx_bcfg->bcfg.boot_dev.cs = 0;
			break;
		case CAVM_RST_BOOT_METHOD_E_SPI0_CS1:
			plat_octeontx_bcfg->bcfg.boot_dev.boot_type = OCTEONTX_BOOT_SPI;
			plat_octeontx_bcfg->bcfg.boot_dev.controller = 0;
			plat_octeontx_bcfg->bcfg.boot_dev.cs = 1;
			break;
		case CAVM_RST_BOOT_METHOD_E_SPI1_CS0:
			plat_octeontx_bcfg->bcfg.boot_dev.boot_type = OCTEONTX_BOOT_SPI;
			plat_octeontx_bcfg->bcfg.boot_dev.controller = 1;
			plat_octeontx_bcfg->bcfg.boot_dev.cs = 0;
			break;
		case CAVM_RST_BOOT_METHOD_E_SPI1_CS1:
			plat_octeontx_bcfg->bcfg.boot_dev.boot_type = OCTEONTX_BOOT_SPI;
			plat_octeontx_bcfg->bcfg.boot_dev.controller = 1;
			plat_octeontx_bcfg->bcfg.boot_dev.cs = 1;
			break;
		case CAVM_RST_BOOT_METHOD_E_EMMC_CS0:
			plat_octeontx_bcfg->bcfg.boot_dev.boot_type = OCTEONTX_BOOT_EMMC;
			plat_octeontx_bcfg->bcfg.boot_dev.controller = 0;
			plat_octeontx_bcfg->bcfg.boot_dev.cs = 0;
			break;
		case CAVM_RST_BOOT_METHOD_E_EMMC_CS1:
			plat_octeontx_bcfg->bcfg.boot_dev.boot_type = OCTEONTX_BOOT_EMMC;
			plat_octeontx_bcfg->bcfg.boot_dev.controller = 0;
			plat_octeontx_bcfg->bcfg.boot_dev.cs = 1;
			break;
		default:
			plat_octeontx_bcfg->bcfg.boot_dev.boot_type = -OCTEONTX_BOOT_UNSUPPORTED;
			break;
	}
}

static int octeontx2_parse_boot_device(const void *fdt, const int offset)
{
	char boot_device[16], *cs;
	const char *name;
	int len, val;

	snprintf(boot_device, sizeof(boot_device), "BOOT-DEVICE.N0");
	name = fdt_getprop(fdt, offset, boot_device, &len);
	if (!name) {
		WARN("No %s is found\n", boot_device);
		return -1;
	}

	strlcpy(boot_device, name, sizeof(boot_device));
	boot_device[sizeof(boot_device) - 1] = '\0';

	debug_dts("BOOT-DEVICE.N0: %s\n", boot_device);
	/* Get boot type */
	if (!strncmp("SPI", boot_device, 3))
		val = OCTEONTX_BOOT_SPI;
	else if (!strncmp("EMMC", boot_device, 4))
		val = OCTEONTX_BOOT_EMMC;
	else if (!strncmp("REMOTE", boot_device, 6))
		val = OCTEONTX_BOOT_REMOTE;
	else
		val = -OCTEONTX_BOOT_UNSUPPORTED;

	plat_octeontx_bcfg->bcfg.boot_dev.boot_type = val;

	/* Get boot controller (only for SPI) */
	if (plat_octeontx_bcfg->bcfg.boot_dev.boot_type == OCTEONTX_BOOT_SPI) {
		if (!strncmp("SPI0", boot_device, 4))
			val = 0;
		else if (!strncmp("SPI1", boot_device, 4))
			val = 1;
		else
			val = -1;
	} else {
		val = -1;
	}

	plat_octeontx_bcfg->bcfg.boot_dev.controller = val;

	/* Get chip select used to boot (EMMC and SPI) */
	if (plat_octeontx_bcfg->bcfg.boot_dev.boot_type == OCTEONTX_BOOT_SPI ||
	    plat_octeontx_bcfg->bcfg.boot_dev.boot_type == OCTEONTX_BOOT_EMMC) {
		cs = strchr(boot_device, '_');
		if (!cs) {
			val = -1;
		} else {
			cs += 3; //Move pointer over "CS" string to point to boot CS
			val = (*cs - '0');
			if ((val != 0) &&
			    (val != 1)) {
				val = -1;
			}
		}
	} else {
		val = -1;
	}

	plat_octeontx_bcfg->bcfg.boot_dev.cs = val;

	return 0;
}

static int octeontx2_fdt_get_bus(const void *fdt, int offset,
		int cgx_idx, int lmac_idx)
{
	int node, bus = -1;
	uint64_t mdio;
	const char *nodename;
	uint32_t i2c;

	if (offset < 0)
		return -1;

	/* obtain parent node and get the name */
	node = fdt_parent_offset(fdt, offset);
	if (node < 0)
		return -1;

	nodename = fdt_get_name(fdt, node, NULL);

	if (!strncmp(nodename, "mdio", 4)) {
		debug_dts("CGX%d.LMAC%d: MDIO node\n", cgx_idx, lmac_idx);
		mdio = octeontx2_fdt_get_uint64(fdt, "reg", node);
		if (mdio != -1)
			bus = (mdio & (1 << 7)) ? 1 : 0;
		debug_dts("CGX%d.LMAC%d: mdio 0x%llx bus %d\n",
				cgx_idx, lmac_idx, mdio, bus);
	} else if (!strncmp(nodename, "i2c", 3)) {
		debug_dts("CGX%d.LMAC%d: I2C node\n", cgx_idx, lmac_idx);
		i2c = octeontx2_fdt_get_int32(fdt, "reg", node);
		/* based on DEVFN, obtain TWSI bus */
		bus = ((i2c >> 8) & 0x7);
		debug_dts("CGX%d.LMAC%d: bus %d\n",
				cgx_idx, lmac_idx, bus);
	} else
		WARN("CGX%d.LMAC%d: no compatible bus type for PHY/SFP\n",
				cgx_idx, lmac_idx);

	return bus;
}

static int octeontx2_fdt_get_i2c_bus_info(const void *fdt, int offset,
		i2c_info_t *i2c_info, int cgx_idx, int lmac_idx)
{
	int parent, ret;
	int bus_offset = -1;

	parent = fdt_parent_offset(fdt, offset);
	if (parent < 0) {
		ERROR("CGX%d.LMAC%d: couldn't find i2c type\n",
				cgx_idx, lmac_idx);
		return -1;
	}

	for (int i = 0; i < ARRAY_SIZE(i2c_compat_list); i++) {
		if (!fdt_check_compatible_new_old_fmt(fdt, parent,
					       i2c_compat_list[i].compatible)) {
			debug_dts("CGX%d.LMAC%d: I2C type %d\n", cgx_idx,
					lmac_idx, i2c_compat_list[i].type);
			i2c_info->type = i2c_compat_list[i].type;
			if (i2c_info->type == I2C_BUS_DEFAULT) {
				/* TWSI bus */
				ret = octeontx2_fdt_get_bus(fdt,
					offset, cgx_idx, lmac_idx);
				if (ret != -1)
					i2c_info->bus = ret;
				else
					return ret;

				bus_offset = fdt_parent_offset(fdt, offset);

			} else { /* all other MUX/SWITCH cases */
				i2c_info->is_mux = i2c_compat_list[i].mux_type;
				i2c_info->enable_bit =
					i2c_compat_list[i].enable;
				i2c_info->channel = octeontx2_fdt_get_int32(fdt,
							"reg", offset);
				i2c_info->addr = octeontx2_fdt_get_int32(fdt,
							"reg", parent);
				/* TWSI bus */
				ret = octeontx2_fdt_get_bus(fdt,
					parent, cgx_idx, lmac_idx);
				if (ret != -1)
					i2c_info->bus = ret;
				else
					return ret;
				debug_dts(
					"CGX%d.LMAC%d: I2C SWITCH %d: channel %d addr 0x%x bus %d\n",
					cgx_idx, lmac_idx, !i2c_info->is_mux,
					i2c_info->channel,
					i2c_info->addr, i2c_info->bus);

				bus_offset = fdt_parent_offset(fdt, parent);
			}
			break;
		}
	}
	if (i2c_info->type == I2C_BUS_NONE) {
		debug_dts("CGX%d.LMAC%d: couldn't find valid I2C BUS type\n",
				cgx_idx, lmac_idx);
		return -1;
	}
	return bus_offset;
}

static int octeontx2_fdt_gpio_get_info_by_phandle(const void *fdt, int offset,
		const char *propname, gpio_info_t *gpio_info,
		int cgx_idx, int lmac_idx)
{
	int len;
	const struct fdt_property *prop;
	const uint32_t *data;
	int phandle, parent;

	prop = fdt_get_property(fdt, offset, propname, &len);
	if (!prop) {
		WARN("CGX%d.LMAC%d: couldn't find %s property\n",
				cgx_idx, lmac_idx, propname);
		return -1;
	}

	if (len != 3 * sizeof(unsigned int)) {
		ERROR("CGX%d.LMAC%d: %s property is of wrong format : must contain phandle, pin & flags\n",
				cgx_idx, lmac_idx, propname);
		return -1;
	}

	data = (const uint32_t *)prop->data;
	phandle = fdt32_to_cpu(data[0]);
	gpio_info->pin = fdt32_to_cpu(data[1]);
	gpio_info->flags = fdt32_to_cpu(data[2]);

	int node = fdt_node_offset_by_phandle(fdt, phandle);

	debug_dts("CGX%d.LMAC%d: GPIO name %s pin %d flags %d\n",
			cgx_idx, lmac_idx, propname, gpio_info->pin, gpio_info->flags);
	for (int i = 0; i < ARRAY_SIZE(gpio_compat_list); i++) {
		if (!fdt_check_compatible_new_old_fmt(fdt, node,
				gpio_compat_list[i].compatible)) {
			debug_dts("CGX%d.LMAC%d: gpio type %d\n", cgx_idx,
					lmac_idx, gpio_compat_list[i].type);

			/* If the gpio is connected directly, just update
			 * the type and return
			 */
			if (gpio_compat_list[i].type == GPIO_PIN_DEFAULT) {
				gpio_info->type = gpio_compat_list[i].type;
				break;
			}

			/* For all other GPIO pins that are connected
			 * through expanders
			 */
			gpio_info->num_pins = octeontx2_fdt_get_int32(fdt,
				"ngpios", node);

			/* If max number of GPIOs are not available from
			 * DT, get it from the static table
			 */
			if (!gpio_info->num_pins)
				gpio_info->num_pins = gpio_compat_list[i].ngpios;
			gpio_info->type = gpio_compat_list[i].type;
			gpio_info->i2c_addr = octeontx2_fdt_get_int32(fdt, "reg",
				node);
			octeontx2_fdt_get_i2c_bus_info(fdt, node,
					&gpio_info->i2c_info,
					cgx_idx, lmac_idx);
			if (gpio_info->i2c_info.type == I2C_BUS_NONE) {
				/* There might be the case of where the GPIO
				 * expander is behind the I2C switch. Hence
				 * pass the parent node to obtain the
				 * i2c info again
				 */
				parent = fdt_parent_offset(fdt, node);
				octeontx2_fdt_get_i2c_bus_info(fdt, parent,
					&gpio_info->i2c_info,
					cgx_idx, lmac_idx);
				if (gpio_info->i2c_info.type == I2C_BUS_NONE)
					return -1;
			}
			gpio_info->i2c_bus = gpio_info->i2c_info.bus;
			debug_dts("CGX%d.LMAC%d: GPIO controller : addr 0x%x bus %d num pins %d\n",
				cgx_idx, lmac_idx,
				gpio_info->i2c_addr, gpio_info->i2c_bus,
				gpio_info->num_pins);
			break;
		}
	}
	if (gpio_info->type == GPIO_PIN_NONE) {
		WARN("CGX%d.LMAC%d: couldn't find any valid GPIO type\n",
				cgx_idx, lmac_idx);
		return -1;
	}
	return 0;
}

/*
 * Function parsing QSFP info
 * Returns:
 *   0: if QSFP slot was parsed & is going to be managed in ATF
 *   1: parsing was skipped as QSFP slot is going to be managed in kernel
 *  -1: on parsing error
 *
 */
static int octeontx2_fdt_parse_qsfp_info(const void *fdt, int offset,
		int cgx_idx, int lmac_idx)
{
	const char *name;
	i2c_info_t i2c_info;
	sfp_slot_info_t *qsfp_info;
	cgx_lmac_config_t *lmac;
	int eeprom, parent, ret;
	int i2c_bus_offset;

	lmac = &(plat_octeontx_bcfg->cgx_cfg[cgx_idx].lmac_cfg[lmac_idx]);
	qsfp_info = &lmac->sfp_info;

	if (fdt_node_check_compatible(fdt, offset, "qsfp-slot"))
		return -1;

	/* Parse EEPROM related I2C info */
	eeprom = octeontx2_fdt_lookup_phandle(fdt, offset, "eeprom");
	if (eeprom < 0) {
		ERROR("CGX%d.LMAC%d: Couldn't find EEPROM info for SFP\n",
				cgx_idx, lmac_idx);

		ret = -1;
		goto qsfp_update;
	}

	parent = fdt_parent_offset(fdt, eeprom);
	i2c_bus_offset = octeontx2_fdt_get_i2c_bus_info(fdt, parent, &i2c_info,
					cgx_idx, lmac_idx);
	if (i2c_bus_offset < 0) {
		ret = -1;
		goto qsfp_update;
	}

	name = fdt_get_name(fdt, offset, NULL);
	if (fdt_get_property(fdt, i2c_bus_offset, "twsi-in-kernel", NULL)) {
		debug_dts("CGX%d.LMAC%d: skipped parsing %s, "
			"i2c bus %d is managed in kernel\n",
				cgx_idx, lmac_idx, name, i2c_info.bus);

		return 1;
	}

	/* Update the list of twsi nodes to be trimmed */
	if (!twsi_trim_list[i2c_info.bus]) {
		twsi_trim_list[i2c_info.bus] = i2c_bus_offset;
	}

	qsfp_info->is_sfp = 0;
	strlcpy(qsfp_info->name, name, sizeof(qsfp_info->name));
	debug_dts("CGX%d.LMAC%d: qsfp_info->name %s\n",
			cgx_idx, lmac_idx, qsfp_info->name);

	memcpy(&qsfp_info->i2c_eeprom_info, &i2c_info, sizeof(i2c_info_t));

	qsfp_info->eeprom_addr = octeontx2_fdt_get_int32(fdt, "reg", eeprom);

	debug_dts("CGX%d.LMAC%d: EEPROM addr 0x%x\n", cgx_idx, lmac_idx,
					qsfp_info->eeprom_addr);

	/* obtain MAX power for the slot as per the board design */
	qsfp_info->max_power = octeontx2_fdt_get_int32(fdt, "max_power",
			offset);


	/* Parse GPIO info for QSFP interface */
	ret = octeontx2_fdt_gpio_get_info_by_phandle(fdt, offset, "mod_sel",
			&qsfp_info->select, cgx_idx, lmac_idx);
	if (ret == -1)
		goto qsfp_update;

	ret = octeontx2_fdt_gpio_get_info_by_phandle(fdt, offset, "reset",
			&qsfp_info->reset, cgx_idx, lmac_idx);
	if (ret == -1)
		goto qsfp_update;

	ret = octeontx2_fdt_gpio_get_info_by_phandle(fdt, offset, "lowpow_mode",
			&qsfp_info->lp_mode, cgx_idx, lmac_idx);
	if (ret == -1)
		goto qsfp_update;

	ret = octeontx2_fdt_gpio_get_info_by_phandle(fdt, offset, "mod_present",
			&qsfp_info->mod_prs, cgx_idx, lmac_idx);
	if (ret == -1)
		goto qsfp_update;

	ret = octeontx2_fdt_gpio_get_info_by_phandle(fdt, offset, "int",
			&qsfp_info->interrupt, cgx_idx, lmac_idx);
	if (ret == -1)
		goto qsfp_update;

	/* Set is_qsfp only when all the required info
	 * are parsed from DTS related to QSFP slot
	 */
	lmac->sfp_slot = 1;	/* SFP slot is present */
	qsfp_info->is_qsfp = 1;	/* To indicate slot is QSFP */

	return 0;

qsfp_update:
	ERROR("%s: %d:%d QSFP slot info not parsed fully\n",
			__func__, cgx_idx, lmac_idx);
	return ret;
}

/*
 * Function parsing SFP info
 * Returns:
 *   0: if SFP slot was parsed & is going to be managed in ATF
 *   1: parsing was skipped as SFP slot is going to be managed in kernel
 *  -1: on parsing error
 *
 */
static int octeontx2_fdt_parse_sfp_info(const void *fdt, int offset,
		int cgx_idx, int lmac_idx)
{
	const char *name;
	i2c_info_t i2c_info;
	sfp_slot_info_t *sfp_info;
	cgx_lmac_config_t *lmac;
	int eeprom, parent, ret;
	int i2c_bus_offset;

	lmac = &(plat_octeontx_bcfg->cgx_cfg[cgx_idx].lmac_cfg[lmac_idx]);
	sfp_info = &lmac->sfp_info;

	if (fdt_node_check_compatible(fdt, offset, "sfp-slot"))
		return -1;

	/* Parse EEPROM related I2C info */
	eeprom = octeontx2_fdt_lookup_phandle(fdt, offset, "eeprom");
	if (eeprom < 0) {
		ERROR("CGX%d.LMAC%d: Couldn't find EEPROM info for SFP\n",
				cgx_idx, lmac_idx);

		ret = -1;
		goto sfp_update;
	}

	parent = fdt_parent_offset(fdt, eeprom);
	i2c_bus_offset = octeontx2_fdt_get_i2c_bus_info(fdt, parent, &i2c_info,
					cgx_idx, lmac_idx);
	if (i2c_bus_offset < 0) {
		ret = -1;
		goto sfp_update;
	}

	name = fdt_get_name(fdt, offset, NULL);
	if (fdt_get_property(fdt, i2c_bus_offset, "twsi-in-kernel", NULL)) {
		debug_dts("CGX%d.LMAC%d: skipped parsing %s, "
			"i2c bus %d is managed in kernel\n",
				cgx_idx, lmac_idx, name, i2c_info.bus);

		return 1;
	}

	/* Update the list of twsi nodes to be trimmed */
	if (!twsi_trim_list[i2c_info.bus]) {
		twsi_trim_list[i2c_info.bus] = i2c_bus_offset;
	}

	sfp_info->is_qsfp = 0;
	strlcpy(sfp_info->name, name, sizeof(sfp_info->name));
	debug_dts("CGX%d.LMAC%d: sfp_info->name %s\n",
			cgx_idx, lmac_idx, sfp_info->name);

	memcpy(&sfp_info->i2c_eeprom_info, &i2c_info, sizeof(i2c_info_t));

	sfp_info->eeprom_addr = octeontx2_fdt_get_int32(fdt, "reg", eeprom);

	debug_dts("CGX%d.LMAC%d: EEPROM addr 0x%x\n", cgx_idx, lmac_idx,
					sfp_info->eeprom_addr);

	/* obtain MAX power for the slot as per the board design */
	sfp_info->max_power = octeontx2_fdt_get_int32(fdt, "max_power", offset);


	/* Parse GPIO info for SFP interface */
	ret = octeontx2_fdt_gpio_get_info_by_phandle(fdt, offset, "detect",
			&sfp_info->mod_abs, cgx_idx, lmac_idx);
	if (ret == -1)
		goto sfp_update;

	ret = octeontx2_fdt_gpio_get_info_by_phandle(fdt, offset, "tx_disable",
			&sfp_info->tx_disable, cgx_idx, lmac_idx);
	if (ret == -1)
		goto sfp_update;
	ret = octeontx2_fdt_gpio_get_info_by_phandle(fdt, offset, "tx_fault",
			&sfp_info->tx_fault, cgx_idx, lmac_idx);
	if (ret == -1)
		goto sfp_update;

	ret = octeontx2_fdt_gpio_get_info_by_phandle(fdt, offset, "rx_los",
			&sfp_info->rx_los, cgx_idx, lmac_idx);
	if (ret == -1)
		goto sfp_update;
	lmac->sfp_slot = 1;	/* SFP slot is present */
	sfp_info->is_sfp = 1;	/* To indicate slot is SFP */

	return 0;

sfp_update:
	ERROR("%s: %d:%d SFP slot info not parsed fully\n",
			__func__, cgx_idx, lmac_idx);
	return ret;
}

/* This routine sets a number of LMACs to initialize and the size to use.
 * For instance:
 *  - SGMII_2X1: will initialize 2 LMACs and each LMAC will take only one
 *  lane
 *  - XAUI_1X4: will initialize 1 LMAC and it will take all 4 lanes
 */
static void octeontx2_lmac_num_touse(int mode_idx, int *cnt, int *touse)
{
	*cnt = 0;
	*touse = 0;
	switch (mode_idx) {
	case QLM_MODE_1G_X:
	case QLM_MODE_SGMII:
	case QLM_MODE_XFI:
	case QLM_MODE_SFI:
	case QLM_MODE_10G_KR:
	case QLM_MODE_20GAUI_C2C:
	case QLM_MODE_25GAUI_C2C:
	case QLM_MODE_25GAUI_C2M:
	case QLM_MODE_25G_CR:
	case QLM_MODE_25G_KR:
		*cnt = 1;
		*touse = 1;
		break;
	case QLM_MODE_XAUI:
	case QLM_MODE_XLAUI:
	case QLM_MODE_XLAUI_C2M:
	case QLM_MODE_40G_CR4:
	case QLM_MODE_40G_KR4:
	case QLM_MODE_50GAUI_4_C2C:
	case QLM_MODE_80GAUI_4_C2C:
	case QLM_MODE_CAUI_4_C2C:
	case QLM_MODE_CAUI_4_C2M:
	case QLM_MODE_100G_CR4:
	case QLM_MODE_100G_KR4:
		*cnt = 1;
		*touse = 4;
		break;
	case QLM_MODE_RXAUI:
	case QLM_MODE_25GAUI_2_C2C:
	case QLM_MODE_40GAUI_2_C2C:
	case QLM_MODE_50GAUI_2_C2C:
	case QLM_MODE_50GAUI_2_C2M:
	case QLM_MODE_50G_CR2:
	case QLM_MODE_50G_KR2:
		*cnt = 1;
		*touse = 2;
		break;
	case QLM_MODE_USXGMII_4X1:
	case QLM_MODE_QSGMII:
		*cnt = 4;
		*touse = 1;
		break;
	case QLM_MODE_USXGMII_2X1:
		*cnt = 2;
		*touse = 1;
		break;
	case QLM_MODE_USXGMII_1X1:
		*cnt = 1;
		*touse = 1;
		break;
	}
}

/* Check if it is possible to configure LMAC in the current mode. Return
 * 0 in case of success, otherwise return -1.
 */
static int octeontx2_check_qlm_lmacs(int cgx_idx,
		int qlm, int mode_idx, int lmac_need)
{
	int lmac_avail;
	cgx_config_t *cgx;
	cgx_lmac_config_t *lmac;
	int i;
	int max_lanes = plat_octeontx_scfg->qlm_max_lane_num[qlm];

	debug_dts("CGX%d: qlm = %d, mode_idx = %d, lmac_need = %d\n",
			 cgx_idx, qlm, mode_idx, lmac_need);
	cgx = &(plat_octeontx_bcfg->cgx_cfg[cgx_idx]);
	lmac_avail = MAX_LMAC_PER_CGX - cgx->lmacs_used;

	/* This code is based on QLM<->CGX mapping and fixed per SoC.
	 * hence add model specific condition
	 */
	if ((mode_idx == QLM_MODE_QSGMII)
	    || (mode_idx == QLM_MODE_USXGMII_4X1)) {
		if (cgx->lmacs_used)
			lmac_avail = cgx->lmacs_used;
	} else if (cavm_is_model(OCTEONTX_CN96XX)) {
		if ((qlm == 3) || (qlm == 7)) {
			/* Only QLM3 or QLM7 may be Ethernet, not both. */
			for (i = 0; i < cgx->lmac_count; i++) {
				lmac = &cgx->lmac_cfg[i];
				if (lmac->qlm != qlm) {
					WARN("CGX%d: Can't configure mode:%s. QLM%d is requested, but QLM%d is used.",
						cgx_idx,
						qlm_get_mode_strmap(mode_idx).bdk_str,
						qlm, lmac->qlm);
					lmac_avail = 0;
					break;
				}
			}
		} else if (max_lanes == 2) {
			/* DLMs does not support quad lane Ethernet protocols.
			 * Only two lanes are available.
			 */
			lmac_avail = 2;
			for (i = 0; i < cgx->lmac_count; i++) {
				lmac = &cgx->lmac_cfg[i];
				if (lmac->qlm == qlm)
					lmac_avail--;
			}
		}
	} else if (cavm_is_model(OCTEONTX_CNF95XX)) {
		/* Only 2 lanes are available DLMs */
		if (max_lanes == 2) {
			lmac_avail = 2;
			for (i = 0; i < cgx->lmac_count; i++) {
				lmac = &cgx->lmac_cfg[i];
				if (lmac->qlm == qlm)
					lmac_avail--;
			}
		}
	}

	if (lmac_need > lmac_avail) {
		WARN("CGX%d: Can't configure mode:%s. Requires %d LMACs, but %d LMACs available on QLM%d.\n",
				cgx_idx,
				qlm_get_mode_strmap(mode_idx).bdk_str,
				lmac_need, lmac_avail, qlm);
		return -1;
	}

	return 0;
}

/* Fill CGX structure, if possible.
 * Return the number of lanes used for initialization.
 */
static int octeontx2_fill_cgx_struct(int cgx_idx, int qlm, int gserx,
			int shift_from_first, int lane)
{
	cgx_config_t *cgx;
	cgx_lmac_config_t *lmac, *temp_lmac;
	int mode, mode_idx;
	int i, j;
	int lcnt, lused, lane_to_sds;
	int lane_order;
	int phy_lane, found_lane, lane_other;
	int lane_sds_idx = 0;
	uint32_t lane_mask = 0;
	qlm_state_lane_t qlm_state;

	cgx = &(plat_octeontx_bcfg->cgx_cfg[cgx_idx]);

	/* Obtain lane order from NETWORK-LANE-ORDER property parsed
	 * from BDK DT
	 * Examples:
	 * 0x3210: Default connection, lanes in order
	 * 0x0123: All lanes are swizzled. 0->3, 1->2, 2->1, 3->0
	 */
	lane_order = cgx->network_lane_order;

	/* lane_to_sds(8 bits) is an array of 2-bit values that map each
	 * logical PCS lane in the LMAC to a physical SerDes lane.
	 * Extract the lane info from lane order and build lane_to_sds.
	 */
	lane_to_sds = ((((lane_order >> 12) & 3) << 6) |
			(((lane_order >> 8) & 3) << 4) |
			(((lane_order >> 4) & 3) << 2) |
			(((lane_order >> 0) & 3)));

	phy_lane = cgx->qlm_ops->qlm_get_lmac_phy_lane(gserx, lane, lane_to_sds);
	if ((cgx->lanes_used_mask >> phy_lane) & 1) {
		debug_dts("CGX%d: QLM%d.LANE%d:%d: already configured for CGX, skip.\n",
				  cgx_idx, qlm, lane, phy_lane);
		return 0;
	}

	/* For DLMs (2 DLMs mapped to CGX), physical lane
	 * can be >= 2. In this case, subtract lane index by
	 * 2 for DLMs so correct lane index can be passed
	 * when obtaining the MODE from SCRATCHX register.
	 */
	if (plat_octeontx_scfg->qlm_max_lane_num[qlm] == 2) {
		if (phy_lane >= 2)
			phy_lane -= 2;
	}

	qlm_state = cgx->qlm_ops->qlm_get_state(gserx, phy_lane);

	debug_dts("QLM%d.LANE%d:%d: mode=%d:%s\n",
			qlm, lane, phy_lane,
			qlm_state.s.mode,
			qlm_get_mode_strmap(qlm_state.s.mode).bdk_str);

	mode_idx = qlm_state.s.mode;

	if ((mode_idx < QLM_MODE_SGMII) || (mode_idx >= QLM_MODE_LAST)) {
		debug_dts("QLM%d.LANE%d: not configured for CGX, skip.\n",
			qlm, lane);
		return 0;
	}
	debug_dts("CGX%d: Configure QLM%d GSER%d Lane%d\n",
		cgx_idx, qlm, gserx, phy_lane);

	if (cgx->lmac_count >= MAX_LMAC_PER_CGX) {
		WARN("CGX%d: already configured, not configuring QLM%d, Lane%d\n",
				cgx_idx, qlm, lane);
		return 0;
	}

	/* if CGX is configured in USXGMII mode, don't configure other modes
	 * in the same CGX
	 */
	if (cgx->usxgmii_mode) {
		WARN("CGX%d: is configured in USXGMII mode, cannot configure other modes\n",
				cgx_idx);
		return 0;
	}

	if (((mode_idx == QLM_MODE_USXGMII_1X1) ||
			(mode_idx == QLM_MODE_USXGMII_2X1) ||
			(mode_idx == QLM_MODE_USXGMII_4X1)) &&
			(cgx->lmac_count)) {
		/* if LMACs in the same CGX for which USXGMII is configured,
		 * are configured with different modes already, UXSGMII cannot
		 * configured
		 */
		WARN("CGX%d: cannot configure USXGMII for this CGX\n",
				cgx_idx);
		return 0;
	}

	octeontx2_lmac_num_touse(mode_idx, &lcnt, &lused);
	if (!lcnt || !lused) {
		debug_dts("CGX%d: the %s mode doesn't require any LMAC initialization.\n",
				cgx_idx,
				qlm_get_mode_strmap(mode_idx).bdk_str);
		return 0;
	}
	debug_dts("CGX%d: mode_idx %d needs %d lanes, %d lmacs\n",
		cgx_idx, mode_idx, lused, lcnt);
	if (octeontx2_check_qlm_lmacs(cgx_idx, qlm, mode_idx, lcnt * lused))
		return 0;
	if (lane % (lcnt * lused)) {
		WARN("CGX%d.LANE%d: wrong phy LANE%d for the %s mode.\n",
				cgx_idx, lane, phy_lane,
				qlm_get_mode_strmap(mode_idx).bdk_str);
		return 0;
	}

	mode = qlm_get_mode_strmap(mode_idx).mode;
	/* For spanning more than one GSER (e.g DLMs) NETWORK-LANE-ORDER should be
	 * interpreted as below. The lowest lane of the lowest GSER
	 * maps to the LMAC physical lane 0 and the highest lane
	 * of the highest GSER maps to the highest LMAC physical lane (typ lane 3)
	 */

	for (i = 0; i < lcnt; i++) {
		lmac = &cgx->lmac_cfg[cgx->lmac_count];

		/* Fill in the CGX/LMAC structures */
		lmac->mode = mode;
		lmac->mode_idx = mode_idx;
		lmac->qlm = qlm;
		lmac->gserx = gserx;
		lmac->shift_from_first = shift_from_first;
		lmac->first_phy_lane = phy_lane;

		if (mode == CAVM_CGX_LMAC_TYPES_E_USXGMII) {
			if (!cgx->usxgmii_mode) {
				/* This is the first lane with USXGMII mode.
				 * update one time all LMACs lane with the same
				 * physical lane. physical SerDes lane to be
				 * programmed in case of USXGMII mode instead
				 * of lane_to_sds
				 */
				for (j = 0; j < MAX_LMAC_PER_CGX; j++) {
					temp_lmac = &cgx->lmac_cfg[j];
					if ((!strncmp(plat_octeontx_bcfg->bcfg.board_model,
						"ebb96", 5)) ||
					(!strncmp(plat_octeontx_bcfg->bcfg.board_model,
						"ebb93", 5)) ||
					(plat_get_altpkg() == CN95XXE_PKG))
						temp_lmac->lane = ~lane & 3;
					else
						temp_lmac->lane = lane;
				}
			}
		} else
			lmac->lane = lane + i;

		found_lane = 0;

		int temp_phy_lane;
		switch (mode) {
		case CAVM_CGX_LMAC_TYPES_E_XAUI:
		case CAVM_CGX_LMAC_TYPES_E_FORTYG_R:
		case CAVM_CGX_LMAC_TYPES_E_HUNDREDG_R:
			lmac->lane_to_sds = lane_to_sds;
			break;
		case CAVM_CGX_LMAC_TYPES_E_RXAUI:
		case CAVM_CGX_LMAC_TYPES_E_FIFTYG_R:
			/* The RXAUI mode is always using a double lane. So
			 * the lane value can be 0 or 2.
			 */
			for (j = 0; j < MAX_LMAC_PER_CGX; j++) {
				temp_phy_lane = lane_to_sds >> (j*2) & 0x3;
				if (temp_phy_lane == cgx->qlm_ops->qlm_get_lmac_phy_lane(
						gserx, lane, lane_to_sds)) {
					found_lane = 1;
					break;
				}
			}
			lane_sds_idx = j;

			/* If the CGX physical lane associated with the GSER
			 * lane is found, then the first 2 lanes in the
			 * network lane order are associated with this QLM
			 * otherwise it is in the upper 2 lanes in the
			 * network lane order.
			 */
			if (found_lane) {
				/* Check if  2-lane protocols to be aligned on a bit multiple of 4 */
				/* Expect 2-lane protocols to be aligned on a bit multiple of 4 */
				/* Verify partner lane is not already used */
				if (lane_sds_idx & 1)
					lane_other = (lane_to_sds >> ((lane_sds_idx - 1) * 2)) & 0x3;
				else
					lane_other = (lane_to_sds >> ((lane_sds_idx + 1) * 2)) & 0x3;
				if ((cgx->lanes_used_mask >> lane_other) & 0x1) {
					WARN("CGX%d: LANE%d already used.  Check network-lane-order.\n",
						 cgx_idx, lane_other);
					return 0;
				}
				lmac->lane_to_sds = (lane_to_sds >> ((j >> 1)*4)) & 0xF;
			} else
				WARN("CGX%d: Unable to find QLM%d LANE%d in the network-lane-order.\n",
					    cgx_idx, qlm, lane);
			break;
		default:
			lmac->lane_to_sds = cgx->qlm_ops->qlm_get_lmac_phy_lane(gserx,
							lane, lane_to_sds);
			break;
		}
		/* Set AN master lane to LMAC logical lane 0 */
		lmac->lane_an_master = lmac->lane_to_sds & 0x3;

		/* Create the GSER lane_mask */
		for (j = 0; j < lused; j++)
			lane_mask |= 1 << ((lmac->lane_to_sds >> (j * 2)) & 0x3);

		lmac->lane_mask = lane_mask;
		/* Update the CGX lane mask */
		cgx->lanes_used_mask |= lane_mask;

		/* max_lane_count is the number of SERDES lanes used by the
		 * original LMAC type (original means it came about as a result
		 * of the device tree property QLM-MODE.N0.QLM%d).  The Ethernet
		 * mode change feature will use max_lane_count to determine if
		 * the new Ethernet mode (that the user wants to change to at
		 * run-time) can be accommodated.
		 */
		lmac->max_lane_count = lused;

		debug_dts(
			"CGX%d: LANE%d: lmac lane_mask 0x%x, gserx %d, first_gserx %d, qlm %d, cgx_lane_mask 0x%x\n",
				cgx_idx, lane, lmac->lane_mask, lmac->gserx,
				lmac->shift_from_first + lmac->gserx,
				lmac->qlm, cgx->lanes_used_mask);

		switch (mode_idx) {
		case QLM_MODE_10G_KR:
		case QLM_MODE_25G_KR:
		case QLM_MODE_25G_CR:
		case QLM_MODE_40G_KR4:
		case QLM_MODE_40G_CR4:
		case QLM_MODE_50G_KR2:
		case QLM_MODE_50G_CR2:
		case QLM_MODE_100G_KR4:
		case QLM_MODE_100G_CR4:
			lmac->use_training = 1;
			break;
		case QLM_MODE_USXGMII_1X1:
		case QLM_MODE_USXGMII_2X1:
		case QLM_MODE_USXGMII_4X1:
			/* set USXGMII for this CGX */
			cgx->usxgmii_mode = 1;
		case QLM_MODE_XFI:
		case QLM_MODE_SFI:
		case QLM_MODE_XLAUI:
		case QLM_MODE_XLAUI_C2M:
		case QLM_MODE_RXAUI:
		case QLM_MODE_XAUI:
		/* Fixed speed option. consider as AN disabled cases */
		case QLM_MODE_20GAUI_C2C:
		case QLM_MODE_25GAUI_C2C:
		case QLM_MODE_25GAUI_C2M:
		case QLM_MODE_25GAUI_2_C2C:
		case QLM_MODE_40GAUI_2_C2C:
		case QLM_MODE_50GAUI_2_C2C:
		case QLM_MODE_50GAUI_2_C2M:
		case QLM_MODE_50GAUI_4_C2C:
		case QLM_MODE_80GAUI_4_C2C:
		case QLM_MODE_CAUI_4_C2C:
		case QLM_MODE_CAUI_4_C2M:
			/* FIXME : always disable AN for USXGMII for now */
			lmac->autoneg_dis = 1;
			break;
		}

		cgx->lmac_count++;
		cgx->lmacs_used += lused;
		cgx->load += qlm_get_mode_strmap(mode_idx).eth_link_speed;

		/* In case of 1000 BASE-X, update the property of LMAC */
		if (mode_idx == QLM_MODE_1G_X) {
			lmac->sgmii_1000x_mode = 1;
			lmac->autoneg_dis = 1;
		}
	}

	cgx->enable = 1;

	/* If a QLM/DLM is configured as USXGMII (even if it uses 1 LMAC),
	 * CGX may not be used by other lanes. Hence always return 4
	 */
	if (cgx->usxgmii_mode) {
		cgx->lanes_used_mask = 0xf;
		return 4;
	}

	return (lcnt * lused);
}

/*
 * From the DTS str list of devices finds the required led_node
 * and returns pointer to it.
 *
 */
static const char *octeontx2_cgx_get_led_node_ptr(const char *property_start,
	const char *property_end, const char *node_name)
{
	const char *node_ptr = NULL;

	if (!property_start || !property_end) {
		debug_dts("GPIO Led helper start and end pointers not valid\n");
		return NULL;
	}

	while (property_start < property_end) {
		node_ptr = octeontx2_first_substring(property_start, node_name);
		if (!node_ptr)
			property_start += (strlen(property_start) + 1);
		else
			break;
	}

	if (!node_ptr) {
		debug_dts
			("GPIO LED link node not found for CGX LMAC::%s\n"
			, node_name);
		return NULL;
	}

	return node_ptr;
}

/*
 * Checks if LED node entry represents a direct gpio.
 * Returns:
 *   1: if the gpio is direct (dev=N0.GPIO0)
 *   0: for all other cases (gpio behind mux/expander or
 *      'dev' attribute wasn't found)
 *
 */
static int octeontx2_cgx_check_led_gpio_is_direct(const char *node_name,
	const char *node_ptr)
{
	const char *devname_ptr;

	devname_ptr = octeontx2_first_substring(node_ptr, "dev");
	if (!devname_ptr) {
		debug_dts
			("GPIO LED: 'dev' attribute not found for CGX LMAC::%s\n"
			, node_name);
		return 0;
	}

	/* now, move the ptr after 'dev=', where actual name starts  */
	devname_ptr = strchr(devname_ptr, '=') + 1;

	if (!strncmp(devname_ptr, "N0.GPIO0", sizeof("N0.GPIO0") - 1))
		return 1;

	return 0;
}

/*
 * From the LED node pointer gets the gpio pin and returns it
 *
 */
static int octeontx2_cgx_update_gpio_leds_helper(const char *node_name,
	const char *node_ptr, const char *find_attr)
{
	char gpio[2];
	int devstr_len = 0;
	int index = 0, gindex = 0;
	const char *gpio_ptr = NULL;

	gpio_ptr = octeontx2_first_substring(node_ptr, find_attr);
	if (!gpio_ptr) {
		debug_dts
			("GPIO LED link gpio not found for CGX LMAC::%s\n"
			, node_name);
		return -1;
	}

	devstr_len = strlen(gpio_ptr);
	index = 0;
	while (index < devstr_len) {
		if (gpio_ptr[index] == ',') {
			/*Assuming GPIO value between 0-9*/
			if (gindex)
				break;

			debug_dts
				("GPIO LED link gpio missed for CGX LMAC::%s\n"
				, node_name);
			return -1;
		}
		if (gpio_ptr[index] >= '0' && gpio_ptr[index] <= '9')
			gpio[gindex++] = gpio_ptr[index];

		/*Assuming GPIO value only between 10-99*/
		if (gindex >= 2)
			break;

		index++;
	}

	gpio[gindex] = '\0';
	return octeontx2_atoi(gpio);
}

static void octeontx2_cgx_update_gpio_leds(const void *fdt,
	int cgx_idx, int lmac_idx, cgx_lmac_config_t *lmac)
{
	char prop[64];
	int offset = 0;
	int gpio_act = -1;
	int gpio_link = -1;
	int property_len = 0;
	int gpio_link_drv = 1;
	const char *act_node_name = NULL;
	const char *act_node_ptr = NULL;
	const char *link_node_name = NULL;
	const char *link_node_ptr = NULL;
	const void *property_end = NULL;
	const void *property_start = NULL;

	if (lmac) {
		lmac->gpio_led.prev_tx_pkt_cnt = 0;
		lmac->gpio_led.prev_rx_pkt_cnt = 0;
		lmac->gpio_led.is_act_supported = 0;
		lmac->gpio_led.is_link_supported = 0;
		lmac->gpio_led.link_active_high = 1;
	}

	offset = fdt_path_offset(fdt, "/cavium,bdk");
	if (offset > 0) {
		property_start = fdt_getprop(fdt, offset, "DEVICES",
			&property_len);
		if (property_start == NULL) {
			debug_dts("GPIO LED devices property not found\n");
			return;
		}
		property_end = property_start + property_len;
	} else {
		debug_dts("GPIO LED fdt root offset not found\n");
		return;
	}

	snprintf(prop, sizeof(prop), "NETWORK-LED-LINK-FAST.N0.CGX%d.P%d",
		 cgx_idx, lmac_idx);

	link_node_name = octeontx2_fdtbdk_get_str(fdt, prop);
	if (link_node_name) {
		link_node_ptr = octeontx2_cgx_get_led_node_ptr(
			property_start, property_end, link_node_name);
	} else {
		debug_dts
			("Gpio LED link not set for CGX::%d LMAC::%d\n",
				cgx_idx, lmac_idx);
	}

	memset(prop, 0, sizeof(prop));
	snprintf(prop, sizeof(prop), "NETWORK-LED-ACTIVITY.N0.CGX%d.P%d",
		 cgx_idx, lmac_idx);
	act_node_name = octeontx2_fdtbdk_get_str(fdt, prop);
	if (act_node_name) {
		act_node_ptr = octeontx2_cgx_get_led_node_ptr(
			property_start, property_end, act_node_name);
	} else {
		debug_dts("Gpio LED act not set for CGX::%d LMAC::%d\n",
			cgx_idx, lmac_idx);
	}

	if (!link_node_name && !act_node_name) {
		debug_dts("Gpio LED property not set for CGX::%d LMAC::%d\n",
			 cgx_idx, lmac_idx);
		return;
	}

	if (link_node_ptr) {
		if (octeontx2_cgx_check_led_gpio_is_direct(
			link_node_name, link_node_ptr)) {

			gpio_link = octeontx2_cgx_update_gpio_leds_helper(
				link_node_name, link_node_ptr, "gpio");
			gpio_link_drv = octeontx2_cgx_update_gpio_leds_helper(
				link_node_name, link_node_ptr, "active_high");
		} else {
			debug_dts("link LED gpio is not direct - parsing skipped\n");
		}
	}

	if (act_node_ptr) {
		if (octeontx2_cgx_check_led_gpio_is_direct(
			act_node_name, act_node_ptr)) {

			gpio_act = octeontx2_cgx_update_gpio_leds_helper(
				act_node_name, act_node_ptr, "gpio");
		} else {
			debug_dts("act LED gpio is not direct - parsing skipped\n");
		}
	}

	if (gpio_link >= 0) {
		lmac->gpio_led.link_status = 0;
		lmac->gpio_led.link = gpio_link;
		lmac->gpio_led.link_active_high = gpio_link_drv;
		lmac->gpio_led.is_link_supported = 1;
	}

	if (gpio_act >= 0) {
		lmac->gpio_led.activity = gpio_act;
		lmac->gpio_led.is_act_supported = 1;
	}
}

/*
 * Function parsing PHY info
 * Returns:
 *   0: if PHY was parsed & will be managed in ATF
 *   1: parsing skipped as PHY is going to be be managed in kernel
 *  -1: on parsing error
 *
 */
static int octeontx2_cgx_get_phy_info(void *fdt, int lmac_offset, int cgx_idx, int lmac_idx)
{
	cgx_lmac_config_t *lmac;
	int phy_offset, mux_offset;
	char phyname[16];
	int mdio_bus_offset;
	int lenp;
	phy_config_t *phy;

	lmac = &plat_octeontx_bcfg->cgx_cfg[cgx_idx].lmac_cfg[lmac_idx];

	strlcpy(phyname, "phy-handle", sizeof(phyname));

	/* FIXME: Using board model is safe to use */
	if (!strncmp(plat_octeontx_bcfg->bcfg.board_model, "ebb96", 5)) {
		/* On EBB9604 board, PHY address can be different on
		 * QLM 3 and QLM 7 and at a time, ethernet can be configured
		 * either on QLM3 or QLM7 only and not both. CGX0 is mapped to
		 * QLM 3 or QLM 7. Hence if it is CGX0, check for the
		 * QLM number it is mapped to look for the phy-handle.
		 * QLM 3: phy-handle
		 * QLM 7: phy-handle1
		 */
		if ((cgx_idx == 0) && (lmac->qlm == 7))
			strlcpy(phyname, "phy-handle1", sizeof(phyname));
	}

	phy_offset = octeontx2_fdt_lookup_phandle(fdt, lmac_offset, phyname);
	if (phy_offset > 0) {
		phy = &lmac->phy_config;

		phy->mdio_bus = octeontx2_fdt_get_bus(fdt,
				phy_offset, cgx_idx,
				lmac_idx);

		if (phy->mdio_bus < 0 || phy->mdio_bus >= MDIO_NUM) {
			ERROR("ERROR: Incorrect mdio bus number\n");
			return -1;
		}

		/* Check if MDIO bus, the PHY is on, has the "mdio-in-kernel"
		 * attribute specified. If yes, then skip parsing the PHY.
		 * Otherwise both, bus and the PHY, are going to be trimmed
		 * from the Linux dts.
		 */
		mdio_bus_offset = fdt_parent_offset(fdt, phy_offset);
		if (fdt_get_property(fdt,
				mdio_bus_offset, "mdio-in-kernel", &lenp)) {

			debug_dts("%s: %d:%d PHY parsing skipped. "
					"MDIO bus managed in kernel\n",
					__func__, cgx_idx, lmac_idx);
			return 1;
		} else if (lenp == -FDT_ERR_NOTFOUND) {

			/* Update the list of MDIO bus nodes to be trimmed */
			if (!mdio_trim_list[phy->mdio_bus])
				mdio_trim_list[phy->mdio_bus] = mdio_bus_offset;

			/* Remove the reference to the PHY from the lmac node */
			fdt_nop_property(fdt, lmac_offset, phyname);
		}

		for (int i = 0; i < ARRAY_SIZE(phy_compat_list); i++) {
			if (!fdt_node_check_compatible(fdt, phy_offset,
				phy_compat_list[i].compatible)) {
				phy->type = phy_compat_list[i].phy_type;
				debug_dts("%s: %d:%d PHY type %d\n",
					__func__, cgx_idx, lmac_idx,
					phy->type);
				break;
			}
		}
		if (phy->type == PHY_NONE) {
			ERROR("ERROR: Supported PHY compatible not found\n");
			return -1;
		}

		/* Save the PHY address and bus for all PHY types */
		phy->addr = octeontx2_fdt_get_int32(fdt,
					"reg", phy_offset);

		phy->port = octeontx2_fdt_get_int32(fdt,
					"port", phy_offset);

		phy->host_order = octeontx2_fdt_get_int32(fdt,
					"host_order", phy_offset);

		phy->line_order = octeontx2_fdt_get_int32(fdt,
					"line_order", phy_offset);

		/* Assign default lane order if property is not
		 * present in DT
		 */
		if (phy->host_order == -1)
			phy->host_order = 0x3210;
		if (phy->line_order == -1)
			phy->line_order = 0x3210;

		/* Check if the MDIO bus is behind a switch */
		mux_offset = octeontx2_fdt_lookup_phandle(fdt,
					phy_offset, "mdio-mux");
		if (mux_offset > 0) {
			if (fdt_node_check_compatible(fdt, mux_offset,
						"octeontx2,mdio-mux")) {
				WARN("%s: %d:%d MDIO switch not compatible\n",
							__func__,
							cgx_idx, lmac_idx);
				return -1;
			}

			octeontx2_fdt_gpio_get_info_by_phandle(fdt,
					mux_offset, "gpios",
					&phy->mux_info, cgx_idx, lmac_idx);
			phy->mux_switch = 1;
		}
		lmac->phy_present = 1;

		/* Vitesse PHY has AN by default. Even for 1000 BASE-X
		 * mode, enable AN in this case
		 */
		if ((lmac->mode_idx == QLM_MODE_1G_X) &&
				(phy->type == PHY_VITESSE_8574))
			lmac->autoneg_dis = 0;
	}
	return 0;
}

/* Get the LMAC information from the Linux DT file. The following properties
 * are checked:
 *  - phy-handle
 *  - num-rvu-vfs
 *  - num-msix-vec
 * SGMII/QSGMII only:
 *  - octeontx,sgmii-mac-phy-mode
 *  - octeontx,disable-autonegotiation
 */
static void octeontx2_cgx_lmacs_check_linux(void *fdt,
		cgx_config_t *cgx, int cgx_idx, int cgx_offset, int *fdt_vfs)
{
	int lmac_idx;
	cgx_lmac_config_t *lmac;
	char name[16], node_name[64];
	const int *val;
	int len, ret;
	int lmac_offset, sfp_offset, qsfp_offset;
	int req_vfs, req_fec, phy_mod_type;
	char sfpname[16], qsfpname[16];
	int lmac_count = 0;

	for (lmac_idx = 0; lmac_idx < cgx->lmac_count; lmac_idx++) {
		int lane = 0;
		lmac = &cgx->lmac_cfg[lmac_idx];

		debug_dts("%s: plat_octeontx_bcfg->qlm_auto_config %d lmac_idx %d lane %d lmac->first_phy_lane %d\n",
				__func__, plat_octeontx_bcfg->qlm_auto_config, lmac_idx, lmac->lane, lmac->first_phy_lane);

		/* BDK passses the same lane as configured as DTS
		 * when QLM-AUTO-CONFIG is set as 0 even though
		 * lanes are swizzled. If QLM-AUTO-CONFIG is set as
		 * 1 and lanes are swizzled, lane is configured
		 * differently. Differentiate the case based on
		 * qlm_auto_config and assign the lane index
		 * to parse the DT.
		 */
		debug_dts("%s: cgx_idx %d plat_octeontx_bcfg->qlm_auto_config %d lmac_idx %d lane %d lmac->first_phy_lane %d num_lanes %d\n", __func__, cgx_idx, plat_octeontx_bcfg->qlm_auto_config,
			lmac_idx, lmac->lane, lmac->first_phy_lane, lmac->max_lane_count);

		/* In case of MODE that is using 2 lanes, lane index
		 * should be aligned to obtain the correct DT entry
		 */
		if (lmac->mode_idx == QLM_MODE_QSGMII)
			lane = lmac_idx;
		else if (((!plat_octeontx_bcfg->qlm_auto_config) ||
			(!strncmp(plat_octeontx_bcfg->bcfg.board_model, "ebb9504n", 8)))
			&& (lmac->max_lane_count != 4))	{
			lane = lmac->first_phy_lane;
			/* For 2 DLMs mapped to 1 CGX, for the higher
			 * DLM, increment the lane index by 2 (phy_lane
			 * was adjusted in LMAC structure earlier) to
			 * get the correct entry from DT
			 */
			if (lmac->shift_from_first == -1)
				lane += 2;
			if ((lmac->max_lane_count == 2) &&
				(lmac->lane != lmac->first_phy_lane))
				lane -= 1;
		} else {
			/* If AUTO-CONFIG is 1 or AUTO-CONFIG is 0
			 * and if the number of lanes is not 1 or 2,
			 * lane index should be same as LMAC index
			 */
			lane = lmac_idx;
			if ((lmac->max_lane_count == 2) && lane)
				lane += 1;
		}

		snprintf(name, sizeof(name), "%s@%d%d",
				qlm_get_mode_strmap(lmac->mode_idx).linux_str,
				cgx_idx, lane);
		lmac_offset = fdt_subnode_offset(fdt, cgx_offset, name);
		if (lmac_offset < 0) {
			/* In case of QSGMII, not all LMACs needs DT entries based on board
			 * design, so it is fine to skip if DTS entries are not present
			 */
			if (lmac->mode_idx == QLM_MODE_QSGMII)
				lmac_count++;
			else
				ERROR("CGX%d.LMAC%d: DT:%s not found in device tree\n",
						cgx_idx, lmac_idx, name);
			continue;
		}

		ret = octeontx2_cgx_get_phy_info(fdt, lmac_offset, cgx_idx, lmac_idx);
		if (ret == -1) {
			/* If there are errors encountered in obtaining the valid PHY
			 * info in case of PHY present, don't enable the LMAC. just return
			 * here.
			 */
			WARN("%s: %d:%d PHY info not correct\n", __func__,
						cgx_idx, lmac_idx);
			continue;
		}

		/* Update GPIO LED information in LMAC config */
		octeontx2_cgx_update_gpio_leds(fdt, cgx_idx, lmac_idx, lmac);

		strlcpy(sfpname, "sfp-slot", sizeof(sfpname));
		strlcpy(qsfpname, "qsfp-slot", sizeof(qsfpname));

		/* FIXME: Using board model is safe to use for now */
		if (!strncmp(plat_octeontx_bcfg->bcfg.board_model, "ebb9", 4)) {
			/* On EBB9604 board, PHY address can be different on
			 * QLM 3 and QLM 7 and at a time, ethernet can be configured
			 * either on QLM3 or QLM7 only and not both. CGX0 is mapped to
			 * QLM 3 or QLM 7. Hence if it is CGX0, check for the
			 * QLM number it is mapped to look for the sfp-slot.
			 * QLM 3: sfp-slot
			 * QLM 7: sfp-slot1
			 */
			if ((cgx_idx == 0) && (lmac->qlm == 7)) {
				strlcpy(sfpname, "sfp-slot1", sizeof(sfpname));
				strlcpy(qsfpname, "qsfp-slot1", sizeof(qsfpname));
			}
		}

		/* Check for sfp-slot info */
		sfp_offset = octeontx2_fdt_lookup_phandle(fdt,
					lmac_offset, sfpname);
		if (sfp_offset > 0 && !octeontx2_fdt_parse_sfp_info(fdt,
				sfp_offset, cgx_idx, lmac_idx)) {

			/* sfp node is managed in ATF and we are
			 * done with parsing it, thus remove the reference
			 * from the lmac node
			 */
			fdt_nop_property(fdt, lmac_offset, sfpname);
		}

		/* Check for qsfp-slot info */
		qsfp_offset = octeontx2_fdt_lookup_phandle(fdt,
				lmac_offset, qsfpname);
		if (qsfp_offset > 0 && !octeontx2_fdt_parse_qsfp_info(fdt,
				qsfp_offset, cgx_idx, lmac_idx)) {

			/* qsfp node is managed in ATF and we are
			 * done with parsing it, thus remove the reference
			 * from the lmac node
			 */
			fdt_nop_property(fdt, lmac_offset, qsfpname);
		}

		/* Override fec, phy-mod params from flash if exist on
		 * same qlm mode.
		 */
		if (!cgx_read_flash_fec(cgx_idx, lmac_idx, &req_fec)) {
			lmac->fec = req_fec;
		} else {
			/* Handle FEC types.
			 * octeontx, fec-type : Host side of PHY/CGX
			 * octeontx, line-fec-type : line side of PHY
			 */
			val = fdt_getprop(fdt, lmac_offset,
					  "octeontx,fec-type", &len);
			if (val) {
				req_fec = fdt32_to_cpu(*val);
				lmac->fec = cgx_validate_fec_config(cgx_idx,
								    lmac_idx,
								    req_fec);
			} else {
				/* User did not specify FEC type property
				 * in the DT. set it to -1 to configure
				 * the default type.
				 */
				lmac->fec = -1;
			}
			val = fdt_getprop(fdt, lmac_offset,
					  "octeontx,line-fec-type", &len);
			if (val) {
				req_fec = fdt32_to_cpu(*val);
				lmac->line_fec = req_fec;
			} else {
				/* User did not specify FEC type property
				 * in the DT. set it to CGX_FEC_NONE
				 * to configure the default type.
				 */
				lmac->line_fec = CGX_FEC_NONE;
			}
		}
		if (!cgx_read_flash_phy_mod(cgx_idx, lmac_idx,
					    &phy_mod_type)) {
			lmac->phy_config.mod_type =
				(phy_mod_type == PHY_MOD_TYPE_PAM4) ?
				PHY_MOD_TYPE_PAM4 : PHY_MOD_TYPE_NRZ;
		} else {
			/* Handle PHY line-side modulation type */
			val = fdt_getprop(fdt, lmac_offset,
					  "octeontx,phy-mod-type", &len);
			if (val) {
				phy_mod_type = fdt32_to_cpu(*val);
				lmac->phy_config.mod_type =
					(phy_mod_type == PHY_MOD_TYPE_PAM4) ?
					PHY_MOD_TYPE_PAM4 : PHY_MOD_TYPE_NRZ;
			} else {
				/* User did not specify PHY modulation type
				 * in the DT. Set it to NRZ, the default type.
				 */
				lmac->phy_config.mod_type = PHY_MOD_TYPE_NRZ;
			}
		}
		if (lmac->phy_config.mod_type == PHY_MOD_TYPE_PAM4) {
			switch (lmac->mode_idx) {
			/* For now, only these two QLM modes are permitted to
			 * have PAM4 modulation on the PHY line side.
			 */
			case QLM_MODE_50GAUI_2_C2C:
			case QLM_MODE_50GAUI_4_C2C:
				/* PAM4 requires RS-FEC */
				lmac->fec = CGX_FEC_RS;
				break;
			default:
				/* PAM4 on PHY line-side does not apply for
				 * other QLM modes.  Must use NRZ.
				 */
				lmac->phy_config.mod_type = PHY_MOD_TYPE_NRZ;
				break;
			}
		}

		/* Construct the proper node name for error handling */
		snprintf(node_name, sizeof(node_name), "%s/%s",
			 fdt_get_name(fdt, cgx_offset, NULL),
			 fdt_get_name(fdt, lmac_offset, NULL));
		val = fdt_getprop(fdt, lmac_offset, "num-rvu-vfs", &len);
		if (val) {
			/* We've got that property, handle any errors with config */
			req_vfs = fdt32_to_cpu(*val);
			lmac->num_rvu_vfs = octeontx2_handle_num_rvu_vfs(req_vfs,
						DEFAULT_VFS, fdt_vfs, node_name);
		} else {
			/* If there's no such property in FDT
			 * try to assign default VFS */
			VERBOSE("RVU: No num-rvu-vfs property for node %s\n", name);
			lmac->num_rvu_vfs = octeontx2_handle_num_rvu_vfs(DEFAULT_VFS,
						DEFAULT_VFS, fdt_vfs, node_name);
		}

		/* Increment number of allocated HWVFs */
		*fdt_vfs += lmac->num_rvu_vfs;

		val = fdt_getprop(fdt, lmac_offset, "num-msix-vec", &len);
		if (val)
			lmac->num_msix_vec = fdt32_to_cpu(*val);
		else {
			VERBOSE("CGX%d.LMAC%d: num-msix-vec not set, configuring %d number of MSIX.\n",
					cgx_idx, lmac_idx, DEFAULT_MSIX_LMAC);
			lmac->num_msix_vec = DEFAULT_MSIX_LMAC;
		}

		/* Fields only for the SGMII/QSGMII LMAC types. */
		if ((lmac->mode == CAVM_CGX_LMAC_TYPES_E_SGMII) ||
				(lmac->mode == CAVM_CGX_LMAC_TYPES_E_QSGMII)) {
			val = fdt_getprop(fdt, lmac_offset,
					"octeontx,sgmii-mac-phy-mode", &len);
			if (val)
				lmac->phy_mode = 1;
			val = fdt_getprop(fdt, lmac_offset,
					"octeontx,disable-autonegotiation",
					&len);
			if (val)
				lmac->autoneg_dis = 1;
		}

		/* Enable LMAC */
		lmac->lmac_enable = 1;
	}
	/* In case of QSGMII, all 4 entries in DTS may not be present.
	 * In that case, reduce the lmac count of entries not present
	 */
	if (lmac_count)
		cgx->lmac_count -= lmac_count;
}

/* Main routine to parse the CGX information from the Linux DT file. */
static void octeontx2_cgx_check_linux(void *fdt)
{
	int i;
	cgx_config_t *cgx;
	int offset, cgx_offset;
	int fdt_vfs = 0;
	char name[16];

	offset = fdt_path_offset(fdt, "/soc@0");
	if (offset < 0) {
		ERROR("DT: Can't find CGX information in the Linux DT.\n");
		return;
	}
	offset = fdt_node_offset_by_compatible(fdt, offset, "pci-bridge");
	if (offset < 0) {
		ERROR("DT: Unable to find mrml_bridge node.\n");
		return;
	}

	for (i = 0; i < plat_octeontx_scfg->cgx_count; i++) {
		cgx = &(plat_octeontx_bcfg->cgx_cfg[i]);
		snprintf(name, sizeof(name), "cgx@%d", i);
		if (!cgx->lmac_count)
			continue;
		cgx_offset = fdt_subnode_offset(fdt, offset, name);
		if (cgx_offset < 0) {
			ERROR("DT: %s node present in the device tree\n", name);
			continue;
		}
		octeontx2_cgx_lmacs_check_linux(fdt, cgx, i, cgx_offset, &fdt_vfs);
	}

	/* As all the ATF-managed sfp/qsfps are parsed, we can proceed to
	 * trim associated twsi buses from Linux dts
	 */
	for (int i = 0; i < TWSI_NUM; i++) {
		if (twsi_trim_list[i]) {
			fdt_nop_node(fdt, twsi_trim_list[i]);
		}
	}

	/* Also, MDIO bus nodes that have no "mdio-in-kernel" attribute
	 * are trimmed along with their subnodes (PHYs).
	 */
	for (int i = 0; i < MDIO_NUM; i++) {
		if (mdio_trim_list[i]) {
			fdt_nop_node(fdt, mdio_trim_list[i]);
		}
	}

	/* Parse RVU configuration */
	octeontx2_parse_rvu_config(fdt, &fdt_vfs);
}

/* Assign all the possible MAC addresses to the LMAC initialized.
 * This is made according to the values from the BDK DT file:
 *   BOARD-MAC-ADDRESS-NUM
 *   BOARD-MAC-ADDRESS
 * First "N" LMACs will be configured. Remaining interfaces will be
 * initialized with zeros.
 */
static void octeontx2_cgx_assign_mac(const void *fdt)
{
	int cgx_idx, lmac_idx;
	cgx_config_t *cgx;
	cgx_lmac_config_t *lmac;
	int mac_num;
	int override;
	long mac;

	/* Parse BDK DT file, to find variables to set MAC address:
	 *   BOARD-MAC-ADDRESS-NUM
	 *   BOARD-MAC-ADDRESS-NUM-OVERRIDE
	 *   BOARD-MAC-ADDRESS
	 */
	mac_num = octeontx2_fdtbdk_get_num(fdt, "BOARD-MAC-ADDRESS-NUM", 10);
	if (!mac_num)
		mac_num = octeontx2_fdtbdk_get_num(fdt, "BOARD-MAC-ADDRESS-NUM", 16);
	debug_dts("BOARD-MAC-ADDRESS-NUM=%d\n", mac_num);
	override = octeontx2_fdtbdk_get_num(fdt, "BOARD-MAC-ADDRESS-NUM-OVERRIDE", 10);
	if (override >= 0) {
		debug_dts("Override number of MAC to set=%d.\n", override);
		mac_num = override;
	}
	if (mac_num <= 0) {
		debug_dts("No MAC addresses should be set.\n");
		return;
	}
	mac = octeontx2_fdtbdk_get_num(fdt, "BOARD-MAC-ADDRESS", 16);
	debug_dts("BOARD-MAC-ADDRESS=%lx\n", mac);
	if (mac == -1) {
		debug_dts("Base MAC address is not defined.\n");
		return;
	}

	/* Update the board configuration */
	plat_octeontx_bcfg->pf_mac_base = mac;
	plat_octeontx_bcfg->pf_mac_num = mac_num;

	/* Initialize N first LMACs with the MAC address. */
	for (cgx_idx = 0; cgx_idx < plat_octeontx_scfg->cgx_count; cgx_idx++) {
		cgx = &(plat_octeontx_bcfg->cgx_cfg[cgx_idx]);
		for (lmac_idx = 0; lmac_idx < cgx->lmac_count; lmac_idx++) {
			lmac = &cgx->lmac_cfg[lmac_idx];
			if (!lmac->lmac_enable)
				continue;
			lmac->local_mac_address[0] = (mac >> 40) & 0xff;
			lmac->local_mac_address[1] = (mac >> 32) & 0xff;
			lmac->local_mac_address[2] = (mac >> 24) & 0xff;
			lmac->local_mac_address[3] = (mac >> 16) & 0xff;
			lmac->local_mac_address[4] = (mac >> 8) & 0xff;
			lmac->local_mac_address[5] = mac & 0xff;
			mac++;
			mac_num--;
			/* If there are no free LMACs, then just return
			 * from the routine.
			 */
			if (!mac_num) {
				debug_dts("All free MAC addresses are assigned.\n");
				return;
			}
		}
	}
}

static void octeontx2_fill_cgx_network_lane_order(const void *fdt)
{
	int cgx, order;
	char prop[64];

	for (cgx = 0; cgx < MAX_CGX; cgx++) {
		snprintf(prop, sizeof(prop), "NETWORK-LANE-ORDER.N0.CGX%d",
				cgx);
		order = octeontx2_fdtbdk_get_num(fdt, prop, 16);
		if (order == -1) {
			/* Look for generic string */
			snprintf(prop, sizeof(prop), "NETWORK-LANE-ORDER");
			order = octeontx2_fdtbdk_get_num(fdt, prop, 16);
			if (order == -1)
				order = CGX_DEFAULT_NETWORK_LANE_ORDER;
		}
		plat_octeontx_bcfg->cgx_cfg[cgx].network_lane_order = order;
		debug_dts("%s: cgx %d lane order 0x%x\n", __func__, cgx,
			plat_octeontx_bcfg->cgx_cfg[cgx].network_lane_order);
	}
}

static void octeontx2_fill_cgx_init_net_flags(const void *fdt)
{
	int cgx, order, lmac;
	char prop[64];

	for (cgx = 0; cgx < MAX_CGX; cgx++) {
		for (lmac = 0; lmac < MAX_LMAC_PER_CGX; lmac++) {
			snprintf(prop, sizeof(prop), "NETWORK-FLAGS.N0.CGX%d.P%d",
				 cgx, lmac);
			order = octeontx2_fdtbdk_get_num(fdt, prop, 16);
			if (order == -1) {
				snprintf(prop, sizeof(prop), "NETWORK-FLAGS.N0.CGX%d",
					 cgx);
				order = octeontx2_fdtbdk_get_num(fdt, prop, 16);
				if (order == -1) {
					snprintf(prop, sizeof(prop), "NETWORK-FLAGS.N0");
					order = octeontx2_fdtbdk_get_num(fdt, prop, 16);
					if (order == -1) {
						/* Look for generic string */
						snprintf(prop, sizeof(prop), "NETWORK-FLAGS");
						order = octeontx2_fdtbdk_get_num(fdt, prop, 16);
						if (order == -1)
							order = 0; /* No FEC */
					}
				}
			}
			plat_octeontx_bcfg->cgx_cfg[cgx].lmac_cfg[lmac].net_flags = order;
			debug_dts("%s: cgx%d.%d network_flags 0x%x\n", __func__, cgx, lmac,
				  plat_octeontx_bcfg->cgx_cfg[cgx].lmac_cfg[lmac].net_flags);
		}
	}
}

static void octeontx2_map_cgx_to_nix(void)
{
	struct {
		nix_block_t block;
		int load;
	} *nix, na[2] = { /* "na" means NIX array */
		[0] = { .block = NIX0, .load = 0 },
		[1] = { .block = NIX1, .load = 0 }
	};
	cgx_config_t *ca[MAX_CGX]; /* "ca" means CGX array */
	cgx_config_t *cgx, *temp;
	int i, j, ca_size;

	/* Put all enabled CGXs in a temporary CGX array */
	ca_size = 0;
	for (i = 0; i < MAX_CGX; i++) {
		cgx = &plat_octeontx_bcfg->cgx_cfg[i];
		if (cgx->enable) {
			ca[ca_size] = cgx;
			ca_size++;
		}
	}

	if (plat_octeontx_get_nix_count() > 2)
		WARN("Unsupported platform NIX block count %d\n",
		     plat_octeontx_get_nix_count());

	if (plat_octeontx_get_nix_count() != 2) {
		/* If only one NIX block, handle it. */
		for (i = 0; i < ca_size; i++) {
			debug_dts("cgx%d using NIX%d\n", i, 0);
			ca[i]->nix_block = NIX0;
		}
		return;
	}

	/* If we reach this point, then we're dealing with two NIX blocks.
	 * Map LMACs to NIX blocks in a way that balances the load.
	 *
	 * Use Selection Sort algorithm to put the CGX array in descending order
	 * by load; if there's a tie, then put the CGX with fewer LMACs at a
	 * lower array index.
	 */
	for (i = 0; i < ca_size; i++) {
		for (j = i + 1; j < ca_size; j++) {
			if (ca[j]->load > ca[i]->load) {
				temp = ca[i];
				ca[i] = ca[j];
				ca[j] = temp;
			} else if (ca[j]->load == ca[i]->load) {
				if (ca[j]->lmac_count < ca[i]->lmac_count) {
					temp = ca[i];
					ca[i] = ca[j];
					ca[j] = temp;
				}
			}
		}
	}

	for (i = 0; i < ca_size; i++) {
		cgx = ca[i];

		/* Point to the NIX that has the lighter load; if there's a tie,
		 * then pick NIX0.
		 */
		if (na[1].load < na[0].load)
			nix = &na[1];
		else
			nix = &na[0];

		debug_dts("CGX%d: NIX load(%d,%d), balancing to use NIX%d\n", i,
			  na[0].load, na[1].load, nix->block - NIX0);

		/* Map all LMACs of this CGX to the NIX */
		cgx->nix_block = nix->block;

		/* Update the load on the NIX */
		nix->load += cgx->load;
	}
}

/* BDK fills the CAVM_GSERNX_LANEX_SCRATCH0 register with mode used by LANE.
 * The routine goes through all the QLM/LANE sets and initializes
 * CGX/LMAC, if any.
 * After it the Linux DT file is used to get other information for CGX.
 */
static void octeontx2_fill_cgx_details(void *fdt)
{
	int qlm_idx;
	int lane_idx;
	int lnum;
	int cgx_idx;
	int shift_from_first;
	int gserx;
	cgx_config_t *cgx;
	const qlm_ops_t *qlm_ops;

	octeontx2_fill_cgx_network_lane_order(fdt);
	octeontx2_fill_cgx_init_net_flags(fdt);

	for (cgx_idx = 0; cgx_idx < plat_octeontx_scfg->cgx_count; cgx_idx++) {
		qlm_ops = plat_otx2_get_qlm_ops(cgx_idx);
		if (qlm_ops == NULL) {
			INFO("%s:get_qlm_ops failed for CGX %d\n",
				__func__, cgx_idx);
		}
		plat_octeontx_bcfg->cgx_cfg[cgx_idx].qlm_ops = qlm_ops;
	}

	for (qlm_idx = 0; qlm_idx < plat_octeontx_scfg->gser_count; qlm_idx++) {
		cgx_idx = plat_get_cgx_idx(qlm_idx);
		if ((cgx_idx < 0) ||
		    (cgx_idx >= plat_octeontx_scfg->cgx_count)) {
			continue;
		}
		cgx = &(plat_octeontx_bcfg->cgx_cfg[cgx_idx]);

		if (cgx->qlm_ops == NULL) {
			WARN("%s:CGX%d: has no qlm_ops\n",  __func__, cgx_idx);
			continue;
		}

		gserx = plat_otx2_get_gserx(qlm_idx, &shift_from_first);

		lnum = plat_octeontx_scfg->qlm_max_lane_num[qlm_idx];
		for (lane_idx = 0; lane_idx < lnum; lane_idx++) {
			octeontx2_fill_cgx_struct(cgx_idx, qlm_idx, gserx,
						shift_from_first, lane_idx);
		}
	}
	octeontx2_cgx_check_linux(fdt);
	octeontx2_cgx_assign_mac(fdt);
	octeontx2_map_cgx_to_nix();
}

static void octeontx2_fill_qlm_details(const void *fdt)
{
	int qlm, lane, polarity, max_lanes, voltage, rx_ad, refset;
	char prop[64];
	int generic_clk_term, clk_term;

	snprintf(prop, sizeof(prop), "QLM-CLK-TERM");
	generic_clk_term = octeontx2_fdtbdk_get_num(fdt, prop, 10);

	for (qlm = 0; qlm < plat_octeontx_scfg->gser_count; qlm++) {
		snprintf(prop, sizeof(prop), "QLM-CLK-TERM.N0.QLM%d", qlm);
		clk_term = octeontx2_fdtbdk_get_num(fdt, prop, 10);
		if (clk_term == -1)
			clk_term = generic_clk_term;
		plat_octeontx_bcfg->qlm_cfg[qlm].clk_term = clk_term;

		max_lanes = plat_octeontx_scfg->qlm_max_lane_num[qlm];
		for (lane = 0; lane < max_lanes; lane++) {
			snprintf(prop, sizeof(prop),
				"QLM-RX-ADAPTATION.N0.QLM%d.LANE%d", qlm, lane);
			rx_ad = octeontx2_fdtbdk_get_num(fdt, prop, 10);
			if (rx_ad == -1)
				rx_ad = QLM_DEFAULT_RX_ADAPTATION;
			plat_octeontx_bcfg->qlm_cfg[qlm].rx_adaptation[lane]
				= rx_ad;

			snprintf(prop, sizeof(prop),
				"QLM-LANE-TX-POLARITY.N0.QLM%d.LANE%d",
				qlm, lane);
			polarity = octeontx2_fdtbdk_get_num(fdt, prop, 10);
			if (polarity == -1)
				polarity = 0; /* Default value */
			plat_octeontx_bcfg->qlm_cfg[qlm].lane_tx_polarity[lane]
				= polarity;

			snprintf(prop, sizeof(prop),
				"QLM-LANE-RX-POLARITY.N0.QLM%d.LANE%d",
				qlm, lane);
			polarity = octeontx2_fdtbdk_get_num(fdt, prop, 10);
			if (polarity == -1)
				polarity = 0; /* Default value */
			plat_octeontx_bcfg->qlm_cfg[qlm].lane_rx_polarity[lane]
				= polarity;

			snprintf(prop, sizeof(prop),
				"QLM-LANE-RX-PREVGA-GN-ADAPT.N0.QLM%d.LANE%d",
				qlm, lane);
			/* Write directly because -1 is default value */
			plat_octeontx_bcfg->qlm_cfg[qlm]
				.lane_rx_prevga_gn_adapt[lane] =
					octeontx2_fdtbdk_get_num(fdt, prop, 10);

			snprintf(prop, sizeof(prop),
				"QLM-LANE-RX-PREVGA-GN-OVRD.N0.QLM%d.LANE%d",
				qlm, lane);
			/* Write directly because -1 is default value */
			plat_octeontx_bcfg->qlm_cfg[qlm]
				.lane_rx_prevga_gn_ovrd[lane] =
					octeontx2_fdtbdk_get_num(fdt, prop, 10);

			snprintf(prop, sizeof(prop),
				"QLM-LANE-IDLE-REFSET-VALUE.N0.QLM%d.LANE%d",
				qlm, lane);
			refset = octeontx2_fdtbdk_get_num(fdt, prop, 10);
			if (refset == -1)
				refset = 3; /* Default value */
			plat_octeontx_bcfg->qlm_cfg[qlm].lane_idle_refset[lane]
				= refset;

			snprintf(prop, sizeof(prop),
				"QLM-LINK-TRAINING-INITIAL-STATE.N0.QLM%d.LANE%d",
				qlm, lane);
			refset = octeontx2_fdtbdk_get_num(fdt, prop, 10);
			if (refset == -1) {
				snprintf(prop, sizeof(prop),
					 "QLM-LINK-TRAINING-INITIAL-STATE.N0.QLM%d",
					 qlm);
				refset = octeontx2_fdtbdk_get_num(fdt, prop, 10);
				if (refset == -1) {
					snprintf(prop, sizeof(prop),
						 "QLM-LINK-TRAINING-INITIAL-STATE.N0");
					refset = octeontx2_fdtbdk_get_num(fdt, prop, 10);
					if (refset == -1) {
						snprintf(prop, sizeof(prop),
							 "QLM-LINK-TRAINING-INITIAL-STATE");
						refset = octeontx2_fdtbdk_get_num(fdt, prop, 10);
						if (refset == -1)
							refset = -1; /* Default value */
					}
				}
			}
			plat_octeontx_bcfg->qlm_cfg[qlm].lt_init_state[lane]
				= refset;
		}
	}

	voltage = octeontx2_fdtbdk_get_num(fdt, "QLM-VOLTAGE.N0", 10);
	if (voltage == -1)
		voltage = QLM_DEFAULT_VOLTAGE;

	plat_octeontx_bcfg->qlm_voltage = voltage;
}

static void octeontx2_fill_twsi_slave_details(const void *fdt)
{
	int twssl_bus, twssl_addr;

	twssl_bus = octeontx2_fdtbdk_get_num(
		fdt, "SCP-TWSI-SLAVE-BUS.N0", 10);

	plat_octeontx_bcfg->bcfg.slave_twsi.s.bus = twssl_bus;

	twssl_addr = octeontx2_fdtbdk_get_num(
		fdt, "SCP-TWSI-SLAVE-ADDR.N0", 16);
	if (twssl_addr == -1)
		twssl_addr = 0x77;

	plat_octeontx_bcfg->bcfg.slave_twsi.s.addr = twssl_addr;
}

/*
 * Parse the device tree RAS error rings (GHES/BERT).
 *
 * on entry,
 *   fdt            : pointer to FDT
 *   offset         : starting offset to parse
 *   fdt_ghes       : array of structures to fill
 *   num_entries    : count of structures to fill
 *   fdt_size_cells : # of size-cells used by node
 *                    (this accommodates different 'size-cells' in DT)
 */
static int parse_fdt_ras(const void *fdt, int offset,
			 struct fdt_ghes *fdt_ghes, int num_entries,
			 size_t fdt_size_cells)
{
	const fdt32_t *frange;
	struct range {
		uint64_t rel;
		uint64_t abs;
		uint32_t size;
	} ranges[GHES_PTRS];
	int child, i, cnt;

	/* parse address translation window */
	frange = fdt_getprop(fdt, offset, "ranges", NULL);
	for (i = 0; i < GHES_PTRS; i++) {
		struct range *r;
		const fdt32_t *foff;
		size_t range_prop_len;

		/* ranges uses 2 addr fields, assume they are 64-bit
		 * (.e. address-cells == 2)
		 */
		range_prop_len = sizeof(uint64_t) * 2;
		range_prop_len += sizeof(uint32_t) * fdt_size_cells;
		/* len is in units of uint32_t */
		range_prop_len /= sizeof(uint32_t);

		r = &ranges[i];
		foff = frange + (range_prop_len * i);

		r->rel = fdt32_to_cpu(foff[0]);
		r->rel <<= 32;
		r->rel |= fdt32_to_cpu(foff[1]);
		r->abs = fdt32_to_cpu(foff[2]);
		r->abs <<= 32;
		r->abs |= fdt32_to_cpu(foff[3]);
		r->size = fdt32_to_cpu(foff[3 + fdt_size_cells]);
		debug_dts("%s r%d %llx %llx %x\n",
			__func__, i, r->rel, r->abs, r->size);
	}

	/* parse each subnode */
	cnt = 0;
	fdt_for_each_subnode(child, fdt, offset) {
		const fdt32_t *freg, *fid;
		struct fdt_ghes *g;
		const char *name;

		g = fdt_ghes + cnt;

		name = fdt_get_name(fdt, child, NULL);
		freg = fdt_getprop(fdt, child, "reg", NULL);
		fid = fdt_getprop(fdt, child, "event-id", NULL);

		/* Only validate params present in ALL nodes */
		if (!freg || !name)
			continue;

		strlcpy(g->name, name, sizeof(g->name));
		if (fid)
			g->id = fdt32_to_cpu(fid[0]);

		for (i = 0; i < GHES_PTRS; i++) {
			struct range *r;
			const fdt32_t *foff;
			size_t reg_prop_len;
			uint64_t base;

			reg_prop_len = sizeof(uint64_t);
			reg_prop_len += sizeof(uint32_t) * fdt_size_cells;
			/* len is in units of uint32_t */
			reg_prop_len /= sizeof(uint32_t);

			r = &ranges[i];
			foff = freg + (reg_prop_len * i);

			base = fdt32_to_cpu(foff[0]);
			base <<= 32;
			base |= fdt32_to_cpu(foff[1]);
			g->size[i] = fdt32_to_cpu(foff[1 + fdt_size_cells]);

			/* check against parent range */
			if (base + g->size[i] > r->rel + r->size ||
						    base < r->rel) {
				printf("%s(%s) r%d %x@%llx outside %x@%llx\n",
					__func__, g->name, i,
					g->size[i], base,
					r->size, r->rel);
				g->base[i] = NULL;
				g->size[i] = 0;
			} else {
				base += r->abs - r->rel;
				debug_dts("%s (%s) id:%x %x@%llx\n", __func__,
					g->name, g->id, g->size[i], base);
				g->base[i] = (void *)base;
			}
		}

		if (++cnt >= num_entries)
			break;
	}

	return cnt;
}

/*
 * octeontx2_fill_ras_details()
 *
 * Populate the RAS error ring structures from the Device Tree.
 */
static void octeontx2_fill_ras_details(const void *fdt)
{
	ras_config_t *c = &plat_octeontx_bcfg->ras_config;
	int offset, cnt;

	offset = fdt_path_offset(fdt, "/soc@0/sdei-ghes");
	debug_dts("%s soc@0/sdei-ghes %d\n", __func__, offset);
	offset = fdt_node_offset_by_compatible(fdt, 0,
					"marvell,sdei-ghes");
	debug_dts("%s sdei-ghes %d\n", __func__, offset);
	cnt = parse_fdt_ras(fdt, offset, c->fdt_ghes, ARRAY_SIZE(c->fdt_ghes),
			    sizeof(uint32_t) / sizeof(uint32_t));
	c->nr_ghes = cnt;

	offset = fdt_path_offset(fdt, "/soc@0/bed-bert");
	debug_dts("%s soc@0/bed-bert %d\n", __func__, offset);
	offset = fdt_node_offset_by_compatible(fdt, 0,
					"marvell,bed-bert");
	debug_dts("%s bed-bert %d\n", __func__, offset);
	parse_fdt_ras(fdt, offset, &c->fdt_bert, 1,
		      sizeof(uint64_t) / sizeof(uint32_t));
}

static void octeontx2_fill_timer_ms(const void *fdt)
{
	int offset, val = 0;
	const uint32_t *reg;

	/* By default, update timer to 1s */
	plat_octeontx_bcfg->timer1_ms = 1000;
	plat_octeontx_bcfg->timer2_ms = 1000;

	if (fdt_check_header(fdt))
		return;

	offset = fdt_path_offset(fdt, "/cgx_poll_timer");
	if (offset > 0) {
		reg = (fdt_getprop(fdt, offset, "cmd_timer",
				NULL));
		if (reg) {
			val = fdt32_to_cpu(*reg);
			/* If the timer frequency is less than 200ms
			 * default to 1s.
			 */
			if (val >= 200)
				plat_octeontx_bcfg->timer1_ms = val;
		}

		reg = (fdt_getprop(fdt, offset, "link_mgmt_timer",
				NULL));
		if (reg) {
			val = fdt32_to_cpu(*reg);
			/* If the timer frequency is less than 200ms
			 * default to 1s.
			 */
			if (val >= 200)
				plat_octeontx_bcfg->timer2_ms = val;
		}
	} else
		debug_dts("%s: Not able to find cgx_poll_timer node, using 1sec as default\n", __func__);
}

int plat_octeontx_fill_board_details(void)
{
	void *fdt = fdt_ptr;
	int offset, rc;

	rc = fdt_check_header(fdt);
	if (rc) {
		WARN("Invalid device tree\n");
		return rc;
	}

	offset = fdt_path_offset(fdt, "/cavium,bdk");
	if (offset < 0) {
		WARN("FDT node not found\n");
		return offset;
	}

	rc = octeontx2_parse_boot_device(fdt, offset);
	if (rc) {
		debug_dts("Using GPIO_STRAPX register for boot device\n");
		octeontx2_boot_device_from_strapx();
	}

	octeontx2_fill_cgx_details(fdt);
	octeontx2_fill_qlm_details(fdt);
	octeontx2_fill_ras_details(fdt);

	octeontx2_fill_twsi_slave_details(fdt);

	octeontx2_fill_timer_ms(fdt);

	return 0;
}
