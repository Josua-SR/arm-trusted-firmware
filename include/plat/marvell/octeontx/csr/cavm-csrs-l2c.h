#ifndef __CAVM_CSRS_L2C_H__
#define __CAVM_CSRS_L2C_H__
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
 * OcteonTX L2C.
 *
 * This file is auto generated. Do not edit.
 *
 */

/**
 * Enumeration inv_cmd_e
 *
 * INTERNAL: INV Command Enumeration
 *
 * Enumerates the different INV command encodings.
 */
#define CAVM_INV_CMD_E_ALLEX (0xc)
#define CAVM_INV_CMD_E_ASIDE1 (0xd)
#define CAVM_INV_CMD_E_GBLSYNC (0xf)
#define CAVM_INV_CMD_E_IALLU (9)
#define CAVM_INV_CMD_E_INV (8)
#define CAVM_INV_CMD_E_IPAS2E1 (7)
#define CAVM_INV_CMD_E_IVAU (4)
#define CAVM_INV_CMD_E_NOP (0)
#define CAVM_INV_CMD_E_SEV (0xe)
#define CAVM_INV_CMD_E_VAAE1 (6)
#define CAVM_INV_CMD_E_VAEX (5)
#define CAVM_INV_CMD_E_VMALLE1 (0xa)
#define CAVM_INV_CMD_E_VMALLS12 (0xb)

/**
 * Enumeration ioc_cmd_e
 *
 * INTERNAL: IOC Command Enumeration
 *
 * Enumerates the different IOC command encodings.
 */
#define CAVM_IOC_CMD_E_ADDR (6)
#define CAVM_IOC_CMD_E_IAADD (0xc)
#define CAVM_IOC_CMD_E_IACAS (0xa)
#define CAVM_IOC_CMD_E_IACLR (0xd)
#define CAVM_IOC_CMD_E_IASET (0xe)
#define CAVM_IOC_CMD_E_IASWP (0xb)
#define CAVM_IOC_CMD_E_IDLE (0)
#define CAVM_IOC_CMD_E_LMTST (3)
#define CAVM_IOC_CMD_E_LOAD (2)
#define CAVM_IOC_CMD_E_SLILD (8)
#define CAVM_IOC_CMD_E_SLIST (7)
#define CAVM_IOC_CMD_E_STORE (1)
#define CAVM_IOC_CMD_E_STOREP (9)

/**
 * Enumeration ior_cmd_e
 *
 * INTERNAL: IOR Command Enumeration
 *
 * Enumerates the different IOR command encodings.
 */
#define CAVM_IOR_CMD_E_DATA (1)
#define CAVM_IOR_CMD_E_IDLE (0)
#define CAVM_IOR_CMD_E_SLIRSP (3)

/**
 * Enumeration l2c_bar_e
 *
 * L2C Base Address Register Enumeration
 * Enumerates the base address registers.
 */
#define CAVM_L2C_BAR_E_L2C_PF_BAR0 (0x87e080800000ll)
#define CAVM_L2C_BAR_E_L2C_PF_BAR0_SIZE 0x800000ull

/**
 * Enumeration l2c_dat_errprio_e
 *
 * L2C Quad Error Priority Enumeration
 * Enumerates the different quad error priorities.
 */
#define CAVM_L2C_DAT_ERRPRIO_E_FBFDBE (4)
#define CAVM_L2C_DAT_ERRPRIO_E_FBFSBE (1)
#define CAVM_L2C_DAT_ERRPRIO_E_L2DDBE (6)
#define CAVM_L2C_DAT_ERRPRIO_E_L2DSBE (3)
#define CAVM_L2C_DAT_ERRPRIO_E_NBE (0)
#define CAVM_L2C_DAT_ERRPRIO_E_SBFDBE (5)
#define CAVM_L2C_DAT_ERRPRIO_E_SBFSBE (2)

/**
 * Enumeration l2c_tad_prf_sel_e
 *
 * L2C TAD Performance Counter Select Enumeration
 * Enumerates the different TAD performance counter selects.
 */
#define CAVM_L2C_TAD_PRF_SEL_E_L2T_HIT (1)
#define CAVM_L2C_TAD_PRF_SEL_E_L2T_MISS (2)
#define CAVM_L2C_TAD_PRF_SEL_E_L2T_NOALLOC (3)
#define CAVM_L2C_TAD_PRF_SEL_E_L2_VIC (4)
#define CAVM_L2C_TAD_PRF_SEL_E_LFB_OCC (7)
#define CAVM_L2C_TAD_PRF_SEL_E_LMC_WR (0x4e)
#define CAVM_L2C_TAD_PRF_SEL_E_LMC_WR_SBLKDTY (0x4f)
#define CAVM_L2C_TAD_PRF_SEL_E_LOOKUP (0x40)
#define CAVM_L2C_TAD_PRF_SEL_E_LOOKUP_ALL (0x44)
#define CAVM_L2C_TAD_PRF_SEL_E_LOOKUP_MIB (0x43)
#define CAVM_L2C_TAD_PRF_SEL_E_LOOKUP_XMC_LCL (0x41)
#define CAVM_L2C_TAD_PRF_SEL_E_LOOKUP_XMC_RMT (0x42)
#define CAVM_L2C_TAD_PRF_SEL_E_NONE (0)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_FLDX_TAG_E_DAT (0x6d)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_FLDX_TAG_E_NODAT (0x6c)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_FWD_CYC_HIT (0x69)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_FWD_RACE (0x6a)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_HAKS (0x6b)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_RLDD (0x6e)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_RLDD_PEMD (0x6f)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_RRQ_DAT_CNT (0x70)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_RRQ_DAT_DMASK (0x71)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_RSP_DAT_CNT (0x72)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_RSP_DAT_DMASK (0x73)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_RSP_DAT_VICD_CNT (0x74)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_RSP_DAT_VICD_DMASK (0x75)
#define CAVM_L2C_TAD_PRF_SEL_E_OCI_RTG_WAIT (0x68)
#define CAVM_L2C_TAD_PRF_SEL_E_OPEN_CCPI (0xa)
#define CAVM_L2C_TAD_PRF_SEL_E_QDX_BNKS(a) (0x82 + 0x10 * (a))
#define CAVM_L2C_TAD_PRF_SEL_E_QDX_IDX(a) (0x80 + 0x10 * (a))
#define CAVM_L2C_TAD_PRF_SEL_E_QDX_RDAT(a) (0x81 + 0x10 * (a))
#define CAVM_L2C_TAD_PRF_SEL_E_QDX_WDAT(a) (0x83 + 0x10 * (a))
#define CAVM_L2C_TAD_PRF_SEL_E_RTG_ALC (0x5d)
#define CAVM_L2C_TAD_PRF_SEL_E_RTG_ALC_EVICT (0x76)
#define CAVM_L2C_TAD_PRF_SEL_E_RTG_ALC_HIT (0x5e)
#define CAVM_L2C_TAD_PRF_SEL_E_RTG_ALC_HITWB (0x5f)
#define CAVM_L2C_TAD_PRF_SEL_E_RTG_ALC_VIC (0x77)
#define CAVM_L2C_TAD_PRF_SEL_E_SC_FAIL (5)
#define CAVM_L2C_TAD_PRF_SEL_E_SC_PASS (6)
#define CAVM_L2C_TAD_PRF_SEL_E_STC_LCL (0x64)
#define CAVM_L2C_TAD_PRF_SEL_E_STC_LCL_FAIL (0x65)
#define CAVM_L2C_TAD_PRF_SEL_E_STC_RMT (0x62)
#define CAVM_L2C_TAD_PRF_SEL_E_STC_RMT_FAIL (0x63)
#define CAVM_L2C_TAD_PRF_SEL_E_STC_TOTAL (0x60)
#define CAVM_L2C_TAD_PRF_SEL_E_STC_TOTAL_FAIL (0x61)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_ALC_HIT (0x48)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_ALC_LCL_CLNVIC (0x59)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_ALC_LCL_DTYVIC (0x5a)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_ALC_LCL_EVICT (0x58)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_ALC_MISS (0x49)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_ALC_NALC (0x4a)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_ALC_RMT_EVICT (0x5b)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_ALC_RMT_VIC (0x5c)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_ALC_RTG_HIT (0x50)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_ALC_RTG_HITE (0x51)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_ALC_RTG_HITS (0x52)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_ALC_RTG_MISS (0x53)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_NALC_HIT (0x4b)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_NALC_MISS (0x4c)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_NALC_RTG_HIT (0x54)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_NALC_RTG_HITE (0x56)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_NALC_RTG_HITS (0x57)
#define CAVM_L2C_TAD_PRF_SEL_E_TAG_NALC_RTG_MISS (0x55)
#define CAVM_L2C_TAD_PRF_SEL_E_WAIT_LFB (8)
#define CAVM_L2C_TAD_PRF_SEL_E_WAIT_VAB (9)

