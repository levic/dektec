//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Ip.c *#*#*#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
//
// Dta driver - IP functionality - Implementation of generic IP port functionality
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
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
DtStatus  DtaIpGetMacAddress(DtaIpPort*, UInt8* pAddress);
DtStatus  DtaIpGetSpeed(DtaIpPort*, Int* pSpeed, Int* pEnableForceSpeed);
DtStatus  DtaIpSetSpeed(DtaIpPort*, Int Speed, Int EnableForceSpeed);
DtStatus  DtaIpGetAdminStatus(DtaIpPort* pIpPort, Int* pAdminStatusUp);
DtStatus  DtaIpSetAdminStatus(DtaIpPort* pIpPort, Int AdminStatusUp);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Globals -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  g_NwDrvVersionMajor = -1;              // Version number of network driver
Int  g_NwDrvVersionMinor = -1;              // Version number of network driver
Int  g_NwDrvVersionMicro = -1;              // Version number of network driver
Int  g_NwDrvVersionBuild = -1;              // Version number of network driver


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpInitType1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIpInitType1(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIpPortType1*  pIpPortType1 = &pIpPort->m_IpPortType1;
    
    // Initialise list of DMA channels to handle DMA processing type independent
    // For now, only 1 channel enabled
    pIpPort->m_NumDmaChannels = 3;
    pIpPort->m_pDmaChannels[0] = &pIpPortType1->m_TxRt.m_DmaChannel;
    pIpPort->m_pDmaChannels[1] = &pIpPortType1->m_TxNrt.m_DmaChannel;
    pIpPort->m_pDmaChannels[2] = &pIpPortType1->m_Rx.m_DmaChannel;

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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCleanupType1Pre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpCleanupType1Pre(DtaIpPort* pIpPort)
{
    // Note: Driver verifier reports an error if we do finalize later (in DtaCleanup)
    
    DtaDmaFinalTransfer(&pIpPort->m_IpPortType1.m_TxRt.m_DmaChannel);
    DtaDmaFinalTransfer(&pIpPort->m_IpPortType1.m_TxNrt.m_DmaChannel);
    DtaDmaFinalTransfer(&pIpPort->m_IpPortType1.m_Rx.m_DmaChannel);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCleanupType1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpCleanupType1(DtaIpPort* pIpPort)
{    
    DtaIpTxCleanupType1(pIpPort);
    DtaIpRxCleanupType1(pIpPort);
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
    DtEventInit(&pIpUserChannels->m_IpRxPacketAvailEvent, TRUE);
    DtSpinLockInit(&pIpUserChannels->m_IpRxBrmSpinLock);
    pIpUserChannels->m_IpRxBrmSkipUpdate = FALSE;
    pIpUserChannels->m_IpRxPacketAvailableTrigger = FALSE;
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
// Remark: We only support type1 of the network port(DTA-160/2160) for now.
//
DtStatus  DtaIpInit(
    DtaDeviceData*  pDvcData,
    Int  PortIndex, 
    Int  IpPortIndex, 
    DtaIpDevice*  pIpDevice)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIpPort*  pIpPort = &pIpDevice->m_IpPorts[IpPortIndex];
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    
    if (DtPropertiesGetBool(pPropData, "CAP_IPPORTTYPE2", PortIndex))
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

    // Initialize register mappings
    pIpPort->m_pGenNwRegs = NULL;

    // Set offsets to network registers
    pIpPort->m_GenNwRegsOffset = DtPropertiesGetUInt16(pPropData, "NW_REGISTERS_GEN",
                                                                               PortIndex);
    // Check if no property error occurred
    Status = DtaPropertiesReportDriverErrors(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;
    
    Status = DtaPhyMacInit(pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising PhyMac");
        return Status;
    }

    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        Status = DtaIpInitType1(pIpPort);
    else {
        Status = DT_STATUS_NOT_SUPPORTED;
        DT_ASSERT(FALSE);
    }
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising Ip type specific");
        return Status;
    }

    DT_ASSERT(pIpPort->m_NumDmaChannels <= DTA_IP_MAX_DMA_CHANNELS);   

    Status = DtaIpRxInit(pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising IpRx");
        return Status;
    }

    Status = DtaIpTxInit(pIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, IP, "Error initialising IpTx");
        return Status;
    }

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
    DtaIpPort*  pIpPort = &pIpDevice->m_IpPorts[IpPortIndex];
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        DtaIpCleanupType1Pre(pIpPort);

}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//
void  DtaIpCleanup(DtaDeviceData*  pDvcData,
    Int  PortIndex, 
    Int  IpPortIndex, 
    DtaIpDevice*  pIpDevice)
{
    
    DtaIpPort*  pIpPort = &pIpDevice->m_IpPorts[IpPortIndex];
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
        DtaIpCleanupType1(pIpPort);
    
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

    for (i = 0; i <pIpPort->m_NumDmaChannels; i++)
    {
        Status = DtaDmaInitChPowerup(pIpPort->m_pDmaChannels[i]);
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
    {
        Status = DtaIpPowerupType1(pIpPort);
        
    } else
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

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
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

     pIpPort = &pDvcData->m_IpDevice.m_IpPorts[IpPortIndex];

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
    pIpPort = &pDvcData->m_IpDevice.m_IpPorts[pIpNwCmdInput->m_IpPortIndex];

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
            DtaIpSetAdminStatus(&pDvcData->m_IpDevice.m_IpPorts[i], 1);
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

    if (pIpPort->m_PortType == DTA_IPPORT_TYPE1)
    {
        Status = DtaIpPowerdownType1Pre(pIpPort);
        
    } else
        DT_ASSERT(FALSE);

    Status = DtaIpRxPowerdownPre(pIpPort);
    Status = DtaIpTxPowerdownPre(pIpPort);
    
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpDevicePowerdown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaIpDevicePowerdown(DtaIpDevice* pIpDevice)
{
   DtaIpTxDevicePowerDown(pIpDevice);
   DtaIpRxDevicePowerDown(pIpDevice);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpPowerdown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called everytime the hardware is power downed for each port.
//
void  DtaIpPowerdown(DtaIpDevice* pIpDevice, DtaIpPort* pIpPort)
{
    Int  LastIpPort = pIpPort->m_pDvcData->m_NumIpPorts - 1;

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

    if (ChannelType == DTA_SH_CHANTYPE_IPRX)
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpSharedBufferReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Notify IP channels that the shared buffer is created for this channel
DtStatus  DtaIpSharedBufferReady(
    DtaIpDevice* pIpDevice, 
    Int ChannelIndex, 
    Int ChannelType)
{   
    UserIpRxChannel* pIpRxChannel = NULL;
    UserIpTxChannel* pIpTxChannel = NULL;

    if (ChannelType == DTA_SH_CHANTYPE_IPTX)
    {
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
    }
     
    if (ChannelType == DTA_SH_CHANTYPE_IPRX)
    { 
        // Buffer contents:
        // |======|======================|=========|====================|
        // |Header|FIFO                  |WrapArea | Rtp/FEC scratchpad
        //                               |Used by driver only
        pIpRxChannel = DtaIpRxUserChGet(&pIpDevice->m_IpUserChannels, ChannelIndex);
        if (pIpRxChannel != NULL)
        {        
            pIpRxChannel->m_pBufferHeader = (IpRxBufferHeader*)
                                                   pIpRxChannel->m_SharedBuffer.m_pBuffer;
            pIpRxChannel->m_pFifo = pIpRxChannel->m_SharedBuffer.m_pBuffer + 
                                                                 sizeof(IpRxBufferHeader);

            pIpRxChannel->m_FifoSize = pIpRxChannel->m_pBufferHeader->m_BufSize - 
                                               DTA_IPRX_BUFWRAPSIZE - DTA_IPRX_BUFRTPSIZE;

            pIpRxChannel->m_pWrapArea = pIpRxChannel->m_pFifo + pIpRxChannel->m_FifoSize;

            pIpRxChannel->m_pRtpBuffer = pIpRxChannel->m_pWrapArea + DTA_IPRX_BUFWRAPSIZE;
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
    
    pIpPort = &pPhyMac->m_pDvcData->m_IpDevice.m_IpPorts[pPhyMac->m_IpPortIndex];
    
    DT_ASSERT(pIpPort->m_PortType == DTA_IPPORT_TYPE1);

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
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpGetNumRxFifoOverflow -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt64  DtaIpGetNumRxFifoOverflow(DtaIpPort* pIpPort)
{
    
    if (!pIpPort->m_PhyMac.m_UsesAlteraMac)
        return pIpPort->m_NumRxFifoOverflow + 
                              DtaMacCMissedFramesCounterGet(pIpPort->m_PhyMac.m_pMacRegs);
    DT_ASSERT(FALSE);
    // TODO
    return pIpPort->m_NumRxFifoOverflow;
}

