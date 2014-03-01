//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaNw.h *#*#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// DtaNw driver - Interface for the DtaNw common driver, used by the IAL.
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

#ifndef __DTANW_H
#define __DTANW_H

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Defines +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA log levels -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// IAL related
#define  LOG_LEVEL_IAL          LOG_AVG
// Toplevel related
#define  LOG_LEVEL_DTANW        LOG_AVG


// Minimum version of Dta driver with which DtaNw can work
#define  DTA_MIN_MAJOR       4
#define  DTA_MIN_MINOR       0
#define  DTA_MIN_MICRO       0


#define  DTANW_DRIVER_VERSION   ((UInt)(DTANW_VERSION_MAJOR<<16) + DTANW_VERSION_MINOR)

#define  DTANW_VENDOR_DESC      "DekTec"
#define  DTANW_VENDOR_ID        0x0014F400

#define  DTANW_MEM_TAG          0x5444574E  /* 'TDWN' */

// Maximum network packet size in bytes, excluding the DMA header.
#define  DTANW_MAX_ETH_PAYLOAD  DT_IP_MAX_ETH_PAYLOAD
#define  DTANW_MAX_LOOKAHEAD    DTANW_MAX_ETH_PAYLOAD
#define  DTANW_MAX_PACKET_SIZE  DT_IP_MAX_PACKET_SIZE


#define  DTANW_LINKSTAT_DOWN    0
#define  DTANW_LINKSTAT_UP      1

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTANW Device Context -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

struct _DtaNwDeviceData
{
    // IAL data
    DtaNwIalData  m_IalData;
    DtIoCtrlParent  m_IoCtrlParent;

    DtDvcObject  m_Device;

    Int  m_TypeNumber;
    Int  m_BusNumber;
    Int  m_SlotNumber;
    Int  m_IpPortIndex;

    Int  m_MaxBufferSize;
    Bool  m_Support8021P_Priority;
    Bool  m_Supports8021Q_Vlan;
    Bool  m_DeviceOpened;

    // MAC-addresses
    Bool  m_MacAddressOverride;
    UInt8  m_MacAddressCur[6];          // Current
    UInt8  m_MacAddressPerm[6];         // Permanent
    
    UInt  m_LinkStatus;                 // Link Status: UP/DOWN
    UInt  m_PhySpeedSet;                // Speed/Duplex setting
    Bool  m_PromiscuousModeSet;         // Promiscuous mode setting

    Bool  m_AlignedPayload;             // TRUE if payload is/must be aligned at DWORD
                                        // boundaries
    UInt  m_MaxNumMulticastItems;
    
    // Shared buffer
    UInt8*  m_pTxBuffer;
    UInt8*  m_pRxBuffer;
    UInt  m_TxBufSize;
    UInt  m_RxBufSize;
    DtaIpNwSharedBufInfo*  m_pTxSharedBufInfo;
    DtaIpNwSharedBufInfo*  m_pRxSharedBufInfo;

    // Threads
    DtThread  m_RxThread;
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTANW public interface -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

DtStatus  DtaNwDriverInit(DtEvtLog* pEvtObject);
DtStatus  DtaNwDeviceOpen(DtaNwDeviceData* pDvcData);
DtStatus  DtaNwDeviceDataInit(DtaNwDeviceData* pDvcData);
DtStatus  DtaNwDeviceInit(DtaNwDeviceData* pDvcData);
void  DtaNwDriverExit(void);
void  DtaNwDeviceClose(DtaNwDeviceData* pDvcData);
void  DtaNwDeviceExit(DtaNwDeviceData* pDvcData);
DtStatus  DtaNwSetMacAddress(DtaNwDeviceData* pDvcData, UInt8* pMacAddress);
DtStatus  DtaNwGetMacAddressPerm(DtaNwDeviceData* pDvcData, UInt8* pMacAddress);
DtStatus  DtaNwGetMaxMulticastList(DtaNwDeviceData* pDvcData, UInt* pMaxMulticastList);
DtStatus  DtaNwGetSpeed(DtaNwDeviceData* pDvcData, UInt* pSpeed);
DtStatus  DtaNwGetStatisticCounter(DtaNwDeviceData* pDvcData, UInt StatisticId, 
                                                                          UInt64* pValue);
DtStatus  DtaNwGetPacketFilter(DtaNwDeviceData* pDvcData, UInt* pPacketFilter);
DtStatus  DtaNwSetPacketFilter(DtaNwDeviceData* pDvcData, UInt PacketFilter);
DtStatus  DtaNwSetPhySpeed(DtaNwDeviceData* pDvcData, UInt PhySpeed);
DtStatus  DtaNwGetPhySpeed(DtaNwDeviceData* pDvcData, UInt* pPhySpeed);
DtStatus  DtaNwGetLinkStatus(DtaNwDeviceData* pDvcData, UInt* pPhyLink);
DtStatus  DtaNwTransmitPacket(DtaNwDeviceData* pDvcData, UInt PacketSize, 
                                                                        UInt WriteOffset);
DtStatus  DtaNwTxGetPointerNewPacket(DtaNwDeviceData* pDvcData, UInt PacketSize, 
                                                     UInt8** pPacket, UInt* pWriteOffset);
DtStatus  DtaNwSetMulticastList(DtaNwDeviceData* pDvcData, UInt NumItems,
                                                                    void* pMulticastList);
#endif // __DTANW_H
