//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* EncD7Pro.c *#*#*#*#*#*#*#*#* (C) 2015-2016 DekTec
//
// Dta driver - Implementation of D7 Pro encoder related functions
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2015-2016 DekTec Digital Video B.V.
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
static DtStatus  DtaEncD7ProSendCommand(DtaNonIpPort*  pNonIpPort,
                         UInt8* pBufOut, Int BytesToWrite,
                         UInt8* pBufIn, Int NumBytesToRead, Int* pBytesRead, Int Timeout);
static DtStatus  DtaEncD7ProDebugRead(DtaNonIpPort*  pNonIpPort,
                         UInt8* pBufIn, Int NumBytesToRead, Int* pBytesRead, Int Timeout);
static DtStatus  DtaEncD7ProAckBoot(DtaNonIpPort* pNonIpPort, Int* pStatus);
static DtStatus  DtaEncD7ProReboot(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaEncD7ProResetSet(DtaNonIpPort* pNonIpPort, Int Value);
DtStatus  DtaEncD7ProPrepareSpiMf(DtaNonIpPort* pNonIpPort, DtFileObject*  pFile);
DtStatus  DtaEncD7ProReleaseSpiMf(DtaNonIpPort* pNonIpPort, DtFileObject*  pFile);
static DtStatus  DtaEncD7ProUpdateSourceSelect(DtaNonIpPort* pNonIpPort);


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProInitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProInitPowerup(DtaNonIpPort*  pNonIpPort)
{
    Int Ext12VPresent, PowerGood, i;
    DtaEncD7ProPort*  pEncD7Pro = &pNonIpPort->m_EncD7Pro;
    volatile UInt8* pFwbRegs = pNonIpPort->m_pFwbRegs;

    // Initialize Uart first, we want the register pointers to be always valid
    DtaUartInitPowerup(&pEncD7Pro->m_CtrlUart, pFwbRegs);
    DtaUartInitPowerup(&pEncD7Pro->m_DbgUart, pFwbRegs);
    // Check 12Volt
    Ext12VPresent = DtaFwbRegRead(pFwbRegs, &FwbEncD7ProSerCtrl.Status_Is12VPresent);
    if(!Ext12VPresent)
    {
        // If not present, do we have to retry? 
        // Power-off
        DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_PowerEnable, 0);
        DtDbgOut(ERR, D7PRO, "External 12V supply is not present");
        pEncD7Pro->m_State = DTA_D7PRO_STATE_NO_12V;
        return DT_STATUS_OK;
    }

    // Clear FanFail indication
    DtaFwbRegClear(pFwbRegs, &FwbEncD7ProSerCtrl.Status_FanFail);
    // Assert reset
    DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_Reset, 1); 

    // Power-up
    DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_PowerEnable, 1);

    // Wait maximum 1 second (100x10ms) for good power
    PowerGood = 0;
    for (i=0; i<100 && !PowerGood; i++)
    {
        DtSleep(10);  
        PowerGood = DtaFwbRegRead(pFwbRegs, &FwbEncD7ProSerCtrl.Status_IsPowerGood);
    }
    if(!PowerGood)
    {
        // Power-off
        DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_PowerEnable, 0);
        DtDbgOut(ERR, D7PRO, "Power-up error");
        pEncD7Pro->m_State = DTA_D7PRO_STATE_POWER_ERR;
        return DT_STATUS_OK;
    }

    // Release reset
    DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_Reset, 0); 

    pEncD7Pro->m_State = DTA_D7PRO_STATE_BOOTING;
    
    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpEncD7ProInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProInit(DtaNonIpPort*  pNonIpPort, pDtaNonIpExclusiveAccess 
                                                                          pFuncExclAccess)
{
    DtaEncD7ProPort*  pEncD7Pro = &pNonIpPort->m_EncD7Pro;
    DT_ASSERT(pNonIpPort->m_CapAvEnc);
    
    pEncD7Pro->m_State = DTA_D7PRO_STATE_INIT;
    pEncD7Pro->m_VidStd = DT_VIDSTD_UNKNOWN;
    pEncD7Pro->m_InputSource = 0;
    pEncD7Pro->m_DetVidStd[0] = DT_VIDSTD_UNKNOWN;
    pEncD7Pro->m_DetVidStd[1] = DT_VIDSTD_UNKNOWN;
    pEncD7Pro->m_DetVidStd[2] = DT_VIDSTD_UNKNOWN;
    pEncD7Pro->m_pFuncExclAccess = pFuncExclAccess;

    if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 2180)
    {
        pEncD7Pro->m_SourcePortIndex[0] = 0;
        pEncD7Pro->m_SourcePortIndex[1] = 1;
        pEncD7Pro->m_SourcePortIndex[2] = -1;
    } else {
        return DT_STATUS_FAIL;
    }

    DT_RETURN_ON_ERROR(DtaUartInit(&pEncD7Pro->m_CtrlUart, &FwbEncD7ProSerCtrl.Uarts[0]));
    DT_RETURN_ON_ERROR(DtaUartInit(&pEncD7Pro->m_DbgUart, &FwbEncD7ProSerCtrl.Uarts[1]));
    DT_RETURN_ON_ERROR(DtaSpiMfInit(&pNonIpPort->m_SpiMf, DtaEncD7ProPrepareSpiMf, 
                            DtaEncD7ProReleaseSpiMf, &FwbEncD7ProSerCtrl.SpiMasterFlash));
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaEncD7ProIoctl(
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
    DtaIoctlD7ProCmdInput*  pD7ProCmdInput = 
                                           (DtaIoctlD7ProCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlD7ProCmdOutput*  pD7ProCmdOutput =
                                         (DtaIoctlD7ProCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlD7ProCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlD7ProCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    pNonIpPort = NULL;  // Assume a board level request
    if (!DT_SUCCESS(DtaGetNonIpPortIndex(pDvcData, pD7ProCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
         return DT_STATUS_INVALID_PARAMETER;
    pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpPortIndex];

    if (pDvcData->m_DevInfo.m_TypeNumber != 2180)
        return DT_STATUS_NOT_SUPPORTED;
    
    // Determine final required output/input sizes
    switch (pD7ProCmdInput->m_Cmd)
    {
    case DTA_D7PRO_CMD_SEND_COMMAND:
        pCmdStr = "DTA_D7PRO_CMD_SEND_COMMAND";
        InReqSize += OFFSETOF(DtaIoctlD7ProSendCommandInput, m_Buf); // Exclude m_Buf
        OutReqSize += OFFSETOF(DtaIoctlD7ProSendCommandOutput, m_Buf); // Exclude m_Buf
        // Check for valid input buffer before reading it
        if (pIoctl->m_InputBufferSize >= InReqSize)
        {
            InReqSize += pD7ProCmdInput->m_Data.m_SendCmd.m_NumBytesToWrite;
            OutReqSize += pD7ProCmdInput->m_Data.m_SendCmd.m_NumBytesToRead;
        }
        break;
    
    case DTA_D7PRO_CMD_DEBUG_READ:
        pCmdStr = "DTA_D7PRO_CMD_DEBUG_READ";
        InReqSize += sizeof(DtaIoctlD7ProDebugReadInput);
        OutReqSize += OFFSETOF(DtaIoctlD7ProDebugReadOutput, m_Buf); // Exclude m_Buf
        // Check for valid input buffer before reading it
        if (pIoctl->m_InputBufferSize >= InReqSize)
            OutReqSize += pD7ProCmdInput->m_Data.m_DebugRead.m_NumBytesToRead;
        break;

    case DTA_D7PRO_CMD_STATUS_GET:
        pCmdStr = "DTA_D7PRO_CMD_STATUS_GET";
        // We expect no additional data in the input buffer
        OutReqSize += sizeof(DtaIoctlD7ProStatusGetOutput);
        break;

    case DTA_D7PRO_CMD_ACK_BOOT:
        pCmdStr = "DTA_D7PRO_CMD_ACK_BOOT";
        // We expect no additional data in the input buffer
        OutReqSize += sizeof(DtaIoctlD7ProStatusGetOutput);
        break;

    case DTA_D7PRO_CMD_REBOOT:
        pCmdStr = "DTA_D7PRO_CMD_REBOOT";
        // We expect no additional data in the input or output buffer
        break;
 
    case DTA_D7PRO_CMD_RESET_SET:
        pCmdStr = "DTA_D7PRO_CMD_RESET_SET";
        InReqSize += sizeof(DtaIoctlD7ProResetSetInput);
        // We expect no additional data in the output buffer
        break;

    default:
        pCmdStr = "??UNKNOWN D7PRO_CMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, D7PRO, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, D7PRO, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, D7PRO, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }

    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        switch (pD7ProCmdInput->m_Cmd)
        {
            case DTA_D7PRO_CMD_SEND_COMMAND:
                Status = DtaEncD7ProSendCommand(pNonIpPort,
                                       pD7ProCmdInput->m_Data.m_SendCmd.m_Buf,
                                       pD7ProCmdInput->m_Data.m_SendCmd.m_NumBytesToWrite,
                                       pD7ProCmdOutput->m_Data.m_SendCmd.m_Buf,
                                       pD7ProCmdInput->m_Data.m_SendCmd.m_NumBytesToRead,
                                       &pD7ProCmdOutput->m_Data.m_SendCmd.m_NumBytes,
                                       pD7ProCmdInput->m_Data.m_SendCmd.m_Timeout);
                break;
    
            case DTA_D7PRO_CMD_DEBUG_READ:
                Status = DtaEncD7ProDebugRead(pNonIpPort,
                                      pD7ProCmdOutput->m_Data.m_DebugRead.m_Buf,
                                      pD7ProCmdInput->m_Data.m_DebugRead.m_NumBytesToRead,
                                      &pD7ProCmdOutput->m_Data.m_DebugRead.m_NumBytes,
                                      pD7ProCmdInput->m_Data.m_DebugRead.m_Timeout);
                break;

            case DTA_D7PRO_CMD_STATUS_GET:
                pD7ProCmdOutput->m_Data.m_StatusGet.m_Status =
                                                           pNonIpPort->m_EncD7Pro.m_State;
                break;

            case DTA_D7PRO_CMD_ACK_BOOT:
                Status = DtaEncD7ProAckBoot(pNonIpPort,
                                             &pD7ProCmdOutput->m_Data.m_AckBoot.m_Status);
                break;

            case DTA_D7PRO_CMD_REBOOT:
                Status = DtaEncD7ProReboot(pNonIpPort);
                break;
            
            case DTA_D7PRO_CMD_RESET_SET:
                Status = DtaEncD7ProResetSet(pNonIpPort, 
                                          pD7ProCmdInput->m_Data.m_ResetSet.m_ResetValue);
                break;
        }
    }

    // If we failed, no data has to be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, D7PRO, "D7ProCmd=0x%x: NOT SUPPORTED", pD7ProCmdInput->m_Cmd);
        else if (Status == DT_STATUS_IO_PENDING)
            DtDbgOut(MAX, D7PRO, "%s: ERROR %xh", pCmdStr, Status); // NOT A REAL ERROR
        else
            DtDbgOut(MIN, D7PRO, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProSendCommand -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaEncD7ProSendCommand(
    DtaNonIpPort*  pNonIpPort,
    UInt8*  pBufOut,
    Int BytesToWrite,
    UInt8*  pBufIn,
    Int  NumBytesToRead,
    Int*  pBytesRead,
    Int  Timeout)
{
    DtaEncD7ProPort*  pD7Pro = &pNonIpPort->m_EncD7Pro;
    if (pD7Pro->m_State != DTA_D7PRO_STATE_OK)
        return DT_STATUS_FAIL;
    DT_RETURN_ON_ERROR(DtaUartWrite(&pD7Pro->m_CtrlUart, pBufOut, BytesToWrite,&Timeout));
    return DtaUartRead(&pD7Pro->m_CtrlUart, pBufIn, NumBytesToRead, Timeout, pBytesRead);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProDebugRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProDebugRead(
    DtaNonIpPort*  pNonIpPort,
    UInt8*  pBuf,
    Int  NumBytesToRead,
    Int*  pBytesRead,
    Int  Timeout)
{
    DtaEncD7ProPort*  pD7Pro = &pNonIpPort->m_EncD7Pro;
    return DtaUartRead(&pD7Pro->m_DbgUart, pBuf, NumBytesToRead, Timeout, pBytesRead);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProAttach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProAckBoot(DtaNonIpPort* pNonIpPort, Int* pStatus)
{
    DtaEncD7ProPort*  pD7Pro = &pNonIpPort->m_EncD7Pro;
    UInt8  Buf[3];
    Int  BytesRead = 0;
    DtStatus  Status = DT_STATUS_OK;
    
    *pStatus = pD7Pro->m_State;

    if (pD7Pro->m_State != DTA_D7PRO_STATE_BOOTING)
        return DT_STATUS_OK;

    DtaUartReset(&pD7Pro->m_CtrlUart);
    Status = DtaUartRead(&pD7Pro->m_CtrlUart, Buf, sizeof(Buf), 200, &BytesRead);
    if (DT_SUCCESS(Status) && BytesRead==sizeof(Buf))
    {
        Int  i;
        Int  WriteTimeout = 10;
        for (i=0; i<(Int)sizeof(Buf); i++)
        {
            if (Buf[i] != 0x18)
                return DT_STATUS_OK;
        }
        Buf[0] = 0x15;
        
        Status = DtaUartWrite(&pD7Pro->m_CtrlUart, Buf, 1, &WriteTimeout);
        if (DT_SUCCESS(Status))
        {
            // Give D7Pro time to ack
            DtSleep(100);
            // Reset uart so we're in a consistent state
            DtaUartReset(&pD7Pro->m_CtrlUart);
            *pStatus = pD7Pro->m_State = DTA_D7PRO_STATE_OK;
        }
    }
    
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProReboot -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProReboot(DtaNonIpPort* pNonIpPort)
{
    DtaEncD7ProPort*  pD7Pro = &pNonIpPort->m_EncD7Pro;

    if (pD7Pro->m_State!=DTA_D7PRO_STATE_BOOTING && pD7Pro->m_State!=DTA_D7PRO_STATE_OK)
        return DT_STATUS_FAIL;

    // Reset device and power-up (it attempts to recover from power and fan issues)
    DtaEncD7ProInitPowerup(pNonIpPort);
    if (pD7Pro->m_State != DTA_D7PRO_STATE_BOOTING)
        return DT_STATUS_FAIL;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProResetSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaEncD7ProResetSet(DtaNonIpPort* pNonIpPort, Int Value)
{
    DtaEncD7ProPort*  pD7Pro = &pNonIpPort->m_EncD7Pro;
    
    if (pD7Pro->m_State!=DTA_D7PRO_STATE_BOOTING && pD7Pro->m_State!=DTA_D7PRO_STATE_OK)
        return DT_STATUS_FAIL;

    // Toggle reset bit
    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs, &FwbEncD7ProSerCtrl.Control_Reset, Value);

    // Update state
    if (Value == 1)
        pD7Pro->m_State = DTA_D7PRO_STATE_BOOTING;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProPrepareSpiMf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaEncD7ProPrepareSpiMf(DtaNonIpPort* pNonIpPort, DtFileObject*  pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaEncD7ProPort*  pD7Pro = &pNonIpPort->m_EncD7Pro;
    DT_ASSERT(pNonIpPort->m_CapAvEnc);
    DT_ASSERT(pD7Pro->m_pFuncExclAccess != NULL);

    // Request exclusive access
    Status = pD7Pro->m_pFuncExclAccess(pNonIpPort, pFile, 
                                                        DTA_EXCLUSIVE_ACCESS_CMD_ACQUIRE);
    if (DT_SUCCESS(Status))
    {
        // Assert reset signal
        Status =  DtaEncD7ProResetSet(pNonIpPort, 1);
        if (!DT_SUCCESS(Status))
            pD7Pro->m_pFuncExclAccess(pNonIpPort, pFile, 
                                                        DTA_EXCLUSIVE_ACCESS_CMD_RELEASE);
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProReleaseSpiMf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaEncD7ProReleaseSpiMf(DtaNonIpPort* pNonIpPort, DtFileObject*  pFile)
{
    DtaEncD7ProPort*  pD7Pro = &pNonIpPort->m_EncD7Pro;
    DT_ASSERT(pNonIpPort->m_CapAvEnc);
    DT_ASSERT(pD7Pro->m_pFuncExclAccess != NULL);

    // Release exclusive access
    pD7Pro->m_pFuncExclAccess(pNonIpPort, pFile, DTA_EXCLUSIVE_ACCESS_CMD_RELEASE);

    // Release reset signal
    return DtaEncD7ProResetSet(pNonIpPort, 0);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProUncleanDetach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProUncleanDetach(DtaNonIpPort* pNonIpPort)
{
    DtaEncD7ProPort*  pD7Pro = &pNonIpPort->m_EncD7Pro;

    if (pD7Pro->m_State != DTA_D7PRO_STATE_OK)
        return DT_STATUS_OK;

    // Toggle reset bit
    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs, &FwbEncD7ProSerCtrl.Control_Reset, 1);
    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs, &FwbEncD7ProSerCtrl.Control_Reset, 0);

    // Update state
    pD7Pro->m_State = DTA_D7PRO_STATE_BOOTING;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7proInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaEncD7proInterrupt(DtaNonIpPort* pNonIpPort)
{
    Bool  IrqHandled = FALSE;

    DT_ASSERT(pNonIpPort->m_CapAvEnc);

    IrqHandled |= DtaUartInterrupt(&pNonIpPort->m_EncD7Pro.m_CtrlUart);
    IrqHandled |= DtaUartInterrupt(&pNonIpPort->m_EncD7Pro.m_DbgUart);

    return IrqHandled;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProGetVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProGetVidStd(DtaNonIpPort* pNonIpPort, Int* pVidStd)
{
    DT_ASSERT(pNonIpPort->m_CapAvEnc);

    *pVidStd = pNonIpPort->m_EncD7Pro.m_VidStd;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProSetVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProSetVidStd(DtaNonIpPort* pNonIpPort, Int VidStd)
{
    DT_ASSERT(pNonIpPort->m_CapAvEnc);
    pNonIpPort->m_EncD7Pro.m_VidStd = VidStd;
    return DtaEncD7ProUpdateSourceSelect(pNonIpPort);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProGetSourcePort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProGetSourcePort(DtaNonIpPort* pNonIpPort, Int* pPortIndex)
{
    Int  Source;
    DT_ASSERT(pNonIpPort->m_CapAvEnc);

    Source = pNonIpPort->m_EncD7Pro.m_InputSource;
    if (Source<0 || Source>=3 || pNonIpPort->m_EncD7Pro.m_SourcePortIndex[Source]==-1)
        return DT_STATUS_FAIL;

    *pPortIndex = pNonIpPort->m_EncD7Pro.m_SourcePortIndex[Source];
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProSetSourcePort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProSetSourcePort(DtaNonIpPort* pNonIpPort, Int PortIndex)
{
    Int  Source = -1;
    Int  i;
    DT_ASSERT(pNonIpPort->m_CapAvEnc);
    for (i=0; i<3; i++)
    {
        if (pNonIpPort->m_EncD7Pro.m_SourcePortIndex[i]!=-1 &&
                                 pNonIpPort->m_EncD7Pro.m_SourcePortIndex[i] == PortIndex)
        {
            Source = i;
            break;
        }
    }
    if (Source == -1)
        return DT_STATUS_INVALID_PARAMETER;
    pNonIpPort->m_EncD7Pro.m_InputSource = Source;
    return DtaEncD7ProUpdateSourceSelect(pNonIpPort);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProNewInputVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProNewInputVidStd(DtaNonIpPort* pNonIpPort,
                                                         Int  SourcePortIndex, Int VidStd)
{
    Int  Source = -1;
    Int  i;
    for (i=0; i<3; i++)
    {
        if (pNonIpPort->m_EncD7Pro.m_SourcePortIndex[i]!=-1 &&
                           pNonIpPort->m_EncD7Pro.m_SourcePortIndex[i] == SourcePortIndex)
        {
            Source = i;
            break;
        }
    }
    DT_ASSERT(Source != -1);
    if (Source == -1)
        return DT_STATUS_INVALID_PARAMETER;
    pNonIpPort->m_EncD7Pro.m_DetVidStd[Source] = VidStd;
    return DtaEncD7ProUpdateSourceSelect(pNonIpPort);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProUpdateSourceSelect -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProUpdateSourceSelect(DtaNonIpPort* pNonIpPort)
{
    Int  SourceSelect = 3; // Mute by default
    Int  WantedInput = pNonIpPort->m_EncD7Pro.m_InputSource;
    if (WantedInput>=0 && WantedInput<3
       && pNonIpPort->m_EncD7Pro.m_DetVidStd[WantedInput]==pNonIpPort->m_EncD7Pro.m_VidStd
       && pNonIpPort->m_EncD7Pro.m_VidStd!=DT_VIDSTD_UNKNOWN)
    {
        SourceSelect = WantedInput;
    }
    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs, &FwbEncD7ProSerCtrl.AvSelect_SourceSelect,
                                                                            SourceSelect);
    return DT_STATUS_OK;
}
