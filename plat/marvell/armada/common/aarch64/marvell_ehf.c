/*
 * Copyright (c) 2017-2018, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include <errno.h>
#include <lib/el3_runtime/context_mgmt.h>
#include <marvell_ehf.h>
#include <plat/common/platform.h>
#include <platform_def.h>

#include <bl31/ehf.h>

#define MARVELL_MAX_EHF_INT_HANDLERS	8

typedef struct irq_handler_entry {
	uint32_t id;
	marvell_irq_handler_t handler;
} irq_handler_entry_t;

static irq_handler_entry_t irq_handlers_list[MARVELL_MAX_EHF_INT_HANDLERS];

/*
 * Enumeration of priority levels on ARM platforms.
 */
ehf_pri_desc_t marvell_exceptions[] = {
	/* IRQ Priority */
	EHF_PRI_DESC(MARVELL_PRI_BITS, PLAT_MARVELL_IRQ_PRI),
#if SDEI_SUPPORT
	/* Critical priority SDEI */
	EHF_PRI_DESC(MARVELL_PRI_BITS, PLAT_SDEI_CRITICAL_PRI),

	/* Normal priority SDEI */
	EHF_PRI_DESC(MARVELL_PRI_BITS, PLAT_SDEI_NORMAL_PRI),
#endif
};

/* Plug in ARM exceptions to Exception Handling Framework. */
EHF_REGISTER_PRIORITIES(marvell_exceptions, ARRAY_SIZE(marvell_exceptions), MARVELL_PRI_BITS);

static int ehf_handle_irq_el3(uint32_t intr_raw, uint32_t flags,
			      void *handle, void *cookie)
{
	uint32_t irq;
	uint64_t rc = 0;

	cm_el1_sysregs_context_save(NON_SECURE);
	for (irq = 0; irq < MARVELL_MAX_EHF_INT_HANDLERS; irq++) {
		if (irq_handlers_list[irq].id == intr_raw) {
			rc = irq_handlers_list[irq].handler(intr_raw, flags,
								cookie);
			break;
		}
	}

	if (irq == MARVELL_MAX_EHF_INT_HANDLERS)
		WARN("Handler for IRQ 0x%x not found!\n", intr_raw);

	plat_ic_end_of_interrupt(intr_raw);

	return rc;
}

int marvell_ehf_register_irq_handler(uint32_t id,
				     marvell_irq_handler_t handler)
{
	uint32_t idx;

	/* Validate the 'handler' parameter */
	if (!handler)
		return -EINVAL;

	/* Check if a handler for this id is already been registered */
	for (idx = 0; idx < MARVELL_MAX_EHF_INT_HANDLERS; idx++) {
		if (irq_handlers_list[idx].id == id)
			return -EALREADY;

		if (!irq_handlers_list[idx].handler) {
			/* Save the handler */
			irq_handlers_list[idx].handler = handler;
			irq_handlers_list[idx].id  = id;

			return 0;
		}
	}

	return -E2BIG;
}

int marvell_ehf_setup(void)
{
	ehf_register_priority_handler(PLAT_MARVELL_IRQ_PRI,
				      ehf_handle_irq_el3);

	return 0;
}
