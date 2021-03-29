/* SPDX-License-Identifier: GPL-2.0-only */

/*
 */

#include <arch/cpu.h>
#include <device/pci_ops.h>
#include <console/console.h>
#include <cpu/intel/microcode.h>
#include <cpu/x86/msr.h>
#include <device/pci.h>
#include <device/pci_ids.h>
#include <intelblocks/mp_init.h>
#include <soc/bootblock.h>
#include <soc/pch.h>
#include <soc/pci_devs.h>
#include <string.h>

static struct {
	u32 cpuid;
	const char *name;
} cpu_table[] = {
	{ CPUID_SKYLAKE_B7, "Skylake B7" },
};

static struct {
	u16 lpcid;
	const char *name;
} pch_table[] = {
	{ PCI_DEVICE_ID_INTEL_LWB_C621, "Lewisburg C621" },
	{ PCI_DEVICE_ID_INTEL_LWB_C622, "Lewisburg C622" },
	{ PCI_DEVICE_ID_INTEL_LWB_C624, "Lewisburg C624" },
	{ PCI_DEVICE_ID_INTEL_LWB_C625, "Lewisburg C625" },
	{ PCI_DEVICE_ID_INTEL_LWB_C626, "Lewisburg C626" },
	{ PCI_DEVICE_ID_INTEL_LWB_C627, "Lewisburg C627" },
	{ PCI_DEVICE_ID_INTEL_LWB_C628, "Lewisburg C628" },
	{ PCI_DEVICE_ID_INTEL_LWB_C629, "Lewisburg C629" },
	{ PCI_DEVICE_ID_INTEL_LWB_C621A, "Lewisburg C621A" },
	{ PCI_DEVICE_ID_INTEL_LWB_C627A, "Lewisburg C627A" },
	{ PCI_DEVICE_ID_INTEL_LWB_C629A, "Lewisburg C629A" },
	{ PCI_DEVICE_ID_INTEL_LWB_C624_SUPER, "Lewisburg C624 SUPER SKU" },
	{ PCI_DEVICE_ID_INTEL_LWB_C627_SUPER_1, "Lewisburg C627 SUPER SKU 1" },
	{ PCI_DEVICE_ID_INTEL_LWB_C621_SUPER, "Lewisburg C621 SUPER SKU" },
	{ PCI_DEVICE_ID_INTEL_LWB_C627_SUPER_2, "Lewisburg C627 SUPER SKU 2" },
	{ PCI_DEVICE_ID_INTEL_LWB_C628_SUPER, "Lewisburg C628 SUPER SKU" },
	{ PCI_DEVICE_ID_INTEL_LWB_C621A_SUPER, "Lewisburg C621A SUPER SKU" },
	{ PCI_DEVICE_ID_INTEL_LWB_C627A_SUPER, "Lewisburg C627A SUPER SKU" },
	{ PCI_DEVICE_ID_INTEL_LWB_C629A_SUPER, "Lewisburg C629A SUPER SKU" },
};

static inline uint8_t get_dev_revision(pci_devfn_t dev)
{
	return pci_read_config8(dev, PCI_REVISION_ID);
}

static inline uint16_t get_dev_id(pci_devfn_t dev)
{
	return pci_read_config16(dev, PCI_DEVICE_ID);
}

static void report_cpu_info(void)
{
	struct cpuid_result cpuidr;
	u32 i, index, cpu_id, cpu_feature_flag;
	const char cpu_not_found[] = "Platform info not available";
	const char *cpu_name = cpu_not_found; /* 48 bytes are reported */
	int vt, txt, aes;
	static const char *const mode[] = {"NOT ", ""};
	const char *cpu_type = "Unknown";
	u32 p[13];

	index = 0x80000000;
	cpuidr = cpuid(index);
	if (cpuidr.eax >= 0x80000004) {
		int j = 0;

		for (i = 2; i <= 4; i++) {
			cpuidr = cpuid(index + i);
			p[j++] = cpuidr.eax;
			p[j++] = cpuidr.ebx;
			p[j++] = cpuidr.ecx;
			p[j++] = cpuidr.edx;
		}
		p[12] = 0;
		cpu_name = (char *)p;

		/* Skip leading spaces in CPU name string */
		while (cpu_name[0] == ' ' && strlen(cpu_name) > 0)
			cpu_name++;
	}

	cpu_id = cpu_get_cpuid();

	/* Look for string to match the name */
	for (i = 0; i < ARRAY_SIZE(cpu_table); i++) {
		if (cpu_table[i].cpuid == cpu_id) {
			cpu_type = cpu_table[i].name;
			break;
		}
	}

	printk(BIOS_DEBUG, "CPU: %s\n", cpu_name);
	printk(BIOS_DEBUG, "CPU: ID %x, %s, ucode: %08x\n",
	       cpu_id, cpu_type, get_current_microcode_rev());


	cpu_feature_flag = cpu_get_feature_flags_ecx();
	aes = (cpu_feature_flag & CPUID_AES) ? 1 : 0;
	txt = (cpu_feature_flag & CPUID_SMX) ? 1 : 0;
	vt = (cpu_feature_flag & CPUID_VMX) ? 1 : 0;
	printk(BIOS_DEBUG,
		"CPU: AES %ssupported, TXT %ssupported, VT %ssupported\n",
		mode[aes], mode[txt], mode[vt]);
}

static void report_pch_info(void)
{
	int i;
	pci_devfn_t dev = PCH_DEV_LPC;
	uint16_t lpcid = get_dev_id(dev);
	const char *pch_type = "Unknown";

	for (i = 0; i < ARRAY_SIZE(pch_table); i++) {
		if (pch_table[i].lpcid == lpcid) {
			pch_type = pch_table[i].name;
			break;
		}
	}
	printk(BIOS_DEBUG, "PCH: device id %04x (rev %02x) is %s\n",
		lpcid, get_dev_revision(dev), pch_type);
}

void report_platform_info(void)
{
	report_cpu_info();
	report_pch_info();
}
