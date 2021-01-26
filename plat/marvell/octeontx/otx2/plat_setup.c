/*
 * Copyright (C) 2018 Marvell International Ltd.
 * This program is provided "as is" without any warranty of any kind,
 * and is distributed under the applicable Marvell proprietary limited use
 * license agreement.
 */

#include <arch.h>
#include <stdio.h>
#include <debug.h>
#include <string.h>
#include <assert.h>
#include <platform_def.h>
#include <octeontx_common.h>
#include <cgx_intf.h>
#include <cgx.h>
#include <plat/common/platform.h>
#include <plat_otx2_configuration.h>
#include <plat_pwrc.h>
#include <octeontx_legacy_pwrc.h>
#include <gpio_octeontx.h>
#include <plat_board_cfg.h>
#include <plat_flr.h>
#include <plat_octeontx.h>
#include <octeontx_utils.h>
#include <octeontx_security.h>
#include <sh_fwdata.h>
#include <octeontx_dram.h>
#include <libfdt.h>
#include <plat_scfg.h>
#include <platform_irqs_def.h>

#if RAS_EXTENSION
#include <plat_ras.h>
#endif

#include "cavm-csrs-fusf.h"
#include "cavm-csrs-gpio.h"

/* Each of these can be overridden by the platform - this is uncommon */
#pragma weak plat_octeontx_get_eth_count
#pragma weak plat_octeontx_get_eth_lmac_count
#pragma weak plat_octeontx_get_eth_lmac_rvu_info
#pragma weak plat_octeontx_enable_eth_lmac
#pragma weak plat_octeontx_is_enabled_eth_lmac
#pragma weak plat_octeontx_is_eth_lmac_rfoe

static int disable_ooo;
static uint64_t disable_ooo_mask;
static uint64_t enable_wfe_mask;

extern void plat_armtrace_init(void);
/* Any SoC family specific setup
 * to be done in BL31 can be initialized
 * in this API. If there are any platform
 * specific init to be done but not common
 * to family, each platform can define
 * its own API and can be called from here
 */
void plat_octeontx_setup(void)
{
	sh_fwdata_init();

	/* Initialize CGX framework */
	cgx_fw_intf_init();

	/* setup gpio interrupt handling */
	plat_gpio_irq_setup();

	/* Workaround for FLR handling on CN9xxx */
	plat_flr_init();

#if RAS_EXTENSION
	if (plat_ras_feature_supported() == true) {
		otx2_ras_init();
		plat_dram_ras_init();
	}
#endif /* RAS_EXTENSION */


	/* Configure PEM0 (EP) streams to use secure world access.
	 * PEM0 streams must be secure to support host remote utils' memory
	 * access while running OcteonTX Linux. Otherwise, any remote access
	 * will be aborted (and an event logged) due to OcteonTX Linux SMMU
	 * initialization.
	 */
	octeontx_configure_pem_ep_security(0 /* PEM0 */, 1 /* secure */);

	/* otx2 trace init */
	plat_armtrace_init();
}

unsigned int is_pem_in_ep_mode(int pem)
{
	cavm_pemx_cfg_t pemx_cfg;
	cavm_pemx_on_t pemx_on;

	/* Check pemon and hostmd bits of PEM for EP mode */
	pemx_on.u = CSR_READ(CAVM_PEMX_ON(pem));
	pemx_cfg.u = CSR_READ(CAVM_PEMX_CFG(pem));
	if (pemx_on.cn9.pemon && !pemx_cfg.cn9.hostmd)
		return 1;

	return 0;
}

/*
 * Program REVID for SDP RVU PF
 */
unsigned int plat_configure_sdp_rid(void)
{
	uint64_t midr;
	static int instance;

	midr = read_midr();

	/*
	 * 98xx has 2xSDPs so mark first instance of SDP block revid as 0 so
	 * that it will be SDP0 and second instance of SDP block revid as 1 so
	 * that it will be SDP1
	 */
	if (IS_OCTEONTX_PN(midr, T98PARTNUM)) {
		if (instance)
			return 1;

		instance = 1;
		return 0;

	} else if (IS_OCTEONTX_PN(midr, T96PARTNUM) ||
		   IS_OCTEONTX_PN(midr, LOKIPARTNUM)) {
		return 0;
	}

	return 0;
}

/*
 * Program REVID for PCIe CPT device.
 */
unsigned int plat_configure_cpt_rid(void)
{
	unsigned int val = 0;
	uint64_t midr;

	midr = read_midr();

	/* program CPT revision id */
	if (IS_OCTEONTX_PN(midr, T98PARTNUM))
		val = 3;
	else if (IS_OCTEONTX_PN(midr, T96PARTNUM)) {
		if (IS_OCTEONTX_VAR(midr, T96PARTNUM, 1))
			val = 1;
		else
			val = 2;
	}
	return val;
}

