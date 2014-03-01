//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* IpTx.c *#*#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// Dta driver - IP TX functionality - Implementation of TX specific functionality for
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
// IPTX Tag
#define IPTX_TAG         0x78547049  // 'xTpI'
#define DTA_IPTX_RT_DMA_MAX_TRANSFER_SIZE  2621440 
                                        // The expected bitrate is 1GBit/s in a 
                                        // period of 20 ms.
                                        // calculation: 1GB/8 * 20mS = 2.5 Mbyte 

#if defined(WINBUILD)
#define DTA_IPTX_RT_DMA_BUF_SIZE        DTA_IPTX_RT_DMA_MAX_TRANSFER_SIZE
#define DTA_IPTX_NRT_DMA_BUF_SIZE       (2*1024*1024)
#else
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20)) || !defined(CONFIG_MMU)
#define DTA_IPTX_RT_DMA_BUF_SIZE        DTA_IPTX_RT_DMA_MAX_TRANSFER_SIZE
#define DTA_IPTX_NRT_DMA_BUF_SIZE       (2*1024*1024)
#else
// Lower value for RHEL/CentOS kernels
#warning "IP buffer too small to get good performance!"
#define DTA_IPTX_RT_DMA_BUF_SIZE        (64*1024)
#define DTA_IPTX_NRT_DMA_BUF_SIZE       (64*1024)
#endif
#endif

#define DTA_IPTX_RT_PINGPONG_BUF_SIZE   (DTA_IPTX_RT_DMA_BUF_SIZE * 2)
#define DTA_IPTX_NRT_SHARED_BUF_SIZE    (DTA_IPTX_NRT_DMA_BUF_SIZE)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// private IOCTL handlers
DtStatus  DtaIpTxUserChAttach(DtaIpUserChannels* pIpUserChannels, Int PortIndex,
                                Int IpPortIndex, DtFileObject* pFile, Int* pChannelIndex);
DtStatus  DtaIpTxUserChDetach(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex);
DtStatus  DtaIpTxUserChClearFlags(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                                               Int Flags);
DtStatus  DtaIpTxUserChGetFlags(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                         Int* pFlags, Int* pLatchedFlags);
DtStatus DtaIpTxUserChSetIpPars(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,  
             UInt8* pSrcIp, UInt16 SrcPort, UInt8* pSrcMac, UInt8* pDstIp, UInt16 DstPort,
             UInt16 DstPort2, UInt8* pDstMac, Int VlanId, Int VlanPriority, Int FecMode, 
             Int Protocol, Int Mode, Int Flags);
DtStatus  DtaIpTxUserChReset(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex);
DtStatus  DtaIpTxUserChSetTxControl(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
                                                                           Int TxControl);
DtStatus  DtaIpTxWriteNDisPacket(DtaIpPort* pIpPort, UInt8* pBuf, UInt BufLen);

// Private Helpers
static void  DtaIpTxReBuildPacket(UserIpTxChannel* pIpTxChannel, UInt8* pBuffer, 
                                                                       UInt PacketLength);
static void  DtaIpTxUserChAddToList(DtaIpUserChannels* pIpUserChannels, 
                                                               UserIpTxChannel* pIpTxNew);
static UserIpTxChannel*  DtaIpTxUserChCreate(Int ChannelIndex, Int PortIndex, 
                                                    Int IpPortIndex, DtFileObject* pFile);
static void  DtaIpTxUserChInitRWPointers(UserIpTxChannel* pIpTxChannel);
static void  DtaIpTxUserChRemoveFromList(DtaIpUserChannels* pIpUserChannels,
                                                           UserIpTxChannel* pIpTxChannel);
static void  DtaIpTxUserChSetTxControlIdle(DtaIpUserChannels* pIpUserChannels, 
                                                           UserIpTxChannel* pIpTxChannel);
static void  DtaIpTxUserChUpdateReferenceTimeStamps(DtaDmaTxHeader* pDtaDmaTxHeader, 
                                      UInt64* pStartTimestamp, UInt64* pRefTimestamp,
                                      UInt32* pLastTimestampOffs, DtaDeviceData* pDvcData,
                                      Bool FifoUnderflow);
static UserIpTxChannel*  DtaIpTxUserChFindNewPacket(DtStatus* pPortStatus,
                               DtaIpUserChannels* pIpUserChannels, UInt64 Max1PacketTime);
