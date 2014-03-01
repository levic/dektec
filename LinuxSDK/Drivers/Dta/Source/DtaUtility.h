//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaUtility.h *#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
//
// Dta driver - DTA utility functions.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
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

#ifndef __DTA_UTILITY_H
#define __DTA_UTILITY_H

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Macros +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// PCI-bus register read/write.
// pRegs must be declared volatile UInt8*.
#define READ_UINT32(p, offs)        (*(volatile UInt32*)(((UInt8*)p)+(offs)))
#define READ_UINT8(p, offs)         (*(volatile UInt8*)(((UInt8*)p)+(offs)))

#define WRITE_UINT32(p, offs, d)    (*(volatile UInt32*)(((UInt8*)p)+(offs))=(UInt32)(d))
#define WRITE_UINT16(p, offs, d)    (*(volatile UInt16*)(((UInt8*)p)+(offs))=(UInt16)(d))
#define WRITE_UINT8(p, offs, d)     (*(volatile UInt8*)(((UInt8*)p)+(offs))=(UInt8)(d))

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Ping pong buffer defines +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
typedef void  (*DtaPPBufferGetLocAddrFunc)(void* pContext, UInt8** ppLocalAddress, 
                                            UInt* pStart, UInt* pEnd, UInt* TransferSize);

// Buffer index ID's
#define DTA_PPBUF_PING_ID   0
#define DTA_PPBUF_PONG_ID   1

// Buffer states
#define  DTA_PPBUF_STAT_DMA_BUSY             1
#define  DTA_PPBUF_STAT_PING                 2      // Buffer ready for DMA
#define  DTA_PPBUF_STAT_PONG                 4      // Buffer ready for DMA

typedef struct _PPBuffer
{
    UInt8*  m_pBufStart;            // Start of the buffer
    DtPageList*  m_pPageList;       // optional pagelist for large memory allocation    
    UInt  m_BufOffset[2];           // Offset of ping/pong buffer
    UInt  m_BufSize[2];             // Size of ping/pong buffer(usually the same for both)
    UInt  m_BufTransferSize[2];     // Size of data (to be) transferred ping or pong buffer
    UInt  m_CurRwBufferId;          // Buffer being read/written by 'user'.
    UInt  m_CurDmaBufferId;         // Buffer being transferred by 'DMA'
    UInt  m_BufState;               // State of DMA and filled/empty states
    DmaChannel*  m_pDmaChannel;     // DMA channel used for the transfer
    DtaPPBufferGetLocAddrFunc  m_pGetLocAddrFunc;
                                    // Function to retrieve new local address
    void*  m_pGetLocAddrContext;    // Context given to m_pGetLocAddrFunc function
    Bool  m_AutoTransferAfterComplete;
                                    // Restart new transfer after DMA is complete
} PPBuffer;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

UInt16  DtaSubsystemId2TypeNumber(Int SubsystemId);
Int  DtaDeviceId2SubDvcNumber(Int TypeNumber, Int  DeviceId);
void DtaWatchdogToggle(volatile UInt8* pBase);
void DtaWatchdogPulse(volatile UInt8* pBase);
UInt32 DtaGetPerIntItvUS(DtaDeviceData* pDvcData);
void  DtaPPBufferInitInternalStates(PPBuffer* pPPBuffer, UInt DmaDirection);
void  DtaPPBufferInitialise(PPBuffer* pPPBuffer, 
                               UInt8* pBufferStart, DtPageList* pPageList, UInt BufSize,
                               DmaChannel* pDmaChannel, 
                               DtaPPBufferGetLocAddrFunc pGetLocAddrFunc,
                               void* pGetLocAddrContext, Bool  AutoTransferAfterComplete);
DtStatus  DtaPPBufferTransferData(PPBuffer* pPPBuffer);
DtStatus  DtaPPBufferTransferDataContext(PPBuffer* pPPBuffer, void* pGetLocalAddrContext);
UInt  DtaPPBufferGetTransferSize(PPBuffer* pPPBuffer);
Bool  DtaPPBufferWriteDataIsBufAvailable(PPBuffer* pPPBuffer);
DtStatus  DtaPPBufferWriteData(UInt8* pSourceBuffer, PPBuffer* pPPBuffer, UInt DataSize);
void  DtaPPBufferWriteDataFinish(PPBuffer* pPPBuffer);
void  DtaPPBufferWriteDataClearBuf(PPBuffer* pPPBuffer);
void*  DtaPPBufferReadDataGetBuffer(PPBuffer* pPPBuffer);
void  DtaPPBufferReadDataFinished(PPBuffer* pPPBuffer);

DtStatus  DtaPropertiesInit(DtaDeviceData* pDvcData);
DtStatus  DtaPropertiesReportDriverErrors(DtaDeviceData* pDvcData);

// Video-standard helepers
Int  DtaIoStd2VidStd(Int  Value, Int  SubValue);
Int  DtaVidStd2Fps(Int  VidStd);
Bool  DtaVidStdIsFractional(Int  VidStd);
Bool  DtaVidStdIsInterlaced(Int  VidStd);


#endif // __DTA_UTILITY_H
