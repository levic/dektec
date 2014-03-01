// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* FpgaGenlock.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2012 DekTec 
//
// Dta driver - FPGA genlock controller

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

#ifndef __FPGAGENLOCK_H
#define __FPGAGENLOCK_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declerations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ I2C register map +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Genlock I2c speed Control Register
#define  DTA_I2C_VCXO_SPEED_CTRL         0x18

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// DtaFpgaGenlock
typedef struct _DtaFpgaGenlock
{
    DtaDeviceData*  m_pDvcData;    

    // State of Genlock reference
    Int  m_LinesPerFrame;
    Int  m_SamplesPerLine;
    Bool  m_Locked;
    Bool  m_LockedPrev;
    Int  m_GraceCnt;
    
    // Device generated reference count
    Bool  m_ValidRefCnt;
    Int64  m_RefCntDelta;
    Int64  m_RefLinesDelta;
    UInt64  m_RefCntprev;    

    // State of the device GENLOCK REFERENCE
    Bool  m_ValidRxSignals;
    UInt32  m_RxLine, m_RxSamp;      // "published" RX line.sample position

    // State of the device GENLOCKED VCO 
    Int  m_VcxoCtrl;                     // VCXO control value *1000    
    Bool  m_I2cBusy;

    //RX
    UInt  m_RxLinePrev;
    UInt  m_RxSampPrev;
    UInt  m_RxFramePrev;
    UInt64  m_RxPosPrev;

    // TX
    UInt  m_TxFramePrev, m_TxLinePrev, m_TxSampPrev;
    UInt64  m_TxPosPrev;
    Int  m_TxMinRefPrev;                 // 27Mhz lock
    Int64  m_TxMinRxLinesPrev;           // Rx channel lock
    Int  m_TxMinRxPrev; 

    // Event handling
    DtDpc  m_EventLockedDpc; 

} DtaFpgaGenlock;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
DtStatus  DtaFpgaGenlockUpdateRx(DtaFpgaGenlock* pFpgaGenlockData, Int PortIndex);
DtStatus  DtaFpgaGenlockUpdateTiming(DtaFpgaGenlock* pFpgaGenlockData);
DtStatus  DtaFpgaGenlockUpdateTx(DtaFpgaGenlock* pFpgaGenlockData, Int PortIndex);
DtStatus  DtaFpgaGenlockInit(DtaDeviceData* pDvcData, DtaFpgaGenlock* pFpgaGenlockData);
DtStatus  DtaFpgaGenlockGetRefState(DtaDeviceData* pDvcData, Int PortIndex,  
                                                           Bool *pEnabled, Bool *pInLock);
DtStatus  DtaFpgaGenlockEnable(DtaDeviceData* pDvcData, DtaFpgaGenlock* pFpgaGenlockData);
DtStatus  DtaFpgaGenlockDisable(DtaDeviceData* pDvcData, 
                                                        DtaFpgaGenlock* pFpgaGenlockData);

#endif // #ifndef __FPGAGENLOCK_H
