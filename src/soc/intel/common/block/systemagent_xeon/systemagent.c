/* SPDX-License-Identifier: GPL-2.0-only */

#include <cbmem.h>
#include <console/console.h>
#include <cpu/cpu.h>
#include <device/device.h>
#include <device/pci.h>
#include <device/pci_ids.h>
#include <intelblocks/acpi.h>
#include <intelblocks/cfg.h>
#include <intelblocks/systemagent_xeon.h>
#include <smbios.h>
#include <soc/iomap.h>
#include <soc/nvs.h>
#include <soc/pci_devs.h>
#include <soc/systemagent.h>
#include <types.h>
#include "systemagent_def.h"

/* SoC override function */
__weak void soc_systemagent_init(struct device *dev)
{
	/* no-op */
}

__weak void soc_add_fixed_mmio_resources(struct device *dev,
		int *resource_cnt)
{
	/* no-op */
}

__weak unsigned long sa_write_acpi_tables(const struct device *dev,
					  unsigned long current,
					  struct acpi_rsdp *rsdp)
{
	return current;
}

static void sa_soc_systemagent_init(struct device *dev)
{
	soc_systemagent_init(dev);

}

/*
 * Add all known fixed MMIO ranges that hang off the host bridge/memory
 * controller device.
 */
void sa_add_fixed_mmio_resources(struct device *dev, int *resource_cnt,
	const struct sa_mmio_descriptor *sa_fixed_resources, size_t count)
{
	int i;
	int index = *resource_cnt;

	for (i = 0; i < count; i++) {
		uintptr_t base;
		size_t size;

		size = sa_fixed_resources[i].size;
		base = sa_fixed_resources[i].base;

		mmio_resource(dev, index++, base / KiB, size / KiB);
	}

	*resource_cnt = index;
}

/*
 * DRAM memory mapped register
 *
 * TOHM: This 64 bit register defines the Top of High Memory
 * TOLM: This 32 bit register defines the Top of Low Memory
 * TSEG: This register contains the base address of TSEG DRAM memory
 */
static const struct sa_mem_map_descriptor sa_memory_map[MAX_MAP_ENTRIES] = {
	{ TOHM, true, "TOHM" },
	{ TOLM, false, "TOLM" },
	{ TSEG, false, "TSEG" },
};

/* Read DRAM memory map register value through PCI configuration space */
static void sa_read_map_entry(struct device *dev,
		const struct sa_mem_map_descriptor *entry, uint64_t *result)
{
	uint64_t value = 0;

	if (entry->is_64_bit) {
		value = pci_read_config32(dev, entry->reg + 4);
		value <<= 32;
	}

	value |= pci_read_config32(dev, entry->reg);
	/* All registers are on a 1MiB granularity. */
	value = ALIGN_DOWN(value, 1 * MiB);

	*result = value;
}

/* Fill MMIO resource above 4GB into GNVS */
void sa_fill_gnvs(struct global_nvs *gnvs)
{
	struct device *sa_dev = pcidev_path_on_root(SA_DEVFN_ROOT);

	sa_read_map_entry(sa_dev, &sa_memory_map[SA_TOHM_REG], &gnvs->a4gb);
	gnvs->a4gs = POWER_OF_2(cpu_phys_address_size()) - gnvs->a4gb;
	printk(BIOS_DEBUG, "PCI space above 4GB MMIO is at 0x%llx, len = 0x%llx\n",
	       gnvs->a4gb, gnvs->a4gs);
}

static void sa_get_mem_map(struct device *dev, uint64_t *values)
{
	int i;
	for (i = 0; i < MAX_MAP_ENTRIES; i++)
		sa_read_map_entry(dev, &sa_memory_map[i], &values[i]);
}

