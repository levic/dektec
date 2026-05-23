# ========================================================================================
# Makefile for DtSal Linux device driver library
# ========================================================================================

TARGET := DtSal$(DRIVER_NAME)

COMMON_DIR := ../../../../Common/Source

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

# Check for 32/64 bit system
ccflags-y += -DLINBUILD
ifeq ($(NUMLBITS), 32)
    ccflags-y += -DLIN32
else
    ccflags-y += -DLIN64
endif

ifeq ("$(DRIVER_NAME)", "DtPcieNw")
	ccflags-y += -DNW_DRIVER
endif

ifeq ("$(DRIVER_NAME)", "DtaNw")
	ccflags-y += -DNW_DRIVER
endif

ifeq ("$(DRIVER_NAME)", "Dtu")
	ccflags-y += -DUSB_DRIVER -DSKIP_DMA
else
	ccflags-y += -DSKIP_USB
endif

# ========================================================================================
# Source definitions

SRCDIR := ..
SRC     = $(SRCDIR)/DtDma.o					\
		  $(SRCDIR)/DtMutex.o				\
		  $(SRCDIR)/DtSpinLock.o			\
		  $(SRCDIR)/DtEvent.o				\
		  $(SRCDIR)/DtTrigEvt.o				\
		  $(SRCDIR)/DtNonVolatileSettings.o	\
		  $(SRCDIR)/DtString.o				\
		  $(SRCDIR)/DtDpc.o                 \
		  $(SRCDIR)/DtWorkItem.o			\
		  $(SRCDIR)/DtThread.o				\
		  $(SRCDIR)/DtUtility.o				\
		  $(SRCDIR)/DtList.o				\
		  $(SRCDIR)/DtUsb.o
		  

ccflags-y += -I$(MYPWD)/$(SRCDIR) -I$(MYPWD)/$(COMMON_DIR)

# ========================================================================================
# Preprocessor definitions

ifndef DRIVER_NAME
DRIVER_NAME := DtSal
endif

ccflags-y += -D_DRIVER_NAME=$(DRIVER_NAME)

# ========================================================================================
# Objects to build

obj-m = $(TARGET).o
$(TARGET)-objs = $(SRC)

# ========================================================================================
# Objects to clean

clean-files = $(SRC)

# ========================================================================================
# Normal part of makefile
# ========================================================================================
else

PWD = $(shell pwd)

# ========================================================================================
# Default option

all:
	$(MAKE) MYPWD=$(PWD) -C $(KERNELDIR) M=$(PWD) clean
	$(MAKE) MYPWD=$(PWD) -C $(KERNELDIR) KBUILD_MODPOST_NOFINAL=1 KBUILD_MODPOST_WARN=1 M=$(PWD) modules

# ========================================================================================
# Clean all option

clean:
	$(MAKE) MYPWD=$(PWD) -C $(KERNELDIR) M=$(PWD) clean

# ========================================================================================
endif

