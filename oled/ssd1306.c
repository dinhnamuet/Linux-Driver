#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include "ssd1306.h"
const int max_X = OLED_WIDTH / FONT_X - 1;
const int max_Y = OLED_HEIGHT / 8 - 1;

struct ssd1306
{
	struct i2c_client *client;
	uint8_t current_X;
	uint8_t current_Y;
	uint8_t *buffer;
	struct work_struct workqueue;
	struct timer_list my_timer;
};

static void ssd1306_write(struct ssd1306 *oled, uint8_t data, write_mode_t mode);
static void ssd1306_init(struct ssd1306 *oled);
static void ssd1306_clear(struct ssd1306 *oled);
static void ssd1306_goto_xy(struct ssd1306 *oled, uint8_t x, uint8_t y);
static void ssd1306_send_char(struct ssd1306 *oled, uint8_t data);
static void ssd1306_send_char_inv(struct ssd1306 *oled, uint8_t data);
static void ssd1306_send_string(struct ssd1306 *oled, uint8_t *str, color_t color);
static void ssd1306_go_to_next_line(struct ssd1306 *oled);
static int ssd1306_draw_pixel(struct ssd1306 *oled, uint8_t x, uint8_t y, color_t color);
static void ssd1306_draw_bitmap(struct ssd1306 *oled, uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t width, uint8_t height, color_t color);
static int ssd1306_burst_write(struct ssd1306 *oled, const uint8_t *data, int len, write_mode_t mode);
static void ssd1306_sync(struct ssd1306 *oled);
static void animation(struct work_struct *work);
static void tmHandler(struct timer_list *tm);

static int oled_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct ssd1306 *oled = NULL;
	oled = devm_kzalloc(&client->dev, sizeof(*oled), GFP_KERNEL);
	if (!oled)
	{
		pr_err("kzalloc failed\n");
		return -1;
	}
	oled->client = client;
	i2c_set_clientdata(client, oled);
	oled->buffer = kzalloc(OLED_WIDTH * OLED_HEIGHT / 8, GFP_KERNEL);
	if(!oled->buffer)
	{
		pr_err("kzalloc failed\n");
		return -1;
	}
	ssd1306_init(oled);
	INIT_WORK(&oled->workqueue, animation);
	timer_setup(&oled->my_timer, tmHandler, 0);
	oled->my_timer.expires = jiffies + HZ/2;
	add_timer(&oled->my_timer);
	return 0;
}
static void oled_remove(struct i2c_client *client)
{
	struct ssd1306 *oled = i2c_get_clientdata(client);
	if (!oled)
	{
		pr_err("Cannot get data\n");
	}
	else
	{
		cancel_work_sync(&oled->workqueue);
		del_timer(&oled->my_timer);
		ssd1306_clear(oled);
		kfree(oled->buffer);
		ssd1306_write(oled, 0xAE, COMMAND); // display off
	}
}
static const struct i2c_device_id oled_device_id[] = {
	{ .name = "nam", 0 },
	{}
};
MODULE_DEVICE_TABLE(i2c, oled_device_id);
static const struct of_device_id oled_of_match_id[] = {
	{ .compatible = "ssd1306-oled,nam", },
	{}
};
MODULE_DEVICE_TABLE(of, oled_of_match_id);

static struct i2c_driver oled_driver = {
	.probe = oled_probe,
	.remove = oled_remove,
	.driver = {
		.name = "oled",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(oled_of_match_id),
	},
	.id_table = oled_device_id,
};

module_i2c_driver(oled_driver);

