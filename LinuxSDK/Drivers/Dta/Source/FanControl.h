//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* FanControl.h *#*#*#*#*#*#*#*#*# (C) 2012-2016 DekTec
//
// Dta driver - Declaration of genlock functions
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

#ifndef __FANCONTROL_H
#define __FANCONTROL_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "Max6639.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants / Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// DtaGenlock
typedef struct _DtaFanControl
{
    Bool  m_SwControlled;               // Fan control is controlled by this driver
    Int  m_FanType;                     // Fan control type
    Int  m_FanCount;                    // Number of fans
    Int  m_RpmDiv;                      // Rotation divisor correction factor
    Int  m_RpmMult;                     // Rotation multiplier correction factor
    Int  m_RpmMinimum;                  // Minimum acceptable rotation rate

    // Specific controllers
    DtaMax6639  m_Max6639;              // MAX6639 controller
    volatile UInt8*  m_pFwbFanRegs;     // Register offset for fan monitor FPGA FW block
} DtaFanControl;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaFanControlInit(DtaDeviceData* pDvcData);
DtStatus  DtaFanControlPowerUp(DtaDeviceData* pDvcData);

#endif // #ifndef __FANCONTROL_H


