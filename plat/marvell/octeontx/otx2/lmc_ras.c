/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

/* RAS/EDAC driver for OcteonTX2 */

#include <arch.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <platform_def.h>
#include <octeontx_irqs_def.h>
#include <octeontx_common.h>
#include <plat_otx2_configuration.h>
#include <debug.h>
#include <errno.h>
#include <bl31/interrupt_mgmt.h>
#include <plat_board_cfg.h>
#include <octeontx_security.h>
#include <octeontx_dram.h>
#include <octeontx_utils.h>
#include <octeontx_mmap_utils.h>
#include <tools_share/uuid.h>
#include <plat/common/platform.h>
#include <plat_ras.h>
#include <plat_ghes.h>
#include <timers.h>
#include <drivers/delay_timer.h>
#include <octeontx_ehf.h>
#include <lib/el3_runtime/context_mgmt.h>
#include <octeontx_sdei.h>
#include <octeontx_plat_configuration.h>

#if RAS_EXTENSION
#include <lib/extensions/ras.h>
#endif /* RAS_EXTENSION */

#include "cavm-csrs-ccs.h"
#include "cavm-csrs-gic.h"
#include "cavm-csrs-lmc.h"
#include "cavm-csrs-mcc.h"

#define MAX_MCS		4

#define ECC_EL3_TEST

union ccs_pic_ctl {
	uint64_t u;
	struct ccs_pic_ctl_s {
		uint64_t cclk_dis:1;
		uint64_t error_inject:2;
		uint64_t reserved_3_31:29;
		uint64_t exdatreq:6;
		uint64_t exdatrqh:6;
		uint64_t excmdreq:6;
		uint64_t excmdrqh:6;
		uint64_t reserved_56_63:8;
	} s;
};

union ccs_tad_ctl {
	uint64_t u;
	struct ccs_tad_ctl_s {
		uint64_t maxlfb:5;
		uint64_t mccwr_thresh:5;
		uint64_t exlvc:5;
		uint64_t exvic:5;
		uint64_t exfwd:5;
		uint64_t exrrq:5;
		uint64_t exrq2:5;
		uint64_t exrq1:5;
		uint64_t reserved_40_44:5;
		uint64_t frcnalc:1;
		uint64_t cclk_dis:1;
		uint64_t disdwb:1;
		uint64_t sameidx_thresh:6;
		uint64_t disrqhprio:1;
		uint64_t disdfill:1;
		uint64_t disadrwbfl:1;
		uint64_t persist_wbfl:3;
		uint64_t frclckdbe:1;
		uint64_t reserved_61_63:3;
	} s;
};

struct ras_dram_lmc_map {
	int lmc;
	int mcc;
	int lmcoe;
	int valid;
};
struct ras_dram_lmc_map plat_lmc_map[MAX_LMC];

static uint64_t el3_test_target[2 * CACHE_WRITEBACK_GRANULE / 8];

struct ras_ccs_addr_conversion_data {
	uint64_t addr;          //uint64_t addr;
	int phys_lmc_mask;      //MASK of LMC
	int num_regions;
	uint64_t regions_start[16], regions_end[16], regions_offset[16];
	uint8_t regions_lmc_mode[16], regions_lmc_mask[16];
	uint8_t rgn_idx_to_id[16]; //added by ATF
	int ASC_REGION;
	int ASC_REM;
	int ASC_MD_LR_BIT;      //Left Right Bit selector
	int ASC_LMC_MASK;		//Mask of Channels enabled
	int ASC_LMC_MODE;		//Mode region is setup as
	uint64_t ASC_MD_LR_EN;	//Bit mask of Address bits used in hash
	uint64_t ASC_OFFSET;	//Address offset
	uint64_t ASC_MCS_EN[4];	//Bit Masks for Hashing
	// T98 specific variables
	int lrbit;
	int t98_cfg;
	int abs_lmc; //Absolute LMC aka 0-5 b/c phys_lmc_mask is relative on t98
	int md_right;
};

// Table to translate ECC single-bit syndrome to "byte.bit" format.
//
// indexed by syndrome, data is [7:4] = byte no. + 1 and [3:0] = bit no.
// hack: make the byte no. bits be non-zero even for byte 0;
// this is so non-explicit data can be 0x00 and will be illegal.
static const unsigned char
ecc_syndrome_to_bytebit[256] = {
	// byte 0, bits 0 to 7
	[0xce] = 0x10, [0xcb] = 0x11, [0xd3] = 0x12, [0xd5] = 0x13,
	[0xd6] = 0x14, [0xd9] = 0x15, [0xda] = 0x16, [0xdc] = 0x17,
	// byte 1, bits 0 to 7
	[0x23] = 0x20, [0x25] = 0x21, [0x26] = 0x22, [0x29] = 0x23,
	[0x2a] = 0x24, [0x2c] = 0x25, [0x31] = 0x26, [0x34] = 0x27,
	// byte 2, bits 0 to 7
	[0x0e] = 0x30, [0x0b] = 0x31, [0x13] = 0x32, [0x15] = 0x33,
	[0x16] = 0x34, [0x19] = 0x35, [0x1a] = 0x36, [0x1c] = 0x37,
	// byte 3, bits 0 to 7
	[0xe3] = 0x40, [0xe5] = 0x41, [0xe6] = 0x42, [0xe9] = 0x43,
	[0xea] = 0x44, [0xec] = 0x45, [0xf1] = 0x46, [0xf4] = 0x47,
	// byte 4, bits 0 to 7
	[0x4f] = 0x50, [0x4a] = 0x51, [0x52] = 0x52, [0x54] = 0x53,
	[0x57] = 0x54, [0x58] = 0x55, [0x5b] = 0x56, [0x5d] = 0x57,
	// byte 5, bits 0 to 7
	[0xa2] = 0x60, [0xa4] = 0x61, [0xa7] = 0x62, [0xa8] = 0x63,
	[0xab] = 0x64, [0xad] = 0x65, [0xb0] = 0x66, [0xb5] = 0x67,
	// byte 6, bits 0 to 7
	[0x8f] = 0x70, [0x8a] = 0x71, [0x92] = 0x72, [0x94] = 0x73,
	[0x97] = 0x74, [0x98] = 0x75, [0x9b] = 0x76, [0x9d] = 0x77,
	// byte 7, bits 0 to 7
	[0x62] = 0x80, [0x64] = 0x81, [0x67] = 0x82, [0x68] = 0x83,
	[0x6b] = 0x84, [0x6d] = 0x85, [0x70] = 0x86, [0x75] = 0x87,
	// byte 8, bits 0 to 7
	[0x01] = 0x90, [0x02] = 0x91, [0x04] = 0x92, [0x08] = 0x93,
	[0x10] = 0x94, [0x20] = 0x95, [0x40] = 0x96, [0x80] = 0x97
};

#ifdef EDAC_POLLED
static int edac_timer = -1; /* periodic poll */
#endif

int edac_alive; /* protect against EDAC_POLLED early polling */

/**
 * Find the number of bits to add to LMC addresses to include LMC num
 *
 * @return number xbits to add to LMC addresses
 */
static inline int ras_ccs_get_num_xbits(void)
{
	if (cavm_is_model(OCTEONTX_CN98XX))
		return 3;
	else
		return 2;
}

/**
 * Add 2 bits into the LMC address to represent the LMC the address
 * came from. (like the CN8xxx LMC address configuration)
 *
 * @param addr  an LMC address without LMC bits
 * @param lmc   The LMC the address originated from
 *
 * @return an LMC address with LMC Bits
 */
static inline uint64_t ras_ccs_get_lmc_addr(uint64_t addr, int lmc)
{
	int xbits = ras_ccs_get_num_xbits();

	return  (((((((1ULL << 36)-1) << 7) & addr) << xbits) |
		  (0x7f & addr)) | (lmc  << 7));
}

/**
 * Remove the two bits from an interal LMC address that represent the
 * LMC the address is on and return just the DRAM bits
 *
 * @param addr  an LMC address with LMC bits
 *
 * @return an LMC address without LMC Bits
 */
static inline uint64_t ras_ccs_get_dram_addr(uint64_t addr)
{
	int xbits = ras_ccs_get_num_xbits();

	return (((((1ULL << 36)-1) << 9) & addr) >> xbits) |
	       (0x7f & addr);
}

/**
 * Extract the LMC from the address provided
 *
 * @param address   A full LMC address with LMC bits indicating
 *                  which lmc it came from
 *
 * @return the LMC the address originated on
 */
static inline int ras_ccs_find_lmc(uint64_t address)
{
	int xbits = ras_ccs_get_num_xbits();

	return (address >> 7) & ((0x1 << xbits)-1);
}

/**
 * Population count
 *
 * @param number	64-bit number to count lmc_mod6_hash_bits
 *
 * @returns		Number of 1 bits
 */
static inline int ras_dpop(uint64_t number)
{
	number -= (number >> 1) & 0x5555555555555555;
	number = (number & 0x3333333333333333) +
		 ((number >> 2) & 0x3333333333333333);
	number = (number + (number >> 4)) & 0x0f0f0f0f0f0f0f0f;
	return (number * 0x0101010101010101) >> 56;
}

/**
 * Perform a Reduction XOR operation and return result
 *
 * @param number    number to do reduction XOR on
 *
 * @return          0 if number of bits is even
 *                  1 if number of bits is odd
 */
static inline int __rxor(uint64_t number)
{
	return ras_dpop(number) % 2;	//64 bit popcount
}

/**
 * Unhash the physical address without the left right bit
 *
 * @param _pa_no_lr_hash	Hashed address without the left right bit
 * @param ccs_adr_mcs		CAVM_CCS_ADR_MCSX data
 *
 * @return on Success		Unhashed LMC address without the left right bit
 *         on Failure		-1
 */
uint64_t ras_ccs_reverse_lmc_hash(uint64_t _pa_no_lr_hash,
				   uint64_t *ccs_adr_mcs)
{
	uint64_t pa_no_lr;
	uint64_t adr_mcs[4];
	int lmc_hash, i;
	int low_addr;
	int num_good_bits = 0;
	int upper_lmc_hash_parity[4];
	int lower_lmc_hash_parity;

	pa_no_lr = _pa_no_lr_hash & ~(0xfff);
	lmc_hash = (_pa_no_lr_hash >> 8) & 0xf;

	debug2ras("%-40s: 0x%016llx\n", "pre pa_no_lr", pa_no_lr);
	debug2ras("%-40s: 0x%x\n", "lmc_hash", lmc_hash);

	for (i = 0; i < 4; i++) {
		adr_mcs[i] = ccs_adr_mcs[i];
		upper_lmc_hash_parity[i] =
			__rxor((adr_mcs[i] >> 12) & (pa_no_lr >> 12));
	}
	debug2ras("%-40s: [%d%d%d%d]\n", "upper_lmc_hash_parity",
		  upper_lmc_hash_parity[0], upper_lmc_hash_parity[1],
		  upper_lmc_hash_parity[2], upper_lmc_hash_parity[3]);

	/* Loop all possible address values in bits [3:0] and check to
	 * see if exactly one matches.
	 */
	for (low_addr = 0; low_addr < 16; low_addr++) {
		num_good_bits = 0;
		for (i = 0; i < 4; i++) {
			lower_lmc_hash_parity =
				__rxor(((adr_mcs[i] >> 8) & 0xf) & low_addr);
			if ((lower_lmc_hash_parity ^
			      upper_lmc_hash_parity[i]) ==
			       ((lmc_hash >> i) & 0x1))
				num_good_bits++;
		}
		if (num_good_bits == 4)
			break;
	}

	debug2ras("%-40s: 0x%x\n", "low_addr", low_addr);

	if (num_good_bits == 4)
		return pa_no_lr | (low_addr << 8);
	else
		return -1;
}

