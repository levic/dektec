//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCfInt.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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

#ifndef __DT_CF_INT_H
#define __DT_CF_INT_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcMSIX.h"

// Name + short-name for the VPD function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_CF_INT_NAME          "Interrupt"
#define DT_CF_INT_SHORTNAME     "INT"

// MACRO: to init an driver-function-ID for the INTERRUPT-CF
#define DT_CF_INT_INIT_ID(ID, ROLE, INSTANCE, UUID)                                      \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_CF_INT_NAME, DT_CF_INT_SHORTNAME, ROLE, INSTANCE, UUID);        \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfInt definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
struct _DtCfInt
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    DtVector*  m_pIntHandlers;  // List with registerd interrupt handlers

    DtBcMSIX*  m_pBcMsix;       // Shortcut to the MSIX BC

};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfInt - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtCfInt_Close(DtDf*);
DtCfInt*  DtCfInt_Open(DtCore*, const char*  pRole, Int  Instance, 
                                                             Int  Uuid, Bool  CreateStub);
Bool  DtCfInt_OnInterrupt(DtCfInt*);
DtStatus  DtCfInt_HandlerRegister(DtCfInt*, const DtBcIntHandlerRegData*);
DtStatus  DtCfInt_HandlerUnregister(DtCfInt*, Int  Index, const DtBc*);

#endif  // #ifndef __DT_CF_INT_H
