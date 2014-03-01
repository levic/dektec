//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dta1xx.h *#*#*#*#*#*#*#*#*# (C) 2000-2008 DekTec

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dta1xx driver, please refer to the Dta1xx readme

#ifndef __DTA1XX_H
#define __DTA1XX_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#ifdef __KERNEL__

#include <linux/version.h>
#include <linux/slab.h>				// kmalloc ()
#include <linux/fs.h>				// inode, file
#include <linux/errno.h>			// error codes
#include <linux/pci.h>				// pci_dev
#include <linux/wait.h>				// wait queue
#include <linux/sched.h>			// wait queue
#include <linux/ioctl.h>
#include <linux/interrupt.h>		// request_irq(), free_irq()
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
#include <linux/semaphore.h>		// semaphore
#else
#include <asm/semaphore.h>			// semaphore
#endif
#include <linux/pagemap.h>			// page_cache_release()
#include <linux/delay.h>			// udelay
#include <asm/scatterlist.h>		// Scatter/Gather list
#include <linux/spinlock.h>			// spinlock
#include <asm/pgtable.h>			// remap_page_range
#include <linux/list.h>				// linked lists

#ifdef CONFIG_COMPAT
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,28)
	#include <asm/compat.h>				// compat_ptr
	#else 
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
		#include <asm-x86/compat.h>		// compat_ptr
		#else
		#include <asm-x86_64/compat.h>	// compat_ptr
		#endif
	#endif
#endif // #ifdef CONFIG_COMPAT

#endif // #ifdef __KERNEL__

#include "DriverTypes.h"
#include "Pci905X.h"
#include "DriverVersion.h"
#include "CrossPlatform.h"
#include "Dta1xxCommon.h"
#include "Helpers.h"


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- LOG LEVEL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// LOG LEVEL with KdPrint
//  0	Off
//	1	Minimal (setting-up / breaking-down of channels)
//	2	Average (up to 10 per second)
//	3	All

// DMA related
#define LOG_LEVEL_DMA				0
#define LOG_LEVEL_CDMA				0

// IPRX related
#define LOG_LEVEL_IPRX				0

// IPTX related
#define LOG_LEVEL_IPTX				0

// NDIS related
#define LOG_LEVEL_NDIS				0

// MAC related
#define LOG_LEVEL_MAC				0

// Modulator related
#define LOG_LEVEL_MODULATION		0

// Genlock related
#define LOG_LEVEL_GENLOCK			0

// DVB-SPI/BT-656 related (0=none, 1:external calls, 2:+helper calls,
// 3:+DSS calls)
#define LOG_LEVEL_SPI				0

// Other logs (0=off not supported)
#define LOG_LEVEL					0

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Module ID information
#define DTA1XX_MAJOR 		0				// Default major number
#define DRIVER_NAME			"Dta1xx"		// Name of the driver

#define DTA1XX_MAX_CARDS	20		// Maximum number of DTA-1xx cards
											// supported

// DTA-1XX (sub-)vendor and (sub-)device ID's
#define DTA1XX_VENDORID_OLD	0x14B4			// DekTec vendor ID (legacy)
#define DTA1XX_VENDORID		0x1A0E			// DekTec vendor ID

#define DTA100_DEVICEID		0xD100			// DTA-100 device ID
#define DTA102_DEVICEID		0xD102			// DTA-102 device ID
#define DTA105_DEVICEID		0x0069			// DTA-105 device ID
#define DTA107_DEVICEID		0xD107			// DTA-107 device ID
#define DTA110_DEVICEID		0xD10A			// DTA-110 device ID
#define DTA111_DEVICEID		0x006F			// DTA-112 device ID
#define DTA112_DEVICEID		0x0070			// DTA-112 device ID
#define DTA115_DEVICEID		0x0073			// DTA-115 device ID
#define DTA116_DEVICEID		0x0074			// DTA-116 device ID
#define DTA117_DEVICEID		0x0075			// DTA-117 device ID
#define DTA120_DEVICEID		0xD114			// DTA-120 device ID
#define DTA122_DEVICEID		0xD116			// DTA-122 device ID
#define DTA124_DEVICEID		0xD118			// DTA-124 device ID
#define DTA140_DEVICEID		0xD128			// DTA-140 device ID
#define DTA145_DEVICEID		0x0091			// DTA-145 device ID
#define DTA2111_DEVICEID	0x083F			// DTA-2111 device ID
#define DTA2135_DEVICEID	0x0857			// DTA-2135 device ID
#define DTA2136_DEVICEID	0x0858			// DTA-2136 device ID
#define DTA2137_DEVICEID	0x0859			// DTA-2137 device ID
#define DTA2144_DEVICEID	0x0860			// DTA-2144 device ID
#define DTA2142_DEVICEID	0x085E			// DTA-2142 device ID
#define DTA2145_DEVICEID	0x0861			// DTA-2145 device ID
#define DTA160_DEVICEID		0x00A0			// DTA-160 device ID
#define DTA2160_DEVICEID	0x0870			// DTA-2160 device ID
#define DTA545_DEVICEID		0xD2BD			// DTA-545 device ID

// Driver log message helpers
#define DTA1XX_LOG(l,m,args...)			printk(l DRIVER_NAME ": " m "\n", ## args);

// DMA buffer size
#define DTA1XX_DMA_BUF_SIZE			(128*1024)
#define DTA1XX_DMA_READ                         0
#define DTA1XX_DMA_WRITE                        1

// Maximum total number of channels used by any application
// Exclusive of IP_RX and IP_TX channels
#define MAX_CHANNELS				7

// Maximum number of TS Rx+Tx channels
#define MAX_TS_CHANNELS				4

// Maximum number of IP/Ethernet ports on any card
#define MAX_IP_PORTS				1

// Max. number of channels listen on same port
#define MAX_NUM_IPRX_LISTEN			15

// Maximum number of times that Dta1xx registers can be mapped in user space in parallel
#define MAX_NUM_USER_MAPPINGS		50

// Number of periodic-interrupt periods used to measure input bit rate
#define NUM_RX_MEAS_PERIODS			250

// IP-Rx bit-rate measurement: maximum #samples stored
#define NUM_IPRX_MAX_SAMPLES		50

// IP-Rx bit-rate measurement: minimum time between samples in 54MHz cycles
#define NUM_IPRX_MIN_PERIOD			10800000				// 200ms

// Number of simultaneous outstanding IP Rx DMA's
#define NUM_IPRX_DMA_QUEUES			2			
#define IPRX_DMA_BUFFER_SIZE		(1024*1024)

// Device Extension Flags
#define FLAG_FDO					0			// It's a FDO
#define FLAG_PDO					1			// It's a PDO

#define DTA160_REF_CLOCK			54000		// 54MHz--> 1ms
#define DTA1XX_IPTX_PERINT			20			// ms
#define IPTX_DMA_BUFFER_SIZE		((8*1024*1024) - 4)
												// 8MB - 4 bytes. Max. length for 1
												// continuous block = 23 bits.
#define IPTX_DMA_MAX_TRANSFER_SIZE  (2*1024*1024)

// Multicast Lists
#define MAX_NUM_MC_ADDRESSES		256			// Maximum #multicast addresses

// IP-Rx FEC packets constants
#define FEC_INC_COLUMN_PORT			2			// Port number increment for FEC column
#define FEC_INC_ROW_PORT			4			// Port number increment for FEC row

#define TOT_NUM_RTP_PACKETS			((MAX_FEC_RECONSTR_ELEMENTS + MAX_NUM_RTP_DVB_PACKETS) * \
									MAX_NUM_FEC_MATRIX)

//#define MAX_NUM_FEC_SEQ_NUM        100  //L*D max 100, L max 20, D max 20
#define MAX_RTP_OUT_OF_SYNC			10


// Channel capabitilies
#define DTA1XX_CHCAP_IN_PORT        0x0001
#define DTA1XX_CHCAP_OUT_PORT       0x0002
#define DTA1XX_CHCAP_BIDIR_PORT     0x0003
#define DTA1XX_CHCAP_DBLBUF         0x0010  // Must be or-ed with one of the cap_x_port
#define DTA1XX_CHCAP_FAILSAFE       0x0020  // Must be or-ed with one of the cap_x_port
#define DTA1XX_CHCAP_LOOPTHR        0x0040  // Must be or-ed with one of the cap_x_port
#define	DTA1XX_CHCAP_DIVERSITY      0x0080  // Must be or-ed with one of the cap_x_port
#define DTA1XX_CHCAP_SHARED         0x0100  // Must be or-ed with one of the cap_x_port
#define DTA1XX_CHCAP_GENREF         0x0200  // Must be or-ed with one of the cap_x_port
#define DTA1XX_CHCAP_GENLOCKED      0x0400  // Must be or-ed with one of the cap_x_port
#define DTA1XX_CHCAP_SPI            0x0800  // Must be or-ed with one of the cap_x_port
#define DTA1XX_CHCAP_SPICLKSEL      0x1000  // Must be or-ed with one of the cap_x_port
#define DTA1XX_CHCAP_TSRATESEL      0x2000  // Must be or-ed with one of the cap_x_port
#define DTA1XX_CHCAP_APSK           0x4000  // Must be or-ed with one of the cap_x_port
#define DTA1XX_CHCAP_AD9789         0x8000  // Channel has AD9789 RF-DAC
#define DTA1XX_CHCAP_DACVGA         0x10000 // Channel has 10-bit RF output level control
#define DTA1XX_CHCAP_PQNCO          0x20000 // Channel has PQNCO 

