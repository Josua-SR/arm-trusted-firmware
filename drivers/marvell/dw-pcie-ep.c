/*
 * Copyright (C) 2016 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <common/debug.h>
#include <lib/mmio.h>
#include <mvebu_def.h>
#include <pci_ep.h>

#define PCIE_CFG_VENDOR			0x0
#define PCIE_CFG_DEVICE			0x2
#define PCIE_CFG_CMD			0x4
#define  PCIE_CFG_CMD_IO_EN		BIT(0)
#define  PCIE_CFG_CMD_MEM_EN		BIT(1)
#define  PCIE_CFG_CMD_MASTER_EN		BIT(2)
#define PCIE_CFG_STATUS			0x6
#define PCIE_CFG_REVISION_ID		0x08	/* Revision ID */
#define PCIE_CFG_CLASS_PROG		0x09	/* Reg. Level Programming Interface */
#define PCIE_CFG_CLASS_DEVICE		0x0a	/* Device class */
#define PCIE_CFG_CACHE_LINE_SIZE	0x0c
/* Header type 0 */
#define PCIE_CFG_SUBSYSTEM_VENDOR_ID	0x2c
#define PCIE_CFG_SUBSYSTEM_ID		0x2e

#define PCIE_SRIOV_DEVID_OFFSET		0x192

#define PCIE_GLOBAL_CONTROL		0x8000
#define PCIE_CFG_RETRY_EN		BIT(9)
#define PCIE_APP_LTSSM_EN		BIT(2)
#define PCIE_DEVICE_TYPE_OFFSET		(4)
#define PCIE_DEVICE_TYPE_MASK		(0xF)
#define PCIE_DEVICE_TYPE_EP		(0x0) /* Endpoint */
#define PCIE_DEVICE_TYPE_LEP		(0x1) /* Legacy endpoint */
#define PCIE_DEVICE_TYPE_RC		(0x4) /* Root complex */

#define PCIE_LINK_CTL_2			0xA0
#define TARGET_LINK_SPEED_MASK		0xF
#define PCIE_LINK_CAPABILITY		0x7C

#define PCIE_GEN3_EQU_CTRL		0x8A8
#define PCIE_GEN3_EQ_FB_MODE_MASK	0xf
#define PCIE_GEN3_EQ_FB_MODE_OFFSET	0x0
#define GEN3_EQU_EVAL_2MS_DISABLE	BIT(5)
#define GEN3_EQ_PSET_REQ_VEC_MASK	0xffff00
#define GEN3_EQ_PSET_REQ_VEC_OFFSET	8


#define PCIE_ARCACHE_TRC		0x8050
#define PCIE_AWCACHE_TRC		0x8054
#define ARCACHE_SHAREABLE_CACHEABLE	0x3511
#define AWCACHE_SHAREABLE_CACHEABLE	0x5311

#define LINK_SPEED_GEN_1		0x1
#define LINK_SPEED_GEN_2		0x2
#define LINK_SPEED_GEN_3		0x3

/* iATU registers */
#define PCIE_IATU_VIEWPORT		0x900
#define PCIE_IATU_REGION_INBOUND	BIT(31)
#define PCIE_IATU_REGION_OUTBOUND	(0x0)

#define PCIE_IATU_CR1			0x904

#define PCIE_IATU_CR2			0x908
#define PCIE_IATU_CR2_REGION_EN		BIT(31)
#define PCIE_IATU_CR2_BAR_EN		BIT(30)
#define PCIE_IATU_CR2_FUNC_EN		BIT(19)
#define PCIE_IATU_CR2_BAR_OFF		8

#define PCIE_IATU_LOWER_BASE		0x90C
#define PCIE_IATU_UPPER_BASE		0x910
#define PCIE_IATU_LIMIT			0x914
#define PCIE_IATU_LOWER_TARGET		0x918
#define PCIE_IATU_UPPER_TARGET		0x91C

#define PCIE_IATU_CR3			0x920
#define PCIE_IATU_CR3_VF_ACTIVE		BIT(31)
#define PCIE_IATU_CR3_VF_NUM_MASK	(0x7)

