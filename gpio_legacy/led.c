/* blink led in gpio 48 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include "led.h"
uint32_t __iomem *base_addr = NULL;
static int __init led_init(void)
{
    base_addr = ioremap(GPIO1_BASE_ADDR, GPIO1_LENGTH);
    if(!base_addr)
        return -ENOMEM;
    iowrite32(*(base_addr + GPIO1_OE_OFFSET / 4) & ~GPIO48, (base_addr + GPIO1_OE_OFFSET / 4));
    iowrite32(*(base_addr + GPIO_SETDATAOUT_OFFSET / 4) | GPIO48, (base_addr + GPIO_SETDATAOUT_OFFSET / 4));
    printk(KERN_INFO "Led is on\n");
    return 0;
}
static void __exit led_exit(void)
{
    iowrite32(*(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) | GPIO48, (base_addr + GPIO_CLEARDATAOUT_OFFSET / 4));
    iounmap(base_addr);
    printk(KERN_INFO "Led is off\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dinh Huu Nam");
MODULE_DESCRIPTION("Legacy GPIO Driver");
MODULE_VERSION("1.0");
