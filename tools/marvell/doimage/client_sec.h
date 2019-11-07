/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __CLIENT_SEC_H__
#define __CLIENT_SEC_H__

#include "image_defs.h"
#include "msg.h"

#define MAX_HOSTNAME		64
#define MAX_PORT_LENGTH		16
#define DEFAULT_SERVER_PORT	4433
#define DEFAULT_SERVER_NAME	"localhost"

typedef struct _signage_server {
	char server_name[MAX_HOSTNAME+1];
	char server_port[MAX_PORT_LENGTH+1];
} signage_server;

extern signage_server server;

/*******************************************************************************
 *    create_rsa_signature (memory buffer content)
 *          Create RSASSA-PSS/SHA-256 signature for memory buffer
 *          using remote sign server over SSL connection
 *    INPUT:
 *          ctx_idx    Intex to Public Key context
 *          input      memory buffer
 *          ilen       buffer length
 *    OUTPUT:
 *          signature  RSA-2048 signature
 *    RETURN:
 *          0 on success
 ******************************************************************************
 */
int create_rsa_signature(uint32_t		ctx_idx,
			 const unsigned char	*input,
			 size_t			ilen,
			 uint8_t		*signature);

/*******************************************************************************
 *    verify_rsa_signature (memory buffer content)
 *          Verify RSASSA-PSS/SHA-256 signature for memory buffer
 *          using RSA Public Key
 *    INPUT:
 *          pub_key    Public Key buffer
 *          klen       Public Key buffer length
 *          input      memory buffer
 *          ilen       buffer length
 *          pers       personalization string for seeding the RNG.
 *          signature  RSA-2048 signature
 *    OUTPUT:
 *          none
 *    RETURN:
 *          0 on success
 ******************************************************************************
 */
int verify_rsa_signature(const unsigned char	*pub_key,
			 size_t			klen,
			 const unsigned char	*input,
			 size_t			ilen,
			 const char		*pers,
			 uint8_t		*signature);

/*******************************************************************************
 *    process_public_keys
 *           read public keys from PEM files and output them into secure
 *           extension buffer in DER format
 *    INPUT:
 *          sec_opts    security options for the crypto operation
 *          kak_key     pointer to KAK key buffer for DER output
 *          csk_keys    pointer to CSK keys array buffer for DER output
 *          csk_key_sz  CSK key offset increment inside the array
 *    OUTPUT:
 *          none
 *    RETURN:
 *          0 on success
 ******************************************************************************
 */
int process_public_keys(sec_options	*sec_opts,
			uint8_t		*kak_key,
			uint8_t		*csk_keys,
			uint32_t	csk_key_sz);

/*******************************************************************************
 *    client_calc_sha256
 *          Calculate SHA-256 hash for memory buffer
 *    INPUT:
 *          input      input buffer to be hashed
 *          ilen       buffer length
 *    OUTPUT:
 *          out        output buffer for hash value
 *    RETURN:
 *          0 on success
 ******************************************************************************
 */
int client_calc_sha256(const unsigned char	*input,
		       size_t			ilen,
		       unsigned char		*out);

#endif /* __CLIENT_SEC_H__ */
