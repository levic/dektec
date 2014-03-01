//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Lmh1983.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2013 DekTec
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

#ifndef __LMH1983_H
#define __LMH1983_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declerations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Master statemachine states
#define  DTA_LMH1983_STATE_INIT         0
#define  DTA_LMH1983_STATE_NOREF        1
#define  DTA_LMH1983_STATE_PLL_LOCKING  2
#define  DTA_LMH1983_STATE_PLL_LOCKED   3
#define  DTA_LMH1983_STATE_ALIGN_TOF    4
#define  DTA_LMH1983_STATE_FULL_LOCK    5
#define  DTA_LMH1983_STATE_HOLD_OVER1   6
#define  DTA_LMH1983_STATE_HOLD_OVER2   7

#define  DTA_LMH1983_I2C_ADDR           0xCC    // I2C device address for LMH1983

// DtaLmh1983
typedef struct _DtaLmh1983
{
    DtaDeviceData*  m_pDvcData;

    DtThread  m_ControlThread;  // LMH-1982 controller thread
    Int  m_State;               // State of master state machine
    DtFastMutex  m_StateMutex;  // Mutex to protect the state-machine state
} DtaLmh1983;


#define  DTA_LMH1983_REG_DEVICE_STATUS1 0x00
#define  DTA_LMH1983_REG_DEVICE_STATUS2 0x01
#define  DTA_LMH1983_REG_DEVICE_CTRL    0x05
#define  DTA_LMH1983_REG_OUT_MODE_MISC  0x09
#define  DTA_LMH1983_REG_OUT_BUF_CTRL   0x0A
#define  DTA_LMH1983_REG_OUT_FRAME_CTRL_OFF1_MSB 0x0B
#define  DTA_LMH1983_REG_OUT_FRAME_CTRL_OFF1_LSB 0x0C
#define  DTA_LMH1983_REG_ALIGNMENT_CTRL_TOF1 0x11
#define  DTA_LMH1983_REG_INPUT_FORMAT   0x20

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
DtStatus  DtaLmh1983Init(DtaDeviceData*  pDvcData, DtaLmh1983*  pLmh1983Data);
DtStatus  DtaLmh1983InitPowerup(DtaLmh1983* pLmh1983Data);
DtStatus  DtaLmh1983ResetStateMachine(DtaLmh1983*  pLmh1983Data);
DtStatus  DtaLmh1983PowerdownPre(DtaLmh1983*  pLmh1983Data);

#endif // #ifndef __LMH1983_H
