KERNEL:=/home/namubuntu/linux
TOOLCHAIN:=aarch64-linux-gnu-

EXTRA_CFLAGS = -Wall
obj-m += mydriver.o
all:
	make ARCH=arm64 CROSS_COMPILE=${TOOLCHAIN} -C ${KERNEL} M=`pwd` modules
clean:
	make ARCH=arm64 -C ${KERNEL} M=`pwd` clean