//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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


#ifndef __DT_CORE_H
#define __DT_CORE_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtIal.h"
typedef struct _DtCore  DtCore;
#include "DtIoStub.h"

// Forwards
typedef struct _DtBc  DtBc;
typedef struct _DtPt  DtPt;
typedef struct _DtObjectBcOrDf  DtObjectBcOrDf;
typedef struct DtVector  DtVectorBc;
typedef struct DtVector  DtVectorDf;
typedef struct DtVector  DtVectorPt;
typedef struct DtVector  DtVectorObject;
typedef struct _DtBcOrDfId  DtBcId;
typedef struct _DtBcOrDfId  DtDfId;
typedef struct _DtIoStubCore  DtIoStubCore;
typedef struct _DtBcMSIX  DtBcMSIX;
typedef struct _DtBcTOD  DtBcTOD;
typedef struct _DtBcVVI  DtBcVVI;
typedef struct _DtDfDebug  DtDfDebug;
typedef struct _DtDfEvent  DtDfEvent;
typedef struct _DtDfProps  DtDfProps;
typedef struct _DtDfVpd  DtDfVpd;
typedef struct _DtDfSettings  DtDfSettings;
typedef struct _DtDfIoConfig  DtDfIoConfig;
typedef struct _DtCfEvt  DtCfEvt;
typedef struct _DtCfEvtData  DtCfEvtData;
typedef struct _DtCfInt  DtCfInt;
typedef struct _DtCfIoCfg  DtCfIoCfg;
typedef struct _DtCfProps  DtCfProps;
typedef struct _DtCfTod  DtCfTod;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Defines / Constants +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Tag for memory allocated by the core
#define CORE_TAG  0x45524F43        // 'EROC'

