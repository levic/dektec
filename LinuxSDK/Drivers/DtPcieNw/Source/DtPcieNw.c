// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPcieNw.c *#*#*#*#*#*#*#*#* (C) 2021-2022 DekTec
//
// DtPcieNw driver - Interface for the DtPcieNw common driver, used by the IAL.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2016 DekTec Digital Video B.V.
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
#include <DtPcieNwIncludes.h>
#include <PtpPkt.h>

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Forward declarations
static void  DtPcieNwAquireLock(DtPcieNwDeviceData* pDvcData);
static void  DtPcieNwReleaseLock(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwGetDtPcieDriverVersionIoCtrl(DtPcieNwDeviceData*  pDvcData,
                                                   Int* pMajor, Int* pMinor, Int* pMicro);
static DtStatus  DtPcieNwGetDeviceInfoIoCtrl(DtPcieNwDeviceData* pDvcData,
                                      UInt16* pDeviceId, UInt16* pVendorId,
                                      UInt16* pSubVendorId, UInt16* pSubSystemId,
                                      Int* pBusNumber, Int* pSlotNumber, Int* pTypeNumber,
                                      Int* pHardwareRevision, Int* pFirmwareVersion,
                                      Int* pFirmwareVariant, UInt64* pSerial);
static DtStatus  DtPcieNwGetPhySpeedIoCtrl(DtPcieNwDeviceData* pDvcData, UInt* pSpeed);
static DtStatus  DtPcieNwSetPhySpeedIoCtrl(DtPcieNwDeviceData* pDvcData, UInt  Speed);
static DtStatus  DtPcieNwGetMacAddressPermIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                      UInt8* pMacAddress);
static DtStatus  DtPcieNwGetPacketFilterIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                     UInt* pPacketFilter);
static DtStatus  DtPcieNwSetPacketFilterIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                       UInt PacketFilter);
static DtStatus  DtPcieNwGetStatisticCounterIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                        UInt StatisticId, UInt64* pValue);
static DtStatus  DtPcieNwRegisterCallbackIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                           LinkStatFunc LinkStatCallback);
static DtStatus  DtPcieNwRegisterOpModeCallbackIoCtrl(DtPcieNwDeviceData* pDvcData,
                                                               OpModeFunc OpModeCallback);
