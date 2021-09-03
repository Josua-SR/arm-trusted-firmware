/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __CGX_H__
#define __CGX_H__

#include <platform_dt.h>

/* max channels per lmac */
#define MAX_CHAN_PER_LMAC	16
#define MAX_GSERX_PER_CGX	2

/* REF values from Octeon TX */

/* Max Packet Size */
#define CGX_MAX_FRAME_SIZE		9216
#define CGX_FCS_BYTES			4
#define CGX_PREAMBLE_BYTES		8
#define CGX_JABBER_ALIGN		16

/* SGMII types */
/* Tx Threshold */
#define CGX_GMP_TX_FIFO_WORDS		0x30

/* Min Packet Size */
#define CGX_GMP_MIN_PACKET_SIZE         (60 - 1)

/* XAUI types */
/* Tx Threshold */
#define CGX_SMUX_TX_FIFO_WORDS		(0x100 - 1)

/* Min Packet Size */
#define CGX_SMUX_MIN_PACKET_SIZE         (60 + 4)

/* Recommended back pressure buffer depth to be 1/4th of FIFO size */
#define CGX_BP_ON_MARK_SIZE_DIV		4

/* Packet data depth is 128-bit and mark to be configured in multiple of 16 bytes */
#define CGX_BP_PACKET_DATA_DEPTH	16

/* software to wait 10 ms to establish stable link */
#define CGX_SPUX_BR_RCV_LINK_DELAY 10000

/* MAX USXGMII types supported on CN9XXX */
#define MAX_USXGMII_TYPES (CAVM_CGX_USXGMII_TYPE_E_QXGMII_10G + 1)

/* MAX USXGMII rate types supported on CN9XXX */
#define MAX_USXGMII_RATE_TYPES (CAVM_CGX_USXGMII_RATE_E_RSV_RATE + 1)

/* TIMEOUT for different usecases */
#define CGX_POLL_AN_STATUS		10000 /* 10 ms */
#define CGX_POLL_AN_COMPLETE_STATUS	500000 /* 500 ms */
#define CGX_POLL_AN_COMPLETE_STATUS2	3000000 /* 3 s - MCP AN */
#define CGX_POLL_AN_PAGE_STATUS		300000	/* 300 ms */
#define CGX_POLL_AN_RESTART_STATUS	80000	/* 80 ms */
#define CGX_POLL_AN_RX_SIGNAL_LONG	10000	/* 10 ms */
#define CGX_POLL_AN_RX_SIGNAL_SHORT	500	/* 500 us */
#define CGX_POLL_AN_RX_SIGNAL2		200000	/* 100 ms - MCP AN*/
#define CGX_POLL_TRAINING_STATUS	600000	/* 500 ms */
#define GSERN_LANEX_TX_RST_SM_TIMEOUT   10000   /* 10 ms */
#define REMOTE_FAULT_TIMEOUT_MS		10000
#define TX_IDLE_TOGGLE_US		1000
#define CGX_TOTAL_LINK_TIMEOUT_MS       4000    /* 4 s */
#define CGX_NO_RX_SIG_FAIL_TIMEOUT_MS   200     /* 200 ms */

/* Bit mask to clear error interrupt bits set in
 * SPUX_INT CSR
 * spux_int.s.rx_link_down
 * spux_int.s.err_blk
 * spux_int.s.bitlckls
 * spux_int.s.synlos
 * spux_int.s.algnlos
 * spux_int.s.dbg_sync
 * spux_int.s.bip_err
 * spux_int.s.fec_corr
 * spux_int.s.fec_uncorr
 * spux_int.s.training_failure
 * spux_int.s.rsfec_corr
 * spux_int.s.rsfec_uncorr
 * spux_int.s.hi_ser
 */
#define CGX_SPUX_CLEAR_ERR_INT_MASK 0x743FE

/* Max retries for different failures */
#define PHY_FAIL_RETRIES 5
/* MCP AN/LT failure counts */
#define MCP_LINK_FAIL_MAX 4
#define MCP_LINK_LT_FAIL_MAX 4

/* Maximum number of extended next page prints */
#define AN_NP_PRINT_MAX 20

