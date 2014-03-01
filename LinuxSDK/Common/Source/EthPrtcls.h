//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* EthPrtcls.h *#*#*#*#*#*#*#*#* (C) 2005-2012 DekTec
//  
//  Note: Only defines that are likely to be used in our implementation are defined.
//        Refer to the standardized protocol specifications for more details.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2005-2012 DekTec Digital Video B.V.
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


#ifndef __ETHPRTCLS_H__
#define __ETHPRTCLS_H__

//     THE PROTOCOLS IMPLEMENTED:
//     =========================
//
//     ETHERNET II ENCAPSULATION 
//
//      0                   1                   2                   3   
//      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     |                  Destination MAC Address                      |
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     |                               |    Source MAC Address         |
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     |                                                               |
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     |        Protocol               | 
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

//      IP ENCAPSULATION - RFC1024
//      
//      0                   1                   2                   3   
//      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     |Version|  IHL  |Type of Service|          Total Length         |
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     |         Identification        |Flags|      Fragment Offset    |
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     |  Time to Live |    Protocol   |         Header Checksum       |
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     |                       Source Address                          |
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     |                    Destination Address                        |
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     |                    Options                    |    Padding    |
//     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
//                  Example Internet Datagram Header

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Conversion macro's -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define NWCW(a)     (UInt16)((((a) >> 8) & 0x00FF) | (((a) << 8)& 0xFF00))
#define NWCDW(a)    (UInt32)(((a) >> 24) | (((a) >> 8) & 0x0000FF00 ) | \
                            (((a) << 8) & 0x00FF0000) | ((a) << 24))

// ETH / 802.2/802.3 Defines
#define ETH_TYPE_IPV4           0x0800      // Ethernet Packet Type IPv4
#define ETH_TYPE_IPV4_BE        0x0008      // Ethernet Packet Type IPv4 Big Endian
#define ETH_TYPE_IPV6           0x86DD      // Ethernet Packet Type IPv6
#define ETH_TYPE_IPV6_BE        0xDD86      // Ethernet Packet Type IPv6 Big Endian

#define ETH_TYPE_VLAN           0x8100      // Ethernet Packet Vlan tagged
#define ETH_TYPE_VLAN_BE        0x0081      // Ethernet Packet Vlan tagged

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- RTP Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define RTP_PAYLOAD_MPEGII      0x21        // Payload Type MPEG-II
#define RTP_PAYLOAD_FEC         0x60        // Payload Type Forward Error Control (FEC) 
                                            // packets
#define RTP_HDR_VERSION2        0x02        // RTP Version 2

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DVB Related Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define SYNC_WITH_DUMMY         0x4700      // Sync Byte used with 188/204 bytes + dummies
#define SYNC_WITH_DATA          0xB800      // Sync Byte used with 188/204 bytes with data

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IP  Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Header lenght
#define IPV4_HDR_IPV4_HL        0x5         // IPV4 header length (in DWORDS) 
                                            // without options + padding
// IP Version defines
#define IPV4_HDR_VER_IPV4       0x4         // Internet protocol (IPV4)
#define IPV4_HDR_VER_IPV6       0x6         // IPV6 and SIP (simple internet)

// Flags
#define IPV4_HDR_FLAGS_NOFRAG   0x02        // IPv4 : Don't fragment (bit 1)
#define IPV4_HDR_FLAGS_MOREFRAG 0x01        // IPv4 : More fragments follow (bit 0)

// Protocol
#define IPV4_HDR_PROT_UDP       0x11        // IPv4 We use UDP ...
#define IPV6_HDR_HOPBYHOP       0x00
#define IPV6_HDR_ROUTING        0x2b
#define IPV6_HDR_FRAGMENT       0x2c
#define IPV6_HDR_ENCAP_SEC      0x32
#define IPV6_HDR_AUTH           0x33
#define IPV6_HDR_NO_NXT_HDR     0x3b
#define IPV6_HDR_DST_OPTIONS    0x3c

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FEC defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Type
#define FEC_HDR_TYPE_XOR        0x00

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Length definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define ETHII_HDR_LENGTH        14          // Ethernet II header length
#define IPV4_HDR_LENGTH         20          // IPv4 header length (bytes): without 
                                            // options + padding!!!!
#define IPV6_HDR_LENGTH         40          // IPv6 header length (bytes)
#define VLAN_HEADER_LENGTH      4           // VLAN tagging length (bytes)
#define UDP_HDR_LENGTH          8           // UDP header length (bytes)
#define RTP_HDR_LENGTH          12          // Fixed RTP header length (bytes)
#define RTP_HDR_EXT_LENGTH      4           // Fixed RTP header extension length (bytes) 
                                            // (exclusive DTA-160 timestamp!!)
