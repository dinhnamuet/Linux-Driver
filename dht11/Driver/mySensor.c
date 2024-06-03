/* DHT11 Attach gpio 18 */
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
    struct device *dev;
};

static int sensor_open(struct inode *inodep, struct file *filep);
static int sensor_close(struct inode *inodep, struct file *filep);
static long sensor_ioctl (struct file *filep, unsigned int cmd, unsigned long data);
static dht_return_t dht_read_data(struct mySensor *dht);
static signed long long get_counter_us(void);

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
    dht->dev = &pdev->dev;
    platform_set_drvdata(pdev, dht);
    dht->data_pin = devm_gpiod_get(dht->dev, "dht11", GPIOD_OUT_HIGH_OPEN_DRAIN );
    dht->misc.minor = MISC_DYNAMIC_MINOR;
    dht->misc.name = "dht11";
    dht->misc.fops = &fops;
    dht->misc.mode = 0666;
    dev_info(dht->dev, "%s, %d\n", __func__, __LINE__);
    return misc_register(&dht->misc);
}
static int sensor_remove(struct platform_device *pdev) {
    struct mySensor *dht = platform_get_drvdata(pdev);
    if (!dht)
        return -ENODEV;
    misc_deregister(&dht->misc);
    dev_info(dht->dev, "%s, %d\n", __func__, __LINE__);
    return 0;
}

static const struct of_device_id sensor_of_match[] = {
    { .compatible = "embedded", },
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
static signed long long get_counter_us(void) {
    return ktime_to_us(ktime_get());
}
static dht_return_t dht_read_data(struct mySensor *dht) {
    int i, j;
    u8 check_sum;
    u8 data[5];
    signed long long start_time;
    /* Start Condition */
    gpiod_direction_output(dht->data_pin, HIGH);
    gpiod_set_value(dht->data_pin, LOW);
    mdelay(19);
    gpiod_set_value(dht->data_pin, HIGH);
    /* Wait Response*/
    gpiod_direction_input(dht->data_pin);
    start_time = get_counter_us();
    while (gpiod_get_value(dht->data_pin)) {
        if (get_counter_us() - start_time > 60) {
            dev_err(dht->dev, "Timeout at %d, %lld\n", __LINE__,(get_counter_us() - start_time));
            return -ERRTIMEOUT;
    	};
    }
    /* Check response */
    start_time = get_counter_us();
    while (!gpiod_get_value(dht->data_pin)) {
        if (get_counter_us() - start_time >= 90) {
            dev_err(dht->dev, "Timeout at %d, %lld\n", __LINE__,(get_counter_us() - start_time));
            return -ERRTIMEOUT;
    	};
    }
    if (get_counter_us() - start_time < 70) {
		dev_err(dht->dev, "Timeout at %d, %lld\n", __LINE__,(get_counter_us() - start_time));
        	return -ERRTIMEOUT;
    };
    /* Start reading data */
    start_time = get_counter_us();
    while (gpiod_get_value(dht->data_pin)) {
        if (get_counter_us() - start_time > 90) {
		dev_err(dht->dev, "Timeout at %d, %lld\n", __LINE__,(get_counter_us() - start_time));
        	return -ERRTIMEOUT;
    	};
    }
    for (j = 0; j < 5; j ++) {
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
                reset(data[j], i);
            else
                set(data[j], i);
        }
    }
    dht->mydht.integer_hum = data[0];
    dht->mydht.decimal_hum = data[1];
    dht->mydht.integer_temp = data[2];
    dht->mydht.decimal_temp = data[3];
    check_sum = data[4];
    if ((u8)(data[0] + data[1] + data[2] + data[3]) == data[4])
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
                dev_info(dht->dev, "return: %d\n", dht_read_data(dht));
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
