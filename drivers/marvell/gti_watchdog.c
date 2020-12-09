/*
 * Copyright (C) 2016-2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

/* GTI Watchdog driver for 96xx */

#include <arch_helpers.h>
#include <assert.h>
#include <debug.h>
#include <lib/psci/psci.h>
#include <errno.h>
#include <drivers/arm/gic_common.h>
#include <drivers/arm/gicv3.h>
#include <plat/common/platform.h>
#include <platform_def.h>
#include <plat_scfg.h>
#include <runtime_svc.h>
#include <bl31/interrupt_mgmt.h>
#include <lib/el3_runtime/context_mgmt.h>
#include <string.h>

#include <octeontx_ecam.h>
#include <octeontx_ehf.h>
#include <octeontx_svc.h>
#include <octeontx_common.h>
#include <octeontx_utils.h>
#include <octeontx_irqs_def.h>
#include <gti_watchdog.h>

#include "cavm-csrs-gti.h"
#include "cavm-csrs-rst.h"

#undef DEBUG_GTI_WDOG

#ifdef DEBUG_GTI_WDOG
#define debug_gti_watchdog printf
#else
#define debug_gti_watchdog(...) ((void) (0))
#endif

#define SPSR_ISR_EL2	SPSR_64(MODE_EL2, MODE_SP_ELX, 3)
#define SPSR_ISR_EL1	SPSR_64(MODE_EL1, MODE_SP_ELX, 3)
#define SCR_ISR		(SCR_NS_BIT | SCR_RW_BIT)

#define ROUND_DOWN(val, align)	((val) / (align) * (align))

void el3_invoke_elx_kernel_callback(uint64_t kernel_cback, uint64_t spsr_el3,
				    uint64_t scr_el3);
el3_state_t g_el3state_save;
gp_regs_t g_gpregs_save;
uint64_t g_el2_sp_save;
uint32_t g_intid;
_Atomic int in_use_lock;
/* Kernel nmi (watchdog) handler when executing userspace (el0) */
uint64_t g_kernel_cback_el0;
/* Kernel nmi (watchdog) handler when executing in kernel (el1 or el2) */
uint64_t g_kernel_cback_el1;
/* Kernel in hypervisor mode (el2), used for context setup on exception
 * simulation.
 */
_Bool g_kernel_hyp_mode;

/* Max two page per core, if ELx_ELR and ELx_SPSR at different page */
#define GTI_PAGES_PERCORE	2
#define GTI_MAX_PAGES	(GTI_PAGES_PERCORE * PLATFORM_CORE_COUNT)

uint64_t gti_pages[GTI_MAX_PAGES];
int git_page_idx;

struct gti_shared_addr_t {
	unsigned long *gti_addr_elr;
	unsigned long *gti_addr_spsr;
};

struct gti_shared_addr_t gti_shared_addr[GTI_MAX_PAGES];

#if defined ARM_TRACE_SECURE_BUFFER
extern void plat_armtrace_stop(void);
#endif

void prepare_elx_kernel_callback(int el_mode)
{
	uint64_t spsr;
#if 0
	/* For testing on ASIM only */
	CSR_WRITE(CAVM_GTI_CWD_WDOGX(plat_my_core_pos()), 0);
#endif

	spsr = g_kernel_hyp_mode ? SPSR_ISR_EL2 : SPSR_ISR_EL1;

	if (el_mode)
		el3_invoke_elx_kernel_callback(g_kernel_cback_el1, spsr,
					       SCR_ISR);
	else
		el3_invoke_elx_kernel_callback(g_kernel_cback_el0, spsr,
					       SCR_ISR);
}

int prepare_elx_restore_context(void)
{
	cpu_context_t *cm_ctxt;
	el3_state_t *el3state_ctx;
	gp_regs_t *gpregs_ctx;
	uint64_t val;
	int el_mode;
	int cpu;

	cm_ctxt = cm_get_context(NON_SECURE);

	gpregs_ctx = get_gpregs_ctx(cm_ctxt);
	el3state_ctx = get_el3state_ctx(cm_ctxt);

	/*
	 * Load EL3 saved elr_el1/2 and spsr_el1/2 sysregs to kernel
	 * shared respective location to simulate an exception return.
	 */

	cpu = plat_core_pos_by_mpidr(read_mpidr());
	val = read_ctx_reg((el3state_ctx), (uint32_t)(CTX_SPSR_EL3));
	*gti_shared_addr[cpu].gti_addr_spsr = val;
	el_mode = GET_EL(val);

	val = read_ctx_reg((el3state_ctx), (uint32_t)(CTX_ELR_EL3));
	*gti_shared_addr[cpu].gti_addr_elr = val;

	memcpy((void *) &g_gpregs_save, (void *)gpregs_ctx, sizeof(gp_regs_t));
	memcpy((void *) &g_el3state_save, (void *)el3state_ctx,
			 sizeof(el3_state_t));

	/*
	 * NOTE: SP_EL2 will be modified during kernel nmi callback invocation,
	 * hence, save it here and restore it back when we restore and resume
	 * interrupted context.
	 */

	__asm__ __volatile__ ("mrs %0, sp_el2\n\t" : "=r"(g_el2_sp_save) : : );

	return el_mode;
}


