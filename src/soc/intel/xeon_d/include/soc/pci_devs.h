/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _SOC_XEON_PCI_DEVS_H_
#define _SOC_XEON_PCI_DEVS_H_

#include <device/pci_def.h>

#define _SA_DEVFN(slot)		PCI_DEVFN(SA_DEV_SLOT_ ## slot, 0)
#define _PCH_DEVFN(slot, func)	PCI_DEVFN(PCH_DEV_SLOT_ ## slot, func)

#if !defined(__SIMPLE_DEVICE__)
#include <device/device.h>
#define _SA_DEV(slot)		pcidev_path_on_root(_SA_DEVFN(slot))
#define _PCH_DEV(slot, func)	pcidev_path_on_root_debug(_PCH_DEVFN(slot, func), __func__)
#else
#define _SA_DEV(slot)		PCI_DEV(0, SA_DEV_SLOT_ ## slot, 0)
#define _PCH_DEV(slot, func)	PCI_DEV(0, PCH_DEV_SLOT_ ## slot, func)
#endif

/* System Agent Devices */
/* XXX JJD should probably change this to CHA or something */

#define DMI_DEV_SLOT_ROOT	0
#define  DMI_DEVFN_ROOT		PCI_DEVFN(DMI_DEV_SLOT_ROOT,0)
#if defined(__SIMPLE_DEVICE__)
#define DMI_DEV_ROOT		PCI_DEV(0, DMI_DEV_SLOT_ROOT, 0)
#endif

#define SA_DEV_SLOT_ROOT	0x05
#define  SA_DEVFN_ROOT		PCI_DEVFN(SA_DEV_SLOT_ROOT, 0)
#if defined(__SIMPLE_DEVICE__)
#define  SA_DEV_ROOT		PCI_DEV(0, SA_DEV_SLOT_ROOT, 0)
#endif

#define SA_DEV_SLOT_VMD		0x05
#define  SA_DEVFN_VMD		PCI_DEVFN(SA_DEV_SLOT_VMD, 5)
#define  SA_DEV_VMD		PCI_DEV(0, SA_DEV_SLOT_VMD, 5)

/* PCH Devices */
/* XXX JJD fill out more when necessary */

#define PCH_DEV_SLOT_XHCI	0x14
#define  PCH_DEVFN_XHCI		_PCH_DEVFN(XHCI, 0)
#define  PCH_DEV_XHCI		_PCH_DEV(XHCI, 0)
#define   PCH_DEVFN_THERMAL	_PCH_DEVFN(XHCI, 2)

#define PCH_DEV_SLOT_SATA	0x17
#define  PCH_DEVFN_SATA		_PCH_DEVFN(SATA, 0)
#define  PCH_DEV_SATA		_PCH_DEV(SATA, 0)

#define PCH_DEV_SLOT_SIO4	0x19
#define  PCH_DEVFN_I2C4		_PCH_DEVFN(SIO4, 0)
#define  PCH_DEVFN_I2C5		_PCH_DEVFN(SIO4, 1)
#define  PCH_DEVFN_UART2	_PCH_DEVFN(SIO4, 2)
#define  PCH_DEV_I2C4		_PCH_DEV(SIO4, 0)
#define  PCH_DEV_I2C5		_PCH_DEV(SIO4, 1)
#define  PCH_DEV_UART2		_PCH_DEV(SIO4, 2)

#define PCH_DEV_SLOT_PCIE_2	0x1b
#define  PCH_DEVFN_PCIE16	_PCH_DEVFN(PCIE_2, 0)
#define  PCH_DEVFN_PCIE17	_PCH_DEVFN(PCIE_2, 1)
#define  PCH_DEVFN_PCIE18	_PCH_DEVFN(PCIE_2, 2)
#define  PCH_DEVFN_PCIE19	_PCH_DEVFN(PCIE_2, 3)

