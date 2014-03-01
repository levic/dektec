# ========================================================================================
# Makefile for Dta1xx Linux device driver
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
TARGET = Dta1xx
SRC    =  $(SRCDIR)CrossPlatform.$(EXT) $(SRCDIR)Dma.$(EXT) $(SRCDIR)Dta1xx.$(EXT) \
          $(SRCDIR)EepromControl.$(EXT) $(SRCDIR)Exclusive.$(EXT) $(SRCDIR)Flags.$(EXT) \
          $(SRCDIR)Genlock.$(EXT) $(SRCDIR)Init.$(EXT) $(SRCDIR)Interrupt.$(EXT) \
          $(SRCDIR)IoCtl.$(EXT) $(SRCDIR)LowLevel.$(EXT) $(SRCDIR)MapMemory.$(EXT) \
          $(SRCDIR)Rate.$(EXT) $(SRCDIR)Spi.$(EXT) $(SRCDIR)Target.$(EXT) \
          $(SRCDIR)Utility.$(EXT) $(SRCDIR)Vpd.$(EXT) $(SRCDIR)NwPhyMac.$(EXT) \
          $(SRCDIR)NwIpTx.$(EXT) $(SRCDIR)NwIpRx.$(EXT) $(SRCDIR)NwNdis.$(EXT)

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

CFLAGS += -O2 -D__KERNEL__ -DMODULE -I$(INCLUDEDIR) -DEXPORT_SYMTAB

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

EXTRA_CFLAGS = -DEXPORT_SYMTAB
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)

# ========================================================================================
# Objects

$(TARGET)-objs := $(SRC)
obj-m := $(TARGET).o

# ========================================================================================
# Default option

default:
	make -C $(KDIR) SUBDIRS=$(PWD) modules

# ========================================================================================
# Target compiling

$(TARGET).o: $(SRC)
	$(LD) $(LD_RFLAG) -r -o $@ $(SRC)

# ========================================================================================
# Install option

install:
	install -d $(INSTALLDIR)
	su -c "cp -v $(TARGET).ko $(INSTALLDIR) && /sbin/depmod -a"

endif

# ========================================================================================
# End of kernel specific code
# ========================================================================================



# ========================================================================================
# Clean all option

clean:
	rm -f $(SRCDIR)*.o $(SRCDIR)*.ko $(SRCDIR)*.o.cmd $(SRCDIR)*~ $(SRCDIR)core $(SRCDIR).depend
	rm -f *.o *.ko .*.o.cmd .*.ko.cmd *.mod.c
	rm -rf .tmp_versions

-include $(KERNELDIR)/Rules.make
