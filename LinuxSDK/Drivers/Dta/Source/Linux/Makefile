# ========================================================================================
# Makefile for Dta Linux device driver
# ========================================================================================
TARGET := Dta

DRIVER_NAME = $(TARGET)

# ========================================================================================
# Directory / file structure definition

COMMON_DIR := ../../../../Common/Source

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

# Force using inline code to mitigate against straight line speculation in stead of using
# the current default thunk-extern which generates duplicate functions.
EXTRA_CFLAGS += -mindirect-branch=thunk-inline

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

EXTRA_CFLAGS += -I$(MYPWD) -I$(MYPWD)/$(SRCDIR) -I$(MYPWD)/$(COMMON_DIR)
EXTRA_CFLAGS += -I$(MYPWD)/$(SALSRCDIR) -I$(MYPWD)/$(DRVCOMMONSRCDIR)

# ========================================================================================
# Preprocessor definitions

EXTRA_CFLAGS += -D_DRIVER_NAME=$(DRIVER_NAME)                  \
                -DDTA_VERSION_MAJOR=$(DTA_VERSION_MAJOR) \
                -DDTA_VERSION_MINOR=$(DTA_VERSION_MINOR) \
                -DDTA_VERSION_MICRO=$(DTA_VERSION_MICRO) \
                -DDTA_VERSION_BUILD=$(DTA_VERSION_BUILD)

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

PWD = $(shell pwd)
INSTALLDIR = /lib/modules/$(CURRENT)/kernel/drivers/misc

# ========================================================================================
# Default option

all:	PREREQ $(DTSAL) $(DTDRVCOMMON)
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

$(DTSAL): PREREQ
	$(MAKE) MYPWD=$(PWD) -C $(SALBUILDDIR) DRIVER_NAME=$(DRIVER_NAME) clean
	$(MAKE) -C $(SALBUILDDIR) DRIVER_NAME=$(DRIVER_NAME)
	
$(DTDRVCOMMON): PREREQ
	$(MAKE) MYPWD=$(PWD) -C $(DRVCOMMONBUILDDIR) DRIVER_NAME=$(DRIVER_NAME) clean
	$(MAKE) -C $(DRVCOMMONBUILDDIR) DRIVER_NAME=$(DRIVER_NAME)
# ========================================================================================
# Clean all option

clean:
	@if [ -f MakefilePrereq ]; then $(MAKE) -f MakefilePrereq clean ; fi 
	$(MAKE) MYPWD=$(PWD) -C $(KERNELDIR) M=$(PWD) clean
	$(MAKE) MYPWD=$(PWD) -C $(SALBUILDDIR) DRIVER_NAME=$(DRIVER_NAME) clean
	$(MAKE) MYPWD=$(PWD) -C $(DRVCOMMONBUILDDIR) DRIVER_NAME=$(DRIVER_NAME) clean
	
# ========================================================================================
endif

