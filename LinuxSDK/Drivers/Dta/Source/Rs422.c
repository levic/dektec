//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Rs422.c *#*#*#*#*#*#*#*#*#* (C) 2012-2016 DekTec
//
// Dta driver - RS-422 read/write operations
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012-2016 DekTec Digital Video B.V.
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtaIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static void  DtaNonIpRs422IntDpc(DtDpcArgs* pArgs);
static DtStatus  DtaRs422Write(DtaNonIpPort*  pNonIpPort, UInt8*  pBuf, Int  NumBytes,
                                                                           Int  Blocking);
static DtStatus  DtaRs422Read(DtaNonIpPort*  pNonIpPort, UInt8*  pBuf, Int  BytesToRead,
                                                        Int  Timeout, Int*  NumBytesRead);
static DtStatus  DtaRs422Flush(DtaNonIpPort*  pNonIpPort);


#define RS422_TX_IDLE        0
#define RS422_TX_TRANSMIT    1

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpRs422Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpRs422Init(DtaNonIpPort*  pNonIpPort)
{
    DtaRs422Port*  pRs422 = &pNonIpPort->m_Rs422;
    DT_ASSERT(pNonIpPort->m_CapRs422);

    DtSpinLockInit(&pRs422->m_StateLock);
    DtDpcInit(&pRs422->m_IntDpc, DtaNonIpRs422IntDpc, TRUE);
    DtEventInit(&pRs422->m_TxCmplEvent, TRUE);
    DtEventInit(&pRs422->m_RxDataAvailEvent, TRUE);

    pRs422->m_RxNumBytes = 0;
    pRs422->m_TxState = RS422_TX_IDLE;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpRs422InterruptEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Enables Non-IP RS-422 port specific interrupts
//
DtStatus  DtaNonIpRs422InterruptEnable(DtaNonIpPort* pNonIpPort)
{
    DtIntEnableState  IntEnaState;

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapRs422);

    IntEnaState = pNonIpPort->m_pDvcData->m_IntEnableState;

    // Do nothing if master state is not enabled or enabling
    if (IntEnaState!=INT_ENABLING && IntEnaState!=INT_ENABLED)
    {
        DtDbgOut(AVG, NONIP, "Master-interupt state (%d) is not enabled", IntEnaState);
        return DT_STATUS_OK;
    }

    DtaRegRs422CtrlSetMaster(pNonIpPort->m_pRs422Regs, 1);
    DtaRegRs422CtrlSetPolarity(pNonIpPort->m_pRs422Regs, 1);
    DtaRegRs422CtrlClrRxFifo(pNonIpPort->m_pRs422Regs);
    DtaRegRs422CtrlClrTxFifo(pNonIpPort->m_pRs422Regs);
    DtaRegRs422CtrlSetRxEnable(pNonIpPort->m_pRs422Regs, 1);

    DtaRegRs422CtrlSetRxDataAvailIntEn(pNonIpPort->m_pRs422Regs, 1);
    DtaRegRs422CtrlSetTxDataTransmitIntEn(pNonIpPort->m_pRs422Regs, 1);
    
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpRs422InterruptDisable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Disables Non-IP RS-422 port specific interrupts
//
DtStatus  DtaNonIpRs422InterruptDisable(DtaNonIpPort* pNonIpPort)
{
    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapRs422);
    
    DtaRegRs422CtrlSetRxDataAvailIntEn(pNonIpPort->m_pRs422Regs, 0);
    DtaRegRs422CtrlSetTxDataTransmitIntEn(pNonIpPort->m_pRs422Regs, 0);
    
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixLastFrameIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaNonIpRs422IntDpc(DtDpcArgs* pArgs)
{
    Int  NumBytes, i;
    DtaNonIpPort*  pNonIpPort = (DtaNonIpPort*)pArgs->m_pContext;
    DtaRs422Port*  pRs422 = &pNonIpPort->m_Rs422;

    DtSpinLockAcquireAtDpc(&pRs422->m_StateLock);
    // Check if TX is complete
    if (DtaRegRs422StatGetTxReadyInt(pNonIpPort->m_pRs422Regs))
    {
        DtaRegRs422StatClrTxReadyInt(pNonIpPort->m_pRs422Regs);
        pRs422->m_TxState = RS422_TX_IDLE;
        DtEventSet(&pRs422->m_TxCmplEvent);
    }
    // Check for RX data
    DtaRegRs422StatClrRxDataAvailInt(pNonIpPort->m_pRs422Regs);
    NumBytes = DtaRegRs422StatGetRxNumBytes(pNonIpPort->m_pRs422Regs);
    // Prevent buffer overflows. In case there is too many data leave the rest in the
    // hardware buffer.
    if (pRs422->m_RxNumBytes+NumBytes > sizeof(pRs422->m_RxBuf))
        NumBytes = sizeof(pRs422->m_RxBuf) - pRs422->m_RxNumBytes;
    for (i=0; i<NumBytes; i++)
    {
        pRs422->m_RxBuf[pRs422->m_RxNumBytes+i] =
                                           DtaRegRs422RxDataGet(pNonIpPort->m_pRs422Regs);
    }
    if (NumBytes > 0)
    {
        pRs422->m_RxNumBytes += NumBytes;
        DtEventSet(&pRs422->m_RxDataAvailEvent);
    }
    DtSpinLockReleaseFromDpc(&pRs422->m_StateLock);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRs422Ioctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaRs422Ioctl(
    DtaDeviceData* pDvcData,
    DtFileObject* pFile,
    DtIoctlObject* pIoctl,
    Bool  PowerDownPending)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  NonIpPortIndex;        // Index in the nonip port struct
    DtaNonIpPort*  pNonIpPort = NULL;
    DtaIoctlRs422CmdInput*  pRs422CmdInput = 
                                           (DtaIoctlRs422CmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlRs422CmdOutput*  pRs422CmdOutput =
                                         (DtaIoctlRs422CmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlRs422CmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlRs422CmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    pNonIpPort = NULL;  // Assume a board level request
    if (DT_SUCCESS(DtaGetNonIpPortIndex(pDvcData, pRs422CmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpPortIndex];
    else
         return DT_STATUS_INVALID_PARAMETER;
    if (!pNonIpPort->m_CapRs422)
        return DT_STATUS_NOT_SUPPORTED;
    
    // Determine final required output/input sizes
    switch (pRs422CmdInput->m_Cmd)
    {
    case DTA_RS422_CMD_WRITE:
        pCmdStr = "DTA_RS422_CMD_WRITE";
        InReqSize += sizeof(DtaIoctlRs422CmdWriteInput);
        // We expect no additional data in the output buffer
        OutReqSize += 0;
        break;

    case DTA_RS422_CMD_READ:
        pCmdStr = "DTA_RS422_CMD_READ";
        InReqSize += sizeof(DtaIoctlRs422CmdReadInput);
        OutReqSize += sizeof(DtaIoctlRs422CmdReadOutput);
        break;

    case DTA_RS422_CMD_FLUSH:
        pCmdStr = "DTA_RS422_CMD_FLUSH";
        // There is no additional in or out data
        OutReqSize += 0; InReqSize += 0;
        break;

    default:
        pCmdStr = "??UNKNOWN RS422CMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, RS422, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, RS422, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, RS422, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pRs422CmdInput->m_Cmd)
        {
        case DTA_RS422_CMD_WRITE:
            if (pRs422CmdInput->m_Data.m_Write.m_NumBytes >
                                             sizeof(pRs422CmdInput->m_Data.m_Write.m_Buf))
                Status = DT_STATUS_INVALID_PARAMETER;
            if (DT_SUCCESS(Status))
                Status = DtaRs422Write(pNonIpPort, pRs422CmdInput->m_Data.m_Write.m_Buf,
                                               pRs422CmdInput->m_Data.m_Write.m_NumBytes,
                                               pRs422CmdInput->m_Data.m_Write.m_Blocking);
            break;

        case DTA_RS422_CMD_READ:
            if (pRs422CmdInput->m_Data.m_Read.m_BytesToRead >
                                             sizeof(pRs422CmdOutput->m_Data.m_Read.m_Buf))
                Status = DT_STATUS_INVALID_PARAMETER;
            if (DT_SUCCESS(Status))
                Status = DtaRs422Read(pNonIpPort, pRs422CmdOutput->m_Data.m_Read.m_Buf,
                                              pRs422CmdInput->m_Data.m_Read.m_BytesToRead,
                                              pRs422CmdInput->m_Data.m_Read.m_Timeout,
                                              &pRs422CmdOutput->m_Data.m_Read.m_NumBytes);
            
            break;

        case DTA_RS422_CMD_FLUSH:
            Status = DtaRs422Flush(pNonIpPort);
            break;
        }
    }

    // If we failed, no data has to be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, RS422, "Rs422Cmd=0x%x: NOT SUPPORTED", pRs422CmdInput->m_Cmd);
        else if (Status == DT_STATUS_IO_PENDING)
            DtDbgOut(MAX, RS422, "%s: ERROR %xh", pCmdStr, Status); // NOT A REAL ERROR
        else
            DtDbgOut(MIN, RS422, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRs422Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaRs422Write(
    DtaNonIpPort*  pNonIpPort,
    UInt8*  pBuf,
    Int  NumBytes,
    Int  Blocking)
{
    DtaRs422Port*  pRs422 = &pNonIpPort->m_Rs422;
    Int  i;

    DtSpinLockAcquire(&pRs422->m_StateLock);
    if (pRs422->m_TxState != RS422_TX_IDLE)
    {
        DtSpinLockRelease(&pRs422->m_StateLock);
        return DT_STATUS_BUSY;
    }
    pRs422->m_TxState = RS422_TX_TRANSMIT;
    DtSpinLockRelease(&pRs422->m_StateLock);

    for (i=0; i<NumBytes; i++)
        DtaRegRs422TxDataSet(pNonIpPort->m_pRs422Regs, pBuf[i]);
    DtEventReset(&pRs422->m_TxCmplEvent);
    DtaRegRs422CtrlSetTxStart(pNonIpPort->m_pRs422Regs);

    if (Blocking)
        return DtEventWait(&pRs422->m_TxCmplEvent, -1);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRs422Read -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaRs422Read(
    DtaNonIpPort*  pNonIpPort,
    UInt8*  pBuf,
    Int  BytesToRead,
    Int  Timeout,
    Int*  NumBytesRead)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaRs422Port*  pRs422 = &pNonIpPort->m_Rs422;
    Int  i, Offset;
#ifdef WINBUILD
    LARGE_INTEGER  StartTime, CurTime;
#else
    struct timespec  StartTime, CurTime;
#endif
    Int  TimeElapsed;
    Int  OrigTimeout = Timeout;

    if (Timeout < 0)
        return DT_STATUS_INVALID_PARAMETER;

    *NumBytesRead = 0;

    DtSpinLockAcquire(&pRs422->m_StateLock);
    if (pRs422->m_RxNumBytes >= BytesToRead)
    {
        // Enough bytes available in internal buffer. Copy bytes and return
        for (i=0; i<BytesToRead; i++)
            pBuf[i] = pRs422->m_RxBuf[i];
        for (; i<pRs422->m_RxNumBytes; i++)
            pRs422->m_RxBuf[i-BytesToRead] = pRs422->m_RxBuf[i];
        pRs422->m_RxNumBytes -= BytesToRead;
        *NumBytesRead = BytesToRead;
        DtSpinLockRelease(&pRs422->m_StateLock);
        return DT_STATUS_OK;
    }

    for (i=0; i<pRs422->m_RxNumBytes; i++)
        pBuf[i] = pRs422->m_RxBuf[i];
    *NumBytesRead = pRs422->m_RxNumBytes;
    pRs422->m_RxNumBytes = 0;
    
    // Reset the event before releasing the lock to prevent any timing issues
    DtEventReset(&pRs422->m_RxDataAvailEvent);

    DtSpinLockRelease(&pRs422->m_StateLock);

    if (Timeout == 0)
        return DT_STATUS_OK;
    
#ifdef WINBUILD
    KeQueryTickCount(&StartTime);
#else
    getnstimeofday(&StartTime);
#endif
    do {
        Status = DtEventWait(&pRs422->m_RxDataAvailEvent, Timeout);
        if (DT_SUCCESS(Status))
        {
            DtSpinLockAcquire(&pRs422->m_StateLock);
            for (i=0; i<pRs422->m_RxNumBytes && *NumBytesRead<BytesToRead; i++)
                pBuf[(*NumBytesRead)++] = pRs422->m_RxBuf[i];
            // Shift any left-over bytes in RX buffer to the front
            Offset = i;
            for (; i<pRs422->m_RxNumBytes; i++)
                pRs422->m_RxBuf[i-Offset] = pRs422->m_RxBuf[i];
            pRs422->m_RxNumBytes -= Offset;
            DtSpinLockRelease(&pRs422->m_StateLock);
        }
#ifdef WINBUILD
        KeQueryTickCount(&CurTime);
        TimeElapsed = (Int)((CurTime.QuadPart - StartTime.QuadPart)*KeQueryTimeIncrement()
                                                                                 / 10000);
#else
        getnstimeofday(&CurTime);
        TimeElapsed = DtDivide64(((CurTime.tv_sec-StartTime.tv_sec)*1000000000LL
                                   + (CurTime.tv_nsec-StartTime.tv_nsec)), 1000000, NULL);
#endif

        Timeout = OrigTimeout - TimeElapsed;
    } while (*NumBytesRead<BytesToRead && DT_SUCCESS(Status) && Timeout>0);

    // A timeout is not an error in this case. Let userspace deal with the fact that
    // the read operation is not complete.
    if (Status == DT_STATUS_TIMEOUT)
        Status = DT_STATUS_OK;

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRs422Flush -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaRs422Flush(DtaNonIpPort*  pNonIpPort)
{
    DtaRs422Port*  pRs422 = &pNonIpPort->m_Rs422;
    DtSpinLockAcquire(&pRs422->m_StateLock);
    // Clear hardware RX fifo
    DtaRegRs422CtrlClrRxFifo(pNonIpPort->m_pRs422Regs);
    // Clear internal buffer
    pRs422->m_RxNumBytes = 0;
    DtSpinLockRelease(&pRs422->m_StateLock);
    return DT_STATUS_OK;
}
