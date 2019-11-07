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
	!defined(MBEDTLS_SHA256_C) || !defined(MBEDTLS_SSL_SRV_C) ||\
	!defined(MBEDTLS_CTR_DRBG_C) || !defined(MBEDTLS_X509_CRT_PARSE_C)
int main(void)
{
	printf("MBEDTLS_BIGNUM_C and/or MBEDTLS_ENTROPY_C and/or "
		"MBEDTLS_SSL_TLS_C and/or MBEDTLS_SSL_CLI_C and/or "
		"MBEDTLS_NET_C and/or MBEDTLS_RSA_C and/or "
		"MBEDTLS_CERTS_C and/or MBEDTLS_PEM_PARSE_C and/or "
		"MBEDTLS_PK_PARSE_C and/or MBEDTLS_FS_IO and/or "
		"MBEDTLS_SHA256_C and/or MBEDTLS_SSL_SRV_C and/or "
		"MBEDTLS_CTR_DRBG_C and/or MBEDTLS_X509_CRT_PARSE_C "
		"not defined.\n");
	return 0;
}

#else  /* MBEDTLS_BIGNUM_C && MBEDTLS_ENTROPY_C && MBEDTLS_SSL_TLS_C &&
	* MBEDTLS_SSL_CLI_C && MBEDTLS_NET_C && MBEDTLS_RSA_C &&
	* MBEDTLS_CERTS_C && MBEDTLS_PEM_PARSE_C && MBEDTLS_CTR_DRBG_C &&
	* MBEDTLS_PK_PARSE_C && MBEDTLS_FS_IO && MBEDTLS_SHA256_C &&
	* MBEDTLS_SSL_SRV_C && MBEDTLS_X509_CRT_PARSE_C
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
#include <mbedtls/sha256.h>
#include <mbedtls/ssl.h>
#if defined(MBEDTLS_SSL_CACHE_C)
#include "mbedtls/ssl_cache.h"
#endif
#include <mbedtls/x509.h>

#include <libconfig.h>	/* for parsing config file */
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "server_sec.h"

#define SRV_VERSION_STRING	"Marvell(C) doimage signage server version 1.0"

#define DEBUG_LEVEL		0

/* Locals */
static mbedtls_pk_context prv_pk_ctx[CSK_ARR_SZ + 1];
static srv_sec_options srv_sec_opt;


static void server_debug(void *ctx, int level,
			 const char *file, int line,
			 const char *str)
{
	fprintf((FILE *)ctx, "%s:%04d: %s", file, line, str);
	fflush((FILE *)ctx);
}

static void usage_err(char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	fprintf(stderr, "run with '-h' argument to get usage information\n");
	exit(-1);
}

static void usage(void)
{
	printf("\n\n%s\n\n", SRV_VERSION_STRING);
	printf("Supported arguments:\n");
	printf("  -c <cfg_file>   Run signage server using parameters\n");
	printf("                  obtained from the configuration file.\n");
	printf("  -h              Show this help message\n");

	exit(-1);
}

/*******************************************************************************
 *    verify_and_copy_file_name_entry
 *    INPUT:
 *          element_name
 *          element
 *    OUTPUT:
 *          copy_to
 *    RETURN:
 *          0 on success
 ******************************************************************************
 */
static int verify_and_copy_file_name_entry(const char *element_name,
					   const char *element, char *copy_to)
{
	int element_length = strlen(element);

	if (element_length >= MAX_FILENAME) {
		fprintf(stderr, "The file name %s for %s is too long (%d). ",
			element, element_name, element_length);
		fprintf(stderr, "Maximum allowed %d characters!\n",
			MAX_FILENAME);
		return -1;
	} else if (element_length == 0) {
		fprintf(stderr, "The file name for %s is empty!\n",
			element_name);
		return -1;
	}
	memcpy(copy_to, element, element_length);

	return 0;
}

