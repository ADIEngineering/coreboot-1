/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _SOC_CHIP_H_
#define _SOC_CHIP_H_

#include <intelblocks/cfg.h>
#include <intelblocks/gpio.h>
#include <intelblocks/pcie_rp.h>
#include <soc/gpe.h>
#include <soc/gpio.h>
#include <soc/pch.h>
#include <soc/pci_devs.h>
#include <soc/pmc.h>
#include <types.h>

enum acpi_cstate_mode {
	CSTATES_ALL = 0
};

struct soc_intel_xeon_d_config {

	/* Common struct containing soc config data required by common code */
	struct soc_intel_common_config common_soc_config;

	/* Gpio group routed to each dword of the GPE0 block. Values are
	 * of the form PMC_GPP_[A:U] or GPD. */
	uint8_t gpe0_dw0; /* GPE0_31_0 STS/EN */
	uint8_t gpe0_dw1; /* GPE0_63_32 STS/EN */
	uint8_t gpe0_dw2; /* GPE0_95_64 STS/EN */

	/* Generic IO decode ranges */
	uint32_t gen1_dec;
	uint32_t gen2_dec;
	uint32_t gen3_dec;
	uint32_t gen4_dec;

	/* TCC activation offset */
	uint32_t tcc_offset;

	/* SATA related */
	uint8_t SataEnable;

	/* PCIe Root Ports */
	uint8_t PcieRpEnable[CONFIG_MAX_ROOT_PORTS];
	uint8_t PcieRpHotPlug[CONFIG_MAX_ROOT_PORTS];
	/* PCIe output clocks type to PCIe devices.
	 * 0-23: PCH rootport, 0x70: LAN, 0x80: unspecified but in use,
	 * 0xFF: not used */
	uint8_t PcieClkSrcUsage[CONFIG_MAX_PCIE_CLOCKS];
	/* PCIe ClkReq-to-ClkSrc mapping, number of clkreq signal assigned to
	 * clksrc. */
	uint8_t PcieClkSrcClkReq[CONFIG_MAX_PCIE_CLOCKS];

	/* Probe CLKREQ# signal before enabling CLKREQ# based power management.*/
	uint8_t PcieRpClkReqDetect[CONFIG_MAX_ROOT_PORTS];

	/* Enable PCIe Precision Time Measurement for Root Ports (disabled by default) */
	uint8_t PciePtm[CONFIG_MAX_ROOT_PORTS];

	/* PCIe RP L1 substate */
	enum L1_substates_control PcieRpL1Substates[CONFIG_MAX_ROOT_PORTS];

	/* PCIe LTR: Enable (1) / Disable (0) */
	uint8_t PcieRpLtrEnable[CONFIG_MAX_ROOT_PORTS];

	/* PCIE RP Advanced Error Report: Enable (1) / Disable (0) */
	uint8_t PcieRpAdvancedErrorReporting[CONFIG_MAX_ROOT_PORTS];

	/* SMBus */
	uint8_t SmbusEnable;

	uint8_t Device4Enable;

	/* HeciEnabled decides the state of Heci1 at end of boot
	 * Setting to 0 (default) disables Heci1 and hides the device from OS */
	uint8_t HeciEnabled;

	/* Enable/Disable EIST. 1b:Enabled, 0b:Disabled */
	uint8_t eist_enable;

	/* HyperThreadingDisable : Yes (1) / No (0) */
	uint8_t HyperThreadingDisable;

	enum acpi_cstate_mode cstate_states;
};

typedef struct soc_intel_xeon_d_config config_t;

#endif
