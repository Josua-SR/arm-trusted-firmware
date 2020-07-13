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
#include <plat_scfg.h>
#include <octeontx_security.h>
#include <octeontx_dram.h>
#include <octeontx_utils.h>

#include "cavm-csrs-ap.h"
#include "cavm-csrs-ccs.h"
#include "cavm-csrs-iobn.h"
#include "cavm-csrs-mcc.h"
#include "cavm-csrs-pccpf.h"
#include "cavm-csrs-pem.h"
#include "cavm-csrs-smmu.h"

struct ccs_region {
	ccs_region_index_t  number;
	unsigned long start;
	unsigned long end;
	unsigned int  secure;
	unsigned int  mandatory;
};

#define LAST_CCS_REGION (~0U)
#define MCC_CONFIG_DIS_TADPSN_BIT	BIT(1)
#define SET_BY_BDK	(~0UL)

#define CCS_ASC_REGION_MASK

struct ccs_region ccs_map [] = {
	/* Secure non preserve region */
	{
		.number    = SECURE_NONPRESERVE,
		.start     = TZDRAM_BASE,
		.end       = TZDRAM_BASE + TZDRAM_SIZE - 1,
		.secure    = 1,
		.mandatory = 1,
	},
	/* Non-secure non preserve region */
	{
		.number    = NSECURE_NONPRESERVE,
		.start     = TZDRAM_BASE + TZDRAM_SIZE,
		.end       = SET_BY_BDK,
		.secure    = 0,
		.mandatory = 1,
	},
	/* Secure preserve region */
	{
		.number    = SECURE_PRESERVE,
		.start     = SET_BY_BDK,
		.end       = SET_BY_BDK,
		.secure    = 1,
		.mandatory = 0,
	},
	/* Non-secure preserve region */
	{
		.number    = NSECURE_PRESERVE,
		.start     = SET_BY_BDK,
		.end       = SET_BY_BDK,
		.secure    = 0,
		.mandatory = 0,
	},
	{
		.number  = LAST_CCS_REGION,
	},
};

/* Returns start and size info of the region
 * Assumes region definition used by BDK and ccs_map in ATF are in sync
 */
uint64_t ccs_region_get_info(ccs_region_index_t index, uint64_t *start)
{
	union cavm_ccs_asc_regionx_attr ccs_asc_attr;
	uint64_t reg_start, reg_end;

	if (index >= CCS_REGION_IDX_MAX)
		return 0;

	ccs_asc_attr.u = CSR_READ(CAVM_CCS_ASC_REGIONX_ATTR(index));
	reg_start = CSR_READ(CAVM_CCS_ASC_REGIONX_START(index));
	reg_end = CSR_READ(CAVM_CCS_ASC_REGIONX_END(index));

	/* Verify if user hasnt configured yet or misconfigured */
	if ((!ccs_asc_attr.s.s_en && !ccs_asc_attr.s.ns_en) ||
	    reg_end < reg_start) {
		return 0;
	}

	/* REGIONX_END always reports lower 24 bits as 0 */
	reg_end |= 0xffffff;

	/* Retrun start and size */
	*start = reg_start;
	return reg_end - reg_start + 1;
}

uint64_t memory_region_get_info(int index, uint64_t *start)
{
	return ccs_region_get_info(index, start);
}

/*
 * This is workaround for errata NIX-31533
 */
static void disable_poison(void)
{
	uint64_t mcc_config;
	int i;

	for (i = 0; i < plat_octeontx_scfg->mcc_count; i++) {
		mcc_config = CSR_READ(CAVM_MCCX_CONFIG(i));
		mcc_config |= MCC_CONFIG_DIS_TADPSN_BIT;
		CSR_WRITE(CAVM_MCCX_CONFIG(i), mcc_config);
	}
}

/* Flush the L2 Cache */
void l2c_flush(void)
{
	/* Select the L2 cache */
	union cavm_ap_csselr_el1 csselr_el1;
	union cavm_ap_ccsidr_el1 ccsidr_el1;
	union cavm_ccs_const ccs_const;

	unsigned int sets, ways, clusters, tads;
	int cluster, tad, tg, way, index;

	ccs_const.u = CSR_READ(CAVM_CCS_CONST);
	clusters = ccs_const.s.clu;
	tads = ccs_const.s.tadclu;
	csselr_el1.s.level = 2;
	__asm__ volatile ("msr csselr_el1, %0" : : "r"((uint64_t)csselr_el1.u));
	__asm__ volatile ("mrs %0, ccsidr_el1" : "=&r"(ccsidr_el1.u));

	sets = ((ccsidr_el1.s.numsets + 1) / tads / clusters);
	ways = (ccsidr_el1.s.associativity + 1);
	for (cluster = 0; cluster < clusters; cluster++) {
		for (tad = 0; tad < tads; tad++) {
			for (tg = 0; tg < 2; tg++) {
				for (way = 0; way < ways; way++) {
					for (index = 0; index < sets; index++) {
						uint64_t encoded = 0;

						encoded |= cluster << 25;
						encoded |= tad << 24;
						encoded |= tg << 23;
						encoded |= way << 18;
						encoded |= index << 8;
						__asm__ volatile
				("sys #0,c11,c0,#5, %0" : : "r" (encoded));
					}
				}
			}
		}
	}
}

