/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <libconfig.h>	/* for parsing config file */
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "client_sec.h"

#define CLIENT_VERSION_STRING	"Marvell(C) doimage utility (client) version 4.0"

#define uart_set_mode(arg, mode)	(arg |= (mode & 0x3))

static options_t opts = {
	.bin_ext_file = "NA",
	.sec_cfg_file = "NA",
	.sec_opts = 0,
	.load_addr = 0x0,
	.exec_addr = 0x0,
	.disable_print = 0,
	.baudrate = 0,
	.key_index = -1,
};

static void usage_err(char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	fprintf(stderr, "run 'doimage -h' to get usage information\n");
	exit(-1);
}

static void usage(void)
{
	printf("\n\n%s\n\n", CLIENT_VERSION_STRING);
	printf("Usage: doimage [options] <input_file> [output_file]\n");
	printf("create bootrom image from u-boot and boot extensions\n\n");

	printf("Arguments\n");
	printf("  input_file   name of boot image file.\n");
	printf("               if -p is used, name of the bootrom image file");
	printf("               to parse.\n");
	printf("  output_file  name of output bootrom image file\n");

	printf("\nOptions\n");
	printf("  -s        target SOC name. supports a8020,a7020\n");
	printf("            different SOCs may have different boot image\n");
	printf("            format so it's mandatory to know the target SOC\n");
	printf("  -i        boot I/F name. supports nand, spi, nor\n");
	printf("            This affects certain parameters coded in the\n");
	printf("            image header\n");
	printf("  -l        boot image load address. default is 0x0\n");
	printf("  -e        boot image entry address. default is 0x0\n");
	printf("  -b        binary extension image file.\n");
	printf("            This image is executed before the boot image.\n");
	printf("            This is typically used to initialize the memory ");
	printf("            controller.\n");
	printf("            Currently supports only a single file.\n");
	printf("  -c        Make trusted boot image using parameters\n");
	printf("            from the configuration file.\n");
	printf("  -p        Parse and display a pre-built boot image\n");
	printf("  -k        Key index for RSA signatures verification\n");
	printf("            when parsing the boot image\n");
	printf("  -m        Disable prints of bootrom and binary extension\n");
	printf("  -u        UART baudrate used for bootrom prints.\n");
	printf("            Must be multiple of 1200\n");
	printf("  -h        Show this help message\n");
	printf(" IO-ROM NFC-NAND boot parameters:\n");
	printf("  -n        NAND device block size in KB [Default is 64KB].\n");
	printf("  -t        NAND cell technology (SLC [Default] or MLC)\n");

	exit(-1);
}

static int get_file_size(char *filename)
{
	struct stat st;

	if (stat(filename, &st) == 0)
		return st.st_size;

	return -1;
}

static uint32_t checksum32(uint32_t *start, int len)
{
	uint32_t sum = 0;
	uint32_t *startp = start;

	do {
		sum += *startp;
		startp++;
		len -= 4;
	} while (len > 0);

	return sum;
}

/*******************************************************************************
 *    verify_secure_header_signatures
 *          Verify CSK array, header and image signatures and print results
 *    INPUT:
 *          main_hdr       Main header
 *          sec_ext        Secure extension
 *    OUTPUT:
 *          none
 *    RETURN:
 *          0 on success
 *******************************************************************************
 */
