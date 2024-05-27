#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ds3231_lib.h"

int main(int argc, char *argv[]) {
    date_t myTime;
    update_time(DS3231_DEV);
    while (1) {
        if (get_time(DS3231_DEV, &myTime) == 0)
            printf ("%s, %d: %d: %d - %d/%d/%d\n", myTime.day, myTime.hour, myTime.min, myTime.sec, myTime.date, myTime.month, myTime.year);
        sleep(1);
    }
    return 0;
}