#define FEC_HDR_LENGTH          16          // FEC header length (bytes)

#pragma pack (push, 1)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EthernetIIHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _EthernetIIHeader 
{
    UInt8  m_DstMac[6];
    UInt8  m_SrcMac[6];
    UInt16  m_Type;
} EthernetIIHeader;

typedef struct _VlanTag
{
    UInt16  m_TPId;                       // Tag Protocol Identifier (0x8100)
    union {
        struct {
        UInt16  m_VId             :12;    // Vlan Identifier: LE
        UInt16  m_Cfi             :1;     // Always 0, no E-RIF present: LE
        UInt16  m_PCP             :3;     // Priority Code Point: LE
        };
        UInt16  m_TCI;            // Big Endian
    };
} VlanTag;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EthernetIIHeaderVlan -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _EthernetIIHeaderVlan 
{
    UInt8  m_DstMac[6];
    UInt8  m_SrcMac[6];
    UInt16  m_TPId;                       // Tag Protocol Identifier (0x8100)

    union {
        struct {
        UInt16  m_VId             :12;    // Vlan Identifier: LE
        UInt16  m_Cfi             :1;     // Always 0, no E-RIF present: LE
        UInt16  m_PCP             :3;     // Priority Code Point: LE
        };
        UInt16  m_TCI;            // Big Endian
    };
    UInt16  m_Type;
} EthernetIIHeaderVlan;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ethernet802_3Header -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _Ethernet802_3Header 
{
    UInt8  m_DstMac[6];
    UInt8  m_SrcMac[6];
    UInt16  m_Length;
    UInt8  m_DSAP;                    //802.2 header: Destination Service Access Point
    UInt8  m_SSAP;                    //802.2 header: Source Service Access Point
    UInt8  m_Ctrl;                    //802.2 header
    UInt8  m_ProtID[3];               //802.2 SNAP header (if DSAP == 170 and SSAP == 170)
    UInt16 m_Type;                    //802.2 SNAP header (if DSAP == 170 and SSAP == 170)
} Ethernet802_3Header;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ethernet802_3HeaderVlan -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _Ethernet802_3HeaderVlan 
{
    UInt8  m_DstMac[6];
    UInt8  m_SrcMac[6];
    UInt16  m_TPId            :16;    // Tag Protocol Identifier (0x8100)
    union {
        struct {
        UInt16  m_VId             :12;    // Vlan Identifier: LE
        UInt16  m_Cfi             :1;     // Always 0, no E-RIF present: LE
        UInt16  m_PCP             :3;     // Priority Code Point: LE
        };
        UInt16  m_TCI;            // Big Endian
    };
    UInt16  m_Length;
    UInt8  m_DSAP;                    //802.2 header: Destination Service Access Point
    UInt8  m_SSAP;                    //802.2 header: Source Service Access Point
    UInt8  m_Ctrl;                    //802.2 header
    UInt8  m_ProtID[3];               //802.2 SNAP header (if DSAP == 170 and SSAP == 170)
    UInt16 m_Type;                    //802.2 SNAP header (if DSAP == 170 and SSAP == 170)
} Ethernet802_3HeaderVlan;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IpHeaderV4 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _IpHeaderV4 
{
    UInt8   m_HeaderLength    :4;
    UInt8   m_Version         :4;
    UInt8   m_TypeOfService;
    UInt16  m_TotalLength;
    UInt16  m_Identification;
    UInt8   m_FragmentOffsetH :5;
    UInt8   m_Flags           :3;
    UInt8   m_FragmentOffsetL;
    UInt8   m_TimeToLive;
    UInt8   m_Protocol;
    UInt16  m_HeaderChecksum;
    UInt8   m_SrcAddress[4];
    UInt8   m_DstAddress[4];
    UInt16  m_OptionsL;    
    UInt16  m_OptionsH;
    UInt8   m_Padding;
} IpHeaderV4;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IpHeaderV6 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _IpHeaderV6 {
    UInt32  m_TrafficClassH   :4;
    UInt32  m_Version         :4;
    UInt32  m_FlowLabelH      :4;
    UInt32  m_TrafficClassL   :4;
    UInt32  m_FlowLabelL      :16;
    UInt16  m_PayloadLength;
    UInt16  m_NextHeader      :8;
    UInt16  m_HopLimit        :8;
    UInt8   m_SrcAddress[16];
    UInt8   m_DstAddress[16];
} IpHeaderV6;

typedef struct _IpV6Options {
    UInt8  m_NextHeader;
    UInt8  m_HeaderExtLength;   // Excluding the first byte
} IpV6Options;


