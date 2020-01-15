/*
 * Copyright (C) 2019 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

/* non-DRAM EDAC driver for OcteonTX2 */

#include <arch.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <platform_def.h>
#include <octeontx_irqs_def.h>
#include <octeontx_common.h>
#include <plat_otx2_configuration.h>
#include <debug.h>
#include <bl31/interrupt_mgmt.h>
#include <plat_board_cfg.h>
#include <octeontx_security.h>
#include <octeontx_dram.h>
#include <octeontx_utils.h>
#include <octeontx_ehf.h>
#include <tools_share/uuid.h>
#include <plat/common/platform.h>
#include <plat_ras.h>
#include <drivers/delay_timer.h>
#include <cavm-csrs-ccu.h>
#include <octeontx_ehf.h>

volatile int edac_active; /* exclude polling in startup & SMC */

const char *ras_serr_str[256] = {
	[CAVM_RAS_SERR_E_NONE]			= "(none)",
	[CAVM_RAS_SERR_E_IMPDEF1]		= "IMPDEF1",
	[CAVM_RAS_SERR_E_SRAM_DATA]		= "SRAM",
	[CAVM_RAS_SERR_E_PIN]			= "PIN",
	[CAVM_RAS_SERR_E_ASSERTION]		= "ASSERT",
	[CAVM_RAS_SERR_E_DATAPATH]		= "D_PATH",
	[CAVM_RAS_SERR_E_CAM_DATA]		= "CAM_D",
	[CAVM_RAS_SERR_E_CAM_TAG]		= "CAM_T",
	[CAVM_RAS_SERR_E_TLB_DATA]		= "TLB_D",
	[CAVM_RAS_SERR_E_TLB_ADDRCTL]		= "TLB_A",
	[CAVM_RAS_SERR_E_PRODUCER_DATA]		= "PRO_D",
	[CAVM_RAS_SERR_E_PRODUCER_ADDRCTL]	= "PRO_A",
	[CAVM_RAS_SERR_E_EXT_DATA]		= "EXT_D",
	[CAVM_RAS_SERR_E_SW_BAD_ADDR]		= "ENOMEM",
	[CAVM_RAS_SERR_E_SW_BAD_ACCESS]		= "EACCES",
	[CAVM_RAS_SERR_E_SW_BAD_STATE]		= "EBUSY",
	[CAVM_RAS_SERR_E_REG_DATA]		= "REG_D",
	[CAVM_RAS_SERR_E_REG_CTL]		= "REG_C",
	[CAVM_RAS_SERR_E_SLAVE]			= "SLAVE",
	[CAVM_RAS_SERR_E_EXT_TO]		= "EXT_TO",
	[CAVM_RAS_SERR_E_INT_TO]		= "INT_TO",
	[CAVM_RAS_SERR_E_DEF_MAS]		= "DEF_MAS",
};

union mdc_ras_entry_s {
	uint64_t u;
	struct mdc_ras_entry_s_s {
		uint64_t reserved_0_17:18;
		uint64_t ras_poison:2;
		uint64_t ras_transient:1;
		uint64_t ras_uet:3;
		uint64_t ras_serr:8;
		uint64_t ras_id:32;
	} s;
	struct mdc_ras_entry_s_cn96xxp3 {
		uint64_t reserved_0_17:18;
		uint64_t ras_poison:2;
		uint64_t ras_transient:1;
		uint64_t ras_uet:3;
		uint64_t ras_serr:8;
		uint64_t ras_id:32;
	} cn96xxp3;
};

union mdc_bist_config {
	uint64_t u;
	struct mdc_bist_config_s {
		uint64_t stagger_period:16;
		uint64_t stagger_disable:1;
		uint64_t bisr_soft_disable_xor:1;
		uint64_t mdn_start_ratio:4;
		uint64_t mdh_start_ratio:3;
		uint64_t mdc_broadcast:1;
		uint64_t clock_gating_enable:1;
		uint64_t reserved_27_63:37;
	} s;
};
union mdc_bist_control {
	uint64_t u;
	struct mdc_bist_control_s {
		uint64_t start:1;
		uint64_t clear:1;
		uint64_t reset:1;
		uint64_t reserved_3_63:61;
	} s;
};

