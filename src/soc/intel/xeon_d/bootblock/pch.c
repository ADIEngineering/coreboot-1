/* SPDX-License-Identifier: GPL-2.0-only */

#include <console/console.h>
#include <device/pci_ops.h>
#include <soc/pci_devs.h>
#include <soc/pcr_ids.h>
#include <device/mmio.h>
#include <device/pci_ops.h>
#include <intelblocks/dmi.h>
#include <intelblocks/fast_spi.h>
#include <intelblocks/lpc_lib.h>
#include <intelblocks/p2sb.h>
#include <intelblocks/pcr.h>
#include <intelblocks/pmclib.h>
#include <intelblocks/rtc.h>
#include <soc/bootblock.h>
#include <soc/iomap.h>
#include <soc/p2sb.h>
#include <soc/pch.h>
#include <soc/pmc.h>
#include <soc/pci_devs.h>

#define PCR_DMI_ACPIBA		0x27B4
#define PCR_DMI_ACPIBDID	0x27B8
#define PCR_DMI_PMBASEA		0x27AC
#define PCR_DMI_PMBASEC		0x27B0

static void soc_config_pwrmbase(void)
{
	/*
	 * XXX JJD, again borrowed from others
	 * Assign Resources to PWRMBASE
	 * Clear BIT 1-2 Command Register
	 */
	pci_and_config16(PCH_DEV_PMC, PCI_COMMAND, ~(PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER));

	/* Program PWRM Base */
	pci_write_config32(PCH_DEV_PMC, PWRMBASE, PCH_PWRM_BASE_ADDRESS);

	/* Enable Bus Master and MMIO Space */
	pci_or_config16(PCH_DEV_PMC, PCI_COMMAND, (PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER));

	/* Enable PWRM in PMC */
	setbits32((void *) PCH_PWRM_BASE_ADDRESS + ACTL, PWRM_EN);
}

void bootblock_pch_early_init(void)
{
	/* 
	 * XXX JJD
	 * P2SB config, shamelessly borrowed from other platforms
	 */
	p2sb_enable_bar();
	p2sb_configure_hpet();

	fast_spi_early_init(SPI_BASE_ADDRESS);

#if 0

	/*
	 * Enabling PWRM Base for accessing
	 * Global Rest Cause Register.
	 * XXX JJD, maybe not necessary
	 */
	soc_config_pwrmbase();
#endif
}

void pch_early_iorange_init(void)
{
	/* 
	 * XXX JJD, figure out if external IO ranges are needed,
	 * (SIO, KBC, EC, etc)
	 */
	lpc_io_setup_comm_a_b();

	/* Program generic IO Decode */
	pch_enable_lpc();
}


static void soc_config_acpibase(void)
{
	uint32_t reg32;

	/* Disable ABASE in PMC Device first before changing Base Address */
	reg32 = pci_read_config32(PCH_DEV_PMC, ACTL);
	pci_write_config32(PCH_DEV_PMC, ACTL, reg32 & ~ACPI_EN);

	/* Program ACPI Base */
	pci_write_config32(PCH_DEV_PMC, ABASE, ACPI_BASE_ADDRESS);

	/* Enable ACPI in PMC */
	pci_write_config32(PCH_DEV_PMC, ACTL, reg32 | ACPI_EN);

	uint32_t data = pci_read_config32(PCH_DEV_PMC, ABASE);
	printk(BIOS_INFO, "%s : pmbase = %x\n", __func__, (int)data);
	/*
	 * Program "ACPI Base Address" PCR[DMI] + 27B4h[23:18, 15:2, 0]
	 * to [0x3F, PMC PCI Offset 40h bit[15:2], 1]
	 */
	reg32 = (0x3f << 18) | ACPI_BASE_ADDRESS | 1;
	pcr_write32(PID_DMI, PCR_DMI_ACPIBA, reg32);
	pcr_write32(PID_DMI, PCR_DMI_ACPIBDID, 0x23a8);

}

void bootblock_pch_init(void)
{
	/*
	 * Enabling ABASE for accessing PM1_STS, PM1_EN, PM1_CNT
	 */
	soc_config_acpibase();

	soc_config_pwrmbase();
	/* XXX JJD GPE config? */

	enable_rtc_upper_bank();

}

#if 0 // XXX JJD, lets ignore this for now
void override_hpet_ioapic_bdf(void)
{
	union p2sb_bdf ioapic_bdf = {
		.bus = PCH_IOAPIC_BUS_NUMBER,
		.dev = PCH_IOAPIC_DEV_NUM,
		.fn = PCH_IOAPIC_FUNC_NUM,
	};
	union p2sb_bdf hpet_bdf = {
		.bus = HPET_BUS_NUM,
		.dev = HPET_DEV_NUM,
		.fn = HPET0_FUNC_NUM,
	};

	p2sb_set_ioapic_bdf(ioapic_bdf);
	p2sb_set_hpet_bdf(hpet_bdf);
}

void pch_lock_dmictl(void)
{
	uint32_t reg32 = pcr_read32(PID_DMI, PCR_DMI_DMICTL);
	pcr_write32(PID_DMI, PCR_DMI_DMICTL, reg32 | PCR_DMI_DMICTL_SRLOCK);
}
#endif
