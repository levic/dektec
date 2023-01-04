// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* PtpPkt.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2021 DekTec
//
#pragma once
#ifdef NW_DRIVER
#ifdef WINBUILD
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef unsigned short uint16_t;
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef unsigned long uint32_t;
typedef short int16_t;
typedef long int32_t;
#endif
#else
#include <cstdint>
using namespace std;
#endif

#pragma pack (push, 1)

#define PTP_VERSION 2

#define TLV_ORGANIZATION_EXTENSION  0x0003 
#define TLV_SUBTYPE_RXTIME          1
#define TLV_SUBTYPE_TXTIME          2
#define TLV_SUBTYPE_SOURCE_IP       3


#define UDP_PORT_PTP_EVENT   319
#define UDP_PORT_PTP_COMMON  320

typedef int64_t TimeInterval; // nanoseconds << 16

// On the wire time stamp format.
typedef struct _Timestamp 
{
    uint16_t   SecondsMsb; // 16 bits + 
    uint32_t   SecondsLsb; // 32 bits = 48 bits
    uint32_t   Nanoseconds;
} Timestamp;

// Internal binary time stamp format.
typedef struct _timestamp 
{
    uint64_t   Sec;
    uint32_t   NSec;
} timestamp;

typedef struct _ClockIdentity 
{
    uint8_t  Id[8];
} ClockIdentity;

typedef struct _PortIdentity 
{
    ClockIdentity  ClkIdentity;
    uint16_t       PortNumber;
} PortIdentity;

typedef struct _PortAddress 
{
    uint16_t  NetworkProtocol;
    uint16_t  AddressLength;
    uint8_t   Address[0];
} PortAddress;

typedef struct _PhysicalAddress 
{
    uint16_t  Length;
    uint8_t   Address[0];
} PhysicalAddress;

typedef struct _ClockQuality 
{
    uint8_t     ClockClass;
    uint8_t     ClockAccuracy;
    uint16_t    OffsetScaledLogVariance;
} ClockQuality;


typedef struct _TlvHeader
{
    uint16_t  Type;
    uint16_t  Length; // must be even
    uint8_t   Value[0];
} TlvHeader;

typedef struct _OETlvHeader
{
    uint16_t Type; // ORGANIZATION_EXTENSION
    uint16_t Length; // 6 + 4 (SubType0)
    uint8_t Id[3]; // 0x0014F4
    uint8_t SubType[3];

} OETlvHeader;

typedef struct _DektecTlvTime
{
    OETlvHeader TlvHdr;
    uint64_t TodNanoseconds;
    uint64_t TodSeconds;
} DektecTlvTime;

typedef struct _DektecTlvSourceIp
{
    OETlvHeader TlvHdr;
    uint8_t IpAddress[16];
    uint8_t IpLength;
    uint8_t Padding;
} DektecTlvSourceIp;

typedef struct _Smpte2059Tlv
{
    OETlvHeader TlvHdr;
    uint32_t DefaultSystemFrameRateNom;
    uint32_t DefaultSystemFrameRateDenom;
    uint8_t MasterLockingStatus;
    uint8_t TimeAddressFlags;
    int32_t CurrentLocalOffset;
    int32_t JumpSeconds;
    uint8_t TimeOfNextJump[6];
    uint8_t TimeOfNextJam[6];
    uint8_t TimeOfPreviousJam[6];
    int32_t PreviousJamLocalOffset;
    uint8_t DaylightSaving;
    uint8_t LeapSecondJump;
} Smpte2059Tlv;

typedef struct _DektecTlvs
{
    union 
    {
        OETlvHeader TlvHeader;
        DektecTlvSourceIp SourceIpTlv;
        DektecTlvTime TimeTlv;
    };
    
} DektecTlvs;

typedef struct _PTPText 
{
    uint8_t  Length;
    uint8_t  Text[0];
} PTPText;

// Values for the messageType field
#define PTP_MSGTYPE_SYNC                    0x0
#define PTP_MSGTYPE_DELAY_REQ               0x1
#define PTP_MSGTYPE_PDELAY_REQ              0x2
#define PTP_MSGTYPE_PDELAY_RESP             0x3
#define PTP_MSGTYPE_FOLLOW_UP               0x8
#define PTP_MSGTYPE_DELAY_RESP              0x9
#define PTP_MSGTYPE_PDELAY_RESP_FOLLOW_UP   0xA
#define PTP_MSGTYPE_ANNOUNCE                0xB
#define PTP_MSGTYPE_SIGNALING               0xC
#define PTP_MSGTYPE_MANAGEMENT              0xD

