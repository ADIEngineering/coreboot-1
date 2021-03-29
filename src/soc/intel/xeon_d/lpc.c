/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <device/device.h>
#include <device/pci.h>
#include <pc80/isa-dma.h>
#include <pc80/i8259.h>
#include <console/console.h>
#include <arch/ioapic.h>
#include <intelblocks/lpc_lib.h>
#include <soc/iomap.h>
#include <soc/pcr_ids.h>
#include <soc/intel/common/block/lpc/lpc_def.h>

#include <soc/soc_chip.h>

void soc_get_gen_io_dec_range(uint32_t *gen_io_dec)
{
	const config_t *config = config_of_soc();

	gen_io_dec[0] = config->gen1_dec;
	gen_io_dec[1] = config->gen2_dec;
	gen_io_dec[2] = config->gen3_dec;
	gen_io_dec[3] = config->gen4_dec;
}

void lpc_soc_init(struct device *dev)
{
	printk(BIOS_SPEW, "pch: lpc_init\n");
	/* legacy init */
	isa_dma_init();
	pch_misc_init();

	/* enable CLKRUN_EN for power gating LPC */
	lpc_enable_pci_clk_cntl();
#if 0
	/* set LPC Serial IRQ mode */
	lpc_set_serirq_mode(config->serirq_mode);
#endif

	/* Interrupt configuration */
	pch_enable_ioapic();
	pch_pirq_init();
	setup_i8259();
	
	i8259_configure_irq_trigger(9,1);
#if 0
	/* FSP configures IOAPIC and PCHInterrupt Config */
	printk(BIOS_SPEW, "IOAPICID 0x%x, 0x%x\n",
		io_apic_read((void *)IO_APIC_ADDR, 0x00),
		((io_apic_read((void *)IO_APIC_ADDR, 0x00) & 0x0f000000) >> 24));
#endif
}
