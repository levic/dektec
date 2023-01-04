// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPipe.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2021 DekTec
//
#include "DtPipe.h"
#include "EthPrtcls.h"

#define PIPE_TAG  0x45504950   // 'EPIP'



#define DT_PIPE_CAP_TX_NRT    (DT_PIPE_CAP_TX|DT_PIPE_CAP_HWP|DT_PIPE_CAP_NRT|DT_PIPE_CAP_KBUF)
#define DT_PIPE_CAP_TX_RT_HWP (DT_PIPE_CAP_TX|DT_PIPE_CAP_HWP|DT_PIPE_CAP_RT)
#define DT_PIPE_CAP_TX_RT_SWP (DT_PIPE_CAP_TX|DT_PIPE_CAP_SWP|DT_PIPE_CAP_RT)
#define DT_PIPE_CAP_TX_RT_HWQ (DT_PIPE_CAP_TX|DT_PIPE_CAP_HWP|DT_PIPE_CAP_KBUF)

#define DT_PIPE_CAP_RX_NRT    (DT_PIPE_CAP_RX|DT_PIPE_CAP_SWP|DT_PIPE_CAP_NRT|DT_PIPE_CAP_KBUF)
#define DT_PIPE_CAP_RX_RT_HWP (DT_PIPE_CAP_RX|DT_PIPE_CAP_HWP|DT_PIPE_CAP_RT)
#define DT_PIPE_CAP_RX_RT_SWP (DT_PIPE_CAP_RX|DT_PIPE_CAP_SWP|DT_PIPE_CAP_RT)
#define DT_PIPE_CAP_RX_HWQ    (DT_PIPE_CAP_RX|DT_PIPE_CAP_HWP|DT_PIPE_CAP_KBUF)

#define IsHwp(Pipe) ((Pipe->m_Capabilities&DT_PIPE_CAP_HWP)!=0)
#define IsHwpKbuf(Pipe) ((Pipe->m_Capabilities&DT_PIPE_CAP_HWP)!=0 && \
                                               (Pipe->m_Capabilities&DT_PIPE_CAP_KBUF)!=0)
#define IsSwp(Pipe) ((Pipe->m_Capabilities&DT_PIPE_CAP_SWP)!=0)
#define IsRx(Pipe) ((Pipe->m_Capabilities&DT_PIPE_CAP_RX)!=0)
#define IsTx(Pipe) ((Pipe->m_Capabilities&DT_PIPE_CAP_TX)!=0)