static int verify_secure_header_signatures(header_t *main_hdr,
					   sec_entry_t *sec_ext)
{
	uint8_t	*image = (uint8_t *)main_hdr + main_hdr->prolog_size;
	uint8_t	signature[RSA_SIGN_BYTE_LEN];
	int	rval = -1;

	/* Save headers signature and reset it in the secure header */
	memcpy(signature, sec_ext->header_sign, RSA_SIGN_BYTE_LEN);
	memset(sec_ext->header_sign, 0, RSA_SIGN_BYTE_LEN);

	fprintf(stdout, "\nCheck RSA Signatures\n");
	fprintf(stdout, "#########################\n");
	fprintf(stdout, "CSK Block Signature: ");
	if (verify_rsa_signature(sec_ext->kak_key,
				 MAX_RSA_DER_BYTE_LEN,
				 &sec_ext->csk_keys[0][0],
				 sizeof(sec_ext->csk_keys),
				 "CSK Block Signature: ",
				 sec_ext->csk_sign) != 0) {
		fprintf(stdout, "ERROR\n");
		goto ver_error;
	}
	fprintf(stdout, "OK\n");

	if (opts.key_index != -1) {
		fprintf(stdout, "Image Signature:     ");
		if (verify_rsa_signature(sec_ext->csk_keys[opts.key_index],
					 MAX_RSA_DER_BYTE_LEN,
					 image, main_hdr->boot_image_size,
					 "Image Signature: ",
					 sec_ext->image_sign) != 0) {
			fprintf(stdout, "ERROR\n");
			goto ver_error;
		}
		fprintf(stdout, "OK\n");

		fprintf(stdout, "Header Signature:    ");
		if (verify_rsa_signature(sec_ext->csk_keys[opts.key_index],
					 MAX_RSA_DER_BYTE_LEN,
					 (uint8_t *)main_hdr,
					 main_hdr->prolog_size,
					 "Header Signature: ",
					 signature) != 0) {
			fprintf(stdout, "ERROR\n");
			goto ver_error;
		}
		fprintf(stdout, "OK\n");
	} else {
		fprintf(stdout, "SKIP Image and Header Signatures");
		fprintf(stdout, " check (undefined key index)\n");
	}

	rval = 0;

ver_error:
	memcpy(sec_ext->header_sign, signature, RSA_SIGN_BYTE_LEN);
	return rval;
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
int verify_and_copy_file_name_entry(const char *element_name,
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
 *    parse_sec_config_file
 *          Read the secure boot configuration from a file
 *          into internal structures
 *    INPUT:
 *          filename      File name
 *    OUTPUT:
 *          none
 *    RETURN:
 *          0 on success
 ******************************************************************************
 */
int parse_sec_config_file(char *filename)
{
	config_t		sec_cfg;
	int			array_sz, element, rval = -1;
	const char		*cfg_string;
	int32_t			cfg_int32;
	const config_setting_t	*csk_array, *control_array;
	sec_options		*sec_opt = 0;

	config_init(&sec_cfg);

	if (config_read_file(&sec_cfg, filename) != CONFIG_TRUE) {
		fprintf(stderr, "Failed to read data from config file ");
		fprintf(stderr, "%s\n\t%s at line %d\n",
			filename, config_error_text(&sec_cfg),
			config_error_line(&sec_cfg));
		goto exit_parse;
	}

	sec_opt = (sec_options *)calloc(sizeof(sec_options), 1);
	if (sec_opt == 0) {
		fprintf(stderr,
			"Cannot allocate memory for secure boot options!\n");
		goto exit_parse;
	}

	/* KAK file name */
	if (config_lookup_string(&sec_cfg, "kak_key_file",
				 &cfg_string) != CONFIG_TRUE) {
		fprintf(stderr, "The \"kak_key_file\" undefined!\n");
		goto exit_parse;
	}
	if (verify_and_copy_file_name_entry("kak_key_file",
					    cfg_string, sec_opt->kak_key_file))
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
				sec_opt->csk_key_file[element])) {
			fprintf(stderr, "Bad csk_key_file[%d] entry!\n",
				element);
			goto exit_parse;
		}
	}

	/* JTAG options */
	if (config_lookup_bool(&sec_cfg, "jtag.enable",
			       &cfg_int32) != CONFIG_TRUE) {
		fprintf(stderr, "Error obtaining \"jtag.enable\" element. ");
		fprintf(stderr, "Using default - FALSE\n");
		cfg_int32 = 0;
	}
	sec_opt->jtag_enable = cfg_int32;

	if (config_lookup_int(&sec_cfg, "jtag.delay",
			      &cfg_int32) != CONFIG_TRUE) {
		fprintf(stderr, "Error obtaining \"jtag.delay\" element. ");
		fprintf(stderr, "Using default - 0us\n");
		cfg_int32 = 0;
	}
	sec_opt->jtag_delay = cfg_int32;

	/* eFUSE option */
	if (config_lookup_bool(&sec_cfg, "efuse_disable",
			       &cfg_int32) != CONFIG_TRUE) {
		fprintf(stderr, "Error obtaining \"efuse_disable\" element. ");
		fprintf(stderr, "Using default - TRUE\n");
		cfg_int32 = 1;
	}
	sec_opt->efuse_disable = cfg_int32;

	/* Box ID option */
	if (config_lookup_int(&sec_cfg, "box_id", &cfg_int32) != CONFIG_TRUE) {
		fprintf(stderr, "Error obtaining \"box_id\" element. ");
		fprintf(stderr, "Using default - 0x0\n");
		cfg_int32 = 0;
	}
	sec_opt->box_id = cfg_int32;

	/* Flash ID option */
	if (config_lookup_int(&sec_cfg, "flash_id",
			      &cfg_int32) != CONFIG_TRUE) {
		fprintf(stderr, "Error obtaining \"flash_id\" element. ");
		fprintf(stderr, "Using default - 0x0\n");
		cfg_int32 = 0;
	}
	sec_opt->flash_id = cfg_int32;

	/* CSK index option */
	if (config_lookup_int(&sec_cfg, "csk_key_index",
			      &cfg_int32) != CONFIG_TRUE) {
		fprintf(stderr, "Error obtaining \"flash_id\" element. ");
		fprintf(stderr, "Using default - 0x0\n");
		cfg_int32 = 0;
	}
	sec_opt->csk_index = cfg_int32;

	/* Secure boot control array */
	control_array = config_lookup(&sec_cfg, "control");
	if (control_array != NULL) {
		array_sz = config_setting_length(control_array);
		if (array_sz == 0)
			fprintf(stderr, "The \"control\" array is empty!\n");
	} else {
		fprintf(stderr, "The \"control\" is undefined!\n");
		array_sz = 0;
	}

	for (element = 0; element < CP_CTRL_EL_ARRAY_SZ; element++) {
		sec_opt->cp_ctrl_arr[element] =
			config_setting_get_int_elem(control_array, element * 2);
		sec_opt->cp_efuse_arr[element] =
			config_setting_get_int_elem(control_array,
						    element * 2 + 1);
	}

	/* CA certificate file name */
	if (config_lookup_string(&sec_cfg, "ca_cert_file",
				 &cfg_string) != CONFIG_TRUE) {
		fprintf(stderr, "The \"ca_cert_file\" undefined!\n");
		goto exit_parse;
	}
	if (verify_and_copy_file_name_entry("ca_cert_file",
					    cfg_string,
					    server.ca_cert_file))
		goto exit_parse;

	/* Signage Server name */
	if (config_lookup_string(&sec_cfg, "server_name",
				 &cfg_string) != CONFIG_TRUE) {
		fprintf(stderr,
			"The \"server_name\" undefined, using default %s\n",
			DEFAULT_SERVER_NAME);
		memcpy(server.server_name, DEFAULT_SERVER_NAME,
		       strlen(DEFAULT_SERVER_NAME));
	} else if (strlen(cfg_string) > MAX_HOSTNAME) {
		fprintf(stderr, "The \"server_name\" is too long!\n");
		goto exit_parse;
	} else
		memcpy(server.server_name, cfg_string, strlen(cfg_string));

	/* Signage Server port */
	if (config_lookup_int(&sec_cfg, "server_port",
			      &cfg_int32) != CONFIG_TRUE) {
		fprintf(stderr, "The \"server_port\" is undefined,\n");
		fprintf(stderr, "using default port %d\n", DEFAULT_SERVER_PORT);
		cfg_int32 = DEFAULT_SERVER_PORT;
	}
	snprintf(server.server_port, MAX_PORT_LENGTH, "%d", cfg_int32);

	opts.sec_opts = sec_opt;
	rval = 0;

