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

#define  MAX_IP_PORTS               2
#define  DTA_IP_MAX_DMA_CHANNELS    4

// IP Port type
#define  DTA_IPPORT_TYPE1           1
#define  DTA_IPPORT_TYPE2           2

// Forward declarations 
typedef struct _UserIpRxChannel  UserIpRxChannel;
typedef struct _UserIpTxChannel  UserIpTxChannel;

// Globals
extern Int  g_NwDrvVersionMajor;              // Version number of network driver
extern Int  g_NwDrvVersionMinor;              // Version number of network driver
extern Int  g_NwDrvVersionMicro;              // Version number of network driver
extern Int  g_NwDrvVersionBuild;              // Version number of network driver

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
    Bool  m_IpRxPacketAvailable;            // RTP packet is found during packet parsing
    DtEvent  m_IpRxPacketAvailEvent;        // Event to signal reconstruct thread that
                                            // workerthread has a packet.
    Bool  m_IpRxPacketAvailableTrigger;     // Trigger to notify reconstruct thread
                                            // to wake up (condition needed for Linux)

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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type1 (DTA 160/2160) -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Rx, Tx  and Dma channel definitions
// Rt = Realtime (DVB), Nrt = Non Realtime

// Definition of type1 port
typedef struct _DtaIpPortType1
{   
    // Rt and Nrt receive (shared channel)
    IpHwChannel  m_Rx;

    // Rt transmit
    IpHwChannel  m_TxRt;
    
    // Nrt transmit
    IpHwChannel  m_TxNrt;
} DtaIpPortType1;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type2 (DTA 2162) -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Rx, Tx  and Dma channel definitions
// Rt = Realtime (DVB), Nrt = Non Realtime

// Definition of one port for DTA-2162
typedef struct _DtaIpPortType2
{   
    // TODO ADDRESS MATCHER
    // TODO SORTER
    // TODO ...

    // Nrt receive
    IpHwChannel  m_RxNrt;
    
    // Nrt transmit
    IpHwChannel  m_TxNrt;
    
    // Rt receive
    IpHwChannel  m_RxRt;      // DMA channel only for odd channel index
    
    // Rt transmit
    IpHwChannel  m_TxRt;      // DMA channel only for odd channel index
} DtaIpPortType2;

// Ip port containing all members for one physical port
struct _DtaIpPort
{
    Int  m_PortIndex;                   // Physical port index
    Int  m_IpPortIndex;
    UInt  m_PortType;                   // Type of port DTA_IPPORT_TYPEx
    DtaDeviceData*  m_pDvcData;
    DtaChildDeviceData*  m_pChildDvcData;
    
    // Capabilities
    Bool  m_CapIp;

    // General network register
    UInt  m_GenNwRegsOffset;
    volatile UInt8*  m_pGenNwRegs;

    // Phy/Mac
    PhyMac  m_PhyMac;

    Int  m_NumDmaChannels;
    DmaChannel*  m_pDmaChannels[DTA_IP_MAX_DMA_CHANNELS];

    PPBuffer  m_RtTxPPBuffer;
    PPBuffer  m_RxPPBuffer;
    DtaIpNrtChannels  m_NrtChannels;
    DtThread  m_RxThread;
    DtThread  m_RxReconstructThread;
    DtThread  m_TxNrtThread;
    UInt64  m_NumRxFifoOverflow;
    
    union
    {
        DtaIpPortType1  m_IpPortType1;
        DtaIpPortType2  m_IpPortType2;
    };
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIpPort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Containing all data for one IP hardware device.
//
typedef struct _DtaIpDevice
{
    UInt  m_PortType;                       // Type of port DTA_IPPORT_TYPEx
    Bool  m_IgnoreAdminStatus;              // Ignore status if requested (registry)
    DtaIpPort  m_IpPorts[MAX_IP_PORTS];
    DtaIpUserChannels  m_IpUserChannels;
    DtDpc  m_IpRtTxDpc;                    // DPC handling all IpTxRt channels and ports
    DtDpc  m_IpRtRxBrmDpc;                 // DPC handling bitrate measurement updates
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
#endif // _IP_H
