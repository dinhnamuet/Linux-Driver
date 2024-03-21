#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"
#define TEM "/sys/class/thermal/thermal_zone0/temp"

int main()
{
	char a[10];
	int fd;
	lcd_init();
	while(1)
	{
		fd = open(TEM, O_RDONLY);
		if(-1 == fd)
		{
			printf("FAILED\n");
			return -1;
		}
		memset(a, 0, 10);
		read(fd, a, 10);
		close(fd);
		sprintf(a, "CPU Temp: %.2f", (double)atoi(a)/1000);
		lcd_goto_xy(0, 0);
		lcd_print(a);
		sleep(3);
	}
	return 0;
}