// UUIDs to be used for core-BCs/DFs
#define CORE_BC_UUID        (DT_UUID_CORE_FLAG|DT_UUID_BC_FLAG|DT_UUID_CORE)
#define CORE_DF_UUID        (DT_UUID_CORE_FLAG|DT_UUID_DF_FLAG|DT_UUID_CORE)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFileHandleInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtFileHandleInfo  DtFileHandleInfo;
struct _DtFileHandleInfo
{
    DtFileHandleInfo*  m_pNext;
    DtFileHandleInfo*  m_pPrev;
    void*  m_pHandle;
    Bool  m_PowerDownPending;
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Interrupt-Handler +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIntHandlerRegData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// MARCO defining the members of the common interrupt handler registration struct
#define DT_INT_HANDLER_REGDATA_COMMON_DATA                                               \
    Int  m_Size;                /* Set to size of this struct OR of a 'derived' struct */\
    void*  m_pContext           /* Context object */
// The struct (including the common members)
// NOTE: 'derived' structs should include DT_INT_HANDLER_REGDATA_COMMON_DATA
// members first, so that the can be casted to a DtIntHandlerRegData struct
typedef struct  _DtIntHandlerRegData
{
    DT_INT_HANDLER_REGDATA_COMMON_DATA;
}  DtIntHandlerRegData;
#define INIT_DT_INT_HANDLER_REG_DATA(DATA, CONTEXT)                                      \
do                                                                                       \
{                                                                                        \
    DtMemZero(&DATA, sizeof(DATA));                                                      \
    DATA.m_Size = sizeof(DATA);                                                          \
    DATA.m_pContext = CONTEXT;                                                           \
} while (0)

// Prefix CM debug logging with the CM name
#define DtDbgOutCm(Level, Module, Cm, Msg, ...)                                          \
                DtDbgOut_Prefix(Level, CM_, Module, "[DF_CORE#1:%s] " Msg,               \
                                        Cm.m_pName,                                      \
                                        ##__VA_ARGS__)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Periodic-Interval-Event +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Signature of the periodic-interval-event handler callback function
typedef void  (*DtOnPeriodicIntervalFunc)(DtObject*, DtTodTime  Time);
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOnPeriodicRegData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Registration data for periodic-interval-event handlers
typedef struct  _DtOnPeriodicIntervalRegData
{
    DtObject*  m_pObject;       // DtBc or DtDf object registering for the event
    DtOnPeriodicIntervalFunc  m_OnPeriodicFunc;  // Event callback
}  DtOnPeriodicIntervalRegData;

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IoConfig Set Notification +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Signature of the ioconfig-set-event handler callback function
typedef DtStatus (*DtOnIoConfigSetFunc)(DtObject*, const DtIoConfig*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoConfigSetHandlerRegData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Registration data for ioconfig-set-event handlers
typedef struct  _DtIoConfigSetHandlerRegData
{
    DtObject*  m_pObject;       // DtBc or DtDf object registering for the event
    DtOnIoConfigSetFunc  m_OnIoConfigSetFunc;  // Event callback
}  DtIoConfigSetHandlerRegData;

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Events +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDriverEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtDriverEvent
{
    Int  m_EventType;
    Int  m_EventValue1;
    Int  m_EventValue2;
}  DtDriverEvent;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDeviceInfo definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDeviceInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// MARCO defining the members of the common device info struct
#define  DT_DEVICE_INFO_COMMON_DATA                                                      \
    Int  m_Size;                /* Set to size of this struct OR of a 'derived' struct */\
    Int  m_TypeNumber;          /* DTX-xxx typenumber (e.g. 100 for DTA-100) */          \
    Int  m_SubType;             /* Device subtype (0=none, 1=A, ...) */                  \
    Int  m_HardwareRevision;    /* Hardware revision (e.g. 302 = 3.2) */                 \
    Int  m_FirmwareVersion;     /* Firmware Version (= Altera revision), e.g. 3 for */   \
                                /* "Firmware Version 3"  */                              \
    Int  m_FirmwareVariant;     /* Firmware Variant, e.g. to distinguish between */      \
                                /* firmware with different #inputs/#outputs */           \
    Int  m_FirmwareStatus;      /* Status of the firmware/driver combination */          \
    DtDrvFwBuildDateTime  m_FwBuildDate; /* Firmware build date */                       \
    Int  m_FwPackageVersion;    /* Firmware Package version, version number given */     \
                                /* to the set of firmware variants */                    \
    UInt64  m_Serial;           /* Serial number */                                      \
    UInt64  m_UniqueId          /* Unique board ID (can be same as serial) */
// The struct (including the common members)
// NOTE: 'derived' structs should include DT_DEVICE_INFO_COMMON_DATA members first, so 
// that the can be casted to a DtDeviceInfo struct
typedef struct  _DtDeviceInfo
{
    DT_DEVICE_INFO_COMMON_DATA;
} DtDeviceInfo;

//+=+=+=+=+=+=+=+=+=+=+=+ Core-Module - DtCore_EVENTS definitions +=+=+=+=+=+=+=+=+=+=+=+=


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtCore object
#define CORE_DEFAULT_PRECONDITIONS(pCore)      \
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore))

// Open core io-stub function
typedef DtIoStubCore*  (*DtIoStubCoreOpenFunc)(DtCore*);

// DtCore call-back functions
typedef UInt32  (*DtCoreRegRead32Func)(DtCore*, UInt32  Address);
typedef void  (*DtCoreRegWrite32Func)(DtCore*, UInt32 Address, UInt32 Val);
typedef DtStatus  (*DtCoreBulkRead32Func)(DtCore*, UInt32 Address, Int NumToRead, 
                                                                         UInt32* pBuffer);
typedef DtStatus  (*DtCoreOpenChildrenFunc)(DtCore*);
typedef void  (*DtCoreCloseChildrenFunc)(DtCore*);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Common data for core driver services or a function based device
#define  DT_CORE_COMMON_DATA_PUBLIC                                                      \
    Int  m_Size;                /* Set to size of this struct OR of a 'derived' struct */\
    const char*  m_TypeName;    /* */                                                    \
    DtIoStubCore*  m_pIoStub;   /* */                                                    \
    /* Common interface callback functions */                                            \
    DtCoreRegRead32Func  m_RegRead32Func;                                                \
    DtCoreRegWrite32Func  m_RegWrite32Func;                                              \
    DtCoreBulkRead32Func  m_BulkRead32Func;                                              \
    DtCoreOpenChildrenFunc  m_OpenChildrenFunc;                                          \
    DtCoreCloseChildrenFunc  m_CloseChildrenFunc
#define DT_CORE_COMMON_DATA_PRIVATE                                                      \
    /* Driver / Device objects */                                                        \
    DtDrvObject  m_Driver;                                                               \
    DtDvcObject  m_Device;                                                               \
    /* IAL data */                                                                       \
    DtIalData  m_IalData;                                                                \
    /* Pointer to the common device info. */                                             \
    /* Must point to a member of the 'derived' DtCore struct that implements */          \
    /* DT_DEVICE_INFO_COMMON_DATA  */                                                    \
    DtDeviceInfo*  m_pDevInfo;                                                           \
    Int  m_NumPorts;                                                                     \
    /* File handles  */                                                                  \
    DtFastMutex  m_FileHandleInfoMutex;                                                  \
    DtFileHandleInfo*  m_pFileHandleInfo;                                                \
    DtCfEvtData*  m_pCfEvtData;     /* Events data*/                                     \
    /* Shortcuts to core driver-functions and block-controllers */                       \
    DtBcVVI*  m_pBcVvi;             /* Shortcut to the one and only VVI BC */            \
    DtCfEvt*  m_pCfEvt;             /* Shortcut to the one and only EVT-CF */            \
    DtCfProps*  m_pCfProps;         /* Shortcut to the one and only PROPS-CF */          \
    DtCfInt*  m_pCfInt;             /* Shortcut to the one and only INT-CF */            \
    DtCfIoCfg*  m_pCfIoCfg;         /* Shortcut to the one and only IOCFG-CF */          \
    DtCfTod*  m_pCfTod;             /* Shortcut to the one and only TOD-CF */            \
    DtDfVpd*  m_pDfVpd;             /* Shortcut to the one and only VPD function */      \
    /* Device level driver-function and block-controllers */                             \
    DtVectorBc*  m_pBcList;         /* List with block-controllers in load order */      \
    DtVectorBc*  m_pUuidToBc;       /* Lookup table from UUID to BC */                   \
    DtVectorDf*  m_pDfList;         /* List with driver-functions in load order */       \
    DtVectorDf*  m_pUuidToDf;       /* Lookup table from UUID to DF */                   \
    DtVectorPt*  m_pPtList          /* List with ports in load order */
#define  DT_CORE_COMMON_DATA                                                             \
    /* Common public data. May be used by others */                                      \
    DT_CORE_COMMON_DATA_PUBLIC;                                                          \
    /* Common private data. Donot touch these members outside of the DtCore object */    \
    DT_CORE_COMMON_DATA_PRIVATE                                                         
    
// The struct (including the common members)
struct _DtCore
{
    DT_CORE_COMMON_DATA;
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtCore_Open(DtCore*, DtFileObject*);
DtStatus  DtCore_Close(DtCore*, DtFileObject*);
DtStatus  DtCore_CloseChildren(DtCore*);
DtStatus  DtCore_OpenChildren(DtCore*);
DtStatus  DtCore_Ioctl(DtCore*, DtFileObject*, DtIoctlObject*);
DtStatus  DtCore_InitSerialAndHwRev(DtCore*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEBUG - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
// NONE AT THE MOMENT
//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtCore_DEVICE_Init(DtCore*, DtIoStubCoreOpenFunc);
DtStatus  DtCore_DEVICE_PowerUp(DtCore*);
DtStatus  DtCore_DEVICE_InterruptEnable(DtCore*);
DtStatus  DtCore_DEVICE_PowerUpPost(DtCore* pCore);
DtStatus  DtCore_DEVICE_PowerDownPre(DtCore* pCore);
DtStatus  DtCore_DEVICE_PowerUpPost(DtCore* pCore);
DtStatus  DtCore_DEVICE_PowerDownPre(DtCore* pCore);
DtStatus  DtCore_DEVICE_InterruptDisable(DtCore* pCore);
DtStatus  DtCore_DEVICE_PowerDown(DtCore* pCore);
DtStatus  DtCore_DEVICE_ExitPre(DtCore* pCore);
void      DtCore_DEVICE_Exit(DtCore*);
Int       DtCore_DEVICE_GetNumPorts(DtCore*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_BC - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtCore_BC_Close(DtBc*);
DtBc*     DtCore_BC_Open(DtCore*, DtBcType, Int  Address, DtPt*  pPt, const DtBcId*,
                                                                        Bool  CreateStub);
DtStatus  DtCore_BC_OpenAll(DtCore*, DtPt*  pPt);
DtStatus  DtCore_BC_OpenVvi(DtCore*);
DtBc*     DtCore_BC_Find(DtCore*, DtPt*  pPt, DtBcType, const char*  pRole);
DtBc*     DtCore_BC_FindByAddress(DtCore*, DtPt*  pPt, Int  Address);
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DF - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtCore_DF_Close(DtDf*);
DtDf*     DtCore_DF_Open(DtCore*, DtFunctionType, DtPt*  pPt, const DtDfId*,
                                                                        Bool  CreateStub);
DtStatus  DtCore_DF_OpenAll(DtCore*, DtPt*  pPt);
DtDf*     DtCore_DF_Find(DtCore*, DtPt*  pPt, DtFunctionType, const char*  pRole);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtCore_PT - Public functions .-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtCore_PT_Close(DtPt*);
DtPt*     DtCore_PT_Open(DtCore*, DtPortType, Int PortIndex);
DtStatus  DtCore_PT_OpenAll(DtCore*, Int NumPorts);
DtPt*     DtCore_PT_Find(DtCore*, Int PortIndex);
DtStatus  DtCore_PT_GetPortIoCaps(DtCore*, Int PortIndex, DtIoCaps*  pIoCaps);
DtStatus  DtCore_PT_GetIoCaps(DtPt*, DtIoCaps*  pIoCaps);
Int  DtCore_PT_GetPortIndex(DtPt*);
//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_EVENTS - public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtCore_EVENTS_CleanupEvtData(DtCfEvtData*);
DtCfEvtData*  DtCore_EVENTS_CreateEvtData(void);
DtStatus  DtCore_EVENTS_Set(DtCore*, DtDriverEvent, Bool AddIfExists);
#ifdef LINBUILD
UInt  DtCore_EVENTS_Poll(DtCore*, DtFileObject*, poll_table*);
#endif
//.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_IOCONFIG - public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtCore_IOCONFIG_Restore(DtCore*  pCore);

//-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_INTERRUPT - Public Functions -.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtCore_INTERRUPT_Register(DtCore*, const DtIntHandlerRegData*);
DtStatus  DtCore_INTERRUPT_Unregister(DtCore*, Int  Index, const DtBc*);
//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PROPS - public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtCore_PROPS_Find(DtCore*, const char*, DtPropertyFilterCriteria,
                                                                      const DtProperty**);
DtStatus  DtCore_PROPS_Get(DtCore*, const char*, DtPropertyFilterCriteria,
                                              DtPropertyValue*, DtPropertyValueType*,
                                              DtPropertyScope*,
                                              Bool  IsOptional, DtPropertyValue  Default);
DtStatus  DtCore_PROPS_GetStr(DtCore*, const char*, DtPropertyFilterCriteria,
                                                     char* pStr, DtPropertyScope* pScope);
Bool      DtCore_PROPS_GetBool(DtCore*, const char*, Int, Bool  Default);
Int       DtCore_PROPS_GetInt(DtCore*, const char*, Int, Int  Default);
Int8      DtCore_PROPS_GetInt8(DtCore*, const char*, Int, Int8  Default);
Int16     DtCore_PROPS_GetInt16(DtCore*, const char*, Int, Int16  Default);
Int32     DtCore_PROPS_GetInt32(DtCore*, const char*, Int, Int32  Default);
Int64     DtCore_PROPS_GetInt64(DtCore*, const char*, Int, Int64  Default);
UInt      DtCore_PROPS_GetUInt(DtCore*, const char*, Int, UInt  Default);
UInt8     DtCore_PROPS_GetUInt8(DtCore*, const char*, Int, UInt8  Default);
UInt16    DtCore_PROPS_GetUInt16(DtCore*, const char*, Int, UInt16  Default);
UInt32    DtCore_PROPS_GetUInt32(DtCore*, const char*, Int, UInt32  Default);
UInt64    DtCore_PROPS_GetUInt64(DtCore*, const char*, Int, UInt64  Default);
DtStatus  DtCore_PROPS_ReInit(DtCore*);
//.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_SETTINGS - public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtCore_SETTINGS_Delete(DtCore*, Int  NumPorts);
DtStatus  DtCore_SETTINGS_ManufRead(DtCore*, const char*, Int64*  pBinValue);
DtStatus  DtCore_SETTINGS_StringRead(DtCore*, Int, const char*, const char* , 
                                                                  char* pValue, Int Size);
DtStatus  DtCore_SETTINGS_StringWrite(DtCore*, Int, const char*, const char*, 
                                                                     const char*  pValue);
DtStatus  DtCore_SETTINGS_ValueRead(DtCore*, Int, const char*, const char*, Int64*);
DtStatus  DtCore_SETTINGS_ValueWrite(DtCore*, Int, const char*, const char*, Int64);
// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_TOD - Public Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtCore_TOD_GetPeriodicItv(const DtCore*, Int* pIntervalMs);
DtStatus  DtCore_TOD_GetTime(const DtCore*, DtTodTime*);
DtStatus  DtCore_TOD_PeriodicItvRegister(DtCore*, const DtOnPeriodicIntervalRegData*);
DtStatus  DtCore_TOD_PeriodicItvUnregister(DtCore*, const DtObject*);
DtTodTime DtCore_TOD_Add(DtTodTime Time, Int64 Offset);
Int64  DtCore_TOD_TimeDiff(DtTodTime Time1, DtTodTime Time2);
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_VPD - public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtCore_VPD_ReadItemRo(const DtCore*, const char* pKeyword,
                                            UInt8* pItemBuf, Int BufSize, Int*  pNumRead);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCore definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the core stub
#define STUB_CORE_DEFAULT_PRECONDITIONS(pStub)      \
    DT_ASSERT(pStub!=NULL && pStub->m_Size>=sizeof(DtIoStubCore))

// MACRO to cast a stub object to a DtIoStubCore object
#define STUB_CORE   ((DtIoStubCore*)pStub)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#define DT_IOSTUB_CORE_COMMON_DATA                                                       \
    DT_IOSTUB_COMMON_DATA              /* Derive from DtIoStub */
// The struct (including the common members)
struct _DtIoStubCore
{
    DT_IOSTUB_CORE_COMMON_DATA;
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtIoStubCore_Init(DtIoStubCore*);
//DtStatus  DtIoStubCore_OnExclAccessCmd(const DtIoStub*, Int PortIndex, Int Cmd, 
//                                                                           DtFileObject*);
DtStatus  DtIoStubCore_OnExclAccessCmd(const DtIoStub*, DtIoStubIoParams*,
                                                                          Int*  pOutSize);
DtIoStub*  DtIoStubCore_GetChildStub(const DtIoStub*, const DtIoStubIoParams*);
//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore_DEBUG - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtIoStubCore_DEBUG_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
DtStatus  DtIoStubCore_DEBUG_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*  pOutSize);
//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore_IOCONFIG - Public functions -.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtIoStubCore_IOCONFIG_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
DtStatus  DtIoStubCore_IOCONFIG_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*  pOutSize);

#endif // #ifndef  __DT_CORE_H



