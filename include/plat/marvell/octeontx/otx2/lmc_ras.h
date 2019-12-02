/*
 * Copyright (C) 2019 Marvell International Ltd.
 * This program is provided "as is" without any warranty of any kind,
 * and is distributed under the applicable Marvell proprietary limited use
 * license agreement.
 */
#ifndef __PLAT_EDAC_H__
#define __PLAT_EDAC_H__

void plat_mcc_ras_irq_setup(void);
int plat_dram_ras_init(void);

static inline int is_asim(void)
{
	static int memo = -1;

	if (memo < 0)
		memo = !strncmp(plat_octeontx_bcfg->bcfg.board_model,
				"asim-", 5);
	return memo;
}

#endif /* __PLAT_EDAC_H__ */