/**
 * Enumeration l2c_tag_errprio_e
 *
 * L2C Tag Error Priority Enumeration
 * Enumerates the different TAG error priorities.
 */
#define CAVM_L2C_TAG_ERRPRIO_E_DBE (3)
#define CAVM_L2C_TAG_ERRPRIO_E_NBE (0)
#define CAVM_L2C_TAG_ERRPRIO_E_NOWAY (1)
#define CAVM_L2C_TAG_ERRPRIO_E_SBE (2)

/**
 * Enumeration oci_ireq_cmd_e
 *
 * INTERNAL: OCI IREQ Command Enumeration
 */
#define CAVM_OCI_IREQ_CMD_E_IAADD (0x10)
#define CAVM_OCI_IREQ_CMD_E_IACAS (0x15)
#define CAVM_OCI_IREQ_CMD_E_IACLR (0x12)
#define CAVM_OCI_IREQ_CMD_E_IASET (0x13)
#define CAVM_OCI_IREQ_CMD_E_IASWP (0x14)
#define CAVM_OCI_IREQ_CMD_E_IDLE (0x1f)
#define CAVM_OCI_IREQ_CMD_E_IOBADDR (6)
#define CAVM_OCI_IREQ_CMD_E_IOBADDRA (7)
#define CAVM_OCI_IREQ_CMD_E_IOBLD (0)
#define CAVM_OCI_IREQ_CMD_E_IOBST (2)
#define CAVM_OCI_IREQ_CMD_E_IOBSTA (3)
#define CAVM_OCI_IREQ_CMD_E_IOBSTP (4)
#define CAVM_OCI_IREQ_CMD_E_IOBSTPA (5)
#define CAVM_OCI_IREQ_CMD_E_LMTST (8)
#define CAVM_OCI_IREQ_CMD_E_LMTSTA (9)
#define CAVM_OCI_IREQ_CMD_E_SLILD (0x1c)
#define CAVM_OCI_IREQ_CMD_E_SLIST (0x1d)

/**
 * Enumeration oci_irsp_cmd_e
 *
 * INTERNAL: OCI IRSP Command Enumeration
 */
#define CAVM_OCI_IRSP_CMD_E_IDLE (0x1f)
#define CAVM_OCI_IRSP_CMD_E_IOBACK (1)
#define CAVM_OCI_IRSP_CMD_E_IOBRSP (0)
#define CAVM_OCI_IRSP_CMD_E_SLIRSP (2)

/**
 * Enumeration oci_mfwd_cmd_e
 *
 * INTERNAL: OCI MFWD Command Enumeration
 */
#define CAVM_OCI_MFWD_CMD_E_FEVX_EH (0xb)
#define CAVM_OCI_MFWD_CMD_E_FEVX_OH (0xc)
#define CAVM_OCI_MFWD_CMD_E_FLDRO_E (0)
#define CAVM_OCI_MFWD_CMD_E_FLDRO_O (1)
#define CAVM_OCI_MFWD_CMD_E_FLDRS_E (2)
#define CAVM_OCI_MFWD_CMD_E_FLDRS_EH (4)
#define CAVM_OCI_MFWD_CMD_E_FLDRS_O (3)
#define CAVM_OCI_MFWD_CMD_E_FLDRS_OH (5)
#define CAVM_OCI_MFWD_CMD_E_FLDT_E (6)
#define CAVM_OCI_MFWD_CMD_E_FLDX_E (7)
#define CAVM_OCI_MFWD_CMD_E_FLDX_EH (9)
#define CAVM_OCI_MFWD_CMD_E_FLDX_O (8)
#define CAVM_OCI_MFWD_CMD_E_FLDX_OH (0xa)
#define CAVM_OCI_MFWD_CMD_E_IDLE (0x1f)
#define CAVM_OCI_MFWD_CMD_E_SINV (0xd)
#define CAVM_OCI_MFWD_CMD_E_SINV_H (0xe)

/**
 * Enumeration oci_mreq_cmd_e
 *
 * INTERNAL: OCI MREQ Command Enumeration
 */
