//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dta1xx.c *#*#*#*#*#*#*#*#*# (C) 2000-2007 DekTec

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dta1xx driver, please refer to the Dta1xx readme

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <linux/kernel.h>	// printk() stuff
#include <linux/module.h>	// MODULE_PARM
#include <linux/init.h>
#include <linux/version.h>

#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"
#include "../Include/DriverVersion.h"

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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// NOTE: pci_find_device has been deprecated and is replaced by pci_get_device
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22)
	#define DTA1XX_PCI_FIND_DEVICE pci_find_device
#else
	#define DTA1XX_PCI_FIND_DEVICE pci_get_device
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Internal function declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Data declaration and definition -.-.-.-.-.-.-.-.-.-.-.-.-.-.

MODULE_AUTHOR("DekTec Digital Video B.V.");
MODULE_DESCRIPTION("Dta1xx Series Driver");

// ADDED TO PREVENT TAINTED MESSAGE. THIS MODULE CAN BE USED FREELY (SEE LICENSE
// STATEMENT ABOVE).
MODULE_LICENSE("GPL");

Int g_Dta1xxMajor	= DTA1XX_MAJOR;		// Driver major number

// The file operations
struct file_operations g_Dta1xxFOps =
{
	open: 			Dta1xxOpen,
	release:		Dta1xxRelease,
	ioctl:			Dta1xxIoCtl,
#ifdef CONFIG_COMPAT
	compat_ioctl:	Dta1xxCompatIoCtl,
#endif
	mmap:			Dta1xxMmap
};

