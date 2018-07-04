/** @file

  Copyright (c) 2016-2018, Cavium Inc. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
#include <stdio.h>
#include <arch.h>
#include <platform_def.h>
#include <cavm_common.h>
#include <cavm_dt.h>
#include <cavm_rvu.h>
#include <platform.h>
#include <assert.h>
#include <debug.h>

#ifdef DEBUG_ATF_RVU
#define debug_rvu printf
#else
#define debug_rvu(...) ((void) (0))
#endif

static struct rvu_device rvu_dev[MAX_RVU_PFS];

static inline int octeontx_get_msix_for_cpt(int node)
{
	union cavm_cptx_priv_lfx_int_cfg cpt_int_cfg;
	union cavm_cptx_af_constants0 cpt_af_const0;
	int cpt = 0, lf = 0;

	cpt_int_cfg.u = CSR_READ(node, CAVM_CPTX_PRIV_LFX_INT_CFG(cpt, lf));
	cpt_af_const0.u = CSR_READ(node, CAVM_CPTX_AF_CONSTANTS0(cpt));

	return (cpt_af_const0.s.lf * cpt_int_cfg.s.msix_size);
}

static inline int octeontx_get_msix_for_npa(int node)
{
	union cavm_npa_priv_lfx_int_cfg npa_int_cfg;
	int lf = 0;

	npa_int_cfg.u = CSR_READ(node, CAVM_NPA_PRIV_LFX_INT_CFG(lf));

	return npa_int_cfg.s.msix_size;
}

static inline int octeontx_get_msix_for_sso_tim(int node)
{
	int lf = 0;
	union cavm_tim_priv_lfx_int_cfg tim_int_cfg;
	union cavm_sso_priv_lfx_hwgrp_int_cfg sso_int_cfg;
	union cavm_ssow_priv_lfx_hws_int_cfg ssow_int_cfg;

	tim_int_cfg.u = CSR_READ(node, CAVM_TIM_PRIV_LFX_INT_CFG(lf));
	sso_int_cfg.u = CSR_READ(node, CAVM_SSO_PRIV_LFX_HWGRP_INT_CFG(lf));
	ssow_int_cfg.u = CSR_READ(node, CAVM_SSOW_PRIV_LFX_HWS_INT_CFG(lf));

	return (tim_int_cfg.s.msix_size +
		ssow_int_cfg.s.msix_size +
		sso_int_cfg.s.msix_size);
}

static inline int octeontx_get_msix_for_cgx(int node)
{
	int lf = 0, nix = 0;
	union cavm_nixx_priv_lfx_int_cfg nixx_int_cfg;

	nixx_int_cfg.u = CSR_READ(node, CAVM_NIXX_PRIV_LFX_INT_CFG(nix, lf));

	return nixx_int_cfg.s.msix_size + octeontx_get_msix_for_npa(node);
}

static void octeontx_init_rvu_af(int *hwvf)
{
	rvu_dev[RVU_AF].enable = TRUE;
	rvu_dev[RVU_AF].num_vfs = bfdt->rvu_config.admin_pf.num_rvu_vfs;
	rvu_dev[RVU_AF].first_hwvf = *hwvf;
	rvu_dev[RVU_AF].pf_num_msix_vec = bfdt->rvu_config.admin_pf.num_msix_vec;
	rvu_dev[RVU_AF].vf_num_msix_vec = RVU_VF_INT_VEC_COUNT +
					  octeontx_get_msix_for_cgx(0);
	rvu_dev[RVU_AF].pf_res_ena = FALSE;
	rvu_dev[RVU_AF].pci.pf_devid = CAVM_PCC_DEV_IDL_E_RVU_AF & DEVID_MASK;
	rvu_dev[RVU_AF].pci.vf_devid = CAVM_PCC_DEV_IDL_E_SW_RVU_AF_VF & DEVID_MASK;
	rvu_dev[RVU_AF].pci.class_code = RVU_CLASS_CODE & CLASS_CODE_MASK;
	/* Increment already allocated HWVFs */
	*hwvf += rvu_dev[RVU_AF].num_vfs;
}