/*
 * Program REVID for PCIe devices.
 * Bits 0..1: minor pass
 * Bits 3..2: major pass
 * Bits 7..4: midr id, 0:96, 1:95, 2:loki, 3:98, 4:f95mm f:unknown
 */
unsigned int plat_configure_rid(void)
{
	unsigned int val;
	uint8_t midr_id;
	uint64_t midr;

	val = 0;
	midr = read_midr();

	switch (MIDR_PARTNUM(midr)) {
	case T96PARTNUM:
		midr_id = 0;
		break;

	case F95PARTNUM:
		midr_id = 1;
		break;

	case LOKIPARTNUM:
		midr_id = 2;
		break;

	case T98PARTNUM:
		midr_id = 3;
		break;

	case F95MMPARTNUM:
		midr_id = 4;
		break;

	default:
		midr_id = 0xf;
		WARN("Unknown partnum 0x%llx, set midr id in REVID to 0xf\n",
			MIDR_PARTNUM(midr));
	}

	val = midr_id << 4;
	/* program minor pass */
	val |= MIDR_REVISION(midr) & 0x3;

	/* program major pass */
	val |= (MIDR_VARIANT(midr) & 0x3) << 2;

	return val;
}

extern void *scmi_handle;

void plat_pwrc_setup(void)
{
	int rc;

#ifdef SCMI_WITH_LEGACY_PM
	/*
	 * Initialize SCMI for custom Cavium configuration protocol.
	 * Initialize legacy pwrc for PSCI
	 */
	rc = octeontx_pwrc_setup();
	if (rc)
		WARN("SCMI initialize falied with %d\n", rc);
	octeontx_legacy_pwrc_setup();
#else
	/*
	 * Try to initialize SCMI, in case of error,
	 * fallback to legacy PM driver
	 */
	rc = octeontx_pwrc_setup();
	if (rc) {
		octeontx_legacy_pwrc_setup();
	}
#endif
}

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			 const plat_psci_ops_t **psci_ops)
{
#ifdef SCMI_WITH_LEGACY_PM
	/*
	 * Always use legacy PSCI ops
	 */
	return octeontx_legacy_setup_psci_ops(sec_entrypoint, psci_ops);
#else
	if (scmi_handle == NULL) {
		return octeontx_legacy_setup_psci_ops(sec_entrypoint, psci_ops);
	} else {
		return octeontx_setup_psci_ops(sec_entrypoint, psci_ops);
	}
#endif
}

/*
 * Get ROM_T_CNT value from the FUSF_RCMD
 *
 * Return: Value in 0-32 range
 */
unsigned int plat_get_rom_t_cnt()
{
	cavm_fusf_rcmd_t read_cmd;
	uint64_t dat;
	uint32_t nv_count_val = 0;
	unsigned int ret = 0;

	read_cmd.u = 0;
	/* In CN9XXX fuses take a 128 bit bank, not a byte address.
	 * ROM_T_CNT is at bank 0 */
	read_cmd.cn9.addr = CAVM_FUSF_FUSE_NUM_E_ROM_T_CNTX(0) >> 7;
	read_cmd.s.pend = 1;
	CSR_WRITE(CAVM_FUSF_RCMD, read_cmd.u);
	do {
		read_cmd.u = CSR_READ(CAVM_FUSF_RCMD);
	} while (read_cmd.s.pend);

	/* ASIM returns 0 on FUSF_RCMD accesses */
	if (!strncmp(plat_octeontx_bcfg->bcfg.board_model, "asim-", 5))
		dat = CSR_READ(CAVM_FUSF_CTL);
	else
		dat = CSR_READ(CAVM_FUSF_BNK_DATX(0));

	/*
	 * FUSF_BNK_DATX contains all 128 fuses
	 * in the bank associated with FUSF_RCMD[ADDR].
	 * ROM_T_CNT is stored on FUSF_BNK_DATX(0)[63:32]
	 */
	nv_count_val = octeontx_bit_extract(dat, CAVM_FUSF_FUSE_NUM_E_ROM_T_CNTX(0), 32);

	/* Convert value from rom_t_cnt to unsigned int */
	if (nv_count_val)
		ret = 32 - __builtin_clz(nv_count_val);

	return ret;
}

/*
 * Return alternative pkg information
 *
 * @return non-zero if an alternative package
 *     0 = Normal package
 *     1 = Alternative package 1 (CN93XXC)
 *     3 = Alternative package 3 (CN95XXE)
 *     5 = Alternative package 5 (CN93XXN)
 */
int plat_get_altpkg(void)
{
	union cavm_gpio_pkg_ver pkg_ver;

	pkg_ver.u = CSR_READ(CAVM_GPIO_PKG_VER);
	return pkg_ver.s.pkg_ver;
}

/*
 * Return to enable/disable OOO mask
 *
 * @return cpu core mask on disable OOO
 */
uint64_t plat_get_ooo_mask_status(void)
{
	return disable_ooo_mask;
}

