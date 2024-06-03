.PHONY: all clean mkObj driver library install
KERNEL=/lib/modules/`uname -r`/build
obj-m += ./Driver/mySensor.o
CC:=gcc
Src:=`pwd`/Src
Inc:=`pwd`/Inc
Obj:=`pwd`/Obj
BIN:=`pwd`/Bin
USR_LIB:=`pwd`/output/libs
OUTPUT_DRV = `pwd`/output/driver
OUTPUT_INC = `pwd`/output/inc

all: library driver
	cp Inc/* ${OUTPUT_INC}
driver:
	make -C ${KERNEL} M=`pwd` modules
	cp ./Driver/mySensor.ko ${OUTPUT_DRV}
mkObj:
	${CC} -c ${Src}/dht11.c -o ${Obj}/dht11.o -I ${Inc}
library: mkObj
	${CC} -shared -o ${USR_LIB}/libdht11.so ${Obj}/dht11.o
install:
	install ${USR_LIB}/libdht11.so /usr/lib
	install ${Inc}/dht11.h /usr/include
clean:
	make -C ${KERNEL} M=`pwd` clean
	rm ${USR_LIB}/* ${OUTPUT_INC}/* test
test:
	gcc -o test main.c -ldht11
