/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <debug.h>
#include <plat/common/platform.h>
#include <platform_def.h>
#include <platform_dt.h>
#include <platform_setup.h>
#include <platform_irqs_def.h>
#include <octeontx_common.h>
#include <octeontx_ecam.h>
#include <gpio_octeontx.h>
#include <bphy.h>
#include <octeontx_utils.h>
#include <octeontx_plat_configuration.h>
#include <plat_otx2_configuration.h>
#include <plat_octeontx.h>
#include <plat_scfg.h>
#include <qlm/qlm.h>

/* This file map memory for different blocks so it needs all csrs definitions */
#include "cavm-csrs.h"

#define CAVM_BPHY_BAR_E_BPHY_PF_BAR0 (0x860000000000ll)
#define CAVM_BPHY_BAR_E_BPHY_PF_BAR0_SIZE 0x8000000000ull
#define CAVM_BPHY_BAR_E_BPHY_PF_BAR2 (0x87e040000000ll)
#define CAVM_BPHY_BAR_E_BPHY_PF_BAR2_SIZE 0x10000000ull
#define CAVM_PSM_INT_VEC_E_GPINTX(a) (0 + (a))
uint64_t CAVM_PSM_MSIX_VECX_ADDR(unsigned long a)
{
	if (a <= 26)
		return 0x860000000000ll + 0x10ll * ((a) & 0x1f);
	return 0;
}

static uint64_t msix_addr_save;

int plat_octeontx_get_ecams_count(void)
{
	return 1;
}

int plat_octeontx_get_iobn_count(void)
{
	return 1;
}

int plat_octeontx_is_lmc_enabled(unsigned int lmc)
{
	union cavm_lmcx_dll_ctl2 lmcx_dll_ctl2;

	/* LMC 1 is not available on F95MM */
	if (lmc == 1)
		return 0;

	lmcx_dll_ctl2.u = CSR_READ(CAVM_LMCX_DLL_CTL2(lmc));
	return lmcx_dll_ctl2.cnf95xx.dreset ? 0 : 1;
}

/*******************************************************************************
 * Setup secondary CPU JUMP address from RESET
 ******************************************************************************/
void plat_octeontx_set_secondary_cpu_jump_addr(uint64_t entrypoint_addr)
{
	/*
	 * Assembly for ROM memory:
	 *  d508711f        ic      ialluis
	 *  d503201f        nop
	 *  58000040        ldr     x0, 328 <branch_addr>
	 *  d61f0000        br      x0
	 *              branch_addr:
	 * Memory is little endain, so 64 bit constants have the first
	 * instruction in the low word
	 */
	CSR_WRITE(CAVM_ROM_MEMX(0), 0xd503201fd508711full);
	CSR_WRITE(CAVM_ROM_MEMX(1), 0xd61f000058000040ull);
	CSR_WRITE(CAVM_ROM_MEMX(2), entrypoint_addr);
}

int plat_octeontx_get_mpi_count(void)
{
	return 2;
}

int plat_octeontx_get_smmu_count(void)
{
	return 1;
}

int plat_octeontx_get_twsi_count(void)
{
	return 6;
}

int plat_octeontx_get_cpt_count(void)
{
	return 0;
}

int plat_octeontx_get_cgx_count(void)
{
	return 2;
}

int plat_octeontx_get_pem_count(void)
{
	return 0;
}

int plat_octeontx_get_gser_count(void)
{
	return 10;
}

int plat_octeontx_get_gserr_count(void)
{
	return 2;
}

int plat_octeontx_get_gserc_count(void)
{
	return 0;
}

int plat_octeontx_get_gserp_count(void)
{
	return 0;
}

int plat_otx2_get_gserx(int qlm, int *shift_from_first)
{
	int gserx;
	int gserp_count;

	if (qlm >= plat_octeontx_get_gser_count())
		return -1;

	gserp_count = plat_octeontx_get_gserp_count();

	if (qlm < gserp_count)
		return -1;

	gserx = qlm - gserp_count;

	if (shift_from_first != NULL)
		*shift_from_first = 0;

	return gserx;
}

extern const qlm_ops_t qlm_gserr_ops;

