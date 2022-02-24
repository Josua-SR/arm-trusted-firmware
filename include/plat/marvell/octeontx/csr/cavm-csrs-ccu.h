#ifndef __CAVM_CSRS_CCU_H__
#define __CAVM_CSRS_CCU_H__
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
 * OcteonTX CCU.
 *
 * This file is auto generated. Do not edit.
 *
 */

/**
 * Enumeration ccu_bar_e
 *
 * CCU Base Address Register Enumeration
 * Enumerates the base address registers.
 */
#define CAVM_CCU_BAR_E_CCUX_PF_BAR0(a) (0x87e050000000ll + 0x1000000ll * (a))
#define CAVM_CCU_BAR_E_CCUX_PF_BAR0_SIZE 0x800000ull
#define CAVM_CCU_BAR_E_CCUX_PF_BAR4(a) (0x87e050f00000ll + 0x1000000ll * (a))
#define CAVM_CCU_BAR_E_CCUX_PF_BAR4_SIZE 0x100000ull

/**
 * Enumeration ccu_int_vec_e
 *
 * CCU MSI-X Vector Enumeration
 * Enumerates the MSI-X interrupt vectors.
 */
#define CAVM_CCU_INT_VEC_E_TADX_INT(a) (0 + (a))

/**
 * Enumeration ccu_pic_prf_sel_e
 *
 * CCU PIC Performance Counter Select Enumeration
 * Enumerates the different PIC performance counter selects.
 */
#define CAVM_CCU_PIC_PRF_SEL_E_ACK_IN (0xc)
#define CAVM_CCU_PIC_PRF_SEL_E_ACK_OUT (0x10)
#define CAVM_CCU_PIC_PRF_SEL_E_CMD_IN (0xd)
#define CAVM_CCU_PIC_PRF_SEL_E_CMD_OUT (0x11)
#define CAVM_CCU_PIC_PRF_SEL_E_CMT (0xa)
#define CAVM_CCU_PIC_PRF_SEL_E_DAT_IN (0xe)
#define CAVM_CCU_PIC_PRF_SEL_E_DAT_OUT (0x12)
#define CAVM_CCU_PIC_PRF_SEL_E_HIT (3)
#define CAVM_CCU_PIC_PRF_SEL_E_HIT_DLYD (4)
#define CAVM_CCU_PIC_PRF_SEL_E_INV (0xb)
#define CAVM_CCU_PIC_PRF_SEL_E_LFB_OCC (5)
#define CAVM_CCU_PIC_PRF_SEL_E_NONE (0)
#define CAVM_CCU_PIC_PRF_SEL_E_REPLAY (2)
#define CAVM_CCU_PIC_PRF_SEL_E_RSC0 (6)
#define CAVM_CCU_PIC_PRF_SEL_E_RSC1 (7)
#define CAVM_CCU_PIC_PRF_SEL_E_RSD0 (8)
#define CAVM_CCU_PIC_PRF_SEL_E_RSD1 (9)
#define CAVM_CCU_PIC_PRF_SEL_E_TAG_RD (1)
#define CAVM_CCU_PIC_PRF_SEL_E_VCC_IN (0xf)

/**
 * Enumeration ccu_tad_prf_sel_e
 *
 * CCU TAD Performance Counter Select Enumeration
 * Enumerates the different TAD performance counter selects.
 */
#define CAVM_CCU_TAD_PRF_SEL_E_ACK_MSH_IN (3)
#define CAVM_CCU_TAD_PRF_SEL_E_ACK_MSH_OUT (0x14)
#define CAVM_CCU_TAD_PRF_SEL_E_CMD_MSH_IN (2)
#define CAVM_CCU_TAD_PRF_SEL_E_CMD_MSH_OUT_ANY (0x13)
#define CAVM_CCU_TAD_PRF_SEL_E_CMD_MSH_OUT_BC (0xc)
#define CAVM_CCU_TAD_PRF_SEL_E_CMD_MSH_OUT_DINV (0xb)
#define CAVM_CCU_TAD_PRF_SEL_E_CMD_MSH_OUT_MCC_RD (0xd)
#define CAVM_CCU_TAD_PRF_SEL_E_CMD_MSH_OUT_MCC_WR (0xe)
#define CAVM_CCU_TAD_PRF_SEL_E_CMD_MSH_OUT_RSVD0 (0x10)
#define CAVM_CCU_TAD_PRF_SEL_E_CMD_MSH_OUT_RSVD1 (0x11)
#define CAVM_CCU_TAD_PRF_SEL_E_CMD_MSH_OUT_RSVD2 (0x12)
#define CAVM_CCU_TAD_PRF_SEL_E_CMD_MSH_OUT_WBAR (0xf)
#define CAVM_CCU_TAD_PRF_SEL_E_DAT_MSH_IN (1)
#define CAVM_CCU_TAD_PRF_SEL_E_DAT_MSH_OUT_ANY (0xa)
#define CAVM_CCU_TAD_PRF_SEL_E_DAT_MSH_OUT_BYP (6)
#define CAVM_CCU_TAD_PRF_SEL_E_DAT_MSH_OUT_FILL (5)
#define CAVM_CCU_TAD_PRF_SEL_E_DAT_MSH_OUT_MCC (7)
#define CAVM_CCU_TAD_PRF_SEL_E_DAT_MSH_OUT_RSVD0 (8)
#define CAVM_CCU_TAD_PRF_SEL_E_DAT_MSH_OUT_RSVD1 (9)
#define CAVM_CCU_TAD_PRF_SEL_E_DAT_RD (0x19)
#define CAVM_CCU_TAD_PRF_SEL_E_DAT_RD_HIT (0x1a)
#define CAVM_CCU_TAD_PRF_SEL_E_DTG_HIT (0x1c)
#define CAVM_CCU_TAD_PRF_SEL_E_LFB_OCC (0x1e)
#define CAVM_CCU_TAD_PRF_SEL_E_LTG_HIT (0x1b)
#define CAVM_CCU_TAD_PRF_SEL_E_NONE (0)
#define CAVM_CCU_TAD_PRF_SEL_E_TAG_RD (0x1d)
#define CAVM_CCU_TAD_PRF_SEL_E_VCC_MSH_IN (4)
#define CAVM_CCU_TAD_PRF_SEL_E_VCC_MSH_OUT_ANY (0x18)
#define CAVM_CCU_TAD_PRF_SEL_E_VCC_MSH_OUT_IOB (0x16)
#define CAVM_CCU_TAD_PRF_SEL_E_VCC_MSH_OUT_PIC (0x15)
#define CAVM_CCU_TAD_PRF_SEL_E_VCC_MSH_OUT_RSVD0 (0x17)

/**
 * Register (RSL) ccu#_msix_pba#
 *
 * CCU MSI-X Pending Bit Array Registers
 * This register is the MSI-X PBA table; the bit number is indexed by the CCU_INT_VEC_E
 * enumeration.
 */
union cavm_ccux_msix_pbax
{
    uint64_t u;
    struct cavm_ccux_msix_pbax_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO) Pending message for the associated CCU()_MSIX_VEC()_CTL, enumerated by
                                                                 CCU_INT_VEC_E. Bits
                                                                 that have no associated CCU_INT_VEC_E are 0. */
#else /* Word 0 - Little Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO) Pending message for the associated CCU()_MSIX_VEC()_CTL, enumerated by
                                                                 CCU_INT_VEC_E. Bits
                                                                 that have no associated CCU_INT_VEC_E are 0. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_msix_pbax_s cn; */
};
typedef union cavm_ccux_msix_pbax cavm_ccux_msix_pbax_t;

static inline uint64_t CAVM_CCUX_MSIX_PBAX(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_MSIX_PBAX(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b==0)))
        return 0x87e050ff0000ll + 0x1000000ll * ((a) & 0x3) + 8ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b==0)))
        return 0x87e050ff0000ll + 0x1000000ll * ((a) & 0x7) + 8ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b==0)))
        return 0x87e050ff0000ll + 0x1000000ll * ((a) & 0x0) + 8ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b==0)))
        return 0x87e050ff0000ll + 0x1000000ll * ((a) & 0x0) + 8ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b==0)))
        return 0x87e050ff0000ll + 0x1000000ll * ((a) & 0x0) + 8ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b==0)))
        return 0x87e050ff0000ll + 0x1000000ll * ((a) & 0x0) + 8ll * ((b) & 0x0);
    __cavm_csr_fatal("CCUX_MSIX_PBAX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_MSIX_PBAX(a,b) cavm_ccux_msix_pbax_t
#define bustype_CAVM_CCUX_MSIX_PBAX(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_MSIX_PBAX(a,b) "CCUX_MSIX_PBAX"
#define device_bar_CAVM_CCUX_MSIX_PBAX(a,b) 0x4 /* PF_BAR4 */
#define busnum_CAVM_CCUX_MSIX_PBAX(a,b) (a)
#define arguments_CAVM_CCUX_MSIX_PBAX(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_msix_vec#_addr
 *
 * CCU MSI-X Vector-Table Address Register
 * This register is the MSI-X vector table, indexed by the CCU_INT_VEC_E enumeration.
 */
union cavm_ccux_msix_vecx_addr
{
    uint64_t u;
    struct cavm_ccux_msix_vecx_addr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_53_63        : 11;
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_1            : 1;
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 1 = This vector's CCU()_MSIX_VEC()_ADDR, CCU()_MSIX_VEC()_CTL, and corresponding
                                                                 bit of CCU()_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.

                                                                 If PCCPF_CCU_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is
                                                                 set, all vectors are secure and function as if [SECVEC] were set. */
#else /* Word 0 - Little Endian */
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 1 = This vector's CCU()_MSIX_VEC()_ADDR, CCU()_MSIX_VEC()_CTL, and corresponding
                                                                 bit of CCU()_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.

                                                                 If PCCPF_CCU_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is
                                                                 set, all vectors are secure and function as if [SECVEC] were set. */
        uint64_t reserved_1            : 1;
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_53_63        : 11;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_msix_vecx_addr_s cn9; */
    /* struct cavm_ccux_msix_vecx_addr_s cn96xxp1; */
    struct cavm_ccux_msix_vecx_addr_cn96xxp3
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_53_63        : 11;
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_1            : 1;
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 The vector's IOVA is sent to the SMMU as nonsecure (though this only affects
                                                                 physical addresses if PCCPF_XXX_VSEC_SCTL[MSIX_PHYS]=1).

                                                                 1 = This vector's CCU()_MSIX_VEC()_ADDR, CCU()_MSIX_VEC()_CTL, and corresponding
                                                                 bit of CCU()_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.
                                                                 The vector's IOVA is sent to the SMMU as secure (though this only affects
                                                                 physical addresses if PCCPF_XXX_VSEC_SCTL[MSIX_PHYS]=1 or
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC_PHYS]=1).

                                                                 If PCCPF_CCU_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is
                                                                 set, all vectors are secure and function as if [SECVEC] were set.

                                                                 Also note the following:
                                                                 * When PCCPF_XXX_VSEC_SCTL[MSIX_SEC_EN]=1, all secure vectors (including secure
                                                                 VF vectors) will act as if PCCPF/PCCVF_XXX_MSIX_CAP_HDR[MSIXEN]=1,
                                                                 PCCPF/PCCVF_XXX_MSIX_CAP_HDR[FUNM]=0 and PCCPF/PCCVF_XXX_CMD[ME]=1.
                                                                 * When PCCPF_XXX_VSEC_SCTL[MSIX_SEC_PHYS]=1, all secure vectors (including
                                                                 secure VF vectors) are considered physical, regardless of
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_PHYS]. */
#else /* Word 0 - Little Endian */
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 The vector's IOVA is sent to the SMMU as nonsecure (though this only affects
                                                                 physical addresses if PCCPF_XXX_VSEC_SCTL[MSIX_PHYS]=1).

                                                                 1 = This vector's CCU()_MSIX_VEC()_ADDR, CCU()_MSIX_VEC()_CTL, and corresponding
                                                                 bit of CCU()_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.
                                                                 The vector's IOVA is sent to the SMMU as secure (though this only affects
                                                                 physical addresses if PCCPF_XXX_VSEC_SCTL[MSIX_PHYS]=1 or
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC_PHYS]=1).

                                                                 If PCCPF_CCU_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is
                                                                 set, all vectors are secure and function as if [SECVEC] were set.

                                                                 Also note the following:
                                                                 * When PCCPF_XXX_VSEC_SCTL[MSIX_SEC_EN]=1, all secure vectors (including secure
                                                                 VF vectors) will act as if PCCPF/PCCVF_XXX_MSIX_CAP_HDR[MSIXEN]=1,
                                                                 PCCPF/PCCVF_XXX_MSIX_CAP_HDR[FUNM]=0 and PCCPF/PCCVF_XXX_CMD[ME]=1.
                                                                 * When PCCPF_XXX_VSEC_SCTL[MSIX_SEC_PHYS]=1, all secure vectors (including
                                                                 secure VF vectors) are considered physical, regardless of
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_PHYS]. */
        uint64_t reserved_1            : 1;
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_53_63        : 11;
#endif /* Word 0 - End */
    } cn96xxp3;
    /* struct cavm_ccux_msix_vecx_addr_cn96xxp3 cn96xxp4; */
    /* struct cavm_ccux_msix_vecx_addr_cn96xxp3 cn98xx; */
    /* struct cavm_ccux_msix_vecx_addr_cn96xxp3 cnf95xx; */
    /* struct cavm_ccux_msix_vecx_addr_cn96xxp3 f95mm; */
    /* struct cavm_ccux_msix_vecx_addr_cn96xxp3 f95o; */
    /* struct cavm_ccux_msix_vecx_addr_cn96xxp3 loki; */
};
typedef union cavm_ccux_msix_vecx_addr cavm_ccux_msix_vecx_addr_t;

