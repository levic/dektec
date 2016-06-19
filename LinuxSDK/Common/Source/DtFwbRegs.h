//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtFwbRegs.h *#*#*#*#*#*#*#*#* (C) 2015-2016 DekTec
//
// Dtx driver - Definition of register sets of firmware blocks
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2015-2016 DekTec Digital Video B.V.
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
#define TRUE true
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




//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=++=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA-2180 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Firmware blocks +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=++=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbPcieGenReg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbPcieGenReg
{
    const DtFwField Control_Reset;          // Device reset
    const DtFwField Control_PerIntEnable;   // Periodic interrupt enable
    const DtFwField Control_PerIntInterval; // Periodic interrrupt interval (x 9.7ms)
    const DtFwField Control_FirmwRev;       // Firmware revision
    const DtFwField Control_TypeNum;        // Type number
    const DtFwField Control_LedControl;     // LED control
    const DtFwField Control_LedGreen;       // Green LED state
    const DtFwField Control_LedRed;         // Red LED state
    const DtFwField Control_LedBlue;        // Blue LED state
    const DtFwField Status_PerIntStatus;    // Periodic interrupt status
    const DtFwField Status_FirmwVar;        // Firmware variant
    const DtFwField Control_TypeNumExt;     // Type number extension
    const DtFwField RefClockCount;          // Reference clock count
    const DtFwField FpgaSpiCtrl_Erase;      // FPGA flash erase request
    const DtFwField FpgaSpiCtrl_PageWrite;  // FPGA flash page write request
    const DtFwField FpgaSpiCtrl_Read;       // FPGA flash read request
    const DtFwField FpgaSpiCtrl_AddrHi;     // FPGA flash address 4MSBs
    const DtFwField FpgaSpiCtrl_IsBusy;     // FPGA flash is busy?
    const DtFwField FpgaSpiCtrl_AddrLo;     // FPGA flash address 20LSBs
    const DtFwField FpgaSpiCtrl_Unlock;     // FPGA flash write unlock
    const DtFwField FpgaSpiData;            // FPGA flash data (read or write)
    const DtFwField RefClockCountHigh;      // Reference clock count, high part
    const DtFwField VpdSpiCtrl_PageWrite;   // VPD eeprom page write request
    const DtFwField VpdSpiCtrl_Read;        // VPD eeprom read request
    const DtFwField VpdSpiCtrl_IsBusy;      // VPD eeprom is busy?
    const DtFwField VpdSpiCtrl_AddrLo;      // VPD eeprom address 20LSBs
    const DtFwField VpdSpiCtrl_Unlock;      // VPD eeprom write unlock
    const DtFwField VpdSpiData;             // VPD eeprom data (read or write)

} DtFwbPcieGenReg;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_PCIE_GENREG(OFFSET)                                                       \
{                                                                                        \
    {0x000 + OFFSET, 2, 1, FALSE, FWFIELDOP_W},      /* Device reset */                  \
    {0x000 + OFFSET, 4, 1, FALSE, FWFIELDOP_RW},     /* Periodic interrupt enable */     \
    {0x000 + OFFSET, 5, 3, FALSE, FWFIELDOP_RW},     /* Periodic int intv (x 9.7ms) */   \
    {0x000 + OFFSET, 8, 8, FALSE, FWFIELDOP_R},      /* Firmware revision */             \
    {0x000 + OFFSET, 16, 8, FALSE, FWFIELDOP_R},     /* Type number */                   \
    {0x000 + OFFSET, 24, 1, FALSE, FWFIELDOP_RW},    /* LED control */                   \
    {0x000 + OFFSET, 25, 1, FALSE, FWFIELDOP_RW},    /* Green LED state */               \
    {0x000 + OFFSET, 26, 1, FALSE, FWFIELDOP_RW},    /* Red LED state */                 \
    {0x000 + OFFSET, 27, 1, FALSE, FWFIELDOP_RW},    /* Blue LED state */                \
    {0x004 + OFFSET, 0, 1, FALSE, FWFIELDOP_RC},     /* Periodic interrupt status */     \
    {0x004 + OFFSET, 8, 5, FALSE, FWFIELDOP_R},      /* Firmware variant */              \
    {0x004 + OFFSET, 16, 3, FALSE, FWFIELDOP_R},     /* Type number extension */         \
    {0x00C + OFFSET, 0, 32, TRUE, FWFIELDOP_R},      /* Reference clock count */         \
    {0x010 + OFFSET, 0, 1, FALSE, FWFIELDOP_W},      /* FPGA flash erase request */      \
    {0x010 + OFFSET, 1, 1, FALSE, FWFIELDOP_W},      /* FPGA flash page write request */ \
    {0x010 + OFFSET, 2, 1, FALSE, FWFIELDOP_W},      /* FPGA flash read request */       \
    {0x010 + OFFSET, 3, 4, FALSE, FWFIELDOP_RW},     /* FPGA flash address 4MSBs */      \
    {0x010 + OFFSET, 7, 1, FALSE, FWFIELDOP_R},      /* FPGA flash is busy? */           \
    {0x010 + OFFSET, 8, 20, FALSE, FWFIELDOP_RW},    /* FPGA flash address 20LSBs */     \
    {0x010 + OFFSET, 28, 4, FALSE, FWFIELDOP_RW},    /* FPGA flash write unlock */       \
    {0x014 + OFFSET, 0, 32, TRUE, FWFIELDOP_RW},     /* FPGA flash data (read, write) */ \
    {0x018 + OFFSET, 0, 8, FALSE, FWFIELDOP_R},      /* Reference clock count, high */   \
    {0x024 + OFFSET, 1, 1, FALSE, FWFIELDOP_W},      /* VPD eeprom page write request */ \
    {0x024 + OFFSET, 2, 1, FALSE, FWFIELDOP_W},      /* VPD eeprom read request */       \
    {0x024 + OFFSET, 7, 1, FALSE, FWFIELDOP_R},      /* VPD eeprom is busy? */           \
    {0x024 + OFFSET, 8, 9, FALSE, FWFIELDOP_RW},     /* VPD eeprom address */            \
    {0x024 + OFFSET, 28, 4, FALSE, FWFIELDOP_RW},    /* VPD eeprom write unlock */       \
    {0x028 + OFFSET, 0, 32, TRUE, FWFIELDOP_RW},     /* VPD eeprom data (read, write) */ \
}
const DtFwbPcieGenReg FwbPcieGenReg = DT_FWB_PCIE_GENREG(0);
#else
extern const DtFwbPcieGenReg FwbPcieGenReg;
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbSpiMasterSimple -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbSpiMasterSimple
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('SPIS')
    const DtFwField  SpiStatus_IsBusy;      // Is SPI busy?
    const DtFwField  SpiStatus_WordLength;  // Word length
    const DtFwField  SpiStatus_ClkEdge;     // Clock edge
    const DtFwField  SpiStatus_ClkPolarity; // Clock polarity
    const DtFwField  SpiStatus_TfOrder;     // Transfer order
    const DtFwField  TxData;                // Transmit data
    const DtFwField  RxData;                // Receive data
} DtFwbSpiMasterSimple;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_SPI_MASTER_SIMPLE(OFFSET)                                                 \
{                                                                                        \
    DT_FWB_BLOCK_ID(0 + OFFSET),                    /* Block ID */                       \
    {0x008 + OFFSET, 0, 1, FALSE, FWFIELDOP_R},     /* Is SPI busy? */                   \
    {0x008 + OFFSET, 1, 2, FALSE, FWFIELDOP_R},     /* Word length */                    \
    {0x008 + OFFSET, 3, 1, FALSE, FWFIELDOP_R},     /* Clock edge */                     \
    {0x008 + OFFSET, 4, 1, FALSE, FWFIELDOP_R},     /* Clock polarity */                 \
    {0x008 + OFFSET, 5, 1, FALSE, FWFIELDOP_RC},    /* Transfer order */                 \
    {0x00C + OFFSET, 0, 32, TRUE, FWFIELDOP_RW},    /* Transmit data */                  \
    {0x010 + OFFSET, 0, 32, TRUE, FWFIELDOP_R}      /* Receive data */                   \
}
const DtFwbSpiMasterSimple FwbSpiMasterSimple = DT_FWB_SPI_MASTER_SIMPLE(0);
#else
extern const DtFwbSpiMasterSimple FwbSpiMasterSimple;
#endif
// Word length for SPI transfers
#define DTFWB_SPIMS_WLEN_8      0x00        // 8 bit word length
#define DTFWB_SPIMS_WLEN_16     0x01        // 16 bit word length
#define DTFWB_SPIMS_WLEN_32     0x02        // 32 bit word length




