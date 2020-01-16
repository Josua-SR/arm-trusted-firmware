/*
 * Copyright (C) 2020 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __OCTEONTX_MMAP_UTILS_H__
#define __OCTEONTX_MMAP_UTILS_H__

void octeontx_init_mmap_lock(void);

#if PLAT_XLAT_TABLES_DYNAMIC
int octeontx_mmap_add_dynamic_region_with_sync(unsigned long long base_pa,
			uintptr_t base_va, size_t size, unsigned int attr);
int octeontx_mmap_remove_dynamic_region_with_sync(uintptr_t base_va,
							size_t size);
#endif /* PLAT_XLAT_TABLES_DYNAMIC */

#endif /* __OCTEONTX_MMAP_UTILS_H__ */
