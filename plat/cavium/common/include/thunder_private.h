/** @file

    Copyright (c) 2016-2017, Cavium Inc. All rights reserved.<BR>
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution.  The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.php

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __THUNDER_PRIVATE_H
#define __THUNDER_PRIVATE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "cavm-arch.h"
#pragma GCC diagnostic pop

#include <arch_helpers.h>
#include <stddef.h>

#undef AP_CVMCTL_EL1
#undef AP_CVM_ACCESS_EL1
#undef AP_CVMMEMCTL0_EL1
#undef AP_CVMMEMCTL1_EL1
#undef AP_CVM_ACCESS_EL2
#undef AP_CVM_ACCESS_EL3

#define AP_CVMCTL_EL1		S3_0_C11_C0_0
#define AP_CVM_ACCESS_EL1	S3_0_C11_C0_3
#define AP_CVMMEMCTL0_EL1	S3_0_C11_C0_4
#define AP_CVMMEMCTL1_EL1	S3_0_C11_C0_5
#define AP_CVM_ACCESS_EL2	S3_4_C11_C0_3
#define AP_CVM_ACCESS_EL3	S3_6_C11_C0_3

DEFINE_RENAME_SYSREG_RW_FUNCS(cvmctl_el1, AP_CVMCTL_EL1)
DEFINE_RENAME_SYSREG_RW_FUNCS(cvmmemctl0_el1, AP_CVMMEMCTL0_EL1)
DEFINE_RENAME_SYSREG_RW_FUNCS(cvmmemctl1_el1, AP_CVMMEMCTL1_EL1)
DEFINE_RENAME_SYSREG_RW_FUNCS(cvm_access_el1, AP_CVM_ACCESS_EL1)
DEFINE_RENAME_SYSREG_RW_FUNCS(cvm_access_el2, AP_CVM_ACCESS_EL2)
DEFINE_RENAME_SYSREG_RW_FUNCS(cvm_access_el3, AP_CVM_ACCESS_EL3)

#define IS_THUNDER_PASS(midr, partnum, hi, low)				\
	((((midr) >> MIDR_PN_SHIFT & MIDR_PN_MASK) == (partnum)) &&	\
	 (((midr) >> MIDR_VAR_SHIFT & MIDR_VAR_MASK) == (hi) - 1) &&	\
	 (((midr) >> MIDR_REV_SHIFT & MIDR_REV_MASK) == (low)))

/* In Mhz */
#define THUNDER_SYSCNT_FREQ	100ull

void octeontx_pci_init(void);
void plat_add_mmio_map(void);
void thunder_io_setup(void);
void thunder_security_setup(void);
unsigned thunder_get_node_count(void);
int thunder_dram_is_lmc_enabled(unsigned node, unsigned lmc);
uint64_t thunder_dram_size_node(unsigned node);
void thunder_cpu_setup(void);
extern void *fdt_ptr;
void thunder_gic_driver_init(void);
void thunder_gic_init(void);
unsigned int thunder_calc_core_pos(unsigned long mpidr);
void thunder_pci_init(void);
void thunder_signal_shutdown(void);
int thunder_pwrc_setup(void);
void thunder_twsi_send(unsigned int node, unsigned int twsi_num,
			uint16_t addr, const uint8_t *buffer, size_t size);
void sata_ipm_quirk(void);

int spi_nor_init(void);
int spi_nor_rw_data(int write, unsigned long addr, int size, void *buf, int buf_size);
int spi_nor_erase_sect(uint32_t addr);

void set_secondary_cpu_jump_addr(unsigned int bl1_base);

static uint8_t thunder_fuse_read_byte(int node, int byte_addr)
{
	union cavm_mio_fus_rcmd read_cmd;

	read_cmd.u = 0;
	read_cmd.s.addr = byte_addr;
	read_cmd.s.pend = 1;
	CSR_WRITE_PA(node, CAVM_MIO_FUS_RCMD, read_cmd.u);
	while ((read_cmd.u = CSR_READ_PA(node, CAVM_MIO_FUS_RCMD)) &&
		read_cmd.s.pend) ;

	return read_cmd.s.dat;
}

static inline int thunder_fuse_read(int node, int fuse)
{
	return (thunder_fuse_read_byte(node, fuse >> 3) >> (fuse & 0x7)) & 1;
}

#define THUNDER_IRQ_MASKED	(1ull << 32)

/* Secure timer IRQ */
#define THUNDER_IRQ_SEC_PHY_TIMER 0x1d

/* GPIO IRQs. ATF receives S_IRQ and then sends NS_IRQ to NS world. */
#define THUNDER_IRQ_GPIO_PWR_NS		0x20
#define THUNDER_IRQ_GPIO_PWR_S		0x21

/* TWSI Secure interrupt for BMC events */
#define THUNDER_IRQ_TWSI_BMC_S		0x22

/*
 * 1 IRQs per UAA
 * 4 UAAs (2 per node)
 * 0x2 (2) IRQs used (shared between nodes)
 */
#define THUNDER_IRQ_UAA0	0x25
#define THUNDER_IRQ_UAA1	0x26

/*
 * 4 IRQs per PEM (INTA, INTB, INTC, INTD)
 * 12 PEMs (6 per node)
 * 0x30 (48) IRQs used
 * Next spare IRQ = 0x60
 */
#define THUNDER_IRQ_PEM_BASE	0x30

/*
 * 2 IRQs per SMMU (GLOBAL, CONTEXT)
 * 8 SMMUS (4 per node)
 * 0x10 (16) IRQs used
 * Next spare IRQ = 0x74
 */
#define THUNDER_IRQ_SMMU_BASE	0x64


/*
 * watchdog GTI irqs assigned from here
 */
#define THUNDER_IRQ_GTI_WDOG	0x74

#endif
