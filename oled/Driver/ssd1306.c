#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include "ssd1306.h"
const int max_X = OLED_WIDTH / FONT_X - 1;
const int max_Y = OLED_HEIGHT / 8 - 1;

struct ssd1306 {
	struct i2c_client *client;
	u8 current_X;
	u8 current_Y;
	struct miscdevice m_dev;
	u8 *buffer;
};

static int ssd1306_write(struct ssd1306 *oled, u8 data, write_mode_t mode);
static void ssd1306_init(struct ssd1306 *oled);
static void ssd1306_clear(struct ssd1306 *oled);
static void ssd1306_goto_xy(struct ssd1306 *oled, u8 x, u8 y);
static void ssd1306_send_char(struct ssd1306 *oled, u8 data);
static void ssd1306_send_char_inv(struct ssd1306 *oled, u8 data);
static void ssd1306_send_string(struct ssd1306 *oled, u8 *str, color_t color);
static void ssd1306_go_to_next_line(struct ssd1306 *oled);
static int ssd1306_burst_write(struct ssd1306 *oled, const u8 *data, int len, write_mode_t mode);
static int ssd1306_open(struct inode *inodep, struct file *filep);
static int ssd1306_close(struct inode *inodep, struct file *filep);
static long ssd1306_ioctl (struct file *filep, unsigned int cmd, unsigned long data);
static ssize_t ssd1306_write_u(struct file *filep, const char __user *ubuf, size_t size, loff_t *offset);

