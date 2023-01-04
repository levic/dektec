//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaNwIal.c *#*#*#*#*#*#*#*#* (C) 2011-2016 DekTec
//
// DtaNw driver - Implementation of the Linux Network interface.
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
#include <DtaNwIncludes.h>
#include <DtaIal.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Compile time defaults
#ifndef DTANW_MAX_DEVICES
#define  DTANW_MAX_DEVICES 10
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Global variables
Int  g_DtaNwMaxDevices = DTANW_MAX_DEVICES;
bool  g_DtaNwEnableFCS = false;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Global variables -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Load time parameters
module_param(g_DtaNwMaxDevices, int, S_IRUGO);  // Do not change int to Int!
module_param(g_DtaNwEnableFCS, bool, S_IRUGO);  // Do not change bool to Bool!
MODULE_PARM_DESC(g_DtaNwEnableFCS, 
                           "Enable Frame Check Sequence (0=disable [default], 1=enable)");


MODULE_AUTHOR("DekTec Digital Video B.V.");
MODULE_DESCRIPTION("Dta Series Network Driver");

// ADDED TO PREVENT TAINTED MESSAGE. THIS MODULE CAN BE USED FREELY (SEE LICENSE
// STATEMENT ABOVE).
MODULE_LICENSE("GPL";)

// Global variables
static DtaNwDeviceData**  g_pDtaNwDeviceData = NULL;
static Int  g_DtaNwDeviceCount = 0;
static DtaDriverItf  g_DtaDriverItf;



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Forward declarations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Private helper functions
static DtStatus  DtaNwIalSetPacketFilter(DtaNwDeviceData* pDvcData, Int Flags);

// Network interface
Int  DtaNwEvtOpen(struct net_device* pDevice);
Int  DtaNwEvtClose(struct net_device* pDevice);
Int  DtaNwEvtStartTx(struct sk_buff* pSkb, struct net_device* pDevice);
void  DtaNwEvtSetMulticastList(struct net_device* pDevice);
static struct net_device_stats*  DtaNwEvtGetStats(struct net_device* pDevice);
static Int  DtaNwEvtSetMacAddr(struct net_device* pDevice, void* pAddr);
#ifdef PERM_ADDR_SUPPORT
Int  DtaNwEvtGetPermAddr(struct net_device* pDevice, struct ethtool_perm_addr* pAddr, 
                                                                            UInt8 *pData);
#endif
static void  DtaNwEvtGetDriverInfo(struct net_device* pDevice, 
                                                           struct ethtool_drvinfo* pInfo);
#ifdef GS_SETTINGS_SUPPORT
static Int  DtaNwEvtGetSettings(struct net_device* pDevice, struct ethtool_cmd* pCmd);
static Int  DtaNwEvtSetSettings(struct net_device* pDevice, struct ethtool_cmd* pCmd);
#else
static Int  DtaNwEvtGetLinkKSettings(struct net_device* pDevice, 
                                           struct ethtool_link_ksettings* pLinkKSettings);
static Int  DtaNwEvtSetLinkKSettings(struct net_device* pDevice, 
                                     const struct ethtool_link_ksettings* pLinkKSettings);
#endif
UInt32  DtaNwEvtGetLink(struct net_device* pDevice);
#ifdef NO_HW_FEATURE_SUPPORT
static UInt32  DtaNwEvtGetRxCsum(struct net_device* pDevice);
static Int  DtaNwEvtSetRxCsum(struct net_device* pDevice, UInt32 Value);
static UInt32  DtaNwEvtGetTxCsum(struct net_device* pDevice);
static Int  DtaNwEvtSetTxCsum(struct net_device* pDevice, u32 Value);
#endif
static dta_netdev_features  DtaNwEvtFixFeatures(struct net_device* pDevice,
                                                            dta_netdev_features Features);
static int  DtaNwEvtSetFeatures(struct net_device* pDevice, dta_netdev_features Features);

// Kernel module interface
static Int  DtaNwModuleInit(void);
static void  DtaNwModuleExit(void);


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Interface declarations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Network interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
static const struct ethtool_ops  EthToolOps = 
{
    .get_drvinfo = DtaNwEvtGetDriverInfo,
#ifdef GS_SETTINGS_SUPPORT
    .get_settings = DtaNwEvtGetSettings,
    .set_settings = DtaNwEvtSetSettings,
#else
    .get_link_ksettings = DtaNwEvtGetLinkKSettings,
    .set_link_ksettings = DtaNwEvtSetLinkKSettings,
#endif
    .get_link = DtaNwEvtGetLink,
#ifdef PERM_ADDR_SUPPORT
    .get_perm_addr = DtaNwEvtGetPermAddr,
#endif
#ifdef NO_HW_FEATURE_SUPPORT
    .get_rx_csum = DtaNwEvtGetRxCsum,
    .set_rx_csum = DtaNwEvtSetRxCsum,
    .get_tx_csum = DtaNwEvtGetTxCsum,
    .set_tx_csum = DtaNwEvtSetTxCsum,
#endif
};

