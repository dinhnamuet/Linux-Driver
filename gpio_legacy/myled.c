/* Test controlling LED on GPIO16 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/io.h>
#include "myled.h"
struct led_test {
    u32 __iomem *base;
};

static int led_probe(struct platform_device *pdev)
{
    u32 regval;
    struct led_test *led;
    struct resource *res;
    led = devm_kzalloc(&pdev->dev, sizeof(*led), GFP_KERNEL);
    if(!led)
        return -ENOMEM;
    platform_set_drvdata(pdev, led);
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if(!res)
        return -ENODEV;
    led->base = (u32 *)ioremap(res->start, 0xF0);
    if(!led->base)
        return -ENOMEM;
    /* config out put */
    regval = ioread32(led->base + GPFSEL1_OFFSET/4);
    regval |= 0x01 << 18; //gpio 16 output
    iowrite32(regval, led->base + GPFSEL1_OFFSET/4);
    /* turn on the led */
    regval = ioread32(led->base + GPSET0_OFFSET/4);
    regval |= 0x01 << 16;
    iowrite32(regval, led->base + GPSET0_OFFSET/4);
    pr_info("Led is on!\n");
    return 0;
}
static int led_remove(struct platform_device *pdev)
{
    u32 regval;
    struct led_test *led = platform_get_drvdata(pdev);
    if(!led)
        return -ENODEV;
    /* Turn off led here */
    regval = ioread32(led->base + GPCLR0_OFFSET/4);
    regval |= 0x01 << 16;
    iowrite32(regval, led->base + GPCLR0_OFFSET/4);
    iounmap(led->base);
    pr_info("Led is off!\n");
    return 0;
}

static struct of_device_id led_of_match[] = {
    { .compatible = "myled", },
    { }
};
MODULE_DEVICE_TABLE(of, led_of_match);

struct platform_driver led_drv = {
    .probe = led_probe,
    .remove = led_remove,
    .driver = {
        .name = "ledtest",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(led_of_match),
    },
};
module_platform_driver(led_drv);

MODULE_LICENSE("GPL");