static void octeontx_init_rvu_fixed(int *hwvf, int rvu, int bfdt_index, int has_vfs)
{
	rvu_sw_rvu_pf_t *sw_pf;

	assert(bfdt_index < SW_RVU_MAX_PF);
	sw_pf = &(bfdt->rvu_config.sw_pf[bfdt_index]);

	rvu_dev[rvu].enable = TRUE;
	rvu_dev[rvu].num_vfs = has_vfs ? sw_pf->num_rvu_vfs : 0;
	rvu_dev[rvu].first_hwvf = has_vfs ? *hwvf : 0;
	rvu_dev[rvu].pf_num_msix_vec = sw_pf->num_msix_vec;
	rvu_dev[rvu].pf_res_ena = FALSE;
	switch (bfdt_index) {
	case SW_RVU_SSO_TIM_PF:
		rvu_dev[rvu].pci.pf_devid =
			CAVM_PCC_DEV_IDL_E_SW_RVU_SSO_TIM_PF & DEVID_MASK;
		rvu_dev[rvu].pci.vf_devid =
			CAVM_PCC_DEV_IDL_E_SW_RVU_SSO_TIM_VF & DEVID_MASK;
		rvu_dev[rvu].pci.class_code = RVU_CLASS_CODE & CLASS_CODE_MASK;
		rvu_dev[rvu].vf_num_msix_vec = has_vfs ?
					(RVU_VF_INT_VEC_COUNT +
					octeontx_get_msix_for_sso_tim(0)) : 0;
		break;
	case SW_RVU_NPA_PF:
		rvu_dev[rvu].pci.pf_devid =
			CAVM_PCC_DEV_IDL_E_SW_RVU_NPA_PF & DEVID_MASK;
		rvu_dev[rvu].pci.vf_devid =
			CAVM_PCC_DEV_IDL_E_SW_RVU_NPA_VF & DEVID_MASK;
		rvu_dev[rvu].pci.class_code = RVU_CLASS_CODE & CLASS_CODE_MASK;
		rvu_dev[rvu].vf_num_msix_vec = has_vfs ?
					(RVU_VF_INT_VEC_COUNT +
					 octeontx_get_msix_for_npa(0)) : 0;
		break;
	case SW_RVU_CPT_PF:
		rvu_dev[rvu].pci.pf_devid =
			CAVM_PCC_DEV_IDL_E_SW_RVU_CPT_PF & DEVID_MASK;
		rvu_dev[rvu].pci.vf_devid =
			CAVM_PCC_DEV_IDL_E_SW_RVU_CPT_VF & DEVID_MASK;
		rvu_dev[rvu].pci.class_code = CPT_CLASS_CODE & CLASS_CODE_MASK;
		rvu_dev[rvu].vf_num_msix_vec = has_vfs ?
					(RVU_VF_INT_VEC_COUNT +
					 octeontx_get_msix_for_cpt(0)) : 0;
		break;
	}
	/* Increment already allocated HWVFs */
	*hwvf += rvu_dev[rvu].num_vfs;
}

static void octeontx_init_rvu_lmac(int *hwvf, int rvu, cgx_config_t *cgx,
				   int lmac_id)
{
	rvu_dev[rvu].enable = TRUE;
	rvu_dev[rvu].num_vfs = cgx->lmac_cfg[lmac_id].num_rvu_vfs;
	rvu_dev[rvu].first_hwvf = *hwvf;
	rvu_dev[rvu].pf_num_msix_vec = cgx->lmac_cfg[lmac_id].num_msix_vec;
	rvu_dev[rvu].vf_num_msix_vec = RVU_VF_INT_VEC_COUNT +
				       octeontx_get_msix_for_cgx(0);
	rvu_dev[rvu].pf_res_ena = TRUE;
	rvu_dev[rvu].pci.pf_devid = CAVM_PCC_DEV_IDL_E_RVU & DEVID_MASK;
	rvu_dev[rvu].pci.vf_devid = CAVM_PCC_DEV_IDL_E_RVU_VF & DEVID_MASK;
	rvu_dev[rvu].pci.class_code = RVU_CLASS_CODE & CLASS_CODE_MASK;
	/* Increment already allocated HWVFs */
	*hwvf += rvu_dev[rvu].num_vfs;
}

