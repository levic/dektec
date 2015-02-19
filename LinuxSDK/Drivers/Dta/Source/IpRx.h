//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* IpRx.h *#*#*#*#*#*#*#*#*#* (C) 2011-2015 DekTec
//
// Dta driver - IP RX functionality - Declaration of RX specific functionality for
//                                    IP ports.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2015 DekTec Digital Video B.V.
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

#ifndef _IP_RX_H
#define _IP_RX_H

#define FEC_INC_COLUMN_PORT     2   // Increment port number for FEC column
#define FEC_INC_ROW_PORT        4   // Increment port number for FEC row

// Max. number of channels listen on same port
#define MAX_NUM_IPRX_LISTENERS_INIT 15
#define MAX_NUM_IPRX_LISTENERS      2048        // This constant is only to protect the
                                                // system resources. Can be increased if
                                                // needed

// IpRx bit-rate measurement: maximum #samples stored
#define NUM_IPRX_MAX_SAMPLES    50

// IpRx Inter Packet Arrival Time: number of seconds to store
#define IPRX_IPAT_NUM_SECONDS   60

// IpRx Packet time difference between port 1 and 2: number of seconds to store
#define IPRX_SKEW_NUM_SECONDS   60

// Max. RTP seq num store for statistics
#define IPRX_MAX_RTP_SEQ_NUM_STAT   20

// Address matcher lookup index IpRxChannel
#define ADDRM_TYPE_MAIN        0    // Index 0
#define ADDRM_TYPE_FECROW      1    // Index 1
#define ADDRM_TYPE_FECCOLUMN   2    // Index 2



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- RtpListEntry -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Describes an entry for the storage of RTP packets 
//
typedef struct _RtpListEntry 
{
    DtListEntry  m_ListEntry;
    UInt  m_BufIndex;         // Index of buffer.
    UInt16  m_RtpSequenceNumber; // RTP sequence number
    UInt16  m_PayloadOffset;  // Offset of the actual payload data (DVB/FEC) (bytes)
    UInt16  m_RtpOffset:15;   // Offset of the RTP header packet
    UInt16  m_InUse:1;        // 1: InUse by RTP reconstructor
    UInt8  m_FrameCount;      // SDI Frame count this packet belongs to
    UInt8  m_Spare;           // Not used at this moment
    UInt32  m_Spare2;         // Not used at this moment
    UInt32  m_RtpTimestamp;   // Timestamp of RTP packet. From 90khz or 27Mhz ref. clock

    // For FEC packets only
    UInt16  m_FecSNBase;      // Base Sequence number DVB packet for this Fec packet
    UInt16  m_FecOffset;       // L parameter for FEC column, D parameter for FEC row
    UInt16  m_FecNA;           // Number of media packets beloging to this FEC.
    UInt16  m_Spare3;          // Not used at this moment
} RtpListEntry;



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IpRxIpat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// IpRx Ipat: Inter Packet Arrival Times including delay factor + BER
//
typedef struct _IpRxIpat
{
    UInt64  m_StartTime;        // Time when first packet was stored
    UInt64  m_LastTime;         // Time when last packet was stored
    UInt64  m_FirstSampleTime;  // Time of first packet
    UInt64  m_LastSampleTime;   // Time of last packet was stored

    // Ipat
    Int  m_MinIpat;             // In 54Mhz Clocks
    Int  m_MaxIpat;             // In 54Mhz Clocks

    // Delay factor
    Int  m_MinDelayF;           // In 54Mhz Clocks
    Int  m_MaxDelayF;           // In 54Mhz Clocks
    Bool  m_MinMaxDelayValid;   // TRUE if m_MinDelayF/m_MaxDelayF are set
    UInt32  m_FirstRtpTime;     // RTP Timestamp of packet stored
    UInt32  m_LastRtpTime;      // RTP Timestamp of packet stored

    // BER
    UInt  m_BerNumPacketsMainStart; 
                                // Final reconstructed stream
    UInt  m_BerNumPacketLostMainStart;
                                // Final reconstructed stream
    UInt  m_BerNumPacketsMainCur;
                                // Final reconstructed stream
    UInt  m_BerNumPacketLostMainCur; 
                                // Final reconstructed stream
    
    UInt  m_BerNumPacketsReceived;
                                // Number of packets stream port x
    UInt  m_BerNumPacketsLost;  // Number of packets lost port x
} IpRxIpat;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IpRxSkew -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// IpRx Skew: Time difference packet Port1/Port2
//
typedef struct _IpRxSkew 
{
     UInt64  m_StartTime;       // Time when first packet was stored
    UInt64  m_SampleTime[2];    // Per port
    Int  m_SeqNum[2];           // Per port
    UInt32  m_RtpTime[2];       // RTP Timestamp of packet stored
    Int64  m_MinSkew;           // In 54Mhz Clocks
    Int64  m_MaxSkew;           // In 54Mhz Clocks
    Bool  m_MinMaxValid;       // TRUE is m_MinSkew/m_MaxSkew are set
} IpRxSkew;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- UserIpRxChannel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This structure holds the state data for a single User-IP Receive channel.
//
struct _UserIpRxChannel 
{
    Int  m_IpPortIndex;                 // IP-port index (not used in failsafe mode)
    Int  m_PortIndex;                   // Port index (seen by application).
    Int  m_ChannelIndex;                // Channel index within set of IpRx channels
    DtFileObject  m_FileObject;         // File object of process associated with this
                                        // channel. The purpose, amongst others, is to
                                        // purging left-over channel objects in Close

