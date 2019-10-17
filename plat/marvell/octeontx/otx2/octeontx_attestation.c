/*
 *
 * Copyright (C) 2016-2019 Marvell International Ltd.
 *
 * This program is provided "as is" without any warranty of any kind,
 * and is distributed under the applicable Marvell proprietary limited use
 * license agreement.
 */

#if ENABLE_ATTESTATION_SERVICE
#include <stdint.h>
#include <string.h>
#include <debug.h>
#include <platform_def.h>
#include <octeontx_common.h>

#include <mbedtls/md.h>
#include <mbedtls/pk.h>
#include <mbedtls/platform.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/cipher.h>
#include <plat_octeontx.h>
#include <endian.h>
#include <sh_fwdata.h>
#include <octeontx_attestation.h>

#define containerof(ptr, type, member) \
	(type *)((void *)ptr - offsetof(type, member))

struct list_head {
	struct list_head *next, *prev;
};

/* heap block definitions */
#define HEAP_BLOCK_SIZE_SMALL  0x80
#define HEAP_BLOCK_SIZE_MEDIUM 0x200
#define HEAP_BLOCK_SIZE_LARGE  0x400
#define HEAP_BLOCK_SIZE_GIANT  0x800

#define HEAP_BLOCK_COUNT_SMALL  96
#define HEAP_BLOCK_COUNT_MEDIUM 16
#define HEAP_BLOCK_COUNT_LARGE  4
#define HEAP_BLOCK_COUNT_GIANT  2

typedef struct heap_block_header {
	struct list_head list;
	size_t           len;
	/* helps to allocate smallest possible (all free blocks on same list) */
	size_t           smaller_len;
	uint8_t          buffer[];
} heap_block_header_t;

#define HEAP_BLOCK_HEADER_SIZE sizeof(heap_block_header_t)

union heap_block_small {
	heap_block_header_t hdr;
	uint8_t             data[HEAP_BLOCK_SIZE_SMALL+HEAP_BLOCK_HEADER_SIZE];
};

union heap_block_medium {
	heap_block_header_t hdr;
	uint8_t             data[HEAP_BLOCK_SIZE_MEDIUM+HEAP_BLOCK_HEADER_SIZE];
};

union heap_block_large {
	heap_block_header_t hdr;
	uint8_t             data[HEAP_BLOCK_SIZE_LARGE+HEAP_BLOCK_HEADER_SIZE];
};

union heap_block_giant {
	heap_block_header_t hdr;
	uint8_t             data[HEAP_BLOCK_SIZE_GIANT+HEAP_BLOCK_HEADER_SIZE];
};

/* Include the signing data */
#if defined(ATTESTATION_SIGNING_KEY_FILE)
#include ATTESTATION_SIGNING_KEY_FILE
#else
#error "Missing attestation signing key file."
#endif

/* This holds the unencrypted key used to sign the attestation information,
 * referred to herein as the 'signing key'.
 * The signing key is encrypted (see 'encrypted_sign_key' variable).
 * It must be decrypted using a secret key that is fused into FUSF_SW.
 * This buffer holds the key after it has been decrypted - see
 * 'decrypt_signing_key()'
 */
#define SIGNING_KEY_DER_LEN 121
uint8_t sign_key_der_buf[SIGNING_KEY_DER_LEN];

/* linked-list of 'free' and 'allocated' pointers used by heap alloc/free */
static struct list_head heap_free = { .next = &heap_free, .prev = &heap_free };
static struct list_head heap_used = { .next = &heap_used, .prev = &heap_used };

/* storage for heap */
static union heap_block_small heap_small[HEAP_BLOCK_COUNT_SMALL];
static union heap_block_medium heap_medium[HEAP_BLOCK_COUNT_MEDIUM];
static union heap_block_large heap_large[HEAP_BLOCK_COUNT_LARGE];
static union heap_block_giant heap_giant[HEAP_BLOCK_COUNT_GIANT];

/* denotes an empty linked-list */
#define list_empty(l) ((l)->next == (l))

/* This contains all the work areas used by the attestation signing code.
 * They are defined as a global variable rather than as local variables so as
 * not to overrun the stack.
 */
