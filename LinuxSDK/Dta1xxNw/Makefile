# ========================================================================================
# Makefile for Dta1xx Linux Network device driver
# ========================================================================================

# ========================================================================================
# KERNELDIR can be speficied on the command line or in the environment. Below you will
# find the most common paths (undefine the correct one)

# Get kernel version
CURRENT = $(shell uname -r)

#Enable these flags to allow debug information to be compiled in
#EXTRA_CFLAGS += -g -Wall -DDEBUG

ifndef KERNELDIR
	KERNELDIR = /lib/modules/$(CURRENT)/build/include
	#KERNELDIR = /usr/src/linux-2.4.20-3/include
	#KERNELDIR = /usr/include/linux/include
endif

# ========================================================================================
# Common code
# ========================================================================================

# check for SMP
ifdef CONFIG_SMP
	EXTRA_CFLAGS += -D__SMP__ -DSMP
endif

# ========================================================================================
# TARGET/Source definitions

# extension depends on kernel version
ifneq (,$(findstring 2.4.,$(CURRENT)))
	EXT = c
else
	EXT = o
endif

SRCDIR = ./Source/
TARGET = Dta1xxNw
SRC    = $(SRCDIR)Dta1xxNw.$(EXT) $(SRCDIR)CrossPlatform.$(EXT)
DEP    = ../Dta1xx

# Installation directory
INSTALLDIR = /lib/modules/$(CURRENT)/kernel/drivers/misc

# ========================================================================================
# Kernel 2.4 makefile:
# ========================================================================================

ifneq (,$(findstring 2.4.,$(CURRENT)))

# ========================================================================================
# Set include directories

INCLUDEDIR = $(KERNELDIR) -I./Include

# ========================================================================================
# Compile flags

CFLAGS += -O2 -D__KERNEL__ -DMODULE -I$(INCLUDEDIR) -DDTA1XXNWDRIVER

# Enable version support (look in config.h\autoconf.h and check if CONFIG_MODVERSIONS
# is defined)
MODVER = $(shell if cat $(KERNELDIR)/linux/config.h $(KERNELDIR)/linux/autoconf.h \
		2>/dev/null | grep -q \
		'^[[:space:]]*\#define[[:space:]]*CONFIG_MODVERSIONS[[:space:]]*1'; \
		then echo 1; else echo 0; fi)
ifeq ($(MODVER),1)
	CFLAGS += -DMODVERSIONS -include $(KERNELDIR)/linux/modversions.h
endif


# ========================================================================================
# Make all option

all: $(TARGET).ko
$(TARGET).ko: $(SRC:.c=.o)
		$(LD) -r $^ -o $@

# ========================================================================================
# Install option

install:
	install -d $(INSTALLDIR)
	install -c $(TARGET).ko $(INSTALLDIR)


# ========================================================================================
# Kernel 2.6 makefile
# ========================================================================================

else

# ========================================================================================
# Variables

EXTRA_CFLAGS = -DDTA1XXNWDRIVER
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)

# ========================================================================================
# Objects

$(TARGET)-objs := $(SRC)
obj-m := $(TARGET).o

# ========================================================================================
# Default option

# We depend on exported symbols in the DTA1xx driver, so this path must be added to
# KBUILD_EXTMOD. This must only be done when we are called by the kernel, but we only know
# the path when we are called the first time. Solution: save depency path when called the
# first time and add dependency path to KBUILD_EXTMOD when called by the kernel.
#ifeq ($(KERNELRELEASE),)
#	export EXTMOD_DTA1XX:=$(PWD)/../Dta1xx
#else
#	export KBUILD_EXTMOD:=$(EXTMOD_DTA1XX)
#endif

# Another solution is to copy the Module.symvers from the Dta1xx driver to the Dta1xxNw
# driver. During the build of the Dta1xxNw driver the Dta1xxNw symbols are added to the
# file, so the Dta1xx symbols also remain in this file.

default: ../Dta1xx/Module.symvers
	cp ../Dta1xx/Module.symvers ./Module.symvers
	make -C $(KDIR) SUBDIRS=$(PWD) modules

../Dta1xx/Module.symvers:
	make -C ../Dta1xx


# ========================================================================================
# Target compiling

$(TARGET).o: $(SRC)
	$(LD) $(LD_RFLAG) -r -o $@ $(SRC)

# ========================================================================================
# Install option

install:
	# If the makefile failes to create the $(INSTALLDIR) execute the remove command below.
	# WARNING: this will delete all files in the $(INSTALLDIR) directory. Make a backup of
	# important files in this directory
	# rm -d -r -f $(INSTALLDIR)

	install -d $(INSTALLDIR)
	su -c "cp -v $(TARGET).ko $(INSTALLDIR) && /sbin/depmod -a"

endif

# ========================================================================================
# End of kernel specific code
# ========================================================================================



# ========================================================================================
# Clean all option

clean:
	rm -f $(SRCDIR)*.o $(SRCDIR)*.ko $(SRCDIR).*.o.cmd $(SRCDIR)*~ $(SRCDIR)core $(SRCDIR).depend
	rm -f *.o *.ko .*.o.cmd .*.ko.cmd *.mod.c Module.????ers
	rm -rf .tmp_versions

-include $(KERNELDIR)/Rules.make
