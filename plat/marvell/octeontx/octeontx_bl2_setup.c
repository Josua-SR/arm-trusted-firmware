/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2016-2019, Marvell International Ltd. All rights reserved.<BR>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <arch_helpers.h>
#include <assert.h>
#include <bl_common.h>
#include <desc_image_load.h>
#include <console.h>
#include <pl011.h>
#include <platform.h>
#include <platform_def.h>
#include <string.h>
#include <xlat_tables_v2.h>
#include <octeontx_common.h>
#include <octeontx_board_cfg.h>
#include <stdio.h>
#include <debug.h>
#include <libfdt.h>
#include <octeontx_security.h>
#include <octeontx_ecam.h>
#include <octeontx_io_storage.h>
#include <timers_octeontx.h>

#include <octeontx_board_cfg_setup.h>
#include <octeontx_scfg_setup.h>
#include <plat_octeontx.h>
#include <octeontx_helpers.h>

#if ENABLE_ATTESTATION_SERVICE
#include <octeontx_attestation.h>
#include <auth/auth_mod.h>
#include <plat_octeontx.h>
#include <tbbr_oid.h>
#include <octeontx_utils.h>
#include <octeontx_io_storage.h>
/* these are required for 'decode_hash_digest()' */
#include <mbedtls/asn1.h>
#include <mbedtls/md.h>
#include <mbedtls/oid.h>
#endif

/* Pointer to memory visible to both BL2 and BL31 for passing data */
extern unsigned char **bl2_el_change_mem_ptr;

/* Data structure which holds the extents of the trusted SRAM for BL2 */
static meminfo_t bl2_tzram_layout __aligned(CACHE_WRITEBACK_GRANULE)
		__attribute((section("tzfw_coherent_mem")));

/* Data structure for console initialization */
static console_pl011_t console;

#if ENABLE_ATTESTATION_SERVICE
/*
 * This holds the BL2 platform data (which includes s/w attestation info).
 * The contents are adjusted to reflect images which have been loaded by BL2.
 * Finally, upon exit, the contents of this structure are passed to BL31.
 *
 * See also BL31 variable 'octeontx_bl31_plat_args'.
 */
static octeontx_bl_platform_args_t octeontx_bl2_plat_args;

/*
 * This is used to retrieve the hash value from a [DER] encoded hash digest.
 *
 * returns,
 *   0 if success (hash & len returned via 'hash' and 'hash_len')
 *  -1 if error
 */
static
int decode_hash_digest(void *digest_info_ptr, unsigned int digest_info_len,
		       unsigned char **hash, size_t *hash_len)
{
	mbedtls_asn1_buf hash_oid, params;
	mbedtls_md_type_t md_alg;
	const mbedtls_md_info_t *md_info;
	unsigned char *p, *end;
	size_t len;
	int rc;

	/* Digest info should be an MBEDTLS_ASN1_SEQUENCE */
	p = (unsigned char *)digest_info_ptr;
	end = p + digest_info_len;
	rc = mbedtls_asn1_get_tag(&p, end, &len, MBEDTLS_ASN1_CONSTRUCTED |
				  MBEDTLS_ASN1_SEQUENCE);
	if (rc != 0)
		return -1;

	/* Get the hash algorithm */
	rc = mbedtls_asn1_get_alg(&p, end, &hash_oid, &params);
	if (rc != 0)
		return -1;

	rc = mbedtls_oid_get_md_alg(&hash_oid, &md_alg);
	if (rc != 0)
		return -1;

	md_info = mbedtls_md_info_from_type(md_alg);
	if (md_info == NULL)
		return -1;

	/* Hash should be octet string type */
	rc = mbedtls_asn1_get_tag(&p, end, &len, MBEDTLS_ASN1_OCTET_STRING);
	if (rc != 0)
		return -1;

	/* Length of hash must match the algorithm's size */
	if (len != mbedtls_md_get_size(md_info))
		return -1;

	*hash = p;
	*hash_len = len;

	return 0;
}