/*
 * Return to enable/disable OOO
 *
 * @return non-zero to disable OOO
 */
int plat_get_ooo_status(void)
{
	if (disable_ooo)
		return 1;
	else
		return 0;
}

/*
 * Return to enable/disable WFE mask
 *
 * @return cpu core mask on enabel/disable WFE
 */
uint64_t plat_get_wfe_status(void)
{
	return enable_wfe_mask;
}

void plat_octeontx_cpu_setup(void)
{
	uint64_t cvmctl_el1, cvmmemctl0_el1, cvmmemctl1_el1, cvmmemctl2_el1, cvmmemctl3_el1;
	uint64_t cvmctl2_el1, midr;
	int core = plat_my_core_pos();
	uint64_t o_mask = plat_get_ooo_mask_status();

	cvmctl_el1 = read_cvmctl_el1();
	cvmctl2_el1 = read_cvmctl2_el1();
	cvmmemctl0_el1 = read_cvmmemctl0_el1();
	cvmmemctl1_el1 = read_cvmmemctl1_el1();
	cvmmemctl2_el1 = read_cvmmemctl2_el1();
	cvmmemctl3_el1 = read_cvmmemctl3_el1();
	midr = read_midr();

	/* Enable CAS/CASP and v8.1 support */
	unset_bit(cvmctl_el1, 36);  /* Enable CAS */
	unset_bit(cvmctl_el1, 37);  /* Enable CASP */

	/* Enable prefetcher */
	set_bit(cvmctl_el1, 43);   /* Ignore the bp for next line prefetcher. */
	set_bit(cvmctl_el1, 42);   /* Use stride of 2. */
	set_bit(cvmctl_el1, 41);   /* Enable next line prefetcher. */
	set_bit(cvmctl_el1, 40);   /* Enable delta prefetcher. */

	/* Errata AP-36579 */
	if (IS_OCTEONTX_PASS(midr, T96PARTNUM, 1, 0)
	    || IS_OCTEONTX_VAR(midr, F95PARTNUM, 1)) {
		/* cvmctl2_el1[3:2] = REDUCE_MAP_BANDWIDTH */
		set_bit(cvmctl2_el1, 2);
		set_bit(cvmctl2_el1, 3);
	}

	/* Errata AP-36933, cvmctl_el1[54] = DISABLE_LDP_STP_FISS */
	set_bit(cvmctl_el1, 54);

	/*
	 * Disable v8.5 store barrier for better performance on all models.
	 * cvmctl_el1[61] = DISABLE_STORE_BARRIER_FUNC
	 */
	set_bit(cvmctl_el1, 61);

	/* Errata AP-38511 : Disable/Enable WFE */
	if (plat_get_wfe_status() & (1UL << core))
		unset_bit(cvmctl_el1, 34);
	else
		set_bit(cvmctl_el1, 34);

	set_bit(cvmmemctl1_el1, 3); /* Enable LMTST */
	set_bit(cvmmemctl1_el1, 4); /* Enable SSO/PKO addr region */
	set_bit(cvmmemctl1_el1, 5); /* Trap any accesses to nonzero node id */
	set_bit(cvmmemctl1_el1, 6); /* Enable SSO switch tag */

	if (IS_OCTEONTX_PN(midr, F95PARTNUM)
	    || IS_OCTEONTX_PN(midr, LOKIPARTNUM)
	    || IS_OCTEONTX_PN(midr, F95MMPARTNUM))
		set_bit(cvmmemctl1_el1, 58); /* Enable 128-bit access to BPHY */

	/*
	 * To improve performance memory-unit for EL1 should be configured in
	 * different way than default.
	 */
	cvmmemctl2_el1 = octeontx_bit_insert(
			cvmmemctl2_el1, MTLB0_BLOCK_VALUE,
			MTLB0_BLOCK_SHIFT, MTLB0_BLOCK_WIDTH);
	cvmmemctl2_el1 = octeontx_bit_insert(
			cvmmemctl2_el1, TLBI_BLOCK_VALUE,
			TLBI_BLOCK_SHIFT, TLBI_BLOCK_WIDTH);

	/*
	 * Errata AP-38589: Disable ordering collision optimizations
	 */
	set_bit(cvmmemctl3_el1, 12);

	/*
	 * Increase livelock stall for retiring instructions
	 */
	cvmctl2_el1 = octeontx_bit_insert(cvmctl2_el1, LIVELOCK_STALL_VALUE,
			LIVELOCK_STALL_SHIFT, LIVELOCK_STALL_WIDTH);
	/*
	 * Set cvm_ctl_el1[5] to workaround debug state execution in
	 * incorrect EL and also to workaround livelock found in the wild
	 */
	set_bit(cvmctl_el1, 4); /* Force execution-unit clock */
	set_bit(cvmctl_el1, 5); /* Force issue-unit clock */

	/*
	 * Fix up defaults from the BDK which is broken and
	 * violates the ARM ARM.
	 */

	/* Don't reset timer on merge as that violates the ARM ARM. */
	unset_bit(cvmmemctl0_el1, 17);
	/* Set Write-buffer timeout for NSH entries to 218 cycles. */
	unset_bit(cvmmemctl0_el1, 18);

	/* Disable/enable OOO based on coremask */
	if (o_mask & (1UL << core))
		set_bit(cvmctl_el1, 44);
	else
		unset_bit(cvmctl_el1, 44);

	/* Disable all cores if disable_ooo is set */
	if (plat_get_ooo_status() && !o_mask)
		set_bit(cvmctl_el1, 44);

	write_cvmctl_el1(cvmctl_el1);
	write_cvmctl2_el1(cvmctl2_el1);
	write_cvmmemctl0_el1(cvmmemctl0_el1);
	write_cvmmemctl1_el1(cvmmemctl1_el1);
	write_cvmmemctl2_el1(cvmmemctl2_el1);
	write_cvmmemctl3_el1(cvmmemctl3_el1);

	/* Allow CVM CACHE instructions from EL1/EL2 */
	write_cvm_access_el1(read_cvm_access_el1() & ~(1 << 8));
	write_cvm_access_el2(read_cvm_access_el2() & ~(1 << 8));
	write_cvm_access_el3(read_cvm_access_el3() & ~(1 << 8));
}

