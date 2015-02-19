//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Lmh1982.h *#*#*#*#*#*#*#*#*# (C) 2012-2015 DekTec
//
// Dta driver - National LMH1982 (Video Clock Generator) controller
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012-2015 DekTec Digital Video B.V.
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

#ifndef __LMH1982_H
#define __LMH1982_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declerations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Master statemachine states
#define  DTA_LMH1982_STATE_INIT         0
#define  DTA_LMH1982_STATE_NOREF        1
#define  DTA_LMH1982_STATE_PLL_LOCKING  2
#define  DTA_LMH1982_STATE_PLL_LOCKED   3
#define  DTA_LMH1982_STATE_ALIGN_TOF1   4
#define  DTA_LMH1982_STATE_ALIGN_TOF2   5
#define  DTA_LMH1982_STATE_FULL_LOCK    6
#define  DTA_LMH1982_STATE_HOLD_OVER1   7
#define  DTA_LMH1982_STATE_HOLD_OVER2   8

#define  DTA_LMH1982_I2C_ADDR           0xDC    // I2C device address for LMH1982

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// DtaLmh1982Regs
typedef struct _DtaLmh1982Regs
{
    // Genlock And Input Reference Control Register
    union
    {
        struct
        {
            UInt8  m_HError     : 3;
            UInt8  m_HoldOver   : 1;
            UInt8  m_RSel       : 1;
            UInt8  m_RSelI2c    : 1;
            UInt8  m_Gnlk       : 1;
            UInt8  m_GnlkIc2    : 1;
        }  Fields;
        UInt8  All;
    }  m_Reg00;

    // Genlock Status And Lock Control Register
    union
    {
    struct
    {
        UInt8  m_RefValid   : 1;
        UInt8  m_SdLock     : 1;
        UInt8  m_HdLock     : 1;
        UInt8  m_LockCtrl   : 5;
    }  Fields;
        UInt8  All;
    }  m_Reg01;

    // Input Control Register
    union
    {
    struct
    {
        UInt8  m_PolVb      : 1;
        UInt8  m_PolHb      : 1;
        UInt8  m_PolVa      : 1;
        UInt8  m_PolHa      : 1;
        UInt8  m_Ref27      : 1;
        UInt8  m_PinOvrd    : 1;
        UInt8  m_Rsv        : 2;
    }  Fields;
        UInt8  All;
    }  m_Reg02;

    // PLL 1 Divider Register (ref-div-sel)
    union
    {
        struct
        {
            UInt8  m_RefDivSel  : 2;
            UInt8  m_Rsv        : 6;
        }  Fields;
        UInt8  All;
    }  m_Reg03;

    // PLL1 Divider Register (feedback divider)
    union
    {
        struct
        {
            UInt16  m_FbDiv      : 13;
            UInt16  m_Rsv        : 3;
        }  Fields;
        UInt16  All;
    }  m_Reg04_05;

    // PLL 4 Charge Pump Current Control Register
    union
    {
        struct
        {
            UInt8  m_Icp4       : 4;
            UInt8  m_Rsv        : 4;
        }  Fields;
        UInt8  All;
    }  m_Reg06;

    UInt8  m_Reg07;

    // Output Clock And TOF Control Register
    union
    {
        struct
        {
            UInt8  m_SdFreq     : 1;
            UInt8  m_SdHiz      : 1;
            UInt8  m_HdFreq     : 2;
            UInt8  m_HdHiz      : 1;
            UInt8  m_TofHiz     : 1;
            UInt8  m_Rsv        : 2;
        }  Fields;
        UInt8  All;
    }  m_Reg08;

    // TOF Reset And Control Register
    union
    {
        struct
        {
            UInt16  m_TofRst     : 13;
            UInt16  m_TofInit    : 1;
            UInt16  m_PolTof     : 1;
            UInt16  m_EnTofRst   : 1;
        }  Fields;
        UInt16  All;
    }  m_Reg09_0A;

    // TOF PLL Register
    union
    {
        struct
        {
            UInt16  m_TofPpl     : 13;
            UInt16  m_TofClk     : 1;
            UInt16  m_Rsv        : 2;
        }  Fields;
        UInt16  All;
    }  m_Reg0B_0C;

    // TOF Lines-Per-Frame Register
    union
    {
        struct
        {
            UInt16  m_TofLpfm    : 12;
            UInt16  m_Rsv        : 4;
        }  Fields;
        UInt16  All;
    }  m_Reg0D_0E;

    // TOF Reference-Lines-Per-Frame Register
    union
    {
        struct
        {
            UInt16  m_RefLpfm    : 12;
            UInt16  m_Rsv        : 4;
        }  Fields;
        UInt16  All;
    }  m_Reg0F_10;

    // TOF PLL Register
    union
    {
        struct
        {
            UInt16  m_TofOffset  : 12;
            UInt16  m_Rsv        : 4;
        }  Fields;
        UInt16  All;
    }  m_Reg11_12;

    // PLL 1 Charge Pump Control Register
    union
    {
        struct
        {
            UInt8  m_Icp1       : 5;
            UInt8  m_Rsv        : 3;
        }  Fields;
        UInt8  All;
    }  m_Reg13;

    // PLL 2,3 Charge Pump Control Register
    union
    {
        struct
        {
            UInt8  m_Icp3       : 4;
            UInt8  m_Icp2       : 4;
        }  Fields;
        UInt8  All;
    }  m_Reg14;

} DtaLmh1982Regs;

