/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif /* MBEDTLS_CONFIG_FILE */

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#endif /* MBEDTLS_PLATFORM_C */

#if	!defined(MBEDTLS_BIGNUM_C) || !defined(MBEDTLS_ENTROPY_C) ||  \
	!defined(MBEDTLS_SSL_TLS_C) || !defined(MBEDTLS_SSL_CLI_C) || \
	!defined(MBEDTLS_NET_C) || !defined(MBEDTLS_RSA_C) ||         \
	!defined(MBEDTLS_CERTS_C) || !defined(MBEDTLS_PEM_PARSE_C) || \
	!defined(MBEDTLS_PK_PARSE_C) || !defined(MBEDTLS_FS_IO) || \
	!defined(MBEDTLS_SHA256_C) || \
	!defined(MBEDTLS_CTR_DRBG_C) || !defined(MBEDTLS_X509_CRT_PARSE_C)
int main(void)
{
	printf("MBEDTLS_BIGNUM_C and/or MBEDTLS_ENTROPY_C and/or "
		"MBEDTLS_SSL_TLS_C and/or MBEDTLS_SSL_CLI_C and/or "
		"MBEDTLS_NET_C and/or MBEDTLS_RSA_C and/or "
		"MBEDTLS_CERTS_C and/or MBEDTLS_PEM_PARSE_C and/or "
		"MBEDTLS_PK_PARSE_C and/or MBEDTLS_FS_IO and/or "
		"MBEDTLS_SHA256_C and/or "
		"MBEDTLS_CTR_DRBG_C and/or MBEDTLS_X509_CRT_PARSE_C "
		"not defined.\n");
	return 0;
}

#else  /* MBEDTLS_BIGNUM_C && MBEDTLS_ENTROPY_C && MBEDTLS_SSL_TLS_C &&
	* MBEDTLS_SSL_CLI_C && MBEDTLS_NET_C && MBEDTLS_RSA_C &&
	* MBEDTLS_CERTS_C && MBEDTLS_PEM_PARSE_C && MBEDTLS_CTR_DRBG_C &&
	* MBEDTLS_PK_PARSE_C && MBEDTLS_FS_IO && MBEDTLS_SHA256_C &&
	* MBEDTLS_X509_CRT_PARSE_C
	*/

#include <mbedtls/base64.h>
#include <mbedtls/error.h>
#include <mbedtls/entropy.h>
#include <mbedtls/certs.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/debug.h>
#include <mbedtls/md.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/pk.h>
#include <mbedtls/ssl.h>
#include <mbedtls/sha256.h>
#include <mbedtls/x509.h>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "client_sec.h"

#define DER_BUF_SZ	1600
#define DEBUG_LEVEL	0

static mbedtls_pk_context pk_ctx[CSK_ARR_SZ + 1];
signage_server server;

static void client_debug(void *ctx, int level,
			 const char *file, int line,
			 const char *str)
{
	fprintf((FILE *)ctx, "%s:%04d: %s", file, line, str);
	fflush((FILE *)ctx);
}

