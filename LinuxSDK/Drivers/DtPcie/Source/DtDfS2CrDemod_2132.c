//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfS2CrDemod_2132.c *#*#*#*#*#*#*#*#* (C) 2017 DekTec
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDfS2CrDemod_2132.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define S2DEC_ROLE_NONE      NULL
#define S2DEMOD_ROLE_NONE    NULL

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfS2CrDemod_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfS2CrDemod function
#define DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfS2CrDemod_2132))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_S2CRDEMOD_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(S2CRDEMOD_2132, pDf)

// Helper macro to cast a DtDf* to a DtDfS2CrDemod_2132*
#define DF_S2CRDEMOD      ((DtDfS2CrDemod_2132*)pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfS2CrDemod_2132_Init(DtDf*);
static DtStatus  DtDfS2CrDemod_2132_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfS2CrDemod_2132_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfS2CrDemod_2132_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfS2CrDemod_2132_OpenChildren(DtDfS2CrDemod_2132*);
static void  DtDfS2CrDemod_2132_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);
static void  DtDfS2CrDemod_2132_RollOffUpdate(DtDfS2CrDemod_2132*);

//=+=+=+=+=+=+=+=+=+=+=+=+ DtDfS2CrDemod_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfS2CrDemod_2132_Close(DtDf*  pDf)
{
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    //Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pDf->m_pCore, (DtObject*)pDf);
    
    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_DemodReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfS2CrDemod_2132_DemodReset(DtDfS2CrDemod_2132 * pDf)
{
    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Set the S2Demod spectrum inversion setting
    return DtBcS2DEMOD_2132_DemodReset(pDf->m_pBcS2Demod);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfS2CrDemod_2132*  DtDfS2CrDemod_2132_Open(DtCore*  pCore, DtPt* pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_S2CRDEMOD_2132_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfS2CrDemod_2132, Id,
                                     DT_FUNC_TYPE_S2CRDEMOD_2132, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfS2CrDemod_2132_Close;
    OpenParams.m_InitFunc = DtDfS2CrDemod_2132_Init;
    OpenParams.m_OnCloseFileFunc = DtDfS2CrDemod_2132_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfS2CrDemod_2132_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfS2CrDemod_2132_OnEnablePreChildren;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfS2CrDemod_2132*)DtDf_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_GetCounters -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfS2CrDemod_2132_GetCounters(DtDfS2CrDemod_2132* pDf, UInt* pFrameCount,
                                       UInt* pFrameSkipCount, UInt* pLdcpSuccessCount,
                                       UInt* pLdcpIterationsCount, UInt* pBchSuccessCount)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Check parameters
     if (pFrameCount == NULL || pFrameSkipCount == NULL || pLdcpSuccessCount == NULL || 
                                 pLdcpIterationsCount == NULL || pBchSuccessCount == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Operational mode must be run
    if (pDf->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    Status = DtBcS2DEC_2132_GetCounters(pDf->m_pBcS2Dec,
                                          pFrameCount, pFrameSkipCount, pLdcpSuccessCount, 
                                          pLdcpIterationsCount, pBchSuccessCount);
    if (!DT_SUCCESS(Status))
        DtDbgOutDf(ERR, S2CRDEMOD_2132, pDf, "ERROR: failed to get S2Dec counters");

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_GetFreqOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfS2CrDemod_2132_GetFreqOffset(DtDfS2CrDemod_2132* pDf, Int* pCoarseFreq, 
                                                     Int* pFineFreq, Int* pDecimationRate)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (pCoarseFreq == NULL || pFineFreq == NULL || pDecimationRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Operational mode must be run
    if (pDf->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    Status = DtBcS2DEMOD_2132_GetFreqOffset(pDf->m_pBcS2Demod,
                                                 pCoarseFreq, pFineFreq, pDecimationRate);
    if (!DT_SUCCESS(Status))
        DtDbgOutDf(ERR, S2CRDEMOD_2132, pDf, "ERROR: failed to get S2Demod freq. offset");

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfS2CrDemod_2132_GetOperationalMode(DtDfS2CrDemod_2132* pDf, Int* pOpMode)
{
    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return cached operational mode
    *pOpMode = pDf->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_GetPlInformation -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfS2CrDemod_2132_GetPlInformation(DtDfS2CrDemod_2132* pDf, Int* pPlMode, 
                                                                           Int* pPlsValue)
{
    DtStatus  Status = DT_STATUS_OK;
    Int BcPlMode = 0;

    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (pPlMode == NULL || pPlsValue==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Operational mode must be run
    if (pDf->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Get the Pl mode and Pls value
    Status = DtBcS2DEMOD_2132_GetPlInformation(pDf->m_pBcS2Demod, &BcPlMode, pPlsValue);
    if (DT_SUCCESS(Status))
    {
        // Convert roll-off to driver type
        switch (BcPlMode)
        {
        case DT_BC_S2DEMOD_2132_PL_MODE_ACM:
            *pPlMode = DT_S2CRDEMOD_2132_PLMODE_ACM;
            break;
        case DT_BC_S2DEMOD_2132_PL_MODE_CCM:
            *pPlMode = DT_S2CRDEMOD_2132_PLMODE_CCM;
            break;
        case DT_BC_S2DEMOD_2132_PL_MODE_VCM:
            *pPlMode = DT_S2CRDEMOD_2132_PLMODE_VCM;
            break;        
        default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
        }
    }
    else
    {
        DtDbgOutDf(ERR, S2CRDEMOD_2132, pDf, "ERROR: failed to get S2Dec roll-off");
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_GetRollOff -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfS2CrDemod_2132_GetRollOff(DtDfS2CrDemod_2132* pDf, Int* pRollOff)
{
    DtStatus  Status = DT_STATUS_OK;
    Int BcRollOff;

    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (pRollOff == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Opereratational mode must be run
    if (pDf->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Get the S2Dec roll-off 
    Status = DtBcS2DEC_2132_GetRollOff(pDf->m_pBcS2Dec, &BcRollOff);
    if (DT_SUCCESS(Status))
    {
        // Convert roll-off to driver type
        switch (BcRollOff)
        {
        case DT_BC_S2DEC_2132_ROLL_OFF_0_05:
            *pRollOff = DT_S2CRDEMOD_2132_ROLL_OFF_0_05;
            break;
        case DT_BC_S2DEC_2132_ROLL_OFF_0_10:
            *pRollOff = DT_S2CRDEMOD_2132_ROLL_OFF_0_10;
            break;
        case DT_BC_S2DEC_2132_ROLL_OFF_0_15:
            *pRollOff = DT_S2CRDEMOD_2132_ROLL_OFF_0_15;
            break;
        case DT_BC_S2DEC_2132_ROLL_OFF_0_20:
            *pRollOff = DT_S2CRDEMOD_2132_ROLL_OFF_0_20;
            break;
        case DT_BC_S2DEC_2132_ROLL_OFF_0_25:
            *pRollOff = DT_S2CRDEMOD_2132_ROLL_OFF_0_25;
            break;
        case DT_BC_S2DEC_2132_ROLL_OFF_0_35:
            *pRollOff = DT_S2CRDEMOD_2132_ROLL_OFF_0_35;
            break;
        case DT_BC_S2DEC_2132_ROLL_OFF_UNKNOWN:
            *pRollOff = DT_S2CRDEMOD_2132_ROLL_OFF_UNKNOWN;
            break;
        default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
        }
    }
    else
    {
        DtDbgOutDf(ERR, S2CRDEMOD_2132, pDf, "ERROR: failed to get S2Dec roll-off");
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_GetSnrInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfS2CrDemod_2132_GetSnrInfo(DtDfS2CrDemod_2132* pDf, 
                                                         UInt* pSigPower, UInt* pErrPower)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (pSigPower==NULL || pErrPower==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Opereratational mode must be run
    if (pDf->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Get the S2Demod SNR information
    Status = DtBcS2DEMOD_2132_GetSnrInfo(pDf->m_pBcS2Demod, pSigPower, pErrPower);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, S2CRDEMOD_2132, pDf, "ERROR: failed to get S2Demod SNR info");
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_GetSpectrumInv -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfS2CrDemod_2132_GetSpectrumInv(DtDfS2CrDemod_2132* pDf, Int* pSpecInvCtrl)
{
    DtStatus  Status = DT_STATUS_OK;
    Int BcSpecInvCtrl;

    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (pSpecInvCtrl == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Get the S2Demod spectrum inversion setting
    Status = DtBcS2DEMOD_2132_GetSpectrumInv(pDf->m_pBcS2Demod, &BcSpecInvCtrl);
    if (DT_SUCCESS(Status))
    {
        switch (BcSpecInvCtrl)
        {
        case DT_BC_S2DEMOD_2132_SPECINV_AUTO:
            *pSpecInvCtrl = DT_S2CRDEMOD_2132_SPECINV_AUTO;
            break;
        case DT_BC_S2DEMOD_2132_SPECINV_INVERT:
            *pSpecInvCtrl = DT_S2CRDEMOD_2132_SPECINV_INVERT;
            break;
        case DT_BC_S2DEMOD_2132_SPECINV_NORMAL:
            *pSpecInvCtrl = DT_S2CRDEMOD_2132_SPECINV_NORMAL;
            break;
        default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
        }
    }
    else
    {
        DtDbgOutDf(ERR, S2CRDEMOD_2132, pDf, 
                                          "ERROR: failed to get S2Demod spectrum invert");
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfS2CrDemod_2132_GetStatus(DtDfS2CrDemod_2132* pDf, Bool* pSignalDetect, 
                                Bool* pSpecInv, Bool* pPlHdrSync, UInt* pPlHdrResyncCount)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (pSignalDetect==NULL || pSpecInv==NULL || pPlHdrSync==NULL
                                                               || pPlHdrResyncCount==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Opereratational mode must be run
    if (pDf->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Get the S2Demod signal status
    Status = DtBcS2DEMOD_2132_GetStatus(pDf->m_pBcS2Demod, pSignalDetect, pSpecInv,
                                                           pPlHdrSync, pPlHdrResyncCount);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, S2CRDEMOD_2132, pDf, "ERROR: failed to get S2Demod status");
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_GetSymbolRate -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfS2CrDemod_2132_GetSymbolRate(DtDfS2CrDemod_2132* pDf, Int* pDecRate, 
                              Int* pSamSymRatio, Int* pSymSamRatio, Int* pMeasSamSymRatio)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (pDecRate==NULL || pSamSymRatio==NULL || pSymSamRatio==NULL
                                                                || pMeasSamSymRatio==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Opereratational mode must be run
    if (pDf->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Get the S2Demod symbol rate
    Status = DtBcS2DEMOD_2132_GetSymbolRate(pDf->m_pBcS2Demod, pDecRate, pSamSymRatio,
                                                          pSymSamRatio, pMeasSamSymRatio);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, S2CRDEMOD_2132, pDf, "ERROR: failed to get S2Demod symbol rate");
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfS2CrDemod_2132_SetOperationalMode(DtDfS2CrDemod_2132*  pDf, Int  OpMode)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);
    // Check parameters
    if (OpMode!=DT_FUNC_OPMODE_IDLE && OpMode!=DT_FUNC_OPMODE_RUN)
    {
        DtDbgOutDf(ERR, S2CRDEMOD_2132, pDf, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // No change?
    if (pDf->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    DtSpinLockAcquire(&pDf->m_OpModeSpinLock);
    if (OpMode == DT_FUNC_OPMODE_IDLE)
    {
        // Set operational mode of S2DEC to IDLE
        Status = DtBcS2DEC_2132_SetOperationalMode(pDf->m_pBcS2Dec, DT_BLOCK_OPMODE_IDLE);
        // Set operational mode of S2DEMOD to IDLE
        if (DT_SUCCESS(Status))
            Status = DtBcS2DEMOD_2132_SetOperationalMode(pDf->m_pBcS2Demod,
                                                                    DT_BLOCK_OPMODE_IDLE);
    }
    else
    {
        // Operational state must be enabled
        DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);
    
        // Set operational mode of S2DEC to RUN
        Status = DtBcS2DEC_2132_SetOperationalMode(pDf->m_pBcS2Dec, DT_BLOCK_OPMODE_RUN);
        // Set operational mode of S2DEMOD to RUN
        if (DT_SUCCESS(Status))
            Status = DtBcS2DEMOD_2132_SetOperationalMode(pDf->m_pBcS2Demod,
                                                                    DT_BLOCK_OPMODE_RUN);
    }
    // Save new operational mode
    if (DT_SUCCESS(Status))
        pDf->m_OperationalMode = OpMode;

    DtSpinLockRelease(&pDf->m_OpModeSpinLock);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_SetPlInformation -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfS2CrDemod_2132_SetPlInformation(DtDfS2CrDemod_2132* pDf, Int PlMode,
                                                                             Int PlsValue)
{
    Int  BcPlMode;

    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Convert and check parameters
    switch (PlMode)
    {
    case DT_S2CRDEMOD_2132_PLMODE_ACM:
        BcPlMode = DT_BC_S2DEMOD_2132_PL_MODE_ACM;
        break;
    case DT_S2CRDEMOD_2132_PLMODE_CCM:
        BcPlMode = DT_BC_S2DEMOD_2132_PL_MODE_CCM;
        break;
    case DT_S2CRDEMOD_2132_PLMODE_VCM:
        BcPlMode = DT_BC_S2DEMOD_2132_PL_MODE_VCM;
        break;
    default:
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Set the S2Demod spectrum inversion setting
    return DtBcS2DEMOD_2132_SetPlInformation(pDf->m_pBcS2Demod, PlMode, PlsValue);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_SetSpectrumInv -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfS2CrDemod_2132_SetSpectrumInv(DtDfS2CrDemod_2132* pDf, Int SpecInvCtrl)
{
    Int BcSpecInvCtrl;

    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Convert and check parameters
    switch (SpecInvCtrl)
    {
    case DT_S2CRDEMOD_2132_SPECINV_AUTO:
        BcSpecInvCtrl = DT_BC_S2DEMOD_2132_SPECINV_AUTO;
        break;
    case DT_S2CRDEMOD_2132_SPECINV_INVERT:
        BcSpecInvCtrl = DT_BC_S2DEMOD_2132_SPECINV_INVERT;
        break;
    case DT_S2CRDEMOD_2132_SPECINV_NORMAL:
        BcSpecInvCtrl = DT_BC_S2DEMOD_2132_SPECINV_NORMAL;
        break;
    default:
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Set the S2Demod spectrum inversion setting
    return DtBcS2DEMOD_2132_SetSpectrumInv(pDf->m_pBcS2Demod, BcSpecInvCtrl);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_SetSymbolRate -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfS2CrDemod_2132_SetSymbolRate(DtDfS2CrDemod_2132* pDf, Int DecRate, 
                                                         Int SamSymRatio, Int SymSamRatio)
{
    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Set the S2Demod symbol reate
    return DtBcS2DEMOD_2132_SetSymbolRate(pDf->m_pBcS2Demod, DecRate, SamSymRatio, 
                                                                             SymSamRatio);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_SoftReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfS2CrDemod_2132_SoftReset(DtDfS2CrDemod_2132* pDf)
{
    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_S2CRDEMOD_MUST_BE_ENABLED(pDf);

    // Perform a soft reset to use MPI registers
    return DtBcS2DEMOD_2132_SoftReset(pDf->m_pBcS2Demod);
}
//=+=+=+=+=+=+=+=+=+=+=+=+ DtDfS2CrDemod_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfS2CrDemod_2132_Init(DtDf*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    DtOnPeriodicIntervalRegData  RegData;

    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Initialize spinlock
    DtSpinLockInit(&DF_S2CRDEMOD->m_OpModeSpinLock);

    // Set defaults
    DF_S2CRDEMOD->m_OperationalMode = DT_FUNC_OPMODE_IDLE;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfS2CrDemod_2132_OpenChildren(DF_S2CRDEMOD);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, S2CRDEMOD_2132, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtDfS2CrDemod_2132_PeriodicIntervalHandler;
    RegData.m_pObject = (DtObject*)pDf;
    Status = DtCore_TOD_PeriodicItvRegister(pDf->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, S2CRDEMOD_2132, pDf, 
                                          "ERROR: failed to register period itv handler");
        return Status;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtDfS2CrDemod_2132_OnCloseFile(DtDf* pDf, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Check if the owner closed the file handle
    Status = DtDf_ExclAccessCheck(pDf, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutDf(AVG, S2CRDEMOD_2132, pDf, "Go to IDLE");

        // Go to IDLE
        Status = DtDfS2CrDemod_2132_SetOperationalMode(DF_S2CRDEMOD, DT_FUNC_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, S2CRDEMOD_2132, pDf, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtDf_OnCloseFile(pDf, pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfS2CrDemod_2132_OnEnablePostChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtDfS2CrDemod_2132* pDf = (DtDfS2CrDemod_2132*) pDfBase;
    // Sanity check
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutDf(AVG, S2CRDEMOD_2132, pDf, "Function enable from disable -> enable");

        // Operational mode must be IDLE
        DT_ASSERT(pDf->m_OperationalMode == DT_FUNC_OPMODE_IDLE);
        //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Start in IDLE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
        // Set operational mode of S2DEC to IDLE
        DT_RETURN_ON_ERROR(DtBcS2DEC_2132_SetOperationalMode(pDf->m_pBcS2Dec,
                                                                   DT_BLOCK_OPMODE_IDLE));
        // Set operational mode of S2DEMOD to IDLE
        DT_RETURN_ON_ERROR(DtBcS2DEMOD_2132_SetOperationalMode(pDf->m_pBcS2Demod,
                                                                   DT_BLOCK_OPMODE_IDLE));
    }
    return DT_STATUS_OK;
}
//.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfS2CrDemod_2132_OnEnablePreChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtDfS2CrDemod_2132* pDf = (DtDfS2CrDemod_2132*) pDfBase;
    // Sanity check
    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);
    if (!Enable)
    {
        // ENABLE -> DISABLE
        DtDbgOutDf(AVG, S2CRDEMOD_2132, pDf,  "Function enable from enable -> disable");
        // Return to IDLE 
        DT_RETURN_ON_ERROR(DtDfS2CrDemod_2132_SetOperationalMode(pDf, 
                                                                    DT_FUNC_OPMODE_IDLE));
    }
    return DT_STATUS_OK;
}
 
//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfS2CrDemod_2132_OpenChildren(DtDfS2CrDemod_2132*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

     // List of children supported by the the S2CRDEMOD function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_S2DEC_2132, DT_BC_S2DEC_2132_NAME, 
                            S2DEC_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcS2Dec), TRUE },
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_S2DEMOD_2132, DT_BC_S2DEMOD_2132_NAME,
                        S2DEMOD_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcS2Demod), TRUE },
    };

    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // All children must have been loaded
    DT_ASSERT(pDf->m_pBcS2Dec != NULL);
    DT_ASSERT(pDf->m_pBcS2Demod != NULL);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_PeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfS2CrDemod_2132_PeriodicIntervalHandler(DtObject* pObj, DtTodTime  Time)
{
    DtDfS2CrDemod_2132*  pDf = (DtDfS2CrDemod_2132*)pObj;
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfS2CrDemod_2132));

    // Only when enabled
    if (!DtDf_IsEnabled((DtDf*)pDf))
        return;

    // Update roll-off
    DtDfS2CrDemod_2132_RollOffUpdate(pDf);
 
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132_RollOffUpdate -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Called from DPC
//
void DtDfS2CrDemod_2132_RollOffUpdate(DtDfS2CrDemod_2132*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    Int DecRollOff, DemRollOff;

    DT_ASSERT(pDf != NULL);
    
    // Prevent mutual access to operational mode
    DtSpinLockAcquireAtDpc(&pDf->m_OpModeSpinLock);

    if (pDf->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
    {
        DtSpinLockReleaseFromDpc(&pDf->m_OpModeSpinLock);
        return;
    }

    // Get the S2Dec roll-off 
    Status = DtBcS2DEC_2132_GetRollOff(pDf->m_pBcS2Dec, &DecRollOff);

    if (DT_SUCCESS(Status) && DecRollOff!=DT_BC_S2DEC_2132_ROLL_OFF_UNKNOWN)
    {
        // Convert S2Decoder roll-off to S2Demod type
        switch (DecRollOff)
        {
        case DT_BC_S2DEC_2132_ROLL_OFF_0_05:
            DemRollOff = DT_BC_S2DEMOD_2132_ROLL_OFF_0_05;
            break;
        case DT_BC_S2DEC_2132_ROLL_OFF_0_10:
            DemRollOff = DT_BC_S2DEMOD_2132_ROLL_OFF_0_10;
            break;
        case DT_BC_S2DEC_2132_ROLL_OFF_0_15:
            DemRollOff = DT_BC_S2DEMOD_2132_ROLL_OFF_0_15;
            break;
        case DT_BC_S2DEC_2132_ROLL_OFF_0_20:
            DemRollOff = DT_BC_S2DEMOD_2132_ROLL_OFF_0_20;
            break;
        case DT_BC_S2DEC_2132_ROLL_OFF_0_25:
            DemRollOff = DT_BC_S2DEMOD_2132_ROLL_OFF_0_25;
            break;
        case DT_BC_S2DEC_2132_ROLL_OFF_0_35:
            DemRollOff = DT_BC_S2DEMOD_2132_ROLL_OFF_0_35;
            break;
        default:
            DT_ASSERT(FALSE);
            DtSpinLockReleaseFromDpc(&pDf->m_OpModeSpinLock);
            return;
        }
        // Update S2-demodulator roll-off
        Status = DtBcS2DEMOD_2132_SetRollOff(pDf->m_pBcS2Demod, DemRollOff);
        DT_ASSERT(DT_SUCCESS(Status));
    }

    DtSpinLockReleaseFromDpc(&pDf->m_OpModeSpinLock);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfS2CrDemod_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtDf member to DtDfS2CrDemod_2132
#define STUB_S2CRDEMOD   ((DtIoStubDfS2CrDemod_2132*)pStub)
#define STUB_DF  ((DtDfS2CrDemod_2132*)STUB_S2CRDEMOD->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtIoStubDfS2CrDemod_OnCmd(const DtIoStub* pStub, 
                                             DtIoStubIoParams* pIoParams, Int * pOutSize);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdDemodReset(const DtIoStubDfS2CrDemod_2132*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdGetCounters( const DtIoStubDfS2CrDemod_2132*,
                                              DtIoctlS2CrDemodCmd_2132GetCountersOutput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdGetFreqOffset( const DtIoStubDfS2CrDemod_2132*,
                                            DtIoctlS2CrDemodCmd_2132GetFreqOffsetOutput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdGetOperationalMode(
               const DtIoStubDfS2CrDemod_2132*, DtIoctlS2CrDemodCmd_2132GetOpModeOutput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdGetPlInformation(
        const DtIoStubDfS2CrDemod_2132*, DtIoctlS2CrDemodCmd_2132GetPlInformationOutput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdGetRollOff(const DtIoStubDfS2CrDemod_2132*, 
                                               DtIoctlS2CrDemodCmd_2132GetRollOffOutput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdGetSnrInfo(const DtIoStubDfS2CrDemod_2132*,
                                               DtIoctlS2CrDemodCmd_2132GetSnrInfoOutput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdGetSpectrumInv(const DtIoStubDfS2CrDemod_2132*, 
                                           DtIoctlS2CrDemodCmd_2132GetSpectrumInvOutput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdGetStatus(const DtIoStubDfS2CrDemod_2132*,
                                                DtIoctlS2CrDemodCmd_2132GetStatusOutput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdGetSymbolRate(const DtIoStubDfS2CrDemod_2132*,
                                            DtIoctlS2CrDemodCmd_2132GetSymbolRateOutput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdSetOperationalMode(
          const DtIoStubDfS2CrDemod_2132*, const DtIoctlS2CrDemodCmd_2132SetOpModeInput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdSetPlInformation(
  const DtIoStubDfS2CrDemod_2132*,  const DtIoctlS2CrDemodCmd_2132SetPlInformationInput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdSetSpectrumInv(const DtIoStubDfS2CrDemod_2132*,
                                      const DtIoctlS2CrDemodCmd_2132SetSpectrumInvInput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdSetSymbolRate(const DtIoStubDfS2CrDemod_2132*,
                                       const DtIoctlS2CrDemodCmd_2132SetSymbolRateInput*);
static DtStatus  DtIoStubDfS2CrDemod_OnCmdSoftReset(const DtIoStubDfS2CrDemod_2132*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132;

static const DtIoctlProperties  IOSTUB_DF_S2CRDEMOD_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubDf_OnExclAccessCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_S2CRDEMOD_CMD_2132(
        DtIoStubDfS2CrDemod_OnCmd, 
        NULL, NULL),
};

//+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfS2CrDemod_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubDfS2CrDemod_2132_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubDfS2CrDemod_2132*  DtIoStubDfS2CrDemod_2132_Open(DtDf*  pDf)
{
    DtIoStubDfS2CrDemod_2132*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_S2CRDEMOD_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfS2CrDemod_2132, pDf, NULL, 
                                                           DtIoStubDfS2CrDemod_2132_Close,
                                                           NULL,  // Use default IOCTL
                                                           IOSTUB_DF_S2CRDEMOD_IOCTLS);
    pStub = (DtIoStubDfS2CrDemod_2132*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfS2CrDemod_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfS2CrDemod_OnCmd(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlS2CrDemodCmdInput_2132*  pInData = NULL;
    DtIoctlS2CrDemodCmdOutput_2132 *  pOutData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_S2CRDEMOD_CMD_2132);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, S2CRDEMOD_2132, pStub, 
                                                   "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_S2CrDemodCmd_2132;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_S2CrDemodCmd_2132;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_S2CRDEMOD_CMD_2132_DEMOD_RESET:
        Status = DtIoStubDfS2CrDemod_OnCmdDemodReset(STUB_S2CRDEMOD);
        break;
    case DT_S2CRDEMOD_CMD_2132_GET_COUNTERS:
        Status = DtIoStubDfS2CrDemod_OnCmdGetCounters(STUB_S2CRDEMOD, 
                                                                  &pOutData->m_GetCounters);
        break;
    case DT_S2CRDEMOD_CMD_2132_GET_FREQ_OFFSET:
        Status = DtIoStubDfS2CrDemod_OnCmdGetFreqOffset(STUB_S2CRDEMOD, 
                                                              &pOutData->m_GetFreqOffset);
        break;
    case DT_S2CRDEMOD_CMD_2132_GET_OPERATIONAL_MODE:
        Status = DtIoStubDfS2CrDemod_OnCmdGetOperationalMode(STUB_S2CRDEMOD, 
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_S2CRDEMOD_CMD_2132_GET_ROLL_OFF:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfS2CrDemod_OnCmdGetRollOff(STUB_S2CRDEMOD, 
                                                                 &pOutData->m_GetRollOff);
        break;
    case DT_S2CRDEMOD_CMD_2132_GET_SNR_INFO:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfS2CrDemod_OnCmdGetSnrInfo(STUB_S2CRDEMOD, 
                                                                 &pOutData->m_GetSnrInfo);
        break;
    case DT_S2CRDEMOD_CMD_2132_GET_SPECTRUM_INV:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfS2CrDemod_OnCmdGetSpectrumInv(STUB_S2CRDEMOD,
                                                             &pOutData->m_GetSpectrumInv);
        break;
    case DT_S2CRDEMOD_CMD_2132_GET_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfS2CrDemod_OnCmdGetStatus(STUB_S2CRDEMOD,
                                                                  &pOutData->m_GetStatus);
        break;
    case DT_S2CRDEMOD_CMD_2132_GET_SYMBOL_RATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfS2CrDemod_OnCmdGetSymbolRate(STUB_S2CRDEMOD,
                                                                 &pOutData->m_GetSymRate);
        break;
    case DT_S2CRDEMOD_CMD_2132_SET_OPERATIONAL_MODE:
        Status = DtIoStubDfS2CrDemod_OnCmdSetOperationalMode(STUB_S2CRDEMOD, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_S2CRDEMOD_CMD_2132_SET_SPECTRUM_INV:
        Status = DtIoStubDfS2CrDemod_OnCmdSetSpectrumInv(STUB_S2CRDEMOD,
                                                              &pInData->m_SetSpectrumInv);
        break;
    case DT_S2CRDEMOD_CMD_2132_SET_SYMBOL_RATE:
        Status = DtIoStubDfS2CrDemod_OnCmdSetSymbolRate(STUB_S2CRDEMOD,
                                                                  &pInData->m_SetSymRate);
        break;
    case DT_S2CRDEMOD_CMD_2132_SOFT_RESET:
        Status = DtIoStubDfS2CrDemod_OnCmdSoftReset(STUB_S2CRDEMOD);
        break;
    case DT_S2CRDEMOD_CMD_2132_GET_PL_INFORMATION:
        Status = DtIoStubDfS2CrDemod_OnCmdGetPlInformation(STUB_S2CRDEMOD, 
                                                                  &pOutData->m_GetPlInfo);
        break;
     case DT_S2CRDEMOD_CMD_2132_SET_PL_INFORMATION:
        Status = DtIoStubDfS2CrDemod_OnCmdSetPlInformation(STUB_S2CRDEMOD, 
                                                                   &pInData->m_SetPlInfo);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdDemodReset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfS2CrDemod_OnCmdDemodReset(const DtIoStubDfS2CrDemod_2132* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    return DtDfS2CrDemod_2132_DemodReset(STUB_DF);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdGetCounters -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfS2CrDemod_OnCmdGetCounters(
    const DtIoStubDfS2CrDemod_2132* pStub, 
    DtIoctlS2CrDemodCmd_2132GetCountersOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pOutData != NULL);

    return DtDfS2CrDemod_2132_GetCounters(STUB_DF, &pOutData->m_FrameCount, 
                          &pOutData->m_FrameSkipCount, &pOutData->m_LdcpSuccessCount,
                          &pOutData->m_LdcpIterationsCount, &pOutData->m_BchSuccessCount);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdGetFreqOffset -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfS2CrDemod_OnCmdGetFreqOffset(
    const DtIoStubDfS2CrDemod_2132* pStub, 
    DtIoctlS2CrDemodCmd_2132GetFreqOffsetOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pOutData != NULL);

    return DtDfS2CrDemod_2132_GetFreqOffset(STUB_DF, &pOutData->m_CoarseFreq, 
                                      &pOutData->m_FineFreq, &pOutData->m_DecimationRate);
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfS2CrDemod_OnCmdGetOperationalMode(
    const DtIoStubDfS2CrDemod_2132* pStub,
    DtIoctlS2CrDemodCmd_2132GetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pOutData != NULL);

    return DtDfS2CrDemod_2132_GetOperationalMode(STUB_DF, &pOutData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdGetPlInformation -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfS2CrDemod_OnCmdGetPlInformation(
    const DtIoStubDfS2CrDemod_2132* pStub,
    DtIoctlS2CrDemodCmd_2132GetPlInformationOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pOutData != NULL);
    return DtDfS2CrDemod_2132_GetPlInformation(STUB_DF, &pOutData->m_PlMode, 
                                                                   &pOutData->m_PlsValue);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdGetRollOff -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfS2CrDemod_OnCmdGetRollOff(
    const DtIoStubDfS2CrDemod_2132* pStub,
    DtIoctlS2CrDemodCmd_2132GetRollOffOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pOutData != NULL);

    return DtDfS2CrDemod_2132_GetRollOff(STUB_DF, &pOutData->m_RollOff);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdGetSnrInfo -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfS2CrDemod_OnCmdGetSnrInfo(
    const DtIoStubDfS2CrDemod_2132* pStub,
    DtIoctlS2CrDemodCmd_2132GetSnrInfoOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pOutData != NULL);
    return DtDfS2CrDemod_2132_GetSnrInfo(STUB_DF, &pOutData->m_SignalPower, 
                                                                 &pOutData->m_ErrorPower);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdGetSpectrumInv -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfS2CrDemod_OnCmdGetSpectrumInv(
    const DtIoStubDfS2CrDemod_2132* pStub,
    DtIoctlS2CrDemodCmd_2132GetSpectrumInvOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pOutData != NULL);
    return DtDfS2CrDemod_2132_GetSpectrumInv(STUB_DF, &pOutData->m_SpectrumInv);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdGetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfS2CrDemod_OnCmdGetStatus(
    const DtIoStubDfS2CrDemod_2132* pStub,
    DtIoctlS2CrDemodCmd_2132GetStatusOutput* pOutData)
{
    DtStatus Status= DT_STATUS_OK;
    Bool SigDetect, SpecInv, PlHdrSync;
    UInt ResyncCount;    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pOutData != NULL);
    Status =  DtDfS2CrDemod_2132_GetStatus(STUB_DF, &SigDetect, &SpecInv, &PlHdrSync, 
                                                                            &ResyncCount);
    pOutData->m_SignalDetected = (Int)SigDetect;
    pOutData->m_SpectrumInverted = (Int)SpecInv;
    pOutData->m_PlHeaderSync = (Int)PlHdrSync;
    pOutData->m_PlHeaderResyncCount = ResyncCount;

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdGetSymbolRate -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfS2CrDemod_OnCmdGetSymbolRate(
    const DtIoStubDfS2CrDemod_2132* pStub,
    DtIoctlS2CrDemodCmd_2132GetSymbolRateOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pOutData != NULL);
    return DtDfS2CrDemod_2132_GetSymbolRate(STUB_DF, &pOutData->m_DecRate,
                                       &pOutData->m_SamSymRatio, &pOutData->m_SymSamRatio,
                                       &pOutData->m_MeasSamSymRatio);
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfS2CrDemod_OnCmdSetOperationalMode(
    const DtIoStubDfS2CrDemod_2132* pStub,
    const DtIoctlS2CrDemodCmd_2132SetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pInData != NULL);
    return DtDfS2CrDemod_2132_SetOperationalMode(STUB_DF, pInData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdSetPlInformation -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfS2CrDemod_OnCmdSetPlInformation(
    const DtIoStubDfS2CrDemod_2132* pStub, 
    const DtIoctlS2CrDemodCmd_2132SetPlInformationInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pInData != NULL);
    return DtDfS2CrDemod_2132_SetPlInformation(STUB_DF, pInData->m_PlMode, 
                                                                     pInData->m_PlsValue);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdSetSpectrumInv -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfS2CrDemod_OnCmdSetSpectrumInv(
    const DtIoStubDfS2CrDemod_2132* pStub,
    const DtIoctlS2CrDemodCmd_2132SetSpectrumInvInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pInData != NULL);
    return DtDfS2CrDemod_2132_SetSpectrumInv(STUB_DF, pInData->m_SpectrumInv);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdSetSymbolRate -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfS2CrDemod_OnCmdSetSymbolRate(
    const DtIoStubDfS2CrDemod_2132* pStub,
    const DtIoctlS2CrDemodCmd_2132SetSymbolRateInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    DT_ASSERT(pInData != NULL);
    return DtDfS2CrDemod_2132_SetSymbolRate(STUB_DF, pInData->m_DecRate, 
                                          pInData->m_SamSymRatio, pInData->m_SymSamRatio);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_OnCmdSoftReset -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfS2CrDemod_OnCmdSoftReset(
    const DtIoStubDfS2CrDemod_2132* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfS2CrDemod_2132));
    return DtDfS2CrDemod_2132_SoftReset(STUB_DF);
}