/**
 * Convert an LMC address into a Physical Address used coreside
 * this tries conversion within a single translation region
 *
 * @param ccs_data	Data used to decode the LMC Address
 *
 * @return  on success	Physical address coressponding to the LMC address
 *          on failure	-1
 */
uint64_t ras_ccs_convert_lmc_to_pa_algorithm(
				struct ras_ccs_addr_conversion_data *ccs_data)
{
	uint64_t pre_offset     = ccs_data->addr;
	int region              = ccs_data->ASC_REGION;
	int ASC_MD_LR_BIT       = ccs_data->ASC_MD_LR_BIT;
	uint64_t ASC_MD_LR_EN   = ccs_data->ASC_MD_LR_EN;
	int ASC_LMC_MASK        = ccs_data->ASC_LMC_MASK;
	int ASC_LMC_MODE        = ccs_data->ASC_LMC_MODE;
	int phys_lmc_mask       = ccs_data->phys_lmc_mask;

	uint64_t pa_no_lr;
	uint64_t pa_no_lr_hash = 0;
	uint64_t pa;
	uint64_t gen_addr;
	uint64_t left_mask;
	uint64_t right_mask;

	int lrbit;
	int MD_mirror = 0;
	int MD_right = 0;
	int MD_right_calculated;
	int lmcquarter;
	int pa_no_lr_hash_mod6;
	int pa_no_lr_hash8 = 0;

	debug2ras("Starting LMC to PA Conversion Algorithm\n");
	debug2ras("%-40s: 0x%016lx\n", "pre_offset", (long)pre_offset);

	/* check if lmcmask and ASC LMC MASK intersect if not this region
	 * isn't mapped to this ASC
	 */
	if ((phys_lmc_mask & ASC_LMC_MASK) == 0) {
		debug_ras("LMC mask 0x%x can't Fit into ASC %d LMC_MASK 0x%x\n",
			  phys_lmc_mask, region, ASC_LMC_MASK);
		return -1;
	}

	if (ccs_data->t98_cfg) {
		MD_right = ccs_data->abs_lmc & 0x1;
		switch (ASC_LMC_MODE) {
		case CAVM_CCS_LMC_MODE_E_FLAT_1:
			pa_no_lr_hash = pre_offset << 8;
			debug2ras("%-40s: 0x%016lx\n", "pa_no_lr_hash",
				  (long)pa_no_lr_hash);
			break;
		case CAVM_CCS_LMC_MODE_E_STRIPE_2:
			if (ASC_LMC_MASK == 0x3)                    //LMC01
				pa_no_lr_hash8 = phys_lmc_mask & 0x1 ? 0 : 1;
			else if (ASC_LMC_MASK == 0x5)               //LMC02
				pa_no_lr_hash8 = phys_lmc_mask & 0x1 ? 0 : 1;
			else if (ASC_LMC_MASK == 0x6)               //LMC12
				pa_no_lr_hash8 = phys_lmc_mask & 0x2 ? 0 : 1;
			pa_no_lr_hash = ((pre_offset << 1) | pa_no_lr_hash8)
					<< 8;

			debug2ras("%-40s: %d\n", "pa_no_lr_hash8",
				  pa_no_lr_hash8);
			debug2ras("%-40s: 0x%016lx\n", "pa_no_lr_hash",
				  (long)pa_no_lr_hash);
		break;

		case CAVM_CCS_LMC_MODE_E_STRIPE_3:
			pa_no_lr_hash = (pre_offset*3 + phys_lmc_mask/2) << 8;
			debug2ras("%-40s: 0x%016lx\n", "pa_no_lr_hash",
				  (long)pa_no_lr_hash);
			break;
		default:
			debug2ras("Unsupported lmc_mode: %x\n", ASC_LMC_MODE);
			return -1;
		}
	} else {
		switch (ASC_LMC_MODE) {
		case CAVM_CCS_LMC_MODE_E_FLAT_1:
			MD_right = pre_offset & 0x1;
			//right shift by 1 to get bits 34:1
			//and left shift by 8 to move bits to [41:8]
			pa_no_lr_hash = (pre_offset >> 1);
			pa_no_lr_hash <<= 8;
			pa_no_lr_hash &= ((1ULL << 41) - 1);

			debug2ras("%-40s: %d\n", "MD_right", MD_right);
			debug2ras("%-40s: 0x%016lx\n", "pa_no_lr_hash",
				  (long)pa_no_lr_hash);
		break;

		case CAVM_CCS_LMC_MODE_E_STRIPE_2:
			pa_no_lr_hash = (pre_offset << 8);
			/*
			 * if phys_lmc_mask is LMC0 MD_right is 1
			 * if it is LMC1 MD_right is 0
			 */
			if (ASC_LMC_MASK == 0x3)                    //LMC01
				MD_right = phys_lmc_mask & 0x1 ? 1 : 0;
			/*
			 * if phys_lmc_mask is LMC0 MD_right is 1
			 * if it is LMC2 MD_right is 0
			 */
			else if (ASC_LMC_MASK == 0x5)               //LMC02
				MD_right = phys_lmc_mask & 0x1 ? 1 : 0;
			/*
			 * if phys_lmc_mask is LMC2 MD_right is 1
			 * if it is LMC1 MD_right is 0
			 */
			else if (ASC_LMC_MASK == 0x6)               //LMC12
				MD_right = phys_lmc_mask & 0x4 ? 1 : 0;

			debug2ras("%-40s: %d\n", "MD_right", MD_right);
			debug2ras("%-40s: 0x%016lx\n", "pa_no_lr_hash",
				  (long)pa_no_lr_hash);
			break;

		case CAVM_CCS_LMC_MODE_E_STRIPE_3:
			lmcquarter = pre_offset & 0x3;
			pa_no_lr_hash_mod6 = 0;

			/*
			 * Determine pa_no_lr_hash_mod6 and MD_right based upon
			 * which LMC the addr came from and the LMC quarter
			 */
			if (phys_lmc_mask == 0x1) {     //LMC0
				MD_right = 1;
				if (lmcquarter == 0x0)
					pa_no_lr_hash_mod6 = 0;
				else if (lmcquarter == 0x1)
					pa_no_lr_hash_mod6 = 0x3;
				else if (lmcquarter == 0x2)
					pa_no_lr_hash_mod6 = 0x5;
				else if (lmcquarter == 0x3) {
					pa_no_lr_hash_mod6 = 0x1;
					MD_right = 0;
				} else {
					debug_ras("ERROR: Unsupported lmc, "
						  "lmcquarter: %x %x\n",
						  phys_lmc_mask, lmcquarter);
					return -1;
				}
			} else if (phys_lmc_mask == 0x2) {  //LMC1
				MD_right = 0;
				if (lmcquarter == 0x0)
					pa_no_lr_hash_mod6 = 0;
				else if (lmcquarter == 0x1)
					pa_no_lr_hash_mod6 = 0x2;
				else if (lmcquarter == 0x2)
					pa_no_lr_hash_mod6 = 0x3;
				else if (lmcquarter == 0x3)
					pa_no_lr_hash_mod6 = 0x4;
				else {
					debug_ras("ERROR: Unsupported lmc, "
						  "lmcquarter: %x %x\n",
						  phys_lmc_mask, lmcquarter);
					return -1;
				}
			} else if (phys_lmc_mask == 0x4) {  //LMC2
				MD_right = 1;
				if (lmcquarter == 0x0)
					pa_no_lr_hash_mod6 = 0x1;
				else if (lmcquarter == 0x1)
					pa_no_lr_hash_mod6 = 0x2;
				else if (lmcquarter == 0x2)
					pa_no_lr_hash_mod6 = 0x4;
				else if (lmcquarter == 0x3) {
					pa_no_lr_hash_mod6 = 0x5;
					MD_right = 0;
				} else {
					debug_ras("ERROR: Unsupported lmc, "
						  "lmcquarter: %x %x\n",
						  phys_lmc_mask, lmcquarter);
					return -1;
				}
			}

			pa_no_lr_hash = (((pre_offset >> 2) * 6) +
					 pa_no_lr_hash_mod6) << 8;

			debug2ras("%-40s: %d\n", "lmcquarter", lmcquarter);
			debug2ras("%-40s: 0x%x\n", "pa_no_lr_hash_mod6",
				  pa_no_lr_hash_mod6);
			debug2ras("%-40s: %d\n", "MD_right", MD_right);
			debug2ras("%-40s: 0x%016lx\n", "pa_no_lr_hash",
				  (long)pa_no_lr_hash);

			break;

		default:
			debug_ras("Unsupported lmc_mode: %x\n", ASC_LMC_MODE);
			return -1;
		}
	}

	pa_no_lr = ras_ccs_reverse_lmc_hash(pa_no_lr_hash,
					    ccs_data->ASC_MCS_EN);
	debug2ras("%-40s: 0x%016lx\n", "pa_no_lr", (long)pa_no_lr);
	if (pa_no_lr == (uint64_t)-1) {
		debug_ras("No Valid LMC Hash Combinations Work for lmc%0d "
			  "address: %0lx.\n", phys_lmc_mask, (long)pre_offset);
		return -1;
	}

	//left mask
	left_mask = ((1ULL << (42 - (ASC_MD_LR_BIT + 7))) - 1) <<
		    (ASC_MD_LR_BIT + 8);

	//right mask
	right_mask = (1ULL << (ASC_MD_LR_BIT + 8)) - 1;

	debug2ras("%-40s: 0x%lx\n", "left_mask", (long)left_mask);
	debug2ras("%-40s: 0x%lx\n", "right_mask", (long)right_mask);

	//pa_no_lr is currently bits 42:0 but bits 7:0 are left as 0
	pa = (pa_no_lr & left_mask) | ((pa_no_lr & right_mask) >> 1);

	debug2ras("%-40s: 0x%016lx\n", "pre_pa", (long)pa);

	MD_right_calculated = __rxor((ASC_MD_LR_EN << 7) & pa) ^ MD_mirror;
	if (MD_right == MD_right_calculated)
		lrbit = 0;
	else
		lrbit = 1;

	gen_addr = (pa | lrbit << (ASC_MD_LR_BIT + 7));

	debug2ras("Generated Addr: 0x%016lx\n", (long)gen_addr);
	return gen_addr;
}

static int prep_done /* = 0 */;
static struct ras_ccs_addr_conversion_data  ccs_data;

