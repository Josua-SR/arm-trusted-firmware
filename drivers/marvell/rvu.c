/*
 * Copyright (C) 2016-2018 Marvell International Ltd.
 * This program is provided "as is" without any warranty of any kind,
 * and is distributed under the applicable Marvell proprietary limited use
 * license agreement.
 */

/* RVU driver for OcteonTX2 */

#include <stdio.h>
#include <arch.h>
#include <platform_def.h>
#include <octeontx_common.h>
#include <plat_board_cfg.h>
#include <rvu.h>
#include <plat/common/platform.h>
#include <assert.h>
#include <debug.h>
#include <octeontx_utils.h>
#include <plat_scfg.h>
#include <plat_otx2_configuration.h>

#include "cavm-csrs-cpt.h"
#include "cavm-csrs-ndc.h"
#include "cavm-csrs-nix.h"
#include "cavm-csrs-npa.h"
#include "cavm-csrs-pccpf.h"
#include "cavm-csrs-pem.h"
#include "cavm-csrs-rvu.h"
#include "cavm-csrs-sso.h"
#include "cavm-csrs-tim.h"

#ifdef DEBUG_ATF_RVU
#define debug_rvu printf
#else
#define debug_rvu(...) ((void) (0))
#endif

struct sw_rvu_dev_info {
	int               type; /* SW_RVU_xxx_PF */
	int               num;  /* SW_RVU_xxx_NUM_PF */
	struct pci_config pci;
};

/* Stores CGX/LMAC data for RVU PF dynamic provisioning */
struct rvu_pf_cgx_lmac {
	struct cgx_lmac_config *lmac;
	uint8_t                cgx_id;
	uint8_t                lmac_id;
};

static struct rvu_device rvu_dev[MAX_RVU_PFS];

static inline int octeontx_get_msix_for_npa(void)
{
	union cavm_npa_priv_lfx_int_cfg npa_int_cfg;
	int lf = 0;

	npa_int_cfg.u = CSR_READ(CAVM_NPA_PRIV_LFX_INT_CFG(lf));

	return npa_int_cfg.s.msix_size;
}

static inline int octeontx_get_msix_for_cgx(void)
{
	int lf = 0, nix = 0;
	union cavm_nixx_priv_lfx_int_cfg nixx_int_cfg;

	nixx_int_cfg.u = CSR_READ(CAVM_NIXX_PRIV_LFX_INT_CFG(nix, lf));

	return nixx_int_cfg.s.msix_size + octeontx_get_msix_for_npa();
}

static void octeontx_init_rvu_af(int *hwvf)
{
	rvu_dev[RVU_AF].enable = TRUE;
	rvu_dev[RVU_AF].num_vfs = plat_octeontx_bcfg->rvu_config.admin_pf.num_rvu_vfs;
	rvu_dev[RVU_AF].first_hwvf = *hwvf;
	rvu_dev[RVU_AF].pf_num_msix_vec = plat_octeontx_bcfg->rvu_config.admin_pf.num_msix_vec;
	rvu_dev[RVU_AF].vf_num_msix_vec = RVU_VF_INT_VEC_COUNT +
					  octeontx_get_msix_for_cgx();
	rvu_dev[RVU_AF].pf_res_ena = FALSE;
	rvu_dev[RVU_AF].pci.pf_devid = CAVM_PCC_DEV_IDL_E_RVU_AF & DEVID_MASK;
	rvu_dev[RVU_AF].pci.vf_devid = CAVM_PCC_DEV_IDL_E_SW_RVU_AF_VF & DEVID_MASK;
	rvu_dev[RVU_AF].pci.class_code = RVU_CLASS_CODE & CLASS_CODE_MASK;
	/* Increment already allocated HWVFs */
	*hwvf += rvu_dev[RVU_AF].num_vfs;
}

static rvu_sw_rvu_pf_t *find_sw_rvu_pf_info(int bfdt_index)
{
	if (bfdt_index < SW_RVU_NUM_PF)
		return &(plat_octeontx_bcfg->rvu_config.sw_pf[bfdt_index]);

	assert(bfdt_index < SW_RVU_NUM_PF);
	return NULL;
}

static struct sw_rvu_dev_info *find_sw_rvu_dev(int bfdt_index)
{
	static struct sw_rvu_dev_info sw_dev_list[] = {
		{ SW_RVU_SSO_TIM_PF(0), SW_RVU_SSO_TIM_NUM_PF,
		  { .pf_devid = CAVM_PCC_DEV_IDL_E_SW_RVU_SSO_TIM_PF,
		    .vf_devid = CAVM_PCC_DEV_IDL_E_SW_RVU_SSO_TIM_VF,
		    .class_code = GSP_CLASS_CODE
		  } },
		{ SW_RVU_NPA_PF(0), SW_RVU_NPA_NUM_PF,
		  { .pf_devid = CAVM_PCC_DEV_IDL_E_SW_RVU_NPA_PF,
		    .vf_devid = CAVM_PCC_DEV_IDL_E_SW_RVU_NPA_VF,
		    .class_code = GSP_CLASS_CODE
		  } },
		{ SW_RVU_SDP_PF(0), SW_RVU_SDP_NUM_PF,
		  { .pf_devid = CAVM_PCC_DEV_IDL_E_SW_RVU_SDP_PF,
		    .vf_devid = CAVM_PCC_DEV_IDL_E_SW_RVU_SDP_VF,
		    .class_code = GSP_CLASS_CODE
		  } },
		{ SW_RVU_CPT_PF(0), SW_RVU_CPT_NUM_PF,
		  { .pf_devid = CAVM_PCC_DEV_IDL_E_SW_RVU_CPT_PF,
		    .vf_devid = CAVM_PCC_DEV_IDL_E_SW_RVU_CPT_VF,
		    .class_code = CPT_CLASS_CODE
		  } },
	}, *sw_dev;
	int i;

