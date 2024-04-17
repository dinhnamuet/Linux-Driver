#ifndef __MYDRV_H__
#define __MYDRV_H__

#define EN 		    0x04
#define RW 		    0x02
#define RS 		    0x01
#define BACK_LIGHT	0x08
#define NO_BL		0x00

typedef enum {
    COMMAND,
    DATA
} wmode_t;
typedef enum {
    OFF,
    ON
} backlight_t;
typedef struct{
    uint8_t integer_temp;
    uint8_t decimal_temp;
    uint8_t integer_hum;
    uint8_t decimal_hum;
} dht_result_t;

#endif