/*******************************************************************************
 *    create_rsa_signature (memory buffer content)
 *          Create RSASSA-PSS/SHA-256 signature for memory buffer
 *          using RSA Private Key
 *    INPUT:
 *          ctx_idx    Index of Public Key context
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
			 uint8_t		*signature)
{
	int ret;
	size_t base64_len, rx_len;
	mbedtls_net_context server_fd;
	uint32_t flags;
	unsigned char buf[MSG_MAX_SZ];
	const char *pers = "doimage_client";
	sign_msg msg;

	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
	mbedtls_x509_crt ca_cert;

#if defined(MBEDTLS_DEBUG_C)
	mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

	/* 0. Initialize the RNG and the session data */
	mbedtls_net_init(&server_fd);
	mbedtls_ssl_init(&ssl);
	mbedtls_ssl_config_init(&conf);
	mbedtls_x509_crt_init(&ca_cert);
	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);

	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
				    (const unsigned char *)pers, strlen(pers));
	if (ret) {
		fprintf(stderr, "Failed to init RNG!\n");
		goto sign_exit;
	}

	/* 1. Initialize certificates */
	ret = mbedtls_x509_crt_parse(&ca_cert,
				(const unsigned char *)mbedtls_test_cas_pem,
				mbedtls_test_cas_pem_len);
	if (ret) {
		fprintf(stderr, "Failed to parse X509 certificate!\n");
		goto sign_exit;
	}

	/* 2. Start the connection */
	ret = mbedtls_net_connect(&server_fd, server.server_name,
				  server.server_port, MBEDTLS_NET_PROTO_TCP);
	if (ret) {
		fprintf(stderr, "Failed to connect to remote server!\n");
		goto sign_exit;
	}

	/* 3. Setup SSL/TLS structure */
	ret = mbedtls_ssl_config_defaults(&conf,
					  MBEDTLS_SSL_IS_CLIENT,
					  MBEDTLS_SSL_TRANSPORT_STREAM,
					  MBEDTLS_SSL_PRESET_DEFAULT);
	if (ret) {
		fprintf(stderr, "Failed to configure SSL defaults!\n");
		goto sign_exit;
	}

	/*
	 * OPTIONAL is not optimal for security,
	 * but makes interop easier in this simplified example
	 */
	mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
	mbedtls_ssl_conf_ca_chain(&conf, &ca_cert, NULL);
	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
	mbedtls_ssl_conf_dbg(&conf, client_debug, stdout);

	ret = mbedtls_ssl_setup(&ssl, &conf);
	if (ret) {
		fprintf(stderr, "Failed to setup SSL!\n");
		goto sign_exit;
	}

	ret = mbedtls_ssl_set_hostname(&ssl, server.server_name);
	if (ret) {
		fprintf(stderr, "Failed to set SSL hostname!\n");
		goto sign_exit;
	}

	mbedtls_ssl_set_bio(&ssl, &server_fd,
			    mbedtls_net_send, mbedtls_net_recv, NULL);

	/* 4. Handshake */
	while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ &&
		    ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			fprintf(stderr, "Failed in SSL handshake!\n");
			goto sign_exit;
		}
	}

	/* 5. Verify the server certificate */
	flags = mbedtls_ssl_get_verify_result(&ssl);
	if (flags) {
		mbedtls_x509_crt_verify_info((char *)buf,
					     sizeof(buf), " ! ", flags);
		fprintf(stderr, "Failed to verify server SSL sert.:\n%s\n",
			buf);
	}

	/* 6. Build the message and send it to the connected server */
	memset(buf, 0, sizeof(buf));
	msg.magic = SIGN_MSG_MAGIC;
	msg.index = ctx_idx;
	msg.type  = MSG_TYP_SIGN_REQEST;
	msg.length = SHA_DIGEST_BYTE_LEN;

	/* Compute the SHA256 hash for the input blob */
	mbedtls_sha256_ret(input, ilen, msg.sign, 0);
	ret = mbedtls_base64_encode(buf, sizeof(buf) - 1, &base64_len,
				    (unsigned char *)&msg, sizeof(msg));
	if (ret) {
		fprintf(stderr, "Failed to compose output message!\n");
		goto sign_exit;
	}

	printf("Request signature using key ID %d\n", ctx_idx);
	while ((ret = mbedtls_ssl_write(&ssl, buf, base64_len)) <= 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ &&
		    ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			fprintf(stderr, "Failed to send server request!\n");
			goto sign_exit;
		}
	}

	/* 7. Get the server response */
	do {
		memset(buf, 0, sizeof(buf));
		rx_len = 0;
		ret = mbedtls_ssl_read(&ssl, buf, sizeof(buf) - 1);
		if (ret == MBEDTLS_ERR_SSL_WANT_READ ||
		    ret == MBEDTLS_ERR_SSL_WANT_WRITE)
			continue;

		if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
			break;

		/* EOF */
		if (ret == 0)
			break;

		if (ret < 0) {
			fprintf(stderr, "Failed to read response!\n");
			break;
		}

		/* 8. Decode the server response */
		rx_len = ret;
		ret = mbedtls_base64_decode((unsigned char *)&msg, sizeof(msg),
					    &base64_len, buf, rx_len);
		if (ret) {
			fprintf(stderr, "Failed to decode server message!\n");
			goto sign_exit;
		}
		/* Message sanity check */
		if ((msg.magic  != SIGN_MSG_MAGIC) ||
		    (msg.index  != ctx_idx) ||
		    (msg.type   != MSG_TYP_SIGN_RETURN) ||
		    (msg.length != RSA_SIGN_BYTE_LEN)) {
			fprintf(stderr,
				"Bad message (%ld bytes) received from server!\n",
				base64_len);
			fprintf(stderr,
				"\tHDR Magic\t%#010x\tKey Index\t%#010x\n",
				msg.magic, msg.index);
			fprintf(stderr,
				"\tMsg Type\t%#010x\tMsg length\t%#010x\n",
				msg.type, msg.length);
			goto sign_exit;
		}
		memcpy(signature, msg.sign, RSA_SIGN_BYTE_LEN);
	} while (1);

	mbedtls_ssl_close_notify(&ssl);

sign_exit:

#ifdef MBEDTLS_ERROR_C
	if (ret) {
		mbedtls_strerror(ret, (char *)buf, sizeof(buf));
		fprintf(stderr, "Last error was: %d - %s\n", ret, buf);
	}
#endif

	/* Roll back the connection */
	mbedtls_net_free(&server_fd);

	mbedtls_x509_crt_free(&ca_cert);
	mbedtls_ssl_free(&ssl);
	mbedtls_ssl_config_free(&conf);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);

	return ret;
} /* end of create_rsa_signature */