#define CAVM_OCI_MREQ_CMD_E_GINV (0x14)
#define CAVM_OCI_MREQ_CMD_E_GSYNC (0x18)
#define CAVM_OCI_MREQ_CMD_E_IDLE (0x1f)
#define CAVM_OCI_MREQ_CMD_E_RADD (0xd)
#define CAVM_OCI_MREQ_CMD_E_RC2D_O (6)
#define CAVM_OCI_MREQ_CMD_E_RC2D_S (7)
#define CAVM_OCI_MREQ_CMD_E_RCAS (0x13)
#define CAVM_OCI_MREQ_CMD_E_RCAS_O (0x15)
#define CAVM_OCI_MREQ_CMD_E_RCAS_S (0x16)
#define CAVM_OCI_MREQ_CMD_E_RCLR (0x12)
#define CAVM_OCI_MREQ_CMD_E_RDEC (0xf)
#define CAVM_OCI_MREQ_CMD_E_REOR (0xb)
#define CAVM_OCI_MREQ_CMD_E_RINC (0xe)
#define CAVM_OCI_MREQ_CMD_E_RLDD (0)
#define CAVM_OCI_MREQ_CMD_E_RLDI (1)
#define CAVM_OCI_MREQ_CMD_E_RLDT (2)
#define CAVM_OCI_MREQ_CMD_E_RLDWB (4)
#define CAVM_OCI_MREQ_CMD_E_RLDX (5)
#define CAVM_OCI_MREQ_CMD_E_RLDY (3)
#define CAVM_OCI_MREQ_CMD_E_RSET (0x11)
#define CAVM_OCI_MREQ_CMD_E_RSMAX (0x1b)
#define CAVM_OCI_MREQ_CMD_E_RSMIN (0x1c)
#define CAVM_OCI_MREQ_CMD_E_RSTC (0x17)
#define CAVM_OCI_MREQ_CMD_E_RSTC_O (0x19)
#define CAVM_OCI_MREQ_CMD_E_RSTC_S (0x1a)
#define CAVM_OCI_MREQ_CMD_E_RSTP (0xa)
#define CAVM_OCI_MREQ_CMD_E_RSTT (8)
#define CAVM_OCI_MREQ_CMD_E_RSTY (9)
#define CAVM_OCI_MREQ_CMD_E_RSWP (0x10)
#define CAVM_OCI_MREQ_CMD_E_RUMAX (0x1d)
#define CAVM_OCI_MREQ_CMD_E_RUMIN (0x1e)

/**
 * Enumeration oci_mrsp_cmd_e
 *
 * INTERNAL: OCI MRSP Command Enumeration
 */
#define CAVM_OCI_MRSP_CMD_E_GSDN (0x18)
#define CAVM_OCI_MRSP_CMD_E_HAKD (4)
#define CAVM_OCI_MRSP_CMD_E_HAKI (6)
#define CAVM_OCI_MRSP_CMD_E_HAKN_S (5)
#define CAVM_OCI_MRSP_CMD_E_HAKS (7)
#define CAVM_OCI_MRSP_CMD_E_HAKV (8)
#define CAVM_OCI_MRSP_CMD_E_IDLE (0x1f)
#define CAVM_OCI_MRSP_CMD_E_P2DF (0xd)
#define CAVM_OCI_MRSP_CMD_E_PACK (0xc)
#define CAVM_OCI_MRSP_CMD_E_PATM (0xb)
#define CAVM_OCI_MRSP_CMD_E_PEMD (0xa)
#define CAVM_OCI_MRSP_CMD_E_PSHA (9)
#define CAVM_OCI_MRSP_CMD_E_VICC (1)
#define CAVM_OCI_MRSP_CMD_E_VICD (0)
#define CAVM_OCI_MRSP_CMD_E_VICDHI (3)
#define CAVM_OCI_MRSP_CMD_E_VICS (2)

/**
 * Enumeration rsc_cmd_e
 *
 * INTERNAL: RSC Command Enumeration
 *
 * Enumerates the different RSC command encodings.
 */
#define CAVM_RSC_CMD_E_FLDN (3)
#define CAVM_RSC_CMD_E_GSDN (2)
#define CAVM_RSC_CMD_E_IACK (5)
#define CAVM_RSC_CMD_E_IFDN (1)
#define CAVM_RSC_CMD_E_NOP (0)
#define CAVM_RSC_CMD_E_SCDN (6)
#define CAVM_RSC_CMD_E_SCFL (7)
#define CAVM_RSC_CMD_E_STDN (4)

/**
 * Enumeration xmc_cmd_e
 *
 * INTERNAL: XMC Command Enumeration
 *
 * Enumerates the different XMC command encodings.
 */
#define CAVM_XMC_CMD_E_AADD (0x28)
#define CAVM_XMC_CMD_E_ACAS (0x26)
#define CAVM_XMC_CMD_E_ACLR (0x29)
#define CAVM_XMC_CMD_E_ADEC (0x25)
#define CAVM_XMC_CMD_E_AEOR (0x2a)
#define CAVM_XMC_CMD_E_AINC (0x24)
#define CAVM_XMC_CMD_E_ALLEX (0x3c)
#define CAVM_XMC_CMD_E_ASET (0x2b)
#define CAVM_XMC_CMD_E_ASIDE1 (0x3d)
#define CAVM_XMC_CMD_E_ASMAX (0x2c)
#define CAVM_XMC_CMD_E_ASMIN (0x2d)
#define CAVM_XMC_CMD_E_ASWP (0x27)
#define CAVM_XMC_CMD_E_AUMAX (0x2e)
#define CAVM_XMC_CMD_E_AUMIN (0x2f)
#define CAVM_XMC_CMD_E_DWB (5)
#define CAVM_XMC_CMD_E_GBLSYNC (0x3f)
#define CAVM_XMC_CMD_E_IAADD (0x68)
#define CAVM_XMC_CMD_E_IACAS (0x66)
#define CAVM_XMC_CMD_E_IACLR (0x69)
#define CAVM_XMC_CMD_E_IALLU (0x39)
#define CAVM_XMC_CMD_E_IASET (0x6b)
#define CAVM_XMC_CMD_E_IASWP (0x67)
#define CAVM_XMC_CMD_E_INVL2 (0x1c)
#define CAVM_XMC_CMD_E_IOBADDR (0x43)
#define CAVM_XMC_CMD_E_IOBADDRA (0x53)
#define CAVM_XMC_CMD_E_IOBLD (0x40)
#define CAVM_XMC_CMD_E_IOBST (0x41)
#define CAVM_XMC_CMD_E_IOBSTA (0x51)
#define CAVM_XMC_CMD_E_IOBSTP (0x42)
#define CAVM_XMC_CMD_E_IOBSTPA (0x52)
#define CAVM_XMC_CMD_E_IPAS2E1 (0x37)
#define CAVM_XMC_CMD_E_IVAU (0x34)
#define CAVM_XMC_CMD_E_LCKL2 (0x1f)
#define CAVM_XMC_CMD_E_LDD (8)
#define CAVM_XMC_CMD_E_LDDT (0xc)
#define CAVM_XMC_CMD_E_LDE (0xb)
#define CAVM_XMC_CMD_E_LDI (2)
#define CAVM_XMC_CMD_E_LDP (7)
#define CAVM_XMC_CMD_E_LDT (1)
#define CAVM_XMC_CMD_E_LDWB (0xd)
#define CAVM_XMC_CMD_E_LDY (6)
#define CAVM_XMC_CMD_E_LMTST (0x45)
#define CAVM_XMC_CMD_E_LMTSTA (0x55)
#define CAVM_XMC_CMD_E_LTGL2I (0x19)
#define CAVM_XMC_CMD_E_NOP (0)
#define CAVM_XMC_CMD_E_PL2 (3)
#define CAVM_XMC_CMD_E_PL2T (0x16)
#define CAVM_XMC_CMD_E_PS2 (0xa)
#define CAVM_XMC_CMD_E_PS2T (0x17)
#define CAVM_XMC_CMD_E_PSL1 (9)
#define CAVM_XMC_CMD_E_RPL2 (4)
#define CAVM_XMC_CMD_E_RSTP (0xf)
#define CAVM_XMC_CMD_E_SEV (0x3e)
#define CAVM_XMC_CMD_E_STC (0x13)
#define CAVM_XMC_CMD_E_STF (0x10)
#define CAVM_XMC_CMD_E_STFIL1 (0x14)
#define CAVM_XMC_CMD_E_STGL2I (0x1a)
#define CAVM_XMC_CMD_E_STP (0x12)
#define CAVM_XMC_CMD_E_STT (0x11)
#define CAVM_XMC_CMD_E_STTIL1 (0x15)
#define CAVM_XMC_CMD_E_STY (0xe)
#define CAVM_XMC_CMD_E_VAAE1 (0x36)
#define CAVM_XMC_CMD_E_VAEX (0x35)
#define CAVM_XMC_CMD_E_VMALLE1 (0x3a)
#define CAVM_XMC_CMD_E_VMALLS12 (0x3b)
#define CAVM_XMC_CMD_E_WBIL2 (0x1d)
#define CAVM_XMC_CMD_E_WBIL2I (0x18)
#define CAVM_XMC_CMD_E_WBL2 (0x1e)
#define CAVM_XMC_CMD_E_WBL2I (0x1b)