/*******************************************************************************
 *    server_create_signature
 *          Create RSA signature (PKCS 2.1)
 *    INPUT:
 *          pk_ctx_idx    Index of pre-initialized Public Key context
 *          rng_ctx       initialized and seeded RNG context
 *          hash        incoming STA256 hash
 *    OUTPUT:
 *          signature     RSA signature created out of digest
 *    RETURN:
 *          0 on success
 ******************************************************************************
 */
static int server_create_signature(uint16_t			pk_ctx_idx,
				   mbedtls_ctr_drbg_context	*rng_ctx,
				   const unsigned char		*hash,
				   uint8_t			*signature)
{
	/* The PK context should be already initialized.
	 * Set the padding type for this PK context
	 */
	mbedtls_rsa_set_padding(mbedtls_pk_rsa(prv_pk_ctx[pk_ctx_idx]),
				MBEDTLS_RSA_PKCS_V21, MBEDTLS_MD_SHA256);


	/* Calculate the hash signature */
	return mbedtls_rsa_rsassa_pss_sign(mbedtls_pk_rsa(
						prv_pk_ctx[pk_ctx_idx]),
					   mbedtls_ctr_drbg_random,
					   rng_ctx, MBEDTLS_RSA_PRIVATE,
					   MBEDTLS_MD_SHA256, 0, hash,
					   signature);
} /* end of server_create_signature */

/*******************************************************************************
 *    server_start
 *          start the SSL server and manage client connections
 *    INPUT:
 *          none
 *    OUTPUT:
 *          none
 *    RETURN:
 *          0 on success
 ******************************************************************************
 */
static int server_start(void)
{
	int ret;
	size_t base64_len, rx_len;
	mbedtls_net_context listen_fd, client_fd;
	unsigned char buf[MSG_MAX_SZ];
	const char *pers = "doimage_server";
	sign_msg msg;

	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
	mbedtls_x509_crt srv_cert;
	mbedtls_pk_context pkey;
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_context cache;
#endif

	mbedtls_net_init(&listen_fd);
	mbedtls_net_init(&client_fd);
	mbedtls_ssl_init(&ssl);
	mbedtls_ssl_config_init(&conf);
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_init(&cache);
#endif
	mbedtls_x509_crt_init(&srv_cert);
	mbedtls_pk_init(&pkey);
	mbedtls_entropy_init(&entropy);
	mbedtls_ctr_drbg_init(&ctr_drbg);

#if defined(MBEDTLS_DEBUG_C)
	mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

	/* 1. Load the certificates and private RSA key */
	ret = mbedtls_x509_crt_parse(&srv_cert,
				(const unsigned char *)mbedtls_test_srv_crt,
				mbedtls_test_srv_crt_len);
	if (ret) {
		fprintf(stderr, "Failed to parse X509 server certificate!\n");
		goto server_err;
	}

	ret = mbedtls_x509_crt_parse(&srv_cert,
				(const unsigned char *)mbedtls_test_cas_pem,
				mbedtls_test_cas_pem_len);
	if (ret) {
		fprintf(stderr, "Failed to parse X509 CA certificate!\n");
		goto server_err;
	}

	ret =  mbedtls_pk_parse_key(&pkey,
				(const unsigned char *)mbedtls_test_srv_key,
				mbedtls_test_srv_key_len, NULL, 0);
	if (ret) {
		fprintf(stderr, "Failed to parse server private key!\n");
		goto server_err;
	}

	/* 2. Setup the listening TCP socket */
	ret = mbedtls_net_bind(&listen_fd, NULL, srv_sec_opt.server_port,
			       MBEDTLS_NET_PROTO_TCP);
	if (ret) {
		fprintf(stderr, "Failed to bind to network port %s!\n",
			srv_sec_opt.server_port);
		goto server_err;
	}

	/*  3. Seed the RNG */
	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
				    (const unsigned char *)pers,
				    strlen(pers));
	if (ret) {
		fprintf(stderr, "Failed to seed the RNG!\n");
		goto server_err;
	}

	/* 4. Setup SSL */
	ret = mbedtls_ssl_config_defaults(&conf,
					  MBEDTLS_SSL_IS_SERVER,
					  MBEDTLS_SSL_TRANSPORT_STREAM,
					  MBEDTLS_SSL_PRESET_DEFAULT);
	if (ret) {
		fprintf(stderr, "Failed to setup SSL defaults!\n");
		goto server_err;
	}

	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
	mbedtls_ssl_conf_dbg(&conf, server_debug, stdout);

