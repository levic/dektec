// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfAsiRx.c *#*#*#*#*#*#*#*# (C) 2017-2018 DekTec
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
#include "DtDfAsiRx.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#define ASIRXP_ROLE_NONE        NULL
#define TSRXFMT_ROLE_NONE       NULL

// ASI-TX-RX Polarity Detection States
#define ASIRX_POL_STATE_LOCKED    0    // Polarity control state locked
#define ASIRX_POL_STATE_UNKNOWN_1 1    // Polarity control state unknown first time
#define ASIRX_POL_STATE_UNKNOWN_N 8    // Polarity control state unknown > seventh time

#define ASIRXP_SYNCS_TO_LOCK    5

// Thresholds used per ASI-TX-RX Polarity Detection State
// in  number of transport-stream words received. This gives the maximum time to wait for
// detection of the packet size.
static const Int  ASIRX_POL_STATE_THRESHOLDS[] =
{
    0,                                  // No threshold when LOCKED
    (ASIRXP_SYNCS_TO_LOCK+1)*204*1,     // Threshold for state UNKNOWN_1
    (ASIRXP_SYNCS_TO_LOCK+1)*204*2,     // Threshold for state UNKNOWN_2
    (ASIRXP_SYNCS_TO_LOCK+1)*204*4,     // Threshold for state UNKNOWN_3
    (ASIRXP_SYNCS_TO_LOCK+1)*204*8,     // Threshold for state UNKNOWN_4
    (ASIRXP_SYNCS_TO_LOCK+1)*204*16,    // Threshold for state UNKNOWN_5
    (ASIRXP_SYNCS_TO_LOCK+1)*204*64,    // Threshold for state UNKNOWN_6
    (ASIRXP_SYNCS_TO_LOCK+1)*204*128,   // Threshold for state UNKNOWN_7
    (ASIRXP_SYNCS_TO_LOCK+1)*204*204,   // Threshold for state UNKNOWN_N
};

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfAsiRx implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiRx function
#define DF_ASIRX_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfAsiRx))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_ASIRX_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(ASIRX, pDf)

// Helper macro to cast a DtDf* to a DtDfAsiRx*
#define DF_ASIRX      ((DtDfAsiRx*)pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfAsiRx_Init(DtDf*);
static DtStatus  DtDfAsiRx_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfAsiRx_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfAsiRx_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfAsiRx_LoadParameters(DtDf*);
static DtStatus  DtDfAsiRx_OpenChildren(DtDfAsiRx*);
static void  DtDfAsiRx_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);
static DtStatus  DtDfAsiRx_PolarityGet(DtDfAsiRx*, Int* pPolCtrl);
static DtStatus  DtDfAsiRx_PolarityGetStatus(DtDfAsiRx*, Int* pPolStatus);
static DtStatus  DtDfAsiRx_PolarityInit(DtDfAsiRx*);
static DtStatus  DtDfAsiRx_PolaritySet(DtDfAsiRx*, Int PolCtrl);
static void DtDfAsiRx_PolarityUpdate(DtDfAsiRx*);


// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfAsiRx - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfAsiRx_Close(DtDf*  pDf)
{
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);

    //Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pDf->m_pCore, (DtObject*)pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfAsiRx*  DtDfAsiRx_Open(DtCore*  pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_ASIRX_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfAsiRx, Id, DT_FUNC_TYPE_ASIRX, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfAsiRx_Close;
    OpenParams.m_InitFunc = DtDfAsiRx_Init;
    OpenParams.m_OnCloseFileFunc = DtDfAsiRx_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfAsiRx_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfAsiRx_OnEnablePreChildren;
    OpenParams.m_LoadParsFunc = DtDfAsiRx_LoadParameters;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfAsiRx*)DtDf_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_GetOperationalStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAsiRx_GetOperationalStatus(DtDfAsiRx*  pDf, Int* pOpStatus)
{
    DtStatus Status = DT_STATUS_OK;
    Int  TSRXFMTOpstatus;
    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_ASIRX_MUST_BE_ENABLED(pDf);

    // Check parameter
    if (pOpStatus == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Determine current operational status
    switch(pDf->m_OperationalMode)
    {
    case DT_FUNC_OPMODE_IDLE: *pOpStatus = DT_FUNC_OPSTATUS_IDLE; break;
    case DT_FUNC_OPMODE_RUN: *pOpStatus = DT_FUNC_OPSTATUS_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // If not in IDLE check whether one of the childeren is in ERROR
    if (pDf->m_OperationalMode != DT_FUNC_OPMODE_IDLE)
    { 
        // Get TSRXFMT operational status
        Status = DtBcTSRXFMT_GetOperationalStatus(pDf->m_pBcTsRxFmt, &TSRXFMTOpstatus);
        if (DT_SUCCESS(Status))
        {
            if (TSRXFMTOpstatus == DT_BLOCK_OPSTATUS_ERROR)
                *pOpStatus = DT_FUNC_OPSTATUS_ERROR;
        }
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_GetPacketMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfAsiRx_GetPacketMode(DtDfAsiRx*  pDf, Int* pPckMode)
{
    DtStatus  Status = DT_STATUS_OK;
    Int BcPckMode;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pPckMode != NULL);

    // Operational state must be enabled
    DF_ASIRX_MUST_BE_ENABLED(pDf);

    // Get TSRXFMT packet mode
    Status = DtBcTSRXFMT_GetPacketMode(pDf->m_pBcTsRxFmt, &BcPckMode);
    if (DT_SUCCESS(Status))
    {
        // Convert packet mode to driver type
        switch (BcPckMode)
        {
        case DT_TSRXFMT_PCKMODE_AUTO: *pPckMode = DT_ASIRX_PCKMODE_AUTO; break; 
        case DT_TSRXFMT_PCKMODE_RAW:  *pPckMode = DT_ASIRX_PCKMODE_RAW; break; 
        default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
        }
    }
    return Status;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_GetPolarityCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAsiRx_GetPolarityCtrl(DtDfAsiRx*  pDf, Int* pPolarityCtrl)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pPolarityCtrl != NULL);

    // Operational state must be enabled
    DF_ASIRX_MUST_BE_ENABLED(pDf);


    // Get current polarity control setting
    Status = DtDfAsiRx_PolarityGet(pDf, pPolarityCtrl);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfAsiRx_GetStatus(
    DtDfAsiRx*  pDf, 
    Int*  pPckSize, 
    Int*  pCarrierDet, 
    Int*  pAsiLock,
    Int*  pDetPolarity)
{
    DtStatus  Status = DT_STATUS_OK;
    Int BcPacketSize;
    Bool BcCarrierDet, BcAsiLock;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pPckSize!=NULL && pCarrierDet!=NULL && pAsiLock!=NULL 
                                                                   && pDetPolarity!=NULL);

    // Operational state must be enabled
    DF_ASIRX_MUST_BE_ENABLED(pDf);

    // Get TSRXFMT packet size
    Status = DtBcTSRXFMT_GetTsPacketSize(pDf->m_pBcTsRxFmt, &BcPacketSize);
    if (DT_SUCCESS(Status))
    {
        // Convert packet size to driver type
        switch (BcPacketSize)
        {
        case DT_TSRXFMT_PCKSIZE_188:     *pPckSize = DT_ASIRX_PCKSIZE_188; break; 
        case DT_TSRXFMT_PCKSIZE_204:     *pPckSize = DT_ASIRX_PCKSIZE_204; break; 
        case DT_TSRXFMT_PCKSIZE_UNKNOWN: *pPckSize = DT_ASIRX_PCKSIZE_UNKNOWN; break; 
        default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
        }
    } 
    else
        return Status;

    // Get ASIRXP status
    Status = DtBcASIRXP_GetStatus(pDf->m_pBcAsiRxP, &BcCarrierDet, &BcAsiLock);
    if (DT_SUCCESS(Status))
    {
        // Convert status to driver type
        *pCarrierDet = (Int)BcCarrierDet;
        *pAsiLock = (Int)BcAsiLock;
    } 
    else
        return Status;

    // Get polarity status
    Status = DtDfAsiRx_PolarityGetStatus(pDf, pDetPolarity);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_GetSyncMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAsiRx_GetSyncMode(DtDfAsiRx*  pDf, Int* pSyncMode)
{
    DtStatus  Status = DT_STATUS_OK;
    Int BcSyncMode;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pSyncMode != NULL);

    // Operational state must be enabled
    DF_ASIRX_MUST_BE_ENABLED(pDf);

    // Get TSRXFMT synchronization mode
    Status = DtBcTSRXFMT_GetSyncMode(pDf->m_pBcTsRxFmt, &BcSyncMode);
    if (DT_SUCCESS(Status))
    {
        // Convert sync mode to driver type
        switch (BcSyncMode)
        {
        case DT_TSRXFMT_SYNCMODE_AUTO: *pSyncMode = DT_ASIRX_SYNCMODE_AUTO; break; 
        case DT_TSRXFMT_SYNCMODE_188:  *pSyncMode = DT_ASIRX_SYNCMODE_188; break; 
        case DT_TSRXFMT_SYNCMODE_204:  *pSyncMode = DT_ASIRX_SYNCMODE_204; break; 
        default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
        }
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_GetTsBitrate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAsiRx_GetTsBitrate(DtDfAsiRx*  pDf, Int* pTsBitrate)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pTsBitrate != NULL);

    // Operational state must be enabled
    DF_ASIRX_MUST_BE_ENABLED(pDf);


    // Get ASIRXP TS-bitrate
    Status = DtBcASIRXP_GetTsBitrate(pDf->m_pBcAsiRxP, pTsBitrate);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_GetViolCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAsiRx_GetViolCount(DtDfAsiRx*  pDf, UInt* pViolCount)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pViolCount != NULL);

    // Operational state must be enabled
    DF_ASIRX_MUST_BE_ENABLED(pDf);

    // Get ASIRXP violation count
    Status = DtBcASIRXP_GetViolCount(pDf->m_pBcAsiRxP, pViolCount);

    return Status;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfAsiRx_SetOperationalMode(DtDfAsiRx*  pDf, Int  OpMode)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (OpMode!=DT_FUNC_OPMODE_IDLE && OpMode!=DT_FUNC_OPMODE_RUN)
    {
        DtDbgOutDf(ERR, ASIRX, pDf, "Invalid function operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // No change?
    if (pDf->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    if (OpMode == DT_FUNC_OPMODE_IDLE)
    {
        // RUN -> IDLE
        // Set operational mode of TSRXFMT to STANDBY
        Status = DtBcTSRXFMT_SetOperationalMode(pDf->m_pBcTsRxFmt, 
                                                                 DT_BLOCK_OPMODE_STANDBY);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, ASIRX, pDf, "ERROR: DtBcTSRXFMT_SetOperationalMode failed");
            return Status;
        }
    }
    else
    {
        // IDLE -> RUN
        // Operational state must be enabled
        DF_ASIRX_MUST_BE_ENABLED(pDf);
        
        // Set operational mode of TSRXFMT to RUN
        Status = DtBcTSRXFMT_SetOperationalMode(pDf->m_pBcTsRxFmt, DT_BLOCK_OPMODE_RUN);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, ASIRX, pDf, "ERROR: DtBcTSRXFMT_SetOperationalMode failed");
            return Status;
        }
    }
    // Save new operational mode
     if (DT_SUCCESS(Status))
        pDf->m_OperationalMode = OpMode;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_SetPacketMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfAsiRx_SetPacketMode(DtDfAsiRx*  pDf, Int  PckMode)
{
    DtStatus  Status = DT_STATUS_OK;
    Int BcPckMode;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_ASIRX_MUST_BE_ENABLED(pDf);


    // Convert packet mode from driver type
    switch (PckMode)
    {
    case DT_ASIRX_PCKMODE_AUTO: BcPckMode = DT_TSRXFMT_PCKMODE_AUTO; break;
    case DT_ASIRX_PCKMODE_RAW:  BcPckMode = DT_TSRXFMT_PCKMODE_RAW; break;
    default: DT_ASSERT(FALSE); return DT_STATUS_INVALID_PARAMETER;
    }

    // Set TSRXFMT packet mode
    Status = DtBcTSRXFMT_SetPacketMode(pDf->m_pBcTsRxFmt, BcPckMode);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_SetPolarityCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAsiRx_SetPolarityCtrl(DtDfAsiRx*  pDf, Int PolarityCtrl)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_ASIRX_MUST_BE_ENABLED(pDf);

    // Get current polarity control setting
    Status = DtDfAsiRx_PolaritySet(pDf, PolarityCtrl);

    return Status;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_SetSyncMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAsiRx_SetSyncMode(DtDfAsiRx*  pDf, Int SyncMode)
{
    DtStatus  Status = DT_STATUS_OK;
    Int BcSyncMode;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_ASIRX_MUST_BE_ENABLED(pDf);

    // Convert sync mode from driver type
    switch (SyncMode)
    {
    case DT_ASIRX_SYNCMODE_AUTO:  BcSyncMode = DT_TSRXFMT_SYNCMODE_AUTO; break;
    case DT_ASIRX_SYNCMODE_188:   BcSyncMode = DT_TSRXFMT_SYNCMODE_188; break;
    case DT_ASIRX_SYNCMODE_204:   BcSyncMode = DT_TSRXFMT_SYNCMODE_204; break;
    default: DT_ASSERT(FALSE); return DT_STATUS_INVALID_PARAMETER;
    }

    // Set TSRXFMT synchronization mode
    Status = DtBcTSRXFMT_SetSyncMode(pDf->m_pBcTsRxFmt, BcSyncMode);

    return Status;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfAsiRx - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAsiRx_Init(DtDf*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    DtOnPeriodicIntervalRegData  RegData;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);

    // Initialize spinlock
    DtSpinLockInit(&DF_ASIRX->m_PolControl.m_PolaritySpinLock);

    // Set defaults
    DF_ASIRX->m_OperationalMode = DT_FUNC_OPMODE_IDLE;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfAsiRx_OpenChildren(DF_ASIRX);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, ASIRX, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    // Init periodic interval handler enable flag and spinlock
    DtSpinLockInit(&DF_ASIRX->m_PerItvSpinLock);
    DF_ASIRX->m_PerItvEnable = FALSE;

    // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtDfAsiRx_PeriodicIntervalHandler;
    RegData.m_pObject = (DtObject*)pDf;
    Status = DtCore_TOD_PeriodicItvRegister(pDf->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, ASIRX, pDf, "ERROR: failed to register period itv handler");
        return Status;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtDfAsiRx_OnCloseFile(DtDf* pDf, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);

    // Check if the owner closed the file handle
    Status = DtDf_ExclAccessCheck(pDf, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutDf(AVG, ASIRX, pDf, "Go to IDLE");

        // Go to IDLE
        Status = DtDfAsiRx_SetOperationalMode(DF_ASIRX, DT_FUNC_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, ASIRX, pDf, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtDf_OnCloseFile(pDf, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAsiRx_OnEnablePostChildren(DtDf* pDfBase, Bool  Enable)
{
    DtDfAsiRx*  pDf = (DtDfAsiRx*)pDfBase;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);
    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutDf(AVG, ASIRX, pDf, "Children Enable from disable -> enable");

        // Operational mode must be IDLE
        DT_ASSERT(pDf->m_OperationalMode == DT_FUNC_OPMODE_IDLE);

        //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Start in IDLE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
        DT_RETURN_ON_ERROR(DtBcASIRXP_SetOperationalMode(pDf->m_pBcAsiRxP, 
                                                                   DT_BLOCK_OPMODE_IDLE));
        DT_RETURN_ON_ERROR(DtBcTSRXFMT_SetOperationalMode(pDf->m_pBcTsRxFmt, 
                                                                   DT_BLOCK_OPMODE_IDLE));

        //-.-.-.-.-.-.-.-.-.-.-.-.-.- Start finding polarity  -.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
        // Set operational mode of TSRXFMT to STANDBY
        DT_RETURN_ON_ERROR(DtBcTSRXFMT_SetOperationalMode(pDf->m_pBcTsRxFmt, 
                                                                DT_BLOCK_OPMODE_STANDBY));
        // Set operational mode of ASIRXP to STANDBY
        DT_RETURN_ON_ERROR(DtBcASIRXP_SetOperationalMode(pDf->m_pBcAsiRxP, 
                                                                   DT_BLOCK_OPMODE_RUN));
        // Initialize polarity
        DT_RETURN_ON_ERROR(DtDfAsiRx_PolarityInit(pDf));

        // Enable periodic interval handler
        DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
        pDf->m_PerItvEnable = TRUE;
        DtSpinLockRelease(&pDf->m_PerItvSpinLock);
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfAsiRx_OnEnablePreChildren(DtDf* pDfBase, Bool  Enable)
{
    DtDfAsiRx*  pDf = (DtDfAsiRx*)pDfBase;

    // Sanity checks
    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);
    if (!Enable)
    {
       // ENABLE -> DISABLE
        DtDbgOutDf(AVG, ASIRX, pDf, "Children Enable from enable -> disable");

        // Disable periodic interval handler
        DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
        pDf->m_PerItvEnable = FALSE;
        DtSpinLockRelease(&pDf->m_PerItvSpinLock);

        //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Return to IDLE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        DT_RETURN_ON_ERROR(DtDfAsiRx_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE));
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfAsiRx_LoadParameters(DtDf*  pDf)
{
    return DT_STATUS_OK;
}
 
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAsiRx_OpenChildren(DtDfAsiRx*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

     // List of children supported by the the ASIRX function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_ASIRXP, DT_BC_ASIRXP_NAME, 
                           ASIRXP_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcAsiRxP), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_TSRXFMT, DT_BC_TSRXFMT_NAME, 
                         TSRXFMT_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcTsRxFmt), TRUE},
    };

    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pDf->m_pBcAsiRxP != NULL);
    DT_ASSERT(pDf->m_pBcTsRxFmt != NULL);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_PeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfAsiRx_PeriodicIntervalHandler(DtObject* pObj, DtTodTime  Time)
{
    DtDfAsiRx*  pDf = (DtDfAsiRx*)pObj;
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfAsiRx));

    DtSpinLockAcquireAtDpc(&pDf->m_PerItvSpinLock);
    // Only when enabled
    if (pDf->m_PerItvEnable)
        DtDfAsiRx_PolarityUpdate(pDf);  // Update polarity
    DtSpinLockReleaseFromDpc(&pDf->m_PerItvSpinLock);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_PolarityGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfAsiRx_PolarityGet(DtDfAsiRx*  pDf, Int* pPolCtrl)
{
    PolarityControl* pPc = &pDf->m_PolControl;
    DT_ASSERT(pDf!=NULL && pPc!=NULL && pPolCtrl!=NULL);
    *pPolCtrl = pPc->m_PolarityCtrl;

    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_PolarityGetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfAsiRx_PolarityGetStatus(DtDfAsiRx*  pDf, Int* pDetPol)
{
    PolarityControl* pPc = &pDf->m_PolControl;
    DtStatus  Status = DT_STATUS_OK;

    DT_ASSERT(pDf!=NULL && pPc!=NULL && pDetPol!=NULL);

    // Prevent mutual access to polarity control data
    DtSpinLockAcquire(&pPc->m_PolaritySpinLock);

    // In lock state, return the current selected polarity
    if (pPc->m_DetectState == ASIRX_POL_STATE_LOCKED)
    {
        if (pPc->m_BcPolarity == DT_BC_ASIRXP_POL_NORMAL)
            *pDetPol = DT_ASIRX_POLARITY_NORMAL;
        else if (pPc->m_BcPolarity == DT_BC_ASIRXP_POL_INVERT)
            *pDetPol = DT_ASIRX_POLARITY_INVERT;
        else
        {
            DT_ASSERT(FALSE);
            *pDetPol = DT_ASIRX_POLARITY_UNKNOWN;
        }
    }
    else
        *pDetPol = DT_ASIRX_POLARITY_UNKNOWN;

    DtSpinLockRelease(&pPc->m_PolaritySpinLock);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_PolarityInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAsiRx_PolarityInit(DtDfAsiRx*  pDf)
{
    PolarityControl* pPc = &pDf->m_PolControl;
    DtStatus  Status = DT_STATUS_OK;
    UInt BcTsWordCnt;

    DT_ASSERT(pDf!=NULL && pPc!=NULL);

    // Prevent mutual access to polarity control data
    DtSpinLockAcquire(&pPc->m_PolaritySpinLock);

    // Get TS-word count
    Status = DtBcASIRXP_GetTsWordCount(pDf->m_pBcAsiRxP, &BcTsWordCnt);
    DT_ASSERT(Status == DT_STATUS_OK);

    // Set default settings: Normal Polarity and Auto Detection
    pPc->m_PolarityCtrl = DT_ASIRX_POLARITY_AUTO;
    pPc->m_DetectState = ASIRX_POL_STATE_UNKNOWN_1;
    pPc->m_TsWordCntThresshold = BcTsWordCnt + 
                                         ASIRX_POL_STATE_THRESHOLDS[pPc->m_DetectState];
    pPc->m_BcPolarity = DT_BC_ASIRXP_POL_NORMAL;
    Status = DtBcASIRXP_SetPolarityCtrl(pDf->m_pBcAsiRxP, pPc->m_BcPolarity);

    DtSpinLockRelease(&pPc->m_PolaritySpinLock);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_PolaritySet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfAsiRx_PolaritySet(DtDfAsiRx*  pDf, Int PolCtrl)
{
    PolarityControl*  pPc = &pDf->m_PolControl;
    DtStatus  Status = DT_STATUS_OK;
    Int  NewBcPolarity;
    DT_ASSERT(pDf!=NULL && pPc!=NULL);

     // No change?
    if (PolCtrl == pPc->m_PolarityCtrl)
        return DT_STATUS_OK;
    
    // Check parameters and determine new ASIRXP polarity, no change if AUTO is selected
    switch (PolCtrl)
    {
    case DT_ASIRX_POLARITY_AUTO:   NewBcPolarity = pPc->m_BcPolarity; break;
    case DT_ASIRX_POLARITY_NORMAL: NewBcPolarity = DT_BC_ASIRXP_POL_NORMAL; break;
    case DT_ASIRX_POLARITY_INVERT: NewBcPolarity = DT_BC_ASIRXP_POL_INVERT; break;
    default: return DT_STATUS_INVALID_PARAMETER;
    }

    // Prevent mutual access to polarity control data
    DtSpinLockAcquire(&pPc->m_PolaritySpinLock);
   
    // New polarity selected?
    if (NewBcPolarity != pPc->m_BcPolarity)
    {
        // Set new ASIRXP polarity
        pPc->m_BcPolarity = NewBcPolarity;
        Status = DtBcASIRXP_SetPolarityCtrl(pDf->m_pBcAsiRxP, pPc->m_BcPolarity);
        DT_ASSERT(Status == DT_STATUS_OK);

        // Polarity state becomes unknown
        pPc->m_DetectState = ASIRX_POL_STATE_UNKNOWN_1;       
    }

    // If the polarity state is not locked set a new thresshold for polarity inversion
    if (pPc->m_DetectState != ASIRX_POL_STATE_LOCKED)
    {
        UInt  BcTsWordCnt = pPc->m_TsWordCntThresshold;
        Status = DtBcASIRXP_GetTsWordCount(pDf->m_pBcAsiRxP, &BcTsWordCnt);
        DT_ASSERT(Status == DT_STATUS_OK);
        pPc->m_TsWordCntThresshold = BcTsWordCnt + 
                                         ASIRX_POL_STATE_THRESHOLDS[pPc->m_DetectState];
    }

    if (DT_SUCCESS(Status))
        // Save new setting
        pPc->m_PolarityCtrl = PolCtrl;

    DtSpinLockRelease(&pPc->m_PolaritySpinLock);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx_PolarityUpdate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Called from DPC
//
void DtDfAsiRx_PolarityUpdate(DtDfAsiRx*  pDf)
{
    PolarityControl* pPc = &pDf->m_PolControl;
    DtStatus  Status = DT_STATUS_OK;
    Bool BcCarrierDet= FALSE, BcAsiLock=FALSE;
    Int BcPacketSize = DT_TSRXFMT_PCKSIZE_UNKNOWN;
    UInt BcTsWordCnt = pPc->m_TsWordCntThresshold;

    DT_ASSERT(pDf!=NULL && pPc!=NULL);
    
    // Prevent mutual access to polarity control data
    DtSpinLockAcquireAtDpc(&pPc->m_PolaritySpinLock);

    // Get ASI lock
    Status = DtBcASIRXP_GetStatus(pDf->m_pBcAsiRxP, &BcCarrierDet, &BcAsiLock);
    DT_ASSERT(Status == DT_STATUS_OK);
    // Get TS-word count
    Status = DtBcASIRXP_GetTsWordCount(pDf->m_pBcAsiRxP, &BcTsWordCnt);
    DT_ASSERT(Status == DT_STATUS_OK);
    // Get detected packet size
    Status = DtBcTSRXFMT_GetTsPacketSize(pDf->m_pBcTsRxFmt, &BcPacketSize);
    DT_ASSERT(Status == DT_STATUS_OK);

    if (pPc->m_DetectState == ASIRX_POL_STATE_LOCKED)
    {
        // Check polarity lock
        if (BcPacketSize==DT_TSRXFMT_PCKSIZE_UNKNOWN || !BcAsiLock)
        {
            // Polarity Locked -> Polarity Unknown
            pPc->m_DetectState = ASIRX_POL_STATE_UNKNOWN_1;

            // Set thresshold for next polarity inversion
            pPc->m_TsWordCntThresshold = BcTsWordCnt + 
                                         ASIRX_POL_STATE_THRESHOLDS[pPc->m_DetectState];
            DtDbgOutDf(AVG, ASIRX, pDf, "Polarity Locked -> Unknown");
        }
    }
    else
    {
        DT_ASSERT(pPc->m_DetectState>=ASIRX_POL_STATE_UNKNOWN_1
                                      && pPc->m_DetectState<=ASIRX_POL_STATE_UNKNOWN_N);
        // Check polarity lock
        if (BcPacketSize!=DT_TSRXFMT_PCKSIZE_UNKNOWN && BcAsiLock)
        {
            // Polarity Locked again
            pPc->m_DetectState = ASIRX_POL_STATE_LOCKED;
            DtDbgOutDf(AVG, ASIRX, pDf, "Unknown -> Locked");
        }
        else if (pPc->m_PolarityCtrl != DT_ASIRX_POLARITY_AUTO)
        {
            // Not in AUTO mode, so nothing to do
        }
        else if (!BcAsiLock)
        {
            // No ASI lock, start all over again
            pPc->m_DetectState = ASIRX_POL_STATE_UNKNOWN_1;
             // Set new thresshold for next polarity inversion
            pPc->m_TsWordCntThresshold = BcTsWordCnt 
                                       + ASIRX_POL_STATE_THRESHOLDS[pPc->m_DetectState];
        }
        else if ((Int)(BcTsWordCnt - pPc->m_TsWordCntThresshold) > 0)
        {
            // Enough TS-word received to detect the packet size.
            // Try again with inverted polarity.
            pPc->m_BcPolarity = (pPc->m_BcPolarity==DT_BC_ASIRXP_POL_NORMAL) ?
                                        DT_BC_ASIRXP_POL_INVERT : DT_BC_ASIRXP_POL_NORMAL;
            Status = DtBcASIRXP_SetPolarityCtrl(pDf->m_pBcAsiRxP, pPc->m_BcPolarity);
            DT_ASSERT(Status == DT_STATUS_OK);

            // Update the ASI-TX-RX Polarity Detection State
            if (pPc->m_DetectState != ASIRX_POL_STATE_UNKNOWN_N)
                pPc->m_DetectState++;

            // Set new thresshold for next polarity inversion
            pPc->m_TsWordCntThresshold = BcTsWordCnt 
                                       + ASIRX_POL_STATE_THRESHOLDS[pPc->m_DetectState];
     
            DtDbgOutDf(AVG, ASIRX, pDf, "Invert Polarity, New State Unknown_%d",
                                                                      pPc->m_DetectState);
        }
    }

    DtSpinLockReleaseFromDpc(&pPc->m_PolaritySpinLock);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfAsiRx implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtDf member to DtDfAsiRx
#define STUB_ASIRX   ((DtIoStubDfAsiRx*)pStub)
#define STUB_DF  ((DtDfAsiRx*)STUB_ASIRX->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtIoStubDfAsiRx_OnCmd(const DtIoStub* pStub, 
                                             DtIoStubIoParams* pIoParams, Int * pOutSize);
static DtStatus  DtIoStubDfAsiRx_OnCmdGetOperationalStatus(const DtIoStubDfAsiRx*,
                                                       DtIoctlAsiRxCmdGetOpStatusOutput*);
static DtStatus  DtIoStubDfAsiRx_OnCmdGetPacketMode(const DtIoStubDfAsiRx*, 
                                                        DtIoctlAsiRxCmdGetPckModeOutput*);
static DtStatus  DtIoStubDfAsiRx_OnCmdGetPolarityCtrl(const DtIoStubDfAsiRx*, 
                                                   DtIoctlAsiRxCmdGetPolarityCtrlOutput*);
static DtStatus  DtIoStubDfAsiRx_OnCmdGetStatus(const DtIoStubDfAsiRx*, 
                                                         DtIoctlAsiRxCmdGetStatusOutput*);
static DtStatus  DtIoStubDfAsiRx_OnCmdGetSyncMode(const DtIoStubDfAsiRx*, 
                                                       DtIoctlAsiRxCmdGetSyncModeOutput*);
static DtStatus  DtIoStubDfAsiRx_OnCmdGetTsBitrate(const DtIoStubDfAsiRx*, 
                                                      DtIoctlAsiRxCmdGetTsBitrateOutput*);
static DtStatus  DtIoStubDfAsiRx_OnCmdGetViolCount(const DtIoStubDfAsiRx*, 
                                                      DtIoctlAsiRxCmdGetViolCountOutput*);
static DtStatus  DtIoStubDfAsiRx_OnCmdSetOperationalMode(const DtIoStubDfAsiRx*,
                                                    const DtIoctlAsiRxCmdSetOpModeInput*);
static DtStatus  DtIoStubDfAsiRx_OnCmdSetPacketMode(const DtIoStubDfAsiRx*, 
                                                   const DtIoctlAsiRxCmdSetPckModeInput*);
static DtStatus  DtIoStubDfAsiRx_OnCmdSetPolarityCtrl(const DtIoStubDfAsiRx*, 
                                              const DtIoctlAsiRxCmdSetPolarityCtrlInput*);
static DtStatus DtIoStubDfAsiRx_OnCmdSetSyncMode(const DtIoStubDfAsiRx*, 
                                                  const DtIoctlAsiRxCmdSetSyncModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_ASIRX;

static const DtIoctlProperties  IOSTUB_DF_ASIRX_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubDf_OnExclAccessCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_ASIRX_CMD(
        DtIoStubDfAsiRx_OnCmd, 
        NULL, NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfAsiRx - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubDfAsiRx_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubDfAsiRx*  DtIoStubDfAsiRx_Open(DtDf*  pDf)
{
    DtIoStubDfAsiRx*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_ASIRX_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfAsiRx, pDf, NULL, 
                                                              DtIoStubDfAsiRx_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_DF_ASIRX_IOCTLS);
    pStub = (DtIoStubDfAsiRx*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfAsiRx - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfAsiRx_OnCmd(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlAsiRxCmdInput*  pInData = NULL;
    DtIoctlAsiRxCmdOutput*  pOutData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_ASIRX_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, ASIRX, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_AsiTsRxCmd;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_AsiTsRxCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_ASIRX_CMD_GET_OPERATIONAL_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfAsiRx_OnCmdGetOperationalStatus(STUB_ASIRX, 
                                                                &pOutData->m_GetOpStatus);
        break;
    case DT_ASIRX_CMD_GET_PACKET_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfAsiRx_OnCmdGetPacketMode(STUB_ASIRX,
                                                              &pOutData->m_GetPacketMode);
        break;
    case DT_ASIRX_CMD_GET_POLARITY_CTRL:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfAsiRx_OnCmdGetPolarityCtrl(STUB_ASIRX,
                                                                &pOutData->m_GetPolarity);
        break;
    case DT_ASIRX_CMD_GET_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfAsiRx_OnCmdGetStatus(STUB_ASIRX, &pOutData->m_GetStatus);
        break;
    case DT_ASIRX_CMD_GET_SYNC_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfAsiRx_OnCmdGetSyncMode(STUB_ASIRX, &pOutData->m_GetSyncMode);
        break;
    case DT_ASIRX_CMD_GET_TS_BITRATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfAsiRx_OnCmdGetTsBitrate(STUB_ASIRX, &pOutData->m_GetTsBitrate);
        break;
    case DT_ASIRX_CMD_GET_VIOL_COUNT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfAsiRx_OnCmdGetViolCount(STUB_ASIRX, &pOutData->m_GetViolCount);
        break;
    case DT_ASIRX_CMD_SET_OPERATIONAL_MODE:
        Status = DtIoStubDfAsiRx_OnCmdSetOperationalMode(STUB_ASIRX, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_ASIRX_CMD_SET_PACKET_MODE:
        Status = DtIoStubDfAsiRx_OnCmdSetPacketMode(STUB_ASIRX, &pInData->m_SetPckMode);
        break;
    case DT_ASIRX_CMD_SET_POLARITY_CTRL:
        Status = DtIoStubDfAsiRx_OnCmdSetPolarityCtrl(STUB_ASIRX,
                                                                 &pInData->m_SetPolarity);
        break;
    case DT_ASIRX_CMD_SET_SYNC_MODE:
        Status = DtIoStubDfAsiRx_OnCmdSetSyncMode(STUB_ASIRX, &pInData->m_SetSyncMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_OnCmdGetOperationalStatus -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfAsiRx_OnCmdGetOperationalStatus(
    const DtIoStubDfAsiRx*  pStub, 
    DtIoctlAsiRxCmdGetOpStatusOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));
    DT_ASSERT(pOutData != NULL);

    return DtDfAsiRx_GetOperationalStatus(STUB_DF, &pOutData->m_OpStatus);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_OnCmdGetPacketMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfAsiRx_OnCmdGetPacketMode(
    const DtIoStubDfAsiRx*  pStub, 
    DtIoctlAsiRxCmdGetPckModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));
    DT_ASSERT(pOutData != NULL);

    return DtDfAsiRx_GetPacketMode(STUB_DF, &pOutData->m_PckMode);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_OnCmdGetPolarityCtrl -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfAsiRx_OnCmdGetPolarityCtrl(
    const DtIoStubDfAsiRx*  pStub, 
    DtIoctlAsiRxCmdGetPolarityCtrlOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));
    DT_ASSERT(pOutData != NULL);

    return DtDfAsiRx_GetPolarityCtrl(STUB_DF, &pOutData->m_PolarityCtrl);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_OnCmdGetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfAsiRx_OnCmdGetStatus(
    const DtIoStubDfAsiRx*  pStub, 
    DtIoctlAsiRxCmdGetStatusOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));
    DT_ASSERT(pOutData != NULL);

    return DtDfAsiRx_GetStatus(STUB_DF,  
                                      &pOutData->m_PacketSize, &pOutData->m_CarrierDetect, 
                                      &pOutData->m_AsiLock, &pOutData->m_AsiPolarity);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_OnCmdGetSyncMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfAsiRx_OnCmdGetSyncMode(
    const DtIoStubDfAsiRx*  pStub, 
    DtIoctlAsiRxCmdGetSyncModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));
    DT_ASSERT(pOutData != NULL);

    return DtDfAsiRx_GetSyncMode(STUB_DF, &pOutData->m_SyncMode);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_OnCmdGetTsBitrate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfAsiRx_OnCmdGetTsBitrate(
    const DtIoStubDfAsiRx*  pStub, 
    DtIoctlAsiRxCmdGetTsBitrateOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));
    DT_ASSERT(pOutData != NULL);

    return DtDfAsiRx_GetTsBitrate(STUB_DF, &pOutData->m_TsBitrate);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_OnCmdGetViolCount -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfAsiRx_OnCmdGetViolCount(
    const DtIoStubDfAsiRx*  pStub, 
    DtIoctlAsiRxCmdGetViolCountOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));
    DT_ASSERT(pOutData != NULL);

    return DtDfAsiRx_GetViolCount(STUB_DF,  &pOutData->m_ViolCount);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfAsiRx_OnCmdSetOperationalMode(
    const DtIoStubDfAsiRx*  pStub, 
    const DtIoctlAsiRxCmdSetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));
    DT_ASSERT(pInData != NULL);

    return DtDfAsiRx_SetOperationalMode(STUB_DF, pInData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_OnCmdSetPacketMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfAsiRx_OnCmdSetPacketMode(
    const DtIoStubDfAsiRx*  pStub, 
    const DtIoctlAsiRxCmdSetPckModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));
    DT_ASSERT(pInData != NULL);

    return DtDfAsiRx_SetPacketMode(STUB_DF, pInData->m_PckMode);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_OnCmdSetPolarityCtrl -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfAsiRx_OnCmdSetPolarityCtrl(
    const DtIoStubDfAsiRx*  pStub, 
    const DtIoctlAsiRxCmdSetPolarityCtrlInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));
    DT_ASSERT(pInData != NULL);

    return DtDfAsiRx_SetPolarityCtrl(STUB_DF, pInData->m_PolarityCtrl);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx_OnCmdSetSyncMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfAsiRx_OnCmdSetSyncMode(
    const DtIoStubDfAsiRx*  pStub, 
    const DtIoctlAsiRxCmdSetSyncModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfAsiRx));
    DT_ASSERT(pInData != NULL);

    return DtDfAsiRx_SetSyncMode(STUB_DF, pInData->m_SyncMode);
}