	sw_dev = NULL;
	for (i = 0; !sw_dev && (i < ARRAY_SIZE(sw_dev_list)); i++)
		if ((bfdt_index >= sw_dev_list[i].type) &&
		    (bfdt_index < (sw_dev_list[i].type + sw_dev_list[i].num)))
			sw_dev = &sw_dev_list[i];

	return sw_dev;
}

static void octeontx_init_rvu_fixed(int *hwvf, int rvu, int bfdt_index,
				    int has_vfs)
{
	struct sw_rvu_dev_info *sw_dev;
	rvu_sw_rvu_pf_t *sw_pf;

	sw_pf = find_sw_rvu_pf_info(bfdt_index);
	if (!sw_pf) {
		ERROR("Internal: cannot locate SW_RVU_xxx %d PF info!\n",
		      bfdt_index);
		return;
	}

	sw_dev = find_sw_rvu_dev(bfdt_index);
	if (!sw_dev) {
		ERROR("Internal: cannot locate SW_RVU_xxx %d device!\n",
		      bfdt_index);
		return;
	}

	rvu_dev[rvu].enable = TRUE;
	rvu_dev[rvu].num_vfs = has_vfs ? sw_pf->num_rvu_vfs : 0;
	rvu_dev[rvu].first_hwvf = has_vfs ? *hwvf : 0;
	rvu_dev[rvu].pf_num_msix_vec = sw_pf->num_msix_vec;
	rvu_dev[rvu].pf_res_ena = FALSE;

	rvu_dev[rvu].pci.pf_devid = sw_dev->pci.pf_devid & DEVID_MASK;
	rvu_dev[rvu].pci.vf_devid = sw_dev->pci.vf_devid & DEVID_MASK;
	rvu_dev[rvu].pci.class_code = sw_dev->pci.class_code & CLASS_CODE_MASK;
	rvu_dev[rvu].vf_num_msix_vec = has_vfs ? sw_pf->num_msix_vec : 0;

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
	rvu_dev[rvu].vf_num_msix_vec = cgx->lmac_cfg[lmac_id].num_msix_vec;
	rvu_dev[rvu].pf_res_ena = TRUE;
	rvu_dev[rvu].pci.pf_devid = CAVM_PCC_DEV_IDL_E_RVU & DEVID_MASK;
	rvu_dev[rvu].pci.vf_devid = CAVM_PCC_DEV_IDL_E_RVU_VF & DEVID_MASK;
	rvu_dev[rvu].pci.class_code = RVU_CLASS_CODE & CLASS_CODE_MASK;
	/* Increment already allocated HWVFs */
	*hwvf += rvu_dev[rvu].num_vfs;
}

/* Return true if any PEM is in EP mode */
static int octeontx_is_in_ep_mode(void)
{
	cavm_pemx_cfg_t pemx_cfg;
	cavm_pemx_on_t pemx_on;
	int pem, pem_num;

	pem_num = plat_octeontx_get_pem_count();

	for (pem = 0; pem < pem_num; pem++) {
		/* Check pemon and hostmd bits of PEM0 for EP mode */
		pemx_on.u = CSR_READ(CAVM_PEMX_ON(pem));
		pemx_cfg.u = CSR_READ(CAVM_PEMX_CFG(pem));
		if (pemx_on.cn9.pemon && !pemx_cfg.cn9.hostmd)
			return 1;
	}

	/* Device is not in EP mode */
	return 0;
}

/*
 * rvu_provision_pfs_for_sw_devs()
 *
 * Performs provisioning of RVU PFs to SW_RVU_xxx devices.
 *
 * RVU PFs for these devices are provisioned dynamically from the 'top'
 * of the RVU PFs allocated for CGX LMACs (i.e. RVU PF <last-3>).
 * Depending upon the "provision-mode", RVU PFs MAY be allocated to
 * the SW_RVU_xxx devices which 'override' an RVU PF reserved for a CGX LMAC.
 *
 * Refer to the RVU PF provisioning document for more information.
 *
 * on entry,
 *   rvu_pf:        starting RVU PF number to provision
 *   top_cgx_pf:    [numerically] highest RVU PF # reserved for CGX LMACs
 *   cur_hwvf:      [pointer to] starting RVU SR-IOV VF number to provision
 *                  (i.e. 0 ... RVU_PRIV_CONST[HWVFS]-1)
 *   uninit_pf_cnt: [pointer to] count of uninitialized RVU PFs
 *                  (i.e. available RVU PFs)
 *   cgx_lmac_list: [pointer to] list of CGX LMAC data
 *                  (used to 'override' an RVU PF allocated to CGX)
 *
 * returns,
 *   [numerically] highest RVU PF remaining to be provisioned from
 *   range reserved for CGX LMACs
 *
 */
static int rvu_provision_pfs_for_sw_devs(int rvu_pf_start, int top_cgx_pf,
					 int *cur_hwvf, int *uninit_pf_cnt,
					 struct rvu_pf_cgx_lmac *cgx_lmac_list)
{
	rvu_sw_rvu_pf_t *sw_pf;
	int i, rvu_pf;

