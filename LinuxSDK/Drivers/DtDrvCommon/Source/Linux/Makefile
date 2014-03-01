# ========================================================================================
# Makefile for DtDrvCommon Linux device driver library
# ========================================================================================

TARGET := DtDrvCommon

SALDIR := ../../../DtSal
SALSRCDIR = $(SALDIR)/Source

COMMON_DIR := ../../../../Common/Source

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

ifeq ("$(DRIVER_NAME)", "DtaNw")
	EXTRA_CFLAGS += -DNW_DRIVER
endif

ifeq ("$(DRIVER_NAME)", "Dtu")
	EXTRA_CFLAGS += -DUSB_DRIVER
else
	EXTRA_CFLAGS += -DSKIP_USB
endif

# ========================================================================================
# Source definitions

SRCDIR := ..
SRC     = $(SRCDIR)/DtProperties.o	    \
		  $(SRCDIR)/IoConfigCodes.o     \
		  $(SRCDIR)/DtTables.o          \
		  $(SRCDIR)/DtUtilities.o
		  
EXTRA_CFLAGS += -I$(MYPWD)/$(SRCDIR) -I$(MYPWD)/$(COMMON_DIR) -I$(MYPWD)/$(SALSRCDIR)

# ========================================================================================
# Preprocessor definitions

ifndef DRIVER_NAME
DRIVER_NAME := DtDrvCommon
endif

EXTRA_CFLAGS += -D_DRIVER_NAME=$(DRIVER_NAME)

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
	make MYPWD=$(PWD) -C $(KERNELDIR) SUBDIRS=$(PWD) modules

# ========================================================================================
# Clean all option

clean:
	make MYPWD=$(PWD) -C $(KERNELDIR) SUBDIRS=$(PWD) clean

# ========================================================================================
endif

