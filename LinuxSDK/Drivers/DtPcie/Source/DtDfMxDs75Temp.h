//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfMxDs75Temp.h *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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

#ifndef __DT_DF_MXDS75TEMP_H
#define __DT_DF_MXDS75TEMP_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcI2CM.h"

// Driver function that controls the MAXIM DS75 digital thermometer and thermostat

// Name + short-name for the MXDS75TEMP function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_MXDS75TEMP_NAME          "MxDs75"
#define DT_DF_MXDS75TEMP_SHORTNAME     "MXDS75TEMP"

// MACRO: to init an driver-function-ID for the MXDS75TEMP-DF
#define DT_DF_MXDS75TEMP_INIT_ID(ID, ROLE, INSTANCE, UUID)                               \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_MXDS75TEMP_NAME, DT_DF_MXDS75TEMP_SHORTNAME, ROLE,           \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMxDs75Temp definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=



//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMxDs75Temp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfMxDs75Temp
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;
    DtBcI2CM*  m_pBcI2Cm;               // I2C master block controller
    Int  m_MxDs75Address;               // Address of the MAXIM DS75
   
    // Temperature is updated is a separate thread once per 100ms
    DtThread  m_TempReadThread;         // Temperature read thread
    DtEvent   m_TempReadStopEvent;      // Temperature read stop event

    DtSpinLock  m_SpinLock;             // Lock for  temperature and status
    Int  m_Temperature;                 // Last read temperature
    DtStatus  m_TempReadStatus;         // Temperature read status
}  DtDfMxDs75Temp;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfMxDs75Temp_Close(DtDf*);
DtDfMxDs75Temp*  DtDfMxDs75Temp_Open(DtCore*, DtPt*, const char*  pRole, Int  Instance, 
                                                             Int  Uuid, Bool  CreateStub);
DtStatus  DtDfMxDs75Temp_GetTemperature(DtDfMxDs75Temp*, Int* pTemperature);

#endif  // #ifndef __DT_DF_MXDS75TEMP_H

