# ========================================================================================
# Makefile for DtPcie Linux device driver
# ========================================================================================
TARGET := DtPcie

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
EXTRA_CFLAGS += -DLINBUILD -DUSE_DF_EXCL_ACCESS
ifeq ($(NUMLBITS), 32)
    EXTRA_CFLAGS += -DLIN32
else
    EXTRA_CFLAGS += -DLIN64
endif

# Add EXPORT_SYMTAB to have a Module.symvers file generated, which contains the symbol
# version information of the exported symbols in the DtPcie driver. These symbols are used
# by the DtPcieNw driver and version info is required during the modpost of the DtPcieNw
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
SRC    = $(SRCDIR)/Linux/DtCorePcie_IAL.o     \
         $(SRCDIR)/Linux/DtPcieDeviceTable.o  \
         $(SRCDIR)/DtCore.o                   \
         $(SRCDIR)/DtCore_BC.o                \
         $(SRCDIR)/DtCore_DEBUG.o             \
         $(SRCDIR)/DtCore_DEVICE.o            \
         $(SRCDIR)/DtCore_DF.o                \
         $(SRCDIR)/DtCore_EVENTS.o            \
         $(SRCDIR)/DtCore_INTERRUPT.o         \
         $(SRCDIR)/DtCore_IOCONFIG.o          \
         $(SRCDIR)/DtCore_PROPS.o             \
         $(SRCDIR)/DtCore_PT.o                \
         $(SRCDIR)/DtCore_SETTINGS.o          \
         $(SRCDIR)/DtCore_TOD.o               \
         $(SRCDIR)/DtCore_VPD.o               \
         $(SRCDIR)/DtCorePcie.o               \
         $(SRCDIR)/DtBc.o                     \
         $(SRCDIR)/DtBcACCUFIFO.o             \
         $(SRCDIR)/DtBcAD9789ITF.o            \
         $(SRCDIR)/DtBcASIRXP.o               \
         $(SRCDIR)/DtBcASITXG.o               \
         $(SRCDIR)/DtBcASITXSER.o             \
         $(SRCDIR)/DtBcATTNCTRL_2116.o        \
         $(SRCDIR)/DtBcBURSTFIFO.o            \
         $(SRCDIR)/DtBcCDMAC.o                \
         $(SRCDIR)/DtBcCDMACTO.o              \
         $(SRCDIR)/DtBcCONSTSINK.o            \
         $(SRCDIR)/DtBcCONSTSOURCE.o          \
         $(SRCDIR)/DtBcDATAC.o                \
         $(SRCDIR)/DtBcDATACDC.o              \
         $(SRCDIR)/DtBcDATAFIFO.o             \
         $(SRCDIR)/DtBcDDRFIFO.o              \
         $(SRCDIR)/DtBcDDRFRONT.o             \
         $(SRCDIR)/DtBcDISEQC.o               \
         $(SRCDIR)/DtBcEMAC10G.o              \
         $(SRCDIR)/DtBcETHIPPAD.o             \
         $(SRCDIR)/DtBcFANC.o                 \
         $(SRCDIR)/DtBcFPGATEMP.o             \
         $(SRCDIR)/DtBcGENL.o                 \
         $(SRCDIR)/DtBcGPSTIME.o              \
         $(SRCDIR)/DtBcGS2988.o               \
         $(SRCDIR)/DtBcI2CM.o                 \
         $(SRCDIR)/DtBcIO_2116.o              \
         $(SRCDIR)/DtBcIOSERIN.o              \
         $(SRCDIR)/DtBcIOPARIN.o              \
         $(SRCDIR)/DtBcIPFIFO.o               \
         $(SRCDIR)/DtBcIPROUT.o               \
         $(SRCDIR)/DtBcIPSCHED.o              \
         $(SRCDIR)/DtBcIPSECG.o               \
         $(SRCDIR)/DtBcIPSUMCHK.o             \
         $(SRCDIR)/DtBcIPSUMINS.o             \
         $(SRCDIR)/DtBcIQAGC.o                \
         $(SRCDIR)/DtBcIQDEMOD.o              \
         $(SRCDIR)/DtBcIQDOWNSRC.o            \
         $(SRCDIR)/DtBcIQFIR.o                \
         $(SRCDIR)/DtBcIQFIR2XCLK.o           \
         $(SRCDIR)/DtBcIQINV.o                \
         $(SRCDIR)/DtBcIQNOISE.o              \
         $(SRCDIR)/DtBcIQMISC.o               \
         $(SRCDIR)/DtBcIQPWR.o                \
         $(SRCDIR)/DtBcIQSYNC.o               \
         $(SRCDIR)/DtBcIQUNPCK.o              \
         $(SRCDIR)/DtBcIQUP.o                 \
         $(SRCDIR)/DtBcIQJESD.o               \
         $(SRCDIR)/DtBcKA.o                   \
         $(SRCDIR)/DtBcLEDB.o                 \
         $(SRCDIR)/DtBcLMH1981.o              \
         $(SRCDIR)/DtBcMSIX.o                 \
         $(SRCDIR)/DtBcREBOOT.o               \
         $(SRCDIR)/DtBcS12GTO3G.o             \
         $(SRCDIR)/DtBcSDIDMX12G.o            \
         $(SRCDIR)/DtBcSDIMUX12G.o            \
         $(SRCDIR)/DtBcSDIRXF.o               \
         $(SRCDIR)/DtBcSDIRXP.o               \
         $(SRCDIR)/DtBcSDIRXPHY.o             \
         $(SRCDIR)/DtBcSDITXF.o               \
         $(SRCDIR)/DtBcSDITXP.o               \
         $(SRCDIR)/DtBcSDITXPHY.o             \
         $(SRCDIR)/DtBcSDITXPLL.o             \
         $(SRCDIR)/DtBcSDIXCFG.o              \
         $(SRCDIR)/DtBcSI2166ITF.o            \
         $(SRCDIR)/DtBcSPIM.o                 \
         $(SRCDIR)/DtBcSPIMF.o                \
         $(SRCDIR)/DtBcST425LR.o              \
         $(SRCDIR)/DtBcSWITCH.o               \
         $(SRCDIR)/DtBcTSRXFMT.o              \
         $(SRCDIR)/DtBcTOD.o                  \
         $(SRCDIR)/DtBcVVI.o                  \
         $(SRCDIR)/DtCfEvt.o                  \
         $(SRCDIR)/DtCfInt.o                  \
         $(SRCDIR)/DtCfIoCfg.o                \
         $(SRCDIR)/DtCfProps.o                \
         $(SRCDIR)/DtCfTod.o                  \
         $(SRCDIR)/DtDf.o                     \
         $(SRCDIR)/DtDfAdf4360Ctrl.o          \
         $(SRCDIR)/DtDfAsiRx.o                \
         $(SRCDIR)/DtDfCh.o                   \
         $(SRCDIR)/DtDfChSdiRx.o              \
         $(SRCDIR)/DtDfChSdiRxPhyOnly.o       \
         $(SRCDIR)/DtDfClkCtrl_2116.o         \
         $(SRCDIR)/DtDfClkGen_2110.o          \
         $(SRCDIR)/DtDfDataFifo.o             \
         $(SRCDIR)/DtDfIpFifo.o               \
         $(SRCDIR)/DtDfNw.o                   \
         $(SRCDIR)/DtDfGenLockCtrl.o          \
         $(SRCDIR)/DtDfMcTc72Temp.o           \
         $(SRCDIR)/DtDfMxDs75Temp.o           \
         $(SRCDIR)/DtDfSdiRx.o                \
         $(SRCDIR)/DtDfSdiTxPhy.o             \
         $(SRCDIR)/DtDfSdiXCfgMgr.o           \
         $(SRCDIR)/DtDfSensTemp.o             \
         $(SRCDIR)/DtDfSi534X.o               \
         $(SRCDIR)/DtDfSpiCableDrvEq.o        \
         $(SRCDIR)/DtDfSpiProm.o              \
         $(SRCDIR)/DtDfTodClockCtrl.o         \
         $(SRCDIR)/DtDfTempFanMgr.o           \
         $(SRCDIR)/DtDfTxPllMgr.o             \
         $(SRCDIR)/DtDfVirtGenRef.o           \
         $(SRCDIR)/DtDfVpd.o                  \
         $(SRCDIR)/DtPt.o                     \
         $(SRCDIR)/DtPtAsiSdiMon.o            \
         $(SRCDIR)/DtPtAsiSdiRxTx.o           \
         $(SRCDIR)/DtPtIp.o                   \
         $(SRCDIR)/DtPtModOutp.o              \
         $(SRCDIR)/DtPtSdiPhyOnlyRxTx.o       \
         $(SRCDIR)/DtPtSdiGenRef.o            \
         $(SRCDIR)/DtIoStub.o                 \
         $(SRCDIR)/DtVectorBc.o               \
         $(SRCDIR)/DtVectorDf.o               \
         $(SRCDIR)/DtVectorDfChUser.o         \
         $(SRCDIR)/DtVectorPt.o               \
         $(SRCDIR)/DtVectorPipe.o             \
         $(SRCDIR)/DtCorePcie.o               \
         $(SRCDIR)/DtCorePcie_DEVICE.o        \
         $(SRCDIR)/DtPciePropertyStore.o      \
         $(SRCDIR)/DtPcieTableStore.o         \
         $(SRCDIR)/DtPcieUtility.o            \
         $(SRCDIR)/DtBcAD5320_2132.o          \
         $(SRCDIR)/DtBcADS4246_2132.o         \
         $(SRCDIR)/DtBcDATASTMUX_2132.o       \
         $(SRCDIR)/DtBcIQCGRAB_2132.o         \
         $(SRCDIR)/DtBcIQFIR_2132.o           \
         $(SRCDIR)/DtBcIQPWR_2132.o           \
         $(SRCDIR)/DtBcIQSRC2132_2132.o       \
         $(SRCDIR)/DtBcLNBH25_2132.o          \
         $(SRCDIR)/DtBcS2DEC_2132.o           \
         $(SRCDIR)/DtBcS2DEMOD_2132.o         \
         $(SRCDIR)/DtBcS2STATS_2132.o         \
         $(SRCDIR)/DtDfS2CrDemod_2132.o       \
         $(SRCDIR)/DtDfTxClkCtrl_2178A.o      \
         $(SRCDIR)/DtBcXPLL.o                 \
         $(SRCDIR)/DtPipe.o                   \
         Messages.o

