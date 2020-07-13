/*
 * Copyright (C) 2016-2018 Marvell International Ltd.
 * This program is provided "as is" without any warranty of any kind,
 * and is distributed under the applicable Marvell proprietary limited use
 * license agreement.
 */

#include <arch.h>
#include <platform_def.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <stdio.h>
#include <stdlib.h>
#include <libfdt.h>
#include <octeontx_utils.h>
#include <octeontx_common.h>
#include <octeontx_mmap_utils.h>
#include <plat_board_cfg.h>

#include "cavm-csrs-rst.h"

#pragma weak plat_flr_init
#pragma weak plat_initialize_boot_error_data_area

extern void plat_add_mmio();

static void plat_initialize_os_persistent_area(void);

void *fdt_ptr = (void *)~0;

unsigned long plat_get_ns_image_entrypoint(void)
{
	return NS_IMAGE_BASE;
}

uint64_t plat_get_syscnt_freq2(void)
{
	return OCTEONTX_SYSCNT_FREQ * 1000 * 1000;
}

void add_map_record(unsigned long addr, unsigned long size, unsigned int attr)
{
	/* Round up to sensible boundry */
	mmap_add_region(ROUND_DOWN(addr, 2*1024*1024),
			ROUND_DOWN(addr, 2*1024*1024),
			ROUND_UP(size, 2*1024*1024),
			attr);
}

static void plat_add_mmio_common(void)
{
	unsigned long attr;

	/* Shared Memory */
	/* Do not use add_map_record here, it will round size up */
	attr = MT_MEMORY | MT_RO | MT_SECURE;

	if (fdt_ptr != (void *)~0)
		mmap_add_region((unsigned long)fdt_ptr, (unsigned long)fdt_ptr,
			TZDRAM_SIZE - (unsigned long)fdt_ptr,
			attr);

	/* Map BFDT structure */
	attr = MT_MEMORY | MT_RW | MT_SECURE;
	mmap_add_region(BOARD_CFG_BASE, BOARD_CFG_BASE, BOARD_CFG_MAX_SIZE, attr);

	attr = MT_MEMORY | MT_RW | MT_NS;
#if defined(IMAGE_BL31) && defined(PLAT_XLAT_TABLES_DYNAMIC)
	octeontx_mmap_add_dynamic_region_with_sync(NS_IMAGE_BASE,
						   NS_IMAGE_BASE,
						   NS_IMAGE_MAX_SIZE, attr);
#else
	mmap_add_region(NS_IMAGE_BASE, NS_IMAGE_BASE, NS_IMAGE_MAX_SIZE, attr);
#endif

	plat_initialize_boot_error_data_area(attr);
	/*
	 * If appropriate, adjust OS persistent area Device Tree settings here,
	 * before enabling mmu.  Once mmu has been enabled, the Device Tree is
	 * read-only (see mapping of fdt_ptr above).
	 */
	plat_initialize_os_persistent_area();
}

void plat_add_mmio_map()
{
	plat_add_mmio_common();

	plat_add_mmio();
}

void plat_error_handler(int err_code)
{
#if TRUSTED_BOARD_BOOT
	/* Handle platform-specific secure boot failure cause */
	cavm_rst_boot_t rst_boot;

	rst_boot.u = CSR_READ(CAVM_RST_BOOT);
	rst_boot.s.dis_huk = 1;
	CSR_WRITE(CAVM_RST_BOOT, rst_boot.u);
#endif
	for(;;);
}

void plat_flr_init(void) {
	return;
}

/* This can be overridden by platform. */
void plat_initialize_boot_error_data_area(unsigned long attr)
{
	(void)attr;
}

/*
 * plat_initialize_os_persistent_area()
 *
 * Update the device tree settings for the OS persistent store area (ramoops).
 * The device tree contains settings for sub-regions within this area.
 * Adjust the size of the 'pmsg-size' area to use the maximum size available,
 * after accounting for the sizes of the other sub-regions.
 *
 */
void plat_initialize_os_persistent_area(void)
{
	uint32_t rec_sz, pmsg_sz, cons_sz, ftrace_sz, *resize_val, new_size;
	uint64_t oops_base, oops_size, size, remaining;
	const char *resize_prop = NULL;
	const void *fdt = fdt_ptr;
	const fdt64_t *freg64;
	const fdt32_t *freg32;
	fdt32_t dt_reg32;
	int oops_off;

	oops_off = fdt_path_offset(fdt, "/reserved-memory/ramoops");
	if (oops_off < 0)
		return;

	/* Retrieve RAMOOPS DT settings */
	oops_base = oops_size = 0;
	freg64 = fdt_getprop(fdt, oops_off, "reg", NULL);
	if (freg64) {
		oops_base = fdt64p_to_cpu(&freg64[0]);
		oops_size = fdt64p_to_cpu(&freg64[1]);
	}

	if (!oops_base || !oops_size)
		return;

	rec_sz = pmsg_sz = cons_sz = ftrace_sz = 0;
	freg32 = fdt_getprop(fdt, oops_off, "pmsg-size", NULL);
	if (freg32)
		pmsg_sz = fdt32_to_cpu(*freg32);

	/* if not allocated, there is nothing to resize */
	if (!pmsg_sz)
		return;

	freg32 = fdt_getprop(fdt, oops_off, "record-size", NULL);
	if (freg32)
		rec_sz = fdt32_to_cpu(*freg32);
	freg32 = fdt_getprop(fdt, oops_off, "console-size", NULL);
	if (freg32)
		cons_sz = fdt32_to_cpu(*freg32);
	freg32 = fdt_getprop(fdt, oops_off, "ftrace-size", NULL);
	if (freg32)
		ftrace_sz = fdt32_to_cpu(*freg32);

	/* ensure that sum of all regions does not exceed allocated size */
	size = rec_sz + pmsg_sz + cons_sz + ftrace_sz;
	if (size > oops_size) {
		WARN("ramoops regions exceed allocated size\n");
		return;
	}

	/* name of property to be resized to maximum */
	resize_prop = "pmsg-size";
	resize_val = &pmsg_sz;

	/* based on total size, calculate unused (i.e. remaining) size */
	remaining = oops_size - size;

	/* determine new size of desired property */
	size = (uint64_t)*resize_val + remaining;
	/* properties are limited to 32-bits; use def value if limit exceeded */
	if (size > (uint64_t)(uint32_t)-1)
		size = 0x1000;

	/* write resized property value back to device tree */
	new_size = (uint32_t)size;
	dt_reg32 = cpu_to_fdt32(new_size);
	if (fdt_setprop((void *)fdt, oops_off, resize_prop, &dt_reg32,
	    sizeof(dt_reg32)))
		WARN("Cannot resize FDT property '%s' from 0x%x to 0x%x\n",
		     resize_prop, *resize_val, new_size);
}

