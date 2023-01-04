// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcGENL.c *#*#*#*#*#*#*#*#*#*#*# (C) 2019 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2019 DekTec Digital Video B.V.
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
#include "DtBcGENL.h"
#include "DtBcGENL_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcGENL implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_GENL_DEFAULT_PRECONDITIONS(pBc)      \
                                     DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcGENL))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_GENL_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(GENL, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcGENL_Init(DtBc*);
static DtStatus  DtBcGENL_OnEnable(DtBc*, Bool  Enable);
static DtStatus  DtBcGENL_RegisterIntHandlers(DtBcGENL*);
static void  DtBcGENL_InterruptDpcStartOfFrame(DtDpcArgs* pArgs);
static DtStatus  DtBcGENL_InterruptHandler(DtBc*, Int, Int, void*);
static void  DtBcGENL_SetControlRegs(DtBcGENL*, Bool BlkEna, Int OpMode, Bool FracClk,
                                                                       Bool ArmCrashLock);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcGENL - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcGENL_Close(DtBc*  pBcBase)
{
    DtBcGENL* pBc = (DtBcGENL*)pBcBase;

    BC_GENL_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBcBase));

    // Clean-up vector of start-of-frame handlers
    if (pBc->m_pOnStartOfFrameHandlers != NULL)
    {
        DtVector_Cleanup(pBc->m_pOnStartOfFrameHandlers);
        pBc->m_pOnStartOfFrameHandlers = NULL;
    }
    // Let base function perform final clean-up
    DtBc_Close(pBcBase);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcGENL*  DtBcGENL_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_GENL_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcGENL, Id, DT_BLOCK_TYPE_GENL,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcGENL_Close;
    OpenParams.m_InitFunc = DtBcGENL_Init;
    OpenParams.m_OnEnableFunc = DtBcGENL_OnEnable;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcGENL*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_GetCrashLockStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcGENL_GetCrashLockStatus(DtBcGENL* pBc, Int* pClStatus)
{
    UInt32  FwStatus;
    // Sanity check
    BC_GENL_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pClStatus == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_GENL_MUST_BE_ENABLED(pBc);

    FwStatus = GENL_Status_READ_CrashLockStatus(pBc);
    *pClStatus = DT_BC_GENL_CRASHLOCK_NORMAL;
    switch (FwStatus)
    {
    case GENL_CRASHLOCK_NORMAL: *pClStatus = DT_BC_GENL_CRASHLOCK_NORMAL; break;
    case GENL_CRASHLOCK_ARMED:  *pClStatus = DT_BC_GENL_CRASHLOCK_ARMED; break;
    case GENL_CRASHLOCK_ADJUST: *pClStatus = DT_BC_GENL_CRASHLOCK_ADJUST; break;
    default:
        DT_ASSERT(FALSE); 
        break;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcGENL_GetOperationalMode(DtBcGENL* pBc, Int* pOpMode)
{
    // Sanity check
    BC_GENL_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_GENL_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_SetCrashLockAdjust -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcGENL_SetCrashLockAdjust(DtBcGENL* pBc, Int AdjustLength)
{
    // Sanity check
    BC_GENL_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (AdjustLength<=0 && AdjustLength>=(1<<25))
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_GENL_MUST_BE_ENABLED(pBc);

    // Must be in RUN
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Set adjustment length
    GENL_AdjustLength_WRITE(pBc,AdjustLength);

    // Set control registers
    DtBcGENL_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                            pBc->m_FractionalClock, TRUE);

    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcGENL_SetOperationalMode(DtBcGENL* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_GENL_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_GENL_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Change operational mode under spinlock protection
    DtSpinLockAcquire(&pBc->m_Lock);

    // Change Operational Mode
    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        // IDLE -> RUN
        // Enable interrupt
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_GENL_STARTOFFRAME);
    }
    else if (OpMode == DT_BLOCK_OPMODE_IDLE)
    {
        // RUN -> IDLE
        // Disable interrupt
        Status = DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_GENL_STARTOFFRAME);
    }

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcGENL_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                           pBc->m_FractionalClock, FALSE);
    DtSpinLockRelease(&pBc->m_Lock);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_SetFrameLength -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcGENL_SetFrameLength(DtBcGENL* pBc, Int NumClockTicks, Bool FracClk)
{
    // Sanity check
    BC_GENL_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (NumClockTicks<=0 && NumClockTicks>=(1<<25))
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_GENL_MUST_BE_ENABLED(pBc);

    // Must be in IDLE
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // No change?
    if (FracClk==pBc->m_FractionalClock && NumClockTicks==pBc->m_FrameLength)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_FractionalClock = FracClk;
    pBc->m_FrameLength = NumClockTicks;
    
    // Set control registers
    DtBcGENL_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                           pBc->m_FractionalClock, FALSE);
    GENL_FrameLength_WRITE(pBc, pBc->m_FrameLength);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_StartOfFrameRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcGENL_StartOfFrameRegister(DtBcGENL* pBc,
                                            const DtBcGENLOnStartOfFrameRegData* pRegData)
{
    DtStatus Status = DT_STATUS_OK;
    Int  i, NumElems = 0;

    // Sanity check
    BC_GENL_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pBc->m_pOnStartOfFrameHandlers != NULL);
    
    // Parameter check
    if (pRegData==NULL || pRegData->m_pObject==NULL || 
                               (pRegData->m_pObject->m_ObjectType!=DT_OBJECT_TYPE_BC &&
                                    pRegData->m_pObject->m_ObjectType!=DT_OBJECT_TYPE_DF))
        return DT_STATUS_INVALID_PARAMETER;

    // Update list under spinlock protection
    DtSpinLockAcquire(&pBc->m_Lock);

    // Check if we already have an entry for this object
    NumElems = DtVector_Size(pBc->m_pOnStartOfFrameHandlers);
    for (i=0; i<NumElems; i++)
    {
        DtBcGENLOnStartOfFrameRegData*  p = 
           (DtBcGENLOnStartOfFrameRegData*)DtVector_At(pBc->m_pOnStartOfFrameHandlers, i);
        if (p->m_pObject != pRegData->m_pObject)
            continue;

        // Overwrite existing element
        p->m_OnStartOfFrameFunc = pRegData->m_OnStartOfFrameFunc;
        DtSpinLockRelease(&pBc->m_Lock);
        return DT_STATUS_OK;
    }
    // Add new element
    Status = DtVector_PushBack(pBc->m_pOnStartOfFrameHandlers, pRegData);
    DtSpinLockRelease(&pBc->m_Lock);
    return Status;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_StartOfFrameUnregister -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcGENL_StartOfFrameUnregister(DtBcGENL* pBc, const DtObject* pObject)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i, NumElems = 0;

    // Sanity check
    BC_GENL_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pBc->m_pOnStartOfFrameHandlers != NULL);
    
    // Parameter check
    if (pObject==NULL || (pObject->m_ObjectType!=DT_OBJECT_TYPE_BC &&
                                                pObject->m_ObjectType!=DT_OBJECT_TYPE_DF))
        return DT_STATUS_INVALID_PARAMETER;

    // Update list under spinlock protection
    DtSpinLockAcquire(&pBc->m_Lock);

    // Find entry for the specified object and erase it
    NumElems = DtVector_Size(pBc->m_pOnStartOfFrameHandlers);
    for (i=0; i<NumElems; i++)
    {
        const DtBcGENLOnStartOfFrameRegData*  p = 
           (DtBcGENLOnStartOfFrameRegData*)DtVector_At(pBc->m_pOnStartOfFrameHandlers, i);
        if (p->m_pObject != pObject)
            continue;

        // Erase the element
        Status = DtVector_Erase(pBc->m_pOnStartOfFrameHandlers, i);
        DtSpinLockRelease(&pBc->m_Lock);
        return Status;
    }
    DtSpinLockRelease(&pBc->m_Lock);
    return DT_STATUS_NOT_FOUND;
}


// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcGENL - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcGENL_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcGENL* pBc = (DtBcGENL*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_FrameLength =    1125*5280;     // 25fps
    pBc->m_FractionalClock = FALSE;
    DtSpinLockInit(&pBc->m_Lock);

    // Set control registers
    DtBcGENL_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                           pBc->m_FractionalClock, FALSE);
    GENL_FrameLength_WRITE(pBc, pBc->m_FrameLength);

    // Create the start of frame registration list
    DT_ASSERT(pBc->m_pOnStartOfFrameHandlers == NULL);
    pBc->m_pOnStartOfFrameHandlers = 
                             DtVector_Create(2, sizeof(DtBcGENLOnStartOfFrameRegData), 2);
    if (pBc->m_pOnStartOfFrameHandlers == NULL)
    {
        DtDbgOutBc(ERR, GENL, pBc,  "ERROR: failed to create int-handler list");
        return DT_STATUS_OUT_OF_MEMORY;
    }

    // Init interrupt DPC
    Status = DtDpcInit(&pBc->m_IntDpc, DtBcGENL_InterruptDpcStartOfFrame, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, GENL, pBc, "ERROR: failed to init DPC (Status=0x%08X)", Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Status = DtBcGENL_RegisterIntHandlers(pBc);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, GENL, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcGENL_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcGENL* pBc = (DtBcGENL*)pBcBase;

    // Sanity check
    BC_GENL_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, GENL, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, GENL, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcGENL_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, GENL, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }

        // Clear list of  start-of-frame handlers
        DtSpinLockAcquire(&pBc->m_Lock);
        DtVector_Clear(pBc->m_pOnStartOfFrameHandlers);
        DtSpinLockRelease(&pBc->m_Lock);
    }
    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control registers
    DtBcGENL_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                           pBc->m_FractionalClock, FALSE);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcGENL_RegisterIntHandlers(DtBcGENL* pBc)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i = 0;

    // Sanity check
    BC_GENL_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks, but do not enable the interrupts. We will enable 
    // them on demand
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_GENL_STARTOFFRAME:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id, 
                                                     DtBcGENL_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, GENL, pBc,
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

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_InterruptDpcStartOfFrame -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcGENL_InterruptDpcStartOfFrame(DtDpcArgs* pArgs)
{
    DtBcGENL*  pBc = (DtBcGENL*)pArgs->m_pContext;
    DtTodTime  SofTod =  {pArgs->m_Data2.m_UInt32_1, pArgs->m_Data2.m_UInt32_2};
    Int i, NumElems;

    // Sanity check
#ifdef DEBUG
    UInt32  IntId = pArgs->m_Data1.m_UInt32_1;
    DT_ASSERT(IntId == DT_INTERRUPT_GENL_STARTOFFRAME);
#endif  // #ifdef DEBUG
    BC_GENL_DEFAULT_PRECONDITIONS(pBc);

    // Call all start-of-frame handlers under spinlock protection
    DtSpinLockAcquire(&pBc->m_Lock);

    // Must be in RUN
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
    { 
        DtSpinLockRelease(&pBc->m_Lock);
        return;
    }

    // Notify all
    NumElems = DtVector_Size(pBc->m_pOnStartOfFrameHandlers);
    for (i=0; i<NumElems; i++)
    {
        const DtBcGENLOnStartOfFrameRegData*  pRegData = 
           (DtBcGENLOnStartOfFrameRegData*)DtVector_At(pBc->m_pOnStartOfFrameHandlers, i);
        DT_ASSERT(pRegData!=NULL && pRegData->m_OnStartOfFrameFunc!=NULL);
        if (pRegData==NULL || pRegData->m_OnStartOfFrameFunc==NULL)
            continue;
        DT_ASSERT(pRegData->m_pObject != NULL);
        pRegData->m_OnStartOfFrameFunc(pRegData->m_pObject, &SofTod);
    }
    DtSpinLockRelease(&pBc->m_Lock);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcGENL_InterruptHandler(DtBc* pBcBase, Int Id, Int Index, void* pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcGENL* pBc = (DtBcGENL*)pBcBase;
    DtDpcArgs  DpcArgs;

    // Sanity check
    BC_GENL_DEFAULT_PRECONDITIONS(pBc);

    // Check ID is one of ours
    switch (Id)
    {
    case DT_INTERRUPT_GENL_STARTOFFRAME:
        break;

       // Not a GENL interrupt
    default:
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    DpcArgs.m_Data2.m_UInt32_1 = GENL_SofTimestampSec_READ(pBc);
    DpcArgs.m_Data2.m_UInt32_2 = GENL_SofTimestampNanosec_READ(pBc);

    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&pBc->m_IntDpc, &DpcArgs);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcGENL_SetControlRegs(DtBcGENL* pBc, Bool BlkEna, Int OpMode, Bool FracClk, 
                                                                       Bool  ArmCrashLock)
{
    UInt32  RegData=0, FwBlkEna=0, FwOpMode=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? GENL_BLKENA_ENABLED : GENL_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = GENL_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = GENL_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = GENL_Control_READ(pBc);
    RegData = GENL_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = GENL_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = GENL_Control_SET_SdiClkSelect(RegData, FracClk ? GENL_SDICLKSEL_FRAC 
                                                               : GENL_SDICLKSEL_NON_FRAC);
    RegData = GENL_Control_SET_ArmCrashLock(RegData, ArmCrashLock ? 1 : 0);
    GENL_Control_WRITE(pBc, RegData);
}

