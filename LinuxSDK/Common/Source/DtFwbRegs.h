//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtFwbRegs.h *#*#*#*#*#*#*#*#*#*#*# (C) 2015 DekTec
//
// Dtx driver - Definition of register sets of firmware blocks
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2015 DekTec Digital Video B.V.
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

#ifndef __DTFWBREGS_H
#define __DTFWBREGS_H

#ifndef FALSE
#define FALSE false
#endif
#ifndef TRUE
#define TRUE false
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Firmware field operations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef enum _DtFwFieldOp
{
        FWFIELDOP_R = 0x01,       // Read
        FWFIELDOP_W = 0x02,       // Write
        FWFIELDOP_C = 0x04,       // Clear
        FWFIELDOP_RW = FWFIELDOP_R + FWFIELDOP_W,       // Read Write
        FWFIELDOP_RC = FWFIELDOP_R + FWFIELDOP_C        // Read Clear
} DtFwFieldOp;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Firmware field definition -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwField
{
    Int32  Offset;          // Offset in firmware block
    Int32  StartBit;        // Start bit in the register (zero-based)
    Int32  NumBits;         // Field size in number of bits
    Bool  Exclusive;        // True if there are no other fields in the same register
    DtFwFieldOp Operation;  // Possible operations
} DtFwField;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Firmware blocks +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbBlockId .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbBlockId
{
    const DtFwField BlockId_Id;                 // BlockId Identification
    const DtFwField BlockId_Version;            // BlockId Version
} DtFwbBlockId;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_BLOCK_ID(OFFSET)                                                          \
{                                                                                        \
    {0x000 + OFFSET, 0, 24, FALSE, FWFIELDOP_R},    /* BlockId Identification */         \
    {0x000 + OFFSET, 24, 8, FALSE, FWFIELDOP_R},    /* BlockId Version */                \
}
const DtFwbBlockId FwbBlockId = DT_FWB_BLOCK_ID(0);
#else
extern const DtFwbBlockId FwbBlockId;
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbUsb3GenReg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbUsb3GenReg
{
    const DtFwbBlockId  FwbBlockId;                 // Block ID
    const DtFwField  TypeNum_TypeNum;               // Type Number
    const DtFwField  FirmwVersion_FirmwRev;         // Firmware Revision
    const DtFwField  FirmwVersion_FirmwVar;         // Firmware Variant
    const DtFwField  FirmwVersion_FirmwBuildNum;    // Firmware Build Number
    const DtFwField  Resets_DspReset;               // DSP Reset
} DtFwbUsb3GenReg;

#ifdef DEFINE_FWFIELDS 
#define DT_FWB_USB3_GENREG(OFFSET)                                                       \
{                                                                                        \
    DT_FWB_BLOCK_ID(0X000 + OFFSET),                /* Block ID */                       \
    {0x004 + OFFSET, 0, 16, TRUE, FWFIELDOP_R},     /* Type Number */                    \
    {0x008 + OFFSET, 0, 8, FALSE, FWFIELDOP_R},     /* Firmware Revision */              \
    {0x008 + OFFSET, 8, 8, FALSE, FWFIELDOP_R},     /* Firmware Variant */               \
    {0x008 + OFFSET, 16, 8, FALSE, FWFIELDOP_R},    /* Firmware Build Number */          \
    {0x00C + OFFSET, 0, 1, TRUE, FWFIELDOP_W}       /* DSP Reset */                      \
}
const DtFwbUsb3GenReg FwbUsb3GenReg = DT_FWB_USB3_GENREG(0);
#else
extern const DtFwbUsb3GenReg FwbUsb3GenReg;
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbFifoMemoryLessUsbDvc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbFifoMemoryLessUsbDvc
{
    const DtFwbBlockId  FwbBlockId;         // Block ID
    const DtFwField  Control_Clear;         // Clear FIFO
    const DtFwField  Control_TxCtrl;        // Transmit Control
    const DtFwField  Status_UflFlag;        // Latched Underflow Flag
    const DtFwField  Size;                  // FIFO Size
    const DtFwField  Load;                  // FIFO Load
}DtFwbFifoMemoryLessUsbDvc;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_FIFO_MEMORYLESS_USBDVC(OFFSET)                                            \
{                                                                                        \
    DT_FWB_BLOCK_ID(0X000 + OFFSET),                /* Block ID */                       \
    {0x004 + OFFSET, 0, 1, FALSE, FWFIELDOP_W},     /* Clear FIFO */                     \
    {0x004 + OFFSET, 1, 2, FALSE, FWFIELDOP_RW},    /* Transmit Control */               \
    {0x008 + OFFSET, 0, 1, FALSE, FWFIELDOP_RC},    /* Latched Underflow Flag */         \
    {0x00C + OFFSET, 0, 32, TRUE, FWFIELDOP_R},     /* FIFO Size */                      \
    {0x010 + OFFSET, 0, 32, TRUE, FWFIELDOP_R}      /* FIFO Load */                      \
}
const DtFwbFifoMemoryLessUsbDvc FwbFifoMemoryLessUsbDvc = 
                                                         DT_FWB_FIFO_MEMORYLESS_USBDVC(0);
