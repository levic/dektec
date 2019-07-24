//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIXCFG.h *#*#*#*#*#*#*#*# (C) 2017-2018 DekTec
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

#ifndef __DT_BC_SDIXCFG_H
#define __DT_BC_SDIXCFG_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_BC_SDIXCFG_MAX_NUM_LOG_CHAN  27     // Maximum number of logical channels


// Name and short-name of the SDI XCVR reconfig block (must match block ID)
#define DT_BC_SDIXCFG_NAME        "SdiXcvrReconfig"
#define DT_BC_SDIXCFG_SHORTNAME   "SDIXCFG"

// MACRO: to init an block-controller-ID for the SDIXCFG-BC
#define DT_BC_SDIXCFG_INIT_ID(ID, ROLE, INSTANCE, UUID)                                  \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_SDIXCFG_NAME, DT_BC_SDIXCFG_SHORTNAME, ROLE, INSTANCE, UUID);\
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIXCFG definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcSDIXCFG
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    DtSpinLock   m_ConfigLock;    // Protection against mutual reconfiguration
    // Cached settings
    Int  m_NumLogChannels;          // Number of logical channels
    DtSdiXCfgChanConfig  m_ChanConfig[DT_BC_SDIXCFG_MAX_NUM_LOG_CHAN];  // Channel config
    UInt32  m_DataMlCounter[DT_BC_SDIXCFG_MAX_NUM_LOG_CHAN];        // Data Ml counters
}  DtBcSDIXCFG;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcSDIXCFG_Close(DtBc*);
DtBcSDIXCFG*  DtBcSDIXCFG_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);

DtStatus  DtBcSDIXCFG_GetConfig(DtBcSDIXCFG*, DtSdiXCfgChanConfig*,
                                                         Int MaxNumToGet,  Int*pNumGot);
DtStatus  DtBcSDIXCFG_GetLogChannel(DtBcSDIXCFG*, Int  PortIndex, Int ChanType,
                                                                        Int* pLogChanNum);
DtStatus  DtBcSDIXCFG_SetMode(DtBcSDIXCFG* pBc, Int LogChanNum, Int Mode);



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIXCFG definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIXCFG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SDIXCFG Block
typedef struct _DtIoStubBcSDIXCFG
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcSDIXCFG;


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIXCFG public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcSDIXCFG_Close(DtIoStub*);
DtIoStubBcSDIXCFG*  DtIoStubBcSDIXCFG_Open(DtBc*);


#endif  // #ifndef __DT_BC_SDIXCFG_H