# ========================================================================================
# Makefile for Dtu Linux device driver
# ========================================================================================

TARGET := Dtu

DRIVER_NAME = $(TARGET)

# ========================================================================================
# Directory / file structure definition

COMMON_DIR:= ../../../../Common/Source

SALDIR := ../../../DtSal
SALSRCDIR = $(SALDIR)/Source
SALBUILDDIR = $(SALDIR)/Source/Linux
DTSAL = $(SALBUILDDIR)/DtSal$(DRIVER_NAME).o

DRVCOMMONDIR := ../../../DtDrvCommon
DRVCOMMONSRCDIR = $(DRVCOMMONDIR)/Source
DRVCOMMONBUILDDIR = $(DRVCOMMONDIR)/Source/Linux
DTDRVCOMMON = $(DRVCOMMONBUILDDIR)/DtDrvCommon$(DRIVER_NAME).o

NUMLBITS = $(shell getconf LONG_BIT)

# ========================================================================================
# KERNELDIR can be speficied on the command line or in the environment. Below you will
# find the most common paths (undefine the correct one)

# Get kernel version
CURRENT = $(shell uname -r)

ifndef KERNELDIR
	KERNELDIR = /lib/modules/$(CURRENT)/build
	#KERNELDIR = /usr/src/linux-2.4.20-3
	#KERNELDIR = /usr/include/linux
endif

# ========================================================================================
# kbuild part of makefile
# ========================================================================================
ifneq ($(KERNELRELEASE),)

# Check for debug
ifdef DEBUG
	EXTRA_CFLAGS += -g -Wall -D_DEBUG
endif

# Check for SMP
ifdef CONFIG_SMP
	EXTRA_CFLAGS += -D__SMP__ -DSMP
endif

# Check for 32/64 bit system
EXTRA_CFLAGS += -DLINBUILD
ifeq ($(NUMLBITS), 32)
    EXTRA_CFLAGS += -DLIN32
else
    EXTRA_CFLAGS += -DLIN64
endif

# Add EXPORT_SYMTAB to have a Module.symvers file generated, which contains the symbol
# version information of the exported symbols in the Dta driver. These symbols are used
# by the DtaNw driver and version info is required during the modpost of the DtaNw
# driver.
EXTRA_CFLAGS += -DEXPORT_SYMTAB

# ========================================================================================
# Include version evironment vars

include $(MYPWD)/../Version.inc

# ========================================================================================
# Source definitions

SRCDIR = ..
SRC    = $(SRCDIR)/Linux/DtuIal.o         \
         $(SRCDIR)/Linux/DtuDeviceTable.o \
         $(SRCDIR)/Dtu.o                  \
         $(SRCDIR)/DtuUtility.o           \
         $(SRCDIR)/DtuDevice.o            \
         $(SRCDIR)/Eeprom.o               \
         $(SRCDIR)/I2c.o                  \
         $(SRCDIR)/Vpd.o                  \
         $(SRCDIR)/Events.o               \
         $(SRCDIR)/NonIp.o                \
         $(SRCDIR)/DtuPropertyStore.o     \
         $(SRCDIR)/IoConfig.o             \
         $(SRCDIR)/Pld.o                  \
         $(SRCDIR)/ReadWrite.o            \
         $(SRCDIR)/ShBuffer.o             \
         $(SRCDIR)/Firmware.o             \
         $(SRCDIR)/EzUsb.o                \
         $(SRCDIR)/DtuRegs.o              \
         $(SRCDIR)/DtuDemodFwStore.o      \
         $(SRCDIR)/DtuFwStore.o           \
         $(SRCDIR)/Demod.o                \
         $(SRCDIR)/DtuTableStore.o        \
         Messages.o

EXTRA_CFLAGS += -I$(MYPWD) -I$(MYPWD)/$(SRCDIR) -I$(MYPWD)/$(COMMON_DIR)
EXTRA_CFLAGS += -I$(MYPWD)/$(SALSRCDIR) -I$(MYPWD)/$(DRVCOMMONSRCDIR)

# ========================================================================================
# Preprocessor definitions

EXTRA_CFLAGS += -D_DRIVER_NAME=$(DRIVER_NAME)                  \
                -DDTU_VERSION_MAJOR=$(DTU_VERSION_MAJOR) \
                -DDTU_VERSION_MINOR=$(DTU_VERSION_MINOR) \
                -DDTU_VERSION_MICRO=$(DTU_VERSION_MICRO) \
                -DDTU_VERSION_BUILD=$(DTU_VERSION_BUILD)
                
EXTRA_CFLAGS += -DUSB_DRIVER

# ========================================================================================
# Objects to build

obj-m := $(TARGET).o
$(TARGET)-objs = $(SRC) $(DTSAL) $(DTDRVCOMMON)

# ========================================================================================
# Objects to clean

clean-files = $(SRC)


# ========================================================================================
# Normal part of makefile
# ========================================================================================
else

# ========================================================================================
# Check udev version
UDEV_VERSION := $(shell udevinfo -V 2>/dev/null | cut -d " " -f 3)
ifeq ($(UDEV_VERSION),)
	UDEV_VERSION := $(shell udevadm --version | cut -d " " -f 1)
endif

UDEV_FILE := $(shell [ $(UDEV_VERSION) -le 98 ] && echo 51-dtu-old.rules || \
                                                                        echo 51-dtu.rules)

PWD = $(shell pwd)
INSTALLDIR = /lib/modules/$(CURRENT)/kernel/drivers/usb/DekTec

# ========================================================================================
# Default option

all: PREREQ $(DTSAL) $(DTDRVCOMMON)
	make MYPWD=$(PWD) -C $(KERNELDIR) M=$(PWD) modules

INSTALL_CMD := "/sbin/rmmod $(TARGET) 2>/dev/null || true &&          \
				install -d $(INSTALLDIR) &&     \
	            cp -v $(TARGET).ko $(INSTALLDIR) && \
			    /sbin/depmod -a &&                  \
			   install -o root -g root -m 644 $(UDEV_FILE) /etc/udev/rules.d/51-dtu.rules" && \
			    /sbin/modprobe $(TARGET)

install:
	su -c $(INSTALL_CMD)


# ========================================================================================
# Prerequisites

PREREQ:
	@if [ -f MakefilePrereq ]; then make -f MakefilePrereq ; fi

$(DTSAL):
	make MYPWD=$(PWD) -C $(SALBUILDDIR) DRIVER_NAME=$(DRIVER_NAME) clean
	make MYPWD=$(PWD) -C $(SALBUILDDIR) DRIVER_NAME=$(DRIVER_NAME)
	
$(DTDRVCOMMON):
	make MYPWD=$(PWD) -C $(DRVCOMMONBUILDDIR) DRIVER_NAME=$(DRIVER_NAME) clean
	make MYPWD=$(PWD) -C $(DRVCOMMONBUILDDIR) DRIVER_NAME=$(DRIVER_NAME)
# ========================================================================================
# Clean all option

clean:
	@if [ -f MakefilePrereq ]; then make -f MakefilePrereq clean ; fi
	make MYPWD=$(PWD) -C $(KERNELDIR) M=$(PWD) clean
	make MYPWD=$(PWD) -C $(SALBUILDDIR) DRIVER_NAME=$(DRIVER_NAME) clean
	make MYPWD=$(PWD) -C $(DRVCOMMONBUILDDIR) DRIVER_NAME=$(DRIVER_NAME) clean
	
# ========================================================================================
endif

