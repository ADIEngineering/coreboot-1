#ifndef SOC_SKYLAKE_D_SYSTEMAGENT_H
#define SOC_SYYLAKE_D_SYSTEMAGENT_H

#include <intelblocks/systemagent_xeon.h>

#define DMIBAR 0x50
#define VTBAR  0x180
#define  VTBAR_ENABLED 0x01

static const struct sa_mmio_descriptor soc_vtbar_mmio_descriptor = {
        VTBAR,
        VT_BASE_ADDRESS,
        VT_BASE_SIZE,
        "VTBAR"
};

/* Hardcoded default values for PCI Bus:Dev.Fun for IOAPIC and HPET */
#define V_P2SB_IBDF_BUS 250
#define V_P2SB_IBDF_DEV 31
#define V_P2SB_IBDF_FUN 0
#define V_DEFAULT_IBDF  ((V_P2SB_IBDF_BUS << 8) | PCI_DEVFN(V_P2SB_IBDF_DEV, V_P2SB_IBDF_FUN))

#define V_P2SB_HBDF_BUS 250
#define V_P2SB_HBDF_DEV 15
#define V_P2SB_HBDF_FUN 0
#define V_DEFAULT_HBDF  ((V_P2SB_HBDF_BUS << 8) | PCI_DEVFN(V_P2SB_HBDF_DEV, V_P2SB_HBDF_FUN))

#endif /* SOC_SKYLAKE_D_SYSTEMAGENT_H */
