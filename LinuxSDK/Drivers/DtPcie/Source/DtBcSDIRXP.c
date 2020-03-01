//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIRXP.c *#*#*#*#*#*#*#*# (C) 2017-2018 DekTec
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
#include "DtBcSDIRXP.h"
#include "DtBcSDIRXP_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXP implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SDIRXP_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDIRXP))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SDIRXP_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SDIRXP, pBc)

// Helper macro to cast a DtBc* to a DtBcSDIRXP*
#define BC_SDIRXP         ((DtBcSDIRXP*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSDIRXP_Init(DtBc*);
static DtStatus  DtBcSDIRXP_OnEnable(DtBc*, Bool);
static DtStatus  DtBcSDIRXP_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcSDIRXP_CheckSdiRate(DtBcSDIRXP*, Int SdiRate);
static void  DtBcSDIRXP_SetControlRegs(DtBcSDIRXP*, Bool BlkEna, Int OpMode, Int SdiRate);
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXP - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSDIRXP_Close(DtBc*  pBc)
{
    BC_SDIRXP_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcSDIRXP*  DtBcSDIRXP_Open(Int  Address, DtCore*  pCore, DtPt*  pPt, DtBcType  Type,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(Type== DT_BLOCK_TYPE_SDIRXP);
    
    // Init open parameters
    DT_BC_SDIRXP_INIT_ID(Id, Type, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSDIRXP, Id, Type, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSDIRXP_Close;
    OpenParams.m_InitFunc = DtBcSDIRXP_Init;
    OpenParams.m_OnEnableFunc = DtBcSDIRXP_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcSDIRXP_OnCloseFile;
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSDIRXP*)DtBc_Open(&OpenParams);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_GetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXP_GetMaxSdiRate(DtBcSDIRXP* pBc, Int * pMaxSdiRate)
{
    // Sanity check
    BC_SDIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pMaxSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXP_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pMaxSdiRate = pBc->m_MaxSdiRate;

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXP_GetOperationalMode(DtBcSDIRXP* pBc, Int * pOpMode)
{
    // Sanity check
    BC_SDIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXP_MUST_BE_ENABLED(pBc);

    // Return cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_GetSdiLineLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXP_GetSdiLineLock(DtBcSDIRXP* pBc, Bool* pLineLock)
{
    // Sanity check
    BC_SDIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pLineLock == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXP_MUST_BE_ENABLED(pBc);

    // Get SDI-line-lock
    *pLineLock = SDIRXP_Status_READ_LineLock(pBc) != 0;

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_GetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXP_GetSdiRate(DtBcSDIRXP* pBc, Int* pSdiRate)
{
    // Sanity check
    BC_SDIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXP_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pSdiRate = pBc->m_SdiRate;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_GetSdiStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXP_GetSdiStatus(DtBcSDIRXP* pBc, Int* pSdiLock, Int* pLineLock,
                                 Int* pValid, Int* pSdiRate, Int* pNumSymsHanc, 
                                 Int* pNumSymsVidVanc, Int* pNumLinesF1, Int* pNumLinesF2, 
                                 Int* pIsLevelB, UInt32* pPayloadId, Int* pFramePeriod)
{
    UInt32 CurrentSdiStatus, SdiStatus1, SdiStatus2, SdiStatus3, SdiStatus4;

    // Sanity check
    BC_SDIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pSdiLock==NULL || pLineLock==NULL  || pValid==NULL || pSdiRate==NULL 
                       || pNumSymsHanc==NULL || pNumSymsVidVanc==NULL || pNumLinesF1==NULL
                       || pNumLinesF2==NULL || pIsLevelB==NULL || pPayloadId==NULL 
                       || pFramePeriod==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXP_MUST_BE_ENABLED(pBc);

    // Read status registers
    SdiStatus1 = SDIRXP_SdiStatus1_READ(pBc);
    SdiStatus2 = SDIRXP_SdiStatus2_READ(pBc);
    SdiStatus3 = SDIRXP_SdiStatus3_READ(pBc);
    SdiStatus4 = SDIRXP_SdiStatus4_READ(pBc);

    // Do they have different frame counts?
    if (    SDIRXP_SdiStatus1_GET_FrameCnt(SdiStatus1)
                                              !=SDIRXP_SdiStatus2_GET_FrameCnt(SdiStatus2)
         || SDIRXP_SdiStatus1_GET_FrameCnt(SdiStatus1)
                                              !=SDIRXP_SdiStatus3_GET_FrameCnt(SdiStatus3)
         || SDIRXP_SdiStatus1_GET_FrameCnt(SdiStatus1)
                                             !=SDIRXP_SdiStatus4_GET_FrameCnt(SdiStatus4))
    {
        // Read status register again
        SdiStatus1 = SDIRXP_SdiStatus1_READ(pBc);
        SdiStatus2 = SDIRXP_SdiStatus2_READ(pBc);
        SdiStatus3 = SDIRXP_SdiStatus3_READ(pBc);
        SdiStatus4 = SDIRXP_SdiStatus4_READ(pBc);
        // Now they should have all the same frame count
        if (    SDIRXP_SdiStatus1_GET_FrameCnt(SdiStatus1)
                                              !=SDIRXP_SdiStatus2_GET_FrameCnt(SdiStatus2)
             || SDIRXP_SdiStatus1_GET_FrameCnt(SdiStatus1)
                                              !=SDIRXP_SdiStatus3_GET_FrameCnt(SdiStatus3)
             || SDIRXP_SdiStatus1_GET_FrameCnt(SdiStatus1)
                                             !=SDIRXP_SdiStatus4_GET_FrameCnt(SdiStatus4))
        {
            // Only one retry
            return DT_STATUS_TIMEOUT;
        }
    }
    // Read current SDI-status fields
    CurrentSdiStatus = SDIRXP_Status_READ(pBc);

    // Get line-lock, bit-lock and SDI-lock
    *pSdiLock = (Int)(SDIRXP_Status_GET_SdiLock(CurrentSdiStatus) == 1);
    *pLineLock = (Int) (SDIRXP_Status_GET_LineLock(CurrentSdiStatus) == 1);
    *pValid = (Int) (SDIRXP_SdiStatus1_GET_Valid(SdiStatus1) == 1);

    // Status fields only valid if SDI-lock
    if (*pValid)
    {
        Int64 SymsPerFrame;
        UInt32 SymCount;
        *pNumSymsHanc = (Int)SDIRXP_SdiStatus1_GET_NumSymHanc(SdiStatus1);
        *pNumSymsVidVanc = (Int)SDIRXP_SdiStatus1_GET_NumSymVideo(SdiStatus1);
        *pIsLevelB = (Int)(SDIRXP_SdiStatus1_GET_LevelAorB(SdiStatus1) 
                                                              == SDIRXP_SDILEVEL_LEVEL_B);
        *pSdiRate = pBc->m_SdiRate;
        *pNumLinesF1 = (Int)SDIRXP_SdiStatus2_GET_NumLinesField1(SdiStatus2);
        *pNumLinesF2 = (Int)SDIRXP_SdiStatus2_GET_NumLinesField2(SdiStatus2);
        *pPayloadId = SDIRXP_SdiStatus4_GET_PayloadIdHigh(SdiStatus4)<<16;
        *pPayloadId |= SDIRXP_SdiStatus3_GET_PayloadIdLow(SdiStatus3);

        // Compute the frame period in nanoseconds
        SymCount = SDIRXP_SymbolCnt_READ(pBc);
        SymsPerFrame = (*pNumSymsHanc + *pNumSymsVidVanc) * (*pNumLinesF1 + *pNumLinesF2);
        if (SymCount > 0)
        {
            Int64  FramePeriod = DtDivide64((SymsPerFrame *10*1000000), SymCount, NULL);
            // In 6G/12G we have a frame per link
            if (pBc->m_SdiRate==DT_DRV_SDIRATE_6G || pBc->m_SdiRate==DT_DRV_SDIRATE_12G)
                FramePeriod *= 4;
            // Check for unexpected values
            if (FramePeriod < (1LL<<31))
                *pFramePeriod = (Int)FramePeriod;
        else
            *pFramePeriod = 0;
        }
        else
            *pFramePeriod = 0;
    }
    else
    {
        *pIsLevelB  = (Int)FALSE;
        *pNumSymsHanc = *pNumSymsVidVanc = *pNumLinesF1 = *pNumLinesF2 
                                                         = *pPayloadId = *pFramePeriod= 0;
        *pSdiRate = DT_DRV_SDIRATE_UNKNOWN;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_GetCrcErrorCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXP_GetCrcErrorCount(DtBcSDIRXP* pBc, UInt* pCrcErrorCnt)
{
    // Sanity check
    BC_SDIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pCrcErrorCnt == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXP_MUST_BE_ENABLED(pBc);

    // Get CRC-error counter
    *pCrcErrorCnt = SDIRXP_CrcErrorCnt_READ(pBc);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXP_SetOperationalMode(DtBcSDIRXP* pBc, Int OpMode)
{
    DtStatus  Status=DT_STATUS_OK;
    // Sanity check
    BC_SDIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXP_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Set new control register value
    DtBcSDIRXP_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode, pBc->m_SdiRate);

    // Save new settings
    pBc->m_OperationalMode = OpMode;
    
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_SetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXP_SetSdiRate(DtBcSDIRXP* pBc, Int SdiRate)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_SDIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    DT_RETURN_ON_ERROR(DtBcSDIRXP_CheckSdiRate(pBc, SdiRate));

    // Must be enabled
    BC_SDIRXP_MUST_BE_ENABLED(pBc);
    
    // Operational mode must be IDLE
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
    {
        DtDbgOutBc(ERR, SDIRXP, pBc, "Operational mode not in idle");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // No change?
    if (pBc->m_SdiRate == SdiRate)
        return DT_STATUS_OK;

    // Set new control register value
    DtBcSDIRXP_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, SdiRate);

    // Save new setting
    pBc->m_SdiRate = SdiRate;

    return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXP - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDIRXP_Init(DtBc*  pBcBase)
{
    DtBcSDIRXP* pBc = (DtBcSDIRXP*)pBcBase;
    UInt32  FwMaxSdiRate;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDIRXP));

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_SdiRate = DT_DRV_SDIRATE_SD;
     
    // Get maximum supported rate
    FwMaxSdiRate = SDIRXP_Config_READ_MaxSdiRate(pBc);
    switch (FwMaxSdiRate)
    {
    case SDIRXP_SDIMODE_SD:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_SD; break;
    case SDIRXP_SDIMODE_HD:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_HD; break;
    case SDIRXP_SDIMODE_3G:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_3G; break;
    case SDIRXP_SDIMODE_6G:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_6G; break;
    case SDIRXP_SDIMODE_12G:    pBc->m_MaxSdiRate = DT_DRV_SDIRATE_12G; break;
    default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
    }

    DtBcSDIRXP_SetControlRegs(BC_SDIRXP, BC_SDIRXP->m_BlockEnabled, 
                                  BC_SDIRXP->m_OperationalMode, BC_SDIRXP->m_SdiRate);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXP_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSDIRXP* pBc = (DtBcSDIRXP*)pBcBase;

    // Sanity check
    BC_SDIRXP_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, SDIRXP, pBc, "Block Enable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
        pBc->m_SdiRate = DT_DRV_SDIRATE_SD;
        DtBcSDIRXP_SetControlRegs(pBc, Enable, pBc->m_OperationalMode, pBc->m_SdiRate);
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, SDIRXP, pBc, "Block Enable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcSDIRXP_SetOperationalMode(BC_SDIRXP, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDIRXP, pBc, "ERROR: DtBcSDIRXP_SetOperationalMode failed");
            DT_ASSERT(FALSE);
            return Status;
        }

        // Disable block
        DtBcSDIRXP_SetControlRegs(pBc, Enable, pBc->m_OperationalMode, pBc->m_SdiRate);
    }
    // Save new setting
    pBc->m_BlockEnabled = Enable;
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDIRXP_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_SDIRXP_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled(pBc))
    {
        DtDbgOutBc(AVG, SDIRXP, pBc, "Go to IDLE");

        // Go to IDLE
        Status = DtBcSDIRXP_SetOperationalMode(BC_SDIRXP, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDIRXP, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_CheckSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXP_CheckSdiRate(DtBcSDIRXP* pBc, Int SdiRate)
{
    // Check whether it is a valid SDI-rate
    if (   SdiRate!=DT_DRV_SDIRATE_SD && SdiRate!=DT_DRV_SDIRATE_HD 
        && SdiRate!=DT_DRV_SDIRATE_3G && SdiRate!=DT_DRV_SDIRATE_6G 
        && SdiRate!=DT_DRV_SDIRATE_12G)
        return DT_STATUS_INVALID_PARAMETER;

    // Assumption we can keep the comparison simple
    DT_ASSERT(DT_DRV_SDIRATE_SD<DT_DRV_SDIRATE_HD 
              && DT_DRV_SDIRATE_HD<DT_DRV_SDIRATE_3G
              && DT_DRV_SDIRATE_3G<DT_DRV_SDIRATE_6G
              && DT_DRV_SDIRATE_6G<DT_DRV_SDIRATE_12G);
    
    // Check whether the SDI-rate is supported
    if (SdiRate > pBc->m_MaxSdiRate)
        return DT_STATUS_NOT_SUPPORTED;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSDIRXP_SetControlRegs(DtBcSDIRXP* pBc, Bool BlkEna, Int OpMode, Int SdiRate)
{
    UInt32 FwBlkEna=0, FwOpMode=0, FwSdiRate=0, RegData=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? SDIRXP_BLKENA_ENABLED :SDIRXP_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = SDIRXP_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = SDIRXP_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = SDIRXP_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }
    // Convert SDI mode to BB-type
    switch (SdiRate)
    {
    case DT_DRV_SDIRATE_SD:  FwSdiRate = SDIRXP_SDIMODE_SD; break;
    case DT_DRV_SDIRATE_HD:  FwSdiRate = SDIRXP_SDIMODE_HD; break;
    case DT_DRV_SDIRATE_3G:  FwSdiRate = SDIRXP_SDIMODE_3G; break;
    case DT_DRV_SDIRATE_6G:  FwSdiRate = SDIRXP_SDIMODE_6G; break;
    case DT_DRV_SDIRATE_12G: FwSdiRate = SDIRXP_SDIMODE_12G; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control
    RegData = SDIRXP_Control_READ(pBc);
    RegData = SDIRXP_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = SDIRXP_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = SDIRXP_Control_SET_SdiRate(RegData, FwSdiRate);
    SDIRXP_Control_WRITE(pBc, RegData);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIRXP implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcSDIRXP
#define SDIRXP_STUB   ((DtIoStubBcSDIRXP*)pStub)
#define SDIRXP_BC  ((DtBcSDIRXP*)SDIRXP_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcSDIRXP_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus DtIoStubBcSDIRXP_OnCmdGetCrcErrorCount(const DtIoStubBcSDIRXP*, 
                                                 DtIoctlSdiRxPCmdGetCrcErrorCountOutput*);
static DtStatus DtIoStubBcSDIRXP_OnCmdGetMaxSdiRate(const DtIoStubBcSDIRXP*,
                                                    DtIoctlSdiRxPCmdGetMaxSdiRateOutput*);
static DtStatus DtIoStubBcSDIRXP_OnCmdGetOperationalMode(const DtIoStubBcSDIRXP*,
                                                        DtIoctlSdiRxPCmdGetOpModeOutput*);
static DtStatus DtIoStubBcSDIRXP_OnCmdGetSdiRate(const DtIoStubBcSDIRXP*,
                                                       DtIoctlSdiRxPCmdGetSdiRateOutput*);
static DtStatus DtIoStubBcSDIRXP_OnCmdGetSdiStatus(const DtIoStubBcSDIRXP*, 
                                                     DtIoctlSdiRxPCmdGetSdiStatusOutput*);
static DtStatus DtIoStubBcSDIRXP_OnCmdGetSdiStatus2(const DtIoStubBcSDIRXP*, 
                                                    DtIoctlSdiRxPCmdGetSdiStatusOutput2*);
static DtStatus DtIoStubBcSDIRXP_OnCmdSetOperationalMode(const DtIoStubBcSDIRXP*,
                                                 const DtIoctlSdiRxPCmdSetOpModeInput*);
static DtStatus DtIoStubBcSDIRXP_OnCmdSetSdiRate(const DtIoStubBcSDIRXP*,
                                                  const DtIoctlSdiRxPCmdSetSdiRateInput*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SDIRXP;

static const DtIoctlProperties  IOSTUB_BC_SDIRXP_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_SDIRXP_CMD(
        DtIoStubBcSDIRXP_OnCmd,
        NULL, NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIRXP - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcSDIRXP_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXP));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcSDIRXP*  DtIoStubBcSDIRXP_Open(DtBc*  pBc)
{
    DtIoStubBcSDIRXP*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDIRXP));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcSDIRXP, pBc, NULL, 
                                                             DtIoStubBcSDIRXP_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_SDIRXP_IOCTLS);
    pStub = (DtIoStubBcSDIRXP*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIRXP- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIRXP_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSdiRxPCmdInput*  pInData = NULL;
    DtIoctlSdiRxPCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXP));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_SDIRXP_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, SDIRXP, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SdiRxPCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SdiRxPCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
     case DT_SDIRXP_CMD_GET_CRC_ERROR_CNT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIRXP_OnCmdGetCrcErrorCount(SDIRXP_STUB, 
                                                             &pOutData->m_GetCrcErrorCnt);
        break;
    case DT_SDIRXP_CMD_GET_MAX_SDIRATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIRXP_OnCmdGetMaxSdiRate(SDIRXP_STUB,
                                                              &pOutData->m_GetMaxSdiRate);
        break;
    case DT_SDIRXP_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIRXP_OnCmdGetOperationalMode(SDIRXP_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_SDIRXP_CMD_GET_SDIRATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIRXP_OnCmdGetSdiRate(SDIRXP_STUB, &pOutData->m_GetSdiRate);
        break;
    case DT_SDIRXP_CMD_GET_SDI_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIRXP_OnCmdGetSdiStatus(SDIRXP_STUB,
                                                               &pOutData->m_GetSdiStatus);
        break;
    case DT_SDIRXP_CMD_GET_SDI_STATUS2:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIRXP_OnCmdGetSdiStatus2(SDIRXP_STUB,
                                                              &pOutData->m_GetSdiStatus2);
        break;
    case DT_SDIRXP_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDIRXP_OnCmdSetOperationalMode(SDIRXP_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_SDIRXP_CMD_SET_SDIRATE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDIRXP_OnCmdSetSdiRate(SDIRXP_STUB, &pInData->m_SetSdiRate);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP_OnCmdGetCrcErrorCount -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIRXP_OnCmdGetCrcErrorCount(
    const DtIoStubBcSDIRXP* pStub,
    DtIoctlSdiRxPCmdGetCrcErrorCountOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXP));
    DT_ASSERT(pOutData != NULL);

    // Get crc error count
    return DtBcSDIRXP_GetCrcErrorCount(SDIRXP_BC, &pOutData->m_CrcErrorCount);
}


//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP_OnCmdGetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIRXP_OnCmdGetMaxSdiRate(
    const DtIoStubBcSDIRXP* pStub,
    DtIoctlSdiRxPCmdGetMaxSdiRateOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXP));
    DT_ASSERT(pOutData != NULL);

    // Get SDI mode
    return DtBcSDIRXP_GetMaxSdiRate(SDIRXP_BC, &pOutData->m_MaxSdiRate);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIRXP_OnCmdGetOperationalMode(
    const DtIoStubBcSDIRXP* pStub,
    DtIoctlSdiRxPCmdGetOpModeOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXP));
    DT_ASSERT(pOutData != NULL);

    // Get operational mode
    return DtBcSDIRXP_GetOperationalMode(SDIRXP_BC, &pOutData->m_OpMode);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP_OnCmdGetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIRXP_OnCmdGetSdiRate(
    const DtIoStubBcSDIRXP* pStub,
    DtIoctlSdiRxPCmdGetSdiRateOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXP));
    DT_ASSERT(pOutData != NULL);

    // Get SDI mode
    return DtBcSDIRXP_GetSdiRate(SDIRXP_BC, &pOutData->m_SdiRate);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP_OnCmdGetSdiStatus -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIRXP_OnCmdGetSdiStatus(
    const DtIoStubBcSDIRXP* pStub,
    DtIoctlSdiRxPCmdGetSdiStatusOutput* pOutData)
{
    Int SdiRate=0;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXP));
    DT_ASSERT(pOutData != NULL);

    // Get SDI-status
    return DtBcSDIRXP_GetSdiStatus(SDIRXP_BC, &pOutData->m_SdiLock,
                                    &pOutData->m_LineLock, &pOutData->m_Valid, &SdiRate,
                                    &pOutData->m_NumSymsHanc, &pOutData->m_NumSymsVidVanc,
                                    &pOutData->m_NumLinesF1, &pOutData->m_NumLinesF2,
                                    &pOutData->m_IsLevelB, &pOutData->m_PayloadId, 
                                    &pOutData->m_FramePeriod);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP_OnCmdGetSdiStatus2 -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIRXP_OnCmdGetSdiStatus2(
    const DtIoStubBcSDIRXP* pStub,
    DtIoctlSdiRxPCmdGetSdiStatusOutput2* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXP));
    DT_ASSERT(pOutData != NULL);

    // Get SDI-status
    return DtBcSDIRXP_GetSdiStatus(SDIRXP_BC, &pOutData->m_SdiLock,
                          &pOutData->m_LineLock, &pOutData->m_Valid, &pOutData->m_SdiRate,
                          &pOutData->m_NumSymsHanc, &pOutData->m_NumSymsVidVanc,
                          &pOutData->m_NumLinesF1, &pOutData->m_NumLinesF2,
                          &pOutData->m_IsLevelB, &pOutData->m_PayloadId, 
                          &pOutData->m_FramePeriod);
}
//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIRXP_OnCmdSetOperationalMode(
    const DtIoStubBcSDIRXP* pStub,
    const DtIoctlSdiRxPCmdSetOpModeInput* pInData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXP));
    DT_ASSERT(pInData != NULL);

    // Set operational mode
    return DtBcSDIRXP_SetOperationalMode(SDIRXP_BC, pInData->m_OpMode);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP_OnCmdSetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIRXP_OnCmdSetSdiRate(
    const DtIoStubBcSDIRXP* pStub,
    const DtIoctlSdiRxPCmdSetSdiRateInput* pInData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXP));
    DT_ASSERT(pInData != NULL);

    // Set SDI mode
    return DtBcSDIRXP_SetSdiRate(SDIRXP_BC, pInData->m_SdiRate);
}