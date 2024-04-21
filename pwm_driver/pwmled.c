#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/pwm.h>
#include <linux/moduleparam.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "pwmled.h"

u32 f_led1 = 1000;
u32 f_led2 = 100;

module_param(f_led1, uint, S_IRUGO);
module_param(f_led2, uint, S_IRUGO);
MODULE_PARM_DESC(f_led1, "PWM frequency of led 1 (in HZ)");
MODULE_PARM_DESC(f_led2, "PWM frequency of led 2 (in HZ)");

struct pwmled {
    struct pwm_device *led1;
    struct pwm_device *led2;
    struct miscdevice misc;
    u32 p_led1;
    u32 p_led2;
};
static int set_duty_cycle(struct pwm_device *dev, u8 duty_cycle)
{
    u32 duty_to_time;
    u32 res;
    if(duty_cycle > 100)
        return -1;
    duty_to_time = duty_cycle * pwm_get_period(dev) / 100;
    res = pwm_config(dev, duty_to_time, pwm_get_period(dev));
    pr_info("%s has duty_cycle is %lld, period is %lld", dev->label, pwm_get_duty_cycle(dev), pwm_get_period(dev));
    return res;
}
static int led_open(struct inode *inodep, struct file *filep)
{
    struct pwmled *leds = container_of(filep->private_data, struct pwmled, misc);
    if(!leds)
        return -ENODEV;
    filep->private_data = leds;
    return 0;
}
static int led_close(struct inode *inodep, struct file *filep)
{
    filep->private_data = NULL;
    return 0;
}
static long led_ioctl(struct file *filep, unsigned int cmd, unsigned long data) 
{
    u8 dc;
    struct pwmled *leds = filep->private_data;
    switch(cmd)
    {
        case LED1:
            if(!copy_from_user(&dc, (u8 *)data, sizeof(u8)))
                set_duty_cycle(leds->led1, dc);
            break;
        case LED2:
            if(!copy_from_user(&dc, (u8 *)data, sizeof(u8)))
                set_duty_cycle(leds->led2, dc);
            break;
        default:
            break;
    }
    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .release = led_close,
    .unlocked_ioctl = led_ioctl,
};

static int led_probe(struct platform_device *pdev)
{
    struct pwmled *leds;
    leds = devm_kzalloc(&pdev->dev, sizeof(*leds), GFP_KERNEL);
    if(!leds)
        return -ENOMEM;
    platform_set_drvdata(pdev, leds);
    leds->led1 = pwm_get(&pdev->dev, "led1");
    leds->led2 = pwm_get(&pdev->dev, "led2");
    if(!leds->led1 || !leds->led2)
        return -ENODEV;
    leds->p_led1 = 1000000000UL/f_led1;
    leds->p_led2 = 1000000000UL/f_led2;
    pwm_config(leds->led1, 0, leds->p_led1);
    pwm_config(leds->led2, 0, leds->p_led2);
    pr_info("Led 1 period is: %lld\n", leds->led1->state.period);
    pr_info("Led 2 period is: %lld\n", pwm_get_period(leds->led2));
    pwm_enable(leds->led1);
    pwm_enable(leds->led2);
    
    leds->misc.minor = MISC_DYNAMIC_MINOR;
    leds->misc.this_device = &pdev->dev;
    leds->misc.fops = &fops;
    leds->misc.mode = 0666;
    leds->misc.name = "leds";
    misc_register(&leds->misc);

    return 0;
}
static int led_remove(struct platform_device *pdev)
{
    struct pwmled *leds = platform_get_drvdata(pdev);
    if(!leds)
        return -ENODEV;
    misc_deregister(&leds->misc);
    pwm_config(leds->led1, 0, leds->p_led1);
    pwm_config(leds->led2, 0, leds->p_led2);
    pwm_disable(leds->led1);
    pwm_disable(leds->led2);
    pwm_put(leds->led1);
    pwm_put(leds->led2);
    pr_info("Goodbye!\n");
    return 0;
}
static const struct of_device_id my_led_of_match[] = {
    { .compatible = "pwmled", },
    { }
};
MODULE_DEVICE_TABLE(of, my_led_of_match);
static struct platform_driver led_driver = {
    .probe = led_probe,
    .remove = led_remove,
    .driver = {
        .name = "pwmled",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(my_led_of_match),
    },
};
module_platform_driver(led_driver);

MODULE_LICENSE("GPL");