exit_parse:
	config_destroy(&sec_cfg);
	if (sec_opt && (rval != 0))
		free(sec_opt);
	return rval;
} /* end of parse_sec_config_file */

int format_sec_ext(char *filename, FILE *out_fd)
{
	ext_header_t	header;
	sec_entry_t	sec_ext;
	int		written;

	/* First, parse the configuration file */
	if (parse_sec_config_file(filename)) {
		fprintf(stderr,
			"failed parsing configuration file %s\n", filename);
		return 1;
	}

	/* Everything except signatures can be created at this stage */
	header.type = EXT_TYPE_SECURITY;
	header.offset = 0;
	header.size = sizeof(sec_entry_t);
	header.reserved = 0;

	if (process_public_keys(opts.sec_opts, sec_ext.kak_key,
				&sec_ext.csk_keys[0][0],
				MAX_RSA_DER_BYTE_LEN)) {
		fprintf(stderr, "failed to process public RSA keys\n");
		return 1;
	}

	/* The CSK block signature can be created here */
	if (create_rsa_signature(CSK_ARR_SZ, /* KAK */
				 &sec_ext.csk_keys[0][0],
				 sizeof(sec_ext.csk_keys),
				 sec_ext.csk_sign) != 0) {
		fprintf(stderr, "Failed to sign CSK keys block!\n");
		return 1;
	}

	/* Check that everything is correct */
	if (verify_rsa_signature(sec_ext.kak_key,
				 MAX_RSA_DER_BYTE_LEN,
				 &sec_ext.csk_keys[0][0],
				 sizeof(sec_ext.csk_keys),
				 opts.sec_opts->kak_key_file,
				 sec_ext.csk_sign) != 0) {
		fprintf(stderr, "Failed to verify CSK keys block signature!\n");
		return 1;
	}

	/* AES encryption is not supported in this client-server mode */
	sec_ext.encrypt_en = 0;

	/* Fill the rest of the trusted boot extension fields */
	sec_ext.box_id		= opts.sec_opts->box_id;
	sec_ext.flash_id	= opts.sec_opts->flash_id;
	sec_ext.efuse_dis	= opts.sec_opts->efuse_disable;
	sec_ext.jtag_delay	= opts.sec_opts->jtag_delay;
	sec_ext.jtag_en		= opts.sec_opts->jtag_enable;

	memcpy(sec_ext.cp_ctrl_arr,
	       opts.sec_opts->cp_ctrl_arr,
	       sizeof(uint32_t) * CP_CTRL_EL_ARRAY_SZ);
	memcpy(sec_ext.cp_efuse_arr,
	       opts.sec_opts->cp_efuse_arr,
	       sizeof(uint32_t) * CP_CTRL_EL_ARRAY_SZ);

	/* Write the resulting extension to file
	 * (image and header signature fields are still empty)
	 */

	/* Write extension header */
	written = fwrite(&header, sizeof(ext_header_t), 1, out_fd);
	if (written != 1) {
		fprintf(stderr,
			"Failed to write SEC extension header to the file\n");
		return 1;
	}
	/* Write extension body */
	written = fwrite(&sec_ext, sizeof(sec_entry_t), 1, out_fd);
	if (written != 1) {
		fprintf(stderr,
			"Failed to write SEC extension body to the file\n");
		return 1;
	}

	return 0;
}