static int ras_ccs_do_prep(void)
{
	int i, region;

	if (!prep_done) {

		//memset(&ccs_data, 0, sizeof(bdk_ccs_addr_conversion_data_t));

		CSR_INIT(adr_ctl, CAVM_CCS_ADR_CTL);
		ccs_data.ASC_MD_LR_BIT =   adr_ctl.s.md_lr_bit;
		ccs_data.ASC_MD_LR_EN =    adr_ctl.s.md_lr_en;

		//T98 hashing algorithm is different
		ccs_data.t98_cfg = cavm_is_model(OCTEONTX_CN98XX);

		for (i = 0; i < 4; i++)
			ccs_data.ASC_MCS_EN[i] = CSR_READ(CAVM_CCS_ADR_MCSX(i));

		// extract the ASC region info
		CSR_INIT(ccs_const, CAVM_CCS_CONST);
		int max_regions = ccs_const.s.asc;

		int idx = 0;

		for (region = 0; region < max_regions; region++) {

			//Check if the ASC Region is Enabled or not
			CSR_INIT(region_attr,
				 CAVM_CCS_ASC_REGIONX_ATTR(region));
			if ((region_attr.s.s_en == 0) &&
			    (region_attr.s.ns_en == 0))
				continue; //Region is disabled, go to the next

			ccs_data.rgn_idx_to_id[idx] = region; // added by ATF
			// record the region's info
			ccs_data.regions_lmc_mode[idx] = region_attr.s.lmc_mode;
			ccs_data.regions_lmc_mask[idx] = region_attr.s.lmc_mask;
			CSR_INIT(start,  CAVM_CCS_ASC_REGIONX_START(region));
			CSR_INIT(end,    CAVM_CCS_ASC_REGIONX_END(region));
			CSR_INIT(offset, CAVM_CCS_ASC_REGIONX_OFFSET(region));
			ccs_data.regions_start[idx]  = start.u;
			ccs_data.regions_end[idx]    = end.u + (1UL << 24);
			ccs_data.regions_offset[idx] = offset.u;

			idx++; // count it
		}
		ccs_data.num_regions = idx;

		prep_done = 1;
	}
	return 0;
}

/**
 * Convert a memory address from a LMC address in CN9XXX to
 * a physical address, trying conversion in all translation regions
 *
 * @param _lmc_addr     Full LMC Address including cache line bits
 * @param r_p		set to ASC region number
 * @param s_p		set to is_secure_region
 * @param ns_p		set to is_ns_region
 *
 * @return	on success	A physical address corresponding with
 *				the lmc address given
 *		on failure	-1
 */
static uint64_t ras_ccs_convert_lmc_to_pa(uint64_t _lmc_addr,
				int *r_p, int *s_p, int *ns_p)
{
	int region;
	uint64_t phys_addr;
	uint64_t internal_lmc_addr;
	int found_good_addr = 0;
	uint64_t final_phys_addr = 0;
	int lmc;

	debug2ras("Starting LMC to PA Conversion\n");

	/* added by ATF */
	if (s_p)
		*s_p = 0;
	if (ns_p)
		*ns_p = 0;

	ras_ccs_do_prep();

	debug2ras("Original LMC ADDR: 0x%016lx\n", (long)_lmc_addr);

	lmc = ras_ccs_find_lmc(_lmc_addr);
	internal_lmc_addr = ras_ccs_get_dram_addr(_lmc_addr);

	if (ccs_data.t98_cfg) {
		ccs_data.phys_lmc_mask = 0x1 << (lmc/2);
		ccs_data.lrbit = lmc & 0x1;
		ccs_data.abs_lmc = lmc; // Include absolute LMC number for t98
	} else
		ccs_data.phys_lmc_mask = 0x1 << lmc;

	debug2ras("ADDR: 0x%016lx LMC origin: %d\n", (long)internal_lmc_addr,
		  lmc);

	for (region = 0; region < ccs_data.num_regions; region++) {
		// every counted region is enabled
		debug2ras("ASC%d: Checking Conversion from LMC to PA\n",
			  region);

		ccs_data.ASC_REGION = region;
		ccs_data.ASC_LMC_MASK = ccs_data.regions_lmc_mask[region];
		ccs_data.ASC_LMC_MODE = ccs_data.regions_lmc_mode[region];

		ccs_data.addr = internal_lmc_addr /
			CACHE_WRITEBACK_GRANULE; /* i.e. BDK_CACHE_LINE_SIZE */

		phys_addr = ras_ccs_convert_lmc_to_pa_algorithm(&ccs_data);
		if (phys_addr == (uint64_t)-1) {
			debug_ras("ASC%d: Failed to Convert LMC to Phys Addr\n",
				  region);
			continue;
		} else {
			int rgn_id;
			uint64_t scaled_offset =
					ccs_data.regions_offset[region];
			if (scaled_offset != 0) {
				switch (ccs_data.ASC_LMC_MODE) {
				case 0: /* CAVM_CCS_LMC_MODE_E_FLAT_1 */
					scaled_offset <<= 24; // 16 MB units
					break;
				case 2: /* CAVM_CCS_LMC_MODE_E_STRIPE_2 */
					scaled_offset <<= 25 ; // 32 MB units
					break;
				case 3: /* CAVM_CCS_LMC_MODE_E_STRIPE_3 */
					scaled_offset *= 3;
					scaled_offset <<= 24; // 48 MB units
					break;
				}
			}

			/* added by ATF */
			rgn_id = ccs_data.rgn_idx_to_id[region];
			CSR_INIT(region_attr,
				 CAVM_CCS_ASC_REGIONX_ATTR(rgn_id));
			if (r_p)
				*r_p = rgn_id;
			if (region_attr.s.s_en && s_p)
				*s_p = 1;
			if (region_attr.s.ns_en && ns_p)
				*ns_p = 1;

			final_phys_addr = (phys_addr - scaled_offset) &
					  ((1ULL << 42) - 1);
			found_good_addr++;
			debug2ras("ASC%d: Found Phys Addr: 0x%016lx\n", region,
				  (long)final_phys_addr);
			break; // FIXME? no further checking now for overlap/etc
		}
	}

	if (found_good_addr == 1)
		return final_phys_addr; // ATF; bdk_numa_get_address(...);
	if (found_good_addr > 1)
		debug_ras("ERROR FOUND ALIASING!\n");
	else
		debug_ras("ERROR UNABLE TO FIND PHYS ADDR!\n");
	//printf("CCS Decode - LMC to PA returned -1\n");
	return -1;
}

/**
 * Return the number of bank bits in use
 *
 * NOTE: all DDR3, and DDR4 x16 today, use only 3 bank bits; DDR4 x4 and x8
 * always have 4 bank bits
 * NOTE: this will change in the future, when DDR4 x16 devices can come with
 * 16 banks!! FIXME!!
 */
static int ras_dram_get_num_bank_bits(int lmc)
{
	union cavm_lmcx_config lmcx_config;
	int bank_width;

	lmcx_config.u = CSR_READ(CAVM_LMCX_CONFIG(lmc)); /* sample LMCn */
	bank_width = lmcx_config.s.bg2_enable ? 4 : 3;

	return bank_width;
}

/**
 * Given a physical DRAM address, extract information about the LMC, DIMM,
 * prank, lrank, bank, row, and column that was accessed.
 *
 * @param address Full LMC Address
 * @param lmc     LMC controller the address was for
 * @param dimm    DIMM the address was for
 * @param prank   Physical RANK on the DIMM
 * @param lrank   Logical RANK on the DIMM
 * @param bank    BANK on the DIMM
 * @param row     Row on the DIMM
 * @param col     Column on the DIMM
 */
void ras_dram_address_extract_info(uint64_t address, int *lmc,
				   int *dimm, int *prank, int *lrank,
				   int *bank, int *row, int *col)
{
	int xbits = 2;
	union cavm_lmcx_config lmcx_config;
	union cavm_lmcx_ext_config ext_config;
	int bank_lsb;
	int bank_width;
	int dimm_lsb;
	int dimm_width;
	int prank_lsb;
	int prank_width;
	int lrank_lsb;
	int lrank_width;
	int row_lsb;
	int row_width;
	int col_hi_lsb;
	int col_hi_width;
	int col_hi;

	/* xbits setup in address conversion to look like 8x */
	*lmc = octeontx_bit_extract(address, 7, xbits);

	bank_lsb = 7 + xbits;
	/* Figure out the bank field width */
	lmcx_config.u = CSR_READ(CAVM_LMCX_CONFIG(*lmc));
	bank_width = ras_dram_get_num_bank_bits(*lmc);

	/* Extract additional info from the LMC_CONFIG CSR */
	ext_config.u = CSR_READ(CAVM_LMCX_EXT_CONFIG(*lmc));
	dimm_lsb    = 28 + lmcx_config.s.pbank_lsb + xbits;
	dimm_width  = 40 - dimm_lsb;
	prank_lsb   = dimm_lsb - lmcx_config.s.rank_ena;
	prank_width = dimm_lsb - prank_lsb;
	lrank_lsb   = prank_lsb - ext_config.s.dimm0_cid;
	lrank_width = prank_lsb - lrank_lsb;
	row_lsb     = 14 + lmcx_config.s.row_lsb + xbits;
	row_width   = lrank_lsb - row_lsb;
	col_hi_lsb  = bank_lsb + bank_width;
	col_hi_width = row_lsb - col_hi_lsb;

	/* Extract the parts of the address */
	*dimm =  octeontx_bit_extract(address, dimm_lsb, dimm_width);
	*prank = octeontx_bit_extract(address, prank_lsb, prank_width);
	*lrank = octeontx_bit_extract(address, lrank_lsb, lrank_width);
	*row =   octeontx_bit_extract(address, row_lsb, row_width);

	/* bank calculation may be aliased... */
	/* xor_bank not present on CN9XXX */
	*bank = octeontx_bit_extract(address, bank_lsb, bank_width);
	/* LMC number already extracted */
	col_hi = octeontx_bit_extract(address, col_hi_lsb, col_hi_width);

	*col = octeontx_bit_extract(address, 3, 4) | (col_hi << 4);
	/* Bus byte is address bits [2:0]. Unused here */
}

static struct ras_dram_lmc_map *ras_dram_get_lmc_map(int lmc)
{
	static int once;

