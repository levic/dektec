//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* EnDec.c *#*#*#*#*#*#*#*#*#* (C) 2015-2016 DekTec
//
// Dta driver - Implementation of decoder/encoder common functions
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
static DtStatus  DtaEnDecExclusiveAccess(DtaNonIpPort* pNonIpPort,
                                                            DtFileObject* pFile, Int Cmd);
static DtStatus  DtaEnDecGetVidStd(DtaNonIpPort* pNonIpPort, Int* pVidStd);
static DtStatus  DtaEnDecSetVidStd(DtaNonIpPort* pNonIpPort, Int VidStd);
static DtStatus  DtaEnDecGetSourcePort(DtaNonIpPort* pNonIpPort, Int* pPortIndex);
static DtStatus  DtaEnDecSetSourcePort(DtaNonIpPort* pNonIpPort, Int PortIndex);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncoderInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaEncoderInit(DtaDeviceData*  pDvcData)
{
    if (pDvcData->m_DevInfo.m_TypeNumber == 2180)
    {
        DtaNonIpSdiAvRxSetNewVidStdCb(&pDvcData->m_pNonIpPorts[0],
                                  DtaEncD7ProNewInputVidStd, &pDvcData->m_pNonIpPorts[3]);
        DtaNonIpHdmiSetNewVidStdCb(&pDvcData->m_pNonIpPorts[1],
                                  DtaEncD7ProNewInputVidStd, &pDvcData->m_pNonIpPorts[3]);
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEnDecIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEnDecIoctl(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl,
    Bool  PowerDownPending)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  NonIpPortIndex;        // Index in the nonip port struct
    DtaNonIpPort*  pNonIpPort = NULL;
    DtaIoctlEnDecCmdInput*  pEnDecCmdInput = 
                                           (DtaIoctlEnDecCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlEnDecCmdOutput*  pEnDecCmdOutput =
                                         (DtaIoctlEnDecCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlEnDecCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlEnDecCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    pNonIpPort = NULL;  // Assume a board level request
    if (!DT_SUCCESS(DtaGetNonIpPortIndex(pDvcData, pEnDecCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
         return DT_STATUS_INVALID_PARAMETER;
    pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpPortIndex];

    if (!pNonIpPort->m_CapAvEnc)
        return DT_STATUS_NOT_SUPPORTED;
    
    // Determine final required output/input sizes
    switch (pEnDecCmdInput->m_Cmd)
    {
    case DTA_ENDEC_CMD_EXCLUSIVE_ACCESS:
        pCmdStr = "DTA_ENDEC_CMD_EXCLUSIVE_ACCESS";
        InReqSize += sizeof(DtaIoctlNonIpCmdExclusiveAccessInput);
        OutReqSize = 0;
        break;
        
    case DTA_ENDEC_CMD_GET_SOURCE_PORT:
        pCmdStr = "DTA_ENDEC_CMD_GET_SOURCE_PORT";
        InReqSize = 0;
        OutReqSize += sizeof(DtaIoctEnDecCmdGetSourcePortOutput);
        break;

    case DTA_ENDEC_CMD_SET_SOURCE_PORT:
        pCmdStr = "DTA_ENDEC_CMD_SET_SOURCE_PORT";
        InReqSize += sizeof(DtaIoctEnDecCmdSetSourcePortInput);
        OutReqSize = 0;
        break;
        
    case DTA_ENDEC_CMD_GET_VIDSTD:
        pCmdStr = "DTA_ENDEC_CMD_GET_VIDSTD";
        InReqSize = 0;
        OutReqSize += sizeof(DtaIoctEnDecCmdGetVidStdOutput);
        break;

    case DTA_ENDEC_CMD_SET_VIDSTD:
        pCmdStr = "DTA_ENDEC_CMD_SET_VIDSTD";
        InReqSize += sizeof(DtaIoctEnDecCmdSetVidStdInput);
        OutReqSize = 0;
        break;

    default:
        pCmdStr = "??UNKNOWN ENDEC_CMD CODE??";
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
        switch (pEnDecCmdInput->m_Cmd)
        {
        case DTA_ENDEC_CMD_EXCLUSIVE_ACCESS:
            Status = DtaEnDecExclusiveAccess(pNonIpPort, pFile,
                                          pEnDecCmdInput->m_Data.m_ExclusiveAccess.m_Cmd);
            break;

        case DTA_ENDEC_CMD_GET_SOURCE_PORT:
            Status = DtaEnDecGetSourcePort(pNonIpPort,
                                    &pEnDecCmdOutput->m_Data.m_GetSourcePort.m_PortIndex);
            break;

        case DTA_ENDEC_CMD_SET_SOURCE_PORT:
            Status = DtaEnDecSetSourcePort(pNonIpPort,
                                      pEnDecCmdInput->m_Data.m_SetSourcePort.m_PortIndex);
            break;

        case DTA_ENDEC_CMD_GET_VIDSTD:
            Status = DtaEnDecGetVidStd(pNonIpPort,
                                           &pEnDecCmdOutput->m_Data.m_GetVidStd.m_VidStd);
            break;

        case DTA_ENDEC_CMD_SET_VIDSTD:
            Status = DtaEnDecSetVidStd(pNonIpPort,
                                             pEnDecCmdInput->m_Data.m_SetVidStd.m_VidStd);
            break;
        }
    }

    // If we failed, no data has to be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, D7PRO, "EnDecCmd=0x%x: NOT SUPPORTED", pEnDecCmdInput->m_Cmd);
        else if (Status == DT_STATUS_IO_PENDING)
            DtDbgOut(MAX, D7PRO, "%s: ERROR %xh", pCmdStr, Status); // NOT A REAL ERROR
        else
            DtDbgOut(MIN, D7PRO, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEnDecInitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaEnDecInitPowerup(DtaNonIpPort* pNonIpPort)
{
    if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 2180)
        return DtaEncD7ProInitPowerup(pNonIpPort);

    return DT_STATUS_NOT_SUPPORTED;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEnDecInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEnDecInit(DtaNonIpPort* pNonIpPort)
{
    DtaEnDecPort*  pEnDec = &pNonIpPort->m_EnDec;

    // Initialize common data first
    pEnDec->m_ExclAccess = FALSE;
    DtSpinLockInit(&pEnDec->m_ExclAccessLock);

    // Defer to encoder/decoder specific implementation
    if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 2180)
        return DtaEncD7ProInit(pNonIpPort, DtaEnDecExclusiveAccess);

    return DT_STATUS_NOT_SUPPORTED;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProExclusiveAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaEnDecExclusiveAccess(
    DtaNonIpPort*  pNonIpPort,
    DtFileObject*  pFile,
    Int  Cmd)
{
    DtStatus  Result = DT_STATUS_OK;
    DtaEnDecPort*  pEnDec = &pNonIpPort->m_EnDec;

    DtSpinLockAcquire(&pEnDec->m_ExclAccessLock);

    if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_ACQUIRE)
    {
        if (!pEnDec->m_ExclAccess)
        {
            pEnDec->m_ExclAccess = TRUE;
            pEnDec->m_ExclAccessOwner = *pFile;
        } else
            Result = DT_STATUS_IN_USE;
    } else if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_RELEASE)
    {
        if (pEnDec->m_ExclAccess)
        {
            if (DtFileCompare(&pEnDec->m_ExclAccessOwner, pFile))
            {
                pEnDec->m_ExclAccess = FALSE;
            }
            else
                Result = DT_STATUS_IN_USE;
        }
    } else if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_PROBE)
    {
        if (pEnDec->m_ExclAccess)
            Result = DT_STATUS_IN_USE;
    } else
        Result = DT_STATUS_NOT_SUPPORTED;
    
    DtSpinLockRelease(&pEnDec->m_ExclAccessLock);

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEnDecGetVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEnDecGetVidStd(DtaNonIpPort* pNonIpPort, Int* pVidStd)
{
    // Defer to encoder/decoder specific implementation
    if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 2180)
        return DtaEncD7ProGetVidStd(pNonIpPort, pVidStd);

    return DT_STATUS_NOT_SUPPORTED;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEnDecSetVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEnDecSetVidStd(DtaNonIpPort* pNonIpPort, Int VidStd)
{
    // Defer to encoder/decoder specific implementation
    if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 2180)
        return DtaEncD7ProSetVidStd(pNonIpPort, VidStd);

    return DT_STATUS_NOT_SUPPORTED;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEnDecGetSourcePort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEnDecGetSourcePort(DtaNonIpPort* pNonIpPort, Int* pPortIndex)
{
    // Defer to encoder/decoder specific implementation
    if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 2180)
        return DtaEncD7ProGetSourcePort(pNonIpPort, pPortIndex);

    return DT_STATUS_NOT_SUPPORTED;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEnDecSetSourcePort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEnDecSetSourcePort(DtaNonIpPort* pNonIpPort, Int PortIndex)
{
    // Defer to encoder/decoder specific implementation
    if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 2180)
        return DtaEncD7ProSetSourcePort(pNonIpPort, PortIndex);

    return DT_STATUS_NOT_SUPPORTED;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEnDecClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEnDecClose(DtaNonIpPort* pNonIpPort, DtFileObject* pFile)
{
    DtaEnDecPort*  pEnDec = &pNonIpPort->m_EnDec;

    DtSpinLockAcquire(&pEnDec->m_ExclAccessLock);

    if (!pEnDec->m_ExclAccess || !DtFileCompare(&pEnDec->m_ExclAccessOwner, pFile))
    {
        DtSpinLockRelease(&pEnDec->m_ExclAccessLock);
        return DT_STATUS_OK;
    }
    
    if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 2180)
        DtaEncD7ProUncleanDetach(pNonIpPort);

    pEnDec->m_ExclAccess = FALSE;

    DtSpinLockRelease(&pEnDec->m_ExclAccessLock);

    return DT_STATUS_OK;
}

