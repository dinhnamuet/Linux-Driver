#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include "lcd_driver.h"
struct lcd1602
{
	struct i2c_client *client;
	uint8_t backlight;
	char *buffer;
	struct miscdevice misdev;
	struct position pos;
};

static int lcd_write(struct lcd1602 *lcd, uint8_t data, uint8_t DC);
static void lcd_init(struct lcd1602 *lcd);
static void lcd_goto_xy(struct lcd1602 *lcd, uint8_t x, uint8_t y);
static void clear_lcd(struct lcd1602 *lcd);
static void clear_row(struct lcd1602 *lcd, uint8_t row);
static void clear_last_index(struct lcd1602 *lcd);
static void lcd_print(struct lcd1602 *lcd, char *data);

static int lcd_open(struct inode *inodep, struct file *filep)
{
	struct lcd1602 *lcd = NULL;
	lcd = container_of(filep->private_data, struct lcd1602, misdev);
	if (lcd)
		filep->private_data = lcd;
	return 0;
}
static int lcd_close(struct inode *inodep, struct file *filep)
{
	filep->private_data = NULL;
	return 0;
}
static long lcd_ioctl(struct file *filep, unsigned int cmd, unsigned long data)
{
	uint8_t foo;
	struct position pos;
	struct lcd1602 *lcd = filep->private_data;
	switch (cmd)
	{
	case CLR_LCD:
		clear_lcd(lcd);
		break;
	case CLR_ROW:
		if (!copy_from_user(&foo, (uint8_t *)data, sizeof(uint8_t)))
			clear_row(lcd, foo);
		break;
	case GOTO_XY:
		if (!copy_from_user(&pos, (struct position *)data, sizeof(struct position)))
			lcd_goto_xy(lcd, pos.x, pos.y);
		break;
	case LCD_SEND_CHAR:
		memset(lcd->buffer, 0, 16);
		if (!copy_from_user(lcd->buffer, (uint8_t *)data, strlen((uint8_t *)data)))
		{
			if (lcd->buffer[strlen(lcd->buffer) - 1] == 0x01)
				lcd->buffer[strlen(lcd->buffer) - 1] = 0;
			lcd_print(lcd, lcd->buffer);
		}
		pr_info("%s\n", lcd->buffer);
		break;
	case CLR_LAST:
		clear_last_index(lcd);
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
	.unlocked_ioctl = lcd_ioctl};
static int lcd_probe(struct i2c_client *client)
{
	struct lcd1602 *lcd = NULL;
	lcd = devm_kzalloc(&client->dev, sizeof(*lcd), GFP_KERNEL);
	if (!lcd)
	{
		printk(KERN_ERR "Fail\n");
		return -1;
	}
	lcd->client = client;
	i2c_set_clientdata(client, lcd);
	lcd->backlight = ON;
	lcd_init(lcd);
	lcd->buffer = kzalloc(16, GFP_KERNEL);
	if (lcd->buffer == NULL)
	{
		printk(KERN_ERR "Allocate memory failure\n");
		return -1;
	}
	lcd->misdev.minor = MISC_DYNAMIC_MINOR;
	lcd->misdev.name = "lcd1602";
	lcd->misdev.fops = &fops;
	if (misc_register(&lcd->misdev) < 0)
	{
		printk(KERN_ERR "Misc Register failed\n");
		kfree(lcd->buffer);
		return -1;
	}
	lcd_goto_xy(lcd, 0, 0);
	lcd_print(lcd, "   CALCULATOR   ");
	lcd_goto_xy(lcd, 0, 1);
	lcd_print(lcd, "  HUY TUAN BUI  ");
	return 0;
}
static void lcd_remove(struct i2c_client *client)
{
	struct lcd1602 *lcd = i2c_get_clientdata(client);
	if (!lcd)
	{
		pr_err("Fault\n");
	}
	else
	{
		lcd->backlight = OFF;
		clear_lcd(lcd);
		kfree(lcd->buffer);
		misc_deregister(&lcd->misdev);
		printk(KERN_INFO "%s, %d\n", __func__, __LINE__);
	}
}

static const struct of_device_id lcd_of_match[] = {
	{ .compatible = "lcd1602", },
	{ }
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
};

module_i2c_driver(lcd_driver);

static int lcd_write(struct lcd1602 *lcd, uint8_t data, uint8_t DC)
{
	uint8_t dataH, dataL;
	uint8_t buff[4];
	dataH = data & 0xF0;
	dataL = (data << 4) & 0xF0;
	if (lcd->backlight)
	{
		dataH |= BACK_LIGHT;
		dataL |= BACK_LIGHT;
	}
	else
	{
		dataH |= NO_BL;
		dataL |= NO_BL;
	}
	if (DC)
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
	lcd_write(lcd, 0x28, CMD); // 4 bits interface, 2 line
	msleep(10);
	lcd_write(lcd, 0x01, CMD); // clear screen
	msleep(10);
	lcd_write(lcd, 0x0F, CMD); // on display, on cursor, blink cursor
	msleep(10);
	lcd_write(lcd, 0x02, CMD); // return home
	msleep(50);
}
static void lcd_goto_xy(struct lcd1602 *lcd, uint8_t x, uint8_t y)
{
	uint8_t data;
	if (y)
		data = 0xC0 + x;
	else
		data = 0x80 + x;
	lcd_write(lcd, data, CMD);
	lcd->pos.x = x;
	lcd->pos.y = y;
}
static void clear_lcd(struct lcd1602 *lcd)
{
	lcd_write(lcd, 0x01, CMD);
	lcd_goto_xy(lcd, 0, 0);
}
static void clear_row(struct lcd1602 *lcd, uint8_t row)
{
	u8 x, y;
	x = lcd->pos.x;
	y = lcd->pos.y;
	char *foo = "                ";
	lcd_goto_xy(lcd, 0, row);
	lcd_print(lcd, foo);
	lcd_goto_xy(lcd, 0, row);
}
static void lcd_print(struct lcd1602 *lcd, char *data)
{
	while (*data)
	{
		lcd_write(lcd, *data++, DATA);
		lcd->pos.x += 1;
	}
}
static void clear_last_index(struct lcd1602 *lcd)
{
	lcd_goto_xy(lcd, lcd->pos.x - 1, lcd->pos.y);
	lcd_print(lcd, " ");
	lcd_goto_xy(lcd, lcd->pos.x - 1, lcd->pos.y);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DinhNam <20021163@vnu.edu.vn>");
MODULE_DESCRIPTION("I2C Driver for lcd 16x2");
MODULE_VERSION("1.0");
