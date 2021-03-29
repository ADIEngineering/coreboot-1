## SPDX-License-Identifier: GPL-2.0-only

ifeq ($(CONFIG_SOC_INTEL_SKYLAKE_D),y)

ramstage-y += cpu.c

CPPFLAGS_common += -I$(src)/soc/intel/xeon_d/skx/include -I$(src)/soc/intel/xeon_d/skx

endif ## CONFIG_SOC_INTEL_SKYLAKE_D
