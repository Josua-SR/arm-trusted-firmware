/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <debug.h>
#include <errno.h>
#include <platform_def.h>
#include <gicv3_setup.h>
#include <octeontx_ehf.h>
#include <plat/common/platform.h>

#include <bl31/ehf.h>

typedef struct irq_handler_entry {
	uint32_t id;
	octeontx_irq_handler_t handler;
} irq_handler_entry_t;

static irq_handler_entry_t irq_handlers_list[NUMBER_OF_GIC_INTERRUPTS];

/*
 * Enumeration of priority levels on OcteonTX2 platforms.
 */
ehf_pri_desc_t octeontx_exceptions[] = {
	/* IRQ Priority */
	EHF_PRI_DESC(OCTEONTX_PRI_BITS, PLAT_IRQ_PRI),
#ifdef PLAT_RAS_PRI
	/* RAS Priority */
	EHF_PRI_DESC(OCTEONTX_PRI_BITS, PLAT_RAS_PRI),
#endif /* PLAT_RAS_PRI */

#if SDEI_SUPPORT
	/* Critical priority SDEI */
	EHF_PRI_DESC(OCTEONTX_PRI_BITS, PLAT_SDEI_CRITICAL_PRI),

	/* Normal priority SDEI */
	EHF_PRI_DESC(OCTEONTX_PRI_BITS, PLAT_SDEI_NORMAL_PRI),
#endif
};

/* Plug in OcteonTX2 exceptions to Exception Handling Framework. */
EHF_REGISTER_PRIORITIES(octeontx_exceptions, ARRAY_SIZE(octeontx_exceptions),
	OCTEONTX_PRI_BITS);

static int ehf_handle_irq_el3(uint32_t intr_raw, uint32_t flags,
	void *handle, void *cookie)
{
	uint32_t irq;
	uint64_t rc = 0;

	for (irq = 0; irq < NUMBER_OF_GIC_INTERRUPTS; irq++) {
		if (irq_handlers_list[irq].id == intr_raw) {
			rc = irq_handlers_list[irq].handler(intr_raw, flags,
								cookie);
			break;
		}
	}

	if (irq == NUMBER_OF_GIC_INTERRUPTS)
		WARN("Handler for IRQ 0x%x not found!\n", intr_raw);

	plat_ic_end_of_interrupt(intr_raw);

	return rc;
}

int octeontx_ehf_register_irq_handler(uint32_t id,
	octeontx_irq_handler_t handler)
{
	uint32_t idx;

	/* Validate the 'handler' parameter */
	if (!handler)
		return -EINVAL;

	/* Check if a handler for this id is already been registered */
	for (idx = 0; idx < NUMBER_OF_GIC_INTERRUPTS; idx++) {
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

int octeontx_ehf_setup(void)
{
	ehf_register_priority_handler(PLAT_IRQ_PRI, ehf_handle_irq_el3);

	return 0;
}
