KERNEL:=/home/namubuntu/linux
TOOLCHAIN:=aarch64-linux-gnu-

obj-m += pwmled.o
all:
	make ARCH=arm64 CROSS_COMPILE=${TOOLCHAIN} -C ${KERNEL} M=`pwd` modules
	${TOOLCHAIN}gcc -o test test.c
clean:
	make ARCH=arm64 CROSS_COMPILE=${TOOLCHAIN} -C ${KERNEL} M=`pwd` clean
	rm test
