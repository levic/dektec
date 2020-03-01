// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcLMH1981.c *#*#*#*#*#*#*#*#*#*# (C) 2019 DekTec
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
#include "DtBcLMH1981.h"
#include "DtBcLMH1981_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLMH1981 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_LMH1981_DEFAULT_PRECONDITIONS(pBc)      \
                                  DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcLMH1981))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_LMH1981_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(LMH1981, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcLMH1981_Init(DtBc*);
static DtStatus  DtBcLMH1981_OnEnable(DtBc*, Bool Enable);
static DtStatus DtBcLMH1981_RegisterIntHandlers(DtBcLMH1981*);
static void  DtBcLMH1981_InterruptDpcStartOfFrame(DtDpcArgs*);
static DtStatus  DtBcLMH1981_InterruptHandler(DtBc*, Int Id, Int Index, void * pContext);
static Int  DtBcLMH1981_DetermineVidSTd(DtBcLMH1981* pBc);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLMH1981 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcLMH1981_Close(DtBc*  pBc)
{
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcLMH1981*  DtBcLMH1981_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // Init open parameters
    DT_BC_LMH1981_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcLMH1981, Id, DT_BLOCK_TYPE_LMH1981,
                           Address, pPt, CreateStub, pCore);
// Register the callbacks
    OpenParams.m_CloseFunc = DtBcLMH1981_Close;
    OpenParams.m_InitFunc = DtBcLMH1981_Init;
    OpenParams.m_OnEnableFunc = DtBcLMH1981_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcLMH1981*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_GetInputStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcLMH1981_GetInputStatus(DtBcLMH1981 * pBc, DtBcLMH1981_Status * pStatus)
{
    UInt32  RegData;
    Int* pVf;

    // Sanity check
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pStatus == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_LMH1981_MUST_BE_ENABLED(pBc);

    // Determine Status
    RegData = LMH1981_SyncRate_READ(pBc);
    pStatus->m_Valid = LMH1981_SyncRate_GET_HsRate(RegData) != 0;
    // Line period and Field period in nano seconds
    pStatus->m_LinePeriod = (LMH1981_SyncRate_GET_HsRate(RegData) * 1000)/256;
    pStatus->m_FramePeriod = LMH1981_SyncRate_GET_VsRate(RegData) * 1000;
    RegData = LMH1981_NumLines_READ(pBc);
    pStatus->m_NumLinesF1 = LMH1981_NumLines_GET_Field1(RegData);
    pStatus->m_NumLinesF2 = LMH1981_NumLines_GET_Field2(RegData);
    RegData = LMH1981_VideoFormat_READ(pBc);
    pVf = &pStatus->m_VideoFormat;
    switch (RegData)
    {
    case LMH1981_FORMAT_NtscF1:  *pVf = DT_LMH1981_FORMAT_525I_F1; break;
    case LMH1981_FORMAT_NtscF2:  *pVf = DT_LMH1981_FORMAT_525I_F2; break;
    case LMH1981_FORMAT_PalF1:   *pVf = DT_LMH1981_FORMAT_625I_F1; break;
    case LMH1981_FORMAT_PalF2:   *pVf = DT_LMH1981_FORMAT_625I_F2; break;
    case LMH1981_FORMAT_480p:    *pVf = DT_LMH1981_FORMAT_480P; break;
    case LMH1981_FORMAT_576p:    *pVf = DT_LMH1981_FORMAT_576P; break;
    case LMH1981_FORMAT_720p:    *pVf = DT_LMH1981_FORMAT_720P; break;
    case LMH1981_FORMAT_1080iF1: *pVf = DT_LMH1981_FORMAT_1080I_F1; break;
    case LMH1981_FORMAT_1080iF2: *pVf = DT_LMH1981_FORMAT_1080I_F2; break;
    case LMH1981_FORMAT_1080p:   *pVf = DT_LMH1981_FORMAT_1080P; break;
    default:                     *pVf = DT_LMH1981_FORMAT_UNKNOWN; break;
    }
    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_GetGenRefEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcLMH1981_GetGenRefEnable(DtBcLMH1981* pBc, Bool* pEnable)
{
    // Sanity check
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);
    // Check parameter
    if (pEnable == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    // Must be enabled
    BC_LMH1981_MUST_BE_ENABLED(pBc);
    // Return cached value
    *pEnable = pBc->m_GenRefEnabled;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_getGenRefSofOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcLMH1981_getGenRefSofOffset(DtBcLMH1981* pBc, Int* pOffsetNs)
{
    // Sanity check
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);
    // Check parameter
    if (pOffsetNs == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    // Must be enabled
    BC_LMH1981_MUST_BE_ENABLED(pBc);
    // Return cached value
    *pOffsetNs = pBc->m_GenRefSofOffset;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_GetGenRefVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcLMH1981_GetGenRefVidStd(DtBcLMH1981 * pBc, Int * pVidStd)
{
    // Sanity check
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);
    // Check parameter
    if (pVidStd == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    // Must be enabled
    BC_LMH1981_MUST_BE_ENABLED(pBc);
    // Return cached value
    *pVidStd = pBc->m_GenRefVidStd;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_SetGenRefEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcLMH1981_SetGenRefEnable(DtBcLMH1981* pBc, Bool Enable)
{
    DtStatus Status = DT_STATUS_OK;
    // Sanity check
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_LMH1981_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_GenRefEnabled == Enable)
        return DT_STATUS_OK;

    // Change GenRefEnable under spinlock protection
    DtSpinLockAcquire(&pBc->m_Lock);
    // Change Operational Mode
    if (Enable)
    { 
        // Enable interrupt
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_LMH1981_STARTOFFRAME);
    }
    else
    {
        DtTodTime  DumySof = {0, 0};
        // Disable interrupt
        Status = DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_LMH1981_STARTOFFRAME);
        // Send one final event
        if (pBc->m_pDfGenLockCtrl != NULL)
            DtDfGenLockCtrl_GenRefStartOfFrameHandler(pBc->m_pDfGenLockCtrl, 
                                                     pBc->m_PortIndex, &DumySof, 0,
                                                     DT_VIDSTD_UNKNOWN, DT_VIDSTD_UNKNOWN,
                                                     DT_DF_GENLOCKCTRL_GENREF_UNDEFINED);
    }
    // Cache value
    pBc->m_GenRefEnabled = Enable;
    DtSpinLockRelease(&pBc->m_Lock);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_SetGenRefSofOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcLMH1981_SetGenRefSofOffset(DtBcLMH1981* pBc, Int OffsetNs)
{
    DtStatus Status = DT_STATUS_OK;
    // Sanity check
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_LMH1981_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_GenRefSofOffset == OffsetNs)
        return DT_STATUS_OK;

    // Change genref start-of-frame offset under spinlock protection
    DtSpinLockAcquire(&pBc->m_Lock);
    pBc->m_GenRefSofOffset = OffsetNs;
    DtSpinLockRelease(&pBc->m_Lock);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_SetGenRefVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcLMH1981_SetGenRefVidStd(DtBcLMH1981 * pBc, Int VidStd)
{
    DtStatus Status = DT_STATUS_OK;
    // Sanity check
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_LMH1981_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_GenRefVidStd == VidStd)
        return DT_STATUS_OK;

    // Change genref video standard under spinlock protection
    DtSpinLockAcquire(&pBc->m_Lock);
    pBc->m_GenRefVidStd = VidStd;
    DtSpinLockRelease(&pBc->m_Lock);
    return Status;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLMH1981 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcLMH1981_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcLMH1981* pBc = (DtBcLMH1981*)pBcBase;

    // Init spinlock
    DtSpinLockInit(&pBc->m_Lock);

    // Save port index
    pBc->m_PortIndex = DtCore_PT_GetPortIndex(pBc->m_pPt);
    
    // Set defaults
    pBc->m_GenRefVidStd = DT_VIDSTD_UNKNOWN;
    pBc->m_GenRefSofOffset = 0;
    pBc->m_GenRefEnabled = FALSE;

    // Init interrupt DPC
    Status = DtDpcInit(&pBc->m_IntDpc, DtBcLMH1981_InterruptDpcStartOfFrame, TRUE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, LMH1981, pBc, "ERROR: failed to init DPC (Status=0x%08X)",
                                                                                  Status);
        return Status;
    }

    // Register interrupt handlers
    Status = DtBcLMH1981_RegisterIntHandlers(pBc);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, LMH1981, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    // Find the device level genlock controller
    pBc->m_pDfGenLockCtrl = (DtDfGenLockCtrl*)DtCore_DF_Find(pBc->m_pCore, NULL,
                                                             DT_FUNC_TYPE_GENLOCKCTRL, NULL);

    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcLMH1981_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcLMH1981* pBc = (DtBcLMH1981*)pBcBase;

    // Sanity check
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, LMH1981, pBc, "OnEnable from disable -> enable");

    } else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, LMH1981, pBc, "OnEnable from enable -> disable");

        // Disable genref
        Status = DtBcLMH1981_SetGenRefEnable(pBc, FALSE);
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcLMH1981_RegisterIntHandlers(DtBcLMH1981* pBc)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i = 0;

    // Sanity check
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks, but do not enable the interrupts. We will enable 
    // them on demand
    for (i = 0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch (Id)
        {
        case DT_INTERRUPT_LMH1981_STARTOFFRAME:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id,
                                             DtBcLMH1981_InterruptHandler, NULL);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, LMH1981, pBc,
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

// .-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_InterruptDpcStartOfFrame -.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcLMH1981_InterruptDpcStartOfFrame(DtDpcArgs* pArgs)
{
    DtBcLMH1981*  pBc = (DtBcLMH1981*)pArgs->m_pContext;
    DtTodTime  SofTod = {pArgs->m_Data2.m_UInt32_1, pArgs->m_Data2.m_UInt32_2};
    Int  ConfigVidStd = pBc->m_GenRefVidStd;
    Int  DetectVidStd = DT_VIDSTD_UNKNOWN;
    Int HSyncPropDelay = 0;

    // Sanity check
#ifdef DEBUG
    UInt32  IntId = pArgs->m_Data1.m_UInt32_1;
    DT_ASSERT(IntId == DT_INTERRUPT_LMH1981_STARTOFFRAME);
#endif  // #ifdef DEBUG
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);


    // Determine detected video standard
    DetectVidStd = DtBcLMH1981_DetermineVidSTd(pBc);

    // HSync propagation delay depends on the video standard 
    switch (DetectVidStd)
    {
        case DT_VIDSTD_525I59_94:   HSyncPropDelay = 40; break;
        case DT_VIDSTD_625I50:      HSyncPropDelay = 60; break;
        case DT_VIDSTD_480P59_94:   HSyncPropDelay = 35; break;
        default:                    HSyncPropDelay = 30; break;
    }

    // Additional 30ns delay for clockconversion etc
    HSyncPropDelay += 30;

    // Adjust SOF timestamp
    SofTod = DtCore_TOD_Add(SofTod, -HSyncPropDelay);

    // Call start-of-frame handler under spinlock protection
    DtSpinLockAcquire(&pBc->m_Lock);
    if (pBc->m_GenRefEnabled && pBc->m_pDfGenLockCtrl!=NULL)
    {
        DtDfGenLockCtrl_GenRefStartOfFrameHandler(pBc->m_pDfGenLockCtrl, pBc->m_PortIndex,
                                                      &SofTod, pBc->m_GenRefSofOffset,
                                                      ConfigVidStd, DetectVidStd,
                                                      DT_DF_GENLOCKCTRL_GENREF_LMH1981);
    }
    DtSpinLockRelease(&pBc->m_Lock);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcLMH1981_InterruptHandler(DtBc* pBcBase, Int Id, Int Index, void* pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcLMH1981* pBc = (DtBcLMH1981*)pBcBase;
    DtDpcArgs  DpcArgs;

    // Sanity check
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);

    // Check ID is one of ours
    switch (Id)
    {
    case DT_INTERRUPT_LMH1981_STARTOFFRAME:
        break;

       // Not a LMH1981 interrupt
    default:
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    DpcArgs.m_Data2.m_UInt32_1 = LMH1981_SofTimestampSec_READ(pBc);
    DpcArgs.m_Data2.m_UInt32_2 = LMH1981_SofTimestampNanosec_READ(pBc);

    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&pBc->m_IntDpc, &DpcArgs);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_DetermineVidSTd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtBcLMH1981_DetermineVidSTd(DtBcLMH1981* pBc)
{
    UInt32 RegData;
    Int FramePeriodUs, FpsX1000;
    Int VidStd = DT_VIDSTD_UNKNOWN;

    // Determine frame period and frame rate
    RegData = LMH1981_SyncRate_READ(pBc);
    FramePeriodUs = (Int)LMH1981_SyncRate_GET_VsRate(RegData);
    if (FramePeriodUs <= 0)
        return DT_VIDSTD_UNKNOWN;

    FpsX1000 = 1000*1000*1000L/FramePeriodUs;

    switch (LMH1981_VideoFormat_READ(pBc))
    {
    case LMH1981_FORMAT_NtscF1:
    case LMH1981_FORMAT_NtscF2:
        VidStd = DT_VIDSTD_525I59_94;
        break;

    case LMH1981_FORMAT_PalF1:
    case LMH1981_FORMAT_PalF2:
        VidStd = DT_VIDSTD_625I50;
        break;

    case LMH1981_FORMAT_480p:
        VidStd = DT_VIDSTD_480P59_94;
        break;

    case LMH1981_FORMAT_576p:
        // Not supported
        break;

    case LMH1981_FORMAT_720p:
        if (FpsX1000>=59970 && FpsX1000<60030)
            VidStd = DT_VIDSTD_720P60;
        else if (FpsX1000>=59910 && FpsX1000<59970)
            VidStd = DT_VIDSTD_720P59_94;
        else if (FpsX1000>=49975 && FpsX1000<50025)
            VidStd = DT_VIDSTD_720P50;
        else if (FpsX1000>=29985 && FpsX1000<30015)
            VidStd = DT_VIDSTD_720P30;
        else if (FpsX1000>=29955 && FpsX1000<29985)
            VidStd = DT_VIDSTD_720P29_97;
        else if (FpsX1000>=24987 && FpsX1000<25012)
            VidStd = DT_VIDSTD_720P25;
        else if (FpsX1000>=23988 && FpsX1000<24012)
            VidStd = DT_VIDSTD_720P24;
        else if (FpsX1000>=23964 && FpsX1000<23988)
            VidStd = DT_VIDSTD_720P23_98;
        break;

    case LMH1981_FORMAT_1080iF1:
    case LMH1981_FORMAT_1080iF2:
        FpsX1000 = FpsX1000/2;  // Check on fields per second
        if (FpsX1000>=59970 && FpsX1000<60030)
            VidStd = DT_VIDSTD_1080I60;
        else if (FpsX1000>=59910 && FpsX1000<59970)
            VidStd = DT_VIDSTD_1080I59_94;
        else if (FpsX1000>=49975 && FpsX1000<50025)
            VidStd = DT_VIDSTD_1080I50;
        else if (FpsX1000>=29985 && FpsX1000<30015)
            VidStd = DT_VIDSTD_1080PSF30;
        else if (FpsX1000>=29955 && FpsX1000<29985)
            VidStd = DT_VIDSTD_1080PSF29_97;
        else if (FpsX1000>=24987 && FpsX1000<25012)
            VidStd = DT_VIDSTD_1080PSF25;
        else if (FpsX1000>=23988 && FpsX1000<24012)
            VidStd = DT_VIDSTD_1080PSF24;
        else if (FpsX1000>=23964 && FpsX1000<23988)
            VidStd = DT_VIDSTD_1080PSF23_98;
        break;

    case LMH1981_FORMAT_1080p:
        if (FpsX1000>=59970 && FpsX1000<60030)
            VidStd = DT_VIDSTD_1080P60;
        else if (FpsX1000>=59910 && FpsX1000<59970)
            VidStd = DT_VIDSTD_1080P59_94;
        else if (FpsX1000>=49975 && FpsX1000<50025)
            VidStd = DT_VIDSTD_1080P50;
        else if (FpsX1000>=29985 && FpsX1000<30015)
            VidStd = DT_VIDSTD_1080P30;
        else if (FpsX1000>=29955 && FpsX1000<29985)
            VidStd = DT_VIDSTD_1080P29_97;
        else if (FpsX1000>=24987 && FpsX1000<25012)
            VidStd = DT_VIDSTD_1080P25;
        else if (FpsX1000>=23988 && FpsX1000<24012)
            VidStd = DT_VIDSTD_1080P24;
        else if (FpsX1000>=23964 && FpsX1000<23988)
            VidStd = DT_VIDSTD_1080P23_98;
        break;
    }
    return VidStd;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLMH1981 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define LMH1981_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcLMH1981))