void octeontx_security_setup(void)
{
	union cavm_ccs_asc_regionx_attr ccs_asc_attr;
	union cavm_ccs_asc_regionx_start bdk_ccs_asc_start, atf_ccs_asc_start;
	union cavm_ccs_asc_regionx_end bdk_ccs_asc_end, atf_ccs_asc_end;
	struct ccs_region *region = ccs_map;
	uint64_t midr;
	uint64_t start, end;

	midr = read_midr();

	if (IS_OCTEONTX_PN(midr, T96PARTNUM)
	    || IS_OCTEONTX_VAR(midr, F95PARTNUM, 1)
	    || IS_OCTEONTX_PASS(midr, LOKIPARTNUM, 1, 0)
	    || IS_OCTEONTX_PASS(midr, T98PARTNUM, 1, 0))
		disable_poison();

	while (region->number != LAST_CCS_REGION) {
		ccs_asc_attr.u = CSR_READ(CAVM_CCS_ASC_REGIONX_ATTR(
							region->number));
		if (ccs_asc_attr.u == 0) {
			/*
			 * If region is no mandatory and is not setup by BDK
			 * skip it initialization.
			 */
			if (!region->mandatory) {
				region++;
				continue;
			}
			NOTICE("Mandatory region %d is not setup by BDK\n",
				region->number);
			/*
			 * Try to use configuration of first region. It should
			 * contain proper LMC_MODE setup.
			 */
			if (region->number != 0)
				ccs_asc_attr.u = CSR_READ(
						     CAVM_CCS_ASC_REGIONX_ATTR(
							       region->number));
		}

		/*
		 * ATF require some ASC regions to have ceratian start or end.
		 * If BDK configure regions in different way print warning and
		 * reconfigure region.
		 */
		if (region->start != SET_BY_BDK) {
			bdk_ccs_asc_start.u =
				CSR_READ(CAVM_CCS_ASC_REGIONX_START(
							region->number));
			atf_ccs_asc_start.u = region->start;
			if (bdk_ccs_asc_start.s.addr !=
			    atf_ccs_asc_start.s.addr) {
				NOTICE(
				      "Start of ASC region %d is different for BDK(0x%x) and ATF(0x%x)\n",
				      region->number, bdk_ccs_asc_start.s.addr,
				      atf_ccs_asc_start.s.addr);
				NOTICE("ATF is using its own ASC config\n");
				CSR_WRITE(CAVM_CCS_ASC_REGIONX_START(
					region->number), atf_ccs_asc_start.u);
			}
		}
		if (region->end != SET_BY_BDK) {
			bdk_ccs_asc_end.u = CSR_READ(CAVM_CCS_ASC_REGIONX_END(
							region->number));
			atf_ccs_asc_end.u = region->end;
			if (bdk_ccs_asc_end.s.addr != atf_ccs_asc_end.s.addr) {
				NOTICE(
				      "End of ASC region %d is different for BDK(0x%x) and ATF(0x%x)\n",
				      region->number, bdk_ccs_asc_end.s.addr,
				      atf_ccs_asc_end.s.addr);
				NOTICE("ATF is using its own ASC config\n");
				CSR_WRITE(CAVM_CCS_ASC_REGIONX_END(
					region->number), atf_ccs_asc_end.u);
			}
		}

		/*
		 * BDK cannot set non-secure regions, do it here.
		 */
		ccs_asc_attr.s.s_en  = region->secure;
		ccs_asc_attr.s.ns_en = !region->secure;

#if !defined PLAT_ARMTRACEBUF_NO_WT
		union cavm_ccs_asc_regionx_attr ccs_asc_attr_wt;

		INFO("Configuring Secure preserve region as writethrough\n");
		if (region->number == SECURE_PRESERVE) {
			ccs_asc_attr_wt.u = ccs_asc_attr.u;
			/* Enable writethrough */
			ccs_asc_attr_wt.s.write_through = 1;
			CSR_WRITE(CAVM_CCS_ASC_REGIONX_ATTR(region->number),
				  ccs_asc_attr_wt.u);
		} else {
			CSR_WRITE(CAVM_CCS_ASC_REGIONX_ATTR(region->number),
				  ccs_asc_attr.u);
		}
#else
		CSR_WRITE(CAVM_CCS_ASC_REGIONX_ATTR(region->number),
			  ccs_asc_attr.u);
#endif

		VERBOSE("Mark memory region %d:: %llx to %llx as %ssecure (%llx)\n",
			region->number,
			CSR_READ(CAVM_CCS_ASC_REGIONX_START(region->number)),
			CSR_READ(CAVM_CCS_ASC_REGIONX_END(region->number)) | 0xffffff,
			region->secure ? "" : "non-",
			CSR_READ(CAVM_CCS_ASC_REGIONX_ATTR(region->number)));

		start = CSR_READ(CAVM_CCS_ASC_REGIONX_START(region->number));
		end = CSR_READ(CAVM_CCS_ASC_REGIONX_END(region->number)) | 0xffffff;
		if ((end - start)) {
			if (region->number == SECURE_PRESERVE)
				NOTICE("Secure Preserved Memory Region: %llx to %llx (%lldKB)\n",
				       start, end,
				       ((end - start + 1) / 1024));
			if (region->number == NSECURE_PRESERVE)
				NOTICE("Non-Secure Preserved Memory Region: %llx to %llx (%lldKB)\n",
				       start, end,
				       ((end - start + 1) / 1024));
		}
		region++;
	}

	/* Display Non-Secure Preserved Memory Region adjustments */
	if (plat_octeontx_bcfg->bert_area.size) {
		start = plat_octeontx_bcfg->bert_area.base;
		end = start + plat_octeontx_bcfg->bert_area.size - 1,
		NOTICE("BERT area: %llx to %llx (%lldKB)\n", start, end,
		       ((end - start + 1) / 1024));
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
 * This function configures IOBN to grant access for eMMC controller
 * to secure/non-secure memory based on input parameter passed
 */
void octeontx_configure_mmc_security(int secure)
{
	/*
	 * rsl_idx - PCC function number for the RSL device
	 * (stream ID<7:0>)
	 * bus_idx - Stream's bus number (stream_id<15:8>).
	 */
	uint64_t rsl_idx = CAVM_PCC_DEV_CON_E_MIO_EMM & 0xFF;
	uint64_t bus_idx = (CAVM_PCC_DEV_CON_E_MIO_EMM >> 8) & 0xFF;
	uint64_t domain_idx = (CAVM_PCC_DEV_CON_E_MIO_EMM >> 16) & 0xFF;
	cavm_iobnx_rslx_streams_t iobn_rslx_stream;
	cavm_iobnx_domx_busx_streams_t iobn_domx_busx_stream;

	/* Check for MMC boot, if not return here */
	if (plat_octeontx_bcfg->bcfg.boot_dev.boot_type != OCTEONTX_BOOT_EMMC)
		return;

	for (int iobn_idx = 0; iobn_idx < plat_octeontx_scfg->iobn_count;
				iobn_idx++) {
		if (secure) {
			/*
			 * While booting from MMC device, it is
			 * necessary to configure IOBN as to grant
			 * access for eMMC controller to secure memory,
			 * where images are loaded
			 */
			iobn_rslx_stream.u = CSR_READ(
				CAVM_IOBNX_RSLX_STREAMS(iobn_idx,
							rsl_idx));
			iobn_rslx_stream.s.strm_nsec = 0;
			iobn_rslx_stream.s.phys_nsec = 0;
			CSR_WRITE(CAVM_IOBNX_RSLX_STREAMS(
						iobn_idx, rsl_idx),
						iobn_rslx_stream.u);

			iobn_domx_busx_stream.u = CSR_READ(
				CAVM_IOBNX_DOMX_BUSX_STREAMS(iobn_idx,
				domain_idx, bus_idx));
			iobn_domx_busx_stream.s.strm_nsec = 0;
			iobn_domx_busx_stream.s.phys_nsec = 0;
			CSR_WRITE(CAVM_IOBNX_DOMX_BUSX_STREAMS(
				iobn_idx, domain_idx, bus_idx),
				iobn_domx_busx_stream.u);
		} else {
			/*
			 * Configure IOBN and mark MMC controller in
			 * NODE0 as acting for non-secure domain.
			 */
			iobn_rslx_stream.u = CSR_READ(
				CAVM_IOBNX_RSLX_STREAMS(
					iobn_idx, rsl_idx));
			iobn_rslx_stream.s.strm_nsec = 1;
			iobn_rslx_stream.s.phys_nsec = 1;
			CSR_WRITE(CAVM_IOBNX_RSLX_STREAMS(
					iobn_idx, rsl_idx), iobn_rslx_stream.u);

			/*
			 * Do not change IOBNX_DOMX_BUSX_STREAMS
			 * (changing IOBNX_RSLX_STREAMS was sufficient).
			 */
		}
	}
}

/*
 * Helper function for setting-up stream security in IOBN.
 */
static void set_iobn_stream_security(int domain_idx, int bus_idx, int dev_idx,
				     int strm_ns, int phys_ns)
{
	int iobn_idx;

	for (iobn_idx = 0; iobn_idx < plat_octeontx_scfg->iobn_count;
	     iobn_idx++) {
		cavm_iobnx_domx_devx_streams_t iobn_comx_devx_stream;
		cavm_iobnx_domx_busx_streams_t iobn_domx_busx_stream;

		iobn_domx_busx_stream.u = CSR_READ(
			CAVM_IOBNX_DOMX_BUSX_STREAMS(iobn_idx, domain_idx,
						     bus_idx));
		iobn_domx_busx_stream.s.strm_nsec = strm_ns;
		iobn_domx_busx_stream.s.phys_nsec = phys_ns;
		CSR_WRITE(CAVM_IOBNX_DOMX_BUSX_STREAMS(
			  iobn_idx, domain_idx, bus_idx),
			  iobn_domx_busx_stream.u);

		iobn_comx_devx_stream.u = CSR_READ(
			CAVM_IOBNX_DOMX_DEVX_STREAMS(iobn_idx, domain_idx,
						     dev_idx));
		iobn_comx_devx_stream.s.strm_nsec = strm_ns;
		iobn_comx_devx_stream.s.phys_nsec = phys_ns;
		CSR_WRITE(CAVM_IOBNX_DOMX_DEVX_STREAMS(
			  iobn_idx, domain_idx, dev_idx),
			  iobn_comx_devx_stream.u);
	}
}

/*
 * This function configures PCI EP streams' security in IOBN.
 *
 * On entry,
 *   secure: 0 to configure stream for NON-secure lookup
 *           1 to configure stream for SECURE lookup
 */
void octeontx_configure_pem_ep_security(int pem, int secure)
{
	int dev_idx;
	int bus_idx;
	int domain_idx;
	int strm_ns, phys_ns;
	uint32_t streamid;
	cavm_pemx_cfg_t pemx_cfg;
	cavm_pemx_on_t pemx_on;
	cavm_smmux_s_gbpa_t s_gbpa;

	switch (MIDR_PARTNUM(read_midr())) {
	case T96PARTNUM:
		if (pem == 0)
			streamid = CAVM_PCC_DEV_CON_E_PCIERC0_CN9;
		else if (pem == 2)
			streamid = CAVM_PCC_DEV_CON_E_PCIERC2_CN9;
		else
			break;

		pemx_on.u = CSR_READ(CAVM_PEMX_ON(pem));
		pemx_cfg.u = CSR_READ(CAVM_PEMX_CFG(pem));

		/* if PEM0 disabled or in RC mode, exit */
		if (!pemx_on.cn9.pemon || pemx_cfg.cn9.hostmd)
			break;

		strm_ns = !secure; /* set according to caller's request */
		phys_ns = 1; /* host can only access non-secure memory */

		domain_idx = (streamid >> 16) & 0xFF;
		bus_idx = (streamid >> 8) & 0xFF;
		dev_idx = 0; /* device 0 is used for host remote WRITE ops */
		set_iobn_stream_security(domain_idx, bus_idx, dev_idx, strm_ns,
					 phys_ns);

		dev_idx = 3; /* device 3 is used for host remote READ ops */
		set_iobn_stream_security(domain_idx, bus_idx, dev_idx, strm_ns,
					 phys_ns);

		/* Ensure that SMMU uses NS bit from secure stream config.
		 * The BDK sets NSCFG override to force secure memory accesses
		 * while loading images.
		 * It is safe to reset this here because all images have been
		 * loaded.
		 */
		s_gbpa.u = CSR_READ(CAVM_SMMUX_S_GBPA(0));
		s_gbpa.s.nscfg = 0;
		CSR_WRITE(CAVM_SMMUX_S_GBPA(0), s_gbpa.u);

		break;

	default:
		break; /* nothing to do ... */
	}
}
