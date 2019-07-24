// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcGENL.h *#*#*#*#*#*#*#*#*#*#*# (C) 2019 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2019 DekTec Digital Video B.V.
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

#ifndef __DT_BC_GENL_H
#define __DT_BC_GENL_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Genlock block (must match block ID)
#define DT_BC_GENL_NAME        "Genlock"
#define DT_BC_GENL_SHORTNAME   "GENL"

// MACRO: to init an block-controller-ID for the GENL-BC
#define DT_BC_GENL_INIT_ID(ID, ROLE, INSTANCE, UUID)                                     \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_GENL_NAME, DT_BC_GENL_SHORTNAME, ROLE,      INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcGENL definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Crashlock status
#define DT_BC_GENL_CRASHLOCK_NORMAL    0x0  // SoFs are generated with FrameLength rythm
#define DT_BC_GENL_CRASHLOCK_ARMED     0x1  // Armed and awaiting next SoF
#define DT_BC_GENL_CRASHLOCK_ADJUST    0x2  // Adjustment period to realign SoF generation
                                            // to genlock source
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Start of Frame Callback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Signature of the start of frame handler callback function
typedef void  (*DtBcGENLOnStartOfFrameFunc)(DtObject*, const DtTodTime* pSofTod);

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENLOnStartOfFrameRegData -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Registration data for start of frame handlers
typedef struct  _DtBcGENLOnStartOfFrameRegData
{
    DtObject*  m_pObject;       // DtBc or DtDf object registering for the event
    DtBcGENLOnStartOfFrameFunc  m_OnStartOfFrameFunc;  // Callback function
}  DtBcGENLOnStartOfFrameRegData;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcGENL
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;
    Int  m_OperationalMode;
    Bool  m_FractionalClock;            // Fractional clock is used
    Int  m_FrameLength;                 // Frame length in number of clock ticks

    // Interrupts related
    DtDpc  m_IntDpc;
    DtVector*  m_pOnStartOfFrameHandlers; // List of registered on-start-of-frame handlers
    DtSpinLock  m_Lock;                   // Spinlock to Sof-handlers and operational mode
}  DtBcGENL;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGENL public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

void  DtBcGENL_Close(DtBc*);
DtBcGENL*  DtBcGENL_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus  DtBcGENL_GetCrashLockStatus(DtBcGENL*, Int* pClStatus);
DtStatus  DtBcGENL_GetOperationalMode(DtBcGENL*, Int* pOpMode);
DtStatus  DtBcGENL_SetCrashLockAdjust(DtBcGENL*, Int AdjustLength);
DtStatus  DtBcGENL_SetOperationalMode(DtBcGENL*, Int OpMode);
DtStatus  DtBcGENL_SetFrameLength(DtBcGENL* , Int NumClockTicks, Bool  FracClk);
DtStatus  DtBcGENL_StartOfFrameRegister(DtBcGENL*, const DtBcGENLOnStartOfFrameRegData*);
DtStatus  DtBcGENL_StartOfFrameUnregister(DtBcGENL*, const DtObject*);

#endif  // #ifndef __DT_BC_GENL_H
