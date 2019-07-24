//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSi534X.h *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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

#ifndef __DT_DF_SI534X_H
#define __DT_DF_SI534X_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcI2CM.h"

// Name + short-name for the SI534X function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_SI534X_NAME          "Si534X"
#define DT_DF_SI534X_SHORTNAME     "SI534X"

// MACRO: to init an driver-function-ID for the SI534X-DF
#define DT_DF_SI534X_INIT_ID(ID, ROLE, INSTANCE, UUID)                                  \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_SI534X_NAME, DT_DF_SI534X_SHORTNAME, ROLE,                 \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSi534X definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Structure for storing a configuration item
typedef struct  _DtDfSi534XConfigItem
{
    UInt16  m_BankData0;     // Bank and first data byte
    UInt8  m_Data1;          // Second data byte
} DtDfSi534XConfigItem;


typedef enum _DtDfSi534XConfig
{
    DT_DF_SI534X_CFG_UNDEFINED,
    DT_DF_SI534X_CFG_FREE_RUN_DUAL_CLOCK,
    DT_DF_SI534X_CFG_FREE_RUN_NONFRAC_CLOCK,
    DT_DF_SI534X_CFG_FREE_RUN_FRAC_CLOCK,
} DtDfSi534XConfig;


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfSi534X
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    DtBcI2CM*  m_pBcI2Cm;               // I2C master block controller
    DtFastMutex  m_AccessMutex;         // Access protection for SI-534X
    Int  m_Si534XAddress;               // Address of  the SI-534X device
    Int  m_NumClockOutputs;             // Number of clock outputs
    Int  m_PrevBank;                    // Previous selected bank
    DtDfSi534XConfig  m_CurConfig;      // Current loaded configuration
    const DtDfSi534XConfigItem*  m_pCurConfigItems; // Current configured items
    Int  m_CurConfigNumItems;           // Number of configured items

}  DtDfSi534X;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfSi534X_Close(DtDf*);
DtDfSi534X*  DtDfSi534X_Open(DtCore*, DtPt*  pPt, const char*  pRole, Int  Instance, 
                                                             Int  Uuid, Bool  CreateStub);
DtStatus  DtDfSi534X_GetNumClocks(DtDfSi534X*, Int*);
DtStatus  DtDfSi534X_SetConfig(DtDfSi534X*, DtDfSi534XConfig);


#endif  // #ifndef __DT_DF_SI534X_H