void  DtaIpTxNrtWorkerThread(DtThread* pThread, void* pContext);
void  DtaIpTxRtGetLocalAddress(void* pContext, UInt8** pLocalAddress, UInt* pStart,
                                                        UInt* pStop, UInt* pTransferSize);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxDeviceInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpTxDeviceInit(DtaIpDevice* pIpDevice)
{
    DtStatus  Status = DT_STATUS_OK;

    if (pIpDevice->m_PortType == DTA_IPPORT_TYPE1)
        Status = DtDpcInit(&pIpDevice->m_IpRtTxDpc, DtaIpTxRtProcessPacketsType1Dpc, TRUE);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxDevicePowerdown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpTxDevicePowerdown(DtaIpDevice* pIpDevice)
{
    DtDpcWaitForCompletion(&pIpDevice->m_IpRtTxDpc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxPowerdownType1Pre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpTxPowerdownType1Pre(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDbgOut(AVG, IP_TX, "Stopping IpTxNrt thread");
    Status = DtThreadStop(&pIpPort->m_IpPortType1.m_TxNrtThread);
    DtDbgOut(AVG, IP_TX, "IpTxNrt thread stopped");
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxPowerdownType2Pre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpTxPowerdownType2Pre(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDbgOut(AVG, IP_TX, "Stopping IpTxNrt thread");
    Status = DtThreadStop(&pIpPort->m_IpPortType2.m_TxNrtThread);
    DtDbgOut(AVG, IP_TX, "IpTxNrt thread stopped");
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxPowerdownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpTxPowerdownPre(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        Status = DtaIpTxPowerdownType1Pre(pIpPort);
    else 
        Status = DtaIpTxPowerdownType2Pre(pIpPort);
        
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxPowerDown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpTxPowerDown(DtaIpPort* pIpPort)
{
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE2)
    {
        if (pIpPort->m_IpPortIndex%2 == 0)
            DtDpcWaitForCompletion(&pIpPort->m_IpPortType2.m_IpRtTxDpc);
    }
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxPowerupType1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpTxPowerupType1(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    
    // Make shure the pingpong buffer has a valid initial state after resume from suspend.
    // See also issue #107: If DMA is finished while powering down, the ping pong buffers
    // already would have the correct states after resume, and this may not be needed.
    DtaPPBufferInitInternalStates(&pIpPort->m_IpPortType1.m_TxRtPPBuffer, 
                                                              DT_DMA_DIRECTION_TO_DEVICE);
    
    DtDbgOut(AVG, IP_TX, "Starting IpTxNrt thread");
    Status = DtThreadStart(&pIpPort->m_IpPortType1.m_TxNrtThread);
    if (DT_SUCCESS(Status))
        DtDbgOut(AVG, IP_TX, "IpTxNrt thread started");

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxPowerupType2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpTxPowerupType2(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    
    // Make shure the pingpong buffer has a valid initial state after resume from suspend.
    // See also issue #107: If DMA is finished while powering down, the ping pong buffers
    // already would have the correct states after resume, and this may not be needed.
    DtaPPBufferInitInternalStates(&pIpPort->m_IpPortType2.m_TxRtPPBuffer, 
                                                              DT_DMA_DIRECTION_TO_DEVICE);
    
    DtDbgOut(AVG, IP_TX, "Starting IpTxNrt thread");
    Status = DtThreadStart(&pIpPort->m_IpPortType2.m_TxNrtThread);
    if (DT_SUCCESS(Status))
        DtDbgOut(AVG, IP_TX, "IpTxNrt thread started");

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpTxPowerup(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        Status = DtaIpTxPowerupType1(pIpPort);
    else
        Status = DtaIpTxPowerupType2(pIpPort);

    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxInitType1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpTxInitType1(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  DmaRegsOffsetTxRt;
    UInt  DmaRegsOffsetTxNrt;
    DtaIpPortType1*  pIpPortType1 = &pIpPort->m_IpPortType1;
    DtPropertyData*  pPropData = &pIpPort->m_pDvcData->m_PropData;
    
    pIpPortType1->m_TxRt.m_pRegs = NULL;
    pIpPortType1->m_TxNrt.m_pRegs = NULL;

    // Initialise register offsets
    pIpPortType1->m_TxNrt.m_RegsOffset = DtPropertiesGetUInt16(pPropData, 
                                             "NW_REGISTERS_NRT_TX", pIpPort->m_PortIndex);
    pIpPortType1->m_TxNrt.m_FifoOffset = 
                              pIpPortType1->m_TxNrt.m_RegsOffset + DTA_LOCALADDR_FIFODATA;

    pIpPortType1->m_TxRt.m_RegsOffset = DtPropertiesGetUInt16(pPropData,
                                              "NW_REGISTERS_RT_TX", pIpPort->m_PortIndex);
    pIpPortType1->m_TxRt.m_FifoOffset = 
                               pIpPortType1->m_TxRt.m_RegsOffset + DTA_LOCALADDR_FIFODATA;

    DmaRegsOffsetTxRt = DtPropertiesGetUInt16(pPropData, 
                                          "NW_REGISTERS_RT_TX_DMA", pIpPort->m_PortIndex);

    DmaRegsOffsetTxNrt = DtPropertiesGetUInt16(pPropData, "NW_REGISTERS_NRT_TX_DMA",
                                                                    pIpPort->m_PortIndex);
    // Report configuration errors
    // Check if no property error occurred
    Status = DtaPropertiesReportDriverErrors(pIpPort->m_pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Initialise Rt Tx DMA channel
    // For Rt Tx we use a Ping-pong buffer to transfer the data.
    // For the complete ping-pong buffer, the SGL list is created
    // The actual DMA transfer size will be the size divided by 2.
    Status = DtaDmaInitCh(pIpPort->m_pDvcData, pIpPort->m_PortIndex, 
                                      DTA_IPTX_RT_PINGPONG_BUF_SIZE, DTA_DMA_MODE_DEFAULT,
                                      DmaRegsOffsetTxRt, DTA_DMA_FLAGS_DATA_BUF_NO_COPY,
                                      -1, NULL, NULL,
                                      &pIpPortType1->m_TxRt.m_DmaChannel, TRUE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_TX, "Error initialising IpTx Rt DMA channel");
        return Status;
    }

    // Initialise Nrt Tx DMA channel
    // We must add extra space for the setup frame in case we use
    // common buf DMA.
    Status = DtaDmaInitCh(pIpPort->m_pDvcData, pIpPort->m_PortIndex, 
                                  DTA_IPTX_NRT_DMA_BUF_SIZE+sizeof(DtaMacCSetupFrame),
                                  DTA_DMA_MODE_DEFAULT, DmaRegsOffsetTxNrt,
                                  DTA_DMA_FLAGS_DATA_BUF_NO_COPY | DTA_DMA_FLAGS_BLOCKING,
                                  -1, NULL, NULL, &pIpPortType1->m_TxNrt.m_DmaChannel,
                                  TRUE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_TX, "Error initialising IpTx Nrt DMA channel");
        return Status;
    }


    // Create and initialize DMA ping pong buffers for this port
    Status = DtaIpCreateDmaPPBuffer(&pIpPort->m_IpPortType1.m_TxRt.m_DmaChannel,
                                          &pIpPort->m_IpPortType1.m_TxRtPPBuffer, pIpPort,
                                          DTA_IPTX_RT_PINGPONG_BUF_SIZE, 
                                          DtaIpTxRtGetLocalAddress,
                                          DT_DMA_DIRECTION_TO_DEVICE, TRUE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_TX, "Error creating IpTx Rt ping-pong buffer");
        return Status;
    }

    // Create and initialise Nrt Tx
    Status = DtaIpTxNrtCreateBuffer(&pIpPort->m_IpPortType1.m_TxNrt.m_DmaChannel,
                                                                 &pIpPort->m_NrtChannels);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_TX, "Error creating IpTx Nrt DMA buffer");
        return Status;
    }

    Status = DtThreadInit(&pIpPort->m_IpPortType1.m_TxNrtThread, DtaIpTxNrtWorkerThread,
                                                                                 pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_TX, "Error initialising Nrt TxThread");
        return Status;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxInitType2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpTxInitType2(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  DmaRegsOffsetTxRt;
    UInt  DmaRegsOffsetTxNrt;
    DtaIpPortType2*  pIpPortType2 = &pIpPort->m_IpPortType2;
    DtPropertyData*  pPropData = &pIpPort->m_pDvcData->m_PropData;
    
    pIpPortType2->m_TxNrt.m_pRegs = NULL;

    // Initialise register offsets
    pIpPortType2->m_TxNrt.m_RegsOffset = DtPropertiesGetUInt16(pPropData,
                                             "NW_REGISTERS_NRT_TX", pIpPort->m_PortIndex);
    pIpPortType2->m_TxNrt.m_FifoOffset = 
                              pIpPortType2->m_TxNrt.m_RegsOffset + DTA_LOCALADDR_FIFODATA;

    DmaRegsOffsetTxNrt = DtPropertiesGetUInt16(pPropData, "NW_REGISTERS_NRT_TX_DMA", 
                                                                    pIpPort->m_PortIndex);

    pIpPortType2->m_TxRt.m_RegsOffset = DtPropertiesGetUInt16(pPropData,
                                              "NW_REGISTERS_RT_TX", pIpPort->m_PortIndex);
    pIpPortType2->m_TxRt.m_FifoOffset = 
                               pIpPortType2->m_TxRt.m_RegsOffset + DTA_LOCALADDR_FIFODATA;
    
    DmaRegsOffsetTxRt = DtPropertiesGetUInt16(pPropData, 
                                          "NW_REGISTERS_RT_TX_DMA", pIpPort->m_PortIndex);

    pIpPortType2->m_MemoryControllerOffset = DtPropertiesGetUInt16(pPropData,
                                             "NW_REGISTERS_MEMCTRL", pIpPort->m_PortIndex);

    // Report configuration errors
    // Check if no property error occurred
    Status = DtaPropertiesReportDriverErrors(pIpPort->m_pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Initialise Nrt Tx DMA channel
    // We must add extra space for the setup frame in case we use
    // common buf DMA.
    Status = DtaDmaInitCh(pIpPort->m_pDvcData, pIpPort->m_PortIndex, 
                                  DTA_IPTX_NRT_DMA_BUF_SIZE+sizeof(DtaMacCSetupFrame),
                                  DTA_DMA_MODE_DEFAULT, DmaRegsOffsetTxNrt,
                                  DTA_DMA_FLAGS_DATA_BUF_NO_COPY | DTA_DMA_FLAGS_BLOCKING,
                                  -1, NULL, NULL, &pIpPortType2->m_TxNrt.m_DmaChannel,
                                  TRUE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_TX, "Error initialising IpTx Nrt DMA channel");
        return Status;
    }

    // Initialise Rt Tx DMA channel
    // For Rt Tx we use a Ping-pong buffer to transfer the data.
    // For the complete ping-pong buffer, the SGL list is created
    // The actual DMA transfer size will be the size divided by 2.
    if (pIpPort->m_IpPortIndex%2 == 0)
    {
        Status = DtaDmaInitCh(pIpPort->m_pDvcData, pIpPort->m_PortIndex, 
                                      DTA_IPTX_RT_PINGPONG_BUF_SIZE, DTA_DMA_MODE_DEFAULT,
                                      DmaRegsOffsetTxRt, DTA_DMA_FLAGS_DATA_BUF_NO_COPY |
                                      DTA_DMA_FLAGS_NOINT_AFTER_DONE,
                                      -1, NULL, NULL,
                                      &pIpPortType2->m_TxRt.m_DmaChannel, TRUE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, IP_TX, "Error initialising IpTx Rt DMA channel");
            return Status;
        }
    }
    
    // Create and initialize DMA ping pong buffers for this port

    // Create and initialise Nrt Tx
    Status = DtaIpTxNrtCreateBuffer(&pIpPortType2->m_TxNrt.m_DmaChannel,
                                                                 &pIpPort->m_NrtChannels);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_TX, "Error creating IpTx Nrt DMA buffer");
        return Status;
    }

    Status = DtThreadInit(&pIpPortType2->m_TxNrtThread, DtaIpTxNrtWorkerThread,
                                                                                 pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP_TX, "Error initialising Nrt TxThread");
        return Status;
    }

    if (pIpPort->m_IpPortIndex%2 == 0)
    {
        // Create and initialize DMA ping pong buffers for this port
        Status = DtaIpCreateDmaPPBuffer(&pIpPort->m_IpPortType2.m_TxRt.m_DmaChannel,
                                                   &pIpPort->m_IpPortType2.m_TxRtPPBuffer,
                                                   pIpPort,
                                                   DTA_IPTX_RT_PINGPONG_BUF_SIZE, 
                                                   DtaIpTxRtGetLocalAddress,
                                                   DT_DMA_DIRECTION_TO_DEVICE, FALSE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, IP_TX, "Error creating IpTx Rt ping-pong buffer");
            return Status;
        }

        Status = DtDpcInit(&pIpPortType2->m_IpRtTxDpc, DtaIpTxRtProcessPacketsType2Dpc, 
                                                                                    TRUE);
        pIpPortType2->m_pIpRtTxDpc = &pIpPortType2->m_IpRtTxDpc;
    } else {
        DtaIpPort*  pIpPortOdd = 
                    &pIpPort->m_pDvcData->m_IpDevice.m_pIpPorts[pIpPort->m_IpPortIndex-1];
        pIpPortType2->m_pIpRtTxDpc = &pIpPortOdd->m_IpPortType2.m_IpRtTxDpc;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxCleanupType1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpTxCleanupType1(DtaIpPort* pIpPort)
{
    DtaIpPortType1*  pIpPortType1 = &pIpPort->m_IpPortType1;
    
    // Cleanup Rt Tx
    DtaDmaCleanupCh(pIpPort->m_pDvcData, &pIpPortType1->m_TxRt.m_DmaChannel);
    DtaIpCleanupDmaPPBuffer(&pIpPortType1->m_TxRt.m_DmaChannel,
                                                           &pIpPortType1->m_TxRtPPBuffer);
    // Cleanup Nrt Tx
    DtaDmaCleanupCh(pIpPort->m_pDvcData, &pIpPortType1->m_TxNrt.m_DmaChannel);
    DtaIpTxNrtCleanupBuffer(&pIpPortType1->m_TxNrt.m_DmaChannel, &pIpPort->m_NrtChannels);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxCleanupType2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpTxCleanupType2(DtaIpPort* pIpPort)
{
    DtaIpPortType2*  pIpPortType2 = &pIpPort->m_IpPortType2;
    
    // Cleanup Nrt Tx
    DtaDmaCleanupCh(pIpPort->m_pDvcData, &pIpPortType2->m_TxNrt.m_DmaChannel);
    DtaIpTxNrtCleanupBuffer(&pIpPortType2->m_TxNrt.m_DmaChannel, &pIpPort->m_NrtChannels);

    // Cleanup Rt Tx
    if (pIpPort->m_IpPortIndex%2 == 0)
    {
        DtaDmaCleanupCh(pIpPort->m_pDvcData, &pIpPortType2->m_TxRt.m_DmaChannel);
        DtaIpCleanupDmaPPBuffer(&pIpPortType2->m_TxRt.m_DmaChannel, 
                                                           &pIpPortType2->m_TxRtPPBuffer);
    }
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// User channel IOCTL's
//
DtStatus  DtaIpTxIoctl(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  IpPortIndex;           // Index in the Ip port struct
    DtaIoctlIpTxCmdInput*  pIpTxCmdInput = (DtaIoctlIpTxCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlIpTxCmdOutput*  pIpTxCmdOutput =
                                          (DtaIoctlIpTxCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlIpTxCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlIpTxCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    if (!DT_SUCCESS(DtaGetIpPortIndex(pDvcData, pIpTxCmdInput->m_PortIndex,
                                                                           &IpPortIndex)))
        return DT_STATUS_INVALID_PARAMETER;
    
    // Determine final required output/input sizes
    switch (pIpTxCmdInput->m_Cmd)
    {
    case DTA_IP_TX_CMD_CHANNEL_ATTACH:
        pCmdStr = "DTA_IP_TX_CMD_CHANNEL_ATTACH";
        OutReqSize += sizeof(DtaIoctlIpXxCmdAttachChannelOutput);
        InReqSize = 0;
        break;
    case DTA_IP_TX_CMD_CHANNEL_DETACH:
        pCmdStr = "DTA_IP_TX_CMD_CHANNEL_DETACH";
        OutReqSize = 0;        
        InReqSize += sizeof(DtaIoctlIpXxCmdAttachChannelOutput);
        break;
    case DTA_IP_TX_CMD_SETCONTROL:
        pCmdStr = "DTA_IP_TX_CMD_SETCONTROL";
        OutReqSize = 0;        
        InReqSize += sizeof(DtaIoctlIpXxCmdSetControlInput);
        break;
    case DTA_IP_TX_CMD_RESET:
        pCmdStr = "DTA_IP_TX_CMD_RESET";
        OutReqSize = 0;        
        InReqSize += sizeof(DtaIoctlIpXxCmdResetInput);
        break;
    case DTA_IP_TX_CMD_CLEARFLAGS:
        pCmdStr = "DTA_IP_TX_CMD_CLEARFLAGS";
        OutReqSize = 0;        
        InReqSize += sizeof(DtaIoctlIpXxCmdClearFlagsInput);
        break;
    case DTA_IP_TX_CMD_GETFLAGS:
        pCmdStr = "DTA_IP_TX_CMD_GETFLAGS";
        OutReqSize += sizeof(DtaIoctlIpXxCmdGetFlagsOutput);
        InReqSize += sizeof(DtaIoctlIpXxCmdGetFlagsInput);
        break;
    case DTA_IP_TX_CMD_SETIPPARS:
        pCmdStr = "DTA_IP_TX_CMD_SETIPPARS";
        OutReqSize = 0;
        InReqSize += sizeof(DtaIoctlIpTxCmdSetIpParsInput);
        break;
    case DTA_IP_TX_CMD_WRITENDISPCKT:
        pCmdStr = "DTA_IP_TX_CMD_WRITENDISPCKT";
        OutReqSize = 0;
        InReqSize += OFFSETOF(DtaIoctlIpTxCmdWriteNdisPcktInput, m_Buf); // Exclude m_Buf
        // Check for valid input buffer before reading it
        if (pIoctl->m_InputBufferSize >= InReqSize)
            InReqSize += pIpTxCmdInput->m_Data.m_WriteNdisPckt.m_BufLen;
        // We expect no output buffer
        OutReqSize = 0;
        break;
    default:
        pCmdStr = "??UNKNOWN IP_TX_CMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, IP_TX, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, IP_TX, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, IP_TX, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
                pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize,
                                                                              OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pIpTxCmdInput->m_Cmd)
        {
        case DTA_IP_TX_CMD_CHANNEL_ATTACH:
            Status = DtaIpTxUserChAttach(&pDvcData->m_IpDevice.m_IpUserChannels, 
                                       pIpTxCmdInput->m_PortIndex, IpPortIndex, pFile,
                                       &pIpTxCmdOutput->m_Data.m_AttachChannel.m_Channel);
            break;

        case DTA_IP_TX_CMD_CHANNEL_DETACH:
            Status = DtaIpTxUserChDetach(&pDvcData->m_IpDevice.m_IpUserChannels,
                                         pIpTxCmdInput->m_Data.m_DetachChannel.m_Channel);
            break;
        case DTA_IP_TX_CMD_CLEARFLAGS:
            Status = DtaIpTxUserChClearFlags(&pDvcData->m_IpDevice.m_IpUserChannels,
                                       pIpTxCmdInput->m_Data.m_ClearFlags.m_Channel,
                                       pIpTxCmdInput->m_Data.m_ClearFlags.m_Flags);
            break;
        case DTA_IP_TX_CMD_GETFLAGS:
            Status = DtaIpTxUserChGetFlags(&pDvcData->m_IpDevice.m_IpUserChannels,
                                       pIpTxCmdInput->m_Data.m_GetFlags.m_Channel,
                                       &pIpTxCmdOutput->m_Data.m_GetFlags.m_Flags, 
                                       &pIpTxCmdOutput->m_Data.m_GetFlags.m_LatchedFlags);
            break;
        case DTA_IP_TX_CMD_RESET:
            Status = DtaIpTxUserChReset(&pDvcData->m_IpDevice.m_IpUserChannels,
                                                 pIpTxCmdInput->m_Data.m_Reset.m_Channel);
            break;
        case DTA_IP_TX_CMD_SETCONTROL:
            Status = DtaIpTxUserChSetTxControl(&pDvcData->m_IpDevice.m_IpUserChannels, 
                                          pIpTxCmdInput->m_Data.m_SetTxControl.m_Channel,
                                          pIpTxCmdInput->m_Data.m_SetTxControl.m_Control);
            break;
        case DTA_IP_TX_CMD_SETIPPARS:
            Status = DtaIpTxUserChSetIpPars(&pDvcData->m_IpDevice.m_IpUserChannels, 
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_Channel, 
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_SrcIp,
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_SrcPort,
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_SrcMac,
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_DstIp,
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_DstPort,
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_DstPort2,
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_DstMac,
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_VlanId,
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_VlanPriority,
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_FecMode,
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_Protocol,
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_Mode,
                                         pIpTxCmdInput->m_Data.m_SetIpPars.m_Flags);
            break;
        case DTA_IP_TX_CMD_WRITENDISPCKT:
            Status = DtaIpTxWriteNDisPacket(&pDvcData->m_IpDevice.m_pIpPorts[IpPortIndex],
                                          pIpTxCmdInput->m_Data.m_WriteNdisPckt.m_Buf,
                                          pIpTxCmdInput->m_Data.m_WriteNdisPckt.m_BufLen);
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
            DtDbgOut(MIN, IP_TX, "IpTxCmd=0x%x: NOT SUPPORTED", pIpTxCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, IP_TX, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IP TX IOCTL Handlers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChAttach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Creates and adds an empty IP TX channel to its internal list. The index of this new
// channel is returned to the caller.
//
// Precondition: PortIndex is the 0 based index to the Port set by the user
//               IpPortIndex is the 0-based index to an IpPort
//
DtStatus  DtaIpTxUserChAttach(
    DtaIpUserChannels*  pIpUserChannels,
    Int  PortIndex,
    Int  IpPortIndex, 
    DtFileObject*  pFile,
    Int*  pChannelIndex)
{   
    UserIpTxChannel*  pIpTxChannel;
    Int  ChannelIndex = 0;
    
    DtFastMutexAcquire(&pIpUserChannels->m_IpTxChannelMutex);
    
    pIpTxChannel = pIpUserChannels->m_pIpTxChannel;

    // find a free index     
    while (pIpTxChannel != NULL)
    {
        // reuse a previously detached channel index if available
        if (pIpTxChannel->m_ChannelIndex != ChannelIndex)
            break;

        ChannelIndex++;
        pIpTxChannel = pIpTxChannel->m_pNext;
    }

    // create a channel with this index
    pIpTxChannel = DtaIpTxUserChCreate(ChannelIndex, PortIndex, IpPortIndex, pFile);
    if (pIpTxChannel == NULL)
    {
        DtFastMutexRelease(&pIpUserChannels->m_IpTxChannelMutex);
        return DT_STATUS_OUT_OF_MEMORY;
    }

    // Initialize and add it to our list:
    // Remark: make shure the DPC does not get access while updating the channel
    DtSpinLockAcquire(&pIpUserChannels->m_IpTxChannelSpinLock);
    pIpTxChannel->m_TxControl = DT_TXCTRL_IDLE;
    pIpTxChannel->m_IpParsValid = FALSE;
    DtaIpTxUserChAddToList(pIpUserChannels, pIpTxChannel);
    DtSpinLockRelease(&pIpUserChannels->m_IpTxChannelSpinLock);

    // return the index
    *pChannelIndex = ChannelIndex;

    DtFastMutexRelease(&pIpUserChannels->m_IpTxChannelMutex);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChDetach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Removes a IP TX channel from its internal list, and frees the used resources.
//
DtStatus  DtaIpTxUserChDetach(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex)
{    
    UserIpTxChannel*  pIpTxChannel;

    pIpTxChannel = DtaIpTxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpTxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Set tx to IDLE state before detaching
    DtaIpTxUserChSetTxControlIdle(pIpUserChannels, pIpTxChannel);
    
    // Destroy the channel completely
    DtaIpTxUserChDestroy(pIpUserChannels, pIpTxChannel);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChClearFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Clears the latched flags used in IP Tx. 
//
DtStatus  DtaIpTxUserChClearFlags(
    DtaIpUserChannels*  pIpUserChannels,
    Int  ChannelIndex,
    Int  Flags)
{
    UserIpTxChannel*  pIpTxChannel;

    pIpTxChannel = DtaIpTxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpTxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    DtSpinLockAcquire(&pIpTxChannel->m_FlagsSpinLock);
    pIpTxChannel->m_LatchedFlags &= ~Flags;
    DtSpinLockRelease(&pIpTxChannel->m_FlagsSpinLock);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChGetFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Returns the current flags and latched flags.
//
DtStatus  DtaIpTxUserChGetFlags(
    DtaIpUserChannels*  pIpUserChannels,
    Int  ChannelIndex, 
    Int*  pFlags, 
    Int*  pLatchedFlags)
{
    UserIpTxChannel* pIpTxChannel;

    pIpTxChannel = DtaIpTxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpTxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    DtSpinLockAcquire(&pIpTxChannel->m_FlagsSpinLock);
    *pFlags = pIpTxChannel->m_Flags;
    *pLatchedFlags = pIpTxChannel->m_LatchedFlags;
    DtSpinLockRelease(&pIpTxChannel->m_FlagsSpinLock);
        
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Resets a IP Tx channel
// Remark: currently only the Idle state is set, and its flags are cleared.
//
DtStatus  DtaIpTxUserChReset(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex)
{
    // Set control state to IDLE
    DtaIpTxUserChSetTxControl(pIpUserChannels, ChannelIndex, DT_TXCTRL_IDLE);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChSetIpPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtaIpTxUserChSetIpPars(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex,
             UInt8* pSrcIp, UInt16 SrcPort, UInt8* pSrcMac, UInt8* pDstIp, UInt16 DstPort,
             UInt16 DstPort2, UInt8* pDstMac, Int VlanId, Int VlanPriority, Int FecMode, 
             Int Protocol, Int Mode, Int Flags)
{
    Int i;

    UserIpTxChannel*  pIpTxChannel = DtaIpTxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpTxChannel == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Only allowed in Tx-Control state IDLE
    if (pIpTxChannel->m_TxControl != DT_TXCTRL_IDLE) 
    {
        DtDbgOut(ERR, IP_TX, "Setting IP pars not allowed if channel is NOT IDLE");
        return DT_STATUS_FAIL;
    }
    
    pIpTxChannel->m_IpParsMode = Mode;
    pIpTxChannel->m_IpParsValid = TRUE;
    pIpTxChannel->m_LastReadOffset = 0;
        
    if (Mode == DTA_IP_TX_DBLBUF)
    {
        Bool  IpV6 = (Flags & DTA_IP_V6) != 0;
        Bool  UseVlan = (VlanId!=0 || VlanPriority!=0);
        pIpTxChannel->m_IpParsFlags = Flags;
        pIpTxChannel->m_FecMode = FecMode;
        pIpTxChannel->m_IpParsMode = Mode;
        pIpTxChannel->m_IpParsFlags = Flags;
        
        // Set port number to big-endian for easy compare and set with original packet
        pIpTxChannel->m_SrcPort = DtUInt16ByteSwap(SrcPort);
        pIpTxChannel->m_DstPort = DstPort;
        pIpTxChannel->m_DstPortMain = DtUInt16ByteSwap(DstPort2);
        pIpTxChannel->m_DstPortMainRow = DtUInt16ByteSwap(DstPort2 + FEC_INC_ROW_PORT);
        pIpTxChannel->m_DstPortMainColumn = DtUInt16ByteSwap(DstPort2 +
                                                                     FEC_INC_COLUMN_PORT);
        
        // Build ethernet header
        if (UseVlan)
        {
            EthernetIIHeaderVlan*  pEthIIHeaderVlan = &pIpTxChannel->m_EthIIHeader.m_Vlan;
            pIpTxChannel->m_EthHeaderSize = sizeof(EthernetIIHeaderVlan);
            DtMemZero(pEthIIHeaderVlan, sizeof(EthernetIIHeaderVlan));
            pEthIIHeaderVlan->m_PCP = (UInt16)VlanPriority;
            pEthIIHeaderVlan->m_VId = (UInt16)VlanId;
            pEthIIHeaderVlan->m_TCI = DtUInt16ByteSwap(pEthIIHeaderVlan->m_TCI);
            for (i=0; i<6; i++) 
            {
                pEthIIHeaderVlan->m_SrcMac[i] = pSrcMac[i];
                pEthIIHeaderVlan->m_DstMac[i] = pDstMac[i];
            }
            pEthIIHeaderVlan->m_TPId = ETH_TYPE_VLAN_BE;
            pEthIIHeaderVlan->m_Type = (IpV6 ? ETH_TYPE_IPV6_BE : ETH_TYPE_IPV4_BE);
        } else {
            EthernetIIHeader*  pEthIIHeader = &pIpTxChannel->m_EthIIHeader.m_Default;
            pIpTxChannel->m_EthHeaderSize = sizeof(EthernetIIHeader);
            DtMemZero(pEthIIHeader, sizeof(EthernetIIHeader));
            pEthIIHeader->m_Type = (IpV6 ? ETH_TYPE_IPV6_BE : ETH_TYPE_IPV4_BE);
            for (i=0; i<6; i++) 
            {
                pEthIIHeader->m_SrcMac[i] = pSrcMac[i];
                pEthIIHeader->m_DstMac[i] = pDstMac[i];
                
            }
        }

        // Build part IP header
        if (IpV6)
        {
            for (i=0; i<16; i++)
            {
                pIpTxChannel->m_IpAddrHeader.m_IpAddressesV6[i] = pSrcIp[i];
                pIpTxChannel->m_IpAddrHeader.m_IpAddressesV6[i+16] = pDstIp[i];
            }
            pIpTxChannel->m_IpAddrHeaderSize = 
                                     sizeof(pIpTxChannel->m_IpAddrHeader.m_IpAddressesV6);
            pIpTxChannel->m_IpAddrHeaderOffset = OFFSETOF(IpHeaderV6, m_SrcAddress);
        } else {
            for (i=0; i<4; i++)
            {
                pIpTxChannel->m_IpAddrHeader.m_IpAddressesV4[i] = pSrcIp[i];
                pIpTxChannel->m_IpAddrHeader.m_IpAddressesV4[i+4] = pDstIp[i];
            }
            pIpTxChannel->m_IpAddrHeaderSize = 
                                     sizeof(pIpTxChannel->m_IpAddrHeader.m_IpAddressesV4);
            pIpTxChannel->m_IpAddrHeaderOffset = OFFSETOF(IpHeaderV4, m_SrcAddress);
        }
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChSetTxControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Puts the IP Tx in SEND/HOLD or IDLE state
//
DtStatus  DtaIpTxUserChSetTxControl(
    DtaIpUserChannels*  pIpUserChannels,
    Int  ChannelIndex, 
    Int  TxControl)
{
    UserIpTxChannel*  pIpTxChannel = NULL;
    DtaDeviceData*  pDvcData = pIpUserChannels->m_pDvcData;
    UInt64  ReferenceDelay = 0;
    UInt32  PeriodicIntMs = DtaGetPerIntItvUS(pDvcData)/1000;
    
    pIpTxChannel = DtaIpTxUserChGet(pIpUserChannels, ChannelIndex);
    if (pIpTxChannel == NULL)
        return  DT_STATUS_INVALID_PARAMETER;

    if (TxControl == DT_TXCTRL_SEND)
    {
        DtSpinLockAcquire(&pIpUserChannels->m_IpTxChannelSpinLock);
        if (!pIpTxChannel->m_SharedBuffer.m_Initialised)
        {
            DtSpinLockRelease(&pIpUserChannels->m_IpTxChannelSpinLock);
            return DT_STATUS_FAIL;
        }

        // Set initial reference timestamp with a delay off:
        // 5 * periodic interrupt cycles (this normally will have a 100ms delay)
        // Delay = Clk(kHz) * period (ms)
        ReferenceDelay = 5 * (pDvcData->m_DevInfo.m_RefClk/1000) * PeriodicIntMs;
        pIpTxChannel->m_RefTimestamp = DtaRegRefClkCntGet64(pDvcData->m_pGenRegs) + 
                                                                           ReferenceDelay;
        pIpTxChannel->m_pTxBufferHead->m_StartTimestamp = pIpTxChannel->m_RefTimestamp;

        pIpTxChannel->m_LastTimestampOffs = 0;
        pIpTxChannel->m_TxControl = DT_TXCTRL_SEND;
        DtSpinLockRelease(&pIpUserChannels->m_IpTxChannelSpinLock);
        return DT_STATUS_OK;
    }
    
    if (TxControl == DT_TXCTRL_HOLD)
    {
        if (!pIpTxChannel->m_SharedBuffer.m_Initialised)
            return DT_STATUS_FAIL;
        
        DtSpinLockAcquire(&pIpUserChannels->m_IpTxChannelSpinLock);
        pIpTxChannel->m_TxControl = DT_TXCTRL_HOLD;
        DtSpinLockRelease(&pIpUserChannels->m_IpTxChannelSpinLock);
    } else {
        // Set to idle
        DtaIpTxUserChSetTxControlIdle(pIpUserChannels, pIpTxChannel);
    }
    
    // reset the fifo underflow flag
    DtSpinLockAcquire(&pIpTxChannel->m_FlagsSpinLock);
    pIpTxChannel->m_Flags &= ~DTA_TX_FIFO_UFL;
    DtSpinLockRelease(&pIpTxChannel->m_FlagsSpinLock);

    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Returns an IP TX channel object based on its channel index
//
UserIpTxChannel*  DtaIpTxUserChGet(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex)
{
    UserIpTxChannel*  pIpTxChannel;

    DtFastMutexAcquire(&pIpUserChannels->m_IpTxChannelMutex);
    pIpTxChannel = pIpUserChannels->m_pIpTxChannel;
    while (pIpTxChannel != NULL)
    {        
        if (pIpTxChannel->m_ChannelIndex == ChannelIndex)
            break;

        pIpTxChannel = pIpTxChannel->m_pNext;
    }
    DtFastMutexRelease(&pIpUserChannels->m_IpTxChannelMutex);
    DT_ASSERT(pIpTxChannel != NULL);
    return pIpTxChannel;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChDestroy -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpTxUserChDestroy(
    DtaIpUserChannels*  pIpUserChannels,
    UserIpTxChannel*  pIpTxChannel)
{
    // gain access to the channel list
    DtFastMutexAcquire(&pIpUserChannels->m_IpTxChannelMutex);

    // Note: We cannot free the shared buffer when we hold a spinlock
    //       This is a linux issue where holding a spinlock is considered as 
    //       being in an interrupt. This normally would trigger an exception in 'vunmap'.
    
    // Remove the channel from the list, it is now no longer available for the DPC
    DtSpinLockAcquire(&pIpUserChannels->m_IpTxChannelSpinLock);
    DtaIpTxUserChRemoveFromList(pIpUserChannels, pIpTxChannel);
    DtSpinLockRelease(&pIpUserChannels->m_IpTxChannelSpinLock);
    
    // It is safe now to free internal resources (see remarks above)    
    DtaShBufferClose(&pIpTxChannel->m_SharedBuffer);
    DtMemFreePool(pIpTxChannel, IPTX_TAG);
    pIpTxChannel = NULL;
    
    DtFastMutexRelease(&pIpUserChannels->m_IpTxChannelMutex);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxRtReportOverFlow -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpTxRtReportOverFlow(DtStatus Status, UserIpTxChannel* pIpTxChannel)
{    
    if (Status != DT_STATUS_BUFFER_OVERFLOW)
    {
        // reset overflow flag
        // No need to acquire spinlock    
        pIpTxChannel->m_Flags &= ~DTA_TX_FIFO_OVF;
        return;
    }    

    DtSpinLockAcquireAtDpc(&pIpTxChannel->m_FlagsSpinLock);
    pIpTxChannel->m_Flags |= DTA_TX_FIFO_OVF;
    pIpTxChannel->m_LatchedFlags |= DTA_TX_FIFO_OVF;
    DtSpinLockReleaseFromDpc(&pIpTxChannel->m_FlagsSpinLock);    
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxRtProcessPacketsType1Dpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Process all channels; Read a packet from the shared buffer, and put them in the output-
// (DMA) buffer.
//
void  DtaIpTxRtProcessPacketsType1Dpc(DtDpcArgs* pArgs)
{    
    DtStatus  Status = DT_STATUS_OK;
    UserIpTxChannel*  pIpTxChannel = NULL;
    DtaDmaTxHeader*  pDmaTxHeader = NULL;
    UInt8*  pSourceBuffer = NULL;
    DtaIpPort*  pIpPort = NULL;
    UInt  PacketLength;
    PPBuffer*  pPPBuffer = NULL;
    UInt64  MaxPacketTime;
    UInt64  MaxDelay;
    UInt32  PrvTimestampOffset;
    DtaIpUserChannels*  pIpUserChannels;
    DtaDeviceData*  pDvcData;
    DtaIpDevice* pIpDevice;
    UInt32  PeriodicIntMs;
    // Port status: NOT_STARTED, BUSY, BUFFER_OVERFLOW, OUT_OF_RESOURCES
    DtStatus PortStatus[4];
    Int  CurrentPort;

    
#ifdef  _DEBUG
    UInt64  Delay;
    UInt64  StartTime;
    UInt64  CurTime;
#endif

    pIpDevice = (DtaIpDevice*)pArgs->m_pContext;
    pIpUserChannels = &pIpDevice->m_IpUserChannels;
    pDvcData = pIpUserChannels->m_pDvcData;
    PeriodicIntMs = DtaGetPerIntItvUS(pDvcData)/1000;
    DT_ASSERT(pDvcData->m_NumIpPorts <= 4);

#ifdef  _DEBUG
    StartTime = DtaRegRefClkCntGet64(pDvcData->m_pGenRegs);    
#endif    

    // Protect channels for access during processing
    DtSpinLockAcquireAtDpc(&pIpUserChannels->m_IpTxChannelSpinLock);

    // Determine the maximum packet time reference
    MaxDelay = 2 * (pDvcData->m_DevInfo.m_RefClk/1000) * PeriodicIntMs;
    MaxPacketTime = DtaRegRefClkCntGet64(pDvcData->m_pGenRegs) + MaxDelay;

    // Reset port status to defaults
    for (CurrentPort=0; CurrentPort<pDvcData->m_NumIpPorts; CurrentPort++)
        PortStatus[CurrentPort] = DT_STATUS_NOT_STARTED;

    // Fill the PPBuffer
    while (TRUE)
    {   
        // Start filling the buffer with the channel with the newest packet
        pIpTxChannel = DtaIpTxUserChFindNewPacket(PortStatus, pIpUserChannels, 
                                                                           MaxPacketTime);
        if (pIpTxChannel == NULL)
            break;
        
        CurrentPort = pIpTxChannel->m_IpPortIndex;

        // Get port, Dma channel and pingpong buffer
        pIpPort = &pIpDevice->m_pIpPorts[CurrentPort];
        DT_ASSERT(pIpPort->m_PortType == DTA_IPPORT_TYPE1);
        pPPBuffer = &pIpPort->m_IpPortType1.m_TxRtPPBuffer;

        // If a port is not processed yet, check if a buffer is available
        // and clear this buffer
        if (PortStatus[CurrentPort] == DT_STATUS_NOT_STARTED)
        {
            // If the current buffer is full we have an error.
            // This means that the last (DMA) transaction for this buffer is not 
            // completed (yet).
            if (!DtaPPBufferWriteDataIsBufAvailable(pPPBuffer))
            {
                DtDbgOut(AVG, DTA, "No buffer available on port %d (DMA delayed).",
                                                             pIpTxChannel->m_IpPortIndex);
                PortStatus[CurrentPort] = DT_STATUS_OUT_OF_RESOURCES;
                continue;
            }

            // A buffer is available, set size to 0
            DtaPPBufferWriteDataClearBuf(pPPBuffer);
            PortStatus[CurrentPort] = DT_STATUS_BUSY;
        }        

        // copy new packet from this channel into the buffer
        pSourceBuffer = pIpTxChannel->m_pBuffer;
        pDmaTxHeader = (DtaDmaTxHeader*)(pSourceBuffer + 
                                             pIpTxChannel->m_pTxBufferHead->m_ReadOffset);
        
        // Determine packetlenght and round up to DWORD's
        PacketLength = pDmaTxHeader->m_TxHeaderGen.m_TransmitControl.m_PacketLength + 
                                                                   sizeof(DtaDmaTxHeader);
        PacketLength += (PacketLength % 4 == 0 ? 0 : 4 - (PacketLength % 4));
   
        // Check if the port for this channel is in Up state
        pIpPort = &pIpDevice->m_pIpPorts[CurrentPort];

        // Add packet to DMA buffer
        if (pIpPort->m_PhyMac.m_NetworkAdminStatus == DTA_NW_ADMIN_STATUS_UP ||
                                                       pIpPort->m_PhyMac.m_LoopbackEnable)
        {
            // Add data to the buffer ( if there is no overflow)
            if (PortStatus[CurrentPort] != DT_STATUS_BUFFER_OVERFLOW)
            {
                // Update the DMA timestamp (the original timestamp wraps around)
                // Make a copy of the offset for restore in case we get an overflow.
                PrvTimestampOffset = pIpTxChannel->m_LastTimestampOffs;
                pIpTxChannel->m_LastTimestampOffs = pDmaTxHeader->m_Timestamp;
                pDmaTxHeader->m_Timestamp = (UInt32)(pIpTxChannel->m_RefTimestamp + 
                                                               pDmaTxHeader->m_Timestamp);
                
                // Store packet in the buffer 
                Status = DtaPPBufferWriteData(pSourceBuffer + 
                    pIpTxChannel->m_pTxBufferHead->m_ReadOffset, pPPBuffer, PacketLength);

                DT_ASSERT(Status==DT_STATUS_OK || Status==DT_STATUS_BUFFER_OVERFLOW);
                
                // report a buffer overflow
                if (Status == DT_STATUS_BUFFER_OVERFLOW)
                {
                    PortStatus[CurrentPort] = DT_STATUS_BUFFER_OVERFLOW;
                    // Restore the timestamps for future processing
                    pDmaTxHeader->m_Timestamp = pIpTxChannel->m_LastTimestampOffs;
                    pIpTxChannel->m_LastTimestampOffs = PrvTimestampOffset;

                    DtDbgOut(ERR, DTA, "OVERFLOW: Channel %d Ip Port %d, PacketLen %d",
                                  pIpTxChannel->m_ChannelIndex, CurrentPort,PacketLength);

                }
            }
        }
       
        if (PortStatus[CurrentPort] == DT_STATUS_BUFFER_OVERFLOW)
        {
            // Report an overflow if needed, and skip processing the rest of the packages 
            // for this port.
            // If not reported, the next time this DPC is called, another attempt is made
            // to send the remaining packets.
            if (pIpTxChannel->m_ReportOverFlow)
                DtaIpTxRtReportOverFlow(PortStatus[CurrentPort], pIpTxChannel);
            else                
                continue; // Do not increase the read pointer
        }

        // Update the read pointer of the source
        if (pIpTxChannel->m_pTxBufferHead->m_ReadOffset+PacketLength > 
                      pIpTxChannel->m_pTxBufferHead->m_BufSize-DTA_IPTX_MAX_PACKET_LENGTH)
            pIpTxChannel->m_pTxBufferHead->m_ReadOffset = 0;
        else
            pIpTxChannel->m_pTxBufferHead->m_ReadOffset += PacketLength;
    }
    
    // Do the actual DMA transfer for every port
    for (CurrentPort=0; CurrentPort<pDvcData->m_NumIpPorts; CurrentPort++)
    { 
        // Nothing to do for this port
        if (PortStatus[CurrentPort]==DT_STATUS_NOT_STARTED ||
                                      PortStatus[CurrentPort]==DT_STATUS_OUT_OF_RESOURCES)
            continue;

        // Do not transmit if no data is added to the buffer
        pIpPort = &pIpDevice->m_pIpPorts[CurrentPort];
        pPPBuffer = &pIpPort->m_IpPortType1.m_TxRtPPBuffer;
        if (DtaPPBufferGetTransferSize(pPPBuffer) == 0)
            continue;
        
        // Set the buffer to ready state
        DtaPPBufferWriteDataFinish(pPPBuffer);
        
        // Start transfer
        Status = DtaPPBufferTransferData(pPPBuffer);
    }    

    DtSpinLockReleaseFromDpc(&pIpUserChannels->m_IpTxChannelSpinLock);

#ifdef  _DEBUG
    CurTime = DtaRegRefClkCntGet64(pDvcData->m_pGenRegs);    
    CurTime = CurTime - StartTime;
    Delay = DtDivide64((CurTime * 10000), (pDvcData->m_DevInfo.m_RefClk/1000), NULL);
    Delay = DtDivide64(Delay, 10000, NULL);
    if (Delay > PeriodicIntMs) 
    {
        DtDbgOut(ERR, DTA, "Functions takes TOO LONG (%u > %u ms)!!\n", 
                                                    (UInt32)Delay, (UInt32)PeriodicIntMs);
    }
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxRtProcessPacketsType2Dpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Process all channels; Read a packet from the shared buffer, and put them in the output-
// (DMA) buffer for the current port.
//
void  DtaIpTxRtProcessPacketsType2Dpc(DtDpcArgs* pArgs)
{    
    DtStatus  Status = DT_STATUS_OK;
    UserIpTxChannel*  pIpTxChannel = NULL;
    DtaDmaTxHeader*  pDmaTxHeader = NULL;
    UInt8*  pSourceBuffer = NULL;
    DtaIpPort*  pIpPort = NULL;
    DtaIpPort*  pIpPortFirst = NULL;
    DtaIpPort*  pIpPortTransfer = NULL;
    UInt  PacketLength;
    PPBuffer*  pPPBuffer = NULL;
    UInt64  MinPacketTime;
    UInt64  MaxPacketTime;
    UInt64  PacketTime;
    UInt32  PrvTimestampOffset;
    DtaIpUserChannels*  pIpUserChannels;
    DtaDeviceData*  pDvcData;
    DtStatus  PortStatus;  // NOT_STARTED, BUSY, BUFFER_OVERFLOW, OUT_OF_RESOURCES
    Bool  DblBuf;
    UInt  TotalNumBytesInBuffer = 0;
    UInt  MaxNumBytesInBuffer;
    Int  OverheadPerPacket = 20 - 32 - 2 + 4; // network overhead - DMA header - padding 
                                              // + CRC
    UInt32  ReadOffset;
    UInt32  WriteOffset;
    Bool  FirstPort;
    UInt32*  pLastTimestampOffs;
    UInt64*  pRefTimestamp;
    UInt64*  pStartTimestamp;
    UInt64  CurTime;

#ifdef  _DEBUG
    UInt64  Delay;
    UInt64  StartTime;
    Bool  PacketTooOldMsg;
    Bool FirstTime = TRUE;
#endif

    // Get IP port from interrupt.
    // We have to start the other port, and need to prepare this one
    pIpPort = (DtaIpPort*)pArgs->m_pContext;
    pDvcData = pIpPort->m_pDvcData;
    FirstPort = (pIpPort->m_IpPortIndex%2) == 0;
    
    pIpPortFirst = &pDvcData->m_IpDevice.m_pIpPorts[pIpPort->m_IpPortIndex&0xfffffffe];
    pIpPortTransfer = &pDvcData->m_IpDevice.m_pIpPorts[(FirstPort?
                                      pIpPort->m_IpPortIndex+1:pIpPort->m_IpPortIndex-1)];
    pIpUserChannels = &pDvcData->m_IpDevice.m_IpUserChannels;
    pPPBuffer = &pIpPortFirst->m_IpPortType2.m_TxRtPPBuffer;
    DT_ASSERT(pIpPort->m_PortType == DTA_IPPORT_TYPE2);

    // Calculate MAX transfer size per slice for specific PHY speed
    MaxNumBytesInBuffer = 2425000; // (1GB/8bits)/ms * slicetime =
                                   // ((1000000000/8)/1000) * 19,4;
    if (pIpPort->m_PhyMac.m_PhySpeed==DTA_PHY_SPEED_100_HALF || 
                                     pIpPort->m_PhyMac.m_PhySpeed==DTA_PHY_SPEED_100_FULL)
        MaxNumBytesInBuffer = 242500;
    else if (pIpPort->m_PhyMac.m_PhySpeed==DTA_PHY_SPEED_10_HALF ||
                                      pIpPort->m_PhyMac.m_PhySpeed==DTA_PHY_SPEED_10_FULL)
        MaxNumBytesInBuffer = 24250;

    // The firmware garanties that the DMA is completed when the slice-interrupt is
    // fired. So if DMA is not ready, we have to execute the completed DPC
    // Update state
    if (!DtaDmaIsReady(&pIpPortFirst->m_IpPortType2.m_TxRt.m_DmaChannel))
        ExecuteDmaCompletedFromDpc(&pIpPortFirst->m_IpPortType2.m_TxRt.m_DmaChannel);
    if (DtaDmaIsAbortActive(&pIpPortFirst->m_IpPortType2.m_TxRt.m_DmaChannel))
        return;
#ifdef  _DEBUG
   // StartTime = DtaRegRefClkCntGet64(pDvcData->m_pGenRegs);
#endif    

    // Protect channels for access during processing
    DtSpinLockAcquireAtDpc(&pIpUserChannels->m_IpTxChannelSpinLock);
    DT_ASSERT(DtaPPBufferWriteDataIsBufAvailable(pPPBuffer));
    if (!DtaPPBufferWriteDataIsBufAvailable(pPPBuffer))
    {
        DtDbgOut(ERR, IP_TX, "[%i] DMA interrupt, but no buffer available!! Error!", 
                                                                  pIpPort->m_IpPortIndex);
    }

    // Start transfer previous prepared. If nothing prepared, nothing is transferred
    Status = DtaPPBufferTransferDataContext(pPPBuffer, pIpPortTransfer);
    
    CurTime = DtaRegRefClkCntGet64(pDvcData->m_pGenRegs);

    // The current time is the time that the prepared packets must be send.
    // We have to prepare the packet for the new timeslice. Add one half period time
    // to current time for current port.

    // Determine the maximum packet time reference for 9,7 ms
    //MinPacketTime = CurTime &~0x7ffff;
    //MaxPacketTime = MinPacketTime + 0x7ffff;

    // Determine the maximum packet time reference for 19,4 ms
    MinPacketTime = (CurTime+0x7ffff) &~0xfffff;
    MaxPacketTime = MinPacketTime + 0xfffff;

    PortStatus = DT_STATUS_NOT_STARTED;
    DtaPPBufferWriteDataClearBuf(pPPBuffer);

    // Fill the PPBuffer
    pIpTxChannel = pIpUserChannels->m_pIpTxChannel;
    
    while (pIpTxChannel != NULL)
    {
        DblBuf = pIpTxChannel->m_IpParsValid && 
                                             pIpTxChannel->m_IpParsMode==DTA_IP_TX_DBLBUF;

        if (pIpTxChannel->m_TxControl != DT_TXCTRL_SEND)
        {
           pIpTxChannel = pIpTxChannel->m_pNext;
           continue; // nothing to send, goto the next channel
        }

        // For DlbBuf we also need to parse the second port
        if (pIpTxChannel->m_IpPortIndex!=pIpPort->m_IpPortIndex && 
                         (!DblBuf || 
                         (DblBuf&&pIpTxChannel->m_IpPortIndex+1!=pIpPort->m_IpPortIndex)))
        {
           pIpTxChannel = pIpTxChannel->m_pNext;
           continue; // For other port, goto the next channel
        }

        // Skip processing for this channel if the port has an overflow
        // and it is NOT reported. 
        // If reported, packets are processed, but not transmitted.
        if (PortStatus==DT_STATUS_BUFFER_OVERFLOW && !pIpTxChannel->m_ReportOverFlow)
        {
            pIpTxChannel = pIpTxChannel->m_pNext;
            continue;
        }

        if (!DblBuf || (DblBuf && FirstPort))
        {
            pLastTimestampOffs = &pIpTxChannel->m_LastTimestampOffs;
            pRefTimestamp = &pIpTxChannel->m_RefTimestamp;
            pStartTimestamp = (UInt64*)&pIpTxChannel->m_pTxBufferHead->m_StartTimestamp;
            // Following only used for DblBuf processing
            // Start at same reference in case previous transfer was not completly 
            // transfered
            pIpTxChannel->m_LastTimestampOffs2 = pIpTxChannel->m_LastTimestampOffs; 
            pIpTxChannel->m_RefTimestamp2 = pIpTxChannel->m_RefTimestamp;
        } else {
            pLastTimestampOffs = &pIpTxChannel->m_LastTimestampOffs2;
            pRefTimestamp = &pIpTxChannel->m_RefTimestamp2;   
            pStartTimestamp = NULL;
        }
#ifdef _DEBUG
        PacketTooOldMsg = FALSE;
#endif
        ReadOffset = pIpTxChannel->m_pTxBufferHead->m_ReadOffset;
        WriteOffset = (FirstPort || !DblBuf?pIpTxChannel->m_pTxBufferHead->m_WriteOffset:
                                                          pIpTxChannel->m_LastReadOffset);

#ifdef  _DEBUG
        if (FirstTime)
        {
            //DtDbgOut(MAX, IP_TX, "Port %i: CurTime:%I64x MinTime:%I64x. "
            //              "MaxTime:%I64x. RO:%i WO:%i RO2:%i WO2:%i",
            //              pIpPort->m_IpPortIndex, CurTime, MinPacketTime, MaxPacketTime,
            //              pIpTxChannel->m_pTxBufferHead->m_ReadOffset, 
            //              pIpTxChannel->m_pTxBufferHead->m_WriteOffset, 
            //              ReadOffset, WriteOffset);
            FirstTime = FALSE;
        }
#endif
        // Check if packets are available for this channel
        // and handle underflow situation
        if (ReadOffset == WriteOffset)
        {
            if (!DblBuf || (DblBuf&&FirstPort))
            {
               // Report underflow, if not yet reported
               DtSpinLockAcquireAtDpc(&pIpTxChannel->m_FlagsSpinLock);
               if ((pIpTxChannel->m_Flags & DTA_TX_FIFO_UFL) == 0)
               {
                    DtDbgOut(ERR, IP_TX, "Detected UNDERFLOW ReadOffset %u, "
                                            "WriteOffset: %u",
                                            pIpTxChannel->m_pTxBufferHead->m_ReadOffset,
                                            pIpTxChannel->m_pTxBufferHead->m_WriteOffset);

                    pIpTxChannel->m_Flags |= DTA_TX_FIFO_UFL;
                    pIpTxChannel->m_LatchedFlags |= DTA_TX_FIFO_UFL;
               }
               DtSpinLockReleaseFromDpc(&pIpTxChannel->m_FlagsSpinLock);
            }
            pIpTxChannel = pIpTxChannel->m_pNext;
            continue; // Nothing to send, goto the next channel
        }

        // Search for all packets to transmit for this channel between
        // MinPacketTime and MaxPacketTime
        while (TRUE)
        {
            pSourceBuffer = pIpTxChannel->m_pBuffer;
            pDmaTxHeader = (DtaDmaTxHeader*)(pSourceBuffer + ReadOffset);
            DT_ASSERT(pDmaTxHeader->m_Tag == 0x445441A0);

            // Update timestamp references for this channel
            DtaIpTxUserChUpdateReferenceTimeStamps(pDmaTxHeader, pStartTimestamp,
                                            pRefTimestamp, pLastTimestampOffs, pDvcData,
                                            (pIpTxChannel->m_Flags&DTA_TX_FIFO_UFL) != 0);

            PacketTime = *pRefTimestamp + pDmaTxHeader->m_Timestamp;
            //DtDbgOut(MAX, IP_TX, "Port %i: CurTime:%I64x MinTime:%I64x. MaxTime:%I64x."
            //                 " RO:%i WO:%i RO2:%i WO2:%i PktTime: %I64xx",
            //                 pIpPort->m_IpPortIndex, CurTime, MinPacketTime,
            //                 MaxPacketTime, pIpTxChannel->m_pTxBufferHead->m_ReadOffset,
            //                 pIpTxChannel->m_pTxBufferHead->m_WriteOffset, 
            //                 ReadOffset, WriteOffset, PacketTime);
            
            // Reset underflow flag
            // No need to acquire spinlock, the DPC is the only one writing this flag
            pIpTxChannel->m_Flags &= ~DTA_TX_FIFO_UFL;

            PacketLength = pDmaTxHeader->m_TxHeaderGen.m_TransmitControl.m_PacketLength + 
                                                                   sizeof(DtaDmaTxHeader);
            PacketLength += (PacketLength % 4 == 0 ? 0 : 4 - (PacketLength % 4));
            
            if (PacketTime < MinPacketTime)
            {
                *pLastTimestampOffs = pDmaTxHeader->m_Timestamp;
#ifdef _DEBUG
                // For now, we skip this packet
                if (!PacketTooOldMsg)
                {
                    DtDbgOut(MAX, IP_TX, "Port %i: Packet too old, skip. Packettime:"
                                       "%llx CurTime: %llx MinTime: %llx. MaxTime: %llx.",
                                       pIpPort->m_IpPortIndex, CurTime, PacketTime, 
                                       MinPacketTime, MaxPacketTime);
                    PacketTooOldMsg = TRUE;
                }
#endif
            } else if (PacketTime > MaxPacketTime)  // Packet not for this slice
            {
                //DtDbgOut(ERR, IP_TX, "Packet not for this timeslice. Packettime: %I64x"
                //                           " MaxTime: %I64x.", PacketTime, MaxPacketTime);
                break;
            }
            else {
                // Copy packet to destination buffer
                if (pIpPort->m_PhyMac.m_NetworkAdminStatus == DTA_NW_ADMIN_STATUS_UP)
                {
                    // Add data to the buffer (if there is no overflow)
                    if (PortStatus != DT_STATUS_BUFFER_OVERFLOW)
                    {
                        // Update the DMA timestamp (the original timestamp wraps around)
                        // Make a copy of the offset for restore in case we get an 
                        // overflow.
                        PrvTimestampOffset = *pLastTimestampOffs;
                        *pLastTimestampOffs = pDmaTxHeader->m_Timestamp;
                        pDmaTxHeader->m_Timestamp = (UInt32)(*pRefTimestamp +
                                                               pDmaTxHeader->m_Timestamp);

                        if (TotalNumBytesInBuffer+PacketLength+OverheadPerPacket > 
                                                                      MaxNumBytesInBuffer)
                        {
                            Status = DT_STATUS_BUFFER_OVERFLOW;
                            DtDbgOut(ERR, DTA, "OVERFLOW BY MAX BITRATE: "
                                                     "Channel %d Ip Port %d,"
                                                     "PacketLen %d",
                                                     pIpTxChannel->m_ChannelIndex,
                                                     pIpPort->m_IpPortIndex,PacketLength);
                        } else {
                            TotalNumBytesInBuffer+= PacketLength+OverheadPerPacket;

                            if (DblBuf && !FirstPort)
                            {
                                // Modify packet for 2nd port
                                DtaIpTxReBuildPacket(pIpTxChannel, 
                                                  pSourceBuffer+ReadOffset, PacketLength);
                            }
                            
                            // Store packet in the buffer
                            Status = DtaPPBufferWriteData(pSourceBuffer + 
                                              ReadOffset,
                                              pPPBuffer, PacketLength);
                        }
                        
                        
                        DT_ASSERT(Status==DT_STATUS_OK || 
                                                       Status==DT_STATUS_BUFFER_OVERFLOW);
                
                        // Restore the timestamps for future processing
                        pDmaTxHeader->m_Timestamp = *pLastTimestampOffs;
                            
                        // Report a buffer overflow
                        if (Status == DT_STATUS_BUFFER_OVERFLOW)
                        {
                            PortStatus = DT_STATUS_BUFFER_OVERFLOW;
                            *pLastTimestampOffs = PrvTimestampOffset;

                            DtDbgOut(ERR, DTA, "OVERFLOW: Channel %d Ip Port %d,"
                                                     "PacketLen %d",
                                                     pIpTxChannel->m_ChannelIndex,
                                                     pIpPort->m_IpPortIndex,PacketLength);
                        }
                    }
                }
       
                if (PortStatus == DT_STATUS_BUFFER_OVERFLOW)
                {
                    // Report an overflow if needed, and skip processing the rest of the
                    // packages for this port.
                    // If not reported, the next time this DPC is called, another attempt
                    // is made to send the remaining packets.
                    if (pIpTxChannel->m_ReportOverFlow)
                        DtaIpTxRtReportOverFlow(PortStatus, pIpTxChannel);
                    else                
                        break; // Do not increase the read pointer
                }
            }
        

            // Update the read pointer of the source
            if (ReadOffset+PacketLength > 
                      pIpTxChannel->m_pTxBufferHead->m_BufSize-DTA_IPTX_MAX_PACKET_LENGTH)
                ReadOffset = 0;
            else
                ReadOffset += PacketLength;

            // Check if there are more packets in the buffer
            if (ReadOffset == WriteOffset)
                break;
        }

        if (!DblBuf || (DblBuf && !FirstPort))
            pIpTxChannel->m_pTxBufferHead->m_ReadOffset = ReadOffset;
        pIpTxChannel->m_LastReadOffset = ReadOffset;
        pIpTxChannel = pIpTxChannel->m_pNext;
    }

    // Check if length == 0, if so, we have to insert a dummy packet
    if (DtaPPBufferGetTransferSize(pPPBuffer) == 0)
    {
        DtaDmaTxHeader  TxHeader;

        DtMemZero(&TxHeader, sizeof(DtaDmaTxHeader));
        TxHeader.m_Length = sizeof(DtaDmaTxHeader);
        TxHeader.m_Tag = 0x445441A0;
        TxHeader.m_Timestamp = (UInt32)MinPacketTime;
        TxHeader.m_Version = 3;
        TxHeader.m_TxHeaderGen.m_TransmitControl.m_PacketLength = 0;
        Status = DtaPPBufferWriteData((UInt8*)&TxHeader, pPPBuffer, sizeof(TxHeader));
        DT_ASSERT(Status==DT_STATUS_OK);
    }

    // Set the buffer to ready state
    DtaPPBufferWriteDataFinish(pPPBuffer);

    DtSpinLockReleaseFromDpc(&pIpUserChannels->m_IpTxChannelSpinLock);

#ifdef  _DEBUG
    /*CurTime = DtaRegRefClkCntGet64(pDvcData->m_pGenRegs);
    CurTime = CurTime - StartTime;
    Delay = DtDivide64((CurTime * 10000), (pDvcData->m_DevInfo.m_RefClk/1000), NULL);
    Delay = DtDivide64(Delay, 10000, NULL);
    if (Delay >= 10) 
    {
        DtDbgOut(ERR, DTA, "Functions takes TOO LONG (%u >= %u ms)!!\n", 
                                                    (UInt32)Delay, (UInt32)10);
    }*/
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxReBuildPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpTxReBuildPacket(UserIpTxChannel* pIpTxChannel, UInt8* pBuffer, 
                                                                        UInt PacketLength)
{
    // For now, assume always 2 bytes packing
    UInt8*  pBuf = pBuffer + sizeof(DtaDmaTxHeader) + 2;
    UdpHeader*  pUdpHeader;
    
    // Copy ethernet header
    DtMemCopy(pBuf, (UInt8*)&pIpTxChannel->m_EthIIHeader, pIpTxChannel->m_EthHeaderSize);
    pBuf+= pIpTxChannel->m_EthHeaderSize + pIpTxChannel->m_IpAddrHeaderOffset;
    
    // Copy IP address
    DtMemCopy(pBuf, (UInt8*)&pIpTxChannel->m_IpAddrHeader, 
                                                        pIpTxChannel->m_IpAddrHeaderSize);
    pBuf+= pIpTxChannel->m_IpAddrHeaderSize;
    
    // Now copy UDP ports
    pUdpHeader = (UdpHeader*)pBuf;

    if (pUdpHeader->m_DestinationPort == pIpTxChannel->m_DstPortMain)
        pUdpHeader->m_DestinationPort = DtUInt16ByteSwap(pIpTxChannel->m_DstPort);
    else if (pUdpHeader->m_DestinationPort == pIpTxChannel->m_DstPortMainRow)
        pUdpHeader->m_DestinationPort = DtUInt16ByteSwap(pIpTxChannel->m_DstPort + 
                                                                        FEC_INC_ROW_PORT);
    else if (pUdpHeader->m_DestinationPort == pIpTxChannel->m_DstPortMainColumn)
        pUdpHeader->m_DestinationPort = DtUInt16ByteSwap(pIpTxChannel->m_DstPort + 
                                                                     FEC_INC_COLUMN_PORT);
    else
        pUdpHeader->m_DestinationPort = 0;
    // m_SrcPort is already in BIG-endian format
    pUdpHeader->m_SourcePort = pIpTxChannel->m_SrcPort;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxRtGetLocalAddressType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpTxRtGetLocalAddress(
    void*  pContext,
    UInt8**  pLocalAddress,
    UInt*  pStart,
    UInt*  pStop,
    UInt*  pTransferSize)
{
    DtaIpPort*  pIpPort = (DtaIpPort*)pContext;

    *pStart = 0;
    *pStop = 0;
    *pTransferSize = 0;
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        *pLocalAddress = (UInt8*)(size_t)pIpPort->m_IpPortType1.m_TxRt.m_FifoOffset;
    else
        *pLocalAddress = (UInt8*)(size_t)pIpPort->m_IpPortType2.m_TxRt.m_FifoOffset;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxWriteNDisPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write one IP packet to a NDIS port, without any assumptions about the contents.
// Note: This function is used for testing DTA2160/160 only
// It should should be preceeded by disabling the networkdriver or by putting the admin 
// status to down.
// Expected is that the DMA header is already added.
DtStatus  DtaIpTxWriteNDisPacket(
    DtaIpPort*  pIpPort,    
    UInt8*  pBuf, 
    UInt  BufLen)
{    
    UInt8*  pDst;    
    UInt  FreeSpaceUntilEnd;
    UInt  FreeSpaceFromBegin;
    DtaDmaTxHeader* pDmaHeader = NULL;
        
    DtaIpNrtChannels*  pNrtChannels = &pIpPort->m_NrtChannels;    
    DtaIpNwSharedBufInfo*  pSharedInfo = pNrtChannels->m_TxSharedBuf.m_pSharedInfo;
    UInt8* pNrtBuf = pNrtChannels->m_TxSharedBuf.m_pBuffer;
    UInt ReadOffset = 0;
    UInt WriteOffset = 0;
    
    // Tranmit one packet at a time: wait for the transmit thread to finish previous 
    // transfers.
    while(pSharedInfo->m_ReadOffset != pSharedInfo->m_WriteOffset);

    ReadOffset = pSharedInfo->m_ReadOffset;
    WriteOffset = pSharedInfo->m_WriteOffset;

    if (pIpPort->m_PortType == DTA_IPPORT_TYPE2)
        return DT_STATUS_NOT_SUPPORTED;

    if (BufLen > DTA_IPTX_MAX_PACKET_LENGTH)
        return DT_STATUS_INVALID_PARAMETER;

    // Calculate free space
    if (WriteOffset < ReadOffset)
    {
        FreeSpaceFromBegin = 0;
        FreeSpaceUntilEnd = ReadOffset - WriteOffset - 1;
    } else {
        FreeSpaceUntilEnd = pNrtChannels->m_TxSharedBuf.m_BufSize - WriteOffset - 1;
        if (ReadOffset == 0)
            FreeSpaceFromBegin = 0;
        else
            FreeSpaceFromBegin = ReadOffset - 1;
    }
    
    if (FreeSpaceUntilEnd >= BufLen)
    {
        pDst = pNrtBuf + WriteOffset;
        WriteOffset += BufLen;
    } else if (FreeSpaceFromBegin >= BufLen) 
    {
        // Mark dummy bytes until end.        
        pNrtBuf[pSharedInfo->m_WriteOffset] = '*';
        pDst = pNrtBuf;
        WriteOffset = BufLen;       
    } else {
        // Not enough room, skip pakket
        DtDbgOut(ERR, IP_TX, "Transmit buffer full. Packet discarded");
        return DT_STATUS_BUFFER_OVERFLOW;
    }

    DtMemCopy(pDst, pBuf, BufLen);
        
    // Align data on DWORD's
    if (BufLen%4 != 0)
        WriteOffset += 4 - (BufLen % 4);

    if (WriteOffset >= pNrtChannels->m_TxSharedBuf.m_BufSize)
    {
        DtDbgOut(ERR, IP_TX, "Write offset exceeds buffersize, setting offset to 0");
        WriteOffset = 0;
    }
    
    pDmaHeader = (DtaDmaTxHeader *)pBuf; 
    pSharedInfo->m_WriteOffset = WriteOffset;

    DtAtomicIncrement(&pSharedInfo->m_DataAvailableCnt);
    DtEventSet(&pSharedInfo->m_DataAvailableEvent);    
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxNrtCleanupBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpTxNrtCleanupBuffer(
    DmaChannel*  pDmaChannel, 
    DtaIpNrtChannels*  pNrtChannels)
{
    DtStatus  Status = DT_STATUS_OK;

    Status = DtaDmaCleanupKernelBuffer(pDmaChannel, pNrtChannels->m_TxSharedBuf.m_pBuffer,
                                                                          NULL, IPTX_TAG);
    pNrtChannels->m_TxSharedBuf.m_pBuffer = NULL;
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxRtCreateBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Create Nrt DMA buffer for DMA transfers. This buffer is also used for the shared
// buffer between network driver and this driver
// Note: The SGL buffer in common memory is already created in DtaDmaInitCh()
//
DtStatus  DtaIpTxNrtCreateBuffer(
    DmaChannel*  pDmaChannel,
    DtaIpNrtChannels*  pNrtChannels)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  BufSize;
    
    DT_ASSERT(DTA_IPTX_NRT_DMA_BUF_SIZE == DTA_IPTX_NRT_SHARED_BUF_SIZE);

    // NRT buffer contains:
    // Shared buffer data + Shared buffer info + Setup frame
    BufSize = DTA_IPTX_NRT_DMA_BUF_SIZE + sizeof(DtaMacCSetupFrame);
    
    // Create buffer
    Status = DtaDmaCreateKernelBuffer(pDmaChannel, DT_DMA_DIRECTION_TO_DEVICE,
                                          (void **)&pNrtChannels->m_TxSharedBuf.m_pBuffer,
                                          NULL, BufSize, IPTX_TAG);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Initialize buffer
    pNrtChannels->m_TxSharedBuf.m_BufSize = DTA_IPTX_NRT_SHARED_BUF_SIZE - 
                                                           sizeof(DtaIpNwSharedBufInfo);
    pNrtChannels->m_TxSharedBuf.m_pSharedInfo = (DtaIpNwSharedBufInfo*)
                                                  (pNrtChannels->m_TxSharedBuf.m_pBuffer +
                                                  pNrtChannels->m_TxSharedBuf.m_BufSize);

    // Clear memory
    DtMemZero(pNrtChannels->m_TxSharedBuf.m_pSharedInfo, sizeof(DtaIpNwSharedBufInfo));
    
    // Init event
    DtEventInit(&pNrtChannels->m_TxSharedBuf.m_pSharedInfo->m_DataAvailableEvent, TRUE);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CheckPackets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// This function can be used to check if the the DMA packet headers in a buffer
// have the correct tags and length.
Bool CheckPackets(UInt8* pBuffer, UInt Size, UInt Offset)
{
    UInt8*  pPacket = &pBuffer[Offset];
    UInt  PacketLength;
    DtaDmaTxHeader*  pDmaTxHeader = NULL;	
    UInt  Index=0;

    do
    {
        pDmaTxHeader = (DtaDmaTxHeader *)pPacket;
        PacketLength = sizeof(DtaDmaTxHeader) + 
                             pDmaTxHeader->m_TxHeaderGen.m_TransmitControl.m_PacketLength;

        if(pDmaTxHeader->m_Tag!=0x445441A0 ||
           pDmaTxHeader->m_Length!=sizeof(DtaDmaTxHeader))
        {
            DtDbgOut(ERR, IP_TX,
                      "Found invalidpacket at pointer: %p, buffer=%p, size=%d, offset=%d",
                      pPacket, pBuffer, Size, Offset);
            return FALSE;
        }
        // Align data on DWORD's
        if (PacketLength%4 != 0)
            PacketLength += 4 - (PacketLength % 4);
        pPacket+=PacketLength;
        Index+=PacketLength;
    } while(Index < Size);

    return TRUE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxNrtWorkerThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpTxNrtWorkerThread(DtThread* pThread, void* pContext)
{
    DtaIpPort*  pIpPort = pContext;
    DtaIpNrtSharedBuf*  pTxSharedBuf = &pIpPort->m_NrtChannels.m_TxSharedBuf;
    DtaIpNwSharedBufInfo*  pSharedInfo = pTxSharedBuf->m_pSharedInfo;
    Bool  StopThread = FALSE;
    DtStatus  Status = DT_STATUS_OK;
    UInt8*  pPacket;
    UInt  PacketLength;
    DtaDmaTxHeader*  pDmaTxHeader = NULL;
    UInt  ReadOffsetTmp;
    DmaChannel*  pDmaCh;
    UInt8*  pLocalAddress;
    UInt  DataToTransfer = 0;
    UInt  ReadOffset;
    
    DtDbgOut(AVG, IP_TX, "Start");

    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
    {
        pDmaCh = &pIpPort->m_IpPortType1.m_TxNrt.m_DmaChannel;
        pLocalAddress = (UInt8*)(size_t)pIpPort->m_IpPortType1.m_TxNrt.m_FifoOffset;
    } else {
        pDmaCh = &pIpPort->m_IpPortType2.m_TxNrt.m_DmaChannel;
        pLocalAddress = (UInt8*)(size_t)pIpPort->m_IpPortType2.m_TxNrt.m_FifoOffset;
    }

    while (!StopThread)
    {
        // Wait for data availble from network driver or a stop event
        Status = DtThreadWaitForStopOrEvent(pThread, &pSharedInfo->m_DataAvailableEvent);
        if (Status == DT_STATUS_CANCELLED)
        {
            StopThread = TRUE;
            continue;
        }

        ReadOffset = pSharedInfo->m_ReadOffset;
        // Get packet data
        while ((ReadOffset!=pSharedInfo->m_WriteOffset ||
                   pIpPort->m_PhyMac.m_SetupframePending) && !DtThreadShouldStop(pThread))
        {
            if (pIpPort->m_PhyMac.m_SetupframePending)
            {
                UInt8*  pDst = pTxSharedBuf->m_pBuffer + DTA_IPTX_NRT_SHARED_BUF_SIZE;
                UInt  Size;
                DT_ASSERT(pIpPort->m_PhyMac.m_MacType == MAC_TYPE_CAST);

                pIpPort->m_PhyMac.m_SetupframePending = FALSE;

                // Build setup frame
                Status = DtaMacCBuildSetupFrame(&pIpPort->m_PhyMac, pDst, &Size);
                if (DT_SUCCESS(Status))
                {
                    // Start DMA stransfer
                    // CheckPackets(pTxSharedBuf->m_pBuffer, Size,
                    //                                      DTA_IPTX_NRT_SHARED_BUF_SIZE);
                    Status = DtaDmaStartKernelBufTransfer(pDmaCh, pTxSharedBuf->m_pBuffer,
                                                       Size, DTA_IPTX_NRT_SHARED_BUF_SIZE,
                                                       pLocalAddress, 0, 0);

                }
                if (!DT_SUCCESS(Status))
                    DtDbgOut(ERR, IP_TX, "Error sending setup frame. Status:%xh", Status);
                else
                    DtDbgOut(MAX, IP_TX, "Sending setup frame");
                DtAtomicDecrement(&pSharedInfo->m_DataAvailableCnt);
                continue;
            }

            pPacket = pTxSharedBuf->m_pBuffer + ReadOffset;
            if (pPacket[0] == '*')
            {
                if (DataToTransfer != 0)
                {
                    // Start DMA transfer
                    // CheckPackets(pTxSharedBuf->m_pBuffer, DataToTransfer, 
                    //                                         pSharedInfo->m_ReadOffset);
                    Status = DtaDmaStartKernelBufTransfer(pDmaCh, pTxSharedBuf->m_pBuffer,
                                   DataToTransfer, pSharedInfo->m_ReadOffset,
                                   pLocalAddress, 0, 0);
                    DataToTransfer = 0;

                }
                // New packet is at start of buffer
                pPacket = pTxSharedBuf->m_pBuffer;
                pSharedInfo->m_ReadOffset = 0;
                ReadOffset = 0;
            }

            pDmaTxHeader = (DtaDmaTxHeader*)pPacket;
            DT_ASSERT(pDmaTxHeader->m_Tag==0x445441A0 &&  
                                          pDmaTxHeader->m_Length==sizeof(DtaDmaTxHeader));
            PacketLength = pDmaTxHeader->m_TxHeaderGen.m_TransmitControl.m_PacketLength +
                                                                   sizeof(DtaDmaTxHeader);

            // Start DMA stransfer
            // Status = DtaDmaStartKernelBufTransfer(pDmaCh, pTxSharedBuf->m_pBuffer,
            //                       PacketLength, pSharedInfo->m_ReadOffset,
            //                       pLocalAddress, 0, 0);
            //DT_ASSERT(DT_SUCCESS(Status) || Status==DT_STATUS_CANCELLED);

            if ((PacketLength % 4) != 0)
                PacketLength += (4 - (PacketLength % 4));
            DataToTransfer+= PacketLength;

            ReadOffsetTmp = ReadOffset + PacketLength;
            if (ReadOffsetTmp>=pTxSharedBuf->m_BufSize || DataToTransfer>1000000)
            {
                if (DataToTransfer != 0)
                {
                    // Start DMA transfer
                    // CheckPackets(pTxSharedBuf->m_pBuffer, DataToTransfer, 
                    //                                         pSharedInfo->m_ReadOffset);
                    Status = DtaDmaStartKernelBufTransfer(pDmaCh, pTxSharedBuf->m_pBuffer,
                                   DataToTransfer, pSharedInfo->m_ReadOffset,
                                   pLocalAddress, 0, 0);
                    DataToTransfer = 0;
                    pSharedInfo->m_ReadOffset = ReadOffsetTmp;
                }
                if (ReadOffsetTmp>=pTxSharedBuf->m_BufSize)
                {
                    ReadOffsetTmp = 0;
                    pSharedInfo->m_ReadOffset = 0;
                }
            }

            ReadOffset = ReadOffsetTmp;
            DtAtomicDecrement(&pSharedInfo->m_DataAvailableCnt);
        }
        if (DataToTransfer != 0)
        {
            // Start DMA transfer
            // CheckPackets(pTxSharedBuf->m_pBuffer, DataToTransfer, 
            //                                                 pSharedInfo->m_ReadOffset);
            Status = DtaDmaStartKernelBufTransfer(pDmaCh, pTxSharedBuf->m_pBuffer,
                            DataToTransfer, pSharedInfo->m_ReadOffset,
                            pLocalAddress, 0, 0);
            DataToTransfer = 0;
            pSharedInfo->m_ReadOffset = ReadOffset;
        } else {
            DT_ASSERT(pSharedInfo->m_ReadOffset == ReadOffset);
        }
        
        // To prevent an endless while, descrease counter if nothing needs to be done.
        // This can occure if 2 setup frames must be send
        if (pSharedInfo->m_DataAvailableCnt>0 && 
                                  pSharedInfo->m_ReadOffset==pSharedInfo->m_WriteOffset &&
                                  !pIpPort->m_PhyMac.m_SetupframePending)
            DtAtomicDecrement(&pSharedInfo->m_DataAvailableCnt);
        
        if (DtThreadShouldStop(pThread))
            StopThread = TRUE;
    }
    DtDbgOut(AVG, IP_TX, "Exit");
    // We don't have to wait until the thread received a stop command. We only got here in
    // that case
    
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChAddToList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Add (insert) a channel in the TX channels list
//
// Pre: Mutex and spinlock to protect list must be aquired.
//
void  DtaIpTxUserChAddToList(
    DtaIpUserChannels*  pIpUserChannels, 
    UserIpTxChannel*  pIpTxNew)
{   
    UserIpTxChannel*  pIpTxChannel = NULL;
    UserIpTxChannel*  pLastIpTx = NULL;
    Int  ChannelIndex = 0;

    DT_ASSERT(pIpTxNew != NULL);

    // Just to be sure.
    pIpTxNew->m_pNext = NULL;
    pIpTxNew->m_pPrev = NULL;
        
    if (pIpUserChannels->m_pIpTxChannel == NULL)
    {   
        // No elements in list, add to first one
        pIpUserChannels->m_pIpTxChannel = pIpTxNew;
        return;
    }
    
    pIpTxChannel = pIpUserChannels->m_pIpTxChannel;
    pLastIpTx = pIpUserChannels->m_pIpTxChannel;
        
    // Insert the new channel into our list. The list is sorted.
    while (pIpTxChannel != NULL)
    {        
        // If an empty position in the list is found, insert the new channel
        if (pIpTxChannel->m_ChannelIndex != ChannelIndex)
        {
            // update the new channel prev/next pointers
            pIpTxNew->m_pPrev = pIpTxChannel->m_pPrev;
            pIpTxNew->m_pNext = pIpTxChannel;
            
            // Check if IpTx was the first in the list (m_pPrev == NULL)
            if (pIpTxChannel->m_pPrev != NULL)
                pIpTxChannel->m_pPrev->m_pNext = pIpTxNew;
            else
                pIpUserChannels->m_pIpTxChannel = pIpTxNew;

            // pIpTxChannel comes after the new channel
            pIpTxChannel->m_pPrev = pIpTxNew; 
            return;
        }         
        ChannelIndex++;
        pLastIpTx = pIpTxChannel;
        pIpTxChannel = pIpTxChannel->m_pNext;
    }
    // The channel must be inserted at the end
    DT_ASSERT(pLastIpTx->m_pNext == NULL);
    pLastIpTx->m_pNext = pIpTxNew;
    pIpTxNew->m_pPrev = pLastIpTx;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChCreate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Create and returns an empty channel object. Only its identifiers are set.
//
UserIpTxChannel*  DtaIpTxUserChCreate(
    Int  ChannelIndex,
    Int  PortIndex,
    Int  IpPortIndex,
    DtFileObject*  pFile)
{
    // create a channel
    UserIpTxChannel*  pIpTxChannel = DtMemAllocPool(DtPoolNonPaged, 
                                                       sizeof(UserIpTxChannel), IPTX_TAG);

    if (pIpTxChannel == NULL)
        return NULL;

    DtMemZero(pIpTxChannel, sizeof(UserIpTxChannel));
    pIpTxChannel->m_pNext = NULL;
    pIpTxChannel->m_pPrev = NULL;
    pIpTxChannel->m_ChannelIndex = ChannelIndex;
    pIpTxChannel->m_PortIndex = PortIndex;
    pIpTxChannel->m_IpPortIndex = IpPortIndex;
    
    // identifies channels to clean for channels that where never detached 
    pIpTxChannel->m_FileObject = *pFile;

    // Init shared buffer    
    pIpTxChannel->m_SharedBuffer.m_pBuffer = NULL;
    pIpTxChannel->m_SharedBuffer.m_pDmaCh = NULL;
    pIpTxChannel->m_SharedBuffer.m_Initialised = FALSE;
    pIpTxChannel->m_SharedBuffer.m_Purpose = DTA_SH_BUF_PURPOSE_GEN;

    // Init packet buffer
    pIpTxChannel->m_pTxBufferHead = NULL;
    pIpTxChannel->m_pBuffer = NULL;

    // Report an overflow or ignore and try to process packets in the next interrupt
    pIpTxChannel->m_ReportOverFlow = FALSE;

    // Initialize concurrency objects
    DtSpinLockInit(&pIpTxChannel->m_FlagsSpinLock);

    return pIpTxChannel;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChInitRWPointers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set the pointers, read from shared memory, write to shared memory
// for a channel on its initial value.
//
void  DtaIpTxUserChInitRWPointers(UserIpTxChannel* pIpTxChannel)
{ 
    // Set read/write pointer to start
    if(pIpTxChannel->m_SharedBuffer.m_Initialised && 
                                                    pIpTxChannel->m_pTxBufferHead != NULL)
    {
        pIpTxChannel->m_pTxBufferHead->m_ReadOffset = 0;
        pIpTxChannel->m_pTxBufferHead->m_WriteOffset= 0;
    } else
        DT_ASSERT(FALSE);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChRemoveFromList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Remove a channel from the list, and link the previous and next item in the list
//
void  DtaIpTxUserChRemoveFromList(
    DtaIpUserChannels*  pIpUserChannels, 
    UserIpTxChannel*  pIpTxChannel)
{
    if (pIpTxChannel->m_pPrev == NULL) 
    {
        // This was the first element in the list.
        pIpUserChannels->m_pIpTxChannel = pIpTxChannel->m_pNext;
        if (pIpTxChannel->m_pNext)
            pIpTxChannel->m_pNext->m_pPrev = NULL;
        return;
    }
    
    pIpTxChannel->m_pPrev->m_pNext = pIpTxChannel->m_pNext;
    if (pIpTxChannel->m_pNext)
        pIpTxChannel->m_pNext->m_pPrev = pIpTxChannel->m_pPrev;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChSetTxControlIdle -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set control to idle.
// remark: To avoid acces from DPC, we have to acquire a spinlock first.
//         After setting to idle, the shared buffer (DTAPI) pointers are initialised.
//
void  DtaIpTxUserChSetTxControlIdle(
    DtaIpUserChannels*  pIpUserChannels,
    UserIpTxChannel*  pIpTxChannel)
{
    if (pIpTxChannel->m_TxControl == DT_TXCTRL_IDLE)
        return;

    DtSpinLockAcquire(&pIpUserChannels->m_IpTxChannelSpinLock);
    pIpTxChannel->m_TxControl = DT_TXCTRL_IDLE;
    DtSpinLockRelease(&pIpUserChannels->m_IpTxChannelSpinLock);

    DtaIpTxUserChInitRWPointers(pIpTxChannel);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChUpdateReferenceTimeStamps -.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpTxUserChUpdateReferenceTimeStamps(
    DtaDmaTxHeader*  pDmaTxHeader, 
    UInt64*  pStartTimestamp, 
    UInt64*  pRefTimestamp,
    UInt32*  pLastTimestampOffs,
    DtaDeviceData*  pDeviceData,
    Bool  FifoUnderflow)
{
    UInt64  ReferenceDelay;
    UInt32  PeriodicIntMs = DtaGetPerIntItvUS(pDeviceData)/1000;
    
    // The timestamp clock has wrapped, increment reference
    if (pDmaTxHeader->m_Timestamp < *pLastTimestampOffs) 
    {
        *pLastTimestampOffs = pDmaTxHeader->m_Timestamp;
        *pRefTimestamp += (UInt64)1<<32;
    }
    if (pStartTimestamp != NULL)
    {
        if (FifoUnderflow)
        {
            // Fifo was empty, but not anymore.
            UInt64 PrevRefTimestamp;

            // Calculate new reference timestamp for this channel
            PrevRefTimestamp = *pRefTimestamp;

            // Set initial reference timestamp with a delay off:
            // 5 * periodic interrupt cycles (this normally will have a delay of 100 ms)
            ReferenceDelay = 5 * (pDeviceData->m_DevInfo.m_RefClk/1000) * PeriodicIntMs;
            *pRefTimestamp = DtaRegRefClkCntGet64(pDeviceData->m_pGenRegs) + 
                                               ReferenceDelay - pDmaTxHeader->m_Timestamp;
            *pLastTimestampOffs = pDmaTxHeader->m_Timestamp;

            // Calculate new StartTimestamp for fifoload calculations in DTAPI
            *pStartTimestamp += *pRefTimestamp - PrevRefTimestamp;
        }
    }
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpTxUserChFindNewPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Check all channels if there are new packets available. Return the channel with the 
// newest packet.
// Note: For efficiency sake this function also handles underflows and Timestamp 
// reference updates.
// 
UserIpTxChannel*  DtaIpTxUserChFindNewPacket(
    DtStatus* pPortStatus,
    DtaIpUserChannels*  pIpUserChannels,
    UInt64  MaxPacketTime)
{
    UserIpTxChannel*  pIpTxChannel = NULL;
    UserIpTxChannel*  pIpTxNewestChannel = NULL;
    DtaDmaTxHeader*  pDmaTxHeader = NULL;
    volatile UInt8*  pSourceBuffer = NULL;
    UInt64  PacketTime;
    UInt64  NewestPacketTime;
    DtaDeviceData*  pDeviceData = pIpUserChannels->m_pDvcData;
    Int  CurrentPort;
    
#ifdef LINBUILD
    NewestPacketTime = 0xffffffffffffffffLLU;
#else
    NewestPacketTime = 0xffffffffffffffff;
#endif

    // Find newest packet and put in the buffer as first packet.
    // Note: This packet will be send as last packet...
    pIpTxChannel = pIpUserChannels->m_pIpTxChannel;
    while (pIpTxChannel != NULL)
    {
        if (pIpTxChannel->m_TxControl != DT_TXCTRL_SEND)
        {
           pIpTxChannel = pIpTxChannel->m_pNext;
           continue; // nothing to send, goto the next channel
        }

        CurrentPort = pIpTxChannel->m_IpPortIndex;
        // Skip processing for this channel if the port has no buffer left 
        if (pPortStatus[CurrentPort] == DT_STATUS_OUT_OF_RESOURCES)
        {
            pIpTxChannel = pIpTxChannel->m_pNext;
            continue;
        }

        // Skip processing for this channel if the port has an overflow
        // and it is NOT reported. 
        // If reported, packets are processed, but not transmitted.
        if (pPortStatus[CurrentPort] == DT_STATUS_BUFFER_OVERFLOW && 
                                                          !pIpTxChannel->m_ReportOverFlow)
        {
            pIpTxChannel = pIpTxChannel->m_pNext;
            continue;
        }

        // Check if packets are available for this channel
        // and handle underflow situation
        if (pIpTxChannel->m_pTxBufferHead->m_ReadOffset == 
                                             pIpTxChannel->m_pTxBufferHead->m_WriteOffset)
        {
           // Report underflow, if not yet reported
           DtSpinLockAcquireAtDpc(&pIpTxChannel->m_FlagsSpinLock);
           if ((pIpTxChannel->m_Flags & DTA_TX_FIFO_UFL) == 0)
           {
                DtDbgOut(ERR, IP_TX, "Detected UNDERFLOW ReadOffset %u, WriteOffset: %u",
                                            pIpTxChannel->m_pTxBufferHead->m_ReadOffset,
                                            pIpTxChannel->m_pTxBufferHead->m_WriteOffset);

                pIpTxChannel->m_Flags |= DTA_TX_FIFO_UFL;
                pIpTxChannel->m_LatchedFlags |= DTA_TX_FIFO_UFL;
           }
           DtSpinLockReleaseFromDpc(&pIpTxChannel->m_FlagsSpinLock);
           pIpTxChannel = pIpTxChannel->m_pNext;
           continue; // Nothing to send, goto the next channel
        }

        // We have a new packet in this channel.
        // read packet
        pSourceBuffer = pIpTxChannel->m_pBuffer;
        pDmaTxHeader = (DtaDmaTxHeader*)(pSourceBuffer + 
                                             pIpTxChannel->m_pTxBufferHead->m_ReadOffset);

        // Update timestamp references for this channel
        DtaIpTxUserChUpdateReferenceTimeStamps(pDmaTxHeader, (UInt64*)
                                         &pIpTxChannel->m_pTxBufferHead->m_StartTimestamp,
                                         &pIpTxChannel->m_RefTimestamp, 
                                         &pIpTxChannel->m_LastTimestampOffs, pDeviceData,
                                         (pIpTxChannel->m_Flags&DTA_TX_FIFO_UFL) != 0);

        // check if this packet is newer then the previous newest
        PacketTime = pIpTxChannel->m_RefTimestamp + pDmaTxHeader->m_Timestamp;
        if (PacketTime<=MaxPacketTime && PacketTime<NewestPacketTime)
        {
            NewestPacketTime = PacketTime;
            pIpTxNewestChannel = pIpTxChannel;
        }

        // reset underflow flag
        // No need to acquire spinlock, the DPC is the only one writing this flag
        pIpTxChannel->m_Flags &= ~DTA_TX_FIFO_UFL;
        pIpTxChannel = pIpTxChannel->m_pNext;
    }

    return pIpTxNewestChannel;
}
