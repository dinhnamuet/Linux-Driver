#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include "lcd_driver.h"

static LIST_HEAD(list);

struct lcd1602 {
	struct i2c_client *client;
	uint8_t backlight;

	char *buffer;
	dev_t dev_num;
	struct class *mclass;
	struct cdev *mcdev;
	struct device *mdevice;
	struct list_head lcd_list;
};
static int lcd_write(struct lcd1602 *lcd, uint8_t data, uint8_t DC);
static void lcd_init(struct lcd1602 *lcd);
static void lcd_goto_xy(struct lcd1602 *lcd, uint8_t x, uint8_t y);
static void clear_lcd(struct lcd1602 *lcd);
static void clear_row(struct lcd1602 *lcd, uint8_t row);
static void lcd_print(struct lcd1602 *lcd, char *data);

static int lcd_open(struct inode *inodep, struct file *filep)
{
	struct lcd1602 *lcd = NULL;
	int status = -ENXIO;
	list_for_each_entry(lcd, &list, lcd_list)
	{
		if(lcd->dev_num == inodep->i_rdev)
		{
			status = 0;
			break;
		}
	}
	if(status)
		return status;
	filep->private_data = lcd;
	return 0;
}
static int lcd_close(struct inode *inodep, struct file *filep)
{
	filep->private_data = NULL;
	return 0;
}
static ssize_t lcd_dev_write(struct file *filep, const char *user_buff, size_t size, loff_t *offset)
{
	struct lcd1602 *lcd = filep->private_data;
	memset(lcd->buffer, 0, 16);
	if (copy_from_user(lcd->buffer, user_buff, size) != 0)
		return -EFAULT;
	lcd_print(lcd, lcd->buffer);
	return size;
}
static long lcd_ioctl(struct file *filep, unsigned int cmd, unsigned long data)
{
	uint8_t row;
	struct position pos;
	struct lcd1602 *lcd = filep->private_data;
	switch(cmd)
	{
		case LCD_INIT:
			lcd_init(lcd);
			break;
		case CLR_LCD:
			clear_lcd(lcd);
			break;
		case CLR_ROW:
			if (!copy_from_user(&row ,(uint8_t *)data, sizeof(uint8_t)))
				clear_row(lcd, row);
			break;
		case GOTO_XY:
			if (!copy_from_user(&pos ,(struct position *)data, sizeof(struct position)))
				lcd_goto_xy(lcd, pos.x, pos.y);
			break;
		default:
			break;
	}
	return 0;
}
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = lcd_open,
	.release = lcd_close,
	.write = lcd_dev_write,
	.unlocked_ioctl = lcd_ioctl
};
static int lcd_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct lcd1602 *lcd = NULL;
	lcd = devm_kzalloc(&client->dev, sizeof(*lcd), GFP_KERNEL);
	if(!lcd)
	{
		printk(KERN_ERR "Fail\n");
		return -1;
	}
	lcd->client = client;
	i2c_set_clientdata(client, lcd);
	lcd->backlight = ON;
	lcd_init(lcd);

	if (alloc_chrdev_region(&lcd->dev_num, 0, 1, DEV_NAME) < 0)
	{
		printk(KERN_ERR "Allocate device number failure!\n");
		return -1;
	}
	lcd->mclass = class_create(THIS_MODULE, DEV_NAME);
	if(lcd->mclass == NULL)
	{
		printk(KERN_INFO "Create class failed\n");
		goto rm_dev_num;
	}
	lcd->mcdev = cdev_alloc();
	lcd->mcdev->owner = THIS_MODULE;
	lcd->mcdev->dev = lcd->dev_num;
	cdev_init(lcd->mcdev, &fops);
	if (cdev_add(lcd->mcdev, lcd->dev_num, 1) < 0)
	{
		printk(KERN_ERR "Cdev add failure\n");
		goto rm_class;
	}
	lcd->mdevice = device_create(lcd->mclass, NULL, lcd->dev_num, NULL, DEV_NAME);
	if (lcd->mdevice == NULL)
	{
		printk(KERN_ERR "create device failure\n");
		goto rm_cdev;
	}
	lcd->buffer = kzalloc(16, GFP_KERNEL);
	if (lcd->buffer == NULL)
	{
		printk(KERN_ERR "Allocate memory failure\n");
		goto rm_device;
	}
	lcd_goto_xy(lcd, 0, 0);
	lcd_print(lcd, "Hello World!");
	lcd_goto_xy(lcd, 0, 1);
	lcd_print(lcd, "LORA DRIVER");	

	INIT_LIST_HEAD(&lcd->lcd_list);
	list_add(&lcd->lcd_list, &list);
	
	printk(KERN_INFO "LCD Driver has been loaded\n");
	return 0;
	
