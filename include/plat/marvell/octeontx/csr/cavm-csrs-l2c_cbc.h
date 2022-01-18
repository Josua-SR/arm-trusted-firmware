#ifndef __CAVM_CSRS_L2C_CBC_H__
#define __CAVM_CSRS_L2C_CBC_H__
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
 * OcteonTX L2C_CBC.
 *
 * This file is auto generated. Do not edit.
 *
 */

/**
 * Enumeration l2c_cbc_bar_e
 *
 * L2C CBC Base Address Register Enumeration
 * Enumerates the base address registers.
 */
#define CAVM_L2C_CBC_BAR_E_L2C_CBCX_PF_BAR0(a) (0x87e058000000ll + 0x1000000ll * (a))
#define CAVM_L2C_CBC_BAR_E_L2C_CBCX_PF_BAR0_SIZE 0x800000ull
#define CAVM_L2C_CBC_BAR_E_L2C_CBCX_PF_BAR4(a) (0x87e058f00000ll + 0x1000000ll * (a))
#define CAVM_L2C_CBC_BAR_E_L2C_CBCX_PF_BAR4_SIZE 0x100000ull

/**
 * Enumeration l2c_cbc_int_vec_e
 *
 * L2C CBC MSI-X Vector Enumeration
 * Enumerates the MSI-X interrupt vectors.
 */
#define CAVM_L2C_CBC_INT_VEC_E_INTS (0)

/**
 * Register (RSL) l2c_cbc#_bist_status
 *
 * L2C CBC BIST Status Registers
 */
union cavm_l2c_cbcx_bist_status
{
    uint64_t u;
    struct cavm_l2c_cbcx_bist_status_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_37_63        : 27;
        uint64_t mibfl                 : 5;  /**< [ 36: 32](RO/H) BIST failure status for various MIB memories. ({XMD, IPM, IRM, MXD, MXN}) */
        uint64_t rsdfl                 : 32; /**< [ 31:  0](RO/H) BIST failure status for RSDQW0-31. */
#else /* Word 0 - Little Endian */
        uint64_t rsdfl                 : 32; /**< [ 31:  0](RO/H) BIST failure status for RSDQW0-31. */
        uint64_t mibfl                 : 5;  /**< [ 36: 32](RO/H) BIST failure status for various MIB memories. ({XMD, IPM, IRM, MXD, MXN}) */
        uint64_t reserved_37_63        : 27;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_bist_status_s cn; */
};
typedef union cavm_l2c_cbcx_bist_status cavm_l2c_cbcx_bist_status_t;

