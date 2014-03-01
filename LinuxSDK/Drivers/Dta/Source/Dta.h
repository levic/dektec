//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dta.h *#*#*#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
//
// Dta driver - Interface for the Dta common driver, used by the IAL.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
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
#define  LOG_LEVEL_IAL           LOG_AVG
// Toplevel related
#define  LOG_LEVEL_DTA           LOG_AVG
// VPD related
#define  LOG_LEVEL_VPD           LOG_MIN
// I2C related
#define  LOG_LEVEL_I2C           LOG_MIN
// Events
#define  LOG_LEVEL_EVENTS        LOG_MAX
// DMA
#define  LOG_LEVEL_DMA           LOG_AVG
// TARGET DETECTION
#define  LOG_LEVEL_TRGDET        LOG_MAX
// IoConfig
#define  LOG_LEVEL_IOCONFIG      LOG_MAX
// Shared Buffer
#define  LOG_LEVEL_SHBUF         LOG_MAX
// Phy Mac
#define  LOG_LEVEL_PHYMAC        LOG_AVG
// NONIP
#define  LOG_LEVEL_NONIP         LOG_AVG
// MATRIX
#define  LOG_LEVEL_MATRIX        LOG_AVG
// IP
#define  LOG_LEVEL_IP            LOG_MAX
// IP Rx
#define  LOG_LEVEL_IP_RX         LOG_AVG
// IP Tx
#define  LOG_LEVEL_IP_TX         LOG_MAX
// IP Address Matcher
#define  LOG_LEVEL_IPADDRM       LOG_MAX
// Ping-pong buffer
#define  LOG_LEVEL_PP            LOG_MIN
// Genlock
#define  LOG_LEVEL_GENL          LOG_AVG


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
    UInt64  m_Serial;           // Serial number
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
    Int  m_Index;
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

    // Registry
    Bool   m_RegistryWriteBusy;
                   
    // VPD
    DtaVpd  m_Vpd;

    // I2C
    DtaI2c  m_I2c;

    // Matrix-API (HD-SDI)
    DtaMatrix  m_Matrix;

    // Genlock
    DtaGenlock  m_Genlock;

    // Saved power measurement lock data
    UInt m_RfPwrPreLockData;

    // Device DPC's
    DtDpc  m_GenPerIntDpc;

    // Port lookup
    DtaPortLookup*  m_pPortLookup;
    Int  m_NumPorts;

    // Non IP ports
    DtaNonIpPort*  m_pNonIpPorts;
    DtFastMutex  m_ExclAccessMutex;
    Int  m_NumNonIpPorts;

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

Bool  DtaDeviceInterrupt(DtaDeviceData* pDvcData);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
DtStatus  DtaGetNonIpPortIndex(DtaDeviceData* pDvcData, Int PortIndex,
                                                                    Int* pNonIpPortIndex);
DtStatus  DtaGetPortIndexNonIp(DtaDeviceData* pDvcData, Int NonIpPortIndex, 
                                                                         Int* pPortIndex);
DtStatus  DtaGetIpPortIndex(DtaDeviceData* pDvcData, Int PortIndex, Int* pIpPortIndex);
DtStatus  DtaGetPortIndexIp(DtaDeviceData* pDvcData, Int IpPortIndex, Int* pPortIndex);

void  DtaRfPwrMeasLock(DtaDeviceData* pDvcData, Int PortIndex, Int Lock);

#endif // __DTA_DRV_H
