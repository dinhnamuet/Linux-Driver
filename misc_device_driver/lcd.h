#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>
#define LCD "/dev/lcd1602"

struct position {
        uint8_t x;
        uint8_t y;
};
#define LCD_INIT _IO('U', 2)
#define CLR_LCD _IO('U', 1)
#define CLR_ROW _IOW('a', '1', uint8_t *)
#define GOTO_XY _IOW('a', '2', struct position *)

int lcd_init(void);
int clear_lcd(void);
int lcd_clear_row(uint8_t row);
int lcd_goto_xy(uint8_t x, uint8_t y);
int lcd_print(char *data);

#endif /* __LCD_H__ */