static int octeontx_init_rvu_from_fdt(void)
{
	int cgx_id, lmac_id, pf, current_hwvf = 0;
	int uninit_pfs = 0, sso_tim_pfs, npa_pfs;
	cgx_config_t *cgx;

	/* Check if FDT config is valid */
	if (!(bfdt->rvu_config.valid)) {
		ERROR("Invalid RVU configuration, skipping RVU init!.\n");
		return -1;
	}

	/* Firstly, initialize fixed setup (PF0, PF13, PF14, PF15) */
	/* Init RVU0 - AF (PF0) */
	octeontx_init_rvu_af(&current_hwvf);

	/* Init RVU13 - SSO_TIM (PF13) */
	octeontx_init_rvu_fixed(&current_hwvf, RVU_SSO_TIM,
				SW_RVU_SSO_TIM_PF, TRUE);

	/* Init RVU14 - NPA (PF14) */
	octeontx_init_rvu_fixed(&current_hwvf, RVU_NPA,
				SW_RVU_NPA_PF, TRUE);

	if (bfdt->rvu_config.cpt_dis)
		uninit_pfs = 1;
	else {
		/* Init RVU15 - as CPT if present */
		octeontx_init_rvu_fixed(&current_hwvf, RVU_PF15,
				SW_RVU_CPT_PF, TRUE);
	}

	/* Initialize CGX PF */
	pf = RVU_CGX0_LMAC0;
	for (cgx_id = 0; cgx_id < MAX_CGX; cgx_id++) {
		cgx = &(bfdt->cgx_cfg[cgx_id]);
		if (cgx->enable) {
			for (lmac_id = 0; lmac_id < MAX_LMAC_PER_CGX; lmac_id++) {
				if (cgx->lmac_cfg[lmac_id].lmac_enable) {
					octeontx_init_rvu_lmac(&current_hwvf,
							       pf, cgx,
							       lmac_id);
					assert(current_hwvf <= MAX_RVU_HWVFS);
					pf++;
				} else {
					/* Increment number of not
					 * configured PFs yet */
					uninit_pfs++;
				}
			}
		} else {
			/* all four LMACs are not initialized */
			uninit_pfs += MAX_LMAC_PER_CGX;
		}
	}

	/*
	 * For all unitialized PFs, divide them by factor and configure:
	 * FACTOR*uninit_pf's as SSO_TIM PF
	 * All left PFs (uninit_pfs - FACTOR*uninit_pfs) as NPA PF
	 * FACTOR, as discussed with ODP developers, is set to 3/4.
	 */
	if (!uninit_pfs)
		return 0;

	sso_tim_pfs = uninit_pfs * SSO_TIM_TO_NPA_PFS_FACTOR;
	npa_pfs = uninit_pfs - sso_tim_pfs;
	while (sso_tim_pfs > 0) {
		if (pf > RVU_CGX2_LMAC3)
			pf = RVU_PF15;
		octeontx_init_rvu_fixed(&current_hwvf, pf++,
					SW_RVU_SSO_TIM_PF, FALSE);
		sso_tim_pfs--;
	}

	while (npa_pfs > 0) {
		if (pf > RVU_CGX2_LMAC3)
			pf = RVU_PF15;
		octeontx_init_rvu_fixed(&current_hwvf, pf++,
					SW_RVU_NPA_PF, FALSE);
		npa_pfs--;
	}

	return 0;
}

/* returns max PFs supported by RVU */
static int octeontx_get_max_rvu_pfs(int node)
{
	union cavm_rvu_priv_const priv_const;

	priv_const.u = CSR_READ(node, CAVM_RVU_PRIV_CONST);

	return priv_const.s.pfs;
}

static void dump_rvu_devs(int node)
{
	int pf;

	for (pf = 0; pf < octeontx_get_max_rvu_pfs(node); pf++) {
		debug_rvu("******************************************\n");
		debug_rvu("PF%d: enable=%d, num_vfs=%d, first_hwvf=%d\n"
			  "pf_num_msix_vec=%d, pf_res_ena=%d\n",
			  pf, rvu_dev[pf].enable, rvu_dev[pf].num_vfs,
			  rvu_dev[pf].first_hwvf, rvu_dev[pf].pf_num_msix_vec,
			  rvu_dev[pf].pf_res_ena);
		debug_rvu("PCI Settings:\n"
			  "pf_devid=0x%x, vf_devid=0x%x, class_code=0x%x\n",
			  rvu_dev[pf].pci.pf_devid, rvu_dev[pf].pci.vf_devid,
			  rvu_dev[pf].pci.class_code);
	}

	debug_rvu("******************************************\n");
}

/* set mailbox memory*/
static void mailbox_enable(int node)
{
	int pf;
	static uint64_t vf_base = VF_MBOX_BASE;
	union cavm_rvu_af_pf_bar4_addr pf_bar4_addr;

	pf_bar4_addr.u = PF_MBOX_BASE;
	CSR_WRITE(node, CAVM_RVU_AF_PF_BAR4_ADDR, pf_bar4_addr.u);

	for (pf = 0; pf < octeontx_get_max_rvu_pfs(node); pf++) {
		if (rvu_dev[pf].enable && rvu_dev[pf].num_vfs) {
			uint64_t bar4_addr = CSR_PA(node, CAVM_RVU_PFX_VFX_BAR2(pf, 0)) + 0x10;
			cavm_write64(bar4_addr, vf_base);
			vf_base = vf_base + (0x10000 * (rvu_dev[pf].num_vfs & 0x7f));
		}
	}
}

