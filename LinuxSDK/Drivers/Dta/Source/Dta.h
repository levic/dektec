//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dta.h *#*#*#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
//
// Dta driver - Interface for the Dta common driver, used by the IAL.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2016 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL DEKTEC DIGITAL VIDEO BV, ITS AGENTS OR ITS EMPLOYEES BE LIABLE FOR
// ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL, OR OTHER DAMAGES (INCLUDING DAMAGES
// FOR THE LOSS OF USE, INFORMATION, GOODWILL, PROFIT, WORK STOPPAGE, DATA, BUSINESS OR
// REVENUE) UNDER ANY CIRCUMSTANCES, OR UNDER ANY LEGAL THEORY, WHETHER IN CONTRACT, IN
// TORT, IN NEGLIGENCE, OR OTHERWISE, ARISING FROM THE USE OF, OR INABILITY TO USE THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#ifndef __DTA_DRV_H
#define __DTA_DRV_H

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Defines +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
#define  DTA_TAG        0x32425444  // '2ATD'

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA log levels -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// IAL related
#define  LOG_LEVEL_IAL           LOG_MAX
// Toplevel related
#define  LOG_LEVEL_DTA           LOG_AVG
// I2C related
#define  LOG_LEVEL_I2C           LOG_MIN
// Events
#define  LOG_LEVEL_EVENTS        LOG_MIN
// DMA
#define  LOG_LEVEL_DMA           LOG_AVG
// TARGET DETECTION
#define  LOG_LEVEL_TRGDET        LOG_MAX
// Shared Buffer
#define  LOG_LEVEL_SHBUF         LOG_AVG
// Phy Mac
#define  LOG_LEVEL_PHYMAC        LOG_MIN
// NONIP
#define  LOG_LEVEL_NONIP         LOG_AVG
// MATRIX
#define  LOG_LEVEL_MATRIX        LOG_AVG
// RS-422
#define  LOG_LEVEL_RS422         LOG_AVG
// IP
#define  LOG_LEVEL_IP            LOG_AVG
// IP Rx
#define  LOG_LEVEL_IP_RX         LOG_AVG
// IP Rx FEC Reconstructor
#define  LOG_LEVEL_IP_RX_REC     LOG_MIN
// IP Tx
#define  LOG_LEVEL_IP_TX         LOG_AVG
// IP Address Matcher
#define  LOG_LEVEL_IPADDRM       LOG_AVG
// Ping-pong buffer
#define  LOG_LEVEL_PP            LOG_MIN
// Genlock
#define  LOG_LEVEL_GENL          LOG_AVG
// Fan
#define  LOG_LEVEL_FAN           LOG_AVG
// Dac AD9129
#define  LOG_LEVEL_DAC           LOG_AVG
// SDI-AV-RX
#define  LOG_LEVEL_SDIAVRX       LOG_AVG
// HDMI
#define  LOG_LEVEL_HDMI          LOG_AVG
// D7Pro Encoder
#define  LOG_LEVEL_D7PRO         LOG_AVG
// SPI master flash
#define  LOG_LEVEL_SPIMF         LOG_AVG
// Programming Interface
#define  LOG_LEVEL_PROGITF       LOG_AVG