//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbSpiMasterFlash -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbSpiMasterFlash
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('SPIF')
    const DtFwField  Config1_SpiClockRate;  // Frequency of SCK signal
    const DtFwField  Config1_MemoryId;      // Identifier for SPI Flash/PROM device
    const DtFwField  Config2_PageSize;      // Page size in log2(bytes)
    const DtFwField  Config2_SectorSize;    // Sector size in log2(bytes), 0=no sector)
    const DtFwField  Config2_MemorySize;    // Memory size in log2(bytes)
    const DtFwField  Config3_TxFifoSize;    // Transmit FIFO size
    const DtFwField  Config3_RxFifoSize;    // Receive FIFO size
    const DtFwField  Control_AllInts;       // All Interruptss
    const DtFwField  Control_TxHalfEmptyIntEnable; // Transmit FIFO half empty int. enable
    const DtFwField  Control_RxHalfFullIntEnable; // Receive FIFO half full int. enable
    const DtFwField  Control_DoneIntEnable; // Done interrupt enable
    const DtFwField  Control_Command;       // Command
    const DtFwField  Control_Unlock ;       // Write unlock
    const DtFwField  Status_All;            // All status fields
    const DtFwField  Status_TxHalfEmptyIntStatus; // Transmit FIFO half empty int.
    const DtFwField  Status_RxHalfFullIntStatus; // Receive FIFO half full int.
    const DtFwField  Status_DoneIntStatus;  // Done interrupt
    const DtFwField  Status_Timeout;        // Timeout
    const DtFwField  Status_IsEnabled;      // Is enabled
    const DtFwField  Status_IsBusy;         // Is busy
    const DtFwField  Status_IsLocked;       // Device locked
    const DtFwField  StartAddress;          // Start address
    const DtFwField  TransferSize;          // Transfer size
    const DtFwField  FifoLoad_TxFifoLoad;   // Transmit FIFO load
    const DtFwField  FifoLoad_RxFifoLoad;   // Receive FIFO load
    const DtFwField  WriteData;             // Write data
    const DtFwField  ReadData;              // Read data
} DtFwbSpiMasterFlash;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_SPI_MASTER_FLASH(OFFSET)                                                 \
{                                                                                        \
    DT_FWB_BLOCK_ID(0 + OFFSET),                    /* Block ID */                       \
    {0x004 + OFFSET, 0, 10, FALSE, FWFIELDOP_R},    /* Frequency of SCK signal */        \
    {0x004 + OFFSET, 16, 12, FALSE, FWFIELDOP_R},   /* Id for SPI Flash/PROM device */   \
    {0x008 + OFFSET, 0, 8, FALSE, FWFIELDOP_R},     /* Page size in log2(bytes) */       \
    {0x008 + OFFSET, 8, 8, FALSE, FWFIELDOP_R},     /* Sector size in log2(bytes) */     \
    {0x008 + OFFSET, 16, 8, FALSE, FWFIELDOP_R},    /* Memory size in log2(bytes) */     \
    {0x00C + OFFSET, 0, 16, FALSE, FWFIELDOP_R},    /* Transmit FIFO size */             \
    {0x00C + OFFSET, 16, 16, FALSE, FWFIELDOP_R},   /* Receive FIFO size */              \
    {0x010 + OFFSET, 0, 3, TRUE, FWFIELDOP_RW},     /* All interrupts */             \
    {0x010 + OFFSET, 0, 1, FALSE, FWFIELDOP_RW},    /* TX FIFO half empty int. enable */ \
    {0x010 + OFFSET, 1, 1, FALSE, FWFIELDOP_RW},    /* RX FIFO half full int. enable */  \
    {0x010 + OFFSET, 2, 1, FALSE, FWFIELDOP_RW},    /* Done interrupt enable */          \
    {0x010 + OFFSET, 8, 3, FALSE, FWFIELDOP_RW},    /* Command */                        \
    {0x010 + OFFSET, 28, 4, FALSE, FWFIELDOP_RW},   /* Device unlock */                  \
    {0x014 + OFFSET, 0, 32, TRUE, FWFIELDOP_RW},    /* All status flags */               \
    {0x014 + OFFSET, 0, 1, FALSE, FWFIELDOP_RC},    /* TX FIFO half empty int. */        \
    {0x014 + OFFSET, 1, 1, FALSE, FWFIELDOP_RC},    /* RX FIFO half full int. */         \
    {0x014 + OFFSET, 2, 1, FALSE, FWFIELDOP_RC},    /* Done interrupt  */                \
    {0x014 + OFFSET, 3, 1, FALSE, FWFIELDOP_RC},    /* Timeout  */                       \
    {0x014 + OFFSET, 4, 1, FALSE, FWFIELDOP_R},     /* Is enabled  */                    \
    {0x014 + OFFSET, 5, 1, FALSE, FWFIELDOP_R},     /* Is flash busy */                  \
    {0x014 + OFFSET, 6, 1, FALSE, FWFIELDOP_R},     /* Is locked */                      \
    {0x018 + OFFSET, 0, 32, TRUE, FWFIELDOP_RW},    /* Start address */                  \
    {0x01C + OFFSET, 0, 32, TRUE, FWFIELDOP_RW},    /* Transfer size */                  \
    {0x020 + OFFSET, 0, 16, FALSE, FWFIELDOP_R},    /* Transmit FIFO load */             \
    {0x020 + OFFSET, 16, 16, FALSE, FWFIELDOP_R},   /* Receive FIFO load */              \
    {0x024 + OFFSET, 0, 32, TRUE, FWFIELDOP_W},     /* Write data */                     \
    {0x028 + OFFSET, 0, 32, TRUE, FWFIELDOP_R},     /* Read data*/                       \
}
const DtFwbSpiMasterFlash FwbSpiMasterFlash = DT_FWB_SPI_MASTER_FLASH(0);
#else
extern const DtFwbSpiMasterFlash FwbSpiMasterFlash;
#endif