/*******************************************************************************
 *    finalize_secure_ext
 *          Make final changes to secure extension - calculate image and header
 *          signatures and encrypt the image if needed.
 *          The main header checksum and image size fields updated accordingly
 *    INPUT:
 *          header       Main header
 *          prolog_buf   the entire prolog buffer
 *          prolog_size  prolog buffer length
 *          image_buf    buffer containing the input binary image
 *          image_size   image buffer size.
 *    OUTPUT:
 *          none
 *    RETURN:
 *          0 on success
 ******************************************************************************
 */
int finalize_secure_ext(header_t *header,
			uint8_t *prolog_buf, uint32_t prolog_size,
			uint8_t *image_buf, int image_size)
{
	int		cur_ext, offset;
	uint8_t		*final_image = image_buf;
	uint32_t	final_image_sz = image_size;
	uint8_t		hdr_sign[RSA_SIGN_BYTE_LEN];
	sec_entry_t	*sec_ext = 0;

	/* Find the Trusted Boot Header between available extensions */
	for (cur_ext = 0, offset = sizeof(header_t);
	     cur_ext < header->ext_count; cur_ext++) {
		ext_header_t *ext_hdr = (ext_header_t *)(prolog_buf + offset);

		if (ext_hdr->type == EXT_TYPE_SECURITY) {
			sec_ext = (sec_entry_t *)(prolog_buf + offset +
				   sizeof(ext_header_t) + ext_hdr->offset);
			break;
		}

		offset += sizeof(ext_header_t);
		/* If offset is Zero, the extension follows its header */
		if (ext_hdr->offset == 0)
			offset += ext_hdr->size;
	}

	if (sec_ext == 0) {
		fprintf(stderr, "Error: No Trusted Boot extension found!\n");
		return -1;
	}

	/* Create the image signature first, since it will be later
	 * signed along with the header signature
	 */
	if (create_rsa_signature(opts.sec_opts->csk_index,
				 final_image, final_image_sz,
				 sec_ext->image_sign) != 0) {
		fprintf(stderr, "Failed to sign image!\n");
		return -1;
	}
	/* Check that the image signature is correct */
	if (verify_rsa_signature(sec_ext->csk_keys[opts.sec_opts->csk_index],
				 MAX_RSA_DER_BYTE_LEN,
				 final_image, final_image_sz,
				 opts.sec_opts->csk_key_file[
					 opts.sec_opts->csk_index],
				 sec_ext->image_sign) != 0) {
		fprintf(stderr, "Failed to verify image signature!\n");
		return -1;
	}

	/* Sign the headers and all the extensions block
	 * when the header signature field is empty
	 */
	if (create_rsa_signature(opts.sec_opts->csk_index,
				 prolog_buf, prolog_size,
				 hdr_sign) != 0) {
		fprintf(stderr, "Failed to sign header!\n");
		return -1;
	}
	/* Check that the header signature is correct */
	if (verify_rsa_signature(sec_ext->csk_keys[opts.sec_opts->csk_index],
				 MAX_RSA_DER_BYTE_LEN,
				 prolog_buf, prolog_size,
				 opts.sec_opts->csk_key_file[
					 opts.sec_opts->csk_index],
				 hdr_sign) != 0) {
		fprintf(stderr, "Failed to verify header signature!\n");
		return -1;
	}

	/* Finally, copy the header signature into the trusted boot extension */
	memcpy(sec_ext->header_sign, hdr_sign, RSA_SIGN_BYTE_LEN);

	return 0;
}

#define FMT_HEX		0
#define FMT_DEC		1
#define FMT_BIN		2
#define FMT_NONE	3

void do_print_field(unsigned int value, char *name,
		    int start, int size, int format)
{
	fprintf(stdout, "[0x%05x : 0x%05x]  %-26s",
		start, start + size - 1, name);

	switch (format) {
	case FMT_HEX:
		printf("0x%x\n", value);
		break;
	case FMT_DEC:
		printf("%d\n", value);
		break;
	default:
		printf("\n");
		break;
	}
}

#define print_field(st, type, field, hex, base) \
			do_print_field((int)st->field, #field, \
			base + offsetof(type, field), sizeof(st->field), hex)