#define  USES_GENREGS(pDvcData)  (pDvcData->m_DevInfo.m_TypeNumber!=100                  \
                                      && pDvcData->m_DevInfo.m_TypeNumber!=102           \
                                      && !((pDvcData->m_DevInfo.m_TypeNumber==122)       \
                                           && (pDvcData->m_DevInfo.m_FirmwareVersion<4)) )


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA device context +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtaDeviceInfo
{
    Int  m_TypeNumber;
    Int  m_SubDvc;

    // PCI properties, loaded by IAL
    UInt16  m_DeviceId;         // Device ID
    UInt16  m_VendorId;         // Vendor ID
    UInt16  m_SubVendorId;      // Subsystem Vendor ID
    UInt16  m_SubSystemId;      // Subsystem ID
    UInt  m_BusNumber;          // PCI Bus Number
    UInt  m_SlotNumber;         // PCI Slot Number
    Int  m_PcieNumLanes;        // Number of allocated PCIe lanes
    Int  m_PcieMaxLanes;        // Maximum number of PCIe lanes
    Int  m_PcieLinkSpeed;       // Current PCIe link speed
    Int  m_PcieMaxSpeed;        // Maximum PCIe link speed

    // Info determined during DtaDeviceInit
    Bool  m_UsesPlxChip;
    Bool  m_Uses9056;
    Bool  m_HasUninitializedPlx;

    // Info determined during DtaDeviceInitPost
    Int  m_HardwareRevision;    // Hardware revision (e.g. 302 = 3.2)
    Int  m_FirmwareVersion;     // Firmware Version (= Altera revision), e.g. 3 for
                                // "Firmware Version 3"
    Int  m_FirmwareVariant;     // Firmware Variant, e.g. to distinguish between
                                // firmware with different #inputs/#outputs
    Int  m_FwPackageVersion;    // Firmware Package version, version number given
                                // to the set of firmware variants
    UInt64  m_Serial;           // Serial number
    UInt64  m_UniqueId;         // Unique board ID (can be same as serial)
    Int  m_SubType;             // Device subtype (0=none, 1=A, ...)
    UInt32  m_RefClk;           // Reference clock
    UInt8  m_PerIntClkBit;      // Bit of the reference clock used for periodic interrupt
    Bool  m_HasPerIntItvReg;    // Indicates whether device has a PerIntItv register
    UInt32  m_PerIntItvUS;      // Periodic interrupt interval in micro seconds
    Bool  m_HasExtClockFreq;    // Indicates if firmware supports an external clock 
                                // frequency register
} DtaDeviceInfo;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFileHandle -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtaFileHandleInfo  DtaFileHandleInfo;
struct _DtaFileHandleInfo
{
    DtaFileHandleInfo*  m_pNext;
    DtaFileHandleInfo*  m_pPrev;
    void*  m_pHandle;
    Bool  m_PowerDownPending;
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPortLookup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#define  DTA_PORT_TYPE_NONE   0
#define  DTA_PORT_TYPE_NONIP  1
#define  DTA_PORT_TYPE_IP     2
typedef struct _DtaPortLookup
{
    Int  m_PortType;
    Int  m_Index;           // Index in NonIp or IP port array
    Int  m_PortNumber;      // Port number as seen by user
} DtaPortLookup;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaChildDeviceData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
struct _DtaChildDeviceData
{
    Int  m_Index;
    struct _DtaDeviceData*  m_pParentDeviceData;
    DtaIalDataChild  m_IalData;         // IAL data for child network devices
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIntEnableState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Interrupt enable state
typedef enum _DtIntEnableState
{
    INT_DISABLED=0,         // Interrupts are disabled
    INT_DISABLING,          // Disabling interrupts
    INT_ENABLING,           // Enabling interrupts
    INT_ENABLED             // Interrupts are enabled
} DtIntEnableState;         

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#define MAX_NUM_FW_VARIANTS  16 
struct _DtaDeviceData
{
    // IAL data
    DtaIalData  m_IalData;
    
    // Driver / Device objects
    DtDrvObject  m_Driver;
    DtDvcObject  m_Device;

    // Property Data
    DtPropertyData  m_PropData;

    // General device information
    DtaDeviceInfo  m_DevInfo;
    
    // Register base mappings
    DtMemDesc  m_DtaRegs;
    DtMemDesc  m_Pci905XConfRegs;

    // User mappings
    DtFastMutex  m_UserMapMutex;
    DtUserMapping*  m_pUserMapping;

    // File handles
    DtFastMutex  m_FileHandleInfoMutex;
    DtaFileHandleInfo*  m_pFileHandleInfo;

    // Events
    DtSpinLock  m_EventsSpinlock;
    DtaEvents*  m_pEvents;

    // Register mappings (shortcut for m_DtaRegs->m_pKernel)
    volatile UInt8*  m_pGenRegs;

    // Interrupt
    DtIrqObject  m_Interrupt;
    volatile DtIntEnableState  m_IntEnableState;  // Interrupt enable state
        
    // Used during powerup to determine what should be initialised
    Bool  m_InitialPowerup;
    Bool  m_PowerupReady;

    // Registry
    Bool   m_RegistryWriteBusy;
    DtEvent  m_RegWriteDoneEvt;
                   