void restore_elx_context_and_return(void)
{
	cpu_context_t *cm_ctxt;
	el3_state_t *el3state_ctx;
	gp_regs_t *gpregs_ctx;

	/*
	 * We had shortcut the top-level interrupt handler path and invoked
	 * the kernel nmi callback via a simulated exception return to EL2
	 * from EL3, so when we return back we need to do interrupt completion.
	 */

	plat_ic_end_of_interrupt(g_intid);

	cm_ctxt = cm_get_context(NON_SECURE);

	gpregs_ctx = get_gpregs_ctx(cm_ctxt);
	el3state_ctx = get_el3state_ctx(cm_ctxt);

	memcpy(gpregs_ctx, (void *) &g_gpregs_save, sizeof(gp_regs_t));
	memcpy(el3state_ctx, (void *) &g_el3state_save, sizeof(el3_state_t));

	__asm__ __volatile__ ("msr sp_el2, %0\n\t" : : "r"(g_el2_sp_save));

	/*
	 * Ensure that the global save state is restored to the expired
	 * watchdog core's context before it gets overwritten by another
	 * watchdog expiration.
	 */
	in_use_lock--;
}

uint64_t gti_cwd_irq_handler(uint32_t id, uint32_t flags, void *cookie)
{
	int el_mode;

#if defined ARM_TRACE_SECURE_BUFFER
	/* Ensure we stop the ARM Trace, so that the trace data
	 * relevant to this watchdog interrupt doesn't get overwritten.
	 */
	plat_armtrace_stop();
#endif
	if ((in_use_lock++) == 0) {
		g_intid = id;
		el_mode = prepare_elx_restore_context();
		prepare_elx_kernel_callback(el_mode);
		/* Does not return here */
	}
	in_use_lock--;
	return 0;
}

#define PLL_REF_CLK 50000000	/* 50 MHz */

/**
 * Setup the watchdog to expire in timeout_ms milliseconds. We are programming
 * the watchdog in Interrupt + DEL3T trap + Soft-reset mode, out of which
 * we ignore the Interrupt.
 *
 * Since we want a DEL3T trap first, we actually program the watchdog to
 * expire at the timeout / 2.
 *
 */
static void gti_watchdog_set(uint64_t timeout_ms, uint64_t cores)
{
	if (timeout_ms > 0) {
		uint64_t sclk;
		union cavm_rst_boot rst_boot;
		union cavm_gti_cwd_wdogx wdog;
		cavm_gti_cwd_int_ena_set_t gti_cwd_ena;
		static int intr_hndlrs_registered;
		int i, rc;

		rst_boot.u = CSR_READ(CAVM_RST_BOOT);
		sclk = PLL_REF_CLK * rst_boot.s.pnr_mul;
		uint64_t timeout_sclk = sclk * timeout_ms / 1000;

		/* Watchdog counts in 1024 cycle steps */
		uint64_t timeout_wdog = timeout_sclk >> 10;
		/*
		 * We can only specify the upper 16 bits of a 24 bit value.
		 * Round up
		 */
		timeout_wdog = (timeout_wdog + 0xff) >> 8;
		/* If the timeout overflows the hardware limit, set max */
		if (timeout_wdog >= 0x10000) {
			debug_gti_watchdog("Watchdog: Timer count overflow!\n");
			timeout_wdog = 0xffff;
		}

		gti_cwd_ena.s.core = cores;
		CSR_WRITE(CAVM_GTI_CWD_INT_ENA_SET, gti_cwd_ena.u);

		debug_gti_watchdog("Watchdog: gti ena set = 0x%llx\n",
			CSR_READ(CAVM_GTI_CWD_INT_ENA_SET));

		/* configure interrupt vectors */

		for (i = 0; i < GTI_CWD_SPI_IRQS; i++) {

			if (!(cores & (1 << i)))
				continue;

			plat_gti_irq_setup(i);
		}

		if (intr_hndlrs_registered)
			goto intr_hndlrs_already_registered;
		else
			intr_hndlrs_registered = 1;

		for (i = 0; i < GTI_CWD_SPI_IRQS; i++) {
			rc = octeontx_ehf_register_irq_handler(
							GTI_CWD_SPI_IRQ(i),
							gti_cwd_irq_handler);
			if (rc) {
				ERROR("err %d register CWD secure interrupt\n",
							 rc);
				return;
			}
		}
intr_hndlrs_already_registered:

		plat_gti_access_secure_memory_setup(1);

		debug_gti_watchdog("Watchdog: Set to expire %llu SCLK cycles\n",
					timeout_wdog << 18);

		wdog.u = 0;
		wdog.s.len = timeout_wdog;
		wdog.s.cnt = timeout_wdog << 8;
		/*
		 * setup watchdog to interrupt + del3t + reset mode.
		 * DEL3T is not enabled currently.
		 */
		wdog.s.mode = 3;

		for (i = 0; i < GTI_CWD_SPI_IRQS; i++) {
			if (!(cores & (1 << i)))
				continue;
			CSR_WRITE(CAVM_GTI_CWD_WDOGX(i), wdog.u);
			CSR_WRITE(CAVM_GTI_CWD_POKEX(i), 0);
		}
	}
}

