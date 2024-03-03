#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
struct gpio_desc *gpio48;
static const struct of_device_id gpio_dt_ids[] = {
    {.compatible = "led,gpio-nam", },
};

static int led_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    gpio48  = gpiod_get(dev, "led48", GPIOD_OUT_LOW);
    gpiod_set_value(gpio48, 1);
    printk(KERN_INFO "%s - %d", __func__, __LINE__);
    return 0;
}
static int led_remove(struct platform_device *pdev)
{
    gpiod_set_value(gpio48, 0);
    gpiod_put(gpio48);
    printk(KERN_INFO "%s - %d", __func__, __LINE__);
    return 0;
}

static struct platform_driver mydrv = {
    .probe  = led_probe,
    .remove = led_remove,
    .driver = {
        .name   = "led,gpio-nam",
        .owner  = THIS_MODULE,
        .of_match_table = of_match_ptr(gpio_dt_ids),
    },
};
module_platform_driver(mydrv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dinh Huu Nam");
MODULE_DESCRIPTION("GPIO Descriptor");
MODULE_VERSION("1.0");