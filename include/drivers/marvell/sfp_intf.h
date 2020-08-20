/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef __SFP_INTF_H__
#define __SFP_INTF_H__

#define MCP_ANLT_INTF_VER 0xABCD1235
#define SFP_MAX_EEPROM_SIZE    0x100

/* Type of GPIO pin */
typedef enum {
	GPIO_PIN_NONE = 0,
	/* GPIO pin is directly connected to SoC */
	GPIO_PIN_DEFAULT,
	/* GPIO pin is PCA953X expander type (multiple expanders use this) */
	GPIO_PIN_PCA953X,
	/* GPIO pin is PCA957X expander type */
	GPIO_PIN_PCA957X,
	/* GPIO pin is PCF857X expander type */
	GPIO_PIN_PCF857X,
	/* 40 pin expander type */
	GPIO_PIN_PCA9698,
	/* GPIO pin is PCA9552 LED driver */
	GPIO_PIN_PCA9552,
	/* Pin is controlled via CPLD(similar to GPIO controller) */
	GPIO_PIN_CPLD,
	GPIO_PIN_OTHER,
} gpio_type_e;

typedef enum i2c_mux_type {
	I2C_SWITCH,
	I2C_MUX,
	I2C_OTHER
} i2c_mux_type_e;

/* Type of i2c bus */
typedef enum {
	I2C_BUS_NONE = 0,
	I2C_BUS_DEFAULT,    /* Directly connected to SoC */
	I2C_BUS_PCA9540,
	I2C_BUS_PCA9542,
	I2C_BUS_PCA9543,
	I2C_BUS_PCA9544,
	I2C_BUS_PCA9545,
	I2C_BUS_PCA9546,
	I2C_BUS_PCA9547,
	I2C_BUS_PCA9548,
	I2C_BUS_OTHER
} i2c_bus_type_e;

/* SFF compliance revisions */
typedef enum {
	ETH_MODULE_SFF_NONE = 0,
	ETH_MODULE_SFF_8079,
	ETH_MODULE_SFF_8472,
	ETH_MODULE_SFF_8636,
	ETH_MODULE_SFF_8436
} sff_id_e;

typedef struct gpio_compat {
	/* compatible string */
	char compatible[64];
	/* type, i.e. gpio_type_e */
	uint32_t type;
	/* max number of pins */
	uint32_t ngpios;
} gpio_compat_t;

typedef struct i2c_compat {
	/* Compatible string */
	char compatible[64];
	/* Device type, i2c_bus_type_e */
	uint32_t type;
	/* Mux or Switch, i2c_mux_type_e */
	uint32_t mux_type;
	/* Enable bit for mux. Each mux have different bit */
	uint32_t enable;
	/* Max number of channels */
	uint32_t size;
} i2c_compat_t;

typedef struct i2c_info {
	/* Address of the MUX/Switch */
	uint32_t addr;
	/* To which channel the slot is connected to */
	uint32_t channel;
	/* Enable bit of channel - Varies for MUX */
	uint32_t enable_bit;
	/* Whether it is a MUX or Switch */
	uint32_t is_mux;
	/* TWSI bus the slot is connected to */
	uint32_t bus;
	/* Type of MUX/Switch */
	uint32_t type;
} i2c_info_t;

typedef struct gpio_info {
	/* details of how the expander is connected to SoC - switch or Mux */
	i2c_info_t i2c_info;
	/* Pin Number */
	uint32_t pin;
	/* Total pins supported by the Expander */
	uint32_t num_pins;
	/* Expander's TWSI address */
	uint32_t i2c_addr;
	/* TWSI bus Expander is connected to */
	uint32_t i2c_bus;
	/* Direction of the Pin is output */
	uint32_t dir_out;
	/* Direction of the Pin is input */
	uint32_t dir_in;
	/* Type of Expander */
	uint32_t type;
	uint32_t flags;
} gpio_info_t;

/* State machine maintain for SFP/QSFP management for communication
 * between AP & MCP
 * sfp_mod_state_info : Module status
 * sfp_data_state_info : EEPROM status
 */
typedef enum sfp_mod_state_info {
	/* Module disconnected */
	SFP_MOD_STATE_ABSENT = 0,
	/* Module connected */
	SFP_MOD_STATE_PRESENT,
	/* Update of EEPROM every 5s */
	SFP_MOD_STATE_EEPROM_UPDATED,
	/* Other state */
	SFP_MOD_STATE_OTHER
} sfp_mod_state_info_t;

