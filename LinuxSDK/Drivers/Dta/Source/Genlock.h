//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Genlock.h *#*#*#*#*#*#*#*#*# (C) 2012-2015 DekTec
//
// Dta driver - Declaration of genlock functions
//

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

#ifndef __GENLOCK_H
#define __GENLOCK_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "Lmh1982.h"
#include "Lmh1983.h"
#include "FpgaGenlock.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants / Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTA_GENLOCK_FRACMODE_NA     -1      // Fractional mode is not applicable
#define DTA_GENLOCK_FRACMODE_OFF    0       // Fractional mode is disabled
#define DTA_GENLOCK_FRACMODE_ON     1       // Fractional mode is enabled

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// DtaGenlock
typedef struct _DtaGenlock
{
    Bool  m_IsSupported;        // Genlocking is supported
    Int  m_GenlArch;            // Genlock architecture
    UInt32  m_PortGroup;        // Mask indicating which ports are under the control of 
                                // GENREF controller
    Int  m_OpModeIntSrc;        // Operational mode when locking to an internal source
    Int  m_VcxoValue;           // Manual VCXO control (-1 = no manual control)
    void*  m_pVcxoOwner;        // Handle of app controlling VCXO
    Int  m_AsyncPortIndex;      // Port-index of analog sync input (-1 = no aync port)
    Int  m_IntGenrefPortIndex;  // Port-index of internal genref port (-1 = no int port)

    Int  m_FracMode;            // Status fractional mode
    Int  m_RefPortIndex;        // Port to be used as reference input (-1=INTERNAL)
    Int  m_RefVidStd;           // Video standard on ref. input (=IOSTD of genref port)
    Int  m_OutVidStd;           // Desired output video standard (cross-lock)

    // Genlock pipeline delay parameters
    Int  m_RefLineDurationNs;   // Duration (in ns) of one line for the reference signal
    Int  m_OutLineDurationNs;   // Duration (in ns) of one line for the output signal
    Int  m_InDelayNs;           // Delay upto the LMH198X video clk generator (in ns)
    Int  m_LineOffset;          // Offset used for TOF (Top-of-Frame) pulse compenstating
                                // for delays introduced by "genlock pipeline"
    Int  m_TofAlignOffsetNs;    // Offset (ns) with which the TOF arrive at the serialiser

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
void  DtaGenlockClose(DtaDeviceData* pDvcData, DtFileObject* pFile);
DtStatus  DtaGenlockApplyGenRefConfig(DtaDeviceData* pDvcData);
DtStatus  DtaGenlockApplyFracModeConfig(DtaDeviceData* pDvcData);
DtStatus  DtaGenlockGetRefState(DtaDeviceData* pDvcData, Int PortIndex, Int *pEnabled,
                                                                            Int *pInLock);
void  DtaGenlockSetVcxo(DtaDeviceData* pDvcData, DtFileObject* pFile, Int  VcxoValue);
void  DtaGenlockResetVcxo(DtaDeviceData* pDvcData);

//DtStatus  DtaGenLockGetSyncDelays();

#endif // #ifndef __GENLOCK_H