typedef struct _IcmpHeader {
    UInt16  m_Type            :8;
    UInt16  m_Code            :8;
    UInt16  m_Checksum        :16;
} IcmpHeader;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TcpHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _TcpHeader {
    UInt16  m_SourcePort;
    UInt16  m_DestinationPort;
    UInt32  m_SequenceNumber;
    UInt32  m_AcknowledgeNumber;
    UInt16  m_DataOffset      :4;
    UInt16  m_Reserved        :6;
    UInt16  m_Urg             :1;
    UInt16  m_Ack             :1;
    UInt16  m_Psh             :1;
    UInt16  m_Rst             :1;
    UInt16  m_Syn             :1;
    UInt16  m_Fin             :1;
    UInt16  m_Window          :16;
    UInt16  m_Checksum;
    UInt16  m_UrgentPointer;
    UInt32  m_Options         :24;
    UInt32  m_Padding         :8;
} TcpHeader;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- UdpHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _UdpHeader
{
    UInt16  m_SourcePort;
    UInt16  m_DestinationPort;
    UInt16  m_Length;
    UInt16  m_Checksum;
} UdpHeader;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- RtpHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _RtpHeader 
{
    UInt8   m_CsrcCount     :4;
    UInt8   m_Extension     :1;
    UInt8   m_Padding       :1;
    UInt8   m_Version       :2;
    UInt8   m_PayloadType   :7;
    UInt8   m_Marker        :1;
    UInt16  m_SequenceNumber;
    UInt32  m_TimeStamp;
    UInt32  m_Ssrc;
} RtpHeader;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FecHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _FecHeader 
{
    UInt16  m_SnBase;
    UInt16  m_LengthRecovery;
    UInt8   m_PtRecovery    :7;
    UInt8   m_E             :1;
    UInt8   m_MaskH;
    UInt16  m_MaskL;
    UInt32  m_TsRecovery;
    UInt8   m_Index         :3;
    UInt8   m_Type          :3;
    UInt8   m_D             :1;
    UInt8   m_X             :1;
    UInt8   m_Offset;
    UInt8   m_NA;
    UInt8   m_SnBaseExt;
} FecHeader;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- RtpExtension -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _RtpExtension
{
    UInt16 m_Type;
    UInt16 m_Length;
} RtpExtension;

#pragma pack (pop)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DmaTxHeader definition -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtaDmaTxHeaderGen 
{
   struct 
    {
        UInt32  m_PacketLength      : 11;   // Excluding DMA header
        UInt32  m_Reserved1         : 11;
        UInt32  m_FilteringType0    : 1;    // Filtering type0 (=0: Altera MAC)
        UInt32  m_PaddingOff        : 1;    // Disable Padding (=0: Altera MAC)
        UInt32  m_Reserved2         : 2;
        UInt32  m_CRCGenerateOff    : 1;    // Disable generating CRC by MAC
        UInt32  m_SetupFrame        : 1;    // Setup frame (=0: Altera MAC)
        UInt32  m_FilteringType1    : 1;    // Filtering type1 (=0: Altera MAC)
        UInt32  m_Reserved3         : 2;
        UInt32  m_IRQOnTx           : 1;    // Interrupt on tx completion
    } m_TransmitControl;
    UInt32  m_Reserved[3];
} DtaDmaTxHeaderGen;

typedef struct _DtaDmaTxHeaderV3
{   
    struct 
    {
        UInt32  m_PacketLength      : 11;   // Excluding DMA header
        UInt32  m_Reserved1         : 11;
        UInt32  m_FilteringType0    : 1;    // Filtering type0 (=0: Altera MAC)
        UInt32  m_PaddingOff        : 1;    // Disable Padding (=0: Altera MAC)
        UInt32  m_Reserved2         : 2;
        UInt32  m_CRCGenerateOff    : 1;    // Disable generating CRC by MAC
        UInt32  m_SetupFrame        : 1;    // Setup frame (=0: Altera MAC)
        UInt32  m_FilteringType1    : 1;    // Filtering type1 (=0: Altera MAC)
        UInt32  m_Reserved3         : 2;
        UInt32  m_IRQOnTx           : 1;    // Interrupt on tx completion
    } m_TransmitControl;
    struct {
        UInt32  m_NoCalcIPChkSum    : 1;
        UInt32  m_NoCalcUDPChkSum   : 1;
        UInt32  m_NoCalcTCPChkSum   : 1;
        UInt32  m_Reserved          : 29;
    } m_ChecksumCmd;
    UInt32  m_Reserved[2];
} DtaDmaTxHeaderV3;

