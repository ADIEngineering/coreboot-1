/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef _SKX_D_CC_IIO_H_
#define _SKX_D_CC_IIO_H_

#include <FspmUpd.h>
#include <soc/pci_devs.h>

enum skxdcc_iio_bifur_table_index {
	Skt0_Iou0 = 0,
	Skt0_Iou1,
};

/*
 * Standard Silicom Ltd. SKX-D-CC Iio Bifurcation Table
 * This is SS 2x16 config.
 */
static const IIO_BIFURCATION_DATA_ENTRY skxdcc_iio_bifur_table[] = {
	{ Iio_Socket0, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 }, /* 1A x16 */
	{ Iio_Socket0, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 }, /* 2A x16 */
};


#if 1 // XXX JJD
#define CFG_UPD_PORT(port,slot,interlock,sltpowersc,sltpowerval,hotplug,vppport,vppadd,ssdcap,ssdvppport,ssdvppadd,hide)			\
	{					\
		.PortIndex		= port, 	\
		.SlotNumber		= slot,		\
		.InterLockPresent	= interlock,	\
		.SlotPowerLimitScale	= sltpowersc,	\
		.SlotPowerLimitValue	= sltpowerval,	\
		.HotPlugCapable		= hotplug,	\
		.VppPort		= vppport,	\
		.VppAddress		= vppadd,	\
		.PcieSSDCapable		= ssdcap,	\
		.PcieSSDVppPort		= ssdvppport,	\
		.PcieSSDVppAddress	= ssdvppadd,	\
		.Hidden			= hide,		\
	}
#else

#define CFG_UPD_PORT(port, hide)				\
	{							\
		.PortIndex           = port,			\
		.HidePort            = hide,			\
		.DeEmphasis          = 0x00,			\
		.PortLinkSpeed       = PcieAuto,		\
		.MaxPayload          = 0x00,			\
		.DfxDnTxPreset       = 0xFF,			\
		.DfxRxPreset         = 0xFF,			\
		.DfxUpTxPreset       = 0xFF,			\
		.Sris                = 0x00,			\
		.PcieCommonClock     = 0x00,			\
		.NtbPpd              = NTB_PORT_TRANSPARENT,	\
		.NtbSplitBar         = 0x00,			\
		.NtbBarSizePBar23    = 0x16,			\
		.NtbBarSizePBar4     = 0x16,			\
		.NtbBarSizePBar5     = 0x16,			\
		.NtbBarSizePBar45    = 0x16,			\
		.NtbBarSizeSBar23    = 0x16,			\
		.NtbBarSizeSBar4     = 0x16,			\
		.NtbBarSizeSBar5     = 0x16,			\
		.NtbBarSizeSBar45    = 0x16,			\
		.NtbSBar01Prefetch   = 0x00,			\
		.NtbXlinkCtlOverride = 0x03,			\
	}
#endif

/*
 * Standard Silicom Ltd. SKX-D-CC Iio PCIe Port Table
 */
#if 1 // XXX JJD
static const IIO_SLOT_CONFIG_DATA_ENTRY skxdcc_iio_pci_port_skt0[] = {
	CFG_UPD_PORT(PORT_1A_INDEX, 1, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, NOT_HIDE),
	CFG_UPD_PORT(PORT_1B_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, HIDE),
	CFG_UPD_PORT(PORT_1C_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, HIDE),
	CFG_UPD_PORT(PORT_1D_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, HIDE),
	CFG_UPD_PORT(PORT_2A_INDEX, 2, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, NOT_HIDE),
	CFG_UPD_PORT(PORT_2B_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, HIDE),
	CFG_UPD_PORT(PORT_2C_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, HIDE),
	CFG_UPD_PORT(PORT_2D_INDEX, NO_SLT_IMP, DISABLE, PWR_SCL_MAX, PWR_VAL_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, DISABLE, VPP_PORT_MAX, VPP_ADDR_MAX, HIDE),
};

#else
static const UPD_PCI_PORT_CONFIG skxdcc_iio_pci_port_skt0[] = {
	CFG_UPD_PORT(PORT_1A, NOT_HIDE),
	CFG_UPD_PORT(PORT_1B, HIDE),
	CFG_UPD_PORT(PORT_1C, HIDE),
	CFG_UPD_PORT(PORT_1D, HIDE),
	CFG_UPD_PORT(PORT_2A, NOT_HIDE),
	CFG_UPD_PORT(PORT_2B, HIDE),
	CFG_UPD_PORT(PORT_2C, HIDE),
	CFG_UPD_PORT(PORT_2D, HIDE),
};
#endif
#if 0
/*
 * Standard Silicom Ltd. SKX-D-CC PCH PCIe Port Table
 */
static const UPD_PCH_PCIE_PORT skxdcc_pch_pci_port_skt0[] = {
	//PortIndex ; ForceEnable ; PortLinkSpeed
	{ 0x00, 0x00, PcieAuto },
	{ 0x04, 0x00, PcieAuto },
	{ 0x05, 0x00, PcieAuto },
};
#endif

#endif /* _SKX_D_CC_IIO_H_ */
