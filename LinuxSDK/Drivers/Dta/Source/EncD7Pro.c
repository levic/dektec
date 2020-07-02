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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  POLL_LOOP_SLEEP        100               // Power control poll interval (ms)
#define  WAIT_POWER_ENABLE      3*POLL_LOOP_SLEEP // Time-out powergood after power enable
#define  MIN_FAN_GOOD_CNT       5                 // Threshold before going to init state
#define  MIN_POWER_PRESENT_CNT  5                 // Threshold before going to init state

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtaEncD7ProSendCommand(DtaNonIpPort*  pNonIpPort,
                         UInt8* pBufOut, Int BytesToWrite,
                         UInt8* pBufIn, Int NumBytesToRead, Int* pBytesRead, Int Timeout);
static DtStatus  DtaEncD7ProDebugRead(DtaNonIpPort*  pNonIpPort,
                         UInt8* pBufIn, Int NumBytesToRead, Int* pBytesRead, Int Timeout);
static DtStatus  DtaEncD7ProAckBoot(DtaNonIpPort* pNonIpPort, Bool* pIsBooted);
static DtStatus  DtaEncD7ProReboot(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaEncD7ProPrepareSpiMf(DtaNonIpPort* pNonIpPort, DtFileObject*  pFile);
static DtStatus  DtaEncD7ProReleaseSpiMf(DtaNonIpPort* pNonIpPort, DtFileObject*  pFile);
static DtStatus  DtaEncD7ProUpdateSourceSelect(DtaNonIpPort* pNonIpPort);
static void  DtaEncD7ProPowerControlThread(DtThread* pThread, void* pContext);
static DtStatus  DtaEncD7ProInitPowerControl(DtaNonIpPort*  pNonIpPort);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProInitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProInitPowerup(DtaNonIpPort*  pNonIpPort)
{
    DtaEncD7ProPort*  pEncD7Pro = &pNonIpPort->m_EncD7Pro;
    volatile UInt8* pFwbRegs = pNonIpPort->m_pFwbRegs;

    // Initialize Uart first, we want the register pointers to be always valid
    DtaUartInitPowerup(&pEncD7Pro->m_CtrlUart, pFwbRegs);
    DtaUartInitPowerup(&pEncD7Pro->m_DbgUart, pFwbRegs);

    DT_RETURN_ON_ERROR(DtaEncD7ProInitPowerControl(pNonIpPort));

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProInitPowerControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaEncD7ProInitPowerControl(DtaNonIpPort*  pNonIpPort)
{
    DtaEncD7ProPort*  pEncD7Pro = &pNonIpPort->m_EncD7Pro;

    // Reset device and power-up (it attempts to recover from power and fan issues)
    if (DtMutexAcquire(&pNonIpPort->m_EncD7Pro.m_StatusLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;
    pEncD7Pro->m_State = DTA_D7PRO_STATE_INIT;
    DtMutexRelease(&pNonIpPort->m_EncD7Pro.m_StatusLock);
    DtEventSet(&pNonIpPort->m_EncD7Pro.m_PowerControlEvent);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpEncD7ProInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProInit(DtaNonIpPort*  pNonIpPort, pDtaNonIpExclusiveAccess 
                                                                          pFuncExclAccess)
{
    DtaEncD7ProPort*  pEncD7Pro = &pNonIpPort->m_EncD7Pro;
    DT_ASSERT(pNonIpPort->m_CapAvEnc);
    
    DtMutexInit(&pEncD7Pro->m_StatusLock);
    pEncD7Pro->m_State = DTA_D7PRO_STATE_INIT;
    pEncD7Pro->m_Ext12FailCnt = 0;
    pEncD7Pro->m_FanFailCnt = 0;
    pEncD7Pro->m_PowerFailCnt = 0;

    pEncD7Pro->m_VidStd = DT_VIDSTD_UNKNOWN;

    // Select InputSource based on TypeNumber and PortIndex
    {
        Int  TypeNumber = pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber;
        Int  PortIndex = pNonIpPort->m_PortIndex;

        if(TypeNumber == 2180 || (TypeNumber == 2182 && PortIndex == 4))
            pEncD7Pro->m_InputSource = 0;
        else if(TypeNumber == 2182 && PortIndex == 5)
            pEncD7Pro->m_InputSource = 1;
        else
            return DT_STATUS_FAIL;
    }

    // Both the 2180 and 2182 have two source ports
    pEncD7Pro->m_SourcePortIndex[0] = 0;
    pEncD7Pro->m_SourcePortIndex[1] = 1;
    pEncD7Pro->m_SourcePortIndex[2] = -1;

    pEncD7Pro->m_DetVidStd[0] = DT_VIDSTD_UNKNOWN;
    pEncD7Pro->m_DetVidStd[1] = DT_VIDSTD_UNKNOWN;
    pEncD7Pro->m_DetVidStd[2] = DT_VIDSTD_UNKNOWN;

    pEncD7Pro->m_pFuncExclAccess = pFuncExclAccess;

    DT_RETURN_ON_ERROR(DtaUartInit(&pEncD7Pro->m_CtrlUart, &FwbEncD7ProSerCtrl.Uarts[0]));
    DT_RETURN_ON_ERROR(DtaUartInit(&pEncD7Pro->m_DbgUart, &FwbEncD7ProSerCtrl.Uarts[1]));
    DT_RETURN_ON_ERROR(DtaSpiMfInit(&pNonIpPort->m_SpiMf, DtaEncD7ProPrepareSpiMf, 
                            DtaEncD7ProReleaseSpiMf, &FwbEncD7ProSerCtrl.SpiMasterFlash));

    // Init power controller thread
    DT_RETURN_ON_ERROR(DtThreadInit(&pEncD7Pro->m_PowerControlThread, 
                                              DtaEncD7ProPowerControlThread, pNonIpPort));
    DtEventInit(&pEncD7Pro->m_PowerControlEvent, TRUE);
    DtEventInit(&pEncD7Pro->m_PowerEnabledEvent, FALSE);

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

    if (pDvcData->m_DevInfo.m_TypeNumber!=2180 && pDvcData->m_DevInfo.m_TypeNumber!=2182)
        return DT_STATUS_NOT_SUPPORTED;
    if (!pNonIpPort->m_CapAvEnc)
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

    case DTA_D7PRO_CMD_REBOOT:
        pCmdStr = "DTA_D7PRO_CMD_REBOOT";
        // We expect no additional data in the input or output buffer
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
                if (DtMutexAcquire(&pNonIpPort->m_EncD7Pro.m_StatusLock, 100) 
                                                                          != DT_STATUS_OK)
                {
                    Status = DT_STATUS_TIMEOUT;
                    break;
                }

                Status = DT_STATUS_OK;
                pD7ProCmdOutput->m_Data.m_StatusGet.m_State =
                                                           pNonIpPort->m_EncD7Pro.m_State;
                pD7ProCmdOutput->m_Data.m_StatusGet.m_Ext12FailCnt =
                                                    pNonIpPort->m_EncD7Pro.m_Ext12FailCnt;
                pD7ProCmdOutput->m_Data.m_StatusGet.m_FanFailCnt =
                                                      pNonIpPort->m_EncD7Pro.m_FanFailCnt;
                pD7ProCmdOutput->m_Data.m_StatusGet.m_PowerFailCnt =
                                                    pNonIpPort->m_EncD7Pro.m_PowerFailCnt;
                DtMutexRelease(&pNonIpPort->m_EncD7Pro.m_StatusLock);
                break;

            case DTA_D7PRO_CMD_REBOOT:
                Status = DtaEncD7ProReboot(pNonIpPort);
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
    Int  State;
    
    if (DtMutexAcquire(&pNonIpPort->m_EncD7Pro.m_StatusLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;
    State = pD7Pro->m_State;
    DtMutexRelease(&pNonIpPort->m_EncD7Pro.m_StatusLock);

    if (State != DTA_D7PRO_STATE_OPERATIONAL)
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProAckBoot .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Pre-conditions: D7Pro is powered, currently booting, but not yet not booted
//
DtStatus  DtaEncD7ProAckBoot(DtaNonIpPort* pNonIpPort, Bool* pIsBooted)
{
    DtaEncD7ProPort*  pD7Pro = &pNonIpPort->m_EncD7Pro;
    UInt8  Buf[3];
    Int  BytesRead = 0;
    DtStatus  Status = DT_STATUS_OK;
    *pIsBooted = FALSE;

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
            *pIsBooted = TRUE;
        }
    }
    
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProReboot -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProReboot(DtaNonIpPort* pNonIpPort)
{
    DT_RETURN_ON_ERROR(DtaEncD7ProInitPowerControl(pNonIpPort));

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProPowerdownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaEncD7ProPowerdownPre(DtaNonIpPort* pNonIpPort)
{
    DtaEncD7ProPort*  pD7Pro = &pNonIpPort->m_EncD7Pro;

    // Stop power control thread
    DtEventSet(&pNonIpPort->m_EncD7Pro.m_PowerControlEvent);
    DT_RETURN_ON_ERROR(DtThreadStop(&pD7Pro->m_PowerControlThread));

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProPowerUpPost -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaEncD7ProPowerUpPost(DtaNonIpPort* pNonIpPort)
{
    DtaEncD7ProPort*  pD7Pro = &pNonIpPort->m_EncD7Pro;

    // Start power controller thread
    DT_RETURN_ON_ERROR(DtThreadStart(&pD7Pro->m_PowerControlThread));

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
        // Assert reset signal and enable power
        if (DtMutexAcquire(&pNonIpPort->m_EncD7Pro.m_StatusLock, 100) != DT_STATUS_OK)
        {
            pD7Pro->m_pFuncExclAccess(pNonIpPort, pFile, 
                                                        DTA_EXCLUSIVE_ACCESS_CMD_RELEASE);
            return DT_STATUS_TIMEOUT;
        }
        pD7Pro->m_State = DTA_D7PRO_STATE_FLASH_PROG;
        // Reset event before release StatusLock mutex to prevent any chance of
        // racing with the power control thread
        DtEventReset(&pNonIpPort->m_EncD7Pro.m_PowerEnabledEvent);
        DtMutexRelease(&pNonIpPort->m_EncD7Pro.m_StatusLock);

        // Wait for reset and power to be enabled
        DtEventSet(&pNonIpPort->m_EncD7Pro.m_PowerControlEvent);
        Status = DtEventWait(&pNonIpPort->m_EncD7Pro.m_PowerEnabledEvent, 
                                                                       WAIT_POWER_ENABLE);
        if (!DT_SUCCESS(Status))
        {
            DtaEncD7ProReleaseSpiMf(pNonIpPort, pFile);
            return DT_STATUS_TIMEOUT;
        }
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
    DT_RETURN_ON_ERROR(DtaEncD7ProInitPowerControl(pNonIpPort));

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProUncleanDetach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEncD7ProUncleanDetach(DtaNonIpPort* pNonIpPort)
{
    DT_RETURN_ON_ERROR(DtaEncD7ProInitPowerControl(pNonIpPort));

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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProPowerControlThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaEncD7ProPowerControlThread(DtThread* pThread, void* pContext)
{
    // Variables
    DtaNonIpPort*  pNonIpPort = (DtaNonIpPort*)pContext;
    DtaEncD7ProPort*  pD7Pro = &pNonIpPort->m_EncD7Pro;
    Int  IsExt12VPresent, IsPowerGood, FanFail, Ext12VAbsent, PowerFail;
    Int  Ext12PresCnt = 0, FanGoodCnt = 0;
    UInt32  MeasuredRotationRate;
    UInt32  MinFanRotation;
    volatile UInt8* pFwbRegs = pNonIpPort->m_pFwbRegs;
    Int FanType = pNonIpPort->m_pDvcData->m_FanControl.m_FanType;
    Int RpmDiv = pNonIpPort->m_pDvcData->m_FanControl.m_RpmDiv;
    Int RpmMult = pNonIpPort->m_pDvcData->m_FanControl.m_RpmMult;
    Int RpmMinimum = pNonIpPort->m_pDvcData->m_FanControl.m_RpmMinimum;
    volatile UInt8* pFanRegs = pNonIpPort->m_pDvcData->m_FanControl.m_pFwbFanRegs;


    // Initialize Fan variables / Fan controller
    DtMutexAcquire(&pNonIpPort->m_EncD7Pro.m_StatusLock, -1);
    if (FanType == FAN_TYPE_FANM)       // Monitored Fan (e.g. Dta 2180)
    {
        // Get the minimum acceptable observed fan rotation rpm
        if (RpmMinimum > 0)
        {
            MinFanRotation = (UInt32)((RpmMinimum * RpmDiv) / RpmMult);
            DtaFwbRegWrite(pFanRegs, &FwbFanController.Control_MinimumRotationRate,
                                                                          MinFanRotation);
        }
        else
            MinFanRotation = DtaFwbRegRead(pFanRegs,
                                              &FwbFanMonitor.Control_MinimumRotationRate);

    }
    else if (FanType == FAN_TYPE_FANC)  // Controlled Fan (e.g. Dta 2182)
    {
        // The controllable fan rotation rate is specified in software.  0x100 should
        // result in a MeasuredRotationRate of roughly 0x13E0.
        //
        const UInt32 CurrentFanRate = 0x350;
        DtaFwbRegWrite(pFanRegs, &FwbFanController.Control_CurrentFanRate,
                                                                          CurrentFanRate);

        // The minimum acceptable observed fan rotation rpm is specified in software.  The
        // firmware monitors the fan speed and considers an observed rotation rate below
        // this threshold to be a failure and cuts power after a period of time.  The
        // driver also monitors the rotation rate and decides failure more quickly than
        // the firmware (perhaps 5 seconds) in an attempt to shut down gracefully.
        //
        MinFanRotation = 0x1000;
        if (RpmMinimum > 0)
            MinFanRotation = (UInt32)((RpmMinimum * RpmDiv) / RpmMult);
        DtaFwbRegWrite(pFanRegs, &FwbFanController.Control_MinimumRotationRate,
                                                                          MinFanRotation);
    }
    else
        DT_ASSERT(0);
    DtMutexRelease(&pNonIpPort->m_EncD7Pro.m_StatusLock);
    
    // Power control state machine
    while (!pThread->m_StopThread)
    {
        // Don't poll to often and exit sleep when we need to do something
        DtEventWait(&pNonIpPort->m_EncD7Pro.m_PowerControlEvent, POLL_LOOP_SLEEP);
        if (pThread->m_StopThread)
            continue;

        // Get mutex so we can change all status variables
        if (DtMutexAcquire(&pNonIpPort->m_EncD7Pro.m_StatusLock, 0) != DT_STATUS_OK)
            continue;

        switch (pD7Pro->m_State)
        {
        case DTA_D7PRO_STATE_INIT:
            // Assert Magnum reset
            DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_Reset, 1);

            // Get status from hardware
            IsExt12VPresent = DtaFwbRegRead(pFwbRegs,
                                                 &FwbEncD7ProSerCtrl.Status_Is12VPresent);
            IsPowerGood = DtaFwbRegRead(pFwbRegs, &FwbEncD7ProSerCtrl.Status_IsPowerGood);
            if (FanType == FAN_TYPE_FANM)
                MeasuredRotationRate = DtaFwbRegRead(pFanRegs, 
                                              &FwbFanMonitor.Status_MeasuredRotationRate);
            else
                MeasuredRotationRate = DtaFwbRegRead(pFanRegs, 
                                           &FwbFanController.Status_MeasuredRotationRate);
            // Apply RPM correction factors
            MeasuredRotationRate = (MeasuredRotationRate*RpmMult) / RpmDiv;

            // Is 12V absent? Wait for 12V presence in EXT_12_FAIL state
            if (!IsExt12VPresent)
            {
                // Increase error counter
                pD7Pro->m_Ext12FailCnt++;

                // Disable Magnum power
                DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_PowerEnable, 0);

                // Goto EXT_12_FAIL state
                Ext12PresCnt = 0;
                pD7Pro->m_State = DTA_D7PRO_STATE_NO_12V;
            }
            else if (MeasuredRotationRate < MinFanRotation)
            {
                // Increase error counter
                pD7Pro->m_FanFailCnt++;

                // Disable Magnum power
                DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_PowerEnable, 0);

                // Goto FAN_FAIL state
                FanGoodCnt = 0;
                pD7Pro->m_State = DTA_D7PRO_STATE_FAN_FAIL;
            } else {
                // Enable Magnum power
                DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_PowerEnable, 1);
                
                // Clear status bits
                DtaFwbRegClear(pFwbRegs, &FwbEncD7ProSerCtrl.Status_FanFail);
                DtaFwbRegClear(pFwbRegs, &FwbEncD7ProSerCtrl.Status_Ext12VAbsent);
                DtaFwbRegClear(pFwbRegs, &FwbEncD7ProSerCtrl.Status_PowerFail);

                // Wait for the power good signal to assert before going to init state
                if (IsPowerGood)
                {
                    // De-assert Magnum reset
                    DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_Reset, 0);

                    // Goto booting state
                    pD7Pro->m_State = DTA_D7PRO_STATE_BOOTING;
                }
            }
            break;

        case DTA_D7PRO_STATE_FLASH_PROG:
            // Flash programming in progress: Assert Magnum reset, enable power and 
            // wait for finish
            DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_Reset, 1);
            DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_PowerEnable, 1);

            // Signal power good
            IsPowerGood = DtaFwbRegRead(pFwbRegs, &FwbEncD7ProSerCtrl.Status_IsPowerGood);
            if (IsPowerGood)
                DtEventSet(&pNonIpPort->m_EncD7Pro.m_PowerEnabledEvent);

            break;

        case DTA_D7PRO_STATE_NO_12V:
            // Get status from hardware
            IsExt12VPresent = DtaFwbRegRead(pFwbRegs,&FwbEncD7ProSerCtrl.Status_Is12VPresent);

            // Check power present and count up to threshold before going to init state
            if (IsExt12VPresent)
                Ext12PresCnt += 1;
            else
                Ext12PresCnt = 0;
            if (Ext12PresCnt > MIN_POWER_PRESENT_CNT)
                pD7Pro->m_State = DTA_D7PRO_STATE_INIT;

            break;

            // Although the firmware has its own failure detector, the driver detects more
            // quickly in an attempt to preemptively enter this safe state before the
            // D7PRO is powered down by the firmware.
            //
        case DTA_D7PRO_STATE_FAN_FAIL:
            // Read fan rotation from hardware
            if (FanType == FAN_TYPE_FANM)
                MeasuredRotationRate = DtaFwbRegRead(pFanRegs, 
                                              &FwbFanMonitor.Status_MeasuredRotationRate);
            else
                MeasuredRotationRate = DtaFwbRegRead(pFanRegs, 
                                           &FwbFanController.Status_MeasuredRotationRate);
            // Apply RPM correction factors
            MeasuredRotationRate = (MeasuredRotationRate*RpmMult) / RpmDiv;

            // Check fan speed and count up to threshold before going to init state
            if (MeasuredRotationRate >= MinFanRotation)
                FanGoodCnt += 1;
            else
                FanGoodCnt = 0;
            if (FanGoodCnt > MIN_FAN_GOOD_CNT)
                pD7Pro->m_State = DTA_D7PRO_STATE_INIT;

            break;

        case DTA_D7PRO_STATE_BOOTING:
        case DTA_D7PRO_STATE_OPERATIONAL:
            // Get status from hardware
            FanFail = DtaFwbRegRead(pFwbRegs, &FwbEncD7ProSerCtrl.Status_FanFail);
            Ext12VAbsent = DtaFwbRegRead(pFwbRegs, 
                                                 &FwbEncD7ProSerCtrl.Status_Ext12VAbsent);
            PowerFail = DtaFwbRegRead(pFwbRegs, &FwbEncD7ProSerCtrl.Status_PowerFail);

            // Is 12V absent? Wait for 12V presence in EXT_12_FAIL state
            if (Ext12VAbsent)
            {
                // Increase error counter
                pD7Pro->m_Ext12FailCnt++;

                // Assert Magnum reset and disable Magnum power
                DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_Reset, 1);
                DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_PowerEnable, 0);

                // Goto EXT_12_FAIL state
                Ext12PresCnt = 0;
                pD7Pro->m_State = DTA_D7PRO_STATE_NO_12V;
            }
            // Is the fan failing? Wait for the fan to operate correctly in FAN_FAIL state
            else if (FanFail)
            {
                // Increase error counter
                pD7Pro->m_FanFailCnt++;

                // Assert Magnum reset and disable Magnum power
                DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_Reset, 1);
                DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_PowerEnable, 0);

                // Goto FAN_FAIL state
                FanGoodCnt = 0;
                pD7Pro->m_State = DTA_D7PRO_STATE_FAN_FAIL;
            }
            // Is the power failing? Wait for the power to operate correctly in init state
            else if (PowerFail)
            {
                // Increase error counter and goto init state
                pD7Pro->m_PowerFailCnt++;
                pD7Pro->m_State = DTA_D7PRO_STATE_INIT;
            }

            // Acknolegde Magnum boot signal
            if (pD7Pro->m_State == DTA_D7PRO_STATE_BOOTING)
            {
                Bool  IsBooted;

                // Release mutex so we can call DtaEncD7ProAckBoot
                DtMutexRelease(&pNonIpPort->m_EncD7Pro.m_StatusLock);

                // Acknolegde Magnum boot signal
                DtaEncD7ProAckBoot(pNonIpPort, &IsBooted);

                // Get mutex again so we can change m_State
                DtMutexAcquire(&pNonIpPort->m_EncD7Pro.m_StatusLock, -1);

                if (IsBooted)
                    pD7Pro->m_State = DTA_D7PRO_STATE_OPERATIONAL;
            }
            break;

        default:
            DT_ASSERT(0);
        }

        // Release the mutex
        DtMutexRelease(&pNonIpPort->m_EncD7Pro.m_StatusLock);
    }

    // Assert Magnum reset and disable Magnum power
    DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_Reset, 1);
    DtaFwbRegWrite(pFwbRegs, &FwbEncD7ProSerCtrl.Control_PowerEnable, 0);
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
