//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaNw.c *#*#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// DtaNw driver - Interface for the DtaNw common driver, used by the IAL.
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
#include <DtaNwIncludes.h>


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Forward declarations
static DtStatus  DtaNwGetDtaDriverVersionIoCtrl(DtaNwDeviceData*  pDvcData,
                                                   Int* pMajor, Int* pMinor, Int* pMicro);
static DtStatus  DtaNwGetDeviceInfoIoCtrl(DtaNwDeviceData* pDvcData,
                                      UInt16* pDeviceId, UInt16* pVendorId,
                                      UInt16* pSubVendorId, UInt16* pSubSystemId,
                                      Int* pBusNumber, Int* pSlotNumber, Int* pTypeNumber,
                                      Int* pHardwareRevision, Int* pFirmwareVersion,
                                      Int* pFirmwareVariant, UInt64* pSerial);
static DtStatus  DtaNwGetPhySpeedIoCtrl(DtaNwDeviceData* pDvcData, UInt* pSpeed);
static DtStatus  DtaNwSetPhySpeedIoCtrl(DtaNwDeviceData* pDvcData, UInt  Speed);
static DtStatus  DtaNwGetMacAddressPermIoCtrl(DtaNwDeviceData* pDvcData, 
                                                                      UInt8* pMacAddress);
static DtStatus  DtaNwGetPacketFilterIoCtrl(DtaNwDeviceData* pDvcData, 
                                                                     UInt* pPacketFilter);
static DtStatus  DtaNwSetPacketFilterIoCtrl(DtaNwDeviceData* pDvcData, UInt PacketFilter);
static DtStatus  DtaNwGetStatisticCounterIoCtrl(DtaNwDeviceData* pDvcData, 
                                                        UInt StatisticId, UInt64* pValue);
static DtStatus  DtaNwRegisterCallbackIoCtrl(DtaNwDeviceData* pDvcData, 
                                                           LinkStatFunc LinkStatCallback);
static DtStatus  DtaNwSetMacAddressIoCtrl(DtaNwDeviceData* pDvcData, UInt8* pMacAddress);
static DtStatus  DtaNwGetMaxMulticastListIoCtrl(DtaNwDeviceData* pDvcData, 
                                                                 UInt* pMaxMulticastList);
static DtStatus  DtaNwUnregisterCallbackIoCtrl(DtaNwDeviceData* pDvcData);
static DtStatus  DtaNwResetNwStatusIoCtrl(DtaNwDeviceData* pDvcData);
static DtStatus  DtaNwSetDriverVersionIoCtrl(DtaNwDeviceData* pDvcData);
static DtStatus  DtaNwGetSharedBuffersIoCtrl(DtaNwDeviceData* pDvcData);
static DtStatus  DtaNwSetMulticastListIoCtrl(DtaNwDeviceData* pDvcData, UInt NumItems,
                                                                    void* pMulticastList);
static DtStatus  DtaNwInitSharedBuffers(DtaNwDeviceData* pDvcData);
void  DtaNwLinkStatusCallback(UInt LinkStatus, void* pContext);
static DtStatus  DtaNwCreateThreads(DtaNwDeviceData* pDvcData);
static void  DtaNwPowerdown(DtaNwDeviceData* pDvcData);
static DtStatus  DtaNwPowerup(DtaNwDeviceData* pDvcData);
static DtStatus  DtaNwCheckDtaDriverVersion(DtaNwDeviceData* pDvcData);



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwDeviceDataInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNwDeviceDataInit(DtaNwDeviceData* pDvcData)
{
    pDvcData->m_PromiscuousModeSet = FALSE;
    pDvcData->m_PhySpeedSet = DTA_PHY_SPEED_AUTO_DETECT;
    pDvcData->m_MacAddressOverride = FALSE;
    pDvcData->m_Support8021P_Priority = TRUE;
    pDvcData->m_Supports8021Q_Vlan = TRUE;
    pDvcData->m_NwRcvNoBuffer = 0;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwDriverInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL and initializes the common (host independent) part
// of the driver.
//
DtStatus  DtaNwDriverInit(DtEvtLog* pEvtObject)
{
    DtStatus  Status = DT_STATUS_OK;
    DtString  DtStrVersion;
    DtStringChar  DtStrVersionBuffer[32];
    DT_STRING_DECL(DtStrDot, ".");

    // Connect DtStrVersionBuffer to DtStrVersion
    DT_STRING_INIT(DtStrVersion, DtStrVersionBuffer, 32);

    // Create driver version string
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTANW_VERSION_MAJOR);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTANW_VERSION_MINOR);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTANW_VERSION_MICRO);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTANW_VERSION_BUILD);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Finally report the event
    DtEvtLogReport(pEvtObject, DTANW_LOG_DRIVER_LOADED, &DtStrVersion, NULL, NULL);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwDriverExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the driver is unloaded. This function should
