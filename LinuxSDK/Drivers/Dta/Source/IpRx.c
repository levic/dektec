//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* IpRx.c *#*#*#*#*#*#*#*#*#* (C) 2011-2015 DekTec
//
// Dta driver - IP RX functionality - Implementation of RX specific functionality for
//                                    IP ports.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2015 DekTec Digital Video B.V.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// IPRX Tag
#define  IPRX_TAG         0x78527049  // 'xRpI'

// This value must be larger then the max network delay in SMPTE-2022-7 mode
#define  DTA_IPRX_RTP_MAXINBUFTIME      (54000000/2)    // 500ms max time in buffer

#define  DTA_IPRX_RT_DMA_MAX_TRANSFER_SIZE  2621440 
                                        // The expected bitrate is 1GBit/s in a 
                                        // period of 20 ms.
                                        // calculation: 1GB/8 * 20mS = 2.5 Mbyte
#if defined(WINBUILD)
#define  DTA_IPRX_RT_BUF_SIZE           (DTA_IPRX_RT_DMA_MAX_TRANSFER_SIZE)       // Type2
#define  DTA_IPRX_DMA_BUF_SIZE          (1024*1024)         // Type1
#define  DTA_IPRX_NRT_SHARED_BUF_SIZE   (2*1024*1024)
#else
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20)) || !defined(CONFIG_MMU)
#define  DTA_IPRX_RT_BUF_SIZE           (DTA_IPRX_RT_DMA_MAX_TRANSFER_SIZE)       // Type2
#define  DTA_IPRX_DMA_BUF_SIZE          (1024*1024)         // Type1
#define  DTA_IPRX_NRT_SHARED_BUF_SIZE   (2*1024*1024)
#else
// Lower value for RHEL/CentOS kernels
#undef DTA_IPRX_RT_DMA_MAX_TRANSFER_SIZE
#define  DTA_IPRX_RT_DMA_MAX_TRANSFER_SIZE  (2*64*1024)
#define  DTA_IPRX_RT_BUF_SIZE           (DTA_IPRX_RT_DMA_MAX_TRANSFER_SIZE/2)     // Type2
#define  DTA_IPRX_DMA_BUF_SIZE          (64*1024)         // Type1
#define  DTA_IPRX_NRT_SHARED_BUF_SIZE   (64*1024)
#endif
#endif

#define  DTA_IPRX_RTP_LOOKUPTABLE_SIZE  (0xFFFF+1)

#define  DTA_IPRX_RT_PINGPONG_BUF_SIZE  (DTA_IPRX_RT_BUF_SIZE * 2)
#define  DTA_IPRX_PINGPONG_BUF_SIZE     (DTA_IPRX_DMA_BUF_SIZE * 2)


#define  DTA_IPRX_DMA_HEADER_TAG        (0x445441A0)
#define  DTA_ETH_HEADER_SIZE  14
#define  DTA_IPRX_MAX_PACKET_SIZE       (DT_IP_MAX_PACKET_SIZE)

#define  MAX_OUTOFSYNC_SEQ_NUM          12

#define  DTA_IPRX_LOST_PKT_SECOND         3

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtaIpRxWorkerThread(DtThread* pThread, void* pContext);
void  DtaIpRxRtWorkerThread(DtThread* pThread, void* pContext);
void  DtaIpRxReconstructThread(DtThread* pThread, void* pContext);
DtStatus  DtaIpRxUserChAttach(DtaIpUserChannels* pIpUserChannels, Int PortIndex,
                                Int IpPortIndex, DtFileObject* pFile, Int* pChannelIndex);
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
void  DtaIpRxResetStatistics(UserIpRxChannel* pIpRxChannel);
DtStatus  DtaIpRxSetIpPars(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex, 
            UInt8* pSrcIp, UInt16 SrcPort, UInt8* pDstIp, UInt16 DstPort, Int VlanId,
            UInt8* pSrcIp2, UInt16 SrcPort2, UInt8* pDstIp2, UInt16 DstPort2, Int VlanId2,
            Int FecMode, Int Protocol, Int Mode, Int Flags, Int VidStd, Int MaxBitrate,
            Int MaxSkew);
DtStatus  DtaIpRxGetIpPars(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                            Int* pDetFecNumCols, Int* pDetFecNumRows, Int* pDetNumTpPerIp,
                            Int* pDetProtocol, Int* pDetVideoStd);
DtStatus  DtaIpRxGetIpStat(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                            UInt* pLostIPPacketsAfterFec, UInt* pLostIPPacketsAfterSort,
                            UInt* pLostIPPacketsBeforeFec, UInt* pLostIPPacketsBeforeSort,
                            UInt* pTotNumIPPackets);
DtStatus  DtaIpRxSetRxControl(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex, 
                                                                           Int RxControl);

void  DtaIpRxGetLocalAddress(void* pContext, UInt8** pLocalAddress, UInt* pStart, 
                                                        UInt* pStop, UInt* pTransferSize);
void  DtaIpRxNrtGetLocalAddress(void* pContext, UInt8** pLocalAddress, UInt* pStart,
                                                        UInt* pStop, UInt* pTransferSize);
void DtaIpRxRtGetLocalAddress(void* pContext, UInt8** pLocalAddress, UInt* pStart,
                                                        UInt* pStop, UInt* pTransferSize);
void  DtaIpRxUserChInit(UserIpRxChannel* pIpRxChannel);
void  DtaIpRxUserChReset(UserIpRxChannel* pIpRxChannel, Bool ClearInputStatus);
UserIpRxChannel*  DtaIpRxUserChCreate(Int ChannelIndex, Int PortIndex, Int IpPortIndex,
                                                                     DtFileObject* pFile);
static void  DtaIpRxUserChAddToList(DtaIpUserChannels* pIpUserChannels, 
                                                               UserIpRxChannel* pIpRxNew);
static void  DtaIpRxUserChRemoveFromList(DtaIpUserChannels* pIpUserChannels,
                                                           UserIpRxChannel* pIpRxChannel);
Bool  DtaIpRxHandleLoopback(DtaIpUserChannels* pIpUserChannels, UInt8* pData, UInt Size);
void  DtaIpRxProcessIpRawPayLoad(UserIpRxChannel* pIpRxChannel, UInt8* pPayLoad, 
                                          Int PayLoadSize, UInt64 TimeStamp, Bool Header);
void  DtaIpRxRtpListsReset(UserIpRxChannel* pIpRxChannel);
void  DtaIpRxRtpListsResetUnsafe(UserIpRxChannel* pIpRxChannel);
void  DtaIpRxRtpListsCleanup( UserIpRxChannel* pIpRxChannel);
void  DtaIpRxSetBrmSkipUpdate(DtaIpUserChannels* pIpRxUserChannels, Bool State);
void  DtaIpRxUpdateBrmDpc(DtDpcArgs* pArgs);
void  DtaIpRxProcessDvbWithFec(UserIpRxChannel* pIpRxChannel, 
                                RtpListEntry** ppReconstructList, UInt64* pRefTimeStored);
void  DtaIpRxRtSliceEventSetDpc(DtDpcArgs* pArgs);
Int  DtaIpRxCalculateNumIpRxListenersType1(DtaIpPort* pIpPort, Bool IpV6Packet, 
                                           Int VlanId, UInt8* pSrcIp, 
                                           UInt8* pDstIp, UInt16 SrcPort, UInt16 DstPort);
Int  DtaIpRxCalculateNumIpRxListenersType2(DtaIpPort* pIpPort, UInt32 IdTag);
DtStatus  DtaIpRxSetupBuffer(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                        UInt TsBufSize, UInt IpBufSize, UInt JumboPktSize,
                                        UInt MinPktDelay, UInt MaxPktOutOfSync);
UInt16  DtaIpRxGetSequenceNumberGap(UInt16 SequenceNum1, UInt16 SequenceNum2);
void  DtaIpRxDoIpStatistics(DtaIpPort* pIpPort, UserIpRxChannel* pIpRxChannel,
                                     UInt64 CurRefTimestamp, DtaDmaRxHeader* pDmaRxHeader,
                                     UInt16 SeqNumber, UInt32 RtpTimestamp);
