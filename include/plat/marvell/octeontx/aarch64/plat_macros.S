/*
 * Copyright (C) 2016-2017 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef PLAT_MACROS_S
#define PLAT_MACROS_S
#include <arch.h>
#include <drivers/arm/gic_common.h>
#include <drivers/arm/gicv3.h>
#include <platform_def.h>

#define CAVM_GICD_BASE	0x801000000000

.section .rodata.gic_reg_name, "aS"
gic_regs: .asciz "icc_ctlr", "icc_hppir0", "icc_hppir1", "icc_rpr_el1", "icc_ap0r0_el1", ""

gicd_pend_reg:
	.asciz "gicd_ispendr regs (Offsets 0x200-0x210)\nOffset\t\t\tValue\n"
newline:
	.asciz "\n"
spacer:
	.asciz ":\t\t 0x"
prefix:
	.asciz "0x"

	/* ---------------------------------------------
	 * The below macro prints out relevant GIC
	 * registers whenever an unhandled exception is
	 * taken in BL31.
	 * ---------------------------------------------
	 */
	.macro plat_crash_print_regs
	mrs	x8, ICC_CTLR_EL3
	mrs	x9, ICC_HPPIR0_EL1
	mrs	x10, ICC_HPPIR1_EL1
	mrs     x11, ICC_RPR_EL1
	mrs     x12, ICC_AP0R0_El1
	adr	x6, gic_regs
	bl	str_in_crash_buf_print
	b       print_gic_common
print_gic_common:
	ldr	x16, =CAVM_GICD_BASE
	/* Print the GICD_ISPENDR regs */
	add	x7, x16, #GICD_ISPENDR
	adr	x4, gicd_pend_reg
	bl	asm_print_str
gicd_ispendr_loop:
	sub	x4, x7, x16
	cmp	x4, #0x214
	b.eq	exit_print_gic_regs

	/* Print "0x" */
	adr	x4, prefix
	bl	asm_print_str

	/* Print offset */
	sub	x4, x7, x16
	mov	x5, #12
	bl	asm_print_hex_bits

	adr	x4, spacer
	bl	asm_print_str

	ldr	w4, [x7], #4
	bl	asm_print_hex

	adr	x4, newline
	bl	asm_print_str
	b	gicd_ispendr_loop
exit_print_gic_regs:
	.endm

/* ------------------------------------------------
 * The below required platform porting macro prints
 * out relevant interconnect registers whenever an
 * unhandled exception is taken in BL3-1.
  * ------------------------------------------------
 */
.macro plat_print_interconnect_regs
	nop
.endm
#endif /* PLAT_MACROS_S */