// Starting at kernel 2.6.29 the net_device callback functions should be placed in the 
// net_device_ops structure
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)

static const struct net_device_ops  NetDevOps = 
{
    .ndo_open = DtaNwEvtOpen,
    .ndo_start_xmit = DtaNwEvtStartTx,
    .ndo_stop = DtaNwEvtClose,
    .ndo_get_stats = DtaNwEvtGetStats,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
    .ndo_set_rx_mode = DtaNwEvtSetMulticastList,
#else
    .ndo_set_multicast_list = DtaNwEvtSetMulticastList,
#endif
    .ndo_set_mac_address = DtaNwEvtSetMacAddr,
#ifndef NO_HW_FEATURE_SUPPORT
    .ndo_fix_features = DtaNwEvtFixFeatures,
    .ndo_set_features = DtaNwEvtSetFeatures,
#endif
};
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaNwEvtOpen(struct net_device* pDevice)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    
    DtDbgOut(MAX, IAL, "Start");

    if (!DT_SUCCESS(DtaNwDeviceOpen(pDvcData)))
    {
        DtaNwDeviceClose(pDvcData);
        return -EFAULT;
    }
    if (!DT_SUCCESS(DtaNwIalSetPacketFilter(pDvcData, pDevice->flags)))
        return -EFAULT;

    netif_start_queue(pDevice);

    DtDbgOut(MAX, IAL, "Exit");
    
    return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaNwEvtClose(struct net_device* pDevice)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    
    DtDbgOut(MAX, IAL, "Start");

    DtaNwDeviceClose(pDvcData);
    netif_stop_queue(pDevice);
    
    DtDbgOut(MAX, IAL, "Exit");
    
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwStartTx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// The stack will execute DtaNwEvtStartTx not in parallel.
// (NETIF_F_LLTX flag is not set)
// If an error occurs, return 1 and do not free the packet!
//
Int  DtaNwEvtStartTx(struct sk_buff* pSkb, struct net_device* pDevice)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    UInt  PacketLength;
    UInt8*  pPacket;
    DtStatus  Status;
    UInt  WriteOffset;
    Bool  EnUdpChecksumGen = FALSE;
    Bool  EnTcpChecksumGen = FALSE;
    Bool  EnIpChecksumGen = FALSE;

    // Check if it's our own dummy generated packet
    if ((pSkb->len <= 200) && (pSkb->len >= (sizeof(EthernetIIHeader) + 
                                        sizeof(IpHeaderV4) - 5 + sizeof(UdpHeader) + 10)))
    {
        EthernetIIHeader*  pEthernetIIHeader = (EthernetIIHeader*)pSkb->data;
        EthernetIIHeaderVlan*  pEthernetIIHeaderVlan = (EthernetIIHeaderVlan*)pSkb->data;
        IpHeaderV4*  pIpHeaderV4 = NULL;
        IpHeaderV6*  pIpHeaderV6 = NULL;
        UdpHeader*  pUdpHeader = NULL;
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
            pIpHeaderV4 = (IpHeaderV4*)(pSkb->data + EthHeaderSize);
        else if (EthType == ETH_TYPE_IPV6_BE)
            pIpHeaderV6 = (IpHeaderV6*)(pSkb->data + EthHeaderSize);

        if (pIpHeaderV4 != NULL)
            pUdpHeader = (UdpHeader*)((UInt8*)pIpHeaderV4 + sizeof(IpHeaderV4) - 5);

        if (pIpHeaderV6 != NULL)
        {
            // Search for UDP header
            IpV6Options*  pOptions;
            UInt  ExtHeader = pIpHeaderV6->m_NextHeader;
            pData = (UInt8*)pIpHeaderV6 + sizeof(IpHeaderV6);
            
            while (pUdpHeader == NULL)
            {
                pOptions = (IpV6Options*)pData;
                if (ExtHeader == IPV4_HDR_PROT_UDP)
                    pUdpHeader = (UdpHeader*)pData;
                else if (ExtHeader!=IPV6_HDR_HOPBYHOP && ExtHeader!=IPV6_HDR_AUTH &&
                       ExtHeader!=IPV6_HDR_DST_OPTIONS && ExtHeader!=IPV6_HDR_DST_OPTIONS)
                    break;  // Exit while
                pData += 1 + pOptions->m_HeaderExtLength;
                ExtHeader = pOptions->m_NextHeader;
            }
        }

        if (pUdpHeader != NULL)
        {
            pData = (UInt8*)pUdpHeader + sizeof(UdpHeader);
            if (pUdpHeader->m_DestinationPort==0x0400 && strcmp(pData, "DektecArp\0")==0)
            {
                // It's our own dummy packet. Skip it.
                dev_kfree_skb(pSkb);
                return 0;
            }
        }
    }

    PacketLength = pSkb->len;
    if (PacketLength < DT_IP_MIN_ETH_PAYLOAD) 
        PacketLength = DT_IP_MIN_ETH_PAYLOAD;   // padding

    // Get pointer to destination buffer
    Status = DtaNwTxGetPointerNewPacket(pDvcData, PacketLength, &pPacket, &WriteOffset);
    if (!DT_SUCCESS(Status))
    {
        pDvcData->m_IalData.m_NetStats.tx_dropped++;
        return 1;
    }
    DtMemCopy(pPacket, pSkb->data, pSkb->len);

    if (pSkb->len < PacketLength)
        DtMemZero(pPacket+pSkb->len, PacketLength-pSkb->len);

    // Check if we need to do task offloading
    if (pDvcData->m_GenerateChecksumFw)
    {
        EnUdpChecksumGen = (pSkb->ip_summed == CHECKSUM_PARTIAL);
        EnTcpChecksumGen = (pSkb->ip_summed == CHECKSUM_PARTIAL);
        EnIpChecksumGen = (pSkb->ip_summed == CHECKSUM_PARTIAL);
    }
    Status = DtaNwTransmitPacket(pDvcData, PacketLength, WriteOffset, EnIpChecksumGen,
                                                      EnUdpChecksumGen, EnTcpChecksumGen);
    if (!DT_SUCCESS(Status))
    {
        pDvcData->m_IalData.m_NetStats.tx_errors++;
        return 1;
    }

    // Just to be save we take kernel version less then v3.1.0
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 1, 0)
    pDevice->trans_start = jiffies;
