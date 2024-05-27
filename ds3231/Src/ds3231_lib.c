#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include "ds3231_lib.h"

struct tm *getTm(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return localtime(&tv.tv_sec);
}

int update_time(const char *path_to_dev) {
    int res, fd;
    fd = open(path_to_dev, O_RDWR);
    if (-1 == fd)
        return -1;
    res = ioctl(fd, UPDATE_TIME, (tm_usr_t *)getTm());
    close(fd);
    return res;
}
int get_time(const char *path_to_dev, date_t *time) {
    int res, fd;
    fd = open(path_to_dev, O_RDONLY);
    if (-1 == fd)
        return -1;
    res = ioctl(fd, GET_TIME, time);
    close(fd);
    return res;
}