// Bits for flagField[0]
#define PTP_FLAG0_ALT_MASTER                (1<<0)
#define PTP_FLAG0_TWO_STEP                  (1<<1)
#define PTP_FLAG0_UNICAST                   (1<<2)

// Bits for flagField[1]
#define PTP_FLAG1_LEAP_61                   (1<<0)
#define PTP_FLAG1_LEAP_59                   (1<<1)
#define PTP_FLAG1_UTC_OFF_VALID             (1<<2)
#define PTP_FLAG1_PTP_TIMESCALE             (1<<3)
#define PTP_FLAG1_TIME_TRACEABLE            (1<<4)
#define PTP_FLAG1_FREQ_TRACEABLE            (1<<5)
#define PTP_FLAG1_SYNC_UNCERTAIN            (1<<6)

enum ControlField 
{
    CTL_SYNC,
    CTL_DELAY_REQ,
    CTL_FOLLOW_UP,
    CTL_DELAY_RESP,
    CTL_MANAGEMENT,
    CTL_OTHER,
};

typedef struct _PtpHeader 
{
    uint8_t         MessageType:4;
    uint8_t         MajorSdoId : 4;
    uint8_t         VersionPtp:4;
    uint8_t         MinorVersionPtp:4;      // 1588-2019
    uint16_t        MessageLength;
    uint8_t         DomainNumber;
    uint8_t         MinorSdoId;             // 1588-2019
    uint8_t         FlagField[2];
    int64_t         CorrectionField;
    
    uint32_t        MessageTypeSpecific;    // 1588-2019
    PortIdentity    SourcePortIdentity;
    uint16_t        SequenceId;
    uint8_t         ControlField;
    int8_t          LogMessageInterval;
} PtpHeader;

typedef struct _AnnounceMsg 
{
    PtpHeader       Hdr;
    Timestamp       OriginTimestamp;
    int16_t         CurrentUtcOffset;
    uint8_t         Reserved;
    uint8_t         GrandmasterPriority1;
    ClockQuality    GrandmasterClockQuality;
    uint8_t         GrandmasterPriority2;
    ClockIdentity   GrandmasterIdentity;
    uint16_t        StepsRemoved;
    uint8_t         TimeSource;
    //uint8_t         Suffix[0];
} AnnounceMsg;

typedef struct _SyncMsg 
{
    PtpHeader       Hdr;
    Timestamp       OriginTimestamp;
} SyncMsg;

typedef struct _DelayReqMsg 
{
    PtpHeader       Hdr;
    Timestamp       OriginTimestamp;
    //uint8_t         Suffix[0];
} DelayReqMsg;

typedef struct _FollowUpMsg 
{
    PtpHeader       Hdr;
    Timestamp       PreciseOriginTimestamp;
    //uint8_t         Suffix[0];
} FollowUpMsg;

typedef struct _DelayRespMsg 
{
    PtpHeader       Hdr;
    Timestamp       ReceiveTimestamp;
    PortIdentity    RequestingPortIdentity;
    //uint8_t         Suffix[0];
} DelayRespMsg;

typedef struct _PDelayReqMsg 
{
    PtpHeader       Hdr;
    Timestamp       OriginTimestamp;
    PortIdentity    Reserved;
} PDelayReqMsg;

typedef struct _PDelayRespMsg 
{
    PtpHeader       Hdr;
    Timestamp       RequestReceiptTimestamp;
    PortIdentity    RequestingPortIdentity;
} PDelayRespMsg;

typedef struct _PDelayRespFupMsg 
{
    PtpHeader       Hdr;
    Timestamp       ResponseOriginTimestamp;
    PortIdentity    RequestingPortIdentity;
    //uint8_t         Suffix[0];
} PDelayRespFupMsg;

typedef struct _SignalingMsg 
{
    PtpHeader       Hdr;
    PortIdentity    TargetPortIdentity;
    //uint8_t         Suffix[0];
} SignalingMsg;

typedef struct _ManagementMsg 
{
    PtpHeader       Hdr;
    PortIdentity    TargetPortIdentity;
    uint8_t         StartingBoundaryHops;
    uint8_t         BoundaryHops;
    uint8_t         Flags; // reserved | actionField
    uint8_t         Reserved;
    //uint8_t         Suffix[0];
} ManagementMsg;

#pragma pack (pop)
