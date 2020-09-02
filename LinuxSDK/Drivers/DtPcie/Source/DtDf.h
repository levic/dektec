//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDf.h *#*#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_DF_H
#define __DT_DF_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtSal.h"
#include "DtBc.h"
#include "DtBcDfCommon.h"
#include "DtVectorBc.h"
#include "DtVectorDf.h"
#include "DtIoStub.h"

// Forwards
typedef struct _DtIoStubDf  DtIoStubDf;
typedef DtIoConfigValueDriver  DtCfIoConfigValue;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Defines / Constants +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Tag for memory allocated by a driver-function
#define DF_TAG  0x20204644     // '  FD'

// Maximum size of DF instance ID string
#define DT_DF_INSTANCE_ID_MAX_SIZE  64

// Prefix DF debug logging with the DF instance ID + UUID index
#define DtDbgOutDf(Level, Module, pDf, Msg, ...)                                         \
do                                                                                       \
{                                                                                        \
    if (!pDf->m_IsCf)                                                                    \
    {                                                                                    \
        DtDbgOut_Prefix(Level, DF_, Module, "[%s:%d] " Msg,                              \
                                pDf->m_InstanceId,                                       \
                                (pDf->m_Id.m_Uuid & DT_UUID_INDEX_MASK),                 \
                                ##__VA_ARGS__);                                          \
    }                                                                                    \
    else                                                                                 \
    {                                                                                    \
        DtDbgOut_Prefix(Level, DF_, Module, "[%s_%s:%d] " Msg,                           \
                                pDf->m_InstanceId,                                       \
                                pDf->m_Id.m_pShortName,                                  \
                                (pDf->m_Id.m_Uuid & DT_UUID_INDEX_MASK),                 \
                                ##__VA_ARGS__);                                          \
    }                                                                                    \
} while(0)
// Specialized version for core-functions
#define DtDbgOutCf(Level, Module, pCf, Msg, ...)                                         \
do                                                                                       \
{                                                                                        \
    DT_ASSERT(pCf->m_IsCf);                                                              \
    DtDbgOut_Prefix(Level, CF_, Module, "[%s_%s:%d] " Msg,                               \
                                pCf->m_InstanceId,                                       \
                                pCf->m_Id.m_pShortName,                                  \
                                (pCf->m_Id.m_Uuid & DT_UUID_INDEX_MASK),                 \
                                ##__VA_ARGS__);                                          \
} while(0)

// MACRO that checks the DF has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_MUST_BE_ENABLED_IMPL(Module, pDf)                                             \
    do                                                                                   \
    {                                                                                    \
        if (!DtDf_IsEnabled((DtDf*)pDf))                                                 \
        {                                                                                \
            DtDbgOutDf(ERR, Module, pDf, "ERROR: DF is not enabled");                    \
            return DT_STATUS_NOT_ENABLED;                                                \
        }                                                                                \
    } while(0)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCommonProps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// List of common DF properties that can be loaded from the property store
typedef struct  _DtDfCommonProps
{
    Int  m_Uuid;            // UUID (PropName="UUID")
    DtFunctionType  m_Type; // Function-type (PropName="TYPE")
    const char* m_pRole;    // Function role (PropName="")
    Bool  m_CreateStub;     // TRUE, if an IO-stub must be created (i.e. DTAPI wants
                            // IOCTL access to this DF)
}  DtDfCommonProps;

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Public DtDfCommonProps functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtDfCommonProps_Load(DtDfCommonProps*, const DtDfId*, Int  PortIndex, DtCore*);
DtStatus  DtDfCommonProps_LoadFromString(DtDfCommonProps*, const char*  InstanceId, 
                                                                 Int  PortIndex, DtCore*);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Driver-function parameter
typedef struct _DtDfParameters
{
    const char*  m_Name;            // Name of the parameter
    const DtPropertyValueType  m_ValueType;  // Property value type for the parameter
    void*  m_pValue;                // Pointer to buffer (of m_ValueType) that should
                                    // receive the parameter value
}  DtDfParameters;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDf definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef enum  _DtDfState
{
    DF_STATE_UNDEFINED=0,       // DF has not been initilised yet and is unusable
    DF_STATE_INITIALISED,       // DF's resouces have been allocated and register access
                                // is possible. However all it's threads should be 
                                // suspended/IDLE and it may not make use of interrupts
    DF_STATE_ENABLED,           // DF is fully enabled and may use of interrupts and 
                                // threads

}  DtDfState;

// Driver-function callback functions
typedef DtDf*  (*DtDfOpenFunc)(DtFileObject*);
typedef void  (*DtDfCloseFunc)(DtDf*);
typedef DtStatus  (*DtDfInitFunc)(DtDf*);
typedef DtStatus  (*DtDfEnableFunc)(DtDf*, Bool  Enable);
typedef DtStatus  (*DtDfOnEnablePreChildrenFunc)(DtDf*, Bool  Enable);
typedef DtStatus  (*DtDfOnEnablePostChildrenFunc)(DtDf*, Bool  Enable);
typedef DtStatus (*DtDfOnCloseFileFunc)(DtDf*, const DtFileObject*);
typedef DtStatus  (*DtDfLoadParametersFunc)(DtDf*);


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Driver function ID
//typedef  DtBcOrDfId  DtDfId;
// MACRO, to initialise driver-function-ID 
#define DT_DF_INIT_ID(ID, NAME, SHORT, ROLE, INSTANCE, UUID)                             \
do                                                                                       \
{                                                                                        \
    DtMemZero(&ID, sizeof(ID));                                                          \
    ID.m_pName = NAME;                                                                   \
    ID.m_pShortName = SHORT;                                                             \
    ID.m_pRole = ROLE;                                                                   \
    ID.m_Instance = INSTANCE;                                                            \
    ID.m_Uuid = UUID;                                                                    \
}                                                                                        \
while (0)

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChildProps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtDfChildProps
{
    Int  m_ObjectType;              // Type of child properties (i.e. BC or DF)
    char m_ShortName[DT_BCORDF_NAME_MAX_SIZE+1];  // Shortname for BC/DF
    Int  m_Instance;                // BC/DF instance number (i.e. Nth occurance of BC/DF)
    union
    {
        DtBcCommonProps  m_Bc;
        DtDfCommonProps  m_Df;
    }  m_Props;
}  DtDfChildProps;

typedef struct  _DtDfChildId
{
    Int  m_ObjectType;              // Type of object (i.e. BC or DF)
    Int  m_Type;                    // BcType or FuncType (depends on object type)
    const char*  m_pRole;           // Role of the block/function
}  DtDfChildId;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSupportedChild -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfSupportedChild
{
    Int  m_ObjectType;              // Type of object (i.e. BC or DF)
    Int  m_Type;                    // BcType or FuncType (depends on object type)
    const char*  m_pName;           // Name of the block/function
    const char*  m_pRole;           // Role of the block/function
    DtObjectBcOrDf**  m_ppChild;    // Pointer to receive a pointer to the child object
    Bool  m_IsMandatory;            // Inidcates if the child is manadatory
}  DtDfSupportedChild;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfOpenParams -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Struct holding with the parameters for the Open function
typedef struct  _DtDfOpenParams
{
    Int  m_Size;                    // Size of the block struct
    DtDfId  m_Id;                   // Driver-function ID info
    DtFunctionType  m_Type;         // Type of function
    DtPt*  m_pPt;                   // Port the function belongs to. NULL, means device
    DtCore*  m_pCore;               // Core driver services
    Bool  m_IsCf;                   // TRUE, if the function is core function 
                                    // (i.e. sub-module of the core)
    DtDfCloseFunc  m_CloseFunc;     // Close function
    DtDfInitFunc  m_InitFunc;       // Initialisation function
    DtDfEnableFunc  m_EnableFunc;   // Enable function
    DtDfOnEnablePreChildrenFunc  m_OnEnablePreChildrenFunc; 
                                    // OnEnable Pre childs function
    DtDfOnEnablePostChildrenFunc  m_OnEnablePostChildrenFunc; 
                                    // OnEnable Post childs function
    DtDfOnCloseFileFunc  m_OnCloseFileFunc;  // On close file function
    DtDfLoadParametersFunc  m_LoadParsFunc;  // Load DF-parameters function
    Bool  m_CreateStub;             // TRUE, if an IO-stub must be created (i.e. DTAPI 
                                    // wants IOCTL access to this DF)
}  DtDfOpenParams;
// MACRO to initialise open parameters safe initial values and set base parameters
#define DT_DF_INIT_OPEN_PARAMS(OP, STRUCT, ID, TYPE, PORT, STUB, CORE)                   \
do                                                                                       \
{                                                                                        \
    DtMemZero(&OP, sizeof(OP));                                                          \
    OP.m_Size = sizeof(STRUCT);                                                          \
    OP.m_Id = ID;                                                                        \
    OP.m_Type = TYPE;                                                                    \
    OP.m_pPt = PORT;                                                                     \
    OP.m_CreateStub = STUB;                                                              \
    OP.m_pCore = CORE;                                                                   \
    OP.m_IsCf = FALSE;                                                                   \
}                                                                                        \
while (0)
// 'OVERLOADED VERSION' for initialising the open-params for a CF
#define DT_CF_INIT_OPEN_PARAMS(OP, STRUCT, ID, TYPE, STUB, CORE)                         \
do                                                                                       \
{                                                                                        \
    /* Init as normal DF, but port is by definition NULL */                              \
    DT_DF_INIT_OPEN_PARAMS(OP, STRUCT, ID, TYPE, NULL, STUB, CORE);                      \
    /* Mark as CF */                                                                     \
    OP.m_IsCf = TRUE;                                                                    \
}                                                                                        \
while (0)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#define DT_DF_COMMON_DATA                                                                \
    DT_OBJECT_BC_OR_DF_COMMON_DATA;  /* Derive from DtObjectBcOrDf */                    \
    char  m_InstanceId[DT_DF_INSTANCE_ID_MAX_SIZE];                                      \
                                /* Instance ID string */                                 \
    DtFunctionType  m_Type; /* Type of function */                                       \
    DtDfState  m_OpState;   /* Current operational state of the DF */                    \
    DtCore*  m_pCore;       /* Shortcut to core driver services */                       \
    DtIoStubDf*  m_pIoStub; /* The IOCTL stub for this block controller */               \
    DtPt*  m_pPt;           /* Port the function belongs to. NULL, means device */       \
    Bool  m_IsCf;           /* TRUE, if this function is a core function */              \
    /* Children: block-controller and sub-functions */                                   \
    DtVectorBc*  m_pBcList;                                                              \
    DtVectorDf*  m_pDfList;                                                              \
    /* Exclusive access */                                                               \
    Bool  m_ExclAccess;         /* True=inuse, False=unused */                           \
    DtExclAccessObject  m_ExclAccessOwner;  /* Current owner */                          \
    DtMutex  m_ExclAccessLock;   /* Mutex protecting the access state */                 \
    /* Callback functions */                                                             \
    DtDfInitFunc  m_InitFunc;                                                            \
    DtDfEnableFunc  m_EnableFunc;                                                        \
    DtDfOnEnablePreChildrenFunc  m_OnEnablePreChildrenFunc;                              \
    DtDfOnEnablePostChildrenFunc  m_OnEnablePostChildrenFunc;                            \
    DtDfCloseFunc  m_CloseFunc;                                                          \
    DtDfOnCloseFileFunc  m_OnCloseFileFunc;                                              \
    DtDfLoadParametersFunc  m_LoadParsFunc
struct _DtDf
{
    DT_DF_COMMON_DATA;
};
typedef DtDf  DtDeviceFunction;
typedef DtDf  DtPortFunction;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public DtDf functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtDf_AddBc(DtDf*, DtBc*);
DtStatus  DtDf_AddDf(DtDf*, DtDf*);
void  DtDf_Close(DtDf*);
DtStatus  DtDf_ExclAccessAcquire(DtDf*, const DtExclAccessObject*);
DtStatus  DtDf_ExclAccessCheck(DtDf*, const DtExclAccessObject*);
DtStatus  DtDf_ExclAccessProbe(DtDf*);
DtStatus  DtDf_ExclAccessRelease(DtDf*, const DtExclAccessObject*);
DtStatus  DtDf_Enable(DtDf*, Bool);
DtStatus  DtDf_OnCloseFile(DtDf*, const DtFileObject*);
DtStatus  DtDf_LoadParameters(const DtDf*, Int, DtDfParameters*);
DtDf*  DtDf_Open(const DtDfOpenParams*);
DtStatus  DtDf_OpenChildren(DtDf*  pDf, const DtDfSupportedChild*, Int  NumSupported);
DtStatus  DtDf_OpenChildrenOfBcType(DtDf*, DtBcType, DtVectorBc* );
DtStatus  DtDf_OpenChildrenOfDfType(DtDf*, DtFunctionType, DtVectorDf*);
DtStatus  DtDf_CloseChildren(DtDf*);
DtStatus  DtDf_EnableChildren(DtDf*, Bool  Enable);
DtDf*  DtDf_OpenType(DtFunctionType, DtCore*, DtPt*  pPt, const DtDfId*, 
                                                                        Bool  CreateStub);
DtStatus  DtDf_ToInstanceId(const DtDf*, char*  pInstanceId, Int  MaxLength);
DtStatus  DtDf_ToInstanceIdFromStrings(const char*  pShortName, Int  Instance,
                                                      char*  pInstanceId, Int  MaxLength);
DtStatus  DtDf_ToChildInstanceId(const DtDf*, Int  ChildInstance,
                                                      char*  pInstanceId, Int  MaxLength);
DtStatus  DtDf_ToChildInstanceIdFromStrings(const char*  pFuncInstanceId, 
                                                      Int  ChildInstance, 
                                                      char*  pInstanceId, Int  MaxLength);
DtStatus  DtDf_ToPropertyName(const DtDf*, const char*  pProp, 
                                                    char*  pFullPropName, Int  MaxLength);
DtStatus  DtDf_ToPropertyNameFromStrings(const char*  pInstanceId,
                                                    const char*  pProp, 
                                                    char*  pFullPropName, Int  MaxLength);

DtStatus  DtDf_SetOpState(DtDf*, DtDfState  NewState);
const char*  DtDf_OpStateToString(DtDfState);
Bool  DtDf_IsEnabled(const DtDf*);
Bool  DtDf_IsInitialised(const DtDf*);

DtStatus  DtDf_LoadChildProps(const DtDf*, Int  ChildInstance, DtDfChildProps*);
DtStatus  DtDf_LoadChildPropsFromString(const char*  pChildInstanceId, 
                                                DtCore*, Int  PortIndex, DtDfChildProps*);
DtStatus  DtDf_LoadChildPropsAll(const DtDf*, DtVector*);
DtStatus  DtDf_LoadChildPropsAllFromString(const char*  pFuncInstanceId, DtCore*, 
                                                               Int  PortIndex, DtVector*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDf definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// 
#define DtDbgOutIoStubDf(Level, Module, pStub, Msg, ...)                                 \
                DtDbgOutIoStub_Prefix(Level, DF_, Module, pStub, Msg, ##__VA_ARGS__)
// Specialized version for core-functions
#define DtDbgOutIoStubCf(Level, Module, pStub, Msg, ...)                                 \
                DtDbgOutIoStub_Prefix(Level, CF_, Module, pStub, Msg, ##__VA_ARGS__)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfOpenParams -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoStubDfOpenParams
{
    DT_IOSTUB_INIT_OPEN_PARAMS_COMMON_DATA;
    DtDf*  m_pDf;                   // The DF controlled by this stub
}  DtIoStubDfOpenParams;
// MACRO to initialise open parameters
#define DT_IOSTUBDF_INIT_OPEN_PARAMS(OP, STRUCT, DF, NAME, CLOSE_F, IOCTL_F, IOCTLS)     \
do                                                                                       \
{                                                                                        \
    DT_IOSTUB_INIT_OPEN_PARAMS(OP, STRUCT, DF->m_pCore, NAME, CLOSE_F, IOCTL_F, IOCTLS); \
    OP.m_ObjectType = DT_OBJECT_TYPE_IOSTUB_DF;                                          \
    OP.m_pDf = DF;                                                                       \
}                                                                                        \
while (0)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Common data for all DF io-control stubs
#define DT_IOSTUB_DF_COMMON_DATA                                                         \
    DT_IOSTUB_COMMON_DATA;                                                               \
    DtDf*  m_pDf                    /* Points to the DF by the stub */                   \
// The struct
struct _DtIoStubDf
{
    DT_IOSTUB_DF_COMMON_DATA;
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public DtIoStubDf functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubDf_Close(DtIoStub*);
DtIoStubDf*  DtIoStubDf_Open(DtIoStubDfOpenParams*);
#ifdef DT_DF_ADD_FUNCTION_INCLUDES
DtIoStubDf*  DtIoStubDf_OpenType(DtDf*);
#endif  // #ifdef DT_DF_ADD_FUNCTION_INCLUDES
DtStatus  DtIoStubDf_OnExclAccessCmd(const DtIoStub*, DtIoStubIoParams*, Int*  pOutSize);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Function includes +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#ifdef DT_DF_ADD_FUNCTION_INCLUDES
#include "DtDfAsiRx.h"          // ASI transport stream receiver driver function
#include "DtDfGenLockCtrl.h"    // SDI genlocking controller function
#include "DtDfMxDs75Temp.h"     // MAXIM DS75 digital thermometer controller function
#include "DtDfSdiTxPhy.h"       // SDI-PHY transmitter driver function
#include "DtDfSdiRx.h"          // SDI-receiver function
#include "DtDfSdiXCfgMgr.h"     // SDI-tranceiver reconfiguration manager function
#include "DtDfSi534X.h"         // SI-5342/5344/5345 controller function
#include "DtDfSpiCableDrvEq.h"  // SPI-cable driver/equalizer controller function
#include "DtDfSpiProm.h"        // SPI-prom driver function
#include "DtDfSensTemp.h"       // Sensor temperature-function
#include "DtDfTempFanMgr.h"     // Temperature and fanspeed manager-function
#include "DtDfTxPllMgr.h"       // TX-PLL manager function
#include "DtDfVirtGenRef.h"     // SDI virtual genref function
#include "DtDfVpd.h"            // Device-level VPD function
#include "DtCfEvt.h"            // Events core-function
#include "DtCfIoCfg.h"          // IoConfig core-function
#include "DtCfInt.h"            // Interrupt core-function
#include "DtCfProps.h"          // Property core-function
#include "DtCfTod.h"            // TimeOfDay core-function

// Local functions
#include "DtDfS2CrDemod_2132.h" // Creonix DVB-S2 demodulator and decoder function

// List with of well-known driver-functions IDs
extern const DtDfId  DT_DF_KNOWN[];
extern const Int  DT_DF_NUM_KNOWN;

#endif // #ifdef DT_DF_ADD_FUNCTION_INCLUDES

#endif  // #ifndef __DT_DF_H

