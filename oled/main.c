#include <stdio.h>
#include "oled.h"

int main(int argc, char *argv[]) {
    oled_init(OLED);
    oled_write(OLED, "EMBEDDED LINUX\nOLED DRIVER\nI2C PROTOCOL\n-- END --");
    return 0;
}
