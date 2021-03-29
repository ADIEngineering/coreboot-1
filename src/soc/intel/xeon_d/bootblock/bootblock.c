/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <bootblock_common.h>
#include <intelblocks/fast_spi.h>
#include <intelblocks/systemagent_xeon.h>
#include <console/console.h>
#include <soc/bootblock.h>
#include <fsp/util.h>
#include <FsptUpd.h>

const FSPT_UPD temp_ram_init_params = {
	.FspUpdHeader = {
		.Signature = FSPT_UPD_SIGNATURE,
		.Revision = 1,
		.Reserved = {0},
	},
	.FsptCommonUpd = {
		.Revision = 0xDE,
//		.MicrocodeRegionBase =   (UINT32)CONFIG_CPU_MICROCODE_CBFS_LOC,
//		.MicrocodeRegionLength = (UINT32)CONFIG_CPU_MICROCODE_CBFS_LEN,
		.CodeRegionBase =        (UINT32)0xFFC00000,
		.CodeRegionLength =      (UINT32)0x00400000,
		.Mmcfg =                 (UINT32)0xC0000000,
	},
	.ReservedFsptUpd1 = {0},
	.UnusedUpdSpace0 = {0},
	.UpdTerminator = 0x55AA,
};

static uint64_t assembly_timestamp;
static uint64_t bootblock_timestamp;

asmlinkage void bootblock_c_entry(uint64_t base_timestamp)
{
#if 0
	/*
	 * FSP-T does not respect its own API and trashes registers
	 * coreboot uses to store its initial timestamp.
	 */
	assembly_timestamp = base_timestamp;
	bootblock_timestamp = timestamp_get();
	fast_spi_cache_bios_region();
#endif
	bootblock_main_with_basetime(MIN(assembly_timestamp, bootblock_timestamp));
}

void bootblock_soc_early_init(void)
{
	bootblock_systemagent_early_init();
	bootblock_pch_early_init();
	fast_spi_cache_bios_region();

	pch_early_iorange_init();
}

void bootblock_soc_init(void)
{
	report_platform_info();
	bootblock_pch_init();

	if (CONFIG(BOOTBLOCK_CONSOLE))
		printk(BIOS_DEBUG, "FSP TempRamInit successful...\n");

	if (assembly_timestamp > bootblock_timestamp)
		printk(BIOS_WARNING, "Invalid initial timestamp detected\n");

	if (CONFIG(FSP_CAR))
		report_fspt_output();
}
