/*
 * Copyright (C) 2020 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <bakery_lock.h>
#include <debug.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <stdint.h>

#include <octeontx_mmap_utils.h>

DEFINE_BAKERY_LOCK(mmap_lock);

void octeontx_init_mmap_lock(void)
{
	bakery_lock_init(&mmap_lock);
}

#if PLAT_XLAT_TABLES_DYNAMIC

int octeontx_mmap_add_dynamic_region_with_sync(unsigned long long base_pa,
			uintptr_t base_va, size_t size, unsigned int attr)
{
	int rc;

	bakery_lock_get(&mmap_lock);
	rc = mmap_add_dynamic_region(base_pa, base_va, size, attr);
	bakery_lock_release(&mmap_lock);
	if (rc) {
		ERROR("Unable to map memory rc=%d, addr=0x%llx, size=0x%lx\n",
			rc, base_pa, size);
		return rc;
	}

	return 0;
}

int octeontx_mmap_remove_dynamic_region_with_sync(uintptr_t base_va,
							size_t size)
{
	int rc;

	bakery_lock_get(&mmap_lock);
	rc = mmap_remove_dynamic_region(base_va, size);
	bakery_lock_release(&mmap_lock);
	if (rc) {
		ERROR(
			"Unable to remove mapped memory rc=%d, addr=0x%lx, size=0x%lx\n",
			rc, base_va, size);
		return rc;
	}

	return 0;
}

int octeontx_xlat_change_mem_attributes(uintptr_t base_va, size_t size,
			uint32_t attr, uint32_t *old_attr)
{
	int rc;

	bakery_lock_get(&mmap_lock);
	if (old_attr)
		xlat_get_mem_attributes(base_va, old_attr);
	rc = xlat_change_mem_attributes(base_va, size, attr);
	bakery_lock_release(&mmap_lock);
	return rc;
}
#endif /* PLAT_XLAT_TABLES_DYNAMIC */
