#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>
#define LCD "/dev/lcd1602"

struct position {
        uint8_t x;
        uint8_t y;
};

#define LCD_SEND_CHAR	        _IOW('a', '5', uint8_t *)
#define CLR_LCD			_IO('U', 2)
#define CLR_LAST		_IO('a', '1')
#define CLR_ROW			_IOW('a', '2', uint8_t *)
#define GOTO_XY			_IOW('a', '3', struct position *)
#define RESULT			_IOW('a', '4', uint8_t *)

int lcd_start(int *fd);
void lcd_stop(int *fd);
int clear_lcd(int *fd);
int lcd_clear_row(int *fd, uint8_t row);
int lcd_goto_xy(int *fd, uint8_t x, uint8_t y);
int lcd_clr_last(int *fd);
int lcd_print_res(int *fd, uint8_t *res);
int lcd_send(int *fd, uint8_t *data);

#endif /* __LCD_H__ */