int octeontx2_configure_ooo_mask(uint64_t x1)
{
	uint64_t cvmctl_el1;

	disable_ooo_mask = x1;

	cvmctl_el1 = read_cvmctl_el1();
	if (disable_ooo_mask & (1UL << plat_my_core_pos()))
		set_bit(cvmctl_el1, 44);
	else if (disable_ooo && !disable_ooo_mask)
		set_bit(cvmctl_el1, 44);
	else
		unset_bit(cvmctl_el1, 44);

	write_cvmctl_el1(cvmctl_el1);

	return 0;
}

int octeontx2_configure_ooo(unsigned int x1)
{
	uint64_t cvmctl_el1;

	disable_ooo = x1;

	cvmctl_el1 = read_cvmctl_el1();
	if (disable_ooo && !disable_ooo_mask)
		set_bit(cvmctl_el1, 44);
	else
		unset_bit(cvmctl_el1, 44);

	write_cvmctl_el1(cvmctl_el1);

	return 0;
}

int octeontx2_configure_wfe(uint64_t x1)
{
	uint64_t cvmctl_el1;

	enable_wfe_mask = x1;

	cvmctl_el1 = read_cvmctl_el1();
	if (enable_wfe_mask & (1UL << plat_my_core_pos()))
		unset_bit(cvmctl_el1, 34);
	else
		set_bit(cvmctl_el1, 34);

	write_cvmctl_el1(cvmctl_el1);

	return 0;
}

/*
 * plat_initialize_boot_error_data_area()
 *
 * The Boot Error Data area shares the preserved, non-secure region
 * with the RAMOOPS data (Linux persistent RAM).
 * The device tree has been configured to assign entire
 * NSECURE_PRESERVE memory to RAMOOPS.
 * Adjust the DT settings to account for Boot Error Data area;
 * also add mapping region for Boot Error Data area.
 *
 * on entry,
 *   attr:  attributes to be used for Boot Error Data area mapping
 *
 * returns,
 *   void
 */