#define DTA_LMH1982_NUM_I2C_REGS    21  // Number of 8-bit registers

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ I2C register map +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Genlock And Input Reference Control Register
#define  DTA_LMH1982_IIC_REG00_8B           0x00
#define  DTA_LMH1982_IIC_REG00_H_ERROR_MSK  0x07
#define  DTA_LMH1982_IIC_REG00_H_ERROR_SH   0
#define  DTA_LMH1982_IIC_REG00_HOLD_OVER    3
#define  DTA_LMH1982_IIC_REG00_RSEL         4
#define  DTA_LMH1982_IIC_REG00_RSEL_I2C     5
#define  DTA_LMH1982_IIC_REG00_GNLK         6
#define  DTA_LMH1982_IIC_REG00_GNLK_I2C     7

// Genlock Status And Lock Control Register
#define  DTA_LMH1982_IIC_REG01_8B           0x01
#define  DTA_LMH1982_IIC_REG01_REF_VALID    0
#define  DTA_LMH1982_IIC_REG01_SD_LOCK      1
#define  DTA_LMH1982_IIC_REG01_HD_LOCK      2
#define  DTA_LMH1982_IIC_REG01_LCK_CTRL_MSK 0xF8
#define  DTA_LMH1982_IIC_REG01_LCK_CTRL_SH  3

// Input Control Register
#define  DTA_LMH1982_IIC_REG02_8B           0x02
#define  DTA_LMH1982_IIC_REG02_POL_VB       0
#define  DTA_LMH1982_IIC_REG02_POL_HB       1
#define  DTA_LMH1982_IIC_REG02_POL_VA       2
#define  DTA_LMH1982_IIC_REG02_POL_HA       3
#define  DTA_LMH1982_IIC_REG02_REF_27       4
#define  DTA_LMH1982_IIC_REG02_PIN6_OVRD    5
#define  DTA_LMH1982_IIC_REG02_RSV_MSK      0xC0
#define  DTA_LMH1982_IIC_REG02_RSV_SH       6

// PLL 1 Divider Register (ref-div-sel)
#define  DTA_LMH1982_IIC_REG03_8B           0x03
#define  DTA_LMH1982_IIC_REG03_REF_DIV_SEL_MSK 0x03
#define  DTA_LMH1982_IIC_REG03_REF_DIV_SEL_SH  0
#define  DTA_LMH1982_IIC_REG03_RSV_MSK      0xFC
#define  DTA_LMH1982_IIC_REG03_RSVSH        2

// PLL1 Divider Register (feedback divider)
#define  DTA_LMH1982_IIC_REG04_16B          0x04
#define  DTA_LMH1982_IIC_REG04_FB_DIV_MSK   0x1FFF
#define  DTA_LMH1982_IIC_REG04_FB_DIV_SH    0
#define  DTA_LMH1982_IIC_REG04_RSV_MSK      0xE000
#define  DTA_LMH1982_IIC_REG04_RSVSH        12

// PLL 4 Charge Pump Current Control Register
#define  DTA_LMH1982_IIC_REG06_16B          0x06
#define  DTA_LMH1982_IIC_REG06_ICP4_MSK	    0x000F
#define  DTA_LMH1982_IIC_REG06_RSV_MSK      0xFFF0
#define  DTA_LMH1982_IIC_REG06_RSVSH        4

// Reserved
#define  DTA_LMH1982_IIC_REG07_8B           0x07

// Output Clock And TOF Control Register
#define  DTA_LMH1982_IIC_REG08_8B           0x08
#define  DTA_LMH1982_IIC_REG08_SD_FREQ	    0
#define  DTA_LMH1982_IIC_REG08_SD_HIZ		1
#define  DTA_LMH1982_IIC_REG08_HD_FREQ_MSK  0x0C
#define  DTA_LMH1982_IIC_REG08_HD_FREQ_SH   2
#define  DTA_LMH1982_IIC_REG08_HD_HIZ		4
#define  DTA_LMH1982_IIC_REG08_TOF_HIZ	    5
#define  DTA_LMH1982_IIC_REG08_RSV_MSK      0xC0
#define  DTA_LMH1982_IIC_REG08_RSVSH        6

