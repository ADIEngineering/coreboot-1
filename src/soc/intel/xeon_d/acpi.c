/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <acpi/acpi.h>
#include <acpi/acpi_gnvs.h>
#include <acpi/acpigen.h>
#include <device/mmio.h>
#include <arch/smp/mpspec.h>
#include <console/console.h>
#include <device/device.h>
#include <device/pci_ops.h>
#include <intelblocks/cpulib.h>
#include <intelblocks/pmclib.h>
#include <intelblocks/acpi.h>
#include <soc/cpu.h>
#include <soc/iomap.h>
#include <soc/nvs.h>
#include <soc/pci_devs.h>
#include <soc/pm.h>
#include <soc/soc_chip.h>
#include <soc/systemagent.h>
#include <string.h>

/*
 * List of supported C-states in this processor.
 */
enum {
	C_STATE_C0,	/* 0 */
	C_STATE_C1,	/* 1 */
	C_STATE_C1E,	/* 2 */
	C_STATE_C3,	/* 3 */
	NUM_C_STATES
};

static const acpi_cstate_t cstate_map[NUM_C_STATES] = {
	[C_STATE_C0] = { },
	[C_STATE_C1] = {
		/* C1 */
		.latency = 0,
		.power = 0x3e8,
		.resource = MWAIT_RES(0, 0),
	},
	[C_STATE_C1E] = {
		/* C1E */
		.latency = 0,
		.power = 0x3e8,
		.resource = MWAIT_RES(0, 1),
	},
	[C_STATE_C3] = {
		/* C3 */
		.latency = 15,
		.power = 0x1f4,
		.resource = MWAIT_RES(1, 0),
	}
};

/* Max states supported */
static int cstate_set_all[] = {
	C_STATE_C0,
	C_STATE_C1,
	C_STATE_C1E,
	C_STATE_C3,
};

acpi_cstate_t *soc_get_cstate_map(size_t *entries)
{
	static acpi_cstate_t map[ARRAY_SIZE(cstate_set_all)];
	int *cstate_set;
	int i;

	const config_t *config = config_of_soc();

	const enum acpi_cstate_mode states = config->cstate_states;

	switch (states) {
	case CSTATES_ALL:
	default:
		*entries = ARRAY_SIZE(cstate_set_all);
		cstate_set = cstate_set_all;
		break;
	}

	for (i = 0; i < *entries; i++) {
		map[i] = cstate_map[cstate_set[i]];
		map[i].ctype = i + 1;
	}
	return map;
}

/*
 * XXX JJD Add power states
 */
void soc_fill_fadt(acpi_fadt_t *fadt)
{
        const uint16_t pmbase = ACPI_BASE_ADDRESS;

	fadt->pm2_cnt_blk = pmbase + PM2_CNT;
	fadt->pm2_cnt_len = 4;

        fadt->pm_tmr_blk = pmbase + PM1_TMR;
        fadt->pm_tmr_len = 4;

	fadt->p_lvl2_lat = 1;
	fadt->p_lvl3_lat = 87;

	fadt->duty_width = 0;
	fadt->mon_alrm = 0x00;
	fadt->century = 0x00;

        fadt->x_pm2_cnt_blk.space_id = ACPI_ADDRESS_SPACE_IO;
        fadt->x_pm2_cnt_blk.bit_width = fadt->pm2_cnt_len * 8;
        fadt->x_pm2_cnt_blk.bit_offset = 0;
        fadt->x_pm2_cnt_blk.access_size = ACPI_ACCESS_SIZE_BYTE_ACCESS;
        fadt->x_pm2_cnt_blk.addrl = pmbase + PM2_CNT;
        fadt->x_pm2_cnt_blk.addrh = 0x0;

        fadt->x_pm_tmr_blk.space_id = ACPI_ADDRESS_SPACE_IO;
        fadt->x_pm_tmr_blk.bit_width = fadt->pm_tmr_len * 8;
        fadt->x_pm_tmr_blk.bit_offset = 0;
        fadt->x_pm_tmr_blk.access_size = ACPI_ACCESS_SIZE_DWORD_ACCESS;
        fadt->x_pm_tmr_blk.addrl = pmbase + PM1_TMR;
        fadt->x_pm_tmr_blk.addrh = 0x0;

}

uint32_t soc_read_sci_irq_select(void)
{
        uintptr_t pmc_bar = soc_read_pmc_base();
        return read32((void *)pmc_bar + IRQ_REG);
}

