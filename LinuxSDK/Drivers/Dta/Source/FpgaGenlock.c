//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* FpgaGenlock.c *#*#*#*#*#*#*#*# (C) 2012-2016 DekTec
//
// Dta driver - FPGA genlock controller
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012-2016 DekTec Digital Video B.V.
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

#define DTA_I2C_VCXO_SPEED_CTRL_TIMEOUT     100

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus DtaFpgaGenLockSetVcxoSpeed(DtaFpgaGenlock* pFpgaGenlockData);
void  DtaFpgaGenlockLockedEventDpc(DtDpcArgs* pArgs);
void  DtaFpgaGenlockLockedInitLogic(DtaFpgaGenlock* pFpgaGenlockData);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFpgaGenlockEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaFpgaGenlockEnable(DtaDeviceData* pDvcData, DtaFpgaGenlock* pFpgaGenlockData)
{
    volatile UInt8*  pGenRegs = (volatile UInt8*)pFpgaGenlockData->m_pDvcData->m_pGenRegs;
    
    UInt  RegValueVideoStd = 0;

    if (pDvcData->m_Genlock.m_RefVidStd!=DT_VIDSTD_525I59_94 && 
                                        pDvcData->m_Genlock.m_RefVidStd!=DT_VIDSTD_625I50)
      return DT_STATUS_NOT_SUPPORTED;
        
    // Initialize logic
    DtaFpgaGenlockLockedInitLogic(pFpgaGenlockData);  
    
    // Note: Revise this when multiboard genlock is implemented
    DtDbgOut(AVG, GENL, "Enabling on board VCO");

    // Enable Vcxo for device wide SDI TX Timing generator
    DtaRegVcxoCtrlSetEnable(pGenRegs, 1);

    // Programm SDI TX Timing generator with the configured SDI standard
    if (pDvcData->m_Genlock.m_RefVidStd == DT_VIDSTD_625I50)    
       DtDbgOut(AVG, GENL, "SDI 625 Genlock Enabled");
    else
       DtDbgOut(AVG, GENL, "SDI 525 Genlock Enabled");

    RegValueVideoStd = pDvcData->m_Genlock.m_RefVidStd == DT_VIDSTD_625I50 ? 0 : 1;
    DtaRegVcxoCtrlSetVidStd(pGenRegs, RegValueVideoStd);
    
    // Start VCXO at 50%
    pFpgaGenlockData->m_VcxoCtrl = 512000;
    DtaFpgaGenLockSetVcxoSpeed(pFpgaGenlockData);
    // Initialise SDI frame position
    DtaRegTxSdiPosSet(pGenRegs, 0x0);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFpgaGenlockDisable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaFpgaGenlockDisable(DtaDeviceData* pDvcData, DtaFpgaGenlock* pFpgaGenlockData)
{
    volatile UInt8*  pGenRegs = (volatile UInt8*)pFpgaGenlockData->m_pDvcData->m_pGenRegs;

    DtDbgOut(AVG, GENL, "Disabling Genlock Rx and Genlock Tx");
   
    // Genlock TX specific
    // Disable Vcxo
    // Note: Tx channels must be inactive when disabling VCO    
    DtaRegVcxoCtrlSetEnable(pGenRegs, 0);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFpgaGenlockLockedInitLogic -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaFpgaGenlockLockedInitLogic(DtaFpgaGenlock* pFpgaGenlockData)
{    
    // Initialise Genlock RX logic
    pFpgaGenlockData->m_Locked = FALSE;			// indicate out of lock
    pFpgaGenlockData->m_LockedPrev = FALSE;
    pFpgaGenlockData->m_GraceCnt = 0;        
    pFpgaGenlockData->m_I2cBusy = FALSE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFpgaGenlockInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaFpgaGenlockInit(DtaDeviceData* pDvcData, DtaFpgaGenlock* pFpgaGenlockData)
{    
    DtStatus  Status = DT_STATUS_OK;
    DtDpcArgs  DpcArgs;    
    
    // Initialize members
    pFpgaGenlockData->m_pDvcData = pDvcData;

    if (pDvcData->m_Genlock.m_GenlArch == GENLOCK_ARCH_2144)    
       DtDbgOut(AVG, GENL, "Using I2c VCXO speed setting");    
    
    // Initialise Genlock RX logic
    DtaFpgaGenlockLockedInitLogic(pFpgaGenlockData);

    // Event handling
    Status = DtDpcInit(&pFpgaGenlockData->m_EventLockedDpc, 
                                                      DtaFpgaGenlockLockedEventDpc, TRUE);
    if (Status != DT_STATUS_OK)
      return Status;
    
    // Schedule DPC for initial NO_LOCK event
    DtDbgOut(AVG, GENL, "Schedule Locked event (NO_LOCK)");  
    DpcArgs.m_pContext = pFpgaGenlockData;
    DpcArgs.m_Data1.m_UInt64 = (UInt64)DTA_EVENT_VALUE1_NO_LOCK;
    DtDpcSchedule(&pFpgaGenlockData->m_EventLockedDpc, &DpcArgs);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenLockSetVcxoSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// range 0 .. 1M
//
DtStatus DtaFpgaGenLockSetVcxoSpeed(DtaFpgaGenlock* pFpgaGenlockData)
{
  volatile UInt8*  pGenRegs;
  volatile UInt8*  pI2cRegs;
  UInt  Speed;
  UInt8  I2CData[2];
  Int  GenlArch = pFpgaGenlockData->m_pDvcData->m_Genlock.m_GenlArch;

  pGenRegs = (volatile UInt8*)pFpgaGenlockData->m_pDvcData->m_pGenRegs;
  pI2cRegs = (volatile UInt8*)pFpgaGenlockData->m_pDvcData->m_I2c.m_pI2cRegs;
  Speed = 0;

  DT_ASSERT(GenlArch == GENLOCK_ARCH_2144 || GenlArch == GENLOCK_ARCH_145);

  if (GenlArch != GENLOCK_ARCH_2144)
  {
     // convert range to 0 .. 1000, 512 is center
    Speed = pFpgaGenlockData->m_VcxoCtrl / 1000;
    DtaRegVcxoCtrlSetSpeed(pGenRegs, Speed);
    return DT_STATUS_OK;
  }
  
  // convert range to 0 .. 4000, 2048 is center
  Speed = pFpgaGenlockData->m_VcxoCtrl / 250;

  // Normal I2cWrite is blocking, which may take a lot of time, eventually blocking the 
  // periodic interrupt...so do not use!
  //  Solution:
  //        Do our own I2c writing with no blocking and no locking 
  //        (We are in an interrupt!)

  // If busy with the previous call and still busy do nothing, else free I2c access
  if(pFpgaGenlockData->m_I2cBusy)
  {
    DtDbgOut(MAX, GENL, "I2C Busy");
    if (pFpgaGenlockData->m_pDvcData->m_I2c.m_Done)
    {
       DtDbgOut(MAX, GENL, "Finished speed setting of VCXO through I2c");
       pFpgaGenlockData->m_pDvcData->m_I2c.m_Done = FALSE;
       pFpgaGenlockData->m_I2cBusy = FALSE; 
    }
    return DT_STATUS_OK;
  }

  pFpgaGenlockData->m_I2cBusy = TRUE;

  // swap byte order
  I2CData[0] = (UInt8) ((Speed & 0xff00) >> 8);	// MSB VCXO setting	
  I2CData[1] = (UInt8) (Speed & 0xff);				  // LSB VCXO setting
  Speed = (I2CData[1] << 8) + I2CData[0];  

  DtDbgOut(MAX, GENL, "Write I2C Data 0x%04X", Speed);
  DtaRegI2cDataSet(pI2cRegs, Speed);
  DtaRegI2cAddrCntSetWrAddrAndCount(pI2cRegs, (DTA_I2C_VCXO_SPEED_CTRL>>1), 2); 

  return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFpgaGenlockUpdateTiming -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtaFpgaGenlockUpdateTiming(DtaFpgaGenlock* pFpgaGenlockData)
{
  UInt64  RefCnt;
  volatile UInt8*  pGenRegs;
  pFpgaGenlockData->m_ValidRefCnt = FALSE;

  pGenRegs = (volatile UInt8*)pFpgaGenlockData->m_pDvcData->m_pGenRegs;

  // obtain periodic interrupt latched reference count
  RefCnt = DtaRegRefClkCntGet64(pGenRegs);

  // DTA-145/2144/2145: 
  // Divide by 2 to convert from 54Mhz to to 27Mhz
  RefCnt = RefCnt/2;					

  // Videostandard Helper  
  if (pFpgaGenlockData->m_pDvcData->m_Genlock.m_RefVidStd == DT_VIDSTD_625I50) 
  {
    pFpgaGenlockData->m_LinesPerFrame = 625;
    pFpgaGenlockData->m_SamplesPerLine = 1728;
  } else {
    pFpgaGenlockData->m_LinesPerFrame = 525;
    pFpgaGenlockData->m_SamplesPerLine = 1716;
  }
    
  // Determine time passed since last Update
  if (RefCnt < pFpgaGenlockData->m_RefCntprev) 
  {		
    DtDbgOut(AVG, GENL, "Notice: RefCnt WRAP, Warning: Skipping one control iteration");
    // When the reference counters wraps, skip further handling		
  } else {
    pFpgaGenlockData->m_RefCntDelta = (RefCnt - pFpgaGenlockData->m_RefCntprev);		

    // 27Mhz Refcounter converted to number of SDI LinesPerFrame lines
    pFpgaGenlockData->m_RefLinesDelta = DtDivide64(pFpgaGenlockData->m_RefCntDelta,
                                                pFpgaGenlockData->m_SamplesPerLine, NULL);
    DtDbgOut(MAX, GENL, "RefLines=%d",(Int)pFpgaGenlockData->m_RefLinesDelta);				

    pFpgaGenlockData->m_ValidRefCnt = TRUE;
  }
  pFpgaGenlockData->m_RefCntprev = RefCnt;
  return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFpgaGenlockUpdateRx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtaFpgaGenlockUpdateRx(DtaFpgaGenlock* pFpgaGenlockData, Int PortIndex)
{	
  Bool  ValidRxSignals = TRUE;
  UInt32  RxFrame, RxLine, RxSamp;
  UInt64  RxPos;
  UInt64  RxCount;
  UInt  RxLineCount;
  DtDpcArgs  DpcArgs;
  volatile UInt8*  pRxRegs;

  // TODO: This is currently always one port with portindex 0 ...
  //       If we support other genref ports, we should put m_RxLinePrev, m_RxSampPrev, 
  //       m_RxFramePrev, m_RxPosPrev in an array, 
  //       or add them to the NonIpChannel structure.
  DT_ASSERT(PortIndex == 0);
  pRxRegs = 
        (volatile UInt8*)pFpgaGenlockData->m_pDvcData->m_pNonIpPorts[PortIndex].m_pRxRegs;
   
  pFpgaGenlockData->m_ValidRxSignals = FALSE;  
  
  // Entry checks  
  if (pFpgaGenlockData->m_ValidRefCnt != TRUE)
    return DT_STATUS_OK;

  // obtain periodic interrupt latched RxCounts
  RxFrame = DtaRegRxSdiPosGetLineSample(pRxRegs, &RxLine, &RxSamp);

  DtDbgOut(MAX, GENL, "Rx=%d.%d.%d", RxFrame, RxLine, RxSamp);
  
  // Sanity checks
  // prevent wrap around problems
  if (RxFrame < pFpgaGenlockData->m_RxFramePrev) 
  {		
    DtDbgOut(AVG, GENL, "Notice: RxFrame WRAP");    
    pFpgaGenlockData->m_RxFramePrev -= 2048;			// Max 2^11 (~ 40 seconden)
    pFpgaGenlockData->m_RxPosPrev -= (2048 * pFpgaGenlockData->m_LinesPerFrame * 
                                                      pFpgaGenlockData->m_SamplesPerLine);
    // Now we can continue handling as normal
  }

  // Absolute sample count measured in 27Mhz variable
  RxPos = (((RxFrame * pFpgaGenlockData->m_LinesPerFrame) + RxLine) * 
                                             pFpgaGenlockData->m_SamplesPerLine) + RxSamp;

  RxCount = RxPos - pFpgaGenlockData->m_RxPosPrev;
  pFpgaGenlockData->m_RxPosPrev = RxPos;
  RxLineCount = (UInt) DtDivide64(RxCount, pFpgaGenlockData->m_SamplesPerLine, NULL);

  DtDbgOut(MAX, GENL, "RxCount = %d / %d lines",  (Int) RxCount, (Int) RxLineCount);	
  if (RxLineCount == 0) 
  {
    DtDbgOut(MAX, GENL, "Warning: No RX input");
    ValidRxSignals = FALSE;		
  } else if (RxLineCount<pFpgaGenlockData->m_RefLinesDelta-2 || 
                                          RxLineCount>pFpgaGenlockData->m_RefLinesDelta+2)
  {
      // The firmware contains in the RX channel (sdicompress) 
      // logic for determining the framesync
      // This introduces one or multiple clocks (>>4) delay in the framecounter 
      // Resulting in incorrect Rx frame compared to the line and sample counters

      // Workaround below detects and ignores the current samples
      if (RxLine == 0) 
      {
        DtDbgOut(MAX, GENL, "Rxline == 0, Warning: Skipping one control iteration");
        ValidRxSignals = FALSE;
      } else if (pFpgaGenlockData->m_RxLinePrev == 0) 
      {
        DtDbgOut(MAX, GENL, "RxlinePrev == 0, Warning: Skipping one control iteration");
        ValidRxSignals = FALSE;						
      } else {
        DtDbgOut(MAX, GENL, "Warning: Bad Rx Count");
        ValidRxSignals = FALSE;
      }
    }

    // We cannot set an event in an interrupt, a DPC is used to trigger the event.
    DpcArgs.m_pContext = pFpgaGenlockData;    
    if (pFpgaGenlockData->m_Locked && !pFpgaGenlockData->m_LockedPrev)
    {
      DtDbgOut(AVG, GENL, "Genlock Acquired");
      DtDbgOut(AVG, GENL, "Schedule Locked event (LOCKED)");  
      DpcArgs.m_Data1.m_UInt64 = (UInt64)DTA_EVENT_VALUE1_LOCKED;
      DtDpcSchedule(&pFpgaGenlockData->m_EventLockedDpc, &DpcArgs);    
    }else if (!pFpgaGenlockData->m_Locked && pFpgaGenlockData->m_LockedPrev)
    {
      DtDbgOut(AVG, GENL, "Genlock Lost");
      DtDbgOut(AVG, GENL, "Schedule Locked event (NO_LOCK)");  
      DpcArgs.m_Data1.m_UInt64 = (UInt64)DTA_EVENT_VALUE1_NO_LOCK;
      DtDpcSchedule(&pFpgaGenlockData->m_EventLockedDpc, &DpcArgs);     
    }

    pFpgaGenlockData->m_LockedPrev = pFpgaGenlockData->m_Locked;

    // "Publish" the Rx position device wide
    pFpgaGenlockData->m_RxLine = RxLine;
    pFpgaGenlockData->m_RxSamp = RxSamp;
    pFpgaGenlockData->m_ValidRxSignals = ValidRxSignals;

    // Store Rx frame + line/sample position locally within channel
    pFpgaGenlockData->m_RxFramePrev = RxFrame;
    pFpgaGenlockData->m_RxLinePrev = RxLine;
    pFpgaGenlockData->m_RxSampPrev = RxSamp;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFpgaGenlockTxTo27Mhz -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtaFpgaGenlockTxTo27Mhz(
  DtaFpgaGenlock*  pFpgaGenlockData, 
  Bool  ValidTxSignals,
  UInt64  TxCount,
  Int*  pSpdDelta)
{ 
  UInt  TxMinRef;
  Int  dTxMinRefdt;

  // ERROR: No valid Tx counters
  if (!ValidTxSignals)
  {	        
    if(pFpgaGenlockData->m_GraceCnt != 10) 
    {
      DtDbgOut(MAX, GENL, "Note: GraceCount %d",  pFpgaGenlockData->m_GraceCnt);
      pFpgaGenlockData->m_GraceCnt++;
      pFpgaGenlockData->m_TxMinRxPrev = 0;		// prevent errors
      pFpgaGenlockData->m_TxMinRefPrev = 0;	  // prevent errors
      return;
    } 

    // We can't do anything if we don't have valid TX signals
    // Center Vcxo and hope for the best
    if (pFpgaGenlockData->m_VcxoCtrl != 512000)
    {
      pFpgaGenlockData->m_VcxoCtrl = 512000;
      DtaFpgaGenLockSetVcxoSpeed(pFpgaGenlockData);        
      DtDbgOut(MAX, GENL, "Force Reset VCXO to %d", pFpgaGenlockData->m_VcxoCtrl);
    }
    // Note: revise code below for crossboard genlock
    // indicate out of lock
    pFpgaGenlockData->m_Locked = FALSE;    
    return;
  }  
  
  // No valid Rx, GENLOCK to 27Mhz  
  if (!pFpgaGenlockData->m_ValidRxSignals) 
  {
    if (pFpgaGenlockData->m_GraceCnt != 10) 
    {
      // Delay locking to 27Mhz until rx lock permanently lost
      DtDbgOut(AVG, GENL, "Note: GraceCount %d", pFpgaGenlockData->m_GraceCnt);
      pFpgaGenlockData->m_GraceCnt++;
      pFpgaGenlockData->m_TxMinRxPrev = 0;		// prevent errors
      pFpgaGenlockData->m_TxMinRefPrev = 0;		// prevent errors
      return;
    }

    // RefCntDelta in 27Mhz units, equals number of samples possible in 
    // previous period
    TxMinRef = (Int) (TxCount - pFpgaGenlockData->m_RefCntDelta);
    DtDbgOut(MAX, GENL, "#27Mhz Reference Cycles: %d, TxCount: %d", 
                                      (Int)pFpgaGenlockData->m_RefCntDelta, (Int)TxCount);

    // Compute derivative of difference between Tx SDI pos and Rx SDI pos
    dTxMinRefdt = pFpgaGenlockData->m_TxMinRefPrev - (Int) TxMinRef;
    pFpgaGenlockData->m_TxMinRefPrev = (Int) TxMinRef;
    DtDbgOut(MAX, GENL, "27Mhz Xtal/VCO cycle difference %d (%d)", TxMinRef, dTxMinRefdt);
    
    // Compute new control value
    *pSpdDelta = - (Int)TxMinRef*10 + dTxMinRefdt*200;
    pFpgaGenlockData->m_Locked = FALSE;
  }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFpgaGenlockTxToRx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtaFpgaGenlockTxToRx(
  DtaFpgaGenlock*  pFpgaGenlockData,
  UInt32  TxLine, 
  UInt32  TxSamp,
  Int*  pSpdDelta)
{
    Int  TxMinRx;
    UInt64  RxPos;
    UInt64  Rest;
    Int  dTxMinRxdt;

    // Computer TxMinRx, maximum difference 1 frame
    // TxMinRx = (Int) ((TxPos - RxPos) % (LinesPerFrame * SamplesPerLine));
    // Compute Rx/Txpos for locking (we wan't to lock to same linenumber.samplenumber)
    // Absolute sample count measured in 27Mhz
    Int64  TxPos = ( TxLine * pFpgaGenlockData->m_SamplesPerLine) + TxSamp;

    // Use "published" RX line/sample for to lock to
    // Absolute sample count measured in 27Mhz variable
    RxPos = ( pFpgaGenlockData->m_RxLine * pFpgaGenlockData->m_SamplesPerLine) + 
                                                               pFpgaGenlockData->m_RxSamp;

    // 20 samples tuning to align them properly
    // Source: GenlockMeasure using DTA-160
    RxPos += 20;	// correct for 20 samples

    // Wrap around
    DtDivide64(RxPos, (Int64)(pFpgaGenlockData->m_LinesPerFrame * 
                                              pFpgaGenlockData->m_SamplesPerLine), &Rest);
    RxPos = (UInt32)Rest;
    TxMinRx = (Int)(TxPos - RxPos);

    if (TxMinRx < pFpgaGenlockData->m_TxMinRxPrev -
                 (pFpgaGenlockData->m_LinesPerFrame*pFpgaGenlockData->m_SamplesPerLine)/2)
      TxMinRx += (pFpgaGenlockData->m_LinesPerFrame * pFpgaGenlockData->m_SamplesPerLine);
    else if (TxMinRx > pFpgaGenlockData->m_TxMinRxPrev + 
                 (pFpgaGenlockData->m_LinesPerFrame*pFpgaGenlockData->m_SamplesPerLine)/2)
      TxMinRx -= (pFpgaGenlockData->m_LinesPerFrame * pFpgaGenlockData->m_SamplesPerLine);

    DtDbgOut(MAX, GENL, ">> Dta: Diff: %d.%d lines <<", 
                                              TxMinRx/pFpgaGenlockData->m_SamplesPerLine,
                                              TxMinRx%pFpgaGenlockData->m_SamplesPerLine);

    if (abs(TxMinRx/pFpgaGenlockData->m_SamplesPerLine) > 5)
    {	
      UInt32  NewTxLine;
      UInt32  NewTxSamp;
      volatile UInt8*  pGenRegs = 
                                (volatile UInt8*)pFpgaGenlockData->m_pDvcData->m_pGenRegs;

      // Out of GENLOCK
      if (pFpgaGenlockData->m_GraceCnt != 10)
      {
        DtDbgOut(MAX, GENL, "Grace %d", pFpgaGenlockData->m_GraceCnt);
        pFpgaGenlockData->m_GraceCnt++;
        return;
      }
      

      // Use "published" RX line/sample for preloading
      NewTxLine = (pFpgaGenlockData->m_LinesPerFrame-TxLine+pFpgaGenlockData->m_RxLine) 
                                                    % pFpgaGenlockData->m_LinesPerFrame;
      NewTxSamp = (pFpgaGenlockData->m_SamplesPerLine-TxSamp+pFpgaGenlockData->m_RxSamp) 
                                                    % pFpgaGenlockData->m_SamplesPerLine;

      DtDbgOut(AVG, GENL, ">> Resyncing TX Counter <<");
      DtDbgOut(AVG, GENL, "\nTX Line counter step: "
                                              "%d -> %d TX Sample counter step: %d -> %d",  
                                              TxLine, NewTxLine, TxSamp, NewTxSamp);

      DtaRegTxSdiPosSetLineSample(pGenRegs, NewTxLine, NewTxSamp);

      pFpgaGenlockData->m_VcxoCtrl = 512000;
      DtaFpgaGenLockSetVcxoSpeed(pFpgaGenlockData);

      pFpgaGenlockData->m_TxMinRxPrev = 0;							
      pFpgaGenlockData->m_TxMinRefPrev = 0;		    // prevent errors

      pFpgaGenlockData->m_Locked = FALSE;			    // indicate out of lock
      pFpgaGenlockData->m_GraceCnt = 0;		        // allow channel to sync      
      return;
    }

    // In GENLOCK      
    DtDbgOut(MAX, GENL, "Genlock delta: %d.%d lines", 
                                            TxMinRx / pFpgaGenlockData->m_SamplesPerLine, 
                                            TxMinRx % pFpgaGenlockData->m_SamplesPerLine);
    if (TxMinRx > pFpgaGenlockData->m_SamplesPerLine)
      TxMinRx = pFpgaGenlockData->m_SamplesPerLine;
    else if (TxMinRx < - pFpgaGenlockData->m_SamplesPerLine)
      TxMinRx = - pFpgaGenlockData->m_SamplesPerLine;
      
    // Compute derivative of difference between Tx SDI pos and Rx SDI pos
    dTxMinRxdt = pFpgaGenlockData->m_TxMinRxPrev - TxMinRx;
    pFpgaGenlockData->m_TxMinRxPrev = TxMinRx;						

    // Compute new control value
    *pSpdDelta = - TxMinRx * 10 + dTxMinRxdt * 200;

    pFpgaGenlockData->m_Locked = TRUE;				// indicate locked
    pFpgaGenlockData->m_GraceCnt = 0;			    // reset grace counter 
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFpgaGenlockUpdateTx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtaFpgaGenlockUpdateTx(DtaFpgaGenlock* pFpgaGenlockData, Int PortIndex)
{
  volatile UInt8*  pGenRegs;
  UInt  TxLineCount;
  UInt32  TxFrame;
  UInt32  TxLine;
  UInt32  TxSamp;
  UInt64  TxPos;
  UInt64  TxCount;
  Int  SpdDelta;
  Bool  ValidTxSignals = TRUE;
  
  // Entry checks
  if (!pFpgaGenlockData->m_ValidRefCnt)
    return DT_STATUS_OK;
  
  SpdDelta = 0;
  pGenRegs = (volatile UInt8*)pFpgaGenlockData->m_pDvcData->m_pGenRegs;
  TxFrame = DtaRegTxSdiPosGetLineSample(pGenRegs, &TxLine, &TxSamp);

  DtDbgOut(MAX, GENL, "Tx=%d.%d.%d", TxFrame, TxLine, TxSamp);

  // Position calculation
  if (TxFrame < pFpgaGenlockData->m_TxFramePrev)
  {		
    DtDbgOut(MAX, GENL, "Notice: TxFrame WRAP");
    pFpgaGenlockData->m_TxFramePrev -= 2048;			// Max 2^11
    pFpgaGenlockData->m_TxPosPrev -= 2048 * pFpgaGenlockData->m_LinesPerFrame * 
                                                       pFpgaGenlockData->m_SamplesPerLine;
    // Now we can continue handling as normal
  }

  // Compute absolute Rx/Txpos  
  // Absolute sample count measured in 27Mhz
  TxPos =  ((TxFrame * pFpgaGenlockData->m_LinesPerFrame + TxLine) *
                                             pFpgaGenlockData->m_SamplesPerLine) + TxSamp;
  TxCount = TxPos - pFpgaGenlockData->m_TxPosPrev;
  pFpgaGenlockData->m_TxPosPrev = TxPos;
  TxLineCount =  (UInt)DtDivide64(TxCount, pFpgaGenlockData->m_SamplesPerLine, NULL);
  DtDbgOut(MAX, GENL, "TxCount = %d / %d lines", (Int)TxCount, (Int)TxLineCount);

  // Sanity checks
  if (TxLineCount<pFpgaGenlockData->m_RefLinesDelta-2 ||
                                          TxLineCount>pFpgaGenlockData->m_RefLinesDelta+2)
  {
    DtDbgOut(MAX, GENL, "TX Genlock Warning: Bad Tx Count");
    ValidTxSignals = FALSE;
  } 

  // Try to genlock Tx to 27Mhz if one or both of the signals are missing
  if(!ValidTxSignals || !pFpgaGenlockData->m_ValidRxSignals)
    DtaFpgaGenlockTxTo27Mhz(pFpgaGenlockData, ValidTxSignals, TxCount, &SpdDelta); 
  else 
    DtaFpgaGenlockTxToRx(pFpgaGenlockData, TxLine, TxSamp, &SpdDelta);
  
  // Adjust VCXO  
  if (SpdDelta != 0)
  {				
    DtDbgOut(MAX, GENL, "SpdDelta=%d", SpdDelta);

    // Update VCXO control
    pFpgaGenlockData->m_VcxoCtrl += SpdDelta;
    if (pFpgaGenlockData->m_VcxoCtrl < 0)
      pFpgaGenlockData->m_VcxoCtrl = 0;
    if (pFpgaGenlockData->m_VcxoCtrl > 1000000)
      pFpgaGenlockData->m_VcxoCtrl = 1000000;

    DtaFpgaGenLockSetVcxoSpeed(pFpgaGenlockData);	
  }

  pFpgaGenlockData->m_TxFramePrev	= TxFrame;
  pFpgaGenlockData->m_TxLinePrev	= TxLine;
  pFpgaGenlockData->m_TxSampPrev	= TxSamp;

  return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFpgaGenlockGetRefState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Return lock status
//
DtStatus  DtaFpgaGenlockGetRefState(
  DtaDeviceData*  pDvcData, 
  Int  PortIndex, 
  Bool  *pEnabled, 
  Bool  *pInLock)
{ 
  // Only supported on genlock input ports
  if (!pDvcData->m_pNonIpPorts[PortIndex].m_CapGenRef)
  {
    DtDbgOut(ERR, GENL, "[%d] Genref not supported", PortIndex);
    return DT_STATUS_NOT_SUPPORTED;
  }
  
  *pEnabled = FALSE;

  if (pDvcData->m_pNonIpPorts[PortIndex].m_IoCfg[DT_IOCONFIG_GENREF].m_Value 
                                                                      != DT_IOCONFIG_TRUE)
  {
    DtDbgOut(AVG, GENL, "[%d] Genref not enabled", PortIndex);
    return DT_STATUS_OK;
  }

  *pEnabled = TRUE;
  *pInLock = pDvcData->m_Genlock.m_FpgaGenlock.m_Locked;
  if (*pInLock)  
    DtDbgOut(AVG, GENL, "[%d] In lock", PortIndex);
   else
    DtDbgOut(AVG, GENL, "[%d] Out of lock", PortIndex);

  return DT_STATUS_OK;  
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFpgaGenlockLockedEventDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaFpgaGenlockLockedEventDpc(DtDpcArgs* pArgs)
{
  DtaFpgaGenlock*  pDtaFpgaGenlock = (DtaFpgaGenlock*)pArgs->m_pContext;
  Int  LockedState = (Int)pArgs->m_Data1.m_UInt64;
    
  DtDbgOut(AVG, GENL, "Locked event: %d", LockedState);  

  // Fire locked event
  DtaEventsSet(pDtaFpgaGenlock->m_pDvcData, NULL, DTA_EVENT_TYPE_GENLOCK, LockedState, 0);  
}
