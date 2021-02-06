#ifndef __CAVM_CSRS_L2C_TAD_H__
#define __CAVM_CSRS_L2C_TAD_H__
/* This file is auto-generated. Do not edit */

/***********************license start***********************************
* Copyright (C) 2018-2021 Marvell
* SPDX-License-Identifier: BSD-3-Clause
* https://spdx.org/licenses
***********************license end**************************************/


/**
 * @file
 *
 * Configuration and status register (CSR) address and type definitions for
 * OcteonTX L2C_TAD.
 *
 * This file is auto generated. Do not edit.
 *
 */

/**
 * Enumeration l2c_tad_bar_e
 *
 * L2C TAD Base Address Register Enumeration
 * Enumerates the base address registers.
 */
#define CAVM_L2C_TAD_BAR_E_L2C_TADX_PF_BAR0(a) (0x87e050000000ll + 0x1000000ll * (a))
#define CAVM_L2C_TAD_BAR_E_L2C_TADX_PF_BAR0_SIZE 0x800000ull
#define CAVM_L2C_TAD_BAR_E_L2C_TADX_PF_BAR4(a) (0x87e050f00000ll + 0x1000000ll * (a))
#define CAVM_L2C_TAD_BAR_E_L2C_TADX_PF_BAR4_SIZE 0x100000ull

/**
 * Enumeration l2c_tad_int_vec_e
 *
 * L2C TAD MSI-X Vector Enumeration
 * Enumerates the MSI-X interrupt vectors.
 */
#define CAVM_L2C_TAD_INT_VEC_E_INTS (0)

/**
 * Register (RSL) l2c_tad#_dll
 *
 * L2C TAD DLL Observability Register
 * This register provides the parameters for DLL observability.
 */
union cavm_l2c_tadx_dll
{
    uint64_t u;
    struct cavm_l2c_tadx_dll_s
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
        uint64_t dll_lock              : 1;  /**< [  0:  0](RO/H) DLL lock: 1 = locked, 0 = unlocked. */
#else /* Word 0 - Little Endian */
        uint64_t dll_lock              : 1;  /**< [  0:  0](RO/H) DLL lock: 1 = locked, 0 = unlocked. */
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
    /* struct cavm_l2c_tadx_dll_s cn; */
};
typedef union cavm_l2c_tadx_dll cavm_l2c_tadx_dll_t;

static inline uint64_t CAVM_L2C_TADX_DLL(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_DLL(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050030000ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050030000ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_DLL", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_DLL(a) cavm_l2c_tadx_dll_t
#define bustype_CAVM_L2C_TADX_DLL(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_DLL(a) "L2C_TADX_DLL"
#define device_bar_CAVM_L2C_TADX_DLL(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_DLL(a) (a)
#define arguments_CAVM_L2C_TADX_DLL(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_err
 *
 * L2C TAD Request Error Info Registers
 * This register records error information for *DISOCI and *NXM
 * interrupts. The NXM logic only applies to local addresses. A command for
 * a remote address does not cause a [RDNXM]/[WRNXM] on the requesting node, but
 * may on the remote node. Similarly, [RDDISOCI]/[WRDISOCI] is always for a remote
 * address. The first [WRDISOCI]/[WRNXM] error will lock the register until the
 * logged error type is cleared; [RDDISOCI]/[RDNXM] never locks the register.
 */
union cavm_l2c_tadx_err
{
    uint64_t u;
    struct cavm_l2c_tadx_err_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t rdnxm                 : 1;  /**< [ 63: 63](RO/H) Logged information is for a L2C_TAD()_INT_W1C[RDNXM] error. */
        uint64_t wrnxm                 : 1;  /**< [ 62: 62](RO/H) Logged information is for a L2C_TAD()_INT_W1C[WRNXM] error. */
        uint64_t rddisoci              : 1;  /**< [ 61: 61](RO/H) Logged information is for a L2C_TAD()_INT_W1C[RDDISOCI] error. */
        uint64_t wrdisoci              : 1;  /**< [ 60: 60](RO/H) Logged information is for a L2C_TAD()_INT_W1C[WRDISOCI] error. */
        uint64_t nonsec                : 1;  /**< [ 59: 59](RO/H) Nonsecure (NS) bit of request causing error. */
        uint64_t cmd                   : 8;  /**< [ 58: 51](RO/H) Encoding of XMC or CCPI command causing error.
                                                                 Internal:
                                                                 If CMD\<7\>==1, use XMC_CMD_E to
                                                                 decode CMD\<6:0\>. If CMD\<7:5\>==0, use OCI_MREQ_CMD_E to decode CMD\<4:0\>. If CMD\<7:5\>==1,
                                                                 use OCI_MFWD_CMD_E to decode CMD\<4:0\>. If CMD\<7:5\>==2, use OCI_MRSP_CMD_E to decode
                                                                 CMD\<4:0\>. */
        uint64_t source                : 7;  /**< [ 50: 44](RO/H) XMC source of request causing error. If [SOURCE]\<6\>==0, then [SOURCE]\<5:0\> is
                                                                 PPID, else [SOURCE]\<3:0\> is BUSID of the IOB which made the request. If
                                                                 [CMD]\<7\>==0, this field is unpredictable. */
        uint64_t reserved_42_43        : 2;
        uint64_t node                  : 2;  /**< [ 41: 40](RO/H) CCPI node of XMC request causing error. For *NXM errors [NODE] is always the node that
                                                                 generated request causing the error (*NXM errors are logged at the home node). For *DISOCI
                                                                 errors, is the NODE the request is directed to (DISOCI request is always the current
                                                                 node). */
        uint64_t addr                  : 40; /**< [ 39:  0](RO/H) XMC address causing the error. [ADDR]\<6:0\> is unpredictable for *DISOCI and *NXM
                                                                 errors. This field is the physical address after index aliasing (if enabled). */
#else /* Word 0 - Little Endian */
        uint64_t addr                  : 40; /**< [ 39:  0](RO/H) XMC address causing the error. [ADDR]\<6:0\> is unpredictable for *DISOCI and *NXM
                                                                 errors. This field is the physical address after index aliasing (if enabled). */
        uint64_t node                  : 2;  /**< [ 41: 40](RO/H) CCPI node of XMC request causing error. For *NXM errors [NODE] is always the node that
                                                                 generated request causing the error (*NXM errors are logged at the home node). For *DISOCI
                                                                 errors, is the NODE the request is directed to (DISOCI request is always the current
                                                                 node). */
        uint64_t reserved_42_43        : 2;
        uint64_t source                : 7;  /**< [ 50: 44](RO/H) XMC source of request causing error. If [SOURCE]\<6\>==0, then [SOURCE]\<5:0\> is
                                                                 PPID, else [SOURCE]\<3:0\> is BUSID of the IOB which made the request. If
                                                                 [CMD]\<7\>==0, this field is unpredictable. */
        uint64_t cmd                   : 8;  /**< [ 58: 51](RO/H) Encoding of XMC or CCPI command causing error.
                                                                 Internal:
                                                                 If CMD\<7\>==1, use XMC_CMD_E to
                                                                 decode CMD\<6:0\>. If CMD\<7:5\>==0, use OCI_MREQ_CMD_E to decode CMD\<4:0\>. If CMD\<7:5\>==1,
                                                                 use OCI_MFWD_CMD_E to decode CMD\<4:0\>. If CMD\<7:5\>==2, use OCI_MRSP_CMD_E to decode
                                                                 CMD\<4:0\>. */
        uint64_t nonsec                : 1;  /**< [ 59: 59](RO/H) Nonsecure (NS) bit of request causing error. */
        uint64_t wrdisoci              : 1;  /**< [ 60: 60](RO/H) Logged information is for a L2C_TAD()_INT_W1C[WRDISOCI] error. */
        uint64_t rddisoci              : 1;  /**< [ 61: 61](RO/H) Logged information is for a L2C_TAD()_INT_W1C[RDDISOCI] error. */
        uint64_t wrnxm                 : 1;  /**< [ 62: 62](RO/H) Logged information is for a L2C_TAD()_INT_W1C[WRNXM] error. */
        uint64_t rdnxm                 : 1;  /**< [ 63: 63](RO/H) Logged information is for a L2C_TAD()_INT_W1C[RDNXM] error. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_err_s cn; */
};
typedef union cavm_l2c_tadx_err cavm_l2c_tadx_err_t;

static inline uint64_t CAVM_L2C_TADX_ERR(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_ERR(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050060000ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050060000ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_ERR", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_ERR(a) cavm_l2c_tadx_err_t
#define bustype_CAVM_L2C_TADX_ERR(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_ERR(a) "L2C_TADX_ERR"
#define device_bar_CAVM_L2C_TADX_ERR(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_ERR(a) (a)
#define arguments_CAVM_L2C_TADX_ERR(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_int_ena_w1c
 *
 * L2C TAD Interrupt Enable Clear Registers
 * This register clears interrupt enable bits.
 */
union cavm_l2c_tadx_int_ena_w1c
{
    uint64_t u;
    struct cavm_l2c_tadx_int_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[WRDISOCI]. */
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[RDDISOCI]. */
        uint64_t reserved_19_33        : 15;
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[GSYNCTO]. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[LFBTO]. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[WRDISLMC]. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[RDDISLMC]. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[WRNXM]. */
        uint64_t reserved_11_12        : 2;
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[NOWAY]. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[TAGDBE]. */
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[TAGSBE]. */
        uint64_t reserved_6_7          : 2;
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[FBFDBE]. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[FBFSBE]. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[SBFDBE]. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[SBFSBE]. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[L2DDBE]. */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[L2DSBE]. */
#else /* Word 0 - Little Endian */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[L2DSBE]. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[L2DDBE]. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[SBFSBE]. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[SBFDBE]. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[FBFSBE]. */
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[FBFDBE]. */
        uint64_t reserved_6_7          : 2;
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[TAGSBE]. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[TAGDBE]. */
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[NOWAY]. */
        uint64_t reserved_11_12        : 2;
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[RDNXM]. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[RDDISLMC]. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[WRDISLMC]. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[LFBTO]. */
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[GSYNCTO]. */
        uint64_t reserved_19_33        : 15;
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[RDDISOCI]. */
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1C/H) Reads or clears enable for L2C_TAD(0)_INT_W1C[WRDISOCI]. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_int_ena_w1c_s cn8; */
    /* struct cavm_l2c_tadx_int_ena_w1c_s cn81xx; */
    struct cavm_l2c_tadx_int_ena_w1c_cn83xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[WRDISOCI]. */
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[RDDISOCI]. */
        uint64_t reserved_19_33        : 15;
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[GSYNCTO]. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[LFBTO]. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[WRDISLMC]. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[RDDISLMC]. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[WRNXM]. */
        uint64_t reserved_11_12        : 2;
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[NOWAY]. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[TAGDBE]. */
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[TAGSBE]. */
        uint64_t reserved_6_7          : 2;
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[FBFDBE]. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[FBFSBE]. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[SBFDBE]. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[SBFSBE]. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[L2DDBE]. */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[L2DSBE]. */
#else /* Word 0 - Little Endian */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[L2DSBE]. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[L2DDBE]. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[SBFSBE]. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[SBFDBE]. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[FBFSBE]. */
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[FBFDBE]. */
        uint64_t reserved_6_7          : 2;
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[TAGSBE]. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[TAGDBE]. */
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[NOWAY]. */
        uint64_t reserved_11_12        : 2;
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[RDNXM]. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[RDDISLMC]. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[WRDISLMC]. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[LFBTO]. */
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[GSYNCTO]. */
        uint64_t reserved_19_33        : 15;
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[RDDISOCI]. */
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1C/H) Reads or clears enable for L2C_TAD(0..3)_INT_W1C[WRDISOCI]. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } cn83xx;
};
typedef union cavm_l2c_tadx_int_ena_w1c cavm_l2c_tadx_int_ena_w1c_t;

static inline uint64_t CAVM_L2C_TADX_INT_ENA_W1C(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_INT_ENA_W1C(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050040020ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050040020ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_INT_ENA_W1C", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_INT_ENA_W1C(a) cavm_l2c_tadx_int_ena_w1c_t
#define bustype_CAVM_L2C_TADX_INT_ENA_W1C(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_INT_ENA_W1C(a) "L2C_TADX_INT_ENA_W1C"
#define device_bar_CAVM_L2C_TADX_INT_ENA_W1C(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_INT_ENA_W1C(a) (a)
#define arguments_CAVM_L2C_TADX_INT_ENA_W1C(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_int_ena_w1s
 *
 * L2C TAD Interrupt Enable Set Registers
 * This register sets interrupt enable bits.
 */
union cavm_l2c_tadx_int_ena_w1s
{
    uint64_t u;
    struct cavm_l2c_tadx_int_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[WRDISOCI]. */
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[RDDISOCI]. */
        uint64_t reserved_19_33        : 15;
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[GSYNCTO]. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[LFBTO]. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[WRDISLMC]. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[RDDISLMC]. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[WRNXM]. */
        uint64_t reserved_11_12        : 2;
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[NOWAY]. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[TAGDBE]. */
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[TAGSBE]. */
        uint64_t reserved_6_7          : 2;
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[FBFDBE]. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[FBFSBE]. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[SBFDBE]. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[SBFSBE]. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[L2DDBE]. */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[L2DSBE]. */
#else /* Word 0 - Little Endian */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[L2DSBE]. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[L2DDBE]. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[SBFSBE]. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[SBFDBE]. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[FBFSBE]. */
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[FBFDBE]. */
        uint64_t reserved_6_7          : 2;
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[TAGSBE]. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[TAGDBE]. */
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[NOWAY]. */
        uint64_t reserved_11_12        : 2;
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[RDNXM]. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[RDDISLMC]. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[WRDISLMC]. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[LFBTO]. */
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[GSYNCTO]. */
        uint64_t reserved_19_33        : 15;
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[RDDISOCI]. */
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1S/H) Reads or sets enable for L2C_TAD(0)_INT_W1C[WRDISOCI]. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_int_ena_w1s_s cn8; */
    /* struct cavm_l2c_tadx_int_ena_w1s_s cn81xx; */
    struct cavm_l2c_tadx_int_ena_w1s_cn83xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[WRDISOCI]. */
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[RDDISOCI]. */
        uint64_t reserved_19_33        : 15;
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[GSYNCTO]. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[LFBTO]. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[WRDISLMC]. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[RDDISLMC]. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[WRNXM]. */
        uint64_t reserved_11_12        : 2;
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[NOWAY]. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[TAGDBE]. */
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[TAGSBE]. */
        uint64_t reserved_6_7          : 2;
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[FBFDBE]. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[FBFSBE]. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[SBFDBE]. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[SBFSBE]. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[L2DDBE]. */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[L2DSBE]. */
#else /* Word 0 - Little Endian */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[L2DSBE]. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[L2DDBE]. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[SBFSBE]. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[SBFDBE]. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[FBFSBE]. */
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[FBFDBE]. */
        uint64_t reserved_6_7          : 2;
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[TAGSBE]. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[TAGDBE]. */
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[NOWAY]. */
        uint64_t reserved_11_12        : 2;
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[RDNXM]. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[RDDISLMC]. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[WRDISLMC]. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[LFBTO]. */
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[GSYNCTO]. */
        uint64_t reserved_19_33        : 15;
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[RDDISOCI]. */
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1S/H) Reads or sets enable for L2C_TAD(0..3)_INT_W1C[WRDISOCI]. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } cn83xx;
};
typedef union cavm_l2c_tadx_int_ena_w1s cavm_l2c_tadx_int_ena_w1s_t;