union mdc_const {
	uint64_t u;
	struct mdc_const_s {
		uint64_t reserved_0_15:16;
		uint64_t max_node_id:10;
		uint64_t max_hub_id:8;
		uint64_t max_chain_id:5;
		uint64_t reserved_39_63:25;
	} s;
};
union mdc_ecc_config {
	uint64_t u;
	struct mdc_ecc_config_s {
		uint64_t polling_period:32;
		uint64_t reserved_32_63:32;
	} s;
};
union mdc_ecc_status {
	uint64_t u;
	struct mdc_ecc_status_s {
		uint64_t sbe:1;
		uint64_t dbe:1;
		uint64_t sbe_plus:1;
		uint64_t dbe_plus:1;
		uint64_t reserved_4_15:12;
		uint64_t node_id:10;
		uint64_t hub_id:7;
		uint64_t chain_id:3;
		uint64_t reserved_36_47:12;
		uint64_t row:14;
		uint64_t reserved_62_63:2;
	} s;
};
union mdc_int_ena_w1c {
	uint64_t u;
	struct mdc_int_ena_w1c_s {
		uint64_t ecc_error:1;
		uint64_t reserved_1_63:63;
	} s;
};

union mdc_win_cmd {
	uint64_t u;
	struct mdc_win_cmd_s {
		uint64_t csr_id:8;
		uint64_t we:1;
		uint64_t bc_nodes:1;
		uint64_t bc_chains:1;
		uint64_t reserved_11_15:5;
		uint64_t node_id:10;
		uint64_t hub_id:7;
		uint64_t chain_id:3;
		uint64_t reserved_36_63:28;
	} s;
};
union mdc_win_dat {
	uint64_t u;
	struct mdc_win_dat_s {
		uint64_t data:32;
		uint64_t read_valid:1;
		uint64_t pending:1;
		uint64_t timeout:1;
		uint64_t reserved_35_63:29;
	} s;
};

#define MDC_RAS_UET_E_NOUC	4
#define MDC_RAS_UET_E_UC	0
#define MDC_RAS_UET_E_UEO	2
#define MDC_RAS_UET_E_UER	3
#define MDC_RAS_UET_E_UEU	1
#define MDC_ACTIVE_PC		0x87e0100000e8ull
#define MDC_BIST_CONFIG		0x87e010000008ull
#define MDC_BIST_CONTROL	0x87e010000028ull
#define MDC_CONST		0x87e010000068ull
#define MDC_ECC_CONFIG		0x87e010000038ull
#define MDC_ECC_STATUS		0x87e010000040ull
#define MDC_INT_ENA_W1C		0x87e010000058ull
#define MDC_INT_ENA_W1S		0x87e010000060ull
#define MDC_INT_W1C		0x87e010000048ull
#define MDC_INT_W1S		0x87e010000050ull
#define MDC_WIN_CMD		0x87e010000010ull
#define MDC_WIN_DAT		0x87e010000018ull
#define MDC_RAS_ROM		0x87e010010000ull

union mdn_const {
	uint32_t u;
	struct mdn_const_s {
		uint32_t cols:11;
		uint32_t rows:14;
		uint32_t bist:1;
		uint32_t bisr:1;
		uint32_t ecc:1;
		uint32_t debug_read:1;
		uint32_t virt:1;
		uint32_t reserved_30_31:2;
	} s;
};
#define MDN_BIST_CONFIG		0x00
#define MDN_BIST_CONTROL	0x04
#define MDN_BIST_STATUS		0x08
#define MDN_DEBUG_DEFECT	0x1c
#define MDN_DEBUG_MARCH		0x20
#define MDN_DEBUG_SKID		0x24
#define MDN_BISR_REPAIR		0x28
#define MDN_ECC_CONFIG		0x2c
#define MDN_ECC_IRQ		0x30
#define MDN_ECC_STATUS		0x34
#define MDN_CONST		0x40
#define MDN_DEBUG		0x80

//#define RAS_MAX_MEM_CHAINS	32
//int64_t __ras_cache_ecc_single_bit_errors[RAS_MAX_MEM_CHAINS];
//int64_t __ras_cache_ecc_double_bit_errors[RAS_MAX_MEM_CHAINS];

static uint64_t mdc_ras_romx(unsigned int a)
{
	int rsize = 8192;

	if (IS_OCTEONTX_PN(read_midr(), T98PARTNUM))
		rsize = 16384;

	if (a >= rsize)
		return 0;

	return octeontx_read64(MDC_RAS_ROM + 8 * a);
}

static union mdc_ras_entry_s mdc_read_ras_entry(int chain, int hub, int node)
{
	uint64_t ver = mdc_ras_romx(0);
	uint64_t cbase, hbase;
	union mdc_ras_entry_s leaf = { .u = 0 };
	int cdex, hdex, ndex;
	static int once;

	if (ver != 1) {
		if (!once)
			ERROR("Unsupported MDC_RAS_ROM version %llx\n", ver);
		once = 1;
		return leaf;
	}

	cdex = 1 + chain;

