/*
 * Copyright (C) 2017 Marvell International Ltd.
 * This program is provided "as is" without any warranty of any kind,
 * and is distributed under the applicable Marvell proprietary limited use
 * license agreement.
 */

#include <drivers/auth/auth_mod.h>
#include <drivers/auth/crypto_mod.h>
#include <platform_def.h>
#include <tools_share/tbbr_oid.h>
#include <stddef.h>

/*
 * Maximum key and hash sizes (in DER format)
 */
#define PK_DER_LEN			294
#define HASH_DER_LEN			51

/*
 * The platform must allocate buffers to store the authentication parameters
 * extracted from the certificates. In this case, because of the way the CoT is
 * established, we can reuse some of the buffers on different stages
 */
static unsigned char tb_fw_hash_buf[HASH_DER_LEN];
static unsigned char soc_fw_hash_buf[HASH_DER_LEN];
static unsigned char nt_world_bl_hash_buf[HASH_DER_LEN];
static unsigned char trusted_world_pk_buf[PK_DER_LEN];
static unsigned char non_trusted_world_pk_buf[PK_DER_LEN];
static unsigned char content_pk_buf[PK_DER_LEN];
#ifdef NT_FW_CONFIG
static unsigned char nt_fw_config_hash_buf[HASH_DER_LEN];
#endif
/*
 * Parameter type descriptors
 */
static auth_param_type_desc_t trusted_nv_ctr = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_NV_CTR, TRUSTED_FW_NVCOUNTER_OID);
static auth_param_type_desc_t non_trusted_nv_ctr = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_NV_CTR, NON_TRUSTED_FW_NVCOUNTER_OID);

static auth_param_type_desc_t subject_pk = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_PUB_KEY, 0);
static auth_param_type_desc_t sig = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_SIG, 0);
static auth_param_type_desc_t sig_alg = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_SIG_ALG, 0);
static auth_param_type_desc_t raw_data = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_RAW_DATA, 0);

static auth_param_type_desc_t trusted_world_pk = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_PUB_KEY, TRUSTED_WORLD_PK_OID);
static auth_param_type_desc_t non_trusted_world_pk = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_PUB_KEY, NON_TRUSTED_WORLD_PK_OID);

static auth_param_type_desc_t soc_fw_content_pk = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_PUB_KEY, SOC_FW_CONTENT_CERT_PK_OID);
static auth_param_type_desc_t nt_fw_content_pk = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_PUB_KEY, NON_TRUSTED_FW_CONTENT_CERT_PK_OID);

static auth_param_type_desc_t tb_fw_hash = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_HASH, TRUSTED_BOOT_FW_HASH_OID);
static auth_param_type_desc_t soc_fw_hash = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_HASH, SOC_AP_FW_HASH_OID);
static auth_param_type_desc_t nt_world_bl_hash = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_HASH, NON_TRUSTED_WORLD_BOOTLOADER_HASH_OID);
#ifdef NT_FW_CONFIG
static auth_param_type_desc_t nt_fw_config_hash = AUTH_PARAM_TYPE_DESC(
		AUTH_PARAM_HASH, NON_TRUSTED_FW_CONFIG_HASH_OID);
#endif

/*
 * BL2
 */
static const auth_img_desc_t trusted_boot_fw_cert = {
		.img_id = TRUSTED_BOOT_FW_CERT_ID,
		.img_type = IMG_CERT,
		.parent = NULL,
		.img_auth_methods = (const auth_method_desc_t[AUTH_METHOD_NUM]) {
			[0] = {
				.type = AUTH_METHOD_SIG,
				.param.sig = {
					.pk = &subject_pk,
					.sig = &sig,
					.alg = &sig_alg,
					.data = &raw_data,
				}
			},
			[1] = {
				.type = AUTH_METHOD_NV_CTR,
				.param.nv_ctr = {
					.cert_nv_ctr = &trusted_nv_ctr,
					.plat_nv_ctr = &trusted_nv_ctr
				}
			}
		},
		.authenticated_data = (const auth_param_desc_t[COT_MAX_VERIFIED_PARAMS]) {
			[0] = {
				.type_desc = &tb_fw_hash,
				.data = {
					.ptr = (void *)tb_fw_hash_buf,
					.len = (unsigned int)HASH_DER_LEN
				}
			}
		}
	};

static const auth_img_desc_t bl2_image = {
		.img_id = BL2_IMAGE_ID,
		.img_type = IMG_RAW,
		.parent = &trusted_boot_fw_cert,
		.img_auth_methods = (const auth_method_desc_t[AUTH_METHOD_NUM]) {
			[0] = {
				.type = AUTH_METHOD_HASH,
				.param.hash = {
					.data = &raw_data,
					.hash = &tb_fw_hash,
				}
			}
		}
	};

/*
 * Trusted key certificate
 */
