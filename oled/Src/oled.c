#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include "oled.h"

int oled_init(const char *path_to_dev) {
    int res;
    int fd = open(path_to_dev, O_WRONLY);
    if (-1 == fd)
        return -1;
    res = ioctl(fd, INIT_OLED, NULL);
    close(fd);
    return res;
}
int oled_clear(const char *path_to_dev) {
    int res;
    int fd = open(path_to_dev, O_WRONLY);
    if (-1 == fd)
        return -1;
    res = ioctl(fd, CLR_OLED, NULL);
    close(fd);
    return res;
}
int oled_goto_next_line(const char *path_to_dev) {
    int res;
    int fd = open(path_to_dev, O_WRONLY);
    if (-1 == fd)
        return -1;
    res = ioctl(fd, NEXT_LINE, NULL);
    close(fd);
    return res;
}
int oled_goto_xy(const char *path_to_dev, u8 x, u8 y) {
    int res;
    struct xy newXY;
    newXY.x = x;
    newXY.y = y;
    int fd = open(path_to_dev, O_WRONLY);
    if (-1 == fd)
        return -1;
    res = ioctl(fd, GOTO_XY, &newXY);
    close(fd);
    return res;
}
int oled_write(const char *path_to_dev, const char *data) {
    int res;
    int fd = open(path_to_dev, O_WRONLY);
    if (-1 == fd)
        return -1;
    res = write(fd, data, strlen(data));
    close(fd);
    return res;
}