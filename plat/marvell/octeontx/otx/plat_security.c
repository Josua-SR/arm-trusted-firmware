/*
 * Copyright (C) 2016-2018 Marvell International Ltd.
 * This program is provided "as is" without any warranty of any kind,
 * and is distributed under the applicable Marvell proprietary limited use
 * license agreement.
 */

#include <stdio.h>
#include <debug.h>
#include <arch.h>
#include <platform_def.h>
#include <octeontx_common.h>
#include <plat_board_cfg.h>
#include <octeontx_security.h>
#include <octeontx_dram.h>

struct l2c_region {
	unsigned int  number;
	unsigned long start;
	unsigned long end;
	unsigned int  secure;
};

#define LAST_L2C_REGION (~0U)
#define UNKNOWN_VALUE	(~0UL)

struct l2c_region l2c_map [] = {
	/* Secure non preserve region */
	{
		.number  = 0,
		.start   = TZDRAM_BASE,
		.end     = TZDRAM_BASE + TZDRAM_SIZE - 1,
		.secure  = 1,
	},
	/* Non-secure non preserve region */
	{
		.number  = 1,
		.start   = TZDRAM_BASE + TZDRAM_SIZE,
		.end     = UNKNOWN_VALUE,
		.secure  = 0,
	},
	/* Secure preserve region */
	{
		.number  = 2,
		.start   = UNKNOWN_VALUE,
		.end     = UNKNOWN_VALUE,
		.secure  = 1,
	},
	/* Non-secure preserve region */
	{
		.number  = 3,
		.start   = UNKNOWN_VALUE,
		.end     = UNKNOWN_VALUE,
		.secure  = 0,
	},
	{
		.number    = LAST_L2C_REGION,
	},
};


/* Flush the L2 Cache */
void l2c_flush(void)
{
	/* Select the L2 cache */
	union cavm_ap_csselr_el1 csselr_el1;
	union cavm_ap_ccsidr_el1 ccsidr_el1;
	unsigned int sets, ways;
	int l2_way, l2_set;
	uint64_t val;

	csselr_el1.s.level = 1;

	__asm__ volatile ("msr csselr_el1, %0" : : "r"((uint64_t)csselr_el1.u));

	__asm__ volatile ("mrs %0, ccsidr_el1" : "=&r"(val));

	ccsidr_el1.u = val;

	sets = ccsidr_el1.s.numsets + 1;
	ways = ccsidr_el1.s.associativity + 1;

	int is_rtg = 1; /* Clear remote tags */

	for (l2_way = 0; l2_way < ways; l2_way++) {
		for (l2_set = 0; l2_set < sets; l2_set++) {
			val = (uint64_t)128 * (l2_set + sets *
						(l2_way + (is_rtg * 16)));
			__asm__ volatile("sys #0,c11,C0,#5, %0\n" : : "r"(val));
		}
	}

	is_rtg = 0; /* Clear local tags */
	for (l2_way = 0; l2_way < ways; l2_way++) {
		for (l2_set = 0; l2_set < sets; l2_set++) {
			val = 128 * (l2_set + sets * (l2_way + (is_rtg * 16)));
			__asm__ volatile("sys #0,c11,C0,#5, %0\n" : : "r"(val));
		}
	}
}