	if (!once && !is_asim()) {
		union cavm_mccx_const mcc_const;
		union cavm_mccx_lmcoex_const oeconst;
		struct ras_dram_lmc_map *lm;
		int mcc, lmcoe, abs_lmc;
		int num_mccs = plat_octeontx_get_mcc_count();

		for (mcc = 0; mcc < num_mccs; mcc++) {
			mcc_const.u = CSR_READ(CAVM_MCCX_CONST(mcc));
			for (lmcoe = 0; lmcoe < mcc_const.s.lmcs ; lmcoe++) {
				oeconst.u = CSR_READ(CAVM_MCCX_LMCOEX_CONST(
							mcc, lmcoe));
				abs_lmc = oeconst.s.lmc;
				lm = &plat_lmc_map[abs_lmc];
				lm->mcc = mcc;
				lm->lmc = abs_lmc;
				lm->lmcoe = lmcoe;
				lm->valid = 1;
			}
		}

/*
 * A conditional is required here to prevent a compiler array bounds error
 * initializing plat_lmc_map[].  When compiled for platforms OTHER than t98,
 * variable plat_lmc_map[MAX_LMC] has fewer than 6 elements, causing a
 * compiler error.  The compiler has no way of knowing that the run-time
 * check for 'OCTEONTX_CN98XX' will prevent invalid memory accesses on
 * non-t98 platforms.  This error was seen with GCC10, not GCC7.
 */
#ifdef PLAT_t98
		/* T98 has problem with MCCX_LMCOEX_CONST; use static config */
		if (cavm_is_model(OCTEONTX_CN98XX)) {
			/* Highlight problem with new chip revs. */
			if (!cavm_is_model(OCTEONTX_CN98XX_PASS1_X))
				ERROR("Software not configured for this chip.");
			else {
				plat_lmc_map[0] = (struct ras_dram_lmc_map) {
				   .lmc = 0, .mcc = 0, .lmcoe = 0, .valid = 1 };
				plat_lmc_map[1] = (struct ras_dram_lmc_map) {
				   .lmc = 1, .mcc = 1, .lmcoe = 0, .valid = 1 };
				plat_lmc_map[2] = (struct ras_dram_lmc_map) {
				   .lmc = 2, .mcc = 0, .lmcoe = 1, .valid = 1 };
				plat_lmc_map[3] = (struct ras_dram_lmc_map) {
				   .lmc = 3, .mcc = 1, .lmcoe = 1, .valid = 1 };
				plat_lmc_map[4] = (struct ras_dram_lmc_map) {
				   .lmc = 4, .mcc = 0, .lmcoe = 2, .valid = 1 };
				plat_lmc_map[5] = (struct ras_dram_lmc_map) {
				   .lmc = 5, .mcc = 1, .lmcoe = 2, .valid = 1 };
			}
		}
#endif

		once++;
	} else if (!once && is_asim()) {
		/* h/w discovery invalid in ASIM, use static config */
		if (cavm_is_model(OCTEONTX_CN96XX) ||
		    cavm_is_model(OCTEONTX_CN98XX)) {
			plat_lmc_map[0] = (struct ras_dram_lmc_map) {
				.lmc = 0, .mcc = 1, .lmcoe = 0, .valid = 1 };
			plat_lmc_map[1] = (struct ras_dram_lmc_map) {
				.lmc = 1, .mcc = 0, .lmcoe = 0, .valid = 1 };
			plat_lmc_map[2] = (struct ras_dram_lmc_map) {
				.lmc = 2, .mcc = 1, .lmcoe = 1, .valid = 1 };
		} else if (cavm_is_model(OCTEONTX_F95MM) ||
			   cavm_is_model(OCTEONTX_CNF95XX)) {
			plat_lmc_map[0] = (struct ras_dram_lmc_map) {
				.lmc = 0, .mcc = 0, .lmcoe = 0, .valid = 1 };
			plat_lmc_map[1] = (struct ras_dram_lmc_map) {
				.valid = 0 };
			plat_lmc_map[2] = (struct ras_dram_lmc_map) {
				.lmc = 2, .mcc = 0, .lmcoe = 1, .valid = 1 };
		} else if (cavm_is_model(OCTEONTX_LOKI) ||
			   cavm_is_model(OCTEONTX_F95O)) {
			plat_lmc_map[0] = (struct ras_dram_lmc_map) {
				.lmc = 0, .mcc = 0, .lmcoe = 0, .valid = 1 };
			plat_lmc_map[1] = (struct ras_dram_lmc_map) {
				.valid = 0 };
			plat_lmc_map[2] = (struct ras_dram_lmc_map) {
				.lmc = 2, .mcc = 0, .lmcoe = 1, .valid = 1 };
		} else {
			ERROR("RAS support mis-configured for this chip.\n");
		}

		once++;
	}

	if (lmc > MAX_LMC || !plat_lmc_map[lmc].valid) {
		ERROR("Unsupported LMC%d map requested\n", lmc);
		return NULL;
	}

	return &plat_lmc_map[lmc];
}

static int mcc_lmc(int mcc, int lmcoe)
{
	int lmc;
	struct ras_dram_lmc_map *lm = ras_dram_get_lmc_map(0);

	for (lmc = 0; !!lm && (lmc < MAX_LMC); lmc++, lm++)
		if (lm->valid && lm->mcc == mcc && lm->lmcoe == lmcoe)
			return lmc;
	return -1;
}

void __arm_err_nn(int thresh, uint64_t fr_r, uint64_t ctlr_r, uint64_t misc0_r)
{
	union cavm_lmcx_ras_err00misc0 misc0
		= { .u = octeontx_read64(misc0_r) };
	union cavm_lmcx_ras_err00fr fr
		= { .u = octeontx_read64(fr_r) };
	union cavm_lmcx_ras_err00ctlr ctlr
		= { .u = octeontx_read64(ctlr_r) };
	union cavm_lmcx_ras_err00ctlr octlr = ctlr;
	union cavm_lmcx_ras_err00misc0 omisc0 = misc0;
	size_t maxval;

	if (fr.s.dui > 1)
		ctlr.s.dui = 1;
	if (fr.s.dui > 2)
		ctlr.s.wdui = 1;
	if (fr.s.cec) {
		/* calculate max value of counter */
		misc0.s.cec = -1l;
		maxval = misc0.s.cec;
		misc0.s.cec = maxval + 1 - thresh;
	}
	if (fr.s.cfi > 1)
		ctlr.s.cfi = 1;
	if (fr.s.fi > 1)
		ctlr.s.fi = 1;
	if (fr.s.ue > 1)
		ctlr.s.ue = 1;
	if (fr.s.ui > 1)
		ctlr.s.ui = 1;
	if (fr.s.ed > 1)
		ctlr.s.ed = 1;

	if (octlr.u != ctlr.u)
		octeontx_write64(ctlr_r, ctlr.u);
	if (omisc0.u != misc0.u)
		octeontx_write64(misc0_r, misc0.u);
}

static int ras_print_syndrome(char *str, int len, uint8_t syn,
			      union cavm_mccx_lmcoex_ras_int ras_int,
			      union cavm_mccx_lmcoex_ras_err00misc0 misc0,
			      int beat)
{
	int bytebit = ecc_syndrome_to_bytebit[syn];

	if (bytebit) {
		if (ras_int.s.err03 || ras_int.s.err02) {
			if (ras_int.s.err03)
				return snprintf(str, len, "DED=0x%02x",
						0x1 << beat);
			else
				return snprintf(str, len,
						"BYTE %d.%d/0x%02x [%x/%04x]",
						((bytebit >> 4) & 0x0f) - 1,
						bytebit & 0x0f,
						0x1 << beat, misc0.s.of,
						misc0.s.cec);
		} else {
			return snprintf(str, len, "SYN=0x%02x", syn);
		}
	}
	return 0;
}

static void ras_check_double_bit07(int mcc, int lmcoe,
				union cavm_mccx_lmcoex_ras_err00status *status,
				union cavm_mccx_lmcoex_ras_err00addr *erraddr,
				uint64_t *syns_left)
{
	status->u  = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR07STATUS(mcc, lmcoe));
	erraddr->u = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR07ADDR(mcc, lmcoe));
	*syns_left = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR07MISC1(mcc, lmcoe));

	/* Clear Error Interrupts by writing
	 * CAVM_MCCX_LMCOEX_RAS_ERR07STATUS[V]
	 */
	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_ERR07STATUS(mcc, lmcoe), status->u);
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 07, mcc, lmcoe);
}

static void ras_check_double_bit06(int mcc, int lmcoe,
				union cavm_mccx_lmcoex_ras_err00status *status,
				union cavm_mccx_lmcoex_ras_err00addr *erraddr,
				uint64_t *syns_left)
{
	status->u  = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR06STATUS(mcc, lmcoe));
	erraddr->u = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR06ADDR(mcc, lmcoe));
	*syns_left = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR06MISC1(mcc, lmcoe));

	/* Clear Error Interrupts by writing
	 * CAVM_MCCX_LMCOEX_RAS_ERR06STATUS[V]
	 */
	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_ERR06STATUS(mcc, lmcoe), status->u);
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 06, mcc, lmcoe);
}

static void ras_check_double_bit02(int mcc, int lmcoe,
				union cavm_mccx_lmcoex_ras_err00status *status,
				union cavm_mccx_lmcoex_ras_err00addr *erraddr,
				uint64_t *syns_left)
{
	status->u  = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR02STATUS(mcc, lmcoe));
	erraddr->u = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR02ADDR(mcc, lmcoe));
	*syns_left = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR02MISC1(mcc, lmcoe));

	/* Clear Error Interrupts by writing
	 * CAVM_MCCX_LMCOEX_RAS_ERR02STATUS[V]
	 */
	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_ERR02STATUS(mcc, lmcoe), status->u);
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 02, mcc, lmcoe);
}

static void ras_check_double_bit03(int mcc, int lmcoe,
				union cavm_mccx_lmcoex_ras_err00status *status,
				union cavm_mccx_lmcoex_ras_err00addr *erraddr,
				uint64_t *syns_left)
{
	status->u  = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR03STATUS(mcc, lmcoe));
	erraddr->u = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR03ADDR(mcc, lmcoe));
	*syns_left = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR03MISC1(mcc, lmcoe));

	/* Clear Error Interrupts by writing
	 * CAVM_MCCX_LMCOEX_RAS_ERR02STATUS[V]
	 */
	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_ERR03STATUS(mcc, lmcoe), status->u);
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 03, mcc, lmcoe);
}

static void ras_check_single_bit05(int mcc, int lmcoe,
				union cavm_mccx_lmcoex_ras_err00status *status,
				union cavm_mccx_lmcoex_ras_err00addr *erraddr,
				union cavm_mccx_lmcoex_ras_err00misc0 *misc0,
				uint64_t *syns_left)
{
	status->u  = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR05STATUS(mcc, lmcoe));
	erraddr->u = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR05ADDR(mcc, lmcoe));
	misc0->u   = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR05MISC0(mcc, lmcoe));
	*syns_left = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR05MISC1(mcc, lmcoe));

	/* Clear Error Interrupts by writing
	 * CAVM_MCCX_LMCOEX_RAS_ERR05STATUS[V]
	 */
	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_ERR05STATUS(mcc, lmcoe), status->u);

	/* Clear error count */
	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_ERR05MISC0(mcc, lmcoe), 0UL);

	/* Re-arm interrupt */
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 05, mcc, lmcoe);
}

static void ras_check_single_bit04(int mcc, int lmcoe,
				union cavm_mccx_lmcoex_ras_err00status *status,
				union cavm_mccx_lmcoex_ras_err00addr *erraddr,
				union cavm_mccx_lmcoex_ras_err00misc0 *misc0,
				uint64_t *syns_left)
{
	status->u  = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR04STATUS(mcc, lmcoe));
	erraddr->u = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR04ADDR(mcc, lmcoe));
	misc0->u   = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR04MISC0(mcc, lmcoe));
	*syns_left = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR04MISC1(mcc, lmcoe));

	/* Clear Error Interrupts by writing
	 * CAVM_MCCX_LMCOEX_RAS_ERR04STATUS[V]
	 */
	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_ERR04STATUS(mcc, lmcoe), status->u);

	/* Clear error count */
	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_ERR04MISC0(mcc, lmcoe), 0UL);

	/* Re-arm interrupt */
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 04, mcc, lmcoe);
}