rm_device:
	device_destroy(lcd->mclass, lcd->dev_num);
rm_cdev:
	cdev_del(lcd->mcdev);
rm_class:
	class_destroy(lcd->mclass);
rm_dev_num:
	unregister_chrdev_region(lcd->dev_num, 1);
	return -1;
}
static int lcd_remove(struct i2c_client *client)
{
	struct lcd1602 *lcd = i2c_get_clientdata(client);
	if(!lcd)
	{
		pr_err("Fault\n");
	}
	lcd->backlight = OFF;
	clear_lcd(lcd);
	kfree(lcd->buffer);
	device_destroy(lcd->mclass, lcd->dev_num);
	cdev_del(lcd->mcdev);
	class_destroy(lcd->mclass);
	unregister_chrdev_region(lcd->dev_num, 1);
	printk(KERN_INFO "%s, %d\n", __func__, __LINE__);
	return 0;
}

static const struct i2c_device_id lcd_match_id[] = {
	{"lcd-1602", 0},
	{ }
};
MODULE_DEVICE_TABLE(i2c, lcd_match_id);

static const struct of_device_id lcd_of_match[] = {
	{ .compatible = "lcd1602,nam", },
	{	}
};
MODULE_DEVICE_TABLE(of, lcd_of_match);

static struct i2c_driver lcd_driver = {
	.probe = lcd_probe,
	.remove = lcd_remove,
	.driver = {
		.name = "lcd1602",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(lcd_of_match),
	},
	.id_table = lcd_match_id,
};

module_i2c_driver(lcd_driver);

static int lcd_write(struct lcd1602 *lcd, uint8_t data, uint8_t DC)
{
        uint8_t dataH, dataL;
        uint8_t buff[4];
        dataH = data&0xF0;
        dataL = (data<<4)&0xF0;
	if(lcd->backlight)
	{
		dataH |= BACK_LIGHT;
		dataL |= BACK_LIGHT;
	}
	else
	{
		dataH |= NO_BL;
		dataL |= NO_BL;
	}
        if(DC)
        {
                dataH |= RS;
                dataL |= RS;
        }
        else
        {
                dataH &= ~RS;
                dataL &= ~RS;
        }

        buff[0] = dataH | EN;
        buff[1] = dataH;
        buff[2] = dataL | EN;
        buff[3] = dataL;
        return i2c_master_send(lcd->client, buff, 4);
}
static void lcd_init(struct lcd1602 *lcd)
{
	lcd_write(lcd, 0x33, CMD);
	msleep(10);
	lcd_write(lcd, 0x32, CMD);
	msleep(15);
	lcd_write(lcd, 0x28, CMD); //4 bits interface, 2 line
	msleep(10);
	lcd_write(lcd, 0x01, CMD); //clear screen
	msleep(10);
	lcd_write(lcd, 0x0F, CMD); //on display, on cursor, blink cursor
	msleep(10);
	lcd_write(lcd, 0x02, CMD);// return home
}
static void lcd_goto_xy(struct lcd1602 *lcd, uint8_t x, uint8_t y)
{
	uint8_t data;
	if(y)
		data = 0xC0 + x;
	else
		data = 0x80 + x;
	lcd_write(lcd, data, CMD);
}
static void clear_lcd(struct lcd1602 *lcd)
{
	lcd_write(lcd, 0x01, CMD);
}
static void clear_row(struct lcd1602 *lcd, uint8_t row)
{
	char *foo = "                ";
	lcd_goto_xy(lcd, 0, row);
	lcd_print(lcd, foo);
	lcd_goto_xy(lcd, 0, row);
}
static void lcd_print(struct lcd1602 *lcd, char *data)
{
	while(*data)
	{
		lcd_write(lcd, *data++, DATA);
	}
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("20021163@vnu.edu.vn");
MODULE_DESCRIPTION("I2C Driver for lcd 16x2");
MODULE_VERSION("1.0");