	rvu_pf = rvu_pf_start;

	/* IF a PCI PEM is in Endpoint mode, provision RVU PFs for SDP */
	for (i = SW_RVU_SDP_NUM_PF - 1; (int)i >= 0; i--) {
		/* Enforce constraint */
		if (rvu_pf < RVU_CGX_FIRST) {
			ERROR("RVU: too many SW_RVU_xxx devices.\n");
			break;
		}

		if (!octeontx_is_in_ep_mode())
			break;

		sw_pf = find_sw_rvu_pf_info(SW_RVU_SDP_PF(i));
		assert(sw_pf != NULL);

		/* [single SDP] already LEGACY-mapped */
		if (sw_pf->mapping == SW_RVU_MAP_LEGACY)
			continue;

		if (sw_pf->mapping == SW_RVU_MAP_NONE) {
			debug_rvu(
			  "RVU: NOT provisioning PF for SW_RVU_SDP #%d\n", i);
			continue;
		}

		if ((sw_pf->mapping == SW_RVU_MAP_AVAILABLE) &&
			 (rvu_pf <= top_cgx_pf)) {
			debug_rvu(
			  "RVU: cannot provision PF for SW_RVU_SDP #%d\n",
			  i);
			continue;
		}

		assert((sw_pf->mapping == SW_RVU_MAP_FORCE) ||
		       (sw_pf->mapping == SW_RVU_MAP_AVAILABLE));

		debug_rvu("RVU: provision PF%d -> SW_RVU_SDP #%d\n",
			  rvu_pf, i);

		octeontx_init_rvu_fixed(cur_hwvf, rvu_pf, SW_RVU_SDP_PF(i),
					TRUE);
		/*
		 * If this PF was already provisioned to CGX,
		 * disable the lmac so CGX driver will not use it.
		 *
		 * Otherwise, adjust uninitialized count.
		 */
		if (rvu_pf <= top_cgx_pf) {
			cgx_lmac_list[rvu_pf].lmac->lmac_enable = 0;
			debug_rvu("RVU: replacing CGX%d/LMAC%d with SDP\n",
				  cgx_lmac_list[rvu_pf].cgx_id,
				  cgx_lmac_list[rvu_pf].lmac_id);
		} else /* i.e. rvu_pf > top_cgx_pf */ {
			*uninit_pf_cnt -= 1;
			debug_rvu("RVU: Decrement uninit_pfs -> %d\n",
				  *uninit_pf_cnt);
		}

		rvu_pf--;
	}

	return rvu_pf;
}