#define PCI_MSI_CAP_ID_NEXT_CTRL_REG	0x50
#define PCI_MSI_ENABLE_MASK		BIT(16)

#define PCIE_MSIX_CAP_ID_NEXT_CTRL_REG	0xb0
#define PCIE_MSIX_CAP_ID		0x3
#define PCIE_MSIX_ENABLE_MASK		BIT(31)
#define PCIE_MSIX_TABLE_ENTRIES		(128 - 1) /* 0 => 1 entry */
#define PCIE_MSIX_TABLE_SIZE		(PCIE_MSIX_TABLE_ENTRIES << 16)
#define PCIE_MSIX_TABLE_OFFSET		0x1000
#define PCIE_MSIX_MSG_BIR		0
#define PCIE_PBA_OFFSET			0x1800
#define PCIE_PBA_BIR			0

#define PCIE_LANE_EQ_CTRL01_REG		0x164
#define PCIE_LANE_EQ_CTRL23_REG		0x168
#define PCIE_LANE_EQ_SETTING		0x55555555

#define PCIE_LINK_FLUSH_CONTROL_OFF_REG	0x8cc
#define PCIE_AUTO_FLUSH_EN_MASK		0x1

#define PCIE_REQ_RESET			0x8058
#define PCIE_LINK_REQ_RST_MASK		BIT(1)
#define PCIE_SMLH_REQ_RST_MASK		BIT(4)

#define PCIE_NUM_OF_VFS_TO_CONFIGURE	0x4
#define PCIE_VF_REGS_OFFSET		0x1000

#define PCIE_IATU_NUM_OF_VFS_TO_MAP	4
#define MAX_IATU_REGIONS		8
#define MAX_IATU_SIZE			(UINT32_MAX + 1ULL)

#define PCI_BASE_ADDRESS_0		0x10	/* 32 bits */
#define  PCI_BASE_ADDRESS_SPACE_MEMORY  0x00
#define  PCI_BASE_ADDRESS_MEM_TYPE_32	0x00	/* 32 bit address */
#define  PCI_BASE_ADDRESS_MEM_TYPE_64	0x04	/* 64 bit address */
#define  PCI_BASE_ADDRESS_MEM_MASK	(~0x0fULL)

#define PCIE_BAR_IS_RESIZABLE(bar)				\
		((bar) == PCI_BAR5 || (bar) == PCI_BAR4 ||	\
		 (bar) == PCI_BAR2 || (bar) == PCI_BAR0)

#define PCIE_RESBAR_EXT_CAP_HDR_REG	0x25c
#define PCIE_RESBAR_EXT_CAP_REG(bar)			\
	(PCIE_RESBAR_EXT_CAP_HDR_REG + 4 + (((bar) / 2 + (bar) % 2) & 0x3) * 8)

#define PCIE_RESBAR_EXT_CAP_REG_MASK	0x000fffff
#define PCIE_RESBAR_EXT_CAP_REG_SHIFT	4

#define BAR_ENABLE_OFFSET		0
#define BAR_ENABLE_MASK			BIT(BAR_ENABLE_OFFSET)

#define upper_32_bits(n) ((uint32_t)(((n) >> 16) >> 16))
#define lower_32_bits(n) ((uint32_t)(n))

/* setup the PCIe configuration header */
static void dw_pcie_write_header(uintptr_t dw_base, struct pci_ep_header *hdr)
{
	mmio_write_16(dw_base + PCIE_CFG_VENDOR, hdr->vendorid);
	mmio_write_16(dw_base + PCIE_CFG_DEVICE, hdr->deviceid);
	mmio_write_16(dw_base + PCIE_SRIOV_DEVID_OFFSET, hdr->vf_deviceid);

	mmio_write_8(dw_base + PCIE_CFG_REVISION_ID, hdr->revid);
	mmio_write_8(dw_base + PCIE_CFG_CLASS_PROG, hdr->progif_code);

	mmio_write_16(dw_base + PCIE_CFG_CLASS_DEVICE,
		      (hdr->baseclass_code << 8) | hdr->subclass_code);

	mmio_write_8(dw_base + PCIE_CFG_CACHE_LINE_SIZE, hdr->cache_line_size);

	mmio_write_16(dw_base + PCIE_CFG_SUBSYSTEM_ID, hdr->subsys_id);
	mmio_write_16(dw_base + PCIE_CFG_SUBSYSTEM_VENDOR_ID,
		      hdr->subsys_vendor_id);
}

