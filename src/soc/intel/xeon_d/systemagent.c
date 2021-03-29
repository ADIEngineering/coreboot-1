/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * This file is created based on Intel SKX-D Processor EDS
 */

#include <cpu/x86/msr.h>
#include <delay.h>
#include <console/console.h>
#include <device/device.h>
#include <device/pci_ops.h>
#include <device/pci.h>
#include <device/pci_ids.h>
#include <intelblocks/systemagent_xeon.h>
#include <soc/iomap.h>
#include <soc/msr.h>
#include <soc/systemagent.h>
#include <soc/soc_chip.h>

/*
 * SoC implementation
 *
 * Add all known fixed memory ranges for Host Controller/Memory
 * controller.
 */
void soc_add_fixed_mmio_resources(struct device *dev, int *index)
{
	static const struct sa_mmio_descriptor soc_mm_fixed_resources[] = {
		{ MMCFG_BASE_BAR, CONFIG_MMCONF_BASE_ADDRESS, CONFIG_MMCONF_LENGTH,
				"MMCFG_BAR" },
		{ DMIBAR, DMI_BASE_ADDRESS, DMI_BASE_SIZE, "DMIBAR" },
	};

	sa_add_fixed_mmio_resources(dev, index, soc_mm_fixed_resources,
			ARRAY_SIZE(soc_mm_fixed_resources));


#if 0	
	sa_add_fixed_mmio_resources(dev, index, &soc_vtbar_mmio_descriptor, 1);
#endif
#if 0	/* XXX JJD, fill out later */
	/* Add Vt-d resources if VT-d is enabled */
	if ((pci_read_config32(dev, CAPID0_A) & VTD_DISABLE))
		return;

	sa_add_fixed_mmio_resources(dev, index, soc_vtd_resources,
			ARRAY_SIZE(soc_vtd_resources));
#endif
}

/*
 * SoC implementation
 *
 * Perform System Agent Initialization during Ramstage phase.
 */
void soc_systemagent_init(struct device *dev)
{
	/* Enable BIOS Reset CPL */
	enable_bios_reset_cpl();

}

int soc_get_uncore_prmmr_base_and_mask(uint64_t *prmrr_base,
        uint64_t *prmrr_mask)
{
        msr_t msr;
        msr = rdmsr(MSR_UNCORE_PRMRR_PHYS_BASE);
        *prmrr_base = (uint64_t) msr.hi << 32 | msr.lo;
        msr = rdmsr(MSR_UNCORE_PRMRR_PHYS_MASK);
        *prmrr_mask = (uint64_t) msr.hi << 32 | msr.lo;
        return 0;
}