static inline uint64_t CAVM_CCUX_MSIX_VECX_ADDR(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_MSIX_VECX_ADDR(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050f00000ll + 0x1000000ll * ((a) & 0x3) + 0x10ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050f00000ll + 0x1000000ll * ((a) & 0x7) + 0x10ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050f00000ll + 0x1000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050f00000ll + 0x1000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050f00000ll + 0x1000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050f00000ll + 0x1000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_MSIX_VECX_ADDR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_MSIX_VECX_ADDR(a,b) cavm_ccux_msix_vecx_addr_t
#define bustype_CAVM_CCUX_MSIX_VECX_ADDR(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_MSIX_VECX_ADDR(a,b) "CCUX_MSIX_VECX_ADDR"
#define device_bar_CAVM_CCUX_MSIX_VECX_ADDR(a,b) 0x4 /* PF_BAR4 */
#define busnum_CAVM_CCUX_MSIX_VECX_ADDR(a,b) (a)
#define arguments_CAVM_CCUX_MSIX_VECX_ADDR(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_msix_vec#_ctl
 *
 * CCU MSI-X Vector-Table Control and Data Register
 * This register is the MSI-X vector table, indexed by the CCU_INT_VEC_E enumeration.
 */
union cavm_ccux_msix_vecx_ctl
{
    uint64_t u;
    struct cavm_ccux_msix_vecx_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_33_63        : 31;
        uint64_t mask                  : 1;  /**< [ 32: 32](R/W) When set, no MSI-X interrupts are sent to this vector. */
        uint64_t data                  : 32; /**< [ 31:  0](R/W) Data to use for MSI-X delivery of this vector. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 32; /**< [ 31:  0](R/W) Data to use for MSI-X delivery of this vector. */
        uint64_t mask                  : 1;  /**< [ 32: 32](R/W) When set, no MSI-X interrupts are sent to this vector. */
        uint64_t reserved_33_63        : 31;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_msix_vecx_ctl_s cn; */
};
typedef union cavm_ccux_msix_vecx_ctl cavm_ccux_msix_vecx_ctl_t;

static inline uint64_t CAVM_CCUX_MSIX_VECX_CTL(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_MSIX_VECX_CTL(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050f00008ll + 0x1000000ll * ((a) & 0x3) + 0x10ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050f00008ll + 0x1000000ll * ((a) & 0x7) + 0x10ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050f00008ll + 0x1000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050f00008ll + 0x1000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050f00008ll + 0x1000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050f00008ll + 0x1000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_MSIX_VECX_CTL", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_MSIX_VECX_CTL(a,b) cavm_ccux_msix_vecx_ctl_t
#define bustype_CAVM_CCUX_MSIX_VECX_CTL(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_MSIX_VECX_CTL(a,b) "CCUX_MSIX_VECX_CTL"
#define device_bar_CAVM_CCUX_MSIX_VECX_CTL(a,b) 0x4 /* PF_BAR4 */
#define busnum_CAVM_CCUX_MSIX_VECX_CTL(a,b) (a)
#define arguments_CAVM_CCUX_MSIX_VECX_CTL(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_msw_crclk_force
 *
 * MSW Conditional Clock Force Register
 */
union cavm_ccux_msw_crclk_force
{
    uint64_t u;
    struct cavm_ccux_msw_crclk_force_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_14_63        : 50;
        uint64_t trb1                  : 1;  /**< [ 13: 13](R/W) Force on TRB1 conditional clock. */
        uint64_t trb0                  : 1;  /**< [ 12: 12](R/W) Force on TRB0 conditional clock. */
        uint64_t prb                   : 1;  /**< [ 11: 11](R/W) Force on PRB conditional clock. */
        uint64_t tsb1                  : 1;  /**< [ 10: 10](R/W) Force on TSB1 conditional clock. */
        uint64_t tsb0                  : 1;  /**< [  9:  9](R/W) Force on TSB0 conditional clock. */
        uint64_t psb                   : 1;  /**< [  8:  8](R/W) Force on PSB conditional clock. */
        uint64_t mtx3                  : 1;  /**< [  7:  7](R/W) Force on MTX3 conditional clock. */
        uint64_t mtx2                  : 1;  /**< [  6:  6](R/W) Force on MTX2 conditional clock. */
        uint64_t mtx1                  : 1;  /**< [  5:  5](R/W) Force on MTX1 conditional clock. */
        uint64_t mtx0                  : 1;  /**< [  4:  4](R/W) Force on MTX0 conditional clock. */
        uint64_t mrx3                  : 1;  /**< [  3:  3](R/W) Force on MRX3 conditional clock. */
        uint64_t mrx2                  : 1;  /**< [  2:  2](R/W) Force on MRX2 conditional clock. */
        uint64_t mrx1                  : 1;  /**< [  1:  1](R/W) Force on MRX1 conditional clock. */
        uint64_t mrx0                  : 1;  /**< [  0:  0](R/W) Force on MRX0 conditional clock. */
#else /* Word 0 - Little Endian */
        uint64_t mrx0                  : 1;  /**< [  0:  0](R/W) Force on MRX0 conditional clock. */
        uint64_t mrx1                  : 1;  /**< [  1:  1](R/W) Force on MRX1 conditional clock. */
        uint64_t mrx2                  : 1;  /**< [  2:  2](R/W) Force on MRX2 conditional clock. */
        uint64_t mrx3                  : 1;  /**< [  3:  3](R/W) Force on MRX3 conditional clock. */
        uint64_t mtx0                  : 1;  /**< [  4:  4](R/W) Force on MTX0 conditional clock. */
        uint64_t mtx1                  : 1;  /**< [  5:  5](R/W) Force on MTX1 conditional clock. */
        uint64_t mtx2                  : 1;  /**< [  6:  6](R/W) Force on MTX2 conditional clock. */
        uint64_t mtx3                  : 1;  /**< [  7:  7](R/W) Force on MTX3 conditional clock. */
        uint64_t psb                   : 1;  /**< [  8:  8](R/W) Force on PSB conditional clock. */
        uint64_t tsb0                  : 1;  /**< [  9:  9](R/W) Force on TSB0 conditional clock. */
        uint64_t tsb1                  : 1;  /**< [ 10: 10](R/W) Force on TSB1 conditional clock. */
        uint64_t prb                   : 1;  /**< [ 11: 11](R/W) Force on PRB conditional clock. */
        uint64_t trb0                  : 1;  /**< [ 12: 12](R/W) Force on TRB0 conditional clock. */
        uint64_t trb1                  : 1;  /**< [ 13: 13](R/W) Force on TRB1 conditional clock. */
        uint64_t reserved_14_63        : 50;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_msw_crclk_force_s cn9; */
    /* struct cavm_ccux_msw_crclk_force_s cn96xx; */
    /* struct cavm_ccux_msw_crclk_force_s cn98xx; */
    struct cavm_ccux_msw_crclk_force_cnf95xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_14_63        : 50;
        uint64_t trb1                  : 1;  /**< [ 13: 13](R/W) Force on TRB1 conditional clock. For diagnostic use only. */
        uint64_t trb0                  : 1;  /**< [ 12: 12](R/W) Force on TRB0 conditional clock. For diagnostic use only. */
        uint64_t prb                   : 1;  /**< [ 11: 11](R/W) Force on PRB conditional clock. For diagnostic use only. */
        uint64_t tsb1                  : 1;  /**< [ 10: 10](R/W) Force on TSB1 conditional clock. For diagnostic use only. */
        uint64_t tsb0                  : 1;  /**< [  9:  9](R/W) Force on TSB0 conditional clock. For diagnostic use only. */
        uint64_t psb                   : 1;  /**< [  8:  8](R/W) Force on PSB conditional clock. For diagnostic use only. */
        uint64_t mtx3                  : 1;  /**< [  7:  7](R/W) Force on MTX3 conditional clock. For diagnostic use only. */
        uint64_t mtx2                  : 1;  /**< [  6:  6](R/W) Force on MTX2 conditional clock. For diagnostic use only. */
        uint64_t mtx1                  : 1;  /**< [  5:  5](R/W) Force on MTX1 conditional clock. For diagnostic use only. */
        uint64_t mtx0                  : 1;  /**< [  4:  4](R/W) Force on MTX0 conditional clock. For diagnostic use only. */
        uint64_t mrx3                  : 1;  /**< [  3:  3](R/W) Force on MRX3 conditional clock. For diagnostic use only. */
        uint64_t mrx2                  : 1;  /**< [  2:  2](R/W) Force on MRX2 conditional clock. For diagnostic use only. */
        uint64_t mrx1                  : 1;  /**< [  1:  1](R/W) Force on MRX1 conditional clock. For diagnostic use only. */
        uint64_t mrx0                  : 1;  /**< [  0:  0](R/W) Force on MRX0 conditional clock. For diagnostic use only. */
#else /* Word 0 - Little Endian */
        uint64_t mrx0                  : 1;  /**< [  0:  0](R/W) Force on MRX0 conditional clock. For diagnostic use only. */
        uint64_t mrx1                  : 1;  /**< [  1:  1](R/W) Force on MRX1 conditional clock. For diagnostic use only. */
        uint64_t mrx2                  : 1;  /**< [  2:  2](R/W) Force on MRX2 conditional clock. For diagnostic use only. */
        uint64_t mrx3                  : 1;  /**< [  3:  3](R/W) Force on MRX3 conditional clock. For diagnostic use only. */
        uint64_t mtx0                  : 1;  /**< [  4:  4](R/W) Force on MTX0 conditional clock. For diagnostic use only. */
        uint64_t mtx1                  : 1;  /**< [  5:  5](R/W) Force on MTX1 conditional clock. For diagnostic use only. */
        uint64_t mtx2                  : 1;  /**< [  6:  6](R/W) Force on MTX2 conditional clock. For diagnostic use only. */
        uint64_t mtx3                  : 1;  /**< [  7:  7](R/W) Force on MTX3 conditional clock. For diagnostic use only. */
        uint64_t psb                   : 1;  /**< [  8:  8](R/W) Force on PSB conditional clock. For diagnostic use only. */
        uint64_t tsb0                  : 1;  /**< [  9:  9](R/W) Force on TSB0 conditional clock. For diagnostic use only. */
        uint64_t tsb1                  : 1;  /**< [ 10: 10](R/W) Force on TSB1 conditional clock. For diagnostic use only. */
        uint64_t prb                   : 1;  /**< [ 11: 11](R/W) Force on PRB conditional clock. For diagnostic use only. */
        uint64_t trb0                  : 1;  /**< [ 12: 12](R/W) Force on TRB0 conditional clock. For diagnostic use only. */
        uint64_t trb1                  : 1;  /**< [ 13: 13](R/W) Force on TRB1 conditional clock. For diagnostic use only. */
        uint64_t reserved_14_63        : 50;
#endif /* Word 0 - End */
    } cnf95xx;
    /* struct cavm_ccux_msw_crclk_force_cnf95xx f95mm; */
    /* struct cavm_ccux_msw_crclk_force_cnf95xx f95o; */
    /* struct cavm_ccux_msw_crclk_force_cnf95xx loki; */
};
typedef union cavm_ccux_msw_crclk_force cavm_ccux_msw_crclk_force_t;

static inline uint64_t CAVM_CCUX_MSW_CRCLK_FORCE(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_MSW_CRCLK_FORCE(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN96XX_PASS3_X) && (a<=3))
        return 0x87e050200020ll + 0x1000000ll * ((a) & 0x3);
    if (cavm_is_model(OCTEONTX_CN96XX_PASS4_X) && (a<=3))
        return 0x87e050200020ll + 0x1000000ll * ((a) & 0x3);
    if (cavm_is_model(OCTEONTX_CN98XX) && (a<=5))
        return 0x87e050200020ll + 0x1000000ll * ((a) & 0x7);
    if (cavm_is_model(OCTEONTX_CNF95XX) && (a==0))
        return 0x87e050200020ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_F95MM) && (a==0))
        return 0x87e050200020ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_F95O) && (a==0))
        return 0x87e050200020ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_LOKI) && (a==0))
        return 0x87e050200020ll + 0x1000000ll * ((a) & 0x0);
    __cavm_csr_fatal("CCUX_MSW_CRCLK_FORCE", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_MSW_CRCLK_FORCE(a) cavm_ccux_msw_crclk_force_t
#define bustype_CAVM_CCUX_MSW_CRCLK_FORCE(a) CSR_TYPE_RSL
#define basename_CAVM_CCUX_MSW_CRCLK_FORCE(a) "CCUX_MSW_CRCLK_FORCE"
#define device_bar_CAVM_CCUX_MSW_CRCLK_FORCE(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_MSW_CRCLK_FORCE(a) (a)
#define arguments_CAVM_CCUX_MSW_CRCLK_FORCE(a) (a),-1,-1,-1

/**
 * Register (RSL) ccu#_msw_msh#_pfc#
 *
 * CCU MSW Performance Counter Registers
 */
union cavm_ccux_msw_mshx_pfcx
{
    uint64_t u;
    struct cavm_ccux_msw_mshx_pfcx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_msw_mshx_pfcx_s cn; */
};
typedef union cavm_ccux_msw_mshx_pfcx cavm_ccux_msw_mshx_pfcx_t;

static inline uint64_t CAVM_CCUX_MSW_MSHX_PFCX(uint64_t a, uint64_t b, uint64_t c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_MSW_MSHX_PFCX(uint64_t a, uint64_t b, uint64_t c)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=3) && (c<=3)))
        return 0x87e050200100ll + 0x1000000ll * ((a) & 0x3) + 0x40000ll * ((b) & 0x3) + 8ll * ((c) & 0x3);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=3) && (c<=3)))
        return 0x87e050200100ll + 0x1000000ll * ((a) & 0x7) + 0x40000ll * ((b) & 0x3) + 8ll * ((c) & 0x3);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=3) && (c<=3)))
        return 0x87e050200100ll + 0x1000000ll * ((a) & 0x0) + 0x40000ll * ((b) & 0x3) + 8ll * ((c) & 0x3);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=3) && (c<=3)))
        return 0x87e050200100ll + 0x1000000ll * ((a) & 0x0) + 0x40000ll * ((b) & 0x3) + 8ll * ((c) & 0x3);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=3) && (c<=3)))
        return 0x87e050200100ll + 0x1000000ll * ((a) & 0x0) + 0x40000ll * ((b) & 0x3) + 8ll * ((c) & 0x3);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=3) && (c<=3)))
        return 0x87e050200100ll + 0x1000000ll * ((a) & 0x0) + 0x40000ll * ((b) & 0x3) + 8ll * ((c) & 0x3);
    __cavm_csr_fatal("CCUX_MSW_MSHX_PFCX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_CCUX_MSW_MSHX_PFCX(a,b,c) cavm_ccux_msw_mshx_pfcx_t
#define bustype_CAVM_CCUX_MSW_MSHX_PFCX(a,b,c) CSR_TYPE_RSL
#define basename_CAVM_CCUX_MSW_MSHX_PFCX(a,b,c) "CCUX_MSW_MSHX_PFCX"
#define device_bar_CAVM_CCUX_MSW_MSHX_PFCX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_MSW_MSHX_PFCX(a,b,c) (a)
#define arguments_CAVM_CCUX_MSW_MSHX_PFCX(a,b,c) (a),(b),(c),-1

/**
 * Register (RSL) ccu#_msw_msh#_prf
 *
 * CCU MSW Performance Counter Control Registers
 * This register controls measurement of the number of active cycles on each sub-mesh
 * out from MSW in one cardinal direction. Note a full cache line on the dat mesh
 * is counted as two active cycles.
 * _ MSH(0) = MSW to Mesh North.
 * _ MSH(1) = MSW to Mesh East.
 * _ MSH(2) = MSW to Mesh South.
 * _ MSH(3) = MSW to Mesh West.
 */
union cavm_ccux_msw_mshx_prf
{
    uint64_t u;
    struct cavm_ccux_msw_mshx_prf_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_4_63         : 60;
        uint64_t vcc_en                : 1;  /**< [  3:  3](R/W) Enable counting the number of active cycles out of MSW on the VCC submesh in CCU()_MSW_MSH()_PFC(3) */
        uint64_t dat_en                : 1;  /**< [  2:  2](R/W) Enable counting the number of active cycles out of MSW on the DAT submesh in CCU()_MSW_MSH()_PFC(2) */
        uint64_t cmd_en                : 1;  /**< [  1:  1](R/W) Enable counting the number of active cycles out of MSW on the CMD submesh in CCU()_MSW_MSH()_PFC(1) */
        uint64_t ack_en                : 1;  /**< [  0:  0](R/W) Enable counting the number of active cycles out of MSW on the ACK submesh in CCU()_MSW_MSH()_PFC(0) */
#else /* Word 0 - Little Endian */
        uint64_t ack_en                : 1;  /**< [  0:  0](R/W) Enable counting the number of active cycles out of MSW on the ACK submesh in CCU()_MSW_MSH()_PFC(0) */
        uint64_t cmd_en                : 1;  /**< [  1:  1](R/W) Enable counting the number of active cycles out of MSW on the CMD submesh in CCU()_MSW_MSH()_PFC(1) */
        uint64_t dat_en                : 1;  /**< [  2:  2](R/W) Enable counting the number of active cycles out of MSW on the DAT submesh in CCU()_MSW_MSH()_PFC(2) */
        uint64_t vcc_en                : 1;  /**< [  3:  3](R/W) Enable counting the number of active cycles out of MSW on the VCC submesh in CCU()_MSW_MSH()_PFC(3) */
        uint64_t reserved_4_63         : 60;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_msw_mshx_prf_s cn; */
};
typedef union cavm_ccux_msw_mshx_prf cavm_ccux_msw_mshx_prf_t;

static inline uint64_t CAVM_CCUX_MSW_MSHX_PRF(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_MSW_MSHX_PRF(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=3)))
        return 0x87e050200018ll + 0x1000000ll * ((a) & 0x3) + 0x40000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=3)))
        return 0x87e050200018ll + 0x1000000ll * ((a) & 0x7) + 0x40000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=3)))
        return 0x87e050200018ll + 0x1000000ll * ((a) & 0x0) + 0x40000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=3)))
        return 0x87e050200018ll + 0x1000000ll * ((a) & 0x0) + 0x40000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=3)))
        return 0x87e050200018ll + 0x1000000ll * ((a) & 0x0) + 0x40000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=3)))
        return 0x87e050200018ll + 0x1000000ll * ((a) & 0x0) + 0x40000ll * ((b) & 0x3);
    __cavm_csr_fatal("CCUX_MSW_MSHX_PRF", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_MSW_MSHX_PRF(a,b) cavm_ccux_msw_mshx_prf_t
#define bustype_CAVM_CCUX_MSW_MSHX_PRF(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_MSW_MSHX_PRF(a,b) "CCUX_MSW_MSHX_PRF"
#define device_bar_CAVM_CCUX_MSW_MSHX_PRF(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_MSW_MSHX_PRF(a,b) (a)
#define arguments_CAVM_CCUX_MSW_MSHX_PRF(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_pic_bist_status
 *
 * CCU PIC BIST Status Register
 */
union cavm_ccux_pic_bist_status
{
    uint64_t u;
    struct cavm_ccux_pic_bist_status_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_10_63        : 54;
        uint64_t mlcd1fl               : 4;  /**< [  9:  6](RO/H) BIST failure status for MLC data arrays for ways 5-9. */
        uint64_t mlcd0fl               : 4;  /**< [  5:  2](RO/H) BIST failure status for MLC data arrays for ways 0-4. */
        uint64_t mlct1fl               : 1;  /**< [  1:  1](RO/H) BIST failure status for MLC tag arrays for ways 5-9. */
        uint64_t mlct0fl               : 1;  /**< [  0:  0](RO/H) BIST failure status for MLC tag arrays for ways 0-4. */
#else /* Word 0 - Little Endian */
        uint64_t mlct0fl               : 1;  /**< [  0:  0](RO/H) BIST failure status for MLC tag arrays for ways 0-4. */
        uint64_t mlct1fl               : 1;  /**< [  1:  1](RO/H) BIST failure status for MLC tag arrays for ways 5-9. */
        uint64_t mlcd0fl               : 4;  /**< [  5:  2](RO/H) BIST failure status for MLC data arrays for ways 0-4. */
        uint64_t mlcd1fl               : 4;  /**< [  9:  6](RO/H) BIST failure status for MLC data arrays for ways 5-9. */
        uint64_t reserved_10_63        : 54;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_pic_bist_status_s cn; */
};
typedef union cavm_ccux_pic_bist_status cavm_ccux_pic_bist_status_t;

static inline uint64_t CAVM_CCUX_PIC_BIST_STATUS(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_PIC_BIST_STATUS(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && (a<=3))
        return 0x87e050000058ll + 0x1000000ll * ((a) & 0x3);
    if (cavm_is_model(OCTEONTX_CN98XX) && (a<=5))
        return 0x87e050000058ll + 0x1000000ll * ((a) & 0x7);
    if (cavm_is_model(OCTEONTX_CNF95XX) && (a==0))
        return 0x87e050000058ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_F95MM) && (a==0))
        return 0x87e050000058ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_F95O) && (a==0))
        return 0x87e050000058ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_LOKI) && (a==0))
        return 0x87e050000058ll + 0x1000000ll * ((a) & 0x0);
    __cavm_csr_fatal("CCUX_PIC_BIST_STATUS", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_PIC_BIST_STATUS(a) cavm_ccux_pic_bist_status_t
#define bustype_CAVM_CCUX_PIC_BIST_STATUS(a) CSR_TYPE_RSL
#define basename_CAVM_CCUX_PIC_BIST_STATUS(a) "CCUX_PIC_BIST_STATUS"
#define device_bar_CAVM_CCUX_PIC_BIST_STATUS(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_PIC_BIST_STATUS(a) (a)
#define arguments_CAVM_CCUX_PIC_BIST_STATUS(a) (a),-1,-1,-1

/**
 * Register (RSL) ccu#_pic_mlc_tag
 *
 * CCU Mid-Level Cache Tag Data Registers
 * This register holds the tag information for IS:LTGI commands.
 */
union cavm_ccux_pic_mlc_tag
{
    uint64_t u;
    struct cavm_ccux_pic_mlc_tag_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_46_63        : 18;
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit. */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) Reserved. */
        uint64_t tag                   : 26; /**< [ 42: 17](RO/H) The tag. TAG\<42:17\> is the corresponding bits from the MLC+LMC internal MLC/DRAM byte
                                                                 address. */
        uint64_t reserved_6_16         : 11;
        uint64_t parity                : 2;  /**< [  5:  4](RO/H) Indicates the tag parity. */
        uint64_t val                   : 1;  /**< [  3:  3](RO/H) Indicates the tag valid bit. */
        uint64_t reserved_0_2          : 3;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_2          : 3;
        uint64_t val                   : 1;  /**< [  3:  3](RO/H) Indicates the tag valid bit. */
        uint64_t parity                : 2;  /**< [  5:  4](RO/H) Indicates the tag parity. */
        uint64_t reserved_6_16         : 11;
        uint64_t tag                   : 26; /**< [ 42: 17](RO/H) The tag. TAG\<42:17\> is the corresponding bits from the MLC+LMC internal MLC/DRAM byte
                                                                 address. */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) Reserved. */
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit. */
        uint64_t reserved_46_63        : 18;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_pic_mlc_tag_s cn; */
};
typedef union cavm_ccux_pic_mlc_tag cavm_ccux_pic_mlc_tag_t;