static void dw_pcie_configure(uintptr_t dw_base, uint32_t cap_speed)
{
	uint32_t reg;
	uint32_t i;
	uintptr_t offset;

	/*
	 * Set the correct hints for lane equalization.
	 *
	 * These registers consist of the following fields:
	 *	- Downstream Port Transmitter Preset - Used for equalization by
	 *	  this port when the Port is operating as a downstream Port.
	 *	- Downstream Port Receiver Preset Hint - May be used as a hint
	 *	  for receiver equalization by this port when the Port is
	 *	  operating as a downstream Port.
	 *	- Upstream Port Transmitter Preset - Field contains the
	 *	  transmit preset value sent or received during link
	 *	  equalization.
	 *	- Upstream Port Receiver Preset Hint - Field contains the
	 *	  receiver preset hint value sent or received during link
	 *	  equalization.
	 *
	 * The default values for this registers aren't optimal for our
	 * hardware, so we set the optimal values according to HW measurements.
	 */
	mmio_write_32(dw_base + PCIE_LANE_EQ_CTRL01_REG,
		      PCIE_LANE_EQ_SETTING);
	mmio_write_32(dw_base + PCIE_LANE_EQ_CTRL23_REG,
		      PCIE_LANE_EQ_SETTING);

	/*  Set link to GEN 3 */;
	reg  = mmio_read_32(dw_base + PCIE_LINK_CTL_2);
	reg &= ~TARGET_LINK_SPEED_MASK;
	reg |= cap_speed;
	mmio_write_32(dw_base + PCIE_LINK_CTL_2, reg);

	reg  = mmio_read_32(dw_base + PCIE_LINK_CAPABILITY);
	reg &= ~TARGET_LINK_SPEED_MASK;
	reg |= cap_speed;
	mmio_write_32(dw_base + PCIE_LINK_CAPABILITY, reg);

	reg = mmio_read_32(dw_base + PCIE_GEN3_EQU_CTRL);
	reg |= GEN3_EQU_EVAL_2MS_DISABLE;

	/*
	 * According to the electrical measuremnts, the best presets for our
	 * receiver are preset3 to preset8, so we are changing the vector of
	 * presets to evaluate during the link equalization training preset3-8.
	 */
	reg &= ~GEN3_EQ_PSET_REQ_VEC_MASK;
	reg |= 0x3f0 << GEN3_EQ_PSET_REQ_VEC_OFFSET;
	reg &= ~PCIE_GEN3_EQ_FB_MODE_MASK;
	reg |= 0x1 << PCIE_GEN3_EQ_FB_MODE_OFFSET;
	mmio_write_32(dw_base + PCIE_GEN3_EQU_CTRL, reg);

	/*
	 * There is an issue in CPN110 that does not allow to
	 * enable/disable the link and perform "hot reset" unless
	 * the auto flush is disabled. So in order to enable the option
	 * to perform hot reset and link disable/enable we need to set
	 * auto flush to disable.
	 */
	reg = mmio_read_32(dw_base + PCIE_LINK_FLUSH_CONTROL_OFF_REG);
	reg &= ~PCIE_AUTO_FLUSH_EN_MASK;
	mmio_write_32(dw_base + PCIE_LINK_FLUSH_CONTROL_OFF_REG, reg);

	/*
	 * When the port is configured as Endpoint,
	 * the hot reset and link disable/enable must
	 * penetrate and reset the MAC configurations and thus
	 * need to unmask the reset.
	 */
	reg = mmio_read_32(dw_base + PCIE_REQ_RESET);
	reg &= ~PCIE_LINK_REQ_RST_MASK;
	mmio_write_32(dw_base + PCIE_REQ_RESET, reg);

	/* Request a Reset on link failure */
	reg = mmio_read_32(dw_base + PCIE_REQ_RESET);
	reg &= ~PCIE_SMLH_REQ_RST_MASK;
	mmio_write_32(dw_base + PCIE_REQ_RESET, reg);

	/*
	 * Enable PCI MSI capability
	 */
	reg = mmio_read_32(dw_base + PCI_MSI_CAP_ID_NEXT_CTRL_REG);
	reg |= PCI_MSI_ENABLE_MASK;
	mmio_write_32(dw_base + PCI_MSI_CAP_ID_NEXT_CTRL_REG, reg);

	/*
	 * Setup PCIE MSI-X capability structures for PF0 and its first VFs
	 */
	for (i = 0, offset = 0; i <= PCIE_NUM_OF_VFS_TO_CONFIGURE;
	     i++, offset += PCIE_VF_REGS_OFFSET) {
		/* PCI MSI-X Capability ID Next Control Register */
		mmio_write_32(dw_base + offset +
			      PCIE_MSIX_CAP_ID_NEXT_CTRL_REG,
			      PCIE_MSIX_ENABLE_MASK |
			      PCIE_MSIX_TABLE_SIZE  |
			      PCIE_MSIX_CAP_ID);
		/* MSI-X Table offset Register */
		mmio_write_32(dw_base + offset +
			      PCIE_MSIX_CAP_ID_NEXT_CTRL_REG + 0x4,
			      PCIE_MSIX_TABLE_OFFSET | PCIE_MSIX_MSG_BIR);
		/* MSI-X PBA offset Register */
		mmio_write_32(dw_base + offset +
			      PCIE_MSIX_CAP_ID_NEXT_CTRL_REG + 0x8,
			      PCIE_PBA_OFFSET | PCIE_PBA_BIR);
	}
}

