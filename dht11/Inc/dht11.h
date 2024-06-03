#ifndef __DHT11_H__
#define __DHT11_H__
#define LOW     0
#define HIGH    1
#define DHT11 "/dev/dht11"
typedef unsigned char u8;

typedef struct{
    u8 integer_temp;
    u8 decimal_temp;
    u8 integer_hum;
    u8 decimal_hum;
} dht_sensor_t;

typedef enum {
    SUCCESS,
    OPEN_FAILED,
    IOCTLFAILED
} dev_return_t;

typedef enum {
    TEMPERATURE,
    HUMIDITY
} property_t;

#define READ_DHT _IOR('R', 1, dht_sensor_t *)
int dht11_read_all(const char *path_to_dev, double *temperature, double *humidity);
double dht11_read_property(const char *path_to_dev, property_t property);

#endif