static void ras_check_single_bit00(int mcc, int lmcoe,
				union cavm_mccx_lmcoex_ras_err00status *status,
				union cavm_mccx_lmcoex_ras_err00addr *erraddr,
				union cavm_mccx_lmcoex_ras_err00misc0 *misc0,
				uint64_t *syns_left)
{
	status->u  = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR00STATUS(mcc, lmcoe));
	erraddr->u = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR00ADDR(mcc, lmcoe));
	misc0->u   = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR00MISC0(mcc, lmcoe));
	*syns_left = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR00MISC1(mcc, lmcoe));

	/* Clear Error Interrupts by writing
	 * CAVM_MCCX_LMCOEX_RAS_ERR00STATUS[V]
	 */
	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_ERR00STATUS(mcc, lmcoe), status->u);

	/* Clear error count */
	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_ERR00MISC0(mcc, lmcoe), 0UL);

	/* Re-arm interrupt */
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 00, mcc, lmcoe);
}

static void ras_check_single_bit01(int mcc, int lmcoe,
				union cavm_mccx_lmcoex_ras_err00status *status,
				union cavm_mccx_lmcoex_ras_err00addr *erraddr,
				union cavm_mccx_lmcoex_ras_err00misc0 *misc0,
				uint64_t *syns_left)
{
	status->u  = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR01STATUS(mcc, lmcoe));
	erraddr->u = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR01ADDR(mcc, lmcoe));
	misc0->u   = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR01MISC0(mcc, lmcoe));
	*syns_left = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERR01MISC1(mcc, lmcoe));

	/* Clear Error Interrupts by writing
	 * CAVM_MCCX_LMCOEX_RAS_ERR01STATUS[V]
	 */
	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_ERR01STATUS(mcc, lmcoe), status->u);

	/* Clear error count */
	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_ERR01MISC0(mcc, lmcoe), 0UL);

	/* Re-arm interrupt */
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 01, mcc, lmcoe);
}

static inline uint64_t virt_to_phys(uint64_t uaddr)
{
	uint64_t par, pa;
	int elx;

	/* Determine from which EL exception came */
	elx = GET_EL(read_spsr_el3());

	debug_ras("%s(%llx EL%d) -> vttbr_el2:%lx ...\n",
		__func__, uaddr, elx, read_vttbr_el2());

	if (elx == MODE_EL3)
		return uaddr;

	/* Translate to Physical Address using the client mapping */
	switch (elx) {
	default:
		break;
	case MODE_EL2:
		ats1e2r(uaddr);
		break;
	case MODE_EL1:
		if (read_vttbr_el2())
			ats12e1r(uaddr);
		else
			ats1e1r(uaddr);
		break;
	case MODE_EL0:
		if (read_vttbr_el2())
			ats12e0r(uaddr);
		else
			ats1e0r(uaddr);
		break;
	}
	dmbst();
	par = read_par_el1();

	/* Extract Physical Address from PAR */
	pa = (par & (PAR_ADDR_MASK << PAR_ADDR_SHIFT));

	debug_ras("%s(%llx EL%d) -> vttbr_el2:%d pa:%llx f:%llx par:%llx\n",
		__func__, uaddr, elx, !!read_vttbr_el2(), pa,
		(par & PAR_F_MASK), par);
	/* If the translation resulted in fault, return failure */
	if ((par & PAR_F_MASK) != 0)
		return -1;

	pa |= (uaddr & PAGE_SIZE_MASK);

	return pa;
}

static inline uint64_t extract(uint64_t input, int lsb, int width)
{
	uint64_t result = input >> lsb;
	uint64_t mask = width == 64 ? -1ull : ~((~0x0ull) << width);

	result &= mask;
	return result;
}

static int ccs_lmc_hash(uint64_t pa_no_lr, int pa_right, uint64_t *adr_mcs,
			union cavm_ccs_asc_regionx_attr attr)
{
	uint64_t lmc_hash = 0;
	int i;

	/* see HRM 7.2 Internal and DRAM Part Addressing */
	debug_ras("lmc_mode:%d lmc_mask:%x pa_right:%x\n",
		  attr.s.lmc_mode, attr.s.lmc_mask, pa_right);
	switch (attr.s.lmc_mode) {
	case CAVM_CCS_LMC_MODE_E_FLAT_1:
		return 0;
	case CAVM_CCS_LMC_MODE_E_STRIPE_2:
		switch (attr.s.lmc_mask) {
		case 3:
			return pa_right ? 0 : 1;
		case 5:
			return pa_right ? 0 : 2;
		case 6:
			return pa_right ? 1 : 2;
		default:
			return -1;
		}
		break;
	case CAVM_CCS_LMC_MODE_E_STRIPE_3:
		for (i = 0; i < MAX_MCS; i++)
			lmc_hash |= __rxor((adr_mcs[i] & pa_no_lr) >> 8) << i;

		lmc_hash |= ((pa_no_lr & ~0xfffull) >> 8);
		debug_ras("%-40s: 0x%llx\n", "pa_no_lr", pa_no_lr);
		debug_ras("%-40s: 0x%x\n", "pa_right", pa_right);
		debug_ras("%-40s: 0x%llx\n", "lmc_hash", lmc_hash);

		switch (((lmc_hash % 6) << 1) | pa_right) {
# define conc(mod6, right) (((mod6) << 1) | (right))
		case conc(0, 0): return 1;
		case conc(1, 0): return 0;
		case conc(2, 0): return 1;
		case conc(3, 0): return 1;
		case conc(4, 0): return 1;
		case conc(5, 0): return 2;
		case conc(0, 1): return 0;
		case conc(1, 1): return 2;
		case conc(2, 1): return 2;
		case conc(3, 1): return 0;
		case conc(4, 1): return 2;
		case conc(5, 1): return 0;
		default: return -1;
# undef conc
		}
	default: return -1;
	}
	return -1;
}

static int pa_to_lmc(uint64_t pa)
{
	uint64_t pa_top = pa & ~((1 << 24) - 1);
	int region;
	int lmc;
	int nr_ascs;
	union cavm_ccs_const ccs_const;
	union cavm_ccs_adr_ctl adr_ctl;

	debug_ras("Starting PA:%llx to LMC Conversion\n", pa);
	/* Find number of ASC regions on this chip */
	ccs_const.u = CSR_READ(CAVM_CCS_CONST);
	nr_ascs = ccs_const.s.asc;

	for (region = 0; region < nr_ascs; region++) {
		union cavm_ccs_asc_regionx_attr attr;
		uint64_t asc_mcs_en[MAX_MCS];
		uint64_t rend = CSR_READ(CAVM_CCS_ASC_REGIONX_END(region));

		if (pa_top < CSR_READ(CAVM_CCS_ASC_REGIONX_START(region)))
			continue;
		if (pa_top > rend && rend)
			continue;
		attr.u = CSR_READ(CAVM_CCS_ASC_REGIONX_ATTR(region));

		/* check if region is enabled & populated */
		if (!(attr.s.ns_en | attr.s.s_en))
			continue;
		if (!attr.s.lmc_mask)
			continue;

		adr_ctl.u = CSR_READ(CAVM_CCS_ADR_CTL);

		for (int mcs = 0; mcs < MAX_MCS; mcs++)
			asc_mcs_en[mcs] =
				CSR_READ(CAVM_CCS_ADR_MCSX(mcs));
		debug_ras("ASC%d: pa:%llx\n", region, pa);

		/*
		 * lmc hashes on pa_no_lr<42:8>, built from
		 *	pa<42:7> with 1 bit removed (bit r)
		 * example: where md_lr_bit is 2, r is 9
		 * pa<42:1+r>,<r:r>,<r-1:7>,<6:0>
		 * pa<42:10>,<9:9>,<8:7>,<6:0>
		 *                        \--byte-addr within cacheline
		 *                  \--------pa_no_lr<9:8>
		 *            \--------------dropped (but part of pa_right hash)
		 *     \---------------------pa_no_lr<42:10>
		 * So msk_drop = (1<<lr_bit) = 0x200
		 * pa_no_lr_hi = pa & ~(msk_drop | (msk_drop - 1))
		 *             = pa & ~(0x3ff)
		 * pa_no_lr_lo = pa & (msk_drop - 1) & ~(CACHE_LINE_SIZE - 1)
		 *             = pa & 0x1ff & ~0x7f
		 *	       = pa & 0x180
		 * pa_no_lr = pa_no_lr_hi | (pa_no_lr_lo << 1)
		 * Note that pa_no_lr_lo slice is empty when !md_lr_bit.
		 */
		int lr_bit = 7 + adr_ctl.s.md_lr_bit;
		uint64_t msk_drop = (1 << lr_bit);
		uint64_t pnl_hi = pa & ~(msk_drop | (msk_drop - 1));
		uint64_t pnl_lo = pa & (msk_drop - 1)
				& ~(CACHE_WRITEBACK_GRANULE - 1);
		uint64_t pa_no_lr = pnl_hi | (pnl_lo << 1);
		int pa_right = __rxor((adr_ctl.s.md_lr_en << 7) & pa);

		debug_ras("lr_bit:%d/%llx pa:%llx pnl:%llx  p_r:%x\n",
		      lr_bit, msk_drop, pa, pa_no_lr, pa_right);

		lmc = ccs_lmc_hash(pa_no_lr, pa_right, asc_mcs_en, attr);
		return lmc;
	}
	return -1;
}

struct elx_map {
	uint64_t scr_el3;
	uint64_t sctlr_el3;
	uint64_t va;
	uint64_t pa;
	uint64_t vpage;
	uint64_t page;
	int nomap;
	int remap;
	int lmcx;
	uint32_t old_attr;
	void *mapped;
};

static void *map_elx_addr(uint64_t address, struct elx_map *m, int is_phys)
{
	int attr = MT_EXECUTE_NEVER | MT_MEMORY | MT_RW | MT_SECURE;

	/* Non-secure address translation requires SCR_EL3.NS set */
	m->scr_el3 = read_scr_el3();
	write_scr_el3(m->scr_el3 | SCR_NS_BIT);

	/* writing to instruction space requires relaxing  WXN */
	m->sctlr_el3 = read_sctlr_el3();
	write_sctlr_el3(m->sctlr_el3 | SCTLR_WXN_BIT | SCTLR_UWXN_BIT);

	m->va = address;
	m->vpage = (address & ~PAGE_SIZE_MASK);
	m->pa = is_phys ? address : virt_to_phys(address);
	m->page = (m->pa & ~PAGE_SIZE_MASK);

	/* ~0 indicates translation error */
	if (!~m->pa)
		goto fail_sctlr;

	isb();

	/*
	 * if PA was not mapped to a EL3-secure 1:1 VA,
	 * make it so. If there's a collision, mapping was
	 * already in place, and correct, as ATF's mappings
	 * are always 1:1
	 */
	m->nomap = octeontx_mmap_add_dynamic_region_with_sync(
			m->page, m->page,
			PAGE_SIZE, attr);
	/* an existing mapping could be non-RW, so save and replace */
	if (m->nomap) {
		m->old_attr = 0;
		if (!octeontx_xlat_change_mem_attributes(
				m->page, PAGE_SIZE,
				attr, &m->old_attr)) {
			m->nomap = 0;
			m->remap = 1;
			dsbishst();
			tlbivae3is(TLBI_ADDR(m->va));
		} else {
			goto fail_sctlr;
		}
	}

	if (m->nomap)
		m->mapped = (void *) m->va;
	else
		m->mapped = (void *) (m->page | (m->pa & PAGE_SIZE_MASK));

	/* Note that we don't yet account for CCS address conversion
	 * but u-boot is invoked with both region's (S & NS) offset
	 * set to zero, so it's not an issue
	 */
	m->lmcx = pa_to_lmc(m->pa);
	debug_ras("(map nomap:%d va:0x%llx -> el3:%p LMC%d)\n",
		m->nomap, address, m->mapped, m->lmcx);

	return m->mapped;

fail_sctlr:
	write_sctlr_el3(m->sctlr_el3);
	write_scr_el3(m->scr_el3);
	m->lmcx = -1;
	return NULL;
}