static void dw_pcie_setup_bar(uintptr_t dw_base, uintptr_t shadow_base,
			      struct addr_map_win *addr_map)
{
	uint32_t  reg = 0;
	uint32_t  flags = PCI_BASE_ADDRESS_SPACE_MEMORY;
	uintptr_t bar_addr = dw_base + PCI_BASE_ADDRESS_0 +
					(addr_map->target_id * 4);
	uintptr_t bar_mask = shadow_base + PCI_BASE_ADDRESS_0 +
					(addr_map->target_id * 4);

	if (addr_map->target_id < PCI_BAR4)
		flags |= PCI_BASE_ADDRESS_MEM_TYPE_64;
	else
		flags |= PCI_BASE_ADDRESS_MEM_TYPE_32;
	/* clear the top 32 bits of the address */
	if (flags & PCI_BASE_ADDRESS_MEM_TYPE_64)
		mmio_write_32(bar_addr + 4, 0);

	mmio_write_32(bar_addr, flags & (~PCI_BASE_ADDRESS_MEM_MASK));

	/*
	 * Set the BAR using resizable BAR capability registers
	 * The minimum (and the default) BAR size is 1MB
	 * Once the Resizable BAR capability register is set
	 * the resizable BAR control register at next offset gets
	 * updated automatically.
	 */
	if (PCIE_BAR_IS_RESIZABLE(addr_map->target_id)) {
		/* BAR size should be power of 2 already */
		reg = (addr_map->win_size >> 20) & PCIE_RESBAR_EXT_CAP_REG_MASK;
		reg <<= PCIE_RESBAR_EXT_CAP_REG_SHIFT;
		mmio_write_32(dw_base +
			      PCIE_RESBAR_EXT_CAP_REG(addr_map->target_id), reg);
	}

	/* Enable bar */
	mmio_setbits_32(bar_mask, BAR_ENABLE_MASK);

	/* Setup the internal target for the BAR.
	 * When the PCIe host accesses the bar
	 * it will reach the space defined by "base_addr" and "win_size"
	 */
	reg = PCIE_IATU_REGION_INBOUND | addr_map->target_id;
	mmio_write_32(dw_base + PCIE_IATU_VIEWPORT, reg);

