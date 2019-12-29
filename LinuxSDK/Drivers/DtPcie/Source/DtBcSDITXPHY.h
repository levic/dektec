//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDITXPHY.h *#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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

#ifndef __DT_BC_SDITXPHY_H
#define __DT_BC_SDITXPHY_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Switch block (must match block ID)
#define DT_BC_SDITXPHY_NAME        "SdiTxPhy"
#define DT_BC_SDITXPHY_SHORTNAME   "SDITXPHY"

// MACRO: to init an block-controller-ID for the SDITXPHY-BC
#define DT_BC_SDITXPHY_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_SDITXPHY_NAME, DT_BC_SDITXPHY_SHORTNAME, ROLE,               \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)

// ARM status
#define DT_BC_SDITXPHY_ARMSTATUS_IDLE       0   // Transmission starts when operational
                                                // mode becomes RUN
#define DT_BC_SDITXPHY_ARMSTATUS_PREARMED   1   // Armed and awaiting RUN
#define DT_BC_SDITXPHY_ARMSTATUS_ARMED      2   // Armed and awaiting GenlockSoF before
                                                // starting transmission

// Device family
#define DT_BC_SDITXPHY_FAMILY_UNKNOWN      -1  // Unknown
#define DT_BC_SDITXPHY_FAMILY_CV            0  // Intel Cyclone V
#define DT_BC_SDITXPHY_FAMILY_C10           1  // Intel Cyclone 10
#define DT_BC_SDITXPHY_FAMILY_A10           2  // Intel Arria 10

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXPHY definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcSDITXPHY
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Bool  m_BlockEnabled;      // Block enabled
    Bool  m_SupportsStartOnSof;  // Support start-on-SoF
    Int  m_SofDelay;           // Start-on-Frame delay in clock ticks
    Int  m_SdiRate;            // SDI-rate used for setting the slew-rate
    Int  m_OperationalMode;    // Current operational mode
    Int  m_NumClocks;          // Number of clock signals
    Int  m_PllIdClk1;          // PllId clock 1
    Int  m_PllIdClk2;          // PllId clock 2
    Int  m_DeviceFamily;       // Device family
    Bool  m_ClockReset;        // Clock reset
    Int  m_UpsampleFactor;     // Up-sample factor
    Int  m_MaxSdiRate;         // Maximum SDI-rate
    Bool  m_C10A10FractClock;  // A10/C10 fractional clock
}  DtBcSDITXPHY;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcSDITXPHY_Close(DtBc*);
DtBcSDITXPHY*  DtBcSDITXPHY_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                            const char*  pRole, Int  Instance, Int  Uuid);
DtStatus  DtBcSDITXPHY_ArmForSof(DtBcSDITXPHY* pBc);
DtStatus  DtBcSDITXPHY_ClearUnderflowFlag(DtBcSDITXPHY* pBc);
DtStatus  DtBcSDITXPHY_GetArmStatus(DtBcSDITXPHY* pBc,  Int*  pArmStatus);
DtStatus  DtBcSDITXPHY_GetClockReset(DtBcSDITXPHY* pBc,  Bool*  pClkReset);
DtStatus  DtBcSDITXPHY_GetDeviceFamily(DtBcSDITXPHY* pBc,  Int* pDeviceFamily);
DtStatus  DtBcSDITXPHY_GetMaxSdiRate(DtBcSDITXPHY* pBc,  Int* pMaxSdiRate);
DtStatus  DtBcSDITXPHY_GetNumClocks(DtBcSDITXPHY* pBc,  Int* pNumClocks);
DtStatus  DtBcSDITXPHY_GetOperationalStatus(DtBcSDITXPHY* pBc,  Int* pOpStatus);
DtStatus  DtBcSDITXPHY_GetTxPllId(DtBcSDITXPHY* pBc,  Int* pTxPllId);
DtStatus  DtBcSDITXPHY_GetSdiRate(DtBcSDITXPHY* pBc,  Int* pSdiRate);
DtStatus  DtBcSDITXPHY_GetSofDelay(DtBcSDITXPHY* pBc,  Int* pSofDelay);
DtStatus  DtBcSDITXPHY_GetUnderflowFlag(DtBcSDITXPHY* pBc, Bool* pUnderflow);
DtStatus  DtBcSDITXPHY_IsResetInProgress(DtBcSDITXPHY* pBc, Bool* pInProgress);
DtStatus  DtBcSDITXPHY_SetClockReset(DtBcSDITXPHY* pBc,  Bool ClkReset);
DtStatus  DtBcSDITXPHY_SetOperationalMode(DtBcSDITXPHY* pBc,  Int OpMode);
DtStatus  DtBcSDITXPHY_SetSdiRate(DtBcSDITXPHY* pBc,  Int SdiRate);
DtStatus  DtBcSDITXPHY_SetSofDelay(DtBcSDITXPHY* pBc,  Int SofDelay);

// Aria 10/ Cyclone 10 specific functions
DtStatus  DtBcSDITXPHY_C10A10_GetCalibrationDone(DtBcSDITXPHY* pBc, Bool* pDone);
DtStatus  DtBcSDITXPHY_C10A10_GetSdiFractionalClock(DtBcSDITXPHY* pBc, Bool*  pFracClk);
DtStatus  DtBcSDITXPHY_C10A10_SetSdiFractionalClock(DtBcSDITXPHY* pBc, Bool  FracClk);
DtStatus  DtBcSDITXPHY_C10A10_StartCalibration(DtBcSDITXPHY* pBc);

#endif  // #ifndef __DT_BC_SDITXPHY_H
