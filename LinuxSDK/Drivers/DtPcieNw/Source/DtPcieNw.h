// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPcieNw.h *#*#*#*#*#*#*#*#* (C) 2011-2021 DekTec
//
// DtPcieNw driver - Interface for the DtPcieNw common driver, used by the IAL.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2016 DekTec Digital Video B.V.
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

#ifndef __DtPcieNw_H
#define __DtPcieNw_H

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Defines +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA log levels -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// IAL related
#define  LOG_LEVEL_IAL          LOG_AVG
// Toplevel related
#define  LOG_LEVEL_DTPCIENW     LOG_AVG


// Minimum version of DtPcie driver with which DtPcieNw can work
#define  DTPCIE_MIN_MAJOR       1
#define  DTPCIE_MIN_MINOR       7
#define  DTPCIE_MIN_MICRO       10

#define DT_DF_NW_UUID_NAME       "DF_NW#2_UUID"

#define  DTPCIENW_DRIVER_VERSION   ((UInt)(DTPCIENW_VERSION_MAJOR<<16) + DTPCIENW_VERSION_MINOR)

#define  DTPCIENW_VENDOR_DESC      "DekTec"
#define  DTPCIENW_VENDOR_ID        0x0014F400

#define  DTPCIENW_MEM_TAG          0x5444574E  /* 'TDWN' */

// Maximum network packet size in bytes, excluding the DMA header.
#define  DTPCIENW_MAX_ETH_PAYLOAD  DT_IP_MAX_ETH_PAYLOAD
#define  DTPCIENW_MAX_LOOKAHEAD    DTPCIENW_MAX_ETH_PAYLOAD
#define  DTPCIENW_MAX_PACKET_SIZE  DT_IP_MAX_PACKET_SIZE


#define  DTPCIENW_LINKSTAT_DOWN    0
#define  DTPCIENW_LINKSTAT_UP      1

#define  DTPCIENW_TXFLAGS_GEN_CHKSUM_UDP    0x001
#define  DTPCIENW_TXFLAGS_GEN_CHKSUM_IP     0x002
#define  DTPCIENW_TXFLAGS_GEN_CHKSUM_TCP    0x004
#define  DTPCIENW_TXFLAGS_GEN_TX_TIMESTAMP  0x008
#define  DTPCIENW_TXFLAGS_PKTTYPE_IPV4      0x010
#define  DTPCIENW_TXFLAGS_PKTTYPE_IPV6      0x020
//#define  DTPCIENW_TXFLAGS_PKTTYPE_OTHER     0x040
#define  DTPCIENW_TXFLAGS_PROT_UDP          0x100
#define  DTPCIENW_TXFLAGS_PROT_TCP          0x200
//#define  DTPCIENW_TXFLAGS_PROT_OTHER        0x400

#define  DTPCIENW_RXFLAGS_IS_IP 0x01
#define  DTPCIENW_RXFLAGS_IP_CHKSUM_FAIL 0x02
#define  DTPCIENW_RXFLAGS_UDP_CHKSUM_FAIL 0x04
#define  DTPCIENW_RXFLAGS_TCP_CHKSUM_FAIL 0x08
#define  DTPCIENW_RXFLAGS_XXXX 0x10
#define  DTPCIENW_RXFLAGS_IP_CHKSUM_CORRECT 0x20
#define  DTPCIENW_RXFLAGS_UDP_CHKSUM_CORRECT 0x40
#define  DTPCIENW_RXFLAGS_TCP_CHKSUM_CORRECT 0x80

#define RX_NRT_SWQ_BUFSIZE        (4*1024*1024)
#define TX_NRT_SWQ_BUFSIZE        (4*1024*1024)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNw Device Context -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define MAX_FINGERPRINT  0x3F
struct _DtPcieNwDeviceData
{
    // IAL data
    DtPcieNwIalData  m_IalData;
    DtIoCtrlParent  m_IoCtrlParent;

    DtDvcObject  m_Device;
    Int  m_OperMode;

    Int  m_TypeNumber;
    Int  m_BusNumber;
    Int  m_SlotNumber;
    Int  m_PortIndex;
    //Int  m_HeaderVersion;
    Int  m_FwVersion;
    Int  m_HwRevision;
    Int  m_NwUuid;

    Int  m_MaxBufferSize;
    Bool  m_Support8021P_Priority;
    Bool  m_Supports8021Q_Vlan;
    Bool  m_DeviceOpened;

    // Frame Check Sequence
    Bool  m_FCSEnabled;

    // MAC-addresses
    Bool  m_MacAddressOverride;
    UInt8  m_MacAddressCur[6];          // Current
    UInt8  m_MacAddressPerm[6];         // Permanent
    
    UInt  m_LinkStatus;                 // Link Status: UP/DOWN
    UInt  m_PhySpeedSet;                // Speed/Duplex setting
    Bool  m_PromiscuousModeSet;         // Promiscuous mode setting
    DtWorkItem  m_LinkStatusWorkItem;

    //UInt  m_PacketAlignment;            // Number of alignment bytes
                                        // boundaries
    UInt  m_MaxNumMulticastItems;
    
    // Shared buffer RX
    UInt  m_RxUuid;
    UInt8*  m_pRxBuffer;
    UInt  m_RxBufSize;
    UInt32  m_RxReadOffset;
    UInt32  m_RxWriteOffsetCache;
    DtEvent  m_RxDataAvailableEvent;
    DtSpinLock  m_RxLock;

    // Shared buffer TX
    UInt  m_TxUuid;

