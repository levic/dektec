// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCommon.h *#*#*#*#*#*#*#*#* (C) 2010-2023 DekTec
//
// SDK - Common definitions and types between for DTAPI/DTA/DTPCIE/DTU drivers
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2022 DekTec Digital Video B.V.
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

#ifndef __DT_COMMON_H
#define __DT_COMMON_H

// Forward declarations
typedef struct _DtIoStub  DtIoStub;
typedef struct _DtIoStubIoParams  DtIoStubIoParams;

// Define Int64/UInt64 here to a non-existing identifier. Since all types in this header 
// are used in a binary interface we have to make sure they are compatible between 32-bit 
// and 64-bit builds. For this we have to align every 64-bit value to an 8-byte boundary.
// The Int64A / UInt64A types do this. Make sure Int64/UInt64 are not used.
#define Int64 ERROR_DO_NOT_USE_UNALIGNED_INT64
#define UInt64 ERROR_DO_NOT_USE_UNALIGNED_UINT64

#ifdef WINBUILD

// Driver / DTAPI include files
#ifdef DTAPI
    // Disable warning about the zero sized array extension
    #pragma warning(disable : 4200)

    #include <winioctl.h>
#endif

#endif // WINBUILD

#if (defined(_MSC_VER) && (_MSC_VER >= 1600))
    // Use native version when available.
    #define ASSERT_SIZE(name, size) static_assert(sizeof(name) == size, \
                                                        "Size mismatch on struct " #name);
#else
    // No native static_assert available, define our own.
    #define ASSERT_SIZE(name, size) \
                            extern const int g_StaticAssert[!!(sizeof(name)==(size))*2-1];
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#ifdef WINBUILD

// Custom event - Structure
typedef struct _DtCustomEvent
{
    UInt  m_HwCategory;             // Hardware category
    Int64A  m_SerialNumber;         // Serial number
    UInt  m_EventType;              // Event type
} DtCustomEvent;

// User buffer to driver with port - Structure 
// For windows, the buffer will be set in the output parameter

typedef struct _DtUserBufferPort
{
    Int  m_PortIndex;
} DtUserBufferPort;

// User buffer to driver - Structure 
// For windows, nothing needed. A dummy value is used.
typedef struct _DtUserBuffer
{
    Int  m_Dummy;
} DtUserBuffer;

#else  // LINBUILD

// User buffer to driver with port - Structure 
typedef struct _DtUserBufferPort
{
    Int  m_PortIndex;
    UInt64A  m_BufferAddr;
    Int  m_NumBytes;
} DtUserBufferPort;

// User buffer to driver - Structure 
typedef struct _DtUserBuffer
{
    UInt64A  m_BufferAddr;
    Int  m_NumBytes;
} DtUserBuffer;

// Virtual memory segment size per port for mmap operations.
// Note: Each port has a 256MB segment and the first segment of 256MB is reserved for 
//       device level resources.
// - Device segment: 0x00000000 .. 0x0FFFFFFF
// - Port 1 segment: 0x10000000 .. 0x1FFFFFFF
// - Port 2 segment: 0x20000000 .. 0x2FFFFFFF
// - ...
// - Port 8 segment: 0x80000000 .. 0x8FFFFFFF
#define DT_MMAP_PORT_MEM_SEGMENT_SIZE    (256ull*1024*1024)

#endif  // WINBUILD


// Type to store the scope that can request a property
typedef enum _DtPropertyScope
{
    PROPERTY_SCOPE_DRIVER = 1,
    PROPERTY_SCOPE_DTAPI = 2,
} DtPropertyScope;

// Type to store the value of a propery
typedef UInt64A  DtPropertyValue;

// Define for not using DtPropertyValue because of PROPERTY_VALUE_TYPE_STRING
#define DT_PVE 0xffffffffffffffffl

// Type to store the original type of a property
typedef enum _DtPropertyValueType
{
    PROPERTY_VALUE_TYPE_BOOL = 0,
    PROPERTY_VALUE_TYPE_INT,
    PROPERTY_VALUE_TYPE_INT8,
    PROPERTY_VALUE_TYPE_INT16,
    PROPERTY_VALUE_TYPE_INT32,
    PROPERTY_VALUE_TYPE_INT64,
    PROPERTY_VALUE_TYPE_UINT,
    PROPERTY_VALUE_TYPE_UINT8,
    PROPERTY_VALUE_TYPE_UINT16,
    PROPERTY_VALUE_TYPE_UINT32,
    PROPERTY_VALUE_TYPE_UINT64,
    PROPERTY_VALUE_TYPE_STRING,
} DtPropertyValueType;

// Table entry
typedef struct _DtTableEntry
{
    Int64A  m_X;
    Int64A  m_Y;
} DtTableEntry;
ASSERT_SIZE(DtTableEntry, 16)

// DtAvFrameProps describes the properties of a Audio/Video frame (e.g. #lines, field 
// start/end, etc)
typedef struct _DtAvFrameProps
{
    Int  m_VidStd;              // Video standard (DT_VIDSTD_XXX)
    Int  m_Fps;                 // Frames per second. NOTE: div by 1.001 for fractional
    UInt8  m_IsHd;              // Is an HD format
    UInt8  m_IsInterlaced;      // Is an interlaced format
    UInt8  m_IsFractional;      // Is a fraction format
    Int  m_NumLines;            // # of lines
       
    // Field 1: Start/end lines
    Int  m_Field1Start, m_Field1End; 
    // Field 1: Start/end line active video
    Int  m_Field1ActVidStart, m_Field1ActVidEnd;

    // Field 2: Start/end lines
    Int  m_Field2Start, m_Field2End;  
    // Field 2: Start/end line active video
    Int  m_Field2ActVidStart, m_Field2ActVidEnd; 

    Int  m_ActVidNumS;          // # of symbols in active video part of line
    Int  m_VancNumS;            // # of symbols in vanc part of line
    Int  m_HancNumS;            // # of symbols in hanc part of line
    Int  m_SavNumS, m_EavNumS;  // # of symbols for EAV and SAV

    Int  m_SyncPointPixelOff;   // # of pixels offset to timing sync point

    Int  m_SwitchingLines[2];   // Switching lines for field 1 & 2
} DtAvFrameProps;
ASSERT_SIZE(DtAvFrameProps, 80)

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtTodTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// A timestamp from the Time-of-Day clock
typedef struct _DtTodTime
{
    UInt32  m_Seconds;          // Integer number of seconds part of the TOD time
    UInt32  m_Nanoseconds;      // Number of nanoseconds part of the TOD time
}  DtTodTime;
ASSERT_SIZE(DtTodTime, 8);


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// DMA priority
#define DMA_PRIO_ROTATING           0
#define DMA_PRIO_CHAN0              1
#define DMA_PRIO_CHAN1              2

#ifdef WINBUILD
// Custom event - GUID - {0578D909-54FB-47fa-AF94-2521A25C566C}
static const GUID  DT_CUSTOM_EVENT_GUID = { 0x578d909, 0x54fb, 0x47fa,
                                     { 0xaf, 0x94, 0x25, 0x21, 0xa2, 0x5c, 0x56, 0x6c } };
#endif

// Custom event - Types
#define DT_CUST_EVENT_TYPE_ADD      1
#define DT_CUST_EVENT_TYPE_REMOVE   2

// Custom event - Hardware category
#define DT_CUST_EVENT_CAT_PCI       1
#define DT_CUST_EVENT_CAT_USB       2
#define DT_CUST_EVENT_CAT_DTE       3

// Shared buffer
#define DT_SH_BUF_PURPOSE_GEN       0            // Generic shared buffer
#define DT_SH_BUF_PURPOSE_DMA       1            // High-performance shared DMA buffer

// Channel types
#define DT_SH_CHANTYPE_UNDEFINED    0            // Undefined IP channel
#define DT_SH_CHANTYPE_IPTX         1            // Transmit IP channel
#define DT_SH_CHANTYPE_IPRX         2            // Receive IP channel
#define DT_SH_CHANTYPE_IPRX_V2      3            // Receive IP channel: Dynamic RX buffers

// Manufacturing plants
#define MANUF_PLANT_DEKTEC          1            // Produced by DekTec
#define MANUF_PLANT_KCS             3            // Produced by KCS
#define MANUF_PLANT_ANUBISS         4            // Produced by Anubiss
#define MANUF_PLANT_PRODRIVE        5            // Produced by Prodrive
#define MANUF_PLANT_SENCORE         6            // Produced by Sencore
#define MANUF_PLANT_ELECTROWATT     7            // Produced by Electro-Watt

// Constants used for CPLD and firmware programming
#define PROG_ITF_JTAG_MASK          0x000F
#define PROG_ITF_SPI_MASK           0x00F0
#define PROG_ITF_NOT_SUPP           0x0000
#define PROG_ITF_JTAG_OLD           0x0001       // (JTAG using Rx/Tx regs)
#define PROG_ITF_JTAG_NORMAL        0x0002       // JTAG using gen regs
#define PROG_ITF_JTAG_FAST          0x0004       // JTAG using optimised CLK/DATA
#define PROG_ITF_SPI_EPCS1          0x0010       // SPI with EPCS1 device
#define PROG_ITF_SPI_EPCS4          0x0020       // SPI with EPCS4 device
#define PROG_ITF_SPI_EPCS16         0x0040       // SPI with EPCS16 device
#define PROG_ITF_SPI_EPCS64         0x0080       // SPI with EPCS64 device
#define PROG_ITF_CFI_FAST           0x0100       // CFI with EPCSx-like interface device
#define PROG_ITF_SPI_MF             0x0200       // SPI master controller

#define ASI_SDI_DESER_ITF_NONE       0           // No ASI/SDI deserialiser used
#define ASI_SDI_DESER_ITF_HOTLINK    1           // Use Cypress hotlink
#define ASI_SDI_DESER_ITF_FPGA_BASED 2           // Default DekTec FPGA based 
                                                 // implementation
#define ASI_SDI_DESER_ITF_GS9060     3           // Uses Gennum GS9060
#define ASI_SDI_DESER_ITF_GS2961     4           // Uses Gennum GS2961
#define ASI_SDI_DESER_ITF_FPGA_LMH0387  5        // FPGA deserialiser + LMH0387 equaliser
#define ASI_SDI_DESER_ITF_FPGA_GS3490  6         // FPGA deserialiser + GS3490 equaliser
#define ASI_SDI_DESER_ITF_FPGA_M23544G 7         // FPGA deserialiser + M23544G equaliser

#define ASI_SDI_SER_ITF_NONE         0           // No ASI/SDI serialiser used
#define ASI_SDI_SER_ITF_HOTLINK      1           // Use Cypress hotlink
#define ASI_SDI_SER_ITF_FPGA_BASED   2           // Default DekTec FPGA based 
                                                 // implementation
#define ASI_SDI_SER_ITF_GS2962       4           // Uses Gennum GS2962
#define ASI_SDI_SER_ITF_FPGA_LMH0387 5           // FPGA serialiser + LMH0387 line driver
#define ASI_SDI_SER_ITF_FPGA_GS3490 6            // FPGA serialiser + GS3490 line driver
#define ASI_SDI_SER_ITF_FPGA_M23145_23528 7      // FPGA serialiser + M23145G reclocker +
                                                 // MACD23145 line driver

// SI534X clock architecture
#define SI534X_DTA2127_LIKE     0               // Like DTA-2127
#define SI534X_DTA2172_LIKE     1               // Like DTA-2172/2174/2178
#define SI534X_DTA2175_LIKE     2               // Like DTA-2175
#define SI534X_DTA2131B_LIKE    3               // Like DTA-2131B

// HD-Channel Status register: detected video standard values 
#define  DT_VIDSTD_UNKNOWN              0x0000
#define  DT_VIDSTD_525I59_94            0x01D6
#define  DT_VIDSTD_625I50               0x0158
#define  DT_VIDSTD_720P23_98            0x0188
#define  DT_VIDSTD_720P24               0x0108
#define  DT_VIDSTD_720P25               0x0106
#define  DT_VIDSTD_720P29_97            0x0182
#define  DT_VIDSTD_720P30               0x0102
#define  DT_VIDSTD_720P50               0x0104
#define  DT_VIDSTD_720P59_94            0x0180
#define  DT_VIDSTD_720P60               0x0100
#define  DT_VIDSTD_1080P23_98           0x0190
#define  DT_VIDSTD_1080P24              0x0110
#define  DT_VIDSTD_1080P25              0x010D
#define  DT_VIDSTD_1080P29_97           0x018B
#define  DT_VIDSTD_1080P30              0x010B
#define  DT_VIDSTD_1080I50              0x014C
#define  DT_VIDSTD_1080I59_94           0x01CA
#define  DT_VIDSTD_1080I60              0x014A
#define  DT_VIDSTD_1080P50              0x010C
#define  DT_VIDSTD_1080P59_94           0x018A
#define  DT_VIDSTD_1080P60              0x010A
#define  DT_VIDSTD_1080P50B             0x030C
#define  DT_VIDSTD_1080P59_94B          0x038A
#define  DT_VIDSTD_1080P60B             0x030A
#define  DT_VIDSTD_1080PSF23_98         0x0001
#define  DT_VIDSTD_1080PSF24            0x0002
#define  DT_VIDSTD_1080PSF25            0x0003
#define  DT_VIDSTD_1080PSF29_97         0x0004
#define  DT_VIDSTD_1080PSF30            0x0005
#define  DT_VIDSTD_480P59_94            0x0006
#define  DT_VIDSTD_525P59_94            0x0007
#define  DT_VIDSTD_625P50               0x0008
// 6G 2160 formats
#define  DT_VIDSTD_2160P23_98           0x1001
#define  DT_VIDSTD_2160P24              0x1002
#define  DT_VIDSTD_2160P25              0x1003
#define  DT_VIDSTD_2160P29_97           0x1004
#define  DT_VIDSTD_2160P30              0x1005
// 12G 2160 formats
#define  DT_VIDSTD_2160P50              0x1006
#define  DT_VIDSTD_2160P59_94           0x1007
#define  DT_VIDSTD_2160P60              0x1008
#define  DT_VIDSTD_2160P50B             0x1009
#define  DT_VIDSTD_2160P59_94B          0x100A
#define  DT_VIDSTD_2160P60B             0x100B

#define  DT_VIDSTD_TS                   -1      // Special case

// Genlock: architecture
#define GENLOCK_ARCH_NONE           0            // No genlock logic present
#define GENLOCK_ARCH_145            1            // FPGA based arch, like a DTA-145
#define GENLOCK_ARCH_2144           2            // FPGA based arch, like a DTA-2144
#define GENLOCK_ARCH_2152           3            // LMH1982 based arch, like a DTA-2152
#define GENLOCK_ARCH_2154           4            // LMH1983 based arch, like a DTA-2154

// Genlock: operation mode for internal clock source
#define GENLOCK_OPMODE_INTSRC_UNDEF     0        // Undefined operation mode
#define GENLOCK_OPMODE_INTSRC_FREE_RUN  1        // Operate in free runnig mode
#define GENLOCK_OPMODE_INTSRC_AFD       2        // Operate in auto format detect mode

// Fan controller types
#define FAN_TYPE_MAX6639             0           // Maxim 6639 controller
#define FAN_TYPE_FANM                1           // FANM firmware block
#define FAN_TYPE_FANC                2           // FANC firmware block 

// Temperature sensor types
#define TEMP_SENS_TYPE_MAX6639       0           // Maxim 6639 controller
#define TEMP_SENS_TYPE_FX3           1           // FX3 controller

// SDI-rates
#define DT_DRV_SDIRATE_UNKNOWN  -1  // Unknown
#define DT_DRV_SDIRATE_SD       0   // SD SDI (and ASI)
#define DT_DRV_SDIRATE_HD       1   // HD SDI
#define DT_DRV_SDIRATE_3G       2   // 3G SDI
#define DT_DRV_SDIRATE_6G       3   // 6G SDI
#define DT_DRV_SDIRATE_12G      4   // 12G SDI

// PacketStream data types
#define DT_PACKETSTREAM_DATATYPE_UNSEPEC     0 // Unspecified
#define DT_PACKETSTREAM_DATATYPE_CONTINUOUS  1 // Continuous data
#define DT_PACKETSTREAM_DATATYPE_TS          2 // Transport stream packets
#define DT_PACKETSTREAM_DATATYPE_SI2166GSE   3 // Si2166 GSE-packet data

// PCIe bridges
#define PCIE_BRIDGE_TYPE_PEX87XX     0           // PLX PEX 87XX chip
#define PCIE_BRIDGE_TYPE_PEX811X     1           // PLX PEX 8711/8712 chip

// RX register block type
#define ARCHITECTURE_RX_LEGACY             0     // Legacy RX channel
#define ARCHITECTURE_RX_LEGACY_MATRIX      1     // Legacy matrix RX channel
#define ARCHITECTURE_RX_LEGACY_TS_MEMLESS  2     // Legacy memory-less TS RX channel
#define ARCHITECTURE_RX_LEGACY_USB_SDI_MEMLESS 3 // Legacy RX channel as on DTU-351


// TX register block type
#define ARCHITECTURE_TX_LEGACY             0     // Legacy TX channel
#define ARCHITECTURE_TX_LEGACY_MATRIX      1     // Legacy matrix TX channel
#define ARCHITECTURE_TX_LEGACY_USB_315     2     // Legacy USB TX channel as on DTU-315

// Properties
#define PROPERTY_NAME_MAX_SIZE      50
#define PROPERTY_STR_MAX_SIZE       96

// MAC types
#define  MAC_TYPE_CAST              0
#define  MAC_TYPE_ALTERA            1

// UUID defines
#define  DT_UUID_INDEX_MASK     0x0000FFFF      // Last 16-bits of UUID contains the index
#define  DT_UUID_BC_FLAG        0x00010000      // If set the UUID is a BC ID
#define  DT_UUID_AF_FLAG        0x00020000      // If set the UUID is a AF ID
#define  DT_UUID_DF_FLAG        0x00040000      // If set the UUID is a DF ID
#define  DT_UUID_CORE_FLAG      0x00080000      // If set the UUID is for a core component
#define  DT_UUID_FLAG_MASK      0x000F0000      // Mask for flags
#define  DT_UUID_CORE           0x00000000      // '0' is reserved for the core function

// MACRO to generate a sequence number for a local block/function type
#define  LTYPE_SEQNUM(Type, Index)   ((Type*100) + Index)

// Maximum length for the friendly name of a channel function
#define DT_CHAN_FRIENDLY_NAME_MAX_LENGTH        64

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Block-controller types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: to maintain backwards compatibility, new types shall be added to the end only!!
typedef enum  _DtBcType
{
    DT_BLOCK_TYPE_UNKNOWN=0,
    DT_BLOCK_TYPE_ASIRXP,       // AsiRxProtocol
    DT_BLOCK_TYPE_ASITXG,       // AsiTxGate
    DT_BLOCK_TYPE_ASITXSER,     // AsiTxSer
    DT_BLOCK_TYPE_CDMAC,        // DmacContPcie
    DT_BLOCK_TYPE_FANC,         // FanCtrl
    DT_BLOCK_TYPE_FPGATEMP,     // FpgaTemperature
    DT_BLOCK_TYPE_GS2988,       // Gs2988Ctrl
    DT_BLOCK_TYPE_I2CM,         // I2cMaster
    DT_BLOCK_TYPE_IPSECG,       // IpSecurityGuard
    DT_BLOCK_TYPE_KA,           // KeepAlive
    DT_BLOCK_TYPE_LEDB,         // LedBoard
    DT_BLOCK_TYPE_MSIX,         // MsixController
    DT_BLOCK_TYPE_REBOOT,       // FpgaReboot
    DT_BLOCK_TYPE_SDIRXF,       // SdiRxFmtSimple
    DT_BLOCK_TYPE_SDIRXP,       // SdiRxProtocol
    DT_BLOCK_TYPE_SDIRXPHY,     // SdiRxPhy
    DT_BLOCK_TYPE_SDITXF,       // SdiTxFmtSimple
    DT_BLOCK_TYPE_SDITXP,       // SdiTxProtocol
    DT_BLOCK_TYPE_SDITXPHY,     // SdiTxPhy
    DT_BLOCK_TYPE_SDITXPLL,     // SdiTxPll
    DT_BLOCK_TYPE_SDIXCFG,      // SdiXcvrReconfig
    DT_BLOCK_TYPE_SPIM,         // SpiMaster
    DT_BLOCK_TYPE_SPIMC,        // SpiMasterCtrl
    DT_BLOCK_TYPE_SPIMF,        // SpiMasterFlash
    DT_BLOCK_TYPE_SWITCH,       // Switch
    DT_BLOCK_TYPE_TOD,          // TimeOfDay
    DT_BLOCK_TYPE_TSRXFMT,      // TsRxFmtTrp
    DT_BLOCK_TYPE_VVI,          // VitalVersionInfo
    DT_BLOCK_TYPE_CONSTSINK,    // ConstRateSink
    DT_BLOCK_TYPE_CONSTSOURCE,  // ConstRateSource
    DT_BLOCK_TYPE_ACCUFIFO,     // AccuFifo
    DT_BLOCK_TYPE_BURSTFIFO,    // BurstFifo
    DT_BLOCK_TYPE_LMH1981,      // Lmh1981Ctrl
    DT_BLOCK_TYPE_GENL,         // GenLock
    DT_BLOCK_TYPE_SDIDMX12G,    // SdiDemux12G
    DT_BLOCK_TYPE_SDIMUX12G,    // SdiMux12G
    DT_BLOCK_TYPE_ST425LR,      // St425LinkReorder
    DT_BLOCK_TYPE_S12GTO3G,     // Sdi12Gto3G
    DT_BLOCK_TYPE_IOSERIN,      // IoSerialInput
    DT_BLOCK_TYPE_SI2166ITF,    // Si2166Itf
    DT_BLOCK_TYPE_EMAC10G,      // EthMac10G
    DT_BLOCK_TYPE_IPSCHED,      // IP schedular
    DT_BLOCK_TYPE_IPSUMCHK,     // IPChecksumChecker
    DT_BLOCK_TYPE_IPSUMINS,     // IPChecksumInserter
    DT_BLOCK_TYPE_IPROUT,       // IpRouter
    DT_BLOCK_TYPE_DDRFRONT,     // DdrFrontEnd
    DT_BLOCK_TYPE_IPFIFO,       // IpDdrFifo
    DT_BLOCK_TYPE_CDMACTO,      // DmacContPcieTimeout
    DT_BLOCK_TYPE_XPLL,         // XcvrPll
    DT_BLOCK_TYPE_DISEQC,       // Diseqc
    DT_BLOCK_TYPE_DATACDC,      // DataCdc
    DT_BLOCK_TYPE_DDRFIFO,      // DdrFifo
    DT_BLOCK_TYPE_ETHIPPAD,     // EthIpPad
    DT_BLOCK_TYPE_GPSTIME,      // GpsTime
    DT_BLOCK_TYPE_IQUNPCK,      // IqUnpacker
    DT_BLOCK_TYPE_IQFIR,        // IqFirFilter
    DT_BLOCK_TYPE_IQNOISE,      // IqNoiseGenerator
    DT_BLOCK_TYPE_IQMISC,       // IqMiscProc
    DT_BLOCK_TYPE_IQSYNC,       // IqSyncGate
    DT_BLOCK_TYPE_IQUP,         // IqUpSampler
    DT_BLOCK_TYPE_IQJESD,       // IqJesdDacItf
    DT_BLOCK_TYPE_DATAFIFO,     // DataDdrFifo
    DT_BLOCK_TYPE_DATAC,        // DataCoupler
    DT_BLOCK_TYPE_IOPARIN,      // IoParallelIn
    DT_BLOCK_TYPE_IQAGC,        // IqAgc
    DT_BLOCK_TYPE_IQDEMOD,      // IqDemodulator
    DT_BLOCK_TYPE_IQDOWNSRC,    // IqDownsampler
    DT_BLOCK_TYPE_IQFIR2XCLK,   // IqFirFilter 2x clock
    DT_BLOCK_TYPE_IQINV,        // IqInv
    DT_BLOCK_TYPE_IQPWR,        // IqPowerEstimator
    DT_BLOCK_TYPE_AD9789ITF,    // Ad9789Itf


    // Local DTA-2132 blocks. DONOT RENUMBER!!
    DT_BLOCK_TYPE_AD5320_2132   = LTYPE_SEQNUM(2132, 1),
    DT_BLOCK_TYPE_ADS4246_2132  = LTYPE_SEQNUM(2132, 2),
    DT_BLOCK_TYPE_LNBH25_2132   = LTYPE_SEQNUM(2132, 3),
    DT_BLOCK_TYPE_S2STATS_2132  = LTYPE_SEQNUM(2132, 4),
    DT_BLOCK_TYPE_IQFIR_2132    = LTYPE_SEQNUM(2132, 5),
    DT_BLOCK_TYPE_IQCGRAB_2132  = LTYPE_SEQNUM(2132, 6),
    DT_BLOCK_TYPE_IQPWR_2132    = LTYPE_SEQNUM(2132, 7),
    DT_BLOCK_TYPE_S2DEC_2132    = LTYPE_SEQNUM(2132, 8),
    DT_BLOCK_TYPE_S2DEMOD_2132  = LTYPE_SEQNUM(2132, 9),
    DT_BLOCK_TYPE_IQSRC2132_2132 = LTYPE_SEQNUM(2132, 10),
    DT_BLOCK_TYPE_DATASTMUX_2132 = LTYPE_SEQNUM(2132, 11),

    // Local DTA-2116 blocks, DONOT renumber!!
    DT_BLOCK_TYPE_ATTNCTRL_2116 = LTYPE_SEQNUM(2116, 1),
    DT_BLOCK_TYPE_IO_2116       = LTYPE_SEQNUM(2116, 2),
}  DtBcType;

// Block operational modes
typedef enum _DtBlockOpMode
{
    DT_BLOCK_OPMODE_IDLE = 0,       // Idle
    DT_BLOCK_OPMODE_STANDBY = 1,    // Standby
    DT_BLOCK_OPMODE_RUN = 2,        // Run
} DtBlockOpMode;

// Block operational status
typedef enum _DtBlockOpStatus
{
    DT_BLOCK_OPSTATUS_IDLE = 0,     // Idle
    DT_BLOCK_OPSTATUS_STANDBY = 1,  // Standby
    DT_BLOCK_OPSTATUS_RUN = 2,      // Run
    DT_BLOCK_OPSTATUS_ERROR = 3     // Error state
} DtBlockOpStatus; 

// Function operational modes
typedef enum _DtFuncOpMode
{
    DT_FUNC_OPMODE_UNDEF = -1,     // Undefined
    DT_FUNC_OPMODE_IDLE = 0,        // Idle
    DT_FUNC_OPMODE_STANDBY = 1,     // Standby
    DT_FUNC_OPMODE_RUN = 2,         // Run
} DtFuncOpMode;
// Function operational status
typedef enum _DtFuncOpStatus
{
    DT_FUNC_OPSTATUS_IDLE = 0,      // Idle
    DT_FUNC_OPSTATUS_STANDBY = 1,   // Standby
    DT_FUNC_OPSTATUS_RUN = 2,       // Run
    DT_FUNC_OPSTATUS_ERROR = 3,     // Error state
} DtFuncOpStatus;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Driver/API function types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: to maintain backwards compatibility, new types shall be added to the end only!!
typedef enum  _DtFunctionType
{
    DT_FUNC_TYPE_UNKNOWN = 0,
    DT_FUNC_TYPE_CF_EVT,
    DT_FUNC_TYPE_CF_INT,
    DT_FUNC_TYPE_CF_IOCFG,
    DT_FUNC_TYPE_CF_PROPS,
    DT_FUNC_TYPE_CF_SETTINGS,
    DT_FUNC_TYPE_CF_TOD,
    DT_FUNC_TYPE_CORE,
    DT_FUNC_TYPE_ASIRX,
    DT_FUNC_TYPE_ASISDIRX,
    DT_FUNC_TYPE_ASISDITX,
    DT_FUNC_TYPE_DEKTECSTPROBE,
    DT_FUNC_TYPE_DMA,
    DT_FUNC_TYPE_DMARXTXTEST_NOT_USED_ANYMORE,
    DT_FUNC_TYPE_HDMI20BTX,
    DT_FUNC_TYPE_IPSECG,
    DT_FUNC_TYPE_LEDBOARD,
    DT_FUNC_TYPE_MXDS75TEMP,
    DT_FUNC_TYPE_REBOOT,
    DT_FUNC_TYPE_SDIRX,
    DT_FUNC_TYPE_SDITXPHY,
    DT_FUNC_TYPE_SDIXCFGMGR,
    DT_FUNC_TYPE_SENSTEMP,
    DT_FUNC_TYPE_SI534X,
    DT_FUNC_TYPE_SPIMF,
    DT_FUNC_TYPE_SPIPROM,
    DT_FUNC_TYPE_TEMPFANMGR,        // TempFanMgr driver function
    DT_FUNC_TYPE_TEMPFANMGR_AF,     // TempFanMGr API function
    DT_FUNC_TYPE_TSTRPRX,
    DT_FUNC_TYPE_VPD,
    DT_FUNC_TYPE_DMAPERFACCU,
    DT_FUNC_TYPE_SPICABLEDRVEQ,     // Cable driver/equalizer driver function
    DT_FUNC_TYPE_GENLOCKCTRL,       // Genlock control driver function
    DT_FUNC_TYPE_GENLOCKCTRL_AF,    // Genlock control API function
    DT_FUNC_TYPE_SDIGENREF,         // SDI genref API function
    DT_FUNC_TYPE_VIRTGENREF,        // SDI virtual genref driver function
    DT_FUNC_TYPE_KEEPALIVE,         // Keep alive control API function
    DT_FUNC_TYPE_ASISDIMON,         // ASI/SDI monitor control API function
    DT_FUNC_TYPE_SDIPHYONLYRX,      // SDI PHY-only receive-channel
    DT_FUNC_TYPE_SDIPHYONLYTX,      // SDI PHY-only transmit-channel
    DT_FUNC_TYPE_I2CM,
    DT_FUNC_TYPE_SPIM,
    DT_FUNC_TYPE_TXPLLMGR,          // TX-PLL manager
    DT_FUNC_TYPE_TODCLKCTRL,        // TOD clock control
    DT_FUNC_TYPE_TODCLKCTRL_AF,     // TOD clock control API function
    DT_FUNC_TYPE_SFP,
    DT_FUNC_TYPE_NW,
    DT_FUNC_TYPE_NW_AF,
    DT_FUNC_TYPE_IPROUTER,
    DT_FUNC_TYPE_DDRFRONT,
    DT_FUNC_TYPE_IPFIFO,
    DT_FUNC_TYPE_IQMODCHAN,         // IQ modulator channel
    DT_FUNC_TYPE_SFNCTRL,           // SFN control
    DT_FUNC_TYPE_GPSTIME,           // GPS time
    DT_FUNC_TYPE_DATAFIFO,
    DT_FUNC_TYPE_CHSDIRX,         // SDI Receive Channel
    DT_FUNC_TYPE_CHSDIRXPHYONLY,  // SDI PHY-Only Receive Channel
    DT_FUNC_TYPE_MCTC72TEMP,
    DT_FUNC_TYPE_IQDEMODRX,         // IQ-demodulation receive channel (API-function)
    DT_FUNC_TYPE_SDRFRONTEND,       // SDR-Front-end (API-function)
    DT_FUNC_TYPE_ADF4360CTRL,       // ADF4360 Control

    // Local DTA-2110 functions. DO NOT RENUMBER!!
    DT_FUNC_TYPE_CLKGEN_2110 = LTYPE_SEQNUM(2110, 1),

    // Local DTA-2127 functions. DONOT RENUMBER!!
    DT_FUNC_TYPE_DEMODCHANNEL_2127 = LTYPE_SEQNUM(2127, 1),
    DT_FUNC_TYPE_CIDCHANNEL_2127 = LTYPE_SEQNUM(2127, 2),
    DT_FUNC_TYPE_DISEQCCONTROL_2127 = LTYPE_SEQNUM(2127, 3),
    
    // Local DTA-2132 functions. DONOT RENUMBER!!
    DT_FUNC_TYPE_SPIM_2132 = LTYPE_SEQNUM(2132, 1),
    DT_FUNC_TYPE_I2CM_2132 = LTYPE_SEQNUM(2132, 2),
    DT_FUNC_TYPE_AD5320_2132 = LTYPE_SEQNUM(2132, 4),
    DT_FUNC_TYPE_ADS4246_2132 = LTYPE_SEQNUM(2132, 5),
    DT_FUNC_TYPE_S2STATS_2132 = LTYPE_SEQNUM(2132, 6),
    DT_FUNC_TYPE_S2CRDEMOD_2132 = LTYPE_SEQNUM(2132, 7),    // S2CRDEMOD driver function
    DT_FUNC_TYPE_AFS2CRDEMOD_2132 = LTYPE_SEQNUM(2132, 8),  // S2CRDEMOD api function
    DT_FUNC_TYPE_IQFIR_2132 = LTYPE_SEQNUM(2132, 9),
    DT_FUNC_TYPE_DATASTMUX_2132 = LTYPE_SEQNUM(2132, 10),
    DT_FUNC_TYPE_IQCGRAB_2132 = LTYPE_SEQNUM(2132, 11),
    DT_FUNC_TYPE_IQPWR_2132 = LTYPE_SEQNUM(2132, 12),
    DT_FUNC_TYPE_IQSRC2132_2132 = LTYPE_SEQNUM(2132, 13),
    DT_FUNC_TYPE_LNBH25_2132 = LTYPE_SEQNUM(2132, 14),

    // Local DTA-2139 functions. DONOT RENUMBER!!
    DT_FUNC_TYPE_I2CM_2139 = LTYPE_SEQNUM(2139, 1),

    // Local DTA-2178(A) functions. DONOT RENUMBER!!
    DT_FUNC_TYPE_TXCLKCTRL_2178 = LTYPE_SEQNUM(2178, 1),

    // Local DTA-2116 functions. DONOT RENUMBER!!
    DT_FUNC_TYPE_ATTNCTRL_2116 = LTYPE_SEQNUM(2116, 1),
    DT_FUNC_TYPE_IO_2116 = LTYPE_SEQNUM(2116, 2),
    DT_FUNC_TYPE_CLKCTRL_2116 = LTYPE_SEQNUM(2116, 3),

}  DtFunctionType;
// Sub-function and functions are the same
typedef DtFunctionType  DtSubFunctionType;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Port types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: to maintain backwards compatibility, new types shall be added to the end only!!
typedef enum  _DtPortType
{
    DT_PORT_TYPE_UNKNOWN=0,
    DT_PORT_TYPE_BASE,              // Base implementation
    DT_PORT_TYPE_ASISDIRX,          // ASI/SDI receiver port
    DT_PORT_TYPE_ASISDIRXTX,        // ASI/SDI receiver/transmitter port
    DT_PORT_TYPE_ASISDITX,          // ASI/SDI transmitter port
    DT_PORT_TYPE_SDIGENREF,         // SDI genref port
    DT_PORT_TYPE_ASISDIMON,         // ASI/SDI monitor port
    DT_PORT_TYPE_SDIPHYONLYRX,      // SDI PHY-only receive port
    DT_PORT_TYPE_SDIPHYONLYTX,      // SDI PHY-only transmit port
    DT_PORT_TYPE_SDIPHYONLYRXTX,    // SDI PHY-only receive/transmit port
    DT_PORT_TYPE_IP,                // IP port
    DT_PORT_TYPE_MODOUTP,           // Modulator output port
}  DtPortType;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Port data types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: to maintain backwards compatibility, new types shall be added to the end only!!
typedef enum  _DtPortDataType
{
    DT_PORT_DATATYPE_UNDEF=0,       // Not defined (must be deduced through other ways)
    DT_PORT_DATATYPE_IQSAMPLES,     // Port consumes/produces a stream of IQ samples
    DT_PORT_DATATYPE_L3FRAMES,      // Port consumes/produces a stream of L3 frames
    DT_PORT_DATATYPE_PACKETSTREAM,  // Port consumes/produces a packet stream
    DT_PORT_DATATYPE_SDIRXFMTSIMPLE,  // Port produces a SdiRxFmtSimple stream
    DT_PORT_DATATYPE_SDITXFMTSIMPLE,  // Port consumes a SdiTxFmtSimple stream
    DT_PORT_DATATYPE_TRANSPARENTPACKETS,  // Port consumes/produces transparent packets
}  DtPortDataType;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Common IOCTL codes +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// On Windows:
// METHOD_OUT_DIRECT is used for 'normal' IOCTLS, METHOD_IN_DIRECT is not needed for
//                   'normal' IOCTLS because output buffers are never used as input
// METHOD_IN_DIRECT and METHOD_OUT_DIRECT are used for DMA's
// METHOD_IN_DIRECT is used for shared buffers. The output buffer can then be used as 
//                  input and output
// METHOD_NEITHER   is not used at all
//

// Constants
#ifdef WINBUILD
    // Windows specific defines
    #define DT_DEVICE_TYPE  45000  // DeviceType for DeviceIoControl operations
    #define DT_IOCTL_BASE   3000   // Offset for Function
#else
    // Linux specific defines
    #define DT_IOCTL_MAGIC         0xCC   // Magic number
    #define DT_IOCTL_MAGIC_SIZE    0xCD   // Magic number for Ioctls with a dynamic size
    #define DT_IOCTL_BASE          0x60   // Offset for Function
#endif

// DTCOMMON functions
// DON'T INSERT NEW VALUES BETWEEN EXISTING ONES, SO THAT NEW DRIVER VERSIONS REMAIN 
// COMPATIBLE WITH USER APPLICATIONS THAT WERE COMPILED AGAINST AN OLDER DRIVER!!
enum {
    DT_FUNC_CODE_GET_DRIVER_VERSION =  DT_IOCTL_BASE,
    DT_FUNC_CODE_GET_DEV_INFO,
    DT_FUNC_CODE_STUB_CMD,
    DT_FUNC_CODE_ASIRX_CMD,
    DT_FUNC_CODE_ASITXG_CMD,
    DT_FUNC_CODE_ASITXSER_CMD,
    DT_FUNC_CODE_CDMAC_CMD,
    DT_FUNC_CODE_DEBUG_CMD,
    DT_FUNC_CODE_EVENT_CMD,
    DT_FUNC_CODE_EXCL_ACCESS_CMD,
    DT_FUNC_CODE_I2CM_CMD,
    DT_FUNC_CODE_IOCONFIG_CMD,
    DT_FUNC_CODE_KA_CMD,
    DT_FUNC_CODE_LEDB_CMD,
    DT_FUNC_CODE_PROPERTY_CMD,
    DT_FUNC_CODE_REBOOT_CMD,
    DT_FUNC_CODE_SDIRX_CMD,
    DT_FUNC_CODE_SDIRXF_CMD,
    DT_FUNC_CODE_SDIRXP_CMD,
    DT_FUNC_CODE_SDITXF_CMD,
    DT_FUNC_CODE_SDITXP_CMD,
    DT_FUNC_CODE_SDITXPHY_CMD,
    DT_FUNC_CODE_SDIXCFG_CMD,
    DT_FUNC_CODE_SPIM_CMD,
    DT_FUNC_CODE_SPIMF_CMD,
    DT_FUNC_CODE_SPIPROM_CMD,
    DT_FUNC_CODE_SWITCH_CMD,
    DT_FUNC_CODE_TOD_CMD,
    DT_FUNC_CODE_TSRXFMT_CMD,
    DT_FUNC_CODE_VPD_CMD,
    DT_FUNC_CODE_ADS4246_CMD_2132,
    DT_FUNC_CODE_AD5320_CMD_2132,
    DT_FUNC_CODE_S2STATS_CMD_2132,
    DT_FUNC_CODE_S2CRDEMOD_CMD_2132,
    DT_FUNC_CODE_IQCGRAB_CMD_2132,
    DT_FUNC_CODE_IQFIR_CMD_2132,
    DT_FUNC_CODE_IQPWR_CMD_2132,
    DT_FUNC_CODE_IQSRC2132_CMD_2132,
    DT_FUNC_CODE_DATASTMUX_CMD_2132,
    DT_FUNC_CODE_LNBH25_CMD_2132,
    DT_FUNC_CODE_IPSECG_CMD,
    DT_FUNC_CODE_SENSTEMP_CMD,
    DT_FUNC_CODE_TEMPFANMGR_CMD,
    DT_FUNC_CODE_CONSTSINK_CMD,
    DT_FUNC_CODE_CONSTSOURCE_CMD,
    DT_FUNC_CODE_ACCUFIFO_CMD,
    DT_FUNC_CODE_BURSTFIFO_CMD,
    DT_FUNC_CODE_LMH1981_CMD,
    DT_FUNC_CODE_GENLOCKCTRL_CMD,
    DT_FUNC_CODE_SDIDMX12G_CMD,
    DT_FUNC_CODE_SDIMUX12G_CMD,
    DT_FUNC_CODE_ST425LR_CMD,
    DT_FUNC_CODE_S12GTO3G_CMD,
    DT_FUNC_CODE_SI2166ITF_CMD,
    DT_FUNC_CODE_TODCLOCKCTRL_CMD,
    DT_FUNC_CODE_EMAC_CMD,
    DT_FUNC_CODE_NW_CMD,
    DT_FUNC_CODE_TRIGEVT_CMD,
    DT_FUNC_CODE_PIPE_CMD,
    DT_FUNC_CODE_SET_NWDRIVER_VERSION,
    DT_FUNC_CODE_GET_NWDRIVER_VERSION,
    DT_FUNC_CODE_DDRFRONT_CMD,
    DT_FUNC_CODE_IOSERIN_CMD,
    DT_FUNC_CODE_DISEQC_CMD,
    DT_FUNC_CODE_GPSTIME_CMD,
    DT_FUNC_CODE_IQUNPCK_CMD,
    DT_FUNC_CODE_DATACDC_CMD,
    DT_FUNC_CODE_IQFIR_CMD,
    DT_FUNC_CODE_IQNOISE_CMD,
    DT_FUNC_CODE_IQMISC_CMD,
    DT_FUNC_CODE_IQSYNC_CMD,
    DT_FUNC_CODE_IQUP_CMD,
    DT_FUNC_CODE_IQJESD_CMD,
    DT_FUNC_CODE_ATTNCTRL_CMD_2116,
    DT_FUNC_CODE_IO_CMD_2116,
    DT_FUNC_CODE_DATAFIFO_CMD,
    DT_FUNC_CODE_DDRFIFO_CMD,
    DT_FUNC_CODE_CHSDIRX_CMD,
    DT_FUNC_CODE_CHSDIRXPHYONLY_CMD,
    DT_FUNC_CODE_DATAC_CMD,
    DT_FUNC_CODE_IOPARIN_CMD,
    DT_FUNC_CODE_IQAGC_CMD,
    DT_FUNC_CODE_IQDEMOD_CMD,
    DT_FUNC_CODE_IQDOWNSRC_CMD,
    DT_FUNC_CODE_IQFIR2XCLK_CMD,
    DT_FUNC_CODE_IQINV_CMD,
    DT_FUNC_CODE_IQPWR_CMD,
    DT_FUNC_CODE_AD9789ITF_CMD
};

// NOP command
#define DT_IOCTL_CMD_NOP        -1

// Common Driver-Function/Block-controller IOCTL input data header
typedef struct _DtIoctlInputDataHdr
{
    Int  m_Uuid;                    // Unique ID of the DF/BC targetted
    Int  m_PortIndex;               // Port-index the DF/BC resides in. -1 = device
    Int  m_Cmd;                     // The command
    Int  m_CmdEx;                   // Extended command
}  DtIoctlInputDataHdr;

// MACRO's to compute the required size of the input and output data buffer
#define DT_IOCTL_INDATA_MINSIZE  ((Int)(sizeof(DtIoctlInputDataHdr)))
#define DT_IOCTL_INDATA_REQSIZE(DATA_STRUCT)            \
    ((Int)(DT_IOCTL_INDATA_MINSIZE + sizeof(DATA_STRUCT)))
#define DT_IOCTL_OUTDATA_REQSIZE(DATA_STRUCT)  ((Int)(sizeof(DATA_STRUCT)))

// MACRO to extract function code from an IOCTL code.
// Note: an IOCTL is an OR of several fields (e.g. DeviceType, RequiredAccess, FunctionCode, 
// TransferType and on Linux also the size of the IOCTL structure is part of the code). When this
// structure grows/shrinks the IOCTL code changes breaking backward compatibility. 
// Therefore one should use the only the function code to determine which IOCTL is received.
#ifdef WINBUILD 
#define DT_IOCTL_TO_FUNCTION(IoctlCode)  ((UInt32)((IoctlCode>>2)&0xFFF))
#else
#define DT_IOCTL_TO_FUNCTION(IoctlCode)  ((UInt32)_IOC_NR(IoctlCode))
#endif

#ifdef WINBUILD
struct _DtTrigEvtDriver
{
    Int m_Id;
    UInt64A m_Reserved;
    union {
        HANDLE m_hEvent;
        // 32-bit applications do have a 32-bit HANDLE pointer. These are interoperabable
        // with 64-bit drivers, but the size in the IOCTL must always be 64-bit.
        void* m_pKernelEvent;  // For kernel driver to driver event (DtEvent)
        UInt64A m_32BitDoNotUseThis;  // For alignment only
    };
    Bool m_KernelEvent;  //TRUE if pDrvEvent is filled (DtEvent)
};
#else
#define SIGNAL_TRIG_EVT_DRIVER 50
struct _DtTrigEvtDriver
{
    Int m_Id;
    UInt64A m_pVoid;
    union {
        void* m_pKernelEvent;  // For kernel driver to driver event (DtEvent)
        UInt64A m_32BitDoNotUseThis;  // For alignment only
    };
    Bool m_KernelEvent;  //TRUE if pDrvEvent is filled (DtEvent)
};
#endif
typedef struct _DtTrigEvtDriver DtTrigEvtDriver;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+= DT_IOCTL_ACCUFIFO_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-. ACCUFIFO commands .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
// NOTE: PUT NEW SETS OF COMMANDS IN ALPHABETICAL ORDER WITHOUT DISTURBING OLD COMMANDS.
typedef enum _DtIoctlAccuFifoCmd
{
    DT_ACCUFIFO_CMD_GET_ACCU_PERIOD = 0,
    DT_ACCUFIFO_CMD_GET_MAX_NUM_MEASM = 1,
    DT_ACCUFIFO_CMD_GET_MEASM_STATUS = 2,
    DT_ACCUFIFO_CMD_GET_NUM_MEASM = 3,
    DT_ACCUFIFO_CMD_READ_MEASMS = 4,
    DT_ACCUFIFO_CMD_RESTART_MEASM = 5,
    DT_ACCUFIFO_CMD_SET_ACCU_PERIOD = 6,
    DT_ACCUFIFO_CMD_SET_NUM_MEASM = 7,
}  DtIoctlAccuFifoCmd;

// -.-.-.-.-.-.-.-.-.-.-.-.- ACCUFIFO Command - Get Accu Period -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAccuFifoCmdGetAccuPeriodInput;
ASSERT_SIZE(DtIoctlAccuFifoCmdGetAccuPeriodInput, 16)
typedef struct _DtIoctlAccuFifoCmdGetAccuPeriodOutput
{
    Int  m_AccuPeriod;           // Accu period in ns
}  DtIoctlAccuFifoCmdGetAccuPeriodOutput;
ASSERT_SIZE(DtIoctlAccuFifoCmdGetAccuPeriodOutput, 4)

// .-.-.-.-.-.-.-.- ACCUFIFO Command - Get Maximum number of measurements -.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAccuFifoCmdGetMaxNumMeasmInput;
ASSERT_SIZE(DtIoctlAccuFifoCmdGetMaxNumMeasmInput, 16)
typedef struct _DtIoctlAccuFifoCmdGetMaxNumMeasmOutput
{
    Int  m_MaxNumMeasm;           // Maximum number of measurments
}  DtIoctlAccuFifoCmdGetMaxNumMeasmOutput;
ASSERT_SIZE(DtIoctlAccuFifoCmdGetMaxNumMeasmOutput, 4)

// .-.-.-.-.-.-.-.-.-.- ACCUFIFO Command - Get number of measurements -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAccuFifoCmdGetNumMeasmInput;
ASSERT_SIZE(DtIoctlAccuFifoCmdGetNumMeasmInput, 16)
typedef struct _DtIoctlAccuFifoCmdGetNumMeasmOutput
{
    Int  m_NumMeasm;           // Number of measurments
}  DtIoctlAccuFifoCmdGetNumMeasmOutput;
ASSERT_SIZE(DtIoctlAccuFifoCmdGetNumMeasmOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.- ACCUFIFO Command - Get measurement status -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAccuFifoCmdGetMeasmStatusInput;
ASSERT_SIZE(DtIoctlAccuFifoCmdGetMeasmStatusInput, 16)
typedef struct _DtIoctlAccuFifoCmdGetMeasmStatusOutput
{
    Int  m_NumAvailable;    // Number of measurments available
    Int  m_MeasmDone;       // Measurement done (bool)
}  DtIoctlAccuFifoCmdGetMeasmStatusOutput;
ASSERT_SIZE(DtIoctlAccuFifoCmdGetMeasmStatusOutput, 8)

// .-.-.-.-.-.-.-.-.-.- ACCUFIFO Command - Read Measurements Command -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlAccuFifoCmdReadMeasmsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumToRead;
} DtIoctlAccuFifoCmdReadMeasmsInput;
ASSERT_SIZE(DtIoctlAccuFifoCmdReadMeasmsInput, 20)

typedef struct _DtIoctlAccuFifoCmdReadMeasmsOutput
{
    Int  m_NumRead;
    Int  m_Buf[0];  // Dynamic sized buffer
} DtIoctlAccuFifoCmdReadMeasmsOutput;
ASSERT_SIZE(DtIoctlAccuFifoCmdReadMeasmsOutput, 4)

// -.-.-.-.-.-.-.-.-.-.-.- ACCUFIFO Command - Restart measurement -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAccuFifoCmdRestartMeasmInput;
ASSERT_SIZE(DtIoctlAccuFifoCmdRestartMeasmInput, 16)

// -.-.-.-.-.-.-.-.-.-.-.-.- ACCUFIFO Command - Set Accu Period -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlAccuFifoCmdSetAccuPeriodInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_AccuPeriod;           // Accu period in ns
}  DtIoctlAccuFifoCmdSetAccuPeriodInput;
ASSERT_SIZE(DtIoctlAccuFifoCmdSetAccuPeriodInput, 20)

// .-.-.-.-.-.-.-.-.-.- ACCUFIFO Command - Set Number of Measurements -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlAccuFifoCmdSetNumMeasmInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumMeasm;           // Number of measurments
}  DtIoctlAccuFifoCmdSetNumMeasmInput;
ASSERT_SIZE(DtIoctlAccuFifoCmdSetNumMeasmInput, 20)


// .-.-.-.-.-.-.-.-.-.-.-.- ACCUFIFO Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// ACCUFIFO command - Input data
typedef union _DtIoctlAccuFifoCmdInput
{
    DtIoctlAccuFifoCmdReadMeasmsInput  m_ReadMeasms;
    DtIoctlAccuFifoCmdSetAccuPeriodInput  m_SetAccuPeriod;
    DtIoctlAccuFifoCmdSetNumMeasmInput  m_SetNumMeasm;
} DtIoctlAccuFifoCmdInput;
ASSERT_SIZE(DtIoctlAccuFifoCmdInput, 20)

// ACCUFIFO command - Output data
typedef union _DtIoctlAccuFifoCmdOutput
{
    DtIoctlAccuFifoCmdGetAccuPeriodOutput  m_GetAccuPeriod;
    DtIoctlAccuFifoCmdGetMaxNumMeasmOutput  m_GetMaxNumMeasm;
    DtIoctlAccuFifoCmdGetMeasmStatusOutput  m_GetMeasmStatus;
    DtIoctlAccuFifoCmdGetNumMeasmOutput  m_GetNumMeasm;
    DtIoctlAccuFifoCmdReadMeasmsOutput  m_ReadMeasms;
} DtIoctlAccuFifoCmdOutput;
ASSERT_SIZE(DtIoctlAccuFifoCmdOutput, 8)

#ifdef WINBUILD
#define DT_IOCTL_ACCUFIFO_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_ACCUFIFO_CMD,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlAccuFifoCmdInOut
{
    DtIoctlAccuFifoCmdInput  m_Input;
    DtIoctlAccuFifoCmdOutput  m_Output;
}  DtIoctlAccuFifoCmdInOut;

#define DT_IOCTL_ACCUFIFO_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_ACCUFIFO_CMD,     \
                                                                 DtIoctlAccuFifoCmdInOut)
#endif


// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_AD9789ITF_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- AD9789ITF Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlAd9789ItfCmd
{
    DT_AD9789ITF_CMD_GET_OPERATIONAL_MODE = 0, // Get operational mode
    DT_AD9789ITF_CMD_SET_OPERATIONAL_MODE = 1, // Set operational mode
}  DtIoctlAd9789ItfCmd;

// .-.-.-.-.-.-.-.-.- AD9789ITF Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlAd9789ItfCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlAd9789ItfCmdGetOpModeInput, 16)
typedef struct _DtIoctlAd9789ItfCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlAd9789ItfCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlAd9789ItfCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.- AD9789ITF Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlAd9789ItfCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlAd9789ItfCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlAd9789ItfCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.- AD9789ITF Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.
// AD9789ITF command - IOCTL input data
typedef union _DtIoctlAd9789ItfCmdInput
{
    DtIoctlAd9789ItfCmdSetOpModeInput  m_SetOpMode;            // Ad9789Itf - Set op. mode
}  DtIoctlAd9789ItfCmdInput;


// AD9789ITF command - IOCTL output data
typedef union _DtIoctlAd9789ItfCmdOutput
{
    DtIoctlAd9789ItfCmdGetOpModeOutput  m_GetOpMode;           // Ad9789Itf - Get op. mode
}  DtIoctlAd9789ItfCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_AD9789ITF_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_AD9789ITF_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlAd9789ItfCmdInOut
    {
        DtIoctlAd9789ItfCmdInput  m_Input;
        DtIoctlAd9789ItfCmdOutput  m_Output;
    }  DtIoctlAd9789ItfCmdInOut;
#define DT_IOCTL_AD9789ITF_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_AD9789ITF_CMD,       \
                                                                     DtIoctlAd9789ItfCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_ASIRX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ASI-TS-Reciever Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlAsiRxCmd
{
    DT_ASIRX_CMD_GET_OPERATIONAL_STATUS = 0,
    DT_ASIRX_CMD_GET_PACKET_MODE = 1,
    DT_ASIRX_CMD_GET_POLARITY_CTRL = 2,
    DT_ASIRX_CMD_GET_STATUS = 3,
    DT_ASIRX_CMD_GET_SYNC_MODE = 4,
    DT_ASIRX_CMD_GET_TS_BITRATE = 5,
    DT_ASIRX_CMD_GET_VIOL_COUNT = 6,
    DT_ASIRX_CMD_SET_OPERATIONAL_MODE = 7,
    DT_ASIRX_CMD_SET_PACKET_MODE = 8,
    DT_ASIRX_CMD_SET_POLARITY_CTRL = 9,
    DT_ASIRX_CMD_SET_SYNC_MODE = 10,
}  DtIoctlAsiRxCmd;


// DT ASIRX packetization mode
#define DT_ASIRX_PCKMODE_AUTO     0   // Automitic packetization
#define DT_ASIRX_PCKMODE_RAW      1   // Raw packetization

// DT ASIRX packetsize
#define DT_ASIRX_PCKSIZE_UNKNOWN  0   // Unknown packetsize
#define DT_ASIRX_PCKSIZE_188      1   // 188-byte TS packets
#define DT_ASIRX_PCKSIZE_204      2   // 204-byte TS packets

// DT ASIRX polarity
#define DT_ASIRX_POLARITY_AUTO    0   // Auto polarity
#define DT_ASIRX_POLARITY_UNKNOWN 1   // Unknown polarity
#define DT_ASIRX_POLARITY_NORMAL  2   // Normal polarity
#define DT_ASIRX_POLARITY_INVERT  3   // Inverted polarity

// DT ASIRX synchronization mode
#define DT_ASIRX_SYNCMODE_AUTO    0   // Automatic synchronization
#define DT_ASIRX_SYNCMODE_188     1   // Synchronize on 188-byte TS packets
#define DT_ASIRX_SYNCMODE_204     2   // Synchronize on 204-byte TS packets

// -.-.-.-.-.-.-.-.-.- ASIRX Command - Get Operational Status Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAsiRxCmdGetOpStatusInput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetOpStatusInput, 16)
typedef struct _DtIoctlAsiRxCmdGetOpStatusOutput
{
    Int  m_OpStatus;             // Operational status
}  DtIoctlAsiRxCmdGetOpStatusOutput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetOpStatusOutput, 4)
// -.-.-.-.-.-.-.-.-.-.-.- ASIRX Command - Get Packet Mode Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlAsiRxCmdGetPckModeInput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetPckModeInput, 16)
typedef struct _DtIoctlAsiRxCmdGetPckModeOutput
{
    Int  m_PckMode;          // Packetizer mode
}  DtIoctlAsiRxCmdGetPckModeOutput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetPckModeOutput, 4)
// .-.-.-.-.-.-.-.-.-.- ASIRX Command - Get Polarity Control Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlAsiRxCmdGetPolarityCtrlInput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetPolarityCtrlInput, 16)
typedef struct _DtIoctlAsiRxCmdGetPolarityCtrlOutput
{
    Int  m_PolarityCtrl;        // Polarity control
}  DtIoctlAsiRxCmdGetPolarityCtrlOutput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetPolarityCtrlOutput, 4)
// -.-.-.-.-.-.-.-.-.-.-.-.- ASIRX Command - Get Status Command -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAsiRxCmdGetStatusInput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetStatusInput, 16)
typedef struct _DtIoctlAsiRxCmdGetStatusOutput
{

    Int  m_PacketSize;          // Detected packet size
    Int  m_CarrierDetect;       // Carrier detect status
    Int  m_AsiLock;             // ASI lock status
    Int  m_AsiPolarity;         // ASI polarity status
}  DtIoctlAsiRxCmdGetStatusOutput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetStatusOutput, 16)
// .-.-.-.-.-.-.-.-.-.-.-.- ASIRX Command - Get Sync Mode Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAsiRxCmdGetSyncModeInput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetSyncModeInput, 16)
typedef struct _DtIoctlAsiRxCmdGetSyncModeOutput
{
    Int  m_SyncMode;            // Synchronizer mode
}  DtIoctlAsiRxCmdGetSyncModeOutput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetSyncModeOutput, 4)
// .-.-.-.-.-.-.-.- ASIRX Command - Get Transport Stream Bitrate Command -.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlAsiRxCmdGetTsBitrateInput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetTsBitrateInput, 16)
typedef struct _DtIoctlAsiRxCmdGetTsBitrateOutput
{
    Int  m_TsBitrate;       // Tranport-stream bitrate
}  DtIoctlAsiRxCmdGetTsBitrateOutput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetTsBitrateOutput, 4)
// -.-.-.-.-.-.-.-.-.-.- ASIRX Command - Get Violation Count Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlAsiRxCmdGetViolCountInput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetViolCountInput, 16)
typedef struct _DtIoctlAsiRxCmdGetViolCountOutput
{
    Int  m_ViolCount;           // Violation count
}  DtIoctlAsiRxCmdGetViolCountOutput;
ASSERT_SIZE(DtIoctlAsiRxCmdGetViolCountOutput, 4)

// .-.-.-.-.-.-.-.-.-.- ASIRX Command - Set Operational Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlAsiRxCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode: IDLE/RUN
}  DtIoctlAsiRxCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlAsiRxCmdSetOpModeInput, 20)
// -.-.-.-.-.-.-.-.-.-.-.- ASIRX Command - Set Packet Mode Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlAsiRxCmdSetPckModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_PckMode;             // Packetizer mode: AUTO/RAW
}  DtIoctlAsiRxCmdSetPckModeInput;
ASSERT_SIZE(DtIoctlAsiRxCmdSetPckModeInput, 20)
// .-.-.-.-.-.-.-.-.-.- ASIRX Command - Set Polarity Control Command -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlAsiRxCmdSetPolarityCtrlInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_PolarityCtrl;        // Polarity control: AUTO/NORMAL/INVERTED
}  DtIoctlAsiRxCmdSetPolarityCtrlInput;
ASSERT_SIZE(DtIoctlAsiRxCmdSetPolarityCtrlInput, 20)
// .-.-.-.-.-.-.-.-.-.- ASIRX Command - Set Synchronizer Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlAsiRxCmdSetSyncModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SyncMode;        // Synchronizer mode: AUTO/188/204
}  DtIoctlAsiRxCmdSetSyncModeInput;
ASSERT_SIZE(DtIoctlAsiRxCmdSetSyncModeInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.-.- ASIRX Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// ASIRX command - IOCTL input data
typedef union _DtIoctlAsiRxCmdInput
{
    DtIoctlAsiRxCmdSetOpModeInput  m_SetOpMode;          // AsiTsRx - Set operational mode
    DtIoctlAsiRxCmdSetPckModeInput  m_SetPckMode;        // AsiTsRx - Set packet mode
    DtIoctlAsiRxCmdSetPolarityCtrlInput  m_SetPolarity;  // AsiTsRx - Set polarity 
    DtIoctlAsiRxCmdSetSyncModeInput  m_SetSyncMode;      // AsiTsRx - Set syncmode
}  DtIoctlAsiRxCmdInput;
// ASIRX command - IOCTL output data
typedef union _DtIoctlAsiRxCmdOutput
{
    DtIoctlAsiRxCmdGetOpStatusOutput  m_GetOpStatus;  // AsiTsRx - Get operational status
    DtIoctlAsiRxCmdGetPckModeOutput  m_GetPacketMode;     // AsiTsRx - Get packet mode
    DtIoctlAsiRxCmdGetPolarityCtrlOutput  m_GetPolarity;  // AsiTsRx - Get polarity
    DtIoctlAsiRxCmdGetStatusOutput  m_GetStatus;          // AsiTsRx - Get signal status
    DtIoctlAsiRxCmdGetTsBitrateOutput  m_GetTsBitrate;    // AsiTsRx - Get TS bitrate
    DtIoctlAsiRxCmdGetSyncModeOutput  m_GetSyncMode;      // AsiTsRx - Get sync mode
    DtIoctlAsiRxCmdGetViolCountOutput  m_GetViolCount;    // AsiTsRx - Get violation count
}  DtIoctlAsiRxCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_ASIRX_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_ASIRX_CMD,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlAsiRxCmdInOut
    {
        DtIoctlAsiRxCmdInput  m_Input;
        DtIoctlAsiRxCmdOutput  m_Output;
    }  DtIoctlAsiRxCmdInOut;
    #define DT_IOCTL_ASIRX_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_ASIRX_CMD,       \
                                                                     DtIoctlAsiRxCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_ASITXG_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ASI-TX-Gate Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlAsiTxGCmd
{
    DT_ASITXG_CMD_GET_ASI_POLARITY = 0,
    DT_ASITXG_CMD_GET_OPERATIONAL_MODE = 1,
    DT_ASITXG_CMD_SET_ASI_POLARITY = 2,
    DT_ASITXG_CMD_SET_OPERATIONAL_MODE = 3,
    DT_ASITXG_CMD_CLEAR_INPUT_STATE = 4,
}  DtIoctlAsiTxGCmd;


// DT ASITXG polarity
#define DT_ASITXG_POL_NORMAL        0   // Normal polarity
#define DT_ASITXG_POL_INVERT        1   // Inverted polarity

// -.-.-.-.-.-.-.-.-.-.- ASITXG Command - Clear Input State Command -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAsiTxGCmdClearInputStateInput;
ASSERT_SIZE(DtIoctlAsiTxGCmdClearInputStateInput, 16)

//.-.-.-.-.-.-.-.-.-.-.- ASITXG Command - Get ASI polarity Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlAsiTxGCmdGetAsiPolarityInput;
ASSERT_SIZE(DtIoctlAsiTxGCmdGetAsiPolarityInput, 16)
typedef struct _DtIoctlAsiTxGCmdGetAsiPolarityOutput
{
    Int  m_AsiPolarity;         // ASI polarity
}  DtIoctlAsiTxGCmdGetAsiPolarityOutput;
ASSERT_SIZE(DtIoctlAsiTxGCmdGetAsiPolarityOutput, 4)

//.-.-.-.-.-.-.-.-.-.- ASITXG Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlAsiTxGCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlAsiTxGCmdGetOpModeInput, 16)
typedef struct _DtIoctlAsiTxGCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlAsiTxGCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlAsiTxGCmdGetOpModeOutput, 4)
//.-.-.-.-.-.-.-.-.-.-.- ASITXG Command - Set ASI Polarity Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlAsiTxGCmdSetAsiPolarityInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_AsiPolarity;         // ASI polarity
}  DtIoctlAsiTxGCmdSetAsiPolarityInput;
ASSERT_SIZE(DtIoctlAsiTxGCmdSetAsiPolarityInput, 20)

//.-.-.-.-.-.-.-.-.-.- ASITXG Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlAsiTxGCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlAsiTxGCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlAsiTxGCmdSetOpModeInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- ASITXG Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// ASITXG command - IOCTL input data
typedef union _DtIoctlAsiTxGCmdInput
{
    DtIoctlAsiTxGCmdSetAsiPolarityInput  m_SetAsiPolarity;  // AsiTxG - Set ASI polarity
    DtIoctlAsiTxGCmdSetOpModeInput  m_SetOpMode;            // AsiTxG - Set op. mode
}  DtIoctlAsiTxGCmdInput;
// ASITXG command - IOCTL output data
typedef union _DtIoctlAsiTxGCmdOutput
{
    DtIoctlAsiTxGCmdGetAsiPolarityOutput  m_GetAsiPolarity; // AsiTxG - Get ASI polarity
    DtIoctlAsiTxGCmdGetOpModeOutput  m_GetOpMode;           // AsiTxG - Get op. mode
}  DtIoctlAsiTxGCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_ASITXG_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_ASITXG_CMD,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlAsiTxGCmdInOut
    {
        DtIoctlAsiTxGCmdInput  m_Input;
        DtIoctlAsiTxGCmdOutput  m_Output;
    }  DtIoctlAsiTxGCmdInOut;
    #define DT_IOCTL_ASITXG_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_ASITXG_CMD,     \
                                                                    DtIoctlAsiTxGCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_ASITXSER_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ASI-TX-Serializer Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlAsiTxSerCmd
{
    DT_ASITXSER_CMD_GET_OPERATIONAL_MODE = 0,
    DT_ASITXSER_CMD_SET_OPERATIONAL_MODE = 1,
}  DtIoctlAsiTxSerCmd;


// DT ASITXSER polarity
#define DT_ASITXSER_POL_NORMAL        0   // Normal polarity
#define DT_ASITXSER_POL_INVERT        1   // Inverted polarity


//-.-.-.-.-.-.-.-.-.- ASITXSER Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAsiTxSerCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlAsiTxSerCmdGetOpModeInput, 16)
typedef struct _DtIoctlAsiTxSerCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlAsiTxSerCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlAsiTxSerCmdGetOpModeOutput, 4)

//-.-.-.-.-.-.-.-.-.- ASITXSER Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlAsiTxSerCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlAsiTxSerCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlAsiTxSerCmdSetOpModeInput, 20)

//-.-.-.-.-.-.-.-.-.-.-.-.- ASITXSER Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// ASITXSER command - IOCTL input data
typedef union _DtIoctlAsiTxSerCmdInput
{
    DtIoctlAsiTxSerCmdSetOpModeInput  m_SetOpMode;            // AsiTxSer - Set op. mode
}  DtIoctlAsiTxSerCmdInput;
// ASITXSER command - IOCTL output data
typedef union _DtIoctlAsiTxSerCmdOutput
{
    DtIoctlAsiTxSerCmdGetOpModeOutput  m_GetOpMode;           // AsiTxSer - Get op. mode
}  DtIoctlAsiTxSerCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_ASITXSER_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_ASITXSER_CMD,   \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlAsiTxSerCmdInOut
    {
        DtIoctlAsiTxSerCmdInput  m_Input;
        DtIoctlAsiTxSerCmdOutput  m_Output;
    }  DtIoctlAsiTxSerCmdInOut;
    #define DT_IOCTL_ASITXSER_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_ASITXSER_CMD, \
                                                                  DtIoctlAsiTxSerCmdInOut)
#endif
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_BURSTFIFO_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- BURSTFIFO Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlBurstFifoCmd
{
    DT_BURSTFIFO_CMD_CLEAR_FIFO_MAX = 0,
    DT_BURSTFIFO_CMD_GET_DIRECTION = 1,
    DT_BURSTFIFO_CMD_GET_FIFO_STATUS = 2,
    DT_BURSTFIFO_CMD_GET_OPERATIONAL_MODE = 3,
    DT_BURSTFIFO_CMD_GET_OVFL_UFL_COUNT = 4,
    DT_BURSTFIFO_CMD_GET_PROPERTIES = 5,
    DT_BURSTFIFO_CMD_GET_RXFLOWCONTROL = 6,
    DT_BURSTFIFO_CMD_SET_DIRECTION = 7,
    DT_BURSTFIFO_CMD_SET_OPERATIONAL_MODE = 8,
    DT_BURSTFIFO_CMD_SET_RXFLOWCONTROL = 9, 
}  DtIoctlBurstFifoCmd;

// DT BURSTFIFO RX/TX capability flags
#define DT_BURSTFIFO_CAP_RX         1   // Receive capability
#define DT_BURSTFIFO_CAP_TX         2   // Transmit capability

// DT BURSTFIFO direction
#define DT_BURSTFIFO_DIR_RX         0   // Receive 
#define DT_BURSTFIFO_DIR_TX         1   // Transmit

// DT BURSTFIFO flow control
#define DT_BURSTFIFO_FLOW_PUSHPULL  0  // Push-pull flow control
#define DT_BURSTFIFO_FLOW_VIRTPUSH  1  // Virtual push flow control

// -.-.-.-.-.-.-.-.-.-.-.-.- BURSTFIFO Command - Clear Fifo Max -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlBurstFifoCmdClearFifoMaxInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_ClearMaxFree;        // Clear max free (boolean)
    Int  m_ClearMaxLoad;        // Clear max load (boolean)
}  DtIoctlBurstFifoCmdClearFifoMaxInput;
ASSERT_SIZE(DtIoctlBurstFifoCmdClearFifoMaxInput, 24)

// .-.-.-.-.-.-.-.-.-.-.- BURSTFIFO Command - Get Direction Command -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlBurstFifoCmdGetDirectionInput;
ASSERT_SIZE(DtIoctlBurstFifoCmdGetDirectionInput, 16)
typedef struct _DtIoctlBurstFifoCmdGetDirectionOutput
{
    Int  m_Direction;      // Direction

}  DtIoctlBurstFifoCmdGetDirectionOutput;
ASSERT_SIZE(DtIoctlBurstFifoCmdGetDirectionOutput, 4)

// -.-.-.-.-.-.-.-.-.-.- BURSTFIFO Command - Get Fifo Status Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlBurstFifoCmdGetFifoStatusInput;
ASSERT_SIZE(DtIoctlBurstFifoCmdGetFifoStatusInput, 16)
typedef struct _DtIoctlBurstFifoCmdGetFifoStatusOutput
{
    Int  m_CurFree;     // Burst current fifo free space in #bytes
    Int  m_CurLoad;     // Burst current fifo load in #bytes
    Int  m_MaxFree;     // Burst fifo maximum free space in #bytes
    Int  m_MaxLoad;     // Burst fifo maximum load in #bytes
}  DtIoctlBurstFifoCmdGetFifoStatusOutput;
ASSERT_SIZE(DtIoctlBurstFifoCmdGetFifoStatusOutput, 16)

// .-.-.-.-.-.-.-.-.- BURSTFIFO Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlBurstFifoCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlBurstFifoCmdGetOpModeInput, 16)
typedef struct _DtIoctlBurstFifoCmdGetOpModeOutput
{
    Int  m_OpMode;      // Operational Mode

}  DtIoctlBurstFifoCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlBurstFifoCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.- BURSTFIFO Command - Get Overflow/Undeflow Count Command -.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlBurstFifoCmdGetOvfUflCountInput;
ASSERT_SIZE(DtIoctlBurstFifoCmdGetOvfUflCountInput, 16)
typedef struct _DtIoctlBurstFifoCmdGetOvfUflCountOutput
{
    Int  m_OvfUflCount;      // Overflow / Underflow count

}  DtIoctlBurstFifoCmdGetOvfUflCountOutput;
ASSERT_SIZE(DtIoctlBurstFifoCmdGetOvfUflCountOutput, 4)

// -.-.-.-.-.-.-.-.-.-.- BURSTFIFO Command - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlBurstFifoCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlBurstFifoCmdGetPropertiesInput, 16)
typedef struct _DtIoctlBurstFifoCmdGetPropertiesOutput
{
    UInt32  m_Capabilities;     // Burst Fifo capability flags
    Int  m_DataWidth;           // Data width in #bits
    Int  m_BurstFifoSize;       // Burst fifo size in #bytes
}  DtIoctlBurstFifoCmdGetPropertiesOutput;
ASSERT_SIZE(DtIoctlBurstFifoCmdGetPropertiesOutput, 12)

// .-.-.-.-.-.-.-.- BURSTFIFO Command - Get RX Input Flow Control Command -.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlBurstFifoCmdGetRxFlowControlInput;
ASSERT_SIZE(DtIoctlBurstFifoCmdGetRxFlowControlInput, 16)
typedef struct _DtIoctlBurstFifoCmdGetRxFlowControlOutput
{
    Int  m_RxFlowControl;      // Rx Input flow control
}  DtIoctlBurstFifoCmdGetRxFlowControlOutput;
ASSERT_SIZE(DtIoctlBurstFifoCmdGetRxFlowControlOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.-.- BURSTFIFO Command - Set Direction -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlBurstFifoCmdSetDirectionInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Direction;              // Direction
}  DtIoctlBurstFifoCmdSetDirectionInput;
ASSERT_SIZE(DtIoctlBurstFifoCmdSetDirectionInput, 20)

// .-.-.-.-.-.-.-.-.-.-.- BURSTFIFO Command - Set Operational Mode -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlBurstFifoCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlBurstFifoCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlBurstFifoCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.- BURSTFIFO Command - Set RX Input Flow Control Command -.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlBurstFifoCmdSetRxFlowControlInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_RxFlowControl;      // Rx Input flow control
}  DtIoctlBurstFifoCmdSetRxFlowControlInput;
ASSERT_SIZE(DtIoctlBurstFifoCmdSetRxFlowControlInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.- BURSTFIFO Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.
// BURSTFIFO command - IOCTL input data
typedef union _DtIoctlBurstFifoCmdInput
{
    DtIoctlBurstFifoCmdClearFifoMaxInput  m_ClearFifoMax;   // Clear fifo max
    DtIoctlBurstFifoCmdSetDirectionInput  m_SetDirection;   // Set direction
    DtIoctlBurstFifoCmdSetOpModeInput  m_SetOpMode;         // Set op mode
    DtIoctlBurstFifoCmdSetRxFlowControlInput  m_SetRxFlCtrl; // Set flow control
}  DtIoctlBurstFifoCmdInput;
// BURSTFIFO command - IOCTL output data
typedef union _DtIoctlBurstFifoCmdOutput
{
    DtIoctlBurstFifoCmdGetDirectionOutput  m_GetDirection;   // Get direction
    DtIoctlBurstFifoCmdGetFifoStatusOutput  m_GetFifoStatus; // Get fifo status
    DtIoctlBurstFifoCmdGetOpModeOutput  m_GetOpMode;         // Get op mode
    DtIoctlBurstFifoCmdGetOvfUflCountOutput  m_GetOvfUflCnt; // Get over/underflow count
    DtIoctlBurstFifoCmdGetPropertiesOutput  m_GetProperties; // Get properties
    DtIoctlBurstFifoCmdGetRxFlowControlOutput  m_GetRxFlCtrl; // Get flow control

}  DtIoctlBurstFifoCmdOutput;
#ifdef WINBUILD
#define DT_IOCTL_BURSTFIFO_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_BURSTFIFO_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlBurstFifoCmdInOut
{
    DtIoctlBurstFifoCmdInput  m_Input;
    DtIoctlBurstFifoCmdOutput  m_Output;
}  DtIoctlBurstFifoCmdInOut;
#define DT_IOCTL_BURSTFIFO_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_BURSTFIFO_CMD,   \
                                                                 DtIoctlBurstFifoCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_DATAFIFO_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DATAFIFO Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlDataFifoCmd
{
    DT_DATAFIFO_CMD_CLEAR_FIFO_MAX       = 0,
    DT_DATAFIFO_CMD_GET_DIRECTION        = 1,
    DT_DATAFIFO_CMD_GET_FIFO_STATUS      = 2,
    DT_DATAFIFO_CMD_GET_OPERATIONAL_MODE = 3,
    DT_DATAFIFO_CMD_GET_OVFL_COUNT       = 4,
    DT_DATAFIFO_CMD_GET_UFL_COUNT        = 5,
    DT_DATAFIFO_CMD_GET_PROPERTIES       = 6,
    DT_DATAFIFO_CMD_SET_DIRECTION        = 7,
    DT_DATAFIFO_CMD_SET_OPERATIONAL_MODE = 8,
}  DtIoctlDataFifoCmd;

// DT DATAFIFO RX/TX capability flags
#define DT_DATAFIFO_CAP_RX         1   // Receive capability
#define DT_DATAFIFO_CAP_TX         2   // Transmit capability

// DT DATAFIFO direction
#define DT_DATAFIFO_DIR_RX         0   // Receive 
#define DT_DATAFIFO_DIR_TX         1   // Transmit

// -.-.-.-.-.-.-.-.-.-.-.-.- DATAFIFO Command - Clear Fifo Max -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlDataFifoCmdClearFifoMaxInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_ClearMaxFree;        // Clear max free (boolean)
    Int  m_ClearMaxLoad;        // Clear max load (boolean)
}  DtIoctlDataFifoCmdClearFifoMaxInput;
ASSERT_SIZE(DtIoctlDataFifoCmdClearFifoMaxInput, 24)

// .-.-.-.-.-.-.-.-.-.-.- DATAFIFO Command - Get Direction Command -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlDataFifoCmdGetDirectionInput;
ASSERT_SIZE(DtIoctlDataFifoCmdGetDirectionInput, 16)
typedef struct _DtIoctlDataFifoCmdGetDirectionOutput
{
    Int  m_Direction;      // Direction

}  DtIoctlDataFifoCmdGetDirectionOutput;
ASSERT_SIZE(DtIoctlDataFifoCmdGetDirectionOutput, 4)

// -.-.-.-.-.-.-.-.-.-.- DATAFIFO Command - Get Fifo Status Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlDataFifoCmdGetFifoStatusInput;
ASSERT_SIZE(DtIoctlDataFifoCmdGetFifoStatusInput, 16)
typedef struct _DtIoctlDataFifoCmdGetFifoStatusOutput
{
    Int  m_CurLoad;     // Burst current fifo load in #bytes
    Int  m_MaxFree;     // Burst fifo maximum free space in #bytes
    Int  m_MaxLoad;     // Burst fifo maximum load in #bytes
}  DtIoctlDataFifoCmdGetFifoStatusOutput;
ASSERT_SIZE(DtIoctlDataFifoCmdGetFifoStatusOutput, 12)

// -.-.-.-.-.-.-.-.-.-.-.- DATAFIFO Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlDataFifoCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlDataFifoCmdGetOpModeInput, 16)
typedef struct _DtIoctlDataFifoCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlDataFifoCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlDataFifoCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.-.- DATAFIFO Command - Get Overflow Count Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlDataFifoCmdGetOvfCountInput;
ASSERT_SIZE(DtIoctlDataFifoCmdGetOvfCountInput, 16)
typedef struct _DtIoctlDataFifoCmdGetOvfCountOutput
{
    Int  m_OvfCount;      // Overflow count

}  DtIoctlDataFifoCmdGetOvfCountOutput;
ASSERT_SIZE(DtIoctlDataFifoCmdGetOvfCountOutput, 4)

// -.-.-.-.-.-.-.-.-.- DATAFIFO Command - Get Underflow Count Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlDataFifoCmdGetUflCountInput;
ASSERT_SIZE(DtIoctlDataFifoCmdGetUflCountInput, 16)
typedef struct _DtIoctlDataFifoCmdGetUflCountOutput
{
    Int  m_UflCount;      // Underflow count

}  DtIoctlDataFifoCmdGetUflCountOutput;
ASSERT_SIZE(DtIoctlDataFifoCmdGetUflCountOutput, 4)

// -.-.-.-.-.-.-.-.-.-.- DATAFIFO Command - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlDataFifoCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlDataFifoCmdGetPropertiesInput, 16)
typedef struct _DtIoctlDataFifoCmdGetPropertiesOutput
{
    UInt32  m_Capabilities;    // Data Fifo capability flags
    Int  m_FifoSize;           // Current FIFO size in #bytes
    Int  m_BurstSize;          // Burst size in #bytes
    Int  m_MaxFifoSize;        // Max fifo size in #bytes
    Int  m_DataWidth;          // DataWidth (#bits)
}  DtIoctlDataFifoCmdGetPropertiesOutput;
ASSERT_SIZE(DtIoctlDataFifoCmdGetPropertiesOutput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.-.- DATAFIFO Command - Set Direction -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlDataFifoCmdSetDirectionInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Direction;              // Direction
}  DtIoctlDataFifoCmdSetDirectionInput;
ASSERT_SIZE(DtIoctlDataFifoCmdSetDirectionInput, 20)

// -.-.-.-.-.-.-.-.-.-.-.- DATAFIFO Command - Set Operational Mode -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlDataFifoCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlDataFifoCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlDataFifoCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.- DATAFIFO Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// DATAFIFO command - IOCTL input data
typedef union _DtIoctlDataFifoCmdInput
{
    DtIoctlDataFifoCmdClearFifoMaxInput  m_ClearFifoMax; // Clear fifo max
    DtIoctlDataFifoCmdSetDirectionInput  m_SetDirection; // Set direction
    DtIoctlDataFifoCmdSetOpModeInput  m_SetOpMode;       // Set op mode
}  DtIoctlDataFifoCmdInput;

// DATAFIFO command - IOCTL output data
typedef union _DtIoctlDataFifoCmdOutput
{
    DtIoctlDataFifoCmdGetDirectionOutput  m_GetDirection;   // Get direction
    DtIoctlDataFifoCmdGetFifoStatusOutput  m_GetFifoStatus; // Get fifo status
    DtIoctlDataFifoCmdGetOpModeOutput  m_GetOpMode;         // Get opmode
    DtIoctlDataFifoCmdGetOvfCountOutput  m_GetOvfCnt;       // Get overflow count
    DtIoctlDataFifoCmdGetUflCountOutput  m_GetUflCnt;       // Get underflow count
    DtIoctlDataFifoCmdGetPropertiesOutput  m_GetProperties; // Get properties
}  DtIoctlDataFifoCmdOutput;

#ifdef WINBUILD
#define DT_IOCTL_DATAFIFO_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_DATAFIFO_CMD,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlDataFifoCmdInOut
{
    DtIoctlDataFifoCmdInput  m_Input;
    DtIoctlDataFifoCmdOutput  m_Output;
}  DtIoctlDataFifoCmdInOut;
#define DT_IOCTL_DATAFIFO_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_DATAFIFO_CMD,     \
                                                                  DtIoctlDataFifoCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_DDRFIFO_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DDRFIFO Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlDdrFifoCmd
{
    DT_DDRFIFO_CMD_GET_FIFO_LOAD        = 0,
    DT_DDRFIFO_CMD_GET_OPERATIONAL_MODE = 1,
    DT_DDRFIFO_CMD_GET_PROPERTIES       = 2,
    DT_DDRFIFO_CMD_SET_OPERATIONAL_MODE = 3,
}  DtIoctlDdrFifoCmd;

// .-.-.-.-.-.-.-.-.-.-.- DDRFIFO Command - Get Fifo Load Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlDdrFifoCmdGetFifoLoadInput;
ASSERT_SIZE(DtIoctlDdrFifoCmdGetFifoLoadInput, 16)
typedef struct _DtIoctlDdrFifoCmdGetFifoLoadOutput
{
    Int  m_FifoLoad;    // Current fifo load in #bytes
}  DtIoctlDdrFifoCmdGetFifoLoadOutput;
ASSERT_SIZE(DtIoctlDdrFifoCmdGetFifoLoadOutput, 4)

// -.-.-.-.-.-.-.-.-.-.-.- DDRFIFO Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlDdrFifoCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlDdrFifoCmdGetOpModeInput, 16)
typedef struct _DtIoctlDdrFifoCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlDdrFifoCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlDdrFifoCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.-.-.- DDRFIFO Command - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlDdrFifoCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlDdrFifoCmdGetPropertiesInput, 16)
typedef struct _DtIoctlDdrFifoCmdGetPropertiesOutput
{
    Int  m_FifoSize;           // Current FIFO size in #bytes
    Int  m_BurstSize;          // Burst size in #bytes
    Int  m_DataWidth;          // DataWidth (#bits)
}  DtIoctlDdrFifoCmdGetPropertiesOutput;
ASSERT_SIZE(DtIoctlDdrFifoCmdGetPropertiesOutput, 12)

// -.-.-.-.-.-.-.-.-.-.-.- DDRFIFO Command - Set Operational Mode -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlDdrFifoCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlDdrFifoCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlDdrFifoCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.- DDRFIFO Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// DDRFIFO command - IOCTL input data
typedef union _DtIoctlDdrFifoCmdInput
{
    DtIoctlDdrFifoCmdSetOpModeInput  m_SetOpMode;       // Set op mode
}  DtIoctlDdrFifoCmdInput;

// DDRFIFO command - IOCTL output data
typedef union _DtIoctlDdrFifoCmdOutput
{
    DtIoctlDdrFifoCmdGetFifoLoadOutput  m_GetFifoLoad;       // Get fifo load
    DtIoctlDdrFifoCmdGetOpModeOutput  m_GetOpMode;         // Get opmode
    DtIoctlDdrFifoCmdGetPropertiesOutput  m_GetProperties; // Get properties
}  DtIoctlDdrFifoCmdOutput;

#ifdef WINBUILD
#define DT_IOCTL_DDRFIFO_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_DDRFIFO_CMD,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlDdrFifoCmdInOut
{
    DtIoctlDdrFifoCmdInput  m_Input;
    DtIoctlDdrFifoCmdOutput  m_Output;
}  DtIoctlDdrFifoCmdInOut;
#define DT_IOCTL_DDRFIFO_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_DDRFIFO_CMD,     \
                                                                  DtIoctlDdrFifoCmdInOut)
#endif
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_CDMAC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlCDmaCCmd
{
    DT_CDMAC_CMD_ALLOCATE_BUFFER = 0,
    DT_CDMAC_CMD_CLEAR_REORDER_BUF_MIN_MAX = 1,
    DT_CDMAC_CMD_FREE_BUFFER = 2,
    DT_CDMAC_CMD_GET_REORDER_BUF_STATUS = 3,
    DT_CDMAC_CMD_GET_PROFILING = 4,
    DT_CDMAC_CMD_GET_PROPERTIES = 5,
    DT_CDMAC_CMD_GET_RX_WRITE_OFFSET = 6,
    DT_CDMAC_CMD_GET_STATUS = 7,
    DT_CDMAC_CMD_GET_TX_READ_OFFSET = 8,
    DT_CDMAC_CMD_ISSUE_CHANNEL_FLUSH = 9,
    DT_CDMAC_CMD_SET_OPERATIONAL_MODE = 10,
    DT_CDMAC_CMD_SET_RX_READ_OFFSET = 11,
    DT_CDMAC_CMD_SET_TEST_MODE = 12,
    DT_CDMAC_CMD_SET_TX_WRITE_OFFSET = 13,
    DT_CDMAC_CMD_CLEAR_PROFILING = 14,
}  DtIoctlCDmaCCmd;

// DT CDMAC test mode
#define DT_CDMAC_TESTMODE_NORMAL    0 // Normal mode
#define DT_CDMAC_TESTMODE_TEST_INT  1 // Test mode internal
#define DT_CDMAC_TESTMODE_TEST_EXT  2 // Test mode external

// DT CDMAC capability flags
#define DT_CDMAC_CAP_RX         1   // Receive capability
#define DT_CDMAC_CAP_TX         2   // Transmit capability

// DT CDMAC direction
#define DT_CDMAC_DIR_RX         0   // Receive 
#define DT_CDMAC_DIR_TX         1   // Transmit

// DT CDMAC status flags
#define DT_CDMAC_STATUS_WAIT_PCIE  1 // Waiting for data to/from the PCI Express bus 
#define DT_CDMAC_STATUS_WAIT_LOCAL 2 // Waiting for local channel to provide data


// DT CDMAC error flags
#define DT_CDMAC_ERROR_ALIGN      1 // Address pointer is not aligned to a 4Kbyte page
#define DT_CDMAC_ERROR_TIMEOUT    2 // DMA access timed out on PCIe bus
#define DT_CDMAC_ERROR_UNDEFINED  4 // DMA access ended with an undefined status

//-.-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Allocate Buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlCDmaCCmdAllocateBufferInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Direction;       // Direction: RX or TX
    Int  m_BufferSize;      // Allocated buffer size in #bytes
    UInt64A  m_BufferAddr;  // Buffer address (Linux only)
}  DtIoctlCDmaCCmdAllocateBufferInput;
ASSERT_SIZE(DtIoctlCDmaCCmdAllocateBufferInput, 32)
typedef struct _DtIoctlCDmaCCmdAllocateBufferOutput
{
    UInt8  m_Buf[4];            // Dummy dynamic sized buffer
                                // On Windows used for passing the DMA buffer;
                                // on Linux not used
}  DtIoctlCDmaCCmdAllocateBufferOutput;
ASSERT_SIZE(DtIoctlCDmaCCmdAllocateBufferOutput, 4)


// .-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Clear Reorder Buf Min Max -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlCDmaCCmdClearReorderBufMinMaxInput;
ASSERT_SIZE(DtIoctlCDmaCCmdClearReorderBufMinMaxInput, 16)
//-.-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Clear Profiling -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlCDmaCCmdClearProfiling;
ASSERT_SIZE(DtIoctlCDmaCCmdClearProfiling, 16)
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - FreeBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlCDmaCCmdFreeBufferInput;
ASSERT_SIZE(DtIoctlCDmaCCmdFreeBufferInput, 16)

//.-.-.-.-.-.-.-.-.-.- CDMAC Command - Get ReorderBuf Status Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlCDmaCCmdGetReorderBufStatusInput;
ASSERT_SIZE(DtIoctlCDmaCCmdGetReorderBufStatusInput, 16)
typedef struct _DtIoctlCDmaCCmdGetReorderBufStatusOutput
{
    Int  m_ReorderBufLoad;          // Reorder buffer load in #bytes
    Int  m_ReorderBufMinMaxLoad;    // Reorder buffer minimum/maximum load in #bytes
}  DtIoctlCDmaCCmdGetReorderBufStatusOutput;
ASSERT_SIZE(DtIoctlCDmaCCmdGetReorderBufStatusOutput, 8)
//.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Get Profiling Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlCDmaCCmdGetProfilingInput;
ASSERT_SIZE(DtIoctlCDmaCCmdGetProfilingInput, 16)
typedef struct _DtIoctlCDmaCCmdGetProfilingOutput
{
    UInt64A  m_MaxSpeed;        // Maximum transfer speed in bps (only in Testmode) 
    UInt64A  m_MinSpeed;        // Minimum transfer speed in bps (only in Testmode) 
    UInt64A  m_AvgSpeed;        // Average transfer speed in bps (only in Testmode) 
    UInt32  m_PerfCount;        // Performance counter
}  DtIoctlCDmaCCmdGetProfilingOutput;
ASSERT_SIZE(DtIoctlCDmaCCmdGetProfilingOutput, 32)
//.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlCDmaCCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlCDmaCCmdGetPropertiesInput, 16)
typedef struct _DtIoctlCDmaCCmdGetPropertiesOutput
{
    UInt32  m_Capabilities;     // DMA capability flags
    Int  m_PrefetchSize;        // DMA prefetch size
    Int  m_PcieDataWidth;       // PCIe interface data width in #bits
    Int  m_ReorderBufSize;       // Burst fifo size in #bytes
}  DtIoctlCDmaCCmdGetPropertiesOutput;
ASSERT_SIZE(DtIoctlCDmaCCmdGetPropertiesOutput, 16)
//-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Get Rx Write Offset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlCDmaCCmdGetRxWrOffsetInput;
ASSERT_SIZE(DtIoctlCDmaCCmdGetRxWrOffsetInput, 16)
typedef struct _DtIoctlCDmaCCmdGetRxWrOffsetOutput
{
    UInt  m_RxWriteOffset;      //  Rx write offset
}  DtIoctlCDmaCCmdGetRxWrOffsetOutput;
ASSERT_SIZE(DtIoctlCDmaCCmdGetRxWrOffsetOutput, 4)
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Get Status -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlCDmaCCmdGetStatusInput;
ASSERT_SIZE(DtIoctlCDmaCCmdGetStatusInput, 16)
typedef struct _DtIoctlCDmaCCmdGetStatusOutput
{
    Int  m_OpStatus;            // Operational status
    UInt32  m_StatusFlags;      // Status flags
    UInt32  m_ErrorFlags;       // Error flags
}  DtIoctlCDmaCCmdGetStatusOutput;
ASSERT_SIZE(DtIoctlCDmaCCmdGetStatusOutput, 12)
//.-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Get Tx Read Offset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlCDmaCCmdGetTxRdOffsetInput;
ASSERT_SIZE(DtIoctlCDmaCCmdGetTxRdOffsetInput, 16)
typedef struct _DtIoctlCDmaCCmdGetTxRdOffsetOutput
{
    UInt  m_TxReadOffset;      //  Tx read offset
}  DtIoctlCDmaCCmdGetTxRdOffsetOutput;
ASSERT_SIZE(DtIoctlCDmaCCmdGetTxRdOffsetOutput, 4)
//-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Issue Channel Flush -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlCDmaCCmdIssueChannelFlushInput;
ASSERT_SIZE(DtIoctlCDmaCCmdIssueChannelFlushInput, 16)
//-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Set Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlCDmaCCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlCDmaCCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlCDmaCCmdSetOpModeInput, 20)
//.-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Set Rx Read Offset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlCDmaCCmdSetRxRdOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt  m_RxReadOffset;       // Rx read offset
}  DtIoctlCDmaCCmdSetRxRdOffsetInput;
ASSERT_SIZE(DtIoctlCDmaCCmdSetRxRdOffsetInput, 20)
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Set Test Mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlCDmaCCmdSetTestModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_TestMode;              // Test mode
}  DtIoctlCDmaCCmdSetTestModeInput;
ASSERT_SIZE(DtIoctlCDmaCCmdSetTestModeInput, 20)
//-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - Set Tx Write Offset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlCDmaCCmdSetTxWrOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt  m_TxWriteOffset;      // Tx write offset
}  DtIoctlCDmaCCmdSetTxWrOffsetInput;
ASSERT_SIZE(DtIoctlCDmaCCmdSetTxWrOffsetInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- CDMAC Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-
// CDMAC command - IOCTL input data
typedef union _DtIoctlCDmaCCmdInput
{
    DtIoctlCDmaCCmdAllocateBufferInput  m_AllocBuf;     // CDmaC - Allocate buffer
    DtIoctlCDmaCCmdSetOpModeInput  m_SetOpMode;         // CDmaC - Set mode
    DtIoctlCDmaCCmdSetRxRdOffsetInput  m_SetRxRdOffset; // CDmaC - Set Rx read offset
    DtIoctlCDmaCCmdSetTestModeInput  m_SetTestMode;     // CDmaC - Set test mode
    DtIoctlCDmaCCmdSetTxWrOffsetInput  m_SetTxWrOffset; // CDmaC - Set Tx write offset
}  DtIoctlCDmaCCmdInput;
// CDMAC command - IOCTL output data
typedef union _DtIoctlCDmaCCmdOutput
{
    DtIoctlCDmaCCmdGetReorderBufStatusOutput  m_GetBufStatus;  // CDmaC - Get buf status
    DtIoctlCDmaCCmdGetProfilingOutput  m_GetProfiling;      // CDmaC - Get profiling
    DtIoctlCDmaCCmdGetPropertiesOutput  m_GetProperties;    // CDmaC - Get properties
    DtIoctlCDmaCCmdGetRxWrOffsetOutput  m_GetRxWrOffset;    // CDmaC - Get Rx write offset
    DtIoctlCDmaCCmdGetStatusOutput  m_GetStatus;            // CDmaC - Get status
    DtIoctlCDmaCCmdGetTxRdOffsetOutput  m_GetTxRdOffset;    // CDmaC - Get Tx read offset
}  DtIoctlCDmaCCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_CDMAC_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_CDMAC_CMD,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlCDmaCCmdInOut
    {
        DtIoctlCDmaCCmdInput  m_Input;
        DtIoctlCDmaCCmdOutput  m_Output;
    }  DtIoctlCDmaCCmdInOut;
    #define DT_IOCTL_CDMAC_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_CDMAC_CMD,       \
                                                                     DtIoctlCDmaCCmdInOut)
#endif

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_DATAC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq unpacker Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlDataCCmd
{
    DT_DATAC_CMD_GET_OPERATIONAL_MODE = 0, // Get operational mode
    DT_DATAC_CMD_SET_OPERATIONAL_MODE = 1, // Set operational mode
}  DtIoctlDataCCmd;

// .-.-.-.-.-.-.-.-.-.- DATAC Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlDataCCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlDataCCmdGetOpModeInput, 16)
typedef struct _DtIoctlDataCCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlDataCCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlDataCCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.-.- DATAC Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlDataCCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlDataCCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlDataCCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.-.- DATAC Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// DATAC command - IOCTL input data
typedef union _DtIoctlDataCCmdInput
{
    DtIoctlDataCCmdSetOpModeInput  m_SetOpMode;            // DataC - Set op. mode
}  DtIoctlDataCCmdInput;


// DATAC command - IOCTL output data
typedef union _DtIoctlDataCCmdOutput
{
    DtIoctlDataCCmdGetOpModeOutput  m_GetOpMode;           // DataC - Get op. mode
}  DtIoctlDataCCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_DATAC_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_DATAC_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlDataCCmdInOut
    {
        DtIoctlDataCCmdInput  m_Input;
        DtIoctlDataCCmdOutput  m_Output;
    }  DtIoctlDataCCmdInOut;
#define DT_IOCTL_DATAC_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_DATAC_CMD,       \
                                                                     DtIoctlDataCCmdInOut)
#endif

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_DATACDC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq unpacker Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlDataCdcCmd
{
    DT_DATACDC_CMD_GET_OPERATIONAL_MODE = 0, // Get operational mode
    DT_DATACDC_CMD_SET_OPERATIONAL_MODE = 1, // Set operational mode
}  DtIoctlDataCdcCmd;

// -.-.-.-.-.-.-.-.-.- DATACDC Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlDataCdcCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlDataCdcCmdGetOpModeInput, 16)
typedef struct _DtIoctlDataCdcCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlDataCdcCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlDataCdcCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.-.- DATACDC Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlDataCdcCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlDataCdcCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlDataCdcCmdSetOpModeInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- DATACDC Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// DATACDC command - IOCTL input data
typedef union _DtIoctlDataCdcCmdInput
{
    DtIoctlDataCdcCmdSetOpModeInput  m_SetOpMode;            // DataCdc - Set op. mode
}  DtIoctlDataCdcCmdInput;


// DATACDC command - IOCTL output data
typedef union _DtIoctlDataCdcCmdOutput
{
    DtIoctlDataCdcCmdGetOpModeOutput  m_GetOpMode;           // DataCdc - Get op. mode
}  DtIoctlDataCdcCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_DATACDC_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_DATACDC_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlDataCdcCmdInOut
    {
        DtIoctlDataCdcCmdInput  m_Input;
        DtIoctlDataCdcCmdOutput  m_Output;
    }  DtIoctlDataCdcCmdInOut;
#define DT_IOCTL_DATACDC_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_DATACDC_CMD,       \
                                                                   DtIoctlDataCdcCmdInOut)
#endif

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_DEBUG_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Debug Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlDebugCmd
{
    DT_DEBUG_CMD_BULK_READ = 0,
    DT_DEBUG_CMD_REGISTER_READ = 1,
    DT_DEBUG_CMD_REGISTER_WRITE = 2,
    DT_DEBUG_CMD_FORCE_RELEASE = 3,
}  DtIoctlDebugCmd;

typedef struct  _DtIoctlRegAddressValuePair
{
    UInt32  m_Address;          // Address offset of the register
    UInt32  m_Value;            // Value of the register
}  DtIoctlRegAddressValuePair;


//.-.-.-.-.-.-.-.-.-.-.-.-.- Debug Command - Bulk Read Command -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlDebugCmdBulkReadInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumToRead;           // Number of register to read
    UInt32  m_StartAddress;     // Address of first register to be read
}  DtIoctlDebugCmdBulkReadInput;
ASSERT_SIZE(DtIoctlDebugCmdBulkReadInput, 24)

typedef struct _DtIoctlDebugCmdBulkReadOutput
{
    Int  m_NumRead;             // Number of register values read
    UInt64A  m_Dummy;           // Only here for alignment reason
    UInt32  m_RegValues[0];     // List of register values
}  DtIoctlDebugCmdBulkReadOutput;
ASSERT_SIZE(DtIoctlDebugCmdBulkReadOutput, 16)

//.-.-.-.-.-.-.-.-.-.-.-.- Debug Command - Read Register Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlDebugCmdRegReadInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumToRead;           // Number of register to read
    UInt32  m_Address[0];       // List of register addresses to be read
}  DtIoctlDebugCmdRegReadInput;
ASSERT_SIZE(DtIoctlDebugCmdRegReadInput, 20)

typedef struct _DtIoctlDebugCmdRegReadOutput
{
    Int  m_NumRead;             // Number of address+value pairs read
    DtIoctlRegAddressValuePair  m_Regs[0];  // List of address+value pairs
}  DtIoctlDebugCmdRegReadOutput;
ASSERT_SIZE(DtIoctlDebugCmdRegReadOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.- Debug Command - Write Register Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlDebugCmdRegWriteInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumToWrite;          // Number of registers to write
    DtIoctlRegAddressValuePair  m_Regs[0];  // List of address+value pairs to write
}  DtIoctlDebugCmdRegWriteInput;
ASSERT_SIZE(DtIoctlDebugCmdRegWriteInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.- Debug Command - Force Release Command -.-.-.-.-.-.-.-.-.-.-.-.
//
// Special command to force all users to release their access hold on a specific port.
//
typedef struct _DtIoctlDebugCmdForceReleaseInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_ReleasePort;                   // Index of port to release resouces for.
}  DtIoctlDebugCmdForceReleaseInput;
ASSERT_SIZE(DtIoctlDebugCmdForceReleaseInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- Debug Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Debug command - IOCTL input data
typedef union _DtIoctlDebugCmdInput
{
    DtIoctlDebugCmdBulkReadInput  m_BulkRead;       // Debug - Bulk read command
    DtIoctlDebugCmdRegReadInput  m_RegRead;         // Debug - Read register command
    DtIoctlDebugCmdRegWriteInput  m_RegWrite;       // Debug - Write register command
    DtIoctlDebugCmdForceReleaseInput m_ForceRelease; // Debug - Force release command
}  DtIoctlDebugCmdInput;

// Debug command - IOCTL output data
typedef union _DtIoctlDebugCmdOutput
{
    DtIoctlDebugCmdBulkReadOutput  m_BulkRead;      // Debug - Bulk read command
    DtIoctlDebugCmdRegReadOutput  m_RegRead;        // Debug - Read register command
}  DtIoctlDebugCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_DEBUG_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_DEBUG_CMD,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlDebugCmdInOut
    {
        DtIoctlDebugCmdInput  m_Input;
        DtIoctlDebugCmdOutput  m_Output;
    }  DtIoctlDebugCmdInOut;

    #define DT_IOCTL_DEBUG_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_DEBUG_CMD,       \
                                                                     DtIoctlDebugCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_DISEQC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DISEQC Interface Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
    typedef enum _DtIoctlDiseqcCmd
    {
        DT_DISEQC_CMD_SEND_TONEBURST = 0,
        DT_DISEQC_CMD_SEND_MESSAGE = 1,
        DT_DISEQC_CMD_SEND_RECEIVE_MESSAGE = 2,
        DT_DISEQC_CMD_ENABLE_TONE = 3

    }  DtIoctlDiseqcCmd;
    // Tone burst
#define DT_DISEQC_TONEBURST_A      0x0  // Tone burst A
#define DT_DISEQC_TONEBURST_B      0x1  // Tone burst B

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DISEQC - Enable Tone +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
    typedef struct _DtIoctlDiseqcCmd_EnableToneInput
    {
        DtIoctlInputDataHdr  m_CmdHdr;
        Int  m_EnableTone;            // Enable 22 kHz tone (bool)
    }  DtIoctlDiseqcCmd_EnableToneInput;
    ASSERT_SIZE(DtIoctlDiseqcCmd_EnableToneInput, 20)

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- DISEQC - Send Burst Tone -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    //
    typedef struct _DtIoctlDiseqcCmd_SendToneBurstInput
    {
        DtIoctlInputDataHdr  m_CmdHdr;
        Int  m_ToneBurst;            // ToneBurst
    }  DtIoctlDiseqcCmd_SendToneBurstInput;
    ASSERT_SIZE(DtIoctlDiseqcCmd_SendToneBurstInput, 20)
    // .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DISEQC - Send Message -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    //
    typedef struct _DtIoctlDiseqcCmd_SendMessageInput
    {
        DtIoctlInputDataHdr  m_CmdHdr;
        Int  m_NumToSend;           // Number of bytes in send buffer
        UInt8  m_Buf[0];            // Dynamic sized buffer
    }  DtIoctlDiseqcCmd_SendMessageInput;
    ASSERT_SIZE(DtIoctlDiseqcCmd_SendMessageInput, 20)
    // .-.-.-.-.-.-.-.-.-.-.-.-.- DISEQC - Send Receive Message -.-.-.-.-.-.-.-.-.-.-.-.-.
    //
    typedef struct _DtIoctlDiseqcCmd_SendReceiveMessageInput
    {
        DtIoctlInputDataHdr  m_CmdHdr;
        Int  m_NumToSend;          // Number of bytes in send buffer
        Int  m_RcvBufSize;         // Receive buffer size 
        UInt8  m_Buf[0];           // Dynamic sized buffer
    }  DtIoctlDiseqcCmd_SendReceiveMessageInput;
    ASSERT_SIZE(DtIoctlDiseqcCmd_SendReceiveMessageInput, 24)
    typedef struct _DtIoctlDiseqcCmd_SendReceiveMessageOutput
    {
        Int  m_NumRcvd;             // Number of bytes received in buffer
        UInt8  m_Buf[0];            // Dynamic sized buffer
    }  DtIoctlDiseqcCmd_SendReceiveMessageOutput;
    ASSERT_SIZE(DtIoctlDiseqcCmd_SendReceiveMessageOutput, 4)
    //-.-.-.-.-.-.-.-.-.-.-.- DISEQC Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
    // DISEQC command - IOCTL input data
    typedef union _DtIoctlDiseqcCmdInput
    {
        DtIoctlDiseqcCmd_EnableToneInput    m_EnableTone;       // Enable 22 kHz tone
        DtIoctlDiseqcCmd_SendToneBurstInput  m_SndToneBurst;    // Send tone burst
        DtIoctlDiseqcCmd_SendMessageInput   m_SndMsg;           // Send message
        DtIoctlDiseqcCmd_SendReceiveMessageInput  m_SndRcvMsg;  // Send/receive message
    }  DtIoctlDiseqcCmdInput;
    // DISEQC command - IOCTL output data
    typedef union _DtIoctlDiseqcCmdOutput
    {
        DtIoctlDiseqcCmd_SendReceiveMessageOutput  m_SndRcvMsg;  // Send/receive message
    }  DtIoctlDiseqcCmdOutput;
#ifdef WINBUILD
#define DT_IOCTL_DISEQC_CMD  CTL_CODE(DT_DEVICE_TYPE,                                    \
                                                        DT_FUNC_CODE_DISEQC_CMD,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlDiseqcCmd_InOut
    {
        DtIoctlDiseqcCmdInput  m_Input;
        DtIoctlDiseqcCmdOutput  m_Output;
    }  DtIoctlDiseqcCmd_InOut;
#define DT_IOCTL_DISEQC_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                                  \
                                                        DT_FUNC_CODE_DISEQC_CMD,         \
                                                        DtIoctlDiseqcCmd_InOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_EVENT_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EVENT Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlEventCmd
{
    DT_EVENT_CMD_GET_EVENT = 0,
    DT_EVENT_CMD_REGISTER  = 1,
    DT_EVENT_CMD_SET_EVENT = 2,     // For testing 
    DT_EVENT_CMD_UNREGISTER = 3,
}  DtIoctlEventCmd;

//.-.-.-.-.-.-.-.-.-.-.-.-.- EVENT Command - Get Event Command -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlEventCmdGetEventInput;
ASSERT_SIZE(DtIoctlEventCmdGetEventInput, 16)
typedef struct _DtIoctlEventCmdGetEventOutput
{
    UInt32  m_EventType;            // Event type, device category specific meaning
    UInt32  m_EventValue1;          // Event value 1, device category specific meaning
    UInt32  m_EventValue2;          // Event value 2, device category specific meaning
}  DtIoctlEventCmdGetEventOutput;
ASSERT_SIZE(DtIoctlEventCmdGetEventOutput, 12)
//.-.-.-.-.-.-.-.-.-.-.-.- EVENT Command - Register Event Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlEventCmdRegisterEventInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt32  m_EventType;            // Event type, device category specific meaning
}  DtIoctlEventCmdRegisterEventInput;
ASSERT_SIZE(DtIoctlEventCmdRegisterEventInput, 20)
//.-.-.-.-.-.-.-.-.-.-.-.- EVENT Command - Register Event Command -.-.-.-.-.-.-.-.-.-.-.-.
//
// For event testing 
//
typedef struct _DtIoctlEventCmdSetEventInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt32  m_EventType;            // Event type, device category specific meaning
    UInt32  m_EventValue1;          // Event value 1, device category specific meaning
    UInt32  m_EventValue2;          // Event value 2, device category specific meaning
    Int  m_AddIfExists;             // Add event if it already exists
}  DtIoctlEventCmdSetEventInput;
ASSERT_SIZE(DtIoctlEventCmdSetEventInput, 32)
//-.-.-.-.-.-.-.-.-.-.-.- EVENT Command - Unregister Event Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlEventCmdUnregisterEventInput;
ASSERT_SIZE(DtIoctlEventCmdUnregisterEventInput, 16)
//.-.-.-.-.-.-.-.-.-.-.-.-.- EVENT Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-
// EVENT command - IOCTL input data
typedef union _DtIoctlEventCmdInput
{
    DtIoctlEventCmdRegisterEventInput  m_RegEvent;      // EVENT - Register event
    DtIoctlEventCmdSetEventInput  m_SetEvent;           // EVENT - Set event
    DtIoctlEventCmdUnregisterEventInput  m_UnregEvent;  // EVENT - Unregister event
}  DtIoctlEventCmdInput;
// EVENT command - IOCTL output data
typedef union _DtIoctlEventCmdOutput
{
    DtIoctlEventCmdGetEventOutput  m_GetEvent; // EVENT - Get event
}  DtIoctlEventCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_EVENT_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_EVENT_CMD,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
// Since EVENT-IOCTL are treated in a special way in DTAPI, we cannot use
// DT_IOCTL_MAGIC_SIZE here, 
    typedef union _DtIoctlEventCmdInOut
    {
        DtIoctlEventCmdInput  m_Input;
        DtIoctlEventCmdOutput  m_Output;
    }  DtIoctlEventCmdInOut;
    #define DT_IOCTL_EVENT_CMD  _IOWR(DT_IOCTL_MAGIC,                                    \
                                             DT_FUNC_CODE_EVENT_CMD, DtIoctlEventCmdInOut)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DRIVER EVENTS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//

// NOTE: ALWAYS ADD NEW EVENTS AND VALUES TO END FOR BACKWARDS COMPATIBILITY.

// Event types
#define DT_DRV_EVENT_TYPE_POWER         0x00000001
#define DT_DRV_EVENT_TYPE_GENLOCK       0x00000002
#define DT_DRV_EVENT_TYPE_IOCONFIG      0x00000004
#define DT_DRV_EVENT_TYPE_ALL           0xFFFFFFFF

// Power event values
#define DT_DRV_EVENT_VALUE1_POWER_DOWN  0x00000001
#define DT_DRV_EVENT_VALUE1_POWER_UP    0x00000002

// Genlock event values
#define DT_DRV_EVENT_VALUE1_NO_LOCK     0x00000001
#define DT_DRV_EVENT_VALUE1_LOCKED      0x00000002


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_EXCL_ACCESS_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Exclusive Access Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlExclAccessCmd
{
    DT_EXCLUSIVE_ACCESS_CMD_ACQUIRE = 0,
    DT_EXCLUSIVE_ACCESS_CMD_RELEASE = 1,
    DT_EXCLUSIVE_ACCESS_CMD_PROBE = 2,
    DT_EXCLUSIVE_ACCESS_CMD_CHECK = 3,
}  DtIoctlExclAccessCmd;

// Exclusive Access Command - IOCTL input data
typedef DtIoctlInputDataHdr DtIoctlExclAccessCmdInput;
ASSERT_SIZE(DtIoctlExclAccessCmdInput, 16)

// NO OUTPUT DATA

#ifdef WINBUILD
    #define DT_IOCTL_EXCL_ACCESS_CMD  CTL_CODE(DT_DEVICE_TYPE,                           \
                                                        DT_FUNC_CODE_EXCL_ACCESS_CMD,    \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlExclAccessCmdInOut
    {
        DtIoctlExclAccessCmdInput  m_Input;
    }  DtIoctlExclAccessCmdInOut;

    #define DT_IOCTL_EXCL_ACCESS_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                         \
                                                         DT_FUNC_CODE_EXCL_ACCESS_CMD,   \
                                                         DtIoctlExclAccessCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_GENLOCKCTRL_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- GENLOCKCTRL commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlGenLockCtrlCmd
{
    DT_GENLOCKCTRL_CMD_GET_STATE         = 0,  // Get state
    DT_GENLOCKCTRL_CMD_RELOCK            = 1,  // Re-lock
    // UNUSED DT_GENLOCKCTRL_CMD_GET_DCO_FREQ_OFFSET    = 2,
    // UNUSED DT_GENLOCKCTRL_CMD_SET_DCO_FREQ_OFFSET    = 3,
    DT_GENLOCKCTRL_CMD_GET_DCO_CLK_PROPS    = 4,  // Get DCO clock propties
    DT_GENLOCKCTRL_CMD_GET_DCO_FREQ_OFFSET  = 5,  // Get DCO frequency offset
    DT_GENLOCKCTRL_CMD_SET_DCO_FREQ_OFFSET  = 6,  // Set DCO frequency offset
    DT_GENLOCKCTRL_CMD_GET_STATE2           = 7,  // Get state v2
}  DtIoctlGenLockCtrlCmd;

// GenLock states
#define  DT_GENLOCKCTRL_STATE_NO_REF        0
#define  DT_GENLOCKCTRL_STATE_INVALID_REF   1
#define  DT_GENLOCKCTRL_STATE_LOCKING       2
#define  DT_GENLOCKCTRL_STATE_LOCKED        3
#define  DT_GENLOCKCTRL_STATE_FREE_RUN      4

// Clock properties:clock types
#define  DT_GENLOCKCTR_CLKTYPE_NON_FRACTIONAL    0
#define  DT_GENLOCKCTR_CLKTYPE_FRACTIONAL    1

// Clock properties
typedef struct _DtIoctlGenLockCtrlClockProps
{
    Int  m_ClockIdx;                // Clock index
    Int  m_ClockType;               // Clock type (Fractional/Non-fractional)
    Int  m_StepSizePpt;             // Minimum step size in  parts per trillion
    Int  m_RangePpt;                // Frequency range in parts per trillion 
    Int64A  m_FrequencyuHz;         // Frequency in micro Hertz
} DtIoctlGenLockCtrlClockProps;


// .-.-.-.-.-.-.-.-.-.-.-.-.- GENLOCKCTRL - Get State Command -.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtIoctlGenLockCtrlCmdGetStateInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlGenLockCtrlCmdGetStateInput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdGetStateInput, 16)

typedef struct _DtIoctlGenLockCtrlCmdGetStateOutput
{
    Int  m_GenLockState;   // GenLock status
    Int  m_RefVidStd;       // Reference video standard
    Int  m_DetVidStd;       // Detected video standard
}  DtIoctlGenLockCtrlCmdGetStateOutput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdGetStateOutput, 12)

// .-.-.-.-.-.-.-.-.-.-.-.-.- GENLOCKCTRL - Get State Command -.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtIoctlGenLockCtrlCmdGetState2Input
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlGenLockCtrlCmdGetState2Input;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdGetState2Input, 16)

typedef struct _DtIoctlGenLockCtrlCmdGetState2Output
{
    Int  m_GenLockState;   // GenLock status
    Int  m_RefVidStd;       // Reference video standard
    Int  m_DetVidStd;       // Detected video standard
    UInt64A m_SofCount;     // Sequence number of last SOF detected at the genlock 
                            // reference input
    Int  m_IsSofValid;      // Is SOF timestamp valid TRUE or FALSE
    DtTodTime m_SofTime;    // Time of the last Start-of-Frame detected at the input
    Int m_TimeSinceLastSof; // Time (in ns) expired since last SOF 

}  DtIoctlGenLockCtrlCmdGetState2Output;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdGetState2Output, 40)

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- GENLOCKCTRL - Re-Lock Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlGenLockCtrlCmdReLockInput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdReLockInput, 16)

// -.-.-.-.-.-.-.-.-.-.-.- GENLOCKCTRL - Get DCO Clock properties -.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtIoctlGenLockCtrlCmdGetDcoClockPropsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_MaxNumEntries;
} DtIoctlGenLockCtrlCmdGetDcoClockPropsInput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdGetDcoClockPropsInput, 20)

typedef struct _DtIoctlGenLockCtrlCmdGetDcoClockPropsOutput
{
    Int  m_NumEntries;
    UInt64A  m_Dummy;           // Only here for alignment reason
    DtIoctlGenLockCtrlClockProps  m_Properties[0];  // Dynamic sized buffer
} DtIoctlGenLockCtrlCmdGetDcoClockPropsOutput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdGetDcoClockPropsOutput, 16)

// -.-.-.-.-.-.-.-.-.-.-.- GENLOCKCTRL - Get DCO Frequency Offset -.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtIoctlGenLockCtrlCmdGetDcoFreqOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_ClockIdx;            // Clock index 
}  DtIoctlGenLockCtrlCmdGetDcoFreqOffsetInput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdGetDcoFreqOffsetInput, 20)

typedef struct _DtIoctlGenLockCtrlCmdGetDcoFreqOffsetOutput
{
    Int64A  m_DcoFrequencyMicroHz;      // DCO frequency in micro Hz
    Int  m_DcoFreqOffsetPpt;            // DCO frequency offset in ppt
}  DtIoctlGenLockCtrlCmdGetDcoFreqOffsetOutput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdGetDcoFreqOffsetOutput, 16)

// -.-.-.-.-.-.-.-.-.-.-.- GENLOCKCTRL - Set DCO Frequency Offset -.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtIoctlGenLockCtrlCmdSetDcoFreqOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_ClockIdx;            // Clock index 
    Int  m_DcoFreqOffsetPpt;    // DCO frequency offset in ppt
}  DtIoctlGenLockCtrlCmdSetDcoFreqOffsetInput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdSetDcoFreqOffsetInput, 24)


// -.-.-.-.-.-.-.-.-.-.-.- GENLOCKCTRL command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
// GENLOCKCTRL command - Input data
typedef union _DtIoctlGenLockCtrlCmdInput
{
    DtIoctlGenLockCtrlCmdGetDcoClockPropsInput  m_GetDcoClkProps;
    DtIoctlGenLockCtrlCmdGetDcoFreqOffsetInput  m_GetDcoFreqOffset;
    DtIoctlGenLockCtrlCmdSetDcoFreqOffsetInput  m_SetDcoFreqOffset;
} DtIoctlGenLockCtrlCmdInput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdInput, 24)

// GENLOCKCTRL command - Output data
typedef union _DtIoctlGenLockCtrlCmdOutput
{
    DtIoctlGenLockCtrlCmdGetStateOutput  m_GetState;
    DtIoctlGenLockCtrlCmdGetState2Output  m_GetState2;
    DtIoctlGenLockCtrlCmdGetDcoClockPropsOutput  m_GetDcoClkProps;
    DtIoctlGenLockCtrlCmdGetDcoFreqOffsetOutput  m_GetDcoFreqOffset;
}  DtIoctlGenLockCtrlCmdOutput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdOutput, 40)


#ifdef WINBUILD
    #define DT_IOCTL_GENLOCKCTRL_CMD  CTL_CODE(DT_DEVICE_TYPE,                           \
                                                        DT_FUNC_CODE_GENLOCKCTRL_CMD,    \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlGenLockCtrlCmdInOut {
        DtIoctlGenLockCtrlCmdInput  m_Input;
        DtIoctlGenLockCtrlCmdOutput  m_Output;
    } DtIoctlGenLockCtrlCmdInOut;

    #define DT_IOCTL_GENLOCKCTRL_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                         \
                                                         DT_FUNC_CODE_GENLOCKCTRL_CMD,   \
                                                         DtIoctlGenLockCtrlCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_GET_DEV_INFO +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

typedef struct  _DtDrvFwBuildDateTime
{
    UInt16  m_Year;
    UInt8  m_Month;
    UInt8  m_Day;
    UInt8  m_Hour;
    UInt8  m_Minute;
} DtDrvFwBuildDateTime;

// Firmware status
#define  DT_FWSTATUS_UNDEFINED -1       // Status of the firmware cannot be defined
#define  DT_FWSTATUS_UPTODATE   0       // Firmware is the latest released version 
                                        // supported by the current driver
#define  DT_FWSTATUS_BETA       1       // Firmware is the latest version supported by the
                                        // current driver but not yet released
#define  DT_FWSTATUS_OLD        2       // Firmware is not the latest version
#define  DT_FWSTATUS_NEW        3       // Firmware is newer than supported by the current
                                        // driver
#define  DT_FWSTATUS_TAINTED    4       // Firmware is an intermediate version which is 
                                        // not supported by the current driver
#define  DT_FWSTATUS_OBSOLETE   5       // Firmware version is not supported anymore
#define  DT_FWSTATUS_FAILED     DT_FWSTATUS_TAINTED
                                        // Driver failed to initialize the firmware

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoctlDevInfoPcie -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// PCIE specific device info
typedef struct _DtIoctlDevInfoPcie
{
    Int  m_BusNumber;           // PCI-bus number
    Int  m_SlotNumber;          // PCI-slot number
    Int  m_PcieNumLanes;        // Number of allocated PCIe lanes
    Int  m_PcieMaxLanes;        // Maximum number of PCIe lanes
    Int  m_PcieLinkSpeed;       // Current PCIe link speed
    Int  m_PcieMaxSpeed;        // Maximum PCIe link speed
    Int  m_PcieMaxPayloadSize;  // Maximum-Payload-Size (MPS)
    Int  m_PcieMaxReadRequestSize;  // Maximum-Read-Request-Size (MRRS)
}  DtIoctlDevInfoPcie;
ASSERT_SIZE(DtIoctlDevInfoPcie, 32)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoctlDevInfoUsb -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// USB specific device info
typedef struct _DtIoctlDevInfoUsb
{
    UInt  m_UsbAddress;         // USB address assigned to device
    Int  m_UsbSpeed;            // USB Speed
}  DtIoctlDevInfoUsb;
ASSERT_SIZE(DtIoctlDevInfoUsb, 8)


// Get Device Info - IOCTL input data
typedef DtIoctlInputDataHdr DtIoctlGetDevInfoInput;
ASSERT_SIZE(DtIoctlGetDevInfoInput, 16)

// Get Device Info - IOCTL output data
#define DT_IOCTL_GET_DEV_INFO_COMMON_OUTPUT_DATA                                         \
    Int  m_TypeNumber;          /* Type number in decimal, e.g. 2160 for DTA-2160 */     \
    Int  m_SubType;             /* Device subtype (0=none, 1=A, ...) */                  \
    Int  m_SubDvc;              /* Sub-device: 0=master, 1=slave1, 2=slave2, etc */      \
    UInt64A  m_Serial;          /* Serial number */                                      \
    Int  m_HardwareRevision;    /* Hardware Revision */                                  \
    Int  m_FirmwareVersion;     /* Firmware Version (= Altera revision), e.g. 3 for */   \
                                /* "Firmware Version 3" */                               \
    Int  m_FirmwareVariant;     /* Firmware Variant, e.g. to distinguish between */      \
                                /* firmware with different #inputs/#outputs */           \
    Int  m_FwPackageVersion;    /* Firmware Package version, version number given */     \
                                /* to the set of firmware variants; -1= NA */            \
    Int  m_FirmwareStatus;      /* Status of the firmware/driver combination */          \
    DtDrvFwBuildDateTime  m_FwBuildDate; /* Firmware build date */                       \
    UInt16  m_VendorId;         /* Vendor ID */                                          \
    UInt16  m_DeviceId;         /* Device ID */                                          \
    UInt16  m_SubVendorId;      /* Subsystem Vendor ID */                                \
    UInt16  m_SubSystemId       /* Subsystem ID */
typedef struct _DtIoctlGetDevInfoCommonOutput
{
    DT_IOCTL_GET_DEV_INFO_COMMON_OUTPUT_DATA;
}  DtIoctlGetDevInfoCommonOutput;
ASSERT_SIZE(DtIoctlGetDevInfoCommonOutput, 64)

typedef struct _DtIoctlGetDevInfoOutput
{
    // Common device info
    DT_IOCTL_GET_DEV_INFO_COMMON_OUTPUT_DATA;
    
    // Device category specific info
    union
    {
        DtIoctlDevInfoPcie  m_Pcie;
        DtIoctlDevInfoUsb  m_Usb;
    }  m_DevSpecific;
    
}  DtIoctlGetDevInfoOutput;
ASSERT_SIZE(DtIoctlGetDevInfoOutput, 96)

#ifdef WINBUILD
    #define DT_IOCTL_GET_DEV_INFO  CTL_CODE(DT_DEVICE_TYPE,                              \
                                                        DT_FUNC_CODE_GET_DEV_INFO,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlGetDevInfoInOut
    {
        DtIoctlGetDevInfoInput  m_Input;
        DtIoctlGetDevInfoOutput  m_Output;
    }  DtIoctlGetDevInfoInOut;

    #define DT_IOCTL_GET_DEV_INFO  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_GET_DEV_INFO, \
                                                                   DtIoctlGetDevInfoInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_GET_DRIVER_VERSION +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.- GetDriver Version - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// Get Driver Version - IOCTL input data
typedef DtIoctlInputDataHdr DtIoctlGetDriverVersionInput;
ASSERT_SIZE(DtIoctlGetDriverVersionInput, 16)

// Get Driver Version - IOCTL output data
typedef struct _DtIoctlGetDriverVersionOutput 
{
    Int  m_Major;
    Int  m_Minor;
    Int  m_Micro;
    Int  m_Build;
}  DtIoctlGetDriverVersionOutput;
ASSERT_SIZE(DtIoctlGetDriverVersionOutput, 16)

#ifdef WINBUILD
    #define DT_IOCTL_GET_DRIVER_VERSION  CTL_CODE(DT_DEVICE_TYPE,                        \
                                                        DT_FUNC_CODE_GET_DRIVER_VERSION, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlGetDriverVersionInOut
    {
        DtIoctlGetDriverVersionInput  m_Input;
        DtIoctlGetDriverVersionOutput  m_Output;
    }  DtIoctlGetDriverVersionInOut;

    #define DT_IOCTL_GET_DRIVER_VERSION  _IOWR(DT_IOCTL_MAGIC_SIZE,                      \
                                                       DT_FUNC_CODE_GET_DRIVER_VERSION,  \
                                                       DtIoctlGetDriverVersionInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_GET_NWDRIVER_VERSION +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.- GetNwDriver Version - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
// Get Driver Version - IOCTL input data
typedef DtIoctlInputDataHdr DtIoctlGetNwDriverVersionInput;
ASSERT_SIZE(DtIoctlGetNwDriverVersionInput, 16)

// Get NwDriver Version - IOCTL output data
typedef struct _DtIoctlGetNwDriverVersionOutput
{
    Int  m_Major;
    Int  m_Minor;
    Int  m_Micro;
    Int  m_Build;
}  DtIoctlGetNwDriverVersionOutput;
ASSERT_SIZE(DtIoctlGetNwDriverVersionOutput, 16)

#ifdef WINBUILD
#define DT_IOCTL_GET_NWDRIVER_VERSION  CTL_CODE(DT_DEVICE_TYPE,                          \
                                                      DT_FUNC_CODE_GET_NWDRIVER_VERSION, \
                                                      METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlGetNwDriverVersionInOut
{
    DtIoctlGetNwDriverVersionInput  m_Input;
    DtIoctlGetNwDriverVersionOutput  m_Output;
}  DtIoctlGetNwDriverVersionInOut;

#define DT_IOCTL_GET_NWDRIVER_VERSION  _IOWR(DT_IOCTL_MAGIC_SIZE,                        \
                                                       DT_FUNC_CODE_GET_NWDRIVER_VERSION,\
                                                       DtIoctlGetNwDriverVersionInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SET_NWDRIVER_VERSION +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.- SetNwDriver Version - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
// Set NwDriver Version - IOCTL input data
typedef struct _DtIoctlSetNwDriverVersionInput
{
    DtIoctlInputDataHdr m_CmdHdr;
    Int  m_Major;
    Int  m_Minor;
    Int  m_Micro;
    Int  m_Build;
}  DtIoctlSetNwDriverVersionInput;
ASSERT_SIZE(DtIoctlSetNwDriverVersionInput, 32)

#ifdef WINBUILD
#define DT_IOCTL_SET_NWDRIVER_VERSION  CTL_CODE(DT_DEVICE_TYPE,                          \
                                                      DT_FUNC_CODE_SET_NWDRIVER_VERSION, \
                                                      METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlSetNwDriverVersionInOut
{
    DtIoctlSetNwDriverVersionInput  m_Input;
}  DtIoctlSetNwDriverVersionInOut;

#define DT_IOCTL_SET_NWDRIVER_VERSION  _IOWR(DT_IOCTL_MAGIC_SIZE,                        \
                                                       DT_FUNC_CODE_SET_NWDRIVER_VERSION,\
                                                       DtIoctlSetNwDriverVersionInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_GPSTIME_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- GPSTIME commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlGpsTimeCmd
{
    DT_GPSTIME_CMD_GET_STATUS   = 0, // Get One PPS and 10 Mhz status
    DT_GPSTIME_CMD_GET_TIME     = 1, // Get time in ns (modulo 1 second)
    DT_GPSTIME_CMD_GET_HOLDOFF  = 2, // Get Holdoff OnePps signal for 50ns (+/-2ns)
    DT_GPSTIME_CMD_SET_HOLDOFF  = 3, // Set Hold off OnePps signal for 50ns(+/-2ns)
}  DtIoctlGpsTimeCmd;

// .-.-.-.-.-.-.-.-.-.-.-.- GPSTIME Command - Get Status Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlGpsTimeCmdGetStatusInput;
ASSERT_SIZE(DtIoctlGpsTimeCmdGetStatusInput, 16)
typedef struct _DtIoctlGpsTimeCmdGetStatusOutput
{
    Int  m_OnePpsErrorCount;    // Get error count
    Int  m_OnePpsValid;         // Get valid state of one-pps pulse (bool)
    Int  m_OnePpsTrailingTime;  // Get trailing time between 1 PPS and 10Mhz 
    Int  m_10MhzPeriodCount;    // Get 10MHz period cycles
}DtIoctlGpsTimeCmdGetStatusOutput;
ASSERT_SIZE(DtIoctlGpsTimeCmdGetStatusOutput, 16)

// -.-.-.-.-.-.-.-.-.-.-.-.- GPSTIME Command - Get Time Command -.-.-.-.-.-.-.-.-.-.-.-.-.
typedef DtIoctlInputDataHdr DtIoctlGpsTimeCmdGetTimeInput;
ASSERT_SIZE(DtIoctlGpsTimeCmdGetTimeInput, 16)
typedef struct _DtIoctlGpsTimeCmdGetTimeOutput
{
    Int  m_Time; // The current time in nanoseconds, modulo 1 second.
}DtIoctlGpsTimeCmdGetTimeOutput;
ASSERT_SIZE(DtIoctlGpsTimeCmdGetTimeOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.- GPSTIME Command - Get HoldOff Command -.-.-.-.-.-.-.-.-.-.-.-.
typedef DtIoctlInputDataHdr DtIoctlGpsTimeCmdGetHoldOffInput;
ASSERT_SIZE(DtIoctlGpsTimeCmdGetHoldOffInput, 16)
typedef struct _DtIoctlGpsTimeCmdGetHoldOffOutput
{
    Int  m_HoldOff;    // Current Hold-off state TRUE or FALSE
}DtIoctlGpsTimeCmdGetHoldOffOutput;
ASSERT_SIZE(DtIoctlGpsTimeCmdGetHoldOffOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.- GPSTIME Command - Set HoldOff Command -.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtIoctlGpsTimeCmdSetHoldOffInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_HoldOff;    // New Hold-off state TRUE or FALSE
}DtIoctlGpsTimeCmdSetHoldOffInput;
ASSERT_SIZE(DtIoctlGpsTimeCmdSetHoldOffInput, 20)

// -.-.-.-.-.-.-.-.-.-.-.-.- GPSTIME command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// GPSTIME command - Input data
typedef union _DtIoctlGpsTimeCmdInput
{
    DtIoctlGpsTimeCmdSetHoldOffInput  m_SetHoldOff;
} DtIoctlGpsTimeCmdInput;

// GPSTIME command - Output data
typedef union _DtIoctlGpsCmdOutput
{
    DtIoctlGpsTimeCmdGetStatusOutput  m_GetStatus;
    DtIoctlGpsTimeCmdGetTimeOutput  m_GetTime;
    DtIoctlGpsTimeCmdGetHoldOffOutput  m_GetHoldOff;
}  DtIoctlGpsTimeCmdOutput;

#ifdef WINBUILD
#define DT_IOCTL_GPSTIME_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_GPSTIME_CMD,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlGpsTimeCmdInOut {
    DtIoctlGpsTimeCmdInput  m_Input;
    DtIoctlGpsTimeCmdOutput  m_Output;
} DtIoctlGpsTimeCmdInOut;

#define DT_IOCTL_GPSTIME_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_GPSTIME_CMD,    \
                                                                   DtIoctlGpsTimeCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_I2CM_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I2CM commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlI2cMCmd
{
    DT_I2CM_CMD_READ                = 0,  // Read data
    DT_I2CM_CMD_WRITE               = 1,  // Write data
    DT_I2CM_CMD_WRITE_READ          = 2,  // Write/read data
}  DtIoctlI2cMCmd;


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I2CM - Read Command  -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlI2cMCmdReadInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SlaveAddr;           // Slave address
    Int  m_NumToRead;           // Number of bytes to read
}  DtIoctlI2cMCmdReadInput;
ASSERT_SIZE(DtIoctlI2cMCmdReadInput, 24)

typedef struct _DtIoctlI2cMCmdReadOutput 
{
    Int  m_NumBytes;            // Number of bytes in buffer
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlI2cMCmdReadOutput;
ASSERT_SIZE(DtIoctlI2cMCmdReadOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I2CM - Write Command  -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlI2cMCmdWriteInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SlaveAddr;           // Start address
    Int  m_NumToWrite;          // Number of bytes in buffer to write
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlI2cMCmdWriteInput;
ASSERT_SIZE(DtIoctlI2cMCmdWriteInput, 24)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I2CM - Write Read Command  -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlI2cMCmdWriteReadInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SlaveAddr;           // Start address
    Int  m_NumToRead;           // Number of bytes to read
    Int  m_NumToWrite;          // Number of bytes in buffer to write
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlI2cMCmdWriteReadInput;
ASSERT_SIZE(DtIoctlI2cMCmdWriteReadInput, 28)
typedef struct _DtIoctlI2cMCmdWriteReadOutput 
{
    Int  m_NumBytes;            // Number of bytes in buffer
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlI2cMCmdWriteReadOutput;
ASSERT_SIZE(DtIoctlI2cMCmdWriteReadOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.-.- I2CM command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-
// I2CM command - Input data
typedef union _DtIoctlI2cMCmdInput
{
    DtIoctlI2cMCmdReadInput  m_Read;
    DtIoctlI2cMCmdWriteInput  m_Write;
    DtIoctlI2cMCmdWriteReadInput  m_WriteRead;
} DtIoctlI2cMCmdInput;
ASSERT_SIZE(DtIoctlI2cMCmdInput, 28)

// I2CM command - Output data
typedef union _DtIoctlI2cMCmdOutput
{
    DtIoctlI2cMCmdReadOutput  m_Read;
    DtIoctlI2cMCmdWriteReadOutput  m_WriteRead;
}  DtIoctlI2cMCmdOutput;
ASSERT_SIZE(DtIoctlI2cMCmdOutput, 4)

#ifdef WINBUILD
    #define DT_IOCTL_I2CM_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_I2CM_CMD,           \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlI2cMCmdInOut {
        DtIoctlI2cMCmdInput  m_Input;
        DtIoctlI2cMCmdOutput  m_Output;
    } DtIoctlI2cMCmdInOut;

    #define DT_IOCTL_I2CM_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_I2CM_CMD,         \
                                                                      DtIoctlI2cMCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IPSECG_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IPSECG commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIpSecGCmd
{
    DT_IPSECG_CMD_GET_STATUS        = 0,  // Get status
    DT_IPSECG_CMD_CHECK             = 1,  // Check
    DT_IPSECG_CMD_CHALLENGE         = 2,  // Challenge
}  DtIoctlIpSecGCmd;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- IPSECG - Get Status Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtIoctlIpSecGCmdGetStatusInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlIpSecGCmdGetStatusInput;
ASSERT_SIZE(DtIoctlIpSecGCmdGetStatusInput, 16)

typedef struct _DtIoctlIpSecGCmdGetStatusOutput
{
    Int64A  m_BoardId;          // Board ID
    Int  m_IsBusy;              // Controller is busy (TRUE or FALSE)
    Int  m_IsOk;                // Status is OK (TRUE or FALSE)
}  DtIoctlIpSecGCmdGetStatusOutput;
ASSERT_SIZE(DtIoctlIpSecGCmdGetStatusOutput, 16)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IPSECG - Check Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtIoctlIpSecGCmdCheckInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumWords;
    UInt32  m_Data[0];
}  DtIoctlIpSecGCmdCheckInput;
ASSERT_SIZE(DtIoctlIpSecGCmdCheckInput, 20)

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- IPSECG - Challenge Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtIoctlIpSecGCmdChallengeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt32  m_Data[3];
}  DtIoctlIpSecGCmdChallengeInput;
ASSERT_SIZE(DtIoctlIpSecGCmdChallengeInput, 28)
typedef struct _DtIoctlIpSecGCmdChallengeOutput
{
    UInt32  m_Data[3];
}  DtIoctlIpSecGCmdChallengeOutput;
ASSERT_SIZE(DtIoctlIpSecGCmdChallengeOutput, 12)

//.-.-.-.-.-.-.-.-.-.-.-.-.- IPSECG command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IPSECG command - Input data
typedef union _DtIoctlIpSecGCmdInput
{
    DtIoctlIpSecGCmdGetStatusInput  m_GetStatus;
    DtIoctlIpSecGCmdCheckInput  m_Check;
    DtIoctlIpSecGCmdChallengeInput  m_Challenge;
} DtIoctlIpSecGCmdInput;
ASSERT_SIZE(DtIoctlIpSecGCmdInput, 28)

// IPSECG command - Output data
typedef union _DtIoctlIpSecGCmdOutput
{
    DtIoctlIpSecGCmdGetStatusOutput  m_GetStatus;
    DtIoctlIpSecGCmdChallengeOutput  m_Challenge;
}  DtIoctlIpSecGCmdOutput;
ASSERT_SIZE(DtIoctlIpSecGCmdOutput, 16)


#ifdef WINBUILD
    #define DT_IOCTL_IPSECG_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IPSECG_CMD,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIpSecGCmdInOut {
        DtIoctlIpSecGCmdInput  m_Input;
        DtIoctlIpSecGCmdOutput  m_Output;
    } DtIoctlIpSecGCmdInOut;

    #define DT_IOCTL_IPSECG_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IPSECG_CMD,     \
                                                                    DtIoctlIpSecGCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IOCONFIG_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// IO configuration description
#define DT_MAX_PARXTRA_COUNT        4
#define IOCONFIG_NAME_MAX_SIZE      50
// NOTE: we added Ioctl suffix to prevent name conflict with DtApi::DtIoConfig struct
typedef struct _DtIoctlIoConfig
{
    Int  m_PortIndex;
    char  m_Group[IOCONFIG_NAME_MAX_SIZE];
    char  m_Value[IOCONFIG_NAME_MAX_SIZE];
    char  m_SubValue[IOCONFIG_NAME_MAX_SIZE];
    Int64A  m_ParXtra[DT_MAX_PARXTRA_COUNT];

    UInt8  m_SkipExclAccessCheck;   // If true, do not check if port is in use.
                                    // NOTE: only set this to true, if you are the
                                    // owner of the port (i.e. you have the excl access)
} DtIoctlIoConfig;
ASSERT_SIZE(DtIoctlIoConfig, 200)

// DtIoConfigId and DtIoConfigValue are split version of DtIoConfigPars for use in 
// GetIoConfig IO-controls
typedef struct  _DtIoctlIoConfigId
{
    Int  m_PortIndex;
    char  m_Group[IOCONFIG_NAME_MAX_SIZE];
} DtIoctlIoConfigId;
ASSERT_SIZE(DtIoctlIoConfigId, 56)
typedef struct  _DtIoctlIoConfigValue
{
    char  m_Value[IOCONFIG_NAME_MAX_SIZE];
    char  m_SubValue[IOCONFIG_NAME_MAX_SIZE];
    Int64A  m_ParXtra[DT_MAX_PARXTRA_COUNT];
} DtIoctlIoConfigValue;
ASSERT_SIZE(DtIoctlIoConfigValue, 136)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCONFIG Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIoConfigCmd
{
    DT_IOCONFIG_CMD_GET_IOCONFIG = 0,
    DT_IOCONFIG_CMD_SET_IOCONFIG = 1,
}  DtIoctlIoConfigCmd;
//-.-.-.-.-.-.-.-.-.-.-.- IOCONFIG Command - Get IO-Config Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIoConfigCmdGetIoConfigInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_IoConfigCount;           // Number of IO-configs to get
    DtIoctlIoConfigId m_IoCfgId[0];      // Array of IO-config IDs to get
}  DtIoctlIoConfigCmdGetIoConfigInput;
ASSERT_SIZE(DtIoctlIoConfigCmdGetIoConfigInput, 20)
typedef struct _DtIoctlIoConfigCmdGetIoConfigOutput
{
    Int  m_IoConfigCount;               // Number of IO-configs got
    DtIoctlIoConfigValue  m_IoCfgValue[0];   // Array of IO-config values
}  DtIoctlIoConfigCmdGetIoConfigOutput;
ASSERT_SIZE(DtIoctlIoConfigCmdGetIoConfigOutput, 8)
//-.-.-.-.-.-.-.-.-.-.-.- IOCONFIG Command - Set IO-Config Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIoConfigCmdSetIoConfigInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_IoConfigCount;               // Number of IO-configs to set
    DtIoctlIoConfig  m_IoCfgPars[0];     // Array of IO-config parameters
}  DtIoctlIoConfigCmdSetIoConfigInput;
ASSERT_SIZE(DtIoctlIoConfigCmdSetIoConfigInput, 24)
//-.-.-.-.-.-.-.-.-.-.-.-.- IOCONFIG Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// IOCONFIG command - IOCTL input data
typedef union _DtIoctlIoConfigCmdInput
{
    DtIoctlIoConfigCmdGetIoConfigInput  m_GetIoConfig;  // IOCONFIG - Get IO-config
    DtIoctlIoConfigCmdSetIoConfigInput  m_SetIoConfig;  // IOCONFIG - Set IO-config
}  DtIoctlIoConfigCmdInput;
// IOCONFIG command - IOCTL output data
typedef union _DtIoctlIoConfigCmdOutput
{
    DtIoctlIoConfigCmdGetIoConfigOutput  m_GetIoConfig; // IOCONFIG - Get IO-config
}  DtIoctlIoConfigCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_IOCONFIG_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IOCONFIG_CMD,   \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIoConfigCmdInOut
    {
        DtIoctlIoConfigCmdInput  m_Input;
        DtIoctlIoConfigCmdOutput  m_Output;
    }  DtIoctlIoConfigCmdInOut;
    #define DT_IOCTL_IOCONFIG_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IOCONFIG_CMD, \
                                                                  DtIoctlIoConfigCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IOPARIN_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOPARIN commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
    typedef enum _DtIoctlIoParInCmd
    {
        DT_IOPARIN_CMD_GET_OPERATIONAL_MODE = 0,  // Get operational mode
        DT_IOPARIN_CMD_SET_OPERATIONAL_MODE = 1,  // Set operational mode
    }  DtIoctlIoParInCmd;

    // -.-.-.-.-.-.-.-.- IOPARIN Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.
    //
    typedef DtIoctlInputDataHdr DtIoctlIoParInCmdGetOpModeInput;
    ASSERT_SIZE(DtIoctlIoParInCmdGetOpModeInput, 16)
    typedef struct _DtIoctlIoParInCmdGetOpModeOutput
    {
        Int  m_OpMode;              // Operational mode
    }  DtIoctlIoParInCmdGetOpModeOutput;
    ASSERT_SIZE(DtIoctlIoParInCmdGetOpModeOutput, 4)

    // -.-.-.-.-.-.-.-.- IOPARIN Command - Set Operational Mode Command -.-.-.-.-.-.-.-.-.
    //
    typedef struct _DtIoctlIoParInCmdSetOpModeInput
    {
        DtIoctlInputDataHdr  m_CmdHdr;
        Int  m_OpMode;              // Operational mode: IDLE/RUN
    } DtIoctlIoParInCmdSetOpModeInput;
    ASSERT_SIZE(DtIoctlIoParInCmdSetOpModeInput, 20)

    // -.-.-.-.-.-.-.-.-.-.-.- IOPARIN command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
    // IOPARIN command - Input data
    typedef union _DtIoctlIoParInCmdInput
    {
        DtIoctlIoParInCmdGetOpModeInput  m_GetOpMode;
        DtIoctlIoParInCmdSetOpModeInput  m_SetOpMode;
    } DtIoctlIoParInCmdInput;
    ASSERT_SIZE(DtIoctlIoParInCmdInput, 20)

    // IOPARIN command - Output data
    typedef union _DtIoctlIoParInCmdOutput
    {
        DtIoctlIoParInCmdGetOpModeOutput  m_GetOpMode;
    }  DtIoctlIoParInCmdOutput;
    ASSERT_SIZE(DtIoctlIoParInCmdOutput, 4)

#ifdef WINBUILD
#define DT_IOCTL_IOPARIN_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IOPARIN_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
        typedef union _DtIoctlIoParInCmdInOut {
        DtIoctlIoParInCmdInput  m_Input;
        DtIoctlIoParInCmdOutput  m_Output;
    } DtIoctlIoParInCmdInOut;

#define DT_IOCTL_IOPARIN_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                           \
                                                            DT_FUNC_CODE_IOPARIN_CMD,  \
                                                                 DtIoctlIoParInCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IOSERIN_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOSERIN commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
    typedef enum _DtIoctlIoSerInCmd
    {
        DT_IOSERIN_CMD_GET_OPERATIONAL_MODE = 0,  // Get operational mode
        DT_IOSERIN_CMD_SET_OPERATIONAL_MODE = 1,  // Set operational mode
    }  DtIoctlIoSerInCmd;

    // -.-.-.-.-.-.-.-.- IOSERIN Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.
    //
    typedef DtIoctlInputDataHdr DtIoctlIoSerInCmdGetOpModeInput;
    ASSERT_SIZE(DtIoctlIoSerInCmdGetOpModeInput, 16)
    typedef struct _DtIoctlIoSerInCmdGetOpModeOutput
    {
        Int  m_OpMode;              // Operational mode
    }  DtIoctlIoSerInCmdGetOpModeOutput;
    ASSERT_SIZE(DtIoctlIoSerInCmdGetOpModeOutput, 4)

    // -.-.-.-.-.-.-.-.- IOSERIN Command - Set Operational Mode Command -.-.-.-.-.-.-.-.-.
    //
    typedef struct _DtIoctlIoSerInCmdSetOpModeInput
    {
        DtIoctlInputDataHdr  m_CmdHdr;
        Int  m_OpMode;              // Operational mode: IDLE/RUN
    } DtIoctlIoSerInCmdSetOpModeInput;
    ASSERT_SIZE(DtIoctlIoSerInCmdSetOpModeInput, 20)

    // -.-.-.-.-.-.-.-.-.-.-.- IOSERIN command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
    // IOSERIN command - Input data
    typedef union _DtIoctlIoSerInCmdInput
    {
        DtIoctlIoSerInCmdGetOpModeInput  m_GetOpMode;
        DtIoctlIoSerInCmdSetOpModeInput  m_SetOpMode;
    } DtIoctlIoSerInCmdInput;
    ASSERT_SIZE(DtIoctlIoSerInCmdInput, 20)

    // IOSERIN command - Output data
    typedef union _DtIoctlIoSerInCmdOutput
    {
        DtIoctlIoSerInCmdGetOpModeOutput  m_GetOpMode;
    }  DtIoctlIoSerInCmdOutput;
    ASSERT_SIZE(DtIoctlIoSerInCmdOutput, 4)

#ifdef WINBUILD
#define DT_IOCTL_IOSERIN_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IOSERIN_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
        typedef union _DtIoctlIoSerInCmdInOut {
        DtIoctlIoSerInCmdInput  m_Input;
        DtIoctlIoSerInCmdOutput  m_Output;
    } DtIoctlIoSerInCmdInOut;

#define DT_IOCTL_IOSERIN_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                           \
                                                            DT_FUNC_CODE_IOSERIN_CMD,  \
                                                                 DtIoctlIoSerInCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQAGC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq Noise Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqAgcCmd
{
    DT_IQAGC_CMD_GET_CONFIG           = 0, // Get configuation
    DT_IQAGC_CMD_GET_GAIN_CONTROL     = 1, // Get gain control
    DT_IQAGC_CMD_GET_GAIN_CTRL_PARS   = 2, // Get gain control parameters
    DT_IQAGC_CMD_GET_GAIN_STATUS      = 3, // Get gain status
    DT_IQAGC_CMD_GET_OPERATIONAL_MODE = 4, // Get operational mode
    DT_IQAGC_CMD_SET_GAIN_CONTROL     = 5, // Set gain control
    DT_IQAGC_CMD_SET_GAIN_CTRL_PARS   = 6, // Set gain control parameters
    DT_IQAGC_CMD_SET_OPERATIONAL_MODE = 7, // Set operational mode
}  DtIoctlIqAgcCmd;

// Signal type
#define DT_IQAGC_SIGNALTYPE_REAL       0x0  // Real (IF) Signal
#define DT_IQAGC_SIGNALTYPE_COMPLEX    0x1  // Complex (IQ) Signal

// Gain correlation
#define DT_IQAGC_CORRELATION_POSITIVE  0x0  // High voltage is high gain
#define DT_IQAGC_CORRELATION_NEGATIVE  0x1  // High voltage is low gain

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- IQAGC Command - Get Config -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqAgcCmdGetConfigInput;
ASSERT_SIZE(DtIoctlIqAgcCmdGetConfigInput, 16)
typedef struct _DtIoctlIqAgcCmdGetConfigOutput
{
    Int m_GainCorrelation1;     // Gain Correlation 1
    Int m_GainCorrelation2;     // Gain Correlation 2
    Int m_MaxDacValue1;         // Maximum DAC Value 1
    Int m_MaxDacValue2;         // Maximum DAC Value 2
    Int m_MinDacValue1;         // Minimum DAC Value 1
    Int m_MinDacValue2;         // Minimum DAC Value 2
    Int m_DacNumBits;           // Number of bits of the DAC device
    Int m_IqBitDepth;           // Number of bits in each I and Q input sample
}  DtIoctlIqAgcCmdGetConfigOutput;
ASSERT_SIZE(DtIoctlIqAgcCmdGetConfigOutput, 32)

// .-.-.-.-.-.-.-.-.-.-.-.-.- IQAGC Command - Get Gain Control -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqAgcCmdGetGainControlInput;
ASSERT_SIZE(DtIoctlIqAgcCmdGetGainControlInput, 16)
typedef struct _DtIoctlIqAgcCmdGetGainControlOutput
{
    Int  m_UseManualGain;        // Gain control: AUTO/MANUAL
    Int  m_ManualGain;           // Value to manually overrule the gain
}  DtIoctlIqAgcCmdGetGainControlOutput;
ASSERT_SIZE(DtIoctlIqAgcCmdGetGainControlOutput, 8)

// -.-.-.-.-.-.-.-.-.-.- IQAGC Command - Get Gain Control Parameters -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqAgcCmdGetGainCtrlParsInput;
ASSERT_SIZE(DtIoctlIqAgcCmdGetGainCtrlParsInput, 16)
typedef struct _DtIoctlIqAgcCmdGetGainCtrlParsOutput
{
    Int  m_SignalType;          // Input signal type Real/Complex
    Int  m_GainCorrelation;     // Correlation between voltage and gain
    Int  m_MaxDacValue;         // Maximum DAC value
    Int  m_MinDacValue;         // Minimum DAC value
    Int  m_StepSizeUnlocked;    // Gain control step size when not locked
    Int  m_StepSizeLocked;      // Gain control step size when  locked
    Int  m_ThresholdHigh;       // Threshold value for the upper side of the working area
    Int  m_ThresholdLow;        // Threshold value for the lower side of the working area
    Int  m_MeasurementPeriod;   // Time period for the gain measurement in us
}  DtIoctlIqAgcCmdGetGainCtrlParsOutput;
ASSERT_SIZE(DtIoctlIqAgcCmdGetGainCtrlParsOutput, 36)

// .-.-.-.-.-.-.-.-.-.-.-.-.- IQAGC Command - Get Gains Status -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqAgcCmdGetGainStatusInput;
ASSERT_SIZE(DtIoctlIqAgcCmdGetGainStatusInput, 16)
typedef struct _DtIoctlIqAgcCmdGetGainStatusOutput
{
    Int  m_Locked;              // Indicates whether the AGC control loop is locked
    Int  m_GainReadback;        // Current gain value sent to the DAC
}  DtIoctlIqAgcCmdGetGainStatusOutput;
ASSERT_SIZE(DtIoctlIqAgcCmdGetGainStatusOutput, 8)

// .-.-.-.-.-.-.-.-.-.-.-.- IQAGC Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqAgcCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqAgcCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqAgcCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqAgcCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqAgcCmdGetOpModeOutput, 4)


// .-.-.-.-.-.-.-.-.-.-.-.-.- IQAGC Command - Set Gain Control -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqAgcCmdSetGainControlInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_UseManualGain;        // Gain control: AUTO/MANUAL
    Int  m_ManualGain;           // Value to manually overrule the gain
}  DtIoctlIqAgcCmdSetGainControlInput;
ASSERT_SIZE(DtIoctlIqAgcCmdSetGainControlInput, 24)

// -.-.-.-.-.-.-.-.-.-.- IQAGC Command - Set Gain Control Parameters -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqAgcCmdSetGainCtrlParsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SignalType;          // Input signal type Real/Complex
    Int  m_GainCorrelation;     // Correlation between voltage and gain
    Int  m_MaxDacValue;         // Maximum DAC value
    Int  m_MinDacValue;         // Minimum DAC value
    Int  m_StepSizeUnlocked;    // Gain control step size when not locked
    Int  m_StepSizeLocked;      // Gain control step size when  locked
    Int  m_ThresholdHigh;       // Threshold value for the upper side of the working area
    Int  m_ThresholdLow;        // Threshold value for the lower side of the working area
    Int  m_MeasurementPeriod;   // Time period for the gain measurement in us
}  DtIoctlIqAgcCmdSetGainCtrlParsInput;
ASSERT_SIZE(DtIoctlIqAgcCmdSetGainCtrlParsInput, 52)

// -.-.-.-.-.-.-.-.-.- IQAGC Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqAgcCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqAgcCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqAgcCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.-.- IQAGC Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQAGC command - IOCTL input data
typedef union _DtIoctlIqAgcCmdInput
{
    DtIoctlIqAgcCmdSetGainControlInput m_SetGainControl;
    DtIoctlIqAgcCmdSetGainCtrlParsInput m_SetCtrlPars;
    DtIoctlIqAgcCmdSetOpModeInput  m_SetOpMode;
}  DtIoctlIqAgcCmdInput;


// IQAGC command - IOCTL output data
typedef union _DtIoctlIqAgcCmdOutput
{
    DtIoctlIqAgcCmdGetConfigOutput m_GetConfig;
    DtIoctlIqAgcCmdGetGainControlOutput m_GetGainControl;
    DtIoctlIqAgcCmdGetGainCtrlParsOutput m_GetCtrlPars;
    DtIoctlIqAgcCmdGetGainStatusOutput m_GetStatus;
    DtIoctlIqAgcCmdGetOpModeOutput  m_GetOpMode;
}  DtIoctlIqAgcCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_IQAGC_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IQAGC_CMD,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqAgcCmdInOut
    {
        DtIoctlIqAgcCmdInput  m_Input;
        DtIoctlIqAgcCmdOutput  m_Output;
    }  DtIoctlIqAgcCmdInOut;
#define DT_IOCTL_IQAGC_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQAGC_CMD,           \
                                                                     DtIoctlIqAgcCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQDEMOD_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq Demodulator Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqDemodCmd
{
    DT_IQDEMOD_CMD_GET_NCO_PARS         = 0, // Get NCO parameters
    DT_IQDEMOD_CMD_GET_OPERATIONAL_MODE = 1, // Get operational mode
    DT_IQDEMOD_CMD_SET_NCO_PARS         = 2, // Set NCO parameters
    DT_IQDEMOD_CMD_SET_OPERATIONAL_MODE = 3, // Set operational mode
}  DtIoctlIqDemodCmd;

// .-.-.-.-.-.-.-.-.-.-.-.- IQDEMOD Command - Get NCO Parameters -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqDemodCmdGetNcoParsInput;
ASSERT_SIZE(DtIoctlIqDemodCmdGetNcoParsInput, 16)
typedef struct _DtIoctlIqDemodCmdGetNcoParsOutput
{
    Int  m_Bypass;
    Int  m_NcoPhaseIncr;
}  DtIoctlIqDemodCmdGetNcoParsOutput;
ASSERT_SIZE(DtIoctlIqDemodCmdGetNcoParsOutput, 8)

// -.-.-.-.-.-.-.-.-.-.-.- IQDEMOD Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqDemodCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqDemodCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqDemodCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqDemodCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqDemodCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.- IQDEMOD Command - Set NCO Parameters -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqDemodCmdSetNcoParsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Bypass;
    Int  m_NcoPhaseIncr;
}  DtIoctlIqDemodCmdSetNcoParsInput;
ASSERT_SIZE(DtIoctlIqDemodCmdSetNcoParsInput, 24)

// -.-.-.-.-.-.-.-.-.- IQDEMOD Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqDemodCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqDemodCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqDemodCmdSetOpModeInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- IQDEMOD Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQDEMOD command - IOCTL input data
typedef union _DtIoctlIqDemodCmdInput
{
    DtIoctlIqDemodCmdSetNcoParsInput m_SetNcoPars;
    DtIoctlIqDemodCmdSetOpModeInput  m_SetOpMode;
}  DtIoctlIqDemodCmdInput;


// IQDEMOD command - IOCTL output data
typedef union _DtIoctlIqDemodCmdOutput
{
    DtIoctlIqDemodCmdGetNcoParsOutput m_GetNcoPars;
    DtIoctlIqDemodCmdGetOpModeOutput  m_GetOpMode;
}  DtIoctlIqDemodCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_IQDEMOD_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IQDEMOD_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqDemodCmdInOut
    {
        DtIoctlIqDemodCmdInput  m_Input;
        DtIoctlIqDemodCmdOutput  m_Output;
    }  DtIoctlIqDemodCmdInOut;
#define DT_IOCTL_IQDEMOD_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQDEMOD_CMD,       \
                                                                   DtIoctlIqDemodCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQDOWNSRC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq DownConverter Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqDownSrcCmd
{
    DT_IQDOWNSRC_CMD_GET_CONV_RATE_PARS   = 0, // Get rate conversion parameters
    DT_IQDOWNSRC_CMD_GET_OPERATIONAL_MODE = 1, // Get operational mode
    DT_IQDOWNSRC_CMD_SET_CONV_RATE_PARS   = 2, // Set rate conversion parameters
    DT_IQDOWNSRC_CMD_SET_OPERATIONAL_MODE = 3, // Set operational mode
}  DtIoctlIqDownSrcCmd;

// -.-.-.-.-.-.-.-.- IQDOWNSRC Command - Get Rate Conversion Parameters -.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqDownSrcCmdGetConvRateParsInput;
ASSERT_SIZE(DtIoctlIqDownSrcCmdGetConvRateParsInput, 16)
typedef struct _DtIoctlIqDownSrcCmdGetConvRateParsOutput
{
    Int  m_Bypass;
    UInt32  m_SampleRateRatio;
}  DtIoctlIqDownSrcCmdGetConvRateParsOutput;
ASSERT_SIZE(DtIoctlIqDownSrcCmdGetConvRateParsOutput, 8)

// -.-.-.-.-.-.-.-.-.- IQDOWNSRC Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqDownSrcCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqDownSrcCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqDownSrcCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqDownSrcCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqDownSrcCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.- IQDOWNSRC Command - Set Rate Conversion Parameters -.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqDownSrcCmdSetConvRateParsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Bypass;
    UInt32  m_SampleRateRatio;
}  DtIoctlIqDownSrcCmdSetConvRateParsInput;
ASSERT_SIZE(DtIoctlIqDownSrcCmdSetConvRateParsInput, 24)

// -.-.-.-.-.-.-.-.-.- IQDOWNSRC Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqDownSrcCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqDownSrcCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqDownSrcCmdSetOpModeInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- IQDOWNSRC Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQDOWNSRC command - IOCTL input data
typedef union _DtIoctlIqDownSrcCmdInput
{
    DtIoctlIqDownSrcCmdSetConvRateParsInput m_SetConvRatePars;
    DtIoctlIqDownSrcCmdSetOpModeInput  m_SetOpMode;
}  DtIoctlIqDownSrcCmdInput;


// IQDOWNSRC command - IOCTL output data
typedef union _DtIoctlIqDownSrcCmdOutput
{
    DtIoctlIqDownSrcCmdGetConvRateParsOutput m_GetConvRatePars;
    DtIoctlIqDownSrcCmdGetOpModeOutput  m_GetOpMode;
}  DtIoctlIqDownSrcCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_IQDOWNSRC_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IQDOWNSRC_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqDownSrcCmdInOut
    {
        DtIoctlIqDownSrcCmdInput  m_Input;
        DtIoctlIqDownSrcCmdOutput  m_Output;
    }  DtIoctlIqDownSrcCmdInOut;
#define DT_IOCTL_IQDOWNSRC_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQDOWNSRC_CMD,   \
                                                                 DtIoctlIqDownSrcCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQFIR_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq -FIR Filter Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqFirCmd
{
    DT_IQFIR_CMD_GET_CONFIG           = 0, // Get filter configuration
    DT_IQFIR_CMD_GET_COEFS            = 1, // Get filter coefficients
    DT_IQFIR_CMD_GET_OPERATIONAL_MODE = 2, // Get operational mode
    DT_IQFIR_CMD_SET_COEFS            = 3, // Set filter coefficients
    DT_IQFIR_CMD_SET_OPERATIONAL_MODE = 4, // Set operational mode
    DT_IQFIR_CMD_SET_PRESCALE         = 5, // Set prescale value
}  DtIoctlIqFirCmd;

// -.-.-.-.-.-.-.-.-.-.-.-.- IQFIR Command - Get Config Command -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqFirCmdGetConfigInput;
ASSERT_SIZE(DtIoctlIqFirCmdGetConfigInput, 16)
typedef struct _DtIoctlIqFirCmdGetConfigOutput
{
    Int  m_NumberCoefs;         // Number of coefficients
    Int  m_CoefSize;            // Coefficient size in number of bits
}  DtIoctlIqFirCmdGetConfigOutput;
ASSERT_SIZE(DtIoctlIqFirCmdGetConfigOutput, 8)

// .-.-.-.-.-.-.-.-.-.-.-.-.- IQFIR - Get Coefficients Command -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqFirCmdGetCoefsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumCoefsToGet;       // Number of coefficients to get
}  DtIoctlIqFirCmdGetCoefsInput;
ASSERT_SIZE(DtIoctlIqFirCmdGetCoefsInput, 20)

typedef struct _DtIoctlIqFirCmdGetCoefsOutput 
{
    Int  m_NumCoefs;            // Number of coefficients in buffer
    Int  m_pCoefs[0];           // Dynamic sized buffer
}  DtIoctlIqFirCmdGetCoefsOutput;
ASSERT_SIZE(DtIoctlIqFirCmdGetCoefsOutput, 4)

// .-.-.-.-.-.-.-.-.-.- IQFIR Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqFirCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqFirCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqFirCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqFirCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqFirCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.-.- IQFIR - Set Coefficients Command -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqFirCmdSetCoefsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumCoefsToSet;       // Number of coefficients to get
    Int  m_pCoefs[0];           // Dynamic sized buffer
}  DtIoctlIqFirCmdSetCoefsInput;
ASSERT_SIZE(DtIoctlIqFirCmdSetCoefsInput, 20)

// .-.-.-.-.-.-.-.-.-.- IQFIR Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqFirCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqFirCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqFirCmdSetOpModeInput, 20)

// -.-.-.-.-.-.-.-.-.-.- IQFIR Command - Set  Prescale Value Command -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqFirCmdSetPrescaleInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_PrescaleValue;              // Prescale value
}  DtIoctlIqFirCmdSetPrescaleInput;
ASSERT_SIZE(DtIoctlIqFirCmdSetPrescaleInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.-.- IQFIR Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQFIR command - IOCTL input data
typedef union _DtIoctlIqFirCmdInput
{
    DtIoctlIqFirCmdGetCoefsInput  m_GetCoefs;   // Get Coefficients
    DtIoctlIqFirCmdSetOpModeInput  m_SetOpMode; // IqFir - Set op. mode
    DtIoctlIqFirCmdSetCoefsInput  m_SetCoefs;   // Set Coefficients
    DtIoctlIqFirCmdSetPrescaleInput m_SetPrescale; // Set prescale value
}  DtIoctlIqFirCmdInput;


// IQFIR command - IOCTL output data
typedef union _DtIoctlIqFirCmdOutput
{
    DtIoctlIqFirCmdGetConfigOutput  m_GetConfig; // Get Config
    DtIoctlIqFirCmdGetCoefsOutput  m_GetCoefs;   // Get Coefficients
    DtIoctlIqFirCmdGetOpModeOutput  m_GetOpMode; // IqFir - Get op. mode
}  DtIoctlIqFirCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_IQFIR_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IQFIR_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqFirCmdInOut
    {
        DtIoctlIqFirCmdInput  m_Input;
        DtIoctlIqFirCmdOutput  m_Output;
    }  DtIoctlIqFirCmdInOut;
#define DT_IOCTL_IQFIR_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQFIR_CMD,     \
                                                                   DtIoctlIqFirCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQFIR2XCLK_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.- Iq -FIR Filter 2x Clock Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqFir2xClkCmd
{
    DT_IQFIR2XCLK_CMD_GET_CONFIG           = 0, // Get filter configuration
    DT_IQFIR2XCLK_CMD_GET_COEFS            = 1, // Get filter coefficients
    DT_IQFIR2XCLK_CMD_GET_OPERATIONAL_MODE = 2, // Get operational mode
    DT_IQFIR2XCLK_CMD_SET_COEFS            = 3, // Set filter coefficients
    DT_IQFIR2XCLK_CMD_SET_OPERATIONAL_MODE = 4, // Set operational mode
}  DtIoctlIqFir2xClkCmd;

// -.-.-.-.-.-.-.-.-.-.-.- IQFIR2XCLK Command - Get Config Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqFir2xClkCmdGetConfigInput;
ASSERT_SIZE(DtIoctlIqFir2xClkCmdGetConfigInput, 16)
typedef struct _DtIoctlIqFir2xClkCmdGetConfigOutput
{
    Int  m_NumberCoefs;         // Number of coefficients
    Int  m_CoefSize;            // Coefficient size in number of bits
}  DtIoctlIqFir2xClkCmdGetConfigOutput;
ASSERT_SIZE(DtIoctlIqFir2xClkCmdGetConfigOutput, 8)

// .-.-.-.-.-.-.-.-.-.-.-.- IQFIR2XCLK - Get Coefficients Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqFir2xClkCmdGetCoefsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumCoefsToGet;       // Number of coefficients to get
}  DtIoctlIqFir2xClkCmdGetCoefsInput;
ASSERT_SIZE(DtIoctlIqFir2xClkCmdGetCoefsInput, 20)

typedef struct _DtIoctlIqFir2xClkCmdGetCoefsOutput 
{
    Int  m_NumCoefs;            // Number of coefficients in buffer
    Int  m_pCoefs[0];           // Dynamic sized buffer
}  DtIoctlIqFir2xClkCmdGetCoefsOutput;
ASSERT_SIZE(DtIoctlIqFir2xClkCmdGetCoefsOutput, 4)

// .-.-.-.-.-.-.-.-.- IQFIR2XCLK Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqFir2xClkCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqFir2xClkCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqFir2xClkCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqFir2xClkCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqFir2xClkCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.- IQFIR2XCLK - Set Coefficients Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqFir2xClkCmdSetCoefsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumCoefsToSet;       // Number of coefficients to get
    Int  m_pCoefs[0];           // Dynamic sized buffer
}  DtIoctlIqFir2xClkCmdSetCoefsInput;
ASSERT_SIZE(DtIoctlIqFir2xClkCmdSetCoefsInput, 20)

// -.-.-.-.-.-.-.-.- IQFIR2XCLK Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqFir2xClkCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqFir2xClkCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqFir2xClkCmdSetOpModeInput, 20)


// .-.-.-.-.-.-.-.-.-.-.-.-.- IQFIR2XCLK Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQFIR2XCLK command - IOCTL input data
typedef union _DtIoctlIqFir2xClkCmdInput
{
    DtIoctlIqFir2xClkCmdGetCoefsInput  m_GetCoefs;   // Get Coefficients
    DtIoctlIqFir2xClkCmdSetOpModeInput  m_SetOpMode; // IqFir2xClk - Set op. mode
    DtIoctlIqFir2xClkCmdSetCoefsInput  m_SetCoefs;   // Set Coefficients
}  DtIoctlIqFir2xClkCmdInput;


// IQFIR2XCLK command - IOCTL output data
typedef union _DtIoctlIqFir2xClkCmdOutput
{
    DtIoctlIqFir2xClkCmdGetConfigOutput  m_GetConfig; // Get Config
    DtIoctlIqFir2xClkCmdGetCoefsOutput  m_GetCoefs;   // Get Coefficients
    DtIoctlIqFir2xClkCmdGetOpModeOutput  m_GetOpMode; // IqFir2xClk - Get op. mode
}  DtIoctlIqFir2xClkCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_IQFIR2XCLK_CMD  CTL_CODE(DT_DEVICE_TYPE,                            \
                           DT_FUNC_CODE_IQFIR2XCLK_CMD, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqFir2xClkCmdInOut
    {
        DtIoctlIqFir2xClkCmdInput  m_Input;
        DtIoctlIqFir2xClkCmdOutput  m_Output;
    }  DtIoctlIqFir2xClkCmdInOut;
#define DT_IOCTL_IQFIR2XCLK_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQFIR2XCLK_CMD, \
                                                                DtIoctlIqFir2xClkCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQINV_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq Demodulator Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqInvCmd
{
    DT_IQINV_CMD_GET_INVERT           = 0, // Get spectral inversion
    DT_IQINV_CMD_GET_OPERATIONAL_MODE = 1, // Get operational mode
    DT_IQINV_CMD_SET_INVERT           = 2, // Set spectral inversion
    DT_IQINV_CMD_SET_OPERATIONAL_MODE = 3, // Set operational mode
}  DtIoctlIqInvCmd;

// -.-.-.-.-.-.-.-.-.-.-.- IQINV Command - Get Spectral Inversion -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqInvCmdGetInvertInput;
ASSERT_SIZE(DtIoctlIqInvCmdGetInvertInput, 16)
typedef struct _DtIoctlIqInvCmdGetInvertOutput
{
    Int  m_Invert;
}  DtIoctlIqInvCmdGetInvertOutput;
ASSERT_SIZE(DtIoctlIqInvCmdGetInvertOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.- IQINV Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqInvCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqInvCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqInvCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqInvCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqInvCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.-.-.-.- IQINV Command - Set Spectral Inversion -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqInvCmdSetInvertInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Invert;
}  DtIoctlIqInvCmdSetInvertInput;
ASSERT_SIZE(DtIoctlIqInvCmdSetInvertInput, 20)

// .-.-.-.-.-.-.-.-.-.- IQINV Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqInvCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqInvCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqInvCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.-.- IQINV Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQINV command - IOCTL input data
typedef union _DtIoctlIqInvCmdInput
{
    DtIoctlIqInvCmdSetInvertInput m_SetInvert;
    DtIoctlIqInvCmdSetOpModeInput  m_SetOpMode;
}  DtIoctlIqInvCmdInput;


// IQINV command - IOCTL output data
typedef union _DtIoctlIqInvCmdOutput
{
    DtIoctlIqInvCmdGetInvertOutput m_GetInvert;
    DtIoctlIqInvCmdGetOpModeOutput  m_GetOpMode;
}  DtIoctlIqInvCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_IQINV_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IQINV_CMD,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqInvCmdInOut
    {
        DtIoctlIqInvCmdInput  m_Input;
        DtIoctlIqInvCmdOutput  m_Output;
    }  DtIoctlIqInvCmdInOut;
#define DT_IOCTL_IQINV_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQINV_CMD,       \
                                                                     DtIoctlIqInvCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQJESD_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq JESD Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqJesdCmd
{
    DT_IQJESD_CMD_CALIBRATE_ATX_PLL               = 0, // Calibrate the Atx PLL
    DT_IQJESD_CMD_GET_OPERATIONAL_MODE            = 1, // Get operational mode    
    DT_IQJESD_CMD_GET_SYSREF_ALWAYS_ON            = 2, // Set sysref always on
    DT_IQJESD_CMD_RE_INITIALIZE_CSR_LINK          = 3, // Re-Initialize Csr Link
    DT_IQJESD_CMD_SET_OPERATIONAL_MODE            = 4, // Set operational mode
    DT_IQJESD_CMD_SET_SYSREF_ALWAYS_ON            = 5  // Set sysref always on
}  DtIoctlIqJesdCmd;

// -.-.-.-.-.-.-.-.-.- IQJESD Command - Get Jesd Calibrate Atx Pll Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqJesdCmdCalibrateAtxPllInput;
ASSERT_SIZE(DtIoctlIqJesdCmdCalibrateAtxPllInput, 16)

// -.-.-.-.-.-.-.-.-.- IQJESD Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqJesdCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqJesdCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqJesdCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqJesdCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqJesdCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.-.- IQJESD Command - Get Jesd Sysref Always On Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqJesdCmdGetSysrefAlwaysOnInput;
ASSERT_SIZE(DtIoctlIqJesdCmdGetSysrefAlwaysOnInput, 16)
typedef struct _DtIoctlIqJesdCmdGetSysrefAlwaysOnOutput
{
    Int  m_AlwaysOn;              // Sysref AlwaysOn flag. TRUE or FALSE 
}  DtIoctlIqJesdCmdGetSysrefAlwaysOnOutput;
ASSERT_SIZE(DtIoctlIqJesdCmdGetSysrefAlwaysOnOutput, 4)

// -.-.-.-.-.-.-.-.-.- IQJESD Command - Re-Initialize Csr Link -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqJesdCmdReInitializeCsrlinkInput;
ASSERT_SIZE(DtIoctlIqJesdCmdReInitializeCsrlinkInput, 16)

// -.-.-.-.-.-.-.-.-.- IQJESD Command - Set Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqJesdCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqJesdCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqJesdCmdSetOpModeInput, 20)

// -.-.-.-.-.-.-.-.-.- IQJESD Command - Set Sysref AlwaysOn Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqJesdCmdSetSysrefAlwaysOnInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_AlwaysOn;              // Sysref AlwaysOn flag. TRUE or FALSE 
}  DtIoctlIqJesdCmdSetSysrefAlwaysOnInput;
ASSERT_SIZE(DtIoctlIqJesdCmdSetSysrefAlwaysOnInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- IQJESD Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQJESD command - IOCTL input data
typedef union _DtIoctlIqJesdCmdInput
{
    DtIoctlIqJesdCmdSetOpModeInput  m_SetOpMode;
    DtIoctlIqJesdCmdSetSysrefAlwaysOnInput  m_SetSysRefAlwaysOn;
}  DtIoctlIqJesdCmdInput;

// IQJESD command - IOCTL output data
typedef union _DtIoctlIqJesdCmdOutput
{
    DtIoctlIqJesdCmdGetOpModeOutput  m_GetOpMode;
    DtIoctlIqJesdCmdGetSysrefAlwaysOnOutput m_GetSysrefAlwaysOn;
}  DtIoctlIqJesdCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_IQJESD_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IQJESD_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqJesdCmdInOut
    {
        DtIoctlIqJesdCmdInput  m_Input;
        DtIoctlIqJesdCmdOutput  m_Output;
    }  DtIoctlIqJesdCmdInOut;
#define DT_IOCTL_IQJESD_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQJESD_CMD,       \
                                                                   DtIoctlIqJesdCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQMISC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq Misc Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqMiscCmd
{
    DT_IQMISC_CMD_GET_IQMUTE             = 0,
    DT_IQMISC_CMD_GET_NOISE_LEVEL        = 1,
    DT_IQMISC_CMD_GET_OFFSET             = 2,
    DT_IQMISC_CMD_GET_OPERATIONAL_MODE   = 3, // Get operational mode
    DT_IQMISC_CMD_GET_OUTPUT_LEVEL       = 4,
    DT_IQMISC_CMD_GET_SPECTRUM_INVERSION = 5,
    DT_IQMISC_CMD_GET_TEST_PATTERN       = 6,
    DT_IQMISC_CMD_SET_IQMUTE             = 7,
    DT_IQMISC_CMD_SET_NOISE_LEVEL        = 8,
    DT_IQMISC_CMD_SET_OFFSET             = 9,
    DT_IQMISC_CMD_SET_OPERATIONAL_MODE   = 10, // Set operational mode
    DT_IQMISC_CMD_SET_OUTPUT_LEVEL       = 11,
    DT_IQMISC_CMD_SET_SPECTRUM_INVERSION = 12,
    DT_IQMISC_CMD_SET_TEST_PATTERN       = 13
}  DtIoctlIqMiscCmd;

// .-.-.-.-.-.-.-.-.-.-.-.- IQMISC Command - Get IQ mute Command -.-.-.-.-.-.-.-.-.-.-.-.
typedef DtIoctlInputDataHdr DtIoctlIqMiscCmdGetIqMuteInput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetIqMuteInput, 16)
typedef struct _DtIoctlIqMiscCmdGetIqMuteOutput
{
    Int  m_MuteI;    // Current mute state for I TRUE or FALSE
    Int  m_MuteQ;    // Current mute state for Q TRUE or FALSE
}DtIoctlIqMiscCmdGetIqMuteOutput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetIqMuteOutput, 8)

// .-.-.-.-.-.-.-.-.-.-.- IQMISC Command - Get Noise level Command -.-.-.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlIqMiscCmdGetNoiseLevelInput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetNoiseLevelInput, 16)
typedef struct _DtIoctlIqMiscCmdGetNoiseLevelOutput
{
    Int  m_NoiseLevel;    // Current noise level value
}DtIoctlIqMiscCmdGetNoiseLevelOutput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetNoiseLevelOutput, 4)

// -.-.-.-.-.-.-.-.-.-.-.-.- IQMISC Command - Get Offset Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlIqMiscCmdGetOffsetInput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetOffsetInput, 16)
typedef struct _DtIoctlIqMiscCmdGetOffsetOutput
{
    Int  m_Offset;    // Current offset value
}DtIoctlIqMiscCmdGetOffsetOutput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetOffsetOutput, 4)

// -.-.-.-.-.-.-.-.-.- IQMISC Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqMiscCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqMiscCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqMiscCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.-.-.-.-.- IQMISC Command - Get Offset Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlIqMiscCmdGetOutputLevelInput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetOutputLevelInput, 16)
typedef struct _DtIoctlIqMiscCmdGetOutputLevelOutput
{
    UInt32  m_OutputLevel;    // Current offset value
}DtIoctlIqMiscCmdGetOutputLevelOutput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetOutputLevelOutput, 4)

// -.-.-.-.-.-.-.-.-.-.- IQMISC Command - Get IQ SpectrumInv Command -.-.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlIqMiscCmdGetSpectrumInvInput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetSpectrumInvInput, 16)
typedef struct _DtIoctlIqMiscCmdGetSpectrumInvOutput
{
    Int  m_SpectrumInversion;   // Current spectrum inversion state TRUE or FALSE
}DtIoctlIqMiscCmdGetSpectrumInvOutput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetSpectrumInvOutput, 4)

// DT IQMISC test pattern
#define DT_IQMISC_TESTPATT_NONE         0 // No test pattern
#define DT_IQMISC_TESTPATT_NYQ          1 // Nyquist
#define DT_IQMISC_TESTPATT_HALFNYQ      2 // Half Nyquist
#define DT_IQMISC_TESTPATT_CW           3 // Constant value for I and Q
#define DT_IQMISC_TESTPATT_CI           4 // Constant value for I
#define DT_IQMISC_TESTPATT_CQ           5 // Constant value for Q

// .-.-.-.-.-.-.-.-.-.-.- IQMISC Command - Get TestPattern Command -.-.-.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlIqMiscCmdGetTestPatternInput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetTestPatternInput, 16)
typedef struct _DtIoctlIqMiscCmdGetTestPatternOutput
{
    Int  m_TestPattern;   // Current test pattern
}DtIoctlIqMiscCmdGetTestPatternOutput;
ASSERT_SIZE(DtIoctlIqMiscCmdGetTestPatternOutput, 4)

// -.-.-.-.-.-.-.-.-.-.-.-.- IQMISC Command - Set IqMute Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtIoctlIqMiscCmdSetIqMuteInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_MuteI;    // New mute state for I TRUE or FALSE
    Int  m_MuteQ;    // New mute state for Q TRUE or FALSE
}DtIoctlIqMiscCmdSetIqMuteInput;
ASSERT_SIZE(DtIoctlIqMiscCmdSetIqMuteInput, 24)

// -.-.-.-.-.-.-.-.-.-.-.- IQMISC Command - Set NoiseLevel Command -.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtIoctlIqMiscCmdSetNoiseLevelInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NoiseLevel;
}DtIoctlIqMiscCmdSetNoiseLevelInput;
ASSERT_SIZE(DtIoctlIqMiscCmdSetNoiseLevelInput, 20)

// -.-.-.-.-.-.-.-.-.-.-.-.- IQMISC Command - Set Offset Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtIoctlIqMiscCmdSetOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Offset;
}DtIoctlIqMiscCmdSetOffsetInput;
ASSERT_SIZE(DtIoctlIqMiscCmdSetOffsetInput, 20)

// -.-.-.-.-.-.-.-.-.- IQMISC Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqMiscCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqMiscCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqMiscCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.-.-.-.- IQMISC Command - Set OutputLevel Command -.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtIoctlIqMiscCmdSetOutputLevelInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OutputLevel;
}DtIoctlIqMiscCmdSetOutputLevelInput;
ASSERT_SIZE(DtIoctlIqMiscCmdSetOutputLevelInput, 20)

// .-.-.-.-.-.-.-.-.-.-.- IQMISC Command - Set SpectrumInv Command -.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtIoctlIqMiscCmdSetSpectrumInvInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SpectrumInversion;   // New spectrum inversion state TRUE or FALSE
}DtIoctlIqMiscCmdSetSpectrumInvInput;
ASSERT_SIZE(DtIoctlIqMiscCmdSetSpectrumInvInput, 20)

// .-.-.-.-.-.-.-.-.-.-.- IQMISC Command - Set TestPattern Command -.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtIoctlIqMiscCmdSetTestPatternInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_TestPattern;   // New test pattern.
}DtIoctlIqMiscCmdSetTestPatternInput;
ASSERT_SIZE(DtIoctlIqMiscCmdSetTestPatternInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- IQMISC Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQMISC command - IOCTL input data
typedef union _DtIoctlIqMiscCmdInput
{
    DtIoctlIqMiscCmdSetIqMuteInput  m_SetIqMute;          // IqMisc - Set IQ mute states
    DtIoctlIqMiscCmdSetNoiseLevelInput  m_SetNoiseLevel;  // IqMisc - Set Noise level
    DtIoctlIqMiscCmdSetOffsetInput  m_SetOffset;          // IqMisc - Set Offset
    DtIoctlIqMiscCmdSetOpModeInput  m_SetOpMode;          // IqMisc - Set op. mode
    DtIoctlIqMiscCmdSetOutputLevelInput m_SetOutputLevel; // IqMisc - Set Output level
    DtIoctlIqMiscCmdSetSpectrumInvInput m_SetSpectrumInv; // IqMisc - Set Spectrum Inv.
    DtIoctlIqMiscCmdSetTestPatternInput m_SetTestPattern; // IqMisc - Set test pattern
}  DtIoctlIqMiscCmdInput;

// IQMISC command - IOCTL output data
typedef union _DtIoctlIqMiscCmdOutput
{
    DtIoctlIqMiscCmdGetIqMuteOutput  m_GetIqMute;           // IqMisc - Get IQ mute states
    DtIoctlIqMiscCmdGetNoiseLevelOutput  m_GetNoiseLevel;   // IqMisc - Get Noise level
    DtIoctlIqMiscCmdGetOffsetOutput  m_GetOffset;           // IqMisc - Get Offset
    DtIoctlIqMiscCmdGetOutputLevelOutput  m_GetOutputLevel; // IqMisc - Get Output level
    DtIoctlIqMiscCmdGetOpModeOutput  m_GetOpMode;           // IqMisc - Get op. mode
    DtIoctlIqMiscCmdGetSpectrumInvOutput  m_GetSpectrumInv; // IqMisc - Get spectyrum inv.
    DtIoctlIqMiscCmdGetTestPatternOutput  m_GetTestPattern; // IqMisc - Get test pattern
}  DtIoctlIqMiscCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_IQMISC_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IQMISC_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqMiscCmdInOut
    {
        DtIoctlIqMiscCmdInput  m_Input;
        DtIoctlIqMiscCmdOutput  m_Output;
    }  DtIoctlIqMiscCmdInOut;
#define DT_IOCTL_IQMISC_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQMISC_CMD,       \
                                                                   DtIoctlIqMiscCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQNOISE_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq Noise Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqNoiseCmd
{
    DT_IQNOISE_CMD_GET_BIT_DEPTH        = 0, // Get Bit depth of the noise generator
    DT_IQNOISE_CMD_GET_OPERATIONAL_MODE = 1, // Get operational mode
    DT_IQNOISE_CMD_SET_OPERATIONAL_MODE = 2, // Set operational mode
}  DtIoctlIqNoiseCmd;

// -.-.-.-.-.-.-.-.-.-.-.- IQNOISE Command - Get Bit Depth Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqNoiseCmdGetBitDepthInput;
ASSERT_SIZE(DtIoctlIqNoiseCmdGetBitDepthInput, 16)
typedef struct _DtIoctlIqNoiseCmdGetBitDepthOutput
{
    UInt8  m_BitDepth;              // Bit Depth noise generator
}  DtIoctlIqNoiseCmdGetBitDepthOutput;
ASSERT_SIZE(DtIoctlIqNoiseCmdGetBitDepthOutput, 1)

// -.-.-.-.-.-.-.-.-.- IQNOISE Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqNoiseCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqNoiseCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqNoiseCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqNoiseCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqNoiseCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.-.- IQNOISE Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqNoiseCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqNoiseCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqNoiseCmdSetOpModeInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- IQNOISE Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQNOISE command - IOCTL input data
typedef union _DtIoctlIqNoiseCmdInput
{
    DtIoctlIqNoiseCmdSetOpModeInput  m_SetOpMode;
}  DtIoctlIqNoiseCmdInput;


// IQNOISE command - IOCTL output data
typedef union _DtIoctlIqNoiseCmdOutput
{
    DtIoctlIqNoiseCmdGetBitDepthOutput  m_GetBitDepth;
    DtIoctlIqNoiseCmdGetOpModeOutput  m_GetOpMode;
}  DtIoctlIqNoiseCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_IQNOISE_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IQNOISE_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqNoiseCmdInOut
    {
        DtIoctlIqNoiseCmdInput  m_Input;
        DtIoctlIqNoiseCmdOutput  m_Output;
    }  DtIoctlIqNoiseCmdInOut;
#define DT_IOCTL_IQNOISE_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQNOISE_CMD,       \
                                                                   DtIoctlIqNoiseCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQPWR_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq PowerEstimator Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqPwrCmd
{
    DT_IQPWR_CMD_GET_OPERATIONAL_MODE = 0, // Get operational mode
    DT_IQPWR_CMD_GET_SUM_OF_SQUARES   = 1, // Get sum of squares
    DT_IQPWR_CMD_GET_WINDOW_SIZE      = 2, // Get window size
    DT_IQPWR_CMD_SET_OPERATIONAL_MODE = 3, // Set operational mode
    DT_IQPWR_CMD_SET_WINDOW_SIZE      = 4, // Set window size
}  DtIoctlIqPwrCmd;

// -.-.-.-.-.-.-.-.-.- IQPWR Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqPwrCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqPwrCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqPwrCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqPwrCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqPwrCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.-.-.-.-.- IQPWR Command - Get Sum of Squares -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqPwrCmdGetSumOfSquaresInput;
ASSERT_SIZE(DtIoctlIqPwrCmdGetSumOfSquaresInput, 16)
typedef struct _DtIoctlIqPwrCmdGetSumOfSquaresOutput
{
    UInt32  m_SumOfSquares;
}  DtIoctlIqPwrCmdGetSumOfSquaresOutput;
ASSERT_SIZE(DtIoctlIqPwrCmdGetSumOfSquaresOutput, 4)

// -.-.-.-.-.-.-.-.-.-.-.-.-.- IQPWR Command - Get Window Size -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqPwrCmdGetWindowSizeInput;
ASSERT_SIZE(DtIoctlIqPwrCmdGetWindowSizeInput, 16)
typedef struct _DtIoctlIqPwrCmdGetWindowSizeOutput
{
    Int  m_WindowSize;
}  DtIoctlIqPwrCmdGetWindowSizeOutput;
ASSERT_SIZE(DtIoctlIqPwrCmdGetWindowSizeOutput, 4)

// -.-.-.-.-.-.-.-.-.- IQPWR Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqPwrCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqPwrCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqPwrCmdSetOpModeInput, 20)

// -.-.-.-.-.-.-.-.-.-.-.-.-.- IQPWR Command - Set WindowSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqPwrCmdSetWindowSizeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_WindowSize;
}  DtIoctlIqPwrCmdSetWindowSizeInput;
ASSERT_SIZE(DtIoctlIqPwrCmdSetWindowSizeInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- IQPWR Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQPWR command - IOCTL input data
typedef union _DtIoctlIqPwrCmdInput
{
    DtIoctlIqPwrCmdSetOpModeInput  m_SetOpMode;
    DtIoctlIqPwrCmdSetWindowSizeInput m_SetWindowSize;
}  DtIoctlIqPwrCmdInput;


// IQPWR command - IOCTL output data
typedef union _DtIoctlIqPwrCmdOutput
{
    DtIoctlIqPwrCmdGetOpModeOutput  m_GetOpMode;
    DtIoctlIqPwrCmdGetSumOfSquaresOutput m_GetSumOfSquares;
    DtIoctlIqPwrCmdGetWindowSizeOutput m_GetWindowSize;
}  DtIoctlIqPwrCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_IQPWR_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IQPWR_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqPwrCmdInOut
    {
        DtIoctlIqPwrCmdInput  m_Input;
        DtIoctlIqPwrCmdOutput  m_Output;
    }  DtIoctlIqPwrCmdInOut;
#define DT_IOCTL_IQPWR_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQPWR_CMD,       \
                                                                      DtIoctlIqPwrCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQSYNC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq Synq Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqSyncCmd
{
    DT_IQSYNC_CMD_GET_OPERATIONAL_MODE    = 0,  // Get operational mode
    DT_IQSYNC_CMD_GET_SAMPLE_COUNTERS     = 1,  // Get Sample Counter
    DT_IQSYNC_CMD_GET_STATUS              = 2,  // Get Status
    DT_IQSYNC_CMD_GET_SYNC_MODE           = 3,  // Get Sync Mode
    DT_IQSYNC_CMD_GET_TIME_TOLERANCE      = 4,  // Get Time Tolerance
    DT_IQSYNC_CMD_GET_TRIGGER_OFFSET      = 5,  // Get Trigger Time Offset
    DT_IQSYNC_CMD_SET_OPERATIONAL_MODE    = 6,  // Set operational mode
    DT_IQSYNC_CMD_SET_SAMPLE_COUNTERS     = 7,  // Set Sample Counter
    DT_IQSYNC_CMD_SET_SYNC_MODE           = 8,  // Set Sync Mode
    DT_IQSYNC_CMD_SET_TIME_TOLERANCE      = 9,  // Set Time Tolerance
    DT_IQSYNC_CMD_SET_TRIGGER_OFFSET      = 10, // Set Trigger Time Offset
}  DtIoctlIqSyncCmd;

// -.-.-.-.-.-.-.-.-.- IQSYNC Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqSyncCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqSyncCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqSyncCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqSyncCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqSyncCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.-.-.- IQSYNC Command - Get Sample Counter Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqSyncCmdGetSampleCountersInput;
ASSERT_SIZE(DtIoctlIqSyncCmdGetSampleCountersInput, 16)
typedef struct _DtIoctlIqSyncCmdGetSampleCounterOutput
{
    UInt32  m_SampleCounterN;              // Integer N
    UInt32  m_SampleCounterP;              // Integer P
    UInt32  m_SampleCounterQ;              // Integer Q
}  DtIoctlIqSyncCmdGetSampleCountersOutput;
ASSERT_SIZE(DtIoctlIqSyncCmdGetSampleCountersOutput, 12)

// DT IQMISC OperationalStatus
#define DT_IQSYNC_OPSTATUS_IDLE     0x0  // Block is inactive
#define DT_IQSYNC_OPSTATUS_STANDBY  0x1  // Block is partially operational
#define DT_IQSYNC_OPSTATUS_RUN      0x2  // Block performs its normal function
#define DT_IQSYNC_OPSTATUS_ERROR    0x3  // An error has occurred

// -.-.-.-.-.-.-.-.-.-.-.-.- IQSYNC Command - Get Status Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqSyncCmdGetStatusInput;
ASSERT_SIZE(DtIoctlIqSyncCmdGetStatusInput, 16)
typedef struct _DtIoctlIqSyncCmdGetStatusOutput
{
    Int  m_OperationalStatus;    // Operational Status
    Int  m_UnderflowCount;    // Underflow Count
    Int  m_TooEarlyCount;     // Too early count
    Int  m_TooLateCount;      // Too late count
}  DtIoctlIqSyncCmdGetStatusOutput;
ASSERT_SIZE(DtIoctlIqSyncCmdGetStatusOutput, 16)

// DT IQMISC test pattern
#define DT_IQSYNC_SYNCMODE_NOSYNC       0 // No sync
#define DT_IQSYNC_SYNCMODE_ONEPPS       1 // Sync on one pps
#define DT_IQSYNC_SYNCMODE_TRIGGER      2 // Sync on trigger

// -.-.-.-.-.-.-.-.-.-.-.- IQSYNC Command - Get Sync Mode Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqSyncCmdGetSyncModeInput;
ASSERT_SIZE(DtIoctlIqSyncCmdGetSyncModeInput, 16)
typedef struct _DtIoctlIqSyncCmdGetSyncModeOutput
{
    Int  m_SyncMode;
}  DtIoctlIqSyncCmdGetSyncModeOutput;
ASSERT_SIZE(DtIoctlIqSyncCmdGetSyncModeOutput, 4)

// -.-.-.-.-.-.-.-.-.-.- IQSYNC Command - Get Time Tolerance Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqSyncCmdGetTimeToleranceInput;
ASSERT_SIZE(DtIoctlIqSyncCmdGetTimeToleranceInput, 16)
typedef struct _DtIoctlIqSyncCmdGetTimeToleranceOutput
{
    Int  m_TimeTolerance;
} DtIoctlIqSyncCmdGetTimeToleranceOutput;
ASSERT_SIZE(DtIoctlIqSyncCmdGetTimeToleranceOutput, 4)

// -.-.-.-.-.-.-.-.-.-.- IQSYNC Command - Get Trigger Offset Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqSyncCmdGetTriggerOffsetInput;
ASSERT_SIZE(DtIoctlIqSyncCmdGetTriggerOffsetInput, 16)
typedef struct _DtIoctlIqSyncCmdGetTriggerOffsetOutput
{
    Int  m_TriggerTimeOffset;
} DtIoctlIqSyncCmdGetTriggerOffsetOutput;
ASSERT_SIZE(DtIoctlIqSyncCmdGetTriggerOffsetOutput, 4)

// .-.-.-.-.-.-.-.-.-.- IQSYNC Command - Set Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqSyncCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;
}  DtIoctlIqSyncCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqSyncCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.-.-.- IQSYNC Command - Set  Sample Counter Command -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqSyncCmdSetSampleCountersInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt32  m_SampleCounterN;              // Integer N
    UInt32  m_SampleCounterP;              // Integer P
    UInt32  m_SampleCounterQ;              // Integer Q
}  DtIoctlIqSyncCmdSetSampleCountersInput;
ASSERT_SIZE(DtIoctlIqSyncCmdSetSampleCountersInput, 28)

// -.-.-.-.-.-.-.-.-.-.-.- IQSYNC Command - Set Sync Mode Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqSyncCmdSetSyncModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SyncMode;
}  DtIoctlIqSyncCmdSetSyncModeInput;
ASSERT_SIZE(DtIoctlIqSyncCmdSetSyncModeInput, 20)

// -.-.-.-.-.-.-.-.-.-.- IQSYNC Command - Set Time Tolerance Command -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqSyncCmdSetTimeToleranceInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_TimeTolerance;
}  DtIoctlIqSyncCmdSetTimeToleranceInput;
ASSERT_SIZE(DtIoctlIqSyncCmdSetTimeToleranceInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.- IQSYNC Command - Set Trigger Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqSyncCmdSetTriggerOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_TriggerTimeOffset;
}  DtIoctlIqSyncCmdSetTriggerOffsetInput;
ASSERT_SIZE(DtIoctlIqSyncCmdSetTriggerOffsetInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- IQSYNC Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQSYNC command - IOCTL input data
typedef union _DtIoctlIqSyncCmdInput
{
    DtIoctlIqSyncCmdSetOpModeInput  m_SetOpMode;
    DtIoctlIqSyncCmdSetSampleCountersInput  m_SetSampleCounter;
    DtIoctlIqSyncCmdSetSyncModeInput  m_SetSyncMode;
    DtIoctlIqSyncCmdSetTimeToleranceInput  m_SetTimeTolerance;
    DtIoctlIqSyncCmdSetTriggerOffsetInput m_SetTriggerTimeOffset;
}  DtIoctlIqSyncCmdInput;

// IQSYNC command - IOCTL output data
typedef union _DtIoctlIqSyncCmdOutput
{
    DtIoctlIqSyncCmdGetOpModeOutput  m_GetOpMode;
    DtIoctlIqSyncCmdGetSampleCountersOutput m_GetSampleCounter;
    DtIoctlIqSyncCmdGetStatusOutput  m_GetStatus;
    DtIoctlIqSyncCmdGetSyncModeOutput  m_GetSyncMode;
    DtIoctlIqSyncCmdGetTimeToleranceOutput  m_GetTimeTolerance;
    DtIoctlIqSyncCmdGetTriggerOffsetOutput m_GetTriggerOffset;
}  DtIoctlIqSyncCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_IQSYNC_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IQSYNC_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqSyncCmdInOut
    {
        DtIoctlIqSyncCmdInput  m_Input;
        DtIoctlIqSyncCmdOutput  m_Output;
    }  DtIoctlIqSyncCmdInOut;
#define DT_IOCTL_IQSYNC_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQSYNC_CMD,       \
                                                                    DtIoctlIqSyncCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQUNPCK_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq unpacker Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqUnpckCmd
{
    DT_IQUNPCK_CMD_GET_OPERATIONAL_MODE = 0, // Get operational mode
    DT_IQUNPCK_CMD_GET_PACKET_MODE      = 1, // Get packet mode
    DT_IQUNPCK_CMD_SET_OPERATIONAL_MODE = 2, // Set operational mode
    DT_IQUNPCK_CMD_SET_PACKET_MODE      = 3, // Set packet mode
}  DtIoctlIqUnpckCmd;

// DT IQUNPCK packet mode
#define DT_IQUNPCK_PCKMODE_IQSAMPLES    0   // IQ samples
#define DT_IQUNPCK_PCKMODE_IQPACKETS    1   // IQ packets

//.-.-.-.-.-.-.-.-.-.-.- IQUNPCK Command - Get packet mode Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqUnpckCmdGetPacketModeInput;
ASSERT_SIZE(DtIoctlIqUnpckCmdGetPacketModeInput, 16)
    typedef struct _DtIoctlIqUnpckCmdGetGetPacketModeOutput
{
    Int  m_PacketMode;         // Packet mode
}  DtIoctlIqUnpckCmdGetPacketModeOutput;
ASSERT_SIZE(DtIoctlIqUnpckCmdGetPacketModeOutput, 4)

//.-.-.-.-.-.-.-.-.-.- IQUNPCK Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqUnpckCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqUnpckCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqUnpckCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqUnpckCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqUnpckCmdGetOpModeOutput, 4)

//.-.-.-.-.-.-.-.-.-.- IQUNPCK Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqUnpckCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqUnpckCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqUnpckCmdSetOpModeInput, 20)

//.-.-.-.-.-.-.-.-.-.-.- IQUNPCK Command - Set packet mode Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqUnpckCmdSetPacketModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_PacketMode;         // ASI polarity
}  DtIoctlIqUnpckCmdSetPacketModeInput;
ASSERT_SIZE(DtIoctlIqUnpckCmdSetPacketModeInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- IQUNPCK Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQUNPCK command - IOCTL input data
typedef union _DtIoctlIqUnpckCmdInput
{
    DtIoctlIqUnpckCmdSetPacketModeInput  m_SetPacketMode;    // IqUnpck - Set packet mode
    DtIoctlIqUnpckCmdSetOpModeInput  m_SetOpMode;            // IqUnpck - Set op. mode
}  DtIoctlIqUnpckCmdInput;

// IQUNPCK command - IOCTL output data
typedef union _DtIoctlIqUnpckCmdOutput
{
    DtIoctlIqUnpckCmdGetPacketModeOutput  m_GetPacketMode;   // IqUnpck - Get packet mode
    DtIoctlIqUnpckCmdGetOpModeOutput  m_GetOpMode;           // IqUnpck - Get op. mode
}  DtIoctlIqUnpckCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_IQUNPCK_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IQUNPCK_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqUnpckCmdInOut
    {
        DtIoctlIqUnpckCmdInput  m_Input;
        DtIoctlIqUnpckCmdOutput  m_Output;
    }  DtIoctlIqUnpckCmdInOut;
#define DT_IOCTL_IQUNPCK_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQUNPCK_CMD,     \
                                                                   DtIoctlIqUnpckCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQUP_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Iq Noise Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqUpCmd
{
    DT_IQUP_CMD_GET_NCO_OFFSET       = 0, // Get NCO offset
    DT_IQUP_CMD_GET_NCO_RATIO        = 1, // Get NCO ratio parameters
    DT_IQUP_CMD_GET_OPERATIONAL_MODE = 2, // Get operational mode
    DT_IQUP_CMD_SET_NCO_OFFSET       = 3, // Set NCO offset
    DT_IQUP_CMD_SET_NCO_RATIO        = 4, // Set Nco ratio parameters
    DT_IQUP_CMD_SET_OPERATIONAL_MODE = 5, // Set operational mode
}  DtIoctlIqUpCmd;

// -.-.-.-.-.-.-.-.-.- IQUP Command - Get NCO Offset -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqUpCmdGetNcoOffsetInput;
ASSERT_SIZE(DtIoctlIqUpCmdGetNcoOffsetInput, 16)
typedef struct _DtIoctlIqUpCmdGetNcoOffsetOutput
{
    UInt32  m_NcoOffset;   // Offset 
}  DtIoctlIqUpCmdGetNcoOffsetOutput;
ASSERT_SIZE(DtIoctlIqUpCmdGetNcoOffsetOutput, 4)

// -.-.-.-.-.-.-.-.-.- IQUP Command - Get NCO ratio  -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqUpCmdGetNcoRatioInput;
ASSERT_SIZE(DtIoctlIqUpCmdGetNcoRatioInput, 16)
typedef struct _DtIoctlIqUpCmdGetNcoRatioOutput
{
    UInt32  m_NcoRatioInteger;        // Ratio Integer
    UInt32  m_NcoRatioNumerator;      // Ratio numerator
    UInt32  m_NcoRatioDenominator;    // Ratio denominator
}  DtIoctlIqUpCmdGetNcoRatioOutput;
ASSERT_SIZE(DtIoctlIqUpCmdGetNcoRatioOutput, 12)

// -.-.-.-.-.-.-.-.-.- IQUP Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqUpCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlIqUpCmdGetOpModeInput, 16)
typedef struct _DtIoctlIqUpCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqUpCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlIqUpCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.-.- IQUP Command - Set NCO Offset -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqUpCmdSetNcoOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt32  m_NcoOffset;        // Offset
}  DtIoctlIqUpCmdSetNcoOffsetInput;
ASSERT_SIZE(DtIoctlIqUpCmdSetNcoOffsetInput, 20)

// -.-.-.-.-.-.-.-.-.- IQUP Command - Set NCO Ratio -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqUpCmdSetNcoRatioInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt32  m_NcoRatioInteger;        // Ratio Integer
    UInt32  m_NcoRatioNumerator;      // Ratio numerator
    UInt32  m_NcoRatioDenominator;    // Ratio denominator
}  DtIoctlIqUpCmdSetNcoRatioInput;
ASSERT_SIZE(DtIoctlIqUpCmdSetNcoRatioInput, 28)

// -.-.-.-.-.-.-.-.-.- IQUP Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqUpCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlIqUpCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlIqUpCmdSetOpModeInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- IQUP Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// IQUP command - IOCTL input data
typedef union _DtIoctlIqUpCmdInput
{
    DtIoctlIqUpCmdSetNcoOffsetInput m_NcoOffset;
    DtIoctlIqUpCmdSetNcoRatioInput m_NcoRatio;
    DtIoctlIqUpCmdSetOpModeInput  m_SetOpMode;
}  DtIoctlIqUpCmdInput;


// IQUP command - IOCTL output data
typedef union _DtIoctlIqUpCmdOutput
{
    DtIoctlIqUpCmdGetNcoOffsetOutput m_NcoOffset;
    DtIoctlIqUpCmdGetNcoRatioOutput m_NcoRatio;
    DtIoctlIqUpCmdGetOpModeOutput  m_GetOpMode;
}  DtIoctlIqUpCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_IQUP_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IQUP_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqUpCmdInOut
    {
        DtIoctlIqUpCmdInput  m_Input;
        DtIoctlIqUpCmdOutput  m_Output;
    }  DtIoctlIqUpCmdInOut;
#define DT_IOCTL_IQUP_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IQUP_CMD,       \
                                                                      DtIoctlIqUpCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_KA_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Keep-Alive Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlKaCmd
{
    DT_KA_CMD_GET_FAILSAFE_INFO = 0,
    DT_KA_CMD_PULSE = 1,
    DT_KA_CMD_SET_FAILSAFE_CONFIG = 2,
}  DtIoctlKaCmd;
//.-.-.-.-.-.-.-.-.-.-.-.- KA Command - Get FailSafeInfo Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlKaCmdGetFailSafeInfoInput;
ASSERT_SIZE(DtIoctlKaCmdGetFailSafeInfoInput, 16)
typedef struct _DtIoctlKaCmdGetFailSafeInfoOutput
{
    Int  m_Enabled;         // Failsafe enabled; TRUE or FALSE
    Int  m_Timeout;         // Watchdog timeout in ms
    Int  m_Alive;           // Still alive; TRUE or FALSE
}  DtIoctlKaCmdGetFailSafeInfoOutput;
ASSERT_SIZE(DtIoctlKaCmdGetFailSafeInfoOutput, 12)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- KA Command - Pulse -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlKaCmdPulseInput;
ASSERT_SIZE(DtIoctlKaCmdPulseInput, 16)

//-.-.-.-.-.-.-.-.-.-.-.- KA Command - Set FailSafe Config Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlKaCmdSetConfigInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Enable;      // Failsafe enabled; TRUE or FALSE
    Int  m_Timeout;     // Watchdog timeout in ms
}  DtIoctlKaCmdSetFailSafeConfigInput;
ASSERT_SIZE(DtIoctlKaCmdSetFailSafeConfigInput, 24)


//.-.-.-.-.-.-.-.-.-.-.-.-.- KA Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// KA command - IOCTL input data
typedef union _DtIoctlKaCmdInput
{
    DtIoctlKaCmdSetFailSafeConfigInput  m_SetConfig;  // Ka - Set failsafe configuration
}  DtIoctlKaCmdInput;
// KA command - IOCTL output data
typedef union _DtIoctlKaCmdOutput
{
    DtIoctlKaCmdGetFailSafeInfoOutput  m_GetInfo;    // Ka - Get failsafe information
}  DtIoctlKaCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_KA_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_KA_CMD,               \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlKaCmdInOut
    {
        DtIoctlKaCmdInput  m_Input;
        DtIoctlKaCmdOutput  m_Output;
    }  DtIoctlKaCmdInOut;
    #define DT_IOCTL_KA_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_KA_CMD,             \
                                                                        DtIoctlKaCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_LEDB_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- LedB Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlLedBCmd
{
    DT_LEDB_CMD_GET_LED_CONTROL = 0,
    DT_LEDB_CMD_SET_LED_CONTROL = 1,
}  DtIoctlLedBCmd;
//.-.-.-.-.-.-.-.-.-.-.-.- LEDB Command - Get Led Control Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlLedBCmdGetLedControlInput;
ASSERT_SIZE(DtIoctlLedBCmdGetLedControlInput, 16)
typedef struct _DtIoctlLedBCmdGetLedControlOutput
{
    Int  m_Overrule;       // Overrule; if FALSE LEDs are automatically controlled
    Int  m_GreenOn;        // if m_Overrule==TRUE green LED state: on/off
    Int  m_RedOn;          // if m_Overrule==TRUE red LED state: on/off
    Int  m_BlueOn;         // if m_Overrule==TRUE blue LED state on/off
}  DtIoctlLedBCmdGetLedControlOutput;
ASSERT_SIZE(DtIoctlLedBCmdGetLedControlOutput, 16)


//.-.-.-.-.-.-.-.-.-.-.-.- LEDB Command - Set Led Control Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlLedBCmdSetLedControlInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Overrule;       // Overrule; if FALSE LEDs are automatically controlled
    Int  m_GreenOn;        // if m_Overrule==TRUE green LED state: on/off
    Int  m_RedOn;          // if m_Overrule==TRUE red LED state: on/off
    Int  m_BlueOn;         // if m_Overrule==TRUE blue LED state on/off
}  DtIoctlLedBCmdSetLedControlInput;
ASSERT_SIZE(DtIoctlLedBCmdSetLedControlInput, 32)

//-.-.-.-.-.-.-.-.-.-.-.-.-.- LEDB Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-
// LEDB command - IOCTL input data
typedef union _DtIoctlLedBCmdInput
{
    DtIoctlLedBCmdSetLedControlInput  m_SetLedControl;    // LedB - Set LED control
}  DtIoctlLedBCmdInput;
// LEDB command - IOCTL output data
typedef union _DtIoctlLedBCmdOutput
{
    DtIoctlLedBCmdGetLedControlOutput  m_GetLedControl;     // LedB - Get LED control
}  DtIoctlLedBCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_LEDB_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_LEDB_CMD,           \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlLedBCmdInOut
    {
        DtIoctlLedBCmdInput  m_Input;
        DtIoctlLedBCmdOutput  m_Output;
    }  DtIoctlLedBCmdInOut;
    #define DT_IOCTL_LEDB_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_LEDB_CMD,         \
                                                                      DtIoctlLedBCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_LMH1981_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Lmh1981 Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlLmh1981Cmd
{
    DT_LMH1981_CMD_GET_INPUT_STATUS = 0,
}  DtIoctlLmh1981Cmd;

// Video Formats
#define  DT_LMH1981_FORMAT_UNKNOWN  0       // Unknow format
#define  DT_LMH1981_FORMAT_525I_F1  1       // 525i (NTSC) Field 1
#define  DT_LMH1981_FORMAT_525I_F2  2       // 525i (NTSC) Field 2
#define  DT_LMH1981_FORMAT_625I_F1  3       // 625i (PAL) Field 1
#define  DT_LMH1981_FORMAT_625I_F2  4       // 625i (PAL) Field 2
#define  DT_LMH1981_FORMAT_480P     5       // 480p
#define  DT_LMH1981_FORMAT_576P     6       // 576p
#define  DT_LMH1981_FORMAT_720P     7       // 720p
#define  DT_LMH1981_FORMAT_1080I_F1 8       // 1080i Field 1
#define  DT_LMH1981_FORMAT_1080I_F2 9       // 1080i Field 2
#define  DT_LMH1981_FORMAT_1080P    10      // 1080p

// -.-.-.-.-.-.-.-.-.-.- LMH1981 Command - Get Input Status Command -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlLmh1981CmdGetInputStatusInput;
ASSERT_SIZE(DtIoctlLmh1981CmdGetInputStatusInput, 16)
typedef struct _DtIoctlLmh1981CmdGetInputStatusOutput
{
    Int  m_Valid;          // Valid; only if TRUE other fields are valid
    Int  m_LinePeriod;      // Line period in nano seconds
    Int  m_FramePeriod;     // Frame period in nano seconds
    Int  m_NumLinesF1;      // Number of lines in Field 1
    Int  m_NumLinesF2;      // Number of lines in Field 2
    Int  m_VideoFormat;     // Video format
}  DtIoctlLmh1981CmdGetInputStatusOutput;
ASSERT_SIZE(DtIoctlLmh1981CmdGetInputStatusOutput, 24)

// -.-.-.-.-.-.-.-.-.-.-.-.- LMH1981 Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// LMH1981 command - IOCTL input data

// LMH1981 command - IOCTL output data
typedef union _DtIoctlLmh1981CmdOutput
{
    DtIoctlLmh1981CmdGetInputStatusOutput  m_GetInpStatus;  // Lmh1981 - Get input status
}  DtIoctlLmh1981CmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_LMH1981_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_LMH1981_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlLmh1981CmdInOut
    {
        DtIoctlLmh1981CmdOutput  m_Output;
    }  DtIoctlLmh1981CmdInOut;
    #define DT_IOCTL_LMH1981_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_LMH1981_CMD,   \
                                                                   DtIoctlLmh1981CmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPERTY_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Property commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlPropertyCmd
{
    DT_PROP_CMD_GET_VALUE = 0,
    DT_PROP_CMD_GET_STR = 1,
    DT_PROP_CMD_GET_TABLE = 2,
}  DtIoctlPropertyCmd;

// MACRO defining common input data for all property commands
#define DT_IOCTL_PROP_COMMON_INPUT_DATA                                                  \
    DtIoctlInputDataHdr  m_CmdHdr;          /* Common input data header */               \
    Int  m_TypeNumber;                      /* Type number (-1=get for current) */       \
    Int16  m_SubDvc;                        /* Sub-device. Not used only here for */     \
                                            /* backwards compatibility. Only valid  */   \
                                            /* value is: -1, to indicate field is */     \
                                            /* not used */                               \
    Int16  m_SubType;                       /* Device subtype (0=none, 1=A, ...) */      \
    Int  m_HardwareRevision;                /* Hardware revision */                      \
    Int  m_FirmwareVersion;                 /* Firmware version */                       \
    Int  m_FirmwareVariant;                 /* Firmware variant */                       \
    Int  m_PortIndex;                       /* Port index */                             \
    char  m_Name[PROPERTY_NAME_MAX_SIZE];   /* Name of property or table */              \
    Int  m_DtapiMaj;                        /* DTAPI major version */                    \
    Int  m_DtapiMin;                        /* DTAPI minor version */                    \
    Int  m_DtapiBugfix                      /* DTAPI bug fix version */
// The struct
typedef struct _DtIoctlPropCmdCommonInput
{
    DT_IOCTL_PROP_COMMON_INPUT_DATA;
}  DtIoctlPropCmdCommonInput;
ASSERT_SIZE(DtIoctlPropCmdCommonInput, 104)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Property - Get Value Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlPropCmdCommonInput  DtIoctlPropCmdGetValueInput;
ASSERT_SIZE(DtIoctlPropCmdGetValueInput, 104)

typedef struct _DtIoctlPropCmdGetValueOutput
{
    Int  m_Scope;                           // Scope of property
    Int  m_Type;                            // Type of property
    UInt64A  m_Value;                       // Property value
}  DtIoctlPropCmdGetValueOutput;
ASSERT_SIZE(DtIoctlPropCmdGetValueOutput, 16)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Property - Get String Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlPropCmdCommonInput  DtIoctlPropCmdGetStrInput;
ASSERT_SIZE(DtIoctlPropCmdGetStrInput, 104)

typedef struct _DtIoctlPropCmdGetStrOutput 
{
    DtPropertyScope  m_Scope;               // Scope of property
    char  m_Str[PROPERTY_STR_MAX_SIZE];     // The requested string
}  DtIoctlPropCmdGetStrOutput;
ASSERT_SIZE(DtIoctlPropCmdGetStrOutput, 100)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Property - Get Table Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlPropCmdGetTableInput
{
    DT_IOCTL_PROP_COMMON_INPUT_DATA;
    Int  m_MaxNumEntries;                   // Max. number of entry's to get
}  DtIoctlPropCmdGetTableInput;
ASSERT_SIZE(DtIoctlPropCmdGetTableInput, 108)

typedef struct _DtIoctlPropCmdGetTableOutput 
{
    Int  m_NumEntries;
    DtTableEntry  m_TableEntry[0];
} DtIoctlPropCmdGetTableOutput;
ASSERT_SIZE(DtIoctlPropCmdGetTableOutput, 8)

//-.-.-.-.-.-.-.-.-.-.-.-.- Property command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// Property command - Output data
typedef union _DtIoctlPropCmdInput
{
    DtIoctlPropCmdGetValueInput  m_GetValue;    // Property - Get value command
    DtIoctlPropCmdGetStrInput  m_GetStr;        // Property - Get string command
    DtIoctlPropCmdGetTableInput  m_GetTable;    // Property - Get table command
}  DtIoctlPropCmdInput;

// Property command - Output data
typedef union _DtIoctlPropCmdOutput
{
    DtIoctlPropCmdGetValueOutput  m_GetValue;   // Property - Get value command
    DtIoctlPropCmdGetStrOutput  m_GetStr;       // Property - Get string command
    DtIoctlPropCmdGetTableOutput  m_GetTable;   // Property - Get table command
}  DtIoctlPropCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_PROPERTY_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_PROPERTY_CMD,   \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlPropertyCmdInOut
    {
        DtIoctlPropCmdInput  m_Input;
        DtIoctlPropCmdOutput  m_Output;
    }  DtIoctlPropertyCmdInOut;

    #define DT_IOCTL_PROPERTY_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_PROPERTY_CMD, \
                                                                  DtIoctlPropertyCmdInOut)
#endif
    
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_CMD_TRIGEVT_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// 
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TRIGEVT Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlTrigEvtCmd
{
    DT_TRIGEVT_CMD_INIT = 0,
    DT_TRIGEVT_CMD_CLOSE = 1,
}  DtIoctlTrigEvtCmd;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TRIGEVT Command - Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlTrigEvtCmdInitInput
{
    DtIoctlInputDataHdr m_CmdHdr;
    DtTrigEvtDriver m_TrigEvt;
} DtIoctlTrigEvtCmdInitInput;
ASSERT_SIZE(DtIoctlTrigEvtCmdInitInput, 48)

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TRIGEVT Command - Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlTrigEvtCmdCloseInput
{
    DtIoctlInputDataHdr m_CmdHdr;
    DtTrigEvtDriver m_TrigEvt;
} DtIoctlTrigEvtCmdCloseInput;
ASSERT_SIZE(DtIoctlTrigEvtCmdCloseInput, 48)

// -.-.-.-.-.-.-.-.-.-.-.-.- TRIGEVT Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// TRIGEVT command - IOCTL input data
typedef union _DtIoctlTrigEvtCmdInput
{
    DtIoctlTrigEvtCmdInitInput  m_Init;
    DtIoctlTrigEvtCmdCloseInput  m_Close;
}  DtIoctlTrigEvtCmdInput;


#ifdef WINBUILD
#define DT_IOCTL_TRIGEVT_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_TRIGEVT_CMD,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlTrigEvtCmdInOut
{
    DtIoctlTrigEvtCmdInput  m_Input;
}  DtIoctlTrigEvtCmdInOut;
#define DT_IOCTL_TRIGEVT_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_TRIGEVT_CMD,       \
                                                                   DtIoctlTrigEvtCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_EMAC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Generic phy defines
// Phy speed
#define DT_PHY_SPEED_10_HALF          0x0
#define DT_PHY_SPEED_10_FULL          0x1
#define DT_PHY_SPEED_100_HALF         0x2
#define DT_PHY_SPEED_100_FULL         0x3
#define DT_PHY_SPEED_1000_MASTER      0x4
#define DT_PHY_SPEED_1000_SLAVE       0x5
#define DT_PHY_SPEED_NOT_SET          0x6     // Init
#define DT_PHY_SPEED_AUTO_DETECT      0x6     // Set speed
#define DT_PHY_SPEED_NO_LINK          0x6     // Get speed
#define DT_PHY_SPEED_10000            0x7

// Phy link status
#define DT_PHY_LINK_DOWN              0x0
#define DT_PHY_LINK_UP                0x1
#define DT_PHY_LINK_NOT_SET           0x2

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EMAC Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlEMACCmd
{
    DT_EMAC_CMD_GET_OPERATIONAL_MODE = 0,
    DT_EMAC_CMD_SET_OPERATIONAL_MODE = 1,
    DT_EMAC_CMD_GET_MACADDRESS_PERM = 2,
    DT_EMAC_CMD_SET_MACADDRESS = 3,
    DT_EMAC_CMD_GET_MACADDRESS = 4,
    DT_EMAC_CMD_GET_MAX_MULTICAST_LIST = 5,
    DT_EMAC_CMD_SET_MULTICAST_LIST = 6,
    DT_EMAC_CMD_SET_PACKET_FILTER = 7,
    DT_EMAC_CMD_GET_PACKET_FILTER = 8,
    DT_EMAC_CMD_GET_COUNTER = 9,
    DT_EMAC_CMD_GET_LINKSTATUS = 10,
    DT_EMAC_CMD_GET_PHY_SPEED = 11,
    DT_EMAC_CMD_SET_PHY_SPEED = 12,
    DT_EMAC_CMD_REGISTER_CALLBACK = 13,
    DT_EMAC_CMD_UNREGISTER_CALLBACK = 14,
    DT_EMAC_CMD_RESET_NW_STATUS = 15
}  DtIoctlEMACCmd;

// -.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlEMACCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlEMACCmdGetOpModeInput, 16)
typedef struct _DtIoctlEMACCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlEMACCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlEMACCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Set  Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlEMACCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlEMACCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlEMACCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.-.-.-.- EMAC Command - Get Perminent MAC Address -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlEMACCmdGetMacAddressPermInput {
    DtIoctlInputDataHdr  m_CmdHdr;
} DtIoctlEMACCmdGetMacAddressPermInput;
ASSERT_SIZE(DtIoctlEMACCmdGetMacAddressPermInput, 16)

typedef struct _DtIoctlEMACCmdGetMacAddressPermOutput {
    UInt8  m_Address[6];
} DtIoctlEMACCmdGetMacAddressPermOutput;
ASSERT_SIZE(DtIoctlEMACCmdGetMacAddressPermOutput, 6)

// -.-.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Set MAC Address -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlEMACCmdSetMacAddressInput {
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt8  m_Address[6];
    UInt8  m_Reserved[2];   // for alignment
} DtIoctlEMACCmdSetMacAddressInput;
ASSERT_SIZE(DtIoctlEMACCmdSetMacAddressInput, 24)

// -.-.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Get MAC Address -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlEMACCmdGetMacAddressInput {
    DtIoctlInputDataHdr  m_CmdHdr;
} DtIoctlEMACCmdGetMacAddressInput;
ASSERT_SIZE(DtIoctlEMACCmdGetMacAddressInput, 16)

typedef struct _DtIoctlEMACCmdGetMacAddressOutput {
    UInt8  m_Address[6];
} DtIoctlEMACCmdGetMacAddressOutput;
ASSERT_SIZE(DtIoctlEMACCmdGetMacAddressOutput, 6)

// -.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Get Max. Multicast List -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlEMACCmdGetMaxMulticastListInput {
    DtIoctlInputDataHdr  m_CmdHdr;
} DtIoctlEMACCmdGetMaxMulticastListInput;
ASSERT_SIZE(DtIoctlEMACCmdGetMaxMulticastListInput, 16)

typedef struct _DtIoctlEMACCmdGetMaxMulticastListOutput {
    UInt  m_MaxListSize;
} DtIoctlEMACCmdGetMaxMulticastListOutput;
ASSERT_SIZE(DtIoctlEMACCmdGetMaxMulticastListOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Set Multicast List -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlEMACCmdSetMulticastListInput {
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt  m_NumItems;
    UInt8  m_Items[0][8];
} DtIoctlEMACCmdSetMulticastListInput;
ASSERT_SIZE(DtIoctlEMACCmdSetMulticastListInput, 20)

// Packet filter defines
#define DT_MAC_FLT_NOTSET             0x00
#define DT_MAC_FLT_DIRECTED           0x01
#define DT_MAC_FLT_MULTICAST          0x02
#define DT_MAC_FLT_BROADCAST          0x04
#define DT_MAC_FLT_ALL_MULTICAST      0x08
#define DT_MAC_FLT_PROMISCUOUS        0x10

// .-.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Set Packet Filter -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlEMACCmdSetPacketFilterInput {
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt  m_PacketFilter;
} DtIoctlEMACCmdSetPacketFilterInput;
ASSERT_SIZE(DtIoctlEMACCmdSetPacketFilterInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Get Packet Filter -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlEMACCmdGetPacketFilterInput {
    DtIoctlInputDataHdr  m_CmdHdr;
} DtIoctlEMACCmdGetPacketFilterInput;
ASSERT_SIZE(DtIoctlEMACCmdGetPacketFilterInput, 16)
typedef struct _DtIoctlEMACCmdGetPacketFilterOutput {
    UInt  m_PacketFilter;
} DtIoctlEMACCmdGetPacketFilterOutput;
ASSERT_SIZE(DtIoctlEMACCmdGetPacketFilterOutput, 4)

// Counter defines
#define DT_MAC_CNT_GEN_BYTES_RCV                  1
#define DT_MAC_CNT_GEN_BYTES_XMIT                 2
#define DT_MAC_CNT_GEN_XMIT_OK                    3
#define DT_MAC_CNT_GEN_RCV_OK                     4
#define DT_MAC_CNT_GEN_XMIT_ERROR                 5
#define DT_MAC_CNT_GEN_RCV_ERROR                  6
#define DT_MAC_CNT_GEN_RCV_NO_BUFFER              7
#define DT_MAC_CNT_GEN_RCV_CRC_ERROR              8
#define DT_MAC_CNT_GEN_TRANSMIT_QUEUE_LENGTH      9
#define DT_MAC_CNT_802_3_RCV_ERROR_ALIGNMENT      10
#define DT_MAC_CNT_802_3_XMIT_ONE_COLLISION       11
#define DT_MAC_CNT_802_3_XMIT_MORE_COLLISIONS     12
#define DT_MAC_CNT_802_3_XMIT_DEFERRED            13
#define DT_MAC_CNT_802_3_XMIT_MAX_COLLISIONS      14
#define DT_MAC_CNT_802_3_RCV_OVERRUN              15
#define DT_MAC_CNT_802_3_XMIT_UNDERRUN            16
#define DT_MAC_CNT_802_3_XMIT_HEARTBEAT_FAILURE   17
#define DT_MAC_CNT_802_3_XMIT_TIMES_CRS_LOST      18
#define DT_MAC_CNT_802_3_XMIT_LATE_COLLISIONS     19
#define DT_MAC_CNT_GEN_RCV_HDR_ERROR              20

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Get Counter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlEMACCmdGetCounterInput {
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt  m_CounterId;
} DtIoctlEMACCmdGetCounterInput;
ASSERT_SIZE(DtIoctlEMACCmdGetCounterInput, 20)

typedef struct _DtIoctlEMACCmdGetCounterOutput {
    UInt64A  m_Value;
} DtIoctlEMACCmdGetCounterOutput;
ASSERT_SIZE(DtIoctlEMACCmdGetCounterOutput, 8)

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Get LinkStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlEMACCmdGetLinkStatusInput {
    DtIoctlInputDataHdr  m_CmdHdr;
} DtIoctlEMACCmdGetLinkStatusInput;
ASSERT_SIZE(DtIoctlEMACCmdGetLinkStatusInput, 16)

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Get PHY Speed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlEMACCmdGetPhySpeedInput {
    DtIoctlInputDataHdr  m_CmdHdr;
} DtIoctlEMACCmdGetPhySpeedInput;
ASSERT_SIZE(DtIoctlEMACCmdGetPhySpeedInput, 16)

typedef struct _DtIoctlEMACCmdGetPhySpeedOutput {
    UInt  m_Speed;              // DT_PHY_SPEED_ constants
} DtIoctlEMACCmdGetPhySpeedOutput;
ASSERT_SIZE(DtIoctlEMACCmdGetPhySpeedOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Set PHY Speed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlEMACCmdSetPhySpeedInput {
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt  m_Speed;              // DT_PHY_SPEED_ constants
} DtIoctlEMACCmdSetPhySpeedInput;
ASSERT_SIZE(DtIoctlEMACCmdSetPhySpeedInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Register callback -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef void  (*DtLinkStatFunc)(UInt PhyLinkStatus, void* pContext);
typedef struct _DtIoctlEMACCmdRegisterCallbackInput {
    DtIoctlInputDataHdr  m_CmdHdr;
    void* m_pContext;
    DtLinkStatFunc  m_LinkStatusCallback;
} DtIoctlEMACCmdRegisterCallbackInput;

// -.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Unregister callback -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlEMACCmdUnregisterCallbackInput {
    DtIoctlInputDataHdr  m_CmdHdr;
    void* m_pContext;
} DtIoctlEMACCmdUnregisterCallbackInput;

// -.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - Reset network status -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlEMACCmdResetNwStatusInput {
    DtIoctlInputDataHdr  m_CmdHdr;
} DtIoctlEMACCmdResetNwStatusInput;


// .-.-.-.-.-.-.-.-.-.-.-.-.- EMAC Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-
// EMAC10G command - IOCTL input data
typedef union _DtIoctlEMACCmdInput
{
    DtIoctlEMACCmdGetOpModeInput  m_GetOpMode;
    DtIoctlEMACCmdSetOpModeInput  m_SetOpMode;
    DtIoctlEMACCmdGetMacAddressPermInput  m_GetMacAddressPerm;
    DtIoctlEMACCmdSetMacAddressInput  m_SetMacAddress;
    DtIoctlEMACCmdGetMacAddressInput  m_GetMacAddress;
    DtIoctlEMACCmdGetMaxMulticastListInput  m_GetMaxMulticastList;
    DtIoctlEMACCmdSetMulticastListInput  m_SetMulticastList;
    DtIoctlEMACCmdGetPacketFilterInput  m_GetPacketFilter;
    DtIoctlEMACCmdSetPacketFilterInput  m_SetPacketFilter;
    DtIoctlEMACCmdGetCounterInput  m_GetCounter;
    DtIoctlEMACCmdGetPhySpeedInput  m_GetPhySpeed;
    DtIoctlEMACCmdSetPhySpeedInput  m_SetPhySpeed;
    DtIoctlEMACCmdRegisterCallbackInput  m_RegisterCallback;
    DtIoctlEMACCmdUnregisterCallbackInput  m_UnregisterCallback;
    DtIoctlEMACCmdResetNwStatusInput  m_ResetNwStatus;
}  DtIoctlEMACCmdInput;

// EMAC10G command - IOCTL output data
typedef union _DtIoctlEMACCmdOutput
{
    DtIoctlEMACCmdGetOpModeOutput  m_GetOpMode;
    DtIoctlEMACCmdGetMacAddressPermOutput  m_GetMacAddressPerm;
    DtIoctlEMACCmdGetMacAddressOutput  m_GetMacAddress;
    DtIoctlEMACCmdGetMaxMulticastListOutput  m_GetMaxMulticastList;
    DtIoctlEMACCmdGetPacketFilterOutput  m_GetPacketFilter;
    DtIoctlEMACCmdGetCounterOutput  m_GetCounter;
    DtIoctlEMACCmdGetPhySpeedOutput  m_GetPhySpeed;
}  DtIoctlEMACCmdOutput;


#ifdef WINBUILD
    #define DT_IOCTL_EMAC_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_EMAC_CMD,           \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlEMACCmdInOut
    {
        DtIoctlEMACCmdInput  m_Input;
        DtIoctlEMACCmdOutput  m_Output;
    }  DtIoctlEMACCmdInOut;
    #define DT_IOCTL_EMAC_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_EMAC_CMD,         \
                                                                      DtIoctlEMACCmdInOut)
#endif



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PIPE_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlPipeCmd
{
    DT_PIPE_CMD_SET_SHARED_BUFFER = 0,
    DT_PIPE_CMD_SET_SHAREDK_BUFFER = 1,
    DT_PIPE_CMD_RELEASE_SHARED_BUFFER = 2,
    DT_PIPE_CMD_GET_PROPERTIES = 3,
    DT_PIPE_CMD_GET_STATUS = 4,
    DT_PIPE_CMD_ISSUE_PIPE_FLUSH = 5,
    DT_PIPE_CMD_SET_OPERATIONAL_MODE = 6,
    DT_PIPE_CMD_SET_RX_READ_OFFSET = 7,
    DT_PIPE_CMD_GET_RX_WRITE_OFFSET = 8,
    DT_PIPE_CMD_SET_TX_WRITE_OFFSET = 9,
    DT_PIPE_CMD_GET_TX_READ_OFFSET = 10,
    DT_PIPE_CMD_REGISTER_EVENT = 11,
    DT_PIPE_CMD_DEREGISTER_EVENT = 12,
    DT_PIPE_CMD_SET_IPFILTER = 13,
}  DtIoctlPipeCmd;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Pipe types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef enum _DtPipeType
{
    DT_PIPE_RX_NRT=1,
    DT_PIPE_TX_NRT=2,
    DT_PIPE_RX_RT_HWP=3,
    DT_PIPE_RX_RT_SWP=4,
    DT_PIPE_TX_RT_HWP=5,
    DT_PIPE_TX_RT_SWP=6,
    DT_PIPE_RX_HWQ=7,
    DT_PIPE_TX_RT_HWQ=8,
} DtPipeType;

// Pipe capability flags
#define DT_PIPE_CAP_RX   0x00001        // Receive capability
#define DT_PIPE_CAP_TX   0x00002        // Transmit capability
#define DT_PIPE_CAP_RT   0x00004        // Realtime capability
#define DT_PIPE_CAP_NRT  0x00008        // Non-realtime capability
#define DT_PIPE_CAP_HWP  0x00010        // Hardware pipe
#define DT_PIPE_CAP_SWP  0x00020        // Software pipe
#define DT_PIPE_CAP_KBUF 0x00100        // Kernel shared buffer

// Pipe status flags
#define DT_PIPE_STATUS_WAIT_PCIE      0x01 // Waiting for data to/from the PCI Express bus 
#define DT_PIPE_STATUS_WAIT_LOCAL     0x02 // Waiting for local channel to provide data
#define DT_PIPE_STATUS_PACKET_WAITING 0x10 // A packet is present at scheduler output and is waiting for its transmit time.
#define DT_PIPE_STATUS_PACKET_READY   0x20 // A packet is present at scheduler output and ready for transmission. It is still waiting for arbitration
#define DT_PIPE_STATUS_PACKET_SENDING 0x40 // A packet is being sent out of scheduler output


// Pipe error flags
#define DT_PIPE_ERROR_ALIGN        0x001 // Address pointer is not aligned to a 4Kbyte page
#define DT_PIPE_ERROR_TIMEOUT      0x002 // DMA access timed out on PCIe bus
#define DT_PIPE_ERROR_UNDEFINED    0x004 // DMA access ended with an undefined status
#define DT_PIPE_ERROR_OVERFLOW     0x080 // Buffer overflow
#define DT_PIPE_ERROR_INVALID_TIME 0x100 // TX Scheduler detected a timestamp error and pipe stalled

// .-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - Set Shared Buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlPipeCmdSetSharedBufferInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt64A  m_BufferAddr;  // Buffer address (Linux only)
    Int  m_BufferSize;      // Allocated buffer size
    Int  m_Spare;
}  DtIoctlPipeCmdSetSharedBufferInput;
ASSERT_SIZE(DtIoctlPipeCmdSetSharedBufferInput, 32)

typedef struct _DtIoctlPipeCmdSetSharedBufferOutput
{
    UInt8  m_Buf[4];            // Dummy dynamic sized buffer
                                // On Windows used for passing the DMA buffer;
                                // on Linux not used
}  DtIoctlPipeCmdSetSharedBufferOutput;
ASSERT_SIZE(DtIoctlPipeCmdSetSharedBufferOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - Set SharedK Buffer -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlPipeCmdSetSharedKBufferInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_RequestedBufferSize;
}  DtIoctlPipeCmdSetSharedKBufferInput;
ASSERT_SIZE(DtIoctlPipeCmdSetSharedKBufferInput, 20)

// Drivers are always 32-bit or 64-bit, not combination of 32/64
typedef struct _DtIoctlPipeCmdSetSharedKBufferOutput
{
    void*  m_pBufferAddr;
    Int  m_ActualBufferSize;
    Int  m_MaxLoad;
}  DtIoctlPipeCmdSetSharedKBufferOutput;

//ASSERT_SIZE(DtIoctlPipeCmdSetSharedKBufferOutput, 16)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - Release Shared Buffer-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlPipeCmdReleaseSharedBufferInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlPipeCmdReleaseSharedBufferInput;
ASSERT_SIZE(DtIoctlPipeCmdReleaseSharedBufferInput, 16)

//.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlPipeCmdGetPropertiesInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlPipeCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlPipeCmdGetPropertiesInput, 16)
typedef struct _DtIoctlPipeCmdGetPropertiesOutput
{
    UInt32  m_Capabilities;     // Pipe capability flags
    Int  m_PrefetchSize;        // Pipe prefetch size
    Int  m_PipeDataWidth;       // Pipe interface data width
    Int  m_PipeType;            // Pipe type
}  DtIoctlPipeCmdGetPropertiesOutput;
ASSERT_SIZE(DtIoctlPipeCmdGetPropertiesOutput, 16)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - Get Status -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlPipeCmdGetStatusInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlPipeCmdGetStatusInput;
ASSERT_SIZE(DtIoctlPipeCmdGetStatusInput, 16)
typedef struct _DtIoctlPipeCmdGetStatusOutput
{
    Int  m_OpStatus;            // Operational status
    UInt32  m_StatusFlags;      // Status flags
    UInt32  m_ErrorFlags;       // Error flags
}  DtIoctlPipeCmdGetStatusOutput;
ASSERT_SIZE(DtIoctlPipeCmdGetStatusOutput, 12)

//-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - Issue Pipe Flush -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlPipeCmdIssuePipeFlushInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlPipeCmdIssuePipeFlushInput;
ASSERT_SIZE(DtIoctlPipeCmdIssuePipeFlushInput, 16)

#define DT_PIPE_OPMODE_IDLE        0
#define DT_PIPE_OPMODE_STANDBY     1
#define DT_PIPE_OPMODE_RUN         2

//-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - Set Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlPipeCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlPipeCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlPipeCmdSetOpModeInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - Set Rx Read Offset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlPipeCmdSetRxReadOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt  m_RxReadOffset;       // Rx read offset
}  DtIoctlPipeCmdSetRxReadOffsetInput;
ASSERT_SIZE(DtIoctlPipeCmdSetRxReadOffsetInput, 20)

//-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - Get Rx Write Offset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlPipeCmdGetRxWriteOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlPipeCmdGetRxWriteOffsetInput;
ASSERT_SIZE(DtIoctlPipeCmdGetRxWriteOffsetInput, 16)
typedef struct _DtIoctlPipeCmdGetRxWriteOffsetOutput
{
    UInt  m_RxWriteOffset;      //  Rx write offset
}  DtIoctlPipeCmdGetRxWriteOffsetOutput;
ASSERT_SIZE(DtIoctlPipeCmdGetRxWriteOffsetOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - Set Tx Write Offset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlPipeCmdSetTxWriteOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt  m_TxWriteOffset;      // Tx write offset
}  DtIoctlPipeCmdSetTxWriteOffsetInput;
ASSERT_SIZE(DtIoctlPipeCmdSetTxWriteOffsetInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - Get Tx Read Offset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlPipeCmdGtTxReadOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlPipeCmdGetTxReadOffsetInput;
ASSERT_SIZE(DtIoctlPipeCmdGetTxReadOffsetInput, 16)
typedef struct _DtIoctlPipeCmdGetTxReadOffsetOutput
{
    UInt  m_TxReadOffset;      //  Tx read offset
}  DtIoctlPipeCmdGetTxReadOffsetOutput;
ASSERT_SIZE(DtIoctlPipeCmdGetTxReadOffsetOutput, 4)


#define DT_PIPE_EVENT_ID_PERINT            1       // Periodic event
#define DT_PIPE_EVENT_ID_DATA_AVAIL_CNT    2       // Data-available event. Ones per num.IP's
#define DT_PIPE_EVENT_ID_DATA_AVAIL_INT    3       // Data-available event. Ones per int.
#define DT_PIPE_EVENT_ID_AUTO              4       // HWP:PerInt, SWP:Data-available thr.
//#define DT_PIPE_EVENT_ID_DATA_AVAIL_THR50  5       // Data-available event. Ones per int. or >50% load

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - RegisterEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlPipeCmdRegisterEventInput
{
    DtIoctlInputDataHdr m_CmdHdr;
    DtTrigEvtDriver m_TrigEvt;
    UInt m_Count;
} DtIoctlPipeCmdRegisterEventInput;
ASSERT_SIZE(DtIoctlPipeCmdRegisterEventInput, 56)

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - DeregisterEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlPipeCmdDeregisterEventInput
{
    DtIoctlInputDataHdr m_CmdHdr;
    DtTrigEvtDriver m_TrigEvt;
} DtIoctlPipeCmdDeregisterEventInput;
ASSERT_SIZE(DtIoctlPipeCmdDeregisterEventInput, 48)

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - Set IpFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// IP filter flags
#define DT_PIPE_IPFLT_FLAG_EN_FILT         0x0001
#define DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV4   0x0002
#define DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV6   0x0004
#define DT_PIPE_IPFLT_FLAG_EN_SRCPORT0     0x0008
#define DT_PIPE_IPFLT_FLAG_EN_SRCPORT1     0x0010
#define DT_PIPE_IPFLT_FLAG_EN_SRCPORT2     0x0020
#define DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV4   0x0040
#define DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV6   0x0080
#define DT_PIPE_IPFLT_FLAG_EN_DSTPORT0     0x0100
#define DT_PIPE_IPFLT_FLAG_EN_DSTPORT1     0x0200
#define DT_PIPE_IPFLT_FLAG_EN_DSTPORT2     0x0400
#define DT_PIPE_IPFLT_FLAG_EN_VLAN         0x0800
#define DT_PIPE_IPFLT_FLAG_DUPLICATE       0x1000
#define DT_PIPE_IPFLT_FLAG_VLAN0_1AD       0x2000
#define DT_PIPE_IPFLT_FLAG_VLAN1_1AD       0x4000

typedef struct _DtIoctlPipeCmdSetIpFilterInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt8  m_DstIp[16];             // Destination: IP address
    UInt16  m_DstPort[3];           // Destination: port number
    UInt8  m_SrcIp[16];             // Source: IP address
    UInt16  m_SrcPort[3];           // Source: port number
    Int  m_VlanId[2];               // VLAN ID
    Int  m_Flags;                   // Control flags: IPv4/IPv6/Enable etc.
}  DtIoctlPipeCmdSetIpFilterInput;
ASSERT_SIZE(DtIoctlPipeCmdSetIpFilterInput, 72)

//.-.-.-.-.-.-.-.-.-.-.-.-.- PIPE Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-
// PIPE command - IOCTL input data
typedef union _DtIoctlPipeCmdInput
{
    DtIoctlPipeCmdSetSharedBufferInput  m_SetSharedBuffer;
    DtIoctlPipeCmdSetSharedKBufferInput  m_SetSharedKBuffer;
    DtIoctlPipeCmdSetOpModeInput  m_SetOpMode;
    DtIoctlPipeCmdSetRxReadOffsetInput  m_SetRxReadOffset;
    DtIoctlPipeCmdSetTxWriteOffsetInput  m_SetTxWriteOffset;
    DtIoctlPipeCmdRegisterEventInput  m_RegisterEvent;
    DtIoctlPipeCmdDeregisterEventInput  m_DeregisterEvent;
    DtIoctlPipeCmdSetIpFilterInput  m_SetIpFilter;
}  DtIoctlPipeCmdInput;
// PIPE command - IOCTL output data
typedef union _DtIoctlPipeCmdOutput
{
    DtIoctlPipeCmdGetPropertiesOutput  m_GetProperties;
    DtIoctlPipeCmdGetRxWriteOffsetOutput  m_GetRxWriteOffset;
    DtIoctlPipeCmdGetStatusOutput  m_GetStatus;
    DtIoctlPipeCmdGetTxReadOffsetOutput  m_GetTxReadOffset;
    DtIoctlPipeCmdSetSharedKBufferOutput  m_SetSharedKBuffer;
}  DtIoctlPipeCmdOutput;
#ifdef WINBUILD
#define DT_IOCTL_PIPE_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_PIPE_CMD,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlPipeCmdInOut
{
    DtIoctlPipeCmdInput  m_Input;
    DtIoctlPipeCmdOutput  m_Output;
}  DtIoctlPipeCmdInOut;
#define DT_IOCTL_PIPE_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_PIPE_CMD,       \
                                                                      DtIoctlPipeCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DT_IOCTL_NW_CMD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Nw Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlNwCmd
{
    DT_NW_CMD_GET_OPERATIONAL_MODE = 0,
    DT_NW_CMD_SET_OPERATIONAL_MODE = 1,
    DT_NW_CMD_PIPE_OPEN = 2,
    DT_NW_CMD_PIPE_CLOSE = 3,
    DT_NW_CMD_REGISTER_OPMODE_CALLBACK = 4,
}  DtIoctlNwCmd;

// .-.-.-.-.-.-.-.-.-.-.-.-.- Nw Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlNwCmdGetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlNwCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlNwCmdGetOpModeInput, 16)
typedef struct _DtIoctlNwCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlNwCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlNwCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.-.- Nw Command - Set Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlNwCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlNwCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlNwCmdSetOpModeInput, 20)

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Nw Command - Pipe Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlNwCmdPipeOpenInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_PipeType;
    Int  m_PipeTypeFallback;    // -1 if no fallback
}  DtIoctlNwCmdPipeOpenInput;
ASSERT_SIZE(DtIoctlNwCmdPipeOpenInput, 24)
typedef struct _DtIoctlNwCmdPipeOpenOutput
{
    UInt  m_PipeUuid;
}  DtIoctlNwCmdPipeOpenOutput;
ASSERT_SIZE(DtIoctlNwCmdPipeOpenOutput, 4)

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Nw Command - Pipe Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlNwCmdPipeCloseInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlNwCmdPipeCloseInput;
ASSERT_SIZE(DtIoctlNwCmdPipeCloseInput, 16)

// -.-.-.-.-.-.-.-.-.-.-.- Nw Command - Register OpMode callback -.-.-.-.-.-.-.-.-.-.-.-
//
typedef void  (*OpModeFunc)(UInt OpMode, void* pContext);
typedef struct _DtIoctlNwCmdRegisterOpModeCallbackInput {
    DtIoctlInputDataHdr  m_CmdHdr;
    void* m_pContext;
    OpModeFunc  m_OpModeCallback;
} DtIoctlNwCmdRegisterOpModeCallbackInput;

// -.-.-.-.-.-.-.-.-.-.-.-.-.- Nw Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Nw command - IOCTL input data
typedef union _DtIoctlNwCmdInput
{
    DtIoctlNwCmdGetOpModeInput  m_GetOpMode;
    DtIoctlNwCmdSetOpModeInput  m_SetOpMode;
    DtIoctlNwCmdPipeOpenInput  m_PipeOpen;
    DtIoctlNwCmdPipeCloseInput  m_PipeClose;
    DtIoctlNwCmdRegisterOpModeCallbackInput  m_RegisterOpModeCallback;
}  DtIoctlNwCmdInput;

// Nw command - IOCTL output data
typedef union _DtIoctlNwCmdOutput
{
    DtIoctlNwCmdGetOpModeOutput  m_GetOpMode;
    DtIoctlNwCmdPipeOpenOutput  m_PipeOpen;
}  DtIoctlNwCmdOutput;

#ifdef WINBUILD
#define DT_IOCTL_NW_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_NW_CMD,             \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlNwCmdInOut
{
    DtIoctlNwCmdInput  m_Input;
    DtIoctlNwCmdOutput  m_Output;
}  DtIoctlNwCmdInOut;
#define DT_IOCTL_NW_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_NW_CMD,           \
                                                                        DtIoctlNwCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DT_IOCTL_DDRFRONT_CMD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DDRFRONT Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlDdrFrontCmd
{
    DT_DDRFRONT_CMD_READ_ADDRESS = 0,
    DT_DDRFRONT_CMD_WRITE_ADDRESS = 1,
    DT_DDRFRONT_CMD_GET_PROPERTIES = 2
}  DtIoctlDdrFrontCmd;


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DDRFRONT Command - Read address -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlDdrFrontCmdReadAddressInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Address;
    Int  m_Num32bItemsToRead;
}  DtIoctlDdrFrontCmdReadAddressInput;
ASSERT_SIZE(DtIoctlDdrFrontCmdReadAddressInput, 24)

typedef struct _DtIoctlDdrFrontCmdReadAddressOutput
{
    Int     m_Num32bItemsRead;
    UInt32  m_Buf[0];  // Dynamic sized buffer
}  DtIoctlDdrFrontCmdReadAddressOutput;
ASSERT_SIZE(DtIoctlDdrFrontCmdReadAddressOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.-.- DDRFRONT Command - Write address -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlDdrFrontCmdWriteAddressInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt32  m_Address;
    Int     m_Num32bItemsToWrite;
    UInt32  m_Buf[0];  // Dynamic sized buffer
}  DtIoctlDdrFrontCmdWriteAddressInput;
ASSERT_SIZE(DtIoctlDdrFrontCmdWriteAddressInput, 24)

// .-.-.-.-.-.-.-.-.-.-.- DDRFRONT Command - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlDdrFrontCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlDdrFrontCmdGetPropertiesInput, 16)
typedef struct _DtIoctlDdrFrontCmdGetPropertiesOutput
{
    Int  m_DataWidth;       // Data width in #bits
    Int64A  m_MemorySize;   // Size in #bytes
}  DtIoctlDdrFrontCmdGetPropertiesOutput;
ASSERT_SIZE(DtIoctlDdrFrontCmdGetPropertiesOutput, 16)


// .-.-.-.-.-.-.-.-.-.-.-.- DDRFRONT Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// DDRFRONT command - IOCTL input data
typedef union _DtIoctlDdrFrontCmdInput
{
    DtIoctlDdrFrontCmdReadAddressInput  m_ReadAddress;
    DtIoctlDdrFrontCmdWriteAddressInput  m_WriteAddress;
}  DtIoctlDdrFrontCmdInput;

// IPROUT command - IOCTL output data
typedef union _DtIoctlDdrFrontCmdOutput
{
    DtIoctlDdrFrontCmdReadAddressOutput  m_ReadAddress;
    DtIoctlDdrFrontCmdGetPropertiesOutput  m_GetProperties; // Get properties
}  DtIoctlDdrFrontCmdOutput;

#ifdef WINBUILD
#define DT_IOCTL_DDRFRONT_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_DDRFRONT_CMD,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlDdrFrontCmdInOut
{
    DtIoctlDdrFrontCmdInput  m_Input;
    DtIoctlDdrFrontCmdOutput  m_Output;
}  DtIoctlDdrFrontCmdInOut;
#define DT_IOCTL_DDRFRONT_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_DDRFRONT_CMD,     \
                                                                  DtIoctlDdrFrontCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_REBOOT_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Reboot Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlRebootCmd
{
    DT_REBOOT_CMD_FPGA_REBOOT = 0,
}  DtIoctlRebootCmd;
//-.-.-.-.-.-.-.-.-.-.-.-.- REBOOT Command - FPGA Reboot Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlRebootCmdFpgaRebootInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_RebootDelay;                 // The delay in ms the FPGA reboot starts
} DtIoctlRebootCmdFpgaRebootInput;
ASSERT_SIZE(DtIoctlRebootCmdFpgaRebootInput, 20)
//.-.-.-.-.-.-.-.-.-.-.-.-.- REBOOT Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// REBOOT command - IOCTL input data
typedef union _DtIoctlRebootCmdInput
{
    DtIoctlRebootCmdFpgaRebootInput  m_FpgaReboot;         // REBOOT - FPGA reboot
}  DtIoctlRebootCmdInput;
#ifdef WINBUILD
    #define DT_IOCTL_REBOOT_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_REBOOT_CMD,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlRebootCmdInOut
    {
        DtIoctlRebootCmdInput  m_Input;
    }  DtIoctlRebootCmdInOut;
    #define DT_IOCTL_REBOOT_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_REBOOT_CMD,     \
                                                                    DtIoctlRebootCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_S12GTO3G_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- S12Gto3G Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlS12Gto3GCmd
{
    DT_S12GTO3G_CMD_GET_OPERATIONAL_MODE = 0,
    DT_S12GTO3G_CMD_GET_SCALING_ENABLE = 1,
    DT_S12GTO3G_CMD_SET_OPERATIONAL_MODE = 2,
    DT_S12GTO3G_CMD_SET_SCALING_ENABLE = 3,
}  DtIoctlS12Gto3GCmd;

// -.-.-.-.-.-.-.-.-.- S12GTO3G Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlS12Gto3GCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlS12Gto3GCmdGetOpModeInput, 16)
typedef struct _DtIoctlS12Gto3GCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlS12Gto3GCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlS12Gto3GCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.-.- S12GTO3G Command - Set ScalingEnable Command -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlS12Gto3GCmdSetScalingEnableInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_ScalingEnable;           // Scaling enable (TRUE or FALSE)
}  DtIoctlS12Gto3GCmdSetScalingEnableInput;
ASSERT_SIZE(DtIoctlS12Gto3GCmdSetScalingEnableInput, 20)

// .-.-.-.-.-.-.-.-.-.- S12GTO3G Command - Get ScalingEnable Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlS12Gto3GCmdGetScalingEnableInput;
ASSERT_SIZE(DtIoctlS12Gto3GCmdGetScalingEnableInput, 16)
typedef struct _DtIoctlS12Gto3GCmdGetScalingEnableOutput
{
    Int  m_ScalingEnable;           // Scaling enable (TRUE or FALSE)
}  DtIoctlS12Gto3GCmdGetScalingEnableOutput;
ASSERT_SIZE(DtIoctlS12Gto3GCmdGetScalingEnableOutput, 4)

// .-.-.-.-.-.-.-.-.- S12GTO3G Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlS12Gto3GCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlS12Gto3GCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlS12Gto3GCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.- S12GTO3G Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// S12GTO3G command - IOCTL input data
typedef union _DtIoctlS12Gto3GCmdInput
{
    DtIoctlS12Gto3GCmdSetScalingEnableInput  m_SetScalingEna;  // Set ScalingEnable
    DtIoctlS12Gto3GCmdSetOpModeInput  m_SetOpMode;             // Set operational mode
}  DtIoctlS12Gto3GCmdInput;

// S12GTO3G command - IOCTL output data
typedef union _DtIoctlS12Gto3GCmdOutput
{
    DtIoctlS12Gto3GCmdGetScalingEnableOutput  m_GetScalingEna;  // Get ScalingEnable
    DtIoctlS12Gto3GCmdGetOpModeOutput  m_GetOpMode;             // Get operational mode
}  DtIoctlS12Gto3GCmdOutput;


#ifdef WINBUILD
    #define DT_IOCTL_S12GTO3G_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_S12GTO3G_CMD,   \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlS12Gto3GCmdInOut
    {
        DtIoctlS12Gto3GCmdInput  m_Input;
        DtIoctlS12Gto3GCmdOutput  m_Output;
    }  DtIoctlS12Gto3GCmdInOut;
    #define DT_IOCTL_S12GTO3G_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_S12GTO3G_CMD, \
                                                                  DtIoctlS12Gto3GCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SDIDMX12G_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SdiDmx12G Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSdiDmx12GCmd
{
    DT_SDIDMX12G_CMD_GET_OPERATIONAL_MODE = 0,
    DT_SDIDMX12G_CMD_GET_SDIRATE = 1,
    DT_SDIDMX12G_CMD_SET_OPERATIONAL_MODE = 2,
    DT_SDIDMX12G_CMD_SET_SDIRATE = 3,
}  DtIoctlSdiDmx12GCmd;


// .-.-.-.-.-.-.-.-.- SDIDMX12G Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiDmx12GCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlSdiDmx12GCmdGetOpModeInput, 16)
typedef struct _DtIoctlSdiDmx12GCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlSdiDmx12GCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlSdiDmx12GCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.-.-.-.- SDIDMX12G Command - Get SdiRate Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiDmx12GCmdGetSdiRateInput;
ASSERT_SIZE(DtIoctlSdiDmx12GCmdGetSdiRateInput, 16)
typedef struct _DtIoctlSdiDmx12GCmdGetSdiRateOutput
{
    Int  m_SdiRate;          // SDI-Rate
}  DtIoctlSdiDmx12GCmdGetSdiRateOutput;
ASSERT_SIZE(DtIoctlSdiDmx12GCmdGetSdiRateOutput, 4)

// .-.-.-.-.-.-.-.-.- SDIDMX12G Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSdiDmx12GCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlSdiDmx12GCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlSdiDmx12GCmdSetOpModeInput, 20)

// -.-.-.-.-.-.-.-.-.-.-.- SDIDMX12G Command - Set SdiRate Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiDmx12GCmdSetSdiRateInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SdiRate;          // SDI-Rate
}  DtIoctlSdiDmx12GCmdSetSdiRateInput;
ASSERT_SIZE(DtIoctlSdiDmx12GCmdSetSdiRateInput, 20)


// .-.-.-.-.-.-.-.-.-.-.-.- SDIDMX12G Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.
// SDIDMX12G command - IOCTL input data
typedef union _DtIoctlSdiDmx12GCmdInput
{
    DtIoctlSdiDmx12GCmdSetSdiRateInput  m_SetSdiRate;   // SdiDmx12G - Set SDI-rate
    DtIoctlSdiDmx12GCmdSetOpModeInput  m_SetOpMode;     // SdiDmx12G - Set op. mode
}  DtIoctlSdiDmx12GCmdInput;

// SDIDMX12G command - IOCTL output data
typedef union _DtIoctlSdiDmx12GCmdOutput
{
    DtIoctlSdiDmx12GCmdGetSdiRateOutput  m_GetSdiRate;  // SdiDmx12G - Get SDI-Rate
    DtIoctlSdiDmx12GCmdGetOpModeOutput  m_GetOpMode;    // SdiDmx12G - Get op. mode
}  DtIoctlSdiDmx12GCmdOutput;


#ifdef WINBUILD
    #define DT_IOCTL_SDIDMX12G_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SDIDMX12G_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSdiDmx12GCmdInOut
    {
        DtIoctlSdiDmx12GCmdInput  m_Input;
        DtIoctlSdiDmx12GCmdOutput  m_Output;
    }  DtIoctlSdiDmx12GCmdInOut;
    #define DT_IOCTL_SDIDMX12G_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                           \
                                                            DT_FUNC_CODE_SDIDMX12G_CMD,  \
                                                            DtIoctlSdiDmx12GCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SDIMUX12G_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SdiMux12G Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSdiMux12GCmd
{
    DT_SDIMUX12G_CMD_GET_OPERATIONAL_MODE = 0,
    DT_SDIMUX12G_CMD_SET_OPERATIONAL_MODE = 2,
}  DtIoctlSdiMux12GCmd;


// .-.-.-.-.-.-.-.-.- SDIMUX12G Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiMux12GCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlSdiMux12GCmdGetOpModeInput, 16)
typedef struct _DtIoctlSdiMux12GCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlSdiMux12GCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlSdiMux12GCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.- SDIMUX12G Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSdiMux12GCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlSdiMux12GCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlSdiMux12GCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.- SDIMUX12G Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.
// SDIMUX12G command - IOCTL input data
typedef union _DtIoctlSdiMux12GCmdInput
{
    DtIoctlSdiMux12GCmdSetOpModeInput  m_SetOpMode;     // SdiMux12G - Set op. mode
}  DtIoctlSdiMux12GCmdInput;

// SDIMUX12G command - IOCTL output data
typedef union _DtIoctlSdiMux12GCmdOutput
{
    DtIoctlSdiMux12GCmdGetOpModeOutput  m_GetOpMode;    // SdiMux12G - Get op. mode
}  DtIoctlSdiMux12GCmdOutput;


#ifdef WINBUILD
    #define DT_IOCTL_SDIMUX12G_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SDIMUX12G_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSdiMux12GCmdInOut
    {
        DtIoctlSdiMux12GCmdInput  m_Input;
        DtIoctlSdiMux12GCmdOutput  m_Output;
    }  DtIoctlSdiMux12GCmdInOut;
    #define DT_IOCTL_SDIMUX12G_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                           \
                                                           DT_FUNC_CODE_SDIMUX12G_CMD,   \
                                                           DtIoctlSdiMux12GCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SDIRX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SDI Receiver Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSdiRxCmd
{
    DT_SDIRX_CMD_GET_MAX_SDIRATE = 0,
    DT_SDIRX_CMD_GET_OPERATIONAL_MODE = 1,
    DT_SDIRX_CMD_GET_RXMODE = 2,
    DT_SDIRX_CMD_GET_SDIRATE = 3,
    DT_SDIRX_CMD_GET_SDI_STATUS = 4,
    DT_SDIRX_CMD_SET_OPERATIONAL_MODE = 5,
    DT_SDIRX_CMD_SET_RXMODE = 6,
    DT_SDIRX_CMD_SET_SDIRATE = 7,
    DT_SDIRX_CMD_GET_SDI_STATUS2 = 8,
}  DtIoctlSdiRxCmd;

// DT SDIRX  RX-mode
#define DT_SDIRX_RXMODE_ASI       0   // ASI
#define DT_SDIRX_RXMODE_SDI       1   // SDI

//-.-.-.-.-.-.-.-.-.-.-.-.- SDIRX Command - Get Maximum SDI-Rate -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxCmdGetMaxSdiRateInput;
ASSERT_SIZE(DtIoctlSdiRxCmdGetMaxSdiRateInput, 16)
typedef struct _DtIoctlSdiRxCmdGetMaxSdiRateOutput
{
    Int  m_MaxSdiRate;              // Maximum SDI-rate
}  DtIoctlSdiRxCmdGetMaxSdiRateOutput;
ASSERT_SIZE(DtIoctlSdiRxCmdGetMaxSdiRateOutput, 4)

//-.-.-.-.-.-.-.-.-.-.- SDIRX Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlSdiRxCmdGetOpModeInput, 16)
typedef struct _DtIoctlSdiRxCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlSdiRxCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlSdiRxCmdGetOpModeOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.- SDIRX Command - Get RX-Mode Command -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxCmdGetRxModeInput;
ASSERT_SIZE(DtIoctlSdiRxCmdGetRxModeInput, 16)
typedef struct _DtIoctlSdiRxCmdGetRxModeOutput
{
    Int  m_RxMode;             // SDI RX-mode
}  DtIoctlSdiRxCmdGetRxModeOutput;
ASSERT_SIZE(DtIoctlSdiRxCmdGetRxModeOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.- SDIRX Command - Get SDI Rate Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxCmdGetSdiRateInput;
ASSERT_SIZE(DtIoctlSdiRxCmdGetSdiRateInput, 16)
typedef struct _DtIoctlSdiRxCmdGetSdiRateOutput
{
    Int  m_SdiRate;             // SDI mode
}  DtIoctlSdiRxCmdGetSdiRateOutput;
ASSERT_SIZE(DtIoctlSdiRxCmdGetSdiRateOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- SDIRX Command - Get SDI status -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxCmdGetSdiStatusInput;
ASSERT_SIZE(DtIoctlSdiRxCmdGetSdiStatusInput, 16)

// Common for Get SDI-Status Output
#define DT_IOCTL_SDIRX_CMD_GET_SDI_STATUS_OUTPUT                                         \
    Int  m_CarrierDetect;       /* ASI/SDI carrier detect                             */ \
    Int  m_SdiLock;             /* SDI-overall lock (TRUE or FALSE)                   */ \
    Int  m_LineLock;            /* SDI-line lock (TRUE or FALSE)                      */ \
    Int  m_Valid;               /* SDI-parameters are valid (TRUE or FALSE)           */ \
    Int  m_NumSymsHanc;         /* Number of HANC symbols per line                    */ \
    Int  m_NumSymsVidVanc;      /* Number of Video/VANC symbols per line              */ \
    Int  m_NumLinesF1;          /* Number of lines in field 1                         */ \
    Int  m_NumLinesF2;          /* Number of lines in field 2                         */ \
    Int  m_IsLevelB;            /* TRUE if received stream HD-SDI format.is Level B   */ \
    UInt  m_PayloadId;          /* Payload ID                                         */ \
    Int  m_FramePeriod;         /* Frame period in nanoseconds                        */
typedef struct _DtIoctlSdiRxCmdGetSdiStatusOutput
{
    DT_IOCTL_SDIRX_CMD_GET_SDI_STATUS_OUTPUT
}  DtIoctlSdiRxCmdGetSdiStatusOutput;
ASSERT_SIZE(DtIoctlSdiRxCmdGetSdiStatusOutput, 44)

//  Get SDI-Status Output extended with SDI-rate
typedef struct _DtIoctlSdiRxCmdGetSdiStatusOutput2
{
    DT_IOCTL_SDIRX_CMD_GET_SDI_STATUS_OUTPUT
    Int  m_SdiRate;             // Current SDI-rate
}  DtIoctlSdiRxCmdGetSdiStatusOutput2;
ASSERT_SIZE(DtIoctlSdiRxCmdGetSdiStatusOutput2, 48)

//-.-.-.-.-.-.-.-.-.-.- SDIRX Command - Set Operational Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiRxCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlSdiRxCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlSdiRxCmdSetOpModeInput, 20)

//-.-.-.-.-.-.-.-.-.-.-.-.- SDIRX Command - Set RX-Mode Command -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSdiRxCmdSetRxModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_RxMode;             // SDI-mode
}  DtIoctlSdiRxCmdSetRxModeInput;
ASSERT_SIZE(DtIoctlSdiRxCmdSetRxModeInput, 20)

//-.-.-.-.-.-.-.-.-.-.-.-.- SDIRX Command - Set SDI-Rate Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiRxCmdSetSdiRateInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SdiRate;             // SDI-rate
}  DtIoctlSdiRxCmdSetSdiRateInput;
ASSERT_SIZE(DtIoctlSdiRxCmdSetSdiRateInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- SDIRX Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-
// SDIRX command - IOCTL input data
typedef union _DtIoctlSdiRxCmdInput
{
    DtIoctlSdiRxCmdSetOpModeInput  m_SetOpMode;              // Set operational mode
    DtIoctlSdiRxCmdSetRxModeInput  m_SetRxMode;              // Set RX mode
    DtIoctlSdiRxCmdSetSdiRateInput  m_SetSdiRate;            // Set SDI rate
}  DtIoctlSdiRxCmdInput;
// SDIRX command - IOCTL output data
typedef union _DtIoctlSdiRxCmdOutput
{
    DtIoctlSdiRxCmdGetMaxSdiRateOutput  m_GetMaxSdiRate;     // Get max SDI-rate
    DtIoctlSdiRxCmdGetOpModeOutput  m_GetOpMode;             // Get operational mode
    DtIoctlSdiRxCmdGetRxModeOutput  m_GetRxMode;             // Get RX-mode
    DtIoctlSdiRxCmdGetSdiRateOutput  m_GetSdiRate;           // Get SDI-rate
    DtIoctlSdiRxCmdGetSdiStatusOutput  m_GetSdiStatus;       // Get SDI-status
    DtIoctlSdiRxCmdGetSdiStatusOutput2  m_GetSdiStatus2;     // Get SDI-status2
}  DtIoctlSdiRxCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_SDIRX_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SDIRX_CMD,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSdiRxCmdInOut
    {
        DtIoctlSdiRxCmdInput  m_Input;
        DtIoctlSdiRxCmdOutput  m_Output;
    }  DtIoctlSdiRxCmdInOut;
    #define DT_IOCTL_SDIRX_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SDIRX_CMD,       \
                                                                   DtIoctlSdiRxCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_CHSDIRX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// .-.-.-.-.-.-.-.-.-.-.-.-.-.- SDI Receiver Channel Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlChSdiRxCmd
{
    DT_CHSDIRX_CMD_ATTACH = 0,
    DT_CHSDIRX_CMD_CONFIGURE = 1,
    DT_CHSDIRX_CMD_DETACH = 2,
    DT_CHSDIRX_CMD_GET_OPERATIONAL_MODE = 3,
    DT_CHSDIRX_CMD_SET_OPERATIONAL_MODE = 4,
    DT_CHSDIRX_CMD_WAIT_FOR_FMT_EVENT = 5,
    DT_CHSDIRX_CMD_GET_WRITE_OFFSET = 6,
    DT_CHSDIRX_CMD_SET_READ_OFFSET = 7,
    DT_CHSDIRX_CMD_GET_PROPS = 8,
    DT_CHSDIRX_CMD_GET_SDI_STATUS = 9,
    DT_CHSDIRX_CMD_MAP_DMA_BUF_TO_USER=10,
} DtIoctlChSdiRxCmd;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CHSDIRX Command - Attach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlChSdiRxCmdAttachInput
{
    DtIoctlInputDataHdr m_CmdHdr;
    Int m_ReqExclusiveAccess;       // True, is we want to attach exclusively
    char m_FriendlyName[DT_CHAN_FRIENDLY_NAME_MAX_LENGTH+1];
                                    // Friendly name used for identifying the user
} DtIoctlChSdiRxCmdAttachInput;
ASSERT_SIZE(DtIoctlChSdiRxCmdAttachInput, 88)

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- CHSDIRX Command - Configure -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlChSdiRxCmdConfigureInput
{
    DtIoctlInputDataHdr  m_CmdHdr;

    Int m_NumPhysicalPorts;         // Number of valid physical port. Valid values: 1 or 4
    Int m_PhysicalPorts[4];         // List physical ports (indexes) used for receiving 
                                    // signal. Typically, this will include just the one 
                                    // port, but for a quad-link setup all four port are 
                                    // used.
                                    // Note: the order of the ports determines the logical
                                    // link order setting for the ST425 link re-orderer
    // DMA buffer configuration
    struct
    {
        Int m_MinSize;              // Minimum size the user wants for the DMA buffer. 
                                    // Actual size may be larger.
    } m_DmaBuf;

    // Format event configuration
    struct
    {
        Int m_IntInterval;          // Interval between format-event interrupts in us
        Int m_IntDelay;             // Delay between SOF and the interrupt in us
        Int m_NumIntsPerFrame;      // Number of format-event interrupts per frame
    } m_FmtEvt;

    // Frame properties
    struct
    {
        Int m_NumSymsHanc;          // Number of HANC symbols per line 
        Int m_NumSymsVidVanc;       // Number of Video/VANC symbols per line  
        Int m_NumLines;             // Number of lines per frame 
        Int m_SdiRate;              // SDI-Rate
        Int m_AssumeInterlaced;     // Assume interlaced (TRUE or FALSE)
        Int m_Scale12GTo3G;         // Scale a 12G-to-3G (TRUE or FALSE)
    } m_FrameProps;

}  DtIoctlChSdiRxCmdConfigureInput;
ASSERT_SIZE(DtIoctlChSdiRxCmdConfigureInput, 76)

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CHSDIRX Command - Detach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlChSdiRxCmdDetachInput;
ASSERT_SIZE(DtIoctlChSdiRxCmdDetachInput, 16)

// -.-.-.-.-.-.-.-.-.-.-.- CHSDIRX Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlChSdiRxCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlChSdiRxCmdGetOpModeInput, 16)
typedef struct _DtIoctlChSdiRxCmdGetOpModeOutput
{
    Int m_OpMode;                   // Operational mode
}  DtIoctlChSdiRxCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlChSdiRxCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.-.-.-.- CHSDIRX Command - Set Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlChSdiRxCmdSetOpModeInput
{
    DtIoctlInputDataHdr m_CmdHdr;
    Int m_OpMode;                   // Operational mode
}  DtIoctlChSdiRxCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlChSdiRxCmdSetOpModeInput, 20)

// -.-.-.-.-.-.-.-.-.-.- CHSDIRX Command - Wait For Formatter Event -.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlChSdiRxCmdWaitForFmtEventInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Timeout;                 // Wait timeout in milliseconds
}  DtIoctlChSdiRxCmdWaitForFmtEventInput;
ASSERT_SIZE(DtIoctlChSdiRxCmdWaitForFmtEventInput, 20)
// Output
typedef struct _DtIoctlChSdiRxCmdWaitForFmtEventOutput
{
    Int m_FrameId;                  // Frame ID (16 least significant bits)
    Int m_SeqNumber;                // Sequence number
    Int m_InSync;                   // In-sync (TRUE or FALSE)
} DtIoctlChSdiRxCmdWaitForFmtEventOutput;
ASSERT_SIZE(DtIoctlChSdiRxCmdWaitForFmtEventOutput, 12)

// -.-.-.-.-.-.-.-.-.-.-.-.- CHSDIRX Command - Get Write Offset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlChSdiRxCmdGetWrOffsetInput;
ASSERT_SIZE(DtIoctlChSdiRxCmdGetWrOffsetInput, 16)
typedef struct _DtIoctlChSdiRxCmdGetRxWrOffsetOutput
{
    UInt32  m_WriteOffset;          //  Write offset
}  DtIoctlChSdiRxCmdGetWrOffsetOutput;
ASSERT_SIZE(DtIoctlChSdiRxCmdGetWrOffsetOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.-.- CHSDIRX Command - Set Read Offset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlChSdiRxCmdSetRdOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt32  m_ReadOffset;           // DMA read offset
}  DtIoctlChSdiRxCmdSetRdOffsetInput;
ASSERT_SIZE(DtIoctlChSdiRxCmdSetRdOffsetInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.-.- CHSDIRX Command - Get Properties -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlChSdiRxCmdGetPropsInput;
ASSERT_SIZE(DtIoctlChSdiRxCmdGetPropsInput, 16)
typedef struct _DtIoctlChSdiRxCmdGetPropsOutput
{
    // DMA specific properties
    struct
    {
        UInt32 m_Caps;              // DMA capability flags
        Int m_PrefetchSize;         // DMA prefetch size
        Int m_PcieDataWidth;        // PCIe interface data width in #bits
        Int m_ReorderBufSize;       // Burst fifo size in #bytes
    } m_Dma;

    Int m_StreamAlignment;          // Stream alignment
}  DtIoctlChSdiRxCmdGetPropsOutput;
ASSERT_SIZE(DtIoctlChSdiRxCmdGetPropsOutput, 20)

// .-.-.-.-.-.-.-.-.-.-.- CHSDIRX Command - Map DMA Buffer to User -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlChSdiRxCmdMapDmaBufToUserInput;
ASSERT_SIZE(DtIoctlChSdiRxCmdMapDmaBufToUserInput, 16)
typedef struct _DtIoctlChSdiRxCmdMapDmaBufToUserOutput
{
    UInt64A m_BufferAddr;           // Mapped DMA buffer address
    Int m_BufSize;                  // Size of the mapped DMA buffer 
    Int m_MaxLoad;                  // Maximum allowed load in the DMA buffer
} DtIoctlChSdiRxCmdMapDmaBufToUserOutput;
ASSERT_SIZE(DtIoctlChSdiRxCmdMapDmaBufToUserOutput, 16);

// .-.-.-.-.-.-.-.-.-.-.-.-.- CHSDIRX Command - Get SDI status -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlChSdiRxCmdGetSdiStatusInput;
ASSERT_SIZE(DtIoctlChSdiRxCmdGetSdiStatusInput, 16)
typedef struct _DtIoctlChSdiRxCmdGetSdiStatusOutput
{
    Int m_CarrierDetect;            // ASI/SDI carrier detect
    Int m_SdiLock;                  // SDI-overall lock
    Int m_LineLock;                 // SDI-line lock
    Int m_Valid;                    // Fields below are valid
    Int m_NumSymsHanc;              // Number of HANC symbols per line
    Int m_NumSymsVidVanc;           // Number of Video/VANC symbols per line
    Int m_NumLinesF1;               // Number of lines in field 1
    Int m_NumLinesF2;               // Number of lines in field 2
    Int m_IsLevelB;                 // TRUE if received stream HD-SDI format.is Level B
    UInt m_PayloadId;               // Payload ID
    Int m_FramePeriod;              // Frame period in nanoseconds
    Int m_SdiRate;                  // Current SDI-rate
}  DtIoctlChSdiRxCmdGetSdiStatusOutput;
ASSERT_SIZE(DtIoctlChSdiRxCmdGetSdiStatusOutput, 48)

// -.-.-.-.-.-.-.-.-.-.-.- SDIRXCXHAN Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.
// SDIRXCXHAN command - IOCTL input data
typedef union _DtIoctlChSdiRxCmdInput
{
    DtIoctlChSdiRxCmdAttachInput m_Attach;
    DtIoctlChSdiRxCmdConfigureInput m_Configure;
    DtIoctlChSdiRxCmdDetachInput m_Detach;
    DtIoctlChSdiRxCmdGetOpModeInput m_GetOpMode;
    DtIoctlChSdiRxCmdSetOpModeInput m_SetOpMode;
    DtIoctlChSdiRxCmdWaitForFmtEventInput m_WaitForFmtEvent;
    DtIoctlChSdiRxCmdGetWrOffsetInput m_GetWriteOffset;
    DtIoctlChSdiRxCmdSetRdOffsetInput m_SetReadOffset;
    DtIoctlChSdiRxCmdGetPropsInput m_GetProps;
    DtIoctlChSdiRxCmdGetSdiStatusInput m_GetSdiStatus;
    DtIoctlChSdiRxCmdMapDmaBufToUserInput m_DmaBufToUser;
}  DtIoctlChSdiRxCmdInput;
// SDIRXCXHAN command - IOCTL output data
typedef union _DtIoctlChSdiRxCmdOutput
{
    DtIoctlChSdiRxCmdGetOpModeOutput m_GetOpMode;
    DtIoctlChSdiRxCmdWaitForFmtEventOutput m_WaitForFmtEvent;
    DtIoctlChSdiRxCmdGetWrOffsetOutput m_GetWriteOffset;
    DtIoctlChSdiRxCmdGetPropsOutput m_GetProps;
    DtIoctlChSdiRxCmdGetSdiStatusOutput m_GetSdiStatus;
    DtIoctlChSdiRxCmdMapDmaBufToUserOutput m_DmaBufToUser;
}  DtIoctlChSdiRxCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_CHSDIRX_CMD  CTL_CODE(DT_DEVICE_TYPE,                               \
                            DT_FUNC_CODE_CHSDIRX_CMD, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlChSdiRxCmdInOut
    {
        DtIoctlChSdiRxCmdInput m_Input;
        DtIoctlChSdiRxCmdOutput m_Output;
    } DtIoctlChSdiRxCmdInOut;
    #define DT_IOCTL_CHSDIRX_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                             \
                                         DT_FUNC_CODE_CHSDIRX_CMD, DtIoctlChSdiRxCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_CHSDIRXPHYONLY_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.- SDI PHY-only Receiver Channel Commands -.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlChSdiRxPhyOnlyCmd
{
    DT_CHSDIRXPHYONLY_CMD_ATTACH = 0,
    DT_CHSDIRXPHYONLY_CMD_CONFIGURE = 1,
    DT_CHSDIRXPHYONLY_CMD_DETACH = 2,
    DT_CHSDIRXPHYONLY_CMD_GET_OPERATIONAL_MODE = 3,
    DT_CHSDIRXPHYONLY_CMD_SET_OPERATIONAL_MODE = 4,
    DT_CHSDIRXPHYONLY_CMD_GET_SDI_STATUS = 5,
} DtIoctlChSdiRxPhyOnlyCmd;

// -.-.-.-.-.-.-.-.-.-.-.-.-.- CHSDIRXPHYONLY Command - Attach -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlChSdiRxPhyOnlyCmdAttachInput
{
    DtIoctlInputDataHdr m_CmdHdr;
    Int m_ReqExclusiveAccess;       // True, is we want to attach exclusively
    char m_FriendlyName[DT_CHAN_FRIENDLY_NAME_MAX_LENGTH+1];
                                    // Friendly name used for identifying the user
} DtIoctlChSdiRxPhyOnlyCmdAttachInput;
ASSERT_SIZE(DtIoctlChSdiRxPhyOnlyCmdAttachInput, 88)

// -.-.-.-.-.-.-.-.-.-.-.-.- CHSDIRXPHYONLY Command - Configure -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlChSdiRxPhyOnlyCmdConfigureInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int m_SdiRate;                  // SDI-Rate

}  DtIoctlChSdiRxPhyOnlyCmdConfigureInput;
ASSERT_SIZE(DtIoctlChSdiRxPhyOnlyCmdConfigureInput, 20)

// -.-.-.-.-.-.-.-.-.-.-.-.-.- CHSDIRXPHYONLY Command - Detach -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlChSdiRxPhyOnlyCmdDetachInput;
ASSERT_SIZE(DtIoctlChSdiRxPhyOnlyCmdDetachInput, 16)

// .-.-.-.-.-.-.-.-.-.- CHSDIRXPHYONLY Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlChSdiRxPhyOnlyCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlChSdiRxPhyOnlyCmdGetOpModeInput, 16)
typedef struct _DtIoctlChSdiRxPhyOnlyCmdGetOpModeOutput
{
    Int m_OpMode;                   // Operational mode
}  DtIoctlChSdiRxPhyOnlyCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlChSdiRxPhyOnlyCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.-.- CHSDIRXPHYONLY Command - Set Operational Mode -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlChSdiRxPhyOnlyCmdSetOpModeInput
{
    DtIoctlInputDataHdr m_CmdHdr;
    Int m_OpMode;                   // Operational mode
}  DtIoctlChSdiRxPhyOnlyCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlChSdiRxPhyOnlyCmdSetOpModeInput, 20)

// -.-.-.-.-.-.-.-.-.-.-.- CHSDIRXPHYONLY Command - Get SDI status -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlChSdiRxPhyOnlyCmdGetSdiStatusInput;
ASSERT_SIZE(DtIoctlChSdiRxPhyOnlyCmdGetSdiStatusInput, 16)
typedef struct _DtIoctlChSdiRxPhyOnlyCmdGetSdiStatusOutput
{
    Int m_CarrierDetect;            // ASI/SDI carrier detect
    Int m_SdiLock;                  // SDI-overall lock
    Int m_LineLock;                 // SDI-line lock
    Int m_Valid;                    // Fields below are valid
    Int m_NumSymsHanc;              // Number of HANC symbols per line
    Int m_NumSymsVidVanc;           // Number of Video/VANC symbols per line
    Int m_NumLinesF1;               // Number of lines in field 1
    Int m_NumLinesF2;               // Number of lines in field 2
    Int m_IsLevelB;                 // TRUE if received stream HD-SDI format.is Level B
    UInt m_PayloadId;               // Payload ID
    Int m_FramePeriod;              // Frame period in nanoseconds
    Int m_SdiRate;                  // Current SDI-rate
}  DtIoctlChSdiRxPhyOnlyCmdGetSdiStatusOutput;
ASSERT_SIZE(DtIoctlChSdiRxPhyOnlyCmdGetSdiStatusOutput, 48)

// -.-.-.-.-.-.-.-.-.-.- CHSDIRXPHYONLY Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.
// SDIRXCXHAN command - IOCTL input data
typedef union _DtIoctlChSdiRxPhyOnlyCmdInput
{
    DtIoctlChSdiRxPhyOnlyCmdAttachInput m_Attach;
    DtIoctlChSdiRxPhyOnlyCmdConfigureInput m_Configure;
    DtIoctlChSdiRxPhyOnlyCmdDetachInput m_Detach;
    DtIoctlChSdiRxPhyOnlyCmdGetOpModeInput m_GetOpMode;
    DtIoctlChSdiRxPhyOnlyCmdSetOpModeInput m_SetOpMode;
    DtIoctlChSdiRxPhyOnlyCmdGetSdiStatusInput m_GetSdiStatus;
}  DtIoctlChSdiRxPhyOnlyCmdInput;
// SDIRXCXHAN command - IOCTL output data
typedef union _DtIoctlChSdiRxPhyOnlyCmdOutput
{
    DtIoctlChSdiRxPhyOnlyCmdGetOpModeOutput m_GetOpMode;
    DtIoctlChSdiRxPhyOnlyCmdGetSdiStatusOutput m_GetSdiStatus;
}  DtIoctlChSdiRxPhyOnlyCmdOutput;

#ifdef WINBUILD
    #define DT_IOCTL_CHSDIRXPHYONLY_CMD  CTL_CODE(DT_DEVICE_TYPE,                        \
                       DT_FUNC_CODE_CHSDIRXPHYONLY_CMD, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlChSdiRxPhyOnlyCmdInOut
    {
        DtIoctlChSdiRxPhyOnlyCmdInput m_Input;
        DtIoctlChSdiRxPhyOnlyCmdOutput m_Output;
    } DtIoctlChSdiRxPhyOnlyCmdInOut;
    #define DT_IOCTL_CHSDIRXPHYONLY_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                      \
                           DT_FUNC_CODE_CHSDIRXPHYONLY_CMD, DtIoctlChSdiRxPhyOnlyCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SDIRXF_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.- SDI Receive Formatter Simple 3G Commands -.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSdiRxFCmd
{
    DT_SDIRXF_CMD_GET_FMT_EVENT_TIMING = 0,
    DT_SDIRXF_CMD_GET_FRAME_PROPERTIES = 1,
    DT_SDIRXF_CMD_GET_MAX_SDIRATE = 2,
    DT_SDIRXF_CMD_GET_OPERATIONAL_MODE = 3,
    DT_SDIRXF_CMD_SET_FMT_EVENT_TIMING = 4,
    DT_SDIRXF_CMD_SET_FRAME_PROPERTIES = 5,
    DT_SDIRXF_CMD_SET_OPERATIONAL_MODE = 6,
    DT_SDIRXF_CMD_WAIT_FOR_FMT_EVENT = 7,
    DT_SDIRXF_CMD_GET_STREAM_ALIGNMENT = 8,
}  _DtIoctlSdiRxFCmd;


//-.-.-.-.-.-.-.-.-.- SDIRXF Command - Get Format Event Timing Command -.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxFCmdGetFmtEventTimingInput;
ASSERT_SIZE(DtIoctlSdiRxFCmdGetFmtEventTimingInput, 16)
typedef struct _DtIoctlSdiRxFCmdGetFmtEventTimingOutput
{
    Int  m_InterruptInterval;       // Interval between format-event interrupts in us
    Int  m_InterruptDelay;          // Delay between SOF and the interrupt in us
    Int  m_InterruptsPerFrame;      // Number of format-event interrupts per frame
} DtIoctlSdiRxFCmdGetFmtEventTimingOutput;
ASSERT_SIZE(DtIoctlSdiRxFCmdGetFmtEventTimingOutput, 12)

//.-.-.-.-.-.-.-.-.-.-.-.- SDIRXF Command - Get Frame Properties -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxFCmdGetFramePropertiesInput;
ASSERT_SIZE(DtIoctlSdiRxFCmdGetFramePropertiesInput, 16)
typedef struct _DtIoctlSdiRxFCmdGetFramePropertiesOutput
{
    Int  m_NumSymsHanc;         // Number of HANC symbols per line 
    Int  m_NumSymsVidVanc;      // Number of Video/VANC symbols per line  
    Int  m_NumLines;            // Number of lines per frame 
    Int  m_SdiRate;             // SDI-Rate
    Int  m_AssumeInterlaced;    // Assume interlaced (TRUE or FALSE)
}  DtIoctlSdiRxFCmdGetFramePropertiesOutput;
ASSERT_SIZE(DtIoctlSdiRxFCmdGetFramePropertiesOutput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.- SDIRXF Command - Get Maximum SDI-Rate  -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxFCmdGetMaxSdiRateInput;
ASSERT_SIZE(DtIoctlSdiRxFCmdGetMaxSdiRateInput, 16)
typedef struct _DtIoctlSdiRxFCmdGetMaxSdiRateOutput
{
    Int  m_MaxSdiRate;         // Maximum SDI-rate
}  DtIoctlSdiRxFCmdGetMaxSdiRateOutput;
ASSERT_SIZE(DtIoctlSdiRxFCmdGetMaxSdiRateOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.- SDIRXF Command - Get Operational Mode  -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxFCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlSdiRxFCmdGetOpModeInput, 16)
typedef struct _DtIoctlSdiRxFCmdGetOpModeOutput
{
    Int  m_OpMode;         // Operational mode
}  DtIoctlSdiRxFCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlSdiRxFCmdGetOpModeOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.- SDIRXF Command - Get Maximum SDI-Rate  -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxFCmdGetStreamAlignmentInput;
ASSERT_SIZE(DtIoctlSdiRxFCmdGetStreamAlignmentInput, 16)
typedef struct _DtIoctlSdiRxFCmdGetStreamAlignmentOutput
{
    Int  m_StreamAlignment;         // Stream alignment
}  DtIoctlSdiRxFCmdGetStreamAlignmentOutput;
ASSERT_SIZE(DtIoctlSdiRxFCmdGetStreamAlignmentOutput, 4)

//-.-.-.-.-.-.-.-.-.- SDIRXF Command - Set Format Event Timing Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiRxFCmdSetFmtEventTimingInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_InterruptInterval;       // Interval between format-event interrupts in us
    Int  m_InterruptDelay;          // Delay between SOF and the interrupt in us
    Int  m_InterruptsPerFrame;      // Number of format-event interrupts per frame
}  DtIoctlSdiRxFCmdSetFmtEventTimingInput;
ASSERT_SIZE(DtIoctlSdiRxFCmdSetFmtEventTimingInput, 28)

//.-.-.-.-.-.-.-.-.-.-.-.- SDIRXF Command - Set Frame Properties -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiRxFCmdSetFramePropertiesInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumSymsHanc;         // Number of HANC symbols per line 
    Int  m_NumSymsVidVanc;      // Number of Video/VANC symbols per line  
    Int  m_NumLines;            // Number of lines per frame 
    Int  m_SdiRate;             // SDI-Rate
    Int  m_AssumeInterlaced;    // Assume interlaced (TRUE or FALSE) 
}  DtIoctlSdiRxFCmdSetFramePropertiesInput;
ASSERT_SIZE(DtIoctlSdiRxFCmdSetFramePropertiesInput, 36)

//.-.-.-.-.-.-.-.-.-.-.-.- SDIRXF Command - Set Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiRxFCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;         // Operational mode
}  DtIoctlSdiRxFCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlSdiRxFCmdSetOpModeInput, 20)

//-.-.-.-.-.-.-.-.-.-.- SDIRXF Command - Wait For Formatter Event   -.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSdiRxFCmdWaitForFmtEventInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Timeout;             // Wait timeout in milliseconds
}  DtIoctlSdiRxFCmdWaitForFmtEventInput;
ASSERT_SIZE(DtIoctlSdiRxFCmdWaitForFmtEventInput, 20)
// Output
typedef struct _DtIoctlSdiRxFCmdWaitForFmtEventOutput
{
    Int  m_FrameId;                 // Frame ID (16 least significant bits)
    Int  m_SeqNumber;               // Sequence number
    Int  m_InSync;                  // In-sync (TRUE or FALSE)
} DtIoctlSdiRxFCmdWaitForFmtEventOutput;
ASSERT_SIZE(DtIoctlSdiRxFCmdWaitForFmtEventOutput, 12)


//.-.-.-.-.-.-.-.-.-.-.-.-.- SDIRXF Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// SDIRXF command - IOCTL input data
typedef union _DtIoctlSdiRxFCmdInput
{
    DtIoctlSdiRxFCmdSetFmtEventTimingInput  m_SetFmtEventTiming; // Set fmt event timing
    DtIoctlSdiRxFCmdSetFramePropertiesInput  m_SetFrameProps; // Set frame properties
    DtIoctlSdiRxFCmdSetOpModeInput  m_SetOpMode;              // Set operational mode
    DtIoctlSdiRxFCmdWaitForFmtEventInput  m_WaitForFmtEvent;  // Wait for fmt int event
}  DtIoctlSdiRxFCmdInput;
// SDIRXF command - IOCTL output data
typedef union _DtIoctlSdiRxFCmdOutput
{
    DtIoctlSdiRxFCmdGetFmtEventTimingOutput m_GetFmtEventTiming; // Get fmt event timing
    DtIoctlSdiRxFCmdGetFramePropertiesOutput  m_GetFrameProps;   // Get frame properties
    DtIoctlSdiRxFCmdGetMaxSdiRateOutput  m_GetMaxSdiRate;        // Get max SDI-rate
    DtIoctlSdiRxFCmdGetOpModeOutput  m_GetOpMode;                // Get operational mode
    DtIoctlSdiRxFCmdGetStreamAlignmentOutput  m_GetAlignment;    // Get alignment
    DtIoctlSdiRxFCmdWaitForFmtEventOutput  m_WaitForFmtEvent;    // Wait for fmt event
}  DtIoctlSdiRxFCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_SDIRXF_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SDIRXF_CMD,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSdiRxFCmdInOut
    {
        DtIoctlSdiRxFCmdInput  m_Input;
        DtIoctlSdiRxFCmdOutput  m_Output;
    }  DtIoctlSdiRxFCmdInOut;
    #define DT_IOCTL_SDIRXF_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SDIRXF_CMD,     \
                                                                  DtIoctlSdiRxFCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SDIRXP_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.- SDI Receiver Protocol 3G Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSdiRxPCmd
{
    DT_SDIRXP_CMD_GET_CRC_ERROR_CNT = 0,
    DT_SDIRXP_CMD_GET_MAX_SDIRATE = 1,
    DT_SDIRXP_CMD_GET_OPERATIONAL_MODE = 2,
    DT_SDIRXP_CMD_GET_SDIRATE = 3,
    DT_SDIRXP_CMD_GET_SDI_STATUS = 4,
    DT_SDIRXP_CMD_SET_OPERATIONAL_MODE = 5,
    DT_SDIRXP_CMD_SET_SDIRATE = 6,
    DT_SDIRXP_CMD_GET_SDI_STATUS2 = 7,
}  DtIoctlSdiRxPCmd;

//-.-.-.-.-.-.-.-.-.-.-.-.- SDIRXP Command - Get CRC-Error Count -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxPCmdGetCrcErrorCountInput;
ASSERT_SIZE(DtIoctlSdiRxPCmdGetCrcErrorCountInput, 16)
typedef struct _DtIoctlSdiRxPCmdGetCrcErrorCountOutput
{
    Int  m_CrcErrorCount;       // CRC-error count
}  DtIoctlSdiRxPCmdGetCrcErrorCountOutput;
ASSERT_SIZE(DtIoctlSdiRxPCmdGetCrcErrorCountOutput, 4)

//.-.-.-.-.-.-.-.-.-.- SDIRXP Command - Get Maximum SDI-Rate Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxPCmdGetMaxSdiRateInput;
ASSERT_SIZE(DtIoctlSdiRxPCmdGetMaxSdiRateInput, 16)
typedef struct _DtIoctlSdiRxPCmdGetMaxSdiRateOutput
{
    Int  m_MaxSdiRate;             // Maximum SDI-rate
}  DtIoctlSdiRxPCmdGetMaxSdiRateOutput;
ASSERT_SIZE(DtIoctlSdiRxPCmdGetMaxSdiRateOutput, 4)

//.-.-.-.-.-.-.-.-.-.- SDIRXP Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxPCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlSdiRxPCmdGetOpModeInput, 16)
typedef struct _DtIoctlSdiRxPCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlSdiRxPCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlSdiRxPCmdGetOpModeOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.- SDIRXP Command - Get SDI-Rate Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxPCmdGetSdiRateInput;
ASSERT_SIZE(DtIoctlSdiRxPCmdGetSdiRateInput, 16)
typedef struct _DtIoctlSdiRxPCmdGetSdiRateOutput
{
    Int  m_SdiRate;             // SDI mode
}  DtIoctlSdiRxPCmdGetSdiRateOutput;
ASSERT_SIZE(DtIoctlSdiRxPCmdGetSdiRateOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.- SDIRXP Command - Get SDI status -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiRxPCmdGetSdiStatusInput;
ASSERT_SIZE(DtIoctlSdiRxPCmdGetSdiStatusInput, 16)

// Common for Get SDI-Status Output
#define DT_IOCTL_SDIRXP_CMD_GET_SDI_STATUS_OUTPUT                                        \
    Int  m_SdiLock;             /* SDI-overall lock                                   */ \
    Int  m_LineLock;            /* SDI-line lock                                      */ \
    Int  m_Valid;               /* Fields below are valid                             */ \
    Int  m_NumSymsHanc;         /* Number of HANC symbols per line                    */ \
    Int  m_NumSymsVidVanc;      /* Number of Video/VANC symbols per line              */ \
    Int  m_NumLinesF1;          /* Number of lines in field 1                         */ \
    Int  m_NumLinesF2;          /* Number of lines in field 2                         */ \
    Int  m_IsLevelB;            /* TRUE if received stream HD-SDI format.is Level B   */ \
    UInt  m_PayloadId;          /* Payload ID                                         */ \
    Int  m_FramePeriod;         /* Frame period in nanoseconds                        */

typedef struct _DtIoctlSdiRxPCmdGetSdiStatusOutput
{
    DT_IOCTL_SDIRXP_CMD_GET_SDI_STATUS_OUTPUT
}  DtIoctlSdiRxPCmdGetSdiStatusOutput;
ASSERT_SIZE(DtIoctlSdiRxPCmdGetSdiStatusOutput, 40)

// Get SDI-Status Output extended with SDI-rate
typedef struct _DtIoctlSdiRxPCmdGetSdiStatusOutput2
{
    DT_IOCTL_SDIRXP_CMD_GET_SDI_STATUS_OUTPUT
    Int  m_SdiRate;             // Current SDI-rate
}  DtIoctlSdiRxPCmdGetSdiStatusOutput2;
ASSERT_SIZE(DtIoctlSdiRxPCmdGetSdiStatusOutput2, 44)

//.-.-.-.-.-.-.-.-.-.- SDIRXP Command - Set Operational Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiRxPCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlSdiRxPCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlSdiRxPCmdSetOpModeInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.- SDIRXP Command - Set SDI-Rate Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiRxPCmdSetSdiRateInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SdiRate;             // SDI-rate
}  DtIoctlSdiRxPCmdSetSdiRateInput;
ASSERT_SIZE(DtIoctlSdiRxPCmdSetSdiRateInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.- SDIRXP Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// SDIRXP command - IOCTL input data
typedef union _DtIoctlSdiRxPCmdInput
{
    DtIoctlSdiRxPCmdSetOpModeInput  m_SetOpMode;              // Set operational mode
    DtIoctlSdiRxPCmdSetSdiRateInput  m_SetSdiRate;            // Set SDI-rate
}  DtIoctlSdiRxPCmdInput;
// SDIRXP command - IOCTL output data
typedef union _DtIoctlSdiRxPCmdOutput
{
    DtIoctlSdiRxPCmdGetCrcErrorCountOutput  m_GetCrcErrorCnt; // Get CRC-error count
    DtIoctlSdiRxPCmdGetMaxSdiRateOutput  m_GetMaxSdiRate;     // Get max SDI-rate
    DtIoctlSdiRxPCmdGetOpModeOutput  m_GetOpMode;             // Get operational mode
    DtIoctlSdiRxPCmdGetSdiRateOutput  m_GetSdiRate;           // Get SDI-rate
    DtIoctlSdiRxPCmdGetSdiStatusOutput  m_GetSdiStatus;       // Get SDI-status
    DtIoctlSdiRxPCmdGetSdiStatusOutput2   m_GetSdiStatus2;    // Get SDI-status
}  DtIoctlSdiRxPCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_SDIRXP_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SDIRXP_CMD,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSdiRxPCmdInOut
    {
        DtIoctlSdiRxPCmdInput  m_Input;
        DtIoctlSdiRxPCmdOutput  m_Output;
    }  DtIoctlSdiRxPCmdInOut;
    #define DT_IOCTL_SDIRXP_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SDIRXP_CMD,     \
                                                                  DtIoctlSdiRxPCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SDITXF_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.- SDI Transmitter Formatter Simple  Commands -.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSdiTxFCmd
{
    DT_SDITXF_CMD_GET_FMT_EVENT_SETTING = 0,
    DT_SDITXF_CMD_GET_OPERATIONAL_MODE = 1,
    DT_SDITXF_CMD_SET_FMT_EVENT_SETTING = 2,
    DT_SDITXF_CMD_SET_OPERATIONAL_MODE = 3,
    DT_SDITXF_CMD_WAIT_FOR_FMT_EVENT = 4,
    DT_SDITXF_CMD_GET_MAX_SDIRATE = 5,
    DT_SDITXF_CMD_GET_STREAM_ALIGNMENT = 6,
}  _DtIoctlSdiTxFCmd;


//.-.-.-.-.-.-.-.-.- SDITXF Command - Get Format Event Setting Command -.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxFCmdGetFmtEventSettingInput;
ASSERT_SIZE(DtIoctlSdiTxFCmdGetFmtEventSettingInput, 16)
typedef struct _DtIoctlSdiTxFCmdGetFmtEventSettingOutput
{
    Int  m_NumLinesPerEvent;    // Num lines between format events
    Int  m_NumSofsBetweenTod;   // Number of frames between SoF timestamp
} DtIoctlSdiTxFCmdGetFmtEventSettingOutput;
ASSERT_SIZE(DtIoctlSdiTxFCmdGetFmtEventSettingOutput, 8)

// .-.-.-.-.-.-.-.-.-.-.-.- SDITXF Command - Get Maximum SDI-Rate -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxFCmdGetMaxSdiRateInput;
ASSERT_SIZE(DtIoctlSdiTxFCmdGetMaxSdiRateInput, 16)
typedef struct _DtIoctlSdiTxFCmdGetMaxSdiRateOutput
{
    Int  m_MaxSdiRate;              // Maximum SDI-rate
}  DtIoctlSdiTxFCmdGetMaxSdiRateOutput;
ASSERT_SIZE(DtIoctlSdiTxFCmdGetMaxSdiRateOutput, 4)


//.-.-.-.-.-.-.-.-.-.-.-.- SDITXF Command - Get Operational Mode  -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxFCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlSdiTxFCmdGetOpModeInput, 16)
typedef struct _DtIoctlSdiTxFCmdGetOpModeOutput
{
    Int  m_OpMode;         // Operational mode
}  DtIoctlSdiTxFCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlSdiTxFCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.- SDITXF Command - Get Stream Alignment -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxFCmdGetStreamAlignmentInput;
ASSERT_SIZE(DtIoctlSdiTxFCmdGetStreamAlignmentInput, 16)
typedef struct _DtIoctlSdiTxFCmdGetStreamAlignmentOutput
{
    Int  m_StreamAlignment;              // Stream alignment
}  DtIoctlSdiTxFCmdGetStreamAlignmentOutput;
ASSERT_SIZE(DtIoctlSdiTxFCmdGetStreamAlignmentOutput, 4)

//.-.-.-.-.-.-.-.-.- SDITXF Command - Set Format Event Setting Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiTxFCmdSetFmtEventSettingInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumLinesPerEvent;    // Num lines between format events
    Int  m_NumSofsBetweenTod;   // Number of frames between SoF timestamp
}  DtIoctlSdiTxFCmdSetFmtEventSettingInput;
ASSERT_SIZE(DtIoctlSdiTxFCmdSetFmtEventSettingInput, 24)


//.-.-.-.-.-.-.-.-.-.-.-.- SDITXF Command - Set Operational Mode -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiTxFCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;         // Operational mode
}  DtIoctlSdiTxFCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlSdiTxFCmdSetOpModeInput, 20)

//-.-.-.-.-.-.-.-.-.-.- SDITXF Command - Wait For Formatter Event   -.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSdiTxFCmdWaitForFmtEventInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Timeout;             // Wait timeout in milliseconds
}  DtIoctlSdiTxFCmdWaitForFmtEventInput;
ASSERT_SIZE(DtIoctlSdiTxFCmdWaitForFmtEventInput, 20)
// Output
typedef struct _DtIoctlSdiTxFCmdWaitForFmtEventOutput
{
    Int  m_FrameId;                 // Frame ID (16 least significant bits)
    Int  m_SeqNumber;               // Sequence number
    Int  m_Underflow;               // Underflow detected (TRUE or FALSE)
    Int  m_SofTimeValid;            // SoF-timestamp is valid (TRUE or FALSE)
    DtTodTime  m_SofTime;           // SoF-timestamp
} DtIoctlSdiTxFCmdWaitForFmtEventOutput;
ASSERT_SIZE(DtIoctlSdiTxFCmdWaitForFmtEventOutput, 24)


//.-.-.-.-.-.-.-.-.-.-.-.-.- SDITXF Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// SDITXF command - IOCTL input data
typedef union _DtIoctlSdiTxFCmdInput
{
    DtIoctlSdiTxFCmdSetFmtEventSettingInput  m_SetFmtEventSetting; // Set event setting
    DtIoctlSdiTxFCmdSetOpModeInput  m_SetOpMode;              // Set operational mode
    DtIoctlSdiTxFCmdWaitForFmtEventInput  m_WaitForFmtEvent;  // Wait for fmt int event
}  DtIoctlSdiTxFCmdInput;
// SDITXF command - IOCTL output data
typedef union _DtIoctlSdiTxFCmdOutput
{
    DtIoctlSdiTxFCmdGetFmtEventSettingOutput m_GetFmtEventSetting; // Get event setting
    DtIoctlSdiTxFCmdGetMaxSdiRateOutput  m_GetMaxSdiRate;          // Get max SDI rate
    DtIoctlSdiTxFCmdGetOpModeOutput  m_GetOpMode;                  // Get operational mode
    DtIoctlSdiTxFCmdGetStreamAlignmentOutput  m_GetAlignment;      // Get alignment
    DtIoctlSdiTxFCmdWaitForFmtEventOutput  m_WaitForFmtEvent;      // Wait for fmt event
}  DtIoctlSdiTxFCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_SDITXF_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SDITXF_CMD,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSdiTxFCmdInOut
    {
        DtIoctlSdiTxFCmdInput  m_Input;
        DtIoctlSdiTxFCmdOutput  m_Output;
    }  DtIoctlSdiTxFCmdInOut;
    #define DT_IOCTL_SDITXF_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SDITXF_CMD,     \
                                                                    DtIoctlSdiTxFCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SDITXP_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- SDI Receiver Protocol Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSdiTxPCmd
{
    DT_SDITXP_CMD_GET_CLAMPED_SYMS_COUNT = 0,
    DT_SDITXP_CMD_GET_GENERATION_MODE = 1,
    DT_SDITXP_CMD_GET_MAX_SDIRATE = 2,
    DT_SDITXP_CMD_GET_OPERATIONAL_MODE = 3,
    DT_SDITXP_CMD_GET_SDIRATE_AND_LVLA2BENA = 4,
    DT_SDITXP_CMD_SET_GENERATION_MODE = 5,
    DT_SDITXP_CMD_SET_OPERATIONAL_MODE = 6,
    DT_SDITXP_CMD_SET_SDIRATE_AND_LVLA2BENA = 7,
}  DtIoctlSdiTxPCmd;

//.-.-.-.-.-.-.-.-.-.-.- SDITXP Command - Get Clamped Symbols Count -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPCmdGetClampedSymsCountInput;
ASSERT_SIZE(DtIoctlSdiTxPCmdGetClampedSymsCountInput, 16)
typedef struct _DtIoctlSdiTxPCmdGetClampedSymsCountOutput
{
    UInt  m_ClampedSymsCount;       // Clamped symbols count
}  DtIoctlSdiTxPCmdGetClampedSymsCountOutput;
ASSERT_SIZE(DtIoctlSdiTxPCmdGetClampedSymsCountOutput, 4)

//-.-.-.-.-.-.-.-.-.-.- SDITXP Command - Get Generation Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPCmdGetGenModeInput;
ASSERT_SIZE(DtIoctlSdiTxPCmdGetGenModeInput, 16)
typedef struct _DtIoctlSdiTxPCmdGetGenModeOutput
{
    Int  m_ClampEnable;        // Clamp enabled
    Int  m_AdpChecksumEnable;  // ADP checksum inserstion enabled
    Int  m_LineCrcEnable;      // Line CRC insertion enabled
}  DtIoctlSdiTxPCmdGetGenModeOutput;
ASSERT_SIZE(DtIoctlSdiTxPCmdGetGenModeOutput, 12)

//.-.-.-.-.-.-.-.-.-.-.- SDITXP Command - Get Clamped Symbols Count -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPCmdGetMaxSdiRateInput;
ASSERT_SIZE(DtIoctlSdiTxPCmdGetMaxSdiRateInput, 16)
typedef struct _DtIoctlSdiTxPCmdGetMaxSdiRateOutput
{
    UInt  m_MaxSdiRate;         // Maximum SDI-rate
}  DtIoctlSdiTxPCmdGetMaxSdiRateOutput;
ASSERT_SIZE(DtIoctlSdiTxPCmdGetMaxSdiRateOutput, 4)

//.-.-.-.-.-.-.-.-.-.- SDITXP Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlSdiTxPCmdGetOpModeInput, 16)
typedef struct _DtIoctlSdiTxPCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlSdiTxPCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlSdiTxPCmdGetOpModeOutput, 4)

//.-.-.-.-.- SDITXP Command - Get SDI Rate and Level A to B Conversion Command -.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPCmdGetSdiRateAndLvlA2BEnaInput;
ASSERT_SIZE(DtIoctlSdiTxPCmdGetSdiRateAndLvlA2BEnaInput, 16)
typedef struct _DtIoctlSdiTxPCmdGetSdiRateAndLvlA2BEnaOutput
{
    Int  m_SdiRate;             // SDI rate
    Int  m_LevelA2BEnable;      // Level A to B converter enabled
}  DtIoctlSdiTxPCmdGetSdiRateAndLvlA2BEnaOutput;
ASSERT_SIZE(DtIoctlSdiTxPCmdGetSdiRateAndLvlA2BEnaOutput, 8)

//-.-.-.-.-.-.-.-.-.-.- SDITXP Command - Set Generation Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiTxPCmdSetGenModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_ClampEnable;        // Clamp enabled
    Int  m_AdpChecksumEnable;  // ADP checksum inserstion enabled
    Int  m_LineCrcEnable;      // Line CRC insertion enabled
}  DtIoctlSdiTxPCmdSetGenModeInput;
ASSERT_SIZE(DtIoctlSdiTxPCmdSetGenModeInput, 28)

//.-.-.-.-.-.-.-.-.-.- SDITXP Command - Set Operational Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiTxPCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlSdiTxPCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlSdiTxPCmdSetOpModeInput, 20)


//.-.-.-.-.- SDITXP Command - Set SDI Rate and Level A to B Conversion Command -.-.-.-.-.-
//
typedef struct _DtIoctlSdiTxPCmdSetSdiRateAndLvlA2BEnaInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SdiRate;             // SDI mode
    Int  m_LevelA2BEnable;      // Level A to B converter enabled
}  DtIoctlSdiTxPCmdSetSdiRateAndLvlA2BEnaInput;
ASSERT_SIZE(DtIoctlSdiTxPCmdSetSdiRateAndLvlA2BEnaInput, 24)

//.-.-.-.-.-.-.-.-.-.-.-.-.- SDITXP Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// SDITXP command - IOCTL input data
typedef union _DtIoctlSdiTxPCmdInput
{
    DtIoctlSdiTxPCmdSetGenModeInput  m_SetGenMode;    // Set generation mode
    DtIoctlSdiTxPCmdSetOpModeInput  m_SetOpMode;      // Set operational mode
    DtIoctlSdiTxPCmdSetSdiRateAndLvlA2BEnaInput  m_SetSdiRateAndLvlA2BEna; 
                                                   // Set SDI rate and Level A 2 B enable
}  DtIoctlSdiTxPCmdInput;
// SDITXP command - IOCTL output data
typedef union _DtIoctlSdiTxPCmdOutput
{
    DtIoctlSdiTxPCmdGetClampedSymsCountOutput  m_GetClampedCnt;// Get clamped syms count
    DtIoctlSdiTxPCmdGetGenModeOutput  m_GetGenMode;           // Get generation mode
    DtIoctlSdiTxPCmdGetMaxSdiRateOutput  m_GetMaxSdiRate;     // Get max SDI-rate
    DtIoctlSdiTxPCmdGetOpModeOutput  m_GetOpMode;             // Get operational mode
    DtIoctlSdiTxPCmdGetSdiRateAndLvlA2BEnaOutput  m_GetSdiRateAndLvlA2BEna; 
                                                   // Get SDI rate and Level A 2 B enable
}  DtIoctlSdiTxPCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_SDITXP_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SDITXP_CMD,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSdiTxPCmdInOut
    {
        DtIoctlSdiTxPCmdInput  m_Input;
        DtIoctlSdiTxPCmdOutput  m_Output;
    }  DtIoctlSdiTxPCmdInOut;
    #define DT_IOCTL_SDITXP_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SDITXP_CMD,     \
                                                                    DtIoctlSdiTxPCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SDITXPHY_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SDI Transmitter Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSdiTxPhyCmd
{
    DT_SDITXPHY_CMD_GET_OPERATIONAL_MODE = 0,
    DT_SDITXPHY_CMD_GET_SDIRATE = 1,
    DT_SDITXPHY_CMD_GET_SDI_STATUS = 2,
    DT_SDITXPHY_CMD_GET_TXMODE = 3,
    DT_SDITXPHY_CMD_SET_OPERATIONAL_MODE = 4,
    DT_SDITXPHY_CMD_SET_SDIRATE = 5,
    DT_SDITXPHY_CMD_SET_TXMODE = 6,
    DT_SDITXPHY_CMD_CLEAR_UNDERFLOW_FLAG = 7,
    DT_SDITXPHY_CMD_GET_UNDERFLOW_FLAG = 8,
    DT_SDITXPHY_CMD_GET_GENLOCK_STATUS = 9,
    DT_SDITXPHY_CMD_SET_OPERATIONAL_MODE_TIMED = 10,
    DT_SDITXPHY_CMD_GET_MAX_SDIRATE = 11,
    DT_SDITXPHY_CMD_GET_START_OF_FRAME_OFFSET = 12,
    DT_SDITXPHY_CMD_SET_START_OF_FRAME_OFFSET = 13,

}  DtIoctlSdiTxPhyCmd;

// DT SDITXPHY  TX-mode
#define DT_SDITXPHY_TXMODE_ASI       0   // ASI
#define DT_SDITXPHY_TXMODE_SDI       1   // SDI

// DT SDITXPHY  GenLock status
#define DT_SDITXPHY_GENLOCK_LOCKED  0   // PHY is genlocked
#define DT_SDITXPHY_GENLOCK_NO_LOCK 1   // PHY is not genlocked

// -.-.-.-.-.-.-.-.-.-.-.- SDITXPHY Command - Clear Undeflow Flag -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPhyCmdClearUnderflowFlagInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdClearUnderflowFlagInput, 16)


// .-.-.-.-.-.-.-.-.-.- SDITXPHY Command - Get GenLock Status Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPhyCmdGetGenLockStatusInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetGenLockStatusInput, 16)
typedef struct _DtIoctlSdiTxPhyCmdGetGenLockStatusOutput
{
    Int  m_GenLockStatus;              // GenLock status
}  DtIoctlSdiTxPhyCmdGetGenLockStatusOutput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetGenLockStatusOutput, 4)

// -.-.-.-.-.-.-.-.-.- SDITXPHY Command - Get Maximum SDI Rate Command -.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPhyCmdGetMaxSdiRateInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetMaxSdiRateInput, 16)
typedef struct _DtIoctlSdiTxPhyCmdGetMaxSdiRateOutput
{
    Int  m_MaxSdiRate;          // SDI rate
}  DtIoctlSdiTxPhyCmdGetMaxSdiRateOutput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetMaxSdiRateOutput, 4)

//-.-.-.-.-.-.-.-.-.- SDITXPHY Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPhyCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetOpModeInput, 16)
typedef struct _DtIoctlSdiTxPhyCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlSdiTxPhyCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetOpModeOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.- SDITXPHY Command - Get SDI Rate Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPhyCmdGetSdiRateInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetSdiRateInput, 16)
typedef struct _DtIoctlSdiTxPhyCmdGetSdiRateOutput
{
    Int  m_SdiRate;             // SDI rate
    Int  m_Fractional;          // Fractional clock (TRUE or FALSE)
}  DtIoctlSdiTxPhyCmdGetSdiRateOutput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetSdiRateOutput, 8)

//.-.-.-.-.-.-.-.-.-.-.-.-.- SDITXPHY Command - Get SDI status -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPhyCmdGetSdiStatusInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetSdiStatusInput, 16)
typedef struct _DtIoctlSdiTxPhyCmdGetSdiStatusOutput
{
    Int  m_TxLock;             // SDI-overall lock
}  DtIoctlSdiTxPhyCmdGetSdiStatusOutput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetSdiStatusOutput, 4)

// .-.-.-.-.-.-.-.- SDITXPHY Command - Get Start-Of-Frame Offset Command -.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPhyCmdGetStartOfFrameOffsetInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetStartOfFrameOffsetInput, 16)
typedef struct _DtIoctlSdiTxPhyCmdGetStartOfFrameOffsetOutput
{
    Int  m_StartOfFrameOffsetNs;    // Start-of-frame offset in ns
}  DtIoctlSdiTxPhyCmdGetStartOfFrameOffsetOutput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetStartOfFrameOffsetOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.- SDITXPHY Command - Get TX-Mode Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPhyCmdGetTxModeInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetTxModeInput, 16)
typedef struct _DtIoctlSdiTxPhyCmdGetTxModeOutput
{
    Int  m_TxMode;             // SDI mode
}  DtIoctlSdiTxPhyCmdGetTxModeOutput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetTxModeOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.- SDITXPHY Command - Get Underflow Flag -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSdiTxPhyCmdGetUnderflowFlagInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetUnderflowFlagInput, 16)
typedef struct _DtIoctlSdiTxPhyCmdGetUnderflowFlagOutput
{
    Int  m_UflFlag;         // Underflow flag
}  DtIoctlSdiTxPhyCmdGetUnderflowFlagOutput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdGetUnderflowFlagOutput, 4)

//-.-.-.-.-.-.-.-.-.- SDITXPHY Command - Set Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSdiTxPhyCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlSdiTxPhyCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.- SDITXPHY Command - Set Operational Mode Timed Command -.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSdiTxPhyCmdSetOpModeTimedInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
    DtTodTime  m_StartTime;     // Start time
}  DtIoctlSdiTxPhyCmdSetOpModeTimedInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdSetOpModeTimedInput, 28)

//-.-.-.-.-.-.-.-.-.-.-.- SDITXPHY Command - Set SDI Rate Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSdiTxPhyCmdSetSdiRateInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SdiRate;             // SDI rate
    Int  m_Fractional;          // Fractional clock (TRUE or FALSE)
}  DtIoctlSdiTxPhyCmdSetSdiRateInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdSetSdiRateInput, 24)

// .-.-.-.-.-.-.-.- SDITXPHY Command - Set Start-Of-Frame Offset Command -.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSdiTxPhyCmdSetStartOfFrameOffsetInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_StartOfFrameOffsetNs;    // Start-of-frame offset in ns
}  DtIoctlSdiTxPhyCmdSetStartOfFrameOffsetInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdSetStartOfFrameOffsetInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.- SDITXPHY Command - Set TX-Mode Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSdiTxPhyCmdSetTxModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_TxMode;             // TX mode
}  DtIoctlSdiTxPhyCmdSetTxModeInput;
ASSERT_SIZE(DtIoctlSdiTxPhyCmdSetTxModeInput, 20)


//-.-.-.-.-.-.-.-.-.-.-.-.- SDITXPHY Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// SDITXPHY command - IOCTL input data
typedef union _DtIoctlSdiTxPhyCmdInput
{
    DtIoctlSdiTxPhyCmdSetOpModeInput  m_SetOpMode;              // Set operational mode
    DtIoctlSdiTxPhyCmdSetOpModeTimedInput  m_SetOpModeTimed;    // Set op mode timed
    DtIoctlSdiTxPhyCmdSetSdiRateInput  m_SetSdiRate;            // Set SDI rate
    DtIoctlSdiTxPhyCmdSetStartOfFrameOffsetInput  m_SetSofOffset; // Set SOF-offset
    DtIoctlSdiTxPhyCmdSetTxModeInput  m_SetTxMode;              // Set TX mode
}  DtIoctlSdiTxPhyCmdInput;
// SDITXPHY command - IOCTL output data
typedef union _DtIoctlSdiTxPhyCmdOutput
{
    DtIoctlSdiTxPhyCmdGetGenLockStatusOutput  m_GetGenLockStatus;  // Get GenLock status
    DtIoctlSdiTxPhyCmdGetMaxSdiRateOutput  m_GetMaxSdiRate;     // Get Max SDI rate
    DtIoctlSdiTxPhyCmdGetOpModeOutput  m_GetOpMode;             // Get operational mode
    DtIoctlSdiTxPhyCmdGetSdiRateOutput  m_GetSdiRate;           // Get SDI rate
    DtIoctlSdiTxPhyCmdGetSdiStatusOutput  m_GetSdiStatus;       // Get SDI-status
    DtIoctlSdiTxPhyCmdGetStartOfFrameOffsetOutput  m_GetSofOffset; // Get SOF-offset
    DtIoctlSdiTxPhyCmdGetTxModeOutput  m_GetTxMode;             // Get TX mode
    DtIoctlSdiTxPhyCmdGetUnderflowFlagOutput  m_GetUflFlag;     // Get underflow flag
}  DtIoctlSdiTxPhyCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_SDITXPHY_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SDITXPHY_CMD,   \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSdiTxPhyCmdInOut
    {
        DtIoctlSdiTxPhyCmdInput  m_Input;
        DtIoctlSdiTxPhyCmdOutput  m_Output;
    }  DtIoctlSdiTxPhyCmdInOut;
    #define DT_IOCTL_SDITXPHY_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SDITXPHY_CMD, \
                                                                DtIoctlSdiTxPhyCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SDIXCFG_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.- SDI Transceiver Reconfig for Cyclone V Commands -.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSdiXCfgCmd
{
    DT_SDIXCFG_CMD_GET_CONFIG = 0,
    DT_SDIXCFG_CMD_SET_MODE = 1,
}  DtIoctlSdiXCfgCmd;

// Structure for specifying the relelation between logical channel number and port index
typedef  struct _DtSdiXCfgChanConfig
{
    Int  m_LogChanNum;          // Logical channel number
    Int  m_ChanType;            // Channel type
    Int  m_PortIndex;           // Related port index
} DtSdiXCfgChanConfig;


// Reconfig channel type
#define DT_SDIXCFG_CHANTYPE_NOT_USED  0 // Not used
#define DT_SDIXCFG_CHANTYPE_RX_CHAN   1 // RX-channel
#define DT_SDIXCFG_CHANTYPE_TX_CHAN   2 // TX-channel
#define DT_SDIXCFG_CHANTYPE_TX_PLL    3 // TX-PLL

// DT SDIXCFG SDI RATE
#define DT_SDIXCFG_MODE_3G           0    // 3G SDI (also for ASI and SD)
#define DT_SDIXCFG_MODE_1G5          1    // 1.5G for HD

// DT SDIXCFG PLL CLOCK
#define DT_SDIXCFG_MODE_NON_FRAC    2    // Non-fractional clock
#define DT_SDIXCFG_MODE_FRAC        3    // Fractional clock

//-.-.-.-.-.-.-.-.-.-.-.-.- SDIXCFG Command - GetConfig Command -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSdiXCfgCmdGetConfigInput 
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_MaxNumToGet;
} DtIoctlSdiXCfgCmdGetConfigInput;
ASSERT_SIZE(DtIoctlSdiXCfgCmdGetConfigInput, 20)

typedef struct _DtIoctlSdiXCfgCmdGetConfigOutput {
    Int  m_NumGot;
    DtSdiXCfgChanConfig  m_Buf[0];  // Dynamic sized buffer
} DtIoctlSdiXCfgCmdGetConfigOutput;
ASSERT_SIZE(DtIoctlSdiXCfgCmdGetConfigOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.-.- SDIXCFG Command - Set Mode Command -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSdiXCfgCmdSetModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_LogChanNum;         // Logical channel number
    Int  m_Mode;               // Mode ASI/SD/HD/3G
}  DtIoctlSdiXCfgCmdSetModeInput;
ASSERT_SIZE(DtIoctlSdiXCfgCmdSetModeInput, 24)


//-.-.-.-.-.-.-.-.-.-.-.-.- SDIXCFG Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// SDIXCFG command - IOCTL input data
typedef union _DtIoctlSdiXCfgCmdInput
{
    DtIoctlSdiXCfgCmdGetConfigInput  m_GetConfig;   // Get config
    DtIoctlSdiXCfgCmdSetModeInput  m_SetMode;       // Set mode
}  DtIoctlSdiXCfgCmdInput;

// SDIXCFG command - IOCTL output data
typedef union _DtIoctlSdiXCfgCmdOutput
{
    DtIoctlSdiXCfgCmdGetConfigOutput  m_GetConfig;  // Get config
}  DtIoctlSdiXCfgCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_SDIXCFG_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SDIXCFG_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSdiXCfgCmdInOut
    {
        DtIoctlSdiXCfgCmdInput  m_Input;
        DtIoctlSdiXCfgCmdOutput  m_Output;
    }  DtIoctlSdiXCfgCmdInOut;
    #define DT_IOCTL_SDIXCFG_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SDIXCFG_CMD,   \
                                                                   DtIoctlSdiXCfgCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SENSTEMP_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SensTEMP commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSensTempCmd
{
    DT_SENSTEMP_CMD_GET_PROPERTIES      = 0,  // Get temperature sensor properties
    DT_SENSTEMP_CMD_GET_TEMPERATURE     = 1,  // Get temperature
}  DtIoctlSensTempCmd;


//.-.-.-.-.-.-.-.-.-.-.-.-.- SENSTEMP - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSensTempCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlSensTempCmdGetPropertiesInput, 16)

typedef struct _DtIoctlSensTempCmdGetPropertiesOutput 
{
    Int  m_MaximumTemperature;      // Maximum temperature
    Int  m_TargetTemperature;       // Target temperature
    char m_TempSensorName[PROPERTY_STR_MAX_SIZE]; // Temperature name
} DtIoctlSensTempCmdGetPropertiesOutput;
ASSERT_SIZE(DtIoctlSensTempCmdGetPropertiesOutput, 104)

//.-.-.-.-.-.-.-.-.-.-.-.-.- SENSTEMP - Get Temperature Command -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSensTempCmdGetTemperatureInput;
ASSERT_SIZE(DtIoctlSensTempCmdGetTemperatureInput, 16)

typedef struct _DtIoctlSensTempCmdGetTemperatureOutput 
{
    Int  m_Temperature;         // Current temperature
} DtIoctlSensTempCmdGetTemperatureOutput;
ASSERT_SIZE(DtIoctlSensTempCmdGetTemperatureOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.- SENSTEMP command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// TEMP command - Input data
typedef union _DtIoctlSensTempCmdInput
{
    DtIoctlSensTempCmdGetPropertiesInput  m_GetProperties;
    DtIoctlSensTempCmdGetTemperatureInput  m_GetTemperature;
} DtIoctlSensTempCmdInput;
ASSERT_SIZE(DtIoctlSensTempCmdInput, 16)

// TEMP command - Output data
typedef union _DtIoctlSensTempCmdOutput
{
    DtIoctlSensTempCmdGetPropertiesOutput  m_GetProperties;
    DtIoctlSensTempCmdGetTemperatureOutput  m_GetTemperature;
}  DtIoctlSensTempCmdOutput;
ASSERT_SIZE(DtIoctlSensTempCmdOutput, 104)

#ifdef WINBUILD
    #define DT_IOCTL_SENSTEMP_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SENSTEMP_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSensTempCmdInOut
    {
        DtIoctlSensTempCmdInput  m_Input;
        DtIoctlSensTempCmdOutput  m_Output;
    } DtIoctlSensTempCmdInOut;

    #define DT_IOCTL_SENSTEMP_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SENSTEMP_CMD, \
                                                                  DtIoctlSensTempCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SI2166ITF_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SI2166ITF commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSi2166ItfCmd
{
    DT_SI2166ITF_CMD_GET_OPERATIONAL_MODE = 0,  // Get operational mode
    DT_SI2166ITF_CMD_SET_OPERATIONAL_MODE = 1,  // Set operational mode
    DT_SI2166ITF_CMD_GET_CONFIGURATION = 2,     // Get current configuration
    DT_SI2166ITF_CMD_SET_CONFIGURATION = 3,     // Set new configuration
}  DtIoctlSi2166ItfCmd;

// Si2166 Mode
#define DT_SI2166ITF_SI2166MODE_TS   0  // Transport Stream
#define DT_SI2166ITF_SI2166MODE_GSE  1  // GSE-HEM/GP/GC

// .-.-.-.-.-.-.-.-.- SI2166ITF Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSi2166ItfCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlSi2166ItfCmdGetOpModeInput, 16)
typedef struct _DtIoctlSi2166ItfCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlSi2166ItfCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlSi2166ItfCmdGetOpModeOutput, 4)

// .-.-.-.-.-.-.-.-.- SI2166ITF Command - Set Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSi2166ItfCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode: IDLE/RUN
} DtIoctlSi2166ItfCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlSi2166ItfCmdSetOpModeInput, 20)

// .-.-.-.-.-.-.-.- SI2166ITF Command - Get Current Configuration Command -.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSi2166ItfCmdGetConfigInput;
ASSERT_SIZE(DtIoctlSi2166ItfCmdGetConfigInput, 16)
typedef struct _DtIoctlSi2166ItfCmdGetConfigOutput
{
    Int  m_Si2166Mode;          // Si2166 Mode: TS or GSE
    Int  m_DataType;            // Packet stream data type
}  DtIoctlSi2166ItfCmdGetConfigOutput;
ASSERT_SIZE(DtIoctlSi2166ItfCmdGetConfigOutput, 8)

// .-.-.-.-.-.-.-.-.- SI2166ITF Command - Set New Configuration Command -.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSi2166ItfCmdSetConfigInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Si2166Mode;          // Si2166 Mode: TS or GSE
    Int  m_DataType;            // Packet stream data type
} DtIoctlSi2166ItfCmdSetConfigInput;
ASSERT_SIZE(DtIoctlSi2166ItfCmdSetConfigInput, 24)

// .-.-.-.-.-.-.-.-.-.-.-.- SI2166ITF command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.
// SI2166ITF command - Input data
typedef union _DtIoctlSi2166ItfCmdInput
{
    DtIoctlSi2166ItfCmdGetOpModeInput  m_GetOpMode;
    DtIoctlSi2166ItfCmdGetConfigInput  m_GetConfig;
    DtIoctlSi2166ItfCmdSetOpModeInput  m_SetOpMode;
    DtIoctlSi2166ItfCmdSetConfigInput  m_SetConfig;
} DtIoctlSi2166ItfCmdInput;
ASSERT_SIZE(DtIoctlSi2166ItfCmdInput, 24)

// SI2166ITF command - Output data
typedef union _DtIoctlSi2166ItfCmdOutput
{
    DtIoctlSi2166ItfCmdGetOpModeOutput  m_GetOpMode;
    DtIoctlSi2166ItfCmdGetConfigOutput  m_GetConfig;
}  DtIoctlSi2166ItfCmdOutput;
ASSERT_SIZE(DtIoctlSi2166ItfCmdOutput, 8)

#ifdef WINBUILD
    #define DT_IOCTL_SI2166ITF_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SI2166ITF_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSi2166ItfCmdInOut {
        DtIoctlSi2166ItfCmdInput  m_Input;
        DtIoctlSi2166ItfCmdOutput  m_Output;
    } DtIoctlSi2166ItfCmdInOut;

    #define DT_IOCTL_SI2166ITF_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                           \
                                                            DT_FUNC_CODE_SI2166ITF_CMD,  \
                                                                 DtIoctlSi2166ItfCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SPIM_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIM commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSpiMCmd
{
    DT_SPIM_CMD_GET_PROPERTIES      = 0,  // Get properties
    DT_SPIM_CMD_READ                = 1,  // Read data
    DT_SPIM_CMD_WRITE               = 2,  // Write data
    DT_SPIM_CMD_WRITE_READ          = 3,  // Write/read data
}  DtIoctlSpiMCmd;

// SPIM device IDs
#define DT_SPIM_SPIDVC_UNDEFINED (-1) // Undefined
#define DT_SPIM_SPIDVC_25AA160C  0  // Microchip 25AA160C 16K SPI bus serial EEPROM
#define DT_SPIM_SPIDVC_AD9642    1  // Analog Devices AD9642 14-bit 170/210/250-MSPS
                                    // Analog to Digital Converter
#define DT_SPIM_SPIDVC_ADS4246   2  // Texas Instruments ADS4246 dual-channel 14-bit
                                    // 160-MSPS ADC
#define DT_SPIM_SPIDVC_GS1661    3  // Gennum GS1661 HD-SDI/ASI receiver
#define DT_SPIM_SPIDVC_LMH0394   4  // Texas Instruments LMH0394 3G-SDI/ASI receiver
#define DT_SPIM_SPIDVC_GS3590    5  // Gennum GS3590 3G-SDI/ASI Cable Driver/Equalizer
#define DT_SPIM_SPIDVC_25AA640A  6  // Microchip 25AA640 64K SPI bus serial EEPROM
#define DT_SPIM_SPIDVC_GS12090   7  // Gennum GS12090 12G-SDI/ASI Cable Driver/Equalizer
#define DT_SPIM_SPIDVC_ADC342X   8  // TI Quad-Channel 12-Bit, 25..125-MSPS ADC
#define DT_SPIM_SPIDVC_RFFC5072  9  // Qorvo RFFC5072 wideband synthesizer/VCO
#define DT_SPIM_SPIDVC_AD9163    10 // Analog Devices AD9163 16-bit 12-GSPS RF DAC
#define DT_SPIM_SPIDVC_LTC6952   11 // Analog Devices LTC6952 ultralow jitter
#define DT_SPIM_SPIDVC_ADS8866   12 // Texas Instruments ADS8866 16-bit 100-kSPS ADC
#define DT_SPIM_SPIDVC_AD9628    13 // Analog Devices AD9628 12-bit 105-MSPS Dual ADC
#define DT_SPIM_SPIDVC_AD9266    14 // Analog Devices AD9266 16-bit 80-MSPS ADC
#define DT_SPIM_SPIDVC_SpiRegIfMaster  15  // SPI-controlled register-interface master
#define DT_SPIM_SPIDVC_TC72      16  // Microchip TC72 Digital Temperature Sensor
#define DT_SPIM_SPIDVC_AD5611    17  // Analog Devices AD5611 10-bit NanoDac
#define DT_SPIM_SPIDVC_ADF4360   18  // Analog Devices ADF4360 Integrated
                                     // Synthesizer and VCO
#define DT_SPIM_SPIDVC_AD9789    19  // Analog Devices AD9789 2400 MSPS RF DAC
#define DT_SPIM_SPIDVC_ADS5562   20  // Texas Instruments ADS5562 16-bit 80 MSPS ADCs
#define DT_SPIM_SPIDVC_DAC121S101 21 // Texas Instruments DAC121S101 12-bit RRO DAC

// SPIM transfermode (replaces duplex mode) 
#define DT_SPIM_TFM_FULL_DUPLEX  0x0    // Send and receive simultaneously
#define DT_SPIM_TFM_HALF_DUPLEX  0x1    // Send or receive
#define DT_SPIM_TFM_SIMPLEX_TX   0x2    // Send only

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIM - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSpiMCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlSpiMCmdGetPropertiesInput, 16)

typedef struct _DtIoctlSpiMCmdGetPropertiesOutput 
{
    Int  m_SpiDeviceId;             // Device ID
    Int  m_SpiClockRate;            // SPI clock rate in
    Int  m_WordSize;                // SPI word size in number of bits
    Int  m_TransferMode;            // Transfer mode: full or full duplex
    Int  m_MaxTransferTime;         // Maximum transfer time in ns 

} DtIoctlSpiMCmdGetPropertiesOutput;
ASSERT_SIZE(DtIoctlSpiMCmdGetPropertiesOutput, 20)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIM - Read Command  -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSpiMCmdReadInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumToRead;           // Number of bytes to read
}  DtIoctlSpiMCmdReadInput;
ASSERT_SIZE(DtIoctlSpiMCmdReadInput, 20)

typedef struct _DtIoctlSpiMCmdReadOutput 
{
    Int  m_NumBytes;            // Number of bytes in buffer
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlSpiMCmdReadOutput;
ASSERT_SIZE(DtIoctlSpiMCmdReadOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIM - Write Command  -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSpiMCmdWriteInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumToWrite;          // Number of bytes in buffer to write
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlSpiMCmdWriteInput;
ASSERT_SIZE(DtIoctlSpiMCmdWriteInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIM - Write Read Command  -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSpiMCmdWriteReadInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumToRead;           // Number of bytes to read
    Int  m_NumToWrite;          // Number of bytes in buffer to write
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlSpiMCmdWriteReadInput;
ASSERT_SIZE(DtIoctlSpiMCmdWriteReadInput, 24)
typedef struct _DtIoctlSpiMCmdWriteReadOutput 
{
    Int  m_NumBytes;            // Number of bytes in buffer
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlSpiMCmdWriteReadOutput;
ASSERT_SIZE(DtIoctlSpiMCmdWriteReadOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIM command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-
// SPIM command - Input data
typedef union _DtIoctlSpiMCmdInput
{
    DtIoctlSpiMCmdGetPropertiesInput  m_GetProperties;
    DtIoctlSpiMCmdReadInput  m_Read;
    DtIoctlSpiMCmdWriteInput  m_Write;
    DtIoctlSpiMCmdWriteReadInput  m_WriteRead;
} DtIoctlSpiMCmdInput;
ASSERT_SIZE(DtIoctlSpiMCmdInput, 24)

// SPIM command - Output data
typedef union _DtIoctlSpiMCmdOutput
{
    DtIoctlSpiMCmdGetPropertiesOutput  m_GetProperties;
    DtIoctlSpiMCmdReadOutput  m_Read;
    DtIoctlSpiMCmdWriteReadOutput  m_WriteRead;
}  DtIoctlSpiMCmdOutput;
ASSERT_SIZE(DtIoctlSpiMCmdOutput, 20)

#ifdef WINBUILD
    #define DT_IOCTL_SPIM_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SPIM_CMD,           \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSpiMCmdInOut {
        DtIoctlSpiMCmdInput  m_Input;
        DtIoctlSpiMCmdOutput  m_Output;
    } DtIoctlSpiMCmdInOut;

    #define DT_IOCTL_SPIM_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SPIM_CMD,         \
                                                                      DtIoctlSpiMCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SPIMF_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIMF commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSpiMfCmd
{
    DT_SPIMF_CMD_LOCK_DEVICE         = 0,  // (Un)lock device for write/erase ops
    DT_SPIMF_CMD_GET_PROPERTIES      = 1,  // Get device properties
    DT_SPIMF_CMD_ERASE               = 2,  // Erase
    DT_SPIMF_CMD_READ                = 3,  // Read data
    DT_SPIMF_CMD_WRITE               = 4,  // Write data
}  DtIoctlSpiMfCmd;

// SPIMF extended commands
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSpiMfCmdEx
{
    DT_SPIMF_CMDEX_LOCK = 0,
    DT_SPIMF_CMDEX_UNLOCK = 1,
}  DtIoctlSpiMfCmdEx;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIMF - Lock Device Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef DtIoctlInputDataHdr DtIoctlSpiMfCmdLockDeviceInput;
ASSERT_SIZE(DtIoctlSpiMfCmdLockDeviceInput, 16)
// NO OUPUT DATA

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIMF - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSpiMfCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlSpiMfCmdGetPropertiesInput, 16)

typedef struct _DtIoctlSpiMfCmdGetPropertiesOutput 
{
    Int  m_SpiClockRate;  // SPI Clock Rate in units of Hz
    Int  m_MemoryId;      // Identifier for SPI Flash/PROM device.
    Int  m_PageSize;      // Device page size. 
    Int  m_SectorSize;    // Device sector size (1 = device hase no sectors).
    Int  m_MemorySize;    // Device memory size

} DtIoctlSpiMfCmdGetPropertiesOutput;
ASSERT_SIZE(DtIoctlSpiMfCmdGetPropertiesOutput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIMF - Erase Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSpiMfCmdEraseInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_StartAddress;        // Start address
    Int  m_NumBytes;            // Number of bytes to be erased
}  DtIoctlSpiMfCmdEraseInput;
ASSERT_SIZE(DtIoctlSpiMfCmdEraseInput, 24)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIMF - Read Command  -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSpiMfCmdReadInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_StartAddress;        // Start address
    Int  m_NumToRead;           // Number of bytes to read
}  DtIoctlSpiMfCmdReadInput;
ASSERT_SIZE(DtIoctlSpiMfCmdReadInput, 24)

typedef struct _DtIoctlSpiMfCmdReadOutput 
{
    Int  m_NumBytes;            // Number of bytes in buffer
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlSpiMfCmdReadOutput;
ASSERT_SIZE(DtIoctlSpiMfCmdReadOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIMF - Write Command  -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSpiMfCmdWriteInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_StartAddress;        // Start address
    Int  m_NumToWrite;          // Number of bytes in buffer to write
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlSpiMfCmdWriteInput;
ASSERT_SIZE(DtIoctlSpiMfCmdWriteInput, 24)

//.-.-.-.-.-.-.-.-.-.-.-.-.- SPIMF command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-
// SPIMF command - Input data
typedef union _DtIoctlSpiMfCmdInput
{
    DtIoctlSpiMfCmdGetPropertiesInput  m_GetProperties;
    DtIoctlSpiMfCmdLockDeviceInput  m_Lock;
    DtIoctlSpiMfCmdEraseInput   m_Erase;
    DtIoctlSpiMfCmdReadInput  m_Read;
    DtIoctlSpiMfCmdWriteInput  m_Write;
} DtIoctlSpiMfCmdInput;
ASSERT_SIZE(DtIoctlSpiMfCmdInput, 24)

// SPIMF command - Output data
typedef union _DtIoctlSpiMfCmdOutput
{
    DtIoctlSpiMfCmdGetPropertiesOutput  m_GetProperties;
    DtIoctlSpiMfCmdReadOutput  m_Read;
}  DtIoctlSpiMfCmdOutput;
ASSERT_SIZE(DtIoctlSpiMfCmdOutput, 20)

#ifdef WINBUILD
    #define DT_IOCTL_SPIMF_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SPIMF_CMD,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSpiMfCmdInOut
    {
        DtIoctlSpiMfCmdInput  m_Input;
        DtIoctlSpiMfCmdOutput  m_Output;
    } DtIoctlSpiMfCmdInOut;

    #define DT_IOCTL_SPIMF_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SPIMF_CMD,       \
                                                                     DtIoctlSpiMfCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SPIPROM_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIPROM commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSpiPromCmd
{
    DT_SPIPROM_CMD_LOCK_DEVICE         = 0,  // (Un)lock device for write/erase ops
    DT_SPIPROM_CMD_GET_PROPERTIES      = 1,  // Get device properties
    DT_SPIPROM_CMD_READ                = 2,  // Read data
    DT_SPIPROM_CMD_WRITE               = 3,  // Write data
}  DtIoctlSpiPromCmd;

// SPIPROM extended commands
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSpiPromCmdEx
{
    DT_SPIPROM_CMDEX_LOCK = 0,
    DT_SPIPROM_CMDEX_UNLOCK = 1,
}  DtIoctlSpiPromCmdEx;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIPROM - Lock Device Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef DtIoctlInputDataHdr DtIoctlSpiPromCmdLockDeviceInput;
ASSERT_SIZE(DtIoctlSpiPromCmdLockDeviceInput, 16)
// NO OUPUT DATA

//-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIPROM - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSpiPromCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlSpiPromCmdGetPropertiesInput, 16)

typedef struct _DtIoctlSpiPromCmdGetPropertiesOutput 
{
    Int  m_PageSize;      // Device page size. 
    Int  m_MemorySize;    // Device memory size
} DtIoctlSpiPromCmdGetPropertiesOutput;
ASSERT_SIZE(DtIoctlSpiPromCmdGetPropertiesOutput, 8)


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIPROM - Read Command  -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSpiPromCmdReadInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_StartAddress;        // Start address
    Int  m_NumToRead;           // Number of bytes to read
}  DtIoctlSpiPromCmdReadInput;
ASSERT_SIZE(DtIoctlSpiPromCmdReadInput, 24)

typedef struct _DtIoctlSpiPromCmdReadOutput 
{
    Int  m_NumBytes;            // Number of bytes in buffer
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlSpiPromCmdReadOutput;
ASSERT_SIZE(DtIoctlSpiPromCmdReadOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIPROM - Write Command  -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSpiPromCmdWriteInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_StartAddress;        // Start address
    Int  m_NumToWrite;          // Number of bytes in buffer to write
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlSpiPromCmdWriteInput;
ASSERT_SIZE(DtIoctlSpiPromCmdWriteInput, 24)

//-.-.-.-.-.-.-.-.-.-.-.-.- SPIPROM command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// SPIPROM command - Input data
typedef union _DtIoctlSpiPromCmdInput
{
    DtIoctlSpiPromCmdGetPropertiesInput  m_GetProperties;
    DtIoctlSpiPromCmdLockDeviceInput  m_Lock;
    DtIoctlSpiPromCmdReadInput  m_Read;
    DtIoctlSpiPromCmdWriteInput  m_Write;
} DtIoctlSpiPromCmdInput;
ASSERT_SIZE(DtIoctlSpiPromCmdInput, 24)

// SPIPROM command - Output data
typedef union _DtIoctlSpiPromCmdOutput
{
    DtIoctlSpiPromCmdGetPropertiesOutput  m_GetProperties;
    DtIoctlSpiPromCmdReadOutput  m_Read;
}  DtIoctlSpiPromCmdOutput;
ASSERT_SIZE(DtIoctlSpiPromCmdOutput, 8)

#ifdef WINBUILD
    #define DT_IOCTL_SPIPROM_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SPIPROM_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSpiPromCmdInOut
    {
        DtIoctlSpiPromCmdInput  m_Input;
        DtIoctlSpiPromCmdOutput  m_Output;
    } DtIoctlSpiPromCmdInOut;

    #define DT_IOCTL_SPIPROM_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SPIPROM_CMD,   \
                                                                   DtIoctlSpiPromCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_STUB_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- STUB commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlStubCmd
{
    DT_STUB_CMD_GET_VERSION = 0,
    DT_STUB_CMD_GET_IOCTLS = 1,
}  DtIoctlStubCmd;

//.-.-.-.-.-.-.-.-.-.-.-.-.- STUB Command - Get Version Command -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr  DtIoctlStubCmdGetVersionInput;
ASSERT_SIZE(DtIoctlStubCmdGetVersionInput, 16)

typedef struct _DtIoctlStubCmdGetVersionOutput {
    Int  m_Major;
    Int  m_Minor;
}  DtIoctlStubCmdGetVersionOutput;
ASSERT_SIZE(DtIoctlStubCmdGetVersionOutput, 8)

//.-.-.-.-.-.-.-.-.-.-.-.-.- STUB Command - Get IOCTLs Command -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlStubCmdGetIoctlsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_MaxNumIoctls;        // Maximum number of IOCTLs to get. -1=get num of IOCTLs
}  DtIoctlStubCmdGetIoctlsInput;
ASSERT_SIZE(DtIoctlStubCmdGetIoctlsInput, 20)

typedef struct _DtIoctlStubCmdGetIoctlsOutput {
    Int  m_NumIoctls;           // Number of IOCTL returned
    UInt32  m_IoctlCodes[0];    // Dynamically sized list with IOCTLs the stub supports.
                                // NOTE1: must have size indicated by m_MaxNumIoctls
                                // NOTE2: if m_MaxNumIoctls is '-1', only m_NumIoctls is 
                                // retuned
}  DtIoctlStubCmdGetIoctlsOutput;
ASSERT_SIZE(DtIoctlStubCmdGetIoctlsOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.- STUB command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-
// STUB command - Input data
typedef union _DtIoctlStubCmdInput
{
    DtIoctlStubCmdGetVersionInput  m_GetVersion;
    DtIoctlStubCmdGetIoctlsInput  m_GetIoctls;
}  DtIoctlStubCmdInput;
ASSERT_SIZE(DtIoctlStubCmdInput, 20)

// STUB command - Output data
typedef union _DtIoctlStubCmdOutput
{
    DtIoctlStubCmdGetVersionOutput  m_GetVersion;
    DtIoctlStubCmdGetIoctlsOutput  m_GetIoctls;
}  DtIoctlStubCmdOutput;
ASSERT_SIZE(DtIoctlStubCmdOutput, 8)

#ifdef WINBUILD
    #define DT_IOCTL_STUB_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_STUB_CMD,           \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlStubCmdInOut {
        DtIoctlStubCmdInput  m_Input;
        DtIoctlStubCmdOutput  m_Output;
    } DtIoctlStubCmdInOut;

    #define DT_IOCTL_STUB_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SPIMF_CMD,        \
                                                                      DtIoctlStubCmdInOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_ST425LR_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- St425Lr Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSt425LrCmd
{
    DT_ST425LR_CMD_GET_LINK_ORDER = 0,
    DT_ST425LR_CMD_GET_OPERATIONAL_MODE = 1,
    DT_ST425LR_CMD_SET_LINK_ORDER = 2,
    DT_ST425LR_CMD_SET_OPERATIONAL_MODE = 3,
}  DtIoctlSt425LrCmd;

// -.-.-.-.-.-.-.-.-.-.-.- ST425LR Command - Get LinkOrder Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSt425LrCmdGetLinkOrderInput;
ASSERT_SIZE(DtIoctlSt425LrCmdGetLinkOrderInput, 16)
typedef struct _DtIoctlSt425LrCmdGetLinkOrderOutput
{
    UInt8  m_pLinkIn[4];          // St425Lr link order; index=link; value=SDI-input
}  DtIoctlSt425LrCmdGetLinkOrderOutput;
ASSERT_SIZE(DtIoctlSt425LrCmdGetLinkOrderOutput, 4)

// -.-.-.-.-.-.-.-.-.- ST425LR Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSt425LrCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlSt425LrCmdGetOpModeInput, 16)
typedef struct _DtIoctlSt425LrCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlSt425LrCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlSt425LrCmdGetOpModeOutput, 4)

// -.-.-.-.-.-.-.-.-.-.-.- ST425LR Command - Set LinkOrder Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSt425LrCmdSetLinkOrderInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt8  m_pLinkIn[4];          // St425Lr link order; index=link; value=SDI-input
}  DtIoctlSt425LrCmdSetLinkOrderInput;
ASSERT_SIZE(DtIoctlSt425LrCmdSetLinkOrderInput, 20)

// -.-.-.-.-.-.-.-.-.- ST425LR Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSt425LrCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlSt425LrCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlSt425LrCmdSetOpModeInput, 20)

// -.-.-.-.-.-.-.-.-.-.-.-.- ST425LR Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// ST425LR command - IOCTL input data
typedef union _DtIoctlSt425LrCmdInput
{
    DtIoctlSt425LrCmdSetLinkOrderInput  m_SetLinkOrder;  // St425Lr - Set link order
    DtIoctlSt425LrCmdSetOpModeInput  m_SetOpMode;        // St425Lr - Set op. mode
}  DtIoctlSt425LrCmdInput;

// ST425LR command - IOCTL output data
typedef union _DtIoctlSt425LrCmdOutput
{
    DtIoctlSt425LrCmdGetLinkOrderOutput  m_GetLinkOrder;  // St425Lr - Get link order
    DtIoctlSt425LrCmdGetOpModeOutput  m_GetOpMode;        // St425Lr - Get op. mode
}  DtIoctlSt425LrCmdOutput;


#ifdef WINBUILD
    #define DT_IOCTL_ST425LR_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_ST425LR_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSt425LrCmdInOut
    {
        DtIoctlSt425LrCmdInput  m_Input;
        DtIoctlSt425LrCmdOutput  m_Output;
    }  DtIoctlSt425LrCmdInOut;
    #define DT_IOCTL_ST425LR_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_ST425LR_CMD,   \
                                                                   DtIoctlSt425LrCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_SWITCH_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Switch Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlSwitchCmd
{
    DT_SWITCH_CMD_GET_CONFIGURATION = 0,
    DT_SWITCH_CMD_GET_POSITION = 1,
    DT_SWITCH_CMD_SET_POSITION = 2,
    DT_SWITCH_CMD_CLEAR = 3,
    DT_SWITCH_CMD_GET_OPERATIONAL_MODE = 4,
    DT_SWITCH_CMD_SET_OPERATIONAL_MODE = 5,
}  DtIoctlSwitchCmd;


//.-.-.-.-.-.-.-.-.-.-.- SWITCH Command - Get Configuration Command -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlSwitchCmdGetConfigurationInput;
ASSERT_SIZE(DtIoctlSwitchCmdGetConfigurationInput, 16)
typedef struct _DtIoctlSwitchCmdGetConfigurationOutput
{
    Int  m_NumInputPorts;         // Number of input ports
    Int  m_NumOutputPorts;        // Number of output ports
}  DtIoctlSwitchCmdGetConfigurationOutput;
ASSERT_SIZE(DtIoctlSwitchCmdGetConfigurationOutput, 8)
//.-.-.-.-.-.-.-.-.-.-.-.- SWITCH Command - Get Position Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSwitchCmdGetPositionInput;
ASSERT_SIZE(DtIoctlSwitchCmdGetPositionInput, 16)
typedef struct _DtIoctlSwitchCmdGetPositionOutput
{
    Int  m_InputIndex;          // Switch position; index of selected input port
    Int  m_OutputIndex;         // Switch position; index of selected output port
}  DtIoctlSwitchCmdGetPositionOutput;
ASSERT_SIZE(DtIoctlSwitchCmdGetPositionOutput, 8)

//.-.-.-.-.-.-.-.-.-.-.-.- SWITCH Command - Set Position Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlSwitchCmdSetPositionInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_InputIndex;          // Switch position; index of selected input port
    Int  m_OutputIndex;         // Switch position; index of selected output port
}  DtIoctlSwitchCmdSetPositionInput;
ASSERT_SIZE(DtIoctlSwitchCmdSetPositionInput, 24)

//.-.-.-.-.-.-.-.-.-.-.-.- SWITCH Command - Clear Switch Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSwitchCmdClearInput;
ASSERT_SIZE(DtIoctlSwitchCmdClearInput, 16)

//.-.-.-.-.-.-.-.-.-.- SWITCH Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSwitchCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlSwitchCmdGetOpModeInput, 16)
typedef struct _DtIoctlSwitchCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlSwitchCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlSwitchCmdGetOpModeOutput, 4)


//.-.-.-.-.-.-.-.-.-.- SWITCH Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlSwitchCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlSwitchCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlSwitchCmdSetOpModeInput, 20)


//.-.-.-.-.-.-.-.-.-.-.-.-.- SWITCH Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// SWITCH command - IOCTL input data
typedef union _DtIoctlSwitchCmdInput
{
    DtIoctlSwitchCmdSetPositionInput  m_SetPosition;    // Switch - Set position
    DtIoctlSwitchCmdSetOpModeInput  m_SetOpMode;        // Switch - Set op. mode
}  DtIoctlSwitchCmdInput;




// SWITCH command - IOCTL output data
typedef union _DtIoctlSwitchCmdOutput
{
    DtIoctlSwitchCmdGetConfigurationOutput  m_GetConfiguration;  
                                                        // Switch - Get configuration
    DtIoctlSwitchCmdGetPositionOutput  m_GetPosition;   // Switch - Get position
    DtIoctlSwitchCmdGetOpModeOutput  m_GetOpMode;       // Switch - Get op. mode
}  DtIoctlSwitchCmdOutput;


#ifdef WINBUILD
    #define DT_IOCTL_SWITCH_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_SWITCH_CMD,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlSwitchCmdInOut
    {
        DtIoctlSwitchCmdInput  m_Input;
        DtIoctlSwitchCmdOutput  m_Output;
    }  DtIoctlSwitchCmdInOut;
    #define DT_IOCTL_SWITCH_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SWITCH_CMD,     \
                                                                    DtIoctlSwitchCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_TEMPFANMGR_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TEMP commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlTempFanMgrCmd
{
    DT_TEMPFANMGR_CMD_GET_FAN_STATUS      = 0,  // Get fan status
    DT_TEMPFANMGR_CMD_GET_NUM_FANS        = 1,  // Get number of fans
    DT_TEMPFANMGR_CMD_GET_NUM_TEMPSENS    = 2,  // Get number of temperature sensors
    DT_TEMPFANMGR_CMD_GET_TEMP_PROPS      = 3,  // Get temperature sensor properties
    DT_TEMPFANMGR_CMD_GET_TEMPERATURE     = 4,  // Get temperature
}  DtIoctlTempFanMgrCmd;

//-.-.-.-.-.-.-.-.-.-.-.-.- TEMPFANMGR - Get Fan Status Command -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlTempFanMgrCmdGetFanStatusInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_FanIndex;                // Fan index
}  DtIoctlTempFanMgrCmdGetFanStatusInput;
ASSERT_SIZE(DtIoctlTempFanMgrCmdGetFanStatusInput, 20)
typedef struct _DtIoctlTempFanMgrCmdGetFanStatusOutput 
{
    Int  m_FanSpeedPct;             // Fan speed in percentage
    Int  m_FanSpeedRpm;             // Fan speed in rpm
    Int  m_IsWatchdogTimeout;       // Watchdog timeout (TRUE or FALSE)
    Int  m_IsFanStuck;              // Fan stuck (TRUE or FALSE)
} DtIoctlTempFanMgrCmdGetFanStatusOutput;
ASSERT_SIZE(DtIoctlTempFanMgrCmdGetFanStatusOutput, 16)

//-.-.-.-.-.-.-.-.-.-.-.- TEMPFANMGR - Get Number of Fans Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlTempFanMgrCmdGetNumFansInput;
ASSERT_SIZE(DtIoctlTempFanMgrCmdGetNumFansInput, 16)
typedef struct _DtIoctlTempFanMgrCmdGetNumFansOutput
{
    Int  m_NumFans;         // Number of fans
}  DtIoctlTempFanMgrCmdGetNumFansOutput;
ASSERT_SIZE(DtIoctlTempFanMgrCmdGetNumFansOutput, 4)

//.-.-.-.-.-.-.-.- TEMPFANMGR - Get Number of Temperature Sensors Command -.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlTempFanMgrCmdGetNumTempSensorsInput;
ASSERT_SIZE(DtIoctlTempFanMgrCmdGetNumTempSensorsInput, 16)
typedef struct _DtIoctlTempFanMgrCmdGetNumTempSensorsOutput
{
    Int  m_NumTempSens;         // Number of temperature sensors
}  DtIoctlTempFanMgrCmdGetNumTempSensorsOutput;
ASSERT_SIZE(DtIoctlTempFanMgrCmdGetNumTempSensorsOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.- TEMPFANMGR - Get Temp Properties Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlTempFanMgrCmdGetTempPropertiesInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_TempSensIndex;           // Temperature sensor index
}  DtIoctlTempFanMgrCmdGetTempPropertiesInput;
ASSERT_SIZE(DtIoctlTempFanMgrCmdGetTempPropertiesInput, 20)
typedef struct _DtIoctlTempFanMgrCmdGetTempPropertiesOutput 
{
    Int  m_MaximumTemperature;      // Maximum temperature
    Int  m_TargetTemperature;       // Target temperature
    char m_TempSensorName[PROPERTY_STR_MAX_SIZE]; // Temperature name
} DtIoctlTempFanMgrCmdGetTempPropertiesOutput;
ASSERT_SIZE(DtIoctlTempFanMgrCmdGetTempPropertiesOutput, 104)

//-.-.-.-.-.-.-.-.-.-.-.-.- TEMPFANMGR - Get Temperature Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlTempFanMgrCmdGetTemperatureInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_TempSensIndex;           // Temperature sensor index
}  DtIoctlTempFanMgrCmdGetTemperatureInput;
ASSERT_SIZE(DtIoctlTempFanMgrCmdGetTemperatureInput, 20)

typedef struct _DtIoctlTempFanMgrCmdGetTemperatureOutput 
{
    Int  m_Temperature;         // Current temperature
} DtIoctlTempFanMgrCmdGetTemperatureOutput;
ASSERT_SIZE(DtIoctlTempFanMgrCmdGetTemperatureOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.- TEMPFANMGR command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.
// TEMP command - Input data
typedef union _DtIoctlTempFanMgrCmdInput
{
    DtIoctlTempFanMgrCmdGetFanStatusInput  m_GetFanStatus;
    DtIoctlTempFanMgrCmdGetNumFansInput  m_GetNumFans;
    DtIoctlTempFanMgrCmdGetNumTempSensorsInput  m_GetNumTemps;
    DtIoctlTempFanMgrCmdGetTempPropertiesInput  m_GetTempProps;
    DtIoctlTempFanMgrCmdGetTemperatureInput  m_GetTemperature;
} DtIoctlTempFanMgrCmdInput;
ASSERT_SIZE(DtIoctlTempFanMgrCmdInput, 20)

// TEMP command - Output data
typedef union _DtIoctlTempFanMgrCmdOutput
{
    DtIoctlTempFanMgrCmdGetFanStatusOutput  m_GetFanStatus;
    DtIoctlTempFanMgrCmdGetNumFansOutput  m_GetNumFans;
    DtIoctlTempFanMgrCmdGetNumTempSensorsOutput  m_GetNumTemps;
    DtIoctlTempFanMgrCmdGetTempPropertiesOutput  m_GetTempProps;
    DtIoctlTempFanMgrCmdGetTemperatureOutput  m_GetTemperature;
}  DtIoctlTempFanMgrCmdOutput;
ASSERT_SIZE(DtIoctlTempFanMgrCmdOutput, 104)

#ifdef WINBUILD
    #define DT_IOCTL_TEMPFANMGR_CMD  CTL_CODE(DT_DEVICE_TYPE,                            \
                                                        DT_FUNC_CODE_TEMPFANMGR_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlTempFanMgrCmdInOut
    {
        DtIoctlTempFanMgrCmdInput  m_Input;
        DtIoctlTempFanMgrCmdOutput  m_Output;
    } DtIoctlTempFanMgrCmdInOut;

    #define DT_IOCTL_TEMPFANMGR_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                          \
                                                          DT_FUNC_CODE_TEMPFANMGR_CMD,   \
                                                          DtIoctlTempFanMgrCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_TOD_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Time-of-Day Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlTodCmd
{
    DT_TOD_CMD_ADJUST_TIME = 0,
    DT_TOD_CMD_GET_PHASE_INCR = 1,
    DT_TOD_CMD_GET_PROPERTIES = 2,
    DT_TOD_CMD_GET_TIME = 3,
    DT_TOD_CMD_SET_PHASE_INCR = 4,
    DT_TOD_CMD_SET_TIME = 5,
    DT_TOD_CMD_ADJUST_PPM = 6,
}  DtIoctlTodCmd;
//.-.-.-.-.-.-.-.-.-.-.-.- TOD Command - Set Adjust Time Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlTodCmdAdjustTimeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int64A  m_DeltaNs;                  // Adjustment in nanoseconds
}  DtIoctlTodCmdAdjustTimeInput;
ASSERT_SIZE(DtIoctlTodCmdAdjustTimeInput, 24)
//.-.-.-.-.-.-.-.-.-.-.- TOD Command - Get Phase Increment Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlTodCmdGetPhaseIncrInput;
ASSERT_SIZE(DtIoctlTodCmdGetPhaseIncrInput, 16)
typedef struct _DtIoctlTodCmdGetPhaseIncrOutput
{
    UInt32  m_PhaseIncr;          // Clock phase increment
}  DtIoctlTodCmdGetPhaseIncrOutput;
ASSERT_SIZE(DtIoctlTodCmdGetPhaseIncrOutput, 4)
//-.-.-.-.-.-.-.-.-.-.-.-.- TOD Command - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlTodCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlTodCmdGetPropertiesInput, 16)
typedef struct _DtIoctlTodCmdGetPropertiesOutput
{
    UInt  m_SysClockFreq;       // System clock frequency in Hz
    UInt  m_SysClockAccuracy;   // System clock accuracy in units of 0.1ppm
}  DtIoctlTodCmdGetPropertiesOutput;
ASSERT_SIZE(DtIoctlTodCmdGetPropertiesOutput, 8)
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- TOD Command - Get Time Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlTodCmdGetTimeInput;
ASSERT_SIZE(DtIoctlTodCmdGetTimeInput, 16)
typedef struct _DtIoctlTodCmdGetTimeOutput
{
    DtTodTime  m_Time;          // Current time
    UInt  m_AdjustmentCount;    // Number of adjustments made to the time-of-day clock
}  DtIoctlTodCmdGetTimeOutput;
ASSERT_SIZE(DtIoctlTodCmdGetTimeOutput, 12)
//.-.-.-.-.-.-.-.-.-.-.- TOD Command - Set Phase Increment Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlTodCmdSetPhaseIncrInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt32  m_PhaseIncr;          // Clock phase increment
}  DtIoctlTodCmdSetPhaseIncrInput;
ASSERT_SIZE(DtIoctlTodCmdSetPhaseIncrInput, 20)
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- TOD Command - Set Time Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlTodCmdSetTimeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    DtTodTime  m_Time;          // New time
}  DtIoctlTodCmdSetTimeInput;
ASSERT_SIZE(DtIoctlTodCmdSetTimeInput, 24)
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TOD Command - AdjustPpm -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlTodCmdAdjustPpmInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int64A  m_ScaledPpm;                  // Adjustment in PPM
}  DtIoctlTodCmdAdjustPpmInput;
ASSERT_SIZE(DtIoctlTodCmdAdjustPpmInput, 24)
//-.-.-.-.-.-.-.-.-.-.-.-.-.- TOD Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// TOD command - IOCTL input data
typedef union _DtIoctlTodCmdInput
{
    DtIoctlTodCmdAdjustTimeInput  m_AdjustTime;         // TOD - Adjust time
    DtIoctlTodCmdSetPhaseIncrInput  m_SetPhaseIncr;     // TOD - Set phase increment
    DtIoctlTodCmdSetTimeInput  m_SetTime;               // TOD - Set time
    DtIoctlTodCmdAdjustPpmInput  m_AdjustPpm;           // TOD - Adjust Ppm
}  DtIoctlTodCmdInput;
// TOD command - IOCTL output data
typedef union _DtIoctlTodCmdOutput
{
    DtIoctlTodCmdGetPropertiesOutput  m_GetProperties;  // TOD - Get properties 
    DtIoctlTodCmdGetPhaseIncrOutput  m_GetPhaseIncr;    // TOD - Get phase increment
    DtIoctlTodCmdGetTimeOutput  m_GetTime;              // TOD - Get time
}  DtIoctlTodCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_TOD_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_TOD_CMD,             \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlTodCmdInOut
    {
        DtIoctlTodCmdInput  m_Input;
        DtIoctlTodCmdOutput  m_Output;
    }  DtIoctlTodCmdInOut;
    #define DT_IOCTL_TOD_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_TOD_CMD,           \
                                                                       DtIoctlTodCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_TODCLOCKCTRL_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TODCLOCKCTRL commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlTodClockCtrlCmd
{
    DT_TODCLOCKCTRL_CMD_GET_STATE         = 0,  // Get state
    DT_TODCLOCKCTRL_CMD_SET_REFERENCE     = 1   // Set TOD clock reference
}  DtIoctlTodClockCtrlCmd;

// TOD clock control states
#define  DT_TODCLOCKCTRL_STATE_INVALID_REF  0
#define  DT_TODCLOCKCTRL_STATE_LOCKING      1
#define  DT_TODCLOCKCTRL_STATE_LOCKED       2
#define  DT_TODCLOCKCTRL_STATE_FREE_RUN     3

// TOD clock reference
#define  DT_TODCLOCKCTRL_REF_INTERNAL       0
#define  DT_TODCLOCKCTRL_REF_STEADYCLOCK    1

// .-.-.-.-.-.-.-.-.-.-.-.-.- TODCLOCKCTRL - Get State Command -.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtIoctlTodClockCtrlCmdGetStateInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlTodClockCtrlCmdGetStateInput;
ASSERT_SIZE(DtIoctlTodClockCtrlCmdGetStateInput, 16)

typedef struct _DtIoctlTodClockCtrlCmdGetStateOutput
{
    Int  m_TodClockCtrlState;   // TOD Clock Control  state
    Int  m_DeviationPpm;        // Deviation in ppm
    Int  m_TodReference;        // Current TOD clock reference
    Int64A  m_TodTimestamp;     // TimeOfDay timestamp
    Int64A  m_RefTimestamp;     // Reference clock timestamp
}  DtIoctlTodClockCtrlCmdGetStateOutput;
ASSERT_SIZE(DtIoctlTodClockCtrlCmdGetStateOutput, 32)

// -.-.-.-.-.-.-.-.-.-.-.- TODCLOCKCTRL - Set TOD clock reference -.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtIoctlTodClockCtrlCmdSetTodReferenceInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_TodReference;        // TOD clock reference
}  DtIoctlTodClockCtrlCmdSetTodReferenceInput;
ASSERT_SIZE(DtIoctlTodClockCtrlCmdSetTodReferenceInput, 20)

// .-.-.-.-.-.-.-.-.-.-.- TODCLOCKCTRL command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
// TODCLOCKCTRL command - Input data
typedef union _DtIoctlTodClockCtrlCmdInput
{
    DtIoctlTodClockCtrlCmdGetStateInput  m_GetState;
    DtIoctlTodClockCtrlCmdSetTodReferenceInput  m_SetReference;
} DtIoctlTodClockCtrlCmdInput;
ASSERT_SIZE(DtIoctlTodClockCtrlCmdInput, 20)

// TODCLOCKCTRL command - Output data
typedef union _DtIoctlTodClockCtrlCmdOutput
{
    DtIoctlTodClockCtrlCmdGetStateOutput  m_GetState;
}  DtIoctlTodClockCtrlCmdOutput;
ASSERT_SIZE(DtIoctlTodClockCtrlCmdOutput, 32)


#ifdef WINBUILD
    #define DT_IOCTL_TODCLOCKCTRL_CMD  CTL_CODE(DT_DEVICE_TYPE,                          \
                                                        DT_FUNC_CODE_TODCLOCKCTRL_CMD,   \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlTodClockCtrlCmdInOut {
        DtIoctlTodClockCtrlCmdInput  m_Input;
        DtIoctlTodClockCtrlCmdOutput  m_Output;
    } DtIoctlTodClockCtrlCmdInOut;

    #define DT_IOCTL_TODCLOCKCTRL_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                        \
                                                         DT_FUNC_CODE_TODCLOCKCTRL_CMD,  \
                                                         DtIoctlTodClockCtrlCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_TSRXFMT_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ASI-TS-Reciever Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlTsRxFmtCmd
{
    DT_TSRXFMT_CMD_GET_OPERATIONAL_STATUS = 0,
    DT_TSRXFMT_CMD_GET_PACKET_MODE = 1,
    DT_TSRXFMT_CMD_GET_PACKET_SIZE = 2,
    DT_TSRXFMT_CMD_GET_SYNC_MODE = 3,
    DT_TSRXFMT_CMD_SET_OPERATIONAL_MODE = 4,
    DT_TSRXFMT_CMD_SET_PACKET_MODE = 5,
    DT_TSRXFMT_CMD_SET_SYNC_MODE = 6,
}  DtIoctlTsRxFmtCmd;


// DT TSRXFMT packetization mode
#define DT_TSRXFMT_PCKMODE_AUTO     0   // Automatic packetization
#define DT_TSRXFMT_PCKMODE_RAW      1   // Raw packetization

// DT TSRXFMT packetsize
#define DT_TSRXFMT_PCKSIZE_UNKNOWN  0   // Unknown packetsize
#define DT_TSRXFMT_PCKSIZE_188      1   // 188-byte TS packets
#define DT_TSRXFMT_PCKSIZE_204      2   // 204-byte TS packets

// DT TSRXFMT synchronization mode
#define DT_TSRXFMT_SYNCMODE_AUTO    0   // Automatic synchronization
#define DT_TSRXFMT_SYNCMODE_188     1   // Synchronize on 188-byte TS packets
#define DT_TSRXFMT_SYNCMODE_204     2   // Synchronize on 204-byte TS packets

//-.-.-.-.-.-.-.-.-.- TSRXFMT Command - Get Operational Status Command -.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlTsRxFmtCmdGetOpStatusInput;
ASSERT_SIZE(DtIoctlTsRxFmtCmdGetOpStatusInput, 16)
typedef struct _DtIoctlTsRxFmtCmdGetOpStatusOutput
{
    Int  m_OpStatus;             // Operational status
}  DtIoctlTsRxFmtCmdGetOpStatusOutput;
ASSERT_SIZE(DtIoctlTsRxFmtCmdGetOpStatusOutput, 4)
//.-.-.-.-.-.-.-.-.-.-.- TSRXFMT Command - Get Packet Mode Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlTsRxFmtCmdGetPckModeInput;
ASSERT_SIZE(DtIoctlTsRxFmtCmdGetPckModeInput, 16)
typedef struct _DtIoctlTsRxFmtCmdGetPckModeOutput
{
    Int  m_PckMode;          // Packetizer mode
}  DtIoctlTsRxFmtCmdGetPckModeOutput;
ASSERT_SIZE(DtIoctlTsRxFmtCmdGetPckModeOutput, 4)
//.-.-.-.-.-.-.-.-.-.-.- TSRXFMT Command - Get Packet Size Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlTsRxFmtCmdGetPacketSizeInput;
ASSERT_SIZE(DtIoctlTsRxFmtCmdGetPacketSizeInput, 16)
typedef struct _DtIoctlTsRxFmtCmdGetPacketSizeOutput
{

    Int  m_PacketSize;          // Detected packet size
}  DtIoctlTsRxFmtCmdGetPacketSizeOutput;
ASSERT_SIZE(DtIoctlTsRxFmtCmdGetPacketSizeOutput, 4)
//-.-.-.-.-.-.-.-.-.-.-.- TSRXFMT Command - Get Sync Mode Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlTsRxFmtCmdGetSyncModeInput;
ASSERT_SIZE(DtIoctlTsRxFmtCmdGetSyncModeInput, 16)
typedef struct _DtIoctlTsRxFmtCmdGetSyncModeOutput
{
    Int  m_SyncMode;            // Synchronizer mode
}  DtIoctlTsRxFmtCmdGetSyncModeOutput;
ASSERT_SIZE(DtIoctlTsRxFmtCmdGetSyncModeOutput, 4)

//.-.-.-.-.-.-.-.-.-.- TSRXFMT Command - Set Operational Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlTsRxFmtCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode: IDLE/RUN
}  DtIoctlTsRxFmtCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlTsRxFmtCmdSetOpModeInput, 20)
//.-.-.-.-.-.-.-.-.-.-.- TSRXFMT Command - Set Packet Mode Command -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlTsRxFmtCmdSetPckModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_PckMode;             // Packetizer mode: AUTO/RAW
}  DtIoctlTsRxFmtCmdSetPckModeInput;
ASSERT_SIZE(DtIoctlTsRxFmtCmdSetPckModeInput, 20)

//-.-.-.-.-.-.-.-.-.- TSRXFMT Command - Set Synchronizer Mode Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlTsRxFmtCmdSetSyncModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SyncMode;        // Synchronizer mode: AUTO/188/204
}  DtIoctlTsRxFmtCmdSetSyncModeInput;
ASSERT_SIZE(DtIoctlTsRxFmtCmdSetSyncModeInput, 20)

//-.-.-.-.-.-.-.-.-.-.-.-.- TSRXFMT Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.
// TSRXFMT command - IOCTL input data
typedef union _DtIoctlTsRxFmtCmdInput
{
    DtIoctlTsRxFmtCmdSetOpModeInput  m_SetOpMode;          // Set operational mode
    DtIoctlTsRxFmtCmdSetPckModeInput  m_SetPckMode;        // Set packet mode
    DtIoctlTsRxFmtCmdSetSyncModeInput  m_SetSyncMode;      // Set syncmode
}  DtIoctlTsRxFmtCmdInput;
// TSRXFMT command - IOCTL output data
typedef union _DtIoctlTsRxFmtCmdOutput
{
    DtIoctlTsRxFmtCmdGetOpStatusOutput  m_GetOpStatus;      // Get operational status
    DtIoctlTsRxFmtCmdGetPckModeOutput  m_GetPacketMode;     // Get packet mode
    DtIoctlTsRxFmtCmdGetPacketSizeOutput  m_GetPacketSize;  // Get signal status
    DtIoctlTsRxFmtCmdGetSyncModeOutput  m_GetSyncMode;      // Get sync mode
}  DtIoctlTsRxFmtCmdOutput;
#ifdef WINBUILD
    #define DT_IOCTL_TSRXFMT_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_TSRXFMT_CMD,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlTsRxFmtCmdInOut
    {
        DtIoctlTsRxFmtCmdInput  m_Input;
        DtIoctlTsRxFmtCmdOutput  m_Output;
    }  DtIoctlTsRxFmtCmdInOut;
    #define DT_IOCTL_TSRXFMT_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_TSRXFMT_CMD,   \
                                                                   DtIoctlTsRxFmtCmdInOut)
#endif

    
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_VPD_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- VPD commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlVpdCmd
{
    DT_VPD_CMD_FORMAT = 0,
    DT_VPD_CMD_GET_PROPERTIES = 1,
    DT_VPD_CMD_ITEM_READ = 2,
    DT_VPD_CMD_ITEM_WRITE = 3,
    DT_VPD_CMD_ITEM_DELETE = 4,
    DT_VPD_CMD_RAW_READ = 5,
    DT_VPD_CMD_RAW_WRITE = 6,
}  DtIoctlVpdCmd;

// DT VPD section ID's
#define DT_VPD_SECT_ID                1
#define DT_VPD_SECT_RO                2
#define DT_VPD_SECT_RW                4

// DT VPD flags
#define DT_VPD_FLAG_RO_WRITE_ALLOWED  1
#define DT_VPD_FLAG_UPDATE_EEPROM     2

// DT VPD size constants
#define DT_VPD_MAX_KEY_LENGTH         15        // Max VPD keyword length
#define DT_VPD_ITEM_MAX_LENGTH        65535     // Max VPD item length


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- VPD Command - Format Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlVpdCmdFormatInput 
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SectionType;
    Int  m_Flags;
} DtIoctlVpdCmdFormatInput;
ASSERT_SIZE(DtIoctlVpdCmdFormatInput, 24)

//-.-.-.-.-.-.-.-.-.-.-.-.- VPD Command - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlVpdCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlVpdCmdGetPropertiesInput, 16)

typedef struct _DtIoctlVpdCmdGetPropertiesOutput
{
    Int  m_RoOffset;    // Offset of start of RO section
    Int  m_RoSize;      // Size of the RO-section
    Int  m_RwOffset;    // Offset of start of RW section
    Int  m_RwSize;      // Size of the RW-section
    Int  m_EepromSize;  // Size of VPD Flash/Eeprom
    Int  m_MaxItemLength;   // Maximum item length
} DtIoctlVpdCmdGetPropertiesOutput;
ASSERT_SIZE(DtIoctlVpdCmdGetPropertiesOutput, 24)

//-.-.-.-.-.-.-.-.-.-.-.-.-.- VPD Command - Item Read Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlVpdCmdItemReadInput 
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SectionType;
    Int  m_KeywordLen;
    char  m_Keyword[DT_VPD_MAX_KEY_LENGTH];
    Int  m_Flags;                              // Not used for now
    Int  m_MaxNumToRead;
} DtIoctlVpdCmdItemReadInput;
ASSERT_SIZE(DtIoctlVpdCmdItemReadInput, 48)

typedef struct _DtIoctlVpdCmdItemReadOutput {
    Int  m_NumRead;
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtIoctlVpdCmdItemReadOutput;
ASSERT_SIZE(DtIoctlVpdCmdItemReadOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.- VPD Command - Item Write Command -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlVpdCmdItemWriteInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SectionType;
    Int  m_KeywordLen;
    char  m_Keyword[DT_VPD_MAX_KEY_LENGTH];
    Int  m_Flags;
    Int  m_NumToWrite;
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtIoctlVpdCmdItemWriteInput;
ASSERT_SIZE(DtIoctlVpdCmdItemWriteInput, 48)

//.-.-.-.-.-.-.-.-.-.-.-.-.- VPD Command - Item Delete Command -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlVpdCmdItemDeleteInput 
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SectionType;
    Int  m_KeywordLen;
    char  m_Keyword[DT_VPD_MAX_KEY_LENGTH];
    Int  m_Flags;
} DtIoctlVpdCmdItemDeleteInput;
ASSERT_SIZE(DtIoctlVpdCmdItemDeleteInput, 44)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- VPD Command - Raw Read Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlVpdCmdRawReadInput 
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt  m_StartOffset;            // Start offset
    Int  m_NumToRead;               // Number of bytes to read
} DtIoctlVpdCmdRawReadInput;
ASSERT_SIZE(DtIoctlVpdCmdRawReadInput, 24)

typedef struct _DtIoctlVpdCmdRawReadOutput 
{
    Int  m_NumRead;                 // Number read
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtIoctlVpdCmdRawReadOutput;
ASSERT_SIZE(DtIoctlVpdCmdRawReadOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.- VPD Command - Raw Write Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlVpdCmdRawWriteInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt  m_StartOffset;            // Start offset
    Int  m_NumToWrite;              // Number of bytes to write
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtIoctlVpdCmdRawWriteInput;
ASSERT_SIZE(DtIoctlVpdCmdRawWriteInput, 24)

//-.-.-.-.-.-.-.-.-.-.-.-.-.- VPD command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// VPD command - Input data
typedef union _DtIoctlVpdCmdInput 
{
    DtIoctlVpdCmdFormatInput  m_Format;
    DtIoctlVpdCmdItemReadInput  m_ItemRead;
    DtIoctlVpdCmdItemWriteInput  m_ItemWrite;
    DtIoctlVpdCmdItemDeleteInput  m_ItemDelete;
    DtIoctlVpdCmdRawReadInput  m_RawRead;
    DtIoctlVpdCmdRawWriteInput  m_RawWrite;
} DtIoctlVpdCmdInput;
ASSERT_SIZE(DtIoctlVpdCmdInput, 48)

// VPD command - Output data
typedef union _DtIoctlVpdCmdOutput 
{
    DtIoctlVpdCmdGetPropertiesOutput  m_GetProps;
    DtIoctlVpdCmdItemReadOutput  m_ItemRead;
    DtIoctlVpdCmdRawReadOutput  m_RawRead;
} DtIoctlVpdCmdOutput;
ASSERT_SIZE(DtIoctlVpdCmdOutput, 24)

#ifdef WINBUILD
    #define DT_IOCTL_VPD_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_VPD_CMD,             \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlVpdCmdInOut
    {
        DtIoctlVpdCmdInput  m_Input;
        DtIoctlVpdCmdOutput  m_Output;
    }  DtIoctlVpdCmdInOut;

    #define DT_IOCTL_VPD_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_VPD_CMD,           \
                                                                       DtIoctlVpdCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_ATTNCTRL_2116_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ATTNCTRL_2116 commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlAttenuatorsCtrl2116Cmd
{
    DT_ATTNCTRL_CMD_2116_GET_ATTENUATORS   = 0, // Get attenuator settings
    DT_ATTNCTRL_CMD_2116_SET_ATTENUATORS   = 1, // Set attenuator settings
}  DtIoctlAttenuatorsCtrlCmd_2116;

// .-.-.-.-.-.-.-.-.-.-.-.- ATTNCTRL_2116 Command - Get Attenuators Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlAttenuatorsCtrlCmd_2116GetAttenuatorsInput;
ASSERT_SIZE(DtIoctlAttenuatorsCtrlCmd_2116GetAttenuatorsInput, 16)
typedef struct _DtIoctlAttenuatorsCtrlCmd_2116GetAttenuatorsOutput
{
    UInt8  m_LastAttenuator;     // Last attenuator setting.
    UInt8  m_MiddleAttenuator;   // Middle attenuator setting.
    UInt8  m_FirstAttenuator;    // First attenuator setting. 
}DtIoctlAttenuatorsCtrlCmd_2116GetAttenuatorsOutput;
ASSERT_SIZE(DtIoctlAttenuatorsCtrlCmd_2116GetAttenuatorsOutput, 3)

// .-.-.-.-.-.-.-.-.-.-.-.- ATTNCTRL_2116 Command - Set Attenuators Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtIoctlAttenuatorsCtrlCmd_2116SetAttenuatorsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    UInt8  m_LastAttenuator;     // Last attenuator setting.
    UInt8  m_MiddleAttenuator;   // Middle attenuator setting.
    UInt8  m_FirstAttenuator;    // First attenuator setting.     
}DtIoctlAttenuatorsCtrlCmd_2116SetAttenuatorsInput;
ASSERT_SIZE(DtIoctlAttenuatorsCtrlCmd_2116SetAttenuatorsInput, 20)

// -.-.-.-.-.-.-.-.-.-.-.-.- ATTNCTRL_2116 command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// ATTNCTRL_2116 command - Input data
typedef union _DtIoctlAttenuatorsCtrlCmdInput_2116
{
    DtIoctlAttenuatorsCtrlCmd_2116SetAttenuatorsInput  m_SetAttenuators;
} DtIoctlAttenuatorsCtrlCmdInput_2116;

// ATTNCTRL_2116 command - Output data
typedef union _DtIoctlAttenuatorsCtrlCmdOutput_2116
{
    DtIoctlAttenuatorsCtrlCmd_2116GetAttenuatorsOutput  m_GetAttenuators;
}  DtIoctlAttenuatorsCtrlCmdOutput_2116;

#ifdef WINBUILD
#define DT_IOCTL_ATTNCTRL_CMD_2116  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_ATTNCTRL_CMD_2116,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlAttenuatorsCtrlCmd_2116InOut {
    DtIoctlAttenuatorsCtrlCmdInput_2116  m_Input;
    DtIoctlAttenuatorsCtrlCmdOutput_2116  m_Output;
} DtIoctlAttenuatorsCtrlCmd_2116InOut;

#define DT_IOCTL_ATTNCTRL_CMD_2116  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_ATTNCTRL_CMD_2116,    \
                                                                   DtIoctlAttenuatorsCtrlCmd_2116InOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IO_2116_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IO_2116 commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIo2116Cmd
{
    DT_IO_CMD_2116_DAC_RESET                = 0, // Reset DAC
    DT_IO_CMD_2116_GET_10MHZ_CLOCK_SELECT   = 1, // Get clock generator 10Mhz input select
    DT_IO_CMD_2116_GET_CALIBRATION_SWITCH   = 2, // Get calibration switch settings
    DT_IO_CMD_2116_GET_CLOCKGEN_STATUS      = 3, // Get clock genrator status    
    DT_IO_CMD_2116_GET_DAC_TX_ENABLE        = 4, // Get DAC enabled state
    DT_IO_CMD_2116_GET_ONEPPS_TERMINATION   = 5, // Get 1PPS termination state
    DT_IO_CMD_2116_SET_10MHZ_CLOCK_SELECT   = 6, // Set clock generator 10Mhz input select
    DT_IO_CMD_2116_SET_CALIBRATION_SWITCH   = 7, // Set calibration switch settings
    DT_IO_CMD_2116_SET_DAC_TX_ENABLE        = 8, // Set DAC enabled state
    DT_IO_CMD_2116_SET_ONEPPS_TERMINATION   = 9,  // Set 1PPS termination state
    DT_IO_CMD_2116_INIT_CLOCKGEN            = 10  // Shutdown clock generator
}  DtIoctlIoCmd_2116;

// -.-.-.-.-.-.-.-.-.-.-.-.- IO_2116 Command - DAC Reset Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlIoCmd_2116DacResetInput;
ASSERT_SIZE(DtIoctlIoCmd_2116DacResetInput, 16)

// 10Mhz clock select flags
#define DT_IO_2116_CLKSELECT_INTERNAL         0   // Internal
#define DT_IO_2116_CLKSELECT_EXTERNAL         1   // External

// .-.-.-.-.-.-.-.-.-.-.-.- IO_2116 Command - Get 10Mhz Clock input Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlIoCmd_2116Get10MhzClockSelectInput;
ASSERT_SIZE(DtIoctlIoCmd_2116Get10MhzClockSelectInput, 16)
typedef struct _DtIoctlIoCmd_2116Get10MhzClockSelectOutput
{
    Int  m_Select;     // INTERNAL, EXTERNAL 
}DtIoctlIoCmd_2116Get10MhzClockSelectOutput;
ASSERT_SIZE(DtIoctlIoCmd_2116Get10MhzClockSelectOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.- IO_2116 Command - Get Calibration switch Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlIoCmd_2116GetCalibrationSwitchInput;
ASSERT_SIZE(DtIoctlIoCmd_2116GetCalibrationSwitchInput, 16)
typedef struct _DtIoctlIoCmd_2116GetCalibrationSwitchOutput
{
    Int  m_Switch1;     // Switch 1 state. TRUE or FALSE
    Int  m_Switch2;     // Switch 2 state. TRUE or FALSE
}DtIoctlIoCmd_2116GetCalibrationSwitchOutput;
ASSERT_SIZE(DtIoctlIoCmd_2116GetCalibrationSwitchOutput, 8)

// .-.-.-.-.-.-.-.-.-.-.-.- IO_2116 Command - Get Clock generator Status Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlIoCmd_2116GetClockGenStatusInput;
ASSERT_SIZE(DtIoctlIoCmd_2116GetClockGenStatusInput, 16)
typedef struct _DtIoctlIoCmd_2116GetClockGenStatusOutput
{
    Int  m_GenStatus;   // OR of (optional) UNLOCK, VCOOK, LOCK, REFOK flags
}DtIoctlIoCmd_2116GetClockGenStatusOutput;
ASSERT_SIZE(DtIoctlIoCmd_2116GetClockGenStatusOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.- IO_2116 Command - Get Dac Tx Enable Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlIoCmd_2116GetDacTxEnableInput;
ASSERT_SIZE(DtIoctlIoCmd_2116GetDacTxEnableInput, 16)
typedef struct _DtIoctlIoCmd_2116GetDacTxEnableOutput
{
    Int  m_Enable;     // Enabled state. TRUE or FALSE
}DtIoctlIoCmd_2116GetDacTxEnableOutput;
ASSERT_SIZE(DtIoctlIoCmd_2116GetDacTxEnableOutput, 4)

// -.-.-.-.-.-.-.-.-.- IO_2116 Command - Get One PPS termination Command -.-.-.-.-.-.-.-.-.-
typedef DtIoctlInputDataHdr DtIoctlIoCmd_2116GetOnePpsTerminationInput;
ASSERT_SIZE(DtIoctlIoCmd_2116GetOnePpsTerminationInput, 16)
typedef struct _DtIoctlIoCmd_2116GetOnePpsTerminationOutput
{
    Int  m_Terminated;     // Terminated state. TRUE or FALSE
}DtIoctlIoCmd_2116GetOnePpsTerminationOutput;
ASSERT_SIZE(DtIoctlIoCmd_2116GetOnePpsTerminationOutput, 4)

// .-.-.-.-.-.-.-.-.-.-.-.- IO_2116 Command - Set 10Mhz Clock input Command  -.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtIoctlIoCmd_2116Set10MhzClockSelectInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Select;     // INTERNAL, EXTERNAL 
}DtIoctlIoCmd_2116Set10MhzClockSelectInput;
ASSERT_SIZE(DtIoctlIoCmd_2116Set10MhzClockSelectInput, 20)

// .-.-.-.-.-.-.-.-.-.-.-.- IO_2116 Command - Set Calibration Switch Command -.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtIoctlIoCmd_2116SetCalibrationSwitchInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Switch1;     // Switch 1 state. TRUE or FALSE
    Int  m_Switch2;     // Switch 2 state. TRUE or FALSE   
}DtIoctlIoCmd_2116SetCalibrationSwitchInput;
ASSERT_SIZE(DtIoctlIoCmd_2116SetCalibrationSwitchInput, 24)

// -.-.-.-.-.-.-.-.-.-.- IO_2116 Command - Set DAC TX enable Command -.-.-.-.-.-.-.-.-.-.-
typedef struct _DtIoctlIoCmd_2116SetDacTxEnableInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Enable;     // Enabled state. TRUE or FALSE
}DtIoctlIoCmd_2116SetDacTxEnableInput;
ASSERT_SIZE(DtIoctlIoCmd_2116SetDacTxEnableInput, 20)

// -.-.-.-.-.-.-.-.-.- IO_2116 Command - Init Clock generator Command -.-.-.-.-.-.-.-.-.-.
typedef DtIoctlInputDataHdr DtIoctlIoCmd_2116InitClockGenInput;
ASSERT_SIZE(DtIoctlIoCmd_2116InitClockGenInput, 16)

// .-.-.-.-.-.-.-.-.- IO_2116 Command - Set One Pps termination Command -.-.-.-.-.-.-.-.-.
typedef struct _DtIoctlIoCmd_2116SetOnePpsTerminationInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Terminated;     // Terminated state. TRUE or FALSE
}DtIoctlIoCmd_2116SetOnePpsTerminationInput;
ASSERT_SIZE(DtIoctlIoCmd_2116SetOnePpsTerminationInput, 20)

// -.-.-.-.-.-.-.-.-.-.-.-.- IO_2116 command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// IO_2116 command - Input data
typedef union _DtIoctlIoCmdInput_2116
{
    DtIoctlIoCmd_2116Set10MhzClockSelectInput  m_Set10MhzClockSelect;
    DtIoctlIoCmd_2116SetCalibrationSwitchInput  m_SetCalibrationSwitch;
    DtIoctlIoCmd_2116DacResetInput  m_DacReset;
    DtIoctlIoCmd_2116InitClockGenInput  m_InitClockGen;
    DtIoctlIoCmd_2116SetDacTxEnableInput  m_SetDacTxEnable;
    DtIoctlIoCmd_2116SetOnePpsTerminationInput  m_SetOnePpsTermination;
} DtIoctlIoCmdInput_2116;

// IO_2116 command - Output data
typedef union _DtIoctlIoCmdOutput_2116
{
    DtIoctlIoCmd_2116GetClockGenStatusOutput  m_GetClockGenStatus;
    DtIoctlIoCmd_2116Get10MhzClockSelectOutput  m_Get10MhzClockSelect;
    DtIoctlIoCmd_2116GetCalibrationSwitchOutput  m_GetCalibrationSwitch;
    DtIoctlIoCmd_2116GetDacTxEnableOutput  m_GetDacTxEnable;
    DtIoctlIoCmd_2116GetOnePpsTerminationOutput  m_GetOnePpsTermination;
}  DtIoctlIoCmdOutput_2116;

#ifdef WINBUILD
#define DT_IOCTL_IO_CMD_2116  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_IO_CMD_2116,         \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtIoctlIoCmd_2116InOut {
    DtIoctlIoCmdInput_2116  m_Input;
    DtIoctlIoCmdOutput_2116  m_Output;
} DtIoctlIoCmd_2116InOut;

#define DT_IOCTL_IO_CMD_2116  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IO_CMD_2116,    \
                                                                   DtIoctlIoCmd_2116InOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_AD5320_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- ADC AD5320 Interface Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlAd5320Cmd_2132
{
    DT_AD5320_CMD_2132_GET_GAIN_CONTROL = 0,
    DT_AD5320_CMD_2132_GET_GAIN_CTRL_PARS = 1,
    DT_AD5320_CMD_2132_GET_GAIN_STATUS = 2,
    DT_AD5320_CMD_2132_GET_OPERATIONAL_MODE = 3,
    DT_AD5320_CMD_2132_SET_GAIN_CONTROL = 4,
    DT_AD5320_CMD_2132_SET_GAIN_CTRL_PARS = 5,
    DT_AD5320_CMD_2132_SET_OPERATIONAL_MODE = 6,
}  DtIoctlAd5320Cmd_2132;

// GainStatus
#define DT_AD5320_2132_GAIN_NOLOCK  0x0  // Gain controller is not locked
#define DT_AD5320_2132_GAIN_LOCKED  0x1  // Gain controller is locked

// GainMode
#define DT_AD5320_2132_GAIN_AUTO    0x0  // Gain automatically calculated
#define DT_AD5320_2132_GAIN_MANUAL  0x1  // Manual gain

//.-.-.-.-.-.-.-.-.-.- AD5320_2132 Command - Get Gain Control Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAd5320Cmd_2132GetGainControlInput;
ASSERT_SIZE(DtIoctlAd5320Cmd_2132GetGainControlInput, 16)
typedef struct _DtIoctlAd5320Cmd_2132GetGainControlOutput
{
    Int  m_GainControl;              // Gain control
    Int  m_ManualGain;               // Manual gain
}  DtIoctlAd5320Cmd_2132GetGainControlOutput;
ASSERT_SIZE(DtIoctlAd5320Cmd_2132GetGainControlOutput, 8)

//.-.-.-.-.-.-.- AD5320_2132 Command - Get Gain Control Parameters Command -.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlAd5320Cmd_2132GetGainCtrlParsInput;
ASSERT_SIZE(DtIoctlAd5320Cmd_2132GetGainCtrlParsInput, 16)
typedef struct _DtIoctlAd5320Cmd_2132GetGainCtrlParsOutput
{
    Int  m_StepSizeUnlocked;    // Gain control step size when not locked
    Int  m_StepSizeLocked;      // Gain control step size when  locked
    Int  m_OverflowThreshold;   // Signed threshold value for detecting overflows
    Int  m_WorkingThreshold;    // Signed threshold value for detecting working level
    UInt  m_TimeWindow;         // Time window for the gain measurement
}  DtIoctlAd5320Cmd_2132GetGainCtrlParsOutput;
ASSERT_SIZE(DtIoctlAd5320Cmd_2132GetGainCtrlParsOutput, 20)


//.-.-.-.-.-.-.-.-.-.- AD5320_2132 Command - Get Gain Status Command -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlAd5320Cmd_2132GetGainStatusInput;
ASSERT_SIZE(DtIoctlAd5320Cmd_2132GetGainStatusInput, 16)
typedef struct _DtIoctlAd5320Cmd_2132GetGainStatusOutput
{
    Int  m_AutoGainStatus;           // Auto gain Status
    Int  m_GainReadBack;             // Gain readback value
}  DtIoctlAd5320Cmd_2132GetGainStatusOutput;
ASSERT_SIZE(DtIoctlAd5320Cmd_2132GetGainStatusOutput, 8)

//.-.-.-.-.-.-.-.-.- AD5320_2132 Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAd5320Cmd_2132GetOpModeInput;
ASSERT_SIZE(DtIoctlAd5320Cmd_2132GetOpModeInput, 16)
typedef struct _DtIoctlAd5320Cmd_2132GetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlAd5320Cmd_2132GetOpModeOutput;
ASSERT_SIZE(DtIoctlAd5320Cmd_2132GetOpModeOutput, 4)

//.-.-.-.-.-.-.-.-.-.- AD5320_2132 Command - Set Gain Control Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlAd5320Cmd_2132SetGainControlInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_GainControl;              // Gain control
    Int  m_ManualGain;               // Manual gain
}  DtIoctlAd5320Cmd_2132SetGainControlInput;
ASSERT_SIZE(DtIoctlAd5320Cmd_2132SetGainControlInput, 24)

//.-.-.-.-.-.-.- AD5320_2132 Command - Set Gain Control Parameters Command -.-.-.-.-.-.-.-
//
typedef struct _DtIoctlAd5320Cmd_2132SetGainCtrlParsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_StepSizeUnlocked;    // Gain control step size when not locked
    Int  m_StepSizeLocked;      // Gain control step size when  locked
    Int  m_OverflowThreshold;   // Signed threshold value for detecting overflows
    Int  m_WorkingThreshold;    // Signed threshold value for detecting working level
    UInt  m_TimeWindow;         // Time window for the gain measurement
}  DtIoctlAd5320Cmd_2132SetGainCtrlParsInput;
ASSERT_SIZE(DtIoctlAd5320Cmd_2132SetGainCtrlParsInput, 36)

//-.-.-.-.-.-.-.-.- AD5320_2132 Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlAd5320Cmd_2132SetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlAd5320Cmd_2132SetOpModeInput;
ASSERT_SIZE(DtIoctlAd5320Cmd_2132SetOpModeInput, 20)
//-.-.-.-.-.-.-.-.-.-.-.- AD5320_2132 Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
// AD5320_2132 command - IOCTL input data
typedef union _DtIoctlAd5320CmdInput_2132
{
    DtIoctlAd5320Cmd_2132SetGainControlInput  m_SetGainCtrl;  // Set gain control
    DtIoctlAd5320Cmd_2132SetGainCtrlParsInput  m_SetGainPars; // Set gain control pars
    DtIoctlAd5320Cmd_2132SetOpModeInput  m_SetOpMode;         // Set operational mode
}  DtIoctlAd5320CmdInput_2132;
// AD5320_2132 command - IOCTL output data
typedef union _DtIoctlAd5320CmdOutput_2132
{
    DtIoctlAd5320Cmd_2132GetGainControlOutput  m_GetGainCtrl;  // Get gain control
    DtIoctlAd5320Cmd_2132GetGainCtrlParsOutput m_GetGainPars;  // Get gain control pars
    DtIoctlAd5320Cmd_2132GetGainStatusOutput  m_GetGainStatus; // Get gain control pars
    DtIoctlAd5320Cmd_2132GetOpModeOutput  m_GetOpMode;         // Get operational mode
}  DtIoctlAd5320CmdOutput_2132;
#ifdef WINBUILD
    #define DT_IOCTL_AD5320_CMD_2132  CTL_CODE(DT_DEVICE_TYPE,                           \
                                                        DT_FUNC_CODE_AD5320_CMD_2132,    \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlAd5320Cmd_2132InOut
    {
        DtIoctlAd5320CmdInput_2132  m_Input;
        DtIoctlAd5320CmdOutput_2132  m_Output;
    }  DtIoctlAd5320Cmd_2132InOut;
    #define DT_IOCTL_AD5320_CMD_2132  _IOWR(DT_IOCTL_MAGIC_SIZE,                         \
                                                         DT_FUNC_CODE_AD5320_CMD_2132,   \
                                                         DtIoctlAd5320Cmd_2132InOut)
#endif
    
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_ADS4246_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- ADC ADS4246 Interface Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlAds4246Cmd_2132
{
    DT_ADS4246_CMD_2132_GET_OPERATIONAL_MODE = 0,
    DT_ADS4246_CMD_2132_SET_OPERATIONAL_MODE = 1,
}  DtIoctlAds4246Cmd_2132;

//-.-.-.-.-.-.-.-.- ADS4246_2132 Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlAds4246Cmd_2132GetOpModeInput;
ASSERT_SIZE(DtIoctlAds4246Cmd_2132GetOpModeInput, 16)
typedef struct _DtIoctlAds4246Cmd_2132GetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlAds4246Cmd_2132GetOpModeOutput;
ASSERT_SIZE(DtIoctlAds4246Cmd_2132GetOpModeOutput, 4)
//-.-.-.-.-.-.-.-.- ADS4246_2132 Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlAds4246Cmd_2132SetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlAds4246Cmd_2132SetOpModeInput;
ASSERT_SIZE(DtIoctlAds4246Cmd_2132SetOpModeInput, 20)

//-.-.-.-.-.-.-.-.-.-.-.- ADS4246_2132 Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
// ADS4246_2132 command - IOCTL input data
typedef union _DtIoctlAds4246CmdInput_2132
{
    DtIoctlAds4246Cmd_2132SetOpModeInput  m_SetOpMode;      // Ads4246_2132 - Set op. mode
}  DtIoctlAds4246CmdInput_2132;
// ADS4246_2132 command - IOCTL output data
typedef union _DtIoctlAds4246CmdOutput_2132
{
    DtIoctlAds4246Cmd_2132GetOpModeOutput  m_GetOpMode;     // Ads4246_2132 - Get op. mode
}  DtIoctlAds4246CmdOutput_2132;
#ifdef WINBUILD
    #define DT_IOCTL_ADS4246_CMD_2132  CTL_CODE(DT_DEVICE_TYPE,                          \
                                                        DT_FUNC_CODE_ADS4246_CMD_2132,   \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlAds4246Cmd_2132InOut
    {
        DtIoctlAds4246CmdInput_2132  m_Input;
        DtIoctlAds4246CmdOutput_2132  m_Output;
    }  DtIoctlAds4246Cmd_2132InOut;
    #define DT_IOCTL_ADS4246_CMD_2132  _IOWR(DT_IOCTL_MAGIC_SIZE,                        \
                                                        DT_FUNC_CODE_ADS4246_CMD_2132,   \
                                                        DtIoctlAds4246Cmd_2132InOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_DATASTMUX_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DATASTMUX Interface Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlDataStMuxCmd_2132
{
    DT_DATASTMUX_CMD_2132_GET_SELECTION = 0,
    DT_DATASTMUX_CMD_2132_SET_SELECTION = 1,
}  DtIoctlDataStMuxCmd_2132;

// Selection
#define DT_DATASTMUX_2132_SELECT_IQ     0x0  // Select I/Q samples
#define DT_DATASTMUX_2132_SELECT_L3     0x1  // Select L3-frames


//.-.-.-.-.-.-.-.-.-.-.-.- DATASTMUX_2132 Command - Get Selection -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlDataStMuxCmd_2132GetSelectionInput;
ASSERT_SIZE(DtIoctlDataStMuxCmd_2132GetSelectionInput, 16)
typedef struct _DtIoctlDataStMuxCmd_2132GetSelectionOutput
{
    UInt32  m_Selection;         // Selection
}  DtIoctlDataStMuxCmd_2132GetSelectionOutput;
ASSERT_SIZE(DtIoctlDataStMuxCmd_2132GetSelectionOutput, 4)


//.-.-.-.-.-.-.-.-.-.-.-.- DATASTMUX_2132 Command - Set Selection -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlDataStMuxCmd_2132SetSelectionInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_Selection;         // Selection
}  DtIoctlDataStMuxCmd_2132SetSelectionInput;
ASSERT_SIZE(DtIoctlDataStMuxCmd_2132SetSelectionInput, 20)


//.-.-.-.-.-.-.-.-.-.-.- DATASTMUX_2132 Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.
// DATASTMUX_2132 command - IOCTL input data
typedef union _DtIoctlDataStMuxCmdInput_2132
{
    DtIoctlDataStMuxCmd_2132SetSelectionInput  m_SetSelection;      // Set Selection
}  DtIoctlDataStMuxCmdInput_2132;
// DATASTMUX_2132 command - IOCTL output data
typedef union _DtIoctlDataStMuxCmdOutput_2132
{
    DtIoctlDataStMuxCmd_2132GetSelectionOutput  m_GetSelection;     // Get Selection
}  DtIoctlDataStMuxCmdOutput_2132;
#ifdef WINBUILD
    #define DT_IOCTL_DATASTMUX_CMD_2132  CTL_CODE(DT_DEVICE_TYPE,                        \
                                                        DT_FUNC_CODE_DATASTMUX_CMD_2132, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlDataStMuxCmd_2132InOut
    {
        DtIoctlDataStMuxCmdInput_2132  m_Input;
        DtIoctlDataStMuxCmdOutput_2132  m_Output;
    }  DtIoctlDataStMuxCmd_2132InOut;
    #define DT_IOCTL_DATASTMUX_CMD_2132  _IOWR(DT_IOCTL_MAGIC_SIZE,                      \
                                                      DT_FUNC_CODE_DATASTMUX_CMD_2132,   \
                                                      DtIoctlDataStMuxCmd_2132InOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQCGRAB_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IQCGRAB Interface Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqCGrabCmd_2132
{
    DT_IQCGRAB_CMD_2132_GET_IQ_POINTS = 0,
}  DtIoctlIqCGrabCmd_2132;


//-.-.-.-.-.-.-.-.-.-.-.- IQCGRAB_2132 - Get Coefficients Command  -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlIqCGrabCmd_2132GetIqPointsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_StreamId;            // Stream Identifier to capture the IQ constellation from
    Int  m_NumIqPointsToGet;    // Number of samples to capture for the IQ constellation
}  DtIoctlIqCGrabCmd_2132GetIqPointsInput;
ASSERT_SIZE(DtIoctlIqCGrabCmd_2132GetIqPointsInput, 24)

typedef struct _DtIoctlIqCGrabCmd_2132GetIqPointsOutput 
{
    Int  m_NumIqPoints;            // Number of IQ constellation points in buffer
    Int16  m_pIqPoints[0];         // Dynamic sized buffer
}  DtIoctlIqCGrabCmd_2132GetIqPointsOutput;
ASSERT_SIZE(DtIoctlIqCGrabCmd_2132GetIqPointsOutput, 4)


//-.-.-.-.-.-.-.-.-.-.-.- IQCGRAB_2132 Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
// IQCGRAB_2132 command - IOCTL input data
typedef union _DtIoctlIqCGrabCmdInput_2132
{
    DtIoctlIqCGrabCmd_2132GetIqPointsInput  m_GetIqPoints;  // Get constellation points
}  DtIoctlIqCGrabCmdInput_2132;
// IQCGRAB_2132 command - IOCTL output data
typedef union _DtIoctlIqCGrabCmdOutput_2132
{
    DtIoctlIqCGrabCmd_2132GetIqPointsOutput  m_GetIqPoints;  // Get constellation points
}  DtIoctlIqCGrabCmdOutput_2132;
#ifdef WINBUILD
    #define DT_IOCTL_IQCGRAB_CMD_2132  CTL_CODE(DT_DEVICE_TYPE,                          \
                                                        DT_FUNC_CODE_IQCGRAB_CMD_2132,   \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqCGrabCmd_2132InOut
    {
        DtIoctlIqCGrabCmdInput_2132  m_Input;
        DtIoctlIqCGrabCmdOutput_2132  m_Output;
    }  DtIoctlIqCGrabCmd_2132InOut;
    #define DT_IOCTL_IQCGRAB_CMD_2132  _IOWR(DT_IOCTL_MAGIC_SIZE,                        \
                                                        DT_FUNC_CODE_IQCGRAB_CMD_2132,   \
                                                        DtIoctlIqCGrabCmd_2132InOut)
#endif
 
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQFIR_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IQFIR Interface Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqFirCmd_2132
{
    DT_IQFIR_CMD_2132_GET_CONFIG = 0,
    DT_IQFIR_CMD_2132_GET_COEFS = 1,
    DT_IQFIR_CMD_2132_SET_COEFS = 2,
}  DtIoctlIqFirCmd_2132;

//-.-.-.-.-.-.-.-.-.-.-.- IQFIR_2132 Command - Get Config Command -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqFirCmd_2132GetConfigInput;
ASSERT_SIZE(DtIoctlIqFirCmd_2132GetConfigInput, 16)
typedef struct _DtIoctlIqFirCmd_2132GetConfigOutput
{
    Int  m_NumberCoefs;         // Number of coefficients
    Int  m_CoefSize;            // Coefficient size in number of bits
}  DtIoctlIqFirCmd_2132GetConfigOutput;
ASSERT_SIZE(DtIoctlIqFirCmd_2132GetConfigOutput, 8)


//.-.-.-.-.-.-.-.-.-.-.-.- IQFIR_2132 - Get Coefficients Command  -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqFirCmd_2132GetCoefsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumCoefsToGet;       // Number of coefficients to get
}  DtIoctlIqFirCmd_2132GetCoefsInput;
ASSERT_SIZE(DtIoctlIqFirCmd_2132GetCoefsInput, 20)

typedef struct _DtIoctlIqFirCmd_2132GetCoefsOutput 
{
    Int  m_NumCoefs;            // Number of coefficients in buffer
    Int  m_pCoefs[0];           // Dynamic sized buffer
}  DtIoctlIqFirCmd_2132GetCoefsOutput;
ASSERT_SIZE(DtIoctlIqFirCmd_2132GetCoefsOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.- IQFIR_2132 - Set Coefficients Command  -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqFirCmd_2132SetCoefsInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumCoefsToSet;       // Number of coefficients to get
    Int  m_pCoefs[0];           // Dynamic sized buffer
}  DtIoctlIqFirCmd_2132SetCoefsInput;
ASSERT_SIZE(DtIoctlIqFirCmd_2132SetCoefsInput, 20)


//-.-.-.-.-.-.-.-.-.-.-.- IQFIR_2132 Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
// IQFIR_2132 command - IOCTL input data
typedef union _DtIoctlIqFirCmdInput_2132
{
    DtIoctlIqFirCmd_2132GetCoefsInput  m_GetCoefs;      // Get Coefficients
    DtIoctlIqFirCmd_2132SetCoefsInput  m_SetCoefs;      // Set Coefficients
}  DtIoctlIqFirCmdInput_2132;
// IQFIR_2132 command - IOCTL output data
typedef union _DtIoctlIqFirCmdOutput_2132
{
    DtIoctlIqFirCmd_2132GetConfigOutput  m_GetConfig;    // Get Config
    DtIoctlIqFirCmd_2132GetCoefsOutput  m_GetCoefs;     // Get Coefficients
}  DtIoctlIqFirCmdOutput_2132;
#ifdef WINBUILD
    #define DT_IOCTL_IQFIR_CMD_2132  CTL_CODE(DT_DEVICE_TYPE,                            \
                                                        DT_FUNC_CODE_IQFIR_CMD_2132,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqFirCmd_2132InOut
    {
        DtIoctlIqFirCmdInput_2132  m_Input;
        DtIoctlIqFirCmdOutput_2132  m_Output;
    }  DtIoctlIqFirCmd_2132InOut;
    #define DT_IOCTL_IQFIR_CMD_2132  _IOWR(DT_IOCTL_MAGIC_SIZE,                          \
                                                          DT_FUNC_CODE_IQFIR_CMD_2132,   \
                                                          DtIoctlIqFirCmd_2132InOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQPWR_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IQPWR Interface Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqPwrCmd_2132
{
    DT_IQPWR_CMD_2132_GET_EST_POWER = 0,
    DT_IQPWR_CMD_2132_GET_EST_WINDOW = 1,
    DT_IQPWR_CMD_2132_SET_EST_WINDOW = 2,
}  DtIoctlIqPwrCmd_2132;

//-.-.-.-.-.-.-.-.-.- IQPWR_2132 Command - Get Estimated Power Command -.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlIqPwrCmd_2132GetEstPowerInput;
ASSERT_SIZE(DtIoctlIqPwrCmd_2132GetEstPowerInput, 16)
typedef struct _DtIoctlIqPwrCmd_2132GetEstPowerOutput
{
    UInt32  m_EstPower;         // Estimated power
}  DtIoctlIqPwrCmd_2132GetEstPowerOutput;
ASSERT_SIZE(DtIoctlIqPwrCmd_2132GetEstPowerOutput, 4)

//.-.-.-.-.-.-.-.-.- IQPWR_2132 Command - Get Estimation Window Command -.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqPwrCmd_2132GetEstWindowInput;
ASSERT_SIZE(DtIoctlIqPwrCmd_2132GetEstWindowInput, 16)
typedef struct _DtIoctlIqPwrCmd_2132GetEstWindowOutput
{
    Int  m_EstWindow;         // Estimation window
}  DtIoctlIqPwrCmd_2132GetEstWindowOutput;
ASSERT_SIZE(DtIoctlIqPwrCmd_2132GetEstWindowOutput, 4)



//.-.-.-.-.-.-.-.-.- IQPWR_2132 Command - Set Estimation Window Command -.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqPwrCmd_2132SetEstWindowInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_EstWindow;         // Estimation window
}  DtIoctlIqPwrCmd_2132SetEstWindowInput;
ASSERT_SIZE(DtIoctlIqPwrCmd_2132SetEstWindowInput, 20)


//-.-.-.-.-.-.-.-.-.-.-.- IQPWR_2132 Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
// IQPWR_2132 command - IOCTL input data
typedef union _DtIoctlIqPwrCmdInput_2132
{
    DtIoctlIqPwrCmd_2132SetEstWindowInput  m_SetEstWindow;      // Set estimation window
}  DtIoctlIqPwrCmdInput_2132;
// IQPWR_2132 command - IOCTL output data
typedef union _DtIoctlIqPwrCmdOutput_2132
{
    DtIoctlIqPwrCmd_2132GetEstPowerOutput  m_GetEstPower;       // Get estimated power
    DtIoctlIqPwrCmd_2132GetEstWindowOutput  m_GetEstWindow;     // Get estimation window
}  DtIoctlIqPwrCmdOutput_2132;
#ifdef WINBUILD
    #define DT_IOCTL_IQPWR_CMD_2132  CTL_CODE(DT_DEVICE_TYPE,                            \
                                                        DT_FUNC_CODE_IQPWR_CMD_2132,     \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqPwrCmd_2132InOut
    {
        DtIoctlIqPwrCmdInput_2132  m_Input;
        DtIoctlIqPwrCmdOutput_2132  m_Output;
    }  DtIoctlIqPwrCmd_2132InOut;
    #define DT_IOCTL_IQPWR_CMD_2132  _IOWR(DT_IOCTL_MAGIC_SIZE,                          \
                                                          DT_FUNC_CODE_IQPWR_CMD_2132,   \
                                                          DtIoctlIqPwrCmd_2132InOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IQSRC2132_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IQSRC2132 Interface Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlIqSrc2132Cmd_2132
{
    DT_IQSRC2132_CMD_2132_GET_SAMPLE_RATE_CONVERSION = 0,
    DT_IQSRC2132_CMD_2132_SET_SAMPLE_RATE_CONVERSION = 1,
}  DtIoctlIqSrc2132Cmd_2132;

// Selection
#define DT_IQSRC2132_2132_FRAC_0_5      0x0  // 1/2 sample rate
#define DT_IQSRC2132_2132_FRAC_0_25     0x1  // 1/4 sample rate
#define DT_IQSRC2132_2132_FRAC_0_125    0x2  // 1/8 sample rate
#define DT_IQSRC2132_2132_FRAC_0_0625   0x3  // 1/16 sample rate
#define DT_IQSRC2132_2132_FRAC_0_03125  0x4  // 1/32 sample rate


//-.-.-.-.-.-.- IQSRC2132_2132 Command - Get Sample Rate Conversion Command -.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlIqSrc2132Cmd_2132GetSampleRateConversionInput;
ASSERT_SIZE(DtIoctlIqSrc2132Cmd_2132GetSampleRateConversionInput, 16)
typedef struct _DtIoctlIqSrc2132Cmd_2132GetSampleRateConversionOutput
{
    Int  m_SrcEnabled;         // Enable sample rate conversion
    UInt32  m_SrcFraction;     // Sample rate fraction
}  DtIoctlIqSrc2132Cmd_2132GetSampleRateConversionOutput;
ASSERT_SIZE(DtIoctlIqSrc2132Cmd_2132GetSampleRateConversionOutput, 8)

//-.-.-.-.-.-.- IQSRC2132_2132 Command - Set Sample Rate Conversion Command -.-.-.-.-.-.-.
//
typedef struct _DtIoctlIqSrc2132Cmd_2132SetSampleRateConversionInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SrcEnable;          // Enable sample rate conversion
    UInt32  m_SrcFraction;     // Sample rate fraction
}  DtIoctlIqSrc2132Cmd_2132SetSampleRateConversionInput;
ASSERT_SIZE(DtIoctlIqSrc2132Cmd_2132SetSampleRateConversionInput, 24)


//.-.-.-.-.-.-.-.-.-.-.- IQSRC2132_2132 Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.
// IQSRC2132_2132 command - IOCTL input data
typedef union _DtIoctlIqSrc2132CmdInput_2132
{
    DtIoctlIqSrc2132Cmd_2132SetSampleRateConversionInput  m_SetSrc;  // Set SRC
}  DtIoctlIqSrc2132CmdInput_2132;
// IQSRC2132_2132 command - IOCTL output data
typedef union _DtIoctlIqSrc2132CmdOutput_2132
{
    DtIoctlIqSrc2132Cmd_2132GetSampleRateConversionOutput  m_GetSrc;  // Get SRC
}  DtIoctlIqSrc2132CmdOutput_2132;
#ifdef WINBUILD
    #define DT_IOCTL_IQSRC2132_CMD_2132  CTL_CODE(DT_DEVICE_TYPE,                        \
                                                        DT_FUNC_CODE_IQSRC2132_CMD_2132, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlIqSrc2132Cmd_2132InOut
    {
        DtIoctlIqSrc2132CmdInput_2132  m_Input;
        DtIoctlIqSrc2132CmdOutput_2132  m_Output;
    }  DtIoctlIqSrc2132Cmd_2132InOut;
    #define DT_IOCTL_IQSRC2132_CMD_2132  _IOWR(DT_IOCTL_MAGIC_SIZE,                      \
                                                      DT_FUNC_CODE_IQSRC2132_CMD_2132,   \
                                                      DtIoctlIqSrc2132Cmd_2132InOut)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_LNBH25_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- LNBH25 Interface Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlLnbh25Cmd_2132
{
    DT_LNBH25_CMD_2132_SEND_TONEBURST = 0,
    DT_LNBH25_CMD_2132_SEND_MESSAGE = 1,
    DT_LNBH25_CMD_2132_SEND_RECEIVE_MESSAGE = 2,
    DT_LNBH25_CMD_2132_ENABLE_TONE = 3

}  DtIoctlLnbh25Cmd_2132;
// Tone burst
#define DT_LNBH25_2132_TONEBURST_A      0x0  // Tone burst A
#define DT_LNBH25_2132_TONEBURST_B      0x1  // Tone burst B

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- LNBH25_2132 - Enable Tone -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtIoctlLnbh25Cmd_2132EnableToneInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_EnableTone;            // Enable 22 kHz tone (bool)
}  DtIoctlLnbh25Cmd_2132EnableToneInput;
ASSERT_SIZE(DtIoctlLnbh25Cmd_2132EnableToneInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- LNBH25_2132 - Send Burst Tone -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlLnbh25Cmd_2132SendToneBurstInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_ToneBurst;            // ToneBurst
}  DtIoctlLnbh25Cmd_2132SendToneBurstInput;
ASSERT_SIZE(DtIoctlLnbh25Cmd_2132SendToneBurstInput, 20)
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- LNBH25_2132 - Send Message -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlLnbh25Cmd_2132SendMessageInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumToSend;           // Number of bytes in send buffer
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlLnbh25Cmd_2132SendMessageInput;
ASSERT_SIZE(DtIoctlLnbh25Cmd_2132SendMessageInput, 20)
//.-.-.-.-.-.-.-.-.-.-.-.-.- LNBH25_2132 - Send Receive Message -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlLnbh25Cmd_2132SendReceiveMessageInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumToSend;          // Number of bytes in send buffer
    Int  m_RcvBufSize;         // Receive buffer size 
    UInt8  m_Buf[0];           // Dynamic sized buffer
}  DtIoctlLnbh25Cmd_2132SendReceiveMessageInput;
ASSERT_SIZE(DtIoctlLnbh25Cmd_2132SendReceiveMessageInput, 24)
typedef struct _DtIoctlLnbh25Cmd_2132SendReceiveMessageOutput 
{
    Int  m_NumRcvd;             // Number of bytes received in buffer
    UInt8  m_Buf[0];            // Dynamic sized buffer
}  DtIoctlLnbh25Cmd_2132SendReceiveMessageOutput;
ASSERT_SIZE(DtIoctlLnbh25Cmd_2132SendReceiveMessageOutput, 4)
//-.-.-.-.-.-.-.-.-.-.-.- LNBH25_2132 Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
// LNBH25_2132 command - IOCTL input data
typedef union _DtIoctlLnbh25CmdInput_2132
{
    DtIoctlLnbh25Cmd_2132EnableToneInput    m_EnableTone;       // Enable 22 kHz tone
    DtIoctlLnbh25Cmd_2132SendToneBurstInput  m_SndToneBurst;    // Send tone burst
    DtIoctlLnbh25Cmd_2132SendMessageInput   m_SndMsg;           // Send message
    DtIoctlLnbh25Cmd_2132SendReceiveMessageInput  m_SndRcvMsg;  // Send/receive message
}  DtIoctlLnbh25CmdInput_2132;
// LNBH25_2132 command - IOCTL output data
typedef union _DtIoctlLnbh25CmdOutput_2132
{
    DtIoctlLnbh25Cmd_2132SendReceiveMessageOutput  m_SndRcvMsg;  // Send/receive message
}  DtIoctlLnbh25CmdOutput_2132;
#ifdef WINBUILD
    #define DT_IOCTL_LNBH25_CMD_2132  CTL_CODE(DT_DEVICE_TYPE,                           \
                                                        DT_FUNC_CODE_LNBH25_CMD_2132,    \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlLnbh25Cmd_2132InOut
    {
        DtIoctlLnbh25CmdInput_2132  m_Input;
        DtIoctlLnbh25CmdOutput_2132  m_Output;
    }  DtIoctlLnbh25Cmd_2132InOut;
    #define DT_IOCTL_LNBH25_CMD_2132  _IOWR(DT_IOCTL_MAGIC_SIZE,                         \
                                                         DT_FUNC_CODE_LNBH25_CMD_2132,   \
                                                         DtIoctlLnbh25Cmd_2132InOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_S2CRDEMOD_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- S2CRDEMOD Interface Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlS2CrDemodCmd_2132
{
    DT_S2CRDEMOD_CMD_2132_GET_OPERATIONAL_MODE = 0,
    DT_S2CRDEMOD_CMD_2132_GET_ROLL_OFF = 1,
    DT_S2CRDEMOD_CMD_2132_GET_SNR_INFO = 2,
    DT_S2CRDEMOD_CMD_2132_GET_SPECTRUM_INV = 3,
    DT_S2CRDEMOD_CMD_2132_GET_STATUS = 4,
    DT_S2CRDEMOD_CMD_2132_GET_SYMBOL_RATE = 5,
    DT_S2CRDEMOD_CMD_2132_SET_OPERATIONAL_MODE = 6,
    DT_S2CRDEMOD_CMD_2132_SET_SPECTRUM_INV = 7,
    DT_S2CRDEMOD_CMD_2132_SET_SYMBOL_RATE = 8,
    DT_S2CRDEMOD_CMD_2132_SOFT_RESET = 9,
    DT_S2CRDEMOD_CMD_2132_GET_COUNTERS = 10,
    DT_S2CRDEMOD_CMD_2132_GET_FREQ_OFFSET = 11,
    DT_S2CRDEMOD_CMD_2132_DEMOD_RESET = 12,
    DT_S2CRDEMOD_CMD_2132_GET_PL_INFORMATION = 13,
    DT_S2CRDEMOD_CMD_2132_SET_PL_INFORMATION = 14

}  DtIoctlS2CrDemodCmd_2132;

// Roll-off factor
#define DT_S2CRDEMOD_2132_ROLL_OFF_UNKNOWN  -1    // Unknown roll off
#define DT_S2CRDEMOD_2132_ROLL_OFF_0_35      0    // 0.35
#define DT_S2CRDEMOD_2132_ROLL_OFF_0_25      1    // 0.25
#define DT_S2CRDEMOD_2132_ROLL_OFF_0_20      2    // 0.20
#define DT_S2CRDEMOD_2132_ROLL_OFF_0_15      3    // 0.15
#define DT_S2CRDEMOD_2132_ROLL_OFF_0_10      4    // 0.10
#define DT_S2CRDEMOD_2132_ROLL_OFF_0_05      5    // 0.05

// Spectrum Inversion
#define DT_S2CRDEMOD_2132_SPECINV_AUTO    0        // Auto spectrum inversion detection
#define DT_S2CRDEMOD_2132_SPECINV_NORMAL  1        // Normal spectrum
#define DT_S2CRDEMOD_2132_SPECINV_INVERT  2        // Inverted spectrum

// Coding and modulation type
#define DT_S2CRDEMOD_2132_PLMODE_ACM  0   // Coding and modulation type ACM
#define DT_S2CRDEMOD_2132_PLMODE_CCM  1   // Coding and modulation type VCM
#define DT_S2CRDEMOD_2132_PLMODE_VCM  2   // Coding and modulation type CCM


//.-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Execute Demod Reset  -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlS2CrDemodCmd_2132DemodResetInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132DemodResetInput, 16)

//.-.-.-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Get Counters -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlS2CrDemodCmd_2132GetCountersInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetCountersInput, 16)
typedef struct _DtIoctlS2CrDemodCmd_2132GetCountersOutput
{
    UInt  m_FrameCount;          // Frame count
    UInt  m_FrameSkipCount;      // Skipped frames count
    UInt  m_LdcpSuccessCount;    // LDCP success count
    UInt  m_LdcpIterationsCount; // LDCP iterations count
    UInt  m_BchSuccessCount;     // BCH success count
}  DtIoctlS2CrDemodCmd_2132GetCountersOutput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetCountersOutput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Get Freq Offset .-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlS2CrDemodCmd_2132GetFreqOffsetInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetFreqOffsetInput, 16)
typedef struct _DtIoctlS2CrDemodCmd_2132GetFreqOffsetOutput
{
    Int  m_CoarseFreq;          // Coarse frequency
    Int  m_FineFreq;            // Fine frequency
    Int  m_DecimationRate;      // Decimation rate
}  DtIoctlS2CrDemodCmd_2132GetFreqOffsetOutput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetFreqOffsetOutput, 12)

//.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlS2CrDemodCmd_2132GetOpModeInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetOpModeInput, 16)
typedef struct _DtIoctlS2CrDemodCmd_2132GetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlS2CrDemodCmd_2132GetOpModeOutput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetOpModeOutput, 4)

//-.-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Get PL information -.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlS2CrDemodCmd_2132GetPlInformationInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetPlInformationInput, 16)
typedef struct _DtIoctlS2CrDemodCmd_2132GetPlInformationOutput
{
    Int  m_PlsValue;           // Pls value
    Int  m_PlMode;             // ACM/VCM/CCM mode
}  DtIoctlS2CrDemodCmd_2132GetPlInformationOutput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetPlInformationOutput, 8)

//.-.-.-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Get Roll Off -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlS2CrDemodCmd_2132GetRollOffInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetRollOffInput, 16)
typedef struct _DtIoctlS2CrDemodCmd_2132GetRollOffOutput
{
    Int  m_RollOff;         // Roll off
}  DtIoctlS2CrDemodCmd_2132GetRollOffOutput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetRollOffOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Get SNR Info -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlS2CrDemodCmd_2132GetSnrInfoInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetSnrInfoInput, 16)
typedef struct _DtIoctlS2CrDemodCmd_2132GetSnrInfoOutput
{
    UInt  m_SignalPower;    // Signal power
    UInt  m_ErrorPower;     // Error power
}  DtIoctlS2CrDemodCmd_2132GetSnrInfoOutput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetSnrInfoOutput, 8)

//-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Get Spectrum Inversion -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlS2CrDemodCmd_2132GetSpectrumInvInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetSpectrumInvInput, 16)
typedef struct _DtIoctlS2CrDemodCmd_2132GetSpectrumInvOutput
{
    Int  m_SpectrumInv;         // Spectrum inversion
}  DtIoctlS2CrDemodCmd_2132GetSpectrumInvOutput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetSpectrumInvOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Get Status -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlS2CrDemodCmd_2132GetStatusInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetStatusInput, 16)
typedef struct _DtIoctlS2CrDemodCmd_2132GetStatusOutput
{
    Int  m_SignalDetected;     // Signal detected (TRUE/FALSE)
    Int  m_SpectrumInverted;   // Spectrum inverted (TRUE/FALSE)
    Int  m_PlHeaderSync;       // PL-header in sync (TRUE/FALSE)
    UInt  m_PlHeaderResyncCount;    // PL-header resync counter
}  DtIoctlS2CrDemodCmd_2132GetStatusOutput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetStatusOutput, 16)

//-.-.-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Get SymbolRate -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlS2CrDemodCmd_2132GetSymbolRateInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetSymbolRateInput, 16)
typedef struct _DtIoctlS2CrDemodCmd_2132GetSymbolRateOutput
{
    Int  m_DecRate;         // Decimation rate
    Int  m_SamSymRatio;     // Sample to symbol rate ratio
    Int  m_SymSamRatio;     // Symbol rate to Sample rate ratio
    Int  m_MeasSamSymRatio; // Measured sample to symbol rate ratio
}  DtIoctlS2CrDemodCmd_2132GetSymbolRateOutput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132GetSymbolRateOutput, 16)

// -.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlS2CrDemodCmd_2132SetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlS2CrDemodCmd_2132SetOpModeInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132SetOpModeInput, 20)

//-.-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Set  PL information -.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlS2CrDemodCmd_2132SetPlInformationInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_PlMode;             // CCM/ACM/VCM
    Int  m_PlsValue;           // Pls value
}DtIoctlS2CrDemodCmd_2132SetPlInformationInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132SetPlInformationInput, 24)

//-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Set Spectrum Inversion -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlS2CrDemodCmd_2132SetSpectrumInvInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_SpectrumInv;         // Spectrum inversion
}  DtIoctlS2CrDemodCmd_2132SetSpectrumInvInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132SetSpectrumInvInput, 20)

//-.-.-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Set Symbol Rate -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlS2CrDemodCmd_2132SetSymbolRateInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_DecRate;         // Decimation rate
    Int  m_SamSymRatio;     // Sample to symbol rate ratio
    Int  m_SymSamRatio;     // Symbol rate to Sample rate ratio
}  DtIoctlS2CrDemodCmd_2132SetSymbolRateInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132SetSymbolRateInput, 28)

//-.-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - Execute Soft Reset  -.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlS2CrDemodCmd_2132SoftResetInput;
ASSERT_SIZE(DtIoctlS2CrDemodCmd_2132SoftResetInput, 16)

//.-.-.-.-.-.-.-.-.-.-.- S2CRDEMOD_2132 Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.
// S2CRDEMOD_2132 command - IOCTL input data
typedef union _DtIoctlS2CrDemodCmdInput_2132
{
    DtIoctlS2CrDemodCmd_2132DemodResetInput m_DemodReset;       // Execute a hard reset
    DtIoctlS2CrDemodCmd_2132GetCountersInput  m_GetCounters;     // Get counters
    DtIoctlS2CrDemodCmd_2132GetFreqOffsetInput  m_GetFreqOffset; // Get frequency offset
    DtIoctlS2CrDemodCmd_2132SetOpModeInput  m_SetOpMode;         // Set operational. mode
    DtIoctlS2CrDemodCmd_2132SetSpectrumInvInput  m_SetSpectrumInv;  // Set spectrum inv
    DtIoctlS2CrDemodCmd_2132SetSymbolRateInput  m_SetSymRate;    // Set symbol rate
    DtIoctlS2CrDemodCmd_2132SetPlInformationInput  m_SetPlInfo;  // Set Pl information
    DtIoctlS2CrDemodCmd_2132SoftResetInput  m_SoftReset;         // Execute a soft reset
 }  DtIoctlS2CrDemodCmdInput_2132;
// S2CRDEMOD_2132 command - IOCTL output data
typedef union _DtIoctlS2CrDemodCmdOutput_2132
{
    DtIoctlS2CrDemodCmd_2132GetCountersOutput  m_GetCounters;     // Get counters
    DtIoctlS2CrDemodCmd_2132GetFreqOffsetOutput  m_GetFreqOffset; // Get frequency offset
    DtIoctlS2CrDemodCmd_2132GetOpModeOutput  m_GetOpMode;         // Get operational. mode
    DtIoctlS2CrDemodCmd_2132GetPlInformationOutput  m_GetPlInfo;  // Get Pl information
    DtIoctlS2CrDemodCmd_2132GetRollOffOutput  m_GetRollOff;       // Get roll off
    DtIoctlS2CrDemodCmd_2132GetSnrInfoOutput  m_GetSnrInfo;       // Get SNR info
    DtIoctlS2CrDemodCmd_2132GetSpectrumInvOutput  m_GetSpectrumInv; // Get spectrum inv
    DtIoctlS2CrDemodCmd_2132GetStatusOutput  m_GetStatus;         // Get status
    DtIoctlS2CrDemodCmd_2132GetSymbolRateOutput  m_GetSymRate;    // Get symbol rate
}  DtIoctlS2CrDemodCmdOutput_2132;
#ifdef WINBUILD
    #define DT_IOCTL_S2CRDEMOD_CMD_2132  CTL_CODE(DT_DEVICE_TYPE,                        \
                                                      DT_FUNC_CODE_S2CRDEMOD_CMD_2132,   \
                                                      METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlS2CrDemodCmd_2132InOut
    {
        DtIoctlS2CrDemodCmdInput_2132  m_Input;
        DtIoctlS2CrDemodCmdOutput_2132  m_Output;
    }  DtIoctlS2CrDemodCmd_2132InOut;
    #define DT_IOCTL_S2CRDEMOD_CMD_2132  _IOWR(DT_IOCTL_MAGIC_SIZE,                      \
                                                      DT_FUNC_CODE_S2CRDEMOD_CMD_2132,   \
                                                      DtIoctlS2CrDemodCmd_2132InOut)
#endif
   
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_S2STATS_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- S2STATS Interface Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlS2StatsCmd_2132
{
    DT_S2STATS_CMD_2132_CLEAR_STATISTICS = 0,
    DT_S2STATS_CMD_2132_GET_OPERATIONAL_MODE = 1,
    DT_S2STATS_CMD_2132_GET_STATISTICS_STATUS = 2,
    DT_S2STATS_CMD_2132_READ_STATISTICS = 3,
    DT_S2STATS_CMD_2132_SET_OPERATIONAL_MODE = 4,
}  DtIoctlS2StatsCmd_2132;

// GainStatus
#define DT_S2STATS_2132_ACM  0x0  // BBFrame ACM/CCM bit is clear
#define DT_S2STATS_2132_CCM  0x1  // BBFrame ACM/CCM bit is set


// DVB-S2-frame status
typedef  struct _DtS2Stats_2132_FrameStatus
{
    UInt8  m_Pls;               // PLS of frame
    UInt8  m_InputStreamId;     // Input stream ID
    UInt8  m_IsBchOk;           // BCH is OK; TRUE/FALSE
    UInt8  m_IsLdpcOk;          // LDPC is OK; TRUE/FALSE
    UInt8  m_CcmAcm;            // BBHeader CCM/ACM bit state
    Int  m_Mer;                 // MER
} DtS2Stats_2132_FrameStatus;


//-.-.-.-.-.-.-.-.-.- S2STATS_2132 Command - Clear Statistics Command -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlS2StatsCmd_2132ClearStatisticsInput;
ASSERT_SIZE(DtIoctlS2StatsCmd_2132ClearStatisticsInput, 16)

//-.-.-.-.-.-.-.-.- S2STATS_2132 Command - Get Statistics Status Command -.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlS2StatsCmd_2132GetStatisticsStatusInput;
ASSERT_SIZE(DtIoctlS2StatsCmd_2132GetStatisticsStatusInput, 16)
typedef struct _DtIoctlS2StatsCmd_2132GetStatisticsStatusOutput
{
    Int  m_BufferSize;          // Maximum number of statistics the buffer can contain
    Int  m_NumAvailable;        // Number of statistics the buffer contain
    Int  m_Overflow;            // Buffer overflow occurred FALSE/TRUE
}  DtIoctlS2StatsCmd_2132GetStatisticsStatusOutput;
ASSERT_SIZE(DtIoctlS2StatsCmd_2132GetStatisticsStatusOutput, 12)

//-.-.-.-.-.-.-.-.- S2STATS_2132 Command - Get Operational Mode Command -.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlS2StatsCmd_2132GetOpModeInput;
ASSERT_SIZE(DtIoctlS2StatsCmd_2132GetOpModeInput, 16)
typedef struct _DtIoctlS2StatsCmd_2132GetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlS2StatsCmd_2132GetOpModeOutput;
ASSERT_SIZE(DtIoctlS2StatsCmd_2132GetOpModeOutput, 4)

//.-.-.-.-.-.-.-.-.-.- S2STATS_2132 Command - Read Statistics Command -.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlS2StatsCmd_2132ReadStatisticsInput 
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_NumToRead;
} DtIoctlS2StatsCmd_2132ReadStatisticsInput;
ASSERT_SIZE(DtIoctlS2StatsCmd_2132ReadStatisticsInput, 20)

typedef struct _DtIoctlS2StatsCmd_2132ReadStatisticsOutput {
    Int  m_NumRead;
    DtS2Stats_2132_FrameStatus  m_Buf[0];  // Dynamic sized buffer
} DtIoctlS2StatsCmd_2132ReadStatisticsOutput;
ASSERT_SIZE(DtIoctlS2StatsCmd_2132ReadStatisticsOutput, 4)

//-.-.-.-.-.-.-.-.- S2STATS_2132 Command - Set  Operational Mode Command -.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlS2StatsCmd_2132SetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlS2StatsCmd_2132SetOpModeInput;
ASSERT_SIZE(DtIoctlS2StatsCmd_2132SetOpModeInput, 20)

//-.-.-.-.-.-.-.-.-.-.-.- S2STATS_2132 Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-
// S2STATS_2132 command - IOCTL input data
typedef union _DtIoctlS2StatsCmdInput_2132
{
    DtIoctlS2StatsCmd_2132ReadStatisticsInput  m_ReadStats; // Read statistics
    DtIoctlS2StatsCmd_2132SetOpModeInput  m_SetOpMode;      // Set operatinal mode
}  DtIoctlS2StatsCmdInput_2132;
// S2STATS_2132 command - IOCTL output data
typedef union _DtIoctlS2StatsCmdOutput_2132
{
    DtIoctlS2StatsCmd_2132GetStatisticsStatusOutput  m_GetStatus; // Get statistics status
    DtIoctlS2StatsCmd_2132GetOpModeOutput  m_GetOpMode;      // Get operatinal mode
    DtIoctlS2StatsCmd_2132ReadStatisticsOutput  m_ReadStats; // Read statistics
}  DtIoctlS2StatsCmdOutput_2132;
#ifdef WINBUILD
    #define DT_IOCTL_S2STATS_CMD_2132  CTL_CODE(DT_DEVICE_TYPE,                          \
                                                        DT_FUNC_CODE_S2STATS_CMD_2132,   \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlS2StatsCmd_2132InOut
    {
        DtIoctlS2StatsCmdInput_2132  m_Input;
        DtIoctlS2StatsCmdOutput_2132  m_Output;
    }  DtIoctlS2StatsCmd_2132InOut;
    #define DT_IOCTL_S2STATS_CMD_2132  _IOWR(DT_IOCTL_MAGIC_SIZE,                        \
                                                        DT_FUNC_CODE_S2STATS_CMD_2132,   \
                                                        DtIoctlS2StatsCmd_2132InOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+= DT_IOCTL_CONSTSINK_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-. CONSTSINK commands .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE: ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
// NOTE: PUT NEW SETS OF COMMANDS IN ALPHABETICAL ORDER WITHOUT DISTURBING OLD COMMANDS.
typedef enum _DtIoctlConstSinkCmd
{
    DT_CONSTSINK_CMD_GET_CHECK_DATA = 0,
    DT_CONSTSINK_CMD_GET_DATA_ERROR_CNT = 1,
    DT_CONSTSINK_CMD_GET_DATA_PATTERN = 2,
    DT_CONSTSINK_CMD_GET_DATA_RATE = 3,
    DT_CONSTSINK_CMD_GET_OPERATIONAL_MODE = 4,
    DT_CONSTSINK_CMD_GET_TEST_INTERVAL = 5,
    DT_CONSTSINK_CMD_GET_UNDERFLOW_CNT = 6,
    DT_CONSTSINK_CMD_RUN_TEST = 7,
    DT_CONSTSINK_CMD_SET_CHECK_DATA = 8,
    DT_CONSTSINK_CMD_SET_DATA_PATTERN = 9,
    DT_CONSTSINK_CMD_SET_DATA_RATE = 10,
    DT_CONSTSINK_CMD_SET_OPERATIONAL_MODE = 11,
    DT_CONSTSINK_CMD_SET_TEST_INTERVAL = 12,
    DT_CONSTSINK_CMD_START_TEST_INTERVAL = 13,
    DT_CONSTSINK_CMD_WAIT_FOR_TEST_ITV_DONE = 14,
}  DtIoctlConstSinkCmd;

// DT CONSTSINK Data Patterns
#define DT_CONSTSINK_INCREMENTING   0x0
#define DT_CONSTSINK_PRBS31         0x1

//-.-.-.-.-.-.-.-.-.-.-.-.-. CONSTSINK Command - Get Check Data .-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlConstSinkCmdGetCheckDataInput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetCheckDataInput, 16)
typedef struct _DtIoctlConstSinkCmdGetCheckDataOutput
{
    Int  m_CheckData;           // check data enabled (bool)
}  DtIoctlConstSinkCmdGetCheckDataOutput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetCheckDataOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.- CONSTSINK Command - Get Data Error Cnt .-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlConstSinkCmdGetDataErrorCntInput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetDataErrorCntInput, 16)
typedef struct _DtIoctlConstSinkCmdGetDataErrorCntOutput
{
    UInt32  m_DataErrorCnt;     // number of mismatched sequences of block specific words
}  DtIoctlConstSinkCmdGetDataErrorCntOutput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetDataErrorCntOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.- CONSTSINK Command - Get Data Pattern -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlConstSinkCmdGetDataPatternInput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetDataPatternInput, 16)
typedef struct _DtIoctlConstSinkCmdGetDataPatternOutput
{
    Int  m_DataPattern;         // DT CONSTSINK Data Pattern
}  DtIoctlConstSinkCmdGetDataPatternOutput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetDataPatternOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.- CONSTSINK Command - Get Data Rate .-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlConstSinkCmdGetDataRateInput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetDataRateInput, 16)
typedef struct _DtIoctlConstSinkCmdGetDataRateOutput
{
    Int64A  m_DataRate;        // data rate (bps)
}  DtIoctlConstSinkCmdGetDataRateOutput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetDataRateOutput, 8)

//-.-.-.-.-.-.-.-.-.-.-.- CONSTSINK Command - Get Operational Mode -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlConstSinkCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetOpModeInput, 16)
typedef struct _DtIoctlConstSinkCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlConstSinkCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetOpModeOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.- CONSTSINK Command - Get Test Interval .-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlConstSinkCmdGetTestIntervalInput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetTestIntervalInput, 16)
typedef struct _DtIoctlConstSinkCmdGetTestIntervalOutput
{
    Int  m_TestItv;             // Test Interval (us)
}  DtIoctlConstSinkCmdGetTestIntervalOutput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetTestIntervalOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.- CONSTSINK Command - Get Underflow Cnt .-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlConstSinkCmdGetUnderflowCntInput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetUnderflowCntInput, 16)
typedef struct _DtIoctlConstSinkCmdGetUnderflowCntOutput
{
    UInt32  m_UnderflowCnt;     // bytes expected but not available during test
}  DtIoctlConstSinkCmdGetUnderflowCntOutput;
ASSERT_SIZE(DtIoctlConstSinkCmdGetUnderflowCntOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-. CONSTSINK Command - Run Test .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlConstSinkCmdRunTestInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int64A  m_DataRate;         // data rate (bps)
    Int  m_TestItv;             // Test Interval (us)
}  DtIoctlConstSinkCmdRunTestInput;
// the alignment of m_DataRate requires the entire struct to be 8 byte aligned
ASSERT_SIZE(DtIoctlConstSinkCmdRunTestInput, 32)
typedef struct _DtIoctlConstSinkCmdRunTestOutput
{
    UInt32  m_UnderflowCnt;     // bytes expected but not available during test
    UInt32  m_DataErrorCnt;     // number of mismatched sequences of block specific words
}  DtIoctlConstSinkCmdRunTestOutput;
ASSERT_SIZE(DtIoctlConstSinkCmdRunTestOutput, 8)

//-.-.-.-.-.-.-.-.-.-.-.-.-. CONSTSINK Command - Set Check Data .-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlConstSinkCmdSetCheckDataInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_CheckData;           // check data enabled (bool)
}  DtIoctlConstSinkCmdSetCheckDataInput;
ASSERT_SIZE(DtIoctlConstSinkCmdSetCheckDataInput, 20)

//-.-.-.-.-.-.-.-.-.-.-.-.- CONSTSINK Command - Set Data Pattern -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlConstSinkCmdSetDataPatternInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_DataPattern;         // DT CONSTSINK Data Pattern
}  DtIoctlConstSinkCmdSetDataPatternInput;
ASSERT_SIZE(DtIoctlConstSinkCmdSetDataPatternInput, 20)

//-.-.-.-.-.-.-.-.-.-.-.-.-. CONSTSINK Command - Set Data Rate -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlConstSinkCmdSetDataRateInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int64A  m_DataRate;         // data rate (bps)
}  DtIoctlConstSinkCmdSetDataRateInput;
ASSERT_SIZE(DtIoctlConstSinkCmdSetDataRateInput, 24)

//-.-.-.-.-.-.-.-.-.-.-.- CONSTSINK Command - Set Operational Mode -.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlConstSinkCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlConstSinkCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlConstSinkCmdSetOpModeInput, 20)

//-.-.-.-.-.-.-.-.-.-.-.-. CONSTSINK Command - Set Test Interval -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtIoctlConstSinkCmdSetTestIntervalInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_TestItv;             // Test Interval (us)
}  DtIoctlConstSinkCmdSetTestIntervalInput;
ASSERT_SIZE(DtIoctlConstSinkCmdSetTestIntervalInput, 20)

//-.-.-.-.-.-.-.-.-.-.-.- CONSTSINK Command - Start Test Interval -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlConstSinkCmdStartTestItvInput;
ASSERT_SIZE(DtIoctlConstSinkCmdStartTestItvInput, 16)

//-.-.-.-.-.-.-.-.-.- CONSTSINK Command - Wait For Test Interval Done -.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlConstSinkCmdWaitForTestItvDoneInput;
ASSERT_SIZE(DtIoctlConstSinkCmdWaitForTestItvDoneInput, 16)

//-.-.-.-.-.-.-.-.-.-.-.-. CONSTSINK Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-
// CONSTSINK command - Input data
typedef union _DtIoctlConstSinkCmdInput 
{
    DtIoctlConstSinkCmdRunTestInput  m_RunTest;
    DtIoctlConstSinkCmdSetCheckDataInput  m_SetCheckData;
    DtIoctlConstSinkCmdSetDataPatternInput  m_SetDataPattern;
    DtIoctlConstSinkCmdSetDataRateInput  m_SetDataRate;
    DtIoctlConstSinkCmdSetOpModeInput  m_SetOpMode;
    DtIoctlConstSinkCmdSetTestIntervalInput  m_SetTestInterval;
    DtIoctlConstSinkCmdStartTestItvInput  m_StartTestInterval;
    DtIoctlConstSinkCmdWaitForTestItvDoneInput  m_WaitForTestItvDone;
} DtIoctlConstSinkCmdInput;
ASSERT_SIZE(DtIoctlConstSinkCmdInput, 32)

// CONSTSINK command - Output data
typedef union _DtIoctlConstSinkCmdOutput 
{
    DtIoctlConstSinkCmdGetCheckDataOutput  m_GetCheckData;
    DtIoctlConstSinkCmdGetDataErrorCntOutput  m_GetDataErrorCnt;
    DtIoctlConstSinkCmdGetDataPatternOutput  m_GetDataPattern;
    DtIoctlConstSinkCmdGetDataRateOutput  m_GetDataRate;
    DtIoctlConstSinkCmdGetOpModeOutput  m_GetOpMode;
    DtIoctlConstSinkCmdGetTestIntervalOutput  m_GetTestInterval;
    DtIoctlConstSinkCmdGetUnderflowCntOutput  m_GetUnderflowCnt;
    DtIoctlConstSinkCmdRunTestOutput  m_RunTest;
} DtIoctlConstSinkCmdOutput;
ASSERT_SIZE(DtIoctlConstSinkCmdOutput, 8)

#ifdef WINBUILD
    #define DT_IOCTL_CONSTSINK_CMD  CTL_CODE(DT_DEVICE_TYPE, DT_FUNC_CODE_CONSTSINK_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlConstSinkCmdInOut
    {
        DtIoctlConstSinkCmdInput  m_Input;
        DtIoctlConstSinkCmdOutput  m_Output;
    }  DtIoctlConstSinkCmdInOut;

    #define DT_IOCTL_CONSTSINK_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                           \
                                                           DT_FUNC_CODE_CONSTSINK_CMD,   \
                                                           DtIoctlConstSinkCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_CONSTSOURCE_CMD =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-. CONSTSOURCE commands .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
// NOTE: PUT NEW SETS OF COMMANDS IN ALPHABETICAL ORDER WITHOUT DISTURBING OLD COMMANDS.
typedef enum _DtIoctlConstSourceCmd
{
    DT_CONSTSOURCE_CMD_GET_DATA_PATTERN = 0,
    DT_CONSTSOURCE_CMD_GET_DATA_RATE = 1,
    DT_CONSTSOURCE_CMD_GET_OPERATIONAL_MODE = 2,
    DT_CONSTSOURCE_CMD_GET_OVERFLOW_CNT = 3,
    DT_CONSTSOURCE_CMD_GET_TEST_INTERVAL = 4,
    DT_CONSTSOURCE_CMD_RUN_TEST = 5,
    DT_CONSTSOURCE_CMD_SET_DATA_PATTERN = 6,
    DT_CONSTSOURCE_CMD_SET_DATA_RATE = 7,
    DT_CONSTSOURCE_CMD_SET_OPERATIONAL_MODE = 8,
    DT_CONSTSOURCE_CMD_SET_TEST_INTERVAL = 9,
    DT_CONSTSOURCE_CMD_START_TEST_INTERVAL = 10,
    DT_CONSTSOURCE_CMD_WAIT_FOR_TEST_ITV_DONE = 11,
}  DtIoctlConstSourceCmd;

// DT CONSTSOURCE Data Patterns
#define DT_CONSTSOURCE_INCREMENTING   0x0
#define DT_CONSTSOURCE_PRBS31         0x1

//.-.-.-.-.-.-.-.-.-.-.-.- CONSTSOURCE Command - Get Data Pattern -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlConstSourceCmdGetDataPatternInput;
ASSERT_SIZE(DtIoctlConstSourceCmdGetDataPatternInput, 16)
typedef struct _DtIoctlConstSourceCmdGetDataPatternOutput
{
    Int  m_DataPattern;         // DT CONSTSOURCE Data Pattern
}  DtIoctlConstSourceCmdGetDataPatternOutput;
ASSERT_SIZE(DtIoctlConstSourceCmdGetDataPatternOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.-.- CONSTSOURCE Command - Get Data Rate .-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlConstSourceCmdGetDataRateInput;
ASSERT_SIZE(DtIoctlConstSourceCmdGetDataRateInput, 16)
typedef struct _DtIoctlConstSourceCmdGetDataRateOutput
{
    Int64A  m_DataRate;        // data rate (bps)
}  DtIoctlConstSourceCmdGetDataRateOutput;
ASSERT_SIZE(DtIoctlConstSourceCmdGetDataRateOutput, 8)

//.-.-.-.-.-.-.-.-.-.-.- CONSTSOURCE Command - Get Operational Mode .-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlConstSourceCmdGetOpModeInput;
ASSERT_SIZE(DtIoctlConstSourceCmdGetOpModeInput, 16)
typedef struct _DtIoctlConstSourceCmdGetOpModeOutput
{
    Int  m_OpMode;              // Operational mode
}  DtIoctlConstSourceCmdGetOpModeOutput;
ASSERT_SIZE(DtIoctlConstSourceCmdGetOpModeOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.- CONSTSOURCE Command - Get Overflow Cnt -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlConstSourceCmdGetOverflowCntInput;
ASSERT_SIZE(DtIoctlConstSourceCmdGetOverflowCntInput, 16)
typedef struct _DtIoctlConstSourceCmdGetOverflowCntOutput
{
    UInt32  m_OverflowCnt;      // bytes expected but not available during test
}  DtIoctlConstSourceCmdGetOverflowCntOutput;
ASSERT_SIZE(DtIoctlConstSourceCmdGetOverflowCntOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.- CONSTSOURCE Command - Get Test Interval .-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtIoctlInputDataHdr DtIoctlConstSourceCmdGetTestIntervalInput;
ASSERT_SIZE(DtIoctlConstSourceCmdGetTestIntervalInput, 16)
typedef struct _DtIoctlConstSourceCmdGetTestIntervalOutput
{
    Int  m_TestItv;             // Test Interval (us)
}  DtIoctlConstSourceCmdGetTestIntervalOutput;
ASSERT_SIZE(DtIoctlConstSourceCmdGetTestIntervalOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.-.-. CONSTSOURCE Command - Run Test .-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlConstSourceCmdRunTestInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int64A  m_DataRate;         // data rate (bps)
    Int  m_TestItv;             // Test Interval (us)
}  DtIoctlConstSourceCmdRunTestInput;
// the alignment of m_DataRate requires the entire struct to be 8 byte aligned
ASSERT_SIZE(DtIoctlConstSourceCmdRunTestInput, 32)
typedef struct _DtIoctlConstSourceCmdRunTestOutput
{
    UInt32  m_OverflowCnt;      // bytes available but not transmitted during test
}  DtIoctlConstSourceCmdRunTestOutput;
ASSERT_SIZE(DtIoctlConstSourceCmdRunTestOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.-. CONSTSOURCE Command - Set Check Data .-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlConstSourceCmdSetCheckDataInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_CheckData;           // check data enabled (bool)
}  DtIoctlConstSourceCmdSetCheckDataInput;
ASSERT_SIZE(DtIoctlConstSourceCmdSetCheckDataInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.- CONSTSOURCE Command - Set Data Pattern -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlConstSourceCmdSetDataPatternInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_DataPattern;         // DT CONSTSOURCE Data Pattern
}  DtIoctlConstSourceCmdSetDataPatternInput;
ASSERT_SIZE(DtIoctlConstSourceCmdSetDataPatternInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-.-. CONSTSOURCE Command - Set Data Rate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlConstSourceCmdSetDataRateInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int64A  m_DataRate;         // data rate (bps)
}  DtIoctlConstSourceCmdSetDataRateInput;
ASSERT_SIZE(DtIoctlConstSourceCmdSetDataRateInput, 24)

//.-.-.-.-.-.-.-.-.-.-.- CONSTSOURCE Command - Set Operational Mode -.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlConstSourceCmdSetOpModeInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_OpMode;              // Operational mode
}  DtIoctlConstSourceCmdSetOpModeInput;
ASSERT_SIZE(DtIoctlConstSourceCmdSetOpModeInput, 20)

//.-.-.-.-.-.-.-.-.-.-.-. CONSTSOURCE Command - Set Test Interval -.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtIoctlConstSourceCmdSetTestIntervalInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
    Int  m_TestItv;             // Test Interval (us)
}  DtIoctlConstSourceCmdSetTestIntervalInput;
ASSERT_SIZE(DtIoctlConstSourceCmdSetTestIntervalInput, 20)

//.-.-.-.-.-.-.-.-.-.-.- CONSTSOURCE Command - Start Test Interval -.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlConstSourceCmdStartTestItvInput;
ASSERT_SIZE(DtIoctlConstSourceCmdStartTestItvInput, 16)

//.-.-.-.-.-.-.-.-.- CONSTSOURCE Command - Wait For Test Interval Done -.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlConstSourceCmdWaitForTestItvDoneInput;
ASSERT_SIZE(DtIoctlConstSourceCmdWaitForTestItvDoneInput, 16)

//.-.-.-.-.-.-.-.-.-.-.-. CONSTSOURCE Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.
// CONSTSOURCE command - Input data
typedef union _DtIoctlConstSourceCmdInput 
{
    DtIoctlConstSourceCmdRunTestInput  m_RunTest;
    DtIoctlConstSourceCmdSetDataPatternInput  m_SetDataPattern;
    DtIoctlConstSourceCmdSetDataRateInput  m_SetDataRate;
    DtIoctlConstSourceCmdSetOpModeInput  m_SetOpMode;
    DtIoctlConstSourceCmdSetTestIntervalInput  m_SetTestInterval;
    DtIoctlConstSourceCmdStartTestItvInput  m_StartTestInterval;
    DtIoctlConstSourceCmdWaitForTestItvDoneInput  m_WaitForTestItvDone;
} DtIoctlConstSourceCmdInput;
ASSERT_SIZE(DtIoctlConstSourceCmdInput, 32)

// CONSTSOURCE command - Output data
typedef union _DtIoctlConstSourceCmdOutput 
{
    DtIoctlConstSourceCmdGetDataPatternOutput  m_GetDataPattern;
    DtIoctlConstSourceCmdGetDataRateOutput  m_GetDataRate;
    DtIoctlConstSourceCmdGetOpModeOutput  m_GetOpMode;
    DtIoctlConstSourceCmdGetOverflowCntOutput  m_GetOverflowCnt;
    DtIoctlConstSourceCmdGetTestIntervalOutput  m_GetTestInterval;
    DtIoctlConstSourceCmdRunTestOutput  m_RunTest;
} DtIoctlConstSourceCmdOutput;
ASSERT_SIZE(DtIoctlConstSourceCmdOutput, 8)

#ifdef WINBUILD
    #define DT_IOCTL_CONSTSOURCE_CMD  CTL_CODE(DT_DEVICE_TYPE,                           \
                                                        DT_FUNC_CODE_CONSTSOURCE_CMD,    \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtIoctlConstSourceCmdInOut
    {
        DtIoctlConstSourceCmdInput  m_Input;
        DtIoctlConstSourceCmdOutput  m_Output;
    }  DtIoctlConstSourceCmdInOut;

    #define DT_IOCTL_CONSTSOURCE_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                         \
                                                         DT_FUNC_CODE_CONSTSOURCE_CMD,   \
                                                         DtIoctlConstSourceCmdInOut)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IOCTL INPUT/OUTPUT DATA +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Common - command input data
#define DT_IOCTL_INPUT_DATA_COMMON_DATA                                                  \
    DtIoctlInputDataHdr  m_CmdHdr;                                                       \
    DtIoctlAccuFifoCmdInput  m_AccuFifoCmd;                                              \
    DtIoctlAd9789ItfCmdInput  m_Ad9789ItfCmd;                                            \
    DtIoctlAsiRxCmdInput  m_AsiTsRxCmd;                                                  \
    DtIoctlAsiTxGCmdInput  m_AsiTxGCmd;                                                  \
    DtIoctlAsiTxSerCmdInput  m_AsiTxSerCmd;                                              \
    DtIoctlBurstFifoCmdInput  m_BurstFifoCmd;                                            \
    DtIoctlDataCCmdInput  m_DataCCmd;                                                    \
    DtIoctlDataCdcCmdInput  m_DataCdcCmd;                                                \
    DtIoctlDataFifoCmdInput m_DataFifoCmd;                                               \
    DtIoctlDdrFifoCmdInput m_DdrFifoCmd;                                                 \
    DtIoctlCDmaCCmdInput  m_CDmaCCmd;                                                    \
    DtIoctlChSdiRxCmdInput  m_ChSdiRxCmd;                                                \
    DtIoctlChSdiRxPhyOnlyCmdInput  m_ChSdiRxPhyOnlyCmd;                                  \
    DtIoctlDebugCmdInput  m_DebugCmd;                                                    \
    DtIoctlDiseqcCmdInput  m_DiseqcCmd;                                                  \
    DtIoctlExclAccessCmdInput  m_ExclAccessCmd;                                          \
    DtIoctlEventCmdInput  m_Event;                                                       \
    DtIoctlGenLockCtrlCmdInput  m_GenLockCtrlCmd;                                        \
    DtIoctlGetDriverVersionInput  m_GetDriverVersion;                                    \
    DtIoctlGetDevInfoInput  m_GetDevInfo;                                                \
    DtIoctlGpsTimeCmdInput  m_GpsTimeCmd;                                                \
    DtIoctlI2cMCmdInput  m_I2cMCmd;                                                      \
    DtIoctlIpSecGCmdInput  m_IpSecGCmd;                                                  \
    DtIoctlIoConfigCmdInput  m_IoConfig;                                                 \
    DtIoctlIoParInCmdInput  m_IoParInCmd;                                                \
    DtIoctlIoSerInCmdInput  m_IoSerInCmd;                                                \
    DtIoctlIqAgcCmdInput  m_IqAgcCmd;                                                    \
    DtIoctlIqDemodCmdInput  m_IqDemodCmd;                                                \
    DtIoctlIqDownSrcCmdInput  m_IqDownSrcCmd;                                            \
    DtIoctlIqFirCmdInput  m_IqFirCmd;                                                    \
    DtIoctlIqFir2xClkCmdInput  m_IqFir2xClkCmd;                                          \
    DtIoctlIqInvCmdInput  m_IqInvCmd;                                                    \
    DtIoctlIqNoiseCmdInput  m_IqNoiseCmd;                                                \
    DtIoctlIqMiscCmdInput  m_IqMiscCmd;                                                  \
    DtIoctlIqPwrCmdInput  m_IqPwrCmd;                                                    \
    DtIoctlIqSyncCmdInput  m_IqSyncCmd;                                                  \
    DtIoctlIqUnpckCmdInput  m_IqUnpckCmd;                                                \
    DtIoctlIqUpCmdInput  m_IqUpCmd;                                                      \
    DtIoctlIqJesdCmdInput  m_IqJesdCmd;                                                  \
    DtIoctlAttenuatorsCtrlCmdInput_2116  m_AttenuatorsCtrlCmd_2116;                      \
    DtIoctlIoCmdInput_2116  m_GpioCmd_2116;                                              \
    DtIoctlPropCmdInput  m_PropCmd;                                                      \
    DtIoctlKaCmdInput  m_KaCmd;                                                          \
    DtIoctlLedBCmdInput  m_LedBCmd;                                                      \
    DtIoctlRebootCmdInput  m_Reboot;                                                     \
    DtIoctlS12Gto3GCmdInput  m_S12Gto3GCmd;                                              \
    DtIoctlSdiXCfgCmdInput  m_SdiXCfgCmd;                                                \
    DtIoctlSdiDmx12GCmdInput  m_SdiDmx12GCmd;                                            \
    DtIoctlSdiMux12GCmdInput  m_SdiMux12GCmd;                                            \
    DtIoctlSdiRxCmdInput  m_SdiRxCmd;                                                    \
    DtIoctlSdiRxPCmdInput  m_SdiRxPCmd;                                                  \
    DtIoctlSdiRxFCmdInput  m_SdiRxFCmd;                                                  \
    DtIoctlSdiTxPhyCmdInput  m_SdiTxPhyCmd;                                              \
    DtIoctlSdiTxPCmdInput  m_SdiTxPCmd;                                                  \
    DtIoctlSdiTxFCmdInput  m_SdiTxFCmd;                                                  \
    DtIoctlSensTempCmdInput  m_SensTempCmd;                                              \
    DtIoctlSi2166ItfCmdInput  m_Si2166ItfCmd;                                            \
    DtIoctlSpiMCmdInput  m_SpiMCmd;                                                      \
    DtIoctlSpiMfCmdInput  m_SpiMfCmd;                                                    \
    DtIoctlSpiPromCmdInput  m_SpiPromCmd;                                                \
    DtIoctlSt425LrCmdInput  m_St425LrCmd;                                                \
    DtIoctlSwitchCmdInput  m_SwitchCmd;                                                  \
    DtIoctlTempFanMgrCmdInput  m_TempFanMgrCmd;                                          \
    DtIoctlTodCmdInput  m_TodCmd;                                                        \
    DtIoctlTodClockCtrlCmdInput  m_TodClkCtrlCmd;                                        \
    DtIoctlTsRxFmtCmdInput  m_TsRxFmtCmd;                                                \
    DtIoctlVpdCmdInput  m_VpdCmd;                                                        \
    DtIoctlAd5320CmdInput_2132  m_Ad5320Cmd_2132;                                        \
    DtIoctlAds4246CmdInput_2132  m_Ads4246Cmd_2132;                                      \
    DtIoctlDataStMuxCmdInput_2132  m_DataStMuxCmd_2132;                                  \
    DtIoctlIqCGrabCmdInput_2132  m_IqCGrabCmd_2132;                                      \
    DtIoctlIqFirCmdInput_2132  m_IqFirCmd_2132;                                          \
    DtIoctlIqPwrCmdInput_2132  m_IqPwrCmd_2132;                                          \
    DtIoctlIqSrc2132CmdInput_2132  m_IqSrc2132Cmd_2132;                                  \
    DtIoctlLnbh25CmdInput_2132  m_Lnbh25Cmd_2132;                                        \
    DtIoctlS2StatsCmdInput_2132  m_S2StatsCmd_2132;                                      \
    DtIoctlS2CrDemodCmdInput_2132  m_S2CrDemodCmd_2132;                                  \
    DtIoctlConstSinkCmdInput  m_ConstSinkCmd;                                            \
    DtIoctlConstSourceCmdInput  m_ConstSourceCmd;                                        \
    DtIoctlEMACCmdInput  m_EMACCmd;                                                      \
    DtIoctlNwCmdInput  m_NwCmd;                                                          \
    DtIoctlPipeCmdInput  m_PipeCmd;                                                      \
    DtIoctlTrigEvtCmdInput  m_TrigEvtCmd;                                                \
    DtIoctlGetNwDriverVersionInput  m_GetNwDriverVersion;                                \
    DtIoctlSetNwDriverVersionInput  m_SetNwDriverVersion;                                \
    DtIoctlDdrFrontCmdInput  m_DdrFrontCmd

// The union
typedef union _DtIoctlInputData 
{
    DT_IOCTL_INPUT_DATA_COMMON_DATA;
} DtIoctlInputData;

// Common - command output data
#define DT_IOCTL_OUTPUT_DATA_COMMON_DATA                                                 \
    DtIoctlAccuFifoCmdOutput  m_AccuFifoCmd;                                             \
    DtIoctlAd9789ItfCmdOutput  m_Ad9789ItfCmd;                                           \
    DtIoctlAsiRxCmdOutput  m_AsiTsRxCmd;                                                 \
    DtIoctlAsiTxGCmdOutput  m_AsiTxGCmd;                                                 \
    DtIoctlAsiTxSerCmdOutput  m_AsiTxSerCmd;                                             \
    DtIoctlBurstFifoCmdOutput  m_BurstFifoCmd;                                           \
    DtIoctlDataCCmdOutput  m_DataCCmd;                                                   \
    DtIoctlDataCdcCmdOutput  m_DataCdcCmd;                                               \
    DtIoctlDataFifoCmdOutput  m_DataFifoCmd;                                             \
    DtIoctlDdrFifoCmdOutput  m_DdrFifoCmd;                                               \
    DtIoctlCDmaCCmdOutput  m_CDmaCCmd;                                                   \
    DtIoctlChSdiRxCmdOutput  m_ChSdiRxCmd;                                               \
    DtIoctlChSdiRxPhyOnlyCmdOutput  m_ChSdiRxPhyOnlyCmd;                                 \
    DtIoctlDebugCmdOutput  m_DebugCmd;                                                   \
    DtIoctlDiseqcCmdOutput  m_DiseqcCmd;                                                 \
    DtIoctlEventCmdOutput  m_Event;                                                      \
    DtIoctlGenLockCtrlCmdOutput  m_GenLockCtrlCmd;                                       \
    DtIoctlGetDriverVersionOutput  m_GetDriverVersion;                                   \
    DtIoctlGetDevInfoOutput  m_GetDevInfo;                                               \
    DtIoctlGpsTimeCmdOutput  m_GpsTimeCmd;                                               \
    DtIoctlI2cMCmdOutput  m_I2cMCmd;                                                     \
    DtIoctlIpSecGCmdOutput  m_IpSecGCmd;                                                 \
    DtIoctlIoConfigCmdOutput  m_IoConfig;                                                \
    DtIoctlIoParInCmdOutput  m_IoParInCmd;                                               \
    DtIoctlIoSerInCmdOutput  m_IoSerInCmd;                                               \
    DtIoctlKaCmdOutput  m_KaCmd;                                                         \
    DtIoctlIqAgcCmdOutput  m_IqAgcCmd;                                                   \
    DtIoctlIqDemodCmdOutput  m_IqDemodCmd;                                               \
    DtIoctlIqDownSrcCmdOutput  m_IqDownSrcCmd;                                           \
    DtIoctlIqFirCmdOutput  m_IqFirCmd;                                                   \
    DtIoctlIqFir2xClkCmdOutput  m_IqFir2xClkCmd;                                         \
    DtIoctlIqInvCmdOutput  m_IqInvCmd;                                                   \
    DtIoctlIqNoiseCmdOutput  m_IqNoiseCmd;                                               \
    DtIoctlIqMiscCmdOutput  m_IqMiscCmd;                                                 \
    DtIoctlIqPwrCmdOutput  m_IqPwrCmd;                                                   \
    DtIoctlIqSyncCmdOutput  m_IqSyncCmd;                                                 \
    DtIoctlIqUnpckCmdOutput  m_IqUnpckCmd;                                               \
    DtIoctlIqUpCmdOutput  m_IqUpCmd;                                                     \
    DtIoctlIqJesdCmdOutput  m_IqJesdCmd;                                                 \
    DtIoctlAttenuatorsCtrlCmdOutput_2116  m_AttenuatorsCtrlCmd_2116;                     \
    DtIoctlIoCmdOutput_2116  m_GpioCmd_2116;                                             \
    DtIoctlLedBCmdOutput  m_LedBCmd;                                                     \
    DtIoctlLmh1981CmdOutput  m_Lmh1981Cmd;                                               \
    DtIoctlPropCmdOutput  m_PropCmd;                                                     \
    DtIoctlS12Gto3GCmdOutput  m_S12Gto3GCmd;                                             \
    DtIoctlSdiDmx12GCmdOutput  m_SdiDmx12GCmd;                                           \
    DtIoctlSdiMux12GCmdOutput  m_SdiMux12GCmd;                                           \
    DtIoctlSdiXCfgCmdOutput  m_SdiXCfgCmd;                                               \
    DtIoctlSdiRxCmdOutput  m_SdiRxCmd;                                                   \
    DtIoctlSdiRxPCmdOutput  m_SdiRxPCmd;                                                 \
    DtIoctlSdiRxFCmdOutput  m_SdiRxFCmd;                                                 \
    DtIoctlSdiTxPhyCmdOutput  m_SdiTxPhyCmd;                                             \
    DtIoctlSdiTxPCmdOutput  m_SdiTxPCmd;                                                 \
    DtIoctlSdiTxFCmdOutput  m_SdiTxFCmd;                                                 \
    DtIoctlSensTempCmdOutput  m_SensTempCmd;                                             \
    DtIoctlSi2166ItfCmdOutput  m_Si2166ItfCmd;                                           \
    DtIoctlSpiMCmdOutput  m_SpiMCmd;                                                     \
    DtIoctlSpiMfCmdOutput  m_SpiMfCmd;                                                   \
    DtIoctlSpiPromCmdOutput  m_SpiPromCmd;                                               \
    DtIoctlSt425LrCmdOutput  m_St425LrCmd;                                               \
    DtIoctlSwitchCmdOutput  m_SwitchCmd;                                                 \
    DtIoctlTempFanMgrCmdOutput  m_TempFanMgrCmd;                                         \
    DtIoctlTodCmdOutput  m_TodCmd;                                                       \
    DtIoctlTodClockCtrlCmdOutput  m_TodClkCtrlCmd;                                       \
    DtIoctlTsRxFmtCmdOutput  m_TsRxFmtCmd;                                               \
    DtIoctlVpdCmdOutput  m_VpdCmd;                                                       \
    DtIoctlAd5320CmdOutput_2132  m_Ad5320Cmd_2132;                                       \
    DtIoctlAds4246CmdOutput_2132  m_Ads4246Cmd_2132;                                     \
    DtIoctlDataStMuxCmdOutput_2132  m_DataStMuxCmd_2132;                                 \
    DtIoctlIqCGrabCmdOutput_2132  m_IqCGrabCmd_2132;                                     \
    DtIoctlIqFirCmdOutput_2132  m_IqFirCmd_2132;                                         \
    DtIoctlIqPwrCmdOutput_2132  m_IqPwrCmd_2132;                                         \
    DtIoctlIqSrc2132CmdOutput_2132  m_IqSrc2132Cmd_2132;                                 \
    DtIoctlLnbh25CmdOutput_2132  m_Lnbh25Cmd_2132;                                       \
    DtIoctlS2StatsCmdOutput_2132  m_S2StatsCmd_2132;                                     \
    DtIoctlS2CrDemodCmdOutput_2132  m_S2CrDemodCmd_2132;                                 \
    DtIoctlConstSinkCmdOutput  m_ConstSinkCmd;                                           \
    DtIoctlConstSourceCmdOutput  m_ConstSourceCmd;                                       \
    DtIoctlEMACCmdOutput  m_EMACCmd;                                                     \
    DtIoctlNwCmdOutput  m_NwCmd;                                                         \
    DtIoctlPipeCmdOutput  m_PipeCmd;                                                     \
    DtIoctlGetNwDriverVersionOutput  m_GetNwDriverVersion;                               \
    DtIoctlDdrFrontCmdOutput  m_DdrFrontCmd

// The union
typedef union _DtIoctlOutputData 
{
    DT_IOCTL_OUTPUT_DATA_COMMON_DATA;
} DtIoctlOutputData;

typedef union _DtIoctlData {
    DtIoctlInputData  m_Input;
    DtIoctlOutputData  m_Output;
} DtIoctlData;

#undef Int64
#undef UInt64

#endif  // __DT_COMMON_H