static int octeontx_init_rvu_from_fdt(void)
{
	int cgx_id, lmac_id, pf, current_hwvf = 0;
	int uninit_pfs = 0, sso_tim_pfs, npa_pfs;
	int top_cgx_pf, top_uninit_pf;
	rvu_sw_rvu_pf_t *sw_pf;
	cgx_config_t *cgx;
	struct rvu_pf_cgx_lmac cgx_lmac_list[MAX_CGX * MAX_LMAC_PER_CGX];

	/* Check if FDT config is valid */
	if (!(plat_octeontx_bcfg->rvu_config.valid)) {
		ERROR("Invalid RVU configuration, skipping RVU init!.\n");
		return -1;
	}

	/*
	 * Firstly, initialize fixed setup
	 *     (PF0, PF(last-2), PF(last-1), PF(last))
	 */
	/* Init RVU0 - AF (PF0) */
	octeontx_init_rvu_af(&current_hwvf);

	/* Init RVU(last-2) - SSO_TIM (PF(last-2)) */
	octeontx_init_rvu_fixed(&current_hwvf, RVU_SSO_TIM,
				SW_RVU_SSO_TIM_PF(0), TRUE);

	/*
	 * Init RVU(last-1) - NPA (PF(last-1))
	 *
	 * Check for 'legacy' SDP provisioning, which is dependent upon ALL of:
	 *   FDT entry indicates 'LEGACY'
	 *   at least one PEM is in Endpoint mode
	 *
	 * For 'legacy' SDP, provision PF(last-1) as SDP instead of NPA.
	 */
	sw_pf = find_sw_rvu_pf_info(SW_RVU_SDP_PF(0));
	assert(sw_pf != NULL);
	if (sw_pf && (sw_pf->mapping == SW_RVU_MAP_LEGACY) &&
	    octeontx_is_in_ep_mode()) {
		debug_rvu("RVU: provision PF%d -> SW_RVU_SDP (override NPA)\n",
			  RVU_NPA);
		octeontx_init_rvu_fixed(&current_hwvf, RVU_NPA,
					SW_RVU_SDP_PF(0), TRUE);
	} else
		octeontx_init_rvu_fixed(&current_hwvf, RVU_NPA,
					SW_RVU_NPA_PF(0), TRUE);

	if (plat_octeontx_bcfg->rvu_config.cpt_dis)
		uninit_pfs = 1;
	else {
		/* Init last RVU - as CPT if present */
		octeontx_init_rvu_fixed(&current_hwvf, RVU_LAST,
				SW_RVU_CPT_PF(0), TRUE);
	}

	/*
	 * The CGX PFs need to be provisioned.
	 * However, some of the RVU PFs reserved for CGX can be
	 * provisioned for SW_RVU_xxx devices instead, depending upon
	 * the "provision-mode" property of the SW_RVU_xxx device.
	 *
	 * These 'overridden' CGX devices may not consume the same number
	 * of LFs that the SW_RVU_xxx devices consume.
	 * Since the allocation of LFs to RVU PFs is done contiguously,
	 * the CGX devices cannot yet be allocated (since they might
	 * subsequently be overridden, this could produce gaps in the LF space).
	 *
	 * So, the count of RVU PFs is calculated first, without actually
	 * allocating LFs to the CGX devices.
	 *
	 * Then, the provisioning of RVU PFs to SW_RVU_xxx devices is
	 * performed, 'overriding' CGX allocations as necessary.
	 *
	 * Finally, after provisioning for all the SW_RVU_xxx devices has
	 * completed, the provisioning for CGX devices can be done.
	 * This ensures that the LFs are allocated in a contiguous manner,
	 * with no gaps.
	 */

	pf = RVU_CGX_FIRST;
	/* This represents the [numerically] highest RVU PF required by CGX */
	top_cgx_pf = RVU_CGX_FIRST;

	/* Determine the required number of CGX LMAC PFs */
	for (cgx_id = 0; cgx_id < MAX_CGX; cgx_id++) {
		cgx = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);
		if (cgx->enable && !cgx->is_rfoe) {
			for (lmac_id = 0; lmac_id < MAX_LMAC_PER_CGX;
			     lmac_id++) {
				if (cgx->lmac_cfg[lmac_id].lmac_enable) {
					/* Save for possible re-allocation */
					cgx_lmac_list[pf].lmac =
						&cgx->lmac_cfg[lmac_id];
					cgx_lmac_list[pf].cgx_id = cgx_id;
					cgx_lmac_list[pf].lmac_id = lmac_id;

					top_cgx_pf = pf++;
				} else
					uninit_pfs++;
			}
		} else {
			/* all four LMACs are not initialized */
			uninit_pfs += MAX_LMAC_PER_CGX;
		}
	}

	debug_rvu("RVU: PF%d is last PF required for CGX\n", top_cgx_pf);

	/*
	 * Now, provision RVU PFs for SW_RVU_xxx devices DOWNWARD starting from
	 * END of [required] CGX PFs.
	 */
	top_uninit_pf = rvu_provision_pfs_for_sw_devs(RVU_CGX_LAST, top_cgx_pf,
						      &current_hwvf,
						      &uninit_pfs,
						      cgx_lmac_list);

	/*
	 * Finally, after any possible 'overrides' by SW_RVU_xxx devices,
	 * perform actual provisioning of RVU PFs to CGX devices.
	 */
	pf = RVU_CGX_FIRST;
	for (cgx_id = 0; cgx_id < MAX_CGX; cgx_id++) {
		cgx = &(plat_octeontx_bcfg->cgx_cfg[cgx_id]);
		if (cgx->enable && !cgx->is_rfoe) {
			for (lmac_id = 0; lmac_id < MAX_LMAC_PER_CGX; lmac_id++) {
				if (cgx->lmac_cfg[lmac_id].lmac_enable) {
					/* Sanity check */
					assert(cgx_lmac_list[pf].lmac ==
					       &cgx->lmac_cfg[lmac_id]);
					assert(cgx_lmac_list[pf].cgx_id ==
					       cgx_id);
					assert(cgx_lmac_list[pf].lmac_id ==
					       lmac_id);
					octeontx_init_rvu_lmac(&current_hwvf,
							       pf, cgx,
							       lmac_id);
					assert(current_hwvf <= MAX_RVU_HWVFS);
					pf++;
				}
			}
		}
	}

	debug_rvu("RVU: PF%d was last PF provisioned for CGX\n", pf-1);
	debug_rvu("RVU: PF%d is top uninitialized PF, count %d\n",
		  top_uninit_pf, uninit_pfs);

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
		/* If CPT was disabled, its reserved PF is uninitialized. */
		if (pf > top_uninit_pf)
			pf = RVU_LAST;
		octeontx_init_rvu_fixed(&current_hwvf, pf++,
					SW_RVU_SSO_TIM_PF(0), FALSE);
		sso_tim_pfs--;
	}

	while (npa_pfs > 0) {
		/* If CPT was disabled, its reserved PF is uninitialized. */
		if (pf > top_uninit_pf)
			pf = RVU_LAST;
		octeontx_init_rvu_fixed(&current_hwvf, pf++,
					SW_RVU_NPA_PF(0), FALSE);
		npa_pfs--;
	}

	return 0;
}

/* returns max PFs supported by RVU */
static int octeontx_get_max_rvu_pfs(void)
{
	union cavm_rvu_priv_const priv_const;

	priv_const.u = CSR_READ(CAVM_RVU_PRIV_CONST);

	return priv_const.s.pfs;
}

/* returns max VFs supported by RVU */
static int octeontx_get_max_rvu_vfs(void)
{
	union cavm_rvu_priv_const priv_const;

	priv_const.u = CSR_READ(CAVM_RVU_PRIV_CONST);

	return priv_const.s.max_vfs_per_pf;
}

