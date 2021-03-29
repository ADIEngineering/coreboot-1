/* SPDX-License-Identifier: GPL-2.0-only */
#include <assert.h>
#include <console/console.h>
#include <fsp/api.h>
#include <soc/romstage.h>
#include <skxsp_skxdcc_iio.h>

void mainboard_memory_init_params(FSPM_UPD *mupd)
{
	FSP_M_CONFIG *mem_cfg = &mupd->FspmConfig;

	mem_cfg->IioBifurcationTablePtr = (IIO_BIFURCATION_DATA_ENTRY *)skxdcc_iio_bifur_table;
	mem_cfg->IioBifurcationTableEntryNum = 2;
	mem_cfg->IioSlotCfgPtr = (IIO_SLOT_CONFIG_DATA_ENTRY *)skxdcc_iio_pci_port_skt0;
	mem_cfg->IioSlotCfgEntryNum = 8;

}
