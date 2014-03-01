//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Lmh1982.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2012 DekTec
//
// Dta driver - National LMH1982 (Video Clock Generator) controller

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012 DekTec Digital Video B.V.
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

#define DTA_LMH1982_I2C_TIMEOUT     100

#define DTA_LMH1982_POLL_EVENT      0
#define DTA_LMH1982_SOF_EVENT       1

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static void  DtaLmh1982ControllerThread(DtThread* pThread, void* pContext);
static DtStatus  DtaLmh1982GoToNextState(DtaLmh1982* pLmh1982Data, Int ChangeEvent);
static DtStatus  DtaLmh1982InitChip(DtaLmh1982* pLmh1982Data);
static Int  DtaLmh1982PllLockStateGet(DtaLmh1982* pLmh1982Data);
static DtStatus  DtaLmh1982ReadReg(DtaLmh1982* pLmh1982Data, UInt8, UInt8*);
static DtStatus  DtaLmh1982ReadReg16(DtaLmh1982* pLmh1982Data, UInt8, UInt16*);
static DtStatus  DtaLmh1982WriteReg(DtaLmh1982* pLmh1982Data, UInt8, UInt8);
static DtStatus  DtaLmh1982WriteReg16(DtaLmh1982* pLmh1982Data, UInt8, UInt16);
//static DtStatus  DtaLmh1982CacheRefresh(DtaLmh1982* pLmh1982Data);
static DtStatus  DtaLmh1982CacheWrite(DtaLmh1982* pLmh1982Data);
static DtStatus  DtaLmh1982StartTofAlign(DtaLmh1982* pLmh1982Data);
static DtStatus  DtaLmh1982StopTofAlign(DtaLmh1982* pLmh1982Data);
static DtStatus  DtaLmh1982SetupRefSource(DtaLmh1982* pLmh1982Data, Int* pRefVidStd,
                                                                          Int* pOutVidStd);
static Int  DtaLmh1982VidStd2Lpfm(Int VidStd);
static const char*  DtaLmh1982State2Str(Int State);