void plat_initialize_boot_error_data_area(unsigned long attr)
{
#ifdef IMAGE_BL2
	uint64_t base, size, oops_base, oops_size, bed_base, bed_size;
	int bed_off, oops_off, bed_dev_off, fail;
	const void *fdt = fdt_ptr;
	const fdt64_t *freg;
	fdt64_t dt_regs[9];
	char bed_name[31];

	fail = 1;

	size = memory_region_get_info(NSECURE_PRESERVE, &base);

	if (!base || !size) {
		fail = 0;
		VERBOSE("Cannot locate non-secure preserved area.\n");
		goto exit;
	}

	bed_off = fdt_path_offset(fdt,
				  "/reserved-memory/ghes-bert");
	if (bed_off == -1) {
		VERBOSE("Missing BERT area from DT\n");
		return;
	}
	oops_off = fdt_path_offset(fdt,
				   "/reserved-memory/ramoops");
	if (oops_off == -1) {
		VERBOSE("Missing RAMMOOPS from DT\n");
		return;
	}
	bed_dev_off = fdt_path_offset(fdt,
				   "/soc@0/bed-bert");
	if (bed_dev_off == -1) {
		VERBOSE("Missing BERT area Device Driver from DT\n");
		return;
	}

	/* Retrieve Boot Error Data area DT settings */
	bed_base = bed_size = 0;
	freg = fdt_getprop(fdt, bed_off, "reg", NULL);
	if (freg) {
		bed_base = fdt64p_to_cpu(&freg[0]);
		bed_size = fdt64p_to_cpu(&freg[1]);
	}
	if (!bed_size || (bed_size > size)) {
		WARN("Unexpected BERT area DT size 0x%lx\n",
		     (long)bed_size);
		goto exit;
	}

	/* Retrieve RAMOOPS DT settings */
	oops_base = oops_size = 0;
	freg = fdt_getprop(fdt, oops_off, "reg", NULL);
	if (freg) {
		oops_base = fdt64p_to_cpu(&freg[0]);
		oops_size = fdt64p_to_cpu(&freg[1]);
	}

	/*
	 * Prior to our adjustment, we expect RAMOOPS in DT
	 * to equal NSECURE_PRESERVE area.  If not, issue warning.
	 * Otherwise, adjust size of RAMOOPS to make room for
	 * Boot Error Data area.
	 */
	if ((oops_base != base) || (oops_size != size)) {
		WARN("Unexpected RAMOOPS DT setting\n");
		WARN("0x%lx/0x%lx vs 0x%lx/0x%lx\n",
		     (long)oops_base, (long)oops_size,
		     (long)base, (long)size);
		goto exit;
	}

	/* adjust RAMOOPS size */
	oops_size -= bed_size;
	/* set Boot Error Data Base */
	bed_base = oops_base + oops_size;

	/* adjust RAMOOPS DT setting */
	dt_regs[0] = cpu_to_fdt64(oops_base);
	dt_regs[1] = cpu_to_fdt64(oops_size);
	if (fdt_setprop((void *)fdt, oops_off, "reg", dt_regs,
			sizeof(dt_regs[0]) * 2)) {
		ERROR("Unable to set adjusted RAMOOPS DT size to 0x%lx\n",
		      (long)oops_size);
		goto exit;
	}

	/* set Boot Data Area DT values */
	dt_regs[0] = cpu_to_fdt64(bed_base);
	dt_regs[1] = cpu_to_fdt64(bed_size);
	if (fdt_setprop((void *)fdt, bed_off, "reg", dt_regs,
			sizeof(dt_regs[0]) * 2)) {
		ERROR("Unable to set DT BERT area 0x%lx/0x%lx\n",
		      (long)bed_base, (long)bed_size);
		goto exit;
	}

	snprintf(bed_name, sizeof(bed_name), "ghes-bert@%016lx",
		 (long)bed_base);
	if (fdt_set_name((void *)fdt, bed_off, bed_name))
		INFO("Unable to set ghes-bert DT node name %s\n", bed_name);

	/* The 'fdt_set_name' may have changed this offset; retrieve it again */
	bed_dev_off = fdt_path_offset(fdt, "/soc@0/bed-bert");

	/* set device driver DT 'ranges' values */
	dt_regs[0] = 0;
	dt_regs[1] = cpu_to_fdt64(bed_base);
	dt_regs[2] = cpu_to_fdt64(bed_size);
	/* set all three ranges to same value */
	dt_regs[3] = dt_regs[6] = dt_regs[0];
	dt_regs[4] = dt_regs[7] = dt_regs[1];
	dt_regs[5] = dt_regs[8] = dt_regs[2];
	if (fdt_setprop((void *)fdt, bed_dev_off, "ranges", dt_regs,
			sizeof(dt_regs[0]) * 9)) {
		ERROR("Unable to set BERT device driver DT ranges\n");
		goto exit;
	}

	/* Add mapping region for Boot Error Data area */
	VERBOSE("Setting BERT area at 0x%lx (0x%lx B)\n",
		(long)bed_base, (long)bed_size);
	mmap_add_region(base, bed_base, bed_size, attr);
	plat_octeontx_bcfg->bert_area.base = bed_base;
	plat_octeontx_bcfg->bert_area.size = bed_size;

	fail = 0;

exit:
	if (fail)
		ERROR("BERT area not available\n");
#endif
}

/*
 * plat_initialize_ghes_hest_area()
 *
 * The Generic Hardware Error Source (GHES) and
 * Hardware Error Source Table (HEST) areas must reside in
 * reserved memory accessible to both firmware and Linux
 * (i.e. non-secure, non-preserved region).
 * In order to prevent fragmentation, these areas are
 * reserved at the top (highest address) of this region.
 *
 * The device tree contains placeholders for these areas
 * (GHES/HEST); here the DT entries are adjusted to use
 * the correct reserved addresses at the top of the region.
 *
 * on entry,
 *   void
 *
 * returns,
 *   void
 */
