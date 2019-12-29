//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDITXF.c *#*#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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
#include "DtBc.h"
#include "DtBcSDITXF.h"
#include "DtBcSDITXF_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXF implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SDITXF_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDITXF))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SDITXF_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SDITXF, pBc)

// Helper macro to cast a DtBc* to a DtBcSDITXF*
#define BC_SDITXF         ((DtBcSDITXF*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSDITXF_Init(DtBc*);
static DtStatus  DtBcSDITXF_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcSDITXF_OnEnable(DtBc*, Bool  Enable);
static DtStatus  DtBcSDITXF_RegisterIntHandlers(DtBcSDITXF*);
static void  DtBcSDITXF_InterruptDpcFmtEvent(DtDpcArgs* pArgs);
static DtStatus  DtBcSDITXF_InterruptHandler(DtBc*, Int, Int, void*);
static void  DtBcSDITXF_SetControlRegs(DtBcSDITXF*, Bool BlkEna, Int OpMode, 
                                                                             Bool UlfEna);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXF - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSDITXF_Close(DtBc*  pBc)
{
    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcSDITXF*  DtBcSDITXF_Open(Int  Address, DtCore*  pCore, DtPt*  pPt, DtBcType  Type,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(Type== DT_BLOCK_TYPE_SDITXF || Type==DT_BLOCK_TYPE_SDITXF6G12G);

    // Init open parameters
    DT_BC_SDITXF_INIT_ID(Id, Type, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSDITXF, Id, Type, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSDITXF_Close;
    OpenParams.m_InitFunc = DtBcSDITXF_Init;
    OpenParams.m_OnEnableFunc = DtBcSDITXF_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcSDITXF_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSDITXF*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_GetFmtEventSetting -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXF_GetFmtEventSetting(DtBcSDITXF* pBc,  Int* pNumLinesPerEvent,
                                                            Int* pNumSofsBetweenTimestamp)
{
    // Sanity check
    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pNumLinesPerEvent==NULL || pNumSofsBetweenTimestamp==NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_SDITXF_MUST_BE_ENABLED(pBc);

    // Return cached format event setting
    *pNumLinesPerEvent = pBc->m_NumLinesPerEvent;
    *pNumSofsBetweenTimestamp = pBc->m_NumSofsBetweenTod;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_GetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXF_GetMaxSdiRate(DtBcSDITXF* pBc, Int* pMaxSdiRate)
{
    // Sanity check
    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pMaxSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXF_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pMaxSdiRate = pBc->m_MaxSdiRate;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXF_GetOperationalMode(DtBcSDITXF* pBc, Int* pOpMode)
{
    // Sanity check
    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXF_MUST_BE_ENABLED(pBc);

    // Return cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_GetStreamAlignment -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXF_GetStreamAlignment(DtBcSDITXF* pBc, Int* pStreamAlignment)
{
    // Sanity check
    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pStreamAlignment == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXF_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pStreamAlignment = pBc->m_StreamAlignment;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_SetFmtEventSetting -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXF_SetFmtEventSetting(DtBcSDITXF*  pBc,  Int NumLinesPerEvent,
                                                             Int  NumSofsBetweenTimestamp)
{
    UInt32 RegFmtEvent;

    // Sanity check
    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (NumLinesPerEvent<0 || NumLinesPerEvent>0x3FF || NumSofsBetweenTimestamp<0)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXF_MUST_BE_ENABLED(pBc);

    // Operational mode must be idle
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Set new Format-Event Setting
    RegFmtEvent = SDITXF_FmtEvent_READ(pBc);
    RegFmtEvent = SDITXF_FmtEvent_SET_NumLines(RegFmtEvent, (UInt32)NumLinesPerEvent);
    SDITXF_FmtEvent_WRITE(pBc, RegFmtEvent);

    // Update cached format event setting
    pBc->m_NumLinesPerEvent = NumLinesPerEvent;
    pBc->m_NumSofsBetweenTod = NumSofsBetweenTimestamp;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXF_SetOperationalMode(DtBcSDITXF* pBc, Int OpMode)
{
    DtStatus Status;

    // Sanity check
    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXF_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;


    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        // IDLE -> RUN
        // Initially disable underflow detection; will be enabled after the first event
        BC_SDITXF->m_UlfEnabled = FALSE;
        BC_SDITXF->m_UlfEnableDelayCount = 0;
        BC_SDITXF->m_UnderflowLatched = FALSE;
        BC_SDITXF->m_Underflow = FALSE;

        // Reset SOF timestamp and counter
        pBc->m_SofCount = 0;
        pBc->m_SofTimeValid = FALSE;

        // Reset format event
        DtEventReset(&pBc->m_FmtEvent);
        // Enable formatter event interrupt
        pBc->m_FmtIntEnabled = TRUE;
        Status = DtBc_InterruptEnable((DtBc*)pBc, pBc->m_FmtIntId);
        DT_ASSERT(DT_SUCCESS(Status));
    }
    else
    {
        // RUN -> IDLE
        // Disable formatter event interrupt
        Status = DtBc_InterruptDisable((DtBc*)pBc, pBc->m_FmtIntId);
        DT_ASSERT(DT_SUCCESS(Status));

        // Make sure that WaitForFmtEvent doesn't wait forever
        pBc->m_FmtIntEnabled = FALSE;
        DtEventSet(&pBc->m_FmtEvent);
    }

    // Set new control
     DtBcSDITXF_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode, pBc->m_UlfEnabled);

    // Update cached operational mode
    pBc->m_OperationalMode = OpMode;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_WaitForFmtEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXF_WaitForFmtEvent(DtBcSDITXF* pBc, Int Timeout, Int* pFrameId,
                                                        Int* pSeqNumber, Int* pUnderflow,
                                                        DtTodTime* pTime, Int* pTimeValid)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (Timeout<-1 || Timeout>1000)
        return DT_STATUS_INVALID_PARAMETER;
    if (pFrameId== NULL || pSeqNumber==NULL || pUnderflow==NULL || pTime==NULL
                                                                      || pTimeValid==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXF_MUST_BE_ENABLED(pBc);

    // Operational mode must be run and format event interrupts must be enabled
    if (pBc->m_OperationalMode!=DT_BLOCK_OPMODE_RUN || !pBc->m_FmtIntEnabled)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Wait for next formatter event interrupt
    Status = DtEventWaitUnInt(&pBc->m_FmtEvent, Timeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SDITXF, pBc, "Wait on event failure, Status:0x%X", Status);
        return Status;
    }

    // Check again
    if (!pBc->m_FmtIntEnabled)
        return DT_STATUS_CANCELLED;


    DtSpinLockAcquire(&pBc->m_IntDataSpinLock);

    // Clear the event 
    DtEventReset(&pBc->m_FmtEvent);

    // Copy the last formatter event interrupt data
    *pFrameId = pBc->m_FrameId;
    *pSeqNumber = pBc->m_SeqNumber;
    *pUnderflow = (Int)pBc->m_UnderflowLatched;
    pBc->m_UnderflowLatched = FALSE;
    *pTime = pBc->m_SofTime;
    *pTimeValid = (Int)pBc->m_SofTimeValid;
    DtSpinLockRelease(&pBc->m_IntDataSpinLock);

    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXF - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDITXF_Init(DtBc*  pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcSDITXF* pBc = (DtBcSDITXF*)pBcBase;
    UInt32  RegFmtEvent;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_UlfEnabled = FALSE;
    pBc->m_UnderflowLatched = FALSE;
    pBc->m_Underflow = FALSE;
    pBc->m_UlfEnableDelayCount = 0;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    DtBcSDITXF_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                       pBc->m_UlfEnabled);
    // Get maximum supported rate
    pBc->m_MaxSdiRate = DT_DRV_SDIRATE_3G;
    if (pBc->m_Version > 0)
    {
        UInt32  FwMaxSdiRate = SDITXF_Config_READ_MaxSdiRate(pBc);
        switch (FwMaxSdiRate)
        {
        case SDITXF_SDIMODE_SD:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_SD; break;
        case SDITXF_SDIMODE_HD:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_HD; break;
        case SDITXF_SDIMODE_3G:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_3G; break;
        case SDITXF_SDIMODE_6G:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_6G; break;
        case SDITXF_SDIMODE_12G:    pBc->m_MaxSdiRate = DT_DRV_SDIRATE_12G; break;
        default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
        }
    }

    // Get stream-alignement
    pBc->m_StreamAlignment = 32;
    if (pBc->m_Version > 0)
    {
        UInt32  FwAlignment = SDITXF_Config_READ_Alignment(pBc);
        switch (FwAlignment)
        {
        case SDITXF_STREAMALIGNMENT_32b:   pBc->m_StreamAlignment = 32; break;
        case SDITXF_STREAMALIGNMENT_64b:   pBc->m_StreamAlignment = 64; break;
        case SDITXF_STREAMALIGNMENT_128b:  pBc->m_StreamAlignment = 128; break;
        case SDITXF_STREAMALIGNMENT_256b:  pBc->m_StreamAlignment = 256; break;
        case SDITXF_STREAMALIGNMENT_512b:  pBc->m_StreamAlignment = 512; break;
        default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
        }
    }
    // Default an interrupt at SOF only
    pBc->m_NumLinesPerEvent = 0;
    RegFmtEvent = SDITXF_FmtEvent_READ(pBc);
    RegFmtEvent = SDITXF_FmtEvent_SET_NumLines(RegFmtEvent, 
                                                         (UInt32)pBc->m_NumLinesPerEvent);
    SDITXF_FmtEvent_WRITE(pBc, RegFmtEvent);

    // Format event interrupts are disabled
    pBc->m_FmtIntEnabled = FALSE;

    // Initialize SpinLock
    DtSpinLockInit(&pBc->m_IntDataSpinLock);

    // Initialize formatter event
    Status = DtEventInit(&pBc->m_FmtEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SDITXF, pBc, 
                              "ERROR: failed to init done event (Status=0x%08X)", Status);
        return Status;
    }
    
    // Init interrupt DPC
    Status = DtDpcInit(&pBc->m_IntDpc, DtBcSDITXF_InterruptDpcFmtEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SDITXF, pBc, "ERROR: failed to init DPC (Status=0x%08X)", Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Status = DtBcSDITXF_RegisterIntHandlers(pBc);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SDITXF, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXF_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSDITXF*  pBc = (DtBcSDITXF*)pBcBase;
    UInt32  RegFmtEvent;

    // Sanity check
    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, SDITXF, pBc, "Block Enable from disable -> enable");

        // Set defaults and enable block
        pBc->m_UlfEnabled = FALSE;
        pBc->m_UlfEnableDelayCount = 0;
        pBc->m_UnderflowLatched = FALSE;
        pBc->m_Underflow = FALSE;
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
        DtBcSDITXF_SetControlRegs(pBc, Enable, pBc->m_OperationalMode,
                                                                       pBc->m_UlfEnabled);
        // Default an interrupt at SOF only
        pBc->m_NumLinesPerEvent = 0;
        pBc->m_NumSofsBetweenTod = 0;

        // Reset SOF timestamp and counter
        pBc->m_SofCount = 0;
        pBc->m_SofTimeValid = FALSE;
        RegFmtEvent = SDITXF_FmtEvent_READ(pBc);
        RegFmtEvent = SDITXF_FmtEvent_SET_NumLines(RegFmtEvent, 
                                                         (UInt32)pBc->m_NumLinesPerEvent);
        SDITXF_FmtEvent_WRITE(pBc, RegFmtEvent);

    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, SDITXF, pBc, "Block Enable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcSDITXF_SetOperationalMode(BC_SDITXF, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDITXF, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }

        // Disable block
        DtBcSDITXF_SetControlRegs(pBc, Enable, pBc->m_OperationalMode,
                                                                       pBc->m_UlfEnabled);
    }
    // Save new setting
    pBc->m_BlockEnabled = Enable;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDITXF_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled(pBc))
    {
        DtDbgOutBc(AVG, SDITXF, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcSDITXF_SetOperationalMode(BC_SDITXF, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDITXF, pBc, "ERROR: failed to set opmode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_InterruptDpcFmtEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcSDITXF_InterruptDpcFmtEvent(DtDpcArgs* pArgs)
{
    DtBcSDITXF*  pBc = (DtBcSDITXF*)pArgs->m_pContext;
    UInt32 RegFmtEvStatus = pArgs->m_Data1.m_UInt32_2;

    // Sanity check
#ifdef DEBUG
    UInt32  IntId = pArgs->m_Data1.m_UInt32_1;
    DT_ASSERT(IntId==DT_INTERRUPT_SDITXF_FMTEVENT 
                                             || IntId==DT_INTERRUPT_SDITXF6G12G_FMTEVENT);
#endif  // #ifdef DEBUG
    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);

    // Get the format event status
    DtSpinLockAcquireAtDpc(&pBc->m_IntDataSpinLock);
    pBc->m_FrameId =  SDITXF_FmtEvStatus_GET_FrameId(RegFmtEvStatus);
    pBc->m_SeqNumber = SDITXF_FmtEvStatus_GET_SeqNum(RegFmtEvStatus);
    pBc->m_Underflow = (SDITXF_FmtEvStatus_GET_Underflow(RegFmtEvStatus) != 0);
    pBc->m_SofTimeValid = FALSE;

    // Start-of-Frame? and take timestamp on SoF?
    if (pBc->m_SeqNumber==0 &&  pBc->m_NumSofsBetweenTod>0)
    {
        pBc->m_SofCount++;
        if (pBc->m_SofCount >= pBc->m_NumSofsBetweenTod)
        {
            DtCore_TOD_GetTime(pBc->m_pCore, &pBc->m_SofTime);
            pBc->m_SofTimeValid = TRUE;
            pBc->m_SofCount = 0;
        }
    }

    // Enable underflow detection after the first event
    if (!pBc->m_UlfEnabled)
    {
        pBc->m_UlfEnableDelayCount++;
        if (pBc->m_UlfEnableDelayCount >= 1)
        {
            pBc->m_UlfEnabled = TRUE;
            DtBcSDITXF_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                       pBc->m_UlfEnabled);
        }
        pBc->m_Underflow = FALSE;
    }
    pBc->m_UnderflowLatched |= pBc->m_Underflow;

    // Signal formatter event
    DtEventSet(&pBc->m_FmtEvent);
    DtSpinLockReleaseFromDpc(&pBc->m_IntDataSpinLock);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDITXF_InterruptHandler(DtBc*  pBc, Int  Id, Int  Index, void*  pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;

    // Sanity check
    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);

    // Check ID is one of ours
    switch (Id)
    {
    case DT_INTERRUPT_SDITXF_FMTEVENT:
    case DT_INTERRUPT_SDITXF6G12G_FMTEVENT:
        break;

        // Not a SDITXF interrupt
    default:
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    // Get the event status
    DpcArgs.m_Data1.m_UInt32_2 = SDITXF_FmtEvStatus_READ(BC_SDITXF);
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcSDITXF*)pBc)->m_IntDpc, &DpcArgs);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDITXF_RegisterIntHandlers(DtBcSDITXF*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;
    Int  i=0;

    // Sanity check
    BC_SDITXF_DEFAULT_PRECONDITIONS(pBc);

    pBc->m_FmtIntId = -1;

    // Register interrupt callbacks, but do not enable the interrupts. We will enable 
    // them on demand
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_SDITXF_FMTEVENT:
        case DT_INTERRUPT_SDITXF6G12G_FMTEVENT:
            // Store format event interrupt ID 
            pBc->m_FmtIntId = Id;
            // NOTE: the SDITXF uses one handler for all interrupt sources
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id, 
                                                     DtBcSDITXF_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, SDITXF, pBc,
                               "ERROR: failed to register interrupt handler (ID=%d)", Id);
            }
            break;

        default:
            DT_ASSERT(FALSE);      // Unknown ID. New version of the block??
            break;
        }
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSDITXF_SetControlRegs(DtBcSDITXF* pBc, Bool BlkEna, Int OpMode, 
                                                                          Bool  UflEnable)
{
    UInt32 RegData=0, FwBlkEna=0, FwOpMode=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? SDITXF_BLKENA_ENABLED : SDITXF_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = SDITXF_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = SDITXF_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = SDITXF_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control
    RegData = SDITXF_Control_READ(pBc);
    RegData = SDITXF_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = SDITXF_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = SDITXF_Control_SET_UflEnable(RegData, UflEnable);
    SDITXF_Control_WRITE(pBc, RegData);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDITXF implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcSDITXF
#define SDITXF_STUB   ((DtIoStubBcSDITXF*)pStub)
#define SDITXF_BC  ((DtBcSDITXF*)SDITXF_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcSDITXF_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcSDITXF_OnCmdGetFmtEventSetting(const DtIoStubBcSDITXF*, 
                                             DtIoctlSdiTxFCmdGetFmtEventSettingOutput*);
static DtStatus  DtIoStubBcSDITXF_OnCmdGetMaxSdiRate(const DtIoStubBcSDITXF*, 
                                                    DtIoctlSdiTxFCmdGetMaxSdiRateOutput*);
static DtStatus  DtIoStubBcSDITXF_OnCmdGetOperationalMode(const DtIoStubBcSDITXF*,
                                                      DtIoctlSdiTxFCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcSDITXF_OnCmdGetStreamAlignment(const DtIoStubBcSDITXF*, 
                                               DtIoctlSdiTxFCmdGetStreamAlignmentOutput*);
static DtStatus  DtIoStubBcSDITXF_OnCmdSetFmtEventSetting(const DtIoStubBcSDITXF*,
                                       const  DtIoctlSdiTxFCmdSetFmtEventSettingInput*);
static DtStatus  DtIoStubBcSDITXF_OnCmdSetOperationalMode(const DtIoStubBcSDITXF*,
                                                 const DtIoctlSdiTxFCmdSetOpModeInput*);
static DtStatus  DtIoStubBcSDITXF_OnCmdWaitForFmtEvent(const DtIoStubBcSDITXF*,
                                           const  DtIoctlSdiTxFCmdWaitForFmtEventInput*,
                                           DtIoctlSdiTxFCmdWaitForFmtEventOutput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SDITXF;

static const DtIoctlProperties  IOSTUB_BC_SDITXF_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_SDITXF_CMD(
        DtIoStubBcSDITXF_OnCmd,
        NULL,  NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDITXF - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXF_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcSDITXF_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXF));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXF_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcSDITXF*  DtIoStubBcSDITXF_Open(DtBc*  pBc)
{
    DtIoStubBcSDITXF*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDITXF));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcSDITXF, pBc, NULL, 
                                                             DtIoStubBcSDITXF_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_SDITXF_IOCTLS);
    pStub = (DtIoStubBcSDITXF*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDITXF- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXF_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDITXF_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSdiTxFCmdInput*  pInData = NULL;
    DtIoctlSdiTxFCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXF));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_SDITXF_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, SDITXF, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SdiTxFCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SdiTxFCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_SDITXF_CMD_GET_FMT_EVENT_SETTING:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDITXF_OnCmdGetFmtEventSetting(SDITXF_STUB, 
                                                          &pOutData->m_GetFmtEventSetting);
        break;
    case DT_SDITXF_CMD_GET_MAX_SDIRATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDITXF_OnCmdGetMaxSdiRate(SDITXF_STUB,
                                                              &pOutData->m_GetMaxSdiRate);
        break;
    case DT_SDITXF_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDITXF_OnCmdGetOperationalMode(SDITXF_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_SDITXF_CMD_GET_STREAM_ALIGNMENT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDITXF_OnCmdGetStreamAlignment(SDITXF_STUB,
                                                               &pOutData->m_GetAlignment);
        break;
    case DT_SDITXF_CMD_SET_FMT_EVENT_SETTING:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDITXF_OnCmdSetFmtEventSetting(SDITXF_STUB, 
                                                           &pInData->m_SetFmtEventSetting);
        break;
    case DT_SDITXF_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDITXF_OnCmdSetOperationalMode(SDITXF_STUB,
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_SDITXF_CMD_WAIT_FOR_FMT_EVENT:
        DT_ASSERT(pInData!=NULL && pOutData!=NULL);
        Status = DtIoStubBcSDITXF_OnCmdWaitForFmtEvent(SDITXF_STUB, 
                               &pInData->m_WaitForFmtEvent, &pOutData->m_WaitForFmtEvent);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXF_OnCmdGetFmtEventSetting -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDITXF_OnCmdGetFmtEventSetting(
    const DtIoStubBcSDITXF* pStub,
    DtIoctlSdiTxFCmdGetFmtEventSettingOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXF));
    DT_ASSERT(pOutData != NULL);

    // Get format event interrupt timing
    return DtBcSDITXF_GetFmtEventSetting(SDITXF_BC, &pOutData->m_NumLinesPerEvent,
                                                          &pOutData->m_NumSofsBetweenTod);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXF_OnCmdGetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDITXF_OnCmdGetMaxSdiRate(
    const DtIoStubBcSDITXF* pStub,
    DtIoctlSdiTxFCmdGetMaxSdiRateOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXF));
    DT_ASSERT(pOutData != NULL);

    // Get maximum SDI-rate
    return DtBcSDITXF_GetMaxSdiRate(SDITXF_BC, &pOutData->m_MaxSdiRate);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXF_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDITXF_OnCmdGetOperationalMode(
    const DtIoStubBcSDITXF* pStub,
    DtIoctlSdiTxFCmdGetOpModeOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXF));
    DT_ASSERT(pOutData != NULL);

    // Get operational Mode
    return DtBcSDITXF_GetOperationalMode(SDITXF_BC, &pOutData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXF_OnCmdGetStreamAlignment -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDITXF_OnCmdGetStreamAlignment(
    const DtIoStubBcSDITXF* pStub,
    DtIoctlSdiTxFCmdGetStreamAlignmentOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXF));
    DT_ASSERT(pOutData != NULL);

    // Get maximum SDI-rate
    return DtBcSDITXF_GetStreamAlignment(SDITXF_BC, &pOutData->m_StreamAlignment);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXF_OnCmdSetFmtEventTiming -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDITXF_OnCmdSetFmtEventSetting(
    const DtIoStubBcSDITXF* pStub,
    const DtIoctlSdiTxFCmdSetFmtEventSettingInput* pInData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXF));
    DT_ASSERT(pInData != NULL);

    // Set format event interrupt timing
    return DtBcSDITXF_SetFmtEventSetting(SDITXF_BC, pInData->m_NumLinesPerEvent,
                                                            pInData->m_NumSofsBetweenTod);
}


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXF_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDITXF_OnCmdSetOperationalMode(
    const DtIoStubBcSDITXF* pStub,
    const DtIoctlSdiTxFCmdSetOpModeInput* pInData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXF));
    DT_ASSERT(pInData != NULL);

    // Set operational mode
    return DtBcSDITXF_SetOperationalMode(SDITXF_BC, pInData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXF_OnCmdWaitForFmtEvent -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDITXF_OnCmdWaitForFmtEvent(
    const DtIoStubBcSDITXF* pStub,
    const DtIoctlSdiTxFCmdWaitForFmtEventInput* pInData,
    DtIoctlSdiTxFCmdWaitForFmtEventOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXF));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    // Set data timeout
    return DtBcSDITXF_WaitForFmtEvent(SDITXF_BC, pInData->m_Timeout,
                                         &pOutData->m_FrameId, &pOutData->m_SeqNumber, 
                                         &pOutData->m_Underflow, 
                                         &pOutData->m_SofTime, &pOutData->m_SofTimeValid);
}
