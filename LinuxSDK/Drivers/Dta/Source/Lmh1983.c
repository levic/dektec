//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Lmh1983.cpp *#*#*#*#*#*#*#*#*#*#*# (C) 2013 DekTec
//
// Dta driver - National LMH1983 (Video Clock Generator) controller

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2013 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static void  DtaLmh1983ControllerThread(DtThread* pThread, void* pContext);
static DtStatus  DtaLmh1983ReadReg(DtaLmh1983* pLmh1983Data, UInt8 RegAddr, UInt8* Value);
static DtStatus  DtaLmh1983WriteReg(DtaLmh1983* pLmh1983Data, UInt8 RegAddr, UInt8 Value);
static DtStatus  DtaLmh1983GoToNextState(DtaLmh1983* pLmh1983Data, Int ChangeEvent);
static DtStatus  DtaLmh1983InitChip(DtaLmh1983* pLmh1983Data);
static Int  DtaLmh1983PllLockStateGet(DtaLmh1983* pLmh1983Data);
static DtStatus  DtaLmh1983StartTofAlign(DtaLmh1983* pLmh1983Data);
static DtStatus  DtaLmh1983StopTofAlign(DtaLmh1983* pLmh1983Data);
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

    // Init controller thread
    DtThreadInit(&pLmh1983Data->m_ControlThread, DtaLmh1983ControllerThread, pLmh1983Data);
    
    return DT_STATUS_OK;
}


#define DTA2154_I2C_INIT_SEQ_SIZE       9
#define MAX_DTA2154_I2C_WRITE_RETRIES   4
static UInt8  Dta2154Lmh1983InitSequence[DTA2154_I2C_INIT_SEQ_SIZE][2] = {
    {DTA_LMH1983_REG_DEVICE_CTRL,             0x80},
    {DTA_LMH1983_REG_OUT_MODE_MISC,           0x02},
    {DTA_LMH1983_REG_OUT_MODE_MISC,           0x00},
    {DTA_LMH1983_REG_DEVICE_CTRL,             0x03},
    {DTA_LMH1983_REG_INPUT_FORMAT,            0x16},
    {DTA_LMH1983_REG_OUT_BUF_CTRL,            0x8E},
    {DTA_LMH1983_REG_ALIGNMENT_CTRL_TOF1,     0x04},
    {DTA_LMH1983_REG_OUT_FRAME_CTRL_OFF1_MSB, 0x00},
    {DTA_LMH1983_REG_OUT_FRAME_CTRL_OFF1_LSB, 0x01},
};

