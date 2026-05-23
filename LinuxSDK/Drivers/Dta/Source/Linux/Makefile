# ========================================================================================
# Makefile for Dta Linux device driver
# ========================================================================================
TARGET := Dta

DRIVER_NAME = $(TARGET)

# ========================================================================================
# Directory / file structure definition

COMMON_DIR := ../../../../Common/Source

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
SRC    = $(SRCDIR)/Linux/DtaIal.o         \
         $(SRCDIR)/Dta.o                  \
         $(SRCDIR)/DtaUtility.o           \
         $(SRCDIR)/Eeprom.o               \
         $(SRCDIR)/I2c.o                  \
         $(SRCDIR)/Vpd.o                  \
         $(SRCDIR)/Events.o               \
         $(SRCDIR)/NonIp.o                \
         $(SRCDIR)/NonIpMatrix.o          \
         $(SRCDIR)/NonIpTx.o              \
         $(SRCDIR)/NonIpRx.o              \
         $(SRCDIR)/Ip.o                   \
         $(SRCDIR)/IpTx.o                 \
         $(SRCDIR)/IpRx.o                 \
         $(SRCDIR)/DtaPropertyStore.o     \
         $(SRCDIR)/IoConfig.o             \
         $(SRCDIR)/Dma.o                  \
         $(SRCDIR)/ShBuffer.o             \
         $(SRCDIR)/NwPhyMac.o             \
         $(SRCDIR)/DtaTableStore.o        \
         $(SRCDIR)/DtaMatrix.o            \
         $(SRCDIR)/Genlock.o              \
         $(SRCDIR)/Gs296x.o               \
         $(SRCDIR)/Lmh1982.o              \
         $(SRCDIR)/Lmh1983.o              \
         $(SRCDIR)/FanControl.o           \
         $(SRCDIR)/Max6639.o              \
         $(SRCDIR)/FpgaGenlock.o          \
         $(SRCDIR)/Rs422.o                \
         $(SRCDIR)/SdiAvRx.o              \
         $(SRCDIR)/Uart.o                 \
         $(SRCDIR)/EnDec.o                \
         $(SRCDIR)/EncD7Pro.o             \
         $(SRCDIR)/Ad9129.o               \
         $(SRCDIR)/Hdmi.o                 \
         $(SRCDIR)/SpiMf.o                \
         $(SRCDIR)/ProgItf.o              \
         $(SRCDIR)/HdmiTx.o               \
         $(SRCDIR)/M23145.o               \
         $(SRCDIR)/M235x4.o               \
         $(SRCDIR)/M23528.o               \
         $(SRCDIR)/Linux/DtaDeviceTable.o \
         Messages.o

ccflags-y += -I$(MYPWD) -I$(MYPWD)/$(SRCDIR) -I$(MYPWD)/$(COMMON_DIR)
ccflags-y += -I$(MYPWD)/$(SALSRCDIR) -I$(MYPWD)/$(DRVCOMMONSRCDIR)

# ========================================================================================
# Preprocessor definitions

ccflags-y += -D_DRIVER_NAME=$(DRIVER_NAME)            \
             -DDTA_VERSION_MAJOR=$(DTA_VERSION_MAJOR) \
             -DDTA_VERSION_MINOR=$(DTA_VERSION_MINOR) \
             -DDTA_VERSION_MICRO=$(DTA_VERSION_MICRO) \
             -DDTA_VERSION_BUILD=$(DTA_VERSION_BUILD)

# ========================================================================================
# set subdirs flags identical to main flags and add extras for DtSal and DtDrvCommon build
subdir-ccflags-y := $(ccflags-y) -DSKIP_USB

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

PWD = $(shell pwd)
INSTALLDIR = /lib/modules/$(CURRENT)/kernel/drivers/misc

# ========================================================================================
# Default option

all:	clean PREREQ
	$(MAKE) MYPWD=$(PWD) -C $(KERNELDIR) M=$(PWD) modules

INSTALL_CMD := "/sbin/rmmod $(TARGET) 2>/dev/null || true &&          \
				install -d $(INSTALLDIR) &&     \
	            cp -v $(TARGET).ko $(INSTALLDIR) &&         \
			    /sbin/depmod -a &&                  \
			    install -o root -g root -m 644 51-dta.rules /etc/udev/rules.d/" && \
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