	cbase = mdc_ras_romx(cdex);
	debug2ras("RAS_ROM[1+c:%x=%x] cbase %llx\n", chain, cdex, cbase);

	hdex = cbase + hub;
	hbase = mdc_ras_romx(hdex);
	debug2ras("RAS_ROM[cbase+h:%x=%x] hbase %llx\n", hub, hdex, hbase);

	ndex = hbase + node;
	leaf.u = mdc_ras_romx(ndex);
	debug2ras("RAS_ROM[hbase+n:%x=%x] leaf %llx\n", node, ndex, leaf.u);

	return leaf;
}

static union mdc_win_dat mdn_rd(union mdc_win_cmd cmd)
{
	union mdc_win_dat dat;

	cmd.s.we = 0;
	octeontx_write64(MDC_WIN_CMD, cmd.u);
	do {
		dat.u = octeontx_read64(MDC_WIN_DAT);
		udelay(10);
	} while (!dat.s.read_valid && !dat.s.timeout && !is_asim());
	return dat;
}

static int mdn_wr(union mdc_win_cmd cmd, union mdc_win_dat dat)
{
	cmd.s.we = 1;
	octeontx_write64(MDC_WIN_DAT, dat.u);
	octeontx_write64(MDC_WIN_CMD, cmd.u);
	do {
		dat.u = octeontx_read64(MDC_WIN_DAT);
		udelay(10);
	} while (dat.s.pending && !dat.s.timeout && !is_asim());
	return dat.s.timeout ? -EBUSY : 0;
}

static void sys_wbillci(int set, int row)
{
	int sets = 0x400; // trim by xxx_const
	uint64_t param;

	if (set < 0) {
		for (set = 0; set < sets; set++)
			sys_wbillci(set, row);
		return;
	}

	param = (row & 0xf) << 17;
	param |= (set & 0x3ff) << 7;
	/*
	 * L2 Cache Index Writeback Invalidate
	 * TODO: see/expand kernel's ./arch/arm64/include/asm/sysreg.h
	 *        and bdk's ./libbdk-arch/bdk-asm.h
	 * __asm__ volatile("sys cvmcachewbil2i, %0" : : "r"(param));
	 */
	__asm__ volatile("sys #0,c11,c0,#5, %0" : : "r"(param));
}

/*
 * Sketch a fatal-error handler
 * platfoms may override
 * this should move to pm_ops
 */
#pragma weak handle_fatal_ecc_by_reboot
int handle_fatal_ecc_by_reboot = 1;

#pragma weak handle_fatal
void handle_fatal(void)
{
	extern void __dead2 octeontx_scp_sys_reboot(void);

	if (handle_fatal_ecc_by_reboot) {
		/*
		 * System integrity has been compromised,
		 * but pausing to allow UART to drain details to console.
		 * Until we can be sure the ATF secure area is undamaged
		 * this should ideally be a NoWayOut watchdog start
		 * rather than an active delay ...
		 */
		printf("Fatal ERROR: rebooting\n");
		mdelay(1000);
		octeontx_scp_sys_reboot();
	}
}

/*
 * To suppress repeated errors, as MDC clears slowly, skip dups.
 * Cause here is possibly automatic retries on error.
 * Special case, zero never matches, so can be used to periodically
 * purge old entries, so their real recurrence is eventually noted.
 * So at 1Hz poll, a single dup err remains hidden for RING seconds.
 */
#define MDC_DUP_RING 32
static int mdc_errs;
static int mdc_dup(uint64_t val)
{
	static uint64_t ring[MDC_DUP_RING];
	int slot;

	for (slot = 0; val && slot < MDC_DUP_RING; slot++)
		if (val == ring[slot])
			return 1;
	ring[++mdc_errs % MDC_DUP_RING] = val;
	return 0;
}

