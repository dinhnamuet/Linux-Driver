#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/sched/signal.h>
#include <linux/mod_devicetable.h>
#include <linux/timer.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include "nokia5110.h"

#define DATA 	1
#define COMMAND 0

struct nokia {
	dev_t dev_num;
	struct spi_device *spi;
	struct gpio_desc *reset;
	struct gpio_desc *dc;
	struct gpio_desc *led;
	struct cdev *mcdev;
	struct device *mdevice;

	struct timer_list tm_list;
	struct kern_time time;
	struct work_struct my_work;
};

void lcd_init(struct nokia *lcd);
void lcd_reset(struct nokia *lcd);
void lcd_write_one_byte(struct nokia *lcd, uint8_t data, uint8_t mode);
void lcd_send_char(struct nokia *lcd, uint8_t data);
void lcd_clear(struct nokia *lcd);
void lcd_goto_XY(struct nokia *lcd, uint8_t X, uint8_t Y);
void lcd_send_string(struct nokia *lcd, uint8_t *s, uint8_t color);
void led_on(struct nokia *lcd);
void led_off(struct nokia *lcd);

static void get_time_of_day(struct kern_time *rt)
{
	time64_t seconds;
        seconds = ktime_to_ns(ktime_get_real())/NSEC_PER_SEC;
        seconds += 7*3600; // GMT +7
        rt->hour = (seconds / 3600) % 24;
        rt->min = (seconds / 60) % 60;
        rt->sec = seconds % 60;
}
static void get_system_up_time(struct kern_time *rt)
{
	time64_t seconds;
        seconds = ktime_to_ns(ktime_get())/NSEC_PER_SEC;
        rt->hour = (seconds / 3600) % 24;
        rt->min = (seconds / 60) % 60;
        rt->sec = seconds % 60;
}

static int print_time(struct nokia *lcd, uint8_t line, uint8_t type)
{
	uint8_t *buff;
	buff = kmalloc(14, GFP_KERNEL);
	if(buff)
	{
		memset(buff, 0, 14);
		if(type == SYSUP_TIME)
			sprintf(buff, "st: %2d:%2d:%2d", lcd->time.hour, lcd->time.min, lcd->time.sec);
		else
			sprintf(buff, "time: %2d:%2d:%2d", lcd->time.hour, lcd->time.min, lcd->time.sec);
		lcd_goto_XY(lcd, 0, line);
		lcd_send_string(lcd, buff, 0);
		kfree(buff);
		return 0;
	}
	return -1;
}
static void work_fn(struct work_struct *work)
{
	struct nokia *lcd = container_of(work, struct nokia, my_work);
        get_time_of_day(&lcd->time);
        print_time(lcd, 1, WALL_TIME);
        get_system_up_time(&lcd->time);
        print_time(lcd, 2, SYSUP_TIME);
}
static void polling(struct timer_list *tm)
{
	struct nokia *lcd = container_of(tm, struct nokia, tm_list);
	if(lcd)
		schedule_work(&lcd->my_work);
	mod_timer(&lcd->tm_list, jiffies + HZ);
}

static int nokia_probe(struct spi_device *spi)
{
	struct nokia *lcd = NULL;
	struct device *dev = &spi->dev;
	lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);
	if(lcd == NULL)
	{
		printk(KERN_ERR "Allocate device failure\n");
		return -1;
	}
	lcd->reset = gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	gpiod_set_value(lcd->reset, 1);
	lcd->dc = gpiod_get(dev, "dc", GPIOD_OUT_HIGH);
	lcd->led = gpiod_get(dev, "led", GPIOD_OUT_HIGH);
	lcd->spi = spi;
	spi_set_drvdata(spi, lcd);
	lcd->spi->mode = SPI_MODE_0;
	lcd->spi->bits_per_word = 8;
	lcd->spi->max_speed_hz = 2000000;
	if (spi_setup(lcd->spi) < 0)
	{
		printk(KERN_ERR "SPI setup failed\n");
		goto rm_lcd;
	}
	
	lcd_init(lcd);

	lcd_goto_XY(lcd, 0, 0);
	lcd_send_string(lcd, " DONG HO NOKIA", 1);

	INIT_WORK(&lcd->my_work, work_fn);
	timer_setup(&lcd->tm_list, polling, 0);
	lcd->tm_list.expires = jiffies + HZ;
	add_timer(&lcd->tm_list);

	printk(KERN_INFO "Init successful\n");
	return 0;
