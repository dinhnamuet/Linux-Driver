#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include "lcd.h"

struct lcd1602 {
	struct i2c_client *client;
	uint8_t backlight;
};
static int lcd_write(struct lcd1602 *lcd, uint8_t data, uint8_t DC);
static void lcd_init(struct lcd1602 *lcd);
static void lcd_goto_xy(struct lcd1602 *lcd, uint8_t x, uint8_t y);
static void clear_lcd(struct lcd1602 *lcd);
static void clear_row(struct lcd1602 *lcd, uint8_t row);
static void lcd_print(struct lcd1602 *lcd, char *data);

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

	lcd_goto_xy(lcd, 0, 0);
	lcd_print(lcd, "Hello World!");
	lcd_goto_xy(lcd, 0, 1);
	lcd_print(lcd, "I2C driver");	
	
	printk(KERN_INFO "%s, %d\n", __func__, __LINE__);
	return 0;
}
static void lcd_remove(struct i2c_client *client)
{
	struct lcd1602 *lcd = i2c_get_clientdata(client);
	lcd->backlight = OFF;
	clear_lcd(lcd);
	printk(KERN_INFO "%s, %d\n", __func__, __LINE__);
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
