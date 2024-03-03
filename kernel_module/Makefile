.PHONY: all clean
KDIR=/lib/modules/`uname -r`/build
obj-m += character_driver.o
all:
	make -C ${KDIR} M=`pwd` modules
clean:
	make -C ${KDIR} M=`pwd` clean