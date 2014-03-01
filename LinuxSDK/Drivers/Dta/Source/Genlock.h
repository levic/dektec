//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Genlock.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2012 DekTec
//
// Dta driver - Declaration of genlock functions

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

#ifndef __GENLOCK_H
#define __GENLOCK_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "Lmh1982.h"
#include "Lmh1983.h"
#include "FpgaGenlock.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants / Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTA_GENLOCK_ARCH_NONE   0
#define DTA_GENLOCK_ARCH_145    145
#define DTA_GENLOCK_ARCH_2144   2144
#define DTA_GENLOCK_ARCH_2152   2152
#define DTA_GENLOCK_ARCH_2154   2154

#define DTA_GENLOCK_REFPORT_INT -1  // -1 denotes internal reference clock 

#define DTA_GENLOCK_FRACMODE_NA     -1      // Fractional mode is not applicable
#define DTA_GENLOCK_FRACMODE_OFF    0       // Fractional mode is disabled
#define DTA_GENLOCK_FRACMODE_ON     1       // Fractional mode is enabled

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// DtaGenlock
typedef struct _DtaGenlock
{
    Bool  m_IsSupported;        // Genlocking is supported
    Int  m_GenlArch;            // Genlock architecture
    Int  m_AsyncPortIndex;      // Port-index of analog sync input (-1 = no aync port)

    Int  m_FracMode;            // Status fractional mode
    Int  m_RefPortIndex;        // Port to be used as reference input (-1=INTERNAL)
    Int  m_RefVidStd;           // Video standard on reference input 

    volatile UInt8*  m_pGenlRegs; // Pointer to base of genlock registers
    
    // Specific controllers
    DtaLmh1982  m_Lmh1982;              // LMH-1982 controller
    DtaLmh1983  m_Lmh1983;              // LMH-1983 controller
    DtaFpgaGenlock  m_FpgaGenlock;      // Genlock controlled by the FPGA
    
} DtaGenlock;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtaGenlockCleanupPre(DtaDeviceData* pDvcData);
void  DtaGenlockCleanup(DtaDeviceData* pDvcData);
DtStatus  DtaGenlockInit(DtaDeviceData* pDvcData);
DtStatus  DtaGenlockInitPowerup(DtaDeviceData* pDvcData);
void  DtaGenlockPowerDown(DtaDeviceData* pDvcData);
DtStatus  DtaGenlockPowerDownPre(DtaDeviceData* pDvcData);
DtStatus  DtaGenlockApplyGenRefConfig(DtaDeviceData* pDvcData);
DtStatus  DtaGenlockApplyFracModeConfig(DtaDeviceData* pDvcData);
DtStatus  DtaGenlockGetRefState(DtaDeviceData* pDvcData, Int PortIndex, Int *pEnabled,
                                                                            Int *pInLock);
#endif // #ifndef __GENLOCK_H


