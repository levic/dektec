//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Lmh1983.c *#*#*#*#*#*#*#*#*# (C) 2013-2015 DekTec
//
// Dta driver - National LMH1983 (Video Clock Generator) controller

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2013-2015 DekTec Digital Video B.V.
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
#include <DtaIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#define DTA_LMH1983_I2C_TIMEOUT     100

#define DTA_LMH1983_POLL_EVENT      0
#define DTA_LMH1983_SOF_EVENT       1

// Bits for the device control register (addr 0x05) of the LMH1983
#define LMH1983_DEVCTRL_SOFT_RESET  0x80
#define LMH1983_DEVCTRL_POWERDOWN   0x40
#define LMH1983_DEVCTRL_EN_AFD      0x20  // Enable Auto Format Detection
#define LMH1983_DEVCTRL_PLL1(x)     ((x & 0x03) << 3)
#define LMH1983_DEVCTRL_PLL1_MASK   0x18
#define LMH1983_DEVCTRL_LOR_MODE    0x04
#define LMH1983_DEVCTRL_FORCE_148   0x02  // Force PLL 2 and 3 to 148.5 resp 148.35 Mhz
#define LMH1983_DEVCTRL_GOE         0x01  // Global Output Enable
#define PLL1_MODE_FREERUN           0x0
#define PLL1_MODE_GENLOCK           0x1
#define PLL1_MODE_HOLDOVER          0x2

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static void  DtaLmh1983ControllerThread(DtThread* pThread, void* pContext);
static DtStatus  DtaLmh1983ReadReg(DtaLmh1983* pLmh1983Data, UInt8 RegAddr, UInt8* Value);
static DtStatus  DtaLmh1983WriteReg(DtaLmh1983* pLmh1983Data, UInt8 RegAddr, UInt8 Value);
static DtStatus  DtaLmh1983GoToNextState(DtaLmh1983* pLmh1983Data, Int ChangeEvent);
static DtStatus  DtaLmh1983InitChip(DtaLmh1983* pLmh1983Data);
static Int  DtaLmh1983LockStateGet(DtaLmh1983* pLmh1983Data);
static DtStatus  DtaLmh1983PllFastlockSet(DtaLmh1983* pLmh1983Data, Bool  Enable);
static DtStatus  DtaLmh1983TofCrashLockSet(DtaLmh1983* pLmh1983Data, Bool  Enable);
static UInt8  DtaLmh1983VidStd2Format(int  VidStd);
#ifdef _DEBUG
static const char*  DtaLmh1983State2Str(Int State);
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1983Init(DtaDeviceData* pDvcData, DtaLmh1983* pLmh1983Data)
{
    pLmh1983Data->m_pDvcData = pDvcData;

    // Init state mutex
    DtFastMutexInit(&pLmh1983Data->m_StateMutex);
    
    // Set initial state of state machine
    pLmh1983Data->m_State = DTA_LMH1983_STATE_INIT;
    pLmh1983Data->m_EnteredStateTime = DtGetTickCount();

    // Init controller thread
    DtThreadInit(&pLmh1983Data->m_ControlThread, DtaLmh1983ControllerThread, 
                                                                            pLmh1983Data);
    
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983WriteInitSeq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaLmh1983WriteInitSeq(DtaLmh1983*  pLmh1983Data)
{
    // Init sequence for LMH1983 on DTA-2154 and DTA-2174
    #define  DTA_LMH1983_I2C_INIT_SEQ_SIZE 9
    #define  DTA_LMH1983_I2C_WRITE_RETRIES 4
    static UInt8  DTA_LMH1983_INIT_SEQUENCE[DTA_LMH1983_I2C_INIT_SEQ_SIZE][2] = 
    {
        { DTA_LMH1983_IIC_REG05, 0x80 },    // Device Control: soft reset => will default
                                            // all registers

        { DTA_LMH1983_IIC_REG09, 0x02 },    // Output Mode – Misc: write 0x02 then 0x00 to 
        { DTA_LMH1983_IIC_REG09, 0x00 },    // prevent poor CLKout3 duty cycle (see
                                            // datasheet INITIALIZATION section)

        { DTA_LMH1983_IIC_REG05, 0x03 },    // Device Control: GOE=1, Force_148=1,
                                            // holdover on LOR, no auto detection

        { DTA_LMH1983_IIC_REG20, 0x16 },    // Input Format: default to 1080i50

        { DTA_LMH1983_IIC_REG0A, 0x8E },    // Output Buffer Control: enable Fout1 and 
                                            // CLKout1..3 

        { DTA_LMH1983_IIC_REG11, 0x04 },    // Alignment Control – TOF1: 
                                            // Alignment mode = auto
                                            // TOF_sync_far=crash mode, 
                                            // TOF_sync_near=drift mode

        { DTA_LMH1983_IIC_REG0B, 0x00 },    // Output Frame Control – Offset1: delay TOF1
        { DTA_LMH1983_IIC_REG0C, 0x00 },    //  by 0 line
    };

    DtStatus  Status = DT_STATUS_OK;
    DtaDeviceData*  pDvcData = pLmh1983Data->m_pDvcData;
    Int  i;
    
    // Get I2C lock.
    // NOTE: Use DT_INVALID_FILE_OBJECT_PTR signals the driver wants to own the lock
    Status = DtaI2cLock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, DTA_LMH1983_I2C_TIMEOUT);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to get I2C lock. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }

    for (i=0; Status==DT_STATUS_OK && i<DTA_LMH1983_I2C_INIT_SEQ_SIZE; i++)
    {
        Int  Retries = 0;
        while (Retries < DTA_LMH1983_I2C_WRITE_RETRIES)
        {
            DtDbgOut(MIN, GENL, "Writing: 0x%X 0x%X", DTA_LMH1983_INIT_SEQUENCE[i][0], 
                                                         DTA_LMH1983_INIT_SEQUENCE[i][1]);

            Status = DtaI2cWrite(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, 
                                   DTA_LMH1983_I2C_ADDR, 2, DTA_LMH1983_INIT_SEQUENCE[i]);
            if (Status == DT_STATUS_TIMEOUT)
                Retries++;
            else
                break;
        }
    }

    // Unlock I2C bus
    DtaI2cUnlock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, FALSE);

    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983InitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaLmh1983InitPowerup(DtaLmh1983*  pLmh1983Data)
{
    DtStatus  Status = DT_STATUS_OK;

    Status = DtaLmh1983WriteInitSeq(pLmh1983Data);
    if (Status != DT_STATUS_OK)
        DtDbgOut(ERR, GENL, "Failed to write LMH-1983 init sequence: 0x%x", Status);

    DtThreadStart(&pLmh1983Data->m_ControlThread);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983ReadReg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaLmh1983ReadReg(DtaLmh1983* pLmh1983Data, UInt8 RegAddr, UInt8* Value)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaDeviceData*  pDvcData = pLmh1983Data->m_pDvcData;

    // Get I2C lock
    // NOTE: Use DT_INVALID_FILE_OBJECT_PTR signals the driver wants to own the lock
    Status = DtaI2cLock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, DTA_LMH1983_I2C_TIMEOUT);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to get I2C lock. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }

    Status = DtaI2cWriteRead(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR,
                                                        DTA_LMH1983_I2C_ADDR, 1, &RegAddr,
                                                        DTA_LMH1983_I2C_ADDR, 1, Value);

    // Release lock
    DtaI2cUnlock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, FALSE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to read register (addr=0x%02X). "
                                                   "Error: 0x%x", RegAddr, Status);
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983WriteReg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1983WriteReg(DtaLmh1983* pLmh1983Data, UInt8 RegAddr, UInt8 Value)
{
    UInt8  I2cBuf[2];
    DtStatus  Status = DT_STATUS_OK;
    DtaDeviceData*  pDvcData = pLmh1983Data->m_pDvcData;

    // Get I2C lock
    // NOTE: Use DT_INVALID_FILE_OBJECT_PTR signals the driver wants to own the lock
    Status = DtaI2cLock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, DTA_LMH1983_I2C_TIMEOUT);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to get I2C lock. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }

    // Write register
    I2cBuf[0] = RegAddr; I2cBuf[1] = Value;
    Status = DtaI2cWrite(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, 
                                                         DTA_LMH1983_I2C_ADDR, 2, I2cBuf); 

    // Release lock
    DtaI2cUnlock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, FALSE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to write register (addr=0x%02X, value=0x%02X). "
                                                   "Error: 0x%x", RegAddr, Value, Status);
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983ResetStateMachine -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1983ResetStateMachine(DtaLmh1983* pLmh1983Data)
{
    DtDbgOut(ERR, GENL, "Start");
    // Acquire state mutex and reset state to initial value
    DtFastMutexAcquire(&pLmh1983Data->m_StateMutex);
    pLmh1983Data->m_State = DTA_LMH1983_STATE_INIT;
    pLmh1983Data->m_EnteredStateTime = DtGetTickCount();
    DtFastMutexRelease(&pLmh1983Data->m_StateMutex);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983PowerdownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1983PowerdownPre(DtaLmh1983* pLmh1983Data)
{
    return DtThreadStop(&pLmh1983Data->m_ControlThread);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983ControllerThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaLmh1983ControllerThread(DtThread* pThread, void* pContext)
{
    Int  TimeoutCnt = 0;
    DtStatus  Status = DT_STATUS_OK;
    DtaLmh1983*  pLmh1983Data = (DtaLmh1983*)pContext;

    DtDbgOut(MAX, GENL, "Thread begin");
        
    TimeoutCnt = 0;
    while (!pThread->m_StopThread)
    {
        // Wait for SOF event (or timeout) 
        Status = DtEventWait(&pLmh1983Data->m_pDvcData->m_Matrix.m_SofFrameSyncEvent, 10);

        if (Status == DT_STATUS_OK)
        {
            // Clear event
            DtEventReset(&pLmh1983Data->m_pDvcData->m_Matrix.m_SofFrameSyncEvent);
            // Update state-machine
            Status = DtaLmh1983GoToNextState(pLmh1983Data, DTA_LMH1983_SOF_EVENT);
            
            // Clear timeout counter
            TimeoutCnt = 0;
        }
        else if (Status == DT_STATUS_TIMEOUT)
        {
            TimeoutCnt++;
            // Update state-machine once every 10 timeouts (i.e. approx 100ms)
            if (TimeoutCnt >= 10)
            {
                // Update state-machine
                Status = DtaLmh1983GoToNextState(pLmh1983Data, DTA_LMH1983_SOF_EVENT);
                TimeoutCnt = 0;
            }
        }
    }
    DtDbgOut(MAX, GENL, "Thread exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983GoToNextState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtaLmh1983GoToNextState(DtaLmh1983* pLmh1983Data, Int ChangeEvent)
{
    const UInt64  LMH1983_HOLDOVER_TIMEOUT_MS = 60000;  // 60sec time out
    DtStatus  Status = DT_STATUS_OK;
    Int  OldState, NewState;
    UInt64  CurTime, TimeDiff;

    // Acquire state lock
    DtFastMutexAcquire(&pLmh1983Data->m_StateMutex);

    // Store current state
    NewState = OldState = pLmh1983Data->m_State;
    CurTime = DtGetTickCount();
            
    switch (OldState)
    {
    case DTA_LMH1983_STATE_INIT:
        // Init LMH-1983 chip
        Status = DtaLmh1983InitChip(pLmh1983Data);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, GENL, "Failed to initialise LMH1983");

            // Release state lock
            DtFastMutexRelease(&pLmh1983Data->m_StateMutex);
            return DT_STATUS_FAIL;
        }
        else 
            NewState = DTA_LMH1983_STATE_NOREF;
        
        // Fire a no-lock event
        Status = DtaEventsSet(pLmh1983Data->m_pDvcData, NULL, DTA_EVENT_TYPE_GENLOCK, 
                                                             DTA_EVENT_VALUE1_NO_LOCK, 0);
        DT_ASSERT(DT_SUCCESS(Status));
        break;

        // No input reference, wait for an reference input
    case DTA_LMH1983_STATE_NOREF:

        // Re-check lock state
        NewState = DtaLmh1983LockStateGet(pLmh1983Data);
        if (NewState == DTA_LMH1983_STATE_LOCKING)
        {
            // Enable fast PLL lock and TOF crash lock modes
            Status = DtaLmh1983PllFastlockSet(pLmh1983Data, TRUE);
            DT_ASSERT(DT_SUCCESS(Status));
            Status = DtaLmh1983TofCrashLockSet(pLmh1983Data, TRUE);
            DT_ASSERT(DT_SUCCESS(Status));
        }
        break;

        // No PLL and TOF lock yet => wait for lock
    case DTA_LMH1983_STATE_LOCKING:

        // Re-check lock state
        NewState = DtaLmh1983LockStateGet(pLmh1983Data);
        if (NewState == DTA_LMH1983_STATE_LOCKED)
        {
            // Disable fast PLL lock and TOF crash lock modes
            Status = DtaLmh1983PllFastlockSet(pLmh1983Data, FALSE);
            DT_ASSERT(DT_SUCCESS(Status));
            Status = DtaLmh1983TofCrashLockSet(pLmh1983Data, FALSE);
            DT_ASSERT(DT_SUCCESS(Status));
        }
        break;

        // Full lock => remain here until lock is lost
    case DTA_LMH1983_STATE_LOCKED:

        // Re-check lock state
        NewState = DtaLmh1983LockStateGet(pLmh1983Data);
        if (NewState != DTA_LMH1983_STATE_LOCKED)
            NewState = DTA_LMH1983_STATE_HOLDOVER;    // Go to HOLDOVER state
        break;
        
        // Lost lock, but chip is set to operate in HOLD over mode. Stay in this state,
        // until lock is reestablished or the HOLD over time-out is reached
    case DTA_LMH1983_STATE_HOLDOVER:

        // Re-check lock state
        NewState = DtaLmh1983LockStateGet(pLmh1983Data);
        if (NewState == DTA_LMH1983_STATE_LOCKED)
            break;  // Restored lock again
        
        // Did the hold over time-out expire?
        TimeDiff = CurTime - pLmh1983Data->m_EnteredStateTime;
        if (TimeDiff < LMH1983_HOLDOVER_TIMEOUT_MS)
            NewState = DTA_LMH1983_STATE_HOLDOVER; // Stay in hold-over state
        break;

    default:
        // Unexpected state => reset state-machine
        DtDbgOut(ERR, GENL, "UNKONWN STATE %d!!!!", OldState);
        NewState =  DTA_LMH1983_STATE_INIT;
        break;
    }

    // Safe new state
    pLmh1983Data->m_State = NewState;

    // Release state lock
    DtFastMutexRelease(&pLmh1983Data->m_StateMutex);

    if (OldState != NewState)
    {
        TimeDiff = CurTime - pLmh1983Data->m_EnteredStateTime;
        
        // Store time at which the new state was entered
        pLmh1983Data->m_EnteredStateTime = CurTime;
        DtDbgOut(MIN, GENL, "[dT=%llums] state transition %s => %s", TimeDiff,
                            DtaLmh1983State2Str(OldState), DtaLmh1983State2Str(NewState));
    }

    // Fire lock state event
    if (OldState<DTA_LMH1983_STATE_LOCKED && NewState==DTA_LMH1983_STATE_LOCKED)
    {
        Status = DtaEventsSet(pLmh1983Data->m_pDvcData, NULL, DTA_EVENT_TYPE_GENLOCK, 
                                                              DTA_EVENT_VALUE1_LOCKED, 0);
        DT_ASSERT(DT_SUCCESS(Status));
    }
    else if (OldState>=DTA_LMH1983_STATE_LOCKED && NewState<DTA_LMH1983_STATE_LOCKED)
    {
        Status = DtaEventsSet(pLmh1983Data->m_pDvcData, NULL, DTA_EVENT_TYPE_GENLOCK, 
                                                             DTA_EVENT_VALUE1_NO_LOCK, 0);
        DT_ASSERT(DT_SUCCESS(Status));
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983SetupRefSource -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaLmh1983SetupRefSource(
    DtaLmh1983*  pLmh1983Data, 
    Int*  pRefVidStd,
    Int*  pOutVidStd)
{
    Int  i, ClkSrc = 0;
    Bool  HasCapMatrix2 = FALSE;
    DtaGenlock*  pGenlock = &pLmh1983Data->m_pDvcData->m_Genlock;
    volatile UInt8*  pGenlRegs = pGenlock->m_pGenlRegs;

    // Check if hardware supports Matrix 2.0
    DT_ASSERT(pLmh1983Data->m_pDvcData != NULL);
    HasCapMatrix2 = FALSE;
    for (i=0; i<pLmh1983Data->m_pDvcData->m_NumNonIpPorts && !HasCapMatrix2; i++)
    {
        DT_ASSERT(pLmh1983Data->m_pDvcData->m_pNonIpPorts != NULL);
        // Does the port support matrix 2.0
        if (!pLmh1983Data->m_pDvcData->m_pNonIpPorts[i].m_CapMatrix2)
            continue;
        HasCapMatrix2 = TRUE;
    }

    // Start with resetting LHM-1983
    DtaRegHdGenlClkConfSetAsyncReset(pGenlRegs, 1);
    DtSleep(5);
    
    if (pGenlock->m_RefPortIndex  == pGenlock->m_IntGenrefPortIndex)
    {
        ClkSrc = 0;     // Internal clock
        *pRefVidStd = pGenlock->m_RefVidStd;
        *pOutVidStd = pGenlock->m_OutVidStd;
    }
    else if (pGenlock->m_RefPortIndex == pGenlock->m_AsyncPortIndex)
    {
        ClkSrc = 15;    // analog clock
        *pRefVidStd = pGenlock->m_RefVidStd;
        *pOutVidStd = pGenlock->m_OutVidStd;
    }
    else
    {
        ClkSrc = pGenlock->m_RefPortIndex+1;    // Use specified port
        *pRefVidStd = pGenlock->m_RefVidStd;
        *pOutVidStd = pGenlock->m_OutVidStd;
    }

    // Set clock source
    DtaRegHdGenlClkConfSetRefSrc(pGenlRegs, ClkSrc);

    if (!HasCapMatrix2)
    {
        // Set interlaced flags
        if (DtaVidStdIsInterlaced(*pRefVidStd))
            DtaRegHdGenlClkConfSetInterlaced(pGenlRegs, 1);
        else
            DtaRegHdGenlClkConfSetInterlaced(pGenlRegs, 0);
    }

    // Finally remove reset
    DtaRegHdGenlClkConfSetAsyncReset(pGenlRegs, 0);
    DtSleep(5);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983SetVcxoValue -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1983SetVcxoValue(DtaLmh1983* pLmh1983Data, Int  VcxoValue)
{
    UInt8  VCFreeMsb, VCFreeLsb;
    DT_RETURN_ON_ERROR(DtaLmh1983ReadReg(pLmh1983Data, DTA_LMH1983_IIC_REG18,&VCFreeMsb));
    VCFreeMsb &= ~0x3;
    VCFreeMsb |= VcxoValue>>8;
    VCFreeLsb = (UInt8)VcxoValue;
    DT_RETURN_ON_ERROR(DtaLmh1983WriteReg(pLmh1983Data, DTA_LMH1983_IIC_REG18,VCFreeMsb));
    DT_RETURN_ON_ERROR(DtaLmh1983WriteReg(pLmh1983Data, DTA_LMH1983_IIC_REG19,VCFreeLsb));
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983SetDevCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaLmh1983SetDevCtrl(DtaLmh1983* pLmh1983Data)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  DvcCtrl = 0;
    Int  RefPortIdx = pLmh1983Data->m_pDvcData->m_Genlock.m_RefPortIndex;
    Int  IntGenrefPortIdx = pLmh1983Data->m_pDvcData->m_Genlock.m_IntGenrefPortIndex;
    Int  OpModeInSrc = pLmh1983Data->m_pDvcData->m_Genlock.m_OpModeIntSrc;

    // Get current value device control register
    Status = DtaLmh1983ReadReg(pLmh1983Data, DTA_LMH1983_IIC_REG05, &DvcCtrl);
    // Set Setup PLL1 mode and auto format detect
    // Clear PLL mode and AFD flag
    DvcCtrl &= ~(LMH1983_DEVCTRL_EN_AFD | LMH1983_DEVCTRL_PLL1_MASK);
    if (RefPortIdx == IntGenrefPortIdx)
    {
        // Use internal clock source => setup according to desired op-mode
        if (OpModeInSrc==GENLOCK_OPMODE_INTSRC_FREE_RUN ||
                                    pLmh1983Data->m_pDvcData->m_Genlock.m_VcxoValue!=-1)
        {
            // Set pll_mode=free-run && AFD=0
            DvcCtrl |= LMH1983_DEVCTRL_PLL1(PLL1_MODE_FREERUN);
        }
        else if (OpModeInSrc == GENLOCK_OPMODE_INTSRC_AFD)
        {
            // Set pll_mode=genlock && AFD=1
            DvcCtrl |= LMH1983_DEVCTRL_PLL1(PLL1_MODE_GENLOCK)|LMH1983_DEVCTRL_EN_AFD;
        }
        else
        {
            DT_ASSERT(1 == 0);
            DtDbgOut(ERR, GENL, "Unknown op-mode: %d", OpModeInSrc);
            return DT_STATUS_FAIL;
        }
    }
    else
    {
        // Use external genlock => PLL_mode=genlock
        DvcCtrl |= LMH1983_DEVCTRL_PLL1(PLL1_MODE_GENLOCK)|LMH1983_DEVCTRL_EN_AFD;
    }

    Status = DtaLmh1983WriteReg(pLmh1983Data, DTA_LMH1983_IIC_REG05, DvcCtrl);
    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983InitChip -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1983InitChip(DtaLmh1983* pLmh1983Data)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  RefVidStd,  OutVidStd;
    UInt8  FormatCode=0, DvcCtrl=0, Offset=0;
    Int  RefPortIdx = pLmh1983Data->m_pDvcData->m_Genlock.m_RefPortIndex;
    Int  OpModeInSrc = pLmh1983Data->m_pDvcData->m_Genlock.m_OpModeIntSrc;

    Status = DtaLmh1983SetupRefSource(pLmh1983Data, &RefVidStd, &OutVidStd);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to set clock source. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }

    //.-.-.-.-.-.-.-.-.-.-.- Step 1: Setup device control register -.-.-.-.-.-.-.-.-.-.-.-

    // Get current value device control register
    Status = DtaLmh1983SetDevCtrl(pLmh1983Data);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to set device control register. Error: 0x%X", Status);
        return Status;
    }
    
    //.-.-.-.-.-.-.-.-.-.-.- Step 2: setup input and output format -.-.-.-.-.-.-.-.-.-.-.-

    // Set the reference format. NOTE: not needed in all cases, because we mostly will be 
    // using auto detection (see above)
    FormatCode = DtaLmh1983VidStd2Format(RefVidStd);
    Status = DtaLmh1983WriteReg(pLmh1983Data, DTA_LMH1983_IIC_REG20, FormatCode);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to set input format register. Error: 0x%X", Status);
        return Status;
    }
    DtDbgOut(MIN, GENL, "Reference format-code: %d", FormatCode);

    // Set output format
    FormatCode = DtaLmh1983VidStd2Format(OutVidStd);
    if (DtaVidStdIsFractional(OutVidStd))
    {
        // Fractional timing is generated by PLL3
        Status = DtaLmh1983WriteReg(pLmh1983Data, DTA_LMH1983_IIC_REG08, FormatCode);
    }
    else
    {
        // Non-fractional timing is generated by PLL2
        Status = DtaLmh1983WriteReg(pLmh1983Data, DTA_LMH1983_IIC_REG07, FormatCode);
    }
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to set output format register. Error: 0x%X", Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 3: Miscellaneous setup -.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Set VCXO value
    if (pLmh1983Data->m_pDvcData->m_Genlock.m_VcxoValue!=-1)
    {
        Status = DtaLmh1983SetVcxoValue(pLmh1983Data,
                                         pLmh1983Data->m_pDvcData->m_Genlock.m_VcxoValue);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, GENL, "Failed to set VCXO value. Error: 0x%X", Status);
            return Status;
        }
    }

    Offset = 0;
    if (RefPortIdx!=pLmh1983Data->m_pDvcData->m_Genlock.m_AsyncPortIndex)
    {
        DtAvFrameProps  VidProps;
        Status = DtAvGetFrameProps(RefVidStd, &VidProps);
        if (DT_SUCCESS(Status))
        {
            if (VidProps.m_IsInterlaced)
                Offset = VidProps.m_NumLines - VidProps.m_Field2ActVidEnd;
            else
                Offset = VidProps.m_NumLines - VidProps.m_Field1ActVidEnd;
        }
    }
    if (DT_SUCCESS(Status))
        Status = DtaLmh1983WriteReg(pLmh1983Data, DTA_LMH1983_IIC_REG0C, Offset);

    // Enable Fast PLL locking
    Status = DtaLmh1983PllFastlockSet(pLmh1983Data, TRUE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to enable Fast PLL locking. Error: 0x%X", Status);
        return Status;
    }
    // Enable TOF crash lock mode
    Status = DtaLmh1983TofCrashLockSet(pLmh1983Data, TRUE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to enable TOF crash lock mode. Error: 0x%X", Status);
        return Status;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983LockStateGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaLmh1983LockStateGet(DtaLmh1983* pLmh1983Data)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  DvcStatus1=0, DvcStatus2=0;
    Int  State = DTA_LMH1983_STATE_NOREF;
    
    Status = DtaLmh1983ReadReg(pLmh1983Data, DTA_LMH1983_IIC_REG00, &DvcStatus1);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to read device status - input ref. Error: 0x%x",
                                                                                  Status);
        return State;
    }
    Status = DtaLmh1983ReadReg(pLmh1983Data, DTA_LMH1983_IIC_REG01, &DvcStatus2);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to read device status. Error: 0x%x", Status);
        return State;
    }
    
    // Check LOR_STATUS first
    if ((DvcStatus1 & 0x04) == 0)
        State = DTA_LMH1983_STATE_NOREF;    // Invalid reference signal
    else if ((DvcStatus2 & 0xC0) == 0xC0)       
        State = DTA_LMH1983_STATE_LOCKED;   // PLL and TOF lock => full lock
    else
        State = DTA_LMH1983_STATE_LOCKING;  // Valid ref, but no PLL and/or TOF lock yet
    return State;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983PllFastlockSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// 
