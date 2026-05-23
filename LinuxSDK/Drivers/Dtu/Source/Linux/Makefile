# ========================================================================================
# Makefile for Dtu Linux device driver
# ========================================================================================

TARGET := Dtu

DRIVER_NAME = $(TARGET)

# ========================================================================================
# Directory / file structure definition

COMMON_DIR:= ../../../../Common/Source

SALDIR := ../../../DtSal
SALSRCDIR := $(SALDIR)/Source
SALSRC := $(wildcard $(MYPWD)/$(SALSRCDIR)/*.c)
SALOBJ := $(patsubst $(MYPWD)/%,%,$(SALSRC:.c=.o))

DRVCOMMONDIR := ../../../DtDrvCommon
DRVCOMMONSRCDIR := $(DRVCOMMONDIR)/Source
DRVCOMMONSRC := $(wildcard $(MYPWD)/$(DRVCOMMONSRCDIR)/*.c)
DRVCOMMONOBJ := $(patsubst $(MYPWD)/%,%,$(DRVCOMMONSRC:.c=.o))

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
	ccflags-y += -g -Wall -D_DEBUG
endif

# Check for SMP
ccflags-$(CONFIG_SMP) += -D__SMP__ -DSMP

# Add default KBUILD module CFLAGS 
ccflags-y += $(KBUILD_CFLAGS_MODULE)

# Check for 32/64 bit system
ccflags-y += -DLINBUILD
ifeq ($(NUMLBITS), 32)
    ccflags-y += -DLIN32
else
    ccflags-y += -DLIN64
endif

# Add EXPORT_SYMTAB to have a Module.symvers file generated, which contains the symbol
# version information of the exported symbols in the Dta driver. These symbols are used
# by the DtaNw driver and version info is required during the modpost of the DtaNw
# driver.
ccflags-y += -DEXPORT_SYMTAB

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

ccflags-y += -I$(MYPWD) -I$(MYPWD)/$(SRCDIR) -I$(MYPWD)/$(COMMON_DIR)
ccflags-y += -I$(MYPWD)/$(SALSRCDIR) -I$(MYPWD)/$(DRVCOMMONSRCDIR)

# ========================================================================================
# Preprocessor definitions

ccflags-y += -D_DRIVER_NAME=$(DRIVER_NAME)            \
             -DDTU_VERSION_MAJOR=$(DTU_VERSION_MAJOR) \
             -DDTU_VERSION_MINOR=$(DTU_VERSION_MINOR) \
             -DDTU_VERSION_MICRO=$(DTU_VERSION_MICRO) \
             -DDTU_VERSION_BUILD=$(DTU_VERSION_BUILD)
                
ccflags-y += -DUSB_DRIVER

# ========================================================================================
# set subdirs flags identical to main flags and add extras for DtSal and DtDrvCommon build
subdir-ccflags-y := $(ccflags-y) -DSKIP_DMA


# ========================================================================================
# Objects to build

obj-m := $(TARGET).o
$(TARGET)-objs = $(SRC) $(SALOBJ) $(DRVCOMMONOBJ)

# ========================================================================================
# Objects to clean

clean-files = $(SRC) $(SALOBJ) $(DRVCOMMONOBJ)


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

all: clean PREREQ
	$(MAKE) MYPWD=$(PWD) -C $(KERNELDIR) M=$(PWD) modules

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
	@if [ -f MakefilePrereq ]; then $(MAKE) -f MakefilePrereq ; fi

# ========================================================================================
# Clean all option

clean:
	@if [ -f MakefilePrereq ]; then $(MAKE) -f MakefilePrereq clean ; fi
	$(MAKE) MYPWD=$(PWD) -C $(KERNELDIR) M=$(PWD) clean
	
# ========================================================================================
endif