const qlm_ops_t *plat_otx2_get_qlm_ops(int cgx_idx)
{
	if (cgx_idx < 0 || cgx_idx >= plat_octeontx_get_cgx_count())
		return NULL;

	return &qlm_gserr_ops;
}

int plat_octeontx_get_uaa_count(void)
{
	return 8;
}

int plat_octeontx_get_rvu_count(void)
{
	return 16;
}

int plat_octeontx_get_mcc_count(void)
{
	return MAX_MCC;
}

/* Return number of lanes available for different QLMs. */
int plat_get_max_lane_num(int qlm)
{
	/* QLMx where x > 2 is in fact DLMx */
	if (qlm >= 2)
		return 2;
	return 4;
}

/* Return the CGX<->QLM mapping */
int plat_get_cgx_idx(int qlm)
{
	int idx;

	switch (qlm) {
	case 0:
		idx = 0;
		break;
	case 1:
		idx = 1;
		break;
	default:
		idx = -1;
		break;
	}

	return idx;
}

/*
 * BDK uses CPC RAM memory as key memory.
 * This is indicated by storing the ROTPK at TRUST-ROT-ADDR,
 * which is in range of CPC_RAM_MEMX.
 * Moreover, ATF needs to have access to SCP-AP Secure0 mailbox.
 * Map required memory as MT_RW.
 */
void plat_map_cpc_mem(void)
{
	cavm_cpc_const_t cpc_const;
	unsigned long cpc_ram_size, attr;

	/* Calculate CPC RAM size based on a number of 16KB memory regions */
	cpc_const.u = CSR_READ(CAVM_CPC_CONST);
	cpc_ram_size = cpc_const.s.mem_regions * 0x4000;

	attr = MT_MEMORY | MT_RW | MT_SECURE;
	add_map_record(CAVM_CPC_RAM_MEMX(0),
		       cpc_ram_size, attr);

	/* Map required XCP memory region for doorbell registers */
	add_map_record(CAVM_XCP_BAR_E_XCPX_PF_BAR0(CAVM_CPC_XCP_MAP_E_SCP),
		       CAVM_XCP_BAR_E_XCPX_PF_BAR0_SIZE, attr);
}

