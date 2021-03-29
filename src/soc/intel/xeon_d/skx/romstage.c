/* SPDX-License-Identifier: GPL-2.0-only */

#include <arch/romstage.h>
#include <intelblocks/rtc.h>
#include <soc/romstage.h>
#include <soc/soc_util.h>

#include <soc/soc_chip.h>

void platform_fsp_memory_init_params_cb(FSPM_UPD *mupd, uint32_t version)
{
//	const config_t *config = config_of_soc();
	FSP_M_CONFIG *m_cfg = &mupd->FspmConfig;

	m_cfg->SerialDebugErrorLevelMask = 8;
	m_cfg->MemoryCacheMapEnable = 1;

	mainboard_memory_init_params(mupd);

}
