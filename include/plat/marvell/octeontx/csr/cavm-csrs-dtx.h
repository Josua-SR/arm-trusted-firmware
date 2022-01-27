#ifndef __CAVM_CSRS_DTX_H__
#define __CAVM_CSRS_DTX_H__
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
 * OcteonTX DTX.
 *
 * This file is auto generated. Do not edit.
 *
 */

/**
 * Register (RSL) dtx_broadcast_ctl
 *
 * DTX BROADCAST Control Register
 */
union cavm_dtx_broadcast_ctl
{
    uint64_t u;
    struct cavm_dtx_broadcast_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_5_63         : 59;
        uint64_t active                : 1;  /**< [  4:  4](R/W) Force block's gated clocks on, so that the state of idle signals may be captured. */
        uint64_t reserved_2_3          : 2;
        uint64_t echoen                : 1;  /**< [  1:  1](R/W) Drive debug bus with the value in DTX_xxx_ENA(0..1) instead of normal block
                                                                 debug data. Not applicable when software directly reads the DTX_xxx_DAT(0..1)
                                                                 registers.  For diagnostic use only. */
        uint64_t swap                  : 1;  /**< [  0:  0](R/W) Swap the high and low 36-bit debug bus outputs. */
#else /* Word 0 - Little Endian */
        uint64_t swap                  : 1;  /**< [  0:  0](R/W) Swap the high and low 36-bit debug bus outputs. */
        uint64_t echoen                : 1;  /**< [  1:  1](R/W) Drive debug bus with the value in DTX_xxx_ENA(0..1) instead of normal block
                                                                 debug data. Not applicable when software directly reads the DTX_xxx_DAT(0..1)
                                                                 registers.  For diagnostic use only. */
        uint64_t reserved_2_3          : 2;
        uint64_t active                : 1;  /**< [  4:  4](R/W) Force block's gated clocks on, so that the state of idle signals may be captured. */
        uint64_t reserved_5_63         : 59;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dtx_broadcast_ctl_s cn; */
};
typedef union cavm_dtx_broadcast_ctl cavm_dtx_broadcast_ctl_t;

#define CAVM_DTX_BROADCAST_CTL CAVM_DTX_BROADCAST_CTL_FUNC()
static inline uint64_t CAVM_DTX_BROADCAST_CTL_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DTX_BROADCAST_CTL_FUNC(void)
{
    return 0x87e0fe7f0060ll;
}

#define typedef_CAVM_DTX_BROADCAST_CTL cavm_dtx_broadcast_ctl_t
#define bustype_CAVM_DTX_BROADCAST_CTL CSR_TYPE_RSL
#define basename_CAVM_DTX_BROADCAST_CTL "DTX_BROADCAST_CTL"
#define busnum_CAVM_DTX_BROADCAST_CTL 0
#define arguments_CAVM_DTX_BROADCAST_CTL -1,-1,-1,-1

/**
 * Register (RSL) dtx_broadcast_ena#
 *
 * DTX BROADCAST Data Enable Register
 */
union cavm_dtx_broadcast_enax
{
    uint64_t u;
    struct cavm_dtx_broadcast_enax_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t ena                   : 36; /**< [ 35:  0](R/W) Output enable vector of which bits to drive onto the low/high 36-bit debug
                                                                 buses. Normally only one block will drive each bit. */
#else /* Word 0 - Little Endian */
        uint64_t ena                   : 36; /**< [ 35:  0](R/W) Output enable vector of which bits to drive onto the low/high 36-bit debug
                                                                 buses. Normally only one block will drive each bit. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dtx_broadcast_enax_s cn; */
};
typedef union cavm_dtx_broadcast_enax cavm_dtx_broadcast_enax_t;

