/*
 * Copyright (C) 2016-2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <debug.h>
#include <plat/common/platform.h>
#include <platform_def.h>
#include <platform_setup.h>
#include <platform_irqs_def.h>
#include <octeontx_common.h>
#include <gpio_octeontx.h>
#include <octeontx_utils.h>
#include <octeontx_plat_configuration.h>
#include <plat_otx2_configuration.h>
#include <plat_octeontx.h>
#include <octeontx_irqs_def.h>
#include <plat_scfg.h>
#include <qlm/qlm.h>

static uint64_t msix_addr_save;

int plat_octeontx_get_ecams_count(void)
{
	return 1;
}

int plat_octeontx_get_iobn_count(void)
{
	return 2;
}

int plat_octeontx_is_lmc_enabled(unsigned lmc)
{
	union cavm_lmcx_dll_ctl2 lmcx_dll_ctl2;

	if ((plat_get_altpkg() == CN95XXE_PKG)
	    && (lmc == 1)) {
		return 0;
	} else if ((plat_get_altpkg() == CN93XX_PKG)
	    && (lmc == 2)) {
		return 0;
	}

	lmcx_dll_ctl2.u = CSR_READ(CAVM_LMCX_DLL_CTL2(lmc));

	return (lmcx_dll_ctl2.s.dreset ? 0 : 1);
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
	return 1;
}

int plat_octeontx_get_cgx_count(void)
{
	return 3;
}

int plat_octeontx_get_pem_count(void)
{
	if (plat_get_altpkg() == CN95XXE_PKG)
		return 1;

	return 3;
}

int plat_octeontx_get_gser_count(void)
{
	if (plat_get_altpkg() == CN93XX_PKG)
		return 6;

	return 8;
}

int plat_octeontx_get_gserp_count(void)
{
	if (IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 3))
		return 5;

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

	if (shift_from_first != NULL) {
		/* On T96 A0 and A1, qlm 4 and 5 are DLMs. 5 is second DLM */
		if (IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 1) && (gserx == 5))
			*shift_from_first = -1;
		else
			*shift_from_first = 0;
	}

	return gserx;
}

extern const qlm_ops_t qlm_gsern_ops;
extern const qlm_ops_t qlm_gserr_ops;

const qlm_ops_t *plat_otx2_get_qlm_ops(int cgx_idx)
{
	if (cgx_idx < 0 || cgx_idx >= plat_octeontx_get_cgx_count())
		return NULL;

	if (IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 1)) {
		return &qlm_gsern_ops;
	} else {
		return &qlm_gserr_ops;
	}
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
	if (IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 3))
		return 4;

	if ((qlm == 4) || (qlm == 5))
		return 2;
	return 4;
}

/* Return the CGX<->QLM mapping */
int plat_get_cgx_idx(int qlm)
{
	int idx;

	switch (qlm) {
	case 3:
		if (IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 1))
			idx = 0;
		else
			idx = -1;
		break;
	case 7:
		idx = 0;
		break;
	case 4:
		if (IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 1))
			idx = 1;
		else
			idx = -1;
		break;
	case 5:
		idx = 1;
		break;
	case 6:
		idx = 2;
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
void plat_map_cpc_mem()
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

