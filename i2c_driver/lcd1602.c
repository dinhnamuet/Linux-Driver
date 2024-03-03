#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/of.h>

#define DATA		1
#define CMD			0
#define EN 			0x04
#define RW 			0x02
#define RS 			0x01
#define BACK_LIGHT	0x08
#define NO_BL		0x00

struct lcd1602 {
	struct i2c_client *client;
	uint8_t backlight;
};


static int lcd_i2c_write(struct lcd1602 *lcd, uint8_t *buf, int count)
{
	return i2c_master_send(lcd->client, buf, count);
}

static int lcd_i2c_read(struct lcd1602 *lcd, uint8_t *buf, int count)
{
	return i2c_master_recv(lcd->client, buf, count);
}
static void lcd_write(struct lcd1602 *lcd, bool isDATA, uint8_t data)
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
		dataH |= BACK_LIGHT;
		dataL |= BACK_LIGHT;
	}
	if(isDATA)
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
//	lcd_i2c_write(lcd, &buff[0], 1);
//	mdelay(1);
	buff[1] = dataH;
//	lcd_i2c_write(lcd, &buff[1], 1);
	buff[2] = dataL | EN;
//	lcd_i2c_write(lcd, &buff[2], 1);
//	mdelay(1);
	buff[3] = dataL;
	lcd_i2c_write(lcd, buff, 4);
}
static void lcd_goto_xy(struct lcd1602 *lcd, uint8_t x, uint8_t y)
{
	unsigned char Address = 0;
	if(y == 0) 
		Address = 0x80 + x;
	if(y == 1)
		Address = 0xc0 + x;
	lcd_write(lcd, CMD, Address);
}
static void clear_lcd(struct lcd1602 *lcd)
{
	lcd_write(lcd, CMD, 0x01);
	mdelay(5);
}
static void lcd_show(struct lcd1602 *lcd, uint8_t *data)
{
	while(*data)
	{
		lcd_write(lcd, DATA, *data++);
	}
}

static int lcd_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct lcd1602 *lcd;
	lcd = kmalloc(sizeof(*lcd), GFP_KERNEL);
	if(!lcd)
	{
		printk(KERN_ERR "Malloc failed\n");
		return -1;
	}
	lcd->client = client;
	lcd->backlight = 1;
	i2c_set_clientdata(client, lcd);


	lcd_write(lcd, CMD, 0x33);
	mdelay(10);
	lcd_write(lcd, CMD, 0x32);
	mdelay(50);
	lcd_write(lcd, CMD, 0x28);
	mdelay(50);
	lcd_write(lcd, CMD, 0x01);
	mdelay(50);
	lcd_write(lcd, CMD, 0x06);
	mdelay(50);
	lcd_write(lcd, CMD, 0x0C);
	mdelay(50);
	lcd_write(lcd, CMD, 0x02);
	mdelay(50);

	clear_lcd(lcd);
	lcd_goto_xy(lcd, 0, 0);
	lcd_show(lcd, "Hello World\n");
	lcd_goto_xy(lcd, 0, 1);
	lcd_show(lcd, "From I2C Driver\n");
	printk(KERN_INFO "%s, %d\n", __func__, __LINE__);
	return 0;
}

static int lcd_remove(struct i2c_client *client)
{
	struct lcd1602 *lcd = i2c_get_clientdata(client);
	clear_lcd(lcd);
	lcd_goto_xy(lcd, 0, 0);
	lcd_show(lcd, "power off");
	kfree(lcd);
	printk(KERN_INFO "%s, %d\n", __func__, __LINE__);
	return 0;
}

static const struct of_device_id lcd_of_match_id[] = 
{
	{ .compatible = "lcd-iic,nam", },
	{	}
};
static struct i2c_device_id lcd_i2c_match_id[] = {
	{ "lcd-iic,nam", 0},
	{	}
};

MODULE_DEVICE_TABLE(i2c, lcd_i2c_match_id);

static struct i2c_driver lcd_i2c_driver = {
	.probe = lcd_probe,
	.remove = lcd_remove,
	.driver = {
		.name = "lcd-iic,nam",
		.owner = THIS_MODULE,
		.of_match_table = lcd_of_match_id,
	},
	.id_table = lcd_i2c_match_id,
};

module_i2c_driver(lcd_i2c_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("20021163@vnu.edu.vn");
MODULE_DESCRIPTION("LCD 1602");
