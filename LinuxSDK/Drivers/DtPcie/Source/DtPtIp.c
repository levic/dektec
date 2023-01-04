// .-.-.-.-.-.-.-.-.-.- DtPtIp.c *#*#*#*#*#*#*#*#*#* (C) 2020 DekTec -.-.-.-.-.-.-.-.-.-.-
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2019 DekTec Digital Video B.V.
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtPtIp.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPtIp implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=



// MACRO to enable/disable a driver function; it returns on error
#define ENABLE_DRIVERFUNC_RETURN_ON_ERR(FUNC, ENABLE)                       \
    do {                                                                    \
        if (FUNC != NULL)                                                   \
        {                                                                   \
            DtStatus  dr = FUNC->m_EnableFunc((DtDf*)FUNC, ENABLE);         \
            if (!DT_SUCCESS(dr))                                            \
                return dr;                                                  \
        }                                                                   \
    } while (0)


// MACRO to enable/disable a block controller; it returns on error
#define ENABLE_BLOCKCTRL_RETURN_ON_ERR(BLOCK, ENABLE)                       \
    do {                                                                    \
        if (BLOCK != NULL)                                                  \
        {                                                                   \
            DtStatus  dr = BLOCK->m_EnableFunc((DtBc*)BLOCK, ENABLE);       \
            if (!DT_SUCCESS(dr))                                            \
                return dr;                                                  \
        }                                                                   \
    } while (0)

// MACRO with default precondition checks for the PtIp function
#define PT_IP_DEFAULT_PRECONDITIONS(pPt)      \
    DT_ASSERT(pPt!=NULL && pPt->m_Size==sizeof(DtPtIp))


// Helper macro to cast a DtDf* to aDtPtAsiSdiMon*
#define PT_IP      ((DtPtIP*)pPt)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtPtIp_Init(DtPt*);
static DtStatus DtPtIp_OnCloseFile(DtPt*, DtFileObject*);
static DtStatus DtPtIp_Enable(DtPt*, Bool Enable);

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtIp - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtIp_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtPtIp_Close(DtPt*  pPt)
{
    PT_IP_DEFAULT_PRECONDITIONS(pPt);

    // Let base function perform final clean-up
    DtPt_Close(pPt);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtIp_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtPtIp* DtPtIp_Open(DtCore* pCore, Int PortIndex, DtPortType Type)
{
    DtPtOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(PortIndex >= 0);

    DT_ASSERT(Type==DT_PORT_TYPE_IP);

    // Init open parameters
    DT_PT_INIT_OPEN_PARAMS(OpenParams,DtPtIp, DT_PT_IP_NAME, PortIndex, Type, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtPtIp_Close;
    OpenParams.m_InitFunc = DtPtIp_Init;
    OpenParams.m_EnableFunc = DtPtIp_Enable;
    OpenParams.m_OnCloseFileFunc = DtPtIp_OnCloseFile;
    //OpenParams.m_SetIoConfigFunc = DtPtIp_SetIoConfig;
    //OpenParams.m_SetIoConfigPrepareFunc = DtPtIp_SetIoConfigPrepare;
    //OpenParams.m_SetIoConfigFinishFunc = DtPtIp_SetIoConfigFinish;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtPtIp*)DtPt_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtIp_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtIp_Init(DtPt* pPtBase)
{
    //DtPtIp*  pPt = (DtPtIp*)pPtBase;
    // Sanity check
    //PT_IP_DEFAULT_PRECONDITIONS(pPt);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtIp_Enable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Overrides the default implementation.
//
DtStatus DtPtIp_Enable(DtPt* pPt, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    PT_IP_DEFAULT_PRECONDITIONS(pPt);

    // Will the state change??
    if ((pPt->m_OpState==PT_STATE_ENABLED && Enable) 
                                     || (pPt->m_OpState==PT_STATE_INITIALISED && !Enable))
        return DT_STATUS_OK;

    // Let the base enable/disable the children
    Status = DtPt_EnableChildren(pPt, Enable);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPt(ERR, COMMON, pPt, "ERROR: failed to enable/disbale children");
        return Status;
    }

    Status = DtPt_SetOpState(pPt, Enable ? PT_STATE_ENABLED : PT_STATE_INITIALISED);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtIp_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtIp_OnCloseFile(DtPt* pPtBase, DtFileObject* pObject)
{
     DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    PT_IP_DEFAULT_PRECONDITIONS(pPtBase);

     // Let the base do the work
     Status = DtPt_OnCloseFile(pPtBase, pObject);
    return Status;
}
