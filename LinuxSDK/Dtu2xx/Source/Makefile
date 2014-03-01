# ========================================================================================
# Makefile for Dtu2xx Linux device driver
# ========================================================================================

# ========================================================================================
# KERNELDIR can be speficied on the command line or in the environment. Below you will
# find the most common paths (undefine the correct one)

ifndef KERNELDIR
	KERNELDIR = /lib/modules/$(shell uname -r)/build
endif

# Comment out this line if you want debugging information compiled in your driver
#DEBUG = -g -Wall -DDEBUG

# Module directory (${MODDIR}) is parsed from top make file.
EXTRA_CFLAGS += -I${MODDIR}/Include ${DEBUG} -DDTU2XX_DRIVER

# ========================================================================================
# Install option

all:
	make -C ${KERNELDIR} M=$(shell pwd) modules

genfw:
	./GenFwFiles

obj-m := Dtu2xx.o

Dtu2xx-objs := DeviceIoControl.o Dtu2xxMod.o Dtu215.o Dtu234.o Exclusive.o Flags.o \
               Init.o RxIoCtl.o TxIoCtl.o Usb.o Utility.o Vpd.o

clean:
	rm -f *.o *.ko .*.o.cmd .*.ko.cmd *.mod.c Module.????ers
	rm -rf .tmp_versions

.PHONY: all clean

