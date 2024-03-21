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

#define LCD_INIT _IO('U', 2)
#define CLR_LCD _IO('U', 1)
#define CLR_ROW	_IOW('a', '1', uint8_t *)
#define GOTO_XY _IOW('a', '2', struct position *)

#endif
