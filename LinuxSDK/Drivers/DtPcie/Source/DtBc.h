// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBc.h *#*#*#*#*#*#*#*#*#* (C) 2017-2018 DekTec
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

#ifndef __DT_BC_H
#define __DT_BC_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtSal.h"
#include "DtDrvCommon.h"
#include "DtCore.h"
#include "DtIoStub.h"
#include "Interrupts.h"
#include "DtBcDfCommon.h"

// Forwards
typedef struct _DtIoStubBc  DtIoStubBc;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Defines / Constants +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Tag for memory allocated by a block-controller
#define BC_TAG  0x20204342        // '  CB'

// Maximum size of the block-id
#define DT_BC_ID_MAX_SIZE  31

// Prefix BC debug logging with the BC instance ID + UUID index
#define DtDbgOutBc(Level, Module, pBc, Msg, ...)                                         \
                DtDbgOut_Prefix(Level, BC_, Module, "[%s:%d] " Msg,                      \
                                        pBc->m_InstanceId,                               \
                                        (pBc->m_Id.m_Uuid & DT_UUID_INDEX_MASK),         \
                                        ##__VA_ARGS__)

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_MUST_BE_ENABLED_IMPL(Module, pBc)                                             \
    do                                                                                   \
    {                                                                                    \
        if (!DtBc_IsEnabled((DtBc*)pBc))                                                 \
        {                                                                                \
            DtDbgOutBc(ERR, Module, pBc, "ERROR: BC is not enabled");                    \
            return DT_STATUS_NOT_ENABLED;                                                \
        }                                                                                \
    } while(0)



//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcInterruptProps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct  _DtBcInterruptProps
{
    Int  m_Id;              // Interrupt ID
    Int  m_Index;           // Interrupt index
    UInt32  m_AddrOffset;   // Byte offset, in block's address space, to status register
}  DtBcInterruptProps;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCommonProps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// List of common BC properties that can be loaded from the property store
typedef struct  _DtBcCommonProps
{
    Int  m_Address;         // Address property (PropName="ADDRESS")
    Int  m_Uuid;            // UUID (PropName="UUID")
    DtBcType  m_Type;       // Block-type (PropName="TYPE")
    const char* m_pRole;    // Block role (PropName="")
    Bool  m_CreateStub;     // TRUE, if an IO-stub must be created (i.e. DTAPI wants
                            // IOCTL access to this BC)
}  DtBcCommonProps;

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Public DtBcCommonProps functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtBcCommonProps_Load(DtBcCommonProps*, const DtBcId*, Int  PortIndex, DtCore*);
DtStatus  DtBcCommonProps_LoadFromString(DtBcCommonProps*, const char*  InstanceId, 
                                                                 Int  PortIndex, DtCore*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBc definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef enum  _DtBcState
{
    BC_STATE_UNDEFINED=0,       // BC has not been initilised yet and is unusable
    BC_STATE_INITIALISED,       // BC's resouces have been allocated and register access
                                // is possible. However all it's threads should be 
                                // suspended/IDLE and it may not make use of interrupts
    BC_STATE_ENABLED,           // BC is fully enabled and may use of interrupts and 
                                // threads
}  DtBcState;


// BC callback functions
typedef DtStatus  (*DtBcInitFunc)(DtBc*);
typedef void  (*DtBcCloseFunc)(DtBc*);
typedef DtStatus  (*DtBcOnCloseFileFunc)(DtBc*, const DtFileObject*);
typedef DtStatus  (*DtBcEnableFunc)(DtBc*, Bool  Enable);
typedef DtStatus  (*DtBcOnEnable)(DtBc*, Bool  Enable);


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Block-controller ID
// MACRO to initialise block-controller-ID 
#define DT_BC_INIT_ID(ID, NAME, SHORT, ROLE, INSTANCE, UUID)                             \
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcOpenParams -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Struct holding with the parameters for the Open function
typedef struct  _DtBcOpenParams
{
    Int  m_Size;                    // Size of the block struct
    DtBcId  m_Id;                   // Block-controller ID info
    DtBcType  m_Type;               // Type of block-controller
    Int  m_Address;                 // Base address of the block
    DtPt*   m_pPt;                  // Port the block belongs to. NULL, means device
    DtCore*  m_pCore;               // Core driver services
    DtBcInitFunc  m_InitFunc;       // Initialisation function
    DtBcCloseFunc  m_CloseFunc;     // Close function
    DtBcEnableFunc  m_EnableFunc;   // Enable function
    DtBcOnEnable  m_OnEnableFunc;   // On enable function
    DtBcOnCloseFileFunc  m_OnCloseFileFunc;  // On close file function
    Bool  m_CreateStub;             // TRUE, if an IO-stub must be created (i.e. DTAPI 
                                    // wants IOCTL access to this BC)
}  DtBcOpenParams;
// MACRO to initialise open parameters
#define DT_BC_INIT_OPEN_PARAMS(OP, STRUCT, ID, TYPE, ADDR, PORT, STUB, CORE)             \
do                                                                                       \
{                                                                                        \
    DtMemZero(&OP, sizeof(OP));                                                          \
    OP.m_Size = sizeof(STRUCT);                                                          \
    OP.m_Id = ID;                                                                        \
    OP.m_Type = TYPE;                                                                    \
    OP.m_Address = ADDR;                                                                 \
    OP.m_pPt = PORT;                                                                     \
    OP.m_CreateStub = STUB;                                                              \
    OP.m_pCore = CORE;                                                                   \
}                                                                                        \
while (0)


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Common data for all block controllers
#define DT_BC_COMMON_DATA_PRIV                                                           \
    DT_OBJECT_BC_OR_DF_COMMON_DATA;  /* Derive from DtObjectBcOrDf */                    \
    char  m_InstanceId[DT_BCORDF_INSTANCE_ID_MAX_SIZE];                                  \
                                /* Instance ID string */                                 \
    DtBcState  m_OpState;       /* Current operational state of the block */             \
    DtCore*  m_pCore;           /* Shortcut to core driver services */                   \
    DtIoStubBc*  m_pIoStub;     /* The IOCTL stub for this block controller */           \
    DtPt*  m_pPt;               /* Port the function belongs to. NULL, means device */   \
    Int  m_Address;             /* Absolute base address of the block */                 \
    DtBcType  m_Type;           /* Type of block */                                      \
    Int  m_Version;             /* Version of the block. Read from Block-ID register */  \
    Int  m_NumInterrupts;       /* Number of interrupts the block supports */            \
    DtBcInterruptProps*  m_IntProps;  /* List of properties per interrupt */             \
    /* Exclusive access */                                                               \
    Bool  m_ExclAccess;         /* True=inuse, False=unused */                           \
    DtExclAccessObject  m_ExclAccessOwner;  /* Current owner */                          \
    DtMutex  m_ExclAccessLock   /* Mutex protecting the access state */
#define DT_BC_COMMON_DATA                                                                \
    DT_BC_COMMON_DATA_PRIV;     /* Common private data. Donot touch these members */     \
    /* Callback functions */                                                             \
    DtBcInitFunc  m_InitFunc;                                                            \
    DtBcEnableFunc  m_EnableFunc;                                                        \
    DtBcOnEnable  m_OnEnableFunc;                                                        \
    DtBcCloseFunc  m_CloseFunc;                                                          \
    DtBcOnCloseFileFunc  m_OnCloseFileFunc
// The struct
struct  _DtBc
{
    DT_BC_COMMON_DATA;
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public DtBc functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtBc_CheckBlockId(DtBc*);
void  DtBc_Close(DtBc*);
DtStatus  DtBc_ExclAccessAcquire(DtBc*, const DtExclAccessObject*);
DtStatus  DtBc_ExclAccessCheck(DtBc*, const DtExclAccessObject*);
DtStatus  DtBc_ExclAccessProbe(DtBc*);
DtStatus  DtBc_ExclAccessRelease(DtBc*, const DtExclAccessObject*);
DtStatus  DtBc_OnCloseFile(DtBc*, const DtFileObject*);
DtStatus  DtBc_InterruptDisable(DtBc*, Int  Id);
DtStatus  DtBc_InterruptEnable(DtBc*, Int  Id);
DtStatus  DtBc_InterruptsDisable(DtBc*);
DtStatus  DtBc_InterruptsEnable(DtBc*);
DtStatus  DtBc_IntHandlerRegister(DtBc*, Int  Id, DtBcIntHandlerFunc, void*);
DtStatus  DtBc_IntHandlerUnregister(DtBc*, Int  Id);
DtStatus  DtBc_IntHandlerUnregisterAll(DtBc*);
DtBc*  DtBc_Open(const  DtBcOpenParams*);
#ifdef DT_BC_ADD_BLOCK_INCLUDES
DtBc*  DtBc_OpenType(DtBcType, Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
#endif  // #ifdef DT_BC_ADD_BLOCK_INCLUDES
DtStatus  DtBc_ToInstanceId(const DtBc*, char*  pInstanceId, Int  MaxLength);
DtStatus  DtBc_ToInstanceIdFromStrings(const char*  pShortName, Int  Instance,
                                                      char*  pInstanceId, Int  MaxLength);
DtStatus  DtBc_ToPropertyName(const DtBc*, const char*  pProp, 
                                                    char*  pFullPropName, Int  MaxLength);
DtStatus  DtBc_ToPropertyNameFromStrings(const char*  pInstanceId, 
                                                    const char*  pProp, 
                                                    char*  pFullPropName, Int  MaxLength);
UInt32  DtBc_RegRead32(DtBc*, Int  Offset);
void  DtBc_RegWrite32(DtBc*, Int  Offset, UInt32  Value);

DtStatus  DtBc_SetOpState(DtBc*, DtBcState  NewState);
const char*  DtBc_OpStateToString(DtBcState  State);
Bool  DtBc_IsEnabled(const DtBc*);
Bool  DtBc_IsInitialised(const DtBc*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBc definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DtDbgOutIoStubBc(Level, Module, pStub, Msg, ...)                                 \
                DtDbgOutIoStub_Prefix(Level, BC_, Module, pStub, Msg, ##__VA_ARGS__)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcOpenParams -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoStubBcOpenParams
{
    DT_IOSTUB_INIT_OPEN_PARAMS_COMMON_DATA;
    DtBc*  m_pBc;                   // The BC controller by this stub
}  DtIoStubBcOpenParams;
// MACRO to initialise open parameters
#define DT_IOSTUBBC_INIT_OPEN_PARAMS(OP, STRUCT, BC, NAME, CLOSE_F, IOCTL_F, IOCTLS)     \
do                                                                                       \
{                                                                                        \
    DT_IOSTUB_INIT_OPEN_PARAMS(OP, STRUCT, BC->m_pCore, NAME, CLOSE_F, IOCTL_F, IOCTLS); \
    OP.m_ObjectType = DT_OBJECT_TYPE_IOSTUB_BC;                                          \
    OP.m_pBc = BC;                                                                       \
}                                                                                        \
while (0)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Common data for all block io-control stubs
#define DT_IOSTUB_BC_COMMON_DATA                                                         \
    DT_IOSTUB_COMMON_DATA;                                                               \
    DtBc*  m_pBc        /* Points to the BC controlled by the stub */                    \
// The struct
struct _DtIoStubBc
{
    DT_IOSTUB_BC_COMMON_DATA;
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public DtIoStubBc functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBc_Close(DtIoStub*);
DtIoStubBc*  DtIoStubBc_Open(DtIoStubBcOpenParams*);
#ifdef DT_BC_ADD_BLOCK_INCLUDES
DtIoStubBc*  DtIoStubBc_OpenType(DtBc*);
#endif  // #ifdef DT_BC_ADD_BLOCK_INCLUDES
DtStatus  DtIoStubBc_OnExclAccessCmd(const DtIoStub*, DtIoStubIoParams*, Int*  pOutSize);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- block-controller includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#ifdef DT_BC_ADD_BLOCK_INCLUDES
#include "DtBcACCUFIFO.h"
#include "DtBcASIRXP.h"
#include "DtBcASITXG.h"
#include "DtBcASITXSER.h"
#include "DtBcBURSTFIFO.h"
#include "DtBcCDMAC.h"
#include "DtBcCONSTSINK.h"
#include "DtBcCONSTSOURCE.h"
#include "DtBcFANC.h"
#include "DtBcFPGATEMP.h"
#include "DtBcGENL.h"
#include "DtBcGS2988.h"
#include "DtBcI2CM.h"
#include "DtBcIPSECG.h"
#include "DtBcKA.h"
#include "DtBcLEDB.h"
#include "DtBcLMH1981.h"
#include "DtBcMSIX.h"
#include "DtBcREBOOT.h"
#include "DtBcSDIXCFG.h"
#include "DtBcSDIRXF.h"
#include "DtBcSDIRXP.h"
#include "DtBcSDIRXPHY.h"
#include "DtBcSDITXF.h"
#include "DtBcSDITXP.h"
#include "DtBcSDITXPHY.h"
#include "DtBcSDITXPLL.h"
#include "DtBcSPIM.h"
#include "DtBcSPIMF.h"
#include "DtBcSWITCH.h"
#include "DtBcTOD.h"
#include "DtBcTSRXFMT.h"
#include "DtBcVVI.h"

// Local block includes
#include "DtBcAD5320_2132.h"
#include "DtBcADS4246_2132.h"
#include "DtBcDATASTMUX_2132.h"
#include "DtBcIQCGRAB_2132.h"
#include "DtBcIQFIR_2132.h"
#include "DtBcIQPWR_2132.h"
#include "DtBcIQSRC2132_2132.h"
#include "DtBcLNBH25_2132.h"
#include "DtBcS2DEC_2132.h"
#include "DtBcS2DEMOD_2132.h"
#include "DtBcS2STATS_2132.h"

// List with of well-known block-controller IDs
extern const DtBcId DT_BC_KNOWN[];
extern const Int  DT_BC_NUM_KNOWN;

#endif // #ifdef DT_BC_ADD_BLOCK_INCLUDES


#endif  // #ifndef __DT_BC_H
