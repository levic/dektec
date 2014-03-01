//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NonIpRx.c *#*#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
//
// Dta driver - Non IP RX functionality - Implementation of RX specific functionality for
//                                        non IP ports.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtaIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtaNonIpRxProcessFlags(DtaNonIpPort* pNonIpPort);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpRxIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpRxIoctl(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  NonIpPortIndex;        // Index in the nonip port struct
    DtaIoctlNonIpRxCmdInput* pNonIpRxCmdInput = 
                                         (DtaIoctlNonIpRxCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlNonIpRxCmdOutput* pNonIpRxCmdOutput =
                                       (DtaIoctlNonIpRxCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlNonIpRxCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlNonIpRxCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    if (!DT_SUCCESS(DtaGetNonIpPortIndex(pDvcData, pNonIpRxCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
        return DT_STATUS_INVALID_PARAMETER;
    

    // Determine final required output/input sizes
    switch (pNonIpRxCmdInput->m_Cmd)
    {
    case DTA_NONIP_RX_CMD_GET_FLAGS:
        pCmdStr = "DTA_NONIP_RX_CMD_GET_FLAGS";
        OutReqSize += sizeof(DtaIoctlNonIpRxCmdGetFlagsOutput);
        // We expect no additional data in the input buffer
        InReqSize += 0;
        break;

    case DTA_NONIP_RX_CMD_GET_FREQ_ESTIMATE:
        pCmdStr = "DTA_NONIP_RX_CMD_GET_FREQ_ESTIMATE";
        OutReqSize += sizeof(DtaIoctlNonIpRxCmdGetFreqEstimateOutput);
        // We expect no additional data in the input buffer
        InReqSize += 0;
        break;

    case DTA_NONIP_RX_CMD_CLEAR_FLAGS:
        pCmdStr = "DTA_NONIP_RX_CMD_CLEAR_FLAGS";
        InReqSize += sizeof(DtaIoctlNonIpRxCmdClearFlagsInput);
        // We expect no output buffer at all
        OutReqSize = 0;
        break;

    case DTA_NONIP_RX_CMD_GET_GENREF_STATE:
        pCmdStr = "DTA_NONIP_RX_CMD_GET_GENREF_STATE";
        OutReqSize += sizeof(DtaIoctlNonIpRxCmdGetGenrefStateOutput);
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
        switch (pNonIpRxCmdInput->m_Cmd)
        {
        case DTA_NONIP_RX_CMD_GET_FLAGS:
            Status = DtaNonIpRxGetFlags(&pDvcData->m_pNonIpPorts[NonIpPortIndex],
                                         &pNonIpRxCmdOutput->m_Data.m_GetFlags.m_Status,
                                         &pNonIpRxCmdOutput->m_Data.m_GetFlags.m_Latched);
            break;

        case DTA_NONIP_RX_CMD_GET_FREQ_ESTIMATE:
            Status = DtaNonIpRxGetFreqEstimation(&pDvcData->m_pNonIpPorts[NonIpPortIndex],
                           &pNonIpRxCmdOutput->m_Data.m_GetFreqEstimate.m_FreqEstimation);
            break;

        case DTA_NONIP_RX_CMD_CLEAR_FLAGS:
            Status = DtaNonIpRxClearFlags(&pDvcData->m_pNonIpPorts[NonIpPortIndex],
                                    pNonIpRxCmdInput->m_Data.m_ClearFlags.m_FlagsToClear);
            break;
        case DTA_NONIP_RX_CMD_GET_GENREF_STATE:
            Status = DtaGenlockGetRefState(pDvcData,
                                    NonIpPortIndex,
                                    &pNonIpRxCmdOutput->m_Data.m_GetGenRefState.m_Enabled,
                                    &pNonIpRxCmdOutput->m_Data.m_GetGenRefState.m_InLock);
                                         
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
            DtDbgOut(MIN, NONIP, "NonIpRxCmd=0x%x: NOT SUPPORTED", 
                                                                 pNonIpRxCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, NONIP, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpRxGetFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpRxGetFlags(DtaNonIpPort* pNonIpPort, Int* pStatus, Int* pLatched)
{
    // Update flags
    DtaNonIpRxProcessFlagsFromUser(pNonIpPort);

    // Update DMA pending status
    //if (NonIpRxIsDmaPending(pNonIpPort))
    //    pNonIpPort->m_Flags |= DTA_RX_DMA_PENDING;

    DtSpinLockAcquire(&pNonIpPort->m_FlagsSpinLock);

    // Return flags
    *pStatus = pNonIpPort->m_Flags;
    *pLatched = pNonIpPort->m_FlagsLatched;

    DtSpinLockRelease(&pNonIpPort->m_FlagsSpinLock);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpRxGetFreqEstimation -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpRxGetFreqEstimation(DtaNonIpPort* pNonIpPort, Int* pFreqEstimate)
{
    *pFreqEstimate = DtaNonIpGetEstimatedRate(pNonIpPort);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpRxClearFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpRxClearFlags(DtaNonIpPort* pNonIpPort, Int FlagsToClear)
{
    DtSpinLockAcquire(&pNonIpPort->m_FlagsSpinLock);

    // Clear latched flags
    pNonIpPort->m_Flags &= ~FlagsToClear;
    pNonIpPort->m_FlagsLatched &= ~FlagsToClear;

    // Also clear flags in Receive Status register, to avoid that flags in
    // m_Latched are set again in next periodic interrupt.
    if ((FlagsToClear&DTA_RX_FIFO_OVF) == DTA_RX_FIFO_OVF)
        DtaRegRxStatClrOvfInt(pNonIpPort->m_pRxRegs);
    if ((FlagsToClear&DTA_RX_SYNC_ERR) == DTA_RX_SYNC_ERR)
        DtaRegRxStatClrSyncInt(pNonIpPort->m_pRxRegs);
    if ((FlagsToClear&DTA_RX_RATE_OVF) == DTA_RX_RATE_OVF)
        DtaRegRxStatClrRateOvfInt(pNonIpPort->m_pRxRegs);
  
    DtSpinLockRelease(&pNonIpPort->m_FlagsSpinLock);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpRxProcessFlagsFromDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaNonIpRxProcessFlagsFromDpc(DtaNonIpPort* pNonIpPort)
{
    DtSpinLockAcquireAtDpc(&pNonIpPort->m_FlagsSpinLock);
    DtaNonIpRxProcessFlags(pNonIpPort);
    DtSpinLockReleaseFromDpc(&pNonIpPort->m_FlagsSpinLock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpRxProcessFlagsFromUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaNonIpRxProcessFlagsFromUser(DtaNonIpPort* pNonIpPort)
{
    DtSpinLockAcquire(&pNonIpPort->m_FlagsSpinLock);
    DtaNonIpRxProcessFlags(pNonIpPort);
    DtSpinLockRelease(&pNonIpPort->m_FlagsSpinLock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpRxProcessFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This routine is called from:
// - General periodic DPC to update status flags: DtaNonIpRxProcessFlagsFromDpc
// - DeviceIoCtrl (GetFlags): DtaNonIpRxProcessFlagsFromUser
//
// Pre: Spinlock m_FlagsSpinlock is acquired
//
void  DtaNonIpRxProcessFlags(DtaNonIpPort* pNonIpPort)
{
    Int  Status = 0;
    UInt32  RxStatus;

    RxStatus = DtaRegRxStatGet(pNonIpPort->m_pRxRegs);
    if (RxStatus & DT_RXSTAT_OVFINT_MSK)
    {
        Status |= DTA_RX_FIFO_OVF;
        DtaRegRxStatClrOvfInt(pNonIpPort->m_pRxRegs);
    }
    if (RxStatus & DT_RXSTAT_SYNCINT_MSK)
    {
        Status |= DTA_RX_SYNC_ERR;
        DtaRegRxStatClrSyncInt(pNonIpPort->m_pRxRegs);
    }
    if (RxStatus & DT_RXSTAT_RATEOVFINT_MSK)
    {
        Status |= DTA_RX_RATE_OVF;
        DtaRegRxStatClrRateOvfInt(pNonIpPort->m_pRxRegs);
    }

    // Latch status flags
    pNonIpPort->m_FlagsLatched |= Status;
    pNonIpPort->m_Flags = Status;
}
