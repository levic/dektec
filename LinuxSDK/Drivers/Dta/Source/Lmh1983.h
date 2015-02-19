//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Lmh1983.h *#*#*#*#*#*#*#*#*# (C) 2013-2015 DekTec
//
// Dta driver - National LMH1983 (Video Clock Generator) controller

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2013-2015 DekTec Digital Video B.V.
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

#ifndef __LMH1983_H
#define __LMH1983_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declerations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Master statemachine states
#define  DTA_LMH1983_STATE_INIT             0
#define  DTA_LMH1983_STATE_NOREF            1
#define  DTA_LMH1983_STATE_LOCKING          2
#define  DTA_LMH1983_STATE_LOCKED           3
#define  DTA_LMH1983_STATE_HOLDOVER         4


#define  DTA_LMH1983_I2C_ADDR           0xCC    // I2C device address for LMH1983

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLhm1983Regs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// 
// Struct representing the LMH1983 register map
//
// NOTE: only the the registers we use in the code have all bit fields defined
//
//typedef struct _DtaLhm1983Regs
//{
//    // Device Status Input Reference
//    union
//    {
//        struct
//        {
//            UInt8  m_Reserved       : 1;
//            UInt8  m_LostHsync      : 1;
//            UInt8  m_LorStatus      : 1;
//            UInt8  m_HOnly          : 1;
//            UInt8  m_HSyncStatus    : 1;
//            UInt8  m_InpPolarity    : 1;
//            UInt8  m_AnalogRef      : 1;
//            UInt8  m_Interlaced     : 1;
//        }  Fields;
//        UInt8  All;
//    }  m_Reg00;
//
//    // Device Status
//    union
//    {
//        struct
//        {
//            UInt8  m_Reserved       : 4;
//            UInt8  m_HoldOver       : 1;
//            UInt8  m_WrongFormat    : 1;
//            UInt8  m_AlignStatus    : 1;
//            UInt8  m_LockStatus     : 1;
//        }  Fields;
//        UInt8  All;
//    }  m_Reg01;
//
//    UInt8  m_Reg02;
//    UInt8  m_Reg03;
//    UInt8  m_Reg04;
//
//    // Device Control
//    union
//    {
//        struct
//        {
//            UInt8  m_GlobalOutpEna  : 1;
//            UInt8  m_Force148       : 1;
//            UInt8  m_LorMode        : 1;
//            UInt8  m_Pll1Mode       : 2;
//            UInt8  m_EnaAfd         : 1;
//            UInt8  m_PowerDown      : 1;
//            UInt8  m_SoftReset      : 1;
//        }  Fields;
//        UInt8  All;
//    }  m_Reg05;
//    
//} DtaLhm1983Regs;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ I2C register map +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#define  DTA_LMH1983_IIC_REG00      0x00    // Device Status Input Reference
#define  DTA_LMH1983_IIC_REG01      0x01    // Device Status
#define  DTA_LMH1983_IIC_REG02      0x02    // PLL Lock and Output Alignment Status
#define  DTA_LMH1983_IIC_REG03      0x03    // Revision ID
#define  DTA_LMH1983_IIC_REG04      0x04    // Reserved
#define  DTA_LMH1983_IIC_REG05      0x05    // Device Control
#define  DTA_LMH1983_IIC_REG06      0x06    // Input Polarity
#define  DTA_LMH1983_IIC_REG07      0x07    // Output Mode – PLL2 Format
#define  DTA_LMH1983_IIC_REG08      0x08    // Output Mode – PLL3 Format
#define  DTA_LMH1983_IIC_REG09      0x09    // Output Mode – Misc
#define  DTA_LMH1983_IIC_REG0A      0x0A    // Output Buffer Control
#define  DTA_LMH1983_IIC_REG0B      0x0B    // Output Frame Control - Offset1_MSB
#define  DTA_LMH1983_IIC_REG0C      0x0C    // Output Frame Control - Offset1_LSB
#define  DTA_LMH1983_IIC_REG0D      0x0D    // Output Frame Control - Offset2_MSB
#define  DTA_LMH1983_IIC_REG0E      0x0E    // Output Frame Control - Offset2_LSB
#define  DTA_LMH1983_IIC_REG0F      0x0F    // Output Frame Control - Offset3_MSB
#define  DTA_LMH1983_IIC_REG10      0x10    // Output Frame Control - Offset1_LSB
#define  DTA_LMH1983_IIC_REG11      0x11    // Alignment Control – TOF1
#define  DTA_LMH1983_IIC_REG12      0x12    // Alignment Control – TOF2
#define  DTA_LMH1983_IIC_REG13      0x13    // Alignment Control – TOF3
#define  DTA_LMH1983_IIC_REG14      0x14    // Alignment Control – AFS
#define  DTA_LMH1983_IIC_REG15      0x15    // Loss of Alignment Control
#define  DTA_LMH1983_IIC_REG16      0x16    // LOR Control – Holdover Sampled Voltage MSB
#define  DTA_LMH1983_IIC_REG17      0x17    // LOR Control – Holdover Sampled Voltage LSB
#define  DTA_LMH1983_IIC_REG18      0x18    // LOR Control Free-run Control Voltage MSB
#define  DTA_LMH1983_IIC_REG19      0x19    // LOR Control Free-run Control Voltage LSB
#define  DTA_LMH1983_IIC_REG1A      0x1A    // LOR Control – ADC & DAC Disable
#define  DTA_LMH1983_IIC_REG1B      0x1B    // Loss of Reference Threshold
#define  DTA_LMH1983_IIC_REG1C      0x1C    // Loss of Lock Threshold
#define  DTA_LMH1983_IIC_REG1D      0x1D    // Mask Control – PLL Lock and Output Align
#define  DTA_LMH1983_IIC_REG1E      0x1E    // Reserved
#define  DTA_LMH1983_IIC_REG1F      0x1F    // Reserved
#define  DTA_LMH1983_IIC_REG20      0x20    // Input Format
#define  DTA_LMH1983_IIC_REG21      0x21    // Output Frame Lookup – Input Vsync Code
#define  DTA_LMH1983_IIC_REG22      0x22    // Output Frame Lookup – PLL2 Vsync Code
#define  DTA_LMH1983_IIC_REG23      0x23    // Output Frame Lookup – PLL3 Vsync Code
#define  DTA_LMH1983_IIC_REG24      0x24    // Reserved
#define  DTA_LMH1983_IIC_REG25      0x25    // PLL1 Advanced Control
#define  DTA_LMH1983_IIC_REG26      0x26    // PLL1 Advanced FastLock Delay
#define  DTA_LMH1983_IIC_REG27      0x27    // PLL1 Advanced Control Fastlock CP Current
#define  DTA_LMH1983_IIC_REG28      0x28    // PLL1 Advanced Control Charge Pump Current
#define  DTA_LMH1983_IIC_REG29      0x29    // PLL1 Advanced Control R Counter MSB
#define  DTA_LMH1983_IIC_REG2A      0x2A    // PLL1 Advanced Control R Counter LSB
#define  DTA_LMH1983_IIC_REG2B      0x2B    // PLL1 Advanced Control N Counter MSB
#define  DTA_LMH1983_IIC_REG2C      0x2C    // PLL1 Advanced Control N Counter LSB
#define  DTA_LMH1983_IIC_REG2D      0x2D    // PLL1 Advanced Control Lock Step Size
#define  DTA_LMH1983_IIC_REG2E      0x2E    // PLL2 Advanced Control Main
#define  DTA_LMH1983_IIC_REG2F      0x2F    // PLL2 Advanced Control Charge Pump Current
#define  DTA_LMH1983_IIC_REG30      0x30    // PLL2 Advanced Control VCO Range
#define  DTA_LMH1983_IIC_REG31      0x31    // PLL3 Advanced Control Main
#define  DTA_LMH1983_IIC_REG32      0x32    // PLL3 Advanced Control Charge Pump Current
#define  DTA_LMH1983_IIC_REG33      0x33    // PLL3 Advanced Control VCO Range
#define  DTA_LMH1983_IIC_REG34      0x34    // PLL4 Advanced Control Main
#define  DTA_LMH1983_IIC_REG35      0x35    // PLL4 Advanced Control Charge Pump Current
#define  DTA_LMH1983_IIC_REG36      0x36    // PLL4 Advanced Control R Counter
#define  DTA_LMH1983_IIC_REG37      0x37    // PLL4 Advanced Control N Counter MSB
#define  DTA_LMH1983_IIC_REG38      0x38    // PLL4 Advanced Control N Counter LSB
#define  DTA_LMH1983_IIC_REG39      0x39    // PLL4 Advanced Control VCO Range
#define  DTA_LMH1983_IIC_REG4A      0x4A    // TOF4 Advanced Control ACLK
#define  DTA_LMH1983_IIC_REG4B      0x4B    // Reserved
#define  DTA_LMH1983_IIC_REG4C      0x4C    // Reserved
#define  DTA_LMH1983_IIC_REG4D      0x4D    // Reserved
#define  DTA_LMH1983_IIC_REG4E      0x4E    // Reserved
#define  DTA_LMH1983_IIC_REG4F      0x4F    // Reserved
#define  DTA_LMH1983_IIC_REG40      0x50    // Reserved
#define  DTA_LMH1983_IIC_REG51      0x51    // User Auto Format 27M High Value MSB
#define  DTA_LMH1983_IIC_REG52      0x52    // User Auto Format 27M High Value LSB
#define  DTA_LMH1983_IIC_REG53      0x53    // User Auto Format 27M Low Value MSB
#define  DTA_LMH1983_IIC_REG54      0x54    // User Auto Format 27M Low Value LSB
#define  DTA_LMH1983_IIC_REG55      0x55    // User Auto Format R Divider MSB
#define  DTA_LMH1983_IIC_REG56      0x56    // User Auto Format R Divider LSB
#define  DTA_LMH1983_IIC_REG57      0x57    // User Auto Format N Divider MSB
#define  DTA_LMH1983_IIC_REG58      0x58    // User Auto Format N Divider LSB
#define  DTA_LMH1983_IIC_REG59      0x59    // User Auto Format Charge Pump Current
#define  DTA_LMH1983_IIC_REG5A      0x5A    // User Auto Format LPF MSB
#define  DTA_LMH1983_IIC_REG5B      0x5B    // User Auto Format LPF LSB
#define  DTA_LMH1983_IIC_REG5C      0x5C    // User Auto Format AFS
#define  DTA_LMH1983_IIC_REG5D      0x5D    // User Auto Format Misc

// DtaLmh1983
typedef struct _DtaLmh1983
{
    DtaDeviceData*  m_pDvcData;

    DtThread  m_ControlThread;  // LMH-1982 controller thread
    Int  m_State;               // State of master state machine
    UInt64  m_EnteredStateTime; // Time at which point the state was entered
    DtFastMutex  m_StateMutex;  // Mutex to protect the state-machine state


} DtaLmh1983;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

DtStatus  DtaLmh1983Init(DtaDeviceData*  pDvcData, DtaLmh1983*  pLmh1983Data);
DtStatus  DtaLmh1983InitPowerup(DtaLmh1983* pLmh1983Data);
DtStatus  DtaLmh1983ResetStateMachine(DtaLmh1983*  pLmh1983Data);
DtStatus  DtaLmh1983PowerdownPre(DtaLmh1983*  pLmh1983Data);
DtStatus  DtaLmh1983SetDevCtrl(DtaLmh1983* pLmh1983Data);
DtStatus  DtaLmh1983SetVcxoValue(DtaLmh1983* pLmh1983Data, Int  VcxoValue);

#endif // #ifndef __LMH1983_H
