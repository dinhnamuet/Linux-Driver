#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/device.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>
#include <linux/ktime.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include "mydriver.h"
#define HIGH 1
#define LOW 0
#define set(var, bit) var |= (1UL << bit)
#define reset(var, bit) var &= ~(1UL << bit)
struct myproject
{
    struct i2c_client *client;
    struct gpio_desc *dht;
    // struct gpio_desc *ldr;
    // struct gpio_desc *gas;
    uint8_t *buffer;
    backlight_t backlight;
    dht_result_t mydht;

    struct timer_list myTimer;
    struct work_struct myWork;
};

static int lcd_write(struct myproject *home, uint8_t data, wmode_t DC);
static void lcd_init(struct myproject *home);
static void lcd_goto_xy(struct myproject *home, uint8_t x, uint8_t y);
static void clear_lcd(struct myproject *home);
static void lcd_print(struct myproject *home, uint8_t *data);
static int dht_read_all(struct myproject *home);
static s64 get_counter_us(void);
static void timer_handler(struct timer_list *tm);
static void sample(struct work_struct *work);

static int my_probe(struct i2c_client *client)
{
    struct device *dev;
    struct myproject *home;
    home = devm_kzalloc(&client->dev, sizeof(*home), GFP_KERNEL);
    if (!home)
        return -ENOMEM;
    i2c_set_clientdata(client, home);
    home->buffer = kzalloc(16, GFP_KERNEL);
    if (!home->buffer)
        return -ENOMEM;
    dev = &client->dev;
    home->client = client;
    home->dht = gpiod_get(dev, "dht", GPIOD_OUT_HIGH);
    gpiod_set_value(home->dht, HIGH);
    home->backlight = ON;
    lcd_init(home);
    INIT_WORK(&home->myWork, sample);
    timer_setup(&home->myTimer, timer_handler, 0);
    home->myTimer.expires = jiffies + HZ;
    add_timer(&home->myTimer);
    return 0;
}
static void my_remove(struct i2c_client *client)
{
    struct myproject *home = i2c_get_clientdata(client);
    if (home)
    {
        kfree(home->buffer);
        gpiod_put(home->dht);
        cancel_work_sync(&home->myWork);
        del_timer(&home->myTimer);
        home->backlight = OFF;
        clear_lcd(home);
    }
    pr_info("GoodBye!\n");
}

static const struct i2c_device_id my_id_table[] = {
    {.name = "lcd1602", 0},
    {}};
MODULE_DEVICE_TABLE(i2c, my_id_table);
static const struct of_device_id my_of_match_table[] = {
    {
        .compatible = "lcd1602",
    },
    {}};
MODULE_DEVICE_TABLE(of, my_of_match_table);

static struct i2c_driver mydrv = {
    .probe = my_probe,
    .remove = my_remove,
    .driver = {
        .name = "lcd1602",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(my_of_match_table),
    },
    .id_table = my_id_table,
};