void plat_add_mmio(void)
{
	unsigned long attr;
	int i, device_type_count;

	attr = MT_DEVICE | MT_RW | MT_SECURE;
	add_map_record(CAVM_RST_BAR_E_RST_PF_BAR0_CN9,
			CAVM_RST_BAR_E_RST_PF_BAR0_CN9_SIZE, attr);
	add_map_record(CAVM_RST_BAR_E_RST_PF_BAR2,
			CAVM_RST_BAR_E_RST_PF_BAR2_SIZE, attr);

	add_map_record(CAVM_CCS_BAR_E_CCS_PF_BAR0,
			CAVM_CCS_BAR_E_CCS_PF_BAR0_SIZE, attr);
	add_map_record(CAVM_MIO_EMM_BAR_E_MIO_EMM_PF_BAR0_CN9,
			CAVM_MIO_EMM_BAR_E_MIO_EMM_PF_BAR0_CN9_SIZE, attr);
	add_map_record(CAVM_MIO_EMM_BAR_E_MIO_EMM_PF_BAR4,
			CAVM_MIO_EMM_BAR_E_MIO_EMM_PF_BAR4_SIZE, attr);
	add_map_record(CAVM_FUSF_BAR_E_FUSF_PF_BAR0,
			CAVM_FUSF_BAR_E_FUSF_PF_BAR0_SIZE, attr);

	device_type_count = plat_octeontx_get_mpi_count();
	for (i = 0; i < device_type_count; i++) {
		add_map_record(CAVM_MPI_BAR_E_MPIX_PF_BAR0(i),
				CAVM_MPI_BAR_E_MPIX_PF_BAR0_SIZE, attr);
		add_map_record(CAVM_MPI_BAR_E_MPIX_PF_BAR4(i),
				CAVM_MPI_BAR_E_MPIX_PF_BAR4_SIZE, attr);
	}
	add_map_record(CAVM_GIC_BAR_E_GIC_PF_BAR2,
			CAVM_GIC_BAR_E_GIC_PF_BAR2_SIZE, attr);
	add_map_record(GIC_PF_BAR4, GIC_PF_BAR4_SIZE, attr);

	device_type_count = plat_octeontx_get_smmu_count();
	for (i = 0; i < device_type_count; i++)
		add_map_record(CAVM_SMMU_BAR_E_SMMUX_PF_BAR0_CN9(i),
			CAVM_SMMU_BAR_E_SMMUX_PF_BAR0_CN9_SIZE, attr);

	add_map_record(CAVM_GTI_BAR_E_GTI_PF_BAR0_CN9,
			CAVM_GTI_BAR_E_GTI_PF_BAR0_CN9_SIZE, attr);
	add_map_record(CAVM_GTI_BAR_E_GTI_PF_BAR4_CN9,
			CAVM_GTI_BAR_E_GTI_PF_BAR4_CN9_SIZE, attr);

	add_map_record(CAVM_LMC_BAR_E_LMCX_PF_BAR0(0),
			CAVM_LMC_BAR_E_LMCX_PF_BAR0_SIZE, attr);
	add_map_record(CAVM_LMC_BAR_E_LMCX_PF_BAR4(0),
			CAVM_LMC_BAR_E_LMCX_PF_BAR4_SIZE, attr);
	add_map_record(CAVM_LMC_BAR_E_LMCX_PF_BAR0(2),
			CAVM_LMC_BAR_E_LMCX_PF_BAR0_SIZE, attr);
	add_map_record(CAVM_LMC_BAR_E_LMCX_PF_BAR4(2),
			CAVM_LMC_BAR_E_LMCX_PF_BAR4_SIZE, attr);

	device_type_count = plat_octeontx_get_twsi_count();
	for (i = 0; i < device_type_count; i++) {
		add_map_record(CAVM_MIO_TWS_BAR_E_MIO_TWSX_PF_BAR0_CN9(i),
			CAVM_MIO_TWS_BAR_E_MIO_TWSX_PF_BAR0_CN9_SIZE, attr);
		add_map_record(CAVM_MIO_TWS_BAR_E_MIO_TWSX_PF_BAR4(i),
			CAVM_MIO_TWS_BAR_E_MIO_TWSX_PF_BAR4_SIZE, attr);
	}

	device_type_count = plat_octeontx_get_cgx_count();
	for (i = 0; i < device_type_count; i++) {
		add_map_record(CAVM_CGX_BAR_E_CGXX_PF_BAR0(i),
				CAVM_CGX_BAR_E_CGXX_PF_BAR0_SIZE, attr);
		add_map_record(CAVM_CGX_BAR_E_CGXX_PF_BAR4(i),
				CAVM_CGX_BAR_E_CGXX_PF_BAR4_SIZE, attr);
	}

	device_type_count = plat_octeontx_get_gserr_count();
	for (i = 0; i < device_type_count; i++)
		add_map_record(CAVM_GSERR_BAR_E_GSERRX_PF_BAR0(i),
			       CAVM_GSERR_BAR_E_GSERRX_PF_BAR0_SIZE, attr);

	add_map_record(CAVM_GPIO_BAR_E_GPIO_PF_BAR0_CN9,
			CAVM_GPIO_BAR_E_GPIO_PF_BAR0_CN9_SIZE, attr);
	add_map_record(CAVM_GPIO_BAR_E_GPIO_PF_BAR4,
			CAVM_GPIO_BAR_E_GPIO_PF_BAR4_SIZE, attr);

	device_type_count = plat_octeontx_get_uaa_count();
	for (i = 0; i < device_type_count; i++) {
		add_map_record(UAAX_PF_BAR0(i),
				CAVM_UAA_BAR_E_UAAX_PF_BAR0_CN9_SIZE, attr);
		add_map_record(CAVM_UAA_BAR_E_UAAX_PF_BAR4(i),
				CAVM_UAA_BAR_E_UAAX_PF_BAR4_SIZE, attr);
	}

	device_type_count = plat_octeontx_get_ecams_count();
	for (i = 0; i < device_type_count; i++) {
		add_map_record(CAVM_ECAM_BAR_E_ECAMX_PF_BAR0_CN9(i),
				CAVM_ECAM_BAR_E_ECAMX_PF_BAR0_CN9_SIZE, attr);
		add_map_record(ECAM_PF_BAR2(i),
				CAVM_ECAM_BAR_E_ECAMX_PF_BAR2_CN9_SIZE, attr);
	}

	add_map_record(CAVM_ROM_BAR_E_ROM_PF_BAR0,
			CAVM_ROM_BAR_E_ROM_PF_BAR0_SIZE, attr);

	device_type_count = plat_octeontx_get_iobn_count();
	for (i = 0; i < device_type_count; ++i) {
		add_map_record(CAVM_IOBN_BAR_E_IOBNX_PF_BAR0_CN9(i),
				CAVM_IOBN_BAR_E_IOBNX_PF_BAR0_CN9_SIZE, attr);
		add_map_record(CAVM_IOBN_BAR_E_IOBNX_PF_BAR4(i),
				CAVM_IOBN_BAR_E_IOBNX_PF_BAR4_SIZE, attr);
	}

	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0),
			CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	device_type_count = plat_octeontx_get_rvu_count();
	for (i = 0; i < device_type_count; ++i)
		add_map_record(CAVM_RVU_BAR_E_RVU_PFX_FUNCX_BAR2(i, 0),
				CAVM_RVU_BAR_E_RVU_PFX_FUNCX_BAR2_SIZE, attr);

	/* Add regions for required for RVU init */
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_NIXX(0) *
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_NPA *
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_SSO *
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_SSOW *
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_TIM *
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_NDCX(0) *
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_NDCX(1) *
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_NDCX(2) *
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);

	add_map_record(CAVM_SMI_BAR_E_SMI_PF_BAR0_CN9,
				CAVM_SMI_BAR_E_SMI_PF_BAR0_CN9_SIZE, attr);

	/* Add BPHY MSI-X space */
	/* The BAR0 space of BPHY is huge so just map few kbytes */
	add_map_record(CAVM_BPHY_BAR_E_BPHY_PF_BAR0, 3 * 4096, attr);

	plat_map_cpc_mem();

	device_type_count = plat_octeontx_get_mcc_count();
	for (i = 0; i < device_type_count; ++i) {
		add_map_record(CAVM_MCC_BAR_E_MCCX_PF_BAR0(i),
			CAVM_MCC_BAR_E_MCCX_PF_BAR0_SIZE, attr);
		add_map_record(CAVM_MCC_BAR_E_MCCX_PF_BAR4(i),
			CAVM_MCC_BAR_E_MCCX_PF_BAR4_SIZE, attr);
	}
	add_map_record(CAVM_MDC_BAR_E_MDC_PF_BAR4,
		       CAVM_MDC_BAR_E_MDC_PF_BAR4_SIZE, attr);

	/*
	 * Map random number generator for gser intertnal
	 * get random function
	 */
	add_map_record(CAVM_RNM_BAR_E_RNM_VF_BAR0_CN9,
				CAVM_RNM_BAR_E_RNM_VF_BAR0_CN9_SIZE, attr);

	/*
	 * Shared memory configuration.
	 * Map additional memory used by RVU/SFP mgmt(shared between AP & MCP).
	 * Do not use add_map_record, it will round size up
	 */
	mmap_add_region(RVU_MEM_BASE, RVU_MEM_BASE,
			RVU_MEM_SIZE, (MT_MEMORY | MT_RW | MT_NS));

	mmap_add_region(SFP_SHMEM_BASE, SFP_SHMEM_BASE,
			SFP_SHMEM_SIZE, (MT_MEMORY | MT_RW | MT_NS));

