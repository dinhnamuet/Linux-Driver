#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/ktime.h>
#include <linux/time.h>
#include "ds3231.h"
struct ds3231 {
    struct i2c_client *client;
    date_t time;

    struct work_struct workq;
    struct timer_list timer;
};

static u8 ds3231_write8(struct ds3231 *rtc, u8 reg, u8 value);
static u8 ds3231_write16(struct ds3231 *rtc, u8 reg, u16 value);
static u8 ds3231_read8(struct ds3231 *rtc, u8 reg);
static u16 ds3231_read16(struct ds3231 *rtc, u8 reg);
static void ds3231_update_real_time(struct ds3231 *rtc);
static void ds3231_init(struct ds3231 *rtc);
static u8 ds3232_read_time(struct ds3231 *rtc);
static u8 encodeBCD8(u8 value);
static u16 encodeBCD16(u16 value);
static u8 decodeBCD8(u8 value);
static u16 decodeBCD16(u16 value);

static void timer_handler(struct timer_list *tm)
{
    struct ds3231 *rtc = container_of(tm, struct ds3231, timer);
    if(rtc)
        schedule_work(&rtc->workq);
}
static void sample(struct work_struct *work)
{
    struct ds3231 *rtc = container_of(work, struct ds3231, workq);
    if(rtc)
    {
        ds3232_read_time(rtc);
        pr_info("%s, %d: %d: %d - %d/%d/%d\n", rtc->time.day, rtc->time.hour, rtc->time.min, rtc->time.sec, rtc->time.date, rtc->time.month, rtc->time.year);
    }
    mod_timer(&rtc->timer, jiffies + HZ);
}

