//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* EthPrtcls.h *#*#*#*#*#*#*#*#*#*#*# (C) 2005 DekTec
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	2005.07.14	RD	Created	


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

  
// DTA-160 packet defines
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Conversion macro's -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define NWCW(a)	(UInt16)((((a) >> 8) & 0x00FF) | (((a) << 8)& 0xFF00))
#define NWCDW(a) 	(UInt32)(((a) >> 24) | (((a) >> 8) & 0x0000FF00 ) | \
				            (((a) << 8) & 0x00FF0000) | ((a) << 24))

//#define PKT_HEADER_START_OFFSET (32 / 4)

#define PROT_UDP                    0                                     //  
#define PROT_RTP                    1                                     //  
#define PROT_RESVD0                 2
#define PROT_RESVD1                 3

#define DVB188                      0
#define DVB204                      1


//-.-.-.-.-.-.-.- General Protocol Related Header  =DWORD= offset Defines -.-.-.-.-.-.-.-.
#define OFFS_ETH                0           // ETH:Ethernet Packet Type
#define OFFSET_ETHII_DEST       OFFS_ETH+0  // ETHII: Destination MAC Address (0+1)
#define OFFSET_ETHII_SRC        OFFS_ETH+1  // ETHII: Source MAC Address      (1+2)
#define OFFSET_ETHII_TYPE       OFFS_ETH+3  // ETHII: Frame Type
#define OFFSET_ETHII_TYPE_W     OFFS_ETH+6  // ETHII: Frame Type


#define OFFSET_802_DEST         OFFS_ETH+0  // 802.x: Destination IP Address
#define OFFSET_802_SRC          OFFS_ETH+1  // 802.x: Source IP Address
#define OFFSET_802_LEN          OFFS_ETH+3  // 802.x: Length
#define OFFSET_802_DSAP         OFFS_ETH+4  // 802.x: Destination Service Access Point  
#define OFFSET_802_SSAP         OFFS_ETH+4  // 802.x: Source Service Access Point
#define OFFSET_802_CNTL         OFFS_ETH+4  // 802.x: Control Field
#define OFFSET_802_ORGCODE      OFFS_ETH+4  // 802.x: Org Code Field
#define OFFSET_802_TYPE         OFFS_ETH+5  // 802.x: Frame Type

#define OFFS_IP_ETH             3           // Start of IP Frame (Ethernet-II Encapsulation)
#define OFFS_IP_802             5           // Start of IP Frame (IEEE802.2 / IEEE802.3 Encapsulation)

/*#define OFFSET_IPHDR_VERSION    0           // IP:Version
#define OFFSET_IPHDR_HDRLEN     0           // IP:Header Length in dwords
#define OFFSET_IPHDR_TOS        0           // IP:Type of Service
#define OFFSET_IPHDR_TOTLEN     1           // IP:Total Length
#define OFFSET_IPHDR_ID         1           // IP:Identification
#define OFFSET_IPHDR_FLGS       2           // IP:Flags
#define OFFSET_IPHDR_FRAGOFFS   2           // IP:Fragment Offset
#define OFFSET_IPHDR_TTL        2           // IP:Time to live
#define OFFSET_IPHDR_PROT       2           // IP:Protocol
#define OFFSET_IPHDR_CHECKSUM   3           // IP:Checksum
#define OFFSET_IPHDR_SRCIP      3           // IP:Source IP Address      (6+7)
#define OFFSET_IPHDR_DESTIP     4           // IP:Destination IP Address (7+8)

#define OFFS_UDP                5
#define OFFSET_UDPHDR_SRCPRT    5           // UDP:Source Port Number
#define OFFSET_UDPHDR_DESTPRT   6           // UDP:Destination Port Number
#define OFFSET_UDPHDR_UDPLEN    6           // UDP:Data Length 
#define OFFSET_UDPHDR_UDPCS     7           // UDP:Checksum

#define OFFS_RTP                8
#define OFFSET_RTPHDR_VPXCCM    8           // RTP:Flags en CSRC count
#define OFFSET_RTPHDR_PT        8           // RTP:Payload type
#define OFFSET_RTPHDR_SEQNUM    9           // RTP:Sequence Number
#define OFFSET_RTPHDR_TS        9           // RTP:Timestamp  (1+2)
#define OFFSET_RTPHDR_SSRC      10          // RTP:Synchronization Source ID  
#define OFFSET_RTPHDR_CSRC      10          // RTP:Contributing Source ID's
*/


