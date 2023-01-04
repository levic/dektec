// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPcieNwIal.c *#*#*#*#*#*#*#* (C) 2021-2022 DekTec
//
// DtPcieNw driver - Implementation of the Linux Network interface.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtPcieNwIncludes.h>
#include <DtIal.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Compile time defaults
#ifndef DTPCIENW_MAX_DEVICES
#define  DTPCIENW_MAX_DEVICES 10
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Global variables
Int  g_DtPcieNwMaxDevices = DTPCIENW_MAX_DEVICES;
bool  g_DtPcieNwEnableFCS = false;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Global variables -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Load time parameters
module_param(g_DtPcieNwMaxDevices, int, S_IRUGO);  // Do not change int to Int!
module_param(g_DtPcieNwEnableFCS, bool, S_IRUGO);  // Do not change bool to Bool!
MODULE_PARM_DESC(g_DtPcieNwEnableFCS, 
                           "Enable Frame Check Sequence (0=disable [default], 1=enable)");


MODULE_AUTHOR("DekTec Digital Video B.V.");
MODULE_DESCRIPTION("DtPcie Series Network Driver");

// ADDED TO PREVENT TAINTED MESSAGE. THIS MODULE CAN BE USED FREELY (SEE LICENSE
// STATEMENT ABOVE).
MODULE_LICENSE("GPL";)

// Global variables
static DtPcieNwDeviceData**  g_pDtPcieNwDeviceData = NULL;
static Int  g_DtPcieNwDeviceCount = 0;
static DtDriverItf  g_DtDriverItf;



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Forward declarations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Private helper functions
static DtStatus  DtPcieNwIalSetPacketFilter(DtPcieNwDeviceData* pDvcData, Int Flags);
#ifdef HAVE_HW_TIME_STAMP
static Int DtPcieNwIalSetHwTimestampConfig(DtPcieNwDeviceData* pDvcData, 
                                                         struct hwtstamp_config* pConfig);
#endif

// Network interface
Int  DtPcieNwEvtOpen(struct net_device* pDevice);
Int  DtPcieNwEvtClose(struct net_device* pDevice);
Int  DtPcieNwEvtStartTx(struct sk_buff* pSkb, struct net_device* pDevice);
void  DtPcieNwEvtSetMulticastList(struct net_device* pDevice);
static struct net_device_stats*  DtPcieNwEvtGetStats(struct net_device* pDevice);
static Int  DtPcieNwEvtSetMacAddr(struct net_device* pDevice, void* pAddr);
#ifdef PERM_ADDR_SUPPORT
Int  DtPcieNwEvtGetPermAddr(struct net_device* pDevice, struct ethtool_perm_addr* pAddr, 
                                                                            UInt8 *pData);
#endif
static void  DtPcieNwEvtGetDriverInfo(struct net_device* pDevice, 
                                                           struct ethtool_drvinfo* pInfo);
#ifdef GS_SETTINGS_SUPPORT
static Int  DtPcieNwEvtGetSettings(struct net_device* pDevice, struct ethtool_cmd* pCmd);
static Int  DtPcieNwEvtSetSettings(struct net_device* pDevice, struct ethtool_cmd* pCmd);
#else
static Int  DtPcieNwEvtGetLinkKSettings(struct net_device* pDevice, 
                                           struct ethtool_link_ksettings* pLinkKSettings);
static Int  DtPcieNwEvtSetLinkKSettings(struct net_device* pDevice, 
                                     const struct ethtool_link_ksettings* pLinkKSettings);
#endif
UInt32  DtPcieNwEvtGetLink(struct net_device* pDevice);
#ifdef NO_HW_FEATURE_SUPPORT
static UInt32  DtPcieNwEvtGetRxCsum(struct net_device* pDevice);
static Int  DtPcieNwEvtSetRxCsum(struct net_device* pDevice, UInt32 Value);
static UInt32  DtPcieNwEvtGetTxCsum(struct net_device* pDevice);
static Int  DtPcieNwEvtSetTxCsum(struct net_device* pDevice, u32 Value);
#endif
static dt_netdev_features  DtPcieNwEvtFixFeatures(struct net_device* pDevice,
                                                             dt_netdev_features Features);
static int  DtPcieNwEvtSetFeatures(struct net_device* pDevice, 
                                                             dt_netdev_features Features);

#ifdef HAVE_HW_TIME_STAMP
static int DtPcieNwEvtDoIoctl(struct net_device* dev, struct ifreq* ifr, int cmd);
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
static int DtPcieNwEvtGetTsInfo(struct net_device* netdev, 
                                                         struct ethtool_ts_info* ts_info);
#endif

// Kernel module interface
static Int  DtPcieNwModuleInit(void);
static void  DtPcieNwModuleExit(void);


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Interface declarations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Network interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
static const struct ethtool_ops  EthToolOps = 
{
    .get_drvinfo = DtPcieNwEvtGetDriverInfo,
#ifdef GS_SETTINGS_SUPPORT
    .get_settings = DtPcieNwEvtGetSettings,
    .set_settings = DtPcieNwEvtSetSettings,
#else
    .get_link_ksettings = DtPcieNwEvtGetLinkKSettings,
    .set_link_ksettings = DtPcieNwEvtSetLinkKSettings,
#endif
    .get_link = DtPcieNwEvtGetLink,
#ifdef PERM_ADDR_SUPPORT
    .get_perm_addr = DtPcieNwEvtGetPermAddr,
#endif
#ifdef NO_HW_FEATURE_SUPPORT
    .get_rx_csum = DtPcieNwEvtGetRxCsum,
    .set_rx_csum = DtPcieNwEvtSetRxCsum,
    .get_tx_csum = DtPcieNwEvtGetTxCsum,
    .set_tx_csum = DtPcieNwEvtSetTxCsum,
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
    .get_ts_info = DtPcieNwEvtGetTsInfo,
#endif
};

// Starting at kernel 2.6.29 the net_device callback functions should be placed in the 
// net_device_ops structure
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)

static const struct net_device_ops  NetDevOps = 
{
    .ndo_open = DtPcieNwEvtOpen,
    .ndo_start_xmit = DtPcieNwEvtStartTx,
    .ndo_stop = DtPcieNwEvtClose,
    .ndo_get_stats = DtPcieNwEvtGetStats,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
    .ndo_set_rx_mode = DtPcieNwEvtSetMulticastList,
#else
    .ndo_set_multicast_list = DtPcieNwEvtSetMulticastList,
#endif
    .ndo_set_mac_address = DtPcieNwEvtSetMacAddr,
#ifndef NO_HW_FEATURE_SUPPORT
    .ndo_fix_features = DtPcieNwEvtFixFeatures,
    .ndo_set_features = DtPcieNwEvtSetFeatures,
#endif
#ifdef HAVE_HW_TIME_STAMP
    .ndo_do_ioctl = DtPcieNwEvtDoIoctl,
#endif
};
#endif


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtPcieNwEvtOpen(struct net_device* pDevice)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    
    DtDbgOut(MAX, IAL, "Start");

    if (!DT_SUCCESS(DtPcieNwDeviceOpen(pDvcData)))
    {
        DtPcieNwDeviceClose(pDvcData);
        return -EFAULT;
    }
    if (!DT_SUCCESS(DtPcieNwIalSetPacketFilter(pDvcData, pDevice->flags)))
        return -EFAULT;

    netif_start_queue(pDevice);

    DtDbgOut(MAX, IAL, "Exit");
    
    return 0;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtPcieNwEvtClose(struct net_device* pDevice)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    
    DtDbgOut(MAX, IAL, "Start");

    netif_stop_queue(pDevice);
    DtPcieNwDeviceClose(pDvcData);
    DtDbgOut(MAX, IAL, "Exit");
    
    return 0;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwAquireLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtPcieNwAquireLockIal(DtPcieNwDeviceData* pDvcData)
{
    // Nothing to do
    return;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwReleaseLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtPcieNwReleaseLockIal(DtPcieNwDeviceData* pDvcData)
{
    // Nothing to do
    return;
}

#ifdef HAVE_HW_TIME_STAMP
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwDoIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int DtPcieNwEvtDoIoctl(struct net_device* pDevice, struct ifreq* ifr, int cmd)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    Int  err = 0;
    struct hwtstamp_config*  pConfig = &pDvcData->m_IalData.m_HwTsConfig;
    DtDbgOut(MAX, IAL, "Start");

    switch (cmd)
    {
    case SIOCGHWTSTAMP: // Get HW Timestamp config
        return copy_to_user(ifr->ifr_data, pConfig, sizeof(*pConfig)) ? -EFAULT : 0;
    case SIOCSHWTSTAMP: // Set HW Timestamp config
        err = DtPcieNwIalSetHwTimestampConfig(pDvcData, pConfig);
        break;
    default:
        err = -EOPNOTSUPP;
    }
	
    DtDbgOut(MAX, IAL, "Exit");

    return err;
}
#endif