static struct {
	mbedtls_md_context_t md_ctx;
	mbedtls_pk_context pk_ctx;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	uint8_t nonce_buf[SW_ATT_INFO_NONCE_MAX_LEN];
	uint8_t digest[SHA256_HASH_LEN];
	uint8_t sig_buf[MBEDTLS_MPI_MAX_SIZE];
} mbedtls_signing_work;

/* This contains all the work areas used by the attestation decryption code.
 * They are defined as a global variable rather than as local variables so as
 * not to overrun the stack.
 */
#define AES_256_IV_LEN 16
#define AES_256_KEYLEN 32
#define KEY_DIGEST_LEN 32
static struct {
	mbedtls_cipher_context_t cipher_ctx;
	uint8_t IV[AES_256_IV_LEN];
	uint8_t key[AES_256_KEYLEN];
} mbedtls_decrypt_work;

/* Adds an entry to a linked-list. */
static inline void list_add(struct list_head *new, struct list_head *head)
{
	head->next->prev = new;
	new->next = head->next;
	new->prev = head;
	head->next = new;
}

/* Removes an entry from a linked-list. */
static inline void list_del(struct list_head *entry)
{
	entry->next->prev = entry->prev;
	entry->prev->next = entry->next;
}

/*
 * This initializes the heap (used by mbedtls during key decryption).
 */
static void init_heap(void)
{
	int i;

	if (!list_empty(&heap_free))
		return;

	for (i = 0; i < HEAP_BLOCK_COUNT_GIANT; i++) {
		list_add(&heap_giant[i].hdr.list, &heap_free);
		heap_giant[i].hdr.len = HEAP_BLOCK_SIZE_GIANT;
		heap_giant[i].hdr.smaller_len = HEAP_BLOCK_SIZE_LARGE;
	}
	for (i = 0; i < HEAP_BLOCK_COUNT_LARGE; i++) {
		list_add(&heap_large[i].hdr.list, &heap_free);
		heap_large[i].hdr.len = HEAP_BLOCK_SIZE_LARGE;
		heap_large[i].hdr.smaller_len = HEAP_BLOCK_SIZE_MEDIUM;
	}
	for (i = 0; i < HEAP_BLOCK_COUNT_MEDIUM; i++) {
		list_add(&heap_medium[i].hdr.list, &heap_free);
		heap_medium[i].hdr.len = HEAP_BLOCK_SIZE_MEDIUM;
		heap_medium[i].hdr.smaller_len = HEAP_BLOCK_SIZE_SMALL;
	}
	for (i = 0; i < HEAP_BLOCK_COUNT_SMALL; i++) {
		list_add(&heap_small[i].hdr.list, &heap_free);
		heap_small[i].hdr.len = HEAP_BLOCK_SIZE_SMALL;
		heap_small[i].hdr.smaller_len = 0; /* this is smallest block */
	}
}

/* This is the heap allocator used by mbedtls. */
void *mbedtls_calloc_octeon(size_t nmem, size_t size)
{
	heap_block_header_t *heap_blk;
	struct list_head *cur;
	size_t len;
	void *ret;

	len = nmem * size;
	//INFO("%s: %ld bytes\n", __func__, len);

	ret = NULL;

	/* search heap 'free' list for an appropriate block */
	cur = &heap_free;
	while ((cur = cur->next) != &heap_free) {
		heap_blk = containerof(cur, heap_block_header_t, list);
		if ((len <= heap_blk->len) && (len > heap_blk->smaller_len)) {
			ret = heap_blk->buffer;
			/* remove from 'free' list */
			list_del(cur);
			/* add to 'allocated' list */
			list_add(cur, &heap_used);
			memset(ret, 0, len);
			//INFO("%s: return %p\n", __func__, ret);
			return ret;
		}
	}

	INFO("%s: NULL (%ld B)\n", __func__, len);

	return NULL;
}

