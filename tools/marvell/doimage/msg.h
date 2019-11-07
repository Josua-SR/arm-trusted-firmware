/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __MSG_H__
#define __MSG_H__

#include <stdint.h>
#include "image_defs.h"

#define SIGN_MSG_MAGIC		0x9e7c0ffe
#define MSG_TYP_SIGN_REQEST	0x2057
#define MSG_TYP_SIGN_RETURN	0x2e72
#define MSG_MAX_SZ		(4 * 1024)

typedef struct _sign_message {
	uint32_t	magic;		/* message magic number */
	uint32_t	index;		/* signage key index */
	uint32_t	type;		/* message type*/
	uint32_t	length;		/* aligned to 4 bytes */
	uint8_t		sign[RSA_SIGN_BYTE_LEN];
} sign_msg;

#endif /* __MSG_H__ */
