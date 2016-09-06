//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* ProgItf.h *#*#*#*#*#*#*#*#*# (C) 2012-2016 DekTec
//
// Dta driver - Declaration of programming interface functions
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

#ifndef __PROGITF_H
#define __PROGITF_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants / Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// DtaGenlock
typedef struct _DtaProgItf
{
    Int  m_InterfaceType;
    Bool  m_SuppFwReboot;

    // Exclusive access (usage) of the programming interface
    Bool  m_ExclAccess;
    DtFileObject  m_ExclAccessOwner;
    DtSpinLock  m_ExclAccessLock;
} DtaProgItf;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaProgItfIoctl(DtaDeviceData*  pDvcData, DtFileObject*  pFile,  
                                          DtIoctlObject*  pIoctl, Bool  PowerDownPending);
DtStatus  DtaProgItfInit(DtaDeviceData* pDvcData);
DtStatus  DtaProgItfClose(DtaDeviceData* pDvcData, DtFileObject* pFile);
DtStatus  DtaProgItfExclusiveAccess(DtaProgItf* pProgItf, DtFileObject*  pFile, Int  Cmd);
#endif // #ifndef __PROGITF_H


