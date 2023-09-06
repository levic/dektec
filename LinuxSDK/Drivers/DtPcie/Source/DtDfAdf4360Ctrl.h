// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfAdf4360Ctrl.h *#*#*#*#*#*#*#*#*# (C) 2023 DekTec
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

#ifndef __DT_DF_ADF4360CTRL_H
#define __DT_DF_ADF4360CTRL_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcSPIM.h"


// Name + short-name for the ADF4360CTRL function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_ADF4360CTRL_NAME          "Adf4360Ctrl"
#define DT_DF_ADF4360CTRL_SHORTNAME     "ADF4360CTRL"

// MACRO: to init an driver-function-ID for the ADF4360CTRL-DF
#define DT_DF_ADF4360CTRL_INIT_ID(ID, ROLE, INSTANCE, UUID)                              \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_ADF4360CTRL_NAME, DT_DF_ADF4360CTRL_SHORTNAME, ROLE,         \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAdf4360Ctrl definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAdf4360Ctrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfAdf4360Ctrl
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Block controllers
    DtBcSPIM*  m_pBcSpiM;               // SPI master block controller
    const UInt32* m_ConfigData;         // ADF4360 configuration data
}  DtDfAdf4360Ctrl;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfAdf4360Ctrl_Close(DtDf*);
DtDfAdf4360Ctrl*  DtDfAdf4360Ctrl_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                                               Int  Instance,  Int  Uuid);
#endif  // #ifndef __DT_DF_ADF4360CTRL_H

