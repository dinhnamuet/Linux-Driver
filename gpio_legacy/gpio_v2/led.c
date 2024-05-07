/* Test controlling LED on GPIO16 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/device.h>

struct gpio_peripherals{
	/* GPIO Function Select */
	u32 GPFSEL0;
	u32 GPFSEL1;
	u32 GPFSEL2;
	u32 GPFSEL3;
	u32 GPFSEL4;
	u32 GPFSEL5;
	u32 padding1;
	/* GPIO Pin Output Set */
	u32 GPSET0;
	u32 GPSET1;
	u32 padding2;
	/* GPIO Pin Output Clear */
	u32 GPCLR0;
	u32 GPCLR1;
	u32 padding3;
	/* GPIO Pin Level */
	u32 GPLEV0;
	u32 GPLEV1;
	u32 padding4;
	/* GPIO Pin Event Detect Status */
	u32 GPEDS0;
	u32 GPEDS1;
	u32 padding5;
	/* GPIO Pin Rising Edge Detect Enable */
	u32 GPREN0;
	u32 GPREN1;
	u32 padding6;
	/* GPIO Pin Falling Edge Detect Enable */
	u32 GPFEN0;
	u32 GPFEN1;
	u32 padding7;
	/* GPIO Pin HIGH Detect Enable */
	u32 GPHEN0;
	u32 GPHEN1;
	/* GPIO Pin LOW Detect Enable */
	u32 GPLEN0;
	u32 GPLEN1;
	u32 padding8;
	/* GPIO Pin Async Rising Edge Detect */
	u32 GPAREN0;
	u32 GPAREN1;
	u32 padding9;
	/* GPIO Pin Async Falling Edge Detect */
	u32 GPAFEN0;
	u32 GPAFEN1;
	u32 padding10;
	/* GPIO Pull-up / Pull-down Register */
	u32 GPIO_PUP_PDN_CNTRL_REG0;
	u32 GPIO_PUP_PDN_CNTRL_REG1;
	u32 GPIO_PUP_PDN_CNTRL_REG2;
	u32 GPIO_PUP_PDN_CNTRL_REG3;
}  ;

struct led_test {
	struct gpio_peripherals __iomem *GPIO;
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
    led->GPIO = devm_ioremap(&pdev->dev, res->start, sizeof(struct gpio_peripherals));
    if(!led->GPIO)
        return -ENOMEM;
    /* config out put */
    regval = ioread32(&led->GPIO->GPFSEL1);
    regval |= 0x01 << 18; //gpio 16 output
    iowrite32(regval, &led->GPIO->GPFSEL1);
    /* turn on the led */
    regval = ioread32(&led->GPIO->GPSET0);
    regval |= 0x01 << 16;
    iowrite32(regval, &led->GPIO->GPSET0);
	pr_info("Led is %s\n", (ioread32(&led->GPIO->GPLEV0) >>16 & 0x01) ? "ON" : "OFF");
    return 0;
}
static int led_remove(struct platform_device *pdev)
{
    u32 regval;
    struct led_test *led = platform_get_drvdata(pdev);
    if(!led)
        return -ENODEV;
    /* Turn off led here */
    regval = ioread32(&led->GPIO->GPCLR0);
    regval |= 0x01 << 16;
    iowrite32(regval, &led->GPIO->GPCLR0);
	pr_info("Led is %s\n", (ioread32(&led->GPIO->GPLEV0) >>16 & 0x01) ? "ON" : "OFF");
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
        .name = "led",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(led_of_match),
    },
};
module_platform_driver(led_drv);

MODULE_LICENSE("GPL");