/* Initialize PCI PF_DEVID and VF_DEVID */
static void config_rvu_pci(int node)
{
	cavm_rvu_priv_pfx_id_cfg_t pf_id_cfg;
	int pf;

	for (pf = 0; pf < octeontx_get_max_rvu_pfs(node); pf++) {
		pf_id_cfg.u = 0;
		pf_id_cfg.s.class_code = rvu_dev[pf].pci.class_code;
		pf_id_cfg.s.pf_devid = rvu_dev[pf].pci.pf_devid;
		pf_id_cfg.s.vf_devid = rvu_dev[pf].pci.vf_devid;
		CSR_WRITE(node, CAVM_RVU_PRIV_PFX_ID_CFG(pf), pf_id_cfg.u);
	}
}

static void msix_error_print_map(int node)
{
	int pf;

	for (pf = 0; pf < octeontx_get_max_rvu_pfs(node); pf++) {
		ERROR(" PF%d: num_pf_msix=%d, num_vfs=%d, total_vfs_msix=%d\n",
			pf, rvu_dev[pf].pf_num_msix_vec, rvu_dev[pf].num_vfs,
			rvu_dev[pf].vf_num_msix_vec);
	}
}

static int msix_enable(int node)
{
	uint32_t pf_msix_offset = 0, vf_msix_offset = VF_MSIX_BASE_IDX_NUMBER;
	int pf, msix_conf_count = 0;
	union cavm_rvu_priv_const priv_const;

	/* Read contents of RVU_PRIV_CONST */
	priv_const.u = CSR_READ(node, CAVM_RVU_PRIV_CONST);

	/* set AF MSIX table base*/
	union cavm_rvu_af_msixtr_base af_msix_cfg;

	af_msix_cfg.u = PF_MSIX_BASE;
	CSR_WRITE(node, CAVM_RVU_AF_MSIXTR_BASE, af_msix_cfg.u);

	/* set PF/VF msix table size and offset */
	for (pf = 0; pf < octeontx_get_max_rvu_pfs(node); pf++) {
		if (rvu_dev[pf].enable) {
			union cavm_rvu_priv_pfx_msix_cfg pfx_msix_cfg;

			/* Get the number of MSIX from rvu_dev array */
			pfx_msix_cfg.u = 0;
			pfx_msix_cfg.s.pf_msixt_offset = pf_msix_offset;
			pfx_msix_cfg.s.pf_msixt_sizem1 = rvu_dev[pf].pf_num_msix_vec - 1;
			pf_msix_offset += (rvu_dev[pf].pf_num_msix_vec);
			/* Increment number of already configured MSI-Xes */
			msix_conf_count += rvu_dev[pf].pf_num_msix_vec;

			/*
			 * If such occurs, we're overlapping with VF base, should
			 * never be reached since we're provisioning at most
			 * 256 MSIX vectors per PF, so FDT gave us wrong setup.
			 */
			assert(pf_msix_offset < VF_MSIX_BASE_IDX_NUMBER);

			if (rvu_dev[pf].num_vfs) {
				pfx_msix_cfg.s.vf_msixt_offset = vf_msix_offset;
				pfx_msix_cfg.s.vf_msixt_sizem1 =
					rvu_dev[pf].vf_num_msix_vec - 1;
				vf_msix_offset += ((rvu_dev[pf].num_vfs & MAX_RVU_VFS_PER_PF) *
						   rvu_dev[pf].vf_num_msix_vec);
				/*
				 * Increment number of already
				 * configured MSI-Xes
				 */
				msix_conf_count += ((rvu_dev[pf].num_vfs & MAX_RVU_VFS_PER_PF) *
						    rvu_dev[pf].vf_num_msix_vec);
			}

			/*
			 * Check if requested number of MSI-X does
			 * not exceedes number of available MSI-X by HRM.
			 * If such configuration is requested,
			 * print error and not configure more PFs.
			 * Return pf index which has invalid configuration.
			 */
			if (msix_conf_count > priv_const.s.max_msix) {
				ERROR("Invalid RVU MSI-X configuration!\n");
				ERROR("Disabling PFs (%d:%d)\n",
				      pf, (octeontx_get_max_rvu_pfs(node) - 1));
				msix_error_print_map(node);
				return pf;
			}

			CSR_WRITE(node, CAVM_RVU_PRIV_PFX_MSIX_CFG(pf), pfx_msix_cfg.u);
		}
	}

	return 0;
}

