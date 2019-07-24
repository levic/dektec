//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcTOD.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtBcTOD.h"
#include "DtBcTOD_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcTOD implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#define NUM_NS_PER_SECOND  (1000*1000*1000)

// Default interval for the periodic interrupt (once per 10ms)
#define DEFAULT_PERIODIC_INTERRUPT_INTERVAL  10

// MACRO with default precondition checks for the Bc
#define BC_TOD_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcTOD))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_TOD_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(TOD, pBc)


// Helper macro to cast a DtBc* to a DtBcTOD*
#define BC_TOD         ((DtBcTOD*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcTOD_OnEnable(DtBc*, Bool Enable);
static DtStatus  DtBcTOD_Init(DtBc*);
static DtStatus  DtBcTOD_RegisterIntHandlers(DtBcTOD*);
static DtStatus  DtBcTOD_InterruptHandler(DtBc*, Int, Int, void*);
static void  DtBcTOD_InterruptDpc(DtDpcArgs* pArgs);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcTOD_Close(DtBc*  pBc)
{
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);
   
    // Let base function perform final clean-up
    DtBc_Close(pBc);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcTOD*  DtBcTOD_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_TOD_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcTOD, Id, DT_BLOCK_TYPE_TOD, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcTOD_Close;
    OpenParams.m_InitFunc = DtBcTOD_Init;
    OpenParams.m_OnEnableFunc = DtBcTOD_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcTOD*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_Adjust -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcTOD_Adjust(DtBcTOD* pBc, Int64  DeltaNs)
{
    Bool NegativeAdjust;
    UInt64  DeltaSeconds, DeltaNanoseconds, FractionSeconds;
    Int64  Adjustment;

    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);

    // Check state
    BC_TOD_MUST_BE_ENABLED(pBc);

    NegativeAdjust =  (DeltaNs < 0);
    if (NegativeAdjust)
        DeltaNs = -DeltaNs;

    // Compute Adjustment
    DeltaSeconds = DtDivide64((UInt64)DeltaNs, NUM_NS_PER_SECOND, NULL);
    DT_ASSERT(DeltaSeconds < (1LL<<32));
    DeltaNanoseconds = ((UInt64)DeltaNs - DeltaSeconds*NUM_NS_PER_SECOND);
    DT_ASSERT(DeltaNanoseconds < (1LL<<32));
    FractionSeconds = DtDivide64(DeltaNanoseconds* 1LL<<32, NUM_NS_PER_SECOND, NULL);
    DT_ASSERT(FractionSeconds < (1LL<<32));
    Adjustment = (DeltaSeconds<<30) + (FractionSeconds>>2);
    if (NegativeAdjust)
        Adjustment = -Adjustment;

    // Prevent mutual access
    DtSpinLockAcquire(&pBc->m_TodLock);
    
    //For best accuracy add the top 4 bits of clock-phase increment 
    Adjustment += pBc->m_ClockPhaseIncr>>28;
    
    // Write adjustment
    // Adjustment is a 62-bit signed value. 
    // The upper 32-bits contain the second ajdust
    TOD_SecondsAdjust_WRITE(BC_TOD, (UInt32)(Adjustment>>30));
    // The lower 30-bits contain the fraction second ajdust
    TOD_FractionAdjust_WRITE(BC_TOD, (UInt32)Adjustment&((1<<30)-1));

    DtSpinLockRelease(&pBc->m_TodLock);

    DtDbgOutBc(MAX, TOD, pBc, "Time adjustment, seconds: %d, fraction: %d", 
                                (UInt32)(Adjustment>>30), (UInt32)Adjustment&((1<<30)-1));
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_GetPeriodicItv -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcTOD_GetPeriodicItv(DtBcTOD* pBc, Int* pIntervalMs)
{
    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pIntervalMs!=NULL);
    
    // Check state
    if (!DtBc_IsEnabled((DtBc*)pBc) && !DtBc_IsInitialised((DtBc*)pBc))
    {
        DtDbgOutBc(ERR, TOD, pBc, "ERROR: BC is not enabled/initialised");
        return DT_STATUS_NOT_ENABLED;
    }

    // Get the periodic interval time
    *pIntervalMs = pBc->m_PeriodicInterval;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_GetPhaseIncr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcTOD_GetPhaseIncr(DtBcTOD* pBc, UInt32* pClockPhaseIncr)
{
    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pClockPhaseIncr!=NULL);

    // Check state
    BC_TOD_MUST_BE_ENABLED(pBc);

    // Prevent mutual access
    DtSpinLockAcquire(&pBc->m_TodLock);

    // Return cached values
    *pClockPhaseIncr  = pBc->m_ClockPhaseIncr;

    DtSpinLockRelease(&pBc->m_TodLock);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcTOD_GetProperties(DtBcTOD* pBc, UInt* pSysClockFreqHz, UInt* pAccuracy)
{
    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pSysClockFreqHz!=NULL && pAccuracy!=NULL);
    
    // Check state
    if (!DtBc_IsEnabled((DtBc*)pBc) && !DtBc_IsInitialised((DtBc*)pBc))
    {
        DtDbgOutBc(ERR, TOD, pBc, "ERROR: BC is not enabled/initialised");
        return DT_STATUS_NOT_ENABLED;
    }

    // Get the properties
    *pSysClockFreqHz = pBc->m_SysClockFreqHz;
    *pAccuracy = pBc->m_SysClockAccuracy;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_GetTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcTOD_GetTime(DtBcTOD* pBc, DtTodTime*  pTime, UInt* pAdjustCount)
{
    UInt32 SecondsTemp;

    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pTime!=NULL && pAdjustCount!=NULL);

    // Check state
    BC_TOD_MUST_BE_ENABLED(pBc);

    // Prevent mutual access
    DtSpinLockAcquire(&pBc->m_TodLock);
    
    // Read Time-of-day 
    SecondsTemp = TOD_TodSeconds_READ(BC_TOD);
    pTime->m_Nanoseconds = TOD_TodNanoseconds_READ(BC_TOD);
    pTime->m_Seconds = TOD_TodSeconds_READ(BC_TOD);

    // If there was a wraparround of nanoseconds, read it again
    if (pTime->m_Seconds != SecondsTemp)
        pTime->m_Nanoseconds = TOD_TodNanoseconds_READ(BC_TOD);

    // Read adjustment count
    *pAdjustCount = TOD_AdjustmentCount_READ(BC_TOD);

    DtSpinLockRelease(&pBc->m_TodLock);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_SetPhaseIncr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcTOD_SetPhaseIncr(DtBcTOD* pBc, UInt32 ClockPhaseIncr)
{
    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);

    // Check state
    BC_TOD_MUST_BE_ENABLED(pBc);

    // Prevent mutual access
    DtSpinLockAcquire(&pBc->m_TodLock);

    // Write ClockIncr  registers        
    TOD_ClockIncr_WRITE(BC_TOD, ClockPhaseIncr);
    
    // Update cached values
    BC_TOD->m_ClockPhaseIncr = ClockPhaseIncr;

    DtSpinLockRelease(&pBc->m_TodLock);

    DtDbgOutBc(MAX, TOD, pBc, "Set clock increment: %d", ClockPhaseIncr);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_SetTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcTOD_SetTime(DtBcTOD* pBc, DtTodTime  Time)
{
    UInt32 SecondsTemp;
    DtTodTime  CurTime;
    UInt  AdjustCount;
    Bool  PositiveDiff;
    UInt64  DeltaSeconds, DeltaNanoseconds, FractionSeconds;
    UInt64  SetTimeNs, CurTimeNs, TimeDiffNs;
    Int64  Adjustment;

    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);

    // Check state
    BC_TOD_MUST_BE_ENABLED(pBc);

    // Check parameters
    if (Time.m_Nanoseconds >= NUM_NS_PER_SECOND)
        return DT_STATUS_INVALID_PARAMETER;

    // Prevent mutual access
    DtSpinLockAcquire(&pBc->m_TodLock);

    // Read current time
    SecondsTemp = TOD_TodSeconds_READ(BC_TOD);
    CurTime.m_Nanoseconds = TOD_TodNanoseconds_READ(BC_TOD);
    CurTime.m_Seconds = TOD_TodSeconds_READ(BC_TOD);

    // If there was a wraparround of nanoseconds, read it again
    if (CurTime.m_Seconds != SecondsTemp)
        CurTime.m_Nanoseconds = TOD_TodNanoseconds_READ(BC_TOD);
    
    // Determine time difference in nanoseconds
    CurTimeNs = (UInt64)CurTime.m_Seconds*NUM_NS_PER_SECOND + CurTime.m_Nanoseconds;
    SetTimeNs = (UInt64)Time.m_Seconds*NUM_NS_PER_SECOND + Time.m_Nanoseconds;
    PositiveDiff= (SetTimeNs > CurTimeNs);
    if (PositiveDiff)
        TimeDiffNs = SetTimeNs - CurTimeNs;
    else
        TimeDiffNs = CurTimeNs -SetTimeNs;

    // Determine time difference as a fraction
    DeltaSeconds = DtDivide64(TimeDiffNs, NUM_NS_PER_SECOND, NULL);
    DT_ASSERT(DeltaSeconds < (1LL<<32));
    DeltaNanoseconds = (TimeDiffNs - DeltaSeconds*NUM_NS_PER_SECOND);
    DT_ASSERT(DeltaNanoseconds < (1LL<<32));
    FractionSeconds = DtDivide64(DeltaNanoseconds* 1LL<<32, NUM_NS_PER_SECOND, NULL);
    DT_ASSERT(FractionSeconds < (1LL<<32));
    Adjustment = (DeltaSeconds<<30) + (FractionSeconds>>2);
    if (!PositiveDiff)
        Adjustment = -Adjustment;
    
    //For best accuracy add the top 4 bits of clock-phase increment 
    Adjustment += pBc->m_ClockPhaseIncr>>28;
    
    // Write adjustment
    // Adjustment is a 62-bit signed value. 
    // The upper 32-bits contain the second ajdust
    TOD_SecondsAdjust_WRITE(BC_TOD, (UInt32)(Adjustment>>30));
    // The lower 30-bits contain the fraction second ajdust
    TOD_FractionAdjust_WRITE(BC_TOD, (UInt32)Adjustment&((1<<30)-1));
    DtSpinLockRelease(&pBc->m_TodLock);

    // Read back time
    DtBcTOD_GetTime(pBc, &CurTime, &AdjustCount);
    DtDbgOutBc(MAX, TOD, pBc, "Current time: %d seconds, %d nanoseconds", 
                                                CurTime.m_Seconds, CurTime.m_Nanoseconds);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_HandlerRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcTOD_PeriodicIntervalRegister(
    DtBcTOD*  pBc, 
    DtOnPeriodicIntervalFunc  pFunc, 
    DtObject*  pObject)
{
    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);

    pBc->m_OnPeriodicFunc = pFunc;
    pBc->m_pOnPeriodicObject = pObject;

    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_HandlerUnregister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcTOD_PeriodicIntervalUnregister(DtBcTOD*  pBc)
{
    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);

    pBc->m_OnPeriodicFunc = NULL;
    pBc->m_pOnPeriodicObject = NULL;

    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcTOD- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcTOD_OnEnable(DtBc*  pBc, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // Enable periodic interrupt
        Status = DtBc_InterruptEnable(pBc, DT_INTERRUPT_TOD_PERIODIC);
        DT_ASSERT(DT_SUCCESS(Status));
    }
    else
    {
        // Disable periodic interrupt
        Status = DtBc_InterruptDisable(pBc, DT_INTERRUPT_TOD_PERIODIC);
        DT_ASSERT(DT_SUCCESS(Status));
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcTOD_Init(DtBc*  pBc)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  ClockPhaseIncr;
    
    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);

    // Init spinlock
    DtSpinLockInit(&BC_TOD->m_TodLock);
    
    //.-.-.-.-.-.-.-.-.-.-.- Get TimeOfDay configuration properties -.-.-.-.-.-.-.-.-.-.-.

    // Config1 - SysClkFreq
    BC_TOD->m_SysClockFreqHz = TOD_Config1_READ_SysClockFreq(BC_TOD);
    DT_ASSERT(BC_TOD->m_SysClockFreqHz != 0);

    // Config1 - SysClkAccuracy (in 0.1ppm)
    BC_TOD->m_SysClockAccuracy = TOD_Config2_READ_SysClockAccuracy(BC_TOD);

    // Compute phase increment
    ClockPhaseIncr = (UInt32)DtDivide64((1LL<<58), BC_TOD->m_SysClockFreqHz, NULL);
      
    // Write clock phase increment registers
    TOD_ClockIncr_WRITE(BC_TOD, ClockPhaseIncr);
    
    // Cache computed phase increment values
    BC_TOD->m_ClockPhaseIncr = ClockPhaseIncr;

    // Init the periodic-interrupt-interval to the default interval period
    TOD_PeriodicInterval_WRITE(BC_TOD, DEFAULT_PERIODIC_INTERRUPT_INTERVAL);
    BC_TOD->m_PeriodicInterval = DEFAULT_PERIODIC_INTERRUPT_INTERVAL;

     // Print configuration
    DtDbgOutBc(AVG, REBOOT, pBc, "Configuration: sys_clock=%dkHz, clock_acc=%dppm, "
                           "clock_incr=%d,  periodic_interval=%dms",
                           BC_TOD->m_SysClockFreqHz/1000, BC_TOD->m_SysClockAccuracy,
                           BC_TOD->m_ClockPhaseIncr, DEFAULT_PERIODIC_INTERRUPT_INTERVAL);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init other attributes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Init interrupt DPC
    Status = DtDpcInit(&BC_TOD->m_IntDpc, DtBcTOD_InterruptDpc, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, TOD, pBc, "ERROR: failed to init DPC (Status=0x%08X)", Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.

    Status = DtBcTOD_RegisterIntHandlers(BC_TOD);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, TOD, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcTOD_RegisterIntHandlers(DtBcTOD*  pBc)
{
    Int  i=0;
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_TOD_PERIODIC:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id,
                                                          DtBcTOD_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, TOD, pBc,
                               "ERROR: failed to register interrupt handler (ID=%d)", Id);
            }
            break;

        default:
            DT_ASSERT(FALSE);      // Unknown ID. New version of the block??
            break;
        }
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcTOD_InterruptHandler(DtBc*  pBc, Int  Id, Int  Index, void*  pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;

    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);


    // Check ID is one of ours
    if (Id != DT_INTERRUPT_TOD_PERIODIC)
    {
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    // Pass current time as arguments
    DpcArgs.m_Data1.m_UInt32_1 = TOD_TodSeconds_READ(BC_TOD);
    DpcArgs.m_Data1.m_UInt32_2 = TOD_TodNanoseconds_READ(BC_TOD);
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcTOD*)pBc)->m_IntDpc, &DpcArgs);
    DT_ASSERT(DT_SUCCESS(Status) || Status==DT_STATUS_IN_USE);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD_InterruptDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcTOD_InterruptDpc(DtDpcArgs* pArgs)
{
    DtBcTOD*  pBc=NULL;
    DtTodTime  Time;

    DT_ASSERT(pArgs != NULL);

    pBc = (DtBcTOD*)pArgs->m_pContext;
    Time.m_Seconds = pArgs->m_Data1.m_UInt32_1;
    Time.m_Nanoseconds = pArgs->m_Data1.m_UInt32_2;

    // Sanity check
    BC_TOD_DEFAULT_PRECONDITIONS(pBc);

    // Do we have a handler
    if (pBc->m_OnPeriodicFunc != NULL)
        pBc->m_OnPeriodicFunc(pBc->m_pOnPeriodicObject, Time);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcTOD implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the BcTOD stub
#define STUB_BC_TOD_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size>=sizeof(DtIoStubBcTOD))

// Helper macro to cast stub's DtBc member to DtBcTOD
#define TOD_STUB   ((DtIoStubBcTOD*)pStub)
#define TOD_BC  ((DtBcTOD*)TOD_STUB->m_pBc)


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcTOD_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcTOD_OnCmdAdjustTime(const DtIoStubBcTOD*,
                                                     const DtIoctlTodCmdAdjustTimeInput*);
static DtStatus  DtIoStubBcTOD_OnCmdGetPhaseIncr(const DtIoStubBcTOD*,
                                                        DtIoctlTodCmdGetPhaseIncrOutput*);
static DtStatus  DtIoStubBcTOD_OnCmdGetProperties(const DtIoStubBcTOD*,
                                                       DtIoctlTodCmdGetPropertiesOutput*);
static DtStatus  DtIoStubBcTOD_OnCmdGetTime(const DtIoStubBcTOD*,
                                                             DtIoctlTodCmdGetTimeOutput*);
static DtStatus  DtIoStubBcTOD_OnCmdSetPhaseIncr(const DtIoStubBcTOD*, 
                                                   const DtIoctlTodCmdSetPhaseIncrInput*);
static DtStatus  DtIoStubBcTOD_OnCmdSetTime(const DtIoStubBcTOD*,
                                                        const DtIoctlTodCmdSetTimeInput*);
    
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_TOD;

static const DtIoctlProperties  IOSTUB_BC_TOD_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_TOD_CMD(
        DtIoStubBcTOD_OnCmd,
        NULL,  NULL),
};