/**
 * Register (RSL) l2c_asc_region#_attr
 *
 * L2C Address Space Control Region Attributes Registers
 */
union cavm_l2c_asc_regionx_attr
{
    uint64_t u;
    struct cavm_l2c_asc_regionx_attr_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_2_63         : 62;
        uint64_t s_en                  : 1;  /**< [  1:  1](R/W) Enables secure access to region.
                                                                 Undefined if both [S_EN] and [NS_EN] are set for the same region. */
        uint64_t ns_en                 : 1;  /**< [  0:  0](R/W) Enables nonsecure access to region.
                                                                 Undefined if both [S_EN] and [NS_EN] are set for the same region.

                                                                 Internal:
                                                                 See also DFA_ASC_REGION()_ATTR[NS_EN]. */
#else /* Word 0 - Little Endian */
        uint64_t ns_en                 : 1;  /**< [  0:  0](R/W) Enables nonsecure access to region.
                                                                 Undefined if both [S_EN] and [NS_EN] are set for the same region.

                                                                 Internal:
                                                                 See also DFA_ASC_REGION()_ATTR[NS_EN]. */
        uint64_t s_en                  : 1;  /**< [  1:  1](R/W) Enables secure access to region.
                                                                 Undefined if both [S_EN] and [NS_EN] are set for the same region. */
        uint64_t reserved_2_63         : 62;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_asc_regionx_attr_s cn; */
};
typedef union cavm_l2c_asc_regionx_attr cavm_l2c_asc_regionx_attr_t;

static inline uint64_t CAVM_L2C_ASC_REGIONX_ATTR(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_ASC_REGIONX_ATTR(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN8XXX) && (a<=3))
        return 0x87e080801010ll + 0x40ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_ASC_REGIONX_ATTR", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_ASC_REGIONX_ATTR(a) cavm_l2c_asc_regionx_attr_t
#define bustype_CAVM_L2C_ASC_REGIONX_ATTR(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_ASC_REGIONX_ATTR(a) "L2C_ASC_REGIONX_ATTR"
#define device_bar_CAVM_L2C_ASC_REGIONX_ATTR(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_ASC_REGIONX_ATTR(a) (a)
#define arguments_CAVM_L2C_ASC_REGIONX_ATTR(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_asc_region#_end
 *
 * L2C Address Space Control Region End Address Registers
 */
union cavm_l2c_asc_regionx_end
{
    uint64_t u;
    struct cavm_l2c_asc_regionx_end_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_40_63        : 24;
        uint64_t addr                  : 20; /**< [ 39: 20](R/W) Node-local physical address \<39:20\> marking the inclusive end of the corresponding ASC
                                                                 region.
                                                                 Note that the region includes this address.
                                                                 Software must ensure that regions do not overlap.
                                                                 To specify an empty region, clear both the [S_EN] and [NS_EN] fields of
                                                                 the corresponding L2C_ASC_REGION()_ATTR register. */
        uint64_t reserved_0_19         : 20;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_19         : 20;
        uint64_t addr                  : 20; /**< [ 39: 20](R/W) Node-local physical address \<39:20\> marking the inclusive end of the corresponding ASC
                                                                 region.
                                                                 Note that the region includes this address.
                                                                 Software must ensure that regions do not overlap.
                                                                 To specify an empty region, clear both the [S_EN] and [NS_EN] fields of
                                                                 the corresponding L2C_ASC_REGION()_ATTR register. */
        uint64_t reserved_40_63        : 24;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_asc_regionx_end_s cn; */
};
typedef union cavm_l2c_asc_regionx_end cavm_l2c_asc_regionx_end_t;

static inline uint64_t CAVM_L2C_ASC_REGIONX_END(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_ASC_REGIONX_END(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN8XXX) && (a<=3))
        return 0x87e080801008ll + 0x40ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_ASC_REGIONX_END", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_ASC_REGIONX_END(a) cavm_l2c_asc_regionx_end_t
#define bustype_CAVM_L2C_ASC_REGIONX_END(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_ASC_REGIONX_END(a) "L2C_ASC_REGIONX_END"
#define device_bar_CAVM_L2C_ASC_REGIONX_END(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_ASC_REGIONX_END(a) (a)
#define arguments_CAVM_L2C_ASC_REGIONX_END(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_asc_region#_start
 *
 * L2C Address Space Control Region Start Address Registers
 */
union cavm_l2c_asc_regionx_start
{
    uint64_t u;
    struct cavm_l2c_asc_regionx_start_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_40_63        : 24;
        uint64_t addr                  : 20; /**< [ 39: 20](R/W) Node-local physical address \<39:20\> marking the start of the corresponding ASC region.
                                                                 Software must ensure that regions do not overlap. */
        uint64_t reserved_0_19         : 20;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_19         : 20;
        uint64_t addr                  : 20; /**< [ 39: 20](R/W) Node-local physical address \<39:20\> marking the start of the corresponding ASC region.
                                                                 Software must ensure that regions do not overlap. */
        uint64_t reserved_40_63        : 24;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_asc_regionx_start_s cn; */
};
typedef union cavm_l2c_asc_regionx_start cavm_l2c_asc_regionx_start_t;

