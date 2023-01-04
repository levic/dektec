//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQSYNC.c *#*#*#*#*#*#*#*#*#*# (C) 2021 DekTec


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Copyright (C) 2021 DekTec Digital Video B.V.
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
#include "DtBc.h"
#include "DtBcIQSYNC.h"
#include "DtBcIQSYNC_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQSYNC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQSYNC))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQSYNC_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQSYNC, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

static DtStatus DtBcIQSYNC_GetOperationalMode(DtBcIQSYNC*, Int*);
static DtStatus  DtBcIQSYNC_GetSampleCounters(DtBcIQSYNC*, UInt32*, UInt32*, UInt32*);
static DtStatus  DtBcIQSYNC_GetStatus(DtBcIQSYNC*, Int*, Int*, Int*, Int*);
static DtStatus  DtBcIQSYNC_GetSyncMode(DtBcIQSYNC*, Int *);
static DtStatus  DtBcIQSYNC_GetTimeTolerance(DtBcIQSYNC*, Int *);
static DtStatus  DtBcIQSYNC_GetTriggerTimeOffset(DtBcIQSYNC*, Int*);
static DtStatus  DtBcIQSYNC_Init(DtBc*);
static DtStatus  DtBcIQSYNC_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQSYNC_OnEnable(DtBc*, Bool);
static void  DtBcIQSYNC_SetControlRegs(DtBcIQSYNC*, Bool, Int, Int);
static DtStatus  DtBcIQSYNC_SetOperationalMode(DtBcIQSYNC*, Int);
static DtStatus  DtBcIQSYNC_SetSampleCounters(DtBcIQSYNC*, UInt32, UInt32 , UInt32 );
static DtStatus  DtBcIQSYNC_SetSyncMode(DtBcIQSYNC*, Int);
static DtStatus  DtBcIQSYNC_SetTimeTolerance(DtBcIQSYNC*, Int);
static DtStatus  DtBcIQSYNC_SetTriggerTimeOffset(DtBcIQSYNC*, Int);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQSYNC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIQSYNC_Close(DtBc*  pBc)
{
    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIQSYNC*  DtBcIQSYNC_Open(Int  Address, DtCore* pCore, DtPt* pPt, const char* pRole, 
    Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_IQSYNC_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQSYNC, Id, DT_BLOCK_TYPE_IQSYNC,
        Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQSYNC_Close;
    OpenParams.m_InitFunc = DtBcIQSYNC_Init;
    OpenParams.m_OnEnableFunc = DtBcIQSYNC_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQSYNC_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQSYNC*)DtBc_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQSYNC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQSYNC_GetOperationalMode(DtBcIQSYNC* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQSYNC_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_GetSampleCounters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQSYNC_GetSampleCounters(DtBcIQSYNC* pBc, UInt32* pCounterN, 
    UInt32* pCounterP, UInt32* pCounterQ)
{
    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQSYNC_MUST_BE_ENABLED(pBc);
    DT_ASSERT(pCounterN != NULL && pCounterP != NULL && pCounterQ != NULL);

    *pCounterN = pBc->m_SampleCounterN;
    *pCounterP = pBc->m_SampleCounterP;
    *pCounterQ = pBc->m_SampleCounterQ;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQSYNC_GetStatus(DtBcIQSYNC* pBc, Int* pOpStatus, Int* pUnderflowCount, 
                                                  Int* pTooEarlyCount, Int* pTooLateCount)
{
    UInt32  RegStatus;
    Int FwOpStatus;

    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQSYNC_MUST_BE_ENABLED(pBc);
    DT_ASSERT(pOpStatus != NULL && pOpStatus!=NULL && pUnderflowCount!=NULL);

    RegStatus = IQSYNC_Status_READ(pBc);
    FwOpStatus = IQSYNC_Status_GET_OperationalStatus(RegStatus);
    switch (FwOpStatus)
    {
        case IQSYNC_OPSTATUS_IDLE: *pOpStatus = DT_IQSYNC_OPSTATUS_IDLE; break;
        case IQSYNC_OPSTATUS_STANDBY: *pOpStatus = DT_IQSYNC_OPSTATUS_STANDBY; break;
        case IQSYNC_OPSTATUS_RUN: *pOpStatus = DT_IQSYNC_OPSTATUS_RUN; break;
        case IQSYNC_OPSTATUS_ERROR: *pOpStatus = DT_IQSYNC_OPSTATUS_ERROR; break;
        default: break;
    };

    *pUnderflowCount = (Int)IQSYNC_Status_GET_UnderflowCnt(RegStatus);
    *pTooEarlyCount = (Int)IQSYNC_TooEarlyCnt_READ(pBc);
    *pTooLateCount = (Int)IQSYNC_TooLateCnt_READ(pBc);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_GetSyncMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQSYNC_GetSyncMode(DtBcIQSYNC* pBc, Int *pSyncMode)
{
    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQSYNC_MUST_BE_ENABLED(pBc);
    DT_ASSERT(pSyncMode != NULL);

    *pSyncMode = pBc->m_SyncMode;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_GetTimeTolerance -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQSYNC_GetTimeTolerance(DtBcIQSYNC* pBc, Int* pTimeTolerance)
{
    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQSYNC_MUST_BE_ENABLED(pBc);
    DT_ASSERT(pTimeTolerance != NULL);

    *pTimeTolerance = pBc->m_TimeTolerance;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_GetTriggerTimeOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQSYNC_GetTriggerTimeOffset(DtBcIQSYNC* pBc, Int* pOffset)
{
    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQSYNC_MUST_BE_ENABLED(pBc);
    DT_ASSERT(pOffset != NULL);

    *pOffset = pBc->m_TriggerTimeOffset;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQSYNC_Init(DtBc* pBcBase)
{
    DtBcIQSYNC* pBc = (DtBcIQSYNC*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_SyncMode =  DT_IQSYNC_SYNCMODE_NOSYNC;

    // Set control register
    DtBcIQSYNC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                         pBc->m_SyncMode);
    // Initialize registers
    pBc->m_SampleCounterN = 0; 
    pBc->m_SampleCounterP = 0; 
    pBc->m_SampleCounterQ = 0;
    IQSYNC_SampleCounterN_WRITE(pBc, pBc->m_SampleCounterN);
    IQSYNC_SampleCounterP_WRITE(pBc, pBc->m_SampleCounterP);
    IQSYNC_SampleCounterQ_WRITE(pBc, pBc->m_SampleCounterQ);
    pBc->m_TimeTolerance = 10;
    IQSYNC_TriggerMode_WRITE(pBc, pBc->m_TimeTolerance);
    pBc->m_TriggerTimeOffset = 0;
    IQSYNC_TriggerTimeOffset_WRITE(pBc, pBc->m_TriggerTimeOffset);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQSYNC_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQSYNC* pBc = (DtBcIQSYNC*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQSYNC, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQSYNC_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQSYNC, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQSYNC_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQSYNC* pBc = (DtBcIQSYNC*)pBcBase;

    // Sanity check
    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQSYNC, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IQSYNC, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIQSYNC_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQSYNC, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIQSYNC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                         pBc->m_SyncMode);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQSYNC_SetControlRegs(DtBcIQSYNC* pBc, Bool BlkEna, Int OpMode, Int SyncMode)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0, FwSyncMode = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQSYNC_BLKENA_ENABLED : IQSYNC_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQSYNC_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQSYNC_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQSYNC_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    switch (SyncMode)
    {
        case DT_IQSYNC_SYNCMODE_NOSYNC: FwSyncMode=IQSYNC_SYNCMODE_NOSYNC; break;
        case DT_IQSYNC_SYNCMODE_ONEPPS: FwSyncMode=IQSYNC_SYNCMODE_ONEPPS; break;
        case DT_IQSYNC_SYNCMODE_TRIGGER: FwSyncMode=IQSYNC_SYNCMODE_TRIGGER; break;
        default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IQSYNC_Control_READ(pBc);
    RegData = IQSYNC_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQSYNC_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = IQSYNC_Control_SET_SyncMode(RegData, FwSyncMode);
    IQSYNC_Control_WRITE(pBc, RegData);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQSYNC_SetOperationalMode(DtBcIQSYNC* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQSYNC_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;

    // Set control register
    DtBcIQSYNC_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode, pBc->m_SyncMode);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_SetSampleCounters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQSYNC_SetSampleCounters(DtBcIQSYNC* pBc, UInt32 CounterN, UInt32 CounterP, 
                                                                          UInt32 CounterQ)
{
    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQSYNC_MUST_BE_ENABLED(pBc);

    pBc->m_SampleCounterN = CounterN;
    pBc->m_SampleCounterP = CounterP;
    pBc->m_SampleCounterQ = CounterQ;
    IQSYNC_SampleCounterN_WRITE(pBc, CounterN);
    IQSYNC_SampleCounterP_WRITE(pBc, CounterP);
    IQSYNC_SampleCounterQ_WRITE(pBc, CounterQ);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_SetSyncMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQSYNC_SetSyncMode(DtBcIQSYNC* pBc, Int SyncMode)
{
    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQSYNC_MUST_BE_ENABLED(pBc);

    if(SyncMode!=DT_IQSYNC_SYNCMODE_NOSYNC && SyncMode!=DT_IQSYNC_SYNCMODE_ONEPPS && 
                                                     SyncMode!=DT_IQSYNC_SYNCMODE_TRIGGER)
        return DT_STATUS_INVALID_PARAMETER;

    pBc->m_SyncMode = SyncMode;

    // Set control register
    DtBcIQSYNC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, SyncMode);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_SetTimeTolerance -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQSYNC_SetTimeTolerance(DtBcIQSYNC* pBc, Int TimeTolerance)
{
    UInt32  RegData;
    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQSYNC_MUST_BE_ENABLED(pBc);

    if (TimeTolerance>=0x100000)
        return DT_STATUS_INVALID_PARAMETER;

    pBc->m_TimeTolerance = TimeTolerance;

    RegData = IQSYNC_TriggerMode_READ(pBc);
    RegData = IQSYNC_TriggerMode_SET_TimeTolerance(RegData, TimeTolerance);
    IQSYNC_TriggerMode_WRITE(pBc, RegData);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSYNC_SetTriggerTimeOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQSYNC_SetTriggerTimeOffset(DtBcIQSYNC* pBc, Int Offset)
{
    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQSYNC_MUST_BE_ENABLED(pBc);

    // Offset is 31-bits signed.
    if (Offset < -1073741823 || Offset>1073741822)
        return DT_STATUS_INVALID_PARAMETER;
    Offset &= 0x7FFFFFFF;

    pBc->m_TriggerTimeOffset = Offset;
    IQSYNC_TriggerTimeOffset_WRITE(pBc, pBc->m_TriggerTimeOffset);
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQSYNC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQSYNC))
// Helper macro to cast stub's DtBc member to DtBcIQSYNC
#define IQSYNC_STUB   ((DtIoStubBcIQSYNC*)pStub)
#define IQSYNC_BC  ((DtBcIQSYNC*)IQSYNC_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQSYNC_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcIQSYNC_OnCmdGetOperationalMode( const DtIoStubBcIQSYNC*,
                                                        DtIoctlIqSyncCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIQSYNC_OnCmdGetSampleCounters( const DtIoStubBcIQSYNC*,
                                                DtIoctlIqSyncCmdGetSampleCountersOutput*);

static DtStatus  DtIoStubBcIQSYNC_OnCmdGetStatus( const DtIoStubBcIQSYNC*,
                                                        DtIoctlIqSyncCmdGetStatusOutput*);

static DtStatus  DtIoStubBcIQSYNC_OnCmdGetSyncMode( const DtIoStubBcIQSYNC*,
                                                      DtIoctlIqSyncCmdGetSyncModeOutput*);

static DtStatus  DtIoStubBcIQSYNC_OnCmdGetTimeTolerance( const DtIoStubBcIQSYNC*,
                                                 DtIoctlIqSyncCmdGetTimeToleranceOutput*);

static DtStatus  DtIoStubBcIQSYNC_OnCmdGetTriggerTimeOffset( const DtIoStubBcIQSYNC*,
                                                 DtIoctlIqSyncCmdGetTriggerOffsetOutput*);

static DtStatus  DtIoStubBcIQSYNC_OnCmdSetOperationalMode( const DtIoStubBcIQSYNC*,
                                                   const DtIoctlIqSyncCmdSetOpModeInput*);

static DtStatus  DtIoStubBcIQSYNC_OnCmdSetSampleCounters( const DtIoStubBcIQSYNC*,
                                            const DtIoctlIqSyncCmdSetSampleCountersInput*);

static DtStatus  DtIoStubBcIQSYNC_OnCmdSetSyncMode( const DtIoStubBcIQSYNC*,
                                                 const DtIoctlIqSyncCmdSetSyncModeInput*);
static DtStatus  DtIoStubBcIQSYNC_OnCmdSetTimeTolerance( const DtIoStubBcIQSYNC*,
                                            const DtIoctlIqSyncCmdSetTimeToleranceInput*);
static DtStatus  DtIoStubBcIQSYNC_OnCmdSetTriggerTimeOffset( const DtIoStubBcIQSYNC*,
                                            const DtIoctlIqSyncCmdSetTriggerOffsetInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQSYNC;

static const DtIoctlProperties  IOSTUB_BC_IQSYNC_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQSYNC_CMD(
        DtIoStubBcIQSYNC_OnCmd,
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQSYNC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcIQSYNC_Close(DtIoStub* pStub)
{
    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIQSYNC*  DtIoStubBcIQSYNC_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQSYNC_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQSYNC, pBc, NULL,
        DtIoStubBcIQSYNC_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IQSYNC_IOCTLS);
    return (DtIoStubBcIQSYNC*)DtIoStubBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQSYNC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQSYNC_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqSyncCmdInput*  pInData = NULL;
    DtIoctlIqSyncCmdOutput*  pOutData = NULL;

    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQSYNC_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQSYNC, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqSyncCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqSyncCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQSYNC_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQSYNC_OnCmdGetOperationalMode(IQSYNC_STUB, 
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_IQSYNC_CMD_GET_SAMPLE_COUNTERS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQSYNC_OnCmdGetSampleCounters(IQSYNC_STUB, 
                                                           &pOutData->m_GetSampleCounter);
        break;
    case DT_IQSYNC_CMD_GET_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQSYNC_OnCmdGetStatus(IQSYNC_STUB, &pOutData->m_GetStatus);
        break;
    case DT_IQSYNC_CMD_GET_SYNC_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQSYNC_OnCmdGetSyncMode(IQSYNC_STUB, &pOutData->m_GetSyncMode);
        break;
    case DT_IQSYNC_CMD_GET_TIME_TOLERANCE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQSYNC_OnCmdGetTimeTolerance(IQSYNC_STUB, 
                                                           &pOutData->m_GetTimeTolerance);
        break;
    case DT_IQSYNC_CMD_GET_TRIGGER_OFFSET:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQSYNC_OnCmdGetTriggerTimeOffset(IQSYNC_STUB, 
                                                           &pOutData->m_GetTriggerOffset);
        break;
    case DT_IQSYNC_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQSYNC_OnCmdSetOperationalMode(IQSYNC_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_IQSYNC_CMD_SET_SAMPLE_COUNTERS:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQSYNC_OnCmdSetSampleCounters(IQSYNC_STUB, 
                                                            &pInData->m_SetSampleCounter);
        break;
    case DT_IQSYNC_CMD_SET_SYNC_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQSYNC_OnCmdSetSyncMode(IQSYNC_STUB, &pInData->m_SetSyncMode);
        break;
    case DT_IQSYNC_CMD_SET_TIME_TOLERANCE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQSYNC_OnCmdSetTimeTolerance(IQSYNC_STUB, 
                                                            &pInData->m_SetTimeTolerance);
        break;
    case DT_IQSYNC_CMD_SET_TRIGGER_OFFSET:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQSYNC_OnCmdSetTriggerTimeOffset(IQSYNC_STUB, 
                                                        &pInData->m_SetTriggerTimeOffset);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQSYNC_OnCmdGetOperationalMode( const DtIoStubBcIQSYNC* pStub,
                                                DtIoctlIqSyncCmdGetOpModeOutput* pOutData)
{
    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQSYNC_GetOperationalMode(IQSYNC_BC, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_OnCmdGetSampleCounters -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQSYNC_OnCmdGetSampleCounters(const DtIoStubBcIQSYNC* pStub,
                                        DtIoctlIqSyncCmdGetSampleCountersOutput* pOutData)
{
    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQSYNC_GetSampleCounters(IQSYNC_BC, &pOutData->m_SampleCounterN, 
                                                             &pOutData->m_SampleCounterP, 
                                                             &pOutData->m_SampleCounterQ);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_OnCmdGetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQSYNC_OnCmdGetStatus(const DtIoStubBcIQSYNC* pStub,
    DtIoctlIqSyncCmdGetStatusOutput* pOutData)
{
    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQSYNC_GetStatus(IQSYNC_BC, &pOutData->m_OperationalStatus, 
                                                              &pOutData->m_UnderflowCount, 
                                                              &pOutData->m_TooEarlyCount,
                                                              &pOutData->m_TooLateCount);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_OnCmdGetSyncMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQSYNC_OnCmdGetSyncMode( const DtIoStubBcIQSYNC* pStub,
                                              DtIoctlIqSyncCmdGetSyncModeOutput* pOutData)
{
    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQSYNC_GetSyncMode(IQSYNC_BC, &pOutData->m_SyncMode);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_OnCmdGetTimeTolerance -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQSYNC_OnCmdGetTimeTolerance( const DtIoStubBcIQSYNC* pStub,
                                         DtIoctlIqSyncCmdGetTimeToleranceOutput* pOutData)
{
    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQSYNC_GetTimeTolerance(IQSYNC_BC, &pOutData->m_TimeTolerance);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_OnCmdGetTriggerTimeOffset -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQSYNC_OnCmdGetTriggerTimeOffset( const DtIoStubBcIQSYNC* pStub,
                                         DtIoctlIqSyncCmdGetTriggerOffsetOutput* pOutData)
{
    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQSYNC_GetTriggerTimeOffset(IQSYNC_BC, &pOutData->m_TriggerTimeOffset);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQSYNC_OnCmdSetOperationalMode(
    const DtIoStubBcIQSYNC* pStub,
    const DtIoctlIqSyncCmdSetOpModeInput* pInData)
{
    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQSYNC_SetOperationalMode(IQSYNC_BC, pInData->m_OpMode);
}


// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_OnCmdSetSampleCounters -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQSYNC_OnCmdSetSampleCounters( const DtIoStubBcIQSYNC* pStub,
                                     const DtIoctlIqSyncCmdSetSampleCountersInput* pInData)
{
    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQSYNC_SetSampleCounters(IQSYNC_BC, pInData->m_SampleCounterN, 
                                                               pInData->m_SampleCounterP, 
                                                               pInData->m_SampleCounterQ);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_OnCmdSetSyncMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQSYNC_OnCmdSetSyncMode( const DtIoStubBcIQSYNC* pStub,
                                          const DtIoctlIqSyncCmdSetSyncModeInput* pInData)
{
    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQSYNC_SetSyncMode(IQSYNC_BC, pInData->m_SyncMode);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_OnCmdSetTimeTolerance -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQSYNC_OnCmdSetTimeTolerance( const DtIoStubBcIQSYNC* pStub,
                                     const DtIoctlIqSyncCmdSetTimeToleranceInput* pInData)
{
    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQSYNC_SetTimeTolerance(IQSYNC_BC, pInData->m_TimeTolerance);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSYNC_OnCmdSetTriggerTimeOffset -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQSYNC_OnCmdSetTriggerTimeOffset( const DtIoStubBcIQSYNC* pStub,
                                     const DtIoctlIqSyncCmdSetTriggerOffsetInput* pInData)
{
    IQSYNC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQSYNC_SetTriggerTimeOffset(IQSYNC_BC, pInData->m_TriggerTimeOffset);
}