#define PCH_DEV_SLOT_PCIE	0x1c
#define  PCH_DEVFN_PCIE0	_PCH_DEVFN(PCIE, 0)
#define  PCH_DEVFN_PCIE1	_PCH_DEVFN(PCIE, 1)
#define  PCH_DEVFN_PCIE2	_PCH_DEVFN(PCIE, 2)
#define  PCH_DEVFN_PCIE3	_PCH_DEVFN(PCIE, 3)
#define  PCH_DEVFN_PCIE4	_PCH_DEVFN(PCIE, 4)
#define  PCH_DEVFN_PCIE5	_PCH_DEVFN(PCIE, 5)
#define  PCH_DEVFN_PCIE6	_PCH_DEVFN(PCIE, 6)
#define  PCH_DEVFN_PCIE7	_PCH_DEVFN(PCIE, 7)
#define  PCH_DEV_PCIE0		_PCH_DEV(PCIE, 0)
#define  PCH_DEV_PCIE1		_PCH_DEV(PCIE, 1)
#define  PCH_DEV_PCIE2		_PCH_DEV(PCIE, 2)
#define  PCH_DEV_PCIE3		_PCH_DEV(PCIE, 3)
#define  PCH_DEV_PCIE4		_PCH_DEV(PCIE, 4)
#define  PCH_DEV_PCIE5		_PCH_DEV(PCIE, 5)
#define  PCH_DEV_PCIE6		_PCH_DEV(PCIE, 6)
#define  PCH_DEV_PCIE7		_PCH_DEV(PCIE, 7)

#define PCH_DEV_SLOT_PCIE_1	0x1d
#define  PCH_DEVFN_PCIE8	_PCH_DEVFN(PCIE_1, 0)
#define  PCH_DEVFN_PCIE9	_PCH_DEVFN(PCIE_1, 1)
#define  PCH_DEVFN_PCIE10	_PCH_DEVFN(PCIE_1, 2)
#define  PCH_DEVFN_PCIE11	_PCH_DEVFN(PCIE_1, 3)
#define  PCH_DEVFN_PCIE12	_PCH_DEVFN(PCIE_1, 4)
#define  PCH_DEVFN_PCIE13	_PCH_DEVFN(PCIE_1, 5)
#define  PCH_DEVFN_PCIE14	_PCH_DEVFN(PCIE_1, 6)
#define  PCH_DEVFN_PCIE15	_PCH_DEVFN(PCIE_1, 7)
#define  PCH_DEV_PCIE8		_PCH_DEV(PCIE_1, 0)
#define  PCH_DEV_PCIE9		_PCH_DEV(PCIE_1, 1)
#define  PCH_DEV_PCIE10		_PCH_DEV(PCIE_1, 2)
#define  PCH_DEV_PCIE11		_PCH_DEV(PCIE_1, 3)
#define  PCH_DEV_PCIE12		_PCH_DEV(PCIE_1, 4)
#define  PCH_DEV_PCIE13		_PCH_DEV(PCIE_1, 5)
#define  PCH_DEV_PCIE14		_PCH_DEV(PCIE_1, 6)
#define  PCH_DEV_PCIE15		_PCH_DEV(PCIE_1, 7)

#define PCH_DEV_SLOT_ESPI	0x1f
#define PCH_DEV_SLOT_LPC	PCH_DEV_SLOT_ESPI
#define  PCH_DEVFN_ESPI		_PCH_DEVFN(ESPI, 0)
#define  PCH_DEVFN_P2SB		_PCH_DEVFN(ESPI, 1)
#define  PCH_DEVFN_PMC		_PCH_DEVFN(ESPI, 2)
#define  PCH_DEVFN_HDA		_PCH_DEVFN(ESPI, 3)
#define  PCH_DEVFN_SMBUS	_PCH_DEVFN(ESPI, 4)
#define  PCH_DEVFN_SPI		_PCH_DEVFN(ESPI, 5)
#define  PCH_DEVFN_GBE		_PCH_DEVFN(ESPI, 6)
#define  PCH_DEVFN_TRACEHUB	_PCH_DEVFN(ESPI, 7)
#define  PCH_DEV_ESPI		_PCH_DEV(ESPI, 0)
#define  PCH_DEV_LPC		PCH_DEV_ESPI
#define  PCH_DEV_P2SB		_PCH_DEV(ESPI, 1)
#define  PCH_DEV_PMC		_PCH_DEV(ESPI, 2)
#define  PCH_DEV_SMBUS		_PCH_DEV(ESPI, 4)
#define  PCH_DEV_SPI		_PCH_DEV(ESPI, 5)
#define  PCH_DEV_GBE		_PCH_DEV(ESPI, 6)

#define PCU_IIO_STACK                   1
#define PCU_DEV                         30
#define PCU_CR1_FUN                     1

#define PCU_CR0_FUN                     0
#define PCU_CR0_PLATFORM_INFO           0xa8
#define PCU_CR0_P_STATE_LIMITS          0xd8
#define P_STATE_LIMITS_LOCK_SHIFT       31
#define P_STATE_LIMITS_LOCK             (1 << P_STATE_LIMITS_LOCK_SHIFT)
#define PCU_CR0_TEMPERATURE_TARGET      0xe4
#define PCU_CR0_PACKAGE_RAPL_LIMIT      0xe8
#define PCU_CR0_CURRENT_CONFIG          0xf8
#define MAX_NON_TURBO_LIM_RATIO_SHIFT   8 /* 8:15 */
#define MAX_NON_TURBO_LIM_RATIO_MASK    (0xff << MAX_NON_TURBO_LIM_RATIO_SHIFT)

