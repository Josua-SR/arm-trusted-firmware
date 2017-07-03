#
# Copyright (c) 2015-2017, ARM Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

include drivers/auth/mbedtls/mbedtls_common.mk

MBEDTLS_SOURCES	+=		drivers/auth/mbedtls/mbedtls_crypto.c

# Cipher type is used to encrypt/decrypt images
ifeq (${CRYPTO_BOARD_BOOT},1)
    ifeq (${MBEDTLS_CIPHER_TYPE},aes-128-cbc)
        MBEDTLS_SOURCES	+=	$(addprefix ${MBEDTLS_DIR}/library/,	\
				aes.c					\
				)
        TBBR_CIPHER_TYPE_ID	:=	TBBR_AES_128_CBC
    else
        $(error "MBEDTLS_CIPHER_TYPE=${MBEDTLS_CIPHER_TYPE} not supported on mbedTLS")
    endif
    $(eval $(call add_define,TBBR_CIPHER_TYPE_ID))
endif
