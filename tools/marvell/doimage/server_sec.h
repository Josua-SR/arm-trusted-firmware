/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __SERVER_SEC_H__
#define __SERVER_SEC_H__

#include <stdint.h>
#include "msg.h"

#define MAX_FILENAME		256
#define CSK_ARR_SZ		16
#define CSK_ARR_EMPTY_FILE	"*"
#define AES_KEY_BIT_LEN		256
#define AES_KEY_BYTE_LEN	(AES_KEY_BIT_LEN >> 3)
#define AES_BLOCK_SZ		16
#define DEFAULT_SERVER_PORT	4433
#define MAX_PORT_LENGTH		16

typedef struct _srv_sec_options {
	char kak_key_file[MAX_FILENAME+1];
	char csk_key_file[CSK_ARR_SZ][MAX_FILENAME+1];
	char server_port[MAX_PORT_LENGTH+1];
	/* TODO - switch to configrable server keys and certs */
	char server_key_file[MAX_FILENAME+1];
	char server_cert_file[MAX_FILENAME+1];
	char ca_cert_file[MAX_FILENAME+1];
} srv_sec_options;

#endif /* __SERVER_SEC_H__ */
