//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCommon.h *#*#*#*#*#*#*#*#* (C) 2010-2017 DekTec
//
// SDK - Common definitions and types between for DTAPI/DTA/DTPCIE/DTU drivers
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2017 DekTec Digital Video B.V.
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
    // Use nativer version when available.
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
}  DtBcType;

// Block operational modes
#define DT_BLOCK_OPMODE_IDLE        0   // Idle
#define DT_BLOCK_OPMODE_STANDBY     1   // Standby
#define DT_BLOCK_OPMODE_RUN         2   // Run

// Block operational status
#define DT_BLOCK_OPSTATUS_IDLE      0   // Idle
#define DT_BLOCK_OPSTATUS_STANDBY   1   // Standby
#define DT_BLOCK_OPSTATUS_RUN       2   // Run
#define DT_BLOCK_OPSTATUS_ERROR     3   // Error state

// Function operational modes
#define DT_FUNC_OPMODE_IDLE         0   // Idle
#define DT_FUNC_OPMODE_STANDBY      1   // Standby
#define DT_FUNC_OPMODE_RUN          2   // Run

// Function operational status
#define DT_FUNC_OPSTATUS_IDLE      0   // Idle
#define DT_FUNC_OPSTATUS_STANDBY   1   // Standby
#define DT_FUNC_OPSTATUS_RUN       2   // Run
#define DT_FUNC_OPSTATUS_ERROR     3   // Error state

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
}  DtPortType;


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
    Int  m_CurFree;     // Burst current fifo free space
    Int  m_CurLoad;     // Burst current fifo load
    Int  m_MaxFree;     // Burst fifo maximum free space
    Int  m_MaxLoad;     // Burst fifo maximum load
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
    Int  m_DataWidth;           // Data width in bits
    Int  m_BurstFifoSize;       // Burst fifo size in bytes
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
    Int  m_BufferSize;      // Allocated buffer size
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
    Int  m_ReorderBufLoad;          // Reorder buffer load
    Int  m_ReorderBufMinMaxLoad;    // Reorder buffer minimum/maximum load
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
    Int  m_PcieDataWidth;       // PCIe interface data width
    Int  m_ReorderBufSize;       // Burst fifo size
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

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_DEBUG_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Debug Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: ALWAYS ADD NEW CMDs TO END FOR BACKWARDS COMPATIBILITY. # SEQUENTIAL START AT 0
typedef enum _DtIoctlDebugCmd
{
    DT_DEBUG_CMD_BULK_READ = 0,
    DT_DEBUG_CMD_REGISTER_READ = 1,
    DT_DEBUG_CMD_REGISTER_WRITE = 2,
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

//.-.-.-.-.-.-.-.-.-.-.-.-.- Debug Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Debug command - IOCTL input data
typedef union _DtIoctlDebugCmdInput
{
    DtIoctlDebugCmdBulkReadInput  m_BulkRead;       // Debug - Bulk read command
    DtIoctlDebugCmdRegReadInput  m_RegRead;         // Debug - Read register command
    DtIoctlDebugCmdRegWriteInput  m_RegWrite;       // Debug - Write register command
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
    DT_GENLOCKCTRL_CMD_GET_STATUS        = 0,  // Get status
    DT_GENLOCKCTRL_CMD_RELOCK            = 1,  // Re-lock
    // UNUSED DT_GENLOCKCTRL_CMD_GET_DCO_FREQ_OFFSET    = 2,
    // UNUSED DT_GENLOCKCTRL_CMD_SET_DCO_FREQ_OFFSET    = 3,
    DT_GENLOCKCTRL_CMD_GET_DCO_CLK_PROPS    = 4,  // Get DCO clock propties
    DT_GENLOCKCTRL_CMD_GET_DCO_FREQ_OFFSET  = 5,  // Get DCO frequency offset
    DT_GENLOCKCTRL_CMD_SET_DCO_FREQ_OFFSET  = 6,  // Set DCO frequency offset
}  DtIoctlGenLockCtrlCmd;

// GenLock states
#define  DT_GENLOCKCTRL_STATUS_NO_REF        0
#define  DT_GENLOCKCTRL_STATUS_INVALID_REF   1
#define  DT_GENLOCKCTRL_STATUS_LOCKING       2
#define  DT_GENLOCKCTRL_STATUS_LOCKED        3
#define  DT_GENLOCKCTRL_STATUS_FREE_RUN      4

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


// .-.-.-.-.-.-.-.-.-.-.-.-.- GENLOCKCTRL - Get Status Command -.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtIoctlGenLockCtrlCmdGetStatusInput
{
    DtIoctlInputDataHdr  m_CmdHdr;
}  DtIoctlGenLockCtrlCmdGetStatusInput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdGetStatusInput, 16)