/*
 * These are the host memory ranges that should be added:
 * - 0 -> 0xa0000: cacheable
 * - 0xc0000 -> TOLM : cacheable
 * - TOLM -> 4GiB: not cacheable with standard MTRRs and reserved
 * - 4GiB -> TOHM: cacheable
 *
 * The default SMRAM space is reserved so that the range doesn't
 * have to be saved during S3 Resume. Once marked reserved the OS
 * cannot use the memory. This is a bit of an odd place to reserve
 * the region, but the CPU devices don't have dev_ops->read_resources()
 * called on them.
 *
 * The range 0xa0000 -> 0xc0000 does not have any resources
 * associated with it to handle legacy VGA memory. If this range
 * is not omitted the mtrr code will setup the area as cacheable
 * causing VGA access to not work.
 *
 * Don't need to mark the entire top_of_ram till TOLUD range (used
 * for stolen memory like GFX and ME, PTT, DPR, PRMRR, TSEG etc) as
 * cacheable for OS usage as coreboot already done with mpinit w/ smm
 * relocation early.
 *
 * It should be noted that cacheable entry types need to be added in
 * order. The reason is that the current MTRR code assumes this and
 * falls over itself if it isn't.
 *
 * The resource index starts low and should not meet or exceed
 * PCI_BASE_ADDRESS_0.
 */
static void sa_add_dram_resources(struct device *dev, int *resource_count)
{
	uintptr_t base_k, tohm_k;
	size_t size_k;
	uint64_t sa_map_values[MAX_MAP_ENTRIES];
	uintptr_t top_of_ram;
	int index = *resource_count;

	top_of_ram = (uintptr_t)cbmem_top();

	/* 0 - > 0xa0000 */
	base_k = 0;
	size_k = (0xa0000 / KiB) - base_k;
	ram_resource(dev, index++, base_k, size_k);

	/* 0xc0000 -> top_of_ram */
	base_k = 0xc0000 / KiB;
	size_k = (top_of_ram / KiB) - base_k;
	ram_resource(dev, index++, base_k, size_k);

	sa_get_mem_map(dev, &sa_map_values[0]);

	/* top_of_ram -> TOLM */
	base_k = top_of_ram;
	size_k = sa_map_values[SA_TOLM_REG] - base_k;
	mmio_resource(dev, index++, base_k / KiB, size_k / KiB);

	/* 4GiB -> TOHM */
	base_k = 4 * (GiB / KiB); /* 4GiB */
	tohm_k = sa_map_values[SA_TOHM_REG] / KiB;
	size_k = tohm_k - base_k;
	if (tohm_k > base_k)
		ram_resource(dev, index++, base_k, size_k);

	/*
	 * Reserve everything between A segment and 1MB:
	 *
	 * 0xa0000 - 0xbffff: legacy VGA
	 * 0xc0000 - 0xfffff: RAM
	 */
	mmio_resource(dev, index++, 0xa0000 / KiB, (0xc0000 - 0xa0000) / KiB);
	reserved_ram_resource(dev, index++, 0xc0000 / KiB,
			(1*MiB - 0xc0000) / KiB);

	*resource_count = index;
}

static void systemagent_read_resources(struct device *dev)
{
	int index = 0;

	/* XXX JJD, SKX Xeon SA (CHA), no std PCI bus resources */

	/* Add all fixed MMIO resources. */
	soc_add_fixed_mmio_resources(dev, &index);

	/* Calculate and add DRAM resources. */
	sa_add_dram_resources(dev, &index);
}

void enable_power_aware_intr(void)
{
#if 0 // XXX JJD skip
	uint8_t pair;

	/* Enable Power Aware Interrupt Routing */
	pair = MCHBAR8(MCH_PAIR);
	pair &= ~0x7;	/* Clear 2:0 */
	pair |= 0x4;	/* Fixed Priority */
	MCHBAR8(MCH_PAIR) = pair;
#endif
}

static struct device_operations systemagent_ops = {
	.read_resources   = systemagent_read_resources,
	.set_resources    = pci_dev_set_resources,
	.enable_resources = pci_dev_enable_resources,
	.init             = sa_soc_systemagent_init,
	.ops_pci          = &pci_dev_ops_pci,
#if CONFIG(HAVE_ACPI_TABLES)
	.write_acpi_tables = sa_write_acpi_tables,
#endif
};

static const unsigned short systemagent_ids[] = {
	PCI_DEVICE_ID_INTEL_SKX_MM,
	0
};

static const struct pci_driver systemagent_driver __pci_driver = {
	.ops     = &systemagent_ops,
	.vendor  = PCI_VENDOR_ID_INTEL,
	.devices = systemagent_ids
};