#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_conf_session_cache(&conf, &cache,
				       mbedtls_ssl_cache_get,
				       mbedtls_ssl_cache_set);
#endif

	mbedtls_ssl_conf_ca_chain(&conf, srv_cert.next, NULL);
	ret = mbedtls_ssl_conf_own_cert(&conf, &srv_cert, &pkey);
	if (ret) {
		fprintf(stderr, "Failed to setup server SSL cetificate!\n");
		goto server_err;
	}

	ret = mbedtls_ssl_setup(&ssl, &conf);
	if (ret) {
		fprintf(stderr, "Failed to setup SSL connection!\n");
		goto server_err;
	}


server_reset:
	/* Main server loop */
#ifdef MBEDTLS_ERROR_C
	if (ret) {
		mbedtls_strerror(ret, (char *)buf, 100);
		fprintf(stderr, "Last error was: %d - %s\n\n", ret, buf);
	}
#endif

	mbedtls_net_free(&client_fd);
	mbedtls_ssl_session_reset(&ssl);

	/* 5. Wait until a client connects */
	ret = mbedtls_net_accept(&listen_fd, &client_fd, NULL, 0, NULL);
	if (ret) {
		fprintf(stderr, "Failed to accept client connection!\n");
		goto server_err;
	}

	mbedtls_ssl_set_bio(&ssl, &client_fd,
			    mbedtls_net_send, mbedtls_net_recv, NULL);

	/* 6. Handshake */
	do {
		ret = mbedtls_ssl_handshake(&ssl);
		if (ret != 0 &&
		    ret != MBEDTLS_ERR_SSL_WANT_READ &&
		    ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			fprintf(stderr, "Failed to run SSL handshake!\n");
			goto server_reset;
		}
	} while (ret);

	/* 7. Read the client request */
	do {
		memset(buf, 0, sizeof(buf));
		ret = mbedtls_ssl_read(&ssl, buf, sizeof(buf) - 1);

		if (ret == MBEDTLS_ERR_SSL_WANT_READ ||
		    ret == MBEDTLS_ERR_SSL_WANT_WRITE)
			continue;

		if (ret <= 0) {
			switch (ret) {
			case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
				fprintf(stderr,
					"Gracefull connection closure\n");
				break;

			case MBEDTLS_ERR_NET_CONN_RESET:
				fprintf(stderr,
					"Connection was reset by peer\n");
				goto server_reset;

			default:
				fprintf(stderr,
					"Failed to get data from client (%d)!\n",
					ret);
				break;
			}
		}
	} while (ret <= 0);

	/* Decode incoming client message */
	rx_len = ret;
	ret = mbedtls_base64_decode((unsigned char *)&msg, sizeof(msg),
				    &base64_len, buf, rx_len);
	if (ret) {
		fprintf(stderr, "Failed to convert input message!\n");
		goto server_reset;
	}
	/* Message sanity check */
	if ((msg.magic  != SIGN_MSG_MAGIC) ||
	    (msg.index  >  CSK_ARR_SZ) ||
	    (msg.type   != MSG_TYP_SIGN_REQEST) ||
	    (msg.length != SHA_DIGEST_BYTE_LEN)) {
		fprintf(stderr, "Bad message received from client!\n");
		fprintf(stderr, "\tHDR Magic\t%#10x\tKey Index\t%#06x\n",
			msg.magic, msg.index);
		fprintf(stderr, "\tMsg Type\t%#06x\tMsg length\t%#10x\n",
			msg.type, msg.length);
		goto server_reset;
	}

	/*  8. Create signature and send it back to the client */
	printf("Create signature for key index %d (%s)\n", msg.index,
	       msg.index == CSK_ARR_SZ ? "KAK" : "CSK");
	ret = server_create_signature(msg.index, &ctr_drbg, msg.sign, buf);
	if (ret) {
		fprintf(stderr, "Failed to create RSA signature!\n");
		goto server_reset;
	}

	/* Prepare the outgoing message */
	msg.type  = MSG_TYP_SIGN_RETURN;
	msg.length = RSA_SIGN_BYTE_LEN;
	memcpy(msg.sign, buf, RSA_SIGN_BYTE_LEN);
	memset(buf, 0, sizeof(buf));

	/* Encode the outgoing message */
	ret = mbedtls_base64_encode(buf, sizeof(buf) - 1, &base64_len,
				    (unsigned char *)&msg, sizeof(msg));
	if (ret) {
		fprintf(stderr, "Failed to compose output message!\n");
		goto server_reset;
	}

	while ((ret = mbedtls_ssl_write(&ssl, buf, base64_len)) <= 0) {
		if (ret == MBEDTLS_ERR_NET_CONN_RESET) {
			fprintf(stderr,
				"Failed to respond, connection closed by peer!\n");
			goto server_reset;
		}

		if (ret != MBEDTLS_ERR_SSL_WANT_READ &&
		    ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			fprintf(stderr,
				"Failed to send data back to client (%d)!\n",
				ret);
			goto server_err;
		}
	}

	ret = 0;
	goto server_reset;

