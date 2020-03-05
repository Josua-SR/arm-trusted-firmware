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
	int i, iobn_nr;
	int domain, bus, dev;
	union cavm_pccpf_xxx_vsec_ctl vsec_ctl;
	union cavm_ecamx_const ecamx_const;
	union cavm_ecamx_domx_const domx_const;
	int iobn5_domain_max;
	int set_all_domains;

	vsec_ctl.u = octeontx_read32(config_base + CAVM_PCCPF_XXX_VSEC_CTL);
	iobn_nr = vsec_ctl.s.inst_num;

	/*
	 * Allow all IO units to access non-secure memory.
	 * We can program secure devices later when they discovered.
	 */
	for (i = 0; i < 256; i++) {
		CSR_WRITE(CAVM_IOBNX_RSLX_STREAMS(iobn_nr, i), 0x3);
	}

	if (IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 1) ||
	    IS_OCTEONTX_PN(read_midr(), F95PARTNUM) ||
	    IS_OCTEONTX_PN(read_midr(), F95MMPARTNUM)) {
		/*
		 * ECAMX_DOMX_CONST shows up only that are present under it.
		 * To enable PEMX controllers on domain 3 4 5 6, use the loop
		 * max as 0x7 to include them.
		 */
		iobn5_domain_max = 0x7;
		set_all_domains = 1;
	} else {
		ecamx_const.u = CSR_READ(CAVM_ECAMX_CONST(0));
		iobn5_domain_max = ecamx_const.s.domains;
		set_all_domains = 0;
	}

	for (domain = 0; domain < iobn5_domain_max; domain++) {
		if (!set_all_domains) {
			/* Domains may not be contiguous */
			domx_const.u = CSR_READ(CAVM_ECAMX_DOMX_CONST(
								0, domain));
			if (!domx_const.s.pres)
				continue;
		}

		for (bus = 0; bus < 256; bus++) {
			/* To get secure interrupts for GPIO and BPHY
			 * the PHYS_NSEC has to be 0
			 */
			if (domain == 0 && (bus == 0 || bus == 6))
				CSR_WRITE(CAVM_IOBNX_DOMX_BUSX_STREAMS(
					  iobn_nr, domain, bus), 0x2);
			else
				CSR_WRITE(CAVM_IOBNX_DOMX_BUSX_STREAMS(
					  iobn_nr, domain, bus), 0x3);
		}
		for (dev = 0; dev < 32; dev++) {
			/* To get secure interrupts for GPIO and BPHY
			 * the PHYS_NSEC has to be 0
			 */
			if (domain == 0 &&
			    (dev == (CAVM_PCC_DEV_CON_E_GPIO_CN9 >> 3)
			    || dev == ((CAVM_PCC_DEV_CON_E_BPHY & 0xFF) >> 3)))
				CSR_WRITE(CAVM_IOBNX_DOMX_DEVX_STREAMS(
					  iobn_nr, domain, dev), 0x2);
			else
				CSR_WRITE(CAVM_IOBNX_DOMX_DEVX_STREAMS(
					  iobn_nr, domain, dev), 0x3);
		}
	}
}
