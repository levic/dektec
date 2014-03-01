//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* IpRx.h *#*#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// Dta driver - IP RX functionality - Declaration of RX specific functionality for
//                                    IP ports.
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

#ifndef _IP_RX_H
#define _IP_RX_H

#define FEC_INC_COLUMN_PORT		2	// Increment port number for FEC column
#define FEC_INC_ROW_PORT		4	// Increment port number for FEC row

// Max. number of channels listen on same port
#define MAX_NUM_IPRX_LISTEN     15

// IP-Rx bit-rate measurement: maximum #samples stored
#define NUM_IPRX_MAX_SAMPLES        50

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ RtpListEntry +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Describes an entry for the storage of RTP packets 
//
typedef struct _RtpListEntry 
{
    DtListEntry m_ListEntry;
    UInt16  m_RTPSequenceNumber;
    UInt16  m_BufIndex;         // Index of buffer.
    UInt16  m_PayloadOffset;    // Offset of the actual payload data (DVB/FEC) (bytes)
    UInt16  m_RTPOffset:15;     // Offset of the RTP header packet
    UInt16  m_InUse:1;          // 1: InUse by RTP reconstructor
            
    // For FEC packets only
    UInt16  m_FecSNBase;    // Base Sequence number DVB packet for this Fec packet
    UInt8   m_FecOffset;    // L parameter for FEC column, D parameter for FEC row
    UInt8   m_FecNA;        // Number of media packets beloging to this FEC.

} RtpListEntry;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- UserIpRxChannel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This structure holds the state data for a single User-IP Receive channel.
//
struct _UserIpRxChannel 
{
    Int  m_IpPortIndex;                 // IP-port index
    Int  m_ChannelIndex;                // Channel index within set of IpRx channels
    DtFileObject  m_FileObject;         // File object of process associated with this
                                        // channel. The purpose, amongst others, is to
                                        // purging left-over channel objects in Close

    DtaShBuffer  m_SharedBuffer;        // Shared User-buffer descriptor
    UInt  m_SharedBufferSize;           // Buffer size

    // Receive FIFO buffer and buffer state
    // Buffer consists of "real" buffer + 256 auxialiary bytes for easy buffer-wrapping
    // + RTP packets (FEC row/column + RTP DVB)
    IpRxBufferHeader* m_pBufferHeader;  // Header containing read/write indices
    UInt8*  m_pFifo;                    // Start of the fifo (the actual data for DTAPI)
    Int  m_FifoSize;                    // Size of the fifo
    UInt8*  m_pRtpBuffer;               // Start of RTP buffer
    UInt8*  m_pWrapArea;                // Start of Wrap area
    Int  m_BufSize;                     // Buffer size; excludes auxiliary space
    
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
    UInt8  m_SrcIPAddress[4];               
    UInt8  m_DstIPAddress[4];           // Used for multicast receive
    Int  m_FecMode;
    Bool  m_IpParsValid;               // True if IpPars is set

    // Detected IP parameters
    Int  m_DetFecNumRows;             // Used for status to user appli
    Int  m_DetFecNumColumns;          // Used for status to user appli
    Int  m_DetNumTpPerIp;             // Used for status to user appli
    Int  m_DetProtocol;               // Used for status to user appli
    Int  m_RstCntFecRow;              // Used for clearing m_DetFecNumRows
    Int  m_RstCntFecColumn;           // Used for clearing m_DetFecNumColumn

    // Bit-rate measurement
    UInt32  m_BrmNumPckBytesRcvd;       // Total #packet bytes received
    Int  m_BrmNumSamples;               // #valid samples in m_BrmPckBytesDiff
    Int  m_BrmIdxLastSample;            // Index in ValidCnt of last sample received
    Int  m_BrmEstimate;                 // Latest bit rate estimate
    UInt64  m_BrmLastRxTimeStamp;       // Last timestamp received

    // Measurement samples: #packet bytes and time stamp (in #50MHz cycles)
    UInt32  m_BrmSampleNumPckBytes[NUM_IPRX_MAX_SAMPLES];
    UInt64  m_BrmSampleTimeStamp[NUM_IPRX_MAX_SAMPLES];

     // RTP Packets List and admin
    DtListEntry  m_RtpEmptyList;
    DtListEntry   m_RtpFecRowList;
    DtListEntry   m_RtpFecColumnList;
    DtListEntry   m_RtpDvbList;
    UInt8*  m_pRtpListEntries;          // Base pointer of array of Rtp list entries
    DtSpinLock  m_RtpListSpinLock;      // Spinlock for the Rtp list pointers
    UInt16  m_RtpLastSeqNum;            // Last used sequence number
    Bool  m_RtpFirstPacket;          // TRUE, if no DVB packets are received yet
    DtSpinLock  m_StatisticsSpinLock;   // Access protection
    UInt  m_NumPacketsReconstructed;
    UInt  m_NumPacketsNotReconstructed;
    UInt  m_TotNumPackets;

    struct _UserIpRxChannel*  m_pNext;  // Pointer to next UserIpRxChannel element
    struct _UserIpRxChannel*  m_pPrev;  // Pointer to previous UserIpRxChannel element
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIpRxNrtCreateBuffer(DtaIpNrtChannels* pNrtChannels);
DtStatus  DtaIpRxNrtCleanupBuffer(DtaIpNrtChannels* pNrtChannels);
DtStatus  DtaIpRxInit(DtaIpPort* pIpPort);
DtStatus  DtaIpRxDeviceInit(DtaIpDevice* pIpDevice);
DtStatus  DtaIpRxInitType1(DtaIpPort* pIpPort);
DtStatus  DtaIpRxIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile, 
                                                                   DtIoctlObject* pIoctl);
void  DtaIpRxCleanupType1(DtaIpPort* pIpPort);
DtStatus  DtaIpRxPowerup(DtaIpPort* pIpPort);
void  DtaIpRxDevicePowerDown(DtaIpDevice* pIpDevice);
DtStatus  DtaIpRxPowerdownPre(DtaIpPort* pIpPort);

UserIpRxChannel*  DtaIpRxUserChGet(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex);
void  DtaIpRxUserChDestroy(DtaIpUserChannels* pIpUserChannels, 
                                                           UserIpRxChannel* pIpRxChannel);

#endif