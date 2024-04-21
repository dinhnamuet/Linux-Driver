#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include "pwmled.h"

int main (int argc, char *argv[])
{
    int fd;
    uint8_t duty_cycle;
    if(argc != 3)
        return -1;
    duty_cycle = atoi(argv[2]);
    fd = open("/dev/leds", O_WRONLY);
    if(-1 == fd)
        return -1;
    if(!strncmp(argv[1], "led1", 4))
    {
        ioctl(fd, LED1, &duty_cycle);
    }
    else if(!strncmp(argv[1], "led2", 4))
    {
        ioctl(fd, LED2, &duty_cycle);
    }
    close(fd);
    return 0;
}