#ifdef NT_FW_CONFIG
	mmap_add_region(NT_FW_CONFIG_BASE, NT_FW_CONFIG_BASE,
			NT_FW_CONFIG_LIMIT, (MT_MEMORY | MT_RW | MT_NS));
#endif

	for (i = 0; i < MAX_CCU; i++) {
		add_map_record(CAVM_CCU_BAR_E_CCUX_PF_BAR0(i),
			       CAVM_CCU_BAR_E_CCUX_PF_BAR0_SIZE, attr);
		add_map_record(CAVM_CCU_BAR_E_CCUX_PF_BAR4(i),
			       CAVM_CCU_BAR_E_CCUX_PF_BAR4_SIZE, attr);
	}
}

void plat_set_gpio_msix_vectors(int gpio_num, int irq_num, int enable)
{
	uint64_t vector_ptr;
	int intr_pinx;

	/* Get the offset of interrupt vector for that GPIO line */
	intr_pinx = CAVM_GPIO_INT_VEC_E_INTR_PINX_F95MM(gpio_num);

	/* INTR_PINX vector address */
	vector_ptr =  CAVM_GPIO_BAR_E_GPIO_PF_BAR4 + intr_pinx * 0x10;

	if (enable) {
		/* Save vector address so that it can be restored.
		 * The value will be same for set and clear vectors so saving
		 * once will suffice.
		 */
		msix_addr_save = octeontx_read64(vector_ptr);
		/* Enable SECVEC to make the vector secure */
		octeontx_write64(vector_ptr, CAVM_GICD_SETSPI_SR | 1);
		vector_ptr += 0x8;
		octeontx_write64(vector_ptr, irq_num);

		/* INTR_PINX_CLEAR vector */
		vector_ptr += 0x8;
		/* Enable SECVEC to make the vector secure */
		octeontx_write64(vector_ptr, CAVM_GICD_CLRSPI_SR | 1);
		vector_ptr += 0x8;
		octeontx_write64(vector_ptr, irq_num);
	} else {
		/* Restore the vector address */
		octeontx_write64(vector_ptr, msix_addr_save);

		/* INTR_PINX_CLEAR vector */
		vector_ptr += 0x10;
		octeontx_write64(vector_ptr, msix_addr_save);
	}
}

