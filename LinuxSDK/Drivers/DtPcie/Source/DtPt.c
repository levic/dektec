//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPt.c *#*#*#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtPt.h"
#include "DtPtAsiSdiMon.h"
#include "DtPtAsiSdiRxTx.h"
#include "DtPtIp.h"
#include "DtPtModOutp.h"
#include "DtPtSdiPhyOnlyRxTx.h"
#include "DtPtSdiGenRef.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPt implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Pt
#define PT_DEFAULT_PRECONDITIONS(pPt)      \
    DT_ASSERT(pPt!=NULL && pPt->m_Size>=sizeof(DtPt))


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtPt_Enable(DtPt*, Bool  Enable);
DtStatus  DtPt_ToInstanceIdFromStrings(const char * pName, Int Port,
                                                       char * pInstanceId, Int MaxLength);
DtStatus  DtPt_SetIoConfig(DtPt *, const DtCfIoConfigValue*, Int);
DtStatus  DtPt_SetIoConfigPrepare(DtPt*, const DtExclAccessObject*);
DtStatus  DtPt_SetIoConfigFinish(DtPt*, const DtExclAccessObject*);
static DtStatus  DtPt_ReleaseExclAccessChildrenImpl(DtPt*,  const DtExclAccessObject* );
DT_UNUSED static Bool  DtPt_NeedsSetIoConfigImplementation(DtPt * pPt);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPt- Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtPt_Close(DtPt*  pPt)
{
    if (pPt == NULL)
        return;     // Nothing to close

    DtDbgOutPt(MIN, COMMON, pPt, "Closing PT");

    // Close any children
    DtPt_CloseChildren(pPt);

    // Cleanup BC and DF lists
    if (pPt->m_pBcList != NULL)
        DtVectorBc_Cleanup(pPt->m_pBcList);
    if (pPt->m_pDfList != NULL)
        DtVectorDf_Cleanup(pPt->m_pDfList);
    if (pPt->m_pBcExclAccessList != NULL)
        DtVector_Cleanup(pPt->m_pBcExclAccessList);
    if (pPt->m_pDfExclAccessList != NULL)
        DtVector_Cleanup(pPt->m_pDfExclAccessList);

    // Free function data
    DtObject_Close((DtObject*)pPt, PT_TAG);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPt_OnCloseFile(DtPt*  pPt, DtFileObject*  pFile)
{
    Int  i=0, Num=0;
    
    PT_DEFAULT_PRECONDITIONS(pPt);


    DtDbgOutPt(MAX, COMMON, pPt, "Releasing resources held by File");

    // Notify all DF's and BC's that the file is closed. 
    // NOTE: first notify all DFs, than the BCs
    Num = DtVectorDf_Size(pPt->m_pDfList);
    for (i=0; i<Num; i++)
    {
        DtDf*  pDf = DtVectorDf_At(pPt->m_pDfList, i);
        if (pDf==NULL || pDf->m_OnCloseFileFunc==NULL)
            continue;
        pDf->m_OnCloseFileFunc(pDf, pFile);
    }

    Num = DtVectorBc_Size(pPt->m_pBcList);
    for (i=0; i<Num; i++)
    {
        DtBc*  pBc = DtVectorBc_At(pPt->m_pBcList, i);
        if (pBc==NULL || pBc->m_OnCloseFileFunc==NULL)
            continue;
        pBc->m_OnCloseFileFunc(pBc, pFile);
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtPt*  DtPt_Open(const DtPtOpenParams*  pParams)
{
    DtStatus  Status = DT_STATUS_OK;
    DtPt*  pPt = NULL;

    DT_ASSERT(pParams != NULL);
    DT_ASSERT(pParams->m_Size >= sizeof(DtPt));
    DT_ASSERT(pParams->m_pCore!=NULL 
                                    && pParams->m_pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(pParams->m_pName != NULL);
    DT_ASSERT(pParams->m_CloseFunc != NULL);
    DT_ASSERT(pParams->m_PortIndex >= 0);
    // All three IO-config callback functions are specified or none
    DT_ASSERT(   (pParams->m_SetIoConfigFunc==NULL 
                                              && pParams->m_SetIoConfigPrepareFunc==NULL
                                              && pParams->m_SetIoConfigFinishFunc==NULL)
              || (pParams->m_SetIoConfigFunc!=NULL 
                                              && pParams->m_SetIoConfigPrepareFunc!=NULL
                                              && pParams->m_SetIoConfigFinishFunc!=NULL));

    //.-.-.-.-.-.-.-.-.-.-.- Step 1: Allocate memory for PT object -.-.-.-.-.-.-.-.-.-.-.-

    pPt = (DtPt*)DtObject_Open(DT_OBJECT_TYPE_PT, pParams->m_Size, PT_TAG);
    if (pPt == NULL)
        return NULL;
    
    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 2: Init PT attributes -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    
    // Init exclusive access mutex
    DtMutexInit(&pPt->m_ExclAccessLock);
   // pPt->m_ExclAccess = FALSE;

    // Set port index
    pPt->m_PortIndex = pParams->m_PortIndex;
    pPt->m_Type = pParams->m_Type;
    
    // Init instance-ID string.
    Status = DtPt_ToInstanceIdFromStrings(pParams->m_pName, pPt->m_PortIndex+1,
                                   pPt->m_InstanceId, DT_SIZEOF_ARRAY(pPt->m_InstanceId));
    DT_ASSERT(DT_SUCCESS(Status));

    // Set shortcut to core services data
    pPt->m_pCore = pParams->m_pCore;

    // Set the IO-capabilities for this port
    Status = DtPt_InitIoCaps(pPt);
    DT_ASSERT(DT_SUCCESS(Status));

    // Init child BC list. Set initial capacity to 8 and grow size to 4
    DT_ASSERT(pPt->m_pBcList == NULL);
    pPt->m_pBcList = DtVectorBc_Create(8, 4);
    DT_ASSERT(pPt->m_pBcList != NULL);

    // Init child DF list. Set initial capacity to 4 and grow size to 2
    DT_ASSERT(pPt->m_pDfList == NULL);
    pPt->m_pDfList = DtVectorDf_Create(4, 2);
    DT_ASSERT(pPt->m_pDfList != NULL);
    
    // Init child BC exclusive access list
    DT_ASSERT(pPt->m_pBcExclAccessList == NULL);
    pPt->m_pBcExclAccessList = DtVector_Create(8, sizeof(Bool), 4);
    DT_ASSERT(pPt->m_pBcExclAccessList != NULL);

    // Init child DF exclusive access list
    DT_ASSERT(pPt->m_pDfExclAccessList == NULL);
    pPt->m_pDfExclAccessList = DtVector_Create(4, sizeof(Bool), 2);
    DT_ASSERT(pPt->m_pDfExclAccessList != NULL);


    // Register our close function and other callbacks
    pPt->m_CloseFunc = pParams->m_CloseFunc;
    pPt->m_InitFunc = pParams->m_InitFunc;

    // Did the derived class define it's own Enable callback?
    if (pParams->m_EnableFunc != NULL)
        pPt->m_EnableFunc = pParams->m_EnableFunc;
    else
        pPt->m_EnableFunc = DtPt_Enable;

#ifdef LINBUILD
    pPt->m_MmapFunc = pParams->m_MmapFunc;
#else
    DT_ASSERT(!pParams->m_MmapFunc);
    pPt->m_MmapFunc = NULL;     // Not supported for Linux
#endif

    // Did the derived class define it's own set IO config callbacks?
    if (pParams->m_SetIoConfigFunc!=NULL && pParams->m_SetIoConfigPrepareFunc!=NULL
                                                && pParams->m_SetIoConfigFinishFunc!=NULL)
    {
        pPt->m_SetIoConfigFunc = pParams->m_SetIoConfigFunc;
        pPt->m_SetIoConfigPrepareFunc = pParams->m_SetIoConfigPrepareFunc;
        pPt->m_SetIoConfigFinishFunc = pParams->m_SetIoConfigFinishFunc;
    }
    else
    {
        // Check whether the SetIoConfig implementation is necessary
        DT_ASSERT(!DtPt_NeedsSetIoConfigImplementation(pPt));
        pPt->m_SetIoConfigFunc = DtPt_SetIoConfig;
        pPt->m_SetIoConfigPrepareFunc = DtPt_SetIoConfigPrepare;
        pPt->m_SetIoConfigFinishFunc = DtPt_SetIoConfigFinish;
    }

    // Did the derived class define it's own File-Close callback?
    if (pParams->m_OnCloseFileFunc != NULL)
        pPt->m_OnCloseFileFunc = pParams->m_OnCloseFileFunc;
    else
        pPt->m_OnCloseFileFunc = DtPt_OnCloseFile;      // Use default function

    // Open children
    Status = DtPt_OpenChildren(pPt);
    if (!DT_SUCCESS(Status))
    {
        DT_ASSERT(pPt->m_CloseFunc != NULL);
        pPt->m_CloseFunc(pPt);
        return NULL;
    }

    // Perform initilisation (if an init function exists)
    if (pPt->m_InitFunc != NULL)
    {
        Status = pPt->m_InitFunc(pPt);
        if (!DT_SUCCESS(Status))
        {
            DT_ASSERT(pPt->m_CloseFunc != NULL);
            pPt->m_CloseFunc(pPt);
            return NULL;
        }
    }

    // Set the state to initialised
    Status = DtPt_SetOpState(pPt, PT_STATE_INITIALISED);
    DT_ASSERT(DT_SUCCESS(Status));
    
    return pPt; 
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_FindBc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBc*  DtPt_FindBc(DtPt*  pPt, DtBcType  Type, const char*  pRole)
{
    PT_DEFAULT_PRECONDITIONS(pPt);
    DT_ASSERT(pPt->m_pCore != NULL);
    return DtCore_BC_Find(pPt->m_pCore, pPt, Type, pRole);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_FindDf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDf*  DtPt_FindDf(DtPt*  pPt, DtFunctionType  Type, const char*  pRole)
{
    PT_DEFAULT_PRECONDITIONS(pPt);
    DT_ASSERT(pPt->m_pCore != NULL);
    return DtCore_DF_Find(pPt->m_pCore, pPt, Type, pRole);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPt_OpenChildren(DtPt*  pPt)
{
    Int  PortIndex=0;
    DtStatus  Status = DT_STATUS_OK;
    DtCore*  pCore = NULL;
    Int i, NumDf, NumBc;
    Bool InitExclAccess = FALSE;
    
    PT_DEFAULT_PRECONDITIONS(pPt);
    pCore = pPt->m_pCore;
    CORE_DEFAULT_PRECONDITIONS(pCore);
    PortIndex = pPt->m_PortIndex;
    DT_ASSERT(PortIndex >= 0);


    //-.-.-.-.-.-.-.-.-.-.- Load non-core port DFs and BCs -.-.-.-.-.-.-.-.-.-.-
    // Open non-core driver-functions
    Status = DtCore_DF_OpenAll((DtCore*)pCore,  pPt);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, CORE, "Failed to load DFs (ERROR: 0x%08X)", Status);
        return Status;
    }
    
    // Initialize the child DF exclusive access list
    NumDf = DtVectorDf_Size(pPt->m_pDfList);
    DtVector_Resize(pPt->m_pDfExclAccessList, NumDf);
    for (i=0; i<NumDf; i++)
        DtVector_Set(pPt->m_pDfExclAccessList, i, &InitExclAccess);

    // Open non-core block-controllers
    Status = DtCore_BC_OpenAll((DtCore*)pCore, pPt);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, CORE, "Failed to open BCs (ERROR: 0x%08X)", Status);
        return Status;
    }

    // Initialize the child BC exclusive access list
    NumBc = DtVectorBc_Size(pPt->m_pBcList);
    DtVector_Resize(pPt->m_pBcExclAccessList, NumBc);
    for (i=0; i<NumBc; i++)
        DtVector_Set(pPt->m_pBcExclAccessList, i, &InitExclAccess);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_CloseChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPt_CloseChildren(DtPt*  pPt)
{
    Int  i=0;

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 1: close child DFs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // NOTE: we close them in reverse order as they were loaded

    if (pPt->m_pDfList != NULL)
    {
        Int  NumDf = DtVectorDf_Size(pPt->m_pDfList);
        for (i=(NumDf-1); i>=0; i--)
        {
            DtDf*  pChildDf= DtVectorDf_At(pPt->m_pDfList, i);
            if (pChildDf == NULL)
                continue;
            DtCore_DF_Close(pChildDf);
            DtVectorDf_Set(pPt->m_pDfList, i, NULL);
        }
        DtVectorDf_Clear(pPt->m_pDfList);
    }

    // Clear the child DF exclusive access list
    DtVector_Clear(pPt->m_pDfExclAccessList);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 2: close child BCs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // NOTE: we close them in reverse order as they were loaded

    if (pPt->m_pBcList != NULL)
    {
        Int  NumBc = DtVectorBc_Size(pPt->m_pBcList);
        for (i=(NumBc-1); i>=0; i--)
        {
            DtBc*  pChildBc = DtVectorBc_At(pPt->m_pBcList, i);
            if (pChildBc == NULL)
                continue;
            DtCore_BC_Close(pChildBc);
            DtVectorBc_Set(pPt->m_pBcList, i, NULL);
        }
        DtVectorBc_Clear(pPt->m_pBcList);
    }
    // Clear the child BC exclusive access list
    DtVector_Clear(pPt->m_pBcExclAccessList);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_EnableChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPt_EnableChildren(DtPt*  pPt, Bool  Enable)
{
    Int  i=0, NumBc=0, NumDf=0;
    DtStatus  Status = DT_STATUS_OK;

    PT_DEFAULT_PRECONDITIONS(pPt);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Enable/Disable all Child-DFs -.-.-.-.-.-.-.-.-.-.-.-.-.-

    DT_ASSERT(pPt->m_pDfList != NULL);
    NumDf = DtVectorDf_Size(pPt->m_pDfList);
    for (i=0; i<NumDf; i++)
    {
        DtDf*  pChildDf = DtVectorDf_At(pPt->m_pDfList, i);
        if (pChildDf == NULL)
            continue;
        
        if (pChildDf->m_EnableFunc == NULL)
        {
            DtDbgOutPt(ERR, COMMON, pPt, "ERROR: no enable-callback for '%s' DF",
                                                                  pChildDf->m_InstanceId);
            DT_ASSERT(pChildDf->m_EnableFunc != NULL);
            continue;
        }
        Status = pChildDf->m_EnableFunc(pChildDf, Enable);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPt(ERR, COMMON, pPt, "ERROR: failed to enable '%s' DF",
                                                                  pChildDf->m_InstanceId);
            return Status;
        }
    }
     
    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Enable/Disable all Child-BCs -.-.-.-.-.-.-.-.-.-.-.-.-.-

    DT_ASSERT(pPt->m_pBcList != NULL);
    NumBc = DtVectorBc_Size(pPt->m_pBcList);
    for (i=0; i<NumBc; i++)
    {
        DtBc*  pChildBc = DtVectorBc_At(pPt->m_pBcList, i);
        if (pChildBc == NULL)
            continue;
    
        if (pChildBc->m_EnableFunc == NULL)
        {
            DtDbgOutPt(ERR, COMMON, pPt, "ERROR: no enable-callback for '%s' BC",
                                                                  pChildBc->m_InstanceId);
            DT_ASSERT(pChildBc->m_EnableFunc != NULL);
            continue;
        }
        Status = pChildBc->m_EnableFunc(pChildBc, Enable);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPt(ERR, COMMON, pPt, "ERROR: failed to enable '%s' BC",
                                                                  pChildBc->m_InstanceId);
            return Status;
        }
    }
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_AcquireExclAccessChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPt_AcquireExclAccessChildren(DtPt*  pPt,  const DtExclAccessObject* pObject)
{
    DtStatus  Status = DT_STATUS_OK;
    Int i, NumDf, NumBc;
    Bool* pHasExclAccess;

    // Sanity check
    PT_DEFAULT_PRECONDITIONS(pPt);
    DT_ASSERT(pObject != NULL);

    // Acquire exclusive access state lock
    if (DtMutexAcquire(&pPt->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    // Check the current child DF exclusive access state;
    NumDf = DtVector_Size(pPt->m_pDfExclAccessList);
    for (i=0; i<NumDf; i++)
    {
        pHasExclAccess = (Bool*)DtVector_At(pPt->m_pDfExclAccessList, i);
        DT_ASSERT(!(*pHasExclAccess));
        if (*pHasExclAccess)
        { 
            // Release exclusive access state lock
            DtMutexRelease(&pPt->m_ExclAccessLock);
            return DT_STATUS_IN_USE;
        }
    }

    // Check the current child BC exclusive access state;
    NumBc = DtVector_Size(pPt->m_pBcExclAccessList);
    for (i=0; i<NumBc; i++)
    {
        pHasExclAccess = (Bool*)DtVector_At(pPt->m_pBcExclAccessList, i);
        DT_ASSERT(!(*pHasExclAccess));
        if (*pHasExclAccess)
        { 
            // Release exclusive access state lock
            DtMutexRelease(&pPt->m_ExclAccessLock);
            return DT_STATUS_IN_USE;
        }
    }

    // Get exclusive access to child DFs
    NumDf = DtVector_Size(pPt->m_pDfExclAccessList);
    for (i=0; i<NumDf; i++)
    {
        // First check whether we have already exclusive access
        DtDf* pChildDf = DtVectorDf_At(pPt->m_pDfList, i);
        Status = DtDf_ExclAccessCheck(pChildDf, pObject);
        if (Status == DT_STATUS_EXCL_ACCESS_REQD)
        {
            // If we don't have exclusive access, try to acquire it
            Status = DtDf_ExclAccessAcquire(pChildDf, pObject);
            if (DT_SUCCESS(Status))
            {
                const Bool HasExclAccess = TRUE;
                DtVector_Set(pPt->m_pDfExclAccessList, i, &HasExclAccess);
            }
            else
                break;
        }
    }

    // Failed?
    if (!DT_SUCCESS(Status))
    {
        // Release exclusive access children
        DtPt_ReleaseExclAccessChildrenImpl(pPt, pObject);

        // Release exclusive access state lock
        DtMutexRelease(&pPt->m_ExclAccessLock);
        return Status;
    }

    // Get exclusive access to child Bcs
    NumBc = DtVector_Size(pPt->m_pBcExclAccessList);
    for (i=0; i<NumBc; i++)
    {
        // First check whether we have already exclusive access
        DtBc* pChildBc = DtVectorBc_At(pPt->m_pBcList, i);
        Status = DtBc_ExclAccessCheck(pChildBc, pObject);
        if (Status == DT_STATUS_EXCL_ACCESS_REQD)
        {
            // If we don't have exclusive access, try to acquire it
            Status = DtBc_ExclAccessAcquire(pChildBc, pObject);
            if (DT_SUCCESS(Status))
            {
                const Bool HasExclAccess = TRUE;
                DtVector_Set(pPt->m_pBcExclAccessList, i, &HasExclAccess);
            }
            else
                break;
        }
    }

    // Failed?
    if (!DT_SUCCESS(Status))
    {
        // Release exclusive access children
        DtPt_ReleaseExclAccessChildrenImpl(pPt, pObject);
    }

    // Release exclusive access state lock
    DtMutexRelease(&pPt->m_ExclAccessLock);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_ProbeExclAccessChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//
DtStatus DtPt_ProbeExclAccessChildren(DtPt*  pPt)
{
    DtStatus  Status = DT_STATUS_OK;
    Int i, NumDf, NumBc;

    // Sanity check
    PT_DEFAULT_PRECONDITIONS(pPt);

    // Acquire exclusive access state lock
    if (DtMutexAcquire(&pPt->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    // Probe exclusive access to child DFs
    NumDf = DtVector_Size(pPt->m_pDfExclAccessList);
    for (i=0; i<NumDf && DT_SUCCESS(Status); i++)
    {
        DtDf* pChildDf = DtVectorDf_At(pPt->m_pDfList, i);
        Status = DtDf_ExclAccessProbe(pChildDf);
    }

    // Probe exclusive access to child Bcs
    NumBc = DtVector_Size(pPt->m_pBcExclAccessList);
    for (i=0; i<NumBc && DT_SUCCESS(Status); i++)
    {
        DtBc* pChildBc = DtVectorBc_At(pPt->m_pBcList, i);
        Status = DtBc_ExclAccessProbe(pChildBc);
    }

    // Release exclusive access state lock
    DtMutexRelease(&pPt->m_ExclAccessLock);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_ReleaseExclAccessChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPt_ReleaseExclAccessChildren(DtPt*  pPt,  const DtExclAccessObject* pObject)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    PT_DEFAULT_PRECONDITIONS(pPt);
    DT_ASSERT(pObject != NULL);

    // Acquire exclusive access state lock
    if (DtMutexAcquire(&pPt->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    // Release exclusive children
    Status = DtPt_ReleaseExclAccessChildrenImpl(pPt, pObject);

    // Release exclusive access state lock
    DtMutexRelease(&pPt->m_ExclAccessLock);

    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_ReleaseExclAccessChildrenImpl -.-.-.-.-.-.-.-.-.-.-.-.-.
//
//
DtStatus DtPt_ReleaseExclAccessChildrenImpl(DtPt*  pPt, const DtExclAccessObject* pObject)
{
    DtStatus  Status = DT_STATUS_OK;
    Int i, NumDf, NumBc;

    // Release exclusive access to child DFs
    NumDf = DtVector_Size(pPt->m_pDfExclAccessList);
    for (i=0; i<NumDf; i++)
    {
        // Check whether we have acquired exclusive access
        Bool* pHasExclAccess = (Bool*)DtVector_At(pPt->m_pDfExclAccessList, i);
        if (*pHasExclAccess)
        {
            // Release it
            const Bool NoExclAccess = FALSE;
            DtDf* pChildDf = DtVectorDf_At(pPt->m_pDfList, i);
            Status = DtDf_ExclAccessRelease(pChildDf, pObject);
            DT_ASSERT(DT_SUCCESS(Status));
            DtVector_Set(pPt->m_pDfExclAccessList, i, &NoExclAccess);
        }
    }

    // Release exclusive access to child Bcs
    NumBc = DtVector_Size(pPt->m_pBcExclAccessList);
    for (i=0; i<NumBc; i++)
    {
        // Check whether we have acquired exclusive access
        Bool* pHasExclAccess = (Bool*)DtVector_At(pPt->m_pBcExclAccessList, i);
        if (*pHasExclAccess)
        {
            // Release it
            const Bool NoExclAccess = FALSE;
            DtBc* pChildBc = DtVectorBc_At(pPt->m_pBcList, i);
            Status = DtBc_ExclAccessRelease(pChildBc, pObject);
            DT_ASSERT(DT_SUCCESS(Status));
            DtVector_Set(pPt->m_pBcExclAccessList, i, &NoExclAccess);
        }
    }
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_OpenType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtPt*  DtPt_OpenType(DtPortType  Type, DtCore*  pCore, Int  PortIndex)
{
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(PortIndex>=0);

    switch(Type)
    {
    case DT_PORT_TYPE_BASE:
    {
        // Create
        DtPtOpenParams BaseParams;
        DT_PT_INIT_OPEN_PARAMS(BaseParams, DtPt, "BASE", PortIndex, Type, pCore);
        BaseParams.m_CloseFunc = DtPt_Close;
        return DtPt_Open(&BaseParams);
    }
    break;

    case DT_PORT_TYPE_ASISDIMON:
        return (DtPt*)DtPtAsiSdiMon_Open(pCore, PortIndex, Type);

    case DT_PORT_TYPE_ASISDIRX:
    case DT_PORT_TYPE_ASISDIRXTX:
    case DT_PORT_TYPE_ASISDITX:
        return (DtPt*)DtPtAsiSdiRxTx_Open(pCore, PortIndex, Type);
    
    case DT_PORT_TYPE_IP:
        return (DtPt*)DtPtIp_Open(pCore, PortIndex, Type);
    

    case DT_PORT_TYPE_MODOUTP:
        return (DtPt*)DtPtModOutp_Open(pCore, PortIndex, Type);

    case DT_PORT_TYPE_SDIPHYONLYRX:
    case DT_PORT_TYPE_SDIPHYONLYRXTX:
    case DT_PORT_TYPE_SDIPHYONLYTX:
        return (DtPt*)DtPtSdiPhyOnlyRxTx_Open(pCore, PortIndex, Type);

    case DT_PORT_TYPE_SDIGENREF:
        return (DtPt*)DtPtSdiGenRef_Open(pCore, PortIndex, Type);

    default:
        DT_ASSERT(FALSE);
        break;
    }
    return NULL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_SetOpState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPt_SetOpState(DtPt*  pPt, DtPtState  NewState)
{

    DT_UNUSED DtPtState  OldState = pPt->m_OpState;
    pPt->m_OpState = NewState;
    DtDbgOutPt(MIN, COMMON, pPt, "State change: %s->%s", DtPt_OpStateToString(OldState),
                                                          DtPt_OpStateToString(NewState));
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_OpStateToString -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
const char*  DtPt_OpStateToString(DtPtState  State)
{
    switch (State)
    {
    case PT_STATE_ENABLED:          return "ENABLED";
    case PT_STATE_INITIALISED:      return "INITIALISED";
    case PT_STATE_UNDEFINED:        return "UNDEFINED";
    }
    return "???";
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_IsEnabled -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtPt_IsEnabled(const DtPt*  pPt)
{
    PT_DEFAULT_PRECONDITIONS(pPt);
    return (pPt->m_OpState == PT_STATE_ENABLED);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_IsInitialised -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtPt_IsInitialised(const DtPt*  pPt)
{
    PT_DEFAULT_PRECONDITIONS(pPt);
    return (pPt->m_OpState == PT_STATE_INITIALISED);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_ToInstanceIdFromStrings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPt_ToInstanceIdFromStrings(const char*  pName, Int  Port,
                                                       char*  pInstanceId, Int  MaxLength)
{
    DtString  TheString;
    DtStatus  Status = DT_STATUS_OK;

    DT_ASSERT(pInstanceId!=NULL && MaxLength>0);
       
    // Allocate string buffer
    Status = DtStringAlloc(&TheString, MaxLength);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, PT_COMMON, "ERROR: failed to allocate string-object");
        return Status;
    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Construct Instance ID -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    // Format: PT_<name> <portnumber> (e.g. PT_VPD#1)
    
#define  _CHECK_RESULT(Func)                    \
    do {                                        \
        Status = Func;                          \
        if(!DT_SUCCESS(Status))                 \
        {                                       \
            /* Free string buffer */            \
            DtStringFree(&TheString);           \
            return Status;                      \
        }                                       \
    } while(0)

    // Part 1: add fixed "PT_" prefix
    _CHECK_RESULT(DtStringAppendChars(&TheString, "PT_"));

    // Part 2: add the name
    DT_ASSERT(pName!=NULL && pName[0]!='\0');
    _CHECK_RESULT(DtStringAppendChars(&TheString, pName));
    
    // Part 3: add port number
    DT_ASSERT(Port > 0);
    _CHECK_RESULT(DtStringAppendChars(&TheString, "_PORT#"));
    _CHECK_RESULT(DtStringUIntegerToDtStringAppend(&TheString, 10, Port));
    
    // convert to a 'normal' character string
    _CHECK_RESULT(DtStringToCharString(&TheString, pInstanceId, MaxLength));

    // Free string buffer
    DtStringFree(&TheString);
#undef  _CHECK_RESULT

    return DT_STATUS_OK;
}

// Linux specific functions
#ifdef LINBUILD

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_Mmap -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Maps the DMA buffer of the port to user space provided the port has a channel with 
// CDMAC.
//
DtStatus DtPt_Mmap(DtPt* pPt, const DtFileObject* pFile, DtVma* pVma)
{
    PT_DEFAULT_PRECONDITIONS(pPt);

    // Check if we have a callback for mmap and call it
    return (pPt->m_MmapFunc) ? pPt->m_MmapFunc(pPt,pFile,pVma) : DT_STATUS_NOT_SUPPORTED;
}

#endif // #ifdef LINBUILD

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPt - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_Enable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPt_Enable(DtPt*  pPt, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;

    PT_DEFAULT_PRECONDITIONS(pPt);

    // Will the state change??
    if ((pPt->m_OpState==PT_STATE_ENABLED && Enable) 
                                     || (pPt->m_OpState==PT_STATE_INITIALISED && !Enable))
        return DT_STATUS_OK;


    // Enable/disable all the children
    Status = DtPt_EnableChildren(pPt, Enable);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPt(ERR, COMMON, pPt, "ERROR: failed to enable/disbale children");
        return Status;
    }


    // Set new state
    Status = DtPt_SetOpState(pPt, Enable ? PT_STATE_ENABLED : PT_STATE_INITIALISED);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_SetIoConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPt_SetIoConfig(DtPt* pPt, const DtCfIoConfigValue* pIoCfgs, Int NumIoCfgs)
{
    Int  IoCfg;

    // Sanity check
    PT_DEFAULT_PRECONDITIONS(pPt);
    DT_ASSERT(pIoCfgs != NULL);

    // Check whether expected IO-configs are there
    for (IoCfg=0; IoCfg<NumIoCfgs; IoCfg++)
    {
        switch (IoCfg)
        {
        case DT_IOCONFIG_IODIR:
        case DT_IOCONFIG_IOSTD:
        case DT_IOCONFIG_RFCLKSEL:
        case DT_IOCONFIG_TSRATESEL:
            // This is valid if the port does not support multiple options
            // E.g. port is configured as INPUT and the port has no OUTPUT capability
            break;
        default:
            // Unsupported IO-configs
            if (pIoCfgs[IoCfg].m_Value != DT_IOCONFIG_NONE)
            {
                Char IoCfgName[IOCONFIG_NAME_MAX_SIZE];
                IoCfgName[0] = '\0';
                IoConfigNameGet(IoCfg-1, IoCfgName, sizeof(IoCfgName));
                DtDbgOutPt(ERR, COMMON, pPt, "ERROR:Unsupported IO-Config: %s", 
                                                                               IoCfgName);
                return DT_STATUS_NOT_SUPPORTED;
            }
            break;
        }
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_SetIoConfigPrepare -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPt_SetIoConfigPrepare(DtPt* pPt, const DtExclAccessObject*  pObject)
{
    // Default implementation
    return DtPt_AcquireExclAccessChildren(pPt, pObject);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_SetIoConfigFinish -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPt_SetIoConfigFinish(DtPt* pPt, const DtExclAccessObject*  pObject)
{
    // Default implementation
    return DtPt_ReleaseExclAccessChildren(pPt, pObject);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_InitIoCaps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPt_InitIoCaps(DtPt*  pPt)
{
    Int  i, PortIndex;
    DtCore* pCore = NULL;

    PT_DEFAULT_PRECONDITIONS(pPt);
    pCore = pPt->m_pCore;
    CORE_DEFAULT_PRECONDITIONS(pCore);
    PortIndex = pPt->m_PortIndex;
    DT_ASSERT(PortIndex >= 0);

    // Clear all caps
    DtIoCapsClearCaps(&pPt->m_IoCaps);

    for (i=0; i<DT_IOCAPS_COUNT; i++)
    {
        if (DtCore_PROPS_GetBool(pCore, DtIoCapDefinitions[i].m_pCapName, PortIndex,
                                                                                   FALSE))
            DtIoCapsSetCap(&pPt->m_IoCaps, DtIoCapDefinitions[i].m_Cap);
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtPt_NeedsSetIoConfigImplementation -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Checks whether a SetIoConfig implementation is needed for this port
//
Bool  DtPt_NeedsSetIoConfigImplementation(DtPt* pPt)
{
    DtIoCaps* pIoCaps = NULL;
    Bool IsMultiDir, IsMultiStd, IsMultiRfClkSel, IsMultiTsRateSel, HasBoolIo;
    Int NumDir=0, NumStd=0, NumTsRate=0;

    // Sanity check
    PT_DEFAULT_PRECONDITIONS(pPt);
    pIoCaps = &pPt->m_IoCaps;

    // Has support for BoolIO?
    HasBoolIo = DtIoCapsHasCap(pIoCaps, DT_IOCAP_DMATESTMODE)
                                            || DtIoCapsHasCap(pIoCaps, DT_IOCAP_FAILSAFE)
                                            || DtIoCapsHasCap(pIoCaps, DT_IOCAP_FRACMODE)
                                            || DtIoCapsHasCap(pIoCaps, DT_IOCAP_GENLOCKED)
                                            || DtIoCapsHasCap(pIoCaps, DT_IOCAP_GENREF)
                                            || DtIoCapsHasCap(pIoCaps, DT_IOCAP_SWS2APSK);

    // Has support for multi directions?
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_DISABLED))
        NumDir++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_INPUT))
        NumDir++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_SHAREDANT))
        NumDir++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_INTINPUT))
        NumDir++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_INTOUTPUT))
        NumDir++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_MONITOR))
        NumDir++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_OUTPUT))
        NumDir++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_DBLBUF))
        NumDir++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_LOOPS2L3))
        NumDir++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_LOOPS2TS))
        NumDir++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_LOOPTHR))
        NumDir++;
    IsMultiDir = (NumDir>1);

    // Has support for multi IO standards?
    IsMultiStd = DtIoCapsHasCap(pIoCaps, DT_IOCAP_12GSDI)
                                              || DtIoCapsHasCap(pIoCaps, DT_IOCAP_3GSDI)
                                              || DtIoCapsHasCap(pIoCaps, DT_IOCAP_6GSDI)
                                              || DtIoCapsHasCap(pIoCaps, DT_IOCAP_HDSDI)
                                              || DtIoCapsHasCap(pIoCaps, DT_IOCAP_SDI)
                                              || DtIoCapsHasCap(pIoCaps, DT_IOCAP_SPISDI);
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_ASI))
        NumStd++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_AVENC))
        NumStd++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_DEKTECST))
        NumStd++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_DEMOD))
        NumStd++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_GPSTIME))
        NumStd++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_HDMI))
        NumStd++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_IFADC))
        NumStd++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_MOD))
        NumStd++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_PHASENOISE))
        NumStd++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_RS422))
        NumStd++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_SDIRX))
        NumStd++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_SPI))
        NumStd++;
    IsMultiStd |= (NumStd > 1);

    // Has support for multi RF-clock frequencies?
    IsMultiRfClkSel = DtIoCapsHasCap(pIoCaps, DT_IOCAP_RFCLKEXT)
                                            && DtIoCapsHasCap(pIoCaps, DT_IOCAP_RFCLKEXT);

    // Has support for multi TS-rates?
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_EXTTSRATE))
        NumTsRate++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_EXTRATIO))
        NumTsRate++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_INTTSRATE))
        NumTsRate++;
    if (DtIoCapsHasCap(pIoCaps, DT_IOCAP_LOCK2INP))
        NumTsRate++;
    IsMultiTsRateSel = (NumTsRate > 1);

   return HasBoolIo || IsMultiDir || IsMultiStd || IsMultiRfClkSel || IsMultiTsRateSel;
}