static void dump_rvu_devs(void)
{
	int pf;

	for (pf = 0; pf < octeontx_get_max_rvu_pfs(); pf++) {
		debug_rvu("******************************************\n");
		debug_rvu("PF%d: enable=%d, num_vfs=%d, first_hwvf=%d\n"
			  "pf_num_msix_vec=%d, vf_num_msix_vec=%d\n"
			  "pf_res_ena=%d\n",
			  pf, rvu_dev[pf].enable, rvu_dev[pf].num_vfs,
			  rvu_dev[pf].first_hwvf, rvu_dev[pf].pf_num_msix_vec,
			  rvu_dev[pf].vf_num_msix_vec, rvu_dev[pf].pf_res_ena);
		debug_rvu("PCI Settings:\n"
			  "pf_devid=0x%x, vf_devid=0x%x, class_code=0x%x\n",
			  rvu_dev[pf].pci.pf_devid, rvu_dev[pf].pci.vf_devid,
			  rvu_dev[pf].pci.class_code);
	}

	debug_rvu("******************************************\n");
}

/* set mailbox memory*/
static void mailbox_enable(void)
{
	int pf;
	static uint64_t vf_base = VF_MBOX_BASE;
	union cavm_rvu_af_pf_bar4_addr pf_bar4_addr;

	pf_bar4_addr.u = PF_MBOX_BASE;
	CSR_WRITE(CAVM_RVU_AF_PF_BAR4_ADDR, pf_bar4_addr.u);

	for (pf = 0; pf < octeontx_get_max_rvu_pfs(); pf++) {
		if (rvu_dev[pf].enable && rvu_dev[pf].num_vfs) {
			uint64_t bar4_addr = CAVM_RVU_BAR_E_RVU_PFX_FUNCX_BAR2(pf, 0) + 0x10;
			octeontx_write64(bar4_addr, vf_base);
			vf_base = vf_base + (0x10000 * (rvu_dev[pf].num_vfs & 0x7f));
		}
	}
}

/* Initialize PCI PF_DEVID and VF_DEVID */
static void config_rvu_pci(void)
{
	cavm_rvu_priv_pfx_id_cfg_t pf_id_cfg;
	int pf;

	for (pf = 0; pf < octeontx_get_max_rvu_pfs(); pf++) {
		pf_id_cfg.u = 0;
		pf_id_cfg.s.class_code = rvu_dev[pf].pci.class_code;
		pf_id_cfg.s.pf_devid = rvu_dev[pf].pci.pf_devid;
		pf_id_cfg.s.vf_devid = rvu_dev[pf].pci.vf_devid;
		CSR_WRITE(CAVM_RVU_PRIV_PFX_ID_CFG(pf), pf_id_cfg.u);
	}
}

static void msix_error_print_map(void)
{
	int pf;

	for (pf = 0; pf < octeontx_get_max_rvu_pfs(); pf++) {
		ERROR(" PF%d: num_pf_msix=%d, num_vfs=%d, total_vfs_msix=%d\n",
			pf, rvu_dev[pf].pf_num_msix_vec, rvu_dev[pf].num_vfs,
			rvu_dev[pf].vf_num_msix_vec);
	}
}

/*
 * Configure AF interrupt map at PF's MSI-X table.
 */
static void conf_msix_admin_blk_offset(void)
{
	union cavm_rvu_priv_pfx_int_cfg af_int_cfg;
	union cavm_nixx_priv_af_int_cfg nix_int_cfg;
	union cavm_npa_priv_af_int_cfg npa_int_cfg;
	union cavm_sso_priv_af_int_cfg sso_int_cfg;
	union cavm_tim_priv_af_int_cfg tim_int_cfg;
	union cavm_ndcx_priv_af_int_cfg ndc_int_cfg;
	union cavm_cptx_priv_af_int_cfg	cpt_int_cfg;
	int af_msix_used = 0, i = 0;
	uint64_t midr;

	/*
	 * AF interrupt vectors enumerated by RVU_AF_INT_VEC_E
	 * have a fixed starting offset (0) in RVU PF(0)'s
	 * MSI-X table, so just increment used MSI-X.
	 * There's no register that holds AF_INT_VEC_E size.
	 */
	af_msix_used += RVU_AF_INT_VEC_E_MSIX_SIZE;

	/* Configure RVU_PF_INT_VEC_E right next to RVU_AF */
	af_int_cfg.u = CSR_READ(CAVM_RVU_PRIV_PFX_INT_CFG(0));
	af_int_cfg.s.msix_offset = af_msix_used;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_INT_CFG(0), af_int_cfg.u);
	af_msix_used += af_int_cfg.s.msix_size;

	/*
	 * Configure next blocks accordingly to the number
	 * of MSI-X AF interrupts already consumed
	 */
	nix_int_cfg.u = CSR_READ(CAVM_NIXX_PRIV_AF_INT_CFG(0));
	nix_int_cfg.s.msix_offset = af_msix_used;
	CSR_WRITE(CAVM_NIXX_PRIV_AF_INT_CFG(0), nix_int_cfg.u);
	af_msix_used += nix_int_cfg.s.msix_size;

	npa_int_cfg.u = CSR_READ(CAVM_NPA_PRIV_AF_INT_CFG);
	npa_int_cfg.s.msix_offset = af_msix_used;
	CSR_WRITE(CAVM_NPA_PRIV_AF_INT_CFG, npa_int_cfg.u);
	af_msix_used += npa_int_cfg.s.msix_size;

	sso_int_cfg.u = CSR_READ(CAVM_SSO_PRIV_AF_INT_CFG);
	sso_int_cfg.s.msix_offset = af_msix_used;
	CSR_WRITE(CAVM_SSO_PRIV_AF_INT_CFG, sso_int_cfg.u);
	af_msix_used += sso_int_cfg.s.msix_size;

	tim_int_cfg.u = CSR_READ(CAVM_TIM_PRIV_AF_INT_CFG);
	tim_int_cfg.s.msix_offset = af_msix_used;
	CSR_WRITE(CAVM_TIM_PRIV_AF_INT_CFG, tim_int_cfg.u);
	af_msix_used += tim_int_cfg.s.msix_size;

	midr = read_midr();
	for (i = 0; i < 3; i++) {
		/* Workaround for errata NDC-34917,
		 * set NDC_PRIV_AF_INT_CFG[MSIX_OFFSET] = 0
		 */
		if (IS_OCTEONTX_PASS(midr, T96PARTNUM, 1, 0)) {
			CSR_WRITE(CAVM_NDCX_PRIV_AF_INT_CFG(i), 0x0);
			continue;
		}
		ndc_int_cfg.u = CSR_READ(CAVM_NDCX_PRIV_AF_INT_CFG(i));
		ndc_int_cfg.s.msix_offset = af_msix_used;
		CSR_WRITE(CAVM_NDCX_PRIV_AF_INT_CFG(i), ndc_int_cfg.u);
		af_msix_used += ndc_int_cfg.s.msix_size;
	}

	if (!plat_octeontx_bcfg->rvu_config.cpt_dis) {
		cpt_int_cfg.u = CSR_READ(CAVM_CPTX_PRIV_AF_INT_CFG(0));
		cpt_int_cfg.s.msix_offset = af_msix_used;
		CSR_WRITE(CAVM_CPTX_PRIV_AF_INT_CFG(0), cpt_int_cfg.u);
		af_msix_used += cpt_int_cfg.s.msix_size;
	}

	/* Sanity check for incorrect FDT setup */
	assert(af_msix_used <= rvu_dev[0].pf_num_msix_vec);
}

