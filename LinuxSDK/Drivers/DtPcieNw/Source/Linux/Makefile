# ========================================================================================
# Makefile for DtPcieNw Linux network driver
# ========================================================================================

TARGET := DtPcieNw

DRIVER_NAME = $(TARGET)

# ========================================================================================
# Directory / file structure definition

SALDIR := ../../../DtSal
SALSRCDIR := $(SALDIR)/Source
SALSRC := $(wildcard $(MYPWD)/$(SALSRCDIR)/*.c)
SALOBJ := $(patsubst $(MYPWD)/%,%,$(SALSRC:.c=.o))

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

KBUILD_EXTRA_SYMBOLS := $(MYPWD)/$(DTPCIE_LINUX)/Module.symvers
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

ccflags-y += -DNW_DRIVER

# ========================================================================================
# Include version evironment vars

include $(MYPWD)/../Version.inc

# ========================================================================================
# Source definitions

SRCDIR = ..
SRC    = $(SRCDIR)/Linux/DtPcieNwIal.o \
		 $(SRCDIR)/DtPcieNw.o          \
		 Messages.o

ccflags-y += -I$(MYPWD) -I$(MYPWD)/$(SRCDIR) -I$(MYPWD)/$(COMMON_DIR)
ccflags-y += -I$(MYPWD)/$(SALSRCDIR)
ccflags-y += -I$(MYPWD)/$(DTPCIE_LINUX) -I$(MYPWD)/$(DTPCIE_COMMON)

# ========================================================================================
# Preprocessor definitions

ccflags-y += -D_DRIVER_NAME=$(DRIVER_NAME)                      \
             -DDTPCIENW_VERSION_MAJOR=$(DTPCIENW_VERSION_MAJOR) \
             -DDTPCIENW_VERSION_MINOR=$(DTPCIENW_VERSION_MINOR) \
             -DDTPCIENW_VERSION_MICRO=$(DTPCIENW_VERSION_MICRO) \
             -DDTPCIENW_VERSION_BUILD=$(DTPCIENW_VERSION_BUILD) \
             -DDTPCIENW_VERSION=$(DTPCIENW_VERSION)

# ========================================================================================
# set subdirs flags identical to main flags and add extras for DtSal build
subdir-ccflags-y := $(ccflags-y) -DSKIP_USB

# ========================================================================================
# Objects to build

obj-m := $(TARGET).o
$(TARGET)-objs = $(SRC) $(SALOBJ) 


# ========================================================================================
# Objects to clean

clean-files = $(SRC) $(SALOBJ)


# ========================================================================================
# Normal part of makefile
# ========================================================================================
else

PWD = $(shell pwd)
INSTALLDIR = /lib/modules/$(CURRENT)/kernel/drivers/misc

# ========================================================================================
# Default option

all: clean PREREQ $(DTPCIE_LINUX)/Module.symvers
	cp $(DTPCIE_LINUX)/Module.symvers ./Module.symvers
	$(MAKE) MYPWD=$(PWD) -C $(KERNELDIR) M=$(PWD) modules

$(DTPCIE_LINUX)/Module.symvers:
	$(MAKE) MYPWD=$(PWD) -C $(DTPCIE_LINUX)

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
	@if [ -f MakefilePrereq ]; then $(MAKE) -f MakefilePrereq ; fi

# ========================================================================================
# Clean all option

clean:
	@if [ -f MakefilePrereq ]; then $(MAKE) -f MakefilePrereq clean ; fi
	$(MAKE) MYPWD=$(PWD) -C $(KERNELDIR) M=$(PWD) clean

# ========================================================================================
endif

