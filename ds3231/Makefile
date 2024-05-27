KERNEL:=/home/namubuntu/linux
TOOLCHAIN:=aarch64-linux-gnu-
Src=./Src/
Inc=./Inc/
obj-m += ./Driver/ds3231.o
all:
	make ARCH=arm64 CROSS_COMPILE=${TOOLCHAIN} -C ${KERNEL} M=`pwd` modules
	${TOOLCHAIN}gcc -o test main.c ${Src}ds3231_lib.c -I ${Inc}
clean:
	make ARCH=arm64 -C ${KERNEL} M=`pwd` clean
	rm test