void plat_initialize_ghes_hest_area(void)
{
#ifdef IMAGE_BL2
	const char *sdei_ghes_mem_name = "/reserved-memory/ghes-hest";
	uint64_t ghes_base, ghes_size, ghes_range_base;
	const char *sdei_ghes_dev_name = "/soc@0/sdei-ghes";
	int ghes_off, ghes_dev_off, fail, idx;
	const void *fdt = fdt_ptr;
	const fdt64_t *freg64;
	const fdt32_t *freg32;
	char ghes_name[31];
	int freg_len;
	struct {
		/* NOTE: these are in FDT format, not CPU format */
		fdt64_t addr; /* DT address-cells = 2 */
		fdt64_t size; /* DT size-cells = 2 */
	} dt_regs;
	struct {
		/* NOTE: these are in FDT format, not CPU format */
		fdt64_t child_addr;  /* child DT address-cells = 2 */
		fdt64_t parent_addr; /* parent DT address-cells = 2 */
		fdt32_t child_size;  /* child DT size-cells = 1 */
	} __packed ghes_ranges[3];

	fail = 1;

	ghes_off = fdt_path_offset(fdt, sdei_ghes_mem_name);
	if (ghes_off == -1) {
		VERBOSE("Missing GHES area from DT\n");
		return;
	}
	ghes_dev_off = fdt_path_offset(fdt, sdei_ghes_dev_name);
	if (ghes_dev_off == -1) {
		VERBOSE("Missing GHES area Device Driver from DT\n");
		return;
	}

	/* Retrieve GHES area DT settings */
	ghes_base = ghes_size = 0;
	freg64 = fdt_getprop(fdt, ghes_off, "reg", &freg_len);
	if (freg64 && (freg_len >= (sizeof(*freg64) * 2))) {
		ghes_base = fdt64p_to_cpu(&freg64[0]);
		ghes_size = fdt64p_to_cpu(&freg64[1]);
	}

	if (ghes_base != 0) {
		VERBOSE("Using fixed GHES addresses @ 0x%llx\n",
			(long long)ghes_base);
		return;
	}

	if (!ghes_size || (ghes_size & PAGE_SIZE_MASK)) {
		WARN("Invalid size 0x%lx for %s\n", (long)ghes_size,
		     sdei_ghes_mem_name);
		goto exit;
	}

	ghes_base = octeontx_dram_reserve(ghes_size, NSECURE_NONPRESERVE);

	/* set GHES DT values */
	dt_regs.addr = cpu_to_fdt64(ghes_base);
	dt_regs.size = cpu_to_fdt64(ghes_size);
	if (fdt_setprop((void *)fdt, ghes_off, "reg", &dt_regs,
			sizeof(dt_regs))) {
		WARN("Unable to set DT GHES area 0x%lx/0x%lx\n",
		     (long)ghes_base, (long)ghes_size);
		goto exit;
	}

	snprintf(ghes_name, sizeof(ghes_name), "ghes-hest@%016lx",
		 (long)ghes_base);
	INFO("Set DT GHES area (%s) 0x%lx/0x%lx\n", ghes_name,
	     (long)ghes_base, (long)ghes_size);
	if (fdt_set_name((void *)fdt, ghes_off, ghes_name))
		INFO("Unable to set ghes-hest DT node name %s\n", ghes_name);

	/* The 'fdt_set_name' may have changed this offset; retrieve it again */
	ghes_dev_off = fdt_path_offset(fdt, sdei_ghes_dev_name);

	/* adjust device driver DT 'ranges' values with GHES base address */
	freg64 = fdt_getprop(fdt, ghes_dev_off, "ranges", &freg_len);
	/* We expect 3 ranges */
	if (freg_len != sizeof(ghes_ranges)) {
		WARN("Invalid GHES device driver DT ranges size (%d vs %d)\n",
		     freg_len, (int)sizeof(ghes_ranges));
		goto exit;
	}
	ghes_range_base = ghes_base;
	for (idx = 0; idx < ARRAY_SIZE(ghes_ranges); idx++) {
		/* per 'cell' settings, ranges consist of 64b, 64b, 32b */
		freg32 = (const fdt32_t *)&freg64[2];
		/*
		 * To avoid unaligned accesses, don't de-reference [FDT] 'freg'
		 * pointers.  Instead, use the 'fdtxxp_to_cpu' macro to extract
		 * the value.  Since the extracted value is in CPU order, use
		 * the function 'fdtxx_to_cpu' to convert it back to FDT order.
		 */
		ghes_ranges[idx].child_addr =
			fdt64_to_cpu(fdt64p_to_cpu(&freg64[0]));
		ghes_ranges[idx].parent_addr = 0; /* ignore placeholder value */
		ghes_ranges[idx].child_size =
			fdt32_to_cpu(fdt32p_to_cpu(freg32));

		ghes_ranges[idx].parent_addr = fdt64_to_cpu(ghes_range_base);
		ghes_range_base += fdt32_to_cpu(ghes_ranges[idx].child_size);

		INFO("%s range %d: 0x%016llx 0x%016llx 0x%08x\n",
		     sdei_ghes_dev_name, idx,
		     fdt64_to_cpu(ghes_ranges[idx].child_addr),
		     fdt64_to_cpu(ghes_ranges[idx].parent_addr),
		     fdt32_to_cpu(ghes_ranges[idx].child_size));

		freg64 = (void *)freg64 + sizeof(ghes_ranges[0]);
	}

	if ((ghes_range_base - ghes_base) > ghes_size) {
		WARN("Ranges for %s exceed %s size\n",
		     sdei_ghes_dev_name, sdei_ghes_mem_name);
		goto exit;
	}

	/* write the modified ranges */
	if (fdt_setprop((void *)fdt, ghes_dev_off, "ranges",
			ghes_ranges, sizeof(ghes_ranges))) {
		WARN("Unable to set GHES device driver DT ranges\n");
		goto exit;
	}

	fail = 0;

exit:
	if (fail)
		ERROR("GHES/HEST area not available\n");
#endif
}