static void ccu_read_err(int ccu, int b, union cavm_mdc_ecc_status mes,
			 int quiet)
{
	union cavm_ccux_pic_mtdx_err mtd;
	union cavm_ccux_pic_tagx_err tag;
	union cavm_ccux_tadx_dat_err tad;
	union cavm_ccux_tadx_xbf_err xbf;

	mtd.u = octeontx_read64(CAVM_CCUX_PIC_MTDX_ERR(ccu, b));
	if (mtd.s.sbe || mtd.s.dbe) {
		if (!quiet)
			printf("CCU%d_PIC_MTD%d_ERR %llx\n", ccu, b, mtd.u);
		/* scrub as in HM-B2.1E pp 281 */
		sys_wbillci(mtd.s.addr & 0x3ff, mes.s.row + 5 * b);
		octeontx_write64(CAVM_CCUX_PIC_MTDX_ERR(ccu, b), mtd.u);
	}
	tag.u = octeontx_read64(CAVM_CCUX_PIC_TAGX_ERR(ccu, b));
	if (tag.s.parerr) {
		if (!quiet)
			printf("CCU%d_PIC_TAG%d_ERR %llx\n", ccu, b, tag.u);
		octeontx_write64(CAVM_CCUX_PIC_TAGX_ERR(ccu, b), tag.u);
	}
	tad.u = octeontx_read64(CAVM_CCUX_TADX_DAT_ERR(ccu, b));
	if (tad.s.sbe || tad.s.dbe) {
		uint64_t param, idx, way;
		int dtg;

		if (!quiet)
			printf("CCU%d_TAD%d_DAT_ERR %llx\n", ccu, b, tad.u);
		octeontx_write64(CAVM_CCUX_TADX_DAT_ERR(ccu, b), tad.u);
		/* scrub as in HM-B2.1E pp 283
		 * mes.s.row sufficient to scrub, with dtg=0, way[4]=0.
		 *               way[3:0]=row[29..26], idx[9:0]=row[25:16]
		 */
		idx = (mes.s.row >> 16) & 0x3ff;
		way = (mes.s.row >> 26) & 0xf;

		/* could reduce to
		 * __asm__ volatile(SYS_CVMCACHE_WBI_L2_INDEXED ", %0"
		 *		    : : "r"(param));
		 * but currently invalidate whole L1 dcache (overkill)
		 */
		__asm__ volatile("sys #0,c11,c0,#2, xzr");
		// inval (wbi for sbe) L2 DCACHE
		for (dtg = 0; dtg < 2; dtg++) {
			param = (ccu << 25) |
				(b << 24) |
				(dtg << 23) |
				(way << 18) |
				(idx << 8);
			if (tad.s.dbe)
				// inv idx
				__asm__ volatile("sys #0,c11,c1,#1, %0"
						 : : "r"(param));
			else
				// wbi idx
				__asm__ volatile("sys #0,c11,c1,#2, %0"
						 : : "r"(param));
		}
	}
	xbf.u = octeontx_read64(CAVM_CCUX_TADX_XBF_ERR(ccu, b));
	if (xbf.s.sbe || xbf.s.dbe) {
		// xbf.s.addr is PA after tad-set aliasing, no scrub needed
		if (!quiet)
			printf("CCU%d_TAD%d_XBF_ERR %llx\n", ccu, b, xbf.u);
		octeontx_write64(CAVM_CCUX_TADX_XBF_ERR(ccu, b), xbf.u);
	}
}

static void ccu_read_errs(union cavm_mdc_ecc_status mes, int quiet)
{
	int ccu, b;

	for (ccu = 0; ccu < MAX_CCU; ccu++)
		for (b = 0; b < 2; b++)
			ccu_read_err(ccu, b, mes, quiet);
}

static void check_lmc_ras(void)
{
	int lmc_no;

	debug2ras("MDC_INT_W1C r %llx\n", CSR_READ(CAVM_MDC_INT_W1C));
	debug2ras("MDC_ECC_STATUS r %llx\n", CSR_READ(CAVM_MDC_ECC_STATUS));

	for (lmc_no = 0; lmc_no < MAX_LMC; lmc_no++) {
		union cavm_lmcx_ras_err00status stat;

		stat.u = CSR_READ(CAVM_LMCX_RAS_ERR00STATUS(lmc_no));

		if (!stat.s.v)
			continue;

#define PR(csr)	debug_ras("%d " #csr " r %llx\n", \
			lmc_no, CSR_READ(csr(lmc_no)))
		PR(CAVM_LMCX_RAS_ERR00STATUS);
		PR(CAVM_LMCX_RAS_ERR00FR);
		PR(CAVM_LMCX_RAS_ERR00ADDR);
		PR(CAVM_LMCX_RAS_ERR00CTLR);
		PR(CAVM_LMCX_RAS_ERR00MISC0);
		PR(CAVM_LMCX_RAS_ERR00MISC1);
#undef PR
		arm_err_nn(1, CAVM_LMCX_RAS_ERR00, lmc_no);
	}
}