//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcTOD - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTOD_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcTOD_Close(DtIoStub*  pStub)
{
    STUB_BC_TOD_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTOD_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcTOD*  DtIoStubBcTOD_Open(DtBc*  pBc)
{
    DtIoStubBcTOD*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    BC_TOD_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcTOD, pBc, NULL, 
                                                              DtIoStubBcTOD_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_BC_TOD_IOCTLS);
    pStub = (DtIoStubBcTOD*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcTOD - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTOD_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTOD_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlTodCmdInput*  pInData = NULL;
    DtIoctlTodCmdOutput*  pOutData = NULL;
    
    // Sanity check
    STUB_BC_TOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_TOD_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_TodCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_TodCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_TOD_CMD_ADJUST_TIME:
        Status = DtIoStubBcTOD_OnCmdAdjustTime(TOD_STUB, &pInData->m_AdjustTime);
        break;
    case DT_TOD_CMD_GET_PHASE_INCR:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcTOD_OnCmdGetPhaseIncr(TOD_STUB, &pOutData->m_GetPhaseIncr);
        break;
    case DT_TOD_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcTOD_OnCmdGetProperties(TOD_STUB, &pOutData->m_GetProperties);
        break;
    case DT_TOD_CMD_GET_TIME:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcTOD_OnCmdGetTime(TOD_STUB, &pOutData->m_GetTime);
        break;
    case DT_TOD_CMD_SET_PHASE_INCR:
        Status = DtIoStubBcTOD_OnCmdSetPhaseIncr(TOD_STUB, &pInData->m_SetPhaseIncr);
        break;
    case DT_TOD_CMD_SET_TIME:
        Status = DtIoStubBcTOD_OnCmdSetTime(TOD_STUB, &pInData->m_SetTime);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTOD_OnCmdAdjustTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcTOD_OnCmdAdjustTime(    
    const DtIoStubBcTOD*  pStub, 
    const DtIoctlTodCmdAdjustTimeInput*  pInData)
{
    STUB_BC_TOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcTOD_Adjust(TOD_BC, pInData->m_DeltaNs);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTOD_OnCmdGetPhaseIncr -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTOD_OnCmdGetPhaseIncr(
    const DtIoStubBcTOD*  pStub, 
    DtIoctlTodCmdGetPhaseIncrOutput*  pOutData)
{
    STUB_BC_TOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtBcTOD_GetPhaseIncr(TOD_BC, &pOutData->m_PhaseIncr);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTOD_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcTOD_OnCmdGetProperties(
    const DtIoStubBcTOD*  pStub, 
    DtIoctlTodCmdGetPropertiesOutput*  pOutData)
{
    STUB_BC_TOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtBcTOD_GetProperties(TOD_BC, &pOutData->m_SysClockFreq, 
                                                           &pOutData->m_SysClockAccuracy);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTOD_OnCmdGetTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTOD_OnCmdGetTime(
    const DtIoStubBcTOD*  pStub, 
    DtIoctlTodCmdGetTimeOutput*  pOutData)
{
    STUB_BC_TOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtBcTOD_GetTime(TOD_BC, &pOutData->m_Time, &pOutData->m_AdjustmentCount);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTOD_OnCmdSetPhaseIncr -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTOD_OnCmdSetPhaseIncr(    
    const DtIoStubBcTOD*  pStub, 
    const DtIoctlTodCmdSetPhaseIncrInput*  pInData)
{
    STUB_BC_TOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcTOD_SetPhaseIncr(TOD_BC, pInData->m_PhaseIncr);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTOD_OnCmdSetTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTOD_OnCmdSetTime(    
    const DtIoStubBcTOD*  pStub, 
    const DtIoctlTodCmdSetTimeInput*  pInData)
{
    STUB_BC_TOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcTOD_SetTime(TOD_BC, pInData->m_Time);
}