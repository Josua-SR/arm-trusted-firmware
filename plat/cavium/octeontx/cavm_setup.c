/** @file

  Copyright (c) 2018, Cavium Inc. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <arch.h>
#include <stdio.h>
#include <debug.h>
#include <platform_def.h>
#include <cavm_common.h>
#include <cavm_legacy_pwrc.h>
#include <cavm_gpio.h>

/* Any SoC family specific setup
 * to be done in BL31 can be initialized
 * in this API. If there are any platform
 * specific init to be done but not common
 * to family, each platform can define
 * its own API and can be called from here
 */
void plat_cavm_setup(void)
{
	/* setup gpio interrupt handling */
	plat_gpio_irq_setup();
}

void plat_pwrc_setup(void)
{
	cavm_legacy_pwrc_setup();
}

void plat_setup_psci_ops(uintptr_t sec_entrypoint,
			 const plat_psci_ops_t **psci_ops)
{
	cavm_legacy_setup_psci_ops(sec_entrypoint, psci_ops);
}

/*
 * Read a single fuse bit from MIO_FUS
 */
int plat_fuse_read(int node, int fuse)
{
	cavm_mio_fus_rcmd_t read_cmd;
	int byte_addr = FUSE_BIT_TO_BYTE_ADDR(fuse);

	read_cmd.u = 0;
	read_cmd.s.addr = byte_addr;
	read_cmd.s.addr_hi = FUSE_HI_ADDR(byte_addr);
	read_cmd.s.pend = 1;
	CSR_WRITE(node, CAVM_MIO_FUS_RCMD, read_cmd.u);
	while ((read_cmd.u = CSR_READ(node, CAVM_MIO_FUS_RCMD)) &&
		read_cmd.s.pend);

	return FUSE_GET_VAL(read_cmd.s.dat, fuse);
}
