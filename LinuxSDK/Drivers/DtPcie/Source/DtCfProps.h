//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCfProps.h *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_CF_PROPS_H
#define __DT_CF_PROPS_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"

// Name + short-name for the VPD function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_CF_PROPS_NAME          "Properties"
#define DT_CF_PROPS_SHORTNAME     "PROPS"

// MACRO: to init an driver-function-ID for the INTERRUPT-CF
#define DT_CF_PROPS_INIT_ID(ID, ROLE, INSTANCE, UUID)                                    \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_CF_PROPS_NAME, DT_CF_PROPS_SHORTNAME, ROLE, INSTANCE, UUID);    \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfProps definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
struct _DtCfProps
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    DtPropertyData  m_PropData;     // The property store
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtCfProps_Close(DtDf*);
DtCfProps*  DtCfProps_Open(DtCore*, const char*  pRole, Int  Instance, 
                                                             Int  Uuid, Bool  CreateStub);
DtStatus  DtCfProps_Find(DtCfProps*, const char*, DtPropertyFilterCriteria,
                                                                      const DtProperty**);
DtStatus  DtCfProps_Get(DtCfProps*, const char*, DtPropertyFilterCriteria,
                                              DtPropertyValue*, DtPropertyValueType*,
                                              DtPropertyScope*,
                                              Bool  IsOptional, DtPropertyValue  Default);
DtStatus  DtCfProps_GetStr(DtCfProps*, const char*, DtPropertyFilterCriteria,
                                                     char* pStr, DtPropertyScope* pScope);
DtStatus  DtCfProps_GetTable(DtCfProps*, const char* pTableName, Int PortIndex, 
                                                    UInt MaxNumEntries, UInt* pNumEntries,
                                                    DtTableEntry*, UInt OutBufSize);
DtStatus  DtCfProps_GetTableForType(DtCfProps*, Int TypeNumber, Int SubType, 
                                                    DtPropertyFilterCriteria,
                                                    const char* pTableName, 
                                                    UInt MaxNumEntries, UInt* pNumEntries, 
                                                    DtTableEntry*, UInt OutBufSize);
Bool      DtCfProps_GetBool(DtCfProps*, const char*, Int, Bool  Default);
Int       DtCfProps_GetInt(DtCfProps*, const char*, Int, Int  Default);
Int8      DtCfProps_GetInt8(DtCfProps*, const char*, Int, Int8  Default);
Int16     DtCfProps_GetInt16(DtCfProps*, const char*, Int, Int16  Default);
Int32     DtCfProps_GetInt32(DtCfProps*, const char*, Int, Int32  Default);
Int64     DtCfProps_GetInt64(DtCfProps*, const char*, Int, Int64  Default);
UInt      DtCfProps_GetUInt(DtCfProps*, const char*, Int, UInt  Default);
UInt8     DtCfProps_GetUInt8(DtCfProps*, const char*, Int, UInt8  Default);
UInt16    DtCfProps_GetUInt16(DtCfProps*, const char*, Int, UInt16  Default);
UInt32    DtCfProps_GetUInt32(DtCfProps*, const char*, Int, UInt32  Default);
UInt64    DtCfProps_GetUInt64(DtCfProps*, const char*, Int, UInt64  Default);
DtStatus  DtCfProps_ReInit(DtCfProps*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfProps definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfProps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for a PROPS Core-Function
typedef struct _DtIoStubCfProps
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubCfProps;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfProps - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubCfProps_Close(DtIoStub*);
DtIoStubCfProps*  DtIoStubCfProps_Open(DtDf*);


#endif  // #ifndef __DT_CF_PROPS_H