EXTRA_CFLAGS += -I$(MYPWD) -I$(MYPWD)/$(SRCDIR) -I$(MYPWD)/$(COMMON_DIR)
EXTRA_CFLAGS += -I$(MYPWD)/$(SALSRCDIR) -I$(MYPWD)/$(DRVCOMMONSRCDIR)

# ========================================================================================
# Preprocessor definitions

EXTRA_CFLAGS += -D_DRIVER_NAME=$(DRIVER_NAME)                  \
                -DDTPCIE_VERSION_MAJOR=$(DTPCIE_VERSION_MAJOR)  \
                -DDTPCIE_VERSION_MINOR=$(DTPCIE_VERSION_MINOR)  \
                -DDTPCIE_VERSION_MICRO=$(DTPCIE_VERSION_MICRO)  \
                -DDTPCIE_VERSION_BUILD=$(DTPCIE_VERSION_BUILD)

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
			    install -o root -g root -m 644 51-dtpcie.rules /etc/udev/rules.d/" && \
			    /sbin/modprobe $(TARGET)

install:
	su -c $(INSTALL_CMD)


# ========================================================================================
# Prerequisites

PREREQ:
	@if [ -f MakefilePrereq ]; then $(MAKE) -f MakefilePrereq ; fi

$(DTSAL):
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

