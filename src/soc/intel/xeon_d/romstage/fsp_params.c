/* SPDX-License-Identifier: GPL-2.0-only */

#include <assert.h>
#include <console/console.h>
#include <cpu/x86/msr.h>
#include <device/device.h>
#include <fsp/util.h>
#include <intelblocks/cpulib.h>
#include <intelblocks/mp_init.h>
//#include <soc/gpio_soc_defs.h>
#include <soc/iomap.h>
#include <soc/msr.h>
#include <soc/pci_devs.h>
#include <soc/romstage.h>
#include <soc/soc_chip.h>
#include <string.h>

static void soc_memory_init_params(FSP_M_CONFIG *m_cfg,
		const struct soc_intel_xeon_d_config *config)
{
	/* XXX JJD, this should all move to devicetree config */
	/* Set debug interface flags */
	m_cfg->SerialDebugEnable = 1;
	m_cfg->SerialDebugPortAddress = 0x3f8;
	m_cfg->SerialDebugPortBaudRate = 115200;
	m_cfg->SerialDebugErrorLevelMask = 0x8000000F;
	m_cfg->MemoryDownEnable = 0;

	m_cfg->MemoryCacheMapEnable = 0;

	m_cfg->GpioCfgEnable = 0; 

}

void platform_fsp_memory_init_params_cb(FSPM_UPD *mupd, uint32_t version)
{
	const struct soc_intel_xeon_d_config *config;
	FSP_M_CONFIG *m_cfg = &mupd->FspmConfig;

	config = config_of_soc();

	soc_memory_init_params(m_cfg, config);
	mainboard_memory_init_params(mupd);
}

__weak void mainboard_memory_init_params(FSPM_UPD *mupd)
{
	printk(BIOS_DEBUG, "WEAK: %s/%s called\n", __FILE__, __func__);
}
