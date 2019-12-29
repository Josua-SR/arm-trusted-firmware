/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:	BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <armada_common.h>
#include <mvebu_def.h>

/*
 * If bootrom is currently at BLE there's no need to include the memory
 * maps structure at this point
 */
#ifndef IMAGE_BLE

/*****************************************************************************
 * AMB Configuration
 *****************************************************************************
 */
struct addr_map_win amb_memory_map_cp0[] = {
	/* CP0 SPI1 CS0 Direct Mode access */
	{0xf900,	0x1000000,	AMB_SPI1_CS0_ID},
};

int marvell_get_amb_memory_map(struct addr_map_win **win, uint32_t *size,
			       uintptr_t base)
{
	switch (base) {
	case MVEBU_CP_REGS_BASE(0):
		*win = amb_memory_map_cp0;
		*size = ARRAY_SIZE(amb_memory_map_cp0);
		return 0;
	default:
		*size = 0;
		*win = 0;
		return 1;
	}
}
#endif

/*****************************************************************************
 * IO WIN Configuration
 *****************************************************************************
 */
struct addr_map_win io_win_memory_map[] = {
#ifndef IMAGE_BLE
	/* MCI 0 indirect window */
	{MVEBU_MCI_REG_BASE_REMAP(0),	0x100000,	MCI_0_TID},
	/* MCI 1 indirect window */
	{MVEBU_MCI_REG_BASE_REMAP(1),	0x100000,	MCI_1_TID},
#endif
};

/* Global Control Register - window default target */
uint32_t marvell_get_io_win_gcr_target(int ap_index)
{
	/*
	 * PIDI == iMCIP AP to SB internal MoChi connection.
	 * In other words CP0
	 */
	return PIDI_TID;
}

int marvell_get_io_win_memory_map(int ap_index, struct addr_map_win **win,
				  uint32_t *size)
{
	*win = io_win_memory_map;
	if (*win == NULL)
		*size = 0;
	else
		*size = ARRAY_SIZE(io_win_memory_map);

	return 0;
}

#ifndef IMAGE_BLE
/*****************************************************************************
 * IOB Configuration
 *****************************************************************************
 */
struct addr_map_win iob_memory_map_cp0[] = {
	/* SPI1_CS0 (RUNIT) window */
	{0x00000000f9000000,	0x1000000,	RUNIT_TID},
	/* PEX0_X4 window */
	{0x0000008000000000,	0x800000000,	PEX0_TID},
};

int marvell_get_iob_memory_map(struct addr_map_win **win, uint32_t *size,
			       uintptr_t base)
{
	switch (base) {
	case MVEBU_CP_REGS_BASE(0):
		*win = iob_memory_map_cp0;
		*size = ARRAY_SIZE(iob_memory_map_cp0);
		return 0;
	default:
		*size = 0;
		*win = 0;
		return 1;
	}
}
#endif

/*****************************************************************************
 * CCU Configuration
 *****************************************************************************
 */
struct addr_map_win ccu_memory_map[] = {	/* IO window */
#ifdef IMAGE_BLE
	{0x00000000f2000000,	0x6000000,	IO_0_TID}, /* IO window */
#else
#if LLC_SRAM
	{PLAT_MARVELL_LLC_SRAM_BASE, PLAT_MARVELL_LLC_SRAM_SIZE, DRAM_0_TID},
#endif
	{0x00000000f2000000,	0xe000000,	IO_0_TID}, /* IO window */
	{0x0000008000000000,	0x800000000,	IO_0_TID}, /* IO window */
#endif
};

uint32_t marvell_get_ccu_gcr_target(int ap)
{
	return DRAM_0_TID;
}

int marvell_get_ccu_memory_map(int ap_index, struct addr_map_win **win,
			       uint32_t *size)
{
	*win = ccu_memory_map;
	*size = ARRAY_SIZE(ccu_memory_map);

	return 0;
}

#ifdef IMAGE_BLE
/*****************************************************************************
 * SKIP IMAGE Configuration
 *****************************************************************************
 */
void *plat_get_skip_image_data(void)
{
	/* No recovery button on CN-9130 board? */
	return NULL;
}
#endif