/*
 * This populates the platform argument pointer that is passed to BL31
 * as 'arg1' - see also 'bl31_early_platform_setup2()'.
 */
static void populate_platform_args_for_bl31(void)
{
#define MAX_COMP_ID_STR_LEN   256
/* 32-byte hash == 64 ASCII characters */
#define SHA256_ASCII_HASH_LEN 64
/* BDK has 64 char limit (+ ".dtb") */
#define MAX_BOARD_NAME_LEN    (64 + 4)
/* BDK has 64 char limit (+ "-linux.dtb") */
#define MAX_BOARD_NAME_LINUX_LEN (MAX_BOARD_NAME_LEN + 10)
	const auth_img_desc_t *img_desc_ptr;
	const auth_param_desc_t *auth_param;
	int img_id, fdt_off, prop_len;
	uint8_t *dst, *hash, digit, byteval;
	char comp_str_buf[MAX_COMP_ID_STR_LEN], *comp_str, *separator;
	char board_dt[MAX_BOARD_NAME_LEN];
	char board_linux_dt[MAX_BOARD_NAME_LINUX_LEN];
	const void *fdt;
	const void *prop, *end;
	size_t idx, hash_len;

	octeontx_bl2_plat_args.fdt = fdt_ptr;

	/* copy BL31 saved authentication signature for s/w attestation info */
	img_id = BL31_IMAGE_ID;
	if (!(auth_img_flags[img_id] & IMG_FLAG_AUTHENTICATED)) {
		ERROR("Image ID %u is not authenticated\n", img_id);
	} else {
		img_desc_ptr = &cot_desc_ptr[img_id];
		assert(img_desc_ptr->parent);

		/* parent image has authentication signature stored within */
		img_desc_ptr = &cot_desc_ptr[img_desc_ptr->parent->img_id];

		auth_param = &img_desc_ptr->authenticated_data[0];
		assert(!strcmp(auth_param->type_desc->cookie,
			       SOC_AP_FW_HASH_OID));

		if ((decode_hash_digest(auth_param->data.ptr,
					auth_param->data.len,
					&hash, &hash_len) == 0) &&
		    (hash_len == sizeof(octeontx_bl2_plat_args.atf_bl31_sig))) {
			memcpy(octeontx_bl2_plat_args.atf_bl31_sig, hash,
			       hash_len);
		} else
			ERROR("Error saving attestation info for image ID %u\n",
			      img_id);
	}

	/* copy BL33 saved authentication signature for s/w attestation info */
	img_id = BL33_IMAGE_ID;
	if (!(auth_img_flags[img_id] & IMG_FLAG_AUTHENTICATED)) {
		ERROR("Image ID %u is not authenticated\n", img_id);
	} else {
		img_desc_ptr = &cot_desc_ptr[img_id];
		assert(img_desc_ptr->parent);

		/* parent image has authentication signature stored within */
		img_desc_ptr = &cot_desc_ptr[img_desc_ptr->parent->img_id];

		auth_param = &img_desc_ptr->authenticated_data[0];
		assert(!strcmp(auth_param->type_desc->cookie,
			       NON_TRUSTED_WORLD_BOOTLOADER_HASH_OID));

		if ((decode_hash_digest(auth_param->data.ptr,
					auth_param->data.len,
					&hash, &hash_len) == 0) &&
		    (hash_len == sizeof(octeontx_bl2_plat_args.atf_bl33_sig))) {
			memcpy(octeontx_bl2_plat_args.atf_bl33_sig, hash,
			       hash_len);
		} else
			ERROR("Error saving attestation info for image ID %u\n",
			      img_id);
	}

	/* Some of the required version info is passed in the FDT (from BDK) */
	fdt = fdt_ptr;

	/* locate board name property & save board name in local buffer */
	board_dt[0] = board_linux_dt[0] = 0;
	prop = NULL;
	if (fdt_check_header(fdt) == 0) {
		fdt_off = fdt_path_offset(fdt, "/cavium,bdk");
		if (fdt_off > 0)
			prop = fdt_getprop(fdt, fdt_off, "BOARD-MODEL",
					   &prop_len);
		if (prop) {
			snprintf(board_dt, sizeof(board_dt), "%s.dtb",
				 (char *)prop);
			snprintf(board_linux_dt, sizeof(board_linux_dt),
				 "%s-linux.dtb", (char *)prop);
		}
	}

	/* locate property containing list of images and their IDs */
	prop = NULL;
	if (fdt_check_header(fdt) == 0) {
		fdt_off = fdt_path_offset(fdt, "/cavium,bdk");
		if (fdt_off > 0)
			prop = fdt_getprop(fdt, fdt_off,
					   "ATTESTATION-IMAGE-LIST",
					   &prop_len);
	}

	if (prop == NULL) {
		ERROR("Error locating image list property\n");
		return;
	}

	end = prop + prop_len;
	/* this is a list of strings; search for individual component strings */
	while (prop < end) {

		/* each string has the following format:
		 *    <name>:<ascii_hash_id>
		 * where:
		 *    <name> = component path name (see BDK code)
		 *    <ascii_hash_id> = ASCII SHA-256 HASH (64 chars)
		 */
		comp_str = comp_str_buf;
		comp_str[MAX_COMP_ID_STR_LEN - 1] = 0;
		strncpy(comp_str, (char *)prop, MAX_COMP_ID_STR_LEN - 1);
		separator = strchr(comp_str, ':');
		if (separator == NULL) {
			ERROR("Invalid image list property\n");
			break;
		}
		*separator = 0;

		/* exclude any path from string name search */
		while (strchr(comp_str, '/') != NULL)
			comp_str = strchr(comp_str, '/') + 1;

		dst = NULL;
		if (strncmp(comp_str, "BOOT.BIN", 8) == 0)
			dst = octeontx_bl2_plat_args.ap_tbl1fw_sig;
		else if (strncmp(comp_str, "init.bin", 8) == 0)
			dst = octeontx_bl2_plat_args.init_bin_sig;
		else if (strncmp(comp_str, "bl2.bin", 7) == 0)
			dst = octeontx_bl2_plat_args.atf_bl2_sig;
		else if (strncmp(comp_str, board_dt, strlen(board_dt)) == 0)
			dst = octeontx_bl2_plat_args.board_dt_sig;
		else if (strncmp(comp_str, board_linux_dt,
				 strlen(board_linux_dt)) == 0)
			dst = octeontx_bl2_plat_args.linux_dt_sig;
		else {
			/* point to next string in list */
			prop += strlen((char *)prop) + 1;
			continue;
		}

		/* set comp_str to point to ASCII hash id (see format above) */
		comp_str = separator + 1;
		if (strlen(comp_str) != SHA256_ASCII_HASH_LEN) {
			ERROR("Invalid hash ID for %s\n", (char *)prop);
			break;
		}

		/* convert ASCII hash ID to binary and store in 'dst' */
		for (idx = 0; (idx < SHA256_ASCII_HASH_LEN); idx += 2) {
			/* force lowercase hex; doesn't affect decimal digits */
			*comp_str |= 'a' - 'A';

			/* convert ASCII decimal digit to binary */
			digit = *comp_str++ - '0';
			/* account for 'a'-'f' hex digits */
			if (digit > 9)
				digit -= ('a' - '9' - 1);
			byteval = digit << 4; /* i.e. digit * 0x10 */

			/* force lowercase hex; doesn't affect decimal digits */
			*comp_str |= 'a' - 'A';

			/* convert ASCII decimal digit to binary */
			digit = *comp_str++ - '0';
			/* account for 'a'-'f' hex digits */
			if (digit > 9)
				digit -= ('a' - '9' - 1);
			byteval |= digit;

			*dst++ = byteval;
		}

		/* point to next string in list */
		prop += strlen((char *)prop) + 1;
	}
}
#endif