static int lcd_write(struct myproject *home, uint8_t data, wmode_t DC)
{
    uint8_t dataH, dataL;
    uint8_t buff[4];
    dataH = data & 0xF0;
    dataL = (data << 4) & 0xF0;
    if (home->backlight)
    {
        dataH |= BACK_LIGHT;
        dataL |= BACK_LIGHT;
    }
    else
    {
        dataH |= NO_BL;
        dataL |= NO_BL;
    }
    if (DC == DATA)
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
    return i2c_master_send(home->client, buff, 4);
}
static void lcd_init(struct myproject *home)
{
    lcd_write(home, 0x33, COMMAND);
    msleep(10);
    lcd_write(home, 0x32, COMMAND);
    msleep(15);
    lcd_write(home, 0x28, COMMAND); // 4 bits interface, 2 line
    msleep(10);
    lcd_write(home, 0x01, COMMAND); // clear screen
    msleep(10);
    lcd_write(home, 0x0F, COMMAND); // on display, on cursor, blink cursor
    msleep(10);
    lcd_write(home, 0x02, COMMAND); // return home
    msleep(10);
}
static void lcd_goto_xy(struct myproject *home, uint8_t x, uint8_t y)
{
    uint8_t data;
    if (y)
        data = 0xC0 + x;
    else
        data = 0x80 + x;
    lcd_write(home, data, COMMAND);
}
static void clear_lcd(struct myproject *home)
{
    lcd_write(home, 0x01, COMMAND);
}
static void lcd_print(struct myproject *home, uint8_t *data)
{
    while (*data)
        lcd_write(home, *data++, DATA);
}
static s64 get_counter_us(void)
{
    return ktime_to_us(ktime_get());
}
static int dht_read_all(struct myproject *home)
{
    int i;
    uint8_t check_sum;
    s64 start_time;
    /* Start Condition */
    gpiod_direction_output(home->dht, 1);
    gpiod_set_value(home->dht, LOW);
    msleep(18);
    gpiod_set_value(home->dht, HIGH);
    /* Wait Response*/
    gpiod_direction_input(home->dht);
    start_time = get_counter_us();
    while(!gpiod_get_value(home->dht))
    {
        if (get_counter_us() - start_time > 90)
        return -1;
    }
    start_time = get_counter_us();
    while (gpiod_get_value(home->dht))
    {
        if (get_counter_us() - start_time > 90)
        return -1;
    }
    start_time = get_counter_us();
    while (!gpiod_get_value(home->dht))
    {
        if (get_counter_us() - start_time >= 90)
            return -1;
    }
    if (get_counter_us() - start_time < 60)
        return -1;
    /* Start reading data */
    start_time = get_counter_us();
    while (gpiod_get_value(home->dht))
    {
        if (get_counter_us() - start_time > 90)
        return -1;
    }
    for (i = 7; i >= 0; i--)
    {
        start_time = get_counter_us();
        while (gpiod_get_value(home->dht) == 0)
        {
            if(get_counter_us() - start_time > 60)
                return -1;
        }
        start_time = get_counter_us();
        while (gpiod_get_value(home->dht) == 1)
        {
            if(get_counter_us() - start_time > 90)
                return -1;
        }
        if (get_counter_us() - start_time < 40)
            reset(home->mydht.integer_hum, i);
        else
            set(home->mydht.integer_hum, i);
    }
    for (i = 7; i >= 0; i--)
    {
        start_time = get_counter_us();
        while (gpiod_get_value(home->dht) == 0)
        {
            if(get_counter_us() - start_time > 60)
                return -1;
        }
        start_time = get_counter_us();
        while (gpiod_get_value(home->dht) == 1)
        {
            if(get_counter_us() - start_time > 90)
                return -1;
        }
        if (get_counter_us() - start_time < 40)
            reset(home->mydht.decimal_hum, i);
        else
            set(home->mydht.decimal_hum, i);
    }
    for (i = 7; i >= 0; i--)
    {
        start_time = get_counter_us();
        while (gpiod_get_value(home->dht) == 0)
        {
            if(get_counter_us() - start_time > 60)
                return -1;
        }
        start_time = get_counter_us();
        while (gpiod_get_value(home->dht) == 1)
        {
            if(get_counter_us() - start_time > 90)
                return -1;
        }
        if (get_counter_us() - start_time < 40)
            reset(home->mydht.integer_temp, i);
        else
            set(home->mydht.integer_temp, i);
    }
    for (i = 7; i >= 0; i--)
    {
        start_time = get_counter_us();
        while (gpiod_get_value(home->dht) == 0)
        {
            if(get_counter_us() - start_time > 60)
                return -1;
        }
        start_time = get_counter_us();
        while (gpiod_get_value(home->dht) == 1)
        {
            if(get_counter_us() - start_time > 90)
                return -1;
        }
        if (get_counter_us() - start_time < 40)
            reset(home->mydht.decimal_temp, i);
        else
            set(home->mydht.decimal_temp, i);
    }
    for (i = 7; i >= 0; i--)
    {
        start_time = get_counter_us();
        while (gpiod_get_value(home->dht) == 0)
        {
            if(get_counter_us() - start_time > 60)
                return -1;
        }
        start_time = get_counter_us();
        while (gpiod_get_value(home->dht) == 1)
        {
            if(get_counter_us() - start_time > 90)
                return -1;
        }
        if (get_counter_us() - start_time < 40)
            reset(check_sum, i);
        else
            set(check_sum, i);
    }
    if (home->mydht.integer_temp + home->mydht.decimal_temp + home->mydht.integer_hum + home->mydht.decimal_hum == check_sum)
        return 0;
    else
        return -1;
}
static void timer_handler(struct timer_list *tm)
{
    struct myproject *home = container_of(tm, struct myproject, myTimer);
    if(home)
        schedule_work(&home->myWork);
}
static void sample(struct work_struct *work)
{
    struct myproject *home = container_of(work, struct myproject, myWork);
    if (home)
    {
        if (dht_read_all(home) == 0)
        {
            lcd_goto_xy(home, 0, 0);
            memset(home->buffer, 0, 16);
            sprintf(home->buffer, "Temp: %d.%d C", home->mydht.integer_temp, home->mydht.decimal_temp);
            lcd_print(home, home->buffer);

            lcd_goto_xy(home, 0, 1);
            memset(home->buffer, 0, 16);
            sprintf(home->buffer, "Humi: %d.%d", home->mydht.integer_hum, home->mydht.decimal_hum);
            lcd_print(home, home->buffer);
        }
    }
    mod_timer(&home->myTimer, jiffies + HZ);
}
module_i2c_driver(mydrv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DinhNam <20021163@vnu.edu.vn>");
MODULE_DESCRIPTION("Sensor Monitoring");
MODULE_VERSION("1.0");