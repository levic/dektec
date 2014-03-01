//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* ShBuffer.c *#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// Dtu driver - Dtu Shared user-driver buffer routines used DTU-351
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2012 DekTec Digital Video B.V.
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
#include <DtuIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtuShBufferInit(DtuIoctlShBufCmdInput* pShBufCmdInput, DtFileObject*  pFile,
                          DtPageList* pPageList, char* pBuffer, Int BufSize, UInt BufType,
                          DtuShBuffer* pShBuffer);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuShBufferIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuShBufferIoctl(
    DtuDeviceData*  pDvcData, 
    DtFileObject*  pFile, 
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pIoctlStr;           // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  Index;
    DtuShBuffer*  pShBuffer = NULL;
    DtuIoctlShBufCmdInput*  pShBufCmdInput = 
                                           (DtuIoctlShBufCmdInput*)pIoctl->m_pInputBuffer;
    
    InReqSize = OFFSETOF(DtuIoctlShBufCmdInput, m_Data);
    
    
    // Check if we can read m_Cmd
    if (pIoctl->m_InputBufferSize < OFFSETOF(DtuIoctlShBufCmdInput, m_Data))
        return DT_STATUS_INVALID_PARAMETER;

    switch (pShBufCmdInput->m_Cmd)
    {
    case DTU_SH_BUF_CMD_INIT:
        pIoctlStr = "DTU_SH_BUF_CMD_INIT";
        InReqSize += sizeof(DtuIoctlShBufCmdInitInput);
        // We expect an output buffer size, but will be checked later
        OutReqSize = 0;
        break;

    case DTU_SH_BUF_CMD_CLOSE:
        pIoctlStr = "DTU_HP_BUF_CMD_CLOSE";
        // We expect no output buffer
        OutReqSize = 0;
        break;

    default:
        pIoctlStr = "??UNKNOWN VPDCMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, SHBUF, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pIoctlStr, 
                                                        pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, SHBUF, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pIoctlStr, 
                                                      pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, SHBUF, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pIoctlStr,
                pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }

    if (pShBufCmdInput->m_BufferIndex != 0)
    {
        DtDbgOut(ERR, SHBUF, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pIoctlStr, 
                                                      pIoctl->m_OutputBufferSize, OutReqSize);
        return DT_STATUS_INVALID_PARAMETER;
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    // Lookup the shared buffer structure
    // Check if the PortIndex is from the NonIpPort
    Status = DtuGetNonIpPortIndex(pDvcData, pShBufCmdInput->m_PortIndex, &Index);
    if (!DT_SUCCESS(Status))
        return DT_STATUS_NOT_FOUND;
    pShBuffer = &pDvcData->m_pNonIpPorts[Index].m_SharedBuffer;

    if (DT_SUCCESS(Status))
    {
        switch (pShBufCmdInput->m_Cmd)
        {
        case DTU_SH_BUF_CMD_INIT:
            {
                char*  pBuffer;
                UInt  Size;
                DtPageList*  pPageList = NULL;
#if defined(WINBUILD)
                DtPageList  PageList;
                PMDL  pMdl;
                NTSTATUS  NtStatus;
                // Retrieve MDL and virtual buffer from request object
                NtStatus = WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
                if (NtStatus != STATUS_SUCCESS)
                {
                    DtDbgOut(ERR, SHBUF, "WdfRequestRetrieveOutputWdmMdl error: %08x", 
                                                                                NtStatus);
                    Status = DT_STATUS_OUT_OF_RESOURCES;
                }
                if (DT_SUCCESS(Status))
                {
                    pBuffer = MmGetMdlVirtualAddress(pMdl);
                    if (pBuffer == NULL)
                    {
                        DtDbgOut(ERR, SHBUF, "DTU_SH_BUF_CMD_INIT: DT_STATUS_OUT_OF_MEMORY");
                        Status = DT_STATUS_OUT_OF_MEMORY;
                    }
                    Size = MmGetMdlByteCount(pMdl);

                    // Build pagelist object for user space buffer
                    pPageList = &PageList;
                    pPageList->m_BufType = DT_BUFTYPE_USER;
                    pPageList->m_OwnedByOs = TRUE;
                    pPageList->m_pMdl = pMdl;
                    pPageList->m_pVirtualKernel = NULL;
                }
#else // LINBUILD
                Size = (UInt)pShBufCmdInput->m_Data.m_Init.m_BufferSize;
#if defined(LIN32)
                pBuffer = (char*)(UInt32)pShBufCmdInput->m_Data.m_Init.m_BufferAddr;
#else
                pBuffer = (char*)(UInt64)pShBufCmdInput->m_Data.m_Init.m_BufferAddr;
#endif
#endif
                if (DT_SUCCESS(Status))
                {
                    Status = DtuShBufferInit(pShBufCmdInput, pFile, pPageList, pBuffer, 
                                                        Size, DT_BUFTYPE_USER, pShBuffer);
                    if (!DT_SUCCESS(Status))
                        DtDbgOut(ERR, SHBUF, "DtuShBufferInit failed");
                }
            }
            break;
        case DTU_SH_BUF_CMD_CLOSE:
            if (pDvcData->m_pNonIpPorts!=NULL && 
                                        pDvcData->m_pNonIpPorts[0].m_RxState==DTU_RX_READ)
            {
                pDvcData->m_pNonIpPorts[0].m_NextRxState = DTU_RX_CHECK_LOCK;
                DtEventSet(&pDvcData->m_pNonIpPorts[0].m_RxStateChanged);
                DtEventWait(&pDvcData->m_pNonIpPorts[0].m_RxStateChangeCmpl, -1);
            }
            Status = DtuShBufferClose(pShBuffer);
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
            DtDbgOut(MIN, SHBUF, "ShBufCmd=0x%x: NOT SUPPORTED", pShBufCmdInput->m_Cmd);
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuShBufferInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuShBufferInit(
    DtuIoctlShBufCmdInput*  pShBufCmdInput,
    DtFileObject*  pFile,
    DtPageList*  pPageList,
    char*  pBuffer,
    Int  BufSize,
    UInt  BufType,
    DtuShBuffer*  pShBuffer)
{
    DtStatus  Status;

    if (pShBuffer->m_Initialised)
        return DT_STATUS_IN_USE;

    // Create a pagelist for the shared buffer
    Status = DtCreatePageList(pBuffer, BufSize, BufType, &pShBuffer->m_PageList);
    if (!DT_SUCCESS(Status))
        return Status;
        
    pShBuffer->m_pBuffer = pBuffer;
    if (BufType == DT_BUFTYPE_USER)
    {
        // Lock buffer into kernel memory
        Status = DtLockUserBuffer(&pShBuffer->m_PageList, pBuffer);
        if (DT_SUCCESS(Status))
            pShBuffer->m_pBuffer = pShBuffer->m_PageList.m_pVirtualKernel;
    }
    if (!DT_SUCCESS(Status))
    {
        DtDeletePageList(&pShBuffer->m_PageList);
        return Status;
    }

    pShBuffer->m_Owner = *pFile;
    pShBuffer->m_Initialised = TRUE;
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuShBufferClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuShBufferClose(DtuShBuffer*  pShBuffer)
{
    if (!pShBuffer->m_Initialised)
        return DT_STATUS_NOT_INITIALISED;
    
    if (pShBuffer->m_PageList.m_BufType == DT_BUFTYPE_USER)
        DtUnlockUserBuffer(&pShBuffer->m_PageList);
    
    DtDeletePageList(&pShBuffer->m_PageList);

    pShBuffer->m_Initialised = FALSE;
    return DT_STATUS_OK;
}
