KDIR:=/lib/modules/`uname -r`/build
obj-m += uart.o

all:
	make -C ${KDIR} M=`pwd` modules
clean:
	make -C ${KDIR} M=`pwd` clean
