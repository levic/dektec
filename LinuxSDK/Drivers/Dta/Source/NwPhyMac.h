//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NwPhyMac.h *#*#*#*#*#*#*#*#* (C) 2010-2015 DekTec
//
// Dta driver - Network Phy and Mac functionality Definitions

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2015 DekTec Digital Video B.V.
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


// Phy types
#define  DTA_PHY_UNKNOWN                0
#define  DTA_PHY_NATIONAL               1
#define  DTA_PHY_MARVELL                2

// Multicast
#define  DTA_MAC_NUM_MULTICAST_LIST     256     // Max number of multicast
                                                // addresses supported

// Phy BUS states
#define  DTA_PHY_BUS_STATE_IDLE         0
#define  DTA_PHY_BUS_STATE_BUSY         1

// Network Admin Status
#define  DTA_NW_ADMIN_STATUS_UP          1
#define  DTA_NW_ADMIN_STATUS_DOWN        2

// Call back functions
typedef struct _PhyMacCallbackFuncs {
    DtFastMutex  m_FastMutex;
    Bool  m_Registered;
    void*  m_pContext;
    LinkStatFunc  m_LinkStatusCallback;
} PhyMacCallbackFuncs;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Setup frame defines (Cast-MAC) -.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtaPFAddr
{   UInt32  m_PhAddr0   :16;                // Physical Address (bit 15..0)
    UInt32  m_Unknown0  :16;
    UInt32  m_PhAddr1   :16;                // Physical Address (bit 31..16)
    UInt32  m_Unknown1  :16;
    UInt32  m_PhAddr2   :16;                // Physical Address (bit 47..32)
    UInt32  m_Unknown2  :16;
} DtaPFAddr;

typedef struct _DtaHFItem
{   UInt32  m_Item0     :16;                 // Hash filter (bit 15..0)*i, i:0..32
    UInt32  m_Unknown0  :16;
} DtaHFItem;

typedef struct _DtaMacCHTable
{   DtaHFItem  m_HFItem[32];                 // Hash filter table
    UInt32  m_Unknown1[7];
    DtaPFAddr  m_PFAddress;                  // Unicast MAC address
    UInt32  m_Unknown2[6];
} DtaHTable;

// Filtering type for Setup frame. (Dmaheader)
#define  DTA_FT_PERFECT     0       // Perfect filtering
#define  DTA_FT_HASH        1       // Hash filtering
#define  DTA_FT_INVERSE     2       // Inverse filtering
#define  DTA_FT_HASH_ONLY   3       // Hash only filerting: 512 bit hash table

// DtaMacCSetupFrame
typedef struct _DtaMacCSetupFrame {
    DtaDmaTxHeader  m_DmaHeader;
    union{
        DtaPFAddr  m_PFAddress[16];
        DtaHTable  m_HTable;
    };
    // DtaPFAddr and DtaHFTable must be of the same size in order to use
    // sizeof(DtaMacCSetupFrame) to return actual used bytes
} DtaMacCSetupFrame;