#else
extern const DtFwbFifoMemoryLessUsbDvc FwbFifoMemoryLessUsbDvc;
#endif
// Fifo transmit control
#define DTFWB_MLFIFOTXCTRL_IDLE            0x00  // Idle
#define DTFWB_MLFIFOTXCTRL_HOLD            0x01  // Hold
#define DTFWB_MLFIFOTXCTRL_SEND            0x03  // Send

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbUnpackerIqMapper -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbUnpackerIqMapper
{
    const DtFwbBlockId  FwbBlockId;             // Block ID
    const  DtFwField  GeneralControl_ModType;   // Modulation Type
    const  DtFwField  IqMapControl;             // IQMapControl
    const  DtFwField  IqMapControl_RdOrWrn;     // Data Direction; 0=WRITE 1=READ
    const  DtFwField  IqMapControl_TableIndex;  // Table Index (0..279)
    const  DtFwField  IqMapWrData;              // IQ Map Write Data
    const  DtFwField  IqMapRdData;              // IQ Map Read Data
} DtFwbUnpackerIqMapper;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_UNPACKER_IQMAPPER(OFFSET)                                                 \
{                                                                                        \
    DT_FWB_BLOCK_ID(0X000 + OFFSET),                /* Block ID */                       \
    {0x004 + OFFSET, 0, 4, FALSE, FWFIELDOP_RW},    /* Modulation Type */                \
    {0x008 + OFFSET, 0, 10, TRUE, FWFIELDOP_RW},    /* IQMapControl */                   \
    {0x008 + OFFSET, 0, 1, FALSE, FWFIELDOP_RW},    /* Data Direction; 0=WRITE 1=READ */ \
    {0x008 + OFFSET, 1, 9, FALSE, FWFIELDOP_RW},    /* Table Index (0..279) */           \
    {0x00C + OFFSET, 0, 32, TRUE, FWFIELDOP_RW},    /* IQ Map Write Data */              \
    {0x010 + OFFSET, 0, 32, TRUE, FWFIELDOP_R},     /* IQ Map Read Data */               \
}
const DtFwbUnpackerIqMapper FwbUnpackerIqMapper = DT_FWB_UNPACKER_IQMAPPER(0);
#else
extern const DtFwbUnpackerIqMapper FwbUnpackerIqMapper;
#endif

