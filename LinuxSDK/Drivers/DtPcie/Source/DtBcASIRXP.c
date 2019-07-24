//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcASIRXP.c *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtDf.h"
#include "DtBcASIRXP.h"
#include "DtBcASIRXP_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcASIRXP implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_ASIRXP_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcASIRXP))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_ASIRXP_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(ASIRXP, pBc)

// Helper macro to cast a DtBc* to a DtBcASIRXP*
#define BC_ASIRXP         ((DtBcASIRXP*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcASIRXP_Init(DtBc*);
static DtStatus  DtBcASIRXP_OnEnable(DtBc*, Bool  Enable);
static void  DtBcASIRXP_SetControlRegs(DtBcASIRXP*, Bool BlkEna, Int OpMode,
                                                                            Int Polarity);
static void  DtBcASIRXP_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);
static void  DtBcASIRXP_BitrateAddSample(DtBcASIRXP*, DtBcASIRXPBitrateMeasure*, 
                                                                          UInt TsWordCnt);
static Int  DtBcASIRXP_BitrateGet(DtBcASIRXPBitrateMeasure*  pBrm);
static void  DtBcASIRXP_BitrateInit(DtBcASIRXPBitrateMeasure*,  Int PeriodicItv);
static void  DtBcASIRXP_CarrierDetectInit(DtBcASIRXP* pBc);
static void  DtBcASIRXP_CarrierDetectUpdate(DtBcASIRXP* pBc);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcASIRXP - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcASIRXP_Close(DtBc*  pBc)
{
    BC_ASIRXP_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    //Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pBc->m_pCore, (DtObject*)pBc);
    
    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcASIRXP*  DtBcASIRXP_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_ASIRXP_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcASIRXP, Id, DT_BLOCK_TYPE_ASIRXP, Address,
                                                                       pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcASIRXP_Close;
    OpenParams.m_InitFunc = DtBcASIRXP_Init;
    OpenParams.m_OnEnableFunc = DtBcASIRXP_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcASIRXP*)DtBc_Open(&OpenParams);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcASIRXP_GetOperationalMode(DtBcASIRXP* pBc, Int* pOpMode)
{
    // Sanity check
    BC_ASIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ASIRXP_MUST_BE_ENABLED(pBc);

    // Return cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_GetPolarityCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcASIRXP_GetPolarityCtrl(DtBcASIRXP* pBc, Int* pPolarity)
{
    // Sanity check
    BC_ASIRXP_DEFAULT_PRECONDITIONS(pBc);


    // Check parameters
    if (pPolarity == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ASIRXP_MUST_BE_ENABLED(pBc);
    
    // Return last cached polarity
    *pPolarity = pBc->m_AsiPolarityCtrl;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcASIRXP_GetStatus(DtBcASIRXP* pBc, Bool* pCarrierDet, Bool* pAsiLock)
{
    UInt  FwAsiLock;

    // Sanity check
    BC_ASIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pCarrierDet==NULL || pAsiLock==NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_ASIRXP_MUST_BE_ENABLED(pBc);

    // Must be in RUN
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
    {
        DtDbgOutBc(ERR, ASIRXP, pBc, "Block not in RUN");
        return DT_STATUS_INVALID_IN_OPMODE;
    }
    // Get current ASI lock status from firmware
    FwAsiLock = ASIRXP_Status_READ_AsiLock(pBc);
    // The returned ASI-lock is false if there is not carrier detected
    *pAsiLock = (FwAsiLock==ASIRXP_ASILOCK_LOCK && pBc->m_CarrierDet);

    // Return the carrier detect status; which was determined on last periodic interrupt
    *pCarrierDet = pBc->m_CarrierDet;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_GetSymInCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcASIRXP_GetSymInCount(DtBcASIRXP* pBc, UInt* pSymInCount)
{
    // Sanity check
    BC_ASIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pSymInCount == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_ASIRXP_MUST_BE_ENABLED(pBc);

    // Must be in RUN
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
    {
        DtDbgOutBc(ERR, ASIRXP, pBc, "Block not in RUN");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // Get input symbol counter
    *pSymInCount = ASIRXP_SymInCnt_READ(pBc);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_GetTsBitrate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcASIRXP_GetTsBitrate(DtBcASIRXP* pBc, Int* pBitrate)
{
    // Sanity check
    BC_ASIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pBitrate == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_ASIRXP_MUST_BE_ENABLED(pBc);

    // Must be in RUN
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
    {
        DtDbgOutBc(ERR, ASIRXP, pBc, "Block not in RUN");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // Compute and get bitrate
    *pBitrate = DtBcASIRXP_BitrateGet(&pBc->m_BitrateMeasure);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_GetTsWordCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcASIRXP_GetTsWordCount(DtBcASIRXP* pBc, UInt* pTsWordCount)
{
    // Sanity check
    BC_ASIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pTsWordCount == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_ASIRXP_MUST_BE_ENABLED(pBc);

    // Must be in RUN
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
    {
        DtDbgOutBc(ERR, ASIRXP, pBc, "Block not in RUN");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // Get TS-word counter
    *pTsWordCount = ASIRXP_TsWordCnt_READ(pBc);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_GetViolCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcASIRXP_GetViolCount(DtBcASIRXP* pBc, UInt* pViolCount)
{
    // Sanity check
    BC_ASIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pViolCount == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_ASIRXP_MUST_BE_ENABLED(pBc);

    // Must be in RUN
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
    {
        DtDbgOutBc(ERR, ASIRXP, pBc, "Block not in RUN");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // Get violation counter
    *pViolCount = ASIRXP_CodeViolationCnt_READ(pBc);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcASIRXP_SetOperationalMode(DtBcASIRXP* pBc, Int OpMode)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_ASIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ASIRXP_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    if (OpMode == DT_BLOCK_OPMODE_IDLE)
    {
        // RUN -> IDLE
        DtBcASIRXP_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode, 
                                                                  pBc->m_AsiPolarityCtrl);
    }
    else
    {
        // IDLE -> RUN
        // Initialize bitrate estimation
        DtBcASIRXP_BitrateInit(&BC_ASIRXP->m_BitrateMeasure, 
                                                           BC_ASIRXP->m_PeriodicInterval);

        DtBcASIRXP_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode, 
                                                                  pBc->m_AsiPolarityCtrl);
    }

    if (DT_SUCCESS(Status))
        // Save new settings
        pBc->m_OperationalMode = OpMode;

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_SetPolarityCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcASIRXP_SetPolarityCtrl(DtBcASIRXP* pBc, Int Polarity)
{
    // Sanity check
    BC_ASIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check and convert parameters
    if (Polarity!=DT_BC_ASIRXP_POL_NORMAL && Polarity!=DT_BC_ASIRXP_POL_INVERT)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_ASIRXP_MUST_BE_ENABLED(pBc);

    // Set ASI polarity
    DtBcASIRXP_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, Polarity);

    // Update cached polarity
    pBc->m_AsiPolarityCtrl = Polarity;

    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcASIRXP - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcASIRXP_Init(DtBc*  pBc)
{
    DtOnPeriodicIntervalRegData  RegData;
    DtStatus  Status=DT_STATUS_OK;
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcASIRXP));


    // Initialize spinlock
    DtSpinLockInit(&BC_ASIRXP->m_BitrateMeasure.m_BrmSpinLock);

    // Initialize the bitrate measurement 
    Status = DtCore_TOD_GetPeriodicItv(pBc->m_pCore, &BC_ASIRXP->m_PeriodicInterval);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, ASIRXP, pBc, "ERROR: DtCore_TOD_GetPeriodicItv failed");
        return Status;
    } 
    DtBcASIRXP_BitrateInit(&BC_ASIRXP->m_BitrateMeasure, BC_ASIRXP->m_PeriodicInterval);

    // Initialize the carrier detection
    DtBcASIRXP_CarrierDetectInit(BC_ASIRXP);

    // Set defaults;
    BC_ASIRXP->m_BlockEnabled = FALSE;
    BC_ASIRXP->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    BC_ASIRXP->m_AsiPolarityCtrl = DT_BC_ASIRXP_POL_NORMAL;

    DtBcASIRXP_SetControlRegs(BC_ASIRXP, BC_ASIRXP->m_BlockEnabled, 
                              BC_ASIRXP->m_OperationalMode, BC_ASIRXP->m_AsiPolarityCtrl);

     // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtBcASIRXP_PeriodicIntervalHandler;
    RegData.m_pObject = (DtObject*)pBc;
    Status = DtCore_TOD_PeriodicItvRegister(pBc->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    { 
        DtDbgOutBc(ERR, ASIRXP, pBc, "DtCore_TOD_PeriodicItvRegister failed");
        return Status;
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcASIRXP_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcASIRXP* pBc = (DtBcASIRXP*)pBcBase;

    // Sanity check
    BC_ASIRXP_DEFAULT_PRECONDITIONS(pBc);


    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, ASIRXP, pBc, "Block Enable from disable -> enable");
        
        // Re-initialize the bitrate measurement
        DtBcASIRXP_BitrateInit(&pBc->m_BitrateMeasure, pBc->m_PeriodicInterval);
        // Re-initialize the carrier detection
        DtBcASIRXP_CarrierDetectInit(pBc);
        
        // Enable block
        DtBcASIRXP_SetControlRegs(pBc, Enable, pBc->m_OperationalMode, 
                                                                  pBc->m_AsiPolarityCtrl);
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, ASIRXP, pBc, "Block Enable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcASIRXP_SetOperationalMode(BC_ASIRXP, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, ASIRXP, pBc, "ERROR: DtBcASIRXP_SetOperationalMode failed");
            return Status;
        }

        // Disable block
        DtBcASIRXP_SetControlRegs(pBc, Enable, pBc->m_OperationalMode, 
                                                                  pBc->m_AsiPolarityCtrl);
    }

    // Save new setting
    pBc->m_BlockEnabled = Enable;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcASIRXP_SetControlRegs(DtBcASIRXP* pBc, Bool BlkEna, Int OpMode, Int Polarity)
{
    UInt32 RegData=0, FwBlkEna=0, FwOpMode=0, FwPolarity=0;


    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? ASIRXP_BLKENA_ENABLED : ASIRXP_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = ASIRXP_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = ASIRXP_OPMODE_RUN; break;
    default:                      DT_ASSERT(FALSE); break;
    }
    // Convert polarity to BB-type
    switch (Polarity)
    {
    case DT_BC_ASIRXP_POL_NORMAL:  FwPolarity = ASIRXP_POL_NORMAL; break;
    case DT_BC_ASIRXP_POL_INVERT:  FwPolarity = ASIRXP_POL_INVERT; break;
    default:                       DT_ASSERT(FALSE); break;
    }

    // Update control
    RegData = ASIRXP_Control_READ(pBc);
    RegData = ASIRXP_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = ASIRXP_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = ASIRXP_Control_SET_AsiPolarity(RegData, FwPolarity);
    ASIRXP_Control_WRITE(pBc, RegData);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_PeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcASIRXP_PeriodicIntervalHandler(DtObject* pObj, DtTodTime  Time)
{
    UInt TsWordCnt;
    DtBcASIRXP*  pBc = (DtBcASIRXP*)pObj;
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcASIRXP));

    // Must be enabled and in RUN
    if (!DtBc_IsEnabled((DtBc*)pBc))
        return;
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return;

    // Get new TS-word count sample
    TsWordCnt =  ASIRXP_TsWordCnt_READ(pBc);

    // Update bitrate measurement
    DtBcASIRXP_BitrateAddSample(pBc, &pBc->m_BitrateMeasure, TsWordCnt);

    // Update carrier detect
    DtBcASIRXP_CarrierDetectUpdate(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_BitrateAddSample -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Updates bitrate adminstration with a new sample; called on periodic interrrupt
//
void  DtBcASIRXP_BitrateAddSample(DtBcASIRXP*  pBc, DtBcASIRXPBitrateMeasure*  pBrm,
                                                                           UInt TsWordCnt)
{
    Int  NewIndex, TsWordCntDiff, CntDiffDiff;
    
    DtSpinLockAcquireAtDpc(&pBrm->m_BrmSpinLock);

    // Check whether last TS-word count is valid
    if (pBrm->m_NumValidSamples < 0)
    {
        pBrm->m_LastTsWordCnt = TsWordCnt;
        pBrm->m_NumValidSamples = 0;
        DtSpinLockReleaseFromDpc(&pBrm->m_BrmSpinLock);
        return;
    }

    // Compute difference TS-word count 
    TsWordCntDiff = TsWordCnt - pBrm->m_LastTsWordCnt;    
    pBrm->m_LastTsWordCnt = TsWordCnt;

    // Compute index of location where to store new sample, with wrap-around
    // this is the same as the index of the eldest sample
    NewIndex = pBrm->m_TsWordCntIndex + 1;
    if (NewIndex >= DT_BC_ASIRXP_NUM_SAMPLES_TOTAL)
        NewIndex = 0;

    // If buffer is full => subtract eldest element
    if (pBrm->m_NumValidSamples == DT_BC_ASIRXP_NUM_SAMPLES_TOTAL)
        pBrm->m_TsWordCntTotal -= pBrm->m_TsWordCntDiff[NewIndex];
    else // Buffer not full => we have an extra sample
        pBrm->m_NumValidSamples++;
  
    // Process the TS-word-counter differential value
    pBrm->m_TsWordCntDiff[NewIndex] = TsWordCntDiff;
    pBrm->m_TsWordCntTotal += TsWordCntDiff;

    // Next time, index is current new index
    pBrm->m_TsWordCntIndex = NewIndex;

    // Integrate the  difference between current and average counts
    // In case of a constant bitrate it is 0. In case of constant bitrate
    // with burst it is less than the burst size.

    // 1) Forget the past a bit. Such that small bitrate changes or roundings errors
    //    don't result in a bitrate reset.
    if (pBrm->m_TsWordDifDifCnt < 0)
        pBrm->m_TsWordDifDifCnt++;
    else if (pBrm->m_TsWordDifDifCnt > 0)
        pBrm->m_TsWordDifDifCnt--;

    // 2) Integrate the  difference between current and average counts
    CntDiffDiff = TsWordCntDiff - (pBrm->m_TsWordCntTotal + pBrm->m_NumValidSamples/2)
                                                                / pBrm->m_NumValidSamples;
    pBrm->m_TsWordDifDifCnt += CntDiffDiff;

    // 3) The sum of diference should be less than the burst size, otherwise reset bitrate
    //    This detects a bitrate change of 20kbps in 100ms, a change of 2kbps in 1 second
    if (pBrm->m_TsWordDifDifCnt<-DT_BC_ASIRXP_MAX_BURST 
                                        || pBrm->m_TsWordDifDifCnt>DT_BC_ASIRXP_MAX_BURST)
    {
        DtDbgOutBc(AVG, ASIRXP, pBc, "Bitrate reset due to big sample deviation."
                                       "Difference: %d NumValidSamples: %d", 
                                        pBrm->m_TsWordDifDifCnt, pBrm->m_NumValidSamples);
        // Start all over again using last sample
        pBrm->m_TsWordCntTotal = TsWordCntDiff;
        pBrm->m_NumValidSamples = 1;
        pBrm->m_TsWordDifDifCnt = 0;
    }
   

    DtSpinLockReleaseFromDpc(&pBrm->m_BrmSpinLock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_BitrateGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtBcASIRXP_BitrateGet(DtBcASIRXPBitrateMeasure*  pBrm)
{
    volatile UInt64 Bitrate, TsWordCntTotal, NumValidSamples, PeriodicItvMs;

    // Copy relevant data under spinlock protection
    DtSpinLockAcquire(&pBrm->m_BrmSpinLock);
    TsWordCntTotal = pBrm->m_TsWordCntTotal;
    NumValidSamples = pBrm->m_NumValidSamples;
    PeriodicItvMs =  pBrm->m_PeriodicItvMs;
    DtSpinLockRelease(&pBrm->m_BrmSpinLock);

    // Prevent divide by zero
    if (NumValidSamples<=0 || PeriodicItvMs<=0)
        Bitrate =0;
    else
        // Compute estimate TS-bitrate
        // Bitrate = (TotalTsWordCount * 8 bit) / (NumberValidSamples * PeriodicInterval)
        Bitrate = DtDivide64(TsWordCntTotal * 8LL * 1000,
                                                   NumValidSamples * PeriodicItvMs, NULL);
    return (Int)Bitrate;
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_InitBitrate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Initializes the bitrate administration
//
void  DtBcASIRXP_BitrateInit(DtBcASIRXPBitrateMeasure*  pBrm,  Int PeriodicItvMs)
{
    DT_ASSERT(PeriodicItvMs > 0);
     // Prevent mutual access to polarity control data
    DtSpinLockAcquire(&pBrm->m_BrmSpinLock);
    pBrm->m_TsWordCntIndex = 0;
    pBrm->m_NumValidSamples = -1;
    pBrm->m_LastTsWordCnt = 0;
    pBrm->m_TsWordCntTotal = 0;
    pBrm->m_TsWordDifDifCnt = 0;
    pBrm->m_EstTsBitrate = 0;
    pBrm->m_PeriodicItvMs = PeriodicItvMs;
    DtSpinLockRelease(&pBrm->m_BrmSpinLock);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_CarrierDetectInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initializes carrier detect status.
//
void  DtBcASIRXP_CarrierDetectInit(DtBcASIRXP* pBc)
{
    pBc->m_PrevSymInCnt =  ASIRXP_SymInCnt_READ(pBc);
    pBc->m_CarrierDet = FALSE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP_CarrierDetectUpdate -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Updates carrier detect status; called on periodic interrrupt
//
void  DtBcASIRXP_CarrierDetectUpdate(DtBcASIRXP* pBc)
{

    // Carrier Detect is derived from the input-symbol count, 
    // if no new symbos have been received during the timer interrupt period
    // Carrier Detect becomes false; otherwise true.
    UInt  SymInCnt;
    SymInCnt = ASIRXP_SymInCnt_READ(pBc);
    pBc->m_CarrierDet = (SymInCnt != pBc->m_PrevSymInCnt);
    pBc->m_PrevSymInCnt = SymInCnt;
}