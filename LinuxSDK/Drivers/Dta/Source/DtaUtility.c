//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaUtility.c *#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
//
// Dta driver - DTA utility functions.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2016 DekTec Digital Video B.V.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtaIncludes.h>

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSubsystemId2TypeNumber -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt16  DtaSubsystemId2TypeNumber(Int SubsystemId)
{
    // For the DTA-106, a wrong ID is filled in the Device ID. 
    if (SubsystemId == 0x106)
        return 106;
    return SubsystemId - (SubsystemId < 0xD100 ? 0 : (0xD100-100));
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceId2SubDvcNumber -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaDeviceId2SubDvcNumber(Int TypeNumber, Int  DeviceId)
{
    // Can only deduce type number from device ID if type number is encoded in bottom
    // 12-bits of the DeviceId
    if ((DeviceId&0X0FFF) == TypeNumber)
    {
        // Sub device number is encoded in MSB 4-bits of device ID
        // NOTE: 0=master, 1=slave1, 2=slavee, etc, etc
        return (DeviceId>>12 & 0x000F);
    }   
    return 0;   // assume this is the one and only sub-device
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaWatchdogToggle -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtaWatchdogToggle(volatile UInt8* pBase)
{
    if (DtaRegControl1GetWatchdog(pBase) == 1)
        DtaRegControl1SetWatchdog(pBase, 0);
    else
        DtaRegControl1SetWatchdog(pBase, 1);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaWatchdogPulse -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtaWatchdogPulse(volatile UInt8* pBase)
{
    UInt  Val = DtaRegControl1GetWatchdog(pBase);
    DtaWatchdogToggle(pBase);
    DtaRegControl1SetWatchdog(pBase, Val);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGetPerIntItvUS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// 
// Returns the current periodic interval in micro seconds
//
UInt32 DtaGetPerIntItvUS(DtaDeviceData* pDvcData)
{
    UInt64  Value64;

    DT_ASSERT(pDvcData->m_DevInfo.m_PerIntClkBit >= 17);
    Value64 = 1000000 * ((UInt64)1<<pDvcData->m_DevInfo.m_PerIntClkBit);
    return (UInt32)DtDivide64(Value64, pDvcData->m_DevInfo.m_RefClk, NULL);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ PingPong buffer handling +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Helper functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static void  DtaPPBufferTransferDataCompleted(DmaChannel* pDmaChannel, void* pContext);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferSwap -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtaPPBufferSwap(UInt* pBufferIndex)
{ 
    *pBufferIndex = (*pBufferIndex == DTA_PPBUF_PONG_ID) ? DTA_PPBUF_PING_ID:
                                                                        DTA_PPBUF_PONG_ID;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferIsFull -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Bool  DtaPPBufferIsFull(PPBuffer* pPPBuffer, UInt BufferId)
{
    if (pPPBuffer->m_pDmaChannel->m_DmaDirection == DT_DMA_DIRECTION_TO_DEVICE)
    {
        if (BufferId == DTA_PPBUF_PING_ID)
            return (pPPBuffer->m_BufState & DTA_PPBUF_STAT_PING) != 0;
        return (pPPBuffer->m_BufState & DTA_PPBUF_STAT_PONG) != 0;
    } 
    if (BufferId == DTA_PPBUF_PING_ID)
        return (pPPBuffer->m_BufState & DTA_PPBUF_STAT_PING) == 0;
    return (pPPBuffer->m_BufState & DTA_PPBUF_STAT_PONG) == 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferRWFinished -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtaPPBufferRWFinished(PPBuffer* pPPBuffer)
{
    UInt  OldState = pPPBuffer->m_BufState;
    UInt  AddState;

    if (pPPBuffer->m_CurRwBufferId == DTA_PPBUF_PING_ID)
        AddState = DTA_PPBUF_STAT_PING;
    else 
        AddState = DTA_PPBUF_STAT_PONG;
    
    DT_ASSERT((OldState & AddState) == 0);
    while (OldState != DtAtomicCompareExchange(&pPPBuffer->m_BufState, OldState, 
                                                                      OldState | AddState))
    {
        OldState = pPPBuffer->m_BufState;
        DT_ASSERT((OldState & AddState) == 0);
    }
    DtDbgOut(MAX, PP, "OldState: %xh NewState: %xh", OldState, OldState | AddState);

    DtaPPBufferSwap(&pPPBuffer->m_CurRwBufferId);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferInitInternalStates -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaPPBufferInitInternalStates(PPBuffer* pPPBuffer, UInt DmaDirection)
{
    pPPBuffer->m_CurRwBufferId = DTA_PPBUF_PING_ID;
    pPPBuffer->m_CurDmaBufferId = DTA_PPBUF_PING_ID;
    pPPBuffer->m_BufTransferSize[DTA_PPBUF_PING_ID] = 0;
    pPPBuffer->m_BufTransferSize[DTA_PPBUF_PONG_ID] = 0;

    if (DmaDirection == DT_DMA_DIRECTION_FROM_DEVICE)
        // PING and PONG ready for DMA
        pPPBuffer->m_BufState = DTA_PPBUF_STAT_PING | DTA_PPBUF_STAT_PONG;
    else
        pPPBuffer->m_BufState = 0; // No buffers ready for DMA.    
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferInitialise -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaPPBufferInitialise(
    PPBuffer*  pPPBuffer, 
    UInt8*  pBufferStart,
    DtPageList*  pPageList,
    UInt  BufSize,
    DmaChannel*  pDmaChannel,
    DtaPPBufferGetLocAddrFunc  pGetLocAddrFunc,
    void*  pGetLocAddrContext,
    Bool  AutoTransferAfterComplete)
{
    UInt  SizePerBufferPart;

    DT_ASSERT(BufSize % 2 == 0);
    
    SizePerBufferPart = BufSize/2;

    // Initialize pingpong buffer
    pPPBuffer->m_pBufStart = pBufferStart;
    pPPBuffer->m_pPageList = pPageList;    
    pPPBuffer->m_BufSize[DTA_PPBUF_PING_ID] = SizePerBufferPart;
    pPPBuffer->m_BufSize[DTA_PPBUF_PONG_ID] = SizePerBufferPart;
    pPPBuffer->m_BufOffset[DTA_PPBUF_PING_ID] = 0;
    pPPBuffer->m_BufOffset[DTA_PPBUF_PONG_ID] = SizePerBufferPart;
    pPPBuffer->m_pDmaChannel = pDmaChannel;
    pPPBuffer->m_pGetLocAddrFunc = pGetLocAddrFunc;
    pPPBuffer->m_pGetLocAddrContext = pGetLocAddrContext;
    pPPBuffer->m_AutoTransferAfterComplete = AutoTransferAfterComplete;
    
    // Initialize internal values that may change during processing
    DtaPPBufferInitInternalStates(pPPBuffer, pDmaChannel->m_DmaDirection);

    DtaDmaReInitCallback(pDmaChannel, DtaPPBufferTransferDataCompleted, pPPBuffer);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferWriteDataIsBufAvailable -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This functions returns TRUE if the current ping/pong buffer is available for
// writing data, and not ready for DMA.
//
Bool  DtaPPBufferWriteDataIsBufAvailable(PPBuffer* pPPBuffer)
{
    DT_ASSERT(pPPBuffer->m_pDmaChannel->m_DmaDirection == DT_DMA_DIRECTION_TO_DEVICE);
    return !DtaPPBufferIsFull(pPPBuffer, pPPBuffer->m_CurRwBufferId);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferWriteData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function adds data to the current ping/pong buffer. If the buffer is full,
// an error is returned
//
DtStatus  DtaPPBufferWriteData(
    UInt8*  pSourceBuffer,
    PPBuffer*  pPPBuffer,
    UInt  DataSize)
{
    UInt8*  pDst = NULL;
    UInt  PPBufferIndex = pPPBuffer->m_CurRwBufferId;
    DT_ASSERT(pPPBuffer->m_pDmaChannel->m_DmaDirection == DT_DMA_DIRECTION_TO_DEVICE);
    DT_ASSERT(!DtaPPBufferIsFull(pPPBuffer, PPBufferIndex));
    
    // Check if we do not exceed the size of the available buffer space
    // If so, we have an overflow situation!
    if (pPPBuffer->m_BufTransferSize[PPBufferIndex]+DataSize >
                                                      pPPBuffer->m_BufSize[PPBufferIndex])
        return DT_STATUS_BUFFER_OVERFLOW;

    // Calculate current location of the destiny for ping or pong buffer
    pDst = pPPBuffer->m_pBufStart + pPPBuffer->m_BufOffset[PPBufferIndex] +
                                              pPPBuffer->m_BufTransferSize[PPBufferIndex];

    DtMemCopy(pDst, pSourceBuffer, DataSize);
    pPPBuffer->m_BufTransferSize[PPBufferIndex] += DataSize;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferWriteDataFinish -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is executed after the last WriteData to indicate that the buffer
// is ready to start a DMA transaction
//
void  DtaPPBufferWriteDataFinish(PPBuffer* pPPBuffer)
{
    DtDbgOut(MAX, PP, "Start");

    DT_ASSERT(pPPBuffer->m_pDmaChannel->m_DmaDirection == DT_DMA_DIRECTION_TO_DEVICE);
    DtaPPBufferRWFinished(pPPBuffer);
    
    DtDbgOut(MAX, PP, "Exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferTransferDataStartDma -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaPPBufferTransferDataStartDma(PPBuffer* pPPBuffer, 
                                                                    UInt BufferToTransfer)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  TransferSize;
    UInt  BufStart;
    UInt  BufStop;
    UInt  TransferSizeNew;
    UInt8*  pLocalAddress;

    DtDbgOut(MAX, PP, "Start");

    if (pPPBuffer->m_pDmaChannel->m_DmaDirection == DT_DMA_DIRECTION_TO_DEVICE)
        TransferSize = pPPBuffer->m_BufTransferSize[BufferToTransfer];
    else
        TransferSize = pPPBuffer->m_BufSize[BufferToTransfer];

    // Transfer buffer
    pPPBuffer->m_pGetLocAddrFunc(pPPBuffer->m_pGetLocAddrContext, 
                                   &pLocalAddress, &BufStart, &BufStop, &TransferSizeNew);
    if (TransferSizeNew != 0)
        TransferSize = TransferSizeNew;

    Status = DtaDmaStartKernelBufTransfer(pPPBuffer->m_pDmaChannel, 
                                                 pPPBuffer->m_pBufStart,TransferSize,
                                                 pPPBuffer->m_BufOffset[BufferToTransfer],
                                                 pLocalAddress, BufStart, BufStop);
    DtDbgOut(MAX, PP, "Exit");
    return Status;
}



//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferTransferDataCompleted -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// A DMA transfer has finished.
// Clear the flag of the transferred buffer and reset the DMA busy flag.
// Note: 
//  The order of executed CompareExchanges is important:
//  A BufferReady executed in between CompareExchanges does not have effect 
//  on correct functioning, if this order is maintained.
//
static void  DtaPPBufferTransferDataCompleted(DmaChannel* pDmaChannel, void* pContext)
{   
    PPBuffer* pPPBuffer = (PPBuffer*)pContext;
    UInt  OldState;
    UInt  CurDmaBufferId = pPPBuffer->m_CurDmaBufferId;

    DtDbgOut(MAX, PP, "Start");

    if (pPPBuffer->m_pDmaChannel->m_DmaDirection == DT_DMA_DIRECTION_FROM_DEVICE)
        pPPBuffer->m_BufTransferSize[CurDmaBufferId] = 
                                         DtaDmaGetBytesReceived(pPPBuffer->m_pDmaChannel);

    DtaPPBufferSwap(&pPPBuffer->m_CurDmaBufferId);

    // Clear flags for active PING
    if (CurDmaBufferId == DTA_PPBUF_PING_ID)
    {
        // Clear DMA and PING if only PING and DMA is set and PONG is clear
        OldState = DtAtomicCompareExchange(&pPPBuffer->m_BufState,
                                            DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING,
                                            0);

        // If nothing was changed, clear DMA and PING when PING, PONG and DMA is set
        if (OldState != (DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING))
        {
            if (pPPBuffer->m_AutoTransferAfterComplete)
            {
                // The PONG is also ready for DMA transfer
                DT_ASSERT(OldState == (DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING |
                                                                        DTA_PPBUF_STAT_PONG));
                OldState = DtAtomicCompareExchange(&pPPBuffer->m_BufState,
                                               DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING |
                                               DTA_PPBUF_STAT_PONG,
                                               DTA_PPBUF_STAT_PONG | DTA_PPBUF_STAT_DMA_BUSY);
                DT_ASSERT(OldState == (DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING |
                                                                        DTA_PPBUF_STAT_PONG));
                // Start new transfer...
                DtaPPBufferTransferDataStartDma(pPPBuffer, pPPBuffer->m_CurDmaBufferId);
            } else {
                DT_ASSERT(OldState == (DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING |
                                                                        DTA_PPBUF_STAT_PONG));
                OldState = DtAtomicCompareExchange(&pPPBuffer->m_BufState,
                                               DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING |
                                               DTA_PPBUF_STAT_PONG, DTA_PPBUF_STAT_PONG);
                DT_ASSERT(OldState == (DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING |
                                                                        DTA_PPBUF_STAT_PONG));
                
            }
        }
    } else {
        // Clear DMA and PONG if only PONG and DMA is set and PING is clear
        OldState = DtAtomicCompareExchange(&pPPBuffer->m_BufState,
                                                                 DTA_PPBUF_STAT_DMA_BUSY |
                                                                 DTA_PPBUF_STAT_PONG, 0);

        // If nothing was changed, clear DMA and PONG when PING, PONG and DMA is set
        if (OldState != (DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PONG))
        {
            // The PING is also ready for DMA transfer
            DT_ASSERT(OldState == (DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING |
                                                                    DTA_PPBUF_STAT_PONG));
            if (pPPBuffer->m_AutoTransferAfterComplete)
            {   OldState = DtAtomicCompareExchange(&pPPBuffer->m_BufState,
                                           DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING |
                                           DTA_PPBUF_STAT_PONG, 
                                           DTA_PPBUF_STAT_PING | DTA_PPBUF_STAT_DMA_BUSY);
                DT_ASSERT(OldState == (DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING |
                                                                    DTA_PPBUF_STAT_PONG));
                // Start new transfer
                DtaPPBufferTransferDataStartDma(pPPBuffer, pPPBuffer->m_CurDmaBufferId);
            } else {
                OldState = DtAtomicCompareExchange(&pPPBuffer->m_BufState,
                                           DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING |
                                           DTA_PPBUF_STAT_PONG, 
                                           DTA_PPBUF_STAT_PING);
                DT_ASSERT(OldState == (DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING |
                                                                    DTA_PPBUF_STAT_PONG));
            }
        }
    }
    DtDbgOut(MAX, PP, "OldState: %xh NewState(NotSafe!): %xh", OldState, 
                                                                   pPPBuffer->m_BufState);

    DtDbgOut(MAX, PP, "Exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferTransferData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Start a DMA transfer and set the DMA busy flag
//
// Note: 
//  The order of executed CompareExchanges is important:
//  A BufferReady executed in between CompareExchanges does not have effect 
//  on correct functioning, if this order is maintained.
//  m_CurDmaBufferId may have an old value if the DMA is still active. This is not
//  a problem, because this is atomically checked.
//
DtStatus  DtaPPBufferTransferData(
    PPBuffer*  pPPBuffer)
{
    UInt  OldState;
    DtStatus  Status = DT_STATUS_OK;
    Bool  StartTransfer = FALSE;

    // Get the filled buffer
    UInt  BufferToTransfer;
    
    DtDbgOut(MAX, PP, "Start");

    while (!StartTransfer) 
    {
        BufferToTransfer = pPPBuffer->m_CurDmaBufferId;
        // Set the DMA busy flag
        if (BufferToTransfer == DTA_PPBUF_PING_ID)
        {
            // try to set PING and DMA if PONG buffer is not in use
            OldState = DtAtomicCompareExchange(&pPPBuffer->m_BufState, 
                                           DTA_PPBUF_STAT_PING,
                                           DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PING);
           
           // If nothing has changed, try to set PING and DMA if PONG buffer is in use
            if (OldState == DTA_PPBUF_STAT_PING)
                StartTransfer = TRUE;
            else if (OldState == 0)
            {
                DtDbgOut(MAX, PP, "Nothing to transfer");
                DtDbgOut(MAX, PP, "Exit");
                return DT_STATUS_OK;
            } else {
                OldState = DtAtomicCompareExchange(&pPPBuffer->m_BufState, 
                                               DTA_PPBUF_STAT_PING | DTA_PPBUF_STAT_PONG,
                                               DTA_PPBUF_STAT_DMA_BUSY |
                                               DTA_PPBUF_STAT_PING | DTA_PPBUF_STAT_PONG);
                if (OldState==(DTA_PPBUF_STAT_PING | DTA_PPBUF_STAT_PONG))
                    StartTransfer = TRUE;
                else if ((OldState & DTA_PPBUF_STAT_DMA_BUSY) != 0)
                {
                    // DMA is still in progress
                    DtDbgOut(MAX, PP, "DMA in progress");
                    DtDbgOut(MAX, PP, "Exit");
                    return DT_STATUS_OK;
                }
            }
            if (!StartTransfer)
                DtDbgOut(AVG, PP, "OldState: %xh BufferToTransfer: %i CurDmaBuffer: %i", 
                                 OldState, BufferToTransfer, pPPBuffer->m_CurDmaBufferId);

                    
        } else {
            // try to set PONG and DMA if PING buffer is not in use
            OldState = DtAtomicCompareExchange(&pPPBuffer->m_BufState, DTA_PPBUF_STAT_PONG,
                                                 DTA_PPBUF_STAT_DMA_BUSY|DTA_PPBUF_STAT_PONG);
           
            // If nothing has changed, try to set PONG and DMA if PING buffer is in use
            if (OldState == DTA_PPBUF_STAT_PONG)
                StartTransfer = TRUE;
            else if (OldState == 0)
            {
                DtDbgOut(MAX, PP, "Nothing to transfer");
                DtDbgOut(MAX, PP, "Exit");
                return DT_STATUS_OK;
            } else {
                DtAtomicCompareExchange(&pPPBuffer->m_BufState, 
                                               DTA_PPBUF_STAT_PING | DTA_PPBUF_STAT_PONG,
                                               DTA_PPBUF_STAT_DMA_BUSY | DTA_PPBUF_STAT_PONG |
                                               DTA_PPBUF_STAT_PING);

                if (OldState==(DTA_PPBUF_STAT_PING | DTA_PPBUF_STAT_PONG))
                    StartTransfer = TRUE;
                else if ((OldState & DTA_PPBUF_STAT_DMA_BUSY) !=0 )
                {
                    // DMA is still in progress
                    DtDbgOut(MAX, PP, "DMA in progress");
                    DtDbgOut(MAX, PP, "Exit");
                    return DT_STATUS_OK;
                }
            }
            if (!StartTransfer)
                DtDbgOut(AVG, PP, "OldState: %xh BufferToTransfer: %i CurDmaBuffer: %i", 
                                 OldState, BufferToTransfer, pPPBuffer->m_CurDmaBufferId);

        }
    }

    DtDbgOut(MAX, PP, "OldState: %xh NewState(NotSafe!): %xh", OldState,
                                                                   pPPBuffer->m_BufState);

    Status = DtaPPBufferTransferDataStartDma(pPPBuffer, BufferToTransfer);
    
    DtDbgOut(MAX, PP, "Exit");

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferTransferDataContext -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// See DtaPPBufferTransferData with the addition to set another context variable for
// the GetLocalAddress callback function
//
DtStatus  DtaPPBufferTransferDataContext(
    PPBuffer*  pPPBuffer,
    void*  pGetLocalAddrContext)
{
    pPPBuffer->m_pGetLocAddrContext = pGetLocalAddrContext;
    return DtaPPBufferTransferData(pPPBuffer);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferWriteDataClearBuf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaPPBufferWriteDataClearBuf(PPBuffer* pPPBuffer)
{
    DT_ASSERT(pPPBuffer->m_pDmaChannel->m_DmaDirection == DT_DMA_DIRECTION_TO_DEVICE);
    pPPBuffer->m_BufTransferSize[pPPBuffer->m_CurRwBufferId] = 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferGetTransferSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtaPPBufferGetTransferSize(PPBuffer* pPPBuffer)
{
    return pPPBuffer->m_BufTransferSize[pPPBuffer->m_CurRwBufferId];
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferReadDataGetBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void*  DtaPPBufferReadDataGetBuffer(PPBuffer* pPPBuffer)
{
    DT_ASSERT(pPPBuffer->m_pDmaChannel->m_DmaDirection == DT_DMA_DIRECTION_FROM_DEVICE);
    return (pPPBuffer->m_pBufStart + pPPBuffer->m_BufOffset[pPPBuffer->m_CurRwBufferId]);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPPBufferReadDataFinished -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaPPBufferReadDataFinished(PPBuffer* pPPBuffer)
{
    DtDbgOut(MAX, PP, "Start");
    DT_ASSERT(pPPBuffer->m_pDmaChannel->m_DmaDirection == DT_DMA_DIRECTION_FROM_DEVICE);
    DtaPPBufferRWFinished(pPPBuffer);
    DtDbgOut(MAX, PP, "Exit");
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPropertiesInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaPropertiesInit(DtaDeviceData* pDvcData)
{
    DtString  DtStrType;
    DtStringChar  DtStrTypeBuffer[9];
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    DtEvtLog*  pEvtObject = &pDvcData->m_Device.m_EvtObject;
    DtStatus  Status;
    
    // Connect DtStrTypeBuffer to DtStrType
    DT_STRING_INIT(DtStrType, DtStrTypeBuffer,
                                            sizeof(DtStrTypeBuffer)/sizeof(DtStringChar));

    // Init properties failed?
    Status = DtPropertiesInit(pPropData);
    if (Status != DT_STATUS_OK)
    {
         // Use event to report driver errors 
        DtStringAppendChars(&DtStrType, "DTA-");
        DtStringUIntegerToDtStringAppend(&DtStrType, 10, pPropData->m_TypeNumber);
        DtEvtLogReport(pEvtObject, DTA_LOG_PROP_STORE_NOT_FOUND, &DtStrType, NULL, NULL);
    }
    return Status;
}



//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPropertiesReportDriverErrors -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaPropertiesReportDriverErrors(DtaDeviceData* pDvcData)
 {
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    DtEvtLog*  pEvtObject = &pDvcData->m_Device.m_EvtObject;

    // Errors to report?
    DtStatus  Status = DtPropertiesReportDriverErrors(pPropData);
    if (Status != DT_STATUS_OK)
    {
        // Use event to report driver errors 
        DtEvtLogReport(pEvtObject, DTA_LOG_CONFIG_FAILED,
                                        &pPropData->m_PropertyNotFoundString, NULL, NULL);
    }
    return Status;
 }

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Video Standard helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoStd2VidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtaIoStd2VidStd(Int  Value, Int  SubValue)
{
    switch (SubValue)
    {
    case DT_IOCONFIG_1080P50:       return DT_VIDSTD_1080P50;
    case DT_IOCONFIG_1080P50B:      return DT_VIDSTD_1080P50B;
    case DT_IOCONFIG_1080P59_94:    return DT_VIDSTD_1080P59_94;
    case DT_IOCONFIG_1080P59_94B:   return DT_VIDSTD_1080P59_94B;
    case DT_IOCONFIG_1080P60:       return DT_VIDSTD_1080P60;
    case DT_IOCONFIG_1080P60B:      return DT_VIDSTD_1080P60B;
    case DT_IOCONFIG_1080I50:       return DT_VIDSTD_1080I50;
    case DT_IOCONFIG_1080I59_94:    return DT_VIDSTD_1080I59_94;
    case DT_IOCONFIG_1080I60:       return DT_VIDSTD_1080I60;
    case DT_IOCONFIG_1080P23_98:    return DT_VIDSTD_1080P23_98;
    case DT_IOCONFIG_1080P24:       return DT_VIDSTD_1080P24;
    case DT_IOCONFIG_1080P25:       return DT_VIDSTD_1080P25;
    case DT_IOCONFIG_1080P29_97:    return DT_VIDSTD_1080P29_97;
    case DT_IOCONFIG_1080P30:       return DT_VIDSTD_1080P30;
    case DT_IOCONFIG_1080PSF23_98:  return DT_VIDSTD_1080PSF23_98;
    case DT_IOCONFIG_1080PSF24:     return DT_VIDSTD_1080PSF24;
    case DT_IOCONFIG_1080PSF25:     return DT_VIDSTD_1080PSF25;
    case DT_IOCONFIG_1080PSF29_97:  return DT_VIDSTD_1080PSF29_97;
    case DT_IOCONFIG_1080PSF30:     return DT_VIDSTD_1080PSF30;
    case DT_IOCONFIG_720P23_98:     return DT_VIDSTD_720P23_98;
    case DT_IOCONFIG_720P24:        return DT_VIDSTD_720P24;
    case DT_IOCONFIG_720P25:        return DT_VIDSTD_720P25;
    case DT_IOCONFIG_720P29_97:     return DT_VIDSTD_720P29_97;
    case DT_IOCONFIG_720P30:        return DT_VIDSTD_720P30;
    case DT_IOCONFIG_720P50:        return DT_VIDSTD_720P50;
    case DT_IOCONFIG_720P59_94:     return DT_VIDSTD_720P59_94;
    case DT_IOCONFIG_720P60:        return DT_VIDSTD_720P60;
    case DT_IOCONFIG_525I59_94:     return DT_VIDSTD_525I59_94;
    case DT_IOCONFIG_625I50:        return DT_VIDSTD_625I50;
    }
    return DT_VIDSTD_UNKNOWN;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaVidStd2Fps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaVidStd2Fps(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_720P59_94:
    case DT_VIDSTD_720P60:
    case DT_VIDSTD_1080P59_94:
    case DT_VIDSTD_1080P59_94B:
    case DT_VIDSTD_1080P60:
    case DT_VIDSTD_1080P60B:
    case DT_VIDSTD_480P59_94:
    case DT_VIDSTD_525P59_94:
        return 60;

    case DT_VIDSTD_720P50:
    case DT_VIDSTD_1080P50:
    case DT_VIDSTD_1080P50B:
    case DT_VIDSTD_625P50:
        return 50;

    case DT_VIDSTD_525I59_94:
    case DT_VIDSTD_720P29_97:
    case DT_VIDSTD_720P30:
    case DT_VIDSTD_1080P30:
    case DT_VIDSTD_1080P29_97:
    case DT_VIDSTD_1080PSF30:
    case DT_VIDSTD_1080PSF29_97:
    case DT_VIDSTD_1080I59_94:
    case DT_VIDSTD_1080I60: 
        return 30;

    case DT_VIDSTD_625I50:
    case DT_VIDSTD_720P25:
    case DT_VIDSTD_1080P25:
    case DT_VIDSTD_1080PSF25:
    case DT_VIDSTD_1080I50:
        return 25;
    
    case DT_VIDSTD_720P23_98:
    case DT_VIDSTD_720P24:
    case DT_VIDSTD_1080P24:
    case DT_VIDSTD_1080P23_98:
    case DT_VIDSTD_1080PSF24:
    case DT_VIDSTD_1080PSF23_98:
        return 24;

   default:
        break;
    }
    return -1;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaVidStdIsFractional -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaVidStdIsFractional(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_720P59_94:
    case DT_VIDSTD_1080P59_94:
    case DT_VIDSTD_1080P59_94B:
    case DT_VIDSTD_525I59_94:
    case DT_VIDSTD_720P29_97:
    case DT_VIDSTD_1080P29_97:
    case DT_VIDSTD_1080PSF29_97:
    case DT_VIDSTD_1080I59_94:
    case DT_VIDSTD_720P23_98:
    case DT_VIDSTD_1080P23_98:
    case DT_VIDSTD_1080PSF23_98:
    case DT_VIDSTD_480P59_94:
    case DT_VIDSTD_525P59_94:
        return TRUE;
    }
    return FALSE;

}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaVidStdIsInterlaced -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaVidStdIsInterlaced(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_525I59_94:
    case DT_VIDSTD_625I50:
    case DT_VIDSTD_1080I50:
    case DT_VIDSTD_1080I59_94:
    case DT_VIDSTD_1080I60:
    case DT_VIDSTD_1080PSF23_98:
    case DT_VIDSTD_1080PSF24:
    case DT_VIDSTD_1080PSF25:
    case DT_VIDSTD_1080PSF29_97:
    case DT_VIDSTD_1080PSF30:
        return TRUE;
    }
    return FALSE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaVidStdIsPsf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaVidStdIsPsf(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_1080PSF30:
    case DT_VIDSTD_1080PSF29_97:
    case DT_VIDSTD_1080PSF25:
    case DT_VIDSTD_1080PSF24:
    case DT_VIDSTD_1080PSF23_98:
        return TRUE;
    }
    return FALSE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaVidStdIs3gSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaVidStdIs3gSdi(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_1080P50:
    case DT_VIDSTD_1080P50B:
    case DT_VIDSTD_1080P59_94:
    case DT_VIDSTD_1080P59_94B:
    case DT_VIDSTD_1080P60:
    case DT_VIDSTD_1080P60B:
        return TRUE;
    }
    return FALSE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaVidStdIs3glvlBSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaVidStdIs3glvlBSdi(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_1080P50B:
    case DT_VIDSTD_1080P59_94B:
    case DT_VIDSTD_1080P60B:
        return TRUE;
    }
    return FALSE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaVidStdIsHdSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaVidStdIsHdSdi(Int  VidStd)
{
    // Not 3G-SDI and also not SD-SDI, than it must be HD-SDI
    return (!DtaVidStdIs3gSdi(VidStd) && !DtaVidStdIsSdSdi(VidStd));
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaVidStdIsSdSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaVidStdIsSdSdi(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_525I59_94:
    case DT_VIDSTD_625I50:
        return TRUE;
    }
    return FALSE;
}

