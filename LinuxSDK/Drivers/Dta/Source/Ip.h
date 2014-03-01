//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Ip.h *#*#*#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
//
// Dta driver - IP functionality - Declaration of IP port functionality
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

#ifndef _IP_H
#define _IP_H

#define  DTA_IP_MAX_DMA_CHANNELS        4

#define  DTA_IPRX_MAX_SLICES            25

// IP Port type
#define  DTA_IPPORT_TYPE1               1
#define  DTA_IPPORT_TYPE2               2

#define DTA_MAX_ADDR_MATCHER_ENTRIES    4096

// Forward declarations 
typedef struct _UserIpRxChannel  UserIpRxChannel;
typedef struct _UserIpTxChannel  UserIpTxChannel;
typedef struct _AddressMatcherLookupEntry  AddressMatcherLookupEntry;
typedef struct _AddressMatcherLookupEntryPart2  AddressMatcherLookupEntryPart2;

// Globals
extern Int  g_NwDrvVersionMajor;              // Version number of network driver
extern Int  g_NwDrvVersionMinor;              // Version number of network driver
extern Int  g_NwDrvVersionMicro;              // Version number of network driver
extern Int  g_NwDrvVersionBuild;              // Version number of network driver

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- AddressMatcherLookupEntry -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
struct _AddressMatcherLookupEntry{
    union {
        struct {
            UInt32  m_Data0;
            UInt32  m_Data1;
            UInt32  m_Data2;
            UInt32  m_Data3;
            UInt32  m_Data4;
            UInt32  m_Data5;
        } m_Data;
        struct {
            UInt64  m_Reserved:3;
            UInt64  m_NRT:1;
            UInt64  m_AddressIDTag:13;
            UInt64  m_BaseUdpPort:16;
            UInt64  m_Data1_Part:31;
            UInt32  m_Data2;
            UInt32  m_Data3;
            UInt32  m_Data4;
            UInt32  m_PartData5:1;
            UInt32  m_SSM:1;
            UInt32  m_Version:1;
            UInt32  m_Port:1;
            UInt32  m_Reserved3:28;
        } m_Gen;
        struct {
            UInt64  m_Reserved:3;
            UInt64  m_NRT:1;
            UInt64  m_AddressIDTag:13;
            UInt64  m_BaseUdpPort:16;
            UInt64  m_DestIpAddressL:31;
            //---------------------------
            UInt64  m_DestIpAddressH:1;
            UInt64  m_SrcIpAddress:32;
            UInt64  m_Reserved1:31;
            //---------------------------
            UInt64  m_Reserved2:33;
            UInt64  m_SSM:1;
            UInt64  m_Version:1;
            UInt64  m_Port:1;
            UInt64  m_Reserved3:28;
        } m_IpV4;
        struct {
            UInt64  m_Reserved:3;
            UInt64  m_NRT:1;
            UInt64  m_AddressIDTag:13;
            UInt64  m_BaseUdpPort:16;
            UInt64  m_DestIpAddressL:31;
            //---------------------------
            UInt64  m_DestIpAddressH1:64;
            //---------------------------
            UInt64  m_DestIpAddressH2:33;
            UInt64  m_SSM:1;
            UInt64  m_Version:1;
            UInt64  m_Port:1;
            UInt64  m_Reserved3:28;
        } m_IpV6;
        struct {
            union{
                struct {
                    UInt64  m_Reserved1:3;
                    UInt64  m_NRT:1;
                    UInt64  m_EntryPart2:12;
                    UInt64  m_Reserved2:1;
                    UInt64  m_BaseUdpPort:16;
                    UInt64  m_DestIpAddressL:31;
                    //---------------------------
                    UInt64  m_DestIpAddressH1:64;
                    //---------------------------
                    UInt64  m_DestIpAddressH2:33;
                    UInt64  m_SSM:1;
                    UInt64  m_Version:1;
                    UInt64  m_Port:1;
                    UInt64  m_Reserved3:28;
                } m_IpV6Part1;
            };
        } m_IpV6SSM;
    };
    AddressMatcherLookupEntry*  m_pNext;  // Next with different content (Other ID Tag)
    AddressMatcherLookupEntry*  m_pPrev;  // Prev. with different content (Other ID Tag)
    AddressMatcherLookupEntryPart2*  m_pPart2; // With IPV6 SSM link
    UInt8  m_StreamType;                  // Type of stream (Main, Fec Row/Column)
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- AddressMatcherLookupEntryPart2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
struct _AddressMatcherLookupEntryPart2 {
    union {
        struct {
            UInt32  m_Data0;
            UInt32  m_Data1;
            UInt32  m_Data2;
            UInt32  m_Data3;
            UInt32  m_Data4;
            UInt32  m_Data5;
        } m_Data;
        struct {
            union{
                struct {
                    UInt64  m_Reserved1:4;
                    UInt64  m_AddressIDTag:13;
                    UInt64  m_NbEntries:12;
                    UInt64  m_Reserved2:4;
                    UInt64  m_SrcIpAddressL:31;
                    //---------------------------
                    UInt64  m_SrcIpAddressH1:64;
                    //---------------------------
                    UInt64  m_SrcIpAddressH2:33;
                    UInt64  m_Reserved3:31;
                } m_IpV6Part2_First;    // Only for first entry pointed by m_pPart2
                struct {
                    UInt64  m_Reserved1:4;
                    UInt64  m_AddressIDTag:13;
                    UInt64  m_NbEntriesClear:12;    // Reserved
                    UInt64  m_Reserved2:4;
                    UInt64  m_SrcIpAddressL:31;
                    //---------------------------
                    UInt64  m_SrcIpAddressH1:64;
                    //---------------------------
                    UInt64  m_SrcIpAddressH2:33;
                    UInt64  m_Reserved3:31;
                } m_IpV6Part2;
            };
        } m_IpV6SSM;
    };
    AddressMatcherLookupEntryPart2*  m_pNext; // Next with different content(Other ID Tag)
    AddressMatcherLookupEntryPart2*  m_pPrev; // Prev with different content(Other ID Tag)
    AddressMatcherLookupEntry*  m_pHead;      // Points to the head if this element is the
                                              // first in the list
    UInt8  m_StreamType;                      // Type of stream (Main, Fec Row/Column)
};


// Ip HW channel
typedef struct _IpHwChannel
{
    UInt16  m_RegsOffset;
    UInt16  m_FifoOffset;
    volatile UInt8*  m_pRegs;
    DmaChannel  m_DmaChannel;
} IpHwChannel;

// Ip user channel
typedef struct _DtaIpUserChannels
{
    DtaDeviceData*  m_pDvcData;             // Reference to device data
    DtFastMutex  m_IpTxChannelMutex;        // access protection for Tx channel list
    DtSpinLock  m_IpTxChannelSpinLock;      // Protect access to the channels by DPC's
    DtSpinLock  m_IpTxFlagsSpinLock;        // Protect flags
    UserIpTxChannel*  m_pIpTxChannel;       // Double linked list of User IP Tx channels

    DtFastMutex  m_IpRxChAccesMutex;        // access protection for Rx channel list
    DtFastMutex  m_IpRxChThreadMutex;       // access protection for Rx channel list
    DtSpinLock  m_IpRxBrmSpinLock;          // Protect access during bitrate measurement DPC
    Bool  m_IpRxBrmSkipUpdate;              // When set updates in the DPC must be skipped.
    UserIpRxChannel*  m_pIpRxChannel;       // Double linked list of User IP Rx channels
    Bool  m_IpRxRtpPacketAvailable;         // RTP packet is found during packet parsing
    DtEvent  m_IpRxRtpPacketAvailEvent;     // Event to signal reconstruct thread that
                                            // workerthread has a packet.
} DtaIpUserChannels;

// NRT shared buffer
typedef struct _DtaIpNrtSharedBuf
{
    DtaIpNwSharedBufInfo*  m_pSharedInfo;
    UInt8*  m_pBuffer;
    UInt  m_BufSize;
} DtaIpNrtSharedBuf;

// NRT channels
typedef struct _DtaIpNrtChannels
{
    DtaIpNrtSharedBuf  m_RxSharedBuf;
    DtaIpNrtSharedBuf  m_TxSharedBuf;
} DtaIpNrtChannels;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type1 (DTA 160/2160) -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Rx, Tx  and Dma channel definitions
// Rt = Realtime (DVB), Nrt = Non Realtime

// Definition of type1 port
typedef struct _DtaIpPortType1
{   
    // Rt and Nrt receive (shared channel)
    IpHwChannel  m_Rx;
    DtThread  m_RxThread;
    PPBuffer  m_RxPPBuffer;

    // Nrt transmit
    IpHwChannel  m_TxNrt;
    DtThread  m_TxNrtThread;

    // Rt transmit
    IpHwChannel  m_TxRt;
    PPBuffer  m_TxRtPPBuffer;
} DtaIpPortType1;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type2 (DTA 2162) -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Rx, Tx  and Dma channel definitions
// Rt = Realtime (DVB), Nrt = Non Realtime

// Definition of one port for DTA-2162
typedef struct _DtaIpPortType2
{   
    DtMutex  m_AddrMatcherMutex;                // Only for odd IpPort index,
                                                // used for lookup table update
    UInt  m_AddrMatcherRegsOffset;
    UInt  m_SorterRegsOffset;                   // Only for odd IpPort index,
    UInt  m_AddrMatcherLookupRegsOffset;        // Shared between even and odd IpPort 
                                                // index
    UInt  m_MemoryControllerOffset;
    DtMutex  m_MacAddrFiltMutex;                // Only for odd IpPort index,
                                                // used for lookup table update
    volatile UInt8*  m_pAddrMatcherRegs;
    volatile UInt8*  m_pSorterRegs;             // Only for odd IpPort index
    volatile UInt8*  m_pAddrMatcherLookupRegs;  // Shared between even and odd IpPort 
                                                // index
    volatile UInt8*  m_pMemoryControllerRegs;

    Bool  m_InterruptsEnabled;                  // Interrupts enabled state

    // Nrt receive
    IpHwChannel  m_RxNrt;
    DtThread  m_RxNrtThread;
    PPBuffer  m_RxNrtPPBuffer;

    // Nrt transmit
    IpHwChannel  m_TxNrt;
    DtThread  m_TxNrtThread;

    // Rt receive
    IpHwChannel  m_RxRt;                        // DMA only for odd IpPort index
    DtThread  m_RxRtThread;                     // Only for odd IpPort index
    PPBuffer  m_RxRtPPBuffer;                   // Only for odd IpPort index, even is 
                                                // using the same buffer
    UInt  m_RxRtSlicePtr[DTA_IPRX_MAX_SLICES];
    UInt  m_RxRtSliceSize[DTA_IPRX_MAX_SLICES];
    volatile UInt  m_RxRtSliceReadIndex;
    volatile UInt  m_RxRtSliceWriteIndex;
    DtEvent  m_RxRtSliceEvent;
    DtDpc  m_RxRtSliceEventSetDpc;
    AddressMatcherLookupEntry  m_AddrMatchHead;
                                                // Only for odd IpPort index
    
    // Rt transmit
    IpHwChannel  m_TxRt;                        // DMA channel only for odd channel index
    DtDpc  m_IpRtTxDpc;                         // DPC handling all IpTxRt channels and 
                                                // ports. Only for odd channel index
    DtDpc*  m_pIpRtTxDpc;                       // Pointing to odd channel DPC
    PPBuffer  m_TxRtPPBuffer;
} DtaIpPortType2;

// Ip port containing all members for one physical port
struct _DtaIpPort
{
    Int  m_PortIndex;                           // Physical port index
    Int  m_IpPortIndex;
    UInt  m_PortType;                           // Type of port DTA_IPPORT_TYPEx
    DtaDeviceData*  m_pDvcData;
    DtaChildDeviceData*  m_pChildDvcData;

