KERNEL=../linux
TOOLCHAIN=arm-linux-gnueabihf-
obj-m += mySensor.o
all:
	make ARCH=arm CROSS_COMPILE=${TOOLCHAIN} -C ${KERNEL} M=`pwd` modules
	${TOOLCHAIN}gcc -o test main.c dht_ioctl.c -I .
clean:
	make ARCH=arm CROSS_COMPILE=${TOOLCHAIN} -C ${KERNEL} M=`pwd` clean
	rm test