// Storage for all Phy and Mac data
typedef struct _PhyMac {
    Int  m_IpPortIndex;                 // IP Port index 
    DtaDeviceData*  m_pDvcData;         // Reference back to the device data 
    UInt  m_GenNwRegsOffset;
    UInt16  m_MacRegsOffset;
    volatile UInt8*  m_pGenNwRegs;      // Base address of General network
    volatile UInt8*  m_pMacRegs;        // Base address of MAC registers
    UInt  m_MacType;                    // Type of MAC used in the hardware.
    Bool  m_AlignedPayload;             // True if Payload is aligned and 2 extra 
                                        // dummy bytes are inbetween DMA header and 
                                        // payload

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General Network -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Bool  m_Initialized;                // TRUE if the PHY/MAC is intialized
    UInt8  m_MacAddrPer[6];             // Network MAC address retrieved from VPD
    UInt8  m_MacAddrCur[6];             // Current Network MAC address. Can be changed
                                        // by network driver
    UInt  m_NumMulticasts;              // Number of multicast IP-addresses in list
    UInt  m_MaxMulticasts;              // Maximum number of multicast IP-addresses in list
    UInt8  m_MulticastList[DTA_MAC_NUM_MULTICAST_LIST][6];
                                        // List of multicast addresses
    UInt8  m_NetworkAdminStatus;        // Administrative status of network adapter

    DtFastMutex  m_FastMutexCounters;   // Mutex for protecting the counters in MAC

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Phy/Mac command/status -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    UInt8  m_PhyType;                   // Type of phy used: PHY_NATIONAL/PHY_MARVELL
    UInt8  m_PhyModel;                  // Model of phy used.
    Int  m_PhyBusState;                 // Indicates if the phy bus is available or busy
    Bool  m_SetupframePending;          // If TRUE, a new multicast list must be send
                                        // to the MAC
    Bool  m_LoopbackEnable;             // If TRUE, the phy is set in loopback mode
                                        // The Tx data is routed back to Rx data
    Bool  m_ForceSpeedDtapiEnable;      // If TRUE, m_ForcedSpeed is used for Phy speed
    UInt  m_ForcedSpeedDtapi;           // See m_NwDrvSpeed for possible values

    UInt  m_PhySpeed;                   // Network speed and duplex from PHY.
                                        // DTA_PHY_SPEED_ constants
                                        
    UInt  m_PhyLink;                    // Network link status form PHY.
                                        // DTA_PHY_LINK_ constants
    UInt  m_MacSpeed;                   // Network speed MAC:
                                        // DTA_PHY_SPEED_ constants
    DtDpc  m_PhyIntDpc;                 // DPC to handle phy interrupt
    DtWorkItem  m_PhyIntWorkItem;       // Workitem to handle phy interrupt for DPC
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Network driver command/status -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Bool  m_NwDrvConnected;             // true if network driver is connected to Dta
    UInt  m_NwDrvLink;                  // Link status to Network driver
    UInt  m_NwDrvSpeed;                 // Network  speed/duplex setting:
    UInt  m_NwDrvPacketFilter;          // Network driver packet filter
    UInt  m_MacPacketFilter;            // Current MAC packet filter

    PhyMacCallbackFuncs  m_Callbacks;   // Callback functions to Network driver
} PhyMac;


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaPhyMacInit(struct _DtaIpPort* pIpPort);
DtStatus  DtaPhyMacPowerup(PhyMac* pPhyMac);
DtStatus  DtaPhyMacPowerdown(PhyMac* pPhyMac);
void  DtaMacGetMacAddressCurrent(PhyMac* pPhyMac, UInt8* pAddress);
DtStatus  DtaMacSetMacAddressCurrent(PhyMac* pPhyMac, UInt8* pAddress);
void  DtaMacGetMacAddressPermanent(PhyMac* pPhyMac, UInt8* pAddress);
DtStatus  DtaPhyMacIoctl(DtaDeviceData* pDvcData, DtIoctlObject* pIoctl);
UInt  DtaPhyGetSpeed(PhyMac* pPhyMac, Bool* pForceSpeedDtapiEnable);
DtStatus  DtaPhySetSpeedDuplex(PhyMac* pPhyMac);
DtStatus  DtaPhySetSpeedDtapi(PhyMac* pPhyMac, UInt Speed, Bool ForceSpeedEnable);
DtStatus  DtaMacCBuildSetupFrame(PhyMac* pPhyMac, UInt8* pDst, UInt* pSize);
UInt32  DtaGetPhyRegister(PhyMac* pPhyMac, UInt32 Reg);
void  DtaSetPhyRegister(PhyMac* pPhyMac, UInt32 Reg, UInt32 Data);
DtStatus  DtaMacGetCounter(PhyMac* pPhyMac, UInt CounterId, UInt64* pValue);
