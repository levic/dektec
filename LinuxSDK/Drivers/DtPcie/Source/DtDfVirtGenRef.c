// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfVirtGenRef.c *#*#*#*#*#*#*#*#*#* (C) 2019 DekTec
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
#include "DtDfVirtGenRef.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
#define BLKCTRL_ROLE_NONE      NULL

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfVirtGenRef implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_VIRTGENREF_DEFAULT_PRECONDITIONS(pDf)      \
                               DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfVirtGenRef))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_VIRTGENREF_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(VIRTGENREF, pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfVirtGenRef_Init(DtDf*);
static void  DtDfVirtGenRef_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfVirtGenRef - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVirtGenRef_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfVirtGenRef_Close(DtDf*  pDf)
{
    DF_VIRTGENREF_DEFAULT_PRECONDITIONS(pDf);

    // Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pDf->m_pCore, (DtObject*)pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVirtGenRef_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfVirtGenRef*  DtDfVirtGenRef_Open(DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                  Int Instance, Int Uuid, Bool CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_VIRTGENREF_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfVirtGenRef, Id, DT_FUNC_TYPE_VIRTGENREF, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfVirtGenRef_Close;
    OpenParams.m_InitFunc = DtDfVirtGenRef_Init;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfVirtGenRef*)DtDf_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVirtGenRef_GetGenRefEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfVirtGenRef_GetGenRefEnable(DtDfVirtGenRef* pDf, Bool* pEnable)
{
    // Sanity check
    DF_VIRTGENREF_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pEnable == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_VIRTGENREF_MUST_BE_ENABLED(pDf);

    // Return cached value
    *pEnable = pDf->m_GenRefEnabled;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVirtGenRef_GetGenRefVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfVirtGenRef_GetGenRefVidStd(DtDfVirtGenRef* pDf, Int* pVidStd)
{
    // Sanity check
    DF_VIRTGENREF_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pVidStd == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_VIRTGENREF_MUST_BE_ENABLED(pDf);

    // Return cached value
    *pVidStd = pDf->m_GenRefVidStd;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVirtGenRef_SetGenRefEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfVirtGenRef_SetGenRefEnable(DtDfVirtGenRef* pDf, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    const DtTodTime  DumySof = {0, 0};

    // Sanity checks
    DF_VIRTGENREF_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_VIRTGENREF_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_GenRefEnabled == Enable)
        return DT_STATUS_OK;

    // Change GenRefEnable under spinlock protection
    DtSpinLockAcquire(&pDf->m_Lock);
    pDf->m_GenRefEnabled = Enable;

    // Send one final event if disabled
    if (!Enable && pDf->m_pDfGenLockCtrl != NULL)
        DtDfGenLockCtrl_GenRefStartOfFrameHandler(pDf->m_pDfGenLockCtrl, 
                                                     pDf->m_PortIndex, &DumySof,
                                                     DT_VIDSTD_UNKNOWN, DT_VIDSTD_UNKNOWN,
                                                     DT_DF_GENLOCKCTRL_GENREF_UNDEFINED);
    DtSpinLockRelease(&pDf->m_Lock);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVirtGenRef_SetGenRefVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfVirtGenRef_SetGenRefVidStd(DtDfVirtGenRef* pDf, Int  VidStd)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_VIRTGENREF_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_VIRTGENREF_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_GenRefVidStd == VidStd)
        return DT_STATUS_OK;

    // Change GenRefEnable under spinlock protection
    DtSpinLockAcquire(&pDf->m_Lock);
    pDf->m_GenRefVidStd = VidStd;
    DtSpinLockRelease(&pDf->m_Lock);

    return Status;
}
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfVirtGenRef - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVirtGenRef_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfVirtGenRef_Init(DtDf* pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfVirtGenRef* pDf = (DtDfVirtGenRef*)pDfBase;
    DtOnPeriodicIntervalRegData  RegData;

    // Sanity checks
    DF_VIRTGENREF_DEFAULT_PRECONDITIONS(pDf);

    // Init spinlock
    DtSpinLockInit(&pDf->m_Lock);

    // Save port index
    pDf->m_PortIndex = DtCore_PT_GetPortIndex(pDf->m_pPt);

    // Set defaults
    pDf->m_GenRefVidStd = DT_VIDSTD_UNKNOWN;
    pDf->m_GenRefEnabled = FALSE;

    // Find the device level genlock controller
    pDf->m_pDfGenLockCtrl = (DtDfGenLockCtrl*)DtCore_DF_Find(pDf->m_pCore, NULL, 
                                                          DT_FUNC_TYPE_GENLOCKCTRL, NULL);

    // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtDfVirtGenRef_PeriodicIntervalHandler;
    RegData.m_pObject = (DtObject*)pDf;
    Status = DtCore_TOD_PeriodicItvRegister(pDf->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, VIRTGENREF, pDf, "ERROR: failed to register period itv handler");
        return Status;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtDfVirtGenRef_PeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfVirtGenRef_PeriodicIntervalHandler(DtObject* pObj, DtTodTime  Time)
{
    DtDfVirtGenRef*  pDf = (DtDfVirtGenRef*)pObj;
    const Int DetectVidStd = DT_VIDSTD_UNKNOWN;

    DF_VIRTGENREF_DEFAULT_PRECONDITIONS(pDf);

    DtSpinLockAcquireAtDpc(&pDf->m_Lock);
    // Only when enabled
    if (pDf->m_GenRefEnabled && pDf->m_pDfGenLockCtrl!=NULL)
    {
        DtDfGenLockCtrl_GenRefStartOfFrameHandler(pDf->m_pDfGenLockCtrl, pDf->m_PortIndex,
                                               &Time, pDf->m_GenRefVidStd, DetectVidStd,
                                               DT_DF_GENLOCKCTRL_GENREF_VIRTUAL);

    }
    DtSpinLockReleaseFromDpc(&pDf->m_Lock);
}
