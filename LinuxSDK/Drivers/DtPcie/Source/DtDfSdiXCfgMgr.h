//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSdiXCfgMgr.h *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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

#ifndef __DT_DF_SDIXCFGMGR_H
#define __DT_DF_SDIXCFGMGR_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcSDIXCFG.h"

// Name + short-name for the SDIXCFGMGR function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_SDIXCFGMGR_NAME          "SdiXcvrReconfigMgr"
#define DT_DF_SDIXCFGMGR_SHORTNAME     "SDIXCFGMGR"

// MACRO: to init an driver-function-ID for the ASITSRX-DF
#define DT_DF_SDIXCFGMGR_INIT_ID(ID, ROLE, INSTANCE, UUID)                               \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_SDIXCFGMGR_NAME, DT_DF_SDIXCFGMGR_SHORTNAME, ROLE, INSTANCE, \
                                                                                   UUID);\
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSdiXCfgMgr definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiXCfgMgr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfSdiXCfgMgr
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Block controllers
    DtVectorBc*  m_pBcSdiXCfgs;            // SdiXcvrReconfig block controllers
    DtVector*  m_pPortIdxToSdiXCfgsChans;  // Lookup table from PortIndex to reconfig 
                                           // channels
}  DtDfSdiXCfgMgr;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfSdiXCfgMgr_Close(DtDf*);
DtDfSdiXCfgMgr*  DtDfSdiXCfgMgr_Open(DtCore*, DtPt*  pPt, const char*  pRole, 
                                                              Int  Instance,  Int  Uuid);
DtStatus  DtDfSdiXCfgMgr_SetXcvrMode(DtDfSdiXCfgMgr*, Int PortIndex, Int ChannelType,
                                                                            Int XcvrMode);

#endif  // #ifndef __DT_DF_SDIXCFGMGR_H

