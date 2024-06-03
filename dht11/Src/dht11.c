#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include "dht11.h"
static double combine_result(u8 integer, u8 decimal) {
    char a[7];
    memset(a, 0, 7);
    snprintf(a, sizeof(a), "%d.%d", integer, decimal);
    return atof(a);
}
static int dht11_read(const char *path_to_dev, dht_sensor_t *mySensor) {
    int fd = open(path_to_dev, O_RDONLY);
    if (-1 == fd)
        return -OPEN_FAILED;
    if (ioctl(fd, READ_DHT, mySensor) < 0) {
        close(fd);
        return -IOCTLFAILED;
    }
    close(fd);
    return SUCCESS;
}
int dht11_read_all(const char *path_to_dev, double *temperature, double *humidity) {
    dht_sensor_t bar;
    if (dht11_read(path_to_dev, &bar) < 0)
        return -1;
    *temperature = combine_result(bar.integer_temp, bar.decimal_temp);
    *humidity = combine_result(bar.integer_hum, bar.decimal_hum);
    return 0;
}
double dht11_read_property(const char *path_to_dev, property_t property) {
    dht_sensor_t foo;
    if (dht11_read(path_to_dev, &foo) < 0)
        return -1;
    if (property == TEMPERATURE)
        return combine_result(foo.integer_temp, foo.decimal_temp);
    else if (property == HUMIDITY)
        return combine_result(foo.integer_hum, foo.decimal_hum);
    else
        return -1;
}