void plat_gpio_irq_setup(void)
{
	gpio_intercept_interrupts = 1;

	if (octeontx_register_gpio_handlers() < 0)
		ERROR("Failed to register GPIO intercept handlers\n");

	if (cavm_register_bphy_intr_handlers() < 0)
		ERROR("Failed to register BPHY intr handlers\n");
}

void plat_set_bphy_psm_msix_vectors(int msix_num, int irq_num, int enable)
{
	volatile struct msix_cap *msicap = NULL;
	uint64_t vector_ptr, config_base;
	struct pcie_config *pconfig;
	uint8_t cap_pointer;
	int bus, dev, fn;

	/* OS pci enumeration disables msix, so enable again */
	/* on 95xx BPHY is on 6:0.0*/
	bus = 6;
	dev = fn = 0;
	config_base = CAVM_ECAM_BAR_E_ECAMX_PF_BAR2_CN9(0) +
		  (bus << 20) + (dev << 15) + (fn << 12);
	pconfig = (struct pcie_config *)config_base;
	cap_pointer = pconfig->cap_pointer;

	while (cap_pointer) {
		msicap = (struct msix_cap *)(config_base + cap_pointer);
		if (msicap->cap_ID == ECAM_PCIEID_MSIX_CAP_ID) {
			msicap->messagecontrol |= (1 << 15);
			break;
		}
		cap_pointer = msicap->next_pointer;
	}

	if (cap_pointer == 0) {
		ERROR("MSI-X cap header not found !\n");
		return;
	}

	vector_ptr = CAVM_PSM_MSIX_VECX_ADDR(
				CAVM_PSM_INT_VEC_E_GPINTX(0) + msix_num);

	if (enable) {
		octeontx_write64(vector_ptr, CAVM_GICD_SETSPI_SR | 1);
		vector_ptr += 8;
		octeontx_write64(vector_ptr, irq_num);
	} else {
		octeontx_write64(vector_ptr, CAVM_GICD_SETSPI_NSR);
		vector_ptr += 8;
		octeontx_write64(vector_ptr, irq_num);
	}
}

/*
 * This API should be provided by each [otx2] platform that requires
 * individual IOBN security settings.
 *
 * During IOBN initialization, this API is invoked to retrieve any
 * platform-specific stream settings which need to be configured.
 *
 * The family-specific stream security settings (i.e. common across platforms)
 * are provided by 'octeontx_init_iobn()'.
 *
 * Please refer to 'octeontx_init_iobn()'.
 *
 * On entry,
 *   void
 *
 * Returns,
 *   array of 'struct otx2_stream_security_setting'
 *   size of array (via ptr)
 */
struct otx2_stream_security_setting *plat_get_otx2_stream_security(int *count)
{
	static struct otx2_stream_security_setting stream_settings[] = {
		/* no platform-specific stream security settings */
	};

	*count = ARRAY_SIZE(stream_settings);
	return stream_settings;
}