	addr_map->base_addr &= ~(addr_map->win_size - 1);
	reg = lower_32_bits(addr_map->base_addr);
	mmio_write_32(dw_base + PCIE_IATU_LOWER_TARGET, reg);

	reg = upper_32_bits(addr_map->base_addr);
	mmio_write_32(dw_base + PCIE_IATU_UPPER_TARGET, reg);
	/* PF0 */
	mmio_write_32(dw_base + PCIE_IATU_CR1, 0);

	reg = PCIE_IATU_CR2_REGION_EN | PCIE_IATU_CR2_BAR_EN |
	    (addr_map->target_id << PCIE_IATU_CR2_BAR_OFF);
	mmio_write_32(dw_base + PCIE_IATU_CR2, reg);
}

static void dw_pcie_link_up(uintptr_t dw_base, uint32_t cap_speed,
			    struct pci_ep_header *pci_hdr)
{
	uint32_t reg;

	/* Disable LTSSM state machine to enable configuration */
	reg = mmio_read_32(dw_base + PCIE_GLOBAL_CONTROL);
	reg &= ~(PCIE_APP_LTSSM_EN);
	reg &= ~(PCIE_DEVICE_TYPE_MASK << PCIE_DEVICE_TYPE_OFFSET);
	mmio_write_32(dw_base + PCIE_GLOBAL_CONTROL, reg);

	/* Set the PCIe master AXI attributes */
	mmio_write_32(dw_base + PCIE_ARCACHE_TRC, ARCACHE_SHAREABLE_CACHEABLE);
	mmio_write_32(dw_base + PCIE_AWCACHE_TRC, AWCACHE_SHAREABLE_CACHEABLE);

	/* DW pre link configurations */
	dw_pcie_configure(dw_base, cap_speed);
	dw_pcie_write_header(dw_base, pci_hdr);

	/* Configuration done. Start LTSSM */
	reg = mmio_read_32(dw_base + PCIE_GLOBAL_CONTROL);
	reg |= PCIE_APP_LTSSM_EN;
	mmio_write_32(dw_base + PCIE_GLOBAL_CONTROL, reg);

}

static void dw_pcie_open_out_atu(uintptr_t dw_base, uint64_t local_base,
				 uint64_t remote_base, uint64_t size)
{
	uint32_t  reg, region = 0;
	uint64_t  remain_size = size;
	uint32_t  vf_num;

	/* Setup outbound regions for PF0 */
	while (remain_size > 0) {
		if (region > (MAX_IATU_REGIONS - PCIE_IATU_NUM_OF_VFS_TO_MAP)) {
			ERROR("Insufficient ATU regions to map host\n");
			break;
		}

		reg = PCIE_IATU_REGION_OUTBOUND | region;
		mmio_write_32(dw_base + PCIE_IATU_VIEWPORT, reg);

		mmio_write_32(dw_base + PCIE_IATU_LOWER_BASE, local_base & UINT32_MAX);
		mmio_write_32(dw_base + PCIE_IATU_UPPER_BASE, local_base >> 32);
		mmio_write_32(dw_base + PCIE_IATU_LOWER_TARGET, remote_base & UINT32_MAX);
		mmio_write_32(dw_base + PCIE_IATU_UPPER_TARGET, remote_base >> 32);

		if (remain_size > MAX_IATU_SIZE)
			reg = MAX_IATU_SIZE - 1;
		else
			reg = remain_size - 1;
		mmio_write_32(dw_base + PCIE_IATU_LIMIT, reg);

		/* PF0 */
		mmio_write_32(dw_base + PCIE_IATU_CR1, 0);

		mmio_write_32(dw_base + PCIE_IATU_CR2, PCIE_IATU_CR2_REGION_EN);

		region++;
		local_base += MAX_IATU_SIZE;
		remote_base += MAX_IATU_SIZE;
		remain_size -= MAX_IATU_SIZE;
	}

	/* Setup outbound regions for VFs
	 * Continue with the next available region and address.
	 * They have already advanced by the PF loop.
	 */
	for (vf_num = 0; vf_num < PCIE_IATU_NUM_OF_VFS_TO_MAP;
	     vf_num++, local_base += MAX_IATU_SIZE, region++) {
		reg = PCIE_IATU_REGION_OUTBOUND | region;
		mmio_write_32(dw_base + PCIE_IATU_VIEWPORT, reg);

		mmio_write_32(dw_base + PCIE_IATU_LOWER_BASE, local_base & UINT32_MAX);
		mmio_write_32(dw_base + PCIE_IATU_UPPER_BASE, local_base >> 32);

		/* We always point to lower 4GB on host side for all VFs */
		mmio_write_32(dw_base + PCIE_IATU_LOWER_TARGET, 0);
		mmio_write_32(dw_base + PCIE_IATU_UPPER_TARGET, 0);

		/* Single region per VF. Region size is set to maximum */
		mmio_write_32(dw_base + PCIE_IATU_LIMIT, UINT32_MAX);

		/* Same PF for all VFs */
		mmio_write_32(dw_base + PCIE_IATU_CR1, 0);

		/* bind this region to VF number */
		reg = (vf_num & PCIE_IATU_CR3_VF_NUM_MASK)
				| PCIE_IATU_CR3_VF_ACTIVE;
		mmio_write_32(dw_base + PCIE_IATU_CR3, reg);

		mmio_write_32(dw_base + PCIE_IATU_CR2, PCIE_IATU_CR2_REGION_EN);
	}
}

