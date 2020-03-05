/*
 * Copyright (C) 2020 Marvell International Ltd.
 * This program is provided "as is" without any warranty of any kind,
 * and is distributed under the applicable Marvell proprietary limited use
 * license agreement.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <octeontx_common.h>
#include <plat_board_cfg.h>
#include <octeontx_ecam.h>
#include <octeontx_utils.h>
#include <platform_def.h>
#include <octeontx_irqs_def.h>

#include "cavm-csrs-ecam.h"
#include "cavm-csrs-iobn.h"
#include "cavm-csrs-pccpf.h"

/*
 * stream_uses_rslx_or_devx()
 *
 * This is used to indicate if a particular stream is affected by
 * IOBNX_RSLX_STREAMS or IOBNX_DOMX_DEVX_STREAMS.
 *
 * When programming IOBN security permissions, streams use a combination of
 * IOBNX_DOMX_BUSX_STREAMS and one of:
 *   IOBNX_DOMX_DEVX_STREAMS
 *   IOBNX_RSLX_STREAMS
 *
 * See HRM documentation for 'IOBNX_DOMX_BUSX_STREAMS'.
 *
 * This function helps distinguish the correct registers to use for
 * security permissions.
 *
 * on entry,
 *   streamid : one of 'CAVM_PCC_DEV_CON_E_xxx'
 *
 * returns,
 *   false  if specified streamid is affected by IOBNX_DOMX_DEVX_STREAMS
 *   !false if specified streamid is affected by IOBNX_RSLX_STREAMS
 */
static bool stream_uses_rslx_or_devx(uint32_t streamid)
{
	int domain_idx, bus_idx;

	domain_idx = STREAM_DMN_IDX(streamid);
	bus_idx = STREAM_BUS_IDX(streamid);

	/* Streams on same domain & bus as CAVM_PCC_DEV_CON_E_MRML are
	 * affected by IOBNX_RSLX_STREAMS.
	 */
	return (domain_idx == STREAM_DMN_IDX(CAVM_PCC_DEV_CON_E_MRML)) &&
	       (bus_idx == STREAM_BUS_IDX(CAVM_PCC_DEV_CON_E_MRML));
}