static int msix_enable(void)
{
	uint32_t msix_offset = 0;
	int pf;
	union cavm_rvu_priv_const priv_const;

	/* Read contents of RVU_PRIV_CONST */
	priv_const.u = CSR_READ(CAVM_RVU_PRIV_CONST);

	/* set AF MSIX table base*/
	union cavm_rvu_af_msixtr_base af_msix_cfg;

	af_msix_cfg.u = MSIX_TABLE_BASE;
	CSR_WRITE(CAVM_RVU_AF_MSIXTR_BASE, af_msix_cfg.u);

	/*
	 * Check if reserved memory for MSI-X table is big enough to hold all
	 * possible MSI-Xes.
	 */
	assert(priv_const.s.max_msix * RVU_MSIX_VEC_SIZE <= MSIX_TABLE_SIZE);

	/* Configure AF interrupt offsets at PF0 */
	conf_msix_admin_blk_offset();

	/* set PF/VF msix table size and offset */
	for (pf = 0; pf < octeontx_get_max_rvu_pfs(); pf++) {
		if (!rvu_dev[pf].enable)
			continue;

		union cavm_rvu_priv_pfx_msix_cfg pfx_msix_cfg;

		/* Get the number of MSIX from rvu_dev array */
		pfx_msix_cfg.u = 0;
		pfx_msix_cfg.s.pf_msixt_offset = msix_offset;
		pfx_msix_cfg.s.pf_msixt_sizem1 =
				rvu_dev[pf].pf_num_msix_vec - 1;
		msix_offset += (rvu_dev[pf].pf_num_msix_vec);

		/* This is workaround for errata RVU-36163 */
		if (IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 1) ||
		    IS_OCTEONTX_PN(read_midr(), F95PARTNUM) ||
		    IS_OCTEONTX_PN(read_midr(), F95MMPARTNUM)) {
			/* If pf_msix_offset needs alignment */
			if (msix_offset & RVU_36163_ALIGNMENT_MASK) {
				msix_offset += RVU_36163_OFFSET_ALIGNMENT;
				msix_offset &= ~(RVU_36163_ALIGNMENT_MASK);
			}
		}

		if (rvu_dev[pf].num_vfs) {
			pfx_msix_cfg.s.vf_msixt_offset = msix_offset;
			pfx_msix_cfg.s.vf_msixt_sizem1 =
				rvu_dev[pf].vf_num_msix_vec - 1;
			msix_offset += ((rvu_dev[pf].num_vfs &
				(MAX_RVU_VFS_PER_PF - 1)) *
				rvu_dev[pf].vf_num_msix_vec);

			/* This is workaround for errata RVU-36163 */
			if (IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 1) ||
			    IS_OCTEONTX_PN(read_midr(), F95PARTNUM) ||
			    IS_OCTEONTX_PN(read_midr(), F95MMPARTNUM)) {
				/* If vf_msix_offset needs alignment */
				if (msix_offset & RVU_36163_ALIGNMENT_MASK) {
					msix_offset +=
						RVU_36163_OFFSET_ALIGNMENT;
					msix_offset &=
						~(RVU_36163_ALIGNMENT_MASK);
				}
			}
		}

		/*
		 * Check if requested number of MSI-X does
		 * not exceedes number of available MSI-X by HRM.
		 * If such configuration is requested,
		 * print error and not configure more PFs.
		 * Return pf index which has invalid configuration.
		 */
		if (msix_offset > priv_const.s.max_msix) {
			ERROR("Invalid RVU MSI-X configuration!\n");
			ERROR("Disabling PFs (%d:%d)\n",
			      pf, (octeontx_get_max_rvu_pfs() - 1));
			msix_error_print_map();
			return pf;
		}

		CSR_WRITE(CAVM_RVU_PRIV_PFX_MSIX_CFG(pf), pfx_msix_cfg.u);
	}

	return 0;
}