typedef struct _DtaDmaTxHeader
{
    UInt32  m_Tag;                      // 445441A0h
    UInt32  m_Length:24;                // Header length
    UInt32  m_Version:8;                // Header version
    UInt32  m_Timestamp;                // 24 bits used by FPGA
    UInt32  m_Reserved;
    union {
        DtaDmaTxHeaderGen  m_TxHeaderGen;
        DtaDmaTxHeaderV3  m_TxHeaderV3;
    };
} DtaDmaTxHeader;


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DmaRxHeader definition -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DtaDmaRxHeaderGen: For all header versions identical
// DtaDmaRxHeaderV2: For DTA_IPPORT_TYPE1 cards
// DtaDmaRxHeaderV3: For DTA_IPPORT_TYPE2 cards

typedef struct _DtaDmaRxHeaderGen
{
    struct 
    {
        UInt32  m_Reserved1         : 16;
        UInt32  m_FrameLength       : 14;    // Packet length(incl. checksum + DMA header)
        UInt32  m_Reserved2         : 2;
    } m_ReceiveStatus;

    struct
    {
        UInt32  m_IPChksumValid    : 1;
        UInt32  m_UDPChksumValid   : 1;
        UInt32  m_TCPChksumValid   : 1;
        UInt32  m_Reserved         : 29;
    } m_ChecksumStatus;

    UInt32  m_Reserved[2];
} DtaDmaRxHeaderGen;

typedef struct _DtaDmaRxHeaderV2
{
    struct 
    {
        UInt32  m_Zero              : 1;
        UInt32  m_CRCError          : 1;
        UInt32  m_Dribbling         : 1;
        UInt32  m_ReportGMIIError   : 1;
        UInt32  m_Reserved1         : 1;
        UInt32  m_FrameType         : 1;
        UInt32  m_CollisionSeen     : 1;
        UInt32  m_FrameTooLong      : 1;
        UInt32  m_LastDescriptor    : 1;
        UInt32  m_FirstDescriptor   : 1;
        UInt32  m_MulticastFrame    : 1;
        UInt32  m_RuntFrame         : 1;
        UInt32  m_Reserved2         : 2;
        UInt32  m_DescriptorError   : 1;
        UInt32  m_ErrorSummary      : 1;
        UInt32  m_FrameLength       : 14;    // Ethernet packet length(including checksum)
        UInt32  m_FilteringFail     : 1;
        UInt32  m_Reserved3         : 1;
    } m_ReceiveStatus;                       //(RDES0) register of the ethernet MAC

    struct
    {
        UInt32  m_IPChksumValid    : 1;
        UInt32  m_UDPChksumValid   : 1;
        UInt32  m_TCPChksumValid   : 1;
        UInt32  m_Reserved         : 29;
    } m_ChecksumStatus;

    UInt32  m_Reserved[2];
} DtaDmaRxHeaderV2;

typedef struct _DtaDmaRxHeaderV3
{
    struct {
        UInt32  m_InvLen            : 1;    // Invalid frame length
        UInt32  m_CRCError          : 1;    // CRC Error
        UInt32  m_Dribbling         : 1;    // Dribbling bit
        UInt32  m_ReportGMIIError   : 1;    // Phy error
        UInt32  m_Reserved1         : 1;
        UInt32  m_FrameType         : 1;
        UInt32  m_CollisionSeen     : 1;
        UInt32  m_FrameTooLong      : 1;
        UInt32  m_LastDescriptor    : 1;
        UInt32  m_FirstDescriptor   : 1;
        UInt32  m_MultiOrBroadcast  : 1;    // Multicast or broadcast frame
        UInt32  m_RuntFrame         : 1;
        UInt32  m_UnicastFrame      : 1;    // Unicast frame
        UInt32  m_MulticastFrame    : 1;    // Multicast frame
        UInt32  m_BroadcastFrame    : 1;    // Broadcast frame
        UInt32  m_ErrorSummary      : 1;
        UInt32  m_FrameLength       : 14;   // Ethernet packet length (including checksum)
        UInt32  m_FilteringFail     : 1;
        UInt32  m_VlanFrame         : 1;
    } m_ReceiveStatus;
    struct{
        UInt32  m_IPChksumValid     : 1;
        UInt32  m_UDPChksumValid    : 1;
        UInt32  m_TCPChksumValid    : 1;
        UInt32  m_IPv4Frame         : 1;
        UInt32  m_IPv6Frame         : 1;
        UInt32  m_UDPFrame          : 1;
        UInt32  m_TCPFrame          : 1;
        UInt32  m_Fragmented        : 1;
        UInt32  m_ESP               : 1;    // Encryption frame -- not used
        UInt32  m_VlanPCP           : 3;    // VLan priority  -- not used
        UInt32  m_Reserved1         : 4;
        UInt32  m_Repaired          : 1;    // FEC: Repaired -- not used
        UInt32  m_NullPck           : 1;    // FEC: Null packet inserted -- not used
        UInt32  m_Reserved2         : 2;
        UInt32  m_VlanId            : 12;   // VLan ID -- not used
    } m_FrameStatus;
    struct{
        UInt32  m_AddrIdTag         : 16;
        UInt32  m_AddrIdTagFound    : 1;
        UInt32  m_RealTime          : 1;    // Frame matched realtime(1) or nrt(0)
        UInt32  m_MatchedUC         : 1;    // Frame matched unicast
        UInt32  m_Reserved          : 1;
        UInt32  m_MatchedMAC        : 1;    // MAC Address matches entry in table
        UInt32  m_PaylOffset        : 11;   // UDP Payload offset
    } m_AddrMatching;
    UInt32  m_Reserved3[1];
} DtaDmaRxHeaderV3;

