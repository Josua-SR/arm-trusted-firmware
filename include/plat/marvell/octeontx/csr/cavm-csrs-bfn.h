#ifndef __CAVM_CSRS_BFN_H__
#define __CAVM_CSRS_BFN_H__
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
 * OcteonTX BFN.
 *
 * This file is auto generated. Do not edit.
 *
 */

/**
 * Register (RSL) bfn_bclk_capture_cfg
 *
 * BFN BCLK-Domain Capture Configuration Register
 * This register controls the BCLK-domain snapshot capture
 * functionality of the BFN block.
 */
union cavm_bfn_bclk_capture_cfg
{
    uint64_t u;
    struct cavm_bfn_bclk_capture_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_11_63        : 53;
        uint64_t capt_trig             : 3;  /**< [ 10:  8](R/W) Selects the trigger for a timestamp capture.
                                                                 0x0 = Match as configured by BFN_MATCH_CFG.
                                                                 0x1 = Start of frame.
                                                                 0x2 = Start of sub-frame.
                                                                 0x3 = Software (capture occurs immediately).
                                                                 0x4 = RFP
                                                                 0x5-0x7 = Reserved. */
        uint64_t reserved_1_7          : 7;
        uint64_t capt_en               : 1;  /**< [  0:  0](R/W/H) Enables simultaneous capture of the various system
                                                                 timestamps (PSM, IEEE 1914, and PTP).
                                                                 This bit resets to 0 after the capture occurs. */
#else /* Word 0 - Little Endian */
        uint64_t capt_en               : 1;  /**< [  0:  0](R/W/H) Enables simultaneous capture of the various system
                                                                 timestamps (PSM, IEEE 1914, and PTP).
                                                                 This bit resets to 0 after the capture occurs. */
        uint64_t reserved_1_7          : 7;
        uint64_t capt_trig             : 3;  /**< [ 10:  8](R/W) Selects the trigger for a timestamp capture.
                                                                 0x0 = Match as configured by BFN_MATCH_CFG.
                                                                 0x1 = Start of frame.
                                                                 0x2 = Start of sub-frame.
                                                                 0x3 = Software (capture occurs immediately).
                                                                 0x4 = RFP
                                                                 0x5-0x7 = Reserved. */
        uint64_t reserved_11_63        : 53;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_bclk_capture_cfg_s cn; */
};
typedef union cavm_bfn_bclk_capture_cfg cavm_bfn_bclk_capture_cfg_t;

