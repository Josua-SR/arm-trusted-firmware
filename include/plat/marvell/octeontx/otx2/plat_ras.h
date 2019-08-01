/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __PLAT_RAS_H__
#define __PLAT_RAS_H__

#include <lib/extensions/ras.h>

/*
 * RAS interrupt handlers that are expected to be implemented by
 * family/platform.
 */
uint64_t otx2_mdc_isr(uint32_t id, uint32_t flags, void *cookie);
uint64_t otx2_mcc_isr(uint32_t id, uint32_t flags, void *cookie);
int otx2_mdc_probe(const struct err_record_info *info, int *probe_data);
int otx2_mcc_probe(const struct err_record_info *info, int *probe_data);

/*
 * API of plat_ras.c
 */
int otx2_ras_init(void);

#endif /* __PLAT_RAS_H__ */
