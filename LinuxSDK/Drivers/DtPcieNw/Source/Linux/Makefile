# ========================================================================================
# Makefile for DtPcieNw Linux network driver
# ========================================================================================

TARGET := DtPcieNw

DRIVER_NAME = $(TARGET)

# ========================================================================================
# Directory / file structure definition

SALDIR := ../../../DtSal
SALSRCDIR = $(SALDIR)/Source
SALBUILDDIR = $(SALDIR)/Source/Linux
DTSAL = $(SALBUILDDIR)/DtSal$(DRIVER_NAME).o

COMMON_DIR := ../../../../Common/Source
DTPCIE_COMMON := ../../../DtPcie/Source
DTPCIE_LINUX := ../../../DtPcie/Source/Linux

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

KBUILD_EXTRA_SYMBOLS := $(MYPWD)/$(DTA_LINUX)/Module.symvers
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

EXTRA_CFLAGS += -DNW_DRIVER

# ========================================================================================
# Include version evironment vars

include $(MYPWD)/../Version.inc

# ========================================================================================
# Source definitions

SRCDIR = ..
SRC    = $(SRCDIR)/Linux/DtPcieNwIal.o         \
	$(SRCDIR)/DtPcieNw.o	\
		 Messages.o

EXTRA_CFLAGS += -I$(MYPWD) -I$(MYPWD)/$(SRCDIR) -I$(MYPWD)/$(COMMON_DIR)
EXTRA_CFLAGS += -I$(MYPWD)/$(SALSRCDIR)
EXTRA_CFLAGS += -I$(MYPWD)/$(DTPCIE_LINUX) -I$(MYPWD)/$(DTPCIE_COMMON)

# ========================================================================================
# Preprocessor definitions

EXTRA_CFLAGS += -D_DRIVER_NAME=$(DRIVER_NAME)                  \
                -DDTPCIENW_VERSION_MAJOR=$(DTPCIENW_VERSION_MAJOR) \
                -DDTPCIENW_VERSION_MINOR=$(DTPCIENW_VERSION_MINOR) \
                -DDTPCIENW_VERSION_MICRO=$(DTPCIENW_VERSION_MICRO) \
                -DDTPCIENW_VERSION_BUILD=$(DTPCIENW_VERSION_BUILD) \
                -DDTPCIENW_VERSION=$(DTPCIENW_VERSION)

# ========================================================================================
# Objects to build

obj-m := $(TARGET).o
$(TARGET)-objs = $(SRC) $(DTSAL)

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

all: PREREQ $(DTPCIE_LINUX)/Module.symvers Messages.h Messages.c $(DTSAL)
	cp $(DTPCIE_LINUX)/Module.symvers ./Module.symvers
	make MYPWD=$(PWD) -C $(KERNELDIR) M=$(PWD) modules

$(DTPCIE_LINUX)/Module.symvers:
	make MYPWD=$(PWD) -C $(DTPCIE_LINUX)

INSTALL_CMD := "/sbin/rmmod $(TARGET) 2>/dev/null || true &&          \
				install -d $(INSTALLDIR) &&     \
	            cp -v $(TARGET).ko $(INSTALLDIR) &&         \
			    /sbin/depmod -a &&                  \
			    /sbin/modprobe $(TARGET) && \
			    if [ -f /etc/modules ]; \
			    then \
			        ( grep -q $(TARGET) /etc/modules || echo $(TARGET) >> /etc/modules ) \
			    else \
			        if [ -f /etc/rc.local ]; \
			        then \
			            ( grep -q $(TARGET) /etc/rc.local || \
			                                  echo modprobe $(TARGET) >> /etc/rc.local ) \
			        fi \
			    fi "

install:
	su -c $(INSTALL_CMD)


# ========================================================================================
# Prerequisites

PREREQ:
	@if [ -f MakefilePrereq ]; then make -f MakefilePrereq ; fi

$(DTSAL):
	make MYPWD=$(PWD) -C $(SALBUILDDIR) DRIVER_NAME=$(DRIVER_NAME)

# ========================================================================================
# Clean all option

clean:
	@if [ -f MakefilePrereq ]; then make -f MakefilePrereq clean ; fi
	make MYPWD=$(PWD) -C $(KERNELDIR) M=$(PWD) clean
	make MYPWD=$(PWD) -C $(SALBUILDDIR) DRIVER_NAME=$(DRIVER_NAME) clean

# ========================================================================================
endif

