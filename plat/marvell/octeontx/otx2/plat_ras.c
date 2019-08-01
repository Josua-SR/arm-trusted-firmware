/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <lib/extensions/ras.h>

struct ras_interrupt otx2_ras_interrupts[] = {
};

struct err_record_info otx2_err_records[] = {
};

REGISTER_ERR_RECORD_INFO(otx2_err_records);
REGISTER_RAS_INTERRUPTS(otx2_ras_interrupts);
