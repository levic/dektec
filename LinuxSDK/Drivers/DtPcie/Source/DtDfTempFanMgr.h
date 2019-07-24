//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfTempFanMgr.h *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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

#ifndef __DT_DF_TEMPFANMGR_H
#define __DT_DF_TEMPFANMGR_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcFANC.h"
#include "DtDfSensTemp.h"

// Name + short-name for the fan manager function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_TEMPFANMGR_NAME          "TempFanMgr"
#define DT_DF_TEMPFANMGR_SHORTNAME     "TEMPFANMGR"

// MACRO: to init an driver-function-ID for the TEMPFANMGR-DF
#define DT_DF_TEMPFANMGR_INIT_ID(ID, ROLE, INSTANCE, UUID)                               \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_TEMPFANMGR_NAME, DT_DF_TEMPFANMGR_SHORTNAME, ROLE,           \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Fan control states
typedef enum _DtDfTempFanMgrFanCtrlState
{
    TEMPFANMGR_FANCTRL_DISABLED,
    TEMPFANMGR_FANCTRL_ENABLED,
}DtDfTempFanMgrFanCtrlState;


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfTempFanMgr
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    DtBcFANC*  m_pBcFanCtrl;            // Fan block-controller
    Int  m_NumFans;                     // Number of fans (0 or 1)
    DtVectorDf*  m_pDfTempSensors;      // Temperature sensors
    DtVector*  m_pTempSensorProps;      // Temperature sensors properties
    Int  m_NumTempSensors;              // Number of temperature sensors
    DtSpinLock  m_FanCtrlStateSpinLock; // Spinlock to protect m_FanCtrlState changes
    DtDfTempFanMgrFanCtrlState  m_FanCtrlState; // Fan control state

    // Fan speed control parameters
    Int  m_PeriodicInterval;            // Periodic timer interval in milliseconds
    Int  m_FanSpeed_x_Delay;            // FanSpeed * Delay
    Int  m_Delay;                       // Control delay in milliseconds per degree
    Int  m_MinFanSpeed;                 // Minimum fan-speed
}  DtDfTempFanMgr;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfTempFanMgr_Close(DtDf*);
DtDfTempFanMgr*  DtDfTempFanMgr_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                             Int  Instance,  Int  Uuid, Bool  CreateStub);

DtStatus DtDfTempFanMgr_GetFanStatus(DtDfTempFanMgr*, Int FanIdx, Int* pFanSpeedPct, 
                                Int* pFanSpeedRpm, Bool* pIsWdTimeout, Bool* pIsFanStuck);
DtStatus DtDfTempFanMgr_GetNumFans(DtDfTempFanMgr*, Int* pNumFans);
DtStatus DtDfTempFanMgr_GetNumTempSensors(DtDfTempFanMgr*, Int* pNumTempSensors);
DtStatus DtDfTempFanMgr_GetTemperature(DtDfTempFanMgr*, Int TempIdx, Int* pTemperature);
DtStatus DtDfTempFanMgr_GetTempProperties(DtDfTempFanMgr*, Int TempIdx, 
                                                                 DtDfSensTempProperties*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfTempFanMgr definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTempFanMgr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an TEMPFANMGR Driver-Function
typedef struct _DtIoStubDfTempFanMgr
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfTempFanMgr;

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTempFanMgr public functions -.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubDfTempFanMgr_Close(DtIoStub*);
DtIoStubDfTempFanMgr*  DtIoStubDfTempFanMgr_Open(DtDf*);

#endif  // #ifndef __DT_DF_TEMPFANMGR_H