static inline uint64_t CAVM_L2C_CBCX_BIST_STATUS(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_BIST_STATUS(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e0580a0000ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e0580a0000ll + 0x1000000ll * ((a) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_BIST_STATUS", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_BIST_STATUS(a) cavm_l2c_cbcx_bist_status_t
#define bustype_CAVM_L2C_CBCX_BIST_STATUS(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_BIST_STATUS(a) "L2C_CBCX_BIST_STATUS"
#define device_bar_CAVM_L2C_CBCX_BIST_STATUS(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_BIST_STATUS(a) (a)
#define arguments_CAVM_L2C_CBCX_BIST_STATUS(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_cbc#_dll
 *
 * L2C CBC DLL Observability Register
 * Register for DLL observability.
 */
union cavm_l2c_cbcx_dll
{
    uint64_t u;
    struct cavm_l2c_cbcx_dll_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_60_63        : 4;
        uint64_t max_dll_setting       : 12; /**< [ 59: 48](RO/H) Max reported DLL setting. */
        uint64_t min_dll_setting       : 12; /**< [ 47: 36](RO/H) Min reported DLL setting. */
        uint64_t pd_pos_rclk_refclk    : 1;  /**< [ 35: 35](RO/H) Phase detector output. */
        uint64_t pdl_rclk_refclk       : 1;  /**< [ 34: 34](RO/H) Phase detector output. */
        uint64_t pdr_rclk_refclk       : 1;  /**< [ 33: 33](RO/H) Phase detector output. */
        uint64_t reserved_32           : 1;
        uint64_t dly_elem_enable       : 16; /**< [ 31: 16](RO/H) Delay element enable. */
        uint64_t dll_setting           : 12; /**< [ 15:  4](RO/H) DLL setting. */
        uint64_t reserved_1_3          : 3;
        uint64_t dll_lock              : 1;  /**< [  0:  0](RO/H) DLL locked. */
#else /* Word 0 - Little Endian */
        uint64_t dll_lock              : 1;  /**< [  0:  0](RO/H) DLL locked. */
        uint64_t reserved_1_3          : 3;
        uint64_t dll_setting           : 12; /**< [ 15:  4](RO/H) DLL setting. */
        uint64_t dly_elem_enable       : 16; /**< [ 31: 16](RO/H) Delay element enable. */
        uint64_t reserved_32           : 1;
        uint64_t pdr_rclk_refclk       : 1;  /**< [ 33: 33](RO/H) Phase detector output. */
        uint64_t pdl_rclk_refclk       : 1;  /**< [ 34: 34](RO/H) Phase detector output. */
        uint64_t pd_pos_rclk_refclk    : 1;  /**< [ 35: 35](RO/H) Phase detector output. */
        uint64_t min_dll_setting       : 12; /**< [ 47: 36](RO/H) Min reported DLL setting. */
        uint64_t max_dll_setting       : 12; /**< [ 59: 48](RO/H) Max reported DLL setting. */
        uint64_t reserved_60_63        : 4;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_dll_s cn; */
};
typedef union cavm_l2c_cbcx_dll cavm_l2c_cbcx_dll_t;

static inline uint64_t CAVM_L2C_CBCX_DLL(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_DLL(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e058040000ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e058040000ll + 0x1000000ll * ((a) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_DLL", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_DLL(a) cavm_l2c_cbcx_dll_t
#define bustype_CAVM_L2C_CBCX_DLL(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_DLL(a) "L2C_CBCX_DLL"
#define device_bar_CAVM_L2C_CBCX_DLL(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_DLL(a) (a)
#define arguments_CAVM_L2C_CBCX_DLL(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_cbc#_int_ena_w1c
 *
 * L2C CBC Interrupt Enable Clear Registers
 * This register clears interrupt enable bits.
 */
union cavm_l2c_cbcx_int_ena_w1c
{
    uint64_t u;
    struct cavm_l2c_cbcx_int_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_9_63         : 55;
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[GSYNCTO].
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[IOWRDISOCI]. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[IORDDISOCI]. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[MIBDBE]. */
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[MIBSBE]. */
        uint64_t reserved_2_3          : 2;
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[RSDDBE]. */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[RSDSBE]. */
#else /* Word 0 - Little Endian */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[RSDSBE]. */
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[RSDDBE]. */
        uint64_t reserved_2_3          : 2;
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[MIBSBE]. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[MIBDBE]. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[IORDDISOCI]. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[IOWRDISOCI]. */
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1C/H) Reads or clears enable for L2C_CBC(0)_INT_W1C[GSYNCTO].
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t reserved_9_63         : 55;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_int_ena_w1c_s cn8; */
    /* struct cavm_l2c_cbcx_int_ena_w1c_s cn81xx; */
    struct cavm_l2c_cbcx_int_ena_w1c_cn83xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_9_63         : 55;
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[GSYNCTO].
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[IOWRDISOCI]. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[IORDDISOCI]. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[MIBDBE]. */
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[MIBSBE]. */
        uint64_t reserved_2_3          : 2;
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[RSDDBE]. */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[RSDSBE]. */
#else /* Word 0 - Little Endian */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[RSDSBE]. */
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[RSDDBE]. */
        uint64_t reserved_2_3          : 2;
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[MIBSBE]. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[MIBDBE]. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[IORDDISOCI]. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[IOWRDISOCI]. */
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1C/H) Reads or clears enable for L2C_CBC(0..1)_INT_W1C[GSYNCTO].
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t reserved_9_63         : 55;
#endif /* Word 0 - End */
    } cn83xx;
};
typedef union cavm_l2c_cbcx_int_ena_w1c cavm_l2c_cbcx_int_ena_w1c_t;

static inline uint64_t CAVM_L2C_CBCX_INT_ENA_W1C(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_INT_ENA_W1C(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e058060020ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e058060020ll + 0x1000000ll * ((a) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_INT_ENA_W1C", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_INT_ENA_W1C(a) cavm_l2c_cbcx_int_ena_w1c_t
#define bustype_CAVM_L2C_CBCX_INT_ENA_W1C(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_INT_ENA_W1C(a) "L2C_CBCX_INT_ENA_W1C"
#define device_bar_CAVM_L2C_CBCX_INT_ENA_W1C(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_INT_ENA_W1C(a) (a)
#define arguments_CAVM_L2C_CBCX_INT_ENA_W1C(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_cbc#_int_ena_w1s
 *
 * L2C CBC Interrupt Enable Set Registers
 * This register sets interrupt enable bits.
 */
union cavm_l2c_cbcx_int_ena_w1s
{
    uint64_t u;
    struct cavm_l2c_cbcx_int_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_9_63         : 55;
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[GSYNCTO].
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[IOWRDISOCI]. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[IORDDISOCI]. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[MIBDBE]. */
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[MIBSBE]. */
        uint64_t reserved_2_3          : 2;
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[RSDDBE]. */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[RSDSBE]. */
#else /* Word 0 - Little Endian */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[RSDSBE]. */
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[RSDDBE]. */
        uint64_t reserved_2_3          : 2;
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[MIBSBE]. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[MIBDBE]. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[IORDDISOCI]. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[IOWRDISOCI]. */
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1S/H) Reads or sets enable for L2C_CBC(0)_INT_W1C[GSYNCTO].
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t reserved_9_63         : 55;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_int_ena_w1s_s cn8; */
    /* struct cavm_l2c_cbcx_int_ena_w1s_s cn81xx; */
    struct cavm_l2c_cbcx_int_ena_w1s_cn83xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_9_63         : 55;
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[GSYNCTO].
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[IOWRDISOCI]. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[IORDDISOCI]. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[MIBDBE]. */
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[MIBSBE]. */
        uint64_t reserved_2_3          : 2;
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[RSDDBE]. */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[RSDSBE]. */
#else /* Word 0 - Little Endian */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[RSDSBE]. */
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[RSDDBE]. */
        uint64_t reserved_2_3          : 2;
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[MIBSBE]. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[MIBDBE]. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[IORDDISOCI]. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[IOWRDISOCI]. */
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1S/H) Reads or sets enable for L2C_CBC(0..1)_INT_W1C[GSYNCTO].
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t reserved_9_63         : 55;
#endif /* Word 0 - End */
    } cn83xx;
};
typedef union cavm_l2c_cbcx_int_ena_w1s cavm_l2c_cbcx_int_ena_w1s_t;

static inline uint64_t CAVM_L2C_CBCX_INT_ENA_W1S(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_INT_ENA_W1S(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e058060028ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e058060028ll + 0x1000000ll * ((a) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_INT_ENA_W1S", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_INT_ENA_W1S(a) cavm_l2c_cbcx_int_ena_w1s_t
#define bustype_CAVM_L2C_CBCX_INT_ENA_W1S(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_INT_ENA_W1S(a) "L2C_CBCX_INT_ENA_W1S"
#define device_bar_CAVM_L2C_CBCX_INT_ENA_W1S(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_INT_ENA_W1S(a) (a)
#define arguments_CAVM_L2C_CBCX_INT_ENA_W1S(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_cbc#_int_w1c
 *
 * L2C CBC Interrupt Registers
 * This register is for CBC-based interrupts.
 */
union cavm_l2c_cbcx_int_w1c
{
    uint64_t u;
    struct cavm_l2c_cbcx_int_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_9_63         : 55;
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1C/H) Global sync timeout. Should never assert, for diagnostic use only.
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1C/H) Illegal I/O write operation to a remote node with L2C_OCI_CTL[ENAOCI][node]
                                                                 clear. See L2C_CBC()_IODISOCIERR for logged information. This interrupt applies
                                                                 to IOBST, IOBSTP, IOBADDR, IASET, IACLR, IAADD, IASWP, IACAS, and LMTST XMC
                                                                 commands.
                                                                 During normal hardware operation, an indication of a software failure and may be
                                                                 considered fatal. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1C/H) Illegal I/O read operation to a remote node with L2C_OCI_CTL[ENAOCI][node]
                                                                 clear. See L2C_CBC()_IODISOCIERR for logged information. This interrupt applies
                                                                 to IOBLD, IASET, IACLR, IAADD, IASWP, and IACAS XMC commands.
                                                                 During normal hardware operation, an indication of a software failure and may be
                                                                 considered fatal. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1C/H) MIB double-bit error occurred. See L2C_CBC()_MIBERR for logged information.
                                                                 An indication of a hardware failure and may be considered fatal. */
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1C/H) MIB single-bit error occurred. See L2C_CBC()_MIBERR for logged
                                                                 information. Hardware corrected the failure. Software may choose to count these
                                                                 single-bit errors. */
        uint64_t reserved_2_3          : 2;
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1C/H) RSD double-bit error occurred. See L2C_CBC()_RSDERR for logged information.
                                                                 An indication of a hardware failure and may be considered fatal. */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1C/H) RSD single-bit error occurred. See L2C_CBC()_RSDERR for logged
                                                                 information. Hardware automatically corrected the error. Software may choose to
                                                                 count the number of these single-bit errors. */