/*
 * Used to retrieve the count of ETH devices (an abstraction of CGX)
 *
 * On entry,
 *   void
 *
 * Returns,
 *   count of ETH devices
 */
int plat_octeontx_get_eth_count(void)
{
	return plat_octeontx_get_cgx_count();
}

/*
 * Used to retrieve the count of LMAC devices per ETH
 *
 * On entry,
 *   void
 *
 * Returns,
 *   count of LMAC devices per ETH
 */
int plat_octeontx_get_eth_lmac_count(void)
{
	return MAX_LMAC_PER_CGX;
}

/*
 * Used to retrieve RVU information for an ETH/LMAC combination.
 *
 * On entry,
 *   eth_id:  ETH instance ID (0..n, see 'plat_octeontx_get_eth_count')
 *   lmac_id: LMAC instance ID (0..n, see 'plat_octeontx_get_eth_lmac_count')
 *   num_rvu_vfs:  ptr by which RVU VF count is returned
 *   num_msix_vec: ptr by which MSIX vector count is returned
 *   nix_block:    ptr by which ETH NIX block is returned
 *
 * Returns,
 *   void
 */
void plat_octeontx_get_eth_lmac_rvu_info(unsigned int eth_id,
					 unsigned int lmac_id,
					 int *num_rvu_vfs,
					 int *num_msix_vec,
					 int *nix_block)
{
	cgx_config_t *cgx_cfg;

	assert(eth_id < MAX_CGX);
	assert(lmac_id < MAX_LMAC_PER_CGX);

	cgx_cfg = &plat_octeontx_bcfg->cgx_cfg[eth_id];

	if (nix_block)
		*nix_block = cgx_cfg->nix_block;
	if (num_rvu_vfs)
		*num_rvu_vfs = cgx_cfg->lmac_cfg[lmac_id].num_rvu_vfs;
	if (num_msix_vec)
		*num_msix_vec = cgx_cfg->lmac_cfg[lmac_id].num_msix_vec;
}

/*
 * Used to mark an ETH/LMAC combination as enabled or disabled.
 *
 * On entry,
 *   eth_id:  ETH instance ID (0..n, see 'plat_octeontx_get_eth_count')
 *   lmac_id: LMAC instance ID (0..n, see 'plat_octeontx_get_eth_lmac_count')
 *   enabled: true or false
 *
 * Returns,
 *   void
 */
void plat_octeontx_enable_eth_lmac(unsigned int eth_id, unsigned int lmac_id,
				   int enabled)
{
	assert(eth_id < MAX_CGX);
	assert(lmac_id < MAX_LMAC_PER_CGX);
	plat_octeontx_bcfg->cgx_cfg[eth_id].lmac_cfg[lmac_id].lmac_enable =
		(enabled != 0);
}

/*
 * Indicates if a particular ETH/LMAC combination is enabled.
 *
 * On entry,
 *   eth_id:  ETH instance ID (0..n, see 'plat_octeontx_get_eth_count')
 *   lmac_id: LMAC instance ID (0..n, see 'plat_octeontx_get_eth_lmac_count')
 *
 * Returns,
 *   true if ETH/LMAC combo is enabled, else false
 */
int plat_octeontx_is_enabled_eth_lmac(unsigned int eth_id, unsigned int lmac_id)
{
	cgx_config_t *cgx_cfg;
	int enabled;

	assert(eth_id < MAX_CGX);
	assert(lmac_id < MAX_LMAC_PER_CGX);

	enabled = 0;

	if ((eth_id < MAX_CGX) && (lmac_id < MAX_LMAC_PER_CGX)) {
		cgx_cfg = &plat_octeontx_bcfg->cgx_cfg[eth_id];
		enabled = cgx_cfg->enable &&
			  cgx_cfg->lmac_cfg[lmac_id].lmac_enable;
	}

	return enabled;
}

/*
 * Indicates if a particular ETH/LMAC combination is configured as rfoe.
 *
 * On entry,
 *   eth_id:  ETH instance ID (0..n, see 'plat_octeontx_get_eth_count')
 *   lmac_id: LMAC instance ID (0..n, see 'plat_octeontx_get_eth_lmac_count')
 *
 * Returns,
 *   true if ETH/LMAC combo is configured as rfoe, else false
 */