#define PCU_CR1_BIOS_MB_DATA_REG                           0x8c

#define PCU_CR1_BIOS_MB_INTERFACE_REG                      0x90
#define   BIOS_MB_RUN_BUSY_MASK                            BIT(31)
#define   BIOS_MB_CMD_MASK                                 0xff
#define   BIOS_CMD_READ_PCU_MISC_CFG                       0x5
#define   BIOS_CMD_WRITE_PCU_MISC_CFG                      0x6
#define   BIOS_ERR_INVALID_CMD                             0x01

#define PCU_CR1_BIOS_RESET_CPL_REG                         0x94
#define   RST_CPL1_MASK                                    BIT(1)
#define   RST_CPL2_MASK                                    BIT(2)
#define   RST_CPL3_MASK                                    BIT(3)
#define   RST_CPL4_MASK                                    BIT(4)
#define   PCODE_INIT_DONE1_MASK                            BIT(9)
#define   PCODE_INIT_DONE2_MASK                            BIT(10)
#define   PCODE_INIT_DONE3_MASK                            BIT(11)
#define   PCODE_INIT_DONE4_MASK                            BIT(12)

#define PCU_CR1_DESIRED_CORES_CFG2_REG                     0xa0
#define PCU_CR1_DESIRED_CORES_CFG2_REG_LOCK_MASK           BIT(31)

#define UBOX_DECS_BUS                   0
#define UBOX_DECS_DEV                   8
#define UBOX_DECS_FUNC                  2
#define UBOX_DECS_CPUBUSNO_CSR          0xcc
#define UBOX_DECS_CPUBUSNO1_CSR         0xd0

#define PCU_CR1_C2C3TT_REG                                 0xdc
#define PCU_CR1_PCIE_ILTR_OVRD                             0xfc
#define PCU_CR1_SAPMCTL                                    0xb0
#define SAPMCTL_LOCK_SHIFT                                 31
#define SAPMCTL_LOCK_MASK                                  (1 << SAPMCTL_LOCK_SHIFT)
#define PCU_CR1_MC_BIOS_REQ                                0x98

#define PCU_CR2_FUN                                        2
#define PCU_CR2_PKG_CST_ENTRY_CRITERIA_MASK                0x8c
#define PCIE_IN_PKGCSTATE_L1_MASK                          0xFFFFFF /* 23:0 bits */
#define PCU_CR2_PKG_CST_ENTRY_CRITERIA_MASK2               0x90
#define KTI_IN_PKGCSTATE_L1_MASK                           0x7 /* 2:0 bits */
#define PCU_CR2_DYNAMIC_PERF_POWER_CTL                     0xdc
#define UNCORE_PLIMIT_OVERRIDE_BIT                         20
#define UNOCRE_PLIMIT_OVERRIDE_SHIFT                       (1 << UNCORE_PLIMIT_OVERRIDE_BIT)
#define PCU_CR2_PROCHOT_RESPONSE_RATIO_REG                 0xb0
#define PROCHOT_RATIO                                      0xa /* bits 0:7 */

#define CHA_UTIL_ALL_DEV                                   29
#define CHA_UTIL_ALL_FUNC                                  1
#define CHA_UTIL_ALL_MMCFG_CSR                             0xc0

#if !ENV_RAMSTAGE
/*
 * PCH_DEV_PMC is intentionally not defined in RAMSTAGE since PMC device gets
 * hidden from PCI bus after call to FSP-S. This leads to resource allocator
 * dropping it from the root bus as unused device. All references to PCH_DEV_PMC
 * would then return NULL and can go unnoticed if not handled properly. Since,
 * this device does not have any special chip config associated with it, it is
 * okay to not provide the definition for it in ramstage.
 */
#define  PCH_DEV_PMC		_PCH_DEV(ESPI, 2)
#endif

#define  PCH_DEV_HDA		_PCH_DEV(ESPI, 3)
#define  PCH_DEV_SMBUS		_PCH_DEV(ESPI, 4)
#define  PCH_DEV_SPI		_PCH_DEV(ESPI, 5)
#define  PCH_DEV_GBE		_PCH_DEV(ESPI, 6)
#define  PCH_DEV_TRACEHUB	_PCH_DEV(ESPI, 7)

#endif
