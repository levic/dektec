//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaMatrix.c *#*#*#*#*#*#*#*#*#*#*# (C) 2012 DekTec
//
// Dta driver - Implements Matrix-API (i.e. HD-SDI) related functions
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012 DekTec Digital Video B.V.
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
void  DtaMatrixSofFrameIntDpc(DtDpcArgs* pArgs);
DtStatus  DtaMatrixSyncInfoGet(DtaDeviceData*  pDvcData, 
                                              DtaIoctlMatrixCmdGetSyncInfoOutput*  pInfo);
DtStatus  DtaMatrixWaitFrame(DtaDeviceData*  pDvcData, DtaNonIpPort*  pNonIpPort,
                                          Int64  WaitFrame, Int  Timeout, Int64*  pFrame);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMatrixInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// NOTE: call init after having initialised ports (SW-init)
//
DtStatus  DtaMatrixInit(DtaDeviceData*  pDvcData)
{
    Int  i;
    DtStatus  Status = DT_STATUS_OK;

    // Matrix-API is supported if we have at-least one port with the MATRIX capability
    pDvcData->m_Matrix.m_IsSupported = FALSE;
    for (i=0; i<pDvcData->m_NumNonIpPorts && !pDvcData->m_Matrix.m_IsSupported; i++)
        pDvcData->m_Matrix.m_IsSupported = pDvcData->m_NonIpPorts[i].m_CapMatrix;
    
    pDvcData->m_Matrix.m_SofFrame = 0;

    pDvcData->m_Matrix.m_SyncInfo.m_IsValid = FALSE;
    DtSpinLockInit(&pDvcData->m_Matrix.m_SyncInfoSpinLock);

    DtDpcInit(&pDvcData->m_Matrix.m_SofFrameIntDpc, DtaMatrixSofFrameIntDpc, TRUE);
    DtEventInit(&pDvcData->m_Matrix.m_SofFrameIntEvent, FALSE);
    DtEventInit(&pDvcData->m_Matrix.m_SofFrameSyncEvent, FALSE);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMatrixIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaMatrixIoctl(
    DtaDeviceData* pDvcData,
    DtFileObject* pFile,
    DtIoctlObject* pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  NonIpPortIndex;        // Index in the nonip port struct
    DtaNonIpPort*  pNonIpPort = NULL;
    DtaIoctlMatrixCmdInput* pMatrixCmdInput = 
                                         (DtaIoctlMatrixCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlMatrixCmdOutput* pMatrixCmdOutput =
                                       (DtaIoctlMatrixCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlMatrixCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlMatrixCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    pNonIpPort = NULL;  // Assume a board level request
    if (pMatrixCmdInput->m_PortIndex == -1)
        NonIpPortIndex = -1;    // Is a board level request
    else if (DT_SUCCESS(DtaGetNonIpPortIndex(pDvcData, pMatrixCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
        pNonIpPort = &pDvcData->m_NonIpPorts[NonIpPortIndex];
    else
         return DT_STATUS_INVALID_PARAMETER;
           
    // Determine final required output/input sizes
    switch (pMatrixCmdInput->m_Cmd)
    {
    case DTA_MATRIX_CMD_WAIT_FRAME:
        pCmdStr = "DTA_MATRIX_CMD_WAIT_FRAME";
        InReqSize += sizeof(DtaIoctlMatrixCmdWaitFrameInput);
        OutReqSize += sizeof(DtaIoctlMatrixCmdWaitFrameOutput);
        
        break;

    case DTA_MATRIX_CMD_GET_SYNC_INFO:
        pCmdStr = "DTA_MATRIX_CMD_GET_SYNC_INFO";
        OutReqSize += sizeof(DtaIoctlMatrixCmdGetSyncInfoOutput);
        // We expect no additional data in the input buffer
        InReqSize += 0;
        break;

    default:
        pCmdStr = "??UNKNOWN MATRIXCMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, MATRIX, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, MATRIX, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, MATRIX, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pMatrixCmdInput->m_Cmd)
        {
        case DTA_MATRIX_CMD_WAIT_FRAME:
            Status = DtaMatrixWaitFrame(pDvcData, pNonIpPort, 
                                        pMatrixCmdInput->m_Data.m_WaitFrame.m_Frame,
                                        pMatrixCmdInput->m_Data.m_WaitFrame.m_Timeout,
                                           &pMatrixCmdOutput->m_Data.m_WaitFrame.m_Frame);
            break;

        case DTA_MATRIX_CMD_GET_SYNC_INFO:
            Status = DtaMatrixSyncInfoGet(pDvcData, &pMatrixCmdOutput->m_Data.m_SyncInfo);
            break;

        default:
            Status = DT_STATUS_NOT_SUPPORTED;
        }
    }

    // If we failed, no data has to be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, MATRIX, "MatrixCmd=0x%x: NOT SUPPORTED", 
                                                                 pMatrixCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, MATRIX, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMatrixSyncInfoGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaMatrixSyncInfoGet(
    DtaDeviceData*  pDvcData,
    DtaIoctlMatrixCmdGetSyncInfoOutput*  pInfo)
{
    DT_ASSERT(pDvcData->m_Matrix.m_IsSupported);

    // Get lock, before copying info
    DtSpinLockAcquire(&pDvcData->m_Matrix.m_SyncInfoSpinLock);
    *pInfo = pDvcData->m_Matrix.m_SyncInfo;
    DtSpinLockRelease(&pDvcData->m_Matrix.m_SyncInfoSpinLock);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMatrixWaitFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaMatrixWaitFrame(
    DtaDeviceData*  pDvcData,
    DtaNonIpPort*  pNonIpPort,
    Int64  WaitFrame, 
    Int  Timeout,
    Int64*  pFrame)
{
    DtStatus  Status = DT_STATUS_OK;
    DtEvent* pWaitEv = NULL;
    static const Int  MAX_WAIT_TIME_MS = 100;
    volatile Int64*  pSofOrLastFrame = NULL;

    DT_ASSERT(pDvcData != NULL);

    if (pNonIpPort == NULL)
    {
        pWaitEv = &pDvcData->m_Matrix.m_SofFrameIntEvent;
        pSofOrLastFrame = &pDvcData->m_Matrix.m_SofFrame;
    }
    else
    {
        pWaitEv = &pNonIpPort->m_Matrix.m_LastFrameIntEvent;
        pSofOrLastFrame = &pNonIpPort->m_Matrix.m_LastFrame;
    }

    // Clear the event in case we have a previous unhandled event
    DtEventReset(pWaitEv);

    // -1 indicates we wait for the next frame
    if (WaitFrame == -1)
        WaitFrame = *pSofOrLastFrame + 1;  // Simply wait for next frame

    while (WaitFrame>*pSofOrLastFrame && Timeout>0)
    {
        Status = DtEventWait(pWaitEv, MAX_WAIT_TIME_MS);
        if (Status==DT_STATUS_TIMEOUT && Timeout<=MAX_WAIT_TIME_MS)
        {
            *pFrame = -1;   // Set frame to -1 to signal timeout
            return DT_STATUS_TIMEOUT;
        }
        else if (Status == DT_STATUS_TIMEOUT)
            Timeout -= MAX_WAIT_TIME_MS;
        else
            Timeout -= 40;  // subtract the typical frame rate
    } 
    
    // Return the current frame
    *pFrame = *pSofOrLastFrame;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMatrixSofFrameIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaMatrixSofFrameIntDpc(DtDpcArgs* pArgs)
{
    Int  i, n;

    DtaDeviceData*  pDvcData = (DtaDeviceData*)pArgs->m_pContext;
    Int64  SofFrame = (Int64)pArgs->m_Data1.m_UInt64;

    DtDbgOut(MAX, GENL, "New Start-of-Frame=%lld", SofFrame);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Latch SOF sync info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    DtSpinLockAcquireAtDpc(&pDvcData->m_Matrix.m_SyncInfoSpinLock);

    pDvcData->m_Matrix.m_SyncInfo.m_SofFrame = SofFrame;
    pDvcData->m_Matrix.m_SyncInfo.m_NumChannels = 0;

    for (i=0, n=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        DtaNonIpPort*  pNonIpPort = &pDvcData->m_NonIpPorts[i];
        if (!pNonIpPort->m_CapMatrix)
            continue;
    
        pDvcData->m_Matrix.m_SyncInfo.m_Channels[n].m_PortIndex = pNonIpPort->m_PortIndex;
        pDvcData->m_Matrix.m_SyncInfo.m_Channels[n].m_SofFrame = 
                                                          pNonIpPort->m_Matrix.m_SofFrame;
        pDvcData->m_Matrix.m_SyncInfo.m_Channels[n].m_SofLine = 
                                                           pNonIpPort->m_Matrix.m_SofLine;

        n++;
    }
    pDvcData->m_Matrix.m_SyncInfo.m_NumChannels = n;
    pDvcData->m_Matrix.m_SyncInfo.m_IsValid = TRUE;

    DtSpinLockReleaseFromDpc(&pDvcData->m_Matrix.m_SyncInfoSpinLock);
    
    // Finaly: set SOF frame event and sync event
    DtEventSet(&pDvcData->m_Matrix.m_SofFrameIntEvent);
    DtEventSet(&pDvcData->m_Matrix.m_SofFrameSyncEvent);
}