void octeontx_security_setup(void)
{
	union cavm_l2c_asc_regionx_attr l2c_asc_attr;
	union cavm_l2c_asc_regionx_start bdk_l2c_asc_start, atf_l2c_asc_start;
	union cavm_l2c_asc_regionx_end bdk_l2c_asc_end, atf_l2c_asc_end;
	struct l2c_region *region = l2c_map;

	while (region->number != LAST_L2C_REGION) {
		/*
		 * ATF require some ASC regions to have ceratian start or end.
		 * If BDK configure regions in different way print error and
		 * panic.
		 */
		if (region->start != UNKNOWN_VALUE) {
			bdk_l2c_asc_start.u =
				CSR_READ(CAVM_L2C_ASC_REGIONX_START(
							region->number));
			atf_l2c_asc_start.u = region->start;
			if (bdk_l2c_asc_start.s.addr !=
			    atf_l2c_asc_start.s.addr) {
				ERROR(
				      "Start of ASC region %d is different for BDK(0x%x) and ATF(0x%x)\n",
				      region->number, bdk_l2c_asc_start.s.addr,
				      atf_l2c_asc_start.s.addr);
				panic();
			}
		}
		if (region->end != UNKNOWN_VALUE) {
			bdk_l2c_asc_end.u = CSR_READ(CAVM_L2C_ASC_REGIONX_END(
							region->number));
			atf_l2c_asc_end.u = region->end;
			if (bdk_l2c_asc_end.s.addr != atf_l2c_asc_end.s.addr) {
				ERROR(
				      "End of ASC region %d is different for BDK(0x%x) and ATF(0x%x)\n",
				      region->number, bdk_l2c_asc_end.s.addr,
				      atf_l2c_asc_end.s.addr);
				panic();
			}
		}

		l2c_asc_attr.u = CSR_READ(CAVM_L2C_ASC_REGIONX_ATTR(region->number));
		/*
		 * BDK cannot set non-secure regions, do it here.
		 */
		l2c_asc_attr.s.s_en  = region->secure;
		l2c_asc_attr.s.ns_en = !region->secure;

		CSR_WRITE(CAVM_L2C_ASC_REGIONX_ATTR(region->number), l2c_asc_attr.u);

		VERBOSE("Mark memory region %d:: %llx to %llx as %ssecure (%llx)\n",
			region->number,
			CSR_READ(CAVM_L2C_ASC_REGIONX_START(region->number)),
			CSR_READ(CAVM_L2C_ASC_REGIONX_END(region->number)),
			region->secure ? "" : "non-",
			CSR_READ(CAVM_L2C_ASC_REGIONX_ATTR(region->number)));

		region++;
	}

	VERBOSE("Flushing L1C\n");
	dcsw_op_all(DCCISW);

	VERBOSE("Flushing L2C\n");
	l2c_flush();

	VERBOSE("Flushing IC\n");
	__asm__ volatile("ic iallu\n"
			 "isb\n");
}

/*
 * This function configures SMMU to grant access for eMMC controller
 * to secure/non-secure memory based on input parameter passed
 */
void octeontx_configure_mmc_security(int secure)
{
	uint64_t val;
	uint64_t ssd_idx = CAVM_PCC_DEV_CON_E_MIO_EMM >> 5;
	uint64_t emm_ssd_mask = (1ULL << (CAVM_PCC_DEV_CON_E_MIO_EMM & 0x1F));
	union cavm_smmux_nscr0 smmux_nscr0;

	/* Check for MMC boot, if not return here */
	if (plat_octeontx_bcfg->bcfg.boot_dev.boot_type != OCTEONTX_BOOT_EMMC)
		return;

	val = CSR_READ(CAVM_SMMUX_SSDRX(0, ssd_idx));

	if (secure) {
		/*
		 * While booting from MMC device, it is necessary to
		 * configure SMMU as to grant access for eMMC controller
		 * to secure memory, where images are loaded
		 */
		smmux_nscr0.u = CSR_READ(CAVM_SMMUX_NSCR0(0));
		smmux_nscr0.s.nscfg = 2;
		CSR_WRITE(CAVM_SMMUX_NSCR0(0), smmux_nscr0.u);
		val &= ~emm_ssd_mask;
	} else {
		/*
		 * Configure SMMU and mark MMC controller in NODE0
		 * as acting for non-secure domain.
		 */
		val |= emm_ssd_mask;
	}
	CSR_WRITE(CAVM_SMMUX_SSDRX(0, ssd_idx), val);
}