void plat_add_mmio()
{
	unsigned long attr;
	int i, device_type_count;

	attr = MT_DEVICE | MT_RW | MT_SECURE;
	add_map_record(CAVM_RST_BAR_E_RST_PF_BAR0_CN9, CAVM_RST_BAR_E_RST_PF_BAR0_CN9_SIZE, attr);
	add_map_record(CAVM_RST_BAR_E_RST_PF_BAR2, CAVM_RST_BAR_E_RST_PF_BAR2_SIZE, attr);
	add_map_record(CAVM_RST_BAR_E_RST_PF_BAR4, CAVM_RST_BAR_E_RST_PF_BAR4_SIZE, attr);

	add_map_record(CAVM_CCS_BAR_E_CCS_PF_BAR0, CAVM_CCS_BAR_E_CCS_PF_BAR0_SIZE, attr);

	add_map_record(CAVM_MIO_EMM_BAR_E_MIO_EMM_PF_BAR0_CN9, CAVM_MIO_EMM_BAR_E_MIO_EMM_PF_BAR0_CN9_SIZE, attr);
	add_map_record(CAVM_MIO_EMM_BAR_E_MIO_EMM_PF_BAR4, CAVM_MIO_EMM_BAR_E_MIO_EMM_PF_BAR4_SIZE, attr);
	add_map_record(CAVM_FUSF_BAR_E_FUSF_PF_BAR0,
		       CAVM_FUSF_BAR_E_FUSF_PF_BAR0_SIZE, attr);
#if ENABLE_ATTESTATION_SERVICE
	add_map_record(CAVM_RNM_BAR_E_RNM_PF_BAR0_CN9,
		       CAVM_RNM_BAR_E_RNM_PF_BAR0_CN9_SIZE, attr);
	add_map_record(CAVM_RNM_BAR_E_RNM_VF_BAR0_CN9,
		       CAVM_RNM_BAR_E_RNM_VF_BAR0_CN9_SIZE, attr);
#endif

	device_type_count = plat_octeontx_get_mpi_count();
	for (i = 0; i < device_type_count; i++) {
		add_map_record(CAVM_MPI_BAR_E_MPIX_PF_BAR0(i), CAVM_MPI_BAR_E_MPIX_PF_BAR0_SIZE, attr);
		add_map_record(CAVM_MPI_BAR_E_MPIX_PF_BAR4(i), CAVM_MPI_BAR_E_MPIX_PF_BAR4_SIZE, attr);
	}
	add_map_record(CAVM_GIC_BAR_E_GIC_PF_BAR2, CAVM_GIC_BAR_E_GIC_PF_BAR2_SIZE, attr);
	add_map_record(GIC_PF_BAR4, GIC_PF_BAR4_SIZE, attr);

	device_type_count = plat_octeontx_get_smmu_count();
	for (i = 0; i < device_type_count; i++)
		add_map_record(CAVM_SMMU_BAR_E_SMMUX_PF_BAR0_CN8(i), CAVM_SMMU_BAR_E_SMMUX_PF_BAR0_CN8_SIZE, attr);

	add_map_record(CAVM_GTI_BAR_E_GTI_PF_BAR0_CN9, CAVM_GTI_BAR_E_GTI_PF_BAR0_CN9_SIZE, attr);
	add_map_record(CAVM_GTI_BAR_E_GTI_PF_BAR4_CN9, CAVM_GTI_BAR_E_GTI_PF_BAR4_CN9_SIZE, attr);

	for (i = 0; i < MAX_LMC; i++) {
		add_map_record(CAVM_LMC_BAR_E_LMCX_PF_BAR0(i), CAVM_LMC_BAR_E_LMCX_PF_BAR0_SIZE, attr);
		add_map_record(CAVM_LMC_BAR_E_LMCX_PF_BAR4(i), CAVM_LMC_BAR_E_LMCX_PF_BAR4_SIZE, attr);
	}

	device_type_count = plat_octeontx_get_twsi_count();
	for (i = 0; i < device_type_count; i++) {
		add_map_record(CAVM_MIO_TWS_BAR_E_MIO_TWSX_PF_BAR0_CN9(i), CAVM_MIO_TWS_BAR_E_MIO_TWSX_PF_BAR0_CN9_SIZE, attr);
		add_map_record(CAVM_MIO_TWS_BAR_E_MIO_TWSX_PF_BAR4(i), CAVM_MIO_TWS_BAR_E_MIO_TWSX_PF_BAR4_SIZE, attr);
	}

	device_type_count = plat_octeontx_get_cpt_count();
	for (i = 0; i < device_type_count; i++) {
		add_map_record(CAVM_CPT_BAR_E_CPTX_PF_BAR0(i), CAVM_CPT_BAR_E_CPTX_PF_BAR0_SIZE, attr);
		add_map_record(CAVM_CPT_BAR_E_CPTX_PF_BAR4(i), CAVM_CPT_BAR_E_CPTX_PF_BAR4_SIZE, attr);
		add_map_record(CAVM_CPT_BAR_E_CPTX_VFX_BAR0(i, 0), 64*CAVM_CPT_BAR_E_CPTX_VFX_BAR0_SIZE, attr);
		add_map_record(CAVM_CPT_BAR_E_CPTX_VFX_BAR4(i, 0), 64*CAVM_CPT_BAR_E_CPTX_VFX_BAR4_SIZE, attr);
	}

	device_type_count = plat_octeontx_get_cgx_count();
	for (i = 0; i < device_type_count; i++) {
		add_map_record(CAVM_CGX_BAR_E_CGXX_PF_BAR0(i), CAVM_CGX_BAR_E_CGXX_PF_BAR0_SIZE, attr);
		add_map_record(CAVM_CGX_BAR_E_CGXX_PF_BAR4(i), CAVM_CGX_BAR_E_CGXX_PF_BAR4_SIZE, attr);
	}

	device_type_count = plat_octeontx_get_pem_count();
	for (i = 0; i < device_type_count; i++) {
		add_map_record(CAVM_PEM_BAR_E_PEMX_PF_BAR0_CN9(i), CAVM_PEM_BAR_E_PEMX_PF_BAR0_CN9_SIZE, attr);
		add_map_record(CAVM_PEM_BAR_E_PEMX_PF_BAR4_CN9(i), CAVM_PEM_BAR_E_PEMX_PF_BAR4_CN9_SIZE, attr);
	}

	if (IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 3)) {
		device_type_count = plat_octeontx_get_cgx_count();
		for (i = 0; i < device_type_count; i++)
			add_map_record(CAVM_GSERR_BAR_E_GSERRX_PF_BAR0(i),
				       CAVM_GSERR_BAR_E_GSERRX_PF_BAR0_SIZE, attr);
	} else {
		device_type_count = plat_octeontx_get_gser_count();
		for (i = 0; i < device_type_count; i++)
			add_map_record(CAVM_GSERN_BAR_E_GSERNX_PF_BAR0(i),
				       CAVM_GSERN_BAR_E_GSERNX_PF_BAR0_SIZE, attr);
	}

	add_map_record(CAVM_GPIO_BAR_E_GPIO_PF_BAR0_CN9, CAVM_GPIO_BAR_E_GPIO_PF_BAR0_CN9_SIZE, attr);
	add_map_record(CAVM_GPIO_BAR_E_GPIO_PF_BAR4, CAVM_GPIO_BAR_E_GPIO_PF_BAR4_SIZE, attr);

	device_type_count = plat_octeontx_get_uaa_count();
	for (i = 0; i < device_type_count; i++) {
		add_map_record(UAAX_PF_BAR0(i), CAVM_UAA_BAR_E_UAAX_PF_BAR0_CN9_SIZE, attr);
		add_map_record(CAVM_UAA_BAR_E_UAAX_PF_BAR4(i), CAVM_UAA_BAR_E_UAAX_PF_BAR4_SIZE, attr);
	}

	device_type_count = plat_octeontx_get_ecams_count();
	for (i = 0; i < device_type_count; i++) {
		add_map_record(CAVM_ECAM_BAR_E_ECAMX_PF_BAR0_CN9(i), CAVM_ECAM_BAR_E_ECAMX_PF_BAR0_CN9_SIZE, attr);
		add_map_record(ECAM_PF_BAR2(i), CAVM_ECAM_BAR_E_ECAMX_PF_BAR2_CN9_SIZE, attr);
	}

	add_map_record(CAVM_ROM_BAR_E_ROM_PF_BAR0, CAVM_ROM_BAR_E_ROM_PF_BAR0_SIZE, attr);

	device_type_count = plat_octeontx_get_iobn_count();
	for (i = 0; i < device_type_count; ++i) {
		add_map_record(CAVM_IOBN_BAR_E_IOBNX_PF_BAR0_CN9(i), CAVM_IOBN_BAR_E_IOBNX_PF_BAR0_CN9_SIZE , attr);
		add_map_record(CAVM_IOBN_BAR_E_IOBNX_PF_BAR4(i), CAVM_IOBN_BAR_E_IOBNX_PF_BAR4_SIZE, attr);
	}

	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0), CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	device_type_count = plat_octeontx_get_rvu_count();
	for (i = 0; i < device_type_count; ++i)
		add_map_record(CAVM_RVU_BAR_E_RVU_PFX_FUNCX_BAR2(i, 0), CAVM_RVU_BAR_E_RVU_PFX_FUNCX_BAR2_SIZE, attr);

	/* Add regions for required for RVU init */
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_NIXX(0) * CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_NPA * CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_SSO * CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_SSOW * CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_TIM * CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_CPTX(0) * CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_NDCX(0) * CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_NDCX(1) * CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);
	add_map_record(CAVM_RVU_BAR_E_RVU_PFX_BAR0(0) +
				CAVM_RVU_BLOCK_ADDR_E_NDCX(2) * CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE,
				CAVM_RVU_BAR_E_RVU_PFX_BAR0_SIZE, attr);

	add_map_record(CAVM_SMI_BAR_E_SMI_PF_BAR0_CN9,
				CAVM_SMI_BAR_E_SMI_PF_BAR0_CN9_SIZE, attr);

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

	for (i = 0; i < MAX_CCU; i++) {
		add_map_record(CAVM_CCU_BAR_E_CCUX_PF_BAR0(i),
			       CAVM_CCU_BAR_E_CCUX_PF_BAR0_SIZE, attr);
		add_map_record(CAVM_CCU_BAR_E_CCUX_PF_BAR4(i),
			       CAVM_CCU_BAR_E_CCUX_PF_BAR4_SIZE, attr);
	}

	/*
	 * Map temperature sensor used by qlm gsern code.
	 */
	add_map_record(CAVM_TSN_BAR_E_TSNX_PF_BAR0(9),
		       CAVM_TSN_BAR_E_TSNX_PF_BAR0_SIZE, attr);

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

#ifdef DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS
	mmap_add_region(SERDES_EYE_DATA_BASE, SERDES_EYE_DATA_BASE,
			SERDES_EYE_DATA_SIZE, (MT_MEMORY | MT_RW | MT_NS));

	mmap_add_region(SERDES_SETTINGS_DATA_BASE, SERDES_SETTINGS_DATA_BASE,
		SERDES_SETTINGS_DATA_SIZE, (MT_MEMORY | MT_RW | MT_NS));
#endif /* DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS */
}

void plat_set_gpio_msix_vectors(int gpio_num, int irq_num, int enable)
{
	uint64_t vector_ptr;
	int intr_pinx;

	/* Get the offset of interrupt vector for that GPIO line */
	intr_pinx = CAVM_GPIO_INT_VEC_E_INTR_PINX_CN96XX(gpio_num);

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
