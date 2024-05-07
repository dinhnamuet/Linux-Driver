KERNEL = /home/namubuntu/linux
TOOLCHAIN = aarch64-linux-gnu-

obj-m += led.o


all:
	make ARCH=arm64 CROSS_COMPILE=$(TOOLCHAIN) -C ${KERNEL} M=`pwd` modules
clean:
	make ARCH=arm64 CROSS_COMPILE=$(TOOLCHAIN) -C ${KERNEL} M=`pwd` clean