int plat_octeontx_is_eth_lmac_rfoe(unsigned int eth_id,
				   unsigned int lmac_id)
{
	cgx_config_t *cgx_cfg;
	int rfoe_flag;

	assert(eth_id < MAX_CGX);
	assert(lmac_id < MAX_LMAC_PER_CGX);

	rfoe_flag = 0;

	if ((eth_id < MAX_CGX) && (lmac_id < MAX_LMAC_PER_CGX)) {
		cgx_cfg = &plat_octeontx_bcfg->cgx_cfg[eth_id];
		rfoe_flag = cgx_cfg->is_rfoe;
	}

	return rfoe_flag;
}

/*
 * plat_ras_feature_supported
 *
 * Indicates if platform supports RAS features.
 *
 * on entry,
 *   void
 * returns,
 *   true if platform supports RAS features else false
 */
bool plat_ras_feature_supported(void)
{
	uint64_t midr;
	static int ret = -1;

	(void)midr;

	/* only perform run-time check ONCE */
	if (ret == -1)
		ret = false; /* default to false (no support) */
	else
		return ret;

#if RAS_EXTENSION
	midr = read_midr();

	/* check if RAS functionality is supported by chip */
	if (!(IS_OCTEONTX_PN(midr, T96PARTNUM) ||
	      IS_OCTEONTX_PN(midr, F95PARTNUM) ||
	      IS_OCTEONTX_PN(midr, F95MMPARTNUM) ||
	      IS_OCTEONTX_PN(midr, LOKIPARTNUM) ||
	      IS_OCTEONTX_PN(midr, T98PARTNUM))) {
		ERROR("%s: RAS enabled on unsupported device\n", __func__);
		return ret;
	}

	/*
	 * check if RAS functionality should be excluded by board
	 *
	 * Disable on 9504n EBB due to problems with some DIMMs.
	 */
	if (!strncmp(plat_octeontx_bcfg->bcfg.board_model, "ebb9504n", 8)) {
		VERBOSE("%s: RAS support excluded from board %s\n",
			__func__, plat_octeontx_bcfg->bcfg.board_model);
		return ret; // i.e. false
	}

	ret = true;
#endif
	return ret;
}

/*
 * This function configures IOBN to grant access for GTI to secure memory
 */
void plat_gti_access_secure_memory_setup(int do_secure)
{
	/*
	 * dev_idx - Stream's dev number (stream_id<7:0>)
	 * bus_idx - Stream's bus number (stream_id<15:8>).
	 */
	uint64_t bus_idx = (CAVM_PCC_DEV_CON_E_GTI_CN9 >> 8) & 0xFF;
	uint64_t domain_idx = (CAVM_PCC_DEV_CON_E_GTI_CN9 >> 16) & 0xFF;
	uint64_t dev_idx = (CAVM_PCC_DEV_CON_E_GTI_CN9 >> 3) & 0xFF;

	cavm_iobnx_domx_busx_streams_t iobn_domx_busx_stream;
	cavm_iobnx_domx_devx_streams_t iobn_domx_devx_stream;

	for (int iobn_idx = 0; iobn_idx < plat_octeontx_scfg->iobn_count;
				iobn_idx++) {

		iobn_domx_busx_stream.u = CSR_READ(
			CAVM_IOBNX_DOMX_BUSX_STREAMS(iobn_idx,
			domain_idx, bus_idx));

		if (do_secure) {
			iobn_domx_busx_stream.s.strm_nsec = 0;
			iobn_domx_busx_stream.s.phys_nsec = 0;
			CSR_WRITE(CAVM_IOBNX_DOMX_BUSX_STREAMS(
				iobn_idx, domain_idx, bus_idx),
				iobn_domx_busx_stream.u);
		}

		iobn_domx_devx_stream.u = CSR_READ(
			CAVM_IOBNX_DOMX_DEVX_STREAMS(iobn_idx,
			domain_idx, dev_idx));

		if (do_secure) {
			iobn_domx_devx_stream.s.strm_nsec = 0;
			iobn_domx_devx_stream.s.phys_nsec = 0;
			CSR_WRITE(CAVM_IOBNX_DOMX_DEVX_STREAMS(
				iobn_idx, domain_idx, dev_idx),
				iobn_domx_devx_stream.u);
		}
	}
}

void plat_gti_irq_setup(int core)
{
	uint64_t vector_ptr;
	int intr_pinx;

	/* Get the offset of interrupt vector for this core */
	intr_pinx = CAVM_GTI_INT_VEC_E_CORE_WDOGX_INT_CN9(core);

	/* INTR_PINX vector address */
	vector_ptr = CAVM_GTI_BAR_E_GTI_PF_BAR4_CN9 + (intr_pinx << 4);

	/* Enable SECVEC to make the vector secure */
	octeontx_write64(vector_ptr, CAVM_GICD_SETSPI_SR | 1);
	vector_ptr += 0x8;
	octeontx_write64(vector_ptr, GTI_CWD_SPI_IRQ(core));
}
