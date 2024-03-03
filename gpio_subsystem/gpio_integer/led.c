#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/fs.h>

static int __init led_init(void)
{
    if (!gpio_is_valid(48))
    {
        printk(KERN_INFO "Invalid GPIO48\n");
        return -ENODEV;
    }
    gpio_request(48, "led");
    gpio_direction_output(48, 0);
    gpio_set_value(48, 1);
    printk(KERN_INFO "Led is on\n");
    return 0;
}
static void __exit led_exit(void)
{
    gpio_set_value(48, 0);
    gpio_free(48);
    printk(KERN_INFO "Led is off\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dinh Huu Nam");
MODULE_DESCRIPTION("Legacy GPIO Driver");
MODULE_VERSION("1.0");