/* Read-Modify-Write APIs for CGX CSRs */
#define CAVM_MODIFY_CGX_CSR(type, csr, field, val)        \
	do {                                                    \
		type c;                                         \
		c.u = CSR_READ(csr);			\
		c.s.field = val;				\
		CSR_WRITE(csr, c.u);			\
	} while (0)

/* Macros to define BIT masks for polling */
#define CGX_GMP_TX_IDLE_MASK		1ULL << 13
#define CGX_GMP_RX_IDLE_MASK		1ULL << 12
#define CGX_GMP_PCS_RESET_MASK		1ULL << 15
#define CGX_GMP_PCS_LNK_ST_MASK		1ULL << 2
#define CGX_GMP_PCS_RXX_SYNC_MASK	1ULL << 1
#define CGX_GMP_PCS_AN_CPT_MASK		1ULL << 5
#define CGX_CMRX_RX_FIFO_LEN_MASK	1ULL << 14
#define CGX_CMRX_TX_IDLE_MASK		1ULL << 14
#define CGX_SPUX_RESET_MASK		1ULL << 15
#define CGX_SPUX_BLK_LOCK_MASK		1ULL
#define CGX_SPUX_RX_ALIGN_MASK		1ULL << 12
#define CGX_SMUX_RX_STATUS_MASK		((1ULL << 0) | (1ULL < 1))	/* Bit 0 & 1 */
#define CGX_SMUX_TX_IDLE_MASK		1ULL << 1
#define CGX_SMUX_RX_IDLE_MASK		1ULL
#define CGX_SMUX_PCS_RCV_LINK_MASK	1ULL << 1
#define CGX_SPUX_AN_RESET_MASK		1ULL << 15
#define CGX_SPUX_AN_RESTART_MASK	1ULL << 9
#define CGX_SPUX_USX_AN_RESET_MASK	1ULL << 15
#define CGX_SPUX_AN_CMP_MASK		1ULL << 12
#define CGX_SPUX_AN_LNK_MASK		1ULL << 11
#define CGX_SPUX_USX_AN_LNK_MASK	1ULL << 19
#define CGX_SPUX_RSFEC_ALGN_STS_MASK	1ULL << 8
#define CGX_SPUX_MARKER_LOCK_MASK	1ULL << 41
#define CGX_SPUX_AN_CPT_MASK		1ULL << 5
#define CGX_SPUX_TRAINING_DONE_MASK	1ULL << 13
#define CGX_SPUX_TRAINING_FAIL_MASK	1ULL << 14
#define CGX_SPUX_AN_RX_PAGE_MASK	1ULL << 10
#define GSERN_LANEX_TX_RST_SM_COMPLETE_MASK	1ULL << 33

/* GSERN macros to define BIT masks for polling */
#define GSERN_RX_IDLEDET_MASK	1ULL << 0
#define GSERN_RX_DEEPIDLE_MASK	1ULL << 26

/* ERROR MASK based on cgx_err_type */
#define CGX_ERR_MASK			0x3FF		/* 10 bits */

/* Default value of NETWORK-LANE-ORDER.N0.CGX%d property */
#define CGX_DEFAULT_NETWORK_LANE_ORDER	0x3210

/* Return codes used by cgx display eye and serdes settings */
#define CGX_DISPLAY_OK			0
#define CGX_DISPLAY_PENDING		1

enum cgx_prbs_cmd {
	CGX_PRBS_START_CMD = 1,
	CGX_PRBS_STOP_CMD,
	CGX_PRBS_GET_DATA_CMD,
	CGX_PRBS_CLEAR_CMD
};

/* enum declaration for FEC */
typedef enum cgx_fec_type {
	CGX_FEC_NONE = 0,
	CGX_FEC_BASE_R,
	CGX_FEC_RS
} fec_type_t;

/* structure declarations */
typedef union cgx_link_status {
	uint64_t u64;
	struct cgx_link_status_s {
		uint64_t link_up:1;
		uint64_t full_duplex:1;
		uint64_t speed:4;		/* cgx_link_speed enum */
		/* current AN state:1 */
		uint64_t an:1;
		/* current FEC type:2 */
		uint64_t fec:2;
		uint64_t reserved:55;
	} s;
} link_state_t;

/* USXGMII sub type/rate mapping */
typedef struct usxgmii_type {
	int baud_rate;
	int lmacs_used;
	int type;
	int max_rate;	/* max rate allowed on a sub type */
} usxgmii_type_t;