static void unmap_elx_addr(struct elx_map *m)
{
	if (m->remap)
		octeontx_xlat_change_mem_attributes(
			m->page, PAGE_SIZE, m->old_attr, NULL);
	else if (!m->nomap)
		octeontx_mmap_remove_dynamic_region_with_sync(
			m->page, PAGE_SIZE);
	dsbishst();
	tlbivae3is(TLBI_ADDR(m->va));

	/* Restore original security model */
	write_sctlr_el3(m->sctlr_el3);
	write_scr_el3(m->scr_el3);

	isb();
	debug_ras("(unmap el3:%p)\n", m->mapped);
}

void ras_rewrite_cacheline(uint64_t physaddr, int secure)
{
	char line[CACHE_WRITEBACK_GRANULE];
	struct elx_map m;
	void *mapped = map_elx_addr(physaddr, &m, 1);
	void *base = (void *)((uint64_t)mapped & ~CACHE_WRITEBACK_GRANULE);

	/*
	 * if PA was not mapped to a EL3-secure 1:1 VA,
	 * make it so. If there's a collision, mapping was
	 * already in place, and correct, as ATF's mappings
	 * are always 1:1
	 */
	if (!mapped) {
		debug_ras("(NOT re-writing mapped %s cacheline @%p)\n",
			secure ? "Secure" : "NS", base);
	} else {
		/* read/rewrite cachline to correct DRAM ECC */
		debug_ras("(re-writing %s cacheline @%p)\n",
			secure ? "Secure" : "NS", base);
		memcpy(line, base, sizeof(line));
		memcpy(base, line, sizeof(line));
		flush_dcache_range((uintptr_t)base, sizeof(line));
	}

	unmap_elx_addr(&m);
}

int lmcoe_ras_check_ecc_errors(int mcc, int lmcoe)
{
	union cavm_mccx_lmcoex_ras_int ras_int;
	union cavm_mccx_lmcoex_ras_int_ena_w1s ras_int_ena;
	/* not just for err00, all errNN have same structure... */
	union cavm_mccx_lmcoex_ras_err00status status;
	union cavm_mccx_lmcoex_ras_err00addr erraddr;
	union cavm_mccx_lmcoex_ras_err00misc0 misc0;
	struct otx2_ghes_err_record *err_rec, *fatal_rec;
	struct otx2_ghes_err_ring *err_ring, *fatal_ring;
	uint64_t paddr;
	char *err_type = "";
	char synstr[256];
	int lmc = mcc_lmc(mcc, lmcoe);
	int dimm = 0;
	int prank = 0;
	int lrank = 0;
	int bank = 0;
	int row = 0;
	int col = 0;
	int beat = 0;
	int fidx = 0;
	uint64_t syns_left = 0;
	uint64_t address = 0;
	uint64_t physaddr = 0;
	uint64_t lmcaddr = 0;
	int report_err;
	int fatal = 0;
	int av = 0;
	int secure = 0;
	int reg, s_reg, ns_reg;
	int repair = 0;
	/* just for debug... */
	static int cnt;
	static uint64_t was[MAX_LMC];

	status.u = 0;
	erraddr.u = 0;
	misc0.u = 0;

	ras_int.u = CSR_READ(CAVM_MCCX_LMCOEX_RAS_ERRGSR0(mcc, lmcoe));

	if (++cnt && (ras_int.u != was[lmc] || !(cnt & (cnt - 1))))
		debug2ras("%s(%d,%d) ras_int:%llx #%d\n",
			__func__, mcc, lmcoe, ras_int.u, cnt);
	was[lmc] = ras_int.u;

	if (!ras_int.u)
		return 0;

	/* use the RAS_INT_ENA bits to say whether we actually had
	 * enabled the interrupts; that is, the ERR0xSTATUS[V] bits are
	 * always current, but we may not desire reporting them
	 */
	ras_int_ena.u = CSR_READ(CAVM_MCCX_LMCOEX_RAS_INT_ENA_W1S(mcc,
							  lmcoe));
	report_err = ((ras_int.u & ras_int_ena.u & 0xFF) != 0);

	/* check for double bit errors only in MCC */
	if (ras_int.s.err07 || ras_int.s.err06 || ras_int.s.err03 ||
	    ras_int.s.err02) {
		if (ras_int.s.err02)
			ras_check_double_bit02(mcc, lmcoe,
					       &status, &erraddr, &syns_left);
		else if (ras_int.s.err03)
			/* more double bit errors from MCC */
			ras_check_double_bit03(mcc, lmcoe,
					       &status, &erraddr, &syns_left);
		else if (ras_int.s.err07) /* scrubber UE */
			ras_check_double_bit07(mcc, lmcoe,
					       &status, &erraddr, &syns_left);
		else if (ras_int.s.err06) /* scrubber UE */
			ras_check_double_bit06(mcc, lmcoe,
					       &status, &erraddr, &syns_left);

		err_type = "double";
		fatal++;
	} else if (ras_int.s.err05 || ras_int.s.err04 || ras_int.s.err01 ||
		   ras_int.s.err00) {
		/* check for single bit errors
		 * else check only when no double bit errs
		 * single bit errors from MCC
		 */
		if (ras_int.s.err00) {
			ras_check_single_bit00(mcc, lmcoe,
					       &status, &erraddr, &misc0,
					       &syns_left);
			/* err_type = "single ERR00"; */
		} else if (ras_int.s.err01) {
			ras_check_single_bit01(mcc, lmcoe,
					       &status, &erraddr, &misc0,
					       &syns_left);
			/* err_type = "single ERR01"; */
		} else if (ras_int.s.err05) /* scrubber CE */
			ras_check_single_bit05(mcc, lmcoe,
					       &status, &erraddr, &misc0,
					       &syns_left);
		else if (ras_int.s.err04) /* scrubber CE */
			ras_check_single_bit04(mcc, lmcoe,
					       &status, &erraddr, &misc0,
					       &syns_left);
		err_type = "single";
	}

	paddr = erraddr.s.paddr;

	debug_ras("int:%llx ena:%llx status:%llx erraddr:%llx/%llx\n",
		ras_int.u, ras_int_ena.u, status.u, erraddr.u, paddr);

	debug_ras("status(av%d v%d ue%d er%d of%d mv%d ce%d de%d"
		  " pn%d uet%d ierr:%x serr:%s)\n",
		(int)status.s.av,
		(int)status.s.v,
		(int)status.s.ue,
		(int)status.s.er,
		(int)status.s.of,
		(int)status.s.mv,
		(int)status.s.ce,
		(int)status.s.de,
		(int)status.s.pn,
		(int)status.s.uet,
		(int)status.s.ierr,
		ras_serr_str[status.s.serr]);

	debug_ras("erraddr: (%s%s paddr:%llx%s %s arem:%llx region:%llx"
		  " ord:%llx off:%llx idx:%llx)\n",
		(erraddr.s.nsec ? "NS" : "Secure"), (erraddr.s.si ? "(?)" : ""),
		paddr, (erraddr.s.ai ? "(?)" : ""),
		(((paddr >> 47) & 1) ? "R" : "L"),
		(paddr >> 43) & 0xf,
		(paddr >> 39) & 0xf,
		(paddr >> 38) & 1,
		(paddr >> 3) & 0x3ffffffffull,
		(paddr >> 0) & 0x7);

	if (!status.s.v)
		return 0;

	av = status.s.av;
	secure = !erraddr.s.nsec;
	repair = av && !erraddr.s.si && !erraddr.s.ai;

	if (av) {
		address = octeontx_bit_extract(erraddr.u, 3, 35)
					* CACHE_WRITEBACK_GRANULE;

		/* Display failing cache line index instead of phase string on
		 * CN9XXX
		 */
		fidx = octeontx_bit_extract(erraddr.u, 0, 3);
	}

	if (!report_err) { /* FIXME? use TRACE to control output or not */
		ERROR("%s: MCC%d.LMC%d: EXTRA ECC: %s @ 0x%016llX"
		      " (I%02llX/E%02llx)\n",
		      __func__, mcc, lmcoe, err_type, address,
		      ras_int.u & 0x0FUL, ras_int_ena.u & 0x0FUL);

		return 0; /* we bypass reporting on this LMC */
	}

	if (av) {
		/* Add memory offset for previous channels */
		lmcaddr = ras_ccs_get_lmc_addr(address, lmc);
		physaddr = ras_ccs_convert_lmc_to_pa(lmcaddr,
					&reg, &s_reg, &ns_reg);
		if (physaddr == (uint64_t)-1) {
			ERROR("MCC%d.LMC%d: ERROR: LMC to Phys conversion"
			      " failed: ADDR: 0x%llx, LMC: 0x%llx\n",
			      mcc, lmcoe, address, lmcaddr);
			av = 0;
		}
	}

	synstr[0] = 0; /* just in case... */
	while (av && syns_left) {
		int stroff = 0;
		/* Check each byte of the syndrome for error data
		 * each byte represents the syndrome data for one of 8 beats
		 * in a transaction
		 *
		 * The syndrome can hold error data for multiple errors
		 * but if the byte is 0 there's no error
		 */
		if (syns_left & 0xFF)
			stroff += ras_print_syndrome(synstr + stroff,
						     sizeof(synstr) - stroff,
						     syns_left & 0xff,
						     ras_int, misc0, beat);
		syns_left >>= 8;
		beat++;
		if (syns_left)
			stroff += snprintf(synstr + stroff,
					   sizeof(synstr) - stroff, ", ");
	}

	if (av) {
		int lmc_num;

		ras_dram_address_extract_info(lmcaddr, &lmc_num, &dimm, &prank,
					      &lrank, &bank, &row, &col);
		if (lmc != lmc_num)
			ERROR("LMC%d or LMC%d - addressing confused\n",
				lmc, lmc_num);
	}

	/*
	 * later, with RAS-compliant kernel, these can be
	 * delivered via linux/cper.h::cper_arm_err_info
	 */
	ERROR("LMC%d: DRAM ECC %s (DIMM%d,Rank%d/%d,Bank%02d,"
	      "Row 0x%05x,Col 0x%04x,FIDX=%d,%s)%s0x%llx%s %s%s%s\n",
	      lmc, err_type, dimm, prank, lrank, bank,
	      row, col, fidx, synstr,
	      (erraddr.s.ai ? "?" : "["), physaddr, (erraddr.s.ai ? "?" : "]"),
	      (erraddr.s.si ? "?" : "("), (secure ? "Secure" : "NS"),
	      (erraddr.s.si ? "?" : ")"));
	if (av && !(secure ? s_reg : ns_reg))
		ERROR("ASC_R%d(ns:%d s:%d) but sec:%d\n",
			reg, ns_reg, s_reg, secure);

	fatal_rec = NULL;
	if (fatal) {
		fatal_rec = otx2_begin_ghes("bert", &fatal_ring);
		if (!fatal_rec)
			ERROR("No fatal error records available\n");
	}

	err_rec = otx2_begin_ghes("mcc", &err_ring);

	/* Ensure fatal error is recorded EVEN if non-fatal ring is full. */
	if (!err_rec && fatal && fatal_rec)
		err_rec = fatal_rec;

	if (err_rec) {
		err_rec->u.mcc.physical_addr = physaddr;
		err_rec->u.mcc.card = lmc;
		err_rec->u.mcc.module = dimm;
		err_rec->u.mcc.bank = bank;
		err_rec->u.mcc.row = row;
		err_rec->u.mcc.column = col;

		err_rec->u.mcc.validation_bits |= (CPER_MEM_VALID_PA |
			CPER_MEM_VALID_CARD | CPER_MEM_VALID_MODULE |
			CPER_MEM_VALID_BANK | CPER_MEM_VALID_ROW |
			CPER_MEM_VALID_COLUMN);

		err_rec->severity = fatal ? CPER_SEV_FATAL : CPER_SEV_CORRECTED;

		snprintf(err_rec->fru_text, sizeof(err_rec->fru_text),
			 "LMC%d: DIMM%d,Rank%d/%d,Bank%02d", lmc, dimm, prank,
			 lrank, bank);

		/* If fatal error, copy it and update fatal ring */
		if (fatal && fatal_rec) {
			if (fatal_rec != err_rec)
				memcpy(fatal_rec, err_rec, sizeof(*fatal_rec));
			if (++fatal_ring->head >= fatal_ring->size)
				fatal_ring->head = 0;
			/* Ensure ring memory is updated prior to reset */
			dmbsy();
			l2c_flush();
			debug_ras("fatal ring: %p, hd/tl/sz %d/%d/%d\n",
				  fatal_ring, fatal_ring->head,
				  fatal_ring->tail, fatal_ring->size);
		}

		/*
		 * The 'err_rec' MAY == the 'fatal_rec' (see above).
		 * If so, do not issue the non-fatal notification.
		 */
		if (err_rec != fatal_rec)
			otx2_send_ghes(err_rec, err_ring,
				       OCTEONTX_SDEI_RAS_MCC_EVENT);
	}

	if (av && !fatal && repair)
		ras_rewrite_cacheline(physaddr, secure);

	if (av && !secure && err_rec)
		fatal = 0;

	return fatal;
}