// undo everything done by DtaNwDriverInit.
//
void  DtaNwDriverExit()
{
    DtDbgOut(MAX, DTANW, "Start");
    
    DtDbgOut(MAX, DTANW, "Exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwDeviceInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the device is found in the system by the Dta
// driver. 
//
DtStatus  DtaNwDeviceInit(DtaNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    DtDbgOut(MAX, DTANW, "Start");
    
    // Get Device Information
    Status = DtaNwGetDeviceInfoIoCtrl(pDvcData, NULL, NULL, NULL, NULL, 
                                         &pDvcData->m_BusNumber, &pDvcData->m_SlotNumber,
                                         &pDvcData->m_TypeNumber, &pDvcData->m_HwRevision,
                                         &pDvcData->m_FwVersion, NULL, NULL);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTANW, "Error getting device info. Error: 0x%x", Status);
        return Status;
    }

    // ToDo: Create property if more devices have different header versions
    if (pDvcData->m_TypeNumber == 2162)
        pDvcData->m_HeaderVersion = 3;
    else
        pDvcData->m_HeaderVersion = 1;

    // Get Dta driver version
    Status = DtaNwCheckDtaDriverVersion(pDvcData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTANW, "Error checking Dta driver version: 0x%x", Status);
        return Status;
    }

    // Get permanent address
    Status = DtaNwGetMacAddressPerm(pDvcData, pDvcData->m_MacAddressPerm);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTANW, "Error getting permanent MAC address. Error: 0x%x", Status);
        return Status;
    }

    // Copy permanent address to current address
    if (!pDvcData->m_MacAddressOverride)
        DtMemCopy(pDvcData->m_MacAddressCur, pDvcData->m_MacAddressPerm,
                                                      sizeof(pDvcData->m_MacAddressPerm));
    
    // Update MAC address in Dta driver always, because during stop and start of network
    // driver, the MAC address can be changed without resetting the Dta MAC address
    Status = DtaNwSetMacAddress(pDvcData, pDvcData->m_MacAddressCur);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTANW, "Error setting current MAC address. Error: 0x%x", Status);
        return Status;
    }
    
    DtDbgOut(MAX, DTANW, "Exit");
    
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwDeviceOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the interface goes to the up state
// The function should undo everything done by DtaNwDeviceOpen.
//
DtStatus  DtaNwDeviceOpen(DtaNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    DtDbgOut(MAX, DTANW, "Start");
    
    pDvcData->m_LinkStatus = DTANW_LINKSTAT_DOWN;
    
    // Register callbacks
    Status = DtaNwRegisterCallbackIoCtrl(pDvcData, DtaNwLinkStatusCallback);
    
    // Reset status in Dta driver
    if (DT_SUCCESS(Status))
        Status = DtaNwResetNwStatusIoCtrl(pDvcData);

    // Set Network driver version
    if (DT_SUCCESS(Status))
        Status = DtaNwSetDriverVersionIoCtrl(pDvcData);

    if (DT_SUCCESS(Status))
        Status = DtaNwGetMaxMulticastList(pDvcData, &pDvcData->m_MaxNumMulticastItems);

    // Initialise shared buffers for Network Tx/Rx
    if (DT_SUCCESS(Status))
        Status = DtaNwInitSharedBuffers(pDvcData);
    
    // Update Speed/Duplex
    if (DT_SUCCESS(Status))
        Status = DtaNwSetPhySpeed(pDvcData, pDvcData->m_PhySpeedSet);
    
    // Create worker threads
    if (DT_SUCCESS(Status))
        Status = DtaNwCreateThreads(pDvcData);

    if (DT_SUCCESS(Status))
        Status = DtaNwPowerup(pDvcData);

    // Mark device as opened also when an error occures. This is not
    // a problem for the close.
    pDvcData->m_DeviceOpened = TRUE;
    
    DtDbgOut(MAX, DTANW, "Exit");
    
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwDeviceClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL when the interface goes to the down state
// The function should undo everything done by DtaNwDeviceOpen.
//
void  DtaNwDeviceClose(DtaNwDeviceData* pDvcData)
{
    DtDbgOut(MAX, DTANW, "Start");
    
    // Unregister callbacks
    DtaNwUnregisterCallbackIoCtrl(pDvcData);

    DtaNwPowerdown(pDvcData);
    
    pDvcData->m_DeviceOpened = FALSE;

    DtDbgOut(MAX, DTANW, "Exit");
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwDeviceExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the device is being disconnected. This
// function should undo everything done by DtaNwDeviceInit.
//
void  DtaNwDeviceExit(DtaNwDeviceData* pDvcData)
{
    DtDbgOut(MAX, DTANW, "Start");

    DtDbgOut(MAX, DTANW, "Exit");
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwTxGetPointerNewPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is used to get a pointer to the Tx buffer with size: PacketSize.
// If not enough space available, an error is returned. The WriteOffset parameter is used
// by the DtaNwTransmitPacket function.
//
DtStatus  DtaNwTxGetPointerNewPacket(
    DtaNwDeviceData*  pDvcData,
    UInt  PacketSize, 
    UInt8**  pPacket, 
    UInt*  pWriteOffset)
{
    UInt  FreeSpaceUntilEnd;
    UInt  FreeSpaceFromBegin;
    UInt  ReadOffset;
    UInt  TotalLength;
    DtaIpNwSharedBufInfo*  pSharedInfo = pDvcData->m_pTxSharedBufInfo;
    
    TotalLength = PacketSize + sizeof(DtaDmaTxHeader);
    if (pDvcData->m_AlignedPayload)
        TotalLength+= 2;

    ReadOffset = pSharedInfo->m_ReadOffset;
    *pWriteOffset = pSharedInfo->m_WriteOffset;

    // Calculate free space
    if (*pWriteOffset < ReadOffset) 
    {
        FreeSpaceFromBegin = 0;
        FreeSpaceUntilEnd = ReadOffset - *pWriteOffset - 1;
    } else {
        FreeSpaceUntilEnd = pDvcData->m_TxBufSize - *pWriteOffset - 1;
        if (ReadOffset == 0)
            FreeSpaceFromBegin = 0;
        else
            FreeSpaceFromBegin = ReadOffset - 1;
    }

    if (FreeSpaceUntilEnd >= TotalLength)
        *pPacket = (pDvcData->m_pTxBuffer + *pWriteOffset);
    else if (FreeSpaceFromBegin >= TotalLength) 
    {
        // Mark dummy bytes until end.
        pDvcData->m_pTxBuffer[pSharedInfo->m_WriteOffset] = '*';

        *pWriteOffset = 0;
        *pPacket = pDvcData->m_pTxBuffer;
    } else {
        // Not enough room, skip pakket
        DtDbgOut(MAX, DTANW, "Shared buffer full. Packet discarded");
        return DT_STATUS_BUFFER_OVERFLOW;
    }

    *pPacket += sizeof(DtaDmaTxHeader);
    if (pDvcData->m_AlignedPayload)
        *pPacket += 2;
    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwTransmitPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Pre: Packet data is already copied by the calling function to the buffer retrieved
// by DtaNwTxGetPointerNewPacket
//
DtStatus  DtaNwTransmitPacket(
    DtaNwDeviceData*  pDvcData,
    UInt  PacketSize,
    UInt  WriteOffset,
    Bool  EnIpChecksumGen,
    Bool  EnUdpChecksumGen,
    Bool  EnTcpChecksumGen)
{
    UInt  TotalLength;
    UInt8*  pDst;
    
    DtaIpNwSharedBufInfo*  pSharedInfo = pDvcData->m_pTxSharedBufInfo;
    DtaDmaTxHeader*  pDmaTxHeader;
    
    if (pDvcData->m_AlignedPayload)
        PacketSize += 2;

    TotalLength = PacketSize + sizeof(DtaDmaTxHeader);
    
    pDst = pDvcData->m_pTxBuffer + WriteOffset;
    
    // Add DMA Tx header
    pDmaTxHeader = (DtaDmaTxHeader*)pDst;
    DtMemZero(pDmaTxHeader, sizeof(DtaDmaTxHeader));
    pDmaTxHeader->m_Tag = 0x445441A0;
    if (pDvcData->m_HeaderVersion == 3) 
    {
        pDmaTxHeader->m_Version = 3;
        pDmaTxHeader->m_TxHeaderV3.m_ChecksumCmd.m_NoCalcIPChkSum = (EnIpChecksumGen?0:1);
        pDmaTxHeader->m_TxHeaderV3.m_ChecksumCmd.m_NoCalcUDPChkSum =
                                                                   (EnUdpChecksumGen?0:1);
        pDmaTxHeader->m_TxHeaderV3.m_ChecksumCmd.m_NoCalcTCPChkSum =
                                                                   (EnTcpChecksumGen?0:1);
    } else
        pDmaTxHeader->m_Version = 1;

    pDmaTxHeader->m_Length = sizeof(DtaDmaTxHeader);
    pDmaTxHeader->m_TxHeaderGen.m_TransmitControl.m_PacketLength = PacketSize;
    pDst+= sizeof(DtaDmaTxHeader);

    // Add dummy bytes
    if (pDvcData->m_AlignedPayload)
    {
        pDst++[0] = 0;
        pDst++[0] = 0;
    }

    // Packet data is already copied by calling function
    
    // Align packets on DWORD's
    if (PacketSize%4 != 0)
        TotalLength += 4 - (PacketSize % 4);

    WriteOffset+= TotalLength;
    if (WriteOffset >= pDvcData->m_TxBufSize)
        WriteOffset = 0;
    
    pSharedInfo->m_WriteOffset = WriteOffset;

    // Trigger Network driver
    DtAtomicIncrement(&pSharedInfo->m_DataAvailableCnt);
    DtEventSet(&pSharedInfo->m_DataAvailableEvent);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetLinkStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNwGetLinkStatus(DtaNwDeviceData* pDvcData, UInt* pPhyLink)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt Speed;
    *pPhyLink = DTA_PHY_LINK_DOWN;
    Status = DtaNwGetPhySpeedIoCtrl(pDvcData, &Speed);
    if (!DT_SUCCESS(Status))
        return Status;
    if (Speed != DTA_PHY_SPEED_NO_LINK)
        *pPhyLink = DTA_PHY_LINK_UP;
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetMacAddressPerm -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNwGetMacAddressPerm(DtaNwDeviceData* pDvcData, UInt8* pMacAddress)
{
    return DtaNwGetMacAddressPermIoCtrl(pDvcData, pMacAddress);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetPacketFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNwGetPacketFilter(DtaNwDeviceData* pDvcData, UInt* pPacketFilter)
{
    return DtaNwGetPacketFilterIoCtrl(pDvcData, pPacketFilter);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwSetPacketFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNwSetPacketFilter(DtaNwDeviceData* pDvcData, UInt PacketFilter)
{
    if (pDvcData->m_PromiscuousModeSet)
        PacketFilter |= DTA_MAC_FLT_PROMISCUOUS;
    return DtaNwSetPacketFilterIoCtrl(pDvcData, PacketFilter);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetStatisticCounter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNwGetStatisticCounter(
    DtaNwDeviceData*  pDvcData,
    UInt  StatisticId,
    UInt64*  pValue)
{
    DtStatus  Status = DtaNwGetStatisticCounterIoCtrl(pDvcData, StatisticId, pValue);
    *pValue+= pDvcData->m_NwRcvNoBuffer;
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetMaxMulticastList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNwGetMaxMulticastList(DtaNwDeviceData* pDvcData, UInt* pMaxMulticastList)
{
    return DtaNwGetMaxMulticastListIoCtrl(pDvcData, pMaxMulticastList);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwSetMulticastList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNwSetMulticastList(
    DtaNwDeviceData*  pDvcData, 
    UInt  NumItems, 
    void*  pMulticastList)
{
    return DtaNwSetMulticastListIoCtrl(pDvcData, NumItems, pMulticastList);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetMulticastList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNwGetMulticastList(DtaNwDeviceData* pDvcData)
{
    DT_ASSERT(FALSE);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNwGetSpeed(DtaNwDeviceData* pDvcData, UInt* pSpeed)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  Speed = DTA_PHY_SPEED_NOT_SET;
    Status = DtaNwGetPhySpeedIoCtrl(pDvcData, &Speed);
    switch (Speed)
    {
    case DTA_PHY_SPEED_10_HALF:
    case DTA_PHY_SPEED_10_FULL:
        *pSpeed = 10000000; 
        break;
    case DTA_PHY_SPEED_100_HALF:
    case DTA_PHY_SPEED_100_FULL:
        *pSpeed = 100000000; 
        break;
    case DTA_PHY_SPEED_1000_MASTER:
    case DTA_PHY_SPEED_1000_SLAVE:
        *pSpeed = 1000000000;
        break;
    default: 
        *pSpeed = 0; 
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetPhySpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNwGetPhySpeed(DtaNwDeviceData* pDvcData, UInt* pPhySpeed)
{
    return DtaNwGetPhySpeedIoCtrl(pDvcData, pPhySpeed);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwSetPhySpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNwSetPhySpeed(DtaNwDeviceData* pDvcData, UInt PhySpeed)
{
    return DtaNwSetPhySpeedIoCtrl(pDvcData, PhySpeed);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwSetMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNwSetMacAddress(DtaNwDeviceData* pDvcData, UInt8* pMacAddress)
{
    return DtaNwSetMacAddressIoCtrl(pDvcData, pMacAddress);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwInitSharedBuffers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwInitSharedBuffers(DtaNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    Status = DtaNwGetSharedBuffersIoCtrl(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;
    
    pDvcData->m_pRxBuffer = pDvcData->m_pRxBuffer;
    pDvcData->m_pTxBuffer = pDvcData->m_pTxBuffer;
    pDvcData->m_RxBufSize = pDvcData->m_RxBufSize - sizeof(DtaIpNwSharedBufInfo);
    pDvcData->m_TxBufSize = pDvcData->m_TxBufSize - sizeof(DtaIpNwSharedBufInfo);
    pDvcData->m_pRxSharedBufInfo = (DtaIpNwSharedBufInfo*)(pDvcData->m_pRxBuffer + 
                                                                   pDvcData->m_RxBufSize);
    pDvcData->m_pTxSharedBufInfo = (DtaIpNwSharedBufInfo*)(pDvcData->m_pTxBuffer +
                                                                   pDvcData->m_TxBufSize);
    
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwRxThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaNwRxThread(DtThread* pThread, void* pContext)
{
    DtaNwDeviceData* pDvcData = pContext;
    DtaIpNwSharedBufInfo*  pSharedInfo = pDvcData->m_pRxSharedBufInfo;
    Bool  StopThread = FALSE;
    DtStatus  Status = DT_STATUS_OK;
    UInt8*  pPacket;
    UInt  PacketSize;
    UInt  DataLength;
    DtaDmaRxHeader*  pDmaRxHeader = NULL;
    UInt  ReadOffset;
    Bool  IPv4 = FALSE;
    Bool  IPv6 = FALSE;
    Bool  IpChecksumCorrect = FALSE;
    Bool  UdpChecksumCorrect = FALSE;
    Bool  TcpChecksumCorrect = FALSE;
    
    DtDbgOut(MAX, DTANW, "Start");
    
    while (!StopThread)
    {
        // Wait for DMA done or stop
        Status = DtThreadWaitForStopOrEvent(pThread, &pSharedInfo->m_DataAvailableEvent);
        if (Status == DT_STATUS_CANCELLED)
        {
            StopThread = TRUE;
            continue;
        }

        // Get packet data
        while (pSharedInfo->m_ReadOffset!=pSharedInfo->m_WriteOffset && 
                                                              !DtThreadShouldStop(pThread))
        {
            pPacket = pDvcData->m_pRxBuffer + pSharedInfo->m_ReadOffset;
            if (pPacket[0] == '*')
            {
                // New packet is at start of buffer
                pPacket = pDvcData->m_pRxBuffer;
                pSharedInfo->m_ReadOffset = 0;
            }

            pDmaRxHeader = (DtaDmaRxHeader*)pPacket;
            DT_ASSERT(pDmaRxHeader->m_Tag==0x445441A0 && 
                                          pDmaRxHeader->m_Length==sizeof(DtaDmaTxHeader));

            if (pDmaRxHeader->m_Tag!=0x445441A0 || pDmaRxHeader->m_Length!=sizeof(DtaDmaTxHeader) ||
                pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength<12)
            {
                // Fatal error: clear RX buffer and log error.
                pSharedInfo->m_ReadOffset = pSharedInfo->m_WriteOffset;
                DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, DTANW_LOG_FATAL_ERROR, NULL, NULL,
                                                                                    NULL);

                continue;
            }
            //Calculate PacketLength: DmaRxHeader is not included in FrameLength
            DataLength = pDmaRxHeader->m_RxHeaderGen.m_ReceiveStatus.m_FrameLength;
            PacketSize = DataLength;
            if (!pDvcData->m_FCSEnabled)
                PacketSize -= 4; //Strip FCS

            pPacket += sizeof(DtaDmaRxHeader);

            // If Payload is aligned, 2 extra dummy bytes are between header and payload.
            if (pDvcData->m_AlignedPayload)
            {
                pPacket += 2;
                PacketSize -= 2; 
            }

            if (pDmaRxHeader->m_Version >= 3)
            {
                IPv4 = pDmaRxHeader->m_RxHeaderV3.m_FrameStatus.m_IPv4Frame == 1;
                IPv6 = pDmaRxHeader->m_RxHeaderV3.m_FrameStatus.m_IPv6Frame == 1;
                IpChecksumCorrect = (IPv4 || IPv6) && 
                              pDmaRxHeader->m_RxHeaderV3.m_FrameStatus.m_IPChksumValid==1;
                UdpChecksumCorrect = pDmaRxHeader->m_RxHeaderV3.m_FrameStatus.m_UDPFrame 
                          && pDmaRxHeader->m_RxHeaderV3.m_FrameStatus.m_UDPChksumValid==1;
                TcpChecksumCorrect = pDmaRxHeader->m_RxHeaderV3.m_FrameStatus.m_TCPFrame 
                          && pDmaRxHeader->m_RxHeaderV3.m_FrameStatus.m_TCPChksumValid==1;

                // Check firmware
                DT_ASSERT(pDmaRxHeader->m_RxHeaderV3.m_FrameStatus.m_IPChksumValid==1 || 
                                                                            IPv4 || IPv6);
                DT_ASSERT(pDmaRxHeader->m_RxHeaderV3.m_FrameStatus.m_UDPChksumValid==1 ||
                                     pDmaRxHeader->m_RxHeaderV3.m_FrameStatus.m_UDPFrame);
                DT_ASSERT(pDmaRxHeader->m_RxHeaderV3.m_FrameStatus.m_TCPChksumValid==1 ||
                                     pDmaRxHeader->m_RxHeaderV3.m_FrameStatus.m_TCPFrame);
            }

            DtaNwEvtNewPacketRxCallback(pDvcData, pPacket, PacketSize, IPv4, IPv6,
                        pDmaRxHeader->m_RxHeaderGen.m_ChecksumStatus.m_IPChksumValid==0,
                        pDmaRxHeader->m_RxHeaderGen.m_ChecksumStatus.m_UDPChksumValid==0,
                        pDmaRxHeader->m_RxHeaderGen.m_ChecksumStatus.m_TCPChksumValid==0,
                        IpChecksumCorrect, UdpChecksumCorrect, TcpChecksumCorrect);
            if ((DataLength % 4) != 0)
                DataLength += (4 - (DataLength % 4));
            
            ReadOffset = pSharedInfo->m_ReadOffset + DataLength + sizeof(DtaDmaRxHeader);
            if (ReadOffset >= pDvcData->m_RxBufSize)
                ReadOffset = 0;

            pSharedInfo->m_ReadOffset = ReadOffset;
            DtAtomicDecrement(&pSharedInfo->m_DataAvailableCnt);
        }
        if (DtThreadShouldStop(pThread))
            StopThread = TRUE;
    }
    
    // We don't have to wait until the thread received a stop command. We only got here in
    // that case
    DtDbgOut(MAX, DTANW, "Exit");
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwCreateThreads -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwCreateThreads(DtaNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    
    Status = DtThreadInit(&pDvcData->m_RxThread, DtaNwRxThread, pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNwPowerup(DtaNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Status = DtThreadStart(&pDvcData->m_RxThread);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwPowerdown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtaNwPowerdown(DtaNwDeviceData* pDvcData)
{
    DtDbgOut(MIN, DTANW, "Stopping Rx thread");
    DtThreadStop(&pDvcData->m_RxThread);
    DtDbgOut(MIN, DTANW, "Rx thread stopped");
    
}


//=+=+=+=+=+=+=+=+=+=+=+ Callback functions for direct Dta calls +=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwLinkStatusCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaNwLinkStatusCallback(UInt PhyLinkStatus, void* pContext)
{
    DtaNwDeviceData*  pDvcData = pContext;
    UInt  MessageId;
    DtString  DtStrMacAddress;
    DtStringChar  DtStrMacAddressBuffer[20];
    
    // Connect DtStrVersionBuffer to DtStrVersion
    DT_STRING_INIT(DtStrMacAddress, DtStrMacAddressBuffer, 20);

    DtDbgOut(MAX, DTANW, "Start");

    switch (PhyLinkStatus)
    {
    case DTA_PHY_LINK_UP:
        pDvcData->m_LinkStatus = DTANW_LINKSTAT_UP;
        MessageId = DTANW_LOG_LINK_CONNECTED;
        break;
    default:
        pDvcData->m_LinkStatus = DTANW_LINKSTAT_DOWN;
        MessageId = DTANW_LOG_LINK_DISCONNECTED;
    }

    // Report Network stack of change
    DtaNwEvtLinkStatusCallback(pDvcData);

    // Report the link status event
    DtMacAddress2DtString(pDvcData->m_MacAddressCur, &DtStrMacAddress);
    DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, MessageId, &DtStrMacAddress, NULL,
                                                                                    NULL);


    DtDbgOut(MAX, DTANW, "Exit");
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helper functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwCheckDtaDriverVersion -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNwCheckDtaDriverVersion(
    DtaNwDeviceData*  pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  DtaMajor;
    Int  DtaMinor;
    Int  DtaMicro;
    Bool  VersionError = FALSE;
    Status = DtaNwGetDtaDriverVersionIoCtrl(pDvcData, &DtaMajor, &DtaMinor,
                                                                            &DtaMicro);
    if (DT_SUCCESS(Status))
    {
        DtString  DtStrVersion;
        DtStringChar  DtStrVersionBuffer[32];
        DT_STRING_DECL(DtStrDot, ".");
    
        // Check driver version
        if (!VersionError && DtaMajor<DTA_MIN_MAJOR) VersionError = TRUE;
        if (!VersionError && DtaMajor>DTA_MIN_MAJOR) return DT_STATUS_OK;
        if (!VersionError && DtaMinor<DTA_MIN_MINOR) VersionError = TRUE;
        if (!VersionError && DtaMinor>DTA_MIN_MINOR) return DT_STATUS_OK;
        if (!VersionError && DtaMicro>=DTA_MIN_MICRO) return DT_STATUS_OK;

        DT_ASSERT(VersionError);
        
        // Report error message
        Status = DT_STATUS_VERSION_MISMATCH;

        // Connect DtStrErrMsgBuffer to DtStrErrMsg
        DT_STRING_INIT(DtStrVersion, DtStrVersionBuffer, 32);

        // Build version
        
        DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTA_MIN_MAJOR);
        DtStringAppendDtString(&DtStrVersion, &DtStrDot);
        DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTA_MIN_MINOR);
        DtStringAppendDtString(&DtStrVersion, &DtStrDot);
        DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTA_MIN_MICRO);
    
        DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, DTANW_LOG_VERSION_MISMATCH,
                                                               &DtStrVersion, NULL, NULL);
    }
    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetStatisticCounterIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwGetStatisticCounterIoCtrl(
    DtaNwDeviceData*  pDvcData,
    UInt  StatisticId,
    UInt64*  pValue)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlPhyMacCmdInput  IoPhyMacCmdInput;
    DtaIoctlPhyMacCmdOutput  IoPhyMacCmdOutput;
    UInt  SizeIn = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data) + 
                                                    sizeof(DtaIoctlPhyMacGetCounterInput);
    UInt  SizeOut = sizeof(DtaIoctlPhyMacGetCounterOutput);
    *pValue = 0;
    IoPhyMacCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoPhyMacCmdInput.m_Cmd = DTA_PHYMAC_CMD_GET_COUNTER;
    IoPhyMacCmdInput.m_Data.m_GetCounter.m_CounterId = StatisticId;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_PHYMAC_CMD,
                          SizeIn, &IoPhyMacCmdInput, SizeOut, &IoPhyMacCmdOutput, NULL);
    if (!DT_SUCCESS(Status))
        return Status;
    *pValue = IoPhyMacCmdOutput.m_Data.m_GetCounter.m_Value;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetMaxMulticastListIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwGetMaxMulticastListIoCtrl(
    DtaNwDeviceData*  pDvcData,
    UInt*  pMaxMulticastList)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlPhyMacCmdInput  IoPhyMacCmdInput;
    DtaIoctlPhyMacCmdOutput  IoPhyMacCmdOutput;
    UInt  SizeIn = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data);
    UInt  SizeOut = sizeof(DtaIoctlPhyMacGetMaxMulticastListOutput);
    IoPhyMacCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoPhyMacCmdInput.m_Cmd = DTA_PHYMAC_CMD_GET_MAX_MULTICAST_LIST;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_PHYMAC_CMD,
                          SizeIn, &IoPhyMacCmdInput, SizeOut, &IoPhyMacCmdOutput, NULL);
    if (!DT_SUCCESS(Status))
        return Status;
    *pMaxMulticastList = IoPhyMacCmdOutput.m_Data.m_GetMaxMulticastList.m_MaxListSize;
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwSetMacAddressIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNwSetMacAddressIoCtrl(DtaNwDeviceData* pDvcData, UInt8* pMacAddress)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlPhyMacCmdInput  IoPhyMacCmdInput;
    UInt  SizeIn = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data) + 
                                                 sizeof(DtaIoctlPhyMacSetMacAddressInput);
    IoPhyMacCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoPhyMacCmdInput.m_Cmd = DTA_PHYMAC_CMD_SET_MACADDRESS;

    DtMemCopy(IoPhyMacCmdInput.m_Data.m_SetMacAddress.m_Address, pMacAddress, 
                               sizeof(IoPhyMacCmdInput.m_Data.m_SetMacAddress.m_Address));
    
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_PHYMAC_CMD,
                          SizeIn, &IoPhyMacCmdInput, 0, NULL, NULL);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetMacAddressPermIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNwGetMacAddressPermIoCtrl(
    DtaNwDeviceData*  pDvcData,
    UInt8*  pMacAddress)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlPhyMacCmdInput  IoPhyMacCmdInput;
    DtaIoctlPhyMacCmdOutput  IoPhyMacCmdOutput;
    UInt  SizeIn = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data);
    UInt  SizeOut = sizeof(DtaIoctlPhyMacGetMacAddressPermOutput);
    
    IoPhyMacCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoPhyMacCmdInput.m_Cmd = DTA_PHYMAC_CMD_GET_MACADDRESS_PERM;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_PHYMAC_CMD,
                          SizeIn, &IoPhyMacCmdInput, 
                          SizeOut, &IoPhyMacCmdOutput, NULL);
    if (!DT_SUCCESS(Status))
        return Status;

    DtMemCopy(pMacAddress, IoPhyMacCmdOutput.m_Data.m_GetMacAddressPerm.m_Address, 
                          sizeof(IoPhyMacCmdOutput.m_Data.m_GetMacAddressPerm.m_Address));
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetPacketFilterIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwGetPacketFilterIoCtrl(
    DtaNwDeviceData*  pDvcData,
    UInt*  pPacketFilter)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlPhyMacCmdInput  IoPhyMacCmdInput;
    DtaIoctlPhyMacCmdOutput  IoPhyMacCmdOutput;
    UInt  SizeIn = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data);
    UInt  SizeOut = sizeof(DtaIoctlPhyMacGetPacketFilterOutput);
    *pPacketFilter = 0;
    IoPhyMacCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoPhyMacCmdInput.m_Cmd = DTA_PHYMAC_CMD_GET_PACKET_FILTER;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_PHYMAC_CMD,
                          SizeIn, &IoPhyMacCmdInput, SizeOut, &IoPhyMacCmdOutput, NULL);
    if (!DT_SUCCESS(Status))
        return Status;
    *pPacketFilter = IoPhyMacCmdOutput.m_Data.m_GetPacketFilter.m_PacketFilter;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwSetPacketFilterIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwSetPacketFilterIoCtrl(DtaNwDeviceData* pDvcData, UInt PacketFilter)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlPhyMacCmdInput  IoPhyMacCmdInput;
    UInt  SizeIn = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data) + 
                                               sizeof(DtaIoctlPhyMacSetPacketFilterInput);
    IoPhyMacCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoPhyMacCmdInput.m_Cmd = DTA_PHYMAC_CMD_SET_PACKET_FILTER;
    IoPhyMacCmdInput.m_Data.m_SetPacketFilter.m_PacketFilter = PacketFilter;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_PHYMAC_CMD,
                                                SizeIn, &IoPhyMacCmdInput, 0, NULL, NULL);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetDeviceInfoIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNwGetDeviceInfoIoCtrl(
    DtaNwDeviceData*  pDvcData,
    UInt16*  pDeviceId,
    UInt16*  pVendorId,
    UInt16*  pSubVendorId,
    UInt16*  pSubSystemId,
    Int*  pBusNumber,
    Int*  pSlotNumber,
    Int*  pTypeNumber,
    Int*  pHardwareRevision,
    Int*  pFirmwareVersion,
    Int*  pFirmwareVariant,
    UInt64*  pSerial)
{
    DtStatus  Status;
    DtaIoctlGetDevInfoOutput  DevInfoOutput;
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_GET_DEV_INFO, 0, NULL,
                                  sizeof(DtaIoctlGetDevInfoOutput), &DevInfoOutput, NULL);
    if (!DT_SUCCESS(Status))
        return Status;

    if (pDeviceId != NULL)
        *pDeviceId = DevInfoOutput.m_DeviceId;
    if (pVendorId != NULL)
        *pVendorId = DevInfoOutput.m_VendorId;
    if (pSubVendorId != NULL)
        *pSubVendorId = DevInfoOutput.m_SubVendorId;
    if (pSubSystemId != NULL)
        *pSubSystemId = DevInfoOutput.m_SubSystemId;
    if (pBusNumber != NULL)
        *pBusNumber = DevInfoOutput.m_BusNumber;
    if (pSlotNumber != NULL)
        *pSlotNumber = DevInfoOutput.m_SlotNumber;
    if (pTypeNumber != NULL)
        *pTypeNumber = DevInfoOutput.m_TypeNumber;
    if (pHardwareRevision != NULL)
        *pHardwareRevision = DevInfoOutput.m_HardwareRevision;
    if (pFirmwareVersion != NULL)
        *pFirmwareVersion = DevInfoOutput.m_FirmwareVersion;
    if (pFirmwareVariant != NULL)
        *pFirmwareVariant = DevInfoOutput.m_FirmwareVariant;
    if (pSerial != NULL)
        *pSerial = DevInfoOutput.m_Serial;
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetPhySpeedIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwGetPhySpeedIoCtrl(
    DtaNwDeviceData*  pDvcData,
    UInt*  pSpeed)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlPhyMacCmdInput  IoPhyMacCmdInput;
    DtaIoctlPhyMacCmdOutput  IoPhyMacCmdOutput;
    UInt  SizeIn = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data);
    UInt  SizeOut = sizeof(DtaIoctlPhyMacGetPhySpeedOutput);
    IoPhyMacCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoPhyMacCmdInput.m_Cmd = DTA_PHYMAC_CMD_GET_PHY_SPEED;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_PHYMAC_CMD,
                            SizeIn, &IoPhyMacCmdInput, SizeOut, &IoPhyMacCmdOutput, NULL);
    if (!DT_SUCCESS(Status))
        return Status;
    *pSpeed = IoPhyMacCmdOutput.m_Data.m_GetPhySpeed.m_Speed;
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwSetPhySpeedIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwSetPhySpeedIoCtrl(
    DtaNwDeviceData*  pDvcData,
    UInt  Speed)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlPhyMacCmdInput  IoPhyMacCmdInput;
    UInt  SizeIn = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data) 
                                                 + sizeof(DtaIoctlPhyMacSetPhySpeedInput);
    
    IoPhyMacCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoPhyMacCmdInput.m_Cmd = DTA_PHYMAC_CMD_SET_PHY_SPEED;
    IoPhyMacCmdInput.m_Data.m_SetPhySpeed.m_Speed = Speed;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_PHYMAC_CMD,
                                                SizeIn, &IoPhyMacCmdInput, 0, NULL, NULL);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwRegisterCallbackIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwRegisterCallbackIoCtrl(
    DtaNwDeviceData*  pDvcData,
    LinkStatFunc  LinkStatCallback)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlPhyMacCmdInput  IoPhyMacCmdInput;
    UInt  SizeIn = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data) +
                    sizeof(DtaIoctlPhyMacRegisterCallbackInput);
    IoPhyMacCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoPhyMacCmdInput.m_Cmd = DTA_PHYMAC_CMD_REGISTER_CALLBACK;
    IoPhyMacCmdInput.m_Data.m_RegisterCallback.m_pContext = pDvcData;
    IoPhyMacCmdInput.m_Data.m_RegisterCallback.m_LinkStatusCallback = LinkStatCallback;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_PHYMAC_CMD,
                                                SizeIn, &IoPhyMacCmdInput, 0, NULL, NULL);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwUnregisterCallbackIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNwUnregisterCallbackIoCtrl(
    DtaNwDeviceData*  pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlPhyMacCmdInput  IoPhyMacCmdInput;
    UInt  SizeIn = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data) +
                    sizeof(DtaIoctlPhyMacUnregisterCallbackInput);
    IoPhyMacCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoPhyMacCmdInput.m_Cmd = DTA_PHYMAC_CMD_UNREGISTER_CALLBACK;
    IoPhyMacCmdInput.m_Data.m_RegisterCallback.m_pContext = pDvcData;
    
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_PHYMAC_CMD,
                                                SizeIn, &IoPhyMacCmdInput, 0, NULL, NULL);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwResetNwStatusIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNwResetNwStatusIoCtrl(DtaNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlPhyMacCmdInput  IoPhyMacCmdInput;
    UInt  SizeIn = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data);
    IoPhyMacCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoPhyMacCmdInput.m_Cmd = DTA_PHYMAC_CMD_RESET_NW_STATUS;
    
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_PHYMAC_CMD,
                                                SizeIn, &IoPhyMacCmdInput, 0, NULL, NULL);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwSetDriverVersionIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwSetDriverVersionIoCtrl(DtaNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlIpNwCmdInput  IoIpNwCmdInput;
    UInt  SizeIn = OFFSETOF(DtaIoctlIpNwCmdInput, m_Data)+sizeof(DtaIoctlIpNwSetDriverVersionInput);
    IoIpNwCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoIpNwCmdInput.m_Cmd = DTA_IP_NW_CMD_SET_DRIVER_VERSION;

    IoIpNwCmdInput.m_Data.m_SetDriverVersion.m_Major = DTANW_VERSION_MAJOR;
    IoIpNwCmdInput.m_Data.m_SetDriverVersion.m_Minor = DTANW_VERSION_MINOR;
    IoIpNwCmdInput.m_Data.m_SetDriverVersion.m_Micro = DTANW_VERSION_MICRO;
    IoIpNwCmdInput.m_Data.m_SetDriverVersion.m_Build = DTANW_VERSION_BUILD;
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_IP_NW_CMD,
                                                SizeIn, &IoIpNwCmdInput, 0, NULL, NULL);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwSetDriverVersionIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwGetSharedBuffersIoCtrl(DtaNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlIpNwCmdInput  IoIpNwCmdInput;
    DtaIoctlIpNwCmdOutput  IoIpNwCmdOutput;
    UInt  SizeIn = OFFSETOF(DtaIoctlIpNwCmdInput, m_Data);
    UInt  SizeOut = sizeof(DtaIoctlIpNwGetSharedBuffersOutput);
    
    IoIpNwCmdInput.m_IpPortIndex = pDvcData->m_IpPortIndex;
    IoIpNwCmdInput.m_Cmd = DTA_IP_NW_CMD_GET_SHARED_BUFFERS;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_IP_NW_CMD,
                                SizeIn, &IoIpNwCmdInput, SizeOut, &IoIpNwCmdOutput, NULL);
    if (DT_SUCCESS(Status))
    {
        pDvcData->m_AlignedPayload = 
                               IoIpNwCmdOutput.m_Data.m_GetSharedBuffers.m_AlignedPayload;
        pDvcData->m_pTxBuffer = IoIpNwCmdOutput.m_Data.m_GetSharedBuffers.m_pTxBuffer;
        pDvcData->m_pRxBuffer = IoIpNwCmdOutput.m_Data.m_GetSharedBuffers.m_pRxBuffer;
        pDvcData->m_TxBufSize = IoIpNwCmdOutput.m_Data.m_GetSharedBuffers.m_TxBufSize;
        pDvcData->m_RxBufSize = IoIpNwCmdOutput.m_Data.m_GetSharedBuffers.m_RxBufSize;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwSetMulticastListIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwSetMulticastListIoCtrl(
    DtaNwDeviceData*  pDvcData,
    UInt  NumItems,
    void*  pMulticastList)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlPhyMacCmdInput* pIoPhyMacCmdInput;
    
    UInt  SizeIn = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data);
    SizeIn += (NumItems * 6) + sizeof(UInt);

    pIoPhyMacCmdInput = DtMemAllocPool(DtPoolNonPaged, SizeIn, DTANW_MEM_TAG);
    if (pIoPhyMacCmdInput == NULL)
        return DT_STATUS_OUT_OF_MEMORY;

    pIoPhyMacCmdInput->m_IpPortIndex = pDvcData->m_IpPortIndex;
    pIoPhyMacCmdInput->m_Cmd = DTA_PHYMAC_CMD_SET_MULTICAST_LIST;
    pIoPhyMacCmdInput->m_Data.m_SetMulticastList.m_NumItems = NumItems;
    DtCopyMulticastListItems(NumItems, pMulticastList, 
                            (UInt8*)pIoPhyMacCmdInput->m_Data.m_SetMulticastList.m_Items);
    
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_PHYMAC_CMD,
                                SizeIn, pIoPhyMacCmdInput, 0, NULL, NULL);
    DtMemFreePool(pIoPhyMacCmdInput, DTANW_MEM_TAG);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetDtaDriverVersionIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNwGetDtaDriverVersionIoCtrl(
    DtaNwDeviceData*  pDvcData,
    Int*  pMajor,
    Int*  pMinor,
    Int*  pMicro)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoctlGetDriverVersionOutput  IoGetDriverVersionOutput;
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DTA_IOCTL_GET_DRIVER_VERSION,
                                          0, NULL, sizeof(DtaIoctlGetDriverVersionOutput),
                                          &IoGetDriverVersionOutput, NULL);
    if (DT_SUCCESS(Status))
    {
        *pMajor = IoGetDriverVersionOutput.m_Major;
        *pMinor = IoGetDriverVersionOutput.m_Minor;
        *pMicro = IoGetDriverVersionOutput.m_Micro;
    }
    return Status;
}