/* set total VFs and HWVFs for PFs */
static void enable_rvu_pf(int node, int pf)
{
	union cavm_rvu_priv_pfx_cfg pf_cfg;

	pf_cfg.u = 0;
	/* enable AF access when configuring the PF as AF or CPT */
	if ((pf == RVU_AF) || ((pf == RVU_PF15) && !bfdt->rvu_config.cpt_dis))
		pf_cfg.s.af_ena = TRUE;
	else
		pf_cfg.s.af_ena = FALSE;
	pf_cfg.s.ena = TRUE;
	pf_cfg.s.nvf = rvu_dev[pf].num_vfs;
	pf_cfg.s.first_hwvf = rvu_dev[pf].first_hwvf;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_CFG(pf), pf_cfg.u);
}

static void disable_rvu_pf(int node, int pf)
{
	union cavm_rvu_priv_pfx_cfg pf_cfg;

	pf_cfg.u = 0;
	pf_cfg.s.af_ena = 0;
	pf_cfg.s.ena = 0;
	pf_cfg.s.nvf = 0;
	pf_cfg.s.first_hwvf = 0;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_CFG(pf), pf_cfg.u);

}
/* Enable NPA for PF*/
static void pf_enable_npa(int node, int pf, int lf_id)
{
	union cavm_rvu_priv_pfx_npa_cfg pf_npa_cfg;
	union cavm_npa_priv_lfx_cfg npa_lf_cfg;

	pf_npa_cfg.u = 0;
	pf_npa_cfg.s.has_lf = 1;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_NPA_CFG(pf), pf_npa_cfg.u);

	npa_lf_cfg.u = 0;
	npa_lf_cfg.s.ena = 1;
	npa_lf_cfg.s.pf_func = (((pf & 0x3f) << 10) | 0x0);
	npa_lf_cfg.s.slot = 0;
	CSR_WRITE(node, CAVM_NPA_PRIV_LFX_CFG(lf_id), npa_lf_cfg.u);
}

/* Disable NPA for PF */
static void pf_disable_npa(int node, int pf, int lf_id)
{
	union cavm_rvu_priv_pfx_npa_cfg pf_npa_cfg;
	union cavm_npa_priv_lfx_cfg npa_lf_cfg;

	pf_npa_cfg.u = 0;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_NPA_CFG(pf), pf_npa_cfg.u);

	npa_lf_cfg.u = 0;
	CSR_WRITE(node, CAVM_NPA_PRIV_LFX_CFG(lf_id), npa_lf_cfg.u);
}

/* Enable NIX for PF*/
static void pf_enable_nix(int node, int pf, int lf_id)
{
	union cavm_rvu_priv_pfx_nixx_cfg pf_nix_cfg;
	union cavm_nixx_priv_lfx_cfg nix_lf_cfg;

	pf_nix_cfg.u = 0;
	pf_nix_cfg.s.has_lf = 1;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_NIXX_CFG(pf, 0), pf_nix_cfg.u);

	nix_lf_cfg.u = 0;
	nix_lf_cfg.s.ena = 1;
	nix_lf_cfg.s.pf_func = (((pf & 0x3f) << 10) | 0x0);
	nix_lf_cfg.s.slot = 0;
	CSR_WRITE(node, CAVM_NIXX_PRIV_LFX_CFG(0, lf_id), nix_lf_cfg.u);
}

/* Disable NIX for PF */
static void pf_disable_nix(int node, int nix, int pf, int lf_id)
{
	union cavm_rvu_priv_pfx_nixx_cfg pf_nix_cfg;
	union cavm_nixx_priv_lfx_cfg nix_lf_cfg;

	pf_nix_cfg.u = 0;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_NIXX_CFG(pf, nix), pf_nix_cfg.u);

	nix_lf_cfg.u = 0;
	CSR_WRITE(node, CAVM_NIXX_PRIV_LFX_CFG(nix, lf_id), nix_lf_cfg.u);
}