    // Capabilities
    Bool  m_CapIp;

    // General network register
    UInt  m_GenNwRegsOffset;
    volatile UInt8*  m_pGenNwRegs;

    // Phy/Mac
    PhyMac  m_PhyMac;

    Int  m_NumDmaChannelsInit;
    Int  m_NumDmaChannelsIsr;
    DmaChannel*  m_pDmaChannelsInit[DTA_IP_MAX_DMA_CHANNELS];
    DmaChannel*  m_pDmaChannelsIsr[DTA_IP_MAX_DMA_CHANNELS];

    DtaIpNrtChannels  m_NrtChannels;

    UInt64  m_NumRxFifoOverflow;
    UInt64  m_NumIncorrectDmaRxHeaders;
    UserIpRxChannel**  m_pIpRxListeners;        // Array of channels listening on the
                                                // current stream received.
    Int  m_MaxNumListeners;                     // Max. array size of m_pIpRxListeners
    DtMutex  m_IpRxListenersMutex;              // Mutex to protect m_pIpRxListeners
    DtFastMutex  m_IpRxListenersFastMutex;      // Mutex to protect re-entrance

    union
    {
        DtaIpPortType1  m_IpPortType1;
        DtaIpPortType2  m_IpPortType2;
    };
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Containing all data for one IP hardware device.
//
typedef struct _DtaIpDevice
{
    UInt  m_PortType;                       // Type of port DTA_IPPORT_TYPEx
    Bool  m_IgnoreAdminStatus;              // Ignore status if requested (registry)
    DtaIpPort*  m_pIpPorts;
    DtaIpUserChannels  m_IpUserChannels;
    DtDpc  m_IpRtTxDpc;                    // DPC handling all IpTxRt channels and ports,
                                           // Type1 only
    DtDpc  m_IpRtRxBrmDpc;                 // DPC handling bitrate measurement updates
    DtThread  m_RxReconstructThread;       // Thread handling all user channels RTP 
                                           // reconstruction
    UserIpRxChannel*  m_pAddrMatchLUTable[DTA_MAX_ADDR_MATCHER_ENTRIES];
    UInt8  m_AddrMatchLUTableType[DTA_MAX_ADDR_MATCHER_ENTRIES];
} DtaIpDevice;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtaIpCleanupPre(DtaDeviceData* pDvcData, Int PortIndex, Int IpPortIndex, 
                                                                  DtaIpDevice* pIpDevice);
void  DtaIpCleanup(DtaDeviceData* pDvcData,Int PortIndex, Int IpPortIndex, 
                                                                 DtaIpDevice*  pIpDevice);
DtStatus  DtaIpInit(DtaDeviceData* pDvcData, Int PortIndex, Int IpPortIndex,
                                                                  DtaIpDevice* pIpDevice);
DtStatus  DtaIpInitChannels(DtaDeviceData* pDvcData, DtaIpDevice* pIpDevice);
DtStatus  DtaIpPowerup(DtaIpPort* pIpPort);
void  DtaIpPowerdown(DtaIpDevice* pIpDevice, DtaIpPort* pIpPort);
DtStatus  DtaIpPowerdownPre(DtaIpPort* pIpPort);
DtStatus  DtaIpIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                                                   DtIoctlObject* pIoctl);
DtStatus  DtaIpNwIoctl(DtaDeviceData* pDvcData, DtIoctlObject* pIoctl);
DtStatus  DtaIpIoConfigGet(DtaIpPort* pIpPort, Int Code, DtaIoConfigValue* pCfgValue);
DtStatus  DtaIpIoConfigSet(DtaIpPort* pIpPort, Int Code, DtaIoConfigValue CfgValue);
DtStatus  DtaIpReleaseResourceFromFileObject(DtaDeviceData* pDvcData,
                                                                     DtFileObject* pFile);
UInt64  DtaIpGetNumRxFifoOverflow(DtaIpPort* pIpPort);
DtaShBuffer*  DtaIpGetSharedBuffer(DtaIpDevice* pIpDevice, Int ChannelIndex, 
                                                                         Int ChannelType);
DtStatus  DtaIpSharedBufferReady(DtaIpDevice* pIpDevice, Int ChannelIndex, 
                                                                         Int ChannelType);
void  DtaIpHandleNewPacketFilter(PhyMac* pPhyMac);
DtStatus  DtaIpCheckReleaseAdminStatus(DtaDeviceData* pDvcData);
DtStatus  DtaIpCreateDmaPPBuffer(DmaChannel* pDmaChannel, PPBuffer* pPPBuffer, 
            DtaIpPort* pIpPort, UInt PPBufSize, DtaPPBufferGetLocAddrFunc pGetLocAddrFunc,
            UInt Direction, Bool AutoTransferAfterComplete);
DtStatus  DtaIpCleanupDmaPPBuffer(DmaChannel* pDmaChannel, PPBuffer* pPPBuffer);
DtStatus  DtaIpUpdateMacAddressFilter(DtaIpPort* pIpPort);
void  DtaIpAddrMatcherUpdateTable(DtaIpPort* pIpPort, AddressMatcherLookupEntry* pHead);
void  DtaIpAddrMatcherDeleteEntry(DtaIpPort* pIpPort, AddressMatcherLookupEntry* pEntry,
                                            AddressMatcherLookupEntryPart2*  pEntryPart2);
void  DtaIpAddrMatcherAddEntry(DtaIpPort* pIpPort, AddressMatcherLookupEntry* pHead, 
                                         AddressMatcherLookupEntry* pNewEntry,
                                         AddressMatcherLookupEntryPart2*  pNewEntryPart2);
void  DtaIpClearIpAddress(Bool IpV6, UInt8* pIpAddr);
void  DtaIpDisableInterrupts(DtaIpPort* pIpPort);
void  DtaIpEnableInterrupts(DtaIpPort* pIpPort);
Bool  DtaIpIsIpAddressEmpty(Bool IpV6, UInt8* pIpAddr);
Bool  DtaIpIsMulticastAddress(Bool IpV6, UInt8* pIpAddr);
Bool  DtaIpIsSSMulticastAddress(Bool IpV6, UInt8* pIpAddr);
Bool  DtaIpNeedDriverSSMCheckType2(Bool IpV6, UInt8* pIpSrc, UInt8* pIpDst);
DtStatus  DtaIpReAllocIpRxNumListeners(DtaIpPort* pIpPort, Int NumListeners);

#endif // _IP_H