static inline uint64_t CAVM_L2C_TADX_INT_ENA_W1S(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_INT_ENA_W1S(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050040028ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050040028ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_INT_ENA_W1S", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_INT_ENA_W1S(a) cavm_l2c_tadx_int_ena_w1s_t
#define bustype_CAVM_L2C_TADX_INT_ENA_W1S(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_INT_ENA_W1S(a) "L2C_TADX_INT_ENA_W1S"
#define device_bar_CAVM_L2C_TADX_INT_ENA_W1S(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_INT_ENA_W1S(a) (a)
#define arguments_CAVM_L2C_TADX_INT_ENA_W1S(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_int_w1c
 *
 * L2C TAD Interrupt Registers
 * This register is for TAD-based interrupts.
 */
union cavm_l2c_tadx_int_w1c
{
    uint64_t u;
    struct cavm_l2c_tadx_int_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1C/H) Illegal write operation to a remote node with L2C_OCI_CTL[ENAOCI][node] clear. See
                                                                 L2C_TAD()_ERR for logged information.
                                                                 During normal hardware operation, an indication of a software failure and may be
                                                                 considered fatal. */
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1C/H) Illegal read operation to a remote node with L2C_OCI_CTL[ENAOCI][node]
                                                                 clear. Note [RDDISOCI] interrupts can occur during normal operation as the cores
                                                                 are allowed to prefetch to nonexistent memory locations. Therefore, [RDDISOCI]
                                                                 is for informational purposes only. See L2C_TAD()_ERR for logged information. */
        uint64_t reserved_19_33        : 15;
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1C/H) Global sync timeout. Should not occur during normal operation. This may be an
                                                                 indication of hardware failure, and may be considered fatal. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1C/H) An LFB entry (or more) has encountered a timeout condition When [LFBTO] timeout
                                                                 condition occurs L2C_TAD()_TIMEOUT is loaded. L2C_TAD()_TIMEOUT is loaded with
                                                                 info from the first LFB that timed out. if multiple LFB timed out
                                                                 simultaneously, then the it will capture info from the lowest LFB number that
                                                                 timed out.
                                                                 Should not occur during normal operation.  OCI/CCPI link failures may cause this
                                                                 failure. This may be an indication of hardware failure, and may be considered
                                                                 fatal. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1C/H) Illegal write to disabled LMC error. A DRAM write arrived before LMC was enabled.
                                                                 Should not occur during normal operation.
                                                                 This may be considered fatal. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1C/H) Illegal read to disabled LMC error. A DRAM read arrived before LMC was enabled.
                                                                 Should not occur during normal operation.
                                                                 This may be considered fatal. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1C/H) Read reference outside all the defined and enabled address space
                                                                 control (ASC) regions, or secure read reference to an ASC region
                                                                 not enabled for secure access, or nonsecure read reference to an ASC
                                                                 region not enabled for nonsecure access.
                                                                 [RDNXM] interrupts can occur during normal operation as the cores are
                                                                 allowed to prefetch to nonexistent memory locations.  Therefore,
                                                                 [RDNXM] is for informational purposes only.
                                                                 See L2C_TAD()_ERR for logged information.
                                                                 See L2C_ASC_REGION()_START, L2C_ASC_REGION()_END, and
                                                                 L2C_ASC_REGION()_ATTR for ASC region specification. */
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1C/H) Write reference outside all the defined and enabled address space
                                                                 control (ASC) regions, or secure write reference to an ASC region
                                                                 not enabled for secure access, or nonsecure write reference to an
                                                                 ASC region not enabled for nonsecure access.
                                                                 This may be an indication of software
                                                                 failure, and may be considered fatal.
                                                                 See L2C_TAD()_ERR for logged information.
                                                                 See L2C_ASC_REGION()_START, L2C_ASC_REGION()_END, and
                                                                 L2C_ASC_REGION()_ATTR for ASC region specification. */
        uint64_t reserved_11_12        : 2;
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1C/H) No way was available for allocation. L2C sets [NOWAY] during its processing of a
                                                                 transaction whenever it needed/wanted to allocate a WAY in the L2 cache, but was
                                                                 unable to. When this bit = 1, it is (generally) not an indication that L2C
                                                                 failed to complete transactions. Rather, it is a hint of possible performance
                                                                 degradation. (For example, L2C must read- modify-write DRAM for every
                                                                 transaction that updates some, but not all, of the bytes in a cache block,
                                                                 misses in the L2 cache, and cannot allocate a WAY.) There is one 'failure' case
                                                                 where L2C sets [NOWAY]: when it cannot leave a block locked in the L2 cache as
                                                                 part of a LCKL2 transaction. See L2C_TTG()_ERR for logged information. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1C/H) TAG double-bit error occurred. See L2C_TTG()_ERR for logged information.
                                                                 This is an indication of a hardware failure and may be considered fatal. */
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1C/H) TAG single-bit error on a read. See L2C_TAD()_TTG_ERR for logged
                                                                 information. When [TAGSBE] is set, hardware corrected the error before using the
                                                                 tag, but did not correct any stored value. When [TAGSBE] is set, software should
                                                                 eject the TAG location indicated by the corresponding
                                                                 L2C_TAD()_TTG_ERR[WAY,L2IDX] (via a SYS CVMCACHEWBIL2I instruction below)
                                                                 before clearing [TAGSBE]. Otherwise, hardware may encounter the error again the
                                                                 next time the same TAG location is referenced. Software may also choose to count
                                                                 the number of these single-bit errors.

                                                                 The SYS CVMCACHEWBIL2I instruction payload should have:
                                                                   \<pre\>
                                                                   payload\<24\> = 0
                                                                   payload\<23:20\> = L2C_TAD()_TTG_ERR[WAY]
                                                                   payload\<19:7\>  = L2C_TAD()_TTG_ERR[L2IDX]
                                                                   \</pre\>
                                                                 Note that L2C_CTL[DISIDXALIAS] has no effect on this payload. */
        uint64_t reserved_6_7          : 2;
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1C/H) FBF double-bit error occurred. See L2C_TAD()_TQD_ERR for logged information. An
                                                                 indication of a hardware failure and may be considered fatal. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1C/H) FBF single-bit error on a read. See L2C_TAD()_TQD_ERR for logged
                                                                 information. Hardware automatically corrected the error. Software may choose to
                                                                 count the number of these single-bit errors. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1C/H) SBF double-bit error occurred. See L2C_TAD()_TQD_ERR for logged information. An
                                                                 indication of a hardware failure and may be considered fatal. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1C/H) SBF single-bit error on a read. See L2C_TAD()_TQD_ERR for logged
                                                                 information. Hardware automatically corrected the error. Software may choose to
                                                                 count the number of these single-bit errors. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1C/H) L2D double-bit error occurred. See L2C_TAD()_TQD_ERR for logged information. An
                                                                 indication of a hardware failure and may be considered fatal. */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1C/H) L2D single-bit error on a read. See L2C_TAD()_TQD_ERR for logged
                                                                 information. When [L2DSBE] is set, hardware corrected the error before using the
                                                                 data, but did not correct any stored value. When [L2DSBE] is set, software
                                                                 should eject the cache block indicated by the corresponding
                                                                 L2C_TAD()_TQD_ERR[QDNUM,L2DIDX] (via a SYS CVMCACHEWBIL2I instruction below)
                                                                 before clearing [L2DSBE]. Otherwise, hardware may encounter the error again the
                                                                 next time the same L2D location is referenced. Software may also choose to count
                                                                 the number of these single-bit errors.

                                                                 The SYS CVMCACHEWBIL2I instruction payload should have:
                                                                 \<pre\>
                                                                   payload\<24:21\> = 0
                                                                   payload\<20:17\> = L2C_TAD()_TQD_ERR[L2DIDX]\<10:7\>  // way
                                                                   payload\<16:10\> = L2C_TAD()_TQD_ERR[L2DIDX]\<6:0\>   // index\<9:3\>
                                                                   payload\<9:8\>   = L2C_TAD()_TQD_ERR[L2DIDX]\<12:11\> // index\<2:1\>
                                                                   payload\<7\>     = L2C_TAD()_TQD_ERR[QDNUM]\<2\>      // index\<0\>
                                                                 \</pre\>

                                                                 Note that L2C_CTL[DISIDXALIAS] has no effect on the payload. */