/*
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- MatchPtpClockDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int MatchPtpClockDevice(struct device* pDevice, void* pData)
{
    if (strncmp(dev_name(pDevice), "ptp", 3) == 0)
        return 1;
    return 0;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwFindPtpClockDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtPcieNwFindPtpClockDevice(struct device* pParentDevice)
{
    struct ptp_clock*  pPtpClock;
    Int  Index;
    struct device* pClockDevice;
    
    pClockDevice = device_find_child(pParentDevice, NULL, MatchPtpClockDevice);
    if (pClockDevice == NULL)
        return -1;
    pPtpClock = dev_get_drvdata(pClockDevice);
    Index = ptp_clock_index(pPtpClock);
    put_device(pClockDevice);
    return Index;
}
*/

#ifdef HAVE_HW_TIME_STAMP
// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwIalSetHwTimestampConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int DtPcieNwIalSetHwTimestampConfig(DtPcieNwDeviceData* pDvcData, 
                                                          struct hwtstamp_config* pConfig)
{
    // TX timestamp: 
    // HWTSTAMP_TX_OFF=0, HWTSTAMP_TX_ON=1, 
    // HWTSTAMP_TX_ONESTEP_SYNC=2, HWTSTAMP_TX_ONESTEP_P2P=3
    if (pConfig->tx_type > 3)
        return -ERANGE;

    // RX TIMESTAMP
    switch (pConfig->rx_filter) 
    {
    case HWTSTAMP_FILTER_NONE:
        break;

    default:
        pConfig->rx_filter = HWTSTAMP_FILTER_ALL;
        break;// return -ERANGE;
    }
    DtMemCopy(&pDvcData->m_IalData.m_HwTsConfig, pConfig, 
                                                sizeof(pDvcData->m_IalData.m_HwTsConfig));
    return 0;
}
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwStartTx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// The stack will execute DtPcieNwEvtStartTx not in parallel.
// (NETIF_F_LLTX flag is not set)
// If an error occurs, return 1 and do not free the packet!
//
Int  DtPcieNwEvtStartTx(struct sk_buff* pSkb, struct net_device* pDevice)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    UInt  PacketLength;
    UInt8*  pData;
    UInt8*  pPacket1;
    UInt8*  pPacket2;
    UInt  PacketSize1;
    UInt  PacketSize2;
    DtStatus  Status;
    
    UInt  TxFlags = 0;
    UInt8  Fingerprint = 0;
    UInt  PortOffset = 0;
    UInt  IpAddrOffset = 0;
    // Check if it's our own dummy generated packet
    if (pSkb->len >= (sizeof(EthernetIIHeader)))
    {
        EthernetIIHeader*  pEthernetIIHeader = (EthernetIIHeader*)pSkb->data;
        EthernetIIHeaderVlan*  pEthernetIIHeaderVlan = (EthernetIIHeaderVlan*)pSkb->data;
        IpHeaderV4*  pIpHeaderV4 = NULL;
        IpHeaderV6*  pIpHeaderV6 = NULL;
        UdpHeader*  pUdpHeader = NULL;
        TcpHeader*  pTcpHeader = NULL;
        UInt8*  pData;
        Int  EthType;
        Int  EthHeaderSize;

        if (pEthernetIIHeader->m_Type == ETH_TYPE_VLAN_BE)
        {
            EthType = pEthernetIIHeaderVlan->m_Type;
            EthHeaderSize = sizeof(EthernetIIHeaderVlan);
        } else {
            EthType = pEthernetIIHeader->m_Type;
            EthHeaderSize = sizeof(EthernetIIHeader);
        }

        if (EthType == ETH_TYPE_IPV4_BE)
        {
            pIpHeaderV4 = (IpHeaderV4*)(pSkb->data + EthHeaderSize);
            TxFlags |= DTPCIENW_TXFLAGS_PKTTYPE_IPV4;
            IpAddrOffset = ((UInt8*)&pIpHeaderV4->m_SrcAddress) - 
                                                   ((UInt8*)pSkb->data) + sizeof(DtEthIp);
            IpAddrOffset = IpAddrOffset / 4;
        }
        else if (EthType == ETH_TYPE_IPV6_BE)
        {
            pIpHeaderV6 = (IpHeaderV6*)(pSkb->data + EthHeaderSize);
            TxFlags |= DTPCIENW_TXFLAGS_PKTTYPE_IPV6;
            IpAddrOffset = ((UInt8*)&pIpHeaderV6->m_SrcAddress) -
                ((UInt8*)pSkb->data) + sizeof(DtEthIp);
            IpAddrOffset = IpAddrOffset / 4;
        }

        if (pIpHeaderV4 != NULL)
        {
            if (pIpHeaderV4->m_Protocol == IPV4_HDR_PROT_UDP)
                pUdpHeader = (UdpHeader*)((UInt8*)pIpHeaderV4 + sizeof(IpHeaderV4) - 5);
            else if (pIpHeaderV4->m_Protocol == IPV4_HDR_PROT_TCP)
                pTcpHeader = (TcpHeader*)((UInt8*)pIpHeaderV4 + sizeof(IpHeaderV4) - 5);
        }

        if (pIpHeaderV6 != NULL)
        {
            // Search for UDP header
            IpV6Options*  pOptions;
            UInt  ExtHeader = pIpHeaderV6->m_NextHeader;
            pData = (UInt8*)pIpHeaderV6 + sizeof(IpHeaderV6);
            
            while (pUdpHeader == NULL && pTcpHeader == NULL)
            {
                pOptions = (IpV6Options*)pData;
                if (ExtHeader == IPV4_HDR_PROT_UDP)
                    pUdpHeader = (UdpHeader*)pData;
                else if (ExtHeader == IPV4_HDR_PROT_TCP)
                    pTcpHeader = (TcpHeader*)pData;
                else if (ExtHeader!=IPV6_HDR_HOPBYHOP && ExtHeader!=IPV6_HDR_AUTH &&
                       ExtHeader!=IPV6_HDR_DST_OPTIONS && ExtHeader!=IPV6_HDR_DST_OPTIONS)
                    break;  // Exit while
                pData += 1 + pOptions->m_HeaderExtLength;
                ExtHeader = pOptions->m_NextHeader;
            }
        }

        if (pUdpHeader != NULL)
        {
            TxFlags |= DTPCIENW_TXFLAGS_PROT_UDP;
            PortOffset = ((UInt8*)pUdpHeader) - ((UInt8*)pSkb->data) + sizeof(DtEthIp);
            PortOffset = PortOffset / 4;
            pData = (UInt8*)pUdpHeader + sizeof(UdpHeader);
            if (pUdpHeader->m_DestinationPort==0x0400 && strcmp(pData, "DektecArp\0")==0)
            {
                // It's our own dummy packet. Skip it.
                dev_kfree_skb(pSkb);
                return 0;
            }
            
        }
        else if (pTcpHeader != NULL)
        {
            TxFlags |= DTPCIENW_TXFLAGS_PROT_TCP;
            PortOffset = ((UInt8*)pTcpHeader) - ((UInt8*)pSkb->data) + sizeof(DtEthIp);
            PortOffset = PortOffset / 4;
        }
    }

    PacketLength = pSkb->len;
    if (PacketLength < DT_IP_MIN_ETH_PAYLOAD) 
        PacketLength = DT_IP_MIN_ETH_PAYLOAD;   // padding

    // Get pointer to destination buffer
    Status = DtPcieNwTxGetPointerNewPacket(pDvcData, PacketLength, &pPacket1, 
                                                   &PacketSize1, &pPacket2, &PacketSize2);
   if (!DT_SUCCESS(Status))
    {
        pDvcData->m_IalData.m_NetStats.tx_dropped++;
        return 1;
    }
    DT_ASSERT(PacketSize1 + PacketSize2 >= PacketLength + sizeof(DtEthIp));

    pData = pSkb->data;
    if (PacketSize1 <= sizeof(DtEthIp))
    {
        DT_ASSERT(PacketSize2 >= (sizeof(DtEthIp) - PacketSize1) + PacketLength);
        // Copy packet data into second buffer
        DtMemCopy(pPacket2+(sizeof(DtEthIp) - PacketSize1), pData, pSkb->len);
    }
    else {
        DT_ASSERT(PacketSize1 - sizeof(DtEthIp) + PacketSize2 >= PacketLength);
        if (PacketSize2 == 0)
        {
            DtMemCopy(pPacket1 + sizeof(DtEthIp), pData, pSkb->len);

            // clear padding bytes
            if (pSkb->len < PacketLength)
                DtMemZero(pPacket1 + sizeof(DtEthIp) + pSkb->len, 
                                                                PacketLength - pSkb->len);
        }
        else
        {
            Int  MaxToCopy = PacketSize1 - sizeof(DtEthIp);
            if (MaxToCopy > pSkb->len)
            {
                // clear padding bytes
                DtMemZero(pPacket1 + pSkb->len, PacketLength - pSkb->len);
                MaxToCopy = pSkb->len;
            }
            DtMemCopy(pPacket1 + sizeof(DtEthIp), pData, MaxToCopy);
            if (pSkb->len - MaxToCopy > 0)
                DtMemCopy(pPacket2, pData+MaxToCopy, pSkb->len - MaxToCopy);
            if (pSkb->len < PacketLength)
            {
                // clear padding bytes
                DtMemZero(pPacket2 + pSkb->len - MaxToCopy, PacketLength - 
                                                                 (pSkb->len - MaxToCopy));
            }
        }
    }
 
    
    // Check if we need to do task offloading
    if (pDvcData->m_GenerateChecksumFw)
    {
        if (pSkb->ip_summed == CHECKSUM_PARTIAL)
            TxFlags |= (DTPCIENW_TXFLAGS_GEN_CHKSUM_UDP | DTPCIENW_TXFLAGS_GEN_CHKSUM_IP |
                                                         DTPCIENW_TXFLAGS_GEN_CHKSUM_TCP);
    }
    
