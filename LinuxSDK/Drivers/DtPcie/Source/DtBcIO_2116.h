//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIO_2116.h *#*#*#*#*#*#*#*#*#*# (C) 2021 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2021 DekTec Digital Video B.V.
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

#ifndef __DT_BC_IO_2116_H
#define __DT_BC_IO_2116_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Gpio_2116 block (must match block ID)
#define DT_BC_IO_2116_NAME        "Gpio2116"
#define DT_BC_IO_2116_SHORTNAME   "IO"

// MACRO: to init an block-controller-ID for the IO_2116-BC
#define DT_BC_IO_2116_INIT_ID(ID, ROLE, INSTANCE, UUID)                            \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_IO_2116_NAME, DT_BC_IO_2116_SHORTNAME, ROLE,     \
                                                                        INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIO_2116 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcIO_2116
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    DtFastMutex  m_AccessMutex;     // Access protection for IO
    Int  m_10MhzClkSelect;
    Bool  m_CalibSwitch1;
    Bool  m_CalibSwitch2;
    Bool  m_DacTxEnable;
    Bool  m_OnePpsTermination;
}  DtBcIO_2116;

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcIO_2116 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcIO_2116_Close(DtBc*);
DtBcIO_2116*  DtBcIO_2116_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIO_2116 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an IO_2116 Block
typedef struct _DtIoStubBcIO_2116
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcIO_2116;

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcIO_2116_Close(DtIoStub*);
DtIoStubBcIO_2116*  DtIoStubBcIO_2116_Open(DtBc*);
DtStatus  DtBCIO_2116_DacReset(DtBcIO_2116*);
DtStatus  DtBcIO_2116_Get10MhzClockSelect(DtBcIO_2116*, Int*);
DtStatus  DtBcIO_2116_GetCalibrationSwitch(DtBcIO_2116*, Bool*, Bool*);
DtStatus  DtBcIO_2116_GetClockGenStatus(DtBcIO_2116*, Bool*);
DtStatus  DtBcIO_2116_GetDacTxEnable(DtBcIO_2116*, Bool*);
DtStatus  DtBcIO_2116_GetOnePpsTermination(DtBcIO_2116*, Bool*);
DtStatus  DtBcIO_2116_Set10MhzClockSelect(DtBcIO_2116*, Int);
DtStatus  DtBcIO_2116_SetCalibrationSwitch(DtBcIO_2116*, Bool, Bool);
DtStatus  DtBcIO_2116_SetDacTxEnable(DtBcIO_2116*, Bool);
DtStatus  DtBcIO_2116_SetOnePpsTermination(DtBcIO_2116*, Bool);
DtStatus  DtBcIO_2116_InitClockGen(DtBcIO_2116*);

#endif  // #ifndef __DT_BC_IO_2116_H
