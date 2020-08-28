/*
 * Copyright (C) 2020 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __PLAT_GHES_H__
#define __PLAT_GHES_H__

#ifndef LINUX_CPER_H

/*
 * Severity definition for error_severity in struct cper_record_header
 * and section_severity in struct cper_section_descriptor
 * (copied from linux/cper.h)
 */
enum {
	CPER_SEV_RECOVERABLE,
	CPER_SEV_FATAL,
	CPER_SEV_CORRECTED,
	CPER_SEV_INFORMATIONAL,
};

/* cper_sec_mem_err[_old] validation_bits (copied from linux/cper.h) */
#define CPER_MEM_VALID_ERROR_STATUS		0x0001
#define CPER_MEM_VALID_PA			0x0002
#define CPER_MEM_VALID_PA_MASK			0x0004
#define CPER_MEM_VALID_NODE			0x0008
#define CPER_MEM_VALID_CARD			0x0010
#define CPER_MEM_VALID_MODULE			0x0020
#define CPER_MEM_VALID_BANK			0x0040
#define CPER_MEM_VALID_DEVICE			0x0080
#define CPER_MEM_VALID_ROW			0x0100
#define CPER_MEM_VALID_COLUMN			0x0200
#define CPER_MEM_VALID_BIT_POSITION		0x0400
#define CPER_MEM_VALID_REQUESTOR_ID		0x0800
#define CPER_MEM_VALID_RESPONDER_ID		0x1000
#define CPER_MEM_VALID_TARGET_ID		0x2000
#define CPER_MEM_VALID_ERROR_TYPE		0x4000
#define CPER_MEM_VALID_RANK_NUMBER		0x8000
#define CPER_MEM_VALID_CARD_HANDLE		0x10000
#define CPER_MEM_VALID_MODULE_HANDLE		0x20000

/* ARM Processor Error Information Structure (copied from linux/cper.h) */
struct cper_arm_err_info {
	uint8_t		version;
	uint8_t		length;
	uint16_t	validation_bits;
	uint8_t		type;
	uint16_t	multiple_error;
	uint8_t		flags;
	uint64_t	error_info;
	uint64_t	virt_fault_addr;
	uint64_t	physical_fault_addr;
} __packed;

/* Old Memory Error Section UEFI 2.1, 2.2 (copied from linux/cper.h) */
struct cper_sec_mem_err_old {
	uint64_t	validation_bits;
	uint64_t	error_status;
	uint64_t	physical_addr;
	uint64_t	physical_addr_mask;
	uint16_t	node;
	uint16_t	card;
	uint16_t	module;
	uint16_t	bank;
	uint16_t	device;
	uint16_t	row;
	uint16_t	column;
	uint16_t	bit_pos;
	uint64_t	requestor_id;
	uint64_t	responder_id;
	uint64_t	target_id;
	uint8_t		error_type;
} __packed;

/* Memory Error Section UEFI >= 2.3 (copied from linux/cper.h )*/
struct cper_sec_mem_err {
	uint64_t	validation_bits;
	uint64_t	error_status;
	uint64_t	physical_addr;
	uint64_t	physical_addr_mask;
	uint16_t	node;
	uint16_t	card;
	uint16_t	module;
	uint16_t	bank;
	uint16_t	device;
	uint16_t	row;
	uint16_t	column;
	uint16_t	bit_pos;
	uint64_t	requestor_id;
	uint64_t	responder_id;
	uint64_t	target_id;
	uint8_t		error_type;
	uint8_t		reserved;
	uint16_t	rank;
	uint16_t	mem_array_handle;	/* card handle in UEFI 2.4 */
	uint16_t	mem_dev_handle;		/* module handle in UEFI 2.4 */
} __packed;
#endif // LINUX_CPER_H

#define OTX2_GHES_ERR_REC_FRU_TEXT_LEN 32
/* This is shared with Linux sdei-ghes driver */
struct otx2_ghes_err_record {
	union {
		struct cper_sec_mem_err_old  mcc;
		struct cper_sec_mem_err_old  mdc;
		struct cper_sec_mem_err_old  lmc;
		struct cper_arm_err_info     ap; /* application processor */
	} u;
	uint32_t                             severity; /* CPER_SEV_xxx */
	char fru_text[OTX2_GHES_ERR_REC_FRU_TEXT_LEN];
};

#define OTX2_GHES_ERR_RING_SIG ((int)'M' << 24 | 'R' << 16 | 'V' << 8 | 'L')
/* This is shared with Linux sdei-ghes driver */
struct otx2_ghes_err_ring {
	uint32_t volatile head;
	uint32_t volatile tail;
	uint32_t size;       /* ring size */
	uint32_t sig;        /* set to OTX2_GHES_ERR_RING_SIG if initialized */
	/* ring of records */
	struct otx2_ghes_err_record records[1] __aligned(8);
};

struct otx2_ghes_err_record *otx2_begin_ghes(const char *name,
		    struct otx2_ghes_err_ring **ringp);
void otx2_send_ghes(struct otx2_ghes_err_record *rec,
		    struct otx2_ghes_err_ring *err_ring,
		    int event);
struct fdt_ghes *otx2_find_ghes(const char *name);
void otx2_map_ghes(void);

#endif // __PLAT_GHES_H__
