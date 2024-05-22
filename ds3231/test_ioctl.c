#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include "ds3231.h"
#define RTC "/dev/ds3231"

static int fd;
static int start_rtc(int *fd);
static int update_time(int *fd);
static int get_time(int *fd, date_t *time);
static void stop_rtc(int *fd);
void ctrlC(int num) {
    stop_rtc(&fd);
    puts("Stop reading!");
}

int main(int argc, char *argv[]) {
    date_t myTime;
    if (start_rtc(&fd) < 0)
        return -1;
    update_time(&fd);
    signal(SIGINT, ctrlC);
    while (1) {
        if (get_time(&fd, &myTime) < 0) {
            stop_rtc(&fd);
            return -1;
        }
        printf ("%s, %d: %d: %d - %d/%d/%d\n", myTime.day, myTime.hour, myTime.min, myTime.sec, myTime.date, myTime.month, myTime.year);
        sleep(1);
    }
    return 0;
}

static int start_rtc(int *fd) {
    *fd = open(RTC, O_RDONLY);
    return *fd;
}
static int update_time(int *fd) {
    return ioctl(*fd, UPDATE_TIME, NULL);
}
static int get_time(int *fd, date_t *time) {
    return ioctl(*fd, GET_TIME, time);
}
static void stop_rtc(int *fd)
{
    close(*fd);
}
