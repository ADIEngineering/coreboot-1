/* SPDX-License-Identifier: GPL-2.0-only */

/*
 */

#include <intelblocks/systemagent_xeon.h>
#include <soc/iomap.h>
#include <soc/romstage.h>
#include <soc/systemagent.h>

void systemagent_early_init(void)
{
	static const struct sa_mmio_descriptor soc_fixed_dmi_resources[] = {
		{ DMIBAR, DMI_BASE_ADDRESS, DMI_BASE_SIZE, "DMIBAR" },
	};

	/* Set Fixed MMIO address into PCI configuration space */
	sa_set_dmi_bar(soc_fixed_dmi_resources,
		       ARRAY_SIZE(soc_fixed_dmi_resources));
	/* Enable PAM registers */
	enable_pam_region();
}
