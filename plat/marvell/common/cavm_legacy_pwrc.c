/** @file

  Copyright (c) 2016-2018, Marvell International Ltd. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <bakery_lock.h>
#include <mmio.h>
#include <platform.h>
#include <cavm_legacy_pwrc.h>
#include <cavm_common.h>
#include <arch_helpers.h>
#include <debug.h>


static int wait_for_core()
{

	int loop=10;
	volatile union cavm_rst_pp_pending pp_pending;

    /*TODO: This might be a good chance to implement
     * Simple timer library.
     **/

    /* give core chance to come up */
    while(loop) {
        __asm__ __volatile__ ("udiv xzr, xzr,xzr");
        __asm__ __volatile__ ("udiv xzr, xzr,xzr");
        __asm__ __volatile__ ("udiv xzr, xzr,xzr");
        __asm__ __volatile__ ("udiv xzr, xzr,xzr");
        loop --;
    }

    loop =1000000;
    while(loop) {
        pp_pending.u = CSR_READ(CAVM_RST_PP_PENDING);

        if(!pp_pending.s.pend)
            break;
        __asm__ __volatile__ ("udiv xzr, xzr,xzr");
        __asm__ __volatile__ ("udiv xzr, xzr,xzr");
        loop--;
    }
    if(loop == 0)
	    return 1;
    return 0;

}

void octeontx_legacy_pwrc_write_pponr(unsigned long mpidr)
{
	union cavm_rst_pp_reset pp_reset;
	unsigned long octeontx_core_id = (unsigned long)(plat_core_pos_by_mpidr
					((u_register_t)mpidr));

	pp_reset.u = CSR_READ(CAVM_RST_PP_RESET);

	if(!(pp_reset.u & (1ul << octeontx_core_id))) {
		/* core is WFI suspended state
		 * Need to reset it by writing 1 to RST_PP_RESET and then
		 * clearing it.
		 **/
		pp_reset.u |= (1ul << octeontx_core_id);
		CSR_WRITE(CAVM_RST_PP_RESET, pp_reset.u);
		__asm("dsb ishst");
		__asm("sev");
		if(wait_for_core()) {
			WARN("Failed to release core:%lu\n ",
					octeontx_core_id);
			while(1);
			return;
		}
		pp_reset.u = CSR_READ(CAVM_RST_PP_RESET);
	}
	pp_reset.u &= ~(1ul << octeontx_core_id);
	CSR_WRITE(CAVM_RST_PP_RESET, pp_reset.u);
	__asm("dsb ishst");
	__asm("sev");
	if(wait_for_core()){
		WARN("Failed to release core:%lu\n ",
				octeontx_core_id);
	}
}

/* Nothing else to do here apart from initializing the lock */
void octeontx_legacy_pwrc_setup(void)
{
	return;
}