void octeontx_init_iobn(uint64_t config_base, uint64_t config_size)
{
	int rsl_idx, iobn_nr;
	int domain, bus, dev, j, inst;
	union cavm_pccpf_xxx_vsec_ctl vsec_ctl;
	union cavm_ecamx_const ecamx_const;
	union cavm_ecamx_domx_const domx_const;
	int iobn5_domain_max;
	int set_all_domains;
	int strm_nsec, phys_nsec;
	uint32_t streamid;
	/*
	 * Settings in this table are common to all OTX2 platforms.
	 *
	 * This array contains specific permissions settings for
	 * individual streams (i.e. devices).
	 *
	 * All other settings use the default (i.e. non-secure):
	 *   strm_nsec = 1
	 *   phys_nsec = 1
	 */
	struct otx2_stream_security_setting stream_settings[] = {
		/* To get secure interrupts for GPIO, PHYS_NSEC must == 0 */
		{ CAVM_PCC_DEV_CON_E_GPIO_CN9, 1, 1 /* strm */, 0 /* phys */ },
		/* To get secure interrupts for BPHY, PHYS_NSEC must == 0 */
		{ CAVM_PCC_DEV_CON_E_BPHY, 1, 1 /* strm */, 0 /* phys */ },
	};

	vsec_ctl.u = octeontx_read32(config_base + CAVM_PCCPF_XXX_VSEC_CTL);
	iobn_nr = vsec_ctl.s.inst_num;

	/*
	 * Allow all IO units to access non-secure memory.
	 * Program secure devices individually as needed
	 * (per 'stream_settings[]' above).
	 */
	for (rsl_idx = 0; rsl_idx < 256; rsl_idx++) {
		CSR_WRITE(CAVM_IOBNX_RSLX_STREAMS(iobn_nr, rsl_idx), 0x3);

		/* check if individual streams require RSLX setting */
		for (j = 0; j < ARRAY_SIZE(stream_settings); j++) {
			/* Iterate all instances of the stream */
			for (inst = 0; inst < stream_settings[j].instances;
			     inst++) {
				streamid = stream_settings[j].streamid + inst;

				/* A stream will use DEVX or RSLX, not both */
				if (!stream_uses_rslx_or_devx(streamid))
					continue;
				/* Verify stream uses this RSLX register */
				if (STREAM_RSL_IDX(streamid) != rsl_idx)
					continue;

				/* configure security in IOBNX_RSLX_STREAMS */
				CSR_MODIFY(c,
					CAVM_IOBNX_RSLX_STREAMS(iobn_nr,
								rsl_idx),
					c.s.strm_nsec =
						stream_settings[j].strm_nsec;
					c.s.phys_nsec =
						stream_settings[j].phys_nsec);
			}
		}
	}

	if (IS_OCTEONTX_VAR(read_midr(), T96PARTNUM, 1) ||
	    IS_OCTEONTX_PN(read_midr(), F95PARTNUM) ||
	    IS_OCTEONTX_PN(read_midr(), F95MMPARTNUM)) {
		/*
		 * ECAMX_DOMX_CONST shows up only that are present under it.
		 * To enable PEMX controllers on domain 3 4 5 6, use the loop
		 * max as 0x7 to include them.
		 */
		iobn5_domain_max = 0x7;
		set_all_domains = 1;
	} else {
		ecamx_const.u = CSR_READ(CAVM_ECAMX_CONST(0));
		iobn5_domain_max = ecamx_const.s.domains;
		set_all_domains = 0;
	}

	for (domain = 0; domain < iobn5_domain_max; domain++) {
		if (!set_all_domains) {
			/* Domains may not be contiguous */
			domx_const.u = CSR_READ(CAVM_ECAMX_DOMX_CONST(
								0, domain));
			if (!domx_const.s.pres)
				continue;
		}

		for (bus = 0; bus < 256; bus++) {
			/* default setting */
			CSR_WRITE(CAVM_IOBNX_DOMX_BUSX_STREAMS(
				  iobn_nr, domain, bus), 0x3);

			/* check if individual streams require BUSX setting */
			for (j = 0; j < ARRAY_SIZE(stream_settings); j++) {
				strm_nsec = stream_settings[j].strm_nsec;
				phys_nsec = stream_settings[j].phys_nsec;

				/* Iterate all instances of the stream */
				for (inst = 0; inst <
				     stream_settings[j].instances;
				     inst++) {
					streamid = stream_settings[j].streamid;
					streamid += inst;

					/* Verify stream uses this domain/bus */
					if ((STREAM_DMN_IDX(streamid) !=
					    domain))
						continue;
					if ((STREAM_BUS_IDX(streamid) != bus))
						continue;

					/* configure IOBNX_BUSX_STREAMS */
					CSR_MODIFY(c,
						CAVM_IOBNX_DOMX_BUSX_STREAMS(
							iobn_nr, domain, bus),
					/*
					 * Only set a more-permissive
					 * value. If already set to
					 * be permissive (i.e. 0),
					 * another device on this bus
					 * has already been configured.
					 * Don't disrupt its settings.
					 * Refer to [96xx] HRM descr
					 * of this register.
					 */
					if (c.s.strm_nsec == 1)
						c.s.strm_nsec =
							strm_nsec;
					if (c.s.phys_nsec == 1)
						c.s.phys_nsec =
							phys_nsec
					); /* end of CSR_MODIFY */
				}
			}
		}

		for (dev = 0; dev < 32; dev++) {
			/* default setting */
			CSR_WRITE(CAVM_IOBNX_DOMX_DEVX_STREAMS(
				  iobn_nr, domain, dev), 0x3);

			/* check if individual streams require DEVX setting */
			for (j = 0; j < ARRAY_SIZE(stream_settings); j++) {
				strm_nsec = stream_settings[j].strm_nsec;
				phys_nsec = stream_settings[j].phys_nsec;

				/* Iterate all instances of the stream */
				for (inst = 0; inst <
				     stream_settings[j].instances;
				     inst++) {
					streamid = stream_settings[j].streamid;
					streamid += inst;

					/* stream uses DEVX or RSLX, not both */
					if (stream_uses_rslx_or_devx(streamid))
						continue;
					/* verify stream uses this domain/dev */
					if ((STREAM_DMN_IDX(streamid) !=
					    domain))
						continue;
					if ((STREAM_DEV_IDX(streamid) != dev))
						continue;

					/* configure IOBNX_DEVX_STREAMS */
					CSR_MODIFY(c,
						CAVM_IOBNX_DOMX_DEVX_STREAMS(
							iobn_nr, domain, dev),
						c.s.strm_nsec = strm_nsec;
						c.s.phys_nsec = phys_nsec);
				}
			}
		}
	}
}


