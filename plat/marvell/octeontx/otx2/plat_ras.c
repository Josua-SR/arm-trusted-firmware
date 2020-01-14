/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <lib/extensions/ras.h>
#include <octeontx_irqs_def.h>
#include <plat_ras.h>
#include <plat/common/platform.h>

/*
 * It is number of all RAS interrupts.
 */
#define NUMBER_OF_RAS_INTERRUPTS	(MDC_SPI_IRQS + \
					MCC_SPI_IRQS)

#define RAS_MDC_HANDLER			0
#define RAS_MCC_HANDLER			1
#define RAS_HANDLERS			2

static int plat_ras_mdc_handler(const struct err_record_info *info,
		int probe_data, const struct err_handler_data *const data)
{
	int ret;

	ret = otx2_mdc_isr(data->interrupt, data->flags, data->cookie);

	/* issue EOI to controller */
	plat_ic_end_of_interrupt(data->interrupt);

	return ret;
}

static int plat_ras_mcc_handler(const struct err_record_info *info,
		int probe_data, const struct err_handler_data *const data)
{
	int ret;

	ret = otx2_mcc_isr(data->interrupt, data->flags, data->cookie);

	/* issue EOI to controller */
	plat_ic_end_of_interrupt(data->interrupt);

	return ret;
}

struct ras_interrupt otx2_ras_interrupts[NUMBER_OF_RAS_INTERRUPTS];

struct err_record_info otx2_err_records[RAS_HANDLERS] = {
	[RAS_MDC_HANDLER] = {
		.probe = otx2_mdc_probe,
		.handler = plat_ras_mdc_handler,
	},
	[RAS_MCC_HANDLER] = {
		.probe = otx2_mcc_probe,
		.handler = plat_ras_mcc_handler,
	},
};

REGISTER_ERR_RECORD_INFO(otx2_err_records);
REGISTER_RAS_INTERRUPTS(otx2_ras_interrupts);

static void plat_ras_initialize_interrupt_array(void)
{
	int i, idx = 0;

	for (i = 0; i < MDC_SPI_IRQS; i++) {
		otx2_ras_interrupts[idx].intr_number = MDC_SPI_IRQ();
		otx2_ras_interrupts[idx].err_record =
				&otx2_err_records[RAS_MDC_HANDLER];
		idx++;
	}

	for (i = 0; i < MCC_SPI_IRQS; i++) {
		otx2_ras_interrupts[idx].intr_number = MCC_SPI_IRQ(i);
		otx2_ras_interrupts[idx].err_record =
				&otx2_err_records[RAS_MCC_HANDLER];
		idx++;
	}

#ifdef RAS_LMC_HANDLER
	for (i = 0; i < LMC_SPI_IRQS; i++) {
		otx2_ras_interrupts[idx].intr_number = LMC_SPI_IRQ(i);
		otx2_ras_interrupts[idx].err_record =
				&otx2_err_records[RAS_LMC_HANDLER];
		idx++;
	}
#endif
}

int otx2_ras_init(void)
{
	plat_ras_initialize_interrupt_array();
	ras_init();

	return 0;
}