#else /* Word 0 - Little Endian */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1C/H) L2D single-bit error on a read. See L2C_TAD()_TQD_ERR for logged
                                                                 information. When [L2DSBE] is set, hardware corrected the error before using the
                                                                 data, but did not correct any stored value. When [L2DSBE] is set, software
                                                                 should eject the cache block indicated by the corresponding
                                                                 L2C_TAD()_TQD_ERR[QDNUM,L2DIDX] (via a SYS CVMCACHEWBIL2I instruction below)
                                                                 before clearing [L2DSBE]. Otherwise, hardware may encounter the error again the
                                                                 next time the same L2D location is referenced. Software may also choose to count
                                                                 the number of these single-bit errors.

                                                                 The SYS CVMCACHEWBIL2I instruction payload should have:
                                                                 \<pre\>
                                                                   payload\<24:21\> = 0
                                                                   payload\<20:17\> = L2C_TAD()_TQD_ERR[L2DIDX]\<10:7\>  // way
                                                                   payload\<16:10\> = L2C_TAD()_TQD_ERR[L2DIDX]\<6:0\>   // index\<9:3\>
                                                                   payload\<9:8\>   = L2C_TAD()_TQD_ERR[L2DIDX]\<12:11\> // index\<2:1\>
                                                                   payload\<7\>     = L2C_TAD()_TQD_ERR[QDNUM]\<2\>      // index\<0\>
                                                                 \</pre\>

                                                                 Note that L2C_CTL[DISIDXALIAS] has no effect on the payload. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1C/H) L2D double-bit error occurred. See L2C_TAD()_TQD_ERR for logged information. An
                                                                 indication of a hardware failure and may be considered fatal. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1C/H) SBF single-bit error on a read. See L2C_TAD()_TQD_ERR for logged
                                                                 information. Hardware automatically corrected the error. Software may choose to
                                                                 count the number of these single-bit errors. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1C/H) SBF double-bit error occurred. See L2C_TAD()_TQD_ERR for logged information. An
                                                                 indication of a hardware failure and may be considered fatal. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1C/H) FBF single-bit error on a read. See L2C_TAD()_TQD_ERR for logged
                                                                 information. Hardware automatically corrected the error. Software may choose to
                                                                 count the number of these single-bit errors. */
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1C/H) FBF double-bit error occurred. See L2C_TAD()_TQD_ERR for logged information. An
                                                                 indication of a hardware failure and may be considered fatal. */
        uint64_t reserved_6_7          : 2;
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1C/H) TAG single-bit error on a read. See L2C_TAD()_TTG_ERR for logged
                                                                 information. When [TAGSBE] is set, hardware corrected the error before using the
                                                                 tag, but did not correct any stored value. When [TAGSBE] is set, software should
                                                                 eject the TAG location indicated by the corresponding
                                                                 L2C_TAD()_TTG_ERR[WAY,L2IDX] (via a SYS CVMCACHEWBIL2I instruction below)
                                                                 before clearing [TAGSBE]. Otherwise, hardware may encounter the error again the
                                                                 next time the same TAG location is referenced. Software may also choose to count
                                                                 the number of these single-bit errors.

                                                                 The SYS CVMCACHEWBIL2I instruction payload should have:
                                                                   \<pre\>
                                                                   payload\<24\> = 0
                                                                   payload\<23:20\> = L2C_TAD()_TTG_ERR[WAY]
                                                                   payload\<19:7\>  = L2C_TAD()_TTG_ERR[L2IDX]
                                                                   \</pre\>
                                                                 Note that L2C_CTL[DISIDXALIAS] has no effect on this payload. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1C/H) TAG double-bit error occurred. See L2C_TTG()_ERR for logged information.
                                                                 This is an indication of a hardware failure and may be considered fatal. */
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1C/H) No way was available for allocation. L2C sets [NOWAY] during its processing of a
                                                                 transaction whenever it needed/wanted to allocate a WAY in the L2 cache, but was
                                                                 unable to. When this bit = 1, it is (generally) not an indication that L2C
                                                                 failed to complete transactions. Rather, it is a hint of possible performance
                                                                 degradation. (For example, L2C must read- modify-write DRAM for every
                                                                 transaction that updates some, but not all, of the bytes in a cache block,
                                                                 misses in the L2 cache, and cannot allocate a WAY.) There is one 'failure' case
                                                                 where L2C sets [NOWAY]: when it cannot leave a block locked in the L2 cache as
                                                                 part of a LCKL2 transaction. See L2C_TTG()_ERR for logged information. */
        uint64_t reserved_11_12        : 2;
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1C/H) Write reference outside all the defined and enabled address space
                                                                 control (ASC) regions, or secure write reference to an ASC region
                                                                 not enabled for secure access, or nonsecure write reference to an
                                                                 ASC region not enabled for nonsecure access.
                                                                 This may be an indication of software
                                                                 failure, and may be considered fatal.
                                                                 See L2C_TAD()_ERR for logged information.
                                                                 See L2C_ASC_REGION()_START, L2C_ASC_REGION()_END, and
                                                                 L2C_ASC_REGION()_ATTR for ASC region specification. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1C/H) Read reference outside all the defined and enabled address space
                                                                 control (ASC) regions, or secure read reference to an ASC region
                                                                 not enabled for secure access, or nonsecure read reference to an ASC
                                                                 region not enabled for nonsecure access.
                                                                 [RDNXM] interrupts can occur during normal operation as the cores are
                                                                 allowed to prefetch to nonexistent memory locations.  Therefore,
                                                                 [RDNXM] is for informational purposes only.
                                                                 See L2C_TAD()_ERR for logged information.
                                                                 See L2C_ASC_REGION()_START, L2C_ASC_REGION()_END, and
                                                                 L2C_ASC_REGION()_ATTR for ASC region specification. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1C/H) Illegal read to disabled LMC error. A DRAM read arrived before LMC was enabled.
                                                                 Should not occur during normal operation.
                                                                 This may be considered fatal. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1C/H) Illegal write to disabled LMC error. A DRAM write arrived before LMC was enabled.
                                                                 Should not occur during normal operation.
                                                                 This may be considered fatal. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1C/H) An LFB entry (or more) has encountered a timeout condition When [LFBTO] timeout
                                                                 condition occurs L2C_TAD()_TIMEOUT is loaded. L2C_TAD()_TIMEOUT is loaded with
                                                                 info from the first LFB that timed out. if multiple LFB timed out
                                                                 simultaneously, then the it will capture info from the lowest LFB number that
                                                                 timed out.
                                                                 Should not occur during normal operation.  OCI/CCPI link failures may cause this
                                                                 failure. This may be an indication of hardware failure, and may be considered
                                                                 fatal. */
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1C/H) Global sync timeout. Should not occur during normal operation. This may be an
                                                                 indication of hardware failure, and may be considered fatal. */
        uint64_t reserved_19_33        : 15;
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1C/H) Illegal read operation to a remote node with L2C_OCI_CTL[ENAOCI][node]
                                                                 clear. Note [RDDISOCI] interrupts can occur during normal operation as the cores
                                                                 are allowed to prefetch to nonexistent memory locations. Therefore, [RDDISOCI]
                                                                 is for informational purposes only. See L2C_TAD()_ERR for logged information. */
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1C/H) Illegal write operation to a remote node with L2C_OCI_CTL[ENAOCI][node] clear. See
                                                                 L2C_TAD()_ERR for logged information.
                                                                 During normal hardware operation, an indication of a software failure and may be
                                                                 considered fatal. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_int_w1c_s cn8; */
    /* struct cavm_l2c_tadx_int_w1c_s cn81xx; */
    struct cavm_l2c_tadx_int_w1c_cn83xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1C/H) Illegal write operation to a remote node with L2C_OCI_CTL[ENAOCI][node] clear. See
                                                                 L2C_TAD()_ERR for logged information.
                                                                 During normal hardware operation, an indication of a software failure and may be
                                                                 considered fatal. */
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1C/H) Illegal read operation to a remote node with L2C_OCI_CTL[ENAOCI][node]
                                                                 clear. Note [RDDISOCI] interrupts can occur during normal operation as the cores
                                                                 are allowed to prefetch to nonexistent memory locations. Therefore, [RDDISOCI]
                                                                 is for informational purposes only. See L2C_TAD()_ERR for logged information. */
        uint64_t reserved_19_33        : 15;
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1C/H) Global sync timeout. Should not occur during normal operation. This may be an
                                                                 indication of hardware failure, and may be considered fatal. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1C/H) An LFB entry (or more) has encountered a timeout condition When [LFBTO] timeout
                                                                 condition occurs L2C_TAD()_TIMEOUT is loaded. L2C_TAD()_TIMEOUT is loaded with
                                                                 info from the first LFB that timed out. if multiple LFB timed out
                                                                 simultaneously, then the it will capture info from the lowest LFB number that
                                                                 timed out.
                                                                 Should not occur during normal operation.  OCI/CCPI link failures may cause this
                                                                 failure. This may be an indication of hardware failure, and may be considered
                                                                 fatal. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1C/H) Illegal write to disabled LMC error. A DRAM write arrived before LMC was enabled.
                                                                 Should not occur during normal operation.
                                                                 This may be considered fatal. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1C/H) Illegal read to disabled LMC error. A DRAM read arrived before LMC was enabled.
                                                                 Should not occur during normal operation.
                                                                 This may be considered fatal. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1C/H) Read reference outside all the defined and enabled address space
                                                                 control (ASC) regions, or secure read reference to an ASC region
                                                                 not enabled for secure access, or nonsecure read reference to an ASC
                                                                 region not enabled for nonsecure access.
                                                                 [RDNXM] interrupts can occur during normal operation as the cores are
                                                                 allowed to prefetch to nonexistent memory locations.  Therefore,
                                                                 [RDNXM] is for informational purposes only.
                                                                 See L2C_TAD()_ERR for logged information.
                                                                 See L2C_ASC_REGION()_START, L2C_ASC_REGION()_END, and
                                                                 L2C_ASC_REGION()_ATTR for ASC region specification. */
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1C/H) Write reference outside all the defined and enabled address space
                                                                 control (ASC) regions, or secure write reference to an ASC region
                                                                 not enabled for secure access, or nonsecure write reference to an
                                                                 ASC region not enabled for nonsecure access.
                                                                 This may be an indication of software
                                                                 failure, and may be considered fatal.
                                                                 See L2C_TAD()_ERR for logged information.
                                                                 See L2C_ASC_REGION()_START, L2C_ASC_REGION()_END, and
                                                                 L2C_ASC_REGION()_ATTR for ASC region specification. */
        uint64_t reserved_11_12        : 2;
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1C/H) No way was available for allocation. L2C sets [NOWAY] during its processing of a
                                                                 transaction whenever it needed/wanted to allocate a WAY in the L2 cache, but was
                                                                 unable to. When this bit = 1, it is (generally) not an indication that L2C
                                                                 failed to complete transactions. Rather, it is a hint of possible performance
                                                                 degradation. (For example, L2C must read- modify-write DRAM for every
                                                                 transaction that updates some, but not all, of the bytes in a cache block,
                                                                 misses in the L2 cache, and cannot allocate a WAY.) There is one 'failure' case
                                                                 where L2C sets [NOWAY]: when it cannot leave a block locked in the L2 cache as
                                                                 part of a LCKL2 transaction. See L2C_TTG()_ERR for logged information. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1C/H) TAG double-bit error occurred. See L2C_TTG()_ERR for logged information.
                                                                 This is an indication of a hardware failure and may be considered fatal. */
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1C/H) TAG single-bit error on a read. See L2C_TAD()_TTG_ERR for logged
                                                                 information. When [TAGSBE] is set, hardware corrected the error before using the
                                                                 tag, but did not correct any stored value. When [TAGSBE] is set, software should
                                                                 eject the TAG location indicated by the corresponding
                                                                 L2C_TAD()_TTG_ERR[WAY,L2IDX] (via a SYS CVMCACHEWBIL2I instruction below)
                                                                 before clearing [TAGSBE]. Otherwise, hardware may encounter the error again the
                                                                 next time the same TAG location is referenced. Software may also choose to count
                                                                 the number of these single-bit errors.

                                                                 The SYS CVMCACHEWBIL2I instruction payload should have:
                                                                   \<pre\>
                                                                   payload\<24\> = 0
                                                                   payload\<23:20\> = L2C_TAD()_TTG_ERR[WAY]
                                                                   payload\<19:7\>  = L2C_TAD()_TTG_ERR[L2IDX]
                                                                   \</pre\>
                                                                 Note that L2C_CTL[DISIDXALIAS] has no effect on this payload. */
        uint64_t reserved_6_7          : 2;
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1C/H) FBF double-bit error occurred. See L2C_TAD()_TQD_ERR for logged information. An
                                                                 indication of a hardware failure and may be considered fatal. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1C/H) FBF single-bit error on a read. See L2C_TAD()_TQD_ERR for logged
                                                                 information. Hardware automatically corrected the error. Software may choose to
                                                                 count the number of these single-bit errors. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1C/H) SBF double-bit error occurred. See L2C_TAD()_TQD_ERR for logged information. An
                                                                 indication of a hardware failure and may be considered fatal. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1C/H) SBF single-bit error on a read. See L2C_TAD()_TQD_ERR for logged
                                                                 information. Hardware automatically corrected the error. Software may choose to
                                                                 count the number of these single-bit errors. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1C/H) L2D double-bit error occurred. See L2C_TAD()_TQD_ERR for logged information. An
                                                                 indication of a hardware failure and may be considered fatal. */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1C/H) L2D single-bit error on a read. See L2C_TAD()_TQD_ERR for logged
                                                                 information. When [L2DSBE] is set, hardware corrected the error before using the
                                                                 data, but did not correct any stored value. When [L2DSBE] is set, software
                                                                 should eject the cache block indicated by the corresponding
                                                                 L2C_TAD()_TQD_ERR[QDNUM,L2DIDX] (via a SYS CVMCACHEWBIL2I instruction below)
                                                                 before clearing [L2DSBE]. Otherwise, hardware may encounter the error again the
                                                                 next time the same L2D location is referenced. Software may also choose to count
                                                                 the number of these single-bit errors.

                                                                 The SYS CVMCACHEWBIL2I instruction payload should have:
                                                                 \<pre\>
                                                                   payload\<24:23\> = 0
                                                                   payload\<22:19\> = L2C_TAD()_TQD_ERR[L2DIDX]\<10:7\>  // way
                                                                   payload\<18:12\> = L2C_TAD()_TQD_ERR[L2DIDX]\<6:0\>   // index\<11:5\>
                                                                   payload\<11:10\> = L2C_TAD()_TQD_ERR[L2DIDX]\<12:11\> // index\<4:3\>
                                                                   payload\<9\>     = L2C_TAD()_TQD_ERR[QDNUM]\<2\>      // index\<2\>
                                                                   payload\<8:7\>   = tad             // index\<1:0\>
                                                                 \</pre\>

                                                                 where tad is the TAD index from this CSR. Note that L2C_CTL[DISIDXALIAS] has no
                                                                 effect on the payload. */
