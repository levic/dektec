// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSPIMF.h *#*#*#*#*#*#*#*# (C) 2017-2019 DekTec
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

#ifndef __DT_BC_SPIMF_H
#define __DT_BC_SPIMF_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the SpiMasterFlash block (must match block ID)
#define DT_BC_SPIMF_NAME        "SpiMasterFlash"
#define DT_BC_SPIMF_SHORTNAME   "SPIMF"

// MACRO: to init an block-controller-ID for the SPIMF-BC
#define DT_BC_SPIMF_INIT_ID(ID, ROLE, INSTANCE, UUID)                                    \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_SPIMF_NAME, DT_BC_SPIMF_SHORTNAME, ROLE, INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSPIMF definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcSPIMF
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    //DtMutex

    // Configuration data read from block
    Bool  m_SupportsV1;             // Supports the SPIMF V1 interface
    Bool  m_FlashPropsAreSet;       // Indicates whether flash properties are set
    Int  m_ConfClockRateHz;         // Frequency of SCK signal (in Hz)
    Int  m_ConfMemoryId;            // Identifier for SPI Flash/PROM device
    Int  m_ConfBitOrder;            // Preset transfer bit order, on a per-byte basis
    Int  m_ConfPageSize;            // Size of pages (in bytes)
    Int  m_ConfSectorSize;          // Size of sectors (in bytes). NOTE: 0=no sectors
    Int  m_ConfMemorySize;          // Size of the flash (in bytes)
    Int  m_ConfTxFifoSize;          // Transmit FIFO size (in bytes)
    Int  m_ConfRxFifoSize;          // Receive FIFO size (in bytes)
    // SPIMF version 1 properties
    Int  m_PageWriteTimeout;        // Page write timeout
    Int  m_BulkEraseTimeout;        // Bulk erase timeout
    Int  m_SectorEraseTimeout;      // Sector erase timeout


     // Read/Write/Erase  state
    volatile Int  m_State;          // Operation in progress
    Int  m_NumToDo;                 // Number of bytes to do
    UInt32* m_pData;                // Data pointer
    DtStatus m_Result;              // Current result
    
    DtDpc  m_IntDpc;
    DtEvent  m_OpComplEvent;        // Event used to indicate the operation is completed
    
}  DtBcSPIMF;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcSPIMF_Close(DtBc*);

DtStatus  DtBcSPIMF_Erase(DtBcSPIMF*, Int StartAddress, Int NumToErase);
DtStatus  DtBcSPIMF_LockDevice(DtBcSPIMF*, Bool Lock);
DtBcSPIMF*  DtBcSPIMF_Open(Int  Address, DtCore*, DtPt* pPt, const char*  pRole,
                                              Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcSPIMF_Read(DtBcSPIMF*, Int StartAddress, Int NumToRead, UInt8* pBuf);
DtStatus  DtBcSPIMF_Write(DtBcSPIMF*, Int StartAddress, Int NumToWrite, 
                                                                       const UInt8* pBuf);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSPIMF definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIMF -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an SPI Master Flash Block
typedef struct _DtIoStubBcSPIMF
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcSPIMF;


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIMF public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcSPIMF_Close(DtIoStub*);
DtIoStubBcSPIMF*  DtIoStubBcSPIMF_Open(DtBc*);


#endif  // #ifndef __DT_BC_SPIMF_H