//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*# Max6639.h *#*#*#*#*#*#*#*#*# (C) 2012-2015 DekTec
//
// Dta driver - Maxim Max6639 - Declaration of Max6639 interface functions

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012-2015 DekTec Digital Video B.V.
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

#ifndef __MAX6639_H
#define __MAX6639_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Registers
// Global configuration
#define  DTA_MAX6639_GLOBAL_CONFIG      0x04
// Reset value
#define  DTA_MAX6639_GLOBAL_CONFIG_RST  0x40

// Max number of values for the register values table
#define MAX_MAX6639_REG_VALUES      30

typedef struct _DtaMax6639
{
    DtaDeviceData*  m_pDvcData;
    UInt16  m_I2cAddress;    
    UInt  m_NumRegValues;
    DtTableEntry m_RegisterValues[MAX_MAX6639_REG_VALUES];
} DtaMax6639;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaMax6639Init(DtaDeviceData* pDvcData);
DtStatus  DtaMax6639InitPowerUp(DtaMax6639* pMax6639);

#endif //#ifndef __MAX6639_H