#define DtDbgOutPipe(Level, pPipe, Msg, ...)                                             \
                DtDbgOut(Level, PIPE, "[%s:Id%i] " Msg,                                  \
                             PipeType2Str(pPipe->m_PipeType), pPipe->m_Id, ##__VA_ARGS__)

#define DT_PAGE_SIZE 4096

const char* PipeType2Str(DtPipeType Type)
{
    switch (Type)
    {
    case DT_PIPE_RX_HWQ: return "RX_HWQ"; break;
    case DT_PIPE_RX_NRT: return "RX_NRT"; break;
    case DT_PIPE_RX_RT_HWP: return "RX_RT_HWP"; break;
    case DT_PIPE_RX_RT_SWP: return "RX_RT_SWP"; break;
    case DT_PIPE_TX_NRT: return "TX_NRT"; break;
    case DT_PIPE_TX_RT_HWP: return "TX_RT_HWP"; break;
    case DT_PIPE_TX_RT_HWQ: return "TX_RT_HWQ"; break;
    case DT_PIPE_TX_RT_SWP: return "TX_RT_SWP"; break;
    default:
        return"UNKNW";
    }
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtPipe* DtPipe_Open(const DtFileObject* pFileObject, DtObject* pPipeOwner, 
                                                   DtPipeType PipeType, Int Id, Int Index)
{
    DtPipe* pPipe = NULL;
    Int Size = sizeof(DtPipe);
    Int Cap = 0;
    switch (PipeType)
    {
    case DT_PIPE_RX_NRT: Size = sizeof(DtPipeRxNrt); Cap = DT_PIPE_CAP_RX_NRT; break;
    case DT_PIPE_TX_NRT: Size = sizeof(DtPipeTxNrt); Cap = DT_PIPE_CAP_TX_NRT; break;
    case DT_PIPE_RX_HWQ: Size = sizeof(DtPipeRxHwq); Cap = DT_PIPE_CAP_RX_HWQ; break;
    case DT_PIPE_RX_RT_HWP: Size = sizeof(DtPipeRxRtHwp); Cap = DT_PIPE_CAP_RX_RT_HWP; break;
    case DT_PIPE_RX_RT_SWP: Size = sizeof(DtPipeRxRtSwp); Cap = DT_PIPE_CAP_RX_RT_SWP; break;
    case DT_PIPE_TX_RT_HWP: Size = sizeof(DtPipeTxRtHwp); Cap = DT_PIPE_CAP_TX_RT_HWP; break;
    case DT_PIPE_TX_RT_SWP: Size = sizeof(DtPipeTxRtSwp); Cap = DT_PIPE_CAP_TX_RT_SWP; break;
    case DT_PIPE_TX_RT_HWQ: Size = sizeof(DtPipeTxRtHwq); Cap = DT_PIPE_CAP_TX_RT_HWQ; break;
    default: DT_ASSERT(FALSE);
    }
    pPipe = (DtPipe*)DtObject_Open(DT_OBJECT_TYPE_PIPE, Size, PIPE_TAG);
    if (pFileObject != NULL)
        pPipe->m_FileObject = *pFileObject;
    pPipe->m_PipeType = PipeType;
    pPipe->m_Id = Id;
    pPipe->m_Index = Index;
    pPipe->m_pPipeOwner = pPipeOwner;
    pPipe->m_InUse = FALSE;
    pPipe->m_Capabilities = Cap;
    
    DtFastMutexInit(&pPipe->m_FastMutex);
    return pPipe;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtPipe_Close(DtPipe* pPipe)
{
    DtPipe_ReleaseResources(pPipe);
    DtObject_Close((DtObject*)pPipe, PIPE_TAG);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_ReleaseResources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtPipe_ReleaseResources(DtPipe* pPipe)
{
    if (pPipe->m_OpMode != DT_PIPE_OPMODE_IDLE)
        DtPipe_SetOperationalMode(pPipe, DT_PIPE_OPMODE_IDLE);
    if (pPipe->m_TrigEvt.m_Initialized)
        DtPipe_DeregisterEvent(pPipe, NULL);
    if (pPipe->m_SharedBufferSet)
        DtPipe_ReleaseSharedBuffer(pPipe);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_SetSharedBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPipe_SetSharedBuffer(DtPipe* pPipe, UInt8* pBuffer, Int BufferSize)
{
    DtStatus Status = DT_STATUS_OK;
    if ((pPipe->m_Capabilities & DT_PIPE_CAP_KBUF) != 0)
        return DT_STATUS_NOT_SUPPORTED;
    if (pPipe->m_SharedBufferSet)
        return DT_STATUS_IN_USE;
    DtDbgOutPipe(AVG, pPipe, "DtPipe_SetSharedBuffer Bufsize:%i", BufferSize);
    if (IsHwp(pPipe))
    {
        DtPipeHwp* pHwp = (DtPipeHwp*)pPipe;
        Int Direction = (IsRx(pPipe) ? DT_CDMAC_DIR_RX : DT_CDMAC_DIR_TX);

        Status = DtBcCDMACTO_AllocateBuffer(pHwp->m_pBcCdma, Direction, pBuffer, 
                                                       BufferSize, DT_BUFTYPE_USER, NULL);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPipe(ERR, pPipe, "ERROR: DtBcCDMACTO_AllocateBuffer. Status: %08xh",
                                                                                  Status);
            return Status;
        }
        // Disable test mode if enabled by someone
        DtBcCDMACTO_SetTestMode(pHwp->m_pBcCdma, DT_CDMAC_TESTMODE_NORMAL);
    }
    else
    {
        DtPipeSwp* pSwp = (DtPipeSwp*)pPipe;
        // Create a pagelist for the shared buffer
        Status = DtCreatePageList(pBuffer, BufferSize, DT_BUFTYPE_USER, 
                                                                      &pSwp->m_PageListU);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPipe(ERR, pPipe, "ERROR: DtCreatePageList. Status: %08xh", Status);
            return Status;
        }
        // Lock buffer into kernel memory
        Status = DtLockUserBuffer(&pSwp->m_PageListU, pBuffer);

        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPipe(ERR, pPipe, "ERROR: DtLockUserBuffer. Status: %08xh", Status);
            DtDeletePageList(&pSwp->m_PageListU);
            return Status;
        }
        pSwp->m_BufSize = BufferSize;
        pSwp->m_pBuffer = pSwp->m_PageListU.m_pVirtualKernel;
    }
    pPipe->m_SharedBufferSet = TRUE;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_ReleaseSharedBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPipe_ReleaseSharedBuffer(DtPipe* pPipe)
{
    DtStatus Status = DT_STATUS_OK;

    if (!pPipe->m_SharedBufferSet)
        return DT_STATUS_OK;
    DtDbgOutPipe(AVG, pPipe, "DtPipe_ReleaseSharedBuffer");

    // .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Kernel buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    if ((pPipe->m_Capabilities & DT_PIPE_CAP_KBUF) != 0)
    {
        if (IsHwp(pPipe))
        {
            DtPipeHwpKbuf* pHwp = (DtPipeHwpKbuf*)pPipe;
            Status = DtBcCDMACTO_FreeBuffer(pHwp->m_pBcCdma, DT_BUFTYPE_KERNEL);
            if (!DT_SUCCESS(Status))
                DtDbgOutPipe(ERR, pPipe, "ERROR: DtBcCDMACTO_FreeBuffer. Status: %08xh", 
                                                                                  Status);
            DtMemFreePoolLarge(pHwp->m_pBuffer, PIPE_TAG, pHwp->m_pPageListK);
        }
        else 
        {
            DtPipeSwpKbuf* pSwp = (DtPipeSwpKbuf*)pPipe;
            DtMemFreePoolLarge(pSwp->m_pBuffer, PIPE_TAG, pSwp->m_pPageListK);
        }
    }
    // -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- User buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    else 
    {
        if (IsHwp(pPipe))
        {
            DtPipeHwp* pHwp = (DtPipeHwp*)pPipe;
            Status = DtBcCDMACTO_FreeBuffer(pHwp->m_pBcCdma, DT_BUFTYPE_USER);
            if (!DT_SUCCESS(Status))
                DtDbgOutPipe(ERR, pPipe, "ERROR: DtBcCDMACTO_FreeBuffer. Status: %08xh",
                                                                                  Status);
        } else
        {
            DtPipeSwp* pSwp = (DtPipeSwp*)pPipe;
            DtUnlockUserBuffer(&pSwp->m_PageListU);
            DtDeletePageList(&pSwp->m_PageListU);
        }
    }
    pPipe->m_SharedBufferSet = FALSE;
    return Status;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_SetSharedBufferKernel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPipe_SetSharedBufferKernel(DtPipe* pPipe, Int RequestSize, Int* pActualSize,
                                                          Int* pMaxLoad, UInt8** ppBuffer)
{
    DtStatus Status = DT_STATUS_OK;
    DtPageList* pPageList = NULL;
    UInt32 Capabilities;
    Int PrefetchSize;
    Int ItfDataWidth;
    Int ActualSize;
    Int PipeType;
    UInt8* pTmpBuffer;

    if (ppBuffer != NULL)
        *ppBuffer = NULL;
    if ((pPipe->m_Capabilities & DT_PIPE_CAP_KBUF) == 0)
        return DT_STATUS_NOT_SUPPORTED;
    
    if (pPipe->m_SharedBufferSet)
        return DT_STATUS_IN_USE;

    Status = DtPipe_GetProperties(pPipe, &Capabilities, &PrefetchSize, &ItfDataWidth, 
                                                                               &PipeType);
    if (Status != DT_STATUS_OK)
        return Status;

    // Calculate buffersize aligned on PAGE_SIZE and multiple of prefetch size
    ActualSize = ((RequestSize + DT_PAGE_SIZE * PrefetchSize - 1) /
                             (DT_PAGE_SIZE * PrefetchSize)) * DT_PAGE_SIZE * PrefetchSize;
    
    if (pActualSize != NULL)
        *pActualSize = ActualSize;
    
    // One data-word is reserved for buffer-full detection
    if (pMaxLoad != NULL)
        *pMaxLoad = ActualSize - (ItfDataWidth / 8);

    // Note: *ppPageList will be set to NULL if we do not need large memory
    pTmpBuffer = (UInt8*)DtMemAllocPoolLarge(DtPoolNonPaged, ActualSize, PIPE_TAG,
                                                                              &pPageList);
    if (pTmpBuffer == NULL)
    {
        DtDbgOutPipe(ERR, pPipe, "ERROR: Out of memory");
        return DT_STATUS_OUT_OF_MEMORY;
    }

    if (IsHwp(pPipe))
    {
        DtPipeHwpKbuf* pHwp = (DtPipeHwpKbuf*)pPipe;
        Int Direction = (IsRx(pPipe) ? DT_CDMAC_DIR_RX : DT_CDMAC_DIR_TX);
        Status = DtBcCDMACTO_AllocateBuffer(pHwp->m_pBcCdma, Direction, pTmpBuffer,
                                                ActualSize, DT_BUFTYPE_KERNEL, pPageList);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPipe(ERR, pPipe, "ERROR: DtBcCDMACTO_AllocateBuffer. Status: %08xh",
                                                                                  Status);
            DtMemFreePoolLarge(pTmpBuffer, PIPE_TAG, pPageList);
            return Status;
        }
        pHwp->m_pPageListK = pPageList;
        pHwp->m_pBuffer = pTmpBuffer;
        pHwp->m_BufSize = ActualSize;
        pHwp->m_MaxLoad = ActualSize - (ItfDataWidth / 8);
    }
    else
    {
        DtPipeSwpKbuf* pSwp = (DtPipeSwpKbuf*)pPipe;
        pSwp->m_pPageListK = pPageList;
        pSwp->m_pBuffer = pTmpBuffer;
        pSwp->m_BufSize = ActualSize;
        pSwp->m_MaxLoad = ActualSize - (ItfDataWidth / 8);
    }
    if (ppBuffer != NULL)
        *ppBuffer = pTmpBuffer;

    pPipe->m_SharedBufferSet = TRUE;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPipe_GetProperties(DtPipe* pPipe, UInt32* pCapabilities, Int* pPrefetchSize,
                                                      Int* pPipeDataWidth, Int* pPipeType)
{
    *pCapabilities = pPipe->m_Capabilities;
    *pPipeType = pPipe->m_PipeType;
    if (IsHwp(pPipe))
    {
        DtPipeHwp* pHwPipe = (DtPipeHwp*)pPipe;
        Int ReorderBufSize;
        UInt32 Capabilities;
        return DtBcCDMACTO_GetProperties(pHwPipe->m_pBcCdma, &Capabilities,
                                           pPrefetchSize, pPipeDataWidth, &ReorderBufSize);
    }
    else
    {
        //DtPipeSwp* pSwPipe = (DtPipeSwp*)pPipe;
        *pPrefetchSize = 1;
        *pPipeDataWidth = 64;    // minimal sizeof(DtEthIpHeader)*8
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPipe_GetStatus(DtPipe* pPipe, Int* pOpStatus, UInt32* pStatusFlags,
                                                                      UInt32* pErrorFlags)
{
    DtStatus Status = DT_STATUS_OK;
    if (IsHwp(pPipe))
    {
        DtPipeHwp* pHwPipe = (DtPipeHwp*)pPipe;
        Status = DtBcCDMACTO_GetStatus(pHwPipe->m_pBcCdma, pOpStatus, pStatusFlags,
                                                                             pErrorFlags);
        *pErrorFlags |= pHwPipe->m_ErrorFlag;
        if (pPipe->m_Capabilities == DT_PIPE_CAP_TX_RT_HWP)
        {
            // Add scheduler status
            DtPipeTxRtHwp* pTxRtHwp = (DtPipeTxRtHwp*)pPipe;
            Bool InvalidTime, Waiting, Ready, Sending;
            DtBcIPSCHED_GetOutputStatus(pTxRtHwp->m_pIpSched, pPipe->m_Index + 1,
                                               &Waiting, &Ready,  &Sending, &InvalidTime);
            if (InvalidTime)
                *pErrorFlags |= DT_PIPE_ERROR_INVALID_TIME;
            if (Waiting)
                *pStatusFlags |= DT_PIPE_STATUS_PACKET_WAITING;
            if (Ready)
                *pStatusFlags |= DT_PIPE_STATUS_PACKET_READY;
            if (Sending)
                *pStatusFlags |= DT_PIPE_STATUS_PACKET_SENDING;
        }
        return Status;
    } else
    {
        DtPipeSwp* pSwPipe = (DtPipeSwp*)pPipe;
        // TODO
        *pStatusFlags = 0;
        *pErrorFlags = pSwPipe->m_ErrorFlag;

        switch (pSwPipe->m_OpMode)
        {
        case DT_BLOCK_OPMODE_IDLE: *pOpStatus = DT_BLOCK_OPSTATUS_IDLE; break;
        case DT_BLOCK_OPMODE_STANDBY: *pOpStatus = DT_BLOCK_OPSTATUS_STANDBY; break;
        case DT_BLOCK_OPMODE_RUN: *pOpStatus = DT_BLOCK_OPSTATUS_RUN; break;
        default: *pOpStatus = DT_BLOCK_OPSTATUS_ERROR; break;
        }
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_IssuePipeFlush -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPipe_IssuePipeFlush(DtPipe* pPipe)
{
    DtStatus Status = DT_STATUS_OK;
    DtDbgOutPipe(AVG, pPipe, "DtPipe_IssuePipeFlush");
    if (IsHwp(pPipe))
    {
        DtPipeHwp* pHwPipe = (DtPipeHwp*)pPipe;
        Status = DtBcCDMACTO_IssueChannelFlush(pHwPipe->m_pBcCdma);
        pHwPipe->m_OffsetCache = 0;
        if (IsRx(pPipe))
        {
            UInt WriteOffset;
            DtBcCDMACTO_GetRxWriteOffset(pHwPipe->m_pBcCdma, &WriteOffset);
            DtBcCDMACTO_SetRxReadOffset(pHwPipe->m_pBcCdma, WriteOffset);
            pHwPipe->m_OffsetCache = WriteOffset;
        }
        else {
            UInt ReadOffset;
            DtBcCDMACTO_GetTxReadOffset(pHwPipe->m_pBcCdma, &ReadOffset);
            DtBcCDMACTO_SetTxWriteOffset(pHwPipe->m_pBcCdma, ReadOffset);
            pHwPipe->m_OffsetCache = ReadOffset;
            if (pPipe->m_Capabilities == DT_PIPE_CAP_TX_RT_HWP)
            {
                DtPipeTxRtHwp* pTxRtHwp = (DtPipeTxRtHwp*)pPipe;
                // Flush IpSchedular output. This will reset the invalid time error too
                // if active
                DtBcIPSCHED_Flush(pTxRtHwp->m_pIpSched, pPipe->m_Index+1);
            }
        }
    }
    else
    {
        DtPipeSwp* pSwPipe = (DtPipeSwp*)pPipe;
        pSwPipe->m_ReadOffset = 0;
        pSwPipe->m_WriteOffset = 0;
        // Reset invalid time error flag
        pSwPipe->m_ErrorFlag &= ~(DT_PIPE_ERROR_INVALID_TIME);
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPipe_SetOperationalMode(DtPipe* pPipe, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;
    DtDbgOutPipe(AVG, pPipe, "DtPipe_SetOperationalMode. OpMode:%i", OpMode);
    if (IsHwp(pPipe))
    {
        DtPipeHwp* pHwPipe = (DtPipeHwp*)pPipe;
        Status = DtBcCDMACTO_SetOperationalMode(pHwPipe->m_pBcCdma, OpMode);
        if (Status != DT_STATUS_OK)
            return Status;

        // Configure the burst FIFO in the TX HWQ or TX HWP
        if (pPipe->m_PipeType == DT_PIPE_TX_RT_HWQ)
        {
            DtPipeTxRtHwq* pHwq = (DtPipeTxRtHwq*)pPipe;
            if (pHwq->m_pBurstFifo != NULL)
            {
                Status = DtBcBURSTFIFO_SetOperationalMode(pHwq->m_pBurstFifo, OpMode);
                if (Status != DT_STATUS_OK)
                    return Status;
            }
        }
        else if (pPipe->m_PipeType == DT_PIPE_TX_RT_HWP)
        {
            DtPipeTxRtHwp* pHwq = (DtPipeTxRtHwp*)pPipe;
            if (pHwq->m_pBurstFifo != NULL)
            {
                Status = DtBcBURSTFIFO_SetOperationalMode(pHwq->m_pBurstFifo, OpMode);
                if (Status != DT_STATUS_OK)
                    return Status;
            }
            if (OpMode == DT_PIPE_OPMODE_RUN && pPipe->m_OpMode == DT_PIPE_OPMODE_STANDBY)
            {
                // Update WritePointer. We are ready to go
                Status = DtBcCDMACTO_SetTxWriteOffset(pHwPipe->m_pBcCdma, 
                                                                  pHwPipe->m_OffsetCache);
                if (Status != DT_STATUS_OK)
                    return Status;
            }
        }

        // It's not allowed to set the OpMode of an IpFifo to IDLE, if it has been
        // started. Otherwise a deadlock can occur in firmware with the memory controller
        if (pHwPipe->m_pDfIpFifo!=NULL && OpMode!=DT_PIPE_OPMODE_IDLE)
        {
            Status = DtDfIpFifo_SetOperationalMode(pHwPipe->m_pDfIpFifo, 
                                                                      DT_FUNC_OPMODE_RUN);
            if (Status != DT_STATUS_OK)
                return Status;
        }
        if (pPipe->m_PipeType == DT_PIPE_RX_HWQ)
        {
            DtPipeRxHwq* pHwq = (DtPipeRxHwq*)pPipe;
            if (pHwq->m_pDdrFifo != NULL)
            {
                Status = DtBcDDRFIFO_SetOperationalMode(pHwq->m_pDdrFifo, OpMode);
                if (Status != DT_STATUS_OK)
                    return Status;
            }
            if (pHwq->m_pEthIpPad != NULL)
            {
                Status = DtBcETHIPPAD_SetOperationalMode(pHwq->m_pEthIpPad, OpMode);
                if (Status != DT_STATUS_OK)
                    return Status;
            }
        }
    }
    pPipe->m_OpMode = OpMode;
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPipe_GetOperationalMode(DtPipe* pPipe, Int* pOpMode)
{
    *pOpMode = pPipe->m_OpMode;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_SetRxReadOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPipe_SetRxReadOffset(DtPipe* pPipe, UInt RxReadOffset)
{
    DtStatus Status = DT_STATUS_OK;
    if (!IsRx(pPipe))
        return DT_STATUS_NOT_SUPPORTED;
    if (IsHwp(pPipe))
    {
        DtPipeHwp* pHwPipe = (DtPipeHwp*)pPipe;
        Status = DtBcCDMACTO_SetRxReadOffset(pHwPipe->m_pBcCdma, RxReadOffset);
        if (DT_SUCCESS(Status))
            pHwPipe->m_OffsetCache = RxReadOffset;
    }
    else
    {
        DtPipeSwp* pSwPipe = (DtPipeSwp*)pPipe;
        pSwPipe->m_ReadOffset = RxReadOffset;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_GetRxWriteOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPipe_GetRxWriteOffset(DtPipe* pPipe, UInt* pRxWriteOffset)
{
    if (!IsRx(pPipe))
        return DT_STATUS_NOT_SUPPORTED;
    if (IsHwp(pPipe))
    {
        DtPipeHwp* pHwPipe = (DtPipeHwp*)pPipe;
        return DtBcCDMACTO_GetRxWriteOffset(pHwPipe->m_pBcCdma, pRxWriteOffset);
    }
    else
    {
        DtPipeSwp* pSwPipe = (DtPipeSwp*)pPipe;
        *pRxWriteOffset = pSwPipe->m_WriteOffset;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_SetTxWriteOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPipe_SetTxWriteOffset(DtPipe* pPipe, UInt TxWriteOffset)
{
    DtStatus Status = DT_STATUS_OK;
    if (!IsTx(pPipe))
        return DT_STATUS_NOT_SUPPORTED;
    if (IsHwp(pPipe))
    {
        DtPipeHwp* pHwPipe = (DtPipeHwp*)pPipe;
        // In standby mode, we don't update the write-pointer. We only fill the
        // shared buffer memory. We update the write pointer if 
        // we go to run-mode
        if (pHwPipe->m_OpMode != DT_PIPE_OPMODE_STANDBY)
            Status = DtBcCDMACTO_SetTxWriteOffset(pHwPipe->m_pBcCdma, TxWriteOffset);
        if (DT_SUCCESS(Status))
            pHwPipe->m_OffsetCache = TxWriteOffset;
    }
    else
    {
        DtPipeSwp* pSwPipe = (DtPipeSwp*)pPipe;
        pSwPipe->m_WriteOffset = TxWriteOffset;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_GetTxReadOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPipe_GetTxReadOffset(DtPipe* pPipe, UInt* pTxReadOffset)
{
    if (!IsTx(pPipe))
        return DT_STATUS_NOT_SUPPORTED;

    if (IsHwp(pPipe))
    {
        DtPipeHwp* pHwPipe = (DtPipeHwp*)pPipe;
        return DtBcCDMACTO_GetTxReadOffset(pHwPipe->m_pBcCdma, pTxReadOffset);
    }
    else
    {
        DtPipeSwp* pSwPipe = (DtPipeSwp*)pPipe;
        *pTxReadOffset = pSwPipe->m_ReadOffset;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_RegisterEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPipe_RegisterEvent(DtPipe* pPipe, DtTrigEvtDriver* pTrigEvtDriver, UInt Count)
{
    DtDbgOutPipe(AVG, pPipe, "Id:%i", pTrigEvtDriver->m_Id);

    if (pPipe->m_TrigEvt.m_Initialized)
        return DT_STATUS_IN_USE;
    if (pTrigEvtDriver->m_Id == DT_PIPE_EVENT_ID_AUTO)
    {
        if (IsHwp(pPipe))
            pTrigEvtDriver->m_Id = DT_PIPE_EVENT_ID_PERINT;
        else
            pTrigEvtDriver->m_Id = DT_PIPE_EVENT_ID_DATA_AVAIL_INT;
    }
    if (pTrigEvtDriver->m_Id!=DT_PIPE_EVENT_ID_PERINT &&
                                  pTrigEvtDriver->m_Id!=DT_PIPE_EVENT_ID_DATA_AVAIL_CNT &&
                                    pTrigEvtDriver->m_Id!=DT_PIPE_EVENT_ID_DATA_AVAIL_INT)
        return DT_STATUS_INVALID_PARAMETER;
    if (IsHwp(pPipe) && pTrigEvtDriver->m_Id!=DT_PIPE_EVENT_ID_PERINT)
        return DT_STATUS_INVALID_PARAMETER;
    if (pTrigEvtDriver->m_Id == DT_PIPE_EVENT_ID_DATA_AVAIL_CNT && Count == 0)
        return DT_STATUS_INVALID_PARAMETER;
    if (pTrigEvtDriver->m_Id == DT_PIPE_EVENT_ID_DATA_AVAIL_CNT)
    {
        DtPipeSwp* pSwPipe = (DtPipeSwp*)pPipe;
        DT_ASSERT(IsSwp(pPipe));
        pSwPipe->m_TrigEvtMaxCnt = Count;
    }
    return DtTrigEvtInit(&pPipe->m_TrigEvt, pTrigEvtDriver);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_DeregisterEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// pTrigEvtDriver can be NULL to close all open events
//
DtStatus DtPipe_DeregisterEvent(DtPipe* pPipe, DtTrigEvtDriver* pTrigEvtDriver)
{
    DtDbgOutPipe(AVG, pPipe, "Id:%i", (pTrigEvtDriver!=NULL?pTrigEvtDriver->m_Id:-1));

    if (pTrigEvtDriver != NULL)
    {
        // Check if event is identical
        if (pPipe->m_TrigEvt.m_Initialized && pPipe->m_TrigEvt.m_Id!=pTrigEvtDriver->m_Id)
        {
            DtDbgOutPipe(ERR, pPipe, "Id mismatch");
            return DT_STATUS_INVALID_PARAMETER;
        }
    }
    return DtTrigEvtClose(&pPipe->m_TrigEvt);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_TriggerEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPipe_TriggerEvent(DtPipe* pPipe)
{
    /*if ((pPipe->m_Capabilities & DT_PIPE_CAP_KBUF) == 0)
    {
        DtTodTime Time;
        UInt64  TimeNs; 
        
        DtCore_TOD_GetTime(pPipe->m_pDtDfNw->m_pCore, &Time);
        TimeNs = Time.m_Seconds * 1000 * 1000 * 1000LL + Time.m_Nanoseconds;
        DtDbgOutPipe(ERR, pPipe, "Id:%i Time:%lluns Delay:%llums", pPipe->m_TrigEvt.m_Id, 
                                                 TimeNs, (TimeNs - pPipe->m_TimeNs)/1000);
        pPipe->m_TimeNs = TimeNs;
    }*/
    return DtTrigEvtSet(&pPipe->m_TrigEvt);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_SetIpFilterDuplicate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPipe_SetIpFilterDuplicate(DtPipe* pPipe, Bool Duplicate)
{
    DtPipeRxRtHwp* pPipeRtHwp = (DtPipeRxRtHwp*)pPipe;
    if (!IsRx(pPipe) || !IsHwp(pPipe))
        return DT_STATUS_NOT_SUPPORTED;
    DtDbgOutPipe(AVG, pPipe, "DtPipe_SetIpFilterDuplicate. Duplicate:%i", 
                                                                     (Duplicate ? 1 : 0));
    return DtBcIPROUT_SetDuplicate(pPipeRtHwp->m_pIpRouter, Duplicate);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_SetIpFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPipe_SetIpFilter(DtPipe* pPipe, UInt8 DstIp[16], UInt16 DstPort[3], 
                             UInt8 SrcIp[16], UInt16 SrcPort[3], Int VlanId[2], Int Flags)
{
    DtStatus Status = DT_STATUS_OK;
    DtDbgOutPipe(AVG, pPipe, "DtPipe_SetIpFilter. Flags:%xh", Flags);
    if (!IsRx(pPipe))
        return DT_STATUS_NOT_SUPPORTED;
    if (IsHwp(pPipe))
    {
        DtPipeRxRtHwp* pPipeRtHwp = (DtPipeRxRtHwp*)pPipe;
        // Flags should be identical. No conversion needed.
        DT_ASSERT(DT_IPROUT_FLAG_EN_FILT == DT_PIPE_IPFLT_FLAG_EN_FILT &&
                  DT_IPROUT_FLAG_EN_SRCIP_IPV4 == DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV4 &&
                  DT_IPROUT_FLAG_EN_SRCIP_IPV6 == DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV6 &&
                  DT_IPROUT_FLAG_EN_SRCPORT0 == DT_PIPE_IPFLT_FLAG_EN_SRCPORT0 &&
                  DT_IPROUT_FLAG_EN_SRCPORT1 == DT_PIPE_IPFLT_FLAG_EN_SRCPORT1 &&
                  DT_IPROUT_FLAG_EN_SRCPORT2 == DT_PIPE_IPFLT_FLAG_EN_SRCPORT2 &&
                  DT_IPROUT_FLAG_EN_DSTIP_IPV4 == DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV4 &&
                  DT_IPROUT_FLAG_EN_DSTIP_IPV6 == DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV6 &&
                  DT_IPROUT_FLAG_EN_DSTPORT0 == DT_PIPE_IPFLT_FLAG_EN_DSTPORT0 &&
                  DT_IPROUT_FLAG_EN_DSTPORT1 == DT_PIPE_IPFLT_FLAG_EN_DSTPORT1 &&
                  DT_IPROUT_FLAG_EN_DSTPORT2 == DT_PIPE_IPFLT_FLAG_EN_DSTPORT2 &&
                  DT_IPROUT_FLAG_EN_VLAN == DT_PIPE_IPFLT_FLAG_EN_VLAN &&
                  DT_IPROUT_FLAG_DUPLICATE == DT_PIPE_IPFLT_FLAG_DUPLICATE &&
                  DT_IPROUT_FLAG_VLAN0_1AD == DT_PIPE_IPFLT_FLAG_VLAN0_1AD &&
                  DT_IPROUT_FLAG_VLAN1_1AD == DT_PIPE_IPFLT_FLAG_VLAN1_1AD);
        Status = DtBcIPROUT_SetIpfilter(pPipeRtHwp->m_pIpRouter, DstIp, DstPort, SrcIp, 
                                                                  SrcPort, VlanId, Flags);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPipe(ERR, pPipe, "ERROR: DtBcIPROUT_SetIpfilter. Status: %08xh",
                                                                                  Status);
            return Status;
        }
        pPipeRtHwp->m_IpParsSet = TRUE;
    }
    else
    {
        Int i;
        DtPipeRxRtSwp* pPipeRtSwp = (DtPipeRxRtSwp*)pPipe;
        for (i = 0; i < 16; i++)
        {
            pPipeRtSwp->m_SrcIp[i] = SrcIp[i];
            pPipeRtSwp->m_DstIp[i] = DstIp[i];
        }
        for (i = 0; i < 3; i++)
        {
            pPipeRtSwp->m_SrcPort[i] = SrcPort[i];
            pPipeRtSwp->m_DstPort[i] = DstPort[i];
        }
        for (i = 0; i < 2; i++)
        {
            pPipeRtSwp->m_VlanId[i] = VlanId[i];
        }
        pPipeRtSwp->m_Flags = Flags;
        pPipeRtSwp->m_IpParsSet = TRUE;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_GetIpFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPipe_GetIpFilter(DtPipe* pPipe, UInt8 DstIp[16], UInt16 DstPort[3],
                           UInt8 SrcIp[16], UInt16  SrcPort[3], Int* pVlanId, Int* pFlags)
{
    DtStatus Status = DT_STATUS_OK;
    if (!IsRx(pPipe))
        return DT_STATUS_NOT_SUPPORTED;
    if (IsHwp(pPipe))
    {
        DtPipeRxRtHwp* pPipeRtHwp = (DtPipeRxRtHwp*)pPipe;
        if (!pPipeRtHwp->m_IpParsSet)
            return DT_STATUS_NOT_INITIALISED;
        Status = DtBcIPROUT_GetIpfilter(pPipeRtHwp->m_pIpRouter, DstIp, DstPort, SrcIp,
                                                                SrcPort, pVlanId, pFlags);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPipe(ERR, pPipe, "ERROR: DtBcIPROUT_GetIpfilter. Status: %08xh",
                                                                                  Status);
            return Status;
        }
    }
    else
    {
        Int i;
        DtPipeRxRtSwp* pPipeRtSwp = (DtPipeRxRtSwp*)pPipe;
        if (!pPipeRtSwp->m_IpParsSet)
            return DT_STATUS_NOT_INITIALISED;

        for (i = 0; i < 16; i++)
        {
            SrcIp[i] = pPipeRtSwp->m_SrcIp[i];
            DstIp[i] = pPipeRtSwp->m_DstIp[i];
        }
        for (i = 0; i < 3; i++)
        {
            SrcPort[i] = pPipeRtSwp->m_SrcPort[i];
            DstPort[i] = pPipeRtSwp->m_DstPort[i];
        }
        for (i = 0; i < 2; i++)
        {
            pVlanId[i] = pPipeRtSwp->m_VlanId[i];
        }
        *pFlags = pPipeRtSwp->m_Flags;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_IsPerIntEventActive -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool DtPipe_IsPerIntEvent(DtPipe* pPipe)
{
    return pPipe->m_TrigEvt.m_Initialized &&
        pPipe->m_TrigEvt.m_Id == DT_PIPE_EVENT_ID_PERINT;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_IsDataAvailEventActive -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool DtPipe_IsDataAvailCntEvent(DtPipe* pPipe)
{
    return pPipe->m_TrigEvt.m_Initialized &&
        pPipe->m_TrigEvt.m_Id == DT_PIPE_EVENT_ID_DATA_AVAIL_CNT;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_IsDataAvailThrEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool DtPipe_IsDataAvailThrEvent(DtPipe* pPipe)
{
    return pPipe->m_TrigEvt.m_Initialized &&
        pPipe->m_TrigEvt.m_Id == DT_PIPE_EVENT_ID_DATA_AVAIL_INT;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_CopyData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Destination write offset is updated only
DtStatus DtPipe_CopyData(DtPipe* pSrc, UInt SrcReadOffset, DtPipe* pDst, Int BytesToCopy)
{
    DtStatus Status = DT_STATUS_OK;
    Int FreeBufSizeDst;
    Int FreeUntilEndDst;
    Int NumBytesToEndSrc;
    UInt SrcBufSize;
    UInt8* pSrcBuf;
    UInt DstReadOffset;
    UInt DstWriteOffset;
    UInt DstBufSize;
    UInt8* pDstBuf;
    UInt8* pData;
    UInt MaxLoad;

    if (!pSrc->m_InUse || !pSrc->m_SharedBufferSet || !pDst->m_InUse || 
                                                                 !pDst->m_SharedBufferSet)
        return DT_STATUS_INVALID_IN_OPMODE;

    if (IsHwpKbuf(pSrc))
    {
        DtPipeHwpKbuf* pHwp = (DtPipeHwpKbuf*)pSrc;
        DT_ASSERT(IsRx(pSrc));
        SrcBufSize = pHwp->m_BufSize;
        pSrcBuf = pHwp->m_pBuffer;
    }
    else if (IsSwp(pSrc))
    {
        DtPipeSwp* pSwp = (DtPipeSwp*)pSrc;
        SrcBufSize = pSwp->m_BufSize;
        pSrcBuf = pSwp->m_pBuffer;
    }
    else
        return DT_STATUS_INVALID_PARAMETER;

    if (IsHwpKbuf(pDst))
    {
        DtPipeHwpKbuf* pHwp = (DtPipeHwpKbuf*)pDst;
        DstBufSize = pHwp->m_BufSize;
        pDstBuf = pHwp->m_pBuffer;
        DT_ASSERT(IsTx(pDst));
        Status = DtPipe_GetTxReadOffset(pDst, &DstReadOffset);
        if (!DT_SUCCESS(Status))
            return Status;
        DstWriteOffset = pHwp->m_OffsetCache;
        MaxLoad = pHwp->m_MaxLoad;
    }
    else if (IsSwp(pDst))
    {
        DtPipeSwp* pSwp = (DtPipeSwp*)pDst;
        DstBufSize = pSwp->m_BufSize;
        pDstBuf = pSwp->m_pBuffer;
        DstReadOffset = pSwp->m_ReadOffset;
        DstWriteOffset = pSwp->m_WriteOffset;
        MaxLoad = DstBufSize - (64 / 8);
    }
    else
        return DT_STATUS_INVALID_PARAMETER;

    // Destination
    if (DstReadOffset == DstWriteOffset)
        FreeBufSizeDst = MaxLoad;
    else
    {
        FreeBufSizeDst = (Int)((DstReadOffset+DstBufSize-DstWriteOffset) % DstBufSize);
        if (FreeBufSizeDst > (Int)MaxLoad)
            FreeBufSizeDst = MaxLoad;
    }
    FreeUntilEndDst = DstBufSize - DstWriteOffset;
    if (FreeBufSizeDst <= BytesToCopy)
    {
        pDst->m_ErrorFlag |= DT_PIPE_ERROR_OVERFLOW;
        return DT_STATUS_BUFFER_OVERFLOW;
    } else
        pDst->m_ErrorFlag &= ~DT_PIPE_ERROR_OVERFLOW;

    // Source
    NumBytesToEndSrc = SrcBufSize - SrcReadOffset;
    pData = pSrcBuf + SrcReadOffset;

    // Copy data to shared buffer
    if (FreeUntilEndDst >= BytesToCopy)
    {
        // Enough space until end in destination buffer to copy at ones
        if (NumBytesToEndSrc >= BytesToCopy)
        {
            //DtDbgOutPipe(ERR, pDst, "NumBytesToEnd >= BytesToCopy DstWriteOffset:%i"
            //                            " BytesToCopy:%i", DstWriteOffset, BytesToCopy);
            DtMemCopy(pDstBuf + DstWriteOffset, pData, BytesToCopy);
        }
        else
        {
            // Source buffer is split
            DtMemCopy(pDstBuf + DstWriteOffset, pData, NumBytesToEndSrc);
            DtMemCopy(pDstBuf + DstWriteOffset + NumBytesToEndSrc, pSrcBuf, 
                                                          BytesToCopy - NumBytesToEndSrc);
            DtDbgOutPipe(MAX, pDst, "1 NumBytesToEnd < BytesToCopy DstWriteOffset:%i"
                                           " NumBytesToEnd:%i BytesToCopy:%i", 
                                           DstWriteOffset, NumBytesToEndSrc, BytesToCopy);
        }
    }
    else
    {
        // Not enough space until end in destination buffer to copy at ones
        if (NumBytesToEndSrc >= BytesToCopy)
        {
            // Source buffer is not split
            DtMemCopy(pDstBuf + DstWriteOffset, pData, FreeUntilEndDst);
            DtMemCopy(pDstBuf, pData+FreeUntilEndDst, BytesToCopy - FreeUntilEndDst);
            DtDbgOutPipe(MAX, pDst, "2 NumBytesToEnd >= BytesToCopy DstWriteOffset:%i"
                                            " FreeUntilEnd:%i BytesToCopy:%i", 
                                            DstWriteOffset, FreeUntilEndDst, BytesToCopy);
        }
        else
        {
            if (FreeUntilEndDst >= NumBytesToEndSrc)
            {
                DtMemCopy(pDstBuf + DstWriteOffset, pData, NumBytesToEndSrc);
                FreeUntilEndDst -= NumBytesToEndSrc;
                DtMemCopy(pDstBuf + DstWriteOffset + NumBytesToEndSrc, pSrcBuf, 
                                                                         FreeUntilEndDst);
                DtMemCopy(pDstBuf, pSrcBuf + FreeUntilEndDst, 
                                        BytesToCopy - FreeUntilEndDst - NumBytesToEndSrc);
                DtDbgOutPipe(MAX, pDst, "3 FreeUntilEnd >= NumBytesToEnd DstWriteOffset:"
                          "%i NumBytesToEnd:%i FreeUntilEnd:%i BytesToCopy:%i", 
                          DstWriteOffset, NumBytesToEndSrc, FreeUntilEndDst, BytesToCopy);
            }
            else
            {
                DtMemCopy(pDstBuf + DstWriteOffset, pData, FreeUntilEndDst);
                NumBytesToEndSrc -= FreeUntilEndDst;
                DtMemCopy(pDstBuf, pData + FreeUntilEndDst, NumBytesToEndSrc);
                DtMemCopy(pDstBuf + NumBytesToEndSrc, pSrcBuf, 
                                        BytesToCopy - FreeUntilEndDst - NumBytesToEndSrc);
                DtDbgOutPipe(MAX, pDst, "4 FreeUntilEnd < NumBytesToEnd DstWriteOffset:%i"
                          " NumBytesToEnd:%i FreeUntilEnd:%i BytesToCopy:%i",
                          DstWriteOffset, NumBytesToEndSrc, FreeUntilEndDst, BytesToCopy);
            }
        }
    }
    DstWriteOffset = (DstWriteOffset + BytesToCopy) % DstBufSize;
    if (IsTx(pDst))
        Status = DtPipe_SetTxWriteOffset(pDst, DstWriteOffset);
    else 
    {
        DtPipeSwp* pSwPipe = (DtPipeSwp*)pDst;
        DT_ASSERT(IsSwp(pDst));
        pSwPipe->m_WriteOffset = DstWriteOffset;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_IsPacketForPipe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool DtPipe_IsPacketForPipe(DtPipeRxRtSwp* pPipe, UInt8* DstIp, UInt16 DstPort,
                        UInt8* SrcIp, UInt16 SrcPort, Int VlanId1, Int VlanId2, Int Flags)
{
    Int i;
    Bool IpV4;
    DT_ASSERT(IsSwp(pPipe) && IsRx(pPipe));
    if (!pPipe->m_IpParsSet)
        return FALSE;
    if (pPipe->m_OpMode != DT_BLOCK_OPMODE_RUN)
        return FALSE;
    if ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_FILT) == 0)
        return FALSE;
    IpV4 = (Flags & DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV4) != 0;

    if (IpV4 && ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV6) != 0 ||
        ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV6) != 0)))
        return FALSE;
    if (!IpV4 && ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV4) != 0 ||
        ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV4) != 0)))
        return FALSE;
    if ((Flags & (DT_PIPE_IPFLT_FLAG_VLAN0_1AD | DT_PIPE_IPFLT_FLAG_VLAN1_1AD)) !=
        (pPipe->m_Flags & (DT_PIPE_IPFLT_FLAG_VLAN0_1AD | DT_PIPE_IPFLT_FLAG_VLAN1_1AD)))
        return FALSE;
    if (((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV4) != 0) ||
        ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV6) != 0))
    {
        for (i = 0; i < (IpV4 ? 4 : 16); i++)
        {
            if (SrcIp[i] != pPipe->m_SrcIp[i])
                return FALSE;
        }
    }
    if (((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV4) != 0) ||
        ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV6) != 0))
    {
        for (i = 0; i < (IpV4 ? 4 : 16); i++)
        {
            if (DstIp[i] != pPipe->m_DstIp[i])
                return FALSE;
        }
    }
    if ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_VLAN) != 0)
    {
        if ((pPipe->m_Flags& DT_PIPE_IPFLT_FLAG_VLAN0_1AD)!=0 && 
                                                            pPipe->m_VlanId[0] != VlanId1)
            return FALSE;
        if ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_VLAN1_1AD) != 0 && 
                                                            pPipe->m_VlanId[1] != VlanId2)
            return FALSE;
    }
    if ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_SRCPORT0) != 0)
    {
        if (pPipe->m_SrcPort[0] != SrcPort)
        {
            if ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_SRCPORT1) == 0 ||
                                                           pPipe->m_SrcPort[1] != SrcPort)
            {
                if ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_SRCPORT2) == 0 || 
                                                           pPipe->m_SrcPort[2] != SrcPort)
                    return FALSE;
            }
        }
    }
    if ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_DSTPORT0) != 0)
    {
        if (pPipe->m_DstPort[0] != DstPort)
        {
            if ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_DSTPORT1) == 0 || 
                                                           pPipe->m_DstPort[1] != DstPort)
            {
                if ((pPipe->m_Flags & DT_PIPE_IPFLT_FLAG_EN_DSTPORT2) == 0 || 
                                                           pPipe->m_DstPort[2] != DstPort)
                    return FALSE;
            }
        }
    }
    return TRUE;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe_NeedDuplicate_HwpHwp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// pPipeD:Pipe to duplicate, pPipeC: Pipe to check
//
Bool DtPipe_IsDuplicateFilter(DtPipe* pPipeD, DtPipe* pPipeC)
{
    Int i;
    Bool IpV4;
    DtStatus Status = DT_STATUS_OK;
    UInt8 DstIpD[16], DstIpC[16];
    UInt16 DstPortD[3], DstPortC[3];
    UInt8 SrcIpD[16], SrcIpC[16];
    UInt16 SrcPortD[3], SrcPortC[3];
    Int VlanIdD[2], VlanIdC[2];
    Int FlagsD, FlagsC;
    Bool SrcIdentical = TRUE;
    Bool ChkSrc = FALSE;
    Bool DstIdentical = TRUE;
    Bool ChkDst = FALSE;

    DT_ASSERT(IsRx(pPipeD) && IsRx(pPipeC));
    
    Status = DtPipe_GetIpFilter(pPipeD, DstIpD, DstPortD, SrcIpD, SrcPortD, VlanIdD, 
                                                                                 &FlagsD);
    if (!DT_SUCCESS(Status))
        return FALSE;
    Status = DtPipe_GetIpFilter(pPipeC, DstIpC, DstPortC, SrcIpC, SrcPortC, VlanIdC, 
                                                                                 &FlagsC);
    if (!DT_SUCCESS(Status))
        return FALSE;

    //if (pPipe->m_OpMode != DT_BLOCK_OPMODE_RUN)
    //    return FALSE;
    if ((FlagsD & DT_PIPE_IPFLT_FLAG_EN_FILT) == 0)
        return FALSE;
    if ((FlagsC & DT_PIPE_IPFLT_FLAG_EN_FILT) == 0)
        return FALSE;

    IpV4 = (FlagsD & (DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV4 | 
                                                  DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV4)) != 0;
    if (((FlagsC & (DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV4 | 
                                         DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV4)) != 0) != IpV4)
        return FALSE;

    // Do we need to check the SrcIp
    if ((((FlagsD & DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV4) != 0) ||
        ((FlagsD & DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV6) != 0)) &&
        (((FlagsC & DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV4) != 0) ||
          ((FlagsC & DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV6) != 0)))
    {
        ChkSrc = TRUE;
        for (i = 0; i < (IpV4 ? 4 : 16); i++)
        {
            if (SrcIpC[i] != SrcIpD[i])
            {
                SrcIdentical = FALSE;
                break;
            }
        }
        DtDbgOutPipe(AVG, pPipeC, "SrcIp Identical: %i Chk: %s:%i", SrcIdentical ? 1 : 0,
                                          PipeType2Str(pPipeD->m_PipeType), pPipeD->m_Id);

    }
    // Do we need to check the DstIp
    if ((((FlagsD & DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV4) != 0) ||
        ((FlagsD & DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV6) != 0)) &&
        (((FlagsC & DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV4) != 0) ||
            ((FlagsC & DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV6) != 0)))
    {
        ChkDst = TRUE;
        for (i = 0; i < (IpV4 ? 4 : 16); i++)
        {
            if (DstIpC[i] != DstIpD[i])
            {
                DstIdentical = FALSE;
                break;
            }
        }
        DtDbgOutPipe(AVG, pPipeC, "DstIp Identical: %i Chk: %s:%i", DstIdentical ? 1 : 0, 
                                          PipeType2Str(pPipeD->m_PipeType), pPipeD->m_Id);
    }
    // Do we need to check the SrcPort
    if ((ChkSrc && SrcIdentical) || !ChkSrc)
    {
        Int i, j;
        // Check src port
        Bool PortIdentical = FALSE;
        Bool ChkPort = FALSE;
        for (i = 0; i < 3 && !PortIdentical; i++)
        {
            int FlagD=0;
            int FlagC=0;
            switch (i)
            {
            case 0: FlagD = FlagsD & DT_PIPE_IPFLT_FLAG_EN_SRCPORT0; break;
            case 1: FlagD = FlagsD & DT_PIPE_IPFLT_FLAG_EN_SRCPORT1; break;
            case 2: FlagD = FlagsD & DT_PIPE_IPFLT_FLAG_EN_SRCPORT2; break;
            }
            if (FlagD == 0)
                break;
            ChkPort = TRUE;
            for (j = 0; j < 3 && !PortIdentical; j++)
            {
                switch (j)
                {
                case 0: FlagC = FlagsC & DT_PIPE_IPFLT_FLAG_EN_SRCPORT0; break;
                case 1: FlagC = FlagsC & DT_PIPE_IPFLT_FLAG_EN_SRCPORT1; break;
                case 2: FlagC = FlagsC & DT_PIPE_IPFLT_FLAG_EN_SRCPORT2; break;
                }
                if (FlagC == 0)
                {
                    if (j == 0) // No source port filtering. Always identical
                        ChkPort = FALSE;
                    break;
                }
                if (SrcPortD[i] == SrcPortC[j])
                    PortIdentical = TRUE;
            }
        }
        if (ChkPort)
        {
            SrcIdentical = PortIdentical;
            DtDbgOutPipe(AVG, pPipeC, "SrcPort Identical: %i Chk: %s:%i", 
                    SrcIdentical ? 1 : 0, PipeType2Str(pPipeD->m_PipeType), pPipeD->m_Id);
        }
    }

    // Do we need to check the DstPort
    if ((ChkDst && DstIdentical) || !ChkDst)
    {
        Int i, j;
        // Check dst port
        Bool PortIdentical = FALSE;
        Bool ChkPort = FALSE;
        for (i = 0; i < 3 && !PortIdentical; i++)
        {
            int FlagD=0;
            int FlagC=0;
            switch (i)
            {
            case 0: FlagD = FlagsD & DT_PIPE_IPFLT_FLAG_EN_DSTPORT0; break;
            case 1: FlagD = FlagsD & DT_PIPE_IPFLT_FLAG_EN_DSTPORT1; break;
            case 2: FlagD = FlagsD & DT_PIPE_IPFLT_FLAG_EN_DSTPORT2; break;
            }
            if (FlagD == 0)
                break;
            ChkPort = TRUE;
            for (j = 0; j < 3 && !PortIdentical; j++)
            {
                switch (j)
                {
                case 0: FlagC = FlagsC & DT_PIPE_IPFLT_FLAG_EN_DSTPORT0; break;
                case 1: FlagC = FlagsC & DT_PIPE_IPFLT_FLAG_EN_DSTPORT1; break;
                case 2: FlagC = FlagsC & DT_PIPE_IPFLT_FLAG_EN_DSTPORT2; break;
                }
                if (FlagC == 0)
                {
                    if (j == 0) // No destination port filtering. Always identical
                        ChkPort = FALSE;
                    break;
                }
                if (DstPortD[i] == DstPortC[j])
                    PortIdentical = TRUE;
            }
        }
        if (ChkPort)
        {
            DstIdentical = PortIdentical;
            DtDbgOutPipe(AVG, pPipeC, "DstPort Identical: %i Chk: %s:%i", 
                    DstIdentical ? 1 : 0, PipeType2Str(pPipeD->m_PipeType), pPipeD->m_Id);
        }
    }
    
    if ((ChkDst && ChkSrc && DstIdentical && SrcIdentical) ||
        (ChkDst && !ChkSrc && DstIdentical) ||
        (!ChkDst && ChkSrc && SrcIdentical))
    {
        // Check VLAN
        if (((FlagsD & DT_PIPE_IPFLT_FLAG_VLAN0_1AD) != 
                                               (FlagsC & DT_PIPE_IPFLT_FLAG_VLAN0_1AD)) ||
            ((FlagsD & DT_PIPE_IPFLT_FLAG_VLAN1_1AD) != 
                                                 (FlagsC & DT_PIPE_IPFLT_FLAG_VLAN1_1AD)))
        {
            DstIdentical = FALSE;
            SrcIdentical = FALSE;
        }
        else {
            if ((FlagsD & DT_PIPE_IPFLT_FLAG_VLAN0_1AD) != 0)
            {
                if (VlanIdD[0] != VlanIdC[0])
                {
                    DstIdentical = FALSE;
                    SrcIdentical = FALSE;
                }
                else if ((FlagsD & DT_PIPE_IPFLT_FLAG_VLAN1_1AD) != 0)
                {
                    if (VlanIdD[1] != VlanIdC[1])
                    {
                        DstIdentical = FALSE;
                        SrcIdentical = FALSE;
                    }
                }
            }
        }
        DtDbgOutPipe(AVG, pPipeC, "VLan Identical: %i Chk: %s:%i", 
                                          DstIdentical||SrcIdentical ? 1 : 0, 
                                          PipeType2Str(pPipeD->m_PipeType), pPipeD->m_Id);
    }
    return ((ChkDst && ChkSrc && DstIdentical && SrcIdentical) ||
                                                    (ChkDst && !ChkSrc && DstIdentical) ||
                                                    (!ChkDst && ChkSrc && SrcIdentical));
}