    // VPD
    DtVpd  m_Vpd;

    // I2C
    DtaI2c  m_I2c;

    // Matrix-API (HD-SDI)
    DtaMatrix  m_Matrix;

    // Genlock
    DtaGenlock  m_Genlock;

    // FanControl
    DtaFanControl  m_FanControl;

    // SPI master flash controller
    DtaSpiMf  m_SpiMf;

    // Multi-modulator RF-Level data
    DtaMultiModData  m_MultiMod;

    // Programming interfaces
    DtaProgItf  m_ProgItfs[2];

    // Saved power measurement lock data
    UInt  m_RfPwrPreLockData;

    // Firmware reboot pending, will be executed during DtaDevicePowerDown
    Bool  m_FwRebootPending;
    Int  m_FwRebootDelay;

    // Device DPC's
    DtDpc  m_GenPerIntDpc;

    // Port lookup
    DtaPortLookup*  m_pPortLookup;
    Int  m_NumPorts;

    // Non IP ports
    DtaNonIpPort*  m_pNonIpPorts;
    Int  m_NumNonIpPorts;
    DtFastMutex  m_ExclAccessMutex;

    // IP ports
    DtaIpDevice  m_IpDevice;
    Int  m_NumIpPorts;

    // DMA Object
    DmaDevice  m_DmaDevice;
    DmaOpt  m_DmaOptions;
};


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA Common interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

DtStatus  DtaDriverInit(DtEvtLog* pEvtObject);
void  DtaDriverExit(void);

DtStatus  DtaDeviceInit(DtaDeviceData* pDvcData);
DtStatus  DtaDeviceParsePciConfig(DtaDeviceData* pDvcData, UInt8* pPciConfig);
DtStatus  DtaDeviceInitPci905X(DtaDeviceData* pDvcData);
DtStatus  DtaDevicePowerUp(DtaDeviceData* pDvcData);
DtStatus  DtaDeviceInterruptEnable(DtaDeviceData* pDvcData);
DtStatus  DtaDevicePowerUpPost(DtaDeviceData* pDvcData);

DtStatus  DtaDevicePowerDownPre(DtaDeviceData* pDvcData);
DtStatus  DtaDeviceInterruptDisable(DtaDeviceData* pDvcData);
DtStatus  DtaDevicePowerDown(DtaDeviceData* pDvcData);
DtStatus  DtaDeviceExitPre(DtaDeviceData* pDvcData);
void  DtaDeviceExit(DtaDeviceData* pDvcData);

DtStatus  DtaDeviceOpen(DtaDeviceData* pDvcData, DtFileObject* pFile);
DtStatus  DtaDeviceClose(DtaDeviceData* pDvcData, DtFileObject* pFile);
DtStatus  DtaDeviceIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                                                   DtIoctlObject* pIoctl);
DtStatus  DtaDeviceIoctlChild(DtaChildDeviceData* pDvcData, DtFileObject* pFile,
                                                                   DtIoctlObject* pIoctl);

DtStatus  DtaDeviceAcquireExclAccess(DtaDeviceData*  pDvcData);
void  DtaDeviceReleaseExclAccess(DtaDeviceData*  pDvcData);

Bool  DtaDeviceInterrupt(DtaDeviceData* pDvcData);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
DtStatus  DtaGetNonIpPortIndex(DtaDeviceData* pDvcData, Int PortIndex,
                                                                    Int* pNonIpPortIndex);
DtStatus  DtaGetPortIndexNonIp(DtaDeviceData* pDvcData, Int NonIpPortIndex, 
                                                                         Int* pPortIndex);
DtStatus  DtaGetIpPortIndex(DtaDeviceData* pDvcData, Int PortIndex, Int* pIpPortIndex);
DtStatus  DtaGetPortIndexIp(DtaDeviceData* pDvcData, Int IpPortIndex, Int* pPortIndex);
DtStatus  DtaGetPortNumber(DtaDeviceData* pDvcData, Int PortIndex, Int* pPortNumber);

void  DtaRfPwrMeasLock(DtaDeviceData* pDvcData, Int PortIndex, Int Lock);

#endif // __DTA_DRV_H