/* This is the heap allocator 'free' routine used by mbedtls */
void mbedtls_free_octeon(void *ptr)
{
	heap_block_header_t *heap_blk;
	struct list_head *cur;

	//INFO("%s: %p\n", __func__, ptr);

	heap_blk = containerof(ptr, heap_block_header_t, buffer);

	/* search heap 'allocated' list for owning block */
	cur = &heap_used;
	while ((cur = cur->next) != &heap_used) {
		heap_blk = containerof(cur, heap_block_header_t, list);
		if (ptr == heap_blk->buffer) {
			/* remove from 'allocated' list */
			list_del(cur);
			/* add to 'free' list */
			list_add(cur, &heap_free);
			memset(ptr, 0, heap_blk->len);
			//INFO("%s: %p OK\n", __func__, ptr);
			return;
		}
	}

	ERROR("%s: bad ptr %p\n", __func__, ptr);
}

/* This is used to cleanup/re-initialize the heap used by tls code.
 * TODO: Fix mbedtls memory leaks
 */
static void reinitialize_heap(void)
{
	heap_block_header_t *heap_blk;
	struct list_head *cur, *tmp;
	size_t count_small, count_medium, count_large, count_giant;

	/* scan heap 'allocated' list for any blocks */
	cur = heap_used.next;
	while (cur != &heap_used) {
		heap_blk = containerof(cur, heap_block_header_t, list);

		/* verbose debug code
		 * ERROR("Heap block leak: %p, %lu B\n",
		 *        heap_blk->buffer, heap_blk->len);
		 */

		/* save 'next' prior to altering it */
		tmp = cur->next;
		/* remove from 'allocated' list */
		list_del(cur);
		/* add to 'free' list */
		list_add(cur, &heap_free);
		memset(heap_blk->buffer, 0, heap_blk->len);
		cur = tmp;
	}

	/* count heap blocks in 'free' list */
	count_small = count_medium = count_large = count_giant = 0;
	cur = &heap_free;
	while ((cur = cur->next) != &heap_free) {
		heap_blk = containerof(cur, heap_block_header_t, list);
		switch (heap_blk->len) {
		case HEAP_BLOCK_SIZE_SMALL:
			count_small++;
			break;
		case HEAP_BLOCK_SIZE_MEDIUM:
			count_medium++;
			break;
		case HEAP_BLOCK_SIZE_LARGE:
			count_large++;
			break;
		case HEAP_BLOCK_SIZE_GIANT:
			count_giant++;
			break;
		default:
			ERROR("Heap: invalid free block %p\n",
			      heap_blk);
			ERROR("Heap: (buffer %p, %lu/%lu B)\n",
			      heap_blk->buffer, heap_blk->len,
			      heap_blk->smaller_len);
			cur = heap_free.prev; /* exit loop */
			break;
		}
	}
	/* verbose debug code
	 * INFO("Free heap blocks: %lu, %lu, %lu, %lu\n",
	 *       count_small, count_medium, count_large, count_giant);
	 */
}

/* copy data to unaligned destination */
void copy_bytes(uint8_t *dst, const uint8_t *src, size_t len)
{
	while (len--)
		*dst++ = *src++;
}

/* copy 16-bit data to unaligned destination */
void copy_data16(void *dst, uint16_t data16)
{
	uint8_t *d = dst;
	*d++ = (uint8_t)data16;
	*d++ = (uint8_t)(data16 >> 8);
}

/* copy 32-bit data to unaligned destination */
void copy_data32(void *dst, uint32_t data32)
{
	uint8_t *d = dst;
	*d++ = (uint8_t)data32;
	*d++ = (uint8_t)(data32 >> 8);
	*d++ = (uint8_t)(data32 >> 16);
	*d++ = (uint8_t)(data32 >> 24);
}

/* copy 64-bit data to unaligned destination */
static void copy_data64(void *dst, uint64_t data64)
{
	uint8_t *d = dst;
	*d++ = (uint8_t)data64;
	*d++ = (uint8_t)(data64 >> 8);
	*d++ = (uint8_t)(data64 >> 16);
	*d++ = (uint8_t)(data64 >> 24);
	*d++ = (uint8_t)(data64 >> 32);
	*d++ = (uint8_t)(data64 >> 40);
	*d++ = (uint8_t)(data64 >> 48);
	*d++ = (uint8_t)(data64 >> 56);
}