/* set total VFs and HWVFs for PFs */
static void enable_rvu_pf(int pf)
{
	union cavm_rvu_priv_pfx_cfg pf_cfg;

	pf_cfg.u = 0;
	/* enable AF access when configuring the PF as AF */
	if (pf == RVU_AF)
		pf_cfg.s.af_ena = TRUE;
	else
		pf_cfg.s.af_ena = FALSE;
	pf_cfg.s.ena = TRUE;
	pf_cfg.s.nvf = rvu_dev[pf].num_vfs;
	pf_cfg.s.first_hwvf = rvu_dev[pf].first_hwvf;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_CFG(pf), pf_cfg.u);
}

static void disable_rvu_pf(int pf)
{
	union cavm_rvu_priv_pfx_cfg pf_cfg;

	pf_cfg.u = 0;
	pf_cfg.s.af_ena = 0;
	pf_cfg.s.ena = 0;
	pf_cfg.s.nvf = 0;
	pf_cfg.s.first_hwvf = 0;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_CFG(pf), pf_cfg.u);

}
/* Enable NPA for PF*/
static void pf_enable_npa(int pf, int lf_id)
{
	union cavm_rvu_priv_pfx_npa_cfg pf_npa_cfg;
	union cavm_npa_priv_lfx_cfg npa_lf_cfg;

	pf_npa_cfg.u = 0;
	pf_npa_cfg.s.has_lf = 1;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_NPA_CFG(pf), pf_npa_cfg.u);

	npa_lf_cfg.u = 0;
	npa_lf_cfg.s.ena = 1;
	npa_lf_cfg.s.pf_func = (((pf & 0x3f) << 10) | 0x0);
	npa_lf_cfg.s.slot = 0;
	CSR_WRITE(CAVM_NPA_PRIV_LFX_CFG(lf_id), npa_lf_cfg.u);
}

/* Disable NPA for PF */
static void pf_disable_npa(int pf, int lf_id)
{
	union cavm_rvu_priv_pfx_npa_cfg pf_npa_cfg;
	union cavm_npa_priv_lfx_cfg npa_lf_cfg;

	pf_npa_cfg.u = 0;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_NPA_CFG(pf), pf_npa_cfg.u);

	npa_lf_cfg.u = 0;
	CSR_WRITE(CAVM_NPA_PRIV_LFX_CFG(lf_id), npa_lf_cfg.u);
}

/* Enable NIX for PF*/
static void pf_enable_nix(int pf, int lf_id)
{
	union cavm_rvu_priv_pfx_nixx_cfg pf_nix_cfg;
	union cavm_nixx_priv_lfx_cfg nix_lf_cfg;

	pf_nix_cfg.u = 0;
	pf_nix_cfg.s.has_lf = 1;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_NIXX_CFG(pf, 0), pf_nix_cfg.u);

	nix_lf_cfg.u = 0;
	nix_lf_cfg.s.ena = 1;
	nix_lf_cfg.s.pf_func = (((pf & 0x3f) << 10) | 0x0);
	nix_lf_cfg.s.slot = 0;
	CSR_WRITE(CAVM_NIXX_PRIV_LFX_CFG(0, lf_id), nix_lf_cfg.u);
}

/* Disable NIX for PF */
static void pf_disable_nix(int nix, int pf, int lf_id)
{
	union cavm_rvu_priv_pfx_nixx_cfg pf_nix_cfg;
	union cavm_nixx_priv_lfx_cfg nix_lf_cfg;

	pf_nix_cfg.u = 0;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_NIXX_CFG(pf, nix), pf_nix_cfg.u);

	nix_lf_cfg.u = 0;
	CSR_WRITE(CAVM_NIXX_PRIV_LFX_CFG(nix, lf_id), nix_lf_cfg.u);
}

/* Reset all the resources before enabling PF */
static void reset_rvu_pf(int pf)
{
	union cavm_rvu_priv_pfx_cptx_cfg cpt_cfg;
	union cavm_rvu_priv_pfx_int_cfg int_cfg;
	union cavm_rvu_priv_pfx_msix_cfg msix_cfg;
	union cavm_rvu_priv_pfx_nixx_cfg nix_cfg;
	union cavm_rvu_priv_pfx_npa_cfg npa_cfg;
	union cavm_rvu_priv_pfx_sso_cfg sso_cfg;
	union cavm_rvu_priv_pfx_ssow_cfg ssow_cfg;
	union cavm_rvu_priv_pfx_tim_cfg tim_cfg;

	if (!plat_octeontx_bcfg->rvu_config.cpt_dis) {	/* CPT is present */
		cpt_cfg.u = 0;
		cpt_cfg.s.num_lfs = 0;
		CSR_WRITE(CAVM_RVU_PRIV_PFX_CPTX_CFG(pf, 0), cpt_cfg.u);
	}

	int_cfg.u = 0;
	int_cfg.s.msix_offset = 0;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_INT_CFG(pf), int_cfg.u);

	msix_cfg.u = 0;
	msix_cfg.s.pf_msixt_offset = 0;
	msix_cfg.s.pf_msixt_sizem1 = 0;
	msix_cfg.s.vf_msixt_offset = 0;
	msix_cfg.s.vf_msixt_sizem1 = 0;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_MSIX_CFG(pf), msix_cfg.u);

	nix_cfg.u = 0;
	nix_cfg.s.has_lf = 0;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_NIXX_CFG(pf, 0), nix_cfg.u);

	npa_cfg.u = 0;
	npa_cfg.s.has_lf = 0;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_NPA_CFG(pf), npa_cfg.u);

	sso_cfg.u = 0;
	sso_cfg.s.num_lfs = 0;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_SSO_CFG(pf), sso_cfg.u);

	ssow_cfg.u = 0;
	ssow_cfg.s.num_lfs = 0;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_SSOW_CFG(pf), ssow_cfg.u);

	tim_cfg.u = 0;
	tim_cfg.s.num_lfs = 0;
	CSR_WRITE(CAVM_RVU_PRIV_PFX_TIM_CFG(pf), tim_cfg.u);
}