static void ssd1306_write(struct ssd1306 *oled, uint8_t data, write_mode_t mode)
{
	/*
	A control byte mainly consists of Co and D/C# bits following by six “0”
	Co | D/C | 000000
	Co bit is equal to 0
	*/
	uint8_t buff[2];
	if (mode == DATA)
		buff[0] = 0x40; // data
	else
		buff[0] = 0x00; // command
	buff[1] = data;
	i2c_master_send(oled->client, buff, 2);
}
static int ssd1306_burst_write(struct ssd1306 *oled, const uint8_t *data, int len, write_mode_t mode)
{
	int res;
	uint8_t *buff;
	buff = kmalloc(len + 1, GFP_KERNEL);
	if(!buff)
		return -1;
	memset(buff, 0, len + 1);
	if (mode == DATA)
		buff[0] = 0x40; // data
	else
		buff[0] = 0x00; // command
	memcpy(&buff[1], data, len);
	res = i2c_master_send(oled->client, buff, len + 1);
	kfree(buff);
	return res;
}
static void ssd1306_init(struct ssd1306 *oled)
{
	msleep(15);
	// set Osc Frequency
	ssd1306_write(oled, 0xD5, COMMAND);
	ssd1306_write(oled, 0x80, COMMAND);
	// set MUX Ratio
	ssd1306_write(oled, 0xA8, COMMAND);
	ssd1306_write(oled, 0x3F, COMMAND);
	// set display offset
	ssd1306_write(oled, 0xD3, COMMAND);
	ssd1306_write(oled, 0x00, COMMAND);
	// set display start line
	ssd1306_write(oled, 0x40, COMMAND);
	// Enable charge pump regulator
	ssd1306_write(oled, 0x8D, COMMAND);
	ssd1306_write(oled, 0x14, COMMAND);
	// Set memory addressing mode
	ssd1306_write(oled, 0x20, COMMAND);
	ssd1306_write(oled, 0x00, COMMAND);
	// Set segment remap with column address 0 mapped to segment 0
	ssd1306_write(oled, 0xA0, COMMAND);
	ssd1306_write(oled, 0xC0, COMMAND);
	// set COM Pin hardware configuration
	ssd1306_write(oled, 0xDA, COMMAND);
	ssd1306_write(oled, 0x12, COMMAND);
	// set contrast control
	ssd1306_write(oled, 0x81, COMMAND);
	ssd1306_write(oled, 0x7F, COMMAND);
	// Set pre-charge period
	ssd1306_write(oled, 0xD9, COMMAND);
	ssd1306_write(oled, 0xF1, COMMAND);
	// Set Vcomh deselect level
	ssd1306_write(oled, 0xDB, COMMAND);
	ssd1306_write(oled, 0x20, COMMAND);
	// disable entire display on
	ssd1306_write(oled, 0xA4, COMMAND);
	// set normal display
	ssd1306_write(oled, 0xA6, COMMAND); //A6 normal a7 inverse
	// set segment re-map
	ssd1306_write(oled, 0xA0, COMMAND);
	// deactive scroll
	ssd1306_write(oled, 0x2E, COMMAND);
	// display on
	ssd1306_write(oled, 0xAF, COMMAND);
	// clear screen
	ssd1306_clear(oled);
}
static void ssd1306_clear(struct ssd1306 *oled)
{
	int i;
	for ( i = 0; i < 1024; i++ )
		ssd1306_write(oled, 0, DATA);
	ssd1306_goto_xy(oled, 0, 0);
}
static void ssd1306_goto_xy(struct ssd1306 *oled, uint8_t x, uint8_t y)
{
	ssd1306_write(oled, 0x21, COMMAND);
	ssd1306_write(oled, x * FONT_X, COMMAND);
	ssd1306_write(oled, OLED_WIDTH - 1, COMMAND);
	ssd1306_write(oled, 0x22, COMMAND);
	ssd1306_write(oled, y, COMMAND);
	ssd1306_write(oled, max_Y, COMMAND);
	oled->current_X = x;
	oled->current_Y = y;
}
static void ssd1306_send_char(struct ssd1306 *oled, uint8_t data)
{
	if (oled->current_X == max_X)
		ssd1306_go_to_next_line(oled);
	ssd1306_burst_write(oled, ssd1306_font[data-32], FONT_X, DATA);
	oled->current_X++;
}
static void ssd1306_send_char_inv(struct ssd1306 *oled, uint8_t data)
{
	uint8_t i;
	uint8_t buff[FONT_X];
	if (oled->current_X == max_X)
		ssd1306_go_to_next_line(oled);
	for ( i = 0; i < FONT_X; i++)
		buff[i] = ~ssd1306_font[data-32][i];
	ssd1306_burst_write(oled, buff, FONT_X, DATA);
	oled->current_X++;
}
static void ssd1306_send_string(struct ssd1306 *oled, uint8_t *str, color_t color)
{
	while (*str)
	{
		if(color == COLOR_WHITE)
			ssd1306_send_char(oled, *str++);
		else
			ssd1306_send_char_inv(oled, *str++);
	}
}
static int ssd1306_draw_pixel(struct ssd1306 *oled, uint8_t x, uint8_t y, color_t color)
{
	if(x > OLED_WIDTH || y > OLED_HEIGHT)
		return -1;
	if(color == COLOR_WHITE)
		oled->buffer[x + (y/8)*OLED_WIDTH] |= (1<<(y % 8));
	else
		oled->buffer[x + (y/8)*OLED_WIDTH] &= ~(1<<(y % 8));
	return 0;
}
static void ssd1306_sync(struct ssd1306 *oled)
{
	ssd1306_burst_write(oled, oled->buffer, OLED_WIDTH * OLED_HEIGHT / 8, DATA);
	memset(oled->buffer, 0, 1024);
}
static void ssd1306_go_to_next_line(struct ssd1306 *oled)
{
	oled->current_Y = (oled->current_Y == max_Y) ? 0 : (oled->current_Y + 1);
	ssd1306_goto_xy(oled, 0, oled->current_Y);
}
static void ssd1306_draw_bitmap(struct ssd1306 *oled, uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t width, uint8_t height, color_t color)
{
	int16_t byteWidth = (width + 7) / 8;
    uint8_t byte = 0;
    for(int16_t j=0; j<height; j++, y++)
    {
        for(int16_t i=0; i<width; i++)
        {
            if(i & 7)
               byte <<= 1;
            else
               byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
            if(byte & 0x80) 
				ssd1306_draw_pixel(oled, x+i, y, color);
        }
    }
}
static void animation(struct work_struct *work)
{
	int i;
	struct ssd1306 *oled = container_of(work, struct ssd1306, workqueue);
	if(oled)
	{
		for (i = 0; i < 28; i ++)
		{
			ssd1306_draw_bitmap(oled, 25, 0, frames[i], 64, 64, COLOR_WHITE);
			ssd1306_sync(oled);
		}
	}
	mod_timer(&oled->my_timer, jiffies + HZ/2);
}
static void tmHandler(struct timer_list *tm)
{
	struct ssd1306 *oled = container_of(tm, struct ssd1306, my_timer);
	if(oled)
		schedule_work(&oled->workqueue);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("20021163@vnu.edu.vn");
MODULE_DESCRIPTION("I2C driver for oled ssd1306");
MODULE_VERSION("1.0");