/*******************************************************************************
 *    verify_rsa_signature (memory buffer content)
 *          Verify RSASSA-PSS/SHA-256 signature for memory buffer
 *          using RSA Public Key
 *    INPUT:
 *          pub_key    Public Key buffer
 *          ilen       Public Key buffer length
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
			 uint8_t		*signature)
{
	mbedtls_entropy_context		entropy;
	mbedtls_ctr_drbg_context	ctr_drbg;
	mbedtls_pk_context		tmp_pk_ctx;
	unsigned char			hash[32];
	int				rval;
	unsigned char			*pkey = (unsigned char *)pub_key;

	/* Not sure this is required,
	 * but it's safer to start with empty buffer
	 */
	memset(hash, 0, sizeof(hash));

	mbedtls_pk_init(&tmp_pk_ctx);
	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);

	/* Seed the random number generator */
	rval = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
				(const unsigned char *)pers, strlen(pers));
	if (rval != 0) {
		fprintf(stderr, " Failed in ctr_drbg_init call (%d)!\n", rval);
		goto verify_exit;
	}

	/* Check ability to read the public key */
	rval = mbedtls_pk_parse_subpubkey(&pkey, pub_key + klen, &tmp_pk_ctx);
	if (rval != 0) {
		fprintf(stderr, " Failed in pk_parse_public_key (%#x)!\n",
			rval);
		goto verify_exit;
	}

	/* Set the padding type for the new PK context */
	mbedtls_rsa_set_padding(mbedtls_pk_rsa(tmp_pk_ctx),
				MBEDTLS_RSA_PKCS_V21,
				MBEDTLS_MD_SHA256);

	/* Compute the SHA256 hash for the input buffer */
	mbedtls_sha256_ret(input, ilen, hash, 0);

	rval = mbedtls_rsa_rsassa_pss_verify(mbedtls_pk_rsa(tmp_pk_ctx),
					     mbedtls_ctr_drbg_random,
					     &ctr_drbg,
					     MBEDTLS_RSA_PUBLIC,
					     MBEDTLS_MD_SHA256, 0,
					     hash, signature);
	if (rval != 0) {
		fprintf(stderr, "Failed to verify signature (%d)!\n", rval);
		fflush(stderr);
	}

verify_exit:

	mbedtls_pk_free(&tmp_pk_ctx);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	return rval;
} /* end of verify_rsa_signature */

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
			uint32_t	csk_key_sz)
{
	int		index;

	/* Bring up RSA context and read public keys from their files */
	for (index = 0; index < (CSK_ARR_SZ + 1); index++) {
		/* for every private key file */
		char		*fname = (index == CSK_ARR_SZ) ?
					sec_opts->kak_key_file :
					sec_opts->csk_key_file[index];
		uint8_t		*csk_key = csk_keys + index * csk_key_sz;
		uint8_t		*out_der_key = (index == CSK_ARR_SZ) ?
					kak_key : csk_key;
		size_t		output_len;
		unsigned char	output_buf[DER_BUF_SZ];
		unsigned char	*der_buf_start;

		/* Handle invalid/reserved file names */
		if (strncmp(CSK_ARR_EMPTY_FILE, fname,
			    strlen(CSK_ARR_EMPTY_FILE)) == 0) {
			if (sec_opts->csk_index == index) {
				fprintf(stderr,
					"CSK file with index %d cannot be %s\n",
					index, CSK_ARR_EMPTY_FILE);
				return 1;
			} else if (index == CSK_ARR_SZ) {
				fprintf(stderr, "KAK file name cannot be %s\n",
					CSK_ARR_EMPTY_FILE);
				return 1;
			}
			/* this key will be empty in CSK array */
			continue;
		}

		mbedtls_pk_init(&pk_ctx[index]);
		/* Read the public RSA key into the context
		 * and verify it (no password)
		 */
		if (mbedtls_pk_parse_public_keyfile(&pk_ctx[index],
						    fname) != 0) {
			fprintf(stderr,
				"Cannot read RSA public key file %s\n", fname);
			return 1;
		}

		/* Create a public key out of private one
		 * and store it in DER format
		 */
		output_len = mbedtls_pk_write_pubkey_der(&pk_ctx[index],
							 output_buf,
							 DER_BUF_SZ);
		if (output_len < 0) {
			fprintf(stderr,
				"Failed to create DER coded PUB key (%s)\n",
				fname);
			return 1;
		}

		/* Data in the output buffer is aligned to the buffer end */
		der_buf_start = output_buf + sizeof(output_buf) - output_len;
		/* In the header DER data is aligned
		 * to the start of appropriate field
		 */
		bzero(out_der_key, MAX_RSA_DER_BYTE_LEN);
		memcpy(out_der_key, der_buf_start, output_len);

	} /* for every private key file */

	return 0;

} /* end of process_public_keys */

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
		       unsigned char		*out)
{
	return mbedtls_sha256_ret(input, ilen, out, 0);
}


#endif /* MBEDTLS_BIGNUM_C && MBEDTLS_ENTROPY_C && MBEDTLS_SSL_TLS_C &&
	* MBEDTLS_SSL_CLI_C && MBEDTLS_NET_C && MBEDTLS_RSA_C &&
	* MBEDTLS_CERTS_C && MBEDTLS_PEM_PARSE_C && MBEDTLS_CTR_DRBG_C &&
	* MBEDTLS_PK_PARSE_C && MBEDTLS_FS_IO && MBEDTLS_SHA256_C &&
	* MBEDTLS_X509_CRT_PARSE_C
	*/