#ifdef HAVE_HW_TIME_STAMP
    if ((skb_shinfo(pSkb)->tx_flags & SKBTX_HW_TSTAMP) != 0)
    {
        Status = DtPcieNwGetFingerprint(pDvcData, pSkb, &Fingerprint);
        if (DT_SUCCESS(Status))
        {
            skb_shinfo(pSkb)->tx_flags |= SKBTX_IN_PROGRESS;
            TxFlags |= DTPCIENW_TXFLAGS_GEN_TX_TIMESTAMP;
        }
    }
#endif

    Status = DtPcieNwTransmitPacket(pDvcData, PacketLength, PacketSize1+PacketSize2, 
                                          TxFlags, Fingerprint, PortOffset, IpAddrOffset);
    if (!DT_SUCCESS(Status))
    {
        pDvcData->m_IalData.m_NetStats.tx_errors++;
        if ((TxFlags & DTPCIENW_TXFLAGS_GEN_TX_TIMESTAMP) != 0)
        {
#ifdef HAVE_HW_TIME_STAMP
            skb_shinfo(pSkb)->tx_flags &= ~SKBTX_IN_PROGRESS;
#endif
            DtPcieNwRemoveFingerprint(pDvcData, Fingerprint);
        }
        return 1;
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37)
    else
    {
        // Do we need to do software timestamping?
        if (skb_shinfo(pSkb)->tx_flags & SKBTX_SW_TSTAMP)
        {
            skb_tx_timestamp(pSkb);
        }
    }
#endif
    // Just to be save we take kernel version less then v3.1.0
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 1, 0)
    pDevice->trans_start = jiffies;
#else
    // trans_start is removed from the net_device struct starting from kernel v4.7.0
    // netif_trans_update function is introduced for legacy drivers
    // Some distribution (CentOs/RedHat 7.4) have backported above removal in kernel 
    // version v3.10 netdev_get_tx_queue function available starting from v2.6.27 kernel
    {
        struct netdev_queue *txq = netdev_get_tx_queue(pDevice, 0);
        if (txq->trans_start != jiffies)
            txq->trans_start = jiffies;
    }