// Unpacker modulation types
#define DTFWB_UNPCKMODTYPE_IDLE          0x00  // Idle
#define DTFWB_UNPCKMODTYPE_SYMB8         0x01  // 8-bit symbol mode
#define DTFWB_UNPCKMODTYPE_SYMB8E        0x02  // 8-bit extended symbol mode
#define DTFWB_UNPCKMODTYPE_SYMB4         0x03  // 4-bit symbol mode
#define DTFWB_UNPCKMODTYPE_IQPCK16       0x04  // Direct I/Q packed 16 bits
#define DTFWB_UNPCKMODTYPE_IQPCK12       0x05  // Direct I/Q packed 12 bits
#define DTFWB_UNPCKMODTYPE_IQPCK10       0x06  // Direct I/Q packed 10 bits


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbTestPatternGenerator -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbTestPatternGenerator
{
    const  DtFwbBlockId FwbBlockId;                 // Block ID
    const  DtFwField TestPatControl_TestPatControl; // Test Pattern Control
} DtFwbTestPatternGenerator;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_TESTPATTERN_GENERATOR(OFFSET)                                             \
{                                                                                        \
    DT_FWB_BLOCK_ID(0X000 + OFFSET),                /* Block ID */                       \
    {0x004 + OFFSET, 0, 4, TRUE, FWFIELDOP_RW}      /* Test Pattern Control */           \
}
const DtFwbTestPatternGenerator FwbTestPatternGenerator = DT_FWB_TESTPATTERN_GENERATOR(0);
#else
extern const DtFwbTestPatternGenerator FwbTestPatternGenerator;
#endif

// Test patterns
#define DTFWB_TESTPAT_NORMAL            0  // Normal mode
#define DTFWB_TESTPAT_NYQUIST           1  // Nyquist test-pattern
#define DTFWB_TESTPAT_HALFNYQ           2  // Half-nyquist tets-pattern
#define DTFWB_TESTPAT_CW                3  // Carrier only
#define DTFWB_TESTPAT_CWI               4  // Constant I
#define DTFWB_TESTPAT_CWQ               5  // Constant Q

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbChannelFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbChannelFilter
{
    const DtFwbBlockId  FwbBlockId;                 // Block ID
    const  DtFwField  FiltCoefControl;              // Filter Coefficient Control
    const  DtFwField  FiltCoefControl_Ready;        // Ready For Next Transfer
    const  DtFwField  FiltCoefControl_Start;        // Start Transfer
    const  DtFwField  FiltCoefControl_Clear;        // Clear FIFO
    const  DtFwField  FiltCoefControl_RdOrWrn;      // Data Direction; 0=WRITE 1=READ
    const  DtFwField  FiltCoefData_FiltCoeffData;   // Filter Coefficient Data
} DtFwbChannelFilter;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_CHANNEL_FILTER(OFFSET)                                                    \
{                                                                                        \
    DT_FWB_BLOCK_ID(0X000 + OFFSET),                /* Block ID */                       \
    {0x004 + OFFSET, 0, 4, TRUE, FWFIELDOP_RW},     /* Filter Coefficient Control */     \
    {0x004 + OFFSET, 0, 1, FALSE, FWFIELDOP_R},     /* Ready For Next Transfer */        \
    {0x004 + OFFSET, 1, 1, FALSE, FWFIELDOP_W},     /* Start Transfer */                 \
    {0x004 + OFFSET, 2, 1, FALSE, FWFIELDOP_W},     /* Clear FIFO */                     \
    {0x004 + OFFSET, 3, 1, FALSE, FWFIELDOP_RW},    /* Data Direction; 0=WRITE 1=READ */ \
    {0x008 + OFFSET, 0, 16, TRUE, FWFIELDOP_RW}     /* Filter Coefficient Data */        \
}
const DtFwbChannelFilter FwbChannelFilter = DT_FWB_CHANNEL_FILTER(0);
#else
extern const DtFwbChannelFilter FwbChannelFilter;
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbAwgnGenerator -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbAwgnGenerator
{
    const DtFwbBlockId FwbBlockId;              // Block ID
    const  DtFwField NoiseControl_NoiseLevel;   // Noise Level
    const  DtFwField NoiseControl_NoiseRange;   // Noise Range
    const  DtFwField NoiseStatus_StaturCntIQ;   // Saturation Counter IQ
    const  DtFwField NoiseStatus_StaturCntI;    // Saturation Counter I
    const  DtFwField NoiseStatus_StaturCntQ;    // Saturation Counter Q
} DtFwbAwgnGenerator;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_AWGN_GENERATOR(OFFSET)                                                    \
{                                                                                        \
    DT_FWB_BLOCK_ID(0X000 + OFFSET),                /* Block ID */                       \
    {0x004 + OFFSET, 0, 8, FALSE, FWFIELDOP_RW},    /* Noise Level */                    \
    {0x004 + OFFSET, 8, 3, FALSE, FWFIELDOP_RW},    /* Noise Range */                    \
    {0x008 + OFFSET, 0, 32, TRUE, FWFIELDOP_R},     /* Saturation Counter IQ */          \
    {0x008 + OFFSET, 0, 16, FALSE, FWFIELDOP_R},    /* Saturation Counter I */           \
    {0x008 + OFFSET, 16, 16, FALSE, FWFIELDOP_R}    /* Saturation Counter Q */           \
};
const DtFwbAwgnGenerator FwbAwgnGenerator = DT_FWB_AWGN_GENERATOR(0);
#else
extern const DtFwbAwgnGenerator FwbAwgnGenerator;
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbVarSampUpConverter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbVarSampUpConverter
{
    const DtFwbBlockId  FwbBlockId;         // Block ID
    const  DtFwField  VsrcControl;          // TxClockInc
} DtFwbVarSampUpConverter;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_VARSAMP_UPCONVERTER(OFFSET)                                               \
{                                                                                        \
    DT_FWB_BLOCK_ID(0X000 + OFFSET),                /* Block ID */                       \
    {0x004 + OFFSET, 0, 32, TRUE, FWFIELDOP_RW}     /* TxClockInc */                     \
}
const DtFwbVarSampUpConverter FwbVarSampUpConverter = DT_FWB_VARSAMP_UPCONVERTER(0);
#else
extern const DtFwbVarSampUpConverter FwbVarSampUpConverter;
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbIqScaler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbIqScaler
{
    const DtFwbBlockId  FwbBlockId;                 // Block ID
    const  DtFwField  MuxControl_SpecInv;           // Spectral Inversion
    const  DtFwField  MuxControl_MuteIQ;            // Mute IQ
    const  DtFwField  MuxControl_MuteI;             // Mute I
    const  DtFwField  MuxControl_MuteQ;             // Mute Q
    const  DtFwField  OutpLvlOffset_OutpLvlOffset;  // Output Level Offset
    const  DtFwField  OutputGain_OutputGain;        // Output Gain
} DtFwbIqScaler;


