// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcCDMACTOTO.h *#*#*#*#*#*#*#* (C) 2017-2021 DekTec
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

#ifndef __DT_BC_CDMACTO_H
#define __DT_BC_CDMACTO_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the DmacContPcie block (must match block ID)
#define DT_BC_CDMACTO_NAME        "DmacContPcieTimeout"
#define DT_BC_CDMACTO_SHORTNAME   "CDMACTO"

//#define DT_BC_CDMACTO_NAME        "DmacContPcie"
//#define DT_BC_CDMACTO_SHORTNAME   "CDMAC"

// MACRO: to init an block-controller-ID for the CDMAC-BC
#define DT_BC_CDMACTO_INIT_ID(ID, ROLE, INSTANCE, UUID)                                  \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_CDMACTO_NAME, DT_BC_CDMACTO_SHORTNAME, ROLE, INSTANCE, UUID);\
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCDMACTO definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMACTOProfiling -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtBcCDMACTOProfiling
{
    DtSpinLock  m_PerfSpinLock;     // Profiling info protection
    UInt64  m_TotalTransferred;     // Total number of bytes transfered
    UInt64  m_TotalTime;            // Total transfertime in nanoseconds
    UInt64  m_MaxTransferSpeed;     // Maximum transfer speed
    UInt64  m_MinTransferSpeed;     // Minimum transfer speed
    UInt64  m_FirstTimeNs;          // First timestamp in nanoseconds
    UInt64  m_NumTransferredThreshold; // Minimum number tranferred before 
                                    // profiling is started
    Bool  m_ProfilingStarted;       // Profiling started flag
    void* pBc;
} DtBcCDMACTOProfiling;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMACTO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcCDMACTO
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Properties
    Int  m_Capabilities;        // DMA capability flags
    Int  m_PrefetchSize;        // Number of pointers prefetched
    Int  m_PcieItfDataWidth;    // PCIe interface data width
    Int  m_ReorderBufSize;      // Reorder buffer size

    DtBcCDMACTOProfiling  m_DmaProfiling; // DMA profiling information
    UInt  m_PrevTransferOffset; // Previous RxWrite/TxRead offset

    // DMA control items
    DtFastMutex  m_DmaMutex;    // Access protection for DMA control
    DmaDevice  m_DmaDevice;     // DMA object
    DmaDirectBuffer  m_PtrTable; // DMA pointer table buffer
    UInt8*  m_pDmaBuffer;       // DMA buffer (shared with user)
    Int  m_DmaBufferSize;       // DMA buffer size
    DmaOsSgl  m_OsSgList;       // Scatter gather list
    DtPageList  m_DmaBufferPageList; // Pagelist of DMA buffer
    // Cached settings
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Current operational mode
    Int  m_Direction;           // DMA direction
    Int  m_TestMode;            // Current test mode
} DtBcCDMACTO;


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMACTO public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcCDMACTO_Close(DtBc*);
DtBcCDMACTO*  DtBcCDMACTO_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcCDMACTO_AllocateBuffer(DtBcCDMACTO*, Int Direction, UInt8* pDmaBuffer,
                                   Int DmaBufferSize, Int BufType, DtPageList* pPageList);
DtStatus  DtBcCDMACTO_ClearProfiling(DtBcCDMACTO * pBc);
DtStatus  DtBcCDMACTO_ClearReorderBufMinMax(DtBcCDMACTO*);
DtStatus  DtBcCDMACTO_FreeBuffer(DtBcCDMACTO*, Int BufType);
DtStatus  DtBcCDMACTO_GetReorderBufStatus(DtBcCDMACTO*, Int* pBufLoad, 
                                                                     Int* pBufMinMaxLoad);
DtStatus  DtBcCDMACTO_GetProfiling(DtBcCDMACTO*, UInt64* pMaxSpeed, UInt64* pMinSpeed,
                                                   UInt64* pAvgSpeed, UInt32* pPerfCount);
DtStatus  DtBcCDMACTO_GetProperties(DtBcCDMACTO*, UInt32* pCapabilities, 
                        Int* pPrefetchSize, Int* pPcieItfDataWidth, Int* pReorderBufSize);
DtStatus  DtBcCDMACTO_GetRxWriteOffset(DtBcCDMACTO*, UInt* pRxWrOffset);
DtStatus  DtBcCDMACTO_GetStatus(DtBcCDMACTO*, Int* pOpStatus, UInt32* pStatusFlags,
                                                                     UInt32* pErrorFlags);
DtStatus  DtBcCDMACTO_GetTxReadOffset(DtBcCDMACTO*, UInt* pTxRdOffset);
DtStatus  DtBcCDMACTO_IssueChannelFlush(DtBcCDMACTO*);
DtStatus  DtBcCDMACTO_SetOperationalMode(DtBcCDMACTO*, Int OpMode);
DtStatus  DtBcCDMACTO_SetRxReadOffset(DtBcCDMACTO*, UInt RxRdOffset);
DtStatus  DtBcCDMACTO_SetTestMode(DtBcCDMACTO*, Int TestMode);
DtStatus  DtBcCDMACTO_SetTxWriteOffset(DtBcCDMACTO*, UInt TxWrOffset);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCDMACTO definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMACTO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for a DmacContPcie Block
typedef struct _DtIoStubBcCDMACTO
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcCDMACTO;


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMACTO public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcCDMACTO_Close(DtIoStub*);
DtIoStubBcCDMACTO*  DtIoStubBcCDMACTO_Open(DtBc*);


#endif  // #ifndef __DT_BC_CDMACTO_H