/* Reset all the resources before enabling PF */
static void reset_rvu_pf(int node, int pf)
{
	union cavm_rvu_priv_pfx_cptx_cfg cpt_cfg;
	union cavm_rvu_priv_pfx_int_cfg int_cfg;
	union cavm_rvu_priv_pfx_msix_cfg msix_cfg;
	union cavm_rvu_priv_pfx_nixx_cfg nix_cfg;
	union cavm_rvu_priv_pfx_npa_cfg npa_cfg;
	union cavm_rvu_priv_pfx_sso_cfg sso_cfg;
	union cavm_rvu_priv_pfx_ssow_cfg ssow_cfg;
	union cavm_rvu_priv_pfx_tim_cfg tim_cfg;

	if (!bfdt->rvu_config.cpt_dis) {	/* CPT is present */
		cpt_cfg.u = 0;
		cpt_cfg.s.num_lfs = 0;
		CSR_WRITE(node, CAVM_RVU_PRIV_PFX_CPTX_CFG(pf, 0), cpt_cfg.u);
	}

	int_cfg.u = 0;
	int_cfg.s.msix_offset = 0;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_INT_CFG(pf), int_cfg.u);

	msix_cfg.u = 0;
	msix_cfg.s.pf_msixt_offset = 0;
	msix_cfg.s.pf_msixt_sizem1 = 0;
	msix_cfg.s.vf_msixt_offset = 0;
	msix_cfg.s.vf_msixt_sizem1 = 0;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_MSIX_CFG(pf), msix_cfg.u);

	nix_cfg.u = 0;
	nix_cfg.s.has_lf = 0;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_NIXX_CFG(pf, 0), nix_cfg.u);

	npa_cfg.u = 0;
	npa_cfg.s.has_lf = 0;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_NPA_CFG(pf), npa_cfg.u);

	sso_cfg.u = 0;
	sso_cfg.s.num_lfs = 0;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_SSO_CFG(pf), sso_cfg.u);

	ssow_cfg.u = 0;
	ssow_cfg.s.num_lfs = 0;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_SSOW_CFG(pf), ssow_cfg.u);

	tim_cfg.u = 0;
	tim_cfg.s.num_lfs = 0;
	CSR_WRITE(node, CAVM_RVU_PRIV_PFX_TIM_CFG(pf), tim_cfg.u);
}

/* Returns max LFs supported by NPA */
static int octeontx2_get_max_npa_lfs(int node)
{
	cavm_npa_af_const_t npa_af_const;

	npa_af_const.u = CSR_READ(node, CAVM_NPA_AF_CONST);

	return npa_af_const.s.lfs;
}

/* Returns max LFs supported by NIX */
static int octeontx2_get_max_nix_lfs(int node, int nix)
{
	cavm_nixx_af_const2_t nixx_af_const2;

	nixx_af_const2.u = CSR_READ(node, CAVM_NIXX_AF_CONST2(nix));

	return nixx_af_const2.s.lfs;
}

/* Exported functions */
int octeontx2_clear_lf_to_pf_mapping(int node)
{
	int pf, lf;

	for (pf = 0; pf < octeontx_get_max_rvu_pfs(node); pf++) {
		for (lf = 0; lf < octeontx2_get_max_nix_lfs(node, 0); lf++) {
			pf_disable_nix(node, 0, pf, lf);
		}

		for (lf = 0; lf < octeontx2_get_max_npa_lfs(node); lf++) {
			pf_disable_npa(node, pf, lf);
		}
	}

	return 0;
}

void octeontx_rvu_init(int node)
{
	int pf, rc;
	int npalf_id = 0, nixlf_id = 0;

	rc = octeontx_init_rvu_from_fdt();
	if (rc < 0)
		return;

	dump_rvu_devs(node);

	for (pf = 0 ; pf < octeontx_get_max_rvu_pfs(node); pf++) {
		if (rvu_dev[pf].enable) {
			reset_rvu_pf(node, pf);
			enable_rvu_pf(node, pf);
			if (rvu_dev[pf].pf_res_ena) {
				pf_enable_nix(node, pf, nixlf_id);
				nixlf_id++;
				pf_enable_npa(node, pf, npalf_id);
				npalf_id++;
			}
		}
		else 	/* Disable unused PFs */
			disable_rvu_pf(node, pf);
	}

	rc = msix_enable(node);
	if (rc) {
		for (pf = rc; pf < octeontx_get_max_rvu_pfs(node); pf++) {
			disable_rvu_pf(node, pf);
			reset_rvu_pf(node, pf);
		}
	}

	mailbox_enable(node);
	config_rvu_pci(node);
}