#define  DTA_LMH1982_IIC_SD_FREQ_27_00      0		// 27.00  Mhz
#define  DTA_LMH1982_IIC_SD_FREQ_67_50      1		// 67.50  Mhz

#define  DTA_LMH1982_IIC_HD_FREQ_74_25      0		// 74.25  Mhz
#define  DTA_LMH1982_IIC_HD_FREQ_74_176     1		// 74.176 Mhz (default)
#define  DTA_LMH1982_IIC_HD_FREQ_148_5      2		// 148.5  Mhz
#define  DTA_LMH1982_IIC_HD_FREQ_148_35     3		// 148.35 Mhz

// TOF Reset And Control Register
#define  DTA_LMH1982_IIC_REG09_16B          0x09
#define  DTA_LMH1982_IIC_REG09_TOF_RST_MSK  0x1FFF
#define  DTA_LMH1982_IIC_REG09_TOF_RST_SH	0
#define  DTA_LMH1982_IIC_REG09_TOF_INIT	    13
#define  DTA_LMH1982_IIC_REG09_POL_TOF	    14
#define  DTA_LMH1982_IIC_REG09_EN_TOF_RST   15

// TOF PLL Register
#define  DTA_LMH1982_IIC_REG0B_16B          0x0B
#define  DTA_LMH1982_IIC_REG0B_TOF_PPL_MSK  0x1FFF
#define  DTA_LMH1982_IIC_REG0B_TOF_PPL_SH   0
#define  DTA_LMH1982_IIC_REG0B_TOF_CLK      13

// TOF Lines-Per-Frame Register
#define  DTA_LMH1982_IIC_REG0D_16B          0x0D
#define  DTA_LMH1982_IIC_REG0D_TOF_LPFM_MSK 0x0FFF
#define  DTA_LMH1982_IIC_REG0D_TOF_LPFM_SH  0

// TOF Reference-Lines-Per-Frame Register
#define  DTA_LMH1982_IIC_REG0F_16B          0x0F
#define  DTA_LMH1982_IIC_REG0F_TOF_LPFM_MSK 0x0FFF
#define  DTA_LMH1982_IIC_REG0F_TOF_LPFM_SH  0

// TOF Offset Register
#define  DTA_LMH1982_IIC_REG11_16B          0x11
#define  DTA_LMH1982_IIC_REG11_TOF_OFFS_MSK 0x0FFF
#define  DTA_LMH1982_IIC_REG11_TOF_OFFS_SH  0

// PLL 1 Charge Pump Control Register
#define  DTA_LMH1982_IIC_REG13_8B           0x13
#define  DTA_LMH1982_IIC_REG13_ICP1_MSK	    0x1F
#define  DTA_LMH1982_IIC_REG13_ICP1_SH	    0

// PLL 2,3 Charge Pump Control Register
#define  DTA_LMH1982_IIC_REG14_8B           0x14
#define  DTA_LMH1982_IIC_REG14_ICP3_MSK	    0x0F
#define  DTA_LMH1982_IIC_REG14_ICP3_SH	    0
#define  DTA_LMH1982_IIC_REG14_ICP2_MSK	    0xF0
#define  DTA_LMH1982_IIC_REG14_ICP2_SH	    4

// DtaLmh1982
typedef struct _DtaLmh1982
{
    DtaDeviceData*  m_pDvcData;

    DtThread  m_ControlThread;  // LMH-1982 controller thread
    Int  m_State;               // State of master state machine
    DtFastMutex  m_StateMutex;  // Mutex to protect the state-machine state
    DtaLmh1982Regs  m_RegCache;  // Cache with copy of LMH-1982 operational registers

} DtaLmh1982;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
void  DtaLmh1982CleanupPre(DtaDeviceData* pDvcData, DtaLmh1982*  pLmh1982Data);
void  DtaLmh1982Cleanup(DtaDeviceData* pDvcData, DtaLmh1982*  pLmh1982Data );
DtStatus  DtaLmh1982Init(DtaDeviceData*  pDvcData, DtaLmh1982*  pLmh1982Data);
DtStatus  DtaLmh1982InitPowerup(DtaLmh1982*  pLmh1982Data);
DtStatus  DtaLmh1982ResetStateMachine(DtaLmh1982*  pLmh1982Data);
DtStatus  DtaLmh1982PowerdownPre(DtaLmh1982*  pLmh1982Data);
void  DtaLmh1982Powerdown(DtaLmh1982*  pLmh1982Data);

#endif // #ifndef __LMH1982_H