// Enable/Disable FAST PLL locking
//
// Step 1: Set Lock_Step_Size field, PLL1 Advanced Control Lock Step Size
// Step 2: Set LOCK1_Threshold field, inLoss of Lock Threshold
//
// See "LOCK TIME CONSIDERATIONS" section in LMH1983 datasheet
//
DtStatus  DtaLmh1983PllFastlockSet(DtaLmh1983* pLmh1983Data, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  RegValue;

    // Step 1: Set Lock_Step_Size field to (enable=0x01, disable=0x10)
    Status = DtaLmh1983ReadReg(pLmh1983Data, DTA_LMH1983_IIC_REG2D, &RegValue);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to read PLL1 Advanced Control Lock Step Size "
                                                         "register. Error: 0x%X", Status);
        return Status;
    }
    RegValue &= ~0x1F; RegValue |= Enable ? 0x01 : 0x10;
    Status = DtaLmh1983WriteReg(pLmh1983Data, DTA_LMH1983_IIC_REG2D, RegValue);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to write to PLL1 Advanced Control Lock Step Size "
                                                         "register. Error: 0x%X", Status);
        return Status;
    }

    // Step 2: Set LOCK1_Threshold field to (enable=0x1F, disable=0x10)
    Status = DtaLmh1983ReadReg(pLmh1983Data, DTA_LMH1983_IIC_REG1C, &RegValue);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to read Loss of Lock Threshold register. "
                                                                   "Error: 0x%X", Status);
        return Status;
    }
    RegValue &= ~0x1F; RegValue |= Enable ? 0x1F : 0x10; 
    Status = DtaLmh1983WriteReg(pLmh1983Data, DTA_LMH1983_IIC_REG1C, RegValue);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to write to Loss of Lock Threshold register. "
                                                                   "Error: 0x%X", Status);
        return Status;
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983TofCrashLockSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Enable/disable TOF align crash lock mode
//
// See "TOF1 ALIGNMENT" section in LMH1983 datasheet
//
DtStatus  DtaLmh1983TofCrashLockSet(DtaLmh1983* pLmh1983Data, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  RegValue;

    // Set TOF1_Sync_Near and TOF1_Sync_Far
    // - Enable: near=crash lock & far=crash lock
    // - Disable: near=drift lock & near=drift lock
    Status = DtaLmh1983ReadReg(pLmh1983Data, DTA_LMH1983_IIC_REG11, &RegValue);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to read Alignment Control – TOF1 register. "
                                                                   "Error: 0x%X", Status);
        return Status;
    }
    RegValue &= ~0x0C; RegValue |= Enable ? 0x0C : 0x00; 
    Status = DtaLmh1983WriteReg(pLmh1983Data, DTA_LMH1983_IIC_REG11, RegValue);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to write to Alignment Control – TOF1 register. "
                                                                   "Error: 0x%X", Status);
        return Status;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983VidStd2Format -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Convert a video standard to the coresponding LMH1983 video format code