typedef struct usxgmii_rate_map {
	int speed;
	int rate;
	int speed_mbps;
} usxgmii_rate_map_t;

typedef struct cgx_lmac_timers {
	uint64_t remote_fault_timeout;
} cgx_lmac_timers_t;

/* this structure will be used to maintain the current
 * link status and also lock mechanism to prevent simultaneous
 * access of CSRs by timer #1 and timer #2 CBs. as, SCRATCHX CSRs
 * have been modified to be opaque, it is not reliable to save
 * the current link status
 */
typedef union cgx_lmac_context {
	uint64_t u64;
	struct cgx_lmac_context_s {
		/* Timer #1 and Timer #2 CB should set lock = 1 (if lock == 0)
		 * before writing to SCRATCHX CSRs and free it after writing
		 * In addition to this lock, ownership status of SCRATCHX(1)
		 * should be checked to make sure kernel is not writing to it
		 */
		uint64_t lock:1;
		/* this variable will be used by timer #2 to poll for the
		 * link periodically as long as this variable is set. updated
		 * by timer #1 callback when there is a LINK UP/DOWN request
		 */
		uint64_t link_enable:1;
		/* this variable will be used by timer #2 to know if there
		 * was a LINK UP request sent but the link was not brought up
		 */
		uint64_t link_req:1;
		/* internal loopback enable/disable */
		uint64_t lbk1_enable:1;
		/* link status */
		uint64_t link_up:1;
		uint64_t full_duplex:1;
		/* cgx_link_speed */
		uint64_t speed:4;
		/* cgx_error_type */
		uint64_t error_type:10;
		/* link change request */
		uint64_t link_change_req:1;
		/* current AN state:1 */
		uint64_t an:1;
		/* current FEC type:2 */
		uint64_t fec:2;
		/* init link state */
		uint64_t init_link:1;
		/* prev module status:2 */
		uint64_t mod_stats:2;
		/* Used to track the Rx link status. */
		uint64_t rx_link_up:1;
		/* Used to track if a remote fault is detected. */
		uint64_t remote_fault:1;
		uint64_t reserved:35;
	} s;
} cgx_lmac_context_t;

/* Flash handling for lmac params */
typedef union cgx_lmac_flash_ctx {
	uint64_t u64;
	struct cgx_lmac_flash_ctx_s {
		uint64_t status:2; /* valid 0x2, other values invalid */
		uint64_t cgx_id:3;
		uint64_t lmac_id:3;
		uint64_t qlm_mode:8;
		uint64_t fec_invalid:1;
		uint64_t fec_type:2;
		uint64_t mod_invalid:1;
		uint64_t mod_type:1;
		uint64_t rsvd1:3;
		uint64_t lmac_mode:8;
		uint64_t ignore:1;
		uint64_t rsvd2:31;
	} s;
} cgx_lmac_flash_ctx_t;

/* Mapping of speed and other parameters to MODE */
typedef struct cgx_speed_mode_map_s {
	int lmac_mode;
	int train_en;
	int qlm_mode;
	int fec_cap;
	int baud_rate;
	uint64_t mode_bitmask;
} cgx_speed_mode_map;

typedef struct cgx_prbs_errors {
	uint64_t err;
	uint64_t phy_host;
	uint64_t phy_line;
} cgx_prbs_errors_t;

typedef struct cgx_prbs_data_s {
	uint64_t num_lanes;
	cgx_prbs_errors_t errors[MAX_LMAC_PER_CGX];
} cgx_prbs_data;

extern int spi_config(uint64_t spi_clk, uint32_t mode, int cpol, int cpha,
		      int spi_con, int cs);
extern int spi_nor_read(uint8_t *buf, int buf_size, uint32_t addr,
			int addr_len, int spi_con, int cs);
extern int spi_nor_write(uint8_t *buf, int buf_size, uint32_t addr,
			int addr_len, int spi_con, int cs);
extern int spi_nor_erase(uint32_t addr, int addr_len, int spi_con, int cs);

int cgx_update_flash_fec_param(int cgx_id, int lmac_id, int fec);
int cgx_update_flash_phy_mod_param(int cgx_id, int lmac_id, int phy_mod);
int cgx_update_flash_mode_param(int cgx_id, int lmac_id, int lmac_mode);
int cgx_update_flash_ignore_param(int cgx_id, int lmac_id, int ignore);