typedef enum sfp_data_state_info {
	SFP_DATA_STATE_IDLE = 0,
	/* MCP is updating the data
	 * (before read of EEPROM, this state to be set)
	 */
	SFP_DATA_STATE_READ_EEPROM,
	/* MCP read the eeprom data and EEPROM data is valid */
	SFP_DATA_STATE_EEPROM_VALID,
	/* MCP read the eeprom data and data is not valid.
	 * MCP should read the EEPROM again or throw error to user
	 */
	SFP_DATA_STATE_EEPROM_NVAL,
} sfp_data_state_info_t;

/* Data structure for the SFP slot info obtained from DT */
typedef struct sfp_slot_info {
	char name[16];
	/* TWSI details for EEPROM connection */
	i2c_info_t i2c_eeprom_info;
	/* TWSI details for DIAG connection (if applicable) */
	i2c_info_t i2c_diag_info;
	/* SFP : GPIO details for tx_disable pin */
	gpio_info_t tx_disable;
	/* SFP : GPIO details for mod_abs pin */
	gpio_info_t mod_abs;
	/* SFP : GPIO details for tx_fault */
	gpio_info_t tx_fault;
	/* SFP : GPIO details for rx_los */
	gpio_info_t rx_los;
	/* QSFP : GPIO details for ModSelL pin */
	gpio_info_t select;
	/* QSFP : GPIO details for ResetL pin */
	gpio_info_t reset;
	/* QSFP : GPIO details for LPMode */
	gpio_info_t lp_mode;
	/* QSFP : GPIO details for ModPrsL */
	gpio_info_t mod_prs;
	/* QSFP : GPIO details for IntL */
	gpio_info_t interrupt;
	/* set when SFP is connected */
	uint32_t is_sfp;
	/* set when QSFP is connected */
	uint32_t is_qsfp;
	/* TWSI address of EEPROM */
	uint32_t eeprom_addr;
	/* different power levels in mW */
	uint32_t max_power;
} sfp_slot_info_t;

/* Ownership of shared memory */
typedef enum sfp_own {
	SFP_OWN_NONE = 0,
	SFP_OWN_AP,
	SFP_OWN_MCP
} sfp_own_info_t;

/* Data structures to be shared between AP and MCP */
typedef struct sfp_context {
	/* transceiver state, sfp_mod_state_info_t*/
	uint32_t mod_status;
	/* transceiver data status, sfp_data_state_info_t*/
	uint32_t data_status;
	/* lock to prevent conflict of access between AP and MCP */
	uint32_t lock;
	/* SFF-8419 provides details of these pins */
	uint32_t mod_abs;        /* state of mod_abs pin for SFP */
	uint32_t tx_disable;     /* state of tx_disable pin for SFP */
	uint32_t tx_fault;       /* state of tx_fault pin for SFP */
	uint32_t rx_los;         /* state of rx_los pin for SFP */
	/* SFF-8438 provides details of these pins */
	uint32_t select;         /* state of Select pin for QSFP */
	uint32_t reset;          /* state of reset pin for QSFP */
	uint32_t lp_mode;        /* state of LPmode pin for QSFP */
	uint32_t interrupt;      /* state of Interrupt pin for QSFP */
	uint32_t mod_prs;        /* state of Mod present pin for QSFP */
} sfp_context_t;

/* FIXME: this is not complete list */
typedef enum async_req_id {
	SFP_REQ_NONE = 0,
	SFP_REQ_TX_ENABLE,    /* Turn ON transmitter */
	SFP_REQ_TX_DISABLE,    /* Turn OFF transmitter */
	SFP_REQ_SET_POWER_MODE,    /* Set to High/Low Power Mode */
	SFP_REQ_GET_POWER_MODE,    /* Get power mode status */
	SFP_REQ_WRITE_GPIO,    /* To toggle on one of SFP/QSFP GPIO pins */
	SFP_REQ_READ_GPIO,    /* To know the status of SFP/QSFP GPIO pins */
	SFP_REQ_READ_BYTE,    /* To read one of the bytes in EEPROM */
	REQ_AN_LT_START,
	REQ_AN_LT_STOP,
} async_req_id_t;

typedef union mcp_an_dbg_state {
	uint64_t u64;
	struct mcp_an_dbg_state_s {
		uint32_t fail_mode:2; /* fail mode (an, lt, link) */
		uint32_t lt_fail_count:4;
		uint32_t lnk_fail_count:4;
		uint32_t fail_type:4; /* Detailed failure */
		uint32_t err_cnt:22;  /* BER/ERR_BLK count */
		uint32_t lt_time:10;  /* Link Training time in ms */
		uint32_t lnk_time:14; /* Link time in ms */
		uint32_t reserved:4;
	} s;
} mcp_an_dbg_state_t;

