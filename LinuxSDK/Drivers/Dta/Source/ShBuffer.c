//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* ShBuffer.c *#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// Dta driver - Dta Shared user-driver buffer routines used for High-Performance DMA
// buffers and IpRx/IpTx buffers
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
#include <DtaIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaShBufferClose(DtaShBuffer* pShBuffer);
DtStatus  DtaShBufferInit(DtaIoctlShBufCmdInput* pShBufCmdInput, DtFileObject*  pFile,
                          DtPageList* pPageList, char* pBuffer, Int BufSize, UInt BufType,
                          DtaShBuffer* pShBuffer, Int Direction, DmaChannel* pDmaCh);
DtStatus  DtaShBufferRead(DtaIoctlShBufCmdInput* pShBufCmdInput, DtaShBuffer* pShBuffer,
                UInt8* pLocalAddress, UInt LocalAddressBufStart, UInt LocalAddressBufEnd);
DtStatus  DtaShBufferWrite(DtaIoctlShBufCmdInput* pShBufCmdInput, DtaShBuffer* pShBuffer,
                UInt8* pLocalAddress, UInt LocalAddressBufStart, UInt LocalAddressBufEnd);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaShBufferIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaShBufferIoctl(
    DtaDeviceData*  pDvcData, 
    DtFileObject*  pFile, 
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pIoctlStr;           // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Bool  IsIpPort = FALSE;
    Int  Index;
    Int  Direction = 0;
    DtaShBuffer*  pShBuffer = NULL;
    DmaChannel*  pDmaCh = NULL;
    UInt8*  pLocalAddress = NULL;
    UInt  LocalAddressBufStart = 0;
    UInt  LocalAddressBufEnd = 0;
    DtaIoctlShBufCmdInput*  pShBufCmdInput = 
                                           (DtaIoctlShBufCmdInput*)pIoctl->m_pInputBuffer;
    
    InReqSize = OFFSETOF(DtaIoctlShBufCmdInput, m_Data);
    
    
    // Check if we can read m_Cmd
    if (pIoctl->m_InputBufferSize < OFFSETOF(DtaIoctlShBufCmdInput, m_Data))
        return DT_STATUS_INVALID_PARAMETER;

    switch (pShBufCmdInput->m_Cmd)
    {
    case DTA_SH_BUF_CMD_INIT:
        pIoctlStr = "DTA_SH_BUF_CMD_INIT";
        InReqSize += sizeof(DtaIoctlShBufCmdInitInput);
        // We expect an output buffer size, but will be checked later
        OutReqSize = 0;
        break;

    case DTA_SH_BUF_CMD_READ:
        pIoctlStr = "DTA_SH_BUF_CMD_READ";
        InReqSize += sizeof(DtaIoctlShBufCmdReadInput);
        // We expect no output buffer
        OutReqSize = 0;
        break;

    case DTA_SH_BUF_CMD_WRITE:
        pIoctlStr = "DTA_SH_BUF_CMD_WRITE";
        InReqSize += sizeof(DtaIoctlShBufCmdWriteInput);
        // We expect no output buffer
        OutReqSize = 0;
        break;

    case DTA_SH_BUF_CMD_CLOSE:
        pIoctlStr = "DTA_HP_BUF_CMD_CLOSE";
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
    Status = DtaGetNonIpPortIndex(pDvcData, pShBufCmdInput->m_PortIndex, &Index);
    if (DT_SUCCESS(Status))
    {
        pShBuffer = &pDvcData->m_NonIpPorts[Index].m_HpBuffer;
        pDmaCh = &pDvcData->m_NonIpPorts[Index].m_DmaChannel;
        Direction = 
              (pDvcData->m_NonIpPorts[Index].m_IoCfg[DT_IOCONFIG_IODIR].m_Value
                                    == DT_IOCONFIG_INPUT ? DT_DMA_DIRECTION_FROM_DEVICE :
                                    DT_DMA_DIRECTION_TO_DEVICE);
        pLocalAddress = (UInt8*)(Int)pDvcData->m_NonIpPorts[Index].m_FifoOffset;
        if (!pDmaCh->m_FixedLocalAddress)
        {
            DT_ASSERT(FALSE);
            // pLocalAccess = ...GetSliceBufferPointer

            return DT_STATUS_NOT_SUPPORTED;
        }
    } else {
        // check if a IP port exists
        Status = DtaGetIpPortIndex(pDvcData, pShBufCmdInput->m_PortIndex, &Index);        
        if (!DT_SUCCESS(Status))
            return Status;
        
        // check if a pointer to the shared buffer for this channel is available
        pShBuffer = DtaIpGetSharedBuffer(&pDvcData->m_IpDevice, 
                           pShBufCmdInput->m_ChannelIndex, pShBufCmdInput->m_ChannelType);

        if (pShBuffer == NULL)
            return DT_STATUS_NOT_FOUND;

        IsIpPort = TRUE;
    }

    if (DT_SUCCESS(Status))
    {
        switch (pShBufCmdInput->m_Cmd)
        {
        case DTA_SH_BUF_CMD_INIT:
            {
                char*  pBuffer;
                UInt  Size;
                DtPageList*  pPageList = NULL;
#if defined(WINBUILD)
                DtPageList  PageList;
                PMDL  pMdl;
                // Retrieve MDL and virtual buffer from request object
                WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
                
                pBuffer = MmGetMdlVirtualAddress(pMdl);
                if (pBuffer == NULL)
                    Status = DT_STATUS_OUT_OF_MEMORY;
                Size = MmGetMdlByteCount(pMdl);

                // Build pagelist object for user space buffer
                pPageList = &PageList;
                pPageList->m_BufType = DT_BUFTYPE_USER;
                pPageList->m_OwnedByOs = TRUE;
                pPageList->m_pMdl = pMdl;
                pPageList->m_pVirtualKernel = NULL;
#else // LINBUILD
                Size = (UInt)pShBufCmdInput->m_Data.m_Init.m_BufferSize;
#if defined(LIN32)
                pBuffer = (char*)(UInt32)pShBufCmdInput->m_Data.m_Init.m_BufferAddr;
#else
                pBuffer = (char*)(UInt64)pShBufCmdInput->m_Data.m_Init.m_BufferAddr;
#endif
#endif
                if (DT_SUCCESS(Status))
                    Status = DtaShBufferInit(pShBufCmdInput, pFile, pPageList, pBuffer, 
                                                     Size, DT_BUFTYPE_USER, pShBuffer,
                                                     Direction, pDmaCh);
                if (DT_SUCCESS(Status) && IsIpPort)
                     Status = DtaIpSharedBufferReady(&pDvcData->m_IpDevice,
                           pShBufCmdInput->m_ChannelIndex, pShBufCmdInput->m_ChannelType);

            }
            break;
        case DTA_SH_BUF_CMD_READ:
            Status = DtaShBufferRead(pShBufCmdInput, pShBuffer, pLocalAddress,
                                                LocalAddressBufStart, LocalAddressBufEnd);
            break;
        case DTA_SH_BUF_CMD_WRITE:
            Status = DtaShBufferWrite(pShBufCmdInput, pShBuffer, pLocalAddress,
                                                LocalAddressBufStart, LocalAddressBufEnd);
            break;
        case DTA_SH_BUF_CMD_CLOSE:
            Status = DtaShBufferClose(pShBuffer);
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaShBufferInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaShBufferInit(
    DtaIoctlShBufCmdInput*  pShBufCmdInput,
    DtFileObject*  pFile,
    DtPageList*  pPageList,
    char*  pBuffer,
    Int  BufSize,
    UInt  BufType,
    DtaShBuffer*  pShBuffer,
    Int  Direction,
    DmaChannel*  pDmaCh)
{
    DtStatus  Status;

    if (pShBuffer->m_Initialised)
        return DT_STATUS_IN_USE;
    
    pShBuffer->m_Purpose = pShBufCmdInput->m_Data.m_Init.m_Purpose;
    if (pShBuffer->m_Purpose == DTA_SH_BUF_PURPOSE_DMA)
    {
        pShBuffer->m_pDmaCh = pDmaCh;
        
        // Initialise DMA channel struct and create SGL buffer
        Status = DtaDmaPrepareDataBuffer(pPageList, pBuffer, BufSize, BufType, pDmaCh, 
                                                                               Direction);

        pShBuffer->m_pBuffer = pDmaCh->m_pBuffer;
        if (!DT_SUCCESS(Status))
            return Status;
    } else {
       
        //Create a pagelist for the shared buffer
        Status = DtCreatePageList(pBuffer, BufSize, BufType, &pShBuffer->m_PageList);
        if (!DT_SUCCESS(Status))
            return Status;
        
        pShBuffer->m_pBuffer = pBuffer;
        pShBuffer->m_pDmaCh = NULL;
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
    }
    pShBuffer->m_Owner = *pFile;
    pShBuffer->m_Initialised = TRUE;
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaShBufferWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaShBufferWrite(
    DtaIoctlShBufCmdInput*  pShBufCmdInput,
    DtaShBuffer*  pShBuffer,
    UInt8*  pLocalAddress,
    UInt  LocalAddressBufStart,
    UInt  LocalAddressBufEnd)
{
    if (!pShBuffer->m_Initialised)
        return DT_STATUS_NOT_INITIALISED;
    
    DT_ASSERT(pShBuffer->m_pDmaCh->m_DmaDirection == DT_DMA_DIRECTION_TO_DEVICE);
    
    DtDbgOut(MAX, SHBUF, "PortIndex: %d, Offset: %d, Length: %d", 
        pShBuffer->m_pDmaCh->m_PortIndex, pShBufCmdInput->m_Data.m_Write.m_Offset,
        pShBufCmdInput->m_Data.m_Write.m_NumBytesToWrite);
    
    return DtaDmaStartHpTransfer(pShBuffer, 
                                   pShBufCmdInput->m_Data.m_Write.m_NumBytesToWrite,
                                   pShBufCmdInput->m_Data.m_Write.m_Offset, pLocalAddress,
                                   LocalAddressBufStart, LocalAddressBufEnd);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaShBufferRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaShBufferRead(
    DtaIoctlShBufCmdInput*  pShBufCmdInput,
    DtaShBuffer*  pShBuffer,
    UInt8*  pLocalAddress,
    UInt  LocalAddressBufStart,
    UInt  LocalAddressBufEnd)
{
    if (!pShBuffer->m_Initialised)
        return DT_STATUS_NOT_INITIALISED;
    
    DT_ASSERT(pShBuffer->m_pDmaCh->m_DmaDirection == DT_DMA_DIRECTION_FROM_DEVICE);
    
    DtDbgOut(MAX, SHBUF, "PortIndex: %d, Offset: %d, Length: %d", 
        pShBuffer->m_pDmaCh->m_PortIndex, pShBufCmdInput->m_Data.m_Read.m_Offset,
        pShBufCmdInput->m_Data.m_Read.m_NumBytesToRead);
    
    return DtaDmaStartHpTransfer(pShBuffer, 
                                   pShBufCmdInput->m_Data.m_Write.m_NumBytesToWrite,
                                   pShBufCmdInput->m_Data.m_Write.m_Offset, pLocalAddress,
                                   LocalAddressBufStart, LocalAddressBufEnd);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaShBufferClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaShBufferClose(
    DtaShBuffer*  pShBuffer)
{

    if (!pShBuffer->m_Initialised)
        return DT_STATUS_NOT_INITIALISED;

    if (pShBuffer->m_Purpose == DTA_SH_BUF_PURPOSE_DMA)
    {
        DtaDmaAbortDma(pShBuffer->m_pDmaCh);
        while (!DtaDmaIsReady(pShBuffer->m_pDmaCh))
            DtSleep(10);

        DtaDmaFinalTransfer(pShBuffer->m_pDmaCh);
        DtaDmaCleanupDataBuffer(pShBuffer->m_pDmaCh);
    } else {
        // clean pagelist
        if (pShBuffer->m_PageList.m_BufType == DT_BUFTYPE_USER)
            DtUnlockUserBuffer(&pShBuffer->m_PageList);
        
        DtDeletePageList(&pShBuffer->m_PageList);
    }

    pShBuffer->m_Initialised = FALSE;
    return DT_STATUS_OK;
}
