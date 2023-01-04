// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfChSdiRxPhyOnly.h *#*#*#*#*#*#*#*#* (C) 2022 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2022 DekTec Digital Video B.V.
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

#ifndef __DT_DF_CHSDIRXPHYONLY_H
#define __DT_DF_CHSDIRXPHYONLY_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDfCh.h"
#include "DtDfSdiRx.h"              // SdiRx driver function

// Name + short-name for the SDIPHYONLYRXCHAN function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_CHSDIRXPHYONLY_NAME              "SdiRxPhyOnlyChannel"
#define DT_DF_CHSDIRXPHYONLY_SHORTNAME         "CHSDIRXPHYONLY"

// MACRO: to init an driver-function-ID for the SDIPHYORXCHAN-DF
#define DT_DF_CHSDIRXPHYONLY_INIT_ID(ID, ROLE, INSTANCE, UUID)                           \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_CHSDIRXPHYONLY_NAME, DT_DF_CHSDIRXPHYONLY_SHORTNAME,         \
                                                                 ROLE, INSTANCE, UUID);  \
}                                                                                        \
while (0)

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfChSdiRxPhyOnly definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- struct DtDfChSdiRxPhyOnlyUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Defines a user for an SDI PHY-only receive channel
typedef struct _DtDfChSdiRxPhyOnlyUser
{
    // NOTE: common chan-user-data must be the first member to allow casting to DtDfChUser
    DT_DF_CH_USER_COMMON_DATA;
    Bool m_IsConfigured;            // User has successfully applied its configuration
} DtDfChSdiRxPhyOnlyUser;

// -.-.-.-.-.-.-.-.-.-.-.-.-.- struct DtDfChSdiRxPhyOnlyConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Defines the configuration of an SDI PHY-only receive channel
typedef struct _DtDfChSdiRxPhyOnlyConfig
{
    Int m_SdiRate;                  // SDI-Rate

} DtDfChSdiRxPhyOnlyConfig;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct DtDfChSdiRxPhyOnly -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtDfChSdiRxPhyOnly
{
    // NOTE: common channel data must be the first members to allow casting to DtCh
    DT_DF_CH_COMMON_DATA;

    Bool m_IsConfigured;            // Channel has been configured
    DtDfChSdiRxPhyOnlyConfig m_Config;  // Currently active configuration

    // =+=+=+=+=+=+=+=+=+ BCs and DFs needed to control an SDI channel +=+=+=+=+=+=+=+=+=+
    
    DtDfSdiRx* m_pDfSdiRx;          // SDI Receive driver function

} DtDfChSdiRxPhyOnly;

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly - Public functions -.-.-.-.-.-.-.-.-.-.-.-.
void DtDfChSdiRxPhyOnly_Close(DtDf*);
DtStatus DtDfChSdiRxPhyOnly_Lock(DtDfChSdiRxPhyOnly*, int Timeout);
DtDfChSdiRxPhyOnly* DtDfChSdiRxPhyOnly_Open(DtCore*, DtPt*  pPt, const char*  pRole, 
                                              Int  Instance, Int  Uuid, Bool  CreateStub);
void DtDfChSdiRxPhyOnly_Unlock(DtDfChSdiRxPhyOnly*);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfChSdiRxPhyOnly definitions +=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.- struct DtIoStubDfChSdiRxPhyOnly -.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SDIPHYONLYRXCHAN Driver-Function
typedef struct _DtIoStubDfChSdiRxPhyOnly
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfChSdiRxPhyOnly;

// -.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRxPhyOnly - Public functions -.-.-.-.-.-.-.-.-.-.-
void DtIoStubDfChSdiRxPhyOnly_Close(DtIoStub*);
DtIoStubDfChSdiRxPhyOnly* DtIoStubDfChSdiRxPhyOnly_Open(DtDf*);

#endif // #ifndef __DT_DF_CHSDIRXPHYONLY_H