static int check_cn9xxx_mdc(union cavm_mdc_ecc_status st, int quiet)
{
	const char *unit = "";
	const char *type = NULL;
	const char *repeat = "";
	union mdc_win_cmd cmd = { .u = 0 };
	union mdc_win_dat dat;

	if (!quiet)
		INFO("%s: ecc_status:%llx chn %d.%d.%d row %d"
#if RAS_DEBUG
			" #%d"
#endif
			" db:%d%s sb%d%s\n",
			__func__, st.u,
			st.s.chain_id, st.s.hub_id, st.s.node_id, st.s.row,
#if RAS_DEBUG
			mdc_errs,
#endif
			st.s.dbe, "+" + !st.s.dbe_plus,
			st.s.sbe, "+" + !st.s.sbe_plus);

	if (st.s.dbe) {
		type = "double";
		if (st.s.dbe_plus)
			repeat = "multiple ";
		// invalidate whole dcache (overkill)
		__asm__ volatile("sys #0,c11,c0,#2, xzr");
	} else if (st.s.sbe) {
		type = "single";
		if (st.s.sbe_plus)
			repeat = "multiple ";
		sys_wbillci(-1, st.s.row);
	}

	cmd.s.chain_id = st.s.chain_id;
	cmd.s.hub_id = st.s.hub_id;
	cmd.s.node_id = st.s.node_id;
	cmd.s.csr_id = MDN_ECC_IRQ;
	dat = mdn_rd(cmd);

	ccu_read_errs(st, quiet);

	/* ack IRQs */
	if (dat.s.data) {
		static int bist_once;

		debug2ras("MDN_ECC_IRQ:%x c:%d h:%d n:%d\n",
			dat.s.data,
			cmd.s.chain_id,
			cmd.s.hub_id,
			cmd.s.node_id);
		cmd.s.csr_id = MDN_BIST_CONFIG;
		dat = mdn_rd(cmd);
		if (dat.s.data != 0xf0 && !bist_once) {
			debug2ras("MDN_BIST_CONFIG %llx"
				", k:%x a:%x c:%x hlt:%x bc:%x\n",
				dat.u,
				(dat.s.data >> 13) & 0x3,
				(dat.s.data >> 11) & 0x3,
				(dat.s.data >> 10) & 0x1,
				(dat.s.data >> 9) & 0x1,
				(dat.s.data >> 8) & 0x1);
			dat.s.data = 0xf0;
			mdn_wr(cmd, dat);
			bist_once++;
		}
	}

	if (!quiet) {
		union mdc_ras_entry_s re = mdc_read_ras_entry(
				cmd.s.chain_id, cmd.s.hub_id, cmd.s.node_id);
		const char *blk_type = ras_serr_str[re.s.ras_serr];

		printf("(chn %d.%d.%d block:%02x:%s id:%08x %d:%c %c %c)\n",
			cmd.s.chain_id, cmd.s.hub_id, cmd.s.node_id,
			re.s.ras_serr, blk_type ?: "-",
			re.s.ras_id,
			re.s.ras_uet, "UHRSN---"[re.s.ras_uet],
			"pt"[re.s.ras_transient],
			"-dc?"[re.s.ras_poison]);
	}

	/* if ECC-injection was enabled, disable it */
	cmd.s.csr_id = MDN_ECC_CONFIG;
	dat = mdn_rd(cmd);
	if (dat.s.data & 0x6) {
		dat.s.data &= ~0x6;
		mdn_wr(cmd, dat);
		debug2ras("disabled ECC injection by %d.%d.%d\n",
			cmd.s.chain_id,
			cmd.s.hub_id,
			cmd.s.node_id);
	}

	/* ideally, scrub cacheline with CACHE_WBI_L2_INDEXED(encoded)
	 * but the mdc_ecc_status does not give us the set/way/rtg needed
	 * so scrub all
	 */
	dcsw_op_all(DCCISW);
	l2c_flush();
	__asm__ volatile("ic iallu");
	dsbsy();
	isb();

	if (type && !quiet)
		debug_ras("cleared ECC %s%s bit %sfail, chn %d.%d.%d Row:%d\n",
		      repeat, type, unit,
		      st.s.chain_id,
		      st.s.hub_id,
		      st.s.node_id,
		      st.s.row);

	return !!type;
}

#if RAS_EXTENSION
int otx2_mdc_probe(const struct err_record_info *info, int *probe_data)
{
	return CSR_READ(CAVM_MDC_INT_W1C) ? 1 : 0;
}
#endif /* RAS_EXTENSION */

uint64_t otx2_mdc_isr(uint32_t id, uint32_t flags, void *cookie)
{
	union cavm_mdc_ecc_status st;
	uint64_t mdc_int = CSR_READ(CAVM_MDC_INT_W1C);
	int burst = 0;
	int quiet = 0;

	if (!~id) {
		/* id ~0 clears BIST errors on startup */
		burst = 1000;
		quiet = 1;
	} else {
		/* holler when we see actual IRQs, but rate-limited */
		static int seen;

		if (id && ++seen && !(seen & (seen - 1)))
			debug_ras("%s(%x) st:%llx #%d\n", __func__, id,
				CSR_READ(CAVM_MDC_ECC_STATUS), seen);
		if (!mdc_int)
			return 0;
	}

	check_lmc_ras();

	do {
		CSR_WRITE(CAVM_MDC_INT_W1C, 1);
		st.u = CSR_READ(CAVM_MDC_ECC_STATUS);
		if (!st.u)
			break;
		quiet |= mdc_dup(st.u);
	} while (check_cn9xxx_mdc(st, quiet) && --burst > 0);

	return 0;
}