// SPI Master Flash status flags (for fast interrupt handling)
#define  DTFWB_SPIMF_STAT_TX_HALFEMPTY  0x01
#define  DTFWB_SPIMF_STAT_RX_HALFFULL   0x02
#define  DTFWB_SPIMF_STAT_DONE          0x04
#define  DTFWB_SPIMF_STAT_INT_MASK      0x07
#define  DTFWB_SPIMF_STAT_TIMEOUT       0x08
#define  DTFWB_SPIMF_STAT_ISENABLED     0x10
#define  DTFWB_SPIMF_STAT_ISBUSY        0x20
#define  DTFWB_SPIMF_STAT_ISLOCKED      0x40

// SPI Master Flash control flags (for fast interrupt handling)
#define  DTFWB_SPIMF_CTRL_TX_HALFEMPTY  0x0001
#define  DTFWB_SPIMF_CTRL_RX_HALFFULL   0x0002
#define  DTFWB_SPIMF_CTRL_DONE          0x0004
#define  DTFWB_SPIMF_CTRL_INT_MASK      0x0007
#define  DTFWB_SPIMF_CTRL_CMD_MASK      0x0700
#define  DTFWB_SPIMF_CTRL_UNLOCK_MASK   0xF0000000


// SPI Master Flash commands
#define DTFWB_SPIMF_CMD_NOP         0x00    // No-operation
#define DTFWB_SPIMF_CMD_PAGE_WRITE  0x01    // Page write
#define DTFWB_SPIMF_CMD_READ        0x02    // Reaed
#define DTFWB_SPIMF_CMD_SECT_ERASE  0x03    // Sector erase
#define DTFWB_SPIMF_CMD_BULK_ERASE  0x04    // Bulk erase
#define DTFWB_SPIMF_CMD_RESET       0x07    // Reset

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbSdiDeserializer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbSdiDeserializer
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('SDSR')
    const DtFwField  SdiStatus_Standard;    // SDI standard
    const DtFwField  SdiStatus_VidFormat;   // Video format
    const DtFwField  SdiStatus_PictRate;    // Picture rate
    const DtFwField  SdiStatus_IsSdiValid;  // Is SDI valid?
    const DtFwField  SdiStatus_IsTrsLocked; // Is TRS locked?
    const DtFwField  SdiStatus_IsVpidValid; // Is VPID valid?
    const DtFwField  SdiStatus_IsVpidBValid;// Is VPID link B valid?
    const DtFwField  CrcErrorCount;         // SDI crc error count
    const DtFwField  VideoPayloadId;        // Video payload identifier (VPID)
    const DtFwField  VideoPayloadIdB;       // Video payload identifier B (VPID B)
}DtFwbSdiDeserializer;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_SDI_DESERIALIZER(OFFSET)                                                  \
{                                                                                        \
    DT_FWB_BLOCK_ID(0 + OFFSET),                    /* Block ID */                       \
    {0x004 + OFFSET, 0, 4, FALSE, FWFIELDOP_R},     /* SDI standard */                   \
    {0x004 + OFFSET, 4, 4, FALSE, FWFIELDOP_R},     /* Video format */                   \
    {0x004 + OFFSET, 8, 4, FALSE, FWFIELDOP_R},     /* Picture rate */                   \
    {0x004 + OFFSET, 12, 1, FALSE, FWFIELDOP_R},    /* Is SDI valid? */                  \
    {0x004 + OFFSET, 13, 1, FALSE, FWFIELDOP_R},    /* Is TRS locked? */                 \
    {0x004 + OFFSET, 14, 1, FALSE, FWFIELDOP_R},    /* Is VPID valid? */                 \
    {0x004 + OFFSET, 15, 1, FALSE, FWFIELDOP_R},    /* Is VPID link B valid? */          \
    {0x008 + OFFSET, 0, 32, TRUE, FWFIELDOP_R},     /* SDI crc error count */            \
    {0x010 + OFFSET, 0, 32, TRUE, FWFIELDOP_R},     /* Video payload id. (VPID) */       \
    {0x014 + OFFSET, 0, 32, TRUE, FWFIELDOP_R},     /* Video payload id. B (VPID B) */   \
}
const DtFwbSdiDeserializer FwbSdiDeserializer = DT_FWB_SDI_DESERIALIZER(0);
#else
extern const DtFwbSdiDeserializer FwbSdiDeserializer;
#endif

