//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* IpTx.h *#*#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
//
// Dta driver - IP TX functionality - Declaration of TX specific functionality for
//                                    IP ports.
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

#ifndef _IP_TX_H
#define _IP_TX_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- UserIpTxChannel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This structure holds the state data for a single User-IP Transmit channel.
//
struct _UserIpTxChannel 
{
    Int  m_IpPortIndex;                 // IP-port index (not used in failsafe mode)
    Int  m_PortIndex;                   // Port index (seen by application).
    Int  m_ChannelIndex;                // Channel index within set of IpRx channels
    DtFileObject  m_FileObject;         // File object of process associated with this
                                        // channel. The purpose, amongst others, is to
                                        // purge left-over channel objects in Close

    // Transmit-FIFO Packet Buffer, shared with DTAPI (user mode)
    // The buffer contains time-stamped IP packet payloads
    // If the transmission parameters are changed, the buffer is reallocated
    DtaShBuffer  m_SharedBuffer;        // Shared User-buffer    
    IpTxBufferHeader*  m_pTxBufferHead; // Header containing read/write pointers shared 
                                        // with DTAPI
    UInt8*  m_pBuffer;                  // The actual channel buffer
    
    // Status flags
    Bool  m_ReportOverFlow;             // If set an overflow is reported
    Int  m_Flags;                       // 
    Int  m_LatchedFlags;                // Latched value of m_Flags
    DtSpinLock  m_FlagsSpinLock;        // Access protection

    // Control of transmission process
    Int  m_TxControl;                   // DTA_TXCTRL_IDLE/HOLD/SEND    

    UInt32  m_LastTimestampOffs;        // Offset of last timestamp. Used for offset 
                                        // wrapping
    UInt64  m_RefTimestamp;             // Initial timestamp after starting Tx channel
                                        // Incremented after timestamp offset wrapping
    // IP pars
    Int  m_FecMode;
    Bool  m_IpParsValid;                // True if IpPars is set
    Int  m_IpParsMode;                  // IpPars mode
    Int  m_IpParsFlags;                 // IpPars flags
    // IP pars 2nd port (only used for even port-indexes: 0, 2, etc.)
    UInt16  m_SrcPort;                  // Source port (BIG-Endian)
    UInt16  m_DstPort;                  // Destination port
    UInt16  m_DstPortMain;              // Destination port of main channel (BIG-Endian)
    UInt16  m_DstPortMainRow;           // Destination port of FEC ROW (BIG-Endian)
    UInt16  m_DstPortMainColumn;        // Destination port of FEC COLUMN (BIG-Endian)
    UInt32  m_LastReadOffset;           // Last offset to be read for second port
    UInt32  m_LastTimestampOffs2;       // Offset of last timestamp. Used for offset 
                                        // wrapping
    UInt64  m_RefTimestamp2;            // Initial timestamp after starting Tx channel
                                        // Incremented after timestamp offset wrapping
    
    // IP headers to be copied to the second packet buffer
    union {
        EthernetIIHeader  m_Default;
        EthernetIIHeaderVlan  m_Vlan;
    } m_EthIIHeader;
    union {
        UInt8  m_IpAddressesV4[8];
        UInt8  m_IpAddressesV6[32];
    } m_IpAddrHeader;
    UInt8  m_IpAddrHeaderOffset;        // Offset of IP-address from start of packet
    UInt8  m_IpAddrHeaderSize;
    UInt8  m_EthHeaderSize;

    struct _UserIpTxChannel*  m_pNext;  // Pointer to next UserIpTxChannel element
    struct _UserIpTxChannel*  m_pPrev;  // Pointer to previous UserIpTxChannel element
};


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
UserIpTxChannel*  DtaIpTxUserChGet(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex);
DtStatus  DtaIpTxIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile, 
                                                                   DtIoctlObject* pIoctl);
void  DtaIpTxUserChDestroy(DtaIpUserChannels* pIpDevice, UserIpTxChannel* pIpTxChannel);
void  DtaIpTxUserChDestroyUnsafe(DtaIpUserChannels* pIpUserChannels,
                                                           UserIpTxChannel* pIpTxChannel);
void  DtaIpTxRtProcessPacketsType1Dpc(DtDpcArgs* pArgs);
void  DtaIpTxRtProcessPacketsType2Dpc(DtDpcArgs* pArgs);
DtStatus  DtaIpTxNrtCreateBuffer(DmaChannel* pDmaChannel, DtaIpNrtChannels* pNrtChannels);
DtStatus  DtaIpTxNrtCleanupBuffer(DmaChannel* pDmaChannel, 
                                                          DtaIpNrtChannels* pNrtChannels);
DtStatus  DtaIpTxDeviceInit(DtaIpDevice* pIpDevice);
void  DtaIpTxDevicePowerdown(DtaIpDevice* pIpDevice);
DtStatus  DtaIpTxPowerup(DtaIpPort* pIpPort);
DtStatus  DtaIpTxPowerdownPre(DtaIpPort* pIpPort);
void  DtaIpTxPowerDown(DtaIpPort* pIpPort);

DtStatus  DtaIpTxInitType1(DtaIpPort* pIpPort);
DtStatus  DtaIpTxInitType2(DtaIpPort* pIpPort);
void  DtaIpTxCleanupType1(DtaIpPort* pIpPort);
void  DtaIpTxCleanupType2(DtaIpPort* pIpPort);




#endif // _IP_TX_H
