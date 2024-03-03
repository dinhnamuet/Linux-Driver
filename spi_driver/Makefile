BBB_KERNEL := /home/nam/linux/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/nam/linux/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-

EXTRA_CFLAGS=-Wall
obj-m := sx1278.o

all:
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules
clean:
	make -C $(BBB_KERNEL) M=$(shell pwd) clean
