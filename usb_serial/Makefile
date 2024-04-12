KERNEL := /home/nam/linux
TOOLCHAIN := aarch64-linux-gnu-

EXTRA_CFLAGS = -Wall
obj-m += usb_to_ser.o
all:
	make ARCH=arm64 CROSS_COMPILE=${TOOLCHAIN} -C ${KERNEL} M=`pwd` modules
clean:
	make -C ${KERNEL} M=`pwd` clean