rm_lcd:
	gpiod_put(lcd->dc);
	gpiod_put(lcd->led);
	gpiod_put(lcd->reset);
	return -1;
}

static void nokia_remove(struct spi_device *spi)
{
	struct nokia *lcd = spi_get_drvdata(spi);
	if(lcd == NULL)
	{
		printk(KERN_ERR "Cannot get driver data\n");
	}
	else
	{
		led_off(lcd);
		del_timer(&lcd->tm_list);
		lcd_clear(lcd);
		gpiod_put(lcd->reset);
		gpiod_put(lcd->dc);
		gpiod_put(lcd->led);
		printk(KERN_INFO "Device free\n");
	}
}

static const struct of_device_id nokia_of_match_id[] = {
	{ .compatible = "nokia-5110,nam", },
	{ }
};
MODULE_DEVICE_TABLE(of, nokia_of_match_id);

static struct spi_driver nokia_driver = {
	.probe = nokia_probe,
	.remove = nokia_remove,
	.driver = {
		.name = "namNokia",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(nokia_of_match_id),
	},
};
module_spi_driver(nokia_driver);

void lcd_init(struct nokia *lcd)
{
	lcd_reset(lcd);
	lcd_write_one_byte(lcd, ADD_CMD, COMMAND); // Using the additional commands
	lcd_write_one_byte(lcd, SET_VOP | 0x40, COMMAND);
	lcd_write_one_byte(lcd, BASIC_CMD, COMMAND); // Using the basic command
	lcd_write_one_byte(lcd, DISP_CTL | DP_NORMAL, COMMAND);
	lcd_clear(lcd);
//	led_on(lcd);
}
void lcd_reset(struct nokia *lcd)
{
	gpiod_set_value(lcd->reset, 0);
	msleep(10);
	gpiod_set_value(lcd->reset, 1);
}
void lcd_write_one_byte(struct nokia *lcd, uint8_t data, uint8_t mode)
{
	if(mode == COMMAND)
		gpiod_set_value(lcd->dc, 0);
	else if(mode == DATA)
		gpiod_set_value(lcd->dc, 1);
	spi_write(lcd->spi, &data, 1);
}
void lcd_send_char(struct nokia *lcd, uint8_t data)
{
	spi_write(lcd->spi, font6x8[data-32], 6);
}
void lcd_send_char_inv(struct nokia *lcd, uint8_t data)
{
	int i;
	uint8_t foo_arr[6];
	for(i = 0; i < 6; i++)
		foo_arr[i] = ~font6x8[data-32][i];
	spi_write(lcd->spi, foo_arr, 6);
}
void lcd_send_string(struct nokia *lcd, uint8_t *s, uint8_t color)
{
	gpiod_set_value(lcd->dc, 1);
	while(*s)
	{
		if(color)
			lcd_send_char_inv(lcd, *s++);
		else
			lcd_send_char(lcd, *s++);
	}
}
void lcd_clear(struct nokia *lcd)
{
	uint32_t pixel;
	for(pixel = 0; pixel < 504; pixel++)
	{
		lcd_write_one_byte(lcd, 0x00, DATA);
	}
	lcd_goto_XY(lcd, 0, 0);
}

void lcd_goto_XY(struct nokia *lcd, uint8_t X, uint8_t Y)
{
	lcd_write_one_byte(lcd, SET_Y | Y, COMMAND);
	lcd_write_one_byte(lcd, SET_X | X*6, COMMAND);
}
void led_on(struct nokia *lcd)
{
	gpiod_set_value(lcd->led, 0);
}
void led_off(struct nokia *lcd)
{
	gpiod_set_value(lcd->led, 1);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("20021163@vnu.edu.vn");
MODULE_DESCRIPTION("SPI driver for lcd nokia5110");
MODULE_VERSION("1.0");
