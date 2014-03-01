//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* IpRx.c *#*#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// Dta driver - IP RX functionality - Implementation of RX specific functionality for
//                                    IP ports.
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


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// IPRX Tag
#define  IPRX_TAG         0x78527049  // 'xRpI'

#if defined(WINBUILD)
#define  DTA_IPRX_DMA_BUF_SIZE          (1024*1024)
#define  DTA_IPRX_NRT_SHARED_BUF_SIZE   (2*1024*1024)
#else
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20)) || !defined(CONFIG_MMU)
#define  DTA_IPRX_DMA_BUF_SIZE          (1024*1024)
#define  DTA_IPRX_NRT_SHARED_BUF_SIZE   (2*1024*1024)
#else
// Lower value for RHEL/CentOS kernels
#warning "IP buffer too small to get good performance!"
#define  DTA_IPRX_DMA_BUF_SIZE          (64*1024)
#define  DTA_IPRX_NRT_SHARED_BUF_SIZE   (2*64*1024)
#endif
#endif

#define  DTA_IPRX_PINGPONG_BUF_SIZE     (DTA_IPRX_DMA_BUF_SIZE * 2)

#define  DTA_IPRX_DMA_HEADER_TAG        (0x445441A0)

#define  DTA_ETH_HEADER_SIZE  14
#define  DTA_MAX_ETH_PAYLOAD  2000
#define  DTA_IPRX_MAX_PACKET_SIZE       (DT_IP_MAX_PACKET_SIZE)

#define  MAX_OUTOFSYNC_SEQ_NUM          12  

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtaIpRxWorkerThread(DtThread* pThread, void* pContext);
void  DtaIpRxReconstructThread(DtThread* pThread, void* pContext);
DtStatus  DtaIpRxUserChAttach(DtaIpUserChannels* pIpUserChannels, Int IpPortIndex,
                                                 DtFileObject* pFile, Int* pChannelIndex);
DtStatus  DtaIpRxUserChDetach(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex);
DtStatus  DtaIpRxUserChClearFlags(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                                               Int Flags);
DtStatus  DtaIpRxUserChGetFlags(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex, 
                                                         Int* pFlags, Int* pLatchedFlags);
DtStatus  DtaIpRxGetStatus(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                          Int* pClkDet, Int* pPacketSize);
DtStatus  DtaIpRxGetTsRate(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                                            Int* pTsRate);
DtStatus  DtaIpRxSetRxMode(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex, 
                                                                              Int RxMode);
DtStatus  DtaIpRxReset(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex);
DtStatus  DtaIpRxSetIpPars(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                             UInt8* pSrcIp, UInt16 SrcPort, UInt8* pDstIp, UInt16 DstPort, 
                             Int FecMode, Int Protocol);
DtStatus  DtaIpRxGetIpPars(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                 Int* pDetFecNumCols, Int* pDetFecNumRows,
                                                 Int* pDetNumTpPerIp, Int* pDetProtocol);
DtStatus  DtaIpRxGetIpStat(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                              Int* pLostIPPacketsAfterFec, Int* pLostIPPacketsAfterSort,
                              Int* pLostIPPacketsBeforeFec, Int* pLostIPPacketsBeforeSort,
                              Int* pTotNumIPPackets);
DtStatus  DtaIpRxSetRxControl(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex, 
                                                                           Int RxControl);

void  DtaIpRxUserChInit(UserIpRxChannel* pIpRxChannel);
void  DtaIpRxUserChReset(UserIpRxChannel* pIpRxChannel, Bool ClearInputStatus);
UserIpRxChannel*  DtaIpRxUserChCreate(Int ChannelIndex, Int IpPortIndex, 
                                                                     DtFileObject* pFile);
static void  DtaIpRxUserChAddToList(DtaIpUserChannels* pIpUserChannels, 
                                                               UserIpRxChannel* pIpRxNew);
static void  DtaIpRxUserChRemoveFromList(DtaIpUserChannels* pIpUserChannels,
                                                           UserIpRxChannel* pIpRxChannel);
DtStatus  DtaIpRxCreateDmaBuffer(DmaChannel* pDmaChannel, PPBuffer* pPPBuffer, 
                                                                      DtaIpPort* pIpPort);
DtStatus  DtaIpRxCleanupDmaBuffer(DmaChannel* pDmaChannel, PPBuffer* pPPBuffer);