int bl2_plat_handle_post_image_load(unsigned int image_id)
{
	int err = 0;
	unsigned long el_status;
	unsigned int mode;
	uintptr_t bl33_fdt_address;
	bl_mem_params_node_t *bl_mem_params = get_bl_mem_params_node(image_id);
#if ENABLE_ATTESTATION_SERVICE
	bl_load_info_t *bl2_load_info;
	const bl_load_info_node_t *bl2_node_info;
#endif
#ifdef NT_FW_CONFIG
	uint64_t nt_fw_config_size;
#endif
	assert(bl_mem_params);

	switch (image_id) {
	case BL31_IMAGE_ID:
#if ENABLE_ATTESTATION_SERVICE
		/* set argument pointer but don't populate yet - see below */
		bl_mem_params->ep_info.args.arg1 =
				(u_register_t)&octeontx_bl2_plat_args;
#else
		bl_mem_params->ep_info.args.arg1 = (unsigned long)fdt_ptr;
#endif
		break;
#ifdef AARCH64
	case BL32_IMAGE_ID:
		bl_mem_params->ep_info.spsr = 0;
		break;
#endif

	case BL33_IMAGE_ID:
		/* Figure out what mode we enter the non-secure world in */
		el_status = read_id_aa64pfr0_el1() >> ID_AA64PFR0_EL2_SHIFT;
		el_status &= ID_AA64PFR0_ELX_MASK;

		if (el_status)
			mode = MODE_EL2;
		else
			mode = MODE_EL1;
		/* BL33 expects to receive the primary CPU MPID (through r0) */
		bl_mem_params->ep_info.args.arg0 = 0xffff & read_mpidr();
		bl_mem_params->ep_info.spsr = SPSR_64(mode, MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS);

		bl33_fdt_address = bl_mem_params->image_info.image_base + bl_mem_params->image_info.image_size;
		VERBOSE("BL33: Image base: %lx, Image size: %d\n",
			bl_mem_params->image_info.image_base, bl_mem_params->image_info.image_size);
		VERBOSE("BL33: FDT address: %lx, FDT size: %d\n",
			bl33_fdt_address, fdt_totalsize(fdt_ptr));

		memcpy((void *)bl33_fdt_address, fdt_ptr, fdt_totalsize(fdt_ptr));

		/*
		 * Flush data to PoC ,because when booting
		 * to BL33 caches will be turned off
		 */
		flush_dcache_range(bl33_fdt_address, fdt_totalsize(bl33_fdt_address));

		/* Pass FDT address and size to BL33 */
		bl_mem_params->ep_info.args.arg1 = bl33_fdt_address;
		bl_mem_params->ep_info.args.arg2 = fdt_totalsize(fdt_ptr);
#ifdef AARCH64
		bl_mem_params->ep_info.args.arg4 = mode;
#endif

		break;

#ifdef SCP_BL2_BASE
	case SCP_BL2_IMAGE_ID:
		/* The subsequent handling of SCP_BL2 is platform specific */
		err = plat_arm_bl2_handle_scp_bl2(&bl_mem_params->image_info);
		if (err) {
			WARN("Failure in platform-specific handling of SCP_BL2 image.\n");
		}
		break;
#endif

#ifdef NT_FW_CONFIG
	/* If non-trusted firmware config is present, pass it's size at BL31 level */
	case NT_FW_CONFIG_ID:
		nt_fw_config_size = bl_mem_params->image_info.image_size;
		bl_mem_params = get_bl_mem_params_node(BL31_IMAGE_ID);
		bl_mem_params->ep_info.args.arg2 = nt_fw_config_size;
		break;
#endif
	}

#if ENABLE_ATTESTATION_SERVICE
	/*
	 * The s/w attestation information can only be populated after
	 * the last image has been loaded (and authenticated).  Here, check
	 * if this is the last image and, if so, populate the platform
	 * arguments (s/w attestation info).
	 */
	bl2_load_info = plat_get_bl_image_load_info();
	bl2_node_info = bl2_load_info->head;
	while (bl2_node_info) {
		if (image_id == bl2_node_info->image_id) {
			/*
			 * If this image is the last to be loaded, populate the
			 * s/w attestion info in the platform arguments.
			 */
			if (bl2_node_info->next_load_info == NULL)
				populate_platform_args_for_bl31();
			break;
		}
		bl2_node_info = bl2_node_info->next_load_info;
	}
#endif

	return err;
}

