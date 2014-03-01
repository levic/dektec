//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* IpTx.h *#*#*#*#*#*#*#*#*#* (C) 2010-2012 DekTec
//
// Dta driver - IP TX functionality - Declaration of TX specific functionality for
//                                    IP ports.
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

#ifndef _IP_TX_H
#define _IP_TX_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- UserIpTxChannel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This structure holds the state data for a single User-IP Transmit channel.
//
struct _UserIpTxChannel 
{
    Int  m_IpPortIndex;                 // IP-port index
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

    UInt32  m_LastTimestampOffs;        // Offset of last timestamp. Used for offset wrapping
    UInt64  m_RefTimestamp;             // Initial timestamp after starting Tx channel
                                        // Incremented after timestamp offset wrapping
    
    struct _UserIpTxChannel* m_pNext;   // Pointer to next UserIpTxChannel element
    struct _UserIpTxChannel* m_pPrev;   // Pointer to previous UserIpTxChannel element
};


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
UserIpTxChannel*  DtaIpTxUserChGet(DtaIpUserChannels* pIpUserChannels, Int ChannelIndex);
DtStatus  DtaIpTxIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile, 
                                                                   DtIoctlObject* pIoctl);
DtStatus  DtaIpTxRtCreateDmaBuffer(DmaChannel* pDmaChannel, PPBuffer* pPPBuffer, 
                                                                      DtaIpPort* pIpPort);
DtStatus  DtaIpTxRtCleanupDmaBuffer(DmaChannel* pDmaChannel, PPBuffer* pPPBuffer);
void  DtaIpTxUserChDestroy(DtaIpUserChannels* pIpDevice, UserIpTxChannel* pIpTxChannel);
void  DtaIpTxRtProcessPacketsDpc(DtDpcArgs* pArgs);
DtStatus  DtaIpTxNrtCreateBuffer(DmaChannel* pDmaChannel, DtaIpNrtChannels* pNrtChannels);
DtStatus  DtaIpTxNrtCleanupBuffer(DmaChannel* pDmaChannel, 
                                                          DtaIpNrtChannels* pNrtChannels);
DtStatus  DtaIpTxDeviceInit(DtaIpDevice* pIpDevice);
DtStatus  DtaIpTxInit(DtaIpPort* pIpPort);
void  DtaIpTxDevicePowerDown(DtaIpDevice* pIpDevice);
DtStatus  DtaIpTxPowerup(DtaIpPort* pIpPort);
DtStatus  DtaIpTxPowerdownPre(DtaIpPort* pIpPort);

DtStatus  DtaIpTxInitType1(DtaIpPort* pIpPort);
void  DtaIpTxCleanupType1(DtaIpPort* pIpPort);




#endif // _IP_TX_H