static inline uint64_t CAVM_CCUX_PIC_MLC_TAG(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_PIC_MLC_TAG(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && (a<=3))
        return 0x87e050000010ll + 0x1000000ll * ((a) & 0x3);
    if (cavm_is_model(OCTEONTX_CN98XX) && (a<=5))
        return 0x87e050000010ll + 0x1000000ll * ((a) & 0x7);
    if (cavm_is_model(OCTEONTX_CNF95XX) && (a==0))
        return 0x87e050000010ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_F95MM) && (a==0))
        return 0x87e050000010ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_F95O) && (a==0))
        return 0x87e050000010ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_LOKI) && (a==0))
        return 0x87e050000010ll + 0x1000000ll * ((a) & 0x0);
    __cavm_csr_fatal("CCUX_PIC_MLC_TAG", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_PIC_MLC_TAG(a) cavm_ccux_pic_mlc_tag_t
#define bustype_CAVM_CCUX_PIC_MLC_TAG(a) CSR_TYPE_RSL
#define basename_CAVM_CCUX_PIC_MLC_TAG(a) "CCUX_PIC_MLC_TAG"
#define device_bar_CAVM_CCUX_PIC_MLC_TAG(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_PIC_MLC_TAG(a) (a)
#define arguments_CAVM_CCUX_PIC_MLC_TAG(a) (a),-1,-1,-1

/**
 * Register (RSL) ccu#_pic_mlc_waydis
 *
 * CCU PIC MLC Way Disable Register
 * These registers disable allocation of ways in the MLC.  For diagnostic use only.
 */
union cavm_ccux_pic_mlc_waydis
{
    uint64_t u;
    struct cavm_ccux_pic_mlc_waydis_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_10_63        : 54;
        uint64_t waydis                : 10; /**< [  9:  0](R/W) Each bit will disable allocation in the corresponding way. */
#else /* Word 0 - Little Endian */
        uint64_t waydis                : 10; /**< [  9:  0](R/W) Each bit will disable allocation in the corresponding way. */
        uint64_t reserved_10_63        : 54;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_pic_mlc_waydis_s cn; */
};
typedef union cavm_ccux_pic_mlc_waydis cavm_ccux_pic_mlc_waydis_t;

static inline uint64_t CAVM_CCUX_PIC_MLC_WAYDIS(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_PIC_MLC_WAYDIS(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && (a<=3))
        return 0x87e050000060ll + 0x1000000ll * ((a) & 0x3);
    if (cavm_is_model(OCTEONTX_CN98XX) && (a<=5))
        return 0x87e050000060ll + 0x1000000ll * ((a) & 0x7);
    if (cavm_is_model(OCTEONTX_CNF95XX) && (a==0))
        return 0x87e050000060ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_F95MM) && (a==0))
        return 0x87e050000060ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_F95O) && (a==0))
        return 0x87e050000060ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_LOKI) && (a==0))
        return 0x87e050000060ll + 0x1000000ll * ((a) & 0x0);
    __cavm_csr_fatal("CCUX_PIC_MLC_WAYDIS", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_PIC_MLC_WAYDIS(a) cavm_ccux_pic_mlc_waydis_t
#define bustype_CAVM_CCUX_PIC_MLC_WAYDIS(a) CSR_TYPE_RSL
#define basename_CAVM_CCUX_PIC_MLC_WAYDIS(a) "CCUX_PIC_MLC_WAYDIS"
#define device_bar_CAVM_CCUX_PIC_MLC_WAYDIS(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_PIC_MLC_WAYDIS(a) (a)
#define arguments_CAVM_CCUX_PIC_MLC_WAYDIS(a) (a),-1,-1,-1

/**
 * Register (RSL) ccu#_pic_mtd#_err
 *
 * CCU PIC MTD DAT Error Info Registers
 * This register records error information for MDC DBE/SBE interrupts which map to the
 * MDC node in the PIC_CTL_MLC_MTD_BISR. The first [DBE] error will lock the register
 * until the logged error type is cleared; [SBE] errors lock the register until either
 * the logged error type is cleared or a [DBE] error is logged. Only one of [SBE],
 * [DBE] should be set at a time. In the event the register is read with both [SBE] and
 * [SBE] equal to 0 during interrupt handling that is an indication that, due to a
 * register set/clear race, information about one or more errors was lost while
 * processing an earlier error.
 *
 * Software should scrub the MTD error indicated by the corresponding MDC_ECC_STATUS[ROW]
 * via a SYS CVMCACHEWBIL2I instruction (see below).  Otherwise, hardware may encounter
 * the error again.  Software may also choose to count the number of these errors.
 *
 * The SYS CVMCACHEWBIL2I instruction payload should have:
 *     \<pre\>
 *       payload\<20:17\> select the way (MDC_ECC_STATUS[ROW] for CCUn_PIC_MTD0_ERR;
 *                                      MDC_ECC_STATUS[ROW]+5 for CCUn_PIC_MTD1_ERR)
 *       payload\<16:7\> select the set/index.
 *     \</pre\>
 */
union cavm_ccux_pic_mtdx_err
{
    uint64_t u;
    struct cavm_ccux_pic_mtdx_err_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t sbe                   : 1;  /**< [ 63: 63](R/W1C/H) If [SBE]=1, then the logged information is for a MDC SBE error. */
        uint64_t dbe                   : 1;  /**< [ 62: 62](R/W1C/H) If [DBE]=1, then the logged information is for a MDC DBE error. */
        uint64_t cmd                   : 3;  /**< [ 61: 59](RO/H) Major command of request which detected the error. */
        uint64_t scmd                  : 4;  /**< [ 58: 55](RO/H) Sub-command of request which detected the error. */
        uint64_t sid                   : 3;  /**< [ 54: 52](RO/H) The sid of the core which issued the request causing the error. */
        uint64_t reserved_46_51        : 6;
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit of request which detected the error. */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) Reserved. */
        uint64_t addr                  : 36; /**< [ 42:  7](RO/H) Block address of the request which detected the error. */
        uint64_t way                   : 3;  /**< [  6:  4](RO/H) Reserved. */
        uint64_t dat                   : 4;  /**< [  3:  0](RO/H) Reserved. */
#else /* Word 0 - Little Endian */
        uint64_t dat                   : 4;  /**< [  3:  0](RO/H) Reserved. */
        uint64_t way                   : 3;  /**< [  6:  4](RO/H) Reserved. */
        uint64_t addr                  : 36; /**< [ 42:  7](RO/H) Block address of the request which detected the error. */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) Reserved. */
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit of request which detected the error. */
        uint64_t reserved_46_51        : 6;
        uint64_t sid                   : 3;  /**< [ 54: 52](RO/H) The sid of the core which issued the request causing the error. */
        uint64_t scmd                  : 4;  /**< [ 58: 55](RO/H) Sub-command of request which detected the error. */
        uint64_t cmd                   : 3;  /**< [ 61: 59](RO/H) Major command of request which detected the error. */
        uint64_t dbe                   : 1;  /**< [ 62: 62](R/W1C/H) If [DBE]=1, then the logged information is for a MDC DBE error. */
        uint64_t sbe                   : 1;  /**< [ 63: 63](R/W1C/H) If [SBE]=1, then the logged information is for a MDC SBE error. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_pic_mtdx_err_s cn; */
};
typedef union cavm_ccux_pic_mtdx_err cavm_ccux_pic_mtdx_err_t;