static DtStatus  DtPcieNwSetMacAddressIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                      UInt8* pMacAddress);
static DtStatus  DtPcieNwGetMaxMulticastListIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                 UInt* pMaxMulticastList);
static DtStatus  DtPcieNwUnregisterCallbackIoCtrl(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwResetNwStatusIoCtrl(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwSetDriverVersionIoCtrl(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwGetRxWriteOffsetIoCtrl(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwSetRxReadOffsetIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                          Int ReadOffset);
static DtStatus  DtPcieNwGetTxReadOffsetIoCtrl(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwSetTxWriteOffsetIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                         Int WriteOffset);
static DtStatus  DtPcieNwSetMulticastListIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                     UInt NumItems, void* pMulticastList);
static DtStatus  DtPcieNwInitSharedBuffers(DtPcieNwDeviceData* pDvcData);
void  DtPcieNwLinkStatusCallback(UInt LinkStatus, void* pContext);
void  DtPcieNwOpModeCallback(UInt OpMode, void* pContext);
static DtStatus  DtPcieNwCreateThreads(DtPcieNwDeviceData* pDvcData);
static void  DtPcieNwPowerdown(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwPowerup(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwCheckDtDriverVersion(DtPcieNwDeviceData* pDvcData);
static DtStatus  GetNwUuId(DtPcieNwDeviceData* pDvcData, Int* pUuId);
static DtStatus GetPropertyIntIoctl(DtPcieNwDeviceData* pDvcData, char* pName, 
                                                                Int PortIndex, Int* pInt);
static DtStatus  DtPcieNwSetOperationalModeIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                              Int OpMode);
static DtStatus  DtPcieNwGetLinkStatusIoCtrl(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwAcquireExclusiveAccess(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwReleaseExclusiveAccess(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwRegisterRxDataEvent(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwUnregisterRxDataEvent(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwOpenNrtPipes(DtPcieNwDeviceData* pDvcData);
static DtStatus  DtPcieNwOpenPipe(DtPcieNwDeviceData* pDvcData, Int PipeType, 
                                                                         UInt* pPipeUuid);
static DtStatus  DtPcieNwClosePipe(DtPcieNwDeviceData* pDvcData, Int PipeType, 
                                                                           UInt PipeUuid);
static DtStatus  DtPcieNwSetSharedKBufferIoCtrl(DtPcieNwDeviceData* pDvcData, 
  UInt PipeUuid, UInt ReqbufSize, UInt8** ppBuffer, UInt* pActualBufSize, UInt* pMaxLoad);
static DtStatus  DtPcieNwGetLinkStatusCB(DtPcieNwDeviceData* pDvcData);
static void  DtPciNwLinkStatusWi(DtWorkItemArgs* pArgs);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwDeviceDataInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPcieNwDeviceDataInit(DtPcieNwDeviceData* pDvcData)
{
    pDvcData->m_PromiscuousModeSet = FALSE;
    pDvcData->m_PhySpeedSet = DTA_PHY_SPEED_AUTO_DETECT;
    pDvcData->m_MacAddressOverride = FALSE;
    pDvcData->m_Support8021P_Priority = TRUE;
    pDvcData->m_Supports8021Q_Vlan = TRUE;
    pDvcData->m_NwRcvNoBuffer = 0;
    pDvcData->m_OperMode = DT_FUNC_OPMODE_IDLE;
    DtSpinLockInit(&pDvcData->m_FingerprintLock);
    DtEventInit(&pDvcData->m_RxDataAvailableEvent, TRUE);
    DtWorkItemInit(&pDvcData->m_LinkStatusWorkItem, DtPciNwLinkStatusWi, TRUE,
                                                                     &pDvcData->m_Device);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwDriverInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL and initializes the common (host independent) part
// of the driver.
//
DtStatus  DtPcieNwDriverInit(DtEvtLog* pEvtObject)
{
    DtStatus  Status = DT_STATUS_OK;
    DtString  DtStrVersion;
    DtStringChar  DtStrVersionBuffer[32];
    DT_STRING_DECL(DtStrDot, ".");

    // Connect DtStrVersionBuffer to DtStrVersion
    DT_STRING_INIT(DtStrVersion, DtStrVersionBuffer, 32);

    // Create driver version string
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTPCIENW_VERSION_MAJOR);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTPCIENW_VERSION_MINOR);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTPCIENW_VERSION_MICRO);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTPCIENW_VERSION_BUILD);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Finally report the event
    DtEvtLogReport(pEvtObject, DTPCIENW_LOG_DRIVER_LOADED, &DtStrVersion, NULL, NULL);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwDriverExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the driver is unloaded. This function should
// undo everything done by DtPcieNwDriverInit.
//
void  DtPcieNwDriverExit()
{
    DtDbgOut(MAX, DTPCIENW, "Start");
    
    DtDbgOut(MAX, DTPCIENW, "Exit");
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwDeviceInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the device is found in the system by the DtPcie
// driver. 
//
DtStatus  DtPcieNwDeviceInit(DtPcieNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    DtDbgOut(MAX, DTPCIENW, "Start");
    DtSpinLockInit(&pDvcData->m_RxLock);

    // Get Device Information
    Status = DtPcieNwGetDeviceInfoIoCtrl(pDvcData, NULL, NULL, NULL, NULL, 
                                         &pDvcData->m_BusNumber, &pDvcData->m_SlotNumber,
                                         &pDvcData->m_TypeNumber, &pDvcData->m_HwRevision,
                                         &pDvcData->m_FwVersion, NULL, NULL);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTPCIENW, "Error getting device info. Error: 0x%x", Status);
        return Status;
    }

    // Get Dta driver version
    Status = DtPcieNwCheckDtDriverVersion(pDvcData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTPCIENW, "Error checking DtPcie driver version: 0x%x", Status);
        return Status;
    }

    // First get UUID driver function NRT
    Status = GetNwUuId(pDvcData, &pDvcData->m_NwUuid);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTPCIENW, "Error retrieving UUID DF NRT: 0x%x", Status);
        return Status;
    }

    Status = DtPcieNwAcquireExclusiveAccess(pDvcData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTPCIENW, "Error requesting exclusive access: 0x%x", Status);
        return Status;
    }

    // Set modules to IDLE if not already in that state
    Status = DtPcieNwSetOperationalModeIoCtrl(pDvcData, DT_FUNC_OPMODE_IDLE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTPCIENW, "Error setting operational mode to IDLE: 0x%x", Status);
        return Status;
    }
    pDvcData->m_OperMode = DT_FUNC_OPMODE_IDLE;

    // Get permanent address
    Status = DtPcieNwGetMacAddressPerm(pDvcData, pDvcData->m_MacAddressPerm);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTPCIENW, "Error getting permanent MAC address. Error: 0x%x", 
                                                                                  Status);
        return Status;
    }

    // Copy permanent address to current address
    if (!pDvcData->m_MacAddressOverride)
        DtMemCopy(pDvcData->m_MacAddressCur, pDvcData->m_MacAddressPerm,
                                                      sizeof(pDvcData->m_MacAddressPerm));
    
    // Update MAC address in DtPcie driver always, because during stop and start of 
    // network driver, the MAC address can be changed without resetting the DtPcie 
    // MAC address
    Status = DtPcieNwSetMacAddress(pDvcData, pDvcData->m_MacAddressCur);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTPCIENW, "Error setting current MAC address. Error: 0x%x", Status);
        return Status;
    }
    
    DtDbgOut(MAX, DTPCIENW, "Exit");
    
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwDeviceOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the interface goes to the up state
//
DtStatus  DtPcieNwDeviceOpen(DtPcieNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    DtDbgOut(MAX, DTPCIENW, "Start");
    
    pDvcData->m_LinkStatus = DTPCIENW_LINKSTAT_DOWN;

    // Register OpMode callback
    if (DT_SUCCESS(Status))
        Status = DtPcieNwRegisterOpModeCallbackIoCtrl(pDvcData, DtPcieNwOpModeCallback);
    
    // Register callback and get link status using callback
    if (DT_SUCCESS(Status))
        Status = DtPcieNwRegisterCallbackIoCtrl(pDvcData, DtPcieNwLinkStatusCallback);
    if (DT_SUCCESS(Status))
        Status = DtPcieNwGetLinkStatusCB(pDvcData);
    
    // Reset status in DtPcie driver
    if (DT_SUCCESS(Status))
        Status = DtPcieNwResetNwStatusIoCtrl(pDvcData);

    // Set Network driver version
    if (DT_SUCCESS(Status))
        Status = DtPcieNwSetDriverVersionIoCtrl(pDvcData);

    if (DT_SUCCESS(Status))
        Status = DtPcieNwGetMaxMulticastList(pDvcData, &pDvcData->m_MaxNumMulticastItems);

    // Open NRT pipes
    if (DT_SUCCESS(Status))
        Status = DtPcieNwOpenNrtPipes(pDvcData);

    // Initialise shared buffers for Network Tx/Rx
    if (DT_SUCCESS(Status))
        Status = DtPcieNwInitSharedBuffers(pDvcData);

    if (DT_SUCCESS(Status))
        Status = DtPcieNwRegisterRxDataEvent(pDvcData);

    // Update Speed/Duplex
    if (DT_SUCCESS(Status))
        Status = DtPcieNwSetPhySpeed(pDvcData, pDvcData->m_PhySpeedSet);
    
    // Create worker threads
    if (DT_SUCCESS(Status))
        Status = DtPcieNwCreateThreads(pDvcData);

    if (DT_SUCCESS(Status))
        Status = DtPcieNwPowerup(pDvcData);

    if (DT_SUCCESS(Status))
    {
        // Set modules to RUN
        Status = DtPcieNwSetOperationalModeIoCtrl(pDvcData, DT_FUNC_OPMODE_RUN);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTPCIENW, "Error setting operational mode to RUN:0x%x", Status);
        }
        pDvcData->m_OperMode = DT_FUNC_OPMODE_RUN;
    }
    
    // Mark device as opened also when an error occures. This is not
    // a problem for the close.
    pDvcData->m_DeviceOpened = TRUE;
    
    
    DtDbgOut(MAX, DTPCIENW, "Exit");
    
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwDeviceClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL when the interface goes to the down state
// The function should undo everything done by DtPcieNwDeviceOpen.
//
void  DtPcieNwDeviceClose(DtPcieNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDbgOut(MAX, DTPCIENW, "Start");

    // Prevent race condition with transmitting packets
    DtPcieNwAquireLock(pDvcData);
    pDvcData->m_OperMode = DT_FUNC_OPMODE_IDLE;
    DtPcieNwReleaseLock(pDvcData);

    // Unregister opmode callback
    DtPcieNwRegisterOpModeCallbackIoCtrl(pDvcData, NULL);

    // Unregister callbacks
    DtPcieNwUnregisterCallbackIoCtrl(pDvcData);

    DtPcieNwUnregisterRxDataEvent(pDvcData);

    // Set modules to IDLE
    Status = DtPcieNwSetOperationalModeIoCtrl(pDvcData, DT_FUNC_OPMODE_IDLE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTPCIENW, "Error setting operational mode to IDLE: 0x%x", Status);
    }
    pDvcData->m_OperMode = DT_FUNC_OPMODE_IDLE;

    // Close pipes
    DtPcieNwClosePipe(pDvcData, DT_PIPE_RX_NRT, pDvcData->m_RxUuid);
    DtPcieNwClosePipe(pDvcData, DT_PIPE_TX_NRT, pDvcData->m_TxUuid);

    DtPcieNwPowerdown(pDvcData);

    pDvcData->m_DeviceOpened = FALSE;
    DtDbgOut(MAX, DTPCIENW, "Exit");
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwDeviceExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the device is being disconnected. This
// function should undo everything done by DtPcieNwDeviceInit.
//
void  DtPcieNwDeviceExit(DtPcieNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    DtDbgOut(MAX, DTPCIENW, "Start");

    Status = DtPcieNwReleaseExclusiveAccess(pDvcData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTPCIENW, "Error releasing exclusive access: 0x%x", Status);
    }
    DtSpinLockFree(&pDvcData->m_RxLock);
    DtDbgOut(MAX, DTPCIENW, "Exit");
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwTxGetPointerNewPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is used to get a pointer to the Tx buffer with size: PacketSize.
// If not enough space available, an error is returned. The WriteOffset parameter is used
// by the DtPcieNwTransmitPacket function.
//
DtStatus  DtPcieNwTxGetPointerNewPacket(
    DtPcieNwDeviceData* pDvcData,
    UInt  PacketSize,
    UInt8** ppPacket1,
    UInt* pPacketSize1,
    UInt8** ppPacket2,
    UInt* pPacketSize2)
{
    UInt  ReadOffset;
    UInt  TotalLength;
    UInt  TotalFreeSpace;

    // Sanity check. Check if we are in operational mode and buffer is valid
    if (pDvcData->m_OperMode==DT_FUNC_OPMODE_IDLE || pDvcData->m_pTxBuffer==NULL || 
                                                                 pDvcData->m_TxBufSize==0)
        return DT_STATUS_OUT_OF_RESOURCES;

    TotalLength = PacketSize + sizeof(DtEthIp);

    if (TotalLength % 8 != 0)
        TotalLength += 8 - (TotalLength % 8);

    ReadOffset = pDvcData->m_TxReadOffsetCache;

    // Calculate free space
    // Enough bytes must be free should be checked before write
    TotalFreeSpace = (Int)((ReadOffset + pDvcData->m_TxMaxLoad -
        pDvcData->m_TxWriteOffset) % pDvcData->m_TxBufSize);
    if (TotalFreeSpace < TotalLength)
    {
        // retrieve new read offset
        DtPcieNwGetTxReadOffsetIoCtrl(pDvcData);
        ReadOffset = pDvcData->m_TxReadOffsetCache;

        TotalFreeSpace = (Int)((ReadOffset + pDvcData->m_TxMaxLoad -
            pDvcData->m_TxWriteOffset) % pDvcData->m_TxBufSize);
        if (TotalFreeSpace < TotalLength)
        {
            // Not enough room, skip pakket
            DtDbgOut(MAX, DTPCIENW, "Shared buffer full. Packet discarded");
            return DT_STATUS_BUFFER_OVERFLOW;
        }
    }
    *ppPacket1 = pDvcData->m_pTxBuffer + pDvcData->m_TxWriteOffset;
    *pPacketSize1 = pDvcData->m_TxBufSize - pDvcData->m_TxWriteOffset;
    if (*pPacketSize1 >= TotalLength)
    {
        *pPacketSize2 = 0;
        *pPacketSize1 = TotalLength;
    }
    else
    {
        *ppPacket2 = pDvcData->m_pTxBuffer;
        *pPacketSize2 = TotalLength - *pPacketSize1;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- BuildPtpTlv -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void BuildPtpRxTimeTlv(DtEthIpTod* pTod, DektecTlvTime* pTlv)
{
    OETlvHeader* pTlvHdr = (OETlvHeader*)pTlv;
    pTlvHdr->Type = NWCW(TLV_ORGANIZATION_EXTENSION);
    pTlvHdr->Id[0] = 0x00;
    pTlvHdr->Id[1] = 0x14;
    pTlvHdr->Id[2] = 0xF4;
    pTlvHdr->SubType[0] = TLV_SUBTYPE_RXTIME;
    pTlvHdr->SubType[1] = 0;
    pTlvHdr->SubType[2] = 0;
    pTlvHdr->Length = NWCW(sizeof(DektecTlvTime)-sizeof(TlvHeader));
    pTlv->TodSeconds = pTod->m_TodSeconds;
    pTlv->TodNanoseconds = pTod->m_TodNanoseconds;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- BuildPtpTxTimeTlv -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void BuildPtpTxTimeTlv(uint64_t Seconds, uint64_t Nanoseconds, DektecTlvTime* pTlv)
{
    OETlvHeader* pTlvHdr = (OETlvHeader*)pTlv;
    pTlvHdr->Type = NWCW(TLV_ORGANIZATION_EXTENSION);
    pTlvHdr->Id[0] = 0x00;
    pTlvHdr->Id[1] = 0x14;
    pTlvHdr->Id[2] = 0xF4;
    pTlvHdr->SubType[0] = TLV_SUBTYPE_TXTIME;
    pTlvHdr->SubType[1] = 0;
    pTlvHdr->SubType[2] = 0;
    pTlvHdr->Length = NWCW(sizeof(DektecTlvTime)-sizeof(TlvHeader));
    pTlv->TodSeconds = Seconds;
    pTlv->TodNanoseconds = Nanoseconds;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- BuildPtpSourceIpTlv -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void BuildPtpSourceIpTlv(UInt8* pIp, Int IpLength, DektecTlvSourceIp* pTlv)
{
    OETlvHeader* pTlvHdr = (OETlvHeader*)pTlv;
    Int i;
    pTlvHdr->Type = NWCW(TLV_ORGANIZATION_EXTENSION);
    pTlvHdr->Id[0] = 0x00;
    pTlvHdr->Id[1] = 0x14;
    pTlvHdr->Id[2] = 0xF4;
    pTlvHdr->SubType[0] = TLV_SUBTYPE_SOURCE_IP;
    pTlvHdr->SubType[1] = 0;
    pTlvHdr->SubType[2] = 0;
    pTlvHdr->Length = NWCW(sizeof(DektecTlvSourceIp)-sizeof(TlvHeader));
    for (i=0; i<IpLength; i++)
        pTlv->IpAddress[i] = pIp[i];
    pTlv->IpLength = (uint8_t)IpLength;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ComputeIpCheckSum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt16 ComputeIpCheckSum(IpHeaderV4* pIpHeader)
{
    // Compute header checksum for IP
    UInt32  Checksum = 0;
    UInt32  ChecksumCs;
    Int i;
    pIpHeader->m_HeaderChecksum = 0;
    for (i = 0; i < (pIpHeader->m_HeaderLength*2); i++)
        Checksum += ((UInt16*)pIpHeader)[i] & 0xffff;

    ChecksumCs = (Checksum >> 16) & 0xffff;       // Include carries - I
    Checksum = (Checksum & 0xffff) + ChecksumCs;  // Add high words to low words
    ChecksumCs = (Checksum >> 16) & 0xffff;       // Include carries - II
    Checksum = (Checksum & 0xffff) + ChecksumCs;
    Checksum ^= -1;
    Checksum &= 0xffff;                           // Checksum = 16 bit
    
    if (Checksum == 0)
        Checksum = 0xffff;

    return (UInt16)Checksum;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- UpdatePacketHeaders -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void UpdatePacketHeaders(UInt8* pPacket, UInt PacketSize, OETlvHeader* pTlvHeader)
{
    EthernetIIHeader* pEthIIHeader;
    EthernetIIHeaderVlan* pEthIIHeaderVlan;
    UdpHeader* pUdpHeader;
    PtpHeader* pPtpHdr;
    Int EthHeaderSize;
    Int  EthType;
    IpHeaderV4* pIpV4Header = NULL;
    IpHeaderV6* pIpV6Header = NULL;

    pEthIIHeader = (EthernetIIHeader*)pPacket;
    pEthIIHeaderVlan = (EthernetIIHeaderVlan*)pPacket;

    if (pEthIIHeader->m_Type == ETH_TYPE_VLAN_BE)
    {
        EthType = pEthIIHeader->m_Type;
        EthHeaderSize = sizeof(EthernetIIHeaderVlan);
    }
    else {
        EthType = pEthIIHeader->m_Type;
        EthHeaderSize = sizeof(EthernetIIHeader);
    }

    if (EthType == ETH_TYPE_IPV4_BE)
    {
        
        pIpV4Header = (IpHeaderV4*)(pPacket + EthHeaderSize);
        pUdpHeader = (UdpHeader*)((UInt8*)pIpV4Header+(pIpV4Header->m_HeaderLength * 4));
    }
    else if (EthType == ETH_TYPE_IPV6_BE)
    {
        UInt8* pData;
        IpV6Options* pOptions;
        UInt ExtHeader;
        pIpV6Header = (IpHeaderV6*)(pPacket + EthHeaderSize);
        // IpV6
        pData = (UInt8*)pIpV6Header + sizeof(IpHeaderV6);
        ExtHeader = pIpV6Header->m_NextHeader;

        while (TRUE)
        {
            pOptions = (IpV6Options*)pData;
            if (ExtHeader == IPV4_HDR_PROT_UDP)
                break;
            else if (ExtHeader != IPV6_HDR_HOPBYHOP && ExtHeader != IPV6_HDR_AUTH &&
                ExtHeader != IPV6_HDR_DST_OPTIONS && ExtHeader != IPV6_HDR_DST_OPTIONS)
                return;
            pData += 1 + pOptions->m_HeaderExtLength;
            ExtHeader = pOptions->m_NextHeader;
        }
        pUdpHeader = (UdpHeader*)((UInt8*)pData);
    }
    else
        return;
    
    if (NWCW(pUdpHeader->m_DestinationPort) != UDP_PORT_PTP_EVENT &&
        NWCW(pUdpHeader->m_DestinationPort) != UDP_PORT_PTP_COMMON)
        return;

    pPtpHdr = (PtpHeader*)(((UInt8*)pUdpHeader) + sizeof(UdpHeader));
    if (pPtpHdr->VersionPtp == PTP_VERSION)
    {
        if (pTlvHeader->Length != 0)
        {
            Int  TlvLength = NWCW(pTlvHeader->Length) + sizeof(TlvHeader);
            pUdpHeader->m_Checksum = 0;
            pUdpHeader->m_Length = NWCW(NWCW(pUdpHeader->m_Length) + TlvLength);
            if (pIpV4Header != NULL)
            {
                /*pIpV4Header->m_SrcAddress[0] = 127;
                pIpV4Header->m_SrcAddress[1] = 0;
                pIpV4Header->m_SrcAddress[2] = 0;
                pIpV4Header->m_SrcAddress[3] = 1;*/
                pIpV4Header->m_TotalLength = NWCW(NWCW(pIpV4Header->m_TotalLength) +
                                                                               TlvLength);
                pIpV4Header->m_HeaderChecksum = ComputeIpCheckSum(pIpV4Header);
            }
            else
            {
                /*pIpV6Header->m_SrcAddress[0] = 0;
                pIpV6Header->m_SrcAddress[1] = 0;
                pIpV6Header->m_SrcAddress[2] = 0;
                pIpV6Header->m_SrcAddress[3] = 0;
                pIpV6Header->m_SrcAddress[4] = 0;
                pIpV6Header->m_SrcAddress[5] = 0;
                pIpV6Header->m_SrcAddress[6] = 0;
                pIpV6Header->m_SrcAddress[7] = 0;
                pIpV6Header->m_SrcAddress[8] = 0;
                pIpV6Header->m_SrcAddress[9] = 0;
                pIpV6Header->m_SrcAddress[10] = 0;
                pIpV6Header->m_SrcAddress[11] = 0;
                pIpV6Header->m_SrcAddress[12] = 0;
                pIpV6Header->m_SrcAddress[13] = 0;
                pIpV6Header->m_SrcAddress[14] = 0;
                pIpV6Header->m_SrcAddress[15] = 1;*/
                pIpV6Header->m_PayloadLength = NWCW(NWCW(pIpV6Header->m_PayloadLength) +
                    TlvLength);
                // TODO: pUdpHeader->m_Checksum
            }
            //pEthIIHeader->m_SrcMac[0] = 0x0;
            //pEthIIHeader->m_SrcMac[1] = 0x0;
            //pEthIIHeader->m_SrcMac[2] = 0x0;
            //pEthIIHeader->m_SrcMac[3] = 0x0;
            pEthIIHeader->m_SrcMac[4] = 0x0;
            pEthIIHeader->m_SrcMac[5] = 0x0;

        }
    }
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ParsePtpPacketRx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void ParsePtpPacketRx(UInt8* pPacket, UInt PacketSize, DektecTlvs* pDektecTlv)
{
    DtEthIp* pEthIp = (DtEthIp*)pPacket;
    DtEthIpHeader* pEthIpHeader = &pEthIp->m_Hdr;
    UdpHeader* pUdpHeader;
    PtpHeader* pPtpHdr;
    
    pDektecTlv->TlvHeader.Length = 0;
    if (pEthIpHeader->m_Protocol != DT_ETHIP_PROTOCOL_UDP)
        return;
    if (PacketSize<((pEthIpHeader->m_PortOffset*4)+sizeof(UdpHeader)+sizeof(PtpHeader)))
        return;
    
    pUdpHeader = (UdpHeader*)(pPacket + (pEthIpHeader->m_PortOffset * 4));
    
    if (NWCW(pUdpHeader->m_DestinationPort) != UDP_PORT_PTP_EVENT &&
                               NWCW(pUdpHeader->m_DestinationPort) != UDP_PORT_PTP_COMMON)
        return;

    pPtpHdr = (PtpHeader*)((UInt8*)pUdpHeader + sizeof(UdpHeader));
    
    if (pPtpHdr->VersionPtp == PTP_VERSION)
    {
        switch (pPtpHdr->MessageType)
        {
        case PTP_MSGTYPE_SYNC:
        case PTP_MSGTYPE_PDELAY_REQ:
        case PTP_MSGTYPE_PDELAY_RESP:
            BuildPtpRxTimeTlv(&pEthIp->m_Tod, (DektecTlvTime*)pDektecTlv);
            //pPtpHdr->MessageLength = NWCW(NWCW(pPtpHdr->MessageLength) +
            //                                                     sizeof(DektecTlvTime));
            break;
        case PTP_MSGTYPE_ANNOUNCE:
        {
            UInt8* pIp = pPacket + pEthIpHeader->m_IpAddressOffset * 4;
            Int IpSize = (pEthIpHeader->m_PacketType == DT_ETHIP_PKTTYPE_IPV4 ? 4 : 16);
            BuildPtpSourceIpTlv(pIp, IpSize, (DektecTlvSourceIp*)pDektecTlv);
            //pPtpHdr->MessageLength = NWCW(NWCW(pPtpHdr->MessageLength) + 
            //                                                 sizeof(DektecTlvSourceIp));
            break;
        }
        }
        if (pDektecTlv->TlvHeader.Length != 0)
        {
            Int  TlvLength = NWCW(pDektecTlv->TlvHeader.Length) + sizeof(TlvHeader);
            pUdpHeader->m_Checksum = 0;
            pUdpHeader->m_Length = NWCW(NWCW(pUdpHeader->m_Length) + TlvLength);
            if (pEthIpHeader->m_PacketType == DT_ETHIP_PKTTYPE_IPV4)
            {
                IpHeaderV4* pIpV4Header = (IpHeaderV4*)(pPacket + 
                                                     pEthIpHeader->m_IpAddressOffset * 4 -
                                                     OFFSETOF(IpHeaderV4, m_SrcAddress));
                pIpV4Header->m_TotalLength = NWCW(NWCW(pIpV4Header->m_TotalLength) + 
                                                                               TlvLength);
                pIpV4Header->m_HeaderChecksum = ComputeIpCheckSum(pIpV4Header);
            }
            else
            {
                IpHeaderV6* pIpV6Header = (IpHeaderV6*)(pPacket + 
                                                     pEthIpHeader->m_IpAddressOffset * 4 -
                                                     OFFSETOF(IpHeaderV6, m_SrcAddress));

                pIpV6Header->m_PayloadLength = NWCW(NWCW(pIpV6Header->m_PayloadLength) +
                                                                               TlvLength);
                // TODO: pUdpHeader->m_Checksum
            }
        }
    }
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwTransmitPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Pre: Packet data is already copied by the calling function to the buffer retrieved
// by DtPcieNwTxGetPointerNewPacket
//
DtStatus  DtPcieNwTransmitPacket(
    DtPcieNwDeviceData* pDvcData,
    UInt  PacketSize,
    UInt  TotalWriteOffset,
    UInt  TxFlags,
    UInt8  Fingerprint,
    UInt8  PortOffset,
    UInt8  IpAddrOffset)
{
    UInt8* pDst;
    DtEthIpHeader* pEthIpHeader;

    DT_ASSERT(PacketSize + sizeof(DtEthIp) <= TotalWriteOffset);
    DT_ASSERT((TotalWriteOffset % 8) == 0);

    pDst = pDvcData->m_pTxBuffer + pDvcData->m_TxWriteOffset;

    // The DtEthIpHeader should fit if at end at buffer.
    pEthIpHeader = (DtEthIpHeader*)pDst;
    DtMemZero(pEthIpHeader, sizeof(DtEthIpHeader));
    pEthIpHeader->m_SyncWord = DT_ETHIP_SYNCWORD;
    pEthIpHeader->m_SizeInBytes = PacketSize;
    pEthIpHeader->m_SizeInQWords = TotalWriteOffset / 8;
    if ((TxFlags & DTPCIENW_TXFLAGS_GEN_TX_TIMESTAMP) != 0)
    {
        pEthIpHeader->m_TimestampRequest = 1;
        pEthIpHeader->m_Fingerprint = Fingerprint;
    }

    // IP Header protocol
    if ((TxFlags & DTPCIENW_TXFLAGS_PROT_UDP) != 0)
        pEthIpHeader->m_Protocol = DT_ETHIP_PROTOCOL_UDP;
    else if ((TxFlags & DTPCIENW_TXFLAGS_PROT_TCP) != 0)
        pEthIpHeader->m_Protocol = DT_ETHIP_PROTOCOL_TCP;
    else
    {
        DT_ASSERT(PortOffset==0);
        pEthIpHeader->m_Protocol = DT_ETHIP_PROTOCOL_OTHER;
    }

    // IP Header packettype
    if ((TxFlags & DTPCIENW_TXFLAGS_PKTTYPE_IPV4) != 0)
        pEthIpHeader->m_PacketType = DT_ETHIP_PKTTYPE_IPV4;
    else if ((TxFlags & DTPCIENW_TXFLAGS_PKTTYPE_IPV6) != 0)
        pEthIpHeader->m_PacketType = DT_ETHIP_PKTTYPE_IPV6;
    else
        pEthIpHeader->m_PacketType = DT_ETHIP_PKTTYPE_OTHER;

    pEthIpHeader->m_IpAddressOffset = IpAddrOffset;
    pEthIpHeader->m_PortOffset = PortOffset;

    
    // Packet data is already copied by calling function
    pDvcData->m_TxWriteOffset = (pDvcData->m_TxWriteOffset +
                              (pEthIpHeader->m_SizeInQWords * 8)) % pDvcData->m_TxBufSize;

    // Trigger DtPcie driver
    DtPcieNwSetTxWriteOffsetIoCtrl(pDvcData, pDvcData->m_TxWriteOffset);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetLinkStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPcieNwGetLinkStatus(DtPcieNwDeviceData* pDvcData, UInt* pPhyLink)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt Speed;
    *pPhyLink = DT_PHY_LINK_DOWN;
    Status = DtPcieNwGetPhySpeedIoCtrl(pDvcData, &Speed);
    if (!DT_SUCCESS(Status))
        return Status;
    if (Speed != DT_PHY_SPEED_NO_LINK)
        *pPhyLink = DT_PHY_LINK_UP;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetLinkStatusCB -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPcieNwGetLinkStatusCB(DtPcieNwDeviceData* pDvcData)
{
    return DtPcieNwGetLinkStatusIoCtrl(pDvcData);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetMacAddressPerm -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPcieNwGetMacAddressPerm(DtPcieNwDeviceData* pDvcData, UInt8* pMacAddress)
{
    return DtPcieNwGetMacAddressPermIoCtrl(pDvcData, pMacAddress);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetPacketFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPcieNwGetPacketFilter(DtPcieNwDeviceData* pDvcData, UInt* pPacketFilter)
{
    return DtPcieNwGetPacketFilterIoCtrl(pDvcData, pPacketFilter);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetPacketFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPcieNwSetPacketFilter(DtPcieNwDeviceData* pDvcData, UInt PacketFilter)
{
    if (pDvcData->m_PromiscuousModeSet)
        PacketFilter |= DTA_MAC_FLT_PROMISCUOUS;
    return DtPcieNwSetPacketFilterIoCtrl(pDvcData, PacketFilter);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetStatisticCounter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPcieNwGetStatisticCounter(
    DtPcieNwDeviceData*  pDvcData,
    UInt  StatisticId,
    UInt64*  pValue)
{
    DtStatus  Status = DtPcieNwGetStatisticCounterIoCtrl(pDvcData, StatisticId, pValue);
    if (StatisticId == DTA_MAC_CNT_GEN_RCV_NO_BUFFER)
        *pValue+= pDvcData->m_NwRcvNoBuffer;

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetMaxMulticastList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPcieNwGetMaxMulticastList(DtPcieNwDeviceData* pDvcData, 
                                                                  UInt* pMaxMulticastList)
{
    return DtPcieNwGetMaxMulticastListIoCtrl(pDvcData, pMaxMulticastList);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetMulticastList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPcieNwSetMulticastList(
    DtPcieNwDeviceData*  pDvcData, 
    UInt  NumItems, 
    void*  pMulticastList)
{
    return DtPcieNwSetMulticastListIoCtrl(pDvcData, NumItems, pMulticastList);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetMulticastList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPcieNwGetMulticastList(DtPcieNwDeviceData* pDvcData)
{
    DT_ASSERT(FALSE);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPcieNwGetSpeed(DtPcieNwDeviceData* pDvcData, UInt64* pSpeed)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  Speed = DT_PHY_SPEED_NOT_SET;
    Status = DtPcieNwGetPhySpeedIoCtrl(pDvcData, &Speed);
    switch (Speed)
    {
    case DT_PHY_SPEED_10_HALF:
    case DT_PHY_SPEED_10_FULL:
        *pSpeed = 10000000; 
        break;
    case DT_PHY_SPEED_100_HALF:
    case DT_PHY_SPEED_100_FULL:
        *pSpeed = 100000000; 
        break;
    case DT_PHY_SPEED_1000_MASTER:
    case DT_PHY_SPEED_1000_SLAVE:
        *pSpeed = 1000000000;
        break;
    case DT_PHY_SPEED_10000:
        *pSpeed = 10000000000;
        break;
    default: 
        *pSpeed = 0; 
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetPhySpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPcieNwGetPhySpeed(DtPcieNwDeviceData* pDvcData, UInt* pPhySpeed)
{
    return DtPcieNwGetPhySpeedIoCtrl(pDvcData, pPhySpeed);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetPhySpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPcieNwSetPhySpeed(DtPcieNwDeviceData* pDvcData, UInt PhySpeed)
{
    return DtPcieNwSetPhySpeedIoCtrl(pDvcData, PhySpeed);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPcieNwSetMacAddress(DtPcieNwDeviceData* pDvcData, UInt8* pMacAddress)
{
    return DtPcieNwSetMacAddressIoCtrl(pDvcData, pMacAddress);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwOpenNrtPipes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwOpenNrtPipes(DtPcieNwDeviceData* pDvcData)
{
    DtStatus Status = DT_STATUS_OK;
    Status = DtPcieNwOpenPipe(pDvcData, DT_PIPE_RX_NRT, &pDvcData->m_RxUuid);
    if (!DT_SUCCESS(Status))
        return Status;
    return DtPcieNwOpenPipe(pDvcData, DT_PIPE_TX_NRT, &pDvcData->m_TxUuid);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwInitSharedBuffers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwInitSharedBuffers(DtPcieNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    Status = DtPcieNwSetSharedKBufferIoCtrl(pDvcData, pDvcData->m_RxUuid, 
                RX_NRT_SWQ_BUFSIZE, &pDvcData->m_pRxBuffer, &pDvcData->m_RxBufSize, NULL);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtPcieNwSetSharedKBufferIoCtrl(pDvcData, pDvcData->m_TxUuid, 
                                          TX_NRT_SWQ_BUFSIZE, &pDvcData->m_pTxBuffer, 
                                          &pDvcData->m_TxBufSize, &pDvcData->m_TxMaxLoad);
    if (!DT_SUCCESS(Status))
        return Status;
    
    pDvcData->m_RxReadOffset = 0;
    pDvcData->m_RxWriteOffsetCache = 0;
    pDvcData->m_TxWriteOffset = 0;
    pDvcData->m_TxReadOffsetCache = 0;
    return Status;
}

Int GetRxBufLoad(DtPcieNwDeviceData* pDvcData)
{
    return (Int)((pDvcData->m_RxWriteOffsetCache + pDvcData->m_RxBufSize -
                                       pDvcData->m_RxReadOffset) % pDvcData->m_RxBufSize);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetFingerprint -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPcieNwGetFingerprint(DtPcieNwDeviceData* pDvcData, void* pPacket, 
                                                                      UInt8* pFingerprint)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  Fingerprint;
    DtSpinLockAcquire(&pDvcData->m_FingerprintLock);
    Fingerprint = pDvcData->m_Fingerprint;
    if (Fingerprint == MAX_FINGERPRINT)
        Fingerprint = 1;
    else
        Fingerprint++;
    if (pDvcData->m_pFingerprintPackets[Fingerprint] != NULL)
    {
        // ERROR out of fingerprints
        DtDbgOut(ERR, DTPCIENW, "Out of fingerprints. #pending:%i", 
                                                      pDvcData->m_NumTxTimestampRequests);
        Status = DT_STATUS_FAIL;
    }
    else {
        
        pDvcData->m_pFingerprintPackets[Fingerprint] = pPacket;
        pDvcData->m_FingerPrintTimestamp[Fingerprint] = DtGetTickCount();
        *pFingerprint = Fingerprint;
        pDvcData->m_Fingerprint = Fingerprint;
        pDvcData->m_NumTxTimestampRequests++;
        DtDbgOut(ERR, DTPCIENW, "New fingerprint: %i. #pending:%i", Fingerprint,
                                                      pDvcData->m_NumTxTimestampRequests);

    }
    DtSpinLockRelease(&pDvcData->m_FingerprintLock);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwRemoveFingerprint -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPcieNwRemoveFingerprint(DtPcieNwDeviceData* pDvcData, UInt8 Fingerprint)
{
    DtStatus  Status = DT_STATUS_OK;
    DtSpinLockAcquire(&pDvcData->m_FingerprintLock);
    if (pDvcData->m_pFingerprintPackets[Fingerprint] == NULL)
    {
        // ERROR out of fingerprints
        DtDbgOut(ERR, DTPCIENW, "No fingerprint registered with ID: %i", Fingerprint);
        Status = DT_STATUS_FAIL;
    }
    else {
        pDvcData->m_pFingerprintPackets[Fingerprint] = NULL;
        pDvcData->m_NumTxTimestampRequests--;
    }
        
    DtSpinLockRelease(&pDvcData->m_FingerprintLock);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwRemoveFingerprint -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPcieNwCheckFingerprintTimeout(DtPcieNwDeviceData* pDvcData, Bool ForceTimeout)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8 Fingerprint;
    UInt64 CurrentTime;
    UInt8 FingerPrintsFound;
    if (pDvcData->m_NumTxTimestampRequests == 0)
        return Status;

    DtSpinLockAcquire(&pDvcData->m_FingerprintLock);
    // Start with the last used fingerprint
    Fingerprint = pDvcData->m_Fingerprint;
    CurrentTime = DtGetTickCount();
    FingerPrintsFound = 0;
    while (FingerPrintsFound != pDvcData->m_NumTxTimestampRequests)
    {
        if (pDvcData->m_pFingerprintPackets[Fingerprint] != NULL)
        {
            if (CurrentTime - pDvcData->m_FingerPrintTimestamp[Fingerprint] > 500 
                                                                          || ForceTimeout)
            {
                DtDbgOut(ERR, DTPCIENW, "No timestamp received for "
                             "fingerprint with ID: %i. Waited for: %llums", Fingerprint,
                             CurrentTime - pDvcData->m_FingerPrintTimestamp[Fingerprint]);
                DtPcieNwEvtHwTxTimestampNotReceived(pDvcData,
                                            pDvcData->m_pFingerprintPackets[Fingerprint]);
                pDvcData->m_pFingerprintPackets[Fingerprint] = NULL;
                pDvcData->m_NumTxTimestampRequests--;
            } else
                FingerPrintsFound++;
        }
        Fingerprint--;
        if (Fingerprint == 0)
            Fingerprint = MAX_FINGERPRINT;
    }
    DtSpinLockRelease(&pDvcData->m_FingerprintLock);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwIncomingFingerprint -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPcieNwIncomingFingerprint(DtPcieNwDeviceData* pDvcData, UInt8 Fingerprint, 
                                                 UInt32 TodSeconds, UInt32 TodNanoSeconds)
{
    DtStatus  Status = DT_STATUS_OK;
    DtSpinLockAcquire(&pDvcData->m_FingerprintLock);
    if (pDvcData->m_pFingerprintPackets[Fingerprint] == NULL)
    {
        // ERROR No packet with this fingerprint registered
        DT_ASSERT(FALSE);
        DtDbgOut(ERR, DTPCIENW, "No fingerprint registered with ID: %i", Fingerprint);
        Status = DT_STATUS_FAIL;
    }
    else {
        DtDbgOut(ERR, DTPCIENW, "Timestamp received for fingerprint with ID: %i %is,%ins", 
                                                 Fingerprint, TodSeconds, TodNanoSeconds);

        DtPcieNwEvtHwTxTimestampReceived(pDvcData, 
                                             pDvcData->m_pFingerprintPackets[Fingerprint],
                                             TodSeconds, TodNanoSeconds);
        pDvcData->m_pFingerprintPackets[Fingerprint] = NULL;
        pDvcData->m_NumTxTimestampRequests--;
    }
    DtSpinLockRelease(&pDvcData->m_FingerprintLock);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwRxThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtPcieNwRxThread(DtThread* pThread, void* pContext)
{
    DtPcieNwDeviceData* pDvcData = pContext;
    Bool  StopThread = FALSE;
    DtStatus  Status = DT_STATUS_OK;
    DtEthIpHeader*  pEthIpHeader = NULL;
    DtEthIpTod*  pTod = NULL;
    Int  BufLoad;
    Int  NumBytesToEnd;
    Int  Retry = 0;
    UInt  RxFlags;
    UInt8*  pPacket;
    UInt8*  pTmpBuffer = DtMemAllocPool(DtPoolNonPaged, DT_ETH_IP_MAX_PACKETSIZE +
                                                       sizeof(DtEthIp), DTPCIENW_MEM_TAG);
    DektecTlvs  DektecTlv;

    DtDbgOut(MAX, DTPCIENW, "Start");
    
    while (!StopThread)
    {
        // Wait for DMA done or stop
        Status = DtThreadWaitForStopOrEvent(pThread, &pDvcData->m_RxDataAvailableEvent);
        if (Status == DT_STATUS_CANCELLED)
        {
            StopThread = TRUE;
            continue;
        }

        // Get packet data
        // Update write pointer 
        DtPcieNwGetRxWriteOffsetIoCtrl(pDvcData);
        Retry = 0;
        
        DtSpinLockAcquire(&pDvcData->m_RxLock);
        while ((pDvcData->m_RxReadOffset != pDvcData->m_RxWriteOffsetCache) &&
                                                             !DtThreadShouldStop(pThread))
        {
            // Sanity check. Check if we are in operational mode and buffer is valid
            if (pDvcData->m_OperMode==DT_FUNC_OPMODE_IDLE ||
                                  pDvcData->m_pRxBuffer==NULL || pDvcData->m_RxBufSize==0)
            {
                // Stop packet processing
               pDvcData->m_RxReadOffset = pDvcData->m_RxWriteOffsetCache;
               continue;
            }
            RxFlags = 0;
            pPacket = pDvcData->m_pRxBuffer + pDvcData->m_RxReadOffset;
            
            BufLoad = GetRxBufLoad(pDvcData);

            // Datapath is minimal 64-bit aligned, so header must fit until end
            NumBytesToEnd = pDvcData->m_RxBufSize - pDvcData->m_RxReadOffset;

            pEthIpHeader = (DtEthIpHeader*)pPacket;
            if (pEthIpHeader->m_SizeInQWords * 8 > BufLoad)
            {
                if (pEthIpHeader->m_SyncWord == DT_ETHIP_SYNCWORD)
                {
                    DtDbgOut(ERR, DTPCIENW, "Not a complete packet received with DMA. %i "
                          "bytes received. Needs %i bytes. Wait for complete packet (%i)", 
                          BufLoad, pEthIpHeader->m_SizeInQWords * 8, Retry + 1);
                    DtSleep(1);
                    DtPcieNwGetRxWriteOffsetIoCtrl(pDvcData);
                    Retry++;
                    if (Retry < 10)
                        continue;
                } else
                    DtDbgOut(ERR, DTPCIENW, "Not a complete packet received with DMA. %i "
                                     "bytes received with HEADER SYNC ERROR!!!", BufLoad);
            }
            Retry = 0;

            // Sanity check
            DT_ASSERT(pEthIpHeader->m_SyncWord==DT_ETHIP_SYNCWORD 
                                              && NumBytesToEnd>= sizeof(DtEthIpHeader)
                                              && pEthIpHeader->m_SizeInQWords*8<=BufLoad);
            if (pEthIpHeader->m_SyncWord != DT_ETHIP_SYNCWORD 
                                                || NumBytesToEnd<sizeof(DtEthIpHeader) 
                                                || pEthIpHeader->m_SizeInQWords*8>BufLoad 
                                                || pEthIpHeader->m_SizeInQWords<2)
            {
                // Fatal error: clear RX buffer and log error.
                pDvcData->m_RxReadOffset = pDvcData->m_RxWriteOffsetCache;
                DtPcieNwSetRxReadOffsetIoCtrl(pDvcData, pDvcData->m_RxReadOffset);
                DtDbgOut(ERR, DTPCIENW, "Packet error RX. Sync word: %xh SizeInQWords:%i",
                                                            pEthIpHeader->m_SyncWord,
                                                            pEthIpHeader->m_SizeInQWords);

                DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, DTPCIENW_LOG_FATAL_ERROR, 
                                                                        NULL, NULL, NULL);
                DtPcieNwGetRxWriteOffsetIoCtrl(pDvcData);
                continue;
            }
            if (pEthIpHeader->m_SizeInQWords * 8 > NumBytesToEnd)
            {
                // Copy packet to temporarily buffer
                if (pEthIpHeader->m_SizeInQWords * 8 > DT_IP_MAX_PACKET_SIZE)
                {
                    DtDbgOut(ERR, DTPCIENW, "Packet error RX. Too large: %xh "
                                              "SizeInQWords:%i", pEthIpHeader->m_SyncWord, 
                                              pEthIpHeader->m_SizeInQWords);

                    DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, 
                                              DTPCIENW_LOG_FATAL_ERROR, NULL, NULL, NULL);
                    pDvcData->m_RxReadOffset = (pDvcData->m_RxReadOffset +
                              (pEthIpHeader->m_SizeInQWords * 8)) % pDvcData->m_RxBufSize;
                    DtPcieNwSetRxReadOffsetIoCtrl(pDvcData, pDvcData->m_RxReadOffset);
                    DtPcieNwGetRxWriteOffsetIoCtrl(pDvcData);
                    continue;
                }
                // Copy first part. It's at least the EthIpHeader
                DtMemCopy(pTmpBuffer, pPacket, NumBytesToEnd);
                DtMemCopy(pTmpBuffer+NumBytesToEnd, pDvcData->m_pRxBuffer,(UInt)
                                          pEthIpHeader->m_SizeInQWords*8 - NumBytesToEnd);
                pPacket = pTmpBuffer;
                pEthIpHeader = (DtEthIpHeader*)pPacket;
            }
            pPacket += sizeof(DtEthIpHeader);
            pTod = (DtEthIpTod*)pPacket;

            if (pEthIpHeader->m_PacketType == DT_ETHIP_PKTTYPE_TIMESTAMP)
            {
                // Special case
                DtPcieNwIncomingFingerprint(pDvcData, (UInt8)pEthIpHeader->m_Fingerprint,
                              (UInt32)pTod->m_TodSeconds, (UInt32)pTod->m_TodNanoseconds);

                pDvcData->m_RxReadOffset = (pDvcData->m_RxReadOffset +
                              (pEthIpHeader->m_SizeInQWords * 8)) % pDvcData->m_RxBufSize;
                DtPcieNwSetRxReadOffsetIoCtrl(pDvcData, pDvcData->m_RxReadOffset);

                // If buffer is empty, check if new data is received in the mean time
                if (pDvcData->m_RxReadOffset == pDvcData->m_RxWriteOffsetCache)
                    DtPcieNwGetRxWriteOffsetIoCtrl(pDvcData);
                continue;
            }
            
            if (!pDvcData->m_FCSEnabled)
                pEthIpHeader->m_SizeInBytes -= 4; //Strip FCS

            //pPacket += sizeof(DtDmaRxHeader);
            if (pEthIpHeader->m_PacketType == DT_ETHIP_PKTTYPE_IPV4 && 
                                                  pEthIpHeader->m_IpV4HdrChecksumError==0)
                RxFlags |= DTPCIENW_RXFLAGS_IP_CHKSUM_CORRECT;
            if (pEthIpHeader->m_Protocol == DT_ETHIP_PROTOCOL_UDP && 
                                                pEthIpHeader->m_UdpChecksumError == 0)
                RxFlags |= DTPCIENW_RXFLAGS_UDP_CHKSUM_CORRECT;
            if (pEthIpHeader->m_Protocol == DT_ETHIP_PROTOCOL_TCP && 
                                                    pEthIpHeader->m_PortOffset == 0 &&
                                                    pEthIpHeader->m_TcpChecksumError == 0)
                RxFlags |= DTPCIENW_RXFLAGS_TCP_CHKSUM_CORRECT; 
            if (pEthIpHeader->m_IpV4HdrChecksumError != 0)
                RxFlags |= DTPCIENW_RXFLAGS_IP_CHKSUM_FAIL;
            if (pEthIpHeader->m_UdpChecksumError != 0)
                RxFlags |= DTPCIENW_RXFLAGS_UDP_CHKSUM_FAIL;
            if (pEthIpHeader->m_TcpChecksumError != 0)
                RxFlags |= DTPCIENW_RXFLAGS_TCP_CHKSUM_FAIL;

            pPacket = ((UInt8*)pTod)+sizeof(DtEthIpTod) + 2; // +2 for alignment
            //pVlanHeader = (EthernetIIHeaderVlan*)pPacket;
            
            if (pEthIpHeader->m_PacketType == DT_ETHIP_PKTTYPE_IPV4 || 
                                      pEthIpHeader->m_PacketType == DT_ETHIP_PKTTYPE_IPV6)
                RxFlags |= DTPCIENW_RXFLAGS_IS_IP;

            // Just for debugging
            //ParsePtpPacketDebug((UInt8*)pEthIpHeader, pEthIpHeader->m_SizeInBytes, &PtpTimes);
            //DektecTlv.

            ParsePtpPacketRx((UInt8*)pEthIpHeader, (UInt)pEthIpHeader->m_SizeInQWords*8,
                                                                              &DektecTlv);

            DtPcieNwEvtNewPacketRxCallback(pDvcData, (UInt32)pTod->m_TodSeconds, 
               (UInt32)pTod->m_TodNanoseconds, pPacket, (UInt)pEthIpHeader->m_SizeInBytes, 
                                                                     RxFlags, &DektecTlv);

            pDvcData->m_RxReadOffset = (pDvcData->m_RxReadOffset + 
                                 (pEthIpHeader->m_SizeInQWords *8))%pDvcData->m_RxBufSize;
            DtPcieNwSetRxReadOffsetIoCtrl(pDvcData, pDvcData->m_RxReadOffset);
            
            // If buffer is empty, check if new data is received in the mean time
            if (pDvcData->m_RxReadOffset == pDvcData->m_RxWriteOffsetCache)
            {
                DtPcieNwGetRxWriteOffsetIoCtrl(pDvcData);
                DtPcieNwCheckFingerprintTimeout(pDvcData, FALSE);
            }
        }
        DtSpinLockRelease(&pDvcData->m_RxLock);

        if (DtThreadShouldStop(pThread))
            StopThread = TRUE;
    }

    // Free temp buffer
    DtMemFreePool(pTmpBuffer, DTPCIENW_MEM_TAG);
    DtPcieNwCheckFingerprintTimeout(pDvcData, TRUE);
    
    // We don't have to wait until the thread received a stop command. We only got here in
    // that case
    DtDbgOut(MAX, DTPCIENW, "Exit");
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwCreateThreads -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwCreateThreads(DtPcieNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    
    Status = DtThreadInit(&pDvcData->m_RxThread, DtPcieNwRxThread, pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwPowerup(DtPcieNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Status = DtThreadStart(&pDvcData->m_RxThread);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwPowerdown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtPcieNwPowerdown(DtPcieNwDeviceData* pDvcData)
{
    DtDbgOut(MIN, DTPCIENW, "Stopping Rx thread");
    DtThreadStop(&pDvcData->m_RxThread);
    DtDbgOut(MIN, DTPCIENW, "Rx thread stopped");
    
}


//=+=+=+=+=+=+=+=+=+=+=+ Callback functions for direct DtPcie calls +=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPciNwLinkStatusWi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtPciNwLinkStatusWi(DtWorkItemArgs* pArgs)
{
    DtPcieNwDeviceData* pDvcData = (DtPcieNwDeviceData*)pArgs->m_pContext;
    UInt32 PhyLinkStatus = pArgs->m_Data1.m_UInt32_1;
    UInt  MessageId;
    DtString  DtStrMacAddress;
    DtStringChar  DtStrMacAddressBuffer[20];

    // Connect DtStrVersionBuffer to DtStrVersion
    DT_STRING_INIT(DtStrMacAddress, DtStrMacAddressBuffer, 20);

    DtDbgOut(MAX, DTPCIENW, "Start");

    switch (PhyLinkStatus)
    {
    case DT_PHY_LINK_UP:
        pDvcData->m_LinkStatus = DTPCIENW_LINKSTAT_UP;
        MessageId = DTPCIENW_LOG_LINK_CONNECTED;
        break;
    default:
        pDvcData->m_LinkStatus = DTPCIENW_LINKSTAT_DOWN;
        MessageId = DTPCIENW_LOG_LINK_DISCONNECTED;
    }

    // Report Network stack of change
    DtPcieNwEvtLinkStatusCallback(pDvcData);

    // Report the link status event
    DtMacAddress2DtString(pDvcData->m_MacAddressCur, &DtStrMacAddress);
    DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, MessageId, &DtStrMacAddress, NULL,
                                                                                    NULL);
    DtDbgOut(MAX, DTPCIENW, "Exit");
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwLinkStatusCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtPcieNwLinkStatusCallback(UInt PhyLinkStatus, void* pContext)
{
    DtPcieNwDeviceData* pDvcData = pContext;
    DtWorkItemArgs DtWorkItemArgs;
    DtStatus Status;
    DtDbgOut(MAX, DTPCIENW, "Start");
    DtWorkItemArgs.m_pContext = pDvcData;
    DtWorkItemArgs.m_Data1.m_UInt32_1 = PhyLinkStatus;
    Status = DtWorkItemSchedule(&pDvcData->m_LinkStatusWorkItem, &DtWorkItemArgs);
    DT_ASSERT(Status == DT_STATUS_OK);
    DtDbgOut(MAX, DTPCIENW, "Exit");
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwOpModeCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtPcieNwOpModeCallback(UInt OpMode, void* pContext)
{
    DtPcieNwDeviceData* pDvcData = pContext;
    DtDbgOut(MAX, DTPCIENW, "Start");
    if (OpMode == DT_FUNC_OPMODE_IDLE)
    {
        // Prevent race condition with transmitting packets
        DtPcieNwAquireLock(pDvcData);
        pDvcData->m_OperMode = DT_FUNC_OPMODE_IDLE;
        DtPcieNwReleaseLock(pDvcData);
    }
    DtDbgOut(MAX, DTPCIENW, "Exit");
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helper functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwCheckDtDriverVersion -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwCheckDtDriverVersion(
    DtPcieNwDeviceData*  pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  DtPcieMajor;
    Int  DtPcieMinor;
    Int  DtPcieMicro;
    Bool  VersionError = FALSE;
    Status = DtPcieNwGetDtPcieDriverVersionIoCtrl(pDvcData, &DtPcieMajor, &DtPcieMinor,
                                                                            &DtPcieMicro);
    if (DT_SUCCESS(Status))
    {
        DtString  DtStrVersion;
        DtStringChar  DtStrVersionBuffer[32];
        DT_STRING_DECL(DtStrDot, ".");
    
        // Check driver version
        if (!VersionError && DtPcieMajor<DTPCIE_MIN_MAJOR) VersionError = TRUE;
        if (!VersionError && DtPcieMajor>DTPCIE_MIN_MAJOR) return DT_STATUS_OK;
        if (!VersionError && DtPcieMinor<DTPCIE_MIN_MINOR) VersionError = TRUE;
        if (!VersionError && DtPcieMinor>DTPCIE_MIN_MINOR) return DT_STATUS_OK;
        if (!VersionError && DtPcieMicro>=DTPCIE_MIN_MICRO) return DT_STATUS_OK;

        DT_ASSERT(VersionError);
        
        // Report error message
        Status = DT_STATUS_VERSION_MISMATCH;

        // Connect DtStrErrMsgBuffer to DtStrErrMsg
        DT_STRING_INIT(DtStrVersion, DtStrVersionBuffer, 32);

        // Build version
        
        DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTPCIE_MIN_MAJOR);
        DtStringAppendDtString(&DtStrVersion, &DtStrDot);
        DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTPCIE_MIN_MINOR);
        DtStringAppendDtString(&DtStrVersion, &DtStrDot);
        DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTPCIE_MIN_MICRO);
    
        DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, DTPCIENW_LOG_VERSION_MISMATCH,
                                                               &DtStrVersion, NULL, NULL);
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetStatisticCounterIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwGetStatisticCounterIoCtrl(
    DtPcieNwDeviceData*  pDvcData,
    UInt  StatisticId,
    UInt64*  pValue)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlEMACCmdGetCounterInput  IoCtrlInput;
    DtIoctlEMACCmdGetCounterOutput  IoCtrlOutput;
    UInt  SizeIn = sizeof(DtIoctlEMACCmdGetCounterInput);
    UInt  SizeOut = sizeof(DtIoctlEMACCmdGetCounterOutput);
    *pValue = 0;
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_EMAC_CMD_GET_COUNTER;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_CounterId = StatisticId;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, &IoCtrlInput, 
                                                            SizeOut, &IoCtrlOutput, NULL);
    if (!DT_SUCCESS(Status))
        return Status;
    *pValue = IoCtrlOutput.m_Value;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetMaxMulticastListIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwGetMaxMulticastListIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                  UInt* pMaxMulticastList)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlEMACCmdGetMaxMulticastListInput  IoCtrlInput;
    DtIoctlEMACCmdGetMaxMulticastListOutput  IoCtrlOutput;
    UInt  SizeIn = sizeof(DtIoctlEMACCmdGetMaxMulticastListInput);
    UInt  SizeOut = sizeof(DtIoctlEMACCmdGetMaxMulticastListOutput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_EMAC_CMD_GET_MAX_MULTICAST_LIST;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, &IoCtrlInput,
                                                            SizeOut, &IoCtrlOutput, NULL);
    if (!DT_SUCCESS(Status))
        return Status;
    *pMaxMulticastList = IoCtrlOutput.m_MaxListSize;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetMacAddressIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwSetMacAddressIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                       UInt8* pMacAddress)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlEMACCmdSetMacAddressInput  IoCtrlInput;
    UInt  SizeIn = sizeof(DtIoctlEMACCmdSetMacAddressInput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_EMAC_CMD_SET_MACADDRESS;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    
    DtMemCopy(IoCtrlInput.m_Address, pMacAddress, sizeof(IoCtrlInput.m_Address));
    
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, &IoCtrlInput,
                                                                           0, NULL, NULL);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetMacAddressPermIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwGetMacAddressPermIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                       UInt8* pMacAddress)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlEMACCmdGetMacAddressPermInput  IoCtrlInput;
    DtIoctlEMACCmdGetMacAddressPermOutput  IoCtrlOutput;
    UInt  SizeIn = sizeof(DtIoctlEMACCmdGetMacAddressPermInput);
    UInt  SizeOut = sizeof(DtIoctlEMACCmdGetMacAddressPermOutput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_EMAC_CMD_GET_MACADDRESS_PERM;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, &IoCtrlInput,
                                                            SizeOut, &IoCtrlOutput, NULL);
    if (!DT_SUCCESS(Status))
        return Status;

    DtMemCopy(pMacAddress, IoCtrlOutput.m_Address, sizeof(IoCtrlOutput.m_Address));
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetPacketFilterIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwGetPacketFilterIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                      UInt* pPacketFilter)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlEMACCmdGetPacketFilterInput  IoCtrlInput;
    DtIoctlEMACCmdGetPacketFilterOutput  IoCtrlOutput;
    UInt  SizeIn = sizeof(DtIoctlEMACCmdGetPacketFilterInput);
    UInt  SizeOut = sizeof(DtIoctlEMACCmdGetPacketFilterOutput);
    *pPacketFilter = 0;
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_EMAC_CMD_GET_PACKET_FILTER;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, &IoCtrlInput,
                                                            SizeOut, &IoCtrlOutput, NULL);
    if (!DT_SUCCESS(Status))
        return Status;
    *pPacketFilter = IoCtrlOutput.m_PacketFilter;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetPacketFilterIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwSetPacketFilterIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                        UInt PacketFilter)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlEMACCmdSetPacketFilterInput  IoCtrlInput;
    UInt  SizeIn = sizeof(DtIoctlEMACCmdSetPacketFilterInput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_EMAC_CMD_SET_PACKET_FILTER;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_PacketFilter = PacketFilter;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, &IoCtrlInput, 
                                                                           0, NULL, NULL);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetDeviceInfoIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwGetDeviceInfoIoCtrl(
    DtPcieNwDeviceData*  pDvcData,
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
    DtIoctlGetDevInfoInput  IoCtrlInput;
    DtIoctlGetDevInfoOutput  IoCtrlOutput;

    UInt  SizeIn = sizeof(DtIoctlGetDevInfoInput);
    UInt  SizeOut = sizeof(DtIoctlGetDevInfoOutput);
    IoCtrlInput.m_PortIndex = -1;
    IoCtrlInput.m_Uuid = DT_UUID_CORE;
    IoCtrlInput.m_Cmd = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_CmdEx = DT_IOCTL_CMD_NOP;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_GET_DEV_INFO, SizeIn, 
                                              &IoCtrlInput, SizeOut, &IoCtrlOutput, NULL);
    if (!DT_SUCCESS(Status))
        return Status;

    if (pDeviceId != NULL)
        *pDeviceId = IoCtrlOutput.m_DeviceId;
    if (pVendorId != NULL)
        *pVendorId = IoCtrlOutput.m_VendorId;
    if (pSubVendorId != NULL)
        *pSubVendorId = IoCtrlOutput.m_SubVendorId;
    if (pSubSystemId != NULL)
        *pSubSystemId = IoCtrlOutput.m_SubSystemId;
    if (pBusNumber != NULL)
        *pBusNumber = IoCtrlOutput.m_DevSpecific.m_Pcie.m_BusNumber;
    if (pSlotNumber != NULL)
        *pSlotNumber = IoCtrlOutput.m_DevSpecific.m_Pcie.m_SlotNumber;
    if (pTypeNumber != NULL)
        *pTypeNumber = IoCtrlOutput.m_TypeNumber;
    if (pHardwareRevision != NULL)
        *pHardwareRevision = IoCtrlOutput.m_HardwareRevision;
    if (pFirmwareVersion != NULL)
        *pFirmwareVersion = IoCtrlOutput.m_FirmwareVersion;
    if (pFirmwareVariant != NULL)
        *pFirmwareVariant = IoCtrlOutput.m_FirmwareVariant;
    if (pSerial != NULL)
        *pSerial = IoCtrlOutput.m_Serial;
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetPhySpeedIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwGetPhySpeedIoCtrl(DtPcieNwDeviceData*  pDvcData, UInt*  pSpeed)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlEMACCmdGetPhySpeedInput  IoCtrlInput;
    DtIoctlEMACCmdGetPhySpeedOutput  IoCtrlOutput;
    UInt  SizeIn = sizeof(DtIoctlEMACCmdGetPhySpeedInput);
    UInt  SizeOut = sizeof(DtIoctlEMACCmdGetPhySpeedOutput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_EMAC_CMD_GET_PHY_SPEED;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, &IoCtrlInput,
                                                            SizeOut, &IoCtrlOutput, NULL);
    if (!DT_SUCCESS(Status))
        return Status;
    *pSpeed = IoCtrlOutput.m_Speed;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetLinkStatusIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwGetLinkStatusIoCtrl(DtPcieNwDeviceData* pDvcData)
{
    DtIoctlEMACCmdGetLinkStatusInput  IoCtrlInput;
    UInt  SizeIn = sizeof(DtIoctlEMACCmdGetLinkStatusInput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_EMAC_CMD_GET_LINKSTATUS;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;

    return DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, &IoCtrlInput,
                                                                           0, NULL, NULL);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetPhySpeedIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwSetPhySpeedIoCtrl(DtPcieNwDeviceData* pDvcData, UInt Speed)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlEMACCmdSetPhySpeedInput  IoCtrlInput;

    UInt  SizeIn = sizeof(DtIoctlEMACCmdSetPhySpeedInput);
    
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_EMAC_CMD_SET_PHY_SPEED;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_Speed = Speed;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, &IoCtrlInput,
                                                                           0, NULL, NULL); 
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwRegisterCallbackIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwRegisterCallbackIoCtrl(DtPcieNwDeviceData* pDvcData,
                                                            LinkStatFunc LinkStatCallback)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlEMACCmdRegisterCallbackInput  IoCtrlInput;

    UInt  SizeIn = sizeof(DtIoctlEMACCmdRegisterCallbackInput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_EMAC_CMD_REGISTER_CALLBACK;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_pContext = pDvcData;
    IoCtrlInput.m_LinkStatusCallback = LinkStatCallback;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, &IoCtrlInput, 
                                                                           0, NULL, NULL);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwUnregisterCallbackIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwUnregisterCallbackIoCtrl(DtPcieNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlEMACCmdUnregisterCallbackInput  IoCtrlInput;
    UInt  SizeIn = sizeof(DtIoctlEMACCmdUnregisterCallbackInput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_EMAC_CMD_UNREGISTER_CALLBACK;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_pContext = pDvcData;
    
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, &IoCtrlInput, 
                                                                           0, NULL, NULL);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwRegisterOpModeCallbackIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwRegisterOpModeCallbackIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                               OpModeFunc  OpModeCallback)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlNwCmdRegisterOpModeCallbackInput  IoCtrlInput;

    UInt  SizeIn = sizeof(DtIoctlNwCmdRegisterOpModeCallbackInput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_NW_CMD_REGISTER_OPMODE_CALLBACK;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_pContext = pDvcData;
    IoCtrlInput.m_OpModeCallback = OpModeCallback;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_NW_CMD, SizeIn, &IoCtrlInput,
                                                                           0, NULL, NULL);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwResetNwStatusIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwResetNwStatusIoCtrl(DtPcieNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlEMACCmdResetNwStatusInput  IoCtrlInput;
    UInt  SizeIn = sizeof(DtIoctlEMACCmdResetNwStatusInput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_EMAC_CMD_RESET_NW_STATUS;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, &IoCtrlInput,
                                                                           0, NULL, NULL);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetDriverVersionIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwSetDriverVersionIoCtrl(DtPcieNwDeviceData* pDvcData)
{

    DtStatus  Status = DT_STATUS_OK;
    DtIoctlSetNwDriverVersionInput  IoCtrlInput;
    UInt  SizeIn = sizeof(DtIoctlSetNwDriverVersionInput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = -1;
    IoCtrlInput.m_CmdHdr.m_Uuid = DT_UUID_CORE;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;

    IoCtrlInput.m_Major = DTPCIENW_VERSION_MAJOR;
    IoCtrlInput.m_Minor = DTPCIENW_VERSION_MINOR;
    IoCtrlInput.m_Micro = DTPCIENW_VERSION_MICRO;
    IoCtrlInput.m_Build = DTPCIENW_VERSION_BUILD;
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_SET_NWDRIVER_VERSION,
                                                     SizeIn, &IoCtrlInput, 0, NULL, NULL);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwOpenPipe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwOpenPipe(DtPcieNwDeviceData* pDvcData, Int PipeType, 
                                                                          UInt* pPipeUuid)
{
    DtStatus Status = DT_STATUS_OK;
    DtIoctlNwCmdPipeOpenInput IoCtrlInput;
    DtIoctlNwCmdPipeOpenOutput IoCtrlOutput;

    UInt  SizeIn = sizeof(DtIoctlNwCmdPipeOpenInput);
    UInt  SizeOut = sizeof(DtIoctlNwCmdPipeOpenOutput);

    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_NW_CMD_PIPE_OPEN;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_PipeType = PipeType;
    IoCtrlInput.m_PipeTypeFallback = -1;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_NW_CMD, SizeIn, &IoCtrlInput,
                                                            SizeOut, &IoCtrlOutput, NULL);
    if (DT_SUCCESS(Status))
        *pPipeUuid = IoCtrlOutput.m_PipeUuid;
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwClosePipe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwClosePipe(DtPcieNwDeviceData* pDvcData, Int PipeType, 
                                                                            UInt PipeUuid)
{
    DtIoctlNwCmdPipeCloseInput IoCtrlInput;
    
    UInt  SizeIn = sizeof(DtIoctlNwCmdPipeOpenInput);
    
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = PipeUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_NW_CMD_PIPE_CLOSE;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;

    return DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_NW_CMD, SizeIn, &IoCtrlInput,
                                                                           0, NULL, NULL);
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetSharedKBufferIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus DtPcieNwSetSharedKBufferIoCtrl(DtPcieNwDeviceData* pDvcData, 
    UInt PipeUuid, UInt ReqBufSize, UInt8**ppBuffer, UInt* pActualBufSize, UInt* pMaxLoad)
{
    DtStatus Status = DT_STATUS_OK;
    DtIoctlPipeCmdSetSharedKBufferInput IoCtrlInput;
    DtIoctlPipeCmdSetSharedKBufferOutput IoCtrlOutput;

    UInt  SizeIn = sizeof(DtIoctlPipeCmdSetSharedKBufferInput);
    UInt  SizeOut = sizeof(DtIoctlPipeCmdSetSharedKBufferOutput);

    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = PipeUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_PIPE_CMD_SET_SHAREDK_BUFFER;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_RequestedBufferSize = ReqBufSize;
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_PIPE_CMD, SizeIn, &IoCtrlInput,
                                                            SizeOut, &IoCtrlOutput, NULL);
    if (DT_SUCCESS(Status))
    {
        *ppBuffer = (UInt8*)IoCtrlOutput.m_pBufferAddr;
        *pActualBufSize = IoCtrlOutput.m_ActualBufferSize;
        if (pMaxLoad != NULL)
            *pMaxLoad = IoCtrlOutput.m_MaxLoad;
    }
    else {
        *ppBuffer = NULL;
        *pActualBufSize = 0;
    }
    return Status;
}

/*
// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetSharedBufferRxIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwGetSharedBufferTxIoCtrl(DtPcieNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlNwCmdGetTxBufferInput  IoCtrlInput;
    DtIoctlNwCmdGetTxBufferOutput  IoCtrlOutput;

    UInt  SizeIn = sizeof(DtIoctlNwCmdGetTxBufferInput);
    UInt  SizeOut = sizeof(DtIoctlNwCmdGetTxBufferOutput);

    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_NW_CMD_GET_TX_BUFFER;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_NW_CMD, SizeIn, &IoCtrlInput,
                                                            SizeOut, &IoCtrlOutput, NULL);
    if (DT_SUCCESS(Status))
    {
        //pDvcData->m_AlignedPayload = 0;
        pDvcData->m_pTxBuffer = IoCtrlOutput.m_pBuffer;
        pDvcData->m_TxBufSize = IoCtrlOutput.m_BufferSize;
        pDvcData->m_TxMaxLoad = IoCtrlOutput.m_MaxBufferLoad;
    }
    return Status;
}*/

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetRxWriteOffsetIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwGetRxWriteOffsetIoCtrl(DtPcieNwDeviceData* pDvcData)
{
    DtStatus Status = DT_STATUS_OK;
    DtIoctlPipeCmdGetRxWriteOffsetInput IoCtrlInput;
    DtIoctlPipeCmdGetRxWriteOffsetOutput IoCtrlOutput;

    UInt  SizeIn = sizeof(DtIoctlPipeCmdGetRxWriteOffsetInput);
    UInt  SizeOut = sizeof(DtIoctlPipeCmdGetRxWriteOffsetOutput);

    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_RxUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_PIPE_CMD_GET_RX_WRITE_OFFSET;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_PIPE_CMD, SizeIn, &IoCtrlInput,
                                                            SizeOut, &IoCtrlOutput, NULL);
    if (DT_SUCCESS(Status))
        pDvcData->m_RxWriteOffsetCache = IoCtrlOutput.m_RxWriteOffset;
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetRxReadOffsetIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwSetRxReadOffsetIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                           Int ReadOffset)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlPipeCmdSetRxReadOffsetInput IoCtrlInput;

    UInt  SizeIn = sizeof(DtIoctlPipeCmdSetRxReadOffsetInput);

    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_RxUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_PIPE_CMD_SET_RX_READ_OFFSET;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_RxReadOffset = ReadOffset;
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_PIPE_CMD, SizeIn, &IoCtrlInput,
                                                                           0, NULL, NULL);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetTxWriteOffsetIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwGetTxReadOffsetIoCtrl(DtPcieNwDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlPipeCmdGetTxReadOffsetInput  IoCtrlInput;
    DtIoctlPipeCmdGetTxReadOffsetOutput  IoCtrlOutput;

    UInt  SizeIn = sizeof(DtIoctlPipeCmdGetTxReadOffsetInput);
    UInt  SizeOut = sizeof(DtIoctlPipeCmdGetTxReadOffsetOutput);

    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_TxUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_PIPE_CMD_GET_TX_READ_OFFSET;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;

    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_PIPE_CMD, SizeIn, &IoCtrlInput,
        SizeOut, &IoCtrlOutput, NULL);
    if (DT_SUCCESS(Status))
        pDvcData->m_TxReadOffsetCache = IoCtrlOutput.m_TxReadOffset;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetTxWriteOffsetIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwSetTxWriteOffsetIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                          Int WriteOffset)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlPipeCmdSetTxWriteOffsetInput IoCtrlInput;

    UInt  SizeIn = sizeof(DtIoctlPipeCmdSetTxWriteOffsetInput);

    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_TxUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_PIPE_CMD_SET_TX_WRITE_OFFSET;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_TxWriteOffset = WriteOffset;
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_PIPE_CMD, SizeIn, &IoCtrlInput,
                                                                           0, NULL, NULL);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetMulticastListIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwSetMulticastListIoCtrl(
    DtPcieNwDeviceData*  pDvcData,
    UInt  NumItems,
    void*  pMulticastList)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlEMACCmdSetMulticastListInput*  pIoCtrlInput;
    
    UInt  SizeIn = sizeof(DtIoctlEMACCmdSetMulticastListInput);
    SizeIn += (NumItems * 6) + sizeof(UInt);

    pIoCtrlInput = DtMemAllocPool(DtPoolNonPaged, SizeIn, DTPCIENW_MEM_TAG);
    if (pIoCtrlInput == NULL)
        return DT_STATUS_OUT_OF_MEMORY;
    pIoCtrlInput->m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    pIoCtrlInput->m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    pIoCtrlInput->m_CmdHdr.m_Cmd = DT_EMAC_CMD_SET_MULTICAST_LIST;
    pIoCtrlInput->m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    pIoCtrlInput->m_NumItems = NumItems;
    DtCopyMulticastListItems(NumItems, pMulticastList, (UInt8*)pIoCtrlInput->m_Items);
    
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EMAC_CMD, SizeIn, pIoCtrlInput, 
                                                                           0, NULL, NULL);
    DtMemFreePool(pIoCtrlInput, DTPCIENW_MEM_TAG);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetDtPcieDriverVersionIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwGetDtPcieDriverVersionIoCtrl(
    DtPcieNwDeviceData*  pDvcData,
    Int*  pMajor,
    Int*  pMinor,
    Int*  pMicro)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlGetDriverVersionInput IoCtrlInput;
    DtIoctlGetDriverVersionOutput  IoCtrlOutput;

    UInt  SizeIn = sizeof(DtIoctlGetDriverVersionInput);
    UInt  SizeOut = sizeof(DtIoctlGetDriverVersionOutput);

    IoCtrlInput.m_Cmd = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_Uuid = 0;
    IoCtrlInput.m_PortIndex = -1;

    // IO output data 
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_GET_DRIVER_VERSION,
                                      SizeIn, &IoCtrlInput, SizeOut, &IoCtrlOutput, NULL);
    if(DT_SUCCESS(Status))
    {
        *pMajor = IoCtrlOutput.m_Major;
        *pMinor = IoCtrlOutput.m_Minor;
        *pMicro = IoCtrlOutput.m_Micro;
        //IoCtrlOutput.m_Build
    } else
    {
        *pMajor = -1;
        *pMinor = -1;
        *pMicro = -1;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- GetPropertyNwUuId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Pre: TypeNumber must be filled
//
static DtStatus GetNwUuId(DtPcieNwDeviceData* pDvcData, Int* pUuId)
{
    return GetPropertyIntIoctl(pDvcData, DT_DF_NW_UUID_NAME, pDvcData->m_PortIndex,
                                                                                   pUuId);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- GetPropertyIntIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus GetPropertyIntIoctl(DtPcieNwDeviceData* pDvcData, char* pName, 
                                                                 Int PortIndex, Int* pInt)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoctlPropCmdGetValueInput  IoCtrlInput;
    DtIoctlPropCmdGetValueOutput  IoCtrlOutput;
    UInt  SizeIn = sizeof(DtIoctlPropCmdGetValueInput);
    UInt  SizeOut = sizeof(DtIoctlPropCmdGetValueOutput);
    
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_PROP_CMD_GET_VALUE; 
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_CmdHdr.m_PortIndex = -1;
    IoCtrlInput.m_CmdHdr.m_Uuid = DT_UUID_CORE;

    // Copy input parameters
    IoCtrlInput.m_TypeNumber = pDvcData->m_TypeNumber;
    // NOTE: old Int DtIoctlPropCmdCommonInput::m_SubDvc field was split into two Int16 
    // fields m_SubDvc and m_SubType. m_SubDvc is no longer used and only exists for 
    // backward compatbility. Now the field must be set to -1 by definition.

    IoCtrlInput.m_SubDvc = -1;
    IoCtrlInput.m_SubType = -1;
    IoCtrlInput.m_HardwareRevision = 0;
    IoCtrlInput.m_FirmwareVersion = 0;
    IoCtrlInput.m_FirmwareVariant = 0;
    IoCtrlInput.m_PortIndex = PortIndex;
    IoCtrlInput.m_DtapiMaj = -1;
    IoCtrlInput.m_DtapiMin = -1;
    IoCtrlInput.m_DtapiBugfix = -1;
    if ((strlen(pName) + 1) > PROPERTY_NAME_MAX_SIZE)
        return DT_STATUS_BUF_TOO_SMALL;
    strcpy(IoCtrlInput.m_Name, pName);

    // Perform IO control
    Status = DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_PROPERTY_CMD, SizeIn, 
                                              &IoCtrlInput, SizeOut, &IoCtrlOutput, NULL);
    if (DT_SUCCESS(Status))
    {

        // Property must have scope DTAPI and property type must match expected type
        DT_ASSERT((IoCtrlOutput.m_Scope & PROPERTY_SCOPE_DTAPI) != 0);
        DT_ASSERT(IoCtrlOutput.m_Type == PROPERTY_VALUE_TYPE_INT);

        // Get the value
        *pInt = (Int)IoCtrlOutput.m_Value;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- RequestExclusiveAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwAcquireExclusiveAccess(DtPcieNwDeviceData* pDvcData)
{
    DtIoctlExclAccessCmdInput IoCtrlInput;

    UInt  SizeIn = sizeof(DtIoctlExclAccessCmdInput);

    IoCtrlInput.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_Cmd = DT_EXCLUSIVE_ACCESS_CMD_ACQUIRE;
    IoCtrlInput.m_CmdEx = DT_IOCTL_CMD_NOP;
    return DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EXCL_ACCESS_CMD, SizeIn, 
                                                             &IoCtrlInput, 0, NULL, NULL);

}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ReleaseExclusiveAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwReleaseExclusiveAccess(DtPcieNwDeviceData* pDvcData)
{
    DtIoctlExclAccessCmdInput IoCtrlInput;

    UInt  SizeIn = sizeof(DtIoctlExclAccessCmdInput);

    IoCtrlInput.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_Cmd = DT_EXCLUSIVE_ACCESS_CMD_RELEASE;
    IoCtrlInput.m_CmdEx = DT_IOCTL_CMD_NOP;
    return DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_EXCL_ACCESS_CMD, SizeIn, 
                                                             &IoCtrlInput, 0, NULL, NULL);

}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetOperationalModeIoCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwSetOperationalModeIoCtrl(DtPcieNwDeviceData* pDvcData, 
                                                                               Int OpMode)
{
    DtIoctlNwCmdSetOpModeInput IoCtrlInput;

    UInt  SizeIn = sizeof(DtIoctlNwCmdSetOpModeInput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_NwUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_NW_CMD_SET_OPERATIONAL_MODE;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_OpMode = OpMode;
    return DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_NW_CMD, SizeIn, &IoCtrlInput,  0, 
                                                                              NULL, NULL);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPciNwRegisterRxDataEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwRegisterRxDataEvent(DtPcieNwDeviceData* pDvcData)
{
    DtIoctlPipeCmdRegisterEventInput IoCtrlInput;

    UInt  SizeIn = sizeof(DtIoctlPipeCmdRegisterEventInput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_RxUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_PIPE_CMD_REGISTER_EVENT;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_Count = 1;
    IoCtrlInput.m_TrigEvt.m_Id = DT_PIPE_EVENT_ID_DATA_AVAIL_CNT;
    IoCtrlInput.m_TrigEvt.m_KernelEvent = TRUE;
    IoCtrlInput.m_TrigEvt.m_pKernelEvent = &pDvcData->m_RxDataAvailableEvent;
    return DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_PIPE_CMD, SizeIn, &IoCtrlInput,
                                                                           0, NULL, NULL);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPciNwUnregisterRxDataEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtPcieNwUnregisterRxDataEvent(DtPcieNwDeviceData* pDvcData)
{
    DtIoctlPipeCmdDeregisterEventInput IoCtrlInput;

    UInt  SizeIn = sizeof(DtIoctlPipeCmdDeregisterEventInput);
    IoCtrlInput.m_CmdHdr.m_PortIndex = pDvcData->m_PortIndex;
    IoCtrlInput.m_CmdHdr.m_Uuid = pDvcData->m_RxUuid;
    IoCtrlInput.m_CmdHdr.m_Cmd = DT_PIPE_CMD_DEREGISTER_EVENT;
    IoCtrlInput.m_CmdHdr.m_CmdEx = DT_IOCTL_CMD_NOP;
    IoCtrlInput.m_TrigEvt.m_Id = DT_PIPE_EVENT_ID_DATA_AVAIL_CNT;
    IoCtrlInput.m_TrigEvt.m_pKernelEvent = &pDvcData->m_RxDataAvailableEvent;
    return DtIoctl(&pDvcData->m_IoCtrlParent, DT_IOCTL_PIPE_CMD, SizeIn, &IoCtrlInput,
                                                                           0, NULL, NULL);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwAquireLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtPcieNwAquireLock(DtPcieNwDeviceData* pDvcData)
{
    DtSpinLockAcquire(&pDvcData->m_RxLock);
    DtPcieNwAquireLockIal(pDvcData);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwReleaseLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtPcieNwReleaseLock(DtPcieNwDeviceData* pDvcData)
{
    DtPcieNwReleaseLockIal(pDvcData);
    DtSpinLockRelease(&pDvcData->m_RxLock);
}

