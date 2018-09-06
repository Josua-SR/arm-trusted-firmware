/** @file

  Copyright (c) 2016-2018, Marvell International Ltd. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __CAVM_SVC_H__
#define __CAVM_SVC_H__

/* SMC function IDs for Standard Service queries */

#define OCTEONTX_SVC_CALL_COUNT	0xc200ff00
#define OCTEONTX_SVC_UID	0xc200ff01

#define OCTEONTX_SVC_VERSION	0xc200ff03

/* OcteonTX Service Calls version numbers */
#define OCTEONTX_VERSION_MAJOR	0x1
#define OCTEONTX_VERSION_MINOR	0x0

/* x1 - node number */
#define OCTEONTX_DRAM_SIZE		0xc2000301

/* no args */
#define OCTEONTX_NODE_COUNT		0xc2000601

/* X1 - gpio_num, X2 - sp, X3 - cpu, X4 - ttbr0 */
#define OCTEONTX_INSTALL_GPIO_INT	0xc2000801
/* X1 - gpio_num */
#define OCTEONTX_REMOVE_GPIO_INT	0xc2000802

/* x1 - character to print */
#define OCTEONTX_PUTC			0xc2000a01

/* x1 - node number */
#if (!defined(PLAT_t81) && !defined(PLAT_t83))
#define OCTEONTX_DISABLE_RVU_LFS	0xc2000b01
#endif

#if (!defined(PLAT_t81) && !defined(PLAT_t83))
#define OCTEONTX_NUM_CALLS		9
#else
#define OCTEONTX_NUM_CALLS		8
#endif

#endif /* __CAVM_SVC_H__ */