#else /* Word 0 - Little Endian */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1C/H) L2D single-bit error on a read. See L2C_TAD()_TQD_ERR for logged
                                                                 information. When [L2DSBE] is set, hardware corrected the error before using the
                                                                 data, but did not correct any stored value. When [L2DSBE] is set, software
                                                                 should eject the cache block indicated by the corresponding
                                                                 L2C_TAD()_TQD_ERR[QDNUM,L2DIDX] (via a SYS CVMCACHEWBIL2I instruction below)
                                                                 before clearing [L2DSBE]. Otherwise, hardware may encounter the error again the
                                                                 next time the same L2D location is referenced. Software may also choose to count
                                                                 the number of these single-bit errors.

                                                                 The SYS CVMCACHEWBIL2I instruction payload should have:
                                                                 \<pre\>
                                                                   payload\<24:23\> = 0
                                                                   payload\<22:19\> = L2C_TAD()_TQD_ERR[L2DIDX]\<10:7\>  // way
                                                                   payload\<18:12\> = L2C_TAD()_TQD_ERR[L2DIDX]\<6:0\>   // index\<11:5\>
                                                                   payload\<11:10\> = L2C_TAD()_TQD_ERR[L2DIDX]\<12:11\> // index\<4:3\>
                                                                   payload\<9\>     = L2C_TAD()_TQD_ERR[QDNUM]\<2\>      // index\<2\>
                                                                   payload\<8:7\>   = tad             // index\<1:0\>
                                                                 \</pre\>

                                                                 where tad is the TAD index from this CSR. Note that L2C_CTL[DISIDXALIAS] has no
                                                                 effect on the payload. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1C/H) L2D double-bit error occurred. See L2C_TAD()_TQD_ERR for logged information. An
                                                                 indication of a hardware failure and may be considered fatal. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1C/H) SBF single-bit error on a read. See L2C_TAD()_TQD_ERR for logged
                                                                 information. Hardware automatically corrected the error. Software may choose to
                                                                 count the number of these single-bit errors. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1C/H) SBF double-bit error occurred. See L2C_TAD()_TQD_ERR for logged information. An
                                                                 indication of a hardware failure and may be considered fatal. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1C/H) FBF single-bit error on a read. See L2C_TAD()_TQD_ERR for logged
                                                                 information. Hardware automatically corrected the error. Software may choose to
                                                                 count the number of these single-bit errors. */
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1C/H) FBF double-bit error occurred. See L2C_TAD()_TQD_ERR for logged information. An
                                                                 indication of a hardware failure and may be considered fatal. */
        uint64_t reserved_6_7          : 2;
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1C/H) TAG single-bit error on a read. See L2C_TAD()_TTG_ERR for logged
                                                                 information. When [TAGSBE] is set, hardware corrected the error before using the
                                                                 tag, but did not correct any stored value. When [TAGSBE] is set, software should
                                                                 eject the TAG location indicated by the corresponding
                                                                 L2C_TAD()_TTG_ERR[WAY,L2IDX] (via a SYS CVMCACHEWBIL2I instruction below)
                                                                 before clearing [TAGSBE]. Otherwise, hardware may encounter the error again the
                                                                 next time the same TAG location is referenced. Software may also choose to count
                                                                 the number of these single-bit errors.

                                                                 The SYS CVMCACHEWBIL2I instruction payload should have:
                                                                   \<pre\>
                                                                   payload\<24\> = 0
                                                                   payload\<23:20\> = L2C_TAD()_TTG_ERR[WAY]
                                                                   payload\<19:7\>  = L2C_TAD()_TTG_ERR[L2IDX]
                                                                   \</pre\>
                                                                 Note that L2C_CTL[DISIDXALIAS] has no effect on this payload. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1C/H) TAG double-bit error occurred. See L2C_TTG()_ERR for logged information.
                                                                 This is an indication of a hardware failure and may be considered fatal. */
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1C/H) No way was available for allocation. L2C sets [NOWAY] during its processing of a
                                                                 transaction whenever it needed/wanted to allocate a WAY in the L2 cache, but was
                                                                 unable to. When this bit = 1, it is (generally) not an indication that L2C
                                                                 failed to complete transactions. Rather, it is a hint of possible performance
                                                                 degradation. (For example, L2C must read- modify-write DRAM for every
                                                                 transaction that updates some, but not all, of the bytes in a cache block,
                                                                 misses in the L2 cache, and cannot allocate a WAY.) There is one 'failure' case
                                                                 where L2C sets [NOWAY]: when it cannot leave a block locked in the L2 cache as
                                                                 part of a LCKL2 transaction. See L2C_TTG()_ERR for logged information. */
        uint64_t reserved_11_12        : 2;
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1C/H) Write reference outside all the defined and enabled address space
                                                                 control (ASC) regions, or secure write reference to an ASC region
                                                                 not enabled for secure access, or nonsecure write reference to an
                                                                 ASC region not enabled for nonsecure access.
                                                                 This may be an indication of software
                                                                 failure, and may be considered fatal.
                                                                 See L2C_TAD()_ERR for logged information.
                                                                 See L2C_ASC_REGION()_START, L2C_ASC_REGION()_END, and
                                                                 L2C_ASC_REGION()_ATTR for ASC region specification. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1C/H) Read reference outside all the defined and enabled address space
                                                                 control (ASC) regions, or secure read reference to an ASC region
                                                                 not enabled for secure access, or nonsecure read reference to an ASC
                                                                 region not enabled for nonsecure access.
                                                                 [RDNXM] interrupts can occur during normal operation as the cores are
                                                                 allowed to prefetch to nonexistent memory locations.  Therefore,
                                                                 [RDNXM] is for informational purposes only.
                                                                 See L2C_TAD()_ERR for logged information.
                                                                 See L2C_ASC_REGION()_START, L2C_ASC_REGION()_END, and
                                                                 L2C_ASC_REGION()_ATTR for ASC region specification. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1C/H) Illegal read to disabled LMC error. A DRAM read arrived before LMC was enabled.
                                                                 Should not occur during normal operation.
                                                                 This may be considered fatal. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1C/H) Illegal write to disabled LMC error. A DRAM write arrived before LMC was enabled.
                                                                 Should not occur during normal operation.
                                                                 This may be considered fatal. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1C/H) An LFB entry (or more) has encountered a timeout condition When [LFBTO] timeout
                                                                 condition occurs L2C_TAD()_TIMEOUT is loaded. L2C_TAD()_TIMEOUT is loaded with
                                                                 info from the first LFB that timed out. if multiple LFB timed out
                                                                 simultaneously, then the it will capture info from the lowest LFB number that
                                                                 timed out.
                                                                 Should not occur during normal operation.  OCI/CCPI link failures may cause this
                                                                 failure. This may be an indication of hardware failure, and may be considered
                                                                 fatal. */
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1C/H) Global sync timeout. Should not occur during normal operation. This may be an
                                                                 indication of hardware failure, and may be considered fatal. */
        uint64_t reserved_19_33        : 15;
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1C/H) Illegal read operation to a remote node with L2C_OCI_CTL[ENAOCI][node]
                                                                 clear. Note [RDDISOCI] interrupts can occur during normal operation as the cores
                                                                 are allowed to prefetch to nonexistent memory locations. Therefore, [RDDISOCI]
                                                                 is for informational purposes only. See L2C_TAD()_ERR for logged information. */
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1C/H) Illegal write operation to a remote node with L2C_OCI_CTL[ENAOCI][node] clear. See
                                                                 L2C_TAD()_ERR for logged information.
                                                                 During normal hardware operation, an indication of a software failure and may be
                                                                 considered fatal. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } cn83xx;
};
typedef union cavm_l2c_tadx_int_w1c cavm_l2c_tadx_int_w1c_t;