static inline uint64_t CAVM_CCUX_PIC_MTDX_ERR(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_PIC_MTDX_ERR(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050000068ll + 0x1000000ll * ((a) & 0x3) + 0x10000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050000068ll + 0x1000000ll * ((a) & 0x7) + 0x10000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050000068ll + 0x1000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050000068ll + 0x1000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050000068ll + 0x1000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050000068ll + 0x1000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_PIC_MTDX_ERR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_PIC_MTDX_ERR(a,b) cavm_ccux_pic_mtdx_err_t
#define bustype_CAVM_CCUX_PIC_MTDX_ERR(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_PIC_MTDX_ERR(a,b) "CCUX_PIC_MTDX_ERR"
#define device_bar_CAVM_CCUX_PIC_MTDX_ERR(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_PIC_MTDX_ERR(a,b) (a)
#define arguments_CAVM_CCUX_PIC_MTDX_ERR(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_pic_pfc#
 *
 * CCU PIC Performance Counter Registers
 */
union cavm_ccux_pic_pfcx
{
    uint64_t u;
    struct cavm_ccux_pic_pfcx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_pic_pfcx_s cn; */
};
typedef union cavm_ccux_pic_pfcx cavm_ccux_pic_pfcx_t;

static inline uint64_t CAVM_CCUX_PIC_PFCX(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_PIC_PFCX(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=3)))
        return 0x87e050000090ll + 0x1000000ll * ((a) & 0x3) + 0x10000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=3)))
        return 0x87e050000090ll + 0x1000000ll * ((a) & 0x7) + 0x10000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=3)))
        return 0x87e050000090ll + 0x1000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=3)))
        return 0x87e050000090ll + 0x1000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=3)))
        return 0x87e050000090ll + 0x1000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=3)))
        return 0x87e050000090ll + 0x1000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x3);
    __cavm_csr_fatal("CCUX_PIC_PFCX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_PIC_PFCX(a,b) cavm_ccux_pic_pfcx_t
#define bustype_CAVM_CCUX_PIC_PFCX(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_PIC_PFCX(a,b) "CCUX_PIC_PFCX"
#define device_bar_CAVM_CCUX_PIC_PFCX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_PIC_PFCX(a,b) (a)
#define arguments_CAVM_CCUX_PIC_PFCX(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_pic_prf
 *
 * CCU PIC Performance Counter Control Registers
 * All four counters are equivalent and can use any of the defined selects.
 */
union cavm_ccux_pic_prf
{
    uint64_t u;
    struct cavm_ccux_pic_prf_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t cnt3sel               : 8;  /**< [ 31: 24](R/W) Selects event to count for CCU()_PIC_PFC(3). Enumerated by CCU_PIC_PRF_SEL_E. */
        uint64_t cnt2sel               : 8;  /**< [ 23: 16](R/W) Selects event to count for CCU()_PIC_PFC(2). Enumerated by CCU_PIC_PRF_SEL_E. */
        uint64_t cnt1sel               : 8;  /**< [ 15:  8](R/W) Selects event to count for CCU()_PIC_PFC(1). Enumerated by CCU_PIC_PRF_SEL_E. */
        uint64_t cnt0sel               : 8;  /**< [  7:  0](R/W) Selects event to count for CCU()_PIC_PFC(0). Enumerated by CCU_PIC_PRF_SEL_E. */
#else /* Word 0 - Little Endian */
        uint64_t cnt0sel               : 8;  /**< [  7:  0](R/W) Selects event to count for CCU()_PIC_PFC(0). Enumerated by CCU_PIC_PRF_SEL_E. */
        uint64_t cnt1sel               : 8;  /**< [ 15:  8](R/W) Selects event to count for CCU()_PIC_PFC(1). Enumerated by CCU_PIC_PRF_SEL_E. */
        uint64_t cnt2sel               : 8;  /**< [ 23: 16](R/W) Selects event to count for CCU()_PIC_PFC(2). Enumerated by CCU_PIC_PRF_SEL_E. */
        uint64_t cnt3sel               : 8;  /**< [ 31: 24](R/W) Selects event to count for CCU()_PIC_PFC(3). Enumerated by CCU_PIC_PRF_SEL_E. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_pic_prf_s cn; */
};
typedef union cavm_ccux_pic_prf cavm_ccux_pic_prf_t;

static inline uint64_t CAVM_CCUX_PIC_PRF(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_PIC_PRF(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && (a<=3))
        return 0x87e050000088ll + 0x1000000ll * ((a) & 0x3);
    if (cavm_is_model(OCTEONTX_CN98XX) && (a<=5))
        return 0x87e050000088ll + 0x1000000ll * ((a) & 0x7);
    if (cavm_is_model(OCTEONTX_CNF95XX) && (a==0))
        return 0x87e050000088ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_F95MM) && (a==0))
        return 0x87e050000088ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_F95O) && (a==0))
        return 0x87e050000088ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_LOKI) && (a==0))
        return 0x87e050000088ll + 0x1000000ll * ((a) & 0x0);
    __cavm_csr_fatal("CCUX_PIC_PRF", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_PIC_PRF(a) cavm_ccux_pic_prf_t
#define bustype_CAVM_CCUX_PIC_PRF(a) CSR_TYPE_RSL
#define basename_CAVM_CCUX_PIC_PRF(a) "CCUX_PIC_PRF"
#define device_bar_CAVM_CCUX_PIC_PRF(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_PIC_PRF(a) (a)
#define arguments_CAVM_CCUX_PIC_PRF(a) (a),-1,-1,-1

/**
 * Register (RSL) ccu#_pic_tag#_err
 *
 * CCU PIC MTD TAG Error Info Registers
 * This register records error information for MDC tag parity interrupts which map to the
 * MDC node in the PIC_CTL_MLC_MTD_TBISR. The first error will lock the register until the
 * logged error type is cleared.  Note that tag entries that cause a parity error are automatically
 * invalidated. In the event the register is read with [PARERR] equal to
 * 0 during interrupt handling that is an indication that, due to a register set/clear
 * race, information about one or more errors was lost while processing an earlier
 * error. Note that no scrubbing is required for TAG parity errors.
 */
union cavm_ccux_pic_tagx_err
{
    uint64_t u;
    struct cavm_ccux_pic_tagx_err_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t parerr                : 1;  /**< [ 63: 63](R/W1C/H) Indicates a parity error. */
        uint64_t reserved_62           : 1;
        uint64_t cmd                   : 3;  /**< [ 61: 59](RO/H) Major command of request which detected the error. */
        uint64_t scmd                  : 4;  /**< [ 58: 55](RO/H) Sub-command of request which detected the error. */
        uint64_t sid                   : 3;  /**< [ 54: 52](RO/H) The sid of the core which issued the request causing the error. */
        uint64_t reserved_46_51        : 6;
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit of request which detected the error. */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) Reserved. */
        uint64_t addr                  : 36; /**< [ 42:  7](RO/H) Block address of the request which detected the error. */
        uint64_t way                   : 3;  /**< [  6:  4](RO/H) Reserved. */
        uint64_t reserved_0_3          : 4;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_3          : 4;
        uint64_t way                   : 3;  /**< [  6:  4](RO/H) Reserved. */
        uint64_t addr                  : 36; /**< [ 42:  7](RO/H) Block address of the request which detected the error. */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) Reserved. */
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit of request which detected the error. */
        uint64_t reserved_46_51        : 6;
        uint64_t sid                   : 3;  /**< [ 54: 52](RO/H) The sid of the core which issued the request causing the error. */
        uint64_t scmd                  : 4;  /**< [ 58: 55](RO/H) Sub-command of request which detected the error. */
        uint64_t cmd                   : 3;  /**< [ 61: 59](RO/H) Major command of request which detected the error. */
        uint64_t reserved_62           : 1;
        uint64_t parerr                : 1;  /**< [ 63: 63](R/W1C/H) Indicates a parity error. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_pic_tagx_err_s cn; */
};
typedef union cavm_ccux_pic_tagx_err cavm_ccux_pic_tagx_err_t;

