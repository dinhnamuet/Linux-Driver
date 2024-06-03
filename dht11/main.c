#include <stdio.h>
#include <sys/signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <dht11.h>

int main(int argc, char *argv[]) {
   double temperature, humidity;
    while (1) {
        // temperature = dht11_read_property(DHT11, TEMPERATURE);
        // humidity = dht11_read_property(DHT11, HUMIDITY);
        if (!dht11_read_all(DHT11, &temperature, &humidity))
            printf("Temperature: %.2f°C, Humidity: %.2f\n", temperature, humidity);
        sleep(2);
    }
    return 0;
}