int print_header(uint8_t *buf, int base)
{
	header_t *main_hdr;

	main_hdr = (header_t *)buf;

	fprintf(stdout, "########### Header ##############\n");
	print_field(main_hdr, header_t, magic, FMT_HEX, base);
	print_field(main_hdr, header_t, prolog_size, FMT_DEC, base);
	print_field(main_hdr, header_t, prolog_checksum, FMT_HEX, base);
	print_field(main_hdr, header_t, boot_image_size, FMT_DEC, base);
	print_field(main_hdr, header_t, boot_image_checksum, FMT_HEX, base);
	print_field(main_hdr, header_t, rsrvd0, FMT_HEX, base);
	print_field(main_hdr, header_t, load_addr, FMT_HEX, base);
	print_field(main_hdr, header_t, exec_addr, FMT_HEX, base);
	print_field(main_hdr, header_t, uart_cfg, FMT_HEX, base);
	print_field(main_hdr, header_t, baudrate, FMT_HEX, base);
	print_field(main_hdr, header_t, ext_count, FMT_DEC, base);
	print_field(main_hdr, header_t, aux_flags, FMT_HEX, base);
	print_field(main_hdr, header_t, io_arg_0, FMT_HEX, base);
	print_field(main_hdr, header_t, io_arg_1, FMT_HEX, base);
	print_field(main_hdr, header_t, io_arg_2, FMT_HEX, base);
	print_field(main_hdr, header_t, io_arg_3, FMT_HEX, base);
	print_field(main_hdr, header_t, rsrvd1, FMT_HEX, base);
	print_field(main_hdr, header_t, rsrvd2, FMT_HEX, base);
	print_field(main_hdr, header_t, rsrvd3, FMT_HEX, base);

	return sizeof(header_t);
}

int print_ext_hdr(ext_header_t *ext_hdr, int base)
{
	print_field(ext_hdr, ext_header_t, type, FMT_HEX, base);
	print_field(ext_hdr, ext_header_t, offset, FMT_HEX, base);
	print_field(ext_hdr, ext_header_t, reserved, FMT_HEX, base);
	print_field(ext_hdr, ext_header_t, size, FMT_DEC, base);

	return base + sizeof(ext_header_t);
}

void print_sec_ext(ext_header_t *ext_hdr, int base)
{
	sec_entry_t	*sec_entry;
	uint32_t	new_base;

	fprintf(stdout, "\n########### Secure extension ###########\n");

	new_base = print_ext_hdr(ext_hdr, base);

	sec_entry = (sec_entry_t *)(ext_hdr + 1);

	do_print_field(0, "KAK key", new_base, MAX_RSA_DER_BYTE_LEN, FMT_NONE);
	new_base += MAX_RSA_DER_BYTE_LEN;
	print_field(sec_entry, sec_entry_t, jtag_delay, FMT_DEC, base);
	print_field(sec_entry, sec_entry_t, box_id, FMT_HEX, base);
	print_field(sec_entry, sec_entry_t, flash_id, FMT_HEX, base);
	print_field(sec_entry, sec_entry_t, encrypt_en, FMT_DEC, base);
	print_field(sec_entry, sec_entry_t, efuse_dis, FMT_DEC, base);
	new_base += 6 * sizeof(uint32_t);
	do_print_field(0, "header signature",
		       new_base, RSA_SIGN_BYTE_LEN, FMT_NONE);
	new_base += RSA_SIGN_BYTE_LEN;
	do_print_field(0, "image signature",
		       new_base, RSA_SIGN_BYTE_LEN, FMT_NONE);
	new_base += RSA_SIGN_BYTE_LEN;
	do_print_field(0, "CSK keys", new_base,
		       CSK_ARR_SZ * MAX_RSA_DER_BYTE_LEN, FMT_NONE);
	new_base += CSK_ARR_SZ * MAX_RSA_DER_BYTE_LEN;
	do_print_field(0, "CSK block signature",
		       new_base, RSA_SIGN_BYTE_LEN, FMT_NONE);
	new_base += RSA_SIGN_BYTE_LEN;
	do_print_field(0, "control", new_base,
		       CP_CTRL_EL_ARRAY_SZ * 2, FMT_NONE);

}

void print_bin_ext(ext_header_t *ext_hdr, int base)
{
	fprintf(stdout, "\n########### Binary extension ###########\n");
	base = print_ext_hdr(ext_hdr, base);
	do_print_field(0, "binary image", base, ext_hdr->size, FMT_NONE);
}

int print_extension(void *buf, int base, int count, int ext_size)
{
	ext_header_t *ext_hdr = buf;
	int pad = ext_size;
	int curr_size;

	while (count--) {
		if (ext_hdr->type == EXT_TYPE_BINARY)
			print_bin_ext(ext_hdr, base);
		else if (ext_hdr->type == EXT_TYPE_SECURITY)
			print_sec_ext(ext_hdr, base);

		curr_size = sizeof(ext_header_t) + ext_hdr->size;
		base += curr_size;
		pad  -= curr_size;
		ext_hdr = (ext_header_t *)((uintptr_t)ext_hdr + curr_size);
	}

	if (pad)
		do_print_field(0, "padding", base, pad, FMT_NONE);

	return ext_size;
}

