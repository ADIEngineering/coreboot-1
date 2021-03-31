## SPDX-License-Identifier: GPL-2.0-or-later

ifeq ($(CONFIG_SOC_INTEL_SKYLAKE_D),y)

subdirs-y += romstage
subdirs-y += ../../../cpu/intel/turbo
subdirs-y += ../../../cpu/intel/microcode
subdirs-y += ../../../cpu/x86/lapic
subdirs-y += ../../../cpu/x86/mtrr
subdirs-y += ../../../cpu/x86/smm
subdirs-y += ../../../cpu/x86/tsc

all-y += spi.c
all-y += gpio.c
all-y += pmutil.c
all-y += soc_util.c

bootblock-y += bootblock/bootblock.c 
bootblock-y += bootblock/pch.c
bootblock-y += bootblock/report_platform.c
bootblock-y += lpc.c
bootblock-y += p2sb.c

romstage-y += reset.c 
romstage-y += pmc.c
#romstage-y += util.c

ramstage-y += acpi.c
ramstage-y += chip.c
ramstage-y += lpc.c
ramstage-y += p2sb.c
#ramstage-y += uncore.c 
ramstage-y += reset.c 
ramstage-y += ramstage.c 
ramstage-y += chip_common.c
#ramstage-y += pch.c
ramstage-y += pmc.c
ramstage-y += systemagent.c
ramstage-y += util.c
ramstage-$(CONFIG_HAVE_SMI_HANDLER) += smmrelocate.c


smm-$(CONFIG_HAVE_SMI_HANDLER) += gpio.c
smm-$(CONFIG_HAVE_SMI_HANDLER) += pmutil.c
smm-$(CONFIG_HAVE_SMI_HANDLER) += p2sb.c
smm-$(CONFIG_HAVE_SMI_HANDLER) += smihandler.c

postcar-y += spi.c

CPPFLAGS_common += -I$(src)/soc/intel/xeon_d/include
CPPFLAGS_common += -I$(src)/soc/intel/common/pch/include/
CPPFLAGS_common += -I$(CONFIG_FSP_HEADER_PATH)

endif ## CONFIG_SOC_INTEL_SKYLAKE_D