typedef struct req_an_lt_args {
	uint32_t gser_type; /* GSERR, GSERC... */
	uint32_t gser_index;
	uint32_t shift_from_first;
	uint32_t lane_mask;
	uint32_t an_master; /* Logical lane 0 */
	uint32_t qlm;
	uint32_t max_num_lanes;
	mcp_an_dbg_state_t mcp_an_dbg;
	uint32_t qlm_mode; /*current/negotiated qlm_mode */
	uint32_t lmac_type; /*current/negotiated lmac_type */
	uint32_t fec_type; /*current/negotiated fec_type */
} async_req_an_lt_args_t;

typedef struct async_req {
	uint32_t req_id;
	/* For ex: which GPIO pin to toggle,
	 * which page/byte of EEPROM to read
	 */
	uint32_t req_args;
} async_req_t;

typedef enum lnk_fail_type {
	RCV_LNK_FAIL = 0,
	LOCAL_FLT_FAIL,
	ERR_BLKS_FAIL,
	BER_CNT_FAIL,
	REMOTE_FLT_FAIL,
	STABLE_TIMEOUT,
} lnk_fail_type_t;

typedef enum an_fail_type {
	UNSUPPORTED_TECH = 0,
	INVALID_WIDTH,
} an_fail_type_t;

typedef enum lt_fail_type {
	SERDES_TIMEOUT = 0,
	SW_TIMEOUT,
} lt_fail_type_t;

typedef enum mcp_an_fail_mode {
	LT_FAIL = 0,
	LNK_FAIL = 1,
	AN_FAIL = 2,
} mcp_an_fail_mode_t;

typedef enum an_lt_state {
	AN_LT_NO_STATE = 0,
	AN_LT_STATE_NO_RX_SIGNAL,
	AN_LT_STATE_AN_FIRST_LOOP,
	AN_LT_STATE_AN_IN_PROGRESS,
	AN_LT_STATE_AN_FAIL, /* If AN fails (e.g. HCD tech mismatch) */
	AN_LT_STATE_AN_COMPLETE,
	AN_LT_STATE_LT_FIRST_LOOP,
	AN_LT_STATE_LT_IN_PROGRESS,
	AN_LT_STATE_LT_FAIL,
	AN_LT_STATE_LINK_FIRST_LOOP,
	AN_LT_STATE_LINK_IN_PROGRESS,
	AN_LT_STATE_LINK_FAIL,
	AN_LT_STATE_LINK_UP,
	AN_LT_STATE_STOPPED, /* Enters on several LT or LINK UP failures */
} an_lt_state_t;

/* Ownership of shared memory for AN/LT processing */
typedef enum an_lt_own {
	AN_LT_OWN_NONE = 0,
	AN_LT_OWN_AP,
	AN_LT_OWN_MCP
} an_lt_own_info_t;

typedef struct async_context {
	/* lock to avoid conflict with AP/MCP SM - an_lt_own_info_t */
	uint32_t lock;
	/* ack to be set by AP and clear by MCP as a acknowledgment
	 * that the command is received
	 */
	uint32_t ack;
	/* 0 indicates fail and 1 indicates success */
	uint32_t req_stat;
	/* to return any data if required as a response */
	uint32_t data; /* state to be updated for REQ_AN_LT_GET_STATE */
	uint32_t sig_detect; /* set when Rx Signal detected once during AN */
} async_context_t;

typedef struct sfp_shared_data {
	uint32_t size;
	/* INTF version info to indicate AN/LT support */
	uint32_t intf_rev;
	/* SFP/QSFP EEPROM data */
	uint8_t buf[SFP_MAX_EEPROM_SIZE];
	char board_model[64];
	/* Module info from DT based on board */
	sfp_slot_info_t sfp_slot;
	/* State machine for SFP/QSFP state */
	sfp_context_t sfp_ctx;
	uint32_t cgx_id;
	uint32_t lmac_id;
	/* Required parameters to process for Rambus AN/LT */
	async_req_an_lt_args_t an_lt_args;
	/* Post Req to MCP async_req_id_t */
	async_req_t async_req;
	/* Context maintained for Async requests btw AP and MCP */
	async_context_t async_ctx;
} sfp_shared_data_t;

#endif /* __SFP_INTF_H__ */