/*#define MASK_ETHHDR_TYPE        0xFFFF      // ETH:Ethernet Packet Type

#define MASK_IPHDR_TOS          0xFF<<8     // IP:Type of Service
#define MASK_IPHDR_HDRLEN       0xF         // IP:Header Length in dwords
#define MASK_IPHDR_VERSION      0xF<<4      // IP:Version

#define MASK_IPHDR_TOTLEN       0xFFFF<<16  // IP:Total Length
#define MASK_IPHDR_ID           0xFFFF      // IP:Identification

#define MASK_IPHDR_FLGS         0x7<<16     // IP:Flags
#define MASK_IPHDR_FRAGOFFS     0x1FFF<<19  // IP:Fragment MASK
#define MASK_IPHDR_TTL          0xFF        // IP:Time to live
#define MASK_IPHDR_PROT         0xFF<<8     // IP:Protocol

#define MASK_IPHDR_CHECKSUM     0xFFFF<<16  // IP:Checksum
#define MASK_IPHDR_SRCIP        0xFFFFFFFF  // IP:Source IP Address
#define MASK_IPHDR_DESTIP       0xFFFFFFFF  // IP:Destination IP Address

#define MASK_UDPHDR_SRCPRT      0xFFFF      // UDP:Source Port Number
#define MASK_UDPHDR_DESTPRT     0xFFFF<<16  // UDP:Destination Port Number
#define MASK_UDPHDR_UDPLEN      0xFFFF      // UDP:Data Length 
#define MASK_UDPHDR_UDPCS       0xFFFF<<16  // UDP:Checksum


#define MASK_RTPHDR_VPXCCM      0x1FF
#define MASK_RTPHDR_VER         0x3<<14
#define MASK_RTPHDR_PADDING     0x1<<13
#define MASK_RTPHDR_EXT         0x1<<12
#define MASK_RTPHDR_CSRCC       0xF<<8
#define MASK_RTPHDR_MRKR        0x1<<7
#define MASK_RTPHDR_PT          0x7F        // RTP:Payload type
#define MASK_RTPHDR_SEQNUM      0xFFFF<<16  // RTP:Sequence Number
#define MASK_RTPHDR_TS          0xFFFFFFFF  // RTP:Timestamp  
#define MASK_RTPHDR_SSRC        0xFFFFFFFF  // RTP:Synchronization Source ID  
#define MASK_RTPHDR_CSRC        0xFFFFFFFF  // RTP:Contributing Source ID's
*/

// ETH / 802.2/802.3 Defines
#define ETH_TYPE_IPV4           0x0800      // Ethernet Packet Type IPv4
#define ETH_TYPE_IPV4_BE        0x0008      // Ethernet Packet Type IPv4 Big Endian
#define ETH_TYPE_IPV6           0x86DD      // Ethernet Packet Type IPv6

// UDP Defines
#define UDP_PACKET_TYPE         0x11        // Packet Type UDP

// RTP Defines
#define MAX_DROP_OUT            1024        // Maximum Drop Out number of RTP packet
#define RTP_PAYLOAD_MPEGII      0x21        // Payload Type MPEG-II
#define RTP_PAYLOAD_FEC         0x60        // Payload Type Forward Error Control (FEC) packets

// DVB Related Defines
#define SYNC_WITH_DUMMY         0x4700      // Sync Byte used with 188/204 bytes and dummies
#define SYNC_WITH_DATA          0xB800      // Sync Byte used with 188/204 bytes with data 