int parse_image(uint8_t *buf, int size)
{
	int base = 0;
	int ret = 1;
	header_t *main_hdr;
	uint32_t checksum, prolog_checksum;


	fprintf(stdout,
		"################### Prolog Start ######################\n\n");
	main_hdr = (header_t *)buf;
	base += print_header(buf, base);

	if (main_hdr->ext_count)
		base += print_extension(buf + base, base,
					main_hdr->ext_count,
					main_hdr->prolog_size -
					sizeof(header_t));

	if (base < main_hdr->prolog_size) {
		fprintf(stdout, "\n########### Padding ##############\n");
		do_print_field(0, "prolog padding",
			       base, main_hdr->prolog_size - base, FMT_HEX);
		base = main_hdr->prolog_size;
	}
	fprintf(stdout,
		"\n################### Prolog End ######################\n");

	fprintf(stdout,
		"\n################### Boot image ######################\n");

	do_print_field(0, "boot image", base, size - base - 4, FMT_NONE);

	fprintf(stdout,
		"################### Image end ########################\n");

	/* Check sanity for certain values */
	printf("\nChecking values:\n");

	if (main_hdr->magic == MAIN_HDR_MAGIC) {
		fprintf(stdout, "Headers magic:    OK!\n");
	} else {
		fprintf(stderr,
			"\n****** ERROR: HEADER MAGIC 0x%08x != 0x%08x\n",
			main_hdr->magic, MAIN_HDR_MAGIC);
		goto error;
	}

	/* headers checksum */
	/* clear the checksum field in header to calculate checksum */
	prolog_checksum = main_hdr->prolog_checksum;
	main_hdr->prolog_checksum = 0;
	checksum = checksum32((uint32_t *)buf, main_hdr->prolog_size);

	if (checksum == prolog_checksum) {
		fprintf(stdout, "Headers checksum: OK!\n");
	} else {
		fprintf(stderr,
			"\n***** ERROR: BAD HEADER CHECKSUM 0x%08x != 0x%08x\n",
			checksum, prolog_checksum);
		goto error;
	}

	/* boot image checksum */
	checksum = checksum32((uint32_t *)(buf + main_hdr->prolog_size),
			      main_hdr->boot_image_size);
	if (checksum == main_hdr->boot_image_checksum) {
		fprintf(stdout, "Image checksum:   OK!\n");
	} else {
		fprintf(stderr,
			"\n****** ERROR: BAD IMAGE CHECKSUM 0x%08x != 0x%08x\n",
			checksum, main_hdr->boot_image_checksum);
		goto error;
	}

	/* RSA signatures */
	if (main_hdr->ext_count) {
		uint8_t		ext_num = main_hdr->ext_count;
		ext_header_t	*ext_hdr = (ext_header_t *)(main_hdr + 1);
		unsigned char	hash[32];
		int		i;

		while (ext_num--) {
			if (ext_hdr->type == EXT_TYPE_SECURITY) {
				sec_entry_t  *sec_entry =
						(sec_entry_t *)(ext_hdr + 1);

				ret = verify_secure_header_signatures(
							main_hdr, sec_entry);
				if (ret != 0) {
					fprintf(stderr,
						"\n****** FAILED TO VERIFY ");
					fprintf(stderr,
						"RSA SIGNATURES ********\n");
					goto error;
				}

				client_calc_sha256(sec_entry->kak_key,
						   MAX_RSA_DER_BYTE_LEN, hash);
				fprintf(stdout,
					">>>>>>>>>> KAK KEY HASH >>>>>>>>>>\n");
				fprintf(stdout, "SHA256: ");
				for (i = 0; i < 32; i++)
					fprintf(stdout, "%02X", hash[i]);

				fprintf(stdout,
					"\n<<<<<<<<< KAK KEY HASH <<<<<<<<<\n");

				break;
			}
			ext_hdr =
				(ext_header_t *)((uint8_t *)(ext_hdr + 1) +
				 ext_hdr->size);
		}
	}

	ret = 0;
error:
	return ret;
}