// PHY types
#define PHY_UNKNOWN                 0
#define PHY_NATIONAL                1
#define PHY_MARVELL                 2

// LED flashing
#define DTA1XX_LEDFLASH_INIT        0
#define DTA1XX_LEDFLASH_ON          1
#define DTA1XX_LEDFLASH_OFF         2
#define DTA1XX_LEDFLASH_EXIT        3

// Target-detection states
#define DTA1XX_TD_INITIAL           0
#define DTA1XX_TD_FIND_TARGETID     1
#define DTA1XX_TD_OPEN_CIRCUIT      2
#define DTA1XX_TD_SPI_SINK          3
#define DTA1XX_TD_TARGETID_STABLE   4
#define DTA1XX_TD_TARGET_ERROR      5

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- VPD constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Valid VPD characters
#define  VALID_VPD_KEYW_CHAR(c) 	(((c)>='0' && (c)<='9') || ((c)>='A' && (c)<='Z'))

// *** PLX-attached EEPROM

// EEPROM attached to PLX 9054/9056
#define  PLX_VPD_SIZE				512

// Start and end of VPD sections
#define  PLX_VPD_ID_START			0x58	// Read-only section directly follows VPD-ID
#define  PLX_VPD_ID_START_NEW		0x63	// Read-only section directly follows VPD-ID: 9056 boards

#define  PLX_VPD_RW_START			0x100
#define  PLX_VPD_END				(PLX_VPD_SIZE - 1)

// Section lengths
#define  PLX_VPD_RW_LENGTH			(PLX_VPD_END - PLX_VPD_RW_START)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Macros -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// PCI-bus register read/write.
// pRegs must be declared volatile UInt8*.
#define READ_UINT32(p, offs)		(*(volatile UInt32*)(((UInt8*)p)+(offs)))
#define READ_UINT8(p, offs)			(*(volatile UInt8*)(((UInt8*)p)+(offs)))

#define WRITE_UINT32(p, offs, d)	(*(volatile UInt32*)(((UInt8*)p)+(offs))=(UInt32)(d))
#define WRITE_UINT16(p, offs, d)	(*(volatile UInt16*)(((UInt8*)p)+(offs))=(UInt16)(d))
#define WRITE_UINT8(p, offs, d)		(*(volatile UInt8*)(((UInt8*)p)+(offs))=(UInt8)(d))

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Globals -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _Globals {
	UInt32	m_MaxDmaTransferSize;		// Maximum transfer length used in a single
										// DMA scatter-gather descriptor
	Int  m_NwVersionMajor;				// Version number of Nw driver
	Int  m_NwVersionMinor;				// Version number of Nw driver
	Int  m_NwVersionMicro;				// Version number of Nw driver
	Int  m_NwVersionBuild;				// Version number of Nw driver
} Globals;

extern Globals  g_Globals;


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helper Structs +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- BitRateMeas -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Structure with variables to measure a bit rate.
//
typedef struct _BitRateMeas {
	// Bit-Rate Measurement
	Int  m_ValidDiff[NUM_RX_MEAS_PERIODS];
										// Number of Valid bytes received per period
										// (=difference between successive Valid counts)
	Int  m_LastIndex;					// Index in ValidCnt of last sample
	Int  m_LastValidCnt;				// Last absolute value of Valid counter
	Int  m_NumBytesTotal;				// Total #valid bytes in measurement period
	Int  m_NumValidSamps;				// Number of "valid" samples in m_ValidCnt[]
	Int  m_ValidCount256;				// #Valid bytes per interval, times 256
	Int  m_ValidCountSample;			// Valid count sample; read in interrupt routine
} BitRateMeas;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CdmaSegmDesc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// CDMA segment descriptor.
// A CDMA buffer is divided into 4 segments 0..3, this structure describes one segment
//
typedef struct _CdmaSegmDesc {
	UInt32  m_SglAddr;					// Scatter-Gather-List descriptor address
	Int  m_CdmaRelAddr;					// Relative segment address in CDMA buffer
										// SGLs may be multiple pages long, so segment
										// length may differ from exactly 1/4
} CdmaSegmDesc;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Network MII Serial Management -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Stores state of MiiSm communication (for DTA-160/2160)
//
typedef struct _MiiSm {
	volatile UInt8  State;
	volatile UInt8  SubState;
	volatile UInt8  Command;
	volatile UInt8	Count;
} MiiSm;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Shared Buffer Descriptor -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Stores state for a buffer that is shared between user application and driver
//
typedef struct _ShBufDesc
{
	BOOLEAN  m_Allocated;			// Shared buffer has been allocated

	UInt8*  m_pUserBuf;				// Virtual user address
	UInt8*  m_pBuf;					// System pointer to buffer
	
	struct page**  m_PageList;		// Buffer with page-list describing locked user buffer
	Int  m_NumPages;				// Number of pages

	BOOLEAN  m_SglAllocated;		// Scatter-Gather List has been allocated
	struct scatterlist* m_pSglKernel;	// Scatter-Gather list for user buffer
	Int m_NumSglEntries;			// Number of entries in kernel scatter gather list
	UInt8*  m_pSglBuf;				// Buffer with scatter-gather descriptors
	Int  m_FirstDescOffset;
	Int  m_SglBufSize;				// Scatter-gather-buffer size
	dma_addr_t  m_PhysAddrSglBuf;	// Physical start address of the sgl-buffer
} ShBufDesc;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- UserMapping -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _UserMapping {
	BOOLEAN  m_EntryUsed;				// TRUE if this entry is used
	void*  m_pUserVirtual;				// Virtual address in user mode
	PFILEOBJECT  m_FileObject;			// Corresponding file object
	volatile UInt  m_RefCount;			// Reference count
	void*  m_pPaMmap;					// Virtual address returned by mmap
} UserMapping;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct DmaBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DMA buffer descriptor
//
typedef struct _DmaBuffer
{
	UInt8*  m_pVirtStartAddr;		// Virtual kernel start address of the buffer
	dma_addr_t  m_PhysStartAddr;	// Physical start address of the buffer
} DmaBuffer;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct DmaChannel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Variables to describe and control a single DMA channel.
//
typedef struct _DmaChannel
{
	// Exclusive access
	struct semaphore  m_Lock;				// Protects DMA channel

	// Register pointers
	volatile UInt8*  m_pRegBase;			// Base address of DMA registers
	volatile UInt8*  m_pRegDmaDesc;			// DMA Descriptor Pointer
	volatile UInt8*  m_pRegCmdStat;			// DMA Command/Status

	Int  m_PlxChannel;						// PLX DMA channel number

	// Dma done event
	wait_queue_head_t  m_DmaDoneQueue;		// DMA done wait queue
	volatile Int  m_DmaDone;				// DMA done flag
	Int  m_DmaDir;							// Direction: DTA1XX_DMA_READ/DTA1XX_DMA_WRITE
	DmaBuffer  m_DmaBuffer;				// The DMA buffer

	// Scatter/gather list descriptor used for DMA transfer to the DTA-124
	SglDesc*  m_pSglDesc;					// Scatter/Gather descriptor
	dma_addr_t  m_PhysAddrSglDesc;			// Physical address of the sgl descriptor

	// Length of current transfer
	volatile UInt  m_CurrentTransferLength;			// For now, only used for IpTx and IpRx
	
} DmaChannel;