/*
 * This is used to decrypt the signing key that will be used to sign the
 * attestation information block.
 *
 * See definition of 'sign_key_der_buf' and 'encrypted_sign_key' above.
 *
 * Also see mbedtls 'aescrypt2' utility.
 */
int decrypt_signing_key(void)
{
	int ret, i;
	uint8_t *IV;
	uint8_t *key;
	uint8_t dec_buf[32];
	uint8_t *enc_data;
	size_t enc_off, enc_len, key_len, len, outlen;
	mbedtls_cipher_context_t *cipher_ctx;
	const mbedtls_cipher_info_t *cipher_info;
	const char *cipher_name = "AES-256-CTR";
	cavm_fusf_swx_t fusf_swx;

	/* Assign local pointers to work area buffers.
	 * The work area buffers are defined as global variables rather than
	 * local variables so as to not overrun the stack.
	 */
	cipher_ctx = &mbedtls_decrypt_work.cipher_ctx;
	IV = mbedtls_decrypt_work.IV;
	key = mbedtls_decrypt_work.key;

	cipher_info = mbedtls_cipher_info_from_string(cipher_name);
	if (cipher_info == NULL) {
		ERROR("Unable to locate cipher '%s'\n", cipher_name);
		ret = -1;
		goto exit;
	}

	mbedtls_cipher_init(cipher_ctx);
	ret = mbedtls_cipher_setup(cipher_ctx, cipher_info);
	if (ret) {
		ERROR("Error -0x%04x from cipher setup.\n", -ret);
		ret = -1;
		goto exit;
	}

	if (mbedtls_cipher_get_block_size(cipher_ctx) > sizeof(dec_buf)) {
		ERROR("Internal error: cipher block size > decode buf size\n");
		ret = -1;
		goto exit;
	}

	memset(IV, 0, AES_256_IV_LEN);
	memset(key, 0, AES_256_KEYLEN);
	memset(dec_buf, 0, sizeof(dec_buf));

	enc_len = sizeof(encrypted_sign_key);

	/* load decryption key from FUSF_SW(2..5) */
	key_len = AES_256_KEYLEN;
	for (i = 0; i < key_len / sizeof(uint64_t); i++) {
		fusf_swx.u = CSR_READ(CAVM_FUSF_SWX(i + 2));
		copy_data64(&((uint64_t *)key)[i], htobe64(fusf_swx.u));
	}

	enc_data = encrypted_sign_key;

	/* copy the IV */
	memcpy(IV, enc_data, AES_256_IV_LEN);
	enc_data += AES_256_IV_LEN;
	/* subtract length of IV */
	enc_len -= AES_256_IV_LEN;

	if (mbedtls_cipher_setkey(cipher_ctx, key, cipher_info->key_bitlen,
				 MBEDTLS_DECRYPT) != 0) {
		ERROR("Unable to set cipher key\n");
		ret = -1;
		goto exit;
	}

	if (mbedtls_cipher_set_iv(cipher_ctx, IV, AES_256_IV_LEN) != 0) {
		ERROR("Unable to set cipher IV\n");
		ret = -1;
		goto exit;
	}

	if (mbedtls_cipher_reset(cipher_ctx) != 0) {
		ERROR("Unable to reset cipher\n");
		ret = -1;
		goto exit;
	}

	/*
	 * Decrypt the ciphertext into plaintext.
	 *
	 * Security note: we are decrypting a private signing key.
	 * The code supports "decrypt-in-place", but we always
	 * decrypt into a separate buffer, which is cleared upon exit.
	 * This way, the [decrypted] private key is not left in memory,
	 * and there is no chance of leaking it.
	 *
	 */
	for (enc_off = 0; enc_off < enc_len; enc_off += len) {
		len = enc_len - enc_off;
		if (len > mbedtls_cipher_get_block_size(cipher_ctx))
			len = mbedtls_cipher_get_block_size(cipher_ctx);

		if ((mbedtls_cipher_update(cipher_ctx, &enc_data[enc_off], len,
					   dec_buf, &outlen) != 0) ||
		    (outlen != len)) {
			ERROR("decryption error: ilen %ld, olen %ld\n", len,
			      outlen);
			ret = -1;
			goto exit;
		}

		/* Copy decrypted data to output buffer.
		 * NOTE: we can use 'enc_off' for the output buffer
		 * because the code supports 'decrypt-in-place'
		 * (thus the offsets are the same) - see security note above.
		 */
		copy_bytes(&sign_key_der_buf[enc_off], dec_buf, len);
	}

	ret = 0;

exit:
	/* In case of error, don't leak the [decrypted] private signing key */
	if (ret)
		memset(sign_key_der_buf, 0, sizeof(sign_key_der_buf));
	memset(IV, 0, AES_256_IV_LEN);
	memset(key, 0, AES_256_KEYLEN);
	memset(dec_buf, 0, sizeof(dec_buf));

	mbedtls_cipher_free(cipher_ctx);

	return ret;
}

