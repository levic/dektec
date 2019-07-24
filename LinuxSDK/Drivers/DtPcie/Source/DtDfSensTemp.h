//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSensTemp.h *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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

#ifndef __DT_DF_SENSTEMP_H
#define __DT_DF_SENSTEMP_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcFPGATEMP.h"
#include "DtDfMxDs75Temp.h"

// Name + short-name for the fan manager function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_SENSTEMP_NAME          "SensTemp"
#define DT_DF_SENSTEMP_SHORTNAME     "SENSTEMP"

// MACRO: to init an driver-function-ID for the TEMP-DF
#define DT_DF_SENSTEMP_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_SENSTEMP_NAME, DT_DF_SENSTEMP_SHORTNAME, ROLE, INSTANCE,     \
                                                                             UUID);      \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSensTemp definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Temperature sensor types
#define DT_DF_SENSTEMP_SENS_TYPE_UNKNOWN    0      // Unknown temperature sensor
#define DT_DF_SENSTEMP_SENS_TYPE_FPGA       1      // FPGA temperature sensor
#define DT_DF_SENSTEMP_SENS_TYPE_MXDS75     2      // MAXIM DS75


// Properties
typedef struct _DtDfSensTempProperties
{
    Int  m_TempSensorType;          // Temperature sensor type
    Int  m_MaximumTemperature;      // Maximum temperature
    Int  m_TargetTemperature;       // Target temperature
    const char*  m_pTempSensorName; // Temperature name
} DtDfSensTempProperties;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSensTemp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfSensTemp
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Block controllers / driver functions
    DtBcFPGATEMP*  m_pBcFpgaTemp;       // FPGA temperature block-controller
    DtDfMxDs75Temp*  m_pDfMxDs75Temp;   // MAXIM DS75 temperature driver function

    Int  m_TempSensorType;              // Sensor type
    Int  m_MaxTemperature;              // Maximum temperature
    Int  m_TargetTemperature;           // Target temperature
    const char*  m_pTempSensorName;     // Temperature sensor name
}  DtDfSensTemp;



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfSensTemp_Close(DtDf*);
DtDfSensTemp*  DtDfSensTemp_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                             Int  Instance,  Int  Uuid, Bool  CreateStub);
DtStatus  DtDfSensTemp_GetProperties(DtDfSensTemp*, DtDfSensTempProperties*);
DtStatus  DtDfSensTemp_GetTemperature(DtDfSensTemp*,Int* pTemperature);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSensTemp definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSensTemp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an sensor temperature Driver-Function
typedef struct _DtIoStubDfSensTemp
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfSensTemp;

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSensTemp public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubDfSensTemp_Close(DtIoStub*);
DtIoStubDfSensTemp*  DtIoStubDfSensTemp_Open(DtDf*);

#endif  // #ifndef __DT_DF_SENSTEMP_H

