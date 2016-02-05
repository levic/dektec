//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NonIpTx.c *#*#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
//
// Dta driver - Non IP TX functionality - Implementation of TX specific functionality for
//                                        non IP ports.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtaNonIpTxProcessFlags(DtaNonIpPort* pNonIpPort);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpTxIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpTxIoctl(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  NonIpPortIndex;        // Index in the nonip port struct
    DtaIoctlNonIpTxCmdInput* pNonIpTxCmdInput = 
                                         (DtaIoctlNonIpTxCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlNonIpTxCmdOutput* pNonIpTxCmdOutput =
                                       (DtaIoctlNonIpTxCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlNonIpTxCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlNonIpTxCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    if (!DT_SUCCESS(DtaGetNonIpPortIndex(pDvcData, pNonIpTxCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
        return DT_STATUS_INVALID_PARAMETER;
    
    // Determine final required output/input sizes
    switch (pNonIpTxCmdInput->m_Cmd)
    {
    case DTA_NONIP_TX_CMD_GET_FLAGS:
        pCmdStr = "DTA_NONIP_TX_CMD_GET_FLAGS";
        OutReqSize += sizeof(DtaIoctlNonIpTxCmdGetFlagsOutput);
        // We expect no additional data in the input buffer
        InReqSize += 0;
        break;

    case DTA_NONIP_TX_CMD_CLEAR_FLAGS:
        pCmdStr = "DTA_NONIP_TX_CMD_CLEAR_FLAGS";
        InReqSize += sizeof(DtaIoctlNonIpTxCmdClearFlagsInput);
        // We expect no output buffer at all
        OutReqSize = 0;
        break;

    case DTA_NONIP_TX_CMD_SET_FAILSAFE_CFG:
        pCmdStr = "DTA_NONIP_TX_CMD_SET_FAILSAFE_CFG";
        InReqSize += sizeof(DtaIoctlNonIpTxCmdSetFailsafeCfgInput);
        // We expect no output buffer at all
        OutReqSize = 0;
        break;

    case DTA_NONIP_TX_CMD_SET_FAILSAFE_ALIVE:
        pCmdStr = "DTA_NONIP_TX_CMD_SET_FAILSAFE_ALIVE";
        // We expect no additional data in the input buffer
        InReqSize += 0;
        // We expect no output buffer at all
        OutReqSize = 0;
        break;
    
    case DTA_NONIP_TX_CMD_GET_FAILSAFE_INFO:
        pCmdStr = "DTA_NONIP_TX_CMD_GET_FAILSAFE_INFO";
        OutReqSize += sizeof(DtaIoctlNonIpTxCmdGetFailsafeInfoOutput);
        // We expect no additional data in the input buffer
        InReqSize += 0;
        break;

    default:
        pCmdStr = "??UNKNOWN VPDCMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, NONIP, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, NONIP, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, NONIP, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
                pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pNonIpTxCmdInput->m_Cmd)
        {
        case DTA_NONIP_TX_CMD_GET_FLAGS:
            Status = DtaNonIpTxGetFlags(&pDvcData->m_pNonIpPorts[NonIpPortIndex],
                                         &pNonIpTxCmdOutput->m_Data.m_GetFlags.m_Status,
                                         &pNonIpTxCmdOutput->m_Data.m_GetFlags.m_Latched);
            break;

        case DTA_NONIP_TX_CMD_CLEAR_FLAGS:
            Status = DtaNonIpTxClearFlags(&pDvcData->m_pNonIpPorts[NonIpPortIndex],
                                    pNonIpTxCmdInput->m_Data.m_ClearFlags.m_FlagsToClear);
            break;

        case DTA_NONIP_TX_CMD_SET_FAILSAFE_CFG:
            Status = DtaNonIpTxSetFailsafeCfg(&pDvcData->m_pNonIpPorts[NonIpPortIndex],
                                     pNonIpTxCmdInput->m_Data.m_SetFailsafeCfg.m_Enable,
                                     pNonIpTxCmdInput->m_Data.m_SetFailsafeCfg.m_Timeout);
            break;

        case DTA_NONIP_TX_CMD_SET_FAILSAFE_ALIVE:
            Status = DtaNonIpTxSetFailsafeAlive(&pDvcData->m_pNonIpPorts[NonIpPortIndex]);
            break;
        
        case DTA_NONIP_TX_CMD_GET_FAILSAFE_INFO:
            Status = DtaNonIpTxGetFailsafeInfo(&pDvcData->m_pNonIpPorts[NonIpPortIndex],
                                   &pNonIpTxCmdOutput->m_Data.m_GetFailsafeInfo.m_Enable,
                                   &pNonIpTxCmdOutput->m_Data.m_GetFailsafeInfo.m_Timeout,
                                   &pNonIpTxCmdOutput->m_Data.m_GetFailsafeInfo.m_Alive);
            break;

        default:
            Status = DT_STATUS_NOT_SUPPORTED;
        }
    }

    // If we failed, no data has te be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, NONIP, "NonIpTxCmd=0x%x: NOT SUPPORTED", 
                                                                 pNonIpTxCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, NONIP, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TX Flags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpTxGetFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpTxGetFlags(DtaNonIpPort* pNonIpPort, Int* pStatus, Int* pLatched)
{
    // Update flags
    DtaNonIpTxProcessFlagsFromUser(pNonIpPort);

    // Update DMA pending status
    //if (NonIpTxIsDmaPending(pNonIpPort))
    //    pNonIpPort->m_Flags |= DTA_TX_DMA_PENDING;

    DtSpinLockAcquire(&pNonIpPort->m_FlagsSpinLock);

    // Return flags
    *pStatus = pNonIpPort->m_Flags;
    *pLatched = pNonIpPort->m_FlagsLatched;

    DtSpinLockRelease(&pNonIpPort->m_FlagsSpinLock);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpTxClearFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpTxClearFlags(DtaNonIpPort* pNonIpPort, Int FlagsToClear)
{
    DtSpinLockAcquire(&pNonIpPort->m_FlagsSpinLock);

    // Clear latched flags
    pNonIpPort->m_Flags &= ~FlagsToClear;
    pNonIpPort->m_FlagsLatched &= ~FlagsToClear;

    // Also clear flags in Transmit Status register, to avoid that flags in
    // m_Latched are set again in next periodic interrupt.
    if (!pNonIpPort->m_CapMatrix)
    {
        if ((FlagsToClear&DTA_TX_FIFO_UFL) == DTA_TX_FIFO_UFL)
            DtaRegTxStatClrUflInt(pNonIpPort->m_pTxRegs);
        if ((FlagsToClear&DTA_TX_SYNC_ERR) == DTA_TX_SYNC_ERR)
            DtaRegTxStatClrSyncInt(pNonIpPort->m_pTxRegs);
    }
    else
    {
        if ((FlagsToClear&DTA_TX_FIFO_UFL) == DTA_TX_FIFO_UFL)
            DtaRegHdStatClrTxUflErrInt(pNonIpPort->m_pTxRegs);
        if ((FlagsToClear&DTA_TX_SYNC_ERR) == DTA_TX_SYNC_ERR)
            DtaRegHdStatClrTxSyncErrInt(pNonIpPort->m_pTxRegs);
    }

    // Special case for DTA-102
    if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 102)
    {
        if ((FlagsToClear&DTA_TX_READBACK_ERR) == DTA_TX_READBACK_ERR)
            DtaRegTxStatClrShortInt(pNonIpPort->m_pTxRegs);
    }

    DtSpinLockRelease(&pNonIpPort->m_FlagsSpinLock);

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpTxProcessFlagsFromDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaNonIpTxProcessFlagsFromDpc(DtaNonIpPort* pNonIpPort)
{
    DtSpinLockAcquireAtDpc(&pNonIpPort->m_FlagsSpinLock);
    DtaNonIpTxProcessFlags(pNonIpPort);
    DtSpinLockReleaseFromDpc(&pNonIpPort->m_FlagsSpinLock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpTxProcessFlagsFromUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaNonIpTxProcessFlagsFromUser(DtaNonIpPort* pNonIpPort)
{
    DtSpinLockAcquire(&pNonIpPort->m_FlagsSpinLock);
    DtaNonIpTxProcessFlags(pNonIpPort);
    DtSpinLockRelease(&pNonIpPort->m_FlagsSpinLock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpTxProcessFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This routine is called from:
// - General periodic DPC to update status flags: DtaNonIpTxProcessFlagsFromDpc
// - DeviceIoCtrl (GetFlags): DtaNonIpTxProcessFlagsFromUser
//
// Pre: Spinlock m_FlagsSpinlock is acquired
//
void  DtaNonIpTxProcessFlags(DtaNonIpPort* pNonIpPort)
{
    Int  Status = 0;
    UInt32  TxStatus;

    DT_ASSERT(pNonIpPort->m_pTxRegs != NULL);
    if (pNonIpPort->m_CapMatrix)
    {
        TxStatus = DtaRegHdStatusGet(pNonIpPort->m_pTxRegs);
        if (TxStatus & DT_HD_STATUS_TXUFLERRINT_MSK)
        {
            Status |= DTA_TX_FIFO_UFL;
            DtaRegHdStatClrTxUflErrInt(pNonIpPort->m_pTxRegs);
        }
        if (TxStatus & DT_HD_STATUS_TXSYNCERRINT_MSK)
        {
            Status |= DTA_TX_SYNC_ERR;
            DtaRegHdStatClrTxSyncErrInt(pNonIpPort->m_pTxRegs);
        }
    }
    else
    {
        TxStatus = DtaRegTxStatGet(pNonIpPort->m_pTxRegs);
        if (TxStatus & DT_TXSTAT_UFLINT_MSK)
        {
            Status |= DTA_TX_FIFO_UFL;
            DtaRegTxStatClrUflInt(pNonIpPort->m_pTxRegs);
        }
        if (TxStatus & DT_TXSTAT_SYNCINT_MSK)
        {
            Status |= DTA_TX_SYNC_ERR;
            DtaRegTxStatClrSyncInt(pNonIpPort->m_pTxRegs);
        }
        if (TxStatus & DT_TXSTAT_SHORTINT_MSK)
        {
            Status |= DTA_TX_READBACK_ERR;
            DtaRegTxStatClrShortInt(pNonIpPort->m_pTxRegs);
        }
    }

    // Latch status flags
    pNonIpPort->m_FlagsLatched |= Status;
    pNonIpPort->m_Flags = Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Failsafe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpTxSetFailsafeCfg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpTxSetFailsafeCfg(DtaNonIpPort* pNonIpPort, Int Enable, Int Timeout)
{
    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_FAILSAFE].m_Value != DT_IOCONFIG_TRUE)
        return DT_STATUS_CONFIG_ERROR;

    // Enable / disable failsafe
    pNonIpPort->m_FailsafeEnable = (Enable != 0) ? TRUE : FALSE;

    // Store timeout and calculate timeout counter if timeout is provided
    if (Timeout > 0)
    {
        pNonIpPort->m_FailsafeTimeout = Timeout;
        pNonIpPort->m_FailsafeTimeoutCnt = (UInt32)DtDivide64((UInt64)Timeout*1000,
                                   pNonIpPort->m_pDvcData->m_DevInfo.m_PerIntItvUS, NULL);
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpTxSetFailsafeAlive -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpTxSetFailsafeAlive(DtaNonIpPort* pNonIpPort)
{
    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_FAILSAFE].m_Value != DT_IOCONFIG_TRUE)
        return DT_STATUS_CONFIG_ERROR;
    
    // Reset failsafe counter
    DtAtomicSet(&pNonIpPort->m_FailsafeCnt, 0);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpTxGetFailsafeInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpTxGetFailsafeInfo(
    DtaNonIpPort*  pNonIpPort,
    Int*  pEnable,
    Int*  pTimeout,
    Int*  pAlive)
{
    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_FAILSAFE].m_Value != DT_IOCONFIG_TRUE)
        return DT_STATUS_CONFIG_ERROR;
    
    // Get failsafe enabled / disabled state
    *pEnable = pNonIpPort->m_FailsafeEnable ? 1 : 0;

    // Get current timeout
    *pTimeout = pNonIpPort->m_FailsafeTimeout;

    // Alive if failsafe counter has not reached the timeout count
    *pAlive = (pNonIpPort->m_FailsafeCnt < pNonIpPort->m_FailsafeTimeoutCnt) ? 1 : 0;

    return DT_STATUS_OK;
}