static inline uint64_t CAVM_DTX_BROADCAST_ENAX(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DTX_BROADCAST_ENAX(uint64_t a)
{
    if (a<=1)
        return 0x87e0fe7f0020ll + 8ll * ((a) & 0x1);
    __cavm_csr_fatal("DTX_BROADCAST_ENAX", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DTX_BROADCAST_ENAX(a) cavm_dtx_broadcast_enax_t
#define bustype_CAVM_DTX_BROADCAST_ENAX(a) CSR_TYPE_RSL
#define basename_CAVM_DTX_BROADCAST_ENAX(a) "DTX_BROADCAST_ENAX"
#define busnum_CAVM_DTX_BROADCAST_ENAX(a) (a)
#define arguments_CAVM_DTX_BROADCAST_ENAX(a) (a),-1,-1,-1

/**
 * Register (RSL) dtx_broadcast_sel#
 *
 * DTX BROADCAST Select Register
 */
union cavm_dtx_broadcast_selx
{
    uint64_t u;
    struct cavm_dtx_broadcast_selx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_24_63        : 40;
        uint64_t value                 : 24; /**< [ 23:  0](R/W) Debug select. Selects which signals to drive onto low/high 36-bit debug buses. */
#else /* Word 0 - Little Endian */
        uint64_t value                 : 24; /**< [ 23:  0](R/W) Debug select. Selects which signals to drive onto low/high 36-bit debug buses. */
        uint64_t reserved_24_63        : 40;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dtx_broadcast_selx_s cn; */
};
typedef union cavm_dtx_broadcast_selx cavm_dtx_broadcast_selx_t;

static inline uint64_t CAVM_DTX_BROADCAST_SELX(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DTX_BROADCAST_SELX(uint64_t a)
{
    if (a<=1)
        return 0x87e0fe7f0000ll + 8ll * ((a) & 0x1);
    __cavm_csr_fatal("DTX_BROADCAST_SELX", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DTX_BROADCAST_SELX(a) cavm_dtx_broadcast_selx_t
#define bustype_CAVM_DTX_BROADCAST_SELX(a) CSR_TYPE_RSL
#define basename_CAVM_DTX_BROADCAST_SELX(a) "DTX_BROADCAST_SELX"
#define busnum_CAVM_DTX_BROADCAST_SELX(a) (a)
#define arguments_CAVM_DTX_BROADCAST_SELX(a) (a),-1,-1,-1

/**
 * Register (RSL) dtx_lmc#_bcst_rsp
 *
 * INTERNAL: DTX LMC Control Register
 */
union cavm_dtx_lmcx_bcst_rsp
{
    uint64_t u;
    struct cavm_dtx_lmcx_bcst_rsp_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t ena                   : 1;  /**< [  0:  0](R/W) Enable this DTX instance as the responder to DTX broadcast read/write operations. */
#else /* Word 0 - Little Endian */
        uint64_t ena                   : 1;  /**< [  0:  0](R/W) Enable this DTX instance as the responder to DTX broadcast read/write operations. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dtx_lmcx_bcst_rsp_s cn; */
};
typedef union cavm_dtx_lmcx_bcst_rsp cavm_dtx_lmcx_bcst_rsp_t;

static inline uint64_t CAVM_DTX_LMCX_BCST_RSP(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DTX_LMCX_BCST_RSP(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e0fe440080ll + 0x8000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e0fe440080ll + 0x8000ll * ((a) & 0x1);
    if (cavm_is_model(OCTEONTX_CN96XX) && (a<=2))
        return 0x87e0fe440080ll + 0x8000ll * ((a) & 0x3);
    if (cavm_is_model(OCTEONTX_CN98XX) && (a<=5))
        return 0x87e0fe440080ll + 0x8000ll * ((a) & 0x7);
    if (cavm_is_model(OCTEONTX_CNF95XX) && (a<=2))
        return 0x87e0fe440080ll + 0x8000ll * ((a) & 0x3);
    if (cavm_is_model(OCTEONTX_F95MM) && (a<=2))
        return 0x87e0fe440080ll + 0x8000ll * ((a) & 0x3);
    if (cavm_is_model(OCTEONTX_F95O) && (a<=2))
        return 0x87e0fe440080ll + 0x8000ll * ((a) & 0x3);
    if (cavm_is_model(OCTEONTX_LOKI) && (a<=2))
        return 0x87e0fe440080ll + 0x8000ll * ((a) & 0x3);
    __cavm_csr_fatal("DTX_LMCX_BCST_RSP", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DTX_LMCX_BCST_RSP(a) cavm_dtx_lmcx_bcst_rsp_t
#define bustype_CAVM_DTX_LMCX_BCST_RSP(a) CSR_TYPE_RSL
#define basename_CAVM_DTX_LMCX_BCST_RSP(a) "DTX_LMCX_BCST_RSP"
#define busnum_CAVM_DTX_LMCX_BCST_RSP(a) (a)
#define arguments_CAVM_DTX_LMCX_BCST_RSP(a) (a),-1,-1,-1

/**
 * Register (RSL) dtx_pem#_dat#
 *
 * INTERNAL: DTX PEM Raw Data Register
 */
union cavm_dtx_pemx_datx
{
    uint64_t u;
    struct cavm_dtx_pemx_datx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t raw                   : 36; /**< [ 35:  0](RO/H) Raw debug data captured by the DTX before the ENA is applied. This gives the
                                                                 ability to peek into blocks during an OCLA capture without OCLA reconfiguration. */
#else /* Word 0 - Little Endian */
        uint64_t raw                   : 36; /**< [ 35:  0](RO/H) Raw debug data captured by the DTX before the ENA is applied. This gives the
                                                                 ability to peek into blocks during an OCLA capture without OCLA reconfiguration. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dtx_pemx_datx_s cn; */
};
typedef union cavm_dtx_pemx_datx cavm_dtx_pemx_datx_t;

static inline uint64_t CAVM_DTX_PEMX_DATX(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DTX_PEMX_DATX(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a<=2) && (b<=1)))
        return 0x87e0fe600040ll + 0x8000ll * ((a) & 0x3) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=3) && (b<=1)))
        return 0x87e0fe600040ll + 0x8000ll * ((a) & 0x3) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e0fef00040ll + 0x8000ll * ((a) & 0x3) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=4) && (b<=1)))
        return 0x87e0fef00040ll + 0x8000ll * ((a) & 0x7) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX_PASS1_X) && ((a==0) && (b<=1)))
        return 0x87e0fef00040ll + 0x8000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e0fef00040ll + 0x8000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e0fef00040ll + 0x8000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    __cavm_csr_fatal("DTX_PEMX_DATX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DTX_PEMX_DATX(a,b) cavm_dtx_pemx_datx_t
#define bustype_CAVM_DTX_PEMX_DATX(a,b) CSR_TYPE_RSL
#define basename_CAVM_DTX_PEMX_DATX(a,b) "DTX_PEMX_DATX"
#define busnum_CAVM_DTX_PEMX_DATX(a,b) (a)
#define arguments_CAVM_DTX_PEMX_DATX(a,b) (a),(b),-1,-1

/**
 * Register (RSL) dtx_pem#_ena#
 *
 * INTERNAL: DTX PEM Data Enable Register
 */
union cavm_dtx_pemx_enax
{
    uint64_t u;
    struct cavm_dtx_pemx_enax_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t ena                   : 36; /**< [ 35:  0](R/W) Output enable vector of which bits to drive onto the low/high 36-bit debug
                                                                 buses. Normally only one block will drive each bit. */
#else /* Word 0 - Little Endian */
        uint64_t ena                   : 36; /**< [ 35:  0](R/W) Output enable vector of which bits to drive onto the low/high 36-bit debug
                                                                 buses. Normally only one block will drive each bit. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dtx_pemx_enax_s cn; */
};
typedef union cavm_dtx_pemx_enax cavm_dtx_pemx_enax_t;

static inline uint64_t CAVM_DTX_PEMX_ENAX(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DTX_PEMX_ENAX(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a<=2) && (b<=1)))
        return 0x87e0fe600020ll + 0x8000ll * ((a) & 0x3) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=3) && (b<=1)))
        return 0x87e0fe600020ll + 0x8000ll * ((a) & 0x3) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e0fef00020ll + 0x8000ll * ((a) & 0x3) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=4) && (b<=1)))
        return 0x87e0fef00020ll + 0x8000ll * ((a) & 0x7) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX_PASS1_X) && ((a==0) && (b<=1)))
        return 0x87e0fef00020ll + 0x8000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e0fef00020ll + 0x8000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e0fef00020ll + 0x8000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    __cavm_csr_fatal("DTX_PEMX_ENAX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DTX_PEMX_ENAX(a,b) cavm_dtx_pemx_enax_t
#define bustype_CAVM_DTX_PEMX_ENAX(a,b) CSR_TYPE_RSL
#define basename_CAVM_DTX_PEMX_ENAX(a,b) "DTX_PEMX_ENAX"
#define busnum_CAVM_DTX_PEMX_ENAX(a,b) (a)
#define arguments_CAVM_DTX_PEMX_ENAX(a,b) (a),(b),-1,-1

/**
 * Register (RSL) dtx_pem#_sel#
 *
 * INTERNAL: DTX PEM Select Register
 */
union cavm_dtx_pemx_selx
{
    uint64_t u;
    struct cavm_dtx_pemx_selx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_24_63        : 40;
        uint64_t value                 : 24; /**< [ 23:  0](R/W) Debug select. Selects which signals to drive onto low/high 36-bit debug buses. */
#else /* Word 0 - Little Endian */
        uint64_t value                 : 24; /**< [ 23:  0](R/W) Debug select. Selects which signals to drive onto low/high 36-bit debug buses. */
        uint64_t reserved_24_63        : 40;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dtx_pemx_selx_s cn; */
};
typedef union cavm_dtx_pemx_selx cavm_dtx_pemx_selx_t;

static inline uint64_t CAVM_DTX_PEMX_SELX(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DTX_PEMX_SELX(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a<=2) && (b<=1)))
        return 0x87e0fe600000ll + 0x8000ll * ((a) & 0x3) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=3) && (b<=1)))
        return 0x87e0fe600000ll + 0x8000ll * ((a) & 0x3) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e0fef00000ll + 0x8000ll * ((a) & 0x3) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=4) && (b<=1)))
        return 0x87e0fef00000ll + 0x8000ll * ((a) & 0x7) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX_PASS1_X) && ((a==0) && (b<=1)))
        return 0x87e0fef00000ll + 0x8000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e0fef00000ll + 0x8000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e0fef00000ll + 0x8000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    __cavm_csr_fatal("DTX_PEMX_SELX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DTX_PEMX_SELX(a,b) cavm_dtx_pemx_selx_t
#define bustype_CAVM_DTX_PEMX_SELX(a,b) CSR_TYPE_RSL
#define basename_CAVM_DTX_PEMX_SELX(a,b) "DTX_PEMX_SELX"
#define busnum_CAVM_DTX_PEMX_SELX(a,b) (a)
#define arguments_CAVM_DTX_PEMX_SELX(a,b) (a),(b),-1,-1

#endif /* __CAVM_CSRS_DTX_H__ */