struct file_operations fops = {
	.owner 			= THIS_MODULE,
	.open			= ssd1306_open,
	.release 		= ssd1306_close,
	.write 			= ssd1306_write_u,
	.unlocked_ioctl = ssd1306_ioctl,
};
static int oled_probe(struct i2c_client *client) {
	struct ssd1306 *oled = NULL;
	oled = devm_kzalloc(&client->dev, sizeof(*oled), GFP_KERNEL);
	if (!oled)
		return -ENOMEM;
	oled->buffer = devm_kzalloc(&client->dev, PAGE_SIZE, GFP_KERNEL);
	if (!oled->buffer)
		return -ENOMEM;
	oled->client = client;
	i2c_set_clientdata(client, oled);
//	ssd1306_init(oled);
	oled->m_dev.minor = MISC_DYNAMIC_MINOR;
	oled->m_dev.name = "ssd1306";
	oled->m_dev.mode = 0666;
	oled->m_dev.fops = &fops;
	return misc_register(&oled->m_dev);
}
static void oled_remove(struct i2c_client *client) {
	struct ssd1306 *oled = i2c_get_clientdata(client);
	if (!oled)
		return;
	else {
		ssd1306_clear(oled);
		ssd1306_write(oled, 0xAE, COMMAND); // display off
		misc_deregister(&oled->m_dev);
	}
}
static const struct i2c_device_id oled_device_id[] = {
	{ .name = "nam", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, oled_device_id);
static const struct of_device_id oled_of_match_id[] = {
	{ .compatible = "ssd1306-oled,nam", },
	{ }
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

static int ssd1306_write(struct ssd1306 *oled, u8 data, write_mode_t mode) {
	/*
	A control byte mainly consists of Co and D/C# bits following by six “0”
	Co | D/C | 000000
	Co bit is equal to 0
	*/
	return i2c_smbus_write_byte_data(oled->client, mode, data);
}
static int ssd1306_burst_write(struct ssd1306 *oled, const u8 *data, int len, write_mode_t mode) {
	return i2c_smbus_write_i2c_block_data(oled->client, mode, len, data);
}
static void ssd1306_init(struct ssd1306 *oled) {
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
	pr_info("%s, %d\n", __func__, __LINE__);
	msleep(5);
}
static void ssd1306_clear(struct ssd1306 *oled) {
	u16 i;
	ssd1306_goto_xy(oled, 0, 0);
	for (i = 0; i < 1024; i++)
		ssd1306_write(oled, 0, DATA);
	ssd1306_goto_xy(oled, 0, 0);
}
static void ssd1306_goto_xy(struct ssd1306 *oled, u8 x, u8 y) {
	ssd1306_write(oled, 0x21, COMMAND);
	ssd1306_write(oled, x * FONT_X, COMMAND);
	ssd1306_write(oled, OLED_WIDTH - 1, COMMAND);
	ssd1306_write(oled, 0x22, COMMAND);
	ssd1306_write(oled, y, COMMAND);
	ssd1306_write(oled, max_Y, COMMAND);
	oled->current_X = x;
	oled->current_Y = y;
}
static void ssd1306_send_char(struct ssd1306 *oled, u8 data) {
	if (oled->current_X == max_X)
		ssd1306_go_to_next_line(oled);
	if (data == '\n') {
		ssd1306_go_to_next_line(oled);
		return;
	}
	ssd1306_burst_write(oled, ssd1306_font[data-32], FONT_X, DATA);
	oled->current_X++;
}
static void ssd1306_send_char_inv(struct ssd1306 *oled, u8 data) {
	u8 i;
	u8 buff[FONT_X];
	if (oled->current_X == max_X)
		ssd1306_go_to_next_line(oled);
	for ( i = 0; i < FONT_X; i++)
		buff[i] = ~ssd1306_font[data-32][i];
	ssd1306_burst_write(oled, buff, FONT_X, DATA);
	oled->current_X++;
}
static void ssd1306_send_string(struct ssd1306 *oled, u8 *str, color_t color) {
	while (*str)
	{
		if(color == COLOR_WHITE)
			ssd1306_send_char(oled, *str++);
		else
			ssd1306_send_char_inv(oled, *str++);
	}
}
static void ssd1306_go_to_next_line(struct ssd1306 *oled) {
	oled->current_Y = (oled->current_Y == max_Y) ? 0 : (oled->current_Y + 1);
	ssd1306_goto_xy(oled, 0, oled->current_Y);
}
static int ssd1306_open(struct inode *inodep, struct file *filep) {
	struct ssd1306 *oled = container_of(filep->private_data, struct ssd1306, m_dev);
	if (oled)
		filep->private_data = oled;
	else
		return -ENODEV;
	return 0;
}
static int ssd1306_close(struct inode *inodep, struct file *filep) {
	filep->private_data = NULL;
	return 0;
}

static long ssd1306_ioctl (struct file *filep, unsigned int cmd, unsigned long data) {
	struct ssd1306 *oled = filep->private_data;
	struct xy newXY;
	long res = 0;
	if (!oled)
		return -EFAULT;
	switch (cmd) {
		case INIT_OLED:
			ssd1306_init(oled);
			break;
		case NEXT_LINE:
			ssd1306_go_to_next_line(oled);
			break;
		case CLR_OLED:
			ssd1306_clear(oled);
			break;
		case GOTO_XY:
			res = copy_from_user(&newXY, (struct xy *)data, sizeof(struct xy));
			ssd1306_goto_xy(oled, newXY.x, newXY.y);
			break;
		default:
			break;
	}
	return res;
}
static ssize_t ssd1306_write_u(struct file *filep, const char __user *ubuf, size_t size, loff_t *offset) {
	struct ssd1306 *oled = filep->private_data;
	if (!oled)
		return -EFAULT;
	memset(oled->buffer, 0, PAGE_SIZE);
	if (copy_from_user(oled->buffer, ubuf, size))
		return -EFAULT;
	pr_info("%s\n", oled->buffer);
	ssd1306_send_string(oled, oled->buffer, COLOR_WHITE);
	return size;
}

module_i2c_driver(oled_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DinhNam <20021163@vnu.edu.vn>");
MODULE_DESCRIPTION("I2C driver for oled ssd1306");
MODULE_VERSION("1.0");