    DtaShBuffer  m_SharedBuffer;        // Shared User-buffer descriptor
    //UInt  m_SharedBufferSize;           // Buffer size

    // Receive FIFO buffer and buffer state
    // Buffer consists of "fifo" buffer + 256 auxialiary bytes for easy buffer-wrapping
    // + RTP packets (FEC row/column + RTP DVB)
    Int  m_BufSize;                     // Buffer size; excludes auxiliary space
    
    IpRxBufferHeader*  m_pBufferHeader; // Header containing read/write indices
    UInt8*  m_pFifo;                    // Start of the fifo (the actual data for DTAPI)
    Int  m_FifoSize;                    // Size of the fifo
    UInt8*  m_pRtpBuffer;               // Start of RTP buffer
    UInt8*  m_pJumboPktBuffer;          // Start of Jumbo pakket buffer
    Int  m_MaxJumboPktSize;             // Size of Jumbo pakket buffer
    UInt32* volatile  m_pRtpAvailLookup1; // Lookup table to register RTP SN received 
                                        // (timestamp is stored)
    UInt32* volatile  m_pRtpAvailLookup2; // Lookup table to register RTP SN received 
                                        // (timestamp is stored)
    UInt  m_MaxPacketOutOfSync;         // Max. time to wait for packet to come in
    UInt  m_MinPacketDelay;             // Min. delay to transfer packet to user buffer
    // Status flags
    // DTAPI_RX_FIFO_OVF, DTAPI_RX_SYNC_ERR
    Int  m_Flags;                       // DTAPI_RX_FIFO_OVF, DTAPI_RX_SYNC_ERR
    Int  m_LatchedFlags;                // Latched value of m_Flags
    DtSpinLock  m_FlagsSpinLock;        // Access protection
    Int  m_PckSizeSrc;                  // (Latest) Packet size of TPs in IP packets
                                        // 0=UNKNOWN, -1=INVALID, 188, 204
    // Control of reception process
    Int  m_RxControl;                   // DTA_RXCTRL_IDLE/DTA_RXCTRL_RCV
    Int  m_RxMode;                      // DTA_RXMODE_ST188/...
    UInt16  m_TrpModePckSeqCount;       // Packet sequenc number of transparent-mode-packets

    // Handshaking
    Int  m_RefCount;                    // Number of thread using this struct
    volatile Bool  m_RxIncomingPackets; // True if new RTP packets are available

    // Fragmented packets
    UInt16  m_FragmentId;
    UInt16  m_FragmentOffset;
    UInt16  m_CurIpHeaderOffset;

    // IP Parameters
    UInt16  m_SrcPort;
    UInt16  m_DstPort;
    UInt8  m_SrcIPAddress[16];
    UInt8  m_DstIPAddress[16];          // Used for multicast receive
    Int  m_VlanId;
    UInt16  m_SrcPort2;
    UInt16  m_DstPort2;
    UInt8  m_SrcIPAddress2[16];
    UInt8  m_DstIPAddress2[16];         // Used for multicast receive
    Int  m_VlanId2;
    
    Int  m_FecMode;
    Bool  m_IpParsValid;                // True if IpPars is set
    Int  m_IpParsMode;                  // IpPars mode
    Int  m_IpParsFlags;                 // IpPars flags
    Bool  m_DoSSMCheckSw;               // TRUE: If SSM check must be done in SW 1st port
    Bool  m_DoSSMCheckSw2;              // TRUE: If SSM check must be done in SW 2nd port