#else /* Word 0 - Little Endian */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1C/H) RSD single-bit error occurred. See L2C_CBC()_RSDERR for logged
                                                                 information. Hardware automatically corrected the error. Software may choose to
                                                                 count the number of these single-bit errors. */
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1C/H) RSD double-bit error occurred. See L2C_CBC()_RSDERR for logged information.
                                                                 An indication of a hardware failure and may be considered fatal. */
        uint64_t reserved_2_3          : 2;
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1C/H) MIB single-bit error occurred. See L2C_CBC()_MIBERR for logged
                                                                 information. Hardware corrected the failure. Software may choose to count these
                                                                 single-bit errors. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1C/H) MIB double-bit error occurred. See L2C_CBC()_MIBERR for logged information.
                                                                 An indication of a hardware failure and may be considered fatal. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1C/H) Illegal I/O read operation to a remote node with L2C_OCI_CTL[ENAOCI][node]
                                                                 clear. See L2C_CBC()_IODISOCIERR for logged information. This interrupt applies
                                                                 to IOBLD, IASET, IACLR, IAADD, IASWP, and IACAS XMC commands.
                                                                 During normal hardware operation, an indication of a software failure and may be
                                                                 considered fatal. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1C/H) Illegal I/O write operation to a remote node with L2C_OCI_CTL[ENAOCI][node]
                                                                 clear. See L2C_CBC()_IODISOCIERR for logged information. This interrupt applies
                                                                 to IOBST, IOBSTP, IOBADDR, IASET, IACLR, IAADD, IASWP, IACAS, and LMTST XMC
                                                                 commands.
                                                                 During normal hardware operation, an indication of a software failure and may be
                                                                 considered fatal. */
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1C/H) Global sync timeout. Should never assert, for diagnostic use only.
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t reserved_9_63         : 55;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_int_w1c_s cn; */
};
typedef union cavm_l2c_cbcx_int_w1c cavm_l2c_cbcx_int_w1c_t;

