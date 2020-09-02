// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfTxPllMgr.c *#*#*#*#*#*#*#*#*#*# (C) 2020 DekTec
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
#include "DtDfTxPllMgr.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTxPllMgr implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiRx function
#define DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfTxPllMgr))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_TXPLLMGR_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(TXPLLMGR, pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfTxPllMgr_Init(DtDf*);
static DtStatus  DtDfTxPllMgr_CalibrateInt(DtDfTxPllMgr*);
static DtStatus  DtDfTxPllMgr_CheckTxPllsLocked(DtDfTxPllMgr*, Bool* pLocked);
static DtStatus  DtDfTxPllMgr_CleanUp(DtDfTxPllMgr*);
static DtStatus  DtDfTxPllMgr_FindTxPll(DtDfTxPllMgr * pDf, Int PllId, DtDfTxPllMgrTxPll*);
static DtStatus  DtDfTxPllMgr_InitTxPllLookupTable(DtDfTxPllMgr*);
static DtStatus  DtDfTxPllMgr_LoadParameters(DtDf*);
static DtStatus  DtDfTxPllMgr_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfTxPllMgr_OpenChildren(DtDfTxPllMgr*);
static DtStatus  DtDfTxPllMgr_ResetTxPlls(DtDfTxPllMgr*);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTxPllMgr - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfTxPllMgr_Close(DtDf*  pDfBase)
{
    DtDfTxPllMgr*  pDf = (DtDfTxPllMgr*)pDfBase;

    DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf);

    // Clean-up
    DtDfTxPllMgr_CleanUp(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDfBase);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfTxPllMgr*  DtDfTxPllMgr_Open(DtCore*  pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_TXPLLMGR_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfTxPllMgr, Id, DT_FUNC_TYPE_TXPLLMGR, pPt,
                                                                            FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfTxPllMgr_Close;
    OpenParams.m_InitFunc = DtDfTxPllMgr_Init;
    OpenParams.m_LoadParsFunc = DtDfTxPllMgr_LoadParameters;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfTxPllMgr_OnEnablePostChildren;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfTxPllMgr*)DtDf_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_Calibrate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfTxPllMgr_Calibrate(DtDfTxPllMgr*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf);
    DF_TXPLLMGR_MUST_BE_ENABLED(pDf);

    // Calibrate under mutex protection
    DtFastMutexAcquire(&pDf->m_AccessMutex);

    Status = DtDfTxPllMgr_CalibrateInt(pDf);

    DtFastMutexRelease(&pDf->m_AccessMutex);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_IsPllLocked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTxPllMgr_IsPllLocked(DtDfTxPllMgr* pDf, Int PllId, Bool* pLocked)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfTxPllMgrTxPll TxPll;
    // Sanity checks
    DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf);
    DF_TXPLLMGR_MUST_BE_ENABLED(pDf);
    if (pLocked == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    DtFastMutexAcquire(&pDf->m_AccessMutex);

    // Find the SDITXPLL
    Status = DtDfTxPllMgr_FindTxPll(pDf, PllId, &TxPll);
    if (!DT_SUCCESS(Status))
    {
        DtFastMutexRelease(&pDf->m_AccessMutex);
        return Status;
    }

    // Get the locked status
    switch(TxPll.m_BcTxPllType)
    {
    //case DT_BLOCK_TYPE_HDMITXPLL:
    //    Status = DtBcHDMITXPLL_IsPllLocked((DtBcHDMITXPLL*)TxPll.m_pBcTxPll, pLocked);
   //     break;
    case DT_BLOCK_TYPE_SDITXPLL:
        Status = DtBcSDITXPLL_IsPllLocked((DtBcSDITXPLL*)TxPll.m_pBcTxPll, pLocked);
        break;
    default:
        DT_ASSERT(FALSE);
        Status =  DT_STATUS_NOT_IMPLEMENTED;
        break;
    }
    DtFastMutexRelease(&pDf->m_AccessMutex);
    return Status;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTxPllMgr - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTxPllMgr_Init(DtDf*  pDfBase)
{
    DtDfTxPllMgr*  pDf = (DtDfTxPllMgr*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf);

    DtFastMutexInit(&pDf->m_AccessMutex);

    // Create list for the SDITXPLLs
    DT_ASSERT(pDf->m_pBcSdiTxPlls == NULL);
    pDf->m_pBcSdiTxPlls = DtVectorBc_Create(4, 4);
    DT_ASSERT(pDf->m_pBcSdiTxPlls != NULL);
    //// Create list for the HDMITXPLLs
    //DT_ASSERT(pDf->m_pBcHdmiTxPlls == NULL);
    //pDf->m_pBcHdmiTxPlls = DtVectorBc_Create(4, 4);
    //DT_ASSERT(pDf->m_pBcHdmiTxPlls != NULL);
    // Create TXPLL look-up table
    DT_ASSERT(pDf->m_pTxPllTable == NULL);
    pDf->m_pTxPllTable = DtVector_Create(4, sizeof(DtDfTxPllMgrTxPll), 4);
    DT_ASSERT(pDf->m_pTxPllTable != NULL);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfTxPllMgr_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, TXPLLMGR, pDf, "ERROR: failed to open children");
        DtDfTxPllMgr_CleanUp(pDf);
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_CalibrateInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfTxPllMgr_CalibrateInt(DtDfTxPllMgr* pDf)
{
    DtStatus Status = DT_STATUS_OK;
    Bool PllsAreLocked = FALSE;
    Int  RetryCount = 3;

    // Sanity checks
    DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf);

    // Reset/recalibrate the TXPLLs and check lock. Maximum retry this 3 times.
    for (; RetryCount>0 && !PllsAreLocked; RetryCount--)
    {
        Int  MaxLockTime = 100;
        Status = DtDfTxPllMgr_ResetTxPlls(pDf);
        for (; DT_SUCCESS(Status) && MaxLockTime>0 && !PllsAreLocked; MaxLockTime--)
        {
            DtSleep(1);
            Status = DtDfTxPllMgr_CheckTxPllsLocked(pDf, &PllsAreLocked);
        }
        // Do we need a retry?
        if (!DT_SUCCESS(Status))
        {
            DT_ASSERT(FALSE);
            DtSleep(200);
            DtDbgOutDf(ERR, TXPLLMGR, pDf, "ERROR: TX-PLLs not locked");
        }
    }
    if (!PllsAreLocked && DT_SUCCESS(Status))
        Status = DT_STATUS_FAIL;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_CheckTxPllsLocked -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfTxPllMgr_CheckTxPllsLocked(DtDfTxPllMgr* pDf, Bool* pLocked)
{
    Int i;
    Bool IsLocked = TRUE;
    *pLocked = FALSE;
    // Sanity checks
    DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_pBcSdiTxPlls!=NULL && pDf->m_pBcSdiTxPlls!=NULL 
                                                             && pDf->m_pTxPllTable!=NULL);
    // Check all SDITXPLLs
    for (i=0; i<DtVectorBc_Size(pDf->m_pBcSdiTxPlls) && IsLocked; i++)
    {
        DtBcSDITXPLL* pSdiTxPll = (DtBcSDITXPLL*)DtVectorBc_At(pDf->m_pBcSdiTxPlls, i);
        if (pSdiTxPll != NULL)
            DT_RETURN_ON_ERROR(DtBcSDITXPLL_IsPllLocked(pSdiTxPll, &IsLocked));
    }
    // Check all HDMITXPLLs
    //for (i=0; i<DtVectorBc_Size(pDf->m_pBcHdmiTxPlls) && IsLocked; i++)
    //{
    //    DtBcHDMITXPLL* pHdmiTxPll = (DtBcHDMITXPLL*)DtVectorBc_At(pDf->m_pBcHdmiTxPlls,i);
    //    if (pHdmiTxPll != NULL)
    //        DT_RETURN_ON_ERROR(DtBcHDMITXPLL_IsPllLocked(pHdmiTxPll, &IsLocked));
    //}
    *pLocked = IsLocked;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_CleanUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfTxPllMgr_CleanUp(DtDfTxPllMgr* pDf)
{
    // Sanity checks
    DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf);
    // Clean-up SDITXPLLs
    if (pDf->m_pBcSdiTxPlls != NULL)
    {
        DtVectorBc_Cleanup(pDf->m_pBcSdiTxPlls);
        pDf->m_pBcSdiTxPlls = NULL;
    }
    // Clean-up HDMITXPLLs
    //if (pDf->m_pBcHdmiTxPlls != NULL)
    //{
    //    DtVectorBc_Cleanup(pDf->m_pBcHdmiTxPlls);
    //    pDf->m_pBcHdmiTxPlls = NULL;
    //}
    // Clean-up TXPLL look-up table
    if (pDf->m_pTxPllTable != NULL)
    {
        DtVector_Cleanup(pDf->m_pTxPllTable);
        pDf->m_pTxPllTable = NULL;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_FindTxPll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTxPllMgr_FindTxPll(DtDfTxPllMgr* pDf, Int PllId, DtDfTxPllMgrTxPll* pTxPll)
{
    Int i;
    DtDfTxPllMgrTxPll* pItem;
    // Sanity checks
    DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf);
    // Parameter check
    if (pTxPll == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    // Find the SDITXPLL
    if (pDf->m_pTxPllTable==NULL || PllId==-1)
        return DT_STATUS_NOT_FOUND;
    for (i=0; i<DtVector_Size(pDf->m_pTxPllTable); i++)
    {
        pItem = (DtDfTxPllMgrTxPll*)DtVector_At(pDf->m_pTxPllTable, i);
        if (pItem->m_PllId == PllId)
        {
            *pTxPll = *pItem;
            return DT_STATUS_OK;
        }
    }
    return DT_STATUS_NOT_FOUND;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_InitSdiTxPllLookupTable -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfTxPllMgr_InitTxPllLookupTable(DtDfTxPllMgr* pDf)
{
    Int i;
    Int  NumTxPlls = 0;
    // Sanity checks
    DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_pBcSdiTxPlls!=NULL //&& pDf->m_pBcHdmiTxPlls!=NULL
                                                             && pDf->m_pTxPllTable!=NULL);
    // Resize look-up table
    NumTxPlls = DtVectorBc_Size(pDf->m_pBcSdiTxPlls);
                                               // + DtVectorBc_Size(pDf->m_pBcHdmiTxPlls);
    DT_RETURN_ON_ERROR(DtVector_Resize(pDf->m_pTxPllTable, NumTxPlls));

    // Fill the look-up table with SDITXPLLs
    for (i=0; i<DtVectorBc_Size(pDf->m_pBcSdiTxPlls); i++)
    {
        DtDfTxPllMgrTxPll  Item;
        Item.m_BcTxPllType = DT_BLOCK_TYPE_SDITXPLL;
        Item.m_pBcTxPll = (DtBc*)DtVectorBc_At(pDf->m_pBcSdiTxPlls, i);
        if (Item.m_pBcTxPll != NULL)
            DT_RETURN_ON_ERROR(DtBcSDITXPLL_GetPllId((DtBcSDITXPLL*)Item.m_pBcTxPll,
                                                                          &Item.m_PllId));
        else
            Item.m_PllId= -1;
        DT_RETURN_ON_ERROR(DtVector_Set(pDf->m_pTxPllTable, i, &Item));
    }
    // Fill the look-up table with HDMITXPLLs
    //for (i=0; i<DtVectorBc_Size(pDf->m_pBcHdmiTxPlls); i++)
    //{
    //    DtDfTxPllMgrTxPll  Item;
    //    Item.m_BcTxPllType = DT_BLOCK_TYPE_HDMITXPLL;
    //    Item.m_pBcTxPll = (DtBc*)DtVectorBc_At(pDf->m_pBcHdmiTxPlls, i);
    //    if (Item.m_pBcTxPll != NULL)
    //        DT_RETURN_ON_ERROR(DtBcHDMITXPLL_GetPllId((DtBcHDMITXPLL*)Item.m_pBcTxPll,
    //                                                                      &Item.m_PllId));
    //    else
    //        Item.m_PllId= -1;
    //    DT_RETURN_ON_ERROR(DtVector_Set(pDf->m_pTxPllTable, i, &Item));
    //}
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfTxPllMgr_LoadParameters(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfTxPllMgr* pDf = (DtDfTxPllMgr*)pDfBase;

    // List of SI-534X  function parameters
    DtDfParameters  DFTXPLLMGR_PARS[] =
    {
        // Name,  Value Type,  Value*
        {"CALIBRATE_ON_STARTUP", PROPERTY_VALUE_TYPE_BOOL, &(pDf->m_CalibrateOnStartUp) },
    };

    // Sanity checks
    DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf);
 
    // Init parameters to their defaults
    pDf->m_CalibrateOnStartUp = FALSE;

    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFTXPLLMGR_PARS), DFTXPLLMGR_PARS);
    if (!DT_SUCCESS(Status))
        return Status;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTxPllMgr_OnEnablePostChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfTxPllMgr* pDf = (DtDfTxPllMgr*)pDfBase;

    // Sanity checks
    DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        // Init SDITXPLL look-up table
        Status = DtDfTxPllMgr_InitTxPllLookupTable(pDf);
        // Calibrate TX-PLLs
        if (DT_SUCCESS(Status) && pDf->m_CalibrateOnStartUp)
        {
            // Wait till clocks are stable
            DtSleep(100);

            // Perform calibration
            Status = DtDfTxPllMgr_CalibrateInt(pDf);
        }
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTxPllMgr_OpenChildren(DtDfTxPllMgr*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // Get the SDI-TX-PLLs
    Status = DtDf_OpenChildrenOfBcType((DtDf*)pDf, DT_BLOCK_TYPE_SDITXPLL, 
                                                                     pDf->m_pBcSdiTxPlls);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, TXPLLMGR, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
    //// Get the HDMI-TX-PLLs
    //Status = DtDf_OpenChildrenOfBcType((DtDf*)pDf, DT_BLOCK_TYPE_HDMITXPLL, 
    //                                                                pDf->m_pBcHdmiTxPlls);
    //if (!DT_SUCCESS(Status))
    //{
    //    DtDbgOutDf(ERR, TXPLLMGR, pDf, "ERROR: failed to open children");
    //    return DT_STATUS_FAIL;
    //}

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr_ResetSdiTxPlls -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfTxPllMgr_ResetTxPlls(DtDfTxPllMgr* pDf)
{
    Int i;
    // Sanity checks
    DF_TXPLLMGR_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_pBcSdiTxPlls!=NULL //&& pDf->m_pBcHdmiTxPlls!=NULL 
                                                             && pDf->m_pTxPllTable!=NULL);
    // Issue reset to all SDITXPLL
    for (i=0; i<DtVectorBc_Size(pDf->m_pBcSdiTxPlls); i++)
    {
        DtBcSDITXPLL* pSdiTxPll = (DtBcSDITXPLL*)DtVectorBc_At(pDf->m_pBcSdiTxPlls, i);
        if (pSdiTxPll != NULL)
            DT_RETURN_ON_ERROR(DtBcSDITXPLL_ResetClock(pSdiTxPll));
    }
    // Issue reset to all HDMITXPLL
    //for (i=0; i<DtVectorBc_Size(pDf->m_pBcHdmiTxPlls); i++)
    //{
    //    DtBcHDMITXPLL* pHdmiTxPll = (DtBcHDMITXPLL*)DtVectorBc_At(pDf->m_pBcHdmiTxPlls, i);
    //    if (pHdmiTxPll != NULL)
    //        DT_RETURN_ON_ERROR(DtBcHDMITXPLL_ResetClock(pHdmiTxPll));
    //}
    return DT_STATUS_OK;
}