static inline uint64_t CAVM_CCUX_PIC_TAGX_ERR(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_PIC_TAGX_ERR(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050000070ll + 0x1000000ll * ((a) & 0x3) + 0x10000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050000070ll + 0x1000000ll * ((a) & 0x7) + 0x10000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050000070ll + 0x1000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050000070ll + 0x1000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050000070ll + 0x1000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050000070ll + 0x1000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_PIC_TAGX_ERR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_PIC_TAGX_ERR(a,b) cavm_ccux_pic_tagx_err_t
#define bustype_CAVM_CCUX_PIC_TAGX_ERR(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_PIC_TAGX_ERR(a,b) "CCUX_PIC_TAGX_ERR"
#define device_bar_CAVM_CCUX_PIC_TAGX_ERR(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_PIC_TAGX_ERR(a,b) (a)
#define arguments_CAVM_CCUX_PIC_TAGX_ERR(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_tad#_dat_err
 *
 * CCU TAD DAT Error Info Registers
 * This register records error information for MDC DBE/SBE interrupts which map to the
 * MDC node in the TAD_CTL_DTC. The first [DBE] error will lock the register until the
 * logged error type is cleared; [SBE] errors lock the register until either the logged
 * error type is cleared or a [DBE] error is logged. Only one of [SBE], [DBE] should be
 * set at a time. In the event the register is read with both [SBE] and [SBE] equal to
 * 0 during interrupt handling that is an indication that, due to a register set/clear
 * race, information about one or more errors was lost while processing an earlier
 * error. Note that MDC_ECC_STATUS[ROW] is sufficient to scrub any DAT
 * error with an index/way command (DTG=0, WAY[4]=0, WAY[3:0]=ROW[29:26],
 * IDX[9:0]=ROW[25:16]).
 */
union cavm_ccux_tadx_dat_err
{
    uint64_t u;
    struct cavm_ccux_tadx_dat_err_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t sbe                   : 1;  /**< [ 63: 63](R/W1C/H) If [SBE]=1, then the logged information is for a MDC SBE error. */
        uint64_t dbe                   : 1;  /**< [ 62: 62](R/W1C/H) If [DBE]=1, then the logged information is for a MDC DBE error. */
        uint64_t cmd                   : 3;  /**< [ 61: 59](RO/H) Major command of request which detected the error. */
        uint64_t scmd                  : 4;  /**< [ 58: 55](RO/H) Sub-command of request which detected the error. */
        uint64_t sid                   : 3;  /**< [ 54: 52](RO/H) If [MS] indicates the source is a cluster, [SID] is the sid of the core within
                                                                 the cluster which issued the request causing the error. If [MS] indicated the
                                                                 source is not a cluster, [SID] is unpredictable. */
        uint64_t ms                    : 6;  /**< [ 51: 46](RO/H) Mesh source of the request which detected the error. */
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit of request which detected the error. */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) Reserved. */
        uint64_t addr                  : 36; /**< [ 42:  7](RO/H) Block address of the cache-line in which the error was detected. This can be the
                                                                 address of the request which detected the error or the address of an eviction
                                                                 caused by the request. This field is the physical address after TAD set aliasing
                                                                 (if enabled, see CCS_ADR_CTL[DISSETALIAS]). */
        uint64_t ow                    : 3;  /**< [  6:  4](RO/H) 128-bit word in which the error was detected. */
        uint64_t reserved_0_3          : 4;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_3          : 4;
        uint64_t ow                    : 3;  /**< [  6:  4](RO/H) 128-bit word in which the error was detected. */
        uint64_t addr                  : 36; /**< [ 42:  7](RO/H) Block address of the cache-line in which the error was detected. This can be the
                                                                 address of the request which detected the error or the address of an eviction
                                                                 caused by the request. This field is the physical address after TAD set aliasing
                                                                 (if enabled, see CCS_ADR_CTL[DISSETALIAS]). */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) Reserved. */
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit of request which detected the error. */
        uint64_t ms                    : 6;  /**< [ 51: 46](RO/H) Mesh source of the request which detected the error. */
        uint64_t sid                   : 3;  /**< [ 54: 52](RO/H) If [MS] indicates the source is a cluster, [SID] is the sid of the core within
                                                                 the cluster which issued the request causing the error. If [MS] indicated the
                                                                 source is not a cluster, [SID] is unpredictable. */
        uint64_t scmd                  : 4;  /**< [ 58: 55](RO/H) Sub-command of request which detected the error. */
        uint64_t cmd                   : 3;  /**< [ 61: 59](RO/H) Major command of request which detected the error. */
        uint64_t dbe                   : 1;  /**< [ 62: 62](R/W1C/H) If [DBE]=1, then the logged information is for a MDC DBE error. */
        uint64_t sbe                   : 1;  /**< [ 63: 63](R/W1C/H) If [SBE]=1, then the logged information is for a MDC SBE error. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_dat_err_s cn; */
};
typedef union cavm_ccux_tadx_dat_err cavm_ccux_tadx_dat_err_t;

