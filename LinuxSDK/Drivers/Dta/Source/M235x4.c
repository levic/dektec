//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* M235x4.c *#*#*#*#*#*#*#*#*# (C) 2012-2015 DekTec
//
// Dta driver - Macom M235x4 - Implementation of M235x4 interface functions
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtaIncludes.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// M235X4 registers
#define  M235X4_REG_CHIP_ID              0x00
#define  M235X4_REG_DIE_REVISION         0x01
#define  M235X4_REG_GEN_CONFIG           0x02
#define  M235X4_REG_OUTPUT0              0x03
#define  M235X4_REG_OUT0_MUX             0x04
#define  M235X4_REG_OUTPUT1              0x05
#define  M235X4_REG_OUT1_MUX             0x06
#define  M235X4_REG_MUTEREF              0x07
#define  M235X4_REG_RECLOCKER            0x08
#define  M235X4_REG_JIT_DETECT           0x09
#define  M235X4_REG_JIT_DET_ACQ          0x0A
#define  M235X4_REG_JIT_DET_EYE          0x0B
#define  M235X4_REG_CABLE_LEN1           0x0C
#define  M235X4_REG_CABLE_LEN0           0x0D
#define  M235X4_REG_STATUS               0x0E
#define  M235X4_REG_ALARM_MASK           0x0F
#define  M235X4_REG_ALARM                0x10
#define  M235X4_REG_RESET                0x11

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtaM235x4ReadRegister(DtaNonIpPort*  pNonIpPort, Int Addr, 
                                                                          UInt32* pValue);
static DtStatus  DtaM235x4WriteRegister(DtaNonIpPort*  pNonIpPort, Int Addr, 
                                                                            UInt32 Value);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaM235x4Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaM235x4Init(DtaNonIpPort* pNonIpPort)
{
    UInt32  Data;
    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;

    DT_ASSERT(pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_DESER_ITF_FPGA_M23544G);

    DT_RETURN_ON_ERROR(DtaM235x4WriteRegister(pNonIpPort, M235X4_REG_JIT_DETECT, 0x24));

    // Read revision
    DT_RETURN_ON_ERROR(DtaM235x4ReadRegister(pNonIpPort, M235X4_REG_DIE_REVISION, &Data));

    switch (Data)
    {
    case 1:
        // The reclocker VCO may not lock at any SDI data rates because its VCO is 
        // out of range. To center the M235x4's VCO locking range, the following register
        // settings must be written to the device:
        //Products Affected: M23544G-12P, die ID (register 01h) = 01h
        //                   M23554G-12P, die ID (register 01h) = 01h
        DT_RETURN_ON_ERROR(DtaM235x4WriteRegister(pNonIpPort, 0x22, 0x09));
        DT_RETURN_ON_ERROR(DtaM235x4WriteRegister(pNonIpPort, 0x32, 0x3F));
        DT_RETURN_ON_ERROR(DtaM235x4WriteRegister(pNonIpPort, 0x3F, 0x03));
        DT_RETURN_ON_ERROR(DtaM235x4WriteRegister(pNonIpPort, 0x42, 0x2C));
        DT_RETURN_ON_ERROR(DtaM235x4WriteRegister(pNonIpPort, M235X4_REG_RESET, 0x2));
        break;
    case 4:
        DT_RETURN_ON_ERROR(DtaM235x4WriteRegister(pNonIpPort, 0x32, 0x3F));
        DT_RETURN_ON_ERROR(DtaM235x4WriteRegister(pNonIpPort, 0x3F, 0x03));
        DT_RETURN_ON_ERROR(DtaM235x4WriteRegister(pNonIpPort, M235X4_REG_RESET, 0x2));
        break;
    case 5: // No errata.
        break;
    case 20: // No errata
        break;
    }

    // Use lower output swing of 400mVppd for port 0
    DT_RETURN_ON_ERROR(DtaM235x4WriteRegister(pNonIpPort, M235X4_REG_OUTPUT0, 0x0D));
    // Powerdown unsused port 1
    DT_RETURN_ON_ERROR(DtaM235x4WriteRegister(pNonIpPort, M235X4_REG_OUTPUT1, 0x0C));
    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaM235x4ReadRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaM235x4ReadRegister(DtaNonIpPort* pNonIpPort, Int Addr, UInt32* pValue)
{
    UInt  Timeout=100, SpiCmd=0;

    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;

    // Step 1: write read command to card
    SpiCmd = (Addr << DT_HD_SPICTRL_ADDR_SH) & DT_HD_SPICTRL_ADDR_MSK;
    //SpiCmd |= (0<<DT_HD_SPICTRL_AUTOINC_SH) & DT_HD_SPICTRL_AUTOINC_MSK;
    SpiCmd |= (1<<DT_HD_SPICTRL_READ_SH) & DT_HD_SPICTRL_READ_MSK;
    SpiCmd |= (1<<DT_HD_SPICTRL_START_SH) & DT_HD_SPICTRL_START_MSK;

    WRITE_UINT(SpiCmd, pNonIpPort->m_pRxRegs, DT_HD_REG_SPICTRL);

    // Step 2: wait for done bit
    SpiCmd = READ_UINT(pNonIpPort->m_pRxRegs, DT_HD_REG_SPICTRL);
    while ((SpiCmd & DT_HD_SPICTRL_DONE_MSK) == 0)
    {
        DtSleep(1);
        SpiCmd = READ_UINT(pNonIpPort->m_pRxRegs, DT_HD_REG_SPICTRL);
        Timeout--;
        if (Timeout <= 0)
            return DT_STATUS_TIMEOUT;
    }
    *pValue = (UInt32)((SpiCmd & DT_HD_SPICTRL_DATA_MSK) >> DT_HD_SPICTRL_DATA_SH);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGs296xWriteRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaM235x4WriteRegister(DtaNonIpPort* pNonIpPort, Int Addr, UInt32 Value)
{
    UInt  Timeout=100, SpiCmd=0;

    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;

    // Step 1: write write-command to card
    SpiCmd = (Value << DT_HD_SPICTRL_DATA_SH) & DT_HD_SPICTRL_DATA_MSK;
    SpiCmd |= (Addr << DT_HD_SPICTRL_ADDR_SH) & DT_HD_SPICTRL_ADDR_MSK;
    SpiCmd |= (0<<DT_HD_SPICTRL_AUTOINC_SH) & DT_HD_SPICTRL_AUTOINC_MSK;
    SpiCmd |= (0<<DT_HD_SPICTRL_READ_SH) & DT_HD_SPICTRL_READ_MSK;
    SpiCmd |= (1<<DT_HD_SPICTRL_START_SH) & DT_HD_SPICTRL_START_MSK;

    DtDbgOut(MAX, DTA, "Addr=0x%04X, Value=0x%04X, SpiCmd=0x%08X", Addr, Value, SpiCmd);
    
    WRITE_UINT(SpiCmd, pNonIpPort->m_pRxRegs, DT_HD_REG_SPICTRL);

    // Step 2: wait for done bit
    SpiCmd = READ_UINT(pNonIpPort->m_pRxRegs, DT_HD_REG_SPICTRL);
    while ((SpiCmd & DT_HD_SPICTRL_DONE_MSK) == 0)
    {
        DtSleep(1);
        SpiCmd = READ_UINT(pNonIpPort->m_pRxRegs, DT_HD_REG_SPICTRL);
        Timeout--;
        if (Timeout <= 0)
            return DT_STATUS_TIMEOUT;
    }
    return DT_STATUS_OK;
}