/**
 * Disable the hardware watchdog
 */
void gti_watchdog_disable(void)
{
	for (int i = 0; i < GTI_CWD_SPI_IRQS; i++) {
		if (gti_watchdog_is_running(i)) {
			debug_gti_watchdog("Disabling watchdog on core %d\n",
					 i);
			CSR_WRITE(CAVM_GTI_CWD_WDOGX(i), 0);
		}
	}

	for (int i = 0; i < GTI_MAX_PAGES; i++) {
		if (gti_pages[i] == 0)
			continue;

		mmap_remove_dynamic_region(gti_pages[i], PAGE_SIZE);
		gti_pages[i] = 0;
	}
	git_page_idx = 0;

	debug_gti_watchdog("Watchdog: Disabled\n");
}

/**
 * Return true if the watchdog is configured and running
 *
 * @return Non-zero if watchdog is running
 */
int gti_watchdog_is_running(int core)
{
	union cavm_gti_cwd_wdogx wdog;

	wdog.u = CSR_READ(CAVM_GTI_CWD_WDOGX(core));
	return wdog.s.mode != 0;
}

int gti_wdog_remove_handler(void)
{
	gti_watchdog_disable();

	return 1;
}

static int gti_wdog_map_info_page(uint64_t phys_addr)
{
	uint64_t align_addr = ROUND_DOWN(phys_addr, PAGE_SIZE);
	int ret = -1;
	int i;

	debug_gti_watchdog("GTI: MAP physical address = %llx\n", phys_addr);

	/* Check if page already mapped */
	for (i = 0; i < GTI_MAX_PAGES; i++) {
		if (align_addr == gti_pages[i])
			return 0;
	}

	if (git_page_idx == GTI_MAX_PAGES) {
		ERROR("Watchdog: Page map limit reached, uninstall gti-wdog\n");
		return ret;
	}

	ret = mmap_add_dynamic_region(align_addr, align_addr, PAGE_SIZE,
				      MT_MEMORY | MT_RW | MT_NS);
	if (ret) {
		ERROR("Watchdog: Failed to map NS region %llx, %d\n",
		      align_addr, ret);
		return ret;
	}

	gti_pages[git_page_idx] = align_addr;
	git_page_idx++;

	return ret;
}

int gti_wdog_install_handler(uint64_t core, uint64_t gti_elr, uint64_t gti_spsr,
			     uint64_t kernel_in_hyp_mode)
{
	int retval = 1;

	g_kernel_hyp_mode = kernel_in_hyp_mode ? 1 : 0;

	debug_gti_watchdog("Watchdog: core = %lld, mpidr = 0x%llx\n",
			 core, read_mpidr());

	retval = gti_wdog_map_info_page(gti_elr);
	if (retval) {
		ERROR("Watchdog: Failed to map elr=%llx page\n", gti_elr);
		return 0;
	}
	retval = gti_wdog_map_info_page(gti_spsr);
	if (retval) {
		ERROR("Watchdog: Failed to map spsr=%llx page\n", gti_spsr);
		return 0;
	}

	gti_shared_addr[core].gti_addr_elr = (unsigned long *)gti_elr;
	gti_shared_addr[core].gti_addr_spsr = (unsigned long *)gti_spsr;

	gicv3_set_spi_routing(GTI_CWD_SPI_IRQ(core), GICV3_IRM_PE,
				read_mpidr());

	return 1;
}

int gti_wdog_start(uint64_t el0_kernel_wdog_callback,
		   uint64_t el1_kernel_wdog_callback,
		   uint64_t watchdog_timeout_ms, uint64_t cores)
{
	debug_gti_watchdog("timeout_ms = %lld, ",
			 watchdog_timeout_ms);

	debug_gti_watchdog("cores = 0x%llx\n", cores);

	g_kernel_cback_el0 = el0_kernel_wdog_callback;
	g_kernel_cback_el1 = el1_kernel_wdog_callback;

	gti_watchdog_set(watchdog_timeout_ms, cores);

	return 1;
}

int gti_wdog_restore_wdog_ctxt(void)
{
	restore_elx_context_and_return();

	return 1;
}
