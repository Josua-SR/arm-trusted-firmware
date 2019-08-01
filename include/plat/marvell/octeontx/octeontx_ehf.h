/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __OCTEONTX_EHF_H__
#define __OCTEONTX_EHF_H__

typedef uint64_t (*octeontx_irq_handler_t)(uint32_t id,
					uint32_t flags,
					void *cookie);

int octeontx_ehf_register_irq_handler(uint32_t id,
	octeontx_irq_handler_t handler);
int octeontx_ehf_setup(void);

#endif /* __OCTEONTX_EHF_H__ */
