/* DHT11 Attach gpio 26 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include "mySensor.h"
#define set(var, bit) var |= (1UL << bit)
#define reset(var, bit) var &= ~(1UL << bit)
struct mySensor {
    struct gpio_desc *data_pin;
    dht_sensor_t mydht;
    struct miscdevice misc;
};
static int sensor_open(struct inode *inodep, struct file *filep);
static int sensor_close(struct inode *inodep, struct file *filep);
static long sensor_ioctl (struct file *filep, unsigned int cmd, unsigned long data);
static dht_return_t dht_read_data(struct mySensor *dht);
static s64 get_counter_us(void);
struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = sensor_open,
    .release = sensor_close,
    .unlocked_ioctl = sensor_ioctl,
};

static int sensor_probe(struct platform_device *pdev) {
    struct mySensor *dht;
    dht = devm_kzalloc(&pdev->dev, sizeof(*dht), GFP_KERNEL);
    if (!dht)
        return -ENOMEM;
    platform_set_drvdata(pdev, dht);
    dht->data_pin = devm_gpiod_get(&pdev->dev, "dht11", GPIOD_OUT_HIGH);
    gpiod_set_value(dht->data_pin, HIGH);
    dht->misc.minor = MISC_DYNAMIC_MINOR; // dynamic allocator minor number
    dht->misc.name = "dht11";            // name of device file
    dht->misc.fops = &fops;
    dht->misc.mode = 0666;
    pr_info("%s, %d\n", __func__, __LINE__);
    return misc_register(&dht->misc);
}
static int sensor_remove(struct platform_device *pdev) {
    struct mySensor *dht = platform_get_drvdata(pdev);
    if (!dht)
        return -ENODEV;
    misc_deregister(&dht->misc);
    pr_info("%s, %d\n", __func__, __LINE__);
    return 0;
}

static const struct of_device_id sensor_of_match[] = {
    { .compatible = "bar,dht11", },
    { }
};
MODULE_DEVICE_TABLE(of, sensor_of_match);

static struct platform_driver sensorDrv = {
    .probe = sensor_probe,
    .remove = sensor_remove,
    .driver = {
        .name = "Sensor",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(sensor_of_match),
    },
};
static s64 get_counter_us(void) {
    return ktime_to_us(ktime_get());
}

static dht_return_t dht_read_data(struct mySensor *dht) {
    int i;
    u8 check_sum;
    s64 start_time;
    /* Start Condition */
    gpiod_direction_output(dht->data_pin, 1);
    gpiod_set_value(dht->data_pin, LOW);
    msleep(18);
    gpiod_set_value(dht->data_pin, HIGH);
    /* Wait Response*/
    gpiod_direction_input(dht->data_pin);
    start_time = get_counter_us();
    while(!gpiod_get_value(dht->data_pin)) {
        if (get_counter_us() - start_time > 90)
        return -ERRTIMEOUT;
    }
    start_time = get_counter_us();
    while (gpiod_get_value(dht->data_pin)) {
        if (get_counter_us() - start_time > 90)
        return -ERRTIMEOUT;
    }
    start_time = get_counter_us();
    while (!gpiod_get_value(dht->data_pin)) {
        if (get_counter_us() - start_time >= 90)
            return -ERRTIMEOUT;
    }
    if (get_counter_us() - start_time < 60)
        return -ERRTIMEOUT;
    /* Start reading data */
    start_time = get_counter_us();
    while (gpiod_get_value(dht->data_pin)) {
        if (get_counter_us() - start_time > 90)
        return -ERRTIMEOUT;
    }
    for (i = 7; i >= 0; i--) {
        start_time = get_counter_us();
        while (gpiod_get_value(dht->data_pin) == 0) {
            if(get_counter_us() - start_time > 60)
                return -ERRTIMEOUT;
        }
        start_time = get_counter_us();
        while (gpiod_get_value(dht->data_pin) == 1) {
            if(get_counter_us() - start_time > 90)
                return -ERRTIMEOUT;
        }
        if (get_counter_us() - start_time < 40)
            reset(dht->mydht.integer_hum, i);
        else
            set(dht->mydht.integer_hum, i);
    }
    for (i = 7; i >= 0; i--) {
        start_time = get_counter_us();
        while (gpiod_get_value(dht->data_pin) == 0) {
            if(get_counter_us() - start_time > 60)
                return -ERRTIMEOUT;
        }
        start_time = get_counter_us();
        while (gpiod_get_value(dht->data_pin) == 1) {
            if(get_counter_us() - start_time > 90)
                return -ERRTIMEOUT;
        }
        if (get_counter_us() - start_time < 40)
            reset(dht->mydht.decimal_hum, i);
        else
            set(dht->mydht.decimal_hum, i);
    }
    for (i = 7; i >= 0; i--) {
        start_time = get_counter_us();
        while (gpiod_get_value(dht->data_pin) == 0) {
            if(get_counter_us() - start_time > 60)
                return -ERRTIMEOUT;
        }
        start_time = get_counter_us();
        while (gpiod_get_value(dht->data_pin) == 1) {
            if(get_counter_us() - start_time > 90)
                return -ERRTIMEOUT;
        }
        if (get_counter_us() - start_time < 40)
            reset(dht->mydht.integer_temp, i);
        else
            set(dht->mydht.integer_temp, i);
    }
    for (i = 7; i >= 0; i--)  {
        start_time = get_counter_us();
        while (gpiod_get_value(dht->data_pin) == 0) {
            if(get_counter_us() - start_time > 60)
                return -ERRTIMEOUT;
        }
        start_time = get_counter_us();
        while (gpiod_get_value(dht->data_pin) == 1) {
            if(get_counter_us() - start_time > 90)
                return -ERRTIMEOUT;
        }
        if (get_counter_us() - start_time < 40)
            reset(dht->mydht.decimal_temp, i);
        else
            set(dht->mydht.decimal_temp, i);
    }
    for (i = 7; i >= 0; i--) {
        start_time = get_counter_us();
        while (gpiod_get_value(dht->data_pin) == 0) {
            if(get_counter_us() - start_time > 60)
                return -ERRTIMEOUT;
        }
        start_time = get_counter_us();
        while (gpiod_get_value(dht->data_pin) == 1){
            if(get_counter_us() - start_time > 90)
                return -ERRTIMEOUT;
        }
        if (get_counter_us() - start_time < 40)
            reset(check_sum, i);
        else
            set(check_sum, i);
    }
    if (dht->mydht.integer_temp + dht->mydht.decimal_temp + dht->mydht.integer_hum + dht->mydht.decimal_hum == check_sum)
        return READSUCCESS;
    else
        return -ERCHECKSUM;
}
static int sensor_open(struct inode *inodep, struct file *filep) {
    struct mySensor *dht11 = container_of(filep->private_data, struct mySensor, misc);
    if (dht11) {
        filep->private_data = dht11;
        return 0;
    } else
        return -EFAULT;
}
static int sensor_close(struct inode *inodep, struct file *filep) {
    filep->private_data = 0;
    return 0;
}
static long sensor_ioctl (struct file *filep, unsigned int cmd, unsigned long data) {
    struct mySensor *dht = filep->private_data;
    long res;
    if(dht) {
        switch (cmd) {
            case READ_DHT:
                pr_info("sensor return: %d\n", dht_read_data(dht));
                res = copy_to_user((dht_sensor_t *)data, &dht->mydht, sizeof(dht_sensor_t));
                break;
            default:
                break;
        }
        return res;
    } else
        return -EFAULT;
}
module_platform_driver(sensorDrv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DinhNam <20021163@vnu.edu.vn>");
MODULE_DESCRIPTION("DHT11 Driver");

// static int __init sensorInit(void)
// {
//     return platform_driver_register(sensorDrv);
// }
// module_init(sensorInit);
// static void __exit sensorExit(void)
// {
//     return platform_driver_unregister(sensorDrv);
// }
// module_exit(sensorExit);
