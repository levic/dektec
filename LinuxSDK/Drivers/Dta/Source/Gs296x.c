//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Gs296x.c *#*#*#*#*#*#*#*#*# (C) 2012-2016 DekTec
//
// Dta driver - Gennum GS296x - Implementation of GS296x interface functions
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012-2016 DekTec Digital Video B.V.
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

// GS2961 registers
#define  GS2961_REG_IOPROC_1             0x00
#define  GS2961_REG_ERROR_STAT1          0x02
#define  GS2961_REG_ERROR_STAT2          0x03
#define  GS2961_REG_EDH_FLAG_IN          0x04
#define  GS2961_REG_DATA_FORMAT_D1       0x06
#define  GS2961_REG_DATA_FORMAT_D2       0x07
#define  GS2961_REG_VIDEO_FORMAT_352_A_1 0x19
#define  GS2961_REG_VIDEO_FORMAT_352_B_1 0x1A
#define  GS2961_REG_RASTER_STRUCT_1      0x1F
#define  GS2961_REG_RASTER_STRUCT_2      0x20
#define  GS2961_REG_RASTER_STRUCT_3      0x21
#define  GS2961_REG_RASTER_STRUCT_4      0x22
#define  GS2961_REG_RATE_SEL             0x24

// GS2962 registers
#define  GS2962_REG_IOPROC               0x00
#define  GS2962_REG_VIDEO_FORMAT_351_OUT_WORD_1 0x0A
#define  GS2962_REG_VIDEO_FORMAT_351_OUT_WORD_2 0x0B

// GS2962_REG_IOPROC flags
#define GS2962_IOPROC_SMPTE_352M_INS     0x0040

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtaGs296xReadRegister(DtaNonIpPort*  pNonIpPort, Int Addr, 
                                                                          UInt32* pValue);