static inline uint64_t CAVM_L2C_TADX_INT_W1C(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_INT_W1C(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050040000ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050040000ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_INT_W1C", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_INT_W1C(a) cavm_l2c_tadx_int_w1c_t
#define bustype_CAVM_L2C_TADX_INT_W1C(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_INT_W1C(a) "L2C_TADX_INT_W1C"
#define device_bar_CAVM_L2C_TADX_INT_W1C(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_INT_W1C(a) (a)
#define arguments_CAVM_L2C_TADX_INT_W1C(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_int_w1s
 *
 * L2C TAD Interrupt Set Registers
 * This register sets interrupt bits.
 */
union cavm_l2c_tadx_int_w1s
{
    uint64_t u;
    struct cavm_l2c_tadx_int_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[WRDISOCI]. */
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[RDDISOCI]. */
        uint64_t reserved_19_33        : 15;
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[GSYNCTO]. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[LFBTO]. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[WRDISLMC]. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[RDDISLMC]. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[WRNXM]. */
        uint64_t reserved_11_12        : 2;
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[NOWAY]. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[TAGDBE]. */
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[TAGSBE]. */
        uint64_t reserved_6_7          : 2;
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[FBFDBE]. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[FBFSBE]. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[SBFDBE]. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[SBFSBE]. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[L2DDBE]. */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[L2DSBE]. */
#else /* Word 0 - Little Endian */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[L2DSBE]. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[L2DDBE]. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[SBFSBE]. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[SBFDBE]. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[FBFSBE]. */
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[FBFDBE]. */
        uint64_t reserved_6_7          : 2;
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[TAGSBE]. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[TAGDBE]. */
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[NOWAY]. */
        uint64_t reserved_11_12        : 2;
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[RDNXM]. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[RDDISLMC]. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[WRDISLMC]. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[LFBTO]. */
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[GSYNCTO]. */
        uint64_t reserved_19_33        : 15;
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[RDDISOCI]. */
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1S/H) Reads or sets L2C_TAD(0)_INT_W1C[WRDISOCI]. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_int_w1s_s cn8; */
    /* struct cavm_l2c_tadx_int_w1s_s cn81xx; */
    struct cavm_l2c_tadx_int_w1s_cn83xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[WRDISOCI]. */
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[RDDISOCI]. */
        uint64_t reserved_19_33        : 15;
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[GSYNCTO]. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[LFBTO]. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[WRDISLMC]. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[RDDISLMC]. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[RDNXM]. */
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[WRNXM]. */
        uint64_t reserved_11_12        : 2;
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[NOWAY]. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[TAGDBE]. */
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[TAGSBE]. */
        uint64_t reserved_6_7          : 2;
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[FBFDBE]. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[FBFSBE]. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[SBFDBE]. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[SBFSBE]. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[L2DDBE]. */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[L2DSBE]. */
#else /* Word 0 - Little Endian */
        uint64_t l2dsbe                : 1;  /**< [  0:  0](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[L2DSBE]. */
        uint64_t l2ddbe                : 1;  /**< [  1:  1](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[L2DDBE]. */
        uint64_t sbfsbe                : 1;  /**< [  2:  2](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[SBFSBE]. */
        uint64_t sbfdbe                : 1;  /**< [  3:  3](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[SBFDBE]. */
        uint64_t fbfsbe                : 1;  /**< [  4:  4](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[FBFSBE]. */
        uint64_t fbfdbe                : 1;  /**< [  5:  5](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[FBFDBE]. */
        uint64_t reserved_6_7          : 2;
        uint64_t tagsbe                : 1;  /**< [  8:  8](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[TAGSBE]. */
        uint64_t tagdbe                : 1;  /**< [  9:  9](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[TAGDBE]. */
        uint64_t noway                 : 1;  /**< [ 10: 10](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[NOWAY]. */
        uint64_t reserved_11_12        : 2;
        uint64_t wrnxm                 : 1;  /**< [ 13: 13](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[WRNXM]. */
        uint64_t rdnxm                 : 1;  /**< [ 14: 14](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[RDNXM]. */
        uint64_t rddislmc              : 1;  /**< [ 15: 15](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[RDDISLMC]. */
        uint64_t wrdislmc              : 1;  /**< [ 16: 16](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[WRDISLMC]. */
        uint64_t lfbto                 : 1;  /**< [ 17: 17](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[LFBTO]. */
        uint64_t gsyncto               : 1;  /**< [ 18: 18](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[GSYNCTO]. */
        uint64_t reserved_19_33        : 15;
        uint64_t rddisoci              : 1;  /**< [ 34: 34](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[RDDISOCI]. */
        uint64_t wrdisoci              : 1;  /**< [ 35: 35](R/W1S/H) Reads or sets L2C_TAD(0..3)_INT_W1C[WRDISOCI]. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } cn83xx;
};
typedef union cavm_l2c_tadx_int_w1s cavm_l2c_tadx_int_w1s_t;

static inline uint64_t CAVM_L2C_TADX_INT_W1S(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_INT_W1S(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050040008ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050040008ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_INT_W1S", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_INT_W1S(a) cavm_l2c_tadx_int_w1s_t
#define bustype_CAVM_L2C_TADX_INT_W1S(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_INT_W1S(a) "L2C_TADX_INT_W1S"
#define device_bar_CAVM_L2C_TADX_INT_W1S(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_INT_W1S(a) (a)
#define arguments_CAVM_L2C_TADX_INT_W1S(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_msix_pba#
 *
 * L2C TAD MSI-X Pending Bit Array Registers
 * This register is the MSI-X PBA table; the bit number is indexed by the L2C_TAD_INT_VEC_E
 * enumeration.
 */
union cavm_l2c_tadx_msix_pbax
{
    uint64_t u;
    struct cavm_l2c_tadx_msix_pbax_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO) Pending message for the associated L2C_TAD()_MSIX_VEC()_CTL, enumerated by
                                                                 L2C_TAD_INT_VEC_E. Bits
                                                                 that have no associated L2C_TAD_INT_VEC_E are 0. */
#else /* Word 0 - Little Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO) Pending message for the associated L2C_TAD()_MSIX_VEC()_CTL, enumerated by
                                                                 L2C_TAD_INT_VEC_E. Bits
                                                                 that have no associated L2C_TAD_INT_VEC_E are 0. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_msix_pbax_s cn; */
};
typedef union cavm_l2c_tadx_msix_pbax cavm_l2c_tadx_msix_pbax_t;

static inline uint64_t CAVM_L2C_TADX_MSIX_PBAX(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_MSIX_PBAX(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b==0)))
        return 0x87e050ff0000ll + 0x1000000ll * ((a) & 0x0) + 8ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=3) && (b==0)))
        return 0x87e050ff0000ll + 0x1000000ll * ((a) & 0x3) + 8ll * ((b) & 0x0);
    __cavm_csr_fatal("L2C_TADX_MSIX_PBAX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_MSIX_PBAX(a,b) cavm_l2c_tadx_msix_pbax_t
#define bustype_CAVM_L2C_TADX_MSIX_PBAX(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_MSIX_PBAX(a,b) "L2C_TADX_MSIX_PBAX"
#define device_bar_CAVM_L2C_TADX_MSIX_PBAX(a,b) 0x4 /* PF_BAR4 */
#define busnum_CAVM_L2C_TADX_MSIX_PBAX(a,b) (a)
#define arguments_CAVM_L2C_TADX_MSIX_PBAX(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_tad#_msix_vec#_addr
 *
 * L2C TAD MSI-X Vector-Table Address Register
 * This register is the MSI-X vector table, indexed by the L2C_TAD_INT_VEC_E enumeration.
 */
union cavm_l2c_tadx_msix_vecx_addr
{
    uint64_t u;
    struct cavm_l2c_tadx_msix_vecx_addr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_49_63        : 15;
        uint64_t addr                  : 47; /**< [ 48:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_1            : 1;
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 1 = This vector's L2C_TAD()_MSIX_VEC()_ADDR, L2C_TAD()_MSIX_VEC()_CTL, and corresponding
                                                                 bit of L2C_TAD()_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.

                                                                 If PCCPF_L2C_TAD_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is
                                                                 set, all vectors are secure and function as if [SECVEC] was set. */
#else /* Word 0 - Little Endian */
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 1 = This vector's L2C_TAD()_MSIX_VEC()_ADDR, L2C_TAD()_MSIX_VEC()_CTL, and corresponding
                                                                 bit of L2C_TAD()_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.

                                                                 If PCCPF_L2C_TAD_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is
                                                                 set, all vectors are secure and function as if [SECVEC] was set. */
        uint64_t reserved_1            : 1;
        uint64_t addr                  : 47; /**< [ 48:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_49_63        : 15;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_msix_vecx_addr_s cn; */
};
typedef union cavm_l2c_tadx_msix_vecx_addr cavm_l2c_tadx_msix_vecx_addr_t;

static inline uint64_t CAVM_L2C_TADX_MSIX_VECX_ADDR(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_MSIX_VECX_ADDR(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b==0)))
        return 0x87e050f00000ll + 0x1000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=3) && (b==0)))
        return 0x87e050f00000ll + 0x1000000ll * ((a) & 0x3) + 0x10ll * ((b) & 0x0);
    __cavm_csr_fatal("L2C_TADX_MSIX_VECX_ADDR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_MSIX_VECX_ADDR(a,b) cavm_l2c_tadx_msix_vecx_addr_t
#define bustype_CAVM_L2C_TADX_MSIX_VECX_ADDR(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_MSIX_VECX_ADDR(a,b) "L2C_TADX_MSIX_VECX_ADDR"
#define device_bar_CAVM_L2C_TADX_MSIX_VECX_ADDR(a,b) 0x4 /* PF_BAR4 */
#define busnum_CAVM_L2C_TADX_MSIX_VECX_ADDR(a,b) (a)
#define arguments_CAVM_L2C_TADX_MSIX_VECX_ADDR(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_tad#_msix_vec#_ctl
 *
 * L2C TAD MSI-X Vector-Table Control and Data Register
 * This register is the MSI-X vector table, indexed by the L2C_TAD_INT_VEC_E enumeration.
 */
union cavm_l2c_tadx_msix_vecx_ctl
{
    uint64_t u;
    struct cavm_l2c_tadx_msix_vecx_ctl_s
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
    /* struct cavm_l2c_tadx_msix_vecx_ctl_s cn; */
};
typedef union cavm_l2c_tadx_msix_vecx_ctl cavm_l2c_tadx_msix_vecx_ctl_t;

static inline uint64_t CAVM_L2C_TADX_MSIX_VECX_CTL(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_MSIX_VECX_CTL(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b==0)))
        return 0x87e050f00008ll + 0x1000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=3) && (b==0)))
        return 0x87e050f00008ll + 0x1000000ll * ((a) & 0x3) + 0x10ll * ((b) & 0x0);
    __cavm_csr_fatal("L2C_TADX_MSIX_VECX_CTL", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_MSIX_VECX_CTL(a,b) cavm_l2c_tadx_msix_vecx_ctl_t
#define bustype_CAVM_L2C_TADX_MSIX_VECX_CTL(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_MSIX_VECX_CTL(a,b) "L2C_TADX_MSIX_VECX_CTL"
#define device_bar_CAVM_L2C_TADX_MSIX_VECX_CTL(a,b) 0x4 /* PF_BAR4 */
#define busnum_CAVM_L2C_TADX_MSIX_VECX_CTL(a,b) (a)
#define arguments_CAVM_L2C_TADX_MSIX_VECX_CTL(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_tad#_pfc#
 *
 * L2C TAD Performance Counter Registers
 */
union cavm_l2c_tadx_pfcx
{
    uint64_t u;
    struct cavm_l2c_tadx_pfcx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Current counter value. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_pfcx_s cn; */
};
typedef union cavm_l2c_tadx_pfcx cavm_l2c_tadx_pfcx_t;

static inline uint64_t CAVM_L2C_TADX_PFCX(uint64_t a, uint64_t b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_PFCX(uint64_t a, uint64_t b)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && ((a==0) && (b<=3)))
        return 0x87e050010100ll + 0x1000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a<=3) && (b<=3)))
        return 0x87e050010100ll + 0x1000000ll * ((a) & 0x3) + 8ll * ((b) & 0x3);
    __cavm_csr_fatal("L2C_TADX_PFCX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_PFCX(a,b) cavm_l2c_tadx_pfcx_t
#define bustype_CAVM_L2C_TADX_PFCX(a,b) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_PFCX(a,b) "L2C_TADX_PFCX"
#define device_bar_CAVM_L2C_TADX_PFCX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_PFCX(a,b) (a)
#define arguments_CAVM_L2C_TADX_PFCX(a,b) (a),(b),-1,-1

/**
 * Register (RSL) l2c_tad#_prf
 *
 * L2C TAD Performance Counter Control Registers
 * All four counters are equivalent and can use any of the defined selects.
 */
union cavm_l2c_tadx_prf
{
    uint64_t u;
    struct cavm_l2c_tadx_prf_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t cnt3sel               : 8;  /**< [ 31: 24](R/W) Selects event to count for L2C_TAD(0)_PFC(3). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t cnt2sel               : 8;  /**< [ 23: 16](R/W) Selects event to count for L2C_TAD(0)_PFC(2). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t cnt1sel               : 8;  /**< [ 15:  8](R/W) Selects event to count for L2C_TAD(0)_PFC(1). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t cnt0sel               : 8;  /**< [  7:  0](R/W) Selects event to count for L2C_TAD(0)_PFC(0). Enumerated by L2C_TAD_PRF_SEL_E. */
#else /* Word 0 - Little Endian */
        uint64_t cnt0sel               : 8;  /**< [  7:  0](R/W) Selects event to count for L2C_TAD(0)_PFC(0). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t cnt1sel               : 8;  /**< [ 15:  8](R/W) Selects event to count for L2C_TAD(0)_PFC(1). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t cnt2sel               : 8;  /**< [ 23: 16](R/W) Selects event to count for L2C_TAD(0)_PFC(2). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t cnt3sel               : 8;  /**< [ 31: 24](R/W) Selects event to count for L2C_TAD(0)_PFC(3). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_prf_s cn8; */
    /* struct cavm_l2c_tadx_prf_s cn81xx; */
    struct cavm_l2c_tadx_prf_cn83xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t cnt3sel               : 8;  /**< [ 31: 24](R/W) Selects event to count for L2C_TAD(0..3)_PFC(3). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t cnt2sel               : 8;  /**< [ 23: 16](R/W) Selects event to count for L2C_TAD(0..3)_PFC(2). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t cnt1sel               : 8;  /**< [ 15:  8](R/W) Selects event to count for L2C_TAD(0..3)_PFC(1). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t cnt0sel               : 8;  /**< [  7:  0](R/W) Selects event to count for L2C_TAD(0..3)_PFC(0). Enumerated by L2C_TAD_PRF_SEL_E. */
#else /* Word 0 - Little Endian */
        uint64_t cnt0sel               : 8;  /**< [  7:  0](R/W) Selects event to count for L2C_TAD(0..3)_PFC(0). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t cnt1sel               : 8;  /**< [ 15:  8](R/W) Selects event to count for L2C_TAD(0..3)_PFC(1). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t cnt2sel               : 8;  /**< [ 23: 16](R/W) Selects event to count for L2C_TAD(0..3)_PFC(2). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t cnt3sel               : 8;  /**< [ 31: 24](R/W) Selects event to count for L2C_TAD(0..3)_PFC(3). Enumerated by L2C_TAD_PRF_SEL_E. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } cn83xx;
};
typedef union cavm_l2c_tadx_prf cavm_l2c_tadx_prf_t;

static inline uint64_t CAVM_L2C_TADX_PRF(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_PRF(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050010000ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050010000ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_PRF", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_PRF(a) cavm_l2c_tadx_prf_t
#define bustype_CAVM_L2C_TADX_PRF(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_PRF(a) "L2C_TADX_PRF"
#define device_bar_CAVM_L2C_TADX_PRF(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_PRF(a) (a)
#define arguments_CAVM_L2C_TADX_PRF(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_stat
 *
 * L2C TAD Status Registers
 * This register holds information about the instantaneous state of the TAD.
 */
union cavm_l2c_tadx_stat
{
    uint64_t u;
    struct cavm_l2c_tadx_stat_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_14_63        : 50;
        uint64_t lfb_valid_cnt         : 6;  /**< [ 13:  8](RO/H) The number of LFBs in use. */
        uint64_t reserved_5_7          : 3;
        uint64_t vbf_inuse_cnt         : 5;  /**< [  4:  0](RO/H) The number of MCI VBFs in use. */
#else /* Word 0 - Little Endian */
        uint64_t vbf_inuse_cnt         : 5;  /**< [  4:  0](RO/H) The number of MCI VBFs in use. */
        uint64_t reserved_5_7          : 3;
        uint64_t lfb_valid_cnt         : 6;  /**< [ 13:  8](RO/H) The number of LFBs in use. */
        uint64_t reserved_14_63        : 50;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_stat_s cn; */
};
typedef union cavm_l2c_tadx_stat cavm_l2c_tadx_stat_t;

static inline uint64_t CAVM_L2C_TADX_STAT(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_STAT(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050020008ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050020008ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_STAT", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_STAT(a) cavm_l2c_tadx_stat_t
#define bustype_CAVM_L2C_TADX_STAT(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_STAT(a) "L2C_TADX_STAT"
#define device_bar_CAVM_L2C_TADX_STAT(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_STAT(a) (a)
#define arguments_CAVM_L2C_TADX_STAT(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_tag
 *
 * L2C TAD Tag Data Registers
 * This register holds the tag information for LTGL2I and STGL2I commands.
 */
union cavm_l2c_tadx_tag
{
    uint64_t u;
    struct cavm_l2c_tadx_tag_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t sblkdty               : 4;  /**< [ 63: 60](R/W/H) Sub-block dirty bits. Ignored/loaded with 0 for RTG accesses. If [TS] is invalid
                                                                 (0), [SBLKDTY] must be 0 or operation is undefined. */
        uint64_t reserved_59           : 1;
        uint64_t nonsec                : 1;  /**< [ 58: 58](R/W/H) Nonsecure bit. */
        uint64_t businfo               : 9;  /**< [ 57: 49](R/W/H) The bus information bits. Ignored/loaded with 0 for RTG accesses. */
        uint64_t ecc                   : 7;  /**< [ 48: 42](R/W/H) The tag ECC. This field is undefined if L2C_CTL[DISECC] is not 1 when the LTGL2I reads the tags. */
        uint64_t reserved_6_41         : 36;
        uint64_t node                  : 2;  /**< [  5:  4](RAZ) Reserved. */
        uint64_t ts                    : 2;  /**< [  3:  2](R/W/H) The tag state.
                                                                 0x0 = Invalid.
                                                                 0x1 = Shared.
                                                                 0x2 = Exclusive.

                                                                 Note that a local address will never have the value of exclusive as that state
                                                                 is encoded as shared in the tag and invalid in the RTG. */
        uint64_t used                  : 1;  /**< [  1:  1](R/W/H) The LRU use bit. If setting the [LOCK] bit, the USE bit should also be set or
                                                                 the operation is undefined. Ignored/loaded with 0 for RTG accesses. */
        uint64_t lock                  : 1;  /**< [  0:  0](R/W/H) The lock bit. If setting the [LOCK] bit, the USE bit should also be set or the
                                                                 operation is undefined. Ignored/loaded with 0 for RTG accesses. */
#else /* Word 0 - Little Endian */
        uint64_t lock                  : 1;  /**< [  0:  0](R/W/H) The lock bit. If setting the [LOCK] bit, the USE bit should also be set or the
                                                                 operation is undefined. Ignored/loaded with 0 for RTG accesses. */
        uint64_t used                  : 1;  /**< [  1:  1](R/W/H) The LRU use bit. If setting the [LOCK] bit, the USE bit should also be set or
                                                                 the operation is undefined. Ignored/loaded with 0 for RTG accesses. */
        uint64_t ts                    : 2;  /**< [  3:  2](R/W/H) The tag state.
                                                                 0x0 = Invalid.
                                                                 0x1 = Shared.
                                                                 0x2 = Exclusive.

                                                                 Note that a local address will never have the value of exclusive as that state
                                                                 is encoded as shared in the tag and invalid in the RTG. */
        uint64_t node                  : 2;  /**< [  5:  4](RAZ) Reserved. */
        uint64_t reserved_6_41         : 36;
        uint64_t ecc                   : 7;  /**< [ 48: 42](R/W/H) The tag ECC. This field is undefined if L2C_CTL[DISECC] is not 1 when the LTGL2I reads the tags. */
        uint64_t businfo               : 9;  /**< [ 57: 49](R/W/H) The bus information bits. Ignored/loaded with 0 for RTG accesses. */
        uint64_t nonsec                : 1;  /**< [ 58: 58](R/W/H) Nonsecure bit. */
        uint64_t reserved_59           : 1;
        uint64_t sblkdty               : 4;  /**< [ 63: 60](R/W/H) Sub-block dirty bits. Ignored/loaded with 0 for RTG accesses. If [TS] is invalid
                                                                 (0), [SBLKDTY] must be 0 or operation is undefined. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_tag_s cn8; */
    struct cavm_l2c_tadx_tag_cn81xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t sblkdty               : 4;  /**< [ 63: 60](R/W/H) Sub-block dirty bits. Ignored/loaded with 0 for RTG accesses. If [TS] is invalid
                                                                 (0), [SBLKDTY] must be 0 or operation is undefined. */
        uint64_t reserved_59           : 1;
        uint64_t nonsec                : 1;  /**< [ 58: 58](R/W/H) Nonsecure bit. */
        uint64_t reserved_57           : 1;
        uint64_t businfo               : 8;  /**< [ 56: 49](R/W/H) The bus information bits. Ignored/loaded with 0 for RTG accesses. */
        uint64_t ecc                   : 7;  /**< [ 48: 42](R/W/H) The tag ECC. This field is undefined if L2C_CTL[DISECC] is not 1 when the LTGL2I reads the tags. */
        uint64_t reserved_40_41        : 2;
        uint64_t tag                   : 23; /**< [ 39: 17](R/W/H) The tag. TAG\<39:17\> is the corresponding bits from the L2C+LMC internal L2/DRAM byte
                                                                 address. */
        uint64_t reserved_6_16         : 11;
        uint64_t node                  : 2;  /**< [  5:  4](RAZ) Reserved. */
        uint64_t ts                    : 2;  /**< [  3:  2](R/W/H) The tag state.
                                                                 0x0 = Invalid.
                                                                 0x1 = Shared.
                                                                 0x2 = Exclusive.

                                                                 Note that a local address will never have the value of exclusive as that state
                                                                 is encoded as shared in the tag and invalid in the RTG. */
        uint64_t used                  : 1;  /**< [  1:  1](R/W/H) The LRU use bit. If setting the [LOCK] bit, the USE bit should also be set or
                                                                 the operation is undefined. Ignored/loaded with 0 for RTG accesses. */
        uint64_t lock                  : 1;  /**< [  0:  0](R/W/H) The lock bit. If setting the [LOCK] bit, the USE bit should also be set or the
                                                                 operation is undefined. Ignored/loaded with 0 for RTG accesses. */
#else /* Word 0 - Little Endian */
        uint64_t lock                  : 1;  /**< [  0:  0](R/W/H) The lock bit. If setting the [LOCK] bit, the USE bit should also be set or the
                                                                 operation is undefined. Ignored/loaded with 0 for RTG accesses. */
        uint64_t used                  : 1;  /**< [  1:  1](R/W/H) The LRU use bit. If setting the [LOCK] bit, the USE bit should also be set or
                                                                 the operation is undefined. Ignored/loaded with 0 for RTG accesses. */
        uint64_t ts                    : 2;  /**< [  3:  2](R/W/H) The tag state.
                                                                 0x0 = Invalid.
                                                                 0x1 = Shared.
                                                                 0x2 = Exclusive.

                                                                 Note that a local address will never have the value of exclusive as that state
                                                                 is encoded as shared in the tag and invalid in the RTG. */
        uint64_t node                  : 2;  /**< [  5:  4](RAZ) Reserved. */
        uint64_t reserved_6_16         : 11;
        uint64_t tag                   : 23; /**< [ 39: 17](R/W/H) The tag. TAG\<39:17\> is the corresponding bits from the L2C+LMC internal L2/DRAM byte
                                                                 address. */
        uint64_t reserved_40_41        : 2;
        uint64_t ecc                   : 7;  /**< [ 48: 42](R/W/H) The tag ECC. This field is undefined if L2C_CTL[DISECC] is not 1 when the LTGL2I reads the tags. */
        uint64_t businfo               : 8;  /**< [ 56: 49](R/W/H) The bus information bits. Ignored/loaded with 0 for RTG accesses. */
        uint64_t reserved_57           : 1;
        uint64_t nonsec                : 1;  /**< [ 58: 58](R/W/H) Nonsecure bit. */
        uint64_t reserved_59           : 1;
        uint64_t sblkdty               : 4;  /**< [ 63: 60](R/W/H) Sub-block dirty bits. Ignored/loaded with 0 for RTG accesses. If [TS] is invalid
                                                                 (0), [SBLKDTY] must be 0 or operation is undefined. */
#endif /* Word 0 - End */
    } cn81xx;
    struct cavm_l2c_tadx_tag_cn83xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t sblkdty               : 4;  /**< [ 63: 60](R/W/H) Sub-block dirty bits. Ignored/loaded with 0 for RTG accesses. If [TS] is invalid
                                                                 (0), [SBLKDTY] must be 0 or operation is undefined. */
        uint64_t reserved_59           : 1;
        uint64_t nonsec                : 1;  /**< [ 58: 58](R/W/H) Nonsecure bit. */
        uint64_t businfo               : 9;  /**< [ 57: 49](R/W/H) The bus information bits. Ignored/loaded with 0 for RTG accesses. */
        uint64_t ecc                   : 7;  /**< [ 48: 42](R/W/H) The tag ECC. This field is undefined if L2C_CTL[DISECC] is not 1 when the LTGL2I reads the tags. */
        uint64_t reserved_40_41        : 2;
        uint64_t tag                   : 22; /**< [ 39: 18](R/W/H) The tag. TAG\<39:18\> is the corresponding bits from the L2C+LMC internal L2/DRAM byte
                                                                 address. */
        uint64_t reserved_6_17         : 12;
        uint64_t node                  : 2;  /**< [  5:  4](RAZ) Reserved. */
        uint64_t ts                    : 2;  /**< [  3:  2](R/W/H) The tag state.
                                                                 0x0 = Invalid.
                                                                 0x1 = Shared.
                                                                 0x2 = Exclusive.

                                                                 Note that a local address will never have the value of exclusive as that state
                                                                 is encoded as shared in the tag and invalid in the RTG. */
        uint64_t used                  : 1;  /**< [  1:  1](R/W/H) The LRU use bit. If setting the [LOCK] bit, the USE bit should also be set or
                                                                 the operation is undefined. Ignored/loaded with 0 for RTG accesses. */
        uint64_t lock                  : 1;  /**< [  0:  0](R/W/H) The lock bit. If setting the [LOCK] bit, the USE bit should also be set or the
                                                                 operation is undefined. Ignored/loaded with 0 for RTG accesses. */
#else /* Word 0 - Little Endian */
        uint64_t lock                  : 1;  /**< [  0:  0](R/W/H) The lock bit. If setting the [LOCK] bit, the USE bit should also be set or the
                                                                 operation is undefined. Ignored/loaded with 0 for RTG accesses. */
        uint64_t used                  : 1;  /**< [  1:  1](R/W/H) The LRU use bit. If setting the [LOCK] bit, the USE bit should also be set or
                                                                 the operation is undefined. Ignored/loaded with 0 for RTG accesses. */
        uint64_t ts                    : 2;  /**< [  3:  2](R/W/H) The tag state.
                                                                 0x0 = Invalid.
                                                                 0x1 = Shared.
                                                                 0x2 = Exclusive.

                                                                 Note that a local address will never have the value of exclusive as that state
                                                                 is encoded as shared in the tag and invalid in the RTG. */
        uint64_t node                  : 2;  /**< [  5:  4](RAZ) Reserved. */
        uint64_t reserved_6_17         : 12;
        uint64_t tag                   : 22; /**< [ 39: 18](R/W/H) The tag. TAG\<39:18\> is the corresponding bits from the L2C+LMC internal L2/DRAM byte
                                                                 address. */
        uint64_t reserved_40_41        : 2;
        uint64_t ecc                   : 7;  /**< [ 48: 42](R/W/H) The tag ECC. This field is undefined if L2C_CTL[DISECC] is not 1 when the LTGL2I reads the tags. */
        uint64_t businfo               : 9;  /**< [ 57: 49](R/W/H) The bus information bits. Ignored/loaded with 0 for RTG accesses. */
        uint64_t nonsec                : 1;  /**< [ 58: 58](R/W/H) Nonsecure bit. */
        uint64_t reserved_59           : 1;
        uint64_t sblkdty               : 4;  /**< [ 63: 60](R/W/H) Sub-block dirty bits. Ignored/loaded with 0 for RTG accesses. If [TS] is invalid
                                                                 (0), [SBLKDTY] must be 0 or operation is undefined. */
#endif /* Word 0 - End */
    } cn83xx;
};
typedef union cavm_l2c_tadx_tag cavm_l2c_tadx_tag_t;

static inline uint64_t CAVM_L2C_TADX_TAG(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_TAG(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050020000ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050020000ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_TAG", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_TAG(a) cavm_l2c_tadx_tag_t
#define bustype_CAVM_L2C_TADX_TAG(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_TAG(a) "L2C_TADX_TAG"
#define device_bar_CAVM_L2C_TADX_TAG(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_TAG(a) (a)
#define arguments_CAVM_L2C_TADX_TAG(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_tbf_bist_status
 *
 * L2C TAD Quad Buffer BIST Status Registers
 */
union cavm_l2c_tadx_tbf_bist_status
{
    uint64_t u;
    struct cavm_l2c_tadx_tbf_bist_status_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vbffl                 : 16; /**< [ 63: 48](RO/H) BIST failure status for VBF ({QD7H1,QD7H0, ... , QD0H1, QD0H0}). */
        uint64_t sbffl                 : 16; /**< [ 47: 32](RO/H) BIST failure status for SBF ({QD7H1,QD7H0, ... , QD0H1, QD0H0}). */
        uint64_t fbfrspfl              : 16; /**< [ 31: 16](RO/H) BIST failure status for FBF RSP port ({QD7H1,QD7H0, ... , QD0H1, QD0H0}). */
        uint64_t fbfwrpfl              : 16; /**< [ 15:  0](RO/H) BIST failure status for FBF WRP port ({QD7H1,QD7H0, ... , QD0H1, QD0H0}). */
#else /* Word 0 - Little Endian */
        uint64_t fbfwrpfl              : 16; /**< [ 15:  0](RO/H) BIST failure status for FBF WRP port ({QD7H1,QD7H0, ... , QD0H1, QD0H0}). */
        uint64_t fbfrspfl              : 16; /**< [ 31: 16](RO/H) BIST failure status for FBF RSP port ({QD7H1,QD7H0, ... , QD0H1, QD0H0}). */
        uint64_t sbffl                 : 16; /**< [ 47: 32](RO/H) BIST failure status for SBF ({QD7H1,QD7H0, ... , QD0H1, QD0H0}). */
        uint64_t vbffl                 : 16; /**< [ 63: 48](RO/H) BIST failure status for VBF ({QD7H1,QD7H0, ... , QD0H1, QD0H0}). */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_tbf_bist_status_s cn; */
};
typedef union cavm_l2c_tadx_tbf_bist_status cavm_l2c_tadx_tbf_bist_status_t;

static inline uint64_t CAVM_L2C_TADX_TBF_BIST_STATUS(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_TBF_BIST_STATUS(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050070000ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050070000ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_TBF_BIST_STATUS", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_TBF_BIST_STATUS(a) cavm_l2c_tadx_tbf_bist_status_t
#define bustype_CAVM_L2C_TADX_TBF_BIST_STATUS(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_TBF_BIST_STATUS(a) "L2C_TADX_TBF_BIST_STATUS"
#define device_bar_CAVM_L2C_TADX_TBF_BIST_STATUS(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_TBF_BIST_STATUS(a) (a)
#define arguments_CAVM_L2C_TADX_TBF_BIST_STATUS(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_tdt_bist_status
 *
 * L2C TAD Data BIST Status Registers
 */
union cavm_l2c_tadx_tdt_bist_status
{
    uint64_t u;
    struct cavm_l2c_tadx_tdt_bist_status_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_16_63        : 48;
        uint64_t l2dfl                 : 16; /**< [ 15:  0](RO/H) BIST failure status for L2D ({QD7H1,QD7H0, ... , QD0H1, QD0H0}). */
#else /* Word 0 - Little Endian */
        uint64_t l2dfl                 : 16; /**< [ 15:  0](RO/H) BIST failure status for L2D ({QD7H1,QD7H0, ... , QD0H1, QD0H0}). */
        uint64_t reserved_16_63        : 48;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_tdt_bist_status_s cn; */
};
typedef union cavm_l2c_tadx_tdt_bist_status cavm_l2c_tadx_tdt_bist_status_t;

static inline uint64_t CAVM_L2C_TADX_TDT_BIST_STATUS(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_TDT_BIST_STATUS(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050070100ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050070100ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_TDT_BIST_STATUS", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_TDT_BIST_STATUS(a) cavm_l2c_tadx_tdt_bist_status_t
#define bustype_CAVM_L2C_TADX_TDT_BIST_STATUS(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_TDT_BIST_STATUS(a) "L2C_TADX_TDT_BIST_STATUS"
#define device_bar_CAVM_L2C_TADX_TDT_BIST_STATUS(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_TDT_BIST_STATUS(a) (a)
#define arguments_CAVM_L2C_TADX_TDT_BIST_STATUS(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_timeout
 *
 * L2C TAD LFB Timeout Info Registers
 * This register records error information for an LFBTO (LFB TimeOut). The first LFBTO error will
 * lock the register until the logged error type s cleared. If multiple LFBs timed out
 * simultaneously, then this will contain the information from the lowest LFB number that has
 * timed-out. The address can be for the original transaction address or the replacement address
 * (if both could have timed out, then the transaction address will be here).
 */
union cavm_l2c_tadx_timeout
{
    uint64_t u;
    struct cavm_l2c_tadx_timeout_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t infolfb               : 1;  /**< [ 63: 63](RO/H) Logged address information is for the LFB original transation. */
        uint64_t infovab               : 1;  /**< [ 62: 62](RO/H) Logged address information is for the VAB (replacement). If both this and
                                                                 [INFOLFB] is set, then both could have timed out, but info captured is from the
                                                                 original LFB. */
        uint64_t reserved_57_61        : 5;
        uint64_t lfbnum                : 5;  /**< [ 56: 52](RO/H) The LFB number of the entry that timed out, and have its info captures in this register. */
        uint64_t cmd                   : 8;  /**< [ 51: 44](RO/H) Encoding of XMC or CCPI command causing error.
                                                                 Internal:
                                                                 If CMD\<7\>==1, use XMC_CMD_E to decode CMD\<6:0\>. If CMD\<7:5\>==0, use
                                                                 OCI_MREQ_CMD_E to
                                                                 decode CMD\<4:0\>. If CMD\<7:5\>==1, use OCI_MFWD_CMD_E to decode CMD\<4:0\>. If CMD\<7:5\>==2,
                                                                 use OCI_MRSP_CMD_E to decode CMD\<4:0\>. */
        uint64_t reserved_42_43        : 2;
        uint64_t node                  : 2;  /**< [ 41: 40](RO/H) Home node of the address causing the error. Similar to [ADDR] below, this can be the
                                                                 request address (if [INFOLFB] is set), else it is the replacement address (if [INFOLFB] is
                                                                 clear & [INFOVAB] is set). */
        uint64_t addr                  : 33; /**< [ 39:  7](RO/H) Cache line address causing the error. This can be either the request address or
                                                                 the replacement (if [INFOLFB] is set), else it is the replacement address (if
                                                                 [INFOLFB] is clear & [INFOVAB] is set). This address is a physical address. L2C
                                                                 performs index aliasing (if enabled) on the written address and uses that for
                                                                 the command. This index-aliased address is what is returned on a read of
                                                                 L2C_XMC_CMD. */
        uint64_t reserved_0_6          : 7;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_6          : 7;
        uint64_t addr                  : 33; /**< [ 39:  7](RO/H) Cache line address causing the error. This can be either the request address or
                                                                 the replacement (if [INFOLFB] is set), else it is the replacement address (if
                                                                 [INFOLFB] is clear & [INFOVAB] is set). This address is a physical address. L2C
                                                                 performs index aliasing (if enabled) on the written address and uses that for
                                                                 the command. This index-aliased address is what is returned on a read of
                                                                 L2C_XMC_CMD. */
        uint64_t node                  : 2;  /**< [ 41: 40](RO/H) Home node of the address causing the error. Similar to [ADDR] below, this can be the
                                                                 request address (if [INFOLFB] is set), else it is the replacement address (if [INFOLFB] is
                                                                 clear & [INFOVAB] is set). */
        uint64_t reserved_42_43        : 2;
        uint64_t cmd                   : 8;  /**< [ 51: 44](RO/H) Encoding of XMC or CCPI command causing error.
                                                                 Internal:
                                                                 If CMD\<7\>==1, use XMC_CMD_E to decode CMD\<6:0\>. If CMD\<7:5\>==0, use
                                                                 OCI_MREQ_CMD_E to
                                                                 decode CMD\<4:0\>. If CMD\<7:5\>==1, use OCI_MFWD_CMD_E to decode CMD\<4:0\>. If CMD\<7:5\>==2,
                                                                 use OCI_MRSP_CMD_E to decode CMD\<4:0\>. */
        uint64_t lfbnum                : 5;  /**< [ 56: 52](RO/H) The LFB number of the entry that timed out, and have its info captures in this register. */
        uint64_t reserved_57_61        : 5;
        uint64_t infovab               : 1;  /**< [ 62: 62](RO/H) Logged address information is for the VAB (replacement). If both this and
                                                                 [INFOLFB] is set, then both could have timed out, but info captured is from the
                                                                 original LFB. */
        uint64_t infolfb               : 1;  /**< [ 63: 63](RO/H) Logged address information is for the LFB original transation. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_timeout_s cn; */
};
typedef union cavm_l2c_tadx_timeout cavm_l2c_tadx_timeout_t;

static inline uint64_t CAVM_L2C_TADX_TIMEOUT(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_TIMEOUT(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050050100ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050050100ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_TIMEOUT", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_TIMEOUT(a) cavm_l2c_tadx_timeout_t
#define bustype_CAVM_L2C_TADX_TIMEOUT(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_TIMEOUT(a) "L2C_TADX_TIMEOUT"
#define device_bar_CAVM_L2C_TADX_TIMEOUT(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_TIMEOUT(a) (a)
#define arguments_CAVM_L2C_TADX_TIMEOUT(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_timetwo
 *
 * L2C TAD LFB Timeout Count Registers
 * This register records the number of LFB entries that have timed out.
 */
union cavm_l2c_tadx_timetwo
{
    uint64_t u;
    struct cavm_l2c_tadx_timetwo_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_33_63        : 31;
        uint64_t sid                   : 4;  /**< [ 32: 29](RO/H) Source id of the original request, that is 'source' of request. This is only valid if the
                                                                 request is a local request (valid if L2C_TAD()_TIMEOUT[CMD] is an XMC request and not
                                                                 relevant if it is an CCPI request). */
        uint64_t busid                 : 4;  /**< [ 28: 25](RO/H) Busid of the original request, that is 'source' of request. */
        uint64_t vabst                 : 3;  /**< [ 24: 22](RO/H) This is the LFB internal state if INFOLFB is set, else will contain VAB internal state if
                                                                 INFOVAB is set. */
        uint64_t lfbst                 : 14; /**< [ 21:  8](RO/H) This is the LFB internal state if INFOLFB is set, else will contain VAB internal state if
                                                                 INFOVAB is set. */
        uint64_t tocnt                 : 8;  /**< [  7:  0](RO/H) This is a running count of the LFB that has timed out ... the count will saturate at 0xFF.
                                                                 Will clear when the LFBTO interrupt is cleared. */
#else /* Word 0 - Little Endian */
        uint64_t tocnt                 : 8;  /**< [  7:  0](RO/H) This is a running count of the LFB that has timed out ... the count will saturate at 0xFF.
                                                                 Will clear when the LFBTO interrupt is cleared. */
        uint64_t lfbst                 : 14; /**< [ 21:  8](RO/H) This is the LFB internal state if INFOLFB is set, else will contain VAB internal state if
                                                                 INFOVAB is set. */
        uint64_t vabst                 : 3;  /**< [ 24: 22](RO/H) This is the LFB internal state if INFOLFB is set, else will contain VAB internal state if
                                                                 INFOVAB is set. */
        uint64_t busid                 : 4;  /**< [ 28: 25](RO/H) Busid of the original request, that is 'source' of request. */
        uint64_t sid                   : 4;  /**< [ 32: 29](RO/H) Source id of the original request, that is 'source' of request. This is only valid if the
                                                                 request is a local request (valid if L2C_TAD()_TIMEOUT[CMD] is an XMC request and not
                                                                 relevant if it is an CCPI request). */
        uint64_t reserved_33_63        : 31;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_timetwo_s cn; */
};
typedef union cavm_l2c_tadx_timetwo cavm_l2c_tadx_timetwo_t;

static inline uint64_t CAVM_L2C_TADX_TIMETWO(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_TIMETWO(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050050000ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050050000ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_TIMETWO", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_TIMETWO(a) cavm_l2c_tadx_timetwo_t
#define bustype_CAVM_L2C_TADX_TIMETWO(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_TIMETWO(a) "L2C_TADX_TIMETWO"
#define device_bar_CAVM_L2C_TADX_TIMETWO(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_TIMETWO(a) (a)
#define arguments_CAVM_L2C_TADX_TIMETWO(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_tqd_err
 *
 * L2C TAD Quad Error Information Registers
 * This register records error information for all L2D/SBF/FBF errors.
 * An error locks the [L2DIDX] and [SYN] fields and sets the bit corresponding to the error
 * received.
 * DBE errors take priority and overwrite an earlier logged SBE error. Only one of SBE/DBE is set
 * at any given time and serves to document which error the [L2DIDX]/[SYN] is associated with.
 * The syndrome is recorded for DBE errors, though the utility of the value is not clear.
 */
union cavm_l2c_tadx_tqd_err
{
    uint64_t u;
    struct cavm_l2c_tadx_tqd_err_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t l2ddbe                : 1;  /**< [ 63: 63](RO/H) L2DIDX/SYN corresponds to a double-bit L2D ECC error. */
        uint64_t sbfdbe                : 1;  /**< [ 62: 62](RO/H) L2DIDX/SYN corresponds to a double-bit SBF ECC error. */
        uint64_t fbfdbe                : 1;  /**< [ 61: 61](RO/H) L2DIDX/SYN corresponds to a double-bit FBF ECC error. */
        uint64_t l2dsbe                : 1;  /**< [ 60: 60](RO/H) L2DIDX/SYN corresponds to a single-bit L2D ECC error. */
        uint64_t sbfsbe                : 1;  /**< [ 59: 59](RO/H) L2DIDX/SYN corresponds to a single-bit SBF ECC error. */
        uint64_t fbfsbe                : 1;  /**< [ 58: 58](RO/H) L2DIDX/SYN corresponds to a single-bit FBF ECC error. */
        uint64_t reserved_40_57        : 18;
        uint64_t syn                   : 8;  /**< [ 39: 32](RO/H) Error syndrome. */
        uint64_t reserved_18_31        : 14;
        uint64_t qdnum                 : 3;  /**< [ 17: 15](RO/H) Quad containing the error. */
        uint64_t qdhlf                 : 1;  /**< [ 14: 14](RO/H) Quad half of the containing the error. */
        uint64_t l2didx                : 14; /**< [ 13:  0](RO/H) For L2D errors, index within the quad-half containing the error. For SBF and FBF errors
                                                                 \<13:5\> is 0x0 and \<4:0\> is the index of the error (\<4:1\> is lfbnum\<3:0\>, \<0\> is addr\<5\>).
                                                                 See L2C_TAD()_INT_W1C[L2DSBE] for an important use of this field. */
#else /* Word 0 - Little Endian */
        uint64_t l2didx                : 14; /**< [ 13:  0](RO/H) For L2D errors, index within the quad-half containing the error. For SBF and FBF errors
                                                                 \<13:5\> is 0x0 and \<4:0\> is the index of the error (\<4:1\> is lfbnum\<3:0\>, \<0\> is addr\<5\>).
                                                                 See L2C_TAD()_INT_W1C[L2DSBE] for an important use of this field. */
        uint64_t qdhlf                 : 1;  /**< [ 14: 14](RO/H) Quad half of the containing the error. */
        uint64_t qdnum                 : 3;  /**< [ 17: 15](RO/H) Quad containing the error. */
        uint64_t reserved_18_31        : 14;
        uint64_t syn                   : 8;  /**< [ 39: 32](RO/H) Error syndrome. */
        uint64_t reserved_40_57        : 18;
        uint64_t fbfsbe                : 1;  /**< [ 58: 58](RO/H) L2DIDX/SYN corresponds to a single-bit FBF ECC error. */
        uint64_t sbfsbe                : 1;  /**< [ 59: 59](RO/H) L2DIDX/SYN corresponds to a single-bit SBF ECC error. */
        uint64_t l2dsbe                : 1;  /**< [ 60: 60](RO/H) L2DIDX/SYN corresponds to a single-bit L2D ECC error. */
        uint64_t fbfdbe                : 1;  /**< [ 61: 61](RO/H) L2DIDX/SYN corresponds to a double-bit FBF ECC error. */
        uint64_t sbfdbe                : 1;  /**< [ 62: 62](RO/H) L2DIDX/SYN corresponds to a double-bit SBF ECC error. */
        uint64_t l2ddbe                : 1;  /**< [ 63: 63](RO/H) L2DIDX/SYN corresponds to a double-bit L2D ECC error. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_tqd_err_s cn; */
};
typedef union cavm_l2c_tadx_tqd_err cavm_l2c_tadx_tqd_err_t;

static inline uint64_t CAVM_L2C_TADX_TQD_ERR(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_TQD_ERR(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050060100ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050060100ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_TQD_ERR", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_TQD_ERR(a) cavm_l2c_tadx_tqd_err_t
#define bustype_CAVM_L2C_TADX_TQD_ERR(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_TQD_ERR(a) "L2C_TADX_TQD_ERR"
#define device_bar_CAVM_L2C_TADX_TQD_ERR(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_TQD_ERR(a) (a)
#define arguments_CAVM_L2C_TADX_TQD_ERR(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_ttg_bist_status
 *
 * L2C TAD Tag BIST Status Registers
 */
union cavm_l2c_tadx_ttg_bist_status
{
    uint64_t u;
    struct cavm_l2c_tadx_ttg_bist_status_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_50_63        : 14;
        uint64_t xmdmskfl              : 2;  /**< [ 49: 48](RO/H) Reserved, always zero. */
        uint64_t reserved_18_47        : 30;
        uint64_t lrulfbfl              : 1;  /**< [ 17: 17](RO) Reserved, always zero. */
        uint64_t lrufl                 : 1;  /**< [ 16: 16](RO/H) BIST failure status for tag LRU. */
        uint64_t tagfl                 : 16; /**< [ 15:  0](RO/H) BIST failure status for TAG ways. */
#else /* Word 0 - Little Endian */
        uint64_t tagfl                 : 16; /**< [ 15:  0](RO/H) BIST failure status for TAG ways. */
        uint64_t lrufl                 : 1;  /**< [ 16: 16](RO/H) BIST failure status for tag LRU. */
        uint64_t lrulfbfl              : 1;  /**< [ 17: 17](RO) Reserved, always zero. */
        uint64_t reserved_18_47        : 30;
        uint64_t xmdmskfl              : 2;  /**< [ 49: 48](RO/H) Reserved, always zero. */
        uint64_t reserved_50_63        : 14;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_ttg_bist_status_s cn8; */
    /* struct cavm_l2c_tadx_ttg_bist_status_s cn81xx; */
    struct cavm_l2c_tadx_ttg_bist_status_cn83xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_50_63        : 14;
        uint64_t xmdmskfl              : 2;  /**< [ 49: 48](RO/H) BIST failure status for RSTP XMDMSK memories. */
        uint64_t reserved_18_47        : 30;
        uint64_t lrulfbfl              : 1;  /**< [ 17: 17](RO) Reserved, always zero. */
        uint64_t lrufl                 : 1;  /**< [ 16: 16](RO/H) BIST failure status for tag LRU. */
        uint64_t tagfl                 : 16; /**< [ 15:  0](RO/H) BIST failure status for TAG ways. */
#else /* Word 0 - Little Endian */
        uint64_t tagfl                 : 16; /**< [ 15:  0](RO/H) BIST failure status for TAG ways. */
        uint64_t lrufl                 : 1;  /**< [ 16: 16](RO/H) BIST failure status for tag LRU. */
        uint64_t lrulfbfl              : 1;  /**< [ 17: 17](RO) Reserved, always zero. */
        uint64_t reserved_18_47        : 30;
        uint64_t xmdmskfl              : 2;  /**< [ 49: 48](RO/H) BIST failure status for RSTP XMDMSK memories. */
        uint64_t reserved_50_63        : 14;
#endif /* Word 0 - End */
    } cn83xx;
};
typedef union cavm_l2c_tadx_ttg_bist_status cavm_l2c_tadx_ttg_bist_status_t;

static inline uint64_t CAVM_L2C_TADX_TTG_BIST_STATUS(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_TTG_BIST_STATUS(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050070200ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050070200ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_TTG_BIST_STATUS", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_TTG_BIST_STATUS(a) cavm_l2c_tadx_ttg_bist_status_t
#define bustype_CAVM_L2C_TADX_TTG_BIST_STATUS(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_TTG_BIST_STATUS(a) "L2C_TADX_TTG_BIST_STATUS"
#define device_bar_CAVM_L2C_TADX_TTG_BIST_STATUS(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_TTG_BIST_STATUS(a) (a)
#define arguments_CAVM_L2C_TADX_TTG_BIST_STATUS(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_tad#_ttg_err
 *
 * L2C TAD Tag Error Information Registers
 * This register records error information for all TAG SBE/DBE/NOWAY errors.
 * The priority of errors (lowest to highest) is NOWAY, SBE, DBE. An error locks [SYN], [WAY],
 * and [L2IDX] for equal or lower priority errors until cleared by software.
 * The syndrome is recorded for DBE errors, though the utility of the value is not clear.
 * A NOWAY error does not change the value of the [SYN] field, and leaves [WAY] unpredictable.
 * [L2IDX]\<18:7\> is the L2 block index associated with the command which had no way to allocate.
 */
union cavm_l2c_tadx_ttg_err
{
    uint64_t u;
    struct cavm_l2c_tadx_ttg_err_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t tagdbe                : 1;  /**< [ 63: 63](RO/H) Information refers to a double-bit TAG ECC error. */
        uint64_t tagsbe                : 1;  /**< [ 62: 62](RO/H) Information refers to a single-bit TAG ECC error. */
        uint64_t noway                 : 1;  /**< [ 61: 61](RO/H) Information refers to a NOWAY error. */
        uint64_t reserved_39_60        : 22;
        uint64_t syn                   : 7;  /**< [ 38: 32](RO/H) Syndrome for the single-bit error. */
        uint64_t reserved_0_31         : 32;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_31         : 32;
        uint64_t syn                   : 7;  /**< [ 38: 32](RO/H) Syndrome for the single-bit error. */
        uint64_t reserved_39_60        : 22;
        uint64_t noway                 : 1;  /**< [ 61: 61](RO/H) Information refers to a NOWAY error. */
        uint64_t tagsbe                : 1;  /**< [ 62: 62](RO/H) Information refers to a single-bit TAG ECC error. */
        uint64_t tagdbe                : 1;  /**< [ 63: 63](RO/H) Information refers to a double-bit TAG ECC error. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tadx_ttg_err_s cn8; */
    struct cavm_l2c_tadx_ttg_err_cn81xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t tagdbe                : 1;  /**< [ 63: 63](RO/H) Information refers to a double-bit TAG ECC error. */
        uint64_t tagsbe                : 1;  /**< [ 62: 62](RO/H) Information refers to a single-bit TAG ECC error. */
        uint64_t noway                 : 1;  /**< [ 61: 61](RO/H) Information refers to a NOWAY error. */
        uint64_t reserved_39_60        : 22;
        uint64_t syn                   : 7;  /**< [ 38: 32](RO/H) Syndrome for the single-bit error. */
        uint64_t reserved_21_31        : 11;
        uint64_t way                   : 4;  /**< [ 20: 17](RO/H) Way of the L2 block containing the error. */
        uint64_t l2idx                 : 10; /**< [ 16:  7](RO/H) Index of the L2 block containing the error.
                                                                 See L2C_TAD()_INT_W1C[TAGSBE] for an important use of this field. */
        uint64_t reserved_0_6          : 7;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_6          : 7;
        uint64_t l2idx                 : 10; /**< [ 16:  7](RO/H) Index of the L2 block containing the error.
                                                                 See L2C_TAD()_INT_W1C[TAGSBE] for an important use of this field. */
        uint64_t way                   : 4;  /**< [ 20: 17](RO/H) Way of the L2 block containing the error. */
        uint64_t reserved_21_31        : 11;
        uint64_t syn                   : 7;  /**< [ 38: 32](RO/H) Syndrome for the single-bit error. */
        uint64_t reserved_39_60        : 22;
        uint64_t noway                 : 1;  /**< [ 61: 61](RO/H) Information refers to a NOWAY error. */
        uint64_t tagsbe                : 1;  /**< [ 62: 62](RO/H) Information refers to a single-bit TAG ECC error. */
        uint64_t tagdbe                : 1;  /**< [ 63: 63](RO/H) Information refers to a double-bit TAG ECC error. */
#endif /* Word 0 - End */
    } cn81xx;
    struct cavm_l2c_tadx_ttg_err_cn83xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t tagdbe                : 1;  /**< [ 63: 63](RO/H) Information refers to a double-bit TAG ECC error. */
        uint64_t tagsbe                : 1;  /**< [ 62: 62](RO/H) Information refers to a single-bit TAG ECC error. */
        uint64_t noway                 : 1;  /**< [ 61: 61](RO/H) Information refers to a NOWAY error. */
        uint64_t reserved_39_60        : 22;
        uint64_t syn                   : 7;  /**< [ 38: 32](RO/H) Syndrome for the single-bit error. */
        uint64_t reserved_23_31        : 9;
        uint64_t way                   : 4;  /**< [ 22: 19](RO/H) Way of the L2 block containing the error. */
        uint64_t l2idx                 : 12; /**< [ 18:  7](RO/H) Index of the L2 block containing the error.
                                                                 See L2C_TAD()_INT_W1C[TAGSBE] for an important use of this field. */
        uint64_t reserved_0_6          : 7;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_6          : 7;
        uint64_t l2idx                 : 12; /**< [ 18:  7](RO/H) Index of the L2 block containing the error.
                                                                 See L2C_TAD()_INT_W1C[TAGSBE] for an important use of this field. */
        uint64_t way                   : 4;  /**< [ 22: 19](RO/H) Way of the L2 block containing the error. */
        uint64_t reserved_23_31        : 9;
        uint64_t syn                   : 7;  /**< [ 38: 32](RO/H) Syndrome for the single-bit error. */
        uint64_t reserved_39_60        : 22;
        uint64_t noway                 : 1;  /**< [ 61: 61](RO/H) Information refers to a NOWAY error. */
        uint64_t tagsbe                : 1;  /**< [ 62: 62](RO/H) Information refers to a single-bit TAG ECC error. */
        uint64_t tagdbe                : 1;  /**< [ 63: 63](RO/H) Information refers to a double-bit TAG ECC error. */
#endif /* Word 0 - End */
    } cn83xx;
};
typedef union cavm_l2c_tadx_ttg_err cavm_l2c_tadx_ttg_err_t;

static inline uint64_t CAVM_L2C_TADX_TTG_ERR(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TADX_TTG_ERR(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a==0))
        return 0x87e050060200ll + 0x1000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=3))
        return 0x87e050060200ll + 0x1000000ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_TADX_TTG_ERR", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TADX_TTG_ERR(a) cavm_l2c_tadx_ttg_err_t
#define bustype_CAVM_L2C_TADX_TTG_ERR(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_TADX_TTG_ERR(a) "L2C_TADX_TTG_ERR"
#define device_bar_CAVM_L2C_TADX_TTG_ERR(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TADX_TTG_ERR(a) (a)
#define arguments_CAVM_L2C_TADX_TTG_ERR(a) (a),-1,-1,-1

#endif /* __CAVM_CSRS_L2C_TAD_H__ */
