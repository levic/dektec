//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcASIRXP.h *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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

#ifndef __DT_BC_ASIRXP_H
#define __DT_BC_ASIRXP_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the AsiRxProtocol block (must match block ID)
#define DT_BC_ASIRXP_NAME        "AsiRxProtocol"
#define DT_BC_ASIRXP_SHORTNAME   "ASIRXP"


// Polarity
#define DT_BC_ASIRXP_POL_NORMAL     0x00    // Pass ASI symbol bits unchanged
#define DT_BC_ASIRXP_POL_INVERT     0x01    // Invert ASI symbol bits

// MACRO: to init an block-controller-ID for the ASIRXP-BC
#define DT_BC_ASIRXP_INIT_ID(ID, ROLE, INSTANCE, UUID)                                   \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_ASIRXP_NAME, DT_BC_ASIRXP_SHORTNAME, ROLE, INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcASIRXP definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define  DT_BC_ASIRXP_NUM_SAMPLES_TOTAL    1024 // Number samples for long term bitrate
#define  DT_BC_ASIRXP_MAX_BURST            250  // Maximum burst (a bit more than 204)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXPBitrateMeasure -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtBcASIRXPBitrateMeasure
{
    DtSpinLock  m_BrmSpinLock;      // Bitrate measurment protection
    Int m_TsWordCntDiff[DT_BC_ASIRXP_NUM_SAMPLES_TOTAL];
                                    // Number of TS-words received per period
                                    // (=difference between successive TS-word counts)
    Int  m_TsWordCntIndex;          // Index of last stored last sample
    Int  m_NumValidSamples;         // Number of valid samples in m_TsWordCntDiff
    UInt  m_LastTsWordCnt;          // Last transport-stream word count     
    Int  m_TsWordCntTotal;          // Number TS-words in total measurement period
    Int  m_TsWordDifDifCnt;         // Integrates the  difference between current
                                    // and average TsWordCountDiff  
    Int  m_EstTsBitrate;            // Estimate TS bitrate in bits per second
    Int  m_PeriodicItvMs;           // Periodic interval in milliseconds
} DtBcASIRXPBitrateMeasure;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcASIRXP
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    UInt  m_PrevSymInCnt;       // Previous input-symbol-count
    Bool  m_CarrierDet;         // Carrier detect status
    DtBcASIRXPBitrateMeasure m_BitrateMeasure;  // Bitrate measurement   
    // Cached values
    Int  m_AsiPolarityCtrl;     // Current ASI polarity control setting
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Operational mode
    Int  m_PeriodicInterval;    // Periodic time interval in milliseconds
}  DtBcASIRXP;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASIRXP public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcASIRXP_Close(DtBc*);
DtBcASIRXP*  DtBcASIRXP_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                            const char*  pRole, Int  Instance, Int  Uuid);
DtStatus  DtBcASIRXP_GetOperationalMode(DtBcASIRXP*, Int* pOpMode);
DtStatus  DtBcASIRXP_GetPolarityCtrl(DtBcASIRXP*, Int* pPolarity);
DtStatus  DtBcASIRXP_GetStatus(DtBcASIRXP*, Bool* pCarrierDet, Bool* pAsiLock);
DtStatus  DtBcASIRXP_GetTsBitrate(DtBcASIRXP*, Int* pBitrate);
DtStatus  DtBcASIRXP_GetTsWordCount(DtBcASIRXP*, UInt* pTsWordCount);
DtStatus  DtBcASIRXP_GetSymInCount(DtBcASIRXP*, UInt* pSymInCount);
DtStatus  DtBcASIRXP_GetViolCount(DtBcASIRXP*, UInt* pViolCount);
DtStatus  DtBcASIRXP_SetOperationalMode(DtBcASIRXP*, Int OpMode);
DtStatus  DtBcASIRXP_SetPolarityCtrl(DtBcASIRXP*, Int Polarity);


#endif  // #ifndef __DT_BC_ASIRXP_H
