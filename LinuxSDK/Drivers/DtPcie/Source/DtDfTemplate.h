//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfTemplate.h *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
// This file is not part of the driver. It can be used as template to create a new
// driver function. Replace "TemplateName" by the driver function name (as in the 
// .FunctionDescriptionsXxx.xml), replace "TEMPLATE" by the driver function short name 
// (all uppercase) and replace "TEMPLATE" by the driver function short name in camel case.
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

#ifndef __DT_DF_TEMPLATE_H
#define __DT_DF_TEMPLATE_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcBLKCTRL.h"

// Name + short-name for the TEMPLATE function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_TEMPLATE_NAME          "TemplateName"
#define DT_DF_TEMPLATE_SHORTNAME     "TEMPLATE"

// MACRO: to init an driver-function-ID for the TEMPLATE-DF
#define DT_DF_TEMPLATE_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_TEMPLATE_NAME, DT_DF_TEMPLATE_SHORTNAME, ROLE,               \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfTemplate
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Parameters
    Int  m_Param1;                  // Parameter 1
    const char*  m_pParam2;         // Parameter 2

    // Block controllers
    DtBcBLKCTRL*  m_pBcBlkCtrl;     // Example block-controller

    // Periodic interval 
    DtSpinLock  m_PerItvSpinLock;   // Spinlock to protect m_PerItvEnable changes
    Bool  m_PerItvEnable;           // Periodic interval hander enabled

    // Helper thread
    DtThread  m_HelperThread;         // Helper thread
    DtEvent   m_HelperStopEvent;      // Helper stop event

    // Cached values
    Int  m_OperationalMode;         // Operational mode
}  DtDfTemplate;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfTemplate_Close(DtDf*);
DtDfTemplate*  DtDfTemplate_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                             Int  Instance,  Int  Uuid, Bool  CreateStub);
DtStatus  DtDfTemplate_GetOperationalMode(DtDfTemplate*, Int*  pOpMode);
DtStatus  DtDfTemplate_SetOperationalMode(DtDfTemplate*, Int OpMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfTemplate definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTemplate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an TEMPLATE Driver-Function
typedef struct _DtIoStubDfTemplate
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfTemplate;

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTemplate public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubDfTemplate_Close(DtIoStub*);
DtIoStubDfTemplate*  DtIoStubDfTemplate_Open(DtDf*);

#endif  // #ifndef __DT_DF_TEMPLATE_H

