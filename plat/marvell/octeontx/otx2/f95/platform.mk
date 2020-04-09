# Copyright (C) 2016-2019 Marvell International Ltd.
#
# SPDX-License-Identifier:     BSD-3-Clause
# https://spdx.org/licenses

RAS_EXTENSION		:=       1
HANDLE_EA_EL3_FIRST	:=       1

# Include common Marvell platform's makefile helper
include plat/marvell/octeontx/platform.mk

PLAT_XLAT_TABLES_DYNAMIC :=	1
$(eval $(call add_define,PLAT_XLAT_TABLES_DYNAMIC))

SCMI_WITH_LEGACY_PM	:=	1
ifdef SCMI_WITH_LEGACY_PM
    $(eval $(call add_define,SCMI_WITH_LEGACY_PM))
endif

# Define DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS to enable diagnostic cmds
#DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS		:=	1
ifdef DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS
    $(eval $(call add_define,DEBUG_ATF_ENABLE_SERDES_DIAGNOSTIC_CMDS))
endif

ARM_ARCH_MINOR		:=	2
# Allow to use armv8.2 specific instructions
TF_CFLAGS_aarch64	:=	$(patsubst -march=%, -march=armv8.2-a, $(TF_CFLAGS_aarch64))

PLAT_INCLUDES		+=	-Iinclude/plat/marvell/octeontx/otx2			\
				-Iplat/marvell/octeontx/otx2/f95/include		\
				-Ilib/libphy/marvell_88x5113/include			\
				-Ilib/libphy/marvell_88x5113/serdes/src/include		\

PLAT_BL_COMMON_SOURCES	+=	plat/marvell/octeontx/otx2/f95/plat_f95_setup.c		\
				plat/marvell/octeontx/otx2/plat_security.c		\
				plat/marvell/octeontx/otx2/plat_bcfg_init.c		\
				drivers/marvell/cgx.c		\
				drivers/marvell/cgx_flash_ops.c		\
				drivers/marvell/gsern/gsern_init_common.c	\
				drivers/marvell/gsern/gsern_init_network.c	\
				drivers/marvell/gsern/gsern_api.c		\
				drivers/marvell/gser_internal.c			\
				drivers/marvell/gsern/gsern_eye.c		\
				drivers/marvell/qlm/qlm.c			\
				drivers/marvell/qlm/qlm_gsern.c			\
				drivers/marvell/qlm/qlm_gserr.c			\
				drivers/marvell/qlm/qlm_gsern_ops.c		\
				drivers/marvell/qlm/qlm_gserr_ops.c		\
				plat/marvell/octeontx/otx2/plat_setup.c		\
				plat/marvell/octeontx/otx2/aarch64/plat_octeontx_common.S	\
				plat/marvell/octeontx/otx2/aarch64/plat_helpers.S		\

BL2_SOURCES		+=	plat/marvell/octeontx/otx2/f95/plat_f95_ecam.c		\
				drivers/marvell/sh_fwdata.c		\
				drivers/marvell/rvu.c		\
				plat/marvell/octeontx/otx2/plat_npc_mcam_profile.c \
				plat/marvell/octeontx/otx2/plat_board_cfg.c			\
				plat/marvell/octeontx/otx2/plat_scfg.c				\
				plat/marvell/octeontx/otx2/plat_otx2_iobn.c			\

BL31_LIBS               +=      lib/libphy/libphy_88x5113.a     \

BL31_SOURCES		+=	plat/marvell/octeontx/otx2/plat_topology.c		\
				plat/marvell/octeontx/otx2/octeontx_trace.c		\
				drivers/marvell/phy/phy_gen.c		\
				drivers/marvell/phy/phy_vitesse.c	\
				drivers/marvell/phy/phy_marvell.c	\
				drivers/marvell/phy/phy_marvell_1514.c	\
				drivers/marvell/phy/phy_marvell_5113.c	\
				drivers/marvell/phy/phy_mgmt.c		\
				drivers/marvell/sh_fwdata.c		\
				drivers/marvell/rvu.c		\
				drivers/marvell/sfp_mgmt.c		\
				drivers/marvell/cgx_intf.c		\
				plat/marvell/octeontx/otx2/plat_pm.c			\
				plat/marvell/octeontx/otx2/plat_pwrc.c		\
				plat/marvell/octeontx/otx2/plat_scmi.c		\
				plat/marvell/octeontx/otx2/plat_flr.c	\
				drivers/marvell/bphy.c			\
				plat/marvell/octeontx/otx2/plat_npc_mcam_profile.c	\
				plat/marvell/octeontx/otx2/plat_legacy_pm_ops.c		\
				plat/marvell/octeontx/otx2/f95/plat_f95_svc.c  \
				plat/marvell/octeontx/otx2/plat_svc.c		\

ifeq (${RAS_EXTENSION},1)
BL31_SOURCES		+=	plat/marvell/octeontx/otx2/smc_ras.c
BL31_SOURCES		+=	plat/marvell/octeontx/otx2/lmc_ras.c
BL31_SOURCES		+=	plat/marvell/octeontx/otx2/mdc_ras.c
BL31_SOURCES		+=	plat/marvell/octeontx/otx2/plat_ras.c
BL31_SOURCES		+=	lib/extensions/ras/std_err_record.c
BL31_SOURCES		+=	lib/extensions/ras/ras_common.c
endif

ifdef NT_FW_CONFIG
    $(eval $(call add_define,NT_FW_CONFIG))
    $(eval $(call TOOL_ADD_PAYLOAD,${NT_FW_CONFIG},--nt-fw-config))
endif