    // Profile
    Int  m_VidStd;                      // Video standard to receive. -1= TS, 0= SDI auto.
    Int  m_MaxBitrate;                  // Maximal expected bitrate
    Int  m_MaxSkew;                     // Max. skew in SMPTE_2022-7

    // Detected IP parameters
    Int  m_DetFecNumRows;               // Status to user appli
    Int  m_DetFecNumColumns;            // Status to user appli
    Int  m_DetNumTpPerIp;               // Status to user appli
    Int  m_DetProtocol;                 // Status to user appli
    Int  m_DetVidStd;                   // Status to user appli
    Int  m_RstCntFecRow;                // Used for clearing m_DetFecNumRows
    Int  m_RstCntFecColumn;             // Used for clearing m_DetFecNumColumn

    // Bit-rate measurement
    UInt32  m_BrmNumPckBytesRcvd;       // Total #packet bytes received
    Int  m_BrmNumSamples;               // #valid samples in m_BrmPckBytesDiff
    Int  m_BrmIdxLastSample;            // Index in ValidCnt of last sample received
    Int  m_BrmEstimate;                 // Latest bit rate estimate
    UInt64  m_BrmLastRxTimeStamp;       // Last timestamp received

    // Statistic: Ipat / DelayFactor / BER
    IpRxIpat  m_Ipat[2][IPRX_IPAT_NUM_SECONDS];
    Int  m_CurIpatEl[2];
    Int  m_NumIpatEl[2];
    Int  m_MinIpatSecond[2];           // Min. inter packet arrival time in 54Mhz Clocks
    Int  m_MaxIpatSecond[2];           // Max. inter packet arrival time in 54Mhz Clocks
    Int  m_MinDelayFSecond[2];         // Min. delay factor in 54Mhz Clocks
    Int  m_MaxDelayFSecond[2];         // Max. delay factor in 54Mhz Clocks
    Int  m_MinIpatMinute[2];           // Min. inter packet arrival time in 54Mhz Clocks
    Int  m_MaxIpatMinute[2];           // Max. inter packet arrival time in 54Mhz Clocks
    Int  m_MinDelayFMinute[2];         // Min. delay factor in 54Mhz Clocks
    Int  m_MaxDelayFMinute[2];         // Max. delay factor in 54Mhz Clocks

    // Path 1 and Path 2 ber measurement values
    UInt16  m_SeqNumStat[2][IPRX_MAX_RTP_SEQ_NUM_STAT];
    UInt  m_NumSeqNumStat[2];
    Int  m_LastSeqNumStat[2];
    UInt  m_BerNumIpPacketsSecond[2];
    UInt  m_BerNumIpPacketsLostSecond[2];
    UInt  m_BerNumIpPacketsMinute[2];
    UInt  m_BerNumIpPacketsLostMinute[2];
    
    // Main stream ber measurement values.. must be somewhat identical values for path 1/2
    UInt  m_BerNumIpPacketsMainSecond[2];
    UInt  m_BerNumIpPacketsLostMainSecond[2];
    UInt  m_BerNumIpPacketsMainMinute[2];
    UInt  m_BerNumIpPacketsLostMainMinute[2];

    // Statistic: Skew
    IpRxSkew  m_Skew[IPRX_SKEW_NUM_SECONDS];
    Int  m_CurSkewEl;
    Int  m_NumSkewEl;
    Int64  m_MinSkewSecond;             // In 54Mhz ticks
    Int64  m_MaxSkewSecond;             // In 54Mhz ticks
    Int64  m_MinSkewMinute;             // In 54Mhz ticks
    Int64  m_MaxSkewMinute;             // In 54Mhz ticks

    // Measurement samples: #packet bytes and time stamp (in #54MHz cycles)
    UInt32  m_BrmSampleNumPckBytes[NUM_IPRX_MAX_SAMPLES];
    UInt64  m_BrmSampleTimeStamp[NUM_IPRX_MAX_SAMPLES];