/* Returns the base address of the attestation information buffer. */
void *attestation_info_base(void)
{
	return (void *)SW_ATTEST_INFO_BASE;
}

/* TODO: formalize return codes */
/*
 * This is used to generate the attestation information in response to an SMC.
 *
 * on entry,
 *   nonce:
 *     The caller has written NONCE data (of nonce_len bytes) into the
 *     attestation information buffer (member 'input_nonce').  The nonce data
 *     is copied into a local buffer, then the attestation information buffer
 *     is filled with appropriate data (see Software Attestation Specification).
 *
 *  TODO: document attestation information buffer address
 *
 * returns,
 *   on success, 0
 *   on error, negative return value
 */
intptr_t generate_attestation_info(uint64_t nonce_len)
{
#define MBEDTLS_ECDSA_SIG_MAX_LEN 72
#define SHA256_HASH_LEN           32
#define MAX_SIGNING_ATTEMPTS      10
	size_t tot_len, tlv_len, len, tlv_img_id_cnt, signing_attempts;
	int ret;
	void *cert, *sig;
	sw_attestation_info_hdr_t *att_info_hdr;
	sw_attestation_tlv_t *tlv;
	const char *pers = "attestation_rnd_seed";
	const mbedtls_md_info_t *md_info;
	mbedtls_md_context_t *md_ctx;
	mbedtls_pk_context *pk_ctx;
	mbedtls_entropy_context *entropy;
	mbedtls_ctr_drbg_context *ctr_drbg;
	uint8_t *nonce_buf;
	uint8_t *digest;
	uint8_t *sig_buf;
	void *(*mbedtls_calloc_orig)(size_t num, size_t size);
	void (*mbedtls_free_orig)(void *buf);
	cavm_rnm_ctl_status_t ctl_status;
	/* TODO: move to header */
	extern octeontx_bl_platform_args_t octeontx_bl31_plat_args;

	/* Assign local pointers to work area buffers.
	 * The work area buffers are defined as global variables rather than
	 * local variables so as to not overrun the stack.
	 */
	md_ctx = &mbedtls_signing_work.md_ctx;
	pk_ctx = &mbedtls_signing_work.pk_ctx;
	entropy = &mbedtls_signing_work.entropy;
	ctr_drbg = &mbedtls_signing_work.ctr_drbg;
	nonce_buf = mbedtls_signing_work.nonce_buf;
	digest = mbedtls_signing_work.digest;
	sig_buf = mbedtls_signing_work.sig_buf;

	init_heap();

	memset(nonce_buf, 0, SW_ATT_INFO_NONCE_MAX_LEN);
	att_info_hdr = (sw_attestation_info_hdr_t *)attestation_info_base();

	if (nonce_len > SW_ATT_INFO_NONCE_MAX_LEN) {
		ERROR("Input nonce too large (%lu vs %u)\n",
		      (long)nonce_len, SW_ATT_INFO_NONCE_MAX_LEN);
		return -2;
	}

	/* copy the caller-supplied nonce from the transfer buffer */
	copy_bytes(nonce_buf, (uint8_t *)att_info_hdr->input_nonce, nonce_len);

	/* INIT_BIN, ATF_BL1, BOARD_DT, LINUX_DT, SCP_TBL1FW, MCP_TBL1FW,
	 * AP_TBL1FW, ATF_BL2, ATF_BL31, ATF_BL33
	 */
	tlv_img_id_cnt = 10;

	tot_len = sizeof(*att_info_hdr);
	/* each image signature is a SHA256 hash in a TLV */
	tlv_len = (tlv_img_id_cnt *
		   (SHA256_HASH_LEN + sizeof(sw_attestation_tlv_t)));
	/* the nonce is added as a TLV element */
	tlv_len += nonce_len + sizeof(sw_attestation_tlv_t);
	tot_len += tlv_len;
	/* add-in length of DER-encoded certificate */
	tot_len += sizeof(sign_cert_der);
	/* add-in the [maximum] length of the signature */
	tot_len += MBEDTLS_ECDSA_SIG_MAX_LEN;

	if (tot_len > SW_ATTEST_INFO_SIZE) {
		ERROR("SW_ATTEST_INFO_SIZE mis-configuration.\n");
		return -2;
	}

	copy_data32(&att_info_hdr->magic_be, htobe32(ATTESTATION_MAGIC_ID));
	copy_data16(&att_info_hdr->tlv_len_be, htobe16(tlv_len));
	copy_data16(&att_info_hdr->total_len_be, htobe16(tot_len));
	copy_data16(&att_info_hdr->certificate_len_be,
		    htobe16(sizeof(sign_cert_der)));
	/* use max len for signing; afterwards, re-do if sig len is smaller */
	copy_data16(&att_info_hdr->signature_len_be,
		    htobe16(MBEDTLS_ECDSA_SIG_MAX_LEN));

	/* populate TLV fields */
	tlv = att_info_hdr->tlv_list;

	copy_data16(&tlv->type_be, htobe16(ATT_IMG_INIT_BIN));
	len = sizeof(octeontx_bl31_plat_args.init_bin_sig);
	copy_data16(&tlv->length_be, htobe16(len));
	copy_bytes(tlv->value, octeontx_bl31_plat_args.init_bin_sig, len);
	tlv = (sw_attestation_tlv_t *)&tlv->value[len];

	copy_data16(&tlv->type_be, htobe16(ATT_IMG_ATF_BL1));
	len = sizeof(octeontx_bl31_plat_args.atf_bl1_sig);
	copy_data16(&tlv->length_be, htobe16(len));
	copy_bytes(tlv->value, octeontx_bl31_plat_args.atf_bl1_sig, len);
	tlv = (sw_attestation_tlv_t *)&tlv->value[len];

	copy_data16(&tlv->type_be, htobe16(ATT_IMG_BOARD_DT));
	len = sizeof(octeontx_bl31_plat_args.board_dt_sig);
	copy_data16(&tlv->length_be, htobe16(len));
	copy_bytes(tlv->value, octeontx_bl31_plat_args.board_dt_sig, len);
	tlv = (sw_attestation_tlv_t *)&tlv->value[len];

	copy_data16(&tlv->type_be, htobe16(ATT_IMG_LINUX_DT));
	len = sizeof(octeontx_bl31_plat_args.linux_dt_sig);
	copy_data16(&tlv->length_be, htobe16(len));
	copy_bytes(tlv->value, octeontx_bl31_plat_args.linux_dt_sig, len);
	tlv = (sw_attestation_tlv_t *)&tlv->value[len];

	copy_data16(&tlv->type_be, htobe16(ATT_IMG_SCP_TBL1FW));
	len = sizeof(octeontx_bl31_plat_args.scp_tbl1fw_sig);
	copy_data16(&tlv->length_be, htobe16(len));
	copy_bytes(tlv->value, octeontx_bl31_plat_args.scp_tbl1fw_sig, len);
	tlv = (sw_attestation_tlv_t *)&tlv->value[len];

	copy_data16(&tlv->type_be, htobe16(ATT_IMG_MCP_TBL1FW));
	len = sizeof(octeontx_bl31_plat_args.mcp_tbl1fw_sig);
	copy_data16(&tlv->length_be, htobe16(len));
	copy_bytes(tlv->value, octeontx_bl31_plat_args.mcp_tbl1fw_sig, len);
	tlv = (sw_attestation_tlv_t *)&tlv->value[len];

	copy_data16(&tlv->type_be, htobe16(ATT_IMG_AP_TBL1FW));
	len = sizeof(octeontx_bl31_plat_args.ap_tbl1fw_sig);
	copy_data16(&tlv->length_be, htobe16(len));
	copy_bytes(tlv->value, octeontx_bl31_plat_args.ap_tbl1fw_sig, len);
	tlv = (sw_attestation_tlv_t *)&tlv->value[len];

	copy_data16(&tlv->type_be, htobe16(ATT_IMG_ATF_BL2));
	len = sizeof(octeontx_bl31_plat_args.atf_bl2_sig);
	copy_data16(&tlv->length_be, htobe16(len));
	copy_bytes(tlv->value, octeontx_bl31_plat_args.atf_bl2_sig, len);
	tlv = (sw_attestation_tlv_t *)&tlv->value[len];

	copy_data16(&tlv->type_be, htobe16(ATT_IMG_ATF_BL31));
	len = sizeof(octeontx_bl31_plat_args.atf_bl31_sig);
	copy_data16(&tlv->length_be, htobe16(len));
	copy_bytes(tlv->value, octeontx_bl31_plat_args.atf_bl31_sig, len);
	tlv = (sw_attestation_tlv_t *)&tlv->value[len];

	copy_data16(&tlv->type_be, htobe16(ATT_IMG_ATF_BL33));
	len = sizeof(octeontx_bl31_plat_args.atf_bl33_sig);
	copy_data16(&tlv->length_be, htobe16(len));
	copy_bytes(tlv->value, octeontx_bl31_plat_args.atf_bl33_sig, len);
	tlv = (sw_attestation_tlv_t *)&tlv->value[len];

	/* append the NONCE as a TLV element */
	copy_data16(&tlv->type_be, htobe16(ATT_SIG_NONCE));
	len = nonce_len;
	copy_data16(&tlv->length_be, htobe16(len));
	copy_bytes(tlv->value, nonce_buf, nonce_len);
	tlv = (sw_attestation_tlv_t *)&tlv->value[len];

	/* populate the certificate (contiguous to TLV list) */
	cert = (void *)tlv;
	len = be16toh(att_info_hdr->certificate_len_be);
	copy_bytes(cert, sign_cert_der, len);
	sig = cert + len;

	/* save original allocator/free functions, then assign ours */
	mbedtls_calloc_orig = mbedtls_calloc;
	mbedtls_free_orig = mbedtls_free;
	mbedtls_platform_set_calloc_free(mbedtls_calloc_octeon,
					 mbedtls_free_octeon);

	/* verbose debug code
	 * INFO("Decrypting attestation signing key...\n");
	 */
	ret = decrypt_signing_key();
	if (ret) {
		ERROR("Error decrypting signing key.\n");
		ret = -3;
		goto exit;
	}

	/* enable random number generator for the entropy source
	 * mbedtls_hardware_poll'()'.
	 */
	ctl_status.u = CSR_READ(CAVM_RNM_CTL_STATUS);
	ctl_status.s.rng_en = 1;
	CSR_WRITE(CAVM_RNM_CTL_STATUS, ctl_status.u);

	/* one-time initialization */
	mbedtls_entropy_init(entropy);
	mbedtls_ctr_drbg_init(ctr_drbg);
	mbedtls_pk_init(pk_ctx);
	INFO("Seeding PRNG...\n");
	ret = mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func,
				    entropy,
				    (const uint8_t *)pers,
				    strlen(pers));
	if (ret) {
		/* mbedtls preferred method of error display */
		ERROR("Error -0x%04x seeding PRNG.\n", -ret);
		ret = -2;
		goto exit;
	}

	/* parse private key */
	INFO("Parsing private key...\n");
	len = sizeof(sign_key_der_buf);
	ret = mbedtls_pk_parse_key(pk_ctx, sign_key_der_buf, len, NULL, 0);
	if (ret) {
		/* mbedtls preferred method of error display */
		ERROR("Error -0x%04x parsing private key.\n", -ret);
		ret = -2;
		goto exit;
	}

	/* sign attestation info structure */
	INFO("Creating signature...\n");
	signing_attempts = 1;
	ret = 0;
	do {
		/* create digest of data to be signed */
		INFO("Generating digest (datalen %lu)...\n",
		     (sig - (void *)att_info_hdr));

		md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
		mbedtls_md_init(md_ctx);
		ret = mbedtls_md_setup(md_ctx, md_info, 0);
		if (ret) {
			/* mbedtls preferred method of error display */
			ERROR("Error -0x%04x from md_setup\n", -ret);
			ret = -2;
			break;
		}
		ret = mbedtls_md_starts(md_ctx);
		if (ret) {
			/* mbedtls preferred method of error display */
			ERROR("Error -0x%04x from md_starts\n", -ret);
			ret = -2;
			break;
		}
		/* exclude signature field from digest */
		ret = mbedtls_md_update(md_ctx, (uint8_t *)att_info_hdr,
					(sig - (void *)att_info_hdr));
		if (ret) {
			/* mbedtls preferred method of error display */
			ERROR("Error -0x%04x from md_update\n", -ret);
			ret = -2;
			break;
		}
		ret = mbedtls_md_finish(md_ctx, digest);
		if (ret) {
			/* mbedtls preferred method of error display */
			ERROR("Error -0x%04x from md_finish\n", -ret);
			ret = -2;
			break;
		}
		mbedtls_md_free(md_ctx);

		ret = mbedtls_pk_sign(pk_ctx, MBEDTLS_MD_SHA256, digest,
				      SHA256_HASH_LEN, sig_buf, &len,
				      mbedtls_ctr_drbg_random, ctr_drbg);
		if (ret) {
			/* mbedtls preferred method of error display */
			ERROR("Error -0x%04x signing digest\n", -ret);
			ret = -2;
			break;
		}

		/* Verify that signature len matches what we used; if it is
		 * different (i.e. smaller), then regenerate data, digest and
		 * signature.
		 */
		if (len > MBEDTLS_ECDSA_SIG_MAX_LEN) {
			ERROR("Signature too long (%lu vs %lu)\n", len,
			      (long)MBEDTLS_ECDSA_SIG_MAX_LEN);
			ret = -1;
			break;
		} else if (len == be16toh(att_info_hdr->signature_len_be)) {
			/* copy the signature into the data field */
			copy_bytes(sig, sig_buf, len);
			ret = 0;
			break;
		}
		INFO("signature len %ld (vs %d), retrying\n", len,
		     be16toh(att_info_hdr->signature_len_be));

		/* Adjust the appropriate length fields in the data */
		tot_len -= be16toh(att_info_hdr->signature_len_be);
		tot_len += len;
		copy_data16(&att_info_hdr->total_len_be, htobe16(tot_len));
		copy_data16(&att_info_hdr->signature_len_be, htobe16(len));

		/* The mbedtls library can produce signatures of different
		 * lengths, depending upon the data.  In this case,
		 * re-calculate the signature due to the different length.
		 */
		if (signing_attempts >= MAX_SIGNING_ATTEMPTS) {
			ERROR("Failed signing after %lu attempts\n",
			      signing_attempts);
			ret = -2;
			break;
		}
		signing_attempts++;
	} while (!ret);

exit:
	/* don't leak the [decrypted] private signing key (or anything else) */
	memset(sign_key_der_buf, 0, sizeof(sign_key_der_buf));
	memset(nonce_buf, 0, SW_ATT_INFO_NONCE_MAX_LEN);

	/* restore original mbedtls alloc/free */
	mbedtls_platform_set_calloc_free(mbedtls_calloc_orig,
					 mbedtls_free_orig);

	reinitialize_heap();

	return ret;
}

/*
 * Entropy source for mbedtls (see definition 'MBEDTLS_ENTROPY_HARDWARE_ALT').
 */
int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len,
			  size_t *olen)
{
	size_t read;

	read = 0;

	while (read++ < len)
		*output++ = (unsigned char)CSR_READ(CAVM_RNM_RANDOM);

	*olen = len;

	return 0;
}
#endif  /* #if ENABLE_ATTESTATION_SERVICE */