static int lmcoe_ras_int(int lmcoe)
{
	if (cavm_is_model(OCTEONTX_CNF95XX))
		return CAVM_MCC_INT_VEC_E_LMCOEX_RAS_INT_CNF95XX(lmcoe);
	if (cavm_is_model(OCTEONTX_CN96XX))
		return CAVM_MCC_INT_VEC_E_LMCOEX_RAS_INT_CN96XX(lmcoe);
	if (cavm_is_model(OCTEONTX_CN98XX))
		return CAVM_MCC_INT_VEC_E_LMCOEX_RAS_INT_CN98XX(lmcoe);
	if (cavm_is_model(OCTEONTX_LOKI))
		return CAVM_MCC_INT_VEC_E_LMCOEX_RAS_INT_LOKI(lmcoe);
	if (cavm_is_model(OCTEONTX_F95MM))
		return CAVM_MCC_INT_VEC_E_LMCOEX_RAS_INT_F95MM(lmcoe);
	if (cavm_is_model(OCTEONTX_F95O))
		return CAVM_MCC_INT_VEC_E_LMCOEX_RAS_INT_F95O(lmcoe);
	return -1;
};

/* some events are indexed by linear LMC */
int lmc_ras_setup(int lmc_no)
{
	uint64_t bar4;
	uint64_t vaddr;
	uint64_t vctl;
#if !RAS_EXTENSION
	int rc = 0;
#endif /* !RAS_EXTENSION */
	union cavm_lmcx_ras_int_ena_w1s int_ena;
	static int irq = -1;
	int vec;

	debug_ras("%s(%d)\n", __func__, lmc_no);

	if ((unsigned int)lmc_no > MAX_LMC)
		return -EINVAL;

	bar4 = CAVM_LMC_BAR_E_LMCX_PF_BAR4(lmc_no);
	vec = CAVM_LMC_INT_VEC_E_RAS_INT;

	CSR_WRITE(CAVM_LMCX_RAS_INT_ENA_W1C(lmc_no),
		  ~0ULL);

	vaddr = bar4 + 0x10 * vec;
	vctl = vaddr + 0x8;

	if (irq < 0 || LMC_SPI_IRQS > 1) {
		irq = LMC_SPI_IRQ(lmc_no);
#if !RAS_EXTENSION
		rc = octeontx_ehf_register_irq_handler(irq, otx2_lmc_isr);
		if (rc) {
			debug_ras("e?%d otx2_lmc_isr(%x), mcc: %d\n",
			     rc, irq, mcc);
			return rc;
		}
#endif /* !RAS_EXTENSION */
	}

	debug_ras("Enable MSIX%d for LMC%d, irq:%d\n",
	     vec, lmc_no, irq);

	octeontx_write64(vctl, irq);
	octeontx_write64(vaddr, CAVM_GICD_SETSPI_SR | 1);

	debug_ras("addr: 0x%llx, ctl: 0x%llx\n",
	     octeontx_read64(vaddr), octeontx_read64(vctl));

	CSR_WRITE(CAVM_LMCX_RAS_INT_ENA_W1C(lmc_no), ~0ULL);

	arm_err_nn(1, CAVM_LMCX_RAS_ERR, 00, lmc_no);

	int_ena.u = 0;
	int_ena.s.err00 = 1;
	CSR_WRITE(CAVM_LMCX_RAS_INT_ENA_W1S(lmc_no), int_ena.u);

	return 0;
}

/* others indexed by (MCC, odd/even) */
int lmcoe_ras_setup(int mcc, int lmcoe)
{
	uint64_t bar4 = CAVM_MCC_BAR_E_MCCX_PF_BAR4(mcc);
	uint64_t vaddr;
	uint64_t vctl;
#if !RAS_EXTENSION
	int rc = 0;
#endif /* !RAS_EXTENSION */
	union cavm_mccx_lmcoex_ras_int_ena_w1s int_ena;
	static int irq = -1;

	debug_ras("%s(%d,%d)\n", __func__, mcc, lmcoe);

	int vec = lmcoe_ras_int(lmcoe);

	if (vec < 0)
		return -EINVAL;

	/* the non-LMCOE events... */
	lmc_ras_setup(mcc_lmc(mcc, lmcoe));

	CSR_WRITE(CAVM_MCCX_LMCOEX_RAS_INT_ENA_W1C(mcc, lmcoe),
		  ~0ULL);

	vaddr = bar4 + 0x10 * vec;
	vctl = vaddr + 0x8;

	if (irq < 0 || MCC_SPI_IRQS > 1) {
		irq = MCC_SPI_IRQ(vec + mcc * 10);
#if !RAS_EXTENSION
		rc = octeontx_ehf_register_irq_handler(irq, otx2_mcc_isr);
		if (rc) {
			debug_ras("e?%d otx2_mcc_isr(%x), mcc: %d\n",
			     rc, irq, mcc);
			return rc;
		}
#endif /* !RAS_EXTENSION */
	}

	debug_ras("Enable MSIX%d for MCC%d.LMCOE.%d, irq:%d\n",
	     vec, mcc, lmcoe, irq);

	CSR_WRITE(
		CAVM_MCCX_LMCOEX_RAS_INT_ENA_W1C(mcc, lmcoe),
		~0ULL);

	octeontx_write64(vctl, irq);
	octeontx_write64(vaddr, CAVM_GICD_SETSPI_SR | 1);

	debug_ras("addr: 0x%llx, ctl: 0x%llx\n",
	     octeontx_read64(vaddr), octeontx_read64(vctl));

	CSR_WRITE(
		CAVM_MCCX_LMCOEX_RAS_INT_ENA_W1C(mcc, lmcoe),
		~0ULL);

	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 00, mcc, lmcoe);
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 01, mcc, lmcoe);
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 02, mcc, lmcoe);
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 03, mcc, lmcoe);
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 04, mcc, lmcoe);
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 05, mcc, lmcoe);
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 06, mcc, lmcoe);
	arm_err_nn(1, CAVM_MCCX_LMCOEX_RAS_ERR, 07, mcc, lmcoe);

	int_ena.u = 0;
	int_ena.s.err00 = 1;
	int_ena.s.err01 = 1;
	int_ena.s.err02 = 1;
	int_ena.s.err03 = 1;
	int_ena.s.err04 = 1;
	int_ena.s.err05 = 1;
	int_ena.s.err06 = 1;
	int_ena.s.err07 = 1;

	CSR_WRITE(
		CAVM_MCCX_LMCOEX_RAS_INT_ENA_W1S(mcc, lmcoe),
		int_ena.u);

	return 0;
}

int lmcoe_scrubber_setup(int mcc, int lmcoe)
{
	union cavm_mccx_lmcoex_bscrub_cfg cfg;
	union cavm_mccx_lmcoex_bscrub_cfg2 cfg2;
	union cavm_lmcx_config lmc_cfg;
	uint64_t a_start, a_end;
	size_t lmc_addr_bits;

	cfg.u = CSR_READ(CAVM_MCCX_LMCOEX_BSCRUB_CFG(mcc, lmcoe));
	cfg2.u = CSR_READ(CAVM_MCCX_LMCOEX_BSCRUB_CFG2(mcc, lmcoe));

	/* refer to LMCX_CONFIG[PBANK_LSB] */
#define LMC_PBANK_LSB_BASE_ADDR (28)
	lmc_cfg.u = CSR_READ(CAVM_LMCX_CONFIG(mcc_lmc(mcc, lmcoe)));
	lmc_addr_bits = LMC_PBANK_LSB_BASE_ADDR + lmc_cfg.s.pbank_lsb;

	/* set scrubber address range to that of memory attached to LMC */
	a_start = 0;
	a_end = BIT_64(lmc_addr_bits) - 1;

	debug_ras("bscrub range was %llx-%llx, want %llx-%llx\n",
		(uint64_t)cfg.s.start_address, (uint64_t)cfg2.s.stop_address,
		a_start, a_end);

	/*
	 * background-scrubber idle count, currently default from HRM,
	 * perhaps should scale to SCLK, with later adjustment via SMC
	 */
	cfg.s.bs_idle_cnt = 0x4f;

	if (cfg.s.enable || cfg.s.busy) {
		ERROR("%s(%d,%d) sees en:%d busy:%d scrubber\n",
			__func__, mcc, lmcoe,
			cfg.s.enable, cfg.s.busy);
		return -1;
	}

	cfg.s.start_address = a_start;
	cfg2.s.stop_address = a_end;

	CSR_WRITE(CAVM_MCCX_LMCOEX_BSCRUB_CFG2(mcc, lmcoe), cfg2.u);
	cfg.s.enable = 1;
	CSR_WRITE(CAVM_MCCX_LMCOEX_BSCRUB_CFG(mcc, lmcoe), cfg.u);

	return 0;
}

