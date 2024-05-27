# KDIR:=/lib/modules/`uname -r`/build
# obj-m += ssd1306.o

# all:
# 	make -C ${KDIR} M=`pwd` modules
# clean:
# 	make -C ${KDIR} M=`pwd` clean
KERNEL=/home/namubuntu/linux
TOOLCHAIN=aarch64-linux-gnu-
#arm-linux-gnueabihf-
obj-m += ./Driver/ssd1306.o
all:
	make ARCH=arm64 CROSS_COMPILE=${TOOLCHAIN} -C ${KERNEL} M=`pwd` modules
	${TOOLCHAIN}gcc -o test main.c ./Src/oled.c -I ./Inc
clean:
	make ARCH=arm64 CROSS_COMPILE=${TOOLCHAIN} -C ${KERNEL} M=`pwd` clean
	rm test
