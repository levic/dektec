//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCommon.h *#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
//
// SDK - Common definitions and types between for DTAPI/DTA/DTU drivers
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2016 DekTec Digital Video B.V.
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


// Define Int64/UInt64 here to a non-existing identifier. Since all types in this header
// are used in a binary interface we have to make sure they are compatible between
// 32-bit and 64-bit builds. For this we have to align every 64-bit value to an 8-byte
// boundary. The Int64A / UInt64A types do this. Make sure Int64/UInt64 are not used.
#define Int64 ERROR_DO_NOT_USE_UNALIGNED_INT64
#define UInt64 ERROR_DO_NOT_USE_UNALIGNED_UINT64

#ifdef WINBUILD

// Driver / DTAPI include files
#ifdef DTAPI
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

#endif  // WINBUILD


// Type to store the scope that can request a property
typedef enum _DtPropertyScope
{
    PROPERTY_SCOPE_DRIVER = 1,
    PROPERTY_SCOPE_DTAPI = 2,
} DtPropertyScope;

// Type to store the value of a propery
typedef UInt64A  DtPropertyValue;

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
    PROPERTY_VALUE_TYPE_STRING
} DtPropertyValueType;

// Table entry
typedef struct _DtTableEntry
{
    Int64A  m_X;
    Int64A  m_Y;
} DtTableEntry;
ASSERT_SIZE(DtTableEntry, 16)

// IO configuration description
#define DT_MAX_PARXTRA_COUNT        4
#define IOCONFIG_NAME_MAX_SIZE      50
// NOTE: we added Pars suffix to prevent name conflict with DtApi::DtIoConfig struct
typedef struct _DtIoConfigPars
{
    Int  m_PortIndex;
    char  m_Group[IOCONFIG_NAME_MAX_SIZE];
    char  m_Value[IOCONFIG_NAME_MAX_SIZE];
    char  m_SubValue[IOCONFIG_NAME_MAX_SIZE];
    Int64A  m_ParXtra[DT_MAX_PARXTRA_COUNT];

    Bool  m_SkipExclAccessCheck;    // If true, do not check if port is in use.
                                    // NOTE: only set this to true, if you are the
                                    // owner of the port (i.e. you have the excl access)
} DtIoConfigPars;
ASSERT_SIZE(DtIoConfigPars, 200)

// DtIoConfigId and DtIoConfigValue are split version of DtIoConfigPars for use in 
// GetIoConfig IO-controls
typedef struct  _DtIoConfigId
{
    Int  m_PortIndex;
    char  m_Group[IOCONFIG_NAME_MAX_SIZE];
} DtIoConfigId;
ASSERT_SIZE(DtIoConfigId, 56)
typedef struct  _DtIoConfigValue
{
    char  m_Value[IOCONFIG_NAME_MAX_SIZE];
    char  m_SubValue[IOCONFIG_NAME_MAX_SIZE];
    Int64A  m_ParXtra[DT_MAX_PARXTRA_COUNT];
} DtIoConfigValue;
ASSERT_SIZE(DtIoConfigValue, 136)

// DtAvFrameProps describes the properties of a Audio/Video frame (e.g. #lines, field 
// start/end, etc)
typedef struct _DtAvFrameProps
{
    Int  m_VidStd;              // Video standard (DT_VIDSTD_XXX)
    Int  m_Fps;                 // Frames per second. NOTE: div by 1.001 for fractional
    Bool  m_IsHd;               // Is an HD format
    Bool  m_IsInterlaced;       // Is an interlaced format
    Bool  m_IsFractional;       // Is a fraction format
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

#define ASI_SDI_SER_ITF_NONE         0           // No ASI/SDI serialiser used
#define ASI_SDI_SER_ITF_HOTLINK      1           // Use Cypress hotlink
#define ASI_SDI_SER_ITF_FPGA_BASED   2           // Default DekTec FPGA based 
                                                 // implementation
#define ASI_SDI_SER_ITF_GS2962       4           // Uses Gennum GS2962
#define ASI_SDI_SER_ITF_FPGA_LMH0387 5           // FPGA serialiser + LMH0387 line driver
#define ASI_SDI_SER_ITF_FPGA_GS3490 6            // FPGA serialiser + GS3490 line driver

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

// Temperature sensor types
#define TEMP_SENS_TYPE_MAX6639       0           // Maxim 6639 controller
#define TEMP_SENS_TYPE_FX3           1           // FX3 controller

// PCIe bridges
#define PCIE_BRIDGE_TYPE_PEX87XX     0           // PLX PEX 87XX chip
#define PCIE_BRIDGE_TYPE_PEX811X     1           // PLX PEX 8711/8712 chip


// Properties
#define PROPERTY_NAME_MAX_SIZE      50
#define PROPERTY_STR_MAX_SIZE       96

// MAC types
#define  MAC_TYPE_CAST              0
#define  MAC_TYPE_ALTERA            1

#undef Int64
#undef UInt64

#endif  // __DT_COMMON_H
