// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSfp.h *#*#*#*#*#*#*#*#* (C) 2020-2024 DekTec
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

#ifndef __DT_DF_SFP_H
#define __DT_DF_SFP_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcI2CM.h"

#define SFP_CAP_10G         1
#define SFP_CAP_25G         2
#define SFP_CAP_10G_RATE    4
#define SFP_CAP_25G_RATE    8

#define SFP_INT_STAT_ABSENT  1
#define SFP_INT_STAT_PRESENT 2


// Name + short-name for the SFP function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_SFP_NAME          "SFPController"
#define DT_DF_SFP_SHORTNAME     "SFP"

// MACRO: to init an driver-function-ID for the TEMPLATE-DF
#define DT_DF_SFP_INIT_ID(ID, ROLE, INSTANCE, UUID)                                      \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_SFP_NAME, DT_DF_SFP_SHORTNAME, ROLE, INSTANCE, UUID);        \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfp definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfSfp
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Parameters
    
    // Block controllers
    DtBcI2CM*  m_pBcI2CM;

    // Cached values
    Int  m_OperationalMode;         // Operational mode
    UInt m_SfpCapabilities;         // Capabilities of SFP module
}  DtDfSfp;

// -.-.-.-.-.-.-.-.-.-.-.-.-.- SFP Interrupt callback function -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef void (*DtSfpIntFunc)(UInt SfpStatus, void* pContext);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtDfSfp*  DtDfSfp_Open(DtCore*, DtPt* pPt, const char* pRole, Int Instance,  Int Uuid);
DtStatus  DtDfSfp_GetOperationalMode(DtDfSfp*, Int*  pOpMode);
DtStatus  DtDfSfp_SetOperationalMode(DtDfSfp*, Int OpMode);
DtStatus  DtDfSfp_ReadEeprom(DtDfSfp* pDf, UInt8* pBuffer, Int StartAddr, Int Size);
DtStatus  DtDfSfp_GetSfpCapabilities(DtDfSfp* pDf, UInt* pCapabilities);

#endif  // #ifndef __DT_DF_SFP_H