typedef struct _DtaDmaRxHeader 
{
    UInt32  m_Tag;                  // 445441A0h
    UInt32  m_Length:24;            // Header length
    UInt32  m_Version:8;            // Header version
    UInt64  m_Timestamp:64;         // Timestamp: not all bits are used
    union {
        DtaDmaRxHeaderGen  m_RxHeaderGen;
        DtaDmaRxHeaderV2  m_RxHeaderV2;
        DtaDmaRxHeaderV3  m_RxHeaderV3;
    };
} DtaDmaRxHeader;

// Default defines

#define  DT_IP_MIN_ETH_PAYLOAD  60
#define  DT_IP_MAX_ETH_PAYLOAD  1600
#define  DT_IP_MAX_PACKET_SIZE  (ETHII_HDR_LENGTH + DT_IP_MAX_ETH_PAYLOAD)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTAPI shared IP definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTA_IPTX_MAX_PACKET_LENGTH  (DT_IP_MAX_PACKET_SIZE+sizeof(DtaDmaTxHeader))
                                                        // DMA header + max. packet length

#define  DTA_IPRX_BUFWRAPSIZE       256
#define  MAX_FEC_RECONSTR_ELEMENTS  20      // (L,D <= 20, L*D<=100, L+D max 20) for FEC
#define  MAX_NUM_RTP_DVB_PACKETS    100     // (L*D = 100)
#define  MAX_NUM_FEC_MATRIX         5       // (Last FEC max L*D after last packet -->
                                            // (L*D)+(L*D)+extra store for incoming pckts

#define  DTA_IPRX_MAX_RTP_PACKETS   ((MAX_FEC_RECONSTR_ELEMENTS +                        \
                                            MAX_NUM_RTP_DVB_PACKETS) * MAX_NUM_FEC_MATRIX)
                                           // ((L+D)+(L*D)) * NumMatrix

#define  DTA_IPRX_MAX_PACKET_LENGTH (DT_IP_MAX_PACKET_SIZE+sizeof(DtaDmaRxHeader))
#define  DTA_IPRX_BUFRTPSIZE        (DTA_IPRX_MAX_RTP_PACKETS *                          \
                                                               DTA_IPRX_MAX_PACKET_LENGTH)
                                                               

typedef struct _DtaIpRawHeader 
{
    UInt16  m_Tag;                      // 0x44A0h = 'D'160
    UInt16  m_Length;                   // IP Packet Length;
    UInt32  m_TimeStamp;                // Timestamp
} DtaIpRawHeader;

// Buffer header used by IpTx
typedef struct _IpTxBufferHeader
{
    volatile UInt32  m_ReadOffset;          // Read offset; updated by DPC
    volatile UInt32  m_WriteOffset;         // Write offset; updated by DTAPI
    volatile UInt64  m_StartTimestamp;      // Timestamp after starting Tx channel
    volatile UInt32  m_BufSize;             // Total allocated size of the packets buffer.
                                            // BufSize is an exact multiple of the size of
                                            // of a time-stamped IP packet.
} IpTxBufferHeader;

// Buffer header used by IpRx
typedef struct _IpRxBufferHeader
{
    volatile UInt32  m_FifoReadOffset;      // Read offset; updated by DTAPI
    volatile UInt32  m_FifoWriteOffset;     // Write offset; updated by driver Thread
    volatile UInt32  m_BufSize;             // Allocated size of buffer without header.
} IpRxBufferHeader;

#endif  // __ETHPRTCLS_H__