void  DtaIpRxRtpListsReset(UserIpRxChannel* pIpRxChannel);
DtStatus  DtaIpRxRtpListsInit(UserIpRxChannel* pIpRxChannel);
void  DtaIpRxRtpListsCleanup( UserIpRxChannel* pIpRxChannel);
void  DtaIpRxSetBrmSkipUpdate(DtaIpUserChannels* pIpRxUserChannels, Bool State);
void  DtaIpRxUpdateBrmDpc(DtDpcArgs* pArgs);
void  DtaIpRxProcessDvbWithFec(UserIpRxChannel* pIpRxChannel);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxDeviceInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxDeviceInit(DtaIpDevice* pIpDevice)
{
    DtStatus  Status = DT_STATUS_OK;

    Status = DtDpcInit(&pIpDevice->m_IpRtRxBrmDpc, DtaIpRxUpdateBrmDpc, TRUE);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxInitType1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxInitType1(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  DmaRegsOffsetRx;
    DtaIpPortType1*  pIpPortType1 = &pIpPort->m_IpPortType1;
    DtPropertyData*  pPropData = &pIpPort->m_pDvcData->m_PropData;

    // Initialise register offsets
    pIpPortType1->m_Rx.m_pRegs = NULL;
    pIpPortType1->m_Rx.m_RegsOffset = DtPropertiesGetUInt16(pPropData,
                                                 "NW_REGISTERS_RX", pIpPort->m_PortIndex);
    pIpPortType1->m_Rx.m_FifoOffset = 
                                 pIpPortType1->m_Rx.m_RegsOffset + DTA_LOCALADDR_FIFODATA;
    DmaRegsOffsetRx = DtPropertiesGetUInt16(pPropData, 
                                             "NW_REGISTERS_RX_DMA", pIpPort->m_PortIndex);
    // Report configuration errors

    // Check if no property error occurred
    Status = DtaPropertiesReportDriverErrors(pIpPort->m_pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

     // Initialise Rx DMA channel
    
    // For Rx we use a Ping-pong buffer to transfer the data.
    // For the complete ping-pong buffer, the SGL list is created
    // The actual DMA transfer size will be the size divided by 2.
    Status = DtaDmaInitCh(pIpPort->m_pDvcData, pIpPort->m_PortIndex, 
                              DTA_IPRX_PINGPONG_BUF_SIZE, DTA_DMA_MODE_TIMEOUT_ENABLE,
                              DmaRegsOffsetRx, DTA_DMA_FLAGS_DATA_BUF_NO_COPY, 2,
                              NULL, NULL, &pIpPortType1->m_Rx.m_DmaChannel);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_RX, "Error initialising IpRx DMA channel");
        return Status;
    }

    Status = DtaIpRxCreateDmaBuffer(&pIpPortType1->m_Rx.m_DmaChannel, 
                                                          &pIpPort->m_RxPPBuffer,pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_RX, "Error creating IpRx DMA buffer");
        return Status;
    }

    // Create and initialise Nrt Rx
    Status = DtaIpRxNrtCreateBuffer(&pIpPort->m_NrtChannels);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_RX, "Error creating IpRx NRT buffer");
        return Status;
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxIoctl(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  IpPortIndex;           // Index in the Ip port struct
    DtaIoctlIpRxCmdInput*  pIpRxCmdInput = (DtaIoctlIpRxCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlIpRxCmdOutput*  pIpRxCmdOutput =
                                          (DtaIoctlIpRxCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlIpRxCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlIpRxCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    if (!DT_SUCCESS(DtaGetIpPortIndex(pDvcData, pIpRxCmdInput->m_NwPortIndex,
                                                                           &IpPortIndex)))
        return DT_STATUS_INVALID_PARAMETER;

    // Determine final required output/input sizes
    switch (pIpRxCmdInput->m_Cmd)
    {
    case DTA_IP_RX_CMD_CHANNEL_ATTACH:
        pCmdStr = "DTA_IP_RX_CMD_CHANNEL_ATTACH";
        OutReqSize += sizeof(DtaIoctlIpXxCmdAttachChannelOutput);
        InReqSize = 0;
        break;
    case DTA_IP_RX_CMD_CHANNEL_DETACH:
        pCmdStr = "DTA_IP_RX_CMD_CHANNEL_DETACH";
        OutReqSize = 0;        
        InReqSize += sizeof(DtaIoctlIpXxCmdAttachChannelOutput);
        break;
    case DTA_IP_RX_CMD_SETCONTROL:
        pCmdStr = "DTA_IP_RX_CMD_SETCONTROL";
        OutReqSize = 0;        
        InReqSize += sizeof(DtaIoctlIpXxCmdSetControlInput);
        break;    
    case DTA_IP_RX_CMD_RESET:
        pCmdStr = "DTA_IP_RX_CMD_RESET";
        OutReqSize = 0;        
        InReqSize += sizeof(DtaIoctlIpXxCmdResetInput);
        break;
    case DTA_IP_RX_CMD_CLEARFLAGS:
        pCmdStr = "DTA_IP_RX_CMD_CLEARFLAGS";
        OutReqSize = 0;        
        InReqSize += sizeof(DtaIoctlIpXxCmdClearFlagsInput);
        break;
    case DTA_IP_RX_CMD_GETFLAGS:
        pCmdStr = "DTA_IP_RX_CMD_GETFLAGS";
        OutReqSize += sizeof(DtaIoctlIpXxCmdGetFlagsOutput);
        InReqSize += sizeof(DtaIoctlIpXxCmdGetFlagsInput);
        break;
    case DTA_IP_RX_CMD_GETTSRATE:
        pCmdStr = "DTA_IP_RX_CMD_GETTSRATE";
        OutReqSize += sizeof(DtaIoctlIpRxCmdGetTsRateOutput);
        InReqSize += sizeof(DtaIoctlIpRxCmdGetTsRateInput);
        break;
    case DTA_IP_RX_CMD_GETSTATUS:
        pCmdStr = "DTA_IP_RX_CMD_GETSTATUS";
        OutReqSize += sizeof(DtaIoctlIpRxCmdGetStatusOutput);
        InReqSize += sizeof(DtaIoctlIpRxCmdGetStatusInput);
        break;
    case DTA_IP_RX_CMD_SETMODE:
        pCmdStr = "DTA_IP_RX_CMD_SETMODE";
        OutReqSize = 0;        
        InReqSize += sizeof(DtaIoctlIpRxCmdSetModeInput);
        break;
    case DTA_IP_RX_CMD_SETIPPARS:
        pCmdStr = "DTA_IP_RX_CMD_SETIPPARS";
        OutReqSize = 0;
        InReqSize += sizeof(DtaIoctlIpRxCmdSetIpParsInput);
        break;
    case DTA_IP_RX_CMD_GETIPPARS:
        pCmdStr = "DTA_IP_RX_CMD_GETIPPARS";
        OutReqSize += sizeof(DtaIoctlIpRxCmdGetIpParsOutput);
        InReqSize += sizeof(DtaIoctlIpRxCmdGetIpParsInput);
        break;
    case DTA_IP_RX_CMD_GETIPSTAT:
        pCmdStr = "DTA_IP_RX_CMD_GETIPSTAT";
        OutReqSize += sizeof(DtaIoctlIpRxCmdGetIpStatOutput);
        InReqSize += sizeof(DtaIoctlIpRxCmdGetIpStatInput);
        break;
    default:
        pCmdStr = "??UNKNOWN IP_RX_CMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, IP_RX, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, IP_RX, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, IP_RX, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
                pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize,
                                                                              OutReqSize);
    }

    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pIpRxCmdInput->m_Cmd)
        {
        case DTA_IP_RX_CMD_CHANNEL_ATTACH:
            Status = DtaIpRxUserChAttach(&pDvcData->m_IpDevice.m_IpUserChannels, 
                                       IpPortIndex, pFile, 
                                       &pIpRxCmdOutput->m_Data.m_AttachChannel.m_Channel);
            break;
        case DTA_IP_RX_CMD_CHANNEL_DETACH:
            Status = DtaIpRxUserChDetach(&pDvcData->m_IpDevice.m_IpUserChannels,
                                         pIpRxCmdInput->m_Data.m_DetachChannel.m_Channel);
            break;
        case DTA_IP_RX_CMD_CLEARFLAGS:
            Status = DtaIpRxUserChClearFlags(&pDvcData->m_IpDevice.m_IpUserChannels,
                                             pIpRxCmdInput->m_Data.m_ClearFlags.m_Channel,
                                             pIpRxCmdInput->m_Data.m_ClearFlags.m_Flags);
            break;
        case DTA_IP_RX_CMD_GETFLAGS:
            Status = DtaIpRxUserChGetFlags(&pDvcData->m_IpDevice.m_IpUserChannels,
                                       pIpRxCmdInput->m_Data.m_GetFlags.m_Channel,
                                       &pIpRxCmdOutput->m_Data.m_GetFlags.m_Flags, 
                                       &pIpRxCmdOutput->m_Data.m_GetFlags.m_LatchedFlags);
            break;
        case DTA_IP_RX_CMD_GETTSRATE:            
            Status = DtaIpRxGetTsRate(&pDvcData->m_IpDevice.m_IpUserChannels,
                                       pIpRxCmdInput->m_Data.m_GetTsRate.m_Channel,
                                       &pIpRxCmdOutput->m_Data.m_GetTsRate.m_TsRate);
            break;
        case DTA_IP_RX_CMD_GETSTATUS:
            Status = DtaIpRxGetStatus(&pDvcData->m_IpDevice.m_IpUserChannels,
                                        pIpRxCmdInput->m_Data.m_GetStatus.m_Channel,
                                        &pIpRxCmdOutput->m_Data.m_GetStatus.m_ClkDet,
                                        &pIpRxCmdOutput->m_Data.m_GetStatus.m_PacketSize);
            break;
        case DTA_IP_RX_CMD_SETMODE:
            Status = DtaIpRxSetRxMode(&pDvcData->m_IpDevice.m_IpUserChannels,
                                              pIpRxCmdInput->m_Data.m_SetRxMode.m_Channel,
                                              pIpRxCmdInput->m_Data.m_SetRxMode.m_RxMode);
            break;
        case DTA_IP_RX_CMD_RESET:
            Status = DtaIpRxReset(&pDvcData->m_IpDevice.m_IpUserChannels,
                                                 pIpRxCmdInput->m_Data.m_Reset.m_Channel);
            break;
        case DTA_IP_RX_CMD_SETIPPARS:            
            Status = DtaIpRxSetIpPars(&pDvcData->m_IpDevice.m_IpUserChannels,
                                             pIpRxCmdInput->m_Data.m_SetIpPars.m_Channel,
                                             pIpRxCmdInput->m_Data.m_SetIpPars.m_SrcIp,
                                             pIpRxCmdInput->m_Data.m_SetIpPars.m_SrcPort,
                                             pIpRxCmdInput->m_Data.m_SetIpPars.m_DstIp,
                                             pIpRxCmdInput->m_Data.m_SetIpPars.m_DstPort,
                                             pIpRxCmdInput->m_Data.m_SetIpPars.m_FecMode,
                                             pIpRxCmdInput->m_Data.m_SetIpPars.m_Protocol);
            break;
        case DTA_IP_RX_CMD_GETIPPARS:
            Status = DtaIpRxGetIpPars(&pDvcData->m_IpDevice.m_IpUserChannels,
                                            pIpRxCmdInput->m_Data.m_GetIpPars.m_Channel,
                                            &pIpRxCmdOutput->m_Data.m_GetIpPars.m_FecNumCols,
                                            &pIpRxCmdOutput->m_Data.m_GetIpPars.m_FecNumRows,
                                            &pIpRxCmdOutput->m_Data.m_GetIpPars.m_NumTpPerIp,
                                            &pIpRxCmdOutput->m_Data.m_GetIpPars.m_Protocol);
            break;
        case DTA_IP_RX_CMD_GETIPSTAT:
            Status = DtaIpRxGetIpStat(&pDvcData->m_IpDevice.m_IpUserChannels,
                                    pIpRxCmdInput->m_Data.m_GetIpStat.m_Channel,
                            &pIpRxCmdOutput->m_Data.m_GetIpStat.m_LostIPPacketsAfterFec,
                            &pIpRxCmdOutput->m_Data.m_GetIpStat.m_LostIPPacketsAfterSort,
                            &pIpRxCmdOutput->m_Data.m_GetIpStat.m_LostIPPacketsBeforeFec,
                            &pIpRxCmdOutput->m_Data.m_GetIpStat.m_LostIPPacketsBeforeSort,
                            &pIpRxCmdOutput->m_Data.m_GetIpStat.m_TotNumIPPackets);
            break;
        case DTA_IP_RX_CMD_SETCONTROL:
            Status = DtaIpRxSetRxControl(&pDvcData->m_IpDevice.m_IpUserChannels, 
                                          pIpRxCmdInput->m_Data.m_SetRxControl.m_Channel, 
                                          pIpRxCmdInput->m_Data.m_SetRxControl.m_Control);
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
            DtDbgOut(MIN, IP_RX, "IpRxCmd=0x%x: NOT SUPPORTED", pIpRxCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, IP_RX, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IP RX IOCTL Handlers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChAttach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Creates and adds an empty IP RX channel to its internal list. The index of this new
// channel is returned to the caller.
//
// Precondition: IpPortIndex is the 0-based index to an IpPort, NOT the port number as
//               set by the user.
//
DtStatus  DtaIpRxUserChAttach(
    DtaIpUserChannels*  pIpUserChannels,
    Int  IpPortIndex, 
    DtFileObject*  pFile,
    Int*  pChannelIndex)
{
    UserIpRxChannel*  pIpRxChannel;
    Int  ChannelIndex = 0;

    DtDbgOut(MAX, IP_RX, "Start");
    pIpRxChannel = pIpUserChannels->m_pIpRxChannel;

    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChAccesMutex);

    // find a free index
    while (pIpRxChannel != NULL)
    {
        // reuse a previously detached channel index if available
        if (pIpRxChannel->m_ChannelIndex != ChannelIndex)
            break;

        ChannelIndex++;
        pIpRxChannel = pIpRxChannel->m_pNext;
    }

    // create a channel with this index
    pIpRxChannel = DtaIpRxUserChCreate(ChannelIndex, IpPortIndex, pFile);
    if (pIpRxChannel == NULL)
    {
        DtFastMutexRelease(&pIpUserChannels->m_IpRxChAccesMutex);
        return DT_STATUS_OUT_OF_MEMORY;
    }    

    // Add it to our list and initialize:
    // Remark: Make shure the workerthreads do not try to use the new channel
    //         before properly added.
    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);
    
    // Skip Brm updates while adding the channel
    DtaIpRxSetBrmSkipUpdate(pIpUserChannels, TRUE);

    pIpRxChannel->m_IpParsValid = FALSE;
    pIpRxChannel->m_RefCount = 0;    

    DtaIpRxUserChInit(pIpRxChannel);
    DtaIpRxUserChReset(pIpRxChannel, TRUE);
    DtaIpRxUserChAddToList(pIpUserChannels, pIpRxChannel);

    // Done, release all
    DtaIpRxSetBrmSkipUpdate(pIpUserChannels, FALSE);

    DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);

    // return the index
    *pChannelIndex = ChannelIndex;
    DtFastMutexRelease(&pIpUserChannels->m_IpRxChAccesMutex);
    
    DtDbgOut(MAX, IP_RX, "Attached");
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChDetach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxUserChDetach(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Destroy the channel completely
    DtaIpRxUserChDestroy(pIpUserChannels, pIpRxChannel);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChClearFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxUserChClearFlags(
    DtaIpUserChannels*  pIpUserChannels, 
    Int  ChannelIndex,
    Int  Flags)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Clear flags specified in IOCTL
    DtSpinLockAcquire(&pIpRxChannel->m_FlagsSpinLock);
    pIpRxChannel->m_LatchedFlags &= ~Flags;
    DtSpinLockRelease(&pIpRxChannel->m_FlagsSpinLock);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChGetFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxUserChGetFlags(
    DtaIpUserChannels*  pIpUserChannels, 
    Int  ChannelIndex,
    Int*  pFlags,
    Int*  pLatchedFlags)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    *pFlags = pIpRxChannel->m_Flags;
    *pLatchedFlags = pIpRxChannel->m_LatchedFlags;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus DtaIpRxGetStatus(
    DtaIpUserChannels*  pIpUserChannels, 
    Int  ChannelIndex,
    Int*  pClkDet,
    Int*  pPacketSize)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Received packet size
    if (pIpRxChannel->m_PckSizeSrc == 188)
        *pPacketSize = DT_PCKSIZE_188;
    else if (pIpRxChannel->m_PckSizeSrc == 204)
        *pPacketSize = DT_PCKSIZE_204;
    else
        *pPacketSize = DT_PCKSIZE_INV;

    // ClkDet
    *pClkDet = (pIpRxChannel->m_BrmEstimate == 0 ? 0 : 1);

    DtDbgOut(MAX, IP_RX, "Channel %d: PacketSize=%d (0=INV;2=188;3=204) ClkDet=%d", 
                                   pIpRxChannel->m_ChannelIndex, *pPacketSize, *pClkDet);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetTsRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaIpRxGetTsRate(
    DtaIpUserChannels*  pIpUserChannels, 
    Int  ChannelIndex,
    Int*  pTsRate)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    *pTsRate = pIpRxChannel->m_BrmEstimate;
    DtDbgOut(MAX,IP_RX,"Channel %d: TsRate=%d", pIpRxChannel->m_ChannelIndex, *pTsRate);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxSetMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxSetRxMode(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex, Int RxMode)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Can change Receive Mode only if Receive-Control state is IDLE
    if (pIpRxChannel->m_RxControl != DT_RXCTRL_IDLE)
        return DT_STATUS_FAIL;

    pIpRxChannel->m_RxMode = RxMode;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChRefWaitFinished -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxUserChRefWaitFinished(UserIpRxChannel* pIpRxChannel)
{
    while (DtAtomicIncrementReturn(&pIpRxChannel->m_RefCount) != 1) 
    {
        DT_ASSERT(pIpRxChannel->m_RefCount>=1);
        DtAtomicDecrement(&pIpRxChannel->m_RefCount);
        // For single processor system with real time priority application
        // give some time to other threads to complete.
        DtSleep(1);
    }
    DtAtomicDecrement(&pIpRxChannel->m_RefCount);    // Set count back to 0
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChRefAdd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxUserChRefAdd(UserIpRxChannel* pIpRxChannel)
{
    DtAtomicIncrement(&pIpRxChannel->m_RefCount);
    DT_ASSERT(pIpRxChannel->m_RefCount>=1);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChRefDecr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxUserChRefDecr(UserIpRxChannel* pIpRxChannel)
{
    DtAtomicDecrement(&pIpRxChannel->m_RefCount);
    DT_ASSERT(pIpRxChannel->m_RefCount>=0);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChRefDecrAll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtaIpRxUserChRefDecrAll(UserIpRxChannel** pIpRxChannels, UInt NumChannels)
{
    UInt i;
    UserIpRxChannel*  pIpRxChannel;
    for(i=0; i<NumChannels; i++)
    {
        pIpRxChannel = pIpRxChannels[i];
        DtAtomicDecrement(&pIpRxChannel->m_RefCount);
        DT_ASSERT(pIpRxChannel->m_RefCount>=0);
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxSetRxControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtaIpRxSetRxControl(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex, 
                                                                            Int RxControl)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    if (RxControl == DT_RXCTRL_RCV) 
    {
        if (!pIpRxChannel->m_SharedBuffer.m_Initialised) 
        {
            DtDbgOut(ERR, IP_RX, "Fifo buffer not initialized for channel %d.", ChannelIndex);
            return DT_STATUS_FAIL;
        }
        pIpRxChannel->m_RxControl = DT_RXCTRL_RCV;
        return DT_STATUS_OK;
    }
    
    pIpRxChannel->m_RxControl = DT_RXCTRL_IDLE;

    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);
    DtaIpRxUserChRefWaitFinished(pIpRxChannel);
    DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxReset(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex)
{
    UserIpRxChannel*  pIpRxChannel = NULL;

    DtaIpRxSetRxControl(pIpUserChannels, ChannelIndex, DT_RXCTRL_IDLE);

    pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    DtaIpRxUserChReset(pIpRxChannel, FALSE);

    if (pIpRxChannel->m_SharedBuffer.m_Initialised)
        DtaIpRxRtpListsReset(pIpRxChannel);

    return DT_STATUS_OK;
}
 
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxSetIpPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxSetIpPars(
    DtaIpUserChannels* pIpUserChannels,
    Int ChannelIndex,
    UInt8*  pSrcIp,
    UInt16  SrcPort,
    UInt8*  pDstIp,
    UInt16  DstPort,
    Int  FecMode,
    Int  Protocol)
{
    Int i;

    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Only allowed in Receive-Control state IDLE
    if (pIpRxChannel->m_RxControl != DT_RXCTRL_IDLE) 
    {
        DtDbgOut(ERR, IP_RX, "Setting IP pars not allowed if channel is NOT IDLE");
        return DT_STATUS_FAIL;
    }

    pIpRxChannel->m_SrcPort = SrcPort;
    pIpRxChannel->m_DstPort = DstPort;

    for (i = 0; i < 4; i++) {
        pIpRxChannel->m_SrcIPAddress[i] = pSrcIp[i];
        pIpRxChannel->m_DstIPAddress[i] = pDstIp[i];
    }

    pIpRxChannel->m_FecMode = FecMode;
    pIpRxChannel->m_IpParsValid = TRUE;

    // Reset channel after including input streams status information
    DtaIpRxUserChReset(pIpRxChannel, TRUE);

    DtDbgOut(MIN, IP_RX, "Channel %d: New IP pars: SrcPort=%d; DstPort=%d " \
                         "SrcIP: %d.%d.%d.%d DstIP: %d.%d.%d.%d",
                         pIpRxChannel->m_ChannelIndex, 
                         pIpRxChannel->m_SrcPort, pIpRxChannel->m_DstPort,
                         pIpRxChannel->m_SrcIPAddress[0],pIpRxChannel->m_SrcIPAddress[1],
                         pIpRxChannel->m_SrcIPAddress[2],pIpRxChannel->m_SrcIPAddress[3],
                         pIpRxChannel->m_DstIPAddress[0],pIpRxChannel->m_DstIPAddress[1],
                         pIpRxChannel->m_DstIPAddress[2],pIpRxChannel->m_DstIPAddress[3]);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetIpPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxGetIpPars(
    DtaIpUserChannels*  pIpUserChannels,
    Int  ChannelIndex,
    Int*  pDetFecNumCols,
    Int*  pDetFecNumRows,    
    Int*  pDetNumTpPerIp,
    Int*  pDetProtocol)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    *pDetFecNumCols = pIpRxChannel->m_DetFecNumColumns;
    *pDetFecNumRows = pIpRxChannel->m_DetFecNumRows;
    *pDetNumTpPerIp = pIpRxChannel->m_DetNumTpPerIp;
    *pDetProtocol = pIpRxChannel->m_DetProtocol;
    DtDbgOut(MIN, IP_RX, "Channel %d: NumCols=%d; NumRows=%d;" 
                       "NumTpPerIp=%d; m_Protocol=%d", pIpRxChannel->m_ChannelIndex, 
                       *pDetFecNumCols, *pDetFecNumRows, *pDetNumTpPerIp, *pDetProtocol);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetIpStat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxGetIpStat(   
    DtaIpUserChannels*  pIpUserChannels,
    Int  ChannelIndex,
    Int*  pLostIPPacketsAfterFec,
    Int*  pLostIPPacketsAfterSort,
    Int*  pLostIPPacketsBeforeFec,
    Int*  pLostIPPacketsBeforeSort,
    Int*  pTotNumIPPackets)
{    
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    DtSpinLockAcquire(&pIpRxChannel->m_StatisticsSpinLock);
    *pTotNumIPPackets = pIpRxChannel->m_TotNumPackets;
    *pLostIPPacketsAfterFec = pIpRxChannel->m_NumPacketsNotReconstructed;
    *pLostIPPacketsBeforeFec = pIpRxChannel->m_NumPacketsNotReconstructed +
                               pIpRxChannel->m_NumPacketsReconstructed;

    *pLostIPPacketsAfterSort = 0;
    *pLostIPPacketsBeforeSort = 0;

    DtSpinLockRelease(&pIpRxChannel->m_StatisticsSpinLock);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxCleanupType1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxCleanupType1(DtaIpPort* pIpPort)
{
    DtaIpPortType1*  pIpPortType1 = &pIpPort->m_IpPortType1;
    
    
    // Cleanup DMA Rx
    DtaIpRxCleanupDmaBuffer(&pIpPortType1->m_Rx.m_DmaChannel, &pIpPort->m_RxPPBuffer);
    
    // Cleanup Nrt Rx
    DtaIpRxNrtCleanupBuffer(&pIpPort->m_NrtChannels);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxInit(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;

    Status = DtThreadInit(&pIpPort->m_RxThread, DtaIpRxWorkerThread, pIpPort);
    if (DT_SUCCESS(Status))
        Status = DtThreadInit(&pIpPort->m_RxReconstructThread, 
             DtaIpRxReconstructThread, &pIpPort->m_pDvcData->m_IpDevice.m_IpUserChannels);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxDevicePowerDown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxDevicePowerDown(DtaIpDevice* pIpDevice)
{
    DtDpcWaitForCompletion(&pIpDevice->m_IpRtRxBrmDpc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxPowerdownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxPowerdownPre(DtaIpPort* pIpPort)
{    
    DtDbgOut(AVG, IP_RX, "Stopping IpRx threads");
    DtThreadStop(&pIpPort->m_RxThread);
    DtThreadStop(&pIpPort->m_RxReconstructThread);
    DtDbgOut(AVG, IP_RX, "IpRx threads stopped");
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxPowerup(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;

    DtDbgOut(AVG, IP_RX, "Starting IpRx threads");
    
    // Try to start the main receiving worker thread
    // do not start the reconstrructor if this fails
    Status = DtThreadStart(&pIpPort->m_RxThread);
    if (DT_SUCCESS(Status))
        Status = DtThreadStart(&pIpPort->m_RxReconstructThread);

    if (DT_SUCCESS(Status))
        DtDbgOut(AVG, IP_RX, "IpRx threads started");

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChGetUnsafe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UserIpRxChannel*  DtaIpRxUserChGetUnsafe(
    DtaIpUserChannels*  pIpUserChannels, 
    Int  ChannelIndex)
{
    UserIpRxChannel* pIpRxChannel = pIpUserChannels->m_pIpRxChannel;
    while (pIpRxChannel != NULL)
    {        
        if (pIpRxChannel->m_ChannelIndex == ChannelIndex)
            break;
        pIpRxChannel = pIpRxChannel->m_pNext;
    }

    if (pIpRxChannel == NULL)
        DtDbgOut(ERR, IP_RX, "Could not find channel %d", ChannelIndex);

     return pIpRxChannel;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Returns an IP RX channel object based on its channel index
//
UserIpRxChannel*  DtaIpRxUserChGet(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex)
{
    UserIpRxChannel*  pIpRxChannel;
    
    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChAccesMutex);
    pIpRxChannel = pIpUserChannels->m_pIpRxChannel;
    while (pIpRxChannel != NULL)
    {        
        if (pIpRxChannel->m_ChannelIndex == ChannelIndex)
            break;

        pIpRxChannel = pIpRxChannel->m_pNext;
    }
    DtFastMutexRelease(&pIpUserChannels->m_IpRxChAccesMutex);

    if (pIpRxChannel == NULL)
        DtDbgOut(ERR, IP_RX, "Could not find channel %d", ChannelIndex);
    
    DT_ASSERT(pIpRxChannel != NULL);
    return pIpRxChannel;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChDestroy -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxUserChDestroy(
    DtaIpUserChannels*  pIpUserChannels,
    UserIpRxChannel*  pIpRxChannel)
{
    // gain access to the channel list    
    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChAccesMutex);
    
    // Remove the channel from the list, make shure the workerthreads do not try to access 
    // the about to destroy channel. 
    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);

    DtDbgOut(AVG, IP_RX, "Destroying channel %i", pIpRxChannel->m_ChannelIndex);

    // Stop Brm updates
    DtaIpRxSetBrmSkipUpdate(pIpUserChannels, TRUE);

    // Wait for finishing processing
    DtaIpRxUserChRefWaitFinished(pIpRxChannel);

    // remove the channel from the list
    DtaIpRxUserChRemoveFromList(pIpUserChannels, pIpRxChannel);
    
    DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
    
    // free internal resources
    DtaIpRxRtpListsCleanup(pIpRxChannel);
    DtaShBufferClose(&pIpRxChannel->m_SharedBuffer);
    DtMemFreePool(pIpRxChannel, IPRX_TAG);
    pIpRxChannel = NULL;

   // Allow Brm updates and release mutexes
    DtaIpRxSetBrmSkipUpdate(pIpUserChannels, FALSE);

    DtFastMutexRelease(&pIpUserChannels->m_IpRxChAccesMutex);

    DtDbgOut(AVG, IP_RX, "Channel is destroyed");
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Private Channel Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIpRxUserChInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxUserChInit(UserIpRxChannel* pIpRxChannel)
{
    // Initialize concurrency objects
    DtSpinLockInit(&pIpRxChannel->m_RtpListSpinLock);
    DtSpinLockInit(&pIpRxChannel->m_StatisticsSpinLock);

    pIpRxChannel->m_RxControl = DT_RXCTRL_IDLE;
    pIpRxChannel->m_RxMode = DT_RXMODE_ST188;
    pIpRxChannel->m_FecMode = DTA_FEC_DISABLE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ClearBrmValues -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  ClearBrmValues(UserIpRxChannel* pIpRxChannel)
{
        // Bit-Rate Management
        pIpRxChannel->m_BrmNumPckBytesRcvd = 0;
        pIpRxChannel->m_BrmNumSamples = 0;
        pIpRxChannel->m_BrmIdxLastSample = 0;
        pIpRxChannel->m_BrmEstimate = 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxUserChReset(UserIpRxChannel* pIpRxChannel, Bool ClearInputStatus)
{
    if (pIpRxChannel->m_RxControl != DT_RXCTRL_IDLE) 
    {
        DtDbgOut(ERR, IP_RX, "Channel reset not allowed if channel is NOT IDLE");
        return;
    }    
    
    // Buffer and buffer state
    if(pIpRxChannel->m_pBufferHeader != NULL)
    {
        pIpRxChannel->m_pBufferHeader->m_FifoReadOffset = 0;
        pIpRxChannel->m_pBufferHeader->m_FifoWriteOffset = 0;
    }
    
    // Handshaking state between threads
    pIpRxChannel->m_RxIncomingPackets = FALSE;
    pIpRxChannel->m_RtpFirstPacket = TRUE;
        
    // FEC Statistics
    pIpRxChannel->m_NumPacketsNotReconstructed = 0;
    pIpRxChannel->m_NumPacketsReconstructed = 0;
    pIpRxChannel->m_TotNumPackets = 0;

    // Fragmented packets
    pIpRxChannel->m_FragmentId = 0;
    pIpRxChannel->m_FragmentOffset = 0;
    
    // Status flags and spin lock for access protection
    pIpRxChannel->m_Flags = 0;                // Clear DTA_RX_FIFO_OVF, DTA_RX_SYNC_ERR
    pIpRxChannel->m_LatchedFlags = 0;
    pIpRxChannel->m_TrpModePckSeqCount = 0;    // Start with sequence number zero

    // Should we reset the detected input status information?
    if (ClearInputStatus)
    {        
        ClearBrmValues(pIpRxChannel);
        pIpRxChannel->m_PckSizeSrc = DT_PCKSIZE_INV;

        // Detection status bytes
        pIpRxChannel->m_DetFecNumRows = 0;
        pIpRxChannel->m_DetFecNumColumns = 0;
        pIpRxChannel->m_DetNumTpPerIp = 0;
        pIpRxChannel->m_DetProtocol = DTA_PROTO_UNKN;
        pIpRxChannel->m_RstCntFecRow = 0;
        pIpRxChannel->m_RstCntFecColumn = 0;
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChAddToList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Add (insert) a channel in the RX channels list
//
// Pre: Mutex and spinlock to protect list must be aquired.
//
void  DtaIpRxUserChAddToList(
    DtaIpUserChannels*  pIpUserChannels, 
    UserIpRxChannel*  pIpRxNew)
{
    UserIpRxChannel*  pIpRxChannel = NULL;
    UserIpRxChannel*  pLastIpRx = NULL;
    Int  ChannelIndex = 0;

    DT_ASSERT(pIpRxNew != NULL);

    DtDbgOut(AVG, IP_RX, "Adding channel %i to list", pIpRxNew->m_ChannelIndex);

    // Just to be sure.
    pIpRxNew->m_pNext = NULL;
    pIpRxNew->m_pPrev = NULL;

    if (pIpUserChannels->m_pIpRxChannel == NULL)
    {   
        // No elements in list, add to first one
        pIpUserChannels->m_pIpRxChannel = pIpRxNew;
        return;
    }

    pIpRxChannel = pIpUserChannels->m_pIpRxChannel;
    pLastIpRx = pIpUserChannels->m_pIpRxChannel;

    // Insert the new channel into our list. The list is sorted.
    while (pIpRxChannel != NULL)
    {        
        // If an empty position in the list is found, insert the new channel
        if (pIpRxChannel->m_ChannelIndex != ChannelIndex)
        {
            // update the new channel prev/next pointers
            pIpRxNew->m_pPrev = pIpRxChannel->m_pPrev;
            pIpRxNew->m_pNext = pIpRxChannel;
            
            // Check if IpRx was the first in the list (m_pPrev == NULL)
            if (pIpRxChannel->m_pPrev != NULL)
                pIpRxChannel->m_pPrev->m_pNext = pIpRxNew;
            else
                pIpUserChannels->m_pIpRxChannel = pIpRxNew;

            // pIpRxChannel comes after the new channel
            pIpRxChannel->m_pPrev = pIpRxNew; 
            return;
        }         
        ChannelIndex++;
        pLastIpRx = pIpRxChannel;
        pIpRxChannel = pIpRxChannel->m_pNext;
    }
    // The channel must be inserted at the end
    DT_ASSERT(pLastIpRx->m_pNext == NULL);
    pLastIpRx->m_pNext = pIpRxNew;
    pIpRxNew->m_pPrev = pLastIpRx;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChRemoveFromList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Remove a channel from the list, and link the previous and next item in the list
//
void  DtaIpRxUserChRemoveFromList(
    DtaIpUserChannels*  pIpUserChannels, 
    UserIpRxChannel*  pIpRxChannel)
{
    DtDbgOut(AVG, IP_RX, "Removing channel %i from list", pIpRxChannel->m_ChannelIndex);

    if (pIpRxChannel->m_pPrev == NULL) 
    {
        // This was the first element in the list.
        pIpUserChannels->m_pIpRxChannel = pIpRxChannel->m_pNext;
        if (pIpRxChannel->m_pNext)
            pIpRxChannel->m_pNext->m_pPrev = NULL;
        return;
    }

    pIpRxChannel->m_pPrev->m_pNext = pIpRxChannel->m_pNext;
    if (pIpRxChannel->m_pNext)
        pIpRxChannel->m_pNext->m_pPrev = pIpRxChannel->m_pPrev;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChCreate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Create and returns an empty channel object. Only its identifiers are set.
//
UserIpRxChannel*  DtaIpRxUserChCreate(
    Int  ChannelIndex, 
    Int  IpPortIndex, 
    DtFileObject*  pFile)
{    
    // create a channel
    UserIpRxChannel*  pIpRxChannel = DtMemAllocPool(DtPoolNonPaged, 
                                                       sizeof(UserIpRxChannel), IPRX_TAG);

    if (pIpRxChannel == NULL)
    {
        DtDbgOut(ERR, IP_RX, "Not enough memory to allocate IP RX Channel");
        return NULL;
    }

    DtMemZero(pIpRxChannel, sizeof(UserIpRxChannel));
    pIpRxChannel->m_pNext = NULL;
    pIpRxChannel->m_pPrev = NULL;
    pIpRxChannel->m_ChannelIndex = ChannelIndex;
    pIpRxChannel->m_IpPortIndex = IpPortIndex;
    
    // identifies channels to clean for channels that where never detached 
    pIpRxChannel->m_FileObject = *pFile;

    // Init shared buffer
    // DTAPI will create a shared buffer later.
    pIpRxChannel->m_SharedBuffer.m_pBuffer = NULL;
    pIpRxChannel->m_SharedBuffer.m_pDmaCh = NULL;
    pIpRxChannel->m_SharedBuffer.m_Initialised = FALSE;
    pIpRxChannel->m_SharedBuffer.m_Purpose = DTA_SH_BUF_PURPOSE_GEN;

    // Initialize RtpLists
    if(DtaIpRxRtpListsInit(pIpRxChannel) != DT_STATUS_OK)
    {
        DtDbgOut(ERR, IP_RX, "Unable to initialize Rtp lists");
        DtMemFreePool(pIpRxChannel, IPRX_TAG);
        pIpRxChannel = NULL;        
    }

    DtDbgOut(AVG, IP_RX, "Created channel %i", ChannelIndex);
    return pIpRxChannel;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxNrtCleanupBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxNrtCleanupBuffer(DtaIpNrtChannels* pNrtChannels)
{
    DtStatus  Status = DT_STATUS_OK;

    if (pNrtChannels->m_RxSharedBuf.m_pBuffer == NULL)
        return DT_STATUS_OK;

    DtMemFreePool(pNrtChannels->m_RxSharedBuf.m_pBuffer, IPRX_TAG);
    pNrtChannels->m_RxSharedBuf.m_pBuffer = NULL;
    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxNrtCreateBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Create Nrt shared buffer for IP packets Rx to Network driver
//
DtStatus  DtaIpRxNrtCreateBuffer(DtaIpNrtChannels* pNrtChannels)
{
    DtStatus  Status = DT_STATUS_OK;
    
    // Create buffer
    pNrtChannels->m_RxSharedBuf.m_pBuffer = DtMemAllocPool(DtPoolNonPaged, 
                                                 DTA_IPRX_NRT_SHARED_BUF_SIZE , IPRX_TAG);
    if (pNrtChannels->m_RxSharedBuf.m_pBuffer == NULL)
        return DT_STATUS_OUT_OF_MEMORY;
    
    // Initialize buffer
    pNrtChannels->m_RxSharedBuf.m_BufSize = DTA_IPRX_NRT_SHARED_BUF_SIZE - 
                                                           sizeof(DtaIpNwSharedBufInfo);
    pNrtChannels->m_RxSharedBuf.m_pSharedInfo = (DtaIpNwSharedBufInfo*)
                                                  (pNrtChannels->m_RxSharedBuf.m_pBuffer +
                                                  pNrtChannels->m_RxSharedBuf.m_BufSize);
    
    // Clear memory
    DtMemZero(pNrtChannels->m_RxSharedBuf.m_pSharedInfo, sizeof(DtaIpNwSharedBufInfo));
    // Init event
    DtEventInit(&pNrtChannels->m_RxSharedBuf.m_pSharedInfo->m_DataAvailableEvent, TRUE);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxCleanupDmaBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxCleanupDmaBuffer(DmaChannel* pDmaChannel, PPBuffer* pPPBuffer)
{
    DtStatus  Status = DT_STATUS_OK;

    Status = DtaDmaCleanupKernelBuffer(pDmaChannel, pPPBuffer->m_pBufStart,
                                                        pPPBuffer->m_pPageList, IPRX_TAG);
    pPPBuffer->m_pBufStart = NULL;
    pPPBuffer->m_pPageList = NULL;
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetLocalAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxGetLocalAddress(
    void*  pContext,
    UInt8**  pLocalAddress,
    UInt*  pStart,
    UInt*  pStop)
{
    DtaIpPort*  pIpPort = (DtaIpPort*)pContext;

    *pStart = 0;
    *pStop = 0;

    DT_ASSERT(pIpPort->m_PortType == DTA_IPPORT_TYPE1);

    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        *pLocalAddress = (UInt8*)(Int)pIpPort->m_IpPortType1.m_Rx.m_FifoOffset;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxCreateDmaBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Create DMA ping-pong buffer for DMA transfers
// This is one large buffer that is divided in 2 parts
// Note: The SGL buffer in common memory is already created in DtaDmaInitCh()
//
DtStatus  DtaIpRxCreateDmaBuffer(
    DmaChannel*  pDmaChannel, 
    PPBuffer*  pPPBuffer, 
    DtaIpPort*  pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    void*  pBuffer = NULL;
    DtPageList* pPageList = NULL;
    
    // Create buffer
    Status = DtaDmaCreateKernelBuffer(pDmaChannel, DT_DMA_DIRECTION_FROM_DEVICE, &pBuffer,
                                        &pPageList, DTA_IPRX_PINGPONG_BUF_SIZE, IPRX_TAG);
    
    // Initialize pingpong buffer
    DtaPPBufferInitialise(pPPBuffer, pBuffer, pPageList, DTA_IPRX_PINGPONG_BUF_SIZE, 
                                     pDmaChannel, DtaIpRxGetLocalAddress, pIpPort, FALSE);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxIsDmaHeaderCorrect -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline Bool  DtaIpRxIsDmaHeaderCorrect(DtaDmaRxHeader* pRxHeader)
{
    return (pRxHeader->m_Tag==DTA_IPRX_DMA_HEADER_TAG && 
                                  pRxHeader->m_Length==sizeof(DtaDmaRxHeader) &&
                                  pRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength<=
                                  DTA_IPRX_MAX_PACKET_SIZE);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxSendPacketToNwDriver -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxSendPacketToNwDriver(
    DtaIpPort*  pIpPort,
    DtaDmaRxHeader*  pDmaRxHeader,
    UInt8*  pPacket,
    UInt  PacketSize)
{
    UInt  FreeSpaceUntilEnd;
    UInt  FreeSpaceFromBegin;
    UInt  ReadOffset;
    UInt  WriteOffset;
    UInt  TotalLength;
    UInt8*  pDst;
    DtaIpNrtSharedBuf*  pIpRxSharedBuf = &pIpPort->m_NrtChannels.m_RxSharedBuf;
    DtaIpNwSharedBufInfo*  pSharedInfo = pIpRxSharedBuf->m_pSharedInfo;
    
    if (!pIpPort->m_PhyMac.m_NwDrvConnected)
    {
        DtDbgOut(MAX, IP_RX, "Network driver not connected. Packet discarded");
        return;
    }
    if (pIpPort->m_PhyMac.m_NwDrvPacketFilter == 0)
    {
        DtDbgOut(MAX, IP_RX, "Network driver not interested in packets");
        return;
    }

    TotalLength = PacketSize + sizeof(DtaDmaRxHeader);
    ReadOffset = pSharedInfo->m_ReadOffset;
    WriteOffset = pSharedInfo->m_WriteOffset;

    // Calculate free space
    if (WriteOffset < ReadOffset) 
    {
        FreeSpaceFromBegin = 0;
        FreeSpaceUntilEnd = ReadOffset - WriteOffset - 1;
    } else {
        FreeSpaceUntilEnd = pIpRxSharedBuf->m_BufSize - WriteOffset - 1;
        if (ReadOffset == 0)
            FreeSpaceFromBegin = 0;
        else
            FreeSpaceFromBegin = ReadOffset - 1;
    }

    if (FreeSpaceUntilEnd >= TotalLength)
        pDst = (pIpRxSharedBuf->m_pBuffer + WriteOffset);
    else if (FreeSpaceFromBegin >= TotalLength) 
    {
        // Mark dummy bytes until end.
        pIpRxSharedBuf->m_pBuffer[WriteOffset] = '*';
        WriteOffset = 0;
        pDst = pIpRxSharedBuf->m_pBuffer + WriteOffset;
    } else {
        // Not enough room, skip pakket
        DtDbgOut(MAX, IP_RX, "Shared buffer full. Packet discarded");
        return;
    }
    DtMemCopy(pDst, pDmaRxHeader, sizeof(DtaDmaRxHeader));
    pDst+= sizeof(DtaDmaRxHeader);
    DtMemCopy(pDst, pPacket, PacketSize);

    // Align packets on DWORD's
    if (PacketSize%4 != 0)
        TotalLength += 4 - (PacketSize % 4);

    WriteOffset+= TotalLength;
    if (WriteOffset >= pIpRxSharedBuf->m_BufSize)
        WriteOffset = 0;
    
    pSharedInfo->m_WriteOffset = WriteOffset;

    // Trigger Network driver.
    DtAtomicIncrement(&pSharedInfo->m_DataAvailableCnt);
    DtEventSet(&pSharedInfo->m_DataAvailableEvent);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxIsPacketForDVB -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt DtaIpRxIsPacketForDVB(
    DtaIpUserChannels*  pIpUserChannels,
    RtpHeader*  pRtpHeader,
    UInt8*  pIpSrc,
    UInt8*  pIpDst, 
    UInt16   SrcPort,
    UInt16 DstPort,
    Bool  MulticastPkt,            // True: If packet received is a Multicast packet
    UserIpRxChannel**  pIpRxA)
{
    Int  i;
    UserIpRxChannel*  pIpRxChannel;
    Bool  Stop;
    Int  Index = 0;
    Bool  IsFecPort = FALSE;

    // Wait until the Rx structure is available
    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);

    // Search the Rx channels for the DVB stream and return the ChannelIndex
    // if found
    pIpRxChannel = pIpUserChannels->m_pIpRxChannel;
    while (pIpRxChannel != NULL) 
    {
        // Check if the channel is the one we are searching for

        // For bitrate calculation to work in idle state, 
        // we check the RxControl somewhere else and only check here 
        // if the parameters are set. 
        if (!pIpRxChannel->m_IpParsValid)
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }

        // Check if source + destination IP address is correct.
        Stop = FALSE;
        for (i=0; i<4; i++) 
        {
            if (pIpRxChannel->m_SrcIPAddress[i]!=0 && 
                                               pIpRxChannel->m_SrcIPAddress[i]!=pIpSrc[i])
            {
                Stop = TRUE;
                break;
            }

            // If a multicast packet is received, 
            // the m_DstIPAddress may not be 0.0.0.0
            if ((pIpRxChannel->m_DstIPAddress[i]!=0 || MulticastPkt) && 
                                              pIpRxChannel->m_DstIPAddress[i]!=pIpDst[i])
            {
                Stop = TRUE;
                break;
            }
        }

        if (Stop) 
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }
        
        // Check if the source port number is correct
        if (pIpRxChannel->m_SrcPort!=0 &&  pIpRxChannel->m_SrcPort!=SrcPort)
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }

        // If a RTP header is available, we must check on its contents
        // to determine if we must add the packet to a FEC stream
        // Add the channel if we cannot use the contents of the packet, and
        // the destiny port matches a FEC port.
        IsFecPort = FALSE;
        if (pIpRxChannel->m_FecMode != DTA_FEC_DISABLE)
        {
            if (pIpRxChannel->m_DstPort+FEC_INC_COLUMN_PORT == DstPort ||
                                      pIpRxChannel->m_DstPort+FEC_INC_ROW_PORT == DstPort)
            {
                if (pRtpHeader != NULL)
                {
                    // If we have a RTP header, determine if this is a FEC packet
                    if(pRtpHeader->m_PayloadType == RTP_PAYLOAD_FEC && 
                                                               pRtpHeader->m_Version == 2)
                        IsFecPort = TRUE;                    
                } else{
                    // if we do not have a RTP header, assume that this is a FEC packet
                    IsFecPort = TRUE;
                }
            }           
        }                

        // If the destiny port matches or if the portnumber it used for a FEC stream
        // add the channel.
        if (pIpRxChannel->m_DstPort==DstPort || IsFecPort)               
        {
            pIpRxA[Index++] = pIpRxChannel;
            
            // Increment use count
            DtaIpRxUserChRefAdd(pIpRxChannel);
            
            if (Index == MAX_NUM_IPRX_LISTEN) 
            {
                DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
                return MAX_NUM_IPRX_LISTEN;
            }
        }
        pIpRxChannel = pIpRxChannel->m_pNext;
    }
    DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
    return Index;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxIsFragmentedPacketForDVB -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Checks if the received fragmented IP packet is for one of the DVB streams. If so,
// an array with the ChannelIndex'es and the number of channels found is returned
//
UInt  DtaIpRxIsFragmentedPacketForDVB(
    DtaIpUserChannels*  pIpUserChannels,
    UInt16  FragmentID,
    UInt16  IpFragmentOffset,
    UserIpRxChannel**  pIpRxA)
{
    UserIpRxChannel*  pIpRxChannel;
    Int  Index = 0;

    // Wait until the Rx structure is available
    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);
    
    // Search the Rx channels for the DVB stream and return the ChannelIndex
    // if found
    pIpRxChannel = pIpUserChannels->m_pIpRxChannel;
    while (pIpRxChannel != NULL) 
    {
        // Check if the channel is the one we are searching for

        // For bitrate calculation to work in idle state, 
        // we check the RxControl somewhere else and only check here 
        // if the parameters are set. 
        if (!pIpRxChannel->m_IpParsValid) 
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }

        // Check if frament identification is identical.
        if (pIpRxChannel->m_FragmentId != FragmentID) 
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }

        // Check if the fragment offset is not
        // exceeding max. packet length of 64kb
        // Note: FragmentOffset is given in units of 8-byte blocks!
        //       See IP4 specs.
        if (IpFragmentOffset < 0x2000) 
        {
            pIpRxA[Index++] = pIpRxChannel;
            
            // Increment reference
            DtaIpRxUserChRefAdd(pIpRxChannel);

            if (Index == MAX_NUM_IPRX_LISTEN) 
            {
                DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
                return MAX_NUM_IPRX_LISTEN;
            }
        }
        pIpRxChannel = pIpRxChannel->m_pNext;
    }
    DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
    return Index;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxParseFragmentedFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Determine if the incoming DMA packet is part of a fragmented frame.
// Return the channel holding a complete frame if the frame is complete.
// Return status values to determine further actions after calling this function
UserIpRxChannel*  DtaIpRxParseFragmentedFrame(
    DtaIpUserChannels*  pIpUserChannels,
    DtaDmaRxHeader*  pDmaRxHeader,
    UInt32  Size,
    IpHeaderV4*  pIpHeader,
    Bool  MulticastPacket,
    DtaIpPort*  pIpPort,
    Bool*  pStopParsing)  // Output
{    
    UInt  i;
    UInt16  IpFragmentOffset = 0;
    UInt8*  pIpPacketData;
    Int  IpPacketDataSize;
    UdpHeader*  pUdpHeader;

    UInt16  DestPort;
    UInt  NumChannels;
    UserIpRxChannel*  pIpRxChannel;
    UserIpRxChannel*  pIpRxA[MAX_NUM_IPRX_LISTEN];
    UserIpRxChannel*  pIpRxFragment = NULL;

    Int  PacketSize = Size - 4 - sizeof(DtaDmaRxHeader);
    UInt8*  pRxData = (UInt8*)pDmaRxHeader + sizeof(DtaDmaRxHeader);

    // Shift high and low both into one UInt16. 
    // Note: IpFragmentOffset is in units of 8 bytes (See IpV4 specs)
    IpFragmentOffset = ((UInt16)pIpHeader->m_FragmentOffsetH << 8) + 
                                                   ((UInt16)pIpHeader->m_FragmentOffsetL);

    // Initialise output parameters
    *pStopParsing = FALSE;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- First fragment -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // First fragmented packet:
    // Copy data including headers into the channel buffer
    if ((pIpHeader->m_Flags & IPV4_HDR_FLAGS_MOREFRAG)!=0 && IpFragmentOffset==0)
    {

        // Check if we have a valid UDP header
        if (pIpHeader->m_Protocol != IPV4_HDR_PROT_UDP)   
        {
            DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
            *pStopParsing = TRUE;
            return NULL;
        }

        DtDbgOut(MAX, IP_RX, "FIRST fragment: %d", 
                                           DtUInt16ByteSwap(pIpHeader->m_Identification));

        // Check if it is for one of our channels
        pUdpHeader = (UdpHeader*)((UInt8*)pIpHeader + (pIpHeader->m_HeaderLength * 4));
        DestPort = DtUInt16ByteSwap(pUdpHeader->m_DestinationPort);
        NumChannels = DtaIpRxIsPacketForDVB(pIpUserChannels, NULL,
                                         pIpHeader->m_SrcAddress, pIpHeader->m_DstAddress,
                                         DtUInt16ByteSwap(pUdpHeader->m_SourcePort),
                                         DestPort, MulticastPacket, pIpRxA);

        if (NumChannels == 0)
        {
            DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
            *pStopParsing = TRUE;
            return NULL;
        }       

        DtDbgOut(MAX, IP_RX, "Storing FIRST fragment: %d", 
                                           DtUInt16ByteSwap(pIpHeader->m_Identification));

        for (i=0; i<NumChannels; i++) 
        {
            pIpRxChannel = pIpRxA[i];
            pIpRxChannel->m_FragmentId = pIpHeader->m_Identification;
            // Store IP Header offset offset for later use
            pIpRxChannel->m_CurIpHeaderOffset = 
                                       (UInt16)((UInt8*)pIpHeader - (UInt8*)pDmaRxHeader);
            
            // Copy data including Dma/Eth/IP header into temp. buffer
            // excluding checksum
            DtMemCopy(pIpRxChannel->m_pRtpBuffer, pDmaRxHeader, Size - 4);
            pIpRxChannel->m_FragmentOffset = (UInt16)Size - 4;
            
            // Decrement use count
            DtaIpRxUserChRefDecr(pIpRxChannel);
        }
        *pStopParsing = TRUE;
        return NULL;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Second..Last-1  Fragment -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    // Not first or last fragmented packet. 
    // Copy only the data into the channel buffer
    // Check all channels to see if current fragment belongs to a particular channel 
    if ((pIpHeader->m_Flags & IPV4_HDR_FLAGS_MOREFRAG)!=0 && IpFragmentOffset!=0)
    {
        NumChannels = DtaIpRxIsFragmentedPacketForDVB(pIpUserChannels, 
                                   pIpHeader->m_Identification, IpFragmentOffset, pIpRxA);

        DtDbgOut(MAX, IP_RX, "NEXT fragment: %d Offset: %d",  
                                            DtUInt16ByteSwap(pIpHeader->m_Identification),
                                            IpFragmentOffset*8);

        if (NumChannels == 0) 
        {
            DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
            *pStopParsing = TRUE;
            return NULL;
        }

        DtDbgOut(MAX, IP_RX, "Storing NEXT fragment: %d", 
                                           DtUInt16ByteSwap(pIpHeader->m_Identification));

        // We have to append packet data in the user channel buffer
        pIpPacketData = (UInt8*)pIpHeader + (pIpHeader->m_HeaderLength * 4);
        IpPacketDataSize = Size - 
            DtPtrToUInt((void*)(pIpPacketData - (UInt8*)pDmaRxHeader)) - 4; // -4 checksum
        for (i=0; i<NumChannels; i++)
        {
            pIpRxChannel = pIpRxA[i];

            // Packet size max. 64kb
            if ((IpPacketDataSize + pIpRxChannel->m_FragmentOffset) > 0xffff) 
            {
                // Size too big, skip packet;
                pIpRxChannel->m_FragmentId = 0;
                pIpRxChannel->m_FragmentOffset = 0;
            } else {
                // Copy data into temp. buffer
                DtMemCopy(pIpRxChannel->m_pRtpBuffer + pIpRxChannel->m_FragmentOffset, 
                                                         pIpPacketData, IpPacketDataSize);
                pIpRxChannel->m_FragmentOffset+=(UInt16)IpPacketDataSize;
            }

            // Decrement use count
            DtaIpRxUserChRefDecr(pIpRxChannel);
        }
        *pStopParsing = TRUE;
        return NULL;
    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Last fragment -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    // Handle last fragment
    if ((pIpHeader->m_Flags & IPV4_HDR_FLAGS_MOREFRAG)==0 && IpFragmentOffset != 0)
    {
        DtDbgOut(MAX, IP_RX, "LAST fragment: %d Offset: %d",  
                                            DtUInt16ByteSwap(pIpHeader->m_Identification),
                                            IpFragmentOffset*8);

        NumChannels = DtaIpRxIsFragmentedPacketForDVB(pIpUserChannels,  
                                      pIpHeader->m_Identification, IpFragmentOffset, pIpRxA);
        if (NumChannels == 0) 
        {
            DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
            *pStopParsing = TRUE;
            return NULL;
        }

        DtDbgOut(MAX, IP_RX, "Storing LAST fragment: %d", pIpHeader->m_Identification);

        // We have to copy the fragmented packet into the buffer
        pIpPacketData = (UInt8*)pIpHeader + (pIpHeader->m_HeaderLength * 4);
        IpPacketDataSize = Size - 
                           DtPtrToUInt((void*)(pIpPacketData - (UInt8*)pDmaRxHeader)) - 4;

        // Try to find the complete packet
        for (i=0; i<NumChannels; i++) 
        {
            UInt UdpLength = 0;

            // Check if this packet is complete. 
            // If so, use this packet
            // Set pointer to the UDP header
            pIpRxChannel = pIpRxA[i];
            pIpHeader = (IpHeaderV4*)(pIpRxChannel->m_pRtpBuffer + 
                                                    pIpRxChannel->m_CurIpHeaderOffset);

            pUdpHeader = (UdpHeader*)((UInt8*)pIpHeader + (pIpHeader->m_HeaderLength * 4));
            UdpLength = pIpRxChannel->m_FragmentOffset + IpPacketDataSize - 
                                        sizeof(DtaDmaRxHeader) - sizeof(EthernetIIHeader) - 
                                        (pIpHeader->m_HeaderLength * 4);

            if (DtUInt16ByteSwap(pUdpHeader->m_Length) > UdpLength) 
                continue;

            if  (IpPacketDataSize+pIpRxChannel->m_FragmentOffset > 0xffff) 
            {
                // Packet to large
                pIpRxChannel->m_FragmentId = 0;
                pIpRxChannel->m_FragmentOffset = 0;
                continue;
            }

            // Copy data into temp. buffer and update channel values
            DtMemCopy(pIpRxChannel->m_pRtpBuffer + pIpRxChannel->m_FragmentOffset, 
                                                                     pIpPacketData, Size);
            pIpRxChannel->m_FragmentOffset+=(UInt16)IpPacketDataSize;
            pDmaRxHeader = (DtaDmaRxHeader*)(pIpRxChannel->m_pRtpBuffer);
            pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength = 
                                                           pIpRxChannel->m_FragmentOffset;
            // Output the found channel that needs to be processed
            pIpRxFragment = pIpRxChannel;
            break;
        }

        // Decrement use count, except for the fragmented packet channel
        for (i=0; i<NumChannels; i++) 
        {
            pIpRxChannel = pIpRxA[i];
            if (pIpRxChannel != pIpRxFragment)
                DtaIpRxUserChRefDecr(pIpRxChannel);
        }

        // No complete packet found
        // skip all and return
        if (pIpRxFragment == NULL) 
            return NULL;
    }

    // At this point our fragmented frame should be complete.
    // or the packet is not fragmented
    return pIpRxFragment;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIpRxProcessDvbPayLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Process the payload of an IP packet for a given IP Rx channel.
//
// NOTE:
// This routine must be called even if Receive-Control state is IDLE, so that bit-rate
// measurement and status-flag computation are active at all times.
//
void  DtaIpRxProcessDvbPayLoad(
    UserIpRxChannel*  pIpRxChannel, // IP Rx channel
    UInt8*  pPayLoad,               // IP payload with packet data
    Int  PayLoadSize,               // Size of payload in #bytes
    UInt64  TimeStamp,              // Time stamp @54MHz
    Int  NumUdpRtpPktLost)          // Number of UDP/RTP packets that where lost
{
    Int  Add16Zeros = 0;            // Add 16 zero bytes
    UInt  NumFree;                  // Number of free bytes in buffer
    UInt  NumPckts;                 // Number of TPs in IP packet
    UInt  PckSizeToCopy;            // #bytes to copy for each packet
    UInt  PckSizeDst;               // Size of TPs (+opt. time stamp) written to buffer
    Int  SyncErr;                   // Synchronisation Error     
    UInt8  NumTpPerIp = 0;          // Calculated number of Tp packets per Ip.
    Int  PckSizeSrc;                // Size of TPs calculated from source buffer

    UInt8*  pRead;                  // Read pointer
    UInt8*  pWrite;                 // Write pointer
    UInt8*  pWrapArea;              // Start of the wrap area

    // Determine Transport-Packet size from IP-packet size
    if ((PayLoadSize % 188) == 0) 
    {
        PckSizeSrc = 188;
        NumPckts = PayLoadSize / 188;
    } else if ((PayLoadSize % 204) == 0) 
    {
        PckSizeSrc = 204;
        NumPckts = PayLoadSize / 204;
    } else {        
        pIpRxChannel->m_PckSizeSrc = DT_PCKSIZE_INV;   // INVALID
        return;                                        // Not a valid IP packet
    }

    // Note: pIpRx->m_PckSizeSrc can be reset by the bitrate calculation thread
    pIpRxChannel->m_PckSizeSrc = PckSizeSrc;

    // Update last Rx timestamp for bitrate calculations
    pIpRxChannel->m_BrmLastRxTimeStamp = TimeStamp;
    
    // Update total #packet bytes received for bitrate calculations
    if ((pIpRxChannel->m_RxMode & DT_RXMODE_MASK) == DT_RXMODE_STRAW) 
    {
        pIpRxChannel->m_BrmNumPckBytesRcvd += NumPckts*PckSizeSrc;
    } else {
        // Always based on 188-byte packets if not in RAW mode.
        pIpRxChannel->m_BrmNumPckBytesRcvd += NumPckts*188;
    }

    // Compute "destination" packet size and PckSizeToCopy: this is the #packet bytes
    // copied from IP-payload to channel buffer = min(PckSizeSrc, PckSizeDst)
    switch (pIpRxChannel->m_RxMode & DT_RXMODE_MASK) 
    {
    case DT_RXMODE_ST188:
        PckSizeDst = PckSizeToCopy = 188;
        break;

    case DT_RXMODE_ST204:
        PckSizeDst = 204;
        if (PckSizeSrc == 188) 
        {
            PckSizeToCopy = 188;
            Add16Zeros = 1;
        } else
            PckSizeToCopy = 204;
        break;

    case DT_RXMODE_STMP2:
    case DT_RXMODE_STRAW:
        PckSizeDst = PckSizeToCopy = PckSizeSrc;
        break;

    case DT_RXMODE_STTRP:
        PckSizeDst = 208; // Transparent-mode-packet are 208 byte long
        PckSizeToCopy = PckSizeSrc;
        if (PckSizeSrc == 188)
            Add16Zeros = 1;
        break;

    default:
        DtDbgOut(ERR, IP_RX, "Channel %d: INVALID RX MODE", pIpRxChannel->m_RxMode);
        return;            // Invalid packet mode
    }
    // Add space for time stamp, if required
    if ((pIpRxChannel->m_RxMode & DT_RX_TIMESTAMP) != 0) 
        PckSizeDst += 4;

    DtDbgOut(MAX, IP_RX, "Channel %d: Handle packet PckSizeSrc=%d PckSizeDst=%d", 
                                    pIpRxChannel->m_ChannelIndex, PckSizeSrc, PckSizeDst);

    // Don't store packet when Receive-Control state is IDLE
    if (pIpRxChannel->m_RxControl == DT_RXCTRL_IDLE)
        return;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Overflow Check -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Compute free buffer space
    // Take local copies of pointer for ease of reference, and to be sure they will not
    // be changed by another thread (for pRead only)
    pRead = pIpRxChannel->m_pFifo + pIpRxChannel->m_pBufferHeader->m_FifoReadOffset;
    pWrite = pIpRxChannel->m_pFifo + pIpRxChannel->m_pBufferHeader->m_FifoWriteOffset;
    pWrapArea = pIpRxChannel->m_pFifo + pIpRxChannel->m_FifoSize;    
    NumFree = (UInt)(pRead>pWrite ? 
                                  pRead-pWrite : pIpRxChannel->m_FifoSize-(pWrite-pRead));

    DtDbgOut(MAX, IP_RX, "Channel %d: NumFree=%d pRead=0x%p pWrite=0x%p", 
                                    pIpRxChannel->m_ChannelIndex, NumFree, pRead, pWrite);


    // Check for overflow
    // Compare with the maximum #bytes that may be written for a single IP packet
    // Transparent packets are 208 bytes long
    if (NumFree < (7*208 + 1)) 
    {    // PckSizeToCopy + TimeStamp, +1 to prevent pRead == pWrite
         DtDbgOut(AVG, IP_RX, "OVERFLOW");

        // Overflow!
        // No need for spinlock: m_Flags is only written from Worker thread
        pIpRxChannel->m_Flags |= DTA_RX_FIFO_OVF;

        // Acquire spin lock for updating latched flags
        DtSpinLockAcquire(&pIpRxChannel->m_FlagsSpinLock);
        pIpRxChannel->m_LatchedFlags |= DTA_RX_FIFO_OVF;
        DtSpinLockRelease(&pIpRxChannel->m_FlagsSpinLock);
        return;
    }
    // No overflow
    pIpRxChannel->m_Flags &= ~DTA_RX_FIFO_OVF;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Write Packet to Buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // The code below works because the buffer is made 256 bytes longer than the size
    // that is actually used. This way, packets that would wrap the buffer are written
    // partly in the extra 256 bytes. The overflow bytes are then copied back to the
    // beginning of the buffer.

    // Walk through payload. pPayLoad is running source pointer, payload
    // size remaining is kept in PayLoadSize, buffer write pointer in m_pWrite
    SyncErr = 0;
    while (PayLoadSize > 0) 
    {

        // Check for synchronisation error
        if (*pPayLoad != 0x47) 
        {
            DtDbgOut(ERR, IP_RX, "SYNC ERROR");
            SyncErr++;
        }
        NumTpPerIp++;

        // Start with writing time stamp, if specified to do so
        if ((pIpRxChannel->m_RxMode & DT_RX_TIMESTAMP) != 0) 
        {
            *(UInt32*)pWrite = (UInt32)TimeStamp;
            pWrite += 4;
        }

        // Payload
        DtMemCopy(pWrite, pPayLoad, PckSizeToCopy);
        pPayLoad += PckSizeSrc;
        PayLoadSize -= PckSizeSrc;
        pWrite += PckSizeToCopy;

        // Adding 16-filler bytes
        if (Add16Zeros != 0) 
        {
            memset(pWrite, 0, 16);
            pWrite += 16;
        }

        // Add Transparent-Mode-Packet trailer, if needed
        if ((pIpRxChannel->m_RxMode & DT_RXMODE_MASK) == DT_RXMODE_STTRP) 
        {
            // Account for lost UDP/RTP packets
            if ( NumUdpRtpPktLost > 0 )
                pIpRxChannel->m_TrpModePckSeqCount += (NumUdpRtpPktLost * 
                                                           pIpRxChannel->m_DetNumTpPerIp);
            
            *pWrite++ = 0x58; // Sync nibble ("0101"), packet_sync and reserved bits
            *pWrite++ = (UInt8)PckSizeSrc; // Valid count
            *pWrite++ = pIpRxChannel->m_TrpModePckSeqCount&0xFF; // Sequence cnt (LSB)
            *pWrite++ = (pIpRxChannel->m_TrpModePckSeqCount>>8)&0xFF; // Sequence cnt(MSB)
            pIpRxChannel->m_TrpModePckSeqCount++; // Increment packet sequence count
        }

        // Check whether packet extends beyond end-of-buffer
        // If so, copy excess bytes back to start of buffer
        if (pWrite >= pWrapArea) 
        {
            DtMemCopy(pIpRxChannel->m_pFifo, pWrapArea, (UInt)(pWrite-pWrapArea));
            pWrite -= pIpRxChannel->m_FifoSize;
        }
    }        
    pIpRxChannel->m_pBufferHeader->m_FifoWriteOffset = 
                                     DtPtrToUInt((void*)(pWrite - pIpRxChannel->m_pFifo));

    // Update Synchronisation-Error flag
    if (SyncErr != 0) 
    {
        // Sync error!
        // No need for spinlock: m_Flags is only written from Worker thread
        pIpRxChannel->m_Flags |= DTA_RX_SYNC_ERR;

        // Acquire spin lock for updating latched flags
        DtSpinLockAcquire(&pIpRxChannel->m_FlagsSpinLock);
        pIpRxChannel->m_LatchedFlags |= DTA_RX_SYNC_ERR;
        DtSpinLockRelease(&pIpRxChannel->m_FlagsSpinLock);
    } else {
        pIpRxChannel->m_Flags &= ~DTA_RX_SYNC_ERR;
        pIpRxChannel->m_DetNumTpPerIp = NumTpPerIp;
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIpRxProcessIpRawPayLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Store the raw IP packet into the user buffer including a new header
//
void  DtaIpRxProcessIpRawPayLoad(
    UserIpRxChannel*  pIpRxChannel, // IP Rx channel
    UInt8*  pPayLoad,               // IP payload with packet data
    Int  PayLoadSize,               // Size of payload in #bytes
    UInt64  TimeStamp,              // Time stamp @54MHz
    Bool  Header)                   // Include header
{
    DtaIpRawHeader  IpRawHeader;
    UInt  NumBytesToEnd;            // Number of bytes from pWrite till end of buffer
    UInt  NumBytesToCopy;           // Number of bytes to copy
    UInt  NumBytes;                 // Temp. store
    UInt  NumFree;                  // Number of free bytes in buffer
    UInt8*  pRead;                  // Read pointer
    UInt8*  pWrite;                 // Write pointer
    UInt8*  pWrapArea;              // Start of the wrap area    
    
    // Update last Rx timestamp for bitrate calculations
    pIpRxChannel->m_BrmLastRxTimeStamp = TimeStamp;

    // Update total #packet bytes received
    // NOTE: Includes FEC packets + all IP Headers
    pIpRxChannel->m_BrmNumPckBytesRcvd += PayLoadSize;

    // Don't store packet when Receive-Control state is IDLE
    if (pIpRxChannel->m_RxControl == DT_RXCTRL_IDLE)
        return;

    IpRawHeader.m_Tag = 0x44A0;
    IpRawHeader.m_TimeStamp = (UInt32)TimeStamp;
    IpRawHeader.m_Length = (UInt16)PayLoadSize;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Overflow Check -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Compute free buffer space
    // Take local copies of pointer for ease of reference, and to be sure they will not
    // be changed by another thread (for pRead only)
    pRead = pIpRxChannel->m_pFifo + pIpRxChannel->m_pBufferHeader->m_FifoReadOffset;
    pWrite = pIpRxChannel->m_pFifo + pIpRxChannel->m_pBufferHeader->m_FifoWriteOffset;
    pWrapArea = pIpRxChannel->m_pFifo + pIpRxChannel->m_FifoSize;

    NumFree = (UInt)(pRead>pWrite ? 
                                  pRead-pWrite : pIpRxChannel->m_FifoSize-(pWrite-pRead));

    DtDbgOut(MAX, IP_RX, "Channel %d: NumFree=%d pRead=0x%p pWrite=0x%p", 
                                    pIpRxChannel->m_ChannelIndex, NumFree, pRead, pWrite);

    // Check for overflow
    // Compare with the maximum #bytes that may be written for a single IP packet
    if (NumFree < (Int)(sizeof(DtaIpRawHeader)+ PayLoadSize+1)) 
    { // +1 to prevent pRead == pWrite

        DtDbgOut(ERR, IP_RX, "OVERFLOW");
        // Overflow!
        // No need for spinlock: m_Flags is only written from Worker thread
        pIpRxChannel->m_Flags |= DTA_RX_FIFO_OVF;

        // Acquire spin lock for updating latched flags
        DtSpinLockAcquire(&pIpRxChannel->m_FlagsSpinLock);
        pIpRxChannel->m_LatchedFlags |= DTA_RX_FIFO_OVF;
        DtSpinLockRelease(&pIpRxChannel->m_FlagsSpinLock);
        return;
    }
    // No overflow
    pIpRxChannel->m_Flags &= ~DTA_RX_FIFO_OVF;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Write Packet to Buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // The code below works because the buffer is made 256 bytes longer than the size
    // that is actually used (Wrap area). 
    if (Header) 
    {
        // Copy header to buffer
        DtMemCopy(pWrite, &IpRawHeader, sizeof(IpRawHeader));
        pWrite+=sizeof(IpRawHeader);

        // Check whether header extends beyond end-of-buffer
        // If so, copy excess bytes back to start of buffer
        if (pWrite >= pWrapArea) 
        {
            DtMemCopy(pIpRxChannel->m_pFifo, pWrapArea, 
                                            (UInt)DtPtrToUInt((void*)(pWrite-pWrapArea)));
            pWrite -= pIpRxChannel->m_FifoSize;
        }
    }

    // Ethernet packet can be larger then the extra 256 bytes, 
    // so split the copy if packet is larger then NumBytesToEnd.
    NumBytesToCopy = PayLoadSize;

    while (NumBytesToCopy) 
    {        
        // NumBytesToEnd = DtPtrToUInt((void*)(pWrapArea - pWrite));
        NumBytesToEnd = DtPtrToUInt((void*)(pWrapArea - pWrite));
        NumBytes = (NumBytesToEnd<NumBytesToCopy?NumBytesToEnd:NumBytesToCopy);

        DtMemCopy(pWrite, pPayLoad, NumBytes);
        NumBytesToCopy -= NumBytes;
        // Move write pointer
        pWrite += NumBytes;
        // Move position of source data pointer
        pPayLoad += NumBytes;

        // Set write pointer to start of buffer
        if (pWrite >= pWrapArea)
            pWrite = pIpRxChannel->m_pFifo;
        
    }
    // Save updated write index in IP Rx channel struct
    pIpRxChannel->m_pBufferHeader->m_FifoWriteOffset = 
                                     DtPtrToUInt((void*)(pWrite - pIpRxChannel->m_pFifo));
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpListsReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Move all RTP packets from m_RtpDvbList, m_RtpFecColumnList and m_RtpFecRowList
// to the m_RtpEmptyList list.
//
void  DtaIpRxRtpListsReset(UserIpRxChannel* pIpRxChannel)
{
    DtListEntry*  pListEntry;

    DtSpinLockAcquire(&pIpRxChannel->m_RtpListSpinLock);

    while (!DtIsListEmpty(&pIpRxChannel->m_RtpDvbList)) 
    {
        pListEntry = DtHeadListRemove(&pIpRxChannel->m_RtpDvbList);
        DtTailListInsert(&pIpRxChannel->m_RtpEmptyList, pListEntry);
    }

    while (!DtIsListEmpty(&pIpRxChannel->m_RtpFecColumnList)) 
    {
        pListEntry = DtHeadListRemove(&pIpRxChannel->m_RtpFecColumnList);
        DtTailListInsert(&pIpRxChannel->m_RtpEmptyList, pListEntry);
    }

    while (!DtIsListEmpty(&pIpRxChannel->m_RtpFecRowList)) 
    {
        pListEntry = DtHeadListRemove(&pIpRxChannel->m_RtpFecRowList);
        DtTailListInsert(&pIpRxChannel->m_RtpEmptyList, pListEntry);
    }

    DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpListsInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxRtpListsInit(UserIpRxChannel* pIpRxChannel)
{
    RtpListEntry*  pRtpList;
    UInt  i;

    DtSpinLockAcquire(&pIpRxChannel->m_RtpListSpinLock);

    DtListHeadInitialize(&pIpRxChannel->m_RtpEmptyList);
    DtListHeadInitialize(&pIpRxChannel->m_RtpDvbList);
    DtListHeadInitialize(&pIpRxChannel->m_RtpFecColumnList);
    DtListHeadInitialize(&pIpRxChannel->m_RtpFecRowList);

    if (pIpRxChannel->m_pRtpListEntries != NULL)    
        DtMemFreePool(pIpRxChannel->m_pRtpListEntries,  IPRX_TAG);

    pIpRxChannel->m_pRtpListEntries = DtMemAllocPool(DtPoolNonPaged, 
                                 DTA_IPRX_MAX_RTP_PACKETS*sizeof(RtpListEntry), IPRX_TAG);

    if (pIpRxChannel->m_pRtpListEntries == NULL)
    {
        DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);
        return DT_STATUS_OUT_OF_RESOURCES;
    }

    pRtpList = (RtpListEntry*)pIpRxChannel->m_pRtpListEntries;

    // Add all entries to the empty list
    for (i=0; i<DTA_IPRX_MAX_RTP_PACKETS; i++) 
    {
        pRtpList->m_BufIndex = (UInt16)i;
        pRtpList->m_InUse = 0;
        DtTailListInsert(&pIpRxChannel->m_RtpEmptyList, &pRtpList->m_ListEntry);
        pRtpList+=1;
    }

    DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpListsCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxRtpListsCleanup( UserIpRxChannel* pIpRxChannel)
{
    if (pIpRxChannel->m_pRtpListEntries)
        DtMemFreePool(pIpRxChannel->m_pRtpListEntries, IPRX_TAG);
    pIpRxChannel->m_pRtpListEntries = NULL;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpGetFreeEntry -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns a new RtpListEntry from the m_RtpEmptyList list
// If no entries are available NULL is returned
//
RtpListEntry* DtaIpRxRtpGetFreeEntry(UserIpRxChannel* pIpRxChannel)
{
    RtpListEntry*  pRtpEntry = NULL;
    DtListEntry*  pListEntry;

    DtSpinLockAcquire(&pIpRxChannel->m_RtpListSpinLock);

    if (!DtIsListEmpty(&pIpRxChannel->m_RtpEmptyList)) 
    {
        pListEntry = DtHeadListRemove(&pIpRxChannel->m_RtpEmptyList);
        pRtpEntry = DtContainingRecord(pListEntry, RtpListEntry, m_ListEntry);      
        pRtpEntry->m_InUse = 0;
    }

    DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);
    return pRtpEntry;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpMoveEntryToEmptyListUnsafe -.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxRtpMoveEntryToEmptyListUnsafe(
    UserIpRxChannel*  pIpRxChannel, 
    RtpListEntry*  pRtpEntry)
{
    DtTailListInsert(&pIpRxChannel->m_RtpEmptyList, &pRtpEntry->m_ListEntry);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpMoveEntryToEmptyList -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Put a RtpListEntry to the m_RtpEmptyList list.
void  DtaIpRxRtpMoveEntryToEmptyList(
    UserIpRxChannel*  pIpRxChannel, 
    RtpListEntry*  pRtpEntry)
{
    DtSpinLockAcquire(&pIpRxChannel->m_RtpListSpinLock);
    DtTailListInsert(&pIpRxChannel->m_RtpEmptyList, &pRtpEntry->m_ListEntry);
    DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpIsSeqNumLess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Sequence number max. 16-bit
// return TRUE: SeqNum1 < SeqNum2
//        FALSE: SeqNum1 >= SeqNum2
//
Bool   DtaIpRxRtpIsSeqNumLess(UInt16 SeqNum1, UInt16 SeqNum2)
{
    if (SeqNum2 > SeqNum1)
        if (SeqNum2-SeqNum1 < 0xFFF) return TRUE; else return FALSE;
    else if (SeqNum1 > SeqNum2) 
    {   
        if (SeqNum1-SeqNum2 < 0x7FFF) return FALSE;    
        if (0xffff-SeqNum1+SeqNum2 < 0xFFF) return TRUE; else return FALSE;
    }
    return FALSE;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpGetDvbEntryUnsafe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns a RtpListEntry for a given SequenceNumber, or NULL if the entry does not exist
// If pLastUsedItem <> NULL, the search starts at pLastUsedItem
// Pre: The m_RtpListSpinLock must be acquired 
// 
RtpListEntry*  DtaIpRxRtpGetDvbEntryUnsafe(
    UserIpRxChannel*  pIpRxChannel,
    UInt16  SequenceNumber,
    DtListEntry*  pLastUsedItem)
{
    Bool  Found;
    DtListEntry*  pEntry;
    RtpListEntry*  pRtpQueuedEntry = NULL;
    
    Found = FALSE;
    if (pLastUsedItem != NULL)    
        pEntry = DtListNextGet(pLastUsedItem);
    else
        pEntry = DtListNextGet(&pIpRxChannel->m_RtpDvbList);

    while (pEntry!=&pIpRxChannel->m_RtpDvbList && !Found) 
    {
        pRtpQueuedEntry = DtContainingRecord(pEntry, RtpListEntry, m_ListEntry);
        if (SequenceNumber == pRtpQueuedEntry->m_RTPSequenceNumber)
            Found = TRUE;
        else
            pEntry = DtListNextGet(pEntry);
    }
    
    if (Found)
        return pRtpQueuedEntry;
    
    return NULL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpListAddEntryUnsafe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaIpRxRtpListAddEntryUnsafe(
    DtListEntry*  pRtpList,
    RtpListEntry*  pRtpEntry)
{
    Bool  Found;
    Bool  Exist;
    DtListEntry*  pEntry;
    RtpListEntry*  pRtpQueuedEntry;

    // Sort the RTP packet
    // Walk through the list and insert into the list.
    // If the packet with the same sequence number already exists return false
    // and skip insertion
    Found = FALSE;
    Exist = FALSE;
    pEntry = DtListNextGet(pRtpList);

    while (pEntry!=pRtpList && !Found) 
    {
        pRtpQueuedEntry = DtContainingRecord(pEntry, RtpListEntry, m_ListEntry);
        if (pRtpEntry->m_RTPSequenceNumber == pRtpQueuedEntry->m_RTPSequenceNumber) 
        {
            Found = TRUE;
            Exist = TRUE;
        } else {
            if (DtaIpRxRtpIsSeqNumLess(pRtpEntry->m_RTPSequenceNumber, 
                                                    pRtpQueuedEntry->m_RTPSequenceNumber))
                Found = TRUE;
            else 
                pEntry = DtListNextGet(pEntry);
        }
    }

    if (!Exist)
    {
        if (!Found)
            // Add element at the end
            DtTailListInsert(pRtpList, &pRtpEntry->m_ListEntry);
        else
            // Insert element before entry
            DtTailListInsert(pEntry, &pRtpEntry->m_ListEntry);
    }

    return !Exist;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpListAddEntry -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaIpRxRtpListAddEntry(UserIpRxChannel* pIpRxChannel, DtListEntry* pRtpList,
                                                                 RtpListEntry*  pRtpEntry)
{
    Bool  Added;

    DtSpinLockAcquire(&pIpRxChannel->m_RtpListSpinLock);
    Added = DtaIpRxRtpListAddEntryUnsafe(pRtpList, pRtpEntry);
    DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);

    return Added;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIpRxCalculateChecksum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Calculates UDP checksum. 
//
UInt16 DtaIpRxCalculateUdpChecksum(
    IpHeaderV4*  pIpHeader,
    UdpHeader*  pUdpHeader,
    UInt8*  pRxData,
    UInt32  PayloadLength)
{
    UInt32  Checksum = 0;
    UInt32  ChecksumCs = 0;
    UInt  Counter;
    
    // For testing purposes only. FPGA can do UDP checksum in final version and
    // skips the packet if checksum is incorrect
    // Calculate checksum for the UDP packet
    // Pseudo header
    Checksum  = DtUInt16ByteSwap(pIpHeader->m_Protocol);
    Checksum += ((UInt16*)pIpHeader->m_SrcAddress)[0] & 0xffff;
    Checksum += ((UInt16*)pIpHeader->m_SrcAddress)[1] & 0xffff;
    Checksum += ((UInt16*)pIpHeader->m_DstAddress)[0] & 0xffff;
    Checksum += ((UInt16*)pIpHeader->m_DstAddress)[1] & 0xffff;
    Checksum += pUdpHeader->m_Length;    

    // Checksum UDP Header
    Checksum += pUdpHeader->m_SourcePort;
    Checksum += pUdpHeader->m_DestinationPort;
    Checksum += pUdpHeader->m_Length;

    // Calculate Payload checksum
    for (Counter=0; Counter <(PayloadLength / 2); Counter++)
        Checksum+= ((UInt16*)pRxData)[Counter];

    ChecksumCs  = (Checksum >> 16) & 0xffff;        // include carries - I
    Checksum  = (Checksum & 0xffff) + ChecksumCs;   // Add High words to low words.
    ChecksumCs  = (Checksum >> 16) & 0xffff;        // include carries - II
    Checksum  = (Checksum & 0xffff) + ChecksumCs;
    Checksum ^= -1;

    Checksum = Checksum & 0xffff;
    if (Checksum == 0)
        Checksum = 0xffff;

    return (UInt16)Checksum;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpParsePayloadMpeg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxParsePayLoadMpeg(
    UserIpRxChannel*  pIpRxChannel,
    RtpHeader*  pRtpHeader,
    DtaDmaRxHeader*  pDmaRxHeader,
    UInt  Size,     
    UInt16  DvbOffset)
{
    RtpListEntry*  pRtpListEntry;
    UInt8*  pRxData = (UInt8*)pDmaRxHeader + sizeof(DtaDmaRxHeader);

    // It's a RTP packet with DVB content. Add to seq. buffer
    pIpRxChannel->m_DetProtocol = DTA_PROTO_RTP;

    DtDbgOut(MAX, IP_RX, "Channel %d: MPEGII Packet rcvd. DVB-Offset=%d, FrameLength=%d",
                               pIpRxChannel->m_ChannelIndex, DvbOffset, 
                               pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength);


    // Increment No Fec packets receive counter
    if (++pIpRxChannel->m_RstCntFecColumn >= MAX_NUM_RTP_DVB_PACKETS)
        pIpRxChannel->m_DetFecNumColumns = 0;
    
    if (++pIpRxChannel->m_RstCntFecRow >= MAX_NUM_RTP_DVB_PACKETS)
        pIpRxChannel->m_DetFecNumRows = 0;

    if (pIpRxChannel->m_RxControl == DT_RXCTRL_IDLE) 
    {
        // We only need to calculate the bitrate
        DtaIpRxProcessDvbPayLoad(
                pIpRxChannel,
                (UInt8*)pDmaRxHeader + DvbOffset + sizeof(DtaDmaRxHeader),
                pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength - DvbOffset - 4,
                pDmaRxHeader->m_Timestamp, 0);
        
        DtaIpRxUserChRefDecr(pIpRxChannel);
        return;
    }
    
    pRtpListEntry = DtaIpRxRtpGetFreeEntry(pIpRxChannel);
    if (pRtpListEntry != NULL)
    {        
        // Fill content
        pRtpListEntry->m_RTPSequenceNumber = 
                                           DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber);
        pRtpListEntry->m_PayloadOffset = DvbOffset;
        pRtpListEntry->m_RTPOffset = (Int)((UInt8*)pRtpHeader - pRxData);

        // Copy complete packet into the buffer
        DtMemCopy(pIpRxChannel->m_pRtpBuffer + 
            (pRtpListEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH), pDmaRxHeader, Size);
        
        // Add packet to the RTP DVB List
        
        // DEBUGGING: Ad this for skipping a package to check reconstruction 
        /*if ( pRtpListEntry->m_RTPSequenceNumber % 51 == 0 && 
                                                  pRtpListEntry->m_RTPSequenceNumber != 0)
        {
            DtDbgOut(ERR, IP_RX, "SKIPPING PACKET SN:%d", 
                                                      pRtpListEntry->m_RTPSequenceNumber);
            DtaIpRxRtpMoveEntryToEmptyList(pIpRxChannel, pRtpListEntry);
        } else */

        if (!DtaIpRxRtpListAddEntry(pIpRxChannel, &pIpRxChannel->m_RtpDvbList, 
                                                                           pRtpListEntry))
        {
            // Packet already in list
            // Set packet back to empty list
            DtaIpRxRtpMoveEntryToEmptyList(pIpRxChannel, pRtpListEntry);
        } else {
            // Tell the reconstruct thread that data is available
            pIpRxChannel->m_RxIncomingPackets = TRUE;
        }
    } else {
        // Out of memory
        DtDbgOut(ERR, IP_RX, 
            "Channel %d: No queue elements available. Buffer FULL. Seq. Num: %d",
            pIpRxChannel->m_ChannelIndex, DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber));
        
        // Extra trigger reconstructor to parse the data
        pIpRxChannel->m_RxIncomingPackets = TRUE;
    }

    DtaIpRxUserChRefDecr(pIpRxChannel);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxParsePayLoadFec -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaIpRxParsePayLoadFec(
    UserIpRxChannel*  pIpRxChannel,
    DtaDmaRxHeader*  pDmaRxHeader,
    Int  Size,
    RtpHeader*  pRtpHeader,
    UInt  PacketSize,
    UInt16  DvbOffset,
    DtaIpPort  *pIpPort,
    Int  DestPort)
{
    
    FecHeader* pFecHeader;
    RtpListEntry*  pRtpListEntry;
    UInt8*  pRxData = (UInt8*)pDmaRxHeader + sizeof(DtaDmaRxHeader);
    pFecHeader = (FecHeader*) (pRxData + DvbOffset); 

    // Check if the correct port number is used for this FEC Packet
    if (((pFecHeader->m_D==0) && 
                             (DestPort!=(pIpRxChannel->m_DstPort+FEC_INC_COLUMN_PORT))) ||
        ((pFecHeader->m_D == 1) &&  
                             (DestPort!=(pIpRxChannel->m_DstPort+FEC_INC_ROW_PORT))))
    {
        // Port number is incorrect for this fec type. 
        DtDbgOut(ERR, IP_RX, 
               "Channel %d: Incorrect destination port number %i for FEC packet type %i.",
               pIpRxChannel->m_ChannelIndex, DestPort, pFecHeader->m_D);
        return FALSE;
    }

    // Check if the correct error correction type is used
    if (pFecHeader->m_Type != 0) 
    {
        // Fec packet is not XOR Fec. 
        DtDbgOut(ERR, IP_RX,"Channel %d: Incorrect FEC error correction type %i",
                                        pIpRxChannel->m_ChannelIndex, pFecHeader->m_Type);

        return FALSE;
    }

    if (pIpRxChannel->m_RxControl == DT_RXCTRL_IDLE) 
    {
        // We only update status in IDLE state
        if (pFecHeader->m_D == 0)     // Column
        {
            pIpRxChannel->m_DetFecNumRows = pFecHeader->m_NA;
            pIpRxChannel->m_RstCntFecRow = 0;
        } else {
            pIpRxChannel->m_DetFecNumColumns = pFecHeader->m_NA;
            pIpRxChannel->m_RstCntFecColumn = 0;
        }

        DtaIpRxUserChRefDecr(pIpRxChannel);
        return TRUE;
    }

    pRtpListEntry = DtaIpRxRtpGetFreeEntry(pIpRxChannel);
    if (pRtpListEntry != NULL) 
    {
        // Fill content
        pRtpListEntry->m_RTPSequenceNumber = 
                                           DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber);
        pRtpListEntry->m_PayloadOffset = DvbOffset + sizeof(FecHeader);
        pRtpListEntry->m_RTPOffset = (Int)((UInt8*)pRtpHeader - pRxData);

        // Store FEC information
        pRtpListEntry->m_FecNA = pFecHeader->m_NA;
        pRtpListEntry->m_FecOffset = pFecHeader->m_Offset;
        pRtpListEntry->m_FecSNBase = DtUInt16ByteSwap(pFecHeader->m_SnBase);
        
        // Copy complete packet into the buffer
        DtMemCopy(pIpRxChannel->m_pRtpBuffer + 
                        (pRtpListEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH), 
                        pDmaRxHeader,
                        Size);

        // Add packet to correct queue
        if (pFecHeader->m_D == 0) 
        {   
            // Column            
            pIpRxChannel->m_DetFecNumRows = pFecHeader->m_NA;
            pIpRxChannel->m_RstCntFecRow = 0;

            // Add packet to the RTP Column List
            if (!DtaIpRxRtpListAddEntry(pIpRxChannel, 
                                        &pIpRxChannel->m_RtpFecColumnList, pRtpListEntry))
            {
                // Packet already in list
                // Set packet back to empty list
                DtaIpRxRtpMoveEntryToEmptyList(pIpRxChannel, pRtpListEntry);
            } else {
                // Tell the reconstruct thread that data is available
                pIpRxChannel->m_RxIncomingPackets = TRUE;
            }
        } else {  

            // Row
            pIpRxChannel->m_DetFecNumColumns = pFecHeader->m_NA;
            pIpRxChannel->m_RstCntFecColumn = 0;

            // Add packet to the RTP Row List
            if (!DtaIpRxRtpListAddEntry(pIpRxChannel,
                                           &pIpRxChannel->m_RtpFecRowList, pRtpListEntry))
            {
                // Packet already in list
                // Set packet back to empty list
                DtaIpRxRtpMoveEntryToEmptyList(pIpRxChannel, pRtpListEntry);
            } else {
                // Tell the FEC reconstruct thread that data is available
                pIpRxChannel->m_RxIncomingPackets = TRUE;
            }        
        }
    } else {
        // Out of memory
        DtDbgOut(ERR, IP_RX, "Channel %d: No queue elements available. Buffer FULL",
                                                            pIpRxChannel->m_ChannelIndex);
        // Tell the FEC reconstruct thread that data is available
        pIpRxChannel->m_RxIncomingPackets = TRUE;        
    }

    DtaIpRxUserChRefDecr(pIpRxChannel);    
    return TRUE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxParsePacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxParsePacket(
    DtaIpPort*  pIpPort,
    UInt8*  pData,
    UInt  Size)
{   
    UInt  i;
    UInt  PacketSize;
    IpHeaderV4*  pIpHeader = NULL;
    UdpHeader*  pUdpHeader = NULL;
    
    Bool  MulticastPacket = FALSE;
    DtaDmaRxHeader*  pDmaRxHeader = (DtaDmaRxHeader*)pData;
    UInt8*  pRxData = (UInt8*)pDmaRxHeader + sizeof(DtaDmaRxHeader);
    EthernetIIHeader*  pEthernetIIHeader = (EthernetIIHeader*)pRxData;
    Ethernet802_3Header*  pEthernet802_3Header = (Ethernet802_3Header*)pRxData;
    UInt8*  pDstMac;
    PhyMac*  pPhyMac = &pIpPort->m_PhyMac;
    
    // fragmented frames
    UserIpRxChannel*  pIpRxFragment = NULL;
    DtaIpUserChannels*  pIpUserChannels = &pIpPort->m_pDvcData->m_IpDevice.m_IpUserChannels;
    UInt  FrameLength;
    UInt  DvbTotalLength;

    // Protocol parsing
    UInt  NumChannels;
    UserIpRxChannel*  pIpRxA[MAX_NUM_IPRX_LISTEN];
    UserIpRxChannel*  pIpRxChannel;
    UInt16  DestPort;
    RtpHeader*  pRtpHeader = NULL;
    RtpExtension*  pRtpHeaderExtension = NULL;
    UInt  ProtocolType;
    Bool  RtpExt = FALSE;
    UInt16  DvbOffset;

    // Calculate packetsize. Extract CRC + DMA header
    PacketSize = Size - 4 - sizeof(DtaDmaRxHeader);

    DtDbgOut(MAX, IP_RX, "Packetsize: %d", PacketSize);

    DT_ASSERT(PacketSize <= DTA_IPRX_MAX_PACKET_SIZE);

    if (pIpPort->m_PhyMac.m_LoopbackEnable) 
    {
        // TODO
        DT_ASSERT(FALSE);
        //if (DtaIpRxHandleLoopback(pIpPort, pData, Size))
        return;
    }

    // Check if the network is administrative up, if not we discard all received packets
    if (pIpPort->m_PhyMac.m_NetworkAdminStatus != DTA_NW_ADMIN_STATUS_UP)
        return;
    
    pEthernetIIHeader = (EthernetIIHeader*)pRxData;
    pEthernet802_3Header = (Ethernet802_3Header*)pRxData;
    
    // Determine the encapsulation method by checking the ethernet type of the packet
    if (pEthernetIIHeader->m_Type == ETH_TYPE_IPV4_BE) 
    {
        // Ethernet II
        pIpHeader = (IpHeaderV4*) (pRxData + sizeof(EthernetIIHeader));
        pDstMac = (UInt8*)pEthernetIIHeader->m_DstMac;
    }
    else if ((pEthernet802_3Header->m_DSAP == 0xAA) && 
             (pEthernet802_3Header->m_SSAP == 0xAA) &&
             (pEthernet802_3Header->m_Type == ETH_TYPE_IPV4_BE)) 
    {
        // 802.2/802.3 encapsulation
        pIpHeader = (IpHeaderV4*) (pRxData + sizeof(Ethernet802_3Header));
        pDstMac = (UInt8*)pEthernet802_3Header->m_DstMac; 
    }
    else {
        // Unknown packet type.....
        DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
        return;
    }
    // Check if the destination mac is a multicast/broadcast address and
    // Rx is enabled.
    if ((pDstMac[0] & 1) != 0) 
    {   
        // It's a multicast or broadcast address
        if (pDstMac[0]==0xff && pDstMac[1]==0xff)
        {
            // Broadcast address
            if ((pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_BROADCAST)==0 &&
                              (pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_PROMISCUOUS)==0)
            {
                // Broadcast Rx disabled
                DtDbgOut(MAX, IP_RX, "Broadcast Rx disabled.");
                return;
            }
        } else if ((pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_PROMISCUOUS)==0 &&
                          (pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_ALL_MULTICAST)==0 &&
                          (pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_MULTICAST)==0)
        {
            // Skip this packet, multicast receive disabled
            DtDbgOut(MAX, IP_RX, "Multicast Rx disabled.MC: "
                                                      "%02x-%02x-%02x-%02x-%02x-%02x",
                                                      pDstMac[0], pDstMac[1], pDstMac[2],
                                                      pDstMac[3], pDstMac[4], pDstMac[5]);
            return;
        } else if ((pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_PROMISCUOUS)==0 &&
                          (pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_ALL_MULTICAST)==0 &&
                          (pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_MULTICAST)==0)
        {
            // Check if the destination MAC address is in the Multicast list
            Bool  Found = FALSE;
            UInt  NumItems = 0;
            Int  i;
            while (!Found && NumItems<pPhyMac->m_NumMulticasts)
            {
                Found = TRUE;
                for (i=0; i<6 && Found; i++) 
                {
                    if (pPhyMac->m_MulticastList[NumItems][i] != pDstMac[i])
                        Found = FALSE;
                }
                NumItems++;
            }
            if (!Found) 
            {
                DtDbgOut(MAX, IP_RX, "Multicast address not in the list. "
                                                      "MC: %02x-%02x-%02x-%02x-%02x-%02x",
                                                      pDstMac[0], pDstMac[1], pDstMac[2],
                                                      pDstMac[3], pDstMac[4], pDstMac[5]);
                return;
            } else {
                DtDbgOut(MAX, IP_RX, "Multicast address Rx. "
                                                      "MC: %02x-%02x-%02x-%02x-%02x-%02x",
                                                      pDstMac[0], pDstMac[1], pDstMac[2],
                                                      pDstMac[3], pDstMac[4], pDstMac[5]);
                MulticastPacket = TRUE;
            }
        }
    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Parse fragmented frame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    
    // Check if we have a complete (fragmented) frame
    if ((pIpHeader->m_Flags&IPV4_HDR_FLAGS_MOREFRAG) != 0 || 
       ((pIpHeader->m_Flags&IPV4_HDR_FLAGS_MOREFRAG) == 0 && 
        (pIpHeader->m_FragmentOffsetH!=0 || pIpHeader->m_FragmentOffsetL!=0)))
    {
        Bool  StopParsing = FALSE;
    
        pIpRxFragment = DtaIpRxParseFragmentedFrame(pIpUserChannels, pDmaRxHeader, Size,
                                       pIpHeader, MulticastPacket, pIpPort, &StopParsing);
        if (StopParsing)
            return;
    }
    
    if(pIpRxFragment != NULL)
    {
        // A fragmented frame is complete and already stored in the RtpBuffer of the
        // first (valid) channel in the pIpRxA array. Redirect buffer pointers.
        // Note: The reference of the fragmented packet is increased.
        //       so from now on it has to be decreased after usage.
        pIpHeader = (IpHeaderV4*)(pIpRxFragment->m_pRtpBuffer + 
                                                      pIpRxFragment->m_CurIpHeaderOffset);
        pDmaRxHeader = (DtaDmaRxHeader*)pIpRxFragment->m_pRtpBuffer;
        PacketSize = pIpRxFragment->m_FragmentOffset - sizeof(DtaDmaRxHeader);
        pRxData = (UInt8*)pDmaRxHeader + sizeof(DtaDmaRxHeader);

        DtDbgOut(MAX, IP_RX, "Fragmented frame, new PacketSize %d.", PacketSize);
    }

    // Check if we have a valid UDP header    
    pUdpHeader = (UdpHeader*)((UInt8*)pIpHeader + (pIpHeader->m_HeaderLength * 4));
    if (pIpHeader->m_Protocol != IPV4_HDR_PROT_UDP)
    {
        if(pIpRxFragment != NULL)
            DtaIpRxUserChRefDecr(pIpRxFragment);
        DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
        return;
    }

    // Check if a un-fragmented frame has the correct length
    FrameLength = DtUInt16ByteSwap(pUdpHeader->m_Length) +
                      DtPtrToUInt((UInt8*)((UInt8*)pUdpHeader - pRxData)) + 4; // +4 = CRC

    if (FrameLength > pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength && 
                                                                   pIpRxFragment == NULL)
    {        
        DtDbgOut(ERR, IP_RX, "UDP Length error. Packet too short. PortIndex=%d", 
                                                                  pIpPort->m_IpPortIndex);
        DT_ASSERT(FALSE);
        
        if (pIpRxFragment != NULL)
            DtaIpRxUserChRefDecr(pIpRxFragment);

        if (PacketSize < DTA_IPRX_MAX_PACKET_LENGTH)
            DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
        return;
    }

    // Calculate Dvb Length and offset
    DvbTotalLength = DtUInt16ByteSwap(pUdpHeader->m_Length) - sizeof(UdpHeader);

    // Store pre. calculation of DVB offset
    DvbOffset = (Int)(((UInt8*)pUdpHeader + sizeof(UdpHeader)) - pRxData);

    if (pIpRxFragment != NULL)
    {
        // Check UDP checksum
        UInt16 Checksum = DtaIpRxCalculateUdpChecksum(pIpHeader, pUdpHeader,
                                                       pRxData+DvbOffset, DvbTotalLength);
        if (pUdpHeader->m_Checksum!=0 && Checksum!=pUdpHeader->m_Checksum)
        {
            DtDbgOut(ERR, IP_RX,"CHECKSUM ERROR, calculated: %x, received: %x", 
                                                        Checksum, pUdpHeader->m_Checksum);
            DtaIpRxUserChRefDecr(pIpRxFragment);
            return;
        }
    }

    // Get the RTP header
    pRtpHeader = (RtpHeader*)((UInt8*)pUdpHeader + sizeof(UdpHeader));

    // Check if the complete (fragmented) packet is for one of our channels
    DestPort = DtUInt16ByteSwap(pUdpHeader->m_DestinationPort);
    NumChannels = DtaIpRxIsPacketForDVB(pIpUserChannels, pRtpHeader,
                                     pIpHeader->m_SrcAddress, pIpHeader->m_DstAddress,
                                     DtUInt16ByteSwap(pUdpHeader->m_SourcePort),
                                     DestPort, MulticastPacket, pIpRxA);
    if (NumChannels == 0)
    {
        if (pIpRxFragment != NULL)
            DtaIpRxUserChRefDecr(pIpRxFragment);
        DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
        return;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Check RTP Header -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    // To determine if the payload of a UDP packet is a RTP packet 
    // or if it is DVB data, we first check the RTP header values.
    // This is not a solid proof, but enough to do the job.
    if ((pRtpHeader->m_PayloadType!=RTP_PAYLOAD_MPEGII && 
                  pRtpHeader->m_PayloadType!=RTP_PAYLOAD_FEC) || pRtpHeader->m_Version!=2)
    {
        // Known payload types not found with RTPVersion of 2.
        // It's a UDP packet.
        ProtocolType = DTA_PROTO_UDP;

        // Check if the Payload is DVB data (0x47 must be the first byte of the payload
        if (*(pRxData + DvbOffset) != 0x47)
        {
            // It's not a DVB packet or a DVB packet with unknown header/content
            DtaIpRxUserChRefDecrAll(pIpRxA, NumChannels);
            if (pIpRxFragment != NULL)
                    DtaIpRxUserChRefDecr(pIpRxFragment);

            DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
            return;
        }
    } else {
        // RTP Packet with MPEG-II or FEC content received
        if (pIpRxFragment != NULL)
        {
            // We can not handle fragmented RTP packets
            // Discard packets            
            DtaIpRxUserChRefDecrAll(pIpRxA, NumChannels);
            if (pIpRxFragment != NULL)
                    DtaIpRxUserChRefDecr(pIpRxFragment);

            DtDbgOut(AVG, IP_RX, "Fragmented RTP packet discarded");
            return;
        }

        // Change packetLength in header if more bytes are received then expected
        // Used in RTP packet reconstruction and DVB extraction
        // So extra bytes not belonging to UDP payload are removed here
        if (pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength != FrameLength)
        {
            DtDbgOut(MAX, IP_RX, "Ethernet packet length (%d) > Calculated length (%d)",
                                pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength,
                                FrameLength);
            pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength = FrameLength;
        }

        RtpExt = (pRtpHeader->m_Extension != 0);

        ProtocolType = DTA_PROTO_RTP;

        // Subtract RTP header size
        DvbTotalLength -= ((pRtpHeader->m_CsrcCount * 4) + sizeof(RtpHeader));
        DvbOffset  += (pRtpHeader->m_CsrcCount * 4) + sizeof(RtpHeader);
    }

    // Check for RTP header extension (RtpExt = FALSE for UDP packets)
    if (RtpExt) 
    {        
        pRtpHeaderExtension =  (RtpExtension*)((UInt8*)pRtpHeader
                             + (pRtpHeader->m_CsrcCount * 4) + sizeof(RtpHeader));
        // Subtract header-extention size  
        DvbTotalLength -= ((DtUInt16ByteSwap(pRtpHeaderExtension->m_Length) * 4) 
                                                                  + sizeof(RtpExtension));
        DvbOffset += (DtUInt16ByteSwap(pRtpHeaderExtension->m_Length) * 4) + 
                                                                     sizeof(RtpExtension);
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- CopyIpPacketsToUserChannels -.-.-.-.-.-.-.-.-.-.-.-.-.-.

    for (i=0; i<NumChannels; i++) 
    {
        // The use count of the IpRx channel is already incremented,
        // so the structure is available
        pIpRxChannel = pIpRxA[i];
        pIpRxChannel->m_FragmentId = 0;
        pIpRxChannel->m_FragmentOffset = 0;      
        
        // Check if we are in Raw IP mode. 
        // If so, the complete packet must be copied 
        // and a header must be added
        if ((pIpRxChannel->m_RxMode & DT_RXMODE_MASK) == DT_RXMODE_IPRAW)
        {
            // Process the raw ip data (including extra bytes after the UDP payload if 
            // available).
            DtaIpRxProcessIpRawPayLoad(pIpRxChannel, pRxData, PacketSize, 
                                                         pDmaRxHeader->m_Timestamp, TRUE);
            // Decrement use count
            DtaIpRxUserChRefDecr(pIpRxChannel);
            continue;
        }

        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Protocol UDP -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        if (ProtocolType == DTA_PROTO_UDP) 
        {
            DtDbgOut(MAX, IP_RX, "Channel %d: UDP DVB Packet received. DVB-DataSize=%i",
                                            pIpRxChannel->m_ChannelIndex, DvbTotalLength);

            // It's a UDP packet with DVB content. Add to user buffer
            DT_ASSERT(*(pRxData + DvbOffset) == 0x47);

            pIpRxChannel->m_DetProtocol = DTA_PROTO_UDP;
            pIpRxChannel->m_DetFecNumColumns = 0;
            pIpRxChannel->m_DetFecNumRows = 0;

            DtaIpRxProcessDvbPayLoad(pIpRxChannel, 
                       pRxData + DvbOffset, DvbTotalLength, pDmaRxHeader->m_Timestamp, 0);
            
            // Decrement use count
            DtaIpRxUserChRefDecr(pIpRxChannel);
            continue;
        }
        
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Protocol RTP -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

        //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Payload MPEGII -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        if (pRtpHeader->m_PayloadType == RTP_PAYLOAD_MPEGII) 
        {
            DtaIpRxParsePayLoadMpeg(pIpRxChannel, pRtpHeader, pDmaRxHeader, Size, 
                                                                               DvbOffset);
            
            // Notify the recontructor
            if (pIpRxChannel->m_RxIncomingPackets)
                pIpUserChannels->m_IpRxPacketAvailable = TRUE;
            continue;
        }

        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Payload FEC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        if (pRtpHeader->m_PayloadType == RTP_PAYLOAD_FEC) 
        {
             // It's a FEC packet
            if (pIpRxChannel->m_FecMode == DTA_FEC_DISABLE) 
            {
                DtDbgOut(MAX, IP_RX, 
                              "Channel %d: FEC packet received, but FEC mode is disabled",
                              pIpRxChannel->m_ChannelIndex);
                
                DtaIpRxUserChRefDecr(pIpRxChannel);
                continue;
            }

            if (!DtaIpRxParsePayLoadFec(pIpRxChannel, pDmaRxHeader, Size, pRtpHeader, 
                                                PacketSize, DvbOffset, pIpPort, DestPort))
            {
                DtaIpRxUserChRefDecrAll(pIpRxA, NumChannels);
                if (pIpRxFragment != NULL)
                    DtaIpRxUserChRefDecr(pIpRxFragment);

                DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
                return;
            }

            // Notify the reconstructor
            if (pIpRxChannel->m_RxIncomingPackets)
                pIpUserChannels->m_IpRxPacketAvailable = TRUE;

            continue;
        }
        
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Unknown Packet Type -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        DT_ASSERT(FALSE);
        // Unknown packet type. Maybe corrupted packet and checksum check is off??
        DtDbgOut(ERR, IP_RX, "Channel %d: UNKNOWN PACKET OR CORRUPTED PACKET??.",
                                                            pIpRxChannel->m_ChannelIndex);

        // Decrement ref count for all channels
        DtaIpRxUserChRefDecrAll(pIpRxA, NumChannels);
        if (pIpRxFragment != NULL)
            DtaIpRxUserChRefDecr(pIpRxFragment);

        DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
        return;
    } // end for

    // make shure the reference to a fragmented packet is decreased
    if (pIpRxFragment != NULL)
        DtaIpRxUserChRefDecr(pIpRxFragment);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CheckAndParseData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Pre: The complete DMA header must be in the pPrevData buffer or pRxData buffer
//
// This functions returns the start of a new packet (pRxData) or
// NULL if no data is available. (*pRxDataSize = 0)
//
UInt8*  CheckAndParseData(
    DtaIpPort*  pIpPort,
    UInt8*  pPrevData,
    UInt*  pPrevDataSize,
    UInt8*  pRxData, 
    UInt*  pRxDataSize)
{
    UInt  NumBytesToChk;
    UInt8*  pDataToChk;
    DtaDmaRxHeader*  pDmaRxHeader;
    UInt  BytesToCopy;
    
    if (*pPrevDataSize != 0)
    {
        pDataToChk = pPrevData;
        NumBytesToChk = *pPrevDataSize + *pRxDataSize;
    } else {
        pDataToChk = pRxData;
        NumBytesToChk = *pRxDataSize;
    }
    DT_ASSERT((*pPrevDataSize==0 && *pRxDataSize >= sizeof(DtaDmaRxHeader)) ||
                                              (*pPrevDataSize >= sizeof(DtaDmaRxHeader)));
            
    pDmaRxHeader = (DtaDmaRxHeader*)pDataToChk;

    if (NumBytesToChk < pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength +
                                                                   sizeof(DtaDmaRxHeader))
    {
        // Not a complete packet available. Copy part of packet into 
        // temp. buffer for next DMA
        DtMemCopy(pPrevData+*pPrevDataSize, pRxData, *pRxDataSize);
        if (*pPrevDataSize == 0)
            DtDbgOut(MAX, IP_RX, "Partly packet stored. Size: %d", *pRxDataSize);
        else
            DtDbgOut(ERR, IP_RX, "Partly packet stored for second time."
                                                       "PrevDataSize: %d. Rx'ed size: %d",
                                                       *pPrevDataSize, *pRxDataSize);

        *pPrevDataSize = *pPrevDataSize + *pRxDataSize;
        *pRxDataSize = 0;
        return NULL;
    }

    if (pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength == 0) 
    {
        // End of chain
        DtDbgOut(MAX, IP_RX, "End of chain");
        *pRxDataSize = 0;
        return NULL;
    }
    // Complete packet received
    BytesToCopy = pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength +
                                                                   sizeof(DtaDmaRxHeader);
    if (*pPrevDataSize != 0)
    {
        // Copy part of DMA packet to Previous data
        BytesToCopy-= *pPrevDataSize;
        DtMemCopy(pDataToChk+*pPrevDataSize, pRxData, BytesToCopy);
    }
    
    DtaIpRxParsePacket(pIpPort, pDataToChk, BytesToCopy+*pPrevDataSize);
    
    // Round to DWORDs
    if ((BytesToCopy % 4) != 0)
        BytesToCopy += (4 - (BytesToCopy % 4));
    pRxData+= BytesToCopy;
    if (*pRxDataSize <= BytesToCopy)
        *pRxDataSize = 0;
    else
        *pRxDataSize-= BytesToCopy;
    *pPrevDataSize = 0;
    return pRxData;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxWorkerThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// IP Rx worker thread.
//
void  DtaIpRxWorkerThread(DtThread* pThread, void* pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIpPort*  pIpPort = pContext;
    UInt8*  pPrevData = NULL;
    UInt8*  pRxData;
    DtaDmaRxHeader*  pRxHeader;
    Bool  StopThread = FALSE;
    DtEvent*  pDmaDoneEvent;
    UInt  NumBytesRx;
    UInt  PrevDataLength = 0;
    UInt  BytesToCopy;
    DmaChannel*  pDmaCh = NULL;
    DtaIpUserChannels*  pIpUserChannels = NULL;
    
    DtDbgOut(MAX, IP_RX, "Start");

    pPrevData = DtMemAllocPool(DtPoolNonPaged, 2048, IPRX_TAG);
    if (pPrevData == NULL) 
    {
        DtDbgOut(ERR, IP_RX, "Not enough memory to allocate data buffer");
        return;
    }

    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        pDmaCh = &pIpPort->m_IpPortType1.m_Rx.m_DmaChannel;
    else
        DT_ASSERT(FALSE);

    pDmaDoneEvent = DtaDmaGetDoneEvent(pDmaCh);

    // Start DMA transfer
    Status = DtaPPBufferTransferData(&pIpPort->m_RxPPBuffer);
    DT_ASSERT(DT_SUCCESS(Status));

    while (!StopThread)
    {
        // Wait for DMA done or stop
        Status = DtThreadWaitForStopOrEvent(pThread, pDmaDoneEvent, DtaDmaIsReady(pDmaCh),
                                                                                   FALSE);
        if (Status==DT_STATUS_CANCELLED || DtaDmaIsAbortActive(pDmaCh))
        {
            StopThread = TRUE;
            continue;
        }

        // DMA ready, start new transfer of other buffer
        Status = DtaPPBufferTransferData(&pIpPort->m_RxPPBuffer);
        DT_ASSERT(DT_SUCCESS(Status));

        // Get number of bytes Rx'ed of previous buffer
        NumBytesRx = DtaPPBufferGetTransferSize(&pIpPort->m_RxPPBuffer);
        
        // Check received data.
        pRxData = DtaPPBufferReadDataGetBuffer(&pIpPort->m_RxPPBuffer);

        //-.-.-.-.-.-.-.-.-.-.- Check if previous data is available -.-.-.-.-.-.-.-.-.-.-.
        if (PrevDataLength != 0)
        {
            // First copy part of header if the header was not already complete
            if (PrevDataLength < sizeof(DtaDmaRxHeader)) 
            {
                BytesToCopy = sizeof(DtaDmaRxHeader) - PrevDataLength;
                if (BytesToCopy > NumBytesRx)
                {
                    BytesToCopy = NumBytesRx;
                    DtDbgOut(ERR, IP_RX, "DMA Header STILL not complete. PrevDataSize:"
                                        " %d.Rx'ed size: %d", PrevDataLength, NumBytesRx);
                }
                DtMemCopy(pPrevData+PrevDataLength, pRxData, BytesToCopy);
                pRxData+= BytesToCopy;
                NumBytesRx-= BytesToCopy;
                PrevDataLength+= BytesToCopy;
            }
            if (PrevDataLength >= sizeof(DtaDmaRxHeader))
            {
                // At least DMA header is available
                pRxHeader = (DtaDmaRxHeader*)pPrevData;

                // Check the header content. If the header is not correct, skip packet 
                // and restart with the new received data.
                if (DtaIpRxIsDmaHeaderCorrect(pRxHeader))
                {
                    DtDbgOut(MAX, IP_RX, "DMA header complete");
                    if (NumBytesRx+PrevDataLength < 
                                   pRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength+
                                   sizeof(DtaDmaRxHeader))
                    {
                        // Not a complete packet available, copy part of header into temp.
                        // buffer for later.
                        DtMemCopy(pPrevData+PrevDataLength, pRxData, NumBytesRx);
                        DtDbgOut(ERR, IP_RX, "Partly packet stored for second time."
                                                       "PrevDataSize: %d. Rx'ed size: %d",
                                                       PrevDataLength, NumBytesRx);
                        PrevDataLength+= NumBytesRx;
                        NumBytesRx = 0;
                    } else {
                        DtDbgOut(MAX, IP_RX, "Packet complete");
                        pRxData = CheckAndParseData(pIpPort, pPrevData, &PrevDataLength, 
                                                                    pRxData, &NumBytesRx);
                    }
                } else {
                    DtDbgOut(ERR, IP_RX, "Header complete but INCORRECT. Skip all"
                                     "  previous data. PrevDataSize: %d", PrevDataLength);
                    // Skip all previous data, and take only the new data
                    PrevDataLength = 0;
                    NumBytesRx = DtaPPBufferGetTransferSize(&pIpPort->m_RxPPBuffer);
                    pRxData = DtaPPBufferReadDataGetBuffer(&pIpPort->m_RxPPBuffer);
                }
            }
        }
        // Parse the DMA data
        while (NumBytesRx != 0)
        {
            if (NumBytesRx < sizeof(DtaDmaRxHeader))
            {
                // Not a complete packet available. Copy part of DMA header into 
                // temp. buffer for next DMA
                DtMemCopy(pPrevData, pRxData, NumBytesRx);
                PrevDataLength = NumBytesRx;
                DtDbgOut(MAX, IP_RX, "Partly DMA header stored. Size: %d", 
                                                                          PrevDataLength);
                break;
            }
            pRxHeader = (DtaDmaRxHeader*)pRxData;
            if (!DtaIpRxIsDmaHeaderCorrect(pRxHeader))
            {
                // Error in DMA header
                DtDbgOut(ERR, IP_RX, "INCORRECT DMA header.");

                // Find packet header
                while (NumBytesRx>=sizeof(DtaDmaRxHeader) && 
                                    !DtaIpRxIsDmaHeaderCorrect(pRxHeader))
                {
                    pRxData++;
                    pRxHeader = (DtaDmaRxHeader*)pRxData;
                    NumBytesRx--;
                }

                // Check if we found a correct packet header
                if (NumBytesRx < sizeof(DtaDmaRxHeader))
                {
                    DtDbgOut(ERR, IP_RX, "No correct DMA header found after incorrect"
                                                                           " DMA header");
                    break;
                }
            }
            pRxData = CheckAndParseData(pIpPort, pPrevData, &PrevDataLength, pRxData, 
                                                                             &NumBytesRx);
        }
        // Finished parsing data
        // If packets are found process them in the reconstructor
        pIpUserChannels = &pIpPort->m_pDvcData->m_IpDevice.m_IpUserChannels;
        if (pIpUserChannels->m_IpRxPacketAvailable)
        {
            pIpUserChannels->m_IpRxPacketAvailable = FALSE;
            pIpUserChannels->m_IpRxPacketAvailableTrigger = TRUE;
            DtEventSet(&pIpUserChannels->m_IpRxPacketAvailEvent);
        }

        // Release buffer
        DtaPPBufferReadDataFinished(&pIpPort->m_RxPPBuffer);
    }

    DtDbgOut(MAX, IP_RX, "Abort DMA");

    DtaDmaAbortDma(pDmaCh);

    while (!DtaDmaIsReady(pDmaCh))
        DtSleep(10);

    DtMemFreePool(pPrevData, IPRX_TAG);

    // We have to wait until the thread received a stop command.
    DtThreadWaitForStop(pThread);

    DtDbgOut(MAX, IP_RX, "Exit");
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIpRxGetMaxOutOfSync -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt16  DtaNwIpRxGetMaxOutOfSync(UserIpRxChannel* pIpRxChannel) 
{
    if (pIpRxChannel->m_DetFecNumColumns!=0 && pIpRxChannel->m_DetFecNumRows!=0) 
    {
        return MAX_OUTOFSYNC_SEQ_NUM + 
                         pIpRxChannel->m_DetFecNumColumns * pIpRxChannel->m_DetFecNumRows;
    }
    
    return MAX_OUTOFSYNC_SEQ_NUM + MAX_NUM_RTP_DVB_PACKETS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetSequenceNumberGap -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt16  DtaIpRxGetSequenceNumberGap(UInt16 SequenceNum1, UInt16 SequenceNum2)
{
    if (SequenceNum1 > SequenceNum2)    
        return 0xffff - SequenceNum1 + SequenceNum2 + 1;

    return SequenceNum2 - SequenceNum1;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetMaxOutOfSync -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt16  DtaIpRxGetMaxOutOfSync(UserIpRxChannel* pIpRxChannel) 
{
    if (pIpRxChannel->m_DetFecNumColumns!=0 && pIpRxChannel->m_DetFecNumRows!=0)
        return MAX_OUTOFSYNC_SEQ_NUM + 
                         pIpRxChannel->m_DetFecNumColumns * pIpRxChannel->m_DetFecNumRows;

    return MAX_OUTOFSYNC_SEQ_NUM + MAX_NUM_RTP_DVB_PACKETS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxProcessRtpDvbPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxProcessRtpDvbPacket(UserIpRxChannel* pIpRxChannel, UInt16* pLastSeqNum)
{
    DtaDmaRxHeader*  pDmaRxHeader;
    UInt16  MaxOutOfSync;
    UInt16  MinPacketsInBuffer;
    RtpListEntry*  pRtpListEntry = NULL;
    RtpListEntry*  pRtpLastListEntry = NULL;
    UInt16  Gap;
    UInt16  NumPackets = 1;
    UInt16  LastSeqNum;

    if (pIpRxChannel->m_FecMode == DTA_FEC_DISABLE) 
    {
        MaxOutOfSync = MAX_OUTOFSYNC_SEQ_NUM;
        MinPacketsInBuffer = 0;
    } else {
        MaxOutOfSync = 3 * DtaIpRxGetMaxOutOfSync(pIpRxChannel);
        // Is a Fec row + column packet received
        MinPacketsInBuffer = MaxOutOfSync;
    }   

    while(TRUE)
    {
        DtSpinLockAcquire(&pIpRxChannel->m_RtpListSpinLock);
        if (pRtpListEntry != NULL)
        {   
            // We don't need this packet anymore, add to empty List
            // Check if the entry is still available. It's possible that the
            // entry is already moved to the empty list due to a reset of the IpRx channel
            if(pRtpListEntry == 
                             DtContainingRecord(DtListNextGet(&pIpRxChannel->m_RtpDvbList),
                             RtpListEntry, m_ListEntry));
            {
                DtHeadListRemove(&pIpRxChannel->m_RtpDvbList);
                DtaIpRxRtpMoveEntryToEmptyListUnsafe(pIpRxChannel, pRtpListEntry);
            }
        }

        if(!DtIsListEmpty(&pIpRxChannel->m_RtpDvbList))
        {
            // Let the entry stay in the list, but get the record
            pRtpListEntry = DtContainingRecord(DtListNextGet(&pIpRxChannel->m_RtpDvbList),
                                                               RtpListEntry, m_ListEntry);
        } else {
            DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);
            break;
        }

        // Retrieve the last received sequence number
        pRtpLastListEntry = DtContainingRecord(DtListPrevGet(&pIpRxChannel->m_RtpDvbList), 
                                                               RtpListEntry, m_ListEntry);
        DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);  

        if (pLastSeqNum != NULL)
            LastSeqNum = *pLastSeqNum;
        else 
            LastSeqNum = pRtpLastListEntry->m_RTPSequenceNumber;

        Gap = DtaIpRxGetSequenceNumberGap(pRtpListEntry->m_RTPSequenceNumber, LastSeqNum);
        
        // Minimal MinPacketsInBuffer packets in buffer for reconstruction
        if ((Gap>MaxOutOfSync) || 
            (DtaIpRxGetSequenceNumberGap(pIpRxChannel->m_RtpLastSeqNum, 
                                              pRtpListEntry->m_RTPSequenceNumber) == 1 &&
                                              !pIpRxChannel->m_RtpFirstPacket && 
                                              pRtpListEntry->m_InUse==0 && 
                                              Gap>=MinPacketsInBuffer))
        {            
            // Packet ready for user buffer

            // Count number of lost packets            
            if (!pIpRxChannel->m_RtpFirstPacket) 
            {
                DtSpinLockAcquire(&pIpRxChannel->m_StatisticsSpinLock);
                NumPackets = DtaIpRxGetSequenceNumberGap(pIpRxChannel->m_RtpLastSeqNum, 
                                                      pRtpListEntry->m_RTPSequenceNumber);

                pIpRxChannel->m_NumPacketsNotReconstructed += NumPackets-1;
                pIpRxChannel->m_TotNumPackets += NumPackets;
                DtSpinLockRelease(&pIpRxChannel->m_StatisticsSpinLock);
            }
            pIpRxChannel->m_RtpLastSeqNum = pRtpListEntry->m_RTPSequenceNumber;
            pIpRxChannel->m_RtpFirstPacket = FALSE;

            pDmaRxHeader = (DtaDmaRxHeader*)(pIpRxChannel->m_pRtpBuffer + 
                                (pRtpListEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));

            DtDbgOut(MAX, IP_RX,
                "Channel %d: RtpDvbPacket.BufIndex:%i,SeqNum:%i,Gap:%i,LSNTx:%i,LSNRx:%i",
                pIpRxChannel->m_ChannelIndex, pRtpListEntry->m_BufIndex, 
                pRtpListEntry->m_RTPSequenceNumber, Gap, 
                pIpRxChannel->m_RtpLastSeqNum, LastSeqNum);

            // Store packet in DVB User buffer
            DtaIpRxProcessDvbPayLoad(pIpRxChannel,
                               (UInt8*)pDmaRxHeader + pRtpListEntry->m_PayloadOffset +
                               sizeof(DtaDmaRxHeader),
                               pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength -
                               pRtpListEntry->m_PayloadOffset - 4,  // -4 for CRC
                               pDmaRxHeader->m_Timestamp, NumPackets-1);
        } else
            break;
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxReconstructThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxReconstructThread(DtThread* pThread, void* pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIpUserChannels*  pIpUserChannels = pContext;
    Bool  StopThread = FALSE;
    UserIpRxChannel*  pIpRxChannel = NULL;
    Int  ChannelIndex;
    
    DtDbgOut(MAX, IP_RX, "Start");

    while (!StopThread)
    {
        // Wait for DMA done or stop
        Status = DtThreadWaitForStopOrEvent(pThread, 
                           &pIpUserChannels->m_IpRxPacketAvailEvent, 
                           (pIpUserChannels->m_IpRxPacketAvailableTrigger == TRUE), TRUE);
        pIpUserChannels->m_IpRxPacketAvailableTrigger = FALSE;
        if (Status == DT_STATUS_CANCELLED)
        {
            StopThread = TRUE;
            continue;
        }        
        
        DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);

        pIpRxChannel = pIpUserChannels->m_pIpRxChannel;
        while(pIpRxChannel != NULL)
        {
            if (pIpRxChannel->m_RxControl != DT_RXCTRL_RCV)
            {
                pIpRxChannel = pIpRxChannel->m_pNext;
                continue;
            }
            if (!pIpRxChannel->m_RxIncomingPackets)
            {
                pIpRxChannel = pIpRxChannel->m_pNext;
                continue;
            }
            pIpRxChannel->m_RxIncomingPackets = FALSE;

            DtaIpRxUserChRefAdd(pIpRxChannel);
            ChannelIndex = pIpRxChannel->m_ChannelIndex;
            DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);

            if (pIpRxChannel->m_FecMode == DTA_FEC_DISABLE)
                DtaIpRxProcessRtpDvbPacket(pIpRxChannel, NULL);
            else
                DtaIpRxProcessDvbWithFec(pIpRxChannel);

            DtaIpRxUserChRefDecr(pIpRxChannel);

            // If a channel is deleted, restart.
            DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);
            pIpRxChannel = DtaIpRxUserChGetUnsafe(pIpUserChannels, ChannelIndex);
            if (pIpRxChannel == NULL)
                pIpRxChannel = pIpUserChannels->m_pIpRxChannel;
            else
                pIpRxChannel = pIpRxChannel->m_pNext;

        }
        DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
    }

    // We have to wait until the thread received a stop command.
    DtThreadWaitForStop(pThread);
    DtDbgOut(MAX, IP_RX, "Exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxSetBrmSkipUpdate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxSetBrmSkipUpdate(DtaIpUserChannels* pIpRxUserChannels, Bool State)
{
    DtSpinLockAcquire(&pIpRxUserChannels->m_IpRxBrmSpinLock);
    pIpRxUserChannels->m_IpRxBrmSkipUpdate = State;
    DtSpinLockRelease(&pIpRxUserChannels->m_IpRxBrmSpinLock);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIpRxBrmProcessSample -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// PRE-CONDITION:
//  - This is not the first sample: pIpRxChannel->m_BrmNumSamples!=0
//  - Time passed since previous sample is at least the minimum period (200ms)
void  DtaIpRxBrmProcessSample(
    UserIpRxChannel*  pIpRxChannel,
    UInt64  RefClock, 
    UInt64  TimeStamp)
{
    Int  IdxEldest;
    Int  IdxNew;
    UInt64  DiffTs;
    UInt64  DiffNumPckBytes; 
    UInt64  DiffNumPckTime;
    
    // Compute index of location where to store new sample, with wrap-around
    IdxNew = pIpRxChannel->m_BrmIdxLastSample + 1;
    if (IdxNew >= NUM_IPRX_MAX_SAMPLES)
        IdxNew = 0;
    
    pIpRxChannel->m_BrmIdxLastSample = IdxNew;

    // Store new sample
    pIpRxChannel->m_BrmSampleNumPckBytes[IdxNew] = pIpRxChannel->m_BrmNumPckBytesRcvd;
    pIpRxChannel->m_BrmSampleTimeStamp[IdxNew] = TimeStamp;

    // Increment #valid samples, unless maximum has been reached
    if (pIpRxChannel->m_BrmNumSamples < NUM_IPRX_MAX_SAMPLES)
        pIpRxChannel->m_BrmNumSamples++;

    // Compute index of eldest sample
    IdxEldest = IdxNew + 1 - pIpRxChannel->m_BrmNumSamples;

    if (IdxEldest < 0)
        IdxEldest += NUM_IPRX_MAX_SAMPLES;

    // Compute bit rate
    // Timestamp can never wrap (55 bits = 21 year)
    DiffTs = TimeStamp - pIpRxChannel->m_BrmSampleTimeStamp[IdxEldest];

    if (pIpRxChannel->m_BrmNumPckBytesRcvd <
                                          pIpRxChannel->m_BrmSampleNumPckBytes[IdxEldest])
    {
        DiffNumPckBytes = 0xffffffff + pIpRxChannel->m_BrmNumPckBytesRcvd - 
                                          pIpRxChannel->m_BrmSampleNumPckBytes[IdxEldest];
    } else {
        DiffNumPckBytes = pIpRxChannel->m_BrmNumPckBytesRcvd - 
                                          pIpRxChannel->m_BrmSampleNumPckBytes[IdxEldest];
    }

    // Skip bitrate calculation if DiffTs == 0 to prevent division by zero exception
    DiffNumPckTime = 8*DiffNumPckBytes*RefClock;
    if (DiffTs != 0)
    {   
        DiffNumPckTime = DtDivide64(DiffNumPckTime, DiffTs, NULL);
        pIpRxChannel->m_BrmEstimate = (Int)DiffNumPckTime;        
    }

    DtDbgOut(MAX, IP_RX,"Channel %d:DiffTs=%llu DiffNumPckBytes=%llu BrmEstimate=%d",
      pIpRxChannel->m_ChannelIndex, DiffTs, DiffNumPckBytes, pIpRxChannel->m_BrmEstimate);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUpdateBrmDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxUpdateBrmDpc(DtDpcArgs* pArgs)
{
    DtaIpDevice*  pIpDevice;
    DtaIpUserChannels*  pIpUserChannels;
    DtaDeviceData*  pDvcData;
    UserIpRxChannel*  pIpRxChannel = NULL;

    UInt64  BrmLastRxTimeStamp;
    UInt32  BrmNumPckBytesRcvd;
    UInt64  CurrentTime;
    UInt64  MinPeriod;

    pIpDevice = (DtaIpDevice*)pArgs->m_pContext;
    pIpUserChannels = &pIpDevice->m_IpUserChannels;
    pDvcData = pIpUserChannels->m_pDvcData;
        
    // Minimum time between samples in clock cycles = 200 mS
    MinPeriod = (pDvcData->m_DevInfo.m_RefClk / 1000) * 200;
    CurrentTime = DtaRegRefClkCntGet64(pDvcData->m_pGenRegs);

    DtSpinLockAcquireAtDpc(&pIpUserChannels->m_IpRxBrmSpinLock);
    if (pIpUserChannels->m_IpRxBrmSkipUpdate)
    {
        DtSpinLockReleaseFromDpc(&pIpUserChannels->m_IpRxBrmSpinLock);
        return;
    }

    pIpRxChannel = pIpUserChannels->m_pIpRxChannel;

    while(pIpRxChannel != NULL)
    {
        BrmLastRxTimeStamp = pIpRxChannel->m_BrmLastRxTimeStamp;
        BrmNumPckBytesRcvd = pIpRxChannel->m_BrmNumPckBytesRcvd;

        if (pIpRxChannel->m_BrmNumSamples == 0)
        {
            // First received sample
            pIpRxChannel->m_BrmEstimate = 0;
            if (BrmNumPckBytesRcvd != 0)
            {
                DtDbgOut(MAX, IP_RX, "Channel %d: First BRM sample", 
                                                            pIpRxChannel->m_ChannelIndex);
                pIpRxChannel->m_BrmSampleNumPckBytes[0] = BrmNumPckBytesRcvd;
                pIpRxChannel->m_BrmSampleTimeStamp[0] = BrmLastRxTimeStamp;
                pIpRxChannel->m_BrmIdxLastSample = 0;
                pIpRxChannel->m_BrmNumSamples = 1;
            }
        } else {
            // Not the first received sample
            UInt64  Period = BrmLastRxTimeStamp - 
                     pIpRxChannel->m_BrmSampleTimeStamp[pIpRxChannel->m_BrmIdxLastSample];
            DtDbgOut(MAX, IP_RX, "Channel %d: BRM sample. Period=%llu", 
                                                    pIpRxChannel->m_ChannelIndex, Period);

            if (Period > MinPeriod)
            {
                DtaIpRxBrmProcessSample(pIpRxChannel, pDvcData->m_DevInfo.m_RefClk, 
                                                                      BrmLastRxTimeStamp);
            }
            else {
                // Did not get any samples for the last 2 seconds:
                // reset measurement values.
                UInt64  DiffTime = CurrentTime - BrmLastRxTimeStamp;
                if (DiffTime >= (10 * MinPeriod))
                {
                    ClearBrmValues(pIpRxChannel);
                    pIpRxChannel->m_PckSizeSrc = DT_PCKSIZE_INV;
                }
            }
        }
        pIpRxChannel = pIpRxChannel->m_pNext;
    }

    DtSpinLockReleaseFromDpc(&pIpUserChannels->m_IpRxBrmSpinLock);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ FEC Reconstruction +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetFecReconstructionList -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns the FEC Packet belonging to the SequenceNumber if all packets to reconstruct
// are available. The pReconstructList is filled with all DVB packets needed to reconstruct
// Pre: The m_RtpListSpinLock must be acquired 
//
RtpListEntry*  DtaIpRxGetFecReconstructionList(
    UserIpRxChannel*  pIpRxChannel,
    DtListEntry*  pFecList,
    UInt16  SequenceNumber,
    RtpListEntry*  pReconstructList[MAX_FEC_RECONSTR_ELEMENTS])
{
    DtListEntry*  pListEntry;
    DtListEntry*  pDvbListEntry = NULL;
    RtpListEntry*  pRtpEntry = NULL;
    RtpListEntry*  pRtpDvbEntry;
    Bool  Found = FALSE;
    UInt32  EndSeqNum;
    UInt32  SeqNum;
    Int  Counter;
    Int  Index = 0;
    UInt16  SeqNrOffset;

    pListEntry = DtListNextGet(pFecList);

    while (!Found && pListEntry!=pFecList) 
    {
        pRtpEntry = DtContainingRecord(pListEntry, RtpListEntry, m_ListEntry);
        
        //Check if required sequence number is in the current FEC range.
        EndSeqNum = (pRtpEntry->m_FecNA*pRtpEntry->m_FecOffset) + pRtpEntry->m_FecSNBase;
        
        if ( (SequenceNumber>=pRtpEntry->m_FecSNBase && SequenceNumber<EndSeqNum) ||
             (EndSeqNum>0xffff && SequenceNumber<(EndSeqNum%0x10000)))
        {
            // Required sequence number is in the correct range, 
            // Now check if the offset is at the correct position.
            // This check is only needed for column FEC packets,
            // for ROW packets: Offset always 1!!
            SeqNrOffset = (Int)SequenceNumber - (Int)pRtpEntry->m_FecSNBase;
            if (SeqNrOffset%pRtpEntry->m_FecOffset == 0) 
            {            
                // This FEC belongs to the needed sequence number
                Found = TRUE;

                // Set search starting point to NULL, start from begin
                pDvbListEntry = NULL;
                
                for (Counter=0; (Counter<pRtpEntry->m_FecNA) && Found; Counter++) 
                {
                    SeqNum = (UInt32)pRtpEntry->m_FecSNBase + 
                             ((UInt32)pRtpEntry->m_FecOffset * Counter);

                    SeqNum = SeqNum % 0x10000;
                    if (SequenceNumber == SeqNum) 
                    {
                        //Skip this one, because we need to reconstruct it
                    } else {
                        pRtpDvbEntry = DtaIpRxRtpGetDvbEntryUnsafe(pIpRxChannel, 
                                                           (UInt16)SeqNum, pDvbListEntry);
                        
                        if (pRtpDvbEntry) 
                        {                        
                            pReconstructList[Index++] = pRtpDvbEntry;

                            // Next time start searching from the following point
                            pDvbListEntry = &pRtpDvbEntry->m_ListEntry;
                        } else
                            Found = FALSE;
                    }
                }
            }
        }
        pListEntry = DtListNextGet(pListEntry);
    }

    if (Found) 
        return pRtpEntry;

    return NULL;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxCalcNewRxTimestamp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt64  DtaIpRxCalcNewRxTimestamp(
    UserIpRxChannel*  pIpRxChannel,
    RtpListEntry*  pRtpDestEntry, 
    RtpListEntry*  pRtpFecEntry, 
    RtpListEntry*  pReconstructList[MAX_FEC_RECONSTR_ELEMENTS])
{
    DtaDmaRxHeader*  pDmaRxHeaderSrc1;
    DtaDmaRxHeader*  pDmaRxHeaderSrc2;
    RtpHeader*  pRtpHeaderSrc1;
    RtpHeader*  pRtpHeaderSrc2;
    UInt  RtpSeqSrc1;
    UInt  RtpSeqSrc2;
    
    RtpListEntry*  pRtpCurEntry;
    UInt64  dT;
    UInt  SeqDiff;
    if (pRtpFecEntry->m_FecNA <= 2) // Is out of spec. No dT can be calculated
        return 0;
    
    pDmaRxHeaderSrc1 = (DtaDmaRxHeader*)(pIpRxChannel->m_pRtpBuffer +
                          (pReconstructList[0]->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));
        
    pDmaRxHeaderSrc2 = (DtaDmaRxHeader*)(pIpRxChannel->m_pRtpBuffer +
                          (pReconstructList[1]->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));

    pRtpHeaderSrc1 = (RtpHeader*)((UInt8*)pDmaRxHeaderSrc1 + sizeof(DtaDmaRxHeader) + 
                                                        pReconstructList[0]->m_RTPOffset);
    pRtpHeaderSrc2 = (RtpHeader*)((UInt8*)pDmaRxHeaderSrc2 + sizeof(DtaDmaRxHeader) + 
                                                        pReconstructList[1]->m_RTPOffset);
    RtpSeqSrc1 = DtUInt16ByteSwap(pRtpHeaderSrc1->m_SequenceNumber);
    RtpSeqSrc2 = DtUInt16ByteSwap(pRtpHeaderSrc2->m_SequenceNumber);

    // Calculate packet distance (dT)
    if (pDmaRxHeaderSrc2->m_Timestamp >= pDmaRxHeaderSrc1->m_Timestamp)
        dT = pDmaRxHeaderSrc2->m_Timestamp - pDmaRxHeaderSrc1->m_Timestamp;
    else
        dT = 0xffffffff - (pDmaRxHeaderSrc2->m_Timestamp&0xffffffff) + 
                                               pDmaRxHeaderSrc1->m_Timestamp & 0xffffffff;

    DtDbgOut(MAX, IP_RX, "Channel %d: dT:%I64u TimeSrc1:%I64u TimeSrc2:%I64u "
             "Seq1: %u Seq2:%u DstSeq:%u FecOffset:%u", pIpRxChannel->m_ChannelIndex, dT, 
             (UInt64)pDmaRxHeaderSrc1->m_Timestamp, (UInt64)pDmaRxHeaderSrc2->m_Timestamp,
             RtpSeqSrc1, RtpSeqSrc2, pRtpDestEntry->m_RTPSequenceNumber, 
             pRtpFecEntry->m_FecOffset);
    if (RtpSeqSrc2 < RtpSeqSrc1) 
        SeqDiff = 0xffff-RtpSeqSrc1 + RtpSeqSrc2;
    else
        SeqDiff = RtpSeqSrc2 - RtpSeqSrc1;
    if (SeqDiff == (2 * pRtpFecEntry->m_FecOffset))
    {
        // Reconstructed packet is in between the other packets
        dT = DtDivide64(dT, 2 * pRtpFecEntry->m_FecOffset, NULL);

        DtDbgOut(MAX, IP_RX, "Channel %d: 1-dT:%I64u TimeSrc1:%I64u TimeSrc2:%I64u "
                 "Seq1: %u Seq2:%u FecOffset:%u NewTime:%I64u SeqDiff:%i", 
                 pIpRxChannel->m_ChannelIndex,  dT, (UInt64)pDmaRxHeaderSrc1->m_Timestamp,
                 (UInt64)pDmaRxHeaderSrc2->m_Timestamp, RtpSeqSrc1, RtpSeqSrc2, 
                 pRtpFecEntry->m_FecOffset, (UInt64)pDmaRxHeaderSrc1->m_Timestamp + dT, 
                 SeqDiff);
    
        return pDmaRxHeaderSrc1->m_Timestamp + (dT * pRtpFecEntry->m_FecOffset);
    } else {
        Int  Index = 0;
        DT_ASSERT(SeqDiff == pRtpFecEntry->m_FecOffset);
        
        dT = DtDivide64(dT, pRtpFecEntry->m_FecOffset, NULL);
        
        // Search for location where reconstructed packet must be inserted
        while (Index < pRtpFecEntry->m_FecNA-1)
        {
            pDmaRxHeaderSrc1 = (DtaDmaRxHeader*)(pIpRxChannel->m_pRtpBuffer +
                      (pReconstructList[Index]->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));
    
            pRtpHeaderSrc1 = (RtpHeader*)((UInt8*)pDmaRxHeaderSrc1 + 
                           sizeof(DtaDmaRxHeader) + pReconstructList[Index]->m_RTPOffset);
            RtpSeqSrc1 = DtUInt16ByteSwap(pRtpHeaderSrc1->m_SequenceNumber);

            // Packet must be insterted after found packet?
            if (((RtpSeqSrc1+pRtpFecEntry->m_FecOffset)%0x10000) == 
                                                       pRtpDestEntry->m_RTPSequenceNumber)
            {
                 DtDbgOut(MAX, IP_RX, "Channel %d: 2-dT:%I64u TimeSrc1:%I64u "
                          "TimeSrc2:%I64u Seq1: %d FecOffset:%d NewTime:%I64u SeqDiff:%i",
                          pIpRxChannel->m_ChannelIndex,  dT, 
                          (UInt64)pDmaRxHeaderSrc1->m_Timestamp, 
                          (UInt64)pDmaRxHeaderSrc2->m_Timestamp,
                          RtpSeqSrc1, pRtpFecEntry->m_FecOffset, 
                          (UInt64)pDmaRxHeaderSrc1->m_Timestamp + dT, SeqDiff);
                return pDmaRxHeaderSrc1->m_Timestamp + (dT * pRtpFecEntry->m_FecOffset);
            }
            
            // Packet must be inserted before found packet?
            if (((RtpSeqSrc1-pRtpFecEntry->m_FecOffset)%0x10000) == 
                                                       pRtpDestEntry->m_RTPSequenceNumber)
            {
                DtDbgOut(MAX, IP_RX, "Channel %d: 3-dT:%I64u TimeSrc1:%I64u"
                          "TimeSrc2:%I64u Seq1: %d FecOffset:%d NewTime:%I64u SeqDiff:%i",
                          pIpRxChannel->m_ChannelIndex,  dT, 
                          (UInt64)pDmaRxHeaderSrc1->m_Timestamp, 
                          (UInt64)pDmaRxHeaderSrc2->m_Timestamp,
                          RtpSeqSrc1, pRtpFecEntry->m_FecOffset, 
                          (UInt64)pDmaRxHeaderSrc1->m_Timestamp - dT, SeqDiff);
                return pDmaRxHeaderSrc1->m_Timestamp - (dT * pRtpFecEntry->m_FecOffset);
            }
            Index++;
        }
    }
    DT_ASSERT(FALSE);
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxReconstructPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Remark: If a RTP header extension is available, skip the reconstruction because the
// IP packet lengths can have different sizes
//
Bool  DtaIpRxReconstructPacket(
    UserIpRxChannel*  pIpRxChannel,
    RtpListEntry*  pRtpDestEntry, 
    RtpListEntry*  pRtpFecEntry, 
    RtpListEntry*  pReconstructList[MAX_FEC_RECONSTR_ELEMENTS])
{
    FecHeader*  pFecHeader;
    DtaDmaRxHeader*  pDmaRxHeader;
    RtpHeader*  pRtpHeader;
    RtpHeader*  pRtpHeaderDest;
    UdpHeader*  pUdpHeaderDest;
    UInt8  PaddingBit;
    UInt8  ExtensionBit;
    UInt8  CCBits;
    UInt8  MarkerBit;
    UInt8  PayloadType;
    UInt32  TimeStamp;
    UInt16  Length;
    UInt8*  pDstData;
    UInt8*  pSrcData;
    UInt32*  pDstPayload;
    UInt32*  pSrcPayload;
    RtpListEntry*  pRtpCurEntry;
    Int  Counter;
    Int  PacketSize;
    Int  Index = 0;
    UInt16  RtpOffset;
    UInt16  PayloadOffset;
    
    // Source FEC
    pSrcData = pIpRxChannel->m_pRtpBuffer + 
                                  (pRtpFecEntry->m_BufIndex*DTA_IPRX_MAX_PACKET_LENGTH);

    pDmaRxHeader = (DtaDmaRxHeader*)pSrcData;
    pSrcData+= sizeof(DtaDmaRxHeader);

    pFecHeader = (FecHeader*)(pSrcData + pRtpFecEntry->m_PayloadOffset - 
                                                                      sizeof(FecHeader));
    pRtpHeader = (RtpHeader*)(pSrcData + pRtpFecEntry->m_RTPOffset);
    
    // Skip reconstruction if RTP header extension is available
    if (pRtpHeader->m_Extension)
        return FALSE;

    PacketSize = pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength - 
                                                        pRtpFecEntry->m_PayloadOffset - 4;

    PaddingBit = 0;
    ExtensionBit = 0;
    CCBits = 0;
    MarkerBit = 0;    
    PayloadType = pFecHeader->m_PtRecovery;
    TimeStamp = pFecHeader->m_TsRecovery;
    Length = pFecHeader->m_LengthRecovery;

    // Destination
    pDstData = (pIpRxChannel->m_pRtpBuffer + 
                                (pRtpDestEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));

    // Get first reconstruction packet to retrieve the offsets
    // L==1: no reconstruction packet available, so take the offsets from the FEC packet
    RtpOffset = pRtpFecEntry->m_RTPOffset;
    PayloadOffset = pRtpFecEntry->m_PayloadOffset - sizeof(FecHeader);

    // Copy start of packet into destination. Only needed for RAW ip mode and debugging
    if (pRtpFecEntry->m_FecNA > 1) 
    {
        pRtpCurEntry = pReconstructList[Index];
        pSrcData = (pIpRxChannel->m_pRtpBuffer + 
                                 (pRtpCurEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));

        DtMemCopy(pDstData, pSrcData, RtpOffset+sizeof(DtaDmaRxHeader));
    } else {
        // No DVB packet available , so take the header from the FEC packet
        DtMemCopy(pDstData, pSrcData - sizeof(DtaDmaRxHeader), 
                                                        RtpOffset+sizeof(DtaDmaRxHeader));
    }
    
    pSrcData = (UInt8*)pDmaRxHeader + pRtpFecEntry->m_PayloadOffset + 
                                                                   sizeof(DtaDmaRxHeader);
    
    pDmaRxHeader = (DtaDmaRxHeader*)pDstData;
    pDmaRxHeader->m_Timestamp = DtaIpRxCalcNewRxTimestamp(pIpRxChannel, pRtpDestEntry,
                                                          pRtpFecEntry, pReconstructList);
    pRtpDestEntry->m_PayloadOffset = PayloadOffset;
    pRtpDestEntry->m_RTPOffset = RtpOffset;
    
    // Sequence number already filled by main thread, so skip here
    // pRtpDestEntry->m_RTPSequenceNumber
    
    pDstData += sizeof(DtaDmaRxHeader);
    pRtpHeaderDest = (RtpHeader*)(pDstData + pRtpDestEntry->m_RTPOffset);
    pUdpHeaderDest = (UdpHeader*)(pDstData + pRtpDestEntry->m_RTPOffset - 
                                                                       sizeof(UdpHeader));
    pDstData = pDstData + pRtpDestEntry->m_PayloadOffset;

    // Store the FEC recovery data into the destination packet
    DtMemCopy(pDstData, pSrcData, PacketSize);

    pDstPayload = (UInt32*)pDstData;

    while (Index < pRtpFecEntry->m_FecNA-1) 
    {
        pRtpCurEntry = pReconstructList[Index++];
        pSrcData = (pIpRxChannel->m_pRtpBuffer + sizeof(DtaDmaRxHeader) +
                                 (pRtpCurEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));
        pRtpHeader = (RtpHeader*)(pSrcData + pRtpCurEntry->m_RTPOffset);
        
        // Skip reconstruction if RTP header extension is available
        if (pRtpHeader->m_Extension)
            return FALSE;
    
        pSrcData += pRtpCurEntry->m_PayloadOffset;
        pSrcPayload = (UInt32*)pSrcData;

        DT_ASSERT(PacketSize%4 == 0);

        for (Counter = 0; Counter < PacketSize/4; Counter++)
            pDstPayload[Counter] = pDstPayload[Counter]^pSrcPayload[Counter];
        

        PaddingBit = PaddingBit^pRtpHeader->m_Padding;
        ExtensionBit = ExtensionBit^pRtpHeader->m_Extension;
        CCBits = CCBits^pRtpHeader->m_CsrcCount;
        MarkerBit = MarkerBit^pRtpHeader->m_Marker;
        
        PayloadType = PayloadType^pRtpHeader->m_PayloadType;
        TimeStamp = TimeStamp^pRtpHeader->m_TimeStamp;
        Length = Length^DtUInt16ByteSwap((UInt16)PacketSize);
    }
    
    // Reconstruction ready
    // Store header information into destination packet
    pRtpHeaderDest->m_CsrcCount = CCBits;
    pRtpHeaderDest->m_Extension = ExtensionBit;
    pRtpHeaderDest->m_Marker = MarkerBit;
    pRtpHeaderDest->m_Padding = PaddingBit;
    pRtpHeaderDest->m_PayloadType = PayloadType;
    pRtpHeaderDest->m_TimeStamp = TimeStamp;
    pRtpHeaderDest->m_Version = 2;
    pRtpHeaderDest->m_SequenceNumber = 
                                     DtUInt16ByteSwap(pRtpDestEntry->m_RTPSequenceNumber);
    pUdpHeaderDest->m_Checksum = 0;
    pRtpHeaderDest->m_Ssrc = pRtpHeader->m_Ssrc;    // Stream ID

    DT_ASSERT(pRtpHeaderDest->m_Extension == 0);

    // If header extension is active, we discard this packet because reconstruction 
    // may have been failed due to packet length differences
    if (pRtpHeaderDest->m_Extension != 0)
        return FALSE;
    return TRUE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIpRxTryReconstructPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Pre: The m_RtpListSpinLock must be acquired 
// Post: The m_RtpListSpinLock is released and acquired again!! 
// (New packets can be inserted to the list inbetween the release and acquire.)
//
RtpListEntry*  DtaIpRxTryReconstructPacket(
    UserIpRxChannel*  pIpRxChannel, 
    UInt16  SequenceNumber)
{
    RtpListEntry*  pRtpEntry = NULL;
    RtpListEntry*  pRtpFecEntry = NULL;    
    RtpListEntry*  ReconstructList[MAX_FEC_RECONSTR_ELEMENTS];
    Int  i;    

    pRtpFecEntry = DtaIpRxGetFecReconstructionList(pIpRxChannel, 
                         &pIpRxChannel->m_RtpFecRowList, SequenceNumber, ReconstructList);

    if (pRtpFecEntry == NULL)
    {
        pRtpFecEntry = DtaIpRxGetFecReconstructionList(pIpRxChannel, 
                      &pIpRxChannel->m_RtpFecColumnList, SequenceNumber, ReconstructList);
    
    }
    if (pRtpFecEntry != NULL) 
    {   
        if (!DtIsListEmpty(&pIpRxChannel->m_RtpEmptyList)) 
        {
            DtListEntry*  pListEntry;
            pListEntry = DtHeadListRemove(&pIpRxChannel->m_RtpEmptyList);

            // Mark all elements used for reconstruction
            pRtpFecEntry->m_InUse = 1;
            for (i = 0; i < pRtpFecEntry->m_FecNA - 1; i++)
                ReconstructList[i]->m_InUse = 1;
          
            // Release spinlock before reconstruction, so list can be accessed during
            // reconstruction
            DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);
            pRtpEntry = DtContainingRecord(pListEntry, RtpListEntry, m_ListEntry);
            pRtpEntry->m_RTPSequenceNumber = SequenceNumber;
            
            if (!DtaIpRxReconstructPacket(pIpRxChannel, pRtpEntry, pRtpFecEntry, 
                                                                         ReconstructList))
            {
                // Reconstruction failed
                DtTailListInsert(&pIpRxChannel->m_RtpEmptyList, &pRtpEntry->m_ListEntry);
                pRtpEntry = NULL;
            }
            
            // Re-Acquire spinlock after reconstruction
            DtSpinLockAcquire(&pIpRxChannel->m_RtpListSpinLock);

            // Release all elements
            pRtpFecEntry->m_InUse = 0;
            for (i = 0; i < pRtpFecEntry->m_FecNA - 1; i++)
                ReconstructList[i]->m_InUse = 0;
        } 
    }
    return pRtpEntry;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRemoveOldFecPackets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Pre: The m_RtpListSpinLock must be acquired 
//
void  DtaIpRxRemoveOldFecPackets(
    UserIpRxChannel*  pIpRxChannel,
    DtListEntry*  pFecList,
    UInt16  LastSequenceNumber)
{
    DtListEntry*  pListEntry;
    DtListEntry*  pListEntryNext;
    RtpListEntry*  pRtpEntry;
    UInt32  EndSeqNum;
    UInt16  MaxGap = DtaIpRxGetMaxOutOfSync(pIpRxChannel);

    if (DtIsListEmpty(pFecList))
        return;    

    pListEntry = DtListNextGet(pFecList);

    while (pListEntry != pFecList)
    {
        pRtpEntry = DtContainingRecord(pListEntry, RtpListEntry, m_ListEntry);
        
        // Get the last sequence number for this fec packet
        EndSeqNum = (pRtpEntry->m_FecNA*pRtpEntry->m_FecOffset) + pRtpEntry->m_FecSNBase;
        pListEntryNext = DtListNextGet(pListEntry);
            
        if (DtaIpRxRtpIsSeqNumLess((UInt16)EndSeqNum, LastSequenceNumber) || 
           (!DtaIpRxRtpIsSeqNumLess((UInt16)EndSeqNum, LastSequenceNumber) && 
           pRtpEntry->m_InUse == 0 &&
           DtaIpRxGetSequenceNumberGap(LastSequenceNumber, (UInt16)EndSeqNum)>(4*MaxGap)))
        {
            DtDbgOut(MAX, IP_RX, 
               "Channel %d: RemoveOldFec. BufIndex:%i, SeqNum:%i, Base: %i, MaxSeqNum:%i",
                         pIpRxChannel->m_ChannelIndex, pRtpEntry->m_BufIndex, 
                         pRtpEntry->m_RTPSequenceNumber, 
                         pRtpEntry->m_FecSNBase, EndSeqNum);

            DtListEntryRemove(pListEntry);
            DtaIpRxRtpMoveEntryToEmptyListUnsafe(pIpRxChannel, pRtpEntry);
        }
        pListEntry = pListEntryNext;
    }
    return;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxProcessDvbWithFec -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxProcessDvbWithFec(UserIpRxChannel* pIpRxChannel)
{
    Bool  PacketReconstructed = FALSE;
    Bool  Stop;
    Bool  SeqNumMissing = FALSE;
    Bool  FecListEmpty = FALSE;
    Bool  DvbListEmpty = FALSE;
    DtListEntry*  pBackElement;
    RtpListEntry*  pRtpListEntry = NULL;
    RtpListEntry*  pRtpLastListEntry;
    RtpListEntry*  pRtpReconstructedEntry;
    UInt16  NumPacketsTried = 0;
    UInt16  Gap;
    UInt16  LastSeqNum;
    UInt16  CurSeqNum;
    UInt16  MissingSeqNumber = 0;
    UInt16*  pLastSeqNumToTx = NULL;

    DtSpinLockAcquire(&pIpRxChannel->m_RtpListSpinLock);

    // Try to reconstruct packets
    if (!pIpRxChannel->m_RtpFirstPacket && !DtIsListEmpty(&pIpRxChannel->m_RtpDvbList))
    {
        PacketReconstructed = TRUE;    // First time
        while (PacketReconstructed) 
        {
            // Retrieve the last received sequence number
            pRtpLastListEntry = 
                            DtContainingRecord(DtListPrevGet(&pIpRxChannel->m_RtpDvbList),
                            RtpListEntry, m_ListEntry);
            LastSeqNum = pRtpLastListEntry->m_RTPSequenceNumber;
            pLastSeqNumToTx = &LastSeqNum;
        
            PacketReconstructed = FALSE;
            SeqNumMissing = FALSE;
            pRtpListEntry = DtContainingRecord(DtListNextGet(&pIpRxChannel->m_RtpDvbList),
                                                               RtpListEntry, m_ListEntry);

            // The following works because m_RtpFirstPacket = FALSE;
            CurSeqNum = pIpRxChannel->m_RtpLastSeqNum;

            while (TRUE) 
            {
                if (DtIsListEmpty(&pIpRxChannel->m_RtpFecColumnList) &&
                    DtIsListEmpty(&pIpRxChannel->m_RtpFecRowList)) 
                {
                    // No FEC packets available
                    // No possibility to reconstruct
                    FecListEmpty = TRUE;
                    break;
                }

                Stop = FALSE;
                while ((UInt16)(CurSeqNum+1)!=pRtpListEntry->m_RTPSequenceNumber && !Stop)
                {
                    // We are missing at least one packet
                    // Try to reconstruct packet if gap is too big
                    if (!SeqNumMissing) 
                    {
                        SeqNumMissing = TRUE;
                        MissingSeqNumber = CurSeqNum+1;
                    }

                    Gap = DtaIpRxGetSequenceNumberGap((CurSeqNum+1), LastSeqNum);
                    if (Gap <= MAX_OUTOFSYNC_SEQ_NUM) 
                    {
                        // Gap is too small. The packet may be received later
                        Stop = TRUE;
                        NumPacketsTried++;
                        continue;
                    }

                    // Try to reconstruct this packet
                    CurSeqNum++;

                    DtDbgOut(MAX, IP_RX, 
                                      "Channel %d: TRY reconstruct SN:%i LSN:%i, NSN:%i",
                                      pIpRxChannel->m_ChannelIndex, CurSeqNum, LastSeqNum,
                                      pRtpListEntry->m_RTPSequenceNumber);

                    // Store the backward link to known if some 
                    // elements are inserted before the entry
                    pBackElement = DtListPrevGet(&pRtpListEntry->m_ListEntry);

                    // After this function, the spinlock is released and acquired again
                    // So some elements can be inserted inbetween
                    pRtpReconstructedEntry = DtaIpRxTryReconstructPacket(pIpRxChannel, 
                                                                               CurSeqNum);
                    if (pRtpReconstructedEntry != NULL) 
                    {                     
                        DtSpinLockAcquire(&pIpRxChannel->m_StatisticsSpinLock);
                        pIpRxChannel->m_NumPacketsReconstructed++;
                        DtSpinLockRelease(&pIpRxChannel->m_StatisticsSpinLock);

                        PacketReconstructed = TRUE;

                        DtDbgOut(MAX, IP_RX, "Channel %d: Reconstructed SN:%i", 
                                                 pIpRxChannel->m_ChannelIndex, CurSeqNum);
                        
                        if (pBackElement == DtListPrevGet(&pRtpListEntry->m_ListEntry)) 
                        {
                            // No elements inserted during reconstruction
                            // Insert packet into list before entry
                            DtTailListInsert(&pRtpListEntry->m_ListEntry , 
                                                    &pRtpReconstructedEntry->m_ListEntry);
                        } else {
                            // Sort element into list
                            if (!DtaIpRxRtpListAddEntryUnsafe(&pIpRxChannel->m_RtpDvbList, 
                                                                  pRtpReconstructedEntry))
                            {
                                // Element already in queue
                                // Add to empty list
                                DtaIpRxRtpMoveEntryToEmptyListUnsafe(pIpRxChannel, 
                                                                  pRtpReconstructedEntry);
                                // Get entry already in queue
                                pRtpReconstructedEntry = DtaIpRxRtpGetDvbEntryUnsafe(
                                                            pIpRxChannel,CurSeqNum, NULL);

                            } 
                            // Update next packet pointer
                            DT_ASSERT(pRtpReconstructedEntry != NULL);
                            if (pRtpReconstructedEntry == NULL)
                                pRtpListEntry = NULL;
                            else
                                pRtpListEntry = DtContainingRecord(
                                      DtListNextGet(&pRtpReconstructedEntry->m_ListEntry),
                                      RtpListEntry, m_ListEntry);
                        }
                    } else {                        
                        DtDbgOut(MAX, IP_RX, "Channel %d: NOT Reconstructed SN:%i", 
                                                 pIpRxChannel->m_ChannelIndex, CurSeqNum);
                    }

                    NumPacketsTried++;
                    // Only try to recover the first 2 matrixes.
                    if (NumPacketsTried > 2*DtaIpRxGetMaxOutOfSync(pIpRxChannel))
                        Stop = TRUE;                    
                }

                if (Stop)               
                    break;

                if (DtListNextGet(&pRtpListEntry->m_ListEntry) == 
                                                              &pIpRxChannel->m_RtpDvbList)
                    break; // End of list
                
                pRtpListEntry = 
                    DtContainingRecord(DtListNextGet(&pRtpListEntry->m_ListEntry),
                                                               RtpListEntry, m_ListEntry);
                CurSeqNum++;
                NumPacketsTried++;
            }
        }
    } else {
        DvbListEmpty = TRUE;
    }
    
    // Remove old FEC packets
    if (!pIpRxChannel->m_RtpFirstPacket) 
    {
        // Check if reconstruction block has run
        if (!DvbListEmpty && !FecListEmpty) 
        {
            if (SeqNumMissing) 
            {
                UInt16 Gap;
                if (MissingSeqNumber > LastSeqNum)
                    Gap = 0xffff - MissingSeqNumber + LastSeqNum;
                else 
                    Gap = LastSeqNum - MissingSeqNumber;
                
                if (Gap > 2*DtaIpRxGetMaxOutOfSync(pIpRxChannel)) 
                {
                    // Let the transmitter decide to transmit the packets
                    LastSeqNum = LastSeqNum - 2*DtaIpRxGetMaxOutOfSync(pIpRxChannel);
                    pLastSeqNumToTx = NULL;
                } else {
                    LastSeqNum = MissingSeqNumber;
                }
            }

            // Remove all FEC Packets not needed for packets < LastSeqNum
            DtDbgOut(MAX, IP_RX, "Channel %d: LastSeqNum:%d", 
                                                pIpRxChannel->m_ChannelIndex, LastSeqNum);
            
            DtaIpRxRemoveOldFecPackets(pIpRxChannel, &pIpRxChannel->m_RtpFecRowList, 
                                                                              LastSeqNum);
            DtaIpRxRemoveOldFecPackets(pIpRxChannel, &pIpRxChannel->m_RtpFecColumnList,
                                                                              LastSeqNum);
        }
    }

    DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);

    // Send all packets in sequence to user buffer
    DtaIpRxProcessRtpDvbPacket(pIpRxChannel, pLastSeqNumToTx);
}
