/** @file

  Copyright (c) 2016-2018, Cavium Inc. All rights reserved.<BR>
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

#define THUNDERX_SVC_CALL_COUNT	0x4300ff00
#define THUNDERX_SVC_UID		0x4300ff01

#define THUNDERX_SVC_VERSION	0x4300ff03

/* ThunderX Service Calls version numbers */
#define THUNDERX_VERSION_MAJOR	0x0
#define THUNDERX_VERSION_MINOR	0x2

/* x1 - node number */
#define THUNDERX_DRAM_SIZE		0x43000301

/* no args */
#define THUNDERX_NODE_COUNT		0x43000601

/* x1 - character to print */
#define THUNDERX_PUTC			0x43000a01

#define THUNDERX_NUM_CALLS		6

#endif /* __THUNDER_SVC_H__ */
