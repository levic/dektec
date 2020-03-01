//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIRXF.c *#*#*#*#*#*#*#*# (C) 2017-2018 DekTec
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
#include "DtBc.h"
#include "DtBcSDIRXF.h"
#include "DtBcSDIRXF_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXF implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDIRXF))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SDIRXF_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SDIRXF, pBc)

// Helper macro to cast a DtBc* to a DtBcSDIRXF*
#define BC_SDIRXF         ((DtBcSDIRXF*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSDIRXF_Init(DtBc*);
static DtStatus  DtBcSDIRXF_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcSDIRXF_OnEnable(DtBc*, Bool  Enable);
static DtStatus  DtBcSDIRXF_RegisterIntHandlers(DtBcSDIRXF*);
static void  DtBcSDIRXF_InterruptDpcFmtEvent(DtDpcArgs* pArgs);
static DtStatus  DtBcSDIRXF_InterruptHandler(DtBc*, Int, Int, void*);
static DtStatus DtBcSDIRXF_CheckSdiRate(DtBcSDIRXF*, Int SdiRate);
static void  DtBcSDIRXF_SetControlRegs(DtBcSDIRXF*, Bool BlkEna, Int OpMode);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXF - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSDIRXF_Close(DtBc*  pBc)
{
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcSDIRXF*  DtBcSDIRXF_Open(Int  Address, DtCore*  pCore, DtPt*  pPt, DtBcType  Type,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(Type== DT_BLOCK_TYPE_SDIRXF);

    
    // Init open parameters
    DT_BC_SDIRXF_INIT_ID(Id, Type, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSDIRXF, Id, Type, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSDIRXF_Close;
    OpenParams.m_InitFunc = DtBcSDIRXF_Init;
    OpenParams.m_OnEnableFunc = DtBcSDIRXF_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcSDIRXF_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSDIRXF*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_GetFmtEventTiming -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXF_GetFmtEventTiming(DtBcSDIRXF* pBc, Int* pInterval, Int* pDelay,
                                                                        Int* pIntPerFrame)
{
    // Sanity check
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pInterval==NULL || pDelay==NULL || pIntPerFrame==NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_SDIRXF_MUST_BE_ENABLED(pBc);
    
    // Check whether it has been set
    if (!pBc->m_FmtEvtTimingSet)
    { 
        DtDbgOutBc(ERR, SDIRXF, pBc, "Format event timing is not yet set");
        return DT_STATUS_NOT_INITIALISED;
    }

    // Return cached format event timing
    *pInterval = pBc->m_InterruptInterval;
    *pDelay = pBc->m_InterruptDelay;
    *pIntPerFrame = pBc->m_InterruptsPerFrame;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_GetFrameProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXF_GetFrameProperties(DtBcSDIRXF* pBc, Int* pNumSymsHanc, 
                                      Int* pNumSymsVidVanc, Int* pNumLines, 
                                      Int* pSdiRate, Int* pAssumeInterlaced)
{
    // Sanity check
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pNumSymsHanc==NULL || pNumSymsVidVanc==NULL  || pNumLines==NULL 
                                             || pSdiRate==NULL || pAssumeInterlaced==NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_SDIRXF_MUST_BE_ENABLED(pBc);

    // Check whether it has been set
    if (!pBc->m_FramePropsSet)
    { 
        DtDbgOutBc(ERR, SDIRXF, pBc, "FrameProperties are not yet set");
        return DT_STATUS_NOT_INITIALISED;
    }

    // Return cached frame properties
    *pNumSymsHanc = pBc->m_NumSymsHanc;
    *pNumSymsVidVanc = pBc->m_NumSymsVidVanc;
    *pNumLines = pBc->m_NumLines;
    *pSdiRate = pBc->m_SdiRate;
    *pAssumeInterlaced = (Int)pBc->m_AssumeInterlaced;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_GetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXF_GetMaxSdiRate(DtBcSDIRXF* pBc, Int* pMaxSdiRate)
{
    // Sanity check
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pMaxSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXF_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pMaxSdiRate = pBc->m_MaxSdiRate;

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXF_GetOperationalMode(DtBcSDIRXF* pBc, Int* pOpMode)
{
    // Sanity check
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXF_MUST_BE_ENABLED(pBc);

    // Return cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_GetStreamAlignment -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXF_GetStreamAlignment(DtBcSDIRXF* pBc, Int* pStreamAlignment)
{
    // Sanity check
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pStreamAlignment == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXF_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pStreamAlignment = pBc->m_StreamAlignment;

    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_SetFmtEventTiming -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXF_SetFmtEventTiming(DtBcSDIRXF* pBc, Int Interval, Int Delay,
                                                                          Int IntPerFrame)
{
    UInt32 RegData;

    // Sanity check
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (Interval<=0 || Interval>0xFFFF)
        return DT_STATUS_INVALID_PARAMETER;
    if (Delay<0 || Delay>0xFFF)
        return DT_STATUS_INVALID_PARAMETER;
    if (IntPerFrame<=0 || IntPerFrame>0xF)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXF_MUST_BE_ENABLED(pBc);

    // Operational mode must be idle
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Set new Format-Event Timing
    RegData = SDIRXF_FmtEvIntTiming_SET_Interval(0, (UInt32)Interval);
    RegData = SDIRXF_FmtEvIntTiming_SET_Delay(RegData, (UInt32)Delay);
    RegData = SDIRXF_FmtEvIntTiming_SET_N(RegData, (UInt32)IntPerFrame);
    SDIRXF_FmtEvIntTiming_WRITE(pBc, RegData);
    
    // Update cached format event timing
    pBc->m_InterruptInterval = Interval;
    pBc->m_InterruptDelay = Delay;
    pBc->m_InterruptsPerFrame = IntPerFrame;

    // Format event timing is set
    pBc->m_FmtEvtTimingSet = TRUE;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_SetFrameProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXF_SetFrameProperties(DtBcSDIRXF* pBc, Int NumSymsHanc, 
                                                       Int NumSymsVidVanc, Int NumLines, 
                                                       Int SdiRate, Int AssumeInterlaced)
{
    UInt32 RegData, FwSdiRate;
    // Sanity check
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (NumSymsHanc<0 || NumSymsHanc>0xFFFF)
        return DT_STATUS_INVALID_PARAMETER;
    if (NumSymsVidVanc<0 || NumSymsVidVanc>0xFFFF)
        return DT_STATUS_INVALID_PARAMETER;
    if (NumLines<0 || NumLines>0x3FFF)
        return DT_STATUS_INVALID_PARAMETER;
    DT_RETURN_ON_ERROR(DtBcSDIRXF_CheckSdiRate(pBc, SdiRate));
    if (AssumeInterlaced!=FALSE && AssumeInterlaced!=TRUE)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXF_MUST_BE_ENABLED(pBc);

    // Operational mode must be idle
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Set new line properties
    RegData = SDIRXF_LineProperties_SET_NumSymH(0, NumSymsHanc);
    RegData = SDIRXF_LineProperties_SET_NumSymVV(RegData, NumSymsVidVanc);
    SDIRXF_LineProperties_WRITE(pBc, RegData);
    // Update cached line properties
    pBc->m_NumSymsHanc = NumSymsHanc;
    pBc->m_NumSymsVidVanc= NumSymsVidVanc;

    // Set new frame properties
    switch (SdiRate)
    {
    case DT_DRV_SDIRATE_SD:  FwSdiRate = SDIRXF_SDIMODE_SD; break;
    case DT_DRV_SDIRATE_HD:  FwSdiRate = SDIRXF_SDIMODE_HD; break;
    case DT_DRV_SDIRATE_3G:  FwSdiRate = SDIRXF_SDIMODE_3G; break;
    case DT_DRV_SDIRATE_6G:  FwSdiRate = SDIRXF_SDIMODE_6G; break;
    case DT_DRV_SDIRATE_12G: FwSdiRate = SDIRXF_SDIMODE_12G; break;
    default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
    }
    RegData = SDIRXF_FrameProperties_SET_LinesPerFrame(0, NumLines);
    RegData = SDIRXF_FrameProperties_SET_SdiRate(RegData, FwSdiRate);
    RegData = SDIRXF_FrameProperties_SET_IsInterlaced(RegData, 
                                                          (Bool)AssumeInterlaced ? 1 : 0);

    SDIRXF_FrameProperties_WRITE(pBc, RegData);

    // Update cached frame properties
    pBc->m_NumSymsHanc = NumLines;
    pBc->m_SdiRate = SdiRate;
    pBc->m_AssumeInterlaced = (Bool)AssumeInterlaced;

    // Frame properties are set
    pBc->m_FramePropsSet = TRUE;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXF_SetOperationalMode(DtBcSDIRXF* pBc, Int OpMode)
{
    DtStatus Status;

    // Sanity check
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXF_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // IDLE/STANDBY -> RUN?
    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        // Check whether required parameters are set
        if (!pBc->m_FmtEvtTimingSet || !pBc->m_FramePropsSet)
        {
            DtDbgOutBc(ERR, SDIRXF, pBc, 
                            "Format event timing and/or FrameProperties are not yet set");
            return DT_STATUS_NOT_INITIALISED;
        }

        // Enable formatter event interrupt
        pBc->m_FmtIntEnabled = TRUE;
        Status = DtBc_InterruptEnable((DtBc*)pBc, pBc->m_FmtIntId);
        DT_ASSERT(DT_SUCCESS(Status));
    }
    // RUN -> IDLE or RUN -> STANDBY?
    else if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    {
        // Disable formatter event interrupt
        Status = DtBc_InterruptDisable((DtBc*)pBc,  pBc->m_FmtIntId);
        DT_ASSERT(DT_SUCCESS(Status));

        // Make sure that WaitForFmtEvent doesn't wait forever
        pBc->m_FmtIntEnabled = FALSE;
        DtEventSet(&pBc->m_FmtEvent);
    }
    else
    {
        // IDLE -> STANDBY
        // Check whether FrameProperties are set
        if (!pBc->m_FramePropsSet)
        {
            DtDbgOutBc(ERR, SDIRXF, pBc, "FrameProperties are not yet set");
            return DT_STATUS_NOT_INITIALISED;
        }
        
        // No further actions needed
    }

    // Set new control
     DtBcSDIRXF_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode);

    // Update cached operational mode
    pBc->m_OperationalMode = OpMode;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_WaitForFmtEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXF_WaitForFmtEvent(DtBcSDIRXF* pBc, Int Timeout, Int* pFrameId,
                                                            Int* pSeqNumber, Int* pInSync)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (Timeout<-1 || Timeout>1000)
        return DT_STATUS_INVALID_PARAMETER;
    if (pFrameId== NULL || pSeqNumber==NULL || pInSync==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXF_MUST_BE_ENABLED(pBc);

    // Clear the event in case we have a previous unhandled event
    DtEventReset(&pBc->m_FmtEvent);

    // Operational mode must be run and format event interrupts must be enabled
    if (pBc->m_OperationalMode!=DT_BLOCK_OPMODE_RUN || !pBc->m_FmtIntEnabled)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Wait for next formatter event interrupt
    Status = DtEventWaitUnInt(&pBc->m_FmtEvent, Timeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SDIRXF, pBc, "Wait on event failure, Status:0x%X", Status);
        return Status;
    }

    // Check again
    if (!pBc->m_FmtIntEnabled)
        return DT_STATUS_CANCELLED;

    // Copy the last formatter event interrupt data
    DtSpinLockAcquire(&pBc->m_IntDataSpinLock);
    *pFrameId = pBc->m_FrameId;
    *pSeqNumber = pBc->m_SeqNumber;
    *pInSync = (Int)pBc->m_InSync;
    DtSpinLockRelease(&pBc->m_IntDataSpinLock);

    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXF - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDIRXF_Init(DtBc*  pBcBase)
{
    DtBcSDIRXF* pBc = (DtBcSDIRXF*)pBcBase;
    DtStatus  Status = DT_STATUS_OK;
    UInt32  FwMaxSdiRate;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    DtBcSDIRXF_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    // Get maximum supported rate
    FwMaxSdiRate = SDIRXF_Config_READ_MaxSdiRate(pBc);
    switch (FwMaxSdiRate)
    {
    case SDIRXF_SDIMODE_SD:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_SD; break;
    case SDIRXF_SDIMODE_HD:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_HD; break;
    case SDIRXF_SDIMODE_3G:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_3G; break;
    case SDIRXF_SDIMODE_6G:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_6G; break;
    case SDIRXF_SDIMODE_12G:    pBc->m_MaxSdiRate = DT_DRV_SDIRATE_12G; break;
    default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
    }

    // Get stream-alignement
    pBc->m_StreamAlignment = 32;
    if (pBc->m_Version > 0)
    {
        UInt32  FwAlignment = SDIRXF_Config_READ_Alignment(pBc);
        switch (FwAlignment)
        {
        case SDIRXF_STREAMALIGNMENT_32b:   pBc->m_StreamAlignment = 32; break;
        case SDIRXF_STREAMALIGNMENT_64b:   pBc->m_StreamAlignment = 64; break;
        case SDIRXF_STREAMALIGNMENT_128b:  pBc->m_StreamAlignment = 128; break;
        case SDIRXF_STREAMALIGNMENT_256b:  pBc->m_StreamAlignment = 256; break;
        case SDIRXF_STREAMALIGNMENT_512b:  pBc->m_StreamAlignment = 512; break;
        default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
        }
    }
    // For format event timing and frame properties we don't have good defaults
    pBc->m_FramePropsSet = FALSE;
    pBc->m_FmtEvtTimingSet = FALSE;

    // Format event interrupts are disabled
    pBc->m_FmtIntEnabled = FALSE;

    // Initialize SpinLock
    DtSpinLockInit(&pBc->m_IntDataSpinLock);

    // Initialize formatter event
    Status = DtEventInit(&pBc->m_FmtEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SDIRXF, pBc, 
                              "ERROR: failed to init done event (Status=0x%08X)", Status);
        return Status;
    }

    // Init interrupt DPC
    Status = DtDpcInit(&pBc->m_IntDpc, DtBcSDIRXF_InterruptDpcFmtEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SDIRXF, pBc, "ERROR: failed to init DPC (Status=0x%08X)", Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Status = DtBcSDIRXF_RegisterIntHandlers(pBc);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SDIRXF, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXF_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSDIRXF* pBc = (DtBcSDIRXF*)pBcBase;

    // Sanity check
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, SDIRXF, pBc, "Block Enable from disable -> enable");
        
        // Enable block
        DtBcSDIRXF_SetControlRegs(pBc, Enable, pBc->m_OperationalMode);

        // For format event timing and frame properties we don't have good defaults
        BC_SDIRXF->m_FramePropsSet = FALSE;
        BC_SDIRXF->m_FmtEvtTimingSet = FALSE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, SDIRXF, pBc, "Block Enable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcSDIRXF_SetOperationalMode(BC_SDIRXF, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDIRXF, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }

        // Disable block
        DtBcSDIRXF_SetControlRegs(pBc, Enable, pBc->m_OperationalMode);
    }

    // Save new setting
    pBc->m_BlockEnabled = Enable;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDIRXF_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled(pBc))
    {
        DtDbgOutBc(AVG, SDIRXF, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcSDIRXF_SetOperationalMode(BC_SDIRXF, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDIRXF, pBc, "ERROR: failed to set opmode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_InterruptDpcFmtEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcSDIRXF_InterruptDpcFmtEvent(DtDpcArgs* pArgs)
{
    DtBcSDIRXF*  pBc = (DtBcSDIRXF*)pArgs->m_pContext;
    UInt32 RegFmtEvStatus;

    // Sanity check
#ifdef DEBUG
    UInt32  IntId = pArgs->m_Data1.m_UInt32_1;
    DT_ASSERT(IntId==DT_INTERRUPT_SDIRXF_FMTEVENT);
#endif  // #ifdef DEBUG
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    // Read formatter event interrupt status
    RegFmtEvStatus = SDIRXF_FmtEvStatus_READ(pBc);
    
    // Get the format event status
    DtSpinLockAcquire(&pBc->m_IntDataSpinLock);
    pBc->m_FrameId =  SDIRXF_FmtEvStatus_GET_FrameId(RegFmtEvStatus);
    pBc->m_SeqNumber = SDIRXF_FmtEvStatus_GET_SeqNum(RegFmtEvStatus);
    pBc->m_InSync = (SDIRXF_FmtEvStatus_GET_FmtSync(RegFmtEvStatus) 
                                                             == SDIRXF_FMTSYNC_IN_SYNC);
    DtSpinLockRelease(&pBc->m_IntDataSpinLock);

    // Signal formatter event
    DtEventSet(&pBc->m_FmtEvent);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDIRXF_InterruptHandler(DtBc*  pBc, Int  Id, Int  Index, void*  pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;

    // Sanity check
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);

    // Check ID is one of ours
    switch (Id)
    {
    case DT_INTERRUPT_SDIRXF_FMTEVENT:
        break;

        // Not a SDIRXF interrupt
    default:
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcSDIRXF*)pBc)->m_IntDpc, &DpcArgs);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDIRXF_RegisterIntHandlers(DtBcSDIRXF*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;
    Int  i=0;

    // Sanity check
    BC_SDIRXF_DEFAULT_PRECONDITIONS(pBc);


    // Register interrupt callbacks, but do not enable the interrupts. We will enable 
    // them on demand
    pBc->m_FmtIntId = -1;
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_SDIRXF_FMTEVENT:
            // Store format event interrupt ID 
            pBc->m_FmtIntId = Id;
            // NOTE: the SDIRXF uses one handler for all interrupt sources
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id, 
                                                     DtBcSDIRXF_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, SDIRXF, pBc,
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
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_CheckSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXF_CheckSdiRate(DtBcSDIRXF* pBc, Int SdiRate)
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSDIRXF_SetControlRegs(DtBcSDIRXF* pBc, Bool BlkEna, Int OpMode)
{
    UInt32 RegData=0, FwBlkEna=0, FwOpMode=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? SDIRXF_BLKENA_ENABLED : SDIRXF_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = SDIRXF_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = SDIRXF_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = SDIRXF_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control
    RegData = SDIRXF_Control_READ(pBc);
    RegData = SDIRXF_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = SDIRXF_Control_SET_OperationalMode(RegData, FwOpMode);
    SDIRXF_Control_WRITE(pBc, RegData);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIRXF implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcSDIRXF
#define SDIRXF_STUB   ((DtIoStubBcSDIRXF*)pStub)
#define SDIRXF_BC  ((DtBcSDIRXF*)SDIRXF_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcSDIRXF_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcSDIRXF_OnCmdGetFmtEventTiming(const DtIoStubBcSDIRXF*, 
                                                DtIoctlSdiRxFCmdGetFmtEventTimingOutput*);
static DtStatus  DtIoStubBcSDIRXF_OnCmdGetFrameProperties(const DtIoStubBcSDIRXF*,
                                               DtIoctlSdiRxFCmdGetFramePropertiesOutput*);
static DtStatus  DtIoStubBcSDIRXF_OnCmdGetMaxSdiRate(const DtIoStubBcSDIRXF*,
                                                    DtIoctlSdiRxFCmdGetMaxSdiRateOutput*);
static DtStatus  DtIoStubBcSDIRXF_OnCmdGetOperationalMode(const DtIoStubBcSDIRXF*,
                                                        DtIoctlSdiRxFCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcSDIRXF_OnCmdGetStreamAlignment(const DtIoStubBcSDIRXF*, 
                                               DtIoctlSdiRxFCmdGetStreamAlignmentOutput*);
static DtStatus  DtIoStubBcSDIRXF_OnCmdSetFmtEventTiming(const DtIoStubBcSDIRXF*,
                                          const  DtIoctlSdiRxFCmdSetFmtEventTimingInput*);
static DtStatus  DtIoStubBcSDIRXF_OnCmdSetFrameProperties(const DtIoStubBcSDIRXF*,
                                          const DtIoctlSdiRxFCmdSetFramePropertiesInput*);
static DtStatus  DtIoStubBcSDIRXF_OnCmdSetOperationalMode(const DtIoStubBcSDIRXF*,
                                                   const DtIoctlSdiRxFCmdSetOpModeInput*);
static DtStatus  DtIoStubBcSDIRXF_OnCmdWaitForFmtEvent(const DtIoStubBcSDIRXF*,
                                             const  DtIoctlSdiRxFCmdWaitForFmtEventInput*,
                                             DtIoctlSdiRxFCmdWaitForFmtEventOutput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SDIRXF;

static const DtIoctlProperties  IOSTUB_BC_SDIRXF_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_SDIRXF_CMD(
        DtIoStubBcSDIRXF_OnCmd,
        NULL,  NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIRXF - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcSDIRXF_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXF));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcSDIRXF*  DtIoStubBcSDIRXF_Open(DtBc*  pBc)
{
    DtIoStubBcSDIRXF*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDIRXF));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcSDIRXF, pBc, NULL, 
                                                             DtIoStubBcSDIRXF_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_SDIRXF_IOCTLS);
    pStub = (DtIoStubBcSDIRXF*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIRXF- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIRXF_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSdiRxFCmdInput*  pInData = NULL;
    DtIoctlSdiRxFCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXF));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_SDIRXF_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, SDIRXF, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SdiRxFCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SdiRxFCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_SDIRXF_CMD_GET_FMT_EVENT_TIMING:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIRXF_OnCmdGetFmtEventTiming(SDIRXF_STUB, 
                                                          &pOutData->m_GetFmtEventTiming);
        break;
    case DT_SDIRXF_CMD_GET_FRAME_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIRXF_OnCmdGetFrameProperties(SDIRXF_STUB, 
                                                              &pOutData->m_GetFrameProps);
        break;
    case DT_SDIRXF_CMD_GET_MAX_SDIRATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIRXF_OnCmdGetMaxSdiRate(SDIRXF_STUB, 
                                                              &pOutData->m_GetMaxSdiRate);
        break;
    case DT_SDIRXF_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIRXF_OnCmdGetOperationalMode(SDIRXF_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_SDIRXF_CMD_GET_STREAM_ALIGNMENT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIRXF_OnCmdGetStreamAlignment(SDIRXF_STUB, 
                                                               &pOutData->m_GetAlignment);
        break;
    case DT_SDIRXF_CMD_SET_FMT_EVENT_TIMING:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDIRXF_OnCmdSetFmtEventTiming(SDIRXF_STUB, 
                                                           &pInData->m_SetFmtEventTiming);
        break;
    case DT_SDIRXF_CMD_SET_FRAME_PROPERTIES:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDIRXF_OnCmdSetFrameProperties(SDIRXF_STUB, 
                                                              &pInData->m_SetFrameProps);
        break;
    case DT_SDIRXF_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDIRXF_OnCmdSetOperationalMode(SDIRXF_STUB,
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_SDIRXF_CMD_WAIT_FOR_FMT_EVENT:
        DT_ASSERT(pInData!=NULL && pOutData!=NULL);
        Status = DtIoStubBcSDIRXF_OnCmdWaitForFmtEvent(SDIRXF_STUB, 
                               &pInData->m_WaitForFmtEvent, &pOutData->m_WaitForFmtEvent);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF_OnCmdGetFmtEventTiming -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIRXF_OnCmdGetFmtEventTiming(
    const DtIoStubBcSDIRXF* pStub,
    DtIoctlSdiRxFCmdGetFmtEventTimingOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXF));
    DT_ASSERT(pOutData != NULL);

    // Get format event interrupt timing
    return DtBcSDIRXF_GetFmtEventTiming(SDIRXF_BC, &pOutData->m_InterruptInterval,
                            &pOutData->m_InterruptDelay, &pOutData->m_InterruptsPerFrame);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF_OnCmdGetFrameProperties -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIRXF_OnCmdGetFrameProperties(
    const DtIoStubBcSDIRXF* pStub,
    DtIoctlSdiRxFCmdGetFramePropertiesOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXF));
    DT_ASSERT(pOutData != NULL);

    // Get frame properties
    return DtBcSDIRXF_GetFrameProperties(SDIRXF_BC, &pOutData->m_NumSymsHanc,
                                    &pOutData->m_NumSymsVidVanc, &pOutData->m_NumLines, 
                                    &pOutData->m_SdiRate, &pOutData->m_AssumeInterlaced);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF_OnCmdGetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIRXF_OnCmdGetMaxSdiRate(
    const DtIoStubBcSDIRXF* pStub,
    DtIoctlSdiRxFCmdGetMaxSdiRateOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXF));
    DT_ASSERT(pOutData != NULL);

    // Get maximum SDI-rate
    return DtBcSDIRXF_GetMaxSdiRate(SDIRXF_BC, &pOutData->m_MaxSdiRate);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIRXF_OnCmdGetOperationalMode(
    const DtIoStubBcSDIRXF* pStub,
    DtIoctlSdiRxFCmdGetOpModeOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXF));
    DT_ASSERT(pOutData != NULL);

    // Get operational Mode
    return DtBcSDIRXF_GetOperationalMode(SDIRXF_BC, &pOutData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF_OnCmdGetStreamAlignment -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIRXF_OnCmdGetStreamAlignment(
    const DtIoStubBcSDIRXF* pStub,
    DtIoctlSdiRxFCmdGetStreamAlignmentOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXF));
    DT_ASSERT(pOutData != NULL);

    // Get maximum SDI-rate
    return DtBcSDIRXF_GetStreamAlignment(SDIRXF_BC, &pOutData->m_StreamAlignment);
}


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF_OnCmdSetFmtEventTiming -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIRXF_OnCmdSetFmtEventTiming(
    const DtIoStubBcSDIRXF* pStub,
    const DtIoctlSdiRxFCmdSetFmtEventTimingInput* pInData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXF));
    DT_ASSERT(pInData != NULL);

    // Set format event interrupt timing
    return DtBcSDIRXF_SetFmtEventTiming(SDIRXF_BC, pInData->m_InterruptInterval,
                                pInData->m_InterruptDelay, pInData->m_InterruptsPerFrame);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF_OnCmdSetFrameProperties -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIRXF_OnCmdSetFrameProperties(
    const DtIoStubBcSDIRXF* pStub,
    const DtIoctlSdiRxFCmdSetFramePropertiesInput* pInData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXF));
    DT_ASSERT(pInData != NULL);

    // Set frame properties
    return DtBcSDIRXF_SetFrameProperties(SDIRXF_BC, pInData->m_NumSymsHanc,
                                        pInData->m_NumSymsVidVanc, pInData->m_NumLines, 
                                        pInData->m_SdiRate, pInData->m_AssumeInterlaced);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIRXF_OnCmdSetOperationalMode(
    const DtIoStubBcSDIRXF* pStub,
    const DtIoctlSdiRxFCmdSetOpModeInput* pInData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXF));
    DT_ASSERT(pInData != NULL);

    // Set operational mode
    return DtBcSDIRXF_SetOperationalMode(SDIRXF_BC, pInData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF_OnCmdWaitForFmtEvent -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIRXF_OnCmdWaitForFmtEvent(
    const DtIoStubBcSDIRXF* pStub,
    const DtIoctlSdiRxFCmdWaitForFmtEventInput* pInData,
    DtIoctlSdiRxFCmdWaitForFmtEventOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIRXF));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    // Set data timeout
    return DtBcSDIRXF_WaitForFmtEvent(SDIRXF_BC, pInData->m_Timeout,
                                       &pOutData->m_FrameId, &pOutData->m_SeqNumber, 
                                       &pOutData->m_InSync);
}