// SDI standard
#define DTFWB_SDIDES_SDISTD_SD      0x00    // SD SDI
#define DTFWB_SDIDES_SDISTD_HD      0x01    // HD SDI
#define DTFWB_SDIDES_SDISTD_3GLVLB  0x02    // 3G Level B
#define DTFWB_SDIDES_SDISTD_3GLVLA  0x03    // 3G Level A
#define DTFWB_SDIDES_SDISTD_6GLVLB  0x04    // 6G Level B
#define DTFWB_SDIDES_SDISTD_6GLVLA  0x05    // 6G Level A
#define DTFWB_SDIDES_SDISTD_12GLVLB 0x06    // 12G Level B
#define DTFWB_SDIDES_SDISTD_12GLVLA 0x07    // 12G Level A
#define DTFWB_SDIDES_SDISTD_UNDEF   0x0F    // Undefined

// Video formats
#define DTFWB_SDIDES_VIDFRMT_525I   0x00    // 525i
#define DTFWB_SDIDES_VIDFRMT_625I   0x01    // 625i
#define DTFWB_SDIDES_VIDFRMT_720P   0x02    // 720p
#define DTFWB_SDIDES_VIDFRMT_1080I  0x03    // 1080i
#define DTFWB_SDIDES_VIDFRMT_1080P  0x04    // 1080p
#define DTFWB_SDIDES_VIDFRMT_UNDEF  0x0F    // Undefined

// Picture rate
#define DTFWB_SDIDES_RATE_UNDEF      0      // Undefined
#define DTFWB_SDIDES_RATE_23_98      2      // 23.98 pictures per second
#define DTFWB_SDIDES_RATE_24         3      // 24 pictures per second
#define DTFWB_SDIDES_RATE_47_95      4      // 47.95 pictures per second
#define DTFWB_SDIDES_RATE_25         5      // 25 pictures per second
#define DTFWB_SDIDES_RATE_29_97      6      // 29.97 pictures per second
#define DTFWB_SDIDES_RATE_30         7      // 30 pictures per second
#define DTFWB_SDIDES_RATE_48         8      // 48 pictures per second
#define DTFWB_SDIDES_RATE_50         9      // 50 pictures per second
#define DTFWB_SDIDES_RATE_59_94     10      // 59.94 pictures per second
#define DTFWB_SDIDES_RATE_60        11      // 60 pictures per second

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbSdiAncExtract -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtFwbSdiAncExtract
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('ANCX')
    const DtFwField  Config_FifoSize;       // FIFO size
    const DtFwField  Control_ExtractEnable; // Extract Enable
    const DtFwField  Control_PacketDid;     // DID of packets to extract
    const DtFwField  Status_FifoLoadLatched; // Fifo Load Latched
    const DtFwField  Status_FifoLoad;       // Fifo Load
    const DtFwField  ReadData;              // Read data
} DtFwbSdiAncExtract;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_ANC_EXTRACT(OFFSET)                                                       \
{                                                                                        \
    DT_FWB_BLOCK_ID(0 + OFFSET),                  /* Block ID */                         \
    {0x004 + OFFSET, 0, 16, TRUE, FWFIELDOP_R},   /* FIFO size */                        \
    {0x008 + OFFSET, 0, 1, FALSE, FWFIELDOP_RW},  /* Extract Enable */                   \
    {0x008 + OFFSET, 8, 8, FALSE, FWFIELDOP_RW},  /* DID of packets to extract */        \
    {0x00C + OFFSET, 0, 16, FALSE, FWFIELDOP_R},  /* Fifo Load Latched */                \
    {0x00C + OFFSET, 16, 16, FALSE, FWFIELDOP_R}, /* Fifo Load */                        \
    {0x010 + OFFSET, 0, 32, TRUE, FWFIELDOP_R},   /* Read data */                        \
}

const DtFwbSdiAncExtract FwbSdiAncExtract = DT_FWB_ANC_EXTRACT(0);
#else
extern const DtFwbSdiAncExtract FwbSdiAncExtract;
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbAudioExtract -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbAudioExtract
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('AEX0')
    const DtFwField  ChanPairSelect;        // Audio channel pair selection
    const DtFwField  Status_IsAcpPresent;   // Is audio control packet present?
    const DtFwField  Status_IsAsynchronous; // Is channel pair asynchronous?
    const DtFwField  Status_Rate;           // Audio sample rate
    const DtFwField  Status_IsChan1Active;  // Is audio channel 1 active?
    const DtFwField  Status_IsChan2Active;  // Is audio channel 2 active?
    const DtFwField  Status_IsStatusNew;    // Is audio status new?
    const DtFwField  ChanStatusData;        // Channel status data
} DtFwbAudioExtract;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_AUDIO_EXTRACT(OFFSET)                                                     \
{                                                                                        \
    DT_FWB_BLOCK_ID(0 + OFFSET),                  /* Block ID */                         \
    {0x004 + OFFSET, 0, 6, TRUE, FWFIELDOP_RW},   /* Audio channel pair selection */     \
    {0x008 + OFFSET, 0, 1, FALSE, FWFIELDOP_R},   /* Is audio control packet present? */ \
    {0x008 + OFFSET, 2, 1, FALSE, FWFIELDOP_R},   /* Is channel pair asynchronous? */    \
    {0x008 + OFFSET, 3, 2, FALSE, FWFIELDOP_R},   /* Audio sample rate */                \
    {0x008 + OFFSET, 8, 1, FALSE, FWFIELDOP_R},   /* Is audio channel 1 active? */       \
    {0x008 + OFFSET, 9, 1, FALSE, FWFIELDOP_R},   /* Is audio channel 2 active? */       \
    {0x008 + OFFSET, 31, 1, FALSE, FWFIELDOP_RC}, /* Is audio status new? */             \
    {0x00C + OFFSET, 0, 32, TRUE, FWFIELDOP_R},   /* Channel status data  */             \
}
   
