/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __IMAGE_DEFS_H__
#define __IMAGE_DEFS_H__

#include <stdint.h>

#define MAX_FILENAME		256
#define CSK_ARR_SZ		16
#define CSK_ARR_EMPTY_FILE	"*"
#define AES_KEY_BIT_LEN		256
#define AES_KEY_BYTE_LEN	(AES_KEY_BIT_LEN >> 3)
#define AES_BLOCK_SZ		16
#define RSA_SIGN_BYTE_LEN	256
#define MAX_RSA_DER_BYTE_LEN	524
#define SHA_DIGEST_BYTE_LEN	32

/* Number of address pairs in control array */
#define CP_CTRL_EL_ARRAY_SZ	32

/* A8K definitions */

/* Extension header types */
#define EXT_TYPE_SECURITY	0x1
#define EXT_TYPE_BINARY		0x2

#define MAIN_HDR_MAGIC		0xB105B002

/* PROLOG alignment considerations:
 *  128B: To allow supporting XMODEM protocol.
 *  8KB: To align the boot image to the largest NAND page size, and simplify
 *  the read operations from NAND.
 *  We choose the largest page size, in order to use a single image for all
 *  NAND page sizes.
 */
#define PROLOG_ALIGNMENT	(8 << 10)

/* UART argument bitfield */
#define UART_MODE_UNMODIFIED	0x0
#define UART_MODE_DISABLE	0x1
#define UART_MODE_UPDATE	0x2

/* The main header of the Prolog */
typedef struct _main_header {
	uint32_t	magic;			/*  0-3  */
	uint32_t	prolog_size;		/*  4-7  */
	uint32_t	prolog_checksum;	/*  8-11 */
	uint32_t	boot_image_size;	/* 12-15 */
	uint32_t	boot_image_checksum;	/* 16-19 */
	uint32_t	rsrvd0;			/* 20-23 */
	uint32_t	load_addr;		/* 24-27 */
	uint32_t	exec_addr;		/* 28-31 */
	uint8_t		uart_cfg;		/*  32   */
	uint8_t		baudrate;		/*  33   */
	uint8_t		ext_count;		/*  34   */
	uint8_t		aux_flags;		/*  35   */
	uint32_t	io_arg_0;		/* 36-39 */
	uint32_t	io_arg_1;		/* 40-43 */
	uint32_t	io_arg_2;		/* 43-47 */
	uint32_t	io_arg_3;		/* 48-51 */
	uint32_t	rsrvd1;			/* 52-55 */
	uint32_t	rsrvd2;			/* 56-59 */
	uint32_t	rsrvd3;			/* 60-63 */
} header_t;

typedef struct _ext_header {
	uint8_t		type;
	uint8_t		offset;
	uint16_t	reserved;
	uint32_t	size;
} ext_header_t;

/* Secure extension */
typedef struct _sec_entry {
	uint8_t		kak_key[MAX_RSA_DER_BYTE_LEN];
	uint32_t	jtag_delay;
	uint32_t	box_id;
	uint32_t	flash_id;
	uint32_t	jtag_en;
	uint32_t	encrypt_en;
	uint32_t	efuse_dis;
	uint8_t		header_sign[RSA_SIGN_BYTE_LEN];
	uint8_t		image_sign[RSA_SIGN_BYTE_LEN];
	uint8_t		csk_keys[CSK_ARR_SZ][MAX_RSA_DER_BYTE_LEN];
	uint8_t		csk_sign[RSA_SIGN_BYTE_LEN];
	uint32_t	cp_ctrl_arr[CP_CTRL_EL_ARRAY_SZ];
	uint32_t	cp_efuse_arr[CP_CTRL_EL_ARRAY_SZ];
} sec_entry_t;

/* Trusted/secure boot configration options */
typedef struct _sec_options {
	char kak_key_file[MAX_FILENAME+1];
	char csk_key_file[CSK_ARR_SZ][MAX_FILENAME+1];
	uint32_t	box_id;
	uint32_t	flash_id;
	uint32_t	jtag_delay;
	uint8_t		csk_index;
	uint8_t		jtag_enable;
	uint8_t		efuse_disable;
	uint32_t	cp_ctrl_arr[CP_CTRL_EL_ARRAY_SZ];
	uint32_t	cp_efuse_arr[CP_CTRL_EL_ARRAY_SZ];
} sec_options;

typedef struct _options {
	char bin_ext_file[MAX_FILENAME+1];
	char sec_cfg_file[MAX_FILENAME+1];
	sec_options *sec_opts;
	uint32_t  load_addr;
	uint32_t  exec_addr;
	uint32_t  baudrate;
	uint8_t	  disable_print;
	int8_t    key_index; /* For header signatures verification only */
	uint32_t  nfc_io_args;
} options_t;

#endif /* __IMAGE_DEFS_H__ */
