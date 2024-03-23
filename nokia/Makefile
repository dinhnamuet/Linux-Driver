KDIR:=/lib/modules/`uname -r`/build
obj-m += nokia5110.o

all:
	make -C ${KDIR} M=`pwd` modules
clean:
	make -C ${KDIR} M=`pwd` clean