DTA1XX_FDO  g_Dta1xxCards[DTA1XX_MAX_CARDS];	// DTA-1xx card data
Int  g_NumOfCards = 0;					// Number of DTA-1xx cards found in the system
Globals  g_Globals;			// Global device-driver data packaged in a single struct

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitModule -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise the module. Registers the module. Scans the system for the available DTA-1xx
// cards and registers the driver and allocates all needed resources.
//
int Dta1xxInitModule()
{
	Int r=0;
	struct pci_dev* pPciCard = NULL;

#ifdef DEBUG
	DTA1XX_LOG( KERN_INFO, "Dta1xxInitModule" );
#endif

#if KERNELVERSION < 26
	// NO SYMBOLS ARE EXPORTED (kernel 2.4 only)
	EXPORT_NO_SYMBOLS;

	//-.-.-.-.-.-.-.-. Check for availabilty of PCI bus (kernel 2.4 only) .-.-.-.-.-.-.-.-

	if ( !pci_present() )
	{
		DTA1XX_LOG( KERN_CRIT, "Dta1xxInitModule: no PCI-bus available" );
		return -EFAULT;
	}
#endif

	//.-.-.-.-.-.-.-.-.-.-.- Init Functional Devices Objects Array -.-.-.-.-.-.-.-.-.-.-.-

	memset( g_Dta1xxCards, 0, ( sizeof(DTA1XX_FDO) * DTA1XX_MAX_CARDS ) );
	g_NumOfCards = 0;

	//-.-.-.-.-.-.-.-.-.-.- Scan system for DTA-1xx series PCI cards -.-.-.-.-.-.-.-.-.-.-

	// Scan for devices with 9054
	pPciCard = DTA1XX_PCI_FIND_DEVICE(PLX_VENDORID, PLX9054_DEVICEID, NULL);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		// Check that this is a DekTec card
		if (    ( pPciCard->subsystem_vendor==DTA1XX_VENDORID_OLD )
			 && (    pPciCard->subsystem_device==DTA100_DEVICEID
			      || pPciCard->subsystem_device==DTA102_DEVICEID
				  || pPciCard->subsystem_device==DTA120_DEVICEID
				  || pPciCard->subsystem_device==DTA122_DEVICEID ) )
		{
			if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
			{
				DTA1XX_LOG( KERN_INFO,
					"The DTA-%d (Firmware Version %d) has started successfully.",
					g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
					g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
			}
			g_NumOfCards++;
		}
		// Get next card
		pPciCard = DTA1XX_PCI_FIND_DEVICE(PLX_VENDORID, PLX9054_DEVICEID, pPciCard);
	}

	// Scan for devices with 9056
	pPciCard = NULL;
	pPciCard = DTA1XX_PCI_FIND_DEVICE(PLX_VENDORID, PLX9056_DEVICEID, NULL);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		// Check that this is a DekTec card
		if ( (    ( pPciCard->subsystem_vendor==DTA1XX_VENDORID_OLD )
			   && (    pPciCard->subsystem_device==DTA107_DEVICEID
			        || pPciCard->subsystem_device==DTA110_DEVICEID
			        || pPciCard->subsystem_device==DTA124_DEVICEID
			        || pPciCard->subsystem_device==DTA140_DEVICEID) )
			// Or DekTec card with new vendor ID
			|| (  ( pPciCard->subsystem_vendor==DTA1XX_VENDORID )
		 	   && (    pPciCard->subsystem_device==DTA111_DEVICEID
					|| pPciCard->subsystem_device==DTA112_DEVICEID
					|| pPciCard->subsystem_device==DTA115_DEVICEID
					|| pPciCard->subsystem_device==DTA116_DEVICEID
					|| pPciCard->subsystem_device==DTA117_DEVICEID
					|| pPciCard->subsystem_device==DTA160_DEVICEID)))
		{
			if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
			{
				DTA1XX_LOG( KERN_INFO,
					"The DTA-%d (Firmware Version %d) has started successfully.",
					g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
					g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
			}
			g_NumOfCards++;
		}
		// Get next card
		pPciCard = DTA1XX_PCI_FIND_DEVICE(PLX_VENDORID, PLX9056_DEVICEID, pPciCard);
	}

	// Scan for DTA-105s
	pPciCard = NULL;
	pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA105_DEVICEID, pPciCard);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
		{
			DTA1XX_LOG( KERN_INFO,
				"The DTA-%d (Firmware Version %d) has started successfully.",
				g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
				g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
		}
		g_NumOfCards++;

		// look for more DTA-105s
		pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA105_DEVICEID, pPciCard);
	}

	// Scan for DTA-145s
	pPciCard = NULL;
	pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA145_DEVICEID, pPciCard);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
		{
			DTA1XX_LOG( KERN_INFO,
				"The DTA-%d (Firmware Version %d) has started successfully.",
				g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
				g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
		}
		g_NumOfCards++;

		// look for more DTA-145s
		pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA145_DEVICEID, pPciCard);
	}

	// Scan for DTA-2111s
	pPciCard = NULL;
	pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2111_DEVICEID, pPciCard);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
		{
			DTA1XX_LOG( KERN_INFO,
				"The DTA-%d (Firmware Version %d) has started successfully.",
				g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
				g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
		}
		g_NumOfCards++;

		// look for more DTA-2111s
		pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2111_DEVICEID, pPciCard);
	}

	// Scan for DTA-2135s
	pPciCard = NULL;
	pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2135_DEVICEID, pPciCard);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
		{
			DTA1XX_LOG( KERN_INFO,
				"The DTA-%d (Firmware Version %d) has started successfully.",
				g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
				g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
		}
		g_NumOfCards++;

		// look for more DTA-2135s
		pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2135_DEVICEID, pPciCard);
	}

	// Scan for DTA-2136s
	pPciCard = NULL;
	pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2136_DEVICEID, pPciCard);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
		{
			DTA1XX_LOG( KERN_INFO,
				"The DTA-%d (Firmware Version %d) has started successfully.",
				g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
				g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
		}
		g_NumOfCards++;

		// look for more DTA-2136s
		pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2136_DEVICEID, pPciCard);
	}
    
    // Scan for DTA-2137s
	pPciCard = NULL;
	pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2137_DEVICEID, pPciCard);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
		{
			DTA1XX_LOG( KERN_INFO,
				"The DTA-%d (Firmware Version %d) has started successfully.",
				g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
				g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
		}
		g_NumOfCards++;

		// look for more DTA-2137s
		pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2137_DEVICEID, pPciCard);
	}

	// Scan for DTA-2144s
	pPciCard = NULL;
	pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2144_DEVICEID, pPciCard);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
		{
			DTA1XX_LOG( KERN_INFO,
				"The DTA-%d (Firmware Version %d) has started successfully.",
				g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
				g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
		}
		g_NumOfCards++;

		// look for more DTA-2144s
		pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2144_DEVICEID, pPciCard);
	}
	
	// Scan for DTA-2142s
	pPciCard = NULL;
	pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2142_DEVICEID, pPciCard);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
		{
			DTA1XX_LOG( KERN_INFO,
				"The DTA-%d (Firmware Version %d) has started successfully.",
				g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
				g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
		}
		g_NumOfCards++;

		// look for more DTA-2142s
		pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2142_DEVICEID, pPciCard);
	}

	// Scan for DTA-2145s
	pPciCard = NULL;
	pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2145_DEVICEID, pPciCard);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
		{
			DTA1XX_LOG( KERN_INFO,
				"The DTA-%d (Firmware Version %d) has started successfully.",
				g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
				g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
		}
		g_NumOfCards++;

		// look for more DTA-2145s
		pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2145_DEVICEID, pPciCard);
	}

	// Scan for DTA-2160s
	pPciCard = NULL;
	pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2160_DEVICEID, pPciCard);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
		{
			DTA1XX_LOG( KERN_INFO,
				"The DTA-%d (Firmware Version %d) has started successfully.",
				g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
				g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
		}
		g_NumOfCards++;

		// look for more DTA-2160s
		pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA2160_DEVICEID, pPciCard);
	}

	// Scan for DTA-545s
	pPciCard = NULL;
	pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA545_DEVICEID, pPciCard);

	while ( (pPciCard!=NULL) && (g_NumOfCards<DTA1XX_MAX_CARDS) )
	{
		if ( 0==Dta1xxStartCard(pPciCard, &g_Dta1xxCards[g_NumOfCards]) )
		{
			DTA1XX_LOG( KERN_INFO,
				"The DTA-%d (Firmware Version %d) has started successfully.",
				g_Dta1xxCards[g_NumOfCards].m_TypeNumber,
				g_Dta1xxCards[g_NumOfCards].m_FirmwareVersion );
		}
		g_NumOfCards++;

		// look for more DTA-545s
		pPciCard = DTA1XX_PCI_FIND_DEVICE(DTA1XX_VENDORID, DTA545_DEVICEID, pPciCard);
	}

	DTA1XX_LOG( KERN_INFO, "Found %d DTA-1xx boards in the system", g_NumOfCards );

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register driver -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	r = register_chrdev(g_Dta1xxMajor, DRIVER_NAME, &g_Dta1xxFOps);
	if ( 0 >= r)
		DTA1XX_LOG( KERN_CRIT, "Dta1xxInitModule: cannot get major %d", g_Dta1xxMajor );

	if ( g_Dta1xxMajor==0 ) {
		// Dynamic major used ==> store returned number
		g_Dta1xxMajor = r;
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Major: %d", g_Dta1xxMajor );
#endif
	}

