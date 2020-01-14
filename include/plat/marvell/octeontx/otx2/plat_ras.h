/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __PLAT_RAS_H__
#define __PLAT_RAS_H__

#include <lib/extensions/ras.h>
#include <plat_board_cfg.h>

/*
 * RAS interrupt handlers that are expected to be implemented by
 * family/platform.
 */
uint64_t otx2_mdc_isr(uint32_t id, uint32_t flags, void *cookie);
uint64_t otx2_mcc_isr(uint32_t id, uint32_t flags, void *cookie);
int otx2_mdc_probe(const struct err_record_info *info, int *probe_data);
int otx2_mcc_probe(const struct err_record_info *info, int *probe_data);

/*
 * API of plat_ras.c
 */
int otx2_ras_init(void);
int plat_dram_ras_init(void);
int lmcoe_ras_setup(int mcc, int lmcoe);
int lmcoe_ras_check_ecc_errors(int mcc, int lmcoe);
void l2c_flush(void);
void ras_rewrite_cacheline(uint64_t physaddr, int secure);
extern const char *ras_serr_str[];

/* enable reporting on a _RAS_ERRnn object, with thresh events until irq */
#define arm_err_nn(thresh, csr_family, ...) __arm_err_nn(thresh, \
					csr_family##FR(__VA_ARGS__), \
					csr_family##CTLR(__VA_ARGS__), \
					csr_family##MISC0(__VA_ARGS__))
extern void __arm_err_nn(int thresh, uint64_t fr_r, uint64_t ctlr_r,
						    uint64_t misc0_r);

/* DEBUG_RAS requires DEBUG, multiplier determines detail depth */
#define DEBUG_RAS (DEBUG * 1)

#ifndef noprintf
__attribute__ ((format (printf, 1, 2)))
static inline int noprintf(const char *fmt, ...)
{
	return 0;
}
#define noprintf noprintf
#endif

#if DEBUG_RAS
# define debug_ras(...) printf(__VA_ARGS__)
#else
# define debug_ras(...) noprintf(__VA_ARGS__)
#endif

#if DEBUG_RAS >= 2
# define debug2ras(...) printf(__VA_ARGS__)
#else
# define debug2ras(...) noprintf(__VA_ARGS__)
#endif

#if DEBUG_RAS >= 3
# define debug3ras(...) printf(__VA_ARGS__)
#else
# define debug3ras(...) noprintf(__VA_ARGS__)
#endif

/*
 * Atomically adds a signed value to a 64 bit (aligned) memory location.
 *
 * This version does not perform 'sync' operations to enforce memory
 * operations.  This should only be used when there are no memory operation
 * ordering constraints.  (This should NOT be used for reference counting -
 * use the standard version instead.)
 *
 * @param ptr    address in memory to add incr to
 * @param incr   amount to increment memory location by (signed)
 */
static inline void ras_atomic_add64_nosync(int64_t *ptr, int64_t incr)
{
	__asm__ volatile ("ldadd %x[i], xzr, [%[b]]"
		      : [r] "+m" (*ptr)
		      : [i] "r" (incr), [b]"r" (ptr)
		      : "memory");
}

static inline int is_asim(void)
{
	static int memo = -1;

	if (memo < 0)
		memo = !strncmp(plat_octeontx_bcfg->bcfg.board_model,
				"asim-", 5);
	return memo;
}

#endif /* __PLAT_RAS_H__ */
