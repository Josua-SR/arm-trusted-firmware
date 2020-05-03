/*
 * Copyright (C) 2016 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#ifndef _PCIE_EP_H_
#define _PCIE_EP_H_

#include <drivers/marvell/addr_map.h>

#define PCIE_MAX_BARS		6
#define PCIE_MAX_LANES		16
#define PCI_VENDOR_ID_MARVELL	0x11ab

enum atu_attribute_ids {
	PCI_BAR0,
	PCI_BAR1,
	PCI_BAR2,
	PCI_BAR3,
	PCI_BAR4,
	PCI_BAR5,
	PCI_BAR6,
	PCI_HOST,
	PCI_INVALID_ID
};

struct pci_ep_header {
	uint16_t	vendorid;
	uint16_t	deviceid;
	uint16_t	vf_deviceid;
	uint8_t		revid;
	uint8_t		progif_code;
	uint8_t		subclass_code;
	uint8_t		baseclass_code;
	uint8_t		cache_line_size;
	uint16_t	subsys_vendor_id;
	uint16_t	subsys_id;
};

struct pci_hw_cfg {
	uint8_t delay_cfg;
	uint8_t master_en;
	uint8_t	lane_width;
	uint8_t	lane_ids[PCIE_MAX_LANES];
	uint8_t	clk_src;
	uint8_t	clk_out;
	uint8_t	is_end_point;
	uintptr_t mac_base;
	uintptr_t comphy_base;
	uintptr_t hpipe_base;
	uintptr_t dfx_base;
	uintptr_t shadow_base;
	struct addr_map_win *atu_map;
	uint32_t atu_map_size;
	struct pci_ep_header *hdr;
};

void dw_pcie_ep_init(struct pci_hw_cfg *cfg);
int comphy_pcie_power_up(uint32_t lane, struct pci_hw_cfg *hw);

#endif /* _PCIE_EP_H_ */

