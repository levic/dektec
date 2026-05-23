//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcFMBC.h *#*#*#*#*#*#*#*#*#*#*# (C) 2014 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2014 DekTec Digital Video B.V.
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

#ifndef __DT_BC_FMBC_H
#define __DT_BC_FMBC_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the FpgaMailboxClient block (must match block ID)
#define DT_BC_FMBC_NAME        "FpgaMailboxClient"
#define DT_BC_FMBC_SHORTNAME   "FMBC"


// MACRO: to init an block-controller-ID for the FMBC-BC
#define DT_BC_FMBC_INIT_ID(ID, ROLE, INSTANCE, UUID)                                     \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_FMBC_NAME, DT_BC_FMBC_SHORTNAME, ROLE, INSTANCE, UUID);      \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcFMBC definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Supported commands
typedef enum _DtBcFMBC_Command
{
    DT_BC_FMBC_CMD_NOOP,
    DT_BC_FMBC_CMD_CONFIG_STATUS,
    DT_BC_FMBC_CMD_GET_IDCODE,
    DT_BC_FMBC_CMD_GET_CHIPID,
    DT_BC_FMBC_CMD_GET_USERCODE,
    DT_BC_FMBC_CMD_GET_VOLTAGE,
    DT_BC_FMBC_CMD_GET_TEMPERATURE,
    DT_BC_FMBC_CMD_QSPI_OPEN,
    DT_BC_FMBC_CMD_QPSI_CLOSE,
    DT_BC_FMBC_CMD_QSPI_SET_CS,
    DT_BC_FMBC_CMD_QSPI_READ_DEVICE_REG,
    DT_BC_FMBC_CMD_QPSI_WRITE_DEVICE_REG,
    DT_BC_FMBC_CMD_QPSI_SEND_DEVICE_OP,
    DT_BC_FMBC_CMD_QPSI_ERASE,
    DT_BC_FMBC_CMD_QSPI_WRITE,
    DT_BC_FMBC_CMD_QSPI_READ,
    DT_BC_FMBC_CMD_READ_SEU_ERROR,
    DT_BC_FMBC_CMD_RSU_GET_SPT,
    DT_BC_FMBC_CMD_RSU_STATUS,
    DT_BC_FMBC_CMD_RSU_IMAGE_UPDATE,
    DT_BC_FMBC_CMD_RSU_NOTIFY,
    DT_BC_FMBC_CMD_GET_CONFIGURATION_TIME,
    DT_BC_FMBC_CMD_QPSI_READ_SHA
}DtBcFMBC_Command;



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcFMBC
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    DtDpc  m_IntDpc;
    DtEvent  m_StatusEvent;     // Event used to indicate the status changed
    DtFastMutex  m_AccessMutex; // Access protection for FMBC
    Int  m_CmdFifoSize;         // Command FIFO depth (words)
    Int  m_RespFifoSize;        // Response FIFO depth (words
    Int  m_ClockFreqHz;         // Clock frequency
    Int  m_MessageId;           // Message ID
}  DtBcFMBC;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcFMBC_Close(DtBc*);
DtBcFMBC*  DtBcFMBC_Open(Int  Address, DtCore*, DtPt*, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus  DtBcFMBC_GetProperties(DtBcFMBC*, Int* pCmdFifoSize, Int* pRespFifoSize,
                                                                       Int* pClockFreqHz);
DtStatus  DtBcFMBC_Write(DtBcFMBC*, DtBcFMBC_Command, Int Length, const UInt32* pBuffer,
                                                                             Int Timeout);
DtStatus  DtBcFMBC_WriteRead(DtBcFMBC*, DtBcFMBC_Command Command,
                                               Int WriteLength, const UInt32* pWriteBuffer,
                                               Int ReadLength, UInt32* pReadBuffer,
                                                                             Int Timeout);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcFMBC definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

////.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcFMBC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
////  Io-control stub for an FMBC Block
//typedef struct _DtIoStubBcFMBC
//{
//    // NOTE: common stub data must be the first members to allow casting to 
//    // DtBcIoCtl
//    DT_IOSTUB_BC_COMMON_DATA;
//}  DtIoStubBcFMBC;
//
//
////-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcFMBC public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//void  DtIoStubBcFMBC_Close(DtIoStub*);
//DtIoStubBcFMBC*  DtIoStubBcFMBC_Open(DtBc*);


#endif  // #ifndef __DT_BC_FMBC_H
