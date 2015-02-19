//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Ip.c *#*#*#*#*#*#*#*#*#*# (C) 2010-2015 DekTec
//
// Dta driver - IP functionality - Implementation of generic IP port functionality
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2015 DekTec Digital Video B.V.
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

// Defines
// IP Tag
#define  IP_TAG         0x20207049  // '  pI'

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaIpGetMacAddress(DtaIpPort* pIpPort, UInt8* pAddress);
DtStatus  DtaIpGetSpeed(DtaIpPort* pIpPort, Int* pSpeed, Int* pEnableForceSpeed);
DtStatus  DtaIpSetSpeed(DtaIpPort* pIpPort, Int Speed, Int EnableForceSpeed);
DtStatus  DtaIpGetAdminStatus(DtaIpPort* pIpPort, Int* pAdminStatusUp);
DtStatus  DtaIpSetAdminStatus(DtaIpPort* pIpPort, Int AdminStatusUp);
DtStatus  DtaIpEnableLoopback(DtaIpPort* pIpPort, Int EnableLoopback);
DtStatus  DtaIpDevicePowerdownPre(DtaIpDevice* pIpDevice);
void  DtaIpDumpAddrMatcherTable(DtaIpPort* pIpPort, AddressMatcherLookupEntry* pHead);
void  DtaIpCleanupIpRxListeners(DtaIpPort* pIpPort);
DtStatus  DtaIpInitIpRxListeners(DtaIpPort* pIpPort);

