#ifndef __LCD_H__
#define __LCD_H__

#define EN          0x04
#define RW          0x02
#define RS          0x01
#define BACKLIGHT   0x08
#define NOBACKLIGHT 0x00

typedef enum {
    COMMAND,
    DATA
} wmode_t;

typedef enum {
    OFF,
    ON
} backlight_t;

typedef struct {
    struct i2c_client *client;
    backlight_t backlight;
    
    struct kobj_attribute line1;
    struct kobj_attribute line2;
    struct kobj_attribute clear;
} lcd;

#endif
