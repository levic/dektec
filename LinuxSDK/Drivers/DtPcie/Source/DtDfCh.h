// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfCh.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2022 DekTec
//
// Base implementation for a channel driver function.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2022 DekTec Digital Video B.V.
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

#ifndef __DT_DF_CH_H
#define __DT_DF_CH_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
// Forwards
typedef struct _DtDfChUser DtDfChUser;
typedef struct _DtDfCh DtDfCh;
#include "DtVectorDfChUser.h"




// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct DtDfChUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#define DT_DF_CH_USER_COMMON_DATA                                                        \
    Int m_Size;             /* Set to size of this struct OR of a 'derived' struct */    \
    DtFileObject m_File;            /* File handle of the user (process) */              \
    char m_FriendlyName[DT_CHAN_FRIENDLY_NAME_MAX_LENGTH+1];                             \
                                    /* Friendly name used to identify the user */        \
    DtFuncOpMode m_OpMode;          /* Operation mode as requested by user */            \
    /* True, when the user has acquired exclusive access over this channel */            \
    Bool m_HasExclAccess
struct _DtDfChUser
{
    DT_DF_CH_USER_COMMON_DATA;
};

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfCh definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Callback functions
typedef void (*DtDfChCloseUserFunc)(DtDfCh*, DtDfChUser*, const DtFileObject*);
typedef DtStatus (*DtDfChDoIdleToStandyFunc)(DtDfCh*);
typedef DtStatus (*DtDfChDoRunToStandbyFunc)(DtDfCh*);
typedef DtStatus (*DtDfChDoStandbyToIdleFunc)(DtDfCh*);
typedef DtStatus (*DtDfChDoStandbyToRunFunc)(DtDfCh*);
typedef DtStatus (*DtDfChInitFunc)(DtDfCh*);
typedef DtStatus (*DtDfChOpenChildrenFunc)(DtDfCh*);
typedef DtDfChUser* (*DtDfChOpenUserFunc)(DtDfCh*, const DtFileObject*, 
                                           Bool HasExclAccess, const char* pFriendlyName);
typedef DtStatus (*DtDfChOnDetachLastUserFunc)(DtDfCh*);

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChOpenParams -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Struct holding with the parameters for the Open function
typedef struct  _DtDfChOpenParams
{
    // Common open parameters
    DtDfOpenParams m_DfParams;      // Base driver function open parameters

    // Channel specific open parameters
    Int m_MaxNumUsersSupported;     // Maximum #users supported by the channel

    DtDfChCloseUserFunc m_CloseUserFunc;        // Close an user function
    DtDfChDoIdleToStandyFunc m_DoIdleToStandyFunc;  // Execute a IDLE->RUN transition
    DtDfChDoRunToStandbyFunc m_DoRunToStandbyFunc;  // Execute a RUN->STANDY transition
    DtDfChDoStandbyToIdleFunc m_DoStandbyToIdleFunc; // Execute a STANDBY->IDLE transition
    DtDfChDoStandbyToRunFunc m_DoStandbyToRunFunc;  // Execute a STANDY->RUN transition
    DtDfChInitFunc m_InitFunc;                  // Initialize channel function
    DtDfChOnDetachLastUserFunc m_OnDetachLastUserFunc;  // On detach from last user func
    DtDfChOpenChildrenFunc m_OpenChildrenFunc;  // Open the channels children
    DtDfChOpenUserFunc m_OpenUserFunc;          // Open an user function

}  DtDfChOpenParams;
// MACRO to initialize open parameters safe initial values and set base parameters
#define DT_DF_CH_INIT_OPEN_PARAMS(OP, STRUCT, ID, TYPE, PORT, STUB, CORE)                \
            DT_DF_INIT_OPEN_PARAMS(OP.m_DfParams, STRUCT, ID, TYPE, PORT, STUB, CORE)

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct DtDfCh -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#define DT_DF_CH_COMMON_DATA                                                             \
    DT_DF_COMMON_DATA;              /* Derived from DtDf */                              \
    Int m_MaxNumUsersSupported;     /* Maximum #users supported by the channel */        \
    DtVectorDfChUser* m_pUsers;     /* List of all users attached to this channel */     \
    DtSpinLock m_UsersLock;         /* spin lock to protect the user list  */            \
    DtMutex m_StateLock;            /* Mutex protecting state of DtDfChSdiRx */          \
    DtFuncOpMode m_OpMode;          /* Channel's current operational mode */             \
    /* Callback functions */                                                             \
    DtDfChCloseUserFunc m_CloseUserFunc;                                                 \
    DtDfChDoIdleToStandyFunc m_DoIdleToStandyFunc;                                       \
    DtDfChDoRunToStandbyFunc m_DoRunToStandbyFunc;                                       \
    DtDfChDoStandbyToIdleFunc m_DoStandbyToIdleFunc;                                     \
    DtDfChDoStandbyToRunFunc m_DoStandbyToRunFunc;                                       \
    DtDfChInitFunc m_InitChanFunc;                                                       \
    DtDfChOnDetachLastUserFunc m_OnDetachLastUserFunc;                                   \
    DtDfChOpenChildrenFunc m_OpenChildrenFunc;                                           \
    DtDfChOpenUserFunc m_OpenUserFunc
struct _DtDfCh
{
    DT_DF_CH_COMMON_DATA;
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus DtDfCh_AddUser(DtDfCh*, const DtFileObject*, Bool HasExclAccess, const char*);
DtStatus DtDfCh_Attach(DtDfCh*, const DtFileObject*, const DtExclAccessObject*, 
                                      Bool ReqExclusiveAccess, const char* pFriendlyName);
void DtDfCh_Close(DtDfCh*);
void DtDfCh_CloseUser(DtDfCh*, DtDfChUser*, const DtFileObject*);
DtStatus DtDfCh_Detach(DtDfCh*, const DtExclAccessObject*, const DtFileObject*);
DtFuncOpMode DtDfCh_FindHighestOpModeRequested(const DtDfCh* pDf);
DtDfChUser* DtDfCh_FindUser(DtDfCh*, const DtFileObject*);
Bool DtDfCh_HasUsers(const DtDfCh*);
DtStatus DtDfCh_Init(DtDfCh*);
DtStatus DtDfCh_Lock(DtDfCh*, int Timeout);
void DtDfCh_LockUsers(DtDfCh*);
DtStatus DtDfCh_OnCloseFile(DtDf* pDf, const DtFileObject*);
DtStatus DtDfCh_OnCloseOtherFiles(DtDf* pDf, const DtFileObject*);
DtDf* DtDfCh_Open(const DtDfChOpenParams*);
DtDfChUser* DtDfCh_OpenUser(DtDfCh*, Int SizeOfObject, const DtFileObject*, 
                                           Bool HasExclAccess, const char* pFriendlyName);
DtStatus DtDfCh_OpenChildren(DtDfCh*, const DtDfSupportedChild*, Int NumSupported);
DtStatus DtDfCh_RemoveUser(DtDfCh*, const DtFileObject*, DtDfChUser*);
DtStatus DtDfCh_SetOperationalMode(DtDfCh*, DtFuncOpMode);
void DtDfCh_Unlock(DtDfCh*);
void DtDfCh_UnlockUsers(DtDfCh*);


#endif // #ifndef __DT_DF_CH_H