#else
    // trans_start is removed from the net_device struct starting from kernel v4.7.0
    // netif_trans_update function is introduced for legacy drivers
    // Some distribution (CentOs/RedHat 7.4) have backported above removal in kernel version v3.10
    // netdev_get_tx_queue function available starting from v2.6.27 kernel
    {
        struct netdev_queue *txq = netdev_get_tx_queue(pDevice, 0);
        if (txq->trans_start != jiffies)
            txq->trans_start = jiffies;
    }
#endif
    dev_kfree_skb(pSkb);
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtSetMulticastList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaNwEvtSetMulticastList(struct net_device* pDevice)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    DtMulticastList  MulticastList;
    DtStatus  Status;
    MulticastList.m_pNetDevice = pDevice;
    Status = DtaNwSetMulticastList(pDvcData, netdev_mc_count(pDevice), &MulticastList);
    DtaNwIalSetPacketFilter(pDvcData, pDevice->flags | 
                             (Status == DT_STATUS_MULTICASTLIST_FULL ? IFF_ALLMULTI : 0));
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtGetStats -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static struct net_device_stats*  DtaNwEvtGetStats(struct net_device* pDevice)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    DtStatus  Status;
    UInt64  OneCollision = 0;
    UInt64  MoreCollisions = 0;
    UInt64  Counter;
    struct net_device_stats*  pNetStats = &pDvcData->m_IalData.m_NetStats;

    Status = DtaNwGetStatisticCounter(pDvcData, DTA_MAC_CNT_GEN_XMIT_OK, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->tx_packets = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
       pNetStats->tx_packets = 0;
    Status = DtaNwGetStatisticCounter(pDvcData, DTA_MAC_CNT_GEN_RCV_OK, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->rx_packets = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->rx_packets = 0;
    Status = DtaNwGetStatisticCounter(pDvcData, DTA_MAC_CNT_GEN_XMIT_ERROR, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->tx_errors = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->tx_errors = 0;
    Status = DtaNwGetStatisticCounter(pDvcData, DTA_MAC_CNT_GEN_RCV_ERROR, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->rx_errors = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->rx_errors = 0;
    Status = DtaNwGetStatisticCounter(pDvcData, DTA_MAC_CNT_GEN_RCV_NO_BUFFER, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->rx_dropped = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->rx_dropped = 0;
    Status = DtaNwGetStatisticCounter(pDvcData, DTA_MAC_CNT_GEN_RCV_CRC_ERROR, &Counter);
    if (DT_SUCCESS(Status)) 
        pDvcData->m_IalData.m_NetStats.rx_crc_errors = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->rx_crc_errors = 0;
    Status = DtaNwGetStatisticCounter(pDvcData, DTA_MAC_CNT_802_3_XMIT_TIMES_CRS_LOST,
                                                                                &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->tx_carrier_errors = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->tx_carrier_errors = 0;
    Status = DtaNwGetStatisticCounter(pDvcData, DTA_MAC_CNT_802_3_XMIT_LATE_COLLISIONS,
                                                                                &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->tx_aborted_errors = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->tx_aborted_errors = 0;
    Status = DtaNwGetStatisticCounter(pDvcData, DTA_MAC_CNT_GEN_BYTES_RCV, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->rx_bytes = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->rx_bytes = 0;
    Status = DtaNwGetStatisticCounter(pDvcData, DTA_MAC_CNT_GEN_BYTES_XMIT, &Counter);
    if (DT_SUCCESS(Status)) 
        pNetStats->tx_bytes = (unsigned long)Counter;
    else if (Status == DT_STATUS_NOT_FOUND)
        pNetStats->tx_bytes = 0;
    DtaNwGetStatisticCounter(pDvcData, DTA_MAC_CNT_802_3_XMIT_ONE_COLLISION, 
                                                                           &OneCollision);
    DtaNwGetStatisticCounter(pDvcData, DTA_MAC_CNT_802_3_XMIT_MORE_COLLISIONS, 
                                                                         &MoreCollisions);
    pNetStats->collisions = OneCollision + MoreCollisions;

#ifdef DTANW_IAL_DUMPSTAT
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtSetMacAddr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtaNwEvtSetMacAddr(struct net_device* pDevice, void* pAddr)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    DtStatus  Status;
    struct sockaddr*  pSockAddr = (struct sockaddr*)pAddr;

    Status = DtaNwSetMacAddress(pDvcData, pSockAddr->sa_data);
    if (!DT_SUCCESS(Status))
        return -EFAULT;

    // Update current MAC address
    DtMemCopy(pDvcData->m_MacAddressCur, pSockAddr->sa_data, 6);
    DtMemCopy((void*)pDevice->dev_addr, pSockAddr->sa_data, 6);
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtGetPermAddr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#ifdef PERM_ADDR_SUPPORT
Int  DtaNwEvtGetPermAddr(
    struct net_device*  pDevice, 
    struct ethtool_perm_addr*  pAddr, 
    UInt8*  pData)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    
    if (pAddr->size < 6)
        return -ETOOSMALL;

    pAddr->size = 6;
    DtMemCopy(pData, pDvcData->m_MacAddressPerm, 6);
    return 0;
}
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtGetDriverInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtaNwEvtGetDriverInfo(
    struct net_device*  pDevice, 
    struct ethtool_drvinfo*  pInfo)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    DtDbgOut(MAX, IAL, "Start");
    strcpy(pInfo->driver, DRIVER_NAME);
    sprintf(pInfo->version, "%i.%i.%i.%i", DTANW_VERSION_MAJOR, DTANW_VERSION_MINOR,
                                          DTANW_VERSION_MICRO, DTANW_VERSION_BUILD);
    sprintf(pInfo->fw_version, "v%i", pDvcData->m_FwVersion);
    sprintf(pInfo->bus_info, "%i", pDvcData->m_BusNumber);
    DtDbgOut(MAX, IAL, "Exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwGetSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtaNwGetSettings(DtaNwDeviceData*  pDvcData, Int* pAutoNeg, Int* pSpeed,
                                          Int* pDuplex, Int* pAdvertised, Int* pSupported)
{
    DtStatus  Status;
    UInt  Speed = 0;
    
    *pSupported = SUPPORTED_10baseT_Half | SUPPORTED_10baseT_Full |
                               SUPPORTED_100baseT_Half | SUPPORTED_100baseT_Full |
                               SUPPORTED_1000baseT_Full |SUPPORTED_Autoneg | SUPPORTED_TP;
    *pAdvertised = ADVERTISED_TP;
    if (pDvcData->m_IalData.m_AutoNegEn)
    {
        *pAdvertised |= ADVERTISED_10baseT_Half | ADVERTISED_10baseT_Full |
                                     ADVERTISED_100baseT_Half | ADVERTISED_100baseT_Full |
                                     ADVERTISED_1000baseT_Full; 
    }
    
    // Get phy speed
    Status = DtaNwGetPhySpeed(pDvcData, &Speed);
    if (!DT_SUCCESS(Status)) 
        return -EFAULT;

    *pAutoNeg = (pDvcData->m_IalData.m_AutoNegEn ? AUTONEG_ENABLE : AUTONEG_DISABLE);

    switch (Speed) 
    {
    case DTA_PHY_SPEED_10_HALF:
        *pSpeed = SPEED_10;
        *pDuplex = DUPLEX_HALF;
        break;
    case DTA_PHY_SPEED_10_FULL:
        *pSpeed = SPEED_10;
        *pDuplex = DUPLEX_FULL;
        break;
    case DTA_PHY_SPEED_100_HALF:
        *pSpeed = SPEED_100;
        *pDuplex = DUPLEX_HALF;
        break;
    case DTA_PHY_SPEED_100_FULL:
        *pSpeed = SPEED_100;
        *pDuplex = DUPLEX_FULL;
        break;
    case DTA_PHY_SPEED_1000_MASTER:
    case DTA_PHY_SPEED_1000_SLAVE:
        *pSpeed = SPEED_1000;
        *pDuplex = DUPLEX_FULL;
        break;
    default:
        *pSpeed = 0;
        *pDuplex = 0;
    }

    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwSetSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaNwSetSettings(DtaNwDeviceData* pDvcData, Int AutoNegSet, Int SpeedSet, 
                                                                            Int DuplexSet)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  Speed;

    if (AutoNegSet == AUTONEG_ENABLE)
    {
        Speed = DTA_PHY_SPEED_AUTO_DETECT;
        pDvcData->m_IalData.m_AutoNegEn = TRUE;
    }
    else if (SpeedSet == SPEED_10)
    {
        if (DuplexSet == DUPLEX_HALF)
            Speed = DTA_PHY_SPEED_10_HALF;
        else
            Speed = DTA_PHY_SPEED_10_FULL;
        pDvcData->m_IalData.m_AutoNegEn = FALSE;
    }
    else if (SpeedSet == SPEED_100)
    {
        if (DuplexSet == DUPLEX_HALF)
            Speed = DTA_PHY_SPEED_100_HALF;
        else
            Speed = DTA_PHY_SPEED_100_FULL;
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
    else
        return -EINVAL;

    Status = DtaNwSetPhySpeed(pDvcData, Speed);

    if (!DT_SUCCESS(Status))
        return -EFAULT;
    return 0;
}

#ifdef GS_SETTINGS_SUPPORT
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtGetSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtaNwEvtGetSettings(struct net_device* pDevice, struct ethtool_cmd* pCmd)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
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
    
    Return = DtaNwGetSettings(pDvcData, &AutoNeg, &Speed, &Duplex, &Advertised,
                                                                              &Supported);
    pCmd->supported = Supported;
    pCmd->advertising = Advertised;
    pCmd->autoneg = AutoNeg;
    pCmd->speed = Speed;
    pCmd->duplex = Duplex;

    DtDbgOut(MAX, IAL, "Exit");
    return Return;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtSetSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtaNwEvtSetSettings(struct net_device* pDevice, struct ethtool_cmd* pCmd)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    return DtaNwSetSettings(pDvcData, pCmd->autoneg, pCmd->speed, pCmd->duplex);
}
#else

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtSetLinkKSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtaNwEvtSetLinkKSettings(struct net_device*pDevice,
                                      const struct ethtool_link_ksettings* pLinkKSettings)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    Int  Speed = pLinkKSettings->base.speed;
    Int  AutoNeg = pLinkKSettings->base.autoneg;
    Int  Duplex = pLinkKSettings->base.duplex;
    return DtaNwSetSettings(pDvcData, AutoNeg, Speed, Duplex);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtGetLinkKSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtaNwEvtGetLinkKSettings(struct net_device*pDevice,
                                      struct ethtool_link_ksettings* pLinkKSettings)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    Int AutoNeg = 0;
    Int Speed = 0;
    Int Duplex = 0;
    Int Supported = 0;
    Int Advertised = 0;
    Int Return;
    DtDbgOut(MAX, IAL, "Start");
    Return = DtaNwGetSettings(pDvcData, &AutoNeg, &Speed, &Duplex, &Advertised,
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


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtGetLink -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt32  DtaNwEvtGetLink(struct net_device* pDevice)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    UInt  PhyLink = DTA_PHY_LINK_DOWN;
    
    DtaNwGetLinkStatus(pDvcData, &PhyLink);

    return (PhyLink == DTA_PHY_LINK_UP ? 1 : 0);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtLinkStatusCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function will be called from the common DtaNw code to trigger a link status
// update
//
void  DtaNwEvtLinkStatusCallback(DtaNwDeviceData* pDvcData)
{
    switch(pDvcData->m_LinkStatus)
    {
    case DTANW_LINKSTAT_UP: netif_carrier_on(pDvcData->m_IalData.m_pNwDevice); break;
    default: netif_carrier_off(pDvcData->m_IalData.m_pNwDevice);
    }
}

#ifdef NO_HW_FEATURE_SUPPORT
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtGetRxCsum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static UInt32  DtaNwEvtGetRxCsum(struct net_device* pDevice)
{
    return ((pDevice->features & NETIF_F_RXCSUM)!=0? 1 : 0);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtSetRxCsum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtaNwEvtSetRxCsum(struct net_device* pDevice, UInt32 Value)
{
    dta_netdev_features  Features = pDevice->features;
    if (Value != 0)
        Features|= NETIF_F_RXCSUM;
    else 
        Features &= ~NETIF_F_RXCSUM;

    Features = DtaNwEvtFixFeatures(pDevice, Features);
    DtaNwEvtSetFeatures(pDevice, Features);
    return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtGetTxCsum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static UInt32  DtaNwEvtGetTxCsum(struct net_device* pDevice)
{
    return ((pDevice->features & (NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM))!=0? 1 : 0);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtSetTxCsum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtaNwEvtSetTxCsum(struct net_device* pDevice, u32 Value)
{
    dta_netdev_features  Features = pDevice->features;
    if (Value != 0)
        Features|= NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM;
    else 
        Features &= ~(NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM);

    Features = DtaNwEvtFixFeatures(pDevice, Features);
    DtaNwEvtSetFeatures(pDevice, Features);
    return 0;
}
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtFixFeatures -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
dta_netdev_features  DtaNwEvtFixFeatures(
    struct net_device*  pDevice,
    dta_netdev_features  Features)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
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

    if (g_DtaNwEnableFCS)
        Features |= NETIF_F_RXFCS;
    return Features;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtSetFeatures -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static int  DtaNwEvtSetFeatures(struct net_device* pDevice, dta_netdev_features Features)
{
    DtaNwDeviceData*  pDvcData = netdev_priv(pDevice);
    
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtNewPacketRxCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function will be called from the common DtaNw code to trigger the arrival of
// a packet
//
void  DtaNwEvtNewPacketRxCallback(
    DtaNwDeviceData*  pDvcData, 
    UInt8*  pPacket, 
    UInt  PacketLength,
    Bool  IPv4,
    Bool  IPv6,
    Bool  IpChecksumFail,
    Bool  UdpChecksumFail,
    Bool  TcpChecksumFail,
    Bool  IpChecksumCorrect,
    Bool  UdpChecksumCorrect,
    Bool  TcpChecksumCorrect)
{
    struct sk_buff*  pSkb;
    
    // Allocate packet buffer struct. Add 2 extra bytes for DWORD payload alignment
    pSkb = dev_alloc_skb(PacketLength + 2);
    if (pSkb == NULL)
    {
        DtDbgOut(ERR, IAL, "Error allocating new packet struct");
        pDvcData->m_NwRcvNoBuffer++;
        pDvcData->m_IalData.m_NetStats.rx_dropped++;
        return;
    }
    pSkb->dev = pDvcData->m_IalData.m_pNwDevice;

    // Align start of headers on DWORD boundary (ethernet header length <> DWORD boundary)
    skb_reserve(pSkb, 2);
    
    // Copy packet into Skb buffer
    DtMemCopy(skb_put(pSkb, PacketLength), (void*)pPacket, PacketLength);

    // Set pSkb->pkt_type, pSkb->mac and protocol fields
    pSkb->protocol = eth_type_trans(pSkb, pDvcData->m_IalData.m_pNwDevice);


    // Task offloading
    pSkb->ip_summed = CHECKSUM_NONE;

    if (pDvcData->m_CheckIpChecksumIPv4Fw ||pDvcData->m_CheckUdpChecksumIPv4Fw ||
               pDvcData->m_CheckUdpChecksumIPv6Fw || pDvcData->m_CheckTcpChecksumIPv4Fw ||
               pDvcData->m_CheckTcpChecksumIPv6Fw)
    {
        // Check if Ip/Udp/Tcp is detected and no errors occured
        if ((IpChecksumCorrect || UdpChecksumCorrect || TcpChecksumCorrect) &&
            !(IpChecksumFail || UdpChecksumFail || TcpChecksumFail))
            pSkb->ip_summed = CHECKSUM_UNNECESSARY;
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
    pDvcData->m_IalData.m_NetStats.rx_bytes+= PacketLength;
    pDvcData->m_IalData.m_NetStats.rx_packets++;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Dta driver communication +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtProbe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is executed by the Dta driver when a child network device must be
// created
//
Int  DtaNwEvtProbe(DtaDeviceItf* pDevItf, Int Id)
{
    Int  Result = 0;
    UInt  DvcIndex;
    struct net_device*  pNwDev;
    DtaNwDeviceData*  pDvcData;

    DtDbgOut(MAX, IAL, "Start");

    // Quick check if maximum device count is reached
    if (g_DtaNwDeviceCount < g_DtaNwMaxDevices)
    {
        // Find free DvcIndex
        for (DvcIndex=0; DvcIndex<g_DtaNwMaxDevices; DvcIndex++)
        {
            if (g_pDtaNwDeviceData[DvcIndex] == NULL)
                break;
        }
        if (DvcIndex == g_DtaNwMaxDevices)
            Result = -ENOMEM;
    } else
        Result = -ENOMEM;

    if (Result >= 0) 
    {
        // Initialise a network device structure
        pNwDev = alloc_etherdev(sizeof(DtaNwDeviceData));
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
        DtMemZero(pDvcData, sizeof(DtaNwDeviceData));
            
        // Store handle to pDvcData in array
        g_pDtaNwDeviceData[DvcIndex] = pDvcData;

        // Initialize the IAL data
        pDvcData->m_IalData.m_DvcIndex = DvcIndex;
        pDvcData->m_IalData.m_pNwDevice = pNwDev;
        pDvcData->m_IalData.m_Id = Id;
        pDvcData->m_IalData.m_pDeviceItf = pDevItf;
        pDvcData->m_IalData.m_AutoNegEn = TRUE;
        DT_ASSERT(Id > 0);
        pDvcData->m_IpPortIndex = Id - 1;
        
        // Initialize the IoCtrl parent
        pDvcData->m_IoCtrlParent.m_pParentFunc = pDevItf->m_pDriverItf->IoCtrl;
        pDvcData->m_IoCtrlParent.m_pContext = pDevItf->m_pDriverItf->m_pContext;
        
        if (!DT_SUCCESS(DtaNwDeviceDataInit(pDvcData)))
            Result = -EFAULT;

        if (!DT_SUCCESS(DtaNwDeviceInit(pDvcData)))
            Result = -EFAULT;

        // We support Frame Check Sequence
#ifndef NO_HW_FEATURE_SUPPORT
        pNwDev->hw_features = NETIF_F_RXFCS;
#endif
        if (g_DtaNwEnableFCS)
        {
             pNwDev->features |= NETIF_F_RXFCS;
             pDvcData->m_FCSEnabled = TRUE;
             DtDbgOut(MIN, IAL, "Frame Check Sequence turned on GLOBAL.");
        }

        // Set hardware checksum support
        SupportHwChkSum = (pDvcData->m_HeaderVersion >= 3);
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
        pNwDev->open = &DtaNwEvtOpen;
        pNwDev->hard_start_xmit = &DtaNwEvtStartTx;
        pNwDev->stop = &DtaNwEvtClose;
        pNwDev->get_stats = &DtaNwEvtGetStats;
        pNwDev->set_multicast_list = &DtaNwEvtSetMulticastList;
        pNwDev->set_mac_address = &DtaNwEvtSetMacAddr;
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
        g_DtaNwDeviceCount++;
        
        DtDbgOut(AVG, IAL, "New device count: %u", g_DtaNwDeviceCount);
    }
    
    DtDbgOut(MAX, IAL, "Exit");
    return Result; 
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwEvtRemove -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is executed by the Dta driver when the child network device must
// be removed from the system
//
void  DtaNwEvtRemove(DtaDeviceItf* pDevItf, Int Id)
{
    UInt  DvcIndex;

    // Search for the device in the device struct
    for (DvcIndex=0; DvcIndex<g_DtaNwMaxDevices; DvcIndex++)
    {
        if (g_pDtaNwDeviceData[DvcIndex] != NULL)
        {
            DtaNwIalData*  pIalData = &g_pDtaNwDeviceData[DvcIndex]->m_IalData;
            if (pIalData->m_pDeviceItf->m_pParentDevice==pDevItf->m_pParentDevice && 
                                                                       pIalData->m_Id==Id)
            {
                DtaNwDeviceExit(g_pDtaNwDeviceData[DvcIndex]);
                unregister_netdev(pIalData->m_pNwDevice);
                free_netdev(pIalData->m_pNwDevice);

                g_pDtaNwDeviceData[DvcIndex] = NULL;
                g_DtaNwDeviceCount--;
                break;
            }
        }
    }
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private helper functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIalSetPacketFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNwIalSetPacketFilter(DtaNwDeviceData* pDvcData, Int Flags)
{
    UInt  PacketFilter = 0;

    if (Flags != 0)
        PacketFilter |= DTA_MAC_FLT_DIRECTED;
    if ((Flags & IFF_PROMISC) != 0)
        PacketFilter |= DTA_MAC_FLT_PROMISCUOUS;
    if ((Flags & IFF_ALLMULTI) != 0)
        PacketFilter |= DTA_MAC_FLT_ALL_MULTICAST;
    if ((Flags & IFF_BROADCAST) != 0)
        PacketFilter |= DTA_MAC_FLT_BROADCAST;
    if ((Flags & IFF_MULTICAST) != 0)
        PacketFilter |= DTA_MAC_FLT_MULTICAST;

    return DtaNwSetPacketFilter(pDvcData, PacketFilter);
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Module init / exit +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwModuleInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtaNwModuleInit(void)
{
    Int  Result = 0;
    DtStatus  Status;
    DtEvtLog  EvtObject;
    
    DtDbgOut(MAX, IAL, "Start");

    // Call init in common driver
    DtEvtLogInit(&EvtObject, &EventGetMessage);
    
    Status = DtaNwDriverInit(&EvtObject);
    if (!DT_SUCCESS(Status))
        Result = -EBUSY;

    if (Result >= 0)
    {
        // Allocate memory for device context pointers
        g_pDtaNwDeviceData = kmalloc(g_DtaNwMaxDevices * sizeof(DtaNwDeviceData*), 
                                                                              GFP_KERNEL);
        if (g_pDtaNwDeviceData != NULL)
            memset(g_pDtaNwDeviceData, 0, g_DtaNwMaxDevices * sizeof(DtaNwDeviceData*));
        else
            Result = -EBUSY;
    }

    if (Result >= 0)
    {
        // We try to register with the Dta driver and wait for the Probe function
        // The probe does the actual Network port creation.
        g_DtaDriverItf.m_pName = DRIVER_NAME;
        g_DtaDriverItf.Probe = DtaNwEvtProbe;
        g_DtaDriverItf.Remove = DtaNwEvtRemove;
        DektecRegisterChildDriver(&g_DtaDriverItf);
    }
    DtDbgOut(MAX, IAL, "Exit");
    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwModuleExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaNwModuleExit(void)
{
    Int  DvcIndex;
    DtDbgOut(MAX, IAL, "Start");

    if (g_DtaNwDeviceCount != 0)
    {
        DtDbgOut(ERR, IAL, "DtaNwModuleExit called, but DtaNwDeviceCount is not 0 but"
                                           " (%u)", g_DtaNwDeviceCount);

        for (DvcIndex=0; DvcIndex<g_DtaNwMaxDevices; DvcIndex++)
        {
            if (g_pDtaNwDeviceData[DvcIndex] != NULL)
            {
                DtaNwEvtRemove(g_pDtaNwDeviceData[DvcIndex]->m_IalData.m_pDeviceItf, 
                                               g_pDtaNwDeviceData[DvcIndex]->m_IalData.m_Id);
                if (g_DtaNwDeviceCount == 0)
                    break;
                
            }
        }
    }
    DektecUnRegisterChildDriver(&g_DtaDriverItf);


    // Call exit in common driver
    DtaNwDriverExit();

    // Free memory for device context pointers
    if (g_pDtaNwDeviceData != NULL)
        kfree(g_pDtaNwDeviceData);
    DtDbgOut(MAX, IAL, "Exit");
}

module_init(DtaNwModuleInit);
module_exit(DtaNwModuleExit);





