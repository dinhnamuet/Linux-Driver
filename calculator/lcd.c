#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "lcd.h"
int lcd_start(int *fd)
{
	*fd = open(LCD, O_WRONLY);
	if (-1 == *fd)
		return -1;
	return *fd;
}
void lcd_stop(int *fd)
{
	close(*fd);
}

int clear_lcd(int *fd)
{
	if (ioctl(*fd, CLR_LCD, NULL) < 0)
		return -1;
	return 0;
}
int lcd_clear_row(int *fd, uint8_t row)
{
	if (ioctl(*fd, CLR_ROW, &row) < 0)
		return -1;
	return 0;
}
int lcd_goto_xy(int *fd, uint8_t x, uint8_t y)
{
	struct position pos;
	pos.x = x;
	pos.y = y;
	if (ioctl(*fd, GOTO_XY, &pos) < 0)
		return -1;
	return 0;
}
int lcd_clr_last(int *fd)
{
	if (ioctl(*fd, CLR_LAST, NULL) < 0)
		return -1;
	return 0;
}
int lcd_print_res(int *fd, uint8_t *res)
{
	if (ioctl(*fd, RESULT, res) < 0)
		return -1;
	return 0;
}
int lcd_send(int *fd, uint8_t *data)
{
	if (ioctl(*fd, LCD_SEND_CHAR, data) < 0)
		return -1;
	return 0;
}
