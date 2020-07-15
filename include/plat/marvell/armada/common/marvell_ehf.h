/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __MARVELL_EHF_H__
#define __MARVELL_EHF_H__

typedef uint64_t (*marvell_irq_handler_t)(uint32_t id,
					  uint32_t flags,
					  void *cookie);

int marvell_ehf_register_irq_handler(uint32_t id,
				     marvell_irq_handler_t handler);
int marvell_ehf_setup(void);

#endif /* __MARVELL_EHF_H__ */