static inline uint64_t CAVM_L2C_ASC_REGIONX_START(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_ASC_REGIONX_START(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN8XXX) && (a<=3))
        return 0x87e080801000ll + 0x40ll * ((a) & 0x3);
    __cavm_csr_fatal("L2C_ASC_REGIONX_START", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_ASC_REGIONX_START(a) cavm_l2c_asc_regionx_start_t
#define bustype_CAVM_L2C_ASC_REGIONX_START(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_ASC_REGIONX_START(a) "L2C_ASC_REGIONX_START"
#define device_bar_CAVM_L2C_ASC_REGIONX_START(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_ASC_REGIONX_START(a) (a)
#define arguments_CAVM_L2C_ASC_REGIONX_START(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_ctl
 *
 * L2C Control Register
 */
union cavm_l2c_ctl
{
    uint64_t u;
    struct cavm_l2c_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_32_63        : 32;
        uint64_t ocla_qos              : 3;  /**< [ 31: 29](R/W) QOS level for the transactions from OCLA to L2C. */
        uint64_t reserved_28           : 1;
        uint64_t disstgl2i             : 1;  /**< [ 27: 27](R/W) Disable STGL2Is from changing the tags. */
        uint64_t reserved_25_26        : 2;
        uint64_t discclk               : 1;  /**< [ 24: 24](R/W) Disable conditional clocking in L2C PNR blocks. */
        uint64_t reserved_16_23        : 8;
        uint64_t rsp_arb_mode          : 1;  /**< [ 15: 15](R/W) Arbitration mode for RSC/RSD bus. 0 = round-robin; 1 = static priority.
                                                                 1. IOR data.
                                                                 2. STIN/FILLs.
                                                                 3. STDN/SCDN/SCFL. */
        uint64_t xmc_arb_mode          : 1;  /**< [ 14: 14](R/W) Arbitration mode for ADD bus QOS queues. 0 = fully determined through QOS, 1 = QOS0
                                                                 highest priority; QOS 1-7 use normal mode. */
        uint64_t rdf_cnt               : 8;  /**< [ 13:  6](R/W) Defines the sample point of the LMC response data in the DDR-clock/core-clock crossing.
                                                                 For optimal performance set to
                                                                 10 * (DDR-clock period/core-clock period) - 1.
                                                                 To disable set to 0. All other values are reserved. */
        uint64_t disdwb                : 1;  /**< [  5:  5](R/W) Suppresses DWB and INVL2 commands, effectively turning them into NOPs.
                                                                 Internal:
                                                                 The real behavior is DWB and INVL2 commands are forced to look like STGL2I commands with
                                                                 DISSTGL2I set. */
        uint64_t disgsyncto            : 1;  /**< [  4:  4](R/W) Disable global sync timeout. */
        uint64_t disldwb               : 1;  /**< [  3:  3](R/W) Suppresses the DWB functionality of any received LDWB, effectively turning them into LDTs. */
        uint64_t dissblkdty            : 1;  /**< [  2:  2](R/W) Disable bandwidth optimization between L2 and LMC and MOB which only transfers modified
                                                                 sub-blocks when possible. In an CCPI system all nodes must use the same setting of
                                                                 DISSBLKDTY or operation is undefined. */
        uint64_t disecc                : 1;  /**< [  1:  1](R/W) Tag and data ECC disable. */
        uint64_t disidxalias           : 1;  /**< [  0:  0](R/W) Index alias disable. */
#else /* Word 0 - Little Endian */
        uint64_t disidxalias           : 1;  /**< [  0:  0](R/W) Index alias disable. */
        uint64_t disecc                : 1;  /**< [  1:  1](R/W) Tag and data ECC disable. */
        uint64_t dissblkdty            : 1;  /**< [  2:  2](R/W) Disable bandwidth optimization between L2 and LMC and MOB which only transfers modified
                                                                 sub-blocks when possible. In an CCPI system all nodes must use the same setting of
                                                                 DISSBLKDTY or operation is undefined. */
        uint64_t disldwb               : 1;  /**< [  3:  3](R/W) Suppresses the DWB functionality of any received LDWB, effectively turning them into LDTs. */
        uint64_t disgsyncto            : 1;  /**< [  4:  4](R/W) Disable global sync timeout. */
        uint64_t disdwb                : 1;  /**< [  5:  5](R/W) Suppresses DWB and INVL2 commands, effectively turning them into NOPs.
                                                                 Internal:
                                                                 The real behavior is DWB and INVL2 commands are forced to look like STGL2I commands with
                                                                 DISSTGL2I set. */
        uint64_t rdf_cnt               : 8;  /**< [ 13:  6](R/W) Defines the sample point of the LMC response data in the DDR-clock/core-clock crossing.
                                                                 For optimal performance set to
                                                                 10 * (DDR-clock period/core-clock period) - 1.
                                                                 To disable set to 0. All other values are reserved. */
        uint64_t xmc_arb_mode          : 1;  /**< [ 14: 14](R/W) Arbitration mode for ADD bus QOS queues. 0 = fully determined through QOS, 1 = QOS0
                                                                 highest priority; QOS 1-7 use normal mode. */
        uint64_t rsp_arb_mode          : 1;  /**< [ 15: 15](R/W) Arbitration mode for RSC/RSD bus. 0 = round-robin; 1 = static priority.
                                                                 1. IOR data.
                                                                 2. STIN/FILLs.
                                                                 3. STDN/SCDN/SCFL. */
        uint64_t reserved_16_23        : 8;
        uint64_t discclk               : 1;  /**< [ 24: 24](R/W) Disable conditional clocking in L2C PNR blocks. */
        uint64_t reserved_25_26        : 2;
        uint64_t disstgl2i             : 1;  /**< [ 27: 27](R/W) Disable STGL2Is from changing the tags. */
        uint64_t reserved_28           : 1;
        uint64_t ocla_qos              : 3;  /**< [ 31: 29](R/W) QOS level for the transactions from OCLA to L2C. */
        uint64_t reserved_32_63        : 32;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_ctl_s cn; */
};
typedef union cavm_l2c_ctl cavm_l2c_ctl_t;

#define CAVM_L2C_CTL CAVM_L2C_CTL_FUNC()
static inline uint64_t CAVM_L2C_CTL_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_CTL_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_CN8XXX))
        return 0x87e080800000ll;
    __cavm_csr_fatal("L2C_CTL", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_CTL cavm_l2c_ctl_t
#define bustype_CAVM_L2C_CTL CSR_TYPE_RSL
#define basename_CAVM_L2C_CTL "L2C_CTL"
#define device_bar_CAVM_L2C_CTL 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_CTL 0
#define arguments_CAVM_L2C_CTL -1,-1,-1,-1

/**
 * Register (RSL) l2c_ecc_ctl
 *
 * L2C ECC Control Register
 * Flip ECC bits to generate single-bit or double-bit ECC errors in all instances of a given
 * memory type. Encodings are as follows.
 * 0x0 = No error.
 * 0x1 = Single-bit error on ECC\<0\>.
 * 0x2 = Single-bit error on ECC\<1\>.
 * 0x3 = Double-bit error on ECC\<1:0\>.
 *
 * L2DFLIP allows software to generate L2DSBE, L2DDBE, VBFSBE, and VBFDBE errors for the purposes
 * of testing error handling code. When one (or both) of these bits are set, a PL2 that misses in
 * the L2 will fill with the appropriate error in the first two OWs of the fill. Software can
 * determine which OW pair gets the error by choosing the desired fill order (address\<6:5\>). A
 * PL2 that hits in the L2 will not inject any errors. Therefore sending a WBIL2 prior to the PL2
 * is recommended to make a miss likely. (If multiple processors are involved, software must be
 * sure that no other processor or I/O device can bring the block into the L2).
 *
 * To generate a VBFSBE or VBFDBE, software must first get the cache block into the cache with an
 * error using a PL2 that misses the L2. Then a store partial to a portion of the cache block
 * without the error must change the block to dirty. Then, a subsequent WBL2/WBIL2/victim will
 * trigger the VBFSBE/VBFDBE error.
 */
union cavm_l2c_ecc_ctl
{
    uint64_t u;
    struct cavm_l2c_ecc_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_12_63        : 52;
        uint64_t mibflip               : 2;  /**< [ 11: 10](R/W) Generate an ECC error in the MIB. See note above. */
        uint64_t l2dflip               : 2;  /**< [  9:  8](R/W) Generate an ECC error in the L2D. See note above. */
        uint64_t l2tflip               : 2;  /**< [  7:  6](R/W) Generate an ECC error in the L2T. */
        uint64_t rdfflip               : 2;  /**< [  5:  4](R/W) Generate an ECC error in RDF memory. */
        uint64_t xmdflip               : 2;  /**< [  3:  2](R/W) Generate an ECC error in all corresponding CBC XMD memories. */
        uint64_t reserved_0_1          : 2;
#else /* Word 0 - Little Endian */
        uint64_t reserved_0_1          : 2;
        uint64_t xmdflip               : 2;  /**< [  3:  2](R/W) Generate an ECC error in all corresponding CBC XMD memories. */
        uint64_t rdfflip               : 2;  /**< [  5:  4](R/W) Generate an ECC error in RDF memory. */
        uint64_t l2tflip               : 2;  /**< [  7:  6](R/W) Generate an ECC error in the L2T. */
        uint64_t l2dflip               : 2;  /**< [  9:  8](R/W) Generate an ECC error in the L2D. See note above. */
        uint64_t mibflip               : 2;  /**< [ 11: 10](R/W) Generate an ECC error in the MIB. See note above. */
        uint64_t reserved_12_63        : 52;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_ecc_ctl_s cn; */
};
typedef union cavm_l2c_ecc_ctl cavm_l2c_ecc_ctl_t;

#define CAVM_L2C_ECC_CTL CAVM_L2C_ECC_CTL_FUNC()
static inline uint64_t CAVM_L2C_ECC_CTL_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_ECC_CTL_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_CN8XXX))
        return 0x87e080800010ll;
    __cavm_csr_fatal("L2C_ECC_CTL", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_ECC_CTL cavm_l2c_ecc_ctl_t
#define bustype_CAVM_L2C_ECC_CTL CSR_TYPE_RSL
#define basename_CAVM_L2C_ECC_CTL "L2C_ECC_CTL"
#define device_bar_CAVM_L2C_ECC_CTL 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_ECC_CTL 0
#define arguments_CAVM_L2C_ECC_CTL -1,-1,-1,-1

/**
 * Register (RSL) l2c_oci_ctl
 *
 * L2C CCPI Control Register
 */
union cavm_l2c_oci_ctl
{
    uint64_t u;
    struct cavm_l2c_oci_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_31_63        : 33;
        uint64_t ncpend                : 1;  /**< [ 30: 30](RO/H) An indication that a node change is pending.  Hardware sets this bit when
                                                                 OCX_COM_NODE[ID] is changed and clears the bit when the node change has taken
                                                                 effect. */
        uint64_t lock_local_cas        : 1;  /**< [ 29: 29](RO) Reserved. */
        uint64_t lock_local_stc        : 1;  /**< [ 28: 28](RO) Reserved. */
        uint64_t lock_local_pp         : 1;  /**< [ 27: 27](RO) Reserved. */
        uint64_t lngtolen              : 5;  /**< [ 26: 22](R/W) Reserved.
                                                                 Internal:
                                                                 This only controls the GSYNC timeout in the L2C_CBCs in non-OCI chips. */
        uint64_t shtolen               : 5;  /**< [ 21: 17](RO) Reserved. */
        uint64_t shtoioen              : 1;  /**< [ 16: 16](RO) Reserved. */
        uint64_t shtoen                : 3;  /**< [ 15: 13](RO) Reserved. */
        uint64_t shto                  : 1;  /**< [ 12: 12](RO) Reserved. */
        uint64_t inv_mode              : 2;  /**< [ 11: 10](RO) Reserved. */
        uint64_t cas_fdx               : 1;  /**< [  9:  9](RO) Reserved. */
        uint64_t rldd_psha             : 1;  /**< [  8:  8](RO) Reserved. */
        uint64_t lock_local_iob        : 1;  /**< [  7:  7](RO) Reserved. */
        uint64_t iofrcl                : 1;  /**< [  6:  6](RO) Reserved. */
        uint64_t reserved_4_5          : 2;
        uint64_t enaoci                : 4;  /**< [  3:  0](RO) CCPI is not present. Any attempt to enable it will be ignored. */
#else /* Word 0 - Little Endian */
        uint64_t enaoci                : 4;  /**< [  3:  0](RO) CCPI is not present. Any attempt to enable it will be ignored. */
        uint64_t reserved_4_5          : 2;
        uint64_t iofrcl                : 1;  /**< [  6:  6](RO) Reserved. */
        uint64_t lock_local_iob        : 1;  /**< [  7:  7](RO) Reserved. */
        uint64_t rldd_psha             : 1;  /**< [  8:  8](RO) Reserved. */
        uint64_t cas_fdx               : 1;  /**< [  9:  9](RO) Reserved. */
        uint64_t inv_mode              : 2;  /**< [ 11: 10](RO) Reserved. */
        uint64_t shto                  : 1;  /**< [ 12: 12](RO) Reserved. */
        uint64_t shtoen                : 3;  /**< [ 15: 13](RO) Reserved. */
        uint64_t shtoioen              : 1;  /**< [ 16: 16](RO) Reserved. */
        uint64_t shtolen               : 5;  /**< [ 21: 17](RO) Reserved. */
        uint64_t lngtolen              : 5;  /**< [ 26: 22](R/W) Reserved.
                                                                 Internal:
                                                                 This only controls the GSYNC timeout in the L2C_CBCs in non-OCI chips. */
        uint64_t lock_local_pp         : 1;  /**< [ 27: 27](RO) Reserved. */
        uint64_t lock_local_stc        : 1;  /**< [ 28: 28](RO) Reserved. */
        uint64_t lock_local_cas        : 1;  /**< [ 29: 29](RO) Reserved. */
        uint64_t ncpend                : 1;  /**< [ 30: 30](RO/H) An indication that a node change is pending.  Hardware sets this bit when
                                                                 OCX_COM_NODE[ID] is changed and clears the bit when the node change has taken
                                                                 effect. */
        uint64_t reserved_31_63        : 33;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_oci_ctl_s cn; */
};
typedef union cavm_l2c_oci_ctl cavm_l2c_oci_ctl_t;

#define CAVM_L2C_OCI_CTL CAVM_L2C_OCI_CTL_FUNC()
static inline uint64_t CAVM_L2C_OCI_CTL_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_OCI_CTL_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_CN8XXX))
        return 0x87e080800020ll;
    __cavm_csr_fatal("L2C_OCI_CTL", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_OCI_CTL cavm_l2c_oci_ctl_t
#define bustype_CAVM_L2C_OCI_CTL CSR_TYPE_RSL
#define basename_CAVM_L2C_OCI_CTL "L2C_OCI_CTL"
#define device_bar_CAVM_L2C_OCI_CTL 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_OCI_CTL 0
#define arguments_CAVM_L2C_OCI_CTL -1,-1,-1,-1

/**
 * Register (RSL) l2c_qos_pp#
 *
 * L2C Core QOS Level Registers
 */
union cavm_l2c_qos_ppx
{
    uint64_t u;
    struct cavm_l2c_qos_ppx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_3_63         : 61;
        uint64_t lvl                   : 3;  /**< [  2:  0](R/W) QOS level to use for this core. */
#else /* Word 0 - Little Endian */
        uint64_t lvl                   : 3;  /**< [  2:  0](R/W) QOS level to use for this core. */
        uint64_t reserved_3_63         : 61;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_qos_ppx_s cn; */
};
typedef union cavm_l2c_qos_ppx cavm_l2c_qos_ppx_t;

static inline uint64_t CAVM_L2C_QOS_PPX(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_QOS_PPX(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a<=3))
        return 0x87e080880000ll + 8ll * ((a) & 0x3);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=23))
        return 0x87e080880000ll + 8ll * ((a) & 0x1f);
    __cavm_csr_fatal("L2C_QOS_PPX", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_QOS_PPX(a) cavm_l2c_qos_ppx_t
#define bustype_CAVM_L2C_QOS_PPX(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_QOS_PPX(a) "L2C_QOS_PPX"
#define device_bar_CAVM_L2C_QOS_PPX(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_QOS_PPX(a) (a)
#define arguments_CAVM_L2C_QOS_PPX(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_qos_wgt
 *
 * L2C QOS Weight Register
 */
union cavm_l2c_qos_wgt
{
    uint64_t u;
    struct cavm_l2c_qos_wgt_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t wgt7                  : 8;  /**< [ 63: 56](R/W) Weight for QOS level 7. */
        uint64_t wgt6                  : 8;  /**< [ 55: 48](R/W) Weight for QOS level 6. */
        uint64_t wgt5                  : 8;  /**< [ 47: 40](R/W) Weight for QOS level 5. */
        uint64_t wgt4                  : 8;  /**< [ 39: 32](R/W) Weight for QOS level 4. */
        uint64_t wgt3                  : 8;  /**< [ 31: 24](R/W) Weight for QOS level 3. */
        uint64_t wgt2                  : 8;  /**< [ 23: 16](R/W) Weight for QOS level 2. */
        uint64_t wgt1                  : 8;  /**< [ 15:  8](R/W) Weight for QOS level 1. */
        uint64_t wgt0                  : 8;  /**< [  7:  0](R/W) Weight for QOS level 0. */
#else /* Word 0 - Little Endian */
        uint64_t wgt0                  : 8;  /**< [  7:  0](R/W) Weight for QOS level 0. */
        uint64_t wgt1                  : 8;  /**< [ 15:  8](R/W) Weight for QOS level 1. */
        uint64_t wgt2                  : 8;  /**< [ 23: 16](R/W) Weight for QOS level 2. */
        uint64_t wgt3                  : 8;  /**< [ 31: 24](R/W) Weight for QOS level 3. */
        uint64_t wgt4                  : 8;  /**< [ 39: 32](R/W) Weight for QOS level 4. */
        uint64_t wgt5                  : 8;  /**< [ 47: 40](R/W) Weight for QOS level 5. */
        uint64_t wgt6                  : 8;  /**< [ 55: 48](R/W) Weight for QOS level 6. */
        uint64_t wgt7                  : 8;  /**< [ 63: 56](R/W) Weight for QOS level 7. */
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_qos_wgt_s cn; */
};
typedef union cavm_l2c_qos_wgt cavm_l2c_qos_wgt_t;

#define CAVM_L2C_QOS_WGT CAVM_L2C_QOS_WGT_FUNC()
static inline uint64_t CAVM_L2C_QOS_WGT_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_QOS_WGT_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_CN8XXX))
        return 0x87e080800008ll;
    __cavm_csr_fatal("L2C_QOS_WGT", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_QOS_WGT cavm_l2c_qos_wgt_t
#define bustype_CAVM_L2C_QOS_WGT CSR_TYPE_RSL
#define basename_CAVM_L2C_QOS_WGT "L2C_QOS_WGT"
#define device_bar_CAVM_L2C_QOS_WGT 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_QOS_WGT 0
#define arguments_CAVM_L2C_QOS_WGT -1,-1,-1,-1

/**
 * Register (RSL) l2c_tad_ctl
 *
 * L2C TAD Control Register
 * In CNXXXX, [MAXLFB], EXLRQ, EXRRQ, EXFWD, EXVIC refer to half-TAD
 * LFBs/VABs. Therefore, even though there are 32 LFBs/VABs in a full TAD, the number
 * applies to both halves.
 * * If [MAXLFB] is != 0, [VBF_THRESH] should be less than [MAXLFB].
 * * If [MAXVBF] is != 0, [VBF_THRESH] should be less than [MAXVBF].
 * * If [MAXLFB] != 0, EXLRQ + EXRRQ + EXFWD + EXVIC must be less than or equal to [MAXLFB] - 4.
 * * If [MAXLFB] == 0, EXLRQ + EXRRQ + EXFWD + EXVIC must be less than or equal to 12.
 */
union cavm_l2c_tad_ctl
{
    uint64_t u;
    struct cavm_l2c_tad_ctl_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_33_63        : 31;
        uint64_t frcnalc               : 1;  /**< [ 32: 32](R/W) When set, all cache accesses are forced to not allocate in the local L2. */
        uint64_t disrstp               : 1;  /**< [ 31: 31](RO) Reserved. */
        uint64_t wtlmcwrdn             : 1;  /**< [ 30: 30](R/W) Be more conservative with LFB done relative to LMC writes. */
        uint64_t wtinvdn               : 1;  /**< [ 29: 29](R/W) Be more conservative with LFB done relative to invalidates. */
        uint64_t wtfilldn              : 1;  /**< [ 28: 28](R/W) Be more conservative with LFB done relative to fills. */
        uint64_t exlrq                 : 4;  /**< [ 27: 24](RO) Reserved. */
        uint64_t exrrq                 : 4;  /**< [ 23: 20](RO) Reserved. */
        uint64_t exfwd                 : 4;  /**< [ 19: 16](RO) Reserved. */
        uint64_t exvic                 : 4;  /**< [ 15: 12](RO) Reserved. */
        uint64_t vbf_thresh            : 4;  /**< [ 11:  8](R/W) VBF threshold. When the number of in-use VBFs exceeds this number the L2C TAD increases
                                                                 the priority of all its write operations in the LMC. */
        uint64_t maxvbf                : 4;  /**< [  7:  4](R/W) Maximum VBFs in use at once (0 means 16, 1-15 as expected). */
        uint64_t maxlfb                : 4;  /**< [  3:  0](R/W) Maximum VABs/LFBs in use at once (0 means 16, 1-15 as expected). */
#else /* Word 0 - Little Endian */
        uint64_t maxlfb                : 4;  /**< [  3:  0](R/W) Maximum VABs/LFBs in use at once (0 means 16, 1-15 as expected). */
        uint64_t maxvbf                : 4;  /**< [  7:  4](R/W) Maximum VBFs in use at once (0 means 16, 1-15 as expected). */
        uint64_t vbf_thresh            : 4;  /**< [ 11:  8](R/W) VBF threshold. When the number of in-use VBFs exceeds this number the L2C TAD increases
                                                                 the priority of all its write operations in the LMC. */
        uint64_t exvic                 : 4;  /**< [ 15: 12](RO) Reserved. */
        uint64_t exfwd                 : 4;  /**< [ 19: 16](RO) Reserved. */
        uint64_t exrrq                 : 4;  /**< [ 23: 20](RO) Reserved. */
        uint64_t exlrq                 : 4;  /**< [ 27: 24](RO) Reserved. */
        uint64_t wtfilldn              : 1;  /**< [ 28: 28](R/W) Be more conservative with LFB done relative to fills. */
        uint64_t wtinvdn               : 1;  /**< [ 29: 29](R/W) Be more conservative with LFB done relative to invalidates. */
        uint64_t wtlmcwrdn             : 1;  /**< [ 30: 30](R/W) Be more conservative with LFB done relative to LMC writes. */
        uint64_t disrstp               : 1;  /**< [ 31: 31](RO) Reserved. */
        uint64_t frcnalc               : 1;  /**< [ 32: 32](R/W) When set, all cache accesses are forced to not allocate in the local L2. */
        uint64_t reserved_33_63        : 31;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_tad_ctl_s cn; */
};
typedef union cavm_l2c_tad_ctl cavm_l2c_tad_ctl_t;

#define CAVM_L2C_TAD_CTL CAVM_L2C_TAD_CTL_FUNC()
static inline uint64_t CAVM_L2C_TAD_CTL_FUNC(void) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_TAD_CTL_FUNC(void)
{
    if (cavm_is_model(OCTEONTX_CN8XXX))
        return 0x87e080800018ll;
    __cavm_csr_fatal("L2C_TAD_CTL", 0, 0, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_TAD_CTL cavm_l2c_tad_ctl_t
#define bustype_CAVM_L2C_TAD_CTL CSR_TYPE_RSL
#define basename_CAVM_L2C_TAD_CTL "L2C_TAD_CTL"
#define device_bar_CAVM_L2C_TAD_CTL 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_TAD_CTL 0
#define arguments_CAVM_L2C_TAD_CTL -1,-1,-1,-1

/**
 * Register (RSL) l2c_wpar_iob#
 *
 * L2C IOB Way Partitioning Registers
 */
union cavm_l2c_wpar_iobx
{
    uint64_t u;
    struct cavm_l2c_wpar_iobx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_16_63        : 48;
        uint64_t mask                  : 16; /**< [ 15:  0](R/W/H) Way partitioning mask (1 means do not use). The read value of [MASK] includes bits set
                                                                 because of the L2C cripple fuses. */
#else /* Word 0 - Little Endian */
        uint64_t mask                  : 16; /**< [ 15:  0](R/W/H) Way partitioning mask (1 means do not use). The read value of [MASK] includes bits set
                                                                 because of the L2C cripple fuses. */
        uint64_t reserved_16_63        : 48;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_wpar_iobx_s cn; */
};
typedef union cavm_l2c_wpar_iobx cavm_l2c_wpar_iobx_t;

static inline uint64_t CAVM_L2C_WPAR_IOBX(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_WPAR_IOBX(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN8XXX) && (a<=15))
        return 0x87e080840200ll + 8ll * ((a) & 0xf);
    __cavm_csr_fatal("L2C_WPAR_IOBX", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_WPAR_IOBX(a) cavm_l2c_wpar_iobx_t
#define bustype_CAVM_L2C_WPAR_IOBX(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_WPAR_IOBX(a) "L2C_WPAR_IOBX"
#define device_bar_CAVM_L2C_WPAR_IOBX(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_WPAR_IOBX(a) (a)
#define arguments_CAVM_L2C_WPAR_IOBX(a) (a),-1,-1,-1

/**
 * Register (RSL) l2c_wpar_pp#
 *
 * L2C Core Way Partitioning Registers
 */
union cavm_l2c_wpar_ppx
{
    uint64_t u;
    struct cavm_l2c_wpar_ppx_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t reserved_16_63        : 48;
        uint64_t mask                  : 16; /**< [ 15:  0](R/W/H) Way partitioning mask (1 means do not use). The read value of [MASK] includes bits set
                                                                 because of the L2C cripple fuses. */
#else /* Word 0 - Little Endian */
        uint64_t mask                  : 16; /**< [ 15:  0](R/W/H) Way partitioning mask (1 means do not use). The read value of [MASK] includes bits set
                                                                 because of the L2C cripple fuses. */
        uint64_t reserved_16_63        : 48;
#endif /* Word 0 - End */
    } s;
    /* struct cavm_l2c_wpar_ppx_s cn; */
};
typedef union cavm_l2c_wpar_ppx cavm_l2c_wpar_ppx_t;

static inline uint64_t CAVM_L2C_WPAR_PPX(uint64_t a) __attribute__ ((pure, always_inline));
static inline uint64_t CAVM_L2C_WPAR_PPX(uint64_t a)
{
    if (cavm_is_model(OCTEONTX_CN81XX) && (a<=3))
        return 0x87e080840000ll + 8ll * ((a) & 0x3);
    if (cavm_is_model(OCTEONTX_CN83XX) && (a<=23))
        return 0x87e080840000ll + 8ll * ((a) & 0x1f);
    __cavm_csr_fatal("L2C_WPAR_PPX", 1, a, 0, 0, 0, 0, 0);
}

#define typedef_CAVM_L2C_WPAR_PPX(a) cavm_l2c_wpar_ppx_t
#define bustype_CAVM_L2C_WPAR_PPX(a) CSR_TYPE_RSL
#define basename_CAVM_L2C_WPAR_PPX(a) "L2C_WPAR_PPX"
#define device_bar_CAVM_L2C_WPAR_PPX(a) 0x0 /* PF_BAR0 */
#define busnum_CAVM_L2C_WPAR_PPX(a) (a)
#define arguments_CAVM_L2C_WPAR_PPX(a) (a),-1,-1,-1

#endif /* __CAVM_CSRS_L2C_H__ */
