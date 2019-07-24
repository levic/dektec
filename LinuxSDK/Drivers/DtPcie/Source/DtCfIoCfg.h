// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCfIoCfg.h *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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

#ifndef __DT_CF_IOCFG_H
#define __DT_CF_IOCFG_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"

// Name + short-name for the IOCONFIG core-function
#define DT_CF_IOCFG_NAME          "IoConfig"
#define DT_CF_IOCFG_SHORTNAME     "IOCFG"

// MACRO: to init an driver-function-ID for the IOCONFIG-CF
#define DT_CF_IOCFG_INIT_ID(ID, ROLE, INSTANCE, UUID)                                    \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_CF_IOCFG_NAME, DT_CF_IOCFG_SHORTNAME, ROLE, INSTANCE, UUID);    \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfIoCfg definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

typedef struct _DtCfIoCfgPortConfig
{
    DtCfIoConfigValue  m_CfgValue[DT_IOCONFIG_COUNT];  // IO-configurations
}DtCfIoCfgPortConfig;


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
struct _DtCfIoCfg
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;
    DtFastMutex  m_IoCfgMutex;              // Access protection for IO config busy
    Bool   m_IoConfigBusy;                  // Registry IO config busy flag
    DtEvent  m_IoConfigDoneEvt;             // Registry IO config done event

    DtCfIoCfgPortConfig*  m_pPortConfigs;    // IO-configuration per port
    Int  m_NumPorts;                        // Number of ports
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtCfIoCfg_Close(DtDf*);
DtCfIoCfg*  DtCfIoCfg_Open(DtCore*, const char*  pRole, Int  Instance, 
                                                             Int  Uuid, Bool  CreateStub);

DtStatus  DtCfIoCfg_Get(DtCfIoCfg*, const DtExclAccessObject*, DtIoConfig*, Int  Count);
DtStatus  DtCfIoCfg_Set(DtCfIoCfg*, const DtExclAccessObject*, const DtIoConfig*, 
                                                                              Int  Count);
DtStatus  DtCfIoCfg_Restore(DtCfIoCfg*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfIoCfg definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfIoCfg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for a IOCONFIG Core-Function
typedef struct _DtIoStubCfIoCfg
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
} DtIoStubCfIoCfg;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfIoCfg - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubCfIoCfg_Close(DtIoStub*);
DtIoStubCfIoCfg*  DtIoStubCfIoCfg_Open(DtDf*);

#endif  // #ifndef __DT_CF_IOCFG_H
