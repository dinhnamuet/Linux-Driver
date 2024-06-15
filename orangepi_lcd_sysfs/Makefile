KERNEL=/home/namubuntu/buildroot/output/build/linux-6.8.12
TOOLCHAIN=/home/namubuntu/buildroot/output/host/bin/aarch64-buildroot-linux-gnu-
obj-m += lcd.o

all:
	make ARCH=arm64 CROSS_COMPILE=${TOOLCHAIN} -C ${KERNEL} M=`pwd` modules
clean:
	make ARCH=arm64 CROSS_COMPILE=${TOOLCHAIN} -C ${KERNEL} M=`pwd` clean
