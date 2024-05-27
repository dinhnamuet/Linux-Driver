#ifndef __OLED_H__
#define __OLED_H__

#define OLED "/dev/ssd1306"

typedef unsigned char u8;

struct xy {
	u8 x;
	u8 y;
};

#define GOTO_XY		_IOW('G', 1, struct xy *)
#define CLR_OLED    _IO('C', 1)
#define NEXT_LINE   _IO('N', 1)
#define INIT_OLED   _IO('I', 1)

int oled_init(const char *path_to_dev);
int oled_clear(const char *path_to_dev);
int oled_goto_next_line(const char *path_to_dev);
int oled_goto_xy(const char *path_to_dev, u8 x, u8 y);
int oled_write(const char *path_to_dev, const char *data);

#endif