/* CGX driver APIs */
void cgx_hw_init(int cgx_id);
int cgx_sgmii_set_link_speed(int cgx_id, int lmac_id,
			link_state_t *link);
int cgx_sgmii_set_link_up(int cgx_id, int lmac_id);
int cgx_sgmii_set_link_down(int cgx_id, int lmac_id);

int cgx_xaui_init_link(int cgx_id, int lmac_id);
int cgx_xaui_set_link_up(int cgx_id, int lmac_id, cgx_lmac_context_t *lmac_ctx);
int cgx_xaui_set_link_down(int cgx_id, int lmac_id);
int cgx_xaui_get_link(int cgx_id, int lmac_id,
		link_state_t *result, cgx_lmac_context_t *lmac_ctx,
		cgx_lmac_timers_t *lmac_tmr);
void cgx_set_internal_loopback(int cgx_id, int lmac_id, int enable);
void cgx_set_external_loopback(int cgx_id, int lmac_id, int enable);
void cgx_set_error_type(int cgx_id, int lmac_id, uint64_t type);
void cgx_get_link_state(int cgx_id, int lmac_id, link_state_t *link);
void cgx_lmac_init_link(int cgx_id, int lmac_id);
int cgx_sgmii_check_link(int cgx_id, int lmac_id);
int cgx_sgmii_check_an_cpt(int cgx_id, int lmac_id);
int cgx_fec_change(int cgx_id, int lmac_id, int new_fec);
int cgx_get_supported_fec_type(int cgx_id, int lmac_id);
int cgx_validate_fec_config(int cgx_id, int lmac_id, int req_fec);
int cgx_get_lane_speed(int cgx_id, int lmac_id);
void cgx_lmac_init(int cgx_id, int lmac_id);
void cgx_set_an_loopback(int cgx_id, int lmac_id, int enable);
int cgx_gserx_mode_chg(int cgx_id, int lmac_id, int baud_mhz, int anlt_fec_only_chg);

/* CGX FW interface APIs */
void cgx_fw_intf_init(void);
void cgx_fw_intf_shutdown(void);

int cgx_read_flash_fec(int cgx_id, int lmac_id, int *fec);
int cgx_set_fec_type(int cgx_id, int lmac_id, int req_fec);
int cgx_do_cmu_reset(int cgx_id, int lmac_id, int cgx_to_reset);
int cgx_read_flash_ignore(int cgx_id, int lmac_id, int *ignore);
int cgx_read_flash_mode_param(int cgx_id, int lmac_id, int *qlm_mode,
			      int *lmac_mode);
int cgx_get_lane_count(int lmac_type);
int qlm_get_baud_rate_for_mode(int qlm_mode);

int cgx_display_eye(int qlm, int qlm_lane, int show_data);
int cgx_display_serdes_settings(int qlm, int qlm_lane, int show_data);
/*
 * Meaning of x3 argument depends on cmd. Look for
 * PLAT_OCTEONTX_SERDES_DBG_PRBS smc description.
 */
int cgx_smc_set_serdes_tune(int cmd, int qlm, int x3, int x4);
int cgx_smc_set_serdes_loop(int cmd, int qlm, int x3);
int cgx_smc_do_prbs(int cmd, int qlm, int x3, int x4);

/* plat APIs specific to Octeon TX2 family */
int plat_get_cgx_idx(int qlm);

int cgx_set_ptp_mode(int cgx_id, int lmac_id, int enable);
void cgx_set_serdes_loop(int cgx_id, int lmac_id, int type);
void cgx_set_serdes_tune(int cgx_id, int tx_swing, int tx_pre, int tx_post, int lane_mask);

void cgx_set_serdes_rx_leq_adaptation(int cgx_id, int lmac_id, int disable,
	int leq_lfg_start, int leq_hfg_sql_start, int leq_mbf_start,
	int leq_mbg_start, int gn_apg_start);
void cgx_set_serdes_rx_dfe_adaptation(int cgx_id, int lmac_id, int disable);
void cgx_gpio_led_handle(int cgx_id, int lmac_id, uint64_t link_up);
int cgx_serdes_tx_control(int cgx_id, int lmac_id, bool enable_tx);

#endif