#endif
    if ((TxFlags & DTPCIENW_TXFLAGS_GEN_TX_TIMESTAMP) == 0)
        dev_kfree_skb(pSkb);
    return 0;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtHwTxTimestampReceived -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtPcieNwEvtHwTxTimestampReceived(DtPcieNwDeviceData* pDvcData, void* pSkb,
                                                 UInt32 TodSeconds, UInt32 TodNanoSeconds)
{
    /*DektecTlvs DektecTlv;
    UInt8* pPacket = ((struct sk_buff*)pSkb)->data;
    UInt SizeInBytes = ((struct sk_buff*)pSkb)->len;
    Int RxFlags = DTPCIENW_RXFLAGS_IS_IP | DTPCIENW_RXFLAGS_IP_CHKSUM_CORRECT |
        DTPCIENW_RXFLAGS_UDP_CHKSUM_CORRECT;
    if (pPacket != NULL)
    {
        BuildPtpTxTimeTlv(TodSeconds, TodNanoSeconds, &DektecTlv.TimeTlv);
        UpdatePacketHeaders(pPacket, SizeInBytes, &DektecTlv.TimeTlv.TlvHdr);
        DtPcieNwEvtNewPacketRxCallback(pDvcData, TodSeconds, TodNanoSeconds, pPacket,
            SizeInBytes, RxFlags, &DektecTlv);
    }*/
#ifdef HAVE_HW_TIME_STAMP
    {
        struct skb_shared_hwtstamps  HwTimestamp;
        HwTimestamp.hwtstamp = ktime_set(TodSeconds, TodNanoSeconds);

        // Notify the stack and then free the skb
        skb_tstamp_tx(pSkb, &HwTimestamp);
    }
#endif
    dev_kfree_skb(pSkb);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtHwTxTimestampNotReceived -.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtPcieNwEvtHwTxTimestampNotReceived(DtPcieNwDeviceData* pDvcData, void* pSkb)
{
    dev_kfree_skb(pSkb);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtSetMulticastList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtPcieNwEvtSetMulticastList(struct net_device* pDevice)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    DtMulticastList  MulticastList;
    DtStatus  Status;
    MulticastList.m_pNetDevice = pDevice;
    Status = DtPcieNwSetMulticastList(pDvcData, netdev_mc_count(pDevice), &MulticastList);
    DtPcieNwIalSetPacketFilter(pDvcData, pDevice->flags | 
                             (Status == DT_STATUS_MULTICASTLIST_FULL ? IFF_ALLMULTI : 0));
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtGetStats -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static struct net_device_stats*  DtPcieNwEvtGetStats(struct net_device* pDevice)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    DtStatus  Status;
    UInt64  OneCollision = 0;
    UInt64  MoreCollisions = 0;
    UInt64  Counter;
    struct net_device_stats*  pNetStats = &pDvcData->m_IalData.m_NetStats;

    Status = DtPcieNwGetStatisticCounter(pDvcData, DT_MAC_CNT_GEN_XMIT_OK, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->tx_packets = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
       pNetStats->tx_packets = 0;
    Status = DtPcieNwGetStatisticCounter(pDvcData, DT_MAC_CNT_GEN_RCV_OK, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->rx_packets = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->rx_packets = 0;
    Status = DtPcieNwGetStatisticCounter(pDvcData, DT_MAC_CNT_GEN_XMIT_ERROR, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->tx_errors = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->tx_errors = 0;
    Status = DtPcieNwGetStatisticCounter(pDvcData, DT_MAC_CNT_GEN_RCV_ERROR, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->rx_errors = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->rx_errors = 0;
    Status = DtPcieNwGetStatisticCounter(pDvcData, DT_MAC_CNT_GEN_RCV_NO_BUFFER, 
                                                                                &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->rx_dropped = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->rx_dropped = 0;
    Status = DtPcieNwGetStatisticCounter(pDvcData, DT_MAC_CNT_GEN_RCV_CRC_ERROR, &Counter);
    if (DT_SUCCESS(Status)) 
        pDvcData->m_IalData.m_NetStats.rx_crc_errors = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->rx_crc_errors = 0;
    Status = DtPcieNwGetStatisticCounter(pDvcData, DT_MAC_CNT_802_3_XMIT_TIMES_CRS_LOST,
                                                                                &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->tx_carrier_errors = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->tx_carrier_errors = 0;
    Status = DtPcieNwGetStatisticCounter(pDvcData, DT_MAC_CNT_802_3_XMIT_LATE_COLLISIONS,
                                                                                &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->tx_aborted_errors = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->tx_aborted_errors = 0;
    Status = DtPcieNwGetStatisticCounter(pDvcData, DT_MAC_CNT_GEN_BYTES_RCV, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->rx_bytes = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->rx_bytes = 0;
    Status = DtPcieNwGetStatisticCounter(pDvcData, DT_MAC_CNT_GEN_BYTES_XMIT, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->tx_bytes = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->tx_bytes = 0;
    DtPcieNwGetStatisticCounter(pDvcData, DT_MAC_CNT_802_3_XMIT_ONE_COLLISION, 
                                                                           &OneCollision);
    DtPcieNwGetStatisticCounter(pDvcData, DT_MAC_CNT_802_3_XMIT_MORE_COLLISIONS, 
                                                                         &MoreCollisions);
    pNetStats->collisions = OneCollision + MoreCollisions;

#ifdef DTPCIENW_IAL_DUMPSTAT
    DtDbgOut(ERR, IAL, "tx_packets: %lu", pNetStats->tx_packets);
    DtDbgOut(ERR, IAL, "rx_packets: %lu", pNetStats->rx_packets);
    DtDbgOut(ERR, IAL, "tx_errors: %lu", pNetStats->tx_errors);
    DtDbgOut(ERR, IAL, "rx_errors: %lu", pNetStats->rx_errors);
    DtDbgOut(ERR, IAL, "rx_dropped: %lu", pNetStats->rx_dropped);
    DtDbgOut(ERR, IAL, "rx_crc_errors: %lu", pNetStats->rx_crc_errors);
    DtDbgOut(ERR, IAL, "tx_carrier_errors: %lu", pNetStats->tx_carrier_errors);
    DtDbgOut(ERR, IAL, "tx_aborted_errors: %lu", pNetStats->tx_aborted_errors);
    DtDbgOut(ERR, IAL, "rx_bytes: %lu", pNetStats->rx_bytes);
    DtDbgOut(ERR, IAL, "tx_bytes: %lu", pNetStats->tx_bytes);
    DtDbgOut(ERR, IAL, "tx_dropped: %lu", pNetStats->tx_dropped);
    DtDbgOut(ERR, IAL, "multicast: %lu", pNetStats->multicast);
    DtDbgOut(ERR, IAL, "tx_window_errors: %lu", pNetStats->tx_window_errors);
    DtDbgOut(ERR, IAL, "rx_length_errors: %lu", pNetStats->rx_length_errors);
    DtDbgOut(ERR, IAL, "rx_over_errors: %lu", pNetStats->rx_over_errors);
    DtDbgOut(ERR, IAL, "rx_frame_errors: %lu", pNetStats->rx_frame_errors);
    DtDbgOut(ERR, IAL, "rx_fifo_errors: %lu", pNetStats->rx_fifo_errors);
    DtDbgOut(ERR, IAL, "rx_missed_errors: %lu", pNetStats->rx_missed_errors);
    DtDbgOut(ERR, IAL, "tx_fifo_errors: %lu", pNetStats->tx_fifo_errors);
    DtDbgOut(ERR, IAL, "rx_compressed: %lu", pNetStats->rx_compressed);
    DtDbgOut(ERR, IAL, "rx_fifo_errors: %lu", pNetStats->rx_fifo_errors);
    DtDbgOut(ERR, IAL, "rx_fifo_errors: %lu", pNetStats->rx_fifo_errors);
    DtDbgOut(ERR, IAL, "tx_compressed: %lu", pNetStats->tx_compressed);
    DtDbgOut(ERR, IAL, "tx_heartbeat_errors: %lu", pNetStats->tx_heartbeat_errors);
    DtDbgOut(ERR, IAL, "collisions: %lu",pNetStats->collisions);
#endif
    return pNetStats;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtSetMacAddr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtPcieNwEvtSetMacAddr(struct net_device* pDevice, void* pAddr)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    DtStatus  Status;
    struct sockaddr*  pSockAddr = (struct sockaddr*)pAddr;

    Status = DtPcieNwSetMacAddress(pDvcData, pSockAddr->sa_data);
    if (!DT_SUCCESS(Status))
        return -EFAULT;

    // Update current MAC address
    DtMemCopy(pDvcData->m_MacAddressCur, pSockAddr->sa_data, 6);
    DtMemCopy((void*)pDevice->dev_addr, pSockAddr->sa_data, 6);
    return 0;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtGetPermAddr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#ifdef PERM_ADDR_SUPPORT
Int  DtPcieNwEvtGetPermAddr(
    struct net_device*  pDevice, 
    struct ethtool_perm_addr*  pAddr, 
    UInt8*  pData)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    
    if (pAddr->size < 6)
        return -ETOOSMALL;

    pAddr->size = 6;
    DtMemCopy(pData, pDvcData->m_MacAddressPerm, 6);
    return 0;
}
#endif

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtGetDriverInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtPcieNwEvtGetDriverInfo(
    struct net_device*  pDevice, 
    struct ethtool_drvinfo*  pInfo)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    DtDbgOut(MAX, IAL, "Start");
    strcpy(pInfo->driver, DRIVER_NAME);
    sprintf(pInfo->version, "%i.%i.%i.%i", DTPCIENW_VERSION_MAJOR, DTPCIENW_VERSION_MINOR,
                                          DTPCIENW_VERSION_MICRO, DTPCIENW_VERSION_BUILD);
    sprintf(pInfo->fw_version, "v%i", pDvcData->m_FwVersion);
    sprintf(pInfo->bus_info, "%i", pDvcData->m_BusNumber);
    DtDbgOut(MAX, IAL, "Exit");
}

#define ADVERTISED_MASK_10G (ADVERTISED_10000baseT_Full | ADVERTISED_10000baseKX4_Full | \
                                                              ADVERTISED_10000baseKR_Full)
#define SUPPORTED_MASK_10G (SUPPORTED_10000baseT_Full | SUPPORTED_10000baseKX4_Full |    \
                                                               SUPPORTED_10000baseKR_Full)

#define SUPPORTED_MASK_1G  (SUPPORTED_1000baseT_Full | SUPPORTED_1000baseKX_Full)
#define ADVERTISED_MASK_1G (ADVERTISED_1000baseT_Full | ADVERTISED_1000baseKX_Full)

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwGetSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtPcieNwGetSettings(DtPcieNwDeviceData*  pDvcData, Int* pAutoNeg, Int* pSpeed,
                                          Int* pDuplex, Int* pAdvertised, Int* pSupported)
{
    DtStatus  Status;
    UInt  Speed = 0;
    
    *pSupported = ADVERTISED_MASK_10G | ADVERTISED_MASK_1G;
    *pAdvertised = 0;
    
    if (pDvcData->m_IalData.m_AutoNegEn)
    {
        *pAdvertised |= (ADVERTISED_MASK_10G | ADVERTISED_MASK_1G);
        *pSupported |= SUPPORTED_Autoneg;
    }
    else {
        // check link speed
        *pAdvertised |= ADVERTISED_MASK_10G;
    }
    
    // Get phy speed
    Status = DtPcieNwGetPhySpeed(pDvcData, &Speed);
    if (!DT_SUCCESS(Status)) 
        return -EFAULT;

    *pAutoNeg = (pDvcData->m_IalData.m_AutoNegEn ? AUTONEG_ENABLE : AUTONEG_DISABLE);

    switch (Speed) 
    {
    case DT_PHY_SPEED_10_HALF:
        *pSpeed = SPEED_10;
        *pDuplex = DUPLEX_HALF;
        break;
    case DT_PHY_SPEED_10_FULL:
        *pSpeed = SPEED_10;
        *pDuplex = DUPLEX_FULL;
        break;
    case DT_PHY_SPEED_100_HALF:
        *pSpeed = SPEED_100;
        *pDuplex = DUPLEX_HALF;
        break;
    case DT_PHY_SPEED_100_FULL:
        *pSpeed = SPEED_100;
        *pDuplex = DUPLEX_FULL;
        break;
    case DT_PHY_SPEED_1000_MASTER:
    case DT_PHY_SPEED_1000_SLAVE:
        *pSpeed = SPEED_1000;
        *pDuplex = DUPLEX_FULL;
        break;
    case DT_PHY_SPEED_10000:
        *pSpeed = SPEED_10000;
        *pDuplex = DUPLEX_FULL;
        break;
    default:
        *pSpeed = 0;
        *pDuplex = 0;
    }

    return 0;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwSetSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtPcieNwSetSettings(DtPcieNwDeviceData* pDvcData, Int AutoNegSet, Int SpeedSet, 
                                                                            Int DuplexSet)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  Speed;

    if (AutoNegSet == AUTONEG_ENABLE)
    {
        Speed = DT_PHY_SPEED_AUTO_DETECT;
        pDvcData->m_IalData.m_AutoNegEn = TRUE;
    }
    else if (SpeedSet == SPEED_10)
    {
        if (DuplexSet == DUPLEX_HALF)
            Speed = DT_PHY_SPEED_10_HALF;
        else
            Speed = DT_PHY_SPEED_10_FULL;
        pDvcData->m_IalData.m_AutoNegEn = FALSE;
    }
    else if (SpeedSet == SPEED_100)
    {
        if (DuplexSet == DUPLEX_HALF)
            Speed = DT_PHY_SPEED_100_HALF;
        else
            Speed = DT_PHY_SPEED_100_FULL;
        pDvcData->m_IalData.m_AutoNegEn = FALSE;
    }
    else if (SpeedSet == SPEED_1000)
    {
        if (DuplexSet == DUPLEX_HALF)
            return -EINVAL;
        // Note: We set the 1Gb speed select to master here. But MASTER/SLAVE configuration
        // is automatically detected by phy.
        Speed = DTA_PHY_SPEED_1000_MASTER;
        pDvcData->m_IalData.m_AutoNegEn = FALSE;
    }
    else if (SpeedSet == SPEED_10000)
    {
        if (DuplexSet == DUPLEX_HALF)
            return -EINVAL;
        // Note: We set the 1Gb speed select to master here. But MASTER/SLAVE configuration
        // is automatically detected by phy.
        Speed = DT_PHY_SPEED_10000;
        pDvcData->m_IalData.m_AutoNegEn = FALSE;
    }
    else
        return -EINVAL;

    Status = DtPcieNwSetPhySpeed(pDvcData, Speed);

    if (!DT_SUCCESS(Status))
        return -EFAULT;
    return 0;
}

#ifdef GS_SETTINGS_SUPPORT
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtGetSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtPcieNwEvtGetSettings(struct net_device* pDevice, struct ethtool_cmd* pCmd)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    Int AutoNeg = 0;
    Int Speed = 0;
    Int Duplex = 0;
    Int Supported = 0;
    Int Advertised = 0;
    Int Return;
    DtDbgOut(MAX, IAL, "Start");


    memset(pCmd, 0, sizeof(struct ethtool_cmd));
    pCmd->port = PORT_TP;
    pCmd->transceiver = XCVR_INTERNAL;
    
    Return = DtPcieNwGetSettings(pDvcData, &AutoNeg, &Speed, &Duplex, &Advertised,
                                                                              &Supported);
    pCmd->supported = Supported;
    pCmd->advertising = Advertised;
    pCmd->autoneg = AutoNeg;
    pCmd->speed = Speed;
    pCmd->duplex = Duplex;

    DtDbgOut(MAX, IAL, "Exit");
    return Return;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtSetSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtPcieNwEvtSetSettings(struct net_device* pDevice, struct ethtool_cmd* pCmd)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    return DtPcieNwSetSettings(pDvcData, pCmd->autoneg, pCmd->speed, pCmd->duplex);
}
#else

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtSetLinkKSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtPcieNwEvtSetLinkKSettings(struct net_device*pDevice,
                                      const struct ethtool_link_ksettings* pLinkKSettings)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    Int  Speed = pLinkKSettings->base.speed;
    Int  AutoNeg = pLinkKSettings->base.autoneg;
    Int  Duplex = pLinkKSettings->base.duplex;
    return DtPcieNwSetSettings(pDvcData, AutoNeg, Speed, Duplex);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtGetLinkKSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtPcieNwEvtGetLinkKSettings(struct net_device*pDevice,
                                      struct ethtool_link_ksettings* pLinkKSettings)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    Int AutoNeg = 0;
    Int Speed = 0;
    Int Duplex = 0;
    Int Supported = 0;
    Int Advertised = 0;
    Int Return;
    DtDbgOut(MAX, IAL, "Start");
    Return = DtPcieNwGetSettings(pDvcData, &AutoNeg, &Speed, &Duplex, &Advertised,
                                                                              &Supported);
    pLinkKSettings->base.port = PORT_TP;
    pLinkKSettings->base.transceiver = XCVR_INTERNAL;
    pLinkKSettings->base.autoneg = AutoNeg;
    pLinkKSettings->base.speed = Speed;
    pLinkKSettings->base.duplex = Duplex;
    ethtool_convert_legacy_u32_to_link_mode(pLinkKSettings->link_modes.supported,
                                                                               Supported);
    ethtool_convert_legacy_u32_to_link_mode(pLinkKSettings->link_modes.advertising,
                                                                              Advertised);

    DtDbgOut(MAX, IAL, "Exit");
    return Return;
}
#endif


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtGetLink -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt32  DtPcieNwEvtGetLink(struct net_device* pDevice)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    UInt  PhyLink = DT_PHY_LINK_DOWN;
    
    DtPcieNwGetLinkStatus(pDvcData, &PhyLink);

    return (PhyLink == DT_PHY_LINK_UP ? 1 : 0);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtLinkStatusCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function will be called from the common DtPcieNw code to trigger a link status
// update
//
void  DtPcieNwEvtLinkStatusCallback(DtPcieNwDeviceData* pDvcData)
{
    switch(pDvcData->m_LinkStatus)
    {
    case DTPCIENW_LINKSTAT_UP: netif_carrier_on(pDvcData->m_IalData.m_pNwDevice); break;
    default: netif_carrier_off(pDvcData->m_IalData.m_pNwDevice);
    }
}

#ifdef NO_HW_FEATURE_SUPPORT
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtGetRxCsum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static UInt32  DtPcieNwEvtGetRxCsum(struct net_device* pDevice)
{
    return ((pDevice->features & NETIF_F_RXCSUM)!=0? 1 : 0);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtSetRxCsum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtPcieNwEvtSetRxCsum(struct net_device* pDevice, UInt32 Value)
{
    dt_netdev_features  Features = pDevice->features;
    if (Value != 0)
        Features|= NETIF_F_RXCSUM;
    else 
        Features &= ~NETIF_F_RXCSUM;

    Features = DtPcieNwEvtFixFeatures(pDevice, Features);
    DtPcieNwEvtSetFeatures(pDevice, Features);
    return 0;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtGetTxCsum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static UInt32  DtPcieNwEvtGetTxCsum(struct net_device* pDevice)
{
    return ((pDevice->features & (NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM))!=0? 1 : 0);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtSetTxCsum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtPcieNwEvtSetTxCsum(struct net_device* pDevice, u32 Value)
{
    dt_netdev_features  Features = pDevice->features;
    if (Value != 0)
        Features|= NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM;
    else 
        Features &= ~(NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM);

    Features = DtPcieNwEvtFixFeatures(pDevice, Features);
    DtPcieNwEvtSetFeatures(pDevice, Features);
    return 0;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtGetTsInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int DtPcieNwEvtGetTsInfo(struct net_device* pDevice, 
                                                          struct ethtool_ts_info* ts_info)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    ts_info->so_timestamping = SOF_TIMESTAMPING_TX_SOFTWARE |
        SOF_TIMESTAMPING_RX_SOFTWARE |
        SOF_TIMESTAMPING_SOFTWARE |
        SOF_TIMESTAMPING_TX_HARDWARE |
        SOF_TIMESTAMPING_RX_HARDWARE |
        SOF_TIMESTAMPING_RAW_HARDWARE;
    ts_info->tx_types = BIT(HWTSTAMP_TX_OFF)
                      | BIT(HWTSTAMP_TX_ON)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
                      | BIT(HWTSTAMP_TX_ONESTEP_SYNC)
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
                      | BIT(HWTSTAMP_TX_ONESTEP_P2P)
#endif
                        ;
    ts_info->rx_filters = BIT(HWTSTAMP_FILTER_NONE)
                        | BIT(HWTSTAMP_FILTER_ALL)
                        | BIT(HWTSTAMP_FILTER_PTP_V1_L4_EVENT)
                        | BIT(HWTSTAMP_FILTER_PTP_V1_L4_SYNC)
                        | BIT(HWTSTAMP_FILTER_PTP_V1_L4_DELAY_REQ)
                        | BIT(HWTSTAMP_FILTER_PTP_V2_EVENT)
                        | BIT(HWTSTAMP_FILTER_PTP_V2_SYNC)
                        | BIT(HWTSTAMP_FILTER_PTP_V2_DELAY_REQ);
    /*if (pDvcData->m_IalData.m_PtpHwClockIndex == -1)
    {
        // Find clock source
        pDvcData->m_IalData.m_PtpHwClockIndex = DtPcieNwFindPtpClockDevice(
                                       pDvcData->m_IalData.m_pDeviceItf->m_pParentDevice);
    }*/
    ts_info->phc_index = *pDvcData->m_IalData.m_pDeviceItf->m_pDriverItf->m_pPtpClockIndex;
    return 0;
}
#endif

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtFixFeatures -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
dt_netdev_features  DtPcieNwEvtFixFeatures(
    struct net_device*  pDevice,
    dt_netdev_features  Features)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    Bool  RxChkSum = (Features & NETIF_F_RXCSUM) != 0;

    // Ipv4 Tx checksum offloading
    if ((Features & NETIF_F_IP_CSUM) != 0)
    {
        // Do we support IPv4 checksum calculations in all situations
        if (!pDvcData->m_SupportIpTxChecksumIPv4Fw ||
                                                !pDvcData->m_SupportUdpTxChecksumIPv4Fw ||
                                                !pDvcData->m_SupportTcpTxChecksumIPv4Fw)
            Features &= ~NETIF_F_IP_CSUM;
    }

    // Ipv6 Tx checksum offloading
    if ((Features & NETIF_F_IPV6_CSUM) != 0)
    {
        // Do we support IPv6 checksum calculations in all situations
        if (!pDvcData->m_SupportUdpTxChecksumIPv6Fw ||
                                                  !pDvcData->m_SupportTcpTxChecksumIPv6Fw)
            Features &= ~NETIF_F_IPV6_CSUM;
    }

    // Rx checksum offloading
    if (RxChkSum && (!pDvcData->m_SupportIpRxChecksumIPv4Fw ||
                                                !pDvcData->m_SupportUdpRxChecksumIPv4Fw ||
                                                !pDvcData->m_SupportTcpRxChecksumIPv4Fw ||
                                                !pDvcData->m_SupportUdpRxChecksumIPv6Fw ||
                                                !pDvcData->m_SupportTcpRxChecksumIPv6Fw))
        Features &= ~NETIF_F_RXCSUM;

    if (g_DtPcieNwEnableFCS)
        Features |= NETIF_F_RXFCS;
    return Features;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtSetFeatures -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static int  DtPcieNwEvtSetFeatures(struct net_device* pDevice, dt_netdev_features Features)
{
    DtPcieNwDeviceData*  pDvcData = netdev_priv(pDevice);
    
    if (NETIF_F_RXFCS != 0)
        pDvcData->m_FCSEnabled = (Features & NETIF_F_RXFCS) != 0;
    pDvcData->m_GenerateChecksumFw = (Features & NETIF_F_IP_CSUM)!=0 ||
                                                        (Features & NETIF_F_IPV6_CSUM)!=0;
    pDvcData->m_CheckIpChecksumIPv4Fw = (Features & NETIF_F_RXCSUM) != 0;
    pDvcData->m_CheckUdpChecksumIPv4Fw = (Features & NETIF_F_RXCSUM) != 0;
    pDvcData->m_CheckUdpChecksumIPv6Fw = (Features & NETIF_F_RXCSUM) != 0;
    pDvcData->m_CheckTcpChecksumIPv4Fw = (Features & NETIF_F_RXCSUM) != 0;
    pDvcData->m_CheckTcpChecksumIPv6Fw = (Features & NETIF_F_RXCSUM) != 0;
    return 0;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtNewPacketRxCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function will be called from the common DtPcieNw code to trigger the arrival of
// a packet
//
void  DtPcieNwEvtNewPacketRxCallback(
    DtPcieNwDeviceData*  pDvcData,
    UInt32  TodSeconds,
    UInt32  TodNanoSeconds,
    UInt8*  pPacket,
    UInt  PacketSize,
    UInt  RxFlags,
    DektecTlvs* pDektecTlv)
{
    struct sk_buff*  pSkb;
    UInt8*  pData;
    UInt  PktLength = PacketSize;
#ifdef HAVE_HW_TIME_STAMP
    struct skb_shared_hwtstamps*  pHwTimestamp;
#endif
    Int TlvLength = NWCW(pDektecTlv->TlvHeader.Length);
    if (TlvLength > 0)
        TlvLength += sizeof(TlvHeader);
    PktLength += TlvLength;

    // Allocate packet buffer struct. Add 2 extra bytes for DWORD payload alignment
    pSkb = dev_alloc_skb(PktLength + 2);
    if (pSkb == NULL)
    {
        DtDbgOut(ERR, IAL, "Error allocating new packet struct");
        pDvcData->m_NwRcvNoBuffer++;
        pDvcData->m_IalData.m_NetStats.rx_dropped++;
        return;
    }
    pSkb->dev = pDvcData->m_IalData.m_pNwDevice;

    // Set RX timestamp
#ifdef HAVE_HW_TIME_STAMP
    pHwTimestamp = skb_hwtstamps(pSkb);
    pHwTimestamp->hwtstamp = ktime_set(TodSeconds, TodNanoSeconds);
#endif

    // Align start of headers on DWORD boundary (ethernet header length <> DWORD boundary)
    skb_reserve(pSkb, 2);
    
    // Copy packet into Skb buffer
    pData = skb_put(pSkb, PktLength);
    DtMemCopy(pData, (void*)pPacket, PacketSize);

    if (TlvLength != 0)
        DtMemCopy(pData + PacketSize, pDektecTlv, TlvLength);

    // Set pSkb->pkt_type, pSkb->mac and protocol fields
    pSkb->protocol = eth_type_trans(pSkb, pDvcData->m_IalData.m_pNwDevice);


    // Task offloading
    pSkb->ip_summed = CHECKSUM_NONE;

    if ((RxFlags & DTPCIENW_RXFLAGS_IS_IP) != 0)
    {
        if ((pDvcData->m_CheckIpChecksumIPv4Fw || pDvcData->m_CheckUdpChecksumIPv4Fw ||
            pDvcData->m_CheckUdpChecksumIPv6Fw || pDvcData->m_CheckTcpChecksumIPv4Fw ||
            pDvcData->m_CheckTcpChecksumIPv6Fw))
        {
            // Check if Ip/Udp/Tcp is detected and no errors occured
            if ((((RxFlags & DTPCIENW_RXFLAGS_IP_CHKSUM_CORRECT) != 0) || 
                                ((RxFlags & DTPCIENW_RXFLAGS_UDP_CHKSUM_CORRECT) != 0) || 
                                ((RxFlags & DTPCIENW_RXFLAGS_TCP_CHKSUM_CORRECT) != 0)) &&
                !(((RxFlags & DTPCIENW_RXFLAGS_IP_CHKSUM_FAIL) != 0) || 
                                    ((RxFlags & DTPCIENW_RXFLAGS_UDP_CHKSUM_FAIL) != 0) ||
                                    ((RxFlags & DTPCIENW_RXFLAGS_TCP_CHKSUM_FAIL) != 0)))
                pSkb->ip_summed = CHECKSUM_UNNECESSARY;
        }
    }

    // Give packet to the network layer. Network layer will cleanup the packet
    netif_rx(pSkb);

#if defined(RHEL_RELEASE_CODE)
#if RHEL_RELEASE_CODE>=RHEL_RELEASE_VERSION(7,5)
#define SKIP_LAST_RX_FOR_RHEL
#endif
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0) && !defined(SKIP_LAST_RX_FOR_RHEL)
    // last_rx is removed from the net_device struct starting from Linux 
    // kernel version >=4.11 and is not replaced by something else. 
    pDvcData->m_IalData.m_pNwDevice->last_rx = jiffies;
#endif
    pDvcData->m_IalData.m_NetStats.rx_bytes+= PacketSize;
    pDvcData->m_IalData.m_NetStats.rx_packets++;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPcieNw driver communication +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtProbe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is executed by the DtPcie driver when a child network device must be
// created
//
Int  DtPcieNwEvtProbe(DtDeviceItf* pDevItf, Int Id)
{
    Int  Result = 0;
    UInt  DvcIndex;
    struct net_device*  pNwDev;
    DtPcieNwDeviceData*  pDvcData;

    DtDbgOut(MAX, IAL, "Start");

    // Quick check if maximum device count is reached
    if (g_DtPcieNwDeviceCount < g_DtPcieNwMaxDevices)
    {
        // Find free DvcIndex
        for (DvcIndex=0; DvcIndex<g_DtPcieNwMaxDevices; DvcIndex++)
        {
            if (g_pDtPcieNwDeviceData[DvcIndex] == NULL)
                break;
        }
        if (DvcIndex == g_DtPcieNwMaxDevices)
            Result = -ENOMEM;
    } else
        Result = -ENOMEM;

    if (Result >= 0) 
    {
        // Initialise a network device structure
        pNwDev = alloc_etherdev(sizeof(DtPcieNwDeviceData));
        if (pNwDev == NULL) 
        {
            DtDbgOut(ERR, IAL, "Error allocating network device structure");
            Result = -ENOMEM;
        }
    }

    if (Result >= 0)
    {
        Bool  SupportHwChkSum;
        
        // Initialise device struct for default ethernet values
        ether_setup(pNwDev);
        pDvcData = netdev_priv(pNwDev);

        // Initialize device data to default values
        DtMemZero(pDvcData, sizeof(DtPcieNwDeviceData));
            
        // Store handle to pDvcData in array
        g_pDtPcieNwDeviceData[DvcIndex] = pDvcData;

        // Initialize the IAL data
        pDvcData->m_IalData.m_DvcIndex = DvcIndex;
        pDvcData->m_IalData.m_pNwDevice = pNwDev;
        pDvcData->m_IalData.m_Id = Id;
        pDvcData->m_IalData.m_pDeviceItf = pDevItf;
        pDvcData->m_IalData.m_AutoNegEn = TRUE;
        
        pDvcData->m_PortIndex = Id;
        
        // Initialize the IoCtrl parent
        pDvcData->m_IoCtrlParent.m_pParentFunc = pDevItf->m_pDriverItf->IoCtrl;
        pDvcData->m_IoCtrlParent.m_pContext = pDevItf->m_pDriverItf->m_pContext;

        //pDvcData->m_IalData.m_PtpHwClockIndex = -1;
        
        if (!DT_SUCCESS(DtPcieNwDeviceDataInit(pDvcData)))
            Result = -EFAULT;

        if (!DT_SUCCESS(DtPcieNwDeviceInit(pDvcData)))
        {
            Result = -EFAULT;
            DtPcieNwDeviceExit(pDvcData);
        }
        // We support Frame Check Sequence
#ifndef NO_HW_FEATURE_SUPPORT
        pNwDev->hw_features = NETIF_F_RXFCS;
#endif
        if (g_DtPcieNwEnableFCS)
        {
             pNwDev->features |= NETIF_F_RXFCS;
             pDvcData->m_FCSEnabled = TRUE;
             DtDbgOut(MIN, IAL, "Frame Check Sequence turned on GLOBAL.");
        }

        // Set hardware checksum support
        SupportHwChkSum = TRUE;
        pDvcData->m_SupportIpRxChecksumIPv4Fw = SupportHwChkSum;
        pDvcData->m_SupportIpTxChecksumIPv4Fw = SupportHwChkSum;
        pDvcData->m_SupportUdpRxChecksumIPv4Fw = SupportHwChkSum;
        pDvcData->m_SupportUdpRxChecksumIPv6Fw = SupportHwChkSum;
        pDvcData->m_SupportUdpTxChecksumIPv4Fw = SupportHwChkSum;
        pDvcData->m_SupportUdpTxChecksumIPv6Fw = SupportHwChkSum;
        pDvcData->m_SupportTcpRxChecksumIPv4Fw = SupportHwChkSum;
        pDvcData->m_SupportTcpRxChecksumIPv6Fw = SupportHwChkSum;
        pDvcData->m_SupportTcpTxChecksumIPv4Fw = SupportHwChkSum;
        pDvcData->m_SupportTcpTxChecksumIPv6Fw = SupportHwChkSum;

        if (SupportHwChkSum)
        {
            // Support hardware checksum calculations for IPv4 and IPv6 and
            // enable it for default
#ifndef NO_HW_FEATURE_SUPPORT
            pNwDev->hw_features |= (NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM | NETIF_F_RXCSUM);
            pNwDev->vlan_features |= (NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM |
                                                                          NETIF_F_RXCSUM);
#endif
            pNwDev->features |= (NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM | NETIF_F_RXCSUM);
            
        }
    }

    if (Result >= 0)
    {
        DtMemCopy((void*)pNwDev->dev_addr, pDvcData->m_MacAddressCur, 6);
#ifdef PERM_ADDR_SUPPORT
        DtMemCopy((void*)pNwDev->perm_addr, pDvcData->m_MacAddressPerm, 6);
#endif
        pDvcData->m_IalData.m_Driver.m_pModule = THIS_MODULE;
        DtEvtLogInit(&pDvcData->m_IalData.m_Driver.m_EvtObject, &EventGetMessage);
         
        // Set callback routines
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)
        pNwDev->netdev_ops = &NetDevOps;
#else
        pNwDev->open = &DtPcieNwEvtOpen;
        pNwDev->hard_start_xmit = &DtPcieNwEvtStartTx;
        pNwDev->stop = &DtPcieNwEvtClose;
        pNwDev->get_stats = &DtPcieNwEvtGetStats;
        pNwDev->set_multicast_list = &DtPcieNwEvtSetMulticastList;
        pNwDev->set_mac_address = &DtPcieNwEvtSetMacAddr;
#endif

#ifdef SET_ETHTOOL_OPS
        SET_ETHTOOL_OPS(pNwDev, &EthToolOps);
#else
        ( (pNwDev)->ethtool_ops = (&EthToolOps) );
#endif

        Result = register_netdev(pNwDev);
        if (Result != 0) 
            DtDbgOut(ERR, IAL, "Failed to register network driver for DTA-%d at bus %d"
                                           " and slot %d", pDvcData->m_TypeNumber,
                                           pDvcData->m_BusNumber, pDvcData->m_SlotNumber);
        else {
            // To show this message without building with DEBUG=1 we use printk directly
            printk(KERN_DEBUG DRIVER_NAME ": Registered network card '%s' for DTA-%d "
                  "%02x:%02x:%02x:%02x:%02x:%02x\n", pNwDev->name, pDvcData->m_TypeNumber,
                  pNwDev->dev_addr[0], pNwDev->dev_addr[1], pNwDev->dev_addr[2],
                  pNwDev->dev_addr[3], pNwDev->dev_addr[4], pNwDev->dev_addr[5]);
        }
        // Increment device count
        g_DtPcieNwDeviceCount++;
        
        DtDbgOut(AVG, IAL, "New device count: %u", g_DtPcieNwDeviceCount);
    }
    
    DtDbgOut(MAX, IAL, "Exit");
    return Result; 
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwEvtRemove -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is executed by the Dta driver when the child network device must
// be removed from the system
//
void  DtPcieNwEvtRemove(DtDeviceItf* pDevItf, Int Id)
{
    UInt  DvcIndex;

    // Search for the device in the device struct
    for (DvcIndex=0; DvcIndex<g_DtPcieNwMaxDevices; DvcIndex++)
    {
        if (g_pDtPcieNwDeviceData[DvcIndex] != NULL)
        {
            DtPcieNwIalData*  pIalData = &g_pDtPcieNwDeviceData[DvcIndex]->m_IalData;
            if (pIalData->m_pDeviceItf->m_pParentDevice==pDevItf->m_pParentDevice && 
                                                                       pIalData->m_Id==Id)
            {
                DtPcieNwDeviceExit(g_pDtPcieNwDeviceData[DvcIndex]);
                unregister_netdev(pIalData->m_pNwDevice);
                free_netdev(pIalData->m_pNwDevice);

                g_pDtPcieNwDeviceData[DvcIndex] = NULL;
                g_DtPcieNwDeviceCount--;
                break;
            }
        }
    }
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private helper functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwIalSetPacketFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtPcieNwIalSetPacketFilter(DtPcieNwDeviceData* pDvcData, Int Flags)
{
    UInt  PacketFilter = 0;

    if (Flags != 0)
        PacketFilter |= DT_MAC_FLT_DIRECTED;
    if ((Flags & IFF_PROMISC) != 0)
        PacketFilter |= DT_MAC_FLT_PROMISCUOUS;
    if ((Flags & IFF_ALLMULTI) != 0)
        PacketFilter |= DT_MAC_FLT_ALL_MULTICAST;
    if ((Flags & IFF_BROADCAST) != 0)
        PacketFilter |= DT_MAC_FLT_BROADCAST;
    if ((Flags & IFF_MULTICAST) != 0)
        PacketFilter |= DT_MAC_FLT_MULTICAST;

    return DtPcieNwSetPacketFilter(pDvcData, PacketFilter);
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Module init / exit +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwModuleInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtPcieNwModuleInit(void)
{
    Int  Result = 0;
    DtStatus  Status;
    DtEvtLog  EvtObject;
    
    DtDbgOut(MAX, IAL, "Start");

    // Call init in common driver
    DtEvtLogInit(&EvtObject, &EventGetMessage);
    
    Status = DtPcieNwDriverInit(&EvtObject);
    if (!DT_SUCCESS(Status))
        Result = -EBUSY;

    if (Result >= 0)
    {
        // Allocate memory for device context pointers
        g_pDtPcieNwDeviceData = kmalloc(g_DtPcieNwMaxDevices * 
                                                 sizeof(DtPcieNwDeviceData*), GFP_KERNEL);
        if (g_pDtPcieNwDeviceData != NULL)
            memset(g_pDtPcieNwDeviceData, 0, 
                                      g_DtPcieNwMaxDevices * sizeof(DtPcieNwDeviceData*));
        else
            Result = -EBUSY;
    }

    if (Result >= 0)
    {
        // We try to register with the Dta driver and wait for the Probe function
        // The probe does the actual Network port creation.
        g_DtDriverItf.m_pName = DRIVER_NAME;
        g_DtDriverItf.Probe = DtPcieNwEvtProbe;
        g_DtDriverItf.Remove = DtPcieNwEvtRemove;
        DtCorePcie_IAL_RegisterChildDriver(&g_DtDriverItf);
    }
    DtDbgOut(MAX, IAL, "Exit");
    return Result;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieNwModuleExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtPcieNwModuleExit(void)
{
    Int  DvcIndex;
    DtDbgOut(MAX, IAL, "Start");

    if (g_DtPcieNwDeviceCount != 0)
    {
        DtDbgOut(ERR, IAL, "DtPcieNwModuleExit called, but DtPcieNwDeviceCount is not"
                                                    " 0 but (%u)", g_DtPcieNwDeviceCount);

        for (DvcIndex=0; DvcIndex<g_DtPcieNwMaxDevices; DvcIndex++)
        {
            if (g_pDtPcieNwDeviceData[DvcIndex] != NULL)
            {
                DtPcieNwEvtRemove(g_pDtPcieNwDeviceData[DvcIndex]->m_IalData.m_pDeviceItf, 
                                         g_pDtPcieNwDeviceData[DvcIndex]->m_IalData.m_Id);
                if (g_DtPcieNwDeviceCount == 0)
                    break;
                
            }
        }
    }
    DtCorePcie_IAL_UnRegisterChildDriver(&g_DtDriverItf);


    // Call exit in common driver
    DtPcieNwDriverExit();

    // Free memory for device context pointers
    if (g_pDtPcieNwDeviceData != NULL)
        kfree(g_pDtPcieNwDeviceData);
    DtDbgOut(MAX, IAL, "Exit");
}

module_init(DtPcieNwModuleInit);
module_exit(DtPcieNwModuleExit);





