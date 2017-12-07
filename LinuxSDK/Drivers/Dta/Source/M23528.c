//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* M23528.c *#*#*#*#*#*#*#*#*# (C) 2012-2015 DekTec
//
// Dta driver - Macom M23528 - Implementation of M23528 interface functions
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

// M23528 registers
#define  M23528_REG_CHIP_ID              0x00
#define  M23528_REG_REVISION             0x01
#define  M23528_REG_GEN_CONFIG           0x02
#define  M23528_REG_INPUT_CONTROL        0x10
#define  M23528_REG_DRIVER1              0x20
#define  M23528_REG_DRIVER2              0x21
#define  M23528_REG_GBL_DRIVER           0x22
#define  M23528_REG_PRE_EMPH             0x23
#define  M23528_REG_LOS_CONTROL          0x31

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#ifdef _DEBUG
static DtStatus  DtaM23528ReadRegister(DtaNonIpPort*  pNonIpPort, Int Dev, Int Addr, 
                                                                          UInt32* pValue);
#endif
static DtStatus  DtaM23528WriteRegister(DtaNonIpPort*  pNonIpPort, Int Dev, Int Addr, 
                                                                            UInt32 Value);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaM235x4Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaM23528Init(DtaNonIpPort* pNonIpPort)
{
    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;

    //DT_ASSERT(pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_DESER_ITF_FPGA_M23544G);

    // Use trace EQ. Equalization is used to compensate for the losses that a high-speed
    // signal accumulates after a transmission across copper traces.
    DT_RETURN_ON_ERROR(DtaM23528WriteRegister(pNonIpPort, 0, M23528_REG_INPUT_CONTROL, 
                                                                                   0x1F));
    
    if (pNonIpPort->m_pDvcData->m_DevInfo.m_HardwareRevision == 0)
    {
        // Powerdown unused port 2. (port1 is used in DTA-2195 HWrev 0)
        DT_RETURN_ON_ERROR(DtaM23528WriteRegister(pNonIpPort, 0, M23528_REG_DRIVER2, 
                                                                                   0x81));
        // Powerup other port
        DT_RETURN_ON_ERROR(DtaM23528WriteRegister(pNonIpPort, 0, M23528_REG_DRIVER1, 
                                                                                   0x80));
    
    } else  {
        // Powerdown unused port 1. (port2 is used in DTA-2195 HWrev > 0)
        DT_RETURN_ON_ERROR(DtaM23528WriteRegister(pNonIpPort, 0, M23528_REG_DRIVER1, 
                                                                                   0x81));
        // Powerup other port and set output swing to +7%
        DT_RETURN_ON_ERROR(DtaM23528WriteRegister(pNonIpPort, 0, M23528_REG_DRIVER2, 
                                                                                   0xF0));
        // Set Pre-emphasis
        DT_RETURN_ON_ERROR(DtaM23528WriteRegister(pNonIpPort, 0, M23528_REG_PRE_EMPH, 
                                                                                   0x70));
    }
    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaM23528ReadRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//
#ifdef _DEBUG
DtStatus  DtaM23528ReadRegister(DtaNonIpPort* pNonIpPort, Int Dev, Int Addr, 
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
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGs296xWriteRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaM23528WriteRegister(DtaNonIpPort* pNonIpPort, Int Dev, Int Addr, 
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
