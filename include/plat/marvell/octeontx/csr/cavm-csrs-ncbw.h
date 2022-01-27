#ifndef __CAVM_CSRS_NCBW_H__
#define __CAVM_CSRS_NCBW_H__
/* This file is auto-generated. Do not edit */

/***********************license start***********************************
* Copyright (C) 2018-2022 Marvell
* SPDX-License-Identifier: BSD-3-Clause
* https://spdx.org/licenses
***********************license end**************************************/


/**
 * @file
 *
 * Configuration and status register (CSR) address and type definitions for
 * OcteonTX NCBW.
 *
 * This file is auto generated. Do not edit.
 *
 */

/**
 * Register (RSL) ncbw_cfg
 *
 * NCBW Global Configuration Register
 */
union cavm_ncbw_cfg
{
    uint64_t u;
    struct cavm_ncbw_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_10_63        : 54;
        uint64_t ncbo_rsp_dbg_sel      : 2;  /**< [  9:  8](R/W/H) dbg3 mux select for device[2:0] read and write response. */
        uint64_t mio_timestp_tick      : 4;  /**< [  7:  4](R/W/H) MIO timestamp tick clock. This field specifies the number of ticks in
                                                                 system clock to sample the timestamp value and pass the timestamp value
                                                                 into the BPHY clock domain. The default is to sample once every four system clocks.
                                                                 Setting this field to 1 will sample the timestamp every system clock until
                                                                 an internal 8-entry FIFO is full.
                                                                 Setting this field to 0 will sample the timestamp every 16th system clock. */
        uint64_t reserved_2_3          : 2;
        uint64_t rst_flush             : 1;  /**< [  1:  1](R/W/H) Set one to flush incoming requests from all BPHY devices and to put NCBO requests
                                                                 in bypass mode. For BPHY reset only. */
        uint64_t wgt_clken             : 1;  /**< [  0:  0](R/W) Force the conditional clocking for NCBW itself. For diagnostic use only. */
#else /* Word 0 - Little Endian */
        uint64_t wgt_clken             : 1;  /**< [  0:  0](R/W) Force the conditional clocking for NCBW itself. For diagnostic use only. */
        uint64_t rst_flush             : 1;  /**< [  1:  1](R/W/H) Set one to flush incoming requests from all BPHY devices and to put NCBO requests
                                                                 in bypass mode. For BPHY reset only. */
        uint64_t reserved_2_3          : 2;
        uint64_t mio_timestp_tick      : 4;  /**< [  7:  4](R/W/H) MIO timestamp tick clock. This field specifies the number of ticks in
                                                                 system clock to sample the timestamp value and pass the timestamp value
                                                                 into the BPHY clock domain. The default is to sample once every four system clocks.
                                                                 Setting this field to 1 will sample the timestamp every system clock until
                                                                 an internal 8-entry FIFO is full.
                                                                 Setting this field to 0 will sample the timestamp every 16th system clock. */
        uint64_t ncbo_rsp_dbg_sel      : 2;  /**< [  9:  8](R/W/H) dbg3 mux select for device[2:0] read and write response. */
        uint64_t reserved_10_63        : 54;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ncbw_cfg_s cn; */
};
typedef union cavm_ncbw_cfg cavm_ncbw_cfg_t;

#define CAVM_NCBW_CFG CAVM_NCBW_CFG_FUNC()
static inline uint64_t CAVM_NCBW_CFG_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_NCBW_CFG_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_CNF95XX))
        return 0x87e040000000ll;
    __cavm_csr_fatal("NCBW_CFG", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_NCBW_CFG cavm_ncbw_cfg_t
#define bustype_CAVM_NCBW_CFG CSR_TYPE_RSL
#define basename_CAVM_NCBW_CFG "NCBW_CFG"
#define busnum_CAVM_NCBW_CFG 0
#define arguments_CAVM_NCBW_CFG -1,-1,-1,-1

/**
 * Register (RSL) ncbw_status
 *
 * NCBW Global Status Register
 */
union cavm_ncbw_status
{
    uint64_t u;
    struct cavm_ncbw_status_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_9_63         : 55;
        uint64_t bphy_freq_changing    : 1;  /**< [  8:  8](RO/H) BPHY-PLL is changing frequency when set to one. */
        uint64_t reserved_5_7          : 3;
        uint64_t ncbw_flush_done       : 1;  /**< [  4:  4](RO/H) NCBW reset flush done. It is set by hardware when SSO, NPA, NCBI, and NCBO reset
                                                                 flush are done. */
        uint64_t sso_flush_done        : 1;  /**< [  3:  3](RO/H) SSO interface flush done when set to one. For BPHY reset only. */
        uint64_t npa_flush_done        : 1;  /**< [  2:  2](RO/H) NPA interface flush done when set to one. For BPHY reset only. */
        uint64_t ncbo_flush_done       : 1;  /**< [  1:  1](RO/H) NCBO interface flush done when set to one. For BPHY reset only. */
        uint64_t ncbi_flush_done       : 1;  /**< [  0:  0](RO/H) NCBI interface flush done when set to one. For BPHY reset only. */
#else /* Word 0 - Little Endian */
        uint64_t ncbi_flush_done       : 1;  /**< [  0:  0](RO/H) NCBI interface flush done when set to one. For BPHY reset only. */
        uint64_t ncbo_flush_done       : 1;  /**< [  1:  1](RO/H) NCBO interface flush done when set to one. For BPHY reset only. */
        uint64_t npa_flush_done        : 1;  /**< [  2:  2](RO/H) NPA interface flush done when set to one. For BPHY reset only. */
        uint64_t sso_flush_done        : 1;  /**< [  3:  3](RO/H) SSO interface flush done when set to one. For BPHY reset only. */
        uint64_t ncbw_flush_done       : 1;  /**< [  4:  4](RO/H) NCBW reset flush done. It is set by hardware when SSO, NPA, NCBI, and NCBO reset
                                                                 flush are done. */
        uint64_t reserved_5_7          : 3;
        uint64_t bphy_freq_changing    : 1;  /**< [  8:  8](RO/H) BPHY-PLL is changing frequency when set to one. */
        uint64_t reserved_9_63         : 55;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ncbw_status_s cn; */
};
typedef union cavm_ncbw_status cavm_ncbw_status_t;

#define CAVM_NCBW_STATUS CAVM_NCBW_STATUS_FUNC()
static inline uint64_t CAVM_NCBW_STATUS_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_NCBW_STATUS_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_CNF95XX))
        return 0x87e040000008ll;
    __cavm_csr_fatal("NCBW_STATUS", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_NCBW_STATUS cavm_ncbw_status_t
#define bustype_CAVM_NCBW_STATUS CSR_TYPE_RSL
#define basename_CAVM_NCBW_STATUS "NCBW_STATUS"
#define busnum_CAVM_NCBW_STATUS 0
#define arguments_CAVM_NCBW_STATUS -1,-1,-1,-1

#endif /* __CAVM_CSRS_NCBW_H__ */