static unsigned long soc_fill_dmar(unsigned long current)
{
#if 0
        const struct device *const igfx_dev = pcidev_path_on_root(SA_DEVFN_IGD);
        uint64_t gfxvtbar = MCHBAR64(GFXVTBAR) & VTBAR_MASK;
        bool gfxvten = MCHBAR32(GFXVTBAR) & VTBAR_ENABLED;

        if (is_dev_enabled(igfx_dev) && gfxvtbar && gfxvten) {
                unsigned long tmp = current;

                current += acpi_create_dmar_drhd(current, 0, 0, gfxvtbar);
                current += acpi_create_dmar_ds_pci(current, 0, 2, 0);

                acpi_dmar_drhd_fixup(tmp, current);
        }

        const struct device *const ipu_dev = pcidev_path_on_root(SA_DEVFN_IPU);
        uint64_t ipuvtbar = MCHBAR64(IPUVTBAR) & VTBAR_MASK;
        bool ipuvten = MCHBAR32(IPUVTBAR) & VTBAR_ENABLED;

        if (is_dev_enabled(ipu_dev) && ipuvtbar && ipuvten) {
                unsigned long tmp = current;

                current += acpi_create_dmar_drhd(current, 0, 0, ipuvtbar);
                current += acpi_create_dmar_ds_pci(current, 0, 5, 0);

                acpi_dmar_drhd_fixup(tmp, current);
        }

        uint64_t vtvc0bar = MCHBAR64(VTVC0BAR) & VTBAR_MASK;
        bool vtvc0en = MCHBAR32(VTVC0BAR) & VTBAR_ENABLED;

        if (vtvc0bar && vtvc0en) {
                const unsigned long tmp = current;

                current += acpi_create_dmar_drhd(current,
                                DRHD_INCLUDE_PCI_ALL, 0, vtvc0bar);
                current += acpi_create_dmar_ds_ioapic(current,
                                2, V_P2SB_CFG_IBDF_BUS, V_P2SB_CFG_IBDF_DEV,
                                V_P2SB_CFG_IBDF_FUNC);
                current += acpi_create_dmar_ds_msi_hpet(current,
                                0, V_P2SB_CFG_HBDF_BUS, V_P2SB_CFG_HBDF_DEV,
                                V_P2SB_CFG_HBDF_FUNC);

                acpi_dmar_drhd_fixup(tmp, current);
        }

        /* TCSS Thunderbolt root ports */
        for (unsigned int i = 0; i < MAX_TBT_PCIE_PORT; i++) {
                uint64_t tbtbar = MCHBAR64(TBT0BAR + i * 8) & VTBAR_MASK;
                bool tbten = MCHBAR32(TBT0BAR + i * 8) & VTBAR_ENABLED;
                if (tbtbar && tbten) {
                        unsigned long tmp = current;

                        current += acpi_create_dmar_drhd(current, 0, 0, tbtbar);
                        current += acpi_create_dmar_ds_pci_br(current, 0, 7, i);

                        acpi_dmar_drhd_fixup(tmp, current);
                }
        }

        /* Add RMRR entry */
        const unsigned long tmp = current;
        current += acpi_create_dmar_rmrr(current, 0,
                sa_get_gsm_base(), sa_get_tolud_base() - 1);
        current += acpi_create_dmar_ds_pci(current, 0, 2, 0);
        acpi_dmar_rmrr_fixup(tmp, current);

#endif
        return current;
}

unsigned long sa_write_acpi_tables(const struct device *dev, unsigned long current,
                                   struct acpi_rsdp *rsdp)
{
        acpi_dmar_t *const dmar = (acpi_dmar_t *)current;

        /*
         * Create DMAR table only if we have VT-d capability and FSP does not override its
         * feature.
         */
        if (!(pci_read_config32(dev, VTBAR) & VTBAR_ENABLED)) 
                return current;

        printk(BIOS_DEBUG, "ACPI:    * DMAR\n");
        acpi_create_dmar(dmar, DMAR_INTR_REMAP | DMA_CTRL_PLATFORM_OPT_IN_FLAG, soc_fill_dmar);
        current += dmar->header.length;
        current = acpi_align_current(current);
        acpi_add_table(rsdp, dmar);

        return current;
}

void soc_fill_gnvs(struct global_nvs *gnvs)
{
//        config_t *config = config_of_soc();

        /* Fill in Above 4GB MMIO resource */
        sa_fill_gnvs(gnvs);
}
uint32_t acpi_fill_soc_wake(uint32_t generic_pm1_en,
                            const struct chipset_power_state *ps)
{
        /*
         * WAK_STS bit is set when the system is in one of the sleep states
         * (via the SLP_EN bit) and an enabled wake event occurs. Upon setting
         * this bit, the PMC will transition the system to the ON state and
         * can only be set by hardware and can only be cleared by writing a one
         * to this bit position.
         */

        generic_pm1_en |= WAK_STS | RTC_EN | PWRBTN_EN;
        return generic_pm1_en;
}

int soc_madt_sci_irq_polarity(int sci)
{
        return MP_IRQ_POLARITY_HIGH;
}
