//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIRXPHY.h *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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

#ifndef __DT_BC_SDIRXPHY_H
#define __DT_BC_SDIRXPHY_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Switch block (must match block ID)
#define DT_BC_SDIRXPHY_NAME        "SdiRxPhy"
#define DT_BC_SDIRXPHY_SHORTNAME   "SDIRXPHY"

// MACRO: to init an block-controller-ID for the SDIRXPHY-BC
#define DT_BC_SDIRXPHY_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_SDIRXPHY_NAME, DT_BC_SDIRXPHY_SHORTNAME, ROLE,               \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXPHY definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Lock mode
#define DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_DATA  0x0  // Lock to data
#define DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF   0x1  // Lock to reference

// Device family
#define DT_BC_SDIRXPHY_FAMILY_UNKNOWN      -1  // Unknown
#define DT_BC_SDIRXPHY_FAMILY_CV            0  // Intel Cyclone V
#define DT_BC_SDIRXPHY_FAMILY_C10           1  // Intel Cyclone 10
#define DT_BC_SDIRXPHY_FAMILY_A10           2  // Intel Arria 10

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcSDIRXPHY
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Bool  m_BlockEnabled;      // Block enabled
    Int  m_OperationalMode;    // Current operational mode
    Int  m_LockMode;           // Lock mode
    Bool  m_ClockReset;        // Clock reset
    Bool  m_DownsamplerEnable; // Downsampler Enable
    Int  m_DeviceFamily;       // Device family
    Int  m_MaxSdiRate;         // Maximum SDI-rate
    Int  m_SdiRate;            // SDI-rate
    Bool  m_SetSdiRateBusy;    // Set SDI-rate is in progress
}  DtBcSDIRXPHY;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcSDIRXPHY_Close(DtBc*);
DtBcSDIRXPHY*  DtBcSDIRXPHY_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                            const char*  pRole, Int  Instance, Int  Uuid);
DtStatus  DtBcSDIRXPHY_ClearCdcFifoOverflow(DtBcSDIRXPHY*);
DtStatus  DtBcSDIRXPHY_GetCdcFifoStatus(DtBcSDIRXPHY*, Int* pFifoLoad, Bool*  pOverflow);
DtStatus  DtBcSDIRXPHY_GetClockReset(DtBcSDIRXPHY* pBc,  Bool*  pClkReset);
DtStatus  DtBcSDIRXPHY_GetDeviceFamily(DtBcSDIRXPHY* pBc,  Int* pDeviceFamily);
DtStatus  DtBcSDIRXPHY_GetMaxSdiRate(DtBcSDIRXPHY* pBc,  Int* pMaxSdiRate);
DtStatus  DtBcSDIRXPHY_GetOperationalMode(DtBcSDIRXPHY* pBc,  Int* pOpMode);
DtStatus  DtBcSDIRXPHY_GetLockMode(DtBcSDIRXPHY*, Int* pLockMode);
DtStatus  DtBcSDIRXPHY_GetSdiRate(DtBcSDIRXPHY* pBc,  Int* pSdiRate);
DtStatus  DtBcSDIRXPHY_IsCarrierDetect(DtBcSDIRXPHY*, Bool* pCarrier);
DtStatus  DtBcSDIRXPHY_IsLockedToData(DtBcSDIRXPHY*, Bool* pLocked);
DtStatus  DtBcSDIRXPHY_IsLockedToRef(DtBcSDIRXPHY*, Bool* pLocked);
DtStatus  DtBcSDIRXPHY_SetClockReset(DtBcSDIRXPHY* pBc,  Bool ClkReset);
DtStatus  DtBcSDIRXPHY_SetDownsamplerEnable(DtBcSDIRXPHY* pBc, Bool Enable);
DtStatus  DtBcSDIRXPHY_SetOperationalMode(DtBcSDIRXPHY* pBc,  Int OpMode);
DtStatus  DtBcSDIRXPHY_SetLockMode(DtBcSDIRXPHY* pBc, Int LockMode);
DtStatus  DtBcSDIRXPHY_StartSetSdiRate(DtBcSDIRXPHY* pBc,  Int SdiRate);
DtStatus  DtBcSDIRXPHY_GetSetSdiRateDone(DtBcSDIRXPHY* pBc,  Bool* pDone);
DtStatus  DtBcSDIRXPHY_FinishSetSdiRate(DtBcSDIRXPHY * pBc);

#endif  // #ifndef __DT_BC_SDIRXPHY_H