/* Returns max LFs supported by NPA */
static int octeontx2_get_max_npa_lfs(void)
{
	cavm_npa_af_const_t npa_af_const;

	npa_af_const.u = CSR_READ(CAVM_NPA_AF_CONST);

	return npa_af_const.s.lfs;
}

/* Returns max LFs supported by NIX */
static int octeontx2_get_max_nix_lfs(int nix)
{
	cavm_nixx_af_const2_t nixx_af_const2;

	nixx_af_const2.u = CSR_READ(CAVM_NIXX_AF_CONST2(nix));

	return nixx_af_const2.s.lfs;
}

/* This is workaround for errata RVU-35948 */
static void rvu_errata_35948(void)
{
	int pf, mbox, vf;
	uint64_t int_reg;

	/* Write zeros to RVU_AF_AFPF()_MBOX() */
	for (pf = 0; pf < octeontx_get_max_rvu_pfs(); pf++) {
		for (mbox = 0; mbox < RVU_MBOX_NUM; mbox++)
			CSR_WRITE(CAVM_RVU_AF_AFPFX_MBOXX(pf, mbox), 0);
	}
	/* Write zeros to RVU_PF_VF()_PFVF_MBOX() */
	for (vf = 0; vf < octeontx_get_max_rvu_vfs(); vf++) {
		for (mbox = 0; mbox < RVU_MBOX_NUM; mbox++)
			CSR_WRITE(CAVM_RVU_PF_VFX_PFVF_MBOXX(vf, mbox), 0);
	}

	/* Clear RVU_PF_INT[MBOX] */
	for (mbox = 0; mbox < RVU_MBOX_NUM; mbox++) {
		int_reg = CSR_READ(CAVM_RVU_PF_INT);
		int_reg |= RVU_PF_INT_SET_MBOX;
		CSR_WRITE(CAVM_RVU_PF_INT, int_reg);
	}
	/* Clear RVU_VF_INT[MBOX] */
	for (mbox = 0; mbox < RVU_MBOX_NUM; mbox++) {
		int_reg = CSR_READ(CAVM_RVU_VF_INT);
		int_reg |= RVU_VF_INT_SET_MBOX;
		CSR_WRITE(CAVM_RVU_VF_INT, int_reg);
	}
}

/* Exported functions */
int octeontx2_clear_lf_to_pf_mapping(void)
{
	int pf, lf;

	for (pf = 0; pf < octeontx_get_max_rvu_pfs(); pf++) {
		for (lf = 0; lf < octeontx2_get_max_nix_lfs(0); lf++) {
			pf_disable_nix(0, pf, lf);
		}

		for (lf = 0; lf < octeontx2_get_max_npa_lfs(); lf++) {
			pf_disable_npa(pf, lf);
		}
	}

	return 0;
}

void octeontx_rvu_init(void)
{
	int pf, rc;
	int npalf_id = 0, nixlf_id = 0;

	/* This is workaround for errata RVU-35948 */
	if (IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 1) ||
	    IS_OCTEONTX_PN(read_midr(), F95PARTNUM) ||
	    IS_OCTEONTX_PN(read_midr(), F95MMPARTNUM))
		rvu_errata_35948();

	/* sanity check - validate platform definition */
	if (octeontx_get_max_rvu_pfs() != MAX_RVU_PFS)
		ERROR("MAX_RVU_PFS %d does not match platform const %d\n",
		      MAX_RVU_PFS, octeontx_get_max_rvu_pfs());

	rc = octeontx_init_rvu_from_fdt();
	if (rc < 0)
		return;

	dump_rvu_devs();

	for (pf = 0 ; pf < octeontx_get_max_rvu_pfs(); pf++) {
		if (rvu_dev[pf].enable) {
			reset_rvu_pf(pf);
			enable_rvu_pf(pf);
			if (rvu_dev[pf].pf_res_ena) {
				pf_enable_nix(pf, nixlf_id);
				nixlf_id++;
				pf_enable_npa(pf, npalf_id);
				npalf_id++;
			}
		}
		else 	/* Disable unused PFs */
			disable_rvu_pf(pf);
	}

	rc = msix_enable();
	if (rc) {
		for (pf = rc; pf < octeontx_get_max_rvu_pfs(); pf++) {
			disable_rvu_pf(pf);
			reset_rvu_pf(pf);
		}
	}

	mailbox_enable();
	config_rvu_pci();
}
