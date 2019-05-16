#ifndef __CAVM_CSRS_DPI_H__
#define __CAVM_CSRS_DPI_H__
/* This file is auto-generated. Do not edit */

/***********************license start***********************************
* Copyright (C) 2019 Marvell International Ltd.
* SPDX-License-Identifier: BSD-3-Clause
* https://spdx.org/licenses
***********************license end**************************************/


/**
 * @file
 *
 * Configuration and status register (CSR) address and type definitions for
 * OcteonTX DPI.
 *
 * This file is auto generated. Do not edit.
 *
 */

/**
 * Enumeration dpi_bar_e
 *
 * DPI Base Address Register Enumeration
 * Enumerates the base address registers.
 */
#define CAVM_DPI_BAR_E_DPIX_PF_BAR0_CN8(a) (0x86e000000000ll + 0x1000000000ll * (a))
#define CAVM_DPI_BAR_E_DPIX_PF_BAR0_CN8_SIZE 0x100000ull
#define CAVM_DPI_BAR_E_DPIX_PF_BAR0_CN9(a) (0x86e000000000ll + 0x1000000000ll * (a))
#define CAVM_DPI_BAR_E_DPIX_PF_BAR0_CN9_SIZE 0x100000000ull
#define CAVM_DPI_BAR_E_DPIX_PF_BAR4_CN8(a) (0x86e010000000ll + 0x1000000000ll * (a))
#define CAVM_DPI_BAR_E_DPIX_PF_BAR4_CN8_SIZE 0x100000ull
#define CAVM_DPI_BAR_E_DPIX_PF_BAR4_CN9(a) (0x86e100000000ll + 0x1000000000ll * (a))
#define CAVM_DPI_BAR_E_DPIX_PF_BAR4_CN9_SIZE 0x100000ull
#define CAVM_DPI_BAR_E_DPIX_VFX_BAR0_CN8(a,b) (0x86e020000000ll + 0x1000000000ll * (a) + 0x100000ll * (b))
#define CAVM_DPI_BAR_E_DPIX_VFX_BAR0_CN8_SIZE 0x100000ull
#define CAVM_DPI_BAR_E_DPIX_VFX_BAR0_CN9(a,b) (0x86e200000000ll + 0x1000000000ll * (a) + 0x100000ll * (b))
#define CAVM_DPI_BAR_E_DPIX_VFX_BAR0_CN9_SIZE 0x100000ull
#define CAVM_DPI_BAR_E_DPIX_VFX_BAR4_CN8(a,b) (0x86e030000000ll + 0x1000000000ll * (a) + 0x100000ll * (b))
#define CAVM_DPI_BAR_E_DPIX_VFX_BAR4_CN8_SIZE 0x100000ull
#define CAVM_DPI_BAR_E_DPIX_VFX_BAR4_CN9(a,b) (0x86e210000000ll + 0x1000000000ll * (a) + 0x100000ll * (b))
#define CAVM_DPI_BAR_E_DPIX_VFX_BAR4_CN9_SIZE 0x100000ull

/**
 * Enumeration dpi_cs_e
 *
 * DPI Completion Status Enumeration
 * Enumerates the completion status return values.
 */
#define CAVM_DPI_CS_E_ERRRSP (3)
#define CAVM_DPI_CS_E_NOERR (0)
#define CAVM_DPI_CS_E_RDFLT (4)
#define CAVM_DPI_CS_E_RDRST (1)
#define CAVM_DPI_CS_E_WRFLT (5)
#define CAVM_DPI_CS_E_WRRST (2)

/**
 * Enumeration dpi_endianswap_e
 *
 * DPI Endian Swap Mode Enumeration
 * Enumerates the endian swap modes that DPI supports.
 */
#define CAVM_DPI_ENDIANSWAP_E_BYTE_SWAP_32B (2)
#define CAVM_DPI_ENDIANSWAP_E_BYTE_SWAP_64B (1)
#define CAVM_DPI_ENDIANSWAP_E_LW_SWAP_64B (3)
#define CAVM_DPI_ENDIANSWAP_E_PASS_THRU (0)

/**
 * Enumeration dpi_hdr_pt_e
 *
 * DPI Header Pointer Type Enumeration
 * Enumerates the pointer type in DPI_DMA_INSTR_HDR_S[PT].
 */
#define CAVM_DPI_HDR_PT_E_CNT (3)
#define CAVM_DPI_HDR_PT_E_WQP (2)
#define CAVM_DPI_HDR_PT_E_ZBW_CA (0)
#define CAVM_DPI_HDR_PT_E_ZBW_NC (1)

/**
 * Enumeration dpi_hdr_pt_wqp_e
 *
 * DPI DMA Instruction Header Work Queue Pointer \<2:0\> Enumeration
 *
 * The DPI_DMA_INSTR_HDR_S[PTR]\<2:0\> encoding when DPI_DMA_INSTR_HDR_S[PT] is
 * DPI_HDR_PT_E::WQP.
 */
#define CAVM_DPI_HDR_PT_WQP_E_NOSTATUS (0)
#define CAVM_DPI_HDR_PT_WQP_E_STATUSCA (1)
#define CAVM_DPI_HDR_PT_WQP_E_STATUSNC (3)

/**
 * Enumeration dpi_hdr_xtype_e
 *
 * DPI Transfer Type Enumeration
 * Enumerates the pointer type in DPI_DMA_INSTR_HDR_S[XTYPE].
 */
#define CAVM_DPI_HDR_XTYPE_E_EXTERNAL (3)
#define CAVM_DPI_HDR_XTYPE_E_EXTERNAL_ONLY (3)
#define CAVM_DPI_HDR_XTYPE_E_INBOUND (1)
#define CAVM_DPI_HDR_XTYPE_E_INTERNAL (2)
#define CAVM_DPI_HDR_XTYPE_E_INTERNAL_ONLY (2)
#define CAVM_DPI_HDR_XTYPE_E_OUTBOUND (0)

/**
 * Enumeration dpi_pf_int_vec_e
 *
 * DPI MSI-X Vector Enumeration
 * Enumerates the MSI-X interrupt vectors.
 */
#define CAVM_DPI_PF_INT_VEC_E_DPI_CCX_INT(a) (0 + (a))
#define CAVM_DPI_PF_INT_VEC_E_DPI_DBE_INT (0x4a)
#define CAVM_DPI_PF_INT_VEC_E_DPI_EPFX_DMA_VF_LINTX(a,b) (0x78 + 2 * (a) + (b))
#define CAVM_DPI_PF_INT_VEC_E_DPI_EPFX_MISC_LINT(a) (0x98 + (a))
#define CAVM_DPI_PF_INT_VEC_E_DPI_EPFX_PP_VF_LINTX(a,b) (0x58 + 2 * (a) + (b))
#define CAVM_DPI_PF_INT_VEC_E_DPI_INT_REG (0x48)
#define CAVM_DPI_PF_INT_VEC_E_DPI_PF_RAS (0xa8)
#define CAVM_DPI_PF_INT_VEC_E_DPI_REQQX_INT(a) (0x40 + (a))
#define CAVM_DPI_PF_INT_VEC_E_DPI_SBE_INT (0x49)
#define CAVM_DPI_PF_INT_VEC_E_SDP_FLR_RING_LINTX(a) (0x48 + (a))
#define CAVM_DPI_PF_INT_VEC_E_SDP_IRE_LINTX(a) (0x4c + (a))
#define CAVM_DPI_PF_INT_VEC_E_SDP_ORD_LINTX(a) (0x54 + (a))
#define CAVM_DPI_PF_INT_VEC_E_SDP_ORE_LINTX(a) (0x50 + (a))

/**
 * Enumeration dpi_vf_int_vec_e
 *
 * DPI MSI-X Vector Enumeration
 * Enumerates the MSI-X interrupt vectors.
 */
#define CAVM_DPI_VF_INT_VEC_E_DPI_VF_INT (0)

/**
 * Enumeration sdp_rmt_int_vec_e
 *
 * SDP MSI-X Vector to Remote Host Enumeration
 * Enumerates the MSI-X interrupt vectors.
 */
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_DMA_RINT (9)
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_DMA_VF_RINTX(a) (0xa + (a))
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_IRERR_RINT (0)
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_IRE_RINT (0)
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_MBOX_RINT (2)
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_MBOX_RINTX(a) (6 + (a))
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_MISC_RINT (0xe)
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_OEI_RINT_CN8 (3)
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_OEI_RINT_CN9 (8)
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_ORERR_RINT (1)
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_ORE_RINT (1)
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_PP_VF_RINTX(a) (0xc + (a))
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_RX_CNTS(a) (0x10 + (a))
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_RSVD (0xf)
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_RSVDX(a) (8 + (a))
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_VFIRE_RINTX(a) (2 + (a))
#define CAVM_SDP_RMT_INT_VEC_E_SDP_EPF_VFORE_RINTX(a) (4 + (a))
#define CAVM_SDP_RMT_INT_VEC_E_SLI_EPF_DMA_RINT (4)
#define CAVM_SDP_RMT_INT_VEC_E_SLI_EPF_DMA_VF_RINT (7)
#define CAVM_SDP_RMT_INT_VEC_E_SLI_EPF_MISC_RINT (5)
#define CAVM_SDP_RMT_INT_VEC_E_SLI_EPF_PP_VF_RINT (6)

/**
 * Structure dpi_dma_func_sel_s
 *
 * DPI DMA Function Selector Structure
 * DPI DMA function selector for MACs
 */
union cavm_dpi_dma_func_sel_s
{
    uint32_t u;
    struct cavm_dpi_dma_func_sel_s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint32_t reserved_0_31         : 32;
#else /* Word 0 - Little Endian */
        uint32_t reserved_0_31         : 32;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpi_dma_func_sel_s_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint32_t reserved_16_31        : 16;
        uint32_t pf                    : 3;  /**< [ 15: 13] Selects the PCIe physical function within the referenced MAC.

                                                                 [PF] must select a valid physical function in a PCIe MAC.
                                                                 PEM_EP_FUNCTIONS_E lists possible physical functions.

                                                                 [PF] must be zero for any non-PCIe MAC. */
        uint32_t func                  : 13; /**< [ 12:  0] Selects the function within the physical function [PF]
                                                                 within the referenced MAC.

                                                                 0x0 = The physical function [PF].
                                                                 0x1 = The first virtual function within the physical function [PF].
                                                                 0x2 = The second virtual function within the physical function [PF].
                                                                 0x3 = ...

                                                                 When [FUNC] is nonzero, it must select a valid virtual
                                                                 function. PEM_EP_FUNCTIONS_E
                                                                 indicate the possible virtual functions for each [PF], but note
                                                                 that their encodings may not match the [FUNC] encoding.

                                                                 [FUNC] must be zero for any MAC that isn't PCIe, doesn't
                                                                 support the PCIe SR-IOV standard, or doesn't have PCIe SR-IOV
                                                                 enabled. */
#else /* Word 0 - Little Endian */
        uint32_t func                  : 13; /**< [ 12:  0] Selects the function within the physical function [PF]
                                                                 within the referenced MAC.

                                                                 0x0 = The physical function [PF].
                                                                 0x1 = The first virtual function within the physical function [PF].
                                                                 0x2 = The second virtual function within the physical function [PF].
                                                                 0x3 = ...

                                                                 When [FUNC] is nonzero, it must select a valid virtual
                                                                 function. PEM_EP_FUNCTIONS_E
                                                                 indicate the possible virtual functions for each [PF], but note
                                                                 that their encodings may not match the [FUNC] encoding.

                                                                 [FUNC] must be zero for any MAC that isn't PCIe, doesn't
                                                                 support the PCIe SR-IOV standard, or doesn't have PCIe SR-IOV
                                                                 enabled. */
        uint32_t pf                    : 3;  /**< [ 15: 13] Selects the PCIe physical function within the referenced MAC.

                                                                 [PF] must select a valid physical function in a PCIe MAC.
                                                                 PEM_EP_FUNCTIONS_E lists possible physical functions.

                                                                 [PF] must be zero for any non-PCIe MAC. */
        uint32_t reserved_16_31        : 16;
#endif /* Word 0 - End */
    } cn8;
    struct cavm_dpi_dma_func_sel_s_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint32_t reserved_16_31        : 16;
        uint32_t pf                    : 4;  /**< [ 15: 12] Selects the PCIe physical function within the referenced MAC.

                                                                 [PF] must select a valid physical function in a PCIe MAC.

                                                                 [PF] must be zero for any non-PCIe MAC. */
        uint32_t func                  : 12; /**< [ 11:  0] Selects the function within the physical function [PF]
                                                                 within the referenced MAC.

                                                                 0x0 = The physical function [PF].
                                                                 0x1 = The first virtual function within the physical function [PF].
                                                                 0x2 = The second virtual function within the physical function [PF].
                                                                 0x3 = ...

                                                                 When [FUNC] is nonzero, it must select a valid virtual
                                                                 function.

                                                                 [FUNC] must be zero for any MAC that isn't PCIe, doesn't
                                                                 support the PCIe SR-IOV standard, or doesn't have PCIe SR-IOV
                                                                 enabled. */
#else /* Word 0 - Little Endian */
        uint32_t func                  : 12; /**< [ 11:  0] Selects the function within the physical function [PF]
                                                                 within the referenced MAC.

                                                                 0x0 = The physical function [PF].
                                                                 0x1 = The first virtual function within the physical function [PF].
                                                                 0x2 = The second virtual function within the physical function [PF].
                                                                 0x3 = ...

                                                                 When [FUNC] is nonzero, it must select a valid virtual
                                                                 function.

                                                                 [FUNC] must be zero for any MAC that isn't PCIe, doesn't
                                                                 support the PCIe SR-IOV standard, or doesn't have PCIe SR-IOV
                                                                 enabled. */
        uint32_t pf                    : 4;  /**< [ 15: 12] Selects the PCIe physical function within the referenced MAC.

                                                                 [PF] must select a valid physical function in a PCIe MAC.

                                                                 [PF] must be zero for any non-PCIe MAC. */
        uint32_t reserved_16_31        : 16;
#endif /* Word 0 - End */
    } cn9;
};

/**
 * Structure dpi_dma_instr_hdr_s
 *
 * DPI DMA Instruction Header Structure
 * DPI DMA Instruction Header Format
 */
union cavm_dpi_dma_instr_hdr_s
{
    uint64_t u[4];
    struct cavm_dpi_dma_instr_hdr_s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_58_63        : 6;
        uint64_t nlst                  : 4;  /**< [ 57: 54] The number of pointers in the last pointers block. Valid values are 1 through 15.

                                                                 With DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::OUTBOUND, and
                                                                 DPI_HDR_XTYPE_E:EXTERNAL_ONLY instructions, the last pointers block
                                                                 contains MAC pointers.

                                                                 With DPI_HDR_XTYPE_E::INTERNAL_ONLY instructions, the last pointers block
                                                                 contains L2/DRAM pointers.

                                                                 The last pointers block is [NFST] * 2 64-bit words. Note that the sum of the number of
                                                                 64-bit words in the last pointers block and first pointers block must never exceed 60. */
        uint64_t reserved_52_53        : 2;
        uint64_t nfst                  : 4;  /**< [ 51: 48] The number of DPI_DMA_LOCAL_PTR_Ss in the first pointers block. Valid values are 1 thru 15.

                                                                 With DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::OUTBOUND, and
                                                                 DPI_HDR_XTYPE_E::INTERNAL_ONLY instructions, the first pointers block contains
                                                                 L2/DRAM pointers.

                                                                 With DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions, the first pointers
                                                                 block contains MAC pointers

                                                                 The first pointers block is [NFST] * 2 64-bit words. Note that the sum of the number
                                                                 of 64-bit words in the last pointers block and first pointers block must never exceed 60. */
        uint64_t reserved_44_47        : 4;
        uint64_t grp                   : 10; /**< [ 43: 34] SSO guest-group. Sent to SSO upon instruction completion when [PT] = DPI_HDR_PT_E::WQP.
                                                                 For the SSO to not discard the add-work request, SSO_PF_MAP() must map
                                                                 [GRP] and DPI()_DMA()_IDS[GMID] as valid.

                                                                 [GRP] must be zero when [PT] != DPI_HDR_PT_E::WQP. */
        uint64_t tt                    : 2;  /**< [ 33: 32] SSO tag type. Sent to SSO upon instruction completion when [PT] = DPI_HDR_PT_E::WQP.

                                                                 [TT] must be zero when [PT] != DPI_HDR_PT_E::WQP. */
        uint64_t tag                   : 32; /**< [ 31:  0] SSO Tag */
#else /* Word 0 - Little Endian */
        uint64_t tag                   : 32; /**< [ 31:  0] SSO Tag */
        uint64_t tt                    : 2;  /**< [ 33: 32] SSO tag type. Sent to SSO upon instruction completion when [PT] = DPI_HDR_PT_E::WQP.

                                                                 [TT] must be zero when [PT] != DPI_HDR_PT_E::WQP. */
        uint64_t grp                   : 10; /**< [ 43: 34] SSO guest-group. Sent to SSO upon instruction completion when [PT] = DPI_HDR_PT_E::WQP.
                                                                 For the SSO to not discard the add-work request, SSO_PF_MAP() must map
                                                                 [GRP] and DPI()_DMA()_IDS[GMID] as valid.

                                                                 [GRP] must be zero when [PT] != DPI_HDR_PT_E::WQP. */
        uint64_t reserved_44_47        : 4;
        uint64_t nfst                  : 4;  /**< [ 51: 48] The number of DPI_DMA_LOCAL_PTR_Ss in the first pointers block. Valid values are 1 thru 15.

                                                                 With DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::OUTBOUND, and
                                                                 DPI_HDR_XTYPE_E::INTERNAL_ONLY instructions, the first pointers block contains
                                                                 L2/DRAM pointers.

                                                                 With DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions, the first pointers
                                                                 block contains MAC pointers

                                                                 The first pointers block is [NFST] * 2 64-bit words. Note that the sum of the number
                                                                 of 64-bit words in the last pointers block and first pointers block must never exceed 60. */
        uint64_t reserved_52_53        : 2;
        uint64_t nlst                  : 4;  /**< [ 57: 54] The number of pointers in the last pointers block. Valid values are 1 through 15.

                                                                 With DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::OUTBOUND, and
                                                                 DPI_HDR_XTYPE_E:EXTERNAL_ONLY instructions, the last pointers block
                                                                 contains MAC pointers.

                                                                 With DPI_HDR_XTYPE_E::INTERNAL_ONLY instructions, the last pointers block
                                                                 contains L2/DRAM pointers.

                                                                 The last pointers block is [NFST] * 2 64-bit words. Note that the sum of the number of
                                                                 64-bit words in the last pointers block and first pointers block must never exceed 60. */
        uint64_t reserved_58_63        : 6;
#endif /* Word 0 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 1 - Big Endian */
        uint64_t reserved_126_127      : 2;
        uint64_t lport                 : 2;  /**< [125:124] Port for the last pointers block.

                                                                 DPI sends MAC memory space reads and writes for the MAC addresses
                                                                 in the last pointers block to the MAC selected by [LPORT]
                                                                 while processing DPI_HDR_XTYPE_E::OUTBOUND, DPI_HDR_XTYPE_E::INBOUND,
                                                                 and DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions.

                                                                 [LPORT] must be zero for DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 instructions. */
        uint64_t reserved_122_123      : 2;
        uint64_t fport                 : 2;  /**< [121:120] Port for the first pointers block.

                                                                 DPI sends MAC memory space reads for the MAC addresses in the first
                                                                 pointers block to the MAC selected by [FPORT] while processing
                                                                 a DPI_HDR_XTYPE_E::EXTERNAL_ONLY DPI DMA instruction.

                                                                 [FPORT] must be zero for DPI_HDR_XTYPE_E::OUTBOUND,
                                                                 DPI_HDR_XTYPE_E::INBOUND, and DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 instructions. */
        uint64_t reserved_114_119      : 6;
        uint64_t xtype                 : 2;  /**< [113:112] Transfer type of the instruction. Enumerated by DPI_HDR_XTYPE_E. Each DPI DMA
                                                                 instruction can be DPI_HDR_XTYPE_E::OUTBOUND (L2/DRAM-\>MAC),
                                                                 DPI_HDR_XTYPE_E::INBOUND (MAC-\>L2/DRAM), DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 (L2/DRAM-\>L2/DRAM), or DPI_HDR_XTYPE_E::EXTERNAL_ONLY (MAC-\>MAC). */
        uint64_t reserved_109_111      : 3;
        uint64_t csel                  : 1;  /**< [108:108] Counter and interrupt select. See [CA] and [FI]. [CSEL] selects which of two counters
                                                                 (SLI()_EPF()_DMA_CNT()[CNT]) and/or two interrupt bits (SLI()_EPF()_DMA_RINT[DMAFI])
                                                                 DPI can modify during DPI_HDR_XTYPE_E::OUTBOUND or DPI_HDR_XTYPE_E::EXTERNAL_ONLY
                                                                 instruction execution.

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL_ONLY DPI DMA
                                                                 instructions, [CSEL] must be zero. */
        uint64_t ca                    : 1;  /**< [107:107] Add to a counter that can interrupt a remote host.

                                                                 When [CA] = 1, DPI updates a selected counter after it completes the DMA
                                                                 DPI_HDR_XTYPE_E::OUTBOUND or DPI_HDR_XTYPE_E::EXTERNAL_ONLY instruction.

                                                                 _ If [CSEL] = 0, DPI updates SLI(0)_EPF()_DMA_CNT(0)[CNT].
                                                                 _ If [CSEL] = 1, DPI updates SLI(0)_EPF()_DMA_CNT(1)[CNT].

                                                                 [LPORT] and [DEALLOCV] determine the EPF used. ([DEALLOCV] is
                                                                 only relevant when [PVFE]=1. Only the DPI_DMA_FUNC_SEL_S[PF]
                                                                 of [DEALLOCV] is relevant here.)

                                                                 Note that these updates may indirectly cause corresponding
                                                                 SLI()_EPF()_DMA_RINT[DCNT,DTIME] to become set (depending on the
                                                                 SLI()_EPF()_DMA_INT_LEVEL() settings), so may cause interrupts to
                                                                 be sent to a remote MAC host.

                                                                 If DPI()_DMA_CONTROL[O_ADD1] = 1, DPI updates the counter by 1.

                                                                 If DPI()_DMA_CONTROL[O_ADD1] = 0, DPI updates the counter by the total bytes in
                                                                 the transfer.

                                                                 When [CA] = 0, DPI does not update any SLI()_EPF()_DMA_CNT()[CNT].

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL_ONLY DPI DMA
                                                                 instructions, [CA] must never be set, and DPI never adds to any
                                                                 SLI()_EPF()_DMA_CNT[CNT]. */
        uint64_t fi                    : 1;  /**< [106:106] Force interrupt to a remote host.

                                                                 When [FI] is set for a (DPI_HDR_XTYPE_E::OUTBOUND or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY) DPI DMA instruction, DPI sets an
                                                                 interrupt bit after completing instruction. This may
                                                                 cause an interrupt to be sent to a remote host.

                                                                 _ If [CSEL] = 0, DPI sets SLI()_EPF()_DMA_RINT[DMAFI\<0\>].
                                                                 _ If [CSEL] = 1, DPI sets SLI()_EPF()_DMA_RINT[DMAFI\<1\>].

                                                                 [LPORT] and [DEALLOCV] determine the EPF used. ([DEALLOCV] is
                                                                 only relevant when [PVFE]=1. Only the DPI_DMA_FUNC_SEL_S[PF]
                                                                 of [DEALLOCV] is relevant here.)

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 instructions, [II] must be clear, and DPI never sets DMAFI interrupt bits. */
        uint64_t ii                    : 1;  /**< [105:105] Ignore I. Determines if DPI_DMA_LOCAL_PTR_S[I]'s influence whether
                                                                 DPI frees a DPI_DMA_LOCAL_PTR_S[PTR] during DPI_HDR_XTYPE_E::OUTBOUND
                                                                 instruction processing.

                                                                 If [II] is set, [FL] solely determines whether DPI frees, and
                                                                 all DPI_DMA_LOCAL_PTR_S[PTR]'s in the DPI_HDR_XTYPE_E::OUTBOUND
                                                                 instruction are either freed or not.

                                                                 If [II] is clear, ([FL] XOR DPI_DMA_LOCAL_PTR_S[I]) determines
                                                                 whether DPI frees a given DPI_DMA_LOCAL_PTR_S[PTR] in the
                                                                 DPI_HDR_XTYPE_E::OUTBOUND instruction, and each .

                                                                 For DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::INTERNAL_ONLY, or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions, [II] must be clear,
                                                                 and DPI never frees local buffers. */
        uint64_t fl                    : 1;  /**< [104:104] Determines whether DPI frees a DPI_DMA_LOCAL_PTR_S[PTR] during
                                                                 DPI_HDR_XTYPE_E::OUTBOUND instruction processing
                                                                 (along with [II] and DPI_DMA_LOCAL_PTR_S[I]).

                                                                 During DPI_HDR_XTYPE_E::OUTBOUND instruction processing, DPI frees a
                                                                 DPI_DMA_LOCAL_PTR_S[PTR] to [AURA] in FPA when:

                                                                 _ [FL] XOR (![II] AND DPI_DMA_LOCAL_PTR_S[I])

                                                                 For DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::INTERNAL_ONLY, or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions, [FL] must be clear,
                                                                 and DPI never frees local buffers. */
        uint64_t reserved_102_103      : 2;
        uint64_t pt                    : 2;  /**< [101:100] Pointer type. Enumerated by DPI_HDR_PT_E. Indicates how [PTR] is used
                                                                 upon completion of the DPI DMA instruction: byte write, SSO
                                                                 work add, or counter add.

                                                                 If no completion indication is desired for the DPI DMA instruction,
                                                                 software should set [PT]=DPI_HDR_PT_E::ZBW_CA and [PTR]=0. */
        uint64_t reserved_64_99        : 36;
#else /* Word 1 - Little Endian */
        uint64_t reserved_64_99        : 36;
        uint64_t pt                    : 2;  /**< [101:100] Pointer type. Enumerated by DPI_HDR_PT_E. Indicates how [PTR] is used
                                                                 upon completion of the DPI DMA instruction: byte write, SSO
                                                                 work add, or counter add.

                                                                 If no completion indication is desired for the DPI DMA instruction,
                                                                 software should set [PT]=DPI_HDR_PT_E::ZBW_CA and [PTR]=0. */
        uint64_t reserved_102_103      : 2;
        uint64_t fl                    : 1;  /**< [104:104] Determines whether DPI frees a DPI_DMA_LOCAL_PTR_S[PTR] during
                                                                 DPI_HDR_XTYPE_E::OUTBOUND instruction processing
                                                                 (along with [II] and DPI_DMA_LOCAL_PTR_S[I]).

                                                                 During DPI_HDR_XTYPE_E::OUTBOUND instruction processing, DPI frees a
                                                                 DPI_DMA_LOCAL_PTR_S[PTR] to [AURA] in FPA when:

                                                                 _ [FL] XOR (![II] AND DPI_DMA_LOCAL_PTR_S[I])

                                                                 For DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::INTERNAL_ONLY, or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions, [FL] must be clear,
                                                                 and DPI never frees local buffers. */
        uint64_t ii                    : 1;  /**< [105:105] Ignore I. Determines if DPI_DMA_LOCAL_PTR_S[I]'s influence whether
                                                                 DPI frees a DPI_DMA_LOCAL_PTR_S[PTR] during DPI_HDR_XTYPE_E::OUTBOUND
                                                                 instruction processing.

                                                                 If [II] is set, [FL] solely determines whether DPI frees, and
                                                                 all DPI_DMA_LOCAL_PTR_S[PTR]'s in the DPI_HDR_XTYPE_E::OUTBOUND
                                                                 instruction are either freed or not.

                                                                 If [II] is clear, ([FL] XOR DPI_DMA_LOCAL_PTR_S[I]) determines
                                                                 whether DPI frees a given DPI_DMA_LOCAL_PTR_S[PTR] in the
                                                                 DPI_HDR_XTYPE_E::OUTBOUND instruction, and each .

                                                                 For DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::INTERNAL_ONLY, or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions, [II] must be clear,
                                                                 and DPI never frees local buffers. */
        uint64_t fi                    : 1;  /**< [106:106] Force interrupt to a remote host.

                                                                 When [FI] is set for a (DPI_HDR_XTYPE_E::OUTBOUND or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY) DPI DMA instruction, DPI sets an
                                                                 interrupt bit after completing instruction. This may
                                                                 cause an interrupt to be sent to a remote host.

                                                                 _ If [CSEL] = 0, DPI sets SLI()_EPF()_DMA_RINT[DMAFI\<0\>].
                                                                 _ If [CSEL] = 1, DPI sets SLI()_EPF()_DMA_RINT[DMAFI\<1\>].

                                                                 [LPORT] and [DEALLOCV] determine the EPF used. ([DEALLOCV] is
                                                                 only relevant when [PVFE]=1. Only the DPI_DMA_FUNC_SEL_S[PF]
                                                                 of [DEALLOCV] is relevant here.)

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 instructions, [II] must be clear, and DPI never sets DMAFI interrupt bits. */
        uint64_t ca                    : 1;  /**< [107:107] Add to a counter that can interrupt a remote host.

                                                                 When [CA] = 1, DPI updates a selected counter after it completes the DMA
                                                                 DPI_HDR_XTYPE_E::OUTBOUND or DPI_HDR_XTYPE_E::EXTERNAL_ONLY instruction.

                                                                 _ If [CSEL] = 0, DPI updates SLI(0)_EPF()_DMA_CNT(0)[CNT].
                                                                 _ If [CSEL] = 1, DPI updates SLI(0)_EPF()_DMA_CNT(1)[CNT].

                                                                 [LPORT] and [DEALLOCV] determine the EPF used. ([DEALLOCV] is
                                                                 only relevant when [PVFE]=1. Only the DPI_DMA_FUNC_SEL_S[PF]
                                                                 of [DEALLOCV] is relevant here.)

                                                                 Note that these updates may indirectly cause corresponding
                                                                 SLI()_EPF()_DMA_RINT[DCNT,DTIME] to become set (depending on the
                                                                 SLI()_EPF()_DMA_INT_LEVEL() settings), so may cause interrupts to
                                                                 be sent to a remote MAC host.

                                                                 If DPI()_DMA_CONTROL[O_ADD1] = 1, DPI updates the counter by 1.

                                                                 If DPI()_DMA_CONTROL[O_ADD1] = 0, DPI updates the counter by the total bytes in
                                                                 the transfer.

                                                                 When [CA] = 0, DPI does not update any SLI()_EPF()_DMA_CNT()[CNT].

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL_ONLY DPI DMA
                                                                 instructions, [CA] must never be set, and DPI never adds to any
                                                                 SLI()_EPF()_DMA_CNT[CNT]. */
        uint64_t csel                  : 1;  /**< [108:108] Counter and interrupt select. See [CA] and [FI]. [CSEL] selects which of two counters
                                                                 (SLI()_EPF()_DMA_CNT()[CNT]) and/or two interrupt bits (SLI()_EPF()_DMA_RINT[DMAFI])
                                                                 DPI can modify during DPI_HDR_XTYPE_E::OUTBOUND or DPI_HDR_XTYPE_E::EXTERNAL_ONLY
                                                                 instruction execution.

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL_ONLY DPI DMA
                                                                 instructions, [CSEL] must be zero. */
        uint64_t reserved_109_111      : 3;
        uint64_t xtype                 : 2;  /**< [113:112] Transfer type of the instruction. Enumerated by DPI_HDR_XTYPE_E. Each DPI DMA
                                                                 instruction can be DPI_HDR_XTYPE_E::OUTBOUND (L2/DRAM-\>MAC),
                                                                 DPI_HDR_XTYPE_E::INBOUND (MAC-\>L2/DRAM), DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 (L2/DRAM-\>L2/DRAM), or DPI_HDR_XTYPE_E::EXTERNAL_ONLY (MAC-\>MAC). */
        uint64_t reserved_114_119      : 6;
        uint64_t fport                 : 2;  /**< [121:120] Port for the first pointers block.

                                                                 DPI sends MAC memory space reads for the MAC addresses in the first
                                                                 pointers block to the MAC selected by [FPORT] while processing
                                                                 a DPI_HDR_XTYPE_E::EXTERNAL_ONLY DPI DMA instruction.

                                                                 [FPORT] must be zero for DPI_HDR_XTYPE_E::OUTBOUND,
                                                                 DPI_HDR_XTYPE_E::INBOUND, and DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 instructions. */
        uint64_t reserved_122_123      : 2;
        uint64_t lport                 : 2;  /**< [125:124] Port for the last pointers block.

                                                                 DPI sends MAC memory space reads and writes for the MAC addresses
                                                                 in the last pointers block to the MAC selected by [LPORT]
                                                                 while processing DPI_HDR_XTYPE_E::OUTBOUND, DPI_HDR_XTYPE_E::INBOUND,
                                                                 and DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions.

                                                                 [LPORT] must be zero for DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 instructions. */
        uint64_t reserved_126_127      : 2;
#endif /* Word 1 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 2 - Big Endian */
        uint64_t ptr                   : 64; /**< [191:128] Completion pointer. Usage determined by [PT] value. The DPI_HDR_PT_E
                                                                 enumeration describes the supported [PT] values and the [PTR] usage
                                                                 and requirements in each case.
                                                                 Bits \<63:49\> are ignored by hardware; software should use a sign-extended bit
                                                                 \<48\> for forward compatibility. */
#else /* Word 2 - Little Endian */
        uint64_t ptr                   : 64; /**< [191:128] Completion pointer. Usage determined by [PT] value. The DPI_HDR_PT_E
                                                                 enumeration describes the supported [PT] values and the [PTR] usage
                                                                 and requirements in each case.
                                                                 Bits \<63:49\> are ignored by hardware; software should use a sign-extended bit
                                                                 \<48\> for forward compatibility. */
#endif /* Word 2 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 3 - Big Endian */
        uint64_t reserved_192_255      : 64;
#else /* Word 3 - Little Endian */
        uint64_t reserved_192_255      : 64;
#endif /* Word 3 - End */
    } s;
    struct cavm_dpi_dma_instr_hdr_s_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_60_63        : 4;
        uint64_t reserved_58_59        : 2;
        uint64_t nlst                  : 4;  /**< [ 57: 54] The number of pointers in the last pointers block. Valid values are 1 through 15.

                                                                 With DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::OUTBOUND, and
                                                                 DPI_HDR_XTYPE_E:EXTERNAL_ONLY instructions, the last pointers block
                                                                 contains MAC pointers.

                                                                 With DPI_HDR_XTYPE_E::INTERNAL_ONLY instructions, the last pointers block
                                                                 contains L2/DRAM pointers.

                                                                 The last pointers block is [NFST] * 2 64-bit words. Note that the sum of the number of
                                                                 64-bit words in the last pointers block and first pointers block must never exceed 60. */
        uint64_t reserved_52_53        : 2;
        uint64_t nfst                  : 4;  /**< [ 51: 48] The number of DPI_DMA_LOCAL_PTR_Ss in the first pointers block. Valid values are 1 thru 15.

                                                                 With DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::OUTBOUND, and
                                                                 DPI_HDR_XTYPE_E::INTERNAL_ONLY instructions, the first pointers block contains
                                                                 L2/DRAM pointers.

                                                                 With DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions, the first pointers
                                                                 block contains MAC pointers

                                                                 The first pointers block is [NFST] * 2 64-bit words. Note that the sum of the number
                                                                 of 64-bit words in the last pointers block and first pointers block must never exceed 60. */
        uint64_t reserved_46_47        : 2;
        uint64_t reserved_44_45        : 2;
        uint64_t grp                   : 10; /**< [ 43: 34] SSO guest-group. Sent to SSO upon instruction completion when [PT] = DPI_HDR_PT_E::WQP.
                                                                 For the SSO to not discard the add-work request, SSO_PF_MAP() must map
                                                                 [GRP] and DPI()_DMA()_IDS[GMID] as valid.

                                                                 [GRP] must be zero when [PT] != DPI_HDR_PT_E::WQP. */
        uint64_t tt                    : 2;  /**< [ 33: 32] SSO tag type. Sent to SSO upon instruction completion when [PT] = DPI_HDR_PT_E::WQP.

                                                                 [TT] must be zero when [PT] != DPI_HDR_PT_E::WQP. */
        uint64_t tag                   : 32; /**< [ 31:  0] SSO Tag */
#else /* Word 0 - Little Endian */
        uint64_t tag                   : 32; /**< [ 31:  0] SSO Tag */
        uint64_t tt                    : 2;  /**< [ 33: 32] SSO tag type. Sent to SSO upon instruction completion when [PT] = DPI_HDR_PT_E::WQP.

                                                                 [TT] must be zero when [PT] != DPI_HDR_PT_E::WQP. */
        uint64_t grp                   : 10; /**< [ 43: 34] SSO guest-group. Sent to SSO upon instruction completion when [PT] = DPI_HDR_PT_E::WQP.
                                                                 For the SSO to not discard the add-work request, SSO_PF_MAP() must map
                                                                 [GRP] and DPI()_DMA()_IDS[GMID] as valid.

                                                                 [GRP] must be zero when [PT] != DPI_HDR_PT_E::WQP. */
        uint64_t reserved_44_45        : 2;
        uint64_t reserved_46_47        : 2;
        uint64_t nfst                  : 4;  /**< [ 51: 48] The number of DPI_DMA_LOCAL_PTR_Ss in the first pointers block. Valid values are 1 thru 15.

                                                                 With DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::OUTBOUND, and
                                                                 DPI_HDR_XTYPE_E::INTERNAL_ONLY instructions, the first pointers block contains
                                                                 L2/DRAM pointers.

                                                                 With DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions, the first pointers
                                                                 block contains MAC pointers

                                                                 The first pointers block is [NFST] * 2 64-bit words. Note that the sum of the number
                                                                 of 64-bit words in the last pointers block and first pointers block must never exceed 60. */
        uint64_t reserved_52_53        : 2;
        uint64_t nlst                  : 4;  /**< [ 57: 54] The number of pointers in the last pointers block. Valid values are 1 through 15.

                                                                 With DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::OUTBOUND, and
                                                                 DPI_HDR_XTYPE_E:EXTERNAL_ONLY instructions, the last pointers block
                                                                 contains MAC pointers.

                                                                 With DPI_HDR_XTYPE_E::INTERNAL_ONLY instructions, the last pointers block
                                                                 contains L2/DRAM pointers.

                                                                 The last pointers block is [NFST] * 2 64-bit words. Note that the sum of the number of
                                                                 64-bit words in the last pointers block and first pointers block must never exceed 60. */
        uint64_t reserved_58_59        : 2;
        uint64_t reserved_60_63        : 4;
#endif /* Word 0 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 1 - Big Endian */
        uint64_t reserved_126_127      : 2;
        uint64_t lport                 : 2;  /**< [125:124] Port for the last pointers block.

                                                                 DPI sends MAC memory space reads and writes for the MAC addresses
                                                                 in the last pointers block to the MAC selected by [LPORT]
                                                                 while processing DPI_HDR_XTYPE_E::OUTBOUND, DPI_HDR_XTYPE_E::INBOUND,
                                                                 and DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions.

                                                                 [LPORT] must be zero for DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 instructions. */
        uint64_t reserved_122_123      : 2;
        uint64_t fport                 : 2;  /**< [121:120] Port for the first pointers block.

                                                                 DPI sends MAC memory space reads for the MAC addresses in the first
                                                                 pointers block to the MAC selected by [FPORT] while processing
                                                                 a DPI_HDR_XTYPE_E::EXTERNAL_ONLY DPI DMA instruction.

                                                                 [FPORT] must be zero for DPI_HDR_XTYPE_E::OUTBOUND,
                                                                 DPI_HDR_XTYPE_E::INBOUND, and DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 instructions. */
        uint64_t reserved_116_119      : 4;
        uint64_t reserved_114_115      : 2;
        uint64_t xtype                 : 2;  /**< [113:112] Transfer type of the instruction. Enumerated by DPI_HDR_XTYPE_E. Each DPI DMA
                                                                 instruction can be DPI_HDR_XTYPE_E::OUTBOUND (L2/DRAM-\>MAC),
                                                                 DPI_HDR_XTYPE_E::INBOUND (MAC-\>L2/DRAM), DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 (L2/DRAM-\>L2/DRAM), or DPI_HDR_XTYPE_E::EXTERNAL_ONLY (MAC-\>MAC). */
        uint64_t reserved_109_111      : 3;
        uint64_t csel                  : 1;  /**< [108:108] Counter and interrupt select. See [CA] and [FI]. [CSEL] selects which of two counters
                                                                 (SLI()_EPF()_DMA_CNT()[CNT]) and/or two interrupt bits (SLI()_EPF()_DMA_RINT[DMAFI])
                                                                 DPI can modify during DPI_HDR_XTYPE_E::OUTBOUND or DPI_HDR_XTYPE_E::EXTERNAL_ONLY
                                                                 instruction execution.

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL_ONLY DPI DMA
                                                                 instructions, [CSEL] must be zero. */
        uint64_t ca                    : 1;  /**< [107:107] Add to a counter that can interrupt a remote host.

                                                                 When [CA] = 1, DPI updates a selected counter after it completes the DMA
                                                                 DPI_HDR_XTYPE_E::OUTBOUND or DPI_HDR_XTYPE_E::EXTERNAL_ONLY instruction.

                                                                 _ If [CSEL] = 0, DPI updates SLI(0)_EPF()_DMA_CNT(0)[CNT].
                                                                 _ If [CSEL] = 1, DPI updates SLI(0)_EPF()_DMA_CNT(1)[CNT].

                                                                 [LPORT] and [DEALLOCV] determine the EPF used. ([DEALLOCV] is
                                                                 only relevant when [PVFE]=1. Only the DPI_DMA_FUNC_SEL_S[PF]
                                                                 of [DEALLOCV] is relevant here.)

                                                                 Note that these updates may indirectly cause corresponding
                                                                 SLI()_EPF()_DMA_RINT[DCNT,DTIME] to become set (depending on the
                                                                 SLI()_EPF()_DMA_INT_LEVEL() settings), so may cause interrupts to
                                                                 be sent to a remote MAC host.

                                                                 If DPI()_DMA_CONTROL[O_ADD1] = 1, DPI updates the counter by 1.

                                                                 If DPI()_DMA_CONTROL[O_ADD1] = 0, DPI updates the counter by the total bytes in
                                                                 the transfer.

                                                                 When [CA] = 0, DPI does not update any SLI()_EPF()_DMA_CNT()[CNT].

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL_ONLY DPI DMA
                                                                 instructions, [CA] must never be set, and DPI never adds to any
                                                                 SLI()_EPF()_DMA_CNT[CNT]. */
        uint64_t fi                    : 1;  /**< [106:106] Force interrupt to a remote host.

                                                                 When [FI] is set for a (DPI_HDR_XTYPE_E::OUTBOUND or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY) DPI DMA instruction, DPI sets an
                                                                 interrupt bit after completing instruction. This may
                                                                 cause an interrupt to be sent to a remote host.

                                                                 _ If [CSEL] = 0, DPI sets SLI()_EPF()_DMA_RINT[DMAFI\<0\>].
                                                                 _ If [CSEL] = 1, DPI sets SLI()_EPF()_DMA_RINT[DMAFI\<1\>].

                                                                 [LPORT] and [DEALLOCV] determine the EPF used. ([DEALLOCV] is
                                                                 only relevant when [PVFE]=1. Only the DPI_DMA_FUNC_SEL_S[PF]
                                                                 of [DEALLOCV] is relevant here.)

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 instructions, [II] must be clear, and DPI never sets DMAFI interrupt bits. */
        uint64_t ii                    : 1;  /**< [105:105] Ignore I. Determines if DPI_DMA_LOCAL_PTR_S[I]'s influence whether
                                                                 DPI frees a DPI_DMA_LOCAL_PTR_S[PTR] during DPI_HDR_XTYPE_E::OUTBOUND
                                                                 instruction processing.

                                                                 If [II] is set, [FL] solely determines whether DPI frees, and
                                                                 all DPI_DMA_LOCAL_PTR_S[PTR]'s in the DPI_HDR_XTYPE_E::OUTBOUND
                                                                 instruction are either freed or not.

                                                                 If [II] is clear, ([FL] XOR DPI_DMA_LOCAL_PTR_S[I]) determines
                                                                 whether DPI frees a given DPI_DMA_LOCAL_PTR_S[PTR] in the
                                                                 DPI_HDR_XTYPE_E::OUTBOUND instruction, and each .

                                                                 For DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::INTERNAL_ONLY, or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions, [II] must be clear,
                                                                 and DPI never frees local buffers. */
        uint64_t fl                    : 1;  /**< [104:104] Determines whether DPI frees a DPI_DMA_LOCAL_PTR_S[PTR] during
                                                                 DPI_HDR_XTYPE_E::OUTBOUND instruction processing
                                                                 (along with [II] and DPI_DMA_LOCAL_PTR_S[I]).

                                                                 During DPI_HDR_XTYPE_E::OUTBOUND instruction processing, DPI frees a
                                                                 DPI_DMA_LOCAL_PTR_S[PTR] to [AURA] in FPA when:

                                                                 _ [FL] XOR (![II] AND DPI_DMA_LOCAL_PTR_S[I])

                                                                 For DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::INTERNAL_ONLY, or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions, [FL] must be clear,
                                                                 and DPI never frees local buffers. */
        uint64_t reserved_102_103      : 2;
        uint64_t pt                    : 2;  /**< [101:100] Pointer type. Enumerated by DPI_HDR_PT_E. Indicates how [PTR] is used
                                                                 upon completion of the DPI DMA instruction: byte write, SSO
                                                                 work add, or counter add.

                                                                 If no completion indication is desired for the DPI DMA instruction,
                                                                 software should set [PT]=DPI_HDR_PT_E::ZBW_CA and [PTR]=0. */
        uint64_t reserved_98_99        : 2;
        uint64_t pvfe                  : 1;  /**< [ 97: 97] Function enable. When [PVFE] is set, DPI directs all MAC reads/writes
                                                                 to the function (physical or virtual) that [DEALLOCV] selects within
                                                                 MAC/port [LPORT]. When [PVFE] is clear, DPI directs all MAC reads/writes
                                                                 to physical function 0 within the MACs/ports [LPORT] or [FPORT]. [PVFE]
                                                                 must not be set when [DEALLOCE] is set. [PVFE] must not be set when
                                                                 [XTYPE] is DPI_HDR_XTYPE_E::INTERNAL_ONLY or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY. [PVFE] can only be set when MAC/PORT
                                                                 [LPORT] is a PCIe MAC that either has multiple physical functions or
                                                                 has SR-IOV enabled. */
        uint64_t dealloce              : 1;  /**< [ 96: 96] Aura count subtract enable. When [DEALLOCE] is set, DPI subtracts
                                                                 [DEALLOCV] from the FPA aura count selected by [AURA]. When [DEALLOCE]
                                                                 is clear, DPI does not subtract [DEALLOCV] from any aura count.
                                                                 [DEALLOCE] can only be set when [XTYPE]=DPI_HDR_XTYPE_E::OUTBOUND.
                                                                 [DEALLOCE] must not be set when [PVFE] is set. [DEALLOCV] must not be 0
                                                                 when [DEALLOCE] is set.

                                                                 The [DEALLOCV] aura count subtract is in addition to other
                                                                 aura count activity. When FPA_AURA([AURA])_CFG[PTR_DIS]==0, FPA also
                                                                 decrements the aura count by one for each L2/DRAM pointer free. */
        uint64_t deallocv              : 16; /**< [ 95: 80] The DEALLOCV/FUNC field.

                                                                 When [DEALLOCE] is set, [DEALLOCV] is the value to decrement the aura count on
                                                                 the instruction's final pointer return.

                                                                 When [PVFE] is set, DPI_DMA_FUNC_SEL_S describes the [DEALLOCV] format.
                                                                 [DEALLOCV] selects the function within the MAC selected by [LPORT]
                                                                 when [PVFE] is set. */
        uint64_t reserved_76_79        : 4;
        uint64_t aura                  : 12; /**< [ 75: 64] FPA guest-aura.  The FPA guest-aura DPI uses for all FPA transactions for the
                                                                 DPI DMA instruction. [AURA] can only be used when
                                                                 [XTYPE]=DPI_HDR_XTYPE_E::OUTBOUND, and must be zero otherwise.

                                                                 For the FPA to not discard the request, FPA_PF_MAP() must map
                                                                 [AURA] and DPI()_DMA()_IDS[GMID] as valid.

                                                                 During an DPI_HDR_XTYPE_E::OUTBOUND DPI DMA instruction, [FL], [II],
                                                                 and DPI_DMA_LOCAL_PTR_S[I] determine whether DPI frees a
                                                                 L2/DRAM address to [AURA] in FPA, and [DEALLOCE] determines
                                                                 whether DPI subtracts [DEALLOCV] from [AURA]'s FPA aura counter. */
#else /* Word 1 - Little Endian */
        uint64_t aura                  : 12; /**< [ 75: 64] FPA guest-aura.  The FPA guest-aura DPI uses for all FPA transactions for the
                                                                 DPI DMA instruction. [AURA] can only be used when
                                                                 [XTYPE]=DPI_HDR_XTYPE_E::OUTBOUND, and must be zero otherwise.

                                                                 For the FPA to not discard the request, FPA_PF_MAP() must map
                                                                 [AURA] and DPI()_DMA()_IDS[GMID] as valid.

                                                                 During an DPI_HDR_XTYPE_E::OUTBOUND DPI DMA instruction, [FL], [II],
                                                                 and DPI_DMA_LOCAL_PTR_S[I] determine whether DPI frees a
                                                                 L2/DRAM address to [AURA] in FPA, and [DEALLOCE] determines
                                                                 whether DPI subtracts [DEALLOCV] from [AURA]'s FPA aura counter. */
        uint64_t reserved_76_79        : 4;
        uint64_t deallocv              : 16; /**< [ 95: 80] The DEALLOCV/FUNC field.

                                                                 When [DEALLOCE] is set, [DEALLOCV] is the value to decrement the aura count on
                                                                 the instruction's final pointer return.

                                                                 When [PVFE] is set, DPI_DMA_FUNC_SEL_S describes the [DEALLOCV] format.
                                                                 [DEALLOCV] selects the function within the MAC selected by [LPORT]
                                                                 when [PVFE] is set. */
        uint64_t dealloce              : 1;  /**< [ 96: 96] Aura count subtract enable. When [DEALLOCE] is set, DPI subtracts
                                                                 [DEALLOCV] from the FPA aura count selected by [AURA]. When [DEALLOCE]
                                                                 is clear, DPI does not subtract [DEALLOCV] from any aura count.
                                                                 [DEALLOCE] can only be set when [XTYPE]=DPI_HDR_XTYPE_E::OUTBOUND.
                                                                 [DEALLOCE] must not be set when [PVFE] is set. [DEALLOCV] must not be 0
                                                                 when [DEALLOCE] is set.

                                                                 The [DEALLOCV] aura count subtract is in addition to other
                                                                 aura count activity. When FPA_AURA([AURA])_CFG[PTR_DIS]==0, FPA also
                                                                 decrements the aura count by one for each L2/DRAM pointer free. */
        uint64_t pvfe                  : 1;  /**< [ 97: 97] Function enable. When [PVFE] is set, DPI directs all MAC reads/writes
                                                                 to the function (physical or virtual) that [DEALLOCV] selects within
                                                                 MAC/port [LPORT]. When [PVFE] is clear, DPI directs all MAC reads/writes
                                                                 to physical function 0 within the MACs/ports [LPORT] or [FPORT]. [PVFE]
                                                                 must not be set when [DEALLOCE] is set. [PVFE] must not be set when
                                                                 [XTYPE] is DPI_HDR_XTYPE_E::INTERNAL_ONLY or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY. [PVFE] can only be set when MAC/PORT
                                                                 [LPORT] is a PCIe MAC that either has multiple physical functions or
                                                                 has SR-IOV enabled. */
        uint64_t reserved_98_99        : 2;
        uint64_t pt                    : 2;  /**< [101:100] Pointer type. Enumerated by DPI_HDR_PT_E. Indicates how [PTR] is used
                                                                 upon completion of the DPI DMA instruction: byte write, SSO
                                                                 work add, or counter add.

                                                                 If no completion indication is desired for the DPI DMA instruction,
                                                                 software should set [PT]=DPI_HDR_PT_E::ZBW_CA and [PTR]=0. */
        uint64_t reserved_102_103      : 2;
        uint64_t fl                    : 1;  /**< [104:104] Determines whether DPI frees a DPI_DMA_LOCAL_PTR_S[PTR] during
                                                                 DPI_HDR_XTYPE_E::OUTBOUND instruction processing
                                                                 (along with [II] and DPI_DMA_LOCAL_PTR_S[I]).

                                                                 During DPI_HDR_XTYPE_E::OUTBOUND instruction processing, DPI frees a
                                                                 DPI_DMA_LOCAL_PTR_S[PTR] to [AURA] in FPA when:

                                                                 _ [FL] XOR (![II] AND DPI_DMA_LOCAL_PTR_S[I])

                                                                 For DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::INTERNAL_ONLY, or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions, [FL] must be clear,
                                                                 and DPI never frees local buffers. */
        uint64_t ii                    : 1;  /**< [105:105] Ignore I. Determines if DPI_DMA_LOCAL_PTR_S[I]'s influence whether
                                                                 DPI frees a DPI_DMA_LOCAL_PTR_S[PTR] during DPI_HDR_XTYPE_E::OUTBOUND
                                                                 instruction processing.

                                                                 If [II] is set, [FL] solely determines whether DPI frees, and
                                                                 all DPI_DMA_LOCAL_PTR_S[PTR]'s in the DPI_HDR_XTYPE_E::OUTBOUND
                                                                 instruction are either freed or not.

                                                                 If [II] is clear, ([FL] XOR DPI_DMA_LOCAL_PTR_S[I]) determines
                                                                 whether DPI frees a given DPI_DMA_LOCAL_PTR_S[PTR] in the
                                                                 DPI_HDR_XTYPE_E::OUTBOUND instruction, and each .

                                                                 For DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::INTERNAL_ONLY, or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions, [II] must be clear,
                                                                 and DPI never frees local buffers. */
        uint64_t fi                    : 1;  /**< [106:106] Force interrupt to a remote host.

                                                                 When [FI] is set for a (DPI_HDR_XTYPE_E::OUTBOUND or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL_ONLY) DPI DMA instruction, DPI sets an
                                                                 interrupt bit after completing instruction. This may
                                                                 cause an interrupt to be sent to a remote host.

                                                                 _ If [CSEL] = 0, DPI sets SLI()_EPF()_DMA_RINT[DMAFI\<0\>].
                                                                 _ If [CSEL] = 1, DPI sets SLI()_EPF()_DMA_RINT[DMAFI\<1\>].

                                                                 [LPORT] and [DEALLOCV] determine the EPF used. ([DEALLOCV] is
                                                                 only relevant when [PVFE]=1. Only the DPI_DMA_FUNC_SEL_S[PF]
                                                                 of [DEALLOCV] is relevant here.)

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 instructions, [II] must be clear, and DPI never sets DMAFI interrupt bits. */
        uint64_t ca                    : 1;  /**< [107:107] Add to a counter that can interrupt a remote host.

                                                                 When [CA] = 1, DPI updates a selected counter after it completes the DMA
                                                                 DPI_HDR_XTYPE_E::OUTBOUND or DPI_HDR_XTYPE_E::EXTERNAL_ONLY instruction.

                                                                 _ If [CSEL] = 0, DPI updates SLI(0)_EPF()_DMA_CNT(0)[CNT].
                                                                 _ If [CSEL] = 1, DPI updates SLI(0)_EPF()_DMA_CNT(1)[CNT].

                                                                 [LPORT] and [DEALLOCV] determine the EPF used. ([DEALLOCV] is
                                                                 only relevant when [PVFE]=1. Only the DPI_DMA_FUNC_SEL_S[PF]
                                                                 of [DEALLOCV] is relevant here.)

                                                                 Note that these updates may indirectly cause corresponding
                                                                 SLI()_EPF()_DMA_RINT[DCNT,DTIME] to become set (depending on the
                                                                 SLI()_EPF()_DMA_INT_LEVEL() settings), so may cause interrupts to
                                                                 be sent to a remote MAC host.

                                                                 If DPI()_DMA_CONTROL[O_ADD1] = 1, DPI updates the counter by 1.

                                                                 If DPI()_DMA_CONTROL[O_ADD1] = 0, DPI updates the counter by the total bytes in
                                                                 the transfer.

                                                                 When [CA] = 0, DPI does not update any SLI()_EPF()_DMA_CNT()[CNT].

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL_ONLY DPI DMA
                                                                 instructions, [CA] must never be set, and DPI never adds to any
                                                                 SLI()_EPF()_DMA_CNT[CNT]. */
        uint64_t csel                  : 1;  /**< [108:108] Counter and interrupt select. See [CA] and [FI]. [CSEL] selects which of two counters
                                                                 (SLI()_EPF()_DMA_CNT()[CNT]) and/or two interrupt bits (SLI()_EPF()_DMA_RINT[DMAFI])
                                                                 DPI can modify during DPI_HDR_XTYPE_E::OUTBOUND or DPI_HDR_XTYPE_E::EXTERNAL_ONLY
                                                                 instruction execution.

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL_ONLY DPI DMA
                                                                 instructions, [CSEL] must be zero. */
        uint64_t reserved_109_111      : 3;
        uint64_t xtype                 : 2;  /**< [113:112] Transfer type of the instruction. Enumerated by DPI_HDR_XTYPE_E. Each DPI DMA
                                                                 instruction can be DPI_HDR_XTYPE_E::OUTBOUND (L2/DRAM-\>MAC),
                                                                 DPI_HDR_XTYPE_E::INBOUND (MAC-\>L2/DRAM), DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 (L2/DRAM-\>L2/DRAM), or DPI_HDR_XTYPE_E::EXTERNAL_ONLY (MAC-\>MAC). */
        uint64_t reserved_114_115      : 2;
        uint64_t reserved_116_119      : 4;
        uint64_t fport                 : 2;  /**< [121:120] Port for the first pointers block.

                                                                 DPI sends MAC memory space reads for the MAC addresses in the first
                                                                 pointers block to the MAC selected by [FPORT] while processing
                                                                 a DPI_HDR_XTYPE_E::EXTERNAL_ONLY DPI DMA instruction.

                                                                 [FPORT] must be zero for DPI_HDR_XTYPE_E::OUTBOUND,
                                                                 DPI_HDR_XTYPE_E::INBOUND, and DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 instructions. */
        uint64_t reserved_122_123      : 2;
        uint64_t lport                 : 2;  /**< [125:124] Port for the last pointers block.

                                                                 DPI sends MAC memory space reads and writes for the MAC addresses
                                                                 in the last pointers block to the MAC selected by [LPORT]
                                                                 while processing DPI_HDR_XTYPE_E::OUTBOUND, DPI_HDR_XTYPE_E::INBOUND,
                                                                 and DPI_HDR_XTYPE_E::EXTERNAL_ONLY instructions.

                                                                 [LPORT] must be zero for DPI_HDR_XTYPE_E::INTERNAL_ONLY
                                                                 instructions. */
        uint64_t reserved_126_127      : 2;
#endif /* Word 1 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 2 - Big Endian */
        uint64_t ptr                   : 64; /**< [191:128] Completion pointer. Usage determined by [PT] value. The DPI_HDR_PT_E
                                                                 enumeration describes the supported [PT] values and the [PTR] usage
                                                                 and requirements in each case.
                                                                 Bits \<63:49\> are ignored by hardware; software should use a sign-extended bit
                                                                 \<48\> for forward compatibility. */
#else /* Word 2 - Little Endian */
        uint64_t ptr                   : 64; /**< [191:128] Completion pointer. Usage determined by [PT] value. The DPI_HDR_PT_E
                                                                 enumeration describes the supported [PT] values and the [PTR] usage
                                                                 and requirements in each case.
                                                                 Bits \<63:49\> are ignored by hardware; software should use a sign-extended bit
                                                                 \<48\> for forward compatibility. */
#endif /* Word 2 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 3 - Big Endian */
        uint64_t reserved_192_255      : 64;
#else /* Word 3 - Little Endian */
        uint64_t reserved_192_255      : 64;
#endif /* Word 3 - End */
    } cn8;
    struct cavm_dpi_dma_instr_hdr_s_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_60_63        : 4;
        uint64_t reserved_58_59        : 2;
        uint64_t nlst                  : 4;  /**< [ 57: 54] The number of pointers in the last pointers block. Valid values are 1 through 15.

                                                                 The last pointers block includes [NFST] DPI_DMA_PTR_S local pointers and is [NFST] *
                                                                 2 64-bit words.

                                                                 Note that the sum of the number of 64-bit words in the last pointers block
                                                                 and first pointers block must never exceed 60. */
        uint64_t reserved_52_53        : 2;
        uint64_t nfst                  : 4;  /**< [ 51: 48] The number of pointers in the first pointers block. Valid values are 1 through 15.

                                                                 The first pointers block includes [NFST] DPI_DMA_PTR_S local pointers and is [NFST]
                                                                 * 2 64-bit words.

                                                                 Note that the sum of the number of 64-bit words in the last pointers block
                                                                 and first pointers block must never exceed 60. */
        uint64_t reserved_46_47        : 2;
        uint64_t reserved_44_45        : 2;
        uint64_t grp                   : 10; /**< [ 43: 34] SSO guest-group. Sent to SSO upon instruction completion when [PT] = DPI_HDR_PT_E::WQP.
                                                                 For the SSO to not discard the add-work request.

                                                                 [GRP] must be zero when [PT] != DPI_HDR_PT_E::WQP. */
        uint64_t tt                    : 2;  /**< [ 33: 32] SSO tag type. Sent to SSO upon instruction completion when [PT] = DPI_HDR_PT_E::WQP.

                                                                 [TT] must be zero when [PT] != DPI_HDR_PT_E::WQP. */
        uint64_t tag                   : 32; /**< [ 31:  0] SSO Tag */
#else /* Word 0 - Little Endian */
        uint64_t tag                   : 32; /**< [ 31:  0] SSO Tag */
        uint64_t tt                    : 2;  /**< [ 33: 32] SSO tag type. Sent to SSO upon instruction completion when [PT] = DPI_HDR_PT_E::WQP.

                                                                 [TT] must be zero when [PT] != DPI_HDR_PT_E::WQP. */
        uint64_t grp                   : 10; /**< [ 43: 34] SSO guest-group. Sent to SSO upon instruction completion when [PT] = DPI_HDR_PT_E::WQP.
                                                                 For the SSO to not discard the add-work request.

                                                                 [GRP] must be zero when [PT] != DPI_HDR_PT_E::WQP. */
        uint64_t reserved_44_45        : 2;
        uint64_t reserved_46_47        : 2;
        uint64_t nfst                  : 4;  /**< [ 51: 48] The number of pointers in the first pointers block. Valid values are 1 through 15.

                                                                 The first pointers block includes [NFST] DPI_DMA_PTR_S local pointers and is [NFST]
                                                                 * 2 64-bit words.

                                                                 Note that the sum of the number of 64-bit words in the last pointers block
                                                                 and first pointers block must never exceed 60. */
        uint64_t reserved_52_53        : 2;
        uint64_t nlst                  : 4;  /**< [ 57: 54] The number of pointers in the last pointers block. Valid values are 1 through 15.

                                                                 The last pointers block includes [NFST] DPI_DMA_PTR_S local pointers and is [NFST] *
                                                                 2 64-bit words.

                                                                 Note that the sum of the number of 64-bit words in the last pointers block
                                                                 and first pointers block must never exceed 60. */
        uint64_t reserved_58_59        : 2;
        uint64_t reserved_60_63        : 4;
#endif /* Word 0 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 1 - Big Endian */
        uint64_t reserved_126_127      : 2;
        uint64_t lport                 : 2;  /**< [125:124] Port for the last pointers block.

                                                                 DPI sends MAC memory space reads and writes for the MAC addresses
                                                                 in the last pointers block to the MAC selected by [LPORT]
                                                                 while processing DPI_HDR_XTYPE_E::OUTBOUND, DPI_HDR_XTYPE_E::INBOUND,
                                                                 and DPI_HDR_XTYPE_E::EXTERNAL instructions.

                                                                 [LPORT]\<0\> normally determines the NCB DPI uses for DPI_HDR_XTYPE_E::INTERNAL
                                                                 instructions.

                                                                 [LPORT]\<1\> must be zero for DPI_HDR_XTYPE_E::INTERNAL
                                                                 instructions. */
        uint64_t reserved_122_123      : 2;
        uint64_t fport                 : 2;  /**< [121:120] Port for the first pointers block.

                                                                 DPI sends MAC memory space reads for the MAC addresses in the first
                                                                 pointers block to the MAC selected by [FPORT] while processing
                                                                 a DPI_HDR_XTYPE_E::EXTERNAL DPI DMA instruction.

                                                                 [FPORT]\<0\> normally determines the NCB DPI uses for
                                                                 DPI_HDR_XTYPE_E::OUTBOUND and DPI_HDR_XTYPE_E::INBOUND instructions.

                                                                 [FPORT]\<0\> normally determines the NCB DPI uses for DPI_HDR_XTYPE_E::INTERNAL
                                                                 instructions.

                                                                 [FPORT]\<1\> must be zero for DPI_HDR_XTYPE_E::OUTBOUND,
                                                                 DPI_HDR_XTYPE_E::INBOUND, and DPI_HDR_XTYPE_E::INTERNAL
                                                                 instructions. */
        uint64_t reserved_116_119      : 4;
        uint64_t reserved_114_115      : 2;
        uint64_t xtype                 : 2;  /**< [113:112] Transfer type of the instruction. Enumerated by DPI_HDR_XTYPE_E. Each DPI DMA
                                                                 instruction can be DPI_HDR_XTYPE_E::OUTBOUND (LLC/DRAM-\>MAC),
                                                                 DPI_HDR_XTYPE_E::INBOUND (MAC-\>LLC/DRAM), DPI_HDR_XTYPE_E::INTERNAL
                                                                 (LLC/DRAM-\>LLC/DRAM), or DPI_HDR_XTYPE_E::EXTERNAL (MAC-\>MAC). */
        uint64_t reserved_109_111      : 3;
        uint64_t csel                  : 1;  /**< [108:108] Counter and interrupt select. See [CA] and [FI]. [CSEL] selects which of two counters
                                                                 (SDP()_EPF()_DMA_CNT()[CNT]) and/or two interrupt bits (SDP()_EPF()_DMA_RINT[DMAFI])
                                                                 DPI can modify during DPI_HDR_XTYPE_E::OUTBOUND or DPI_HDR_XTYPE_E::EXTERNAL
                                                                 instruction execution.

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL DPI DMA
                                                                 instructions, [CSEL] must be zero. */
        uint64_t ca                    : 1;  /**< [107:107] Add to a counter that can interrupt a remote host.

                                                                 When [CA] = 1, DPI updates a selected counter after it completes the DMA
                                                                 DPI_HDR_XTYPE_E::OUTBOUND or DPI_HDR_XTYPE_E::EXTERNAL instruction.

                                                                 _ If [CSEL] = 0, DPI updates SDP(0)_EPF(0..15)_DMA_CNT(0)[CNT].
                                                                 _ If [CSEL] = 1, DPI updates SDP(0)_EPF(0..15)_DMA_CNT(1)[CNT].

                                                                 Note that these updates may indirectly cause
                                                                 SDP()_EPF()_DMA_RINT[DCNT,DTIME] to become set for all MACs
                                                                 (depending on the SDP()_EPF()_DMA_INT_LEVEL() settings), so may cause interrupts to
                                                                 be sent to a remote MAC host.

                                                                 If DPI()_DMA_CONTROL[O_ADD1] = 1, DPI updates the counter by 1.

                                                                 If DPI()_DMA_CONTROL[O_ADD1] = 0, DPI updates the counter by the total bytes in
                                                                 the transfer.

                                                                 When [CA] = 0, DPI does not update any SDP()_EPF()_DMA_CNT()[CNT].

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL DPI DMA
                                                                 instructions, [CA] must never be set, and DPI never adds to any
                                                                 SDP()_EPF()_DMA_CNT()[CNT]. */
        uint64_t fi                    : 1;  /**< [106:106] Force interrupt to a remote host.

                                                                 When [FI] is set for a (DPI_HDR_XTYPE_E::OUTBOUND or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL) DPI DMA instruction, DPI sets an
                                                                 interrupt bit after completing instruction. If [CSEL] = 0, DPI sets
                                                                 SDP()_EPF()_DMA_RINT[DMAFI]\<0\> for all MACs, else DPI sets
                                                                 SDP()_EPF()_DMA_RINT[DMAFI]\<1\> for all MACs. This may
                                                                 cause an interrupt to be sent to a remote host.

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL
                                                                 instructions, [II] must be clear, and DPI never sets DMAFI interrupt bits. */
        uint64_t ii                    : 1;  /**< [105:105] Ignore I. Determines if DPI_DMA_PTR_S[I]'s influence whether
                                                                 DPI frees a DPI_DMA_PTR_S[PTR] during DPI_HDR_XTYPE_E::OUTBOUND
                                                                 instruction processing.

                                                                 If [II] is set, [FL] solely determines whether DPI frees, and
                                                                 all DPI_DMA_PTR_S[PTR]'s in the DPI_HDR_XTYPE_E::OUTBOUND
                                                                 instruction are either freed or not.

                                                                 If [II] is clear, ([FL] XOR DPI_DMA_PTR_S[I]) determines
                                                                 whether DPI frees a given DPI_DMA_PTR_S[PTR] in the
                                                                 DPI_HDR_XTYPE_E::OUTBOUND instruction, and each .

                                                                 For DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::INTERNAL, or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL instructions, [II] must be clear,
                                                                 and DPI never frees local buffers. */
        uint64_t fl                    : 1;  /**< [104:104] Determines whether DPI frees a DPI_DMA_PTR_S[PTR] during
                                                                 DPI_HDR_XTYPE_E::OUTBOUND instruction processing
                                                                 (along with [II] and DPI_DMA_PTR_S[I]).

                                                                 During DPI_HDR_XTYPE_E::OUTBOUND instruction processing, DPI frees a
                                                                 DPI_DMA_PTR_S[PTR] to [AURA] in NPA when:

                                                                 _ [FL] XOR (![II] AND DPI_DMA_PTR_S[I])

                                                                 For DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::INTERNAL, or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL instructions, [FL] must be clear,
                                                                 and DPI never frees local buffers. */
        uint64_t pvfe                  : 1;  /**< [103:103] Function enable. When [PVFE] is set, DPI directs all MAC reads/writes
                                                                 to the function (physical or virtual) that [FUNC] selects within
                                                                 MAC/port [LPORT]. When [PVFE] is clear, DPI directs all MAC reads/writes
                                                                 to physical function 0 within the MACs/ports [LPORT] or [FPORT]. [PVFE] must not be set when
                                                                 [XTYPE] is DPI_HDR_XTYPE_E::INTERNAL or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL. [PVFE] can only be set when MAC/PORT
                                                                 [LPORT] is a PCIe MAC that either has multiple physical functions or
                                                                 has SR-IOV enabled. */
        uint64_t reserved_102          : 1;
        uint64_t pt                    : 2;  /**< [101:100] Pointer type. Enumerated by DPI_HDR_PT_E. Indicates how [PTR] is used
                                                                 upon completion of the DPI DMA instruction: byte write, SSO
                                                                 work add, or counter add.

                                                                 If no completion indication is desired for the DPI DMA instruction,
                                                                 software should set [PT]=DPI_HDR_PT_E::ZBW_CA and [PTR]=0. */
        uint64_t func                  : 16; /**< [ 99: 84] [FUNC] selects the function within the MAC selected by [LPORT]
                                                                 when [PVFE] is set. Defined by DPI_DMA_FUNC_SEL_S. */
        uint64_t aura                  : 20; /**< [ 83: 64] NPA guest-aura.  The NPA guest-aura DPI uses for all NPA transactions for the
                                                                 DPI DMA instruction. [AURA] can only be used when
                                                                 [XTYPE]=DPI_HDR_XTYPE_E::OUTBOUND, and must be zero otherwise.

                                                                 For the NPA to not discard the request, NPA must map [AURA] to be
                                                                 valid inside the PF/function DPI()_DMA()_IDS[SSO_PF_FUNC].

                                                                 During an DPI_HDR_XTYPE_E::OUTBOUND DPI DMA instruction, [FL], [II],
                                                                 and DPI_DMA_PTR_S[I] determine whether DPI frees a
                                                                 DPI_DMA_PTR_S[PTR] to [AURA] in NPA. */
#else /* Word 1 - Little Endian */
        uint64_t aura                  : 20; /**< [ 83: 64] NPA guest-aura.  The NPA guest-aura DPI uses for all NPA transactions for the
                                                                 DPI DMA instruction. [AURA] can only be used when
                                                                 [XTYPE]=DPI_HDR_XTYPE_E::OUTBOUND, and must be zero otherwise.

                                                                 For the NPA to not discard the request, NPA must map [AURA] to be
                                                                 valid inside the PF/function DPI()_DMA()_IDS[SSO_PF_FUNC].

                                                                 During an DPI_HDR_XTYPE_E::OUTBOUND DPI DMA instruction, [FL], [II],
                                                                 and DPI_DMA_PTR_S[I] determine whether DPI frees a
                                                                 DPI_DMA_PTR_S[PTR] to [AURA] in NPA. */
        uint64_t func                  : 16; /**< [ 99: 84] [FUNC] selects the function within the MAC selected by [LPORT]
                                                                 when [PVFE] is set. Defined by DPI_DMA_FUNC_SEL_S. */
        uint64_t pt                    : 2;  /**< [101:100] Pointer type. Enumerated by DPI_HDR_PT_E. Indicates how [PTR] is used
                                                                 upon completion of the DPI DMA instruction: byte write, SSO
                                                                 work add, or counter add.

                                                                 If no completion indication is desired for the DPI DMA instruction,
                                                                 software should set [PT]=DPI_HDR_PT_E::ZBW_CA and [PTR]=0. */
        uint64_t reserved_102          : 1;
        uint64_t pvfe                  : 1;  /**< [103:103] Function enable. When [PVFE] is set, DPI directs all MAC reads/writes
                                                                 to the function (physical or virtual) that [FUNC] selects within
                                                                 MAC/port [LPORT]. When [PVFE] is clear, DPI directs all MAC reads/writes
                                                                 to physical function 0 within the MACs/ports [LPORT] or [FPORT]. [PVFE] must not be set when
                                                                 [XTYPE] is DPI_HDR_XTYPE_E::INTERNAL or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL. [PVFE] can only be set when MAC/PORT
                                                                 [LPORT] is a PCIe MAC that either has multiple physical functions or
                                                                 has SR-IOV enabled. */
        uint64_t fl                    : 1;  /**< [104:104] Determines whether DPI frees a DPI_DMA_PTR_S[PTR] during
                                                                 DPI_HDR_XTYPE_E::OUTBOUND instruction processing
                                                                 (along with [II] and DPI_DMA_PTR_S[I]).

                                                                 During DPI_HDR_XTYPE_E::OUTBOUND instruction processing, DPI frees a
                                                                 DPI_DMA_PTR_S[PTR] to [AURA] in NPA when:

                                                                 _ [FL] XOR (![II] AND DPI_DMA_PTR_S[I])

                                                                 For DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::INTERNAL, or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL instructions, [FL] must be clear,
                                                                 and DPI never frees local buffers. */
        uint64_t ii                    : 1;  /**< [105:105] Ignore I. Determines if DPI_DMA_PTR_S[I]'s influence whether
                                                                 DPI frees a DPI_DMA_PTR_S[PTR] during DPI_HDR_XTYPE_E::OUTBOUND
                                                                 instruction processing.

                                                                 If [II] is set, [FL] solely determines whether DPI frees, and
                                                                 all DPI_DMA_PTR_S[PTR]'s in the DPI_HDR_XTYPE_E::OUTBOUND
                                                                 instruction are either freed or not.

                                                                 If [II] is clear, ([FL] XOR DPI_DMA_PTR_S[I]) determines
                                                                 whether DPI frees a given DPI_DMA_PTR_S[PTR] in the
                                                                 DPI_HDR_XTYPE_E::OUTBOUND instruction, and each .

                                                                 For DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::INTERNAL, or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL instructions, [II] must be clear,
                                                                 and DPI never frees local buffers. */
        uint64_t fi                    : 1;  /**< [106:106] Force interrupt to a remote host.

                                                                 When [FI] is set for a (DPI_HDR_XTYPE_E::OUTBOUND or
                                                                 DPI_HDR_XTYPE_E::EXTERNAL) DPI DMA instruction, DPI sets an
                                                                 interrupt bit after completing instruction. If [CSEL] = 0, DPI sets
                                                                 SDP()_EPF()_DMA_RINT[DMAFI]\<0\> for all MACs, else DPI sets
                                                                 SDP()_EPF()_DMA_RINT[DMAFI]\<1\> for all MACs. This may
                                                                 cause an interrupt to be sent to a remote host.

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL
                                                                 instructions, [II] must be clear, and DPI never sets DMAFI interrupt bits. */
        uint64_t ca                    : 1;  /**< [107:107] Add to a counter that can interrupt a remote host.

                                                                 When [CA] = 1, DPI updates a selected counter after it completes the DMA
                                                                 DPI_HDR_XTYPE_E::OUTBOUND or DPI_HDR_XTYPE_E::EXTERNAL instruction.

                                                                 _ If [CSEL] = 0, DPI updates SDP(0)_EPF(0..15)_DMA_CNT(0)[CNT].
                                                                 _ If [CSEL] = 1, DPI updates SDP(0)_EPF(0..15)_DMA_CNT(1)[CNT].

                                                                 Note that these updates may indirectly cause
                                                                 SDP()_EPF()_DMA_RINT[DCNT,DTIME] to become set for all MACs
                                                                 (depending on the SDP()_EPF()_DMA_INT_LEVEL() settings), so may cause interrupts to
                                                                 be sent to a remote MAC host.

                                                                 If DPI()_DMA_CONTROL[O_ADD1] = 1, DPI updates the counter by 1.

                                                                 If DPI()_DMA_CONTROL[O_ADD1] = 0, DPI updates the counter by the total bytes in
                                                                 the transfer.

                                                                 When [CA] = 0, DPI does not update any SDP()_EPF()_DMA_CNT()[CNT].

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL DPI DMA
                                                                 instructions, [CA] must never be set, and DPI never adds to any
                                                                 SDP()_EPF()_DMA_CNT()[CNT]. */
        uint64_t csel                  : 1;  /**< [108:108] Counter and interrupt select. See [CA] and [FI]. [CSEL] selects which of two counters
                                                                 (SDP()_EPF()_DMA_CNT()[CNT]) and/or two interrupt bits (SDP()_EPF()_DMA_RINT[DMAFI])
                                                                 DPI can modify during DPI_HDR_XTYPE_E::OUTBOUND or DPI_HDR_XTYPE_E::EXTERNAL
                                                                 instruction execution.

                                                                 For DPI_HDR_XTYPE_E::INBOUND or DPI_HDR_XTYPE_E::INTERNAL DPI DMA
                                                                 instructions, [CSEL] must be zero. */
        uint64_t reserved_109_111      : 3;
        uint64_t xtype                 : 2;  /**< [113:112] Transfer type of the instruction. Enumerated by DPI_HDR_XTYPE_E. Each DPI DMA
                                                                 instruction can be DPI_HDR_XTYPE_E::OUTBOUND (LLC/DRAM-\>MAC),
                                                                 DPI_HDR_XTYPE_E::INBOUND (MAC-\>LLC/DRAM), DPI_HDR_XTYPE_E::INTERNAL
                                                                 (LLC/DRAM-\>LLC/DRAM), or DPI_HDR_XTYPE_E::EXTERNAL (MAC-\>MAC). */
        uint64_t reserved_114_115      : 2;
        uint64_t reserved_116_119      : 4;
        uint64_t fport                 : 2;  /**< [121:120] Port for the first pointers block.

                                                                 DPI sends MAC memory space reads for the MAC addresses in the first
                                                                 pointers block to the MAC selected by [FPORT] while processing
                                                                 a DPI_HDR_XTYPE_E::EXTERNAL DPI DMA instruction.

                                                                 [FPORT]\<0\> normally determines the NCB DPI uses for
                                                                 DPI_HDR_XTYPE_E::OUTBOUND and DPI_HDR_XTYPE_E::INBOUND instructions.

                                                                 [FPORT]\<0\> normally determines the NCB DPI uses for DPI_HDR_XTYPE_E::INTERNAL
                                                                 instructions.

                                                                 [FPORT]\<1\> must be zero for DPI_HDR_XTYPE_E::OUTBOUND,
                                                                 DPI_HDR_XTYPE_E::INBOUND, and DPI_HDR_XTYPE_E::INTERNAL
                                                                 instructions. */
        uint64_t reserved_122_123      : 2;
        uint64_t lport                 : 2;  /**< [125:124] Port for the last pointers block.

                                                                 DPI sends MAC memory space reads and writes for the MAC addresses
                                                                 in the last pointers block to the MAC selected by [LPORT]
                                                                 while processing DPI_HDR_XTYPE_E::OUTBOUND, DPI_HDR_XTYPE_E::INBOUND,
                                                                 and DPI_HDR_XTYPE_E::EXTERNAL instructions.

                                                                 [LPORT]\<0\> normally determines the NCB DPI uses for DPI_HDR_XTYPE_E::INTERNAL
                                                                 instructions.

                                                                 [LPORT]\<1\> must be zero for DPI_HDR_XTYPE_E::INTERNAL
                                                                 instructions. */
        uint64_t reserved_126_127      : 2;
#endif /* Word 1 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 2 - Big Endian */
        uint64_t ptr                   : 64; /**< [191:128] Completion pointer. Usage determined by [PT] value. The DPI_HDR_PT_E
                                                                 enumeration describes the supported [PT] values and the [PTR] usage
                                                                 and requirements in each case.
                                                                 Bits \<63:53\> are ignored by hardware; software should use a sign-extended bit
                                                                 \<52\> for forward compatibility. */
#else /* Word 2 - Little Endian */
        uint64_t ptr                   : 64; /**< [191:128] Completion pointer. Usage determined by [PT] value. The DPI_HDR_PT_E
                                                                 enumeration describes the supported [PT] values and the [PTR] usage
                                                                 and requirements in each case.
                                                                 Bits \<63:53\> are ignored by hardware; software should use a sign-extended bit
                                                                 \<52\> for forward compatibility. */
#endif /* Word 2 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 3 - Big Endian */
        uint64_t reserved_192_255      : 64;
#else /* Word 3 - Little Endian */
        uint64_t reserved_192_255      : 64;
#endif /* Word 3 - End */
    } cn9;
};

/**
 * Structure dpi_dma_local_ptr_s
 *
 * DPI DMA Local Pointer Structure
 * DPI DMA L2/DRAM and MAC Address Pointer Format
 */
union cavm_dpi_dma_local_ptr_s
{
    uint64_t u[2];
    struct cavm_dpi_dma_local_ptr_s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t i                     : 1;  /**< [ 63: 63] Invert free. When set for a DPI_DMA_LOCAL_PTR_S that contains an L2/DRAM pointer,
                                                                 can invert the decision to free the pointer to FPA.

                                                                 [I] can only be set in a DPI_DMA_LOCAL_PTR_S in the first pointers area
                                                                 of a DPI_HDR_XTYPE_E::OUTBOUND instruction.

                                                                 [I] must be zero in, and DPI never frees, any DPI_DMA_LOCAL_PTR_S containing
                                                                 a MAC address, any DPI_DMA_LOCAL_PTR_S that contains an L2/DRAM address to be
                                                                 written, and in any DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::EXTERNAL_ONLY,
                                                                 or DPI_HDR_XTYPE_E::INTERNAL_ONLY DPI DMA instruction.

                                                                 [I] gives software the ability to free buffers independently for a
                                                                 DPI_HDR_XTYPE_E::OUTBOUND DMA transfer when DPI_DMA_INSTR_HDR_S[II] is clear.
                                                                 [I] is not used by DPI when DPI_DMA_INSTR_HDR_S[II] is set. See
                                                                 DPI_DMA_INSTR_HDR_S[II] and DPI_DMA_INSTR_HDR_S[FL]. */
        uint64_t f                     : 1;  /**< [ 62: 62] Full-block write operations are allowed. When set for a DPI_DMA_LOCAL_PTR_S
                                                                 that contains an L2/DRAM pointer to be written, the hardware is permitted to
                                                                 write all the bytes in any cache block that contains any byte in the range
                                                                 [PTR] .. [PTR] + [LENGTH] - 1. Setting [F] can improve memory system performance
                                                                 when a write misses in the L2 cache.

                                                                 [F] must be zero in any DPI_DMA_LOCAL_PTR_S containing a MAC address.
                                                                 [F] must be zero in any DPI_DMA_LOCAL_PTR_S containing a L2/DRAM
                                                                 address to be read.

                                                                 [F] can only be set for L2/DRAM DPI_DMA_LOCAL_PTR_S's that can be written to:

                                                                 * The DPI_DMA_LOCAL_PTR_S's in the first-pointers area that are write pointers
                                                                   for DPI_HDR_XTYPE_E::INBOUND DPI DMA instructions.

                                                                 * The DPI_DMA_LOCAL_PTR_S's in the last-pointers area that are
                                                                   write pointers DPI_HDR_XTYPE_E::INTERNAL_ONLY DPI DMA instructions. */
        uint64_t ac                    : 1;  /**< [ 61: 61] Allocate L2.  When set for a DPI_DMA_LOCAL_PTR_S that contains an L2/DRAM pointer,
                                                                 [AC] is a hint to DPI that the cache blocks should be allocated in
                                                                 the L2 cache (if they were not already). Should typically be set for L2/DRAM
                                                                 pointers to allocate the referenced cache blocks into the L2 cache.

                                                                 [AC] must be zero in any DPI_DMA_LOCAL_PTR_S containing a MAC address and
                                                                 does not control any cache allocation.

                                                                 When the L2/DRAM pointer is read (e.g. a DPI_HDR_XTYPE_E::OUTBOUND
                                                                 DPI DMA instruction), the referenced cache blocks are not allocated into the L2
                                                                 cache as part of completing the DMA (when not already present in the L2) if [AC]
                                                                 is clear.

                                                                 When the L2/DRAM pointer is written (e.g. a DPI_HDR_XTYPE_E::INBOUND DPI DMA
                                                                 instruction), the referenced cache blocks are not allocated into the cache as
                                                                 part of completing the DMA (when not already present in the L2) if [AC] is
                                                                 clear, and either:

                                                                 * [PTR] .. [PTR] + [LENGTH] - 1 includes the entire cache block, or

                                                                 * [F] is set so that DPI can write the entire cache block. */
        uint64_t bed                   : 1;  /**< [ 60: 60] Big-endian data. When set for a DPI_DMA_LOCAL_PTR_S that contains an L2/DRAM pointer
                                                                 for reading, DPI performs a DPI_ENDIANSWAP_E::BYTE_SWAP_64B swap on any
                                                                 naturally-aligned 64-bit L2/DRAM words that contain one or more bytes before
                                                                 extracting the bytes to be used. When set for a DPI_DMA_LOCAL_PTR_S that contains
                                                                 an L2/DRAM pointer for writing, before writing the L2/DRAM word, DPI performs a
                                                                 DPI_ENDIANSWAP_E::BYTE_SWAP_64B on the bytes to be written within any naturally-aligned
                                                                 64-bit L2/DRAM word, including the associated byte mask.

                                                                 [BED] must be zero and does not control endian swapping in any DPI_DMA_LOCAL_PTR_S
                                                                 that contains a MAC address. (CSR and MAC address bits select the DPI_ENDIANSWAP_E
                                                                 used with MAC addresses.) */
        uint64_t reserved_16_59        : 44;
        uint64_t length                : 16; /**< [ 15:  0] Number of bytes of the contiguous space specified by PTR. Must not be zero.

                                                                 [LENGTH\<15:12\>] must be zero in any DPI_DMA_LOCAL_PTR_S containing an L2/DRAM pointer.

                                                                 Note that the sum of the LENGTHs in the first-pointers area must exactly
                                                                 equal the sum of the LENGTHs in the last-pointers area. */
#else /* Word 0 - Little Endian */
        uint64_t length                : 16; /**< [ 15:  0] Number of bytes of the contiguous space specified by PTR. Must not be zero.

                                                                 [LENGTH\<15:12\>] must be zero in any DPI_DMA_LOCAL_PTR_S containing an L2/DRAM pointer.

                                                                 Note that the sum of the LENGTHs in the first-pointers area must exactly
                                                                 equal the sum of the LENGTHs in the last-pointers area. */
        uint64_t reserved_16_59        : 44;
        uint64_t bed                   : 1;  /**< [ 60: 60] Big-endian data. When set for a DPI_DMA_LOCAL_PTR_S that contains an L2/DRAM pointer
                                                                 for reading, DPI performs a DPI_ENDIANSWAP_E::BYTE_SWAP_64B swap on any
                                                                 naturally-aligned 64-bit L2/DRAM words that contain one or more bytes before
                                                                 extracting the bytes to be used. When set for a DPI_DMA_LOCAL_PTR_S that contains
                                                                 an L2/DRAM pointer for writing, before writing the L2/DRAM word, DPI performs a
                                                                 DPI_ENDIANSWAP_E::BYTE_SWAP_64B on the bytes to be written within any naturally-aligned
                                                                 64-bit L2/DRAM word, including the associated byte mask.

                                                                 [BED] must be zero and does not control endian swapping in any DPI_DMA_LOCAL_PTR_S
                                                                 that contains a MAC address. (CSR and MAC address bits select the DPI_ENDIANSWAP_E
                                                                 used with MAC addresses.) */
        uint64_t ac                    : 1;  /**< [ 61: 61] Allocate L2.  When set for a DPI_DMA_LOCAL_PTR_S that contains an L2/DRAM pointer,
                                                                 [AC] is a hint to DPI that the cache blocks should be allocated in
                                                                 the L2 cache (if they were not already). Should typically be set for L2/DRAM
                                                                 pointers to allocate the referenced cache blocks into the L2 cache.

                                                                 [AC] must be zero in any DPI_DMA_LOCAL_PTR_S containing a MAC address and
                                                                 does not control any cache allocation.

                                                                 When the L2/DRAM pointer is read (e.g. a DPI_HDR_XTYPE_E::OUTBOUND
                                                                 DPI DMA instruction), the referenced cache blocks are not allocated into the L2
                                                                 cache as part of completing the DMA (when not already present in the L2) if [AC]
                                                                 is clear.

                                                                 When the L2/DRAM pointer is written (e.g. a DPI_HDR_XTYPE_E::INBOUND DPI DMA
                                                                 instruction), the referenced cache blocks are not allocated into the cache as
                                                                 part of completing the DMA (when not already present in the L2) if [AC] is
                                                                 clear, and either:

                                                                 * [PTR] .. [PTR] + [LENGTH] - 1 includes the entire cache block, or

                                                                 * [F] is set so that DPI can write the entire cache block. */
        uint64_t f                     : 1;  /**< [ 62: 62] Full-block write operations are allowed. When set for a DPI_DMA_LOCAL_PTR_S
                                                                 that contains an L2/DRAM pointer to be written, the hardware is permitted to
                                                                 write all the bytes in any cache block that contains any byte in the range
                                                                 [PTR] .. [PTR] + [LENGTH] - 1. Setting [F] can improve memory system performance
                                                                 when a write misses in the L2 cache.

                                                                 [F] must be zero in any DPI_DMA_LOCAL_PTR_S containing a MAC address.
                                                                 [F] must be zero in any DPI_DMA_LOCAL_PTR_S containing a L2/DRAM
                                                                 address to be read.

                                                                 [F] can only be set for L2/DRAM DPI_DMA_LOCAL_PTR_S's that can be written to:

                                                                 * The DPI_DMA_LOCAL_PTR_S's in the first-pointers area that are write pointers
                                                                   for DPI_HDR_XTYPE_E::INBOUND DPI DMA instructions.

                                                                 * The DPI_DMA_LOCAL_PTR_S's in the last-pointers area that are
                                                                   write pointers DPI_HDR_XTYPE_E::INTERNAL_ONLY DPI DMA instructions. */
        uint64_t i                     : 1;  /**< [ 63: 63] Invert free. When set for a DPI_DMA_LOCAL_PTR_S that contains an L2/DRAM pointer,
                                                                 can invert the decision to free the pointer to FPA.

                                                                 [I] can only be set in a DPI_DMA_LOCAL_PTR_S in the first pointers area
                                                                 of a DPI_HDR_XTYPE_E::OUTBOUND instruction.

                                                                 [I] must be zero in, and DPI never frees, any DPI_DMA_LOCAL_PTR_S containing
                                                                 a MAC address, any DPI_DMA_LOCAL_PTR_S that contains an L2/DRAM address to be
                                                                 written, and in any DPI_HDR_XTYPE_E::INBOUND, DPI_HDR_XTYPE_E::EXTERNAL_ONLY,
                                                                 or DPI_HDR_XTYPE_E::INTERNAL_ONLY DPI DMA instruction.

                                                                 [I] gives software the ability to free buffers independently for a
                                                                 DPI_HDR_XTYPE_E::OUTBOUND DMA transfer when DPI_DMA_INSTR_HDR_S[II] is clear.
                                                                 [I] is not used by DPI when DPI_DMA_INSTR_HDR_S[II] is set. See
                                                                 DPI_DMA_INSTR_HDR_S[II] and DPI_DMA_INSTR_HDR_S[FL]. */
#endif /* Word 0 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 1 - Big Endian */
        uint64_t ptr                   : 64; /**< [127: 64] Byte pointer to L2/DRAM or MAC data source/destination. Can be any byte alignment.

                                                                 [PTR\<63:49\>] must be zero when an L2/DRAM pointer.

                                                                 All 64 bits are valid when [PTR] is a MAC address. */
#else /* Word 1 - Little Endian */
        uint64_t ptr                   : 64; /**< [127: 64] Byte pointer to L2/DRAM or MAC data source/destination. Can be any byte alignment.

                                                                 [PTR\<63:49\>] must be zero when an L2/DRAM pointer.

                                                                 All 64 bits are valid when [PTR] is a MAC address. */
#endif /* Word 1 - End */
    } s;
    /* struct cavm_dpi_dma_local_ptr_s_s cn; */
};

/**
 * Structure dpi_dma_ptr_s
 *
 * DPI DMA Local Pointer Structure
 * DPI DMA Local Pointer Format
 */
union cavm_dpi_dma_ptr_s
{
    uint64_t u[2];
    struct cavm_dpi_dma_ptr_s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t i                     : 1;  /**< [ 63: 63] Invert free. Only used with LLC/DRAM pointers.

                                                                 This bit gives the software the ability to free buffers
                                                                 independently for a DPI_HDR_XTYPE_E::OUTBOUND DMA transfer when
                                                                 DPI_DMA_INSTR_HDR_S[II] is clear. See DPI_DMA_INSTR_HDR_S[II] and
                                                                 DPI_DMA_INSTR_HDR_S[FL].

                                                                 [I] is not used by DPI when DPI_DMA_INSTR_HDR_S[II] is set. [I] must not be set,
                                                                 and DPI never frees buffers for DPI_HDR_XTYPE_E::INBOUND and
                                                                 DPI_HDR_XTYPE_E::INTERNAL DPI DMA instructions. */
        uint64_t f                     : 1;  /**< [ 62: 62] Full-block write operations are allowed.
                                                                 Only used with LLC/DRAM pointers.

                                                                 When set, the hardware is permitted to write all the bytes in the cache blocks
                                                                 covered by [PTR] .. [PTR] + [LENGTH] - 1. This can improve memory system performance
                                                                 when the write misses in the LLC.

                                                                 [F] can only be set for LLC/DRAM pointers that can be written to:

                                                                 * The DPI_DMA_PTR_S's in the first-pointers area that are write pointers
                                                                   for DPI_HDR_XTYPE_E::INBOUND DPI DMA instructions.

                                                                 * The DPI_DMA_PTR_S's in the last-pointers area that are always write
                                                                   pointers (when present for DPI_HDR_XTYPE_E::INTERNAL DPI DMA instructions).

                                                                 [F] must not be set for LLC/DRAM pointers that are not written to:

                                                                 * The DPI_DMA_PTR_S's in the first-pointers area for DPI_HDR_XTYPE_E::OUTBOUND
                                                                   and DPI_HDR_XTYPE_E::INTERNAL DPI DMA instructions. */
        uint64_t ac                    : 1;  /**< [ 61: 61] Allocate LLC.  Only used with LLC/DRAM Pointers.
                                                                 This is a hint to DPI that the cache blocks should be allocated in
                                                                 the LLC (if they were not already). Should typically be set to allocate the
                                                                 referenced cache blocks into the LLC.

                                                                 When the LLC/DRAM pointer is a source of data (e.g. a DPI_HDR_XTYPE_E::OUTBOUND
                                                                 DPI DMA instruction), the referenced cache blocks are not allocated into the LLC
                                                                 as part of completing the DMA (when not already present in the LLC) if [AC]
                                                                 is clear.

                                                                 When the LLC/DRAM pointer is a destination for data (e.g. a
                                                                 DPI_HDR_XTYPE_E::INBOUND DPI DMA instruction), the referenced cache blocks are
                                                                 not allocated into the cache as part of completing the DMA (when not already
                                                                 present in the LLC) if [AC] is clear, and either:

                                                                 * the entire cache block is written by this LLC/DRAM pointer, or

                                                                 * [F] is set so that the entire cache block can be written. */
        uint64_t bed                   : 1;  /**< [ 60: 60] Big-endian data.
                                                                 Only used with LLC/DRAM pointers. */
        uint64_t reserved_16_59        : 44;
        uint64_t length                : 16; /**< [ 15:  0] For LLC/DRAM - only bits 11:0 used. bits 15:12 should be zero.

                                                                 Size in bytes of the contiguous space specified by PTR. A SIZE value of 0x0 is
                                                                 illegal.

                                                                 Note that the sum of the sizes in the first-pointers area must always exactly
                                                                 equal the sum of the sizes/lengths in the last-pointers area:

                                                                 * With DPI_HDR_XTYPE_E::OUTBOUND and DPI_HDR_XTYPE_E::INBOUND DPI DMA
                                                                   instructions, the sum of the (DPI_DMA_INSTR_HDR_S[NFST] number of)
                                                                   DPI_DMA_PTR_S[LENGTH]'s in the first pointers block must exactly equal
                                                                   the sum of the (DPI_DMA_INSTR_HDR_S[NLST] number of) length DPI components in
                                                                   the last pointers block.

                                                                 * With DPI_HDR_XTYPE_E::INTERNAL DPI DMA instructions, the sum of the
                                                                   (DPI_DMA_INSTR_HDR_S[NFST] number of) DPI_DMA_PTR_S[LENGTH]'s in the
                                                                   first pointers block must exactly equal the sum of the
                                                                   (DPI_DMA_INSTR_HDR_S[NLST] number of) DPI_DMA_PTR_S[LENGTH]'s in the
                                                                   last pointers block. */
#else /* Word 0 - Little Endian */
        uint64_t length                : 16; /**< [ 15:  0] For LLC/DRAM - only bits 11:0 used. bits 15:12 should be zero.

                                                                 Size in bytes of the contiguous space specified by PTR. A SIZE value of 0x0 is
                                                                 illegal.

                                                                 Note that the sum of the sizes in the first-pointers area must always exactly
                                                                 equal the sum of the sizes/lengths in the last-pointers area:

                                                                 * With DPI_HDR_XTYPE_E::OUTBOUND and DPI_HDR_XTYPE_E::INBOUND DPI DMA
                                                                   instructions, the sum of the (DPI_DMA_INSTR_HDR_S[NFST] number of)
                                                                   DPI_DMA_PTR_S[LENGTH]'s in the first pointers block must exactly equal
                                                                   the sum of the (DPI_DMA_INSTR_HDR_S[NLST] number of) length DPI components in
                                                                   the last pointers block.

                                                                 * With DPI_HDR_XTYPE_E::INTERNAL DPI DMA instructions, the sum of the
                                                                   (DPI_DMA_INSTR_HDR_S[NFST] number of) DPI_DMA_PTR_S[LENGTH]'s in the
                                                                   first pointers block must exactly equal the sum of the
                                                                   (DPI_DMA_INSTR_HDR_S[NLST] number of) DPI_DMA_PTR_S[LENGTH]'s in the
                                                                   last pointers block. */
        uint64_t reserved_16_59        : 44;
        uint64_t bed                   : 1;  /**< [ 60: 60] Big-endian data.
                                                                 Only used with LLC/DRAM pointers. */
        uint64_t ac                    : 1;  /**< [ 61: 61] Allocate LLC.  Only used with LLC/DRAM Pointers.
                                                                 This is a hint to DPI that the cache blocks should be allocated in
                                                                 the LLC (if they were not already). Should typically be set to allocate the
                                                                 referenced cache blocks into the LLC.

                                                                 When the LLC/DRAM pointer is a source of data (e.g. a DPI_HDR_XTYPE_E::OUTBOUND
                                                                 DPI DMA instruction), the referenced cache blocks are not allocated into the LLC
                                                                 as part of completing the DMA (when not already present in the LLC) if [AC]
                                                                 is clear.

                                                                 When the LLC/DRAM pointer is a destination for data (e.g. a
                                                                 DPI_HDR_XTYPE_E::INBOUND DPI DMA instruction), the referenced cache blocks are
                                                                 not allocated into the cache as part of completing the DMA (when not already
                                                                 present in the LLC) if [AC] is clear, and either:

                                                                 * the entire cache block is written by this LLC/DRAM pointer, or

                                                                 * [F] is set so that the entire cache block can be written. */
        uint64_t f                     : 1;  /**< [ 62: 62] Full-block write operations are allowed.
                                                                 Only used with LLC/DRAM pointers.

                                                                 When set, the hardware is permitted to write all the bytes in the cache blocks
                                                                 covered by [PTR] .. [PTR] + [LENGTH] - 1. This can improve memory system performance
                                                                 when the write misses in the LLC.

                                                                 [F] can only be set for LLC/DRAM pointers that can be written to:

                                                                 * The DPI_DMA_PTR_S's in the first-pointers area that are write pointers
                                                                   for DPI_HDR_XTYPE_E::INBOUND DPI DMA instructions.

                                                                 * The DPI_DMA_PTR_S's in the last-pointers area that are always write
                                                                   pointers (when present for DPI_HDR_XTYPE_E::INTERNAL DPI DMA instructions).

                                                                 [F] must not be set for LLC/DRAM pointers that are not written to:

                                                                 * The DPI_DMA_PTR_S's in the first-pointers area for DPI_HDR_XTYPE_E::OUTBOUND
                                                                   and DPI_HDR_XTYPE_E::INTERNAL DPI DMA instructions. */
        uint64_t i                     : 1;  /**< [ 63: 63] Invert free. Only used with LLC/DRAM pointers.

                                                                 This bit gives the software the ability to free buffers
                                                                 independently for a DPI_HDR_XTYPE_E::OUTBOUND DMA transfer when
                                                                 DPI_DMA_INSTR_HDR_S[II] is clear. See DPI_DMA_INSTR_HDR_S[II] and
                                                                 DPI_DMA_INSTR_HDR_S[FL].

                                                                 [I] is not used by DPI when DPI_DMA_INSTR_HDR_S[II] is set. [I] must not be set,
                                                                 and DPI never frees buffers for DPI_HDR_XTYPE_E::INBOUND and
                                                                 DPI_HDR_XTYPE_E::INTERNAL DPI DMA instructions. */
#endif /* Word 0 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 1 - Big Endian */
        uint64_t ptr                   : 64; /**< [127: 64] For LLC/DRAM - bits \<52:0\> used for byte pointer. Points to where the packet data
                                                                 starts. PTR can be any byte alignment. Note that [PTR] is interpreted as a
                                                                 little-endian byte pointer when BED is clear, a big-endian byte pointer when
                                                                 [BED] is set. Bits \<63:53\> ignored and should be zero For MAC - 64-bit memory
                                                                 space pointer */
#else /* Word 1 - Little Endian */
        uint64_t ptr                   : 64; /**< [127: 64] For LLC/DRAM - bits \<52:0\> used for byte pointer. Points to where the packet data
                                                                 starts. PTR can be any byte alignment. Note that [PTR] is interpreted as a
                                                                 little-endian byte pointer when BED is clear, a big-endian byte pointer when
                                                                 [BED] is set. Bits \<63:53\> ignored and should be zero For MAC - 64-bit memory
                                                                 space pointer */
#endif /* Word 1 - End */
    } s;
    /* struct cavm_dpi_dma_ptr_s_s cn; */
};

/**
 * Structure dpi_sdp_addr_s
 *
 * INTERNAL: DPI/SDP Address Structure
 *
 * Address decoding for DPI/SDP CSR address space
 */
union cavm_dpi_sdp_addr_s
{
    uint64_t u;
    struct cavm_dpi_sdp_addr_s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_48_63        : 16;
        uint64_t bit47_46              : 2;  /**< [ 47: 46] NA. */
        uint64_t nn                    : 2;  /**< [ 45: 44] NA. */
        uint64_t did                   : 8;  /**< [ 43: 36] PEM DID. */
        uint64_t region                : 2;  /**< [ 35: 34] NA. */
        uint64_t r33_32                : 2;  /**< [ 33: 32] NA. */
        uint64_t sdp                   : 1;  /**< [ 31: 31] 0 = SDP register, 1 = DPI register. */
        uint64_t ncbonly               : 1;  /**< [ 30: 30] Set for registers that can only be accessed by AP cores. */
        uint64_t epf                   : 5;  /**< [ 29: 25] EPF targeted by AP cores. */
        uint64_t ring                  : 8;  /**< [ 24: 17] SDP packet ring. */
        uint64_t space                 : 1;  /**< [ 16: 16] SDP and DPI decode space:
                                                                 0x0 = SDP commmon space([17]==1)/ MSIX SPACE([17]==0)
                                                                 0x1 = DPI ring space. */
        uint64_t offset                : 12; /**< [ 15:  4] Register offset. */
        uint64_t bit3_0                : 4;  /**< [  3:  0] NA */
#else /* Word 0 - Little Endian */
        uint64_t bit3_0                : 4;  /**< [  3:  0] NA */
        uint64_t offset                : 12; /**< [ 15:  4] Register offset. */
        uint64_t space                 : 1;  /**< [ 16: 16] SDP and DPI decode space:
                                                                 0x0 = SDP commmon space([17]==1)/ MSIX SPACE([17]==0)
                                                                 0x1 = DPI ring space. */
        uint64_t ring                  : 8;  /**< [ 24: 17] SDP packet ring. */
        uint64_t epf                   : 5;  /**< [ 29: 25] EPF targeted by AP cores. */
        uint64_t ncbonly               : 1;  /**< [ 30: 30] Set for registers that can only be accessed by AP cores. */
        uint64_t sdp                   : 1;  /**< [ 31: 31] 0 = SDP register, 1 = DPI register. */
        uint64_t r33_32                : 2;  /**< [ 33: 32] NA. */
        uint64_t region                : 2;  /**< [ 35: 34] NA. */
        uint64_t did                   : 8;  /**< [ 43: 36] PEM DID. */
        uint64_t nn                    : 2;  /**< [ 45: 44] NA. */
        uint64_t bit47_46              : 2;  /**< [ 47: 46] NA. */
        uint64_t reserved_48_63        : 16;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpi_sdp_addr_s_s cn; */
};

/**
 * Structure sdp_buf_info_pair_s
 *
 * SDP Buffer/Information Pair Structure
 * The 16 byte buffer/info pointer pair. Each entry in a PCIe unsolicited
 * output ring is a SDP_BUF_INFO_PAIR_S.
 */
union cavm_sdp_buf_info_pair_s
{
    uint64_t u[2];
    struct cavm_sdp_buf_info_pair_s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t buf_ptr               : 64; /**< [ 63:  0] The buffer pointer. */
#else /* Word 0 - Little Endian */
        uint64_t buf_ptr               : 64; /**< [ 63:  0] The buffer pointer. */
#endif /* Word 0 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 1 - Big Endian */
        uint64_t info_ptr              : 64; /**< [127: 64] The info pointer. \<2:0\> must always be zero.

                                                                 When SDP()_EPF()_R()_OUT_CONTROL[IMODE]=0, [INFO_PTR] must be zero. */
#else /* Word 1 - Little Endian */
        uint64_t info_ptr              : 64; /**< [127: 64] The info pointer. \<2:0\> must always be zero.

                                                                 When SDP()_EPF()_R()_OUT_CONTROL[IMODE]=0, [INFO_PTR] must be zero. */
#endif /* Word 1 - End */
    } s;
    /* struct cavm_sdp_buf_info_pair_s_s cn8; */
    struct cavm_sdp_buf_info_pair_s_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t buf_ptr               : 64; /**< [ 63:  0] The buffer pointer. */
#else /* Word 0 - Little Endian */
        uint64_t buf_ptr               : 64; /**< [ 63:  0] The buffer pointer. */
#endif /* Word 0 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 1 - Big Endian */
        uint64_t info_ptr              : 64; /**< [127: 64] The info pointer. \<2:0\> must always be zero.

                                                                 When SDP()_R()_OUT_CONTROL[IMODE]=0, [INFO_PTR] must be zero. */
#else /* Word 1 - Little Endian */
        uint64_t info_ptr              : 64; /**< [127: 64] The info pointer. \<2:0\> must always be zero.

                                                                 When SDP()_R()_OUT_CONTROL[IMODE]=0, [INFO_PTR] must be zero. */
#endif /* Word 1 - End */
    } cn9;
};

/**
 * Structure sdp_func_sel_s
 *
 * SDP Function Selector Structure
 * DPI DMA function selector for MACs
 */
union cavm_sdp_func_sel_s
{
    uint32_t u;
    struct cavm_sdp_func_sel_s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint32_t reserved_12_31        : 20;
        uint32_t epf                   : 4;  /**< [ 11:  8] Selects the PCIe physical function within the referenced MAC.

                                                                 [EPF] must select a valid physical function in a PCIe MAC. */
        uint32_t func                  : 8;  /**< [  7:  0] Selects the function within the physical function [EPF]
                                                                 within the referenced MAC.

                                                                 0x0 = The physical function [EPF].
                                                                 0x1 = The first virtual function within the physical function [EPF].
                                                                 0x2 = The second virtual function within the physical function [EPF].
                                                                 0x3 = ...

                                                                 When [FUNC] is nonzero, it must select a valid virtual
                                                                 function.

                                                                 [FUNC] must be zero for any MAC that isn't PCIe, doesn't
                                                                 support the PCIe SR-IOV standard, or doesn't have PCIe SR-IOV
                                                                 enabled. */
#else /* Word 0 - Little Endian */
        uint32_t func                  : 8;  /**< [  7:  0] Selects the function within the physical function [EPF]
                                                                 within the referenced MAC.

                                                                 0x0 = The physical function [EPF].
                                                                 0x1 = The first virtual function within the physical function [EPF].
                                                                 0x2 = The second virtual function within the physical function [EPF].
                                                                 0x3 = ...

                                                                 When [FUNC] is nonzero, it must select a valid virtual
                                                                 function.

                                                                 [FUNC] must be zero for any MAC that isn't PCIe, doesn't
                                                                 support the PCIe SR-IOV standard, or doesn't have PCIe SR-IOV
                                                                 enabled. */
        uint32_t epf                   : 4;  /**< [ 11:  8] Selects the PCIe physical function within the referenced MAC.

                                                                 [EPF] must select a valid physical function in a PCIe MAC. */
        uint32_t reserved_12_31        : 20;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdp_func_sel_s_s cn; */
};

/**
 * Structure sdp_instr_hdr_s
 *
 * SDP Instruction Header Structure
 * SDP_INSTR_HDR_S is the instruction header that is present in
 * each PCIe instruction in remote host memory.
 */
union cavm_sdp_instr_hdr_s
{
    uint64_t u;
    struct cavm_sdp_instr_hdr_s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_63           : 1;
        uint64_t g                     : 1;  /**< [ 62: 62] Gather is used.

                                                                 When [G] is set and [GSZ]!=0, the instruction is an indirect gather
                                                                 instruction. In this case:

                                                                 * DPTR points at an array of SDP_SGLIST_COMPONENT_S's in the
                                                                   remote host's memory, and the pointers in that (indirect) gather
                                                                   list point at the packet data in the host's memory.

                                                                 * [GSZ] is the number of pointers in the indirect gather list.

                                                                 * The sum of the [GSZ] valid SDP_SGLIST_COMPONENT_S[LEN*]
                                                                   must equal [TLEN]-[FSZ].

                                                                 When [G] is set and [GSZ]==0, the instruction is a direct gather
                                                                 instruction. In this case:

                                                                 * The instruction contains a number of DPTRs (DPTR, DPTR 1, ...) and
                                                                   associated lengths (LEN, LEN 1, ..., respectively) that point at
                                                                   the packet data in the host's memory.

                                                                 * The length word (containing LEN, LEN 1, LEN 2, and LEN 3) is present
                                                                   immediately following the (eight-byte-aligned) Front Data, if front
                                                                   data is present. If the instruction is 32 bytes or if [FSZ]==0, then
                                                                   front data is not present, and the length word immediately follows
                                                                   the SDP_INSTR_HDR_S in the direct gather instruction.

                                                                 * LEN and LEN 1 must be nonzero and must indicate the number of input
                                                                   packet bytes pointed at by DPTR and DPTR 1, respectively.

                                                                 * If LEN 2 is nonzero, then DPTR 2 is present and LEN 2 must indicate
                                                                   the number of input packet bytes pointed at by DPTR 2. LEN 2 must be
                                                                   zero for 32-byte direct gather instructions.

                                                                 * If LEN 3 is nonzero, then DPTR 3 is present, and LEN 3 must indicate
                                                                   the number of input packet bytes pointed at by DPTR 3, and LEN 2 must
                                                                   also be nonzero. LEN 3 must be zero for 32-byte direct gather
                                                                   instructions.

                                                                 * LEN + (LEN 1) + (LEN 2) + (LEN 3) must equal [TLEN]-[FSZ].

                                                                 When [G] is clear, the instruction is a direct data instruction. In
                                                                 this case:

                                                                 * The instruction contains exactly one DPTR, which points directly at
                                                                   the packet data in the host's memory.

                                                                 * The amount of packet data at DPTR in host memory is [TLEN]-[FSZ]. */
        uint64_t gsz                   : 14; /**< [ 61: 48] When [G] is one, [GSZ] is the number of gather list pointers in remote
                                                                 host memory. If [GSZ] is zero, direct gather is selected. If [GSZ]
                                                                 is not zero, indirect gather is selected. [GSZ] must be zero when
                                                                 [G] is clear. [GSZ] must never be one (i.e. an indirect gather list
                                                                 must be at least two entries). Note that with indirect gather,
                                                                 the number of SDP_SGLIST_COMPONENT_S's for the instruction in
                                                                 remote host memory is ([GSZ] + 3)/4. */
        uint64_t fsz                   : 6;  /**< [ 47: 42] The number of front data bytes directly included in the PCIe instruction.
                                                                 The front data begins the input packet.

                                                                 For the direct data and indirect gather cases (i.e. [G]==0 or [GSZ]!=0):

                                                                 * [FSZ] must be \<= 23 with a 32-byte instruction, \<= 55 with a 64-byte
                                                                   instruction. An [FSZ] value between 17 and 23 with a 32-byte instruction
                                                                   can be used to insert a pad between the front data and the DPTR data.
                                                                   The hardware inserts unpredictable bytes for the extra bytes
                                                                   that are not actually contained in the DPI instruction in this case.
                                                                   The same is true for [FSZ] values between 49 and 55 with a 64-byte
                                                                   instruction.

                                                                 For the direct gather case (i.e. [G]!=0 and [GSZ]=0):

                                                                 * With 32-byte direct gather instructions:

                                                                   o [FSZ] must be zero (and the Front Data field is not present in
                                                                     the PCIe Instruction).

                                                                 * With 64-byte direct gather instructions:

                                                                   o When [FSZ] is zero, the Front Data field is not present.

                                                                   o When [FSZ] is nonzero, the size of the Front Data field
                                                                     in the PCIe instruction in remote host memory is [FSZ] rounded
                                                                     up to the nearest multiple of 8 bytes.

                                                                   o [FSZ] can legally range between 0 and 32 bytes (inclusive) when LEN 2
                                                                     and LEN 3 are both zero.

                                                                   o [FSZ] can legally range between 0 and 24 bytes (inclusive) when LEN 3
                                                                     is zero.

                                                                   o [FSZ] can legally range between 0 and 16 bytes (inclusive) in all
                                                                     circumstances. */
        uint64_t pkind                 : 6;  /**< [ 41: 36] The port kind assigned to the packet in PKI. */
        uint64_t reserved_16_35        : 20;
        uint64_t tlen                  : 16; /**< [ 15:  0] The length of the input packet in bytes.  This should include the length of
                                                                 the front data (FSZ), plus the sum of packet data from all direct or indirect
                                                                 gather buffers.

                                                                 [TLEN] must not be zero. [TLEN] must be greater than or equal to [FSZ].
                                                                 [TLEN]-[FSZ] is the number of bytes that CNXXXX will fetch from the DPTR's. */
#else /* Word 0 - Little Endian */
        uint64_t tlen                  : 16; /**< [ 15:  0] The length of the input packet in bytes.  This should include the length of
                                                                 the front data (FSZ), plus the sum of packet data from all direct or indirect
                                                                 gather buffers.

                                                                 [TLEN] must not be zero. [TLEN] must be greater than or equal to [FSZ].
                                                                 [TLEN]-[FSZ] is the number of bytes that CNXXXX will fetch from the DPTR's. */
        uint64_t reserved_16_35        : 20;
        uint64_t pkind                 : 6;  /**< [ 41: 36] The port kind assigned to the packet in PKI. */
        uint64_t fsz                   : 6;  /**< [ 47: 42] The number of front data bytes directly included in the PCIe instruction.
                                                                 The front data begins the input packet.

                                                                 For the direct data and indirect gather cases (i.e. [G]==0 or [GSZ]!=0):

                                                                 * [FSZ] must be \<= 23 with a 32-byte instruction, \<= 55 with a 64-byte
                                                                   instruction. An [FSZ] value between 17 and 23 with a 32-byte instruction
                                                                   can be used to insert a pad between the front data and the DPTR data.
                                                                   The hardware inserts unpredictable bytes for the extra bytes
                                                                   that are not actually contained in the DPI instruction in this case.
                                                                   The same is true for [FSZ] values between 49 and 55 with a 64-byte
                                                                   instruction.

                                                                 For the direct gather case (i.e. [G]!=0 and [GSZ]=0):

                                                                 * With 32-byte direct gather instructions:

                                                                   o [FSZ] must be zero (and the Front Data field is not present in
                                                                     the PCIe Instruction).

                                                                 * With 64-byte direct gather instructions:

                                                                   o When [FSZ] is zero, the Front Data field is not present.

                                                                   o When [FSZ] is nonzero, the size of the Front Data field
                                                                     in the PCIe instruction in remote host memory is [FSZ] rounded
                                                                     up to the nearest multiple of 8 bytes.

                                                                   o [FSZ] can legally range between 0 and 32 bytes (inclusive) when LEN 2
                                                                     and LEN 3 are both zero.

                                                                   o [FSZ] can legally range between 0 and 24 bytes (inclusive) when LEN 3
                                                                     is zero.

                                                                   o [FSZ] can legally range between 0 and 16 bytes (inclusive) in all
                                                                     circumstances. */
        uint64_t gsz                   : 14; /**< [ 61: 48] When [G] is one, [GSZ] is the number of gather list pointers in remote
                                                                 host memory. If [GSZ] is zero, direct gather is selected. If [GSZ]
                                                                 is not zero, indirect gather is selected. [GSZ] must be zero when
                                                                 [G] is clear. [GSZ] must never be one (i.e. an indirect gather list
                                                                 must be at least two entries). Note that with indirect gather,
                                                                 the number of SDP_SGLIST_COMPONENT_S's for the instruction in
                                                                 remote host memory is ([GSZ] + 3)/4. */
        uint64_t g                     : 1;  /**< [ 62: 62] Gather is used.

                                                                 When [G] is set and [GSZ]!=0, the instruction is an indirect gather
                                                                 instruction. In this case:

                                                                 * DPTR points at an array of SDP_SGLIST_COMPONENT_S's in the
                                                                   remote host's memory, and the pointers in that (indirect) gather
                                                                   list point at the packet data in the host's memory.

                                                                 * [GSZ] is the number of pointers in the indirect gather list.

                                                                 * The sum of the [GSZ] valid SDP_SGLIST_COMPONENT_S[LEN*]
                                                                   must equal [TLEN]-[FSZ].

                                                                 When [G] is set and [GSZ]==0, the instruction is a direct gather
                                                                 instruction. In this case:

                                                                 * The instruction contains a number of DPTRs (DPTR, DPTR 1, ...) and
                                                                   associated lengths (LEN, LEN 1, ..., respectively) that point at
                                                                   the packet data in the host's memory.

                                                                 * The length word (containing LEN, LEN 1, LEN 2, and LEN 3) is present
                                                                   immediately following the (eight-byte-aligned) Front Data, if front
                                                                   data is present. If the instruction is 32 bytes or if [FSZ]==0, then
                                                                   front data is not present, and the length word immediately follows
                                                                   the SDP_INSTR_HDR_S in the direct gather instruction.

                                                                 * LEN and LEN 1 must be nonzero and must indicate the number of input
                                                                   packet bytes pointed at by DPTR and DPTR 1, respectively.

                                                                 * If LEN 2 is nonzero, then DPTR 2 is present and LEN 2 must indicate
                                                                   the number of input packet bytes pointed at by DPTR 2. LEN 2 must be
                                                                   zero for 32-byte direct gather instructions.

                                                                 * If LEN 3 is nonzero, then DPTR 3 is present, and LEN 3 must indicate
                                                                   the number of input packet bytes pointed at by DPTR 3, and LEN 2 must
                                                                   also be nonzero. LEN 3 must be zero for 32-byte direct gather
                                                                   instructions.

                                                                 * LEN + (LEN 1) + (LEN 2) + (LEN 3) must equal [TLEN]-[FSZ].

                                                                 When [G] is clear, the instruction is a direct data instruction. In
                                                                 this case:

                                                                 * The instruction contains exactly one DPTR, which points directly at
                                                                   the packet data in the host's memory.

                                                                 * The amount of packet data at DPTR in host memory is [TLEN]-[FSZ]. */
        uint64_t reserved_63           : 1;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdp_instr_hdr_s_s cn; */
};

/**
 * Structure sdp_length_s
 *
 * SDP Packet Output Length Structure
 * The 8 byte packet length structure that is appended to the end of
 * the info pointer bytes when SDP()_R()_OUT_CONTROL[IMODE]=1, or
 * prepended as the first 8 bytes written in front of the packet when
 * SDP()_R()_OUT_CONTROL[IMODE]=0.
 */
union cavm_sdp_length_s
{
    uint64_t u;
    struct cavm_sdp_length_s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_16_63        : 48;
        uint64_t len                   : 16; /**< [ 15:  0] The packet length in bytes.

                                                                 When SDP()_EPF()_R()_OUT_CONTROL[IMODE]=0:

                                                                 * Hardware writes MINIMUM([LEN]+8,SDP()_EPF()_R()_OUT_CONTROL[BSIZE])
                                                                   bytes to the first SDP_BUF_INFO_PAIR_S[BUF_PTR]. The "+8" is
                                                                   due to the SDP_LENGTH_S prepend.

                                                                 * If packet bytes remain, hardware uses additional
                                                                   SDP_BUF_INFO_PAIR_S[BUF_PTR]'s for the packet data, writing
                                                                   SDP()_EPF()_R()_OUT_CONTROL[BSIZE] packet data bytes to each until
                                                                   the packet data is consumed.

                                                                 * The number of SDP_BUF_INFO_PAIR_S's consumed by a packet is
                                                                   ceiling(([LEN]+8)/SDP()_EPF()_R()_OUT_CONTROL[BSIZE]), where
                                                                   ceiling() rounds up to the nearest integer.

                                                                 * The SDP_LENGTH_S will be unaligned in host memory if
                                                                   the first SDP_BUF_INFO_PAIR_S[BUF_PTR] is unaligned.

                                                                 When SDP()_EPF()_R()_OUT_CONTROL[IMODE]=1:

                                                                 * Hardware skips the first SDP()_EPF()_R()_OUT_CONTROL[ISIZE] bytes
                                                                   of the packet, writing the remaining packet bytes to the
                                                                   first SDP_BUF_INFO_PAIR_S[BUF_PTR].

                                                                 * If further packet bytes follow, hardware uses additional
                                                                   SDP_BUF_INFO_PAIR_S[BUF_PTR]'s for the packet data, writing
                                                                   SDP()_EPF()_R()_OUT_CONTROL[BSIZE] packet data bytes to each until
                                                                   the packet data is consumed.

                                                                 * Finally, hardware writes the first SDP()_EPF()_R()_OUT_CONTROL[ISIZE]
                                                                   bytes of the packet followed by the SDP_LENGTH_S to
                                                                   the first SDP_BUF_INFO_PAIR_S[INFO_PTR]. There is pad between
                                                                   the packet data and the SDP_LENGTH_S if necessary to
                                                                   naturally-align the 8B SDP_LENGTH_S.

                                                                 * The number of SDP_BUF_INFO_PAIR_S's consumed by a packet is
                                                                   ceiling(([LEN]-SDP()_EPF()_R()_OUT_CONTROL[ISIZE])/SDP()_EPF()_R()_OUT_CONTROL[BSIZE])
                                                                   when [LEN] \> SDP()_EPF()_R()_OUT_CONTROL[ISIZE],
                                                                   where ceiling() rounds up to the nearest integer. Only the
                                                                   first SDP_BUF_INFO_PAIR_S[INFO_PTR] is used.

                                                                 * Minimum allowed output packet size is 1 byte.

                                                                 * If [LEN] \<= SDP()_EPF()_R()_OUT_CONTROL[ISIZE], SDP uses one SDP_BUF_INFO_PAIR_S
                                                                   for the packet, writes all the packet data to SDP_BUF_INFO_PAIR_S[INFO_PTR],
                                                                   and doesn't use SDP_BUF_INFO_PAIR_S[BUF_PTR]. SDP writes the SDP_LENGTH_S to
                                                                   the same location as if the packet were large in this case, padding as needed. */
#else /* Word 0 - Little Endian */
        uint64_t len                   : 16; /**< [ 15:  0] The packet length in bytes.

                                                                 When SDP()_EPF()_R()_OUT_CONTROL[IMODE]=0:

                                                                 * Hardware writes MINIMUM([LEN]+8,SDP()_EPF()_R()_OUT_CONTROL[BSIZE])
                                                                   bytes to the first SDP_BUF_INFO_PAIR_S[BUF_PTR]. The "+8" is
                                                                   due to the SDP_LENGTH_S prepend.

                                                                 * If packet bytes remain, hardware uses additional
                                                                   SDP_BUF_INFO_PAIR_S[BUF_PTR]'s for the packet data, writing
                                                                   SDP()_EPF()_R()_OUT_CONTROL[BSIZE] packet data bytes to each until
                                                                   the packet data is consumed.

                                                                 * The number of SDP_BUF_INFO_PAIR_S's consumed by a packet is
                                                                   ceiling(([LEN]+8)/SDP()_EPF()_R()_OUT_CONTROL[BSIZE]), where
                                                                   ceiling() rounds up to the nearest integer.

                                                                 * The SDP_LENGTH_S will be unaligned in host memory if
                                                                   the first SDP_BUF_INFO_PAIR_S[BUF_PTR] is unaligned.

                                                                 When SDP()_EPF()_R()_OUT_CONTROL[IMODE]=1:

                                                                 * Hardware skips the first SDP()_EPF()_R()_OUT_CONTROL[ISIZE] bytes
                                                                   of the packet, writing the remaining packet bytes to the
                                                                   first SDP_BUF_INFO_PAIR_S[BUF_PTR].

                                                                 * If further packet bytes follow, hardware uses additional
                                                                   SDP_BUF_INFO_PAIR_S[BUF_PTR]'s for the packet data, writing
                                                                   SDP()_EPF()_R()_OUT_CONTROL[BSIZE] packet data bytes to each until
                                                                   the packet data is consumed.

                                                                 * Finally, hardware writes the first SDP()_EPF()_R()_OUT_CONTROL[ISIZE]
                                                                   bytes of the packet followed by the SDP_LENGTH_S to
                                                                   the first SDP_BUF_INFO_PAIR_S[INFO_PTR]. There is pad between
                                                                   the packet data and the SDP_LENGTH_S if necessary to
                                                                   naturally-align the 8B SDP_LENGTH_S.

                                                                 * The number of SDP_BUF_INFO_PAIR_S's consumed by a packet is
                                                                   ceiling(([LEN]-SDP()_EPF()_R()_OUT_CONTROL[ISIZE])/SDP()_EPF()_R()_OUT_CONTROL[BSIZE])
                                                                   when [LEN] \> SDP()_EPF()_R()_OUT_CONTROL[ISIZE],
                                                                   where ceiling() rounds up to the nearest integer. Only the
                                                                   first SDP_BUF_INFO_PAIR_S[INFO_PTR] is used.

                                                                 * Minimum allowed output packet size is 1 byte.

                                                                 * If [LEN] \<= SDP()_EPF()_R()_OUT_CONTROL[ISIZE], SDP uses one SDP_BUF_INFO_PAIR_S
                                                                   for the packet, writes all the packet data to SDP_BUF_INFO_PAIR_S[INFO_PTR],
                                                                   and doesn't use SDP_BUF_INFO_PAIR_S[BUF_PTR]. SDP writes the SDP_LENGTH_S to
                                                                   the same location as if the packet were large in this case, padding as needed. */
        uint64_t reserved_16_63        : 48;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdp_length_s_s cn8; */
    struct cavm_sdp_length_s_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_16_63        : 48;
        uint64_t len                   : 16; /**< [ 15:  0] The packet length in bytes.

                                                                 When SDP()_R()_OUT_CONTROL[IMODE]=0:

                                                                 * Hardware writes MINIMUM([LEN]+8,SDP()_R()_OUT_CONTROL[BSIZE])
                                                                   bytes to the first SDP_BUF_INFO_PAIR_S[BUF_PTR]. The "+8" is
                                                                   due to the SDP_LENGTH_S prepend.

                                                                 * If packet bytes remain, hardware uses additional
                                                                   SDP_BUF_INFO_PAIR_S[BUF_PTR]'s for the packet data, writing
                                                                   SDP()_R()_OUT_CONTROL[BSIZE] packet data bytes to each until
                                                                   the packet data is consumed.

                                                                 * The number of SDP_BUF_INFO_PAIR_S's consumed by a packet is
                                                                   ceiling(([LEN]+8)/SDP()_R()_OUT_CONTROL[BSIZE]), where
                                                                   ceiling() rounds up to the nearest integer.

                                                                 * The SDP_LENGTH_S will be unaligned in host memory if
                                                                   the first SDP_BUF_INFO_PAIR_S[BUF_PTR] is unaligned.

                                                                 When SDP()_R()_OUT_CONTROL[IMODE]=1:

                                                                 * Hardware skips the first SDP()_R()_OUT_CONTROL[ISIZE] bytes
                                                                   of the packet, writing the remaining packet bytes to the
                                                                   first SDP_BUF_INFO_PAIR_S[BUF_PTR].

                                                                 * If further packet bytes follow, hardware uses additional
                                                                   SDP_BUF_INFO_PAIR_S[BUF_PTR]'s for the packet data, writing
                                                                   SDP()_R()_OUT_CONTROL[BSIZE] packet data bytes to each until
                                                                   the packet data is consumed.

                                                                 * Finally, hardware writes the first SDP()_R()_OUT_CONTROL[ISIZE]
                                                                   bytes of the packet followed by the SDP_LENGTH_S to
                                                                   the first SDP_BUF_INFO_PAIR_S[INFO_PTR]. There is pad between
                                                                   the packet data and the SDP_LENGTH_S if necessary to
                                                                   naturally-align the 8B SDP_LENGTH_S.

                                                                 * The number of SDP_BUF_INFO_PAIR_S's consumed by a packet is
                                                                   ceiling(([LEN]-SDP()_R()_OUT_CONTROL[ISIZE])/SDP()_R()_OUT_CONTROL[BSIZE]),
                                                                   where ceiling() rounds up to the nearest integer. Only the
                                                                   first SDP_BUF_INFO_PAIR_S[INFO_PTR] is used.

                                                                 * Minimum allowed output packet size is 1 byte. */
#else /* Word 0 - Little Endian */
        uint64_t len                   : 16; /**< [ 15:  0] The packet length in bytes.

                                                                 When SDP()_R()_OUT_CONTROL[IMODE]=0:

                                                                 * Hardware writes MINIMUM([LEN]+8,SDP()_R()_OUT_CONTROL[BSIZE])
                                                                   bytes to the first SDP_BUF_INFO_PAIR_S[BUF_PTR]. The "+8" is
                                                                   due to the SDP_LENGTH_S prepend.

                                                                 * If packet bytes remain, hardware uses additional
                                                                   SDP_BUF_INFO_PAIR_S[BUF_PTR]'s for the packet data, writing
                                                                   SDP()_R()_OUT_CONTROL[BSIZE] packet data bytes to each until
                                                                   the packet data is consumed.

                                                                 * The number of SDP_BUF_INFO_PAIR_S's consumed by a packet is
                                                                   ceiling(([LEN]+8)/SDP()_R()_OUT_CONTROL[BSIZE]), where
                                                                   ceiling() rounds up to the nearest integer.

                                                                 * The SDP_LENGTH_S will be unaligned in host memory if
                                                                   the first SDP_BUF_INFO_PAIR_S[BUF_PTR] is unaligned.

                                                                 When SDP()_R()_OUT_CONTROL[IMODE]=1:

                                                                 * Hardware skips the first SDP()_R()_OUT_CONTROL[ISIZE] bytes
                                                                   of the packet, writing the remaining packet bytes to the
                                                                   first SDP_BUF_INFO_PAIR_S[BUF_PTR].

                                                                 * If further packet bytes follow, hardware uses additional
                                                                   SDP_BUF_INFO_PAIR_S[BUF_PTR]'s for the packet data, writing
                                                                   SDP()_R()_OUT_CONTROL[BSIZE] packet data bytes to each until
                                                                   the packet data is consumed.

                                                                 * Finally, hardware writes the first SDP()_R()_OUT_CONTROL[ISIZE]
                                                                   bytes of the packet followed by the SDP_LENGTH_S to
                                                                   the first SDP_BUF_INFO_PAIR_S[INFO_PTR]. There is pad between
                                                                   the packet data and the SDP_LENGTH_S if necessary to
                                                                   naturally-align the 8B SDP_LENGTH_S.

                                                                 * The number of SDP_BUF_INFO_PAIR_S's consumed by a packet is
                                                                   ceiling(([LEN]-SDP()_R()_OUT_CONTROL[ISIZE])/SDP()_R()_OUT_CONTROL[BSIZE]),
                                                                   where ceiling() rounds up to the nearest integer. Only the
                                                                   first SDP_BUF_INFO_PAIR_S[INFO_PTR] is used.

                                                                 * Minimum allowed output packet size is 1 byte. */
        uint64_t reserved_16_63        : 48;
#endif /* Word 0 - End */
    } cn9;
};

/**
 * Structure sdp_sglist_component_s
 *
 * SDP Scatter/Gather List Component Structure
 * A scatter or gather list is an array of SDP_SGLIST_COMPONENT_S's
 * in remote host memory. The number of SDP_SGLIST_COMPONENT_S's
 * in the array is (LL + 3)/4, where LL is either
 * SDP_INSTR_HDR_S[GSZ]. The array must be naturally-aligned on an
 * 8B boundary in remote host memory.
 */
union cavm_sdp_sglist_component_s
{
    uint64_t u[5];
    struct cavm_sdp_sglist_component_s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t len0                  : 16; /**< [ 63: 48] The number of bytes at [PTR0] on the remote host. Must be greater
                                                                 than zero. */
        uint64_t len1                  : 16; /**< [ 47: 32] The number of bytes at [PTR1] on the remote host. Must be greater
                                                                 than zero when valid. [LEN1,PTR1] are invalid in the last
                                                                 SDP_SGLIST_COMPONENT_S in the array (that is the scatter/
                                                                 gather list) when (LL AND 0x3) is 1, where LL is
                                                                 SDP_INSTR_HDR_S[GSZ]. */
        uint64_t len2                  : 16; /**< [ 31: 16] The number of bytes at [PTR2] on the remote host. Must be greater
                                                                 than zero when valid. [LEN2,PTR2] are invalid in the last
                                                                 SDP_SGLIST_COMPONENT_S in the array (that is the scatter/
                                                                 gather list) when (LL AND 0x3) is 1 or 2, where LL is
                                                                 SDP_INSTR_HDR_S[GSZ]. */
        uint64_t len3                  : 16; /**< [ 15:  0] The number of bytes at [PTR3] on the remote host. Must be greater
                                                                 than zero when valid. [LEN3,PTR3] are invalid in the last
                                                                 SDP_SGLIST_COMPONENT_S in the array (that is the scatter/
                                                                 gather list) when (LL AND 0x3) is 1 or 2 or 3, where LL is
                                                                 SDP_INSTR_HDR_S[GSZ]. */
#else /* Word 0 - Little Endian */
        uint64_t len3                  : 16; /**< [ 15:  0] The number of bytes at [PTR3] on the remote host. Must be greater
                                                                 than zero when valid. [LEN3,PTR3] are invalid in the last
                                                                 SDP_SGLIST_COMPONENT_S in the array (that is the scatter/
                                                                 gather list) when (LL AND 0x3) is 1 or 2 or 3, where LL is
                                                                 SDP_INSTR_HDR_S[GSZ]. */
        uint64_t len2                  : 16; /**< [ 31: 16] The number of bytes at [PTR2] on the remote host. Must be greater
                                                                 than zero when valid. [LEN2,PTR2] are invalid in the last
                                                                 SDP_SGLIST_COMPONENT_S in the array (that is the scatter/
                                                                 gather list) when (LL AND 0x3) is 1 or 2, where LL is
                                                                 SDP_INSTR_HDR_S[GSZ]. */
        uint64_t len1                  : 16; /**< [ 47: 32] The number of bytes at [PTR1] on the remote host. Must be greater
                                                                 than zero when valid. [LEN1,PTR1] are invalid in the last
                                                                 SDP_SGLIST_COMPONENT_S in the array (that is the scatter/
                                                                 gather list) when (LL AND 0x3) is 1, where LL is
                                                                 SDP_INSTR_HDR_S[GSZ]. */
        uint64_t len0                  : 16; /**< [ 63: 48] The number of bytes at [PTR0] on the remote host. Must be greater
                                                                 than zero. */
#endif /* Word 0 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 1 - Big Endian */
        uint64_t ptr0                  : 64; /**< [127: 64] The first pointer in the SDP_SGLIST_COMPONENT_S. Must point
                                                                 to a buffer in remote host memory. */
#else /* Word 1 - Little Endian */
        uint64_t ptr0                  : 64; /**< [127: 64] The first pointer in the SDP_SGLIST_COMPONENT_S. Must point
                                                                 to a buffer in remote host memory. */
#endif /* Word 1 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 2 - Big Endian */
        uint64_t ptr1                  : 64; /**< [191:128] The second pointer in the SDP_SGLIST_COMPONENT_S. Must point
                                                                 to a buffer in remote host memory when corresponding [LEN1] is valid. */
#else /* Word 2 - Little Endian */
        uint64_t ptr1                  : 64; /**< [191:128] The second pointer in the SDP_SGLIST_COMPONENT_S. Must point
                                                                 to a buffer in remote host memory when corresponding [LEN1] is valid. */
#endif /* Word 2 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 3 - Big Endian */
        uint64_t ptr2                  : 64; /**< [255:192] The third pointer in the SDP_SGLIST_COMPONENT_S. Must point
                                                                 to a buffer in remote host memory when corresponding [LEN2] is valid. */
#else /* Word 3 - Little Endian */
        uint64_t ptr2                  : 64; /**< [255:192] The third pointer in the SDP_SGLIST_COMPONENT_S. Must point
                                                                 to a buffer in remote host memory when corresponding [LEN2] is valid. */
#endif /* Word 3 - End */
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 4 - Big Endian */
        uint64_t ptr3                  : 64; /**< [319:256] The fourth pointer in the SDP_SGLIST_COMPONENT_S. Must point
                                                                 to a buffer in remote host memory when corresponding [LEN3] is valid. */
#else /* Word 4 - Little Endian */
        uint64_t ptr3                  : 64; /**< [319:256] The fourth pointer in the SDP_SGLIST_COMPONENT_S. Must point
                                                                 to a buffer in remote host memory when corresponding [LEN3] is valid. */
#endif /* Word 4 - End */
    } s;
    /* struct cavm_sdp_sglist_component_s_s cn; */
};

/**
 * Register (NCB) dpi#_bist_status
 *
 * DPI BIST Status Register
 * This is the built-in self-test (BIST) status register. Each bit is the BIST result of an
 * individual memory (per bit, 0 = pass and 1 = fail).
 */
union cavm_dpix_bist_status
{
    uint64_t u;
    struct cavm_dpix_bist_status_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t bist                  : 36; /**< [ 35:  0](RO/H) BIST results. Hardware sets a bit in BIST for memory that fails.
                                                                 Internal:
                                                                 dpi.dma.csr.spi__csr_bist_status[3:0],     35:32
                                                                 dpi.dma.csr.rdb_buff__bist_status[23:0],   31:8
                                                                 dpi.dma.csr.rdb_tmem__bist_status,          7
                                                                 dpi.dma.csr.req_mem__bist_status[1:0],      6:5
                                                                 dpi.dma.csr.dsi__bist_status,               4:4
                                                                 dpi.dma.csr.ncbo__bist_status[2:0],         3:1
                                                                 dpi.dma.csr.ncbi__csr_bist_status           0 */
#else /* Word 0 - Little Endian */
        uint64_t bist                  : 36; /**< [ 35:  0](RO/H) BIST results. Hardware sets a bit in BIST for memory that fails.
                                                                 Internal:
                                                                 dpi.dma.csr.spi__csr_bist_status[3:0],     35:32
                                                                 dpi.dma.csr.rdb_buff__bist_status[23:0],   31:8
                                                                 dpi.dma.csr.rdb_tmem__bist_status,          7
                                                                 dpi.dma.csr.req_mem__bist_status[1:0],      6:5
                                                                 dpi.dma.csr.dsi__bist_status,               4:4
                                                                 dpi.dma.csr.ncbo__bist_status[2:0],         3:1
                                                                 dpi.dma.csr.ncbi__csr_bist_status           0 */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_bist_status_s cn; */
};
typedef union cavm_dpix_bist_status cavm_dpix_bist_status_t;

static inline uint64_t CAVM_DPIX_BIST_STATUS(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_BIST_STATUS(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004000ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_BIST_STATUS", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_BIST_STATUS(a) cavm_dpix_bist_status_t
#define bustype_CAVM_DPIX_BIST_STATUS(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_BIST_STATUS(a) "DPIX_BIST_STATUS"
#define device_bar_CAVM_DPIX_BIST_STATUS(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_BIST_STATUS(a) (a)
#define arguments_CAVM_DPIX_BIST_STATUS(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_bp_test0
 *
 * INTERNAL: DPI Backpressure Test Register 0
 */
union cavm_dpix_bp_test0
{
    uint64_t u;
    struct cavm_dpix_bp_test0_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t enable                : 14; /**< [ 63: 50](R/W) Enable test mode. For diagnostic use only.
                                                                 Internal:
                                                                 Once a bit is set, random backpressure is generated
                                                                 at the corresponding point to allow for more frequent backpressure.
                                                                 \<63\> = Backpressure NCBI DMA Rd requests.
                                                                 \<62\> = Backpressure NCBI INS Rd requests.
                                                                 \<61\> = Backpressure NCBI DMA Wr requests.
                                                                 \<60\> = Backpressure NCBI ZBW requests.
                                                                 \<59\> = Backpressure NCBI CSR responses.
                                                                 \<58\> = Backpressure NCBI WIN requests.
                                                                 \<57\> = Backpressure NCBO csr requests.
                                                                 \<56\> = Backpressure EBI requests.
                                                                 \<55\> = Backpressure EBI responses.
                                                                 \<54\> = Backpressure EBO completions.
                                                                 \<53\> = Backpressure EBO posted requests.
                                                                 \<52\> = Backpressure EBO non-posted requests.
                                                                 \<51:50\> = Backpressure inbound packet drains. */
        uint64_t reserved_44_49        : 6;
        uint64_t bp_cfg                : 28; /**< [ 43: 16](R/W) Backpressure weight. For diagnostic use only.
                                                                 Internal:
                                                                 There are 2 backpressure configuration bits per enable, with the two bits
                                                                 defined as 0x0=100% of the time, 0x1=75% of the time, 0x2=50% of the time,
                                                                 0x3=25% of the time.
                                                                   \<43:42\> = Config 13.
                                                                   \<41:40\> = Config 12.
                                                                   \<39:38\> = Config 11.
                                                                   \<37:36\> = Config 10.
                                                                   \<35:34\> = Config 9.
                                                                   \<33:32\> = Config 8.
                                                                   \<31:30\> = Config 7.
                                                                   \<29:28\> = Config 6.
                                                                   \<27:26\> = Config 5.
                                                                   \<25:24\> = Config 4.
                                                                   \<23:22\> = Config 3.
                                                                   \<21:20\> = Config 2.
                                                                   \<19:18\> = Config 1.
                                                                   \<17:16\> = Config 0. */
        uint64_t reserved_12_15        : 4;
        uint64_t lfsr_freq             : 12; /**< [ 11:  0](R/W) Test LFSR update frequency in coprocessor-clocks minus one. */
#else /* Word 0 - Little Endian */
        uint64_t lfsr_freq             : 12; /**< [ 11:  0](R/W) Test LFSR update frequency in coprocessor-clocks minus one. */
        uint64_t reserved_12_15        : 4;
        uint64_t bp_cfg                : 28; /**< [ 43: 16](R/W) Backpressure weight. For diagnostic use only.
                                                                 Internal:
                                                                 There are 2 backpressure configuration bits per enable, with the two bits
                                                                 defined as 0x0=100% of the time, 0x1=75% of the time, 0x2=50% of the time,
                                                                 0x3=25% of the time.
                                                                   \<43:42\> = Config 13.
                                                                   \<41:40\> = Config 12.
                                                                   \<39:38\> = Config 11.
                                                                   \<37:36\> = Config 10.
                                                                   \<35:34\> = Config 9.
                                                                   \<33:32\> = Config 8.
                                                                   \<31:30\> = Config 7.
                                                                   \<29:28\> = Config 6.
                                                                   \<27:26\> = Config 5.
                                                                   \<25:24\> = Config 4.
                                                                   \<23:22\> = Config 3.
                                                                   \<21:20\> = Config 2.
                                                                   \<19:18\> = Config 1.
                                                                   \<17:16\> = Config 0. */
        uint64_t reserved_44_49        : 6;
        uint64_t enable                : 14; /**< [ 63: 50](R/W) Enable test mode. For diagnostic use only.
                                                                 Internal:
                                                                 Once a bit is set, random backpressure is generated
                                                                 at the corresponding point to allow for more frequent backpressure.
                                                                 \<63\> = Backpressure NCBI DMA Rd requests.
                                                                 \<62\> = Backpressure NCBI INS Rd requests.
                                                                 \<61\> = Backpressure NCBI DMA Wr requests.
                                                                 \<60\> = Backpressure NCBI ZBW requests.
                                                                 \<59\> = Backpressure NCBI CSR responses.
                                                                 \<58\> = Backpressure NCBI WIN requests.
                                                                 \<57\> = Backpressure NCBO csr requests.
                                                                 \<56\> = Backpressure EBI requests.
                                                                 \<55\> = Backpressure EBI responses.
                                                                 \<54\> = Backpressure EBO completions.
                                                                 \<53\> = Backpressure EBO posted requests.
                                                                 \<52\> = Backpressure EBO non-posted requests.
                                                                 \<51:50\> = Backpressure inbound packet drains. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_bp_test0_s cn; */
};
typedef union cavm_dpix_bp_test0 cavm_dpix_bp_test0_t;

static inline uint64_t CAVM_DPIX_BP_TEST0(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_BP_TEST0(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0000040b0ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_BP_TEST0", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_BP_TEST0(a) cavm_dpix_bp_test0_t
#define bustype_CAVM_DPIX_BP_TEST0(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_BP_TEST0(a) "DPIX_BP_TEST0"
#define device_bar_CAVM_DPIX_BP_TEST0(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_BP_TEST0(a) (a)
#define arguments_CAVM_DPIX_BP_TEST0(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_csclk_active_pc
 *
 * DPI Conditional Coprocessor Clock Counter Register
 * This register counts conditional clocks for power management.
 */
union cavm_dpix_csclk_active_pc
{
    uint64_t u;
    struct cavm_dpix_csclk_active_pc_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Count of conditional coprocessor-clock cycles since reset. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Count of conditional coprocessor-clock cycles since reset. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_csclk_active_pc_s cn; */
};
typedef union cavm_dpix_csclk_active_pc cavm_dpix_csclk_active_pc_t;

static inline uint64_t CAVM_DPIX_CSCLK_ACTIVE_PC(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_CSCLK_ACTIVE_PC(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e000004000ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_CSCLK_ACTIVE_PC", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_CSCLK_ACTIVE_PC(a) cavm_dpix_csclk_active_pc_t
#define bustype_CAVM_DPIX_CSCLK_ACTIVE_PC(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_CSCLK_ACTIVE_PC(a) "DPIX_CSCLK_ACTIVE_PC"
#define device_bar_CAVM_DPIX_CSCLK_ACTIVE_PC(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_CSCLK_ACTIVE_PC(a) (a)
#define arguments_CAVM_DPIX_CSCLK_ACTIVE_PC(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_ctl
 *
 * DPI Control Register
 * This register provides the control bits for DPI.
 */
union cavm_dpix_ctl
{
    uint64_t u;
    struct cavm_dpix_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_11_63        : 53;
        uint64_t force_ebo_clks_on     : 1;  /**< [ 10: 10](R/W) When set this will force conditional clocks in DPIs EBO bus unconditionally on.
                                                                 For diagnostic use only. */
        uint64_t force_ncb_clks_on     : 1;  /**< [  9:  9](R/W) When set this will force conditional clocks in DPIs NCB bus unconditionally on.
                                                                 For diagnostic use only. */
        uint64_t force_clks_on         : 1;  /**< [  8:  8](R/W) When set this will force conditional clocks in DPI unconditionally on. For diagnostic use only. */
        uint64_t reserved_2_7          : 6;
        uint64_t rd_psn_ign            : 1;  /**< [  1:  1](R/W) Read poison ignore.

                                                                 When [RD_PSN_IGN] is clear,
                                                                 poisons received on a NCB instruction response are fatal and set interrupt
                                                                 DPI()_REQQ()_INT[INSTR_PSN] and DPI()_PF_RAS[NCB_CMD_PSN],
                                                                 poisons received on a NCB DMA data response are propagated into RDB data
                                                                 structures and set interrupt DPI()_PF_RAS[NCB_DAT_PSN],
                                                                 and poisons returned on EBI data response are propagated into RDB data
                                                                 structures and set interrupt DPI()_PF_RAS[EBI_DAT_PSN].

                                                                 When [RD_PSN_IGN] is set,
                                                                 poisons received on a NCB instruction response are ignored and set interrupt
                                                                 DPI()_PF_RAS[NCB_CMD_PSN],
                                                                 poisons received on a NCB DMA data response are ignored and set interrupt
                                                                 DPI()_PF_RAS[NCB_DAT_PSN],
                                                                 and poisons returned on EBI data response are ignored and set interrupt DPI()_PF_RAS[EBI_DAT_PSN]. */
        uint64_t en                    : 1;  /**< [  0:  0](R/W) Turns on the DMA and packet state machines. */
#else /* Word 0 - Little Endian */
        uint64_t en                    : 1;  /**< [  0:  0](R/W) Turns on the DMA and packet state machines. */
        uint64_t rd_psn_ign            : 1;  /**< [  1:  1](R/W) Read poison ignore.

                                                                 When [RD_PSN_IGN] is clear,
                                                                 poisons received on a NCB instruction response are fatal and set interrupt
                                                                 DPI()_REQQ()_INT[INSTR_PSN] and DPI()_PF_RAS[NCB_CMD_PSN],
                                                                 poisons received on a NCB DMA data response are propagated into RDB data
                                                                 structures and set interrupt DPI()_PF_RAS[NCB_DAT_PSN],
                                                                 and poisons returned on EBI data response are propagated into RDB data
                                                                 structures and set interrupt DPI()_PF_RAS[EBI_DAT_PSN].

                                                                 When [RD_PSN_IGN] is set,
                                                                 poisons received on a NCB instruction response are ignored and set interrupt
                                                                 DPI()_PF_RAS[NCB_CMD_PSN],
                                                                 poisons received on a NCB DMA data response are ignored and set interrupt
                                                                 DPI()_PF_RAS[NCB_DAT_PSN],
                                                                 and poisons returned on EBI data response are ignored and set interrupt DPI()_PF_RAS[EBI_DAT_PSN]. */
        uint64_t reserved_2_7          : 6;
        uint64_t force_clks_on         : 1;  /**< [  8:  8](R/W) When set this will force conditional clocks in DPI unconditionally on. For diagnostic use only. */
        uint64_t force_ncb_clks_on     : 1;  /**< [  9:  9](R/W) When set this will force conditional clocks in DPIs NCB bus unconditionally on.
                                                                 For diagnostic use only. */
        uint64_t force_ebo_clks_on     : 1;  /**< [ 10: 10](R/W) When set this will force conditional clocks in DPIs EBO bus unconditionally on.
                                                                 For diagnostic use only. */
        uint64_t reserved_11_63        : 53;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_ctl_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t en                    : 1;  /**< [  0:  0](R/W) Turns on the DMA and packet state machines. */
#else /* Word 0 - Little Endian */
        uint64_t en                    : 1;  /**< [  0:  0](R/W) Turns on the DMA and packet state machines. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } cn8;
    /* struct cavm_dpix_ctl_s cn9; */
};
typedef union cavm_dpix_ctl cavm_dpix_ctl_t;

static inline uint64_t CAVM_DPIX_CTL(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_CTL(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004010ll + 0x1000000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e000004010ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_CTL", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_CTL(a) cavm_dpix_ctl_t
#define bustype_CAVM_DPIX_CTL(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_CTL(a) "DPIX_CTL"
#define device_bar_CAVM_DPIX_CTL(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_CTL(a) (a)
#define arguments_CAVM_DPIX_CTL(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_dbe_int
 *
 * DPI ECC SBE Interrupt Register
 * This register contains ECC double-bit error interrupt summary bits.
 */
union cavm_dpix_dbe_int
{
    uint64_t u;
    struct cavm_dpix_dbe_int_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t rdb_dbe               : 1;  /**< [  0:  0](R/W1C/H) Set when a double-bit error is detected in the RDB RAM. */
#else /* Word 0 - Little Endian */
        uint64_t rdb_dbe               : 1;  /**< [  0:  0](R/W1C/H) Set when a double-bit error is detected in the RDB RAM. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dbe_int_s cn; */
};
typedef union cavm_dpix_dbe_int cavm_dpix_dbe_int_t;

static inline uint64_t CAVM_DPIX_DBE_INT(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DBE_INT(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e0000041a8ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_DBE_INT", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DBE_INT(a) cavm_dpix_dbe_int_t
#define bustype_CAVM_DPIX_DBE_INT(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DBE_INT(a) "DPIX_DBE_INT"
#define device_bar_CAVM_DPIX_DBE_INT(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DBE_INT(a) (a)
#define arguments_CAVM_DPIX_DBE_INT(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_dbe_int_ena_w1c
 *
 * DPI DBE Interrupt Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_dpix_dbe_int_ena_w1c
{
    uint64_t u;
    struct cavm_dpix_dbe_int_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t rdb_dbe               : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_DBE_INT[RDB_DBE]. */
#else /* Word 0 - Little Endian */
        uint64_t rdb_dbe               : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_DBE_INT[RDB_DBE]. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dbe_int_ena_w1c_s cn; */
};
typedef union cavm_dpix_dbe_int_ena_w1c cavm_dpix_dbe_int_ena_w1c_t;

static inline uint64_t CAVM_DPIX_DBE_INT_ENA_W1C(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DBE_INT_ENA_W1C(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e0000041b8ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_DBE_INT_ENA_W1C", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DBE_INT_ENA_W1C(a) cavm_dpix_dbe_int_ena_w1c_t
#define bustype_CAVM_DPIX_DBE_INT_ENA_W1C(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DBE_INT_ENA_W1C(a) "DPIX_DBE_INT_ENA_W1C"
#define device_bar_CAVM_DPIX_DBE_INT_ENA_W1C(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DBE_INT_ENA_W1C(a) (a)
#define arguments_CAVM_DPIX_DBE_INT_ENA_W1C(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_dbe_int_ena_w1s
 *
 * DPI DBE Interrupt Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_dpix_dbe_int_ena_w1s
{
    uint64_t u;
    struct cavm_dpix_dbe_int_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t rdb_dbe               : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_DBE_INT[RDB_DBE]. */
#else /* Word 0 - Little Endian */
        uint64_t rdb_dbe               : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_DBE_INT[RDB_DBE]. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dbe_int_ena_w1s_s cn; */
};
typedef union cavm_dpix_dbe_int_ena_w1s cavm_dpix_dbe_int_ena_w1s_t;

static inline uint64_t CAVM_DPIX_DBE_INT_ENA_W1S(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DBE_INT_ENA_W1S(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e0000041c0ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_DBE_INT_ENA_W1S", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DBE_INT_ENA_W1S(a) cavm_dpix_dbe_int_ena_w1s_t
#define bustype_CAVM_DPIX_DBE_INT_ENA_W1S(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DBE_INT_ENA_W1S(a) "DPIX_DBE_INT_ENA_W1S"
#define device_bar_CAVM_DPIX_DBE_INT_ENA_W1S(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DBE_INT_ENA_W1S(a) (a)
#define arguments_CAVM_DPIX_DBE_INT_ENA_W1S(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_dbe_int_w1s
 *
 * DPI DBE Interrupt Set Register
 * This register sets interrupt bits.
 */
union cavm_dpix_dbe_int_w1s
{
    uint64_t u;
    struct cavm_dpix_dbe_int_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t rdb_dbe               : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_DBE_INT[RDB_DBE]. */
#else /* Word 0 - Little Endian */
        uint64_t rdb_dbe               : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_DBE_INT[RDB_DBE]. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dbe_int_w1s_s cn; */
};
typedef union cavm_dpix_dbe_int_w1s cavm_dpix_dbe_int_w1s_t;

static inline uint64_t CAVM_DPIX_DBE_INT_W1S(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DBE_INT_W1S(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e0000041b0ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_DBE_INT_W1S", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DBE_INT_W1S(a) cavm_dpix_dbe_int_w1s_t
#define bustype_CAVM_DPIX_DBE_INT_W1S(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DBE_INT_W1S(a) "DPIX_DBE_INT_W1S"
#define device_bar_CAVM_DPIX_DBE_INT_W1S(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DBE_INT_W1S(a) (a)
#define arguments_CAVM_DPIX_DBE_INT_W1S(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_dma#_err_rsp_status
 *
 * DPI DMA Error Response Status Register
 */
union cavm_dpix_dmax_err_rsp_status
{
    uint64_t u;
    struct cavm_dpix_dmax_err_rsp_status_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_6_63         : 58;
        uint64_t status                : 6;  /**< [  5:  0](R/W1C/H) [STATUS] captures the ErrorResponse status of the last 6 instructions for each
                                                                 instruction queue. [STATUS]\<5\> represents the status for first instruction in
                                                                 instruction order while [STATUS]\<0\> represents the last or most recent
                                                                 instruction. If [STATUS]\<n\> is set, then the Nth instruction in the given queue
                                                                 experienced an ErrorResponse. Otherwise, it completed normally. */
#else /* Word 0 - Little Endian */
        uint64_t status                : 6;  /**< [  5:  0](R/W1C/H) [STATUS] captures the ErrorResponse status of the last 6 instructions for each
                                                                 instruction queue. [STATUS]\<5\> represents the status for first instruction in
                                                                 instruction order while [STATUS]\<0\> represents the last or most recent
                                                                 instruction. If [STATUS]\<n\> is set, then the Nth instruction in the given queue
                                                                 experienced an ErrorResponse. Otherwise, it completed normally. */
        uint64_t reserved_6_63         : 58;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dmax_err_rsp_status_s cn; */
};
typedef union cavm_dpix_dmax_err_rsp_status cavm_dpix_dmax_err_rsp_status_t;

static inline uint64_t CAVM_DPIX_DMAX_ERR_RSP_STATUS(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMAX_ERR_RSP_STATUS(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e000000030ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e000000038ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_DMAX_ERR_RSP_STATUS", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMAX_ERR_RSP_STATUS(a,b) cavm_dpix_dmax_err_rsp_status_t
#define bustype_CAVM_DPIX_DMAX_ERR_RSP_STATUS(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMAX_ERR_RSP_STATUS(a,b) "DPIX_DMAX_ERR_RSP_STATUS"
#define device_bar_CAVM_DPIX_DMAX_ERR_RSP_STATUS(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMAX_ERR_RSP_STATUS(a,b) (a)
#define arguments_CAVM_DPIX_DMAX_ERR_RSP_STATUS(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma#_ibuff_csize
 *
 * DPI DMA Instruction-Buffer Chunk Size Registers
 * These registers provide configuration for each DMA instruction queue. These register
 * should only be written to when the specified queue is disabled
 * (DPI()_VDMA()_EN[QEN]).
 */
union cavm_dpix_dmax_ibuff_csize
{
    uint64_t u;
    struct cavm_dpix_dmax_ibuff_csize_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_14_63        : 50;
        uint64_t csize                 : 14; /**< [ 13:  0](R/W) The size in 8-byte words of the DMA instruction chunk. This value should only be written
                                                                 at known times in order to prevent corruption of the instruction queue. */
#else /* Word 0 - Little Endian */
        uint64_t csize                 : 14; /**< [ 13:  0](R/W) The size in 8-byte words of the DMA instruction chunk. This value should only be written
                                                                 at known times in order to prevent corruption of the instruction queue. */
        uint64_t reserved_14_63        : 50;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dmax_ibuff_csize_s cn; */
};
typedef union cavm_dpix_dmax_ibuff_csize cavm_dpix_dmax_ibuff_csize_t;

static inline uint64_t CAVM_DPIX_DMAX_IBUFF_CSIZE(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMAX_IBUFF_CSIZE(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e000000000ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e000000000ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_DMAX_IBUFF_CSIZE", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMAX_IBUFF_CSIZE(a,b) cavm_dpix_dmax_ibuff_csize_t
#define bustype_CAVM_DPIX_DMAX_IBUFF_CSIZE(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMAX_IBUFF_CSIZE(a,b) "DPIX_DMAX_IBUFF_CSIZE"
#define device_bar_CAVM_DPIX_DMAX_IBUFF_CSIZE(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMAX_IBUFF_CSIZE(a,b) (a)
#define arguments_CAVM_DPIX_DMAX_IBUFF_CSIZE(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma#_ids
 *
 * DPI DMA Request ID Registers
 * These registers configure the stream ids, PF functions and aura values for each request queue.
 */
union cavm_dpix_dmax_ids
{
    uint64_t u;
    struct cavm_dpix_dmax_ids_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_60_63        : 4;
        uint64_t inst_aura             : 12; /**< [ 59: 48](R/W) FPA guest-aura instruction chunk. The guest-aura that the instruction chunk for
                                                                 DMA operations page will be returned to when freed. */
        uint64_t inst_strm             : 8;  /**< [ 47: 40](R/W) Stream identifier bits \<7:0\> for instruction reads. Stream ID \<15:8\> is from the DPI's
                                                                 PCC bus identifier, PCC_DEV_CON_E::DPI()\<15:8\>.

                                                                 Internal:
                                                                 Stream ID \<15:8\> comes from pcc__blk_stream_id. */
        uint64_t dma_strm              : 8;  /**< [ 39: 32](R/W) Stream identifier bits \<7:0\> for DMA reads and writes. Stream ID \<15:8\> is from the DPI's
                                                                 PCC bus identifier, PCC_DEV_CON_E::DPI()\<15:8\>.

                                                                 Internal:
                                                                 Stream ID \<15:8\> comes from pcc__blk_stream_id. */
        uint64_t npa_pf_func           : 16; /**< [ 31: 16](R/W) NPA RVU physical and virtual function. Format specified by RVU_PF_FUNC_S. */
        uint64_t reserved_0_15         : 16;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_15         : 16;
        uint64_t npa_pf_func           : 16; /**< [ 31: 16](R/W) NPA RVU physical and virtual function. Format specified by RVU_PF_FUNC_S. */
        uint64_t dma_strm              : 8;  /**< [ 39: 32](R/W) Stream identifier bits \<7:0\> for DMA reads and writes. Stream ID \<15:8\> is from the DPI's
                                                                 PCC bus identifier, PCC_DEV_CON_E::DPI()\<15:8\>.

                                                                 Internal:
                                                                 Stream ID \<15:8\> comes from pcc__blk_stream_id. */
        uint64_t inst_strm             : 8;  /**< [ 47: 40](R/W) Stream identifier bits \<7:0\> for instruction reads. Stream ID \<15:8\> is from the DPI's
                                                                 PCC bus identifier, PCC_DEV_CON_E::DPI()\<15:8\>.

                                                                 Internal:
                                                                 Stream ID \<15:8\> comes from pcc__blk_stream_id. */
        uint64_t inst_aura             : 12; /**< [ 59: 48](R/W) FPA guest-aura instruction chunk. The guest-aura that the instruction chunk for
                                                                 DMA operations page will be returned to when freed. */
        uint64_t reserved_60_63        : 4;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_dmax_ids_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_60_63        : 4;
        uint64_t inst_aura             : 12; /**< [ 59: 48](R/W) FPA guest-aura instruction chunk. The guest-aura that the instruction chunk for
                                                                 DMA operations page will be returned to when freed. */
        uint64_t inst_strm             : 8;  /**< [ 47: 40](R/W) Stream identifier bits \<7:0\> for instruction reads. Stream ID \<15:8\> is from the DPI's
                                                                 PCC bus identifier, PCC_DEV_CON_E::DPI()\<15:8\>.

                                                                 Internal:
                                                                 Stream ID \<15:8\> comes from pcc__blk_stream_id. */
        uint64_t dma_strm              : 8;  /**< [ 39: 32](R/W) Stream identifier bits \<7:0\> for DMA reads and writes. Stream ID \<15:8\> is from the DPI's
                                                                 PCC bus identifier, PCC_DEV_CON_E::DPI()\<15:8\>.

                                                                 Internal:
                                                                 Stream ID \<15:8\> comes from pcc__blk_stream_id. */
        uint64_t reserved_16_31        : 16;
        uint64_t gmid                  : 16; /**< [ 15:  0](R/W) Guest machine identifier. The GMID this request queue uses for FPA buffer
                                                                 free/allocates, and for SSO add-works.
                                                                 Must be nonzero or FPA/SSO will drop requests; see FPA_PF_MAP() and SSO_PF_MAP(). */
#else /* Word 0 - Little Endian */
        uint64_t gmid                  : 16; /**< [ 15:  0](R/W) Guest machine identifier. The GMID this request queue uses for FPA buffer
                                                                 free/allocates, and for SSO add-works.
                                                                 Must be nonzero or FPA/SSO will drop requests; see FPA_PF_MAP() and SSO_PF_MAP(). */
        uint64_t reserved_16_31        : 16;
        uint64_t dma_strm              : 8;  /**< [ 39: 32](R/W) Stream identifier bits \<7:0\> for DMA reads and writes. Stream ID \<15:8\> is from the DPI's
                                                                 PCC bus identifier, PCC_DEV_CON_E::DPI()\<15:8\>.

                                                                 Internal:
                                                                 Stream ID \<15:8\> comes from pcc__blk_stream_id. */
        uint64_t inst_strm             : 8;  /**< [ 47: 40](R/W) Stream identifier bits \<7:0\> for instruction reads. Stream ID \<15:8\> is from the DPI's
                                                                 PCC bus identifier, PCC_DEV_CON_E::DPI()\<15:8\>.

                                                                 Internal:
                                                                 Stream ID \<15:8\> comes from pcc__blk_stream_id. */
        uint64_t inst_aura             : 12; /**< [ 59: 48](R/W) FPA guest-aura instruction chunk. The guest-aura that the instruction chunk for
                                                                 DMA operations page will be returned to when freed. */
        uint64_t reserved_60_63        : 4;
#endif /* Word 0 - End */
    } cn8;
    struct cavm_dpix_dmax_ids_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_48_63        : 16;
        uint64_t inst_strm             : 8;  /**< [ 47: 40](R/W) Stream identifier bits \<7:0\> for instruction reads. Stream ID \<15:8\> is from the DPI's
                                                                 PCC bus identifier, PCC_DEV_CON_E::DPI0\<15:8\>.

                                                                 Internal:
                                                                 Stream ID \<15:8\> comes from pcc__blk_stream_id. */
        uint64_t dma_strm              : 8;  /**< [ 39: 32](R/W) Stream identifier bits \<7:0\> for DMA reads and writes. Stream ID \<15:8\> is from the DPI's
                                                                 PCC bus identifier, PCC_DEV_CON_E::DPI0\<15:8\>.

                                                                 Internal:
                                                                 Stream ID \<15:8\> comes from pcc__blk_stream_id. */
        uint64_t npa_pf_func           : 16; /**< [ 31: 16](R/W) NPA RVU physical and virtual function. Format specified by RVU_PF_FUNC_S. */
        uint64_t sso_pf_func           : 16; /**< [ 15:  0](R/W) SSO RVU physical and virtual function. Indicates the RVU PF and VF to which
                                                                 this ring's SSO add works are sent. Format specified by RVU_PF_FUNC_S. */
#else /* Word 0 - Little Endian */
        uint64_t sso_pf_func           : 16; /**< [ 15:  0](R/W) SSO RVU physical and virtual function. Indicates the RVU PF and VF to which
                                                                 this ring's SSO add works are sent. Format specified by RVU_PF_FUNC_S. */
        uint64_t npa_pf_func           : 16; /**< [ 31: 16](R/W) NPA RVU physical and virtual function. Format specified by RVU_PF_FUNC_S. */
        uint64_t dma_strm              : 8;  /**< [ 39: 32](R/W) Stream identifier bits \<7:0\> for DMA reads and writes. Stream ID \<15:8\> is from the DPI's
                                                                 PCC bus identifier, PCC_DEV_CON_E::DPI0\<15:8\>.

                                                                 Internal:
                                                                 Stream ID \<15:8\> comes from pcc__blk_stream_id. */
        uint64_t inst_strm             : 8;  /**< [ 47: 40](R/W) Stream identifier bits \<7:0\> for instruction reads. Stream ID \<15:8\> is from the DPI's
                                                                 PCC bus identifier, PCC_DEV_CON_E::DPI0\<15:8\>.

                                                                 Internal:
                                                                 Stream ID \<15:8\> comes from pcc__blk_stream_id. */
        uint64_t reserved_48_63        : 16;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_dpix_dmax_ids cavm_dpix_dmax_ids_t;

static inline uint64_t CAVM_DPIX_DMAX_IDS(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMAX_IDS(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e000000018ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e000000018ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_DMAX_IDS", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMAX_IDS(a,b) cavm_dpix_dmax_ids_t
#define bustype_CAVM_DPIX_DMAX_IDS(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMAX_IDS(a,b) "DPIX_DMAX_IDS"
#define device_bar_CAVM_DPIX_DMAX_IDS(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMAX_IDS(a,b) (a)
#define arguments_CAVM_DPIX_DMAX_IDS(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma#_ids2
 *
 * DPI DMA Request ID 2 Registers
 * These registers configure the NPA auras for each request queue.
 */
union cavm_dpix_dmax_ids2
{
    uint64_t u;
    struct cavm_dpix_dmax_ids2_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_20_63        : 44;
        uint64_t inst_aura             : 20; /**< [ 19:  0](R/W) Instruction NPA aura. The NPA guest-aura that the instruction chunk for
                                                                 DMA operations page will be returned to when freed. */
#else /* Word 0 - Little Endian */
        uint64_t inst_aura             : 20; /**< [ 19:  0](R/W) Instruction NPA aura. The NPA guest-aura that the instruction chunk for
                                                                 DMA operations page will be returned to when freed. */
        uint64_t reserved_20_63        : 44;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dmax_ids2_s cn; */
};
typedef union cavm_dpix_dmax_ids2 cavm_dpix_dmax_ids2_t;

static inline uint64_t CAVM_DPIX_DMAX_IDS2(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMAX_IDS2(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e000000020ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_DMAX_IDS2", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMAX_IDS2(a,b) cavm_dpix_dmax_ids2_t
#define bustype_CAVM_DPIX_DMAX_IDS2(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMAX_IDS2(a,b) "DPIX_DMAX_IDS2"
#define device_bar_CAVM_DPIX_DMAX_IDS2(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMAX_IDS2(a,b) (a)
#define arguments_CAVM_DPIX_DMAX_IDS2(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma#_iflight
 *
 * DPI DMA In-Flight Register
 */
union cavm_dpix_dmax_iflight
{
    uint64_t u;
    struct cavm_dpix_dmax_iflight_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_3_63         : 61;
        uint64_t cnt                   : 3;  /**< [  2:  0](R/W) The number of instructions from a given queue that can be in flight to the DMA engines at
                                                                 a time. Reset value matches the number of DMA engines. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 3;  /**< [  2:  0](R/W) The number of instructions from a given queue that can be in flight to the DMA engines at
                                                                 a time. Reset value matches the number of DMA engines. */
        uint64_t reserved_3_63         : 61;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dmax_iflight_s cn; */
};
typedef union cavm_dpix_dmax_iflight cavm_dpix_dmax_iflight_t;

static inline uint64_t CAVM_DPIX_DMAX_IFLIGHT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMAX_IFLIGHT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e000000020ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e000000028ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_DMAX_IFLIGHT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMAX_IFLIGHT(a,b) cavm_dpix_dmax_iflight_t
#define bustype_CAVM_DPIX_DMAX_IFLIGHT(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMAX_IFLIGHT(a,b) "DPIX_DMAX_IFLIGHT"
#define device_bar_CAVM_DPIX_DMAX_IFLIGHT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMAX_IFLIGHT(a,b) (a)
#define arguments_CAVM_DPIX_DMAX_IFLIGHT(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma#_qrst
 *
 * DPI DMA Instruction Queue Reset Register
 */
union cavm_dpix_dmax_qrst
{
    uint64_t u;
    struct cavm_dpix_dmax_qrst_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t qrst                  : 1;  /**< [  0:  0](R/W1/H) Resets the instruction queue to clear out the local cache, reset the pointer
                                                                 inside the chunk, and clear the doorbell count. Has no effect unless the
                                                                 corresponding DPI()_DMA_ENG()_EN[QEN]=0 and DPI()_VDMA()_SADDR[IDLE]=1. */
#else /* Word 0 - Little Endian */
        uint64_t qrst                  : 1;  /**< [  0:  0](R/W1/H) Resets the instruction queue to clear out the local cache, reset the pointer
                                                                 inside the chunk, and clear the doorbell count. Has no effect unless the
                                                                 corresponding DPI()_DMA_ENG()_EN[QEN]=0 and DPI()_VDMA()_SADDR[IDLE]=1. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dmax_qrst_s cn; */
};
typedef union cavm_dpix_dmax_qrst cavm_dpix_dmax_qrst_t;

static inline uint64_t CAVM_DPIX_DMAX_QRST(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMAX_QRST(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e000000028ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e000000030ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_DMAX_QRST", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMAX_QRST(a,b) cavm_dpix_dmax_qrst_t
#define bustype_CAVM_DPIX_DMAX_QRST(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMAX_QRST(a,b) "DPIX_DMAX_QRST"
#define device_bar_CAVM_DPIX_DMAX_QRST(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMAX_QRST(a,b) (a)
#define arguments_CAVM_DPIX_DMAX_QRST(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma#_reqbnk0
 *
 * DPI DMA Request State Bank 0 Registers
 * These registers provide the current contents of the request state machine, bank 0.
 */
union cavm_dpix_dmax_reqbnk0
{
    uint64_t u;
    struct cavm_dpix_dmax_reqbnk0_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t state                 : 64; /**< [ 63:  0](RO/H) State. Provides the current contents of the request state machine. */
#else /* Word 0 - Little Endian */
        uint64_t state                 : 64; /**< [ 63:  0](RO/H) State. Provides the current contents of the request state machine. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dmax_reqbnk0_s cn8; */
    struct cavm_dpix_dmax_reqbnk0_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t state                 : 64; /**< [ 63:  0](RO/H) State. Provides the current contents of the request state machine. For
                                                                 diagnostic use only. */
#else /* Word 0 - Little Endian */
        uint64_t state                 : 64; /**< [ 63:  0](RO/H) State. Provides the current contents of the request state machine. For
                                                                 diagnostic use only. */
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_dpix_dmax_reqbnk0 cavm_dpix_dmax_reqbnk0_t;

static inline uint64_t CAVM_DPIX_DMAX_REQBNK0(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMAX_REQBNK0(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e000000008ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e000000008ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_DMAX_REQBNK0", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMAX_REQBNK0(a,b) cavm_dpix_dmax_reqbnk0_t
#define bustype_CAVM_DPIX_DMAX_REQBNK0(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMAX_REQBNK0(a,b) "DPIX_DMAX_REQBNK0"
#define device_bar_CAVM_DPIX_DMAX_REQBNK0(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMAX_REQBNK0(a,b) (a)
#define arguments_CAVM_DPIX_DMAX_REQBNK0(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma#_reqbnk1
 *
 * DPI DMA Request State Bank 1 Registers
 * These registers provide the current contents of the request state machine, bank 1.
 */
union cavm_dpix_dmax_reqbnk1
{
    uint64_t u;
    struct cavm_dpix_dmax_reqbnk1_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t state                 : 64; /**< [ 63:  0](RO/H) State. Provides the current contents of the request state machine. */
#else /* Word 0 - Little Endian */
        uint64_t state                 : 64; /**< [ 63:  0](RO/H) State. Provides the current contents of the request state machine. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dmax_reqbnk1_s cn8; */
    struct cavm_dpix_dmax_reqbnk1_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t state                 : 64; /**< [ 63:  0](RO/H) State. Provides the current contents of the request state machine. For
                                                                 diagnostic use only. */
#else /* Word 0 - Little Endian */
        uint64_t state                 : 64; /**< [ 63:  0](RO/H) State. Provides the current contents of the request state machine. For
                                                                 diagnostic use only. */
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_dpix_dmax_reqbnk1 cavm_dpix_dmax_reqbnk1_t;

static inline uint64_t CAVM_DPIX_DMAX_REQBNK1(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMAX_REQBNK1(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e000000010ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e000000010ll + 0x1000000000ll * ((a) & 0x0) + 0x800ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_DMAX_REQBNK1", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMAX_REQBNK1(a,b) cavm_dpix_dmax_reqbnk1_t
#define bustype_CAVM_DPIX_DMAX_REQBNK1(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMAX_REQBNK1(a,b) "DPIX_DMAX_REQBNK1"
#define device_bar_CAVM_DPIX_DMAX_REQBNK1(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMAX_REQBNK1(a,b) (a)
#define arguments_CAVM_DPIX_DMAX_REQBNK1(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma_cc#_cnt
 *
 * DPI DMA Per-Process Instruction Completion Counter Register
 * These registers provide a per-process instruction completion counter.
 */
union cavm_dpix_dma_ccx_cnt
{
    uint64_t u;
    struct cavm_dpix_dma_ccx_cnt_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_16_63        : 48;
        uint64_t cnt                   : 16; /**< [ 15:  0](R/W/H) DPI DMA per-process instruction completion counter. DPI can increment a counter upon
                                                                 completion of a DPI DMA instruction. DPI subtracts the value written
                                                                 from [CNT] on a software write. A nonzero [CNT] asserts the corresponding
                                                                 DPI()_DMA_CC()_INT bit.

                                                                 DPI increments the [CNT] selected by DPI_DMA_INSTR_HDR_S[PTR] by one after completing
                                                                 a DPI_DMA_INSTR_HDR_S[PT]=DPI_HDR_PT_E::CNT DPI DMA instruction. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 16; /**< [ 15:  0](R/W/H) DPI DMA per-process instruction completion counter. DPI can increment a counter upon
                                                                 completion of a DPI DMA instruction. DPI subtracts the value written
                                                                 from [CNT] on a software write. A nonzero [CNT] asserts the corresponding
                                                                 DPI()_DMA_CC()_INT bit.

                                                                 DPI increments the [CNT] selected by DPI_DMA_INSTR_HDR_S[PTR] by one after completing
                                                                 a DPI_DMA_INSTR_HDR_S[PT]=DPI_HDR_PT_E::CNT DPI DMA instruction. */
        uint64_t reserved_16_63        : 48;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dma_ccx_cnt_s cn; */
};
typedef union cavm_dpix_dma_ccx_cnt cavm_dpix_dma_ccx_cnt_t;

static inline uint64_t CAVM_DPIX_DMA_CCX_CNT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMA_CCX_CNT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=63)))
        return 0x86e000006000ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3f);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=63)))
        return 0x86e000006000ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3f);
    __cavm_csr_fatal("DPIX_DMA_CCX_CNT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMA_CCX_CNT(a,b) cavm_dpix_dma_ccx_cnt_t
#define bustype_CAVM_DPIX_DMA_CCX_CNT(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMA_CCX_CNT(a,b) "DPIX_DMA_CCX_CNT"
#define device_bar_CAVM_DPIX_DMA_CCX_CNT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMA_CCX_CNT(a,b) (a)
#define arguments_CAVM_DPIX_DMA_CCX_CNT(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma_cc#_int
 *
 * DPI DMA Per-Process Instruction Completion Interrupt Register
 * This register contains per-process completion interrupt bits.
 */
union cavm_dpix_dma_ccx_int
{
    uint64_t u;
    struct cavm_dpix_dma_ccx_int_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t complete              : 1;  /**< [  0:  0](R/W1C/H) DPI DMA per-process instruction completion interrupt. See DPI()_DMA_CC()_CNT.

                                                                 If these interrupts are enabled, the DPI()_VF()_INT interrupts should not be enabled. */
#else /* Word 0 - Little Endian */
        uint64_t complete              : 1;  /**< [  0:  0](R/W1C/H) DPI DMA per-process instruction completion interrupt. See DPI()_DMA_CC()_CNT.

                                                                 If these interrupts are enabled, the DPI()_VF()_INT interrupts should not be enabled. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dma_ccx_int_s cn8; */
    struct cavm_dpix_dma_ccx_int_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t complete              : 1;  /**< [  0:  0](R/W1C/H) DPI DMA per-process instruction completion interrupt. See DPI()_DMA_CC()_CNT.

                                                                 If these interrupts are enabled, typically the DPI()_DMA_CC()_INT interrupts
                                                                 would not be used and should not be enabled. */
#else /* Word 0 - Little Endian */
        uint64_t complete              : 1;  /**< [  0:  0](R/W1C/H) DPI DMA per-process instruction completion interrupt. See DPI()_DMA_CC()_CNT.

                                                                 If these interrupts are enabled, typically the DPI()_DMA_CC()_INT interrupts
                                                                 would not be used and should not be enabled. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_dpix_dma_ccx_int cavm_dpix_dma_ccx_int_t;

static inline uint64_t CAVM_DPIX_DMA_CCX_INT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMA_CCX_INT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=63)))
        return 0x86e000005000ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3f);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=63)))
        return 0x86e000005000ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3f);
    __cavm_csr_fatal("DPIX_DMA_CCX_INT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMA_CCX_INT(a,b) cavm_dpix_dma_ccx_int_t
#define bustype_CAVM_DPIX_DMA_CCX_INT(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMA_CCX_INT(a,b) "DPIX_DMA_CCX_INT"
#define device_bar_CAVM_DPIX_DMA_CCX_INT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMA_CCX_INT(a,b) (a)
#define arguments_CAVM_DPIX_DMA_CCX_INT(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma_cc#_int_ena_w1c
 *
 * DPI DMA Per-Process Instruction Completion Interrupt Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_dpix_dma_ccx_int_ena_w1c
{
    uint64_t u;
    struct cavm_dpix_dma_ccx_int_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t complete              : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_DMA_CC(0..63)_INT[COMPLETE]. */
#else /* Word 0 - Little Endian */
        uint64_t complete              : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_DMA_CC(0..63)_INT[COMPLETE]. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dma_ccx_int_ena_w1c_s cn; */
};
typedef union cavm_dpix_dma_ccx_int_ena_w1c cavm_dpix_dma_ccx_int_ena_w1c_t;

static inline uint64_t CAVM_DPIX_DMA_CCX_INT_ENA_W1C(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMA_CCX_INT_ENA_W1C(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=63)))
        return 0x86e000005800ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3f);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=63)))
        return 0x86e000005800ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3f);
    __cavm_csr_fatal("DPIX_DMA_CCX_INT_ENA_W1C", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMA_CCX_INT_ENA_W1C(a,b) cavm_dpix_dma_ccx_int_ena_w1c_t
#define bustype_CAVM_DPIX_DMA_CCX_INT_ENA_W1C(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMA_CCX_INT_ENA_W1C(a,b) "DPIX_DMA_CCX_INT_ENA_W1C"
#define device_bar_CAVM_DPIX_DMA_CCX_INT_ENA_W1C(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMA_CCX_INT_ENA_W1C(a,b) (a)
#define arguments_CAVM_DPIX_DMA_CCX_INT_ENA_W1C(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma_cc#_int_ena_w1s
 *
 * DPI DMA Per-Process Instruction Completion Interrupt Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_dpix_dma_ccx_int_ena_w1s
{
    uint64_t u;
    struct cavm_dpix_dma_ccx_int_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t complete              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_DMA_CC(0..63)_INT[COMPLETE]. */
#else /* Word 0 - Little Endian */
        uint64_t complete              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_DMA_CC(0..63)_INT[COMPLETE]. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dma_ccx_int_ena_w1s_s cn; */
};
typedef union cavm_dpix_dma_ccx_int_ena_w1s cavm_dpix_dma_ccx_int_ena_w1s_t;

static inline uint64_t CAVM_DPIX_DMA_CCX_INT_ENA_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMA_CCX_INT_ENA_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=63)))
        return 0x86e000005c00ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3f);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=63)))
        return 0x86e000005c00ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3f);
    __cavm_csr_fatal("DPIX_DMA_CCX_INT_ENA_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMA_CCX_INT_ENA_W1S(a,b) cavm_dpix_dma_ccx_int_ena_w1s_t
#define bustype_CAVM_DPIX_DMA_CCX_INT_ENA_W1S(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMA_CCX_INT_ENA_W1S(a,b) "DPIX_DMA_CCX_INT_ENA_W1S"
#define device_bar_CAVM_DPIX_DMA_CCX_INT_ENA_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMA_CCX_INT_ENA_W1S(a,b) (a)
#define arguments_CAVM_DPIX_DMA_CCX_INT_ENA_W1S(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma_cc#_int_w1s
 *
 * DPI DMA Per-Process Instruction Completion Interrupt Set Register
 * This register sets interrupt bits.
 */
union cavm_dpix_dma_ccx_int_w1s
{
    uint64_t u;
    struct cavm_dpix_dma_ccx_int_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t complete              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_DMA_CC(0..63)_INT[COMPLETE]. */
#else /* Word 0 - Little Endian */
        uint64_t complete              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_DMA_CC(0..63)_INT[COMPLETE]. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dma_ccx_int_w1s_s cn; */
};
typedef union cavm_dpix_dma_ccx_int_w1s cavm_dpix_dma_ccx_int_w1s_t;

static inline uint64_t CAVM_DPIX_DMA_CCX_INT_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMA_CCX_INT_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=63)))
        return 0x86e000005400ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3f);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=63)))
        return 0x86e000005400ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3f);
    __cavm_csr_fatal("DPIX_DMA_CCX_INT_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMA_CCX_INT_W1S(a,b) cavm_dpix_dma_ccx_int_w1s_t
#define bustype_CAVM_DPIX_DMA_CCX_INT_W1S(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMA_CCX_INT_W1S(a,b) "DPIX_DMA_CCX_INT_W1S"
#define device_bar_CAVM_DPIX_DMA_CCX_INT_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMA_CCX_INT_W1S(a,b) (a)
#define arguments_CAVM_DPIX_DMA_CCX_INT_W1S(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_dma_control
 *
 * DPI DMA Control Register
 * This register controls the operation of DMA input and output.
 */
union cavm_dpix_dma_control
{
    uint64_t u;
    struct cavm_dpix_dma_control_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_60_63        : 4;
        uint64_t ffp_dis               : 1;  /**< [ 59: 59](R/W) Force forward progress disable. The DMA engines will compete for shared resources. If the
                                                                 hardware detects that particular engines are not able to make requests to an interface,
                                                                 the hardware will periodically trade-off throughput for fairness. */
        uint64_t reserved_57_58        : 2;
        uint64_t pkt_en                : 1;  /**< [ 56: 56](R/W) Enables the packet interface. When the packet interface is enabled, engines 4 and 5 are
                                                                 used for packets and are not available for DMA. When PKT_EN=1, then DMA_ENB\<5\>=0 and
                                                                 DMA_ENB\<4\>=0. */
        uint64_t uo_dis                : 1;  /**< [ 55: 55](R/W) Disables the use of the unordered mode for SLI packet reads */
        uint64_t reserved_54           : 1;
        uint64_t dma_enb               : 6;  /**< [ 53: 48](R/W) DMA engine enable. Enables the operation of the DMA engine. After being enabled an engine
                                                                 should not be disabled while processing instructions.
                                                                 When PKT_EN=1, then DMA_ENB\<5\>=0 and DMA_ENB\<4\>=0. */
        uint64_t wqecsdis              : 1;  /**< [ 47: 47](R/W) Work queue completion status disable. See DPI_HDR_PT_WQP_E.

                                                                 When [WQECSDIS] is set, DPI never writes completion status into a work queue entry. */
        uint64_t wqecsoff              : 7;  /**< [ 46: 40](R/W) Work queue completion status byte offset. For a DPI_HDR_PT_WQP_E::STATUSCA
                                                                 or DPI_HDR_PT_WQP_E::STATUSNC DPI DMA instruction, DPI writes a
                                                                 non-DPI_CS_E::NOERR (i.e. nonzero) completion status byte to (big-endian
                                                                 byte address) L2/DRAM address
                                                                    (DPI_DMA_INSTR_HDR_S[PTR] & 0xFFFFFFFFFFFFFFF8) + [WQECSOFF]

                                                                 With the reset value 0x7, DPI will write WORD0\<7:0\> of the WQE. */
        uint64_t zbwcsen               : 1;  /**< [ 39: 39](R/W) Zero-byte-write completion status enable.
                                                                 See DPI_HDR_PT_E::ZBC_CA and DPI_HDR_PT_E::ZBC_NC. */
        uint64_t wqecsmode             : 2;  /**< [ 38: 37](R/W) WQE completion status mode. Relevant for DPI DMA instructions with
                                                                 DPI_DMA_INSTR_HDR_S[PT]=DPI_HDR_PT_E::WQP when [WQECSDIS]=0.
                                                                 0x0 = Normal behavior. DPI will not write the completion status byte for
                                                                       DPI_HDR_PT_E::WQP DPI DMA instructions with DPI_CS_E::NOERR (i.e. zero)
                                                                       completion status, regardless of the DPI_HDR_PT_WQP_E selection of
                                                                       DPI_DMA_INSTR_HDR_S[PTR\<2:0\>]. DPI will write the completion
                                                                       status byte for all other DPI_CS_E (i.e. nonzero) values
                                                                       when DPI_DMA_INSTR_HDR_S[PTR\<2:0\>] is DPI_HDR_PT_WQP_E::STATUSCA
                                                                       or DPI_HDR_PT_WQP_E::STATUSNC and [WQECSDIS] is clear.
                                                                 0x1 = DPI will perform the completion status byte write for all
                                                                       DPI_HDR_PT_E::WQP DPI DMA instructions when DPI_DMA_INSTR_HDR_S[PTR\<2:0\>]
                                                                       is DPI_HDR_PT_WQP_E::STATUSCA or DPI_HDR_PT_WQP_E::STATUSNC
                                                                       and [WQECSDIS] is clear, regardless of the DPI_CS_E completion
                                                                       status value for the instruction.
                                                                 0x2 = DPI will not wait for the completion status write commit before issuing
                                                                       SSO work queue add.
                                                                 0x3 = Both debug modes specified above (under 0x1 and 0x2) are enabled. */
        uint64_t reserved_35_36        : 2;
        uint64_t ncb_tag_dis           : 1;  /**< [ 34: 34](R/W) NCB tag disable. DMA read/write transactions over NCB are mapped to individual
                                                                 request queues by using tags. If this bit is set, all NCB DMA transactions will
                                                                 use a tag of 0x0, reducing performance. Additionally, any NCB faults will be
                                                                 reported on request queue 0. */
        uint64_t reserved_33           : 1;
        uint64_t ldwb                  : 1;  /**< [ 32: 32](R/W) Load-don't-write-back. When set, the hardware is able to issue LDWB commands for pointers
                                                                 that are being freed. As a result, the line will not be written back when replaced.
                                                                 When clear, the hardware issues regular load commands to the cache which cause the
                                                                 line to be written back before being replaced. */
        uint64_t reserved_20_31        : 12;
        uint64_t o_add1                : 1;  /**< [ 19: 19](R/W) Add one.
                                                                 0 = Add the number of bytes in the DMA transfer to SLI()_EPF()_DMA_CNT(()[CNT].
                                                                 1 = Add 1 to SLI()_EPF()_DMA_CNT()[CNT]. */
        uint64_t o_ro                  : 1;  /**< [ 18: 18](R/W) If [O_MODE]=1 (DPTR format 0), [O_RO] is the relaxed ordering mode attribute
                                                                 for PCIe DMA transactions.

                                                                 If [O_MODE]=0 (DPTR format 1), [O_RO] is MACADD\<60\> in the PCIE MAC address and
                                                                 the relaxed ordering mode attribute comes from DPTR\<60\> in the DMA MAC pointer. */
        uint64_t o_ns                  : 1;  /**< [ 17: 17](R/W) If [O_MODE]=1 (DPTR format 0), [O_NS] is the no snoop attribute for PCIe DMA
                                                                 transactions.

                                                                 If [O_MODE]=0 (DPTR format 1), [O_NS] is MACADD\<61\> in the PCIE MAC Address and
                                                                 the no snoop mode attribute comes from DPTR\<61\> in the DMA MAC pointer. */
        uint64_t o_es                  : 2;  /**< [ 16: 15](R/W) If [O_MODE]=1 (DPTR format 0), [O_ES] is the endian swap mode for PCIe DMA
                                                                 transactions.

                                                                 If [O_MODE]=0 (DPTR format 1), [O_ES] is MACADD\<63:62\> in the PCIE MAC address
                                                                 and the endian swap mode comes from DPTR\<63:62\> in the DMA MAC pointer.

                                                                 See DPI_ENDIANSWAP_E. */
        uint64_t o_mode                : 1;  /**< [ 14: 14](R/W) Select PCI_POINTER mode.
                                                                 0 = DPTR format 1 is used. Use register values for address; use pointer values for ES, NS,
                                                                 RO.
                                                                 1 = DPTR format 0 is used. Use pointer values for address; use register values for ES, NS,
                                                                 RO. */
        uint64_t reserved_0_13         : 14;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_13         : 14;
        uint64_t o_mode                : 1;  /**< [ 14: 14](R/W) Select PCI_POINTER mode.
                                                                 0 = DPTR format 1 is used. Use register values for address; use pointer values for ES, NS,
                                                                 RO.
                                                                 1 = DPTR format 0 is used. Use pointer values for address; use register values for ES, NS,
                                                                 RO. */
        uint64_t o_es                  : 2;  /**< [ 16: 15](R/W) If [O_MODE]=1 (DPTR format 0), [O_ES] is the endian swap mode for PCIe DMA
                                                                 transactions.

                                                                 If [O_MODE]=0 (DPTR format 1), [O_ES] is MACADD\<63:62\> in the PCIE MAC address
                                                                 and the endian swap mode comes from DPTR\<63:62\> in the DMA MAC pointer.

                                                                 See DPI_ENDIANSWAP_E. */
        uint64_t o_ns                  : 1;  /**< [ 17: 17](R/W) If [O_MODE]=1 (DPTR format 0), [O_NS] is the no snoop attribute for PCIe DMA
                                                                 transactions.

                                                                 If [O_MODE]=0 (DPTR format 1), [O_NS] is MACADD\<61\> in the PCIE MAC Address and
                                                                 the no snoop mode attribute comes from DPTR\<61\> in the DMA MAC pointer. */
        uint64_t o_ro                  : 1;  /**< [ 18: 18](R/W) If [O_MODE]=1 (DPTR format 0), [O_RO] is the relaxed ordering mode attribute
                                                                 for PCIe DMA transactions.

                                                                 If [O_MODE]=0 (DPTR format 1), [O_RO] is MACADD\<60\> in the PCIE MAC address and
                                                                 the relaxed ordering mode attribute comes from DPTR\<60\> in the DMA MAC pointer. */
        uint64_t o_add1                : 1;  /**< [ 19: 19](R/W) Add one.
                                                                 0 = Add the number of bytes in the DMA transfer to SLI()_EPF()_DMA_CNT(()[CNT].
                                                                 1 = Add 1 to SLI()_EPF()_DMA_CNT()[CNT]. */
        uint64_t reserved_20_31        : 12;
        uint64_t ldwb                  : 1;  /**< [ 32: 32](R/W) Load-don't-write-back. When set, the hardware is able to issue LDWB commands for pointers
                                                                 that are being freed. As a result, the line will not be written back when replaced.
                                                                 When clear, the hardware issues regular load commands to the cache which cause the
                                                                 line to be written back before being replaced. */
        uint64_t reserved_33           : 1;
        uint64_t ncb_tag_dis           : 1;  /**< [ 34: 34](R/W) NCB tag disable. DMA read/write transactions over NCB are mapped to individual
                                                                 request queues by using tags. If this bit is set, all NCB DMA transactions will
                                                                 use a tag of 0x0, reducing performance. Additionally, any NCB faults will be
                                                                 reported on request queue 0. */
        uint64_t reserved_35_36        : 2;
        uint64_t wqecsmode             : 2;  /**< [ 38: 37](R/W) WQE completion status mode. Relevant for DPI DMA instructions with
                                                                 DPI_DMA_INSTR_HDR_S[PT]=DPI_HDR_PT_E::WQP when [WQECSDIS]=0.
                                                                 0x0 = Normal behavior. DPI will not write the completion status byte for
                                                                       DPI_HDR_PT_E::WQP DPI DMA instructions with DPI_CS_E::NOERR (i.e. zero)
                                                                       completion status, regardless of the DPI_HDR_PT_WQP_E selection of
                                                                       DPI_DMA_INSTR_HDR_S[PTR\<2:0\>]. DPI will write the completion
                                                                       status byte for all other DPI_CS_E (i.e. nonzero) values
                                                                       when DPI_DMA_INSTR_HDR_S[PTR\<2:0\>] is DPI_HDR_PT_WQP_E::STATUSCA
                                                                       or DPI_HDR_PT_WQP_E::STATUSNC and [WQECSDIS] is clear.
                                                                 0x1 = DPI will perform the completion status byte write for all
                                                                       DPI_HDR_PT_E::WQP DPI DMA instructions when DPI_DMA_INSTR_HDR_S[PTR\<2:0\>]
                                                                       is DPI_HDR_PT_WQP_E::STATUSCA or DPI_HDR_PT_WQP_E::STATUSNC
                                                                       and [WQECSDIS] is clear, regardless of the DPI_CS_E completion
                                                                       status value for the instruction.
                                                                 0x2 = DPI will not wait for the completion status write commit before issuing
                                                                       SSO work queue add.
                                                                 0x3 = Both debug modes specified above (under 0x1 and 0x2) are enabled. */
        uint64_t zbwcsen               : 1;  /**< [ 39: 39](R/W) Zero-byte-write completion status enable.
                                                                 See DPI_HDR_PT_E::ZBC_CA and DPI_HDR_PT_E::ZBC_NC. */
        uint64_t wqecsoff              : 7;  /**< [ 46: 40](R/W) Work queue completion status byte offset. For a DPI_HDR_PT_WQP_E::STATUSCA
                                                                 or DPI_HDR_PT_WQP_E::STATUSNC DPI DMA instruction, DPI writes a
                                                                 non-DPI_CS_E::NOERR (i.e. nonzero) completion status byte to (big-endian
                                                                 byte address) L2/DRAM address
                                                                    (DPI_DMA_INSTR_HDR_S[PTR] & 0xFFFFFFFFFFFFFFF8) + [WQECSOFF]

                                                                 With the reset value 0x7, DPI will write WORD0\<7:0\> of the WQE. */
        uint64_t wqecsdis              : 1;  /**< [ 47: 47](R/W) Work queue completion status disable. See DPI_HDR_PT_WQP_E.

                                                                 When [WQECSDIS] is set, DPI never writes completion status into a work queue entry. */
        uint64_t dma_enb               : 6;  /**< [ 53: 48](R/W) DMA engine enable. Enables the operation of the DMA engine. After being enabled an engine
                                                                 should not be disabled while processing instructions.
                                                                 When PKT_EN=1, then DMA_ENB\<5\>=0 and DMA_ENB\<4\>=0. */
        uint64_t reserved_54           : 1;
        uint64_t uo_dis                : 1;  /**< [ 55: 55](R/W) Disables the use of the unordered mode for SLI packet reads */
        uint64_t pkt_en                : 1;  /**< [ 56: 56](R/W) Enables the packet interface. When the packet interface is enabled, engines 4 and 5 are
                                                                 used for packets and are not available for DMA. When PKT_EN=1, then DMA_ENB\<5\>=0 and
                                                                 DMA_ENB\<4\>=0. */
        uint64_t reserved_57_58        : 2;
        uint64_t ffp_dis               : 1;  /**< [ 59: 59](R/W) Force forward progress disable. The DMA engines will compete for shared resources. If the
                                                                 hardware detects that particular engines are not able to make requests to an interface,
                                                                 the hardware will periodically trade-off throughput for fairness. */
        uint64_t reserved_60_63        : 4;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_dma_control_s cn8; */
    struct cavm_dpix_dma_control_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_60_63        : 4;
        uint64_t ffp_dis               : 1;  /**< [ 59: 59](R/W) Force forward progress disable. The DMA engines will compete for shared resources. If the
                                                                 hardware detects that particular engines are not able to make requests to an interface,
                                                                 the hardware will periodically trade-off throughput for fairness. */
        uint64_t reserved_57_58        : 2;
        uint64_t pkt_en                : 1;  /**< [ 56: 56](R/W) Enables the packet interface. When the packet interface is enabled, engines 4 and 5 are
                                                                 used for packets and are not available for DMA. When [PKT_EN]=1, then [DMA_ENB]\<5\>=0 and
                                                                 [DMA_ENB]\<4\>=0. */
        uint64_t uo_dis                : 1;  /**< [ 55: 55](R/W) Disables the use of the unordered mode for EBUS packet reads */
        uint64_t reserved_54           : 1;
        uint64_t dma_enb               : 6;  /**< [ 53: 48](R/W) DMA engine enable. Enables the operation of the DMA engine. After being enabled an engine
                                                                 should not be disabled while processing instructions.
                                                                 When [PKT_EN]=1, then [DMA_ENB]\<5\>=0 and [DMA_ENB]\<4\>=0. */
        uint64_t wqecsdis              : 1;  /**< [ 47: 47](R/W) Work queue completion status disable. See DPI_HDR_PT_WQP_E.

                                                                 When [WQECSDIS] is set, DPI never writes completion status into a work queue entry. */
        uint64_t wqecsoff              : 7;  /**< [ 46: 40](R/W) Work queue completion status byte offset. For a DPI_HDR_PT_WQP_E::STATUSCA
                                                                 or DPI_HDR_PT_WQP_E::STATUSNC DPI DMA instruction, DPI writes a
                                                                 non-DPI_CS_E::NOERR (i.e. nonzero) completion status byte to (big-endian
                                                                 byte address) LLC/DRAM address
                                                                    (DPI_DMA_INSTR_HDR_S[PTR] & 0xFFFFFFFFFFFFFFF8) + [WQECSOFF]

                                                                 With the reset value 0x7, DPI will write WORD0\<7:0\> of the WQE. */
        uint64_t zbwcsen               : 1;  /**< [ 39: 39](R/W) Zero-byte-write completion status enable.
                                                                 See DPI_HDR_PT_E::ZBW_CA and DPI_HDR_PT_E::ZBW_NC. */
        uint64_t wqecsmode             : 2;  /**< [ 38: 37](R/W) WQE completion status mode. Relevant for DPI DMA instructions with
                                                                 DPI_DMA_INSTR_HDR_S[PT]=DPI_HDR_PT_E::WQP when [WQECSDIS]=0.
                                                                 0x0 = Normal behavior. DPI will not write the completion status byte for
                                                                       DPI_HDR_PT_E::WQP DPI DMA instructions with DPI_CS_E::NOERR (i.e. zero)
                                                                       completion status, regardless of the DPI_HDR_PT_WQP_E selection of
                                                                       DPI_DMA_INSTR_HDR_S[PTR]\<2:0\>. DPI will write the completion
                                                                       status byte for all other DPI_CS_E (i.e. nonzero) values
                                                                       when DPI_DMA_INSTR_HDR_S[PTR]\<2:0\> is DPI_HDR_PT_WQP_E::STATUSCA
                                                                       or DPI_HDR_PT_WQP_E::STATUSNC and [WQECSDIS] is clear.
                                                                 0x1 = DPI will perform the completion status byte write for all
                                                                       DPI_HDR_PT_E::WQP DPI DMA instructions when DPI_DMA_INSTR_HDR_S[PTR]\<2:0\>
                                                                       is DPI_HDR_PT_WQP_E::STATUSCA or DPI_HDR_PT_WQP_E::STATUSNC
                                                                       and [WQECSDIS] is clear, regardless of the DPI_CS_E completion
                                                                       status value for the instruction.
                                                                 0x2 = DPI will not wait for the completion status write commit before issuing
                                                                       SSO work queue add.
                                                                 0x3 = Both debug modes specified above (under 0x1 and 0x2) are enabled. */
        uint64_t reserved_33_36        : 4;
        uint64_t ldwb                  : 1;  /**< [ 32: 32](R/W) Load-don't-write-back. When set, the hardware is able to issue LDWB commands for pointers
                                                                 that are being freed. As a result, the line will not be written back when replaced.
                                                                 When clear, the hardware issues regular load commands to the cache which cause the
                                                                 line to be written back before being replaced. */
        uint64_t reserved_20_31        : 12;
        uint64_t o_add1                : 1;  /**< [ 19: 19](R/W) Add one.
                                                                 0 = The number of bytes in the DMA transfer is added to SDP()_EPF()_DMA_CNT().
                                                                 1 = Add 1 to the SDP()_EPF()_DMA_CNT() DMA counters. */
        uint64_t o_ro                  : 1;  /**< [ 18: 18](R/W) If [O_MODE]=1 (DPTR format 0), [O_RO] is the relaxed ordering mode attribute
                                                                 for PCIe DMA transactions.

                                                                 If [O_MODE]=0 (DPTR format 1), [O_RO] is MACADD\<60\> in the PCIe MAC address and
                                                                 the relaxed ordering mode attribute comes from DPTR\<60\> in the DMA MAC pointer. */
        uint64_t o_ns                  : 1;  /**< [ 17: 17](R/W) If [O_MODE]=1 (DPTR format 0), [O_NS] is the no snoop attribute for PCIe DMA
                                                                 transactions.

                                                                 If [O_MODE]=0 (DPTR format 1), [O_NS] is MACADD\<61\> in the PCIe MAC Address and
                                                                 the no snoop mode attribute comes from DPTR\<61\> in the DMA MAC pointer. */
        uint64_t o_es                  : 2;  /**< [ 16: 15](R/W) If [O_MODE]=1 (DPTR format 0), [O_ES] is the endian swap mode for PCIe DMA
                                                                 transactions.

                                                                 If [O_MODE]=0 (DPTR format 1), [O_ES] is MACADD\<63:62\> in the PCIe MAC address
                                                                 and the endian swap mode comes from DPTR\<63:62\> in the DMA MAC pointer.

                                                                 See DPI_ENDIANSWAP_E. */
        uint64_t o_mode                : 1;  /**< [ 14: 14](R/W) Select DPTR format mode.
                                                                 0 = DPTR format 1 is used. Use register values for address; use pointer values for ES, NS,
                                                                 RO.
                                                                 1 = DPTR format 0 is used. Use pointer values for address; use register values for ES, NS,
                                                                 RO. */
        uint64_t reserved_0_13         : 14;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_13         : 14;
        uint64_t o_mode                : 1;  /**< [ 14: 14](R/W) Select DPTR format mode.
                                                                 0 = DPTR format 1 is used. Use register values for address; use pointer values for ES, NS,
                                                                 RO.
                                                                 1 = DPTR format 0 is used. Use pointer values for address; use register values for ES, NS,
                                                                 RO. */
        uint64_t o_es                  : 2;  /**< [ 16: 15](R/W) If [O_MODE]=1 (DPTR format 0), [O_ES] is the endian swap mode for PCIe DMA
                                                                 transactions.

                                                                 If [O_MODE]=0 (DPTR format 1), [O_ES] is MACADD\<63:62\> in the PCIe MAC address
                                                                 and the endian swap mode comes from DPTR\<63:62\> in the DMA MAC pointer.

                                                                 See DPI_ENDIANSWAP_E. */
        uint64_t o_ns                  : 1;  /**< [ 17: 17](R/W) If [O_MODE]=1 (DPTR format 0), [O_NS] is the no snoop attribute for PCIe DMA
                                                                 transactions.

                                                                 If [O_MODE]=0 (DPTR format 1), [O_NS] is MACADD\<61\> in the PCIe MAC Address and
                                                                 the no snoop mode attribute comes from DPTR\<61\> in the DMA MAC pointer. */
        uint64_t o_ro                  : 1;  /**< [ 18: 18](R/W) If [O_MODE]=1 (DPTR format 0), [O_RO] is the relaxed ordering mode attribute
                                                                 for PCIe DMA transactions.

                                                                 If [O_MODE]=0 (DPTR format 1), [O_RO] is MACADD\<60\> in the PCIe MAC address and
                                                                 the relaxed ordering mode attribute comes from DPTR\<60\> in the DMA MAC pointer. */
        uint64_t o_add1                : 1;  /**< [ 19: 19](R/W) Add one.
                                                                 0 = The number of bytes in the DMA transfer is added to SDP()_EPF()_DMA_CNT().
                                                                 1 = Add 1 to the SDP()_EPF()_DMA_CNT() DMA counters. */
        uint64_t reserved_20_31        : 12;
        uint64_t ldwb                  : 1;  /**< [ 32: 32](R/W) Load-don't-write-back. When set, the hardware is able to issue LDWB commands for pointers
                                                                 that are being freed. As a result, the line will not be written back when replaced.
                                                                 When clear, the hardware issues regular load commands to the cache which cause the
                                                                 line to be written back before being replaced. */
        uint64_t reserved_33_36        : 4;
        uint64_t wqecsmode             : 2;  /**< [ 38: 37](R/W) WQE completion status mode. Relevant for DPI DMA instructions with
                                                                 DPI_DMA_INSTR_HDR_S[PT]=DPI_HDR_PT_E::WQP when [WQECSDIS]=0.
                                                                 0x0 = Normal behavior. DPI will not write the completion status byte for
                                                                       DPI_HDR_PT_E::WQP DPI DMA instructions with DPI_CS_E::NOERR (i.e. zero)
                                                                       completion status, regardless of the DPI_HDR_PT_WQP_E selection of
                                                                       DPI_DMA_INSTR_HDR_S[PTR]\<2:0\>. DPI will write the completion
                                                                       status byte for all other DPI_CS_E (i.e. nonzero) values
                                                                       when DPI_DMA_INSTR_HDR_S[PTR]\<2:0\> is DPI_HDR_PT_WQP_E::STATUSCA
                                                                       or DPI_HDR_PT_WQP_E::STATUSNC and [WQECSDIS] is clear.
                                                                 0x1 = DPI will perform the completion status byte write for all
                                                                       DPI_HDR_PT_E::WQP DPI DMA instructions when DPI_DMA_INSTR_HDR_S[PTR]\<2:0\>
                                                                       is DPI_HDR_PT_WQP_E::STATUSCA or DPI_HDR_PT_WQP_E::STATUSNC
                                                                       and [WQECSDIS] is clear, regardless of the DPI_CS_E completion
                                                                       status value for the instruction.
                                                                 0x2 = DPI will not wait for the completion status write commit before issuing
                                                                       SSO work queue add.
                                                                 0x3 = Both debug modes specified above (under 0x1 and 0x2) are enabled. */
        uint64_t zbwcsen               : 1;  /**< [ 39: 39](R/W) Zero-byte-write completion status enable.
                                                                 See DPI_HDR_PT_E::ZBW_CA and DPI_HDR_PT_E::ZBW_NC. */
        uint64_t wqecsoff              : 7;  /**< [ 46: 40](R/W) Work queue completion status byte offset. For a DPI_HDR_PT_WQP_E::STATUSCA
                                                                 or DPI_HDR_PT_WQP_E::STATUSNC DPI DMA instruction, DPI writes a
                                                                 non-DPI_CS_E::NOERR (i.e. nonzero) completion status byte to (big-endian
                                                                 byte address) LLC/DRAM address
                                                                    (DPI_DMA_INSTR_HDR_S[PTR] & 0xFFFFFFFFFFFFFFF8) + [WQECSOFF]

                                                                 With the reset value 0x7, DPI will write WORD0\<7:0\> of the WQE. */
        uint64_t wqecsdis              : 1;  /**< [ 47: 47](R/W) Work queue completion status disable. See DPI_HDR_PT_WQP_E.

                                                                 When [WQECSDIS] is set, DPI never writes completion status into a work queue entry. */
        uint64_t dma_enb               : 6;  /**< [ 53: 48](R/W) DMA engine enable. Enables the operation of the DMA engine. After being enabled an engine
                                                                 should not be disabled while processing instructions.
                                                                 When [PKT_EN]=1, then [DMA_ENB]\<5\>=0 and [DMA_ENB]\<4\>=0. */
        uint64_t reserved_54           : 1;
        uint64_t uo_dis                : 1;  /**< [ 55: 55](R/W) Disables the use of the unordered mode for EBUS packet reads */
        uint64_t pkt_en                : 1;  /**< [ 56: 56](R/W) Enables the packet interface. When the packet interface is enabled, engines 4 and 5 are
                                                                 used for packets and are not available for DMA. When [PKT_EN]=1, then [DMA_ENB]\<5\>=0 and
                                                                 [DMA_ENB]\<4\>=0. */
        uint64_t reserved_57_58        : 2;
        uint64_t ffp_dis               : 1;  /**< [ 59: 59](R/W) Force forward progress disable. The DMA engines will compete for shared resources. If the
                                                                 hardware detects that particular engines are not able to make requests to an interface,
                                                                 the hardware will periodically trade-off throughput for fairness. */
        uint64_t reserved_60_63        : 4;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_dpix_dma_control cavm_dpix_dma_control_t;

static inline uint64_t CAVM_DPIX_DMA_CONTROL(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMA_CONTROL(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004018ll + 0x1000000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e000004018ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_DMA_CONTROL", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMA_CONTROL(a) cavm_dpix_dma_control_t
#define bustype_CAVM_DPIX_DMA_CONTROL(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMA_CONTROL(a) "DPIX_DMA_CONTROL"
#define device_bar_CAVM_DPIX_DMA_CONTROL(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMA_CONTROL(a) (a)
#define arguments_CAVM_DPIX_DMA_CONTROL(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_dma_eng#_en
 *
 * DPI DMA-Engine Queue Enable Registers
 * These registers provide control for the DMA engines.
 */
union cavm_dpix_dma_engx_en
{
    uint64_t u;
    struct cavm_dpix_dma_engx_en_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_42_63        : 22;
        uint64_t eng_molr              : 10; /**< [ 41: 32](R/W) Per-engine maximum outstanding read request. 0x0 means no maximum per engine
                                                                 (although it will be still limited by the per port MOLR value.) */
        uint64_t reserved_8_31         : 24;
        uint64_t qen                   : 8;  /**< [  7:  0](R/W) Instruction queue enable. This field specifies which of eight logical instruction queues
                                                                 can be serviced by the DMA engine.
                                                                   0 = Prevent service.
                                                                   1 = Allow service.

                                                                 Setting [QEN] = 0x0 effectively disables the engine. When
                                                                 DPI()_DMA_CONTROL[PKT_EN] = 1, then DPI()_DMA_ENG()_EN[QEN] must be zero. */
#else /* Word 0 - Little Endian */
        uint64_t qen                   : 8;  /**< [  7:  0](R/W) Instruction queue enable. This field specifies which of eight logical instruction queues
                                                                 can be serviced by the DMA engine.
                                                                   0 = Prevent service.
                                                                   1 = Allow service.

                                                                 Setting [QEN] = 0x0 effectively disables the engine. When
                                                                 DPI()_DMA_CONTROL[PKT_EN] = 1, then DPI()_DMA_ENG()_EN[QEN] must be zero. */
        uint64_t reserved_8_31         : 24;
        uint64_t eng_molr              : 10; /**< [ 41: 32](R/W) Per-engine maximum outstanding read request. 0x0 means no maximum per engine
                                                                 (although it will be still limited by the per port MOLR value.) */
        uint64_t reserved_42_63        : 22;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_dma_engx_en_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_39_63        : 25;
        uint64_t eng_molr              : 7;  /**< [ 38: 32](R/W) Per-engine maximum outstanding read request. 0x0 means no maximum per engine
                                                                 (although it will be still limited by the per port MOLR value.) */
        uint64_t reserved_8_31         : 24;
        uint64_t qen                   : 8;  /**< [  7:  0](R/W) Instruction queue enable. This field specifies which of eight logical instruction queues
                                                                 can be serviced by the DMA engine.
                                                                   0 = Prevent service.
                                                                   1 = Allow service.

                                                                 Setting [QEN] = 0x0 effectively disables the engine. When
                                                                 DPI()_DMA_CONTROL[PKT_EN] = 1, then DPI()_DMA_ENG()_EN[QEN] must be zero. */
#else /* Word 0 - Little Endian */
        uint64_t qen                   : 8;  /**< [  7:  0](R/W) Instruction queue enable. This field specifies which of eight logical instruction queues
                                                                 can be serviced by the DMA engine.
                                                                   0 = Prevent service.
                                                                   1 = Allow service.

                                                                 Setting [QEN] = 0x0 effectively disables the engine. When
                                                                 DPI()_DMA_CONTROL[PKT_EN] = 1, then DPI()_DMA_ENG()_EN[QEN] must be zero. */
        uint64_t reserved_8_31         : 24;
        uint64_t eng_molr              : 7;  /**< [ 38: 32](R/W) Per-engine maximum outstanding read request. 0x0 means no maximum per engine
                                                                 (although it will be still limited by the per port MOLR value.) */
        uint64_t reserved_39_63        : 25;
#endif /* Word 0 - End */
    } cn8;
    /* struct cavm_dpix_dma_engx_en_s cn9; */
};
typedef union cavm_dpix_dma_engx_en cavm_dpix_dma_engx_en_t;

static inline uint64_t CAVM_DPIX_DMA_ENGX_EN(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_DMA_ENGX_EN(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=5)))
        return 0x86e000004040ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=5)))
        return 0x86e000004040ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_DMA_ENGX_EN", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_DMA_ENGX_EN(a,b) cavm_dpix_dma_engx_en_t
#define bustype_CAVM_DPIX_DMA_ENGX_EN(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_DMA_ENGX_EN(a,b) "DPIX_DMA_ENGX_EN"
#define device_bar_CAVM_DPIX_DMA_ENGX_EN(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_DMA_ENGX_EN(a,b) (a)
#define arguments_CAVM_DPIX_DMA_ENGX_EN(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_ebus_port#_cfg
 *
 * DPI EBus Port Configuration Register
 * This register configures the max read request size, max payload size, and max number of EBUS
 * tags in use.
 */
union cavm_dpix_ebus_portx_cfg
{
    uint64_t u;
    struct cavm_dpix_ebus_portx_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_21_63        : 43;
        uint64_t exact_rd_dis          : 1;  /**< [ 20: 20](R/W) Exact read mode disable.
                                                                 0 = DPI will issue a 32-bit read request to the PEM when the size and alignment
                                                                 ensure that the request will not cross a 32-bit boundary.
                                                                 1 = DPI will always use a minimum 64-bit read request to the PEM. */
        uint64_t reserved_18_19        : 2;
        uint64_t molr                  : 10; /**< [ 17:  8](R/W) Maximum outstanding load requests. Limits the number of outstanding load requests on the
                                                                 port by restricting the number of tags available to this port. This value
                                                                 can range from 0 to 512. The maximum number of load requests across all ports is 512.  Setting MOLR
                                                                 to a value of zero halts all read traffic to the port. There are no restrictions on when this
                                                                 value can be changed. */
        uint64_t mps_lim               : 1;  /**< [  7:  7](R/W) MAC memory space write requests cannot cross the (naturally-aligned) MPS boundary.
                                                                 When clear, DPI is allowed to issue a MAC memory- space read that crosses the naturally-
                                                                 aligned boundary of size defined by MPS. (DPI will still only cross the boundary when it
                                                                 would eliminate a write by doing so.)

                                                                 When set, DPI will never issue a MAC memory space write that crosses the naturally-aligned
                                                                 boundary of size defined by MPS. */
        uint64_t mps                   : 3;  /**< [  6:  4](R/W) Maximum payload size.
                                                                 0x0 = 128B.
                                                                 0x1 = 256B.
                                                                 0x2 = 512B.
                                                                 0x3 = 1024B.

                                                                 Larger sizes are not supported. If [MPS] = 0x2 or 0x3, outbound packet writes
                                                                 will use the 512B/1024B size but DMA is limited to 256B.

                                                                 The MPS size must not exceed the size selected by PCIE*_CFG030[MPS]. */
        uint64_t mrrs_lim              : 1;  /**< [  3:  3](R/W) MAC memory space read requests cannot cross the (naturally-aligned) MRRS boundary.

                                                                 When clear, DPI is allowed to issue a MAC memory-space read that crosses the naturally-
                                                                 aligned boundary of size defined by MRRS. (DPI will still only cross the boundary when it
                                                                 would eliminate a read by doing so.)

                                                                 When set, DPI will never issue a MAC memory space read that crosses the naturally-aligned
                                                                 boundary of size defined by MRRS. */
        uint64_t mrrs                  : 3;  /**< [  2:  0](R/W) Maximum read-request size.
                                                                 0x0 = 128B.
                                                                 0x1 = 256B.
                                                                 0x2 = 512B.
                                                                 0x3 = 1024B.

                                                                 Larger sizes are not supported.

                                                                 The MRRS size must not exceed the size selected by PCIE*_CFG030[MRRS]. */
#else /* Word 0 - Little Endian */
        uint64_t mrrs                  : 3;  /**< [  2:  0](R/W) Maximum read-request size.
                                                                 0x0 = 128B.
                                                                 0x1 = 256B.
                                                                 0x2 = 512B.
                                                                 0x3 = 1024B.

                                                                 Larger sizes are not supported.

                                                                 The MRRS size must not exceed the size selected by PCIE*_CFG030[MRRS]. */
        uint64_t mrrs_lim              : 1;  /**< [  3:  3](R/W) MAC memory space read requests cannot cross the (naturally-aligned) MRRS boundary.

                                                                 When clear, DPI is allowed to issue a MAC memory-space read that crosses the naturally-
                                                                 aligned boundary of size defined by MRRS. (DPI will still only cross the boundary when it
                                                                 would eliminate a read by doing so.)

                                                                 When set, DPI will never issue a MAC memory space read that crosses the naturally-aligned
                                                                 boundary of size defined by MRRS. */
        uint64_t mps                   : 3;  /**< [  6:  4](R/W) Maximum payload size.
                                                                 0x0 = 128B.
                                                                 0x1 = 256B.
                                                                 0x2 = 512B.
                                                                 0x3 = 1024B.

                                                                 Larger sizes are not supported. If [MPS] = 0x2 or 0x3, outbound packet writes
                                                                 will use the 512B/1024B size but DMA is limited to 256B.

                                                                 The MPS size must not exceed the size selected by PCIE*_CFG030[MPS]. */
        uint64_t mps_lim               : 1;  /**< [  7:  7](R/W) MAC memory space write requests cannot cross the (naturally-aligned) MPS boundary.
                                                                 When clear, DPI is allowed to issue a MAC memory- space read that crosses the naturally-
                                                                 aligned boundary of size defined by MPS. (DPI will still only cross the boundary when it
                                                                 would eliminate a write by doing so.)

                                                                 When set, DPI will never issue a MAC memory space write that crosses the naturally-aligned
                                                                 boundary of size defined by MPS. */
        uint64_t molr                  : 10; /**< [ 17:  8](R/W) Maximum outstanding load requests. Limits the number of outstanding load requests on the
                                                                 port by restricting the number of tags available to this port. This value
                                                                 can range from 0 to 512. The maximum number of load requests across all ports is 512.  Setting MOLR
                                                                 to a value of zero halts all read traffic to the port. There are no restrictions on when this
                                                                 value can be changed. */
        uint64_t reserved_18_19        : 2;
        uint64_t exact_rd_dis          : 1;  /**< [ 20: 20](R/W) Exact read mode disable.
                                                                 0 = DPI will issue a 32-bit read request to the PEM when the size and alignment
                                                                 ensure that the request will not cross a 32-bit boundary.
                                                                 1 = DPI will always use a minimum 64-bit read request to the PEM. */
        uint64_t reserved_21_63        : 43;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_ebus_portx_cfg_s cn; */
};
typedef union cavm_dpix_ebus_portx_cfg cavm_dpix_ebus_portx_cfg_t;

static inline uint64_t CAVM_DPIX_EBUS_PORTX_CFG(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EBUS_PORTX_CFG(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=1)))
        return 0x86e000004100ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    __cavm_csr_fatal("DPIX_EBUS_PORTX_CFG", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EBUS_PORTX_CFG(a,b) cavm_dpix_ebus_portx_cfg_t
#define bustype_CAVM_DPIX_EBUS_PORTX_CFG(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EBUS_PORTX_CFG(a,b) "DPIX_EBUS_PORTX_CFG"
#define device_bar_CAVM_DPIX_EBUS_PORTX_CFG(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EBUS_PORTX_CFG(a,b) (a)
#define arguments_CAVM_DPIX_EBUS_PORTX_CFG(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_ebus_port#_err
 *
 * DPI EBus Port Error Address Registers
 * This register logs the address associated with the reported EBus error response.
 */
union cavm_dpix_ebus_portx_err
{
    uint64_t u;
    struct cavm_dpix_ebus_portx_err_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t addr                  : 59; /**< [ 63:  5](RO/H) Address of the failed load request. Address is locked along with the
                                                                 DPI()_EBUS_PORT()_ERR_INFO register. See the
                                                                 DPI()_EBUS_PORT()_ERR_INFO[LOCK] description for further information. */
        uint64_t reserved_0_4          : 5;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_4          : 5;
        uint64_t addr                  : 59; /**< [ 63:  5](RO/H) Address of the failed load request. Address is locked along with the
                                                                 DPI()_EBUS_PORT()_ERR_INFO register. See the
                                                                 DPI()_EBUS_PORT()_ERR_INFO[LOCK] description for further information. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_ebus_portx_err_s cn; */
};
typedef union cavm_dpix_ebus_portx_err cavm_dpix_ebus_portx_err_t;

static inline uint64_t CAVM_DPIX_EBUS_PORTX_ERR(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EBUS_PORTX_ERR(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=1)))
        return 0x86e000004280ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    __cavm_csr_fatal("DPIX_EBUS_PORTX_ERR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EBUS_PORTX_ERR(a,b) cavm_dpix_ebus_portx_err_t
#define bustype_CAVM_DPIX_EBUS_PORTX_ERR(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EBUS_PORTX_ERR(a,b) "DPIX_EBUS_PORTX_ERR"
#define device_bar_CAVM_DPIX_EBUS_PORTX_ERR(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EBUS_PORTX_ERR(a,b) (a)
#define arguments_CAVM_DPIX_EBUS_PORTX_ERR(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_ebus_port#_err_info
 *
 * DPI EBus Port Error Info Registers
 * This register logs information associated with the reported EBUS error response.
 */
union cavm_dpix_ebus_portx_err_info
{
    uint64_t u;
    struct cavm_dpix_ebus_portx_err_info_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t pvf                   : 16; /**< [ 31: 16](RO/H) Physical/virtual function that caused the ErrorResponse. DPI_DMA_FUNC_SEL_S format. */
        uint64_t reserved_9_15         : 7;
        uint64_t lock                  : 1;  /**< [  8:  8](RO/H) DPI()_EBUS_PORT()_ERR and DPI()_EBUS_PORT()_ERR_INFO have captured and locked contents.
                                                                 When CNXXXX first detects an ErrorResponse, the TYPE, REQQ, and ADDR of the error is saved
                                                                 and an internal lock state is set so the data associated with the initial error is
                                                                 preserved.

                                                                 Subsequent ErrorResponses will optionally raise an interrupt, but will not
                                                                 modify [TYPE], [REQQ], nor DPI()_EBUS_PORT()_ERR[ADDR] until the internal lock
                                                                 state is cleared.

                                                                 Software can clear the internal lock state by writing a one to the appropriate
                                                                 bit in either DPI()_REQ_ERR_RSP or DPI()_PKT_ERR_RSP depending on [TYPE]. Once
                                                                 the internal lock state is cleared, the next ErrorResponse will set [TYPE],
                                                                 [REQQ], nor DPI()_EBUS_PORT()_ERR[ADDR] for the new transaction. */
        uint64_t reserved_5_7          : 3;
        uint64_t type                  : 1;  /**< [  4:  4](RO/H) Type of transaction that caused the ErrorResponse.
                                                                 0 = DMA Instruction.
                                                                 1 = PKT Instruction. */
        uint64_t reserved_3            : 1;
        uint64_t reqq                  : 3;  /**< [  2:  0](RO/H) Request queue that made the failed load request. */
#else /* Word 0 - Little Endian */
        uint64_t reqq                  : 3;  /**< [  2:  0](RO/H) Request queue that made the failed load request. */
        uint64_t reserved_3            : 1;
        uint64_t type                  : 1;  /**< [  4:  4](RO/H) Type of transaction that caused the ErrorResponse.
                                                                 0 = DMA Instruction.
                                                                 1 = PKT Instruction. */
        uint64_t reserved_5_7          : 3;
        uint64_t lock                  : 1;  /**< [  8:  8](RO/H) DPI()_EBUS_PORT()_ERR and DPI()_EBUS_PORT()_ERR_INFO have captured and locked contents.
                                                                 When CNXXXX first detects an ErrorResponse, the TYPE, REQQ, and ADDR of the error is saved
                                                                 and an internal lock state is set so the data associated with the initial error is
                                                                 preserved.

                                                                 Subsequent ErrorResponses will optionally raise an interrupt, but will not
                                                                 modify [TYPE], [REQQ], nor DPI()_EBUS_PORT()_ERR[ADDR] until the internal lock
                                                                 state is cleared.

                                                                 Software can clear the internal lock state by writing a one to the appropriate
                                                                 bit in either DPI()_REQ_ERR_RSP or DPI()_PKT_ERR_RSP depending on [TYPE]. Once
                                                                 the internal lock state is cleared, the next ErrorResponse will set [TYPE],
                                                                 [REQQ], nor DPI()_EBUS_PORT()_ERR[ADDR] for the new transaction. */
        uint64_t reserved_9_15         : 7;
        uint64_t pvf                   : 16; /**< [ 31: 16](RO/H) Physical/virtual function that caused the ErrorResponse. DPI_DMA_FUNC_SEL_S format. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_ebus_portx_err_info_s cn; */
};
typedef union cavm_dpix_ebus_portx_err_info cavm_dpix_ebus_portx_err_info_t;

static inline uint64_t CAVM_DPIX_EBUS_PORTX_ERR_INFO(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EBUS_PORTX_ERR_INFO(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=1)))
        return 0x86e000004200ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    __cavm_csr_fatal("DPIX_EBUS_PORTX_ERR_INFO", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EBUS_PORTX_ERR_INFO(a,b) cavm_dpix_ebus_portx_err_info_t
#define bustype_CAVM_DPIX_EBUS_PORTX_ERR_INFO(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EBUS_PORTX_ERR_INFO(a,b) "DPIX_EBUS_PORTX_ERR_INFO"
#define device_bar_CAVM_DPIX_EBUS_PORTX_ERR_INFO(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EBUS_PORTX_ERR_INFO(a,b) (a)
#define arguments_CAVM_DPIX_EBUS_PORTX_ERR_INFO(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_ebus_port#_scfg
 *
 * DPI EBus Port Secure Configuration Register
 * This register configures write virtual, bar 0 disable and window diable bits for the EBUS port.
 */
union cavm_dpix_ebus_portx_scfg
{
    uint64_t u;
    struct cavm_dpix_ebus_portx_scfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_3_63         : 61;
        uint64_t wvirt                 : 1;  /**< [  2:  2](SR/W) Write virtual.
                                                                   0 = Addresses are physical addresses.
                                                                   1 = Addresses in SDP()_WIN_WR_ADDR and SDP()_WIN_RD_ADDR are virtual addresses.

                                                                   As virtual addresses allow the improved security provided by the SMMU, it is
                                                                   recommended this bit typically be set. */
        uint64_t wind_d                : 1;  /**< [  1:  1](SR/W) Window disable. When set, disables access to the window registers from the MAC. */
        uint64_t bar0_d                : 1;  /**< [  0:  0](SR/W) BAR0 disable. When set, disables access from the MAC to SDP BAR0 registers. */
#else /* Word 0 - Little Endian */
        uint64_t bar0_d                : 1;  /**< [  0:  0](SR/W) BAR0 disable. When set, disables access from the MAC to SDP BAR0 registers. */
        uint64_t wind_d                : 1;  /**< [  1:  1](SR/W) Window disable. When set, disables access to the window registers from the MAC. */
        uint64_t wvirt                 : 1;  /**< [  2:  2](SR/W) Write virtual.
                                                                   0 = Addresses are physical addresses.
                                                                   1 = Addresses in SDP()_WIN_WR_ADDR and SDP()_WIN_RD_ADDR are virtual addresses.

                                                                   As virtual addresses allow the improved security provided by the SMMU, it is
                                                                   recommended this bit typically be set. */
        uint64_t reserved_3_63         : 61;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_ebus_portx_scfg_s cn; */
};
typedef union cavm_dpix_ebus_portx_scfg cavm_dpix_ebus_portx_scfg_t;

static inline uint64_t CAVM_DPIX_EBUS_PORTX_SCFG(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EBUS_PORTX_SCFG(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=1)))
        return 0x86e000004180ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    __cavm_csr_fatal("DPIX_EBUS_PORTX_SCFG", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EBUS_PORTX_SCFG(a,b) cavm_dpix_ebus_portx_scfg_t
#define bustype_CAVM_DPIX_EBUS_PORTX_SCFG(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EBUS_PORTX_SCFG(a,b) "DPIX_EBUS_PORTX_SCFG"
#define device_bar_CAVM_DPIX_EBUS_PORTX_SCFG(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EBUS_PORTX_SCFG(a,b) (a)
#define arguments_CAVM_DPIX_EBUS_PORTX_SCFG(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_ebus_recal
 *
 * DPI EBus Port Recalibration Register
 * This register is used for triggering EBus EBI recalibration.
 */
union cavm_dpix_ebus_recal
{
    uint64_t u;
    struct cavm_dpix_ebus_recal_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t ebus_recal            : 1;  /**< [  0:  0](WO) A write of 1 will trigger an EBI recalibration. For diagnostic purposes only. */
#else /* Word 0 - Little Endian */
        uint64_t ebus_recal            : 1;  /**< [  0:  0](WO) A write of 1 will trigger an EBI recalibration. For diagnostic purposes only. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_ebus_recal_s cn; */
};
typedef union cavm_dpix_ebus_recal cavm_dpix_ebus_recal_t;

static inline uint64_t CAVM_DPIX_EBUS_RECAL(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EBUS_RECAL(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0000040f0ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_EBUS_RECAL", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EBUS_RECAL(a) cavm_dpix_ebus_recal_t
#define bustype_CAVM_DPIX_EBUS_RECAL(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EBUS_RECAL(a) "DPIX_EBUS_RECAL"
#define device_bar_CAVM_DPIX_EBUS_RECAL(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EBUS_RECAL(a) (a)
#define arguments_CAVM_DPIX_EBUS_RECAL(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_ecc_ctl
 *
 * DPI ECC Control Register
 * This register allows inserting ECC errors for testing.
 */
union cavm_dpix_ecc_ctl
{
    uint64_t u;
    struct cavm_dpix_ecc_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_33_63        : 31;
        uint64_t ram_cdis              : 1;  /**< [ 32: 32](R/W) RDB RAM ECC correction disable. Each bit corresponds to a different RAM. */
        uint64_t reserved_17_31        : 15;
        uint64_t ram_flip1             : 1;  /**< [ 16: 16](R/W) Flip syndrome bits on write. Flip syndrome bits \<1\> on writes to the corresponding RDB RAM
                                                                 to test single-bit or double-bit error handling. */
        uint64_t reserved_1_15         : 15;
        uint64_t ram_flip0             : 1;  /**< [  0:  0](R/W) Flip syndrome bits on write. Flip syndrome bits \<0\> on writes to the corresponding RDB RAM
                                                                 to test single-bit or double-bit error handling. */
#else /* Word 0 - Little Endian */
        uint64_t ram_flip0             : 1;  /**< [  0:  0](R/W) Flip syndrome bits on write. Flip syndrome bits \<0\> on writes to the corresponding RDB RAM
                                                                 to test single-bit or double-bit error handling. */
        uint64_t reserved_1_15         : 15;
        uint64_t ram_flip1             : 1;  /**< [ 16: 16](R/W) Flip syndrome bits on write. Flip syndrome bits \<1\> on writes to the corresponding RDB RAM
                                                                 to test single-bit or double-bit error handling. */
        uint64_t reserved_17_31        : 15;
        uint64_t ram_cdis              : 1;  /**< [ 32: 32](R/W) RDB RAM ECC correction disable. Each bit corresponds to a different RAM. */
        uint64_t reserved_33_63        : 31;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_ecc_ctl_s cn; */
};
typedef union cavm_dpix_ecc_ctl cavm_dpix_ecc_ctl_t;

static inline uint64_t CAVM_DPIX_ECC_CTL(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_ECC_CTL(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004008ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_ECC_CTL", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_ECC_CTL(a) cavm_dpix_ecc_ctl_t
#define bustype_CAVM_DPIX_ECC_CTL(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_ECC_CTL(a) "DPIX_ECC_CTL"
#define device_bar_CAVM_DPIX_ECC_CTL(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_ECC_CTL(a) (a)
#define arguments_CAVM_DPIX_ECC_CTL(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_eng#_buf
 *
 * DPI DMA Engine Buffer Registers
 */
union cavm_dpix_engx_buf
{
    uint64_t u;
    struct cavm_dpix_engx_buf_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_22_63        : 42;
        uint64_t base                  : 6;  /**< [ 21: 16](RO/H) The base address in 512-byte blocks of the DMA engine FIFO. */
        uint64_t reserved_6_15         : 10;
        uint64_t blks                  : 6;  /**< [  5:  0](R/W) The size of the DMA engine FIFO. The sum of the allocated FIFOs across all six
                                                                 DPI()_ENG()_BUF[BLKS] registers must not exceed the overall RDB memory size of
                                                                 32 KB. If the value of 0 is used, the corresponding engine enable in
                                                                 DPI()_DMA_CONTROL[DMA_ENB]
                                                                 must also be cleared.

                                                                 0x00 = Engine disabled.
                                                                 0x01 = 1 KB FIFO.
                                                                 0x02 = 2 KB FIFO.
                                                                 0x03 = 3 KB FIFO.
                                                                 0x04 = 4 KB FIFO.
                                                                 0x05 = 5 KB FIFO.
                                                                 0x06 = 6 KB FIFO.
                                                                 0x07 = 7 KB FIFO.
                                                                 0x08 = 8 KB FIFO.
                                                                 0x09 = 9 KB FIFO.
                                                                 0x0A = 10 KB FIFO.
                                                                 0x0B = 11 KB FIFO.
                                                                 0x0C = 12 KB FIFO.
                                                                 0x0D = 13 KB FIFO.
                                                                 0x0E = 14 KB FIFO.
                                                                 0x0F = 15 KB FIFO.
                                                                 0x10 = 16 KB FIFO. */
#else /* Word 0 - Little Endian */
        uint64_t blks                  : 6;  /**< [  5:  0](R/W) The size of the DMA engine FIFO. The sum of the allocated FIFOs across all six
                                                                 DPI()_ENG()_BUF[BLKS] registers must not exceed the overall RDB memory size of
                                                                 32 KB. If the value of 0 is used, the corresponding engine enable in
                                                                 DPI()_DMA_CONTROL[DMA_ENB]
                                                                 must also be cleared.

                                                                 0x00 = Engine disabled.
                                                                 0x01 = 1 KB FIFO.
                                                                 0x02 = 2 KB FIFO.
                                                                 0x03 = 3 KB FIFO.
                                                                 0x04 = 4 KB FIFO.
                                                                 0x05 = 5 KB FIFO.
                                                                 0x06 = 6 KB FIFO.
                                                                 0x07 = 7 KB FIFO.
                                                                 0x08 = 8 KB FIFO.
                                                                 0x09 = 9 KB FIFO.
                                                                 0x0A = 10 KB FIFO.
                                                                 0x0B = 11 KB FIFO.
                                                                 0x0C = 12 KB FIFO.
                                                                 0x0D = 13 KB FIFO.
                                                                 0x0E = 14 KB FIFO.
                                                                 0x0F = 15 KB FIFO.
                                                                 0x10 = 16 KB FIFO. */
        uint64_t reserved_6_15         : 10;
        uint64_t base                  : 6;  /**< [ 21: 16](RO/H) The base address in 512-byte blocks of the DMA engine FIFO. */
        uint64_t reserved_22_63        : 42;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_engx_buf_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_22_63        : 42;
        uint64_t base                  : 6;  /**< [ 21: 16](RO/H) The base address in 512-byte blocks of the DMA engine FIFO. */
        uint64_t reserved_5_15         : 11;
        uint64_t blks                  : 5;  /**< [  4:  0](R/W) The size of the DMA engine FIFO. The sum of the allocated FIFOs across all six
                                                                 DPI()_ENG()_BUF[BLKS] registers must not exceed the overall RDB memory size of
                                                                 32 KB. If the value of 0 is used, the corresponding engine enable in
                                                                 DPI()_DMA_CONTROL[DMA_ENB]
                                                                 must also be cleared.

                                                                 0x00 = Engine disabled.
                                                                 0x01 = 1 KB FIFO.
                                                                 0x02 = 2 KB FIFO.
                                                                 0x03 = 3 KB FIFO.
                                                                 0x04 = 4 KB FIFO.
                                                                 0x05 = 5 KB FIFO.
                                                                 0x06 = 6 KB FIFO.
                                                                 0x07 = 7 KB FIFO.
                                                                 0x08 = 8 KB FIFO.
                                                                 0x09 = 9 KB FIFO.
                                                                 0x0A = 10 KB FIFO.
                                                                 0x0B = 11 KB FIFO.
                                                                 0x0C = 12 KB FIFO.
                                                                 0x0D = 13 KB FIFO.
                                                                 0x0E = 14 KB FIFO.
                                                                 0x0F = 15 KB FIFO.
                                                                 0x10 = 16 KB FIFO. */
#else /* Word 0 - Little Endian */
        uint64_t blks                  : 5;  /**< [  4:  0](R/W) The size of the DMA engine FIFO. The sum of the allocated FIFOs across all six
                                                                 DPI()_ENG()_BUF[BLKS] registers must not exceed the overall RDB memory size of
                                                                 32 KB. If the value of 0 is used, the corresponding engine enable in
                                                                 DPI()_DMA_CONTROL[DMA_ENB]
                                                                 must also be cleared.

                                                                 0x00 = Engine disabled.
                                                                 0x01 = 1 KB FIFO.
                                                                 0x02 = 2 KB FIFO.
                                                                 0x03 = 3 KB FIFO.
                                                                 0x04 = 4 KB FIFO.
                                                                 0x05 = 5 KB FIFO.
                                                                 0x06 = 6 KB FIFO.
                                                                 0x07 = 7 KB FIFO.
                                                                 0x08 = 8 KB FIFO.
                                                                 0x09 = 9 KB FIFO.
                                                                 0x0A = 10 KB FIFO.
                                                                 0x0B = 11 KB FIFO.
                                                                 0x0C = 12 KB FIFO.
                                                                 0x0D = 13 KB FIFO.
                                                                 0x0E = 14 KB FIFO.
                                                                 0x0F = 15 KB FIFO.
                                                                 0x10 = 16 KB FIFO. */
        uint64_t reserved_5_15         : 11;
        uint64_t base                  : 6;  /**< [ 21: 16](RO/H) The base address in 512-byte blocks of the DMA engine FIFO. */
        uint64_t reserved_22_63        : 42;
#endif /* Word 0 - End */
    } cn8;
    struct cavm_dpix_engx_buf_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_22_63        : 42;
        uint64_t base                  : 6;  /**< [ 21: 16](RO/H) The base address in 1 Kbyte blocks of the DMA engine FIFO. */
        uint64_t reserved_6_15         : 10;
        uint64_t blks                  : 6;  /**< [  5:  0](R/W) The size of the DMA engine FIFO in KBytes. The sum of the allocated FIFOs across all six
                                                                 DPI()_ENG()_BUF[BLKS] registers must not exceed the overall RDB memory size of
                                                                 64 KB. If the value of 0 is used, the corresponding engine enable in
                                                                 DPI()_DMA_CONTROL[DMA_ENB] must also be cleared.

                                                                 0x00 = Engine disabled.
                                                                 0x01 = 1 KB FIFO.
                                                                 0x02 = 2 KB FIFO.
                                                                 0x03 = 3 KB FIFO.
                                                                 0x04 = 4 KB FIFO.
                                                                 ..
                                                                 0x1F = 31 KB FIFO.
                                                                 0x20 = 32 KB FIFO. */
#else /* Word 0 - Little Endian */
        uint64_t blks                  : 6;  /**< [  5:  0](R/W) The size of the DMA engine FIFO in KBytes. The sum of the allocated FIFOs across all six
                                                                 DPI()_ENG()_BUF[BLKS] registers must not exceed the overall RDB memory size of
                                                                 64 KB. If the value of 0 is used, the corresponding engine enable in
                                                                 DPI()_DMA_CONTROL[DMA_ENB] must also be cleared.

                                                                 0x00 = Engine disabled.
                                                                 0x01 = 1 KB FIFO.
                                                                 0x02 = 2 KB FIFO.
                                                                 0x03 = 3 KB FIFO.
                                                                 0x04 = 4 KB FIFO.
                                                                 ..
                                                                 0x1F = 31 KB FIFO.
                                                                 0x20 = 32 KB FIFO. */
        uint64_t reserved_6_15         : 10;
        uint64_t base                  : 6;  /**< [ 21: 16](RO/H) The base address in 1 Kbyte blocks of the DMA engine FIFO. */
        uint64_t reserved_22_63        : 42;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_dpix_engx_buf cavm_dpix_engx_buf_t;

static inline uint64_t CAVM_DPIX_ENGX_BUF(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_ENGX_BUF(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=5)))
        return 0x86e0000040c0ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=5)))
        return 0x86e0000040c0ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_ENGX_BUF", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_ENGX_BUF(a,b) cavm_dpix_engx_buf_t
#define bustype_CAVM_DPIX_ENGX_BUF(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_ENGX_BUF(a,b) "DPIX_ENGX_BUF"
#define device_bar_CAVM_DPIX_ENGX_BUF(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_ENGX_BUF(a,b) (a)
#define arguments_CAVM_DPIX_ENGX_BUF(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_epf#_dma_vf_lint#
 *
 * DPI DMA Error Response VF Bit Array Registers
 * When an error response is received for a VF DMA transaction read, the appropriate VF indexed
 * bit is set.  The appropriate PF should read the appropriate register.
 * These registers are only valid for PEM0 PF0 and PEM2 PF0.
 * Each EPF supports 128 VFs, lower index set VFs 0 to 63 and upper index set VFs 64 to 127
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_dpix_epfx_dma_vf_lintx
{
    uint64_t u;
    struct cavm_dpix_epfx_dma_vf_lintx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) When an error response is received for a VF DMA transaction read, the appropriate VF
                                                                 indexed bit is set. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) When an error response is received for a VF DMA transaction read, the appropriate VF
                                                                 indexed bit is set. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_epfx_dma_vf_lintx_s cn; */
};
typedef union cavm_dpix_epfx_dma_vf_lintx cavm_dpix_epfx_dma_vf_lintx_t;

static inline uint64_t CAVM_DPIX_EPFX_DMA_VF_LINTX(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EPFX_DMA_VF_LINTX(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e000006800ll + 0x1000000000ll * ((a) & 0x0) + 0x20ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("DPIX_EPFX_DMA_VF_LINTX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EPFX_DMA_VF_LINTX(a,b,c) cavm_dpix_epfx_dma_vf_lintx_t
#define bustype_CAVM_DPIX_EPFX_DMA_VF_LINTX(a,b,c) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EPFX_DMA_VF_LINTX(a,b,c) "DPIX_EPFX_DMA_VF_LINTX"
#define device_bar_CAVM_DPIX_EPFX_DMA_VF_LINTX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EPFX_DMA_VF_LINTX(a,b,c) (a)
#define arguments_CAVM_DPIX_EPFX_DMA_VF_LINTX(a,b,c) (a),(b),(c),-1

/**
 * Register (NCB) dpi#_epf#_dma_vf_lint#_ena_w1c
 *
 * DPI DMA Error Response VF Bit Array Local Enable Clear Registers
 * This register clears interrupt enable bits.
 */
union cavm_dpix_epfx_dma_vf_lintx_ena_w1c
{
    uint64_t u;
    struct cavm_dpix_epfx_dma_vf_lintx_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_DMA_VF_LINT(0..1)[VF_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_DMA_VF_LINT(0..1)[VF_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_epfx_dma_vf_lintx_ena_w1c_s cn; */
};
typedef union cavm_dpix_epfx_dma_vf_lintx_ena_w1c cavm_dpix_epfx_dma_vf_lintx_ena_w1c_t;

static inline uint64_t CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e000006c00ll + 0x1000000000ll * ((a) & 0x0) + 0x20ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("DPIX_EPFX_DMA_VF_LINTX_ENA_W1C", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1C(a,b,c) cavm_dpix_epfx_dma_vf_lintx_ena_w1c_t
#define bustype_CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1C(a,b,c) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1C(a,b,c) "DPIX_EPFX_DMA_VF_LINTX_ENA_W1C"
#define device_bar_CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1C(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1C(a,b,c) (a)
#define arguments_CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1C(a,b,c) (a),(b),(c),-1

/**
 * Register (NCB) dpi#_epf#_dma_vf_lint#_ena_w1s
 *
 * DPI DMA Error Response VF Bit Array Local Enable Set Registers
 * This register sets interrupt enable bits.
 */
union cavm_dpix_epfx_dma_vf_lintx_ena_w1s
{
    uint64_t u;
    struct cavm_dpix_epfx_dma_vf_lintx_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_DMA_VF_LINT(0..1)[VF_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_DMA_VF_LINT(0..1)[VF_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_epfx_dma_vf_lintx_ena_w1s_s cn; */
};
typedef union cavm_dpix_epfx_dma_vf_lintx_ena_w1s cavm_dpix_epfx_dma_vf_lintx_ena_w1s_t;

static inline uint64_t CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e000006e00ll + 0x1000000000ll * ((a) & 0x0) + 0x20ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("DPIX_EPFX_DMA_VF_LINTX_ENA_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1S(a,b,c) cavm_dpix_epfx_dma_vf_lintx_ena_w1s_t
#define bustype_CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1S(a,b,c) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1S(a,b,c) "DPIX_EPFX_DMA_VF_LINTX_ENA_W1S"
#define device_bar_CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1S(a,b,c) (a)
#define arguments_CAVM_DPIX_EPFX_DMA_VF_LINTX_ENA_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (NCB) dpi#_epf#_dma_vf_lint#_w1s
 *
 * DPI DMA Error Response VF Bit Array Set Registers
 * This register sets interrupt bits.
 */
union cavm_dpix_epfx_dma_vf_lintx_w1s
{
    uint64_t u;
    struct cavm_dpix_epfx_dma_vf_lintx_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_DMA_VF_LINT(0..1)[VF_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_DMA_VF_LINT(0..1)[VF_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_epfx_dma_vf_lintx_w1s_s cn; */
};
typedef union cavm_dpix_epfx_dma_vf_lintx_w1s cavm_dpix_epfx_dma_vf_lintx_w1s_t;

static inline uint64_t CAVM_DPIX_EPFX_DMA_VF_LINTX_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EPFX_DMA_VF_LINTX_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e000006a00ll + 0x1000000000ll * ((a) & 0x0) + 0x20ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("DPIX_EPFX_DMA_VF_LINTX_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EPFX_DMA_VF_LINTX_W1S(a,b,c) cavm_dpix_epfx_dma_vf_lintx_w1s_t
#define bustype_CAVM_DPIX_EPFX_DMA_VF_LINTX_W1S(a,b,c) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EPFX_DMA_VF_LINTX_W1S(a,b,c) "DPIX_EPFX_DMA_VF_LINTX_W1S"
#define device_bar_CAVM_DPIX_EPFX_DMA_VF_LINTX_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EPFX_DMA_VF_LINTX_W1S(a,b,c) (a)
#define arguments_CAVM_DPIX_EPFX_DMA_VF_LINTX_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (NCB) dpi#_epf#_misc_lint
 *
 * DPI EPF Interrupt Summary Register
 * This register contains the different interrupt-summary bits for one MAC in the DPI.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_dpix_epfx_misc_lint
{
    uint64_t u;
    struct cavm_dpix_epfx_misc_lint_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_7_63         : 57;
        uint64_t flr                   : 1;  /**< [  6:  6](R/W1C/H) A FLR occurred for the PF on the corresponding MAC. */
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1C/H) An error response was received for a PF DMA transaction read. Additional
                                                                 information is captured in DPI()_EBUS_PORT()_ERR_INFO and
                                                                 DPI()_EBUS_PORT()_ERR[ADDR]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1C/H) Set when an error response is received for a PF PP transaction read. Additional
                                                                 information is captured in DPI()_EBUS_PORT()_ERR_INFO and
                                                                 DPI()_EBUS_PORT()_ERR[ADDR]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1C/H) Received unsupported N-TLP for window register from the corresponding MAC. This
                                                                 occurs when the window registers are disabled and a window register access occurs. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1C/H) Received unsupported N-TLP for Bar 0 from the corresponding MAC. This occurs
                                                                 when the BAR 0 address space is disabled. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1C/H) Received unsupported P-TLP for window register from the corresponding MAC. This
                                                                 occurs when the window registers are disabled and a window register access
                                                                 occurs. */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1C/H) Received unsupported P-TLP for Bar 0 from the corresponding MAC. This occurs
                                                                 when the BAR 0 address space is disabled. */
#else /* Word 0 - Little Endian */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1C/H) Received unsupported P-TLP for Bar 0 from the corresponding MAC. This occurs
                                                                 when the BAR 0 address space is disabled. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1C/H) Received unsupported P-TLP for window register from the corresponding MAC. This
                                                                 occurs when the window registers are disabled and a window register access
                                                                 occurs. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1C/H) Received unsupported N-TLP for Bar 0 from the corresponding MAC. This occurs
                                                                 when the BAR 0 address space is disabled. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1C/H) Received unsupported N-TLP for window register from the corresponding MAC. This
                                                                 occurs when the window registers are disabled and a window register access occurs. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1C/H) Set when an error response is received for a PF PP transaction read. Additional
                                                                 information is captured in DPI()_EBUS_PORT()_ERR_INFO and
                                                                 DPI()_EBUS_PORT()_ERR[ADDR]. */
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1C/H) An error response was received for a PF DMA transaction read. Additional
                                                                 information is captured in DPI()_EBUS_PORT()_ERR_INFO and
                                                                 DPI()_EBUS_PORT()_ERR[ADDR]. */
        uint64_t flr                   : 1;  /**< [  6:  6](R/W1C/H) A FLR occurred for the PF on the corresponding MAC. */
        uint64_t reserved_7_63         : 57;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_epfx_misc_lint_s cn; */
};
typedef union cavm_dpix_epfx_misc_lint cavm_dpix_epfx_misc_lint_t;

static inline uint64_t CAVM_DPIX_EPFX_MISC_LINT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EPFX_MISC_LINT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e000007000ll + 0x1000000000ll * ((a) & 0x0) + 0x20ll * ((b) & 0xf);
    __cavm_csr_fatal("DPIX_EPFX_MISC_LINT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EPFX_MISC_LINT(a,b) cavm_dpix_epfx_misc_lint_t
#define bustype_CAVM_DPIX_EPFX_MISC_LINT(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EPFX_MISC_LINT(a,b) "DPIX_EPFX_MISC_LINT"
#define device_bar_CAVM_DPIX_EPFX_MISC_LINT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EPFX_MISC_LINT(a,b) (a)
#define arguments_CAVM_DPIX_EPFX_MISC_LINT(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_epf#_misc_lint_ena_w1c
 *
 * DPI EPF Interrupt Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_dpix_epfx_misc_lint_ena_w1c
{
    uint64_t u;
    struct cavm_dpix_epfx_misc_lint_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_7_63         : 57;
        uint64_t flr                   : 1;  /**< [  6:  6](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[FLR]. */
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[DMAPF_ERR]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[PPPF_ERR]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[UN_WI]. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[UN_B0]. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[UP_WI]. */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[UP_B0]. */
#else /* Word 0 - Little Endian */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[UP_B0]. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[UP_WI]. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[UN_B0]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[UN_WI]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[PPPF_ERR]. */
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[DMAPF_ERR]. */
        uint64_t flr                   : 1;  /**< [  6:  6](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_MISC_LINT[FLR]. */
        uint64_t reserved_7_63         : 57;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_epfx_misc_lint_ena_w1c_s cn; */
};
typedef union cavm_dpix_epfx_misc_lint_ena_w1c cavm_dpix_epfx_misc_lint_ena_w1c_t;

static inline uint64_t CAVM_DPIX_EPFX_MISC_LINT_ENA_W1C(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EPFX_MISC_LINT_ENA_W1C(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e000007010ll + 0x1000000000ll * ((a) & 0x0) + 0x20ll * ((b) & 0xf);
    __cavm_csr_fatal("DPIX_EPFX_MISC_LINT_ENA_W1C", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EPFX_MISC_LINT_ENA_W1C(a,b) cavm_dpix_epfx_misc_lint_ena_w1c_t
#define bustype_CAVM_DPIX_EPFX_MISC_LINT_ENA_W1C(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EPFX_MISC_LINT_ENA_W1C(a,b) "DPIX_EPFX_MISC_LINT_ENA_W1C"
#define device_bar_CAVM_DPIX_EPFX_MISC_LINT_ENA_W1C(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EPFX_MISC_LINT_ENA_W1C(a,b) (a)
#define arguments_CAVM_DPIX_EPFX_MISC_LINT_ENA_W1C(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_epf#_misc_lint_ena_w1s
 *
 * DPI EPF Interrupt Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_dpix_epfx_misc_lint_ena_w1s
{
    uint64_t u;
    struct cavm_dpix_epfx_misc_lint_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_7_63         : 57;
        uint64_t flr                   : 1;  /**< [  6:  6](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[FLR]. */
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[DMAPF_ERR]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[PPPF_ERR]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[UN_WI]. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[UN_B0]. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[UP_WI]. */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[UP_B0]. */
#else /* Word 0 - Little Endian */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[UP_B0]. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[UP_WI]. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[UN_B0]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[UN_WI]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[PPPF_ERR]. */
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[DMAPF_ERR]. */
        uint64_t flr                   : 1;  /**< [  6:  6](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_MISC_LINT[FLR]. */
        uint64_t reserved_7_63         : 57;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_epfx_misc_lint_ena_w1s_s cn; */
};
typedef union cavm_dpix_epfx_misc_lint_ena_w1s cavm_dpix_epfx_misc_lint_ena_w1s_t;

static inline uint64_t CAVM_DPIX_EPFX_MISC_LINT_ENA_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EPFX_MISC_LINT_ENA_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e000007018ll + 0x1000000000ll * ((a) & 0x0) + 0x20ll * ((b) & 0xf);
    __cavm_csr_fatal("DPIX_EPFX_MISC_LINT_ENA_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EPFX_MISC_LINT_ENA_W1S(a,b) cavm_dpix_epfx_misc_lint_ena_w1s_t
#define bustype_CAVM_DPIX_EPFX_MISC_LINT_ENA_W1S(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EPFX_MISC_LINT_ENA_W1S(a,b) "DPIX_EPFX_MISC_LINT_ENA_W1S"
#define device_bar_CAVM_DPIX_EPFX_MISC_LINT_ENA_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EPFX_MISC_LINT_ENA_W1S(a,b) (a)
#define arguments_CAVM_DPIX_EPFX_MISC_LINT_ENA_W1S(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_epf#_misc_lint_w1s
 *
 * DPI EPF Interrupt Set Register
 * This register sets interrupt bits.
 */
union cavm_dpix_epfx_misc_lint_w1s
{
    uint64_t u;
    struct cavm_dpix_epfx_misc_lint_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_7_63         : 57;
        uint64_t flr                   : 1;  /**< [  6:  6](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[FLR]. */
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[DMAPF_ERR]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[PPPF_ERR]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[UN_WI]. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[UN_B0]. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[UP_WI]. */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[UP_B0]. */
#else /* Word 0 - Little Endian */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[UP_B0]. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[UP_WI]. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[UN_B0]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[UN_WI]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[PPPF_ERR]. */
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[DMAPF_ERR]. */
        uint64_t flr                   : 1;  /**< [  6:  6](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_MISC_LINT[FLR]. */
        uint64_t reserved_7_63         : 57;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_epfx_misc_lint_w1s_s cn; */
};
typedef union cavm_dpix_epfx_misc_lint_w1s cavm_dpix_epfx_misc_lint_w1s_t;

static inline uint64_t CAVM_DPIX_EPFX_MISC_LINT_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EPFX_MISC_LINT_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e000007008ll + 0x1000000000ll * ((a) & 0x0) + 0x20ll * ((b) & 0xf);
    __cavm_csr_fatal("DPIX_EPFX_MISC_LINT_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EPFX_MISC_LINT_W1S(a,b) cavm_dpix_epfx_misc_lint_w1s_t
#define bustype_CAVM_DPIX_EPFX_MISC_LINT_W1S(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EPFX_MISC_LINT_W1S(a,b) "DPIX_EPFX_MISC_LINT_W1S"
#define device_bar_CAVM_DPIX_EPFX_MISC_LINT_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EPFX_MISC_LINT_W1S(a,b) (a)
#define arguments_CAVM_DPIX_EPFX_MISC_LINT_W1S(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_epf#_pp_vf_lint#
 *
 * DPI DMA Error Response VF Bit Array Registers
 * When an error response is received for a VF PP transaction read, the appropriate VF indexed
 * bit is set.  The appropriate PF should read the appropriate register.
 * These registers are only valid for PEM0 PF0 and PEM2 PF0.
 * Each EPF supports 128 VFs, lower index set VFs 0 to 63 and upper index set VFs 64 to 127
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_dpix_epfx_pp_vf_lintx
{
    uint64_t u;
    struct cavm_dpix_epfx_pp_vf_lintx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) When an error response is received for a VF PP transaction read, the appropriate VF
                                                                 indexed bit is set. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) When an error response is received for a VF PP transaction read, the appropriate VF
                                                                 indexed bit is set. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_epfx_pp_vf_lintx_s cn; */
};
typedef union cavm_dpix_epfx_pp_vf_lintx cavm_dpix_epfx_pp_vf_lintx_t;

static inline uint64_t CAVM_DPIX_EPFX_PP_VF_LINTX(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EPFX_PP_VF_LINTX(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e000007200ll + 0x1000000000ll * ((a) & 0x0) + 0x20ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("DPIX_EPFX_PP_VF_LINTX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EPFX_PP_VF_LINTX(a,b,c) cavm_dpix_epfx_pp_vf_lintx_t
#define bustype_CAVM_DPIX_EPFX_PP_VF_LINTX(a,b,c) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EPFX_PP_VF_LINTX(a,b,c) "DPIX_EPFX_PP_VF_LINTX"
#define device_bar_CAVM_DPIX_EPFX_PP_VF_LINTX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EPFX_PP_VF_LINTX(a,b,c) (a)
#define arguments_CAVM_DPIX_EPFX_PP_VF_LINTX(a,b,c) (a),(b),(c),-1

/**
 * Register (NCB) dpi#_epf#_pp_vf_lint#_ena_w1c
 *
 * DPI DMA Error Response VF Bit Array Local Enable Clear Registers
 * This register clears interrupt enable bits.
 */
union cavm_dpix_epfx_pp_vf_lintx_ena_w1c
{
    uint64_t u;
    struct cavm_dpix_epfx_pp_vf_lintx_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_PP_VF_LINT(0..1)[VF_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for DPI(0)_EPF(0..15)_PP_VF_LINT(0..1)[VF_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_epfx_pp_vf_lintx_ena_w1c_s cn; */
};
typedef union cavm_dpix_epfx_pp_vf_lintx_ena_w1c cavm_dpix_epfx_pp_vf_lintx_ena_w1c_t;

static inline uint64_t CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e000007600ll + 0x1000000000ll * ((a) & 0x0) + 0x20ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("DPIX_EPFX_PP_VF_LINTX_ENA_W1C", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1C(a,b,c) cavm_dpix_epfx_pp_vf_lintx_ena_w1c_t
#define bustype_CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1C(a,b,c) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1C(a,b,c) "DPIX_EPFX_PP_VF_LINTX_ENA_W1C"
#define device_bar_CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1C(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1C(a,b,c) (a)
#define arguments_CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1C(a,b,c) (a),(b),(c),-1

/**
 * Register (NCB) dpi#_epf#_pp_vf_lint#_ena_w1s
 *
 * DPI DMA Error Response VF Bit Array Local Enable Set Registers
 * This register sets interrupt enable bits.
 */
union cavm_dpix_epfx_pp_vf_lintx_ena_w1s
{
    uint64_t u;
    struct cavm_dpix_epfx_pp_vf_lintx_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_PP_VF_LINT(0..1)[VF_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for DPI(0)_EPF(0..15)_PP_VF_LINT(0..1)[VF_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_epfx_pp_vf_lintx_ena_w1s_s cn; */
};
typedef union cavm_dpix_epfx_pp_vf_lintx_ena_w1s cavm_dpix_epfx_pp_vf_lintx_ena_w1s_t;

static inline uint64_t CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e000007800ll + 0x1000000000ll * ((a) & 0x0) + 0x20ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("DPIX_EPFX_PP_VF_LINTX_ENA_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1S(a,b,c) cavm_dpix_epfx_pp_vf_lintx_ena_w1s_t
#define bustype_CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1S(a,b,c) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1S(a,b,c) "DPIX_EPFX_PP_VF_LINTX_ENA_W1S"
#define device_bar_CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1S(a,b,c) (a)
#define arguments_CAVM_DPIX_EPFX_PP_VF_LINTX_ENA_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (NCB) dpi#_epf#_pp_vf_lint#_w1s
 *
 * DPI DMA Error Response VF Bit Array Set Registers
 * This register sets interrupt bits.
 */
union cavm_dpix_epfx_pp_vf_lintx_w1s
{
    uint64_t u;
    struct cavm_dpix_epfx_pp_vf_lintx_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_PP_VF_LINT(0..1)[VF_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets DPI(0)_EPF(0..15)_PP_VF_LINT(0..1)[VF_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_epfx_pp_vf_lintx_w1s_s cn; */
};
typedef union cavm_dpix_epfx_pp_vf_lintx_w1s cavm_dpix_epfx_pp_vf_lintx_w1s_t;

static inline uint64_t CAVM_DPIX_EPFX_PP_VF_LINTX_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_EPFX_PP_VF_LINTX_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e000007400ll + 0x1000000000ll * ((a) & 0x0) + 0x20ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("DPIX_EPFX_PP_VF_LINTX_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_DPIX_EPFX_PP_VF_LINTX_W1S(a,b,c) cavm_dpix_epfx_pp_vf_lintx_w1s_t
#define bustype_CAVM_DPIX_EPFX_PP_VF_LINTX_W1S(a,b,c) CSR_TYPE_NCB
#define basename_CAVM_DPIX_EPFX_PP_VF_LINTX_W1S(a,b,c) "DPIX_EPFX_PP_VF_LINTX_W1S"
#define device_bar_CAVM_DPIX_EPFX_PP_VF_LINTX_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_EPFX_PP_VF_LINTX_W1S(a,b,c) (a)
#define arguments_CAVM_DPIX_EPFX_PP_VF_LINTX_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (NCB) dpi#_info_reg
 *
 * DPI Information Register
 */
union cavm_dpix_info_reg
{
    uint64_t u;
    struct cavm_dpix_info_reg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_8_63         : 56;
        uint64_t ffp                   : 4;  /**< [  7:  4](R/W1C/H) Force forward progress indicator. */
        uint64_t reserved_2_3          : 2;
        uint64_t ncb                   : 1;  /**< [  1:  1](R/W1C/H) NCB register access. This bit gets set in normal operation when software reads a DPI
                                                                 register through the NCB interface. */
        uint64_t rsl                   : 1;  /**< [  0:  0](R/W1C/H) RSL register access. This bit is set in normal operation when software reads a DPI
                                                                 register through the RSL interface. */
#else /* Word 0 - Little Endian */
        uint64_t rsl                   : 1;  /**< [  0:  0](R/W1C/H) RSL register access. This bit is set in normal operation when software reads a DPI
                                                                 register through the RSL interface. */
        uint64_t ncb                   : 1;  /**< [  1:  1](R/W1C/H) NCB register access. This bit gets set in normal operation when software reads a DPI
                                                                 register through the NCB interface. */
        uint64_t reserved_2_3          : 2;
        uint64_t ffp                   : 4;  /**< [  7:  4](R/W1C/H) Force forward progress indicator. */
        uint64_t reserved_8_63         : 56;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_info_reg_s cn; */
};
typedef union cavm_dpix_info_reg cavm_dpix_info_reg_t;

static inline uint64_t CAVM_DPIX_INFO_REG(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_INFO_REG(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004160ll + 0x1000000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e000004300ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_INFO_REG", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_INFO_REG(a) cavm_dpix_info_reg_t
#define bustype_CAVM_DPIX_INFO_REG(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_INFO_REG(a) "DPIX_INFO_REG"
#define device_bar_CAVM_DPIX_INFO_REG(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_INFO_REG(a) (a)
#define arguments_CAVM_DPIX_INFO_REG(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_int_ena_w1c
 *
 * DPI Interrupt Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_dpix_int_ena_w1c
{
    uint64_t u;
    struct cavm_dpix_int_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t nfovr                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for DPI(0)_INT_REG[NFOVR]. */
        uint64_t nderr                 : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_INT_REG[NDERR]. */
#else /* Word 0 - Little Endian */
        uint64_t nderr                 : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_INT_REG[NDERR]. */
        uint64_t nfovr                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for DPI(0)_INT_REG[NFOVR]. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_int_ena_w1c_s cn; */
};
typedef union cavm_dpix_int_ena_w1c cavm_dpix_int_ena_w1c_t;

static inline uint64_t CAVM_DPIX_INT_ENA_W1C(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_INT_ENA_W1C(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004178ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_INT_ENA_W1C", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_INT_ENA_W1C(a) cavm_dpix_int_ena_w1c_t
#define bustype_CAVM_DPIX_INT_ENA_W1C(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_INT_ENA_W1C(a) "DPIX_INT_ENA_W1C"
#define device_bar_CAVM_DPIX_INT_ENA_W1C(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_INT_ENA_W1C(a) (a)
#define arguments_CAVM_DPIX_INT_ENA_W1C(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_int_ena_w1s
 *
 * DPI Interrupt Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_dpix_int_ena_w1s
{
    uint64_t u;
    struct cavm_dpix_int_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t nfovr                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for DPI(0)_INT_REG[NFOVR]. */
        uint64_t nderr                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_INT_REG[NDERR]. */
#else /* Word 0 - Little Endian */
        uint64_t nderr                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_INT_REG[NDERR]. */
        uint64_t nfovr                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for DPI(0)_INT_REG[NFOVR]. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_int_ena_w1s_s cn; */
};
typedef union cavm_dpix_int_ena_w1s cavm_dpix_int_ena_w1s_t;

static inline uint64_t CAVM_DPIX_INT_ENA_W1S(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_INT_ENA_W1S(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004180ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_INT_ENA_W1S", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_INT_ENA_W1S(a) cavm_dpix_int_ena_w1s_t
#define bustype_CAVM_DPIX_INT_ENA_W1S(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_INT_ENA_W1S(a) "DPIX_INT_ENA_W1S"
#define device_bar_CAVM_DPIX_INT_ENA_W1S(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_INT_ENA_W1S(a) (a)
#define arguments_CAVM_DPIX_INT_ENA_W1S(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_int_reg
 *
 * DPI Interrupt Summary Register
 * This register contains error flags for DPI.
 */
union cavm_dpix_int_reg
{
    uint64_t u;
    struct cavm_dpix_int_reg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t nfovr                 : 1;  /**< [  1:  1](R/W1C/H) CSR FIFO overflow error. DPI can store up to 16 CSR requests, and the FIFO overflows if
                                                                 that number is exceeded. */
        uint64_t nderr                 : 1;  /**< [  0:  0](R/W1C/H) IOI decode error. This bit is set when the DPI received an IOI transaction on the outbound
                                                                 bus to the DPI device ID, but the command was not recognized. */
#else /* Word 0 - Little Endian */
        uint64_t nderr                 : 1;  /**< [  0:  0](R/W1C/H) IOI decode error. This bit is set when the DPI received an IOI transaction on the outbound
                                                                 bus to the DPI device ID, but the command was not recognized. */
        uint64_t nfovr                 : 1;  /**< [  1:  1](R/W1C/H) CSR FIFO overflow error. DPI can store up to 16 CSR requests, and the FIFO overflows if
                                                                 that number is exceeded. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_int_reg_s cn; */
};
typedef union cavm_dpix_int_reg cavm_dpix_int_reg_t;

static inline uint64_t CAVM_DPIX_INT_REG(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_INT_REG(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004168ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_INT_REG", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_INT_REG(a) cavm_dpix_int_reg_t
#define bustype_CAVM_DPIX_INT_REG(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_INT_REG(a) "DPIX_INT_REG"
#define device_bar_CAVM_DPIX_INT_REG(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_INT_REG(a) (a)
#define arguments_CAVM_DPIX_INT_REG(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_int_reg_w1s
 *
 * DPI Interrupt Set Register
 * This register sets interrupt bits.
 */
union cavm_dpix_int_reg_w1s
{
    uint64_t u;
    struct cavm_dpix_int_reg_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t nfovr                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets DPI(0)_INT_REG[NFOVR]. */
        uint64_t nderr                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_INT_REG[NDERR]. */
#else /* Word 0 - Little Endian */
        uint64_t nderr                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_INT_REG[NDERR]. */
        uint64_t nfovr                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets DPI(0)_INT_REG[NFOVR]. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_int_reg_w1s_s cn; */
};
typedef union cavm_dpix_int_reg_w1s cavm_dpix_int_reg_w1s_t;

static inline uint64_t CAVM_DPIX_INT_REG_W1S(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_INT_REG_W1S(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004170ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_INT_REG_W1S", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_INT_REG_W1S(a) cavm_dpix_int_reg_w1s_t
#define bustype_CAVM_DPIX_INT_REG_W1S(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_INT_REG_W1S(a) "DPIX_INT_REG_W1S"
#define device_bar_CAVM_DPIX_INT_REG_W1S(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_INT_REG_W1S(a) (a)
#define arguments_CAVM_DPIX_INT_REG_W1S(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_ncb#_cfg
 *
 * DPI NCB Configuration Register
 */
union cavm_dpix_ncbx_cfg
{
    uint64_t u;
    struct cavm_dpix_ncbx_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_22_63        : 42;
        uint64_t rd_fifo_threshold     : 6;  /**< [ 21: 16](R/W) NCB read response data FIFO priority threshold. Allows the RDB memory arbiter to switch
                                                                 priority away from reads to NCB writes to ensure the NCB response FIFO does not overflow.
                                                                 This value can range from 0 to 32, 0x0 disables this feature. */
        uint64_t reserved_7_15         : 9;
        uint64_t molr                  : 7;  /**< [  6:  0](R/W) Max outstanding load requests. Limits the number of outstanding load requests on the I/O
                                                                 interconnect. This value can range from 1 to 32. Setting a value of zero halts all read
                                                                 traffic to the I/O Interconnect. There are no restrictions on when this value can be
                                                                 changed. */
#else /* Word 0 - Little Endian */
        uint64_t molr                  : 7;  /**< [  6:  0](R/W) Max outstanding load requests. Limits the number of outstanding load requests on the I/O
                                                                 interconnect. This value can range from 1 to 32. Setting a value of zero halts all read
                                                                 traffic to the I/O Interconnect. There are no restrictions on when this value can be
                                                                 changed. */
        uint64_t reserved_7_15         : 9;
        uint64_t rd_fifo_threshold     : 6;  /**< [ 21: 16](R/W) NCB read response data FIFO priority threshold. Allows the RDB memory arbiter to switch
                                                                 priority away from reads to NCB writes to ensure the NCB response FIFO does not overflow.
                                                                 This value can range from 0 to 32, 0x0 disables this feature. */
        uint64_t reserved_22_63        : 42;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_ncbx_cfg_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_22_63        : 42;
        uint64_t rd_fifo_threshold     : 6;  /**< [ 21: 16](R/W) NCB read response data FIFO priority threshold. Allows the RDB memory arbiter to switch
                                                                 priority away from reads to NCB writes to ensure the NCB response FIFO does not overflow.
                                                                 This value can range from 0 to 32, 0x0 disables this feature. */
        uint64_t reserved_6_15         : 10;
        uint64_t molr                  : 6;  /**< [  5:  0](R/W) Max outstanding load requests. Limits the number of outstanding load requests on the I/O
                                                                 interconnect. This value can range from 1 to 32. Setting a value of zero halts all read
                                                                 traffic to the I/O Interconnect. There are no restrictions on when this value can be
                                                                 changed. */
#else /* Word 0 - Little Endian */
        uint64_t molr                  : 6;  /**< [  5:  0](R/W) Max outstanding load requests. Limits the number of outstanding load requests on the I/O
                                                                 interconnect. This value can range from 1 to 32. Setting a value of zero halts all read
                                                                 traffic to the I/O Interconnect. There are no restrictions on when this value can be
                                                                 changed. */
        uint64_t reserved_6_15         : 10;
        uint64_t rd_fifo_threshold     : 6;  /**< [ 21: 16](R/W) NCB read response data FIFO priority threshold. Allows the RDB memory arbiter to switch
                                                                 priority away from reads to NCB writes to ensure the NCB response FIFO does not overflow.
                                                                 This value can range from 0 to 32, 0x0 disables this feature. */
        uint64_t reserved_22_63        : 42;
#endif /* Word 0 - End */
    } cn8;
    struct cavm_dpix_ncbx_cfg_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_22_63        : 42;
        uint64_t rd_fifo_threshold     : 6;  /**< [ 21: 16](R/W) NCB read response data FIFO priority threshold. Allows the RDB memory arbiter to switch
                                                                 priority away from reads to NCB writes to ensure the NCB response FIFO does not overflow.
                                                                 This value can range from 0 to 32, 0x0 disables this feature. */
        uint64_t reserved_7_15         : 9;
        uint64_t molr                  : 7;  /**< [  6:  0](R/W) Max outstanding load requests. Limits the number of outstanding load requests on the NCB
                                                                 bus. This value can range from 1 to 64. Setting a value of zero halts all read
                                                                 traffic on NCB. There are no restrictions on when this value can be changed. */
#else /* Word 0 - Little Endian */
        uint64_t molr                  : 7;  /**< [  6:  0](R/W) Max outstanding load requests. Limits the number of outstanding load requests on the NCB
                                                                 bus. This value can range from 1 to 64. Setting a value of zero halts all read
                                                                 traffic on NCB. There are no restrictions on when this value can be changed. */
        uint64_t reserved_7_15         : 9;
        uint64_t rd_fifo_threshold     : 6;  /**< [ 21: 16](R/W) NCB read response data FIFO priority threshold. Allows the RDB memory arbiter to switch
                                                                 priority away from reads to NCB writes to ensure the NCB response FIFO does not overflow.
                                                                 This value can range from 0 to 32, 0x0 disables this feature. */
        uint64_t reserved_22_63        : 42;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_dpix_ncbx_cfg cavm_dpix_ncbx_cfg_t;

static inline uint64_t CAVM_DPIX_NCBX_CFG(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_NCBX_CFG(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b==0)))
        return 0x86e0000040a0ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b==0)))
        return 0x86e0000040a0ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x0);
    __cavm_csr_fatal("DPIX_NCBX_CFG", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_NCBX_CFG(a,b) cavm_dpix_ncbx_cfg_t
#define bustype_CAVM_DPIX_NCBX_CFG(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_NCBX_CFG(a,b) "DPIX_NCBX_CFG"
#define device_bar_CAVM_DPIX_NCBX_CFG(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_NCBX_CFG(a,b) (a)
#define arguments_CAVM_DPIX_NCBX_CFG(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_pf_msix_pba#
 *
 * DPI MSI-X Pending Bit Array Registers
 * This register is the MSI-X PBA table; the bit number is indexed by the DPI_PF_INT_VEC_E
 * enumeration.
 */
union cavm_dpix_pf_msix_pbax
{
    uint64_t u;
    struct cavm_dpix_pf_msix_pbax_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO/H) Pending message for the associated DPI()_PF_MSIX_VEC()_CTL, enumerated by
                                                                 DPI_PF_INT_VEC_E. Bits that have no associated DPI_PF_INT_VEC_E are 0. */
#else /* Word 0 - Little Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO/H) Pending message for the associated DPI()_PF_MSIX_VEC()_CTL, enumerated by
                                                                 DPI_PF_INT_VEC_E. Bits that have no associated DPI_PF_INT_VEC_E are 0. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_pf_msix_pbax_s cn; */
};
typedef union cavm_dpix_pf_msix_pbax cavm_dpix_pf_msix_pbax_t;

static inline uint64_t CAVM_DPIX_PF_MSIX_PBAX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_PF_MSIX_PBAX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=1)))
        return 0x86e0100f0000ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=2)))
        return 0x86e1000f0000ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3);
    __cavm_csr_fatal("DPIX_PF_MSIX_PBAX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_PF_MSIX_PBAX(a,b) cavm_dpix_pf_msix_pbax_t
#define bustype_CAVM_DPIX_PF_MSIX_PBAX(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_PF_MSIX_PBAX(a,b) "DPIX_PF_MSIX_PBAX"
#define device_bar_CAVM_DPIX_PF_MSIX_PBAX(a,b) 0x4 /* PF_BAR4 */
#define busnum_CAVM_DPIX_PF_MSIX_PBAX(a,b) (a)
#define arguments_CAVM_DPIX_PF_MSIX_PBAX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_pf_msix_vec#_addr
 *
 * DPI MSI-X Vector-Table Address Register
 * This register is the MSI-X vector table, indexed by the DPI_PF_INT_VEC_E enumeration.
 */
union cavm_dpix_pf_msix_vecx_addr
{
    uint64_t u;
    struct cavm_dpix_pf_msix_vecx_addr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_53_63        : 11;
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_1            : 1;
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 1 =  This vector's DPI_PF_MSIX_VEC()_ADDR, DPI_PF_MSIX_VEC()_CTL, and corresponding
                                                                 bit of DPI_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.

                                                                 If PCCPF_DPI_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is set, all vectors are secure and function as if
                                                                 [SECVEC] was set. */
#else /* Word 0 - Little Endian */
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 1 =  This vector's DPI_PF_MSIX_VEC()_ADDR, DPI_PF_MSIX_VEC()_CTL, and corresponding
                                                                 bit of DPI_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.

                                                                 If PCCPF_DPI_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is set, all vectors are secure and function as if
                                                                 [SECVEC] was set. */
        uint64_t reserved_1            : 1;
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_53_63        : 11;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_pf_msix_vecx_addr_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_49_63        : 15;
        uint64_t addr                  : 47; /**< [ 48:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_1            : 1;
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 1 =  This vector's DPI_PF_MSIX_VEC()_ADDR, DPI_PF_MSIX_VEC()_CTL, and corresponding
                                                                 bit of DPI_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.

                                                                 If PCCPF_DPI_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is set, all vectors are secure and function as if
                                                                 [SECVEC] was set. */
#else /* Word 0 - Little Endian */
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 1 =  This vector's DPI_PF_MSIX_VEC()_ADDR, DPI_PF_MSIX_VEC()_CTL, and corresponding
                                                                 bit of DPI_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.

                                                                 If PCCPF_DPI_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is set, all vectors are secure and function as if
                                                                 [SECVEC] was set. */
        uint64_t reserved_1            : 1;
        uint64_t addr                  : 47; /**< [ 48:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_49_63        : 15;
#endif /* Word 0 - End */
    } cn8;
    struct cavm_dpix_pf_msix_vecx_addr_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_53_63        : 11;
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_1            : 1;
        uint64_t secvec                : 1;  /**< [  0:  0](SR/W) Secure vector.
                                                                 0 = This vector may be read or written by either secure or nonsecure states.
                                                                 The vector's IOVA is sent to the SMMU as nonsecure (though this only affects
                                                                 physical addresses if PCCPF_XXX_VSEC_SCTL[MSIX_PHYS]=1).

                                                                 1 = This vector's DPI()_PF_MSIX_VEC()_ADDR, DPI()_PF_MSIX_VEC()_CTL, and corresponding
                                                                 bit of DPI()_PF_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.
                                                                 The vector's IOVA is sent to the SMMU as secure (though this only affects
                                                                 physical addresses if PCCPF_XXX_VSEC_SCTL[MSIX_PHYS]=1 or
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC_PHYS]=1).

                                                                 If PCCPF_DPI_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is set, all vectors are secure and function as if
                                                                 [SECVEC] was set.

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

                                                                 1 = This vector's DPI()_PF_MSIX_VEC()_ADDR, DPI()_PF_MSIX_VEC()_CTL, and corresponding
                                                                 bit of DPI()_PF_MSIX_PBA() are RAZ/WI and does not cause a fault when accessed
                                                                 by the nonsecure world.
                                                                 The vector's IOVA is sent to the SMMU as secure (though this only affects
                                                                 physical addresses if PCCPF_XXX_VSEC_SCTL[MSIX_PHYS]=1 or
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC_PHYS]=1).

                                                                 If PCCPF_DPI_VSEC_SCTL[MSIX_SEC] (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]) is set, all vectors are secure and function as if
                                                                 [SECVEC] was set.

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
    } cn9;
};
typedef union cavm_dpix_pf_msix_vecx_addr cavm_dpix_pf_msix_vecx_addr_t;

static inline uint64_t CAVM_DPIX_PF_MSIX_VECX_ADDR(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_PF_MSIX_VECX_ADDR(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=74)))
        return 0x86e010000000ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x7f);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=168)))
        return 0x86e100000000ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0xff);
    __cavm_csr_fatal("DPIX_PF_MSIX_VECX_ADDR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_PF_MSIX_VECX_ADDR(a,b) cavm_dpix_pf_msix_vecx_addr_t
#define bustype_CAVM_DPIX_PF_MSIX_VECX_ADDR(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_PF_MSIX_VECX_ADDR(a,b) "DPIX_PF_MSIX_VECX_ADDR"
#define device_bar_CAVM_DPIX_PF_MSIX_VECX_ADDR(a,b) 0x4 /* PF_BAR4 */
#define busnum_CAVM_DPIX_PF_MSIX_VECX_ADDR(a,b) (a)
#define arguments_CAVM_DPIX_PF_MSIX_VECX_ADDR(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_pf_msix_vec#_ctl
 *
 * DPI MSI-X Vector-Table Control and Data Register
 * This register is the MSI-X vector table, indexed by the DPI_PF_INT_VEC_E enumeration.
 */
union cavm_dpix_pf_msix_vecx_ctl
{
    uint64_t u;
    struct cavm_dpix_pf_msix_vecx_ctl_s
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
    struct cavm_dpix_pf_msix_vecx_ctl_cn8
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
    } cn8;
    /* struct cavm_dpix_pf_msix_vecx_ctl_s cn9; */
};
typedef union cavm_dpix_pf_msix_vecx_ctl cavm_dpix_pf_msix_vecx_ctl_t;

static inline uint64_t CAVM_DPIX_PF_MSIX_VECX_CTL(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_PF_MSIX_VECX_CTL(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=74)))
        return 0x86e010000008ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x7f);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=168)))
        return 0x86e100000008ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0xff);
    __cavm_csr_fatal("DPIX_PF_MSIX_VECX_CTL", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_PF_MSIX_VECX_CTL(a,b) cavm_dpix_pf_msix_vecx_ctl_t
#define bustype_CAVM_DPIX_PF_MSIX_VECX_CTL(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_PF_MSIX_VECX_CTL(a,b) "DPIX_PF_MSIX_VECX_CTL"
#define device_bar_CAVM_DPIX_PF_MSIX_VECX_CTL(a,b) 0x4 /* PF_BAR4 */
#define busnum_CAVM_DPIX_PF_MSIX_VECX_CTL(a,b) (a)
#define arguments_CAVM_DPIX_PF_MSIX_VECX_CTL(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_pf_ras
 *
 * DPI PF RAS Register
 * This register contains PF RAS error flags for DPI.
 */
union cavm_dpix_pf_ras
{
    uint64_t u;
    struct cavm_dpix_pf_ras_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_3_63         : 61;
        uint64_t ncb_cmd_psn           : 1;  /**< [  2:  2](R/W1C/H) Poison received on a NCB instruction response. */
        uint64_t ncb_dat_psn           : 1;  /**< [  1:  1](R/W1C/H) Poison received on a NCB data response. */
        uint64_t ebi_dat_psn           : 1;  /**< [  0:  0](R/W1C/H) Poison returned on a EBI data response. */
#else /* Word 0 - Little Endian */
        uint64_t ebi_dat_psn           : 1;  /**< [  0:  0](R/W1C/H) Poison returned on a EBI data response. */
        uint64_t ncb_dat_psn           : 1;  /**< [  1:  1](R/W1C/H) Poison received on a NCB data response. */
        uint64_t ncb_cmd_psn           : 1;  /**< [  2:  2](R/W1C/H) Poison received on a NCB instruction response. */
        uint64_t reserved_3_63         : 61;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_pf_ras_s cn; */
};
typedef union cavm_dpix_pf_ras cavm_dpix_pf_ras_t;

static inline uint64_t CAVM_DPIX_PF_RAS(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_PF_RAS(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e000004308ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_PF_RAS", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_PF_RAS(a) cavm_dpix_pf_ras_t
#define bustype_CAVM_DPIX_PF_RAS(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_PF_RAS(a) "DPIX_PF_RAS"
#define device_bar_CAVM_DPIX_PF_RAS(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_PF_RAS(a) (a)
#define arguments_CAVM_DPIX_PF_RAS(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_pf_ras_ena_w1c
 *
 * DPI PF RAS Register
 * This register clears interrupt enable bits.
 */
union cavm_dpix_pf_ras_ena_w1c
{
    uint64_t u;
    struct cavm_dpix_pf_ras_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_3_63         : 61;
        uint64_t ncb_cmd_psn           : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for DPI(0)_PF_RAS[NCB_CMD_PSN]. */
        uint64_t ncb_dat_psn           : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for DPI(0)_PF_RAS[NCB_DAT_PSN]. */
        uint64_t ebi_dat_psn           : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_PF_RAS[EBI_DAT_PSN]. */
#else /* Word 0 - Little Endian */
        uint64_t ebi_dat_psn           : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_PF_RAS[EBI_DAT_PSN]. */
        uint64_t ncb_dat_psn           : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for DPI(0)_PF_RAS[NCB_DAT_PSN]. */
        uint64_t ncb_cmd_psn           : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for DPI(0)_PF_RAS[NCB_CMD_PSN]. */
        uint64_t reserved_3_63         : 61;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_pf_ras_ena_w1c_s cn; */
};
typedef union cavm_dpix_pf_ras_ena_w1c cavm_dpix_pf_ras_ena_w1c_t;

static inline uint64_t CAVM_DPIX_PF_RAS_ENA_W1C(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_PF_RAS_ENA_W1C(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e000004318ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_PF_RAS_ENA_W1C", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_PF_RAS_ENA_W1C(a) cavm_dpix_pf_ras_ena_w1c_t
#define bustype_CAVM_DPIX_PF_RAS_ENA_W1C(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_PF_RAS_ENA_W1C(a) "DPIX_PF_RAS_ENA_W1C"
#define device_bar_CAVM_DPIX_PF_RAS_ENA_W1C(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_PF_RAS_ENA_W1C(a) (a)
#define arguments_CAVM_DPIX_PF_RAS_ENA_W1C(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_pf_ras_ena_w1s
 *
 * DPI PF RAS Register
 * This register sets interrupt enable bits.
 */
union cavm_dpix_pf_ras_ena_w1s
{
    uint64_t u;
    struct cavm_dpix_pf_ras_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_3_63         : 61;
        uint64_t ncb_cmd_psn           : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for DPI(0)_PF_RAS[NCB_CMD_PSN]. */
        uint64_t ncb_dat_psn           : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for DPI(0)_PF_RAS[NCB_DAT_PSN]. */
        uint64_t ebi_dat_psn           : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_PF_RAS[EBI_DAT_PSN]. */
#else /* Word 0 - Little Endian */
        uint64_t ebi_dat_psn           : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_PF_RAS[EBI_DAT_PSN]. */
        uint64_t ncb_dat_psn           : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for DPI(0)_PF_RAS[NCB_DAT_PSN]. */
        uint64_t ncb_cmd_psn           : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for DPI(0)_PF_RAS[NCB_CMD_PSN]. */
        uint64_t reserved_3_63         : 61;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_pf_ras_ena_w1s_s cn; */
};
typedef union cavm_dpix_pf_ras_ena_w1s cavm_dpix_pf_ras_ena_w1s_t;

static inline uint64_t CAVM_DPIX_PF_RAS_ENA_W1S(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_PF_RAS_ENA_W1S(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e000004320ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_PF_RAS_ENA_W1S", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_PF_RAS_ENA_W1S(a) cavm_dpix_pf_ras_ena_w1s_t
#define bustype_CAVM_DPIX_PF_RAS_ENA_W1S(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_PF_RAS_ENA_W1S(a) "DPIX_PF_RAS_ENA_W1S"
#define device_bar_CAVM_DPIX_PF_RAS_ENA_W1S(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_PF_RAS_ENA_W1S(a) (a)
#define arguments_CAVM_DPIX_PF_RAS_ENA_W1S(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_pf_ras_w1s
 *
 * DPI PF RAS Register
 * This register sets interrupt bits.
 */
union cavm_dpix_pf_ras_w1s
{
    uint64_t u;
    struct cavm_dpix_pf_ras_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_3_63         : 61;
        uint64_t ncb_cmd_psn           : 1;  /**< [  2:  2](R/W1S/H) Reads or sets DPI(0)_PF_RAS[NCB_CMD_PSN]. */
        uint64_t ncb_dat_psn           : 1;  /**< [  1:  1](R/W1S/H) Reads or sets DPI(0)_PF_RAS[NCB_DAT_PSN]. */
        uint64_t ebi_dat_psn           : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_PF_RAS[EBI_DAT_PSN]. */
#else /* Word 0 - Little Endian */
        uint64_t ebi_dat_psn           : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_PF_RAS[EBI_DAT_PSN]. */
        uint64_t ncb_dat_psn           : 1;  /**< [  1:  1](R/W1S/H) Reads or sets DPI(0)_PF_RAS[NCB_DAT_PSN]. */
        uint64_t ncb_cmd_psn           : 1;  /**< [  2:  2](R/W1S/H) Reads or sets DPI(0)_PF_RAS[NCB_CMD_PSN]. */
        uint64_t reserved_3_63         : 61;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_pf_ras_w1s_s cn; */
};
typedef union cavm_dpix_pf_ras_w1s cavm_dpix_pf_ras_w1s_t;

static inline uint64_t CAVM_DPIX_PF_RAS_W1S(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_PF_RAS_W1S(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e000004310ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_PF_RAS_W1S", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_PF_RAS_W1S(a) cavm_dpix_pf_ras_w1s_t
#define bustype_CAVM_DPIX_PF_RAS_W1S(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_PF_RAS_W1S(a) "DPIX_PF_RAS_W1S"
#define device_bar_CAVM_DPIX_PF_RAS_W1S(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_PF_RAS_W1S(a) (a)
#define arguments_CAVM_DPIX_PF_RAS_W1S(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_pkt_err_rsp
 *
 * DPI Packet Error Response Register
 */
union cavm_dpix_pkt_err_rsp
{
    uint64_t u;
    struct cavm_dpix_pkt_err_rsp_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t pkterr                : 1;  /**< [  0:  0](R/W1C/H) Indicates that an ErrorResponse was received from the I/O subsystem. */
#else /* Word 0 - Little Endian */
        uint64_t pkterr                : 1;  /**< [  0:  0](R/W1C/H) Indicates that an ErrorResponse was received from the I/O subsystem. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_pkt_err_rsp_s cn; */
};
typedef union cavm_dpix_pkt_err_rsp cavm_dpix_pkt_err_rsp_t;

static inline uint64_t CAVM_DPIX_PKT_ERR_RSP(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_PKT_ERR_RSP(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004098ll + 0x1000000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e000004098ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_PKT_ERR_RSP", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_PKT_ERR_RSP(a) cavm_dpix_pkt_err_rsp_t
#define bustype_CAVM_DPIX_PKT_ERR_RSP(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_PKT_ERR_RSP(a) "DPIX_PKT_ERR_RSP"
#define device_bar_CAVM_DPIX_PKT_ERR_RSP(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_PKT_ERR_RSP(a) (a)
#define arguments_CAVM_DPIX_PKT_ERR_RSP(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_req_err_rsp
 *
 * DPI Instruction Queue Error Response Register
 */
union cavm_dpix_req_err_rsp
{
    uint64_t u;
    struct cavm_dpix_req_err_rsp_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_8_63         : 56;
        uint64_t qerr                  : 8;  /**< [  7:  0](R/W1C/H) Indicates which instruction queue received an ErrorResponse from the I/O subsystem.
                                                                 Software must clear the bit before the corresponding instruction queue will continue
                                                                 processing instructions if DPI()_REQ_ERR_RSP_EN[EN] is set. */
#else /* Word 0 - Little Endian */
        uint64_t qerr                  : 8;  /**< [  7:  0](R/W1C/H) Indicates which instruction queue received an ErrorResponse from the I/O subsystem.
                                                                 Software must clear the bit before the corresponding instruction queue will continue
                                                                 processing instructions if DPI()_REQ_ERR_RSP_EN[EN] is set. */
        uint64_t reserved_8_63         : 56;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_req_err_rsp_s cn; */
};
typedef union cavm_dpix_req_err_rsp cavm_dpix_req_err_rsp_t;

static inline uint64_t CAVM_DPIX_REQ_ERR_RSP(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_REQ_ERR_RSP(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004078ll + 0x1000000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e000004078ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_REQ_ERR_RSP", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_REQ_ERR_RSP(a) cavm_dpix_req_err_rsp_t
#define bustype_CAVM_DPIX_REQ_ERR_RSP(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_REQ_ERR_RSP(a) "DPIX_REQ_ERR_RSP"
#define device_bar_CAVM_DPIX_REQ_ERR_RSP(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_REQ_ERR_RSP(a) (a)
#define arguments_CAVM_DPIX_REQ_ERR_RSP(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_req_err_rsp_en
 *
 * DPI Instruction Queue ErrorResponse Enable Register
 */
union cavm_dpix_req_err_rsp_en
{
    uint64_t u;
    struct cavm_dpix_req_err_rsp_en_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_8_63         : 56;
        uint64_t en                    : 8;  /**< [  7:  0](R/W) Indicates which instruction queues should stop dispatching instructions when an
                                                                 ErrorResponse is received from the I/O subsystem. */
#else /* Word 0 - Little Endian */
        uint64_t en                    : 8;  /**< [  7:  0](R/W) Indicates which instruction queues should stop dispatching instructions when an
                                                                 ErrorResponse is received from the I/O subsystem. */
        uint64_t reserved_8_63         : 56;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_req_err_rsp_en_s cn; */
};
typedef union cavm_dpix_req_err_rsp_en cavm_dpix_req_err_rsp_en_t;

static inline uint64_t CAVM_DPIX_REQ_ERR_RSP_EN(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_REQ_ERR_RSP_EN(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004088ll + 0x1000000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e000004088ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_REQ_ERR_RSP_EN", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_REQ_ERR_RSP_EN(a) cavm_dpix_req_err_rsp_en_t
#define bustype_CAVM_DPIX_REQ_ERR_RSP_EN(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_REQ_ERR_RSP_EN(a) "DPIX_REQ_ERR_RSP_EN"
#define device_bar_CAVM_DPIX_REQ_ERR_RSP_EN(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_REQ_ERR_RSP_EN(a) (a)
#define arguments_CAVM_DPIX_REQ_ERR_RSP_EN(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_reqq#_int
 *
 * DPI DMA Per Instruction Queue Interrupt Register
 * This register contains per-request queue error interrupts.
 */
union cavm_dpix_reqqx_int
{
    uint64_t u;
    struct cavm_dpix_reqqx_int_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_8_63         : 56;
        uint64_t instr_psn             : 1;  /**< [  7:  7](R/W1C/H) NCB poison on instruction read. This error will also disable the corresponding instruction
                                                                 queue (DPI()_VDMA()_EN[QEN]) and must be reset with DPI()_DMA()_QRST[QRST] before
                                                                 reenabling. */
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1C/H) Checks on Instruction words during fill detected an error. Checks include:
                                                                   * NFST or NLST value in header is zero.
                                                                   * Read or write pointer length is zero.
                                                                   * Total byte count for all read pointers is not equal to total for all write pointers.

                                                                 This error will also disable the corresponding instruction queue (DPI()_VDMA()_EN[QEN])
                                                                 and must be reset with DPI()_DMA()_QRST[QRST] before reenabling. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1C/H) Instruction fill returned all zeros for starting address of next chunk. This error will
                                                                 also disable the corresponding instruction queue (DPI()_VDMA()_EN[QEN]) and must be reset
                                                                 with DPI()_DMA()_QRST[QRST] before reenabling. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1C/H) Instruction fill doorbell overflow error. This error will also disable the corresponding
                                                                 instruction queue (DPI()_VDMA()_EN[QEN]) and must be reset with DPI()_DMA()_QRST[QRST]
                                                                 before reenabling. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1C/H) NCB fault on completion status or zero byte write. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1C/H) NCB fault on DMA data write. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1C/H) NCB fault on DMA data read. */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1C/H) NCB fault on instruction read. This error will also disable the corresponding instruction
                                                                 queue (DPI()_VDMA()_EN[QEN]) and must be reset with DPI()_DMA()_QRST[QRST] before
                                                                 reenabling. */
#else /* Word 0 - Little Endian */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1C/H) NCB fault on instruction read. This error will also disable the corresponding instruction
                                                                 queue (DPI()_VDMA()_EN[QEN]) and must be reset with DPI()_DMA()_QRST[QRST] before
                                                                 reenabling. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1C/H) NCB fault on DMA data read. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1C/H) NCB fault on DMA data write. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1C/H) NCB fault on completion status or zero byte write. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1C/H) Instruction fill doorbell overflow error. This error will also disable the corresponding
                                                                 instruction queue (DPI()_VDMA()_EN[QEN]) and must be reset with DPI()_DMA()_QRST[QRST]
                                                                 before reenabling. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1C/H) Instruction fill returned all zeros for starting address of next chunk. This error will
                                                                 also disable the corresponding instruction queue (DPI()_VDMA()_EN[QEN]) and must be reset
                                                                 with DPI()_DMA()_QRST[QRST] before reenabling. */
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1C/H) Checks on Instruction words during fill detected an error. Checks include:
                                                                   * NFST or NLST value in header is zero.
                                                                   * Read or write pointer length is zero.
                                                                   * Total byte count for all read pointers is not equal to total for all write pointers.

                                                                 This error will also disable the corresponding instruction queue (DPI()_VDMA()_EN[QEN])
                                                                 and must be reset with DPI()_DMA()_QRST[QRST] before reenabling. */
        uint64_t instr_psn             : 1;  /**< [  7:  7](R/W1C/H) NCB poison on instruction read. This error will also disable the corresponding instruction
                                                                 queue (DPI()_VDMA()_EN[QEN]) and must be reset with DPI()_DMA()_QRST[QRST] before
                                                                 reenabling. */
        uint64_t reserved_8_63         : 56;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_reqqx_int_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_7_63         : 57;
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1C/H) Checks on Instruction words during fill detected an error. Checks include:
                                                                   * NFST or NLST value in header is zero.
                                                                   * Read or write pointer length is zero.
                                                                   * Total byte count for all read pointers is not equal to total for all write pointers.

                                                                 This error will also disable the corresponding instruction queue (DPI()_VDMA()_EN[QEN])
                                                                 and must be reset with DPI()_DMA()_QRST[QRST] before reenabling. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1C/H) Instruction fill returned all zeros for starting address of next chunk. This error will
                                                                 also disable the corresponding instruction queue (DPI()_VDMA()_EN[QEN]) and must be reset
                                                                 with DPI()_DMA()_QRST[QRST] before reenabling. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1C/H) Instruction fill doorbell overflow error. This error will also disable the corresponding
                                                                 instruction queue (DPI()_VDMA()_EN[QEN]) and must be reset with DPI()_DMA()_QRST[QRST]
                                                                 before reenabling. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1C/H) NCB fault on completion status or zero byte write. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1C/H) NCB fault on DMA data write. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1C/H) NCB fault on DMA data read. */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1C/H) NCB fault on instruction read. This error will also disable the corresponding instruction
                                                                 queue (DPI()_VDMA()_EN[QEN]) and must be reset with DPI()_DMA()_QRST[QRST] before
                                                                 reenabling. */
#else /* Word 0 - Little Endian */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1C/H) NCB fault on instruction read. This error will also disable the corresponding instruction
                                                                 queue (DPI()_VDMA()_EN[QEN]) and must be reset with DPI()_DMA()_QRST[QRST] before
                                                                 reenabling. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1C/H) NCB fault on DMA data read. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1C/H) NCB fault on DMA data write. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1C/H) NCB fault on completion status or zero byte write. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1C/H) Instruction fill doorbell overflow error. This error will also disable the corresponding
                                                                 instruction queue (DPI()_VDMA()_EN[QEN]) and must be reset with DPI()_DMA()_QRST[QRST]
                                                                 before reenabling. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1C/H) Instruction fill returned all zeros for starting address of next chunk. This error will
                                                                 also disable the corresponding instruction queue (DPI()_VDMA()_EN[QEN]) and must be reset
                                                                 with DPI()_DMA()_QRST[QRST] before reenabling. */
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1C/H) Checks on Instruction words during fill detected an error. Checks include:
                                                                   * NFST or NLST value in header is zero.
                                                                   * Read or write pointer length is zero.
                                                                   * Total byte count for all read pointers is not equal to total for all write pointers.

                                                                 This error will also disable the corresponding instruction queue (DPI()_VDMA()_EN[QEN])
                                                                 and must be reset with DPI()_DMA()_QRST[QRST] before reenabling. */
        uint64_t reserved_7_63         : 57;
#endif /* Word 0 - End */
    } cn8;
    /* struct cavm_dpix_reqqx_int_s cn9; */
};
typedef union cavm_dpix_reqqx_int cavm_dpix_reqqx_int_t;

static inline uint64_t CAVM_DPIX_REQQX_INT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_REQQX_INT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e000006600ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e000006600ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_REQQX_INT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_REQQX_INT(a,b) cavm_dpix_reqqx_int_t
#define bustype_CAVM_DPIX_REQQX_INT(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_REQQX_INT(a,b) "DPIX_REQQX_INT"
#define device_bar_CAVM_DPIX_REQQX_INT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_REQQX_INT(a,b) (a)
#define arguments_CAVM_DPIX_REQQX_INT(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_reqq#_int_ena_w1c
 *
 * DPI DMA Per Instruction Queue Interrupt Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_dpix_reqqx_int_ena_w1c
{
    uint64_t u;
    struct cavm_dpix_reqqx_int_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_8_63         : 56;
        uint64_t instr_psn             : 1;  /**< [  7:  7](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INSTR_PSN]. */
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INST_FILL_INVAL]. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INST_ADDR_NULL]. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INST_DBO]. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[CSFLT]. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[WRFLT]. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[RDFLT]. */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INSTRFLT]. */
#else /* Word 0 - Little Endian */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INSTRFLT]. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[RDFLT]. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[WRFLT]. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[CSFLT]. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INST_DBO]. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INST_ADDR_NULL]. */
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INST_FILL_INVAL]. */
        uint64_t instr_psn             : 1;  /**< [  7:  7](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INSTR_PSN]. */
        uint64_t reserved_8_63         : 56;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_reqqx_int_ena_w1c_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_7_63         : 57;
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INST_FILL_INVAL]. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INST_ADDR_NULL]. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INST_DBO]. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[CSFLT]. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[WRFLT]. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[RDFLT]. */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INSTRFLT]. */
#else /* Word 0 - Little Endian */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INSTRFLT]. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[RDFLT]. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[WRFLT]. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[CSFLT]. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INST_DBO]. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INST_ADDR_NULL]. */
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1C/H) Reads or clears enable for DPI(0)_REQQ(0..7)_INT[INST_FILL_INVAL]. */
        uint64_t reserved_7_63         : 57;
#endif /* Word 0 - End */
    } cn8;
    /* struct cavm_dpix_reqqx_int_ena_w1c_s cn9; */
};
typedef union cavm_dpix_reqqx_int_ena_w1c cavm_dpix_reqqx_int_ena_w1c_t;

static inline uint64_t CAVM_DPIX_REQQX_INT_ENA_W1C(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_REQQX_INT_ENA_W1C(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e000006680ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e000006680ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_REQQX_INT_ENA_W1C", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_REQQX_INT_ENA_W1C(a,b) cavm_dpix_reqqx_int_ena_w1c_t
#define bustype_CAVM_DPIX_REQQX_INT_ENA_W1C(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_REQQX_INT_ENA_W1C(a,b) "DPIX_REQQX_INT_ENA_W1C"
#define device_bar_CAVM_DPIX_REQQX_INT_ENA_W1C(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_REQQX_INT_ENA_W1C(a,b) (a)
#define arguments_CAVM_DPIX_REQQX_INT_ENA_W1C(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_reqq#_int_ena_w1s
 *
 * DPI DMA Per Instruction Queue Interrupt Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_dpix_reqqx_int_ena_w1s
{
    uint64_t u;
    struct cavm_dpix_reqqx_int_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_8_63         : 56;
        uint64_t instr_psn             : 1;  /**< [  7:  7](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INSTR_PSN]. */
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INST_FILL_INVAL]. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INST_ADDR_NULL]. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INST_DBO]. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[CSFLT]. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[WRFLT]. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[RDFLT]. */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INSTRFLT]. */
#else /* Word 0 - Little Endian */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INSTRFLT]. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[RDFLT]. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[WRFLT]. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[CSFLT]. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INST_DBO]. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INST_ADDR_NULL]. */
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INST_FILL_INVAL]. */
        uint64_t instr_psn             : 1;  /**< [  7:  7](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INSTR_PSN]. */
        uint64_t reserved_8_63         : 56;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_reqqx_int_ena_w1s_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_7_63         : 57;
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INST_FILL_INVAL]. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INST_ADDR_NULL]. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INST_DBO]. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[CSFLT]. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[WRFLT]. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[RDFLT]. */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INSTRFLT]. */
#else /* Word 0 - Little Endian */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INSTRFLT]. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[RDFLT]. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[WRFLT]. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[CSFLT]. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INST_DBO]. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INST_ADDR_NULL]. */
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1S/H) Reads or sets enable for DPI(0)_REQQ(0..7)_INT[INST_FILL_INVAL]. */
        uint64_t reserved_7_63         : 57;
#endif /* Word 0 - End */
    } cn8;
    /* struct cavm_dpix_reqqx_int_ena_w1s_s cn9; */
};
typedef union cavm_dpix_reqqx_int_ena_w1s cavm_dpix_reqqx_int_ena_w1s_t;

static inline uint64_t CAVM_DPIX_REQQX_INT_ENA_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_REQQX_INT_ENA_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e0000066c0ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e0000066c0ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_REQQX_INT_ENA_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_REQQX_INT_ENA_W1S(a,b) cavm_dpix_reqqx_int_ena_w1s_t
#define bustype_CAVM_DPIX_REQQX_INT_ENA_W1S(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_REQQX_INT_ENA_W1S(a,b) "DPIX_REQQX_INT_ENA_W1S"
#define device_bar_CAVM_DPIX_REQQX_INT_ENA_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_REQQX_INT_ENA_W1S(a,b) (a)
#define arguments_CAVM_DPIX_REQQX_INT_ENA_W1S(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_reqq#_int_w1s
 *
 * DPI DMA Per Instruction Queue Interrupt Set Register
 * This register sets interrupt bits.
 */
union cavm_dpix_reqqx_int_w1s
{
    uint64_t u;
    struct cavm_dpix_reqqx_int_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_8_63         : 56;
        uint64_t instr_psn             : 1;  /**< [  7:  7](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INSTR_PSN]. */
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INST_FILL_INVAL]. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INST_ADDR_NULL]. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INST_DBO]. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[CSFLT]. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[WRFLT]. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[RDFLT]. */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INSTRFLT]. */
#else /* Word 0 - Little Endian */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INSTRFLT]. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[RDFLT]. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[WRFLT]. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[CSFLT]. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INST_DBO]. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INST_ADDR_NULL]. */
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INST_FILL_INVAL]. */
        uint64_t instr_psn             : 1;  /**< [  7:  7](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INSTR_PSN]. */
        uint64_t reserved_8_63         : 56;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_reqqx_int_w1s_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_7_63         : 57;
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INST_FILL_INVAL]. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INST_ADDR_NULL]. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INST_DBO]. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[CSFLT]. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[WRFLT]. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[RDFLT]. */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INSTRFLT]. */
#else /* Word 0 - Little Endian */
        uint64_t instrflt              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INSTRFLT]. */
        uint64_t rdflt                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[RDFLT]. */
        uint64_t wrflt                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[WRFLT]. */
        uint64_t csflt                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[CSFLT]. */
        uint64_t inst_dbo              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INST_DBO]. */
        uint64_t inst_addr_null        : 1;  /**< [  5:  5](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INST_ADDR_NULL]. */
        uint64_t inst_fill_inval       : 1;  /**< [  6:  6](R/W1S/H) Reads or sets DPI(0)_REQQ(0..7)_INT[INST_FILL_INVAL]. */
        uint64_t reserved_7_63         : 57;
#endif /* Word 0 - End */
    } cn8;
    /* struct cavm_dpix_reqqx_int_w1s_s cn9; */
};
typedef union cavm_dpix_reqqx_int_w1s cavm_dpix_reqqx_int_w1s_t;

static inline uint64_t CAVM_DPIX_REQQX_INT_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_REQQX_INT_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e000006640ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e000006640ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_REQQX_INT_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_REQQX_INT_W1S(a,b) cavm_dpix_reqqx_int_w1s_t
#define bustype_CAVM_DPIX_REQQX_INT_W1S(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_REQQX_INT_W1S(a,b) "DPIX_REQQX_INT_W1S"
#define device_bar_CAVM_DPIX_REQQX_INT_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_REQQX_INT_W1S(a,b) (a)
#define arguments_CAVM_DPIX_REQQX_INT_W1S(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_sbe_int
 *
 * DPI ECC SBE Interrupt Register
 * This register contains ECC single-bit error interrupt summary bits.
 */
union cavm_dpix_sbe_int
{
    uint64_t u;
    struct cavm_dpix_sbe_int_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t rdb_sbe               : 1;  /**< [  0:  0](R/W1C/H) Set when a single-bit error is detected in the RDB RAM. */
#else /* Word 0 - Little Endian */
        uint64_t rdb_sbe               : 1;  /**< [  0:  0](R/W1C/H) Set when a single-bit error is detected in the RDB RAM. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_sbe_int_s cn; */
};
typedef union cavm_dpix_sbe_int cavm_dpix_sbe_int_t;

static inline uint64_t CAVM_DPIX_SBE_INT(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_SBE_INT(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004188ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_SBE_INT", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_SBE_INT(a) cavm_dpix_sbe_int_t
#define bustype_CAVM_DPIX_SBE_INT(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_SBE_INT(a) "DPIX_SBE_INT"
#define device_bar_CAVM_DPIX_SBE_INT(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_SBE_INT(a) (a)
#define arguments_CAVM_DPIX_SBE_INT(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_sbe_int_ena_w1c
 *
 * DPI SBE Interrupt Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_dpix_sbe_int_ena_w1c
{
    uint64_t u;
    struct cavm_dpix_sbe_int_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t rdb_sbe               : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_SBE_INT[RDB_SBE]. */
#else /* Word 0 - Little Endian */
        uint64_t rdb_sbe               : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_SBE_INT[RDB_SBE]. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_sbe_int_ena_w1c_s cn; */
};
typedef union cavm_dpix_sbe_int_ena_w1c cavm_dpix_sbe_int_ena_w1c_t;

static inline uint64_t CAVM_DPIX_SBE_INT_ENA_W1C(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_SBE_INT_ENA_W1C(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004198ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_SBE_INT_ENA_W1C", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_SBE_INT_ENA_W1C(a) cavm_dpix_sbe_int_ena_w1c_t
#define bustype_CAVM_DPIX_SBE_INT_ENA_W1C(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_SBE_INT_ENA_W1C(a) "DPIX_SBE_INT_ENA_W1C"
#define device_bar_CAVM_DPIX_SBE_INT_ENA_W1C(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_SBE_INT_ENA_W1C(a) (a)
#define arguments_CAVM_DPIX_SBE_INT_ENA_W1C(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_sbe_int_ena_w1s
 *
 * DPI SBE Interrupt Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_dpix_sbe_int_ena_w1s
{
    uint64_t u;
    struct cavm_dpix_sbe_int_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t rdb_sbe               : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_SBE_INT[RDB_SBE]. */
#else /* Word 0 - Little Endian */
        uint64_t rdb_sbe               : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_SBE_INT[RDB_SBE]. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_sbe_int_ena_w1s_s cn; */
};
typedef union cavm_dpix_sbe_int_ena_w1s cavm_dpix_sbe_int_ena_w1s_t;

static inline uint64_t CAVM_DPIX_SBE_INT_ENA_W1S(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_SBE_INT_ENA_W1S(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e0000041a0ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_SBE_INT_ENA_W1S", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_SBE_INT_ENA_W1S(a) cavm_dpix_sbe_int_ena_w1s_t
#define bustype_CAVM_DPIX_SBE_INT_ENA_W1S(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_SBE_INT_ENA_W1S(a) "DPIX_SBE_INT_ENA_W1S"
#define device_bar_CAVM_DPIX_SBE_INT_ENA_W1S(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_SBE_INT_ENA_W1S(a) (a)
#define arguments_CAVM_DPIX_SBE_INT_ENA_W1S(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_sbe_int_w1s
 *
 * DPI SBE Interrupt Set Register
 * This register sets interrupt bits.
 */
union cavm_dpix_sbe_int_w1s
{
    uint64_t u;
    struct cavm_dpix_sbe_int_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t rdb_sbe               : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_SBE_INT[RDB_SBE]. */
#else /* Word 0 - Little Endian */
        uint64_t rdb_sbe               : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_SBE_INT[RDB_SBE]. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_sbe_int_w1s_s cn; */
};
typedef union cavm_dpix_sbe_int_w1s cavm_dpix_sbe_int_w1s_t;

static inline uint64_t CAVM_DPIX_SBE_INT_W1S(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_SBE_INT_W1S(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x86e000004190ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("DPIX_SBE_INT_W1S", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_SBE_INT_W1S(a) cavm_dpix_sbe_int_w1s_t
#define bustype_CAVM_DPIX_SBE_INT_W1S(a) CSR_TYPE_NCB
#define basename_CAVM_DPIX_SBE_INT_W1S(a) "DPIX_SBE_INT_W1S"
#define device_bar_CAVM_DPIX_SBE_INT_W1S(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_SBE_INT_W1S(a) (a)
#define arguments_CAVM_DPIX_SBE_INT_W1S(a) (a),-1,-1,-1

/**
 * Register (NCB) dpi#_sli_prt#_cfg
 *
 * DPI SLI Port Configuration Register
 * This register configures the max read request size, max payload size, and max number of SLI
 * tags in use.
 */
union cavm_dpix_sli_prtx_cfg
{
    uint64_t u;
    struct cavm_dpix_sli_prtx_cfg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_25_63        : 39;
        uint64_t halt                  : 1;  /**< [ 24: 24](RO/H) When set, HALT indicates that the MAC has detected a reset condition. No further
                                                                 instructions that reference the MAC from any instruction queue will be issued until the
                                                                 MAC comes out of reset and HALT is cleared in SLI()_M2S_MAC()_CTL[DIS_PORT]. */
        uint64_t reserved_17_23        : 7;
        uint64_t exact_rd_dis          : 1;  /**< [ 16: 16](R/W) Exact read mode disable.
                                                                 0 = DPI will issue a 32-bit read request to the PEM when the size and alignment
                                                                 ensure that the request will not cross a 32-bit boundary.
                                                                 1 = DPI will always use a minimum 64-bit read request to the PEM. */
        uint64_t reserved_15           : 1;
        uint64_t molr                  : 7;  /**< [ 14:  8](R/W) Maximum outstanding load requests. Limits the number of outstanding load requests on the
                                                                 port by restricting the number of tags used by the SLI to track load responses. This value
                                                                 can range from 1 to 64 (maximum of 32 if port not configured for eight lanes). Setting MOLR
                                                                 to a value of zero halts all read traffic to the port. There are no restrictions on when this
                                                                 value can be changed. */
        uint64_t mps_lim               : 1;  /**< [  7:  7](R/W) MAC memory space write requests cannot cross the (naturally-aligned) MPS boundary.
                                                                 When clear, DPI is allowed to issue a MAC memory- space read that crosses the naturally-
                                                                 aligned boundary of size defined by MPS. (DPI will still only cross the boundary when it
                                                                 would eliminate a write by doing so.)

                                                                 When set, DPI will never issue a MAC memory space write that crosses the naturally-aligned
                                                                 boundary of size defined by MPS. */
        uint64_t mps                   : 3;  /**< [  6:  4](R/W) Maximum payload size.
                                                                 0x0 = 128 bytes.
                                                                 0x1 = 256 bytes.
                                                                 0x2 = 512 bytes.

                                                                 Larger sizes are not supported. If MPS = 0x2, outbound packet writes will use the 512B
                                                                 size but DMA is limited to 256B.

                                                                 The MPS size must not exceed the size selected by PCIE*_CFG030[MPS]. */
        uint64_t mrrs_lim              : 1;  /**< [  3:  3](R/W) MAC memory space read requests cannot cross the (naturally-aligned) MRRS boundary.

                                                                 When clear, DPI is allowed to issue a MAC memory-space read that crosses the naturally-
                                                                 aligned boundary of size defined by MRRS. (DPI will still only cross the boundary when it
                                                                 would eliminate a read by doing so.)

                                                                 When set, DPI will never issue a MAC memory space read that crosses the naturally-aligned
                                                                 boundary of size defined by MRRS. */
        uint64_t mrrs                  : 3;  /**< [  2:  0](R/W) Maximum read-request size.
                                                                 0x0 = 128 bytes.
                                                                 0x1 = 256 bytes.
                                                                 0x2 = 512 bytes.
                                                                 0x3 = 1024 bytes.

                                                                 Larger sizes are not supported.

                                                                 The MRRS size must not exceed the size selected by PCIE*_CFG030[MRRS]. */
#else /* Word 0 - Little Endian */
        uint64_t mrrs                  : 3;  /**< [  2:  0](R/W) Maximum read-request size.
                                                                 0x0 = 128 bytes.
                                                                 0x1 = 256 bytes.
                                                                 0x2 = 512 bytes.
                                                                 0x3 = 1024 bytes.

                                                                 Larger sizes are not supported.

                                                                 The MRRS size must not exceed the size selected by PCIE*_CFG030[MRRS]. */
        uint64_t mrrs_lim              : 1;  /**< [  3:  3](R/W) MAC memory space read requests cannot cross the (naturally-aligned) MRRS boundary.

                                                                 When clear, DPI is allowed to issue a MAC memory-space read that crosses the naturally-
                                                                 aligned boundary of size defined by MRRS. (DPI will still only cross the boundary when it
                                                                 would eliminate a read by doing so.)

                                                                 When set, DPI will never issue a MAC memory space read that crosses the naturally-aligned
                                                                 boundary of size defined by MRRS. */
        uint64_t mps                   : 3;  /**< [  6:  4](R/W) Maximum payload size.
                                                                 0x0 = 128 bytes.
                                                                 0x1 = 256 bytes.
                                                                 0x2 = 512 bytes.

                                                                 Larger sizes are not supported. If MPS = 0x2, outbound packet writes will use the 512B
                                                                 size but DMA is limited to 256B.

                                                                 The MPS size must not exceed the size selected by PCIE*_CFG030[MPS]. */
        uint64_t mps_lim               : 1;  /**< [  7:  7](R/W) MAC memory space write requests cannot cross the (naturally-aligned) MPS boundary.
                                                                 When clear, DPI is allowed to issue a MAC memory- space read that crosses the naturally-
                                                                 aligned boundary of size defined by MPS. (DPI will still only cross the boundary when it
                                                                 would eliminate a write by doing so.)

                                                                 When set, DPI will never issue a MAC memory space write that crosses the naturally-aligned
                                                                 boundary of size defined by MPS. */
        uint64_t molr                  : 7;  /**< [ 14:  8](R/W) Maximum outstanding load requests. Limits the number of outstanding load requests on the
                                                                 port by restricting the number of tags used by the SLI to track load responses. This value
                                                                 can range from 1 to 64 (maximum of 32 if port not configured for eight lanes). Setting MOLR
                                                                 to a value of zero halts all read traffic to the port. There are no restrictions on when this
                                                                 value can be changed. */
        uint64_t reserved_15           : 1;
        uint64_t exact_rd_dis          : 1;  /**< [ 16: 16](R/W) Exact read mode disable.
                                                                 0 = DPI will issue a 32-bit read request to the PEM when the size and alignment
                                                                 ensure that the request will not cross a 32-bit boundary.
                                                                 1 = DPI will always use a minimum 64-bit read request to the PEM. */
        uint64_t reserved_17_23        : 7;
        uint64_t halt                  : 1;  /**< [ 24: 24](RO/H) When set, HALT indicates that the MAC has detected a reset condition. No further
                                                                 instructions that reference the MAC from any instruction queue will be issued until the
                                                                 MAC comes out of reset and HALT is cleared in SLI()_M2S_MAC()_CTL[DIS_PORT]. */
        uint64_t reserved_25_63        : 39;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_sli_prtx_cfg_s cn; */
};
typedef union cavm_dpix_sli_prtx_cfg cavm_dpix_sli_prtx_cfg_t;

static inline uint64_t CAVM_DPIX_SLI_PRTX_CFG(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_SLI_PRTX_CFG(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=3)))
        return 0x86e000004100ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3);
    __cavm_csr_fatal("DPIX_SLI_PRTX_CFG", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_SLI_PRTX_CFG(a,b) cavm_dpix_sli_prtx_cfg_t
#define bustype_CAVM_DPIX_SLI_PRTX_CFG(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_SLI_PRTX_CFG(a,b) "DPIX_SLI_PRTX_CFG"
#define device_bar_CAVM_DPIX_SLI_PRTX_CFG(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_SLI_PRTX_CFG(a,b) (a)
#define arguments_CAVM_DPIX_SLI_PRTX_CFG(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_sli_prt#_err
 *
 * DPI SLI Port Error Address Registers
 * This register logs the address associated with the reported SLI error response.
 */
union cavm_dpix_sli_prtx_err
{
    uint64_t u;
    struct cavm_dpix_sli_prtx_err_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t addr                  : 61; /**< [ 63:  3](RO/H) Address of the failed load request. Address is locked along with the
                                                                 DPI()_SLI_PRT()_ERR_INFO register. See the
                                                                 DPI()_SLI_PRT()_ERR_INFO[LOCK] description for further information. */
        uint64_t reserved_0_2          : 3;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_2          : 3;
        uint64_t addr                  : 61; /**< [ 63:  3](RO/H) Address of the failed load request. Address is locked along with the
                                                                 DPI()_SLI_PRT()_ERR_INFO register. See the
                                                                 DPI()_SLI_PRT()_ERR_INFO[LOCK] description for further information. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_sli_prtx_err_s cn; */
};
typedef union cavm_dpix_sli_prtx_err cavm_dpix_sli_prtx_err_t;

static inline uint64_t CAVM_DPIX_SLI_PRTX_ERR(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_SLI_PRTX_ERR(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=3)))
        return 0x86e000004120ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3);
    __cavm_csr_fatal("DPIX_SLI_PRTX_ERR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_SLI_PRTX_ERR(a,b) cavm_dpix_sli_prtx_err_t
#define bustype_CAVM_DPIX_SLI_PRTX_ERR(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_SLI_PRTX_ERR(a,b) "DPIX_SLI_PRTX_ERR"
#define device_bar_CAVM_DPIX_SLI_PRTX_ERR(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_SLI_PRTX_ERR(a,b) (a)
#define arguments_CAVM_DPIX_SLI_PRTX_ERR(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_sli_prt#_err_info
 *
 * DPI SLI MAC Port Error Info Registers
 * This register logs information associated with the reported SLI error response.
 */
union cavm_dpix_sli_prtx_err_info
{
    uint64_t u;
    struct cavm_dpix_sli_prtx_err_info_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t pvf                   : 16; /**< [ 31: 16](RO/H) Physical/virtual function that caused the ErrorResponse. DPI_DMA_FUNC_SEL_S format. */
        uint64_t reserved_9_15         : 7;
        uint64_t lock                  : 1;  /**< [  8:  8](RO/H) DPI()_SLI_PRT()_ERR and DPI()_SLI_PRT()_ERR_INFO have captured and locked contents.
                                                                 When CNXXXX first detects an ErrorResponse, the TYPE, REQQ, and ADDR of the error is saved
                                                                 and an internal lock state is set so the data associated with the initial error is
                                                                 preserved.

                                                                 Subsequent ErrorResponses will optionally raise an interrupt, but will not modify
                                                                 [TYPE], [REQQ], nor [ADDR] until the internal lock state is cleared.

                                                                 Software can clear the internal lock state by writing a one to the appropriate bit in either
                                                                 DPI()_REQ_ERR_RSP or DPI()_PKT_ERR_RSP depending on [TYPE]. Once the internal lock
                                                                 state is cleared, the next ErrorResponse will set [TYPE], [REQQ], nor [ADDR] for the new
                                                                 transaction. */
        uint64_t reserved_5_7          : 3;
        uint64_t type                  : 1;  /**< [  4:  4](RO/H) Type of transaction that caused the ErrorResponse.
                                                                 0 = DMA Instruction.
                                                                 1 = PKT Instruction. */
        uint64_t reserved_3            : 1;
        uint64_t reqq                  : 3;  /**< [  2:  0](RO/H) Request queue that made the failed load request. */
#else /* Word 0 - Little Endian */
        uint64_t reqq                  : 3;  /**< [  2:  0](RO/H) Request queue that made the failed load request. */
        uint64_t reserved_3            : 1;
        uint64_t type                  : 1;  /**< [  4:  4](RO/H) Type of transaction that caused the ErrorResponse.
                                                                 0 = DMA Instruction.
                                                                 1 = PKT Instruction. */
        uint64_t reserved_5_7          : 3;
        uint64_t lock                  : 1;  /**< [  8:  8](RO/H) DPI()_SLI_PRT()_ERR and DPI()_SLI_PRT()_ERR_INFO have captured and locked contents.
                                                                 When CNXXXX first detects an ErrorResponse, the TYPE, REQQ, and ADDR of the error is saved
                                                                 and an internal lock state is set so the data associated with the initial error is
                                                                 preserved.

                                                                 Subsequent ErrorResponses will optionally raise an interrupt, but will not modify
                                                                 [TYPE], [REQQ], nor [ADDR] until the internal lock state is cleared.

                                                                 Software can clear the internal lock state by writing a one to the appropriate bit in either
                                                                 DPI()_REQ_ERR_RSP or DPI()_PKT_ERR_RSP depending on [TYPE]. Once the internal lock
                                                                 state is cleared, the next ErrorResponse will set [TYPE], [REQQ], nor [ADDR] for the new
                                                                 transaction. */
        uint64_t reserved_9_15         : 7;
        uint64_t pvf                   : 16; /**< [ 31: 16](RO/H) Physical/virtual function that caused the ErrorResponse. DPI_DMA_FUNC_SEL_S format. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_sli_prtx_err_info_s cn; */
};
typedef union cavm_dpix_sli_prtx_err_info cavm_dpix_sli_prtx_err_info_t;

static inline uint64_t CAVM_DPIX_SLI_PRTX_ERR_INFO(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_SLI_PRTX_ERR_INFO(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=3)))
        return 0x86e000004140ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3);
    __cavm_csr_fatal("DPIX_SLI_PRTX_ERR_INFO", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_SLI_PRTX_ERR_INFO(a,b) cavm_dpix_sli_prtx_err_info_t
#define bustype_CAVM_DPIX_SLI_PRTX_ERR_INFO(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_SLI_PRTX_ERR_INFO(a,b) "DPIX_SLI_PRTX_ERR_INFO"
#define device_bar_CAVM_DPIX_SLI_PRTX_ERR_INFO(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_DPIX_SLI_PRTX_ERR_INFO(a,b) (a)
#define arguments_CAVM_DPIX_SLI_PRTX_ERR_INFO(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vdma#_cnt
 *
 * DPI DMA Per-Request Queue Instruction Completion Counter Register
 * These registers provide a per-request queue instruction completion counter.
 */
union cavm_dpix_vdmax_cnt
{
    uint64_t u;
    struct cavm_dpix_vdmax_cnt_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_16_63        : 48;
        uint64_t cnt                   : 16; /**< [ 15:  0](R/W/H) DPI DMA per-request queue instruction completion counter. DPI can increment a counter upon
                                                                 completion of a DPI DMA instruction. DPI subtracts the value written
                                                                 from [CNT] on a software write. A nonzero [CNT] asserts the corresponding
                                                                 DPI()_DMA_VF_INT bit.

                                                                 DPI increments the [CNT] for the corresponding request queue by one after completing
                                                                 a DPI_DMA_INSTR_HDR_S[PT]=DPI_HDR_PT_E::CNT DPI DMA instruction. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 16; /**< [ 15:  0](R/W/H) DPI DMA per-request queue instruction completion counter. DPI can increment a counter upon
                                                                 completion of a DPI DMA instruction. DPI subtracts the value written
                                                                 from [CNT] on a software write. A nonzero [CNT] asserts the corresponding
                                                                 DPI()_DMA_VF_INT bit.

                                                                 DPI increments the [CNT] for the corresponding request queue by one after completing
                                                                 a DPI_DMA_INSTR_HDR_S[PT]=DPI_HDR_PT_E::CNT DPI DMA instruction. */
        uint64_t reserved_16_63        : 48;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_vdmax_cnt_s cn8; */
    struct cavm_dpix_vdmax_cnt_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_16_63        : 48;
        uint64_t cnt                   : 16; /**< [ 15:  0](R/W/H) DPI DMA per-request queue instruction completion counter. DPI can increment a counter upon
                                                                 completion of a DPI DMA instruction. DPI subtracts the value written
                                                                 from [CNT] on a software write. A nonzero [CNT] asserts the corresponding
                                                                 DPI()_VF()_INT bit.

                                                                 DPI increments the [CNT] for the corresponding request queue by one after completing
                                                                 a DPI_DMA_INSTR_HDR_S[PT]=DPI_HDR_PT_E::CNT DPI DMA instruction. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 16; /**< [ 15:  0](R/W/H) DPI DMA per-request queue instruction completion counter. DPI can increment a counter upon
                                                                 completion of a DPI DMA instruction. DPI subtracts the value written
                                                                 from [CNT] on a software write. A nonzero [CNT] asserts the corresponding
                                                                 DPI()_VF()_INT bit.

                                                                 DPI increments the [CNT] for the corresponding request queue by one after completing
                                                                 a DPI_DMA_INSTR_HDR_S[PT]=DPI_HDR_PT_E::CNT DPI DMA instruction. */
        uint64_t reserved_16_63        : 48;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_dpix_vdmax_cnt cavm_dpix_vdmax_cnt_t;

static inline uint64_t CAVM_DPIX_VDMAX_CNT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VDMAX_CNT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e020000038ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e200000038ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_VDMAX_CNT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VDMAX_CNT(a,b) cavm_dpix_vdmax_cnt_t
#define bustype_CAVM_DPIX_VDMAX_CNT(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VDMAX_CNT(a,b) "DPIX_VDMAX_CNT"
#define device_bar_CAVM_DPIX_VDMAX_CNT(a,b) 0x0 /* VF_BAR0 */
#define busnum_CAVM_DPIX_VDMAX_CNT(a,b) (a)
#define arguments_CAVM_DPIX_VDMAX_CNT(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vdma#_counts
 *
 * DPI DMA Instruction Counts Registers
 * These registers provide values for determining the number of instructions in the local
 * instruction FIFO.
 */
union cavm_dpix_vdmax_counts
{
    uint64_t u;
    struct cavm_dpix_vdmax_counts_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_39_63        : 25;
        uint64_t fcnt                  : 7;  /**< [ 38: 32](RO/H) FIFO count. Specifies the number of words in the instruction FIFO locally cached within DPI. */
        uint64_t dbell                 : 32; /**< [ 31:  0](RO/H) Doorbell. Specifies the number of available words of instructions to read. */
#else /* Word 0 - Little Endian */
        uint64_t dbell                 : 32; /**< [ 31:  0](RO/H) Doorbell. Specifies the number of available words of instructions to read. */
        uint64_t fcnt                  : 7;  /**< [ 38: 32](RO/H) FIFO count. Specifies the number of words in the instruction FIFO locally cached within DPI. */
        uint64_t reserved_39_63        : 25;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_vdmax_counts_s cn; */
};
typedef union cavm_dpix_vdmax_counts cavm_dpix_vdmax_counts_t;

static inline uint64_t CAVM_DPIX_VDMAX_COUNTS(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VDMAX_COUNTS(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e020000020ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e200000020ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_VDMAX_COUNTS", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VDMAX_COUNTS(a,b) cavm_dpix_vdmax_counts_t
#define bustype_CAVM_DPIX_VDMAX_COUNTS(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VDMAX_COUNTS(a,b) "DPIX_VDMAX_COUNTS"
#define device_bar_CAVM_DPIX_VDMAX_COUNTS(a,b) 0x0 /* VF_BAR0 */
#define busnum_CAVM_DPIX_VDMAX_COUNTS(a,b) (a)
#define arguments_CAVM_DPIX_VDMAX_COUNTS(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vdma#_dbell
 *
 * DPI DMA Doorbell Registers
 * This is the door bell register for the eight DMA instruction queues.
 */
union cavm_dpix_vdmax_dbell
{
    uint64_t u;
    struct cavm_dpix_vdmax_dbell_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_16_63        : 48;
        uint64_t dbell                 : 16; /**< [ 15:  0](WO/H) Doorbell value. The value written to this register is added to the number of eight-byte
                                                                 words to be read and processed for the low-priority DMA queue. */
#else /* Word 0 - Little Endian */
        uint64_t dbell                 : 16; /**< [ 15:  0](WO/H) Doorbell value. The value written to this register is added to the number of eight-byte
                                                                 words to be read and processed for the low-priority DMA queue. */
        uint64_t reserved_16_63        : 48;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_vdmax_dbell_s cn8; */
    struct cavm_dpix_vdmax_dbell_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_16_63        : 48;
        uint64_t dbell                 : 16; /**< [ 15:  0](WO/H) Doorbell value. The value written to this register is added to the number of eight-byte
                                                                 words to be read and processed. */
#else /* Word 0 - Little Endian */
        uint64_t dbell                 : 16; /**< [ 15:  0](WO/H) Doorbell value. The value written to this register is added to the number of eight-byte
                                                                 words to be read and processed. */
        uint64_t reserved_16_63        : 48;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_dpix_vdmax_dbell cavm_dpix_vdmax_dbell_t;

static inline uint64_t CAVM_DPIX_VDMAX_DBELL(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VDMAX_DBELL(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e020000010ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e200000010ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_VDMAX_DBELL", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VDMAX_DBELL(a,b) cavm_dpix_vdmax_dbell_t
#define bustype_CAVM_DPIX_VDMAX_DBELL(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VDMAX_DBELL(a,b) "DPIX_VDMAX_DBELL"
#define device_bar_CAVM_DPIX_VDMAX_DBELL(a,b) 0x0 /* VF_BAR0 */
#define busnum_CAVM_DPIX_VDMAX_DBELL(a,b) (a)
#define arguments_CAVM_DPIX_VDMAX_DBELL(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vdma#_en
 *
 * DPI Request Global Enable Register
 */
union cavm_dpix_vdmax_en
{
    uint64_t u;
    struct cavm_dpix_vdmax_en_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t qen                   : 1;  /**< [  0:  0](R/W/H) Controls if instruction queue is enabled and can dispatch instructions to a requesting engine. */
#else /* Word 0 - Little Endian */
        uint64_t qen                   : 1;  /**< [  0:  0](R/W/H) Controls if instruction queue is enabled and can dispatch instructions to a requesting engine. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_vdmax_en_s cn; */
};
typedef union cavm_dpix_vdmax_en cavm_dpix_vdmax_en_t;

static inline uint64_t CAVM_DPIX_VDMAX_EN(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VDMAX_EN(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e020000000ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e200000000ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_VDMAX_EN", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VDMAX_EN(a,b) cavm_dpix_vdmax_en_t
#define bustype_CAVM_DPIX_VDMAX_EN(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VDMAX_EN(a,b) "DPIX_VDMAX_EN"
#define device_bar_CAVM_DPIX_VDMAX_EN(a,b) 0x0 /* VF_BAR0 */
#define busnum_CAVM_DPIX_VDMAX_EN(a,b) (a)
#define arguments_CAVM_DPIX_VDMAX_EN(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vdma#_iwbusy
 *
 * DPI DMA Instruction Word Busy Register
 */
union cavm_dpix_vdmax_iwbusy
{
    uint64_t u;
    struct cavm_dpix_vdmax_iwbusy_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t iwbusy                : 1;  /**< [  0:  0](RO/H) Indicates that there are valid instructions in the request queue local
                                                                 cache. These instructions will be dropped if DPI()_DMA()_QRST[QRST] is set for
                                                                 the corresponding request queue. */
#else /* Word 0 - Little Endian */
        uint64_t iwbusy                : 1;  /**< [  0:  0](RO/H) Indicates that there are valid instructions in the request queue local
                                                                 cache. These instructions will be dropped if DPI()_DMA()_QRST[QRST] is set for
                                                                 the corresponding request queue. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_vdmax_iwbusy_s cn; */
};
typedef union cavm_dpix_vdmax_iwbusy cavm_dpix_vdmax_iwbusy_t;

static inline uint64_t CAVM_DPIX_VDMAX_IWBUSY(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VDMAX_IWBUSY(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e020000030ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e200000030ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_VDMAX_IWBUSY", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VDMAX_IWBUSY(a,b) cavm_dpix_vdmax_iwbusy_t
#define bustype_CAVM_DPIX_VDMAX_IWBUSY(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VDMAX_IWBUSY(a,b) "DPIX_VDMAX_IWBUSY"
#define device_bar_CAVM_DPIX_VDMAX_IWBUSY(a,b) 0x0 /* VF_BAR0 */
#define busnum_CAVM_DPIX_VDMAX_IWBUSY(a,b) (a)
#define arguments_CAVM_DPIX_VDMAX_IWBUSY(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vdma#_naddr
 *
 * DPI DMA Next Ichunk Address Registers
 * These registers provide the IOVA to read the next Ichunk data.
 */
union cavm_dpix_vdmax_naddr
{
    uint64_t u;
    struct cavm_dpix_vdmax_naddr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_53_63        : 11;
        uint64_t addr                  : 53; /**< [ 52:  0](RO/H) Address. Provides the next L2C address to read instructions.  For diagnostic use only. */
#else /* Word 0 - Little Endian */
        uint64_t addr                  : 53; /**< [ 52:  0](RO/H) Address. Provides the next L2C address to read instructions.  For diagnostic use only. */
        uint64_t reserved_53_63        : 11;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_vdmax_naddr_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_49_63        : 15;
        uint64_t addr                  : 49; /**< [ 48:  0](RO/H) Address. Provides the next L2C address to read instructions.  For diagnostic use only. */
#else /* Word 0 - Little Endian */
        uint64_t addr                  : 49; /**< [ 48:  0](RO/H) Address. Provides the next L2C address to read instructions.  For diagnostic use only. */
        uint64_t reserved_49_63        : 15;
#endif /* Word 0 - End */
    } cn8;
    struct cavm_dpix_vdmax_naddr_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_53_63        : 11;
        uint64_t addr                  : 53; /**< [ 52:  0](RO/H) Address. Provides the next IOVA to read instructions.  For diagnostic use only. */
#else /* Word 0 - Little Endian */
        uint64_t addr                  : 53; /**< [ 52:  0](RO/H) Address. Provides the next IOVA to read instructions.  For diagnostic use only. */
        uint64_t reserved_53_63        : 11;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_dpix_vdmax_naddr cavm_dpix_vdmax_naddr_t;

static inline uint64_t CAVM_DPIX_VDMAX_NADDR(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VDMAX_NADDR(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e020000028ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e200000028ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_VDMAX_NADDR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VDMAX_NADDR(a,b) cavm_dpix_vdmax_naddr_t
#define bustype_CAVM_DPIX_VDMAX_NADDR(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VDMAX_NADDR(a,b) "DPIX_VDMAX_NADDR"
#define device_bar_CAVM_DPIX_VDMAX_NADDR(a,b) 0x0 /* VF_BAR0 */
#define busnum_CAVM_DPIX_VDMAX_NADDR(a,b) (a)
#define arguments_CAVM_DPIX_VDMAX_NADDR(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vdma#_reqq_ctl
 *
 * DPI DMA Request Queue Control Register
 * This register contains the control bits for transactions on the eight request queues.
 */
union cavm_dpix_vdmax_reqq_ctl
{
    uint64_t u;
    struct cavm_dpix_vdmax_reqq_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_13_63        : 51;
        uint64_t des_be                : 1;  /**< [ 12: 12](R/W) Instruction descriptor big-endian mode. When set, instructions data will come from memory
                                                                 in big-endian format and the bytes will be reversed before being used in DPI. */
        uint64_t reserved_9_11         : 3;
        uint64_t st_cmd                : 1;  /**< [  8:  8](R/W) When DPI issues a store full line command to the L2C that is to be cached, this field
                                                                 select the type of store command to use:
                                                                 0 = STF.
                                                                 1 = STY. */
        uint64_t reserved_2_7          : 6;
        uint64_t ld_cmd                : 2;  /**< [  1:  0](R/W) When DPI issues a load command to the L2C that is to be cached, this field select the type
                                                                 of load command to use:
                                                                 0x0 = LDD.
                                                                 0x1 = LDI.
                                                                 0x2 = LDE.
                                                                 0x3 = LDY. */
#else /* Word 0 - Little Endian */
        uint64_t ld_cmd                : 2;  /**< [  1:  0](R/W) When DPI issues a load command to the L2C that is to be cached, this field select the type
                                                                 of load command to use:
                                                                 0x0 = LDD.
                                                                 0x1 = LDI.
                                                                 0x2 = LDE.
                                                                 0x3 = LDY. */
        uint64_t reserved_2_7          : 6;
        uint64_t st_cmd                : 1;  /**< [  8:  8](R/W) When DPI issues a store full line command to the L2C that is to be cached, this field
                                                                 select the type of store command to use:
                                                                 0 = STF.
                                                                 1 = STY. */
        uint64_t reserved_9_11         : 3;
        uint64_t des_be                : 1;  /**< [ 12: 12](R/W) Instruction descriptor big-endian mode. When set, instructions data will come from memory
                                                                 in big-endian format and the bytes will be reversed before being used in DPI. */
        uint64_t reserved_13_63        : 51;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_vdmax_reqq_ctl_s cn8; */
    struct cavm_dpix_vdmax_reqq_ctl_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_13_63        : 51;
        uint64_t des_be                : 1;  /**< [ 12: 12](R/W) Instruction descriptor big-endian mode. When set, instructions data will come from memory
                                                                 in big-endian format and the bytes will be reversed before being used in DPI. */
        uint64_t reserved_9_11         : 3;
        uint64_t st_cmd                : 1;  /**< [  8:  8](R/W) When DPI issues a store full line command to the NCB that is to be cached, this field
                                                                 select the type of store command to use:
                                                                 0 = STF.
                                                                 1 = STY. */
        uint64_t reserved_2_7          : 6;
        uint64_t ld_cmd                : 2;  /**< [  1:  0](R/W) When DPI issues a load command to the NCB that is to be cached, this field select the type
                                                                 of load command to use:
                                                                 0x0 = LDD.
                                                                 0x1 = LDI.
                                                                 0x2 = LDE.
                                                                 0x3 = LDY. */
#else /* Word 0 - Little Endian */
        uint64_t ld_cmd                : 2;  /**< [  1:  0](R/W) When DPI issues a load command to the NCB that is to be cached, this field select the type
                                                                 of load command to use:
                                                                 0x0 = LDD.
                                                                 0x1 = LDI.
                                                                 0x2 = LDE.
                                                                 0x3 = LDY. */
        uint64_t reserved_2_7          : 6;
        uint64_t st_cmd                : 1;  /**< [  8:  8](R/W) When DPI issues a store full line command to the NCB that is to be cached, this field
                                                                 select the type of store command to use:
                                                                 0 = STF.
                                                                 1 = STY. */
        uint64_t reserved_9_11         : 3;
        uint64_t des_be                : 1;  /**< [ 12: 12](R/W) Instruction descriptor big-endian mode. When set, instructions data will come from memory
                                                                 in big-endian format and the bytes will be reversed before being used in DPI. */
        uint64_t reserved_13_63        : 51;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_dpix_vdmax_reqq_ctl cavm_dpix_vdmax_reqq_ctl_t;

static inline uint64_t CAVM_DPIX_VDMAX_REQQ_CTL(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VDMAX_REQQ_CTL(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e020000008ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e200000008ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_VDMAX_REQQ_CTL", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VDMAX_REQQ_CTL(a,b) cavm_dpix_vdmax_reqq_ctl_t
#define bustype_CAVM_DPIX_VDMAX_REQQ_CTL(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VDMAX_REQQ_CTL(a,b) "DPIX_VDMAX_REQQ_CTL"
#define device_bar_CAVM_DPIX_VDMAX_REQQ_CTL(a,b) 0x0 /* VF_BAR0 */
#define busnum_CAVM_DPIX_VDMAX_REQQ_CTL(a,b) (a)
#define arguments_CAVM_DPIX_VDMAX_REQQ_CTL(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vdma#_saddr
 *
 * DPI DMA Instruction-Buffer Starting-Address Registers
 * These registers provide the address to start reading instructions for the eight DMA
 * instruction queues. These register should only be written to when the specified queue is
 * disabled (DPI()_VDMA()_EN[QEN]).
 */
union cavm_dpix_vdmax_saddr
{
    uint64_t u;
    struct cavm_dpix_vdmax_saddr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t idle                  : 1;  /**< [ 63: 63](RO/H) DMA request queue is idle. When asserted, the associated request queue is idle. */
        uint64_t reserved_53_62        : 10;
        uint64_t saddr                 : 46; /**< [ 52:  7](R/W/H) Starting address. The 128-byte aligned starting or chunk address. SADDR is address bit
                                                                 \<48:7\> of the starting instructions address. When new chunks are fetched by the hardware,
                                                                 SADDR is updated to reflect the address of the current chunk. A write to SADDR resets both
                                                                 the queue's doorbell (DPI()_VDMA()_COUNTS[DBELL]) and its tail pointer
                                                                 (DPI()_VDMA()_NADDR[ADDR]). */
        uint64_t reserved_0_6          : 7;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_6          : 7;
        uint64_t saddr                 : 46; /**< [ 52:  7](R/W/H) Starting address. The 128-byte aligned starting or chunk address. SADDR is address bit
                                                                 \<48:7\> of the starting instructions address. When new chunks are fetched by the hardware,
                                                                 SADDR is updated to reflect the address of the current chunk. A write to SADDR resets both
                                                                 the queue's doorbell (DPI()_VDMA()_COUNTS[DBELL]) and its tail pointer
                                                                 (DPI()_VDMA()_NADDR[ADDR]). */
        uint64_t reserved_53_62        : 10;
        uint64_t idle                  : 1;  /**< [ 63: 63](RO/H) DMA request queue is idle. When asserted, the associated request queue is idle. */
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_vdmax_saddr_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t idle                  : 1;  /**< [ 63: 63](RO/H) DMA request queue is idle. When asserted, the associated request queue is idle. */
        uint64_t reserved_49_62        : 14;
        uint64_t saddr                 : 42; /**< [ 48:  7](R/W/H) Starting address. The 128-byte aligned starting or chunk address. SADDR is address bit
                                                                 \<48:7\> of the starting instructions address. When new chunks are fetched by the hardware,
                                                                 SADDR is updated to reflect the address of the current chunk. A write to SADDR resets both
                                                                 the queue's doorbell (DPI()_VDMA()_COUNTS[DBELL]) and its tail pointer
                                                                 (DPI()_VDMA()_NADDR[ADDR]). */
        uint64_t reserved_0_6          : 7;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_6          : 7;
        uint64_t saddr                 : 42; /**< [ 48:  7](R/W/H) Starting address. The 128-byte aligned starting or chunk address. SADDR is address bit
                                                                 \<48:7\> of the starting instructions address. When new chunks are fetched by the hardware,
                                                                 SADDR is updated to reflect the address of the current chunk. A write to SADDR resets both
                                                                 the queue's doorbell (DPI()_VDMA()_COUNTS[DBELL]) and its tail pointer
                                                                 (DPI()_VDMA()_NADDR[ADDR]). */
        uint64_t reserved_49_62        : 14;
        uint64_t idle                  : 1;  /**< [ 63: 63](RO/H) DMA request queue is idle. When asserted, the associated request queue is idle. */
#endif /* Word 0 - End */
    } cn8;
    struct cavm_dpix_vdmax_saddr_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t idle                  : 1;  /**< [ 63: 63](RO/H) DMA request queue is idle. When asserted, the associated request queue is idle. */
        uint64_t reserved_53_62        : 10;
        uint64_t saddr                 : 46; /**< [ 52:  7](R/W/H) Starting address. The 128-byte aligned starting or chunk address. [SADDR] is address bit
                                                                 \<52:7\> of the starting instructions address. When new chunks are fetched by the hardware,
                                                                 SADDR is updated to reflect the address of the current chunk. A write to SADDR resets both
                                                                 the queue's doorbell (DPI()_VDMA()_COUNTS[DBELL]) and its tail pointer
                                                                 (DPI()_VDMA()_NADDR[ADDR]). */
        uint64_t reserved_0_6          : 7;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_6          : 7;
        uint64_t saddr                 : 46; /**< [ 52:  7](R/W/H) Starting address. The 128-byte aligned starting or chunk address. [SADDR] is address bit
                                                                 \<52:7\> of the starting instructions address. When new chunks are fetched by the hardware,
                                                                 SADDR is updated to reflect the address of the current chunk. A write to SADDR resets both
                                                                 the queue's doorbell (DPI()_VDMA()_COUNTS[DBELL]) and its tail pointer
                                                                 (DPI()_VDMA()_NADDR[ADDR]). */
        uint64_t reserved_53_62        : 10;
        uint64_t idle                  : 1;  /**< [ 63: 63](RO/H) DMA request queue is idle. When asserted, the associated request queue is idle. */
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_dpix_vdmax_saddr cavm_dpix_vdmax_saddr_t;

static inline uint64_t CAVM_DPIX_VDMAX_SADDR(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VDMAX_SADDR(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e020000018ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e200000018ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_VDMAX_SADDR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VDMAX_SADDR(a,b) cavm_dpix_vdmax_saddr_t
#define bustype_CAVM_DPIX_VDMAX_SADDR(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VDMAX_SADDR(a,b) "DPIX_VDMAX_SADDR"
#define device_bar_CAVM_DPIX_VDMAX_SADDR(a,b) 0x0 /* VF_BAR0 */
#define busnum_CAVM_DPIX_VDMAX_SADDR(a,b) (a)
#define arguments_CAVM_DPIX_VDMAX_SADDR(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vf#_int
 *
 * DPI DMA Per-Request Queue Instruction Completion Interrupt Register
 * This register contains per-request queue completion interrupt bits.
 */
union cavm_dpix_vfx_int
{
    uint64_t u;
    struct cavm_dpix_vfx_int_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t complete              : 1;  /**< [  0:  0](R/W1C/H) DPI DMA per-process instruction completion interrupt. See DPI()_VDMA()_CNT.

                                                                 If these interrupts are enabled, the DPI()_DMA_CC()_INT interrupts should not be enabled. */
#else /* Word 0 - Little Endian */
        uint64_t complete              : 1;  /**< [  0:  0](R/W1C/H) DPI DMA per-process instruction completion interrupt. See DPI()_VDMA()_CNT.

                                                                 If these interrupts are enabled, the DPI()_DMA_CC()_INT interrupts should not be enabled. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_vfx_int_s cn; */
};
typedef union cavm_dpix_vfx_int cavm_dpix_vfx_int_t;

static inline uint64_t CAVM_DPIX_VFX_INT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VFX_INT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e020000100ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e200000100ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_VFX_INT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VFX_INT(a,b) cavm_dpix_vfx_int_t
#define bustype_CAVM_DPIX_VFX_INT(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VFX_INT(a,b) "DPIX_VFX_INT"
#define device_bar_CAVM_DPIX_VFX_INT(a,b) 0x0 /* VF_BAR0 */
#define busnum_CAVM_DPIX_VFX_INT(a,b) (a)
#define arguments_CAVM_DPIX_VFX_INT(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vf#_int_ena_w1c
 *
 * DPI DMA Per-Request Queue Instruction Completion Interrupt Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_dpix_vfx_int_ena_w1c
{
    uint64_t u;
    struct cavm_dpix_vfx_int_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t complete              : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_VF(0..7)_INT[COMPLETE]. */
#else /* Word 0 - Little Endian */
        uint64_t complete              : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for DPI(0)_VF(0..7)_INT[COMPLETE]. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_vfx_int_ena_w1c_s cn; */
};
typedef union cavm_dpix_vfx_int_ena_w1c cavm_dpix_vfx_int_ena_w1c_t;

static inline uint64_t CAVM_DPIX_VFX_INT_ENA_W1C(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VFX_INT_ENA_W1C(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e020000110ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e200000110ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_VFX_INT_ENA_W1C", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VFX_INT_ENA_W1C(a,b) cavm_dpix_vfx_int_ena_w1c_t
#define bustype_CAVM_DPIX_VFX_INT_ENA_W1C(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VFX_INT_ENA_W1C(a,b) "DPIX_VFX_INT_ENA_W1C"
#define device_bar_CAVM_DPIX_VFX_INT_ENA_W1C(a,b) 0x0 /* VF_BAR0 */
#define busnum_CAVM_DPIX_VFX_INT_ENA_W1C(a,b) (a)
#define arguments_CAVM_DPIX_VFX_INT_ENA_W1C(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vf#_int_ena_w1s
 *
 * DPI DMA Per-Request Queue Instruction Completion Interrupt Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_dpix_vfx_int_ena_w1s
{
    uint64_t u;
    struct cavm_dpix_vfx_int_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t complete              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_VF(0..7)_INT[COMPLETE]. */
#else /* Word 0 - Little Endian */
        uint64_t complete              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for DPI(0)_VF(0..7)_INT[COMPLETE]. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_vfx_int_ena_w1s_s cn; */
};
typedef union cavm_dpix_vfx_int_ena_w1s cavm_dpix_vfx_int_ena_w1s_t;

static inline uint64_t CAVM_DPIX_VFX_INT_ENA_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VFX_INT_ENA_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e020000118ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e200000118ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_VFX_INT_ENA_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VFX_INT_ENA_W1S(a,b) cavm_dpix_vfx_int_ena_w1s_t
#define bustype_CAVM_DPIX_VFX_INT_ENA_W1S(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VFX_INT_ENA_W1S(a,b) "DPIX_VFX_INT_ENA_W1S"
#define device_bar_CAVM_DPIX_VFX_INT_ENA_W1S(a,b) 0x0 /* VF_BAR0 */
#define busnum_CAVM_DPIX_VFX_INT_ENA_W1S(a,b) (a)
#define arguments_CAVM_DPIX_VFX_INT_ENA_W1S(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vf#_int_w1s
 *
 * DPI DMA Per-Request Queue Instruction Completion Interrupt Set Register
 * This register sets interrupt bits.
 */
union cavm_dpix_vfx_int_w1s
{
    uint64_t u;
    struct cavm_dpix_vfx_int_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t complete              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_VF(0..7)_INT[COMPLETE]. */
#else /* Word 0 - Little Endian */
        uint64_t complete              : 1;  /**< [  0:  0](R/W1S/H) Reads or sets DPI(0)_VF(0..7)_INT[COMPLETE]. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_vfx_int_w1s_s cn; */
};
typedef union cavm_dpix_vfx_int_w1s cavm_dpix_vfx_int_w1s_t;

static inline uint64_t CAVM_DPIX_VFX_INT_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VFX_INT_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7)))
        return 0x86e020000108ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7)))
        return 0x86e200000108ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7);
    __cavm_csr_fatal("DPIX_VFX_INT_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VFX_INT_W1S(a,b) cavm_dpix_vfx_int_w1s_t
#define bustype_CAVM_DPIX_VFX_INT_W1S(a,b) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VFX_INT_W1S(a,b) "DPIX_VFX_INT_W1S"
#define device_bar_CAVM_DPIX_VFX_INT_W1S(a,b) 0x0 /* VF_BAR0 */
#define busnum_CAVM_DPIX_VFX_INT_W1S(a,b) (a)
#define arguments_CAVM_DPIX_VFX_INT_W1S(a,b) (a),(b),-1,-1

/**
 * Register (NCB) dpi#_vf#_msix_pba#
 *
 * DPI MSI-X Pending Bit Array Registers
 * This register is the MSI-X PBA table; the bit number is indexed by the DPI_VF()_INT_VEC_E
 * enumeration.
 */
union cavm_dpix_vfx_msix_pbax
{
    uint64_t u;
    struct cavm_dpix_vfx_msix_pbax_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO/H) Pending message for the associated DPI()_PF_MSIX_VEC()_CTL, enumerated by
                                                                 DPI_PF_INT_VEC_E. Bits that have no associated DPI_PF_INT_VEC_E are 0. */
#else /* Word 0 - Little Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO/H) Pending message for the associated DPI()_PF_MSIX_VEC()_CTL, enumerated by
                                                                 DPI_PF_INT_VEC_E. Bits that have no associated DPI_PF_INT_VEC_E are 0. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_dpix_vfx_msix_pbax_s cn; */
};
typedef union cavm_dpix_vfx_msix_pbax cavm_dpix_vfx_msix_pbax_t;

static inline uint64_t CAVM_DPIX_VFX_MSIX_PBAX(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VFX_MSIX_PBAX(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7) && (c==0)))
        return 0x86e0300f0000ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7) + 8ll * ((c) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7) && (c==0)))
        return 0x86e2100f0000ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7) + 8ll * ((c) & 0x0);
    __cavm_csr_fatal("DPIX_VFX_MSIX_PBAX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VFX_MSIX_PBAX(a,b,c) cavm_dpix_vfx_msix_pbax_t
#define bustype_CAVM_DPIX_VFX_MSIX_PBAX(a,b,c) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VFX_MSIX_PBAX(a,b,c) "DPIX_VFX_MSIX_PBAX"
#define device_bar_CAVM_DPIX_VFX_MSIX_PBAX(a,b,c) 0x4 /* VF_BAR4 */
#define busnum_CAVM_DPIX_VFX_MSIX_PBAX(a,b,c) (a)
#define arguments_CAVM_DPIX_VFX_MSIX_PBAX(a,b,c) (a),(b),(c),-1

/**
 * Register (NCB) dpi#_vf#_msix_vec#_addr
 *
 * DPI VF MSI-X Vector-Table Address Register
 * This register is the MSI-X vector table, indexed by the DPI_VF_INT_VEC_E enumeration.
 */
union cavm_dpix_vfx_msix_vecx_addr
{
    uint64_t u;
    struct cavm_dpix_vfx_msix_vecx_addr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_53_63        : 11;
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_1            : 1;
        uint64_t secvec                : 1;  /**< [  0:  0](RAZ) Secure vector. Zero as not supported on a per-vector basis for VFs; use
                                                                 PCCPF_DPI_VSEC_SCTL[MSIX_SEC] instead (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]). */
#else /* Word 0 - Little Endian */
        uint64_t secvec                : 1;  /**< [  0:  0](RAZ) Secure vector. Zero as not supported on a per-vector basis for VFs; use
                                                                 PCCPF_DPI_VSEC_SCTL[MSIX_SEC] instead (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]). */
        uint64_t reserved_1            : 1;
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_53_63        : 11;
#endif /* Word 0 - End */
    } s;
    struct cavm_dpix_vfx_msix_vecx_addr_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_49_63        : 15;
        uint64_t addr                  : 47; /**< [ 48:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_1            : 1;
        uint64_t secvec                : 1;  /**< [  0:  0](RAZ) Secure vector. Zero as not supported on a per-vector basis for VFs; use
                                                                 PCCPF_DPI_VSEC_SCTL[MSIX_SEC] instead (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]). */
#else /* Word 0 - Little Endian */
        uint64_t secvec                : 1;  /**< [  0:  0](RAZ) Secure vector. Zero as not supported on a per-vector basis for VFs; use
                                                                 PCCPF_DPI_VSEC_SCTL[MSIX_SEC] instead (for documentation, see
                                                                 PCCPF_XXX_VSEC_SCTL[MSIX_SEC]). */
        uint64_t reserved_1            : 1;
        uint64_t addr                  : 47; /**< [ 48:  2](R/W) IOVA to use for MSI-X delivery of this vector. */
        uint64_t reserved_49_63        : 15;
#endif /* Word 0 - End */
    } cn8;
    /* struct cavm_dpix_vfx_msix_vecx_addr_s cn9; */
};
typedef union cavm_dpix_vfx_msix_vecx_addr cavm_dpix_vfx_msix_vecx_addr_t;

static inline uint64_t CAVM_DPIX_VFX_MSIX_VECX_ADDR(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VFX_MSIX_VECX_ADDR(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7) && (c==0)))
        return 0x86e030000000ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7) + 0x10ll * ((c) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7) && (c==0)))
        return 0x86e210000000ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7) + 0x10ll * ((c) & 0x0);
    __cavm_csr_fatal("DPIX_VFX_MSIX_VECX_ADDR", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VFX_MSIX_VECX_ADDR(a,b,c) cavm_dpix_vfx_msix_vecx_addr_t
#define bustype_CAVM_DPIX_VFX_MSIX_VECX_ADDR(a,b,c) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VFX_MSIX_VECX_ADDR(a,b,c) "DPIX_VFX_MSIX_VECX_ADDR"
#define device_bar_CAVM_DPIX_VFX_MSIX_VECX_ADDR(a,b,c) 0x4 /* VF_BAR4 */
#define busnum_CAVM_DPIX_VFX_MSIX_VECX_ADDR(a,b,c) (a)
#define arguments_CAVM_DPIX_VFX_MSIX_VECX_ADDR(a,b,c) (a),(b),(c),-1

/**
 * Register (NCB) dpi#_vf#_msix_vec#_ctl
 *
 * DPI MSI-X Vector-Table Control and Data Register
 * This register is the MSI-X vector table, indexed by the DPI_VF_INT_VEC_E enumeration.
 */
union cavm_dpix_vfx_msix_vecx_ctl
{
    uint64_t u;
    struct cavm_dpix_vfx_msix_vecx_ctl_s
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
    struct cavm_dpix_vfx_msix_vecx_ctl_cn8
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
    } cn8;
    /* struct cavm_dpix_vfx_msix_vecx_ctl_s cn9; */
};
typedef union cavm_dpix_vfx_msix_vecx_ctl cavm_dpix_vfx_msix_vecx_ctl_t;

static inline uint64_t CAVM_DPIX_VFX_MSIX_VECX_CTL(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_DPIX_VFX_MSIX_VECX_CTL(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=7) && (c==0)))
        return 0x86e030000008ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7) + 0x10ll * ((c) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=7) && (c==0)))
        return 0x86e210000008ll + 0x1000000000ll * ((a) & 0x0) + 0x100000ll * ((b) & 0x7) + 0x10ll * ((c) & 0x0);
    __cavm_csr_fatal("DPIX_VFX_MSIX_VECX_CTL", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_DPIX_VFX_MSIX_VECX_CTL(a,b,c) cavm_dpix_vfx_msix_vecx_ctl_t
#define bustype_CAVM_DPIX_VFX_MSIX_VECX_CTL(a,b,c) CSR_TYPE_NCB
#define basename_CAVM_DPIX_VFX_MSIX_VECX_CTL(a,b,c) "DPIX_VFX_MSIX_VECX_CTL"
#define device_bar_CAVM_DPIX_VFX_MSIX_VECX_CTL(a,b,c) 0x4 /* VF_BAR4 */
#define busnum_CAVM_DPIX_VFX_MSIX_VECX_CTL(a,b,c) (a)
#define arguments_CAVM_DPIX_VFX_MSIX_VECX_CTL(a,b,c) (a),(b),(c),-1

/**
 * Register (NCB) sdp#_bp_test0
 *
 * INTERNAL: SDP Backpressure Test Register 0
 */
union cavm_sdpx_bp_test0
{
    uint64_t u;
    struct cavm_sdpx_bp_test0_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t enable                : 4;  /**< [ 63: 60](R/W) Enable test mode. For diagnostic use only.
                                                                 Internal:
                                                                 Once a bit is set, random backpressure is generated
                                                                 at the corresponding point to allow for more frequent backpressure.
                                                                 \<63\> = Backpressure the SDP to DPI write requests.
                                                                 \<62\> = Backpressure the SDP to DPI read requests.
                                                                 \<61\> = Backpressure the SDP X2P bus.
                                                                 \<60\> = Backpressure SDP MSIX writes. */
        uint64_t reserved_24_59        : 36;
        uint64_t bp_cfg                : 8;  /**< [ 23: 16](R/W) Backpressure weight. For diagnostic use only.
                                                                 Internal:
                                                                 There are 2 backpressure configuration bits per enable, with the two bits
                                                                 defined as 0x0=100% of the time, 0x1=75% of the time, 0x2=50% of the time,
                                                                 0x3=25% of the time.
                                                                   \<23:22\> = Config 3.
                                                                   \<21:20\> = Config 2.
                                                                   \<19:18\> = Config 1.
                                                                   \<17:16\> = Config 0. */
        uint64_t reserved_12_15        : 4;
        uint64_t lfsr_freq             : 12; /**< [ 11:  0](R/W) Test LFSR update frequency in coprocessor-clocks minus one. */
#else /* Word 0 - Little Endian */
        uint64_t lfsr_freq             : 12; /**< [ 11:  0](R/W) Test LFSR update frequency in coprocessor-clocks minus one. */
        uint64_t reserved_12_15        : 4;
        uint64_t bp_cfg                : 8;  /**< [ 23: 16](R/W) Backpressure weight. For diagnostic use only.
                                                                 Internal:
                                                                 There are 2 backpressure configuration bits per enable, with the two bits
                                                                 defined as 0x0=100% of the time, 0x1=75% of the time, 0x2=50% of the time,
                                                                 0x3=25% of the time.
                                                                   \<23:22\> = Config 3.
                                                                   \<21:20\> = Config 2.
                                                                   \<19:18\> = Config 1.
                                                                   \<17:16\> = Config 0. */
        uint64_t reserved_24_59        : 36;
        uint64_t enable                : 4;  /**< [ 63: 60](R/W) Enable test mode. For diagnostic use only.
                                                                 Internal:
                                                                 Once a bit is set, random backpressure is generated
                                                                 at the corresponding point to allow for more frequent backpressure.
                                                                 \<63\> = Backpressure the SDP to DPI write requests.
                                                                 \<62\> = Backpressure the SDP to DPI read requests.
                                                                 \<61\> = Backpressure the SDP X2P bus.
                                                                 \<60\> = Backpressure SDP MSIX writes. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_bp_test0_s cn; */
};
typedef union cavm_sdpx_bp_test0 cavm_sdpx_bp_test0_t;

static inline uint64_t CAVM_SDPX_BP_TEST0(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_BP_TEST0(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0c00802d0ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_BP_TEST0", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_BP_TEST0(a) cavm_sdpx_bp_test0_t
#define bustype_CAVM_SDPX_BP_TEST0(a) CSR_TYPE_NCB
#define basename_CAVM_SDPX_BP_TEST0(a) "SDPX_BP_TEST0"
#define device_bar_CAVM_SDPX_BP_TEST0(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_BP_TEST0(a) (a)
#define arguments_CAVM_SDPX_BP_TEST0(a) (a),-1,-1,-1

/**
 * Register (NCB) sdp#_const
 *
 * SDP Constants Register
 * This register contains constants for software discovery.
 */
union cavm_sdpx_const
{
    uint64_t u;
    struct cavm_sdpx_const_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t rings                 : 16; /**< [ 31: 16](RO) Number of rings implemented. */
        uint64_t fifosz                : 16; /**< [ 15:  0](RO) Number of bytes in outbound FIFO. */
#else /* Word 0 - Little Endian */
        uint64_t fifosz                : 16; /**< [ 15:  0](RO) Number of bytes in outbound FIFO. */
        uint64_t rings                 : 16; /**< [ 31: 16](RO) Number of rings implemented. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_const_s cn; */
};
typedef union cavm_sdpx_const cavm_sdpx_const_t;

static inline uint64_t CAVM_SDPX_CONST(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_CONST(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x874000880300ll + 0x1000000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0c0080300ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_CONST", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_CONST(a) cavm_sdpx_const_t
#define bustype_CAVM_SDPX_CONST(a) CSR_TYPE_NCB
#define basename_CAVM_SDPX_CONST(a) "SDPX_CONST"
#define device_bar_CAVM_SDPX_CONST(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_CONST(a) (a)
#define arguments_CAVM_SDPX_CONST(a) (a),-1,-1,-1

/**
 * Register (NCB) sdp#_csclk_active_pc
 *
 * SDP Conditional Coprocessor Clock Counter Register
 * This register counts conditional clocks for power management.
 */
union cavm_sdpx_csclk_active_pc
{
    uint64_t u;
    struct cavm_sdpx_csclk_active_pc_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Count of conditional coprocessor-clock cycles since reset. */
#else /* Word 0 - Little Endian */
        uint64_t count                 : 64; /**< [ 63:  0](R/W/H) Count of conditional coprocessor-clock cycles since reset. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_csclk_active_pc_s cn; */
};
typedef union cavm_sdpx_csclk_active_pc cavm_sdpx_csclk_active_pc_t;

static inline uint64_t CAVM_SDPX_CSCLK_ACTIVE_PC(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_CSCLK_ACTIVE_PC(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0c0080220ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_CSCLK_ACTIVE_PC", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_CSCLK_ACTIVE_PC(a) cavm_sdpx_csclk_active_pc_t
#define bustype_CAVM_SDPX_CSCLK_ACTIVE_PC(a) CSR_TYPE_NCB
#define basename_CAVM_SDPX_CSCLK_ACTIVE_PC(a) "SDPX_CSCLK_ACTIVE_PC"
#define device_bar_CAVM_SDPX_CSCLK_ACTIVE_PC(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_CSCLK_ACTIVE_PC(a) (a)
#define arguments_CAVM_SDPX_CSCLK_ACTIVE_PC(a) (a),-1,-1,-1

/**
 * Register (NCB) sdp#_diag
 *
 * SDP Diagnostic Status Register
 * This register indicates some conditions which may be interesting for diagnostics.
 */
union cavm_sdpx_diag
{
    uint64_t u;
    struct cavm_sdpx_diag_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_3_63         : 61;
        uint64_t in_dir_toobig         : 1;  /**< [  2:  2](R/W1C/H) Direct gather list length specified is greater than (packet length - FSZ) in instruction. */
        uint64_t in_ind_unaligned      : 1;  /**< [  1:  1](R/W1C/H) Indirect gather list is not aligned to 8-byte boundary.  This will also cause IND_DMA_ERR. */
        uint64_t in_ind_toobig         : 1;  /**< [  0:  0](R/W1C/H) Indirect gather list length specified is greater than (packet length - FSZ) in instruction. */
#else /* Word 0 - Little Endian */
        uint64_t in_ind_toobig         : 1;  /**< [  0:  0](R/W1C/H) Indirect gather list length specified is greater than (packet length - FSZ) in instruction. */
        uint64_t in_ind_unaligned      : 1;  /**< [  1:  1](R/W1C/H) Indirect gather list is not aligned to 8-byte boundary.  This will also cause IND_DMA_ERR. */
        uint64_t in_dir_toobig         : 1;  /**< [  2:  2](R/W1C/H) Direct gather list length specified is greater than (packet length - FSZ) in instruction. */
        uint64_t reserved_3_63         : 61;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_diag_s cn8; */
    struct cavm_sdpx_diag_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_3_63         : 61;
        uint64_t in_dir_toobig         : 1;  /**< [  2:  2](R/W1C/H) Direct gather list length specified is greater than (packet length - FSZ) in instruction. */
        uint64_t in_ind_unaligned      : 1;  /**< [  1:  1](R/W1C/H) Indirect gather list is not aligned to 8-byte boundary.  This will also cause
                                                                 SDP()_R()_ERR_TYPE[IND_DMA_ERR]. */
        uint64_t in_ind_toobig         : 1;  /**< [  0:  0](R/W1C/H) Indirect gather list length specified is greater than (packet length - FSZ) in instruction. */
#else /* Word 0 - Little Endian */
        uint64_t in_ind_toobig         : 1;  /**< [  0:  0](R/W1C/H) Indirect gather list length specified is greater than (packet length - FSZ) in instruction. */
        uint64_t in_ind_unaligned      : 1;  /**< [  1:  1](R/W1C/H) Indirect gather list is not aligned to 8-byte boundary.  This will also cause
                                                                 SDP()_R()_ERR_TYPE[IND_DMA_ERR]. */
        uint64_t in_dir_toobig         : 1;  /**< [  2:  2](R/W1C/H) Direct gather list length specified is greater than (packet length - FSZ) in instruction. */
        uint64_t reserved_3_63         : 61;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_sdpx_diag cavm_sdpx_diag_t;

static inline uint64_t CAVM_SDPX_DIAG(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_DIAG(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x874000880140ll + 0x1000000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0c0080140ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_DIAG", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_DIAG(a) cavm_sdpx_diag_t
#define bustype_CAVM_SDPX_DIAG(a) CSR_TYPE_NCB
#define basename_CAVM_SDPX_DIAG(a) "SDPX_DIAG"
#define device_bar_CAVM_SDPX_DIAG(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_DIAG(a) (a)
#define arguments_CAVM_SDPX_DIAG(a) (a),-1,-1,-1

/**
 * Register (NCB) sdp#_eco
 *
 * INTERNAL: SDP ECO Register
 */
union cavm_sdpx_eco
{
    uint64_t u;
    struct cavm_sdpx_eco_s
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
    /* struct cavm_sdpx_eco_s cn8; */
    struct cavm_sdpx_eco_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t eco_rw                : 32; /**< [ 31:  0](R/W) Reserved for ECO usage. */
#else /* Word 0 - Little Endian */
        uint64_t eco_rw                : 32; /**< [ 31:  0](R/W) Reserved for ECO usage. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_sdpx_eco cavm_sdpx_eco_t;

static inline uint64_t CAVM_SDPX_ECO(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_ECO(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x874000880260ll + 0x1000000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0c00802c0ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_ECO", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_ECO(a) cavm_sdpx_eco_t
#define bustype_CAVM_SDPX_ECO(a) CSR_TYPE_NCB
#define basename_CAVM_SDPX_ECO(a) "SDPX_ECO"
#define device_bar_CAVM_SDPX_ECO(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_ECO(a) (a)
#define arguments_CAVM_SDPX_ECO(a) (a),-1,-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_dma_cnt#
 *
 * SDP DMA Count Registers
 * These registers contain the DMA count values.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_dma_cntx
{
    uint64_t u;
    struct cavm_sdpx_epfx_dma_cntx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t cnt                   : 32; /**< [ 31:  0](R/W/H) The DMA counter. SDP/DPI hardware subtracts the written value from
                                                                 the counter whenever software writes this CSR. SDP/DPI hardware increments this
                                                                 counter after completing an OUTBOUND or EXTERNAL-ONLY DMA instruction
                                                                 with DPI_DMA_INSTR_HDR_S[CA] set DPI_DMA_INSTR_HDR_S[CSEL] equal to this
                                                                 CSR index. These increments may cause interrupts.
                                                                 See SDP()_EPF()_DMA_INT_LEVEL() and SDP()_EPF()_DMA_RINT[DCNT,DTIME]. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 32; /**< [ 31:  0](R/W/H) The DMA counter. SDP/DPI hardware subtracts the written value from
                                                                 the counter whenever software writes this CSR. SDP/DPI hardware increments this
                                                                 counter after completing an OUTBOUND or EXTERNAL-ONLY DMA instruction
                                                                 with DPI_DMA_INSTR_HDR_S[CA] set DPI_DMA_INSTR_HDR_S[CSEL] equal to this
                                                                 CSR index. These increments may cause interrupts.
                                                                 See SDP()_EPF()_DMA_INT_LEVEL() and SDP()_EPF()_DMA_RINT[DCNT,DTIME]. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_dma_cntx_s cn; */
};
typedef union cavm_sdpx_epfx_dma_cntx cavm_sdpx_epfx_dma_cntx_t;

static inline uint64_t CAVM_SDPX_EPFX_DMA_CNTX(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_DMA_CNTX(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020460ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_DMA_CNTX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_DMA_CNTX(a,b,c) cavm_sdpx_epfx_dma_cntx_t
#define bustype_CAVM_SDPX_EPFX_DMA_CNTX(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_DMA_CNTX(a,b,c) "SDPX_EPFX_DMA_CNTX"
#define device_bar_CAVM_SDPX_EPFX_DMA_CNTX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_DMA_CNTX(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_DMA_CNTX(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_dma_int_level#
 *
 * SDP DMA Interrupt Level Registers
 * These registers contain the thresholds for DMA count and timer interrupts.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_dma_int_levelx
{
    uint64_t u;
    struct cavm_sdpx_epfx_dma_int_levelx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t tim                   : 32; /**< [ 63: 32](R/W) Whenever the SDP()_EPF()_DMA_TIM()[TIM] timer exceeds this value,
                                                                 SDP()_EPF()_DMA_RINT[DTIME]\<x\> is set. The SDP()_EPF()_DMA_TIM()[TIM] timer
                                                                 increments every SDP clock whenever SDP()_EPF()_DMA_CNT()[CNT] != 0, and is cleared
                                                                 when SDP()_EPF()_DMA_CNT()[CNT] is written to a non zero value. */
        uint64_t cnt                   : 32; /**< [ 31:  0](R/W) Whenever SDP()_EPF()_DMA_CNT()[CNT] exceeds this value, SDP()_EPF()_DMA_RINT[DCNT]\<x\>
                                                                 is set. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 32; /**< [ 31:  0](R/W) Whenever SDP()_EPF()_DMA_CNT()[CNT] exceeds this value, SDP()_EPF()_DMA_RINT[DCNT]\<x\>
                                                                 is set. */
        uint64_t tim                   : 32; /**< [ 63: 32](R/W) Whenever the SDP()_EPF()_DMA_TIM()[TIM] timer exceeds this value,
                                                                 SDP()_EPF()_DMA_RINT[DTIME]\<x\> is set. The SDP()_EPF()_DMA_TIM()[TIM] timer
                                                                 increments every SDP clock whenever SDP()_EPF()_DMA_CNT()[CNT] != 0, and is cleared
                                                                 when SDP()_EPF()_DMA_CNT()[CNT] is written to a non zero value. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_dma_int_levelx_s cn; */
};
typedef union cavm_sdpx_epfx_dma_int_levelx cavm_sdpx_epfx_dma_int_levelx_t;

static inline uint64_t CAVM_SDPX_EPFX_DMA_INT_LEVELX(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_DMA_INT_LEVELX(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020440ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_DMA_INT_LEVELX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_DMA_INT_LEVELX(a,b,c) cavm_sdpx_epfx_dma_int_levelx_t
#define bustype_CAVM_SDPX_EPFX_DMA_INT_LEVELX(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_DMA_INT_LEVELX(a,b,c) "SDPX_EPFX_DMA_INT_LEVELX"
#define device_bar_CAVM_SDPX_EPFX_DMA_INT_LEVELX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_DMA_INT_LEVELX(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_DMA_INT_LEVELX(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_dma_rint
 *
 * SDP/DPI DTIME/DCNT/DMAFI Interrupt Registers
 * These registers contain interrupts related to the DPI DMA engines.
 * The given register associated with an EPF will be reset due to a PF FLR or MAC reset.
 * These registers are not affected by VF FLR.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_dma_rint
{
    uint64_t u;
    struct cavm_sdpx_epfx_dma_rint_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_6_63         : 58;
        uint64_t dtime                 : 2;  /**< [  5:  4](R/W1C/H) Whenever SDP()_EPF()_DMA_CNT()[CNT] is not 0, the SDP()_EPF()_DMA_TIM()[TIM]
                                                                 timer increments every SDP clock. [DTIME]\<x\> is set whenever
                                                                 SDP()_EPF()_DMA_TIM()[TIM] \> SDP()_EPF()_DMA_INT_LEVEL()[TIM].
                                                                 [DTIME]\<x\> is cleared when writing a non zero value to SDP()_EPF()_DMA_CNT()[CNT]
                                                                 causing SDP()_EPF()_DMA_TIM()[TIM] to clear to 0 and
                                                                 SDP()_EPF()_DMA_TIM()[TIM] to fall below SDP()_EPF()_DMA_INT_LEVEL()[TIM]. */
        uint64_t dcnt                  : 2;  /**< [  3:  2](R/W1C/H) [DCNT]\<x\> is set whenever SDP()_EPF()_DMA_CNT()[CNT] \> SDP()_EPF()_DMA_INT_LEVEL()[CNT].
                                                                 [DCNT]\<x\> is normally cleared by decreasing SDP()_EPF()_DMA_CNT()[CNT].

                                                                 Since DCNT is a level interrupt, SDP()_EPF()_DMA_CNT()[CNT] must be decremented below
                                                                 SDP()_EPF()_DMA_INT_LEVEL()[CNT] to receive subsequent DCNT interrupts. */
        uint64_t dmafi                 : 2;  /**< [  1:  0](R/W1C/H) DMA set forced interrupts. Set by SDP/DPI after completing a DPI DMA
                                                                 Instruction with DPI_DMA_INSTR_HDR_S[FI] set. */
#else /* Word 0 - Little Endian */
        uint64_t dmafi                 : 2;  /**< [  1:  0](R/W1C/H) DMA set forced interrupts. Set by SDP/DPI after completing a DPI DMA
                                                                 Instruction with DPI_DMA_INSTR_HDR_S[FI] set. */
        uint64_t dcnt                  : 2;  /**< [  3:  2](R/W1C/H) [DCNT]\<x\> is set whenever SDP()_EPF()_DMA_CNT()[CNT] \> SDP()_EPF()_DMA_INT_LEVEL()[CNT].
                                                                 [DCNT]\<x\> is normally cleared by decreasing SDP()_EPF()_DMA_CNT()[CNT].

                                                                 Since DCNT is a level interrupt, SDP()_EPF()_DMA_CNT()[CNT] must be decremented below
                                                                 SDP()_EPF()_DMA_INT_LEVEL()[CNT] to receive subsequent DCNT interrupts. */
        uint64_t dtime                 : 2;  /**< [  5:  4](R/W1C/H) Whenever SDP()_EPF()_DMA_CNT()[CNT] is not 0, the SDP()_EPF()_DMA_TIM()[TIM]
                                                                 timer increments every SDP clock. [DTIME]\<x\> is set whenever
                                                                 SDP()_EPF()_DMA_TIM()[TIM] \> SDP()_EPF()_DMA_INT_LEVEL()[TIM].
                                                                 [DTIME]\<x\> is cleared when writing a non zero value to SDP()_EPF()_DMA_CNT()[CNT]
                                                                 causing SDP()_EPF()_DMA_TIM()[TIM] to clear to 0 and
                                                                 SDP()_EPF()_DMA_TIM()[TIM] to fall below SDP()_EPF()_DMA_INT_LEVEL()[TIM]. */
        uint64_t reserved_6_63         : 58;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_dma_rint_s cn; */
};
typedef union cavm_sdpx_epfx_dma_rint cavm_sdpx_epfx_dma_rint_t;

static inline uint64_t CAVM_SDPX_EPFX_DMA_RINT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_DMA_RINT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020400ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_DMA_RINT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_DMA_RINT(a,b) cavm_sdpx_epfx_dma_rint_t
#define bustype_CAVM_SDPX_EPFX_DMA_RINT(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_DMA_RINT(a,b) "SDPX_EPFX_DMA_RINT"
#define device_bar_CAVM_SDPX_EPFX_DMA_RINT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_DMA_RINT(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_DMA_RINT(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_dma_rint_ena_w1c
 *
 * SDP/DPI DTIME/DCNT/DMAFI Interrupt Remote Enable Clear Registers
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_epfx_dma_rint_ena_w1c
{
    uint64_t u;
    struct cavm_sdpx_epfx_dma_rint_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_6_63         : 58;
        uint64_t dtime                 : 2;  /**< [  5:  4](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_DMA_RINT[DTIME]. */
        uint64_t dcnt                  : 2;  /**< [  3:  2](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_DMA_RINT[DCNT]. */
        uint64_t dmafi                 : 2;  /**< [  1:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_DMA_RINT[DMAFI]. */
#else /* Word 0 - Little Endian */
        uint64_t dmafi                 : 2;  /**< [  1:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_DMA_RINT[DMAFI]. */
        uint64_t dcnt                  : 2;  /**< [  3:  2](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_DMA_RINT[DCNT]. */
        uint64_t dtime                 : 2;  /**< [  5:  4](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_DMA_RINT[DTIME]. */
        uint64_t reserved_6_63         : 58;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_dma_rint_ena_w1c_s cn; */
};
typedef union cavm_sdpx_epfx_dma_rint_ena_w1c cavm_sdpx_epfx_dma_rint_ena_w1c_t;

static inline uint64_t CAVM_SDPX_EPFX_DMA_RINT_ENA_W1C(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_DMA_RINT_ENA_W1C(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020420ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_DMA_RINT_ENA_W1C", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_DMA_RINT_ENA_W1C(a,b) cavm_sdpx_epfx_dma_rint_ena_w1c_t
#define bustype_CAVM_SDPX_EPFX_DMA_RINT_ENA_W1C(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_DMA_RINT_ENA_W1C(a,b) "SDPX_EPFX_DMA_RINT_ENA_W1C"
#define device_bar_CAVM_SDPX_EPFX_DMA_RINT_ENA_W1C(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_DMA_RINT_ENA_W1C(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_DMA_RINT_ENA_W1C(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_dma_rint_ena_w1s
 *
 * SDP/DPI DTIME/DCNT/DMAFI Interrupt Remote Enable Set Registers
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_epfx_dma_rint_ena_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_dma_rint_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_6_63         : 58;
        uint64_t dtime                 : 2;  /**< [  5:  4](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_DMA_RINT[DTIME]. */
        uint64_t dcnt                  : 2;  /**< [  3:  2](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_DMA_RINT[DCNT]. */
        uint64_t dmafi                 : 2;  /**< [  1:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_DMA_RINT[DMAFI]. */
#else /* Word 0 - Little Endian */
        uint64_t dmafi                 : 2;  /**< [  1:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_DMA_RINT[DMAFI]. */
        uint64_t dcnt                  : 2;  /**< [  3:  2](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_DMA_RINT[DCNT]. */
        uint64_t dtime                 : 2;  /**< [  5:  4](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_DMA_RINT[DTIME]. */
        uint64_t reserved_6_63         : 58;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_dma_rint_ena_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_dma_rint_ena_w1s cavm_sdpx_epfx_dma_rint_ena_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_DMA_RINT_ENA_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_DMA_RINT_ENA_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020430ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_DMA_RINT_ENA_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_DMA_RINT_ENA_W1S(a,b) cavm_sdpx_epfx_dma_rint_ena_w1s_t
#define bustype_CAVM_SDPX_EPFX_DMA_RINT_ENA_W1S(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_DMA_RINT_ENA_W1S(a,b) "SDPX_EPFX_DMA_RINT_ENA_W1S"
#define device_bar_CAVM_SDPX_EPFX_DMA_RINT_ENA_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_DMA_RINT_ENA_W1S(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_DMA_RINT_ENA_W1S(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_dma_rint_w1s
 *
 * SDP/DPI DTIME/DCNT/DMAFI Interrupt Set Registers
 * This register sets interrupt bits.
 */
union cavm_sdpx_epfx_dma_rint_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_dma_rint_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_6_63         : 58;
        uint64_t dtime                 : 2;  /**< [  5:  4](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_DMA_RINT[DTIME]. */
        uint64_t dcnt                  : 2;  /**< [  3:  2](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_DMA_RINT[DCNT]. */
        uint64_t dmafi                 : 2;  /**< [  1:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_DMA_RINT[DMAFI]. */
#else /* Word 0 - Little Endian */
        uint64_t dmafi                 : 2;  /**< [  1:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_DMA_RINT[DMAFI]. */
        uint64_t dcnt                  : 2;  /**< [  3:  2](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_DMA_RINT[DCNT]. */
        uint64_t dtime                 : 2;  /**< [  5:  4](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_DMA_RINT[DTIME]. */
        uint64_t reserved_6_63         : 58;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_dma_rint_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_dma_rint_w1s cavm_sdpx_epfx_dma_rint_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_DMA_RINT_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_DMA_RINT_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020410ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_DMA_RINT_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_DMA_RINT_W1S(a,b) cavm_sdpx_epfx_dma_rint_w1s_t
#define bustype_CAVM_SDPX_EPFX_DMA_RINT_W1S(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_DMA_RINT_W1S(a,b) "SDPX_EPFX_DMA_RINT_W1S"
#define device_bar_CAVM_SDPX_EPFX_DMA_RINT_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_DMA_RINT_W1S(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_DMA_RINT_W1S(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_dma_tim#
 *
 * SDP DMA Timer Registers
 * These registers contain the DMA timer values.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_dma_timx
{
    uint64_t u;
    struct cavm_sdpx_epfx_dma_timx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t tim                   : 32; /**< [ 31:  0](RO/H) The DMA timer value. The timer increments when
                                                                 SDP()_EPF()_DMA_CNT()[CNT]!=0 and clears when SDP()_EPF()_DMA_RINT[DTIME]\<x\> is
                                                                 written with one. */
#else /* Word 0 - Little Endian */
        uint64_t tim                   : 32; /**< [ 31:  0](RO/H) The DMA timer value. The timer increments when
                                                                 SDP()_EPF()_DMA_CNT()[CNT]!=0 and clears when SDP()_EPF()_DMA_RINT[DTIME]\<x\> is
                                                                 written with one. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_dma_timx_s cn; */
};
typedef union cavm_sdpx_epfx_dma_timx cavm_sdpx_epfx_dma_timx_t;

static inline uint64_t CAVM_SDPX_EPFX_DMA_TIMX(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_DMA_TIMX(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020480ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_DMA_TIMX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_DMA_TIMX(a,b,c) cavm_sdpx_epfx_dma_timx_t
#define bustype_CAVM_SDPX_EPFX_DMA_TIMX(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_DMA_TIMX(a,b,c) "SDPX_EPFX_DMA_TIMX"
#define device_bar_CAVM_SDPX_EPFX_DMA_TIMX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_DMA_TIMX(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_DMA_TIMX(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_dma_vf_rint#
 *
 * SDP DMA Error Response VF Bit Array Registers
 * When an error response is received for a VF DMA transaction read, the appropriate VF indexed
 * bit is set.  The appropriate PF should read the appropriate register.
 * Each EPF supports 128 VFs.
 * Register index 0 refers to the VF functions 1..64 of the EPF.
 * Register index 1 refers to the VF functions 65..128 of the EPF.
 * The given register associated with an EPF will be reset due to a PF FLR or MAC reset.
 * These registers are not affected by VF FLR.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_dma_vf_rintx
{
    uint64_t u;
    struct cavm_sdpx_epfx_dma_vf_rintx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) When an error response is received for a VF DMA transaction read, the appropriate VF
                                                                 indexed bit is set. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) When an error response is received for a VF DMA transaction read, the appropriate VF
                                                                 indexed bit is set. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_dma_vf_rintx_s cn; */
};
typedef union cavm_sdpx_epfx_dma_vf_rintx cavm_sdpx_epfx_dma_vf_rintx_t;

static inline uint64_t CAVM_SDPX_EPFX_DMA_VF_RINTX(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_DMA_VF_RINTX(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e0800204e0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_DMA_VF_RINTX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_DMA_VF_RINTX(a,b,c) cavm_sdpx_epfx_dma_vf_rintx_t
#define bustype_CAVM_SDPX_EPFX_DMA_VF_RINTX(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_DMA_VF_RINTX(a,b,c) "SDPX_EPFX_DMA_VF_RINTX"
#define device_bar_CAVM_SDPX_EPFX_DMA_VF_RINTX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_DMA_VF_RINTX(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_DMA_VF_RINTX(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_dma_vf_rint#_ena_w1c
 *
 * SDP DMA Error Response VF Bit Array Local Enable Clear Registers
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_epfx_dma_vf_rintx_ena_w1c
{
    uint64_t u;
    struct cavm_sdpx_epfx_dma_vf_rintx_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_DMA_VF_RINT(0..1)[VF_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_DMA_VF_RINT(0..1)[VF_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_dma_vf_rintx_ena_w1c_s cn; */
};
typedef union cavm_sdpx_epfx_dma_vf_rintx_ena_w1c cavm_sdpx_epfx_dma_vf_rintx_ena_w1c_t;

static inline uint64_t CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020520ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_DMA_VF_RINTX_ENA_W1C", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1C(a,b,c) cavm_sdpx_epfx_dma_vf_rintx_ena_w1c_t
#define bustype_CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1C(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1C(a,b,c) "SDPX_EPFX_DMA_VF_RINTX_ENA_W1C"
#define device_bar_CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1C(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1C(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1C(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_dma_vf_rint#_ena_w1s
 *
 * SDP DMA Error Response VF Bit Array Local Enable Set Registers
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_epfx_dma_vf_rintx_ena_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_dma_vf_rintx_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_DMA_VF_RINT(0..1)[VF_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_DMA_VF_RINT(0..1)[VF_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_dma_vf_rintx_ena_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_dma_vf_rintx_ena_w1s cavm_sdpx_epfx_dma_vf_rintx_ena_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020540ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_DMA_VF_RINTX_ENA_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1S(a,b,c) cavm_sdpx_epfx_dma_vf_rintx_ena_w1s_t
#define bustype_CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1S(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1S(a,b,c) "SDPX_EPFX_DMA_VF_RINTX_ENA_W1S"
#define device_bar_CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1S(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_DMA_VF_RINTX_ENA_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_dma_vf_rint#_w1s
 *
 * SDP DMA Error Response VF Bit Array Set Registers
 * This register sets interrupt bits.
 */
union cavm_sdpx_epfx_dma_vf_rintx_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_dma_vf_rintx_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_DMA_VF_RINT(0..1)[VF_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_DMA_VF_RINT(0..1)[VF_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_dma_vf_rintx_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_dma_vf_rintx_w1s cavm_sdpx_epfx_dma_vf_rintx_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_DMA_VF_RINTX_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_DMA_VF_RINTX_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020500ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_DMA_VF_RINTX_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_DMA_VF_RINTX_W1S(a,b,c) cavm_sdpx_epfx_dma_vf_rintx_w1s_t
#define bustype_CAVM_SDPX_EPFX_DMA_VF_RINTX_W1S(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_DMA_VF_RINTX_W1S(a,b,c) "SDPX_EPFX_DMA_VF_RINTX_W1S"
#define device_bar_CAVM_SDPX_EPFX_DMA_VF_RINTX_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_DMA_VF_RINTX_W1S(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_DMA_VF_RINTX_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_ire_rint
 *
 * SDP Input Error Status Register
 * This register indicates if an error has been detected on an input ring.
 * The given register associated with an EPF will be reset due to a PF FLR or MAC Reset.
 * These registers are not affected by VF FLR.
 */
union cavm_sdpx_epfx_ire_rint
{
    uint64_t u;
    struct cavm_sdpx_epfx_ire_rint_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Error has been detected on input ring i. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Error has been detected on input ring i. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_ire_rint_s cn; */
};
typedef union cavm_sdpx_epfx_ire_rint cavm_sdpx_epfx_ire_rint_t;

static inline uint64_t CAVM_SDPX_EPFX_IRE_RINT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_IRE_RINT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020200ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_IRE_RINT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_IRE_RINT(a,b) cavm_sdpx_epfx_ire_rint_t
#define bustype_CAVM_SDPX_EPFX_IRE_RINT(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_IRE_RINT(a,b) "SDPX_EPFX_IRE_RINT"
#define device_bar_CAVM_SDPX_EPFX_IRE_RINT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_IRE_RINT(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_IRE_RINT(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_ire_rint_ena_w1c
 *
 * SDP Input Error Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_epfx_ire_rint_ena_w1c
{
    uint64_t u;
    struct cavm_sdpx_epfx_ire_rint_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_IRE_RINT[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_IRE_RINT[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_ire_rint_ena_w1c_s cn; */
};
typedef union cavm_sdpx_epfx_ire_rint_ena_w1c cavm_sdpx_epfx_ire_rint_ena_w1c_t;

static inline uint64_t CAVM_SDPX_EPFX_IRE_RINT_ENA_W1C(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_IRE_RINT_ENA_W1C(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020220ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_IRE_RINT_ENA_W1C", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_IRE_RINT_ENA_W1C(a,b) cavm_sdpx_epfx_ire_rint_ena_w1c_t
#define bustype_CAVM_SDPX_EPFX_IRE_RINT_ENA_W1C(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_IRE_RINT_ENA_W1C(a,b) "SDPX_EPFX_IRE_RINT_ENA_W1C"
#define device_bar_CAVM_SDPX_EPFX_IRE_RINT_ENA_W1C(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_IRE_RINT_ENA_W1C(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_IRE_RINT_ENA_W1C(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_ire_rint_ena_w1s
 *
 * SDP Input Error Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_epfx_ire_rint_ena_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_ire_rint_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_IRE_RINT[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_IRE_RINT[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_ire_rint_ena_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_ire_rint_ena_w1s cavm_sdpx_epfx_ire_rint_ena_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_IRE_RINT_ENA_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_IRE_RINT_ENA_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020230ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_IRE_RINT_ENA_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_IRE_RINT_ENA_W1S(a,b) cavm_sdpx_epfx_ire_rint_ena_w1s_t
#define bustype_CAVM_SDPX_EPFX_IRE_RINT_ENA_W1S(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_IRE_RINT_ENA_W1S(a,b) "SDPX_EPFX_IRE_RINT_ENA_W1S"
#define device_bar_CAVM_SDPX_EPFX_IRE_RINT_ENA_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_IRE_RINT_ENA_W1S(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_IRE_RINT_ENA_W1S(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_ire_rint_w1s
 *
 * SDP Input Error Status Set Register
 * This register sets interrupt bits.
 */
union cavm_sdpx_epfx_ire_rint_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_ire_rint_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_IRE_RINT[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_IRE_RINT[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_ire_rint_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_ire_rint_w1s cavm_sdpx_epfx_ire_rint_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_IRE_RINT_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_IRE_RINT_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020210ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_IRE_RINT_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_IRE_RINT_W1S(a,b) cavm_sdpx_epfx_ire_rint_w1s_t
#define bustype_CAVM_SDPX_EPFX_IRE_RINT_W1S(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_IRE_RINT_W1S(a,b) "SDPX_EPFX_IRE_RINT_W1S"
#define device_bar_CAVM_SDPX_EPFX_IRE_RINT_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_IRE_RINT_W1S(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_IRE_RINT_W1S(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_ism_msix_recovery
 *
 * SDP Ring Information Register
 * This register can be used to implement a minimum delay between sending MSI-X messages
 * and ISM messages.
 * This could be used to prevent a misbehaving VF from flooding the system with MSI-X
 * or ISM writes and starving traffic from other VFs.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_ism_msix_recovery
{
    uint64_t u;
    struct cavm_sdpx_epfx_ism_msix_recovery_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_12_63        : 52;
        uint64_t delay                 : 12; /**< [ 11:  0](R/W) Delay value in system clock cycles. */
#else /* Word 0 - Little Endian */
        uint64_t delay                 : 12; /**< [ 11:  0](R/W) Delay value in system clock cycles. */
        uint64_t reserved_12_63        : 52;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_ism_msix_recovery_s cn; */
};
typedef union cavm_sdpx_epfx_ism_msix_recovery cavm_sdpx_epfx_ism_msix_recovery_t;

static inline uint64_t CAVM_SDPX_EPFX_ISM_MSIX_RECOVERY(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_ISM_MSIX_RECOVERY(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020600ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_ISM_MSIX_RECOVERY", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_ISM_MSIX_RECOVERY(a,b) cavm_sdpx_epfx_ism_msix_recovery_t
#define bustype_CAVM_SDPX_EPFX_ISM_MSIX_RECOVERY(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_ISM_MSIX_RECOVERY(a,b) "SDPX_EPFX_ISM_MSIX_RECOVERY"
#define device_bar_CAVM_SDPX_EPFX_ISM_MSIX_RECOVERY(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_ISM_MSIX_RECOVERY(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_ISM_MSIX_RECOVERY(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_mbox_rint#
 *
 * SDP Mailbox Interrupt Status Register
 * This register indicates which VF/ring has signaled an interrupt.
 * Register index 0 refers to the VF functions 1..64 of the EPF.
 * Register index 1 refers to the VF functions 65..128 of the EPF.
 * The given register associated with an EPF will be reset due to a PF FLR or MAC Reset.
 * These registers are not affected by VF FLR.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_mbox_rintx
{
    uint64_t u;
    struct cavm_sdpx_epfx_mbox_rintx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_num              : 64; /**< [ 63:  0](R/W1C/H) Each bit indicates a ring from 0-63. */
#else /* Word 0 - Little Endian */
        uint64_t ring_num              : 64; /**< [ 63:  0](R/W1C/H) Each bit indicates a ring from 0-63. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_mbox_rintx_s cn; */
};
typedef union cavm_sdpx_epfx_mbox_rintx cavm_sdpx_epfx_mbox_rintx_t;

static inline uint64_t CAVM_SDPX_EPFX_MBOX_RINTX(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_MBOX_RINTX(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020100ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_MBOX_RINTX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_MBOX_RINTX(a,b,c) cavm_sdpx_epfx_mbox_rintx_t
#define bustype_CAVM_SDPX_EPFX_MBOX_RINTX(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_MBOX_RINTX(a,b,c) "SDPX_EPFX_MBOX_RINTX"
#define device_bar_CAVM_SDPX_EPFX_MBOX_RINTX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_MBOX_RINTX(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_MBOX_RINTX(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_mbox_rint#_ena_w1c
 *
 * SDP Mailbox Interrupt Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_epfx_mbox_rintx_ena_w1c
{
    uint64_t u;
    struct cavm_sdpx_epfx_mbox_rintx_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_num              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MBOX_RINT(0..1)[RING_NUM]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_num              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MBOX_RINT(0..1)[RING_NUM]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_mbox_rintx_ena_w1c_s cn; */
};
typedef union cavm_sdpx_epfx_mbox_rintx_ena_w1c cavm_sdpx_epfx_mbox_rintx_ena_w1c_t;

static inline uint64_t CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020140ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_MBOX_RINTX_ENA_W1C", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1C(a,b,c) cavm_sdpx_epfx_mbox_rintx_ena_w1c_t
#define bustype_CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1C(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1C(a,b,c) "SDPX_EPFX_MBOX_RINTX_ENA_W1C"
#define device_bar_CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1C(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1C(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1C(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_mbox_rint#_ena_w1s
 *
 * SDP Mailbox Interrupt Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_epfx_mbox_rintx_ena_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_mbox_rintx_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_num              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MBOX_RINT(0..1)[RING_NUM]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_num              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MBOX_RINT(0..1)[RING_NUM]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_mbox_rintx_ena_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_mbox_rintx_ena_w1s cavm_sdpx_epfx_mbox_rintx_ena_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020160ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_MBOX_RINTX_ENA_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1S(a,b,c) cavm_sdpx_epfx_mbox_rintx_ena_w1s_t
#define bustype_CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1S(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1S(a,b,c) "SDPX_EPFX_MBOX_RINTX_ENA_W1S"
#define device_bar_CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1S(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_MBOX_RINTX_ENA_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_mbox_rint#_w1s
 *
 * SDP Mailbox Interrupt Set Register
 * This register sets interrupt bits.
 */
union cavm_sdpx_epfx_mbox_rintx_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_mbox_rintx_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_num              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MBOX_RINT(0..1)[RING_NUM]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_num              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MBOX_RINT(0..1)[RING_NUM]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_mbox_rintx_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_mbox_rintx_w1s cavm_sdpx_epfx_mbox_rintx_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_MBOX_RINTX_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_MBOX_RINTX_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020120ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_MBOX_RINTX_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_MBOX_RINTX_W1S(a,b,c) cavm_sdpx_epfx_mbox_rintx_w1s_t
#define bustype_CAVM_SDPX_EPFX_MBOX_RINTX_W1S(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_MBOX_RINTX_W1S(a,b,c) "SDPX_EPFX_MBOX_RINTX_W1S"
#define device_bar_CAVM_SDPX_EPFX_MBOX_RINTX_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_MBOX_RINTX_W1S(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_MBOX_RINTX_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_misc_rint
 *
 * SDP MAC Interrupt Summary Register
 * This register contains the different interrupt-summary bits for one MAC in the SDP.
 * The given register associated with an EPF will be reset due to a PF FLR or MAC reset.
 * These registers are not affected by VF FLR.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_misc_rint
{
    uint64_t u;
    struct cavm_sdpx_epfx_misc_rint_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_6_63         : 58;
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1C/H) Set when an error response is received for a PF DMA transaction read. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1C/H) Set when an error response is received for a PF PP transaction read. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1C/H) Received unsupported N-TLP for window register from the corresponding MAC. This
                                                                 occurs when the window registers are disabled and a window register access
                                                                 occurs. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1C/H) Received unsupported N-TLP for Bar 0 from the corresponding MAC. This occurs
                                                                 when the BAR 0 address space is disabled. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1C/H) Received unsupported P-TLP for window register from the corresponding MAC. This
                                                                 occurs when the window registers are disabled and a window register access
                                                                 occurs. */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1C/H) Received unsupported P-TLP for Bar 0 from the corresponding MAC. This occurs
                                                                 when the BAR 0 address space is disabled. */
#else /* Word 0 - Little Endian */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1C/H) Received unsupported P-TLP for Bar 0 from the corresponding MAC. This occurs
                                                                 when the BAR 0 address space is disabled. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1C/H) Received unsupported P-TLP for window register from the corresponding MAC. This
                                                                 occurs when the window registers are disabled and a window register access
                                                                 occurs. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1C/H) Received unsupported N-TLP for Bar 0 from the corresponding MAC. This occurs
                                                                 when the BAR 0 address space is disabled. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1C/H) Received unsupported N-TLP for window register from the corresponding MAC. This
                                                                 occurs when the window registers are disabled and a window register access
                                                                 occurs. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1C/H) Set when an error response is received for a PF PP transaction read. */
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1C/H) Set when an error response is received for a PF DMA transaction read. */
        uint64_t reserved_6_63         : 58;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_misc_rint_s cn; */
};
typedef union cavm_sdpx_epfx_misc_rint cavm_sdpx_epfx_misc_rint_t;

static inline uint64_t CAVM_SDPX_EPFX_MISC_RINT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_MISC_RINT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e0800204a0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_MISC_RINT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_MISC_RINT(a,b) cavm_sdpx_epfx_misc_rint_t
#define bustype_CAVM_SDPX_EPFX_MISC_RINT(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_MISC_RINT(a,b) "SDPX_EPFX_MISC_RINT"
#define device_bar_CAVM_SDPX_EPFX_MISC_RINT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_MISC_RINT(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_MISC_RINT(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_misc_rint_ena_w1c
 *
 * SDP MAC Interrupt Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_epfx_misc_rint_ena_w1c
{
    uint64_t u;
    struct cavm_sdpx_epfx_misc_rint_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_6_63         : 58;
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MISC_RINT[DMAPF_ERR]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MISC_RINT[PPPF_ERR]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MISC_RINT[UN_WI]. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MISC_RINT[UN_B0]. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MISC_RINT[UP_WI]. */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MISC_RINT[UP_B0]. */
#else /* Word 0 - Little Endian */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MISC_RINT[UP_B0]. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MISC_RINT[UP_WI]. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MISC_RINT[UN_B0]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MISC_RINT[UN_WI]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MISC_RINT[PPPF_ERR]. */
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_MISC_RINT[DMAPF_ERR]. */
        uint64_t reserved_6_63         : 58;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_misc_rint_ena_w1c_s cn; */
};
typedef union cavm_sdpx_epfx_misc_rint_ena_w1c cavm_sdpx_epfx_misc_rint_ena_w1c_t;

static inline uint64_t CAVM_SDPX_EPFX_MISC_RINT_ENA_W1C(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_MISC_RINT_ENA_W1C(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e0800204c0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_MISC_RINT_ENA_W1C", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_MISC_RINT_ENA_W1C(a,b) cavm_sdpx_epfx_misc_rint_ena_w1c_t
#define bustype_CAVM_SDPX_EPFX_MISC_RINT_ENA_W1C(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_MISC_RINT_ENA_W1C(a,b) "SDPX_EPFX_MISC_RINT_ENA_W1C"
#define device_bar_CAVM_SDPX_EPFX_MISC_RINT_ENA_W1C(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_MISC_RINT_ENA_W1C(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_MISC_RINT_ENA_W1C(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_misc_rint_ena_w1s
 *
 * SDP MAC Interrupt Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_epfx_misc_rint_ena_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_misc_rint_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_6_63         : 58;
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MISC_RINT[DMAPF_ERR]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MISC_RINT[PPPF_ERR]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MISC_RINT[UN_WI]. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MISC_RINT[UN_B0]. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MISC_RINT[UP_WI]. */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MISC_RINT[UP_B0]. */
#else /* Word 0 - Little Endian */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MISC_RINT[UP_B0]. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MISC_RINT[UP_WI]. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MISC_RINT[UN_B0]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MISC_RINT[UN_WI]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MISC_RINT[PPPF_ERR]. */
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_MISC_RINT[DMAPF_ERR]. */
        uint64_t reserved_6_63         : 58;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_misc_rint_ena_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_misc_rint_ena_w1s cavm_sdpx_epfx_misc_rint_ena_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_MISC_RINT_ENA_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_MISC_RINT_ENA_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e0800204d0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_MISC_RINT_ENA_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_MISC_RINT_ENA_W1S(a,b) cavm_sdpx_epfx_misc_rint_ena_w1s_t
#define bustype_CAVM_SDPX_EPFX_MISC_RINT_ENA_W1S(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_MISC_RINT_ENA_W1S(a,b) "SDPX_EPFX_MISC_RINT_ENA_W1S"
#define device_bar_CAVM_SDPX_EPFX_MISC_RINT_ENA_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_MISC_RINT_ENA_W1S(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_MISC_RINT_ENA_W1S(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_misc_rint_w1s
 *
 * SDP MAC Interrupt Set Register
 * This register sets interrupt bits.
 */
union cavm_sdpx_epfx_misc_rint_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_misc_rint_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_6_63         : 58;
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MISC_RINT[DMAPF_ERR]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MISC_RINT[PPPF_ERR]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MISC_RINT[UN_WI]. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MISC_RINT[UN_B0]. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MISC_RINT[UP_WI]. */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MISC_RINT[UP_B0]. */
#else /* Word 0 - Little Endian */
        uint64_t up_b0                 : 1;  /**< [  0:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MISC_RINT[UP_B0]. */
        uint64_t up_wi                 : 1;  /**< [  1:  1](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MISC_RINT[UP_WI]. */
        uint64_t un_b0                 : 1;  /**< [  2:  2](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MISC_RINT[UN_B0]. */
        uint64_t un_wi                 : 1;  /**< [  3:  3](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MISC_RINT[UN_WI]. */
        uint64_t pppf_err              : 1;  /**< [  4:  4](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MISC_RINT[PPPF_ERR]. */
        uint64_t dmapf_err             : 1;  /**< [  5:  5](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_MISC_RINT[DMAPF_ERR]. */
        uint64_t reserved_6_63         : 58;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_misc_rint_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_misc_rint_w1s cavm_sdpx_epfx_misc_rint_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_MISC_RINT_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_MISC_RINT_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e0800204b0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_MISC_RINT_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_MISC_RINT_W1S(a,b) cavm_sdpx_epfx_misc_rint_w1s_t
#define bustype_CAVM_SDPX_EPFX_MISC_RINT_W1S(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_MISC_RINT_W1S(a,b) "SDPX_EPFX_MISC_RINT_W1S"
#define device_bar_CAVM_SDPX_EPFX_MISC_RINT_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_MISC_RINT_W1S(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_MISC_RINT_W1S(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_oei_rint
 *
 * SDP Output Endpoint Interrupt Register
 * Interrupt data for interrupts sent to remote hosts.
 * This register is reset due to PF FLR, it is not affected by VF FLR.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_oei_rint
{
    uint64_t u;
    struct cavm_sdpx_epfx_oei_rint_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1C/H) Data for remote host. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1C/H) Data for remote host. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_oei_rint_s cn; */
};
typedef union cavm_sdpx_epfx_oei_rint cavm_sdpx_epfx_oei_rint_t;

static inline uint64_t CAVM_SDPX_EPFX_OEI_RINT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_OEI_RINT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=3)))
        return 0x874080020140ll + 0x1000000000ll * ((a) & 0x0) + 0x800000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020360ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_OEI_RINT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_OEI_RINT(a,b) cavm_sdpx_epfx_oei_rint_t
#define bustype_CAVM_SDPX_EPFX_OEI_RINT(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_OEI_RINT(a,b) "SDPX_EPFX_OEI_RINT"
#define device_bar_CAVM_SDPX_EPFX_OEI_RINT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_OEI_RINT(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_OEI_RINT(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_oei_rint_ena_w1c
 *
 * SDP Output Endpoint Interrupt Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_epfx_oei_rint_ena_w1c
{
    uint64_t u;
    struct cavm_sdpx_epfx_oei_rint_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..3)_OEI_RINT[DATA]. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..3)_OEI_RINT[DATA]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_oei_rint_ena_w1c_s cn8; */
    struct cavm_sdpx_epfx_oei_rint_ena_w1c_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_OEI_RINT[DATA]. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_OEI_RINT[DATA]. */
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_sdpx_epfx_oei_rint_ena_w1c cavm_sdpx_epfx_oei_rint_ena_w1c_t;

static inline uint64_t CAVM_SDPX_EPFX_OEI_RINT_ENA_W1C(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_OEI_RINT_ENA_W1C(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=3)))
        return 0x874080020160ll + 0x1000000000ll * ((a) & 0x0) + 0x800000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020380ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_OEI_RINT_ENA_W1C", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_OEI_RINT_ENA_W1C(a,b) cavm_sdpx_epfx_oei_rint_ena_w1c_t
#define bustype_CAVM_SDPX_EPFX_OEI_RINT_ENA_W1C(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_OEI_RINT_ENA_W1C(a,b) "SDPX_EPFX_OEI_RINT_ENA_W1C"
#define device_bar_CAVM_SDPX_EPFX_OEI_RINT_ENA_W1C(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_OEI_RINT_ENA_W1C(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_OEI_RINT_ENA_W1C(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_oei_rint_ena_w1s
 *
 * SDP Output Endpoint Interrupt Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_epfx_oei_rint_ena_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_oei_rint_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..3)_OEI_RINT[DATA]. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..3)_OEI_RINT[DATA]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_oei_rint_ena_w1s_s cn8; */
    struct cavm_sdpx_epfx_oei_rint_ena_w1s_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_OEI_RINT[DATA]. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_OEI_RINT[DATA]. */
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_sdpx_epfx_oei_rint_ena_w1s cavm_sdpx_epfx_oei_rint_ena_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_OEI_RINT_ENA_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_OEI_RINT_ENA_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=3)))
        return 0x874080020170ll + 0x1000000000ll * ((a) & 0x0) + 0x800000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020390ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_OEI_RINT_ENA_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_OEI_RINT_ENA_W1S(a,b) cavm_sdpx_epfx_oei_rint_ena_w1s_t
#define bustype_CAVM_SDPX_EPFX_OEI_RINT_ENA_W1S(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_OEI_RINT_ENA_W1S(a,b) "SDPX_EPFX_OEI_RINT_ENA_W1S"
#define device_bar_CAVM_SDPX_EPFX_OEI_RINT_ENA_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_OEI_RINT_ENA_W1S(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_OEI_RINT_ENA_W1S(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_oei_rint_w1s
 *
 * SDP Output Endpoint Interrupt Set Register
 * This register sets interrupt bits.
 */
union cavm_sdpx_epfx_oei_rint_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_oei_rint_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..3)_OEI_RINT[DATA]. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..3)_OEI_RINT[DATA]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_oei_rint_w1s_s cn8; */
    struct cavm_sdpx_epfx_oei_rint_w1s_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_OEI_RINT[DATA]. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_OEI_RINT[DATA]. */
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_sdpx_epfx_oei_rint_w1s cavm_sdpx_epfx_oei_rint_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_OEI_RINT_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_OEI_RINT_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=3)))
        return 0x874080020150ll + 0x1000000000ll * ((a) & 0x0) + 0x800000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020370ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_OEI_RINT_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_OEI_RINT_W1S(a,b) cavm_sdpx_epfx_oei_rint_w1s_t
#define bustype_CAVM_SDPX_EPFX_OEI_RINT_W1S(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_OEI_RINT_W1S(a,b) "SDPX_EPFX_OEI_RINT_W1S"
#define device_bar_CAVM_SDPX_EPFX_OEI_RINT_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_OEI_RINT_W1S(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_OEI_RINT_W1S(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_epf#_oei_trig
 *
 * SDP Output Endpoint Interrupt Trigger Register
 * This trigger register can be used to generate outbound interrupts to the remote host
 * when one of the supported MACs is configured in endpoint mode. This trigger register
 * can either be written directly by the local CPU, or it can be indirectly written by
 * hardware by configuring a local MSI-X table with the address of this register, after
 * translation by the SMMU.  The [SET] field is used for edge triggered interrupts and
 * [SET] and [CLR] can be used to mimic a level sensitive interrupt.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 *
 * Internal:
 * These registers must be on a dedicated 64KB page to isolate guests from changing other CSRs.
 */
union cavm_sdpx_epfx_oei_trig
{
    uint64_t u;
    struct cavm_sdpx_epfx_oei_trig_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_20_63        : 44;
        uint64_t set                   : 1;  /**< [ 19: 19](R/W1S/H) When written with a 1, bit number [BIT_NUM] will be set in
                                                                 SDP()_EPF()_OEI_RINT_W1S. If the outbound interrupt is enabled in
                                                                 SDP()_EPF()_OEI_RINT_ENA_W1S, an MSI-X message will be generated to the remote
                                                                 host. If [CLR] is also being written with 1, the operation will be ignored. */
        uint64_t clr                   : 1;  /**< [ 18: 18](R/W1C/H) When written with a 1, bit number [BIT_NUM] will be cleared in
                                                                 SDP()_EPF()_OEI_RINT_W1S. If the outbound interrupt is enabled in
                                                                 SDP()_EPF()_OEI_RINT_ENA_W1S, an MSI-X message will be generated to the remote
                                                                 host. If [SET] is also being written with 1, the operation will be ignored. */
        uint64_t reserved_6_17         : 12;
        uint64_t bit_num               : 6;  /**< [  5:  0](R/W) Which bit number is to be set or cleared. */
#else /* Word 0 - Little Endian */
        uint64_t bit_num               : 6;  /**< [  5:  0](R/W) Which bit number is to be set or cleared. */
        uint64_t reserved_6_17         : 12;
        uint64_t clr                   : 1;  /**< [ 18: 18](R/W1C/H) When written with a 1, bit number [BIT_NUM] will be cleared in
                                                                 SDP()_EPF()_OEI_RINT_W1S. If the outbound interrupt is enabled in
                                                                 SDP()_EPF()_OEI_RINT_ENA_W1S, an MSI-X message will be generated to the remote
                                                                 host. If [SET] is also being written with 1, the operation will be ignored. */
        uint64_t set                   : 1;  /**< [ 19: 19](R/W1S/H) When written with a 1, bit number [BIT_NUM] will be set in
                                                                 SDP()_EPF()_OEI_RINT_W1S. If the outbound interrupt is enabled in
                                                                 SDP()_EPF()_OEI_RINT_ENA_W1S, an MSI-X message will be generated to the remote
                                                                 host. If [CLR] is also being written with 1, the operation will be ignored. */
        uint64_t reserved_20_63        : 44;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_oei_trig_s cn8; */
    struct cavm_sdpx_epfx_oei_trig_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_20_63        : 44;
        uint64_t set                   : 1;  /**< [ 19: 19](WO/H) When written with a 1, bit number [BIT_NUM] will be set in
                                                                 SDP()_EPF()_OEI_RINT_W1S. If the outbound interrupt is enabled in
                                                                 SDP()_EPF()_OEI_RINT_ENA_W1S, an MSI-X message will be generated to the remote
                                                                 host. If [CLR] is also being written with 1, the operation will be ignored. */
        uint64_t clr                   : 1;  /**< [ 18: 18](WO/H) When written with a 1, bit number [BIT_NUM] will be cleared in
                                                                 SDP()_EPF()_OEI_RINT_W1S. If the outbound interrupt is enabled in
                                                                 SDP()_EPF()_OEI_RINT_ENA_W1S, an MSI-X message will be generated to the remote
                                                                 host. If [SET] is also being written with 1, the operation will be ignored. */
        uint64_t reserved_6_17         : 12;
        uint64_t bit_num               : 6;  /**< [  5:  0](WO/H) Which bit number is to be set or cleared. */
#else /* Word 0 - Little Endian */
        uint64_t bit_num               : 6;  /**< [  5:  0](WO/H) Which bit number is to be set or cleared. */
        uint64_t reserved_6_17         : 12;
        uint64_t clr                   : 1;  /**< [ 18: 18](WO/H) When written with a 1, bit number [BIT_NUM] will be cleared in
                                                                 SDP()_EPF()_OEI_RINT_W1S. If the outbound interrupt is enabled in
                                                                 SDP()_EPF()_OEI_RINT_ENA_W1S, an MSI-X message will be generated to the remote
                                                                 host. If [SET] is also being written with 1, the operation will be ignored. */
        uint64_t set                   : 1;  /**< [ 19: 19](WO/H) When written with a 1, bit number [BIT_NUM] will be set in
                                                                 SDP()_EPF()_OEI_RINT_W1S. If the outbound interrupt is enabled in
                                                                 SDP()_EPF()_OEI_RINT_ENA_W1S, an MSI-X message will be generated to the remote
                                                                 host. If [CLR] is also being written with 1, the operation will be ignored. */
        uint64_t reserved_20_63        : 44;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_sdpx_epfx_oei_trig cavm_sdpx_epfx_oei_trig_t;

static inline uint64_t CAVM_SDPX_EPFX_OEI_TRIG(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_OEI_TRIG(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=3)))
        return 0x874000800000ll + 0x1000000000ll * ((a) & 0x0) + 0x10000ll * ((b) & 0x3);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e0c0000000ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_OEI_TRIG", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_OEI_TRIG(a,b) cavm_sdpx_epfx_oei_trig_t
#define bustype_CAVM_SDPX_EPFX_OEI_TRIG(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_EPFX_OEI_TRIG(a,b) "SDPX_EPFX_OEI_TRIG"
#define device_bar_CAVM_SDPX_EPFX_OEI_TRIG(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_OEI_TRIG(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_OEI_TRIG(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_ore_rint
 *
 * SDP Output Error Status Register
 * This register indicates if an error has been detected on an output ring.
 * The given register associated with an EPF will be reset due to a PF FLR or MAC Reset.
 * These registers are not affected by VF FLR.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_ore_rint
{
    uint64_t u;
    struct cavm_sdpx_epfx_ore_rint_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Error has been detected on ring output ring i. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Error has been detected on ring output ring i. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_ore_rint_s cn; */
};
typedef union cavm_sdpx_epfx_ore_rint cavm_sdpx_epfx_ore_rint_t;

static inline uint64_t CAVM_SDPX_EPFX_ORE_RINT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_ORE_RINT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020320ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_ORE_RINT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_ORE_RINT(a,b) cavm_sdpx_epfx_ore_rint_t
#define bustype_CAVM_SDPX_EPFX_ORE_RINT(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_ORE_RINT(a,b) "SDPX_EPFX_ORE_RINT"
#define device_bar_CAVM_SDPX_EPFX_ORE_RINT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_ORE_RINT(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_ORE_RINT(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_ore_rint_ena_w1c
 *
 * SDP Output Error Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_epfx_ore_rint_ena_w1c
{
    uint64_t u;
    struct cavm_sdpx_epfx_ore_rint_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_ORE_RINT[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_ORE_RINT[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_ore_rint_ena_w1c_s cn; */
};
typedef union cavm_sdpx_epfx_ore_rint_ena_w1c cavm_sdpx_epfx_ore_rint_ena_w1c_t;

static inline uint64_t CAVM_SDPX_EPFX_ORE_RINT_ENA_W1C(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_ORE_RINT_ENA_W1C(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020340ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_ORE_RINT_ENA_W1C", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_ORE_RINT_ENA_W1C(a,b) cavm_sdpx_epfx_ore_rint_ena_w1c_t
#define bustype_CAVM_SDPX_EPFX_ORE_RINT_ENA_W1C(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_ORE_RINT_ENA_W1C(a,b) "SDPX_EPFX_ORE_RINT_ENA_W1C"
#define device_bar_CAVM_SDPX_EPFX_ORE_RINT_ENA_W1C(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_ORE_RINT_ENA_W1C(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_ORE_RINT_ENA_W1C(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_ore_rint_ena_w1s
 *
 * SDP Output Error Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_epfx_ore_rint_ena_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_ore_rint_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_ORE_RINT[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_ORE_RINT[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_ore_rint_ena_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_ore_rint_ena_w1s cavm_sdpx_epfx_ore_rint_ena_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_ORE_RINT_ENA_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_ORE_RINT_ENA_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020350ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_ORE_RINT_ENA_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_ORE_RINT_ENA_W1S(a,b) cavm_sdpx_epfx_ore_rint_ena_w1s_t
#define bustype_CAVM_SDPX_EPFX_ORE_RINT_ENA_W1S(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_ORE_RINT_ENA_W1S(a,b) "SDPX_EPFX_ORE_RINT_ENA_W1S"
#define device_bar_CAVM_SDPX_EPFX_ORE_RINT_ENA_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_ORE_RINT_ENA_W1S(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_ORE_RINT_ENA_W1S(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_ore_rint_w1s
 *
 * SDP Output Error Status Set Register
 * This register sets interrupt bits.
 */
union cavm_sdpx_epfx_ore_rint_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_ore_rint_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_ORE_RINT[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_ORE_RINT[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_ore_rint_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_ore_rint_w1s cavm_sdpx_epfx_ore_rint_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_ORE_RINT_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_ORE_RINT_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e080020330ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_ORE_RINT_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_ORE_RINT_W1S(a,b) cavm_sdpx_epfx_ore_rint_w1s_t
#define bustype_CAVM_SDPX_EPFX_ORE_RINT_W1S(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_ORE_RINT_W1S(a,b) "SDPX_EPFX_ORE_RINT_W1S"
#define device_bar_CAVM_SDPX_EPFX_ORE_RINT_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_ORE_RINT_W1S(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_ORE_RINT_W1S(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_pp_vf_rint#
 *
 * SDP PP Error Response VF Bit Array Registers
 * When an error response is received for a VF PP transaction read, the appropriate VF indexed
 * bit is set.  The appropriate PF should read the appropriate register.
 * Each EPF supports 128 VFs.
 * Register index 0 refers to the VF functions 1..64 of the EPF.
 * Register index 1 refers to the VF functions 65..128 of the EPF.
 * The given register associated with an EPF will be reset due to a PF FLR or MAC reset.
 * These registers are not affected by VF FLR.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_pp_vf_rintx
{
    uint64_t u;
    struct cavm_sdpx_epfx_pp_vf_rintx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) When an error response is received for a VF PP transaction read, the appropriate VF
                                                                 indexed bit is set. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) When an error response is received for a VF PP transaction read, the appropriate VF
                                                                 indexed bit is set. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_pp_vf_rintx_s cn; */
};
typedef union cavm_sdpx_epfx_pp_vf_rintx cavm_sdpx_epfx_pp_vf_rintx_t;

static inline uint64_t CAVM_SDPX_EPFX_PP_VF_RINTX(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_PP_VF_RINTX(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020560ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_PP_VF_RINTX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_PP_VF_RINTX(a,b,c) cavm_sdpx_epfx_pp_vf_rintx_t
#define bustype_CAVM_SDPX_EPFX_PP_VF_RINTX(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_PP_VF_RINTX(a,b,c) "SDPX_EPFX_PP_VF_RINTX"
#define device_bar_CAVM_SDPX_EPFX_PP_VF_RINTX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_PP_VF_RINTX(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_PP_VF_RINTX(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_pp_vf_rint#_ena_w1c
 *
 * SDP PP Error Response VF Bit Array Local Enable Clear Registers
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_epfx_pp_vf_rintx_ena_w1c
{
    uint64_t u;
    struct cavm_sdpx_epfx_pp_vf_rintx_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_PP_VF_RINT(0..1)[VF_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_PP_VF_RINT(0..1)[VF_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_pp_vf_rintx_ena_w1c_s cn; */
};
typedef union cavm_sdpx_epfx_pp_vf_rintx_ena_w1c cavm_sdpx_epfx_pp_vf_rintx_ena_w1c_t;

static inline uint64_t CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e0800205a0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_PP_VF_RINTX_ENA_W1C", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1C(a,b,c) cavm_sdpx_epfx_pp_vf_rintx_ena_w1c_t
#define bustype_CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1C(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1C(a,b,c) "SDPX_EPFX_PP_VF_RINTX_ENA_W1C"
#define device_bar_CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1C(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1C(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1C(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_pp_vf_rint#_ena_w1s
 *
 * SDP PP Error Response VF Bit Array Local Enable Set Registers
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_epfx_pp_vf_rintx_ena_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_pp_vf_rintx_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_PP_VF_RINT(0..1)[VF_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_PP_VF_RINT(0..1)[VF_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_pp_vf_rintx_ena_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_pp_vf_rintx_ena_w1s cavm_sdpx_epfx_pp_vf_rintx_ena_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e0800205c0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_PP_VF_RINTX_ENA_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1S(a,b,c) cavm_sdpx_epfx_pp_vf_rintx_ena_w1s_t
#define bustype_CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1S(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1S(a,b,c) "SDPX_EPFX_PP_VF_RINTX_ENA_W1S"
#define device_bar_CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1S(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_PP_VF_RINTX_ENA_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_pp_vf_rint#_w1s
 *
 * SDP PP Error Response VF Bit Array Set Registers
 * This register sets interrupt bits.
 */
union cavm_sdpx_epfx_pp_vf_rintx_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_pp_vf_rintx_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_PP_VF_RINT(0..1)[VF_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t vf_int                : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_PP_VF_RINT(0..1)[VF_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_pp_vf_rintx_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_pp_vf_rintx_w1s cavm_sdpx_epfx_pp_vf_rintx_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_PP_VF_RINTX_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_PP_VF_RINTX_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020580ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_PP_VF_RINTX_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_PP_VF_RINTX_W1S(a,b,c) cavm_sdpx_epfx_pp_vf_rintx_w1s_t
#define bustype_CAVM_SDPX_EPFX_PP_VF_RINTX_W1S(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_PP_VF_RINTX_W1S(a,b,c) "SDPX_EPFX_PP_VF_RINTX_W1S"
#define device_bar_CAVM_SDPX_EPFX_PP_VF_RINTX_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_PP_VF_RINTX_W1S(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_PP_VF_RINTX_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_rinfo
 *
 * SDP Ring Information Register
 * This register sets the total number of vfs and vf rings for a given MAC and PF
 * combination (EPF).  An EPF can only access VF rings through VF bar space,
 * it cannot access a VF's rings through its own PF bar space,
 *
 * This register does not specify rings a EPF owns, see SDP()_MAC()_PF_RING_CTL.
 *
 * An individual VF will own SDP()_EPF()_RINFO[RPVF] number of rings.
 *
 * SDP()_EPVF_RING()[EPVF] must be written to values that
 * correlate with the fields in this register.
 *
 * The given register associated with an EPF will be reset due to a PF FLR or MAC Reset.
 * These registers are not affected by VF FLR.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_rinfo
{
    uint64_t u;
    struct cavm_sdpx_epfx_rinfo_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_56_63        : 8;
        uint64_t nvfs                  : 8;  /**< [ 55: 48](R/W) The number of VFs for this PF. This field must not be zero whenever RPVF != 0.
                                                                 Legal values are 0 to 64, with the requirement of (NVFS * RPVF) \<= TRS. */
        uint64_t reserved_36_47        : 12;
        uint64_t rpvf                  : 4;  /**< [ 35: 32](R/W) The number of rings assigned to a VF for this PF. Legal values are 0 to 8
                                                                 with the requirement of (NVFS * RPVF) \<= TRS. */
        uint64_t reserved_24_31        : 8;
        uint64_t trs                   : 8;  /**< [ 23: 16](RO) The number of rings assigned to the EPF. This is the same as
                                                                 the SLI()_LMAC_CONST0()[RINGS] field for the MAC/PF
                                                                 corresponding to this EPF.

                                                                 Internal:
                                                                 This is always 64 for CN83XX. */
        uint64_t reserved_8_15         : 8;
        uint64_t srn                   : 8;  /**< [  7:  0](RO) The starting ring number used by the EPF.
                                                                 Internal:
                                                                 This is 0x0 for EPF0, and 64 for EPF1. */
#else /* Word 0 - Little Endian */
        uint64_t srn                   : 8;  /**< [  7:  0](RO) The starting ring number used by the EPF.
                                                                 Internal:
                                                                 This is 0x0 for EPF0, and 64 for EPF1. */
        uint64_t reserved_8_15         : 8;
        uint64_t trs                   : 8;  /**< [ 23: 16](RO) The number of rings assigned to the EPF. This is the same as
                                                                 the SLI()_LMAC_CONST0()[RINGS] field for the MAC/PF
                                                                 corresponding to this EPF.

                                                                 Internal:
                                                                 This is always 64 for CN83XX. */
        uint64_t reserved_24_31        : 8;
        uint64_t rpvf                  : 4;  /**< [ 35: 32](R/W) The number of rings assigned to a VF for this PF. Legal values are 0 to 8
                                                                 with the requirement of (NVFS * RPVF) \<= TRS. */
        uint64_t reserved_36_47        : 12;
        uint64_t nvfs                  : 8;  /**< [ 55: 48](R/W) The number of VFs for this PF. This field must not be zero whenever RPVF != 0.
                                                                 Legal values are 0 to 64, with the requirement of (NVFS * RPVF) \<= TRS. */
        uint64_t reserved_56_63        : 8;
#endif /* Word 0 - End */
    } s;
    struct cavm_sdpx_epfx_rinfo_cn8
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_55_63        : 9;
        uint64_t nvfs                  : 7;  /**< [ 54: 48](R/W) The number of VFs for this PF. This field must not be zero whenever RPVF != 0.
                                                                 Legal values are 0 to 64, with the requirement of (NVFS * RPVF) \<= TRS. */
        uint64_t reserved_36_47        : 12;
        uint64_t rpvf                  : 4;  /**< [ 35: 32](R/W) The number of rings assigned to a VF for this PF. Legal values are 0 to 8
                                                                 with the requirement of (NVFS * RPVF) \<= TRS. */
        uint64_t reserved_24_31        : 8;
        uint64_t trs                   : 8;  /**< [ 23: 16](RO) The number of rings assigned to the EPF. This is the same as
                                                                 the SLI()_LMAC_CONST0()[RINGS] field for the MAC/PF
                                                                 corresponding to this EPF.

                                                                 Internal:
                                                                 This is always 64 for CN83XX. */
        uint64_t reserved_7_15         : 9;
        uint64_t srn                   : 7;  /**< [  6:  0](RO) The starting ring number used by the EPF.
                                                                 Internal:
                                                                 This is 0x0 for EPF0, and 64 for EPF1. */
#else /* Word 0 - Little Endian */
        uint64_t srn                   : 7;  /**< [  6:  0](RO) The starting ring number used by the EPF.
                                                                 Internal:
                                                                 This is 0x0 for EPF0, and 64 for EPF1. */
        uint64_t reserved_7_15         : 9;
        uint64_t trs                   : 8;  /**< [ 23: 16](RO) The number of rings assigned to the EPF. This is the same as
                                                                 the SLI()_LMAC_CONST0()[RINGS] field for the MAC/PF
                                                                 corresponding to this EPF.

                                                                 Internal:
                                                                 This is always 64 for CN83XX. */
        uint64_t reserved_24_31        : 8;
        uint64_t rpvf                  : 4;  /**< [ 35: 32](R/W) The number of rings assigned to a VF for this PF. Legal values are 0 to 8
                                                                 with the requirement of (NVFS * RPVF) \<= TRS. */
        uint64_t reserved_36_47        : 12;
        uint64_t nvfs                  : 7;  /**< [ 54: 48](R/W) The number of VFs for this PF. This field must not be zero whenever RPVF != 0.
                                                                 Legal values are 0 to 64, with the requirement of (NVFS * RPVF) \<= TRS. */
        uint64_t reserved_55_63        : 9;
#endif /* Word 0 - End */
    } cn8;
    struct cavm_sdpx_epfx_rinfo_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_56_63        : 8;
        uint64_t nvfs                  : 8;  /**< [ 55: 48](R/W) The number of VFs for this PF. This field must not be zero whenever [RPVF] != 0.
                                                                 Legal values are 0 to 128, with the requirement of (NVFS * RPVF) \<= TRS. */
        uint64_t reserved_36_47        : 12;
        uint64_t rpvf                  : 4;  /**< [ 35: 32](R/W) The number of rings assigned to a VF for this PF. Legal values are 0 to 8
                                                                 with the requirement of (NVFS * RPVF) \<= TRS. */
        uint64_t reserved_24_31        : 8;
        uint64_t reserved_16_23        : 8;
        uint64_t reserved_8_15         : 8;
        uint64_t srn                   : 8;  /**< [  7:  0](R/W) The starting ring number used by the EPF. */
#else /* Word 0 - Little Endian */
        uint64_t srn                   : 8;  /**< [  7:  0](R/W) The starting ring number used by the EPF. */
        uint64_t reserved_8_15         : 8;
        uint64_t reserved_16_23        : 8;
        uint64_t reserved_24_31        : 8;
        uint64_t rpvf                  : 4;  /**< [ 35: 32](R/W) The number of rings assigned to a VF for this PF. Legal values are 0 to 8
                                                                 with the requirement of (NVFS * RPVF) \<= TRS. */
        uint64_t reserved_36_47        : 12;
        uint64_t nvfs                  : 8;  /**< [ 55: 48](R/W) The number of VFs for this PF. This field must not be zero whenever [RPVF] != 0.
                                                                 Legal values are 0 to 128, with the requirement of (NVFS * RPVF) \<= TRS. */
        uint64_t reserved_56_63        : 8;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_sdpx_epfx_rinfo cavm_sdpx_epfx_rinfo_t;

static inline uint64_t CAVM_SDPX_EPFX_RINFO(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_RINFO(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=1)))
        return 0x874080020190ll + 0x1000000000ll * ((a) & 0x0) + 0x800000ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e0800205f0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_RINFO", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_RINFO(a,b) cavm_sdpx_epfx_rinfo_t
#define bustype_CAVM_SDPX_EPFX_RINFO(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_RINFO(a,b) "SDPX_EPFX_RINFO"
#define device_bar_CAVM_SDPX_EPFX_RINFO(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_RINFO(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_RINFO(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_rmt_msix_pba#
 *
 * SDP Remote MSI-X Pending Bit Array Registers
 * This register is the remote MSI-X PBA table.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_rmt_msix_pbax
{
    uint64_t u;
    struct cavm_sdpx_epfx_rmt_msix_pbax_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO/H) Pending message for the associated SLI_MSIX_VEC()_CTL[DATA].
                                                                 This is reset by PF-FLR and VF-FLR. */
#else /* Word 0 - Little Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO/H) Pending message for the associated SLI_MSIX_VEC()_CTL[DATA].
                                                                 This is reset by PF-FLR and VF-FLR. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_rmt_msix_pbax_s cn8; */
    struct cavm_sdpx_epfx_rmt_msix_pbax_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO/H) Pending message for the associated SDP()_EPF()_RMT_MSIX_VEC()_CTL.
                                                                 This is reset by PF-FLR and VF-FLR. */
#else /* Word 0 - Little Endian */
        uint64_t pend                  : 64; /**< [ 63:  0](RO/H) Pending message for the associated SDP()_EPF()_RMT_MSIX_VEC()_CTL.
                                                                 This is reset by PF-FLR and VF-FLR. */
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_sdpx_epfx_rmt_msix_pbax cavm_sdpx_epfx_rmt_msix_pbax_t;

static inline uint64_t CAVM_SDPX_EPFX_RMT_MSIX_PBAX(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_RMT_MSIX_PBAX(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=3) && (c<=1)))
        return 0x874080004000ll + 0x1000000000ll * ((a) & 0x0) + 0x800000ll * ((b) & 0x3) + 8ll * ((c) & 0x1);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080004000ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 8ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_RMT_MSIX_PBAX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_RMT_MSIX_PBAX(a,b,c) cavm_sdpx_epfx_rmt_msix_pbax_t
#define bustype_CAVM_SDPX_EPFX_RMT_MSIX_PBAX(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_RMT_MSIX_PBAX(a,b,c) "SDPX_EPFX_RMT_MSIX_PBAX"
#define device_bar_CAVM_SDPX_EPFX_RMT_MSIX_PBAX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_RMT_MSIX_PBAX(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_RMT_MSIX_PBAX(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_rmt_msix_vec#_addr
 *
 * SDP Remote MSI-X Vector-Table Address Register
 * This register is the MSI-X vector table for remote hosts (when CNXXXX is an
 * endpoint), and is indexed by the SDP_RMT_INT_VEC_E enumeration.
 *
 * This register is not affected by PF-FLR, VF-FLR, VF enable falling edge, or
 * MAC reset. It must be initialized before use.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_rmt_msix_vecx_addr
{
    uint64_t u;
    struct cavm_sdpx_epfx_rmt_msix_vecx_addr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t addr                  : 62; /**< [ 63:  2](R/W) PCI address to use for MSI-X delivery of this vector. */
        uint64_t reserved_0_1          : 2;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_1          : 2;
        uint64_t addr                  : 62; /**< [ 63:  2](R/W) PCI address to use for MSI-X delivery of this vector. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_rmt_msix_vecx_addr_s cn; */
};
typedef union cavm_sdpx_epfx_rmt_msix_vecx_addr cavm_sdpx_epfx_rmt_msix_vecx_addr_t;

static inline uint64_t CAVM_SDPX_EPFX_RMT_MSIX_VECX_ADDR(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_RMT_MSIX_VECX_ADDR(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=3) && (c<=79)))
        return 0x874080000000ll + 0x1000000000ll * ((a) & 0x0) + 0x800000ll * ((b) & 0x3) + 0x10ll * ((c) & 0x7f);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=79)))
        return 0x86e080000000ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x7f);
    __cavm_csr_fatal("SDPX_EPFX_RMT_MSIX_VECX_ADDR", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_RMT_MSIX_VECX_ADDR(a,b,c) cavm_sdpx_epfx_rmt_msix_vecx_addr_t
#define bustype_CAVM_SDPX_EPFX_RMT_MSIX_VECX_ADDR(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_RMT_MSIX_VECX_ADDR(a,b,c) "SDPX_EPFX_RMT_MSIX_VECX_ADDR"
#define device_bar_CAVM_SDPX_EPFX_RMT_MSIX_VECX_ADDR(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_RMT_MSIX_VECX_ADDR(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_RMT_MSIX_VECX_ADDR(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_rmt_msix_vec#_ctl
 *
 * SDP Remote MSI-X Table Entry Data Register
 * This register is the MSI-X vector table for remote hosts (when CNXXXX is an
 * endpoint), and is indexed by the SDP_RMT_INT_VEC_E enumeration.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_rmt_msix_vecx_ctl
{
    uint64_t u;
    struct cavm_sdpx_epfx_rmt_msix_vecx_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_33_63        : 31;
        uint64_t mask                  : 1;  /**< [ 32: 32](R/W/H) When set, no MSI-X interrupts are sent to this vector.
                                                                 This field is reset by hardware in the event of a PF-FLR,
                                                                 VF-FLR, VF_EN falling edge, or MAC reset. */
        uint64_t data                  : 32; /**< [ 31:  0](R/W) Data to use for MSI-X delivery of this vector.
                                                                 This field is not affected by PF-FLR, VF-FLR, VF_EN falling
                                                                 edge or MAC reset. It must be initialized before use. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 32; /**< [ 31:  0](R/W) Data to use for MSI-X delivery of this vector.
                                                                 This field is not affected by PF-FLR, VF-FLR, VF_EN falling
                                                                 edge or MAC reset. It must be initialized before use. */
        uint64_t mask                  : 1;  /**< [ 32: 32](R/W/H) When set, no MSI-X interrupts are sent to this vector.
                                                                 This field is reset by hardware in the event of a PF-FLR,
                                                                 VF-FLR, VF_EN falling edge, or MAC reset. */
        uint64_t reserved_33_63        : 31;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_rmt_msix_vecx_ctl_s cn8; */
    struct cavm_sdpx_epfx_rmt_msix_vecx_ctl_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_33_63        : 31;
        uint64_t mask                  : 1;  /**< [ 32: 32](R/W/H) When set, no MSI-X interrupts are sent to this vector.
                                                                 This field is reset by hardware in the event of a PF-FLR,
                                                                 VF-FLR, VF enable falling edge, or MAC reset. */
        uint64_t data                  : 32; /**< [ 31:  0](R/W) Data to use for MSI-X delivery of this vector.
                                                                 This field is not affected by PF-FLR, VF-FLR, VF enable falling
                                                                 edge or MAC reset. It must be initialized before use. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 32; /**< [ 31:  0](R/W) Data to use for MSI-X delivery of this vector.
                                                                 This field is not affected by PF-FLR, VF-FLR, VF enable falling
                                                                 edge or MAC reset. It must be initialized before use. */
        uint64_t mask                  : 1;  /**< [ 32: 32](R/W/H) When set, no MSI-X interrupts are sent to this vector.
                                                                 This field is reset by hardware in the event of a PF-FLR,
                                                                 VF-FLR, VF enable falling edge, or MAC reset. */
        uint64_t reserved_33_63        : 31;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_sdpx_epfx_rmt_msix_vecx_ctl cavm_sdpx_epfx_rmt_msix_vecx_ctl_t;

static inline uint64_t CAVM_SDPX_EPFX_RMT_MSIX_VECX_CTL(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_RMT_MSIX_VECX_CTL(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=3) && (c<=79)))
        return 0x874080000008ll + 0x1000000000ll * ((a) & 0x0) + 0x800000ll * ((b) & 0x3) + 0x10ll * ((c) & 0x7f);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=79)))
        return 0x86e080000008ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x7f);
    __cavm_csr_fatal("SDPX_EPFX_RMT_MSIX_VECX_CTL", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_RMT_MSIX_VECX_CTL(a,b,c) cavm_sdpx_epfx_rmt_msix_vecx_ctl_t
#define bustype_CAVM_SDPX_EPFX_RMT_MSIX_VECX_CTL(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_RMT_MSIX_VECX_CTL(a,b,c) "SDPX_EPFX_RMT_MSIX_VECX_CTL"
#define device_bar_CAVM_SDPX_EPFX_RMT_MSIX_VECX_CTL(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_RMT_MSIX_VECX_CTL(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_RMT_MSIX_VECX_CTL(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_scratch
 *
 * SDP Scratch Register
 * These registers are general purpose 64-bit scratch registers for software use.
 */
union cavm_sdpx_epfx_scratch
{
    uint64_t u;
    struct cavm_sdpx_epfx_scratch_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W) The value in this register is totally software defined. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W) The value in this register is totally software defined. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_scratch_s cn; */
};
typedef union cavm_sdpx_epfx_scratch cavm_sdpx_epfx_scratch_t;

static inline uint64_t CAVM_SDPX_EPFX_SCRATCH(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_SCRATCH(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15)))
        return 0x86e0800205e0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf);
    __cavm_csr_fatal("SDPX_EPFX_SCRATCH", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_SCRATCH(a,b) cavm_sdpx_epfx_scratch_t
#define bustype_CAVM_SDPX_EPFX_SCRATCH(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_SCRATCH(a,b) "SDPX_EPFX_SCRATCH"
#define device_bar_CAVM_SDPX_EPFX_SCRATCH(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_SCRATCH(a,b) (a)
#define arguments_CAVM_SDPX_EPFX_SCRATCH(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_epf#_vfire_rint#
 *
 * SDP Input Error Status Register
 * This register indicates if an error has been detected on an input VF ring.
 * Register index 0 refers to the frist 64 rings starting from SDP()_EPF()_RINFO[SRN].
 * Register index 1 refers to the second 64 rings starting from
 * SDP()_EPF()_RINFO[SRN] + 64.
 * The given register associated with an EPF will be reset due to a PF FLR or MAC Reset.
 * These registers are not affected by VF FLR.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_vfire_rintx
{
    uint64_t u;
    struct cavm_sdpx_epfx_vfire_rintx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Error has been detected on input ring i. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Error has been detected on input ring i. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_vfire_rintx_s cn; */
};
typedef union cavm_sdpx_epfx_vfire_rintx cavm_sdpx_epfx_vfire_rintx_t;

static inline uint64_t CAVM_SDPX_EPFX_VFIRE_RINTX(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_VFIRE_RINTX(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020180ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_VFIRE_RINTX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_VFIRE_RINTX(a,b,c) cavm_sdpx_epfx_vfire_rintx_t
#define bustype_CAVM_SDPX_EPFX_VFIRE_RINTX(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_VFIRE_RINTX(a,b,c) "SDPX_EPFX_VFIRE_RINTX"
#define device_bar_CAVM_SDPX_EPFX_VFIRE_RINTX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_VFIRE_RINTX(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_VFIRE_RINTX(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_vfire_rint#_ena_w1c
 *
 * SDP Input Error Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_epfx_vfire_rintx_ena_w1c
{
    uint64_t u;
    struct cavm_sdpx_epfx_vfire_rintx_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_VFIRE_RINT(0..1)[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_VFIRE_RINT(0..1)[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_vfire_rintx_ena_w1c_s cn; */
};
typedef union cavm_sdpx_epfx_vfire_rintx_ena_w1c cavm_sdpx_epfx_vfire_rintx_ena_w1c_t;

static inline uint64_t CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e0800201c0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_VFIRE_RINTX_ENA_W1C", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1C(a,b,c) cavm_sdpx_epfx_vfire_rintx_ena_w1c_t
#define bustype_CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1C(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1C(a,b,c) "SDPX_EPFX_VFIRE_RINTX_ENA_W1C"
#define device_bar_CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1C(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1C(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1C(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_vfire_rint#_ena_w1s
 *
 * SDP Input Error Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_epfx_vfire_rintx_ena_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_vfire_rintx_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_VFIRE_RINT(0..1)[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_VFIRE_RINT(0..1)[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_vfire_rintx_ena_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_vfire_rintx_ena_w1s cavm_sdpx_epfx_vfire_rintx_ena_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e0800201e0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_VFIRE_RINTX_ENA_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1S(a,b,c) cavm_sdpx_epfx_vfire_rintx_ena_w1s_t
#define bustype_CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1S(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1S(a,b,c) "SDPX_EPFX_VFIRE_RINTX_ENA_W1S"
#define device_bar_CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1S(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_VFIRE_RINTX_ENA_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_vfire_rint#_w1s
 *
 * SDP Input Error Status Set Register
 * This register sets interrupt bits.
 */
union cavm_sdpx_epfx_vfire_rintx_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_vfire_rintx_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_VFIRE_RINT(0..1)[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_VFIRE_RINT(0..1)[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_vfire_rintx_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_vfire_rintx_w1s cavm_sdpx_epfx_vfire_rintx_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_VFIRE_RINTX_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_VFIRE_RINTX_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e0800201a0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_VFIRE_RINTX_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_VFIRE_RINTX_W1S(a,b,c) cavm_sdpx_epfx_vfire_rintx_w1s_t
#define bustype_CAVM_SDPX_EPFX_VFIRE_RINTX_W1S(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_VFIRE_RINTX_W1S(a,b,c) "SDPX_EPFX_VFIRE_RINTX_W1S"
#define device_bar_CAVM_SDPX_EPFX_VFIRE_RINTX_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_VFIRE_RINTX_W1S(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_VFIRE_RINTX_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_vfore_rint#
 *
 * SDP Output Error Status Register
 * This register indicates if an error has been detected on an output VF ring.
 * Register index 0 refers to the frist 64 rings starting from SDP()_EPF()_RINFO[SRN].
 * Register index 1 refers to the second 64 rings starting from
 * SDP()_EPF()_RINFO[SRN] + 64.
 * The given register associated with an EPF will be reset due to a PF FLR or MAC Reset.
 * These registers are not affected by VF FLR.
 *
 * Note: EPF(0..7) are mapped to MAC0, EPF(8..15) are mapped to MAC2.
 */
union cavm_sdpx_epfx_vfore_rintx
{
    uint64_t u;
    struct cavm_sdpx_epfx_vfore_rintx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Error has been detected on ring output VF ring i. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Error has been detected on ring output VF ring i. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_vfore_rintx_s cn; */
};
typedef union cavm_sdpx_epfx_vfore_rintx cavm_sdpx_epfx_vfore_rintx_t;

static inline uint64_t CAVM_SDPX_EPFX_VFORE_RINTX(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_VFORE_RINTX(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020240ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_VFORE_RINTX", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_VFORE_RINTX(a,b,c) cavm_sdpx_epfx_vfore_rintx_t
#define bustype_CAVM_SDPX_EPFX_VFORE_RINTX(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_VFORE_RINTX(a,b,c) "SDPX_EPFX_VFORE_RINTX"
#define device_bar_CAVM_SDPX_EPFX_VFORE_RINTX(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_VFORE_RINTX(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_VFORE_RINTX(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_vfore_rint#_ena_w1c
 *
 * SDP Output Error Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_epfx_vfore_rintx_ena_w1c
{
    uint64_t u;
    struct cavm_sdpx_epfx_vfore_rintx_ena_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_VFORE_RINT(0..1)[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_EPF(0..15)_VFORE_RINT(0..1)[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_vfore_rintx_ena_w1c_s cn; */
};
typedef union cavm_sdpx_epfx_vfore_rintx_ena_w1c cavm_sdpx_epfx_vfore_rintx_ena_w1c_t;

static inline uint64_t CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1C(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020280ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_VFORE_RINTX_ENA_W1C", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1C(a,b,c) cavm_sdpx_epfx_vfore_rintx_ena_w1c_t
#define bustype_CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1C(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1C(a,b,c) "SDPX_EPFX_VFORE_RINTX_ENA_W1C"
#define device_bar_CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1C(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1C(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1C(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_vfore_rint#_ena_w1s
 *
 * SDP Output Error Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_epfx_vfore_rintx_ena_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_vfore_rintx_ena_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_VFORE_RINT(0..1)[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_EPF(0..15)_VFORE_RINT(0..1)[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_vfore_rintx_ena_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_vfore_rintx_ena_w1s cavm_sdpx_epfx_vfore_rintx_ena_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e0800202a0ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_VFORE_RINTX_ENA_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1S(a,b,c) cavm_sdpx_epfx_vfore_rintx_ena_w1s_t
#define bustype_CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1S(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1S(a,b,c) "SDPX_EPFX_VFORE_RINTX_ENA_W1S"
#define device_bar_CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1S(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_VFORE_RINTX_ENA_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epf#_vfore_rint#_w1s
 *
 * SDP Output Error Status Set Register
 * This register sets interrupt bits.
 */
union cavm_sdpx_epfx_vfore_rintx_w1s
{
    uint64_t u;
    struct cavm_sdpx_epfx_vfore_rintx_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_VFORE_RINT(0..1)[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_EPF(0..15)_VFORE_RINT(0..1)[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epfx_vfore_rintx_w1s_s cn; */
};
typedef union cavm_sdpx_epfx_vfore_rintx_w1s cavm_sdpx_epfx_vfore_rintx_w1s_t;

static inline uint64_t CAVM_SDPX_EPFX_VFORE_RINTX_W1S(unsigned long a, unsigned long b, unsigned long c) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPFX_VFORE_RINTX_W1S(unsigned long a, unsigned long b, unsigned long c)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=15) && (c<=1)))
        return 0x86e080020260ll + 0x1000000000ll * ((a) & 0x0) + 0x2000000ll * ((b) & 0xf) + 0x10ll * ((c) & 0x1);
    __cavm_csr_fatal("SDPX_EPFX_VFORE_RINTX_W1S", 3, a, b, c, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPFX_VFORE_RINTX_W1S(a,b,c) cavm_sdpx_epfx_vfore_rintx_w1s_t
#define bustype_CAVM_SDPX_EPFX_VFORE_RINTX_W1S(a,b,c) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPFX_VFORE_RINTX_W1S(a,b,c) "SDPX_EPFX_VFORE_RINTX_W1S"
#define device_bar_CAVM_SDPX_EPFX_VFORE_RINTX_W1S(a,b,c) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPFX_VFORE_RINTX_W1S(a,b,c) (a)
#define arguments_CAVM_SDPX_EPFX_VFORE_RINTX_W1S(a,b,c) (a),(b),(c),-1

/**
 * Register (PEXP_NCB) sdp#_epvf_ring#
 *
 * SDP EPVF Ring Register
 * These registers provide the virtual function number for each ring (both input and
 * output). They must be programmed by the PF along with SDP()_EPF()_RINFO before
 * the given ring is enabled. They are not accessible by the VF.
 *
 * All 64 registers associated with an EPF will be reset due to a PF FLR or MAC Reset.
 * These registers are not affected by VF FLR.
 *
 * This register is indexed by ring and would normally be accessed from an AP core where all
 * 256 rings can be indexed.
 * A PF would index this register based upon the settings of SDP()_EPF()_RINFO[SRN]
 * and SDP()_MAC()_PF_RING_CTL[SRN].
 */
union cavm_sdpx_epvf_ringx
{
    uint64_t u;
    struct cavm_sdpx_epvf_ringx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_12_63        : 52;
        uint64_t epvf                  : 12; /**< [ 11:  0](R/W) The function that a ring belongs to.
                                                                 SDP_FUNC_SEL_S format. */
#else /* Word 0 - Little Endian */
        uint64_t epvf                  : 12; /**< [ 11:  0](R/W) The function that a ring belongs to.
                                                                 SDP_FUNC_SEL_S format. */
        uint64_t reserved_12_63        : 52;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_epvf_ringx_s cn; */
};
typedef union cavm_sdpx_epvf_ringx cavm_sdpx_epvf_ringx_t;

static inline uint64_t CAVM_SDPX_EPVF_RINGX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_EPVF_RINGX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080026000ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_EPVF_RINGX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_EPVF_RINGX(a,b) cavm_sdpx_epvf_ringx_t
#define bustype_CAVM_SDPX_EPVF_RINGX(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_EPVF_RINGX(a,b) "SDPX_EPVF_RINGX"
#define device_bar_CAVM_SDPX_EPVF_RINGX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_EPVF_RINGX(a,b) (a)
#define arguments_CAVM_SDPX_EPVF_RINGX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_flr_ring_lint#
 *
 * SDP Function Level Reset VF Bit Array Registers
 * These registers are bit maps for the 256 rings in SDP and
 * indicate a ring that is owned by a function that went into
 * FLR.
 *
 * _ SDP()_FLR_RING_LINT(0) represents rings 63..0.
 * _ SDP()_FLR_RING_LINT(1) represents rings 127..63.
 * _ SDP()_FLR_RING_LINT(2) represents rings 191..128.
 * _ SDP()_FLR_RING_LINT(3) represents rings 256..192.
 *
 * These registers are only valid for PEM0 PF0 and PEM2 PF0.
 */
union cavm_sdpx_flr_ring_lintx
{
    uint64_t u;
    struct cavm_sdpx_flr_ring_lintx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_int              : 64; /**< [ 63:  0](R/W1C/H) When a VF or PF causes an FLR the appropriate bit for rings
                                                                 under that function will be set. */
#else /* Word 0 - Little Endian */
        uint64_t ring_int              : 64; /**< [ 63:  0](R/W1C/H) When a VF or PF causes an FLR the appropriate bit for rings
                                                                 under that function will be set. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_flr_ring_lintx_s cn; */
};
typedef union cavm_sdpx_flr_ring_lintx cavm_sdpx_flr_ring_lintx_t;

static inline uint64_t CAVM_SDPX_FLR_RING_LINTX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_FLR_RING_LINTX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0081000ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_FLR_RING_LINTX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_FLR_RING_LINTX(a,b) cavm_sdpx_flr_ring_lintx_t
#define bustype_CAVM_SDPX_FLR_RING_LINTX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_FLR_RING_LINTX(a,b) "SDPX_FLR_RING_LINTX"
#define device_bar_CAVM_SDPX_FLR_RING_LINTX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_FLR_RING_LINTX(a,b) (a)
#define arguments_CAVM_SDPX_FLR_RING_LINTX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_flr_ring_lint_ena_w1c#
 *
 * SDP Function Level Reset VF Bit Array Local Enable Clear Registers
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_flr_ring_lint_ena_w1cx
{
    uint64_t u;
    struct cavm_sdpx_flr_ring_lint_ena_w1cx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_int              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_FLR_RING_LINT(0..3)[RING_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_int              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_FLR_RING_LINT(0..3)[RING_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_flr_ring_lint_ena_w1cx_s cn; */
};
typedef union cavm_sdpx_flr_ring_lint_ena_w1cx cavm_sdpx_flr_ring_lint_ena_w1cx_t;

static inline uint64_t CAVM_SDPX_FLR_RING_LINT_ENA_W1CX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_FLR_RING_LINT_ENA_W1CX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0081200ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_FLR_RING_LINT_ENA_W1CX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_FLR_RING_LINT_ENA_W1CX(a,b) cavm_sdpx_flr_ring_lint_ena_w1cx_t
#define bustype_CAVM_SDPX_FLR_RING_LINT_ENA_W1CX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_FLR_RING_LINT_ENA_W1CX(a,b) "SDPX_FLR_RING_LINT_ENA_W1CX"
#define device_bar_CAVM_SDPX_FLR_RING_LINT_ENA_W1CX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_FLR_RING_LINT_ENA_W1CX(a,b) (a)
#define arguments_CAVM_SDPX_FLR_RING_LINT_ENA_W1CX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_flr_ring_lint_ena_w1s#
 *
 * SDP Function Level Reset VF Bit Array Local Enable Set Registers
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_flr_ring_lint_ena_w1sx
{
    uint64_t u;
    struct cavm_sdpx_flr_ring_lint_ena_w1sx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_int              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_FLR_RING_LINT(0..3)[RING_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_int              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_FLR_RING_LINT(0..3)[RING_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_flr_ring_lint_ena_w1sx_s cn; */
};
typedef union cavm_sdpx_flr_ring_lint_ena_w1sx cavm_sdpx_flr_ring_lint_ena_w1sx_t;

static inline uint64_t CAVM_SDPX_FLR_RING_LINT_ENA_W1SX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_FLR_RING_LINT_ENA_W1SX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0081300ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_FLR_RING_LINT_ENA_W1SX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_FLR_RING_LINT_ENA_W1SX(a,b) cavm_sdpx_flr_ring_lint_ena_w1sx_t
#define bustype_CAVM_SDPX_FLR_RING_LINT_ENA_W1SX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_FLR_RING_LINT_ENA_W1SX(a,b) "SDPX_FLR_RING_LINT_ENA_W1SX"
#define device_bar_CAVM_SDPX_FLR_RING_LINT_ENA_W1SX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_FLR_RING_LINT_ENA_W1SX(a,b) (a)
#define arguments_CAVM_SDPX_FLR_RING_LINT_ENA_W1SX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_flr_ring_lint_w1s#
 *
 * SDP Function Level Reset VF Bit Array Set Registers
 * This register sets interrupt bits.
 */
union cavm_sdpx_flr_ring_lint_w1sx
{
    uint64_t u;
    struct cavm_sdpx_flr_ring_lint_w1sx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_int              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_FLR_RING_LINT(0..3)[RING_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_int              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_FLR_RING_LINT(0..3)[RING_INT]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_flr_ring_lint_w1sx_s cn; */
};
typedef union cavm_sdpx_flr_ring_lint_w1sx cavm_sdpx_flr_ring_lint_w1sx_t;

static inline uint64_t CAVM_SDPX_FLR_RING_LINT_W1SX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_FLR_RING_LINT_W1SX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0081100ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_FLR_RING_LINT_W1SX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_FLR_RING_LINT_W1SX(a,b) cavm_sdpx_flr_ring_lint_w1sx_t
#define bustype_CAVM_SDPX_FLR_RING_LINT_W1SX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_FLR_RING_LINT_W1SX(a,b) "SDPX_FLR_RING_LINT_W1SX"
#define device_bar_CAVM_SDPX_FLR_RING_LINT_W1SX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_FLR_RING_LINT_W1SX(a,b) (a)
#define arguments_CAVM_SDPX_FLR_RING_LINT_W1SX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_gbl_control
 *
 * SDP Global Control Register
 * Global control bits for SDP.
 */
union cavm_sdpx_gbl_control
{
    uint64_t u;
    struct cavm_sdpx_gbl_control_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_17_63        : 47;
        uint64_t force_clks_on         : 1;  /**< [ 16: 16](R/W) When set this will force conditional clocks in SDP unconditionally on. For diagnostic use only. */
        uint64_t reserved_14_15        : 2;
        uint64_t bpkind                : 6;  /**< [ 13:  8](R/W) PKIND sent to PKI when SDP_INSTR_HDR_S[PKIND] corresponding bit in
                                                                 SDP()_PKIND_VALID[ENB] is set to a 0. */
        uint64_t reserved_4_7          : 4;
        uint64_t pkpfval               : 1;  /**< [  3:  3](R/W) When zero, only VF's are subject to SDP()_PKIND_VALID constraints, and PF instructions
                                                                 can select any PKI PKIND. When one, both PF's and VF's are subject to SDP()_PKIND_VALID
                                                                 constraints. */
        uint64_t bpflr_d               : 1;  /**< [  2:  2](R/W) Disables clearing SDP_PKT_OUT_BP_EN bit on an FLR. */
        uint64_t reserved_0_1          : 2;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_1          : 2;
        uint64_t bpflr_d               : 1;  /**< [  2:  2](R/W) Disables clearing SDP_PKT_OUT_BP_EN bit on an FLR. */
        uint64_t pkpfval               : 1;  /**< [  3:  3](R/W) When zero, only VF's are subject to SDP()_PKIND_VALID constraints, and PF instructions
                                                                 can select any PKI PKIND. When one, both PF's and VF's are subject to SDP()_PKIND_VALID
                                                                 constraints. */
        uint64_t reserved_4_7          : 4;
        uint64_t bpkind                : 6;  /**< [ 13:  8](R/W) PKIND sent to PKI when SDP_INSTR_HDR_S[PKIND] corresponding bit in
                                                                 SDP()_PKIND_VALID[ENB] is set to a 0. */
        uint64_t reserved_14_15        : 2;
        uint64_t force_clks_on         : 1;  /**< [ 16: 16](R/W) When set this will force conditional clocks in SDP unconditionally on. For diagnostic use only. */
        uint64_t reserved_17_63        : 47;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_gbl_control_s cn8; */
    struct cavm_sdpx_gbl_control_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_17_63        : 47;
        uint64_t force_clks_on         : 1;  /**< [ 16: 16](R/W) When set this will force conditional clocks in SDP unconditionally on. For diagnostic use only. */
        uint64_t reserved_14_15        : 2;
        uint64_t bpkind                : 6;  /**< [ 13:  8](R/W) PKIND sent to PKI when SDP_INSTR_HDR_S[PKIND] corresponding bit in
                                                                 SDP()_PKIND_VALID[ENB] is set to a 0. */
        uint64_t reserved_4_7          : 4;
        uint64_t pkpfval               : 1;  /**< [  3:  3](R/W) When zero, only VF's are subject to SDP()_PKIND_VALID constraints, and PF instructions
                                                                 can select any PKI PKIND. When one, both PF's and VF's are subject to SDP()_PKIND_VALID
                                                                 constraints. */
        uint64_t bpflr_d               : 1;  /**< [  2:  2](R/W) Disables clearing SDP()_OUT_BP_EN()_W1C[ENB] bit on an FLR. */
        uint64_t reserved_0_1          : 2;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_1          : 2;
        uint64_t bpflr_d               : 1;  /**< [  2:  2](R/W) Disables clearing SDP()_OUT_BP_EN()_W1C[ENB] bit on an FLR. */
        uint64_t pkpfval               : 1;  /**< [  3:  3](R/W) When zero, only VF's are subject to SDP()_PKIND_VALID constraints, and PF instructions
                                                                 can select any PKI PKIND. When one, both PF's and VF's are subject to SDP()_PKIND_VALID
                                                                 constraints. */
        uint64_t reserved_4_7          : 4;
        uint64_t bpkind                : 6;  /**< [ 13:  8](R/W) PKIND sent to PKI when SDP_INSTR_HDR_S[PKIND] corresponding bit in
                                                                 SDP()_PKIND_VALID[ENB] is set to a 0. */
        uint64_t reserved_14_15        : 2;
        uint64_t force_clks_on         : 1;  /**< [ 16: 16](R/W) When set this will force conditional clocks in SDP unconditionally on. For diagnostic use only. */
        uint64_t reserved_17_63        : 47;
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_sdpx_gbl_control cavm_sdpx_gbl_control_t;

static inline uint64_t CAVM_SDPX_GBL_CONTROL(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_GBL_CONTROL(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x874000880200ll + 0x1000000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0c0080200ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_GBL_CONTROL", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_GBL_CONTROL(a) cavm_sdpx_gbl_control_t
#define bustype_CAVM_SDPX_GBL_CONTROL(a) CSR_TYPE_NCB
#define basename_CAVM_SDPX_GBL_CONTROL(a) "SDPX_GBL_CONTROL"
#define device_bar_CAVM_SDPX_GBL_CONTROL(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_GBL_CONTROL(a) (a)
#define arguments_CAVM_SDPX_GBL_CONTROL(a) (a),-1,-1,-1

/**
 * Register (PEXP_NCB) sdp#_in_rate_limit#
 *
 * SDP Input Instruction Rate Limit Register
 * This register is the control for rate limiting input instruction rings.
 * Each index represents a token bucket.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the PF or AP prior to enabling the ring.
 *
 * This register is indexed by ring and would normally be accessed from an AP core where all
 * 256 rings can be indexed.
 * A PF would index this register based upon the settings of SDP()_EPF()_RINFO[SRN]
 * and SDP()_MAC()_PF_RING_CTL[SRN].
 */
union cavm_sdpx_in_rate_limitx
{
    uint64_t u;
    struct cavm_sdpx_in_rate_limitx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_53_63        : 11;
        uint64_t wgt                   : 4;  /**< [ 52: 49](R/W) Weight to calculate average packet size. */
        uint64_t init_ap               : 16; /**< [ 48: 33](R/W) Initial average packet size of instruction rings mapped to this bucket. */
        uint64_t max_tokens            : 16; /**< [ 32: 17](R/W) Maximum tokens that can be added to token bucket.
                                                                 Must be as large as the largest packet size input ring can fetch */
        uint64_t ref_tokens            : 16; /**< [ 16:  1](R/W) Tokens to add per ring every 1024 cycle period. */
        uint64_t enable                : 1;  /**< [  0:  0](R/W) Enable token bucket. */
#else /* Word 0 - Little Endian */
        uint64_t enable                : 1;  /**< [  0:  0](R/W) Enable token bucket. */
        uint64_t ref_tokens            : 16; /**< [ 16:  1](R/W) Tokens to add per ring every 1024 cycle period. */
        uint64_t max_tokens            : 16; /**< [ 32: 17](R/W) Maximum tokens that can be added to token bucket.
                                                                 Must be as large as the largest packet size input ring can fetch */
        uint64_t init_ap               : 16; /**< [ 48: 33](R/W) Initial average packet size of instruction rings mapped to this bucket. */
        uint64_t wgt                   : 4;  /**< [ 52: 49](R/W) Weight to calculate average packet size. */
        uint64_t reserved_53_63        : 11;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_in_rate_limitx_s cn; */
};
typedef union cavm_sdpx_in_rate_limitx cavm_sdpx_in_rate_limitx_t;

static inline uint64_t CAVM_SDPX_IN_RATE_LIMITX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_IN_RATE_LIMITX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e08002a000ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_IN_RATE_LIMITX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_IN_RATE_LIMITX(a,b) cavm_sdpx_in_rate_limitx_t
#define bustype_CAVM_SDPX_IN_RATE_LIMITX(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_IN_RATE_LIMITX(a,b) "SDPX_IN_RATE_LIMITX"
#define device_bar_CAVM_SDPX_IN_RATE_LIMITX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_IN_RATE_LIMITX(a,b) (a)
#define arguments_CAVM_SDPX_IN_RATE_LIMITX(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_in_ring_tb_map#
 *
 * SDP Input Ring Token Bucket Map Register
 * This register maps rings to token buckets used in Packet Input Byte Rate Limiting.
 * Each index represents a ring.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the PF or AP prior to enabling the ring. Also, this register cannot be written
 * while either of the following conditions is true:
 *   * SDP()_R()_IN_CONTROL[IDLE] is clear.
 *   * Or, SDP()_R()_IN_ENABLE[ENB] is set.
 *
 * This register is indexed by ring and would normally be accessed from an AP core where all
 * 256 rings can be indexed.
 * A PF would index this register based upon the settings of SDP()_EPF()_RINFO[SRN]
 * and SDP()_MAC()_PF_RING_CTL[SRN].
 */
union cavm_sdpx_in_ring_tb_mapx
{
    uint64_t u;
    struct cavm_sdpx_in_ring_tb_mapx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_9_63         : 55;
        uint64_t enable                : 1;  /**< [  8:  8](R/W) Enable token bucket mapping for ring. */
        uint64_t tb                    : 8;  /**< [  7:  0](R/W) Maps ring to one of 256 token bucket programmed by SDP()_IN_RATE_LIMIT(). */
#else /* Word 0 - Little Endian */
        uint64_t tb                    : 8;  /**< [  7:  0](R/W) Maps ring to one of 256 token bucket programmed by SDP()_IN_RATE_LIMIT(). */
        uint64_t enable                : 1;  /**< [  8:  8](R/W) Enable token bucket mapping for ring. */
        uint64_t reserved_9_63         : 55;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_in_ring_tb_mapx_s cn; */
};
typedef union cavm_sdpx_in_ring_tb_mapx cavm_sdpx_in_ring_tb_mapx_t;

static inline uint64_t CAVM_SDPX_IN_RING_TB_MAPX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_IN_RING_TB_MAPX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080028000ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_IN_RING_TB_MAPX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_IN_RING_TB_MAPX(a,b) cavm_sdpx_in_ring_tb_mapx_t
#define bustype_CAVM_SDPX_IN_RING_TB_MAPX(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_IN_RING_TB_MAPX(a,b) "SDPX_IN_RING_TB_MAPX"
#define device_bar_CAVM_SDPX_IN_RING_TB_MAPX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_IN_RING_TB_MAPX(a,b) (a)
#define arguments_CAVM_SDPX_IN_RING_TB_MAPX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_ire_lint#
 *
 * SDP Input Error Status Register
 * This register indicates if an error has been detected on an input ring.
 * The given register associated with an EPF will be reset due to a PF FLR or MAC Reset.
 * SDP()_IRE_LINT(0) holds status of rings 0..63.
 * SDP()_IRE_LINT(1) holds status of rings 64..127.
 * SDP()_IRE_LINT(2) holds status of rings 128..191.
 * SDP()_IRE_LINT(3) holds status of rings 192..255.
 * These registers are not affected by VF FLR.
 */
union cavm_sdpx_ire_lintx
{
    uint64_t u;
    struct cavm_sdpx_ire_lintx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Error has been detected on input ring i. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Error has been detected on input ring i. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_ire_lintx_s cn; */
};
typedef union cavm_sdpx_ire_lintx cavm_sdpx_ire_lintx_t;

static inline uint64_t CAVM_SDPX_IRE_LINTX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_IRE_LINTX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080400ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_IRE_LINTX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_IRE_LINTX(a,b) cavm_sdpx_ire_lintx_t
#define bustype_CAVM_SDPX_IRE_LINTX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_IRE_LINTX(a,b) "SDPX_IRE_LINTX"
#define device_bar_CAVM_SDPX_IRE_LINTX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_IRE_LINTX(a,b) (a)
#define arguments_CAVM_SDPX_IRE_LINTX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_ire_lint_ena_w1c#
 *
 * SDP Input Error Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_ire_lint_ena_w1cx
{
    uint64_t u;
    struct cavm_sdpx_ire_lint_ena_w1cx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_IRE_LINT(0..3)[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_IRE_LINT(0..3)[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_ire_lint_ena_w1cx_s cn; */
};
typedef union cavm_sdpx_ire_lint_ena_w1cx cavm_sdpx_ire_lint_ena_w1cx_t;

static inline uint64_t CAVM_SDPX_IRE_LINT_ENA_W1CX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_IRE_LINT_ENA_W1CX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080600ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_IRE_LINT_ENA_W1CX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_IRE_LINT_ENA_W1CX(a,b) cavm_sdpx_ire_lint_ena_w1cx_t
#define bustype_CAVM_SDPX_IRE_LINT_ENA_W1CX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_IRE_LINT_ENA_W1CX(a,b) "SDPX_IRE_LINT_ENA_W1CX"
#define device_bar_CAVM_SDPX_IRE_LINT_ENA_W1CX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_IRE_LINT_ENA_W1CX(a,b) (a)
#define arguments_CAVM_SDPX_IRE_LINT_ENA_W1CX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_ire_lint_ena_w1s#
 *
 * SDP Input Error Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_ire_lint_ena_w1sx
{
    uint64_t u;
    struct cavm_sdpx_ire_lint_ena_w1sx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_IRE_LINT(0..3)[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_IRE_LINT(0..3)[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_ire_lint_ena_w1sx_s cn; */
};
typedef union cavm_sdpx_ire_lint_ena_w1sx cavm_sdpx_ire_lint_ena_w1sx_t;

static inline uint64_t CAVM_SDPX_IRE_LINT_ENA_W1SX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_IRE_LINT_ENA_W1SX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080700ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_IRE_LINT_ENA_W1SX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_IRE_LINT_ENA_W1SX(a,b) cavm_sdpx_ire_lint_ena_w1sx_t
#define bustype_CAVM_SDPX_IRE_LINT_ENA_W1SX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_IRE_LINT_ENA_W1SX(a,b) "SDPX_IRE_LINT_ENA_W1SX"
#define device_bar_CAVM_SDPX_IRE_LINT_ENA_W1SX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_IRE_LINT_ENA_W1SX(a,b) (a)
#define arguments_CAVM_SDPX_IRE_LINT_ENA_W1SX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_ire_lint_w1s#
 *
 * SDP Input Error Status Set Register
 * This register sets interrupt bits.
 */
union cavm_sdpx_ire_lint_w1sx
{
    uint64_t u;
    struct cavm_sdpx_ire_lint_w1sx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_IRE_LINT(0..3)[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_IRE_LINT(0..3)[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_ire_lint_w1sx_s cn; */
};
typedef union cavm_sdpx_ire_lint_w1sx cavm_sdpx_ire_lint_w1sx_t;

static inline uint64_t CAVM_SDPX_IRE_LINT_W1SX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_IRE_LINT_W1SX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080500ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_IRE_LINT_W1SX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_IRE_LINT_W1SX(a,b) cavm_sdpx_ire_lint_w1sx_t
#define bustype_CAVM_SDPX_IRE_LINT_W1SX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_IRE_LINT_W1SX(a,b) "SDPX_IRE_LINT_W1SX"
#define device_bar_CAVM_SDPX_IRE_LINT_W1SX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_IRE_LINT_W1SX(a,b) (a)
#define arguments_CAVM_SDPX_IRE_LINT_W1SX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_lmac_const0#
 *
 * SDP Logical MAC Capabilities Register 0
 * These registers along with SDP()_LMAC_CONST1() create a table of logical MAC
 * capabilities.  Each entry is 128 bits, with half the information in SDP()_LMAC_CONST0()
 * and half in SDP()_LMAC_CONST1().
 * The list ends with an entry where [V] is clear.
 *
 * Internal:
 * For CN93XX the table is as follows:
 * * SDP(0)_LMAC_CONST0/1(0) [ V=1 EP=1 IFTY=0 IFN=0 MAC=0 PF=8 EPF=0 VFS=128
 * VRINGS=128 PRINGS=64 TRINGS=256].
 * * SDP(0)_LMAC_CONST0/1(1) [ V=1 EP=0 IFTY=0 IFN=1 MAC=1 PF=0 EPF=2 VFS=0   VRINGS=0
 * PRINGS=0  TRINGS=0  ].
 * * SDP(0)_LMAC_CONST0/1(2) [ V=1 EP=1 IFTY=0 IFN=2 MAC=2 PF=8 EPF=1 VFS=128
 * VRINGS=128 PRINGS=64 TRINGS=256].
 * * SDP(0)_LMAC_CONST0/1(3) [ V=1 EP=0 IFTY=0 IFN=3 MAC=3 PF=0 EPF=3 VFS=0   VRINGS=0
 * PRINGS=0  TRINGS=0  ].
 * * SDP(0)_LMAC_CONST0/1(4) [ V=0 ].
 */
union cavm_sdpx_lmac_const0x
{
    uint64_t u;
    struct cavm_sdpx_lmac_const0x_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_40_63        : 24;
        uint64_t epf                   : 8;  /**< [ 39: 32](RO) EPF number. Indicates the index number to EPF registers, e.g. the second index
                                                                 of SDP()_EPF()_RINFO. */
        uint64_t pf                    : 8;  /**< [ 31: 24](RO) Max number of physical functions that are supported.
                                                                 SDP()_MAC()_PF_RING_CTL[NPFS] should not exceed [PF]. */
        uint64_t mac                   : 8;  /**< [ 23: 16](RO) Relative MAC number. Indicates the index number to MAC registers, e.g. the
                                                                 second index of SDP()_MAC()_PF_RING_CTL. */
        uint64_t ifn                   : 8;  /**< [ 15:  8](RO) Interface number. Indicates the physical PEM number. */
        uint64_t ifty                  : 4;  /**< [  7:  4](RO) Interface type.
                                                                 0x0 = PEM. */
        uint64_t reserved_2_3          : 2;
        uint64_t ep                    : 1;  /**< [  1:  1](RO) Endpoint.
                                                                 0 = This MAC/PF does not support endpoint mode; many registers are not
                                                                 implemented including input and output ring-based registers. MSI-X message
                                                                 generation is also not implemented.
                                                                 1 = This MAC/PF combination supports endpoint mode. */
        uint64_t v                     : 1;  /**< [  0:  0](RO) Valid entry.
                                                                 0 = Fields in this register will all be zero. This ends the list of capabilities.
                                                                 1 = Fields are valid. There will be at least one subsequent list entry. */
#else /* Word 0 - Little Endian */
        uint64_t v                     : 1;  /**< [  0:  0](RO) Valid entry.
                                                                 0 = Fields in this register will all be zero. This ends the list of capabilities.
                                                                 1 = Fields are valid. There will be at least one subsequent list entry. */
        uint64_t ep                    : 1;  /**< [  1:  1](RO) Endpoint.
                                                                 0 = This MAC/PF does not support endpoint mode; many registers are not
                                                                 implemented including input and output ring-based registers. MSI-X message
                                                                 generation is also not implemented.
                                                                 1 = This MAC/PF combination supports endpoint mode. */
        uint64_t reserved_2_3          : 2;
        uint64_t ifty                  : 4;  /**< [  7:  4](RO) Interface type.
                                                                 0x0 = PEM. */
        uint64_t ifn                   : 8;  /**< [ 15:  8](RO) Interface number. Indicates the physical PEM number. */
        uint64_t mac                   : 8;  /**< [ 23: 16](RO) Relative MAC number. Indicates the index number to MAC registers, e.g. the
                                                                 second index of SDP()_MAC()_PF_RING_CTL. */
        uint64_t pf                    : 8;  /**< [ 31: 24](RO) Max number of physical functions that are supported.
                                                                 SDP()_MAC()_PF_RING_CTL[NPFS] should not exceed [PF]. */
        uint64_t epf                   : 8;  /**< [ 39: 32](RO) EPF number. Indicates the index number to EPF registers, e.g. the second index
                                                                 of SDP()_EPF()_RINFO. */
        uint64_t reserved_40_63        : 24;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_lmac_const0x_s cn; */
};
typedef union cavm_sdpx_lmac_const0x cavm_sdpx_lmac_const0x_t;

static inline uint64_t CAVM_SDPX_LMAC_CONST0X(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_LMAC_CONST0X(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=4)))
        return 0x86e0c0050000ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x7);
    __cavm_csr_fatal("SDPX_LMAC_CONST0X", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_LMAC_CONST0X(a,b) cavm_sdpx_lmac_const0x_t
#define bustype_CAVM_SDPX_LMAC_CONST0X(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_LMAC_CONST0X(a,b) "SDPX_LMAC_CONST0X"
#define device_bar_CAVM_SDPX_LMAC_CONST0X(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_LMAC_CONST0X(a,b) (a)
#define arguments_CAVM_SDPX_LMAC_CONST0X(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_lmac_const1#
 *
 * SDP Logical MAC Capabilities Register 1
 * See SDP()_LMAC_CONST0().
 */
union cavm_sdpx_lmac_const1x
{
    uint64_t u;
    struct cavm_sdpx_lmac_const1x_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t trings                : 16; /**< [ 63: 48](RO) Total number of rings.
                                                                 If SDP()_LMAC_CONST0()[EP] is set then this field indicates the total available
                                                                 rings that can be assigned to all functions in all MACS.
                                                                 Physical rings are assigned per MAC in SDP()_MAC(f)_PF_RING_CTL.
                                                                 Virtual rings are assigned per EPF in SDP()_EPF()_RINFO.
                                                                 The total number of rings programmed in all EPF SDP()_EPF()_RINFO and
                                                                 MAC SDP()_MAC(f)_PF_RING_CTL registers cannot exceed [TRINGS].

                                                                 If SDP()_LMAC_CONST0()[EP] is clear then this field will be zero. */
        uint64_t prings                : 16; /**< [ 47: 32](RO) Max number of rings under the physical function.
                                                                 If SDP()_LMAC_CONST0()[EP] is set then this field indicates the total available
                                                                 rings that can be assigned to each physical function.
                                                                 Rings are assigned per MAC in SDP()_MAC()_PF_RING_CTL.
                                                                 SDP()_MAC()_PF_RING_CTL[RPPF] cannot exceed [PRINGS].

                                                                 If SDP()_LMAC_CONST0()[EP] is clear then this field will be zero. */
        uint64_t vrings                : 16; /**< [ 31: 16](RO) Max Number of virtual rings under a EPF.
                                                                 If SDP()_LMAC_CONST0()[EP] is set then this field indicates the number of rings
                                                                 that can be programmed in SDP()_EPF()_RINFO.
                                                                 SDP()_EPF()_RINFO[NVFS] * SDP()_EPF()_RINFO[RPVF] cannot exceed [VRINGS].

                                                                 If SDP()_LMAC_CONST0()[EP] is clear then this field will be zero. */
        uint64_t vfs                   : 16; /**< [ 15:  0](RO) Max Number of virtual functions under a EPF.
                                                                 The maximum number that may be programmed in SDP()_EPF()_RINFO[NVFS]. */
#else /* Word 0 - Little Endian */
        uint64_t vfs                   : 16; /**< [ 15:  0](RO) Max Number of virtual functions under a EPF.
                                                                 The maximum number that may be programmed in SDP()_EPF()_RINFO[NVFS]. */
        uint64_t vrings                : 16; /**< [ 31: 16](RO) Max Number of virtual rings under a EPF.
                                                                 If SDP()_LMAC_CONST0()[EP] is set then this field indicates the number of rings
                                                                 that can be programmed in SDP()_EPF()_RINFO.
                                                                 SDP()_EPF()_RINFO[NVFS] * SDP()_EPF()_RINFO[RPVF] cannot exceed [VRINGS].

                                                                 If SDP()_LMAC_CONST0()[EP] is clear then this field will be zero. */
        uint64_t prings                : 16; /**< [ 47: 32](RO) Max number of rings under the physical function.
                                                                 If SDP()_LMAC_CONST0()[EP] is set then this field indicates the total available
                                                                 rings that can be assigned to each physical function.
                                                                 Rings are assigned per MAC in SDP()_MAC()_PF_RING_CTL.
                                                                 SDP()_MAC()_PF_RING_CTL[RPPF] cannot exceed [PRINGS].

                                                                 If SDP()_LMAC_CONST0()[EP] is clear then this field will be zero. */
        uint64_t trings                : 16; /**< [ 63: 48](RO) Total number of rings.
                                                                 If SDP()_LMAC_CONST0()[EP] is set then this field indicates the total available
                                                                 rings that can be assigned to all functions in all MACS.
                                                                 Physical rings are assigned per MAC in SDP()_MAC(f)_PF_RING_CTL.
                                                                 Virtual rings are assigned per EPF in SDP()_EPF()_RINFO.
                                                                 The total number of rings programmed in all EPF SDP()_EPF()_RINFO and
                                                                 MAC SDP()_MAC(f)_PF_RING_CTL registers cannot exceed [TRINGS].

                                                                 If SDP()_LMAC_CONST0()[EP] is clear then this field will be zero. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_lmac_const1x_s cn; */
};
typedef union cavm_sdpx_lmac_const1x cavm_sdpx_lmac_const1x_t;

static inline uint64_t CAVM_SDPX_LMAC_CONST1X(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_LMAC_CONST1X(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=4)))
        return 0x86e0c0051000ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x7);
    __cavm_csr_fatal("SDPX_LMAC_CONST1X", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_LMAC_CONST1X(a,b) cavm_sdpx_lmac_const1x_t
#define bustype_CAVM_SDPX_LMAC_CONST1X(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_LMAC_CONST1X(a,b) "SDPX_LMAC_CONST1X"
#define device_bar_CAVM_SDPX_LMAC_CONST1X(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_LMAC_CONST1X(a,b) (a)
#define arguments_CAVM_SDPX_LMAC_CONST1X(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_mac#_pf_ring_ctl
 *
 * SDP PF Ring Control Register
 * This register sets up the PF rings on each MAC including the starting rings numbers
 * and total rings per PF.
 * This register does not effect PEM configuration registers or how the PEM's are
 * advertised during scan of the PCIe bus.
 *
 * SDP()_EPVF_RING()[EPVF] must be written to values that
 * correlate with the fields in this register.
 *
 * Note: MAC1 and MAC3 does not exist in CN93XX and CNF95XX.
 * Note: MAC2 does not exist in CNF95XX.
 */
union cavm_sdpx_macx_pf_ring_ctl
{
    uint64_t u;
    struct cavm_sdpx_macx_pf_ring_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_22_63        : 42;
        uint64_t rppf                  : 6;  /**< [ 21: 16](R/W) Rings per PF. */
        uint64_t srn                   : 8;  /**< [ 15:  8](R/W) Starting ring number of PF rings. */
        uint64_t reserved_4_7          : 4;
        uint64_t npfs                  : 4;  /**< [  3:  0](R/W) Number of PFs for this MAC. */
#else /* Word 0 - Little Endian */
        uint64_t npfs                  : 4;  /**< [  3:  0](R/W) Number of PFs for this MAC. */
        uint64_t reserved_4_7          : 4;
        uint64_t srn                   : 8;  /**< [ 15:  8](R/W) Starting ring number of PF rings. */
        uint64_t rppf                  : 6;  /**< [ 21: 16](R/W) Rings per PF. */
        uint64_t reserved_22_63        : 42;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_macx_pf_ring_ctl_s cn; */
};
typedef union cavm_sdpx_macx_pf_ring_ctl cavm_sdpx_macx_pf_ring_ctl_t;

static inline uint64_t CAVM_SDPX_MACX_PF_RING_CTL(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_MACX_PF_RING_CTL(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e08002c000ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_MACX_PF_RING_CTL", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_MACX_PF_RING_CTL(a,b) cavm_sdpx_macx_pf_ring_ctl_t
#define bustype_CAVM_SDPX_MACX_PF_RING_CTL(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_MACX_PF_RING_CTL(a,b) "SDPX_MACX_PF_RING_CTL"
#define device_bar_CAVM_SDPX_MACX_PF_RING_CTL(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_MACX_PF_RING_CTL(a,b) (a)
#define arguments_CAVM_SDPX_MACX_PF_RING_CTL(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_mac_number
 *
 * SDP MAC Number Register
 * When read from a MAC, this register returns the MAC's port number, otherwise returns zero.
 */
union cavm_sdpx_mac_number
{
    uint64_t u;
    struct cavm_sdpx_mac_number_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t chip_rev              : 8;  /**< [ 31: 24](RO/H) Chip revision. See FUS_FUSE_NUM_E::CHIP_ID(). */
        uint64_t reserved_9_23         : 15;
        uint64_t a_mode                : 1;  /**< [  8:  8](RO/H) Trusted mode.  See RST_BOOT[TRUSTED_MODE]. */
        uint64_t num                   : 8;  /**< [  7:  0](RO/H) MAC number. */
#else /* Word 0 - Little Endian */
        uint64_t num                   : 8;  /**< [  7:  0](RO/H) MAC number. */
        uint64_t a_mode                : 1;  /**< [  8:  8](RO/H) Trusted mode.  See RST_BOOT[TRUSTED_MODE]. */
        uint64_t reserved_9_23         : 15;
        uint64_t chip_rev              : 8;  /**< [ 31: 24](RO/H) Chip revision. See FUS_FUSE_NUM_E::CHIP_ID(). */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    struct cavm_sdpx_mac_number_cn
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t chip_rev              : 8;  /**< [ 31: 24](RO/H) Chip revision. See FUS_FUSE_NUM_E::CHIP_ID(). */
        uint64_t reserved_20_23        : 4;
        uint64_t reserved_16_19        : 4;
        uint64_t reserved_9_15         : 7;
        uint64_t a_mode                : 1;  /**< [  8:  8](RO/H) Trusted mode.  See RST_BOOT[TRUSTED_MODE]. */
        uint64_t num                   : 8;  /**< [  7:  0](RO/H) MAC number. */
#else /* Word 0 - Little Endian */
        uint64_t num                   : 8;  /**< [  7:  0](RO/H) MAC number. */
        uint64_t a_mode                : 1;  /**< [  8:  8](RO/H) Trusted mode.  See RST_BOOT[TRUSTED_MODE]. */
        uint64_t reserved_9_15         : 7;
        uint64_t reserved_16_19        : 4;
        uint64_t reserved_20_23        : 4;
        uint64_t chip_rev              : 8;  /**< [ 31: 24](RO/H) Chip revision. See FUS_FUSE_NUM_E::CHIP_ID(). */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } cn;
};
typedef union cavm_sdpx_mac_number cavm_sdpx_mac_number_t;

static inline uint64_t CAVM_SDPX_MAC_NUMBER(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_MAC_NUMBER(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e08002c100ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_MAC_NUMBER", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_MAC_NUMBER(a) cavm_sdpx_mac_number_t
#define bustype_CAVM_SDPX_MAC_NUMBER(a) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_MAC_NUMBER(a) "SDPX_MAC_NUMBER"
#define device_bar_CAVM_SDPX_MAC_NUMBER(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_MAC_NUMBER(a) (a)
#define arguments_CAVM_SDPX_MAC_NUMBER(a) (a),-1,-1,-1

/**
 * Register (PEXP_NCB) sdp#_mbox_pf_vf_data#
 *
 * SDP PF to VF Mailbox Data Registers
 * These registers are used for communication of data from the PF to VF.
 * A write to this register from the PF will cause the corresponding bit in
 * SDP()_R()_MBOX_PF_VF_INT[INTR] to be set, along with other bits in
 * SDP()_R()_OUT_CNTS[MBOX_INT], and SDP()_R()_IN_CNTS[MBOX_INT].
 * A VF will see this data written by reading its corresponding
 * SDP()_R()_MBOX_PF_VF_DATA register.
 * A read from and AP core will always return 0.
 *
 * This register is indexed by ring.
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_mbox_pf_vf_datax
{
    uint64_t u;
    struct cavm_sdpx_mbox_pf_vf_datax_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W) Communication data from PF to VF. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W) Communication data from PF to VF. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_mbox_pf_vf_datax_s cn; */
};
typedef union cavm_sdpx_mbox_pf_vf_datax cavm_sdpx_mbox_pf_vf_datax_t;

static inline uint64_t CAVM_SDPX_MBOX_PF_VF_DATAX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_MBOX_PF_VF_DATAX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080022000ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_MBOX_PF_VF_DATAX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_MBOX_PF_VF_DATAX(a,b) cavm_sdpx_mbox_pf_vf_datax_t
#define bustype_CAVM_SDPX_MBOX_PF_VF_DATAX(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_MBOX_PF_VF_DATAX(a,b) "SDPX_MBOX_PF_VF_DATAX"
#define device_bar_CAVM_SDPX_MBOX_PF_VF_DATAX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_MBOX_PF_VF_DATAX(a,b) (a)
#define arguments_CAVM_SDPX_MBOX_PF_VF_DATAX(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_mbox_vf_pf_data#
 *
 * SDP PF to VF Mailbox Data Registers
 * These registers are used for communication of data from the VF to PF.
 * This is a mirror copy of SDP()_R()_MBOX_VF_PF_DATA[DATA] but this register
 * is how the PF accesses the data.
 *
 * This register is indexed by ring.
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_mbox_vf_pf_datax
{
    uint64_t u;
    struct cavm_sdpx_mbox_vf_pf_datax_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W) Communication data from VF to PF. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W) Communication data from VF to PF. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_mbox_vf_pf_datax_s cn; */
};
typedef union cavm_sdpx_mbox_vf_pf_datax cavm_sdpx_mbox_vf_pf_datax_t;

static inline uint64_t CAVM_SDPX_MBOX_VF_PF_DATAX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_MBOX_VF_PF_DATAX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080024000ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_MBOX_VF_PF_DATAX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_MBOX_VF_PF_DATAX(a,b) cavm_sdpx_mbox_vf_pf_datax_t
#define bustype_CAVM_SDPX_MBOX_VF_PF_DATAX(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_MBOX_VF_PF_DATAX(a,b) "SDPX_MBOX_VF_PF_DATAX"
#define device_bar_CAVM_SDPX_MBOX_VF_PF_DATAX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_MBOX_VF_PF_DATAX(a,b) (a)
#define arguments_CAVM_SDPX_MBOX_VF_PF_DATAX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_mcast_table#
 *
 * SDP Output Multicast Table Register
 * Implements SDP Multicast Table consisting of a chain of rings and next pointers
 * for SDP multicast outbound packets from NIX TX.
 */
union cavm_sdpx_mcast_tablex
{
    uint64_t u;
    struct cavm_sdpx_mcast_tablex_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_19_63        : 45;
        uint64_t ring                  : 8;  /**< [ 18: 11](R/W) Indicates a SDP output ring in multicast chain. */
        uint64_t nxt_ptr               : 11; /**< [ 10:  0](R/W) Points to the next pointer index in SDP()_MCAST_TABLE().  The next pointer
                                                                 index follows multicast chain of rings.  If 0x7FF is written to [NXT_PTR],
                                                                 indicates last this is the last ring in the multicast chain of rings. */
#else /* Word 0 - Little Endian */
        uint64_t nxt_ptr               : 11; /**< [ 10:  0](R/W) Points to the next pointer index in SDP()_MCAST_TABLE().  The next pointer
                                                                 index follows multicast chain of rings.  If 0x7FF is written to [NXT_PTR],
                                                                 indicates last this is the last ring in the multicast chain of rings. */
        uint64_t ring                  : 8;  /**< [ 18: 11](R/W) Indicates a SDP output ring in multicast chain. */
        uint64_t reserved_19_63        : 45;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_mcast_tablex_s cn; */
};
typedef union cavm_sdpx_mcast_tablex cavm_sdpx_mcast_tablex_t;

static inline uint64_t CAVM_SDPX_MCAST_TABLEX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_MCAST_TABLEX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=2047)))
        return 0x86e0c0040000ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x7ff);
    __cavm_csr_fatal("SDPX_MCAST_TABLEX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_MCAST_TABLEX(a,b) cavm_sdpx_mcast_tablex_t
#define bustype_CAVM_SDPX_MCAST_TABLEX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_MCAST_TABLEX(a,b) "SDPX_MCAST_TABLEX"
#define device_bar_CAVM_SDPX_MCAST_TABLEX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_MCAST_TABLEX(a,b) (a)
#define arguments_CAVM_SDPX_MCAST_TABLEX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_ord_lint#
 *
 * SDP Output Drop Status Register
 * This register indicates if an output ring has transitioned from no drop state to
 * drop state indicated by a rings' status in SDP()_OUT_DROP_STATE().
 * The given register associated with an EPF will be reset due to a PF FLR or MAC Reset.
 * SDP()_ORD_LINT(0) holds status of rings 0..63.
 * SDP()_ORD_LINT(1) holds status of rings 64..127.
 * SDP()_ORD_LINT(2) holds status of rings 128..191.
 * SDP()_ORD_LINT(3) holds status of rings 192..255.
 * These registers are not affected by VF FLR.
 */
union cavm_sdpx_ord_lintx
{
    uint64_t u;
    struct cavm_sdpx_ord_lintx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_drop_state       : 64; /**< [ 63:  0](R/W1C/H) Ring has transitioned to drop state. */
#else /* Word 0 - Little Endian */
        uint64_t ring_drop_state       : 64; /**< [ 63:  0](R/W1C/H) Ring has transitioned to drop state. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_ord_lintx_s cn; */
};
typedef union cavm_sdpx_ord_lintx cavm_sdpx_ord_lintx_t;

static inline uint64_t CAVM_SDPX_ORD_LINTX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_ORD_LINTX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080c00ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_ORD_LINTX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_ORD_LINTX(a,b) cavm_sdpx_ord_lintx_t
#define bustype_CAVM_SDPX_ORD_LINTX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_ORD_LINTX(a,b) "SDPX_ORD_LINTX"
#define device_bar_CAVM_SDPX_ORD_LINTX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_ORD_LINTX(a,b) (a)
#define arguments_CAVM_SDPX_ORD_LINTX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_ord_lint_ena_w1c#
 *
 * SDP Output Drop Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_ord_lint_ena_w1cx
{
    uint64_t u;
    struct cavm_sdpx_ord_lint_ena_w1cx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_drop_state       : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_ORD_LINT(0..3)[RING_DROP_STATE]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_drop_state       : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_ORD_LINT(0..3)[RING_DROP_STATE]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_ord_lint_ena_w1cx_s cn; */
};
typedef union cavm_sdpx_ord_lint_ena_w1cx cavm_sdpx_ord_lint_ena_w1cx_t;

static inline uint64_t CAVM_SDPX_ORD_LINT_ENA_W1CX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_ORD_LINT_ENA_W1CX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080e00ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_ORD_LINT_ENA_W1CX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_ORD_LINT_ENA_W1CX(a,b) cavm_sdpx_ord_lint_ena_w1cx_t
#define bustype_CAVM_SDPX_ORD_LINT_ENA_W1CX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_ORD_LINT_ENA_W1CX(a,b) "SDPX_ORD_LINT_ENA_W1CX"
#define device_bar_CAVM_SDPX_ORD_LINT_ENA_W1CX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_ORD_LINT_ENA_W1CX(a,b) (a)
#define arguments_CAVM_SDPX_ORD_LINT_ENA_W1CX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_ord_lint_ena_w1s#
 *
 * SDP Output Drop Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_ord_lint_ena_w1sx
{
    uint64_t u;
    struct cavm_sdpx_ord_lint_ena_w1sx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_drop_state       : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_ORD_LINT(0..3)[RING_DROP_STATE]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_drop_state       : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_ORD_LINT(0..3)[RING_DROP_STATE]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_ord_lint_ena_w1sx_s cn; */
};
typedef union cavm_sdpx_ord_lint_ena_w1sx cavm_sdpx_ord_lint_ena_w1sx_t;

static inline uint64_t CAVM_SDPX_ORD_LINT_ENA_W1SX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_ORD_LINT_ENA_W1SX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080f00ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_ORD_LINT_ENA_W1SX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_ORD_LINT_ENA_W1SX(a,b) cavm_sdpx_ord_lint_ena_w1sx_t
#define bustype_CAVM_SDPX_ORD_LINT_ENA_W1SX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_ORD_LINT_ENA_W1SX(a,b) "SDPX_ORD_LINT_ENA_W1SX"
#define device_bar_CAVM_SDPX_ORD_LINT_ENA_W1SX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_ORD_LINT_ENA_W1SX(a,b) (a)
#define arguments_CAVM_SDPX_ORD_LINT_ENA_W1SX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_ord_lint_w1s#
 *
 * SDP Output Drop Status Set Register
 * This register sets interrupt bits.
 */
union cavm_sdpx_ord_lint_w1sx
{
    uint64_t u;
    struct cavm_sdpx_ord_lint_w1sx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_drop_state       : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_ORD_LINT(0..3)[RING_DROP_STATE]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_drop_state       : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_ORD_LINT(0..3)[RING_DROP_STATE]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_ord_lint_w1sx_s cn; */
};
typedef union cavm_sdpx_ord_lint_w1sx cavm_sdpx_ord_lint_w1sx_t;

static inline uint64_t CAVM_SDPX_ORD_LINT_W1SX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_ORD_LINT_W1SX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080d00ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_ORD_LINT_W1SX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_ORD_LINT_W1SX(a,b) cavm_sdpx_ord_lint_w1sx_t
#define bustype_CAVM_SDPX_ORD_LINT_W1SX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_ORD_LINT_W1SX(a,b) "SDPX_ORD_LINT_W1SX"
#define device_bar_CAVM_SDPX_ORD_LINT_W1SX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_ORD_LINT_W1SX(a,b) (a)
#define arguments_CAVM_SDPX_ORD_LINT_W1SX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_ore_lint#
 *
 * SDP Output Error Status Register
 * This register indicates if an error has been detected on an output ring.
 * The given register associated with an EPF will be reset due to a PF FLR or MAC Reset.
 * SDP()_ORE_LINT(0) holds status of rings 0..63.
 * SDP()_ORE_LINT(1) holds status of rings 64..127.
 * SDP()_ORE_LINT(2) holds status of rings 128..191.
 * SDP()_ORE_LINT(3) holds status of rings 192..255.
 * These registers are not affected by VF FLR.
 */
union cavm_sdpx_ore_lintx
{
    uint64_t u;
    struct cavm_sdpx_ore_lintx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Error has been detected on output ring i. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Error has been detected on output ring i. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_ore_lintx_s cn; */
};
typedef union cavm_sdpx_ore_lintx cavm_sdpx_ore_lintx_t;

static inline uint64_t CAVM_SDPX_ORE_LINTX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_ORE_LINTX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080800ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_ORE_LINTX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_ORE_LINTX(a,b) cavm_sdpx_ore_lintx_t
#define bustype_CAVM_SDPX_ORE_LINTX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_ORE_LINTX(a,b) "SDPX_ORE_LINTX"
#define device_bar_CAVM_SDPX_ORE_LINTX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_ORE_LINTX(a,b) (a)
#define arguments_CAVM_SDPX_ORE_LINTX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_ore_lint_ena_w1c#
 *
 * SDP Output Error Enable Clear Register
 * This register clears interrupt enable bits.
 */
union cavm_sdpx_ore_lint_ena_w1cx
{
    uint64_t u;
    struct cavm_sdpx_ore_lint_ena_w1cx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_ORE_LINT(0..3)[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1C/H) Reads or clears enable for SDP(0)_ORE_LINT(0..3)[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_ore_lint_ena_w1cx_s cn; */
};
typedef union cavm_sdpx_ore_lint_ena_w1cx cavm_sdpx_ore_lint_ena_w1cx_t;

static inline uint64_t CAVM_SDPX_ORE_LINT_ENA_W1CX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_ORE_LINT_ENA_W1CX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080a00ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_ORE_LINT_ENA_W1CX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_ORE_LINT_ENA_W1CX(a,b) cavm_sdpx_ore_lint_ena_w1cx_t
#define bustype_CAVM_SDPX_ORE_LINT_ENA_W1CX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_ORE_LINT_ENA_W1CX(a,b) "SDPX_ORE_LINT_ENA_W1CX"
#define device_bar_CAVM_SDPX_ORE_LINT_ENA_W1CX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_ORE_LINT_ENA_W1CX(a,b) (a)
#define arguments_CAVM_SDPX_ORE_LINT_ENA_W1CX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_ore_lint_ena_w1s#
 *
 * SDP Output Error Enable Set Register
 * This register sets interrupt enable bits.
 */
union cavm_sdpx_ore_lint_ena_w1sx
{
    uint64_t u;
    struct cavm_sdpx_ore_lint_ena_w1sx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_ORE_LINT(0..3)[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets enable for SDP(0)_ORE_LINT(0..3)[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_ore_lint_ena_w1sx_s cn; */
};
typedef union cavm_sdpx_ore_lint_ena_w1sx cavm_sdpx_ore_lint_ena_w1sx_t;

static inline uint64_t CAVM_SDPX_ORE_LINT_ENA_W1SX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_ORE_LINT_ENA_W1SX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080b00ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_ORE_LINT_ENA_W1SX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_ORE_LINT_ENA_W1SX(a,b) cavm_sdpx_ore_lint_ena_w1sx_t
#define bustype_CAVM_SDPX_ORE_LINT_ENA_W1SX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_ORE_LINT_ENA_W1SX(a,b) "SDPX_ORE_LINT_ENA_W1SX"
#define device_bar_CAVM_SDPX_ORE_LINT_ENA_W1SX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_ORE_LINT_ENA_W1SX(a,b) (a)
#define arguments_CAVM_SDPX_ORE_LINT_ENA_W1SX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_ore_lint_w1s#
 *
 * SDP Output Error Status Set Register
 * This register sets interrupt bits.
 */
union cavm_sdpx_ore_lint_w1sx
{
    uint64_t u;
    struct cavm_sdpx_ore_lint_w1sx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_ORE_LINT(0..3)[RING_ERR]. */
#else /* Word 0 - Little Endian */
        uint64_t ring_err              : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_ORE_LINT(0..3)[RING_ERR]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_ore_lint_w1sx_s cn; */
};
typedef union cavm_sdpx_ore_lint_w1sx cavm_sdpx_ore_lint_w1sx_t;

static inline uint64_t CAVM_SDPX_ORE_LINT_W1SX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_ORE_LINT_W1SX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080900ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_ORE_LINT_W1SX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_ORE_LINT_W1SX(a,b) cavm_sdpx_ore_lint_w1sx_t
#define bustype_CAVM_SDPX_ORE_LINT_W1SX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_ORE_LINT_W1SX(a,b) "SDPX_ORE_LINT_W1SX"
#define device_bar_CAVM_SDPX_ORE_LINT_W1SX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_ORE_LINT_W1SX(a,b) (a)
#define arguments_CAVM_SDPX_ORE_LINT_W1SX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_out_bp_en#_w1c
 *
 * SDP Packet Output Backpressure W1C Register
 * This register enables sending backpressure to NIX TX.
 * SDP()_OUT_BP_EN(0)_W1C sets rings 0..63.
 * SDP()_OUT_BP_EN(1)_W1C sets rings 64..127.
 * SDP()_OUT_BP_EN(2)_W1C sets rings 128..191.
 * SDP()_OUT_BP_EN(3)_W1C sets rings 192..255.
 */
union cavm_sdpx_out_bp_enx_w1c
{
    uint64_t u;
    struct cavm_sdpx_out_bp_enx_w1c_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t enb                   : 64; /**< [ 63:  0](R/W1C/H) When set, enables the channel-level backpressure to be sent to PKO. Backpressure is sent
                                                                 to the PKO on the channels 0x100-0x13F (index 0) and 0x140-17F (index 1).
                                                                 See SDP()_OUT_WMARK[WMARK].
                                                                 A read of this register will return the current value of the enables for those channels. */
#else /* Word 0 - Little Endian */
        uint64_t enb                   : 64; /**< [ 63:  0](R/W1C/H) When set, enables the channel-level backpressure to be sent to PKO. Backpressure is sent
                                                                 to the PKO on the channels 0x100-0x13F (index 0) and 0x140-17F (index 1).
                                                                 See SDP()_OUT_WMARK[WMARK].
                                                                 A read of this register will return the current value of the enables for those channels. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_out_bp_enx_w1c_s cn8; */
    struct cavm_sdpx_out_bp_enx_w1c_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t enb                   : 64; /**< [ 63:  0](R/W1C/H) When set, enables the channel-level backpressure to be sent to NIX TX. Backpressure is sent
                                                                 to the NIX TX on the channels 0x100-0x13F (index 0) and 0x140-17F (index 1).
                                                                 See SDP()_OUT_WMARK[WMARK].
                                                                 A read of this register will return the current value of the enables for those channels. */
#else /* Word 0 - Little Endian */
        uint64_t enb                   : 64; /**< [ 63:  0](R/W1C/H) When set, enables the channel-level backpressure to be sent to NIX TX. Backpressure is sent
                                                                 to the NIX TX on the channels 0x100-0x13F (index 0) and 0x140-17F (index 1).
                                                                 See SDP()_OUT_WMARK[WMARK].
                                                                 A read of this register will return the current value of the enables for those channels. */
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_sdpx_out_bp_enx_w1c cavm_sdpx_out_bp_enx_w1c_t;

static inline uint64_t CAVM_SDPX_OUT_BP_ENX_W1C(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_OUT_BP_ENX_W1C(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=1)))
        return 0x874000880220ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080240ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_OUT_BP_ENX_W1C", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_OUT_BP_ENX_W1C(a,b) cavm_sdpx_out_bp_enx_w1c_t
#define bustype_CAVM_SDPX_OUT_BP_ENX_W1C(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_OUT_BP_ENX_W1C(a,b) "SDPX_OUT_BP_ENX_W1C"
#define device_bar_CAVM_SDPX_OUT_BP_ENX_W1C(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_OUT_BP_ENX_W1C(a,b) (a)
#define arguments_CAVM_SDPX_OUT_BP_ENX_W1C(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_out_bp_en#_w1s
 *
 * SDP Packet Output Backpressure W1S Register
 * This register reads or sets bits.
 */
union cavm_sdpx_out_bp_enx_w1s
{
    uint64_t u;
    struct cavm_sdpx_out_bp_enx_w1s_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t enb                   : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_OUT_BP_EN(0..1)_W1C[ENB]. */
#else /* Word 0 - Little Endian */
        uint64_t enb                   : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_OUT_BP_EN(0..1)_W1C[ENB]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_out_bp_enx_w1s_s cn8; */
    struct cavm_sdpx_out_bp_enx_w1s_cn9
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t enb                   : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_OUT_BP_EN(0..3)_W1C[ENB]. */
#else /* Word 0 - Little Endian */
        uint64_t enb                   : 64; /**< [ 63:  0](R/W1S/H) Reads or sets SDP(0)_OUT_BP_EN(0..3)_W1C[ENB]. */
#endif /* Word 0 - End */
    } cn9;
};
typedef union cavm_sdpx_out_bp_enx_w1s cavm_sdpx_out_bp_enx_w1s_t;

static inline uint64_t CAVM_SDPX_OUT_BP_ENX_W1S(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_OUT_BP_ENX_W1S(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && ((a==0) && (b<=1)))
        return 0x874000880240ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x1);
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0080280ll + 0x1000000000ll * ((a) & 0x0) + 0x10ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_OUT_BP_ENX_W1S", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_OUT_BP_ENX_W1S(a,b) cavm_sdpx_out_bp_enx_w1s_t
#define bustype_CAVM_SDPX_OUT_BP_ENX_W1S(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_OUT_BP_ENX_W1S(a,b) "SDPX_OUT_BP_ENX_W1S"
#define device_bar_CAVM_SDPX_OUT_BP_ENX_W1S(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_OUT_BP_ENX_W1S(a,b) (a)
#define arguments_CAVM_SDPX_OUT_BP_ENX_W1S(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_out_bp_timer#
 *
 * SDP Output Packet Backpressure Timer Register
 * These registers hold timers that increment every 1024 cycles when a SDP out packet ring
 * buffers fall below SDP()_OUT_WMARK[WMARK] and SDP starts asserting backpressure to NIX TX
 * if SDP()_OUT_BP_EN()_W1S[ENB] is set.
 * IF SDP stops asserting backpressure to NIX TX for a SDP out packet ring, the
 * associated SDP()_OUT_BP_TIMER()[TIMER] will clear.
 *
 * This register is indexed by ring and would normally be accessed from an AP core where all
 * 256 rings can be indexed.
 * A PF would index this register based upon the settings of SDP()_EPF()_RINFO[SRN]
 * and SDP()_MAC()_PF_RING_CTL[SRN].
 * These register are for debug purposes only.
 */
union cavm_sdpx_out_bp_timerx
{
    uint64_t u;
    struct cavm_sdpx_out_bp_timerx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t timer                 : 32; /**< [ 31:  0](RO/H) Timer that is compared to SDP(0)_OUT_BP_TIMER_CTL[TIMER_LIMIT] to put ring in drop state. */
#else /* Word 0 - Little Endian */
        uint64_t timer                 : 32; /**< [ 31:  0](RO/H) Timer that is compared to SDP(0)_OUT_BP_TIMER_CTL[TIMER_LIMIT] to put ring in drop state. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_out_bp_timerx_s cn; */
};
typedef union cavm_sdpx_out_bp_timerx cavm_sdpx_out_bp_timerx_t;

static inline uint64_t CAVM_SDPX_OUT_BP_TIMERX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_OUT_BP_TIMERX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e0c0061000ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_OUT_BP_TIMERX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_OUT_BP_TIMERX(a,b) cavm_sdpx_out_bp_timerx_t
#define bustype_CAVM_SDPX_OUT_BP_TIMERX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_OUT_BP_TIMERX(a,b) "SDPX_OUT_BP_TIMERX"
#define device_bar_CAVM_SDPX_OUT_BP_TIMERX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_OUT_BP_TIMERX(a,b) (a)
#define arguments_CAVM_SDPX_OUT_BP_TIMERX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_out_bp_timer_ctl
 *
 * SDP Output Packet Backpressure Timer Control Register
 * This register defines a timer limit to detect bad SDP output rings.
 * SDP()_OUT_BP_TIMER()[TIMER] will start to increment when a SDP out packet ring buffers
 * fall below SDP()_OUT_WMARK[WMARK] and SDP starts asserting backpressure to NIX TX if
 * SDP()_OUT_BP_EN()_W1C[ENB] is set.
 *
 * IF SDP stops asserting backpressure to NIX TX for a SDP out packet ring the
 * associated SDP()_OUT_BP_TIMER()[TIMER] will clear.
 *
 * If SDP()_OUT_BP_TIMER()[TIMER] reaches [TIMER_LIMIT] the associated bit in
 * SDP()_OUT_DROP_STATE()[STATE] will be set for that ring and an interrupt bit will be
 * set in SDP()_ORD_LINT().
 */
union cavm_sdpx_out_bp_timer_ctl
{
    uint64_t u;
    struct cavm_sdpx_out_bp_timer_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t timer_limit           : 32; /**< [ 31:  0](R/W) If the WMARK backpressure timer exceeds [TIMER_LIMIT] an interrupt bit will be
                                                                 set in SDP()_ORD_LINT(). If 0x0 this feature is disabled. */
#else /* Word 0 - Little Endian */
        uint64_t timer_limit           : 32; /**< [ 31:  0](R/W) If the WMARK backpressure timer exceeds [TIMER_LIMIT] an interrupt bit will be
                                                                 set in SDP()_ORD_LINT(). If 0x0 this feature is disabled. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_out_bp_timer_ctl_s cn; */
};
typedef union cavm_sdpx_out_bp_timer_ctl cavm_sdpx_out_bp_timer_ctl_t;

static inline uint64_t CAVM_SDPX_OUT_BP_TIMER_CTL(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_OUT_BP_TIMER_CTL(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0c0060010ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_OUT_BP_TIMER_CTL", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_OUT_BP_TIMER_CTL(a) cavm_sdpx_out_bp_timer_ctl_t
#define bustype_CAVM_SDPX_OUT_BP_TIMER_CTL(a) CSR_TYPE_NCB
#define basename_CAVM_SDPX_OUT_BP_TIMER_CTL(a) "SDPX_OUT_BP_TIMER_CTL"
#define device_bar_CAVM_SDPX_OUT_BP_TIMER_CTL(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_OUT_BP_TIMER_CTL(a) (a)
#define arguments_CAVM_SDPX_OUT_BP_TIMER_CTL(a) (a),-1,-1,-1

/**
 * Register (NCB) sdp#_out_drop_state#
 *
 * SDP Output Packet Drop State Register
 * These registers holds the SDP Output ring drop state bitwise vectors for all SDP Output
 * rings.  At reset all rings start out with STATE=1 and will drop all packets.
 * Once a ring has wrote SDP()_R()_OUT_SLIST_DBELL with sufficient buffers to go above
 * SDP()_OUT_WMARK[WMARK] SDP will set the appropriate bit in STATE to 0.  This will allow
 * SDP to stop dropping packets for that ring.
 * If a ring is in STATE=0 the following conditions can cause a ring to transition to
 * STATE=1 and start dropping packets again.
 *
 * * SDP()_R()_OUT_ENABLE[ENB] bit is 0.
 * * A packet is received from NIX TX and there are no corresponding buffers available
 *   for that output ring to send the packet. See SDP()_R()_OUT_CONTROL[DROP_CNT].
 * * SDP()_OUT_BP_TIMER()[TIMER] for a ring has reached SDP()_OUT_BP_TIMER_CTL[TIMER_LIMIT]
 *   and SDP()_OUT_BP_TIMER_CTL[TIMER_LIMIT] is nonzero.
 */
union cavm_sdpx_out_drop_statex
{
    uint64_t u;
    struct cavm_sdpx_out_drop_statex_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t state                 : 64; /**< [ 63:  0](RO/H) Bit vector drop state. A value of 1 implies that a ring is in drop state and will
                                                                 drop all packets that it receives from NIX TX destined for that ring.  A value of 0
                                                                 implies that SDP will stop dropping packets and send EBP backpressure before buffers
                                                                 become too low to send packets.  When a ring transitions from 0 to 1, the corresponding
                                                                 interrupt bit in SDP()_ORD_LINT() will be set. */
#else /* Word 0 - Little Endian */
        uint64_t state                 : 64; /**< [ 63:  0](RO/H) Bit vector drop state. A value of 1 implies that a ring is in drop state and will
                                                                 drop all packets that it receives from NIX TX destined for that ring.  A value of 0
                                                                 implies that SDP will stop dropping packets and send EBP backpressure before buffers
                                                                 become too low to send packets.  When a ring transitions from 0 to 1, the corresponding
                                                                 interrupt bit in SDP()_ORD_LINT() will be set. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_out_drop_statex_s cn; */
};
typedef union cavm_sdpx_out_drop_statex cavm_sdpx_out_drop_statex_t;

static inline uint64_t CAVM_SDPX_OUT_DROP_STATEX(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_OUT_DROP_STATEX(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=3)))
        return 0x86e0c0060020ll + 0x1000000000ll * ((a) & 0x0) + 8ll * ((b) & 0x3);
    __cavm_csr_fatal("SDPX_OUT_DROP_STATEX", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_OUT_DROP_STATEX(a,b) cavm_sdpx_out_drop_statex_t
#define bustype_CAVM_SDPX_OUT_DROP_STATEX(a,b) CSR_TYPE_NCB
#define basename_CAVM_SDPX_OUT_DROP_STATEX(a,b) "SDPX_OUT_DROP_STATEX"
#define device_bar_CAVM_SDPX_OUT_DROP_STATEX(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_OUT_DROP_STATEX(a,b) (a)
#define arguments_CAVM_SDPX_OUT_DROP_STATEX(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_out_mcast_ctl
 *
 * SDP Output Packet Mcast Control Register
 * This register contain control and state for MCAST packets.
 */
union cavm_sdpx_out_mcast_ctl
{
    uint64_t u;
    struct cavm_sdpx_out_mcast_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t idle                  : 1;  /**< [  0:  0](RO/H) IDLE is set if all multicast packets received by SDP have had there table look up completed.
                                                                 Software would normally query this first and then the specific
                                                                 SDP()_R()_OUT_CONTROL[IDLE] bits of the chain to see if all packets of a MCAST chain
                                                                 have been sent. */
#else /* Word 0 - Little Endian */
        uint64_t idle                  : 1;  /**< [  0:  0](RO/H) IDLE is set if all multicast packets received by SDP have had there table look up completed.
                                                                 Software would normally query this first and then the specific
                                                                 SDP()_R()_OUT_CONTROL[IDLE] bits of the chain to see if all packets of a MCAST chain
                                                                 have been sent. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_out_mcast_ctl_s cn; */
};
typedef union cavm_sdpx_out_mcast_ctl cavm_sdpx_out_mcast_ctl_t;

static inline uint64_t CAVM_SDPX_OUT_MCAST_CTL(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_OUT_MCAST_CTL(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0c0060200ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_OUT_MCAST_CTL", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_OUT_MCAST_CTL(a) cavm_sdpx_out_mcast_ctl_t
#define bustype_CAVM_SDPX_OUT_MCAST_CTL(a) CSR_TYPE_NCB
#define basename_CAVM_SDPX_OUT_MCAST_CTL(a) "SDPX_OUT_MCAST_CTL"
#define device_bar_CAVM_SDPX_OUT_MCAST_CTL(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_OUT_MCAST_CTL(a) (a)
#define arguments_CAVM_SDPX_OUT_MCAST_CTL(a) (a),-1,-1,-1

/**
 * Register (NCB) sdp#_out_wmark
 *
 * SDP Output Packet Watermark Register
 * This register defines a minimum pointer pair threshold required in order to send output
 * packets to the SDP. If the port's DBELL count is less than this threshold,
 * the corresponding bit in SDP()_OUT_DROP_STATE() is clear, and
 * the corresponding bit in SDP()_OUT_BP_EN()_W1S is set, back-pressure is
 * indicated to NIX TX.
 *
 * In the event that a packet arrives at SDP while the corresponding bit in
 * SDP()_OUT_DROP_STATE() is 0 and there are no buffers available to store it,
 * SDP will set the corresponding bit in SDP()_OUT_DROP_STATE()
 * to 1 and discard remaining packets destined for the port.
 * This should not happen except for a mis-programmed threshold value.
 */
union cavm_sdpx_out_wmark
{
    uint64_t u;
    struct cavm_sdpx_out_wmark_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t wmark                 : 32; /**< [ 31:  0](R/W) Threshold value in pointer-pairs. */
#else /* Word 0 - Little Endian */
        uint64_t wmark                 : 32; /**< [ 31:  0](R/W) Threshold value in pointer-pairs. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_out_wmark_s cn; */
};
typedef union cavm_sdpx_out_wmark cavm_sdpx_out_wmark_t;

static inline uint64_t CAVM_SDPX_OUT_WMARK(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_OUT_WMARK(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x874000880000ll + 0x1000000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0c0060000ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_OUT_WMARK", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_OUT_WMARK(a) cavm_sdpx_out_wmark_t
#define bustype_CAVM_SDPX_OUT_WMARK(a) CSR_TYPE_NCB
#define basename_CAVM_SDPX_OUT_WMARK(a) "SDPX_OUT_WMARK"
#define device_bar_CAVM_SDPX_OUT_WMARK(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_OUT_WMARK(a) (a)
#define arguments_CAVM_SDPX_OUT_WMARK(a) (a),-1,-1,-1

/**
 * Register (NCB) sdp#_pkind_valid
 *
 * SDP Packet PKIND Valid Register
 * Enables bits per PKIND that are allowed to be sent to PKI specified in the
 * SDP_INSTR_HDR_S[PKIND] packet instruction field.
 */
union cavm_sdpx_pkind_valid
{
    uint64_t u;
    struct cavm_sdpx_pkind_valid_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t enb                   : 64; /**< [ 63:  0](R/W) Enables bits for 64 possible pkinds.  If set to a 1, the corresponding
                                                                 SDP_INSTR_HDR_S[PKIND] is allowed to be passed to PKI.  If set to a 0,
                                                                 the SDP_INSTR_HDR_S[PKIND] will be changed to the pkind set in
                                                                 SDP()_GBL_CONTROL[BPKIND] when sent to PKI. */
#else /* Word 0 - Little Endian */
        uint64_t enb                   : 64; /**< [ 63:  0](R/W) Enables bits for 64 possible pkinds.  If set to a 1, the corresponding
                                                                 SDP_INSTR_HDR_S[PKIND] is allowed to be passed to PKI.  If set to a 0,
                                                                 the SDP_INSTR_HDR_S[PKIND] will be changed to the pkind set in
                                                                 SDP()_GBL_CONTROL[BPKIND] when sent to PKI. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_pkind_valid_s cn; */
};
typedef union cavm_sdpx_pkind_valid cavm_sdpx_pkind_valid_t;

static inline uint64_t CAVM_SDPX_PKIND_VALID(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_PKIND_VALID(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN83XX) && (a==0))
        return 0x874000880210ll + 0x1000000000ll * ((a) & 0x0);
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0c0080210ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_PKIND_VALID", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_PKIND_VALID(a) cavm_sdpx_pkind_valid_t
#define bustype_CAVM_SDPX_PKIND_VALID(a) CSR_TYPE_NCB
#define basename_CAVM_SDPX_PKIND_VALID(a) "SDPX_PKIND_VALID"
#define device_bar_CAVM_SDPX_PKIND_VALID(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_PKIND_VALID(a) (a)
#define arguments_CAVM_SDPX_PKIND_VALID(a) (a),-1,-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_err_type
 *
 * SDP Ring Error Type Register
 * These registers indicate which type of error(s) have been detected when
 * SDP()_ORE_LINT(), SDP()_EPF()_ORE_RINT, SDP()_IRE_LINT(), or SDP()_EPF()_IRE_RINT is
 * set. Multiple bits can be set at the same time if multiple errors have occurred for
 * that ring.
 *
 * These registers are not affected by VF FLR, PF FLR or MAC Reset.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_err_type
{
    uint64_t u;
    struct cavm_sdpx_rx_err_type_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_37_63        : 27;
        uint64_t drop_timer            : 1;  /**< [ 36: 36](R/W1C/H) SDP()_OUT_BP_TIMER() reached SDP()_OUT_BP_TIMER_CTL[TIMER_LIMIT] causing ring to go into
                                                                 drop state. */
        uint64_t port_dis              : 1;  /**< [ 35: 35](R/W1C/H) Output port was in non-drop state and enabled, then was disabled causing ring to
                                                                 go into drop state. */
        uint64_t dbell_empty           : 1;  /**< [ 34: 34](R/W1C/H) A packet is received from NIX TX and there are no corresponding buffers available
                                                                 for that output ring to send the packet causing ring to go into drop state..
                                                                 See SDP()_R()_OUT_CONTROL[DROP_CNT]. */
        uint64_t oring_drop_cnt_err    : 1;  /**< [ 33: 33](R/W1C/H) SDP()_R()_OUT_CONTROL[DROP_CNT] was misprogrammed allowing an output packet
                                                                 to be sent greater then buffers available.
                                                                 Will disabled output ring. */
        uint64_t oring_dma_err         : 1;  /**< [ 32: 32](R/W1C/H) DMA read error or poison response on output pointer pair fetch.
                                                                 Will disabled output ring */
        uint64_t reserved_9_31         : 23;
        uint64_t illegal_dir_gthr      : 1;  /**< [  8:  8](R/W1C/H) Illegal Direct gather instruction.
                                                                 Direct gather has been removed and instructions that have SDP_INSTR_HDR_S[G]=1 and
                                                                 SDP_INSTR_HDR_S[GSZ]=0x0 are illegal. */
        uint64_t illegal_fsz           : 1;  /**< [  7:  7](R/W1C/H) Illegal FSZ specified in instruction.
                                                                 For direct gather, FSZ must be \<= 32 for 64B instructions and 0 for 32B instructions.
                                                                 For direct data/indirect gather, FSZ must be \<= 55 for 64B instructions and \<= 23 for 32B
                                                                 instructions. This check is done before any length checks. */
        uint64_t pkt_dma_err           : 1;  /**< [  6:  6](R/W1C/H) DMA read error response on packet fetch. */
        uint64_t inst_dma_err          : 1;  /**< [  5:  5](R/W1C/H) DMA read error or poison response on instruction fetch. */
        uint64_t pkt_toosmall          : 1;  /**< [  4:  4](R/W1C/H) Attempted packet read with LEN=0 or LEN \< FSZ. */
        uint64_t dir_len_toosmall      : 1;  /**< [  3:  3](R/W1C/H) Direct gather combined LEN fields are less than the packet length specified. */
        uint64_t ind_dma_err           : 1;  /**< [  2:  2](R/W1C/H) DMA read error or poison response on indirect gather list fetch.  This could also be caused by
                                                                 an unaligned gather list, in which case SDP()_DIAG[IN_IND_UNALIGNED] will also be set. */
        uint64_t ind_zero_det          : 1;  /**< [  1:  1](R/W1C/H) Indirect gather list contains length of 0. */
        uint64_t ind_toosmall          : 1;  /**< [  0:  0](R/W1C/H) Indirect gather list length specified less than (packet length - FSZ) in instruction. */
#else /* Word 0 - Little Endian */
        uint64_t ind_toosmall          : 1;  /**< [  0:  0](R/W1C/H) Indirect gather list length specified less than (packet length - FSZ) in instruction. */
        uint64_t ind_zero_det          : 1;  /**< [  1:  1](R/W1C/H) Indirect gather list contains length of 0. */
        uint64_t ind_dma_err           : 1;  /**< [  2:  2](R/W1C/H) DMA read error or poison response on indirect gather list fetch.  This could also be caused by
                                                                 an unaligned gather list, in which case SDP()_DIAG[IN_IND_UNALIGNED] will also be set. */
        uint64_t dir_len_toosmall      : 1;  /**< [  3:  3](R/W1C/H) Direct gather combined LEN fields are less than the packet length specified. */
        uint64_t pkt_toosmall          : 1;  /**< [  4:  4](R/W1C/H) Attempted packet read with LEN=0 or LEN \< FSZ. */
        uint64_t inst_dma_err          : 1;  /**< [  5:  5](R/W1C/H) DMA read error or poison response on instruction fetch. */
        uint64_t pkt_dma_err           : 1;  /**< [  6:  6](R/W1C/H) DMA read error response on packet fetch. */
        uint64_t illegal_fsz           : 1;  /**< [  7:  7](R/W1C/H) Illegal FSZ specified in instruction.
                                                                 For direct gather, FSZ must be \<= 32 for 64B instructions and 0 for 32B instructions.
                                                                 For direct data/indirect gather, FSZ must be \<= 55 for 64B instructions and \<= 23 for 32B
                                                                 instructions. This check is done before any length checks. */
        uint64_t illegal_dir_gthr      : 1;  /**< [  8:  8](R/W1C/H) Illegal Direct gather instruction.
                                                                 Direct gather has been removed and instructions that have SDP_INSTR_HDR_S[G]=1 and
                                                                 SDP_INSTR_HDR_S[GSZ]=0x0 are illegal. */
        uint64_t reserved_9_31         : 23;
        uint64_t oring_dma_err         : 1;  /**< [ 32: 32](R/W1C/H) DMA read error or poison response on output pointer pair fetch.
                                                                 Will disabled output ring */
        uint64_t oring_drop_cnt_err    : 1;  /**< [ 33: 33](R/W1C/H) SDP()_R()_OUT_CONTROL[DROP_CNT] was misprogrammed allowing an output packet
                                                                 to be sent greater then buffers available.
                                                                 Will disabled output ring. */
        uint64_t dbell_empty           : 1;  /**< [ 34: 34](R/W1C/H) A packet is received from NIX TX and there are no corresponding buffers available
                                                                 for that output ring to send the packet causing ring to go into drop state..
                                                                 See SDP()_R()_OUT_CONTROL[DROP_CNT]. */
        uint64_t port_dis              : 1;  /**< [ 35: 35](R/W1C/H) Output port was in non-drop state and enabled, then was disabled causing ring to
                                                                 go into drop state. */
        uint64_t drop_timer            : 1;  /**< [ 36: 36](R/W1C/H) SDP()_OUT_BP_TIMER() reached SDP()_OUT_BP_TIMER_CTL[TIMER_LIMIT] causing ring to go into
                                                                 drop state. */
        uint64_t reserved_37_63        : 27;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_err_type_s cn; */
};
typedef union cavm_sdpx_rx_err_type cavm_sdpx_rx_err_type_t;

static inline uint64_t CAVM_SDPX_RX_ERR_TYPE(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_ERR_TYPE(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010400ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_ERR_TYPE", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_ERR_TYPE(a,b) cavm_sdpx_rx_err_type_t
#define bustype_CAVM_SDPX_RX_ERR_TYPE(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_ERR_TYPE(a,b) "SDPX_RX_ERR_TYPE"
#define device_bar_CAVM_SDPX_RX_ERR_TYPE(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_ERR_TYPE(a,b) (a)
#define arguments_CAVM_SDPX_RX_ERR_TYPE(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_byte_cnt
 *
 * SDP Packet Input Byte Count Register
 * This register contains byte counts per ring that have been read into SDP.
 * The counter will wrap when it reaches its maximum value. It should be cleared
 * before the ring is enabled for an accurate count.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_byte_cnt
{
    uint64_t u;
    struct cavm_sdpx_rx_in_byte_cnt_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_48_63        : 16;
        uint64_t cnt                   : 48; /**< [ 47:  0](R/W/H) Byte count, can be reset by software by writing SDP()_R()_IN_PKT_CNT[CNT]
                                                                 with 0xFFFFFFFFF. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 48; /**< [ 47:  0](R/W/H) Byte count, can be reset by software by writing SDP()_R()_IN_PKT_CNT[CNT]
                                                                 with 0xFFFFFFFFF. */
        uint64_t reserved_48_63        : 16;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_byte_cnt_s cn; */
};
typedef union cavm_sdpx_rx_in_byte_cnt cavm_sdpx_rx_in_byte_cnt_t;

static inline uint64_t CAVM_SDPX_RX_IN_BYTE_CNT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_BYTE_CNT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010090ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_BYTE_CNT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_BYTE_CNT(a,b) cavm_sdpx_rx_in_byte_cnt_t
#define bustype_CAVM_SDPX_RX_IN_BYTE_CNT(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_BYTE_CNT(a,b) "SDPX_RX_IN_BYTE_CNT"
#define device_bar_CAVM_SDPX_RX_IN_BYTE_CNT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_BYTE_CNT(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_BYTE_CNT(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_cnts
 *
 * SDP Input Instruction Ring Counts Register
 * This register contains the counters for the input instruction rings.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the VF prior to enabling the ring.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_cnts
{
    uint64_t u;
    struct cavm_sdpx_rx_in_cnts_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t send_ism              : 1;  /**< [ 63: 63](WO/H) A write of 1 will cause a ISM message to be sent with the current value of [CNT].
                                                                 Hardware will ignore the written value of [CNT] if [SEND_ISM] is set and will NOT
                                                                 subtract the amount written to [CNT]. ISM messages must be enabled in
                                                                 SDP()_R()_IN_CNTS_ISM.EN.  Hardware will also ignore a write of 1 to the [RESEND] bit
                                                                 if 1 is written to this field.
                                                                 Note this is orthogonal to normal ISM messages that will always be sent before a counts
                                                                 interrupt if enabled. */
        uint64_t out_int               : 1;  /**< [ 62: 62](RO/H) Returns a 1 when:
                                                                  * SDP()_R()_OUT_CNTS[CNT] \> SDP()_R()_OUT_INT_LEVELS[CNT] &
                                                                    SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_OUT_CNTS[TIMER] \> SDP()_R()_OUT_INT_LEVELS[TIMET] &
                                                                    SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_OUT_INT_MDRT_CTL1[ENABLE]==1 &
                                                                    SDP()_R()_OUT_CNTS[TIMER] has reached an interrupt threshold based
                                                                    on the values in the SDP()_R()_OUT_INT_MDRT_CTL* registers.
                                                                 To clear the bit, the SDP()_R()_OUT_CNTS register must be written to clear the
                                                                 underlying condition.

                                                                 Internal:
                                                                 These interrupt bits are not cleared due to FLR becase the CNTS and
                                                                 LEVELS registers are not reset and we wish to make the interrupt state
                                                                 consistent with CNTS/LEVELS even after FLR. The CNTS register must be
                                                                 cleared by software as part of initialization after a reset (including FLR)
                                                                 which will cause the interrupt state to clear. */
        uint64_t in_int                : 1;  /**< [ 61: 61](RO/H) Returns a 1 when:
                                                                  * SDP()_R()_IN_CNTS[CNT] \> SDP()_R()_IN_INT_LEVELS[CNT] &
                                                                    SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_IN_CNTS[TIMER] \> SDP()_R()_IN_INT_LEVELS[TIMET] &
                                                                    SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_IN_INT_MDRT_CTL1[ENABLE]=1 &
                                                                    SDP()_R()_IN_CNTS[TIMER] has reached an interrupt threshold based
                                                                    on the values in the SDP()_R()_IN_INT_MDRT_CTL* registers.
                                                                 To clear the bit, the SDP()_R()_IN_CNTS register must be written to clear the
                                                                 underlying condition.

                                                                 Internal:
                                                                 These interrupt bits are not cleared due to FLR becase the CNTS and
                                                                 LEVELS registers are not reset and we wish to make the interrupt state
                                                                 consistent with CNTS/LEVELS even after FLR. The CNTS register must be
                                                                 cleared by software as part of initialization after a reset (including FLR)
                                                                 which will cause the interrupt state to clear. */
        uint64_t mbox_int              : 1;  /**< [ 60: 60](RO/H) Returns a 1 when:
                                                                  * SDP()_R()_MBOX_PF_VF_INT[INTR] is set

                                                                 To clear the bit, write SDP()_R()_MBOX_PF_VF_INT[INTR] with 1.
                                                                 This bit is also cleared due to an FLR. */
        uint64_t resend                : 1;  /**< [ 59: 59](WO/H) A write of 1 will resend an MSI-X interrupt message and a corresponding ISM message if enabled
                                                                 if any of the following conditions are true for the respective ring:
                                                                  * SDP()_R()_IN_CNTS[CNT] \> SDP()_R()_IN_INT_LEVELS[CNT] &
                                                                    SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_IN_CNTS[TIMER] \> SDP()_R()_IN_INT_LEVELS[TIMET] &
                                                                    SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]==1.

                                                                 If SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]=0 a write of
                                                                 [RESEND] will clear [IN_INT].  This is generally used when
                                                                 SDP()_R()_IN_INT_MDRT_CTL1[ENABLE]==1 to prepare for the next MDRT
                                                                 timer interrupt event.

                                                                 If [SEND_ISM] is also written to a 1 writes of 1 to [RESEND] will be ignored. */
        uint64_t reserved_54_58        : 5;
        uint64_t timer                 : 22; /**< [ 53: 32](RO/H) Timer, incremented every 1024 coprocessor-clock cycles when [CNT] is
                                                                 not zero. The hardware clears [TIMER] when [CNT] is written to a
                                                                 nonzero value. The first increment of this count can occur between 0 to
                                                                 1023 coprocessor-clock cycles after [CNT] becomes nonzero. */
        uint64_t cnt                   : 32; /**< [ 31:  0](R/W/H) Packet counter. Hardware adds to [CNT] as it reads packets. On a write
                                                                 to this CSR, hardware subtracts the amount written to the [CNT] field from
                                                                 [CNT], which will clear SDP()_R()_IN_CNTS[IN_INT] if [CNT] becomes \<=
                                                                 SDP()_R()_IN_INT_LEVELS[CNT]. This register should be cleared before
                                                                 enabling a ring by reading the current value and writing it back. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 32; /**< [ 31:  0](R/W/H) Packet counter. Hardware adds to [CNT] as it reads packets. On a write
                                                                 to this CSR, hardware subtracts the amount written to the [CNT] field from
                                                                 [CNT], which will clear SDP()_R()_IN_CNTS[IN_INT] if [CNT] becomes \<=
                                                                 SDP()_R()_IN_INT_LEVELS[CNT]. This register should be cleared before
                                                                 enabling a ring by reading the current value and writing it back. */
        uint64_t timer                 : 22; /**< [ 53: 32](RO/H) Timer, incremented every 1024 coprocessor-clock cycles when [CNT] is
                                                                 not zero. The hardware clears [TIMER] when [CNT] is written to a
                                                                 nonzero value. The first increment of this count can occur between 0 to
                                                                 1023 coprocessor-clock cycles after [CNT] becomes nonzero. */
        uint64_t reserved_54_58        : 5;
        uint64_t resend                : 1;  /**< [ 59: 59](WO/H) A write of 1 will resend an MSI-X interrupt message and a corresponding ISM message if enabled
                                                                 if any of the following conditions are true for the respective ring:
                                                                  * SDP()_R()_IN_CNTS[CNT] \> SDP()_R()_IN_INT_LEVELS[CNT] &
                                                                    SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_IN_CNTS[TIMER] \> SDP()_R()_IN_INT_LEVELS[TIMET] &
                                                                    SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]==1.

                                                                 If SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]=0 a write of
                                                                 [RESEND] will clear [IN_INT].  This is generally used when
                                                                 SDP()_R()_IN_INT_MDRT_CTL1[ENABLE]==1 to prepare for the next MDRT
                                                                 timer interrupt event.

                                                                 If [SEND_ISM] is also written to a 1 writes of 1 to [RESEND] will be ignored. */
        uint64_t mbox_int              : 1;  /**< [ 60: 60](RO/H) Returns a 1 when:
                                                                  * SDP()_R()_MBOX_PF_VF_INT[INTR] is set

                                                                 To clear the bit, write SDP()_R()_MBOX_PF_VF_INT[INTR] with 1.
                                                                 This bit is also cleared due to an FLR. */
        uint64_t in_int                : 1;  /**< [ 61: 61](RO/H) Returns a 1 when:
                                                                  * SDP()_R()_IN_CNTS[CNT] \> SDP()_R()_IN_INT_LEVELS[CNT] &
                                                                    SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_IN_CNTS[TIMER] \> SDP()_R()_IN_INT_LEVELS[TIMET] &
                                                                    SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_IN_INT_MDRT_CTL1[ENABLE]=1 &
                                                                    SDP()_R()_IN_CNTS[TIMER] has reached an interrupt threshold based
                                                                    on the values in the SDP()_R()_IN_INT_MDRT_CTL* registers.
                                                                 To clear the bit, the SDP()_R()_IN_CNTS register must be written to clear the
                                                                 underlying condition.

                                                                 Internal:
                                                                 These interrupt bits are not cleared due to FLR becase the CNTS and
                                                                 LEVELS registers are not reset and we wish to make the interrupt state
                                                                 consistent with CNTS/LEVELS even after FLR. The CNTS register must be
                                                                 cleared by software as part of initialization after a reset (including FLR)
                                                                 which will cause the interrupt state to clear. */
        uint64_t out_int               : 1;  /**< [ 62: 62](RO/H) Returns a 1 when:
                                                                  * SDP()_R()_OUT_CNTS[CNT] \> SDP()_R()_OUT_INT_LEVELS[CNT] &
                                                                    SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_OUT_CNTS[TIMER] \> SDP()_R()_OUT_INT_LEVELS[TIMET] &
                                                                    SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_OUT_INT_MDRT_CTL1[ENABLE]==1 &
                                                                    SDP()_R()_OUT_CNTS[TIMER] has reached an interrupt threshold based
                                                                    on the values in the SDP()_R()_OUT_INT_MDRT_CTL* registers.
                                                                 To clear the bit, the SDP()_R()_OUT_CNTS register must be written to clear the
                                                                 underlying condition.

                                                                 Internal:
                                                                 These interrupt bits are not cleared due to FLR becase the CNTS and
                                                                 LEVELS registers are not reset and we wish to make the interrupt state
                                                                 consistent with CNTS/LEVELS even after FLR. The CNTS register must be
                                                                 cleared by software as part of initialization after a reset (including FLR)
                                                                 which will cause the interrupt state to clear. */
        uint64_t send_ism              : 1;  /**< [ 63: 63](WO/H) A write of 1 will cause a ISM message to be sent with the current value of [CNT].
                                                                 Hardware will ignore the written value of [CNT] if [SEND_ISM] is set and will NOT
                                                                 subtract the amount written to [CNT]. ISM messages must be enabled in
                                                                 SDP()_R()_IN_CNTS_ISM.EN.  Hardware will also ignore a write of 1 to the [RESEND] bit
                                                                 if 1 is written to this field.
                                                                 Note this is orthogonal to normal ISM messages that will always be sent before a counts
                                                                 interrupt if enabled. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_cnts_s cn; */
};
typedef union cavm_sdpx_rx_in_cnts cavm_sdpx_rx_in_cnts_t;

static inline uint64_t CAVM_SDPX_RX_IN_CNTS(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_CNTS(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010050ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_CNTS", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_CNTS(a,b) cavm_sdpx_rx_in_cnts_t
#define bustype_CAVM_SDPX_RX_IN_CNTS(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_CNTS(a,b) "SDPX_RX_IN_CNTS"
#define device_bar_CAVM_SDPX_RX_IN_CNTS(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_CNTS(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_CNTS(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_cnts_ism
 *
 * SDP Ring Input Counts Status Message Register
 * This register configures input count status messages.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_cnts_ism
{
    uint64_t u;
    struct cavm_sdpx_rx_in_cnts_ism_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_53_63        : 11;
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) PCI address hardware will write status messages to. See [EN]. */
        uint64_t msix_dis              : 1;  /**< [  1:  1](R/W) Disables MSIX interrupt and setting PBA bits for input packet interrupts
                                                                 when only using ISM messages for completion status. */
        uint64_t en                    : 1;  /**< [  0:  0](R/W) Enable writing input counts messages. When set, before sending an input counts
                                                                 interrupt MSI to the host, hardware will write the value of value of
                                                                 SDP()_R()_IN_CNTS[CNT] to the 32-bits at [ADDR]. This does not cause an
                                                                 interrupt; instead software may use this inside the standard
                                                                 input-counts-initiated interrupt handler to appropriately handle the interrupt. */
#else /* Word 0 - Little Endian */
        uint64_t en                    : 1;  /**< [  0:  0](R/W) Enable writing input counts messages. When set, before sending an input counts
                                                                 interrupt MSI to the host, hardware will write the value of value of
                                                                 SDP()_R()_IN_CNTS[CNT] to the 32-bits at [ADDR]. This does not cause an
                                                                 interrupt; instead software may use this inside the standard
                                                                 input-counts-initiated interrupt handler to appropriately handle the interrupt. */
        uint64_t msix_dis              : 1;  /**< [  1:  1](R/W) Disables MSIX interrupt and setting PBA bits for input packet interrupts
                                                                 when only using ISM messages for completion status. */
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) PCI address hardware will write status messages to. See [EN]. */
        uint64_t reserved_53_63        : 11;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_cnts_ism_s cn; */
};
typedef union cavm_sdpx_rx_in_cnts_ism cavm_sdpx_rx_in_cnts_ism_t;

static inline uint64_t CAVM_SDPX_RX_IN_CNTS_ISM(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_CNTS_ISM(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010520ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_CNTS_ISM", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_CNTS_ISM(a,b) cavm_sdpx_rx_in_cnts_ism_t
#define bustype_CAVM_SDPX_RX_IN_CNTS_ISM(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_CNTS_ISM(a,b) "SDPX_RX_IN_CNTS_ISM"
#define device_bar_CAVM_SDPX_RX_IN_CNTS_ISM(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_CNTS_ISM(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_CNTS_ISM(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_control
 *
 * SDP Input Instruction Ring Control Register
 * This register is the control for read operations on the input instruction rings.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the VF prior to enabling the ring.  Also, this register cannot be written
 * while either of the following conditions is true:
 *   * [IDLE] is clear.
 *   * Or, SDP()_R()_IN_ENABLE[ENB] is set.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_control
{
    uint64_t u;
    struct cavm_sdpx_rx_in_control_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_52_63        : 12;
        uint64_t rpvf                  : 4;  /**< [ 51: 48](RO/H) The number of rings assigned to this VF.
                                                                 Read only copy of SDP()_EPF()_RINFO[RPVF] */
        uint64_t reserved_29_47        : 19;
        uint64_t idle                  : 1;  /**< [ 28: 28](RO/H) Asserted when this ring has no packets in-flight. */
        uint64_t reserved_27           : 1;
        uint64_t rdsize                : 2;  /**< [ 26: 25](R/W) Number of instructions to be read in one read request. Two-bit values are:
                                                                 0x0 = 1 instruction.
                                                                 0x1 = 2 instructions.
                                                                 0x2 = 4 instructions.
                                                                 0x3 = 8 instructions. */
        uint64_t is64b                 : 1;  /**< [ 24: 24](R/W) If 1, the ring uses 64-byte instructions.
                                                                 If 0, the ring uses 32-byte instructions. */
        uint64_t reserved_9_23         : 15;
        uint64_t d_nsr                 : 1;  /**< [  8:  8](R/W/H) [D_NSR] is ADDRTYPE\<1\> for first direct and gather DPTR reads. ADDRTYPE\<1\> is the
                                                                 no-snoop attribute for PCIe. */
        uint64_t d_esr                 : 2;  /**< [  7:  6](R/W/H) [D_ESR] is ES\<1:0\> for first direct and gather DPTR reads.
                                                                 ES\<1:0\> is the endian-swap attribute for these MAC memory space reads.
                                                                 Enumerated by DPI_ENDIANSWAP_E. */
        uint64_t d_ror                 : 1;  /**< [  5:  5](R/W/H) [D_ROR] is ADDRTYPE\<0\> for first direct and gather DPTR reads. ADDRTYPE\<0\> is the
                                                                 relaxed-order attribute for PCIe. */
        uint64_t reserved_4            : 1;
        uint64_t nsr                   : 1;  /**< [  3:  3](R/W/H) [NSR] is ADDRTYPE\<1\> for input instruction reads (from
                                                                 SDP()_R()_IN_INSTR_BADDR) and first indirect DPTR reads. ADDRTYPE\<1\>
                                                                 is the no-snoop attribute for PCIe. */
        uint64_t esr                   : 2;  /**< [  2:  1](R/W/H) [ESR] is ES\<1:0\> for input instruction reads (from
                                                                 SDP()_R()_IN_INSTR_BADDR) and first indirect DPTR reads. ES\<1:0\> is
                                                                 the endian-swap attribute for these MAC memory space reads.
                                                                 Enumerated by DPI_ENDIANSWAP_E. */
        uint64_t ror                   : 1;  /**< [  0:  0](R/W/H) [ROR] is ADDRTYPE\<0\> for input instruction reads (from
                                                                 SDP()_R()_IN_INSTR_BADDR) and first indirect DPTR reads.
                                                                 ADDRTYPE\<0\> is the relaxed-order attribute for PCIe. */
#else /* Word 0 - Little Endian */
        uint64_t ror                   : 1;  /**< [  0:  0](R/W/H) [ROR] is ADDRTYPE\<0\> for input instruction reads (from
                                                                 SDP()_R()_IN_INSTR_BADDR) and first indirect DPTR reads.
                                                                 ADDRTYPE\<0\> is the relaxed-order attribute for PCIe. */
        uint64_t esr                   : 2;  /**< [  2:  1](R/W/H) [ESR] is ES\<1:0\> for input instruction reads (from
                                                                 SDP()_R()_IN_INSTR_BADDR) and first indirect DPTR reads. ES\<1:0\> is
                                                                 the endian-swap attribute for these MAC memory space reads.
                                                                 Enumerated by DPI_ENDIANSWAP_E. */
        uint64_t nsr                   : 1;  /**< [  3:  3](R/W/H) [NSR] is ADDRTYPE\<1\> for input instruction reads (from
                                                                 SDP()_R()_IN_INSTR_BADDR) and first indirect DPTR reads. ADDRTYPE\<1\>
                                                                 is the no-snoop attribute for PCIe. */
        uint64_t reserved_4            : 1;
        uint64_t d_ror                 : 1;  /**< [  5:  5](R/W/H) [D_ROR] is ADDRTYPE\<0\> for first direct and gather DPTR reads. ADDRTYPE\<0\> is the
                                                                 relaxed-order attribute for PCIe. */
        uint64_t d_esr                 : 2;  /**< [  7:  6](R/W/H) [D_ESR] is ES\<1:0\> for first direct and gather DPTR reads.
                                                                 ES\<1:0\> is the endian-swap attribute for these MAC memory space reads.
                                                                 Enumerated by DPI_ENDIANSWAP_E. */
        uint64_t d_nsr                 : 1;  /**< [  8:  8](R/W/H) [D_NSR] is ADDRTYPE\<1\> for first direct and gather DPTR reads. ADDRTYPE\<1\> is the
                                                                 no-snoop attribute for PCIe. */
        uint64_t reserved_9_23         : 15;
        uint64_t is64b                 : 1;  /**< [ 24: 24](R/W) If 1, the ring uses 64-byte instructions.
                                                                 If 0, the ring uses 32-byte instructions. */
        uint64_t rdsize                : 2;  /**< [ 26: 25](R/W) Number of instructions to be read in one read request. Two-bit values are:
                                                                 0x0 = 1 instruction.
                                                                 0x1 = 2 instructions.
                                                                 0x2 = 4 instructions.
                                                                 0x3 = 8 instructions. */
        uint64_t reserved_27           : 1;
        uint64_t idle                  : 1;  /**< [ 28: 28](RO/H) Asserted when this ring has no packets in-flight. */
        uint64_t reserved_29_47        : 19;
        uint64_t rpvf                  : 4;  /**< [ 51: 48](RO/H) The number of rings assigned to this VF.
                                                                 Read only copy of SDP()_EPF()_RINFO[RPVF] */
        uint64_t reserved_52_63        : 12;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_control_s cn; */
};
typedef union cavm_sdpx_rx_in_control cavm_sdpx_rx_in_control_t;

static inline uint64_t CAVM_SDPX_RX_IN_CONTROL(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_CONTROL(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010000ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_CONTROL", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_CONTROL(a,b) cavm_sdpx_rx_in_control_t
#define bustype_CAVM_SDPX_RX_IN_CONTROL(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_CONTROL(a,b) "SDPX_RX_IN_CONTROL"
#define device_bar_CAVM_SDPX_RX_IN_CONTROL(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_CONTROL(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_CONTROL(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_enable
 *
 * SDP Input Instruction Ring Enable Register
 * This register is the enable for read operations on the input instruction rings.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_enable
{
    uint64_t u;
    struct cavm_sdpx_rx_in_enable_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t enb                   : 1;  /**< [  0:  0](R/W/H) Enable for the input ring.  Various errors and FLR events can clear this bit.
                                                                 Software can also clear this bit at anytime. The bit may not be set unless
                                                                 SDP()_R()_IN_CONTROL[IDLE] == 0. */
#else /* Word 0 - Little Endian */
        uint64_t enb                   : 1;  /**< [  0:  0](R/W/H) Enable for the input ring.  Various errors and FLR events can clear this bit.
                                                                 Software can also clear this bit at anytime. The bit may not be set unless
                                                                 SDP()_R()_IN_CONTROL[IDLE] == 0. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_enable_s cn; */
};
typedef union cavm_sdpx_rx_in_enable cavm_sdpx_rx_in_enable_t;

static inline uint64_t CAVM_SDPX_RX_IN_ENABLE(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_ENABLE(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010010ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_ENABLE", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_ENABLE(a,b) cavm_sdpx_rx_in_enable_t
#define bustype_CAVM_SDPX_RX_IN_ENABLE(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_ENABLE(a,b) "SDPX_RX_IN_ENABLE"
#define device_bar_CAVM_SDPX_RX_IN_ENABLE(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_ENABLE(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_ENABLE(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_instr_baddr
 *
 * SDP Input Instruction Ring Base Address Register
 * This register contains the base address for the input instruction ring.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the VF prior to enabling the ring.  Also, this register cannot be written
 * while either of the following conditions is true:
 *   * SDP()_R()_IN_CONTROL[IDLE] is clear.
 *   * Or, SDP()_R()_IN_ENABLE[ENB] is set.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_instr_baddr
{
    uint64_t u;
    struct cavm_sdpx_rx_in_instr_baddr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t addr                  : 60; /**< [ 63:  4](R/W) Base address for input instruction ring. Must be 16-byte aligned. */
        uint64_t reserved_0_3          : 4;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_3          : 4;
        uint64_t addr                  : 60; /**< [ 63:  4](R/W) Base address for input instruction ring. Must be 16-byte aligned. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_instr_baddr_s cn; */
};
typedef union cavm_sdpx_rx_in_instr_baddr cavm_sdpx_rx_in_instr_baddr_t;

static inline uint64_t CAVM_SDPX_RX_IN_INSTR_BADDR(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_INSTR_BADDR(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010020ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_INSTR_BADDR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_INSTR_BADDR(a,b) cavm_sdpx_rx_in_instr_baddr_t
#define bustype_CAVM_SDPX_RX_IN_INSTR_BADDR(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_INSTR_BADDR(a,b) "SDPX_RX_IN_INSTR_BADDR"
#define device_bar_CAVM_SDPX_RX_IN_INSTR_BADDR(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_INSTR_BADDR(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_INSTR_BADDR(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_instr_dbell
 *
 * SDP Input Instruction Ring Input Doorbell Registers
 * This register contains the doorbell and base-address offset for the next read operation.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the VF prior to enabling the ring.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_instr_dbell
{
    uint64_t u;
    struct cavm_sdpx_rx_in_instr_dbell_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t aoff                  : 32; /**< [ 63: 32](RO/H) Address offset. The offset from the SDP()_R()_IN_INSTR_BADDR where the
                                                                 next pointer is read. A write of 0xFFFFFFFF to [DBELL] clears [DBELL] and [AOFF]. */
        uint64_t dbell                 : 32; /**< [ 31:  0](R/W/H) Pointer list doorbell count. Write operations to this field increments the present
                                                                 value here. Read operations return the present value. The value of this field is
                                                                 decremented as read operations are issued for instructions. A write of 0xFFFFFFFF
                                                                 to this field clears [DBELL] and [AOFF].  This register should be cleared before
                                                                 enabling a ring. */
#else /* Word 0 - Little Endian */
        uint64_t dbell                 : 32; /**< [ 31:  0](R/W/H) Pointer list doorbell count. Write operations to this field increments the present
                                                                 value here. Read operations return the present value. The value of this field is
                                                                 decremented as read operations are issued for instructions. A write of 0xFFFFFFFF
                                                                 to this field clears [DBELL] and [AOFF].  This register should be cleared before
                                                                 enabling a ring. */
        uint64_t aoff                  : 32; /**< [ 63: 32](RO/H) Address offset. The offset from the SDP()_R()_IN_INSTR_BADDR where the
                                                                 next pointer is read. A write of 0xFFFFFFFF to [DBELL] clears [DBELL] and [AOFF]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_instr_dbell_s cn; */
};
typedef union cavm_sdpx_rx_in_instr_dbell cavm_sdpx_rx_in_instr_dbell_t;

static inline uint64_t CAVM_SDPX_RX_IN_INSTR_DBELL(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_INSTR_DBELL(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010040ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_INSTR_DBELL", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_INSTR_DBELL(a,b) cavm_sdpx_rx_in_instr_dbell_t
#define bustype_CAVM_SDPX_RX_IN_INSTR_DBELL(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_INSTR_DBELL(a,b) "SDPX_RX_IN_INSTR_DBELL"
#define device_bar_CAVM_SDPX_RX_IN_INSTR_DBELL(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_INSTR_DBELL(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_INSTR_DBELL(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_instr_rsize
 *
 * SDP Input Instruction Ring Size Register
 * This register contains the input instruction ring size.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the VF prior to enabling the ring.  Also, this register cannot be written
 * while either of the following conditions is true:
 *   * SDP()_R()_IN_CONTROL[IDLE] is clear.
 *   * or, SDP()_R()_IN_ENABLE[ENB] is set.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_instr_rsize
{
    uint64_t u;
    struct cavm_sdpx_rx_in_instr_rsize_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t rsize                 : 32; /**< [ 31:  0](R/W) Ring size (number of instructions). */
#else /* Word 0 - Little Endian */
        uint64_t rsize                 : 32; /**< [ 31:  0](R/W) Ring size (number of instructions). */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_instr_rsize_s cn; */
};
typedef union cavm_sdpx_rx_in_instr_rsize cavm_sdpx_rx_in_instr_rsize_t;

static inline uint64_t CAVM_SDPX_RX_IN_INSTR_RSIZE(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_INSTR_RSIZE(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010030ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_INSTR_RSIZE", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_INSTR_RSIZE(a,b) cavm_sdpx_rx_in_instr_rsize_t
#define bustype_CAVM_SDPX_RX_IN_INSTR_RSIZE(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_INSTR_RSIZE(a,b) "SDPX_RX_IN_INSTR_RSIZE"
#define device_bar_CAVM_SDPX_RX_IN_INSTR_RSIZE(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_INSTR_RSIZE(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_INSTR_RSIZE(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_int_levels
 *
 * SDP Input Instruction Interrupt Levels Register
 * This register contains input instruction interrupt levels.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the VF prior to enabling the ring.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_int_levels
{
    uint64_t u;
    struct cavm_sdpx_rx_in_int_levels_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t bmode                 : 1;  /**< [ 63: 63](R/W) Determines whether SDP()_R()_IN_CNTS[CNT] is a byte or packet counter. When
                                                                 [BMODE]=1, SDP()_R()_IN_CNTS[CNT] is a byte counter, else SDP()_R()_IN_CNTS[CNT]
                                                                 is a packet counter. */
        uint64_t time_cnt_ena          : 1;  /**< [ 62: 62](R/W) Enables generating MSIX-X interrupts whenever SDP()_R()_IN_CNTS[TIMER] \> [TIMET]
                                                                 or SDP()_R()_IN_CNTS[CNT] \> [CNT] conditions are met. */
        uint64_t max_len_ena           : 1;  /**< [ 61: 61](R/W) Maximum packet length enable. If set, An MSI-X interrupt will be generated for
                                                                 any packet with length greater than [MAX_LEN] KBytes. */
        uint64_t max_len               : 7;  /**< [ 60: 54](R/W) Maximum packet length not to generate interrupt (in KB). IF [MAX_LEN_ENA] is set
                                                                 an MSI-X interrupt will be generated for any packet with length greater than
                                                                 [MAX_LEN] KBytes. */
        uint64_t timet                 : 22; /**< [ 53: 32](R/W) Input port counter time interrupt threshold. An MSI-X interrupt will be generated
                                                                 whenever SDP()_R()_IN_CNTS[TIMER] \> [TIMET] and [TIME_CNT_ENA] is set.
                                                                 Whenever software changes the value of [TIMET], it should also subsequently write
                                                                 the corresponding SDP()_R()_IN_CNTS CSR (with a value of zero if desired) to
                                                                 ensure that the hardware correspondingly updates SDP()_R()_IN_CNTS[IN_INT]. */
        uint64_t cnt                   : 32; /**< [ 31:  0](R/W) Input packet counter interrupt threshold. An MSI-X interrupt will be generated
                                                                 whenever SDP()_R()_IN_CNTS[CNT] \> [CNT] and [TIME_CNT_ENA] is set.
                                                                 Whenever software changes the value of [CNT], it should also subsequently write
                                                                 the corresponding SDP()_R()_IN_CNTS[CNT] CSR (with a value of zero if desired) to
                                                                 ensure that the hardware correspondingly updates SDP()_R()_IN_CNTS[IN_INT] */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 32; /**< [ 31:  0](R/W) Input packet counter interrupt threshold. An MSI-X interrupt will be generated
                                                                 whenever SDP()_R()_IN_CNTS[CNT] \> [CNT] and [TIME_CNT_ENA] is set.
                                                                 Whenever software changes the value of [CNT], it should also subsequently write
                                                                 the corresponding SDP()_R()_IN_CNTS[CNT] CSR (with a value of zero if desired) to
                                                                 ensure that the hardware correspondingly updates SDP()_R()_IN_CNTS[IN_INT] */
        uint64_t timet                 : 22; /**< [ 53: 32](R/W) Input port counter time interrupt threshold. An MSI-X interrupt will be generated
                                                                 whenever SDP()_R()_IN_CNTS[TIMER] \> [TIMET] and [TIME_CNT_ENA] is set.
                                                                 Whenever software changes the value of [TIMET], it should also subsequently write
                                                                 the corresponding SDP()_R()_IN_CNTS CSR (with a value of zero if desired) to
                                                                 ensure that the hardware correspondingly updates SDP()_R()_IN_CNTS[IN_INT]. */
        uint64_t max_len               : 7;  /**< [ 60: 54](R/W) Maximum packet length not to generate interrupt (in KB). IF [MAX_LEN_ENA] is set
                                                                 an MSI-X interrupt will be generated for any packet with length greater than
                                                                 [MAX_LEN] KBytes. */
        uint64_t max_len_ena           : 1;  /**< [ 61: 61](R/W) Maximum packet length enable. If set, An MSI-X interrupt will be generated for
                                                                 any packet with length greater than [MAX_LEN] KBytes. */
        uint64_t time_cnt_ena          : 1;  /**< [ 62: 62](R/W) Enables generating MSIX-X interrupts whenever SDP()_R()_IN_CNTS[TIMER] \> [TIMET]
                                                                 or SDP()_R()_IN_CNTS[CNT] \> [CNT] conditions are met. */
        uint64_t bmode                 : 1;  /**< [ 63: 63](R/W) Determines whether SDP()_R()_IN_CNTS[CNT] is a byte or packet counter. When
                                                                 [BMODE]=1, SDP()_R()_IN_CNTS[CNT] is a byte counter, else SDP()_R()_IN_CNTS[CNT]
                                                                 is a packet counter. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_int_levels_s cn; */
};
typedef union cavm_sdpx_rx_in_int_levels cavm_sdpx_rx_in_int_levels_t;

static inline uint64_t CAVM_SDPX_RX_IN_INT_LEVELS(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_INT_LEVELS(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010060ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_INT_LEVELS", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_INT_LEVELS(a,b) cavm_sdpx_rx_in_int_levels_t
#define bustype_CAVM_SDPX_RX_IN_INT_LEVELS(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_INT_LEVELS(a,b) "SDPX_RX_IN_INT_LEVELS"
#define device_bar_CAVM_SDPX_RX_IN_INT_LEVELS(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_INT_LEVELS(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_INT_LEVELS(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_int_mdrt_ctl0
 *
 * SDP Input Instruction Interrupt Moderation Control 0 Register
 * This register is used for interrupt moderation.
 * See SDP()_R()_IN_INT_MDRT_CTL1.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_int_mdrt_ctl0
{
    uint64_t u;
    struct cavm_sdpx_rx_in_int_mdrt_ctl0_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_54_63        : 10;
        uint64_t hi_timer              : 22; /**< [ 53: 32](R/W) High timer level. */
        uint64_t reserved_22_31        : 10;
        uint64_t lo_timer              : 22; /**< [ 21:  0](R/W) Low timer level. */
#else /* Word 0 - Little Endian */
        uint64_t lo_timer              : 22; /**< [ 21:  0](R/W) Low timer level. */
        uint64_t reserved_22_31        : 10;
        uint64_t hi_timer              : 22; /**< [ 53: 32](R/W) High timer level. */
        uint64_t reserved_54_63        : 10;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_int_mdrt_ctl0_s cn; */
};
typedef union cavm_sdpx_rx_in_int_mdrt_ctl0 cavm_sdpx_rx_in_int_mdrt_ctl0_t;

static inline uint64_t CAVM_SDPX_RX_IN_INT_MDRT_CTL0(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_INT_MDRT_CTL0(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010280ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_INT_MDRT_CTL0", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_INT_MDRT_CTL0(a,b) cavm_sdpx_rx_in_int_mdrt_ctl0_t
#define bustype_CAVM_SDPX_RX_IN_INT_MDRT_CTL0(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_INT_MDRT_CTL0(a,b) "SDPX_RX_IN_INT_MDRT_CTL0"
#define device_bar_CAVM_SDPX_RX_IN_INT_MDRT_CTL0(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_INT_MDRT_CTL0(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_INT_MDRT_CTL0(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_int_mdrt_ctl1
 *
 * SDP Input Instruction Interrupt Moderation Control 1 Register
 * This register is used for interrupt moderation. For each ring the user sets a pair
 * of [LO_RATE] bytes and SDP()_R()_IN_INT_MDRT_CTL0[LO_TIMER] time, and a pair
 * of [HI_RATE] bytes and SDP()_R()_IN_INT_MDRT_CTL0[HI_TIMER] time. The logic
 * will also take as inputs the current byte rate and the current timer value. If the
 * current byte rate is smaller or equal to [LO_RATE] the logic will issue an interrupt
 * if the current timer value is higher than Lo_Timer. Similarly if the current byte
 * rate is greater than [HI_RATE] the logic will issue an interrupt if the current
 * timer value is greater than Hi_Timer. Otherwise, the logic will calculate if the
 * current timer is above the timer level that corresponds to the current byte rate,
 * based on the Byte_Rate to Timer_Level linear relationship and in that case it will
 * issue an interrupt.
 *
 * The Byte_Rate is the moving average of the number of bytes per 1024 cycles and is
 * recalculated for each ring every 1024 cycles based on the old value and the byte
 * count during the last 1024 cycles segment:
 *
 * _ Current_Byte_Rate = (Old_Byte_Rate * (256 - 2^[WEIGHT]) + Byte_Cnt * 2^[WEIGHT]) / 256.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_int_mdrt_ctl1
{
    uint64_t u;
    struct cavm_sdpx_rx_in_int_mdrt_ctl1_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t enable                : 1;  /**< [ 63: 63](R/W) Enable interrupt moderation. */
        uint64_t reserved_44_62        : 19;
        uint64_t weight                : 4;  /**< [ 43: 40](R/W) Weight:
                                                                 0x0 = Weight of 1.
                                                                 0x1 = Weight of 2.
                                                                 0x2 = Weight of 4.
                                                                 0x3 = Weight of 8.
                                                                 0x4 = Weight of 16.
                                                                 0x5 = Weight of 32.
                                                                 0x6 = Weight of 64.
                                                                 0x7 = Weight of 128.
                                                                 0x8 = Weight of 256.
                                                                 _ else = Reserved.

                                                                 Internal:
                                                                 Reserved treated as 0x8. */
        uint64_t reserved_37_39        : 3;
        uint64_t hi_rate               : 17; /**< [ 36: 20](R/W) High byte rate. */
        uint64_t reserved_17_19        : 3;
        uint64_t lo_rate               : 17; /**< [ 16:  0](R/W) Low byte rate. */
#else /* Word 0 - Little Endian */
        uint64_t lo_rate               : 17; /**< [ 16:  0](R/W) Low byte rate. */
        uint64_t reserved_17_19        : 3;
        uint64_t hi_rate               : 17; /**< [ 36: 20](R/W) High byte rate. */
        uint64_t reserved_37_39        : 3;
        uint64_t weight                : 4;  /**< [ 43: 40](R/W) Weight:
                                                                 0x0 = Weight of 1.
                                                                 0x1 = Weight of 2.
                                                                 0x2 = Weight of 4.
                                                                 0x3 = Weight of 8.
                                                                 0x4 = Weight of 16.
                                                                 0x5 = Weight of 32.
                                                                 0x6 = Weight of 64.
                                                                 0x7 = Weight of 128.
                                                                 0x8 = Weight of 256.
                                                                 _ else = Reserved.

                                                                 Internal:
                                                                 Reserved treated as 0x8. */
        uint64_t reserved_44_62        : 19;
        uint64_t enable                : 1;  /**< [ 63: 63](R/W) Enable interrupt moderation. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_int_mdrt_ctl1_s cn; */
};
typedef union cavm_sdpx_rx_in_int_mdrt_ctl1 cavm_sdpx_rx_in_int_mdrt_ctl1_t;

static inline uint64_t CAVM_SDPX_RX_IN_INT_MDRT_CTL1(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_INT_MDRT_CTL1(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e0800102a0ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_INT_MDRT_CTL1", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_INT_MDRT_CTL1(a,b) cavm_sdpx_rx_in_int_mdrt_ctl1_t
#define bustype_CAVM_SDPX_RX_IN_INT_MDRT_CTL1(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_INT_MDRT_CTL1(a,b) "SDPX_RX_IN_INT_MDRT_CTL1"
#define device_bar_CAVM_SDPX_RX_IN_INT_MDRT_CTL1(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_INT_MDRT_CTL1(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_INT_MDRT_CTL1(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_int_mdrt_dbg
 *
 * SDP Input Instruction Interrupt Moderation Debug Register
 * This register is used for interrupt moderation.
 * See SDP()_R()_IN_INT_MDRT_CTL1.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_int_mdrt_dbg
{
    uint64_t u;
    struct cavm_sdpx_rx_in_int_mdrt_dbg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_17_63        : 47;
        uint64_t avg_rate              : 17; /**< [ 16:  0](R/W/H) Average byte rate. */
#else /* Word 0 - Little Endian */
        uint64_t avg_rate              : 17; /**< [ 16:  0](R/W/H) Average byte rate. */
        uint64_t reserved_17_63        : 47;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_int_mdrt_dbg_s cn; */
};
typedef union cavm_sdpx_rx_in_int_mdrt_dbg cavm_sdpx_rx_in_int_mdrt_dbg_t;

static inline uint64_t CAVM_SDPX_RX_IN_INT_MDRT_DBG(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_INT_MDRT_DBG(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e0800102c0ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_INT_MDRT_DBG", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_INT_MDRT_DBG(a,b) cavm_sdpx_rx_in_int_mdrt_dbg_t
#define bustype_CAVM_SDPX_RX_IN_INT_MDRT_DBG(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_INT_MDRT_DBG(a,b) "SDPX_RX_IN_INT_MDRT_DBG"
#define device_bar_CAVM_SDPX_RX_IN_INT_MDRT_DBG(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_INT_MDRT_DBG(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_INT_MDRT_DBG(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_in_pkt_cnt
 *
 * SDP Packet Input Packet Count Register
 * This register contains packet counts per ring that have been read into SDP.
 * The counter will wrap when it reaches its maximum value.  It should be cleared
 * before the ring is enabled for an accurate count.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_in_pkt_cnt
{
    uint64_t u;
    struct cavm_sdpx_rx_in_pkt_cnt_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t cnt                   : 36; /**< [ 35:  0](R/W/H) Packet count, can be written by software to any value.  If a value of 0xFFFFFFFFF is
                                                                 written to this field, it will cause this field as well as SDP()_R()_IN_BYTE_CNT to
                                                                 clear. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 36; /**< [ 35:  0](R/W/H) Packet count, can be written by software to any value.  If a value of 0xFFFFFFFFF is
                                                                 written to this field, it will cause this field as well as SDP()_R()_IN_BYTE_CNT to
                                                                 clear. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_in_pkt_cnt_s cn; */
};
typedef union cavm_sdpx_rx_in_pkt_cnt cavm_sdpx_rx_in_pkt_cnt_t;

static inline uint64_t CAVM_SDPX_RX_IN_PKT_CNT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_IN_PKT_CNT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010080ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_IN_PKT_CNT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_IN_PKT_CNT(a,b) cavm_sdpx_rx_in_pkt_cnt_t
#define bustype_CAVM_SDPX_RX_IN_PKT_CNT(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_IN_PKT_CNT(a,b) "SDPX_RX_IN_PKT_CNT"
#define device_bar_CAVM_SDPX_RX_IN_PKT_CNT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_IN_PKT_CNT(a,b) (a)
#define arguments_CAVM_SDPX_RX_IN_PKT_CNT(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_mbox_ism
 *
 * SDP Ring Mailbox Interrupt Status Message Register
 * This register configures mailbox status messages.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_mbox_ism
{
    uint64_t u;
    struct cavm_sdpx_rx_mbox_ism_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_53_63        : 11;
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) PCI address hardware will write status messages to. See [EN]. */
        uint64_t msix_dis              : 1;  /**< [  1:  1](R/W) Disables MSIX interrupt and setting PBA bits for MBOX interrupts
                                                                 when only using ISM messages for completion status. */
        uint64_t en                    : 1;  /**< [  0:  0](R/W) Enable writing mailbox pending messages. When set, before sending a mailbox
                                                                 interrupt MSI to the host (from SDP()_R()_MBOX_PF_VF_INT[INTR] being set),
                                                                 hardware will write a one to the 32-bits at [ADDR]. This does not cause an
                                                                 interrupt; instead software may use this inside the standard mailbox-initiated
                                                                 interrupt handler to appropriately handle the interrupt. */
#else /* Word 0 - Little Endian */
        uint64_t en                    : 1;  /**< [  0:  0](R/W) Enable writing mailbox pending messages. When set, before sending a mailbox
                                                                 interrupt MSI to the host (from SDP()_R()_MBOX_PF_VF_INT[INTR] being set),
                                                                 hardware will write a one to the 32-bits at [ADDR]. This does not cause an
                                                                 interrupt; instead software may use this inside the standard mailbox-initiated
                                                                 interrupt handler to appropriately handle the interrupt. */
        uint64_t msix_dis              : 1;  /**< [  1:  1](R/W) Disables MSIX interrupt and setting PBA bits for MBOX interrupts
                                                                 when only using ISM messages for completion status. */
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) PCI address hardware will write status messages to. See [EN]. */
        uint64_t reserved_53_63        : 11;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_mbox_ism_s cn; */
};
typedef union cavm_sdpx_rx_mbox_ism cavm_sdpx_rx_mbox_ism_t;

static inline uint64_t CAVM_SDPX_RX_MBOX_ISM(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_MBOX_ISM(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010500ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_MBOX_ISM", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_MBOX_ISM(a,b) cavm_sdpx_rx_mbox_ism_t
#define bustype_CAVM_SDPX_RX_MBOX_ISM(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_MBOX_ISM(a,b) "SDPX_RX_MBOX_ISM"
#define device_bar_CAVM_SDPX_RX_MBOX_ISM(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_MBOX_ISM(a,b) (a)
#define arguments_CAVM_SDPX_RX_MBOX_ISM(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_mbox_pf_vf_data
 *
 * SDP PF to VF Mailbox Data Registers
 * These registers are used for communication of data from the PF to VF.
 * A write to SDP()_MBOX_PF_VF_DATA() from the PF will also write to
 * SDP()_R()_MBOX_PF_VF_DATA[DATA] and cause the corresponding bit in
 * SDP()_R()_MBOX_PF_VF_INT[INTR] to be set, along with other bits in
 * SDP()_R()_OUT_CNTS[MBOX_INT] and SDP()_R()_IN_CNTS[MBOX_INT].
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_mbox_pf_vf_data
{
    uint64_t u;
    struct cavm_sdpx_rx_mbox_pf_vf_data_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W) Communication data from PF to VF. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W) Communication data from PF to VF. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_mbox_pf_vf_data_s cn; */
};
typedef union cavm_sdpx_rx_mbox_pf_vf_data cavm_sdpx_rx_mbox_pf_vf_data_t;

static inline uint64_t CAVM_SDPX_RX_MBOX_PF_VF_DATA(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_MBOX_PF_VF_DATA(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010210ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_MBOX_PF_VF_DATA", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_MBOX_PF_VF_DATA(a,b) cavm_sdpx_rx_mbox_pf_vf_data_t
#define bustype_CAVM_SDPX_RX_MBOX_PF_VF_DATA(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_MBOX_PF_VF_DATA(a,b) "SDPX_RX_MBOX_PF_VF_DATA"
#define device_bar_CAVM_SDPX_RX_MBOX_PF_VF_DATA(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_MBOX_PF_VF_DATA(a,b) (a)
#define arguments_CAVM_SDPX_RX_MBOX_PF_VF_DATA(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_mbox_pf_vf_int
 *
 * SDP Packet PF to VF Mailbox Interrupt Register
 * These registers contain interrupt status and enable for the PF to VF mailbox communication
 * registers. A write to SDP()_MBOX_VF_PF_DATA() from the PF will cause the [INTR] bit
 * in this register to set, along with corresponding bits in
 * SDP()_R()_OUT_CNTS[MBOX_INT] and SDP()_R()_IN_CNTS[MBOX_INT].
 * All of these bits are cleared by writing 1 to the [INTR] bit in this register.
 * If the [ENAB] bit is set, then an MSI-X interrupt will also be generated when the [INTR] bit
 * is set. This register is cleared also due to an FLR.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_mbox_pf_vf_int
{
    uint64_t u;
    struct cavm_sdpx_rx_mbox_pf_vf_int_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t enab                  : 1;  /**< [  1:  1](R/W) PF to VF mailbox interrupt enable. */
        uint64_t intr                  : 1;  /**< [  0:  0](R/W1C/H) PF to VF mailbox interrupt signal. */
#else /* Word 0 - Little Endian */
        uint64_t intr                  : 1;  /**< [  0:  0](R/W1C/H) PF to VF mailbox interrupt signal. */
        uint64_t enab                  : 1;  /**< [  1:  1](R/W) PF to VF mailbox interrupt enable. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_mbox_pf_vf_int_s cn; */
};
typedef union cavm_sdpx_rx_mbox_pf_vf_int cavm_sdpx_rx_mbox_pf_vf_int_t;

static inline uint64_t CAVM_SDPX_RX_MBOX_PF_VF_INT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_MBOX_PF_VF_INT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010220ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_MBOX_PF_VF_INT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_MBOX_PF_VF_INT(a,b) cavm_sdpx_rx_mbox_pf_vf_int_t
#define bustype_CAVM_SDPX_RX_MBOX_PF_VF_INT(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_MBOX_PF_VF_INT(a,b) "SDPX_RX_MBOX_PF_VF_INT"
#define device_bar_CAVM_SDPX_RX_MBOX_PF_VF_INT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_MBOX_PF_VF_INT(a,b) (a)
#define arguments_CAVM_SDPX_RX_MBOX_PF_VF_INT(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_mbox_vf_pf_data
 *
 * SDP VF to PF Mailbox Data Registers
 * These registers are used for communication of data from the VF to PF.
 * A write by the VF to this register will also write to SDP()_MBOX_VF_PF_DATA()
 * and cause the corresponding bit in  SDP()_EPF()_MBOX_RINT() to be set to be set,
 * and an MSI-X message to be generated.
 * To clear the interrupt condition, the PF should write a 1 to SDP()_R()_MBOX_PF_VF_INT[INTR].
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_mbox_vf_pf_data
{
    uint64_t u;
    struct cavm_sdpx_rx_mbox_vf_pf_data_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W) Communication data from VF to PF. */
#else /* Word 0 - Little Endian */
        uint64_t data                  : 64; /**< [ 63:  0](R/W) Communication data from VF to PF. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_mbox_vf_pf_data_s cn; */
};
typedef union cavm_sdpx_rx_mbox_vf_pf_data cavm_sdpx_rx_mbox_vf_pf_data_t;

static inline uint64_t CAVM_SDPX_RX_MBOX_VF_PF_DATA(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_MBOX_VF_PF_DATA(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010230ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_MBOX_VF_PF_DATA", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_MBOX_VF_PF_DATA(a,b) cavm_sdpx_rx_mbox_vf_pf_data_t
#define bustype_CAVM_SDPX_RX_MBOX_VF_PF_DATA(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_MBOX_VF_PF_DATA(a,b) "SDPX_RX_MBOX_VF_PF_DATA"
#define device_bar_CAVM_SDPX_RX_MBOX_VF_PF_DATA(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_MBOX_VF_PF_DATA(a,b) (a)
#define arguments_CAVM_SDPX_RX_MBOX_VF_PF_DATA(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_byte_cnt
 *
 * SDP Packet Output Byte Count Register
 * This register contains byte counts per ring that have been written to memory by SDP.
 * The counter will wrap when it reaches its maximum value.  It should be cleared
 * before the ring is enabled for an accurate count.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_out_byte_cnt
{
    uint64_t u;
    struct cavm_sdpx_rx_out_byte_cnt_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_48_63        : 16;
        uint64_t cnt                   : 48; /**< [ 47:  0](R/W/H) Byte count, can be reset by software by writing SDP()_R()_OUT_PKT_CNT[CNT]
                                                                 with 0xFFFFFFFFF. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 48; /**< [ 47:  0](R/W/H) Byte count, can be reset by software by writing SDP()_R()_OUT_PKT_CNT[CNT]
                                                                 with 0xFFFFFFFFF. */
        uint64_t reserved_48_63        : 16;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_byte_cnt_s cn; */
};
typedef union cavm_sdpx_rx_out_byte_cnt cavm_sdpx_rx_out_byte_cnt_t;

static inline uint64_t CAVM_SDPX_RX_OUT_BYTE_CNT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_BYTE_CNT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010190ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_BYTE_CNT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_BYTE_CNT(a,b) cavm_sdpx_rx_out_byte_cnt_t
#define bustype_CAVM_SDPX_RX_OUT_BYTE_CNT(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_BYTE_CNT(a,b) "SDPX_RX_OUT_BYTE_CNT"
#define device_bar_CAVM_SDPX_RX_OUT_BYTE_CNT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_BYTE_CNT(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_BYTE_CNT(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_cnts
 *
 * SDP Packet Output Counts Register
 * This register contains the counters for SDP output ports.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the VF prior to enabling the ring.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_out_cnts
{
    uint64_t u;
    struct cavm_sdpx_rx_out_cnts_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t send_ism              : 1;  /**< [ 63: 63](WO/H) A write of 1 will cause a ISM message to be sent with the current value of [CNT].
                                                                 Hardware will ignore the written value of [CNT] if [SEND_ISM] is set and will NOT
                                                                 subtract the amount written to [CNT]. ISM messages must be enabled in
                                                                 SDP()_R()_OUT_CNTS_ISM[EN].
                                                                 Note this is orthogonal to normal ISM messages that will always be sent before a counts
                                                                 interrupt if enabled. */
        uint64_t out_int               : 1;  /**< [ 62: 62](RO/H) Returns a 1 when:
                                                                  * SDP()_R()_OUT_CNTS[CNT] \> SDP()_R()_OUT_INT_LEVELS[CNT] &
                                                                    SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_OUT_CNTS[TIMER] \> SDP()_R()_OUT_INT_LEVELS[TIMET] &
                                                                    SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_OUT_INT_MDRT_CTL1[ENABLE]=1 &
                                                                    SDP()_R()_OUT_CNTS[TIMER] has reached an interrupt threshold based
                                                                    on the values in the SDP()_R()_OUT_INT_MDRT_CTL* registers.
                                                                 To clear the bit, the SDP()_R()_OUT_CNTS register must be written to clear the
                                                                 underlying condition.

                                                                 Internal:
                                                                 These interrupt bits are not cleared due to FLR becase the CNTS and
                                                                 LEVELS registers are not reset and we wish to make the interrupt state
                                                                 consistent with CNTS/LEVELS even after FLR. The CNTS register must be
                                                                 cleared by software as part of initialization after a reset (including FLR)
                                                                 which will cause the interrupt state to clear. */
        uint64_t in_int                : 1;  /**< [ 61: 61](RO/H) Returns a 1 when:
                                                                  * SDP()_R()_IN_CNTS[CNT] \> SDP()_R()_IN_INT_LEVELS[CNT] &
                                                                    SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_IN_CNTS[TIMER] \> SDP()_R()_IN_INT_LEVELS[TIMET] &
                                                                    SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_IN_INT_MDRT_CTL1[ENABLE]=1 &
                                                                    SDP()_R()_IN_CNTS[TIMER] has reached an interrupt threshold based
                                                                    on the values in the SDP()_R()_IN_INT_MDRT_CTL* registers.
                                                                 To clear the bit, the SDP()_R()_IN_CNTS register must be written to clear the
                                                                 underlying condition.

                                                                 Internal:
                                                                 These interrupt bits are not cleared due to FLR becase the CNTS and
                                                                 LEVELS registers are not reset and we wish to make the interrupt state
                                                                 consistent with CNTS/LEVELS even after FLR. The CNTS register must be
                                                                 cleared by software as part of initialization after a reset (including FLR)
                                                                 which will cause the interrupt state to clear. */
        uint64_t mbox_int              : 1;  /**< [ 60: 60](RO/H) Returns a 1 when:
                                                                  * SDP()_R()_MBOX_PF_VF_INT[INTR] is set.

                                                                 To clear the bit, write SDP()_R()_MBOX_PF_VF_INT[INTR] with 1.
                                                                 This bit is also cleared due to an FLR. */
        uint64_t resend                : 1;  /**< [ 59: 59](WO/H) A write of 1 will resend an MSI-X interrupt message and resend an ISM
                                                                 message when SDP()_R()_IN_CNTS_ISM.EN is set,
                                                                 if any of the following conditions are true for the respective ring R():
                                                                  * SDP()_R()_OUT_CNTS[CNT]   \> SDP()_R()_OUT_INT_LEVELS[CNT] &
                                                                    SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * SDP()_R()_OUT_CNTS[TIMER] \> SDP()_R()_OUT_INT_LEVELS[TIMET] &
                                                                    SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]==1.

                                                                 If SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]=0 a write of
                                                                 [RESEND] will clear [OUT_INT].  This is generally used when
                                                                 SDP()_R()_OUT_INT_MDRT_CTL1[ENABLE]==1 to prepare for the next MDRT
                                                                 timer interrupt event.

                                                                 If [SEND_ISM] is also written to a 1 writes of 1 to the [RESEND]
                                                                 bit will be ignored. */
        uint64_t reserved_54_58        : 5;
        uint64_t timer                 : 22; /**< [ 53: 32](RO/H) Timer, incremented every 1024 coprocessor-clock cycles when [CNT] is
                                                                 not zero. The hardware clears [TIMER] when [CNT] is written to a
                                                                 nonzero value. The first increment of this count can occur between 0 to
                                                                 1023 coprocessor-clock cycles after [CNT] becomes nonzero. */
        uint64_t cnt                   : 32; /**< [ 31:  0](R/W/H) Packet counter. Hardware adds to [CNT] as it sends packets out. On a write
                                                                 to this CSR with [SEND_ISM] set to 0, hardware subtracts the amount written
                                                                 to the [CNT] field from [CNT], which will clear SDP()_R()_OUT_CNTS[OUT_INT] if ([CNT]
                                                                 becomes \<= SDP()_R()_OUT_INT_LEVELS[CNT] and SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]
                                                                 is set). When SDP()_R()_OUT_INT_LEVELS[BMODE] is clear,
                                                                 the hardware adds 1 to [CNT] per packet. When SDP()_R()_OUT_INT_LEVELS[BMODE] is
                                                                 set, the hardware adds the packet length to [CNT] per packet. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 32; /**< [ 31:  0](R/W/H) Packet counter. Hardware adds to [CNT] as it sends packets out. On a write
                                                                 to this CSR with [SEND_ISM] set to 0, hardware subtracts the amount written
                                                                 to the [CNT] field from [CNT], which will clear SDP()_R()_OUT_CNTS[OUT_INT] if ([CNT]
                                                                 becomes \<= SDP()_R()_OUT_INT_LEVELS[CNT] and SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]
                                                                 is set). When SDP()_R()_OUT_INT_LEVELS[BMODE] is clear,
                                                                 the hardware adds 1 to [CNT] per packet. When SDP()_R()_OUT_INT_LEVELS[BMODE] is
                                                                 set, the hardware adds the packet length to [CNT] per packet. */
        uint64_t timer                 : 22; /**< [ 53: 32](RO/H) Timer, incremented every 1024 coprocessor-clock cycles when [CNT] is
                                                                 not zero. The hardware clears [TIMER] when [CNT] is written to a
                                                                 nonzero value. The first increment of this count can occur between 0 to
                                                                 1023 coprocessor-clock cycles after [CNT] becomes nonzero. */
        uint64_t reserved_54_58        : 5;
        uint64_t resend                : 1;  /**< [ 59: 59](WO/H) A write of 1 will resend an MSI-X interrupt message and resend an ISM
                                                                 message when SDP()_R()_IN_CNTS_ISM.EN is set,
                                                                 if any of the following conditions are true for the respective ring R():
                                                                  * SDP()_R()_OUT_CNTS[CNT]   \> SDP()_R()_OUT_INT_LEVELS[CNT] &
                                                                    SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * SDP()_R()_OUT_CNTS[TIMER] \> SDP()_R()_OUT_INT_LEVELS[TIMET] &
                                                                    SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]==1.

                                                                 If SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]=0 a write of
                                                                 [RESEND] will clear [OUT_INT].  This is generally used when
                                                                 SDP()_R()_OUT_INT_MDRT_CTL1[ENABLE]==1 to prepare for the next MDRT
                                                                 timer interrupt event.

                                                                 If [SEND_ISM] is also written to a 1 writes of 1 to the [RESEND]
                                                                 bit will be ignored. */
        uint64_t mbox_int              : 1;  /**< [ 60: 60](RO/H) Returns a 1 when:
                                                                  * SDP()_R()_MBOX_PF_VF_INT[INTR] is set.

                                                                 To clear the bit, write SDP()_R()_MBOX_PF_VF_INT[INTR] with 1.
                                                                 This bit is also cleared due to an FLR. */
        uint64_t in_int                : 1;  /**< [ 61: 61](RO/H) Returns a 1 when:
                                                                  * SDP()_R()_IN_CNTS[CNT] \> SDP()_R()_IN_INT_LEVELS[CNT] &
                                                                    SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_IN_CNTS[TIMER] \> SDP()_R()_IN_INT_LEVELS[TIMET] &
                                                                    SDP()_R()_IN_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_IN_INT_MDRT_CTL1[ENABLE]=1 &
                                                                    SDP()_R()_IN_CNTS[TIMER] has reached an interrupt threshold based
                                                                    on the values in the SDP()_R()_IN_INT_MDRT_CTL* registers.
                                                                 To clear the bit, the SDP()_R()_IN_CNTS register must be written to clear the
                                                                 underlying condition.

                                                                 Internal:
                                                                 These interrupt bits are not cleared due to FLR becase the CNTS and
                                                                 LEVELS registers are not reset and we wish to make the interrupt state
                                                                 consistent with CNTS/LEVELS even after FLR. The CNTS register must be
                                                                 cleared by software as part of initialization after a reset (including FLR)
                                                                 which will cause the interrupt state to clear. */
        uint64_t out_int               : 1;  /**< [ 62: 62](RO/H) Returns a 1 when:
                                                                  * SDP()_R()_OUT_CNTS[CNT] \> SDP()_R()_OUT_INT_LEVELS[CNT] &
                                                                    SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_OUT_CNTS[TIMER] \> SDP()_R()_OUT_INT_LEVELS[TIMET] &
                                                                    SDP()_R()_OUT_INT_LEVELS[TIME_CNT_ENA]==1.
                                                                  * Or, SDP()_R()_OUT_INT_MDRT_CTL1[ENABLE]=1 &
                                                                    SDP()_R()_OUT_CNTS[TIMER] has reached an interrupt threshold based
                                                                    on the values in the SDP()_R()_OUT_INT_MDRT_CTL* registers.
                                                                 To clear the bit, the SDP()_R()_OUT_CNTS register must be written to clear the
                                                                 underlying condition.

                                                                 Internal:
                                                                 These interrupt bits are not cleared due to FLR becase the CNTS and
                                                                 LEVELS registers are not reset and we wish to make the interrupt state
                                                                 consistent with CNTS/LEVELS even after FLR. The CNTS register must be
                                                                 cleared by software as part of initialization after a reset (including FLR)
                                                                 which will cause the interrupt state to clear. */
        uint64_t send_ism              : 1;  /**< [ 63: 63](WO/H) A write of 1 will cause a ISM message to be sent with the current value of [CNT].
                                                                 Hardware will ignore the written value of [CNT] if [SEND_ISM] is set and will NOT
                                                                 subtract the amount written to [CNT]. ISM messages must be enabled in
                                                                 SDP()_R()_OUT_CNTS_ISM[EN].
                                                                 Note this is orthogonal to normal ISM messages that will always be sent before a counts
                                                                 interrupt if enabled. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_cnts_s cn; */
};
typedef union cavm_sdpx_rx_out_cnts cavm_sdpx_rx_out_cnts_t;

static inline uint64_t CAVM_SDPX_RX_OUT_CNTS(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_CNTS(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010100ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_CNTS", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_CNTS(a,b) cavm_sdpx_rx_out_cnts_t
#define bustype_CAVM_SDPX_RX_OUT_CNTS(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_CNTS(a,b) "SDPX_RX_OUT_CNTS"
#define device_bar_CAVM_SDPX_RX_OUT_CNTS(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_CNTS(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_CNTS(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_cnts_ism
 *
 * SDP Ring Output Counts Status Message Register
 * This register configures output count status messages.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_out_cnts_ism
{
    uint64_t u;
    struct cavm_sdpx_rx_out_cnts_ism_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_53_63        : 11;
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) PCI address hardware will write status messages to. See [EN]. */
        uint64_t msix_dis              : 1;  /**< [  1:  1](R/W) Disables MSIX interrupt and setting PBA bits for output packet interrupts
                                                                 when only using ISM messages for completion status. */
        uint64_t en                    : 1;  /**< [  0:  0](R/W) Enable writing output counts messages. When set, before sending an output counts
                                                                 interrupt MSI to the host, hardware will write the value of value of
                                                                 SDP()_R()_OUT_CNTS[CNT] to the 32-bits at [ADDR]. This does not cause an
                                                                 interrupt; instead software may use this inside the standard
                                                                 output-counts-initiated interrupt handler to appropriately handle the interrupt. */
#else /* Word 0 - Little Endian */
        uint64_t en                    : 1;  /**< [  0:  0](R/W) Enable writing output counts messages. When set, before sending an output counts
                                                                 interrupt MSI to the host, hardware will write the value of value of
                                                                 SDP()_R()_OUT_CNTS[CNT] to the 32-bits at [ADDR]. This does not cause an
                                                                 interrupt; instead software may use this inside the standard
                                                                 output-counts-initiated interrupt handler to appropriately handle the interrupt. */
        uint64_t msix_dis              : 1;  /**< [  1:  1](R/W) Disables MSIX interrupt and setting PBA bits for output packet interrupts
                                                                 when only using ISM messages for completion status. */
        uint64_t addr                  : 51; /**< [ 52:  2](R/W) PCI address hardware will write status messages to. See [EN]. */
        uint64_t reserved_53_63        : 11;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_cnts_ism_s cn; */
};
typedef union cavm_sdpx_rx_out_cnts_ism cavm_sdpx_rx_out_cnts_ism_t;

static inline uint64_t CAVM_SDPX_RX_OUT_CNTS_ISM(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_CNTS_ISM(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010510ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_CNTS_ISM", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_CNTS_ISM(a,b) cavm_sdpx_rx_out_cnts_ism_t
#define bustype_CAVM_SDPX_RX_OUT_CNTS_ISM(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_CNTS_ISM(a,b) "SDPX_RX_OUT_CNTS_ISM"
#define device_bar_CAVM_SDPX_RX_OUT_CNTS_ISM(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_CNTS_ISM(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_CNTS_ISM(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_control
 *
 * SDP Packet Output Control Register
 * This register contains control bits for output packet rings.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the VF prior to enabling the ring.  Also, this register cannot be written
 * while either of the following conditions is true:
 *   * SDP()_R()_OUT_CONTROL[IDLE] is clear.
 *   * Or, SDP()_R()_OUT_ENABLE[ENB] is set.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_out_control
{
    uint64_t u;
    struct cavm_sdpx_rx_out_control_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_41_63        : 23;
        uint64_t idle                  : 1;  /**< [ 40: 40](RO/H) Asserted when this ring has no packets in-flight. If SDP(0)_OUT_MCAST_CTL.IDLE is also 1 there are
                                                                 also no multicast packets that are destined for this ring in flight. */
        uint64_t drop_cnt              : 4;  /**< [ 39: 36](R/W) Minimum number of buffers needed to send an outbound packet.
                                                                 This value is in multiples of 16 and should be greater than 0.
                                                                 If a packet is received by SDP Output from NIXTX and the
                                                                 SDP()_R()_OUT_SLIST_DBELL[DBELL] \< [DROP_CNT], SDP will set
                                                                 SDP()_OUT_DROP_STATE()[STATE] to 1 and drop this packet and future packets
                                                                 until SDP()_OUT_DROP_STATE()[STATE] goes to a 0.
                                                                 [DROP_CNT] * 16 should be set to a value less than SDP()_OUT_WMARK[WMARK].
                                                                 If SDP()_OUT_BP_EN()_W1C[ENB] is set to 0 (not enabled) for this ring, then
                                                                 ([DROP_CNT] * 16) \> (Largest Packet Expected) / SDP()_R()_OUT_CONTROL[BSIZE].
                                                                 Note if this field is set to 0 SDP will treat it as if it was written to 1. */
        uint64_t es_i                  : 2;  /**< [ 35: 34](R/W) [ES_I] is ES\<1:0\> for info buffer write operations to buffer/info
                                                                 pair MAC memory space addresses fetched from packet output ring. ES\<1:0\> is the
                                                                 endian-swap attribute for these MAC memory space writes.

                                                                 Enumerated by DPI_ENDIANSWAP_E. */
        uint64_t nsr_i                 : 1;  /**< [ 33: 33](R/W) [NSR_I] is ADDRTYPE\<1\> for info buffer write operations to buffer/info
                                                                 pair MAC memory space addresses fetched from packet output ring. ADDRTYPE\<1\> is
                                                                 the no-snoop attribute for PCIe. */
        uint64_t ror_i                 : 1;  /**< [ 32: 32](R/W) [ROR_I] is ADDRTYPE\<0\> for info buffer write operations to buffer/info
                                                                 pair MAC memory space addresses fetched from packet output ring. ADDRTYPE\<0\> is
                                                                 the relaxed-order attribute for PCIe. */
        uint64_t es_d                  : 2;  /**< [ 31: 30](R/W) [ES_D] is ES\<1:0\> for data buffer write operations to buffer/info
                                                                 pair MAC memory space addresses fetched from packet output ring. ES\<1:0\> is the
                                                                 endian-swap attribute for these MAC memory space writes.

                                                                 Enumerated by DPI_ENDIANSWAP_E. */
        uint64_t nsr_d                 : 1;  /**< [ 29: 29](R/W) [NSR_D] is ADDRTYPE\<1\> for data buffer write operations to buffer/info
                                                                 pair MAC memory space addresses fetched from packet output ring. ADDRTYPE\<1\> is
                                                                 the no-snoop attribute for PCIe. */
        uint64_t ror_d                 : 1;  /**< [ 28: 28](R/W) [ROR_D] is ADDRTYPE\<0\> for data buffer write operations to buffer/info
                                                                 pair MAC memory space addresses fetched from packet output ring. ADDRTYPE\<0\> is
                                                                 the relaxed-order attribute for PCIe. */
        uint64_t es_p                  : 2;  /**< [ 27: 26](R/W) [ES_P] is ES\<1:0\> for the packet output ring reads that fetch buffer/info pointer pairs
                                                                 (from SDP()_R()_OUT_SLIST_BADDR[ADDR]+). ES\<1:0\> is the endian-swap attribute for these
                                                                 MAC memory space reads.

                                                                 Enumerated by DPI_ENDIANSWAP_E. */
        uint64_t nsr_p                 : 1;  /**< [ 25: 25](R/W) [NSR_P] is ADDRTYPE\<1\> for the packet output ring reads that fetch buffer/info pointer
                                                                 pairs (from SDP()_R()_OUT_SLIST_BADDR[ADDR]+). ADDRTYPE\<1\> is the no-snoop attribute for PCIe. */
        uint64_t ror_p                 : 1;  /**< [ 24: 24](R/W) [ROR_P] is ADDRTYPE\<0\> for the packet output ring reads that fetch buffer/info pointer
                                                                 pairs (from SDP()_R()_OUT_SLIST_BADDR[ADDR]+). ADDRTYPE\<0\> is the relaxed-order attribute
                                                                 for PCIe. */
        uint64_t imode                 : 1;  /**< [ 23: 23](R/W) When IMODE=1, packet output ring is in info-pointer mode; otherwise the packet output ring
                                                                 is in buffer-pointer-only mode. */
        uint64_t isize                 : 7;  /**< [ 22: 16](R/W/H) Info bytes size (bytes) for the output port. Legal sizes are 0 to 120. Not used
                                                                 in buffer-pointer-only mode.  If a value is written that is between 120-127 then
                                                                 a value of 120 will be forced by hardware. */
        uint64_t bsize                 : 16; /**< [ 15:  0](R/W/H) Buffer size (bytes) for the output ring.  The minimum size is 128 bytes; if a value
                                                                 smaller than 128 is written, hardware will force a value of 128. */
#else /* Word 0 - Little Endian */
        uint64_t bsize                 : 16; /**< [ 15:  0](R/W/H) Buffer size (bytes) for the output ring.  The minimum size is 128 bytes; if a value
                                                                 smaller than 128 is written, hardware will force a value of 128. */
        uint64_t isize                 : 7;  /**< [ 22: 16](R/W/H) Info bytes size (bytes) for the output port. Legal sizes are 0 to 120. Not used
                                                                 in buffer-pointer-only mode.  If a value is written that is between 120-127 then
                                                                 a value of 120 will be forced by hardware. */
        uint64_t imode                 : 1;  /**< [ 23: 23](R/W) When IMODE=1, packet output ring is in info-pointer mode; otherwise the packet output ring
                                                                 is in buffer-pointer-only mode. */
        uint64_t ror_p                 : 1;  /**< [ 24: 24](R/W) [ROR_P] is ADDRTYPE\<0\> for the packet output ring reads that fetch buffer/info pointer
                                                                 pairs (from SDP()_R()_OUT_SLIST_BADDR[ADDR]+). ADDRTYPE\<0\> is the relaxed-order attribute
                                                                 for PCIe. */
        uint64_t nsr_p                 : 1;  /**< [ 25: 25](R/W) [NSR_P] is ADDRTYPE\<1\> for the packet output ring reads that fetch buffer/info pointer
                                                                 pairs (from SDP()_R()_OUT_SLIST_BADDR[ADDR]+). ADDRTYPE\<1\> is the no-snoop attribute for PCIe. */
        uint64_t es_p                  : 2;  /**< [ 27: 26](R/W) [ES_P] is ES\<1:0\> for the packet output ring reads that fetch buffer/info pointer pairs
                                                                 (from SDP()_R()_OUT_SLIST_BADDR[ADDR]+). ES\<1:0\> is the endian-swap attribute for these
                                                                 MAC memory space reads.

                                                                 Enumerated by DPI_ENDIANSWAP_E. */
        uint64_t ror_d                 : 1;  /**< [ 28: 28](R/W) [ROR_D] is ADDRTYPE\<0\> for data buffer write operations to buffer/info
                                                                 pair MAC memory space addresses fetched from packet output ring. ADDRTYPE\<0\> is
                                                                 the relaxed-order attribute for PCIe. */
        uint64_t nsr_d                 : 1;  /**< [ 29: 29](R/W) [NSR_D] is ADDRTYPE\<1\> for data buffer write operations to buffer/info
                                                                 pair MAC memory space addresses fetched from packet output ring. ADDRTYPE\<1\> is
                                                                 the no-snoop attribute for PCIe. */
        uint64_t es_d                  : 2;  /**< [ 31: 30](R/W) [ES_D] is ES\<1:0\> for data buffer write operations to buffer/info
                                                                 pair MAC memory space addresses fetched from packet output ring. ES\<1:0\> is the
                                                                 endian-swap attribute for these MAC memory space writes.

                                                                 Enumerated by DPI_ENDIANSWAP_E. */
        uint64_t ror_i                 : 1;  /**< [ 32: 32](R/W) [ROR_I] is ADDRTYPE\<0\> for info buffer write operations to buffer/info
                                                                 pair MAC memory space addresses fetched from packet output ring. ADDRTYPE\<0\> is
                                                                 the relaxed-order attribute for PCIe. */
        uint64_t nsr_i                 : 1;  /**< [ 33: 33](R/W) [NSR_I] is ADDRTYPE\<1\> for info buffer write operations to buffer/info
                                                                 pair MAC memory space addresses fetched from packet output ring. ADDRTYPE\<1\> is
                                                                 the no-snoop attribute for PCIe. */
        uint64_t es_i                  : 2;  /**< [ 35: 34](R/W) [ES_I] is ES\<1:0\> for info buffer write operations to buffer/info
                                                                 pair MAC memory space addresses fetched from packet output ring. ES\<1:0\> is the
                                                                 endian-swap attribute for these MAC memory space writes.

                                                                 Enumerated by DPI_ENDIANSWAP_E. */
        uint64_t drop_cnt              : 4;  /**< [ 39: 36](R/W) Minimum number of buffers needed to send an outbound packet.
                                                                 This value is in multiples of 16 and should be greater than 0.
                                                                 If a packet is received by SDP Output from NIXTX and the
                                                                 SDP()_R()_OUT_SLIST_DBELL[DBELL] \< [DROP_CNT], SDP will set
                                                                 SDP()_OUT_DROP_STATE()[STATE] to 1 and drop this packet and future packets
                                                                 until SDP()_OUT_DROP_STATE()[STATE] goes to a 0.
                                                                 [DROP_CNT] * 16 should be set to a value less than SDP()_OUT_WMARK[WMARK].
                                                                 If SDP()_OUT_BP_EN()_W1C[ENB] is set to 0 (not enabled) for this ring, then
                                                                 ([DROP_CNT] * 16) \> (Largest Packet Expected) / SDP()_R()_OUT_CONTROL[BSIZE].
                                                                 Note if this field is set to 0 SDP will treat it as if it was written to 1. */
        uint64_t idle                  : 1;  /**< [ 40: 40](RO/H) Asserted when this ring has no packets in-flight. If SDP(0)_OUT_MCAST_CTL.IDLE is also 1 there are
                                                                 also no multicast packets that are destined for this ring in flight. */
        uint64_t reserved_41_63        : 23;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_control_s cn; */
};
typedef union cavm_sdpx_rx_out_control cavm_sdpx_rx_out_control_t;

static inline uint64_t CAVM_SDPX_RX_OUT_CONTROL(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_CONTROL(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010150ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_CONTROL", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_CONTROL(a,b) cavm_sdpx_rx_out_control_t
#define bustype_CAVM_SDPX_RX_OUT_CONTROL(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_CONTROL(a,b) "SDPX_RX_OUT_CONTROL"
#define device_bar_CAVM_SDPX_RX_OUT_CONTROL(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_CONTROL(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_CONTROL(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_enable
 *
 * SDP Packet Output Enable Register
 * This register is the enable for the output pointer rings.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_out_enable
{
    uint64_t u;
    struct cavm_sdpx_rx_out_enable_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t enb                   : 1;  /**< [  0:  0](R/W/H) Enable for the output ring i. This bit can be cleared by hardware if certain
                                                                 errors occur or an FLR is indicated by the remote host. It can be cleared by
                                                                 software at any time. It cannot be set unless SDP()_R()_OUT_CONTROL[IDLE] == 0. */
#else /* Word 0 - Little Endian */
        uint64_t enb                   : 1;  /**< [  0:  0](R/W/H) Enable for the output ring i. This bit can be cleared by hardware if certain
                                                                 errors occur or an FLR is indicated by the remote host. It can be cleared by
                                                                 software at any time. It cannot be set unless SDP()_R()_OUT_CONTROL[IDLE] == 0. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_enable_s cn; */
};
typedef union cavm_sdpx_rx_out_enable cavm_sdpx_rx_out_enable_t;

static inline uint64_t CAVM_SDPX_RX_OUT_ENABLE(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_ENABLE(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010160ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_ENABLE", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_ENABLE(a,b) cavm_sdpx_rx_out_enable_t
#define bustype_CAVM_SDPX_RX_OUT_ENABLE(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_ENABLE(a,b) "SDPX_RX_OUT_ENABLE"
#define device_bar_CAVM_SDPX_RX_OUT_ENABLE(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_ENABLE(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_ENABLE(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_int_levels
 *
 * SDP Packet Output Interrupt Levels Register
 * This register contains SDP output packet interrupt levels.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the VF prior to enabling the ring.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_out_int_levels
{
    uint64_t u;
    struct cavm_sdpx_rx_out_int_levels_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t bmode                 : 1;  /**< [ 63: 63](R/W) Determines whether SDP()_R()_OUT_CNTS[CNT] is a byte or packet counter. When
                                                                 [BMODE]=1, SDP()_R()_OUT_CNTS[CNT] is a byte counter, else SDP()_R()_OUT_CNTS[CNT]
                                                                 is a packet counter. */
        uint64_t time_cnt_ena          : 1;  /**< [ 62: 62](R/W) Enables generating MSIX-X interrupts whenever SDP()_R()_OUT_CNTS[TIMER] \> [TIMET]
                                                                 or SDP()_R()_OUT_CNTS[CNT] \> [CNT] conditions are met. */
        uint64_t max_len_ena           : 1;  /**< [ 61: 61](R/W) Maximum packet length enable. If set, An MSI-X interrupt will be generated for
                                                                 any packet with length greater than [MAX_LEN] KBytes. */
        uint64_t max_len               : 7;  /**< [ 60: 54](R/W) Maximum packet Length not to generate interrupt (in KB). IF [MAX_LEN_ENA] is set
                                                                 an MSI-X interrupt will be generated for any packet with length greater than
                                                                 [MAX_LEN] KBytes. */
        uint64_t timet                 : 22; /**< [ 53: 32](R/W) Output port counter time interrupt threshold. An MSI-X interrupt will be generated
                                                                 whenever SDP()_R()_OUT_CNTS[TIMER] \> [TIMET] and [TIME_CNT_ENA] is set.
                                                                 Whenever software changes the value of [TIMET], it should also subsequently write
                                                                 the corresponding SDP()_R()_OUT_CNTS CSR (with a value of zero if desired) to
                                                                 ensure that the hardware correspondingly updates SDP()_R()_OUT_CNTS[OUT_INT]. */
        uint64_t cnt                   : 32; /**< [ 31:  0](R/W) Output port counter interrupt threshold. An MSI-X interrupt will be generated
                                                                 whenever SDP()_R()_OUT_CNTS[CNT] \> [CNT] and [TIME_CNT_ENA] is set.
                                                                 Whenever software changes the value of [CNT], it should also subsequently write
                                                                 the corresponding SDP()_R()_OUT_CNTS CSR (with a value of zero if desired) to
                                                                 ensure that the hardware correspondingly updates SDP()_R()_OUT_CNTS[OUT_INT]. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 32; /**< [ 31:  0](R/W) Output port counter interrupt threshold. An MSI-X interrupt will be generated
                                                                 whenever SDP()_R()_OUT_CNTS[CNT] \> [CNT] and [TIME_CNT_ENA] is set.
                                                                 Whenever software changes the value of [CNT], it should also subsequently write
                                                                 the corresponding SDP()_R()_OUT_CNTS CSR (with a value of zero if desired) to
                                                                 ensure that the hardware correspondingly updates SDP()_R()_OUT_CNTS[OUT_INT]. */
        uint64_t timet                 : 22; /**< [ 53: 32](R/W) Output port counter time interrupt threshold. An MSI-X interrupt will be generated
                                                                 whenever SDP()_R()_OUT_CNTS[TIMER] \> [TIMET] and [TIME_CNT_ENA] is set.
                                                                 Whenever software changes the value of [TIMET], it should also subsequently write
                                                                 the corresponding SDP()_R()_OUT_CNTS CSR (with a value of zero if desired) to
                                                                 ensure that the hardware correspondingly updates SDP()_R()_OUT_CNTS[OUT_INT]. */
        uint64_t max_len               : 7;  /**< [ 60: 54](R/W) Maximum packet Length not to generate interrupt (in KB). IF [MAX_LEN_ENA] is set
                                                                 an MSI-X interrupt will be generated for any packet with length greater than
                                                                 [MAX_LEN] KBytes. */
        uint64_t max_len_ena           : 1;  /**< [ 61: 61](R/W) Maximum packet length enable. If set, An MSI-X interrupt will be generated for
                                                                 any packet with length greater than [MAX_LEN] KBytes. */
        uint64_t time_cnt_ena          : 1;  /**< [ 62: 62](R/W) Enables generating MSIX-X interrupts whenever SDP()_R()_OUT_CNTS[TIMER] \> [TIMET]
                                                                 or SDP()_R()_OUT_CNTS[CNT] \> [CNT] conditions are met. */
        uint64_t bmode                 : 1;  /**< [ 63: 63](R/W) Determines whether SDP()_R()_OUT_CNTS[CNT] is a byte or packet counter. When
                                                                 [BMODE]=1, SDP()_R()_OUT_CNTS[CNT] is a byte counter, else SDP()_R()_OUT_CNTS[CNT]
                                                                 is a packet counter. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_int_levels_s cn; */
};
typedef union cavm_sdpx_rx_out_int_levels cavm_sdpx_rx_out_int_levels_t;

static inline uint64_t CAVM_SDPX_RX_OUT_INT_LEVELS(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_INT_LEVELS(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010110ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_INT_LEVELS", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_INT_LEVELS(a,b) cavm_sdpx_rx_out_int_levels_t
#define bustype_CAVM_SDPX_RX_OUT_INT_LEVELS(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_INT_LEVELS(a,b) "SDPX_RX_OUT_INT_LEVELS"
#define device_bar_CAVM_SDPX_RX_OUT_INT_LEVELS(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_INT_LEVELS(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_INT_LEVELS(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_int_mdrt_ctl0
 *
 * SDP Output Instruction Interrupt Moderation Control 0 Register
 */
union cavm_sdpx_rx_out_int_mdrt_ctl0
{
    uint64_t u;
    struct cavm_sdpx_rx_out_int_mdrt_ctl0_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_54_63        : 10;
        uint64_t hi_timer              : 22; /**< [ 53: 32](R/W) High timer level. */
        uint64_t reserved_22_31        : 10;
        uint64_t lo_timer              : 22; /**< [ 21:  0](R/W) Low timer level. */
#else /* Word 0 - Little Endian */
        uint64_t lo_timer              : 22; /**< [ 21:  0](R/W) Low timer level. */
        uint64_t reserved_22_31        : 10;
        uint64_t hi_timer              : 22; /**< [ 53: 32](R/W) High timer level. */
        uint64_t reserved_54_63        : 10;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_int_mdrt_ctl0_s cn; */
};
typedef union cavm_sdpx_rx_out_int_mdrt_ctl0 cavm_sdpx_rx_out_int_mdrt_ctl0_t;

static inline uint64_t CAVM_SDPX_RX_OUT_INT_MDRT_CTL0(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_INT_MDRT_CTL0(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010380ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_INT_MDRT_CTL0", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_INT_MDRT_CTL0(a,b) cavm_sdpx_rx_out_int_mdrt_ctl0_t
#define bustype_CAVM_SDPX_RX_OUT_INT_MDRT_CTL0(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_INT_MDRT_CTL0(a,b) "SDPX_RX_OUT_INT_MDRT_CTL0"
#define device_bar_CAVM_SDPX_RX_OUT_INT_MDRT_CTL0(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_INT_MDRT_CTL0(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_INT_MDRT_CTL0(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_int_mdrt_ctl1
 *
 * SDP Output Instruction Interrupt Moderation Control 1 Register
 */
union cavm_sdpx_rx_out_int_mdrt_ctl1
{
    uint64_t u;
    struct cavm_sdpx_rx_out_int_mdrt_ctl1_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t enable                : 1;  /**< [ 63: 63](R/W) Enable interrupt moderation. */
        uint64_t reserved_44_62        : 19;
        uint64_t weight                : 4;  /**< [ 43: 40](R/W) Weight:
                                                                 0x0 = Weight of 1.
                                                                 0x1 = Weight of 2.
                                                                 0x2 = Weight of 4.
                                                                 0x3 = Weight of 8.
                                                                 0x4 = Weight of 16.
                                                                 0x5 = Weight of 32.
                                                                 0x6 = Weight of 64.
                                                                 0x7 = Weight of 128.
                                                                 0x8 = Weight of 256.
                                                                 _ else = Reserved.

                                                                 Internal:
                                                                 Reserved treated as 0x8. */
        uint64_t reserved_37_39        : 3;
        uint64_t hi_rate               : 17; /**< [ 36: 20](R/W) High byte rate. */
        uint64_t reserved_17_19        : 3;
        uint64_t lo_rate               : 17; /**< [ 16:  0](R/W) Low byte rate. */
#else /* Word 0 - Little Endian */
        uint64_t lo_rate               : 17; /**< [ 16:  0](R/W) Low byte rate. */
        uint64_t reserved_17_19        : 3;
        uint64_t hi_rate               : 17; /**< [ 36: 20](R/W) High byte rate. */
        uint64_t reserved_37_39        : 3;
        uint64_t weight                : 4;  /**< [ 43: 40](R/W) Weight:
                                                                 0x0 = Weight of 1.
                                                                 0x1 = Weight of 2.
                                                                 0x2 = Weight of 4.
                                                                 0x3 = Weight of 8.
                                                                 0x4 = Weight of 16.
                                                                 0x5 = Weight of 32.
                                                                 0x6 = Weight of 64.
                                                                 0x7 = Weight of 128.
                                                                 0x8 = Weight of 256.
                                                                 _ else = Reserved.

                                                                 Internal:
                                                                 Reserved treated as 0x8. */
        uint64_t reserved_44_62        : 19;
        uint64_t enable                : 1;  /**< [ 63: 63](R/W) Enable interrupt moderation. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_int_mdrt_ctl1_s cn; */
};
typedef union cavm_sdpx_rx_out_int_mdrt_ctl1 cavm_sdpx_rx_out_int_mdrt_ctl1_t;

static inline uint64_t CAVM_SDPX_RX_OUT_INT_MDRT_CTL1(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_INT_MDRT_CTL1(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e0800103a0ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_INT_MDRT_CTL1", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_INT_MDRT_CTL1(a,b) cavm_sdpx_rx_out_int_mdrt_ctl1_t
#define bustype_CAVM_SDPX_RX_OUT_INT_MDRT_CTL1(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_INT_MDRT_CTL1(a,b) "SDPX_RX_OUT_INT_MDRT_CTL1"
#define device_bar_CAVM_SDPX_RX_OUT_INT_MDRT_CTL1(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_INT_MDRT_CTL1(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_INT_MDRT_CTL1(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_int_mdrt_dbg
 *
 * SDP Output Instruction Interrupt Moderation Debug Register
 */
union cavm_sdpx_rx_out_int_mdrt_dbg
{
    uint64_t u;
    struct cavm_sdpx_rx_out_int_mdrt_dbg_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_17_63        : 47;
        uint64_t avg_rate              : 17; /**< [ 16:  0](R/W/H) Average byte rate. */
#else /* Word 0 - Little Endian */
        uint64_t avg_rate              : 17; /**< [ 16:  0](R/W/H) Average byte rate. */
        uint64_t reserved_17_63        : 47;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_int_mdrt_dbg_s cn; */
};
typedef union cavm_sdpx_rx_out_int_mdrt_dbg cavm_sdpx_rx_out_int_mdrt_dbg_t;

static inline uint64_t CAVM_SDPX_RX_OUT_INT_MDRT_DBG(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_INT_MDRT_DBG(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e0800103c0ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_INT_MDRT_DBG", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_INT_MDRT_DBG(a,b) cavm_sdpx_rx_out_int_mdrt_dbg_t
#define bustype_CAVM_SDPX_RX_OUT_INT_MDRT_DBG(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_INT_MDRT_DBG(a,b) "SDPX_RX_OUT_INT_MDRT_DBG"
#define device_bar_CAVM_SDPX_RX_OUT_INT_MDRT_DBG(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_INT_MDRT_DBG(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_INT_MDRT_DBG(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_pkt_cnt
 *
 * SDP Packet Output Packet Count Register
 * This register contains packet counts per ring that have been written to memory by SDP.
 * The counter will wrap when it reaches its maximum value.  It should be cleared
 * before the ring is enabled for an accurate count.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_out_pkt_cnt
{
    uint64_t u;
    struct cavm_sdpx_rx_out_pkt_cnt_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_36_63        : 28;
        uint64_t cnt                   : 36; /**< [ 35:  0](R/W/H) Packet count, can be written by software to any value.  If a value of 0xFFFFFFFFF is
                                                                 written to this field, it will cause this field as well as SDP()_R()_OUT_BYTE_CNT to
                                                                 clear. */
#else /* Word 0 - Little Endian */
        uint64_t cnt                   : 36; /**< [ 35:  0](R/W/H) Packet count, can be written by software to any value.  If a value of 0xFFFFFFFFF is
                                                                 written to this field, it will cause this field as well as SDP()_R()_OUT_BYTE_CNT to
                                                                 clear. */
        uint64_t reserved_36_63        : 28;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_pkt_cnt_s cn; */
};
typedef union cavm_sdpx_rx_out_pkt_cnt cavm_sdpx_rx_out_pkt_cnt_t;

static inline uint64_t CAVM_SDPX_RX_OUT_PKT_CNT(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_PKT_CNT(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010180ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_PKT_CNT", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_PKT_CNT(a,b) cavm_sdpx_rx_out_pkt_cnt_t
#define bustype_CAVM_SDPX_RX_OUT_PKT_CNT(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_PKT_CNT(a,b) "SDPX_RX_OUT_PKT_CNT"
#define device_bar_CAVM_SDPX_RX_OUT_PKT_CNT(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_PKT_CNT(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_PKT_CNT(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_slist_baddr
 *
 * SDP Packet Ring Base Address Register
 * This register contains the base address for the output ring.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the VF prior to enabling the ring.  Also, this register cannot be written
 * while either of the following conditions is true:
 *   * SDP()_R()_OUT_CONTROL[IDLE] is clear.
 *   * Or, SDP()_R()_OUT_ENABLE[ENB] is set.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_out_slist_baddr
{
    uint64_t u;
    struct cavm_sdpx_rx_out_slist_baddr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t addr                  : 60; /**< [ 63:  4](R/W) Base address for the output ring, which is an array with
                                                                 SDP()_R()_OUT_SLIST_RSIZE[RSIZE] entries, each entry being a
                                                                 SDP_BUF_INFO_PAIR_S.

                                                                 SDP()_R()_OUT_SLIST_BADDR contains a byte address that must be 16-byte
                                                                 aligned, so SDP()_R()_OUT_SLIST_BADDR\<3:0\> must be zero. */
        uint64_t reserved_0_3          : 4;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_3          : 4;
        uint64_t addr                  : 60; /**< [ 63:  4](R/W) Base address for the output ring, which is an array with
                                                                 SDP()_R()_OUT_SLIST_RSIZE[RSIZE] entries, each entry being a
                                                                 SDP_BUF_INFO_PAIR_S.

                                                                 SDP()_R()_OUT_SLIST_BADDR contains a byte address that must be 16-byte
                                                                 aligned, so SDP()_R()_OUT_SLIST_BADDR\<3:0\> must be zero. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_slist_baddr_s cn; */
};
typedef union cavm_sdpx_rx_out_slist_baddr cavm_sdpx_rx_out_slist_baddr_t;

static inline uint64_t CAVM_SDPX_RX_OUT_SLIST_BADDR(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_SLIST_BADDR(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010120ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_SLIST_BADDR", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_SLIST_BADDR(a,b) cavm_sdpx_rx_out_slist_baddr_t
#define bustype_CAVM_SDPX_RX_OUT_SLIST_BADDR(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_SLIST_BADDR(a,b) "SDPX_RX_OUT_SLIST_BADDR"
#define device_bar_CAVM_SDPX_RX_OUT_SLIST_BADDR(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_SLIST_BADDR(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_SLIST_BADDR(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_slist_dbell
 *
 * SDP Packet Base-Address Offset and Doorbell Registers
 * This register contains the doorbell and base-address offset for the next read operation.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the VF prior to enabling the ring.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_out_slist_dbell
{
    uint64_t u;
    struct cavm_sdpx_rx_out_slist_dbell_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t aoff                  : 32; /**< [ 63: 32](RO/H) Address offset. The offset from the SDP()_R()_OUT_SLIST_BADDR where the next pointer
                                                                 is read.
                                                                 A write of 0xFFFFFFFF to [DBELL] clears [DBELL] and [AOFF]. */
        uint64_t dbell                 : 32; /**< [ 31:  0](R/W/H) Pointer pair list doorbell count. Write operations to this field increments the present
                                                                 value here. Read operations return the present value. The value of this field is
                                                                 decremented as read operations are issued for scatter pointers. A write of 0xFFFFFFFF
                                                                 to this field clears [DBELL] and [AOFF]. The value of this field is in number of
                                                                 SDP_BUF_INFO_PAIR_S's.  This register should be cleared before enabling a ring. */
#else /* Word 0 - Little Endian */
        uint64_t dbell                 : 32; /**< [ 31:  0](R/W/H) Pointer pair list doorbell count. Write operations to this field increments the present
                                                                 value here. Read operations return the present value. The value of this field is
                                                                 decremented as read operations are issued for scatter pointers. A write of 0xFFFFFFFF
                                                                 to this field clears [DBELL] and [AOFF]. The value of this field is in number of
                                                                 SDP_BUF_INFO_PAIR_S's.  This register should be cleared before enabling a ring. */
        uint64_t aoff                  : 32; /**< [ 63: 32](RO/H) Address offset. The offset from the SDP()_R()_OUT_SLIST_BADDR where the next pointer
                                                                 is read.
                                                                 A write of 0xFFFFFFFF to [DBELL] clears [DBELL] and [AOFF]. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_slist_dbell_s cn; */
};
typedef union cavm_sdpx_rx_out_slist_dbell cavm_sdpx_rx_out_slist_dbell_t;

static inline uint64_t CAVM_SDPX_RX_OUT_SLIST_DBELL(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_SLIST_DBELL(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010140ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_SLIST_DBELL", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_SLIST_DBELL(a,b) cavm_sdpx_rx_out_slist_dbell_t
#define bustype_CAVM_SDPX_RX_OUT_SLIST_DBELL(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_SLIST_DBELL(a,b) "SDPX_RX_OUT_SLIST_DBELL"
#define device_bar_CAVM_SDPX_RX_OUT_SLIST_DBELL(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_SLIST_DBELL(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_SLIST_DBELL(a,b) (a),(b),-1,-1

/**
 * Register (PEXP_NCB) sdp#_r#_out_slist_rsize
 *
 * SDP Packet Ring Size Register
 * This register contains the output packet ring size.
 * This register is not affected by reset (including FLR) and must be initialized
 * by the VF prior to enabling the ring.  Also, this register cannot be written
 * while either of the following conditions is true:
 *   * SDP()_R()_OUT_CONTROL[IDLE] is clear.
 *   * Or, SDP()_R()_OUT_ENABLE[ENB] is set.
 *
 * This register is indexed by ring.
 * From a VF the amount of rings that can be indexed is controlled by SDP()_EPF()_RINFO[RPVF].
 * From a PF the amount of rings that can be indexed is controlled by
 * SDP()_MAC()_PF_RING_CTL[RPPF].
 * From an AP core all 256 rings can be indexed.
 */
union cavm_sdpx_rx_out_slist_rsize
{
    uint64_t u;
    struct cavm_sdpx_rx_out_slist_rsize_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t rsize                 : 32; /**< [ 31:  0](R/W/H) Ring size (number of SDP_BUF_INFO_PAIR_S's). This value must be 16 or
                                                                 greater. If a value is written that is less than 16, then hardware
                                                                 will force a value of 16 to be written. */
#else /* Word 0 - Little Endian */
        uint64_t rsize                 : 32; /**< [ 31:  0](R/W/H) Ring size (number of SDP_BUF_INFO_PAIR_S's). This value must be 16 or
                                                                 greater. If a value is written that is less than 16, then hardware
                                                                 will force a value of 16 to be written. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_rx_out_slist_rsize_s cn; */
};
typedef union cavm_sdpx_rx_out_slist_rsize cavm_sdpx_rx_out_slist_rsize_t;

static inline uint64_t CAVM_SDPX_RX_OUT_SLIST_RSIZE(unsigned long a, unsigned long b) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_RX_OUT_SLIST_RSIZE(unsigned long a, unsigned long b)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && ((a==0) && (b<=255)))
        return 0x86e080010130ll + 0x1000000000ll * ((a) & 0x0) + 0x20000ll * ((b) & 0xff);
    __cavm_csr_fatal("SDPX_RX_OUT_SLIST_RSIZE", 2, a, b, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_RX_OUT_SLIST_RSIZE(a,b) cavm_sdpx_rx_out_slist_rsize_t
#define bustype_CAVM_SDPX_RX_OUT_SLIST_RSIZE(a,b) CSR_TYPE_PEXP_NCB
#define basename_CAVM_SDPX_RX_OUT_SLIST_RSIZE(a,b) "SDPX_RX_OUT_SLIST_RSIZE"
#define device_bar_CAVM_SDPX_RX_OUT_SLIST_RSIZE(a,b) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_RX_OUT_SLIST_RSIZE(a,b) (a)
#define arguments_CAVM_SDPX_RX_OUT_SLIST_RSIZE(a,b) (a),(b),-1,-1

/**
 * Register (NCB) sdp#_sctl
 *
 * SDP Secure Control Register
 */
union cavm_sdpx_sctl
{
    uint64_t u;
    struct cavm_sdpx_sctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_1_63         : 63;
        uint64_t scen                  : 1;  /**< [  0:  0](SR/W) Allow SDP window transactions to request secure-world accesses.

                                                                 0 = SDP()_WIN_RD_ADDR[SECEN], SDP()_WIN_WR_ADDR[SECEN] are ignored and treated
                                                                 as if zero. Window transactions onto NCB are nonsecure, though the SMMU may
                                                                 later promote them to secure.

                                                                 1 = SDP()_WIN_RD_ADDR[SECEN], SDP()_WIN_WR_ADDR[SECEN] are honored. Window
                                                                 transactions may request nonsecure or secure world. This bit should not be set
                                                                 in trusted-mode.

                                                                 Resets to 0 when in trusted-mode (RST_BOOT[TRUSTED_MODE]), else resets to 1. */
#else /* Word 0 - Little Endian */
        uint64_t scen                  : 1;  /**< [  0:  0](SR/W) Allow SDP window transactions to request secure-world accesses.

                                                                 0 = SDP()_WIN_RD_ADDR[SECEN], SDP()_WIN_WR_ADDR[SECEN] are ignored and treated
                                                                 as if zero. Window transactions onto NCB are nonsecure, though the SMMU may
                                                                 later promote them to secure.

                                                                 1 = SDP()_WIN_RD_ADDR[SECEN], SDP()_WIN_WR_ADDR[SECEN] are honored. Window
                                                                 transactions may request nonsecure or secure world. This bit should not be set
                                                                 in trusted-mode.

                                                                 Resets to 0 when in trusted-mode (RST_BOOT[TRUSTED_MODE]), else resets to 1. */
        uint64_t reserved_1_63         : 63;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_sctl_s cn; */
};
typedef union cavm_sdpx_sctl cavm_sdpx_sctl_t;

static inline uint64_t CAVM_SDPX_SCTL(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_SCTL(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x86e0c0080310ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_SCTL", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_SCTL(a) cavm_sdpx_sctl_t
#define bustype_CAVM_SDPX_SCTL(a) CSR_TYPE_NCB
#define basename_CAVM_SDPX_SCTL(a) "SDPX_SCTL"
#define device_bar_CAVM_SDPX_SCTL(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_SDPX_SCTL(a) (a)
#define arguments_CAVM_SDPX_SCTL(a) (a),-1,-1,-1

/**
 * Register (PEXP) sdp#_win_rd_addr
 *
 * SDP Window Read Address Register
 * When this register is written the address in this register will be read and
 * the data returned from this read operation is placed in the SDP()_WIN_RD_DATA
 * register.
 * This register should not be used to read SDP_* registers.
 */
union cavm_sdpx_win_rd_addr
{
    uint64_t u;
    struct cavm_sdpx_win_rd_addr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t secen                 : 1;  /**< [ 63: 63](R/W) This request is a secure-world transaction. This is intended to be set only for
                                                                 transactions during early boot when the SMMU is in bypass mode; after SMMU
                                                                 initialization IOBN()_DOM()_BUS()_STREAMS may be used to control which SDP
                                                                 streams are secure.

                                                                 If SDP()_SCTL[SCEN] = 0, this bit is ignored and transactions are always nonsecure
                                                                 onto the NCB, though the SMMU may later promote them to secure. */
        uint64_t reserved_55_62        : 8;
        uint64_t ld_cmd                : 2;  /**< [ 54: 53](R/W) The load command size.
                                                                 0x3 = Load 8 bytes.
                                                                 0x2 = Load 4 bytes.
                                                                 0x1 = Load 2 bytes.
                                                                 0x0 = Load 1 bytes. */
        uint64_t rd_addr               : 53; /**< [ 52:  0](R/W) The IOVA sent to the NCB for this load request. */
#else /* Word 0 - Little Endian */
        uint64_t rd_addr               : 53; /**< [ 52:  0](R/W) The IOVA sent to the NCB for this load request. */
        uint64_t ld_cmd                : 2;  /**< [ 54: 53](R/W) The load command size.
                                                                 0x3 = Load 8 bytes.
                                                                 0x2 = Load 4 bytes.
                                                                 0x1 = Load 2 bytes.
                                                                 0x0 = Load 1 bytes. */
        uint64_t reserved_55_62        : 8;
        uint64_t secen                 : 1;  /**< [ 63: 63](R/W) This request is a secure-world transaction. This is intended to be set only for
                                                                 transactions during early boot when the SMMU is in bypass mode; after SMMU
                                                                 initialization IOBN()_DOM()_BUS()_STREAMS may be used to control which SDP
                                                                 streams are secure.

                                                                 If SDP()_SCTL[SCEN] = 0, this bit is ignored and transactions are always nonsecure
                                                                 onto the NCB, though the SMMU may later promote them to secure. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_win_rd_addr_s cn; */
};
typedef union cavm_sdpx_win_rd_addr cavm_sdpx_win_rd_addr_t;

static inline uint64_t CAVM_SDPX_WIN_RD_ADDR(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_WIN_RD_ADDR(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x20010ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_WIN_RD_ADDR", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_WIN_RD_ADDR(a) cavm_sdpx_win_rd_addr_t
#define bustype_CAVM_SDPX_WIN_RD_ADDR(a) CSR_TYPE_PEXP
#define basename_CAVM_SDPX_WIN_RD_ADDR(a) "SDPX_WIN_RD_ADDR"
#define busnum_CAVM_SDPX_WIN_RD_ADDR(a) (a)
#define arguments_CAVM_SDPX_WIN_RD_ADDR(a) (a),-1,-1,-1

/**
 * Register (PEXP) sdp#_win_rd_data
 *
 * SDP Window Read Data Register
 * This register contains the data returned from a write to SDP()_WIN_RD_ADDR.
 */
union cavm_sdpx_win_rd_data
{
    uint64_t u;
    struct cavm_sdpx_win_rd_data_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t rd_data               : 64; /**< [ 63:  0](RO/H) The read data. */
#else /* Word 0 - Little Endian */
        uint64_t rd_data               : 64; /**< [ 63:  0](RO/H) The read data. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_win_rd_data_s cn; */
};
typedef union cavm_sdpx_win_rd_data cavm_sdpx_win_rd_data_t;

static inline uint64_t CAVM_SDPX_WIN_RD_DATA(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_WIN_RD_DATA(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x20040ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_WIN_RD_DATA", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_WIN_RD_DATA(a) cavm_sdpx_win_rd_data_t
#define bustype_CAVM_SDPX_WIN_RD_DATA(a) CSR_TYPE_PEXP
#define basename_CAVM_SDPX_WIN_RD_DATA(a) "SDPX_WIN_RD_DATA"
#define busnum_CAVM_SDPX_WIN_RD_DATA(a) (a)
#define arguments_CAVM_SDPX_WIN_RD_DATA(a) (a),-1,-1,-1

/**
 * Register (PEXP) sdp#_win_wr_addr
 *
 * SDP Window Write Address Register
 * Contains the address to be written to when a write operation is started by writing
 * SDP()_WIN_WR_DATA.
 * This register should not be used to write SDP_* registers.
 */
union cavm_sdpx_win_wr_addr
{
    uint64_t u;
    struct cavm_sdpx_win_wr_addr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t secen                 : 1;  /**< [ 63: 63](R/W) This request is a secure-world transaction. This is intended to be set only for
                                                                 transactions during early boot when the SMMU is in bypass mode; after SMMU
                                                                 initialization IOBN()_DOM()_BUS()_STREAMS may be used to control which SDP
                                                                 streams are secure.

                                                                 If SDP()_SCTL[SCEN] = 0, this bit is ignored and transactions are always nonsecure
                                                                 onto the NCB, though the SMMU may later promote them to secure. */
        uint64_t reserved_53_62        : 10;
        uint64_t wr_addr               : 50; /**< [ 52:  3](R/W) The IOVA sent to the NCB for this store request. */
        uint64_t reserved_0_2          : 3;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_2          : 3;
        uint64_t wr_addr               : 50; /**< [ 52:  3](R/W) The IOVA sent to the NCB for this store request. */
        uint64_t reserved_53_62        : 10;
        uint64_t secen                 : 1;  /**< [ 63: 63](R/W) This request is a secure-world transaction. This is intended to be set only for
                                                                 transactions during early boot when the SMMU is in bypass mode; after SMMU
                                                                 initialization IOBN()_DOM()_BUS()_STREAMS may be used to control which SDP
                                                                 streams are secure.

                                                                 If SDP()_SCTL[SCEN] = 0, this bit is ignored and transactions are always nonsecure
                                                                 onto the NCB, though the SMMU may later promote them to secure. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_win_wr_addr_s cn; */
};
typedef union cavm_sdpx_win_wr_addr cavm_sdpx_win_wr_addr_t;

static inline uint64_t CAVM_SDPX_WIN_WR_ADDR(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_WIN_WR_ADDR(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x20000ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_WIN_WR_ADDR", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_WIN_WR_ADDR(a) cavm_sdpx_win_wr_addr_t
#define bustype_CAVM_SDPX_WIN_WR_ADDR(a) CSR_TYPE_PEXP
#define basename_CAVM_SDPX_WIN_WR_ADDR(a) "SDPX_WIN_WR_ADDR"
#define busnum_CAVM_SDPX_WIN_WR_ADDR(a) (a)
#define arguments_CAVM_SDPX_WIN_WR_ADDR(a) (a),-1,-1,-1

/**
 * Register (PEXP) sdp#_win_wr_data
 *
 * SDP Window Write Data Register
 * This register contains the data to write to the address located in SDP()_WIN_WR_ADDR.
 * Writing this register causes a write operation to take place.
 */
union cavm_sdpx_win_wr_data
{
    uint64_t u;
    struct cavm_sdpx_win_wr_data_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t wr_data               : 64; /**< [ 63:  0](R/W) The data to be written. */
#else /* Word 0 - Little Endian */
        uint64_t wr_data               : 64; /**< [ 63:  0](R/W) The data to be written. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_win_wr_data_s cn; */
};
typedef union cavm_sdpx_win_wr_data cavm_sdpx_win_wr_data_t;

static inline uint64_t CAVM_SDPX_WIN_WR_DATA(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_WIN_WR_DATA(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x20020ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_WIN_WR_DATA", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_WIN_WR_DATA(a) cavm_sdpx_win_wr_data_t
#define bustype_CAVM_SDPX_WIN_WR_DATA(a) CSR_TYPE_PEXP
#define basename_CAVM_SDPX_WIN_WR_DATA(a) "SDPX_WIN_WR_DATA"
#define busnum_CAVM_SDPX_WIN_WR_DATA(a) (a)
#define arguments_CAVM_SDPX_WIN_WR_DATA(a) (a),-1,-1,-1

/**
 * Register (PEXP) sdp#_win_wr_mask
 *
 * SDP Window Write Mask Register
 * This register contains the mask for the data in SDP()_WIN_WR_DATA.
 */
union cavm_sdpx_win_wr_mask
{
    uint64_t u;
    struct cavm_sdpx_win_wr_mask_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_8_63         : 56;
        uint64_t wr_mask               : 8;  /**< [  7:  0](R/W) The byte enables sent to the NCB for this store request. */
#else /* Word 0 - Little Endian */
        uint64_t wr_mask               : 8;  /**< [  7:  0](R/W) The byte enables sent to the NCB for this store request. */
        uint64_t reserved_8_63         : 56;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_sdpx_win_wr_mask_s cn; */
};
typedef union cavm_sdpx_win_wr_mask cavm_sdpx_win_wr_mask_t;

static inline uint64_t CAVM_SDPX_WIN_WR_MASK(unsigned long a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_SDPX_WIN_WR_MASK(unsigned long a)
{
    if (cavm_is_model(OCTEONTX_CN9XXX) && (a==0))
        return 0x20030ll + 0x1000000000ll * ((a) & 0x0);
    __cavm_csr_fatal("SDPX_WIN_WR_MASK", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_SDPX_WIN_WR_MASK(a) cavm_sdpx_win_wr_mask_t
#define bustype_CAVM_SDPX_WIN_WR_MASK(a) CSR_TYPE_PEXP
#define basename_CAVM_SDPX_WIN_WR_MASK(a) "SDPX_WIN_WR_MASK"
#define busnum_CAVM_SDPX_WIN_WR_MASK(a) (a)
#define arguments_CAVM_SDPX_WIN_WR_MASK(a) (a),-1,-1,-1

#endif /* __CAVM_CSRS_DPI_H__ */