    UInt8*  m_pTxBuffer;
    UInt  m_TxBufSize;
    UInt  m_TxMaxLoad;
    UInt32  m_TxWriteOffset;
    UInt32  m_TxReadOffsetCache;

    // TX Timestamping
    UInt8  m_Fingerprint;
    UInt8  m_NumTxTimestampRequests;
    void*  m_pFingerprintPackets[MAX_FINGERPRINT+1];
    UInt64  m_FingerPrintTimestamp[MAX_FINGERPRINT+1];
    DtSpinLock  m_FingerprintLock;
    
    // Threads
    DtThread  m_RxThread;

    // Task offloading by firmware
    Bool  m_SupportIpRxChecksumIPv4Fw;  // Ip checksum check and not disabled
    Bool  m_SupportIpTxChecksumIPv4Fw;  // Ip checksum generation and not disabled
    Bool  m_SupportUdpRxChecksumIPv4Fw; // Udp checksum check and not disabled
    Bool  m_SupportUdpRxChecksumIPv6Fw; // Udp checksum check and not disabled
    Bool  m_SupportUdpTxChecksumIPv4Fw; // Udp checksum generation and not disabled
    Bool  m_SupportUdpTxChecksumIPv6Fw; // Udp checksum generation and not disabled
    Bool  m_SupportTcpRxChecksumIPv4Fw; // Tcp checksum check and not disabled
    Bool  m_SupportTcpRxChecksumIPv6Fw; // Tcp checksum check and not disabled
    Bool  m_SupportTcpTxChecksumIPv4Fw; // Tcp checksum generation and not disabled
    Bool  m_SupportTcpTxChecksumIPv6Fw; // Tcp checksum generation and not disabled
    Bool  m_GenerateChecksumFw;         // Generate checksums by firmware
    Bool  m_CheckIpChecksumIPv4Fw;      // Indicate Ip Rx checksum status
    Bool  m_CheckUdpChecksumIPv4Fw;     // Indicate Udp Rx checksum status
    Bool  m_CheckUdpChecksumIPv6Fw;     // Indicate Udp Rx checksum status
    Bool  m_CheckTcpChecksumIPv4Fw;     // Indicate Tcp Rx checksum status
    Bool  m_CheckTcpChecksumIPv6Fw;     // Indicate Tcp Rx checksum status

    // Internal counters
    UInt  m_NwRcvNoBuffer;
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNw public interface -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

DtStatus  DtPcieNwDriverInit(DtEvtLog* pEvtObject);
DtStatus  DtPcieNwDeviceOpen(DtPcieNwDeviceData* pDvcData);
DtStatus  DtPcieNwDeviceDataInit(DtPcieNwDeviceData* pDvcData);
DtStatus  DtPcieNwDeviceInit(DtPcieNwDeviceData* pDvcData);
void  DtPcieNwDriverExit(void);
void  DtPcieNwDeviceClose(DtPcieNwDeviceData* pDvcData);
void  DtPcieNwDeviceExit(DtPcieNwDeviceData* pDvcData);
DtStatus  DtPcieNwSetMacAddress(DtPcieNwDeviceData* pDvcData, UInt8* pMacAddress);
DtStatus  DtPcieNwGetMacAddressPerm(DtPcieNwDeviceData* pDvcData, UInt8* pMacAddress);
DtStatus  DtPcieNwGetMaxMulticastList(DtPcieNwDeviceData* pDvcData, UInt* pMaxMulticastList);
DtStatus  DtPcieNwGetSpeed(DtPcieNwDeviceData* pDvcData, UInt64* pSpeed);
DtStatus  DtPcieNwGetStatisticCounter(DtPcieNwDeviceData* pDvcData, UInt StatisticId, 
                                                                          UInt64* pValue);
DtStatus  DtPcieNwGetPacketFilter(DtPcieNwDeviceData* pDvcData, UInt* pPacketFilter);
DtStatus  DtPcieNwSetPacketFilter(DtPcieNwDeviceData* pDvcData, UInt PacketFilter);
DtStatus  DtPcieNwSetPhySpeed(DtPcieNwDeviceData* pDvcData, UInt PhySpeed);
DtStatus  DtPcieNwGetPhySpeed(DtPcieNwDeviceData* pDvcData, UInt* pPhySpeed);
DtStatus  DtPcieNwGetLinkStatus(DtPcieNwDeviceData* pDvcData, UInt* pPhyLink);
DtStatus  DtPcieNwTransmitPacket(DtPcieNwDeviceData* pDvcData, UInt PacketSize, 
                                  UInt TotalWriteOffset, UInt TxFlags, UInt8  Fingerprint,
                                  UInt8  PortOffset, UInt8  IpAddrOffset);
DtStatus  DtPcieNwTxGetPointerNewPacket(DtPcieNwDeviceData* pDvcData, UInt PacketSize, 
                                                   UInt8** ppPacket1, UInt* pPacketSize1,
                                                   UInt8** ppPacket2, UInt* pPacketSize2);
DtStatus  DtPcieNwSetMulticastList(DtPcieNwDeviceData* pDvcData, UInt NumItems,
                                                                    void* pMulticastList);
DtStatus  DtPcieNwGetFingerprint(DtPcieNwDeviceData* pDvcData, void* pPacket, UInt8* pFingerprint);
DtStatus  DtPcieNwRemoveFingerprint(DtPcieNwDeviceData* pDvcData, UInt8 Fingerprint);
void BuildPtpTxTimeTlv(uint64_t Seconds, uint64_t Nanoseconds, DektecTlvTime* pTlv);
void UpdatePacketHeaders(UInt8* pPacket, UInt PacketSize, OETlvHeader* pTlvHeader);
#endif // __DtPcieNw_H