static DtStatus  DtaLmh1983WriteInitSeq(DtaLmh1983*  pLmh1983Data)
{
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

    for (i=0; Status==DT_STATUS_OK && i<DTA2154_I2C_INIT_SEQ_SIZE; i++)
    {
        Int  Retries = 0;
        while (Retries < MAX_DTA2154_I2C_WRITE_RETRIES)
        {
            DtDbgOut(ERR, GENL, "Writing: %X %X", Dta2154Lmh1983InitSequence[i][0], Dta2154Lmh1983InitSequence[i][1]);
            Status = DtaI2cWrite(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, 
                               DTA_LMH1983_I2C_ADDR, 2, Dta2154Lmh1983InitSequence[i]);
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

DtStatus  DtaLmh1983InitPowerup(DtaLmh1983*  pLmh1983Data)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaDeviceData*  pDvcData = pLmh1983Data->m_pDvcData;
    
    Status = DtaLmh1983WriteInitSeq(pLmh1983Data);

    if (Status != DT_STATUS_OK)
        DtDbgOut(ERR, GENL, "Failed to initialize DTA-2154: 0x%x", Status);
    
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
                Status = DtaLmh1983GoToNextState(pLmh1983Data, DTA_LMH1983_POLL_EVENT);
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
    DtStatus  Status = DT_STATUS_OK;
    Int  OldState, NewState;

    // Acquire state lock
    DtFastMutexAcquire(&pLmh1983Data->m_StateMutex);

    // Store current state
    NewState = OldState = pLmh1983Data->m_State;
            
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

    case DTA_LMH1983_STATE_NOREF:

        // Re-check PLL lock state
        NewState = DtaLmh1983PllLockStateGet(pLmh1983Data);

        break;

    case DTA_LMH1983_STATE_PLL_LOCKING:

        // Re-check PLL lock state
        NewState = DtaLmh1983PllLockStateGet(pLmh1983Data);

        break;

    case DTA_LMH1983_STATE_PLL_LOCKED:

        // Re-check PLL lock state
        NewState = DtaLmh1983PllLockStateGet(pLmh1983Data);
        if (NewState != DTA_LMH1983_STATE_PLL_LOCKED)
            break;  // PLL is not locked anymore
        // Wait for start-of-frame event to begin alignment procedure
        else if (ChangeEvent == DTA_LMH1983_SOF_EVENT)
        {
            // Start TOF alignment
            if (DT_SUCCESS(DtaLmh1983StartTofAlign(pLmh1983Data)))
                NewState = DTA_LMH1983_STATE_ALIGN_TOF;
        }
        break;

    case DTA_LMH1983_STATE_ALIGN_TOF:

        // Re-check PLL lock state
        NewState = DtaLmh1983PllLockStateGet(pLmh1983Data);
        if (NewState != DTA_LMH1983_STATE_PLL_LOCKED)
        {
            // Lost lock during alignment => stop tof alignment procedure
            DtaLmh1983StopTofAlign(pLmh1983Data);
            break;
        }

        // Wait for start-of-frame to end aligment process
        if (ChangeEvent == DTA_LMH1983_SOF_EVENT)
        {
            Status = DtaLmh1983StopTofAlign(pLmh1983Data);
            if (DT_SUCCESS(Status))
                NewState = DTA_LMH1983_STATE_FULL_LOCK;
            else
                NewState = DTA_LMH1983_STATE_PLL_LOCKED;    // fall-back
        }
        break;

    case DTA_LMH1983_STATE_FULL_LOCK:
        // Re-check PLL lock state
        NewState = DtaLmh1983PllLockStateGet(pLmh1983Data);
        if (NewState != DTA_LMH1983_STATE_PLL_LOCKED)
            NewState = DTA_LMH1983_STATE_HOLD_OVER1; // Go to hold over state
        else
            NewState = DTA_LMH1983_STATE_FULL_LOCK; // Stay in current state
        break;

    case DTA_LMH1983_STATE_HOLD_OVER1:
        // Check PLL lock state
        NewState = DtaLmh1983PllLockStateGet(pLmh1983Data);
        if (NewState == DTA_LMH1983_STATE_PLL_LOCKED)
            NewState = DTA_LMH1983_STATE_FULL_LOCK;  // Go back to full lock
        else
            NewState = DTA_LMH1983_STATE_HOLD_OVER2; // Go to next hold over state
        break;

    case DTA_LMH1983_STATE_HOLD_OVER2:
        // Check PLL lock state
        NewState = DtaLmh1983PllLockStateGet(pLmh1983Data);
        if (NewState == DTA_LMH1982_STATE_PLL_LOCKED)
            NewState = DTA_LMH1982_STATE_FULL_LOCK;  // Go back to full lock
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
        DtDbgOut(MIN, GENL, "state transition %s => %s", DtaLmh1983State2Str(OldState), 
                                                           DtaLmh1983State2Str(NewState));
    // Fire lock state event
    if (OldState<DTA_LMH1983_STATE_FULL_LOCK && NewState==DTA_LMH1983_STATE_FULL_LOCK)
    {
        Status = DtaEventsSet(pLmh1983Data->m_pDvcData, NULL, DTA_EVENT_TYPE_GENLOCK, 
                                                              DTA_EVENT_VALUE1_LOCKED, 0);
        DT_ASSERT(DT_SUCCESS(Status));
    }
    else if (OldState>=DTA_LMH1983_STATE_FULL_LOCK && 
                                                     NewState<DTA_LMH1983_STATE_FULL_LOCK)
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
    Int  ClkSrc = 0;
    volatile UInt8*  pGenRegs = pLmh1983Data->m_pDvcData->m_pGenRegs;
    DtaGenlock*  pGenlock = &pLmh1983Data->m_pDvcData->m_Genlock;

    // Start with resetting LHM-1983
    DtaRegHdGenlClkConfSetAsyncReset(pGenRegs, 1);
    DtSleep(5);
    
    if (pGenlock->m_RefPortIndex  == DTA_GENLOCK_REFPORT_INT)
    {
        ClkSrc = 0;     // Internal clock
        *pRefVidStd = DT_VIDSTD_625I50;      // Internal clock always is 625I50, but we
                                              // can generate any output standard from it
        *pOutVidStd = pGenlock->m_RefVidStd;
    }
    else if (pGenlock->m_RefPortIndex == pGenlock->m_AsyncPortIndex)
    {
        ClkSrc = 15;    // analog clock
        *pRefVidStd = *pOutVidStd = pGenlock->m_RefVidStd;
    }
    else
    {
        ClkSrc = pGenlock->m_RefPortIndex+1;    // Use specified port
        *pRefVidStd = *pOutVidStd = pGenlock->m_RefVidStd;
    }

    // Set clock source
    DtaRegHdGenlClkConfSetRefSrc(pGenRegs, ClkSrc);

    // Set interlaced flags
    if (DtaVidStdIsInterlaced(*pRefVidStd))
        DtaRegHdGenlClkConfSetInterlaced(pGenRegs, 1);
    else
        DtaRegHdGenlClkConfSetInterlaced(pGenRegs, 0);

    // Finally remove reset
    DtaRegHdGenlClkConfSetAsyncReset(pGenRegs, 0);
    DtSleep(5);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983InitChip -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1983InitChip(DtaLmh1983* pLmh1983Data)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  RefVidStd,  OutVidStd;
    UInt8  FormatCode;

    Status = DtaLmh1983SetupRefSource(pLmh1983Data, &RefVidStd, &OutVidStd);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to set clock source. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }


    //Status = DtaLmh1983WriteInitSeq(pLmh1983Data);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to re-initialize LMH-1983 chip. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }

    switch (RefVidStd)
    {
    case DT_VIDSTD_525I59_94: FormatCode = 0; break;
    case DT_VIDSTD_625I50: FormatCode = 1; break;
    case DT_VIDSTD_720P23_98: FormatCode = 11; break;
    case DT_VIDSTD_720P24: FormatCode = 10; break;
    case DT_VIDSTD_720P25: FormatCode = 9; break;
    case DT_VIDSTD_720P29_97: FormatCode = 8; break;
    case DT_VIDSTD_720P30: FormatCode = 7; break;
    case DT_VIDSTD_720P50: FormatCode = 6; break;
    case DT_VIDSTD_720P59_94: FormatCode = 5; break;
    case DT_VIDSTD_720P60: FormatCode = 4; break;
    case DT_VIDSTD_1080P23_98: FormatCode = 19; break;
    case DT_VIDSTD_1080P24: FormatCode = 18; break;
    case DT_VIDSTD_1080P25: FormatCode = 17; break;
    case DT_VIDSTD_1080P29_97: FormatCode = 16; break;
    case DT_VIDSTD_1080P30: FormatCode = 15; break;
    case DT_VIDSTD_1080I50: FormatCode = 22; break;
    case DT_VIDSTD_1080I59_94: FormatCode = 21; break;
    case DT_VIDSTD_1080I60: FormatCode = 20; break;
    case DT_VIDSTD_1080P50: FormatCode = 14; break;
    case DT_VIDSTD_1080P59_94: FormatCode = 13; break;
    case DT_VIDSTD_1080P60: FormatCode = 12; break;
    default:
        DtDbgOut(ERR, GENL, "Unknown reference video standard: 0x%X", RefVidStd);
        FormatCode = 1;
        break;
    }
    Status = DtaLmh1983WriteReg(pLmh1983Data, DTA_LMH1983_REG_INPUT_FORMAT, FormatCode);
    if (!DT_SUCCESS(Status))
        DtDbgOut(ERR, GENL, "Failed to write input format register. Error: 0x%X", Status);
    DtDbgOut(ERR, GENL, "Formatcode: %d", FormatCode);

    if (DT_SUCCESS(Status))
    {
        UInt8  DvcCtrl;
        Status = DtaLmh1983ReadReg(pLmh1983Data, DTA_LMH1983_REG_DEVICE_CTRL, &DvcCtrl);
        // Set PLL1 operating mode to genlock
        DvcCtrl &= ~0x18;
        if (pLmh1983Data->m_pDvcData->m_Genlock.m_RefPortIndex != DTA_GENLOCK_REFPORT_INT)
            DvcCtrl |= 0x08;
        Status = DtaLmh1983WriteReg(pLmh1983Data, DTA_LMH1983_REG_DEVICE_CTRL, DvcCtrl);
    }


    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983PllLockStateGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaLmh1983PllLockStateGet(DtaLmh1983* pLmh1983Data)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  DvcStatus1=0, DvcStatus2=0;
    Int  State = DTA_LMH1983_STATE_NOREF;
        
    Status = DtaLmh1983ReadReg(pLmh1983Data, DTA_LMH1983_REG_DEVICE_STATUS1, &DvcStatus1);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to read device status - input ref. Error: 0x%x",
                                                                                  Status);
        return State;
    }
    Status = DtaLmh1983ReadReg(pLmh1983Data, DTA_LMH1983_REG_DEVICE_STATUS2, &DvcStatus2);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to read device status. Error: 0x%x", Status);
        return State;
    }
    // Check LOR_STATUS first
    if ((DvcStatus1 & 0x04) == 0)
        State = DTA_LMH1983_STATE_NOREF;        // Invalid reference signal
    else if ((DvcStatus2 & 0x80) == 0x80)
        State = DTA_LMH1983_STATE_PLL_LOCKED;   // Have lock
    else
        State = DTA_LMH1983_STATE_PLL_LOCKING;  // Valid reference, but PLL is not locked

    return State;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983StartTofAlign -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1983StartTofAlign(DtaLmh1983* pLmh1983Data)
{
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1983StopTofAlign -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1983StopTofAlign(DtaLmh1983* pLmh1983Data)
{
    return DT_STATUS_OK;
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
    case DTA_LMH1983_STATE_PLL_LOCKING:     return "PLL_LOCKING";
    case DTA_LMH1983_STATE_PLL_LOCKED:      return "PLL_LOCKED";
    case DTA_LMH1983_STATE_ALIGN_TOF:       return "ALIGN_TOF";
    case DTA_LMH1983_STATE_FULL_LOCK:       return "FULL_LOCK";
    case DTA_LMH1983_STATE_HOLD_OVER1:      return "HOLD_OVER1";
    case DTA_LMH1983_STATE_HOLD_OVER2:      return "HOLD_OVER2";
    default:                                return "UNKNOWN";
    }
}
#endif