#if RAS_EXTENSION
int otx2_mcc_probe(const struct err_record_info *info, int *probe_data)
{
	union cavm_mccx_lmcoex_ras_int lmcoe_ras_int;
	union cavm_mccx_const mcc_const;
	int mcc, lmcoe;
	int num_mccs = plat_octeontx_get_mcc_count();

	for (mcc = 0; mcc < num_mccs; mcc++) {
		mcc_const.u = CSR_READ(CAVM_MCCX_CONST(mcc));
		for (lmcoe = 0; lmcoe < mcc_const.s.lmcs; lmcoe++) {
			lmcoe_ras_int.u = CSR_READ(
					CAVM_MCCX_LMCOEX_RAS_INT(mcc, lmcoe));
			if (lmcoe_ras_int.u) {
				lmcoe |= (mcc << 8);
				*probe_data = lmcoe;
				return 1;
			}
		}
	}

	return 0;
}
#endif /* RAS_EXTENSION */

uint64_t otx2_mcc_isr(uint32_t id, uint32_t flags, void *cookie)
{
	union cavm_mccx_const mcc_const;
	int mcc, lmcoe;
	int num_mccs = plat_octeontx_get_mcc_count();
	int fatal = 0;
	static int seen; /* announce first IRQ */

	if (id && !seen && ++seen)
		debug_ras("%s(%x)\n", __func__, id);

	for (mcc = 0; mcc < num_mccs; mcc++) {
		mcc_const.u = CSR_READ(CAVM_MCCX_CONST(mcc));
		for (lmcoe = 0; lmcoe < mcc_const.s.lmcs ; lmcoe++)
			fatal += lmcoe_ras_check_ecc_errors(mcc, lmcoe);
	}

	if (fatal)
		handle_fatal();

	return 0;
}

int lmcoe_scrubber_setup(int mcc, int lmcoe)
{
	union cavm_mccx_lmcoex_bscrub_cfg cfg;
	union cavm_mccx_lmcoex_bscrub_cfg2 cfg2;
	union cavm_ccs_asc_regionx_start r_start;
	union cavm_ccs_asc_regionx_end r_end;
	uint64_t a_start, a_end;

	cfg.u = CSR_READ(CAVM_MCCX_LMCOEX_BSCRUB_CFG(mcc, lmcoe));
	cfg2.u = CSR_READ(CAVM_MCCX_LMCOEX_BSCRUB_CFG2(mcc, lmcoe));

	/*
	 * FIXME: configure scrubber region more generally.
	 * This just assumes existing bdk/atf practice of
	 * REGION0 covering low DRAM, REGION1 high DRAM
	 */
	r_start.u = CSR_READ(CAVM_CCS_ASC_REGIONX_START(0));
	r_end.u = CSR_READ(CAVM_CCS_ASC_REGIONX_END(1));
	a_start = r_start.s.addr << 24;
	a_end = r_end.s.addr << 24;

	debug_ras("bscrub range was %llx-%llx, want %llx-%llx\n",
		(uint64_t)cfg.s.start_address, (uint64_t)cfg2.s.stop_address,
		a_start, a_end);

	/*
	 * background-scrubber idle count, currently default from HRM,
	 * perhaps should scale to SCLK, with later adjustment via SMC
	 */
	cfg.s.bs_idle_cnt = 0x4f;

	if (cfg.s.enable || cfg.s.busy) {
		ERROR("%s(%d,%d) sees en:%d busy:%d scrubber\n",
			__func__, mcc, lmcoe,
			cfg.s.enable, cfg.s.busy);
		return -1;
	}

	cfg.s.start_address = a_start;
	cfg2.s.stop_address = a_end;

	CSR_WRITE(CAVM_MCCX_LMCOEX_BSCRUB_CFG2(mcc, lmcoe), cfg2.u);
	cfg.s.enable = 1;
	CSR_WRITE(CAVM_MCCX_LMCOEX_BSCRUB_CFG(mcc, lmcoe), cfg.u);

	return 0;
}

