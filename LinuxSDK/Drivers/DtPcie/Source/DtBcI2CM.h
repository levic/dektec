//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcI2CM.h *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_BC_I2CM_H
#define __DT_BC_I2CM_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the I2CM block (must match block ID)
#define DT_BC_I2CM_NAME        "I2cMaster"
#define DT_BC_I2CM_SHORTNAME   "I2CM"

// MACRO: to init an block-controller-ID for the I2CM-BC
#define DT_BC_I2CM_INIT_ID(ID, ROLE, INSTANCE, UUID)                                     \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_I2CM_NAME, DT_BC_I2CM_SHORTNAME, ROLE, INSTANCE, UUID);      \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcI2CM definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcI2CM
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    DtDpc  m_IntDpc;
    DtEvent  m_DoneEvent;       // Event used to indicate the operation is completed 
    DtFastMutex  m_AccessMutex; // Access protection for I2C
    Int  m_FifoSize;            // I2c master FifoSize
    Int  m_I2cRate;             // I2c rate in Hz
    Int  m_DeviceId;            // I2C device identification
}  DtBcI2CM;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcI2CM_Close(DtBc*);
DtBcI2CM*  DtBcI2CM_Open(Int  Address, DtCore*, DtPt*, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus  DtBcI2CM_Read(DtBcI2CM*, UInt SlaveAddr, Int Length, UInt8* pBuffer);
DtStatus  DtBcI2CM_Write(DtBcI2CM*, UInt SlaveAddr, Int Length, const UInt8* pBuffer);
DtStatus  DtBcI2CM_WriteRead(DtBcI2CM*, UInt SlaveAddr,
                                               Int WriteLength, const UInt8* pWriteBuffer,
                                               Int ReadLength, UInt8* pReadBuffer);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcI2CM definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcI2CM -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an I2CM Block
typedef struct _DtIoStubBcI2CM
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcI2CM;


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcI2CM public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcI2CM_Close(DtIoStub*);
DtIoStubBcI2CM*  DtIoStubBcI2CM_Open(DtBc*);


#endif  // #ifndef __DT_BC_I2CM_H