#define  DTANW_MIN_MAJOR    3
#define  DTANW_MIN_MINOR    2
#define  DTANW_MIN_MICRO    0

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Globals -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  g_NwDrvVersionMajor = -1;              // Version number of network driver
Int  g_NwDrvVersionMinor = -1;              // Version number of network driver
Int  g_NwDrvVersionMicro = -1;              // Version number of network driver
Int  g_NwDrvVersionBuild = -1;              // Version number of network driver


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCheckDtaNwDriverVersion -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpCheckDtaNwDriverVersion(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool  VersionError = FALSE;
    DtString  DtStrVersion;
    DtStringChar  DtStrVersionBuffer[32];
    DT_STRING_DECL(DtStrDot, ".");

    if (g_NwDrvVersionMajor==-1 && g_NwDrvVersionMinor==-1 && g_NwDrvVersionMicro==-1)
        return DT_STATUS_OK;
    
    // Check driver version
    if (!VersionError && g_NwDrvVersionMajor<DTANW_MIN_MAJOR) VersionError = TRUE;
    if (!VersionError && g_NwDrvVersionMajor>DTANW_MIN_MAJOR) return DT_STATUS_OK;
    if (!VersionError && g_NwDrvVersionMinor<DTANW_MIN_MINOR) VersionError = TRUE;
    if (!VersionError && g_NwDrvVersionMinor>DTANW_MIN_MINOR) return DT_STATUS_OK;
    if (!VersionError && g_NwDrvVersionMicro>=DTANW_MIN_MICRO) return DT_STATUS_OK;

    DT_ASSERT(VersionError);
        
    // Report error message
    Status = DT_STATUS_VERSION_MISMATCH;

    // Connect DtStrErrMsgBuffer to DtStrErrMsg
    DT_STRING_INIT(DtStrVersion, DtStrVersionBuffer, 32);

    // Build version
    DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTANW_MIN_MAJOR);
    DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTANW_MIN_MINOR);
    DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTANW_MIN_MICRO);
    DtEvtLogReport(&pIpPort->m_pDvcData->m_Device.m_EvtObject, DTA_LOG_NWVERSION_MISMATCH,
                                                               &DtStrVersion, NULL, NULL);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpInitType1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpInitType1(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIpPortType1*  pIpPortType1 = &pIpPort->m_IpPortType1;
    
    // Initialise list of DMA channels to handle DMA processing type independent
    // For now, only 1 channel enabled
    pIpPort->m_NumDmaChannelsInit = 3;
    pIpPort->m_pDmaChannelsInit[0] = &pIpPortType1->m_TxRt.m_DmaChannel;
    pIpPort->m_pDmaChannelsInit[1] = &pIpPortType1->m_TxNrt.m_DmaChannel;
    pIpPort->m_pDmaChannelsInit[2] = &pIpPortType1->m_Rx.m_DmaChannel;

    pIpPort->m_NumDmaChannelsIsr = 3;
    pIpPort->m_pDmaChannelsIsr[0] = &pIpPortType1->m_TxRt.m_DmaChannel;
    pIpPort->m_pDmaChannelsIsr[1] = &pIpPortType1->m_TxNrt.m_DmaChannel;
    pIpPort->m_pDmaChannelsIsr[2] = &pIpPortType1->m_Rx.m_DmaChannel;

    

    Status = DtaIpRxInitType1(pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising IpRx Type1");
        return Status;
    }

    Status = DtaIpTxInitType1(pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising IpTx Type1");
        return Status;
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpInitType2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpInitType2(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIpPortType2*  pIpPortType2 = &pIpPort->m_IpPortType2;
    DtPropertyData*  pPropData = &pIpPort->m_pDvcData->m_PropData;
    
    // Initialise list of DMA channels to handle DMA processing type independent
    if (pIpPort->m_IpPortIndex%2 == 0)
    {
        pIpPort->m_NumDmaChannelsInit = 4;
        pIpPort->m_pDmaChannelsInit[0] = &pIpPortType2->m_TxNrt.m_DmaChannel;
        pIpPort->m_pDmaChannelsInit[1] = &pIpPortType2->m_RxNrt.m_DmaChannel;
        pIpPort->m_pDmaChannelsInit[2] = &pIpPortType2->m_RxRt.m_DmaChannel;
        pIpPort->m_pDmaChannelsInit[3] = &pIpPortType2->m_TxRt.m_DmaChannel;

        pIpPort->m_NumDmaChannelsIsr = 3;
        pIpPort->m_pDmaChannelsIsr[0] = &pIpPortType2->m_TxNrt.m_DmaChannel;
        pIpPort->m_pDmaChannelsIsr[1] = &pIpPortType2->m_RxNrt.m_DmaChannel;
        pIpPort->m_pDmaChannelsIsr[2] = &pIpPortType2->m_RxRt.m_DmaChannel;
    } else {
        pIpPort->m_NumDmaChannelsInit = 2;
        pIpPort->m_pDmaChannelsInit[0] = &pIpPortType2->m_TxNrt.m_DmaChannel;
        pIpPort->m_pDmaChannelsInit[1] = &pIpPortType2->m_RxNrt.m_DmaChannel;

        pIpPort->m_NumDmaChannelsIsr = 2;
        pIpPort->m_pDmaChannelsIsr[0] = &pIpPortType2->m_TxNrt.m_DmaChannel;
        pIpPort->m_pDmaChannelsIsr[1] = &pIpPortType2->m_RxNrt.m_DmaChannel;
    }

    pIpPortType2->m_InterruptsEnabled = TRUE;

    Status = DtaIpRxInitType2(pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising IpRx Type2");
        return Status;
    }
    Status = DtaIpTxInitType2(pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising IpTx Type2");
        return Status;
    }

    // Initialise register offsets
    pIpPortType2->m_pAddrMatcherRegs = NULL;
    pIpPortType2->m_pAddrMatcherLookupRegs = NULL;
    pIpPortType2->m_pSorterRegs = NULL;

    pIpPortType2->m_AddrMatcherRegsOffset = DtPropertiesGetUInt16(pPropData, 
                                         "NW_REGISTERS_ADDR_MATCH", pIpPort->m_PortIndex);
    pIpPortType2->m_AddrMatcherLookupRegsOffset = DtPropertiesGetUInt16(pPropData, 
                                      "NW_REGISTERS_ADDR_MATCH_LU", pIpPort->m_PortIndex);
    pIpPortType2->m_SorterRegsOffset = DtPropertiesGetUInt16(pPropData, 
                                             "NW_REGISTERS_SORTER", pIpPort->m_PortIndex);
    if (pIpPort->m_IpPortIndex%2 == 0)
    {
        DtMutexInit(&pIpPortType2->m_MacAddrFiltMutex);
        DtMutexInit(&pIpPortType2->m_AddrMatcherMutex);
        pIpPortType2->m_AddrMatchHead.m_pNext = NULL;
        pIpPortType2->m_AddrMatchHead.m_pPrev = NULL;
        pIpPortType2->m_AddrMatchHead.m_pPart2 = NULL;
    }
    
    // Report configuration errors
    // Check if no property error occurred
    Status = DtaPropertiesReportDriverErrors(pIpPort->m_pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCleanupType1Pre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpCleanupType1Pre(DtaIpPort* pIpPort)
{
    // Note: Driver verifier reports an error if we do finalize later (in DtaCleanup)
    
    DtaDmaFinalTransfer(&pIpPort->m_IpPortType1.m_TxRt.m_DmaChannel);
    DtaDmaFinalTransfer(&pIpPort->m_IpPortType1.m_TxNrt.m_DmaChannel);
    DtaDmaFinalTransfer(&pIpPort->m_IpPortType1.m_Rx.m_DmaChannel);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCleanupType2Pre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpCleanupType2Pre(DtaIpPort* pIpPort)
{
    // Note: Driver verifier reports an error if we do finalize later (in DtaCleanup)
    
    DtaDmaFinalTransfer(&pIpPort->m_IpPortType2.m_RxNrt.m_DmaChannel);
    DtaDmaFinalTransfer(&pIpPort->m_IpPortType2.m_TxNrt.m_DmaChannel);
    if (pIpPort->m_IpPortIndex%2 == 0)
    {
        DtaDmaFinalTransfer(&pIpPort->m_IpPortType2.m_RxRt.m_DmaChannel);
        DtaDmaFinalTransfer(&pIpPort->m_IpPortType2.m_TxRt.m_DmaChannel);
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCleanupType1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpCleanupType1(DtaIpPort* pIpPort)
{    
    DtaIpTxCleanupType1(pIpPort);
    DtaIpRxCleanupType1(pIpPort);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCleanupType2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpCleanupType2(DtaIpPort* pIpPort)
{    
    DtaIpTxCleanupType2(pIpPort);
    DtaIpRxCleanupType2(pIpPort);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCleanupDmaPPBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpCleanupDmaPPBuffer(DmaChannel* pDmaChannel, PPBuffer* pPPBuffer)
{
    DtStatus  Status = DT_STATUS_OK;

    Status = DtaDmaCleanupKernelBuffer(pDmaChannel, pPPBuffer->m_pBufStart,
                                                          pPPBuffer->m_pPageList, IP_TAG);
    pPPBuffer->m_pBufStart = NULL;
    pPPBuffer->m_pPageList = NULL;
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCreateDmaPPBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Create DMA ping-pong buffer for DMA transfers
// This is one large buffer that is divided in 2 parts
// Pre: The SGL buffer in common memory must be already created with DtaDmaInitCh()
//
DtStatus  DtaIpCreateDmaPPBuffer(
    DmaChannel*  pDmaChannel, 
    PPBuffer*  pPPBuffer, 
    DtaIpPort*  pIpPort,
    UInt  PPBufSize,
    DtaPPBufferGetLocAddrFunc  pGetLocAddrFunc,
    UInt  Direction,
    Bool  AutoTransferAfterComplete)
{
    DtStatus  Status = DT_STATUS_OK;
    void*  pBuffer = NULL;
    DtPageList* pPageList = NULL;
    
    // Create buffer
    Status = DtaDmaCreateKernelBuffer(pDmaChannel, Direction, &pBuffer, &pPageList, 
                                                                       PPBufSize, IP_TAG);
    
    // Initialize pingpong buffer
    DtaPPBufferInitialise(pPPBuffer, (UInt8*)pBuffer, pPageList, PPBufSize, pDmaChannel, 
                                     pGetLocAddrFunc, pIpPort, AutoTransferAfterComplete);
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpUserChInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpUserChInit(DtaDeviceData* pDvcData, DtaIpUserChannels* pIpUserChannels)
{
    pIpUserChannels->m_pDvcData = pDvcData;
    pIpUserChannels->m_pIpTxChannel = NULL;
    DtFastMutexInit(&pIpUserChannels->m_IpTxChannelMutex);
    DtSpinLockInit(&pIpUserChannels->m_IpTxChannelSpinLock);

    pIpUserChannels->m_pIpRxChannel = NULL;
    DtFastMutexInit(&pIpUserChannels->m_IpRxChThreadMutex);
    DtFastMutexInit(&pIpUserChannels->m_IpRxChAccesMutex);
    DtEventInit(&pIpUserChannels->m_IpRxRtpPacketAvailEvent, TRUE);
    DtSpinLockInit(&pIpUserChannels->m_IpRxBrmSpinLock);
    pIpUserChannels->m_IpRxBrmSkipUpdate = FALSE;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpLoadAdminStatusConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpDeviceLoadAdminStatusConfig(
    DtaDeviceData*  pDvcData, 
    Bool*  pAdminStatusConfig)
{
    DtStatus  Status = DT_STATUS_OK;

    Int64  Value = 0;
    Status = DtNonVolatileSettingsValueRead(&pDvcData->m_Driver, 
                      pDvcData->m_DevInfo.m_Serial, -1,"NwAdminStatus", "Ignore", &Value);

    *pAdminStatusConfig = FALSE;
    if (Value > 0)
        *pAdminStatusConfig = TRUE;

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpDeviceInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// 
DtStatus  DtaIpDeviceInit(DtaDeviceData*  pDvcData, DtaIpDevice*  pIpDevice)
{
    DtStatus  Status = DT_STATUS_OK;

    // Load admin status (not critical if failed)
    pIpDevice->m_IgnoreAdminStatus = FALSE;
    DtaIpDeviceLoadAdminStatusConfig(pDvcData, &pIpDevice->m_IgnoreAdminStatus);
    if (pIpDevice->m_IgnoreAdminStatus)
        DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, DTA_LOG_SKIP_ADMIN_STATUS, NULL,
                                                                              NULL, NULL);

    Status = DtaIpUserChInit(pDvcData, &pIpDevice->m_IpUserChannels);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising user channel");
        return Status;
    }

    Status = DtaIpTxDeviceInit(pIpDevice);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising IpTx device");
        return Status;
    }

    Status = DtaIpRxDeviceInit(pIpDevice);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising IpRx device");
        return Status;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpInit(
    DtaDeviceData*  pDvcData,
    Int  PortIndex, 
    Int  IpPortIndex, 
    DtaIpDevice*  pIpDevice)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIpPort*  pIpPort = &pIpDevice->m_pIpPorts[IpPortIndex];
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    
    if (DtPropertiesGetBool(pPropData, "CAP_IPPAIR", PortIndex))
    {
        if (pIpDevice->m_PortType == 0)
            pIpDevice->m_PortType = DTA_IPPORT_TYPE2;
        else if (pIpDevice->m_PortType != DTA_IPPORT_TYPE2)
            return DT_STATUS_CONFIG_ERROR;
    } else {
        if (pIpDevice->m_PortType == 0)
            pIpDevice->m_PortType = DTA_IPPORT_TYPE1;
        else if (pIpDevice->m_PortType != DTA_IPPORT_TYPE1)
            return DT_STATUS_CONFIG_ERROR;
    }

    // Capabilities
    pIpPort->m_CapIp = DtPropertiesGetBool(pPropData, "CAP_IP", PortIndex);
    
    // DTA_IOCONFIG_IOCONFIG
    DT_ASSERT(pIpPort->m_CapIp);

    // Initialise general members
    pIpPort->m_PortType = pIpDevice->m_PortType;
    pIpPort->m_PortIndex = PortIndex;
    pIpPort->m_IpPortIndex = IpPortIndex;
    pIpPort->m_pDvcData = pDvcData;
    pIpPort->m_NumRxFifoOverflow = 0;
    pIpPort->m_NumIncorrectDmaRxHeaders = 0;
    
    // Initialize register mappings
    pIpPort->m_pGenNwRegs = NULL;

    // Set offsets to network registers
    pIpPort->m_GenNwRegsOffset = DtPropertiesGetUInt16(pPropData, "NW_REGISTERS_GEN", 
                                                                               PortIndex);

    // Check if no property error occurred
    Status = DtaPropertiesReportDriverErrors(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Init IpRx listeners
    Status = DtaIpInitIpRxListeners(pIpPort);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Init PHYMAC
    Status = DtaPhyMacInit(pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising PhyMac");
        return Status;
    }

    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        Status = DtaIpInitType1(pIpPort);
    else if (pIpPort->m_PortType == DTA_IPPORT_TYPE2)
        Status = DtaIpInitType2(pIpPort);
    else
        DT_ASSERT(FALSE);

    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising Ip type specific");
        return Status;
    }

    DT_ASSERT(pIpPort->m_NumDmaChannelsInit <= DTA_IP_MAX_DMA_CHANNELS);
    DT_ASSERT(pIpPort->m_NumDmaChannelsIsr <= DTA_IP_MAX_DMA_CHANNELS);
    
    // Initialize on device level if the first port is initialized
    if (pIpPort->m_IpPortIndex == 0)
        Status = DtaIpDeviceInit(pDvcData, pIpDevice);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCleanupPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpCleanupPre(DtaDeviceData*  pDvcData,
    Int  PortIndex, 
    Int  IpPortIndex, 
    DtaIpDevice*  pIpDevice)
{
    DtaIpPort*  pIpPort = &pIpDevice->m_pIpPorts[IpPortIndex];
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        DtaIpCleanupType1Pre(pIpPort);
    else
        DtaIpCleanupType2Pre(pIpPort);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//
void  DtaIpCleanup(DtaDeviceData*  pDvcData,
    Int  PortIndex, 
    Int  IpPortIndex, 
    DtaIpDevice*  pIpDevice)
{
    DtaIpPort*  pIpPort = &pIpDevice->m_pIpPorts[IpPortIndex];
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        DtaIpCleanupType1(pIpPort);
    else
        DtaIpCleanupType2(pIpPort);
    
    DtaIpCleanupIpRxListeners(pIpPort);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpPowerdownType1Pre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpPowerdownType1Pre(DtaIpPort* pIpPort)
{    
    DtaDmaAbortDma(&pIpPort->m_IpPortType1.m_TxRt.m_DmaChannel);
    DtaDmaAbortDma(&pIpPort->m_IpPortType1.m_TxNrt.m_DmaChannel);
    DtaDmaAbortDma(&pIpPort->m_IpPortType1.m_Rx.m_DmaChannel);

    while (!DtaDmaIsReady(&pIpPort->m_IpPortType1.m_TxRt.m_DmaChannel) || 
                           !DtaDmaIsReady(&pIpPort->m_IpPortType1.m_TxNrt.m_DmaChannel) ||
                           !DtaDmaIsReady(&pIpPort->m_IpPortType1.m_Rx.m_DmaChannel))
        DtSleep(10);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpPowerdownType2Pre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpPowerdownType2Pre(DtaIpPort* pIpPort)
{    
    DtaDmaAbortDma(&pIpPort->m_IpPortType2.m_RxNrt.m_DmaChannel);
    DtaDmaAbortDma(&pIpPort->m_IpPortType2.m_TxNrt.m_DmaChannel);
    if (pIpPort->m_IpPortIndex%2 == 0)
    {   DtaDmaAbortDma(&pIpPort->m_IpPortType2.m_TxRt.m_DmaChannel);
        DtaDmaAbortDma(&pIpPort->m_IpPortType2.m_RxRt.m_DmaChannel);
    }
    
    if (pIpPort->m_IpPortIndex%2 == 0)
    {
        while (!DtaDmaIsReady(&pIpPort->m_IpPortType2.m_RxNrt.m_DmaChannel) || 
                           !DtaDmaIsReady(&pIpPort->m_IpPortType2.m_TxNrt.m_DmaChannel) ||
                           !DtaDmaIsReady(&pIpPort->m_IpPortType2.m_TxRt.m_DmaChannel) ||
                           !DtaDmaIsReady(&pIpPort->m_IpPortType2.m_RxRt.m_DmaChannel))
        DtSleep(10);
    } else {
        while (!DtaDmaIsReady(&pIpPort->m_IpPortType2.m_RxNrt.m_DmaChannel) || 
                            !DtaDmaIsReady(&pIpPort->m_IpPortType2.m_TxNrt.m_DmaChannel));
            DtSleep(10);
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpInitPowerupType1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpPowerupType1(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIpPortType1*  pIpPortType1 = &pIpPort->m_IpPortType1;
    Int  i;

    pIpPortType1->m_Rx.m_pRegs = pIpPort->m_pDvcData->m_pGenRegs + 
                                                          pIpPortType1->m_Rx.m_RegsOffset;

    pIpPortType1->m_TxRt.m_pRegs = pIpPort->m_pDvcData->m_pGenRegs +  
                                                        pIpPortType1->m_TxRt.m_RegsOffset;

    pIpPortType1->m_TxNrt.m_pRegs = pIpPort->m_pDvcData->m_pGenRegs + 
                                                       pIpPortType1->m_TxNrt.m_RegsOffset;

    for (i = 0; i <pIpPort->m_NumDmaChannelsInit; i++)
    {
        Status = DtaDmaInitChPowerup(pIpPort->m_pDmaChannelsInit[i]);
        if (!DT_SUCCESS(Status))
            break;
    }
    
    
    // Enable Rt(DVB)transmit
    DtaNwTxCtrlTxControlSet(pIpPort->m_IpPortType1.m_TxRt.m_pRegs, 1);
    DtaNwTxCtrlTxOnTimeSet(pIpPort->m_IpPortType1.m_TxRt.m_pRegs, 1);
    
    // Enable Rt/Nrt receive
    DtaNwRxCtrlRxControlSet(pIpPort->m_IpPortType1.m_Rx.m_pRegs, 1);

    // Enable Nrt transmit
    DtaNwTxCtrlTxControlSet(pIpPort->m_IpPortType1.m_TxNrt.m_pRegs, 1);
    DtaNwTxCtrlTxOnTimeSet(pIpPort->m_IpPortType1.m_TxNrt.m_pRegs, 0);

    // Enable deletion of incorrect packets by FPGA
    DtaNwRxCtrlSetDelPckFpga(pIpPort->m_IpPortType1.m_Rx.m_pRegs, 1);

    // Enable Rx Fifo Overflow interrupt enable
    //DtaNwRxCtrlSetRxFifoOvfIntEn(pFdoExt->m_IpMqRx[i]->m_pRegBase, 1);


    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpInitPowerupType2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpPowerupType2(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIpPortType2*  pIpPortType2 = &pIpPort->m_IpPortType2;
    Int  i;

    pIpPortType2->m_RxNrt.m_pRegs = pIpPort->m_pDvcData->m_pGenRegs +
                                                       pIpPortType2->m_RxNrt.m_RegsOffset;

    pIpPortType2->m_TxNrt.m_pRegs = pIpPort->m_pDvcData->m_pGenRegs +
                                                       pIpPortType2->m_TxNrt.m_RegsOffset;
    pIpPortType2->m_RxRt.m_pRegs = pIpPort->m_pDvcData->m_pGenRegs +
                                                        pIpPortType2->m_RxRt.m_RegsOffset;
    pIpPortType2->m_TxRt.m_pRegs = pIpPort->m_pDvcData->m_pGenRegs +
                                                        pIpPortType2->m_TxRt.m_RegsOffset;
    pIpPortType2->m_pAddrMatcherRegs = pIpPort->m_pDvcData->m_pGenRegs +
                                                    pIpPortType2->m_AddrMatcherRegsOffset;
    pIpPortType2->m_pAddrMatcherLookupRegs = pIpPort->m_pDvcData->m_pGenRegs +
                                              pIpPortType2->m_AddrMatcherLookupRegsOffset;
    pIpPortType2->m_pSorterRegs = pIpPort->m_pDvcData->m_pGenRegs +
                                                         pIpPortType2->m_SorterRegsOffset;
    pIpPortType2->m_pMemoryControllerRegs = pIpPort->m_pDvcData->m_pGenRegs+
                                                   pIpPortType2->m_MemoryControllerOffset;

    for (i = 0; i <pIpPort->m_NumDmaChannelsInit; i++)
    {
        Status = DtaDmaInitChPowerup(pIpPort->m_pDmaChannelsInit[i]);
        if (!DT_SUCCESS(Status))
            break;
    }
    
    // Setup and enable Nrt Tx
    DtaNwTxCtrlClrFifoSet(pIpPort->m_IpPortType2.m_TxNrt.m_pRegs);
    DtaNwTxCtrlTxOnTimeSet(pIpPort->m_IpPortType2.m_TxNrt.m_pRegs, 0);
    DtaNwTxCtrlTxControlSet(pIpPort->m_IpPortType2.m_TxNrt.m_pRegs, 1);

    // Setup and enable Nrt Rx
    DtaNwRxCtrlSetClrFifo(pIpPort->m_IpPortType2.m_RxNrt.m_pRegs);
    DtaDmaTimeOutSet(pIpPort->m_IpPortType2.m_RxNrt.m_DmaChannel.m_pRegBase, 1);
    DtaNwRxCtrlRxControlSet(pIpPort->m_IpPortType2.m_RxNrt.m_pRegs, 1);
    DtaNwRxCtrlSetDelPckFpga(pIpPort->m_IpPortType2.m_RxNrt.m_pRegs, 1);

    // Set timeslice to 19,4ms == 20
    DtaNwCtrlSetTimeslice(pIpPort->m_pGenNwRegs, 20);
    
    // Enable Rx Fifo Overflow interrupt enable
    //DtaNwRxCtrlSetRxFifoOvfIntEn(pFdoExt->m_IpMqRx[i]->m_pRegBase, 1);
    
    // Disable address matcher promiscious mode
    DtaAddrMatchPromDis(pIpPort->m_IpPortType2.m_pAddrMatcherRegs ,1);

    // Enable MAC address filter so NRT packets are filtered
    DtaMacAddrFilterEnable(pIpPort->m_IpPortType2.m_pAddrMatcherRegs, 1);

    // Enable Rt(DVB) transmit
    // (Interrupt enable is done in DtaIpEnableInterrupts)
    DtaNwTxCtrlClrFifoSet(pIpPort->m_IpPortType2.m_TxRt.m_pRegs);
    DtaNwTxCtrlSetDmaAbortEn(pIpPort->m_IpPortType2.m_TxRt.m_pRegs, 1);
    DtaNwTxCtrlTxOnTimeSet(pIpPort->m_IpPortType2.m_TxRt.m_pRegs, 1);
    DtaNwTxCtrlTxControlSet(pIpPort->m_IpPortType2.m_TxRt.m_pRegs, 1);
    if (pIpPort->m_IpPortIndex%2 == 0)
    {
        // Enable sorter
        DtaFrameSrtCtrlEn(pIpPort->m_IpPortType2.m_pSorterRegs, 1);
    }
    
    // Enable Rt(DVB) receive
    // (Interrupt enable is done in DtaIpEnableInterrupts)
    DtaNwRxCtrlSetClrFifo(pIpPort->m_IpPortType2.m_RxRt.m_pRegs);
    DtaNwRxCtrlRxControlSet(pIpPort->m_IpPortType2.m_RxRt.m_pRegs, 1);
    DtaNwRxCtrlSetDelPckFpga(pIpPort->m_IpPortType2.m_RxRt.m_pRegs, 1);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpDisableInterrupts -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpDisableInterrupts(DtaIpPort* pIpPort)
{
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        return;
    // Disable Rt(DVB) transmit int
    DtaNwTxCtrlSetDmaReadyIntEn(pIpPort->m_IpPortType2.m_TxRt.m_pRegs, 0);
    // Disable Rt(RVB) receive int
    DtaNwRxCtrlSetSliceIntEn(pIpPort->m_IpPortType2.m_RxRt.m_pRegs, 0);

    // We have to clear the RT fifo to reset the time slicer in firmware 
    // in case we restart later on.
    DtaNwTxCtrlClrFifoSet(pIpPort->m_IpPortType2.m_TxRt.m_pRegs);

    // Make sure interrupts are not handled anymore
    pIpPort->m_IpPortType2.m_InterruptsEnabled = FALSE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpEnableInterrups -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpEnableInterrupts(DtaIpPort* pIpPort)
{
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        return;
    
    // Enable handling interrupts
    pIpPort->m_IpPortType2.m_InterruptsEnabled = TRUE;

    // Enable Rt(DVB) transmit int
    DtaNwTxCtrlSetDmaReadyIntEn(pIpPort->m_IpPortType2.m_TxRt.m_pRegs, 1);
    // Enable Rt(RVB) receive int
    DtaNwRxCtrlSetSliceIntEn(pIpPort->m_IpPortType2.m_RxRt.m_pRegs, 1);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called everytime the hardware is powered up. Resources like registers
// can be remapped, so we need to recalculate the register pointers.
//
DtStatus  DtaIpPowerup(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    
    // Recalculate registers
    pIpPort->m_pGenNwRegs = pIpPort->m_pDvcData->m_pGenRegs + pIpPort->m_GenNwRegsOffset;

    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        Status = DtaIpPowerupType1(pIpPort);
    else if (pIpPort->m_PortType == DTA_IPPORT_TYPE2)
        Status = DtaIpPowerupType2(pIpPort);
    else
        Status = DT_STATUS_NOT_SUPPORTED;
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising Ip powerup type specific");
        return Status;
    }

    Status = DtaPhyMacPowerup(&pIpPort->m_PhyMac);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising PhyMac powerup");
        return Status;
    }

    Status = DtaIpRxPowerup(pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising IpRx powerup");
        return Status;
    }

    Status = DtaIpTxPowerup(pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising IpTx powerup");
        return Status;
    }

    if (pIpPort->m_IpPortIndex == 0)
    {
        Status = DtaIpRxDevicePowerup(&pIpPort->m_pDvcData->m_IpDevice);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, IP, "Error initialising IpRx device powerup");
            return Status;
        }
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpIoctl(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  IpPortIndex;           // Index in the ip port struct
    DtaIpPort*  pIpPort;
    
    DtaIoctlIpCmdInput* pIpCmdInput = (DtaIoctlIpCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlIpCmdOutput* pIpCmdOutput =(DtaIoctlIpCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlIpCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlIpCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
     if (!DT_SUCCESS(DtaGetIpPortIndex(pDvcData, pIpCmdInput->m_PortIndex, &IpPortIndex)))
        return DT_STATUS_INVALID_PARAMETER;

     pIpPort = &pDvcData->m_IpDevice.m_pIpPorts[IpPortIndex];

    // Determine final required output/input sizes
    switch (pIpCmdInput->m_Cmd)
    {
    case DTA_IP_CMD_GETMACADDRESS:
        pCmdStr = "DTA_IP_CMD_GETMACADDRESS";
        InReqSize = 0;
        OutReqSize += sizeof(DtaIoctlIpCmdGetMacAddressOutput);
        break;
    case DTA_IP_CMD_GETSPEED:
        pCmdStr = "DTA_IP_CMD_GETSPEED";
        InReqSize = 0;
        OutReqSize += sizeof(DtaIoctlIpCmdGetSpeedOutput);
        break;
    case DTA_IP_CMD_SETSPEED:
        pCmdStr = "DTA_IP_CMD_SETSPEED";
        InReqSize += sizeof(DtaIoctlIpCmdSetSpeedInput);
        OutReqSize = 0;
        break;
    case DTA_IP_CMD_GETADMINSTATUS:
        pCmdStr = "DTA_IP_CMD_GETADMINSTATUS";
        InReqSize = 0;
        OutReqSize += sizeof(DtaIoctlIpCmdGetAdminStatusOutput);
        break;
    case DTA_IP_CMD_SETADMINSTATUS:
        pCmdStr = "DTA_IP_CMD_SETADMINSTATUS";
        InReqSize += sizeof(DtaIoctlIpCmdSetAdminStatusInput);
        OutReqSize = 0;
        break;
    case DTA_IP_CMD_GETMACCOUNTER:
        pCmdStr = "DTA_IP_CMD_GETMACCOUNTER";
        InReqSize += sizeof(DtaIoctlIpCmdGetMacCounterInput);
        OutReqSize += sizeof(DtaIoctlIpCmdGetMacCounterOutput);
        break;
    case DTA_IP_CMD_GETPHYREGISTER:
        pCmdStr = "DTA_IP_CMD_GETPHYREGISTER";
        InReqSize += sizeof(DtaIoctlIpCmdGetPhyRegisterInput);
        OutReqSize += sizeof(DtaIoctlIpCmdGetPhyRegisterOutput);
        break;
    case DTA_IP_CMD_SETPHYREGISTER:
        pCmdStr = "DTA_IP_CMD_SETPHYREGISTER";
        InReqSize += sizeof(DtaIoctlIpCmdSetPhyRegisterInput);
        OutReqSize = 0;
        break;
    case DTA_IP_CMD_ENABLELOOPBACK:
        pCmdStr = "DTA_IP_CMD_ENABLELOOPBACK";
        InReqSize += sizeof(DtaIoctlIpCmdSetAdminStatusInput);
        OutReqSize = 0;
        break;

    default:
        pCmdStr = "??UNKNOWN IP_CMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, IP, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, IP, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, IP, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pIpCmdInput->m_Cmd)
        {
        case DTA_IP_CMD_GETMACADDRESS:
            Status = DtaIpGetMacAddress(pIpPort,
                                             pIpCmdOutput->m_Data.m_MacAddress.m_Address);
            break;

        case DTA_IP_CMD_GETSPEED:
            Status = DtaIpGetSpeed(pIpPort, &pIpCmdOutput->m_Data.m_GetSpeed.m_Speed,
                                     &pIpCmdOutput->m_Data.m_GetSpeed.m_EnableForceSpeed);
            break;
        case DTA_IP_CMD_SETSPEED:
            Status = DtaIpSetSpeed(pIpPort, pIpCmdInput->m_Data.m_SetSpeed.m_Speed,
                                       pIpCmdInput->m_Data.m_SetSpeed.m_EnableForceSpeed);
            break;
        case DTA_IP_CMD_GETADMINSTATUS:
            Status = DtaIpGetAdminStatus(pIpPort,
                                  &pIpCmdOutput->m_Data.m_GetAdminStatus.m_AdminStatusUp);
            break;
        case DTA_IP_CMD_SETADMINSTATUS:
            if (!pDvcData->m_IpDevice.m_IgnoreAdminStatus)
                Status = DtaIpSetAdminStatus(pIpPort,
                                    pIpCmdInput->m_Data.m_SetAdminStatus.m_AdminStatusUp);
            break;
        case DTA_IP_CMD_GETMACCOUNTER:
            Status = DtaMacGetCounter(&pIpPort->m_PhyMac, 
                                          pIpCmdInput->m_Data.m_GetMacCounter.m_CounterId,
                                          &pIpCmdOutput->m_Data.m_GetMacCounter.m_Value);
            break;
        case DTA_IP_CMD_GETPHYREGISTER:
            pIpCmdOutput->m_Data.m_GetPhyRegister.m_Value =
                                           DtaGetPhyRegister(&pIpPort->m_PhyMac, 
                                           pIpCmdInput->m_Data.m_GetPhyRegister.m_Offset);
            break;
        case DTA_IP_CMD_SETPHYREGISTER:
            DtaSetPhyRegister(&pIpPort->m_PhyMac, 
                                            pIpCmdInput->m_Data.m_SetPhyRegister.m_Offset,
                                            pIpCmdInput->m_Data.m_SetPhyRegister.m_Value);

        case DTA_IP_CMD_ENABLELOOPBACK:
                Status = DtaIpEnableLoopback(pIpPort,
                                           pIpCmdInput->m_Data.m_EnableLoopback.m_Enable);
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
            DtDbgOut(MIN, IP, "IpCmd=0x%x: NOT SUPPORTED", pIpCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, IP, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpNwIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpNwIoctl(
    DtaDeviceData*  pDvcData,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    DtaIpPort*  pIpPort;
    
    DtaIoctlIpNwCmdInput* pIpNwCmdInput = (DtaIoctlIpNwCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlIpNwCmdOutput* pIpNwCmdOutput =
                                          (DtaIoctlIpNwCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlIpNwCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlIpNwCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    if (pIpNwCmdInput->m_IpPortIndex >= pDvcData->m_NumIpPorts)
        return DT_STATUS_INVALID_PARAMETER;
    pIpPort = &pDvcData->m_IpDevice.m_pIpPorts[pIpNwCmdInput->m_IpPortIndex];

    // Determine final required output/input sizes
    switch (pIpNwCmdInput->m_Cmd)
    {
    case DTA_IP_NW_CMD_SET_DRIVER_VERSION:
        pCmdStr = "DTA_IP_NW_CMD_SET_DRIVER_VERSION";
        InReqSize += sizeof(DtaIoctlIpNwSetDriverVersionInput);
        OutReqSize = 0;
        break;
    case DTA_IP_NW_CMD_GET_SHARED_BUFFERS:
        pCmdStr = "DTA_IP_NW_CMD_GET_SHARED_BUFFERS";
        InReqSize = 0;
        OutReqSize += sizeof(DtaIoctlIpNwGetSharedBuffersOutput);
        break;
    
    default:
        pCmdStr = "??UNKNOWN IP_CMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, IP, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, IP, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, IP, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pIpNwCmdInput->m_Cmd)
        {
        case DTA_IP_NW_CMD_SET_DRIVER_VERSION:
            g_NwDrvVersionMajor = pIpNwCmdInput->m_Data.m_SetDriverVersion.m_Major;
            g_NwDrvVersionMinor = pIpNwCmdInput->m_Data.m_SetDriverVersion.m_Minor;
            g_NwDrvVersionMicro = pIpNwCmdInput->m_Data.m_SetDriverVersion.m_Micro;
            g_NwDrvVersionBuild = pIpNwCmdInput->m_Data.m_SetDriverVersion.m_Build;
            Status = DtaIpCheckDtaNwDriverVersion(pIpPort);
            break;

        case DTA_IP_NW_CMD_GET_SHARED_BUFFERS:
            pIpNwCmdOutput->m_Data.m_GetSharedBuffers.m_AlignedPayload = 
                                                       pIpPort->m_PhyMac.m_AlignedPayload;
            pIpNwCmdOutput->m_Data.m_GetSharedBuffers.m_pRxBuffer = 
                                           pIpPort->m_NrtChannels.m_RxSharedBuf.m_pBuffer;
            pIpNwCmdOutput->m_Data.m_GetSharedBuffers.m_pTxBuffer =
                                           pIpPort->m_NrtChannels.m_TxSharedBuf.m_pBuffer;
            pIpNwCmdOutput->m_Data.m_GetSharedBuffers.m_RxBufSize = 
                                          pIpPort->m_NrtChannels.m_RxSharedBuf.m_BufSize +
                                          sizeof(DtaIpNwSharedBufInfo);
            pIpNwCmdOutput->m_Data.m_GetSharedBuffers.m_TxBufSize = 
                                          pIpPort->m_NrtChannels.m_TxSharedBuf.m_BufSize +
                                          sizeof(DtaIpNwSharedBufInfo);


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
            DtDbgOut(MIN, IP, "IpNwCmd=0x%x: NOT SUPPORTED", pIpNwCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, IP, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpIoConfigSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpIoConfigSet(
    DtaIpPort*  pIpPort,
    Int  Code,
    DtaIoConfigValue  CfgValue)
{
    switch (Code)
    {
    case DT_IOCONFIG_IOSTD:
        // IP is the only supported IOSTD
        if (CfgValue.m_Value != DT_IOCONFIG_IP)
            return DT_STATUS_CONFIG_ERROR;
        // No further action required
        break;

    case DT_IOCONFIG_TSRATESEL:
        // INTERNAL RATE is the only supported TSRATESEL
        if (CfgValue.m_Value != DT_IOCONFIG_INTTSRATE)
            return DT_STATUS_CONFIG_ERROR;
        // No further action required
        break;

    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpIoConfigGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpIoConfigGet(
    DtaIpPort* pIpPort, 
    Int Code,
    DtaIoConfigValue* pCfgValue)
{
    Int i;
    
    // Init to 'safe' values
    pCfgValue->m_Value = DT_IOCONFIG_NONE;
    pCfgValue->m_SubValue = DT_IOCONFIG_NONE;
    for (i=0; i<4; i++)
        pCfgValue->m_ParXtra[i] = -1;

    switch (Code)
    {
    case DT_IOCONFIG_IOSTD:
        // Always return DT_IOCONFIG_IP to the caller
        pCfgValue->m_Value = DT_IOCONFIG_IP;
        pCfgValue->m_SubValue = DT_IOCONFIG_NONE;
        break;

    case DT_IOCONFIG_TSRATESEL:
        // Always return DT_IOCONFIG_INTTSRATE to the caller
        pCfgValue->m_Value = DT_IOCONFIG_INTTSRATE;
        pCfgValue->m_SubValue = DT_IOCONFIG_NONE;
        break;

    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.- DtaIpUserChReleaseResourceFromFileObject -.-.-.-.-.-.-.-.-.-.-.-
//
// purge channels
//
DtStatus  DtaIpUserChReleaseResourceFromFileObject(
    DtaIpUserChannels*  pIpUserChannels,
    DtFileObject*  pFile)
{
    UserIpTxChannel*  pUserIpTxChannel = pIpUserChannels->m_pIpTxChannel;
    UserIpTxChannel*  pUserIpTxToDelete  = NULL;
    UserIpRxChannel*  pUserIpRxChannel = pIpUserChannels->m_pIpRxChannel;
    UserIpRxChannel*  pUserIpRxToDelete  = NULL;
    
    // Cleanup shared TX IP channel buffers    
    while(pUserIpTxChannel != NULL)
    {
        // Rescue the ipTx pointer to the next item BEFORE we delete the current pointer
        pUserIpTxToDelete = pUserIpTxChannel;        
        pUserIpTxChannel = pUserIpTxChannel->m_pNext;

        if (DtFileCompare(&pUserIpTxToDelete->m_SharedBuffer.m_Owner, pFile))
        {                
            DtDbgOut(AVG, IP, 
                     "Cleanup shared buffer resource for TX IP channel %i using Port %i",
                     pUserIpTxToDelete->m_ChannelIndex, pUserIpTxToDelete->m_IpPortIndex);
            DtaIpTxUserChDestroy(pIpUserChannels, pUserIpTxToDelete);
        }
    }
    
    // Cleanup shared RX IP channel buffers
    while(pUserIpRxChannel != NULL)
    {
        // Rescue the ipRx pointer to the next item BEFORE we delete the current pointer
        pUserIpRxToDelete = pUserIpRxChannel;
        pUserIpRxChannel = pUserIpRxChannel->m_pNext;

        if (DtFileCompare(&pUserIpRxToDelete->m_SharedBuffer.m_Owner, pFile))
        {                
            DtDbgOut(AVG, IP, 
                     "Cleanup shared buffer resource for RX IP channel %i using Port %i",
                     pUserIpRxToDelete->m_ChannelIndex, pUserIpRxToDelete->m_IpPortIndex);
            DtaIpRxUserChDestroy(pIpUserChannels, pUserIpRxToDelete);
        }
    }

    return DT_STATUS_OK; 
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpReleaseResourceFromFileObject -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpReleaseResourceFromFileObject(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile)
{
    return DtaIpUserChReleaseResourceFromFileObject(
                                                   &pDvcData->m_IpDevice.m_IpUserChannels,
                                                   pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCheckReleaseAdminStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpCheckReleaseAdminStatus(DtaDeviceData* pDvcData)
{
    DtaIpUserChannels*  pUserIpChannel = &pDvcData->m_IpDevice.m_IpUserChannels;
    Int  i;

    // Only check for cards with IP ports
    if (pDvcData->m_NumIpPorts == 0)
        return DT_STATUS_OK;

    // Gain access to the channel list
    DtFastMutexAcquire(&pUserIpChannel->m_IpRxChAccesMutex);
    DtFastMutexAcquire(&pUserIpChannel->m_IpTxChannelMutex);
    
    if (pUserIpChannel->m_pIpTxChannel==NULL && pUserIpChannel->m_pIpRxChannel==NULL)
    {
        // No more Ip channels available
        // Set AdminStatus to up
        for (i=0; i<pDvcData->m_NumIpPorts; i++)
            DtaIpSetAdminStatus(&pDvcData->m_IpDevice.m_pIpPorts[i], 1);
    }
    DtFastMutexRelease(&pUserIpChannel->m_IpTxChannelMutex);
    DtFastMutexRelease(&pUserIpChannel->m_IpRxChAccesMutex);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpGetMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpGetMacAddress(DtaIpPort* pIpPort, UInt8* pAddress)
{
    DtaMacGetMacAddressCurrent(&pIpPort->m_PhyMac, pAddress);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpPowerdownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called everytime the hardware is power downed just before the
// interrupts are disabled.
//
DtStatus  DtaIpPowerdownPre(DtaIpPort* pIpPort)
{    
    DtStatus  Status = DT_STATUS_OK;
    Int  LastIpPort = pIpPort->m_pDvcData->m_NumIpPorts - 1;

    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        Status = DtaIpPowerdownType1Pre(pIpPort);
    else
        Status = DtaIpPowerdownType2Pre(pIpPort);

    Status = DtaIpRxPowerdownPre(pIpPort);
    Status = DtaIpTxPowerdownPre(pIpPort);

     
    // Only pre power down on device level if the last port is powered down
    if (pIpPort->m_IpPortIndex == LastIpPort)
        Status = DtaIpDevicePowerdownPre(&pIpPort->m_pDvcData->m_IpDevice);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpDevicePowerdownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpDevicePowerdownPre(DtaIpDevice* pIpDevice)
{
    DtStatus  Status = DT_STATUS_OK;
    Status = DtaIpRxDevicePowerdownPre(pIpDevice);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpDevicePowerdown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpDevicePowerdown(DtaIpDevice* pIpDevice)
{
   DtaIpTxDevicePowerdown(pIpDevice);
   DtaIpRxDevicePowerdown(pIpDevice);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpPowerdown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called everytime the hardware is power downed for each port.
//
void  DtaIpPowerdown(DtaIpDevice* pIpDevice, DtaIpPort* pIpPort)
{
    Int  LastIpPort = pIpPort->m_pDvcData->m_NumIpPorts - 1;

    DtaIpTxPowerDown(pIpPort);

    // Only power down on device level if the last port is powered down
    if (pIpPort->m_IpPortIndex == LastIpPort)
        DtaIpDevicePowerdown(pIpDevice);

    DtaPhyMacPowerdown(&pIpPort->m_PhyMac);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpGetAdminStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpGetAdminStatus(DtaIpPort* pIpPort, Int* pAdminStatusUp)
{
    PhyMac* pPhyMac = &pIpPort->m_PhyMac;
    *pAdminStatusUp = pPhyMac->m_NetworkAdminStatus == DTA_NW_ADMIN_STATUS_UP ? 1 : 0;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpSetAdminStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpSetAdminStatus(DtaIpPort* pIpPort, Int AdminStatusUp)
{    
    PhyMac* pPhyMac = &pIpPort->m_PhyMac;

    UInt8  NewAdminStatus = (AdminStatusUp == 1 ? DTA_NW_ADMIN_STATUS_UP : 
                                                                DTA_NW_ADMIN_STATUS_DOWN);

    if (pPhyMac->m_NetworkAdminStatus != NewAdminStatus)
    {
        // Report error in event viewer.
        if (NewAdminStatus == DTA_NW_ADMIN_STATUS_UP)
            DtEvtLogReport(&pIpPort->m_pDvcData->m_Device.m_EvtObject, 
                                               DTA_LOG_ADMIN_STATUS_UP, NULL, NULL, NULL);
        else
            DtEvtLogReport(&pIpPort->m_pDvcData->m_Device.m_EvtObject, 
                                             DTA_LOG_ADMIN_STATUS_DOWN, NULL, NULL, NULL);
    }

    pPhyMac->m_NetworkAdminStatus = NewAdminStatus;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpEnableLoopback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpEnableLoopback(DtaIpPort* pIpPort, Int EnableLoopback)
{
    PhyMac*  pPhyMac = &pIpPort->m_PhyMac;
    if (pPhyMac->m_LoopbackEnable != EnableLoopback)
    {
        pPhyMac->m_LoopbackEnable = EnableLoopback == 1 ? TRUE: FALSE;
        DtaPhySetSpeedDuplex(pPhyMac);
    }    
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpGetSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpGetSpeed(
    DtaIpPort*  pIpPort,
    Int*  pSpeed, 
    Int*  pEnableForceSpeed)
{    
    Bool  EnableForceSpeed;
    PhyMac*  pPhyMac = &pIpPort->m_PhyMac;
    *pSpeed = DtaPhyGetSpeed(pPhyMac, &EnableForceSpeed);
    *pEnableForceSpeed = EnableForceSpeed ? 1 : 0;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpGetSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpSetSpeed(DtaIpPort* pIpPort, Int Speed, Int EnableForceSpeed)
{    
    PhyMac*  pPhyMac = &pIpPort->m_PhyMac;
    DtaPhySetSpeedDtapi(pPhyMac, Speed, EnableForceSpeed != 0 ? TRUE : FALSE);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpGetSharedBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtaShBuffer*  DtaIpGetSharedBuffer(
    DtaIpDevice* pIpDevice, 
    Int ChannelIndex, 
    Int ChannelType)
{    
    UserIpRxChannel* pIpRxChannel = NULL;
    UserIpTxChannel* pIpTxChannel = NULL;

    if (ChannelType==DTA_SH_CHANTYPE_IPRX || ChannelType==DTA_SH_CHANTYPE_IPRX_V2)
    {      
      pIpRxChannel = DtaIpRxUserChGet(&pIpDevice->m_IpUserChannels, ChannelIndex);
      if (pIpRxChannel != NULL)
          return &pIpRxChannel->m_SharedBuffer;
    }

    if (ChannelType == DTA_SH_CHANTYPE_IPTX)
    {
      pIpTxChannel = DtaIpTxUserChGet(&pIpDevice->m_IpUserChannels, ChannelIndex);
      if (pIpTxChannel != NULL)
          return &pIpTxChannel->m_SharedBuffer;
    }
    return NULL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpSharedBufferClosing -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpSharedBufferClosing(DtaIpDevice* pIpDevice, Int ChannelIndex, 
                                                                          Int ChannelType)
{   
    UserIpRxChannel* pIpRxChannel = NULL;
    UserIpTxChannel* pIpTxChannel = NULL;

    if (ChannelType == DTA_SH_CHANTYPE_IPTX)
    {
        pIpTxChannel = DtaIpTxUserChGet(&pIpDevice->m_IpUserChannels, ChannelIndex);
        if (pIpTxChannel != NULL)
        {          
            pIpTxChannel->m_pTxBufferHead = NULL;
            return DT_STATUS_OK;
        }
    } else if (ChannelType == DTA_SH_CHANTYPE_IPRX)
    { 
        pIpRxChannel = DtaIpRxUserChGet(&pIpDevice->m_IpUserChannels, ChannelIndex);
        if (pIpRxChannel != NULL)
        {
            pIpRxChannel->m_pBufferHeader = NULL;
            return DT_STATUS_OK;
        }       
    } else if (ChannelType == DTA_SH_CHANTYPE_IPRX_V2)
    { 
        pIpRxChannel = DtaIpRxUserChGet(&pIpDevice->m_IpUserChannels, ChannelIndex);
        if (pIpRxChannel != NULL)
        {        
            pIpRxChannel->m_pBufferHeader = NULL;
            return DT_STATUS_OK;
        }
    }
    return DT_STATUS_NOT_FOUND;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpSharedBufferReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Notify IP channels that the shared buffer is created for this channel
//
DtStatus  DtaIpSharedBufferReady(
    DtaIpDevice* pIpDevice, 
    Int ChannelIndex, 
    Int ChannelType)
{   
    UserIpRxChannel* pIpRxChannel = NULL;
    UserIpTxChannel* pIpTxChannel = NULL;

    if (ChannelType == DTA_SH_CHANTYPE_IPTX)
    {
        // Buffer contents:
        // |======|======================|
        // |Header|FIFO                  
        //                               
        pIpTxChannel = DtaIpTxUserChGet(&pIpDevice->m_IpUserChannels, ChannelIndex);
        if (pIpTxChannel != NULL)
        {          
            // The shared buffer from DTAPI is filled with a header and the
            // actual packetbuffer.
            // Set the internal pointers to the correct position
            pIpTxChannel->m_pTxBufferHead = 
                               (IpTxBufferHeader *)pIpTxChannel->m_SharedBuffer.m_pBuffer;
            pIpTxChannel->m_pBuffer = pIpTxChannel->m_SharedBuffer.m_pBuffer + 
                                                                 sizeof(IpTxBufferHeader);
            return DT_STATUS_OK;
        }
    } else if (ChannelType == DTA_SH_CHANTYPE_IPRX)
    { 
        // Buffer contents:
        // |======|======================|=========|====================|
        // |Header|FIFO                  |WrapArea | RTP/FEC scratchpad
        //                               |Used by driver only
        // The wrap area is only used for easy data copiing to TS buffer when free TS buf
        // size is less then the TS packet size
        pIpRxChannel = DtaIpRxUserChGet(&pIpDevice->m_IpUserChannels, ChannelIndex);
        if (pIpRxChannel != NULL)
        {   Int  RtpBufSize;
            pIpRxChannel->m_pBufferHeader = (IpRxBufferHeader*)
                                                   pIpRxChannel->m_SharedBuffer.m_pBuffer;
            pIpRxChannel->m_BufSize = pIpRxChannel->m_pBufferHeader->m_BufSize;
            pIpRxChannel->m_pFifo = pIpRxChannel->m_SharedBuffer.m_pBuffer +
                                                                 sizeof(IpRxBufferHeader);
            pIpRxChannel->m_FifoSize = pIpRxChannel->m_pBufferHeader->m_BufSize - 
                                            DTA_IPRX_BUFWRAPSIZE - DTA_IPRX_BUFRTPSIZE_TS;
            // We create the RTP list also in the IP-buffer
            pIpRxChannel->m_pRtpListEntries = pIpRxChannel->m_pFifo + 
                                          pIpRxChannel->m_FifoSize + DTA_IPRX_BUFWRAPSIZE;
            RtpBufSize =  DTA_IPRX_MAX_PACKET_LENGTH * DTA_IPRX_MAX_RTP_PACKETS_TS;
            // Initialize RTP List entries
            pIpRxChannel->m_pRtpBuffer = DtaIpRxRtpListsInit(pIpRxChannel, RtpBufSize);
            // The jumbo packets are stored in the RTP buffers.
            pIpRxChannel->m_pJumboPktBuffer = pIpRxChannel->m_pRtpBuffer;
            pIpRxChannel->m_MaxJumboPktSize = 0x10000; // 64kb
            pIpRxChannel->m_MaxPacketOutOfSync = 0;
            pIpRxChannel->m_MinPacketDelay = 0;
            return DT_STATUS_OK;
        }       
    } else if (ChannelType == DTA_SH_CHANTYPE_IPRX_V2)
    { 
        // Buffer contents:
        // |======|======================|=========|====================|
        // |Header|FIFO                  |WrapArea | RTP/FEC scratchpad
        //                               |Used by driver only
        // The wrap area is only used for easy data copying to TS buffer when free TS buf
        // size is less then the TS packet size
        pIpRxChannel = DtaIpRxUserChGet(&pIpDevice->m_IpUserChannels, ChannelIndex);
        if (pIpRxChannel != NULL)
        {        
            pIpRxChannel->m_pBufferHeader = (IpRxBufferHeader*)
                                                   pIpRxChannel->m_SharedBuffer.m_pBuffer;
            pIpRxChannel->m_BufSize = pIpRxChannel->m_pBufferHeader->m_BufSize;
            pIpRxChannel->m_pFifo = pIpRxChannel->m_SharedBuffer.m_pBuffer +
                                                                 sizeof(IpRxBufferHeader);
            pIpRxChannel->m_FifoSize = 0;
            pIpRxChannel->m_pRtpBuffer = NULL;
            pIpRxChannel->m_pRtpListEntries = NULL;
            pIpRxChannel->m_pJumboPktBuffer = NULL;
            pIpRxChannel->m_MaxJumboPktSize = 0;
            pIpRxChannel->m_MaxPacketOutOfSync = 0;
            pIpRxChannel->m_MinPacketDelay = 0;
            return DT_STATUS_OK;
        }
    }
    return DT_STATUS_NOT_FOUND;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpHandleNewPacketFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpHandleNewPacketFilter(PhyMac* pPhyMac)
{
    DtaIpPort*  pIpPort;
    
    pIpPort = &pPhyMac->m_pDvcData->m_IpDevice.m_pIpPorts[pPhyMac->m_IpPortIndex];
    
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
    {

        if ((pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_PROMISCUOUS) != 0)
        {
            // Disable deletion of incorrect packets by FPGA
            DtaNwRxCtrlSetDelPckFpga(pIpPort->m_IpPortType1.m_Rx.m_pRegs, 0);
#ifndef  _DEBUG
            // Enable deletion of packets with packet length error
            // Only enabled in a release driver version
            // so NDIS test works with a release version
            DtaNwRxCtrlSetDelLenPckFpga(pIpPort->m_IpPortType1.m_Rx.m_pRegs);
#endif
        } else {
            // Enable deletion of incorrect packets by FPGA
            DtaNwRxCtrlSetDelPckFpga(pIpPort->m_IpPortType1.m_Rx.m_pRegs, 1);
        }
    } else {
        if ((pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_PROMISCUOUS) != 0)
        {
            // Disable deletion of incorrect packets by FPGA
            DtaNwRxCtrlSetDelPckFpga(pIpPort->m_IpPortType2.m_RxNrt.m_pRegs, 0);
            DtaNwRxCtrlSetDelPckFpga(pIpPort->m_IpPortType2.m_RxRt.m_pRegs, 0);
#ifndef  _DEBUG
            // Enable deletion of packets with packet length error
            // Only enabled in a release driver version
            // so NDIS test works with a release version
            DtaNwRxCtrlSetDelLenPckFpga(pIpPort->m_IpPortType2.m_RxNrt.m_pRegs);
            DtaNwRxCtrlSetDelLenPckFpga(pIpPort->m_IpPortType2.m_RxRt.m_pRegs);
#endif
        } else {
            // Enable deletion of incorrect packets by FPGA
            DtaNwRxCtrlSetDelPckFpga(pIpPort->m_IpPortType2.m_RxNrt.m_pRegs, 1);
            DtaNwRxCtrlSetDelPckFpga(pIpPort->m_IpPortType2.m_RxRt.m_pRegs, 1);
        }
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpGetNumRxFifoOverflow -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt64  DtaIpGetNumRxFifoOverflow(DtaIpPort* pIpPort)
{
    
    if (pIpPort->m_PhyMac.m_MacType == MAC_TYPE_CAST)
        return pIpPort->m_NumRxFifoOverflow + 
                              DtaMacCMissedFramesCounterGet(pIpPort->m_PhyMac.m_pMacRegs);
    return pIpPort->m_NumRxFifoOverflow;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpUpdateMacAddressFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpUpdateMacAddressFilter(DtaIpPort* pIpPort)
{
    UInt  NumEntries = 0;
    UInt  i;
    UInt  FirstIpPortNum =  (pIpPort->m_IpPortIndex & 0xfffffffe);
    DtaIpPort*  pFirstPort = &pIpPort->m_pDvcData->m_IpDevice.m_pIpPorts[FirstIpPortNum];
    volatile UInt8*  pAddrMLUBase = pIpPort->m_IpPortType2.m_pAddrMatcherLookupRegs;
    volatile UInt8*  pAddrMBase = pIpPort->m_IpPortType2.m_pAddrMatcherRegs;
    PhyMac*  pPhyMac = &pIpPort->m_PhyMac;
    UInt8  BroadcastAddress[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    UInt32  LookupAddress;
    
    DT_ASSERT(pIpPort->m_PortType == DTA_IPPORT_TYPE2);
    
    // Protect updating port 0/1
    DtMutexAcquire(&pFirstPort->m_IpPortType2.m_MacAddrFiltMutex, -1);

    // Busy wait until address matcher is ready updating a previous switch
    // Max. wait time about 12us after update if fired for 1Gb
    while (DtaMacAddrFilterStatGetLutUpdatePending(pAddrMBase)!=0)
    {
        DtDbgOut(ERR, PHYMAC, "[%i] MAC address filter pending: %u", 
             pIpPort->m_IpPortIndex, DtaMacAddrFilterStatGetLutUpdatePending(pAddrMBase));
    }


    DtDbgOut(AVG, PHYMAC, "[%i]  MAC Address=%02x%02x%02x%02x%02x%02x", 
                                      pIpPort->m_IpPortIndex,
                                      pPhyMac->m_MacAddrCur[0], pPhyMac->m_MacAddrCur[1],
                                      pPhyMac->m_MacAddrCur[2], pPhyMac->m_MacAddrCur[3],
                                      pPhyMac->m_MacAddrCur[4], pPhyMac->m_MacAddrCur[5]);

    if (pPhyMac->m_NumMulticasts+1 < 32)
    {
        if (pIpPort->m_IpPortIndex%2 == 0)
            NumEntries = DtaMacAddrLookupCtrlGetNumEntryPhy0(pAddrMLUBase);
        else
            NumEntries = DtaMacAddrLookupCtrlGetNumEntryPhy1(pAddrMLUBase);
        DtDbgOut(MAX, PHYMAC, "[%i] NumEntries: %i", pIpPort->m_IpPortIndex, NumEntries);

        // Remove all entries
        // Start at entry 0
        DtaMacAddrLookupAddrSet(pAddrMLUBase, 0);
        for (i=0; i<NumEntries; i++)
        {
            DtaMacAddrLookupData0Set(pAddrMLUBase, 0);
            DtaMacAddrLookupData1Set(pAddrMLUBase, 0);
        }

        // Start at entry 0 of table 0 or 1, phy0/phy1
        LookupAddress = (pIpPort->m_IpPortIndex%2 == 0 ? 0 : 
                                                            DTA_MACADDR_LOOKUP_ADDR_PHY1);
        DtaMacAddrLookupAddrSet(pAddrMLUBase, LookupAddress);
        NumEntries = 0;

        // Our own unicast MAC address will be filtered by the MAC. 
        // This filter only filters multicast/broadcast.
        DtaMacAddrLookupData0Set(pAddrMLUBase, MAC_ADDRESS_LOW(BroadcastAddress));
        DtaMacAddrLookupData1Set(pAddrMLUBase, MAC_ADDRESS_HIGH(BroadcastAddress));
        NumEntries++;

        for (i=0; i<pPhyMac->m_NumMulticasts; i++) 
        {
            DtDbgOut(AVG, PHYMAC, "[%i] ML[%02d]=%02x%02x%02x%02x%02x%02x", 
                          pIpPort->m_IpPortIndex, i,
                          pPhyMac->m_MulticastList[i][0], pPhyMac->m_MulticastList[i][1],
                          pPhyMac->m_MulticastList[i][2], pPhyMac->m_MulticastList[i][3],
                          pPhyMac->m_MulticastList[i][4], pPhyMac->m_MulticastList[i][5]);
            DtaMacAddrLookupData0Set(pAddrMLUBase, 
                                            MAC_ADDRESS_LOW(pPhyMac->m_MulticastList[i]));
            DtaMacAddrLookupData1Set(pAddrMLUBase, 
                                           MAC_ADDRESS_HIGH(pPhyMac->m_MulticastList[i]));
            NumEntries++;
        }

         if (pIpPort->m_IpPortIndex%2 == 0)
            DtaMacAddrLookupCtrlSetNumEntryPhy0(pAddrMLUBase, NumEntries);
        else
            DtaMacAddrLookupCtrlSetNumEntryPhy1(pAddrMLUBase, NumEntries);

        // Update table for MAC Address matcher
        DtaMacAddrFilterLutUpdate(pAddrMBase);
        DtaMacAddrFilterDelFilteredEn(pAddrMBase, 1);

        DtDbgOut(MAX, PHYMAC, "[%i]  Finished. Num. Entries: %i ",
                                                      pIpPort->m_IpPortIndex, NumEntries);
    } else {
        DtDbgOut(MAX, PHYMAC, "[%i] Finished. Num. Entries %i > 31. Deletion disabled",
                                      pIpPort->m_IpPortIndex, pPhyMac->m_NumMulticasts+1);
        DtaMacAddrFilterDelFilteredEn(pAddrMBase, 0);
    }
    DtMutexRelease(&pFirstPort->m_IpPortType2.m_MacAddrFiltMutex);
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ Address Matcher Helper Routines +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpGetFirstFreeIdTag -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtaIpGetFirstFreeIdTag(DtaIpPort*  pIpPort)
{
    UInt  i = 0;
    DtaIpDevice*  pIpDevice = &pIpPort->m_pDvcData->m_IpDevice;
    DT_ASSERT(pIpPort->m_pDvcData->m_NumIpPorts <= 2);

    while (i < DTA_MAX_ADDR_MATCHER_ENTRIES)
    {
        if (pIpDevice->m_pAddrMatchLUTable[i] == NULL)
            return i;
        i++;
    }
    DT_ASSERT(FALSE);
    return DTA_MAX_ADDR_MATCHER_ENTRIES - 1;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpSetNewOwnerAddrMatchLUTable -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpSetNewOwnerAddrMatchLUTable(
    DtaIpPort*  pIpPort, 
    UInt  IdTag, 
    UserIpRxChannel*  pIpRx,
    UInt8  StreamType)
{
    DtaIpDevice*  pIpDevice = &pIpPort->m_pDvcData->m_IpDevice;
    pIpDevice->m_pAddrMatchLUTable[IdTag] = pIpRx;
    pIpDevice->m_AddrMatchLUTableType[IdTag] = StreamType;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpRemoveIdTag -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpRemoveIdTag(DtaIpPort* pIpPort, UInt IdTag)
{
    DtaIpDevice*  pIpDevice = &pIpPort->m_pDvcData->m_IpDevice;
    DT_ASSERT(pIpPort->m_pDvcData->m_NumIpPorts <= 2);

    pIpDevice->m_pAddrMatchLUTable[IdTag] = NULL;
    return;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpAddrMatcherAddEntryPart2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpAddrMatcherAddEntryPart2(
    DtaIpPort*  pIpPort,
    AddressMatcherLookupEntry*  pEntryHead,
    AddressMatcherLookupEntryPart2*  pNewEntry)
{
    AddressMatcherLookupEntryPart2*  pLastEntry = pEntryHead->m_pPart2;
    AddressMatcherLookupEntryPart2*  pEntry = pEntryHead->m_pPart2;
    UserIpRxChannel*  pIpRxNewEntry = NULL;
    UserIpRxChannel*  pIpRxEntry = NULL;
    UInt  IpPort =  pIpPort->m_IpPortIndex;
    UInt  IdTag;
    
    pNewEntry->m_pNext = NULL;
    pNewEntry->m_pPrev = NULL;
    
    pIpRxNewEntry = DtContainingRecord(pNewEntry, UserIpRxChannel, 
                             m_AddrMatcherEntrySSMPart2[pNewEntry->m_StreamType][IpPort]);
    while (pEntry != NULL)
    {
        pLastEntry = pEntry;

        // Search for location to insert
        if (pNewEntry->m_Data.m_Data5 < pEntry->m_Data.m_Data5)
            break;
        if (pNewEntry->m_Data.m_Data5 > pEntry->m_Data.m_Data5)
        {
            pEntry = pEntry->m_pNext;
            continue;
        }
        if (pNewEntry->m_Data.m_Data4 < pEntry->m_Data.m_Data4)
            break;
        if (pNewEntry->m_Data.m_Data4 > pEntry->m_Data.m_Data4)
        {
            pEntry = pEntry->m_pNext;
            continue;
        }
        if (pNewEntry->m_Data.m_Data3 < pEntry->m_Data.m_Data3)
            break;
        if (pNewEntry->m_Data.m_Data3 > pEntry->m_Data.m_Data3)
        {
            pEntry = pEntry->m_pNext;
            continue;
        }
        if (pNewEntry->m_Data.m_Data2 < pEntry->m_Data.m_Data2)
            break;
        if (pNewEntry->m_Data.m_Data2 > pEntry->m_Data.m_Data2)
        {
            pEntry = pEntry->m_pNext;
            continue;
        }
        if (pNewEntry->m_Data.m_Data1 < pEntry->m_Data.m_Data1)
            break;
        if (pNewEntry->m_Data.m_Data1 > pEntry->m_Data.m_Data1)
        {
            pEntry = pEntry->m_pNext;
            continue;
        }
        // Skip data 0 check. AddressIDTag and NbEntries doesn't need to be sorted and
        // should not be checked otherwise same entries can't be detected
        /*if (pNewEntry->m_Data.m_Data0 < pEntry->m_Data.m_Data0)
            break;
        if (pNewEntry->m_Data.m_Data0 > pEntry->m_Data.m_Data0)
        {
            pEntry = pEntry->m_pNext;
            continue;
        }*/

        // Entries are the same
        pNewEntry->m_IpV6SSM.m_IpV6Part2.m_AddressIDTag = 
                                             pEntry->m_IpV6SSM.m_IpV6Part2.m_AddressIDTag;
        pIpRxEntry = DtContainingRecord(pEntry, UserIpRxChannel, 
                                m_AddrMatcherEntrySSMPart2[pEntry->m_StreamType][IpPort]);
        pIpRxNewEntry->m_pNextAddrMatcherEntryPart2[pNewEntry->m_StreamType][IpPort] = 
                   pIpRxEntry->m_pNextAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort];
        pIpRxNewEntry->m_pPrevAddrMatcherEntryPart2[pNewEntry->m_StreamType][IpPort] =
                                                                                   pEntry;
        pIpRxEntry->m_pNextAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort] = 
                                                                                pNewEntry;
            
        DtDbgOut(MAX, IPADDRM, "[%i] Found same IPV6 element for NewEntry: %p. IdTag=%x",
                                         pIpPort->m_IpPortIndex, pNewEntry,
                                         pNewEntry->m_IpV6SSM.m_IpV6Part2.m_AddressIDTag);
        return;
    }
    
    if (pEntry == NULL)
    {
        // Insert at end
        pLastEntry->m_pNext = pNewEntry;
        pNewEntry->m_pPrev = pLastEntry;
    } else {
        // Insert before entry
        if (pEntry->m_pPrev == NULL)
        {
            DT_ASSERT(pEntry->m_pHead != NULL);
            pEntry->m_pHead->m_pPart2 = pNewEntry;
            pNewEntry->m_pPrev = NULL;
            pNewEntry->m_pHead = pEntry->m_pHead;
            pEntry->m_pHead = NULL;
            pEntry->m_pPrev = pNewEntry;
            pNewEntry->m_pNext = pEntry;
        } else {
            DT_ASSERT(pEntry->m_pHead == NULL);
            pEntry->m_pPrev->m_pNext = pNewEntry;
            pNewEntry->m_pPrev = pEntry->m_pPrev;
            pEntry->m_pPrev = pNewEntry;
            pNewEntry->m_pNext = pEntry;
        }
    }

    IdTag = DtaIpGetFirstFreeIdTag(pIpPort);
    pNewEntry->m_IpV6SSM.m_IpV6Part2_First.m_AddressIDTag = IdTag;
        
    DtaIpSetNewOwnerAddrMatchLUTable(pIpPort, IdTag, pIpRxNewEntry, 
                                                                 pNewEntry->m_StreamType);
    
    DtDbgOut(MAX, IPADDRM, "[%i] New entry: %p inserted. IdTag=%x", 
                                                pIpPort->m_IpPortIndex, pNewEntry, IdTag);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpAddrMatcherAddEntry -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpAddrMatcherAddEntry(
    DtaIpPort*  pIpPort,
    AddressMatcherLookupEntry*  pHead, 
    AddressMatcherLookupEntry*  pNewEntry,
    AddressMatcherLookupEntryPart2*  pNewEntryPart2)
{
    AddressMatcherLookupEntry*  pLastEntry = pHead;
    AddressMatcherLookupEntry*  pEntry;
    UInt  IdTag;
    UserIpRxChannel*  pIpRxNewEntry = NULL;
    UserIpRxChannel*  pIpRxEntry = NULL;
    UInt  IpPort =  pIpPort->m_IpPortIndex;
    UInt  FirstIpPortNum = pIpPort->m_IpPortIndex&0xfffffffe;
    DtaIpPort*  pFirstPort = &pIpPort->m_pDvcData->m_IpDevice.m_pIpPorts[FirstIpPortNum];
    
    DtMutexAcquire(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex, -1);
    pEntry = pHead->m_pNext;
    pIpRxNewEntry = DtContainingRecord(pNewEntry, UserIpRxChannel, 
                                     m_AddrMatcherEntry[pNewEntry->m_StreamType][IpPort]);
    pIpRxNewEntry->m_pNextAddrMatcherEntry[pNewEntry->m_StreamType][IpPort] = NULL;
    pIpRxNewEntry->m_pPrevAddrMatcherEntry[pNewEntry->m_StreamType][IpPort] = NULL;
    pIpRxNewEntry->m_pNextAddrMatcherEntryPart2[pNewEntry->m_StreamType][IpPort] = NULL;
    pIpRxNewEntry->m_pPrevAddrMatcherEntryPart2[pNewEntry->m_StreamType][IpPort] = NULL;

    DT_ASSERT(pNewEntry->m_pNext==NULL && pNewEntry->m_pPrev== NULL && 
                                                               pNewEntry->m_pPart2==NULL);
    DT_ASSERT(pNewEntryPart2==NULL || pNewEntryPart2->m_pHead==NULL);

    DtDbgOut(MAX, IPADDRM, "[%i] Search location to insert NewEntry: %p", 
                                                       pIpPort->m_IpPortIndex, pNewEntry);
    
    while (pEntry != NULL)
    {
        pLastEntry = pEntry;

        // Search for location to insert
        // The m_Nrt and m_AddressIDTag does not need to be sorted so the m_AddressIDTag 
        // can be changed later on!
        if (pNewEntry->m_Data.m_Data5 < pEntry->m_Data.m_Data5)
            break;
        if (pNewEntry->m_Data.m_Data5 > pEntry->m_Data.m_Data5)
        {
            pEntry = pEntry->m_pNext;
            continue;
        }
        if (pNewEntry->m_Data.m_Data4 < pEntry->m_Data.m_Data4)
            break;
        if (pNewEntry->m_Data.m_Data4 > pEntry->m_Data.m_Data4)
        {
            pEntry = pEntry->m_pNext;
            continue;
        }
        if (pNewEntry->m_Data.m_Data3 < pEntry->m_Data.m_Data3)
            break;
        if (pNewEntry->m_Data.m_Data3 > pEntry->m_Data.m_Data3)
        {
            pEntry = pEntry->m_pNext;
            continue;
        }
        if (pNewEntry->m_Data.m_Data2 < pEntry->m_Data.m_Data2)
            break;
        if (pNewEntry->m_Data.m_Data2 > pEntry->m_Data.m_Data2)
        {
            pEntry = pEntry->m_pNext;
            continue;
        }
        if (pNewEntry->m_Data.m_Data1 < pEntry->m_Data.m_Data1)
            break;
        if (pNewEntry->m_Data.m_Data1 > pEntry->m_Data.m_Data1)
        {
            pEntry = pEntry->m_pNext;
            continue;
        }
        // The lowest 16-bits does not need to be sorted. Also the ID tag
        // is in there that is different for the same packet(can be 0 when not yet
        // filled, so we filter it here.
        if ((pNewEntry->m_Data.m_Data0&0xfffe0000) < (pEntry->m_Data.m_Data0&0xfffe0000))
            break;
        if ((pNewEntry->m_Data.m_Data0&0xfffe0000) > (pEntry->m_Data.m_Data0&0xfffe0000))
        {
            pEntry = pEntry->m_pNext;
            continue;
        }
        
        // Entries are the same, Can happen with SSM IPv6 but also with IPv4.
        if (pEntry->m_IpV6.m_Version==1 && pEntry->m_IpV6.m_SSM==1)
        {
            // Add SSM to list
            DtaIpAddrMatcherAddEntryPart2(pIpPort, pEntry, pNewEntryPart2);
            DtDbgOut(MAX, IPADDRM, "[%i] SSM IPv6 entry added for NewEntry: %p. IdTag=%x", 
                              pIpPort->m_IpPortIndex, pNewEntry, 
                              pNewEntryPart2->m_IpV6SSM.m_IpV6Part2_First.m_AddressIDTag);
        } else {
            pNewEntry->m_Gen.m_AddressIDTag = pEntry->m_Gen.m_AddressIDTag;
            pIpRxEntry = DtContainingRecord(pEntry, UserIpRxChannel, 
                                        m_AddrMatcherEntry[pEntry->m_StreamType][IpPort]);
            pIpRxNewEntry->m_pNextAddrMatcherEntry[pNewEntry->m_StreamType][IpPort] = 
                        pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType][IpPort];
            pIpRxNewEntry->m_pPrevAddrMatcherEntry[pNewEntry->m_StreamType][IpPort] = 
                                                                                   pEntry;
            if (pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType][IpPort] != NULL)
            {
                Int StreamType2 = pIpRxEntry->m_pNextAddrMatcherEntry\
                                             [pEntry->m_StreamType][IpPort]->m_StreamType;
                UserIpRxChannel*  pIpRxEntryTmp = NULL;
    
                pIpRxEntryTmp = DtContainingRecord(pIpRxEntry->m_pNextAddrMatcherEntry \
                                          [pEntry->m_StreamType][IpPort], UserIpRxChannel,
                                          m_AddrMatcherEntry[StreamType2][IpPort]);
                pIpRxEntryTmp->m_pPrevAddrMatcherEntry[StreamType2][IpPort] = pNewEntry;
            }

            pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType][IpPort] = pNewEntry;
            
            
            DtDbgOut(MAX, IPADDRM, "[%i] Found same element for NewEntry: %p. IdTag=%x", 
                                                    pIpPort->m_IpPortIndex, pNewEntry,
                                                    (Int)pNewEntry->m_Gen.m_AddressIDTag);
        }
        DtMutexRelease(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex);
        return;
    }

    if (pEntry == NULL)
    {
        // Insert at end
        pLastEntry->m_pNext = pNewEntry;
        pNewEntry->m_pPrev = pLastEntry;
    } else {
        // Insert before entry
        pEntry->m_pPrev->m_pNext = pNewEntry;
        pNewEntry->m_pPrev = pEntry->m_pPrev;
        pEntry->m_pPrev = pNewEntry;
        pNewEntry->m_pNext = pEntry;
    }

    IdTag = DtaIpGetFirstFreeIdTag(pIpPort);
        
    if (pNewEntry->m_Gen.m_SSM==1 && pNewEntry->m_Gen.m_Version==1)
    {
        pNewEntry->m_pPart2 = pNewEntryPart2;
        pNewEntryPart2->m_IpV6SSM.m_IpV6Part2_First.m_AddressIDTag = IdTag;
        pNewEntryPart2->m_pPrev = NULL;
        pNewEntryPart2->m_pNext = NULL;
        pNewEntryPart2->m_pHead = pNewEntry;
    } else
        pNewEntry->m_Gen.m_AddressIDTag = IdTag;
        
    DtaIpSetNewOwnerAddrMatchLUTable(pIpPort, IdTag, pIpRxNewEntry, 
                                                                 pNewEntry->m_StreamType);
    
    DtDbgOut(MAX, IPADDRM, "[%i] New entry: %p inserted. IdTag=%x", 
                                                pIpPort->m_IpPortIndex, pNewEntry, IdTag);
    DtMutexRelease(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex);
    return;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpAddrMatcherDeleteIpV4_6Entry -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Pre: m_AddrMatcherMutex is acquired
//
void  DtaIpAddrMatcherDeleteIpV4_6Entry(
    DtaIpPort*  pIpPort, 
    AddressMatcherLookupEntry*  pEntry)
{
    UInt8  StreamType;
    UInt  IpPort = pIpPort->m_IpPortIndex;
    UserIpRxChannel*  pIpRxEntry = NULL;
    
    pIpRxEntry = DtContainingRecord(pEntry, UserIpRxChannel, 
                                        m_AddrMatcherEntry[pEntry->m_StreamType][IpPort]);
    
    if (pEntry->m_pNext!=NULL || pEntry->m_pPrev!=NULL)
    {
        // This is a root element, check if child elements are attached
        DT_ASSERT(pIpRxEntry->m_pPrevAddrMatcherEntry[pEntry->m_StreamType][IpPort] == 
                                                                                    NULL);
        if (pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType][IpPort] != NULL) 
        {
            // Another element has same IP-settings. Set other element as root element
            UserIpRxChannel*  pIpRxEntry2 = NULL;
            AddressMatcherLookupEntry*  pEntry2 = 
                        pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType][IpPort];
            StreamType = pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType] \
                                                                   [IpPort]->m_StreamType;
            pIpRxEntry2 = DtContainingRecord(
                       pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType][IpPort],
                       UserIpRxChannel, m_AddrMatcherEntry[pEntry->m_StreamType][IpPort]);
            DT_ASSERT(pIpRxEntry2->m_pPrevAddrMatcherEntry[StreamType][IpPort] == pEntry);
            pIpRxEntry2->m_pPrevAddrMatcherEntry[StreamType][IpPort] = NULL;
            pEntry2->m_pNext = pEntry->m_pNext;
            if (pEntry->m_pNext != NULL)
                pEntry2->m_pNext->m_pPrev = pEntry2;
            pEntry2->m_pPrev = pEntry->m_pPrev;
            pEntry->m_pPrev->m_pNext = pEntry2;
            DtaIpSetNewOwnerAddrMatchLUTable(pIpPort, (UInt)pEntry2->m_Gen.m_AddressIDTag,
                                                                 pIpRxEntry2, StreamType);
            DtDbgOut(MAX, IPADDRM, "[%i] Entry: %p deleted and new owner %p."
                                     " IdTag=%x", pIpPort->m_IpPortIndex, pEntry, pEntry2,
                                     (Int)pEntry->m_Gen.m_AddressIDTag);
        } else {
            // No child elements, we can remove it without setting new root element
            pEntry->m_pPrev->m_pNext = pEntry->m_pNext;
            if (pEntry->m_pNext != NULL)
                pEntry->m_pNext->m_pPrev = pEntry->m_pPrev;
            DtaIpRemoveIdTag(pIpPort, (UInt)pEntry->m_Gen.m_AddressIDTag);
            DtDbgOut(MAX, IPADDRM, "[%i] Entry: %p deleted and no new owner."
                                              " IdTag=%x", pIpPort->m_IpPortIndex, pEntry,
                                              (Int)pEntry->m_Gen.m_AddressIDTag);
        }
        pEntry->m_pPrev = NULL;
        pEntry->m_pNext = NULL;
    } else {
        UserIpRxChannel*  pIpRxEntryTmp = NULL;
            
        // This is a sub-element, we only need to change the child elements
        DT_ASSERT(pEntry->m_pNext==NULL && pEntry->m_pPrev==NULL);
        if (pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType][IpPort] != NULL)
        {
            StreamType = pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType] \
                                                                   [IpPort]->m_StreamType;
            // Update the previous element of the next element
            pIpRxEntryTmp = DtContainingRecord(
                       pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType][IpPort],
                       UserIpRxChannel, m_AddrMatcherEntry[pEntry->m_StreamType][IpPort]);
            pIpRxEntryTmp->m_pPrevAddrMatcherEntry[StreamType][IpPort] =
                        pIpRxEntry->m_pPrevAddrMatcherEntry[pEntry->m_StreamType][IpPort];
        }
        // Update the next  element of the previous element
        StreamType = pIpRxEntry->m_pPrevAddrMatcherEntry[pEntry->m_StreamType] \
                                                                   [IpPort]->m_StreamType;
        pIpRxEntryTmp = DtContainingRecord(
                       pIpRxEntry->m_pPrevAddrMatcherEntry[pEntry->m_StreamType][IpPort],
                       UserIpRxChannel, m_AddrMatcherEntry[pEntry->m_StreamType][IpPort]);
        pIpRxEntryTmp->m_pNextAddrMatcherEntry[StreamType][IpPort] = 
                        pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType][IpPort];
        pIpRxEntry->m_pPrevAddrMatcherEntry[pEntry->m_StreamType][IpPort] = NULL;
        pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType][IpPort] = NULL;
        DtDbgOut(MAX, IPADDRM, "[%i] Entry: %p deleted. No owner change."
                                              " IdTag=%x", pIpPort->m_IpPortIndex, pEntry,
                                              (Int)pEntry->m_Gen.m_AddressIDTag);
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpAddrMatcherDeleteIpV6SSMEntry -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Pre: m_AddrMatcherMutex is acquired
//
void  DtaIpAddrMatcherDeleteIpV6SSMEntry(
    DtaIpPort*  pIpPort,
    AddressMatcherLookupEntry*  pEntry,
    AddressMatcherLookupEntryPart2*  pEntryPart2)
{
    UInt8  StreamType;
    UInt  IpPort = pIpPort->m_IpPortIndex;
    UserIpRxChannel*  pIpRxEntry = NULL;
    pIpRxEntry = DtContainingRecord(pEntry, UserIpRxChannel, 
                                        m_AddrMatcherEntry[pEntry->m_StreamType][IpPort]);
    
    //+=+=+=+=+=+=+=+=+=+=+=+=+ First check the part 2 elements +=+=+=+=+=+=+=+=+=+=+=+=+=
    if (pEntryPart2->m_pNext!=NULL || pEntryPart2->m_pPrev!=NULL || 
                                                               pEntryPart2->m_pHead!=NULL)
    {
        // This is a root element, check if child elements are attached
        DT_ASSERT(pIpRxEntry->m_pPrevAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort] 
                                                                                 == NULL);
        if (pIpRxEntry->m_pNextAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort] != 
                                                                                     NULL)
        {
            // Another element has same IP-settings. Set other element as root element
            UserIpRxChannel*  pIpRxEntry2 = NULL;
            AddressMatcherLookupEntryPart2*  pEntryPart2_2 = 
                   pIpRxEntry->m_pNextAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort];
            StreamType =  pIpRxEntry->m_pNextAddrMatcherEntryPart2[pEntry->m_StreamType] \
                                                                   [IpPort]->m_StreamType;
            pIpRxEntry2 = DtContainingRecord(
               pIpRxEntry->m_pNextAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort],
               UserIpRxChannel, m_AddrMatcherEntrySSMPart2[pEntry->m_StreamType][IpPort]);
            DT_ASSERT(pIpRxEntry2->m_pPrevAddrMatcherEntryPart2[StreamType][IpPort] == 
                                                                             pEntryPart2);
            pIpRxEntry2->m_pPrevAddrMatcherEntryPart2[StreamType][IpPort] = NULL;
            pEntryPart2_2->m_pNext = pEntryPart2->m_pNext;
            if (pEntryPart2->m_pNext != NULL)
                pEntryPart2_2->m_pNext->m_pPrev = pEntryPart2_2;
            pEntryPart2_2->m_pPrev = pEntryPart2->m_pPrev;
            if (pEntryPart2->m_pPrev == NULL) 
            {
                DT_ASSERT(pEntryPart2->m_pHead != NULL);
                
                pEntryPart2->m_pHead->m_pPart2 = pEntryPart2_2;
                pEntryPart2_2->m_pHead = pEntryPart2->m_pHead;
            } else 
                pEntryPart2->m_pPrev->m_pNext = pEntryPart2_2;
            DtaIpSetNewOwnerAddrMatchLUTable(pIpPort, 
                            (UInt)pEntryPart2->m_IpV6SSM.m_IpV6Part2_First.m_AddressIDTag,
                            pIpRxEntry2, StreamType);
            DtDbgOut(MAX, IPADDRM, "[%i] Entry: %p deleted and new owner %p."
                          " IdTag=%x", pIpPort->m_IpPortIndex, pEntryPart2, pEntryPart2_2,
                          (Int)pEntryPart2_2->m_IpV6SSM.m_IpV6Part2_First.m_AddressIDTag);
        } else {
            // No child elements, we can remove it without setting new root element
            if (pEntryPart2->m_pPrev == NULL)
            {
                DT_ASSERT(pEntryPart2->m_pHead != NULL);
                
                pEntryPart2->m_pHead->m_pPart2 = pEntryPart2->m_pNext;
                if (pEntryPart2->m_pNext != NULL)
                {
                    pEntryPart2->m_pNext->m_pHead = pEntryPart2->m_pHead;
                    pEntryPart2->m_pNext->m_pPrev = NULL;
                }
                
            } else {
                pEntryPart2->m_pPrev->m_pNext = pEntryPart2->m_pNext;
                if (pEntryPart2->m_pNext != NULL)
                    pEntryPart2->m_pNext->m_pPrev = pEntryPart2->m_pPrev;
            }
            DtaIpRemoveIdTag(pIpPort,
                           (UInt)pEntryPart2->m_IpV6SSM.m_IpV6Part2_First.m_AddressIDTag);
            DtDbgOut(MAX, IPADDRM, "[%i] Entry: %p deleted and no new owner."
                            " IdTag=%x", pIpPort->m_IpPortIndex, pEntryPart2,
                            (Int)pEntryPart2->m_IpV6SSM.m_IpV6Part2_First.m_AddressIDTag);
        }
        pEntryPart2->m_pPrev = NULL;
        pEntryPart2->m_pNext = NULL;
        pEntryPart2->m_pHead = NULL;
    } else {
        UserIpRxChannel*  pIpRxEntryTmp = NULL;
            
        // This is a sub-element, we only need to change the child elements
        DT_ASSERT(pEntryPart2->m_pNext==NULL && pEntryPart2->m_pPrev==NULL &&
                                                              pEntryPart2->m_pHead==NULL);
        if (pIpRxEntry->m_pNextAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort]
                                                                                  != NULL)
        {
            StreamType = pIpRxEntry->m_pNextAddrMatcherEntryPart2[pEntry->m_StreamType] \
                                                                   [IpPort]->m_StreamType;
            
            // Update the previous element of the next element
            pIpRxEntryTmp = DtContainingRecord(
               pIpRxEntry->m_pNextAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort],
               UserIpRxChannel, m_AddrMatcherEntrySSMPart2[pEntry->m_StreamType][IpPort]);
            pIpRxEntryTmp->m_pPrevAddrMatcherEntryPart2[StreamType][IpPort] =
                   pIpRxEntry->m_pPrevAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort];
        }
        // Update the next  element of the previous element
        StreamType = pIpRxEntry->m_pPrevAddrMatcherEntryPart2[pEntry->m_StreamType] \
                                                                   [IpPort]->m_StreamType;
        pIpRxEntryTmp = DtContainingRecord(
               pIpRxEntry->m_pPrevAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort],
               UserIpRxChannel, m_AddrMatcherEntrySSMPart2[pEntry->m_StreamType][IpPort]);
        pIpRxEntryTmp->m_pNextAddrMatcherEntryPart2[StreamType][IpPort] = 
                   pIpRxEntry->m_pNextAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort];
        pIpRxEntry->m_pPrevAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort] = NULL;
        pIpRxEntry->m_pNextAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort] = NULL;
        DtDbgOut(MAX, IPADDRM, "[%i] Entry: %p deleted. No owner change."
                            " IdTag=%x", pIpPort->m_IpPortIndex, pEntryPart2,
                            (Int)pEntryPart2->m_IpV6SSM.m_IpV6Part2_First.m_AddressIDTag);
    }
    //=+=+=+=+=+=+=+=+=+=+=+=+=+ Now check the part 1 elements +=+=+=+=+=+=+=+=+=+=+=+=+=+
    if (pEntry->m_pPart2 != NULL)
    {
        UserIpRxChannel*  pIpRxEntryTmp = NULL;
        AddressMatcherLookupEntry*  pEntry2;
        DT_ASSERT(pEntry->m_pPrev != NULL);
        StreamType = pEntry->m_pPart2->m_StreamType;
        // We have to set a new part1 element
        pIpRxEntryTmp = DtContainingRecord(pEntry->m_pPart2,
                         UserIpRxChannel, m_AddrMatcherEntrySSMPart2 \
                                                [pEntry->m_pPart2->m_StreamType][IpPort]);
        DT_ASSERT(pIpRxEntryTmp != pIpRxEntry);
        pEntry2 = &pIpRxEntryTmp->m_AddrMatcherEntry[StreamType][IpPort];
        DT_ASSERT(pEntry2->m_pNext==NULL && pEntry2->m_pPrev==NULL && 
                                                                 pEntry2->m_pPart2==NULL);
        pEntry2->m_pPart2 = pEntry->m_pPart2;
        DT_ASSERT(pEntry2->m_pPart2->m_pPrev==NULL && pEntry2->m_pPart2->m_pHead!=NULL);
        pEntry2->m_pPart2->m_pHead = pEntry2;
        pEntry2->m_pNext = pEntry->m_pNext;
        if (pEntry->m_pNext != NULL)
            pEntry->m_pNext->m_pPrev = pEntry2;
        pEntry2->m_pPrev = pEntry->m_pPrev;
        if (pEntry->m_pPrev != NULL)
            pEntry->m_pPrev->m_pNext = pEntry2;
    } else {
        // We don't need to set another head. We can remove this element from the list
        if (pEntry->m_pNext != NULL)
            pEntry->m_pNext->m_pPrev = pEntry->m_pPrev;
        if (pEntry->m_pPrev != NULL)
            pEntry->m_pPrev->m_pNext = pEntry->m_pNext;
    }

    pEntry->m_pPart2 = NULL;
    pEntry->m_pNext = NULL;
    pEntry->m_pPrev = NULL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpAddrMatcherDeleteEntry -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// If pEntry is not in the list, this is a NOP.
//
void  DtaIpAddrMatcherDeleteEntry(
    DtaIpPort*  pIpPort,
    AddressMatcherLookupEntry*  pEntry,
    AddressMatcherLookupEntryPart2*  pEntryPart2)
{
    UInt  FirstIpPortNum = pIpPort->m_IpPortIndex&0xfffffffe;
    DtaIpPort*  pFirstPort = &pIpPort->m_pDvcData->m_IpDevice.m_pIpPorts[FirstIpPortNum];
    UInt  IpPort = pIpPort->m_IpPortIndex;
    UserIpRxChannel*  pIpRxEntry = NULL;
    pIpRxEntry = DtContainingRecord(pEntry, UserIpRxChannel, 
                                        m_AddrMatcherEntry[pEntry->m_StreamType][IpPort]);
    
    DtMutexAcquire(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex, -1);

    if (pEntry->m_pNext==NULL && pEntry->m_pPrev==NULL && 
          pIpRxEntry->m_pPrevAddrMatcherEntry[pEntry->m_StreamType][IpPort]== NULL &&
          pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType][IpPort]== NULL &&
          pEntryPart2->m_pNext==NULL && pEntryPart2->m_pPrev==NULL &&
          pEntryPart2->m_pHead==NULL &&
          pIpRxEntry->m_pPrevAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort]== NULL &&
          pIpRxEntry->m_pNextAddrMatcherEntryPart2[pEntry->m_StreamType][IpPort]== NULL)
    {
        // Entry not in list
        DtDbgOut(MAX, IPADDRM, "[%i] Entry: %p not in list.",
                                                          pIpPort->m_IpPortIndex, pEntry);
        DtMutexRelease(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex);
        return;
    }

    if (pEntry->m_IpV6SSM.m_IpV6Part1.m_Version==1  && 
                                                   pEntry->m_IpV6SSM.m_IpV6Part1.m_SSM==1)
        DtaIpAddrMatcherDeleteIpV6SSMEntry(pIpPort, pEntry, pEntryPart2);
    else 
        DtaIpAddrMatcherDeleteIpV4_6Entry(pIpPort, pEntry);
    
    DtMutexRelease(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpAddrMatcherUpdateTable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function will update both IP-ports in firmware
//
void  DtaIpAddrMatcherUpdateTable(
    DtaIpPort*  pIpPort,
    AddressMatcherLookupEntry*  pHead)
{
    volatile UInt8*  pAddrMLUBase = pIpPort->m_IpPortType2.m_pAddrMatcherLookupRegs;
    volatile UInt8*  pAddrMBase[2];

    UInt  FirstIpPortNum = pIpPort->m_IpPortIndex&0xfffffffe;
    DtaIpPort*  pFirstPort = &pIpPort->m_pDvcData->m_IpDevice.m_pIpPorts[FirstIpPortNum];
    UInt  ActiveTbl;
    UInt  NewTbl;
    UInt  NumEntries = 0;
    UInt  NumSSMEntries = 0;
    AddressMatcherLookupEntry*  pEntry;
    
    pAddrMBase[0] = pFirstPort->m_IpPortType2.m_pAddrMatcherRegs;
    pAddrMBase[1] = pFirstPort->m_pDvcData->m_IpDevice.m_pIpPorts
                         [pFirstPort->m_IpPortIndex + 1].m_IpPortType2.m_pAddrMatcherRegs;

    // Protect updating port 0/1
    DtMutexAcquire(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex, -1);
    pEntry = pHead->m_pNext;

    // Busy wait until address matcher is ready updating a previous switch
    // Max. wait time about 12us after update if fired for 1Gb
    while (DtaAddrMatchStatGetLutUpdatePending(pAddrMBase[0])!=0 ||
                                   DtaAddrMatchStatGetLutUpdatePending(pAddrMBase[1])!=0)
    {
        DtDbgOut(ERR, IPADDRM, "Address matcher pending: %u %u", 
                                      DtaAddrMatchStatGetLutUpdatePending(pAddrMBase[0]),
                                      DtaAddrMatchStatGetLutUpdatePending(pAddrMBase[1]));
    }

    ActiveTbl = DtaAddrMatchStatGetLutSel(pAddrMBase[0]);
    NewTbl = (ActiveTbl == 0? 1 : 0);
    DT_ASSERT(DtaAddrMatchStatGetLutSel(pAddrMBase[0]) == 
                                               DtaAddrMatchStatGetLutSel( pAddrMBase[1]));

    if (NewTbl == 0)
        NumEntries = DtaAddrMLookupCtrlGetNumEntryTbl0(pAddrMLUBase);
    else
        NumEntries = DtaAddrMLookupCtrlGetNumEntryTbl1(pAddrMLUBase);

    DtDbgOut(AVG, IPADDRM, "ActiveTbl: %u, NewTbl: %u, OldNumEntries-1: %u", 
                                                           ActiveTbl, NewTbl, NumEntries);
    // Remove all entries
    // Start at entry 0
    /*DtaAddrMLookupAddrSet(pAddrMLUBase, 0);
    for (i=0; i<NumEntries; i++)
    {
        DtaAddrMLookupData0Set(pAddrMLUBase, 0);
        DtaAddrMLookupData1Set(pAddrMLUBase, 0);
        DtaAddrMLookupData2Set(pAddrMLUBase, 0);
        DtaAddrMLookupData3Set(pAddrMLUBase, 0);
        DtaAddrMLookupData4Set(pAddrMLUBase, 0);
        DtaAddrMLookupData5Set(pAddrMLUBase, 0);
    }*/

    // Start at entry 0
    DtaAddrMLookupAddrSet(pAddrMLUBase, 0);
    NumEntries = 0;
    NumSSMEntries = 0;

    while (pEntry)
    {
        if (pEntry->m_Gen.m_SSM==1 && pEntry->m_Gen.m_Version==1)
        {
            AddressMatcherLookupEntryPart2*  pPart2;
            UInt32  LastLocation = DtaAddrMLookupAddrGet(pAddrMLUBase);
            UInt  Count = 0;
            DT_ASSERT(pEntry->m_pPart2 != NULL);
            // Source specific multicast. Count part2 entries
            pPart2 = pEntry->m_pPart2;
            while (pPart2 != NULL)
            {
                Count++;
                pPart2 = pPart2->m_pNext;
            }

            NumSSMEntries += Count;
            pEntry->m_IpV6SSM.m_IpV6Part1.m_EntryPart2 = DTA_MAX_ADDR_MATCHER_ENTRIES - 
                                                                        NumSSMEntries - 1;
            DtaAddrMLookupAddrSet(pAddrMLUBase, DTA_MAX_ADDR_MATCHER_ENTRIES -
                                                                       NumSSMEntries - 1);
            // Store part2 entries in AddressMatcher
            pPart2 = pEntry->m_pPart2;
            pPart2->m_IpV6SSM.m_IpV6Part2_First.m_NbEntries = Count;
            Count = 0;
            while (pPart2 != NULL)
            {
                // Clear all NbEntries except first one just to be clean
                if (Count != 0)
                    pPart2->m_IpV6SSM.m_IpV6Part2.m_NbEntriesClear = 0;
            
                DtaAddrMLookupData0Set(pAddrMLUBase, pPart2->m_Data.m_Data0);
                DtaAddrMLookupData1Set(pAddrMLUBase, pPart2->m_Data.m_Data1);
                DtaAddrMLookupData2Set(pAddrMLUBase, pPart2->m_Data.m_Data2);
                DtaAddrMLookupData3Set(pAddrMLUBase, pPart2->m_Data.m_Data3);
                DtaAddrMLookupData4Set(pAddrMLUBase, pPart2->m_Data.m_Data4);
                DtaAddrMLookupData5Set(pAddrMLUBase, pPart2->m_Data.m_Data5);
                DtDbgOut(MAX, IPADDRM, 
                    "AMDataXSet: EntryP2:%p LookupIndex:%02i, Id:%02i %08x %08x %08x %08x"
                    " %08x %08x",
                    pPart2, DTA_MAX_ADDR_MATCHER_ENTRIES-NumSSMEntries+Count-1, 
                    (Int)pPart2->m_IpV6SSM.m_IpV6Part2_First.m_AddressIDTag,
                    pPart2->m_Data.m_Data5, pPart2->m_Data.m_Data4,
                    pPart2->m_Data.m_Data3, pPart2->m_Data.m_Data2,
                    pPart2->m_Data.m_Data1, pPart2->m_Data.m_Data0);
                pPart2 = pPart2->m_pNext;
                Count++;
            }
            
            // Restore last location for part1's
            DT_ASSERT(LastLocation < pEntry->m_IpV6SSM.m_IpV6Part1.m_EntryPart2);
            DtaAddrMLookupAddrSet(pAddrMLUBase, LastLocation);
        }

        DtaAddrMLookupData0Set(pAddrMLUBase, pEntry->m_Data.m_Data0);
        DtaAddrMLookupData1Set(pAddrMLUBase, pEntry->m_Data.m_Data1);
        DtaAddrMLookupData2Set(pAddrMLUBase, pEntry->m_Data.m_Data2);
        DtaAddrMLookupData3Set(pAddrMLUBase, pEntry->m_Data.m_Data3);
        DtaAddrMLookupData4Set(pAddrMLUBase, pEntry->m_Data.m_Data4);
        DtaAddrMLookupData5Set(pAddrMLUBase, pEntry->m_Data.m_Data5);
        if (pEntry->m_Gen.m_SSM==1 && pEntry->m_Gen.m_Version==1)
        {
            DtDbgOut(MAX, IPADDRM, 
                      "AMDataXSet: Entry:%p LookupIndex:%02i, I2:%02i %08x %08x %08x %08x"
                      " %08x %08x", 
                      pEntry, NumEntries, pEntry->m_IpV6SSM.m_IpV6Part1.m_EntryPart2,
                      pEntry->m_Data.m_Data5, pEntry->m_Data.m_Data4,
                      pEntry->m_Data.m_Data3, pEntry->m_Data.m_Data2,
                      pEntry->m_Data.m_Data1, pEntry->m_Data.m_Data0);
        } else {
            DtDbgOut(MAX, IPADDRM, 
                      "AMDataXSet: Entry:%p LookupIndex:%02i, Id:%02i %08x %08x %08x %08x"
                      " %08x %08x",
                      pEntry, NumEntries, (Int)pEntry->m_Gen.m_AddressIDTag,
                      pEntry->m_Data.m_Data5, pEntry->m_Data.m_Data4,
                      pEntry->m_Data.m_Data3, pEntry->m_Data.m_Data2,
                      pEntry->m_Data.m_Data1, pEntry->m_Data.m_Data0);
        }
        NumEntries++;
        pEntry = pEntry->m_pNext;
    }

    // The address matcher can not have 0 elements, so we need to create an empty element
    if (NumEntries == 0)
    {
        DtaAddrMLookupData0Set(pAddrMLUBase, 0);
        DtaAddrMLookupData1Set(pAddrMLUBase, 0);
        DtaAddrMLookupData2Set(pAddrMLUBase, 0);
        DtaAddrMLookupData3Set(pAddrMLUBase, 0);
        DtaAddrMLookupData4Set(pAddrMLUBase, 0);
        DtaAddrMLookupData5Set(pAddrMLUBase, 0);
    } else
        NumEntries--;

    // Update the lookup table and set the last used entry (NumEntries-1) to the firmware
    if (NewTbl == 0)
        DtaAddrMLookupCtrlSetNumEntryTbl0(pAddrMLUBase, NumEntries);
    else
        DtaAddrMLookupCtrlSetNumEntryTbl1(pAddrMLUBase, NumEntries);

    DtDbgOut(AVG, IPADDRM, "ActiveTbl: %u, NewTbl: %u, New NumEntries-1: %u", 
                                                           ActiveTbl, NewTbl, NumEntries);
    DtaAddrMatchLutUpdate(pAddrMBase[0]);
    DtaAddrMatchLutUpdate(pAddrMBase[1]);

    DtMutexRelease (&pFirstPort->m_IpPortType2.m_AddrMatcherMutex);
#ifdef  _DEBUG
    DtaIpDumpAddrMatcherTable(pIpPort, pHead);
#endif
}

#ifdef  _DEBUG
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpDumpAddrMatcherTable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Dump complete linked lists of address matcher entries: Only for debugging purposes
//
void  DtaIpDumpAddrMatcherTable(DtaIpPort* pIpPort, AddressMatcherLookupEntry* pHead)
{
    AddressMatcherLookupEntry*  pEntry;
    UInt  FirstIpPortNum = pIpPort->m_IpPortIndex&0xfffffffe;
    DtaIpPort*  pFirstPort = &pIpPort->m_pDvcData->m_IpDevice.m_pIpPorts[FirstIpPortNum];
    
    // Protect updating port 0/1
    DtMutexAcquire(&pFirstPort->m_IpPortType2.m_AddrMatcherMutex, -1);
    pEntry = pHead->m_pNext;

    while (pEntry)
    {
        AddressMatcherLookupEntry*  pEntryCN;
        AddressMatcherLookupEntry*  pEntryCP;

        UserIpRxChannel*  pIpRxEntry = NULL;
        pIpRxEntry = DtContainingRecord(pEntry, UserIpRxChannel, 
                          m_AddrMatcherEntry[pEntry->m_StreamType][pEntry->m_Gen.m_Port]);
        pEntryCN = pIpRxEntry->m_pNextAddrMatcherEntry[pEntry->m_StreamType] \
                                                                   [pEntry->m_Gen.m_Port];
        pEntryCP = pIpRxEntry->m_pPrevAddrMatcherEntry[pEntry->m_StreamType] \
                                                                   [pEntry->m_Gen.m_Port];
        
        DtDbgOut(ERR, IPADDRM, "pEntry:%p ST:%i Prev:%p Next:%p ChildPrev:%p"
                                    " ChildNext:%p", pEntry, pEntry->m_StreamType, 
                                    pEntry->m_pPrev, pEntry->m_pNext, pEntryCP, pEntryCN);
        while (pEntryCN != NULL)
        {
            AddressMatcherLookupEntry*  pEntryTmp = pEntryCN;

            pIpRxEntry = DtContainingRecord(pEntryTmp, UserIpRxChannel, 
                    m_AddrMatcherEntry[pEntryTmp->m_StreamType][pEntryTmp->m_Gen.m_Port]);
            pEntryCN = pIpRxEntry->m_pNextAddrMatcherEntry[pEntryTmp->m_StreamType] \
                                                                [pEntryTmp->m_Gen.m_Port];
            pEntryCP = pIpRxEntry->m_pPrevAddrMatcherEntry[pEntryTmp->m_StreamType] \
                                                                [pEntryTmp->m_Gen.m_Port];
        
            DtDbgOut(ERR, IPADDRM, "    Sub: pEntry:%p ST:%i Prev:%p Next:%p ChildPrev:%p"
                              " ChildNext:%p ", pEntryTmp, pEntryTmp->m_StreamType,
                              pEntryTmp->m_pPrev, pEntryTmp->m_pNext, pEntryCP, pEntryCN);
        }

        if (pEntry->m_Gen.m_SSM==1 && pEntry->m_Gen.m_Version==1)
        {
            AddressMatcherLookupEntryPart2*  pPart2;
            pPart2 = pEntry->m_pPart2;
            while (pPart2 != NULL)
            {
                AddressMatcherLookupEntryPart2*  pEntryP2CN;
                AddressMatcherLookupEntryPart2*  pEntryP2CP;
                UserIpRxChannel*  pIpRxEntry = NULL;
                pIpRxEntry = DtContainingRecord(pPart2, UserIpRxChannel, 
                                        m_AddrMatcherEntrySSMPart2[pPart2->m_StreamType] \
                                        [pEntry->m_Gen.m_Port]);
                pEntryP2CN = 
                          pIpRxEntry->m_pNextAddrMatcherEntryPart2[pPart2->m_StreamType] \
                                                                   [pEntry->m_Gen.m_Port];
                pEntryP2CP = 
                          pIpRxEntry->m_pPrevAddrMatcherEntryPart2[pPart2->m_StreamType] \
                                                                   [pEntry->m_Gen.m_Port];
        
                DtDbgOut(ERR, IPADDRM, "  pEntryP2:%p ST: %i Prev:%p Next:%p ChildPrev:%p"
                                " ChildNext:%p", pPart2, pPart2->m_StreamType, 
                                pPart2->m_pPrev, pPart2->m_pNext, pEntryP2CP, pEntryP2CN);

                while (pEntryP2CN != NULL)
                {
                    AddressMatcherLookupEntryPart2*  pEntryP2Tmp = pEntryP2CN;

                    pIpRxEntry = DtContainingRecord(pEntryP2Tmp, UserIpRxChannel, 
                                   m_AddrMatcherEntrySSMPart2[pEntryP2Tmp->m_StreamType] \
                                                                  [pEntry->m_Gen.m_Port]);
                    pEntryP2CN = pIpRxEntry->m_pNextAddrMatcherEntryPart2 \
                                        [pEntryP2Tmp->m_StreamType][pEntry->m_Gen.m_Port];
                    pEntryP2CP = pIpRxEntry->m_pPrevAddrMatcherEntryPart2 \
                                        [pEntryP2Tmp->m_StreamType][pEntry->m_Gen.m_Port];
        
                    DtDbgOut(ERR, IPADDRM, "    Sub: pEntryP2:%p ST: %i Prev:%p Next:%p"
                                          " ChildPrev:%p ChildNext:%p ", pEntryP2Tmp, 
                                          pEntryP2Tmp->m_StreamType, pEntryP2Tmp->m_pPrev,
                                          pEntryP2Tmp->m_pNext, pEntryP2CN, pEntryP2CP);
                }
                pPart2 = pPart2->m_pNext;
            }
        }
        pEntry = pEntry->m_pNext;
    }
    DtMutexRelease (&pFirstPort->m_IpPortType2.m_AddrMatcherMutex);
}
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpIsIpAddressEmpty -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaIpIsIpAddressEmpty(Bool IpV6, UInt8* pIpAddr)
{
    Int  i;
    for (i=0; i<(IpV6?16:4); i++)
        if (pIpAddr[i] != 0)
            return FALSE;
    return TRUE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpClearIpAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaIpClearIpAddress(Bool IpV6, UInt8* pIpAddr)
{
    Int  i;
    for (i=0; i<(IpV6?16:4); i++)
        pIpAddr[i] = 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpIsMulticastAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaIpIsMulticastAddress(Bool IpV6, UInt8* pIpAddr)
{
    if (IpV6)
        return pIpAddr[0] == 0xff;
    return pIpAddr[0]>=224 && pIpAddr[0]<=239;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpIsSSMulticastAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaIpIsSSMulticastAddress(Bool IpV6, UInt8* pIpAddr)
{
    if (IpV6)
        return pIpAddr[0]==0xff && (pIpAddr[1]&0xf0)==0x30;
    return pIpAddr[0]==232;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpNeedDriverSSMCheckType2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Due to a firmware limitation of the DTA-2162, the address matcher can't use 
// SSM outside of the SSM range. We need to do the check in software.
//
Bool  DtaIpNeedDriverSSMCheckType2(Bool IpV6, UInt8* pIpSrc, UInt8* pIpDst)
{
    if (!DtaIpIsIpAddressEmpty(IpV6, pIpSrc))
    {
        if (!DtaIpIsSSMulticastAddress(IpV6, pIpDst))
            return TRUE;
    }
    return FALSE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpInitIpRxListeners -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpInitIpRxListeners(DtaIpPort*  pIpPort)
{
    DtMutexInit(&pIpPort->m_IpRxListenersMutex);
    DtFastMutexInit(&pIpPort->m_IpRxListenersFastMutex);
    pIpPort->m_pIpRxListeners = (UserIpRxChannel**)DtMemAllocPool(DtPoolNonPaged,
                            MAX_NUM_IPRX_LISTENERS_INIT*sizeof(UserIpRxChannel*), IP_TAG);
    if (pIpPort->m_pIpRxListeners == NULL)
        return DT_STATUS_OUT_OF_MEMORY;
    pIpPort->m_MaxNumListeners = MAX_NUM_IPRX_LISTENERS_INIT;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCleanupIpRxListeners -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpCleanupIpRxListeners(DtaIpPort*  pIpPort)
{
    if (pIpPort->m_pIpRxListeners == NULL)
        return;

    DtMutexAcquire(&pIpPort->m_IpRxListenersMutex, -1);
    DtMemFreePool(pIpPort->m_pIpRxListeners, IP_TAG);
    pIpPort->m_pIpRxListeners = NULL;
    pIpPort->m_MaxNumListeners = 0;
    DtMutexRelease(&pIpPort->m_IpRxListenersMutex);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpReCalcIpRxNumListeners -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// The list is only incrementing in steps of MAX_NUM_IPRX_LISTENERS_INIT.
// The list is set back to the initial MAX_NUM_IPRX_LISTENERS_INIT if NumRxChannels == -1
//
DtStatus  DtaIpReAllocIpRxNumListeners(DtaIpPort*  pIpPort, Int  NumListeners)
{
    UserIpRxChannel** pIpRxChannels;
    UserIpRxChannel** pIpRxChannelsTmp;
    
    DtFastMutexAcquire(&pIpPort->m_IpRxListenersFastMutex);
    // Minimal listeners == MAX_NUM_IPRX_LISTEN_INIT
    if (NumListeners<=pIpPort->m_MaxNumListeners && NumListeners!=-1)
    {
        DtDbgOut(AVG, IP, "[%i] NumListeners:%i <= MAX:%i", pIpPort->m_IpPortIndex,
                                                NumListeners, pIpPort->m_MaxNumListeners);
        DtFastMutexRelease(&pIpPort->m_IpRxListenersFastMutex);
        return DT_STATUS_OK;
    }

    
    if (NumListeners == -1)
    {
        if (pIpPort->m_MaxNumListeners == MAX_NUM_IPRX_LISTENERS_INIT)
        {
            DtDbgOut(AVG, IP, "[%i] Reset. MaxNumListeners==MIN:%i", 
                                       pIpPort->m_IpPortIndex,pIpPort->m_MaxNumListeners);
            DtFastMutexRelease(&pIpPort->m_IpRxListenersFastMutex);
            return DT_STATUS_OK;
        }
        NumListeners = MAX_NUM_IPRX_LISTENERS_INIT;
    } else
        NumListeners = pIpPort->m_MaxNumListeners + MAX_NUM_IPRX_LISTENERS_INIT;

    DtDbgOut(AVG, IP, "[%i] Set new max. Prev:%i New:%i", pIpPort->m_IpPortIndex, 
                                                pIpPort->m_MaxNumListeners, NumListeners);
            

    if (NumListeners > MAX_NUM_IPRX_LISTENERS)
    {
        DtDbgOut(ERR, IP, "[%i] Max. number of listeners reached", 
                                                                  pIpPort->m_IpPortIndex);
        DtFastMutexRelease(&pIpPort->m_IpRxListenersFastMutex);
        return DT_STATUS_OUT_OF_RESOURCES;
    }

    pIpRxChannels= (UserIpRxChannel**)DtMemAllocPool(DtPoolNonPaged,
                                           NumListeners*sizeof(UserIpRxChannel*), IP_TAG);
    if (pIpRxChannels == NULL)
    {
        DtDbgOut(ERR, IP, "[%i] Error allocating m_pIpRxListeners for %i elements", 
                                                    pIpPort->m_IpPortIndex, NumListeners);
        // It's not an error if the new number is less then the current max. number of 
        // same listeners
        DtFastMutexRelease(&pIpPort->m_IpRxListenersFastMutex);
        if (NumListeners <= pIpPort->m_MaxNumListeners)
            return DT_STATUS_OK;
        return DT_STATUS_OUT_OF_MEMORY;
    }

    DtMutexAcquire(&pIpPort->m_IpRxListenersMutex, -1);

    pIpRxChannelsTmp = pIpPort->m_pIpRxListeners;
    pIpPort->m_pIpRxListeners = pIpRxChannels;
    pIpPort->m_MaxNumListeners = NumListeners;
    DtMutexRelease(&pIpPort->m_IpRxListenersMutex);
    
    DtMemFreePool(pIpRxChannelsTmp, IP_TAG);
    
    DtFastMutexRelease(&pIpPort->m_IpRxListenersFastMutex);
    return DT_STATUS_OK;
}
