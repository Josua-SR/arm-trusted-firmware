/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __PLAT_OTX2_CONFIGURATION_H__
#define __PLAT_OTX2_CONFIGURATION_H__

#define CN93XX_PKG   1
#define CN95XXE_PKG  3

/* Used to initialize individual device IOBN security settings */
struct otx2_stream_security_setting {
	uint32_t streamid;    /* id of instance 0, see CAVM_PCC_DEV_CON_E_xxx */
	int      instances;   /* # of stream instances */
	/* following are applied to BOTH BUSX_STREAMS & DEVX/RSLX_STREAMS */
	int      strm_nsec;
	int      phys_nsec;
};

int plat_octeontx_get_iobn_count(void);
int plat_octeontx_get_cpt_count(void);
int plat_octeontx_get_cgx_count(void);
int plat_octeontx_get_gser_count(void);
int plat_octeontx_get_gserp_count(void);
int plat_octeontx_get_gserr_count(void);
int plat_octeontx_get_mcc_count(void);
int plat_octeontx_get_pem_count(void);
int plat_get_altpkg(void);
int plat_get_ooo_status(void);
unsigned int plat_configure_rid(void);
unsigned int plat_configure_cpt_rid(void);
struct otx2_stream_security_setting *plat_get_otx2_stream_security(int *count);

#endif /* __PLAT_OTX2_CONFIGURATION_H__ */