static void bl2_platform_print_chip_id(void)
{
	const void *fdt = fdt_ptr;
	const char *uid;
	char uid_prop[21];
	int offset, len;

	offset = fdt_path_offset(fdt, "/cavium,bdk");
	if (offset < 0) {
		INFO("WARNING: FDT node not found\n");
		return;
	}
	snprintf(uid_prop, sizeof(uid_prop), "CHIP-UNIQUE-ID.NODE0");
	uid = fdt_getprop(fdt, offset, uid_prop, &len);
	if (uid) {
		if (!strncmp(uid, "00000000000000000000", 20))
			NOTICE("CHIP UniqueID not set\n");
		else
			NOTICE("CHIP UniqueID = %s\n", uid);

	} else {
		INFO("WARNING: No CHIP-Unique-ID is found\n");
		NOTICE("CHIP UniqueID not set\n");
	}
}

void bl2_el3_early_platform_setup(u_register_t arg0, u_register_t arg1,
				u_register_t arg2, u_register_t arg3)
{
	plat_octeontx_cpu_setup();

	/* Do it here. Later this region will be mapped as RO. */
	fdt_pack(fdt_ptr);

	/* Initialize the console to provide early debug support */
	console_pl011_register(UAAX_PF_BAR0(0), 0, 0, &console);
	console_set_scope((console_t *)&console, CONSOLE_FLAG_RUNTIME);
	console_switch_state(CONSOLE_FLAG_RUNTIME);

	/* Allow BL1 to see the whole Trusted RAM */
	bl2_tzram_layout.total_base = TZDRAM_BASE;
	bl2_tzram_layout.total_size = TZDRAM_SIZE;
}

