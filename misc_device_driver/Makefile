KDIR:=/lib/modules/`uname -r`/build
obj-m += lcd_driver.o

all:
	make -C ${KDIR} M=`pwd` modules
	gcc -o run main.c lcd.c -I .
clean:
	make -C ${KDIR} M=`pwd` clean
	rm -rf run