DtStatus  DtaIpRxGetIpStat2(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                               DtaIoctlIpRxCmdGetIpStat2Output* pIpStat2);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxDeviceInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxDeviceInit(DtaIpDevice* pIpDevice)
{
    DtStatus  Status = DT_STATUS_OK;

    Status = DtDpcInit(&pIpDevice->m_IpRtRxBrmDpc, DtaIpRxUpdateBrmDpc, TRUE);

    if (DT_SUCCESS(Status))
        Status = DtThreadInit(&pIpDevice->m_RxReconstructThread, 
                                  DtaIpRxReconstructThread, &pIpDevice->m_IpUserChannels);
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
                              NULL, NULL, &pIpPortType1->m_Rx.m_DmaChannel, TRUE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_RX, "Error initialising IpRx DMA channel");
        return Status;
    }

    Status = DtaIpCreateDmaPPBuffer(&pIpPortType1->m_Rx.m_DmaChannel, 
                                                     &pIpPortType1->m_RxPPBuffer, pIpPort,
                                                     DTA_IPRX_PINGPONG_BUF_SIZE,
                                                     DtaIpRxGetLocalAddress,
                                                     DT_DMA_DIRECTION_FROM_DEVICE, FALSE);
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

    Status = DtThreadInit(&pIpPort->m_IpPortType1.m_RxThread, DtaIpRxWorkerThread,
                                                                                 pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_RX, "Error creating IpRx Rx thread");
        return Status;
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxInitType2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxInitType2(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  DmaRegsOffsetNrtRx;
    UInt  DmaRegsOffsetRtRx;
    DtaIpPortType2*  pIpPortType2 = &pIpPort->m_IpPortType2;
    DtPropertyData*  pPropData = &pIpPort->m_pDvcData->m_PropData;
    
    // Initialise register offsets
    pIpPortType2->m_RxNrt.m_pRegs = NULL;
    pIpPortType2->m_RxNrt.m_RegsOffset = DtPropertiesGetUInt16(pPropData, 
                                             "NW_REGISTERS_NRT_RX", pIpPort->m_PortIndex);
    pIpPortType2->m_RxNrt.m_FifoOffset = 
                              pIpPortType2->m_RxNrt.m_RegsOffset + DTA_LOCALADDR_FIFODATA;
    
    DmaRegsOffsetNrtRx = DtPropertiesGetUInt16(pPropData, "NW_REGISTERS_NRT_RX_DMA",
                                                                    pIpPort->m_PortIndex);

    pIpPortType2->m_RxRt.m_RegsOffset = DtPropertiesGetUInt16(pPropData, 
                                              "NW_REGISTERS_RT_RX", pIpPort->m_PortIndex);
    pIpPortType2->m_RxRt.m_FifoOffset = 
                               pIpPortType2->m_RxRt.m_RegsOffset + DTA_LOCALADDR_FIFODATA;
    
    DmaRegsOffsetRtRx = DtPropertiesGetUInt16(pPropData, "NW_REGISTERS_RT_RX_DMA", 
                                                                    pIpPort->m_PortIndex);
    
    // Report configuration errors
    // Check if no property error occurred
    Status = DtaPropertiesReportDriverErrors(pIpPort->m_pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

     // Initialise NrtRx DMA channel
    
    // For NrtRx we use a Ping-pong buffer to transfer the data.
    // For the complete ping-pong buffer, the SGL list is created
    // The actual DMA transfer size will be the size divided by 2.
    Status = DtaDmaInitCh(pIpPort->m_pDvcData, pIpPort->m_PortIndex, 
                                  DTA_IPRX_PINGPONG_BUF_SIZE, DTA_DMA_MODE_TIMEOUT_ENABLE,
                                  DmaRegsOffsetNrtRx, DTA_DMA_FLAGS_DATA_BUF_NO_COPY, 2,
                                  NULL, NULL, &pIpPortType2->m_RxNrt.m_DmaChannel, TRUE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_RX, "Error initialising IpRx Nrt DMA channel");
        return Status;
    }

    Status = DtaIpCreateDmaPPBuffer(&pIpPortType2->m_RxNrt.m_DmaChannel, 
                                                  &pIpPortType2->m_RxNrtPPBuffer, pIpPort,
                                                  DTA_IPRX_PINGPONG_BUF_SIZE,
                                                  DtaIpRxNrtGetLocalAddress,
                                                  DT_DMA_DIRECTION_FROM_DEVICE, FALSE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_RX, "Error creating IpRx Nrt DMA buffer");
        return Status;
    }

    // Create and initialise Nrt Rx
    Status = DtaIpRxNrtCreateBuffer(&pIpPort->m_NrtChannels);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_RX, "Error creating IpRx NRT buffer");
        return Status;
    }

    Status = DtThreadInit(&pIpPort->m_IpPortType2.m_RxNrtThread, DtaIpRxWorkerThread,
                                                                                 pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_RX, "Error creating IpRx RxNrt thread");
        return Status;
    }

    // Create and initialise Rt Rx
    if (pIpPort->m_IpPortIndex%2 == 0)
    {
        // We set the FixedLocalAddress to FALSE because the SGL list local address
        // must be incremented.
        Status = DtaDmaInitCh(pIpPort->m_pDvcData, pIpPort->m_PortIndex, 
                                   DTA_IPRX_RT_PINGPONG_BUF_SIZE, 0,
                                   DmaRegsOffsetRtRx, DTA_DMA_FLAGS_DATA_BUF_NO_COPY, -1,
                                   NULL, NULL, &pIpPortType2->m_RxRt.m_DmaChannel, FALSE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, IP_RX, "Error initialising IpRx Rt DMA channel");
            return Status;
        }
        // We create a ping-pong buffer so we can easily use a DMA channel for 2 ports.
        Status = DtaIpCreateDmaPPBuffer(&pIpPortType2->m_RxRt.m_DmaChannel, 
                                                   &pIpPortType2->m_RxRtPPBuffer, pIpPort,
                                                   DTA_IPRX_RT_PINGPONG_BUF_SIZE,
                                                   DtaIpRxRtGetLocalAddress,
                                                   DT_DMA_DIRECTION_FROM_DEVICE, FALSE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, IP_RX, "Error creating IpRx Rt DMA buffer");
            return Status;
        }

        Status = DtThreadInit(&pIpPort->m_IpPortType2.m_RxRtThread, DtaIpRxRtWorkerThread,
                                                                                 pIpPort);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, IP_RX, "Error creating IpRx RxRt thread");
            return Status;
        }
    }
    pIpPort->m_IpPortType2.m_RxRtSliceReadIndex = 0;
    pIpPort->m_IpPortType2.m_RxRtSliceWriteIndex = 0;
    DtEventInit(&pIpPort->m_IpPortType2.m_RxRtSliceEvent, TRUE);
    Status = DtDpcInit(&pIpPort->m_IpPortType2.m_RxRtSliceEventSetDpc,
                                                         DtaIpRxRtSliceEventSetDpc, TRUE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_RX, "Error initializing DPC for slice event");
        return Status;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile, 
                                                                    DtIoctlObject* pIoctl)
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
    if (!DT_SUCCESS(DtaGetIpPortIndex(pDvcData, pIpRxCmdInput->m_PortIndex,
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
    case DTA_IP_RX_CMD_SETIPPARS2:
        pCmdStr = "DTA_IP_RX_CMD_SETIPPARS2";
        OutReqSize = 0;
        InReqSize += sizeof(DtaIoctlIpRxCmdSetIpPars2Input);
        break;
    case DTA_IP_RX_CMD_SETIPPARS3:
        pCmdStr = "DTA_IP_RX_CMD_SETIPPARS3";
        OutReqSize = 0;
        InReqSize += sizeof(DtaIoctlIpRxCmdSetIpPars3Input);
        break;
    case DTA_IP_RX_CMD_GETIPPARS:
        pCmdStr = "DTA_IP_RX_CMD_GETIPPARS";
        OutReqSize += sizeof(DtaIoctlIpRxCmdGetIpParsOutput);
        InReqSize += sizeof(DtaIoctlIpRxCmdGetIpParsInput);
        break;
    case DTA_IP_RX_CMD_GETIPPARS2:
        pCmdStr = "DTA_IP_RX_CMD_GETIPPARS2";
        OutReqSize += sizeof(DtaIoctlIpRxCmdGetIpPars2Output);
        InReqSize += sizeof(DtaIoctlIpRxCmdGetIpPars2Input);
        break;
    case DTA_IP_RX_CMD_GETIPSTAT:
        pCmdStr = "DTA_IP_RX_CMD_GETIPSTAT";
        OutReqSize += sizeof(DtaIoctlIpRxCmdGetIpStatOutput);
        InReqSize += sizeof(DtaIoctlIpRxCmdGetIpStatInput);
        break;
    case DTA_IP_RX_CMD_GETIPSTAT2:
        pCmdStr = "DTA_IP_RX_CMD_GETIPSTAT2";
        OutReqSize += sizeof(DtaIoctlIpRxCmdGetIpStat2Output);
        InReqSize += sizeof(DtaIoctlIpRxCmdGetIpStat2Input);
        break;
    case DTA_IP_RX_CMD_SETUPBUFFER:
        pCmdStr = "DTA_IP_RX_CMD_SETUPBUFFER";
        OutReqSize = 0;
        InReqSize += sizeof(DtaIoctlIpRxCmdSetupBufferInput);
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
                                       pIpRxCmdInput->m_PortIndex, IpPortIndex, pFile,
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
                                             0, NULL, 0, NULL, 0, 0,
                                             pIpRxCmdInput->m_Data.m_SetIpPars.m_FecMode,
                                             pIpRxCmdInput->m_Data.m_SetIpPars.m_Protocol,
                                             DTA_IP_V4, DTA_IP_NORMAL,
                                             DT_VIDSTD_TS,
                                             270000000,
                                             0);
            break;
        case DTA_IP_RX_CMD_SETIPPARS2:
            Status = DtaIpRxSetIpPars(&pDvcData->m_IpDevice.m_IpUserChannels,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_Channel,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_SrcIp,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_SrcPort,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_DstIp,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_DstPort,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_VlanId,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_SrcIp2,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_SrcPort2,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_DstIp2,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_DstPort2,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_VlanId2,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_FecMode,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_Protocol,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_Mode,
                                            pIpRxCmdInput->m_Data.m_SetIpPars2.m_Flags,
                                            DT_VIDSTD_TS,
                                            270000000,
                                            0);
            break;
        case DTA_IP_RX_CMD_SETIPPARS3:
            Status = DtaIpRxSetIpPars(&pDvcData->m_IpDevice.m_IpUserChannels,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_Channel,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_SrcIp,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_SrcPort,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_DstIp,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_DstPort,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_VlanId,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_SrcIp2,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_SrcPort2,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_DstIp2,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_DstPort2,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_VlanId2,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_FecMode,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_Protocol,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_Mode,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_Flags,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_VidStd,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_MaxBitrate,
                                          pIpRxCmdInput->m_Data.m_SetIpPars3.m_MaxSkew);
            break;
        case DTA_IP_RX_CMD_GETIPPARS:
            Status = DtaIpRxGetIpPars(&pDvcData->m_IpDevice.m_IpUserChannels,
                                         pIpRxCmdInput->m_Data.m_GetIpPars.m_Channel,
                                         &pIpRxCmdOutput->m_Data.m_GetIpPars.m_FecNumCols,
                                         &pIpRxCmdOutput->m_Data.m_GetIpPars.m_FecNumRows,
                                         &pIpRxCmdOutput->m_Data.m_GetIpPars.m_NumTpPerIp,
                                         &pIpRxCmdOutput->m_Data.m_GetIpPars.m_Protocol,
                                         NULL);
            break;
        case DTA_IP_RX_CMD_GETIPPARS2:
            Status = DtaIpRxGetIpPars(&pDvcData->m_IpDevice.m_IpUserChannels,
                                        pIpRxCmdInput->m_Data.m_GetIpPars.m_Channel,
                                        &pIpRxCmdOutput->m_Data.m_GetIpPars2.m_FecNumCols,
                                        &pIpRxCmdOutput->m_Data.m_GetIpPars2.m_FecNumRows,
                                        &pIpRxCmdOutput->m_Data.m_GetIpPars2.m_NumTpPerIp,
                                        &pIpRxCmdOutput->m_Data.m_GetIpPars2.m_Protocol,
                                        &pIpRxCmdOutput->m_Data.m_GetIpPars2.m_VidStd);
            break;
        case DTA_IP_RX_CMD_GETIPSTAT:
            Status = DtaIpRxGetIpStat(&pDvcData->m_IpDevice.m_IpUserChannels,
                            pIpRxCmdInput->m_Data.m_GetIpStat.m_Channel,
                            &pIpRxCmdOutput->m_Data.m_GetIpStat.m_LostIPPacketsAfterFec,
                            &pIpRxCmdOutput->m_Data.m_GetIpStat.m_Reserved1,
                            &pIpRxCmdOutput->m_Data.m_GetIpStat.m_LostIPPacketsBeforeFec,
                            &pIpRxCmdOutput->m_Data.m_GetIpStat.m_Reserved2,
                            &pIpRxCmdOutput->m_Data.m_GetIpStat.m_TotNumIPPackets);
            break;
        case DTA_IP_RX_CMD_GETIPSTAT2:
            Status = DtaIpRxGetIpStat2(&pDvcData->m_IpDevice.m_IpUserChannels,
                                              pIpRxCmdInput->m_Data.m_GetIpStat.m_Channel,
                                              &pIpRxCmdOutput->m_Data.m_GetIpStat2);
            break;
        case DTA_IP_RX_CMD_SETUPBUFFER:
            Status = DtaIpRxSetupBuffer(&pDvcData->m_IpDevice.m_IpUserChannels, 
                                   pIpRxCmdInput->m_Data.m_SetupBuffer.m_Channel,
                                   pIpRxCmdInput->m_Data.m_SetupBuffer.m_TsBufSize,
                                   pIpRxCmdInput->m_Data.m_SetupBuffer.m_IpBufSize,
                                   pIpRxCmdInput->m_Data.m_SetupBuffer.m_JumboPktSize,
                                   pIpRxCmdInput->m_Data.m_SetupBuffer.m_MinPktDelay,
                                   pIpRxCmdInput->m_Data.m_SetupBuffer.m_MaxPktOutOfSync);
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
// Precondition: PortIndex is the 0 based index to the Port set by the user
//               IpPortIndex is the 0-based index to an IpPort
//
DtStatus  DtaIpRxUserChAttach(DtaIpUserChannels* pIpUserChannels, Int PortIndex, 
                                 Int IpPortIndex, DtFileObject* pFile, Int* pChannelIndex)
{
    UserIpRxChannel*  pIpRxChannel;
    Int  ChannelIndex = 0;

    DtDbgOut(MAX, IP_RX, "Start");

    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChAccesMutex);

    pIpRxChannel = pIpUserChannels->m_pIpRxChannel;

    // find a free index
    while (pIpRxChannel != NULL)
    {
        // reuse a previously detached channel index if available
        if (pIpRxChannel->m_ChannelIndex != ChannelIndex)
            break;

        ChannelIndex++;
        pIpRxChannel = pIpRxChannel->m_pNext;
    }

    // Create a channel with this index
    pIpRxChannel = DtaIpRxUserChCreate(ChannelIndex, PortIndex, IpPortIndex, pFile);
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
DtStatus  DtaIpRxUserChClearFlags(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                                                Int Flags)
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
DtStatus  DtaIpRxUserChGetFlags(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                          Int* pFlags, Int* pLatchedFlags)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    *pFlags = pIpRxChannel->m_Flags;
    *pLatchedFlags = pIpRxChannel->m_LatchedFlags;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus DtaIpRxGetStatus(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                           Int* pClkDet, Int* pPacketSize)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Received packet size
    if (pIpRxChannel->m_DetVidStd == DT_VIDSTD_TS)
    {
        if (pIpRxChannel->m_PckSizeSrc == 188)
            *pPacketSize = DT_PCKSIZE_188;
        else if (pIpRxChannel->m_PckSizeSrc == 204)
            *pPacketSize = DT_PCKSIZE_204;
        else
            *pPacketSize = DT_PCKSIZE_INV;
    } else {
        switch(pIpRxChannel->m_DetVidStd)
        {
        case DT_VIDSTD_525I59_94: *pPacketSize = DT_SDIMODE_525; break;
        case DT_VIDSTD_625I50: *pPacketSize = DT_SDIMODE_625; break;
        default: *pPacketSize = DT_SDIMODE_INV; 
        }
    }

    // ClkDet
    *pClkDet = (pIpRxChannel->m_BrmEstimate == 0 ? 0 : 1);

    DtDbgOut(MAX, IP_RX, "Channel %d: PacketSize=%d (0=INV;2=188;3=204) ClkDet=%d", 
                                   pIpRxChannel->m_ChannelIndex, *pPacketSize, *pClkDet);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetTsRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaIpRxGetTsRate(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                                             Int* pTsRate)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    *pTsRate = pIpRxChannel->m_BrmEstimate;
    DtDbgOut(MAX,IP_RX,"Channel %d: TsRate=%d", pIpRxChannel->m_ChannelIndex, *pTsRate);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxHandleLoopback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaIpRxHandleLoopback(DtaIpUserChannels* pIpUserChannels, UInt8* pData, UInt Size)
{
    Bool  DataHandled = FALSE;
    UserIpRxChannel*  pIpRxChannel;

    // All data must be copied directly to a enabled channel
    // Wait until the Rx structure is available
    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);

    // Search the Rx channels to be enabled for loopback mode
    pIpRxChannel = pIpUserChannels->m_pIpRxChannel;
    while (pIpRxChannel != NULL)
    {
        // Check if the channel is the one we are searching for
        // Is the channel receiving?
        if (pIpRxChannel->m_RxControl==DT_RXCTRL_RCV && 
                             (pIpRxChannel->m_RxMode & DT_RXMODE_MASK) == DT_RXMODE_IPRAW)
        {
            DtaIpRxProcessIpRawPayLoad(pIpRxChannel, pData, Size, 0, FALSE);
            DataHandled = TRUE;
        }
        pIpRxChannel = pIpRxChannel->m_pNext;
    }

    DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
    return DataHandled;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxSetMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxSetRxMode(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                                               Int RxMode)
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
        if (!pIpRxChannel->m_SharedBuffer.m_Initialised || 
                                                      pIpRxChannel->m_pBufferHeader==NULL)
        {
            DtDbgOut(ERR, IP_RX, "Fifo buffer not initialized for channel %d.", 
                                                                            ChannelIndex);
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

    if (pIpRxChannel->m_SharedBuffer.m_Initialised && pIpRxChannel->m_pBufferHeader!=NULL)
        DtaIpRxRtpListsReset(pIpRxChannel);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxAddrMatcherPrepareAndAddEntry -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxAddrMatcherPrepareAndAddEntry(DtaIpPort* pIpPort, 
           AddressMatcherLookupEntry* pEntry, AddressMatcherLookupEntryPart2* pEntryPart2,
           Bool IpV6, UInt8* pSrcIp, UInt8* pDstIp, UInt16 DstPort)
{
    DtaIpPort*  pIpPortFirst = 
           &pIpPort->m_pDvcData->m_IpDevice.m_pIpPorts[pIpPort->m_IpPortIndex&0xfffffffe];
    Bool  IpV6SSM = FALSE;
    DtStatus  Status = DT_STATUS_OK;
    Int  NumListeners = 0;
    
    // Delete address matcher entry from list. If not in list, it's NOPed.
    DtaIpAddrMatcherDeleteEntry(pIpPort, pEntry, pEntryPart2);

    // Update entry
    pEntry->m_Data.m_Data0 = 0;
    pEntry->m_Data.m_Data1 = 0;
    pEntry->m_Data.m_Data2 = 0;
    pEntry->m_Data.m_Data3 = 0;
    pEntry->m_Data.m_Data4 = 0;
    pEntry->m_Data.m_Data5 = 0;

    pEntryPart2->m_Data.m_Data0 = 0;
    pEntryPart2->m_Data.m_Data1 = 0;
    pEntryPart2->m_Data.m_Data2 = 0;
    pEntryPart2->m_Data.m_Data3 = 0;
    pEntryPart2->m_Data.m_Data4 = 0;
    pEntryPart2->m_Data.m_Data5 = 0;

    if (IpV6)
    {
        pEntry->m_IpV6.m_Version = 1; // IPv6
        pEntry->m_IpV6.m_BaseUdpPort = DstPort;
        pEntry->m_IpV6.m_DestIpAddressH2 = (UInt64)(pDstIp[0]<<25) |
                                        (UInt64)(pDstIp[1])<<17 | (UInt64)(pDstIp[2])<<9 |
                                        (UInt64)(pDstIp[3])<<1 | (UInt64)(pDstIp[4])>>7;
        pEntry->m_IpV6.m_DestIpAddressH1 = (UInt64)(pDstIp[4])<<57 |
                                       (UInt64)(pDstIp[5])<<49 | (UInt64)(pDstIp[6])<<41 |
                                       (UInt64)(pDstIp[7])<<33 | (UInt64)(pDstIp[8])<<25 |
                                       (UInt64)(pDstIp[9])<<17 | (UInt64)(pDstIp[10])<<9 |
                                       (UInt64)(pDstIp[11])<<1 | (UInt64)(pDstIp[12])>>7;
        pEntry->m_IpV6.m_DestIpAddressL = (UInt64)(pDstIp[12])<<24 |
                                      (UInt64)(pDstIp[13])<<16 | (UInt64)(pDstIp[14])<<8 |
                                      (UInt64)(pDstIp[15]);
        pEntry->m_IpV6.m_NRT = 0;
        pEntry->m_IpV6.m_Port = pIpPort->m_IpPortIndex;
        if (DtaIpIsIpAddressEmpty(IpV6, pSrcIp))
        {
            pEntry->m_IpV6.m_SSM = 0;
            pEntryPart2->m_IpV6SSM.m_IpV6Part2.m_SrcIpAddressH1 = 0;
            pEntryPart2->m_IpV6SSM.m_IpV6Part2.m_SrcIpAddressH2 = 0;
            pEntryPart2->m_IpV6SSM.m_IpV6Part2.m_SrcIpAddressL = 0;
        } else {
            IpV6SSM = TRUE;
            pEntry->m_IpV6.m_SSM = 1;
            pEntryPart2->m_IpV6SSM.m_IpV6Part2.m_SrcIpAddressH2 = 
                                        (UInt64)(pSrcIp[0]<<25) |
                                        (UInt64)(pSrcIp[1])<<17 | (UInt64)(pSrcIp[2])<<9 |
                                        (UInt64)(pSrcIp[3])<<1 | (UInt64)(pSrcIp[4])>>7;
            pEntryPart2->m_IpV6SSM.m_IpV6Part2.m_SrcIpAddressH1 = 
                                       (UInt64)(pSrcIp[4])<<57 |
                                       (UInt64)(pSrcIp[5])<<49 | (UInt64)(pSrcIp[6])<<41 |
                                       (UInt64)(pSrcIp[7])<<33 | (UInt64)(pSrcIp[8])<<25 |
                                       (UInt64)(pSrcIp[9])<<17 | (UInt64)(pSrcIp[10])<<9 |
                                       (UInt64)(pSrcIp[11])<<1 | (UInt64)(pSrcIp[12])>>7;
            pEntryPart2->m_IpV6SSM.m_IpV6Part2.m_SrcIpAddressL = 
                                     (UInt64)(pSrcIp[12])<<24 | (UInt64)(pSrcIp[13])<<16 |
                                     (UInt64)(pSrcIp[14])<<8 | (UInt64)(pSrcIp[15]);
        }
 
        // Add entry
        DtaIpAddrMatcherAddEntry(pIpPort, &pIpPortFirst->m_IpPortType2.m_AddrMatchHead, 
                                                                     pEntry, pEntryPart2);
    } else {
        pEntry->m_IpV4.m_Version = 0; // IpV4
        pEntry->m_IpV4.m_BaseUdpPort = DstPort;
        pEntry->m_IpV4.m_DestIpAddressL = (UInt32)(pDstIp[0]&0x7f)<<24 |
                                        (UInt32)(pDstIp[1])<<16 | (UInt32)(pDstIp[2])<<8 |
                                        (UInt32)(pDstIp[3]);
        pEntry->m_IpV4.m_DestIpAddressH = ((pDstIp[0]&0x80) != 0?1:0);
        pEntry->m_IpV4.m_SrcIpAddress = (UInt32)(pSrcIp[0])<<24 | 
                                        (UInt32)(pSrcIp[1])<<16 | (UInt32)(pSrcIp[2])<<8 |
                                        (UInt32)(pSrcIp[3]);
        pEntry->m_IpV4.m_NRT = 0;
        pEntry->m_IpV4.m_Port = pIpPort->m_IpPortIndex;
        if (DtaIpIsIpAddressEmpty(IpV6, pSrcIp))
            pEntry->m_IpV4.m_SSM = 0;
        else
            pEntry->m_IpV4.m_SSM = 1;
            
        // Add entry
        DtaIpAddrMatcherAddEntry(pIpPort, &pIpPortFirst->m_IpPortType2.m_AddrMatchHead, 
                                                                            pEntry, NULL);
    }

    NumListeners = DtaIpRxCalculateNumIpRxListenersType2(pIpPort, (UInt32)
                    (IpV6SSM ? pEntryPart2->m_IpV6SSM.m_IpV6Part2_First.m_AddressIDTag 
                    : pEntry->m_Gen.m_AddressIDTag));
    Status = DtaIpReAllocIpRxNumListeners(pIpPort, NumListeners);
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxSetIpPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxSetIpPars(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex, 
            UInt8* pSrcIp, UInt16 SrcPort, UInt8* pDstIp, UInt16 DstPort, Int VlanId,
            UInt8* pSrcIp2, UInt16 SrcPort2, UInt8* pDstIp2, UInt16 DstPort2, Int VlanId2,
            Int FecMode, Int Protocol, Int Mode, Int Flags, Int VidStd, Int MaxBitrate,
            Int MaxSkew)
{
    Int  i;
    DtaIpDevice*  pIpDevice = &pIpUserChannels->m_pDvcData->m_IpDevice;
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    DtaIpPort*  pIpPort;
    Bool  IpV6 = (Flags & DTA_IP_V6) != 0;
    DtStatus  Status = DT_STATUS_OK;
    
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    pIpPort =  &pIpDevice->m_pIpPorts[pIpRxChannel->m_IpPortIndex];
    
    // Only allowed in Receive-Control state IDLE
    if (pIpRxChannel->m_RxControl != DT_RXCTRL_IDLE) 
    {
        DtDbgOut(ERR, IP_RX, "Setting IP pars not allowed if channel is NOT IDLE");
        return DT_STATUS_FAIL;
    }

    pIpRxChannel->m_SrcPort = SrcPort;
    pIpRxChannel->m_DstPort = DstPort;
    pIpRxChannel->m_SrcPort2 = SrcPort2;
    pIpRxChannel->m_DstPort2 = DstPort2;
    pIpRxChannel->m_IpParsMode = Mode;
    pIpRxChannel->m_IpParsFlags = Flags;
    pIpRxChannel->m_VlanId = VlanId;
    pIpRxChannel->m_VlanId2 = VlanId2;

    // Profile values
    pIpRxChannel->m_VidStd = VidStd;
    pIpRxChannel->m_MaxBitrate = MaxBitrate;
    pIpRxChannel->m_MaxSkew = MaxSkew;

    for (i=0; i<16; i++) 
    {
        pIpRxChannel->m_SrcIPAddress[i] = pSrcIp[i];
        pIpRxChannel->m_DstIPAddress[i] = pDstIp[i];
        if (pSrcIp2 != NULL)
            pIpRxChannel->m_SrcIPAddress2[i] = pSrcIp2[i];
        if (pDstIp2 != NULL)
            pIpRxChannel->m_DstIPAddress2[i] = pDstIp2[i];
    }

    pIpRxChannel->m_FecMode = FecMode;

    if (((Mode & DTA_IP_RX_2022_7) != 0) || FecMode!=DTA_FEC_DISABLE)
    {
        if (pIpRxChannel->m_pRtpAvailLookup1 == NULL)
        {
            pIpRxChannel->m_pRtpAvailLookup1 = (UInt32*)DtMemAllocPool(DtPoolNonPaged, 
                                               DTA_IPRX_RTP_LOOKUPTABLE_SIZE*4, IPRX_TAG);
            if (pIpRxChannel->m_pRtpAvailLookup1 == NULL)
                return DT_STATUS_OUT_OF_MEMORY;
        }
        if (pIpRxChannel->m_pRtpAvailLookup2 == NULL)
        {
            pIpRxChannel->m_pRtpAvailLookup2 = (UInt32*)DtMemAllocPool(DtPoolNonPaged, 
                                               DTA_IPRX_RTP_LOOKUPTABLE_SIZE*4, IPRX_TAG);
            if (pIpRxChannel->m_pRtpAvailLookup2 == NULL)
                return DT_STATUS_OUT_OF_MEMORY;
        }
        DtMemZero(pIpRxChannel->m_pRtpAvailLookup1, DTA_IPRX_RTP_LOOKUPTABLE_SIZE*4);
        DtMemZero(pIpRxChannel->m_pRtpAvailLookup2, DTA_IPRX_RTP_LOOKUPTABLE_SIZE*4);
    } else {
        if (pIpRxChannel->m_pRtpAvailLookup1 != NULL)
            DtMemFreePool(pIpRxChannel->m_pRtpAvailLookup1, IPRX_TAG);
        pIpRxChannel->m_pRtpAvailLookup1 = NULL;
        if (pIpRxChannel->m_pRtpAvailLookup2 != NULL)
            DtMemFreePool(pIpRxChannel->m_pRtpAvailLookup2, IPRX_TAG);
        pIpRxChannel->m_pRtpAvailLookup2 = NULL;
    }

    if (pIpUserChannels->m_pDvcData->m_IpDevice.m_PortType == DTA_IPPORT_TYPE2)
    {
        DtaIpPort*  pIpPortFirst = 
                           &pIpDevice->m_pIpPorts[pIpRxChannel->m_IpPortIndex&0xfffffffe];
        AddressMatcherLookupEntry*  pEntry = 
                        &pIpRxChannel->m_AddrMatcherEntry[0][pIpRxChannel->m_IpPortIndex];
        AddressMatcherLookupEntryPart2*  pEntryPart2 = 
                &pIpRxChannel->m_AddrMatcherEntrySSMPart2[0][pIpRxChannel->m_IpPortIndex];

        // Check if pDstIp is not in the SSM range and pSrcIp not is 0.0.0.0
        // If so, we have to check the pSrcIp in our receive thread. The FW does not
        // support it.
        pIpRxChannel->m_DoSSMCheckSw = FALSE;
        if (DtaIpNeedDriverSSMCheckType2(IpV6, pSrcIp, pDstIp))
        {
            DtaIpClearIpAddress(IpV6, pSrcIp);
            pIpRxChannel->m_DoSSMCheckSw = TRUE;
        }

        Status = DtaIpRxAddrMatcherPrepareAndAddEntry(pIpPort, pEntry, pEntryPart2, IpV6,
                                                                 pSrcIp, pDstIp, DstPort);
        if (Status != DT_STATUS_OK)
            return Status;
        
        if (FecMode != DTA_FEC_DISABLE)
        {
            pEntry = &pIpRxChannel->m_AddrMatcherEntry[ADDRM_TYPE_FECROW] \
                                                            [pIpRxChannel->m_IpPortIndex];
            pEntryPart2 = &pIpRxChannel->m_AddrMatcherEntrySSMPart2[ADDRM_TYPE_FECROW] \
                                                            [pIpRxChannel->m_IpPortIndex];
            Status = DtaIpRxAddrMatcherPrepareAndAddEntry(pIpPort, pEntry, pEntryPart2, 
                                          IpV6, pSrcIp, pDstIp, DstPort+FEC_INC_ROW_PORT);
            if (Status != DT_STATUS_OK)
                return Status;

            pEntry = &pIpRxChannel->m_AddrMatcherEntry[ADDRM_TYPE_FECCOLUMN] \
                                                            [pIpRxChannel->m_IpPortIndex];
            pEntryPart2 = &pIpRxChannel->m_AddrMatcherEntrySSMPart2[ADDRM_TYPE_FECCOLUMN]\
                                                            [pIpRxChannel->m_IpPortIndex];
            Status = DtaIpRxAddrMatcherPrepareAndAddEntry(pIpPort, pEntry, pEntryPart2, 
                                       IpV6, pSrcIp, pDstIp, DstPort+FEC_INC_COLUMN_PORT);
            if (Status != DT_STATUS_OK)
                return Status;
        } else {
            // Delete FEC entries if used in previous call
            pEntry = &pIpRxChannel->m_AddrMatcherEntry[ADDRM_TYPE_FECROW] \
                                                                 [pIpPort->m_IpPortIndex];
            pEntryPart2 = &pIpRxChannel->m_AddrMatcherEntrySSMPart2[ADDRM_TYPE_FECROW] \
                                                                 [pIpPort->m_IpPortIndex];
            DtaIpAddrMatcherDeleteEntry(pIpPort, pEntry, pEntryPart2);

            pEntry = &pIpRxChannel->m_AddrMatcherEntry[ADDRM_TYPE_FECCOLUMN] \
                                                                 [pIpPort->m_IpPortIndex];
            pEntryPart2 = 
                         &pIpRxChannel->m_AddrMatcherEntrySSMPart2[ADDRM_TYPE_FECCOLUMN] \
                                                                 [pIpPort->m_IpPortIndex];
            DtaIpAddrMatcherDeleteEntry(pIpPort, pEntry, pEntryPart2);
        }

        if ((Mode & DTA_IP_RX_2022_7) != 0)
        {
            DtaIpPort*  pIpPort = &pIpDevice->m_pIpPorts[pIpPortFirst->m_IpPortIndex+1];
            AddressMatcherLookupEntry*  pEntry = 
                      &pIpRxChannel->m_AddrMatcherEntry[0][pIpPortFirst->m_IpPortIndex+1];
            AddressMatcherLookupEntryPart2*  pEntryPart2 = 
              &pIpRxChannel->m_AddrMatcherEntrySSMPart2[0][pIpPortFirst->m_IpPortIndex+1];
            DT_ASSERT((pIpPortFirst->m_IpPortIndex & 1) == 0);

            pIpRxChannel->m_DoSSMCheckSw2 = FALSE;
            if (DtaIpNeedDriverSSMCheckType2(IpV6, pSrcIp2, pDstIp2))
            {
                DtaIpClearIpAddress(IpV6, pSrcIp2);
                pIpRxChannel->m_DoSSMCheckSw2 = TRUE;
            }
            
            // We need a second entry for port2
            Status = DtaIpRxAddrMatcherPrepareAndAddEntry(pIpPort, pEntry, pEntryPart2, 
                                                        IpV6, pSrcIp2, pDstIp2, DstPort2);
            if (Status != DT_STATUS_OK)
                return Status;

            if (FecMode != DTA_FEC_DISABLE)
            {
                pEntry = &pIpRxChannel->m_AddrMatcherEntry[ADDRM_TYPE_FECROW] \
                                                          [pIpPort->m_IpPortIndex];
                pEntryPart2 = 
                            &pIpRxChannel->m_AddrMatcherEntrySSMPart2[ADDRM_TYPE_FECROW] \
                                                          [pIpPort->m_IpPortIndex];
                Status = DtaIpRxAddrMatcherPrepareAndAddEntry(pIpPort, pEntry, 
                          pEntryPart2, IpV6, pSrcIp2, pDstIp2, DstPort2+FEC_INC_ROW_PORT);
                if (Status != DT_STATUS_OK)
                    return Status;

                pEntry = &pIpRxChannel->m_AddrMatcherEntry[ADDRM_TYPE_FECCOLUMN] \
                                                          [pIpPort->m_IpPortIndex];
                pEntryPart2 = 
                         &pIpRxChannel->m_AddrMatcherEntrySSMPart2[ADDRM_TYPE_FECCOLUMN] \
                         [pIpPort->m_IpPortIndex];
                DtaIpRxAddrMatcherPrepareAndAddEntry(pIpPort, pEntry, pEntryPart2, IpV6,
                                          pSrcIp2, pDstIp2, DstPort2+FEC_INC_COLUMN_PORT);
            } else {
                // Delete FEC entries if used in previous call
                pEntry = &pIpRxChannel->m_AddrMatcherEntry[ADDRM_TYPE_FECROW] \
                                                              [pIpPort->m_IpPortIndex];
                pEntryPart2 =
                            &pIpRxChannel->m_AddrMatcherEntrySSMPart2[ADDRM_TYPE_FECROW] \
                                                                 [pIpPort->m_IpPortIndex];
                DtaIpAddrMatcherDeleteEntry(pIpPort, pEntry, pEntryPart2);

                pEntry = &pIpRxChannel->m_AddrMatcherEntry[ADDRM_TYPE_FECCOLUMN] \
                                                              [pIpPort->m_IpPortIndex];
                pEntryPart2 = 
                         &pIpRxChannel->m_AddrMatcherEntrySSMPart2[ADDRM_TYPE_FECCOLUMN] \
                                                                 [pIpPort->m_IpPortIndex];
                DtaIpAddrMatcherDeleteEntry(pIpPort, pEntry, pEntryPart2);
            }
        } else if ((pIpPort->m_IpPortIndex&1) == 0) {
            DtaIpPort*  pIpPort = &pIpDevice->m_pIpPorts[pIpPortFirst->m_IpPortIndex+1];
            AddressMatcherLookupEntry*  pEntry = 
                      &pIpRxChannel->m_AddrMatcherEntry[0][pIpPortFirst->m_IpPortIndex+1];
            AddressMatcherLookupEntryPart2*  pEntryPart2 = 
              &pIpRxChannel->m_AddrMatcherEntrySSMPart2[0][pIpPortFirst->m_IpPortIndex+1];
            DT_ASSERT((pIpPortFirst->m_IpPortIndex & 1) == 0);
            
            // Delete entry port2 if current channel is using port1
            // Delete address matcher entry from list. If not in list, it's NOPed.
            DtaIpAddrMatcherDeleteEntry(pIpPort, pEntry, pEntryPart2);

            pEntry = &pIpRxChannel->m_AddrMatcherEntry[ADDRM_TYPE_FECROW] \
                                                          [pIpPort->m_IpPortIndex];
            pEntryPart2 = &pIpRxChannel->m_AddrMatcherEntrySSMPart2[ADDRM_TYPE_FECROW] \
                                                          [pIpPort->m_IpPortIndex];
            DtaIpAddrMatcherDeleteEntry(pIpPort, pEntry, pEntryPart2);

            pEntry = &pIpRxChannel->m_AddrMatcherEntry[ADDRM_TYPE_FECCOLUMN] \
                                                          [pIpPort->m_IpPortIndex];
            pEntryPart2 = 
                         &pIpRxChannel->m_AddrMatcherEntrySSMPart2[ADDRM_TYPE_FECCOLUMN] \
                                                          [pIpPort->m_IpPortIndex];
            DtaIpAddrMatcherDeleteEntry(pIpPort, pEntry, pEntryPart2);
        }

        // Update address matcher
        DtaIpAddrMatcherUpdateTable(pIpPort, 
                                            &pIpPortFirst->m_IpPortType2.m_AddrMatchHead);
    } else {
        Int  NumListeners;
        pIpRxChannel->m_DoSSMCheckSw = TRUE;
        NumListeners = DtaIpRxCalculateNumIpRxListenersType1(pIpPort, IpV6, 
                                            VlanId, pSrcIp, pDstIp, SrcPort, DstPort);
        Status = DtaIpReAllocIpRxNumListeners(pIpPort, NumListeners);
        if (Status != DT_STATUS_OK)
            return Status;
        if (FecMode != DTA_FEC_DISABLE)
        {
           NumListeners = DtaIpRxCalculateNumIpRxListenersType1(pIpPort, IpV6, 
                                                          VlanId, pSrcIp, pDstIp, SrcPort,
                                                          DstPort+FEC_INC_COLUMN_PORT);
            Status = DtaIpReAllocIpRxNumListeners(pIpPort, NumListeners);
            if (Status != DT_STATUS_OK)
                return Status;
            NumListeners = DtaIpRxCalculateNumIpRxListenersType1(pIpPort, IpV6, 
                                                          VlanId, pSrcIp, pDstIp, SrcPort,
                                                          DstPort+FEC_INC_ROW_PORT);
            Status = DtaIpReAllocIpRxNumListeners(pIpPort, NumListeners);
            if (Status != DT_STATUS_OK)
                return Status;
        }
    }

    // Reset channel after including input streams status information
    DtaIpRxUserChReset(pIpRxChannel, TRUE);
    if ((Flags & DTA_IP_V6) != 0)
        DtDbgOut(MIN, IP_RX, "Channel %d: New IP pars: SrcPort=%d; DstPort=%d "
                          "DstIP: %d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d",
                          pIpRxChannel->m_ChannelIndex, 
                          pIpRxChannel->m_SrcPort, pIpRxChannel->m_DstPort,
                          pIpRxChannel->m_DstIPAddress[0],pIpRxChannel->m_DstIPAddress[1],
                          pIpRxChannel->m_DstIPAddress[2],pIpRxChannel->m_DstIPAddress[3],
                          pIpRxChannel->m_DstIPAddress[4],pIpRxChannel->m_DstIPAddress[5],
                       pIpRxChannel->m_DstIPAddress[6],pIpRxChannel->m_DstIPAddress[7],
                       pIpRxChannel->m_DstIPAddress[8],pIpRxChannel->m_DstIPAddress[9],
                       pIpRxChannel->m_DstIPAddress[10],pIpRxChannel->m_DstIPAddress[11],
                       pIpRxChannel->m_DstIPAddress[12],pIpRxChannel->m_DstIPAddress[13],
                       pIpRxChannel->m_DstIPAddress[14],pIpRxChannel->m_DstIPAddress[15]);
    else
        DtDbgOut(MIN, IP_RX, "Channel %d: New IP pars: SrcPort=%d; DstPort=%d "
                         "SrcIP: %d.%d.%d.%d DstIP: %d.%d.%d.%d",
                         pIpRxChannel->m_ChannelIndex, 
                         pIpRxChannel->m_SrcPort, pIpRxChannel->m_DstPort,
                         pIpRxChannel->m_SrcIPAddress[0],pIpRxChannel->m_SrcIPAddress[1],
                         pIpRxChannel->m_SrcIPAddress[2],pIpRxChannel->m_SrcIPAddress[3],
                         pIpRxChannel->m_DstIPAddress[0],pIpRxChannel->m_DstIPAddress[1],
                         pIpRxChannel->m_DstIPAddress[2],pIpRxChannel->m_DstIPAddress[3]);
    pIpRxChannel->m_IpParsValid = TRUE;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxSetupBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxSetupBuffer(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                        UInt TsBufSize, UInt IpBufSize, UInt JumboPktSize,
                                        UInt MinPktDelay, UInt MaxPktOutOfSync)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Only allowed in Receive-Control state IDLE
    if (pIpRxChannel->m_RxControl != DT_RXCTRL_IDLE) 
    {
        DtDbgOut(ERR, IP_RX, "Setting IpRx buffers not allowed if channel is NOT IDLE");
        return DT_STATUS_FAIL;
    }
    
    // Check if total buf size less then shared buffer
    if (TsBufSize+IpBufSize+JumboPktSize > (UInt)pIpRxChannel->m_BufSize)
    {
        DtDbgOut(ERR, IP_RX, "Error setting IpRx buffers. Buffer too small");
        return DT_STATUS_INVALID_PARAMETER;
    }

    pIpRxChannel->m_pFifo = pIpRxChannel->m_SharedBuffer.m_pBuffer +
                                                                 sizeof(IpRxBufferHeader);
    pIpRxChannel->m_FifoSize = TsBufSize - DTA_IPRX_BUFWRAPSIZE;

    // Jumbo packet buffer
    pIpRxChannel->m_MaxJumboPktSize = JumboPktSize;
    pIpRxChannel->m_pJumboPktBuffer = pIpRxChannel->m_pFifo + TsBufSize;

    // Initialize RTP List entries
    pIpRxChannel->m_pRtpListEntries = pIpRxChannel->m_pJumboPktBuffer + JumboPktSize;
    pIpRxChannel->m_pRtpBuffer = DtaIpRxRtpListsInit(pIpRxChannel, IpBufSize);

    // RTP Packet delays
    pIpRxChannel->m_MinPacketDelay = MinPktDelay;
    pIpRxChannel->m_MaxPacketOutOfSync = MaxPktOutOfSync;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetIpPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxGetIpPars(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
         Int* pDetFecNumCols, Int* pDetFecNumRows, Int* pDetNumTpPerIp, Int* pDetProtocol,
         Int* pDetVidStd)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    *pDetFecNumCols = pIpRxChannel->m_DetFecNumColumns;
    *pDetFecNumRows = pIpRxChannel->m_DetFecNumRows;
    *pDetNumTpPerIp = pIpRxChannel->m_DetNumTpPerIp;
    *pDetProtocol = pIpRxChannel->m_DetProtocol;
    if (pDetVidStd != NULL)
        *pDetVidStd = pIpRxChannel->m_DetVidStd;
    DtDbgOut(MIN, IP_RX, "Channel %d: NumCols=%d; NumRows=%d;" 
               "NumTpPerIp=%d; Protocol=%d; DetVideoStd=%d", pIpRxChannel->m_ChannelIndex,
               *pDetFecNumCols, *pDetFecNumRows, *pDetNumTpPerIp, *pDetProtocol, 
               pIpRxChannel->m_DetVidStd);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetIpStat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxGetIpStat(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
            UInt* pLostIPPacketsAfterFec, UInt* pReserved1, UInt* pLostIPPacketsBeforeFec,
            UInt* pReserved2, UInt* pTotNumIPPackets)
{    
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    DtSpinLockAcquire(&pIpRxChannel->m_StatisticsSpinLock);
    *pTotNumIPPackets = pIpRxChannel->m_TotNumPackets;
    *pLostIPPacketsAfterFec = pIpRxChannel->m_NumPacketsNotReconstructed;
    *pLostIPPacketsBeforeFec = pIpRxChannel->m_NumPacketsNotReconstructed +
                               pIpRxChannel->m_NumPacketsReconstructed;

    *pReserved1 = 0;
    *pReserved2 = 0;

    DtSpinLockRelease(&pIpRxChannel->m_StatisticsSpinLock);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetIpStat2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxGetIpStat2(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                DtaIoctlIpRxCmdGetIpStat2Output* pIpStat2)
{
    UserIpRxChannel*  pIpRxChannel = DtaIpRxUserChGet(pIpUserChannels, ChannelIndex);
    Int  i;
    Int  Index;
    if (pIpRxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    DtSpinLockAcquire(&pIpRxChannel->m_StatisticsSpinLock);
    pIpStat2->m_TotNumIPPackets = pIpRxChannel->m_TotNumPackets;
    pIpStat2->m_LostIPPacketsAfterFec = pIpRxChannel->m_NumPacketsNotReconstructed;
    pIpStat2->m_LostIPPacketsBeforeFec = pIpRxChannel->m_NumPacketsNotReconstructed +
                                                  pIpRxChannel->m_NumPacketsReconstructed;
    pIpStat2->m_Spare = 0;
    DtSpinLockRelease(&pIpRxChannel->m_StatisticsSpinLock);

    if ((pIpRxChannel->m_IpParsMode & DTA_IP_RX_2022_7) == 0)
        Index = 0;
    else if (pIpRxChannel->m_BerNumIpPacketsMainMinute[0]!=0 && 
                                                    pIpRxChannel->m_LastSeqNumStat[0]!=-1)
        Index = 0;
    else 
        Index = 1;

    pIpStat2->m_BerNumIpPacketsMainSec = pIpRxChannel->m_BerNumIpPacketsMainSecond[Index];
    pIpStat2->m_BerNumIpPacketsMainMin = pIpRxChannel->m_BerNumIpPacketsMainMinute[Index];
        
    pIpStat2->m_BerNumIpPacketsLostMainSec = 
                                     pIpRxChannel->m_BerNumIpPacketsLostMainSecond[Index];
    pIpStat2->m_BerNumIpPacketsLostMainMin = 
                                     pIpRxChannel->m_BerNumIpPacketsLostMainMinute[Index];
    
    for (i=0; i<2; i++)
    {
        pIpStat2->m_BerNumIpPacketsSec[i] = pIpRxChannel->m_BerNumIpPacketsSecond[i];
        pIpStat2->m_BerNumIpPacketsLostSec[i] = 
                                             pIpRxChannel->m_BerNumIpPacketsLostSecond[i];
        pIpStat2->m_BerNumIpPacketsMin[i] = pIpRxChannel->m_BerNumIpPacketsMinute[i];
        pIpStat2->m_BerNumIpPacketsLostMin[i] = 
                                             pIpRxChannel->m_BerNumIpPacketsLostMinute[i];
    
        pIpStat2->m_DelayFactorSec[i] = pIpRxChannel->m_MaxDelayFSecond[i] - 
                                                       pIpRxChannel->m_MinDelayFSecond[i];
        pIpStat2->m_DelayFactorMin[i] = pIpRxChannel->m_MaxDelayFMinute[i] - 
                                                       pIpRxChannel->m_MinDelayFMinute[i];
        pIpStat2->m_MinIpatSec[i] = pIpRxChannel->m_MinIpatSecond[i];
        pIpStat2->m_MaxIpatSec[i] = pIpRxChannel->m_MaxIpatSecond[i];
        pIpStat2->m_MinIpatMin[i] = pIpRxChannel->m_MinIpatMinute[i];
        if (pIpRxChannel->m_MaxIpatMinute[i] == -1)
            pIpStat2->m_MaxIpatMin[i] = 0;
        else
            pIpStat2->m_MaxIpatMin[i] = pIpRxChannel->m_MaxIpatMinute[i];

        pIpStat2->m_MinSkewSec = pIpRxChannel->m_MinSkewSecond;
        pIpStat2->m_MaxSkewSec = pIpRxChannel->m_MaxSkewSecond;
        pIpStat2->m_MinSkewMin = pIpRxChannel->m_MinSkewMinute;
        pIpStat2->m_MaxSkewMin = pIpRxChannel->m_MaxSkewMinute;

        pIpStat2->m_NumIpPacketsReceived[i] = pIpRxChannel->m_NumIpPacketsReceived[i];
        pIpStat2->m_NumIpPacketsLost[i] = pIpRxChannel->m_NumIpPacketsLost[i];
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxCleanupType1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxCleanupType1(DtaIpPort* pIpPort)
{
    DtaIpPortType1*  pIpPortType1 = &pIpPort->m_IpPortType1;
    
    
    // Cleanup DMA Rx
    DtaIpCleanupDmaPPBuffer(&pIpPortType1->m_Rx.m_DmaChannel, 
                                                             &pIpPortType1->m_RxPPBuffer);
    
    // Cleanup Nrt Rx
    DtaIpRxNrtCleanupBuffer(&pIpPort->m_NrtChannels);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxCleanupType2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxCleanupType2(DtaIpPort* pIpPort)
{
    DtaIpPortType2*  pIpPortType2 = &pIpPort->m_IpPortType2;
    
    // Cleanup Nrt Rx
    DtaDmaCleanupCh(pIpPort->m_pDvcData, &pIpPortType2->m_RxNrt.m_DmaChannel);

    // Cleanup DMA Nrt Rx
    DtaIpCleanupDmaPPBuffer(&pIpPortType2->m_RxNrt.m_DmaChannel, 
                                                          &pIpPortType2->m_RxNrtPPBuffer);
    
    // Cleanup Nrt Rx
    DtaIpRxNrtCleanupBuffer(&pIpPort->m_NrtChannels);

    // Cleanup Rt Rx
    if (pIpPort->m_IpPortIndex%2 == 0)
    {
        DtaDmaCleanupCh(pIpPort->m_pDvcData, &pIpPortType2->m_RxRt.m_DmaChannel);
        DtaIpCleanupDmaPPBuffer(&pIpPortType2->m_RxRt.m_DmaChannel, 
                                                           &pIpPortType2->m_RxRtPPBuffer);
    }
    
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxDevicePowerdownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxDevicePowerdownPre(DtaIpDevice* pIpDevice)
{
    DtDbgOut(AVG, IP_RX, "Stopping IpRx reconstructor thread");
    DtThreadStop(&pIpDevice->m_RxReconstructThread);
    DtDbgOut(AVG, IP_RX, "IpRx reconstructor thread stopped");
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxDevicePowerdown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxDevicePowerdown(DtaIpDevice* pIpDevice)
{
    DtDpcWaitForCompletion(&pIpDevice->m_IpRtRxBrmDpc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxPowerdownType1Pre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxPowerdownType1Pre(DtaIpPort* pIpPort)
{
    DtDbgOut(AVG, IP_RX, "Stopping IpRx thread");
    DtThreadStop(&pIpPort->m_IpPortType1.m_RxThread);
    DtDbgOut(AVG, IP_RX, "IpRx thread stopped");
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxPowerdownType2Pre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxPowerdownType2Pre(DtaIpPort* pIpPort)
{
    DtDbgOut(AVG, IP_RX, "Stopping IpRx Nrt thread");
    DtThreadStop(&pIpPort->m_IpPortType2.m_RxNrtThread);
    DtDbgOut(AVG, IP_RX, "IpRx Nrt thread stopped");
    if (pIpPort->m_IpPortIndex%2 == 0)
    {
        DtDbgOut(AVG, IP_RX, "Stopping IpRx Rt thread");
        DtThreadStop(&pIpPort->m_IpPortType2.m_RxRtThread);
        DtDbgOut(AVG, IP_RX, "IpRx Rt thread stopped");
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxPowerdownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxPowerdownPre(DtaIpPort* pIpPort)
{    
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        DtaIpRxPowerdownType1Pre(pIpPort);
    else
        DtaIpRxPowerdownType2Pre(pIpPort);
    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxDevicePowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxDevicePowerup(DtaIpDevice* pIpDevice)
{
    DtStatus  Status = DT_STATUS_OK;
 
    DtDbgOut(AVG, IP_RX, "Starting IpRx reconstruct thread");
    
    // Try to start the main receiving worker thread
    Status = DtThreadStart(&pIpDevice->m_RxReconstructThread);

    if (DT_SUCCESS(Status))
        DtDbgOut(AVG, IP_RX, "IpRx reconstruct thread started");

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxPowerupType1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxPowerupType1(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;

    DtDbgOut(AVG, IP_RX, "Starting IpRx thread");
    
    // Try to start the main receiving worker thread
    Status = DtThreadStart(&pIpPort->m_IpPortType1.m_RxThread);
   
    if (DT_SUCCESS(Status))
        DtDbgOut(AVG, IP_RX, "IpRx thread started");

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxPowerupType2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxPowerupType2(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;

    DtDbgOut(AVG, IP_RX, "Starting IpRx Nrt thread");
    
    // Try to start the NRT receiving worker thread
    Status = DtThreadStart(&pIpPort->m_IpPortType2.m_RxNrtThread);
   
    if (!DT_SUCCESS(Status))
        return Status;
    DtDbgOut(AVG, IP_RX, "IpRx Nrt thread started");
    
    if (pIpPort->m_IpPortIndex%2 == 0)
    {
        DtDbgOut(AVG, IP_RX, "Starting IpRx Rt thread");
    
        // Try to start the RT receiving worker thread
        Status = DtThreadStart(&pIpPort->m_IpPortType2.m_RxRtThread);
   
        if (!DT_SUCCESS(Status))
            return Status;
        DtDbgOut(AVG, IP_RX, "IpRx Rt thread started");
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpRxPowerup(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;

    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        Status = DtaIpRxPowerupType1(pIpPort);
    else
       Status = DtaIpRxPowerupType2(pIpPort);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChGetUnsafe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UserIpRxChannel*  DtaIpRxUserChGetUnsafe(DtaIpUserChannels* pIpUserChannels, 
                                                                         Int ChannelIndex)
{
    UserIpRxChannel*  pIpRxChannel = pIpUserChannels->m_pIpRxChannel;
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
void  DtaIpRxUserChDestroy(DtaIpUserChannels* pIpUserChannels,
                                                            UserIpRxChannel* pIpRxChannel)
{
    DtaIpDevice*  pIpDevice = &pIpUserChannels->m_pDvcData->m_IpDevice;
    Int  i;
    
    // Gain access to the channel list
    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChAccesMutex);
    
    // Remove the channel from the list, make shure the workerthreads do not try to access
    // the about to destroy channel. 
    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);

    DtDbgOut(AVG, IP_RX, "Destroying channel %i", pIpRxChannel->m_ChannelIndex);

    // Stop Brm updates
    DtaIpRxSetBrmSkipUpdate(pIpUserChannels, TRUE);

    // Wait for finishing processing
    DtaIpRxUserChRefWaitFinished(pIpRxChannel);

    if (pIpUserChannels->m_pDvcData->m_IpDevice.m_PortType == DTA_IPPORT_TYPE2)
    {
        DtaIpPort*  pIpPortFirst = 
                           &pIpDevice->m_pIpPorts[pIpRxChannel->m_IpPortIndex&0xfffffffe];
        DtaIpPort*  pIpPortSecond = 
                            &pIpDevice->m_pIpPorts[pIpPortFirst->m_IpPortIndex+1];
        // Delete entry in address matcher
        for (i=0; i<3; i++)
        {
            DtaIpAddrMatcherDeleteEntry(pIpPortFirst, 
               &pIpRxChannel->m_AddrMatcherEntry[i][pIpPortFirst->m_IpPortIndex],
               &pIpRxChannel->m_AddrMatcherEntrySSMPart2[i][pIpPortFirst->m_IpPortIndex]);
            DtaIpAddrMatcherDeleteEntry(pIpPortSecond, 
              &pIpRxChannel->m_AddrMatcherEntry[i][pIpPortSecond->m_IpPortIndex],
              &pIpRxChannel->m_AddrMatcherEntrySSMPart2[i][pIpPortSecond->m_IpPortIndex]);
        }

        // Update address matcher
        DtaIpAddrMatcherUpdateTable(pIpPortFirst, 
                                            &pIpPortFirst->m_IpPortType2.m_AddrMatchHead);
    }

    // Remove the channel from the list
    DtaIpRxUserChRemoveFromList(pIpUserChannels, pIpRxChannel);
    if (pIpUserChannels->m_pIpRxChannel == NULL)
    {
        // Initialize array size to minimal size
        for (i=0; i<pIpUserChannels->m_pDvcData->m_NumIpPorts; i++)
        {
            DtaIpPort*  pIpPort = &pIpUserChannels->m_pDvcData->m_IpDevice.m_pIpPorts[i];
            DtaIpReAllocIpRxNumListeners(pIpPort, -1);
        }
    }

    DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
    
    // Free internal resources
    //DtaIpRxRtpListsCleanup(pIpRxChannel);
    DtaShBufferClose(&pIpRxChannel->m_SharedBuffer);
    if (pIpRxChannel->m_pRtpAvailLookup1 != NULL)
        DtMemFreePool(pIpRxChannel->m_pRtpAvailLookup1, IPRX_TAG);
    if (pIpRxChannel->m_pRtpAvailLookup2 != NULL)
        DtMemFreePool(pIpRxChannel->m_pRtpAvailLookup2, IPRX_TAG);
    
    DtMemFreePool(pIpRxChannel, IPRX_TAG);
    pIpRxChannel = NULL;

    // Allow Brm updates and release mutexes
    DtaIpRxSetBrmSkipUpdate(pIpUserChannels, FALSE);

    DtFastMutexRelease(&pIpUserChannels->m_IpRxChAccesMutex);

    DtDbgOut(AVG, IP_RX, "Channel is destroyed");
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Private Channel Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIpRxUserChInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxUserChInit(UserIpRxChannel* pIpRxChannel)
{
    UInt8  i;
    
    // Initialize concurrency objects
    DtSpinLockInit(&pIpRxChannel->m_RtpListSpinLock);
    DtSpinLockInit(&pIpRxChannel->m_StatisticsSpinLock);
    DtSpinLockInit(&pIpRxChannel->m_FlagsSpinLock);


    pIpRxChannel->m_RxControl = DT_RXCTRL_IDLE;
    pIpRxChannel->m_RxMode = DT_RXMODE_ST188;
    pIpRxChannel->m_FecMode = DTA_FEC_DISABLE;

    pIpRxChannel->m_VidStd = DT_VIDSTD_TS;
    pIpRxChannel->m_MaxBitrate = 270000000;

    for (i=0; i<3; i++)
    {
        pIpRxChannel->m_AddrMatcherEntry[i][0].m_StreamType = i;
        pIpRxChannel->m_AddrMatcherEntry[i][0].m_Gen.m_Port = 0;
        pIpRxChannel->m_AddrMatcherEntry[i][1].m_StreamType = i;
        pIpRxChannel->m_AddrMatcherEntry[i][1].m_Gen.m_Port = 1;
    }
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
    if (pIpRxChannel->m_pBufferHeader != NULL)
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

    pIpRxChannel->m_NumIpPacketsReceived[0] = 0;
    pIpRxChannel->m_NumIpPacketsReceived[1] = 0;
    pIpRxChannel->m_NumIpPacketsLost[0] = 0;
    pIpRxChannel->m_NumIpPacketsLost[1] = 0;

    // Fragmented packets
    pIpRxChannel->m_FragmentId = 0;
    pIpRxChannel->m_FragmentOffset = 0;
    
    // Status flags and spin lock for access protection
    pIpRxChannel->m_Flags = 0;                // Clear DTA_RX_FIFO_OVF, DTA_RX_SYNC_ERR
    pIpRxChannel->m_LatchedFlags = 0;
    pIpRxChannel->m_TrpModePckSeqCount = 0;    // Start with sequence number zero

    // SDI handling
    pIpRxChannel->m_SdiNumBytesCopied = -1;

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
        pIpRxChannel->m_DetVidStd = DT_VIDSTD_UNKNOWN;
        pIpRxChannel->m_RstCntFecRow = 0;
        pIpRxChannel->m_RstCntFecColumn = 0;
        DtaIpRxResetStatistics(pIpRxChannel);
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChAddToList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Add (insert) a channel in the RX channels list
//
// Pre: Mutex and spinlock to protect list must be aquired.
//
void  DtaIpRxUserChAddToList(DtaIpUserChannels* pIpUserChannels, 
                                                                UserIpRxChannel* pIpRxNew)
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
void  DtaIpRxUserChRemoveFromList(DtaIpUserChannels* pIpUserChannels, 
                                                            UserIpRxChannel* pIpRxChannel)
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
    if (pIpRxChannel->m_pNext != NULL)
        pIpRxChannel->m_pNext->m_pPrev = pIpRxChannel->m_pPrev;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxUserChCreate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Create and returns an empty channel object. Only its identifiers are set.
//
UserIpRxChannel*  DtaIpRxUserChCreate(Int ChannelIndex, Int PortIndex, Int IpPortIndex, 
                                                                      DtFileObject* pFile)
{    
    // Create a channel
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
    pIpRxChannel->m_PortIndex = PortIndex;
    pIpRxChannel->m_IpPortIndex = IpPortIndex;
    
    // Identifies channels to clean for channels that where never detached 
    pIpRxChannel->m_FileObject = *pFile;

    // Init shared buffer
    // DTAPI will create a shared buffer later.
    pIpRxChannel->m_SharedBuffer.m_pBuffer = NULL;
    pIpRxChannel->m_SharedBuffer.m_pDmaCh = NULL;
    pIpRxChannel->m_SharedBuffer.m_Initialised = FALSE;
    pIpRxChannel->m_SharedBuffer.m_Purpose = DTA_SH_BUF_PURPOSE_GEN;

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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetLocalAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxGetLocalAddress(void* pContext, UInt8** pLocalAddress, UInt* pStart,
                                                         UInt* pStop, UInt* pTransferSize)
{
    DtaIpPort*  pIpPort = (DtaIpPort*)pContext;

    *pStart = 0;
    *pStop = 0;
    *pTransferSize = 0;

    DT_ASSERT(pIpPort->m_PortType == DTA_IPPORT_TYPE1);
    *pLocalAddress = (UInt8*)(size_t)pIpPort->m_IpPortType1.m_Rx.m_FifoOffset;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxNrtGetLocalAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxNrtGetLocalAddress(void* pContext, UInt8** pLocalAddress, UInt* pStart,
                                                         UInt* pStop, UInt* pTransferSize)
{
    DtaIpPort*  pIpPort = (DtaIpPort*)pContext;

    *pStart = 0;
    *pStop = 0;
    *pTransferSize = 0;

    DT_ASSERT(pIpPort->m_PortType == DTA_IPPORT_TYPE2);
    *pLocalAddress = (UInt8*)(size_t)pIpPort->m_IpPortType2.m_RxNrt.m_FifoOffset;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtGetLocalAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtaIpRxRtGetLocalAddress(void* pContext, UInt8** pLocalAddress, UInt* pStart,
                                                         UInt* pStop, UInt* pTransferSize)
{
    // The context is updated each transfer for port 0/1,
    DtaIpPort*  pIpPort = (DtaIpPort*)pContext;
    
    DT_ASSERT(pIpPort->m_PortType == DTA_IPPORT_TYPE2);
    DT_ASSERT(pIpPort->m_IpPortType2.m_RxRtSliceReadIndex != 
                                            pIpPort->m_IpPortType2.m_RxRtSliceWriteIndex);
    *pStart = 0;
    *pStop = 0xffffffff;
    *pLocalAddress = (UInt8*)(size_t)
       pIpPort->m_IpPortType2.m_RxRtSlicePtr[pIpPort->m_IpPortType2.m_RxRtSliceReadIndex];
    *pTransferSize = 
      pIpPort->m_IpPortType2.m_RxRtSliceSize[pIpPort->m_IpPortType2.m_RxRtSliceReadIndex];
    DT_ASSERT((*pTransferSize+(size_t)(*pLocalAddress)) <= 
                          DtaMemGetPointer(pIpPort->m_IpPortType2.m_pMemoryControllerRegs,
                          DTA_MEM_RT_RX_BUF_END));
    

    DT_ASSERT(*pTransferSize != 0);

    if (*pTransferSize > DTA_IPRX_RT_BUF_SIZE)
        *pTransferSize = DTA_IPRX_RT_BUF_SIZE;
    // Update read index pointer
    if (pIpPort->m_IpPortType2.m_RxRtSliceReadIndex+1 == DTA_IPRX_MAX_SLICES)
        pIpPort->m_IpPortType2.m_RxRtSliceReadIndex = 0;
    else
        pIpPort->m_IpPortType2.m_RxRtSliceReadIndex++;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtSliceEventSetDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxRtSliceEventSetDpc(DtDpcArgs* pArgs)
{
    DtaIpPort*  pIpPort = (DtaIpPort*)pArgs->m_pContext;
    DtEventSet(&pIpPort->m_IpPortType2.m_RxRtSliceEvent);
}
/*
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtUpdateSlicePointer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtaIpRxRtUpdateSlicePointer(DtaIpPort* pIpPort)
{   DtDpcArgs  DpcArgs;
    DtaIpPortType2*  pIpType2 = &pIpPort->m_IpPortType2;
    
    pIpType2->m_RxRtSliceSize[pIpType2->m_RxRtSliceWriteIndex] =
                                           DtaRegTxSliceSizeGet(pIpType2->m_RxRt.m_pRegs);
    if (pIpType2->m_RxRtSliceSize[pIpType2->m_RxRtSliceWriteIndex] == 0)
        pIpType2->m_RxRtSlicePtr[pIpType2->m_RxRtSliceWriteIndex] = 0;
    else {
        pIpType2->m_RxRtSlicePtr[pIpType2->m_RxRtSliceWriteIndex] =
                                        DtaRegRxSlicePointerGet(pIpType2->m_RxRt.m_pRegs);
        DtDbgOut(AVG, IP_RX, "New slice: Pointer: %p  Size: %d",
        pIpType2->m_RxRtSlicePtr[pIpType2->m_RxRtSliceWriteIndex], 
                              pIpType2->m_RxRtSliceSize[pIpType2->m_RxRtSliceWriteIndex]);
    }

    //DtDbgOut(ERR, IP_RX, "Port:%i Size:%d Pointer:%p WriteIdx:%i ReadIdx:%i", 
    //                           pIpPort->m_IpPortIndex, 
    //                           pIpType2->m_RxRtSliceSize[pIpType2->m_RxRtSliceWriteIndex],
    //                           pIpType2->m_RxRtSlicePtr[pIpType2->m_RxRtSliceWriteIndex],
    //                           pIpType2->m_RxRtSliceWriteIndex,
    //                           pIpType2->m_RxRtSliceReadIndex);

    DT_ASSERT(pIpType2->m_RxRtSliceSize[pIpType2->m_RxRtSliceWriteIndex] <=
                                                                    DTA_IPRX_RT_BUF_SIZE);
    // Update write index pointer
    if (pIpType2->m_RxRtSliceWriteIndex+1 == DTA_IPRX_MAX_SLICES)
    {
        // Check if there's a free place after increment. If not, we skip this sample
        if (pIpType2->m_RxRtSliceReadIndex == 0)
        {
            pIpPort->m_NumRxFifoOverflow++;
            //DtDbgOut(AVG, IP_RX, "[%i] Slice buffer FULL", pIpPort->m_IpPortIndex);
            return;
        }
        pIpType2->m_RxRtSliceWriteIndex = 0;
    } else {
        // Check if there's a free place after increment. If not, we skip this sample
        if (pIpType2->m_RxRtSliceReadIndex == pIpType2->m_RxRtSliceWriteIndex+1)
        {
            pIpPort->m_NumRxFifoOverflow++;
            //DtDbgOut(AVG, IP_RX, "[%i] Slice buffer FULL", pIpPort->m_IpPortIndex);
            return;
        }
        pIpType2->m_RxRtSliceWriteIndex++;
    }
    // Start DPC to set Event
    DpcArgs.m_pContext = pIpPort;
    DtDpcSchedule(&pIpType2->m_RxRtSliceEventSetDpc, &DpcArgs);

}*/

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtUpdateSlicePointer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxRtUpdateSlicePointer(DtaIpPort* pIpPort, Bool SliceOverflow)
{   DtDpcArgs  DpcArgs;
    DtaIpPortType2*  pIpType2 = &pIpPort->m_IpPortType2;
    UInt  SliceSize = DtaRegTxSliceSizeGet(pIpType2->m_RxRt.m_pRegs);
    //Bool  Overflow = FALSE;
    UInt  NewWriteIndex;

    // Check for overflow
    if (pIpType2->m_RxRtSliceWriteIndex+1 == DTA_IPRX_MAX_SLICES)
    {
        // Check if there's a free place after increment. If not, we skip this sample
        if (pIpType2->m_RxRtSliceReadIndex == 0)
        {
            pIpPort->m_NumRxFifoOverflow++;
            return;
        }
        NewWriteIndex = 0;
    } else {
        // Check if there's a free place after increment. If not, we skip this sample
        if (pIpType2->m_RxRtSliceReadIndex == pIpType2->m_RxRtSliceWriteIndex+1)
        {
            pIpPort->m_NumRxFifoOverflow++;
            return;
        }
        NewWriteIndex = pIpType2->m_RxRtSliceWriteIndex+1;
    }
    
    if (SliceSize==0 && SliceOverflow)
    {
        // Firmware is full and we don't get any data anymore.
        
        // First check if complete list is empty.
        Int  ReadIndex = pIpType2->m_RxRtSliceReadIndex;
        Bool  Empty = TRUE;
        while (Empty && pIpType2->m_RxRtSliceWriteIndex != ReadIndex)
        {
            if (pIpType2->m_RxRtSliceSize[ReadIndex] != 0)
                Empty = FALSE;
            else {
                ReadIndex++;
                if (ReadIndex == DTA_IPRX_MAX_SLICES)
                    ReadIndex = 0;
            }
        }

        // Wait for empty list to prevent overlap address locations read
        if (Empty)
        {
            // Read last 4 bytes of previous slice to clear the memory.
            UInt32  StartRtRxMemory = DtaMemGetPointer(
                                           pIpPort->m_IpPortType2.m_pMemoryControllerRegs,
                                           DTA_MEM_RT_RX_BUF_START);
            pIpType2->m_RxRtSliceSize[pIpType2->m_RxRtSliceWriteIndex] = 4;

            pIpType2->m_RxRtSlicePtr[pIpType2->m_RxRtSliceWriteIndex] = 
                                    DtaRegRxSlicePointerGet(pIpType2->m_RxRt.m_pRegs) - 4;
            if (pIpType2->m_RxRtSlicePtr[pIpType2->m_RxRtSliceWriteIndex] < 
                                                                          StartRtRxMemory)
                pIpType2->m_RxRtSlicePtr[pIpType2->m_RxRtSliceWriteIndex] =
                          DtaMemGetPointer(pIpPort->m_IpPortType2.m_pMemoryControllerRegs,
                          DTA_MEM_RT_RX_BUF_END) - 4;
            DtDbgOut(ERR, IP_RX, "[%i] Firmware overflow. Clear state by reading last"
                               "4 bytes from %xh", pIpPort->m_IpPortIndex,
                               pIpType2->m_RxRtSlicePtr[pIpType2->m_RxRtSliceWriteIndex]);
                
            DT_ASSERT(pIpType2->m_RxRtSliceSize[pIpType2->m_RxRtSliceWriteIndex] <=
                                                                    DTA_IPRX_RT_BUF_SIZE);
        } else {
            pIpType2->m_RxRtSliceSize[pIpType2->m_RxRtSliceWriteIndex] = 0;
            pIpType2->m_RxRtSlicePtr[pIpType2->m_RxRtSliceWriteIndex] = 
                                        DtaRegRxSlicePointerGet(pIpType2->m_RxRt.m_pRegs);
        }
    } else {
        pIpType2->m_RxRtSliceSize[pIpType2->m_RxRtSliceWriteIndex] = SliceSize;
        pIpType2->m_RxRtSlicePtr[pIpType2->m_RxRtSliceWriteIndex] = 
                                        DtaRegRxSlicePointerGet(pIpType2->m_RxRt.m_pRegs);
        DT_ASSERT(pIpType2->m_RxRtSliceSize[pIpType2->m_RxRtSliceWriteIndex] <=
                                                                    DTA_IPRX_RT_BUF_SIZE);
    }
    if (pIpType2->m_RxRtSliceSize[pIpType2->m_RxRtSliceWriteIndex] != 0)
    {
        /*DtDbgOut(AVG, IP_RX, "[%i] New slice: Pointer: %p  Size: %d",
                            pIpPort->m_IpPortIndex, 
                                pIpType2->m_RxRtSlicePtr[pIpType2->m_RxRtSliceWriteIndex], 
                                pIpType2->m_RxRtSliceSize[pIpType2->m_RxRtSliceWriteIndex]);*/
    }
    pIpType2->m_RxRtSliceWriteIndex = NewWriteIndex;
    
    // Start DPC to set Event
    DpcArgs.m_pContext = pIpPort;
    DtDpcSchedule(&pIpType2->m_RxRtSliceEventSetDpc, &DpcArgs);
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
void  DtaIpRxSendPacketToNwDriver(DtaIpPort* pIpPort, DtaDmaRxHeader* pDmaRxHeader,
                                       UInt8* pPacket, UInt PacketSize)   // Excluding CRC
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

    // Always include the CRC to the network driver.
    PacketSize+= 4;

    TotalLength = PacketSize + sizeof(DtaDmaRxHeader);
    ReadOffset = pSharedInfo->m_ReadOffset;
    WriteOffset = pSharedInfo->m_WriteOffset;

    // Align packets on DWORD's
    if (TotalLength%4 != 0)
        TotalLength+= 4 - (TotalLength%4);

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
// This functions will increment the channel's use count returned in pIpRxA
// Pre: m_IpRxListenersMutex mutex must be acquired
//
UInt DtaIpRxIsPacketForDVB(
    DtaIpUserChannels*  pIpUserChannels,
    RtpHeader*  pRtpHeader,
    UInt8*  pIpSrc,
    UInt8*  pIpDst, 
    UInt16   SrcPort,
    UInt16 DstPort,
    Bool  MulticastPkt,            // True: If packet received is a Multicast packet
    Bool  IpV6Packet,
    Int  VlanId,
    UserIpRxChannel**  pIpRxA,
    Int  MaxNumListeners)
{
    Int  i;
    UserIpRxChannel*  pIpRxChannel;
    Bool  Stop;
    Int  Index = 0;
    Bool  IsFecPortDst = FALSE;
    Bool  IsFecPortSrc = FALSE;
   
    // Wait until the Rx structure is available
    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);

    // Search the Rx channels for the DVB stream and return the ChannelIndex
    // if found
    pIpRxChannel = pIpUserChannels->m_pIpRxChannel;
    while (pIpRxChannel != NULL) 
    {
        // Check if the channel is the one we are searching for
        Bool  EmptySrcIp;
        Bool  EmptyDstIp;
        
        // For bitrate calculation to work in idle state, 
        // we check the RxControl somewhere else and only check here 
        // if the parameters are set. 
        if (!pIpRxChannel->m_IpParsValid)
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }
        if (pIpRxChannel->m_VlanId != VlanId)
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }
        if (IpV6Packet && (pIpRxChannel->m_IpParsFlags & DTA_IP_V6)==0)
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }
        if (!IpV6Packet && (pIpRxChannel->m_IpParsFlags & DTA_IP_V6)!=0)
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }

        // Check if source + destination IP address is correct.
        EmptySrcIp = TRUE;
        EmptyDstIp = TRUE;
        for (i=0; i<(IpV6Packet?16:4); i++) 
        {
            if (pIpRxChannel->m_SrcIPAddress[i] != 0)
                EmptySrcIp = FALSE;
            if (pIpRxChannel->m_DstIPAddress[i] != 0)
                EmptyDstIp = FALSE;
        }

        Stop = FALSE;
        for (i=0; i<(IpV6Packet?16:4); i++) 
        {
            if (!EmptySrcIp && pIpRxChannel->m_SrcIPAddress[i]!=pIpSrc[i])
            {
                Stop = TRUE;
                break;
            }

            // If a multicast packet is received, 
            // the m_DstIPAddress may not be 0.0.0.0
            if ((!EmptyDstIp || MulticastPkt) && 
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

        // If a RTP header is available, we must check on its contents
        // to determine if we must add the packet to a FEC stream
        // Add the channel if we cannot use the contents of the packet, and
        // the source port matches a FEC port.
        // For COP#3 there was no requirement for the source port
        // For SMPTE-2022 the source port must be equal.
        IsFecPortSrc = FALSE;
        // This must be enabled if the applications supports the new DTAPI
        //if ((pIpRxChannel->m_IpParsFlags & DTA_IP_RX_DIFFSRCPORTFEC) != 0)
        {
            if (pIpRxChannel->m_SrcPort!=0 && pIpRxChannel->m_FecMode!=DTA_FEC_DISABLE && 
                                               pIpRxChannel->m_DetProtocol==DTA_PROTO_RTP)
            {
                if (pIpRxChannel->m_SrcPort+FEC_INC_COLUMN_PORT==SrcPort ||
                                        pIpRxChannel->m_SrcPort+FEC_INC_ROW_PORT==SrcPort)
                {
                    if (pRtpHeader != NULL)
                    {
                        // If we have a RTP header, determine if this is a FEC packet
                        if (pRtpHeader->m_PayloadType==RTP_PAYLOAD_FEC && 
                                                                 pRtpHeader->m_Version==2)
                            IsFecPortSrc = TRUE;
                    } else{
                        // if we do not have a RTP header, assume that this is a FEC 
                        // packet
                        IsFecPortSrc = TRUE;
                    }
                }
            }
        }

        // Check if the source port number is correct
        if (pIpRxChannel->m_SrcPort!=0 && pIpRxChannel->m_SrcPort!=SrcPort && 
                                                                            !IsFecPortSrc)
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }

        // If a RTP header is available, we must check on its contents
        // to determine if we must add the packet to a FEC stream
        // Add the channel if we cannot use the contents of the packet, and
        // the destination port matches a FEC port.
        IsFecPortDst = FALSE;
        if (pIpRxChannel->m_FecMode!=DTA_FEC_DISABLE && 
                                               pIpRxChannel->m_DetProtocol==DTA_PROTO_RTP)
        {
            if (pIpRxChannel->m_DstPort+FEC_INC_COLUMN_PORT==DstPort ||
                                        pIpRxChannel->m_DstPort+FEC_INC_ROW_PORT==DstPort)
            {
                if (pRtpHeader != NULL)
                {
                    // If we have a RTP header, determine if this is a FEC packet
                    if(pRtpHeader->m_PayloadType==RTP_PAYLOAD_FEC && 
                                                                 pRtpHeader->m_Version==2)
                        IsFecPortDst = TRUE;                    
                } else{
                    // if we do not have a RTP header, assume that this is a FEC packet
                    IsFecPortDst = TRUE;
                }
            }
        }

        // If the destination port matches or if the portnumber it used for a FEC stream
        // add the channel.
        if (pIpRxChannel->m_DstPort==DstPort || IsFecPortDst)
        {
            pIpRxA[Index++] = pIpRxChannel;
            
            // Increment use count
            DtaIpRxUserChRefAdd(pIpRxChannel);
            
            if (Index == MaxNumListeners) 
            {
                DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
                return MaxNumListeners;
            }
        }
        pIpRxChannel = pIpRxChannel->m_pNext;
    }
    DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
    return Index;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetChannelsForDVB -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This functions will increment the channel's use count returned in pIpRxA
// Pre: m_IpRxListenersMutex mutex must be acquired
//
UInt  DtaIpRxGetChannelsForDVB(DtaIpPort* pIpPort, UInt32 IdTag, UInt8* pIpSrc, 
                            UInt16 SrcPort, UserIpRxChannel** pIpRxA, Int MaxNumListeners)
{
    Bool  SSMIpV6 = FALSE;
    DtaIpUserChannels*  pIpUserChannels = 
                                        &pIpPort->m_pDvcData->m_IpDevice.m_IpUserChannels;
    UserIpRxChannel*  pIpRxChannel;
    Int  Index = 0;
    UInt  FirstIpPortNum = pIpPort->m_IpPortIndex&0xfffffffe;
    DtaIpPort*  pFirstPort = &pIpPort->m_pDvcData->m_IpDevice.m_pIpPorts[FirstIpPortNum];
    Int  StreamType;
    
    // Wait until the Rx structure is available
    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);
    DtMutexAcquire(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex, -1);

    // Get the channel list for this IdTag
    pIpRxChannel = pIpPort->m_pDvcData->m_IpDevice.m_pAddrMatchLUTable[IdTag];
    StreamType = pIpPort->m_pDvcData->m_IpDevice.m_AddrMatchLUTableType[IdTag];
    
    if (pIpRxChannel != NULL)
        SSMIpV6 = 
            pIpRxChannel->m_AddrMatcherEntry[StreamType] \
                                              [pIpPort->m_IpPortIndex&1].m_Gen.m_SSM==1 &&
            pIpRxChannel->m_AddrMatcherEntry[StreamType] \
                                            [pIpPort->m_IpPortIndex&1].m_Gen.m_Version==1;
    
    while (pIpRxChannel != NULL) 
    {
        UInt8*  pIpAddr = NULL;
        Bool  StreamForThisChannel = TRUE;
            
        if ((pIpRxChannel->m_IpParsMode & DTA_IP_RX_2022_7) !=0 )
        {
            if (((pIpPort->m_IpPortIndex&1)==0) && pIpRxChannel->m_DoSSMCheckSw)
                pIpAddr = pIpRxChannel->m_SrcIPAddress;
            else if (((pIpPort->m_IpPortIndex&1)==1) && pIpRxChannel->m_DoSSMCheckSw2)
                pIpAddr = pIpRxChannel->m_SrcIPAddress2;
        } else if (pIpRxChannel->m_DoSSMCheckSw)
            pIpAddr = pIpRxChannel->m_SrcIPAddress;
        
        if (pIpAddr != NULL)
        {
            Bool  IpV6 = (pIpRxChannel->m_IpParsFlags & DTA_IP_V6) != 0;
            Int  i;
            // Check if source IP-address is identical
            for (i=0; i<(IpV6?16:4) && StreamForThisChannel; i++)
            {
                if (pIpAddr[i] != pIpSrc[i])
                    StreamForThisChannel = FALSE;
            }
        }
        if (StreamForThisChannel)
        {
            // DTA 2162 does not have source port filtering in hardware
            UInt16*  pSrcPort = NULL;
            if (((pIpPort->m_IpPortIndex&1)==0) && pIpRxChannel->m_SrcPort != 0)
                pSrcPort = &pIpRxChannel->m_SrcPort;
            else if (((pIpPort->m_IpPortIndex&1)==1) && pIpRxChannel->m_SrcPort2 != 0)
                pSrcPort = &pIpRxChannel->m_SrcPort2;
            if (pSrcPort!=NULL && *pSrcPort!=SrcPort)
                StreamForThisChannel = FALSE;
        }

        if (StreamForThisChannel)
        {
            pIpRxA[Index++] = pIpRxChannel;
            
            // Increment use count
            DtaIpRxUserChRefAdd(pIpRxChannel);
        
            if (Index == MaxNumListeners) 
            {
                DtMutexRelease(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex);
                DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
                return MaxNumListeners;
            }
        }

        if (SSMIpV6)
        {
           if (pIpRxChannel->m_pNextAddrMatcherEntryPart2[StreamType] \
                                                       [pIpPort->m_IpPortIndex&1] != NULL)
            {
                Int  StreamType2;
                StreamType2 = pIpRxChannel->m_pNextAddrMatcherEntryPart2[StreamType] \
                                                 [pIpPort->m_IpPortIndex&1]->m_StreamType;
                
                pIpRxChannel = DtContainingRecord(
                       pIpRxChannel->m_pNextAddrMatcherEntryPart2[StreamType] \
                       [pIpPort->m_IpPortIndex&1], UserIpRxChannel,
                       m_AddrMatcherEntrySSMPart2[StreamType][pIpPort->m_IpPortIndex&1]);
                StreamType = StreamType2;
            } else 
                pIpRxChannel = NULL;
        } else {
            if (pIpRxChannel->m_pNextAddrMatcherEntry[StreamType] \
                                                       [pIpPort->m_IpPortIndex&1] != NULL)
            {
                Int  StreamType2;
                StreamType2 = pIpRxChannel->m_pNextAddrMatcherEntry[StreamType] \
                                                 [pIpPort->m_IpPortIndex&1]->m_StreamType;
                pIpRxChannel = DtContainingRecord(
                               pIpRxChannel->m_pNextAddrMatcherEntry[StreamType] \
                               [pIpPort->m_IpPortIndex&1], UserIpRxChannel,
                               m_AddrMatcherEntry[StreamType][pIpPort->m_IpPortIndex&1]);
                StreamType = StreamType2;
            } else 
                pIpRxChannel = NULL;
        }
    }
    DtMutexRelease(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex);
    DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
    return Index;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxIsFragmentedPacketForDVB -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Checks if the received fragmented IP packet is for one of the DVB streams. If so,
// an array with the ChannelIndex'es and the number of channels found is returned
// Pre: m_IpRxListenersMutex mutex must be acquired
//
UInt  DtaIpRxIsFragmentedPacketForDVB(DtaIpUserChannels* pIpUserChannels,
                                            UInt16 FragmentID, UInt16 IpFragmentOffset, 
                                            UserIpRxChannel** pIpRxA, Int MaxNumListeners)
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

        pIpRxA[Index++] = pIpRxChannel;
            
        // Increment reference
        DtaIpRxUserChRefAdd(pIpRxChannel);

        if (Index == MaxNumListeners) 
        {
            DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
            return MaxNumListeners;
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
UserIpRxChannel*  DtaIpRxParseFragmentedFrame(DtaIpUserChannels* pIpUserChannels,
                 DtaDmaRxHeader* pDmaRxHeader, UInt32 Size, IpHeaderV4* pIpHeader,
                 Bool MulticastPacket, DtaIpPort* pIpPort, Int VlanId, Bool* pStopParsing)
{    
    UInt  i;
    UInt16  IpFragmentOffset = 0;
    UInt8*  pIpPacketData;
    Int  IpPacketDataSize;
    UdpHeader*  pUdpHeader;

    UInt16  DstPort;
    UInt16  SrcPort;
    UInt  NumChannels;
    UserIpRxChannel*  pIpRxChannel;
    UserIpRxChannel*  pIpRxFragment = NULL;

    Int  PacketSize = Size - 4 - sizeof(DtaDmaRxHeader);
    UInt8*  pRxData = (UInt8*)pDmaRxHeader + sizeof(DtaDmaRxHeader);
    Bool  Type2 = (pIpPort->m_PortType == DTA_IPPORT_TYPE2);


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
        SrcPort = DtUInt16ByteSwap(pUdpHeader->m_SourcePort);
        DstPort = DtUInt16ByteSwap(pUdpHeader->m_DestinationPort);
        DtMutexAcquire(&pIpPort->m_IpRxListenersMutex, -1);
        // For Type2, we already know it's for one or more of the RT Ip channels
        if (Type2)
            NumChannels = DtaIpRxGetChannelsForDVB(pIpPort, 
                                   pDmaRxHeader->m_RxHeaderV3.m_AddrMatching.m_AddrIdTag,
                                   pIpHeader->m_SrcAddress, SrcPort,
                                   pIpPort->m_pIpRxListeners, pIpPort->m_MaxNumListeners);
        else {
             // Check if it is for one of our channels
            pUdpHeader = (UdpHeader*)((UInt8*)pIpHeader +
                                                         (pIpHeader->m_HeaderLength * 4));
            DstPort = DtUInt16ByteSwap(pUdpHeader->m_DestinationPort);
            NumChannels = DtaIpRxIsPacketForDVB(pIpUserChannels, NULL,
                                         pIpHeader->m_SrcAddress, pIpHeader->m_DstAddress,
                                         SrcPort, DstPort, MulticastPacket, FALSE, VlanId,
                                         pIpPort->m_pIpRxListeners,
                                         pIpPort->m_MaxNumListeners);
        }
        if (NumChannels == 0)
        {
            DtMutexRelease(&pIpPort->m_IpRxListenersMutex);
            DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
            *pStopParsing = TRUE;
            return NULL;
        }       

        DtDbgOut(MAX, IP_RX, "Storing FIRST fragment: %d", 
                                           DtUInt16ByteSwap(pIpHeader->m_Identification));

        for (i=0; i<NumChannels; i++) 
        {
            pIpRxChannel =  pIpPort->m_pIpRxListeners[i];
            if (Size-4 <= (UInt)pIpRxChannel->m_MaxJumboPktSize)
            {
                pIpRxChannel->m_FragmentId = pIpHeader->m_Identification;
                // Store IP Header offset offset for later use
                pIpRxChannel->m_CurIpHeaderOffset = 
                                       (UInt16)((UInt8*)pIpHeader - (UInt8*)pDmaRxHeader);
            
                // Copy data including Dma/Eth/IP header into temp. buffer
                // excluding checksum
                DtMemCopy(pIpRxChannel->m_pJumboPktBuffer, pDmaRxHeader, Size - 4);
                pIpRxChannel->m_FragmentOffset = (UInt16)Size - 4;
            }
            
            // Decrement use count
            DtaIpRxUserChRefDecr(pIpRxChannel);
        }
        DtMutexRelease(&pIpPort->m_IpRxListenersMutex);
        *pStopParsing = TRUE;
        return NULL;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Second..Last-1  Fragment -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    // Not first or last fragmented packet. 
    // Copy only the data into the channel buffer
    // Check all channels to see if current fragment belongs to a particular channel 
    if ((pIpHeader->m_Flags & IPV4_HDR_FLAGS_MOREFRAG)!=0 && IpFragmentOffset!=0)
    {
        DtMutexAcquire(&pIpPort->m_IpRxListenersMutex, -1);
        NumChannels = DtaIpRxIsFragmentedPacketForDVB(pIpUserChannels, 
                                   pIpHeader->m_Identification, IpFragmentOffset,
                                   pIpPort->m_pIpRxListeners, pIpPort->m_MaxNumListeners);

        DtDbgOut(MAX, IP_RX, "NEXT fragment: %d Offset: %d",  
                                            DtUInt16ByteSwap(pIpHeader->m_Identification),
                                            IpFragmentOffset*8);

        if (NumChannels == 0) 
        {
            DtMutexRelease(&pIpPort->m_IpRxListenersMutex);
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
            pIpRxChannel = pIpPort->m_pIpRxListeners[i];

            // Check max. jumbo packet size
            if (IpPacketDataSize+pIpRxChannel->m_FragmentOffset > 
                                                          pIpRxChannel->m_MaxJumboPktSize)
            {
                // Size too big, skip packet;
                pIpRxChannel->m_FragmentId = 0;
                pIpRxChannel->m_FragmentOffset = 0;
            } else {
                // Copy data into temp. buffer
                DtMemCopy(pIpRxChannel->m_pJumboPktBuffer+pIpRxChannel->m_FragmentOffset, 
                                                         pIpPacketData, IpPacketDataSize);
                pIpRxChannel->m_FragmentOffset+=(UInt16)IpPacketDataSize;
            }

            // Decrement use count
            DtaIpRxUserChRefDecr(pIpRxChannel);
        }
        DtMutexRelease(&pIpPort->m_IpRxListenersMutex);
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
        DtMutexAcquire(&pIpPort->m_IpRxListenersMutex, -1);
        NumChannels = DtaIpRxIsFragmentedPacketForDVB(pIpUserChannels,
                                   pIpHeader->m_Identification, IpFragmentOffset,
                                   pIpPort->m_pIpRxListeners, pIpPort->m_MaxNumListeners);
        if (NumChannels == 0) 
        {
            DtMutexRelease(&pIpPort->m_IpRxListenersMutex);
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
            pIpRxChannel =  pIpPort->m_pIpRxListeners[i];
            pIpHeader = (IpHeaderV4*)(pIpRxChannel->m_pJumboPktBuffer + 
                                                    pIpRxChannel->m_CurIpHeaderOffset);

            pUdpHeader = (UdpHeader*)((UInt8*)pIpHeader + 
                                                         (pIpHeader->m_HeaderLength * 4));
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
            DtMemCopy(pIpRxChannel->m_pJumboPktBuffer + pIpRxChannel->m_FragmentOffset, 
                                                                     pIpPacketData, Size);
            pIpRxChannel->m_FragmentOffset+=(UInt16)IpPacketDataSize;
            pDmaRxHeader = (DtaDmaRxHeader*)(pIpRxChannel->m_pJumboPktBuffer);
            pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength = 
                                                           pIpRxChannel->m_FragmentOffset;
            // Output the found channel that needs to be processed
            pIpRxFragment = pIpRxChannel;
            break;
        }

        // Decrement use count, except for the fragmented packet channel
        for (i=0; i<NumChannels; i++) 
        {
            pIpRxChannel =  pIpPort->m_pIpRxListeners[i];
            if (pIpRxChannel != pIpRxFragment)
                DtaIpRxUserChRefDecr(pIpRxChannel);
        }
        DtMutexRelease(&pIpPort->m_IpRxListenersMutex);

        // No complete packet found
        // skip all and return
        if (pIpRxFragment == NULL)
            return NULL;
    }

    // At this point our fragmented frame should be complete.
    // or the packet is not fragmented
    return pIpRxFragment;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIpRxProcessDvbPayLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
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

#ifdef _DEBUG
static int PrevSeqNr = -1;
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxProcessSdiPayLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxProcessSdiPayLoad(
    UserIpRxChannel*  pIpRxChannel, // IP Rx channel
    RtpHeader*  pRtpHeader,         // RTP header
    UInt8*  pPayLoad,               // IP payload with packet data
    Int  PayLoadSize,               // Size of payload in #bytes
    UInt64  TimeStamp)              // Time stamp @54MHz
{
    UInt  NumFree;                  // Number of free bytes in buffer
    Int  SyncErr;                   // Synchronisation Error
    UInt8*  pRead;                  // Read pointer
    UInt8*  pWrite;                 // Write pointer
    UInt8*  pWrapArea;              // Start of the wrap area
    Bool  SkipData = FALSE;
    
    HbrMediaPlHeader*  pHbrHeader = (HbrMediaPlHeader*)((UInt8*)
                                                            pRtpHeader+sizeof(RtpHeader));

    // Update last Rx timestamp for bitrate calculations
    pIpRxChannel->m_BrmLastRxTimeStamp = TimeStamp;
    pIpRxChannel->m_BrmNumPckBytesRcvd+= 11000/8;   // Just an estimation for both SDI
                                                    // standards

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
    
    if (pIpRxChannel->m_SdiNumBytesCopied == -1)
    {
        Int  ExtraSize = 0;
        pIpRxChannel->m_SdiNumLeftOverBytes = 0;
        if (pIpRxChannel->m_DetVidStd == DT_VIDSTD_525I59_94)
        {
            pIpRxChannel->m_SdiFrameSize = 900900*10/8;
            pIpRxChannel->m_SdiFrameSize += 3; // Padding bytes
            pIpRxChannel->m_DetNumTpPerIp = 900900*10/8/SMPTE_2022_6_PAYLOAD_SIZE;
        } else if (pIpRxChannel->m_DetVidStd == DT_VIDSTD_625I50)
        {
            pIpRxChannel->m_SdiFrameSize = 1080000*10/8;
            pIpRxChannel->m_DetNumTpPerIp = 1080000*10/8/SMPTE_2022_6_PAYLOAD_SIZE;
        } else 
        {
            DtDbgOut(ERR, IP_RX, "Video standard not detected");
            pIpRxChannel->m_DetNumTpPerIp = 0;
            return;
        }
        
        //DtDbgOut(ERR, IP_RX, "Start new frame. FrameCounter: %i", pHbrHeader->m_FRCount);

        pIpRxChannel->m_pSdiInternalWritePointer = pWrite;
        pIpRxChannel->m_SdiCurFrameNumber = pHbrHeader->m_FRCount;
        pIpRxChannel->m_SdiNumBytesCopied = 0;
        
        if ((pIpRxChannel->m_RxMode & DT_RX_TIMESTAMP) != 0) 
            ExtraSize += 4;

        if ((pIpRxChannel->m_RxMode & DT_RXMODE_SDI_STAT) != 0) 
            ExtraSize += sizeof(SdiRxFrameStat);

        // Check for overflow
        // +5 for 10->8 bit overhead during copy
        if (NumFree < (UInt)(pIpRxChannel->m_SdiFrameSize+ExtraSize+5)) 
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
        // Start with writing time stamp, if specified to do so
        if ((pIpRxChannel->m_RxMode & DT_RX_TIMESTAMP) != 0) 
        {
            *(UInt32*)pIpRxChannel->m_pSdiInternalWritePointer = (UInt32)TimeStamp;
            pIpRxChannel->m_pSdiInternalWritePointer += 4;
        }
        // Add stat header, if specified to do so
        if ((pIpRxChannel->m_RxMode & DT_RXMODE_SDI_STAT) != 0) 
        {
            pIpRxChannel->m_pSdiRxFrameStat = 
                                (SdiRxFrameStat*)pIpRxChannel->m_pSdiInternalWritePointer;
            pIpRxChannel->m_pSdiInternalWritePointer += sizeof(SdiRxFrameStat);
            pIpRxChannel->m_pSdiRxFrameStat->m_FrameCount = (UInt32)pHbrHeader->m_FRCount;
            pIpRxChannel->m_pSdiRxFrameStat->m_Timestamp = 
                                                DtUInt32ByteSwap(pRtpHeader->m_TimeStamp);
            pIpRxChannel->m_pSdiRxFrameStat->m_MaxIpat = 0;
            pIpRxChannel->m_pSdiRxFrameStat->m_MinIpat = 0xffffffff;
            pIpRxChannel->m_pSdiRxFrameStat->m_Spare1 = 0;
            pIpRxChannel->m_pSdiRxFrameStat->m_Spare2 = 0;
            pIpRxChannel->m_SdiFrameLastTimestamp = TimeStamp;
        }
    }

    DtDbgOut(MAX, IP_RX, "Channel %d: NumFree=%d pRead=0x%p pWrite=0x%p", 
                       pIpRxChannel->m_ChannelIndex, NumFree, pRead, pWrite);

    // No overflow
    pIpRxChannel->m_Flags &= ~DTA_RX_FIFO_OVF;
    SyncErr = 0;
    DtDbgOut(MAX, IP_RX, "TimeStamp:%08x RTP SeqNr: %i FrameCounter: %i Marker: %i"
                    " PayloadSize:%i", DtUInt32ByteSwap(pRtpHeader->m_TimeStamp), 
                    DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber), pHbrHeader->m_FRCount, 
                    pRtpHeader->m_Marker, PayLoadSize);
#ifdef _DEBUG    
    if (PrevSeqNr != -1)
    {
        if (((UInt16)(PrevSeqNr+1)) != DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber))
             DtDbgOut(MIN, IP_RX_REC, "Seq. out of sync. Prev. Seq: %i TimeStamp:%08x RTP" 
             " SeqNr: %i FrameCounter: %i", PrevSeqNr, 
             DtUInt32ByteSwap(pRtpHeader->m_TimeStamp), 
             DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber), pHbrHeader->m_FRCount);
    }
    PrevSeqNr = DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber);
#endif

    // Check if we are still in sync

    // Frame counter as expected?
    if (pIpRxChannel->m_SdiCurFrameNumber != pHbrHeader->m_FRCount)
    {
        SyncErr = 1;
        DtDbgOut(ERR, IP_RX, "SYNC ERROR. Expected frame counter %i, Received: %i"
                                     " (RTP seq.: %i)", pIpRxChannel->m_SdiCurFrameNumber, 
                                     pHbrHeader->m_FRCount, pRtpHeader->m_SequenceNumber);
    }
    // Not end of frame marker, but already too much data?
    if (pRtpHeader->m_Marker!=1 && (pIpRxChannel->m_SdiNumBytesCopied + PayLoadSize >=
                                                            pIpRxChannel->m_SdiFrameSize))
    {
        SyncErr = 1;
        DtDbgOut(ERR, IP_RX, "SYNC ERROR. NumBytesCopied:%i. FrameSize:%i PayloadSize:%i",
            pIpRxChannel->m_SdiNumBytesCopied, pIpRxChannel->m_SdiFrameSize, PayLoadSize);
    }
    // End of frame marker, but not enough data?
    if (pRtpHeader->m_Marker==1 && (pIpRxChannel->m_SdiNumBytesCopied + PayLoadSize <
                                                            pIpRxChannel->m_SdiFrameSize))
    {
        SyncErr = 1;
        DtDbgOut(ERR, IP_RX, "SYNC ERROR. Not enough bytes for last packet."
            " NumBytesCopied:%i. FrameSize:%i PayloadSize:%i",
            pIpRxChannel->m_SdiNumBytesCopied, pIpRxChannel->m_SdiFrameSize, PayLoadSize);
        
        // Increase frame number for next packet
        pHbrHeader->m_FRCount++;
        SkipData = TRUE;
    }

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

        // Skip current IP packets and restart with new frame
        pIpRxChannel->m_pSdiInternalWritePointer = pWrite;
        pIpRxChannel->m_SdiCurFrameNumber = pHbrHeader->m_FRCount;
        pIpRxChannel->m_SdiNumBytesCopied = 0;
        pIpRxChannel->m_SdiNumLeftOverBytes = 0;
        SyncErr = 0;
        // Start with writing time stamp, if specified to do so
        if ((pIpRxChannel->m_RxMode & DT_RX_TIMESTAMP) != 0) 
        {
            *(UInt32*)pIpRxChannel->m_pSdiInternalWritePointer = (UInt32)TimeStamp;
            pIpRxChannel->m_pSdiInternalWritePointer += 4;
        }
        // Add stat header, if specified to do so
        if ((pIpRxChannel->m_RxMode & DT_RXMODE_SDI_STAT) != 0)
        {
            pIpRxChannel->m_pSdiRxFrameStat = 
                                (SdiRxFrameStat*)pIpRxChannel->m_pSdiInternalWritePointer;
            pIpRxChannel->m_pSdiInternalWritePointer += sizeof(SdiRxFrameStat);
            pIpRxChannel->m_pSdiRxFrameStat->m_FrameCount = (UInt32)pHbrHeader->m_FRCount;
            pIpRxChannel->m_pSdiRxFrameStat->m_Timestamp = 
                                                DtUInt32ByteSwap(pRtpHeader->m_TimeStamp);
            pIpRxChannel->m_pSdiRxFrameStat->m_MaxIpat = 0;
            pIpRxChannel->m_pSdiRxFrameStat->m_MinIpat = 0xffffffff;
            pIpRxChannel->m_pSdiRxFrameStat->m_Spare1 = 0;
            pIpRxChannel->m_pSdiRxFrameStat->m_Spare2 = 0;
            pIpRxChannel->m_SdiFrameLastTimestamp = TimeStamp;
        }
    } else
        pIpRxChannel->m_Flags &= ~DTA_RX_SYNC_ERR;

    if (SkipData)
        return;

    if ((pIpRxChannel->m_RxMode & DT_RXMODE_SDI_STAT)!=0 &&
                                                     pIpRxChannel->m_SdiNumBytesCopied!=0)
    {
        // Calculate IPAT for this frame
        if ((TimeStamp - pIpRxChannel->m_SdiFrameLastTimestamp) < 
                                               pIpRxChannel->m_pSdiRxFrameStat->m_MinIpat)
            pIpRxChannel->m_pSdiRxFrameStat->m_MinIpat = (UInt32)
                                      (TimeStamp - pIpRxChannel->m_SdiFrameLastTimestamp);
        if ((TimeStamp - pIpRxChannel->m_SdiFrameLastTimestamp) > 
                                               pIpRxChannel->m_pSdiRxFrameStat->m_MaxIpat)
            pIpRxChannel->m_pSdiRxFrameStat->m_MaxIpat = (UInt32)
                                      (TimeStamp - pIpRxChannel->m_SdiFrameLastTimestamp);
        pIpRxChannel->m_SdiFrameLastTimestamp = TimeStamp;
    }

    pWrite = pIpRxChannel->m_pSdiInternalWritePointer;
    if (pIpRxChannel->m_SdiNumLeftOverBytes != 0)
    {
        // Copy the leftover bytes before current payload. We overwrite the headers, 
        // but that's not a problem. We don't need it anymore.
        pPayLoad-= pIpRxChannel->m_SdiNumLeftOverBytes;
        DtMemCopy(pPayLoad, pIpRxChannel->m_SdiLeftOverBytes, 
                                                     pIpRxChannel->m_SdiNumLeftOverBytes);
        PayLoadSize+= pIpRxChannel->m_SdiNumLeftOverBytes;
        pIpRxChannel->m_SdiNumLeftOverBytes = 0;
    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Write Packet to Buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // The code below works because the buffer is made 256 bytes longer than the size
    // that is actually used. This way, packets that would wrap the buffer are written
    // partly in the extra 256 bytes. The overflow bytes are then copied back to the
    // beginning of the buffer.
    //
    while (PayLoadSize>=5 && 
                           pIpRxChannel->m_SdiNumBytesCopied<pIpRxChannel->m_SdiFrameSize)
    {
        // Convert payload from BigEndian(network byte order) to LittleEndian
        *pWrite++ = (pPayLoad[0]<<2) | (pPayLoad[1]>>6);
        *pWrite++ = (pPayLoad[0]>>6) | (pPayLoad[1]<<6) | ((pPayLoad[2]&0xf0)>>2);
        *pWrite++ = ((pPayLoad[1]>>2)&0xf) | ((pPayLoad[3]<<2)&0xf0);
        *pWrite++ = ((pPayLoad[2]&0xf)<<2) | (pPayLoad[3]>>6) | (pPayLoad[4]<<6);
        *pWrite++ = (pPayLoad[3]<<6) | (pPayLoad[4]>>2);
        pPayLoad+=5;
        pIpRxChannel->m_SdiNumBytesCopied+=5;
        PayLoadSize-=5;

        // Did we reach the end of the buffer?
        if (pWrite >= pWrapArea)
        {
            if (pWrite != pWrapArea)
                DtMemCopy(pIpRxChannel->m_pFifo, pWrapArea, (UInt)(pWrite-pWrapArea));
            pWrite = pIpRxChannel->m_pFifo + (UInt)(pWrite-pWrapArea);
        }
    }
    if (PayLoadSize !=0 && pIpRxChannel->m_SdiNumBytesCopied<pIpRxChannel->m_SdiFrameSize)
    {
        // We have leftover bytes.
        // Copy to temp channel buffer
        pIpRxChannel->m_SdiNumLeftOverBytes = PayLoadSize;
        DtMemCopy(pIpRxChannel->m_SdiLeftOverBytes, pPayLoad,
                                                     pIpRxChannel->m_SdiNumLeftOverBytes);
    } else
        pIpRxChannel->m_SdiNumLeftOverBytes = 0;

    pIpRxChannel->m_pSdiInternalWritePointer = pWrite;

    if (pIpRxChannel->m_SdiNumBytesCopied >= pIpRxChannel->m_SdiFrameSize)
    {
        // We are finished, update status
        Int  ToMuchCopied = pIpRxChannel->m_SdiNumBytesCopied -
                                                             pIpRxChannel->m_SdiFrameSize;
        pIpRxChannel->m_SdiNumBytesCopied = -1;
        
        if (ToMuchCopied == 0)
        {    pIpRxChannel->m_pBufferHeader->m_FifoWriteOffset = (UInt)
                                                           (pWrite-pIpRxChannel->m_pFifo);
        } else if ((pWrite-ToMuchCopied)<pIpRxChannel->m_pFifo)
        {
            ToMuchCopied -= (Int)(pWrite - pIpRxChannel->m_pFifo);
            pIpRxChannel->m_pBufferHeader->m_FifoWriteOffset = 
                                   (UInt)(pWrapArea-pIpRxChannel->m_pFifo) - ToMuchCopied;
        } else {
            pIpRxChannel->m_pBufferHeader->m_FifoWriteOffset = (UInt)
                                            (pWrite-pIpRxChannel->m_pFifo) - ToMuchCopied;
        }
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIpRxProcessIpRawPayLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
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
    DtSpinLockAcquire(&pIpRxChannel->m_RtpListSpinLock);
    DtaIpRxRtpListsResetUnsafe(pIpRxChannel);
    DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpListsReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Move all RTP packets from m_RtpDvbList, m_RtpFecColumnList and m_RtpFecRowList
// to the m_RtpEmptyList list.
//
void  DtaIpRxRtpListsResetUnsafe(UserIpRxChannel* pIpRxChannel)
{
    DtListEntry*  pListEntry;

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

    if (pIpRxChannel->m_pRtpAvailLookup1 != NULL)
        DtMemZero(pIpRxChannel->m_pRtpAvailLookup1, DTA_IPRX_RTP_LOOKUPTABLE_SIZE*4);
    if (pIpRxChannel->m_pRtpAvailLookup2 != NULL)
        DtMemZero(pIpRxChannel->m_pRtpAvailLookup2, DTA_IPRX_RTP_LOOKUPTABLE_SIZE*4);
    pIpRxChannel->m_RtpFirstPacket = TRUE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpListsInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// returns Start of RTP buffer
//
UInt8*  DtaIpRxRtpListsInit(UserIpRxChannel* pIpRxChannel, UInt RtpBufSize)
{
    RtpListEntry*  pRtpList = NULL;
    UInt  i;
    UInt  NumEntries;

    DtListHeadInitialize(&pIpRxChannel->m_RtpEmptyList);
    DtListHeadInitialize(&pIpRxChannel->m_RtpDvbList);
    DtListHeadInitialize(&pIpRxChannel->m_RtpFecColumnList);
    DtListHeadInitialize(&pIpRxChannel->m_RtpFecRowList);

    DT_ASSERT(pIpRxChannel->m_pRtpListEntries != NULL);

    pRtpList = (RtpListEntry*)pIpRxChannel->m_pRtpListEntries;

    // Calculate NumEntries
    NumEntries = (UInt)DtDivide64(RtpBufSize, (DTA_IPRX_MAX_PACKET_LENGTH + 
                                                             sizeof(RtpListEntry)), NULL);

    // Add all entries to the empty list
    for (i=0; i<NumEntries; i++) 
    {
        pRtpList->m_BufIndex = i;
        pRtpList->m_InUse = 0;
        DtTailListInsert(&pIpRxChannel->m_RtpEmptyList, &pRtpList->m_ListEntry);
        pRtpList+=1;
    }
    if (pIpRxChannel->m_pRtpAvailLookup1 != NULL)
        DtMemZero(pIpRxChannel->m_pRtpAvailLookup1, DTA_IPRX_RTP_LOOKUPTABLE_SIZE*4);
    if (pIpRxChannel->m_pRtpAvailLookup2 != NULL)
        DtMemZero(pIpRxChannel->m_pRtpAvailLookup2, DTA_IPRX_RTP_LOOKUPTABLE_SIZE*4);
        
    return (UInt8*)pRtpList;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpGetFreeEntry -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns a new RtpListEntry from the m_RtpEmptyList list
// If no entries are available NULL is returned
//
RtpListEntry*  DtaIpRxRtpGetFreeEntry(UserIpRxChannel* pIpRxChannel)
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
void  DtaIpRxRtpMoveEntryToEmptyListUnsafe(UserIpRxChannel* pIpRxChannel, 
                                                                  RtpListEntry* pRtpEntry)
{
    DtTailListInsert(&pIpRxChannel->m_RtpEmptyList, &pRtpEntry->m_ListEntry);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpMoveEntryToEmptyList -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Put a RtpListEntry to the m_RtpEmptyList list.
void  DtaIpRxRtpMoveEntryToEmptyList(UserIpRxChannel* pIpRxChannel, 
                                                                  RtpListEntry* pRtpEntry)
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
Bool  DtaIpRxRtpIsSeqNumLess(UInt16 SeqNum1, UInt16 SeqNum2)
{
    if (SeqNum2 > SeqNum1)
        if (SeqNum2-SeqNum1 < 0x1FFF) return TRUE; else return FALSE;
    else if (SeqNum1 > SeqNum2) 
    {   
        if (SeqNum1-SeqNum2 < 0x7FFF) return FALSE;
        if (0xffff-SeqNum1+SeqNum2 < 0x1FFF) return TRUE; else return FALSE;
    }
    return FALSE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpIsTimestampLess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Timestamp max. 32-bit
// return TRUE: Timestamp1 < Timestamp2
//        FALSE: Timestamp1 >= Timestamp2
//
Bool  DtaIpRxRtpIsTimestampLess(UInt32 Timestamp1, UInt32 Timestamp2)
{
    if (Timestamp2 > Timestamp1)
        if (Timestamp2-Timestamp1 < 0xFFFFFFF) return TRUE; else return FALSE;
    else if (Timestamp1 > Timestamp2) 
    {   
        if (Timestamp1-Timestamp2 < 0x7FFFFFFF) return FALSE;
        if (0xFFFFFFFF-Timestamp1+Timestamp2 < 0xFFFFFFF) return TRUE; else return FALSE;
    }
    return FALSE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpGetTimestampDelta -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Only works if DtaIpRxRtpIsTimestampLess = FALSE
//
UInt32 DtaIpRxRtpGetTimestampDelta(UInt32 Timestamp1, UInt32 Timestamp2)
{
    if (Timestamp1 > Timestamp2)
        return (0xFFFFFFFF-Timestamp1+Timestamp2);
    return Timestamp2 - Timestamp1;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpIsSeqNumLess2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaIpRxRtpIsSeqNumLess2(UInt16 SeqNum1, UInt32 TimeStamp1,
                                                        UInt16 SeqNum2, UInt32 TimeStamp2)
{
    if (TimeStamp1 == TimeStamp2)
    {
        if (SeqNum2 > SeqNum1)
            if (SeqNum2-SeqNum1 < 0x1FFF) return TRUE; else return FALSE;
        else if (SeqNum1 > SeqNum2) 
        {   
            if (SeqNum1-SeqNum2 < 0x7FFF) return FALSE;
            if (0xFFFF-SeqNum1+SeqNum2 < 0x1FFF) return TRUE; else return FALSE;
        }
        return FALSE;
    }

    if (TimeStamp2 > TimeStamp1)
        if (TimeStamp2-TimeStamp1 < 0x1FFFFFFF) return TRUE; else return FALSE;
    else if (TimeStamp1 > TimeStamp2) 
    {   
        if (TimeStamp1-TimeStamp2 < 0x7FFFFFFF) return FALSE;
        if (0xFFFFFFFF-TimeStamp1+TimeStamp2 < 0x1FFFFFFF) return TRUE; else return FALSE;
    }
    return FALSE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpGetDvbEntryUnsafe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns a RtpListEntry for a given SequenceNumber, or NULL if the entry does not exist
// If pLastUsedItem <> NULL, the search starts at pLastUsedItem
// Pre: The m_RtpListSpinLock must be acquired 
// 
RtpListEntry*  DtaIpRxRtpGetDvbEntryUnsafe(UserIpRxChannel* pIpRxChannel, 
                      UInt16 SequenceNumber, UInt32 Timestamp, DtListEntry* pLastUsedItem)
{
    Bool  Found;
    DtListEntry*  pEntry;
    RtpListEntry*  pRtpQueuedEntry = NULL;

    if (pIpRxChannel->m_pRtpAvailLookup1!=NULL && pIpRxChannel->m_pRtpAvailLookup2!=NULL)
    {
        if (pIpRxChannel->m_pRtpAvailLookup1[SequenceNumber]==0 && 
                                      pIpRxChannel->m_pRtpAvailLookup2[SequenceNumber]==0)
            return NULL;
    }
    
    Found = FALSE;
    if (pLastUsedItem != NULL)
        pEntry = DtListNextGet(pLastUsedItem);
    else
        pEntry = DtListNextGet(&pIpRxChannel->m_RtpDvbList);

    while (pEntry!=&pIpRxChannel->m_RtpDvbList && !Found) 
    {
        pRtpQueuedEntry = DtContainingRecord(pEntry, RtpListEntry, m_ListEntry);
        if ((SequenceNumber == pRtpQueuedEntry->m_RtpSequenceNumber) && 
                    DtaIpRxRtpIsTimestampLess(pRtpQueuedEntry->m_RtpTimestamp, Timestamp))
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
// Sort the RTP packet. If the packet with the same sequence number already exists,
// FALSE is returned and insertion is skipped
//
Bool  DtaIpRxRtpListAddEntryUnsafe(DtListEntry* pRtpList, RtpListEntry* pRtpEntry)
{
    Bool  Found = FALSE;
    Bool  Exist = FALSE;
    Bool  Last = FALSE;
    DtListEntry*  pEntry;
    RtpListEntry*  pRtpQueuedEntry = NULL;

    // First check if the element must be inserted after the last one. This is the most
    // common situation and then we don't need to walk through the entire list.
    pEntry = DtListPrevGet(pRtpList);
    if (pEntry != pRtpList) 
    {
        pRtpQueuedEntry = DtContainingRecord(pEntry, RtpListEntry, m_ListEntry);
        if (pRtpEntry->m_RtpSequenceNumber==pRtpQueuedEntry->m_RtpSequenceNumber &&
                               pRtpEntry->m_RtpTimestamp==pRtpQueuedEntry->m_RtpTimestamp)
        {
            Found = TRUE;
            Exist = TRUE;
            DtDbgOut(ERR, IP_RX, "EXIST: SeqNumNew:%i TimeStampNew:%x QueuedSeqNum:%i" 
                          " QueuedTimeStamp:%x", pRtpEntry->m_RtpSequenceNumber, 
                          pRtpEntry->m_RtpTimestamp, pRtpQueuedEntry->m_RtpSequenceNumber,
                           pRtpQueuedEntry->m_RtpTimestamp);

            
        } else if (DtaIpRxRtpIsSeqNumLess2(pRtpQueuedEntry->m_RtpSequenceNumber, 
                          pRtpQueuedEntry->m_RtpTimestamp, pRtpEntry->m_RtpSequenceNumber,
                          pRtpEntry->m_RtpTimestamp))
            Last = TRUE;
        else {
            DtDbgOut(ERR, IP_RX, "Why NOT LAST??: SeqNumNew:%i TimeStampNew:%x "
                     "QueuedSeqNum:%i QueuedTimeStamp:%x", pRtpEntry->m_RtpSequenceNumber,
                     pRtpEntry->m_RtpTimestamp, pRtpQueuedEntry->m_RtpSequenceNumber, 
                     pRtpQueuedEntry->m_RtpTimestamp);

            pRtpQueuedEntry = NULL;
        }
        
    }
    if (!Last)
        pEntry = DtListNextGet(pRtpList);

    // Walk through the list and insert element into the list sorted
    while (pEntry!=pRtpList && !Found && !Last) 
    {
        pRtpQueuedEntry = DtContainingRecord(pEntry, RtpListEntry, m_ListEntry);
        if (pRtpEntry->m_RtpSequenceNumber==pRtpQueuedEntry->m_RtpSequenceNumber &&
                               pRtpEntry->m_RtpTimestamp==pRtpQueuedEntry->m_RtpTimestamp)
        {
            Found = TRUE;
            Exist = TRUE;
        } else {
            if (DtaIpRxRtpIsSeqNumLess2(pRtpEntry->m_RtpSequenceNumber, 
                          pRtpEntry->m_RtpTimestamp, pRtpQueuedEntry->m_RtpSequenceNumber,
                          pRtpQueuedEntry->m_RtpTimestamp))
                Found = TRUE;
            else 
                pEntry = DtListNextGet(pEntry);
        }
    }

    if (!Exist)
    {
        if (!Found)
        {
            // Add element at the end
            DtTailListInsert(pRtpList, &pRtpEntry->m_ListEntry);
            if (pRtpQueuedEntry != NULL)
                DtDbgOut(MAX, IP_RX, "Add at end: SnNew: %u SnEntry:%u NewEntry:%p"
                         " pEntry:%p pRtpList:%p Last:%i", pRtpEntry->m_RtpSequenceNumber,
                         pRtpQueuedEntry->m_RtpSequenceNumber, pRtpEntry, pEntry, 
                         pRtpList, Last?1:0);
            else
                DtDbgOut(MAX, IP_RX, "Add at end: SnNew: %u NewEntry:%p pEntry:%p "
                          "FIRST ONE pRtpList:%p Last:%i", pRtpEntry->m_RtpSequenceNumber,
                          pRtpEntry, pEntry, pRtpList, Last?1:0);
        }
        else {
            // Insert element before entry
            DtTailListInsert(pEntry, &pRtpEntry->m_ListEntry);
            DtDbgOut(MAX, IP_RX, "Add before: SnNew: %u SnEntry:%u NewEntry:%p pEntry:%p"
                       " pRtpList:%p", pRtpEntry->m_RtpSequenceNumber,
                       pRtpQueuedEntry->m_RtpSequenceNumber, pRtpEntry, pEntry, pRtpList);
        }
    }
    return !Exist;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtpListAddEntry -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaIpRxRtpListAddEntry(UserIpRxChannel* pIpRxChannel, DtListEntry* pRtpList,
                                                                  RtpListEntry* pRtpEntry)
{
    Bool  Added;

    DtSpinLockAcquire(&pIpRxChannel->m_RtpListSpinLock);
    
    // Check if entry is too late and user buffer is already further in time.
    if (pRtpList==&pIpRxChannel->m_RtpDvbList && 
        ((pRtpEntry->m_RtpSequenceNumber==pIpRxChannel->m_RtpLastSeqNum&&
        pRtpEntry->m_RtpTimestamp==pIpRxChannel->m_RtpLastTimestamp) ||
        (DtaIpRxRtpIsSeqNumLess2(pRtpEntry->m_RtpSequenceNumber, pRtpEntry->m_RtpTimestamp,
        pIpRxChannel->m_RtpLastSeqNum, pIpRxChannel->m_RtpLastTimestamp) && 
        !pIpRxChannel->m_RtpFirstPacket)))
    {
        Added = FALSE;
        DtDbgOut(ERR, IP_RX, "Entry too late or already in buffer: SeqNumNew:%i "
                         "TimeStampNew:%x LastSeqNum:%i LastTimeStamp:%x", 
                         pRtpEntry->m_RtpSequenceNumber, pRtpEntry->m_RtpTimestamp, 
                         pIpRxChannel->m_RtpLastSeqNum, pIpRxChannel->m_RtpLastTimestamp);
    }
    else
        Added = DtaIpRxRtpListAddEntryUnsafe(pRtpList, pRtpEntry);
    if (Added && pIpRxChannel->m_pRtpAvailLookup1!=NULL && 
          pIpRxChannel->m_pRtpAvailLookup2!=NULL && pRtpList==&pIpRxChannel->m_RtpDvbList)
    {
        if (pIpRxChannel->m_pRtpAvailLookup1[pRtpEntry->m_RtpSequenceNumber] == 0)
            pIpRxChannel->m_pRtpAvailLookup1[pRtpEntry->m_RtpSequenceNumber] = 
                                                                pRtpEntry->m_RtpTimestamp;
        else if (pIpRxChannel->m_pRtpAvailLookup2[pRtpEntry->m_RtpSequenceNumber] == 0)
            pIpRxChannel->m_pRtpAvailLookup2[pRtpEntry->m_RtpSequenceNumber] = 
                                                                pRtpEntry->m_RtpTimestamp;
        else
            DT_ASSERT(FALSE);
    }
    DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);
    return Added;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIpRxCalculateChecksum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Calculates UDP checksum.
//
UInt16 DtaIpRxCalculateUdpChecksum(IpHeaderV4* pIpHeader, UdpHeader* pUdpHeader,
                                                     UInt8* pRxData, UInt32 PayloadLength)
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxResetStatistics -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxResetStatistics(UserIpRxChannel* pIpRxChannel)
{
    int  i;
    // Reset elements
    pIpRxChannel->m_NumIpatEl[0] = 0;
    pIpRxChannel->m_NumIpatEl[1] = 0;
    pIpRxChannel->m_CurIpatEl[0] = 0;
    pIpRxChannel->m_CurIpatEl[1] = 0;
    pIpRxChannel->m_CurSkewEl = 0;
    pIpRxChannel->m_NumSkewEl = 0;

    
    // Reset status port 1 and 2
    for (i=0; i<2; i++)
    {
        pIpRxChannel->m_LastSeqNumStat[i] = -1;
        pIpRxChannel->m_NumSeqNumStat[i] = 0;
        pIpRxChannel->m_MinIpatSecond[i] = -1;
        pIpRxChannel->m_MaxIpatSecond[i] = 0;
        pIpRxChannel->m_MinDelayFSecond[i] = 0;
        pIpRxChannel->m_MaxDelayFSecond[i] = 0;
        pIpRxChannel->m_BerNumIpPacketsLostMainSecond[i] = 0;
        pIpRxChannel->m_BerNumIpPacketsLostSecond[i] = 0;
        pIpRxChannel->m_BerNumIpPacketsMainSecond[i] = 0;
        pIpRxChannel->m_BerNumIpPacketsSecond[i] = 0;

        pIpRxChannel->m_MinIpatMinute[i] = -1;
        pIpRxChannel->m_MaxIpatMinute[i] = -1;
        pIpRxChannel->m_MinDelayFMinute[i] = 0;
        pIpRxChannel->m_MaxDelayFMinute[i] = 0;
        pIpRxChannel->m_BerNumIpPacketsLostMainMinute[i] = 0;
        pIpRxChannel->m_BerNumIpPacketsLostMinute[i] = 0;
        pIpRxChannel->m_BerNumIpPacketsMainMinute[i] = 0;
        pIpRxChannel->m_BerNumIpPacketsMinute[i] = 0;
    }
    pIpRxChannel->m_MinSkewSecond = 0;
    pIpRxChannel->m_MaxSkewSecond = 0;
    pIpRxChannel->m_MinSkewMinute = 0;
    pIpRxChannel->m_MaxSkewMinute = 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxCalcDiffValue32 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtaIpRxCalcDiffValue32(UInt StartValue,  UInt CurValue)
{
    if (StartValue > CurValue)  // Wrapped
    {
        UInt64  Value = (UInt64)0xffffffff + CurValue - StartValue;
        return (UInt)Value;
    }
    return (CurValue - StartValue);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxDoIpStatistics -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxDoIpStatistics(
    DtaIpPort*  pIpPort,
    UserIpRxChannel*  pIpRxChannel,
    UInt64  CurRefTimestamp,
    DtaDmaRxHeader*  pDmaRxHeader,
    UInt16  SeqNumber,
    UInt32  RtpTimestamp)                  // 0 if no RTP packet
{
    IpRxIpat*  pIpat;
    IpRxSkew*  pSkew;
    Bool  Recalc = FALSE;
    Int  IpPortIndex = pIpPort->m_IpPortIndex%2;
    Int  IpPortIndex2;
    UInt  NumPackets;

    if (IpPortIndex == 0)
        IpPortIndex2 = 1;
    else
        IpPortIndex2 = 0;
    
    if (pIpRxChannel->m_IpParsMode != DTA_IP_RX_2022_7)
        IpPortIndex = 0;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ipat / DelayFactor -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    pIpat =  &pIpRxChannel->m_Ipat[IpPortIndex][pIpRxChannel->m_CurIpatEl[IpPortIndex]];
    
    if (pIpRxChannel->m_NumIpatEl[IpPortIndex] == 0)
    {
        pIpRxChannel->m_NumIpatEl[IpPortIndex] = 1;
        pIpat->m_StartTime = CurRefTimestamp;
        
        pIpat->m_FirstSampleTime = pDmaRxHeader->m_Timestamp;
        pIpat->m_LastSampleTime = pDmaRxHeader->m_Timestamp;
        pIpat->m_MinIpat = -1;
        pIpat->m_MaxIpat = -1;
        pIpat->m_MinDelayF = 0;
        pIpat->m_MaxDelayF = 0;
        pIpat->m_MinMaxDelayValid = FALSE;
        pIpat->m_FirstRtpTime = RtpTimestamp;
        pIpat->m_LastRtpTime = RtpTimestamp;
        pIpat->m_BerNumPacketsReceived = 0;
        pIpat->m_BerNumPacketsLost = 0;
        pIpat->m_BerNumPacketsMainStart = pIpRxChannel->m_TotNumPackets;
        pIpat->m_BerNumPacketLostMainStart = pIpRxChannel->m_NumPacketsNotReconstructed;
        pIpat->m_BerNumPacketsMainCur = pIpRxChannel->m_TotNumPackets;
        pIpat->m_BerNumPacketLostMainCur = pIpRxChannel->m_NumPacketsNotReconstructed;
    } else
    {
        Int  NewIpat;
        Int  NewDelayF;
        if (CurRefTimestamp-pIpat->m_StartTime>54000000)   // 1 second
        {
            IpRxIpat*  pIpatPrev = pIpat;
            IpRxIpat*  pIpat2;
            if (pIpat->m_MinIpat!=-1 && pIpat->m_MaxIpat!=-1)
            {
                // Store values per second for user
                pIpRxChannel->m_MinIpatSecond[IpPortIndex] = pIpat->m_MinIpat;
                pIpRxChannel->m_MaxIpatSecond[IpPortIndex] = pIpat->m_MaxIpat;
            }
            if (pIpat->m_MinMaxDelayValid)
            {
                pIpRxChannel->m_MinDelayFSecond[IpPortIndex] = pIpat->m_MinDelayF;
                pIpRxChannel->m_MaxDelayFSecond[IpPortIndex] = pIpat->m_MaxDelayF;
            }
            
            pIpRxChannel->m_BerNumIpPacketsSecond[IpPortIndex] = 
                                                           pIpat->m_BerNumPacketsReceived;
            pIpRxChannel->m_BerNumIpPacketsLostSecond[IpPortIndex] = 
                                                               pIpat->m_BerNumPacketsLost;
            pIpRxChannel->m_BerNumIpPacketsMainSecond[IpPortIndex] = 
                                   DtaIpRxCalcDiffValue32(pIpat->m_BerNumPacketsMainStart,
                                   pIpat->m_BerNumPacketsMainCur);
            pIpRxChannel->m_BerNumIpPacketsLostMainSecond[IpPortIndex] = 
                                DtaIpRxCalcDiffValue32(pIpat->m_BerNumPacketLostMainStart,
                                pIpat->m_BerNumPacketLostMainCur);
            
            if (pIpRxChannel->m_IpParsMode == DTA_IP_RX_2022_7)
            {
                // Check if other channel is not receiving packets. If so, we estimate the 
                // lost packets of the other channel
                pIpat2 =  &pIpRxChannel->m_Ipat[IpPortIndex2]
                                                [pIpRxChannel->m_CurIpatEl[IpPortIndex2]];
                if (CurRefTimestamp - pIpat2->m_StartTime > 
                                                        54000000*DTA_IPRX_LOST_PKT_SECOND)
                {
                    // Reset statistics
                    pIpat2->m_BerNumPacketsReceived = 0;
                    pIpat2->m_BerNumPacketsLost = 0;
                    pIpRxChannel->m_NumSeqNumStat[IpPortIndex2] = 0;
                    pIpRxChannel->m_LastSeqNumStat[IpPortIndex2] = -1;
                    pIpRxChannel->m_NumIpPacketsLost[IpPortIndex2] = 
                                      pIpRxChannel->m_NumIpPacketsReceived[IpPortIndex] -
                                      pIpRxChannel->m_NumIpPacketsReceived[IpPortIndex2] +
                                      pIpRxChannel->m_NumIpPacketsLost[IpPortIndex];

                    pIpRxChannel->m_BerNumIpPacketsSecond[IpPortIndex2] = 
                                                           pIpat->m_BerNumPacketsReceived;
                    pIpRxChannel->m_BerNumIpPacketsLostSecond[IpPortIndex2] = 
                                                           pIpat->m_BerNumPacketsReceived;
                    pIpRxChannel->m_BerNumIpPacketsMinute[IpPortIndex2] = 
                                                           pIpat->m_BerNumPacketsReceived;
                    pIpRxChannel->m_BerNumIpPacketsLostMinute[IpPortIndex2] = 
                                                           pIpat->m_BerNumPacketsReceived;
                    pIpRxChannel->m_MinSkewSecond = 0;
                    pIpRxChannel->m_MaxSkewSecond = 0;
                    pIpRxChannel->m_MinSkewMinute = 0;
                    pIpRxChannel->m_MaxSkewMinute = 0;
                }
            }
            // Store values also in per minute statistics for user if we don't need
            // to recalculate
            if (pIpRxChannel->m_MinIpatMinute[IpPortIndex]==-1 || 
                pIpRxChannel->m_MaxIpatMinute[IpPortIndex]==-1)
            {
                pIpRxChannel->m_MinIpatMinute[IpPortIndex] = pIpat->m_MinIpat;
                pIpRxChannel->m_MaxIpatMinute[IpPortIndex] = pIpat->m_MaxIpat;
                if (pIpat->m_MinMaxDelayValid)
                {
                    pIpRxChannel->m_MinDelayFMinute[IpPortIndex] = pIpat->m_MinDelayF;
                    pIpRxChannel->m_MaxDelayFMinute[IpPortIndex] = pIpat->m_MaxDelayF;
                }
                // Calc BER
                pIpRxChannel->m_BerNumIpPacketsMinute[IpPortIndex] = 
                                                           pIpat->m_BerNumPacketsReceived;
                pIpRxChannel->m_BerNumIpPacketsLostMinute[IpPortIndex] = 
                                                               pIpat->m_BerNumPacketsLost;
                pIpRxChannel->m_BerNumIpPacketsMainMinute[IpPortIndex] = 
                                   pIpRxChannel->m_BerNumIpPacketsMainSecond[IpPortIndex];
                pIpRxChannel->m_BerNumIpPacketsLostMainMinute[IpPortIndex] = 
                               pIpRxChannel->m_BerNumIpPacketsLostMainSecond[IpPortIndex];

            } else if (pIpRxChannel->m_NumIpatEl[IpPortIndex] != IPRX_IPAT_NUM_SECONDS)
            {
                UInt  BerNumPktsMin = 0;
                UInt  BerNumLostMin = 0;
                
                UInt  BerNumPktsMainMin = 0;
                UInt  BerNumLostMainMin = 0;
                Int  i;
                // Calc Ipat minute
                if (pIpat->m_MinIpat!=-1 && pIpat->m_MaxIpat != -1)
                {
                    if (pIpat->m_MinIpat < pIpRxChannel->m_MinIpatMinute[IpPortIndex])
                        pIpRxChannel->m_MinIpatMinute[IpPortIndex] = pIpat->m_MinIpat;
                    if (pIpat->m_MaxIpat > pIpRxChannel->m_MaxIpatMinute[IpPortIndex])
                        pIpRxChannel->m_MaxIpatMinute[IpPortIndex] = pIpat->m_MaxIpat;
                }

                // Calc Delay factor minute
                if (pIpat->m_MinMaxDelayValid)
                {
                    if (pIpat->m_MinDelayF < pIpRxChannel->m_MinDelayFMinute[IpPortIndex])
                        pIpRxChannel->m_MinDelayFMinute[IpPortIndex] = pIpat->m_MinDelayF;
                    else if (pIpat->m_MaxDelayF > 
                                             pIpRxChannel->m_MaxDelayFMinute[IpPortIndex])
                        pIpRxChannel->m_MaxDelayFMinute[IpPortIndex] = pIpat->m_MaxDelayF;
                }
                // Calc BER minute
                for (i=0; i<pIpRxChannel->m_NumIpatEl[IpPortIndex];i++)
                {
                    BerNumPktsMin+= 
                             pIpRxChannel->m_Ipat[IpPortIndex][i].m_BerNumPacketsReceived;
                    BerNumLostMin+= 
                                 pIpRxChannel->m_Ipat[IpPortIndex][i].m_BerNumPacketsLost;
                    
                    BerNumPktsMainMin += DtaIpRxCalcDiffValue32(
                            pIpRxChannel->m_Ipat[IpPortIndex][i].m_BerNumPacketsMainStart,
                            pIpRxChannel->m_Ipat[IpPortIndex][i].m_BerNumPacketsMainCur);
                    BerNumLostMainMin += DtaIpRxCalcDiffValue32(
                         pIpRxChannel->m_Ipat[IpPortIndex][i].m_BerNumPacketLostMainStart,
                         pIpRxChannel->m_Ipat[IpPortIndex][i].m_BerNumPacketLostMainCur);
                    
                }
                pIpRxChannel->m_BerNumIpPacketsMinute[IpPortIndex] = BerNumPktsMin;
                pIpRxChannel->m_BerNumIpPacketsLostMinute[IpPortIndex] = BerNumLostMin;
                pIpRxChannel->m_BerNumIpPacketsMainMinute[IpPortIndex] = 
                                                                        BerNumPktsMainMin;
                pIpRxChannel->m_BerNumIpPacketsLostMainMinute[IpPortIndex] = 
                                                                        BerNumLostMainMin;
            }

            pIpRxChannel->m_CurIpatEl[IpPortIndex]++;
            if (pIpRxChannel->m_CurIpatEl[IpPortIndex] == IPRX_IPAT_NUM_SECONDS)
                pIpRxChannel->m_CurIpatEl[IpPortIndex] = 0;

            if (pIpRxChannel->m_NumIpatEl[IpPortIndex] == IPRX_IPAT_NUM_SECONDS)
            {
                // Max. reached, we have to recalculate
                Recalc = TRUE;
            } else 
                pIpRxChannel->m_NumIpatEl[IpPortIndex]++;
            pIpat = 
               &pIpRxChannel->m_Ipat[IpPortIndex][pIpRxChannel->m_CurIpatEl[IpPortIndex]];
            
            pIpat->m_StartTime = CurRefTimestamp;
            pIpat->m_MinIpat = -1;
            pIpat->m_MaxIpat = -1;
            pIpat->m_MinDelayF = 0;
            pIpat->m_MaxDelayF = 0;
            pIpat->m_MinMaxDelayValid = FALSE;
            pIpat->m_BerNumPacketsReceived = 0;
            pIpat->m_BerNumPacketsLost = 0;

            // Take values from previous slice. IN case slice is too fast for the bitrate
            pIpat->m_LastSampleTime = pIpatPrev->m_LastSampleTime;
            pIpat->m_FirstSampleTime = pIpatPrev->m_LastSampleTime;
            pIpat->m_FirstRtpTime = pIpatPrev->m_LastRtpTime;
            pIpat->m_BerNumPacketsMainStart = pIpatPrev->m_BerNumPacketsMainCur;
            pIpat->m_BerNumPacketLostMainStart = pIpatPrev->m_BerNumPacketLostMainCur;
        }

        pIpat->m_BerNumPacketsMainCur = pIpRxChannel->m_TotNumPackets;
        pIpat->m_BerNumPacketLostMainCur = pIpRxChannel->m_NumPacketsNotReconstructed;

        if (pIpRxChannel->m_NumSeqNumStat[IpPortIndex] == IPRX_MAX_RTP_SEQ_NUM_STAT)
        {
            // Calculate gap
            if (pIpRxChannel->m_LastSeqNumStat[IpPortIndex] != -1)
            {
                NumPackets = DtaIpRxGetSequenceNumberGap(
                                      (UInt16)pIpRxChannel->m_LastSeqNumStat[IpPortIndex],
                                      (UInt16)pIpRxChannel->m_SeqNumStat[IpPortIndex][0]);

                pIpat->m_BerNumPacketsLost += NumPackets-1;
                pIpat->m_BerNumPacketsReceived += NumPackets;
                pIpRxChannel->m_NumIpPacketsReceived[IpPortIndex] +=1;
                pIpRxChannel->m_NumIpPacketsLost[IpPortIndex] += NumPackets-1;
            }
            pIpRxChannel->m_LastSeqNumStat[IpPortIndex] = 
                                          (Int)pIpRxChannel->m_SeqNumStat[IpPortIndex][0];
            // Remove first one
            DtMemMove(&pIpRxChannel->m_SeqNumStat[IpPortIndex][0], 
                                       &pIpRxChannel->m_SeqNumStat[IpPortIndex][1], 
                                       sizeof(pIpRxChannel->m_SeqNumStat[IpPortIndex][0])*
                                       (IPRX_MAX_RTP_SEQ_NUM_STAT-1));
            pIpRxChannel->m_NumSeqNumStat[IpPortIndex] = IPRX_MAX_RTP_SEQ_NUM_STAT - 1;
        }
        // Add new seq. number
        if (pIpRxChannel->m_NumSeqNumStat[IpPortIndex] == 0)
        {
            pIpRxChannel->m_SeqNumStat[IpPortIndex][0] = SeqNumber;
            pIpRxChannel->m_NumSeqNumStat[IpPortIndex] = 1;
        } else {
            // Check if we can insert it at the end
            if (SeqNumber!=pIpRxChannel->m_LastSeqNumStat[IpPortIndex] && 
                                     !DtaIpRxRtpIsSeqNumLess(SeqNumber,
                                     (UInt16)pIpRxChannel->m_LastSeqNumStat[IpPortIndex]))
            {
                // Not too late, add
                // First check if the element must be inserted after the last one. This is
                // the most common situation and then we don't need to walk through the
                // entire list.
                UInt16  SeqNumTmp = pIpRxChannel->m_SeqNumStat[IpPortIndex]
                                           [pIpRxChannel->m_NumSeqNumStat[IpPortIndex]-1];
                if (SeqNumTmp != SeqNumber)
                {
                    if (DtaIpRxRtpIsSeqNumLess(SeqNumTmp, SeqNumber))
                    {
                        // Add to end
                        pIpRxChannel->m_SeqNumStat[IpPortIndex]
                                 [pIpRxChannel->m_NumSeqNumStat[IpPortIndex]] = SeqNumber;
                        pIpRxChannel->m_NumSeqNumStat[IpPortIndex]++;
                    } else {
                        // Insert sorted in list
                        Bool  Ready = FALSE;
                        Int  i=0;
                        while (!Ready)
                        {
                            SeqNumTmp = pIpRxChannel->m_SeqNumStat[IpPortIndex][i];
                            if (DtaIpRxRtpIsSeqNumLess(SeqNumber, SeqNumTmp))
                            {
                                // Found location
                                DtMemMove(&pIpRxChannel->m_SeqNumStat[IpPortIndex][i+1], 
                                       &pIpRxChannel->m_SeqNumStat[IpPortIndex][i], 
                                       sizeof(pIpRxChannel->m_SeqNumStat[IpPortIndex][0])*
                                       (pIpRxChannel->m_NumSeqNumStat[IpPortIndex]-i));
                                pIpRxChannel->m_SeqNumStat[IpPortIndex][i] = SeqNumber;
                                pIpRxChannel->m_NumSeqNumStat[IpPortIndex]++;
                                Ready = TRUE;
                            } else 
                            {
                                i++;
                                if ((UInt)i >= pIpRxChannel->m_NumSeqNumStat[IpPortIndex])
                                {
                                    // Error. Clear list
                                    pIpRxChannel->m_NumSeqNumStat[IpPortIndex] = 1;
                                    pIpRxChannel->m_SeqNumStat[IpPortIndex][0] = 
                                                                                SeqNumber;
                                    Ready = TRUE;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Calculate Ipat
        NewIpat = (Int)(pDmaRxHeader->m_Timestamp - pIpat->m_LastSampleTime);
        if (pIpat->m_MinIpat==-1 || NewIpat<pIpat->m_MinIpat)
            pIpat->m_MinIpat = NewIpat;

        if (pIpat->m_MaxIpat==-1 || NewIpat>pIpat->m_MaxIpat)
            pIpat->m_MaxIpat = NewIpat;

        pIpat->m_LastSampleTime = pDmaRxHeader->m_Timestamp;
        pIpat->m_LastRtpTime = RtpTimestamp;

        // Calculate delay factor
        if (pIpat->m_FirstRtpTime != pIpat->m_LastRtpTime)
        {
            // Calculate delay factor of new received packet
            Int64  ActualTime;
            Int64  ExpectedTime;
            if (pIpat->m_FirstRtpTime > pIpat->m_LastRtpTime)
                ExpectedTime = 0xffffffff - pIpat->m_FirstRtpTime + pIpat->m_LastRtpTime;
            else
                ExpectedTime = pIpat->m_LastRtpTime - pIpat->m_FirstRtpTime;


            // We don't want to devide here, so we calculate in units of 54us
            if (pIpRxChannel->m_DetVidStd == DT_VIDSTD_TS)
                ExpectedTime = (ExpectedTime * 600);        // in 54 us units
            else
                ExpectedTime = (ExpectedTime * 2);          // in 54 us units
            ActualTime = pIpat->m_LastSampleTime - pIpat->m_FirstSampleTime; // in 54us units
            NewDelayF = (Int)(ActualTime - ExpectedTime);
            if (!pIpat->m_MinMaxDelayValid || NewDelayF<pIpat->m_MinDelayF)
                pIpat->m_MinDelayF = NewDelayF;
            if (!pIpat->m_MinMaxDelayValid || NewDelayF>pIpat->m_MaxDelayF)
                pIpat->m_MaxDelayF = NewDelayF;
            pIpat->m_MinMaxDelayValid = TRUE;
        }

        if (Recalc)
        {
            int  i;
            Bool  Init = TRUE;
            
            UInt  BerNumPktsMin = 0;
            UInt  BerNumLostMin = 0;
                
            UInt  BerNumPktsMainMin = 0;
            UInt  BerNumLostMainMin = 0;
            
            // Calculate Min/Max Ipat / DelayF
            Int  MinIpatMinute = pIpRxChannel->m_Ipat[IpPortIndex][0].m_MinIpat;
            Int  MaxIpatMinute = pIpRxChannel->m_Ipat[IpPortIndex][0].m_MaxIpat;
            Int  MinDelayFMinute = -1;
            Int  MaxDelayFMinute = -1;

            // Do local calculations to minimize disruptions 
            for (i=0; i<pIpRxChannel->m_NumIpatEl[IpPortIndex]; i++)
            {
                if (pIpRxChannel->m_Ipat[IpPortIndex][i].m_MinIpat < MinIpatMinute)
                    MinIpatMinute = pIpRxChannel->m_Ipat[IpPortIndex][i].m_MinIpat;
                if (pIpRxChannel->m_Ipat[IpPortIndex][i].m_MaxIpat > MaxIpatMinute)
                    MaxIpatMinute = pIpRxChannel->m_Ipat[IpPortIndex][i].m_MaxIpat;

                if (pIpRxChannel->m_Ipat[IpPortIndex][i].m_MinMaxDelayValid)
                {
                    if (Init)
                    {
                        Init = FALSE;
                        MinDelayFMinute = 
                                         pIpRxChannel->m_Ipat[IpPortIndex][i].m_MinDelayF;
                        MaxDelayFMinute = 
                                         pIpRxChannel->m_Ipat[IpPortIndex][i].m_MaxDelayF;
                    } else {
                        if (pIpRxChannel->m_Ipat[IpPortIndex][i].m_MinDelayF < 
                                                                          MinDelayFMinute)
                            MinDelayFMinute = 
                                         pIpRxChannel->m_Ipat[IpPortIndex][i].m_MinDelayF;
                        if (pIpRxChannel->m_Ipat[IpPortIndex][i].m_MaxDelayF > 
                                                                          MaxDelayFMinute)
                            MaxDelayFMinute = 
                                         pIpRxChannel->m_Ipat[IpPortIndex][i].m_MaxDelayF;
                    }
                }
            }

            pIpRxChannel->m_MinIpatMinute[IpPortIndex] = MinIpatMinute;
            pIpRxChannel->m_MaxIpatMinute[IpPortIndex] = MaxIpatMinute;
            pIpRxChannel->m_MinDelayFMinute[IpPortIndex] = MinDelayFMinute;
            pIpRxChannel->m_MaxDelayFMinute[IpPortIndex] = MaxDelayFMinute;

            // Calc BER minute
            for (i=0; i<pIpRxChannel->m_NumIpatEl[IpPortIndex];i++)
            {
                BerNumPktsMin+= 
                             pIpRxChannel->m_Ipat[IpPortIndex][i].m_BerNumPacketsReceived;
                BerNumLostMin+= 
                                 pIpRxChannel->m_Ipat[IpPortIndex][i].m_BerNumPacketsLost;
                BerNumPktsMainMin += DtaIpRxCalcDiffValue32(
                            pIpRxChannel->m_Ipat[IpPortIndex][i].m_BerNumPacketsMainStart,
                            pIpRxChannel->m_Ipat[IpPortIndex][i].m_BerNumPacketsMainCur);
                BerNumLostMainMin += DtaIpRxCalcDiffValue32(
                         pIpRxChannel->m_Ipat[IpPortIndex][i].m_BerNumPacketLostMainStart,
                         pIpRxChannel->m_Ipat[IpPortIndex][i].m_BerNumPacketLostMainCur);
            }
            
            pIpRxChannel->m_BerNumIpPacketsMinute[IpPortIndex] = BerNumPktsMainMin;
            pIpRxChannel->m_BerNumIpPacketsLostMinute[IpPortIndex] = BerNumLostMainMin;
            pIpRxChannel->m_BerNumIpPacketsMainMinute[IpPortIndex] = BerNumPktsMainMin;
            pIpRxChannel->m_BerNumIpPacketsLostMainMinute[IpPortIndex] = 
                                                                        BerNumLostMainMin;
        }
    }
    
    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Skew -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    // The skew can only be measured if using 2 ports. It's the packet delay between these
    // ports
    Recalc = FALSE;

    if (pIpRxChannel->m_IpParsMode != DTA_IP_RX_2022_7)
        return;
    pSkew =  &pIpRxChannel->m_Skew[pIpRxChannel->m_CurSkewEl];
    
    if (pIpRxChannel->m_NumSkewEl == 0)
    {
        pIpRxChannel->m_NumSkewEl = 1;
        pSkew->m_StartTime = CurRefTimestamp;
        pSkew->m_SeqNum[0] = -1;
        pSkew->m_SeqNum[1] = -1;
        pSkew->m_SampleTime[IpPortIndex] = pDmaRxHeader->m_Timestamp;
        pSkew->m_SeqNum[IpPortIndex] = SeqNumber;
        pSkew->m_RtpTime[IpPortIndex] = RtpTimestamp;
        pSkew->m_MinSkew = 0;
        pSkew->m_MaxSkew = 0;
        pSkew->m_MinMaxValid = FALSE;
    } else 
    {
        if (CurRefTimestamp-pSkew->m_StartTime>54000000)   // 1 second
        {
            IpRxSkew*  pSkewPrev = pSkew;

            // Store values per second for user
            if (pSkew->m_MinMaxValid)
            {
                pIpRxChannel->m_MinSkewSecond = pSkew->m_MinSkew;
                pIpRxChannel->m_MaxSkewSecond = pSkew->m_MaxSkew;
            
                // Store values also in per minute statistics for user if we don't need
                // to recalculate
                if (pIpRxChannel->m_NumSkewEl == 1)
                {
                    pIpRxChannel->m_MinSkewMinute = pSkew->m_MinSkew;
                    pIpRxChannel->m_MaxSkewMinute = pSkew->m_MaxSkew;
                } else if (pIpRxChannel->m_NumSkewEl != IPRX_SKEW_NUM_SECONDS)
                {
                    if (abs(pSkew->m_MinSkew) < abs(pIpRxChannel->m_MinSkewMinute))
                        pIpRxChannel->m_MinSkewMinute = pSkew->m_MinSkew;
                    if (abs(pSkew->m_MaxSkew) > abs(pIpRxChannel->m_MaxSkewMinute))
                        pIpRxChannel->m_MaxSkewMinute = pSkew->m_MaxSkew;
                }
            }

            pIpRxChannel->m_CurSkewEl++;
            if (pIpRxChannel->m_CurSkewEl == IPRX_SKEW_NUM_SECONDS)
                pIpRxChannel->m_CurSkewEl = 0;

            if (pIpRxChannel->m_NumSkewEl == IPRX_SKEW_NUM_SECONDS)
            {
                // Max. reached, we have to recalculate
                Recalc = TRUE;
            } else 
                pIpRxChannel->m_NumSkewEl++;

            pSkew =  &pIpRxChannel->m_Skew[pIpRxChannel->m_CurSkewEl];

            // Store last sample
            pSkew->m_StartTime = CurRefTimestamp;
            if (!pSkewPrev->m_MinMaxValid && (pSkew->m_SeqNum[0]==-1 || 
                                                                  pSkew->m_SeqNum[1]==-1))
            {
                pSkew->m_SeqNum[0] = pSkewPrev->m_SeqNum[0];
                pSkew->m_SeqNum[1] = pSkewPrev->m_SeqNum[1];
                pSkew->m_RtpTime[0] = pSkewPrev->m_RtpTime[0];
                pSkew->m_RtpTime[1] = pSkewPrev->m_RtpTime[1];
                pSkew->m_SampleTime[0] = pSkewPrev->m_SampleTime[0];
                pSkew->m_SampleTime[1] = pSkewPrev->m_SampleTime[1];
            } else {
                pSkew->m_SeqNum[0] = -1;
                pSkew->m_SeqNum[1] = -1;
                pSkew->m_RtpTime[0] = 0;
                pSkew->m_RtpTime[1] = 0;
            }
            pSkew->m_MaxSkew = 0;
            pSkew->m_MinMaxValid = FALSE;
        }

        // Check current sample
        if (pSkew->m_SeqNum[0]==-1 && pSkew->m_SeqNum[1]==-1)
        {
            // Store first sample
            pSkew->m_SampleTime[IpPortIndex] = pDmaRxHeader->m_Timestamp;
            pSkew->m_SeqNum[IpPortIndex] = SeqNumber;
            pSkew->m_RtpTime[IpPortIndex] = RtpTimestamp;
        } else 
        {
            if (pSkew->m_SeqNum[IpPortIndex2] != -1)
            {
                // Check if we have a match
                if (RtpTimestamp == pSkew->m_RtpTime[IpPortIndex2])
                {
                    if (SeqNumber == (UInt16)pSkew->m_SeqNum[IpPortIndex2])
                    {
                        // Found a match
                        Int64  Skew = pDmaRxHeader->m_Timestamp - 
                                                        pSkew->m_SampleTime[IpPortIndex2];
                        if (pSkew->m_MinMaxValid)
                        {
                            if (abs(Skew) < abs(pSkew->m_MinSkew))
                                pSkew->m_MinSkew = Skew;
                            else if (abs(Skew) > abs(pSkew->m_MaxSkew))
                                pSkew->m_MaxSkew = Skew;
                        } else {
                            pSkew->m_MinSkew = Skew;
                            pSkew->m_MaxSkew = Skew;
                            pSkew->m_MinMaxValid = TRUE;
                        }

                        // Reset
                        pSkew->m_SeqNum[IpPortIndex] = -1;
                        pSkew->m_SeqNum[IpPortIndex2] = -1;
                    } else if (!DtaIpRxRtpIsSeqNumLess2(SeqNumber, RtpTimestamp,
                                                    (UInt16)pSkew->m_SeqNum[IpPortIndex2],
                                                    pSkew->m_RtpTime[IpPortIndex2]))
                    {
                        pSkew->m_SampleTime[IpPortIndex] = pDmaRxHeader->m_Timestamp;
                        pSkew->m_SeqNum[IpPortIndex] = SeqNumber;
                        pSkew->m_RtpTime[IpPortIndex] = RtpTimestamp;
                        
                        pSkew->m_SeqNum[IpPortIndex2] = -1;
                    } else {
                        // Skip this sample
                    }
                } else if (!DtaIpRxRtpIsTimestampLess(RtpTimestamp, 
                                                      pSkew->m_RtpTime[IpPortIndex2]))
                {
                    if (pSkew->m_SeqNum[IpPortIndex] == -1)
                    {
                        pSkew->m_SampleTime[IpPortIndex] = pDmaRxHeader->m_Timestamp;
                        pSkew->m_SeqNum[IpPortIndex] = SeqNumber;
                        pSkew->m_RtpTime[IpPortIndex] = RtpTimestamp;
                    }
                } else
                {
                    // Skip this sample
                }

            } else 
            {
                // For now, skip
            }
        }

        if (Recalc)
        {
            // Calculate Min/Max Skew
            int  i;
            Bool  Init = TRUE;
            Int64  MinSkewMinute = 0;
            Int64  MaxSkewMinute = 0; 
            
            // Do local calculations to minimize
            for (i=0; i<pIpRxChannel->m_NumSkewEl; i++)
            {
                if (pIpRxChannel->m_Skew[i].m_MinMaxValid)
                {    
                    if (Init)
                    {
                        Init = FALSE;
                        MinSkewMinute = pIpRxChannel->m_Skew[i].m_MinSkew;
                        MaxSkewMinute = pIpRxChannel->m_Skew[i].m_MaxSkew;
                    } else
                    {
                        if (abs(pIpRxChannel->m_Skew[i].m_MinSkew) < abs(MinSkewMinute))
                            MinSkewMinute = pIpRxChannel->m_Skew[i].m_MinSkew;
                        if (abs(pIpRxChannel->m_Skew[i].m_MaxSkew) > abs(MaxSkewMinute))
                            MaxSkewMinute = pIpRxChannel->m_Skew[i].m_MaxSkew;
                    }
                }
            }
            pIpRxChannel->m_MinSkewMinute = MinSkewMinute;
            pIpRxChannel->m_MaxSkewMinute = MaxSkewMinute;
        }
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpParsePayloadMpeg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxParsePayLoadRtp(UserIpRxChannel* pIpRxChannel, DtaDmaRxHeader* pDmaRxHeader,
        UInt16 SeqNumber, UInt32 Timestamp, UInt Size, UInt16 DvbOffset, UInt16 RtpOffset,
        UInt8 FrameCount)
{
    RtpListEntry*  pRtpListEntry;
    Int  MaxNumRtpDvbPackets = MAX_NUM_RTP_PACKETS_TS;
    // Timestamp must not be 0 for the lookup table to work
    if (Timestamp == 0)
        Timestamp = 1;

    DtDbgOut(MAX, IP_RX_REC, "Channel %d: RTP packet rcvd.  SeqNr: %u, Timestamp: %08xh,"
                            " payload offset=%d, FrameLength=%d",
                            pIpRxChannel->m_ChannelIndex, SeqNumber, Timestamp, DvbOffset, 
                            pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength);

    if (pIpRxChannel->m_VidStd != DT_VIDSTD_TS)
        MaxNumRtpDvbPackets = MAX_NUM_RTP_PACKETS_SDI;

    // Increment No Fec packets receive counter
    if (++pIpRxChannel->m_RstCntFecColumn >= MaxNumRtpDvbPackets)
        pIpRxChannel->m_DetFecNumColumns = 0;
    
    if (++pIpRxChannel->m_RstCntFecRow >= MaxNumRtpDvbPackets)
        pIpRxChannel->m_DetFecNumRows = 0;

    if (pIpRxChannel->m_RxControl == DT_RXCTRL_IDLE) 
    {
        // We only need to calculate the bitrate
        if (pIpRxChannel->m_DetVidStd == DT_VIDSTD_TS)
            DtaIpRxProcessDvbPayLoad(
                pIpRxChannel,
                (UInt8*)pDmaRxHeader + DvbOffset + sizeof(DtaDmaRxHeader),
                pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength - DvbOffset - 4,
                pDmaRxHeader->m_Timestamp, 0);
        
        DtaIpRxUserChRefDecr(pIpRxChannel);
        return;
    }
    
    if (!pIpRxChannel->m_RtpFirstPacket && DtaIpRxRtpIsSeqNumLess2(SeqNumber, Timestamp, 
                         pIpRxChannel->m_RtpLastSeqNum, pIpRxChannel->m_RtpLastTimestamp))
    {
        // Skip this packet. It's already too late
        DtDbgOut(ERR, IP_RX, "SKIPPING PACKET SN:%d Time:%x LastSN:%d LastTime:%x", 
                                      SeqNumber, Timestamp, pIpRxChannel->m_RtpLastSeqNum,
                                      pIpRxChannel->m_RtpLastTimestamp);
    } else {
        // First check if seq. number already in list by checking lookup table
        if ((pIpRxChannel->m_pRtpAvailLookup1==NULL && 
                                pIpRxChannel->m_pRtpAvailLookup2==NULL) || 
                                (pIpRxChannel->m_pRtpAvailLookup1[SeqNumber]!=Timestamp && 
                                pIpRxChannel->m_pRtpAvailLookup2[SeqNumber]!=Timestamp))
        {
            pRtpListEntry = DtaIpRxRtpGetFreeEntry(pIpRxChannel);
            if (pRtpListEntry != NULL)
            {        
                // Fill content
                pRtpListEntry->m_RtpSequenceNumber = SeqNumber;
                pRtpListEntry->m_RtpTimestamp = Timestamp;
                pRtpListEntry->m_PayloadOffset = DvbOffset;
                pRtpListEntry->m_RtpOffset = RtpOffset;
                pRtpListEntry->m_FrameCount = FrameCount;
                // Copy complete packet into the buffer
                DtMemCopy(pIpRxChannel->m_pRtpBuffer + 
                                 (pRtpListEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH),
                                 pDmaRxHeader, Size);
        
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
                DtDbgOut(ERR, IP_RX, "Channel %d: No queue elements available. Buffer "
                           "FULL. Seq. Num: %d", pIpRxChannel->m_ChannelIndex, SeqNumber);

                // Extra trigger reconstructor to parse the data
                pIpRxChannel->m_RxIncomingPackets = TRUE;
            }
        }
    }
    DtaIpRxUserChRefDecr(pIpRxChannel);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxParsePayLoadFec -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaIpRxParsePayLoadFec(UserIpRxChannel* pIpRxChannel, DtaDmaRxHeader* pDmaRxHeader,
            UInt16 SeqNumber, UInt32 Timestamp, Int Size, UInt16 DvbOffset, 
            UInt16 RtpOffset, UInt PacketSize, DtaIpPort *pIpPort, Int DestPort, Bool Sdi)
{
    
    RtpListEntry*  pRtpListEntry;
    UInt8*  pRxData = (UInt8*)pDmaRxHeader + sizeof(DtaDmaRxHeader);
    UInt16  SnBase;
    UInt16  Na;
    Bool  Row;
    UInt16  Offset;
    UInt  HeaderSize;
    
    if (Sdi)
    {
        FecHeader2022_5*  pFecHeader = (FecHeader2022_5*) (pRxData + DvbOffset);
        HeaderSize = sizeof(FecHeader2022_5);
        Na = ((UInt)pFecHeader->m_NAH<<2) + pFecHeader->m_NAL;
        Offset = ((UInt16)pFecHeader->m_OffsetH<<2) + pFecHeader->m_OffsetL;
        Row = (Offset == 1); 
        SnBase = pFecHeader->m_SnBase;
        DtDbgOut(AVG, IP_RX_REC, "Na: %xh Offset: %xh Row: %xh SnBase:%x", Na, Offset, 
                                                                             Row, SnBase);
    } else
    {
        FecHeader*  pFecHeader = (FecHeader*) (pRxData + DvbOffset);
        Bool  DstPortCorrect = FALSE;
        HeaderSize = sizeof(FecHeader);
        // Check if the correct port number is used for this FEC Packet
        Row = pFecHeader->m_D==1;
        if (!Row && DestPort==(pIpRxChannel->m_DstPort+FEC_INC_COLUMN_PORT))
            DstPortCorrect = TRUE;
        else if (Row && DestPort==(pIpRxChannel->m_DstPort+FEC_INC_ROW_PORT))
            DstPortCorrect = TRUE;
        else if ((pIpRxChannel->m_IpParsMode & DTA_IP_RX_2022_7) != 0)
        {
            if (!Row && DestPort==(pIpRxChannel->m_DstPort2+FEC_INC_COLUMN_PORT))
                DstPortCorrect = TRUE;
            else if (Row && DestPort==(pIpRxChannel->m_DstPort2+FEC_INC_ROW_PORT))
                DstPortCorrect = TRUE;
        }
        if (!DstPortCorrect)
        {
            // Port number is incorrect for this fec type. 
            DtDbgOut(ERR, IP_RX, 
               "Channel %d: Incorrect destination port number %i for FEC packet type %i.",
               pIpRxChannel->m_ChannelIndex, DestPort, pFecHeader->m_D);
            // Don't release channel. It will be done autom.
            return FALSE;
        }

        // Check if the correct error correction type is used
        if (pFecHeader->m_Type != 0) 
        {
            // Fec packet is not XOR Fec. 
            DtDbgOut(ERR, IP_RX,"Channel %d: Incorrect FEC error correction type %i",
                                        pIpRxChannel->m_ChannelIndex, pFecHeader->m_Type);

            // Don't release channel. It will be done autom.
            return FALSE;
        }
        Na = pFecHeader->m_NA;
        Offset = pFecHeader->m_Offset;
        SnBase = pFecHeader->m_SnBase;
    }

    if (Na==0 || Offset==0)
    {
        // Illegal FEC packet
        DtDbgOut(ERR, IP_RX,"Channel %d: Invalid FEC packet. SnBase:%i Offset:%i Na:%i",
                   pIpRxChannel->m_ChannelIndex, SnBase, Offset, Na);
        DT_ASSERT(FALSE);
        // Don't release channel. It will be done autom.
        return FALSE;
    }

    if (pIpRxChannel->m_RxControl == DT_RXCTRL_IDLE) 
    {
        // We only update status in IDLE state
        if (!Row)     // Column
        {
            pIpRxChannel->m_DetFecNumRows = Na;
            pIpRxChannel->m_RstCntFecRow = 0;
        } else {
            pIpRxChannel->m_DetFecNumColumns = Na;
            pIpRxChannel->m_RstCntFecColumn = 0;
        }

        DtaIpRxUserChRefDecr(pIpRxChannel);
        return TRUE;
    }

#ifdef _DEBUG
    /*if (Offset == 1)
    {
        // We skip the row FEC.
        DtDbgOut(ERR, IP_RX_REC, 
               "Channel %d: RD TST:SKIP FEC ROW, SeqNum:%i, Base: %i, MaxSeqNum:%i",
                         pIpRxChannel->m_ChannelIndex, 
                        -1, 
                         SnBase, ((Na-1)*Offset) + SnBase);

        DtaIpRxUserChRefDecr(pIpRxChannel);
        return TRUE;
    }*/
#endif
    pRtpListEntry = DtaIpRxRtpGetFreeEntry(pIpRxChannel);
    if (pRtpListEntry != NULL) 
    {
        // Fill content
        pRtpListEntry->m_RtpSequenceNumber = SeqNumber;
        pRtpListEntry->m_RtpTimestamp = Timestamp;
        pRtpListEntry->m_PayloadOffset = DvbOffset + HeaderSize;
        pRtpListEntry->m_RtpOffset = RtpOffset;

        // Store FEC information
        pRtpListEntry->m_FecNA = Na;
        pRtpListEntry->m_FecOffset = Offset;
        pRtpListEntry->m_FecSNBase = DtUInt16ByteSwap(SnBase);
        
        // Copy complete packet into the buffer
        DtMemCopy(pIpRxChannel->m_pRtpBuffer + 
                                 (pRtpListEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH),
                                 pDmaRxHeader, Size);

        // Add packet to correct queue
        if (!Row) 
        {   
            // Column            
            pIpRxChannel->m_DetFecNumRows = Na;
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
            DtDbgOut(AVG, IP_RX_REC, 
             "Channel %d: AddFec: COLUMN. BufIndex:%i, SeqNum:%i, Base: %i, MaxSeqNum:%i",
                                 pIpRxChannel->m_ChannelIndex, pRtpListEntry->m_BufIndex, 
                                 pRtpListEntry->m_RtpSequenceNumber, 
                                 pRtpListEntry->m_FecSNBase, 
                                 ((pRtpListEntry->m_FecNA-1)*pRtpListEntry->m_FecOffset) + 
                                 pRtpListEntry->m_FecSNBase);
        } else {  
            // Row
            pIpRxChannel->m_DetFecNumColumns = Na;
            pIpRxChannel->m_RstCntFecColumn = 0;
            DtDbgOut(AVG, IP_RX_REC, 
                "Channel %d: AddFec: ROW. BufIndex:%i, SeqNum:%i, Base: %i, MaxSeqNum:%i",
                                 pIpRxChannel->m_ChannelIndex, pRtpListEntry->m_BufIndex, 
                                 pRtpListEntry->m_RtpSequenceNumber, 
                                 pRtpListEntry->m_FecSNBase, 
                                 ((pRtpListEntry->m_FecNA-1)*pRtpListEntry->m_FecOffset) + 
                                 pRtpListEntry->m_FecSNBase);
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
void  DtaIpRxParsePacket(DtaIpPort* pIpPort, UInt8* pData, UInt Size, Bool NrtRxType2)
{   
    UInt  i;
    UInt  PacketSize;
    IpHeaderV4*  pIpHeaderV4 = NULL;
    IpHeaderV6*  pIpHeaderV6 = NULL;
    UdpHeader*  pUdpHeader = NULL;
    HbrMediaPlHeader*  pHbrHeader = NULL;
    Bool  PacketProcessed = FALSE;
    
    Bool  MulticastPacket = FALSE;
    DtaDmaRxHeader*  pDmaRxHeader = (DtaDmaRxHeader*)pData;
    UInt8*  pRxData = (UInt8*)pDmaRxHeader + sizeof(DtaDmaRxHeader);
    UInt8*  pRxDataAligned;
    UInt  PacketSizeAligned;
    EthernetIIHeader*  pEthernetIIHeader;
    EthernetIIHeaderVlan*  pEthernetIIHeaderVlan;
    Ethernet802_3Header*  pEthernet802_3Header;
    Ethernet802_3HeaderVlan*  pEthernet802_3HeaderVlan;
    UInt8*  pDstMac;
    PhyMac*  pPhyMac = &pIpPort->m_PhyMac;
    
    // fragmented frames
    UserIpRxChannel*  pIpRxFragment = NULL;
    DtaIpUserChannels*  pIpUserChannels = 
                                        &pIpPort->m_pDvcData->m_IpDevice.m_IpUserChannels;
    UInt  FrameLength;
    UInt  DvbTotalLength;

    // Protocol parsing
    UInt  NumChannels;
    UserIpRxChannel*  pIpRxChannel;
    UInt16  SrcPort;
    UInt16  DstPort;
    RtpHeader*  pRtpHeader = NULL;
    RtpExtension*  pRtpHeaderExtension = NULL;
    UInt  ProtocolType;
    Bool  RtpExt = FALSE;
    UInt16  DvbOffset;
    UInt16  RtpOffset;
    Bool  Type2 = (pIpPort->m_PortType == DTA_IPPORT_TYPE2);
    Int  EthType;
    Int  EthHeaderSize;
    Int  VlanId = 0;
    Int  DetVideoStd;
    UInt8  FrameCount = 0;
    
    // Calculate packetsize. Extract CRC + DMA header
    PacketSize = Size - 4 - sizeof(DtaDmaRxHeader);

    DtDbgOut(MAX, IP_RX, "Packetsize: %d", PacketSize);

    DT_ASSERT(PacketSize <= DTA_IPRX_MAX_PACKET_SIZE);

    if (pIpPort->m_PhyMac.m_LoopbackEnable) 
    {
        if (DtaIpRxHandleLoopback(pIpUserChannels, pRxData, PacketSize))
            return;
    }

    // Check if the network is administrative up, if not we discard all received packets
    if (pIpPort->m_PhyMac.m_NetworkAdminStatus != DTA_NW_ADMIN_STATUS_UP)
        return;

    if (NrtRxType2)
    {
        // For now, NRT Rx for type2 will be sent to network driver directly without any 
        // destination check.
        DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
        return;
    }

    pRxDataAligned = pRxData;
    PacketSizeAligned = PacketSize;

    if (pIpPort->m_PhyMac.m_AlignedPayload)
    {
        pRxDataAligned+= 2;
        PacketSizeAligned-= 2;
    }

    pEthernetIIHeader = (EthernetIIHeader*)pRxDataAligned;
    pEthernet802_3Header = (Ethernet802_3Header*)pRxDataAligned;
    pEthernetIIHeaderVlan = (EthernetIIHeaderVlan*)pRxDataAligned;
    pEthernet802_3HeaderVlan = (Ethernet802_3HeaderVlan*)pRxDataAligned;
    
    // Determine the encapsulation method by checking the ethernet type of the packet
    if (pEthernetIIHeader->m_Type == ETH_TYPE_VLAN_BE)
    {
        UInt16  OrigTci;
        EthType = pEthernetIIHeaderVlan->m_Type;
        EthHeaderSize = sizeof(EthernetIIHeaderVlan);
        // Restore the Tci value after use. It's used in the network driver also
        OrigTci = pEthernetIIHeaderVlan->m_TCI;
        pEthernetIIHeaderVlan->m_TCI = DtUInt16ByteSwap(pEthernetIIHeaderVlan->m_TCI);
        VlanId = pEthernetIIHeaderVlan->m_VId;
        pEthernetIIHeaderVlan->m_TCI = OrigTci;
    } else {
        EthType = pEthernetIIHeader->m_Type;
        EthHeaderSize = sizeof(EthernetIIHeader);
    }
    
    if (EthType == ETH_TYPE_IPV4_BE) 
    {
        // Ethernet II IpV4
        pIpHeaderV4 = (IpHeaderV4*)(pRxDataAligned + EthHeaderSize);
        pDstMac = (UInt8*)pEthernetIIHeader->m_DstMac;
    } else if (EthType == ETH_TYPE_IPV6_BE)
    {
        // Ethernet II IpV6
        pIpHeaderV6 = (IpHeaderV6*)(pRxDataAligned + EthHeaderSize);
        pDstMac = (UInt8*)pEthernetIIHeader->m_DstMac;
    } else if (pEthernet802_3Header->m_DSAP==0xAA && pEthernet802_3Header->m_SSAP==0xAA)
    {
        if (pEthernet802_3Header->m_Type == ETH_TYPE_VLAN_BE)
        {
            EthType = pEthernet802_3HeaderVlan->m_Type;
            EthHeaderSize = sizeof(Ethernet802_3HeaderVlan);
        } else {
            EthType = pEthernet802_3Header->m_Type;
            EthHeaderSize = sizeof(Ethernet802_3Header);
        }

        if (EthType == ETH_TYPE_IPV4_BE)
        {
            // 802.2/802.3 encapsulation IpV4
            pIpHeaderV4 = (IpHeaderV4*)(pRxDataAligned + EthHeaderSize);
            pDstMac = (UInt8*)pEthernet802_3Header->m_DstMac; 
        } else if (EthType == ETH_TYPE_IPV6_BE)
        {
            // 802.2/802.3 encapsulation IpV6
            pIpHeaderV6 = (IpHeaderV6*) (pRxDataAligned + EthHeaderSize);
            pDstMac = (UInt8*)pEthernet802_3Header->m_DstMac; 
        } else {
            // Unknown packet type.....
            DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
            return;
        }
    } else {
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
    if (pIpHeaderV4!=NULL && ((pIpHeaderV4->m_Flags&IPV4_HDR_FLAGS_MOREFRAG) != 0 || 
               ((pIpHeaderV4->m_Flags&IPV4_HDR_FLAGS_MOREFRAG) == 0 && 
               (pIpHeaderV4->m_FragmentOffsetH!=0 || pIpHeaderV4->m_FragmentOffsetL!=0))))
    {
        Bool  StopParsing = FALSE;
    
        pIpRxFragment = DtaIpRxParseFragmentedFrame(pIpUserChannels, pDmaRxHeader, Size,
                             pIpHeaderV4, MulticastPacket, pIpPort, VlanId, &StopParsing);
        if (StopParsing)
            return;
    }
    
    if (pIpRxFragment != NULL)
    {
        // A fragmented frame is complete and already stored in the RtpBuffer of the
        // first (valid) channel in the pIpRxA array. Redirect buffer pointers.
        // Note: The reference of the fragmented packet is increased.
        //       so from now on it has to be decreased after usage.
        pIpHeaderV4 = (IpHeaderV4*)(pIpRxFragment->m_pRtpBuffer + 
                                                      pIpRxFragment->m_CurIpHeaderOffset);
        pDmaRxHeader = (DtaDmaRxHeader*)pIpRxFragment->m_pRtpBuffer;
        PacketSize = pIpRxFragment->m_FragmentOffset - sizeof(DtaDmaRxHeader);
        pRxData = (UInt8*)pDmaRxHeader + sizeof(DtaDmaRxHeader);

        pRxDataAligned = pRxData;
        PacketSizeAligned = PacketSize;

        DtDbgOut(MAX, IP_RX, "Fragmented frame, new PacketSize %d.", PacketSize);

        if (pIpPort->m_PhyMac.m_AlignedPayload)
        {
            pRxDataAligned+= 2;
            PacketSizeAligned-= 2;
        }
    }

    // Check if we have a valid UDP header
    if (pIpHeaderV4 != NULL)
    {
        pUdpHeader = (UdpHeader*)((UInt8*)pIpHeaderV4 + 
                                                       (pIpHeaderV4->m_HeaderLength * 4));
        if (pIpHeaderV4->m_Protocol != IPV4_HDR_PROT_UDP)
        {
            if(pIpRxFragment != NULL)
                DtaIpRxUserChRefDecr(pIpRxFragment);
            DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
            return;
        }
    } else {
        // IpV6
        UInt8*  pData = (UInt8*)pIpHeaderV6 + sizeof(IpHeaderV6);
        IpV6Options*  pOptions;
        UInt  ExtHeader = pIpHeaderV6->m_NextHeader;
        
        while (TRUE)
        {
            pOptions = (IpV6Options*)pData;
            if (ExtHeader == IPV4_HDR_PROT_UDP)
                break;
            else if (ExtHeader!=IPV6_HDR_HOPBYHOP && ExtHeader!=IPV6_HDR_AUTH &&
                       ExtHeader!=IPV6_HDR_DST_OPTIONS && ExtHeader!=IPV6_HDR_DST_OPTIONS)
            {
                DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
                return;
            }
            pData += 1 + pOptions->m_HeaderExtLength;
            ExtHeader = pOptions->m_NextHeader;
        }
        pUdpHeader = (UdpHeader*)((UInt8*)pData);
     }

    // Check if a un-fragmented frame has the correct length
    FrameLength = DtUInt16ByteSwap(pUdpHeader->m_Length) +
                      DtPtrToUInt((UInt8*)((UInt8*)pUdpHeader - pRxData)) + 4; // +4 = CRC

    if (FrameLength > pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength&&
                                                                      pIpRxFragment==NULL)
    {        
        DtDbgOut(ERR, IP_RX, "UDP Length error. Packet too short. PortIndex=%d", 
                                                                  pIpPort->m_IpPortIndex);
        DT_ASSERT(FALSE);
        
        if (pIpRxFragment != NULL)
            DtaIpRxUserChRefDecr(pIpRxFragment);

        if (PacketSize < DTA_IPRX_MAX_PACKET_LENGTH)
        {
            DtDbgOut(ERR, IP_RX, "UDP length error!");
            DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
        }
        return;
    }

    // Calculate Dvb Length and offset
    DvbTotalLength = DtUInt16ByteSwap(pUdpHeader->m_Length) - sizeof(UdpHeader);

    // Store pre. calculation of DVB offset
    DvbOffset = (Int)(((UInt8*)pUdpHeader + sizeof(UdpHeader)) - pRxData);

    if (pIpRxFragment != NULL)
    {
        // Check UDP checksum
        UInt16 Checksum = DtaIpRxCalculateUdpChecksum(pIpHeaderV4, pUdpHeader,
                                                       pRxData+DvbOffset, DvbTotalLength);
        if (pUdpHeader->m_Checksum!=0 && Checksum!=pUdpHeader->m_Checksum)
        {
            DtDbgOut(ERR, IP_RX,"CHECKSUM ERROR, calculated: %x, received: %x", 
                                                        Checksum, pUdpHeader->m_Checksum);
            DtaIpRxUserChRefDecr(pIpRxFragment);
            return;
        }
    }

    SrcPort = DtUInt16ByteSwap(pUdpHeader->m_SourcePort);
    DstPort = DtUInt16ByteSwap(pUdpHeader->m_DestinationPort);

    // Get the RTP header
    pRtpHeader = (RtpHeader*)((UInt8*)pUdpHeader + sizeof(UdpHeader));
    RtpOffset = (Int)(((UInt8*)pRtpHeader) - pRxData);

    // Check if the complete (fragmented) packet is for one of our channels
    // For Type2, we already know it's for one or more of the RT Ip channels.
    DtMutexAcquire(&pIpPort->m_IpRxListenersMutex, -1);
    if (Type2)
    {
        UInt8*  pSrcAddress = (pIpHeaderV4!=NULL ? pIpHeaderV4->m_SrcAddress : 
                                                               pIpHeaderV6->m_SrcAddress);
        NumChannels = DtaIpRxGetChannelsForDVB(pIpPort, 
                                    pDmaRxHeader->m_RxHeaderV3.m_AddrMatching.m_AddrIdTag,
                                    pSrcAddress, SrcPort, pIpPort->m_pIpRxListeners,
                                    pIpPort->m_MaxNumListeners);
    } else {
        UInt8*  pSrcAddress = (pIpHeaderV4!=NULL ? pIpHeaderV4->m_SrcAddress : 
                                                               pIpHeaderV6->m_SrcAddress);
        UInt8*  pDstAddress = (pIpHeaderV4!=NULL ? pIpHeaderV4->m_DstAddress : 
                                                               pIpHeaderV6->m_DstAddress);
        NumChannels = DtaIpRxIsPacketForDVB(pIpUserChannels, pRtpHeader, pSrcAddress, 
                                   pDstAddress, SrcPort, DstPort, 
                                   MulticastPacket, pIpHeaderV4==NULL, VlanId,
                                   pIpPort->m_pIpRxListeners, pIpPort->m_MaxNumListeners);
        
    }
    if (NumChannels == 0)
    {
        DtMutexRelease(&pIpPort->m_IpRxListenersMutex);
        if (pIpRxFragment != NULL)
            DtaIpRxUserChRefDecr(pIpRxFragment);
        if (Type2)
            DtDbgOut(MAX, IP_RX, "No channels for TagId: %i!", 
                                   pDmaRxHeader->m_RxHeaderV3.m_AddrMatching.m_AddrIdTag);
        else
            DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
        return;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Check RTP Header -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    // To determine if the payload of a UDP packet is a RTP packet 
    // or if it is DVB data, we first check the RTP header values.
    // This is not a solid proof, but enough to do the job.
    if ((pRtpHeader->m_PayloadType!=RTP_PAYLOAD_MPEGII && 
              pRtpHeader->m_PayloadType!=RTP_PAYLOAD_FEC &&
              pRtpHeader->m_PayloadType!=RTP_PAYLOAD_SDI && 
              pRtpHeader->m_PayloadType!=RTP_PAYLOAD_FEC_SDI) || pRtpHeader->m_Version!=2)
{
        // Known payload types not found with RTPVersion of 2.
        // It's a UDP packet.
        ProtocolType = DTA_PROTO_UDP;
    } else {
        // RTP Packet with MPEG-II or FEC content received
        if (pIpRxFragment != NULL)
        {
            // We can not handle fragmented RTP packets
            // Discard packets
            DtaIpRxUserChRefDecrAll(pIpPort->m_pIpRxListeners, NumChannels);
            DtMutexRelease(&pIpPort->m_IpRxListenersMutex);
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
    
    if (ProtocolType==DTA_PROTO_RTP && pRtpHeader->m_PayloadType==RTP_PAYLOAD_SDI)
    {
        pHbrHeader = (HbrMediaPlHeader*) (pRxData + DvbOffset);
        DvbOffset += sizeof(HbrMediaPlHeader);
        DvbTotalLength -= sizeof(HbrMediaPlHeader);
        if (pHbrHeader->m_F == 0)
        {
            // No video source format present. Put packet to NDIS
            DtaIpRxUserChRefDecrAll(pIpPort->m_pIpRxListeners, NumChannels);
            DtMutexRelease(&pIpPort->m_IpRxListenersMutex);
            if (pIpRxFragment != NULL)
                DtaIpRxUserChRefDecr(pIpRxFragment);
            DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
            return;
            /*DvbOffset -= 4;
            DvbTotalLength += 4;
            DetVideoStd = DT_VIDSTD_UNKNOWN;
            FrameCount = 0;*/
        } else {
            UInt8  Frame = ((UInt8)pHbrHeader->VSF.m_FrameH<<4)|pHbrHeader->VSF.m_FrameL;
            DetVideoStd = DT_VIDSTD_UNKNOWN;
                
            switch(Frame)
            {
            case 0x10: DetVideoStd = DT_VIDSTD_525I59_94;break;
            case 0x11: DetVideoStd = DT_VIDSTD_625I50; break;
            }
            FrameCount = pHbrHeader->m_FRCount;
        }
        if (pHbrHeader->m_CfH!=0 || pHbrHeader->m_CfL!=0)
        {
            // Extra video timestamp included 
            DvbOffset += 4;
            DvbTotalLength -= 4;
        }
        if (pHbrHeader->m_Ext != 0)
        {
            // header is extended
            DvbOffset += 4*pHbrHeader->m_Ext;
            DvbTotalLength -= 4*pHbrHeader->m_Ext;
        }
    } else if (pRtpHeader->m_PayloadType == RTP_PAYLOAD_MPEGII)
        DetVideoStd = DT_VIDSTD_TS;
    
    //-.-.-.-.-.-.-.-.-.-.-.-.-.- CopyIpPacketsToUserChannels -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    PacketProcessed = FALSE;
    for (i=0; i<NumChannels; i++) 
    {
        Int  VlanIdCh;
        
        // The use count of the IpRx channel is already incremented,
        // so the structure is available
        pIpRxChannel = pIpPort->m_pIpRxListeners[i];
        pIpRxChannel->m_FragmentId = 0;
        pIpRxChannel->m_FragmentOffset = 0;

        // Check if packet is for correct VLAN
        if ((pIpRxChannel->m_IpParsMode & DTA_IP_RX_2022_7)!=0 && 
                                                            (pIpPort->m_IpPortIndex&1)!=0)
            VlanIdCh = pIpRxChannel->m_VlanId2;
        else
            VlanIdCh = pIpRxChannel->m_VlanId;

        if (VlanIdCh != VlanId)
        {
            // Decrement use count
            DtaIpRxUserChRefDecr(pIpRxChannel);
            continue;
        }
        PacketProcessed = TRUE;
        
        // Check if we are in Raw IP mode. 
        // If so, the complete packet must be copied 
        // and a header must be added
        if ((pIpRxChannel->m_RxMode & DT_RXMODE_MASK) == DT_RXMODE_IPRAW)
        {
            // Process the raw ip data (including extra bytes after the UDP payload if 
            // available).
            DtaIpRxProcessIpRawPayLoad(pIpRxChannel, pRxDataAligned, PacketSizeAligned, 
                                                         pDmaRxHeader->m_Timestamp, TRUE);
            // Decrement use count
            DtaIpRxUserChRefDecr(pIpRxChannel);
            continue;
        }
        
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Protocol UDP -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
        if (ProtocolType == DTA_PROTO_UDP) 
        {
            DtDbgOut(MAX, IP_RX, "Channel %d: UDP DVB Packet received. DVB-DataSize=%i",
                                            pIpRxChannel->m_ChannelIndex, DvbTotalLength);

            // It's a UDP packet with DVB content. 
            // If we are in double buffer mode we parse it like a RTP packet with packet
            // sorting otherwhise we copy it directly to the user buffer

            DT_ASSERT(*(pRxData + DvbOffset) == 0x47);

            pIpRxChannel->m_DetProtocol = DTA_PROTO_UDP;
            pIpRxChannel->m_DetFecNumColumns = 0;
            pIpRxChannel->m_DetFecNumRows = 0;
            pIpRxChannel->m_DetVidStd = DT_VIDSTD_TS;
            
            if (pIpRxChannel->m_VidStd!=DT_VIDSTD_UNKNOWN &&
                                                     pIpRxChannel->m_VidStd!=DT_VIDSTD_TS)
            {
                // Decrement use count, we don't want this format
                DtaIpRxUserChRefDecr(pIpRxChannel);
                continue;
            }

            DtaIpRxDoIpStatistics(pIpPort, pIpRxChannel, 
                        pIpPort->m_pDvcData->m_IpDevice.m_RefTimeStored, 
                        pDmaRxHeader, DtUInt16ByteSwap(pIpHeaderV4->m_Identification), 0);

            // We do not support doubly buffering with IpV6.
            if (((pIpRxChannel->m_IpParsMode & DTA_IP_RX_2022_7)!=0) && pIpHeaderV4!=NULL)
            {
                DtDbgOut(MAX, IP_RX_REC, "[%i]    PACKET SN:%d Time:%x\tLast: %d Time:%x", 
                         pIpPort->m_IpPortIndex, 
                         DtUInt16ByteSwap(pIpHeaderV4->m_Identification),
                         (UInt32)pDmaRxHeader->m_Timestamp, pIpRxChannel->m_RtpLastSeqNum,
                         pIpRxChannel->m_RtpLastTimestamp);
                // Don't use Rx timestamp. Otherwise identification field for sorting
                // does not work. We don't support high bitrates or long delays in that
                // case. So UDP is limited supported.
                DtaIpRxParsePayLoadRtp(pIpRxChannel, pDmaRxHeader, 
                                          DtUInt16ByteSwap(pIpHeaderV4->m_Identification),
                                          0, Size, DvbOffset, 0, 0);

                // Notify the recontructor
                if (pIpRxChannel->m_RxIncomingPackets)
                    pIpUserChannels->m_IpRxRtpPacketAvailable = TRUE;
                continue;
            } else {
                DtaIpRxProcessDvbPayLoad(pIpRxChannel, 
                       pRxData + DvbOffset, DvbTotalLength, pDmaRxHeader->m_Timestamp, 0);
            
                // Decrement use count
                DtaIpRxUserChRefDecr(pIpRxChannel);
                continue;
            }
        }
        
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Protocol RTP -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

        //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Payload MPEGII -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        if (pRtpHeader->m_PayloadType==RTP_PAYLOAD_MPEGII ||
                                             pRtpHeader->m_PayloadType == RTP_PAYLOAD_SDI)
        {
            DtDbgOut(MAX, IP_RX, "[%i]     PACKET SN:%d Time:%x\tLast:%d Time:%x", 
                 pIpPort->m_IpPortIndex, DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber),
                 DtUInt32ByteSwap(pRtpHeader->m_TimeStamp), pIpRxChannel->m_RtpLastSeqNum,
                 pIpRxChannel->m_RtpLastTimestamp);
            // It's a RTP packet with DVB content. Add to seq. buffer
            pIpRxChannel->m_DetProtocol = DTA_PROTO_RTP;
            pIpRxChannel->m_DetVidStd = DetVideoStd;

            if (pIpRxChannel->m_VidStd!=DT_VIDSTD_UNKNOWN &&
                                        pIpRxChannel->m_VidStd!=pIpRxChannel->m_DetVidStd)
            {
                DtDbgOut(MAX, IP_RX, 
                           "Channel %d: Wrong video standard. Expected:%xh, received:%xh",
                           pIpRxChannel->m_ChannelIndex, pIpRxChannel->m_VidStd, 
                           pIpRxChannel->m_DetVidStd);
                // Decrement use count, we don't want this format
                DtaIpRxUserChRefDecr(pIpRxChannel);
                continue;
            }

            DtaIpRxDoIpStatistics(pIpPort, pIpRxChannel, 
                             pIpPort->m_pDvcData->m_IpDevice.m_RefTimeStored,
                             pDmaRxHeader, DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber),
                             DtUInt32ByteSwap(pRtpHeader->m_TimeStamp));
#ifdef _DEBUG
            /*if (((DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber)+1)%100 == 0) ||
                ((DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber)+1)%101 == 0))
            {
                DtDbgOut(ERR, IP_RX_REC, 
               "Channel %d: RD TST:SKIP PACKET, SeqNum:%i",
                         pIpRxChannel->m_ChannelIndex, 
                         DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber));
                DtaIpRxUserChRefDecr(pIpRxChannel);
                
                continue;
            }*/

#endif
            DT_ASSERT(pDmaRxHeader->m_Timestamp != 0);
            DtaIpRxParsePayLoadRtp(pIpRxChannel, pDmaRxHeader,
                                           DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber), 
                                           DtUInt32ByteSwap(pRtpHeader->m_TimeStamp),
                                           Size, DvbOffset, RtpOffset, FrameCount);
            
            // Notify the recontructor
            if (pIpRxChannel->m_RxIncomingPackets)
                pIpUserChannels->m_IpRxRtpPacketAvailable = TRUE;
            continue;
        }

        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Payload FEC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        if (pRtpHeader->m_PayloadType==RTP_PAYLOAD_FEC || 
                                           pRtpHeader->m_PayloadType==RTP_PAYLOAD_FEC_SDI) 
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
            if (pIpRxChannel->m_VidStd!=DT_VIDSTD_UNKNOWN &&
                                        pIpRxChannel->m_VidStd!=pIpRxChannel->m_DetVidStd)
            {
                // Decrement use count, we don't want this format
                DtDbgOut(MAX, IP_RX, 
                              "Channel %d: FEC packet received, but wrong video standard."
                              "Expected:%xh, received:%xh", pIpRxChannel->m_ChannelIndex,
                              pIpRxChannel->m_VidStd, pIpRxChannel->m_DetVidStd);
                DtaIpRxUserChRefDecr(pIpRxChannel);
                continue;
            }
            if (!DtaIpRxParsePayLoadFec(pIpRxChannel, pDmaRxHeader,
                                          DtUInt16ByteSwap(pRtpHeader->m_SequenceNumber),
                                          DtUInt32ByteSwap(pRtpHeader->m_TimeStamp),
                                          Size, DvbOffset, RtpOffset, PacketSize,
                                          pIpPort, DstPort, 
                                          pRtpHeader->m_PayloadType==RTP_PAYLOAD_FEC_SDI))
            {
                DtaIpRxUserChRefDecrAll(pIpPort->m_pIpRxListeners, NumChannels);
                if (pIpRxFragment != NULL)
                    DtaIpRxUserChRefDecr(pIpRxFragment);

                DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
                return;
            }

            // Notify the reconstructor
            if (pIpRxChannel->m_RxIncomingPackets)
                pIpUserChannels->m_IpRxRtpPacketAvailable = TRUE;

            continue;
        }
        
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Unknown Packet Type -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        DT_ASSERT(FALSE);
        // Unknown packet type. Maybe corrupted packet and checksum check is off??
        DtDbgOut(ERR, IP_RX, "Channel %d: UNKNOWN PACKET OR CORRUPTED PACKET??.",
                                                            pIpRxChannel->m_ChannelIndex);

        // Decrement ref count for all channels
        DtaIpRxUserChRefDecrAll(pIpPort->m_pIpRxListeners, NumChannels);
        DtMutexRelease(&pIpPort->m_IpRxListenersMutex);
        if (pIpRxFragment != NULL)
            DtaIpRxUserChRefDecr(pIpRxFragment);

        DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
        return;
    } // end for

    DtMutexRelease(&pIpPort->m_IpRxListenersMutex);

    if (NumChannels!=0 && !PacketProcessed)
    {
        // Incorrect Vlan, send to network driver
        DtaIpRxSendPacketToNwDriver(pIpPort, pDmaRxHeader, pRxData, PacketSize);
    }

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
// This function is not used for RT RX type2
//
UInt8*  CheckAndParseData(DtaIpPort* pIpPort, UInt8* pPrevData, UInt* pPrevDataSize,
                                                        UInt8* pRxData, UInt* pRxDataSize)
{
    UInt  NumBytesToChk;
    UInt8*  pDataToChk;
    DtaDmaRxHeader*  pDmaRxHeader;
    UInt  BytesToCopy;
    Bool  NrtRxType2 = (pIpPort->m_PortType == DTA_IPPORT_TYPE2);
    
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
            DtDbgOut(MAX, IP_RX, "Partly packet stored for second time."
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
    
    DtaIpRxParsePacket(pIpPort, pDataToChk, BytesToCopy+*pPrevDataSize, NrtRxType2);
    
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
// IP Rx worker thread: Type1: RT/NRT
//                      Type2: NRT only
//
void  DtaIpRxWorkerThread(DtThread* pThread, void* pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIpPort*  pIpPort = pContext;
    UInt8*  pPrevData = NULL;
    UInt8*  pRxData = NULL;
    DtaDmaRxHeader*  pRxHeader;
    Bool  StopThread = FALSE;
    DtEvent*  pDmaDoneEvent;
    UInt  NumBytesRx = 0;
    UInt  PrevDataLength = 0;
    UInt  BytesToCopy;
    DmaChannel*  pDmaCh = NULL;
    DtaIpUserChannels*  pIpUserChannels = NULL;
    PPBuffer*  pPPBuffer = NULL;
    
    DtDbgOut(MAX, IP_RX, "Start");

    pPrevData = (UInt8*)DtMemAllocPool(DtPoolNonPaged, 2048, IPRX_TAG);
    if (pPrevData == NULL)
    {
        DtDbgOut(ERR, IP_RX, "Not enough memory to allocate data buffer");
        return;
    }

    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
    {
        pDmaCh = &pIpPort->m_IpPortType1.m_Rx.m_DmaChannel;
        pPPBuffer = &pIpPort->m_IpPortType1.m_RxPPBuffer;
    }
    else  if (pIpPort->m_PortType == DTA_IPPORT_TYPE2) 
    {
        pDmaCh = &pIpPort->m_IpPortType2.m_RxNrt.m_DmaChannel;
        pPPBuffer = &pIpPort->m_IpPortType2.m_RxNrtPPBuffer;
    } else 
        DT_ASSERT(FALSE);

    pDmaDoneEvent = DtaDmaGetDoneEvent(pDmaCh);

    // Start DMA transfer
    Status = DtaPPBufferTransferData(pPPBuffer);
    DT_ASSERT(DT_SUCCESS(Status));

    while (!StopThread)
    {
        // Wait for DMA done or stop
        Status = DtThreadWaitForStopOrEvent(pThread, pDmaDoneEvent);
        if (Status==DT_STATUS_CANCELLED || DtaDmaIsAbortActive(pDmaCh))
        {
            StopThread = TRUE;
            continue;
        }

        // DMA ready, start new transfer of other buffer
        Status = DtaPPBufferTransferData(pPPBuffer);
        DT_ASSERT(DT_SUCCESS(Status));

        // Get number of bytes Rx'ed of previous buffer
        NumBytesRx = DtaPPBufferGetTransferSize(pPPBuffer);
        
        // Check received data.
        pRxData = (UInt8*)DtaPPBufferReadDataGetBuffer(pPPBuffer);
        
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
                        DtDbgOut(MAX, IP_RX, "Partly packet stored for second time."
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
                    pIpPort->m_NumIncorrectDmaRxHeaders++;
                    // Skip all previous data, and take only the new data
                    PrevDataLength = 0;
                    NumBytesRx = DtaPPBufferGetTransferSize(pPPBuffer);
                    pRxData = DtaPPBufferReadDataGetBuffer(pPPBuffer);
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
                DtDbgOut(ERR, IP_RX, "[%i] INCORRECT DMA header RT/NRT.", 
                                                                  pIpPort->m_IpPortIndex);
                pIpPort->m_NumIncorrectDmaRxHeaders++;

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

        // Only for RT data.
        if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        {
            // Finished parsing data
            // If packets are found process them in the reconstructor
            pIpUserChannels = &pIpPort->m_pDvcData->m_IpDevice.m_IpUserChannels;
            if (pIpUserChannels->m_IpRxRtpPacketAvailable)
            {
                pIpUserChannels->m_IpRxRtpPacketAvailable = FALSE;
                DtEventSet(&pIpUserChannels->m_IpRxRtpPacketAvailEvent);
            }
        }

        // Release buffer
        DtaPPBufferReadDataFinished(pPPBuffer);
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

//.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtCheckHeaderAndParseDataType2 -.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxRtCheckHeaderAndParseDataType2(DtaIpPort* pIpPort, UInt8* pRxData, 
                                                                          UInt RxDataSize)
{
    DtaIpUserChannels*  pIpUserChannels = NULL;
    DtaDmaRxHeader*  pRxHeader;
    UInt  BytesToCopy;
    DT_ASSERT(pIpPort->m_PortType == DTA_IPPORT_TYPE2);
    
    pIpUserChannels = &pIpPort->m_pDvcData->m_IpDevice.m_IpUserChannels;
    // Parse the DMA data
    while (RxDataSize != 0)
    {
        if (RxDataSize < sizeof(DtaDmaRxHeader))
        {
            // Not a complete packet available. This should not happen!
            DT_ASSERT(FALSE);
            DtDbgOut(ERR, IP_RX, "[%i] Partly DMA header received. Error.", 
                                                                  pIpPort->m_IpPortIndex);
            pIpPort->m_NumIncorrectDmaRxHeaders++;
            break;
        }
        pRxHeader = (DtaDmaRxHeader*)pRxData;
        if (!DtaIpRxIsDmaHeaderCorrect(pRxHeader))
        {
            // Error in DMA header
            DtDbgOut(ERR, IP_RX, "[%i] INCORRECT DMA header RT.", pIpPort->m_IpPortIndex);
            pIpPort->m_NumIncorrectDmaRxHeaders++;

            // Find packet header
            while (RxDataSize>=sizeof(DtaDmaRxHeader) && 
                                !DtaIpRxIsDmaHeaderCorrect(pRxHeader))
            {
                pRxData++;
                pRxHeader = (DtaDmaRxHeader*)pRxData;
                RxDataSize--;
            }

            // Check if we found a correct packet header
            if (RxDataSize < sizeof(DtaDmaRxHeader))
            {
                DtDbgOut(ERR, IP_RX, "[%i] No correct DMA header found after incorrect"
                                                   " DMA header", pIpPort->m_IpPortIndex);
                break;
            }
        }
        if (RxDataSize < pRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength +
                                                                   sizeof(DtaDmaRxHeader))
        {
            DtDbgOut(ERR, IP_RX, "[%i] No complete data packet received. Expected: %u."
                            " Received: %u", pIpPort->m_IpPortIndex,
                            (UInt)pRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength +
                            sizeof(DtaDmaRxHeader), RxDataSize);
            break;
        }
        if (pRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength == 0)
        {
            // End of chain marker. Should not happen anymore
            DtDbgOut(ERR, IP_RX, "[%i] End of chain", pIpPort->m_IpPortIndex);
            DT_ASSERT(FALSE);
            pIpPort->m_NumIncorrectDmaRxHeaders++;
            break;
        }

        // Complete packet received
        BytesToCopy = pRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength +
                                                                   sizeof(DtaDmaRxHeader);
        //DtaIpRxParsePacket(pIpPort, pRxData, BytesToCopy);
        if (pRxHeader->m_RxHeaderV3.m_AddrMatching.m_AddrIdTagFound == 1)
        {
            DT_ASSERT(pRxHeader->m_RxHeaderV3.m_AddrMatching.m_RealTime == 1);
            // It's for the RealTime Receive
            DtaIpRxParsePacket(pIpPort, pRxData, BytesToCopy, FALSE);
            
        } else {
            // Copy to NDIS
            DtaIpRxSendPacketToNwDriver(pIpPort, pRxHeader,
                              pRxData+sizeof(DtaDmaRxHeader), 
                              pRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength - 4);
        }
    
        // Round to DWORDs
        if ((BytesToCopy % 4) != 0)
            BytesToCopy += (4 - (BytesToCopy % 4));
        pRxData+= BytesToCopy;
        if (RxDataSize <= BytesToCopy)
            RxDataSize = 0;
        else
            RxDataSize-= BytesToCopy;
    }

    // Finished parsing data
    // If packets are found process them in the reconstructor
    if (pIpUserChannels->m_IpRxRtpPacketAvailable)
    {
        pIpUserChannels->m_IpRxRtpPacketAvailable = FALSE;
        DtEventSet(&pIpUserChannels->m_IpRxRtpPacketAvailEvent);
    }
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRtWorkerThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// IP Rx worker thread: Type2: RT only
//
void  DtaIpRxRtWorkerThread(DtThread* pThread, void* pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIpPort*  pIpPort[2];
    DtaIpPortType2*  pIpPortT2[2];
    
    UInt8*  pRxData = NULL;
    Bool  StopThread = FALSE;
    DtEvent*  pDmaDoneEvent;
    UInt  NumBytesRx = 0;
    DmaChannel*  pDmaCh = NULL;
    PPBuffer*  pPPBuffer = NULL;
    Bool  DataAvailable = FALSE;
    UInt  TransferSize[2]= {0,0};

    DtDbgOut(MAX, IP_RX, "Start");

    pIpPort[0] = (DtaIpPort*)pContext;
    pIpPort[1] = 
              &pIpPort[0]->m_pDvcData->m_IpDevice.m_pIpPorts[pIpPort[0]->m_IpPortIndex+1];
    pIpPortT2[0] = &pIpPort[0]->m_IpPortType2;
    pIpPortT2[1] = &pIpPort[1]->m_IpPortType2;
        
    DT_ASSERT(pIpPort[0]->m_PortType == DTA_IPPORT_TYPE2);

    pDmaCh = &pIpPort[0]->m_IpPortType2.m_RxRt.m_DmaChannel;
    pPPBuffer = &pIpPort[0]->m_IpPortType2.m_RxRtPPBuffer;
    
    pDmaDoneEvent = DtaDmaGetDoneEvent(pDmaCh);

    while (!StopThread)
    {
        // If no data is transferd in previous loop, we have to wait here for new data
        if (!DataAvailable)
        {
            // Wait for slice available port x
            while (pIpPortT2[0]->m_RxRtSliceWriteIndex == 
                                        pIpPortT2[0]->m_RxRtSliceReadIndex && !StopThread)
            {
                Status = DtThreadWaitForStopOrEvent(pThread, 
                                                         &pIpPortT2[0]->m_RxRtSliceEvent);
                if (Status == DT_STATUS_CANCELLED)
                    StopThread = TRUE;
            }
            if (StopThread)
                continue;

            TransferSize[0] = 
                        pIpPortT2[0]->m_RxRtSliceSize[pIpPortT2[0]->m_RxRtSliceReadIndex];
            if (TransferSize[0] != 0)
            {
                // Start DMA transfer port 0
                Status = DtaPPBufferTransferDataContext(pPPBuffer, pIpPort[0]);
                DT_ASSERT(DT_SUCCESS(Status));
            } else {
                // Increment read index
                if (pIpPortT2[0]->m_RxRtSliceReadIndex+1 == DTA_IPRX_MAX_SLICES)
                    pIpPortT2[0]->m_RxRtSliceReadIndex = 0;
                else
                    pIpPortT2[0]->m_RxRtSliceReadIndex++;
            }
        }
        if (TransferSize[0] != 0)
        {
            // Wait for DMA done or stop port 0
            Status = DtThreadWaitForStopOrEvent(pThread, pDmaDoneEvent);
            if (Status==DT_STATUS_CANCELLED || DtaDmaIsAbortActive(pDmaCh))
            {
                StopThread = TRUE;
                continue;
            }
        
            // Get number of bytes Rx'ed of previous buffer
            NumBytesRx = DtaPPBufferGetTransferSize(pPPBuffer);
            pRxData = (UInt8*)DtaPPBufferReadDataGetBuffer(pPPBuffer);
        }

        // Wait for slice available port 1
        while (pIpPortT2[1]->m_RxRtSliceWriteIndex == 
                                        pIpPortT2[1]->m_RxRtSliceReadIndex && !StopThread)
        {
            Status = DtThreadWaitForStopOrEvent(pThread, &pIpPortT2[0]->m_RxRtSliceEvent);
            if (Status==DT_STATUS_CANCELLED)
                StopThread = TRUE;
        }
        if (StopThread)
            continue;

        TransferSize[1] = 
                        pIpPortT2[1]->m_RxRtSliceSize[pIpPortT2[1]->m_RxRtSliceReadIndex];
        if (TransferSize[1] != 0)
        {
            // Start DMA transfer port1
            Status = DtaPPBufferTransferDataContext(pPPBuffer, pIpPort[1]);
            DT_ASSERT(DT_SUCCESS(Status));
        } else {
            // Increment read index
            if (pIpPortT2[1]->m_RxRtSliceReadIndex+1 == DTA_IPRX_MAX_SLICES)
                pIpPortT2[1]->m_RxRtSliceReadIndex = 0;
            else
                pIpPortT2[1]->m_RxRtSliceReadIndex++;
        }

        if (TransferSize[0] != 0)
        {
            DtaIpRxRtCheckHeaderAndParseDataType2(pIpPort[0], pRxData, NumBytesRx);
        
            // Release buffer
            DtaPPBufferReadDataFinished(pPPBuffer);
        }

        if (TransferSize[1] != 0)
        {
            // Wait for DMA done or stop port 1
            Status = DtThreadWaitForStopOrEvent(pThread, pDmaDoneEvent);
            if (Status==DT_STATUS_CANCELLED || DtaDmaIsAbortActive(pDmaCh))
            {
                StopThread = TRUE;
                continue;
            }

            // Get number of bytes Rx'ed of port 1
            NumBytesRx = DtaPPBufferGetTransferSize(pPPBuffer);
            pRxData = (UInt8*)DtaPPBufferReadDataGetBuffer(pPPBuffer);
        }

        DataAvailable = FALSE;
        if (pIpPortT2[0]->m_RxRtSliceWriteIndex != 
                                           pIpPort[0]->m_IpPortType2.m_RxRtSliceReadIndex)
        {
            DataAvailable = TRUE;

            // Check transfer size
             TransferSize[0] = 
                        pIpPortT2[0]->m_RxRtSliceSize[pIpPortT2[0]->m_RxRtSliceReadIndex];
            if (TransferSize[0] != 0)
            {
                // Start DMA transfer port 0
                Status = DtaPPBufferTransferDataContext(pPPBuffer, pIpPort[0]);
                DT_ASSERT(DT_SUCCESS(Status));
            } else {
                // Increment read index
                if (pIpPortT2[0]->m_RxRtSliceReadIndex+1 == DTA_IPRX_MAX_SLICES)
                    pIpPortT2[0]->m_RxRtSliceReadIndex = 0;
                else
                    pIpPortT2[0]->m_RxRtSliceReadIndex++;
            }

        }
        if (TransferSize[1] != 0)
        {
            DtaIpRxRtCheckHeaderAndParseDataType2(pIpPort[1], pRxData, NumBytesRx);
        
            // Release buffer
            DtaPPBufferReadDataFinished(pPPBuffer);
        }
    }

    DtDbgOut(MAX, IP_RX, "Abort DMA");

    DtaDmaAbortDma(pDmaCh);

    while (!DtaDmaIsReady(pDmaCh))
        DtSleep(10);

    // We have to wait until the thread received a stop command.
    DtThreadWaitForStop(pThread);

    DtDbgOut(MAX, IP_RX, "Exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetSequenceNumberGap -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt16  DtaIpRxGetSequenceNumberGap(UInt16 SequenceNum1, UInt16 SequenceNum2)
{
    if (SequenceNum1 > SequenceNum2)
        return 0xffff - SequenceNum1 + SequenceNum2 + 1;

    return SequenceNum2 - SequenceNum1;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetSequenceNumberGap2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// 1/16 sec = 5625 ticks 90khz clock
// 1/16 sec = 1687500 ticks 27Mhz clock
// 1/16 sec:
// = 6 packets 1Mbit, = 5936 packets 1Gbit, =59366 packets 10Gbit
//
// SDI = 270MBit is around 1100 ticks dT at 27Mhz clock (27e6*11000/270e6)
//
UInt32  DtaIpRxGetSequenceNumberGap2(UInt16 SequenceNum1, UInt32 RtpTimestamp1,
                      UInt16 SequenceNum2, UInt32 RtpTimestamp2, Bool Clk90k, Int MaxRate)
{
    UInt32  dT;
    UInt32  NumPackets;
    UInt32 Gap;

    Gap = DtaIpRxGetSequenceNumberGap(SequenceNum1, SequenceNum2);
    if (RtpTimestamp1 == RtpTimestamp2)
        return Gap;
    if (DtaIpRxRtpIsTimestampLess(RtpTimestamp2, RtpTimestamp1))
    {
        // Message is no error when removing FEC packets
        DtDbgOut(ERR, IP_RX, "ERR RtpTimestamp2(%08xh)<RtpTimestamp1(%08xh)", 
                                                            RtpTimestamp2, RtpTimestamp1);
        
        return 0x1ffff;   // old data
    }
    // Calculate number of wraps
    dT = DtaIpRxRtpGetTimestampDelta(RtpTimestamp1, RtpTimestamp2);
    if (Clk90k)
    {
        // If MaxBitrate is set, we can optimise the check for lower and higher bitrates
        if (MaxRate!=0 && MaxRate<10000000)
        {
            if (dT>(9557*5625) && Gap<0x1000)
                Gap+= 0xFFFF;
        } else if (MaxRate!=0 && MaxRate>270000000)
        {
            if (dT>(10*5625) && Gap<0x1000)
                Gap+= 0xFFFF;
        } else // If less, we never could get a wrap for 270Mbit stream and >10Mb.
            if (dT>(4*11*5625) && Gap<0x1000)
                Gap+= 0xffff;
    } else {
        NumPackets = dT/1100;
        if (NumPackets>0xe000 && Gap<0x1000)
            Gap+= 0xffff;
    }
    return Gap;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxIsNextPacketAvail -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// 1/16 sec = 5625 ticks 90khz clock
// 1/16 sec = 1687500 ticks 27Mhz clock
// 1/16 sec:
// = 6 packets 1Mbit, = 5936 packets 1Gbit, =59366 packets 10Gbit
//
// SDI = 270MBit is around 1100 ticks dT at 27Mhz clock (27e6*11000/270e6)
//
Bool DtaIpRxIsNextPacketAvail(UInt16 SequenceNum1, UInt32 RtpTimestamp1,
                                   UInt16 SequenceNum2, UInt32 RtpTimestamp2, Bool Clk90k)
{
    UInt32  dT = DtaIpRxRtpGetTimestampDelta(RtpTimestamp1, RtpTimestamp2);
    if (Clk90k)
        return  (SequenceNum2==SequenceNum1+1) && (dT < 5625);
    return  (SequenceNum2==SequenceNum1+1) && (dT < 1687500);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxGetMaxOutOfSync -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt16  DtaIpRxGetMaxOutOfSync(UserIpRxChannel* pIpRxChannel) 
{
    Int  MaxNumRtpDvbPackets = MAX_NUM_RTP_PACKETS_TS;
    if (pIpRxChannel->m_VidStd != DT_VIDSTD_TS)
        MaxNumRtpDvbPackets = MAX_NUM_RTP_PACKETS_SDI;

    if (pIpRxChannel->m_DetFecNumColumns!=0 && pIpRxChannel->m_DetFecNumRows!=0)
        return MAX_OUTOFSYNC_SEQ_NUM + 
                         pIpRxChannel->m_DetFecNumColumns * pIpRxChannel->m_DetFecNumRows;
    if (pIpRxChannel->m_DetFecNumColumns !=0)
        return MAX_OUTOFSYNC_SEQ_NUM + pIpRxChannel->m_DetFecNumColumns;
    return MAX_OUTOFSYNC_SEQ_NUM + MaxNumRtpDvbPackets;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxProcessRtpDvbPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpRxProcessRtpDvbPacket(UserIpRxChannel* pIpRxChannel, UInt16* pLastSeqNum, 
                                           UInt32* pLastTimestamp, UInt64* pRefTimeStored)
{
    DtaDmaRxHeader*  pDmaRxHeader;
    UInt  MaxOutOfSync;
    UInt  MinPacketsInBuffer;
    RtpListEntry*  pRtpListEntry = NULL;
    RtpListEntry*  pRtpLastListEntry = NULL;
    UInt  Gap;
    UInt  NumPackets = 1;
    UInt16  LastSeqNum;
    UInt32  LastTimestamp;
    Bool  PacketTooLongInBuf;

    if (pIpRxChannel->m_MinPacketDelay==0 && pIpRxChannel->m_MaxPacketOutOfSync==0)
    {
        if (pIpRxChannel->m_IpParsMode == DTA_IP_RX_2022_7) 
        {
            MaxOutOfSync = 200;
            if (pIpRxChannel->m_FecMode == DTA_FEC_DISABLE) 
                MinPacketsInBuffer = 0;
            else
                MinPacketsInBuffer = 3 * DtaIpRxGetMaxOutOfSync(pIpRxChannel);
        } else if (pIpRxChannel->m_FecMode == DTA_FEC_DISABLE) 
        {
            MaxOutOfSync = MAX_OUTOFSYNC_SEQ_NUM;
            MinPacketsInBuffer = 0;
        } else {
            MaxOutOfSync = 3 * DtaIpRxGetMaxOutOfSync(pIpRxChannel);
            // Is a Fec row + column packet received
            MinPacketsInBuffer = MaxOutOfSync;
        }
    } else 
    {
        MinPacketsInBuffer = pIpRxChannel->m_MinPacketDelay;
        MaxOutOfSync = pIpRxChannel->m_MaxPacketOutOfSync;
    }
#ifdef DEBUG
    if (pIpRxChannel->m_DetFecNumColumns != 0)
    { 
        Int NumPkts = pIpRxChannel->m_DetFecNumColumns;
        if (pIpRxChannel->m_DetFecNumRows != 0)
        {
            NumPkts *= pIpRxChannel->m_DetFecNumRows;
        }
        DT_ASSERT(MinPacketsInBuffer >= (MAX_OUTOFSYNC_SEQ_NUM+NumPkts));
    }
#endif
    while(TRUE)
    {
        DtSpinLockAcquire(&pIpRxChannel->m_RtpListSpinLock);
        if (pRtpListEntry != NULL)
        {   
            RtpListEntry*  pRtpListEntryTmp = DtContainingRecord(
                   DtListNextGet(&pIpRxChannel->m_RtpDvbList), RtpListEntry, m_ListEntry);
                
            // We don't need this packet anymore, add to empty List
            // It's possible that in the mean time new packets are inserted before the
            // handled entry. We need to remove all these packets, because these packets
            // are too late.
            while (pRtpListEntry!=pRtpListEntryTmp && 
                                              !DtIsListEmpty(&pIpRxChannel->m_RtpDvbList))
            {
                DtHeadListRemove(&pIpRxChannel->m_RtpDvbList);
                DtDbgOut(ERR, IP_RX, "Remove before: Sn: %u pEntry:%p pRtpList:%p", 
                                           pRtpListEntryTmp->m_RtpSequenceNumber, 
                                           pRtpListEntryTmp, &pIpRxChannel->m_RtpDvbList);
                
                if (pIpRxChannel->m_pRtpAvailLookup1!=NULL && 
                   pIpRxChannel->m_pRtpAvailLookup1[pRtpListEntryTmp->m_RtpSequenceNumber]
                                                       ==pRtpListEntryTmp->m_RtpTimestamp)
                    pIpRxChannel->m_pRtpAvailLookup1
                                              [pRtpListEntryTmp->m_RtpSequenceNumber] = 0;
                else if (pIpRxChannel->m_pRtpAvailLookup2!=NULL && 
                   pIpRxChannel->m_pRtpAvailLookup2[pRtpListEntryTmp->m_RtpSequenceNumber]
                                                       ==pRtpListEntryTmp->m_RtpTimestamp)
                    pIpRxChannel->m_pRtpAvailLookup2
                                              [pRtpListEntryTmp->m_RtpSequenceNumber] = 0;
                else if (pIpRxChannel->m_pRtpAvailLookup1!=NULL && 
                                                   pIpRxChannel->m_pRtpAvailLookup2!=NULL)
                    DT_ASSERT(FALSE);
                DtaIpRxRtpMoveEntryToEmptyListUnsafe(pIpRxChannel, pRtpListEntryTmp);
                pRtpListEntryTmp = DtContainingRecord(
                   DtListNextGet(&pIpRxChannel->m_RtpDvbList), RtpListEntry, m_ListEntry);
            }
            DT_ASSERT(!DtIsListEmpty(&pIpRxChannel->m_RtpDvbList));

            // Just to be sure...should not happen.
            if (!DtIsListEmpty(&pIpRxChannel->m_RtpDvbList))
            {
                // Entry still in list. Remove.
                DtHeadListRemove(&pIpRxChannel->m_RtpDvbList);
                if (pIpRxChannel->m_pRtpAvailLookup1!=NULL && 
                      pIpRxChannel->m_pRtpAvailLookup1[pRtpListEntry->m_RtpSequenceNumber]
                                                          ==pRtpListEntry->m_RtpTimestamp)
                    pIpRxChannel->m_pRtpAvailLookup1[pRtpListEntry->m_RtpSequenceNumber] 
                                                                                      = 0;
                else if (pIpRxChannel->m_pRtpAvailLookup2!=NULL && 
                      pIpRxChannel->m_pRtpAvailLookup2[pRtpListEntry->m_RtpSequenceNumber]
                                                          ==pRtpListEntry->m_RtpTimestamp)
                    pIpRxChannel->m_pRtpAvailLookup2[pRtpListEntry->m_RtpSequenceNumber] 
                                                                                      = 0;
                else if (pIpRxChannel->m_pRtpAvailLookup1!=NULL && 
                                                   pIpRxChannel->m_pRtpAvailLookup2!=NULL)
                {
                    DT_ASSERT(FALSE);
                    DtDbgOut(ERR, IP_RX, "Seq not in lookup. : Seq.Nr.:%i Timestamp:%x" 
                     " L1:%x L2:%x", pRtpListEntry->m_RtpSequenceNumber, 
                     pRtpListEntry->m_RtpTimestamp, 
                     pIpRxChannel->m_pRtpAvailLookup1[pRtpListEntry->m_RtpSequenceNumber], 
                     pIpRxChannel->m_pRtpAvailLookup2[pRtpListEntry->m_RtpSequenceNumber]
                                                                                        );
                }
                DtaIpRxRtpMoveEntryToEmptyListUnsafe(pIpRxChannel, pRtpListEntry);
            }
        }

        if (!DtIsListEmpty(&pIpRxChannel->m_RtpDvbList))
        {
            // Let the entry stay in the list, but get the record
            pRtpListEntry = DtContainingRecord(DtListNextGet(&pIpRxChannel->m_RtpDvbList),
                                                               RtpListEntry, m_ListEntry);
        } else {
            
            // List is empty, reset state
            if (!pIpRxChannel->m_RtpFirstPacket)
            {
                pIpRxChannel->m_ResetPacketsPending = FALSE;
                pIpRxChannel->m_SdiNumBytesCopied = -1; // Let SDI format be redetected
                DtaIpRxRtpListsResetUnsafe(pIpRxChannel);
                DtDbgOut(ERR, IP_RX, "Channel %d: Buffer underflow."
                                      " Reset RTP buffers", pIpRxChannel->m_ChannelIndex);
            }
            DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);
            break;
        }

        // Retrieve the last received sequence number
        pRtpLastListEntry = DtContainingRecord(DtListPrevGet(&pIpRxChannel->m_RtpDvbList), 
                                                               RtpListEntry, m_ListEntry);

        if (pLastSeqNum != NULL)
        {
            LastSeqNum = *pLastSeqNum;
            LastTimestamp = *pLastTimestamp;
            DtDbgOut(MAX, IP_RX_REC,
                              "Channel %d: RtpDvbPacket1. LastSeqNum:%i,LastTimestamp:%x",
                              pIpRxChannel->m_ChannelIndex, LastSeqNum, LastTimestamp);
        } else {
            LastSeqNum = pRtpLastListEntry->m_RtpSequenceNumber;
            LastTimestamp = pRtpLastListEntry->m_RtpTimestamp;
            DtDbgOut(MAX, IP_RX_REC,
                              "Channel %d: RtpDvbPacket2. LastSeqNum:%i,LastTimestamp:%x",
                              pIpRxChannel->m_ChannelIndex, LastSeqNum, LastTimestamp);
        }

        Gap = DtaIpRxGetSequenceNumberGap2(pRtpListEntry->m_RtpSequenceNumber, 
                     pRtpListEntry->m_RtpTimestamp, LastSeqNum, LastTimestamp, 
                     pIpRxChannel->m_DetVidStd==DT_VIDSTD_TS, pIpRxChannel->m_MaxBitrate);

        // Minimal MinPacketsInBuffer packets in buffer for reconstruction
        PacketTooLongInBuf = FALSE;
        DT_ASSERT(((DtaDmaRxHeader*)(pIpRxChannel->m_pRtpBuffer + 
                                (pRtpListEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH)))->m_Timestamp != 0);
        if (/*!pIpRxChannel->m_RtpFirstPacket && */pRtpListEntry->m_InUse==0)
        {
            pDmaRxHeader = (DtaDmaRxHeader*)(pIpRxChannel->m_pRtpBuffer + 
                                (pRtpListEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));
            if (*pRefTimeStored>pDmaRxHeader->m_Timestamp && 
                (*pRefTimeStored - pDmaRxHeader->m_Timestamp) > DTA_IPRX_RTP_MAXINBUFTIME)
            {
                // Packet already more then x seconds in buffer. 
                // We have to send it to the user.
                PacketTooLongInBuf = TRUE;
                DtDbgOut(ERR, IP_RX,
                    "Channel %d: Packet too long in buffer. Send to user. SN:%i"
                            " RxTime: %I64x CurTime: %I64x", pIpRxChannel->m_ChannelIndex,
                            pRtpListEntry->m_RtpSequenceNumber,
                            pDmaRxHeader->m_Timestamp, *pRefTimeStored);
            }
        }
        if (PacketTooLongInBuf || Gap>MaxOutOfSync || 
                  (DtaIpRxIsNextPacketAvail(pIpRxChannel->m_RtpLastSeqNum, 
                   pRtpLastListEntry->m_RtpTimestamp, pRtpListEntry->m_RtpSequenceNumber,
                   pRtpListEntry->m_RtpTimestamp, pIpRxChannel->m_DetVidStd==DT_VIDSTD_TS)
                   && !pIpRxChannel->m_RtpFirstPacket && 
                   pRtpListEntry->m_InUse==0 && Gap>=MinPacketsInBuffer))
        {
            UInt16  PrevLastSeqNum = pIpRxChannel->m_RtpLastSeqNum;
            UInt32  PrevLastTimestamp = pIpRxChannel->m_RtpLastTimestamp;
            pIpRxChannel->m_RtpLastSeqNum = pRtpListEntry->m_RtpSequenceNumber;
            pIpRxChannel->m_RtpLastTimestamp = pRtpListEntry->m_RtpTimestamp;
            
            DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);

            // Packet ready for user buffer
            // Count number of lost packets
            if (!pIpRxChannel->m_RtpFirstPacket) 
            {
                DtSpinLockAcquire(&pIpRxChannel->m_StatisticsSpinLock);
                NumPackets = DtaIpRxGetSequenceNumberGap2(PrevLastSeqNum,
                                    PrevLastTimestamp, pRtpListEntry->m_RtpSequenceNumber,
                                    pRtpListEntry->m_RtpTimestamp, 
                                    pIpRxChannel->m_DetVidStd==DT_VIDSTD_TS,
                                    pIpRxChannel->m_MaxBitrate);
                pIpRxChannel->m_NumPacketsNotReconstructed += NumPackets-1;
                pIpRxChannel->m_TotNumPackets += NumPackets;
                DtSpinLockRelease(&pIpRxChannel->m_StatisticsSpinLock);
                if (NumPackets !=1)
                    DtDbgOut(AVG, IP_RX_REC, "Channel %d: RtpDvbPacketxx SeqNumPrev:%i," 
                              " TimestampPrev:%x, SeqNum:%i,Timestamp:%x,NumPackets:%i",
                              pIpRxChannel->m_ChannelIndex, /*pRtpListEntry->m_BufIndex,*/
                              PrevLastSeqNum, PrevLastTimestamp,
                              pRtpListEntry->m_RtpSequenceNumber, 
                              pRtpListEntry->m_RtpTimestamp, NumPackets);
            }
            
            pDmaRxHeader = (DtaDmaRxHeader*)(pIpRxChannel->m_pRtpBuffer + 
                                (pRtpListEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));

            DtDbgOut(AVG, IP_RX_REC, "Channel %d: RtpDvbPacket. SeqNum:%i,Gap:%i, "
                              "MinPacketsInBuffer:%i,LSNTx:%i,LSNRx:%i NR:%i",
                              pIpRxChannel->m_ChannelIndex, /*pRtpListEntry->m_BufIndex,*/ 
                              pRtpListEntry->m_RtpSequenceNumber, Gap, MinPacketsInBuffer,
                              pIpRxChannel->m_RtpLastSeqNum, LastSeqNum, 
                              pIpRxChannel->m_NumPacketsNotReconstructed);
                
            // Store packet in DVB User buffer
            if (pIpRxChannel->m_DetVidStd == DT_VIDSTD_UNKNOWN)
            {
                DtDbgOut(MAX, IP_RX, "SKIPPED:CopytoDvb Sn:%u pEntry:%p SKIPPED. "
                     "VidStd unknown", pRtpListEntry->m_RtpSequenceNumber, pRtpListEntry);
            
            } else if (pIpRxChannel->m_DetVidStd == DT_VIDSTD_TS)
            {
                DtDbgOut(MAX, IP_RX, "CopytoDvb Sn:%u pEntry:%p", 
                                       pRtpListEntry->m_RtpSequenceNumber, pRtpListEntry);
            
                DtaIpRxProcessDvbPayLoad(pIpRxChannel,
                              (UInt8*)pDmaRxHeader + pRtpListEntry->m_PayloadOffset +
                              sizeof(DtaDmaRxHeader),
                              pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength -
                              pRtpListEntry->m_PayloadOffset - 4,  // -4 for CRC
                              pDmaRxHeader->m_Timestamp, NumPackets-1);
            } else {
                 DtDbgOut(MAX, IP_RX, "CopytoSdi Sn:%u pEntry:%p", 
                                       pRtpListEntry->m_RtpSequenceNumber, pRtpListEntry);
              
                 DtaIpRxProcessSdiPayLoad(pIpRxChannel, (RtpHeader*)((UInt8*)pDmaRxHeader+
                                pRtpListEntry->m_RtpOffset+sizeof(DtaDmaRxHeader)),
                                (UInt8*)pDmaRxHeader + pRtpListEntry->m_PayloadOffset +
                                sizeof(DtaDmaRxHeader),
                                pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength-
                                pRtpListEntry->m_PayloadOffset - 4,  // -4 for CRC
                                pDmaRxHeader->m_Timestamp);
            }
            pIpRxChannel->m_RtpFirstPacket = FALSE;
        } else {
            DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);
            break;
        }
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
    RtpListEntry**  ppReconstructList = (RtpListEntry**)DtMemAllocPool(DtPoolNonPaged, 
                           sizeof(RtpListEntry*)*MAX_FEC_RECONSTR_ELEMENTS_SDI, IPRX_TAG);
    volatile UInt64*  pRefTimeStored = 
                                 &pIpUserChannels->m_pDvcData->m_IpDevice.m_RefTimeStored;
    DtDbgOut(MAX, IP_RX, "Start");
    
    if (ppReconstructList == NULL)
        DtDbgOut(ERR, IP_RX, "Not enough memory to do FEC reconstruction");
    
    while (!StopThread)
    {
        // Wait for RTP packets available or stop
        Status = DtThreadWaitForStopOrEvent(pThread, 
                                             &pIpUserChannels->m_IpRxRtpPacketAvailEvent);
        if (Status == DT_STATUS_CANCELLED)
        {
            StopThread = TRUE;
            continue;
        }
        pIpUserChannels->m_pDvcData->m_IpDevice.m_LastTimeRxTriggered = *pRefTimeStored;
        
        DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);
        
        pIpRxChannel = pIpUserChannels->m_pIpRxChannel;
        while (pIpRxChannel != NULL)
        {
            if (pIpRxChannel->m_RxControl != DT_RXCTRL_RCV)
            {
                pIpRxChannel = pIpRxChannel->m_pNext;
                continue;
            }
            if (!pIpRxChannel->m_RxIncomingPackets && 
                                     (*pRefTimeStored-pIpRxChannel->m_RtpLastCheckedTime<=
                                     DTA_IPRX_RTP_MAXINBUFTIME))
            {
                pIpRxChannel = pIpRxChannel->m_pNext;
                continue;
            }
            pIpRxChannel->m_RxIncomingPackets = FALSE;
            
            DtaIpRxUserChRefAdd(pIpRxChannel);
            ChannelIndex = pIpRxChannel->m_ChannelIndex;
            DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);

            if (pIpRxChannel->m_ResetPacketsPending)
            {
                pIpRxChannel->m_ResetPacketsPending = FALSE;
                DtDbgOut(ERR, IP_RX, "RTP Buffer reset due to reset pending");
                // The following must be done because a new stream could be started
                // and new RTP timestamps and sequence numbers may be received that will
                // skip a lot of packets received at the start of the new stream. Also
                // FEC packets needs to be removed, because of other stream.
                // RtpFirstPacket will be reset in the DtaIpRxRtpListReset function
                // Remove all FEC packets.
                DtaIpRxRtpListsReset(pIpRxChannel);
            }

            if (pIpRxChannel->m_FecMode==DTA_FEC_DISABLE || ppReconstructList==NULL)
                DtaIpRxProcessRtpDvbPacket(pIpRxChannel, NULL, NULL, 
                                                                 (UInt64*)pRefTimeStored);
            else
                DtaIpRxProcessDvbWithFec(pIpRxChannel, ppReconstructList, 
                                                                 (UInt64*)pRefTimeStored);
            pIpRxChannel->m_RtpLastCheckedTime = *pRefTimeStored;
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
    if (ppReconstructList != NULL)
        DtMemFreePool(ppReconstructList, IPRX_TAG);
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIpRxBrmProcessSample -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// PRE-CONDITION:
//  - This is not the first sample: pIpRxChannel->m_BrmNumSamples!=0
//  - Time passed since previous sample is at least the minimum period (200ms)
void  DtaIpRxBrmProcessSample(UserIpRxChannel* pIpRxChannel, UInt64 RefClock, 
                                                                         UInt64 TimeStamp)
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
    pIpDevice->m_RefTimeStored = CurrentTime;
    
    DtSpinLockAcquireAtDpc(&pIpUserChannels->m_IpRxBrmSpinLock);
    if (pIpUserChannels->m_IpRxBrmSkipUpdate)
    {
        DtSpinLockReleaseFromDpc(&pIpUserChannels->m_IpRxBrmSpinLock);
        return;
    }

    // Check last time triggered. If not triggered for x seconds, retrigger
    if (CurrentTime-pIpDevice->m_LastTimeRxTriggered > DTA_IPRX_RTP_MAXINBUFTIME)
        DtEventSet(&pIpUserChannels->m_IpRxRtpPacketAvailEvent);

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
// are available. The pReconstructList is filled with all DVB packets needed to 
// reconstruct.
// Pre: The m_RtpListSpinLock must be acquired 
//
RtpListEntry*  DtaIpRxGetFecReconstructionList(UserIpRxChannel* pIpRxChannel,
                           DtListEntry* pFecList, UInt16 SequenceNumber, UInt32 Timestamp,
                           RtpListEntry** ppReconstructList)
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
        EndSeqNum = ((pRtpEntry->m_FecNA-1)*pRtpEntry->m_FecOffset) + 
                                                                   pRtpEntry->m_FecSNBase;
        
        if (((SequenceNumber>=pRtpEntry->m_FecSNBase && SequenceNumber<=EndSeqNum) ||
                       (EndSeqNum>0xffff && SequenceNumber<=(EndSeqNum%0x10000)))
                       && DtaIpRxRtpIsTimestampLess(Timestamp, pRtpEntry->m_RtpTimestamp))
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
                DtDbgOut(MAX, IP_RX_REC, "Reconstruct SequenceNumber:%i with FecBase:%i" 
                                  " FecOffset:%i", SequenceNumber, pRtpEntry->m_FecSNBase,
                                  pRtpEntry->m_FecOffset);

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
                        // First check lookup table
                        pRtpDvbEntry = NULL;
                        if ((pIpRxChannel->m_pRtpAvailLookup1==NULL&&
                                            pIpRxChannel->m_pRtpAvailLookup2==NULL) || 
                                            pIpRxChannel->m_pRtpAvailLookup1[SeqNum]!=0 ||
                                            pIpRxChannel->m_pRtpAvailLookup2[SeqNum]!=0)
                        {
                            pRtpDvbEntry = DtaIpRxRtpGetDvbEntryUnsafe(pIpRxChannel, 
                                                (UInt16)SeqNum, pRtpEntry->m_RtpTimestamp, 
                                                pDvbListEntry);
                        } else {
                            DtDbgOut(AVG, IP_RX_REC, "SEQ NUM NOT AVAIL. SeqNr:%i L1:%x "
                                                 "L2:%x Time:%x", SeqNum, 
                                                 pIpRxChannel->m_pRtpAvailLookup1[SeqNum],
                                                 pIpRxChannel->m_pRtpAvailLookup2[SeqNum], 
                                                 pRtpEntry->m_RtpTimestamp);
                        }

                        if (pRtpDvbEntry != NULL)
                        {
                            ppReconstructList[Index++] = pRtpDvbEntry;

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
UInt64  DtaIpRxCalcNewRxTimestamp(UserIpRxChannel* pIpRxChannel, 
                                  RtpListEntry* pRtpDestEntry, RtpListEntry* pRtpFecEntry,
                                  RtpListEntry** ppReconstructList)
{
    DtaDmaRxHeader*  pDmaRxHeaderSrc1;
    DtaDmaRxHeader*  pDmaRxHeaderSrc2;
    RtpHeader*  pRtpHeaderSrc1;
    RtpHeader*  pRtpHeaderSrc2;
    UInt  RtpSeqSrc1;
    UInt  RtpSeqSrc2;
    
    UInt64  dT;
    UInt  SeqDiff;
    if (pRtpFecEntry->m_FecNA <= 2) // Is out of spec. No dT can be calculated
        return 0;
    
    pDmaRxHeaderSrc1 = (DtaDmaRxHeader*)(pIpRxChannel->m_pRtpBuffer +
                         (ppReconstructList[0]->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));
        
    pDmaRxHeaderSrc2 = (DtaDmaRxHeader*)(pIpRxChannel->m_pRtpBuffer +
                         (ppReconstructList[1]->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));

    pRtpHeaderSrc1 = (RtpHeader*)((UInt8*)pDmaRxHeaderSrc1 + sizeof(DtaDmaRxHeader) + 
                                                       ppReconstructList[0]->m_RtpOffset);
    pRtpHeaderSrc2 = (RtpHeader*)((UInt8*)pDmaRxHeaderSrc2 + sizeof(DtaDmaRxHeader) + 
                                                       ppReconstructList[1]->m_RtpOffset);
    RtpSeqSrc1 = DtUInt16ByteSwap(pRtpHeaderSrc1->m_SequenceNumber);
    RtpSeqSrc2 = DtUInt16ByteSwap(pRtpHeaderSrc2->m_SequenceNumber);

    // Calculate packet distance (dT)
    if (pDmaRxHeaderSrc2->m_Timestamp >= pDmaRxHeaderSrc1->m_Timestamp)
        dT = pDmaRxHeaderSrc2->m_Timestamp - pDmaRxHeaderSrc1->m_Timestamp;
    else
        dT = 0xffffffff - (pDmaRxHeaderSrc2->m_Timestamp&0xffffffff) + 
                                             (pDmaRxHeaderSrc1->m_Timestamp & 0xffffffff);

    DtDbgOut(MAX, IP_RX, "Channel %d: dT:%llu TimeSrc1:%llu TimeSrc2:%llu "
             "Seq1: %u Seq2:%u DstSeq:%u FecOffset:%u", pIpRxChannel->m_ChannelIndex, dT, 
             (UInt64)pDmaRxHeaderSrc1->m_Timestamp, (UInt64)pDmaRxHeaderSrc2->m_Timestamp,
             RtpSeqSrc1, RtpSeqSrc2, pRtpDestEntry->m_RtpSequenceNumber, 
             pRtpFecEntry->m_FecOffset);
    if (RtpSeqSrc2 < RtpSeqSrc1) 
        SeqDiff = 0xffff-RtpSeqSrc1 + RtpSeqSrc2;
    else
        SeqDiff = RtpSeqSrc2 - RtpSeqSrc1;
    if (SeqDiff == (2 * pRtpFecEntry->m_FecOffset))
    {
        // Reconstructed packet is in between the other packets
        dT = DtDivide64(dT, 2 * pRtpFecEntry->m_FecOffset, NULL);

        DtDbgOut(MAX, IP_RX, "Channel %d: 1-dT:%llu TimeSrc1:%llu TimeSrc2:%llu "
                 "Seq1: %u Seq2:%u FecOffset:%u NewTime:%llu SeqDiff:%u", 
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
                     (ppReconstructList[Index]->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));
    
            pRtpHeaderSrc1 = (RtpHeader*)((UInt8*)pDmaRxHeaderSrc1 + 
                          sizeof(DtaDmaRxHeader) + ppReconstructList[Index]->m_RtpOffset);
            RtpSeqSrc1 = DtUInt16ByteSwap(pRtpHeaderSrc1->m_SequenceNumber);

            // Packet must be insterted after found packet?
            if (((RtpSeqSrc1+pRtpFecEntry->m_FecOffset)%0x10000) == 
                                                       pRtpDestEntry->m_RtpSequenceNumber)
            {
                 DtDbgOut(MAX, IP_RX, "Channel %d: 2-dT:%llu TimeSrc1:%llu "
                            "TimeSrc2:%llu Seq1: %d FecOffset:%d NewTime:%llu SeqDiff:%u",
                            pIpRxChannel->m_ChannelIndex,  dT, 
                            (UInt64)pDmaRxHeaderSrc1->m_Timestamp, 
                            (UInt64)pDmaRxHeaderSrc2->m_Timestamp,
                            RtpSeqSrc1, pRtpFecEntry->m_FecOffset, 
                            (UInt64)pDmaRxHeaderSrc1->m_Timestamp + dT, SeqDiff);
                return pDmaRxHeaderSrc1->m_Timestamp + (dT * pRtpFecEntry->m_FecOffset);
            }
            
            // Packet must be inserted before found packet?
            if (((RtpSeqSrc1-pRtpFecEntry->m_FecOffset)%0x10000) == 
                                                       pRtpDestEntry->m_RtpSequenceNumber)
            {
                DtDbgOut(MAX, IP_RX, "Channel %d: 3-dT:%llu TimeSrc1:%llu"
                            "TimeSrc2:%llu Seq1: %d FecOffset:%d NewTime:%llu SeqDiff:%u",
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
Bool  DtaIpRxReconstructPacket(UserIpRxChannel* pIpRxChannel, RtpListEntry* pRtpDestEntry,
                             RtpListEntry* pRtpFecEntry, RtpListEntry** ppReconstructList)
{
    FecHeader*  pFecHeader;
    FecHeader2022_5*  pFecHeader2022_5;

    DtaDmaRxHeader*  pDmaRxHeader;
    RtpHeader*  pRtpHeader;
    RtpHeader*  pRtpHeaderDest;
    UdpHeader*  pUdpHeaderDest;
    UInt8  PaddingBit;
    UInt8  ExtensionBit;
    UInt8  CCBits;
    UInt8  MarkerBit;
    UInt8  PayloadType;
    UInt32  Timestamp;
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
    Bool  IsSdi;
    
    // Source FEC
    pSrcData = pIpRxChannel->m_pRtpBuffer + 
                                    (pRtpFecEntry->m_BufIndex*DTA_IPRX_MAX_PACKET_LENGTH);

    pDmaRxHeader = (DtaDmaRxHeader*)pSrcData;
    pSrcData+= sizeof(DtaDmaRxHeader);

    pFecHeader = (FecHeader*)(pSrcData + pRtpFecEntry->m_PayloadOffset - 
                                                                       sizeof(FecHeader));
    pFecHeader2022_5 = (FecHeader2022_5*)(pSrcData + pRtpFecEntry->m_PayloadOffset - 
                                                                 sizeof(FecHeader2022_5));
    pRtpHeader = (RtpHeader*)(pSrcData + pRtpFecEntry->m_RtpOffset);
    IsSdi = pRtpHeader->m_PayloadType == RTP_PAYLOAD_FEC_SDI;
    
    PacketSize = pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength - 
                                                        pRtpFecEntry->m_PayloadOffset - 4;

    if (IsSdi)
    {
        PayloadType = pFecHeader2022_5->m_PtRecovery;
        Timestamp = pFecHeader2022_5->m_TsRecovery;
        Length = pFecHeader2022_5->m_LengthRecovery;
        PaddingBit = pFecHeader2022_5->m_PRecovery;
        ExtensionBit = pFecHeader2022_5->m_XRecovery;
        CCBits = pFecHeader2022_5->m_CCRecovery;
        MarkerBit = pFecHeader2022_5->m_MRecovery;
    } else 
    {
        PayloadType = pFecHeader->m_PtRecovery;
        Timestamp = pFecHeader->m_TsRecovery;
        Length = pFecHeader->m_LengthRecovery;
        PaddingBit = 0;
        ExtensionBit = 0;
        CCBits = 0;
        MarkerBit = 0;
    }
    // Destination
    pDstData = (pIpRxChannel->m_pRtpBuffer + 
                                (pRtpDestEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));

    // Get first reconstruction packet to retrieve the offsets
    // L==1: no reconstruction packet available, so take the offsets from the FEC packet
    RtpOffset = pRtpFecEntry->m_RtpOffset;
    if (IsSdi)
    {
        PayloadOffset = pRtpFecEntry->m_PayloadOffset - sizeof(FecHeader2022_5);
        pRtpDestEntry->m_PayloadOffset = PayloadOffset + sizeof(HbrMediaPlHeader);
    
    } else
    {
        PayloadOffset = pRtpFecEntry->m_PayloadOffset - sizeof(FecHeader);
        pRtpDestEntry->m_PayloadOffset = PayloadOffset;
    }

    // Copy start of packet into destination. Only needed for RAW ip mode and debugging
    if (pRtpFecEntry->m_FecNA > 1) 
    {
        pRtpCurEntry = ppReconstructList[Index];
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
                                                         pRtpFecEntry, ppReconstructList);
    pRtpDestEntry->m_RtpOffset = RtpOffset;
    
    // Sequence number already filled by main thread, so skip here
    // pRtpDestEntry->m_RTPSequenceNumber
    
    pDstData += sizeof(DtaDmaRxHeader);
    pRtpHeaderDest = (RtpHeader*)(pDstData + pRtpDestEntry->m_RtpOffset);
    pUdpHeaderDest = (UdpHeader*)(pDstData + pRtpDestEntry->m_RtpOffset - 
                                                                       sizeof(UdpHeader));
    pDstData = pDstData + PayloadOffset;

    // Store the FEC recovery data into the destination packet
    DtMemCopy(pDstData, pSrcData, PacketSize);

    pDstPayload = (UInt32*)pDstData;

    while (Index < pRtpFecEntry->m_FecNA-1) 
    {
        pRtpCurEntry = ppReconstructList[Index++];
        pSrcData = (pIpRxChannel->m_pRtpBuffer + sizeof(DtaDmaRxHeader) +
                                 (pRtpCurEntry->m_BufIndex * DTA_IPRX_MAX_PACKET_LENGTH));
        pRtpHeader = (RtpHeader*)(pSrcData + pRtpCurEntry->m_RtpOffset);

        // Skip reconstruction if RTP header extension is available
        if (pRtpHeader->m_Extension)
            return FALSE;
    
        pSrcData += PayloadOffset;
        pSrcPayload = (UInt32*)pSrcData;

        DT_ASSERT(PacketSize%4 == 0);
        DtDbgOut(MAX, IP_RX_REC, "Pkt for RECON: RtpSeq:%i RtpTime:%x", 
                         pRtpCurEntry->m_RtpSequenceNumber, pRtpCurEntry->m_RtpTimestamp);

        for (Counter = 0; Counter < PacketSize/4; Counter++)
            pDstPayload[Counter] = pDstPayload[Counter]^pSrcPayload[Counter];
        

        PaddingBit = PaddingBit^pRtpHeader->m_Padding;
        ExtensionBit = ExtensionBit^pRtpHeader->m_Extension;
        CCBits = CCBits^pRtpHeader->m_CsrcCount;
        MarkerBit = MarkerBit^pRtpHeader->m_Marker;
        
        PayloadType = PayloadType^pRtpHeader->m_PayloadType;
        Timestamp = Timestamp^pRtpHeader->m_TimeStamp;
        Length = Length^DtUInt16ByteSwap((UInt16)PacketSize);
    }
    
    // Reconstruction ready
    // Store header information into destination packet
    pRtpHeaderDest->m_CsrcCount = CCBits;
    pRtpHeaderDest->m_Extension = ExtensionBit;
    pRtpHeaderDest->m_Marker = MarkerBit;
    pRtpHeaderDest->m_Padding = PaddingBit;
    pRtpHeaderDest->m_PayloadType = PayloadType;
    // Timestamp must not be 0 for the lookup table to work
    pRtpHeaderDest->m_TimeStamp = Timestamp;
    pRtpHeaderDest->m_Version = 2;
    pRtpHeaderDest->m_SequenceNumber = 
                                     DtUInt16ByteSwap(pRtpDestEntry->m_RtpSequenceNumber);
    pUdpHeaderDest->m_Checksum = 0;
    pRtpHeaderDest->m_Ssrc = pRtpHeader->m_Ssrc;    // Stream ID
    if (Timestamp == 0)
        Timestamp = 1;
    pRtpDestEntry->m_RtpTimestamp = DtUInt32ByteSwap(Timestamp);
    // Check if the source does include a clock. If so, we have to add 4 to the payload 
    // offset
    if (IsSdi)
    {
        HbrMediaPlHeader*  pHbrHeader = (HbrMediaPlHeader*)pDstPayload;
        if (pHbrHeader->m_CfH!=0 || pHbrHeader->m_CfL!=0)
            pRtpDestEntry->m_PayloadOffset += 4;
        if (pHbrHeader->m_Ext != 0)
            pRtpDestEntry->m_PayloadOffset += 4*pHbrHeader->m_Ext;
        if (pHbrHeader->m_F == 0)
            pRtpDestEntry->m_PayloadOffset -= 4;
    }
    DtDbgOut(MAX, IP_RX_REC, "RtpSeqRecon:%i RtpTime:%x M:%i", 
                        pRtpDestEntry->m_RtpSequenceNumber, pRtpDestEntry->m_RtpTimestamp,
                        pRtpHeaderDest->m_Marker);
    return TRUE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIpRxTryReconstructPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Pre: The m_RtpListSpinLock must be acquired 
// Post: The m_RtpListSpinLock is released and acquired again!! 
// (New packets can be inserted to the list inbetween the release and acquire.)
//
RtpListEntry*  DtaIpRxTryReconstructPacket(UserIpRxChannel* pIpRxChannel, 
                UInt16 SequenceNumber, UInt32 Timestamp, RtpListEntry** ppReconstructList)
{
    RtpListEntry*  pRtpEntry = NULL;
    RtpListEntry*  pRtpFecEntry = NULL;
    Int  i;

    pRtpFecEntry = DtaIpRxGetFecReconstructionList(pIpRxChannel, 
            &pIpRxChannel->m_RtpFecRowList, SequenceNumber, Timestamp, ppReconstructList);

    if (pRtpFecEntry == NULL)
    {
        pRtpFecEntry = DtaIpRxGetFecReconstructionList(pIpRxChannel, 
         &pIpRxChannel->m_RtpFecColumnList, SequenceNumber, Timestamp, ppReconstructList);
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
                ppReconstructList[i]->m_InUse = 1;
          
            // Release spinlock before reconstruction, so list can be accessed during
            // reconstruction
            DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);
            pRtpEntry = DtContainingRecord(pListEntry, RtpListEntry, m_ListEntry);
            pRtpEntry->m_RtpSequenceNumber = SequenceNumber;
            
            if (!DtaIpRxReconstructPacket(pIpRxChannel, pRtpEntry, pRtpFecEntry, 
                                                                       ppReconstructList))
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
                ppReconstructList[i]->m_InUse = 0;
        } 
    }
    return pRtpEntry;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxRemoveOldFecPackets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Pre: The m_RtpListSpinLock must be acquired 
//
void  DtaIpRxRemoveOldFecPackets(UserIpRxChannel* pIpRxChannel, DtListEntry* pFecList,
                                          UInt16 LastSequenceNumber, UInt32 LastTimestamp)
{
    DtListEntry*  pListEntry;
    DtListEntry*  pListEntryNext;
    RtpListEntry*  pRtpEntry;
    UInt32  EndSeqNum;
    UInt16  MaxGap = DtaIpRxGetMaxOutOfSync(pIpRxChannel);
    
    if (DtIsListEmpty(pFecList))
        return;    

    pListEntry = DtListNextGet(pFecList);

    while (pListEntry!=pFecList)
    {
        pRtpEntry = DtContainingRecord(pListEntry, RtpListEntry, m_ListEntry);
        
        // Get the last sequence number for this fec packet
        EndSeqNum = (pRtpEntry->m_FecNA*pRtpEntry->m_FecOffset) + pRtpEntry->m_FecSNBase;
        pListEntryNext = DtListNextGet(pListEntry);
        
        if (DtaIpRxRtpIsSeqNumLess2((UInt16)EndSeqNum, pRtpEntry->m_RtpTimestamp, 
                                                     LastSequenceNumber, LastTimestamp) || 
           (!DtaIpRxRtpIsSeqNumLess2((UInt16)EndSeqNum, pRtpEntry->m_RtpTimestamp, 
                                                     LastSequenceNumber, LastTimestamp) && 
           pRtpEntry->m_InUse == 0 && (
               DtaIpRxGetSequenceNumberGap2(LastSequenceNumber, LastTimestamp, 
               (UInt16)EndSeqNum, pRtpEntry->m_RtpTimestamp,
                pIpRxChannel->m_DetVidStd==DT_VIDSTD_TS, pIpRxChannel->m_MaxBitrate)>
                                                                     (UInt32)(4*MaxGap))))
        {
            DtDbgOut(AVG, IP_RX_REC, 
               "Channel %d: RemoveOldFec. BufIndex:%i, SeqNum:%i Timestamp:%x, Base: %i,"
                    " MaxSeqNum:%i l1:%i l2:%i Gap:%i",
                    pIpRxChannel->m_ChannelIndex, pRtpEntry->m_BufIndex, 
                    pRtpEntry->m_RtpSequenceNumber, pRtpEntry->m_RtpTimestamp,
                    pRtpEntry->m_FecSNBase, EndSeqNum,
                    DtaIpRxRtpIsSeqNumLess2((UInt16)EndSeqNum, pRtpEntry->m_RtpTimestamp, 
                                                       LastSequenceNumber, LastTimestamp),
                    DtaIpRxRtpIsSeqNumLess2((UInt16)EndSeqNum, pRtpEntry->m_RtpTimestamp, 
                                                       LastSequenceNumber, LastTimestamp),
                    DtaIpRxGetSequenceNumberGap2(LastSequenceNumber, LastTimestamp, 
                (UInt16)EndSeqNum, pRtpEntry->m_RtpTimestamp,
                pIpRxChannel->m_DetVidStd==DT_VIDSTD_TS, pIpRxChannel->m_MaxBitrate));

            DtListEntryRemove(pListEntry);
            DtaIpRxRtpMoveEntryToEmptyListUnsafe(pIpRxChannel, pRtpEntry);
        }
        pListEntry = pListEntryNext;
    }
    return;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxProcessDvbWithFec -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRxProcessDvbWithFec(UserIpRxChannel* pIpRxChannel, 
                                 RtpListEntry** ppReconstructList, UInt64* pRefTimeStored)
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
    UInt32  LastTimestamp;
    UInt32  RtpLastTimestamp;
    UInt16  CurSeqNum;
    UInt16  MissingSeqNumber = 0;
    UInt16*  pLastSeqNumToTx = NULL;
    UInt32*  pLastTimestampToTx = NULL;
    UInt32  PrevLastTimestamp = pIpRxChannel->m_RtpLastTimestamp;
    DtSpinLockAcquire(&pIpRxChannel->m_RtpListSpinLock);

    // Try to reconstruct packets
    if (!DtIsListEmpty(&pIpRxChannel->m_RtpDvbList))
    {
        PacketReconstructed = TRUE;    // First time
        DtDbgOut(AVG, IP_RX_REC, "START RECONSTRUCT THREAD");
        while (PacketReconstructed)
        {
            // Retrieve the last received sequence number
            NumPacketsTried = 0;
            pRtpLastListEntry = 
                            DtContainingRecord(DtListPrevGet(&pIpRxChannel->m_RtpDvbList),
                            RtpListEntry, m_ListEntry);
            LastSeqNum = pRtpLastListEntry->m_RtpSequenceNumber;
            LastTimestamp = pRtpLastListEntry->m_RtpTimestamp;
            pLastSeqNumToTx = &LastSeqNum;
            pLastTimestampToTx = &LastTimestamp;
        
            PacketReconstructed = FALSE;
            SeqNumMissing = FALSE;
            pRtpListEntry = DtContainingRecord(DtListNextGet(&pIpRxChannel->m_RtpDvbList),
                                                               RtpListEntry, m_ListEntry);

            // If m_RtpFirstPacket is true, we take the first sequence number in list -1
            if (pIpRxChannel->m_RtpFirstPacket)
            {
                CurSeqNum = pRtpListEntry->m_RtpSequenceNumber-1;
                RtpLastTimestamp = pRtpListEntry->m_RtpTimestamp;
            }
            else {
                CurSeqNum = pIpRxChannel->m_RtpLastSeqNum;
                RtpLastTimestamp = pIpRxChannel->m_RtpLastTimestamp;
            }

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
                while ((UInt16)(CurSeqNum+1)!=pRtpListEntry->m_RtpSequenceNumber && !Stop)
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

                    DtDbgOut(AVG, IP_RX_REC, 
                                   "Channel %d: TRY reconstruct SN:%i LSN:%i, RtpTime:%x",
                                   pIpRxChannel->m_ChannelIndex, CurSeqNum, LastSeqNum,
                                   RtpLastTimestamp);

                    // Store the backward link to known if some 
                    // elements are inserted before the entry
                    pBackElement = DtListPrevGet(&pRtpListEntry->m_ListEntry);

                    // After this function, the spinlock is released and acquired again
                    // So some elements can be inserted inbetween
                    pRtpReconstructedEntry = DtaIpRxTryReconstructPacket(pIpRxChannel, 
                                         CurSeqNum, RtpLastTimestamp,  ppReconstructList);
                    if (pRtpReconstructedEntry != NULL) 
                    {                     
                        DtSpinLockAcquire(&pIpRxChannel->m_StatisticsSpinLock);
                        pIpRxChannel->m_NumPacketsReconstructed++;
                        DtSpinLockRelease(&pIpRxChannel->m_StatisticsSpinLock);

                        PacketReconstructed = TRUE;

                        DtDbgOut(AVG, IP_RX_REC, "Channel %d: Reconstructed SN:%i", 
                                                 pIpRxChannel->m_ChannelIndex, CurSeqNum);
                        
                        if (pBackElement == DtListPrevGet(&pRtpListEntry->m_ListEntry)) 
                        {
                            // No elements inserted during reconstruction
                            // Insert packet into list before entry
                            DtTailListInsert(&pRtpListEntry->m_ListEntry , 
                                                    &pRtpReconstructedEntry->m_ListEntry);
                            if (pIpRxChannel->m_pRtpAvailLookup1!=NULL && 
                                           pIpRxChannel->m_pRtpAvailLookup1[CurSeqNum]==0)
                                    pIpRxChannel->m_pRtpAvailLookup1[CurSeqNum] = 
                                                   pRtpReconstructedEntry->m_RtpTimestamp;
                                else if (pIpRxChannel->m_pRtpAvailLookup2!=NULL && 
                                           pIpRxChannel->m_pRtpAvailLookup2[CurSeqNum]==0)
                                    pIpRxChannel->m_pRtpAvailLookup2[CurSeqNum] = 
                                                   pRtpReconstructedEntry->m_RtpTimestamp;
                        } else {
                            Bool Added;
                            if (pIpRxChannel->m_RtpFirstPacket)
                            {
                                Added = !DtaIpRxRtpListAddEntryUnsafe(
                                     &pIpRxChannel->m_RtpDvbList, pRtpReconstructedEntry);
                            } else if ((((pRtpReconstructedEntry->m_RtpSequenceNumber==
                                                         pIpRxChannel->m_RtpLastSeqNum) &&
                                  (pRtpReconstructedEntry->m_RtpTimestamp==
                                                     pIpRxChannel->m_RtpLastTimestamp)) ||
                                 DtaIpRxRtpIsSeqNumLess2(
                                              pRtpReconstructedEntry->m_RtpSequenceNumber,
                                              pRtpReconstructedEntry->m_RtpTimestamp,
                                              pIpRxChannel->m_RtpLastSeqNum, 
                                              pIpRxChannel->m_RtpLastTimestamp)))
                            {
                                Added = FALSE;

                            } else {
                                Added = !DtaIpRxRtpListAddEntryUnsafe(
                                                              &pIpRxChannel->m_RtpDvbList,
                                                              pRtpReconstructedEntry);
                            }
                            // Sort element into list
                            if (!Added)
                            {
                                // Element already in queue
                                // Add to empty list
                                DtaIpRxRtpMoveEntryToEmptyListUnsafe(pIpRxChannel, 
                                                                  pRtpReconstructedEntry);
                                // Get entry already in queue
                                pRtpReconstructedEntry = DtaIpRxRtpGetDvbEntryUnsafe(
                                          pIpRxChannel, CurSeqNum, 
                                          pRtpReconstructedEntry->m_RtpTimestamp+1, NULL);
                                DtDbgOut(MAX, IP_RX_REC, "Channel %d: Reconstructed SN:%i"
                                             " NOT ADDED. L1:%x L2:%x!!", 
                                             pIpRxChannel->m_ChannelIndex, CurSeqNum,   
                                             pIpRxChannel->m_pRtpAvailLookup1[CurSeqNum],
                                             pIpRxChannel->m_pRtpAvailLookup2[CurSeqNum]);
                            } else {
                                if (pIpRxChannel->m_pRtpAvailLookup1!=NULL && 
                                           pIpRxChannel->m_pRtpAvailLookup1[CurSeqNum]==0)
                                    pIpRxChannel->m_pRtpAvailLookup1[CurSeqNum] = 
                                                   pRtpReconstructedEntry->m_RtpTimestamp;
                                else if (pIpRxChannel->m_pRtpAvailLookup2!=NULL && 
                                           pIpRxChannel->m_pRtpAvailLookup2[CurSeqNum]==0)
                                    pIpRxChannel->m_pRtpAvailLookup2[CurSeqNum] = 
                                                   pRtpReconstructedEntry->m_RtpTimestamp;
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
                        DtDbgOut(AVG, IP_RX_REC, "Channel %d: NOT Reconstructed SN:%i", 
                                                 pIpRxChannel->m_ChannelIndex, CurSeqNum);
                    }

                    NumPacketsTried++;
                    // Only try to recover the first 3 matrixes.
                    if (NumPacketsTried > 3*DtaIpRxGetMaxOutOfSync(pIpRxChannel) && 
                                                          !pIpRxChannel->m_RtpFirstPacket)
                        Stop = TRUE;
                }

                if (Stop)
                    break;

                if (DtListNextGet(&pRtpListEntry->m_ListEntry) == 
                                                              &pIpRxChannel->m_RtpDvbList)
                    break; // End of list
                if (!SeqNumMissing) 
                    PrevLastTimestamp = pRtpListEntry->m_RtpTimestamp;
                pRtpListEntry = 
                    DtContainingRecord(DtListNextGet(&pRtpListEntry->m_ListEntry),
                                                               RtpListEntry, m_ListEntry);
                CurSeqNum++;
                NumPacketsTried++;
            }
        }
        DtDbgOut(AVG, IP_RX_REC, "STOP RECONSTRUCT THREAD: reconstructed: %i Tried:%i", 
                                                    PacketReconstructed, NumPacketsTried);
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
                UInt32 Gap;
                Gap = DtaIpRxGetSequenceNumberGap2(MissingSeqNumber, PrevLastTimestamp, 
                     LastSeqNum, LastTimestamp,  
                     pIpRxChannel->m_DetVidStd==DT_VIDSTD_TS, pIpRxChannel->m_MaxBitrate);
                if (Gap > (UInt32)2*DtaIpRxGetMaxOutOfSync(pIpRxChannel)) 
                {
                    // Let the transmitter decide to transmit the packets
                    LastSeqNum = LastSeqNum - 2*DtaIpRxGetMaxOutOfSync(pIpRxChannel);
                    pLastSeqNumToTx = NULL;
                } else {
                    LastSeqNum = MissingSeqNumber;
                }
                // Update timestamp to last incremented seq. timestamp for removing old
                // fec packets
                LastTimestamp = PrevLastTimestamp;//pIpRxChannel->m_RtpLastTimestamp;
                DtDbgOut(AVG, IP_RX_REC, "Channel %d: LastSeqNum:%d LastTimestamp:%x "
                             "MissingSeqNumber:%d Gap:%i MaxOutOfSync:%i", 
                             pIpRxChannel->m_ChannelIndex, LastSeqNum, LastTimestamp, 
                             MissingSeqNumber, Gap, DtaIpRxGetMaxOutOfSync(pIpRxChannel));
            } else {
                DtDbgOut(AVG, IP_RX_REC, "Channel %d: LastSeqNum:%d LastTimestamp:%x", 
                                 pIpRxChannel->m_ChannelIndex, LastSeqNum, LastTimestamp);
            }

            // Remove all FEC Packets not needed for packets < LastSeqNum
            DtaIpRxRemoveOldFecPackets(pIpRxChannel, &pIpRxChannel->m_RtpFecRowList, 
                                                               LastSeqNum, LastTimestamp);
            DtaIpRxRemoveOldFecPackets(pIpRxChannel, &pIpRxChannel->m_RtpFecColumnList,
                                                               LastSeqNum, LastTimestamp);
        }
    }

    DtSpinLockRelease(&pIpRxChannel->m_RtpListSpinLock);

    // Send all packets in sequence to user buffer
    DtaIpRxProcessRtpDvbPacket(pIpRxChannel, pLastSeqNumToTx, pLastTimestampToTx,
                                                                          pRefTimeStored);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxCalculateNumIpRxListenersType1 -.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaIpRxCalculateNumIpRxListenersType1(DtaIpPort* pIpPort, Bool IpV6Packet, 
                 Int VlanId, UInt8* pSrcIp, UInt8* pDstIp, UInt16 SrcPort, UInt16 DstPort)
{
    Int  i;
    DtaIpUserChannels*  pIpUserChannels =
                                        &pIpPort->m_pDvcData->m_IpDevice.m_IpUserChannels;
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

        // For bitrate calculation to work in idle state,  we check the RxControl 
        // somewhere else and only check here if the parameters are set. 
        if (!pIpRxChannel->m_IpParsValid)
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }
        if (pIpRxChannel->m_VlanId != VlanId)
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }
        if (IpV6Packet && (pIpRxChannel->m_IpParsFlags & DTA_IP_V6)==0)
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }
        if (!IpV6Packet && (pIpRxChannel->m_IpParsFlags & DTA_IP_V6)!=0)
        {
            pIpRxChannel = pIpRxChannel->m_pNext;
            continue;
        }

        // Check if source + destination IP address is correct.
        Stop = FALSE;
        for (i=0; i<(IpV6Packet?16:4); i++) 
        {
            if (pIpRxChannel->m_SrcIPAddress[i]!=0 && 
                                               pIpRxChannel->m_SrcIPAddress[i]!=pSrcIp[i])
            {
                Stop = TRUE;
                break;
            }

            // If a multicast packet is received, the m_DstIPAddress may not be 0.0.0.0
            if (pIpRxChannel->m_DstIPAddress[i] != pDstIp[i])
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
        
        // We skip source port check.
        // All source ports are merged for this calculation
        
       // Check destination port
       IsFecPort = FALSE;
        if (pIpRxChannel->m_FecMode!=DTA_FEC_DISABLE)
        {
            if (pIpRxChannel->m_DstPort+FEC_INC_COLUMN_PORT==DstPort ||
                                        pIpRxChannel->m_DstPort+FEC_INC_ROW_PORT==DstPort)
               IsFecPort = TRUE;
        }

        // If the destination port matches or if the portnumber it used for a FEC stream
        // add the channel.
        if (pIpRxChannel->m_DstPort==DstPort || IsFecPort)
            Index++;

        pIpRxChannel = pIpRxChannel->m_pNext;
    }
    DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
    DtDbgOut(MAX, IP_RX, "Max. listeners current channel: %i(Tot max:%i)", 
                                                       Index, pIpPort->m_MaxNumListeners);
    return Index;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRxCalculateNumIpRxListenersType2 -.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaIpRxCalculateNumIpRxListenersType2(DtaIpPort* pIpPort, UInt32 IdTag)
{
    Bool  SSMIpV6 = FALSE;
    DtaIpUserChannels*  pIpUserChannels = 
                                        &pIpPort->m_pDvcData->m_IpDevice.m_IpUserChannels;
    UserIpRxChannel*  pIpRxChannel;
    Int  Index = 0;
    UInt  FirstIpPortNum = pIpPort->m_IpPortIndex&0xfffffffe;
    DtaIpPort*  pFirstPort = &pIpPort->m_pDvcData->m_IpDevice.m_pIpPorts[FirstIpPortNum];
    Int  StreamType;
    
    // Wait until the Rx structure is available
    DtFastMutexAcquire(&pIpUserChannels->m_IpRxChThreadMutex);
    DtMutexAcquire(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex, -1);

    // Get the channel list for this IdTag
    pIpRxChannel = pIpPort->m_pDvcData->m_IpDevice.m_pAddrMatchLUTable[IdTag];
    StreamType = pIpPort->m_pDvcData->m_IpDevice.m_AddrMatchLUTableType[IdTag];
    
    if (pIpRxChannel != NULL)
        SSMIpV6 = 
            pIpRxChannel->m_AddrMatcherEntry[StreamType] \
                                              [pIpPort->m_IpPortIndex&1].m_Gen.m_SSM==1 &&
            pIpRxChannel->m_AddrMatcherEntry[StreamType] \
                                            [pIpPort->m_IpPortIndex&1].m_Gen.m_Version==1;
    
    while (pIpRxChannel != NULL) 
    {
        Index++;

        if (SSMIpV6)
        {
           if (pIpRxChannel->m_pNextAddrMatcherEntryPart2[StreamType] \
                                                       [pIpPort->m_IpPortIndex&1] != NULL)
            {
                Int  StreamType2;
                StreamType2 = pIpRxChannel->m_pNextAddrMatcherEntryPart2[StreamType] \
                                                 [pIpPort->m_IpPortIndex&1]->m_StreamType;
                
                pIpRxChannel = DtContainingRecord(
                       pIpRxChannel->m_pNextAddrMatcherEntryPart2[StreamType] \
                       [pIpPort->m_IpPortIndex&1], UserIpRxChannel,
                       m_AddrMatcherEntrySSMPart2[StreamType][pIpPort->m_IpPortIndex&1]);
                StreamType = StreamType2;
            } else 
                pIpRxChannel = NULL;
        } else {
            if (pIpRxChannel->m_pNextAddrMatcherEntry[StreamType] \
                                                       [pIpPort->m_IpPortIndex&1] != NULL)
            {
                Int  StreamType2;
                StreamType2 = pIpRxChannel->m_pNextAddrMatcherEntry[StreamType] \
                                                 [pIpPort->m_IpPortIndex&1]->m_StreamType;
                pIpRxChannel = DtContainingRecord(
                               pIpRxChannel->m_pNextAddrMatcherEntry[StreamType] \
                               [pIpPort->m_IpPortIndex&1], UserIpRxChannel,
                               m_AddrMatcherEntry[StreamType][pIpPort->m_IpPortIndex&1]);
                StreamType = StreamType2;
            } else 
                pIpRxChannel = NULL;
        }
    }
    DtMutexRelease(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex);
    DtFastMutexRelease(&pIpUserChannels->m_IpRxChThreadMutex);
    DtDbgOut(MAX, IP_RX, "Max. listeners current channel: %i(Tot max:%i)", 
                                                       Index, pIpPort->m_MaxNumListeners);
    return Index;
}