     // RTP Packets List and admin
    DtListEntry  m_RtpEmptyList;
    DtListEntry  m_RtpFecRowList;
    DtListEntry  m_RtpFecColumnList;
    DtListEntry  m_RtpDvbList;
    UInt8*  m_pRtpListEntries;          // Base pointer of array of Rtp list entries
                                        // These entries are stored in the shared buffer
    DtSpinLock  m_RtpListSpinLock;      // Spinlock for the Rtp list pointers
    UInt16  m_RtpLastSeqNum;            // Last used sequence number
    UInt32  m_RtpLastTimestamp;         // Last used timestamp
    Bool  m_RtpFirstPacket;             // TRUE, if no DVB packets are received yet
    DtSpinLock  m_StatisticsSpinLock;   // Access protection
    UInt  m_NumPacketsReconstructed;    // Main stream
    UInt  m_NumPacketsNotReconstructed; // Main stream
    UInt  m_TotNumPackets;              // Main stream
    UInt  m_NumIpPacketsReceived[2];    // SMPTE-7 mode
    UInt  m_NumIpPacketsLost[2];        // SMPTE-7 mode

    // SDI
    UInt8*  m_pSdiInternalWritePointer; // Update main pointer only at frame base
    UInt8  m_SdiCurFrameNumber;         // Current SDI frame number
    Int  m_SdiNumBytesCopied;           // Number of bytes copied for current frame
    Int  m_SdiFrameSize;                // Total number of bytes 1 frame
    UInt8  m_SdiLeftOverBytes[5];       // The bytes left over for 10-bit conversion
    Int  m_SdiNumLeftOverBytes;         // Number of bytes left over for 10-bit conversion
    Bool  m_SdiSkipFrame;               // True if frame must be skipped because of error
    
    UserIpRxChannel*  m_pNext;          // Pointer to next UserIpRxChannel element
    UserIpRxChannel*  m_pPrev;          // Pointer to previous UserIpRxChannel element
    
    // Extra type2 fields for Address Matcher entries
    // [x][y]: x: StreamType(main, FecR,FecC), y=First+Second ethernet port
    AddressMatcherLookupEntry*  m_pNextAddrMatcherEntry[3][2];
                                        // Pointer to next UserIpRxChannel using the same 
                                        // address matcher entry
    AddressMatcherLookupEntry*  m_pPrevAddrMatcherEntry[3][2];
                                        // Pointer to prev. UserIpRxChannel using the same
                                        // address matcher entry
    
    AddressMatcherLookupEntryPart2*  m_pNextAddrMatcherEntryPart2[3][2];
                                        // Pointer to next UserIpRxChannel using the same 
                                        // address matcher entry part2
    AddressMatcherLookupEntryPart2*  m_pPrevAddrMatcherEntryPart2[3][2];
                                        // Pointer to prev. UserIpRxChannel using the same
                                        // address matcher entry part2
    AddressMatcherLookupEntry  m_AddrMatcherEntry[3][2];
                                        // Address Matcher entry
    AddressMatcherLookupEntryPart2  m_AddrMatcherEntrySSMPart2[3][2];
                                        // Address Matcher SSM entry
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxNrtCreateBuffer(DtaIpNrtChannels* pNrtChannels);
DtStatus  DtaIpRxNrtCleanupBuffer(DtaIpNrtChannels* pNrtChannels);
DtStatus  DtaIpRxDeviceInit(DtaIpDevice* pIpDevice);
DtStatus  DtaIpRxInitType1(DtaIpPort* pIpPort);
DtStatus  DtaIpRxInitType2(DtaIpPort* pIpPort);
DtStatus  DtaIpRxIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile, 
                                                                   DtIoctlObject* pIoctl);
void  DtaIpRxCleanupType1(DtaIpPort* pIpPort);
void  DtaIpRxCleanupType2(DtaIpPort* pIpPort);
DtStatus  DtaIpRxPowerup(DtaIpPort* pIpPort);
DtStatus  DtaIpRxDevicePowerup(DtaIpDevice* pIpDevice);
void  DtaIpRxDevicePowerdown(DtaIpDevice* pIpDevice);
DtStatus  DtaIpRxPowerdownPre(DtaIpPort* pIpPort);
DtStatus  DtaIpRxDevicePowerdownPre(DtaIpDevice* pIpDevice);

UserIpRxChannel*  DtaIpRxUserChGet(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex);
void  DtaIpRxUserChDestroy(DtaIpUserChannels* pIpUserChannels, 
                                                           UserIpRxChannel* pIpRxChannel);
void  DtaIpRxRtUpdateSlicePointer(DtaIpPort* pIpPort, Bool SliceOverflow);
UInt8*  DtaIpRxRtpListsInit(UserIpRxChannel* pIpRxChannel, UInt RtpBufSize);

#endif