typedef struct _DtIoctlGenLockCtrlCmdGetStatusOutput
{
    Int  m_GenLockStatus;   // GenLock status
    Int  m_RefVidStd;       // Reference video standard
    Int  m_DetVidStd;       // Detected video standard
}  DtIoctlGenLockCtrlCmdGetStatusOutput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdGetStatusOutput, 12)

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
    DtIoctlGenLockCtrlCmdGetStatusOutput  m_GetStatus;
    DtIoctlGenLockCtrlCmdGetDcoClockPropsOutput  m_GetDcoClkProps;
    DtIoctlGenLockCtrlCmdGetDcoFreqOffsetOutput  m_GetDcoFreqOffset;
}  DtIoctlGenLockCtrlCmdOutput;
ASSERT_SIZE(DtIoctlGenLockCtrlCmdOutput, 16)


#ifdef WINBUILD
    #define DT_IOCTL_GENLOCKCTRL_CMD  CTL_CODE(DT_DEVICE_TYPE,                           \
                                                        DT_FUNC_CODE_GENLOCKCTRL_CMD,    \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGenLockCtrlCmdInOut {
        DtIoctlGenLockCtrlCmdInput  m_Input;
        DtIoctlGenLockCtrlCmdOutput  m_Output;
    } DtaIoctlGenLockCtrlCmdInOut;

    #define DT_IOCTL_GENLOCKCTRL_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                         \
                                                         DT_FUNC_CODE_GENLOCKCTRL_CMD,   \
                                                         DtaIoctlGenLockCtrlCmdInOut)
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
    typedef union _DtaIoctlI2cMCmdInOut {
        DtIoctlI2cMCmdInput  m_Input;
        DtIoctlI2cMCmdOutput  m_Output;
    } DtaIoctlI2cMCmdInOut;

    #define DT_IOCTL_I2CM_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_I2CM_CMD,         \
                                                                     DtaIoctlI2cMCmdInOut)
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
    typedef union _DtaIoctlIpSecGCmdInOut {
        DtIoctlIpSecGCmdInput  m_Input;
        DtIoctlIpSecGCmdOutput  m_Output;
    } DtaIoctlIpSecGCmdInOut;

    #define DT_IOCTL_IPSECG_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_IPSECG_CMD,     \
                                                                   DtaIoctlIpSecGCmdInOut)
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
    typedef union _DtaIoctlSensTempCmdInOut
    {
        DtIoctlSensTempCmdInput  m_Input;
        DtIoctlSensTempCmdOutput  m_Output;
    } DtaIoctlSensTempCmdInOut;

    #define DT_IOCTL_SENSTEMP_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SENSTEMP_CMD, \
                                                                 DtaIoctlSensTempCmdInOut)
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

// SPIM duplex Mode
#define DT_SPIM_DPX_FULL_DUPLEX  0x0    // Send and receive simultaneously
#define DT_SPIM_DPX_HALF_DUPLEX  0x1    // Send or receivee

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- SPIM - Get Properties Command -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef DtIoctlInputDataHdr DtIoctlSpiMCmdGetPropertiesInput;
ASSERT_SIZE(DtIoctlSpiMCmdGetPropertiesInput, 16)