static inline void flush_dcache_all(int op)
{
	dcsw_op_all(op);
	l2c_flush();
	__asm__ volatile("ic iallu\n"
			 "isb\n");
	dsbsy();
}

static inline void dram_set_poison(int lmcx, void *pa, int bit, int enable)
{
	uint64_t addr = (uint64_t) pa;
	union cavm_lmcx_ecc_parity_test parity_test;

	if (bit >= 0)
		bit = (addr & 0xf) * 8 + bit;

	if (!enable) {
		CSR_WRITE(CAVM_LMCX_CHAR_MASK0(lmcx), 0);
		CSR_WRITE(CAVM_LMCX_CHAR_MASK2(lmcx), 0);
		parity_test.u = CSR_READ(CAVM_LMCX_ECC_PARITY_TEST(lmcx));
		parity_test.s.ecc_corrupt_idx = 0;
		parity_test.s.ecc_corrupt_ena = 0;
		CSR_WRITE(CAVM_LMCX_ECC_PARITY_TEST(lmcx), parity_test.u);
		CSR_READ(CAVM_LMCX_ECC_PARITY_TEST(lmcx));
		return;
	}
	if (bit >= 0 && bit < 64)
		CSR_WRITE(CAVM_LMCX_CHAR_MASK0(lmcx), 1ull << bit);
	else
		CSR_WRITE(CAVM_LMCX_CHAR_MASK0(lmcx), (bit < 0) ? 3ull : 0ull);

	CSR_WRITE(CAVM_LMCX_CHAR_MASK2(lmcx),
		((bit >= 64) ? (1ull << (bit - 64)) : 0ull));

	parity_test.u = CSR_READ(CAVM_LMCX_ECC_PARITY_TEST(lmcx));
	parity_test.s.ecc_corrupt_idx = (addr & 0x7f) >> 4;
	parity_test.s.ecc_corrupt_ena = 1;
	CSR_WRITE(CAVM_LMCX_ECC_PARITY_TEST(lmcx), parity_test.u);
	CSR_READ(CAVM_LMCX_ECC_PARITY_TEST(lmcx));
	dmbsy();
}

static int dram_inject_error(struct elx_map *m, int bit, int flags)
{
	uint64_t data = 0;
	int reread = flags & 0x100;

	if (m->mapped)
		data = *(uint64_t *)m->mapped;
	__asm__ volatile ("dc civac, %0" : : "r"(m->mapped) : "memory");

	/* drain caches, so nothing spills while poisoning */
	flush_dcache_all(DCCSW);

	/* Enable error injection */
	dram_set_poison(m->lmcx, m->mapped, bit, true);

	/* Perform a write and flush it to DRAM */
	if (m->mapped)
		*(uint64_t *)m->mapped = data;

	/* clean/inval L1C by VA */
	dccivac((uint64_t)m->mapped);
	/* Write-back invalidate last-level-cache CVMCACHE_WBI_L3 */
	__asm__ volatile ("sys #0,c11,c1,#2, %0" : : "r"(m->pa) : "memory");
	dmbsy();

	/* Disable error injection */
	dram_set_poison(m->lmcx, m->mapped, bit, false);

	if (m->mapped) {
		dcivac((uint64_t)m->mapped);
		printf("Injected error va:0x%llx, pa:%p, lmc%d (mcc%d)\n",
			m->va, m->mapped, m->lmcx,
			plat_lmc_map[m->lmcx].mcc);

		if (reread) {
			/* Read back the data which should now cause an error */
			debug_ras("re-reading...\n");
			data = *(uint64_t *)m->mapped;
		}
	}

	return 0;
}

/* Level-1 cache can inject single/double errors */
static inline int l1c_ecc_inject(void *pa, int bits)
{
	union ccs_pic_ctl pc;

	pc.u = CSR_READ(CAVM_CCS_PIC_CTL);
	pc.s.error_inject = bits;
	CSR_WRITE(CAVM_CCS_PIC_CTL, pc.u);
	dsbsy();

	return 0;
}

/*
 * L3 = last-level-cache, sometimes mis-labeled L2C
 * can only inject double-bit errors (bits = 3)
 */
static inline int llc_ecc_inject(void *pa, int bits)
{
	union ccs_tad_ctl tc;

	if (bits && bits != 3)
		return -EINVAL;

	tc.u = CSR_READ(CAVM_CCS_TAD_CTL);
	tc.s.frclckdbe = (bits == 3);
	CSR_WRITE(CAVM_CCS_TAD_CTL, tc.u);
	dsbsy();

	/* when clearing, drop L1$ copy */
	if (!bits)
		dcivac((uint64_t)pa);

	return 0;
}

static inline int cache_ecc_inject(int level, int icache, void *pa, int bits)
{
	/* flush old state to DRAM */
	if (bits)
		__asm__ volatile("dc civac, %0" :: "r"(pa));

	bits &= 3; /* 1 or 2 for SBE, -1 becomes 3 for SBE */

	switch (level) {
	case 1:
		return l1c_ecc_inject(pa, bits);
	case 2:
		if (!icache)
			return -EINVAL;
		/* flush I-cache, so L2-Icache refills from LLC */
		__asm__ volatile ("ic iallu");
		/* fallthrough */
	case 3:
		return llc_ecc_inject(pa, bits);
	default:
		return -EINVAL;
	}
}

static int cache_inject_error(int level, int icache, struct elx_map *m,
			      int bit, int flags)
{
	int ret;

	/* drain caches, so nothing spills while poisoning */
	flush_dcache_all(DCCSW);

	/* which bits to corrupt */
	if (bit < 0)
		ret = cache_ecc_inject(level, icache, m->mapped, 3);
	else if (!bit)
		ret = cache_ecc_inject(level, icache, m->mapped, 1);
	else
		ret = cache_ecc_inject(level, icache, m->mapped, 2);

	if (ret == -EINVAL) {
		debug_ras("Error: Level-%d %c-Cache %s not implemented\n",
		       level, "DI"[icache], bit < 0 ? "DBE" : "SBE");
		return ret;
	}

	/* Perform a write and flush it to cache */
	*(volatile uint64_t *)m->mapped = *(volatile uint64_t *)m->mapped;
	dmbsy();

	/* Disable error injection */
	cache_ecc_inject(level, icache, m->mapped, 0);

	debug_ras("Injected error address 0x%llx, %c-cache level-%d\n",
	       m->va, "di"[icache], level);

	return ret;
}

#ifdef ECC_EL3_TEST
static void just_ret(void) { __asm__("ret"); }
#endif

int64_t plat_ras_lmc_inject(u_register_t x2, u_register_t x3,
				u_register_t x4)
{
	uint64_t address = x2;
	uint64_t aligned_address;
	int flags = x3;
	int bit = (flags & 8) ? -1 : (flags & 0x7);
	int cache = ((flags >> 4) & 7);
	int icache = (flags & 0x80);
	int reread = (flags & 0x100);
	int nomap = (flags & 0x200);
	struct elx_map m;
	int ret;
#ifdef ECC_EL3_TEST
	uint64_t __attribute__((unused)) data;
#endif
	int read_own_code = 0;
	memset(&m, 0, sizeof(m));

	debug_ras("%s(a:%lx f:%lx x4:%lx)\n", __func__, x2, x3, x4);

#ifdef ECC_EL3_TEST
	/* special case, address 3/7 denote EL3 i/d reread point */
	if ((address & ~4) == 3 && ((bit >= 0 || DEBUG_RAS))) {

		if (address & 4)
			reread = 1;
		else
			read_own_code = 1;

		address = (uint64_t) &el3_test_target;
		address += CACHE_WRITEBACK_GRANULE;
		address &= ~CACHE_WRITEBACK_GRANULE;
		*(uint64_t *) address = *(uint64_t *) just_ret;
		nomap = 1;
	}
#endif

	aligned_address = address & ~7ull;

	map_elx_addr(aligned_address, &m, nomap);

	if (m.lmcx < 0 || !plat_lmc_map[m.lmcx].valid) {
		ERROR("%s(0x%llx, %d): ERROR: Could not map to LMC\n",
		       __func__, address, bit);
		debug_ras("LMC%d l%d m%d oe%d v%d\n",
			m.lmcx, plat_lmc_map[m.lmcx].lmc,
			plat_lmc_map[m.lmcx].mcc,
			plat_lmc_map[m.lmcx].lmcoe,
			plat_lmc_map[m.lmcx].valid);
		unmap_elx_addr(&m);
		return -1;
	}

	// FIXME: much common code from both can be moved back to here ..
	if (cache)
		ret = cache_inject_error(cache, icache, &m, bit, flags);
	else
		ret = dram_inject_error(&m, bit, flags);

	/* Read back the data which should now cause an error */
	if (read_own_code) {
		void (*target)(void) = (void *)aligned_address;

		debug_ras("re-reading poisoned EL3 code...\n");
		octeontx_xlat_change_mem_attributes(
			m.page, PAGE_SIZE, MT_CODE, NULL);
		target();
	} else if (reread) {
		debug_ras("re-reading poisoned EL3 data...\n");
		data = *(volatile uint64_t *)aligned_address;
	}


	unmap_elx_addr(&m);

	return ret;
}

/*
 * Placeholder for real ISR.  This is required by RAS infrastructure changes.
 */
uint64_t otx2_lmc_isr(uint32_t id, uint32_t flags, void *cookie)
{
	int lmc = (uint64_t) cookie;
	union cavm_lmcx_ras_err00status status;
	union cavm_lmcx_ras_err00addr erraddr;
	union cavm_lmcx_ras_err00misc0 m0;
	union cavm_lmcx_ras_err00misc1 m1;

	status.u  = CSR_READ(CAVM_LMCX_RAS_ERR00STATUS(lmc));
	erraddr.u = CSR_READ(CAVM_LMCX_RAS_ERR00ADDR(lmc));
	m0.u = CSR_READ(CAVM_LMCX_RAS_ERR00MISC0(lmc));
	m1.u = CSR_READ(CAVM_LMCX_RAS_ERR00MISC1(lmc));

	CSR_WRITE(CAVM_LMCX_RAS_ERR00STATUS(lmc), status.u);
	arm_err_nn(1, CAVM_LMCX_RAS_ERR, 00, lmc);
	debug2ras("%s lmc%d st:%llx ad:%llx m0:%llx m1:%llx\n",
		__func__, lmc, status.u, erraddr.u, m0.u, m1.u);

	return 0;
}

#if RAS_EXTENSION
int otx2_lmc_probe(const struct err_record_info *info, int *probe_data)
{
	union cavm_lmcx_ras_int lmc_ras_int;
	int lmc;

	for (lmc = 0; lmc < MAX_LMC; lmc++) {
		if (!plat_octeontx_is_lmc_enabled(lmc))
			continue;
		lmc_ras_int.u = CSR_READ(CAVM_LMCX_RAS_INT(lmc));
		if (lmc_ras_int.u) {
			*probe_data = lmc;
			return 1;
		}
	}

	return 0;
}
#endif /* RAS_EXTENSION */