static inline uint64_t CAVM_CCUX_TADX_DAT_ERR(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_DAT_ERR(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050400018ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050400018ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050400018ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050400018ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050400018ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050400018ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_TADX_DAT_ERR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_DAT_ERR(a,b) cavm_ccux_tadx_dat_err_t
#define bustype_CAVM_CCUX_TADX_DAT_ERR(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_DAT_ERR(a,b) "CCUX_TADX_DAT_ERR"
#define device_bar_CAVM_CCUX_TADX_DAT_ERR(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_DAT_ERR(a,b) (a)
#define arguments_CAVM_CCUX_TADX_DAT_ERR(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_tad#_int_ena_w1c
 *
 * CCU TAD Interrupt Enable Clear Registers
 * This register clears interrupt enable bits.
 */
union cavm_ccux_tadx_int_ena_w1c
{
    uint64_t u;
    struct cavm_ccux_tadx_int_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for CCU(0..3)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for CCU(0..3)_TAD(0..1)_INT_W1C[RDNXM]. */
#else /* Word 0 - Little Endian */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for CCU(0..3)_TAD(0..1)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for CCU(0..3)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_int_ena_w1c_s cn9; */
    /* struct cavm_ccux_tadx_int_ena_w1c_s cn96xx; */
    struct cavm_ccux_tadx_int_ena_w1c_cn98xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for CCU(0..5)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for CCU(0..5)_TAD(0..1)_INT_W1C[RDNXM]. */
#else /* Word 0 - Little Endian */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for CCU(0..5)_TAD(0..1)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for CCU(0..5)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } cn98xx;
    struct cavm_ccux_tadx_int_ena_w1c_cnf95xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for CCU(0)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for CCU(0)_TAD(0..1)_INT_W1C[RDNXM]. */
#else /* Word 0 - Little Endian */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for CCU(0)_TAD(0..1)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for CCU(0)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } cnf95xx;
    /* struct cavm_ccux_tadx_int_ena_w1c_cnf95xx f95mm; */
    /* struct cavm_ccux_tadx_int_ena_w1c_cnf95xx f95o; */
    /* struct cavm_ccux_tadx_int_ena_w1c_cnf95xx loki; */
};
typedef union cavm_ccux_tadx_int_ena_w1c cavm_ccux_tadx_int_ena_w1c_t;

static inline uint64_t CAVM_CCUX_TADX_INT_ENA_W1C(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_INT_ENA_W1C(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050480010ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050480010ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050480010ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050480010ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050480010ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050480010ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_TADX_INT_ENA_W1C", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_INT_ENA_W1C(a,b) cavm_ccux_tadx_int_ena_w1c_t
#define bustype_CAVM_CCUX_TADX_INT_ENA_W1C(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_INT_ENA_W1C(a,b) "CCUX_TADX_INT_ENA_W1C"
#define device_bar_CAVM_CCUX_TADX_INT_ENA_W1C(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_INT_ENA_W1C(a,b) (a)
#define arguments_CAVM_CCUX_TADX_INT_ENA_W1C(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_tad#_int_ena_w1s
 *
 * CCU TAD Interrupt Enable Set Registers
 * This register sets interrupt enable bits.
 */
union cavm_ccux_tadx_int_ena_w1s
{
    uint64_t u;
    struct cavm_ccux_tadx_int_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for CCU(0..3)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for CCU(0..3)_TAD(0..1)_INT_W1C[RDNXM]. */
#else /* Word 0 - Little Endian */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for CCU(0..3)_TAD(0..1)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for CCU(0..3)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_int_ena_w1s_s cn9; */
    /* struct cavm_ccux_tadx_int_ena_w1s_s cn96xx; */
    struct cavm_ccux_tadx_int_ena_w1s_cn98xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for CCU(0..5)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for CCU(0..5)_TAD(0..1)_INT_W1C[RDNXM]. */
#else /* Word 0 - Little Endian */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for CCU(0..5)_TAD(0..1)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for CCU(0..5)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } cn98xx;
    struct cavm_ccux_tadx_int_ena_w1s_cnf95xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for CCU(0)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for CCU(0)_TAD(0..1)_INT_W1C[RDNXM]. */
#else /* Word 0 - Little Endian */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for CCU(0)_TAD(0..1)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for CCU(0)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } cnf95xx;
    /* struct cavm_ccux_tadx_int_ena_w1s_cnf95xx f95mm; */
    /* struct cavm_ccux_tadx_int_ena_w1s_cnf95xx f95o; */
    /* struct cavm_ccux_tadx_int_ena_w1s_cnf95xx loki; */
};
typedef union cavm_ccux_tadx_int_ena_w1s cavm_ccux_tadx_int_ena_w1s_t;

static inline uint64_t CAVM_CCUX_TADX_INT_ENA_W1S(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_INT_ENA_W1S(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050480018ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050480018ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050480018ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050480018ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050480018ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050480018ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_TADX_INT_ENA_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_INT_ENA_W1S(a,b) cavm_ccux_tadx_int_ena_w1s_t
#define bustype_CAVM_CCUX_TADX_INT_ENA_W1S(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_INT_ENA_W1S(a,b) "CCUX_TADX_INT_ENA_W1S"
#define device_bar_CAVM_CCUX_TADX_INT_ENA_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_INT_ENA_W1S(a,b) (a)
#define arguments_CAVM_CCUX_TADX_INT_ENA_W1S(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_tad#_int_w1c
 *
 * CCU TAD Interrupt Register
 * This register is for TAD-based interrupts.
 */
union cavm_ccux_tadx_int_w1c
{
    uint64_t u;
    struct cavm_ccux_tadx_int_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1C/H) Write NxM error. Write reference to outside all the defined and enabled address
                                                                 space control (ASC) regions, or secure write reference to an ASC region
                                                                 not enabled for secure access, or nonsecure write reference to an
                                                                 ASC region not enabled for nonsecure access.
                                                                 See CCU()_TAD()_NXM_ERR for logged information.
                                                                 See CCS_ASC_REGION()_START, CCS_ASC_REGION()_END, and
                                                                 CCS_ASC_REGION()_ATTR for ASC region specification. */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1C/H) Read NxM error. Read reference to outside all the defined and enabled address
                                                                 space control (ASC) regions, or secure read reference to an ASC region not
                                                                 enabled for secure access, or nonsecure read reference to an ASC region not
                                                                 enabled for nonsecure access.
                                                                 [RDNXM] interrupts can occur during normal operation as the cores are
                                                                 allowed to prefetch to nonexistent memory locations.  Therefore,
                                                                 [RDNXM] is for informational purposes only.
                                                                 See CCU()_TAD()_NXM_ERR for logged information.
                                                                 See CCS_ASC_REGION()_START, CCS_ASC_REGION()_END, and
                                                                 CCS_ASC_REGION()_ATTR for ASC region specification. */
#else /* Word 0 - Little Endian */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1C/H) Read NxM error. Read reference to outside all the defined and enabled address
                                                                 space control (ASC) regions, or secure read reference to an ASC region not
                                                                 enabled for secure access, or nonsecure read reference to an ASC region not
                                                                 enabled for nonsecure access.
                                                                 [RDNXM] interrupts can occur during normal operation as the cores are
                                                                 allowed to prefetch to nonexistent memory locations.  Therefore,
                                                                 [RDNXM] is for informational purposes only.
                                                                 See CCU()_TAD()_NXM_ERR for logged information.
                                                                 See CCS_ASC_REGION()_START, CCS_ASC_REGION()_END, and
                                                                 CCS_ASC_REGION()_ATTR for ASC region specification. */
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1C/H) Write NxM error. Write reference to outside all the defined and enabled address
                                                                 space control (ASC) regions, or secure write reference to an ASC region
                                                                 not enabled for secure access, or nonsecure write reference to an
                                                                 ASC region not enabled for nonsecure access.
                                                                 See CCU()_TAD()_NXM_ERR for logged information.
                                                                 See CCS_ASC_REGION()_START, CCS_ASC_REGION()_END, and
                                                                 CCS_ASC_REGION()_ATTR for ASC region specification. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_int_w1c_s cn; */
};
typedef union cavm_ccux_tadx_int_w1c cavm_ccux_tadx_int_w1c_t;

static inline uint64_t CAVM_CCUX_TADX_INT_W1C(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_INT_W1C(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050480000ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050480000ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050480000ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050480000ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050480000ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050480000ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_TADX_INT_W1C", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_INT_W1C(a,b) cavm_ccux_tadx_int_w1c_t
#define bustype_CAVM_CCUX_TADX_INT_W1C(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_INT_W1C(a,b) "CCUX_TADX_INT_W1C"
#define device_bar_CAVM_CCUX_TADX_INT_W1C(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_INT_W1C(a,b) (a)
#define arguments_CAVM_CCUX_TADX_INT_W1C(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_tad#_int_w1s
 *
 * CCU TAD Interrupt Set Registers
 * This register sets interrupt bits.
 */
union cavm_ccux_tadx_int_w1s
{
    uint64_t u;
    struct cavm_ccux_tadx_int_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets CCU(0..3)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets CCU(0..3)_TAD(0..1)_INT_W1C[RDNXM]. */
#else /* Word 0 - Little Endian */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets CCU(0..3)_TAD(0..1)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets CCU(0..3)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_int_w1s_s cn9; */
    /* struct cavm_ccux_tadx_int_w1s_s cn96xx; */
    struct cavm_ccux_tadx_int_w1s_cn98xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets CCU(0..5)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets CCU(0..5)_TAD(0..1)_INT_W1C[RDNXM]. */
#else /* Word 0 - Little Endian */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets CCU(0..5)_TAD(0..1)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets CCU(0..5)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } cn98xx;
    struct cavm_ccux_tadx_int_w1s_cnf95xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets CCU(0)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets CCU(0)_TAD(0..1)_INT_W1C[RDNXM]. */
#else /* Word 0 - Little Endian */
        uint64_t rdnxm                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets CCU(0)_TAD(0..1)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets CCU(0)_TAD(0..1)_INT_W1C[WRNXM]. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } cnf95xx;
    /* struct cavm_ccux_tadx_int_w1s_cnf95xx f95mm; */
    /* struct cavm_ccux_tadx_int_w1s_cnf95xx f95o; */
    /* struct cavm_ccux_tadx_int_w1s_cnf95xx loki; */
};
typedef union cavm_ccux_tadx_int_w1s cavm_ccux_tadx_int_w1s_t;

static inline uint64_t CAVM_CCUX_TADX_INT_W1S(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_INT_W1S(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050480008ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050480008ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050480008ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050480008ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050480008ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050480008ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_TADX_INT_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_INT_W1S(a,b) cavm_ccux_tadx_int_w1s_t
#define bustype_CAVM_CCUX_TADX_INT_W1S(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_INT_W1S(a,b) "CCUX_TADX_INT_W1S"
#define device_bar_CAVM_CCUX_TADX_INT_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_INT_W1S(a,b) (a)
#define arguments_CAVM_CCUX_TADX_INT_W1S(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_tad#_mpar#_acnt
 *
 * CCU Memory Paritioning Allocate Count Registers
 */
union cavm_ccux_tadx_mparx_acnt
{
    uint64_t u;
    struct cavm_ccux_tadx_mparx_acnt_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_48_63        : 16;
        uint64_t cnt                   : 48; /**< [ 47:  0](R/W/H) Counter incremented whenever a request allocates while using the corresponding
                                                                 MPARID. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 48; /**< [ 47:  0](R/W/H) Counter incremented whenever a request allocates while using the corresponding
                                                                 MPARID. */
        uint64_t reserved_48_63        : 16;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_mparx_acnt_s cn; */
};
typedef union cavm_ccux_tadx_mparx_acnt cavm_ccux_tadx_mparx_acnt_t;

static inline uint64_t CAVM_CCUX_TADX_MPARX_ACNT(uint64_t a, uint64_t b, uint64_t c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_MPARX_ACNT(uint64_t a, uint64_t b, uint64_t c)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1) && (c<=255)))
        return 0x87e050401000ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0xff);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1) && (c<=255)))
        return 0x87e050401000ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0xff);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1) && (c<=255)))
        return 0x87e050401000ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0xff);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1) && (c<=255)))
        return 0x87e050401000ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0xff);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1) && (c<=255)))
        return 0x87e050401000ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0xff);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1) && (c<=255)))
        return 0x87e050401000ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0xff);
    __cavm_csr_fatal("CCUX_TADX_MPARX_ACNT", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_MPARX_ACNT(a,b,c) cavm_ccux_tadx_mparx_acnt_t
#define bustype_CAVM_CCUX_TADX_MPARX_ACNT(a,b,c) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_MPARX_ACNT(a,b,c) "CCUX_TADX_MPARX_ACNT"
#define device_bar_CAVM_CCUX_TADX_MPARX_ACNT(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_MPARX_ACNT(a,b,c) (a)
#define arguments_CAVM_CCUX_TADX_MPARX_ACNT(a,b,c) (a),(b),(c),-1

/**
 * Register (RSL) ccu#_tad#_mpar#_hcnt
 *
 * CCU Memory Paritioning Hit Count Registers
 */
union cavm_ccux_tadx_mparx_hcnt
{
    uint64_t u;
    struct cavm_ccux_tadx_mparx_hcnt_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_48_63        : 16;
        uint64_t cnt                   : 48; /**< [ 47:  0](R/W/H) Counter incremented whenever a request hits in the LTG or DTG while using the
                                                                 corresponding MPARID. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 48; /**< [ 47:  0](R/W/H) Counter incremented whenever a request hits in the LTG or DTG while using the
                                                                 corresponding MPARID. */
        uint64_t reserved_48_63        : 16;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_mparx_hcnt_s cn; */
};
typedef union cavm_ccux_tadx_mparx_hcnt cavm_ccux_tadx_mparx_hcnt_t;

static inline uint64_t CAVM_CCUX_TADX_MPARX_HCNT(uint64_t a, uint64_t b, uint64_t c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_MPARX_HCNT(uint64_t a, uint64_t b, uint64_t c)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1) && (c<=255)))
        return 0x87e050401008ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0xff);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1) && (c<=255)))
        return 0x87e050401008ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0xff);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1) && (c<=255)))
        return 0x87e050401008ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0xff);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1) && (c<=255)))
        return 0x87e050401008ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0xff);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1) && (c<=255)))
        return 0x87e050401008ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0xff);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1) && (c<=255)))
        return 0x87e050401008ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0xff);
    __cavm_csr_fatal("CCUX_TADX_MPARX_HCNT", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_MPARX_HCNT(a,b,c) cavm_ccux_tadx_mparx_hcnt_t
#define bustype_CAVM_CCUX_TADX_MPARX_HCNT(a,b,c) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_MPARX_HCNT(a,b,c) "CCUX_TADX_MPARX_HCNT"
#define device_bar_CAVM_CCUX_TADX_MPARX_HCNT(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_MPARX_HCNT(a,b,c) (a)
#define arguments_CAVM_CCUX_TADX_MPARX_HCNT(a,b,c) (a),(b),(c),-1

/**
 * Register (RSL) ccu#_tad#_nxm_err
 *
 * CCU TAD ASC Error Info Registers
 * This register records error information for RDNXM and WRNXM interrupts. The NXM
 * logic only applies to local addresses. A command for a remote address does not cause
 * a [RDNXM]/[WRNXM] on the requesting node, but may on the remote node. The first
 * [WRNXM] error will lock the register until the logged error type is cleared; [RDNXM]
 * errors lock the register until either the logged error type is cleared or a [WRNXM]
 * error is logged.
 */
union cavm_ccux_tadx_nxm_err
{
    uint64_t u;
    struct cavm_ccux_tadx_nxm_err_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t rdnxm                 : 1;  /**< [ 63: 63](RO/H) Logged information is for a CCU()_TAD()_INT_W1C[RDNXM] error. */
        uint64_t wrnxm                 : 1;  /**< [ 62: 62](RO/H) Logged information is for a CCU()_TAD()_INT_W1C[WRNXM] error. */
        uint64_t cmd                   : 3;  /**< [ 61: 59](RO/H) Major command of request causing error. */
        uint64_t scmd                  : 4;  /**< [ 58: 55](RO/H) Sub-command of request causing error. */
        uint64_t sid                   : 3;  /**< [ 54: 52](RO/H) If [MS] indicates the source is a cluster, [SID] is the sid of the core within
                                                                 the cluster which issued the request causing the error. If [MS] indicated the
                                                                 source is not a cluster, [SID] is unpredictable. */
        uint64_t ms                    : 6;  /**< [ 51: 46](RO/H) Mesh source of the request causing the error. */
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit of request causing error. */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) Reserved. */
        uint64_t addr                  : 43; /**< [ 42:  0](RO/H) Address of the request causing the error. [ADDR]\<6:0\> is unpredictable. This
                                                                 field is the physical address after TAD set aliasing (if enabled, see
                                                                 CCS_ADR_CTL[DISSETALIAS]). */
#else /* Word 0 - Little Endian */
        uint64_t addr                  : 43; /**< [ 42:  0](RO/H) Address of the request causing the error. [ADDR]\<6:0\> is unpredictable. This
                                                                 field is the physical address after TAD set aliasing (if enabled, see
                                                                 CCS_ADR_CTL[DISSETALIAS]). */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) Reserved. */
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit of request causing error. */
        uint64_t ms                    : 6;  /**< [ 51: 46](RO/H) Mesh source of the request causing the error. */
        uint64_t sid                   : 3;  /**< [ 54: 52](RO/H) If [MS] indicates the source is a cluster, [SID] is the sid of the core within
                                                                 the cluster which issued the request causing the error. If [MS] indicated the
                                                                 source is not a cluster, [SID] is unpredictable. */
        uint64_t scmd                  : 4;  /**< [ 58: 55](RO/H) Sub-command of request causing error. */
        uint64_t cmd                   : 3;  /**< [ 61: 59](RO/H) Major command of request causing error. */
        uint64_t wrnxm                 : 1;  /**< [ 62: 62](RO/H) Logged information is for a CCU()_TAD()_INT_W1C[WRNXM] error. */
        uint64_t rdnxm                 : 1;  /**< [ 63: 63](RO/H) Logged information is for a CCU()_TAD()_INT_W1C[RDNXM] error. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_nxm_err_s cn; */
};
typedef union cavm_ccux_tadx_nxm_err cavm_ccux_tadx_nxm_err_t;