int format_bin_ext(char *filename, FILE *out_fd)
{
	ext_header_t header;
	FILE *in_fd;
	int size, written;
	int aligned_size, pad_bytes;
	char c;

	in_fd = fopen(filename, "rb");
	if (in_fd == NULL) {
		fprintf(stderr, "failed to open bin extension file %s\n",
			filename);
		return 1;
	}

	size = get_file_size(filename);
	if (size <= 0) {
		fprintf(stderr, "bin extension file size is bad\n");
		return 1;
	}

	/* Align extension size to 8 bytes */
	aligned_size = (size + 7) & (~7);
	pad_bytes    = aligned_size - size;

	header.type = EXT_TYPE_BINARY;
	header.offset = 0;
	header.size = aligned_size;
	header.reserved = 0;

	/* Write header */
	written = fwrite(&header, sizeof(ext_header_t), 1, out_fd);
	if (written != 1) {
		fprintf(stderr, "failed writing header to extension file\n");
		return 1;
	}

	/* Write image */
	while (size--) {
		c = getc(in_fd);
		fputc(c, out_fd);
	}

	while (pad_bytes--)
		fputc(0, out_fd);

	fclose(in_fd);

	return 0;
}

/* ****************************************
 *
 * Write all extensions (binary, secure
 * extensions) to file
 *
 * ****************************************/

int format_extensions(char *ext_filename)
{
	FILE *out_fd;
	int ret = 0;

	out_fd = fopen(ext_filename, "wb");
	if (out_fd == NULL) {
		fprintf(stderr, "failed to open extension output file %s",
			ext_filename);
		return 1;
	}

	if (strncmp(opts.bin_ext_file, "NA", MAX_FILENAME)) {
		if (format_bin_ext(opts.bin_ext_file, out_fd)) {
			ret = 1;
			goto error;
		}
	}
	if (strncmp(opts.sec_cfg_file, "NA", MAX_FILENAME)) {
		if (format_sec_ext(opts.sec_cfg_file, out_fd)) {
			ret = 1;
			goto error;
		}
	}

error:
	fflush(out_fd);
	fclose(out_fd);
	return ret;
}

void update_uart(header_t *header)
{
	header->uart_cfg = 0;
	header->baudrate = 0;

	if (opts.disable_print)
		uart_set_mode(header->uart_cfg, UART_MODE_DISABLE);

	if (opts.baudrate)
		header->baudrate = (opts.baudrate / 1200);
}

/* ****************************************
 *
 * Write the image prolog, i.e.
 * main header and extensions, to file
 *
 * ****************************************/

int write_prolog(int ext_cnt, char *ext_filename,
		 uint8_t *image_buf, int image_size, FILE *out_fd)
{
	header_t		*header;
	int main_hdr_size = sizeof(header_t);
	int prolog_size = main_hdr_size;
	FILE *ext_fd;
	char *buf;
	int written, read;
	int ret = 1;


	if (ext_cnt)
		prolog_size +=  get_file_size(ext_filename);

	prolog_size = ((prolog_size + PROLOG_ALIGNMENT) &
		     (~(PROLOG_ALIGNMENT-1)));

	/* Allocate a zeroed buffer to zero the padding bytes */
	buf = calloc(prolog_size, 1);
	if (buf == NULL) {
		fprintf(stderr, "Error: failed allocating checksum buffer\n");
		return 1;
	}

	header = (header_t *)buf;
	header->magic       = MAIN_HDR_MAGIC;
	header->prolog_size = prolog_size;
	header->load_addr   = opts.load_addr;
	header->exec_addr   = opts.exec_addr;
	header->io_arg_0    = opts.nfc_io_args;
	header->ext_count   = ext_cnt;
	header->aux_flags   = 0;
	header->boot_image_size = (image_size + 3) & (~0x3);
	header->boot_image_checksum = checksum32((uint32_t *)image_buf,
						 image_size);

	update_uart(header);

	/* Populate buffer with main header and extensions */
	if (ext_cnt) {
		ext_fd = fopen(ext_filename, "rb");
		if (ext_fd == NULL) {
			fprintf(stderr,
				"Error: failed to open extensions file\n");
			goto error;
		}

		read = fread(&buf[main_hdr_size],
			     get_file_size(ext_filename), 1, ext_fd);
		if (read != 1) {
			fprintf(stderr,
				"Error: failed to open extensions file\n");
			goto error;
		}

		/* Secure boot mode? */
		if (opts.sec_opts != 0) {
			ret = finalize_secure_ext(header, (uint8_t *)buf,
						  prolog_size, image_buf,
						  image_size);
			if (ret != 0) {
				fprintf(stderr, "Error: failed to handle ");
				fprintf(stderr, "secure extension!\n");
				goto error;
			}
		} /* secure boot mode */
	}

	/* Update the total prolog checksum */
	header->prolog_checksum = checksum32((uint32_t *)buf, prolog_size);

	/* Now spill everything to output file */
	written = fwrite(buf, prolog_size, 1, out_fd);
	if (written != 1) {
		fprintf(stderr,
			"Error: failed to write prolog to output file\n");
		goto error;
	}

	ret = 0;

error:
	free(buf);
	return ret;
}

int write_boot_image(uint8_t *buf, uint32_t image_size, FILE *out_fd)
{
	int written;

	written = fwrite(buf, image_size, 1, out_fd);
	if (written != 1) {
		fprintf(stderr, "Error: Failed to write boot image\n");
		goto error;
	}

	return 0;
error:
	return 1;
}