//-.-.-.-.-.-.-.-.-.-.-.-.- FrequencyResponseCompensation -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// frequency-response compensation table
//
typedef struct _FreqCompTableEntry {
	Int16 m_Freq;		// Frequency in MHz
	Int16 m_Fsc;		// Full scale current
	Int16 m_Attn;		// Attenuation in dBm
} FreqCompTableEntry, FreqCompTable[], *PFreqCompTable;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct MemDesc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Memory-space descriptor, used for PCI-9054 and Dta1xx operational registers.
//
typedef struct _MemDesc {
    unsigned int	m_PciAddr;			// PCI-bus relative start address
	unsigned int	m_Length;			// Length of memory block
	volatile UInt8*	m_pKernel;			// Address when mapped in kernel space.
} MemDesc;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct TargetIdDetection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Target-ID Detection (for DTA-102/DTA-122)
//
typedef struct _TargetIdDetection {
	int  m_TargetId;					// Current best guess for target-adapter ID
	int  m_Present;						// Current best guess for target-adapter presence
	int  m_TdState;						// Target-detection state
	int  m_LastTargetId;				// Last measurement for target-adapter ID
	int  m_NumTimesInFind;				// #times in state DTA1XX_TD_FIND_TARGETID
	int  m_StableCount;					// Counter to assess whether measurement is stable
	int  m_TargetError;					// Target Error, set in target-detection state
										// machine, cleared in Dta1xxTxProcessStatusFlags
} TargetIdDetection;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Channel +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Describes a single channel, which is either:
//	- Transport-Stream Receive or Transmit Channel (User <-> PCI Card)
//	- IP "Main-Queue" Receive or Transmit Channel (Driver <-> PCI Card)
//  - NW Receive or Transmit Channel (NW Driver <-> PCI Card)
//
typedef struct _Channel
{
	// Identity information: Reference back to device object and index
	struct _DTA1XX_FDO*	m_pFdo;

	Int  m_ChannelType;					// DTA1XX_CHANNEL_UNUSED/DTA1XX_TS_TX_CHANNEL/...
	Int  m_PortIndex;					// Index in Device Extension array
	Int  m_Capability;					// Channel capability
	Int  m_IoConfig;					// Current IO config
	Int  m_IoConfigPar;					// Extra paremeter for IO config
	Int  m_SpiMode;                     // Current SPI mode
    Int  m_TxRateSel;                   // Current TxRate selection
    Int  m_ExtClkFreq;                  // External frequency rate
                                        // This is a parameter in TS ratio calculations

	// Port properties
	UInt32  m_TxRate;					// Current Tx channel - Tx interface word rate (dHz)

	// General
	volatile UInt8*  m_pRegBase;			// Base address of channel registers
	// Address map location helpers
	//
	// Below is an example of a typical address map of a bidir port
	//
	// m_pRegBase + m_AddrRxRegMapOffset	->	rx-registers (typ. offset=0x000)
	// m_pRegBase + m_AddrTxRegMapOffset	->	tx-registers (typ. offset=0x080)
	// m_pRegBase + m_AddrDmaRegMapOffset	->	dma-registers (typ. offset=0x100)
	//
	Int  m_AddrBaseOffset;				// RegBase offset within cards address space
										// (i.e. offset from card's base address)
	Int  m_AddrRxRegMapOffset;			// Internal offset to rx-registers
	Int  m_AddrTxRegMapOffset;			// Internal offset to tx-registers
	Int  m_AddrDmaRegMapOffset;			// Internal offset to dma-registers

	// DMA
	DmaChannel  m_Dma;					// DMA channel for reading data from channel

	// Exclusive Access
	struct semaphore  m_ExclAccLock;	// Protect updates to channel members
	Int	 m_ExclAccess;					// Does someone have exclusive access?
	PFILEOBJECT  m_pExclAccFileObject;	// File object used to request exclusive access

	//.-.-.-.-.-.-.-.-.-.-.-.- Members for Continuous DMA (CDMA) -.-.-.-.-.-.-.-.-.-.-.-.-
	// CDMA buffer is divided into 4 segments 0..3
	Int  m_CdmaBufSize;					// CMDA buffer size in #bytes
	BOOLEAN  m_CdmaDmaRunning;			// CDMA running
	KDPC  m_CdmaDpcCompleted;			// CDMA DPC for regular interrupts
	UInt32  m_CdmaRefClkSamp;			// Reference-clock sample @ last segment interrupt
	volatile Int  m_CdmaSegment;		// Segment# in CDMA buffer currently being DMA-ed
	CdmaSegmDesc  m_CdmaSegmDesc[4];	// Segment descriptors
	ShBufDesc  m_CdmaShBuf;				// Shared CDMA-buffer descriptor
	Int64 m_CdmaClockDiv;				// Clock divider used to compute buffer position
										// in Dta1xxCdmaGetBufPos method
	volatile Int m_CdmaDone;			// CDMA done status
	wait_queue_head_t
 					m_CdmaEvent;		// CDMA event queue

	// Status Flags: Updated every periodic interrupt
	Int  m_Status;						// Actual value of status flags
	Int  m_Latched;						// Latched value of status flags
//	Int  m_FlagsToClear;				// Flags to be cleared

	// Bit rate measurement
	BitRateMeas  m_BrMeas;				// Bit-rate measurement
	TargetIdDetection  m_TrgIdDet;	// For DTA-102/122 target-ID detection

	//-.-.-.-.-.-.-.-.-.-.- Members Specific for TS Receive Channel -.-.-.-.-.-.-.-.-.-.-.

	Int64  m_RfFreq;				    // RF-frequency (in Hz)
	Int  m_OutpLeveldBm;			    // Output level in dBm (expressed in 0.1 dB units)
	Int  m_OutpLevelOffset;			    // Output driver DAC offset dBm (expressed in 0.1 dB units)
	Int  m_OutpLevelGain;			    // Output driver DAC gain 1024/dBm (expressed in 0.1 dB units)
	PFreqCompTable m_FreqCompTable;     // frequency-response compensation table
	Int  m_FreqCompTableSize;           // number of entries in table
	Int32  m_Nco0FreqTuningWord;        // Cached Tx NCO #0 tuning word

	// Statistics
	Int  m_ViolCountSamp;				// Sample of code-violation counter (DTA-120)

	//-.-.-.-.-.-.-.-.-.-.- Members Specific for TS Transmit Channel -.-.-.-.-.-.-.-.-.-.-

	// Rate
	Int  m_TsSymOrSampRate;				// TS-, Symbol- or Sample rate
										// This is a shadow variable: value may be derived
										// from hardware too, but with less precision

	// Synchronisation
	struct semaphore  m_MutexTxControl;	// Mutex to protect access to TxControl register

	// Current modulation parameters for DTA-110/110T/115/2111
	// This is for remembering the last modulation settings between applications
	// In device-extension initialisation, an attempt is made to initialise these
	// variables in a meaningful way. If this is not possible => initialise to -1
	Int  m_ModType;						// Modulation type (to know whether this is OFDM)
	Int  m_XtraPar[3];					// Extra parameters
	Int  m_ModSampRate;					// Sample rate, used in Dta1xxCdmaGetBufPos

	// Failsafe for DTA-145/2145
	volatile UInt32 m_FailsafeCnt;
	UInt32 m_FailsafeTimeout;			// Timeout set by application. Default 200ms
	UInt32 m_FailsafeTimeoutCnt;		// Calculated
	Int m_FailsafeEnable;				// If enabled, the Timeout is checked.
										// Otherwise relais is always in failsafe state if
										// Failsafe mode is active
	// Genlock
	UInt m_RxFramePrev, m_RxLinePrev, m_RxSampPrev;
	Int64 m_RxPosPrev;

	// SPI interface configuration
	struct _Dta1xxSpiGen*  m_pSpiReg;	// Pointer to SPI registers
	FAST_MUTEX   m_DssMutex;      // Protect simultaneous access to DSS
	UInt32  m_DssFreq;		            // Current DSS frequency

} Channel;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ RtpListEntry +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Describes an entry for the storage of RTP packets 
//
typedef struct _RtpListEntry {
	LIST_ENTRY m_ListEntry;
	UInt16  m_RTPSequenceNumber;
	UInt16	m_BufIndex;			// Index of buffer.
	UInt16	m_PayloadOffset;	// Offset of the actual payload data (DVB/FEC) (bytes)
	UInt16  m_RTPOffset:15;		// Offset of the RTP header packet
	UInt16	m_InUse:1;			// 1: InUse by RTP reconstructor
			
	// For FEC packets only
	UInt16  m_FecSNBase;	// Base Sequence number DVB packet for this Fec packet
	UInt8   m_FecOffset;	// L parameter for FEC column, D parameter for FEC row
	UInt8	m_FecNA;		// Number of media packets beloging to this FEC.

} RtpListEntry;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ UserIpRxChannel +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// This structure holds the state data for a single User-IP Receive channel.
//
typedef struct _UserIpRxChannel {
	Int  m_IpPortIndex;
	Int  m_ChannelIndex;				// Channel index within set of IpRx channels
	PFILEOBJECT  m_FileObject;			// File object of process associated with this
										// channel. The purpose, amongst others, is to
										// purging left-over channel objects in Close
	// Receive FIFO buffer and buffer state
	// Buffer consists of "real" buffer + 256 auxialiary bytes for easy buffer-wrapping
	// + RTP packets (FEC row/column + RTP DVB)
	ShBufDesc m_SharedBuf;				// Shared User-buffer descriptor

	UInt8*  m_pEndOfBuffer;				// Last location of Stream buffer + 1
	UInt8*	m_pRtpBuffer;				// Start of RTP buffer
	Int  m_BufSize;						// Buffer size; excludes auxiliary space
	volatile UInt8*  m_pRead;			// Read pointer; updated by user thread
	volatile UInt8*  m_pWrite;			// Write pointer; updated by worker thread

	// Fragmented packets
	UInt16	m_FragmentId;
	UInt16	m_FragmentOffset;

	// Handshaking
	ATOMIC m_UseCount;					// Number of thread using this struct
										// InterlockedIncrement/InterlockedDecrement is used
	volatile BOOLEAN m_RxIncomingPackets;		 // True if new RTP packets are available
	
	// Status flags
	// DTAPI_RX_FIFO_OVF, DTAPI_RX_SYNC_ERR
	Int  m_Flags;						// DTAPI_RX_FIFO_OVF, DTAPI_RX_SYNC_ERR
	Int  m_LatchedFlags;				// Latched value of m_Flags
	KSPIN_LOCK  m_FlagsSpinLock;		// Access protection
	Int  m_PckSizeSrc;					// (Latest) Packet size of TPs in IP packets
										// 0=UNKNOWN, -1=INVALID, 188, 204

	// Control of reception process
	Int  m_RxControl;					// DTA1XX_RXCTRL_IDLE/DTA1XX_RXCTRL_RCV
	Int  m_RxMode;						// DTA1XX_RXMODE_ST188/...
	Int  m_PckSizeDst;					// #bytes per packet written to buffer, taking
										// into account receive mode
	UInt16  m_TrpModePckSeqCount;		// Packet sequence number of transparent-mode-packets

	// Bit-rate measurement
	UInt32  m_NumPckBytesRcvd;			// Total #packet bytes received
	Int  m_BrmNumSamples;				// #valid samples in m_BrmPckBytesDiff
	Int  m_BrmIdxLastSample;			// Index in ValidCnt of last sample received
	Int  m_BrmEstimate;					// Latest bit rate estimate
	UInt64  m_BrmLastRxTimeStamp;		// Last timestamp received

	// Measurement samples: #packet bytes and time stamp (in #50-MHz cycles)
	UInt32  m_BrmSampleNumPckBytes[NUM_IPRX_MAX_SAMPLES];
	UInt64  m_BrmSampleTimeStamp[NUM_IPRX_MAX_SAMPLES];

	// IP Parameters
	UInt16 m_SrcPort;
	UInt16 m_DstPort;
	UInt8  m_SrcIPAddress[4];				
	UInt8  m_DstIPAddress[4];			// Used for multicast receive
	Int    m_FecMode;
	UInt8  m_IpParsValid;				// True if IpPars is set

	// Detected IP parameters
	UInt8  m_DetFecNumRows;				// Used for status to user appli
	UInt8  m_DetFecNumColumns;			// Used for status to user appli
	UInt8  m_DetNumTpPerIp;				// Used for status to user appli
	UInt8  m_DetProtocol;				// Used for status to user appli
	UInt8  m_RstCntFecRow;				// Used for clearing m_DetFecNumRows
	UInt8  m_RstCntFecColumn;			// Used for clearing m_DetFecNumColumn

	// RTP Packets List and admin
	LIST_ENTRY  m_RtpEmptyList;
	LIST_ENTRY  m_RtpFecRowList;
	LIST_ENTRY  m_RtpFecColumnList;
	LIST_ENTRY  m_RtpDvbList;
	UInt8*  m_pRtpListEntries;			// Base pointer of array of Rtp list entries
	KSPIN_LOCK  m_RtpListSpinLock;		// Spinlock for the Rtp list pointers
	UInt16  m_RtpLastSeqNum;			// Last used sequence number
	BOOLEAN  m_RtpFirstPacket;			// TRUE, if no DVB packets are received yet
	
	KSPIN_LOCK  m_StatisticsSpinLock;	// Access protection
	UInt  m_NumPacketsReconstructed;
	UInt  m_NumPacketsNotReconstructed;
	UInt  m_TotNumPackets;

	struct _UserIpRxChannel* m_pNext;	// Pointer to next UserIpRxChannel element
	struct _UserIpRxChannel* m_pPrev;	// Pointer to previous UserIpRxChannel element
} UserIpRxChannel;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ UserIpTxChannel +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//

