//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPt.h *#*#*#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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

#ifndef __DT_PT_H
#define __DT_PT_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtSal.h"
#include "DtBc.h"
#include "DtDf.h"
#include "DtBcDfCommon.h"
#include "DtVectorBc.h"
#include "DtVectorDf.h"
#include "DtVectorPt.h"
#include "DtIoCaps.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Defines / Constants +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Tag for memory allocated by a port
#define PT_TAG  0x20205450     // '  TP'

// Maximum size of PT instance ID string
#define DT_PT_INSTANCE_ID_MAX_SIZE  64

// Prefix PT debug logging with the PT instance ID + port index
#define DtDbgOutPt(Level, Module, pPt, Msg, ...)                                         \
do                                                                                       \
{                                                                                        \
        DtDbgOut_Prefix(Level, PT_, Module, "[%s] " Msg,                                 \
                                pPt->m_InstanceId,                                       \
                                ##__VA_ARGS__);                                          \
} while(0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPt definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef enum  _DtPtState
{
    PT_STATE_UNDEFINED=0,       // PT has not been initilised yet and is unusable
    PT_STATE_INITIALISED,       // PT's resouces have been allocated and register access
                                // is possible. However all it's threads should be 
                                // suspended/IDLE and it may not make use of interrupts
    PT_STATE_ENABLED,           // PT is fully enabled and may use of interrupts and 
                                // threads

}  DtPtState;

// Driver-function callback functions
typedef DtPt*  (*DtPtOpenFunc)(DtFileObject*);
typedef void  (*DtPtCloseFunc)(DtPt*);
typedef DtStatus  (*DtPtEnableFunc)(DtPt*, Bool  Enable);
typedef DtStatus  (*DtPtInitFunc)(DtPt*);
typedef DtStatus (*DtPtOnCloseFileFunc)(DtPt*, DtFileObject*);
typedef DtStatus (*DtPtSetIoConfigFunc)(DtPt*, const DtCfIoConfigValue*, Int NumIoCfgs);
typedef DtStatus (*DtPtSetIoConfigPrepareFunc)(DtPt*, const DtExclAccessObject*);
typedef DtStatus (*DtPtSetIoConfigFinishFunc)(DtPt*, const DtExclAccessObject*);
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtOpenParams -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Struct holding with the parameters for the Open function
typedef struct  _DtPtOpenParams
{
    Int  m_Size;                    // Size of thestruct
    DtPortType  m_Type;             // Type of port
    const char*  m_pName;           // Name of the PT
    Int  m_PortIndex;               // Port index
    DtCore*  m_pCore;               // Core driver services
    DtPtCloseFunc  m_CloseFunc;     // Close function
    DtPtEnableFunc  m_EnableFunc;   // Enable function
    DtPtInitFunc  m_InitFunc;       // Initialisation function
    DtPtOnCloseFileFunc  m_OnCloseFileFunc;  // On close file function
    DtPtSetIoConfigFunc  m_SetIoConfigFunc;  // Set IO configuration
    DtPtSetIoConfigPrepareFunc  m_SetIoConfigPrepareFunc; // Prepare for set IO config
    DtPtSetIoConfigFinishFunc  m_SetIoConfigFinishFunc;   // Finish set IO config
}  DtPtOpenParams;

// MACRO to initialise open parameters safe initial values and set base parameters
#define DT_PT_INIT_OPEN_PARAMS(OP, STRUCT, NAME, PORT, TYPE, CORE)                       \
do                                                                                       \
{                                                                                        \
    DtMemZero(&OP, sizeof(OP));                                                          \
    OP.m_Size = sizeof(STRUCT);                                                          \
    OP.m_pName = NAME;                                                                   \
    OP.m_PortIndex = PORT;                                                               \
    OP.m_Type = TYPE;                                                                    \
    OP.m_pCore = CORE;                                                                   \
}                                                                                        \
while (0)
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#define DT_PT_COMMON_DATA                                                                \
    DT_OBJECT_COMMON_DATA;           /* Derive from DtObject */                          \
    char  m_InstanceId[DT_PT_INSTANCE_ID_MAX_SIZE];                                      \
                                    /* Instance ID string */                             \
    DtPtState  m_OpState;           /* Current operational state of the port */          \
    Int  m_PortIndex;               /* Physical port index. */                           \
    DtFunctionType  m_Type;         /* Type of port */                                   \
    DtCore*  m_pCore;               /* Shortcut to core driver services */               \
    DtIoCaps  m_IoCaps;             /* Port IO capabilities */                           \
    /* Children: block-controller and driver-functions */                                \
    DtVectorBc*  m_pBcList;                                                              \
    DtVectorDf*  m_pDfList;                                                              \
    /* Exclusive access */                                                               \
    DtVector*  m_pBcExclAccessList;                                                      \
    DtVector*  m_pDfExclAccessList;                                                      \
    DtMutex  m_ExclAccessLock;   /* Mutex protecting the access state */                 \
    /* Callback functions */                                                             \
    DtPtInitFunc  m_InitFunc;                                                            \
    DtPtEnableFunc  m_EnableFunc;                                                        \
    DtPtCloseFunc  m_CloseFunc;                                                          \
    DtPtOnCloseFileFunc  m_OnCloseFileFunc;                                              \
    DtPtSetIoConfigFunc  m_SetIoConfigFunc;                                              \
    DtPtSetIoConfigPrepareFunc  m_SetIoConfigPrepareFunc;                                \
    DtPtSetIoConfigFinishFunc  m_SetIoConfigFinishFunc

// The struct
struct _DtPt
{
    DT_PT_COMMON_DATA;
};


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Protected DtPt functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtPt_Close(DtPt*);
DtStatus  DtPt_OnCloseFile(DtPt*  pPort, DtFileObject*  pFile);
DtPt*  DtPt_Open(const DtPtOpenParams*);
DtPt*  DtPt_OpenType(DtPortType, DtCore*, Int  PortIndex);
DtBc * DtPt_FindBc(DtPt*, DtBcType, const char * pRole);
DtDf * DtPt_FindDf(DtPt*, DtFunctionType, const char * pRole);
DtStatus  DtPt_OpenChildren(DtPt*);
DtStatus  DtPt_CloseChildren(DtPt*);
DtStatus  DtPt_EnableChildren(DtPt*, Bool  Enable);
DtStatus  DtPt_AcquireExclAccessChildren(DtPt* pPt, const DtExclAccessObject*);
DtStatus  DtPt_ProbeExclAccessChildren(DtPt*  pPt);
DtStatus  DtPt_ReleaseExclAccessChildren(DtPt* pPt, const DtExclAccessObject*);
DtStatus  DtPt_SetOpState(DtPt*, DtPtState  NewState);
const char*  DtPt_OpStateToString(DtPtState);
Bool  DtPt_IsEnabled(const DtPt*);
Bool  DtPt_IsInitialised(const DtPt*);
DtStatus DtPt_InitIoCaps(DtPt*);

                                                               
#endif  // #ifndef __DT_PT_H