static inline uint64_t CAVM_CCUX_TADX_NXM_ERR(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_NXM_ERR(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050400010ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050400010ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050400010ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050400010ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050400010ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050400010ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_TADX_NXM_ERR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_NXM_ERR(a,b) cavm_ccux_tadx_nxm_err_t
#define bustype_CAVM_CCUX_TADX_NXM_ERR(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_NXM_ERR(a,b) "CCUX_TADX_NXM_ERR"
#define device_bar_CAVM_CCUX_TADX_NXM_ERR(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_NXM_ERR(a,b) (a)
#define arguments_CAVM_CCUX_TADX_NXM_ERR(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_tad#_pfc#
 *
 * CCU TAD Performance Counter Registers
 */
union cavm_ccux_tadx_pfcx
{
    uint64_t u;
    struct cavm_ccux_tadx_pfcx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_pfcx_s cn; */
};
typedef union cavm_ccux_tadx_pfcx cavm_ccux_tadx_pfcx_t;

static inline uint64_t CAVM_CCUX_TADX_PFCX(uint64_t a, uint64_t b, uint64_t c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_PFCX(uint64_t a, uint64_t b, uint64_t c)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1) && (c<=3)))
        return 0x87e050420040ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0x3);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1) && (c<=3)))
        return 0x87e050420040ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0x3);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1) && (c<=3)))
        return 0x87e050420040ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0x3);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1) && (c<=3)))
        return 0x87e050420040ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0x3);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1) && (c<=3)))
        return 0x87e050420040ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0x3);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1) && (c<=3)))
        return 0x87e050420040ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1) + 0x10ll * ((c) & 0x3);
    __cavm_csr_fatal("CCUX_TADX_PFCX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_PFCX(a,b,c) cavm_ccux_tadx_pfcx_t
#define bustype_CAVM_CCUX_TADX_PFCX(a,b,c) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_PFCX(a,b,c) "CCUX_TADX_PFCX"
#define device_bar_CAVM_CCUX_TADX_PFCX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_PFCX(a,b,c) (a)
#define arguments_CAVM_CCUX_TADX_PFCX(a,b,c) (a),(b),(c),-1

/**
 * Register (RSL) ccu#_tad#_prf
 *
 * CCU TAD Performance Counter Control Registers
 * All four counters are equivalent and can use any of the defined selects.
 */
union cavm_ccux_tadx_prf
{
    uint64_t u;
    struct cavm_ccux_tadx_prf_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t cnt3sel               : 8;  /**< [ 31: 24](R/W) Selects event to count for CCU()_TAD()_PFC(3). Enumerated by CCU_TAD_PRF_SEL_E. */
        uint64_t cnt2sel               : 8;  /**< [ 23: 16](R/W) Selects event to count for CCU()_TAD()_PFC(2). Enumerated by CCU_TAD_PRF_SEL_E. */
        uint64_t cnt1sel               : 8;  /**< [ 15:  8](R/W) Selects event to count for CCU()_TAD()_PFC(1). Enumerated by CCU_TAD_PRF_SEL_E. */
        uint64_t cnt0sel               : 8;  /**< [  7:  0](R/W) Selects event to count for CCU()_TAD()_PFC(0). Enumerated by CCU_TAD_PRF_SEL_E. */
#else /* Word 0 - Little Endian */
        uint64_t cnt0sel               : 8;  /**< [  7:  0](R/W) Selects event to count for CCU()_TAD()_PFC(0). Enumerated by CCU_TAD_PRF_SEL_E. */
        uint64_t cnt1sel               : 8;  /**< [ 15:  8](R/W) Selects event to count for CCU()_TAD()_PFC(1). Enumerated by CCU_TAD_PRF_SEL_E. */
        uint64_t cnt2sel               : 8;  /**< [ 23: 16](R/W) Selects event to count for CCU()_TAD()_PFC(2). Enumerated by CCU_TAD_PRF_SEL_E. */
        uint64_t cnt3sel               : 8;  /**< [ 31: 24](R/W) Selects event to count for CCU()_TAD()_PFC(3). Enumerated by CCU_TAD_PRF_SEL_E. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_prf_s cn; */
};
typedef union cavm_ccux_tadx_prf cavm_ccux_tadx_prf_t;

static inline uint64_t CAVM_CCUX_TADX_PRF(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_PRF(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050420000ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050420000ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050420000ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050420000ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050420000ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050420000ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_TADX_PRF", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_PRF(a,b) cavm_ccux_tadx_prf_t
#define bustype_CAVM_CCUX_TADX_PRF(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_PRF(a,b) "CCUX_TADX_PRF"
#define device_bar_CAVM_CCUX_TADX_PRF(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_PRF(a,b) (a)
#define arguments_CAVM_CCUX_TADX_PRF(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_tad#_tag
 *
 * CCU TAD TAG Data Register
 * This register records the tag information read by a LTGI command.
 */
union cavm_ccux_tadx_tag
{
    uint64_t u;
    struct cavm_ccux_tadx_tag_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t is                    : 1;  /**< [ 63: 63](RO/H) The i-stream bit. */
        uint64_t sblkdty               : 4;  /**< [ 62: 59](RO/H) The sub-block dirty bits. */
        uint64_t ci                    : 10; /**< [ 58: 49](RO/H) The cluster info. */
        uint64_t ts                    : 3;  /**< [ 48: 46](RO/H) The tag state. */
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit. */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) The node bits. */
        uint64_t tag                   : 25; /**< [ 42: 18](RO/H) The tag bits. */
        uint64_t reserved_0_17         : 18;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_17         : 18;
        uint64_t tag                   : 25; /**< [ 42: 18](RO/H) The tag bits. */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) The node bits. */
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit. */
        uint64_t ts                    : 3;  /**< [ 48: 46](RO/H) The tag state. */
        uint64_t ci                    : 10; /**< [ 58: 49](RO/H) The cluster info. */
        uint64_t sblkdty               : 4;  /**< [ 62: 59](RO/H) The sub-block dirty bits. */
        uint64_t is                    : 1;  /**< [ 63: 63](RO/H) The i-stream bit. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_tag_s cn; */
};
typedef union cavm_ccux_tadx_tag cavm_ccux_tadx_tag_t;

static inline uint64_t CAVM_CCUX_TADX_TAG(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_TAG(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050400028ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050400028ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050400028ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050400028ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050400028ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050400028ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_TADX_TAG", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_TAG(a,b) cavm_ccux_tadx_tag_t
#define bustype_CAVM_CCUX_TADX_TAG(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_TAG(a,b) "CCUX_TADX_TAG"
#define device_bar_CAVM_CCUX_TADX_TAG(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_TAG(a,b) (a)
#define arguments_CAVM_CCUX_TADX_TAG(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_tad#_wbar
 *
 * CCU TAD Write Barrier Register
 * This register allows the initiation of a WBAR/WRFL sequence and provides status of
 * its completion. If a new sequence is requested before the first is completed INPROG
 * will remain asserted until all memory writes preceding the last CSR write to [START]
 * have been followed by a WBAR/WRFL sequence.
 */
union cavm_ccux_tadx_wbar
{
    uint64_t u;
    struct cavm_ccux_tadx_wbar_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t inprog                : 1;  /**< [ 63: 63](RO/H) In progress. Set after [START] is written to one, and will clear when the
                                                                 requested WBAR/WRFL sequence has completed. */
        uint64_t reserved_4_62         : 59;
        uint64_t lmcmsk                : 3;  /**< [  3:  1](R/W) Mask of which LMCs to send WBAR/WRFL sequences to. A value of 0x0 will send to all. */
        uint64_t start                 : 1;  /**< [  0:  0](R/W1/H) Start. Writing a one requests a WBAR/WRFL sequence with the persist bit
                                                                 set. [START] will clear once the sequence has been logged as a request. [INPROG]
                                                                 will remain one until all requested sequences are complete. Writing a zero to
                                                                 this bit has no effect. */
#else /* Word 0 - Little Endian */
        uint64_t start                 : 1;  /**< [  0:  0](R/W1/H) Start. Writing a one requests a WBAR/WRFL sequence with the persist bit
                                                                 set. [START] will clear once the sequence has been logged as a request. [INPROG]
                                                                 will remain one until all requested sequences are complete. Writing a zero to
                                                                 this bit has no effect. */
        uint64_t lmcmsk                : 3;  /**< [  3:  1](R/W) Mask of which LMCs to send WBAR/WRFL sequences to. A value of 0x0 will send to all. */
        uint64_t reserved_4_62         : 59;
        uint64_t inprog                : 1;  /**< [ 63: 63](RO/H) In progress. Set after [START] is written to one, and will clear when the
                                                                 requested WBAR/WRFL sequence has completed. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_wbar_s cn; */
};
typedef union cavm_ccux_tadx_wbar cavm_ccux_tadx_wbar_t;

static inline uint64_t CAVM_CCUX_TADX_WBAR(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_WBAR(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050400030ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050400030ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050400030ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050400030ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050400030ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050400030ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_TADX_WBAR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_WBAR(a,b) cavm_ccux_tadx_wbar_t
#define bustype_CAVM_CCUX_TADX_WBAR(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_WBAR(a,b) "CCUX_TADX_WBAR"
#define device_bar_CAVM_CCUX_TADX_WBAR(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_WBAR(a,b) (a)
#define arguments_CAVM_CCUX_TADX_WBAR(a,b) (a),(b),-1,-1

/**
 * Register (RSL) ccu#_tad#_xbf_err
 *
 * CCU TAD XBF Error Info Registers
 * This register records error information for XBF MDC DBE/SBE interrupts which map to
 * the MDC node in the TAD DAT XBF. The first [DBE] error will lock the register until the
 * logged error type is cleared; [SBE] errors lock the register until either the logged
 * error type is cleared or a [DBE] error is logged. Only one of [SBE], [DBE] should be
 * set at a time. In the event the register is read with both [SBE] and [SBE] equal to
 * 0 during interrupt handling that is an indication that, due to a register set/clear
 * race, information about one or more errors was lost while processing an earlier
 * error. Note that no scrubbing is required for XBF errors.
 */
union cavm_ccux_tadx_xbf_err
{
    uint64_t u;
    struct cavm_ccux_tadx_xbf_err_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t sbe                   : 1;  /**< [ 63: 63](R/W1C/H) If [SBE]=1, then the logged information is for a MDC SBE error. */
        uint64_t dbe                   : 1;  /**< [ 62: 62](R/W1C/H) If [DBE]=1, then the logged information is for a MDC DBE error. */
        uint64_t cmd                   : 3;  /**< [ 61: 59](RO/H) Major command of request which detected the error. */
        uint64_t scmd                  : 4;  /**< [ 58: 55](RO/H) Sub-command of request which detected the error. */
        uint64_t sid                   : 3;  /**< [ 54: 52](RO/H) If [MS] indicates the source is a cluster, [SID] is the sid of the core within
                                                                 the cluster which issued the request causing the error. If [MS] indicated the
                                                                 source is not a cluster, [SID] is unpredictable. */
        uint64_t ms                    : 6;  /**< [ 51: 46](RO/H) Mesh source of the request which detected the error. */
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit of request which detected the error. */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) Reserved. */
        uint64_t addr                  : 36; /**< [ 42:  7](RO/H) Block address of the request which detected the error. This field is the
                                                                 physical address after TAD set aliasing (if enabled, see
                                                                 CCS_ADR_CTL[DISSETALIAS]). */
        uint64_t ow                    : 3;  /**< [  6:  4](RO/H) 128-bit word in which the error was detected. */
        uint64_t reserved_1_3          : 3;
        uint64_t fbf                   : 1;  /**< [  0:  0](RO/H) If =1 error was found by the FBF ECC checker, if =0 found by the SBF ECC
                                                                 checker. */
#else /* Word 0 - Little Endian */
        uint64_t fbf                   : 1;  /**< [  0:  0](RO/H) If =1 error was found by the FBF ECC checker, if =0 found by the SBF ECC
                                                                 checker. */
        uint64_t reserved_1_3          : 3;
        uint64_t ow                    : 3;  /**< [  6:  4](RO/H) 128-bit word in which the error was detected. */
        uint64_t addr                  : 36; /**< [ 42:  7](RO/H) Block address of the request which detected the error. This field is the
                                                                 physical address after TAD set aliasing (if enabled, see
                                                                 CCS_ADR_CTL[DISSETALIAS]). */
        uint64_t node                  : 2;  /**< [ 44: 43](RO/H) Reserved. */
        uint64_t nonsec                : 1;  /**< [ 45: 45](RO/H) Nonsecure (NS) bit of request which detected the error. */
        uint64_t ms                    : 6;  /**< [ 51: 46](RO/H) Mesh source of the request which detected the error. */
        uint64_t sid                   : 3;  /**< [ 54: 52](RO/H) If [MS] indicates the source is a cluster, [SID] is the sid of the core within
                                                                 the cluster which issued the request causing the error. If [MS] indicated the
                                                                 source is not a cluster, [SID] is unpredictable. */
        uint64_t scmd                  : 4;  /**< [ 58: 55](RO/H) Sub-command of request which detected the error. */
        uint64_t cmd                   : 3;  /**< [ 61: 59](RO/H) Major command of request which detected the error. */
        uint64_t dbe                   : 1;  /**< [ 62: 62](R/W1C/H) If [DBE]=1, then the logged information is for a MDC DBE error. */
        uint64_t sbe                   : 1;  /**< [ 63: 63](R/W1C/H) If [SBE]=1, then the logged information is for a MDC SBE error. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_ccux_tadx_xbf_err_s cn; */
};
typedef union cavm_ccux_tadx_xbf_err cavm_ccux_tadx_xbf_err_t;

static inline uint64_t CAVM_CCUX_TADX_XBF_ERR(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_CCUX_TADX_XBF_ERR(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN96XX) && ((a<=3) && (b<=1)))
        return 0x87e050400020ll + 0x1000000ll * ((a) & 0x3) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN98XX) && ((a<=5) && (b<=1)))
        return 0x87e050400020ll + 0x1000000ll * ((a) & 0x7) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CNF95XX) && ((a==0) && (b<=1)))
        return 0x87e050400020ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95MM) && ((a==0) && (b<=1)))
        return 0x87e050400020ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_F95O) && ((a==0) && (b<=1)))
        return 0x87e050400020ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_LOKI) && ((a==0) && (b<=1)))
        return 0x87e050400020ll + 0x1000000ll * ((a) & 0x0) + 0x200000ll * ((b) & 0x1);
    __cavm_csr_fatal("CCUX_TADX_XBF_ERR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_CCUX_TADX_XBF_ERR(a,b) cavm_ccux_tadx_xbf_err_t
#define bustype_CAVM_CCUX_TADX_XBF_ERR(a,b) CSR_TYPE_RSL
#define basename_CAVM_CCUX_TADX_XBF_ERR(a,b) "CCUX_TADX_XBF_ERR"
#define device_bar_CAVM_CCUX_TADX_XBF_ERR(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_CCUX_TADX_XBF_ERR(a,b) (a)
#define arguments_CAVM_CCUX_TADX_XBF_ERR(a,b) (a),(b),-1,-1

#endif /* __CAVM_CSRS_CCU_H__ */