const DtFwbAudioExtract FwbAudioExtract = DT_FWB_AUDIO_EXTRACT(0);

#else
extern const DtFwbAudioExtract FwbAudioExtract;
#endif

// Audio channel pair  sample rate
#define DTFWB_AUDEXT_RATE_48KHZ     0x00    // 48 kHz sample rate
#define DTFWB_AUDEXT_RATE_44_1KHZ   0x01    // 44.1 kHz sample rate
#define DTFWB_AUDEXT_RATE_32KHZ     0x02    // 32 kHz sample rate

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbSdiAvReceiver -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbSdiAvReceiver
{
    const DtFwbBlockId FwbBlockId;              // Block ID ('SRXA')
    const DtFwField  Config_NumAudExtrs;        // Number of audio extractors
    const DtFwField  Control_FrameIntEnable;    // Frame interrupt enable
    const DtFwField  Status_FrameIntStatus;     // Frame interupt status
    const DtFwField  Status_IsAudioStatusNew;   // Is audio status new?
    const DtFwField  Status_IsVideoStatusNew;   // Is video status new?
    const DtFwField  AudioPresence;             // Audio presence per channel
    // Firmware blocks
    const DtFwbSpiMasterSimple  Lmh0394Control; // SDI equalizer control
    const DtFwbSdiDeserializer  SdiDeserializer;// SDI serializer
    const DtFwbSdiAncExtract  AncExtract;       // Ancillary data extractor
    const DtFwbAudioExtract  AudioExtract[8];   // Audio extractors
} DtFwbSdiAvReceiver;

#ifdef DEFINE_FWFIELDS
const DtFwbSdiAvReceiver FwbSdiAvReceiver = 
{
    DT_FWB_BLOCK_ID(0),                     // Block ID
    {0x004, 0, 5, TRUE, FWFIELDOP_R},       // Number of audio extractors
    {0x008, 0, 1, FALSE, FWFIELDOP_RW},     // Frame interrupt enable
    {0x00C, 0, 1, FALSE, FWFIELDOP_RC},     // Frame interupt status
    {0x00C, 1, 1, FALSE, FWFIELDOP_R},      // Is audio status new?
    {0x00C, 2, 1, FALSE, FWFIELDOP_RC},     // Is video status new?
    {0x010, 0, 16, TRUE, FWFIELDOP_R},      // Audio presence per channel
    // Firmware blocks
    DT_FWB_SPI_MASTER_SIMPLE(0x020),        // SDI equalizer control
    DT_FWB_SDI_DESERIALIZER(0x040),         // SDI serializer
    DT_FWB_ANC_EXTRACT(0x060),              // Ancillary data extractor
    {                                       // Audio extractors
        DT_FWB_AUDIO_EXTRACT(0x080), DT_FWB_AUDIO_EXTRACT(0x090),
        DT_FWB_AUDIO_EXTRACT(0x0A0), DT_FWB_AUDIO_EXTRACT(0x0B0),
        DT_FWB_AUDIO_EXTRACT(0x0C0), DT_FWB_AUDIO_EXTRACT(0x0D0),
        DT_FWB_AUDIO_EXTRACT(0x0E0), DT_FWB_AUDIO_EXTRACT(0x0F0)
    }
};
#else
extern const DtFwbSdiAvReceiver FwbSdiAvReceiver;
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbSerialCommCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbSerialCommCtrl
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('UART')
    const DtFwField  Config_TxFifoSize;     // Transmit FIFO size
    const DtFwField  Config_RxFifoSize;     // Receive FIFO size
    const DtFwField  Config_BaudRate;       // Baudrate
    const DtFwField  TxCtrl_EmptyIntEnable; // Transmit FIFO empty interrupt enable
    const DtFwField  TxCtrl_HalfEmptyIntEnable; // Transmit FIFO half empty int. enable
    const DtFwField  TxCtrl_Reset;          // Transmit FIFO reset
    const DtFwField  TxStat_EmptyIntStatus; // Transmit FIFO empty interrupt status
    const DtFwField  TxStat_HalfEmptyIntStatus; // Transmit FIFO half empty int. status
    const DtFwField  TxStat_FifoLoad;       // Transmit FIFO load
    const DtFwField  TxData;                // Transmit FIFO data
    const DtFwField  RxCtrl_DataIdleIntEnable; // Receive FIFO data idle interrupt enable
    const DtFwField  RxCtrl_HalfFullIntEnable; // Receive FIFO half full interrupt enable
    const DtFwField  RxCtrl_Reset;          // Receive FIFO reset
    const DtFwField  RxStat_DataIdleIntStatus; // Receive FIFO data idle interrupt status
    const DtFwField  RxStat_HalfFullIntStatus; // Receive FIFO half full interrupt status
    const DtFwField  RxStat_FifoOverflow;   // Receive FIFO overflow
    const DtFwField  RxStat_FifoLoad;       // Receive FIFO load
    const DtFwField  RxData;                // Receive FIFO data
} DtFwbSerialCommCtrl;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_SERIAL_COMM_CTRL(OFFSET)                                                  \
{                                                                                        \
    DT_FWB_BLOCK_ID(0 + OFFSET),                    /* Block ID */                       \
    {0x004 + OFFSET, 0, 12, FALSE, FWFIELDOP_R},    /* Transmit FIFO size */             \
    {0x004 + OFFSET, 12, 12, FALSE, FWFIELDOP_R},   /* Receive FIFO size */              \
    {0x004 + OFFSET, 24, 4, FALSE, FWFIELDOP_R},    /* Baudrate */                       \
    {0x008 + OFFSET, 0, 1, FALSE, FWFIELDOP_RW},    /* Transmit FIFO empty int. enable */\
    {0x008 + OFFSET, 1, 1, FALSE, FWFIELDOP_RW},    /* Transmit FIFO half empty int. */  \
    {0x008 + OFFSET, 2, 1, FALSE, FWFIELDOP_W},     /* Transmit FIFO reset */            \
    {0x00C + OFFSET, 0, 1, FALSE, FWFIELDOP_RC},    /* Transmit FIFO empty status */     \
    {0x00C + OFFSET, 1, 1, FALSE, FWFIELDOP_RC},    /* Transmit FIFO half empty status */\
    {0x00C + OFFSET, 8, 12, FALSE, FWFIELDOP_R},    /* Transmit FIFO load */             \
    {0x010 + OFFSET, 0, 8, TRUE, FWFIELDOP_W},      /* Transmit FIFO data */             \
    {0x014 + OFFSET, 0, 1, FALSE, FWFIELDOP_RW},    /* Receive FIFO data idle int. */    \
    {0x014 + OFFSET, 1, 1, FALSE, FWFIELDOP_RW},    /* Receive FIFO half full int. */    \
    {0x014 + OFFSET, 2, 1, FALSE, FWFIELDOP_W},     /* Receive FIFO reset */             \
    {0x018 + OFFSET, 0, 1, FALSE, FWFIELDOP_RC},    /* Receive FIFO data idle status */  \
    {0x018 + OFFSET, 1, 1, FALSE, FWFIELDOP_RC},    /* Receive FIFO half full status */  \
    {0x018 + OFFSET, 2, 1, FALSE, FWFIELDOP_RC},    /* Receive FIFO overflow */          \
    {0x018 + OFFSET, 8, 12, FALSE, FWFIELDOP_R},    /* Receive FIFO load */              \
    {0x01C + OFFSET, 0, 8, TRUE, FWFIELDOP_R},      /* Receive FIFO data */              \
}
   