int main(int argc, char *argv[])
{
	char in_file[MAX_FILENAME+1];
	char out_file[MAX_FILENAME+1];
	char ext_file[MAX_FILENAME+1];
	FILE *in_fd = NULL;
	FILE *out_fd = NULL;
	int parse = 0;
	int ext_cnt = 0;
	int opt;
	int ret = 0;
	int image_size, file_size;
	uint8_t *image_buf = NULL;
	int read;
	uint32_t nand_block_size_kb, mlc_nand;

	/* Create temporary file for building extensions
	 * Use process ID for allowing multiple parallel runs
	 */
	snprintf(ext_file, MAX_FILENAME, "/tmp/ext_file-%x", getpid());

	while ((opt = getopt(argc, argv, "hpms:i:l:e:a:b:u:n:t:c:k:")) != -1) {
		switch (opt) {
		case 'h':
			usage();
			break;
		case 'l':
			opts.load_addr = strtoul(optarg, NULL, 0);
			break;
		case 'e':
			opts.exec_addr = strtoul(optarg, NULL, 0);
			break;
		case 'm':
			opts.disable_print = 1;
			break;
		case 'u':
			opts.baudrate = strtoul(optarg, NULL, 0);
			break;
		case 'b':
			strncpy(opts.bin_ext_file, optarg, MAX_FILENAME);
			ext_cnt++;
			break;
		case 'p':
			parse = 1;
			break;
		case 'n':
			nand_block_size_kb = strtoul(optarg, NULL, 0);
			opts.nfc_io_args |= (nand_block_size_kb / 64);
			break;
		case 't':
			mlc_nand = 0;
			if (!strncmp("MLC", optarg, 3))
				mlc_nand = 1;
			opts.nfc_io_args |= (mlc_nand << 8);
			break;
		case 'c': /* SEC extension */
			strncpy(opts.sec_cfg_file, optarg, MAX_FILENAME);
			ext_cnt++;
			break;
		case 'k':
			opts.key_index = strtoul(optarg, NULL, 0);
			break;
		default: /* '?' */
			usage_err("Unknown argument");
			exit(EXIT_FAILURE);
		}
	}

	/* Check validity of inputs */
	if (opts.load_addr % 8)
		usage_err("Load address must be 8 bytes aligned");

	if (opts.baudrate % 1200)
		usage_err("Baudrate must be a multiple of 1200");

	/* The remaining arguments are the input
	 * and potentially output file
	 */
	/* Input file must exist so exit if not */
	if (optind >= argc)
		usage_err("missing input file name");

	strncpy(in_file, argv[optind], MAX_FILENAME);
	optind++;

	/* Output file must exist in non parse mode */
	if (optind < argc)
		strncpy(out_file, argv[optind], MAX_FILENAME);
	else if (!parse)
		usage_err("missing output file name");

	/* open the input file */
	in_fd = fopen(in_file, "rb");
	if (in_fd == NULL) {
		printf("Error: Failed to open input file %s\n", in_file);
		goto main_exit;
	}

	/* Read the input file to buffer
	 * Always align the image to 16 byte boundary
	 */
	file_size  = get_file_size(in_file);
	image_size = (file_size + AES_BLOCK_SZ - 1) & ~(AES_BLOCK_SZ - 1);
	image_buf  = calloc(image_size, 1);
	if (image_buf == NULL) {
		fprintf(stderr, "Error: failed allocating input buffer\n");
		return 1;
	}

	read = fread(image_buf, file_size, 1, in_fd);
	if (read != 1) {
		fprintf(stderr, "Error: failed to read input file\n");
		goto main_exit;
	}

	/* Parse the input image and leave */
	if (parse) {
		if (opts.key_index >= CSK_ARR_SZ) {
			fprintf(stderr,
				"Wrong key IDX value. Valid values 0 - %d\n",
				CSK_ARR_SZ - 1);
			goto main_exit;
		}
		ret = parse_image(image_buf, image_size);
		goto main_exit;
	}

	/* Create a blob file from all extensions */
	if (ext_cnt) {
		ret = format_extensions(ext_file);
		if (ret)
			goto main_exit;
	}

	out_fd = fopen(out_file, "wb");
	if (out_fd == NULL) {
		fprintf(stderr,
			"Error: Failed to open output file %s\n", out_file);
		goto main_exit;
	}

	ret = write_prolog(ext_cnt, ext_file, image_buf, image_size, out_fd);
	if (ret)
		goto main_exit;

	ret = write_boot_image(image_buf, image_size, out_fd);

main_exit:
	if (in_fd)
		fclose(in_fd);

	if (out_fd)
		fclose(out_fd);

	if (image_buf)
		free(image_buf);

	unlink(ext_file);

	if (opts.sec_opts)
		free(opts.sec_opts);

	exit(ret);
}