static int ds3231_probe(struct i2c_client *client)
{
    struct ds3231 *rtc;
    rtc = kzalloc(sizeof(*rtc), GFP_KERNEL);
    if(!rtc)
        return -ENOMEM;
    i2c_set_clientdata(client, rtc);
    rtc->client = client;
    ds3231_init(rtc);
    INIT_WORK(&rtc->workq, sample);
    timer_setup(&rtc->timer, timer_handler, 0);
    rtc->timer.expires = jiffies + HZ;
    add_timer(&rtc->timer);
    return 0;
}
static void ds3231_remove(struct i2c_client *client)
{
    struct ds3231 *rtc = i2c_get_clientdata(client);
    if(rtc)
    {
        cancel_work_sync(&rtc->workq);
        del_timer(&rtc->timer);
        kfree(rtc);
    }
}
static const struct i2c_device_id ds3231_id_table[] = {
    { "rtc-ds3231", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, ds3231_id_table);
static const struct of_device_id ds3231_of_match[] = {
    { .compatible = "rtc-ds3231", }, 
    { }
};
MODULE_DEVICE_TABLE(of, ds3231_of_match);
static struct i2c_driver ds3231_drv = {
    .probe = ds3231_probe,
    .remove = ds3231_remove,
    .driver = {
        .name = "rtc-ds3231",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(ds3231_of_match),
    },
    .id_table = ds3231_id_table,
};
static u8 encodeBCD8(u8 value)
{
    return (value % 10 + ((value / 10) << 4));
}
static u16 encodeBCD16(u16 value)
{
    return ((value % 10) + (((value / 10) % 10) << 4) + (((value / 100) % 10) << 8) + (((value / 1000) % 10) << 12));
}
static u8 decodeBCD8(u8 value)
{
    return (((value & 0xf0) >> 4) * 10) + (value & 0x0f);
}
static u16 decodeBCD16(u16 value)
{
    return (((value & 0xf000) >> 12) * 1000) + (((value & 0x0f00) >> 8) * 100) + (((value & 0xf0) >> 4) * 10) + (value & 0x0f);
}
static u8 ds3231_write8(struct ds3231 *rtc, u8 reg, u8 value)
{
    return i2c_smbus_write_byte_data(rtc->client, reg, value);
}
static u8 ds3231_write16(struct ds3231 *rtc, u8 reg, u16 value)
{
    return i2c_smbus_write_word_data(rtc->client, reg, value);
}
static u8 ds3231_read8(struct ds3231 *rtc, u8 reg)
{
    return i2c_smbus_read_byte_data(rtc->client, reg);
}
static u16 ds3231_read16(struct ds3231 *rtc, u8 reg)
{
    return i2c_smbus_read_word_data(rtc->client, reg);
}
static void ds3231_update_real_time(struct ds3231 *rtc)
{
    time64_t second;
    struct tm time_info;
    second = ktime_to_ns(ktime_get_real())/NSEC_PER_SEC;
    time64_to_tm(second, 7*3600, &time_info);

    ds3231_write8(rtc, DS3231_REG_DAY, encodeBCD8(time_info.tm_wday));
    ds3231_write8(rtc, DS3231_REG_DATE, encodeBCD8(time_info.tm_mday));
    ds3231_write8(rtc, DS3231_REG_MONTH, encodeBCD8(time_info.tm_mon + 1));
    ds3231_write16(rtc, DS3231_REG_YEAR, encodeBCD16(time_info.tm_year + 1900));
    ds3231_write8(rtc, DS3231_REG_HOUR, encodeBCD8(time_info.tm_hour));
    ds3231_write8(rtc, DS3231_REG_MINUTE, encodeBCD8(time_info.tm_min));
    ds3231_write8(rtc, DS3231_REG_SECOND, encodeBCD8(time_info.tm_sec));
}
static void ds3231_init(struct ds3231 *rtc)
{
    u8 foo;
    foo = ds3231_read8(rtc, DS3231_REG_CONTROL);
    foo &= ~0x03; //disable alarm
    ds3231_write8(rtc, DS3231_REG_CONTROL, foo);
    ds3231_write8(rtc, DS3231_REG_STATUS, 0x00); //clear all flag
    ds3231_update_real_time(rtc);
}
static u8 ds3232_read_time(struct ds3231 *rtc)
{
    u8 day;
    rtc->time.hour  = decodeBCD8(ds3231_read8(rtc, DS3231_REG_HOUR));
    rtc->time.min   = decodeBCD8(ds3231_read8(rtc, DS3231_REG_MINUTE));
    rtc->time.sec   = decodeBCD8(ds3231_read8(rtc, DS3231_REG_SECOND));

    rtc->time.date   = decodeBCD8(ds3231_read8(rtc, DS3231_REG_DATE));
    rtc->time.month  = decodeBCD8(ds3231_read8(rtc, DS3231_REG_MONTH));
    rtc->time.year   = decodeBCD16(ds3231_read16(rtc, DS3231_REG_YEAR));
    day              = decodeBCD8(ds3231_read8(rtc, DS3231_REG_DAY));
    memset(rtc->time.day, 0, sizeof(rtc->time.day));
    switch(day)
    {
        case 1:
            strncpy(rtc->time.day, "Monday", strlen("Monday"));
            break;
        case 2:
            strncpy(rtc->time.day, "Tuesday", strlen("Tuesday"));
            break;
        case 3:
            strncpy(rtc->time.day, "Wednesday", strlen("Wednesday"));
            break;
        case 4:
            strncpy(rtc->time.day, "Thursday", strlen("Thursday"));
            break;
        case 5:
            strncpy(rtc->time.day, "Friday", strlen("Friday"));
            break;
        case 6:
            strncpy(rtc->time.day, "Saturday", strlen("Saturday"));
            break;
        case 7:
            strncpy(rtc->time.day, "Sunday ", strlen("Sunday "));
            break;
        default:
            break;
    }
    return day;
}
module_i2c_driver(ds3231_drv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DinhNam <20021163@vnu.edu.vn>");
MODULE_DESCRIPTION("I2C SMBUS Test for DS3231");
MODULE_VERSION("1.0");
