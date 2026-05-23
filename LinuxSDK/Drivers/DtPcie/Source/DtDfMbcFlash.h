//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfMbcFlash.h *#*#*#*#*#*#*#*#*#*# (C) 2024 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2024 DekTec Digital Video B.V.
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

#ifndef __DT_DF_MbcFlash_H
#define __DT_DF_MbcFlash_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcFMBC.h"

// Name + short-name for the MbcFlash function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_MBCFLASH_NAME          "MbcFlash"
#define DT_DF_MBCFLASH_SHORTNAME     "MBCFLASH"

// MACRO: to init an driver-function-ID for the MbcFlash-DF
#define DT_DF_MbcFlash_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_MBCFLASH_NAME, DT_DF_MBCFLASH_SHORTNAME, ROLE,               \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

typedef enum  _DtDfMbcFlash_DeviceId
{
    MBCFLASH_DEVICE_UNDEFINED = 0,      // Undefined flash device
    MBCFLASH_DEVICE_MT25QU256           // MT25QU256 256MByte flash device
}  DtDfMbcFlash_DeviceId;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfMbcFlash
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Parameters
    const char* m_pDeviceType;      // Device type
    DtDfMbcFlash_DeviceId  m_DeviceId;  // Device ID
    DtBcFMBC* m_pBcFmbc;            // FPGA mailbox client
    UInt32* m_pTempBuffer;          // Temporary command/response buffer

    // Cached values
    Bool m_Locked;                  // Device is locked for writing/erasing/reading
    Int  m_FmbcMaxCmdSize;          // Largest chunk of data that can be written
    Int  m_FmbcMaxRespSize;         // Largest chunk of data that can be read
    Int  m_PropPageSize;            // Size of pages (in bytes)
    Int  m_PropSectorSize;          // Size of sectors (in bytes). NOTE: 0=no sectors
    Int  m_PropMemorySize;          // Size of the flash (in bytes)
    Int  m_PropMaxSectorEraseTime;  // Maximum erase time per sector in ms
    Int  m_PropMaxPageWriteTime;    // Maximum page write time in ms
}  DtDfMbcFlash;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfMbcFlash_Close(DtDf*);
DtDfMbcFlash*  DtDfMbcFlash_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                             Int  Instance,  Int  Uuid, Bool  CreateStub);
DtStatus  DtDfMbcFlash_Erase(DtDfMbcFlash*, Int StartAddress, Int NumToErase);
DtStatus  DtDfMbcFlash_LockDevice(DtDfMbcFlash*, Bool Lock);
DtStatus  DtDfMbcFlash_Read(DtDfMbcFlash*, Int StartAddress, Int NumToRead, UInt8* pBuf);
DtStatus  DtDfMbcFlash_Write(DtDfMbcFlash*, Int StartAddress, Int NumToWrite,
                                                                       const UInt8* pBuf);
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfMbcFlash definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcFlash -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an MbcFlash Driver-Function
typedef struct _DtIoStubDfMbcFlash
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfMbcFlash;

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcFlash public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubDfMbcFlash_Close(DtIoStub*);
DtIoStubDfMbcFlash*  DtIoStubDfMbcFlash_Open(DtDf*);

#endif  // #ifndef __DT_DF_MbcFlash_H

