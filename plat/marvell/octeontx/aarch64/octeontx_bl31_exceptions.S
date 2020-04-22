/*
 * Copyright (C) 2020 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <arch.h>
#include <asm_macros.S>
#include <context.h>

	.global	el3_exit_wfi
	.global el3_start_el0_isr
	.global el3_invoke_elx_kernel_callback

#define SPSR_ISR	((1ULL << SPSR_E_SHIFT) | (3 << SPSR_AIF_SHIFT))
#define SCR_ISR		(SCR_NS_BIT | SCR_TWE_BIT | SCR_TWI_BIT | SCR_RW_BIT)

func el3_exit_wfi

	/*discard recent LR */
	ldr	x0, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_LR_BCK]
	str	x0, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_LR]

	/* Restores EL0ISR sysregs */
	/* OS-ELmode was saved at CTX_GPREG_X6 of EL0ISR_CTX_GPREGS_OFFSET */
	mov	x0, xzr
	add	x0, sp, #EL0ISR_CTX_GPREGS_OFFSET
	ldr	x6, [x0, #CTX_GPREG_X6]

	mov x8, xzr
	add x8, sp, #EL0ISR_CTX_SYSREGS_OFFSET
	cmp x6, xzr
	beq 1f
	/* X6 == 1, OS is in EL2 mode */
	ldr x9, [x8, #EL0ISR_CTX_TCR_EL12]
	msr tcr_el2, x9
	ldr x9, [x8, #EL0ISR_CTX_TTBR0_EL12]
	msr ttbr0_el2, x9
	ldr x9, [x8, #EL0ISR_CTX_TTBR1_EL12]
	msr S3_4_C2_C0_1, x9
	b 2f
1:
	/* X6 == 0, OS is in EL2 mode */
	ldr x9, [x8, #EL0ISR_CTX_TCR_EL12]
	msr tcr_el1, x9
	ldr x9, [x8, #EL0ISR_CTX_TTBR0_EL12]
	msr ttbr0_el1, x9
	ldr x9, [x8, #EL0ISR_CTX_TTBR1_EL12]
	msr ttbr1_el1, x9
2:
	ldr x9, [x8, #EL0ISR_CTX_SCR_EL3]
	msr scr_el3, x9
	ldr x9, [x8, #EL0ISR_CTX_SPSR_EL3]
	msr spsr_el3, x9
	ldr x9, [x8, #EL0ISR_CTX_ELR_EL3]
	msr elr_el3, x9
	/*restore GP registers*/
	mov x0, xzr
	add x0, sp, #EL0ISR_CTX_GPREGS_OFFSET
	ldp	x4, x5, [x0, #CTX_GPREG_X4]
	ldp	x6, x7, [x0, #CTX_GPREG_X6]
	ldp	x8, x9, [x0, #CTX_GPREG_X8]
	ldp	x10, x11, [x0, #CTX_GPREG_X10]
	ldp	x12, x13, [x0, #CTX_GPREG_X12]
	ldp	x14, x15, [x0, #CTX_GPREG_X14]
	ldp	x18, x19, [x0, #CTX_GPREG_X18]
	ldp	x20, x21, [x0, #CTX_GPREG_X20]
	ldp	x22, x23, [x0, #CTX_GPREG_X22]
	ldp	x24, x25, [x0, #CTX_GPREG_X24]
	ldp	x26, x27, [x0, #CTX_GPREG_X26]
	ldp	x28, x29, [x0, #CTX_GPREG_X28]
	ldp	 x30, x17, [x0, #CTX_GPREG_LR]
	/* restore runtime stack */
	msr	sp_el0, x17
	nop
	ldp	x16, x17, [x0, #CTX_GPREG_X16]
	ldp	x2, x3, [x0, #CTX_GPREG_X2]
	ldp	x0, x1, [x0, #CTX_GPREG_X0]
	dsb sy
	msr spsel, #0
	dsb sy
	isb
	ret
endfunc el3_exit_wfi

func el3_start_el0_isr
	/* -------------------------------------------------------
	 * Stop El1 interrupts before jumping. This assumes no
	 * EL2 virtualization
	 * X0 holds the gpio_num,
	 * X1 holds sp
	 * X2 holds ttbr0
	 * X3 holds ttbr1
	 * X4 holds isr_base
	 * X5 holds tcr_el1
	 * X6 holds el_mode
	 * -------------------------------------------------------
	 */
	msr spsel, #1
	nop
	str x0, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_TMP_HANDLE]
	ldr	x0, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_LR]
	str	x0, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_LR_BCK]

	/* save GP registers */
	mov x0, xzr
	add x0, sp, #EL0ISR_CTX_GPREGS_OFFSET
	stp	x2, x3, [x0, #CTX_GPREG_X2]
	stp	x4, x5, [x0, #CTX_GPREG_X4]
	stp	x6, x7, [x0, #CTX_GPREG_X6]
	stp	x8, x9, [x0, #CTX_GPREG_X8]
	stp	x10, x11, [x0, #CTX_GPREG_X10]
	stp	x12, x13, [x0, #CTX_GPREG_X12]
	stp	x14, x15, [x0, #CTX_GPREG_X14]
	stp	x16, x17, [x0, #CTX_GPREG_X16]
	stp	x18, x19, [x0, #CTX_GPREG_X18]
	stp	x20, x21, [x0, #CTX_GPREG_X20]
	stp	x22, x23, [x0, #CTX_GPREG_X22]
	stp	x24, x25, [x0, #CTX_GPREG_X24]
	stp	x26, x27, [x0, #CTX_GPREG_X26]
	stp	x28, x29, [x0, #CTX_GPREG_X28]
	mrs	x18, sp_el0
	str	x18, [x0, #CTX_GPREG_SP_EL0]
	str	x30, [x0, #CTX_GPREG_LR]
	ldr	x0, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_TMP_HANDLE]
	mov x8, xzr
	add x8, sp, #EL0ISR_CTX_GPREGS_OFFSET
	stp	x0, x1, [x8, #CTX_GPREG_X0]
	/*save EL0ISR specific system registers */
	mov x8, xzr
	add x8, sp, #EL0ISR_CTX_SYSREGS_OFFSET
	/* X6 == 0, OS is in EL1 mode
	 * X6 == 1, OS is in EL2 mode */
	cmp	x6, xzr
	beq	1f
	mrs 	x9, tcr_el2
	str 	x9, [x8, #EL0ISR_CTX_TCR_EL12]
	mrs 	x9, ttbr0_el2
	str 	x9, [x8, #EL0ISR_CTX_TTBR0_EL12]
	mrs	x9, S3_4_C2_C0_1		 /* ttbr1_el2 */
	str	x9, [x8, #EL0ISR_CTX_TTBR1_EL12]
	b	2f
1:
	mrs x9, tcr_el1
	str x9, [x8, #EL0ISR_CTX_TCR_EL12]
	mrs x9, ttbr0_el1
	str x9, [x8, #EL0ISR_CTX_TTBR0_EL12]
	mrs x9, ttbr1_el1
	str x9, [x8, #EL0ISR_CTX_TTBR1_EL12]
2:
	mrs x9, scr_el3
	str x9, [x8, #EL0ISR_CTX_SCR_EL3]
	mrs x9, spsr_el3
	str x9, [x8, #EL0ISR_CTX_SPSR_EL3]
	mrs x9, elr_el3
	str x9, [x8, #EL0ISR_CTX_ELR_EL3]

	/*EL0 specific settings
	 * This will allow wfi in EL0 to cause async exception to EL3
	 */

	/* Set SCTLR_ELx.nTWI = 1
	 * Kernel uses SCTLR_EL1 for VHE as well.
	 * SCTLR_EL2.nTWI is reserved as of today but has been referred to in
	 * ARM ARM documents so setting there as well
	 */
	mrs	x10, sctlr_el1
	orr	x10, x10, #(1ULL << 16)
	msr	sctlr_el1, x10
	mrs	x10, sctlr_el2
	orr	x10, x10, #(1ULL << 16)
	msr	sctlr_el2, x10
	msr	sp_el0, x1

	msr sp_el0, x1
	nop
	cmp	x6, xzr
	beq	1f
	msr	tcr_el2, x5
	msr	ttbr0_el2, x2
	msr	S3_4_C2_C0_1, x3
	b	2f
1:
	msr tcr_el1, x5
	msr ttbr0_el1, x2
	msr ttbr1_el1, x3
2:
	ldr x2, =SCR_ISR
	ldr x3, =SPSR_ISR
	msr scr_el3, x2
	msr spsr_el3, x3
	msr elr_el3, x4
	/*clear all registers except x0(gpio_num)*/
	mov x1, xzr; mov x2, xzr; mov x3, xzr; mov x4, xzr;
	mov x5, xzr; mov x6, xzr; mov x7, xzr; mov x8, xzr;
	mov x9, xzr; mov x10, xzr; mov x11, xzr; mov x12, xzr;
	mov x13, xzr; mov x14, xzr; mov x15, xzr; mov x16, xzr;
	mov x17, xzr; mov x18, xzr; mov x19, xzr; mov x20, xzr;
	mov x21, xzr; mov x22, xzr; mov x23, xzr; mov x24, xzr;
	mov x25, xzr; mov x26, xzr; mov x27, xzr; mov x28, xzr;
	mov x29, xzr; mov x30, xzr;
	dsb sy
	isb
	before_eret
	eret
endfunc el3_start_el0_isr

func el3_invoke_elx_kernel_callback
	/*
	 * X0 holds the kernel callback
	 * X1 holds spsr_el3
	 * X2 holds scr_el3
	 */

	/*
	 * msr sp_el0, x1
	 * nop
	 */

	msr scr_el3, x2
	msr spsr_el3, x1
	msr elr_el3, x0

	msr spsel, #1
	nop

	ldp	x0, x1, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X0]
	ldp	x2, x3, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X2]
	ldp	x4, x5, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X4]
	ldp	x6, x7, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X6]
	ldp	x8, x9, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X8]
	ldp	x10, x11, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X10]
	ldp	x12, x13, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X12]
	ldp	x14, x15, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X14]
	ldp	x18, x19, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X18]
	ldp	x20, x21, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X20]
	ldp	x22, x23, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X22]
	ldp	x24, x25, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X24]
	ldp	x26, x27, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X26]
	ldp	x28, x29, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X28]
	ldp	x30, x17, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_LR]
	msr	sp_el0, x17
	ldp	x16, x17, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_X16]

	/* Can't access any kernel memory here as no kernel pagetables mapped */

	dsb sy
	isb
	before_eret
	eret

endfunc el3_invoke_elx_kernel_callback