typedef struct _DtIoctlSpiMCmdGetPropertiesOutput 
{
    Int  m_SpiDeviceId;             // Device ID
    Int  m_SpiClockRate;            // SPI clock rate in
    Int  m_WordSize;                // SPI word size in number of bits
    Int  m_DuplexMode;              // Duplex mode: full or full duplex
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
    typedef union _DtaIoctlSpiMCmdInOut {
        DtIoctlSpiMCmdInput  m_Input;
        DtIoctlSpiMCmdOutput  m_Output;
    } DtaIoctlSpiMCmdInOut;

    #define DT_IOCTL_SPIM_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SPIM_CMD,         \
                                                                     DtaIoctlSpiMCmdInOut)
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
    typedef union _DtaIoctlSpiMfCmdInOut
    {
        DtIoctlSpiMfCmdInput  m_Input;
        DtIoctlSpiMfCmdOutput  m_Output;
    } DtaIoctlSpiMfCmdInOut;

    #define DT_IOCTL_SPIMF_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SPIMF_CMD,       \
                                                                    DtaIoctlSpiMfCmdInOut)
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
    typedef union _DtaIoctlSpiPromCmdInOut
    {
        DtIoctlSpiPromCmdInput  m_Input;
        DtIoctlSpiPromCmdOutput  m_Output;
    } DtaIoctlSpiPromCmdInOut;

    #define DT_IOCTL_SPIPROM_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SPIPROM_CMD,   \
                                                                  DtaIoctlSpiPromCmdInOut)
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
    typedef union _DtaIoctlStubCmdInOut {
        DtIoctlStubCmdInput  m_Input;
        DtIoctlStubCmdOutput  m_Output;
    } DtaIoctlStubCmdInOut;

    #define DT_IOCTL_STUB_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE, DT_FUNC_CODE_SPIMF_CMD,        \
                                                                     DtaIoctlStubCmdInOut)
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
    typedef union _DtaIoctlTempFanMgrCmdInOut
    {
        DtIoctlTempFanMgrCmdInput  m_Input;
        DtIoctlTempFanMgrCmdOutput  m_Output;
    } DtaIoctlTempFanMgrCmdInOut;

    #define DT_IOCTL_TEMPFANMGR_CMD  _IOWR(DT_IOCTL_MAGIC_SIZE,                          \
                                                          DT_FUNC_CODE_TEMPFANMGR_CMD,   \
                                                          DtaIoctlTempFanMgrCmdInOut)
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
//-.-.-.-.-.-.-.-.-.-.-.-.-.- TOD Command - IOCTL In/Out Data -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// TOD command - IOCTL input data
typedef union _DtIoctlTodCmdInput
{
    DtIoctlTodCmdAdjustTimeInput  m_AdjustTime;         // TOD - Adjust time
    DtIoctlTodCmdSetPhaseIncrInput  m_SetPhaseIncr;     // TOD - Set phase increment
    DtIoctlTodCmdSetTimeInput  m_SetTime;               // TOD - Set time
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
#define DT_TSRXFMT_PCKMODE_AUTO     0   // Automitic packetization
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
    DtIoctlAsiRxCmdInput  m_AsiTsRxCmd;                                                  \
    DtIoctlAsiTxGCmdInput  m_AsiTxGCmd;                                                  \
    DtIoctlAsiTxSerCmdInput  m_AsiTxSerCmd;                                              \
    DtIoctlBurstFifoCmdInput  m_BurstFifoCmd;                                            \
    DtIoctlCDmaCCmdInput  m_CDmaCCmd;                                                    \
    DtIoctlDebugCmdInput  m_DebugCmd;                                                    \
    DtIoctlExclAccessCmdInput  m_ExclAccessCmd;                                          \
    DtIoctlEventCmdInput  m_Event;                                                       \
    DtIoctlGenLockCtrlCmdInput  m_GenLockCtrlCmd;                                        \
    DtIoctlGetDriverVersionInput  m_GetDriverVersion;                                    \
    DtIoctlGetDevInfoInput  m_GetDevInfo;                                                \
    DtIoctlI2cMCmdInput  m_I2cMCmd;                                                      \
    DtIoctlIpSecGCmdInput  m_IpSecGCmd;                                                  \
    DtIoctlIoConfigCmdInput  m_IoConfig;                                                 \
    DtIoctlPropCmdInput  m_PropCmd;                                                      \
    DtIoctlKaCmdInput  m_KaCmd;                                                          \
    DtIoctlLedBCmdInput  m_LedBCmd;                                                      \
    DtIoctlRebootCmdInput  m_Reboot;                                                     \
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
    DtIoctlSpiMCmdInput  m_SpiMCmd;                                                      \
    DtIoctlSpiMfCmdInput  m_SpiMfCmd;                                                    \
    DtIoctlSpiPromCmdInput  m_SpiPromCmd;                                                \
    DtIoctlSt425LrCmdInput  m_St425LrCmd;                                                \
    DtIoctlSwitchCmdInput  m_SwitchCmd;                                                  \
    DtIoctlTempFanMgrCmdInput  m_TempFanMgrCmd;                                          \
    DtIoctlTodCmdInput  m_TodCmd;                                                        \
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
    DtIoctlConstSourceCmdInput  m_ConstSourceCmd

// The union
typedef union _DtIoctlInputData 
{
    DT_IOCTL_INPUT_DATA_COMMON_DATA;
} DtIoctlInputData;

// Common - command output data
#define DT_IOCTL_OUTPUT_DATA_COMMON_DATA                                                 \
    DtIoctlAccuFifoCmdOutput  m_AccuFifoCmd;                                             \
    DtIoctlAsiRxCmdOutput  m_AsiTsRxCmd;                                                 \
    DtIoctlAsiTxGCmdOutput  m_AsiTxGCmd;                                                 \
    DtIoctlAsiTxSerCmdOutput  m_AsiTxSerCmd;                                             \
    DtIoctlBurstFifoCmdOutput  m_BurstFifoCmd;                                           \
    DtIoctlCDmaCCmdOutput  m_CDmaCCmd;                                                   \
    DtIoctlDebugCmdOutput  m_DebugCmd;                                                   \
    DtIoctlEventCmdOutput  m_Event;                                                      \
    DtIoctlGenLockCtrlCmdOutput  m_GenLockCtrlCmd;                                       \
    DtIoctlGetDriverVersionOutput  m_GetDriverVersion;                                   \
    DtIoctlGetDevInfoOutput  m_GetDevInfo;                                               \
    DtIoctlI2cMCmdOutput  m_I2cMCmd;                                                     \
    DtIoctlIpSecGCmdOutput  m_IpSecGCmd;                                                 \
    DtIoctlIoConfigCmdOutput  m_IoConfig;                                                \
    DtIoctlKaCmdOutput  m_KaCmd;                                                         \
    DtIoctlLedBCmdOutput  m_LedBCmd;                                                     \
    DtIoctlLmh1981CmdOutput  m_Lmh1981Cmd;                                               \
    DtIoctlPropCmdOutput  m_PropCmd;                                                     \
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
    DtIoctlSpiMCmdOutput  m_SpiMCmd;                                                     \
    DtIoctlSpiMfCmdOutput  m_SpiMfCmd;                                                   \
    DtIoctlSpiPromCmdOutput  m_SpiPromCmd;                                               \
    DtIoctlSt425LrCmdOutput  m_St425LrCmd;                                               \
    DtIoctlSwitchCmdOutput  m_SwitchCmd;                                                 \
    DtIoctlTempFanMgrCmdOutput  m_TempFanMgrCmd;                                         \
    DtIoctlTodCmdOutput  m_TodCmd;                                                       \
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
    DtIoctlConstSourceCmdOutput  m_ConstSourceCmd

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