// Helper macro to cast stub's DtBc member to DtBcLMH1981
#define LMH1981_STUB   ((DtIoStubBcLMH1981*)pStub)
#define LMH1981_BC  ((DtBcLMH1981*)LMH1981_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcLMH1981_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcLMH1981_OnCmdGetInputStatus(
        const DtIoStubBcLMH1981*,
        DtIoctlLmh1981CmdGetInputStatusOutput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
    DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
    DECL_DT_IOCTL_CMD_PROPS_LMH1981;

    static const DtIoctlProperties  IOSTUB_BC_LMH1981_IOCTLS[] =
    {
        DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
            DtIoStubBc_OnExclAccessCmd,     // Use default handler
            NULL, NULL),
        DT_IOCTL_PROPS_LMH1981_CMD(
            DtIoStubBcLMH1981_OnCmd,
            NULL, NULL),
    };

    //=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLMH1981 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLMH1981_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    //
    void  DtIoStubBcLMH1981_Close(DtIoStub* pStub)
    {
        LMH1981_STUB_DEFAULT_PRECONDITIONS(pStub);

        // Let base function perform final clean-up
        DtIoStubBc_Close(pStub);
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLMH1981_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    //
    DtIoStubBcLMH1981*  DtIoStubBcLMH1981_Open(DtBc*  pBc)
    {
        DtIoStubBcOpenParams  OpenParams;

        BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);

        // Use base function to allocate and perform standard initialisation of stub data
        DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcLMH1981, pBc, NULL,
                                     DtIoStubBcLMH1981_Close,
                                     NULL,  // Use default IOCTL
                                     IOSTUB_BC_LMH1981_IOCTLS);
        return (DtIoStubBcLMH1981*)DtIoStubBc_Open(&OpenParams);
    }

    // =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLMH1981 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=


    // -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLMH1981_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    //
    DtStatus  DtIoStubBcLMH1981_OnCmd(const DtIoStub*  pStub,
                                      DtIoStubIoParams*  pIoParams, Int*  pOutSize)
    {
        DtStatus Status = DT_STATUS_OK;
        DtIoctlLmh1981CmdOutput*  pOutData = NULL;

        LMH1981_STUB_DEFAULT_PRECONDITIONS(pStub);
        DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
        DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_LMH1981_CMD);

        // Do we need exlusive access?
        if (pIoParams->m_ExclAccessIsRequired)
        {
            Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
                                          &pIoParams->m_ExclAccessObj);
            if (Status != DT_STATUS_OK)
            {
                DtDbgOutIoStubBc(ERR, LMH1981, pStub, "ERROR: block is not locked by me");
                return Status;
            }
        }

        // Get in-/out-data
        DT_ASSERT(pIoParams->m_pInData != NULL);
        if (pIoParams->m_OutReqSize > 0)
        {
            DT_ASSERT(pIoParams->m_pOutData != NULL);
            pOutData = &pIoParams->m_pOutData->m_Lmh1981Cmd;
        }

        //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

        switch (pIoParams->m_Cmd)
        {
        case DT_LMH1981_CMD_GET_INPUT_STATUS:
            DT_ASSERT(pOutData != NULL);
            Status = DtIoStubBcLMH1981_OnCmdGetInputStatus(LMH1981_STUB,
                                                           &pOutData->m_GetInpStatus);
            break;
        default:
            DT_ASSERT(FALSE);
            return DT_STATUS_NOT_SUPPORTED;
        }
        return Status;
    }

    // .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLMH1981_OnCmdGetInputStatus -.-.-.-.-.-.-.-.-.-.-.-.
    //
    DtStatus  DtIoStubBcLMH1981_OnCmdGetInputStatus(
        const DtIoStubBcLMH1981* pStub,
        DtIoctlLmh1981CmdGetInputStatusOutput* pOutData)
    {
        DtStatus  Status = DT_STATUS_OK;
        DtBcLMH1981_Status  InpStatus;
        LMH1981_STUB_DEFAULT_PRECONDITIONS(pStub);
        DT_ASSERT(pOutData != NULL);

        Status = DtBcLMH1981_GetInputStatus(LMH1981_BC, &InpStatus);
        if (DT_SUCCESS(Status))
        {
            pOutData->m_Valid = (Int)(InpStatus.m_Valid);
            pOutData->m_LinePeriod = InpStatus.m_LinePeriod;
            pOutData->m_FramePeriod = InpStatus.m_FramePeriod;
            pOutData->m_NumLinesF1 = InpStatus.m_NumLinesF1;
            pOutData->m_NumLinesF2 = InpStatus.m_NumLinesF2;
            pOutData->m_VideoFormat = InpStatus.m_VideoFormat;
        }
        return Status;
    }