static const auth_img_desc_t trusted_key_cert = {
		.img_id = TRUSTED_KEY_CERT_ID,
		.img_type = IMG_CERT,
		.parent = NULL,
		.img_auth_methods = (const auth_method_desc_t[AUTH_METHOD_NUM]) {
			[0] = {
				.type = AUTH_METHOD_SIG,
				.param.sig = {
					.pk = &subject_pk,
					.sig = &sig,
					.alg = &sig_alg,
					.data = &raw_data,
				}
			},
			[1] = {
				.type = AUTH_METHOD_NV_CTR,
				.param.nv_ctr = {
					.cert_nv_ctr = &trusted_nv_ctr,
					.plat_nv_ctr = &trusted_nv_ctr
				}
			}
		},
		.authenticated_data = (const auth_param_desc_t[COT_MAX_VERIFIED_PARAMS]) {
			[0] = {
				.type_desc = &trusted_world_pk,
				.data = {
					.ptr = (void *)trusted_world_pk_buf,
					.len = (unsigned int)PK_DER_LEN
				}
			},
			[1] = {
				.type_desc = &non_trusted_world_pk,
				.data = {
					.ptr = (void *)non_trusted_world_pk_buf,
					.len = (unsigned int)PK_DER_LEN
				}
			}
		}
	};

/*
 * SoC Firmware
 */
static const auth_img_desc_t soc_fw_key_cert = {
		.img_id = SOC_FW_KEY_CERT_ID,
		.img_type = IMG_CERT,
		.parent = &trusted_key_cert,
		.img_auth_methods = (const auth_method_desc_t[AUTH_METHOD_NUM]) {
			[0] = {
				.type = AUTH_METHOD_SIG,
				.param.sig = {
					.pk = &trusted_world_pk,
					.sig = &sig,
					.alg = &sig_alg,
					.data = &raw_data,
				}
			},
			[1] = {
				.type = AUTH_METHOD_NV_CTR,
				.param.nv_ctr = {
					.cert_nv_ctr = &trusted_nv_ctr,
					.plat_nv_ctr = &trusted_nv_ctr
				}
			}
		},
		.authenticated_data = (const auth_param_desc_t[COT_MAX_VERIFIED_PARAMS]) {
			[0] = {
				.type_desc = &soc_fw_content_pk,
				.data = {
					.ptr = (void *)content_pk_buf,
					.len = (unsigned int)PK_DER_LEN
				}
			}
		}
	};

static const auth_img_desc_t soc_fw_content_cert = {
		.img_id = SOC_FW_CONTENT_CERT_ID,
		.img_type = IMG_CERT,
		.parent = &soc_fw_key_cert,
		.img_auth_methods = (const auth_method_desc_t[AUTH_METHOD_NUM]) {
			[0] = {
				.type = AUTH_METHOD_SIG,
				.param.sig = {
					.pk = &soc_fw_content_pk,
					.sig = &sig,
					.alg = &sig_alg,
					.data = &raw_data,
				}
			},
			[1] = {
				.type = AUTH_METHOD_NV_CTR,
				.param.nv_ctr = {
					.cert_nv_ctr = &trusted_nv_ctr,
					.plat_nv_ctr = &trusted_nv_ctr
				}
			}
		},
		.authenticated_data = (const auth_param_desc_t[COT_MAX_VERIFIED_PARAMS]) {
			[0] = {
				.type_desc = &soc_fw_hash,
				.data = {
					.ptr = (void *)soc_fw_hash_buf,
					.len = (unsigned int)HASH_DER_LEN
				}
			}
		}
	};

static const auth_img_desc_t bl31_image = {
		.img_id = BL31_IMAGE_ID,
		.img_type = IMG_RAW,
		.parent = &soc_fw_content_cert,
		.img_auth_methods = (const auth_method_desc_t[AUTH_METHOD_NUM]) {
			[0] = {
				.type = AUTH_METHOD_HASH,
				.param.hash = {
					.data = &raw_data,
					.hash = &soc_fw_hash,
				}
			}
		}
	};

/*
 * Non-Trusted Firmware
 */
static const auth_img_desc_t non_trusted_fw_key_cert = {
		.img_id = NON_TRUSTED_FW_KEY_CERT_ID,
		.img_type = IMG_CERT,
		.parent = &trusted_key_cert,
		.img_auth_methods = (const auth_method_desc_t[AUTH_METHOD_NUM]) {
			[0] = {
				.type = AUTH_METHOD_SIG,
				.param.sig = {
					.pk = &non_trusted_world_pk,
					.sig = &sig,
					.alg = &sig_alg,
					.data = &raw_data,
				}
			},
			[1] = {
				.type = AUTH_METHOD_NV_CTR,
				.param.nv_ctr = {
					.cert_nv_ctr = &non_trusted_nv_ctr,
					.plat_nv_ctr = &non_trusted_nv_ctr
				}
			}
		},
		.authenticated_data = (const auth_param_desc_t[COT_MAX_VERIFIED_PARAMS]) {
			[0] = {
				.type_desc = &nt_fw_content_pk,
				.data = {
					.ptr = (void *)content_pk_buf,
					.len = (unsigned int)PK_DER_LEN
				}
			}
		}
	};