#if KERNELVERSION < 26
	// Set the module owner (kernel 2.4 only)
	SET_MODULE_OWNER(&g_Dta1xxFOps);
#endif

	DTA1XX_LOG( KERN_INFO, "The driver (Rev %d.%d.%d.%d) has loaded successfully.",
				DTA1XX_VERSION_MAJOR, DTA1XX_VERSION_MINOR, DTA1XX_VERSION_MICRO,
				DTA1XX_VERSION_BUILD );

    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCleanUpModule -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Module clean-up. Unregisters the module. Frees the resources used for the DTA-1xx
// cards.
//
void Dta1xxCleanUpModule()
{
	Int i;
#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxCleanUpModule" );
#endif

	// Stop cards
	for (i=0; i<g_NumOfCards; i++)
   		Dta1xxStopCard(&g_Dta1xxCards[i]);
 	g_NumOfCards = 0;

	// Unregister driver (major)
	unregister_chrdev(g_Dta1xxMajor, DRIVER_NAME);
}
module_init(Dta1xxInitModule);
module_exit(Dta1xxCleanUpModule);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxStartCard -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Initialisises the DTA-1xx card described by pPdo and allocates all resources needed to
// control the card.
//
int Dta1xxStartCard(
	struct pci_dev* pPdo,	// Physical device object, representing the DTA-1xx card
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
#if LOG_LEVEL > 0
 	DTA1XX_LOG( KERN_INFO, "Dta1xxStartCard" );
#endif

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Initialise FDO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// Store pointer to PCI device structure
	pFdo->m_pPdo = pPdo;

	// Copy location info
	pFdo->m_BusNumber	= pFdo->m_pPdo->bus->number;
	pFdo->m_SlotNumber	= PCI_SLOT(pFdo->m_pPdo->devfn);
	pFdo->m_DeviceId	= (UInt16)pPdo->device;
	pFdo->m_VendorId	= (UInt16)pPdo->vendor;
	pFdo->m_SubSystemId	= (UInt16)pPdo->subsystem_device;
	pFdo->m_SubVendorId	= (UInt16)pPdo->subsystem_vendor;

	// Determine whether this PCI card uses a PLX bridge chip (PCI9054/9056)
	pFdo->m_UsesPlxChip = (pFdo->m_VendorId==PLX_VENDORID) ? TRUE : FALSE;

    // Determine type of DTA-1xx card (from Subsystem ID or Device ID, depending on PLX)
	pFdo->m_TypeNumber = Dta1xxSubsystemId2TypeNumber(pFdo->m_UsesPlxChip ?
											pFdo->m_SubSystemId : pFdo->m_DeviceId);

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- Basic initialisation of card -.-.-.-.-.-.-.-.-.-.-.-.-.-

	// Enable the card
	if ( pci_enable_device(pFdo->m_pPdo)<0 )
		return -EFAULT;
	pci_set_master(pFdo->m_pPdo);
	pFdo->m_NwInitialised = FALSE;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Memory Mapping -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	
	// Initialise user mappings MUTEX
	init_MUTEX(&pFdo->m_UserMapMutex);
	
	// Aquire memory resources
	if ( 0!=pci_request_regions(pFdo->m_pPdo, DRIVER_NAME) )
	{
		DTA1XX_LOG( KERN_CRIT, "Dta1xxStartCard: failed to get memory resources" );
		return -EFAULT;
	}

	// Memory mapping for devices with PLX chip (BAR0 and BAR2)
	if ( pFdo->m_UsesPlxChip )
	{
		// Get PLX address range (BAR0)
		pFdo->m_Pci905XConfRegs.m_PciAddr = pci_resource_start(pFdo->m_pPdo, 0);
		pFdo->m_Pci905XConfRegs.m_Length = pci_resource_len(pFdo->m_pPdo, 0);
		// Map into kernel space
		pFdo->m_Pci905XConfRegs.m_pKernel = ioremap_nocache(
												pFdo->m_Pci905XConfRegs.m_PciAddr,
												pFdo->m_Pci905XConfRegs.m_Length);

		if ( (pFdo->m_Pci905XConfRegs.m_pKernel==NULL))
		{
			Dta1xxStopCard(pFdo);
			return -EFAULT;
		}

		// Get Dta1xx operational address range (BAR2)
		pFdo->m_Dta1xxRegsBase.m_PciAddr = pci_resource_start(pFdo->m_pPdo, 2);
		pFdo->m_Dta1xxRegsBase.m_Length = pci_resource_len(pFdo->m_pPdo, 2);
		// Map into kernel space
		pFdo->m_Dta1xxRegsBase.m_pKernel = ioremap_nocache(
												pFdo->m_Dta1xxRegsBase.m_PciAddr,
												pFdo->m_Dta1xxRegsBase.m_Length);

		if ( (pFdo->m_Dta1xxRegsBase.m_pKernel==NULL))
		{
			Dta1xxStopCard(pFdo);
			return -EFAULT;
		}

		// Initialise PCI-9054/9056 (BAR0 must be mapped)
		Dta1xxInitPci905X(pFdo);			// Cannot fail...
	}
	// Memory mapping for devices without PLX chip (only BAR0)
	else
	{
		// Get DTA1xx memory address range (BAR0)
		pFdo->m_Dta1xxRegsBase.m_PciAddr = pci_resource_start(pFdo->m_pPdo, 0);
		pFdo->m_Dta1xxRegsBase.m_Length = pci_resource_len(pFdo->m_pPdo, 0);
		// Map into kernel space
		pFdo->m_Dta1xxRegsBase.m_pKernel = ioremap_nocache(
												pFdo->m_Dta1xxRegsBase.m_PciAddr,
												pFdo->m_Dta1xxRegsBase.m_Length);

		if ( (pFdo->m_Dta1xxRegsBase.m_pKernel==NULL) )
		{
			Dta1xxStopCard(pFdo);
			return -EFAULT;
		}
	}

	//+=+=+=+=+=+=+=+=+=+ FROM HERE ON WE CAN ACCESS DTA-1XX REGISTERS +=+=+=+=+=+=+=+=+=+

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Initialise Hardware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// Init device
	if ( 0 != Dta1xxInitDevice(pFdo) )
	{
		Dta1xxStopCard(pFdo);
		return -EFAULT;
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Interrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	// Disable interrupts
	Dta1xxDisableInterrupts(pFdo);

	// No interupt handler registered yet
	pFdo->m_IntAttached = FALSE;

    // Request the interrupt
	if ( 0 > request_irq(pFdo->m_pPdo->irq,
						 (void*)Dta1xxIsr,	// interrupt service routine
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
						 SA_SHIRQ,			// Shared interrupt
#else
						 IRQF_SHARED,		// Shared interrupt
#endif
						 DRIVER_NAME,		// Driver name
						 pFdo) )			// is parameter
	{
		Dta1xxStopCard(pFdo);
		DTA1XX_LOG( KERN_CRIT, "Dta1xxStartCard: failed register int. handler (IRQ: %d)",
					pFdo->m_pPdo->irq);
		return -EFAULT;
	}
	// handler has been registered
	pFdo->m_IntAttached = TRUE;
	// Enable interrupts
	Dta1xxEnableInterrupts(pFdo);

	// Now initialise the hardware that needs the interrupt source
	Dta1xxInitDevicePost(pFdo);

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxStopCard -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Stops the card and deallocates all resources used by the card
//
void Dta1xxStopCard(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	Int i;
	Channel* pCh=NULL;
	ShBufDesc* pCdmaBuf=NULL;

#if LOG_LEVEL > 0
 	DTA1XX_LOG( KERN_INFO, "Dta1xxStopCard" );
#endif

	if (pFdo->m_NumIpPorts !=0 ) {
		// Stop the IP Rx Worker threads so the outstanding IRP for the DMA read
		// is cancelled if not already finished.
		// Otherwise the ReleaseRemoveLock will not be finished if no packets are
		// received from the DMA.
		Dta1xxNwIpRxStopWorkerThreads(pFdo);
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Free interrupt resources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	if ( pFdo->m_IntAttached )
	{
        Dta1xxDisableInterrupts(pFdo);
		free_irq(pFdo->m_pPdo->irq, pFdo);
		pFdo->m_IntAttached = FALSE;
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Disable the Phy -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	for ( i=0; i<pFdo->m_NumIpPorts; i++ )
		Dta1xxNwMacDisablePhy(&(pFdo->m_NwStore[i]));

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Release memory resources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	// Release and free IP Tx Cdma buffer
	// This must be executed before the DMA adapter is released
	// and the DMA resources are freed.
#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxStopCard: Release and free IP Tx Cdma buffer" );
#endif
	for ( i=0; i<pFdo->m_NumIpPorts; i++ )
		Dta1xxNwIpTxCleanUpCdmaBuffer(&pFdo->m_NwStore[i]);

	// Free IP Rx buffers
	for ( i=0; i<pFdo->m_NumIpPorts; i++ )
	{
		pCh = pFdo->m_IpMqRx[i];
		pCdmaBuf = &pCh->m_CdmaShBuf;
	
		if (pCdmaBuf->m_SglAllocated) {
			Dta1xxCdmaSglCleanUp(pCh);
		}

		if (pCdmaBuf->m_Allocated) {
			FreeCdmaBuffer(pCh);
			pCdmaBuf->m_Allocated = FALSE;
		}
	}
	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Free DMA buffer memory -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	if ( pFdo->m_pPdo!=NULL )
	{
		for ( i=0; i<pFdo->m_NumChannels; i++ )
		{
			pCh = &pFdo->m_Channel[i];
			pCdmaBuf = &pCh->m_CdmaShBuf;

			if ( pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr!=NULL )
			{
				// Free buffer
				pci_free_consistent(pFdo->m_pPdo, DTA1XX_DMA_BUF_SIZE,
					(void*)pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr,
					pCh->m_Dma.m_DmaBuffer.m_PhysStartAddr);
				pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr = NULL;
			}
			// Free scatter gather list descriptor
			if ( pCh->m_Dma.m_pSglDesc != NULL )
			{
				// Free buffer
				pci_free_consistent( pFdo->m_pPdo, sizeof(SglDesc),
					(void*)pCh->m_Dma.m_pSglDesc,
					pCh->m_Dma.m_PhysAddrSglDesc);
				pCh->m_Dma.m_pSglDesc = NULL;
			}

			// Free CDMA buffers
			if ( pCdmaBuf->m_Allocated )
				Dta1xxSharedBufRelease(pCh, i, pCdmaBuf, DTA1XX_SHBUF_DIRIQ_CDMA);
		}
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Free EEPROM cache -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	if ( pFdo->m_pEepromCache != NULL )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxStopCard: Free EEPROM cache buffer" );
#endif
		kfree( (char*)pFdo->m_pEepromCache );
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Free PCI memory resources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	// Unmap PCI IO memory
	if ( pFdo->m_UsesPlxChip )
	{
		if ( pFdo->m_Pci905XConfRegs.m_pKernel!=NULL )
		{
			iounmap( (char*)(pFdo->m_Pci905XConfRegs.m_pKernel) );
			pFdo->m_Pci905XConfRegs.m_pKernel = NULL;
		}
	}
	if ( pFdo->m_Dta1xxRegsBase.m_pKernel!=NULL )
	{
		iounmap( (char*)(pFdo->m_Dta1xxRegsBase.m_pKernel) );
		pFdo->m_Dta1xxRegsBase.m_pKernel = NULL;
	}

	// Finally: release PCI regions
	if ( pFdo->m_pPdo!=NULL )
	{
        pci_release_regions(pFdo->m_pPdo);
		pFdo->m_pPdo = NULL;
    }

	// Clear reference count
	pFdo->m_DvcRefCount = 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Opens a connection to a specific DTA-1xx card.
//
int Dta1xxOpen(
	struct inode *inode,
	struct file *filp)
{
	Int num;
	DTA1XX_FDO* pFdo;


#if LOG_LEVEL > 0
 	DTA1XX_LOG( KERN_INFO, "Dta1xxOpen" );
#endif

 	//.-.-.-.-.-.-.-.-.-.-.- Get the cards relative sequence number -.-.-.-.-.-.-.-.-.-.-.
	num = MINOR(inode->i_rdev);

 	// Do we have that many cards??
	if ( num >= g_NumOfCards)
		return -ENODEV;

	// Store pointer to card in private structure
	filp->private_data	= &g_Dta1xxCards[num];

	// Increment number of times opened
	pFdo = &g_Dta1xxCards[num];
	pFdo->m_DvcRefCount++;

#if LOG_LEVEL > 1
	DTA1XX_LOG( KERN_INFO, "Dta1xxOpen: Type=%d, RefCount=%d",
				pFdo->m_TypeNumber, pFdo->m_DvcRefCount );
#endif

#if KERNELVERSION < 26
	// Increment reference count (kernel 2.4 only)
	MOD_INC_USE_COUNT;
#endif

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxRelease -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Releases a connection to a specific DTA-1xx card
//
int Dta1xxRelease(
	struct inode *inode,
	struct file *filp)
{
	Int i, result=0;
	DTA1XX_FDO* pFdo = (DTA1XX_FDO*)filp->private_data;
	Channel* pCh;

#if LOG_LEVEL > 0
 	DTA1XX_LOG( KERN_INFO, "Dta1xxRelease" );
#endif

	// Get associated card info
	if (pFdo != NULL)
	{
		// Decrement number of times opened
		if ( pFdo->m_DvcRefCount > 0 )
			pFdo->m_DvcRefCount--;

#if LOG_LEVEL > 1
		DTA1XX_LOG( KERN_INFO, "Dta1xxRelease: Type=%d, RefCount=%d",
					pFdo->m_TypeNumber, pFdo->m_DvcRefCount );
#endif
		// Did we close the last reference to DTA-110/112/115/116/2111?
		if (   (   pFdo->m_DvcRefCount==0 )
		    && (   pFdo->m_TypeNumber==110 || pFdo->m_TypeNumber==112
			    || pFdo->m_TypeNumber==115 || pFdo->m_TypeNumber==116 
				|| pFdo->m_TypeNumber==2111) )
		{
			for ( i=0; i<pFdo->m_NumNonIpChannels; i++ )
			{
				pCh = &pFdo->m_Channel[i];

				// Stop CDMA for modulator channels.
				// - DTA-110/2111 modulator channel is port 1 (index 0)
				// - DTA-112/115/116 modulator channel is port 2 (index 1)
				if (    (pFdo->m_TypeNumber==110 && pCh->m_PortIndex==0)
					 || (pFdo->m_TypeNumber==112 && pCh->m_PortIndex==1)
					 || (pFdo->m_TypeNumber==115 && pCh->m_PortIndex==1)
					 || (pFdo->m_TypeNumber==116 && pCh->m_PortIndex==1)
					 || (pFdo->m_TypeNumber==2111&& pCh->m_PortIndex==0) )
				{
					// Stop CDMA
					Dta1xxCdmaStop( pCh);

					// Release shared CDMA buffer
					result = Dta1xxSharedBufRelease(pCh, pCh->m_PortIndex,
													&pCh->m_CdmaShBuf,
													DTA1XX_SHBUF_DIRIQ_CDMA);
#if LOG_LEVEL > 0
					DTA1XX_LOG( KERN_INFO, "Dta1xxRelease: CDMA Stop/Release result=%d",
								result );
#endif
				}
			}
		}
		// Purge any IP Rx and Tx channels that have not been closed properly
		Dta1xxNwIpRxPurgeLeftOverChannels(pFdo, filp);
		Dta1xxNwIpTxPurgeLeftOverChannels(pFdo, filp);

		// If no IpRx and IpTx channels are available we have to set AdminStatus to up
		if (pFdo->m_NumIpPorts != 0) {
			// Wait until the Rx & Tx structure is available
			ExAcquireFastMutex(&pFdo->m_IpRxFastMutex);
			ExAcquireFastMutex(&pFdo->m_IpTxFastMutex);
			if ((pFdo->m_pIpRx == NULL) && 
				(pFdo->m_pIpTx == NULL)) {
				// No more Ip channels available
				// Set AdminStatus to up
				for (i = 0; i < pFdo->m_NumIpPorts; i++) {
					pFdo->m_NwStore[i].m_AdminStatus = DTA1XX_NW_ADMINSTATUS_UP;
				}
			}
			ExReleaseFastMutex(&pFdo->m_IpTxFastMutex);
			ExReleaseFastMutex(&pFdo->m_IpRxFastMutex);
		}

		// Clear exclusive-access lock of current file-object if aquired
		result = Dta1xxCheckExclusiveAccess(pFdo, filp);

		result = Dta1xxUnmapOperRegsForFileObject(pFdo, filp);


	}

	// Set to NULL (i.e. no card associated)
	filp->private_data	= NULL;

#if KERNELVERSION < 26
 	// Decrement reference count (kernel 2.4 only)
	MOD_DEC_USE_COUNT;
#endif

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxMmap -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Map PCI device memory (register map) to userspace
//
// NOTE: PCI device memory might not be allocated on a page-aligned address, the BIOS does
//		 not look into that. 'io_remap_pfn_range' can only map page-aligned addresses
//		 We use IOCTL_DTA1XX_GET_ADDR_OPER_REGS the get the actual register address 
//		 base pointers
//
int Dta1xxMmap(
	struct file *filp,
	struct vm_area_struct *vma)
{
	UInt RegionStart=0, RegionSize=0;

	DTA1XX_FDO* pFdo = (DTA1XX_FDO*)filp->private_data;

	// Get start and size for operation register region
	RegionStart = pFdo->m_Dta1xxRegsBase.m_PciAddr;
	RegionSize =  pFdo->m_Dta1xxRegsBase.m_Length;

	// We only permit at least the same register block size, just to be safe
	if ((vma->vm_end - vma->vm_start) < RegionSize) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO,
					"Dta1xxMmap: Incorrect mapping length %u. Should be at least %u",
					(UInt)(vma->vm_end - vma->vm_start), RegionSize );
#endif
		return -EFAULT;
	}

	return Dta1xxMapOperRegsToUserSpace(pFdo, filp, vma);
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Child driver entry points +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxOpenI -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int Dta1xxOpenI(Int CardIndex, UInt32 ChildId)
{
	if ((CardIndex >= g_NumOfCards) || (CardIndex < 0))
		return -EFAULT;

	if (ChildId < 0)
		return -EFAULT;

	return 0;
}
EXPORT_SYMBOL(Dta1xxOpenI);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCloseI -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int Dta1xxCloseI(Int CardIndex, UInt32 ChildId)
{
	if ((CardIndex >= g_NumOfCards) || (CardIndex < 0))
		return -EFAULT;

	if (ChildId < 0)
		return -EFAULT;

	return 0;
}
EXPORT_SYMBOL(Dta1xxCloseI);

EXPORT_SYMBOL(Dta1xxIoCtlI);







