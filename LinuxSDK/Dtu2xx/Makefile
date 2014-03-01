# ========================================================================================
# Top-level makefile for the Dtu2xx Linux device driver
# ========================================================================================

# Export the directory name of the top-level makefile for sub-make processes
export MODDIR = $(shell pwd)

all:
	cd Source ; make all

genfw:
	cd Source ; make genfw

clean:
	make -C Source clean

install: all
	sudo install -d /lib/modules/`uname -r`/kernel/drivers/usb/DekTec/
	sudo cp Source/Dtu2xx.ko /lib/modules/`uname -r`/kernel/drivers/usb/DekTec/
	sudo /sbin/depmod -a
	sudo /sbin/rmmod Dtu2xx || true

.PHONY: clean all install