static const auth_img_desc_t non_trusted_fw_content_cert = {
		.img_id = NON_TRUSTED_FW_CONTENT_CERT_ID,
		.img_type = IMG_CERT,
		.parent = &non_trusted_fw_key_cert,
		.img_auth_methods = (const auth_method_desc_t[AUTH_METHOD_NUM]) {
			[0] = {
				.type = AUTH_METHOD_SIG,
				.param.sig = {
					.pk = &nt_fw_content_pk,
					.sig = &sig,
					.alg = &sig_alg,
					.data = &raw_data,
				}
			},
			[1] = {
				.type = AUTH_METHOD_NV_CTR,
				.param.nv_ctr = {
					.cert_nv_ctr = &non_trusted_nv_ctr,
					.plat_nv_ctr = &non_trusted_nv_ctr
				}
			}
		},
		.authenticated_data = (const auth_param_desc_t[COT_MAX_VERIFIED_PARAMS]) {
			[0] = {
				.type_desc = &nt_world_bl_hash,
				.data = {
					.ptr = (void *)nt_world_bl_hash_buf,
					.len = (unsigned int)HASH_DER_LEN
				}
			},
#ifdef NT_FW_CONFIG
			[1] = {
				.type_desc = &nt_fw_config_hash,
				.data = {
					.ptr = (void *)nt_fw_config_hash_buf,
					.len = (unsigned int)HASH_DER_LEN
				}
			}
#endif
		}
	};

static const auth_img_desc_t bl33_image = {
		.img_id = BL33_IMAGE_ID,
		.img_type = IMG_RAW,
		.parent = &non_trusted_fw_content_cert,
		.img_auth_methods = (const auth_method_desc_t[AUTH_METHOD_NUM]) {
			[0] = {
				.type = AUTH_METHOD_HASH,
				.param.hash = {
					.data = &raw_data,
					.hash = &nt_world_bl_hash,
				}
			}
		}
	};

#ifdef NT_FW_CONFIG
/*
 * NT FW Config
 */
static const auth_img_desc_t nt_fw_config = {
		.img_id = NT_FW_CONFIG_ID,
		.img_type = IMG_RAW,
		.parent = &non_trusted_fw_content_cert,
		.img_auth_methods = (const auth_method_desc_t[AUTH_METHOD_NUM]) {
			[0] = {
				.type = AUTH_METHOD_HASH,
				.param.hash = {
					.data = &raw_data,
					.hash = &nt_fw_config_hash,
				}
			}
		}
	};
#endif /* NT_FW_CONFIG */

/*
 * TBBR Chain of trust definition
 */
static const auth_img_desc_t * const cot_desc[] = {
	[TRUSTED_BOOT_FW_CERT_ID]		= &trusted_boot_fw_cert,
	[BL2_IMAGE_ID]				= &bl2_image,
	[TRUSTED_KEY_CERT_ID]			= &trusted_key_cert,
	[SOC_FW_KEY_CERT_ID]			= &soc_fw_key_cert,
	[SOC_FW_CONTENT_CERT_ID]		= &soc_fw_content_cert,
	[BL31_IMAGE_ID]				= &bl31_image,
	[NON_TRUSTED_FW_KEY_CERT_ID]		= &non_trusted_fw_key_cert,
	[NON_TRUSTED_FW_CONTENT_CERT_ID]	= &non_trusted_fw_content_cert,
	[BL33_IMAGE_ID]				= &bl33_image,
#ifdef NT_FW_CONFIG
	[NT_FW_CONFIG_ID]			= &nt_fw_config,
#endif /* NT_FW_CONFIG */
};

/*
 * Register the images that needs to be encrypted at build time
 * and decrypted during runtime.
 * Definitions and IDs of those images HAS TO match the
 * TBBR CoT defined above.
 */
#if CRYPTO_BOARD_BOOT
static const crypto_img_desc_t crypto_params_desc [] = {
	[BL2_IMAGE_ID] = {
		.img_id = BL2_IMAGE_ID,
		.img_type = IMG_RAW,
		.tbbr_cipher_type_id = TBBR_AES_128_CBC,
	},
	[BL31_IMAGE_ID] = {
		.img_id = BL31_IMAGE_ID,
		.img_type = IMG_RAW,
		.tbbr_cipher_type_id = TBBR_AES_128_CBC,
	},
	[BL33_IMAGE_ID] = {
		.img_id = BL33_IMAGE_ID,
		.img_type = IMG_RAW,
		.tbbr_cipher_type_id = TBBR_AES_128_CBC,
	}
};

/* Register the crypto parameters in the cryptographic module */
REGISTER_CRYPTO_PARAMS(crypto_params_desc);

#endif /* CRYPTO_BOARD_BOOT */

/* Register the CoT in the authentication module */
REGISTER_COT(cot_desc);