const DtFwbSerialCommCtrl FwbSerialCommCtrl = DT_FWB_SERIAL_COMM_CTRL(0);

#else
extern const DtFwbSerialCommCtrl FwbSerialCommCtrl;
#endif

// UART baudrates
#define DTFWB_UART_BAUDRATE_9K6     0x00    //   9600 Baud
#define DTFWB_UART_BAUDRATE_19K2    0x01    //  19200 Baud
#define DTFWB_UART_BAUDRATE_38K4    0x02    //  38400 Baud
#define DTFWB_UART_BAUDRATE_57K6    0x03    //  57600 Baud
#define DTFWB_UART_BAUDRATE_115K2   0x04    // 115200 Baud


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbEncD7ProSerCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbEncD7ProSerCtrl
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('ED7S')
    const DtFwField  Control_PowerEnable;   // Power enable
    const DtFwField  Control_Reset;         // Reset
    const DtFwField  Status_Is12VPresent;   // Is 12Volt present?
    const DtFwField  Status_IsPowerGood;    // Is Magnum power good?
    const DtFwField  Status_FanFail;        // Fan failure
    const DtFwField  Status_Ext12VAbsent;   // Is 12Volt absent?
    const DtFwField  Status_PowerFail;      // Is Magnum power failed?
    const DtFwField  AvSelect_SourceSelect; // Audio video input source selection
    const DtFwField  AvSelect_AudioSelects[8]; // D7Pro audio selectors
    // Firmware blocks
    const DtFwbSerialCommCtrl  Uarts[2];    // Control and debug UART
    const DtFwbSpiMasterFlash  SpiMasterFlash; // SPI Flash Controller
} DtFwbEncD7ProSerCtrl;

#ifdef DEFINE_FWFIELDS
const DtFwbEncD7ProSerCtrl FwbEncD7ProSerCtrl =
{ 
    DT_FWB_BLOCK_ID(0),                     // Block ID
    {0x004, 0, 1, FALSE, FWFIELDOP_RW},     // Power enable
    {0x004, 1, 1, FALSE, FWFIELDOP_RW},     // Reset
    {0x008, 0, 1, FALSE, FWFIELDOP_R},      // Is 12Volt present?
    {0x008, 1, 1, FALSE, FWFIELDOP_R},      // Is Magnum power good?
    {0x008, 2, 1, FALSE, FWFIELDOP_RC},     // Fan failure
    {0x008, 3, 1, FALSE, FWFIELDOP_RC},     // Is 12Volt absent?
    {0x008, 4, 1, FALSE, FWFIELDOP_RC},     // Is Magnum power failed?
    {0x00C, 0, 2, FALSE, FWFIELDOP_RW},     // Audio video input source selection
    {
        {0x00C, 8, 3, FALSE, FWFIELDOP_RW},     // D7Pro audio pair 1 select
        {0x00C, 11, 3, FALSE, FWFIELDOP_RW},    // D7Pro audio pair 2 select
        {0x00C, 14, 3, FALSE, FWFIELDOP_RW},    // D7Pro audio pair 3 select
        {0x00C, 17, 3, FALSE, FWFIELDOP_RW},    // D7Pro audio pair 4 select
        {0x00C, 20, 3, FALSE, FWFIELDOP_RW},    // D7Pro audio pair 5 select
        {0x00C, 23, 3, FALSE, FWFIELDOP_RW},    // D7Pro audio pair 6 select
        {0x00C, 26, 3, FALSE, FWFIELDOP_RW},    // D7Pro audio pair 7 select
        {0x00C, 29, 3, FALSE, FWFIELDOP_RW},    // D7Pro audio pair 8 select
    },
    // Firmware blocks
    {
        DT_FWB_SERIAL_COMM_CTRL(0x020),     // Control UART
        DT_FWB_SERIAL_COMM_CTRL(0x040),     // Debug UART
    },
    DT_FWB_SPI_MASTER_FLASH(0x080),         // SPI master flash
};

