//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dma.h *#*#*#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
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


#ifndef __DMA_H
#define __DMA_H

#define  DTA_DMA_MAX_TRANSFER_SIZE       (128*1024*1024)  // 128 MB
#define  DTA_DMA_MAX_PLX_DMA_CONTROLLERS  2

// TODO: ASSERTION FOR CHECK:
// Max. length for 1 continuous DMA block = 8MB-4 (23 bits).

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SGL Descriptor -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Scatter-Gather list descriptor 32-bit and 64-bit(FPGA only).
//
typedef struct _SglDesc {
    UInt32  m_32PciAddress;
    UInt32  m_32LocalAddress;
    UInt32  m_32TransferSize;
    UInt32  m_32NextSglDesc;
} SglDesc;

typedef struct _SglDesc64 {
    UInt32  m_32PciAddress;
    UInt32  m_32LocalAddress;
    UInt32  m_32TransferSize;
    UInt32  m_32NextSglDesc;
    UInt32  m_DacPciAddress;
    UInt32  m_Dummy[3];
} SglDesc64;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DMA Options -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DmaOpt {
    // Generic DMA options
    Bool  m_UseDmaInFpga;
    Bool  m_Supports64Bit;

    // PLX DMA options
    Int  m_PlxDmaChannelCount;            // Number of PLX DMA channels available
    Int  m_PlxDmaChannelPort[DTA_DMA_MAX_PLX_DMA_CONTROLLERS];
                                          // This array contains the physical port using
                                          // the specific PLX DMA channel
    UInt32  m_PlxDmaChannelMode[DTA_DMA_MAX_PLX_DMA_CONTROLLERS];
                                          // Holds the PLX DMA mode register value
    UInt32  m_PlxDmaThreshold;            // Holds the PLX DMA threshold register value
} DmaOpt;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DMA Mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#define  DTA_DMA_MODE_DEFAULT               0
#define  DTA_DMA_MODE_SIZE_ENABLE           1   
                                          // Do partly SGL (ringbuffer) transfers
#define  DTA_DMA_MODE_TIMEOUT_ENABLE        2   
                                          // Timeout Rx enable
#define  DTA_DMA_MODE_RING_BUFFER           4   
                                          // Create SGL ring-buffer.

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DMA Flags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#define  DTA_DMA_FLAGS_NONE                 0
#define  DTA_DMA_FLAGS_BLOCKING             1
                                          // Read/Write blocks until DMA is finished.
#define  DTA_DMA_FLAGS_DATA_BUF_NO_COPY     2
                                          // Data buffer is already allocated with the 
                                          // notion of number of map-registers and can
                                          // be directly used for DMA.
                                          // The copy to common buffer is not needed.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DmaCallbackFunc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef void  (*DmaCallbackFunc)(DmaChannel* pDmaChannel, void* pContext);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DMA Channel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
struct _DmaChannel {
    DtaDeviceData*  m_pDvcData;
    UInt  m_PortIndex;

    DtDpc  m_DmaCompletedDpc;
    
    // DMA register access
    UInt  m_RegsOffset;
    volatile UInt8*  m_pRegBase;
    volatile UInt8*  m_pRegCmdStat;
    volatile UInt8*  m_pRegDmaDescLow;
    volatile UInt8*  m_pRegDmaDescHigh;
    volatile UInt8*  m_pRegDmaMode;

    // DMA configuration
    Bool  m_Use64BitDma;
    Bool  m_UseDirectBufDma;    
    Bool  m_FixedLocalAddress;          // Local address is fixed or read from FPGA
    UInt  m_DmaDirection;
    UInt  m_MaxDmaLength;
    UInt  m_DmaMode;
    UInt  m_DmaFlags;
    Int  m_Timeout;                     // in 2^14 clock cycles or -1 if not used
    Bool  m_ReUseDataBuffer;            // true if the databuffer is prepared for DMA and
                                        // and SGL list is not cleaned up after the DMA
                                        // transfer is done.
    
    // DMA status
    Int  m_State;                       // DTA_DMA_STATE_
    DtEvent  m_DmaDoneEvent;
    Bool  m_FinalTransfer;
    UInt  m_TransferNumber;
    UInt  m_CurrentTransferLength;
    UInt  m_NumBytesRead;
    