server_err:

#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		mbedtls_strerror(ret, (char *)buf, sizeof(buf));
		fprintf(stderr, "Last error was: %d - %s\n\n", ret, buf);
	}
#endif

	mbedtls_net_free(&client_fd);
	mbedtls_net_free(&listen_fd);

	mbedtls_x509_crt_free(&srv_cert);
	mbedtls_pk_free(&pkey);
	mbedtls_ssl_free(&ssl);
	mbedtls_ssl_config_free(&conf);
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_free(&cache);
#endif
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);

	return ret;
} /* end of server_start */

/*******************************************************************************
 *    process_private_keys
 *           read private keys from PEM files
 *    INPUT:
 *          none
 *    OUTPUT:
 *          none
 *    RETURN:
 *          0 on success
 ******************************************************************************
 */
static int process_private_keys(void)
{
	int		index;

	/* Bring up RSA context and read public keys from their files */
	for (index = 0; index < (CSK_ARR_SZ + 1); index++) {
		/* for every private key file */
		char		*fname = (index == CSK_ARR_SZ) ?
					srv_sec_opt.kak_key_file :
					srv_sec_opt.csk_key_file[index];

		/* Handle invalid/reserved file names */
		if (strncmp(CSK_ARR_EMPTY_FILE, fname,
			    strlen(CSK_ARR_EMPTY_FILE)) == 0) {
			if (index == CSK_ARR_SZ) {
				fprintf(stderr, "KAK file name cannot be %s\n",
					CSK_ARR_EMPTY_FILE);
				return 1;
			}
			/* this key will be empty in CSK array */
			continue;
		}

		mbedtls_pk_init(&prv_pk_ctx[index]);
		/* Read the public RSA key into the context
		 * and verify it (no password)
		 */
		if (mbedtls_pk_parse_keyfile(&prv_pk_ctx[index],
					     fname, "") != 0) {
			fprintf(stderr,
				"Cannot read RSA private key file %s\n", fname);
			return 1;
		}
	} /* for every private key file */

	return 0;

} /* end of process_private_keys */

/*******************************************************************************
 *    parse_server_config_file
 *          Read the server configuration from a file
 *          into internal structures
 *    INPUT:
 *          filename      File name
 *    OUTPUT:
 *          none
 *    RETURN:
 *          0 on success
 ******************************************************************************
 */
