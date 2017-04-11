//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* M23145.c *#*#*#*#*#*#*#*#*# (C) 2012-2015 DekTec
//
// Dta driver - Macom M23145 - Implementation of M23528 interface functions
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

// M23145 registers
#define  M23145_REG_CHIP_ID              0x00
#define  M23145_REG_DIE_REVISION         0x01
#define  M23145_REG_GEN_CONFIG           0x02
#define  M23145_REG_OUTPUT0              0x03
#define  M23145_REG_OUT0_MUX             0x04
#define  M23145_REG_OUTPUT1              0x05
#define  M23145_REG_OUT1_MUX             0x06
#define  M23145_REG_RECLOCKER            0x08
#define  M23145_REG_JIT_DET              0x09
#define  M23145_REG_JIT_DET_ACQ          0x0A
#define  M23145_REG_JIT_DET_EYE          0x0B
#define  M23145_BROADBAND_EQ             0x0C
#define  M23145_BOOST_EQ                 0x0D
#define  M23145_STATUS                   0x0E
#define  M23145_ALARM_MASK               0x0F
#define  M23145_ALARM                    0x10
#define  M23145_RESET                    0x11

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtaM23145ReadRegister(DtaNonIpPort* pNonIpPort, Int Dev, Int Addr, 
                                                                          UInt32* pValue);
static DtStatus  DtaM23145WriteRegister(DtaNonIpPort* pNonIpPort, Int Dev, Int Addr, 
                                                                            UInt32 Value);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaM235x4Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaM23145Init(DtaNonIpPort* pNonIpPort)
{
    UInt32  Data;
    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;

    // Read revision
    DT_RETURN_ON_ERROR(DtaM23145ReadRegister(pNonIpPort, 1, M23145_REG_DIE_REVISION, 
                                                                                  &Data));

    switch (Data)
    {
    case 0x80:
        DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, 0x43, 0x25));
        DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, M23145_RESET, 0x02));
        break;
    case 0x81:
        DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, 0x22, 0x09));
        DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, 0x32, 0x3F));
        DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, 0x3F, 0x03));
        DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, 0x42, 0x2C));
        DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, M23145_RESET, 0x02));
        break;
    case 0x84:
        DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, 0x32, 0x3F));
        DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, 0x3F, 0x03));
        DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, M23145_RESET, 0x02));
        break;
    case 0x85: // No errata
        break;
    }

    // Use adaptive trace EQ
    DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, M23145_BROADBAND_EQ, 0x20));
    DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, M23145_BOOST_EQ, 0x40));
    // Powerdown unsused port 1
    DT_RETURN_ON_ERROR(DtaM23145WriteRegister(pNonIpPort, 1, M23145_REG_OUTPUT1, 0x0C));
    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaM23528ReadRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//
DtStatus  DtaM23145ReadRegister(DtaNonIpPort* pNonIpPort, Int Dev, Int Addr, 
                                                                           UInt32* pValue)
{
    UInt  Timeout=100, SpiCmd=0;

    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;

    // Step 1: write read command to card
    SpiCmd = (Addr << DT_HD_SPICTRL_ADDR_SH) & DT_HD_SPICTRL_ADDR_MSK;
    SpiCmd |= (Dev<<DT_HD_SPICTRL_DEVSEL_SH) & DT_HD_SPICTRL_DEVSEL_MSK;
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
DtStatus  DtaM23145WriteRegister(DtaNonIpPort* pNonIpPort, Int Dev, Int Addr, 
                                                                             UInt32 Value)
{
    UInt  Timeout=100, SpiCmd=0;

    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;

    // Step 1: write write-command to card
    SpiCmd = (Value << DT_HD_SPICTRL_DATA_SH) & DT_HD_SPICTRL_DATA_MSK;
    SpiCmd |= (Addr << DT_HD_SPICTRL_ADDR_SH) & DT_HD_SPICTRL_ADDR_MSK;
    SpiCmd |= (Dev<<DT_HD_SPICTRL_DEVSEL_SH) & DT_HD_SPICTRL_DEVSEL_MSK;
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