static inline uint64_t CAVM_L2C_CBCX_INT_W1C(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_INT_W1C(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e058060000ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e058060000ll + 0x1000000ll * ((a) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_INT_W1C", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_INT_W1C(a) cavm_l2c_cbcx_int_w1c_t
#define bustype_CAVM_L2C_CBCX_INT_W1C(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_INT_W1C(a) "L2C_CBCX_INT_W1C"
#define device_bar_CAVM_L2C_CBCX_INT_W1C(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_INT_W1C(a) (a)
#define arguments_CAVM_L2C_CBCX_INT_W1C(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_cbc#_int_w1s
 *
 * L2C CBC Interrupt Set Registers
 * This register sets interrupt bits.
 */
union cavm_l2c_cbcx_int_w1s
{
    uint64_t u;
    struct cavm_l2c_cbcx_int_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_9_63         : 55;
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[GSYNCTO].
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[IOWRDISOCI]. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[IORDDISOCI]. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[MIBDBE]. */
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[MIBSBE]. */
        uint64_t reserved_2_3          : 2;
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[RSDDBE]. */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[RSDSBE]. */
#else /* Word 0 - Little Endian */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[RSDSBE]. */
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[RSDDBE]. */
        uint64_t reserved_2_3          : 2;
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[MIBSBE]. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[MIBDBE]. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[IORDDISOCI]. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[IOWRDISOCI]. */
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1S/H) Reads or sets L2C_CBC(0)_INT_W1C[GSYNCTO].
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t reserved_9_63         : 55;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_int_w1s_s cn8; */
    /* struct cavm_l2c_cbcx_int_w1s_s cn81xx; */
    struct cavm_l2c_cbcx_int_w1s_cn83xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_9_63         : 55;
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[GSYNCTO].
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[IOWRDISOCI]. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[IORDDISOCI]. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[MIBDBE]. */
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[MIBSBE]. */
        uint64_t reserved_2_3          : 2;
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[RSDDBE]. */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[RSDSBE]. */
#else /* Word 0 - Little Endian */
        uint64_t rsdsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[RSDSBE]. */
        uint64_t rsddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[RSDDBE]. */
        uint64_t reserved_2_3          : 2;
        uint64_t mibsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[MIBSBE]. */
        uint64_t mibdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[MIBDBE]. */
        uint64_t iorddisoci            : 1;  /**< [  6:  6](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[IORDDISOCI]. */
        uint64_t iowrdisoci            : 1;  /**< [  7:  7](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[IOWRDISOCI]. */
        uint64_t gsyncto               : 1;  /**< [  8:  8](R/W1S/H) Reads or sets L2C_CBC(0..1)_INT_W1C[GSYNCTO].
                                                                 Internal:
                                                                 The CBC global sync timeout only, so not an OCI timeout. */
        uint64_t reserved_9_63         : 55;
#endif /* Word 0 - End */
    } cn83xx;
};
typedef union cavm_l2c_cbcx_int_w1s cavm_l2c_cbcx_int_w1s_t;

static inline uint64_t CAVM_L2C_CBCX_INT_W1S(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_INT_W1S(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e058060008ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e058060008ll + 0x1000000ll * ((a) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_INT_W1S", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_INT_W1S(a) cavm_l2c_cbcx_int_w1s_t
#define bustype_CAVM_L2C_CBCX_INT_W1S(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_INT_W1S(a) "L2C_CBCX_INT_W1S"
#define device_bar_CAVM_L2C_CBCX_INT_W1S(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_INT_W1S(a) (a)
#define arguments_CAVM_L2C_CBCX_INT_W1S(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_cbc#_inv#_pfc
 *
 * L2C CBC IOC Performance Counter Registers
 */
union cavm_l2c_cbcx_invx_pfc
{
    uint64_t u;
    struct cavm_l2c_cbcx_invx_pfc_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_invx_pfc_s cn; */
};
typedef union cavm_l2c_cbcx_invx_pfc cavm_l2c_cbcx_invx_pfc_t;

static inline uint64_t CAVM_L2C_CBCX_INVX_PFC(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_INVX_PFC(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b==0)))
        return 0x87e058000020ll + 0x1000000ll * ((a) & 0x0) + 0x40ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=1) && (b<=1)))
        return 0x87e058000020ll + 0x1000000ll * ((a) & 0x1) + 0x40ll * ((b) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_INVX_PFC", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_INVX_PFC(a,b) cavm_l2c_cbcx_invx_pfc_t
#define bustype_CAVM_L2C_CBCX_INVX_PFC(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_INVX_PFC(a,b) "L2C_CBCX_INVX_PFC"
#define device_bar_CAVM_L2C_CBCX_INVX_PFC(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_INVX_PFC(a,b) (a)
#define arguments_CAVM_L2C_CBCX_INVX_PFC(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_cbc#_ioc#_pfc
 *
 * L2C CBC IOC Performance Counter Register
 */
union cavm_l2c_cbcx_iocx_pfc
{
    uint64_t u;
    struct cavm_l2c_cbcx_iocx_pfc_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_iocx_pfc_s cn; */
};
typedef union cavm_l2c_cbcx_iocx_pfc cavm_l2c_cbcx_iocx_pfc_t;

static inline uint64_t CAVM_L2C_CBCX_IOCX_PFC(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_IOCX_PFC(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b==0)))
        return 0x87e058000028ll + 0x1000000ll * ((a) & 0x0) + 0x40ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=1) && (b==0)))
        return 0x87e058000028ll + 0x1000000ll * ((a) & 0x1) + 0x40ll * ((b) & 0x0);
    __cavm_csr_fatal("L2C_CBCX_IOCX_PFC", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_IOCX_PFC(a,b) cavm_l2c_cbcx_iocx_pfc_t
#define bustype_CAVM_L2C_CBCX_IOCX_PFC(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_IOCX_PFC(a,b) "L2C_CBCX_IOCX_PFC"
#define device_bar_CAVM_L2C_CBCX_IOCX_PFC(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_IOCX_PFC(a,b) (a)
#define arguments_CAVM_L2C_CBCX_IOCX_PFC(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_cbc#_iocerr
 *
 * L2C CBC Error Information Registers
 * Reserved.
 */
union cavm_l2c_cbcx_iocerr
{
    uint64_t u;
    struct cavm_l2c_cbcx_iocerr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_0_63         : 64;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_63         : 64;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_iocerr_s cn; */
};
typedef union cavm_l2c_cbcx_iocerr cavm_l2c_cbcx_iocerr_t;

static inline uint64_t CAVM_L2C_CBCX_IOCERR(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_IOCERR(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e058080010ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e058080010ll + 0x1000000ll * ((a) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_IOCERR", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_IOCERR(a) cavm_l2c_cbcx_iocerr_t
#define bustype_CAVM_L2C_CBCX_IOCERR(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_IOCERR(a) "L2C_CBCX_IOCERR"
#define device_bar_CAVM_L2C_CBCX_IOCERR(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_IOCERR(a) (a)
#define arguments_CAVM_L2C_CBCX_IOCERR(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_cbc#_iodisocierr
 *
 * L2C CBC IODISOCI Error Information Registers
 * This register records error information associated with IORDDISOCI/IOWRDISOCI interrupts.
 * IOWRDISOCI events take priority over previously captured IORDDISOCI events. Of the available
 * I/O transactions, some commands will either set [IORDDISOCI], set [IOWRDISOCI], or set both
 * [IORDDISOCI] and [IOWRDISOCI]. See L2C_CBC()_INT_W1C for information about which I/O
 * transactions
 * may result in IORDDISOCI/IOWRDISOCI interrupts.
 */
union cavm_l2c_cbcx_iodisocierr
{
    uint64_t u;
    struct cavm_l2c_cbcx_iodisocierr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t iorddisoci            : 1;  /**< [ 63: 63](RO/H) Logged information is for a IORDDISOCI error. */
        uint64_t iowrdisoci            : 1;  /**< [ 62: 62](RO/H) Logged information is for a IOWRDISOCI error. */
        uint64_t reserved_59_61        : 3;
        uint64_t cmd                   : 7;  /**< [ 58: 52](RO/H) Encoding of XMC command.
                                                                 Internal:
                                                                 Enumerated by XMC_CMD_E. */
        uint64_t ppvid                 : 6;  /**< [ 51: 46](RO/H) CMB source PPVID. */
        uint64_t node                  : 2;  /**< [ 45: 44](RO/H) Destination node ID. */
        uint64_t did                   : 8;  /**< [ 43: 36](RO/H) Destination device ID. */
        uint64_t addr                  : 36; /**< [ 35:  0](RO/H) I/O address. */
#else /* Word 0 - Little Endian */
        uint64_t addr                  : 36; /**< [ 35:  0](RO/H) I/O address. */
        uint64_t did                   : 8;  /**< [ 43: 36](RO/H) Destination device ID. */
        uint64_t node                  : 2;  /**< [ 45: 44](RO/H) Destination node ID. */
        uint64_t ppvid                 : 6;  /**< [ 51: 46](RO/H) CMB source PPVID. */
        uint64_t cmd                   : 7;  /**< [ 58: 52](RO/H) Encoding of XMC command.
                                                                 Internal:
                                                                 Enumerated by XMC_CMD_E. */
        uint64_t reserved_59_61        : 3;
        uint64_t iowrdisoci            : 1;  /**< [ 62: 62](RO/H) Logged information is for a IOWRDISOCI error. */
        uint64_t iorddisoci            : 1;  /**< [ 63: 63](RO/H) Logged information is for a IORDDISOCI error. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_iodisocierr_s cn; */
};
typedef union cavm_l2c_cbcx_iodisocierr cavm_l2c_cbcx_iodisocierr_t;

static inline uint64_t CAVM_L2C_CBCX_IODISOCIERR(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_IODISOCIERR(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e058080008ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e058080008ll + 0x1000000ll * ((a) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_IODISOCIERR", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_IODISOCIERR(a) cavm_l2c_cbcx_iodisocierr_t
#define bustype_CAVM_L2C_CBCX_IODISOCIERR(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_IODISOCIERR(a) "L2C_CBCX_IODISOCIERR"
#define device_bar_CAVM_L2C_CBCX_IODISOCIERR(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_IODISOCIERR(a) (a)
#define arguments_CAVM_L2C_CBCX_IODISOCIERR(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_cbc#_ior#_pfc
 *
 * L2C CBC IOR Performance Counter Register
 */
union cavm_l2c_cbcx_iorx_pfc
{
    uint64_t u;
    struct cavm_l2c_cbcx_iorx_pfc_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_iorx_pfc_s cn; */
};
typedef union cavm_l2c_cbcx_iorx_pfc cavm_l2c_cbcx_iorx_pfc_t;

static inline uint64_t CAVM_L2C_CBCX_IORX_PFC(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_IORX_PFC(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b==0)))
        return 0x87e058000030ll + 0x1000000ll * ((a) & 0x0) + 0x40ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=1) && (b==0)))
        return 0x87e058000030ll + 0x1000000ll * ((a) & 0x1) + 0x40ll * ((b) & 0x0);
    __cavm_csr_fatal("L2C_CBCX_IORX_PFC", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_IORX_PFC(a,b) cavm_l2c_cbcx_iorx_pfc_t
#define bustype_CAVM_L2C_CBCX_IORX_PFC(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_IORX_PFC(a,b) "L2C_CBCX_IORX_PFC"
#define device_bar_CAVM_L2C_CBCX_IORX_PFC(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_IORX_PFC(a,b) (a)
#define arguments_CAVM_L2C_CBCX_IORX_PFC(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_cbc#_miberr
 *
 * L2C CBC MIB Error Information Registers
 * This register records error information for all CBC MIB errors. An error locks the INDEX and
 * [SYN] fields and set the bit corresponding to the error received. MIBDBE errors take priority
 * and overwrite an earlier logged MIBSBE error. Only one of [MIBSBE]/[MIBDBE] is set at any
 * given
 * time and serves to document which error the INDEX/[SYN] is associated with. The syndrome is
 * recorded for DBE errors, though the utility of the value is not clear.
 */
union cavm_l2c_cbcx_miberr
{
    uint64_t u;
    struct cavm_l2c_cbcx_miberr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t mibdbe                : 1;  /**< [ 63: 63](RO/H) INDEX/SYN corresponds to a double-bit MIB ECC error. */
        uint64_t mibsbe                : 1;  /**< [ 62: 62](RO/H) INDEX/SYN corresponds to a single-bit MIB ECC error. */
        uint64_t reserved_40_61        : 22;
        uint64_t syn                   : 8;  /**< [ 39: 32](RO/H) Error syndrome. */
        uint64_t reserved_3_31         : 29;
        uint64_t memid                 : 2;  /**< [  2:  1](RO/H) Indicates the memory that had the error.
                                                                 0x0 = Error from MXB_VC_MRN, MXB_VC_MFN, MXB_VC_MPN VCs.
                                                                 0x1 = Error from MXB_VC_MRD, MXB_VC_MPD VCs.
                                                                 0x2 = Error from MXB_VC_IRM VC.
                                                                 0x3 = Error from MXB_VC_IPM VC. */
        uint64_t mibnum                : 1;  /**< [  0:  0](RO/H) Indicates the MIB bus that had the error. */
#else /* Word 0 - Little Endian */
        uint64_t mibnum                : 1;  /**< [  0:  0](RO/H) Indicates the MIB bus that had the error. */
        uint64_t memid                 : 2;  /**< [  2:  1](RO/H) Indicates the memory that had the error.
                                                                 0x0 = Error from MXB_VC_MRN, MXB_VC_MFN, MXB_VC_MPN VCs.
                                                                 0x1 = Error from MXB_VC_MRD, MXB_VC_MPD VCs.
                                                                 0x2 = Error from MXB_VC_IRM VC.
                                                                 0x3 = Error from MXB_VC_IPM VC. */
        uint64_t reserved_3_31         : 29;
        uint64_t syn                   : 8;  /**< [ 39: 32](RO/H) Error syndrome. */
        uint64_t reserved_40_61        : 22;
        uint64_t mibsbe                : 1;  /**< [ 62: 62](RO/H) INDEX/SYN corresponds to a single-bit MIB ECC error. */
        uint64_t mibdbe                : 1;  /**< [ 63: 63](RO/H) INDEX/SYN corresponds to a double-bit MIB ECC error. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_miberr_s cn; */
};
typedef union cavm_l2c_cbcx_miberr cavm_l2c_cbcx_miberr_t;

static inline uint64_t CAVM_L2C_CBCX_MIBERR(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_MIBERR(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e058080020ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e058080020ll + 0x1000000ll * ((a) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_MIBERR", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_MIBERR(a) cavm_l2c_cbcx_miberr_t
#define bustype_CAVM_L2C_CBCX_MIBERR(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_MIBERR(a) "L2C_CBCX_MIBERR"
#define device_bar_CAVM_L2C_CBCX_MIBERR(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_MIBERR(a) (a)
#define arguments_CAVM_L2C_CBCX_MIBERR(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_cbc#_msix_pba#
 *
 * L2C CBC MSI-X Pending Bit Array Registers
 * This register is the MSI-X PBA table; the bit number is indexed by the L2C_CBC_INT_VEC_E
 * enumeration.
 */
union cavm_l2c_cbcx_msix_pbax
{
    uint64_t u;
    struct cavm_l2c_cbcx_msix_pbax_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO) Pending message for the associated L2C_CBC()_MSIX_VEC()_CTL, enumerated by
                                                                 L2C_CBC_INT_VEC_E. Bits
                                                                 that have no associated L2C_CBC_INT_VEC_E are 0. */
#else /* Word 0 - Little Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO) Pending message for the associated L2C_CBC()_MSIX_VEC()_CTL, enumerated by
                                                                 L2C_CBC_INT_VEC_E. Bits
                                                                 that have no associated L2C_CBC_INT_VEC_E are 0. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_msix_pbax_s cn; */
};
typedef union cavm_l2c_cbcx_msix_pbax cavm_l2c_cbcx_msix_pbax_t;

static inline uint64_t CAVM_L2C_CBCX_MSIX_PBAX(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_MSIX_PBAX(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b==0)))
        return 0x87e058ff0000ll + 0x1000000ll * ((a) & 0x0) + 8ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=1) && (b==0)))
        return 0x87e058ff0000ll + 0x1000000ll * ((a) & 0x1) + 8ll * ((b) & 0x0);
    __cavm_csr_fatal("L2C_CBCX_MSIX_PBAX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_MSIX_PBAX(a,b) cavm_l2c_cbcx_msix_pbax_t
#define bustype_CAVM_L2C_CBCX_MSIX_PBAX(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_MSIX_PBAX(a,b) "L2C_CBCX_MSIX_PBAX"
#define device_bar_CAVM_L2C_CBCX_MSIX_PBAX(a,b) 0x4 /* PF_BAR4 */
#define busnum_CAVM_L2C_CBCX_MSIX_PBAX(a,b) (a)
#define arguments_CAVM_L2C_CBCX_MSIX_PBAX(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_cbc#_msix_vec#_addr
 *
 * L2C CBC MSI-X Vector-Table Address Register
 * This register is the MSI-X vector table, indexed by the L2C_CBC_INT_VEC_E enumeration.
 */
union cavm_l2c_cbcx_msix_vecx_addr
{
    uint64_t u;
    struct cavm_l2c_cbcx_msix_vecx_addr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_49_63        : 15;
        uint64_t addr                  : 47; /**< [ 48:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_1            : 1;
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 1 = This vector's L2C_CBC()_MSIX_VEC()_ADDR, L2C_CBC()_MSIX_VEC()_CTL, and corresponding
                                                                 bit of L2C_CBC()_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.

                                                                 If PCCPF_L2C_CBC_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is
                                                                 set, all vectors are secure and function as if [SECVEC] was set. */
#else /* Word 0 - Little Endian */
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 1 = This vector's L2C_CBC()_MSIX_VEC()_ADDR, L2C_CBC()_MSIX_VEC()_CTL, and corresponding
                                                                 bit of L2C_CBC()_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.

                                                                 If PCCPF_L2C_CBC_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is
                                                                 set, all vectors are secure and function as if [SECVEC] was set. */
        uint64_t reserved_1            : 1;
        uint64_t addr                  : 47; /**< [ 48:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_49_63        : 15;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_msix_vecx_addr_s cn; */
};
typedef union cavm_l2c_cbcx_msix_vecx_addr cavm_l2c_cbcx_msix_vecx_addr_t;

static inline uint64_t CAVM_L2C_CBCX_MSIX_VECX_ADDR(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_MSIX_VECX_ADDR(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b==0)))
        return 0x87e058f00000ll + 0x1000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=1) && (b==0)))
        return 0x87e058f00000ll + 0x1000000ll * ((a) & 0x1) + 0x10ll * ((b) & 0x0);
    __cavm_csr_fatal("L2C_CBCX_MSIX_VECX_ADDR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_MSIX_VECX_ADDR(a,b) cavm_l2c_cbcx_msix_vecx_addr_t
#define bustype_CAVM_L2C_CBCX_MSIX_VECX_ADDR(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_MSIX_VECX_ADDR(a,b) "L2C_CBCX_MSIX_VECX_ADDR"
#define device_bar_CAVM_L2C_CBCX_MSIX_VECX_ADDR(a,b) 0x4 /* PF_BAR4 */
#define busnum_CAVM_L2C_CBCX_MSIX_VECX_ADDR(a,b) (a)
#define arguments_CAVM_L2C_CBCX_MSIX_VECX_ADDR(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_cbc#_msix_vec#_ctl
 *
 * L2C CBC MSI-X Vector-Table Control and Data Register
 * This register is the MSI-X vector table, indexed by the L2C_CBC_INT_VEC_E enumeration.
 */
union cavm_l2c_cbcx_msix_vecx_ctl
{
    uint64_t u;
    struct cavm_l2c_cbcx_msix_vecx_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_33_63        : 31;
        uint64_t mask                  : 1;  /**< [ 32: 32](R/W) When set, no MSI-X interrupts are sent to this vector. */
        uint64_t reserved_20_31        : 12;
        uint64_t data                  : 20; /**< [ 19:  0](R/W) Data to use for MSI-X delivery of this vector. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 20; /**< [ 19:  0](R/W) Data to use for MSI-X delivery of this vector. */
        uint64_t reserved_20_31        : 12;
        uint64_t mask                  : 1;  /**< [ 32: 32](R/W) When set, no MSI-X interrupts are sent to this vector. */
        uint64_t reserved_33_63        : 31;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_msix_vecx_ctl_s cn; */
};
typedef union cavm_l2c_cbcx_msix_vecx_ctl cavm_l2c_cbcx_msix_vecx_ctl_t;

static inline uint64_t CAVM_L2C_CBCX_MSIX_VECX_CTL(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_MSIX_VECX_CTL(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b==0)))
        return 0x87e058f00008ll + 0x1000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=1) && (b==0)))
        return 0x87e058f00008ll + 0x1000000ll * ((a) & 0x1) + 0x10ll * ((b) & 0x0);
    __cavm_csr_fatal("L2C_CBCX_MSIX_VECX_CTL", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_MSIX_VECX_CTL(a,b) cavm_l2c_cbcx_msix_vecx_ctl_t
#define bustype_CAVM_L2C_CBCX_MSIX_VECX_CTL(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_MSIX_VECX_CTL(a,b) "L2C_CBCX_MSIX_VECX_CTL"
#define device_bar_CAVM_L2C_CBCX_MSIX_VECX_CTL(a,b) 0x4 /* PF_BAR4 */
#define busnum_CAVM_L2C_CBCX_MSIX_VECX_CTL(a,b) (a)
#define arguments_CAVM_L2C_CBCX_MSIX_VECX_CTL(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_cbc#_rsc#_pfc
 *
 * L2C CBC COMMIT Bus Performance Counter Registers
 */
union cavm_l2c_cbcx_rscx_pfc
{
    uint64_t u;
    struct cavm_l2c_cbcx_rscx_pfc_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_rscx_pfc_s cn; */
};
typedef union cavm_l2c_cbcx_rscx_pfc cavm_l2c_cbcx_rscx_pfc_t;

static inline uint64_t CAVM_L2C_CBCX_RSCX_PFC(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_RSCX_PFC(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b<=1)))
        return 0x87e058000010ll + 0x1000000ll * ((a) & 0x0) + 0x40ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=1) && (b<=2)))
        return 0x87e058000010ll + 0x1000000ll * ((a) & 0x1) + 0x40ll * ((b) & 0x3);
    __cavm_csr_fatal("L2C_CBCX_RSCX_PFC", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_RSCX_PFC(a,b) cavm_l2c_cbcx_rscx_pfc_t
#define bustype_CAVM_L2C_CBCX_RSCX_PFC(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_RSCX_PFC(a,b) "L2C_CBCX_RSCX_PFC"
#define device_bar_CAVM_L2C_CBCX_RSCX_PFC(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_RSCX_PFC(a,b) (a)
#define arguments_CAVM_L2C_CBCX_RSCX_PFC(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_cbc#_rsd#_pfc
 *
 * L2C CBC FILL Bus Performance Counter Registers
 */
union cavm_l2c_cbcx_rsdx_pfc
{
    uint64_t u;
    struct cavm_l2c_cbcx_rsdx_pfc_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_rsdx_pfc_s cn; */
};
typedef union cavm_l2c_cbcx_rsdx_pfc cavm_l2c_cbcx_rsdx_pfc_t;

static inline uint64_t CAVM_L2C_CBCX_RSDX_PFC(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_RSDX_PFC(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b<=1)))
        return 0x87e058000018ll + 0x1000000ll * ((a) & 0x0) + 0x40ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=1) && (b<=2)))
        return 0x87e058000018ll + 0x1000000ll * ((a) & 0x1) + 0x40ll * ((b) & 0x3);
    __cavm_csr_fatal("L2C_CBCX_RSDX_PFC", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_RSDX_PFC(a,b) cavm_l2c_cbcx_rsdx_pfc_t
#define bustype_CAVM_L2C_CBCX_RSDX_PFC(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_RSDX_PFC(a,b) "L2C_CBCX_RSDX_PFC"
#define device_bar_CAVM_L2C_CBCX_RSDX_PFC(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_RSDX_PFC(a,b) (a)
#define arguments_CAVM_L2C_CBCX_RSDX_PFC(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_cbc#_rsderr
 *
 * L2C CBC RSD Error Information Registers
 * This register records error information for all CBC RSD errors.
 * An error locks the INDEX and [SYN] fields and set the bit corresponding to the error received.
 * RSDDBE errors take priority and overwrite an earlier logged RSDSBE error. Only one of
 * [RSDSBE]/[RSDDBE] is set at any given time and serves to document which error the INDEX/[SYN]
 * is
 * associated with.
 * The syndrome is recorded for DBE errors, though the utility of the value is not clear.
 */
union cavm_l2c_cbcx_rsderr
{
    uint64_t u;
    struct cavm_l2c_cbcx_rsderr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t rsddbe                : 1;  /**< [ 63: 63](RO/H) INDEX/SYN corresponds to a double-bit RSD ECC error. */
        uint64_t rsdsbe                : 1;  /**< [ 62: 62](RO/H) INDEX/SYN corresponds to a single-bit RSD ECC error. */
        uint64_t reserved_40_61        : 22;
        uint64_t syn                   : 8;  /**< [ 39: 32](RO/H) Error syndrome. */
        uint64_t reserved_9_31         : 23;
        uint64_t tadnum                : 3;  /**< [  8:  6](RO/H) Indicates the TAD FIFO containing the error. */
        uint64_t qwnum                 : 2;  /**< [  5:  4](RO/H) Indicates the QW containing the error. */
        uint64_t rsdnum                : 4;  /**< [  3:  0](RO/H) Indicates the RSD that had the error. */
#else /* Word 0 - Little Endian */
        uint64_t rsdnum                : 4;  /**< [  3:  0](RO/H) Indicates the RSD that had the error. */
        uint64_t qwnum                 : 2;  /**< [  5:  4](RO/H) Indicates the QW containing the error. */
        uint64_t tadnum                : 3;  /**< [  8:  6](RO/H) Indicates the TAD FIFO containing the error. */
        uint64_t reserved_9_31         : 23;
        uint64_t syn                   : 8;  /**< [ 39: 32](RO/H) Error syndrome. */
        uint64_t reserved_40_61        : 22;
        uint64_t rsdsbe                : 1;  /**< [ 62: 62](RO/H) INDEX/SYN corresponds to a single-bit RSD ECC error. */
        uint64_t rsddbe                : 1;  /**< [ 63: 63](RO/H) INDEX/SYN corresponds to a double-bit RSD ECC error. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_rsderr_s cn; */
};
typedef union cavm_l2c_cbcx_rsderr cavm_l2c_cbcx_rsderr_t;

static inline uint64_t CAVM_L2C_CBCX_RSDERR(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_RSDERR(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e058080018ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e058080018ll + 0x1000000ll * ((a) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_RSDERR", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_RSDERR(a) cavm_l2c_cbcx_rsderr_t
#define bustype_CAVM_L2C_CBCX_RSDERR(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_RSDERR(a) "L2C_CBCX_RSDERR"
#define device_bar_CAVM_L2C_CBCX_RSDERR(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_RSDERR(a) (a)
#define arguments_CAVM_L2C_CBCX_RSDERR(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_cbc#_scratch
 *
 * INTERNAL: L2C CBC General Purpose Scratch Register
 *
 * These registers are only reset by hardware during chip cold reset. The values of the CSR
 * fields in these registers do not change during chip warm or soft resets.
 */
union cavm_l2c_cbcx_scratch
{
    uint64_t u;
    struct cavm_l2c_cbcx_scratch_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_8_63         : 56;
        uint64_t scratch               : 7;  /**< [  7:  1](R/W) General purpose scratch register. */
        uint64_t invdly                : 1;  /**< [  0:  0](R/W) Delays all invalidates for 9 cycles after a broadcast invalidate. */
#else /* Word 0 - Little Endian */
        uint64_t invdly                : 1;  /**< [  0:  0](R/W) Delays all invalidates for 9 cycles after a broadcast invalidate. */
        uint64_t scratch               : 7;  /**< [  7:  1](R/W) General purpose scratch register. */
        uint64_t reserved_8_63         : 56;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_scratch_s cn; */
};
typedef union cavm_l2c_cbcx_scratch cavm_l2c_cbcx_scratch_t;

static inline uint64_t CAVM_L2C_CBCX_SCRATCH(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_SCRATCH(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e0580d0000ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e0580d0000ll + 0x1000000ll * ((a) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_SCRATCH", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_SCRATCH(a) cavm_l2c_cbcx_scratch_t
#define bustype_CAVM_L2C_CBCX_SCRATCH(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_SCRATCH(a) "L2C_CBCX_SCRATCH"
#define device_bar_CAVM_L2C_CBCX_SCRATCH(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_SCRATCH(a) (a)
#define arguments_CAVM_L2C_CBCX_SCRATCH(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_cbc#_xmc#_pfc
 *
 * L2C CBC ADD bus Performance Counter Registers
 */
union cavm_l2c_cbcx_xmcx_pfc
{
    uint64_t u;
    struct cavm_l2c_cbcx_xmcx_pfc_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_xmcx_pfc_s cn; */
};
typedef union cavm_l2c_cbcx_xmcx_pfc cavm_l2c_cbcx_xmcx_pfc_t;

static inline uint64_t CAVM_L2C_CBCX_XMCX_PFC(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_XMCX_PFC(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b<=1)))
        return 0x87e058000000ll + 0x1000000ll * ((a) & 0x0) + 0x40ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=1) && (b<=2)))
        return 0x87e058000000ll + 0x1000000ll * ((a) & 0x1) + 0x40ll * ((b) & 0x3);
    __cavm_csr_fatal("L2C_CBCX_XMCX_PFC", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_XMCX_PFC(a,b) cavm_l2c_cbcx_xmcx_pfc_t
#define bustype_CAVM_L2C_CBCX_XMCX_PFC(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_XMCX_PFC(a,b) "L2C_CBCX_XMCX_PFC"
#define device_bar_CAVM_L2C_CBCX_XMCX_PFC(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_XMCX_PFC(a,b) (a)
#define arguments_CAVM_L2C_CBCX_XMCX_PFC(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_cbc#_xmc_cmd
 *
 * L2C CBC ADD Bus Command Register
 * Note the following:
 *
 * The ADD bus command chosen must not be a IOB-destined command or operation is UNDEFINED.
 *
 * The ADD bus command will have SID forced to IOB, DID forced to L2C, no virtualization checks
 * performed (always pass), and xmdmsk forced to 0. Note that this implies that commands that
 * REQUIRE a STORE cycle (STP, STC, SAA, FAA, FAS) should not be used or the results are
 * unpredictable. The sid = IOB means that the way partitioning used for the command is
 * L2C_WPAR_IOB(). L2C_QOS_PP() are not used for these commands.
 *
 * Any FILL responses generated by the ADD bus command are ignored. Generated STINs, however,
 * will correctly invalidate the required cores.
 *
 * A write that arrives while [INUSE] is set will block until [INUSE] clears. This
 * gives software two options when needing to issue a stream of write operations to L2C_XMC_CMD:
 * polling on [INUSE], or allowing hardware to handle the interlock -- at the expense of
 * locking up the RSL bus for potentially tens of cycles at a time while waiting for an available
 * LFB/VAB entry. Note that when [INUSE] clears, the only ordering it implies is that
 * software can send another ADD bus command. Subsequent commands may complete out of order
 * relative to earlier commands.
 *
 * The address written to L2C_XMC_CMD is a physical address. L2C performs index
 * aliasing (if enabled) on the written address and uses that for the command. This
 * index-aliased address is what is returned on a read of L2C_XMC_CMD.
 */
union cavm_l2c_cbcx_xmc_cmd
{
    uint64_t u;
    struct cavm_l2c_cbcx_xmc_cmd_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t inuse                 : 1;  /**< [ 63: 63](RO/H) Set to 1 by hardware upon receiving a write; cleared when command has issued (not
                                                                 necessarily completed, but ordered relative to other traffic) and hardware can accept
                                                                 another command. */
        uint64_t cmd                   : 7;  /**< [ 62: 56](R/W) Command to use for simulated ADD bus request. A new request can be accepted. */
        uint64_t reserved_49_55        : 7;
        uint64_t nonsec                : 1;  /**< [ 48: 48](R/W) Nonsecure bit to use for simulated ADD bus request. */
        uint64_t reserved_47           : 1;
        uint64_t qos                   : 3;  /**< [ 46: 44](R/W) QOS level to use for simulated ADD bus request. */
        uint64_t reserved_42_43        : 2;
        uint64_t node                  : 2;  /**< [ 41: 40](R/W) CCPI node to use for simulated ADD bus request. */
        uint64_t addr                  : 40; /**< [ 39:  0](R/W) Address to use for simulated ADD bus request. (The address written to
                                                                 L2C_CBC()_XMC_CMD is a physical address. L2C performs index aliasing (if
                                                                 enabled) on the written address and uses that for the command. This
                                                                 index-aliased address is what is returned on a read of L2C_CBC()_XMC_CMD.) */
#else /* Word 0 - Little Endian */
        uint64_t addr                  : 40; /**< [ 39:  0](R/W) Address to use for simulated ADD bus request. (The address written to
                                                                 L2C_CBC()_XMC_CMD is a physical address. L2C performs index aliasing (if
                                                                 enabled) on the written address and uses that for the command. This
                                                                 index-aliased address is what is returned on a read of L2C_CBC()_XMC_CMD.) */
        uint64_t node                  : 2;  /**< [ 41: 40](R/W) CCPI node to use for simulated ADD bus request. */
        uint64_t reserved_42_43        : 2;
        uint64_t qos                   : 3;  /**< [ 46: 44](R/W) QOS level to use for simulated ADD bus request. */
        uint64_t reserved_47           : 1;
        uint64_t nonsec                : 1;  /**< [ 48: 48](R/W) Nonsecure bit to use for simulated ADD bus request. */
        uint64_t reserved_49_55        : 7;
        uint64_t cmd                   : 7;  /**< [ 62: 56](R/W) Command to use for simulated ADD bus request. A new request can be accepted. */
        uint64_t inuse                 : 1;  /**< [ 63: 63](RO/H) Set to 1 by hardware upon receiving a write; cleared when command has issued (not
                                                                 necessarily completed, but ordered relative to other traffic) and hardware can accept
                                                                 another command. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_xmc_cmd_s cn; */
};
typedef union cavm_l2c_cbcx_xmc_cmd cavm_l2c_cbcx_xmc_cmd_t;

static inline uint64_t CAVM_L2C_CBCX_XMC_CMD(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_XMC_CMD(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e0580c0000ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=1))
        return 0x87e0580c0000ll + 0x1000000ll * ((a) & 0x1);
    __cavm_csr_fatal("L2C_CBCX_XMC_CMD", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_XMC_CMD(a) cavm_l2c_cbcx_xmc_cmd_t
#define bustype_CAVM_L2C_CBCX_XMC_CMD(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_XMC_CMD(a) "L2C_CBCX_XMC_CMD"
#define device_bar_CAVM_L2C_CBCX_XMC_CMD(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_XMC_CMD(a) (a)
#define arguments_CAVM_L2C_CBCX_XMC_CMD(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_cbc#_xmd#_pfc
 *
 * L2C CBC STORE bus Performance Counter Registers
 */
union cavm_l2c_cbcx_xmdx_pfc
{
    uint64_t u;
    struct cavm_l2c_cbcx_xmdx_pfc_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_cbcx_xmdx_pfc_s cn; */
};
typedef union cavm_l2c_cbcx_xmdx_pfc cavm_l2c_cbcx_xmdx_pfc_t;

static inline uint64_t CAVM_L2C_CBCX_XMDX_PFC(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CBCX_XMDX_PFC(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b<=1)))
        return 0x87e058000008ll + 0x1000000ll * ((a) & 0x0) + 0x40ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=1) && (b<=2)))
        return 0x87e058000008ll + 0x1000000ll * ((a) & 0x1) + 0x40ll * ((b) & 0x3);
    __cavm_csr_fatal("L2C_CBCX_XMDX_PFC", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CBCX_XMDX_PFC(a,b) cavm_l2c_cbcx_xmdx_pfc_t
#define bustype_CAVM_L2C_CBCX_XMDX_PFC(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_CBCX_XMDX_PFC(a,b) "L2C_CBCX_XMDX_PFC"
#define device_bar_CAVM_L2C_CBCX_XMDX_PFC(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CBCX_XMDX_PFC(a,b) (a)
#define arguments_CAVM_L2C_CBCX_XMDX_PFC(a,b) (a),(b),-1,-1

#endif /* __CAVM_CSRS_L2C_CBC_H__ */