// Debug helpers
#ifdef _DEBUG
static DtStatus  DtaLmh1982CachePrint(DtaLmh1982* pLmh1982Data);
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982CleanupPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaLmh1982CleanupPre(DtaDeviceData* pDvcData, DtaLmh1982* pLmh1982Data)
{
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982Cleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaLmh1982Cleanup(DtaDeviceData* pDvcData, DtaLmh1982* pLmh1982Data)
{
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1982Init(DtaDeviceData* pDvcData, DtaLmh1982* pLmh1982Data)
{
    pLmh1982Data->m_pDvcData = pDvcData;

    // Init state mutex
    DtFastMutexInit(&pLmh1982Data->m_StateMutex);
    
    // Set initial state of state machine
    pLmh1982Data->m_State = DTA_LMH1982_STATE_INIT;

    // Init controller thread
    DtThreadInit(&pLmh1982Data->m_ControlThread, DtaLmh1982ControllerThread, pLmh1982Data);
    
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982InitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaLmh1982InitPowerup(DtaLmh1982* pLmh1982Data)
{
    DtStatus  Status = DT_STATUS_OK;
    DtThreadStart(&pLmh1982Data->m_ControlThread);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982ResetStateMachine -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1982ResetStateMachine(DtaLmh1982* pLmh1982Data)
{
    // Acquire state mutex and reset state to initial value
    DtFastMutexAcquire(&pLmh1982Data->m_StateMutex);
    pLmh1982Data->m_State = DTA_LMH1982_STATE_INIT;
    DtFastMutexRelease(&pLmh1982Data->m_StateMutex);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982PowerdownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1982PowerdownPre(DtaLmh1982* pLmh1982Data)
{
    DtStatus  Status = DT_STATUS_OK;
    Status = DtThreadStop(&pLmh1982Data->m_ControlThread);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982Powerdown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaLmh1982Powerdown(DtaLmh1982* pLmh1982Data)
{ 
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982ControllerThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaLmh1982ControllerThread(DtThread* pThread, void* pContext)
{
    Int  TimeoutCnt = 0;
    DtStatus  Status = DT_STATUS_OK;
    DtaLmh1982*  pLmh1982Data = (DtaLmh1982*)pContext;

    DtDbgOut(MAX, GENL, "Thread begin");
        
    TimeoutCnt = 0;
    while (!pThread->m_StopThread)
    {
        // Wait for SOF event (or timeout) 
        Status = DtEventWait(&pLmh1982Data->m_pDvcData->m_Matrix.m_SofFrameSyncEvent, 10);

        if (Status == DT_STATUS_OK)
        {
            // Clear event
            DtEventReset(&pLmh1982Data->m_pDvcData->m_Matrix.m_SofFrameSyncEvent);
            // Update state-machine
            Status = DtaLmh1982GoToNextState(pLmh1982Data, DTA_LMH1982_SOF_EVENT);
            
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
                Status = DtaLmh1982GoToNextState(pLmh1982Data, DTA_LMH1982_POLL_EVENT);
                TimeoutCnt = 0;
            }
        }
    }

    DtDbgOut(MAX, GENL, "Thread exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982GoToNextState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtaLmh1982GoToNextState(DtaLmh1982* pLmh1982Data, Int ChangeEvent)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  OldState, NewState;

    // Acquire state lock
    DtFastMutexAcquire(&pLmh1982Data->m_StateMutex);

    // Store current state
    NewState = OldState = pLmh1982Data->m_State;
            
    switch (OldState)
    {
    case DTA_LMH1982_STATE_INIT:
        // Init LMH-1982 chip
        Status = DtaLmh1982InitChip(pLmh1982Data);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, GENL, "Failed to initialise LMH1982");

            // Release state lock
            DtFastMutexRelease(&pLmh1982Data->m_StateMutex);
            return DT_STATUS_FAIL;
        }
        else 
            NewState = DTA_LMH1982_STATE_NOREF;

        // Fire a no-lock event
        Status = DtaEventsSet(pLmh1982Data->m_pDvcData, NULL, DTA_EVENT_TYPE_GENLOCK, 
                                                             DTA_EVENT_VALUE1_NO_LOCK, 0);
        DT_ASSERT(DT_SUCCESS(Status));
        break;

    case DTA_LMH1982_STATE_NOREF:

        // Re-check PLL lock state
        NewState = DtaLmh1982PllLockStateGet(pLmh1982Data);

        break;

    case DTA_LMH1982_STATE_PLL_LOCKING:

        // Re-check PLL lock state
        NewState = DtaLmh1982PllLockStateGet(pLmh1982Data);

        break;

    case DTA_LMH1982_STATE_PLL_LOCKED:

        // Re-check PLL lock state
        NewState = DtaLmh1982PllLockStateGet(pLmh1982Data);
        if (NewState != DTA_LMH1982_STATE_PLL_LOCKED)
            break;  // PLL is not locked anymore
        // Wait for start-of-frame event to begin alignment procedure
        else if (ChangeEvent == DTA_LMH1982_SOF_EVENT)
        {
            // Start TOF alignment
            if (DT_SUCCESS(DtaLmh1982StartTofAlign(pLmh1982Data)))
                NewState = DTA_LMH1982_STATE_ALIGN_TOF;
        }
        break;

    case DTA_LMH1982_STATE_ALIGN_TOF:

        // Re-check PLL lock state
        NewState = DtaLmh1982PllLockStateGet(pLmh1982Data);
        if (NewState != DTA_LMH1982_STATE_PLL_LOCKED)
        {
            // Lost lock during alignment => stop tof alignment procedure
            DtaLmh1982StopTofAlign(pLmh1982Data);
            break;
        }

        // Wait for start-of-frame to end aligment process
        if (ChangeEvent == DTA_LMH1982_SOF_EVENT)
        {
            Status = DtaLmh1982StopTofAlign(pLmh1982Data);
            if (DT_SUCCESS(Status))
                NewState = DTA_LMH1982_STATE_FULL_LOCK;
            else
                NewState = DTA_LMH1982_STATE_PLL_LOCKED;    // fall-back
        }
        break;

    case DTA_LMH1982_STATE_FULL_LOCK:
        // Re-check PLL lock state
        NewState = DtaLmh1982PllLockStateGet(pLmh1982Data);
        if (NewState != DTA_LMH1982_STATE_PLL_LOCKED)
            NewState = DTA_LMH1982_STATE_HOLD_OVER1; // Go to hold over state
        else
            NewState = DTA_LMH1982_STATE_FULL_LOCK; // Stay in current state
        break;

    case DTA_LMH1982_STATE_HOLD_OVER1:
        // Check PLL lock state
        NewState = DtaLmh1982PllLockStateGet(pLmh1982Data);
        if (NewState == DTA_LMH1982_STATE_PLL_LOCKED)
            NewState = DTA_LMH1982_STATE_FULL_LOCK;  // Go back to full lock
        else
            NewState = DTA_LMH1982_STATE_HOLD_OVER2; // Go to next hold over state
        break;

    case DTA_LMH1982_STATE_HOLD_OVER2:
        // Check PLL lock state
        NewState = DtaLmh1982PllLockStateGet(pLmh1982Data);
        if (NewState == DTA_LMH1982_STATE_PLL_LOCKED)
            NewState = DTA_LMH1982_STATE_FULL_LOCK;  // Go back to full lock
        break;

    default:
        // Unexpected state => reset state-machine
        DtDbgOut(ERR, GENL, "UNKONWN STATE %d!!!!", OldState);
        NewState =  DTA_LMH1982_STATE_INIT;
        break;
    }

    // Safe new state
    pLmh1982Data->m_State = NewState;

    // Release state lock
    DtFastMutexRelease(&pLmh1982Data->m_StateMutex);

    if (OldState != NewState)
        DtDbgOut(MIN, GENL, "state transition %s => %s", DtaLmh1982State2Str(OldState), 
                                                           DtaLmh1982State2Str(NewState));
    // Fire lock state event
    if (OldState<DTA_LMH1982_STATE_FULL_LOCK && NewState==DTA_LMH1982_STATE_FULL_LOCK)
    {
        Status = DtaEventsSet(pLmh1982Data->m_pDvcData, NULL, DTA_EVENT_TYPE_GENLOCK, 
                                                              DTA_EVENT_VALUE1_LOCKED, 0);
        DT_ASSERT(DT_SUCCESS(Status));
    }
    else if (OldState>=DTA_LMH1982_STATE_FULL_LOCK && 
                                                     NewState<DTA_LMH1982_STATE_FULL_LOCK)
    {
        Status = DtaEventsSet(pLmh1982Data->m_pDvcData, NULL, DTA_EVENT_TYPE_GENLOCK, 
                                                             DTA_EVENT_VALUE1_NO_LOCK, 0);
        DT_ASSERT(DT_SUCCESS(Status));
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982InitChip -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1982InitChip(DtaLmh1982* pLmh1982Data)
{
    DtStatus  Status = DT_STATUS_OK;
    Int Icp1NanoAmp, Icp1;
    DtaLmh1982Regs*  pRegs = &pLmh1982Data->m_RegCache;
    DtaGenlock*  pGenlock = &pLmh1982Data->m_pDvcData->m_Genlock;
    Bool  IsFrac = FALSE;
    Int  RefVidStd,  OutVidStd, Fps, TofVidStd;
    UInt16 TofRst25Fps = 1;	    // Helper, required for calibration of genlock parameters

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 1: Set clock source -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    Status = DtaLmh1982SetupRefSource(pLmh1982Data, &RefVidStd, &OutVidStd);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to set clock source. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }
    
    //-.-.-.-.-.-.-.-.-.-.-.-.- Step 2: Init with default values -.-.-.-.-.-.-.-.-.-.-.-.-

    // Init cache to default values
    pRegs->m_Reg00.All      = 0xA3;
    pRegs->m_Reg01.All      = 0x86;
    pRegs->m_Reg02.All      = 0x00;
    pRegs->m_Reg03.All      = 0x01;
    pRegs->m_Reg04_05.All   = 0x06B4;
    pRegs->m_Reg06.All      = 0x00;
    pRegs->m_Reg07          = 0x00;
    pRegs->m_Reg08.All      = 0x04;
    pRegs->m_Reg09_0A.All   = 0x0001;       // TOF_RST = 1
    pRegs->m_Reg0B_0C.All   = 0x06B4;
    pRegs->m_Reg0D_0E.All   = 0x020D;
    pRegs->m_Reg0F_10.All   = 0x020D;
    pRegs->m_Reg11_12.All   = 0x0000;
    pRegs->m_Reg13.All      = 0x88;
    pRegs->m_Reg14.All      = 0x88;

    // Write cache to chip
    Status = DtaLmh1982CacheWrite(pLmh1982Data);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to init cache. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }

    //-.-.-.-.-.-.-.-.- Step 3.1: Special initialisation for 148.35 MHz -.-.-.-.-.-.-.-.-.
    //
    // 148.35 MHz PLL Initialization Sequence the following programming sequence is
    // required to initialize PLL 3 and generate a correct 148.35 MHz output once it is
    // selected as the HD_CLK; otherwise, the clock may have duty cycle errors, frequency 
    // errors, and/or high jitter. This PLL initialization sequence must be programmed 
    // after switching from another HD clock frequency or Hi-Z mode, as well as after a
    // device reset or power cycle condition. Each programming step below represents 
    // a separate write sequence.
    // 1. Program HD_FREQ = 11b and HD_HIZ = 0 (register 08h)
    // to select 148.35 MHz and enable the HD_CLK output.
    // 2. Program a value of 1 to the following register parameters
    // (a single write sequence is valid for this step):
    // — FB_DIV = 1 (register 04h-05h)
    // — TOF_RST = 1 (register 09h-0Ah)
    // — REF_LPFM = 1 (register 0Fh-10h)
    // — EN_TOF_RST = 1 (register 0Ah)
    // 3. Wait at least 2 cycles of the 27 MHz VCXO clock, then
    // program EN_TOF_RST = 0.
    // After this sequence is completed, the 148.35 MHz clock will operate correctly 
    // and normal device configuration can resume. All other output clocks do not 
    // require this initialization sequence for proper clock operation.

    if (DtaVidStdIsFractional(OutVidStd))
    {
        DtDbgOut(MIN, GENL, "Switching to DTA_LMH1982_IIC_HD_FREQ_148_35");

        pRegs->m_Reg08.Fields.m_HdFreq = DTA_LMH1982_IIC_HD_FREQ_148_35;
        Status = DtaLmh1982WriteReg(pLmh1982Data, DTA_LMH1982_IIC_REG08_8B,
                                                                      pRegs->m_Reg08.All);
        if (!DT_SUCCESS(Status))
            return DT_STATUS_FAIL;

        pRegs->m_Reg04_05.Fields.m_FbDiv = 1;
        Status = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG04_16B,
                                                                   pRegs->m_Reg04_05.All);
        if (!DT_SUCCESS(Status))
            return DT_STATUS_FAIL;

        pRegs->m_Reg09_0A.Fields.m_TofRst = 1;
        Status = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG09_16B, 
                                                                   pRegs->m_Reg09_0A.All);
        if (!DT_SUCCESS(Status))
            return DT_STATUS_FAIL;

        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 1;
        Status = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG0F_16B, 
                                                                   pRegs->m_Reg0F_10.All);
        if (!DT_SUCCESS(Status))
            return DT_STATUS_FAIL;

        pRegs->m_Reg09_0A.Fields.m_EnTofRst = 1;
        Status = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG09_16B, 
                                                                   pRegs->m_Reg09_0A.All);
        if (!DT_SUCCESS(Status))
            return DT_STATUS_FAIL;

        // Wait at least 2 27Mhz cycles
        DtSleep(1);
        pRegs->m_Reg09_0A.Fields.m_EnTofRst = 0;
        Status = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG09_16B, 
                                                                   pRegs->m_Reg09_0A.All);
        if (!DT_SUCCESS(Status))
            return DT_STATUS_FAIL;
    }
    else
    {
        DtDbgOut(MIN, GENL, "Switching to DTA_LMH1982_IIC_HD_FREQ_148_5");
        pRegs->m_Reg08.Fields.m_HdFreq = DTA_LMH1982_IIC_HD_FREQ_148_5;
    }
    
    //.-.-.-.-.-.-.-.-.-.-.-.- Step 3.2: Set Reference Registers -.-.-.-.-.-.-.-.-.-.-.-.-

    // Set reference lines per frame, feedback divider and reference divider selection
    switch (RefVidStd)
    {
    case DT_VIDSTD_525I59_94:   
        pRegs->m_Reg04_05.Fields.m_FbDiv = 1716;
        pRegs->m_Reg03.Fields.m_RefDivSel = 1;     // PLL 1 divider = 1
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 525;
        TofRst25Fps = 1200;
        break;

    case DT_VIDSTD_625I50:      
        pRegs->m_Reg04_05.Fields.m_FbDiv = 1728;
        pRegs->m_Reg03.Fields.m_RefDivSel = 1;     // PLL 1 divider = 1
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 625;
        TofRst25Fps = 1;
        break;

    case DT_VIDSTD_720P60:      
        pRegs->m_Reg04_05.Fields.m_FbDiv = 600;
        pRegs->m_Reg03.Fields.m_RefDivSel = 1;     // PLL 1 divider = 1
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 750;
        TofRst25Fps = 12;
        break;

    case DT_VIDSTD_720P59_94:
        pRegs->m_Reg04_05.Fields.m_FbDiv = 3003;
        pRegs->m_Reg03.Fields.m_RefDivSel = 2;     // PLL 1 divider = 5
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 750;
        TofRst25Fps = 2400;
        break;

    case DT_VIDSTD_720P50:      
        pRegs->m_Reg04_05.Fields.m_FbDiv = 3600;
        pRegs->m_Reg03.Fields.m_RefDivSel = 2;     // PLL 1 divider = 5
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 750/5;
        TofRst25Fps = 2;
        break;

    case DT_VIDSTD_720P30:      
        pRegs->m_Reg04_05.Fields.m_FbDiv = 1200;
        pRegs->m_Reg03.Fields.m_RefDivSel = 1;     // PLL 1 divider = 1
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 750;
        TofRst25Fps = 6;
        break;

    case DT_VIDSTD_720P29_97:
        pRegs->m_Reg04_05.Fields.m_FbDiv = 6006;
        pRegs->m_Reg03.Fields.m_RefDivSel = 2;     // PLL 1 divider = 5
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 750;
        TofRst25Fps = 1200;
        break;

    case DT_VIDSTD_720P25:      
        pRegs->m_Reg04_05.Fields.m_FbDiv = 1440;
        pRegs->m_Reg03.Fields.m_RefDivSel = 1;     // PLL 1 divider = 1
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 750;
        TofRst25Fps = 1;
        break;

    case DT_VIDSTD_720P24:      
        pRegs->m_Reg04_05.Fields.m_FbDiv = 1500;
        pRegs->m_Reg03.Fields.m_RefDivSel = 1;     // PLL 1 divider = 1
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 750;
        TofRst25Fps = 24;
        break;

    case DT_VIDSTD_720P23_98:   
        pRegs->m_Reg04_05.Fields.m_FbDiv = 3003;
        pRegs->m_Reg03.Fields.m_RefDivSel = 0;     // PLL 1 divider = 2
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 750;
        TofRst25Fps = 3003;
        break;

    case DT_VIDSTD_1080P60:     
        pRegs->m_Reg04_05.Fields.m_FbDiv = 400;
        pRegs->m_Reg03.Fields.m_RefDivSel = 1;     // PLL 1 divider = 1
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 1125;
        TofRst25Fps = 12;
        break;

    case DT_VIDSTD_1080P59_94:  
        pRegs->m_Reg04_05.Fields.m_FbDiv = 2002;
        pRegs->m_Reg03.Fields.m_RefDivSel = 2;     // PLL 1 divider = 5
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 1125;
        TofRst25Fps = 2400;
        break;
 
    case DT_VIDSTD_1080P50:     
        pRegs->m_Reg04_05.Fields.m_FbDiv = 2400;
        pRegs->m_Reg03.Fields.m_RefDivSel = 2;     // PLL 1 divider = 5
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 1125/5;
        TofRst25Fps = 2;
        break;

    case DT_VIDSTD_1080P30:
    case DT_VIDSTD_1080I60:
        pRegs->m_Reg04_05.Fields.m_FbDiv = 800;
        pRegs->m_Reg03.Fields.m_RefDivSel = 1;     // PLL 1 divider = 1
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 1125;

        TofRst25Fps = 6;
        break;

    case DT_VIDSTD_1080I59_94:  
    case DT_VIDSTD_1080P29_97:  
        pRegs->m_Reg04_05.Fields.m_FbDiv = 4004;
        pRegs->m_Reg03.Fields.m_RefDivSel = 2;     // PLL 1 divider = 5
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 1125;

        TofRst25Fps = 1200;
        break;

    case DT_VIDSTD_1080P25:
    case DT_VIDSTD_1080I50:     
        pRegs->m_Reg04_05.Fields.m_FbDiv = 4800;
        pRegs->m_Reg03.Fields.m_RefDivSel = 2;     // PLL 1 divider = 5
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 1125/5;
        TofRst25Fps = 1;
        break;

    case DT_VIDSTD_1080P24:	
        pRegs->m_Reg04_05.Fields.m_FbDiv = 1000;
        pRegs->m_Reg03.Fields.m_RefDivSel = 1;     // PLL 1 divider = 1
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 1125;
        TofRst25Fps = 24;
        break;

    case DT_VIDSTD_1080P23_98:
        pRegs->m_Reg04_05.Fields.m_FbDiv = 1001;
        pRegs->m_Reg03.Fields.m_RefDivSel = 1;     // PLL 1 divider = 1
        pRegs->m_Reg0F_10.Fields.m_RefLpfm = 1125;
        TofRst25Fps = 3003;
        break;

    default:
        DtDbgOut(ERR, GENL, "UNKNOWN REFERENCE VIDSTD (%d)", RefVidStd);
        return DT_STATUS_FAIL;
    }

    // Set reference input A polarity bits
    pRegs->m_Reg02.Fields.m_PolVa = 0;
    pRegs->m_Reg02.Fields.m_PolHa = 0;

    // Set Tof pulse polarity
    pRegs->m_Reg09_0A.Fields.m_PolTof = 1;

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 4: TOF timing registers -.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Select a video standard with matching frame rate to the desired output standard
    Fps = DtaVidStd2Fps(OutVidStd);
    IsFrac = DtaVidStdIsFractional(OutVidStd);

    switch(Fps)
    {
    case 24:
        TofVidStd = IsFrac ? DT_VIDSTD_720P23_98 : DT_VIDSTD_720P24;
        break;

    case 25: 
        TofVidStd = DT_VIDSTD_720P25; 
        break;

    case 30: 
        // Both DT_VIDSTD_720P29_97 AND DT_VIDSTD_720P30 have issues
        // TofVidStd = IsFrac ? DT_VIDSTD_720P29_97 : DT_VIDSTD_720P30;	
        
        // Workaround by using DT_VIDSTD_1080I59_94 and DT_VIDSTD_1080I60 instead
        TofVidStd = IsFrac ? DT_VIDSTD_1080I59_94 : DT_VIDSTD_1080I60;
        break;

    case 50: 
        TofVidStd = DT_VIDSTD_720P50;
        break;

    case 60: 
        TofVidStd = IsFrac ? DT_VIDSTD_720P59_94 : DT_VIDSTD_720P60; 
        break;

    default: 
        DtDbgOut(ERR, GENL, "UNSUPPORTED FRAME RATE %d (OutVidStd=%d)", Fps, OutVidStd);
        return DT_STATUS_FAIL;
    }

    // Init TOF reset to default value of 1
    pRegs->m_Reg09_0A.Fields.m_TofRst = 1;

    // Set TOF offset based genref source and video standard
    if (DtaVidStdIsInterlaced(RefVidStd))
    {
        // Check for 'normal' port
        if (pGenlock->m_RefPortIndex!=DTA_GENLOCK_REFPORT_INT 
                                  && pGenlock->m_RefPortIndex!=pGenlock->m_AsyncPortIndex)
        {
            // Verified for Digital Sync SDI-625, measured delta -140 symbols, 
            // sofline = 0x270
            pRegs->m_Reg11_12.Fields.m_TofOffset = (pRegs->m_Reg0F_10.Fields.m_RefLpfm/2);
        }
        else	
        {
            // Verified for Analog Sync SDI-625, measured delta -040 symbols, sofline = 0
            pRegs->m_Reg11_12.Fields.m_TofOffset = 
                                               (pRegs->m_Reg0F_10.Fields.m_RefLpfm/2) - 1;
        }
    } 
    else 
    {

        if (pGenlock->m_RefPortIndex!=DTA_GENLOCK_REFPORT_INT 
                                  && pGenlock->m_RefPortIndex!=pGenlock->m_AsyncPortIndex)
            pRegs->m_Reg11_12.Fields.m_TofOffset = pRegs->m_Reg0F_10.Fields.m_RefLpfm;
        else
            pRegs->m_Reg11_12.Fields.m_TofOffset = pRegs->m_Reg0F_10.Fields.m_RefLpfm-1;
    }

    // Set TOF Clock, TOF pixels per line and TOF lines per frame     
    switch (TofVidStd)
    {
    case DT_VIDSTD_720P23_98:
        pRegs->m_Reg0B_0C.Fields.m_TofClk = 0;			// 27 MHZ
        pRegs->m_Reg0B_0C.Fields.m_TofPpl = 3003;		
        pRegs->m_Reg0D_0E.Fields.m_TofLpfm = 375;
        // Enable crosslock from 25fps to 23.98fps
        if (pGenlock->m_RefPortIndex == DTA_GENLOCK_REFPORT_INT)
            pRegs->m_Reg09_0A.Fields.m_TofRst = 3125;
        break;

    case DT_VIDSTD_720P24:
        pRegs->m_Reg0B_0C.Fields.m_TofClk = 0;
        pRegs->m_Reg0B_0C.Fields.m_TofPpl = 1500;
        pRegs->m_Reg0D_0E.Fields.m_TofLpfm = 750;
        // Enable crosslock from 25fps to 24fps
        if (pGenlock->m_RefPortIndex == DTA_GENLOCK_REFPORT_INT)
            pRegs->m_Reg09_0A.Fields.m_TofRst = 25;
        break;

    case DT_VIDSTD_720P25:
        pRegs->m_Reg0B_0C.Fields.m_TofClk = 0;			// 27 MHZ
        pRegs->m_Reg0B_0C.Fields.m_TofPpl = 1440;		
        pRegs->m_Reg0D_0E.Fields.m_TofLpfm = 750;
        // Enable crosslock from 25fps to 25fps
        if (pGenlock->m_RefPortIndex == DTA_GENLOCK_REFPORT_INT)
            pRegs->m_Reg09_0A.Fields.m_TofRst = 1;
        else
        {
            // For genlock tests we need to be able to crosslock from all external 
            // standards to SDI625
            // Note: the TOF will not be aligned, but delta between two SD signals generated 
            // by two different 2152 boards that are crosslocked to the same standard 
            // will be the constant	
            pRegs->m_Reg09_0A.Fields.m_TofRst = TofRst25Fps;
        }
        break;

    case DT_VIDSTD_720P29_97:
        // Note: has some issues. Not used for the moment
        pRegs->m_Reg0B_0C.Fields.m_TofClk = 0;			// 27 MHZ
        pRegs->m_Reg0B_0C.Fields.m_TofPpl = 6006;		
        pRegs->m_Reg0D_0E.Fields.m_TofLpfm = 150;
        // Enable crosslock from 25fps to 29.97fps
        if (pGenlock->m_RefPortIndex == DTA_GENLOCK_REFPORT_INT)
            pRegs->m_Reg09_0A.Fields.m_TofRst = 1001;
        break;

    case DT_VIDSTD_1080I59_94:
        // Used for generating 59.94 FPS instead of DT_VIDSTD_720P29_97
        pRegs->m_Reg0B_0C.Fields.m_TofClk = 0;			// 27 MHZ
        pRegs->m_Reg0B_0C.Fields.m_TofPpl = 4004;		
        pRegs->m_Reg0D_0E.Fields.m_TofLpfm = 225;
        // Enable crosslock from 25fps to 29.97fps
        if (pGenlock->m_RefPortIndex == DTA_GENLOCK_REFPORT_INT)
            pRegs->m_Reg09_0A.Fields.m_TofRst = 1001;
        break;

    case DT_VIDSTD_720P30:
        // Note: has some issues. Not used for the moment
        pRegs->m_Reg0B_0C.Fields.m_TofClk = 0;			// 27 MHZ
        pRegs->m_Reg0B_0C.Fields.m_TofPpl = 1200;		
        pRegs->m_Reg0D_0E.Fields.m_TofLpfm = 150;
        // Enable crosslock from 25fps to 30fps
        if (pGenlock->m_RefPortIndex == DTA_GENLOCK_REFPORT_INT)
            pRegs->m_Reg09_0A.Fields.m_TofRst = 5;
        break;

    case DT_VIDSTD_1080I60:
        // Used for generating 60.0 FPS instead of DT_VIDSTD_720P30
        pRegs->m_Reg0B_0C.Fields.m_TofClk = 0;			// 27 MHZ
        pRegs->m_Reg0B_0C.Fields.m_TofPpl = 800;		
        pRegs->m_Reg0D_0E.Fields.m_TofLpfm = 1125;
        // Enable crosslock from 25fps to 30fps
        if (pGenlock->m_RefPortIndex == DTA_GENLOCK_REFPORT_INT)
            pRegs->m_Reg09_0A.Fields.m_TofRst = 5;
        break;

    case DT_VIDSTD_720P50:
        pRegs->m_Reg0B_0C.Fields.m_TofClk = 0;			// 27 MHZ
        pRegs->m_Reg0B_0C.Fields.m_TofPpl = 720;		
        pRegs->m_Reg0D_0E.Fields.m_TofLpfm = 750;
        // Enable crosslock from 25fps to 50fps
        if (pGenlock->m_RefPortIndex == DTA_GENLOCK_REFPORT_INT)
            pRegs->m_Reg09_0A.Fields.m_TofRst = 1;
        break;

    case DT_VIDSTD_720P59_94:
        pRegs->m_Reg0B_0C.Fields.m_TofClk = 0;			// 27 MHZ
        pRegs->m_Reg0B_0C.Fields.m_TofPpl = 3003;		
        pRegs->m_Reg0D_0E.Fields.m_TofLpfm = 150;
        // Enable crosslock from 25fps to 59.94fps
        if (pGenlock->m_RefPortIndex == DTA_GENLOCK_REFPORT_INT)
            pRegs->m_Reg09_0A.Fields.m_TofRst = 1001;

    case DT_VIDSTD_720P60:
        pRegs->m_Reg0B_0C.Fields.m_TofClk = 0;			// 27 MHZ
        pRegs->m_Reg0B_0C.Fields.m_TofPpl = 600;		
        pRegs->m_Reg0D_0E.Fields.m_TofLpfm = 750;
        // Enable crosslock from 25fps to 60fps
        if (pGenlock->m_RefPortIndex == DTA_GENLOCK_REFPORT_INT)
            pRegs->m_Reg09_0A.Fields.m_TofRst = 5;
        break;

    default: 
        DtDbgOut(ERR, GENL, "UNKNOWN TOF VIDEO STANDARD (TofVidStd=%d)", TofVidStd);
        return DT_STATUS_FAIL;
    }
    
    //-.-.-.-.-.-.-.-.-.- Step 5: Set 'miscellaneous' other registers  -.-.-.-.-.-.-.-.-.-

    // Set H-ERROR to max
    pRegs->m_Reg00.Fields.m_HError = 0x7;

    // Set PLL charge pumps
    pRegs->m_Reg14.Fields.m_Icp2 = 0x08;
    pRegs->m_Reg14.Fields.m_Icp3 = 0x08;
    
    // Set PLL charge pump current for low jitter operation. Compute value from 
    // FbDiv: -3dB loop bandwidth(BW) = 3Hz, Rs=20k,
    // Kvco=900 Icp1 = BW * FbDiv / (Rs * Kvco) = 3 * FbDiv / (20k * 900)
    // Compute in nA for rounding
    Icp1NanoAmp = (((Int)(pRegs->m_Reg04_05.Fields.m_FbDiv) * 3) / 18) * 1000;
    Icp1 = (Int)((Icp1NanoAmp + 15624) / 31250);
    // Clip to 0x1F
    if (Icp1>0x1F) 
        pRegs->m_Reg13.Fields.m_Icp1 = 0x1F;
    // Min. value if 0x2
    else if (Icp1<0x02) 
        pRegs->m_Reg13.Fields.m_Icp1 = 0x02;
    else
        pRegs->m_Reg13.Fields.m_Icp1 = (UInt8)(Icp1 & 0x1F);

    // Enable holdover mode
    pRegs->m_Reg00.Fields.m_HoldOver = 1;
    // Donot enable Genlock mode yet, done after writing all settings
    pRegs->m_Reg00.Fields.m_Gnlk = 0;

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 6: Write cache to chip -.-.-.-.-.-.-.-.-.-.-.-.-.-.

    Status = DtaLmh1982CacheWrite(pLmh1982Data);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to write reg-cache to LMH1982. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }

    // Finally enable genlock mode
    pRegs->m_Reg00.Fields.m_Gnlk = 1;
    Status = DtaLmh1982WriteReg(pLmh1982Data, DTA_LMH1982_IIC_REG00_8B, pRegs->m_Reg00.All);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to enable genlock mode. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982SetupRefSource -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaLmh1982SetupRefSource(
    DtaLmh1982*  pLmh1982Data, 
    Int*  pRefVidStd,
    Int*  pOutVidStd)
{
    Int  ClkSrc = 0;
    volatile UInt8*  pGenRegs = pLmh1982Data->m_pDvcData->m_pGenRegs;
    DtaGenlock*  pGenlock = &pLmh1982Data->m_pDvcData->m_Genlock;

    // Start with resetting LHM-1982
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982StartTofAlign -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1982StartTofAlign(DtaLmh1982* pLmh1982Data)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaLmh1982Regs*  pRegs = &pLmh1982Data->m_RegCache;
    
    DtDbgOut(AVG, GENL, "Starting TOF alignment");

    // Set EN_TOF_RST=1 and TOF_INIT=0 to enable alignment mode
    pRegs->m_Reg09_0A.Fields.m_EnTofRst = 1;
    pRegs->m_Reg09_0A.Fields.m_TofInit = 0;
    // Write now
    Status = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG09_16B,
                                                                   pRegs->m_Reg09_0A.All);
    if (!DT_SUCCESS(Status))
        return DT_STATUS_FAIL;

    // Now set TOF_INIT=1 to reset internal counters
    pRegs->m_Reg09_0A.Fields.m_TofInit = 1;
    // Write now
    Status = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG09_16B,
                                                                   pRegs->m_Reg09_0A.All);
    if (!DT_SUCCESS(Status))
        return DT_STATUS_FAIL;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982StopTofAlign -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1982StopTofAlign(DtaLmh1982* pLmh1982Data)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaLmh1982Regs*  pRegs = &pLmh1982Data->m_RegCache;
    
    DtDbgOut(AVG, GENL, "Stop TOF alignment");

    // Clear TOF_INIT=0 to stop alignment mode
    pRegs->m_Reg09_0A.Fields.m_TofInit = 0;
    // Write now
    Status = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG09_16B, 
                                                                   pRegs->m_Reg09_0A.All);
    if (!DT_SUCCESS(Status))
        return DT_STATUS_FAIL;


    // Clear EN_TOF_RST=0 to stop alignment mode
    pRegs->m_Reg09_0A.Fields.m_EnTofRst = 0;
    // Write now
    Status = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG09_16B, 
                                                                   pRegs->m_Reg09_0A.All);
    if (!DT_SUCCESS(Status))
        return DT_STATUS_FAIL;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982PllLockStateGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaLmh1982PllLockStateGet(DtaLmh1982* pLmh1982Data)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  RegValue = 0;
    Int  State = DTA_LMH1982_STATE_NOREF;
        
    Status = DtaLmh1982ReadReg(pLmh1982Data, DTA_LMH1982_IIC_REG01_8B, &RegValue);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to read PLL lock status. Error: 0x%x", Status);
        return State;
    }

    // Check status of DTA_LMH1982_IIC_REG01_REF_VALID, DTA_LMH1982_IIC_REG01_SD_LOCK
    // and DTA_LMH1982_IIC_REG01_HD_LOCK bits
    if ((RegValue & 0x1) == 0)
        State = DTA_LMH1982_STATE_NOREF;        // Invalid reference signal
    else if ((RegValue & 0x7) == 0x7)
        State = DTA_LMH1982_STATE_PLL_LOCKED;   // Have SD & HD lock
    else if ((RegValue & 0x1) != 0)
        State = DTA_LMH1982_STATE_PLL_LOCKING;  // Valid reference, but PLL is not locked
    
    return State;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982ReadReg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaLmh1982ReadReg(DtaLmh1982* pLmh1982Data, UInt8 RegAddr, UInt8* pValue)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaDeviceData*  pDvcData = pLmh1982Data->m_pDvcData;

    // Get I2C lock.
    // NOTE: Use DT_INVALID_FILE_OBJECT_PTR signals the driver wants to own the lock
    Status = DtaI2cLock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, DTA_LMH1982_I2C_TIMEOUT);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to get I2C lock. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }

    // First write I2c address
    Status = DtaI2cWrite(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, 
                                                       DTA_LMH1982_I2C_ADDR, 1, &RegAddr); 
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to write I2C address (addr=0x%02X). "
                                                          "Error: 0x%x", RegAddr, Status);

        DtaI2cUnlock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, FALSE);
        return DT_STATUS_FAIL;
    }

    // Next read register value
    Status = DtaI2cRead(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, 
                                                         DTA_LMH1982_I2C_ADDR, 1, pValue); 
    DtaI2cUnlock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, FALSE);

    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to read register value (addr=0x%02X, value=0x%02X). "
                                                 "Error: 0x%x", RegAddr, *pValue, Status);
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982ReadReg16 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1982ReadReg16(DtaLmh1982* pLmh1982Data, UInt8 RegAddr, UInt16* pValue)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaDeviceData*  pDvcData = pLmh1982Data->m_pDvcData;

    // Get I2C lock
    // NOTE: Use DT_INVALID_FILE_OBJECT_PTR signals the driver wants to own the lock
    Status = DtaI2cLock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, DTA_LMH1982_I2C_TIMEOUT);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to get I2C lock. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }

    // First write I2c address
    Status = DtaI2cWrite(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, 
                                                       DTA_LMH1982_I2C_ADDR, 1, &RegAddr); 
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to write I2C address (addr=0x%02X). "
                                                          "Error: 0x%x", RegAddr, Status);

        DtaI2cUnlock(pDvcData, -1, NULL, FALSE);
        return DT_STATUS_FAIL;
    }

    // Next read register value
    Status = DtaI2cRead(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, 
                                                 DTA_LMH1982_I2C_ADDR, 2, (UInt8*)pValue); 
    DtaI2cUnlock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, FALSE);

    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to read register value (addr=0x%02X, value=0x%04X). "
                                                 "Error: 0x%x", RegAddr, *pValue, Status);
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982WriteReg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh1982WriteReg(DtaLmh1982* pLmh1982Data, UInt8 RegAddr, UInt8 Value)
{
    UInt8  I2cBuf[2];
    DtStatus  Status = DT_STATUS_OK;
    DtaDeviceData*  pDvcData = pLmh1982Data->m_pDvcData;

    // Get I2C lock
    // NOTE: Use DT_INVALID_FILE_OBJECT_PTR signals the driver wants to own the lock
    Status = DtaI2cLock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, DTA_LMH1982_I2C_TIMEOUT);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to get I2C lock. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }

    // Write register
    I2cBuf[0] = RegAddr; I2cBuf[1] = Value;
    Status = DtaI2cWrite(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, 
                                                         DTA_LMH1982_I2C_ADDR, 2, I2cBuf); 

    // Release lock
    DtaI2cUnlock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, FALSE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to write register (addr=0x%02X, value=0x%02X). "
                                                   "Error: 0x%x", RegAddr, Value, Status);
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982WriteReg16 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaLmh1982WriteReg16(DtaLmh1982* pLmh1982Data, UInt8 RegAddr, UInt16 Value)
{
    UInt8  I2cBuf[3];
    DtStatus  Status = DT_STATUS_OK;
    DtaDeviceData*  pDvcData = pLmh1982Data->m_pDvcData;

    // Get I2C lock
    // NOTE: Use DT_INVALID_FILE_OBJECT_PTR signals the driver wants to own the lock
    Status = DtaI2cLock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, DTA_LMH1982_I2C_TIMEOUT);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to get I2C lock. Error: 0x%x", Status);
        return DT_STATUS_FAIL;
    }

    // Write register
    I2cBuf[0] = RegAddr; I2cBuf[1] = (Value&0xFF); I2cBuf[2] = ((Value>>8)&0xFF);
    Status = DtaI2cWrite(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, 
                                                         DTA_LMH1982_I2C_ADDR, 3, I2cBuf); 
    
    // Release lock
    DtaI2cUnlock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, FALSE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to write register (addr=0x%02X, value=0x%04X). "
                                                   "Error: 0x%x", RegAddr, Value, Status);
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982CacheRefresh -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
//DtStatus  DtaLmh1982CacheRefresh(DtaLmh1982* pLmh1982Data)
//{
//    DtStatus  s = DT_STATUS_OK;
//    DtaLmh1982Regs*  pRegs = &pLmh1982Data->m_RegCache;
//
//    s = DtaLmh1982ReadReg(pLmh1982Data, DTA_LMH1982_IIC_REG00_8B, &pRegs->m_Reg00.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg(pLmh1982Data, DTA_LMH1982_IIC_REG01_8B, &pRegs->m_Reg01.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg(pLmh1982Data, DTA_LMH1982_IIC_REG02_8B, &pRegs->m_Reg02.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg(pLmh1982Data, DTA_LMH1982_IIC_REG03_8B, &pRegs->m_Reg03.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg16(pLmh1982Data, DTA_LMH1982_IIC_REG04_16B, 
//                                                                  &pRegs->m_Reg04_05.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg(pLmh1982Data, DTA_LMH1982_IIC_REG06_16B, &pRegs->m_Reg06.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg(pLmh1982Data, DTA_LMH1982_IIC_REG07_8B, &pRegs->m_Reg07);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg(pLmh1982Data, DTA_LMH1982_IIC_REG08_8B, &pRegs->m_Reg08.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg16(pLmh1982Data, DTA_LMH1982_IIC_REG09_16B, 
//                                                                  &pRegs->m_Reg09_0A.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg16(pLmh1982Data, DTA_LMH1982_IIC_REG0B_16B,
//                                                                  &pRegs->m_Reg0B_0C.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg16(pLmh1982Data, DTA_LMH1982_IIC_REG0D_16B,
//                                                                  &pRegs->m_Reg0D_0E.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg16(pLmh1982Data, DTA_LMH1982_IIC_REG0F_16B, 
//                                                                  &pRegs->m_Reg0F_10.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg16(pLmh1982Data, DTA_LMH1982_IIC_REG11_16B,
//                                                                  &pRegs->m_Reg11_12.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg(pLmh1982Data, DTA_LMH1982_IIC_REG13_8B, &pRegs->m_Reg13.All);
//    if (!DT_SUCCESS(s)) return s;
//    s = DtaLmh1982ReadReg(pLmh1982Data, DTA_LMH1982_IIC_REG14_8B, &pRegs->m_Reg14.All);
//    if (!DT_SUCCESS(s)) return s;
//    
//    return s;
//}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982CacheWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaLmh1982CacheWrite(DtaLmh1982* pLmh1982Data)
{
    DtStatus  s = DT_STATUS_OK;
    DtaLmh1982Regs*  pRegs = &pLmh1982Data->m_RegCache;

    s = DtaLmh1982WriteReg(pLmh1982Data, DTA_LMH1982_IIC_REG00_8B, pRegs->m_Reg00.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg(pLmh1982Data, DTA_LMH1982_IIC_REG01_8B, pRegs->m_Reg01.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg(pLmh1982Data, DTA_LMH1982_IIC_REG02_8B, pRegs->m_Reg02.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg(pLmh1982Data, DTA_LMH1982_IIC_REG03_8B, pRegs->m_Reg03.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG04_16B, 
                                                                   pRegs->m_Reg04_05.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg(pLmh1982Data, DTA_LMH1982_IIC_REG06_16B, pRegs->m_Reg06.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg(pLmh1982Data, DTA_LMH1982_IIC_REG07_8B, pRegs->m_Reg07);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg(pLmh1982Data, DTA_LMH1982_IIC_REG08_8B, pRegs->m_Reg08.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG09_16B,
                                                                   pRegs->m_Reg09_0A.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG0B_16B, 
                                                                   pRegs->m_Reg0B_0C.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG0D_16B, 
                                                                   pRegs->m_Reg0D_0E.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG0F_16B,
                                                                   pRegs->m_Reg0F_10.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg16(pLmh1982Data, DTA_LMH1982_IIC_REG11_16B,
                                                                   pRegs->m_Reg11_12.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg(pLmh1982Data, DTA_LMH1982_IIC_REG13_8B, pRegs->m_Reg13.All);
    if (!DT_SUCCESS(s)) return s;
    s = DtaLmh1982WriteReg(pLmh1982Data, DTA_LMH1982_IIC_REG14_8B, pRegs->m_Reg14.All);
    if (!DT_SUCCESS(s)) return s;

    return s;
}

#ifdef _DEBUG
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982CachePrint -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaLmh1982CachePrint(DtaLmh1982* pLmh1982Data)
{
    DtaLmh1982Regs*  pRegs = &pLmh1982Data->m_RegCache;

    DtDbgOut(MIN, GENL, "------------------ Lmh1982PrintCache ------------------");
    DtDbgOut(MIN, GENL, "ADDR:  DATA");
    DtDbgOut(MIN, GENL, " %02X: 0x%02X", DTA_LMH1982_IIC_REG00_8B, pRegs->m_Reg00.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%02X", DTA_LMH1982_IIC_REG01_8B, pRegs->m_Reg01.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%02X", DTA_LMH1982_IIC_REG02_8B, pRegs->m_Reg02.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%02X", DTA_LMH1982_IIC_REG03_8B, pRegs->m_Reg03.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%04X", DTA_LMH1982_IIC_REG04_16B, pRegs->m_Reg04_05.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%04X", DTA_LMH1982_IIC_REG06_16B, pRegs->m_Reg06.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%02X", DTA_LMH1982_IIC_REG07_8B, pRegs->m_Reg07);
    DtDbgOut(MIN, GENL, " %02X: 0x%02X", DTA_LMH1982_IIC_REG08_8B, pRegs->m_Reg08.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%04X", DTA_LMH1982_IIC_REG09_16B, pRegs->m_Reg09_0A.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%04X", DTA_LMH1982_IIC_REG0B_16B, pRegs->m_Reg0B_0C.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%04X", DTA_LMH1982_IIC_REG0D_16B, pRegs->m_Reg0D_0E.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%04X", DTA_LMH1982_IIC_REG0F_16B, pRegs->m_Reg0F_10.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%04X", DTA_LMH1982_IIC_REG11_16B, pRegs->m_Reg11_12.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%02X", DTA_LMH1982_IIC_REG13_8B, pRegs->m_Reg13.All);
    DtDbgOut(MIN, GENL, " %02X: 0x%02X", DTA_LMH1982_IIC_REG14_8B, pRegs->m_Reg14.All);

    
    return DT_STATUS_OK;
}
#endif // #ifdef _DEBUG

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982VidStd2Lpfm -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaLmh1982VidStd2Lpfm(Int VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_525I59_94:   
        return 525;

    case DT_VIDSTD_625I50:      
        return 625;

    case DT_VIDSTD_720P23_98:
    case DT_VIDSTD_720P24:
    case DT_VIDSTD_720P25:
    case DT_VIDSTD_720P29_97:
    case DT_VIDSTD_720P30:
    case DT_VIDSTD_720P50:
    case DT_VIDSTD_720P59_94:
    case DT_VIDSTD_720P60:
        return 750;

    case DT_VIDSTD_1080P23_98:
    case DT_VIDSTD_1080P24:
    case DT_VIDSTD_1080P25:
    case DT_VIDSTD_1080P30:
    case DT_VIDSTD_1080P29_97:
    case DT_VIDSTD_1080P50:
    case DT_VIDSTD_1080P59_94:
    case DT_VIDSTD_1080P60:
    case DT_VIDSTD_1080I50:
    case DT_VIDSTD_1080I59_94:
    case DT_VIDSTD_1080I60: 
        return 1125;

    default:
        break;
    }
    return -1;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh1982State2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
const char*  DtaLmh1982State2Str(Int State)
{
    switch (State)
    {
    case DTA_LMH1982_STATE_INIT:            return "INIT";
    case DTA_LMH1982_STATE_NOREF:           return "NOREF";
    case DTA_LMH1982_STATE_PLL_LOCKING:     return "PLL_LOCKING";
    case DTA_LMH1982_STATE_PLL_LOCKED:      return "PLL_LOCKED";
    case DTA_LMH1982_STATE_ALIGN_TOF:       return "ALIGN_TOF";
    case DTA_LMH1982_STATE_FULL_LOCK:       return "FULL_LOCK";
    case DTA_LMH1982_STATE_HOLD_OVER1:      return "HOLD_OVER1";
    case DTA_LMH1982_STATE_HOLD_OVER2:      return "HOLD_OVER2";
    default:                                return "UNKNOWN";
    }
}