#define CAVM_BFN_BCLK_CAPTURE_CFG CAVM_BFN_BCLK_CAPTURE_CFG_FUNC()
static inline uint64_t CAVM_BFN_BCLK_CAPTURE_CFG_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_BCLK_CAPTURE_CFG_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047022000ll;
    __cavm_csr_fatal("BFN_BCLK_CAPTURE_CFG", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_BCLK_CAPTURE_CFG cavm_bfn_bclk_capture_cfg_t
#define bustype_CAVM_BFN_BCLK_CAPTURE_CFG CSR_TYPE_RSL
#define basename_CAVM_BFN_BCLK_CAPTURE_CFG "BFN_BCLK_CAPTURE_CFG"
#define busnum_CAVM_BFN_BCLK_CAPTURE_CFG 0
#define arguments_CAVM_BFN_BCLK_CAPTURE_CFG -1,-1,-1,-1

/**
 * Register (RSL) bfn_bclk_capture_ieee1914
 *
 * BFN BCLK-Domain Capture IEEE 1914 Register
 * This register contains the IEEE 1914 timestamp value sampled at the
 * most recent BCLK-domain capture event.
 */
union cavm_bfn_bclk_capture_ieee1914
{
    uint64_t u;
    struct cavm_bfn_bclk_capture_ieee1914_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_29_63        : 35;
        uint64_t nsec                  : 24; /**< [ 28:  5](RO/H) Captured value of the IEEE 1914 timestamp integer
                                                                 nanosecond field. */
        uint64_t fnsec                 : 5;  /**< [  4:  0](RO/H) Captured value of the IEEE 1914 timestamp fractional
                                                                 nanosecond field. */
#else /* Word 0 - Little Endian */
        uint64_t fnsec                 : 5;  /**< [  4:  0](RO/H) Captured value of the IEEE 1914 timestamp fractional
                                                                 nanosecond field. */
        uint64_t nsec                  : 24; /**< [ 28:  5](RO/H) Captured value of the IEEE 1914 timestamp integer
                                                                 nanosecond field. */
        uint64_t reserved_29_63        : 35;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_bclk_capture_ieee1914_s cn; */
};
typedef union cavm_bfn_bclk_capture_ieee1914 cavm_bfn_bclk_capture_ieee1914_t;

#define CAVM_BFN_BCLK_CAPTURE_IEEE1914 CAVM_BFN_BCLK_CAPTURE_IEEE1914_FUNC()
static inline uint64_t CAVM_BFN_BCLK_CAPTURE_IEEE1914_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_BCLK_CAPTURE_IEEE1914_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047022030ll;
    __cavm_csr_fatal("BFN_BCLK_CAPTURE_IEEE1914", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_BCLK_CAPTURE_IEEE1914 cavm_bfn_bclk_capture_ieee1914_t
#define bustype_CAVM_BFN_BCLK_CAPTURE_IEEE1914 CSR_TYPE_RSL
#define basename_CAVM_BFN_BCLK_CAPTURE_IEEE1914 "BFN_BCLK_CAPTURE_IEEE1914"
#define busnum_CAVM_BFN_BCLK_CAPTURE_IEEE1914 0
#define arguments_CAVM_BFN_BCLK_CAPTURE_IEEE1914 -1,-1,-1,-1

/**
 * Register (RSL) bfn_bclk_capture_psm
 *
 * BFN BCLK-Domain Capture PSM Register
 * This register contains the PSM frame, subframe and tick values sampled
 * at the most recent BCLK-domain capture event.
 */
union cavm_bfn_bclk_capture_psm
{
    uint64_t u;
    struct cavm_bfn_bclk_capture_psm_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t frame                 : 12; /**< [ 31: 20](RO/H) Captured value of the PSM frame counter. */
        uint64_t sf                    : 4;  /**< [ 19: 16](RO/H) Captured value of the PSM subframe counter. */
        uint64_t tick                  : 16; /**< [ 15:  0](RO/H) Captured value of the PSM tick counter. */
#else /* Word 0 - Little Endian */
        uint64_t tick                  : 16; /**< [ 15:  0](RO/H) Captured value of the PSM tick counter. */
        uint64_t sf                    : 4;  /**< [ 19: 16](RO/H) Captured value of the PSM subframe counter. */
        uint64_t frame                 : 12; /**< [ 31: 20](RO/H) Captured value of the PSM frame counter. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_bclk_capture_psm_s cn; */
};
typedef union cavm_bfn_bclk_capture_psm cavm_bfn_bclk_capture_psm_t;

#define CAVM_BFN_BCLK_CAPTURE_PSM CAVM_BFN_BCLK_CAPTURE_PSM_FUNC()
static inline uint64_t CAVM_BFN_BCLK_CAPTURE_PSM_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_BCLK_CAPTURE_PSM_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047022010ll;
    __cavm_csr_fatal("BFN_BCLK_CAPTURE_PSM", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_BCLK_CAPTURE_PSM cavm_bfn_bclk_capture_psm_t
#define bustype_CAVM_BFN_BCLK_CAPTURE_PSM CSR_TYPE_RSL
#define basename_CAVM_BFN_BCLK_CAPTURE_PSM "BFN_BCLK_CAPTURE_PSM"
#define busnum_CAVM_BFN_BCLK_CAPTURE_PSM 0
#define arguments_CAVM_BFN_BCLK_CAPTURE_PSM -1,-1,-1,-1

/**
 * Register (RSL) bfn_bclk_capture_ptp
 *
 * BFN BCLK-Domain Capture PTP Register
 * This register contains the PTP timestamp value sampled at the
 * most recent BCLK-domain capture event.
 */
union cavm_bfn_bclk_capture_ptp
{
    uint64_t u;
    struct cavm_bfn_bclk_capture_ptp_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ptp                   : 64; /**< [ 63:  0](RO/H) Captured value of the PTP timestamp. */
#else /* Word 0 - Little Endian */
        uint64_t ptp                   : 64; /**< [ 63:  0](RO/H) Captured value of the PTP timestamp. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_bclk_capture_ptp_s cn; */
};
typedef union cavm_bfn_bclk_capture_ptp cavm_bfn_bclk_capture_ptp_t;

#define CAVM_BFN_BCLK_CAPTURE_PTP CAVM_BFN_BCLK_CAPTURE_PTP_FUNC()
static inline uint64_t CAVM_BFN_BCLK_CAPTURE_PTP_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_BCLK_CAPTURE_PTP_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047022020ll;
    __cavm_csr_fatal("BFN_BCLK_CAPTURE_PTP", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_BCLK_CAPTURE_PTP cavm_bfn_bclk_capture_ptp_t
#define bustype_CAVM_BFN_BCLK_CAPTURE_PTP CSR_TYPE_RSL
#define basename_CAVM_BFN_BCLK_CAPTURE_PTP "BFN_BCLK_CAPTURE_PTP"
#define busnum_CAVM_BFN_BCLK_CAPTURE_PTP 0
#define arguments_CAVM_BFN_BCLK_CAPTURE_PTP -1,-1,-1,-1

/**
 * Register (RSL) bfn_bfn_err_count
 *
 * BFN BFN Error Count Register
 * This register contains a running count of BFN errors detected.
 */
union cavm_bfn_bfn_err_count
{
    uint64_t u;
    struct cavm_bfn_bfn_err_count_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t bfn_err               : 32; /**< [ 31:  0](R/W/H) Count of BFN errors detected. */
#else /* Word 0 - Little Endian */
        uint64_t bfn_err               : 32; /**< [ 31:  0](R/W/H) Count of BFN errors detected. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_bfn_err_count_s cn; */
};
typedef union cavm_bfn_bfn_err_count cavm_bfn_bfn_err_count_t;

#define CAVM_BFN_BFN_ERR_COUNT CAVM_BFN_BFN_ERR_COUNT_FUNC()
static inline uint64_t CAVM_BFN_BFN_ERR_COUNT_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_BFN_ERR_COUNT_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047037030ll;
    __cavm_csr_fatal("BFN_BFN_ERR_COUNT", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_BFN_ERR_COUNT cavm_bfn_bfn_err_count_t
#define bustype_CAVM_BFN_BFN_ERR_COUNT CSR_TYPE_RSL
#define basename_CAVM_BFN_BFN_ERR_COUNT "BFN_BFN_ERR_COUNT"
#define busnum_CAVM_BFN_BFN_ERR_COUNT 0
#define arguments_CAVM_BFN_BFN_ERR_COUNT -1,-1,-1,-1

/**
 * Register (RSL) bfn_capture_cfg
 *
 * BFN Capture Configuration Register
 * This register controls the snapshot capture functionality of the BFN block.
 */
union cavm_bfn_capture_cfg
{
    uint64_t u;
    struct cavm_bfn_capture_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_11_63        : 53;
        uint64_t capt_trig             : 3;  /**< [ 10:  8](R/W) Selects the trigger for a timestamp capture.
                                                                 0x0 = Match as configured by BFN_MATCH_CFG.
                                                                 0x1 = Start of frame.
                                                                 0x2 = Start of sub-frame.
                                                                 0x3 = Software (capture occurs immediately).
                                                                 0x4 = RFP
                                                                 0x5-0x7 = Reserved. */
        uint64_t reserved_1_7          : 7;
        uint64_t capt_en               : 1;  /**< [  0:  0](R/W/H) Enables simultaneous capture of the various system
                                                                 timestamps (PSM, IEEE 1914, and PTP).
                                                                 This bit resets to 0 after the capture occurs. */
#else /* Word 0 - Little Endian */
        uint64_t capt_en               : 1;  /**< [  0:  0](R/W/H) Enables simultaneous capture of the various system
                                                                 timestamps (PSM, IEEE 1914, and PTP).
                                                                 This bit resets to 0 after the capture occurs. */
        uint64_t reserved_1_7          : 7;
        uint64_t capt_trig             : 3;  /**< [ 10:  8](R/W) Selects the trigger for a timestamp capture.
                                                                 0x0 = Match as configured by BFN_MATCH_CFG.
                                                                 0x1 = Start of frame.
                                                                 0x2 = Start of sub-frame.
                                                                 0x3 = Software (capture occurs immediately).
                                                                 0x4 = RFP
                                                                 0x5-0x7 = Reserved. */
        uint64_t reserved_11_63        : 53;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_capture_cfg_s cn; */
};
typedef union cavm_bfn_capture_cfg cavm_bfn_capture_cfg_t;

#define CAVM_BFN_CAPTURE_CFG CAVM_BFN_CAPTURE_CFG_FUNC()
static inline uint64_t CAVM_BFN_CAPTURE_CFG_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_CAPTURE_CFG_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047036000ll;
    __cavm_csr_fatal("BFN_CAPTURE_CFG", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_CAPTURE_CFG cavm_bfn_capture_cfg_t
#define bustype_CAVM_BFN_CAPTURE_CFG CSR_TYPE_RSL
#define basename_CAVM_BFN_CAPTURE_CFG "BFN_CAPTURE_CFG"
#define busnum_CAVM_BFN_CAPTURE_CFG 0
#define arguments_CAVM_BFN_CAPTURE_CFG -1,-1,-1,-1

/**
 * Register (RSL) bfn_capture_ieee1914
 *
 * BFN Capture IEEE 1914 Register
 * This register contains the IEEE 1914 timestamp value sampled at the
 * most recent capture event.
 */
union cavm_bfn_capture_ieee1914
{
    uint64_t u;
    struct cavm_bfn_capture_ieee1914_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_29_63        : 35;
        uint64_t nsec                  : 24; /**< [ 28:  5](RO/H) Captured value of the IEEE 1914 timestamp integer
                                                                 nanosecond field. */
        uint64_t fnsec                 : 5;  /**< [  4:  0](RO/H) Captured value of the IEEE 1914 timestamp fractional
                                                                 nanosecond field. */
#else /* Word 0 - Little Endian */
        uint64_t fnsec                 : 5;  /**< [  4:  0](RO/H) Captured value of the IEEE 1914 timestamp fractional
                                                                 nanosecond field. */
        uint64_t nsec                  : 24; /**< [ 28:  5](RO/H) Captured value of the IEEE 1914 timestamp integer
                                                                 nanosecond field. */
        uint64_t reserved_29_63        : 35;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_capture_ieee1914_s cn; */
};
typedef union cavm_bfn_capture_ieee1914 cavm_bfn_capture_ieee1914_t;

#define CAVM_BFN_CAPTURE_IEEE1914 CAVM_BFN_CAPTURE_IEEE1914_FUNC()
static inline uint64_t CAVM_BFN_CAPTURE_IEEE1914_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_CAPTURE_IEEE1914_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047036300ll;
    __cavm_csr_fatal("BFN_CAPTURE_IEEE1914", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_CAPTURE_IEEE1914 cavm_bfn_capture_ieee1914_t
#define bustype_CAVM_BFN_CAPTURE_IEEE1914 CSR_TYPE_RSL
#define basename_CAVM_BFN_CAPTURE_IEEE1914 "BFN_CAPTURE_IEEE1914"
#define busnum_CAVM_BFN_CAPTURE_IEEE1914 0
#define arguments_CAVM_BFN_CAPTURE_IEEE1914 -1,-1,-1,-1

/**
 * Register (RSL) bfn_capture_psm
 *
 * BFN Capture PSM Register
 * This register contains the PSM frame, subframe and tick values sampled
 * at the most recent capture event.
 */
union cavm_bfn_capture_psm
{
    uint64_t u;
    struct cavm_bfn_capture_psm_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t frame                 : 12; /**< [ 31: 20](RO/H) Captured value of the PSM frame counter. */
        uint64_t sf                    : 4;  /**< [ 19: 16](RO/H) Captured value of the PSM subframe counter. */
        uint64_t tick                  : 16; /**< [ 15:  0](RO/H) Captured value of the PSM tick counter. */
#else /* Word 0 - Little Endian */
        uint64_t tick                  : 16; /**< [ 15:  0](RO/H) Captured value of the PSM tick counter. */
        uint64_t sf                    : 4;  /**< [ 19: 16](RO/H) Captured value of the PSM subframe counter. */
        uint64_t frame                 : 12; /**< [ 31: 20](RO/H) Captured value of the PSM frame counter. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_capture_psm_s cn; */
};
typedef union cavm_bfn_capture_psm cavm_bfn_capture_psm_t;

#define CAVM_BFN_CAPTURE_PSM CAVM_BFN_CAPTURE_PSM_FUNC()
static inline uint64_t CAVM_BFN_CAPTURE_PSM_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_CAPTURE_PSM_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047036100ll;
    __cavm_csr_fatal("BFN_CAPTURE_PSM", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_CAPTURE_PSM cavm_bfn_capture_psm_t
#define bustype_CAVM_BFN_CAPTURE_PSM CSR_TYPE_RSL
#define basename_CAVM_BFN_CAPTURE_PSM "BFN_CAPTURE_PSM"
#define busnum_CAVM_BFN_CAPTURE_PSM 0
#define arguments_CAVM_BFN_CAPTURE_PSM -1,-1,-1,-1

/**
 * Register (RSL) bfn_capture_psm_cnt
 *
 * BFN Capture PSM Register
 * This register contains the PSM frame and subframe cycle counter
 * values sampled at the most recent capture event.
 */
union cavm_bfn_capture_psm_cnt
{
    uint64_t u;
    struct cavm_bfn_capture_psm_cnt_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_56_63        : 8;
        uint64_t frame_cnt             : 24; /**< [ 55: 32](RO/H) Captured value of the PSM frame cycle counter. */
        uint64_t reserved_20_31        : 12;
        uint64_t sf_cnt                : 20; /**< [ 19:  0](RO/H) Captured value of the PSM subframe cycle counter. */
#else /* Word 0 - Little Endian */
        uint64_t sf_cnt                : 20; /**< [ 19:  0](RO/H) Captured value of the PSM subframe cycle counter. */
        uint64_t reserved_20_31        : 12;
        uint64_t frame_cnt             : 24; /**< [ 55: 32](RO/H) Captured value of the PSM frame cycle counter. */
        uint64_t reserved_56_63        : 8;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_capture_psm_cnt_s cn; */
};
typedef union cavm_bfn_capture_psm_cnt cavm_bfn_capture_psm_cnt_t;

#define CAVM_BFN_CAPTURE_PSM_CNT CAVM_BFN_CAPTURE_PSM_CNT_FUNC()
static inline uint64_t CAVM_BFN_CAPTURE_PSM_CNT_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_CAPTURE_PSM_CNT_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047036110ll;
    __cavm_csr_fatal("BFN_CAPTURE_PSM_CNT", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_CAPTURE_PSM_CNT cavm_bfn_capture_psm_cnt_t
#define bustype_CAVM_BFN_CAPTURE_PSM_CNT CSR_TYPE_RSL
#define basename_CAVM_BFN_CAPTURE_PSM_CNT "BFN_CAPTURE_PSM_CNT"
#define busnum_CAVM_BFN_CAPTURE_PSM_CNT 0
#define arguments_CAVM_BFN_CAPTURE_PSM_CNT -1,-1,-1,-1

/**
 * Register (RSL) bfn_capture_ptp
 *
 * BFN Capture PTP Register
 * This register contains the PTP timestamp value sampled at the
 * most recent capture event.
 */
union cavm_bfn_capture_ptp
{
    uint64_t u;
    struct cavm_bfn_capture_ptp_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ptp                   : 64; /**< [ 63:  0](RO/H) Captured value of the PTP timestamp. */
#else /* Word 0 - Little Endian */
        uint64_t ptp                   : 64; /**< [ 63:  0](RO/H) Captured value of the PTP timestamp. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_capture_ptp_s cn; */
};
typedef union cavm_bfn_capture_ptp cavm_bfn_capture_ptp_t;

#define CAVM_BFN_CAPTURE_PTP CAVM_BFN_CAPTURE_PTP_FUNC()
static inline uint64_t CAVM_BFN_CAPTURE_PTP_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_CAPTURE_PTP_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047036200ll;
    __cavm_csr_fatal("BFN_CAPTURE_PTP", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_CAPTURE_PTP cavm_bfn_capture_ptp_t
#define bustype_CAVM_BFN_CAPTURE_PTP CSR_TYPE_RSL
#define basename_CAVM_BFN_CAPTURE_PTP "BFN_CAPTURE_PTP"
#define busnum_CAVM_BFN_CAPTURE_PTP 0
#define arguments_CAVM_BFN_CAPTURE_PTP -1,-1,-1,-1

/**
 * Register (RSL) bfn_cfg
 *
 * BFN Configuration Register
 * This register controls the basic functionality of the BFN block.
 */
union cavm_bfn_cfg
{
    uint64_t u;
    struct cavm_bfn_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t bfn_enable            : 1;  /**< [ 63: 63](R/W) Enables the BFN block. */
        uint64_t reserved_28_62        : 35;
        uint64_t safe_late_rfp         : 1;  /**< [ 27: 27](R/W) When set to 1, this will suppress subframe handling when a late-RFP
                                                                 condition occurs.  Without this, a late-RFP may result in an extra
                                                                 SOSF and incorrect subframe and tick values. */
        uint64_t internal_rfp_mode     : 1;  /**< [ 26: 26](R/W) Enables the timer internal RFP mode.  When set, the BFN will
                                                                 ignore the RFP signal and instead generate a SOF
                                                                 (start of frame) when the frame counter reaches the value
                                                                 specified by [FRAME_MAX]. */
        uint64_t rfp_pol               : 1;  /**< [ 25: 25](R/W) Polarity of the radio frame pulse:
                                                                 0 = RFP detected on a falling edge.
                                                                 1 = RFP detected on a rising edge. */
        uint64_t rfp_type              : 1;  /**< [ 24: 24](R/W) Frame handling upon detection of RFP.
                                                                 0 = BFN burst carries the frame number for the subsequent frame.
                                                                 1 = BFN burst carries the frame number for the current frame.
                                                                 The operating frame number will be incremented by 1, and if
                                                                 this value does not match the BFN burst, an error will be
                                                                 raised. */
        uint64_t reserved_16_23        : 8;
        uint64_t bfn_clk_per           : 16; /**< [ 15:  0](R/W) This specifies the clock period of the BFN clock, in BTS_CLK
                                                                 cycles. */
#else /* Word 0 - Little Endian */
        uint64_t bfn_clk_per           : 16; /**< [ 15:  0](R/W) This specifies the clock period of the BFN clock, in BTS_CLK
                                                                 cycles. */
        uint64_t reserved_16_23        : 8;
        uint64_t rfp_type              : 1;  /**< [ 24: 24](R/W) Frame handling upon detection of RFP.
                                                                 0 = BFN burst carries the frame number for the subsequent frame.
                                                                 1 = BFN burst carries the frame number for the current frame.
                                                                 The operating frame number will be incremented by 1, and if
                                                                 this value does not match the BFN burst, an error will be
                                                                 raised. */
        uint64_t rfp_pol               : 1;  /**< [ 25: 25](R/W) Polarity of the radio frame pulse:
                                                                 0 = RFP detected on a falling edge.
                                                                 1 = RFP detected on a rising edge. */
        uint64_t internal_rfp_mode     : 1;  /**< [ 26: 26](R/W) Enables the timer internal RFP mode.  When set, the BFN will
                                                                 ignore the RFP signal and instead generate a SOF
                                                                 (start of frame) when the frame counter reaches the value
                                                                 specified by [FRAME_MAX]. */
        uint64_t safe_late_rfp         : 1;  /**< [ 27: 27](R/W) When set to 1, this will suppress subframe handling when a late-RFP
                                                                 condition occurs.  Without this, a late-RFP may result in an extra
                                                                 SOSF and incorrect subframe and tick values. */
        uint64_t reserved_28_62        : 35;
        uint64_t bfn_enable            : 1;  /**< [ 63: 63](R/W) Enables the BFN block. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_cfg_s cn; */
};
typedef union cavm_bfn_cfg cavm_bfn_cfg_t;

#define CAVM_BFN_CFG CAVM_BFN_CFG_FUNC()
static inline uint64_t CAVM_BFN_CFG_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_CFG_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047030000ll;
    __cavm_csr_fatal("BFN_CFG", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_CFG cavm_bfn_cfg_t
#define bustype_CAVM_BFN_CFG CSR_TYPE_RSL
#define basename_CAVM_BFN_CFG "BFN_CFG"
#define busnum_CAVM_BFN_CFG 0
#define arguments_CAVM_BFN_CFG -1,-1,-1,-1

/**
 * Register (RSL) bfn_ctrl
 *
 * BFN Control Register
 * This register is used to configure the clock domain crossing to
 * the BTS_CLK domain.  It should be properly initialized before
 * using the BTS_CLK domain logic of the BFN block.
 */
union cavm_bfn_ctrl
{
    uint64_t u;
    struct cavm_bfn_ctrl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_63           : 1;
        uint64_t bts_clk_rst_n         : 1;  /**< [ 62: 62](R/W) Reset for the BTS_CLK domain logic of the BFN. */
        uint64_t bts_clk_rst_done      : 1;  /**< [ 61: 61](RO/H) Indicates the BTS_CLK reset has completed. */
        uint64_t bts_clk_clk_check     : 1;  /**< [ 60: 60](R/W/H) Used to check the presence of the BTS_CLK Clock.  See
                                                                 [BTS_CLK_CLK_ACTIVE] for more details. */
        uint64_t bts_clk_clk_active    : 1;  /**< [ 59: 59](RO/H) Indicates the activity of the BTS_CLK clock.  When
                                                                 [BTS_CLK_CLK_CHECK] is written to 1, the previous clock status
                                                                 is cleared.  After a rising edge and falling edge of the BTS_CLK
                                                                 clock are detected, the [BTS_CLK_CLK_ACTIVE] bit will become 1,
                                                                 and the [BTS_CLK_CLK_CHECK] bit will return to 0. */
        uint64_t reserved_57_58        : 2;
        uint64_t bts_clk_domain_dis    : 1;  /**< [ 56: 56](R/W) Isolates the BTS_CLK domain logic of the BFN until it is
                                                                 properly configured.
                                                                 1 = BTS_CLK domain is disabled and most BFN registers and
                                                                 functionality is unavailable.
                                                                 0 = BTS_CLK domain is enabled. */
        uint64_t reserved_0_55         : 56;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_55         : 56;
        uint64_t bts_clk_domain_dis    : 1;  /**< [ 56: 56](R/W) Isolates the BTS_CLK domain logic of the BFN until it is
                                                                 properly configured.
                                                                 1 = BTS_CLK domain is disabled and most BFN registers and
                                                                 functionality is unavailable.
                                                                 0 = BTS_CLK domain is enabled. */
        uint64_t reserved_57_58        : 2;
        uint64_t bts_clk_clk_active    : 1;  /**< [ 59: 59](RO/H) Indicates the activity of the BTS_CLK clock.  When
                                                                 [BTS_CLK_CLK_CHECK] is written to 1, the previous clock status
                                                                 is cleared.  After a rising edge and falling edge of the BTS_CLK
                                                                 clock are detected, the [BTS_CLK_CLK_ACTIVE] bit will become 1,
                                                                 and the [BTS_CLK_CLK_CHECK] bit will return to 0. */
        uint64_t bts_clk_clk_check     : 1;  /**< [ 60: 60](R/W/H) Used to check the presence of the BTS_CLK Clock.  See
                                                                 [BTS_CLK_CLK_ACTIVE] for more details. */
        uint64_t bts_clk_rst_done      : 1;  /**< [ 61: 61](RO/H) Indicates the BTS_CLK reset has completed. */
        uint64_t bts_clk_rst_n         : 1;  /**< [ 62: 62](R/W) Reset for the BTS_CLK domain logic of the BFN. */
        uint64_t reserved_63           : 1;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_ctrl_s cn; */
};
typedef union cavm_bfn_ctrl cavm_bfn_ctrl_t;

#define CAVM_BFN_CTRL CAVM_BFN_CTRL_FUNC()
static inline uint64_t CAVM_BFN_CTRL_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_CTRL_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047020000ll;
    __cavm_csr_fatal("BFN_CTRL", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_CTRL cavm_bfn_ctrl_t
#define bustype_CAVM_BFN_CTRL CSR_TYPE_RSL
#define basename_CAVM_BFN_CTRL "BFN_CTRL"
#define busnum_CAVM_BFN_CTRL 0
#define arguments_CAVM_BFN_CTRL -1,-1,-1,-1

/**
 * Register (RSL) bfn_ctrl2
 *
 * INTERNAL: BFN Control2 Register
 *
 * This register provides controls for additional BFN functionality.
 */
union cavm_bfn_ctrl2
{
    uint64_t u;
    struct cavm_bfn_ctrl2_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t rfoe_time_mode        : 1;  /**< [  1:  1](R/W) Set to 1 to drive SOF/SOSF to RFOE as single BCLK-cycle
                                                                 pulses. */
        uint64_t bclk_div_sel          : 1;  /**< [  0:  0](R/W) Set to 1 to select BCLK/4 for PCBW i/f. */
#else /* Word 0 - Little Endian */
        uint64_t bclk_div_sel          : 1;  /**< [  0:  0](R/W) Set to 1 to select BCLK/4 for PCBW i/f. */
        uint64_t rfoe_time_mode        : 1;  /**< [  1:  1](R/W) Set to 1 to drive SOF/SOSF to RFOE as single BCLK-cycle
                                                                 pulses. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_ctrl2_s cn; */
};
typedef union cavm_bfn_ctrl2 cavm_bfn_ctrl2_t;

#define CAVM_BFN_CTRL2 CAVM_BFN_CTRL2_FUNC()
static inline uint64_t CAVM_BFN_CTRL2_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_CTRL2_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e04702ff00ll;
    __cavm_csr_fatal("BFN_CTRL2", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_CTRL2 cavm_bfn_ctrl2_t
#define bustype_CAVM_BFN_CTRL2 CSR_TYPE_RSL
#define basename_CAVM_BFN_CTRL2 "BFN_CTRL2"
#define busnum_CAVM_BFN_CTRL2 0
#define arguments_CAVM_BFN_CTRL2 -1,-1,-1,-1

/**
 * Register (RSL) bfn_dbg
 *
 * INTERNAL: BFN DBG Register
 *
 * This register controls the debug (DTX) logic for the BFN's
 * bts_clk domain.
 */
union cavm_bfn_dbg
{
    uint64_t u;
    struct cavm_bfn_dbg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_17_63        : 47;
        uint64_t dbg_push_throttle     : 1;  /**< [ 16: 16](R/W) Internal:
                                                                 When set to 1, throttles DBG pushes so that they occur on
                                                                 every other bts_clk cycle.  This should be enabled if
                                                                 the BCLK is slower than TBD. */
        uint64_t dbg1_sel              : 8;  /**< [ 15:  8](R/W) Internal:
                                                                 0x00 : disabled
                                                                 0x10 : {frame_val[11:0],frame_cnt_val[23:0]};
                                                                 0x11 : {4'h0,sof,sosf,match_detect_1a,capt_event_1a,sync_event_1a,rfp_good_incr_
                                                                 1a,rfp_bad_incr_1a,bfn_err_incr_1a,subframe_val[3:0],sf_cnt_val[19:0]};
                                                                 0x12 : {sof,sosf,capt_event_1a,sync_event_1a,tick_val_fxp[31:0]};
                                                                 0x20 : {sof,sosf,capt_event_1a,sync_event_1a,rfp_good_incr_1a,rfp_bad_incr_1a,bf
                                                                 n_err_incr_1a,nsec_val[23:0],fnsec_val[4:0]};
                                                                 0x30 : ptp_val[35:0];
                                                                 0x31 : ptp_val[63:28];
                                                                 0x40 : {bfn_bit_ctr[3:0],bts_clk_div_ctr[15:0],rx_bfn_val[15:0]}; */
        uint64_t dbg0_sel              : 8;  /**< [  7:  0](R/W) Internal:
                                                                 See BCN_DBG[DBG1_SEL]. */
#else /* Word 0 - Little Endian */
        uint64_t dbg0_sel              : 8;  /**< [  7:  0](R/W) Internal:
                                                                 See BCN_DBG[DBG1_SEL]. */
        uint64_t dbg1_sel              : 8;  /**< [ 15:  8](R/W) Internal:
                                                                 0x00 : disabled
                                                                 0x10 : {frame_val[11:0],frame_cnt_val[23:0]};
                                                                 0x11 : {4'h0,sof,sosf,match_detect_1a,capt_event_1a,sync_event_1a,rfp_good_incr_
                                                                 1a,rfp_bad_incr_1a,bfn_err_incr_1a,subframe_val[3:0],sf_cnt_val[19:0]};
                                                                 0x12 : {sof,sosf,capt_event_1a,sync_event_1a,tick_val_fxp[31:0]};
                                                                 0x20 : {sof,sosf,capt_event_1a,sync_event_1a,rfp_good_incr_1a,rfp_bad_incr_1a,bf
                                                                 n_err_incr_1a,nsec_val[23:0],fnsec_val[4:0]};
                                                                 0x30 : ptp_val[35:0];
                                                                 0x31 : ptp_val[63:28];
                                                                 0x40 : {bfn_bit_ctr[3:0],bts_clk_div_ctr[15:0],rx_bfn_val[15:0]}; */
        uint64_t dbg_push_throttle     : 1;  /**< [ 16: 16](R/W) Internal:
                                                                 When set to 1, throttles DBG pushes so that they occur on
                                                                 every other bts_clk cycle.  This should be enabled if
                                                                 the BCLK is slower than TBD. */
        uint64_t reserved_17_63        : 47;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_dbg_s cn; */
};
typedef union cavm_bfn_dbg cavm_bfn_dbg_t;

#define CAVM_BFN_DBG CAVM_BFN_DBG_FUNC()
static inline uint64_t CAVM_BFN_DBG_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_DBG_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e04703ffe0ll;
    __cavm_csr_fatal("BFN_DBG", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_DBG cavm_bfn_dbg_t
#define bustype_CAVM_BFN_DBG CSR_TYPE_RSL
#define basename_CAVM_BFN_DBG "BFN_DBG"
#define busnum_CAVM_BFN_DBG 0
#define arguments_CAVM_BFN_DBG -1,-1,-1,-1

/**
 * Register (RSL) bfn_eco
 *
 * INTERNAL: BFN ECO Register
 */
union cavm_bfn_eco
{
    uint64_t u;
    struct cavm_bfn_eco_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t eco_rw                : 32; /**< [ 31:  0](R/W) Internal:
                                                                 Reserved for ECO usage. */
#else /* Word 0 - Little Endian */
        uint64_t eco_rw                : 32; /**< [ 31:  0](R/W) Internal:
                                                                 Reserved for ECO usage. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_eco_s cn; */
};
typedef union cavm_bfn_eco cavm_bfn_eco_t;

#define CAVM_BFN_ECO CAVM_BFN_ECO_FUNC()
static inline uint64_t CAVM_BFN_ECO_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_ECO_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e04703fff0ll;
    __cavm_csr_fatal("BFN_ECO", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_ECO cavm_bfn_eco_t
#define bustype_CAVM_BFN_ECO CSR_TYPE_RSL
#define basename_CAVM_BFN_ECO "BFN_ECO"
#define busnum_CAVM_BFN_ECO 0
#define arguments_CAVM_BFN_ECO -1,-1,-1,-1

/**
 * Register (RSL) bfn_err_test
 *
 * INTERNAL: BFN Error Test Register
 *
 * This register provides a way to set the BFN error bits for
 * connectivity testing.  Writing a 1 to a given error bit will
 * assert that error signal on the BFN output.  This mechanism
 * is only available in internal-timer mode.  These bits will
 * automatically self-clear.
 */
union cavm_bfn_err_test
{
    uint64_t u;
    struct cavm_bfn_err_test_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_4_63         : 60;
        uint64_t rfp_missing           : 1;  /**< [  3:  3](R/W/H) Assert the RFP_MISSING error bit. */
        uint64_t bfn_err               : 1;  /**< [  2:  2](R/W/H) Assert the BFN_ERR error bit. */
        uint64_t rfp_late              : 1;  /**< [  1:  1](R/W/H) Assert the RFP_LATE error bit. */
        uint64_t rfp_early             : 1;  /**< [  0:  0](R/W/H) Assert the RFP_EARLY error bit. */
#else /* Word 0 - Little Endian */
        uint64_t rfp_early             : 1;  /**< [  0:  0](R/W/H) Assert the RFP_EARLY error bit. */
        uint64_t rfp_late              : 1;  /**< [  1:  1](R/W/H) Assert the RFP_LATE error bit. */
        uint64_t bfn_err               : 1;  /**< [  2:  2](R/W/H) Assert the BFN_ERR error bit. */
        uint64_t rfp_missing           : 1;  /**< [  3:  3](R/W/H) Assert the RFP_MISSING error bit. */
        uint64_t reserved_4_63         : 60;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_err_test_s cn; */
};
typedef union cavm_bfn_err_test cavm_bfn_err_test_t;

#define CAVM_BFN_ERR_TEST CAVM_BFN_ERR_TEST_FUNC()
static inline uint64_t CAVM_BFN_ERR_TEST_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_ERR_TEST_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e04702ff10ll;
    __cavm_csr_fatal("BFN_ERR_TEST", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_ERR_TEST cavm_bfn_err_test_t
#define bustype_CAVM_BFN_ERR_TEST CSR_TYPE_RSL
#define basename_CAVM_BFN_ERR_TEST "BFN_ERR_TEST"
#define busnum_CAVM_BFN_ERR_TEST 0
#define arguments_CAVM_BFN_ERR_TEST -1,-1,-1,-1

/**
 * Register (RSL) bfn_ieee1914_cfg
 *
 * BFN IEEE 1914 Counter Configuration Register
 * This register configures the logic that generates the IEEE 1914
 * counters.
 */
union cavm_bfn_ieee1914_cfg
{
    uint64_t u;
    struct cavm_bfn_ieee1914_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_20_63        : 44;
        uint64_t ns_div                : 4;  /**< [ 19: 16](R/W) This configures the BTS_CLK divider for the IEEE1914 timestamp
                                                                 counter.  See [NS_INC] for more details. */
        uint64_t ns_inc                : 16; /**< [ 15:  0](R/W) This specifies the increment value for the IEEE 1914 timestamp
                                                                 counter.  After every [NS_DIV] BTS_CLK cycles, the IEEE 1914
                                                                 timestamp value is incremented by [NS_INC].  This value is
                                                                 an 8.8 fixed-point value. */
#else /* Word 0 - Little Endian */
        uint64_t ns_inc                : 16; /**< [ 15:  0](R/W) This specifies the increment value for the IEEE 1914 timestamp
                                                                 counter.  After every [NS_DIV] BTS_CLK cycles, the IEEE 1914
                                                                 timestamp value is incremented by [NS_INC].  This value is
                                                                 an 8.8 fixed-point value. */
        uint64_t ns_div                : 4;  /**< [ 19: 16](R/W) This configures the BTS_CLK divider for the IEEE1914 timestamp
                                                                 counter.  See [NS_INC] for more details. */
        uint64_t reserved_20_63        : 44;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_ieee1914_cfg_s cn; */
};
typedef union cavm_bfn_ieee1914_cfg cavm_bfn_ieee1914_cfg_t;

#define CAVM_BFN_IEEE1914_CFG CAVM_BFN_IEEE1914_CFG_FUNC()
static inline uint64_t CAVM_BFN_IEEE1914_CFG_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_IEEE1914_CFG_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047032000ll;
    __cavm_csr_fatal("BFN_IEEE1914_CFG", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_IEEE1914_CFG cavm_bfn_ieee1914_cfg_t
#define bustype_CAVM_BFN_IEEE1914_CFG CSR_TYPE_RSL
#define basename_CAVM_BFN_IEEE1914_CFG "BFN_IEEE1914_CFG"
#define busnum_CAVM_BFN_IEEE1914_CFG 0
#define arguments_CAVM_BFN_IEEE1914_CFG -1,-1,-1,-1

/**
 * Register (RSL) bfn_ieee1914_val
 *
 * BFN IEEE 1914 Counter Value Register
 * This register contains the current value of the IEEE 1914
 * counters.
 */
union cavm_bfn_ieee1914_val
{
    uint64_t u;
    struct cavm_bfn_ieee1914_val_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_29_63        : 35;
        uint64_t nsec                  : 24; /**< [ 28:  5](RO/H) Current value of the IEEE 1914 timestamp integer
                                                                 nanosecond field. */
        uint64_t fnsec                 : 5;  /**< [  4:  0](RO/H) Current value of the IEEE 1914 timestamp fractional
                                                                 nanosecond field. */
#else /* Word 0 - Little Endian */
        uint64_t fnsec                 : 5;  /**< [  4:  0](RO/H) Current value of the IEEE 1914 timestamp fractional
                                                                 nanosecond field. */
        uint64_t nsec                  : 24; /**< [ 28:  5](RO/H) Current value of the IEEE 1914 timestamp integer
                                                                 nanosecond field. */
        uint64_t reserved_29_63        : 35;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_ieee1914_val_s cn; */
};
typedef union cavm_bfn_ieee1914_val cavm_bfn_ieee1914_val_t;

#define CAVM_BFN_IEEE1914_VAL CAVM_BFN_IEEE1914_VAL_FUNC()
static inline uint64_t CAVM_BFN_IEEE1914_VAL_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_IEEE1914_VAL_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047032010ll;
    __cavm_csr_fatal("BFN_IEEE1914_VAL", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_IEEE1914_VAL cavm_bfn_ieee1914_val_t
#define bustype_CAVM_BFN_IEEE1914_VAL CSR_TYPE_RSL
#define basename_CAVM_BFN_IEEE1914_VAL "BFN_IEEE1914_VAL"
#define busnum_CAVM_BFN_IEEE1914_VAL 0
#define arguments_CAVM_BFN_IEEE1914_VAL -1,-1,-1,-1

/**
 * Register (RSL) bfn_int_ieee1914_val
 *
 * INTERNAL: BFN internal IEEE1914 timestamp value Register
 */
union cavm_bfn_int_ieee1914_val
{
    uint64_t u;
    struct cavm_bfn_int_ieee1914_val_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_57_63        : 7;
        uint64_t ts_update             : 1;  /**< [ 56: 56](R/W/H) If written 1, update the [NSEC] and [FNSEC] counts.  If written 0,
                                                                 the [NSEC] and [FNSEC] counts remain unchanged. */
        uint64_t reserved_29_55        : 27;
        uint64_t nsec                  : 24; /**< [ 28:  5](R/W/H) Current value of the internal IEEE 1914 timestamp
                                                                 integer nanosecond field. */
        uint64_t fnsec                 : 5;  /**< [  4:  0](R/W/H) Current value of the internal IEEE 1914 timestamp
                                                                 fractional nanosecond field. */
#else /* Word 0 - Little Endian */
        uint64_t fnsec                 : 5;  /**< [  4:  0](R/W/H) Current value of the internal IEEE 1914 timestamp
                                                                 fractional nanosecond field. */
        uint64_t nsec                  : 24; /**< [ 28:  5](R/W/H) Current value of the internal IEEE 1914 timestamp
                                                                 integer nanosecond field. */
        uint64_t reserved_29_55        : 27;
        uint64_t ts_update             : 1;  /**< [ 56: 56](R/W/H) If written 1, update the [NSEC] and [FNSEC] counts.  If written 0,
                                                                 the [NSEC] and [FNSEC] counts remain unchanged. */
        uint64_t reserved_57_63        : 7;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_int_ieee1914_val_s cn; */
};
typedef union cavm_bfn_int_ieee1914_val cavm_bfn_int_ieee1914_val_t;

#define CAVM_BFN_INT_IEEE1914_VAL CAVM_BFN_INT_IEEE1914_VAL_FUNC()
static inline uint64_t CAVM_BFN_INT_IEEE1914_VAL_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_INT_IEEE1914_VAL_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047021110ll;
    __cavm_csr_fatal("BFN_INT_IEEE1914_VAL", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_INT_IEEE1914_VAL cavm_bfn_int_ieee1914_val_t
#define bustype_CAVM_BFN_INT_IEEE1914_VAL CSR_TYPE_RSL
#define basename_CAVM_BFN_INT_IEEE1914_VAL "BFN_INT_IEEE1914_VAL"
#define busnum_CAVM_BFN_INT_IEEE1914_VAL 0
#define arguments_CAVM_BFN_INT_IEEE1914_VAL -1,-1,-1,-1

/**
 * Register (RSL) bfn_int_psm_val
 *
 * INTERNAL: BFN internal PSM timestamp value Register
 */
union cavm_bfn_int_psm_val
{
    uint64_t u;
    struct cavm_bfn_int_psm_val_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_59_63        : 5;
        uint64_t frame_update          : 1;  /**< [ 58: 58](R/W/H) If written 1, update [FRAME] count.  If written 0, the [FRAME]
                                                                 count remains unchanged. */
        uint64_t sf_update             : 1;  /**< [ 57: 57](R/W/H) If written 1, update [SUBFRAME] count.  If written 0, the [SUBFRAME]
                                                                 count remains unchanged. */
        uint64_t tick_update           : 1;  /**< [ 56: 56](R/W/H) If written 1, update [TICK] count.  If written 0, the [TICK]
                                                                 count remains unchanged. */
        uint64_t reserved_32_55        : 24;
        uint64_t frame                 : 12; /**< [ 31: 20](R/W/H) Current value of the internal PSM frame counter. */
        uint64_t subframe              : 4;  /**< [ 19: 16](R/W/H) Current value of the internal PSM subframe counter. */
        uint64_t tick                  : 16; /**< [ 15:  0](R/W/H) Current value of the internal PSM tick counter. */
#else /* Word 0 - Little Endian */
        uint64_t tick                  : 16; /**< [ 15:  0](R/W/H) Current value of the internal PSM tick counter. */
        uint64_t subframe              : 4;  /**< [ 19: 16](R/W/H) Current value of the internal PSM subframe counter. */
        uint64_t frame                 : 12; /**< [ 31: 20](R/W/H) Current value of the internal PSM frame counter. */
        uint64_t reserved_32_55        : 24;
        uint64_t tick_update           : 1;  /**< [ 56: 56](R/W/H) If written 1, update [TICK] count.  If written 0, the [TICK]
                                                                 count remains unchanged. */
        uint64_t sf_update             : 1;  /**< [ 57: 57](R/W/H) If written 1, update [SUBFRAME] count.  If written 0, the [SUBFRAME]
                                                                 count remains unchanged. */
        uint64_t frame_update          : 1;  /**< [ 58: 58](R/W/H) If written 1, update [FRAME] count.  If written 0, the [FRAME]
                                                                 count remains unchanged. */
        uint64_t reserved_59_63        : 5;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_int_psm_val_s cn; */
};
typedef union cavm_bfn_int_psm_val cavm_bfn_int_psm_val_t;

#define CAVM_BFN_INT_PSM_VAL CAVM_BFN_INT_PSM_VAL_FUNC()
static inline uint64_t CAVM_BFN_INT_PSM_VAL_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_INT_PSM_VAL_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047021100ll;
    __cavm_csr_fatal("BFN_INT_PSM_VAL", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_INT_PSM_VAL cavm_bfn_int_psm_val_t
#define bustype_CAVM_BFN_INT_PSM_VAL CSR_TYPE_RSL
#define basename_CAVM_BFN_INT_PSM_VAL "BFN_INT_PSM_VAL"
#define busnum_CAVM_BFN_INT_PSM_VAL 0
#define arguments_CAVM_BFN_INT_PSM_VAL -1,-1,-1,-1

/**
 * Register (RSL) bfn_internal_timer_cfg
 *
 * INTERNAL: BFN internal timer configuration Register
 */
union cavm_bfn_internal_timer_cfg
{
    uint64_t u;
    struct cavm_bfn_internal_timer_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t en                    : 1;  /**< [ 63: 63](R/W) Enable for BFN internal timer mode. */
        uint64_t reserved_42_62        : 21;
        uint64_t ns_inc                : 10; /**< [ 41: 32](R/W) Defines the increment value used for the IEEE1914 timestamp
                                                                 each BCLK cycle during BFN internal timer mode.  This is
                                                                 a 2.8 fixed-point value. */
        uint64_t subf_ticks            : 16; /**< [ 31: 16](R/W) Sets the maximum tick count value for generating an SOSF during
                                                                 BFN internal timer mode.  The BFN will generate a SOSF when the
                                                                 tick counter reaches the value specified by [SUBF_TICKS]. */
        uint64_t tick_div              : 16; /**< [ 15:  0](R/W) Divider value for generating the tick clock from the BCLK during
                                                                 BFN internal timer mode.  This is a fixed-point value, consisting
                                                                 of an 8-bit integer and an 8-bit fraction.  A value of 0x0280
                                                                 would generate a tick-clock that is 2.5x slower than BCLK.  The
                                                                 minimum value of 0x0100 represents a 1:1 ratio of BCLK to
                                                                 tick-clock.  The actual [TICK_DIV] value should be chosen to
                                                                 prevent the 16-bit TICK counter from rolling over within the
                                                                 1ms subframe interval. */
#else /* Word 0 - Little Endian */
        uint64_t tick_div              : 16; /**< [ 15:  0](R/W) Divider value for generating the tick clock from the BCLK during
                                                                 BFN internal timer mode.  This is a fixed-point value, consisting
                                                                 of an 8-bit integer and an 8-bit fraction.  A value of 0x0280
                                                                 would generate a tick-clock that is 2.5x slower than BCLK.  The
                                                                 minimum value of 0x0100 represents a 1:1 ratio of BCLK to
                                                                 tick-clock.  The actual [TICK_DIV] value should be chosen to
                                                                 prevent the 16-bit TICK counter from rolling over within the
                                                                 1ms subframe interval. */
        uint64_t subf_ticks            : 16; /**< [ 31: 16](R/W) Sets the maximum tick count value for generating an SOSF during
                                                                 BFN internal timer mode.  The BFN will generate a SOSF when the
                                                                 tick counter reaches the value specified by [SUBF_TICKS]. */
        uint64_t ns_inc                : 10; /**< [ 41: 32](R/W) Defines the increment value used for the IEEE1914 timestamp
                                                                 each BCLK cycle during BFN internal timer mode.  This is
                                                                 a 2.8 fixed-point value. */
        uint64_t reserved_42_62        : 21;
        uint64_t en                    : 1;  /**< [ 63: 63](R/W) Enable for BFN internal timer mode. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_internal_timer_cfg_s cn; */
};
typedef union cavm_bfn_internal_timer_cfg cavm_bfn_internal_timer_cfg_t;

#define CAVM_BFN_INTERNAL_TIMER_CFG CAVM_BFN_INTERNAL_TIMER_CFG_FUNC()
static inline uint64_t CAVM_BFN_INTERNAL_TIMER_CFG_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_INTERNAL_TIMER_CFG_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047021000ll;
    __cavm_csr_fatal("BFN_INTERNAL_TIMER_CFG", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_INTERNAL_TIMER_CFG cavm_bfn_internal_timer_cfg_t
#define bustype_CAVM_BFN_INTERNAL_TIMER_CFG CSR_TYPE_RSL
#define basename_CAVM_BFN_INTERNAL_TIMER_CFG "BFN_INTERNAL_TIMER_CFG"
#define busnum_CAVM_BFN_INTERNAL_TIMER_CFG 0
#define arguments_CAVM_BFN_INTERNAL_TIMER_CFG -1,-1,-1,-1

/**
 * Register (RSL) bfn_match_cfg
 *
 * BFN Match Configuration Register
 * This register configures the logic that timer match functionality
 * that can be used for timestamp synchronization or capture.
 */
union cavm_bfn_match_cfg
{
    uint64_t u;
    struct cavm_bfn_match_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_10_63        : 54;
        uint64_t match_sel             : 2;  /**< [  9:  8](R/W) Timestamp to be matched
                                                                 0x0 = PSM timestamp.
                                                                 0x1 = IEEE 1914 timestamp.
                                                                 0x2 = PTP timestamp.
                                                                 0x3 = Reserved. */
        uint64_t reserved_2_7          : 6;
        uint64_t match_exact           : 1;  /**< [  1:  1](R/W) When set to 1, the timestamp match will only succeed when
                                                                 the value in BFN_MATCH_VALUE exactly matches the timestamp
                                                                 selected by [MATCH_SEL].  When 0, the match will succeed
                                                                 when the selected timestamp is equal to or greater than
                                                                 the value in BFN_MATCH_VALUE. */
        uint64_t match_en              : 1;  /**< [  0:  0](R/W) Enables the timestamp match functionality. */
#else /* Word 0 - Little Endian */
        uint64_t match_en              : 1;  /**< [  0:  0](R/W) Enables the timestamp match functionality. */
        uint64_t match_exact           : 1;  /**< [  1:  1](R/W) When set to 1, the timestamp match will only succeed when
                                                                 the value in BFN_MATCH_VALUE exactly matches the timestamp
                                                                 selected by [MATCH_SEL].  When 0, the match will succeed
                                                                 when the selected timestamp is equal to or greater than
                                                                 the value in BFN_MATCH_VALUE. */
        uint64_t reserved_2_7          : 6;
        uint64_t match_sel             : 2;  /**< [  9:  8](R/W) Timestamp to be matched
                                                                 0x0 = PSM timestamp.
                                                                 0x1 = IEEE 1914 timestamp.
                                                                 0x2 = PTP timestamp.
                                                                 0x3 = Reserved. */
        uint64_t reserved_10_63        : 54;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_match_cfg_s cn; */
};
typedef union cavm_bfn_match_cfg cavm_bfn_match_cfg_t;

#define CAVM_BFN_MATCH_CFG CAVM_BFN_MATCH_CFG_FUNC()
static inline uint64_t CAVM_BFN_MATCH_CFG_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_MATCH_CFG_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047034000ll;
    __cavm_csr_fatal("BFN_MATCH_CFG", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_MATCH_CFG cavm_bfn_match_cfg_t
#define bustype_CAVM_BFN_MATCH_CFG CSR_TYPE_RSL
#define basename_CAVM_BFN_MATCH_CFG "BFN_MATCH_CFG"
#define busnum_CAVM_BFN_MATCH_CFG 0
#define arguments_CAVM_BFN_MATCH_CFG -1,-1,-1,-1

/**
 * Register (RSL) bfn_match_value
 *
 * BFN Match Value Register
 * This register sets the value to be compared for the timer match.
 */
union cavm_bfn_match_value
{
    uint64_t u;
    struct cavm_bfn_match_value_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t match_value           : 64; /**< [ 63:  0](R/W) The value to be compared for the timer match, as described in
                                                                 BFN_MATCH_CFG. */
#else /* Word 0 - Little Endian */
        uint64_t match_value           : 64; /**< [ 63:  0](R/W) The value to be compared for the timer match, as described in
                                                                 BFN_MATCH_CFG. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_match_value_s cn; */
};
typedef union cavm_bfn_match_value cavm_bfn_match_value_t;

#define CAVM_BFN_MATCH_VALUE CAVM_BFN_MATCH_VALUE_FUNC()
static inline uint64_t CAVM_BFN_MATCH_VALUE_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_MATCH_VALUE_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047034010ll;
    __cavm_csr_fatal("BFN_MATCH_VALUE", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_MATCH_VALUE cavm_bfn_match_value_t
#define bustype_CAVM_BFN_MATCH_VALUE CSR_TYPE_RSL
#define basename_CAVM_BFN_MATCH_VALUE "BFN_MATCH_VALUE"
#define busnum_CAVM_BFN_MATCH_VALUE 0
#define arguments_CAVM_BFN_MATCH_VALUE -1,-1,-1,-1

/**
 * Register (RSL) bfn_psm_cfg
 *
 * BFN PSM Counter Configuration Register
 * This register configures the logic that generates the PSM frame,
 * subframe, and tick counters.
 */
union cavm_bfn_psm_cfg
{
    uint64_t u;
    struct cavm_bfn_psm_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t frame_max             : 24; /**< [ 63: 40](R/W) Defines the length of a PSM frame in BTS_CLK cycles.  The
                                                                 default value corresponds to a 10 msec frame when the BTS_CLK
                                                                 is 30.72 MHz. */
        uint64_t subf_max              : 20; /**< [ 39: 20](R/W) Defines the length of a PSM subframe in BTS_CLK cycles.  The
                                                                 default value corresponds to a 1 msec subframe when the BTS_CLK
                                                                 is 30.72 MHz. */
        uint64_t tick_inc              : 20; /**< [ 19:  0](R/W) Defines the increment value for the PSM tick counter, on each
                                                                 BTS_CLK cycle.  The increment value is a 2.16 fixed point number,
                                                                 and the default value corresponds to a 15.625 nsec tick when
                                                                 the BTS_CLK is 30.72 MHz. */
#else /* Word 0 - Little Endian */
        uint64_t tick_inc              : 20; /**< [ 19:  0](R/W) Defines the increment value for the PSM tick counter, on each
                                                                 BTS_CLK cycle.  The increment value is a 2.16 fixed point number,
                                                                 and the default value corresponds to a 15.625 nsec tick when
                                                                 the BTS_CLK is 30.72 MHz. */
        uint64_t subf_max              : 20; /**< [ 39: 20](R/W) Defines the length of a PSM subframe in BTS_CLK cycles.  The
                                                                 default value corresponds to a 1 msec subframe when the BTS_CLK
                                                                 is 30.72 MHz. */
        uint64_t frame_max             : 24; /**< [ 63: 40](R/W) Defines the length of a PSM frame in BTS_CLK cycles.  The
                                                                 default value corresponds to a 10 msec frame when the BTS_CLK
                                                                 is 30.72 MHz. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_psm_cfg_s cn; */
};
typedef union cavm_bfn_psm_cfg cavm_bfn_psm_cfg_t;

#define CAVM_BFN_PSM_CFG CAVM_BFN_PSM_CFG_FUNC()
static inline uint64_t CAVM_BFN_PSM_CFG_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_PSM_CFG_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047031000ll;
    __cavm_csr_fatal("BFN_PSM_CFG", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_PSM_CFG cavm_bfn_psm_cfg_t
#define bustype_CAVM_BFN_PSM_CFG CSR_TYPE_RSL
#define basename_CAVM_BFN_PSM_CFG "BFN_PSM_CFG"
#define busnum_CAVM_BFN_PSM_CFG 0
#define arguments_CAVM_BFN_PSM_CFG -1,-1,-1,-1

/**
 * Register (RSL) bfn_psm_cnt_val
 *
 * BFN PSM Counter Value Register
 * This register contains the current value of the PSM frame and
 * subframe cycle counters.
 */
union cavm_bfn_psm_cnt_val
{
    uint64_t u;
    struct cavm_bfn_psm_cnt_val_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_56_63        : 8;
        uint64_t frame_cnt             : 24; /**< [ 55: 32](RO/H) Current value of the PSM frame cycle counter.  The cycle counter
                                                                 is a running count of BTS_CLK cycles during a frame. */
        uint64_t reserved_20_31        : 12;
        uint64_t sf_cnt                : 20; /**< [ 19:  0](RO/H) Current value of the PSM subframe cycle counter.  The cycle counter
                                                                 is a running count of BTS_CLK cycles during a subframe. */
#else /* Word 0 - Little Endian */
        uint64_t sf_cnt                : 20; /**< [ 19:  0](RO/H) Current value of the PSM subframe cycle counter.  The cycle counter
                                                                 is a running count of BTS_CLK cycles during a subframe. */
        uint64_t reserved_20_31        : 12;
        uint64_t frame_cnt             : 24; /**< [ 55: 32](RO/H) Current value of the PSM frame cycle counter.  The cycle counter
                                                                 is a running count of BTS_CLK cycles during a frame. */
        uint64_t reserved_56_63        : 8;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_psm_cnt_val_s cn; */
};
typedef union cavm_bfn_psm_cnt_val cavm_bfn_psm_cnt_val_t;

#define CAVM_BFN_PSM_CNT_VAL CAVM_BFN_PSM_CNT_VAL_FUNC()
static inline uint64_t CAVM_BFN_PSM_CNT_VAL_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_PSM_CNT_VAL_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047031020ll;
    __cavm_csr_fatal("BFN_PSM_CNT_VAL", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_PSM_CNT_VAL cavm_bfn_psm_cnt_val_t
#define bustype_CAVM_BFN_PSM_CNT_VAL CSR_TYPE_RSL
#define basename_CAVM_BFN_PSM_CNT_VAL "BFN_PSM_CNT_VAL"
#define busnum_CAVM_BFN_PSM_CNT_VAL 0
#define arguments_CAVM_BFN_PSM_CNT_VAL -1,-1,-1,-1

/**
 * Register (RSL) bfn_psm_val
 *
 * BFN PSM Counter Value Register
 * This register contains the current value of the PSM frame,
 * subframe, and tick counters.
 */
union cavm_bfn_psm_val
{
    uint64_t u;
    struct cavm_bfn_psm_val_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t frame                 : 12; /**< [ 31: 20](RO/H) Current value of the PSM frame counter. */
        uint64_t subframe              : 4;  /**< [ 19: 16](RO/H) Current value of the PSM subframe counter. */
        uint64_t tick                  : 16; /**< [ 15:  0](RO/H) Current value of the PSM tick counter. */
#else /* Word 0 - Little Endian */
        uint64_t tick                  : 16; /**< [ 15:  0](RO/H) Current value of the PSM tick counter. */
        uint64_t subframe              : 4;  /**< [ 19: 16](RO/H) Current value of the PSM subframe counter. */
        uint64_t frame                 : 12; /**< [ 31: 20](RO/H) Current value of the PSM frame counter. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_psm_val_s cn; */
};
typedef union cavm_bfn_psm_val cavm_bfn_psm_val_t;

#define CAVM_BFN_PSM_VAL CAVM_BFN_PSM_VAL_FUNC()
static inline uint64_t CAVM_BFN_PSM_VAL_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_PSM_VAL_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047031010ll;
    __cavm_csr_fatal("BFN_PSM_VAL", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_PSM_VAL cavm_bfn_psm_val_t
#define bustype_CAVM_BFN_PSM_VAL CSR_TYPE_RSL
#define basename_CAVM_BFN_PSM_VAL "BFN_PSM_VAL"
#define busnum_CAVM_BFN_PSM_VAL 0
#define arguments_CAVM_BFN_PSM_VAL -1,-1,-1,-1

/**
 * Register (RSL) bfn_ptp_cfg
 *
 * BFN PTP Configuration Register
 * This register configures the logic that samples the PTP timestamp
 * into the BTS_CLK domain, within the BFN block.
 */
union cavm_bfn_ptp_cfg
{
    uint64_t u;
    struct cavm_bfn_ptp_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_6_63         : 58;
        uint64_t ptp_sample_rate       : 6;  /**< [  5:  0](R/W) Rate in BCLK cycles to sample the PTP timestamp value and
                                                                 pass it to the BTS_CLK domain.  The value of 0x0 will disable
                                                                 the PTP sampling.  The recommended setting will depend on
                                                                 the BCLK frequency.

                                                                 Internal:
                                                                 This is the sampling rate for the PTP value going from
                                                                 BCLK to BTS_CLK, within the BFN block.
                                                                 FIXME, add recommended settings */
#else /* Word 0 - Little Endian */
        uint64_t ptp_sample_rate       : 6;  /**< [  5:  0](R/W) Rate in BCLK cycles to sample the PTP timestamp value and
                                                                 pass it to the BTS_CLK domain.  The value of 0x0 will disable
                                                                 the PTP sampling.  The recommended setting will depend on
                                                                 the BCLK frequency.

                                                                 Internal:
                                                                 This is the sampling rate for the PTP value going from
                                                                 BCLK to BTS_CLK, within the BFN block.
                                                                 FIXME, add recommended settings */
        uint64_t reserved_6_63         : 58;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_ptp_cfg_s cn; */
};
typedef union cavm_bfn_ptp_cfg cavm_bfn_ptp_cfg_t;

#define CAVM_BFN_PTP_CFG CAVM_BFN_PTP_CFG_FUNC()
static inline uint64_t CAVM_BFN_PTP_CFG_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_PTP_CFG_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047033000ll;
    __cavm_csr_fatal("BFN_PTP_CFG", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_PTP_CFG cavm_bfn_ptp_cfg_t
#define bustype_CAVM_BFN_PTP_CFG CSR_TYPE_RSL
#define basename_CAVM_BFN_PTP_CFG "BFN_PTP_CFG"
#define busnum_CAVM_BFN_PTP_CFG 0
#define arguments_CAVM_BFN_PTP_CFG -1,-1,-1,-1

/**
 * Register (RSL) bfn_ptp_sample_cfg
 *
 * BFN PTP Sampling Configuration Register
 * This register configures the logic that samples the PTP timestamp from
 * the PTP block into the BFN logic.
 */
union cavm_bfn_ptp_sample_cfg
{
    uint64_t u;
    struct cavm_bfn_ptp_sample_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_4_63         : 60;
        uint64_t ptp_sample_rate       : 4;  /**< [  3:  0](R/W) Rate in SCLK cycles to sample the PTP timestamp value and
                                                                 pass it to the BCLK domain.  The value of 0x0 will disable
                                                                 the PTP sampling.  The recommended setting will depend on
                                                                 the SCLK and BCLK frequencies.

                                                                 Internal:
                                                                 This is the sampling rate sent to the PCBW (in ROC channel
                                                                 north), for the PTP value going from SCLK to BCLK.  In 105,
                                                                 this sampling is done by the NCBW.
                                                                 FIXME, add recommended settings */
#else /* Word 0 - Little Endian */
        uint64_t ptp_sample_rate       : 4;  /**< [  3:  0](R/W) Rate in SCLK cycles to sample the PTP timestamp value and
                                                                 pass it to the BCLK domain.  The value of 0x0 will disable
                                                                 the PTP sampling.  The recommended setting will depend on
                                                                 the SCLK and BCLK frequencies.

                                                                 Internal:
                                                                 This is the sampling rate sent to the PCBW (in ROC channel
                                                                 north), for the PTP value going from SCLK to BCLK.  In 105,
                                                                 this sampling is done by the NCBW.
                                                                 FIXME, add recommended settings */
        uint64_t reserved_4_63         : 60;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_ptp_sample_cfg_s cn; */
};
typedef union cavm_bfn_ptp_sample_cfg cavm_bfn_ptp_sample_cfg_t;

#define CAVM_BFN_PTP_SAMPLE_CFG CAVM_BFN_PTP_SAMPLE_CFG_FUNC()
static inline uint64_t CAVM_BFN_PTP_SAMPLE_CFG_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_PTP_SAMPLE_CFG_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047020300ll;
    __cavm_csr_fatal("BFN_PTP_SAMPLE_CFG", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_PTP_SAMPLE_CFG cavm_bfn_ptp_sample_cfg_t
#define bustype_CAVM_BFN_PTP_SAMPLE_CFG CSR_TYPE_RSL
#define basename_CAVM_BFN_PTP_SAMPLE_CFG "BFN_PTP_SAMPLE_CFG"
#define busnum_CAVM_BFN_PTP_SAMPLE_CFG 0
#define arguments_CAVM_BFN_PTP_SAMPLE_CFG -1,-1,-1,-1

/**
 * Register (RSL) bfn_ptp_val
 *
 * BFN PTP Timestamp Value Register
 * This register contains the current value of the PTP timestamp.
 */
union cavm_bfn_ptp_val
{
    uint64_t u;
    struct cavm_bfn_ptp_val_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ptp                   : 64; /**< [ 63:  0](RO/H) Current value of the PTP timestamp. */
#else /* Word 0 - Little Endian */
        uint64_t ptp                   : 64; /**< [ 63:  0](RO/H) Current value of the PTP timestamp. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_ptp_val_s cn; */
};
typedef union cavm_bfn_ptp_val cavm_bfn_ptp_val_t;

#define CAVM_BFN_PTP_VAL CAVM_BFN_PTP_VAL_FUNC()
static inline uint64_t CAVM_BFN_PTP_VAL_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_PTP_VAL_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047033010ll;
    __cavm_csr_fatal("BFN_PTP_VAL", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_PTP_VAL cavm_bfn_ptp_val_t
#define bustype_CAVM_BFN_PTP_VAL CSR_TYPE_RSL
#define basename_CAVM_BFN_PTP_VAL "BFN_PTP_VAL"
#define busnum_CAVM_BFN_PTP_VAL 0
#define arguments_CAVM_BFN_PTP_VAL -1,-1,-1,-1

/**
 * Register (RSL) bfn_rec_clk#_ctrl
 *
 * INTERNAL: BFN Recovered Clock Monitor Control Registers
 *
 * These registers control the rec_clk_out monitor ports for external
 * visibility of the recovered clocks.
 *
 * Internal:
 * FIXME?  keep this functionality for f95o?
 */
union cavm_bfn_rec_clkx_ctrl
{
    uint64_t u;
    struct cavm_bfn_rec_clkx_ctrl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_11_63        : 53;
        uint64_t clk_div               : 3;  /**< [ 10:  8](R/W) Divider value for the selected recovered clock.  Valid divider
                                                                 settings are 1, 2, and 4, and 0 disables the REC_CLK_OUT output.
                                                                 The divider is used to reduce the frequency to below 200 MHz so
                                                                 that the signal can be driven on a chip output.
                                                                 The recommended settings are are as follows:
                                                                 \<pre\>
                                                                   SERDES       recovered    [CLK_DIV]   CLKOUT
                                                                   datarate     clock                    frequency
                                                                   (Gb/s)       (Mhz)                    (MHz)
                                                                  --------------------------------------------------
                                                                   25.78125     644.53125      0x4       161.1328125
                                                                   10.31250     515.62500      0x4       128.9062500
                                                                    9.83040     491.52000      0x4       122.8800000
                                                                    6.14400     307.20000      0x2       153.6000000
                                                                    4.91520     245.76000      0x2       122.8800000
                                                                    3.07200     153.60000      0x1       153.6000000
                                                                    2.45760     122.88000      0x1       122.8800000
                                                                    1.25000      62.50000      0x1        62.5000000

                                                                   bcn_clk      307.20000      0x2       153.6000000
                                                                   bcn_pll_ref  122.88000      0x1       122.8800000
                                                                 \</pre\> */
        uint64_t reserved_5_7          : 3;
        uint64_t clk_sel               : 5;  /**< [  4:  0](R/W) Clock selection.
                                                                 0x00 = GSERC0 lane 0 recovered receive clock.
                                                                 0x01 = GSERC0 lane 0 transmit clock from GSERC PLL.
                                                                 0x02 = GSERC0 lane 1 recovered receive clock.
                                                                 0x03 = GSERC0 lane 1 transmit clock from GSERC PLL.
                                                                 0x04 = GSERC1 lane 0 recovered receive clock.
                                                                 0x05 = GSERC1 lane 0 transmit clock from GSERC PLL.
                                                                 0x06 = GSERC1 lane 1 recovered receive clock.
                                                                 0x07 = GSERC1 lane 1 transmit clock from GSERC PLL.
                                                                 0x08 = GSERC2 lane 0 recovered receive clock.
                                                                 0x09 = GSERC2 lane 0 transmit clock from GSERC PLL.
                                                                 0x0A = GSERC2 lane 1 recovered receive clock.
                                                                 0x0B = GSERC2 lane 1 transmit clock from GSERC PLL.
                                                                 0x0C = GSERC3 lane 0 recovered receive clock.
                                                                 0x0D = GSERC3 lane 0 transmit clock from GSERC PLL.
                                                                 0x0E = GSERC3 lane 1 recovered receive clock.
                                                                 0x0F = GSERC3 lane 1 transmit clock from GSERC PLL.
                                                                 0x10 = GSERC4 lane 0 recovered receive clock.
                                                                 0x11 = GSERC4 lane 0 transmit clock from GSERC PLL.
                                                                 0x12 = GSERC4 lane 1 recovered receive clock.
                                                                 0x13 = GSERC4 lane 1 transmit clock from GSERC PLL.
                                                                 0x14 = GSERR0 lane 0 recovered receive clock.
                                                                 0x15 = GSERR0 lane 1 recovered receive clock.
                                                                 0x16 = GSERR0 lane 2 recovered receive clock.
                                                                 0x17 = GSERR0 lane 3 recovered receive clock.
                                                                 0x18 = bcn_pll reference clock.
                                                                 0x19 = bcn_clk. */
#else /* Word 0 - Little Endian */
        uint64_t clk_sel               : 5;  /**< [  4:  0](R/W) Clock selection.
                                                                 0x00 = GSERC0 lane 0 recovered receive clock.
                                                                 0x01 = GSERC0 lane 0 transmit clock from GSERC PLL.
                                                                 0x02 = GSERC0 lane 1 recovered receive clock.
                                                                 0x03 = GSERC0 lane 1 transmit clock from GSERC PLL.
                                                                 0x04 = GSERC1 lane 0 recovered receive clock.
                                                                 0x05 = GSERC1 lane 0 transmit clock from GSERC PLL.
                                                                 0x06 = GSERC1 lane 1 recovered receive clock.
                                                                 0x07 = GSERC1 lane 1 transmit clock from GSERC PLL.
                                                                 0x08 = GSERC2 lane 0 recovered receive clock.
                                                                 0x09 = GSERC2 lane 0 transmit clock from GSERC PLL.
                                                                 0x0A = GSERC2 lane 1 recovered receive clock.
                                                                 0x0B = GSERC2 lane 1 transmit clock from GSERC PLL.
                                                                 0x0C = GSERC3 lane 0 recovered receive clock.
                                                                 0x0D = GSERC3 lane 0 transmit clock from GSERC PLL.
                                                                 0x0E = GSERC3 lane 1 recovered receive clock.
                                                                 0x0F = GSERC3 lane 1 transmit clock from GSERC PLL.
                                                                 0x10 = GSERC4 lane 0 recovered receive clock.
                                                                 0x11 = GSERC4 lane 0 transmit clock from GSERC PLL.
                                                                 0x12 = GSERC4 lane 1 recovered receive clock.
                                                                 0x13 = GSERC4 lane 1 transmit clock from GSERC PLL.
                                                                 0x14 = GSERR0 lane 0 recovered receive clock.
                                                                 0x15 = GSERR0 lane 1 recovered receive clock.
                                                                 0x16 = GSERR0 lane 2 recovered receive clock.
                                                                 0x17 = GSERR0 lane 3 recovered receive clock.
                                                                 0x18 = bcn_pll reference clock.
                                                                 0x19 = bcn_clk. */
        uint64_t reserved_5_7          : 3;
        uint64_t clk_div               : 3;  /**< [ 10:  8](R/W) Divider value for the selected recovered clock.  Valid divider
                                                                 settings are 1, 2, and 4, and 0 disables the REC_CLK_OUT output.
                                                                 The divider is used to reduce the frequency to below 200 MHz so
                                                                 that the signal can be driven on a chip output.
                                                                 The recommended settings are are as follows:
                                                                 \<pre\>
                                                                   SERDES       recovered    [CLK_DIV]   CLKOUT
                                                                   datarate     clock                    frequency
                                                                   (Gb/s)       (Mhz)                    (MHz)
                                                                  --------------------------------------------------
                                                                   25.78125     644.53125      0x4       161.1328125
                                                                   10.31250     515.62500      0x4       128.9062500
                                                                    9.83040     491.52000      0x4       122.8800000
                                                                    6.14400     307.20000      0x2       153.6000000
                                                                    4.91520     245.76000      0x2       122.8800000
                                                                    3.07200     153.60000      0x1       153.6000000
                                                                    2.45760     122.88000      0x1       122.8800000
                                                                    1.25000      62.50000      0x1        62.5000000

                                                                   bcn_clk      307.20000      0x2       153.6000000
                                                                   bcn_pll_ref  122.88000      0x1       122.8800000
                                                                 \</pre\> */
        uint64_t reserved_11_63        : 53;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_rec_clkx_ctrl_s cn; */
};
typedef union cavm_bfn_rec_clkx_ctrl cavm_bfn_rec_clkx_ctrl_t;

static inline uint64_t CAVM_BFN_REC_CLKX_CTRL(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_REC_CLKX_CTRL(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_F95O) && (a<=2))
        return 0x87e047020200ll + 0x10ll * ((a) & 0x3);
    __cavm_csr_fatal("BFN_REC_CLKX_CTRL", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_REC_CLKX_CTRL(a) cavm_bfn_rec_clkx_ctrl_t
#define bustype_CAVM_BFN_REC_CLKX_CTRL(a) CSR_TYPE_RSL
#define basename_CAVM_BFN_REC_CLKX_CTRL(a) "BFN_REC_CLKX_CTRL"
#define busnum_CAVM_BFN_REC_CLKX_CTRL(a) (a)
#define arguments_CAVM_BFN_REC_CLKX_CTRL(a) (a),-1,-1,-1

/**
 * Register (RSL) bfn_rec_clk_to_ptp
 *
 * INTERNAL: BFN Recovered Clock To PTP Control Register
 *
 * This register controls the recovered clock that is sent to
 * the PTP block.  See also MIO_PTP_CLOCK_CFG[EXT_CLK_IN].
 *
 * Internal:
 * FIXME?  keep this functionality for f95o?
 */
union cavm_bfn_rec_clk_to_ptp
{
    uint64_t u;
    struct cavm_bfn_rec_clk_to_ptp_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_11_63        : 53;
        uint64_t clk_div               : 3;  /**< [ 10:  8](R/W) Divider value for the selected PTP clock.  Valid divider
                                                                 settings are 1, 2, and 4, and 0 disables the PTP clock.
                                                                 The divider is used to reduce the frequency to below 200 MHz so
                                                                 that the clock can be used by the PTP logic.
                                                                 See BFN_REC_CLK()_CTRL[CLK_DIV] for recommended values. */
        uint64_t reserved_5_7          : 3;
        uint64_t clk_sel               : 5;  /**< [  4:  0](R/W) Clock selection.  See BFN_REC_CLK()_CTRL[CLK_SEL] for values. */
#else /* Word 0 - Little Endian */
        uint64_t clk_sel               : 5;  /**< [  4:  0](R/W) Clock selection.  See BFN_REC_CLK()_CTRL[CLK_SEL] for values. */
        uint64_t reserved_5_7          : 3;
        uint64_t clk_div               : 3;  /**< [ 10:  8](R/W) Divider value for the selected PTP clock.  Valid divider
                                                                 settings are 1, 2, and 4, and 0 disables the PTP clock.
                                                                 The divider is used to reduce the frequency to below 200 MHz so
                                                                 that the clock can be used by the PTP logic.
                                                                 See BFN_REC_CLK()_CTRL[CLK_DIV] for recommended values. */
        uint64_t reserved_11_63        : 53;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_rec_clk_to_ptp_s cn; */
};
typedef union cavm_bfn_rec_clk_to_ptp cavm_bfn_rec_clk_to_ptp_t;

#define CAVM_BFN_REC_CLK_TO_PTP CAVM_BFN_REC_CLK_TO_PTP_FUNC()
static inline uint64_t CAVM_BFN_REC_CLK_TO_PTP_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_REC_CLK_TO_PTP_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047020280ll;
    __cavm_csr_fatal("BFN_REC_CLK_TO_PTP", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_REC_CLK_TO_PTP cavm_bfn_rec_clk_to_ptp_t
#define bustype_CAVM_BFN_REC_CLK_TO_PTP CSR_TYPE_RSL
#define basename_CAVM_BFN_REC_CLK_TO_PTP "BFN_REC_CLK_TO_PTP"
#define busnum_CAVM_BFN_REC_CLK_TO_PTP 0
#define arguments_CAVM_BFN_REC_CLK_TO_PTP -1,-1,-1,-1

/**
 * Register (RSL) bfn_rfp_bad_count
 *
 * BFN RFP Bad Count Register
 * This register contains a running count of RFPs received with the
 * incorrect timing, where the PSM frame cycle counter does not
 * match BFN_PSM_CFG[FRAME_MAX].
 */
union cavm_bfn_rfp_bad_count
{
    uint64_t u;
    struct cavm_bfn_rfp_bad_count_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t rfp_bad               : 32; /**< [ 31:  0](R/W/H) Count of bad RFPs received. */
#else /* Word 0 - Little Endian */
        uint64_t rfp_bad               : 32; /**< [ 31:  0](R/W/H) Count of bad RFPs received. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_rfp_bad_count_s cn; */
};
typedef union cavm_bfn_rfp_bad_count cavm_bfn_rfp_bad_count_t;

#define CAVM_BFN_RFP_BAD_COUNT CAVM_BFN_RFP_BAD_COUNT_FUNC()
static inline uint64_t CAVM_BFN_RFP_BAD_COUNT_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_RFP_BAD_COUNT_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047037010ll;
    __cavm_csr_fatal("BFN_RFP_BAD_COUNT", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_RFP_BAD_COUNT cavm_bfn_rfp_bad_count_t
#define bustype_CAVM_BFN_RFP_BAD_COUNT CSR_TYPE_RSL
#define basename_CAVM_BFN_RFP_BAD_COUNT "BFN_RFP_BAD_COUNT"
#define busnum_CAVM_BFN_RFP_BAD_COUNT 0
#define arguments_CAVM_BFN_RFP_BAD_COUNT -1,-1,-1,-1

/**
 * Register (RSL) bfn_rfp_cyc_count
 *
 * BFN RFP Cycle Count Register
 * This register contains the value of the BTS_CLK domain frame
 * cycle counter when the most recent RFP was received.
 */
union cavm_bfn_rfp_cyc_count
{
    uint64_t u;
    struct cavm_bfn_rfp_cyc_count_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_24_63        : 40;
        uint64_t rfp_cyc               : 24; /**< [ 23:  0](RO/H) Frame counter value when RFP was received. */
#else /* Word 0 - Little Endian */
        uint64_t rfp_cyc               : 24; /**< [ 23:  0](RO/H) Frame counter value when RFP was received. */
        uint64_t reserved_24_63        : 40;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_rfp_cyc_count_s cn; */
};
typedef union cavm_bfn_rfp_cyc_count cavm_bfn_rfp_cyc_count_t;

#define CAVM_BFN_RFP_CYC_COUNT CAVM_BFN_RFP_CYC_COUNT_FUNC()
static inline uint64_t CAVM_BFN_RFP_CYC_COUNT_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_RFP_CYC_COUNT_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047037020ll;
    __cavm_csr_fatal("BFN_RFP_CYC_COUNT", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_RFP_CYC_COUNT cavm_bfn_rfp_cyc_count_t
#define bustype_CAVM_BFN_RFP_CYC_COUNT CSR_TYPE_RSL
#define basename_CAVM_BFN_RFP_CYC_COUNT "BFN_RFP_CYC_COUNT"
#define busnum_CAVM_BFN_RFP_CYC_COUNT 0
#define arguments_CAVM_BFN_RFP_CYC_COUNT -1,-1,-1,-1

/**
 * Register (RSL) bfn_rfp_good_count
 *
 * BFN RFP Good Count Register
 * This register contains a running count of RFPs received with the
 * correct timing, where the PSM frame cycle counter matches
 * BFN_PSM_CFG[FRAME_MAX].
 */
union cavm_bfn_rfp_good_count
{
    uint64_t u;
    struct cavm_bfn_rfp_good_count_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t rfp_good              : 32; /**< [ 31:  0](R/W/H) Count of good RFPs received. */
#else /* Word 0 - Little Endian */
        uint64_t rfp_good              : 32; /**< [ 31:  0](R/W/H) Count of good RFPs received. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_rfp_good_count_s cn; */
};
typedef union cavm_bfn_rfp_good_count cavm_bfn_rfp_good_count_t;

#define CAVM_BFN_RFP_GOOD_COUNT CAVM_BFN_RFP_GOOD_COUNT_FUNC()
static inline uint64_t CAVM_BFN_RFP_GOOD_COUNT_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_RFP_GOOD_COUNT_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047037000ll;
    __cavm_csr_fatal("BFN_RFP_GOOD_COUNT", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_RFP_GOOD_COUNT cavm_bfn_rfp_good_count_t
#define bustype_CAVM_BFN_RFP_GOOD_COUNT CSR_TYPE_RSL
#define basename_CAVM_BFN_RFP_GOOD_COUNT "BFN_RFP_GOOD_COUNT"
#define busnum_CAVM_BFN_RFP_GOOD_COUNT 0
#define arguments_CAVM_BFN_RFP_GOOD_COUNT -1,-1,-1,-1

/**
 * Register (RSL) bfn_scratch
 *
 * INTERNAL: BFN scratch Register
 */
union cavm_bfn_scratch
{
    uint64_t u;
    struct cavm_bfn_scratch_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t scratch_rw            : 32; /**< [ 31:  0](R/W) Internal:
                                                                 Reserved for scratch usage. */
#else /* Word 0 - Little Endian */
        uint64_t scratch_rw            : 32; /**< [ 31:  0](R/W) Internal:
                                                                 Reserved for scratch usage. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_scratch_s cn; */
};
typedef union cavm_bfn_scratch cavm_bfn_scratch_t;

#define CAVM_BFN_SCRATCH CAVM_BFN_SCRATCH_FUNC()
static inline uint64_t CAVM_BFN_SCRATCH_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_SCRATCH_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e04702fff0ll;
    __cavm_csr_fatal("BFN_SCRATCH", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_SCRATCH cavm_bfn_scratch_t
#define bustype_CAVM_BFN_SCRATCH CSR_TYPE_RSL
#define basename_CAVM_BFN_SCRATCH "BFN_SCRATCH"
#define busnum_CAVM_BFN_SCRATCH 0
#define arguments_CAVM_BFN_SCRATCH -1,-1,-1,-1

/**
 * Register (RSL) bfn_sync_cfg
 *
 * BFN Synchronization Configuration Register
 * This register configures the logic for timestamp synchronization.
 */
union cavm_bfn_sync_cfg
{
    uint64_t u;
    struct cavm_bfn_sync_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_11_63        : 53;
        uint64_t sync_trig             : 3;  /**< [ 10:  8](R/W) Selects the trigger for a synchronization event.
                                                                 0x0 = Match as configured by BFN_MATCH_CFG.
                                                                 0x1 = Start of frame.
                                                                 0x2 = Start of sub-frame.
                                                                 0x3 = Software (synchronization occurs immediately).
                                                                 0x4 = RFP
                                                                 0x5-0x7 = Reserved. */
        uint64_t reserved_2_7          : 6;
        uint64_t sync_ieee1914_en      : 1;  /**< [  1:  1](R/W/H) Enables synchronization of the IEEE 1914 counter at the trigger
                                                                 event selected by [SYNC_TRIG].  This bit resets to 0 after the
                                                                 synchronization occurs. */
        uint64_t sync_psm_en           : 1;  /**< [  0:  0](R/W/H) Enables synchronization of the PSM counter at the trigger event
                                                                 selected by [SYNC_TRIG].  This bit resets to 0 after the
                                                                 synchronization occurs. */
#else /* Word 0 - Little Endian */
        uint64_t sync_psm_en           : 1;  /**< [  0:  0](R/W/H) Enables synchronization of the PSM counter at the trigger event
                                                                 selected by [SYNC_TRIG].  This bit resets to 0 after the
                                                                 synchronization occurs. */
        uint64_t sync_ieee1914_en      : 1;  /**< [  1:  1](R/W/H) Enables synchronization of the IEEE 1914 counter at the trigger
                                                                 event selected by [SYNC_TRIG].  This bit resets to 0 after the
                                                                 synchronization occurs. */
        uint64_t reserved_2_7          : 6;
        uint64_t sync_trig             : 3;  /**< [ 10:  8](R/W) Selects the trigger for a synchronization event.
                                                                 0x0 = Match as configured by BFN_MATCH_CFG.
                                                                 0x1 = Start of frame.
                                                                 0x2 = Start of sub-frame.
                                                                 0x3 = Software (synchronization occurs immediately).
                                                                 0x4 = RFP
                                                                 0x5-0x7 = Reserved. */
        uint64_t reserved_11_63        : 53;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_sync_cfg_s cn; */
};
typedef union cavm_bfn_sync_cfg cavm_bfn_sync_cfg_t;

#define CAVM_BFN_SYNC_CFG CAVM_BFN_SYNC_CFG_FUNC()
static inline uint64_t CAVM_BFN_SYNC_CFG_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_SYNC_CFG_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047035000ll;
    __cavm_csr_fatal("BFN_SYNC_CFG", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_SYNC_CFG cavm_bfn_sync_cfg_t
#define bustype_CAVM_BFN_SYNC_CFG CSR_TYPE_RSL
#define basename_CAVM_BFN_SYNC_CFG "BFN_SYNC_CFG"
#define busnum_CAVM_BFN_SYNC_CFG 0
#define arguments_CAVM_BFN_SYNC_CFG -1,-1,-1,-1

/**
 * Register (RSL) bfn_sync_ieee1914
 *
 * BFN IEEE 1914 Synchronization Register
 * This register is used to configure the IEEE 1914 counter values that
 * will be used for the synchronization event.
 */
union cavm_bfn_sync_ieee1914
{
    uint64_t u;
    struct cavm_bfn_sync_ieee1914_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_29_63        : 35;
        uint64_t nsec                  : 24; /**< [ 28:  5](R/W) The IEEE 1914 timestamp integer nanosecond field to be used for synchronization. */
        uint64_t fnsec                 : 5;  /**< [  4:  0](R/W) The IEEE 1914 timestamp fractional nanosecond field to be used for synchronization. */
#else /* Word 0 - Little Endian */
        uint64_t fnsec                 : 5;  /**< [  4:  0](R/W) The IEEE 1914 timestamp fractional nanosecond field to be used for synchronization. */
        uint64_t nsec                  : 24; /**< [ 28:  5](R/W) The IEEE 1914 timestamp integer nanosecond field to be used for synchronization. */
        uint64_t reserved_29_63        : 35;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_sync_ieee1914_s cn; */
};
typedef union cavm_bfn_sync_ieee1914 cavm_bfn_sync_ieee1914_t;

#define CAVM_BFN_SYNC_IEEE1914 CAVM_BFN_SYNC_IEEE1914_FUNC()
static inline uint64_t CAVM_BFN_SYNC_IEEE1914_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_SYNC_IEEE1914_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047035200ll;
    __cavm_csr_fatal("BFN_SYNC_IEEE1914", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_SYNC_IEEE1914 cavm_bfn_sync_ieee1914_t
#define bustype_CAVM_BFN_SYNC_IEEE1914 CSR_TYPE_RSL
#define basename_CAVM_BFN_SYNC_IEEE1914 "BFN_SYNC_IEEE1914"
#define busnum_CAVM_BFN_SYNC_IEEE1914 0
#define arguments_CAVM_BFN_SYNC_IEEE1914 -1,-1,-1,-1

/**
 * Register (RSL) bfn_sync_psm
 *
 * BFN PSM Synchronization Register
 * This register is used to configure the PSM frame, subframe, and tick
 * values that will be used for the synchronization event.
 */
union cavm_bfn_sync_psm
{
    uint64_t u;
    struct cavm_bfn_sync_psm_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t frame                 : 12; /**< [ 31: 20](R/W) The PSM frame value to be used for synchronization. */
        uint64_t subframe              : 4;  /**< [ 19: 16](R/W) The PSM subframe value to be used for synchronization. */
        uint64_t tick                  : 16; /**< [ 15:  0](R/W) The PSM tick value to be used for synchronization. */
#else /* Word 0 - Little Endian */
        uint64_t tick                  : 16; /**< [ 15:  0](R/W) The PSM tick value to be used for synchronization. */
        uint64_t subframe              : 4;  /**< [ 19: 16](R/W) The PSM subframe value to be used for synchronization. */
        uint64_t frame                 : 12; /**< [ 31: 20](R/W) The PSM frame value to be used for synchronization. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_sync_psm_s cn; */
};
typedef union cavm_bfn_sync_psm cavm_bfn_sync_psm_t;

#define CAVM_BFN_SYNC_PSM CAVM_BFN_SYNC_PSM_FUNC()
static inline uint64_t CAVM_BFN_SYNC_PSM_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_SYNC_PSM_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047035100ll;
    __cavm_csr_fatal("BFN_SYNC_PSM", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_SYNC_PSM cavm_bfn_sync_psm_t
#define bustype_CAVM_BFN_SYNC_PSM CSR_TYPE_RSL
#define basename_CAVM_BFN_SYNC_PSM "BFN_SYNC_PSM"
#define busnum_CAVM_BFN_SYNC_PSM 0
#define arguments_CAVM_BFN_SYNC_PSM -1,-1,-1,-1

/**
 * Register (RSL) bfn_sync_psm_cnt
 *
 * BFN PSM Count Synchronization Register
 * This register is used to configure the PSM frame and subframe cycle
 * counter values that will be used for the synchronization event.
 */
union cavm_bfn_sync_psm_cnt
{
    uint64_t u;
    struct cavm_bfn_sync_psm_cnt_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_56_63        : 8;
        uint64_t frame_cnt             : 24; /**< [ 55: 32](R/W) This field sets the value of the PSM frame cycle counter that
                                                                 will be used for synchronization.  This counter is a running
                                                                 count of BTS_CLK cycles during a frame, and the sync value
                                                                 can be set to account for a synchronization that occurs
                                                                 in the middle of a frame. */
        uint64_t reserved_20_31        : 12;
        uint64_t sf_cnt                : 20; /**< [ 19:  0](R/W) This field sets the value of the PSM subframe cycle counter that
                                                                 will be used for synchronization.  This counter is a running
                                                                 count of BTS_CLK cycles during a subframe, and the sync value
                                                                 can be set to account for a synchronization that occurs
                                                                 in the middle of a subframe. */
#else /* Word 0 - Little Endian */
        uint64_t sf_cnt                : 20; /**< [ 19:  0](R/W) This field sets the value of the PSM subframe cycle counter that
                                                                 will be used for synchronization.  This counter is a running
                                                                 count of BTS_CLK cycles during a subframe, and the sync value
                                                                 can be set to account for a synchronization that occurs
                                                                 in the middle of a subframe. */
        uint64_t reserved_20_31        : 12;
        uint64_t frame_cnt             : 24; /**< [ 55: 32](R/W) This field sets the value of the PSM frame cycle counter that
                                                                 will be used for synchronization.  This counter is a running
                                                                 count of BTS_CLK cycles during a frame, and the sync value
                                                                 can be set to account for a synchronization that occurs
                                                                 in the middle of a frame. */
        uint64_t reserved_56_63        : 8;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_bfn_sync_psm_cnt_s cn; */
};
typedef union cavm_bfn_sync_psm_cnt cavm_bfn_sync_psm_cnt_t;

#define CAVM_BFN_SYNC_PSM_CNT CAVM_BFN_SYNC_PSM_CNT_FUNC()
static inline uint64_t CAVM_BFN_SYNC_PSM_CNT_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_BFN_SYNC_PSM_CNT_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_F95O))
        return 0x87e047035110ll;
    __cavm_csr_fatal("BFN_SYNC_PSM_CNT", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_BFN_SYNC_PSM_CNT cavm_bfn_sync_psm_cnt_t
#define bustype_CAVM_BFN_SYNC_PSM_CNT CSR_TYPE_RSL
#define basename_CAVM_BFN_SYNC_PSM_CNT "BFN_SYNC_PSM_CNT"
#define busnum_CAVM_BFN_SYNC_PSM_CNT 0
#define arguments_CAVM_BFN_SYNC_PSM_CNT -1,-1,-1,-1

#endif /* __CAVM_CSRS_BFN_H__ */