// This structure holds the state data for a single User-IP Transmit channel.
//
typedef struct _UserIpTxChannel {
	Int  m_IpPortIndex;					// IP-port index
	Int  m_ChannelIndex;				// Channel index within set of IpRx channels
	PFILEOBJECT  m_FileObject;			// File object of process associated with this
										// channel. The purpose, amongst others, is to
										// purge left-over channel objects in Close

	// Transmit-FIFO Packet Buffer, shared with DTAPI (user mode)
	// The buffer contains time-stamped IP packet payloads
	// If the transmission parameters are changed, the buffer is reallocated
	ShBufDesc  m_SharedBuf;				// Shared User-buffer descriptor
	UInt  m_BufSize;					// Buffer size
	
	// Status flags
	Int  m_Flags;						// 
	Int  m_LatchedFlags;				// Latched value of m_Flags
	KSPIN_LOCK  m_FlagsSpinLock;		// Access protection

	// Control of transmission process
	Int  m_TxControl;					// DTA1XX_TXCTRL_IDLE/HOLD/SEND
	Int  m_TxMode;						// Not used (handled in DTAPI)

	// IP Parameters
	UInt8  m_DstIp[4];					// Destination IP address
	UInt16  m_DstPort;					// Destination port number
	UInt  m_Protocol;					// UDP or RTP
	UInt  m_FecMode;					// Error correction mode
	UInt  m_FecNumRows;					// �L� = #rows in FEC matrix
	UInt  m_FecNumCols;					// �D� = #columns in FEC matrix
	UInt8  m_DstMacAddr[6];				// Destination MAC address: Not used for now
	UserIpTxCommon*  m_pCommon;			// shared memory between Dtapi and Dta1xx driver

	UInt32  m_LastTimestampOffs;		// Offset of last timestamp. Used for offset wrapping
	UInt64  m_RefTimestamp;				// Initial timestamp after starting Tx channel
										// Incremented after timestamp offset wrapping
	
	struct _UserIpTxChannel* m_pNext;	// Pointer to next UserIpTxChannel element
	struct _UserIpTxChannel* m_pPrev;	// Pointer to previous UserIpTxChannel element
} UserIpTxChannel;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ NwStore +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// This structure holds the state data for a single network (Ethernet) port.
//
typedef struct _NwStore {
	// Identity information: Reference back to device object and index
	struct _DTA1XX_FDO*	m_pFdo;
	Int    m_IpPortIndex;				// IP Port index in Device Extension array
	UInt8*   m_pRegBase;				// Base address of channel registers

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General Ethernet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	UInt8  m_MCList[MAX_NUM_MC_ADDRESSES][6];	// Multicast list
	UInt  m_NumMulticast;				// Number of multicasts enabled
	UInt8  m_MacAddrPer[6];				// Network MAC address retrieved from VPD
	UInt8  m_MacAddrCur[6];				// Network MAC address retrieved from VPD and can
										// be changed by NDIS
	volatile UInt8  m_AdminStatus;		// Administrative status of network 
		
	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Phy/Mac cmd/stat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	UInt8  m_PhyType;					// Type of phy used. PHY_NATIONAL, PHY_MARVELL
	UInt8  m_PhyModel;					// Model of phy used.
	MiiSm  m_MiiSm;						// Network Serial Management protocol state info
	UInt8  m_PhySpeed  : 3;				// Network speed from PHY:
										// 0 = 10, 1 = 100, 2 = 1000, 3 = reserved
	UInt8  m_PhyLink   : 2;				// Network link status form PHY: 
										// 0 = no link, 1 = good link
	UInt8  m_PhyDuplex : 2;				// Network duplex status from PHY: 
										// 0 = half, 1 = full
	UInt8  m_PhyMaster : 1;				// 0 = Slave mode, 1 = Master mode
	UInt8  m_MacSpeed  : 3;				// Network speed MAC:
										// 0 = 10, 1 = 100, 2 = 1000, 3 = reserved
										// 4 = Not set
	UInt8  m_NwiLink  : 2;				// Network link to NDis driver
										// 0 = discon., 1 = connected, 2: not set
	UInt8  m_MacDuplex : 2;				// Network duplex MAC:
										// 0 = half, 1 = full, 2: Not set
	KDPC  m_MacDpc;						// DPC for updating MAC status
	KMUTEX  m_MacMutex;					// Mutex to prevent updating MAC status
										// more then ones
	volatile BOOLEAN m_MacPending;		// If TRUE, a MAC status update must be handled
	UInt  m_InterruptPending;			// If TRUE, an MDIO interrupt is pending
	BOOLEAN  m_LoopbackEnable;			// If TRUE, the phy is set in loopback mode
										// The Tx data is routed back to Rx data
	BOOLEAN  m_ForceSpeedEnable;		// If TRUE, m_ForcedSpeed is used for Phy speed
	UInt  m_ForcedSpeed;				// See m_NwiSpeed for possible values
	FAST_MUTEX  m_FastMutexCounters;	// Mutex for reading MAC counters
	
	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ndis cmd/stat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//PIRP  m_StatusIrp;					// IRP currently pending for status indication
	DTA1XX_NWI_STATUS*  m_pStatus;		// Current status block from Network driver
	KSPIN_LOCK  m_StatusSpinLock;		// Spinlock for the status IRP
	UInt  m_NwiSpeed;					// NDis speed/duplex setting: 
										// 0 = Auto Detect, 1 = 10MB-half, 2 = 10MB-full, 
										// 3 = 100MB-half,  4 = 100MB-full,
										// 255 = power down
	volatile BOOLEAN  m_SpeedPending;	// If TRUE, NDis has sent a new speed setting to
										// Dta1xx but setting is not yet handled
	ULONG  m_NwiPacketFilter;			// NDis packet filter
	ULONG  m_MacPacketFilter;			// Current Mac packet filter
	//PDEVICE_OBJECT  m_NdisDO;			// NDIS Device object
	BOOLEAN  m_EnableNwiRx;				// TRUE: NDis is enabled and PacketFilter <> 0
	volatile BOOLEAN  m_LinkPending;	// If TRUE, the link status has been changed, but not yet
										// send to NDis
	volatile BOOLEAN m_LinkStatDPCBusy;	// Link Status DPC has been started
	KDPC  m_LinkStatDPC;				// DPC for sending LinkStatus to NDIS

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Mac setup frame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	KDPC  m_SetupFrameDpc;				// DPC for sending Setup Frame to MAC
										// (creating workitem)
	KMUTEX m_SetupFrameMutex;			// Mutex to prevent sending the setup frame
										// more then ones
	volatile BOOLEAN m_SetupFrameRunning;// If TRUE, a setyup fram is running
	volatile BOOLEAN m_SetupFramePending;// If TRUE, a setup frame must be send to the Phy
	IO_WORKITEM  m_SetupFrameWI;		// Workitem for sending Setup Frame to MAC
	
	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IpTx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	KDPC  m_IpTxDPC;					// DPC for sending DVB IP packets to the FPGA
	ATOMIC  m_IpTxNumBytesToSend;		// Number of bytes ready to be send using Cdma
	UInt  m_IpTxBufSize;				// Size of cdma buffer
	UInt8*  m_pIpTxRead;				// Current read pointer of Cdma buffer
	UInt8*  m_pIpTxWrite;				// Current write pointer of Cdma buffer
	UInt8*  m_pIpTxEndOfBuffer;			// End of Cdma buffer
	volatile BOOLEAN m_IpTxStop;		// TRUE: IpTx must be stopped
	
	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IpRx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	Dta1xxNwBuf*  m_pNwRxQueue;			// Internal Queue for NDis receive
	PKTHREAD  m_IPRxThread;				// System worker thread for IP receive packets
	UInt8*  m_pIpRxBuffer[NUM_IPRX_DMA_QUEUES];
										// Buffer for DMA received data
	volatile ULONG	m_NumRxFifoOverflows;
										// Number of times packets were lost due to fifo overflow
	
	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ethernet status/counter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	volatile PIRP  m_StatCntpIrp;		// IRP currently pending for Status/Counter retrieval
	volatile BOOLEAN  m_StatCntPending;	// If TRUE, the get Status/Counter must be retrieved
	KDPC  m_StatCntDPC;					// DPC for completing the Status/Counter IRP
	UInt8  m_StatCntMode;				// 0 = Get, 1 = Set
	volatile UInt32 m_StatCntValue;		// The return value for mode get
} NwStore, *PNwStore;


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ struct DTA1XX_FDO +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
typedef struct _DTA1XX_FDO {

	struct pci_dev*	m_pPdo;				// The physical device object (PCI card info)

	// Location Info
	UInt32  m_BusNumber;				// PCI-bus number
	UInt32  m_SlotNumber;				// PCI-bus slot number
	UInt16  m_DeviceId;					// Device ID
	UInt16  m_VendorId;					// Vendor ID
	UInt16  m_SubSystemId;				// Subsystem ID
	UInt16  m_SubVendorId;				// Subsystem Vendor ID
	Int  m_TypeNumber;					// Type number in decimal, e.g. 102
	Int  m_FirmwareVersion;				// Firmware Version (= Altera revision),
										// e.g. 3 for "Firmware Version 3"
	Int  m_FirmwareVariant;				// Firmware Variant, e.g. to distinguish between
										// firmware with different #inputs/#outputs

	// Hardware characteristics
	BOOLEAN  m_Dta110T;					// DTA-110 with DTA-110T valid license yes/no
	BOOLEAN  m_UsesGenReg;				// Uses General-Registers block yes/no
	BOOLEAN  m_UsesDmaInFpga;			// Uses DMA Controller in Altera, not in PLX
	BOOLEAN  m_UsesPlxChip;				// Uses PLX bridge chip yes/no
	BOOLEAN  m_Uses9054;				// Uses PCI9054 yes/no
	BOOLEAN  m_Uses9056;				// Uses PCI9056 yes/no

	// Channels between driver and hardware
	Int  m_NumChannels;					// Total #channels
	Int  m_NumNonIpChannels;			// Total #non-IP channels
	Channel  m_Channel[MAX_CHANNELS];	// Actual channel data

	// Pointers into channel data: IP channels that are used to communicate between
	// driver and PCI card, but that are not directly connected with the user application
	Int  m_NumIpPorts;					// #IP ports
	Channel*  m_IpMqRx[MAX_IP_PORTS];	// Pointers to IP-MQ-Receive channels (NW+TS)
	Channel*  m_IpMqTx[MAX_IP_PORTS];	// Pointers to IP-MQ-Transmit channels (TS)
	Channel*  m_NwTx[MAX_IP_PORTS];		// Pointers to ETH-Transmit channels (NW)

	
	// User IP channel data
	UserIpRxChannel*  m_pIpRx;			// Double Linked list of User-IP-Rx channels
	UserIpTxChannel*  m_pIpTx;			// Double Linked list of User-IP-Tx channels
	FAST_MUTEX	m_IpRxFastMutex2;		// Mutex to protect access to m_pIpRx by worker threads
	FAST_MUTEX  m_IpRxFastMutex;		// Mutex to protect access to m_pIpRx during Add/Remove/Search
	KSPIN_LOCK  m_IpRxBrmSpinlock;		// Mutex to protect access to m_pIpRx during bitrate measurement
	BOOLEAN  m_IpRxBrmSkip;				// TRUE if m_pIpRx is modified and may not be used in the Brm Dpc
	KDPC  m_DpcBrmIpRx;					// DPC for IpRx bitrate measurement
	KSPIN_LOCK	m_IpTxSpinLock;			// Spinlock to protect access to m_pIpTx by DPC's
	FAST_MUTEX	m_IpTxFastMutex;		// Mutex to protect access to m_pIpTx during Add/Remove/Search
	
 	// Interrupt
	BOOLEAN  m_IntAttached;				// Interrupt handler attached (yes/no)

	// Device reference count
	Int  m_DvcRefCount;					// Number of times the device is opened

	// LED Flashing after driver start-up
	// Dynamic
	Int  m_LedFlashFlashCounter;		// Counts number of LED flashes
	BOOLEAN  m_LedFlashStartUp;			// Indicates LED flashing is active
	Int  m_LedFlashStateCounter;		// Counts number of interrupts
	Int  m_LedFlashState;				// State: DTA1XX_LEDFLASH_INIT/...
	// Static
	Int  m_LedFlashDurationExit;		// Duration of EXIT period (LED off)
	Int  m_LedFlashDurationInit;		// Duration of INIT period (LED off)
	Int  m_LedFlashDurationOff;			// Duration of LED-off period
	Int  m_LedFlashDurationOn;			// Duration of LED-on period
	Int  m_LedFlashNumFlashes;			// Number of times LED has to flashes

	// Memory Management
	MemDesc  m_Pci905XConfRegs;			// PCI 9054/9056 Local Configuration registers
	MemDesc  m_Dta1xxRegsBase;			// Base address of registers outside PCI 9054/9056
	volatile UInt8*  m_pGenRegs;		// Pointer to General register block
	UserMapping  m_UserMapping[MAX_NUM_USER_MAPPINGS];
	struct semaphore  m_UserMapMutex;	// Mutex to protect against simultaneous access 
										// to m_UserMapping

    // EEPROM Caching for VPD managment
    // 1. PCI Cards that use the Altera flash for storing VPF
    //    (will probably become obsolete)
    Int  m_EepromAddrLastSector;		// Address of last sector in EEPROM
    Int  m_EepromRelAddrVpd;			// Address of VPD data in m_pEepromCache
    Int  m_EepromSectorSize;			// Size in bytes of a single EEPROM sector
    Int  m_EepromSize;					// Size in bytes of full EEPROM
	BOOLEAN  m_UseFpgaForVpd;			// If !=0 and m_UsesPlxChip == 0, FPGA is used to
										// store the VPD data

	// 2. PCI cards with a small, VPD-specific EEPROM connected to FPGA
	Int  m_VpdEepromPageSize;			// Size of 1 page

	// 3. PCI cards with a small, VPD-specific EEPROM connected to PLX
	struct semaphore m_VpdEepromMutex;	// Mutex to protect access to VPD EEPROM

	// Common VPD data
	// Common VPD data
	UInt8*  m_pEepromCache;				// Cache for VPD EEPROM content.
										// Can contain part of other data
	UInt8*  m_pVpdEeprom;				// Pointer to start VPD data
	Int  m_VpdIdOffset;					// Offset of start VPD ID
	Int  m_VpdEepromSize;				// Size of VPD area
	struct semaphore  m_VpdMutex;		// Mutex to protect simultaneous read/write from/to VPD

	// Data for network connections and associated Nw driver
	NwStore	 m_NwStore[MAX_IP_PORTS];	// Store for MAC/PHY related attributes
	BOOLEAN  m_NwInitialised;			// TRUE if worker threads are initialised

	// I2C interface
	BOOLEAN  m_SupportsI2cItf;			// Device support the I2C interface
	struct semaphore  m_I2cMutex;		// Mutex to protect against simultaneous access
										// to I2C interface

    struct file*  m_pI2cExclAccFileObj; // File object that owns the I2C lock
    struct semaphore  m_I2cExclAccLock; // Protect updates to m_pI2cExclAccFileObj
    Int  m_I2cExclAccRecursiveCount;    // Counts number of recursive exclusive access

	// IP Rx 
	PKTHREAD  m_IPRxRecThread;			// System worker thread for IP receive reconstruction
	KEVENT  m_IPRxPacketAvailEvent;		// New packet is available in 1 or more channels
	volatile Int m_IPRxPacketAvailTrig;	// 1 if new packet is available in 1 or more channels
	BOOLEAN	m_IPRxPacketAvail;			// TRUE: New RTP packet is found during packet parsing
	KEVENT  m_IPRxThreadStopEvent;		// Event to signal the Receiver thread(s) to stop

	// Clock compensation
	Int  m_ClockErrPpb;					// Master clock frequency error in ppb
	Int  m_ClockPerIntCnt;				// Periodic interrupt counter; Used to dispatch
										// a DPC every 16 periodic interrupts
	UInt32  m_ClockPrevSampMaster;		// Master - Previous clock sample
	UInt32  m_ClockPrevSampSlave;		// Slave - Previous clock sample
	Int  m_ClockEstPpb;					// Estimated clock deviation in ppb

	// State of Genlock reference
	Int m_Genlock_vidstd;				// DTA1XX_GENLOCK_SDI625 / DTA1XX_GENLOCL_SDI525
	Int m_LinesPerFrame;
	Int	m_SamplesPerLine;
	Int m_Genlock;						// 0 = No Lock, 1 = Genlocked
	Int m_GenlockPrev;
	Int m_GraceCnt;
	
	// Device generated reference count
	BOOLEAN m_ValidRefCnt;
	Int64 m_RefCntDelta;
	Int64 m_RefLinesDelta;
	UInt64 m_RefCntprev;	

	// State of the device GENLOCK REFERENCE
	BOOLEAN m_ValidRxSignals;
	UInt32 m_RxLine, m_RxSamp;		// "published" RX line.sample position

	// State of the device GENLOCKED VCO 
	Int m_VcxoCtrl;						// VCXO control value *1000
	Int m_I2cWait;
	Int m_I2cBusy;

	UInt m_TxFramePrev, m_TxLinePrev, m_TxSampPrev;
	Int64 m_TxPosPrev;
	Int m_TxMinRefPrev;					// 27Mhz lock

	Int	m_TxMinRxLinesPrev;				// Rx channel lock
	Int m_TxMinRxPrev; 

} DTA1XX_FDO,*PDTA1XX_FDO, DTA1XX_FDO_EXTENSION, *PDTA1XX_FDO_EXTENSION;