static int ras_init_mcc(int mcc)
{
	union cavm_mccx_const mc;
	int lmcoe;

	debug_ras("%s(%d)\n", __func__, mcc);

	mc.u = CSR_READ(CAVM_MCCX_CONST(mcc));

	/* t96-asim-specific */
	if (!mc.s.lmcs && is_asim())
		mc.s.lmcs = mcc ? 2 : 1;

	for (lmcoe = 0; lmcoe < mc.s.lmcs; lmcoe++) {
		lmcoe_ras_setup(mcc, lmcoe);
		lmcoe_scrubber_setup(mcc, lmcoe);
	}

	debug_ras("%s done, %d LMCs\n", __func__, mc.s.lmcs);

	return 0;
}

static void ras_log_regions(void)
{
	int r;
	union cavm_ccs_asc_regionx_start r_start;
	union cavm_ccs_asc_regionx_attr r_attr;
	union cavm_ccs_asc_regionx_end r_end;
	union cavm_ccs_asc_regionx_offset r_off;
	uint64_t a_start, a_end;

	for (r = 0; r < 16; r++) {
		r_start.u = CSR_READ(CAVM_CCS_ASC_REGIONX_START(r));
		r_attr.u = CSR_READ(CAVM_CCS_ASC_REGIONX_ATTR(r));
		r_off.u = CSR_READ(CAVM_CCS_ASC_REGIONX_OFFSET(r));
		r_end.u = CSR_READ(CAVM_CCS_ASC_REGIONX_END(r));
		a_start = r_start.s.addr << 24;
		a_end = r_end.s.addr << 24;

		if (!(r_start.u | r_attr.u | r_off.u | r_end.u))
			continue;
		debug_ras("ASC.R%d %llx..%llx o:%x wt%d"
			" lmc(mo:%x ma:%x) s%d ns%d\n",
			r, a_start, a_end,
			(int)r_off.s.offset,
			(int)r_attr.s.write_through,
			(int)r_attr.s.lmc_mode,
			(int)r_attr.s.lmc_mask,
			(int)r_attr.s.s_en,
			(int)r_attr.s.ns_en);
	}
}

static int ras_init_mccs(void)
{
	uint64_t vaddr = CAVM_MDC_PF_MSIX_VECX_ADDR(0);
	uint64_t vctl = CAVM_MDC_PF_MSIX_VECX_CTL(0);
#if !RAS_EXTENSION
	int rc = 0;
#endif /* !RAS_EXTENSION */
	int irq = MDC_SPI_IRQ(0);
	int mcc;
	int num_mccs = plat_octeontx_get_mcc_count();
	uint64_t ctl = CAVM_GICD_SETSPI_SR | 1;
	int ccu;

	debug_ras("%s: %d MCCs\n", __func__, num_mccs);

	ras_log_regions();
	for (mcc = 0; mcc < num_mccs; mcc++)
		ras_init_mcc(mcc);

	CSR_WRITE(CAVM_MDC_INT_W1C, ~0ULL);
	CSR_WRITE(CAVM_MDC_INT_ENA_W1C, ~0ULL);

#if !RAS_EXTENSION
	rc = octeontx_ehf_register_irq_handler(irq, otx2_mdc_isr);
	if (rc) {
		INFO("e?%d otx2_mdc_isr(%x)\n", rc, irq);
		return rc;
	}
#endif /* !RAS_EXTENSION */

	/* Workaround for [stream] security issue; use non-secure register */
	/* configure non-secure register to allow setting of secure SPI */
	{
		int reg, spi_shift;

		/* there are 16 2-bit SPI fields in each NSACRX reg */
		reg = irq / 16;              /* 16 SPIs per reg */
		spi_shift = (irq % 16) * 2;  /* 2 bits per SPI */

		/* set value of 01 to permit "set pending" */
		CSR_MODIFY(c, CAVM_GICD_NSACRX(reg),
			   c.s.vec &= ~(3 << spi_shift);
			   c.s.vec |= 1 << spi_shift);
	}
	octeontx_write64(vctl, irq);
	octeontx_write64(vaddr, CAVM_GICD_SETSPI_NSR | 1);

	CSR_WRITE(CAVM_MDC_INT_ENA_W1S, 1ULL);

	/* clear any err signatures left from BIST */
	otx2_mdc_isr(~0, 0, NULL);

	for (ccu = 0; ccu < MAX_CCU; ccu++) {
		uint64_t b, r;

		for (b = 0; b < 2; b++) {
			/* clear BIST noise */
			r = CAVM_CCUX_PIC_MTDX_ERR(ccu, b);
			octeontx_write64(r,
				octeontx_read64(r));
			r = CAVM_CCUX_PIC_TAGX_ERR(ccu, b);
			octeontx_write64(r,
				octeontx_read64(r));
			r = CAVM_CCUX_TADX_DAT_ERR(ccu, b);
			octeontx_write64(r,
				octeontx_read64(r));
			r = CAVM_CCUX_TADX_XBF_ERR(ccu, b);
			octeontx_write64(r,
				octeontx_read64(r));

			/* register NXM IRQs */
			r = CAVM_CCUX_TADX_INT_W1C(ccu, b);
			octeontx_write64(r, ~0ull);
			r = CAVM_CCUX_TADX_INT_ENA_W1C(ccu, b);
			octeontx_write64(r, ~0ull);

			debug_ras("Registering CCU%d_TAD%lld irq handlers\n",
				ccu, b);
			vaddr = CAVM_CCUX_MSIX_VECX_ADDR(ccu, b);
			vctl = CAVM_CCUX_MSIX_VECX_CTL(ccu, b);
			octeontx_write64(vaddr, ctl);
			octeontx_write64(vctl, irq);
			r = CAVM_CCUX_TADX_INT_ENA_W1S(ccu, b);
			octeontx_write64(r, ~0ull);
		}
	}

	return 0;
}