static void dw_pcie_master_enable(uintptr_t dw_base)
{
	mmio_write_32(dw_base + PCIE_CFG_CMD, PCIE_CFG_CMD_MEM_EN |
					      PCIE_CFG_CMD_IO_EN |
					      PCIE_CFG_CMD_MASTER_EN);
}

static void dw_pcie_delay_cfg(uintptr_t dw_base, uint8_t delay)
{
	if (delay)	/* postpone the device configuration */
		mmio_setbits_32(dw_base + PCIE_GLOBAL_CONTROL, PCIE_CFG_RETRY_EN);
	else		/* enable the device right away */
		mmio_clrbits_32(dw_base + PCIE_GLOBAL_CONTROL, PCIE_CFG_RETRY_EN);
}

static void dw_pcie_disable_bar(uintptr_t shadow_base, uint32_t barnum)
{
	mmio_write_32(shadow_base + PCI_BASE_ADDRESS_0 + barnum * 4, 0);
}


void dw_pcie_ep_init(struct pci_hw_cfg *cfg)
{
	uint32_t  mapid;
	uint32_t  bar, bar_bmp;

	dw_pcie_link_up(cfg->mac_base, LINK_SPEED_GEN_3, cfg->hdr);

	/* setup BARs and ATU regions */
	for (mapid = 0, bar_bmp = 0; mapid < cfg->atu_map_size; mapid++) {
		struct addr_map_win *addr_win = cfg->atu_map + mapid;

		if ((addr_win->target_id == PCI_HOST) && (cfg->master_en)) {
			dw_pcie_open_out_atu(cfg->mac_base,
					     addr_win->base_addr, 0,
					     addr_win->win_size);
			dw_pcie_master_enable(cfg->mac_base);
		} else {
			/* BAR0 and BAR2 are 64 bits wide and occupy
			 * 2 consecutive BAR registers
			 */
			if (addr_win->target_id == PCI_BAR0 ||
			    addr_win->target_id == PCI_BAR2)
				bar_bmp |= 0x3 << addr_win->target_id;
			else
				bar_bmp |= 0x1 << addr_win->target_id;

			dw_pcie_setup_bar(cfg->mac_base,
					  cfg->shadow_base, addr_win);
		}
	}

	/* Disable unused BARs */
	for (bar = PCI_BAR0; bar < PCI_HOST; bar++) {
		uint32_t bar_mask = 1 << bar;

		if (!(bar_bmp & bar_mask))
			dw_pcie_disable_bar(cfg->shadow_base, bar);
	}

	dw_pcie_delay_cfg(cfg->mac_base, cfg->delay_cfg);
}