#else
extern const DtFwbEncD7ProSerCtrl FwbEncD7ProSerCtrl;
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbI2cMaster -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbI2cMaster
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('IC2M')
    const DtFwField  Control_DoneIntEnable; // Done Interrupt Enable
    const DtFwField  Status_DoneIntStatus;  // Done Interrupt
    const DtFwField  Status_Busy;           // I2C Bus Busy
    const DtFwField  Status_AddressNack;    // Address not Acknowledged
    const DtFwField  Status_WriteDataNack;  // Write Data not Acknowledged
    const DtFwField  Status_Timeout;        // Clock-Stretching Timeout
    const DtFwField  Status_WriteFifoUnderflow;  // Write FIFO Underflow
    const DtFwField  AddressCount;          // Address/count
    const DtFwField  ReadData;              // Read Data
    const DtFwField  WriteData;             // Write Data
} DtFwbI2cMaster;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_I2C_MASTER(OFFSET)                                                        \
{                                                                                        \
    DT_FWB_BLOCK_ID(0 + OFFSET),                     /* Block ID                    */   \
    {0x008 + OFFSET, 0, 1, TRUE, FWFIELDOP_RW},      /* Done Interrupt Enable       */   \
    {0x00C + OFFSET, 0, 1, FALSE, FWFIELDOP_RC},     /* Done Interrupt              */   \
    {0x00C + OFFSET, 1, 1, FALSE, FWFIELDOP_R},      /* I2C Bus Busy                */   \
    {0x00C + OFFSET, 2, 1, FALSE, FWFIELDOP_R},      /* Address not Acknowledged    */   \
    {0x00C + OFFSET, 3, 1, FALSE, FWFIELDOP_R},      /* Write Data not Acknowledged */   \
    {0x00C + OFFSET, 4, 1, FALSE, FWFIELDOP_R},      /* Clock-Stretching Timeout    */   \
    {0x00C + OFFSET, 5, 1, FALSE, FWFIELDOP_R},      /* Write FIFO Underflow        */   \
    {0x010 + OFFSET, 0, 32, FALSE, FWFIELDOP_RW},    /* Address/count               */   \
    {0x014 + OFFSET, 31, 32, TRUE, FWFIELDOP_R},     /* Read Data                   */   \
    {0x018 + OFFSET, 31, 32, TRUE, FWFIELDOP_W},     /* Write Data                  */   \
}

const DtFwbI2cMaster FwbI2cMaster = DT_FWB_I2C_MASTER(0);
#else
extern const DtFwbI2cMaster FwbI2cMaster;
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbHdmiAdv7610Ctrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbHdmiAdv7610Ctrl
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('7610')
    const DtFwField  Control_Reset;         // Reset
    const DtFwField  Control_IntEnable;     // Interrupt Enable
    const DtFwField  Control_VgaFraming;    // VGA Framing
    const DtFwField  Status_IntStatus;      // Interrupt
    const DtFwbI2cMaster  I2cMaster;        // I2cMaster
} DtFwbHdmiAdv7610Ctrl;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_HDMI_ADV7610(OFFSET)                                                      \
{                                                                                        \
    DT_FWB_BLOCK_ID(0 + OFFSET),                     /* Block ID                    */   \
    {0x004 + OFFSET, 0, 1, FALSE, FWFIELDOP_RW},     /* Reset                       */   \
    {0x004 + OFFSET, 1, 1, FALSE, FWFIELDOP_RW},     /* Interrupt Enable            */   \
    {0x004 + OFFSET, 2, 2, FALSE, FWFIELDOP_RW},     /* VGA Framing                 */   \
    {0x008 + OFFSET, 1, 1, TRUE, FWFIELDOP_R},       /* Interrupt                   */   \
    DT_FWB_I2C_MASTER(0x20 + OFFSET)                 /* I2cMaster                   */   \
}

const DtFwbHdmiAdv7610Ctrl FwbHdmiAdv7610Ctrl = DT_FWB_HDMI_ADV7610(0);
#else
extern const DtFwbHdmiAdv7610Ctrl FwbHdmiAdv7610Ctrl;
#endif

// HDMI VGA Framing
#define DTFWB_HDMI_NO_FRAMING       0x00    // No framing
#define DTFWB_HDMI_FRAME_VGA        0x01    // Convert 640x480p to 525i
#define DTFWB_HDMI_FRAME_WVGA       0x02    // Convert 720x480p to 525i
#define DTFWB_HDMI_FRAME_EDTV       0x03    // Convert 720x576p to 625i

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbSlowClockSynthesizer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbSlowClockSynthesizer
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('SCS0')
    const DtFwField  Config_ReferenceClockRate;  // Reference Clock Rate
    const DtFwField  Control_OutputClockEnable;  // Output Clock Enable
    const DtFwField  Control_PhaseIncrement;  // Phase Increment
    const DtFwField  PhaseLock_InputDivide;   // Input Divide
    const DtFwField  PhaseLock_InputMultiply; // Input Multiply
} DtFwbSlowClockSynthesizer;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_SLOW_CLOCK_SYNTHESIZER(OFFSET)                                            \
{                                                                                        \
    DT_FWB_BLOCK_ID(0 + OFFSET),                     /* Block ID                    */   \
    {0x004 + OFFSET, 0, 12, TRUE, FWFIELDOP_R},      /* Reference Clock Rate        */   \
    {0x008 + OFFSET, 0, 1, FALSE, FWFIELDOP_RW},     /* Output Clock Enable         */   \
    {0x008 + OFFSET, 8, 24, FALSE, FWFIELDOP_RW},    /* Phase Increment             */   \
    {0x00C + OFFSET, 0, 8, FALSE, FWFIELDOP_RW},     /* Input Divide                */   \
    {0x00C + OFFSET, 8, 8, FALSE, FWFIELDOP_RW},     /* Input Multiply              */   \
}

const DtFwbSlowClockSynthesizer FwbSlowClockSynthesizer =DT_FWB_SLOW_CLOCK_SYNTHESIZER(0);
#else
extern const DtFwbSlowClockSynthesizer FwbSlowClockSynthesizer;
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbAudioSrcWithDpll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbAudioSrcWithDpll
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('ASWD')
    const DtFwField  Control_OperationalMode;  // Reset
    const DtFwField  Control_Bypass;        // Bypass
    const DtFwField  Control_InputRate;     // Input Rate
    const DtFwField  Status_Overflow;       // Overflow
    const DtFwField  Status_Underflow;      // Underflow
    DtFwbSlowClockSynthesizer  SlowClockSynthesizer; // SlowClockSynthesizer
} DtFwbAudioSrcWithDpll;