// IP  Defines
#define IPV4_HDR_ID             0x4         // IPv4 identification
#define IPV4_HDR_LENGTHDW       0x5         // IPv4 header length (in DWORDS) without options + padding
#define IPV4_HDR_TOS            0x08        // IPv4 Used for bulk transport
//#define IPV4_HDR_TOTLENGTH      0xFF        // IPv4 Total Length "mask"
#define IPV4_HDR_IDENTIFICATION 0x0000
#define IPV4_HDR_FLAGS          0x2         // IPv4 Fragmentation Flags: Don't fragment
#define IPV4_HDR_FRAGOFFS       0x000       // IPv4 Fragmentation Offset - Not Used
#define IPV4_HDR_TTL            0x20        // IPv4 Defaults TTL to 32
#define IPV4_HDR_PROT_UDP       0x11        // IPv4 We use UDP ...
#define IPV4_HDR_FLAGS_MOREFRAG	0x1			// IPv4 More fragments
/*
#define IPV6_HDR_ID             0x6         // IPv6 Identification
#define IPV6_HDR_TRAFFCLASS     0x0         // IPv6 Traffic Class
#define IPV6_HDR_LENGTH         0x14        // IPv6 Header Length
#define IPV6_HDR_FLOWLABEL      0x000       // IPv6 Flow Label
#define IPV6_HDR_PAYLOAD_LENGTH 0x0000      // IPv6 Payload Length
#define IPV6_HDR_NEXT_HDR       0x0         // IPv6 Next Header (see Prot. IPv4)
#define IPV6_HDR_HOPLIMIT       0x00        // IPv6 Hop Limit (TTL)
*/
#define ETHII_HDR_LENGTH        14          // Ethernet II header length
#define IPV4_HDR_LENGTH         20          // IPv4 header length (bytes): without options + padding!!!!
#define UDP_HDR_LENGTH          8           // UDP header length (bytes)
#define RTP_HDR_LENGTH          12          // Fixed RTP header length (bytes)
#define RTP_HDR_EXT_LENGTH      4           // Fixed RTP header extension length (bytes) (exclusive DTA-160 timestsmp!!)
#define FEC_HDR_LENGTH          16          // FEC header length (bytes)

#define FEC_HDR_TYPE_XOR        0x00

 
#define RTP_HDR_VERSION         0x02        // RTP Version 2
#define RTP_HDR_PADDING         0x00        // Padding Inactive
#define RTP_HDR_EXTENSION       0x00        // Header Extension not present
#define RTP_HDR_CSRC            0x00        // Contributing Synchronisation Source Identiefier Count
#define RTP_HDR_MARKER          0x01        // Marker
/*
#define RTP_HDR_PADDING         0x20        // Padding Active
#define RTP_HDR_EXTENSION       0x10        // Header Extension present
#define RTP_HDR_PAYLOADTYPE     
*/

#pragma pack (push, 1)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EthernetIIHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _EthernetIIHeader {
	UInt8  m_DstMac[6];
	UInt8  m_SrcMac[6];
	UInt16  m_Type;
} EthernetIIHeader;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ethernet802_3Header -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _Ethernet802_3Header {
	UInt8  m_DstMac[6];
	UInt8  m_SrcMac[6];
	UInt16  m_Length;
	UInt8  m_DSAP;		//802.2 header: Destination Service Access Point
	UInt8  m_SSAP;		//802.2 header: Source Service Access Point
	UInt8  m_Ctrl;		//802.2 header
	UInt8  m_ProtID[3];	//802.2 SNAP header (if DSAP == 170 and SSAP == 170)
	UInt16 m_Type;		//802.2 SNAP header (if DSAP == 170 and SSAP == 170)
} Ethernet802_3Header;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IpHeaderV4 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _IpHeaderV4 {
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- UdpHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _UdpHeader
{
	UInt16  m_SourcePort;			//Start UDP Header
	UInt16  m_DestinationPort;
	UInt16  m_Length;
	UInt16  m_Checksum;
} UdpHeader;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- RtpHeader -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _RtpHeader {
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
typedef struct _FecHeader {
	UInt16  m_SnBase;
	UInt16  m_LengthRecovery;
	UInt8   m_PtRecovery    :7;
	UInt8   m_E             :1;
	UInt8   m_MaskH;					// Mask High/low maybe swapped!!
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

#endif  // __ETHPRTCLS_H__
