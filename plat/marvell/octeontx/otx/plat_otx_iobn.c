/*
 * Copyright (C) 2020 Marvell International Ltd.
 * This program is provided "as is" without any warranty of any kind,
 * and is distributed under the applicable Marvell proprietary limited use
 * license agreement.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <octeontx_common.h>
#include <plat_board_cfg.h>
#include <octeontx_ecam.h>
#include <octeontx_utils.h>
#include <platform_def.h>
#include <octeontx_irqs_def.h>

#include "cavm-csrs-ecam.h"
#include "cavm-csrs-iobn.h"
#include "cavm-csrs-pccpf.h"

void octeontx_init_iobn(uint64_t config_base, uint64_t config_size)
{
	int iobn_nr;
	union cavm_pccpf_xxx_vsec_ctl vsec_ctl;
	union cavm_iobnx_dis_ncbi_io iobn_dis_ncbi;

	vsec_ctl.u = octeontx_read32(config_base + CAVM_PCCPF_XXX_VSEC_CTL);
	iobn_nr = vsec_ctl.s.inst_num;

	/*
	 * When booting Trusted Mode, the Boot ROM disables accesses originating
	 * from SLI (sets IOBN0_DIS_NCBI_IO.SLI_OFF=1). We unset the bit here.
	 * Ideally signed software has to clear this  bit if it wants to allow
	 * external PCIe devices to do DMA.
	 */
	if (iobn_nr == 0) {
		iobn_dis_ncbi.u = CSR_READ(CAVM_IOBNX_DIS_NCBI_IO(iobn_nr));

		iobn_dis_ncbi.s.sli_off = 0;
		CSR_WRITE(CAVM_IOBNX_DIS_NCBI_IO(iobn_nr), iobn_dis_ncbi.u);
	}
}