/*******************************************************************************
 * Perform platform specific setup. For now just initialize the memory location
 * to use for passing arguments to BL31.
 ******************************************************************************/
void bl2_platform_setup(void)
{
	bl2_platform_print_chip_id();
	octeontx_fill_soc_details();
	octeontx_fill_board_details(1);

	timers_octeontx_init_delay();
	/*
	 * Do initial security configuration to allow DRAM/device access.
	 */
	octeontx_security_setup();

	/* Enumerate devices on ECAMs */
	octeontx_pci_init();

	/* Initialise the IO layer and register platform IO devices */
	octeontx_io_setup();
}

/*******************************************************************************
 * Perform the very early platform specific architectural setup here. At the
 * moment this is only intializes the mmu in a quick and dirty way.
 ******************************************************************************/
void bl2_el3_plat_arch_setup(void)
{
	mmap_add_region(bl2_tzram_layout.total_base, bl2_tzram_layout.total_base,
			bl2_tzram_layout.total_size,
			MT_MEMORY | MT_RW | MT_SECURE);
	mmap_add_region(BL_CODE_BASE, BL_CODE_BASE,
			BL_CODE_END - BL_CODE_BASE,
			MT_MEMORY | MT_RO | MT_SECURE);
#if USE_COHERENT_MEM
	mmap_add_region(BL_COHERENT_RAM_BASE, BL_COHERENT_RAM_BASE,
			BL_COHERENT_RAM_END - BL_COHERENT_RAM_BASE,
			MT_MEMORY | MT_RW | MT_SECURE);
#endif

	plat_add_mmio_map();

	init_xlat_tables();

	enable_mmu_el3(0);
	plat_octeontx_set_secondary_cpu_jump_addr(
				(uint64_t)plat_secondary_cold_boot_setup);
}

/*******************************************************************************
 * Reference to structures which holds the arguments which need to be passed
 * to BL31
 ******************************************************************************/

meminfo_t *bl2_plat_sec_mem_layout(void)
{
	return &bl2_tzram_layout;
}
