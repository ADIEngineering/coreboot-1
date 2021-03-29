#include <acpi/acpi.h>
#include <device/device.h>

static void mainboard_init(struct device *dev)
{
}

static void mainboard_enable(struct device *dev)
{
	dev->ops->init = mainboard_init;
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};