static int parse_server_config_file(char *filename)
{
	config_t		sec_cfg;
	int			array_sz, element, rval = -1;
	const char		*cfg_string;
	int32_t			cfg_int32;
	const config_setting_t	*csk_array;

	config_init(&sec_cfg);

	if (config_read_file(&sec_cfg, filename) != CONFIG_TRUE) {
		fprintf(stderr, "Failed to read data from config file ");
		fprintf(stderr, "%s\n\t%s at line %d\n",
			filename, config_error_text(&sec_cfg),
			config_error_line(&sec_cfg));
		goto exit_parse;
	}

	/* KAK file name */
	if (config_lookup_string(&sec_cfg, "kak_key_file",
				 &cfg_string) != CONFIG_TRUE) {
		fprintf(stderr, "The \"kak_key_file\" undefined!\n");
		goto exit_parse;
	}
	if (verify_and_copy_file_name_entry("kak_key_file",
					    cfg_string,
					    srv_sec_opt.kak_key_file))
		goto exit_parse;

	/* CSK file names array */
	csk_array = config_lookup(&sec_cfg, "csk_key_file");
	if (csk_array == NULL) {
		fprintf(stderr, "The \"csk_key_file\" undefined!\n");
		goto exit_parse;
	}
	array_sz = config_setting_length(csk_array);
	if (array_sz > CSK_ARR_SZ) {
		fprintf(stderr, "The \"csk_key_file\" array is too big! ");
		fprintf(stderr, "Only first %d elements will be used\n",
			CSK_ARR_SZ);
		array_sz = CSK_ARR_SZ;
	} else if (array_sz == 0) {
		fprintf(stderr, "The \"csk_key_file\" array is empty!\n");
		goto exit_parse;
	}

	for (element = 0; element < array_sz; element++) {
		cfg_string = config_setting_get_string_elem(csk_array, element);
		if (verify_and_copy_file_name_entry(
				"csk_key_file", cfg_string,
				srv_sec_opt.csk_key_file[element])) {
			fprintf(stderr, "Bad csk_key_file[%d] entry!\n",
				element);
			goto exit_parse;
		}
	}

	/* Signage Server port */
	if (config_lookup_int(&sec_cfg, "server_port",
			      &cfg_int32) != CONFIG_TRUE) {
		fprintf(stderr, "The \"server_port\" is undefined,\n");
		fprintf(stderr, "using default port %d\n", DEFAULT_SERVER_PORT);
		cfg_int32 = DEFAULT_SERVER_PORT;
	}
	snprintf(srv_sec_opt.server_port, MAX_PORT_LENGTH, "%d", cfg_int32);

	rval = process_private_keys();

exit_parse:
	config_destroy(&sec_cfg);

	return rval;
} /* end of parse_server_config_file */

/*******************************************************************************
 * main()
 *******************************************************************************
 */
int main(int argc, char *argv[])
{
	char cfg_file[MAX_FILENAME + 1];
	int opt;
	int ret = EXIT_SUCCESS;

	printf("Starting %s\n\n", SRV_VERSION_STRING);

	memset(cfg_file, 0, sizeof(cfg_file));
	while ((opt = getopt(argc, argv, "hc:")) != -1) {
		switch (opt) {
		case 'h':
			usage();
			break;
		case 'c':
			strncpy(cfg_file, optarg, MAX_FILENAME);
			break;
		default:
			usage_err("Unknown argument");
			exit(EXIT_FAILURE);
		}
	}

	/* First, parse the configuration file */
	if (parse_server_config_file(cfg_file)) {
		fprintf(stderr,
			"failed parsing configuration file %s\n", cfg_file);
		exit(EXIT_FAILURE);
	}

	ret = server_start();

	printf("Server stop (%d)\n", ret);
	exit(ret);
}

#endif /* MBEDTLS_BIGNUM_C && MBEDTLS_ENTROPY_C && MBEDTLS_SSL_TLS_C &&
	* MBEDTLS_SSL_CLI_C && MBEDTLS_NET_C && MBEDTLS_RSA_C &&
	* MBEDTLS_CERTS_C && MBEDTLS_PEM_PARSE_C && MBEDTLS_CTR_DRBG_C &&
	* MBEDTLS_PK_PARSE_C && MBEDTLS_FS_IO && MBEDTLS_SHA256_C &&
	* MBEDTLS_SSL_SRV_C && MBEDTLS_X509_CRT_PARSE_C
	*/
