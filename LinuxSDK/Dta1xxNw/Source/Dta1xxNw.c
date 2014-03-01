//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dta1Nw.c *#*#*#*#*#*#*#*#*# (C) 2000-2007 DekTec

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	RD	21-08-2007	Created

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <linux/kernel.h>	// printk() stuff
#include <linux/module.h>	// MODULE_PARM
#include <linux/init.h>
#include <linux/version.h>
#include <asm/io.h>

#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/ethtool.h>

#include "../Include/Dta1xxNw.h"
#include "../Include/EthPrtcls.h"



#ifndef CONFIG_PCI
	#error "PCI support is required"
#endif

// Detect kernel version and check if supported
#ifndef KERNEL_VERSION
	#error "Only kernel 2.4 or higher is currently supported"
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
	#error "Only kernel 2.4 or higher is currently supported"
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0) && LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
	#define KERNELVERSION 24
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0) && LINUX_VERSION_CODE < KERNEL_VERSION(2,7,0)
	#define KERNELVERSION 26
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,14) && LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
	#define PERM_ADDR_SUPPORT
#endif

#ifndef HAVE_NETDEV_PRIV
#define HAVE_NETDEV_PRIV
static inline void *netdev_priv(struct net_device *dev)
{
	return dev->priv;
}
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Internal function declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//

// Call back routines
static int Dta1xxNwClose(struct net_device *pDevice);
static struct net_device_stats* Dta1xxNwGetStats(struct net_device *pDevice);
static int Dta1xxNwOpen(struct net_device *pDevice);
static void Dta1xxNwSetMulticastList(struct net_device *pDevice);
static int Dta1xxNwStartXmit(struct sk_buff *pSkb, struct net_device *pDevice);
static int Dta1xxNwSetMacAddr(struct net_device *pDevice, void* pAddr);
//static void Dta1xxNwTxTimeout (struct net_device *pDevice);
#ifdef PERM_ADDR_SUPPORT
static int Dta1xxNwGetPermAddr(struct net_device* pDevice, struct ethtool_perm_addr* pAddr, u8 *pData);
#endif


// ethtool_ops
static void Dta1xxNwGetDriverInfo(struct net_device *pDevice, struct ethtool_drvinfo *pInfo);
static u32 Dta1xxNwGetLink(struct net_device *pDevice);
//static u32 Dta1xxNwGetMessageLevel(struct net_device *pDevice);
static int Dta1xxNwGetSettings(struct net_device *pDevice, struct ethtool_cmd *pCmd);
//static void Dta1xxNwSetMessageLevel(struct net_device *pDevice, u32 Level);
static int Dta1xxNwSetSettings(struct net_device *pDevice, struct ethtool_cmd *pCmd);

// Internal functions
Int Dta1xxNwReset(Dta1xxNw_Private* lp);
Int Dta1xxNwSetNwVersion(Dta1xxNw_Private* lp);
Int Dta1xxNwGetSpeed(Dta1xxNw_Private* lp, UInt* pSpeed);
Int Dta1xxNwSetSpeed(Dta1xxNw_Private* lp, UInt Speed);
Int Dta1xxNwGetPermanentMacAddress(Dta1xxNw_Private* lp, UInt8* Address);
Int Dta1xxNwSetPacketFilter(Dta1xxNw_Private* lp, Int Flags);
Int Dta1xxNwStopWorkerThreads(Dta1xxNw_Private* lp);
Int Dta1xxNwCreateWorkerThreads(Dta1xxNw_Private* lp);
Int Dta1xxNwInitLinkStatus(Dta1xxNw_Private* lp);
Int Dta1xxNwGetCounter(Dta1xxNw_Private* lp, DTA1XX_NWI_INFO* pInfo, UInt32 OID, unsigned long* pCounter, UInt Size);


