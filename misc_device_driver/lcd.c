#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "lcd.h"

int lcd_init(void)
{
	int fd;
	fd = open(LCD, O_RDWR);
	if (-1 == fd)
        	return -1;
	if (ioctl(fd, LCD_INIT, NULL) < 0)
    	{
        	close(fd);
        	return -1;
    	}
	close(fd);
	return 0;
}
int clear_lcd(void)
{
	int fd;
	fd = open(LCD, O_RDWR);
	if (-1 == fd)
        	return -1;
	if (ioctl(fd, CLR_LCD, NULL) < 0)
    	{
        	close(fd);
        	return -1;
    	}
	close(fd);
 	return 0;
}
int lcd_clear_row(uint8_t row)
{
	int fd;
        fd = open(LCD, O_RDWR);
        if (-1 == fd)
                return -1;
        if (ioctl(fd, CLR_ROW, &row) < 0)
        {
                close(fd);
                return -1;
        }
        close(fd);
        return 0;

}
int lcd_goto_xy(uint8_t x, uint8_t y)
{
	struct position pos;
	pos.x = x;
	pos.y = y;
	int fd;
        fd = open(LCD, O_RDWR);
        if (-1 == fd)
                return -1;
        if (ioctl(fd, GOTO_XY, &pos) < 0)
        {
                close(fd);
                return -1;
        }
        close(fd);
        return 0;
}
int lcd_print(char *data)
{
	int fd = open(LCD, O_RDWR);
	if (-1 == fd)
        	return -1;
	write(fd, data, strlen(data));
	close(fd);
	return 0;
}
