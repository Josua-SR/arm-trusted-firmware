/*
 * Copyright (C) 2016-2017 Marvell International Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <arch.h>
#include <platform_def.h>
#include <lib/psci/psci.h>

static const unsigned char plat_octeontx_power_domain_tree_desc[] = {
	/* No of root nodes */
	[0] = PLATFORM_NODE_COUNT,
	/* No of clusters */
	[1 ... PLATFORM_NODE_COUNT]
		= PLATFORM_MAX_CLUSTERS_PER_NODE,
	/* No of CPU cores */
	[1 + PLATFORM_NODE_COUNT ...
		1 + PLATFORM_NODE_COUNT + PLATFORM_CLUSTER_COUNT - 1] =
			PLATFORM_CORE_PER_CLUSTER,
};

/*******************************************************************************
 * This function returns the Cavium default topology tree information.
 ******************************************************************************/
const unsigned char *plat_get_power_domain_tree_desc(void)
{
	return plat_octeontx_power_domain_tree_desc;
}

/*******************************************************************************
 * This function implements a part of the critical interface between the psci
 * generic layer and the platform that allows the former to query the platform
 * to convert an MPIDR to a unique linear index. An error code (-1) is returned
 * in case the MPIDR is invalid.
 ******************************************************************************/
int plat_core_pos_by_mpidr(u_register_t mpidr)
{
	unsigned int cpu_id_upper2bits, cpu_id, node_id;

	mpidr &= MPIDR_AFFINITY_MASK;

	if (mpidr & ~(MPIDR_CLUSTER_MASK | MPIDR_CPU_MASK | MPIDR_NODE_MASK))
		return -1;

	node_id = (mpidr >> MPIDR_AFF2_SHIFT) & MPIDR_AFFLVL_MASK;
	/*
	 * cpu_id on OcteonTX is splited across AFF0 and AFF1 fields.
	 * AF1 holds upper 2 bits of cpu_id and AF0 holds lower 4 bits.
	 */
	cpu_id_upper2bits = (mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK;
	cpu_id = (mpidr >> MPIDR_AFF0_SHIFT) & MPIDR_AFFLVL_MASK;
	cpu_id |= cpu_id_upper2bits << 4;

	if (node_id >= PLATFORM_MAX_NODES)
		return -1;

	if (cpu_id >= PLATFORM_MAX_CPUS_PER_CLUSTER *
			PLATFORM_MAX_CLUSTERS_PER_NODE)
		return -1;
	return (cpu_id + (node_id *
			PLATFORM_MAX_CLUSTERS_PER_NODE *
			PLATFORM_MAX_CPUS_PER_CLUSTER));
}
