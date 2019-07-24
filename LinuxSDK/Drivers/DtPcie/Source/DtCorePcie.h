//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCorePcie.h *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
//
// DtPcie driver - Interface for the DtPcie common driver, used by the IAL.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017 DekTec Digital Video B.V.
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

#ifndef __DTPCIE_DRV_H
#define __DTPCIE_DRV_H

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Defines +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcie log levels -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//// DMA
//#define  LOG_LEVEL_DMA           LOG_MAX
//// Toplevel related
//#define  LOG_LEVEL_DTPCIE        LOG_MAX
//// Events
//#define  LOG_LEVEL_EVENTS        LOG_MAX
// IAL related
#define  LOG_LEVEL_IAL           LOG_MIN


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTP device context +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieDeviceInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtPcieDeviceInfo
{
    // Common device info MACRO.
    // NOTE: must be the first member of the DtPcieDeviceInfo struct. This enables safe 
    // casting of a DtCorePcie pointer to a DtDeviceInfo pointer
    DT_DEVICE_INFO_COMMON_DATA;

    Int  m_SubDvc;              // Sub-device

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
    Int  m_PcieMaxPayloadSize;  // Maximum-Payload-Size (MPS)
    Int  m_PcieMaxReadRequestSize;  // Maximum-Read-Request-Size (MRRS)
} DtPcieDeviceInfo;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieChildDeviceData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
struct _DtPcieChildDeviceData
{
    Int  m_Index;
    struct _DtCorePcie*  m_pParentDeviceData;
    DtIalDataChild  m_IalData;         // IAL data for child network devices
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

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCorePcie definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtCorePcie object
#define COREPCIE_DEFAULT_PRECONDITIONS(pCore)      \
    DT_ASSERT(pCore!=NULL && pCore->m_Size==sizeof(DtCorePcie))

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#define MAX_NUM_FW_VARIANTS  16
#define MAX_NUM_FUNCTIONS  32
struct _DtCorePcie
{
    // Common device data. 
    // NOTE: must be the first member of the DtCorePcie struct. This enables safe 
    // casting of a DtCorePcie pointer to a DtCore pointer
    DT_CORE_COMMON_DATA;

    // General device information
    DtPcieDeviceInfo  m_DevInfo;

    // Register base mappings
    DtMemDesc  m_DtPcieRegs;
    // Register mappings (shortcut for m_DtPcieRegs->m_pKernel)
    volatile UInt8*  m_pRegBase;

    // Interrupt
    DtIrqObject  m_Interrupt;
    volatile DtIntEnableState  m_IntEnableState;  // Interrupt enable state

    DtFastMutex  m_ExclAccessMutex;

};

#ifndef  COMMON_CORE
// Helper macro to access cast DtCorePcie to DtCore
// NOTE: only works if there is a variable pCore of type DtCorePcie*
#define  COMMON_CORE   ((DtCore*)pCore)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore - Public versions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtCorePcie_Open(DtCorePcie* pCore, DtFileObject* pFile);
DtStatus  DtCorePcie_Close(DtCorePcie* pCore, DtFileObject* pFile);
DtStatus  DtCorePcie_Ioctl(DtCorePcie*, DtFileObject*,DtIoctlObject*);
DtStatus  DtCorePcie_IoctlChild(DtPcieChildDeviceData* pCore, DtFileObject* pFile,
                                                                   DtIoctlObject* pIoctl);
DtStatus  DtCorePcie_AcquireExclAccess(DtCorePcie*  pCore);
void      DtCorePcie_ReleaseExclAccess(DtCorePcie*  pCore);
//-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DRIVER - Public versions -.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtCorePcie_DRIVER_Init(DtEvtLog* pEvtObject);
void  DtCorePcie_DRIVER_Exit(void);
//-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE - Public versions -.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtCorePcie_DEVICE_Init(DtCorePcie* pCore);
DtStatus  DtCorePcie_DEVICE_PowerUp(DtCorePcie* pCore);
DtStatus  DtCorePcie_DEVICE_InterruptEnable(DtCorePcie* pCore);
DtStatus  DtCorePcie_DEVICE_PowerUpPost(DtCorePcie* pCore);
DtStatus  DtCorePcie_DEVICE_PowerDownPre(DtCorePcie* pCore);
DtStatus  DtCorePcie_DEVICE_InterruptDisable(DtCorePcie* pCore);
DtStatus  DtCorePcie_DEVICE_PowerDown(DtCorePcie* pCore);
DtStatus  DtCorePcie_DEVICE_ExitPre(DtCorePcie* pCore);
void      DtCorePcie_DEVICE_Exit(DtCorePcie* pCore);
Bool      DtCorePcie_DEVICE_Interrupt(DtCorePcie*, Int  MessageId);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCorePcie definition +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCorePcie -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtIoStubCorePcie
{
    DT_IOSTUB_CORE_COMMON_DATA;
}  DtIoStubCorePcie;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCorePcie public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubCorePcie_Close(DtIoStub*);
DtIoStubCore*  DtIoStubCorePcie_Open(DtCore*);

#endif // __DTPCIE_DRV_H