    // User function arguments
    UInt8*  m_pBuffer;    
    UInt  m_TransferSize;
    UInt  m_TransferOffet;
    volatile UInt8*  m_pLocalAddress;
    UInt  m_LocalAddressStart;
    UInt  m_LocalAddressEnd;
    DmaCallbackFunc  m_pDmaFinishFunc;
    void*  m_pDmaFinishContext;

    // DMA buffers
    DmaDirectBuffer  m_SGListBuf;
    union
    {   
        DmaDirectBuffer  m_DmaData; 
        DmaOsSgl  m_OsSgList;
    }  m_Data;
};



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaDmaCleanupKernelBuffer(DmaChannel* pDmaCh, void* pBuffer, 
                                                         DtPageList* pPageList, UInt Tag);
DtStatus  DtaDmaCreateKernelBuffer(DmaChannel* pDmaCh, UInt DmaDirection, void** ppBuffer,
                                          DtPageList** pPageList, UInt BufSize, UInt Tag);
DtStatus  DtaDmaCleanupDataBuffer(DmaChannel* pDmaCh);
DtStatus  DtaDmaPrepareDataBuffer(DtPageList* pPageList, void* pBuffer, UInt BufSize,
                                        UInt BufType, DmaChannel* pDmaCh, UInt Direction);
DtStatus  DtaDmaGetDmaChannelPlx(DtaDeviceData* pDvcData, Int PlxDmaChannel, 
                                                               DmaChannel** ppDmaChannel);
DtStatus  DtaDmaInit(DtaDeviceData* pDvcData);
DtStatus  DtaDmaInitCh(DtaDeviceData* pDvcData, Int PortIndex, UInt MaxDmaLength,
                                      UInt DmaMode, UInt DmaRegsOffset, UInt  DmaFlags,
                                      Int Timeout, DmaCallbackFunc pDmaFinishFunc, 
                                      void* pDmaFinishContext, DmaChannel* pDmaCh);
void  DtaDmaCleanupCh(DtaDeviceData* pDvcData, DmaChannel* pDmaCh);
DtStatus  DtaDmaInitChPowerup(DmaChannel* pDmaCh);
DtStatus  DtaDmaInitPowerup(DtaDeviceData* pDvcData);
DtStatus  DtaDmaStartHpTransfer(DtaShBuffer* pShBuffer, Int TransferSize,
                                      Int TransferOffset, UInt8* pLocalAddress,
                                      UInt LocalAddressBufStart, UInt LocalAddressBufEnd);
DtStatus  DtaDmaStartTransfer(DmaChannel* pDmaCh, DtPageList* pPageList, Int BufType,
                                     UInt Direction, UInt8* pBuffer, Int TransferSize, 
                                     Int TransferOffset,
                                     UInt8* pLocalAddress, UInt LocalAddressBufStart, 
                                     UInt LocalAddressBufEnd, Bool ReuseDataBuffer,
                                     Int* pNumBytesRead);
DtStatus  DtaDmaStartKernelBufTransfer(DmaChannel* pDmaCh, UInt8* pBuffer, 
                                      Int TransferSize,
                                      Int TransferOffset, UInt8* pLocalAddress, 
                                      UInt LocalAddressBufStart, UInt LocalAddressBufEnd);

DtStatus  DtaDmaFinalTransfer(DmaChannel* pDmaCh);
DtEvent*  DtaDmaGetDoneEvent(DmaChannel* pDmaCh);
UInt  DtaDmaGetBytesReceived(DmaChannel* pDmaCh);
Bool  DtaDmaIsReady(DmaChannel* pDmaCh);
DtStatus  DtaDmaAbortDma(DmaChannel* pDmaCh);
Bool  DtaDmaIsAbortActive(DmaChannel* pDmaCh);
void  DtaDmaReInitCallback(DmaChannel* pDmaCh, DmaCallbackFunc pDmaFinishFunc, 
                                                                           void* pDmaFinishContext);

#endif  //  __DMA_H