#ifdef DEFINE_FWFIELDS
#define DT_FWB_AUDIO_SRC_WITH_DPLL(OFFSET)                                               \
{                                                                                        \
    DT_FWB_BLOCK_ID(0 + OFFSET),                     /* Block ID                    */   \
    {0x004 + OFFSET, 0, 1, FALSE, FWFIELDOP_RW},     /* Operational Mode            */   \
    {0x004 + OFFSET, 1, 1, FALSE, FWFIELDOP_RW},     /* Bypass                      */   \
    {0x004 + OFFSET, 2, 2, FALSE, FWFIELDOP_RW},     /* Input Rate                  */   \
    {0x008 + OFFSET, 0, 1, FALSE, FWFIELDOP_RC},     /* Overflow                    */   \
    {0x008 + OFFSET, 1, 1, FALSE, FWFIELDOP_RC},     /* Underflow                   */   \
    DT_FWB_SLOW_CLOCK_SYNTHESIZER(0x10 + OFFSET)     /* SlowClockSynthesizer        */   \
}

const DtFwbAudioSrcWithDpll FwbAudioSrcWithDpll = DT_FWB_AUDIO_SRC_WITH_DPLL(0);
#else
extern const DtFwbAudioSrcWithDpll FwbAudioSrcWithDpll;
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbHdmiAdv7610AudioSrc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbHdmiAdv7610AudioSrc
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('HSRC')
    const DtFwbAudioSrcWithDpll  AudioSrcWithDpll; // Audio Sample Rate Converter
    const DtFwbHdmiAdv7610Ctrl  HdmiAdv7610Ctrl; // HDMI Receiver
} DtFwbHdmiAdv7610AudioSrc;

#ifdef DEFINE_FWFIELDS
const DtFwbHdmiAdv7610AudioSrc FwbHdmiAdv7610AudioSrc =
{
    DT_FWB_BLOCK_ID(0),                     // Block ID
    DT_FWB_AUDIO_SRC_WITH_DPLL(0x20),       // Audio Sample Rate Converter
    DT_FWB_HDMI_ADV7610(0x40)               // HDMI Receiver
};
#else
extern const DtFwbHdmiAdv7610AudioSrc FwbHdmiAdv7610AudioSrc;
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbTsRxMemless -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtFwbTsRxMemless
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('TSRX')
    const DtFwField  Config_FifoSize;       // Burst FIFO Size
    const DtFwField  Control_ReceiveEnable; // Receive Enable
    const DtFwField  Control_SyncMode;      // Packet Synchronization Mode
    const DtFwField  Control_ClearMaxFifoLoad; // Clear Maximum Load of Burst FIFO
    const DtFwField  Status_RateOverflow;   // Rate Overflow
    const DtFwField  Status_TransportPacketSize; // Transport Packet Size
    const DtFwField  WriteCount;            // Receive Write Count
    const DtFwField  ValidCount;            // Receive Valid Count
    const DtFwField  FifoLoad_CurrentFifoLoad; // Current Load of Burst FIFO
    const DtFwField  FifoLoad_MaxFifoLoad;  // Maximum Load of Burst FIFO
} DtFwbTsRxMemless;

#ifdef DEFINE_FWFIELDS
const DtFwbTsRxMemless FwbTsRxMemless =
{ 
    DT_FWB_BLOCK_ID(0),                     // Block ID
    {0x004, 0, 14, TRUE, FWFIELDOP_R},      // Burst FIFO Size
    {0x008, 0, 1, FALSE, FWFIELDOP_RW},     // Receive Enable
    {0x008, 1, 2, FALSE, FWFIELDOP_RW},     // Packet Synchronization Mode
    {0x008, 3, 1, FALSE, FWFIELDOP_W},      // Clear Maximum Load of Burst FIFO
    {0x00C, 0, 1, FALSE, FWFIELDOP_RC},     // Rate Overflow
    {0x00C, 1, 2, FALSE, FWFIELDOP_R},     // Transport Packet Size
    {0x010, 0, 32, TRUE, FWFIELDOP_R},      // Receive Write Count
    {0x014, 0, 32, TRUE, FWFIELDOP_R},      // Receive Valid Count
    {0x018, 0, 15, FALSE, FWFIELDOP_R},     // Current Load of Burst FIFO
    {0x018, 16, 15, FALSE, FWFIELDOP_R},    // Maximum Load of Burst FIFO
};
#else
extern const DtFwbTsRxMemless FwbTsRxMemless;
#endif

#define DTFWB_SYNCMODE_AUTO         0       // Sync to either 188 or 204 byte packets
#define DTFWB_SYNCMODE_RAW          1       // Don't try to sync at all
#define DTFWB_SYNCMODE_188          2       // Sync only to 188-byte packets
#define DTFWB_SYNCMODE_204          3       // Sync only to 204-byte packets

#define DTFWB_PACKETSIZE_UNKNOWN    0       // Packetsize not detected
#define DTFWB_PACKETSIZE_188        2       // 188-byte packets detected
#define DTFWB_PACKETSIZE_204        3       // 204-byte packets detected


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FwbFanMonitor -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtFwbFanMonitor
{
    const DtFwbBlockId FwbBlockId;          // Block ID ('FANM')
    const DtFwField  Correction_CorrectionFactorP;  // Correction factor P
    const DtFwField  Correction_CorrectionFactorQ;  // Correction factor Q
    const DtFwField  Control_MinimumRotationRate;   // Minimum Fan Rotation Rate
    const DtFwField  Status_MeasuredRotationRate;   // Measured Fan Rotation Rate
} DtFwbFanMonitor;

#ifdef DEFINE_FWFIELDS
const DtFwbFanMonitor FwbFanMonitor = 
{
    DT_FWB_BLOCK_ID(0),                     // Block ID
    {0x004, 0, 10, FALSE, FWFIELDOP_R},     // Correction factor P
    {0x004, 10, 10, FALSE, FWFIELDOP_R},    // Correction factor Q
    {0x008, 0, 24, TRUE, FWFIELDOP_RW},     // Minimum Fan Rotation Rate
    {0x00C, 0, 24, TRUE, FWFIELDOP_R},      // Measured Fan Rotation Rate
};
#else
extern const DtFwbFanMonitor FwbFanMonitor;
#endif

#endif // #ifndef __DTFWBREGS_H