int plat_dram_ras_init(void)
{
	uint64_t midr = read_midr();

	debug_ras("%s\n", __func__);

	if (!(IS_OCTEONTX_PN(midr, T96PARTNUM) ||
	      IS_OCTEONTX_PN(midr, F95PARTNUM) ||
	      IS_OCTEONTX_PN(midr, LOKIPARTNUM) ||
	      IS_OCTEONTX_PN(midr, T98PARTNUM))) {
		ERROR("%s: unsupported device\n", __func__);
		return 0;
	}

	ras_init_mccs();
	debug_ras("RAS handlers registered\n");
	return 0;
}

/* smc_ras support ... */

static union mdc_const mc;	/* cached geometry */
static int ce, he, ne, re;	/* endpoints */
static uint64_t rsp_last = ~0;	/* filters out repeat values */

int64_t plat_ras_mdc_rom(u_register_t addr)
{
	return CSR_READ(CAVM_MDC_RAS_ROMX(addr));
}

int64_t plat_ras_mdc_limits(void)
{
	union mdc_win_cmd lim;

	if (!mc.u) {
		mc.u = CSR_READ(CAVM_MDC_CONST);
		ce = mc.s.max_chain_id;
		he = mc.s.max_hub_id;
		ne = mc.s.max_node_id;

		if (IS_OCTEONTX_PN(read_midr(), T98PARTNUM))
			re = 0x100 + (31 << 2);
		else
			re = 0x80 + (31 << 2);
	}

	lim.u = 0;
	lim.s.csr_id = re;
	lim.s.node_id = ne;
	lim.s.hub_id = he;
	lim.s.chain_id = ce;

	return lim.u;
}

int64_t plat_ras_mdc_rw(u_register_t x2, u_register_t x3, u_register_t x4)
{
	union mdc_win_cmd addr = { .u = x2 };
	union mdc_win_cmd cmd = {.u = 0 };
	union mdc_win_dat dat = {.u = 0 };
	union mdc_win_dat rsp;

	int c = addr.s.chain_id;
	int h = addr.s.hub_id;
	int n = addr.s.node_id;
	int r = addr.s.csr_id;
	int w = addr.s.we;
	uint32_t val = x3;
	uint32_t mask = x4;

	plat_ras_mdc_limits();

	if (c > ce || h > he || n > ne || r > re)
		return -EINVAL;
	if (r >= 0 && (r & 3))
		return -EINVAL;

	cmd.s.chain_id = c;
	cmd.s.hub_id = h;
	cmd.s.node_id = n;
	cmd.s.csr_id = r;

	if (!w) {
		rsp = mdn_rd(cmd);
		if (rsp.u != rsp_last)
			printf("%d.%d.%d.%x %llx\n", c, h, n, r, rsp.u);
		rsp_last = rsp.u;
		return rsp.u;
	}

	/* restrict what can be written */
	switch (r) {
	case MDN_ECC_CONFIG:
		/* allow double-bit injection in debug build */
		mask &= 6;
#if !DEBUG
		/* only single-bit injection in normal build */
		switch (val & mask) {
		case 0:
		case 2:
		case 4:
			break;
		default:
			return -EACCES;
		}
#endif
		break;
	default:
		mask = 0;
		break;
	}
	if (!mask)
		return -EACCES;
	dat = mdn_rd(cmd);
	dat.s.data &= ~mask;
	dat.s.data |= (val & mask);
	printf("chn %d.%d.%d.%x := %x\n",
	       c, h, n, r, dat.s.data);
	return mdn_wr(cmd, dat);
}