extern DTA1XX_FDO  g_Dta1xxCards[DTA1XX_MAX_CARDS];	// DTA-1xx card data
extern Int  g_NumOfCards;				// Number of DTA-1xx cards found in the system


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
Int  Dta1xxAd9789Write(DTA1XX_FDO* pFdo, Int PortIndex,
                                               Int RegAddr, Int NumToWrite, UInt8* pData);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CrossPlatform.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
NTSTATUS AllocateCdmaBuffer(Channel* pCh, UInt* BufSize);
void FreeCdmaBuffer(Channel* pCh);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dma.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
Int  Dta1xxCdmaCtrl(DTA1XX_FDO* pFdo, DTA1XX_CDMA_CONTROL*);
Int  Dta1xxCdmaStart(Channel* pCh);
Int  Dta1xxCdmaStartSegment(Channel* pCh, UInt SegmentIndex);
Int  Dta1xxCdmaStartSize(IN Channel* pCh);
Int  Dta1xxCdmaStop(Channel* pCh);
Int  Dta1xxCdmaWaitSegment(Channel* pCh, DTA1XX_CDMA_CONTROL*);
Int  Dta1xxCdmaGetBufPos(Channel* pCh, DTA1XX_CDMA_CONTROL*);
Int  Dta1xxDmaKernelMemWrite(Channel* pCh, UInt8*, Int);
Int  Dta1xxDmaUserMemRead(DTA1XX_FDO* pFdo, Int, Int, UInt8*, Int);
Int  Dta1xxDmaUserMemWrite(DTA1XX_FDO* pFdo, Int, Int, UInt8*, Int);
Int  Dta1xxDmaTransfer(Channel* pCh, UInt32, Int, Int);
Int  Dta1xxCdmaSglCreate(Channel* pCh);
Int  Dta1xxCdmaSglCleanUp(Channel* pCh);
#ifdef WINBUILD
VOID  Dta1xxCdmaDpcDma(IN PKDPC, IN PVOID, IN PVOID, IN PVOID);
#else
void  Dta1xxCdmaDpcDma(unsigned long);
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xx.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
int  Dta1xxInitModule(void);
void Dta1xxCleanUpModule(void);
int  Dta1xxStartCard(struct pci_dev* pPdo, DTA1XX_FDO* pFdo);
void Dta1xxStopCard(DTA1XX_FDO* pFdo);
int  Dta1xxOpen(struct inode *inode, struct file *filp);
int  Dta1xxRelease(struct inode *inode, struct file *filp);
int  Dta1xxMmap(struct file *filp, struct vm_area_struct *vma);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DacVga.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
NTSTATUS DacVgaFrequencyResponseCompensation(Channel * pCh);
NTSTATUS DacVgaGetOutputLevel(Channel * pCh, Int * pOutputLevel);
NTSTATUS DacVgaSetOutputLevel(Channel * pCh, Int OutputLevel);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta2111.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void Dta2111Init(Channel * pCh);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EepromControl.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Dta1xxEepromReadLastPage(DTA1XX_FDO* pFdo);
Int  Dta1xxEepromReadWord16(DTA1XX_FDO* pFdo, UInt WordAddr, UInt16* pData);
Int  Dta1xxEepromSpiRead(DTA1XX_FDO* pFdo);
Int  Dta1xxEepromSpiWrite(DTA1XX_FDO* pFdo);
Int  Dta1xxEepromWriteLastPage(DTA1XX_FDO* pFdo);
Int  Dta1xxEepromWriteWord16(DTA1XX_FDO* pFdo, UInt WordAddr, UInt Data);
Int  Dta1xxSetProtectRegister(DTA1XX_FDO* pFdo);
Int  Dta1xxSetProtectRegisterClear(DTA1XX_FDO* pFdo);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Exclusive.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Dta1xxCheckExclusiveAccess(DTA1XX_FDO* pFdo, struct file*);
Int  Dta1xxRequestExclusiveAccess(DTA1XX_FDO* pFdo, Int, struct file*, Int, Int*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Flags.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Dta1xxClearFlags(DTA1XX_FDO* pFdo, Int, Int, Int);
Int  Dta1xxGetFlags(DTA1XX_FDO* pFdo, Int, Int, Int*, Int*);
Int  Dta1xxRxClearLatchedStatusFlags(Channel* pTsRx, Int);
Int  Dta1xxRxProcessStatusFlags(Channel* pTsRx);
Int  Dta1xxTxClearLatchedStatusFlags(Channel* pTsTx, Int);
Int  Dta1xxTxProcessStatusFlags(Channel* pTsTx);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Genlock.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

NTSTATUS  Dta1xxUpdateGenlockTiming(IN PDTA1XX_FDO_EXTENSION  pFdoExt);
NTSTATUS  Dta1xxUpdateGenlockRx(IN PDTA1XX_FDO_EXTENSION  pFdoExt);
NTSTATUS  Dta1xxUpdateGenlockTx(IN PDTA1XX_FDO_EXTENSION  pFdoExt);
NTSTATUS  Dta1xxSetVcxoSpeed(PDTA1XX_FDO_EXTENSION  pFdoExt);
NTSTATUS  Dta1xxGetGenRefState(PDTA1XX_FDO_EXTENSION  pFdoExt, DTA1XX_GENLOCK* pGenlock);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoCtl.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Dta1xxIoCtl(struct inode *inode, struct file *filp, unsigned int cmd,
				 unsigned long arg);
#ifdef CONFIG_COMPAT
long  Dta1xxCompatIoCtl(struct file *filp, unsigned int cmd, unsigned long arg);
#endif
Int  Dta1xxFirmwareReboot(DTA1XX_FDO* pFdo);
Int  Dta1xxI2cReqExclAccess(DTA1XX_FDO* pFdo, struct file*, Int Request, Int* pGranted);
Int  Dta1xxI2cTransferIoctl(DTA1XX_FDO* pFdo, struct file*, DTA1XX_I2C_TRANSFER*);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Dta1xxInitDevice(DTA1XX_FDO* pFdo);
Int  Dta1xxInitDevicePost(DTA1XX_FDO* pFdo);
void  Dta1xxInitPci905X(DTA1XX_FDO* pFdo);
Int  Dta1xxGetIoConfig(DTA1XX_FDO* pFdo, DTA1XX_IOCONFIG*);
Int  Dta1xxGetIoConfig2(DTA1XX_FDO* pFdo, DTA1XX_IOCONFIG2*);
Int  Dta1xxSetIoConfig(DTA1XX_FDO* pFdo, DTA1XX_IOCONFIG*, BOOLEAN);
Int  Dta1xxSetIoConfig2(DTA1XX_FDO* pFdo, DTA1XX_IOCONFIG2*, BOOLEAN);
Int  Dta1xxGetFailsafeState(DTA1XX_FDO* pFdo, DTA1XX_FAILSAFE*);
Int  Dta1xxSetFailsafeState(DTA1XX_FDO* pFdo, DTA1XX_FAILSAFE*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Interrupt.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
unsigned long  Dta1xxIsr(int Irq, void* pContext, struct pt_regs *pRegs);
void  Dta1xxDisableInterrupts(DTA1XX_FDO* pFdo);
void  Dta1xxEnableInterrupts(DTA1XX_FDO* pFdo);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- LowLevel.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int    Dta1xxComputeRollOff(Channel* pCh);
void   Dta1xxTxDeriveModPars(DTA1XX_FDO* pFdo, Int);
char*  Dta1xxChType2Str(Int ChannelType);
char*  Dta1xxModType2Str(Int ModType);
char*  Dta1xxAtscConstel2Str(Int Constel);
char*  Dta1xxDvbtCodeRate2Str(Int CodeRate);
char*  Dta1xxDvbtPar2BwStr(Int ParXtra1);
char*  Dta1xxDvbtPar2ConstStr(Int ParXtra1);
char*  Dta1xxDvbtPar2GuardStr(Int ParXtra1);
char*  Dta1xxDvbtPar2InDepthStr(Int ParXtra1);
char*  Dta1xxDvbtPar2ModeStr(Int ParXtra1);
void  Dta1xxSetModulatorScaler(Channel* pCh);
Int  Dta1xxComputeRollOff(Channel* pCh);
BOOLEAN  Dta1xxIsSoftQam(Channel* pCh);
BOOLEAN  Dta1xxIsHardQamAorC(Channel* pCh);
Int  Dta1xxRxReset(Channel* pTsRx, Int);
Int  Dta1xxRxSetRxMode(DTA1XX_FDO* pFdo, Int, Int);
Int  Dta1xxTxGetModControl(DTA1XX_FDO* pFdo, Int, Int*,	Int*, Int*, Int*);
Int  Dta1xxTxGetTxControl(DTA1XX_FDO* pFdo, Int, Int*);
Int  Dta1xxTxGetTxMode(DTA1XX_FDO* pFdo, Int, Int*, Int*);
Int  Dta1xxTxReset(Channel* pTsTx, Int);
Int  Dta1xxTxSetModControl2(DTA1XX_FDO* pFdo, Int, Int, Int, Int, Int);
Int  Dta1xxTxSetTxControl(DTA1XX_FDO* pFdo, Int, Int);
Int  Dta1xxTxSetTxMode(DTA1XX_FDO* pFdo, Int, Int, Int);
Int  Dta1xxGenSetLed(DTA1XX_FDO* pFdo, Int);
Int  Dta1xxI2cRead(DTA1XX_FDO* pFdo, struct file*, UInt, UInt Length, UInt8*);
Int  Dta1xxI2cWrite(DTA1XX_FDO* pFdo, struct file*, UInt, UInt Length, UInt8*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- MapMemory.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Dta1xxGetAddrOperRegs(DTA1XX_FDO*, struct file*, Int, Int, void**, UInt*, void*);
Int  Dta1xxMapOperRegsToUserSpace(DTA1XX_FDO*, struct file*, struct vm_area_struct*);
NTSTATUS  Dta1xxNwIpRxReleaseBuf(IN PDTA1XX_FDO pFdo,
								 IN UserIpRxChannel* pIpRx);
NTSTATUS  Dta1xxNwIpTxReleaseBuf(IN PDTA1XX_FDO pFdo,
								 IN UserIpTxChannel* pIpTx);
Int  Dta1xxReleaseAddrOperRegs(DTA1XX_FDO* pFdo, struct file*, Int, Int, void**, UInt*);
Int  Dta1xxSharedBufAlloc(DTA1XX_FDO* pFdo, DTA1XX_SHAREDBUF_CTRL*);
Int  Dta1xxSharedBufFree(DTA1XX_FDO* pFdo, DTA1XX_SHAREDBUF_CTRL*);
Int  Dta1xxSharedBufRelease(Channel* pCh, Int, ShBufDesc*, Int);
Int  Dta1xxUnmapOperRegsForFileObject(DTA1XX_FDO*, struct file*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- NwIpRx.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#ifdef WINBUILD
void  Dta1xxNwIpRxBrmPerIntDpc(IN PKDPC, IN PVOID, IN PVOID, IN PVOID  SystemArgument2);
#else
void  Dta1xxNwIpRxBrmPerIntDpc(IN unsigned long DeferredContext);
#endif
NTSTATUS  Dta1xxNwIpRxCreateWorkerThreads(IN PDTA1XX_FDO);
void      Dta1xxNwIpRxFreeRtpBuffers(IN UserIpRxChannel*  pIpRx);
UserIpRxChannel* Dta1xxNwIpRxGetChannel(IN PDTA1XX_FDO, IN Int);
NTSTATUS  Dta1xxNwIpRxInitRtpBuffers(IN UserIpRxChannel*  pIpRx);
NTSTATUS  Dta1xxNwIpRxIoctl(IN PDTA1XX_FDO, IN PFILEOBJECT, IN ULONG InBufferSize,
			 IN ULONG OutBufferSize, IN PDTA1XX_IP_RXTX_CTRL, OUT PDTA1XX_IP_RXTX_RTRN);
void	  Dta1xxNwIpRxPurgeLeftOverChannels(IN PDTA1XX_FDO, IN PFILEOBJECT);
NTSTATUS  Dta1xxNwIpRxStopWorkerThreads(IN PDTA1XX_FDO);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- NwIpTx.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void      Dta1xxNwIpTxCleanUpCdmaBuffer(NwStore* pNwStore);
NTSTATUS  Dta1xxNwIpTxCreateCdmaBuffer(NwStore* pNwStore);
UserIpTxChannel* Dta1xxNwIpTxGetChannel(IN PDTA1XX_FDO, IN Int);
NTSTATUS  Dta1xxNwIpTxIoctl(IN PDTA1XX_FDO, IN PFILEOBJECT, IN ULONG InBufferSize,
			 IN ULONG OutBufferSize, IN PDTA1XX_IP_RXTX_CTRL, OUT PDTA1XX_IP_RXTX_RTRN);
void	  Dta1xxNwIpTxPurgeLeftOverChannels(IN PDTA1XX_FDO, IN PFILEOBJECT);
#ifdef WINBUILD
void   Dta1xxNwIpTxWorkerDpc(IN PKDPC Dpc, IN PVOID DeferredContext, 
							 IN PVOID SystemArgument1, IN PVOID SystemArgument2);
#else
void Dta1xxNwIpTxWorkerDpc(IN unsigned long DeferredContext);
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- NwNdis.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#ifdef WINBUILD
void Dta1xxNwIndicateLinkStatusDpc(IN PKDPC Dpc, IN PVOID DeferredContext, 
								IN PVOID  SystemArgument1, IN PVOID  SystemArgument2);
#else
void Dta1xxNwIndicateLinkStatusDpc(IN unsigned long DeferredContext);
#endif

NTSTATUS  Dta1xxNwNdisIoctlQueryInfo(ULONG InBufferSize, ULONG* OutBufferSize, 
							  PDTA1XX_NWI_INFO pInfo, PDTA1XX_FDO pFdoExt);
NTSTATUS  Dta1xxNwNdisIoctlSetInfo(ULONG InBufferSize, ULONG OutBufferSize, 
							  PDTA1XX_NWI_INFO pInfo, PDTA1XX_FDO pFdoExt);
//NTSTATUS  Dta1xxNwNdisQueueStatusIrp(IN PNwStore pNwStore, IN PIRP pIrp);


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- NwPhyMac.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
UInt32    Dta1xxEthGetCounter(IN PNwStore pNwStore, UInt Register);
UInt32    Dta1xxEthGetSingleCounter(IN PNwStore pNwStore, UInt Register);
void      Dta1xxEthLockCounters(IN PNwStore pNwStore);
void      Dta1xxEthUnLockCounters(IN PNwStore pNwStore);
void	  Dta1xxEthWaitMacReady(IN PNwStore pNwStore);
NTSTATUS  Dta1xxNwGetSpeed(IN PNwStore pNwStore, DTA1XX_NWSPEED* pNwSpeed);
void	  Dta1xxNwMacClearInterrupts(IN PNwStore pNwStore);
void      Dta1xxNwMacDisablePhy(IN PNwStore pNwStore);
NTSTATUS  Dta1xxNwMacGetMacAddress(IN PNwStore pNwStore, DTA1XX_MACADDRESS* pMacAddress);
NTSTATUS  Dta1xxNwMacInitMacAddress(IN PNwStore pNwStore);
UInt8	  Dta1xxNwMacMiiSmCheckState(IN PNwStore pNwStore);
void	  Dta1xxNwMacInitMiiSm(IN PNwStore pNwStore);
void	  Dta1xxNwQueueLinkAutoNegChangeDpc(IN PNwStore pNwStore);
void      Dta1xxNwResetPhyMacPowerup(IN PNwStore pNwStore);
NTSTATUS  Dta1xxNwSetLoopback(IN PNwStore pNwStore, DTA1XX_NWLOOPBACK* pNwLoopback);
NTSTATUS  Dta1xxNwSetSpeed(IN PNwStore pNwStore, DTA1XX_NWSPEED* pNwSpeed);
NTSTATUS  Dta1xxNwGetSetStatCnt(IN PNwStore pNwStore, IN PIRP pIrp, 
								IN DTA1XX_NWSTATCNT* pNwStatCnt, UInt8 Mode);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Rate.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Dta1xxGetFreqEst(DTA1XX_FDO* pFdo, Int, Int, Int*);
Int  Dta1xxGetRxClkFreq(DTA1XX_FDO* pFdo, DTA1XX_PORT_INTPAR* pPortIntPar);
Int  Dta1xxGetTxPhaseIncr(DTA1XX_FDO* pFdo, DTA1XX_PORT_2INTPAR* pPortIntPar);
Int  Dta1xxSetTxPhaseIncr(DTA1XX_FDO* pFdo, DTA1XX_PORT_2INTPAR* pPortIntPar);
/*NTSTATUS  Dta1xxTxGetTsRate(IN PDTA1XX_FDO_EXTENSION pFdoExt, IN Int PortIndex,
							OUT Int* pnClockGenMode, OUT Int* pTsRate);
NTSTATUS  Dta1xxTxSetTsRate(IN PDTA1XX_FDO_EXTENSION pFdoExt, IN Int PortIndex,
							IN Int SetTsRate, IN Int TsRate);*/
Int  Dta1xxTxGetRate2(DTA1XX_FDO* pFdo,	Int, Int*, Int*);
Int  Dta1xxTxSetRate2(DTA1XX_FDO* pFdo, Int, Int, Int);
Int  Dta1xxTxSetRate3(DTA1XX_FDO* pFdo, Int, Int, Int);
Int  Dta1xxGetTxRateSel(Channel*, Int*);
Int  Dta1xxSetTxRateSel(Channel*, Int);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Spi.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
NTSTATUS  Dta1xxSpiGetSpiClksel(Channel* pChannel, Int* pClkSel);
NTSTATUS  Dta1xxSpiGetSpiMode(Channel* pChannel, Int* pMode);
NTSTATUS  Dta1xxSpiGetSpiStd(Channel* pChannel, Int* pStd);
NTSTATUS  Dta1xxSpiGetRxClkFreq(Channel* pChannel, Int* pClkFreq);
NTSTATUS  Dta1xxSpiGetTxRateBps(Channel* pChannel, UInt32* TxRateBps);
NTSTATUS  Dta1xxSpiInit(Channel* pChannel);
NTSTATUS  Dta1xxSpiSetSpiClkSel(Channel* pChannel, Int ClkSel);
NTSTATUS  Dta1xxSpiSetSpiMode(Channel* pChannel, Int Mode);
NTSTATUS  Dta1xxSpiSetSpiStd(Channel* pChannel, Int Std);
NTSTATUS  Dta1xxSpiSetRxModeDvb(Channel* pChannel, Int RxMode);
NTSTATUS  Dta1xxSpiSetSdiClock(Channel* pChannel);
NTSTATUS  Dta1xxSpiSetSpiClock(Channel* pChannel, Int SpiClk);
NTSTATUS  Dta1xxSpiSetTxRateBps(Channel* pChannel, UInt32 TxRateBps);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Target.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
Int  Dta1xxGetTargetId(DTA1XX_FDO* pFdo, Int, Int, Int*, Int*);
void  Dta1xxTargetDetectStateMachine(DTA1XX_FDO* pFdo);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Utility.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
UInt32  Dta1xxGetPeriodicIntIntervalMicroS(DTA1XX_FDO* pFdo);
BOOLEAN  Dta1xxIsHardQamA(Int, Int);
BOOLEAN  Dta1xxIsHardQamAorC2(Int, Int);
BOOLEAN  Dta1xxIsHardQamC(Int, Int);
BOOLEAN  Dta1xxIsIqMode(Int);
BOOLEAN  Dta1xxIsQamB(Int, Int);
BOOLEAN  Dta1xxIsQamMode(Int);
Int  Dta1xxNumBitsPerSymbol(Int);
UInt16  Dta1xxSubsystemId2TypeNumber(Int  SubsystemId);
UInt  Dta1xxTxMode2PacketSize(Int  TxPacketMode);
Int  Dta1xxWaitMs(Int TimeInMs);
Int64  Dta1xxBinDiv(Int64 num, Int64 denom, Int64* pRest);
UInt64  Dta1xxBinUDiv(UInt64 num, UInt64 denom, UInt64* pRest);
Int  Dta1xxReadConfigSpace(DTA1XX_FDO* pFdo, UInt32, UInt32, UInt8*);
Int  Dta1xxWriteConfigSpace(DTA1XX_FDO* pFdo, UInt32, UInt32, UInt8*);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Vpd.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
Int  Dta1xxVpdIoctl(DTA1XX_FDO* pFdo, DTA1XX_VPD_DATA*);
void  Dta1xxVpdInitOffset(DTA1XX_FDO* pFdo);
Int  Dta1xxVpdRead(DTA1XX_FDO* pFdo, UInt8*, UInt, UInt);
Int  Dta1xxVpdReadFromCache(DTA1XX_FDO* pFdo, UInt8*, UInt, UInt);
Int  Dta1xxVpdReadItemRoRw(DTA1XX_FDO* pFdo, char*, char*);
Int  Dta1xxVpdWrite(DTA1XX_FDO* pFdo, UInt8*, UInt, UInt);

#endif //__DTA1XX_H
