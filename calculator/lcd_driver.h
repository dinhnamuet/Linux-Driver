#ifndef __LCD_DRIVER_H__
#define __LCD_DRIVER_H__

#define DATA		1
#define CMD		0
#define EN 		0x04
#define RW 		0x02
#define RS 		0x01
#define BACK_LIGHT	0x08
#define NO_BL		0x00
#define ON		1
#define OFF		0
#define DEV_NAME	"lcd1602"

struct position {
	uint8_t x;
	uint8_t y;
};

#define CLR_LCD                 _IO('U', 2)
#define CLR_LAST                _IO('a', '1')
#define CLR_ROW                 _IOW('a', '2', uint8_t *)
#define GOTO_XY                 _IOW('a', '3', struct position *)
#define LCD_SEND_CHAR           _IOW('a', '5', uint8_t *)


#endif