#ifdef DEFINE_FWFIELDS
#define DT_FWB_IQSCALER(OFFSET)                                                          \
{                                                                                        \
    DT_FWB_BLOCK_ID(0X000 + OFFSET),                /* Block ID */                       \
    {0x004 + OFFSET, 0, 1, FALSE, FWFIELDOP_RW},    /* Spectral Inversion */             \
    {0x004 + OFFSET, 1, 2, FALSE, FWFIELDOP_RW},    /* Mute IQ */                        \
    {0x004 + OFFSET, 1, 1, FALSE, FWFIELDOP_RW},    /* Mute I */                         \
    {0x004 + OFFSET, 2, 1, FALSE, FWFIELDOP_RW},    /* Mute Q */                         \
    {0x008 + OFFSET, 0, 18, TRUE, FWFIELDOP_RW},    /* Output Level Offset */            \
    {0x00C + OFFSET, 0, 16, TRUE, FWFIELDOP_RW}     /* Output Gain */                    \
}
const DtFwbIqScaler FwbIqScaler = DT_FWB_IQSCALER(0);
#else
extern const DtFwbIqScaler FwbIqScaler;
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbDat31R5Attenuator -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbDat31R5Attenuator
{
    const DtFwbBlockId  FwbBlockId;         // Block ID
    const  DtFwField  Control_AttenLevel;   // Attenuation Level
} DtFwbDat31R5Attenuator;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_DAT31R5_ATTENUATOR(OFFSET)                                                \
{                                                                                        \
    DT_FWB_BLOCK_ID(0X000 + OFFSET),                /* Block ID */                       \
    {0x004 + OFFSET, 0, 6, TRUE, FWFIELDOP_RW}      /* Attenuation Level */              \
}
const DtFwbDat31R5Attenuator FwbDat31R5Attenuator = DT_FWB_DAT31R5_ATTENUATOR(0);
#else
extern const DtFwbDat31R5Attenuator FwbDat31R5Attenuator;
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbHmc839RfPll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbHmc839RfPll
{
    const DtFwbBlockId  FwbBlockId;         // Block ID
    const  DtFwField  NDiv_NDiv;            // N Divider
    const  DtFwField  Status_Locked;        // Locked
} DtFwbHmc839RfPll;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_HMC839_RFPLL(OFFSET)                                                      \
{                                                                                        \
    DT_FWB_BLOCK_ID(0X000 + OFFSET),                /* Block ID */                       \
    {0x004 + OFFSET, 0, 8, TRUE, FWFIELDOP_RW},     /* N Divider */                      \
    {0x008 + OFFSET, 0, 1, TRUE, FWFIELDOP_R}       /* Locked */                         \
}
const DtFwbHmc839RfPll FwbHmc839RfPll = DT_FWB_HMC839_RFPLL(0);
#else
extern const DtFwbHmc839RfPll FwbHmc839RfPll;
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbMax5868Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbMax5868Control
{
    const DtFwbBlockId  FwbBlockId;         // Block ID
    const  DtFwField  Control_Reset;        // Reset
} DtFwbMax5868Control;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_MAX5868_CONTROL(OFFSET)                                                   \
{                                                                                        \
    DT_FWB_BLOCK_ID(0X000 + OFFSET),                /* Block ID */                       \
    {0x004 + OFFSET, 0, 1, TRUE, FWFIELDOP_RW}      /* Reset */                          \
}
const DtFwbMax5868Control FwbMax5868Control = DT_FWB_MAX5868_CONTROL(0);
#else
extern const DtFwbMax5868Control FwbMax5868Control;
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- MAX5868 Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  MAX5868REG_CFGCHIPOM    0x00    // Configure Chip Operation Mode
#define  MAX5868REG_CFGDSP       0x01    // Configure DSP Engine 
#define  MAX5868REG_CFGNCO0      0x02    // Configure NCO Frequency Control 0
#define  MAX5868REG_CFGNCO1      0x03    // Configure NCO Frequency Control 1
#define  MAX5868REG_CFGNCO2      0x04    // Configure NCO Frequency Control 2
#define  MAX5868REG_CFGNCO3      0x05    // Configure NCO Frequency Control 3
#define  MAX5868REG_CFGPD        0x06    // Configure Peak Detector
#define  MAX5868REG_CFGPDT       0x07    // Configure Peak Detector Threshold
#define  MAX5868REG_CFGPDMC0     0x08    // Configure Peak Detector Maximum Count 0
#define  MAX5868REG_CFGPDMC1     0x09    // Configure Peak Detector Maximum Count 1
#define  MAX5868REG_CFGPDMC2     0x0A    // Configure Peak Detector Maximum Count 2
#define  MAX5868REG_CFGPDMC3     0x0B    // Configure Peak Detector Maximum Count 3
#define  MAX5868REG_CFGPDMC4     0x0C    // Configure Peak Detector Maximum Count 4
#define  MAX5868REG_CFGPDMC5     0x0D    // Configure Peak Detector Maximum Count 5
#define  MAX5868REG_STATPD0      0x0E    // Configure Peak Detector Status 0
#define  MAX5868REG_STATPD1      0x0F    // Configure Peak Detector Status 1
#define  MAX5868REG_CFGSYNC      0x50    // Configure SYNCI and SYNCO
#define  MAX5868REG_CFGFIFO      0x51    // Configure Input FIFO
#define  MAX5868REG_EMUTE        0x54    // Enable Mute Mask
#define  MAX5868REG_EINT         0x55    // Enable Interrupt Mask
#define  MAX5868REG_STATUS       0x56    // Status
#define  MAX5868REG_DEVSN0       0x57    // Device Serial Number 0
#define  MAX5868REG_DEVSN1       0x58    // Device Serial Number 1
#define  MAX5868REG_DEVSN2       0x59    // Device Serial Number 2
#define  MAX5868REG_CFGADKEY     0x60    // Configure Advanced Digital Access Key
#define  MAX5868REG_CFGNCO_E0    0x6B    // Configure NCO Extended Resolution 0
#define  MAX5868REG_CFGNCO_E1    0x6C    // Configure NCO Extended Resolution 1
#define  MAX5868REG_CFGNCO_E2    0x6D    // Configure NCO Extended Resolution 2
#define  MAX5868REG_CFGNCO_E3    0x6E    // Configure NCO Extended Resolution 3
#define  MAX5868REG_CFGNCO_E4    0x6F    // Configure NCO Extended Resolution 4


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFwbFx3 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbFx3
{
    const  DtFwField UsbSpeed;            // USB speed
    const  DtFwField WatchDog;            // Watchdog
    const  DtFwField PhyErrors;           // PHY errors
    const  DtFwField LinkErrors;          // Link errors
    const  DtFwField PnpCmdStat;          // PNP cmd stat
    const  DtFwField LedCtrl;             // LED control
    const  DtFwField PcbTemperature;      // PCB-Temperature
    const  DtFwField DacTemperature;      // DAC-Temperature
    const  DtFwField TemperatureError;    // Temperature error
    const  DtFwField TypeNumber;          // Device typenumber (e.g. 351 for DTU-351)
    const  DtFwField FirmwareVersion;     // FX3 firmware version
    const  DtFwField FirmwareBuildNum;    // FX3 firmware build number
} DtFwbFx3;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_FX3(OFFSET)                                                               \
{                                                                                        \
    {0x000 + OFFSET, 0, 8,  TRUE, FWFIELDOP_R},     /* USB speed */                      \
    {0x004 + OFFSET, 0, 1,  TRUE, FWFIELDOP_RC},    /* Watchdog device triggered */      \
    {0x008 + OFFSET, 0, 16, TRUE, FWFIELDOP_RW},    /* PHY errors */                     \
    {0x00C + OFFSET, 0, 16, TRUE, FWFIELDOP_RW},    /* Link errors */                    \
    {0x010 + OFFSET, 0, 2,  TRUE, FWFIELDOP_RW},    /* PNP cmd stat */                   \
    {0x018 + OFFSET, 0, 8,  TRUE, FWFIELDOP_RW},    /* LED control */                    \
    {0x020 + OFFSET, 0, 16, FALSE, FWFIELDOP_R},    /* PCB-temperature */                \
    {0x020 + OFFSET, 16, 16, FALSE, FWFIELDOP_R},   /* DAC-temperature */                \
    {0x024 + OFFSET, 0, 1,  TRUE, FWFIELDOP_RC},    /* Temperature error */              \
    {0x0F4 + OFFSET, 0, 32, TRUE, FWFIELDOP_R},     /* Device typenumber */              \
    {0x0F8 + OFFSET, 0, 32, TRUE, FWFIELDOP_R},     /* FX3 firmware version */           \
    {0x0FC + OFFSET, 0, 32, TRUE, FWFIELDOP_R}      /* FX3 firmware build number */      \
}
const DtFwbFx3  FwbFx3 = DT_FWB_FX3(0);
#else
extern const DtFwbFx3 FwbFx3;
#endif


#endif // #ifndef __DTFWBREGS_H