static DtStatus  DtaGs296xWriteRegister(DtaNonIpPort*  pNonIpPort, Int Addr, 
                                                                            UInt32 Value);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGs2961Enable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaGs2961Enable(DtaNonIpPort*  pNonIpPort, Bool  EnaAsiMode, Bool  EnaSdiProc)
{
    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;

    DT_ASSERT(pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_DESER_ITF_GS2961);

    // Toggle reset
    DtaRegHdCtrl1SetIoReset(pNonIpPort->m_pRxRegs, 1);
    DtSleep(5);
    DtaRegHdCtrl1SetIoReset(pNonIpPort->m_pRxRegs, 0);
    DtSleep(5);

    // ASI or SDI mode
    if (EnaAsiMode)
    {
        // For ASI, set the mode fixed to 27Mhz (SD-rate)
        DT_RETURN_ON_ERROR(DtaGs296xWriteRegister(pNonIpPort, 
                                                            GS2961_REG_RATE_SEL, 0x0001));
    }
    else
    {
        UInt  IoProc = 0x1A18;
        // For SDI, enable auto rate (SD, HD or 3G) detection
        DT_RETURN_ON_ERROR(DtaGs296xWriteRegister(pNonIpPort, 
                                                            GS2961_REG_RATE_SEL, 0x0004));

        // Set IO-processing:
        // Enable:   
        // Disable:  ANC_CSUM_INS_DS1, ILLEGAL_WORD_REMAP_DS1, TRS_WORD_REMAP_DS1,
        //           ANC_DATA_EXT, EDH_CRC_INS, EDH_FLAG_UPDATE
        // Disable on EnaSdiProc flag: TRS_INS_DS1, LNUM_INS_DS1, CRC_INS_DS1, H_CONFIG
        // Other bits are set to their default (see GS2961 datasheet)

        if (!EnaSdiProc)
            IoProc |= 0x0007;

        DT_RETURN_ON_ERROR(DtaGs296xWriteRegister(pNonIpPort, 
                                                            GS2961_REG_IOPROC_1, IoProc));
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGs2961GetVideoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaGs2961GetVideoStd(DtaNonIpPort*  pNonIpPort, Int*  pVidStd)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  Ds1Reg=0, Raster4Reg=0;
    Int  GsVidStd = 0;
    Bool  IsFrac=FALSE, IsStdLock=FALSE, IsInterlaced=FALSE;
    UInt32  VpidA=0, VpidB=0;
    UInt32  Vpid=0;

    DT_ASSERT(pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_DESER_ITF_GS2961);

    // Read data-format 1 register
    Status = DtaGs296xReadRegister(pNonIpPort, GS2961_REG_DATA_FORMAT_D1, &Ds1Reg);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Read raster struct 4 register
    Status = DtaGs296xReadRegister(pNonIpPort, GS2961_REG_RASTER_STRUCT_4, &Raster4Reg);
    if (!DT_SUCCESS(Status))
        return Status;

    // Read SMPTE 352M VPID registers
    Status = DtaGs296xReadRegister(pNonIpPort, GS2961_REG_VIDEO_FORMAT_352_A_1, &VpidA);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtaGs296xReadRegister(pNonIpPort, GS2961_REG_VIDEO_FORMAT_352_B_1, &VpidB);
    if (!DT_SUCCESS(Status))
        return Status;
    Vpid = (VpidA<<24) | ((VpidA&0xFF00)<<8) | ((VpidB&0xFF)<<8) | (VpidB>>8);

    // Check if standard lock flag (bit 12) is set
    IsStdLock = (Raster4Reg & 0x1000)!=0;
    if (!IsStdLock)
    {
        *pVidStd = DT_VIDSTD_UNKNOWN;
        return DT_STATUS_OK;
    }
    // Check for fractional mode (bit 13)
    IsFrac = (Raster4Reg & 0x2000)!=0;
    // Check for interlaced mode (bit 11)
    IsInterlaced = (Raster4Reg & 0x0800)!=0;
    
    // Convert to internal value
    GsVidStd = (Ds1Reg>>8) & 0x3F;
    
    // Assume non-psf standard by default. Only switch if vpid matches detected
    // standard and indicates psf.
    switch (GsVidStd)
    {
    case 0x00: *pVidStd = IsFrac ? DT_VIDSTD_720P59_94 : DT_VIDSTD_720P60; break;
    case 0x02: *pVidStd = IsFrac ? DT_VIDSTD_720P29_97 : DT_VIDSTD_720P30; break;
    case 0x04: *pVidStd = DT_VIDSTD_720P50; break;
    case 0x06: *pVidStd = IsFrac ? DT_VIDSTD_UNKNOWN : DT_VIDSTD_720P25; break;
    case 0x08: *pVidStd = IsFrac ? DT_VIDSTD_720P23_98 : DT_VIDSTD_720P24; break;
    case 0x0A:
        *pVidStd = IsFrac ? DT_VIDSTD_1080I59_94 : DT_VIDSTD_1080I60;
        if (!IsFrac && (Vpid&0xFFCF0000)==0x85470000)
            *pVidStd = DT_VIDSTD_1080PSF30;
        else if (IsFrac && (Vpid&0xFFCF0000)==0x85460000)
            *pVidStd = DT_VIDSTD_1080PSF29_97;
        break;
    case 0x0B: *pVidStd = IsFrac ? DT_VIDSTD_1080P29_97 : DT_VIDSTD_1080P30; break;
    case 0x0C:
        *pVidStd = DT_VIDSTD_1080I50;
        if ((Vpid&0xFFCF0000) == 0x85450000)
            *pVidStd = DT_VIDSTD_1080PSF25;
        break;
    case 0x0D: *pVidStd = DT_VIDSTD_1080P25; break;
    case 0x10: *pVidStd = IsFrac ? DT_VIDSTD_1080P23_98 : DT_VIDSTD_1080P24; break;
    case 0x11:
        *pVidStd = DT_VIDSTD_UNKNOWN;
        if (!IsFrac && (Vpid&0xFFCF0000)==0x85430000)
            *pVidStd = DT_VIDSTD_1080PSF24;
        else if (IsFrac && (Vpid&0xFFCF0000)==0x85420000)
            *pVidStd = DT_VIDSTD_1080PSF23_98;
        break;
    case 0x16: *pVidStd = DT_VIDSTD_525I59_94; break;
    case 0x18: *pVidStd = DT_VIDSTD_625I50; break;
    default:   *pVidStd = DT_VIDSTD_UNKNOWN; break;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGs2962Enable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaGs2962Enable(DtaNonIpPort*  pNonIpPort)
{
    UInt  IoProc;
    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;

    DT_ASSERT(pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_SER_ITF_GS2962);

    // Toggle reset
    DtaRegHdCtrl1SetIoReset(pNonIpPort->m_pTxRegs, 1);
    DtSleep(5);
    DtaRegHdCtrl1SetIoReset(pNonIpPort->m_pTxRegs, 0);
    DtSleep(5);

    // Set IO-processing:
    // Enable:   SMPTE_352M_INS, CRC_INS, LNUM_INS, TRS_INS
    // Disable:  ANC_INS
    //
    // Other bits are set to their default value (see GS2962 datasheet)
    IoProc = 0x0A00;
    if (!DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
    {
        // Disable: ILLEGAL_WORD_REMAP, ANC_CSUM_INS,
        IoProc |= 0x0028;
    }
    // Set VPID insertion bytes
    DtaGs2962SetVpid(pNonIpPort, DtaRegHdSdiFormat1Get(pNonIpPort->m_pTxRegs));
    return DtaGs296xWriteRegister(pNonIpPort, GS2962_REG_IOPROC, IoProc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGs2962SetVpid -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaGs2962SetVpid(DtaNonIpPort*  pNonIpPort, UInt  Vpid)
{
    UInt16  VpidA, VpidB;
    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;
    
    DT_ASSERT(pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_SER_ITF_GS2962);
    
    VpidA = Vpid&0xFFFF;
    VpidB = Vpid>>16;
    DT_RETURN_ON_ERROR(DtaGs296xWriteRegister(pNonIpPort,
                                          GS2962_REG_VIDEO_FORMAT_351_OUT_WORD_1, VpidA));
    return DtaGs296xWriteRegister(pNonIpPort, GS2962_REG_VIDEO_FORMAT_351_OUT_WORD_2,
                                                                                   VpidB);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGs296xReadRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaGs296xReadRegister(DtaNonIpPort*  pNonIpPort, Int Addr, UInt32*  pValue)
{
    UInt  Timeout=100, SpiCmd=0;

    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;

    // Step 1: write read command to card
    SpiCmd = (Addr << DT_HD_SPICTRL_ADDR_SH) & DT_HD_SPICTRL_ADDR_MSK;
    SpiCmd |= (0<<DT_HD_SPICTRL_AUTOINC_SH) & DT_HD_SPICTRL_AUTOINC_MSK;
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
DtStatus  DtaGs296xWriteRegister(DtaNonIpPort*  pNonIpPort, Int Addr, UInt32  Value)
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

