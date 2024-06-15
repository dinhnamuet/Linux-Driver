#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include "lcd.h"

static struct kobject *lcd_obj;
static int lcd_write(lcd *mylcd, u8 data, wmode_t DC);
static void lcd_init(lcd *mylcd);
static void lcd_goto_xy(lcd *mylcd, u8 x, u8 y);
static void clear_lcd(lcd *mylcd);
static void lcd_print(lcd *mylcd, const char *data, u8 len);

static ssize_t line1_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
static ssize_t line2_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
static ssize_t clear_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

static int lcd_probe(struct i2c_client *client) {
    lcd *mylcd;
    mylcd = devm_kzalloc(&client->dev, sizeof(*mylcd), GFP_KERNEL);
    if (!mylcd)
	    return -ENOMEM;

    lcd_obj = kobject_create_and_add("lcd1602", NULL);
    mylcd->line1.attr.name = "line1";
    mylcd->line1.attr.mode = 0222;
    mylcd->line1.store = line1_store;

    mylcd->line2.attr.name = "line2";
    mylcd->line2.attr.mode = 0222;
    mylcd->line2.store = line2_store;

    mylcd->clear.attr.name = "clear";
    mylcd->clear.attr.mode = 0222;
    mylcd->clear.store = clear_store;

    const struct attribute * const my_lcd[] = {
	    &mylcd->line1.attr,
	    &mylcd->line2.attr,
	    &mylcd->clear.attr,
	    NULL,
    };

    mylcd->client = client;
    i2c_set_clientdata(client, mylcd);
    mylcd->backlight = ON;
    lcd_init(mylcd);
    lcd_print(mylcd, "Hello World!", strlen("Hello World!"));
    lcd_goto_xy(mylcd, 0, 1);
    lcd_print(mylcd, "Linux Kernel", strlen("Linux Kernel"));
    dev_info(&client->dev, "my lcd is starting!\n");
    return sysfs_create_files(lcd_obj, my_lcd);
}

static void lcd_remove(struct i2c_client *client) {
    lcd *mylcd = i2c_get_clientdata(client);
    if (mylcd) {
        mylcd->backlight = OFF;
        clear_lcd(mylcd);

        sysfs_remove_file(lcd_obj, &mylcd->line1.attr);
	sysfs_remove_file(lcd_obj, &mylcd->line2.attr);
	sysfs_remove_file(lcd_obj, &mylcd->clear.attr);
    }
    kobject_put(lcd_obj);
}

static const struct of_device_id lcd_of_match[] = {
    { .compatible = "lcd1602", },
    { }
};
MODULE_DEVICE_TABLE(of, lcd_of_match);

static const struct i2c_device_id lcd_id[] = {
    { "lcd", },
    { }
};
MODULE_DEVICE_TABLE(i2c, lcd_id);

static struct i2c_driver lcdDrv = {
    .probe = lcd_probe,
    .remove = lcd_remove,
    .driver = {
        .name = "lcd",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(lcd_of_match),
    },
    .id_table = lcd_id,
};

static int lcd_write(lcd *mylcd, u8 data, wmode_t DC) {
    u8 dataH, dataL;
    u8 buff[4];
    dataH = data & 0xF0;
    dataL = (data << 4) & 0xF0;
    if (mylcd->backlight) {
        dataH |= BACKLIGHT;
        dataL |= BACKLIGHT;
    }
    else {
        dataH |= NOBACKLIGHT;
        dataL |= NOBACKLIGHT;
    }
    if (DC == DATA) {
        dataH |= RS;
        dataL |= RS;
    }
    else {
        dataH &= ~RS;
        dataL &= ~RS;
    }

    buff[0] = dataH | EN;
    buff[1] = dataH;
    buff[2] = dataL | EN;
    buff[3] = dataL;
    return i2c_master_send(mylcd->client, buff, 4);
}
static void lcd_init(lcd *mylcd) {
    lcd_write(mylcd, 0x33, COMMAND);
    msleep(10);
    lcd_write(mylcd, 0x32, COMMAND);
    msleep(15);
    lcd_write(mylcd, 0x28, COMMAND); // 4 bits interface, 2 mylcd->line
    msleep(10);
    lcd_write(mylcd, 0x01, COMMAND); // clear screen
    msleep(10);
    lcd_write(mylcd, 0x0C, COMMAND); // on display, off cursor
    msleep(10);
    lcd_write(mylcd, 0x02, COMMAND); // return mylcd
    msleep(10);
    clear_lcd(mylcd);
    lcd_goto_xy(mylcd, 0, 0);
    msleep(10);
}
static void lcd_goto_xy(lcd *mylcd, u8 x, u8 y) {
    u8 data;
    if (y)
        data = 0xC0 + x;
    else
        data = 0x80 + x;
    lcd_write(mylcd, data, COMMAND);
}
static void clear_lcd(lcd *mylcd) {
    lcd_write(mylcd, 0x01, COMMAND);
}
static void lcd_print(lcd *mylcd, const char *data, u8 len) {
    u8 i;
    for (i = 0; i < len; i++) {
	if (data[i])
        	lcd_write(mylcd, data[i], DATA);
    }
}

static ssize_t line1_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    	lcd *mylcd = container_of(attr, lcd, line1);
    	if (!mylcd)
	    return -EFAULT;
	lcd_goto_xy(mylcd, 0, 0);
    	lcd_print(mylcd, buf, count);
    	return count;
}
static ssize_t line2_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    lcd *mylcd = container_of(attr, lcd, line2);
    if (!mylcd)
        return -EFAULT;
    lcd_goto_xy(mylcd, 0, 1);
    lcd_print(mylcd, buf, count);
    return count;
}
static ssize_t clear_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    lcd *mylcd = container_of(attr, lcd, clear);
    if (!mylcd)
        return -EFAULT;
    clear_lcd(mylcd);
    return count;
}

module_i2c_driver(lcdDrv);

MODULE_LICENSE("GPL");