// helper functions
UInt32 GetChildId(Int TypeNumber, UInt16 Id)
{
	return (TypeNumber<< 16 | Id);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Data declaration and definition -.-.-.-.-.-.-.-.-.-.-.-.-.-.

MODULE_AUTHOR("DEKTEC Digital Video B.V.");
MODULE_DESCRIPTION("Dta1xxNw network driver");

// ADDED TO PREVENT TAINTED MESSAGE. THIS MODULE CAN BE USED FREELY (SEE LICENSE
// STATEMENT ABOVE).
MODULE_LICENSE("GPL");

static Dta1xxNw_Private* g_pNwDevices[MaxPciDevices];
static int g_NumNwDevices;

static const struct ethtool_ops EthToolOps = {
	.get_drvinfo = Dta1xxNwGetDriverInfo,
	.get_settings = Dta1xxNwGetSettings,
	.set_settings = Dta1xxNwSetSettings,
	.get_link = Dta1xxNwGetLink,
#ifdef PERM_ADDR_SUPPORT
	.get_perm_addr = Dta1xxNwGetPermAddr,
#endif

	//.get_msglevel = Dta1NwGetMessageLevel,
	//.set_msglevel = Dta1NwSetMessageLevel,
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- MallocAligned -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void* MallocAligned(Int Align, Int Size)
{	UInt8*  Ptr;
	UInt8*  MPtr;
	Int  Diff;
	Ptr = (UInt8*) kmalloc(Size + Align - 1 + sizeof(void*), GFP_KERNEL);
	if (Ptr == NULL)
		return NULL;
	MPtr = (UInt8*)(((UIntPtr)Ptr + Align - 1) & ~(Align - 1));
	Diff = (Int)(MPtr - Ptr);
	if (Diff < sizeof(void*))
		MPtr += Align;
	((void **)MPtr)[-1] = Ptr;
	return MPtr;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FreeAligned -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  FreeAligned(void* MPtr)
{
	void*  Ptr;
	Ptr = ((void**)MPtr)[-1];
	kfree(Ptr);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwGetDriverInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void Dta1xxNwGetDriverInfo(struct net_device *pDevice, struct ethtool_drvinfo *pInfo)
{
	strcpy(pInfo->driver, DRIVER_NAME);
	strcpy(pInfo->version, DTA1XXNW_STR_VERSION);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwGetSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static int Dta1xxNwGetSettings(struct net_device* pDevice, struct ethtool_cmd* pCmd)
{
	Dta1xxNw_Private* lp = netdev_priv(pDevice);
	UInt Speed;

#if LOG_LEVEL > 0
	DTA1XXNW_LOG(KERN_DEBUG, "Dta1xxNwGetSettings is called");
#endif
	

	memset(pCmd, 0, sizeof(struct ethtool_cmd));
	
	pCmd->supported =	SUPPORTED_10baseT_Half		|
						SUPPORTED_10baseT_Full		|
						SUPPORTED_100baseT_Half		|
						SUPPORTED_100baseT_Full		|
						SUPPORTED_1000baseT_Full	|
						SUPPORTED_Autoneg			|
						SUPPORTED_TP;
	pCmd->advertising = /*ADVERTISED_10baseT_Half		|
						ADVERTISED_10baseT_Full		|
						ADVERTISED_100baseT_Half	|
						ADVERTISED_100baseT_Full	|
						ADVERTISED_1000baseT_Full	|
						ADVERTISED_Autoneg			|*/
						ADVERTISED_TP;
	pCmd->port = PORT_TP;
	pCmd->transceiver = XCVR_INTERNAL;

	// Get Speed settings
	Dta1xxNwGetSpeed(lp, &Speed);

	pCmd->autoneg = AUTONEG_DISABLE;
	switch (Speed) {
		case DTA1XX_NWSPEED_10MB_HALF:
            pCmd->speed = SPEED_10;
			pCmd->duplex = DUPLEX_HALF;
			break;
		case DTA1XX_NWSPEED_10MB_FULL:
			pCmd->speed = SPEED_10;
			pCmd->duplex = DUPLEX_FULL;
			break;
		case DTA1XX_NWSPEED_100MB_HALF:
            pCmd->speed = SPEED_100;
			pCmd->duplex = DUPLEX_HALF;
			break;
		case DTA1XX_NWSPEED_100MB_FULL:
			pCmd->speed = SPEED_100;
			pCmd->duplex = DUPLEX_FULL;
			break;
		case DTA1XX_NWSPEED_1GB_MASTER:
		case DTA1XX_NWSPEED_1GB_SLAVE:
			pCmd->speed = SPEED_1000;
			pCmd->duplex = DUPLEX_FULL;
			break;
		default:
			pCmd->autoneg = AUTONEG_ENABLE;
			break;
	}
	return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwSetSettings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static int Dta1xxNwSetSettings(struct net_device* pDevice, struct ethtool_cmd* pCmd)
{
	Dta1xxNw_Private* lp = netdev_priv(pDevice);
	UInt Speed = 0;

#if LOG_LEVEL > 0
	DTA1XXNW_LOG(KERN_DEBUG, "Dta1xxNwSetSettings is called");
#endif
	
	if (pCmd->autoneg == AUTONEG_ENABLE) {
		// Auto negotiation
		Speed = DTA1XX_NWSPEED_AUTO;
	} else if (pCmd->speed == SPEED_10) {
		if (pCmd->duplex == DUPLEX_HALF)
			 Speed = DTA1XX_NWSPEED_10MB_HALF;
		else Speed = DTA1XX_NWSPEED_10MB_FULL;
	} else if (pCmd->speed == SPEED_100) {
		if (pCmd->duplex == DUPLEX_HALF)
			 Speed = DTA1XX_NWSPEED_100MB_HALF;
		else Speed = DTA1XX_NWSPEED_100MB_FULL;
	} else return STATUS_INVALID_PARAMETER;
	return Dta1xxNwSetSpeed(lp, Speed);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwInitModule -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise the module. Registers the module. Scans the system for the available Dektec
// Network cards and registers the network driver.
//
static int __init Dta1xxNwInitModule(void)
{
	struct net_device*  pNwDev;
	int  Error;
	int  Index;
	int  TypeNumber;
#if LOG_LEVEL > 0
	DTA1XXNW_LOG(KERN_DEBUG, "Initialising module");
#endif

	//-.-.-.-.-.-.-.-.-.-.- Scan system for loaded DTA-1XX network cards -.-.-.-.-.-.-.-.-.-.-

	// Search for Dektec network devices 
	// Supported devices: DTA-160/2160
	
	g_NumNwDevices = 0;

	for (Index = 0; Index < MaxPciDevices; Index++) {

		// Try to open a dektec device
		DTA1XX_IOCTL_DATA iodDta1xx;
		Dta1xxNw_Private* lp;
		int  result;
		
		// We try to access the dta1xx driver
		result = Dta1xxOpenI(Index, GetChildId(0, g_NumNwDevices));
		if (result < 0) {
			continue;
		}

		// Found a new device. Get location info.
		memset(&iodDta1xx, 0, sizeof(DTA1XX_IOCTL_DATA));
		if (Dta1xxIoCtlI(Index, GetChildId(0, g_NumNwDevices), IOCTL_DTA1XX_GET_LOC_INFO, &iodDta1xx) < 0) {
			Dta1xxCloseI(Index, GetChildId(0, g_NumNwDevices));
			continue;
		}
		
		// Found a DEKTEC device!
		// Check if it's a DTA-160 or DTA-2160
		if ((iodDta1xx.m_LocInfo.m_SubVendorId == DTA1XX_VENDORID_DEKTEC) &&
			(iodDta1xx.m_LocInfo.m_SubSystemId == DTA160_DEVICEID)) {
			TypeNumber = 160;
		} else if ((iodDta1xx.m_LocInfo.m_VendorId == DTA1XX_VENDORID_DEKTEC) &&
			(iodDta1xx.m_LocInfo.m_DeviceId == DTA2160_DEVICEID)) {
			TypeNumber = 2160;
		} else {
			Dta1xxCloseI(Index, GetChildId(0, g_NumNwDevices));
			continue;
		}
#if LOG_LEVEL > 0	
		DTA1XXNW_LOG(KERN_INFO, "Found a DTA-%d", TypeNumber);
#endif
		
		// Initialise a network device structure
		pNwDev = alloc_etherdev(sizeof(Dta1xxNw_Private));
		if (pNwDev == NULL) {
			DTA1XXNW_LOG(KERN_ERR, "Error allocating network device structure");
			return -ENOMEM;
		}

#if KERNELVERSION == 24
		SET_MODULE_OWNER(pNwDev);
#endif

		// Initialise device struct for default ethernet values
		ether_setup(pNwDev);

		// Include the Dta1xx tx header
		//pNwDev->hard_header_len += sizeof(Dta1xxDmaTxHeader);
		
		pNwDev->flags |= IFF_MULTICAST;
		
		lp = netdev_priv(pNwDev);

		// Initialise private structure
		lp->m_pNwDevice = pNwDev;
		lp->m_Index = g_NumNwDevices;
		lp->m_Dta1xxIndex = Index;
		lp->m_PortIndex = 0;		// Only 1 ethernet port available...
		lp->m_Connected = 0;
		lp->m_TypeNumber = TypeNumber;
		spin_lock_init(&lp->m_Lock);
        
		g_pNwDevices[g_NumNwDevices++] = lp;
        
		// Set callback routines
		pNwDev->open = &Dta1xxNwOpen;
		pNwDev->hard_start_xmit = &Dta1xxNwStartXmit;
		pNwDev->stop = &Dta1xxNwClose;
		pNwDev->get_stats = &Dta1xxNwGetStats;
		pNwDev->set_multicast_list = &Dta1xxNwSetMulticastList;
		pNwDev->set_mac_address = &Dta1xxNwSetMacAddr;
		
		//pNwDev->tx_timeout = Dta1xxNwTxTimeout;
		//pNwDev->watchdog_timeo = TX_TIMEOUT;
		
		SET_ETHTOOL_OPS(pNwDev, &EthToolOps);

		// Get the physical NICs MAC address
		Dta1xxNwGetPermanentMacAddress(lp, pNwDev->dev_addr);
#ifdef PERM_ADDR_SUPPORT
		memcpy(pNwDev->perm_addr, pNwDev->dev_addr, ETH_LENGTH_OF_ADDRESS);
#endif

        // Reset internal state
		Dta1xxNwReset(lp);
		Dta1xxNwSetNwVersion(lp);
			
		// We don't want to lock the device until the interface is up
		Dta1xxCloseI(Index, GetChildId(0, g_NumNwDevices));
			
		Error = register_netdev(pNwDev);
		if (Error != 0) {
			DTA1XXNW_LOG(KERN_ERR, "Failed to register network driver at bus %d and slot %d",
						iodDta1xx.m_LocInfo.m_BusNumber,
						iodDta1xx.m_LocInfo.m_SlotNumber);
			return Error;
		}
		DTA1XXNW_LOG(KERN_INFO, "Registered network card '%s' for DTA-%d %02x:%02x:%02x:%02x:%02x:%02x",
					pNwDev->name, TypeNumber,
					pNwDev->dev_addr[0], pNwDev->dev_addr[1], pNwDev->dev_addr[2],
					pNwDev->dev_addr[3], pNwDev->dev_addr[4], pNwDev->dev_addr[5]);
	}
	if (g_NumNwDevices == 0) {
		DTA1XXNW_LOG(KERN_WARNING, "No dektec network devices found.");
		return -ENODEV;
	}

	DTA1XXNW_LOG(KERN_INFO, "The network driver (Rev %d.%d.%d.%d) has loaded successfully.",
				DTA1XXNW_VERSION_MAJOR, DTA1XXNW_VERSION_MINOR, DTA1XXNW_VERSION_MICRO,
				DTA1XXNW_VERSION_BUILD);

    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCleanUpModule -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Module clean-up. Unregisters the module. Frees the resources used for the DTA-1xx
// network cards.
//
static void __exit Dta1xxNwCleanUpModule(void)
{
	int Index;
	
#if LOG_LEVEL > 0
	DTA1XXNW_LOG(KERN_DEBUG, "Module clean-up");
#endif

	for (Index = 0; Index < g_NumNwDevices; Index++) {
		if (g_pNwDevices[Index]->m_Connected)
			Dta1xxCloseI(g_pNwDevices[Index]->m_Dta1xxIndex, 
						 GetChildId(g_pNwDevices[Index]->m_TypeNumber, g_pNwDevices[Index]->m_Index));
		g_pNwDevices[Index]->m_Connected = 0;
		
		unregister_netdev(g_pNwDevices[Index]->m_pNwDevice);
		free_netdev(g_pNwDevices[Index]->m_pNwDevice);
	}
}

module_init(Dta1xxNwInitModule);
module_exit(Dta1xxNwCleanUpModule);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Opens a connection to a specific DTA-1xx network card.
//
int Dta1xxNwOpen(struct net_device *pDevice)
{
	Dta1xxNw_Private* lp = (Dta1xxNw_Private*)netdev_priv(pDevice);
	int result;
	
#if LOG_LEVEL > 0
	DTA1XXNW_LOG(KERN_DEBUG, "Dta1xxNwOpen is called");
#endif

	Dta1xxNwCreateWorkerThreads(lp);

	// Interface network card up
	result = Dta1xxOpenI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index));
	if (result < 0) {
		DTA1XXNW_LOG(KERN_ERR, "Failed to reconnect to Dta1xx driver for %s",
					pDevice->name);
		return result;
	}

	lp->m_Connected = 1;

	Dta1xxNwInitLinkStatus(lp);

	Dta1xxNwSetPacketFilter(lp, pDevice->flags);

	netif_start_queue(pDevice);
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Close a connection to a specific DTA-1xx network card
//
int Dta1xxNwClose(struct net_device *pDevice)
{
	Dta1xxNw_Private* lp = (Dta1xxNw_Private*)netdev_priv(pDevice);
#if LOG_LEVEL > 0
 	DTA1XXNW_LOG(KERN_DEBUG, "Dta1xxNwClose is called");
#endif

	Dta1xxNwSetPacketFilter(lp, 0);
	
	Dta1xxNwStopWorkerThreads(lp);

	Dta1xxNwReset(lp);
	
	// Interface network card down
	if (lp->m_Connected)
		Dta1xxCloseI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index));
	lp->m_Connected = 0;
		
    netif_stop_queue(pDevice);
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwStartXmit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// The stack will execute Dta1xxNwStartXmit only ones.
// (NETIF_F_LLTX flag is not set)
//
int Dta1xxNwStartXmit(struct sk_buff* pSkb, struct net_device* pDevice)
{	
	Dta1xxNw_Private* lp = (Dta1xxNw_Private*)netdev_priv(pDevice);
	Dta1xxDmaTxHeader* pTxHeader;
	Int PktLength;
	UInt FreeSpaceUntilEnd;
	UInt FreeSpaceFromBegin;
	UInt ReadOffset;
	UInt WriteOffset;
	Dta1xxNwBuf* pNwTx = &lp->m_NwTxBuf;

#if LOG_LEVEL > 0
	DTA1XXNW_LOG(KERN_DEBUG, "Dta1xxNwStartXmit is called");
#endif
	
	// Check if it's our own dummy generated packet
	if ((pSkb->len >= (sizeof(EthernetIIHeader) + sizeof(IpHeaderV4)-5 + sizeof(UdpHeader) + 10)) &&
	    (pSkb->len <= ETH_ZLEN)){
		EthernetIIHeader* pEthernetIIHeader;
		IpHeaderV4* pIpHeader;
		UdpHeader* pUdpHeader;
		UInt8* pData;
	
		pEthernetIIHeader = (EthernetIIHeader*)pSkb->data;
		pIpHeader = (IpHeaderV4*)(pSkb->data + sizeof(EthernetIIHeader));
		pUdpHeader = (UdpHeader*)(pSkb->data + sizeof(EthernetIIHeader)+ sizeof(IpHeaderV4)-5);
		pData = pSkb->data + sizeof(EthernetIIHeader) + sizeof(IpHeaderV4)-5 + sizeof(UdpHeader);
		if ((pUdpHeader->m_DestinationPort ==  0x0400) && 
			(strcmp(pData, "DektecArp\0") == 0)) {
			// It's our own dummy packet. Skip it.
			dev_kfree_skb(pSkb);
			return STATUS_SUCCESS;
		} 
	}

	if (lp->m_Connected == 0) {
		lp->m_NetStats.tx_errors++;
		return STATUS_UNSUCCESSFUL;
	}
	PktLength = (pSkb->len < ETH_ZLEN ? ETH_ZLEN : pSkb->len);
	// Make sure we allign on multiples of 4
	if ( (PktLength%4)!=0 )
		PktLength += (4-(PktLength%4));

	// Check if we have room in our Tx buffer
	ReadOffset = pNwTx->m_ReadOffset;
	WriteOffset = pNwTx->m_WriteOffset;
	if (WriteOffset < ReadOffset) {
		FreeSpaceFromBegin = 0;
		FreeSpaceUntilEnd = ReadOffset - WriteOffset - 1;
	} else {
		FreeSpaceUntilEnd = pNwTx->m_BufSize - WriteOffset - 1;
		if (ReadOffset == 0)
			FreeSpaceFromBegin = 0;
		else
			FreeSpaceFromBegin = ReadOffset - 1;
	}

	if (FreeSpaceUntilEnd >= PktLength+sizeof(Dta1xxDmaTxHeader)) {
		// Copy transmit header
		pTxHeader = (Dta1xxDmaTxHeader*)(pNwTx->m_pBuffer + WriteOffset);

	} else if (FreeSpaceFromBegin >= PktLength+sizeof(Dta1xxDmaTxHeader)) {
		pNwTx->m_pBuffer[WriteOffset] = '*';
		WriteOffset = 0;
		pTxHeader = (Dta1xxDmaTxHeader*)(pNwTx->m_pBuffer + WriteOffset);
	} else {
		// Not enough room, skip pakket
		lp->m_NetStats.tx_dropped++;
		return STATUS_UNSUCCESSFUL;
	}

	memset(pTxHeader, 0, sizeof(Dta1xxDmaTxHeader));

	pTxHeader->Tag = 0x445441A0;
	pTxHeader->Version = 1;
	pTxHeader->Length = sizeof(Dta1xxDmaTxHeader);
	pTxHeader->TransmitControl.PacketLength = PktLength;
	WriteOffset+= sizeof(Dta1xxDmaTxHeader);
	
	if (pSkb->len < ETH_ZLEN) {
		// Reset stuffing bytes
		memset(pNwTx->m_pBuffer+WriteOffset, 0, ETH_ZLEN);
	}
	memcpy(pNwTx->m_pBuffer+WriteOffset, pSkb->data, pSkb->len);
	WriteOffset+= PktLength;

	if (WriteOffset == pNwTx->m_BufSize) {
		// Can not occur due to size check above. 
		//If occurs then WriteOffset==ReadOffset can not be prevented!!
		WriteOffset = 0;
	}

	pNwTx->m_WriteOffset = WriteOffset;
	pDevice->trans_start = jiffies;

	// Acknowledge new packet arrived
	set_bit(0, (void*)&pNwTx->m_PktAvail);
	wake_up_interruptible(&pNwTx->m_WaitQueue);

	dev_kfree_skb(pSkb);
	return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxWorkerThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int  Dta1xxNwIpRxWorkerThread(
    IN void* pData)
{	Dta1xxNw_Private* lp = (Dta1xxNw_Private*)pData;
	DTA1XX_RW_INFO RwInfo;
	
	Int ret;
	BOOLEAN StopThread = FALSE;
	UInt8* pBuf;
	UInt PktLength;
	UInt ReadOffset;
	Dta1xxDmaRxHeader* pRxHeader = NULL;
	Dta1xxNwBuf* pNwRx = &lp->m_NwRxBuf;
	struct sk_buff* pSkb;

	memset(&RwInfo, 0, sizeof(DTA1XX_RW_INFO)); 

	// Give packet buffer to Dta1xx driver
	RwInfo.m_ChannelType = DTA1XX_NW_RX_CHANNEL;
	RwInfo.m_PortIndex = lp->m_PortIndex;
	RwInfo.m_pBuffer = (UInt8*)pNwRx;
	RwInfo.m_NumBytes2Transfer = sizeof(Dta1xxNwBuf);

	if (Dta1xxIoCtlI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index), IOCTL_DTA1XX_DMA_USERMEM_READ, (DTA1XX_IOCTL_DATA*)&RwInfo) < 0) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwIpRxWorkerThread: Error receiving packet");
	}
				
	while (!StopThread) {

		// Wait for new Rx packets or link status update
		ret = wait_event_interruptible(pNwRx->m_WaitQueue,	
							(test_bit(0, (void*)&pNwRx->m_PktAvail)) ||
							(test_bit(0, (void*)&lp->m_Status.m_LinkStatus.m_Avail)) ||
							Dta1xxKThreadShouldStop());

		if (Dta1xxKThreadShouldStop() || (ret!=0) /*-ERESTARTSYS*/ ){
			StopThread = TRUE;
		} else {
			clear_bit(0, (void*)&pNwRx->m_PktAvail);
			
			if (lp->m_Status.m_LinkStatus.m_Avail != 0) {
				// We have a link status update
				if (lp->m_Status.m_GeneralStatus == NW_STATUS_MEDIA_CONNECT) {
					// Connected
#if LOG_LEVEL > 0
					DTA1XXNW_LOG(KERN_INFO, "Link UP");
#endif
					netif_carrier_on(lp->m_pNwDevice);
				} else {
					// Disconnected
#if LOG_LEVEL > 0
					DTA1XXNW_LOG(KERN_INFO, "Link DOWN");
#endif
					netif_carrier_off(lp->m_pNwDevice);
				}

			}
			clear_bit(0, (void*)&lp->m_Status.m_LinkStatus.m_Avail);
			

			// Check for read/write pointers and get the data
			while (pNwRx->m_ReadOffset != pNwRx->m_WriteOffset) {
				clear_bit(0, (void*)&pNwRx->m_PktAvail);
				pBuf = pNwRx->m_pBuffer+pNwRx->m_ReadOffset;
				if (pBuf[0] == '*') {
					// New packet is at start of buffer
					pBuf = pNwRx->m_pBuffer;
					pNwRx->m_ReadOffset = 0;
				}
					
				pRxHeader = (Dta1xxDmaRxHeader*)pBuf;
				PktLength = pRxHeader->ReceiveStatus.FrameLength - 4; //-4 CRC checksum

				pSkb = dev_alloc_skb(PktLength + 2);
				if( pSkb == NULL) {
					lp->m_NetStats.rx_dropped++;	
				} else {
					pSkb->dev = lp->m_pNwDevice;
	
					// Allign start of headers on DWORD boundary (ethernet header length <> DWORD boundary)
					skb_reserve(pSkb, 2);	
					// Copy packet into Skb buffer without DMA header
					memcpy(skb_put(pSkb, PktLength), (void*)(pBuf + sizeof(Dta1xxDmaRxHeader)), PktLength);
					
					// Set pSkb->pkt_type, pSkb->mac and protocol fields
					pSkb->protocol = eth_type_trans(pSkb, lp->m_pNwDevice);
					 
 					// Give it to the network layer
					netif_rx(pSkb);
 
					lp->m_pNwDevice->last_rx = jiffies;
					lp->m_NetStats.rx_bytes+= PktLength;
					lp->m_NetStats.rx_packets++;
				}
				
				// Make sure we allign on multiples of 4
				if ( (PktLength%4)!=0 )
					PktLength += (4-(PktLength%4));
				
				// Update read pointer
				ReadOffset = pNwRx->m_ReadOffset + PktLength + 4 + sizeof(Dta1xxDmaRxHeader);
				if (ReadOffset == pNwRx->m_BufSize)
					ReadOffset = 0;

				pNwRx->m_ReadOffset = ReadOffset;
			}
		}
	}
	return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwSetMulticastList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxNwSetMulticastList(struct net_device *pDevice)
{	Dta1xxNw_Private* lp = (Dta1xxNw_Private*)netdev_priv(pDevice);
	DTA1XX_NWI_INFO* pInfo;
	UInt Size;
	Int i, j;
	struct dev_mc_list* pMCList;
	UInt8* pMC;
	Int Status;
	
#if LOG_LEVEL > 0
	DTA1XXNW_LOG(KERN_DEBUG, "Dta1xxNwSetMulticastList is called");
#endif
	
	if (lp->m_Connected == 0)
		return;

	Size = sizeof(DTA1XX_NWI_INFO) + (pDevice->mc_count * ETH_LENGTH_OF_ADDRESS) - sizeof(ULONG);

	//Allocate memory to store the hardware addresses
	pInfo = kmalloc(Size, GFP_ATOMIC);

	if (pInfo == NULL) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwSetMulticastList: Error allocating buffer");
		return;
	}

	pInfo->m_IpPortIndex = lp->m_PortIndex;
	pInfo->m_Oid = OID_802_3_MULTICAST_LIST;
	pInfo->m_BytesWritten = (pDevice->mc_count * ETH_LENGTH_OF_ADDRESS);
	
	// Copy MAC addresses
	pMC = (UInt8*)&pInfo->m_Data[0];
	pMCList = pDevice->mc_list;
	for (i = 0; i < pDevice->mc_count; i++) {
		for (j = 0; j < 6; j++)
			*pMC++ = pMCList->dmi_addr[j];
		pMCList = pMCList->next;
	}

	Status = Dta1xxIoCtlI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index), IOCTL_DTA1XX_NWI_SET_INFO, (DTA1XX_IOCTL_DATA*)pInfo);
	if (Status < 0) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwSetMulticastList: Error setting multicast list");
	}
	kfree(pInfo);

	Dta1xxNwSetPacketFilter(lp, pDevice->flags | 
								(Status == NDIS_STATUS_MULTICAST_FULL ? IFF_ALLMULTI : 0));
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwGetLink -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
u32 Dta1xxNwGetLink(struct net_device *pDevice)
{	Dta1xxNw_Private* lp = (Dta1xxNw_Private*)netdev_priv(pDevice);
	UInt Speed;
	
#if LOG_LEVEL > 0
	DTA1XXNW_LOG(KERN_DEBUG, "Dta1xxNwGetLink is called");
#endif
	
	if (lp->m_Connected == 0)
		return 0;

	Dta1xxNwGetSpeed(lp, &Speed);
	
	return (Speed == DTA1XX_NWSPEED_NOLINK ? 0 : 1);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwGetSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int Dta1xxNwGetSpeed(Dta1xxNw_Private* lp, UInt* pSpeed)
{
	DTA1XX_NWSPEED NwSpeed;

	memset(&NwSpeed, 0, sizeof(DTA1XX_NWSPEED));
	*pSpeed = DTA1XX_NWSPEED_NOLINK;
	
	if (lp->m_Connected == 0)
		return STATUS_UNSUCCESSFUL;

	NwSpeed.m_IpPortIndex = lp->m_PortIndex;
	NwSpeed.m_Speed = DTA1XX_NWSPEED_NOLINK;
	
	if (Dta1xxIoCtlI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index), IOCTL_DTA1XX_GET_NWSPEED, (DTA1XX_IOCTL_DATA*)&NwSpeed) < 0) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwGetSpeed: Error getting current speed setting");
		return STATUS_UNSUCCESSFUL;
	}
	*pSpeed = NwSpeed.m_Speed;
	return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwSetSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int Dta1xxNwSetSpeed(Dta1xxNw_Private* lp, UInt Speed)
{	DTA1XX_NWI_SPEED NwiSpeed;
	
	if (lp->m_Connected == 0)
		return STATUS_UNSUCCESSFUL;

	memset(&NwiSpeed, 0, sizeof(DTA1XX_NWI_SPEED));
	
	NwiSpeed.m_Speed = Speed;
	NwiSpeed.m_IpPortIndex = lp->m_PortIndex;
	
	if (Dta1xxIoCtlI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index), IOCTL_DTA1XX_NWI_SET_SPEED, (DTA1XX_IOCTL_DATA*)&NwiSpeed) < 0) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwSetSpeed: Error setting speed/duplex");
		return STATUS_UNSUCCESSFUL;
	}
	return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwGetStats -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static struct net_device_stats* Dta1xxNwGetStats(struct net_device *pDevice)
{
	Dta1xxNw_Private* lp = (Dta1xxNw_Private*)netdev_priv(pDevice);
	DTA1XX_NWI_INFO* pInfo;
	Int Size;

#if LOG_LEVEL > 0
	DTA1XXNW_LOG(KERN_DEBUG, "Dta1xxNwGetStats is called");
#endif
	
	if (lp->m_Connected != 0) {
		// Retrieve counters from Dta1xx driver

		// Allocate buffer
		Size = sizeof(DTA1XX_NWI_INFO) - sizeof(ULONG) + sizeof(ULONG64);

		//Allocate memory to store the hardware addresses
		pInfo = kmalloc(Size, GFP_ATOMIC);

		if (pInfo == NULL) {
			DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwGetStats: Error allocating buffer");
		} else {
			unsigned long OneCollision;
			unsigned long MoreCollisions;

			memset(pInfo, 0, Size);
			Dta1xxNwGetCounter(lp, pInfo, OID_GEN_XMIT_OK, &lp->m_NetStats.tx_packets, 4);
			Dta1xxNwGetCounter(lp, pInfo, OID_GEN_RCV_OK, &lp->m_NetStats.rx_packets, 4);
			Dta1xxNwGetCounter(lp, pInfo, OID_GEN_XMIT_ERROR, &lp->m_NetStats.tx_errors, 4);
			Dta1xxNwGetCounter(lp, pInfo, OID_GEN_RCV_ERROR, &lp->m_NetStats.rx_errors, 4);
			Dta1xxNwGetCounter(lp, pInfo, OID_GEN_RCV_NO_BUFFER, &lp->m_NetStats.rx_dropped, 4);
			Dta1xxNwGetCounter(lp, pInfo, OID_GEN_RCV_CRC_ERROR, &lp->m_NetStats.rx_crc_errors, 4);
			Dta1xxNwGetCounter(lp, pInfo, OID_802_3_XMIT_TIMES_CRS_LOST, &lp->m_NetStats.tx_carrier_errors, 4);
			Dta1xxNwGetCounter(lp, pInfo, OID_802_3_XMIT_LATE_COLLISIONS, &lp->m_NetStats.tx_aborted_errors, 4);
			Dta1xxNwGetCounter(lp, pInfo, OID_802_3_XMIT_ONE_COLLISION, &OneCollision, 4);
			Dta1xxNwGetCounter(lp, pInfo, OID_802_3_XMIT_MORE_COLLISIONS, &MoreCollisions, 4);
			Dta1xxNwGetCounter(lp, pInfo, OID_GEN_BYTES_RCV, &lp->m_NetStats.rx_bytes, 4);
			Dta1xxNwGetCounter(lp, pInfo, OID_GEN_BYTES_XMIT, &lp->m_NetStats.tx_bytes, 4);

			lp->m_NetStats.collisions = OneCollision + MoreCollisions;
			kfree(pInfo);
		}
		

	/*	Left over:
			(unsigned long  tx_dropped) 
			unsigned long  multicast 
			unsigned long  tx_window_errors 
			unsigned long  rx_length_errors 
			unsigned long  rx_over_errors 
			unsigned long  rx_frame_errors 
			unsigned long  rx_fifo_errors 
			unsigned long  rx_missed_errors 
			unsigned long  tx_fifo_errors 
			unsigned long  rx_compressed 
			unsigned long  tx_compressed 

	*/
	}
	return &lp->m_NetStats;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int Dta1xxNwReset(Dta1xxNw_Private* lp)
{	DTA1XX_NWI_RESET  NwiReset;

	if (lp->m_Connected == 0)
		return STATUS_SUCCESS;
	
	NwiReset.m_IpPortIndex = lp->m_PortIndex;

	if (Dta1xxIoCtlI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index), IOCTL_DTA1XX_NWI_RESET, (DTA1XX_IOCTL_DATA*)&NwiReset) < 0) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwReset: Error resetting network status");
		return STATUS_UNSUCCESSFUL;
	}
	
	return STATUS_SUCCESS;
}	

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwSetNwVersion -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int Dta1xxNwSetNwVersion(Dta1xxNw_Private* lp)
{	
	DTA1XX_DRIVER_VERSION  DriverVersion;
	
	if (lp->m_Connected == 0)
		return STATUS_UNSUCCESSFUL;

	DriverVersion.m_VersionMajor = DTA1XXNW_VERSION_MAJOR;
	DriverVersion.m_VersionMinor = DTA1XXNW_VERSION_MINOR;
	DriverVersion.m_VersionMicro = DTA1XXNW_VERSION_MICRO;
	DriverVersion.m_VersionBuild = DTA1XXNW_VERSION_BUILD;
	
	if (Dta1xxIoCtlI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index), IOCTL_DTA1XX_NWI_SET_VERSION, (DTA1XX_IOCTL_DATA*)&DriverVersion) < 0) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwSetNwVersion: Error setting version");
		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwGetPermanentMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int Dta1xxNwGetPermanentMacAddress(Dta1xxNw_Private* lp, UInt8* pAddress)
{	DTA1XX_NWI_INFO* pInfo;
	UInt Size;
	
	Size = sizeof(DTA1XX_NWI_INFO) + ETH_LENGTH_OF_ADDRESS - sizeof(ULONG);

	//Allocate memory to store the hardware addresses
	pInfo = kmalloc(Size, GFP_ATOMIC);

	if (pInfo == NULL) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwGetPermanentMacAddress: Error allocating buffer");
		return STATUS_UNSUCCESSFUL;
	}

	pInfo->m_IpPortIndex = lp->m_PortIndex;
	pInfo->m_Oid = OID_802_3_PERMANENT_ADDRESS;
	pInfo->m_BytesWritten = ETH_LENGTH_OF_ADDRESS;
	
	if (Dta1xxIoCtlI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index), IOCTL_DTA1XX_NWI_QUERY_INFO, (DTA1XX_IOCTL_DATA*)pInfo) < 0) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwGetPermanentMacAddress: Error getting permanent MAC address");
		kfree(pInfo);
		return STATUS_UNSUCCESSFUL;
	}
	memcpy(pAddress, &(pInfo->m_Data[0]), ETH_LENGTH_OF_ADDRESS);
	kfree(pInfo);
	return STATUS_SUCCESS;	
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwSetMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int Dta1xxNwSetMacAddress(Dta1xxNw_Private* lp, UInt8* pAddress)
{	DTA1XX_NWI_INFO* pInfo;
	UInt Size;
	
	Size = sizeof(DTA1XX_NWI_INFO) + ETH_LENGTH_OF_ADDRESS - sizeof(ULONG);

	//Allocate memory to store the hardware addresses
	pInfo = kmalloc(Size, GFP_ATOMIC);

	if (pInfo == NULL) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwSetMacAddress: Error allocating buffer");
		return STATUS_UNSUCCESSFUL;
	}

	pInfo->m_IpPortIndex = lp->m_PortIndex;
	pInfo->m_Oid = OID_802_3_CURRENT_ADDRESS;
	pInfo->m_BytesWritten = ETH_LENGTH_OF_ADDRESS;
	memcpy(&(pInfo->m_Data[0]), pAddress, ETH_LENGTH_OF_ADDRESS);
	
	if (Dta1xxIoCtlI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index), IOCTL_DTA1XX_NWI_SET_INFO, (DTA1XX_IOCTL_DATA*)pInfo) < 0) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwSetMacAddress: Error getting permanent MAC address");
		kfree(pInfo);
		return STATUS_UNSUCCESSFUL;
	}
	
	kfree(pInfo);
	return STATUS_SUCCESS;	
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwGetPermAddr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#ifdef PERM_ADDR_SUPPORT
int Dta1xxNwGetPermAddr(struct net_device* pDevice, struct ethtool_perm_addr* pAddr, u8 *pData)
{	Dta1xxNw_Private* lp = netdev_priv(pDevice);

#if LOG_LEVEL > 0
	DTA1XXNW_LOG(KERN_DEBUG, "Dta1xxNwGetPermAddr is called");
#endif
		
	if (pAddr->size < ETH_LENGTH_OF_ADDRESS)
		return -ETOOSMALL;

	if (lp->m_Connected == 0)
		return STATUS_UNSUCCESSFUL;
	
	pAddr->size = ETH_LENGTH_OF_ADDRESS;

	return Dta1xxNwGetPermanentMacAddress(lp, pData);
}
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwSetMacAddr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static int Dta1xxNwSetMacAddr(struct net_device *pDevice, void* pAddr)
{	Dta1xxNw_Private* lp = (Dta1xxNw_Private*)netdev_priv(pDevice);
	Int Status, i;
	UInt8* pMacAddress = (UInt8*)pAddr;

#if LOG_LEVEL > 0
	DTA1XXNW_LOG(KERN_DEBUG, "Dta1xxNwSetMacAddr is called");
#endif 

	Status = Dta1xxNwSetMacAddress(lp, pMacAddress);
	if (Status == STATUS_SUCCESS) {
		// Update address in device structure
		for (i = 0; i < ETH_LENGTH_OF_ADDRESS; i++) {
			pDevice->dev_addr[i] = pMacAddress[i];
		}
	}
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwSetPacketFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int Dta1xxNwSetPacketFilter(Dta1xxNw_Private* lp, Int Flags)
{	ULONG PacketFilter = 0;
	DTA1XX_NWI_INFO Info;

	if (Flags != 0)
		PacketFilter |= NW_PACKET_TYPE_DIRECTED;
	if (Flags & IFF_PROMISC) 
		PacketFilter |= NW_PACKET_TYPE_PROMISCUOUS;
	if (Flags & IFF_ALLMULTI)
		PacketFilter |= NW_PACKET_TYPE_ALL_MULTICAST;
	if (Flags & IFF_BROADCAST)
		PacketFilter |= NW_PACKET_TYPE_BROADCAST;
	if (Flags & IFF_MULTICAST)
		PacketFilter |= NW_PACKET_TYPE_MULTICAST;

	Info.m_IpPortIndex = lp->m_PortIndex;
	Info.m_Oid = OID_GEN_CURRENT_PACKET_FILTER;
	Info.m_BytesWritten = sizeof(ULONG);
	memcpy(&(Info.m_Data[0]), &PacketFilter, sizeof(ULONG));
	if (Dta1xxIoCtlI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index), IOCTL_DTA1XX_NWI_SET_INFO, (DTA1XX_IOCTL_DATA*)&Info) < 0) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xNwSetPacketFilter: Error setting packet filter");
		return STATUS_UNSUCCESSFUL;
	}
	return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxWorkerThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int  Dta1xxNwIpTxWorkerThread(
    IN void* pData)
{	Dta1xxNw_Private* lp = (Dta1xxNw_Private*)pData;
	DTA1XX_RW_INFO RwInfo;
	
	Int ret;
	BOOLEAN StopThread = FALSE;
	UInt8* pBuf;
	UInt PktLength;
	UInt ReadOffset;
	Dta1xxDmaTxHeader* pTxHeader = NULL;
	Dta1xxNwBuf* pNwTx = &lp->m_NwTxBuf;

	memset(&RwInfo, 0, sizeof(DTA1XX_RW_INFO)); 

	while (!StopThread) {

		// Wait for new Tx packets
		ret = wait_event_interruptible(pNwTx->m_WaitQueue,	(test_bit(0, (void*)&pNwTx->m_PktAvail)) ||
									Dta1xxKThreadShouldStop());

		if (Dta1xxKThreadShouldStop() || (ret!=0) /*-ERESTARTSYS*/ ){
			StopThread = TRUE;
		} else {
			clear_bit(0, (void*)&pNwTx->m_PktAvail);

			// Check for read/write pointers and transmit the data
			while (pNwTx->m_ReadOffset != pNwTx->m_WriteOffset) {
				clear_bit(0, (void*)&pNwTx->m_PktAvail);
				pBuf = pNwTx->m_pBuffer+pNwTx->m_ReadOffset;
				if (pBuf[0] == '*') {
					// New packet is at start of buffer
					pBuf = pNwTx->m_pBuffer;
					pNwTx->m_ReadOffset = 0;
				}
					
				pTxHeader = (Dta1xxDmaTxHeader*)pBuf;
				PktLength = pTxHeader->TransmitControl.PacketLength + sizeof(Dta1xxDmaTxHeader);
				
				// Send packet to Dta1xx driver
				RwInfo.m_ChannelType = DTA1XX_NW_TX_CHANNEL;
				RwInfo.m_PortIndex = lp->m_PortIndex;
				RwInfo.m_pBuffer = (UInt8*)pTxHeader;
				RwInfo.m_NumBytes2Transfer = PktLength;

				if (Dta1xxIoCtlI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index), IOCTL_DTA1XX_DMA_USERMEM_WRITE, (DTA1XX_IOCTL_DATA*)&RwInfo) < 0) {
					DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwIpTxWorkerThread: Error transmitting packet");
					lp->m_NetStats.tx_dropped++;
				} else {
					lp->m_NetStats.tx_bytes+= PktLength;
					lp->m_NetStats.tx_packets++;
				}

				// Make sure we allign on multiples of 4
				if ( (PktLength%4)!=0 )
					PktLength += (4-(PktLength%4));

				ReadOffset = pNwTx->m_ReadOffset + PktLength;
				if (ReadOffset == pNwTx->m_BufSize)
					ReadOffset = 0;

				pNwTx->m_ReadOffset = ReadOffset;
			}
		}
	}
	return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwCreateWorkerThreads -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int Dta1xxNwCreateWorkerThreads(Dta1xxNw_Private* lp)
{
	Dta1xxNwBuf*	pNwTx = &lp->m_NwTxBuf;
	Dta1xxNwBuf*	pNwRx = &lp->m_NwRxBuf;
	
	// initialise events
	init_waitqueue_head(&pNwTx->m_WaitQueue);
	init_waitqueue_head(&pNwRx->m_WaitQueue);

	// Initialise buffers
	pNwTx->m_ReadOffset = 0;
	pNwTx->m_WriteOffset = 0;

	pNwRx->m_ReadOffset = 0;
	pNwRx->m_WriteOffset = 0;

	clear_bit(0, (void*)&pNwRx->m_PktAvail);
	clear_bit(0, (void*)&pNwTx->m_PktAvail);

	pNwTx->m_pBuffer = NULL;
	pNwRx->m_pBuffer = NULL;

	lp->m_pRxThread = NULL;
	lp->m_pTxThread = NULL;

	// Create buffers
	pNwTx->m_BufSize = DTA1XXNW_TXBUFSIZE;
	pNwTx->m_pBuffer = MallocAligned(4, DTA1XXNW_TXBUFSIZE);
	if (pNwTx->m_pBuffer == NULL) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwCreateWorkerThreads: Error creating NwTx buffer");
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	pNwRx->m_BufSize = DTA1XXNW_RXBUFSIZE;
	pNwRx->m_pBuffer = MallocAligned(4, DTA1XXNW_RXBUFSIZE);
	if (pNwRx->m_pBuffer == NULL) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwCreateWorkerThreads: Error creating NwRx buffer");
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	// Create Rx thread
	lp->m_pRxThread = Dta1xxKThreadRun(Dta1xxNwIpRxWorkerThread, lp, "DTA1XXNWRX");
	if (IS_ERR(lp->m_pRxThread)) {
		lp->m_pRxThread = NULL;
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwCreateWorkerThreads: Error creating NwRx thread");
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	
	// Create Tx thread
	lp->m_pTxThread = Dta1xxKThreadRun(Dta1xxNwIpTxWorkerThread, lp, "DTA1XXNWTX");
	if (IS_ERR(lp->m_pTxThread)) {
		lp->m_pTxThread = NULL;
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwCreateWorkerThreads: Error creating NwTx thread");
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwStopWorkerThreads -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int Dta1xxNwStopWorkerThreads(Dta1xxNw_Private* lp)
{

	// Stop Rx thread
	if (lp->m_pRxThread != NULL) 
		Dta1xxKThreadStop(lp->m_pRxThread);

	// Stop Tx thread
	if (lp->m_pTxThread != NULL) 
		Dta1xxKThreadStop(lp->m_pTxThread);

	// Release buffers
	if (lp->m_NwTxBuf.m_pBuffer) 
		FreeAligned(lp->m_NwTxBuf.m_pBuffer);
	lp->m_NwTxBuf.m_pBuffer = NULL;

	if (lp->m_NwRxBuf.m_pBuffer) 
		FreeAligned(lp->m_NwRxBuf.m_pBuffer);
	lp->m_NwRxBuf.m_pBuffer = NULL;

	return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwInitLinkStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int Dta1xxNwInitLinkStatus(Dta1xxNw_Private* lp)
{
	memset(&lp->m_Status, 0, sizeof(DTA1XX_NWI_STATUS));
	
	if (lp->m_Connected == 0)
		return STATUS_UNSUCCESSFUL;

	lp->m_Status.m_IpPortIndex = lp->m_PortIndex;

	// Set the status event to the Rx Queue event
	lp->m_Status.m_LinkStatus.m_StatusAvail = &lp->m_NwRxBuf.m_WaitQueue;
	// m_StatusBufferLength not used

	if (Dta1xxIoCtlI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index), IOCTL_DTA1XX_NWI_GET_STATUS, (DTA1XX_IOCTL_DATA*)&lp->m_Status) < 0) {
		DTA1XXNW_LOG(KERN_INFO, "Dta1xxNwInitLinkStatus: Error getting status");
		return STATUS_UNSUCCESSFUL;
	}
	return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwGetCounter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int Dta1xxNwGetCounter(Dta1xxNw_Private* lp, DTA1XX_NWI_INFO* pInfo, UInt32 OID, unsigned long* pCounter, UInt Size)
{	ULONG64* pCnt;
	if (lp->m_Connected == 0)
		return STATUS_UNSUCCESSFUL;

	pCnt = (ULONG64*)&pInfo->m_Data[0];

	pInfo->m_IpPortIndex = lp->m_PortIndex;
	pInfo->m_BytesWritten = Size;
	pInfo->m_Oid = OID;
			
	if (Dta1xxIoCtlI(lp->m_Dta1xxIndex, GetChildId(lp->m_TypeNumber, lp->m_Index), IOCTL_DTA1XX_NWI_QUERY_INFO, (DTA1XX_IOCTL_DATA*)pInfo) < 0) {
		return STATUS_UNSUCCESSFUL;
	}

	*pCounter = (unsigned long) *pCnt;
	return STATUS_SUCCESS;
}