//
UInt8  DtaLmh1983VidStd2Format(int  VidStd)
{
    UInt8  FormatCode = 1;
    switch (VidStd)
    {
    case DT_VIDSTD_525I59_94:   FormatCode = 0; break;
    case DT_VIDSTD_625I50:      FormatCode = 1; break;
    case DT_VIDSTD_720P23_98:   FormatCode = 11; break;
    case DT_VIDSTD_720P24:      FormatCode = 10; break;
    case DT_VIDSTD_720P25:      FormatCode = 9; break;
    case DT_VIDSTD_720P29_97:   FormatCode = 8; break;
    case DT_VIDSTD_720P30:      FormatCode = 7; break;
    case DT_VIDSTD_720P50:      FormatCode = 6; break;
    case DT_VIDSTD_720P59_94:   FormatCode = 5; break;
    case DT_VIDSTD_720P60:      FormatCode = 4; break;
    case DT_VIDSTD_1080P23_98:  FormatCode = 19; break;
    case DT_VIDSTD_1080P24:     FormatCode = 18; break;
    case DT_VIDSTD_1080P25:     FormatCode = 17; break;
    case DT_VIDSTD_1080P29_97:  FormatCode = 16; break;
    case DT_VIDSTD_1080P30:     FormatCode = 15; break;
    case DT_VIDSTD_1080I50:     FormatCode = 22; break;
    case DT_VIDSTD_1080I59_94:  FormatCode = 21; break;
    case DT_VIDSTD_1080I60:     FormatCode = 20; break;
    case DT_VIDSTD_1080P50:     FormatCode = 14; break;
    case DT_VIDSTD_1080P59_94:  FormatCode = 13; break;
    case DT_VIDSTD_1080P60:     FormatCode = 12; break;
    default:
        DtDbgOut(ERR, GENL, "Unknown video standard: 0x%X", VidStd);
        FormatCode = 1;
        break;
    }
    return FormatCode;
}

#ifdef _DEBUG
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983State2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
const char*  DtaLmh1983State2Str(Int State)
{
    switch (State)
    {
    case DTA_LMH1983_STATE_INIT:            return "INIT";
    case DTA_LMH1983_STATE_NOREF:           return "NOREF";
    case DTA_LMH1983_STATE_LOCKING:         return "LOCKING";
    case DTA_LMH1983_STATE_LOCKED:          return "LOCKED";
    case DTA_LMH1983_STATE_HOLDOVER:        return "HOLDOVER";
    default:                                return "UNKNOWN";
    }
}
#endif
