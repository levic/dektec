//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Ad9129.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2015 DekTec
//
//
// Dta driver - Ad9129 - Implementation of Ad9129 interface functions

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DT_AD9129REG_SPICONTROL   0x00    // SPI Communications Control
#define  DT_AD9129REG_POWERDOWN    0x01    // Power Control 
#define  DT_AD9129REG_IRQENA0      0x03    // Interrupt Enable  0
#define  DT_AD9129REG_IRQENA1      0x04    // Interrupt Enable  1
#define  DT_AD9129REG_IRQREQ0      0x05    // Interrupt Request  0
#define  DT_AD9129REG_IRQREQ1      0x06    // Interrupt Request  1
#define  DT_AD9129REG_FRAMEPIN     0x07    // Frame Pin Usage 
#define  DT_AD9129REG_DATACTRL0    0x0A    // Data Receiver Control 0 
#define  DT_AD9129REG_DATACTRL1    0x0B    // Data Receiver Control 1 
#define  DT_AD9129REG_DATACTRL2    0x0C    // Data Receiver Control 2 
#define  DT_AD9129REG_DATACTRL3    0x0D    // Data Receiver Control 3 
#define  DT_AD9129REG_DATASTATUS0  0x0E    // Data Receiver Status 0 
#define  DT_AD9129REG_FIFOCTRL     0x11    // FIFO Control 
#define  DT_AD9129REG_FIFOOFFSET   0x12    // FIFO Offset 
#define  DT_AD9129REG_FIFOTHERMPH0 0x13    // FIFO Thermometer for Phase 0 Status 
#define  DT_AD9129REG_FIFOTHERMPH1 0x14    // FIFO Thermometer for Phase 1 Status 
#define  DT_AD9129REG_FIFOTHERMPH2 0x15    // FIFO Thermometer for Phase 2 Status 
#define  DT_AD9129REG_FIFOTHERMPH3 0x16    // FIFO Thermometer for Phase 3 Status 
#define  DT_AD9129REG_DATAMODECTRL 0x18    // Data Mode Control 
#define  DT_AD9129REG_DECODERCTRL  0x19    // Decoder Control (Program Thermometer Type) 
#define  DT_AD9129REG_SYNCCTRL     0x1A    // Sync Control 
#define  DT_AD9129REG_FSCADJUST1   0x20    // Full-Scale Current Adjust (Lower) 
#define  DT_AD9129REG_FSCADJUST2   0x21    // Full-Scale Current Adjust (Upper) 
#define  DT_AD9129REG_ANALOGCTRL1  0x22    // Analog Control 1 
#define  DT_AD9129REG_ANALOGCTRL2  0x23    // Analog Control 2 
#define  DT_AD9129REG_CLOCKCTRL1   0x30    // Clock Control 1 
#define  DT_AD9129REG_RETIMERCTRL0 0x33    // Retimer Control 0 
#define  DT_AD9129REG_RETIMERCTRL1 0x34    // Retimer Control 1 
#define  DT_AD9129REG_RETIMERTATUS 0x35    // Retimer Status 0 
#define  DT_AD9129REG_SEDCTRL      0x50    // Sample Error Detection (SED) Control 
#define  DT_AD9129REG_SEDRISE0L    0x51    // SED Data Port 0 Rising Edge Status Low 
#define  DT_AD9129REG_SEDRISE0H    0x52    // SED Data Port 0 Rising Edge Status High 
#define  DT_AD9129REG_SEDRISE1L    0x53    // SED Data Port 1 Rising Edge Status Low 
#define  DT_AD9129REG_SEDRISE1H    0x54    // SED Data Port 1 Rising Edge Status High 
#define  DT_AD9129REG_SEDFALL0L    0x55    // SED Data Port 0 Falling Edge Status Low 
#define  DT_AD9129REG_SEDFALL0H    0x56    // SED Data Port 0 Falling Edge Status High 
#define  DT_AD9129REG_SEDFALL1L    0x57    // SED Data Port 1 Falling Edge Status Low 
#define  DT_AD9129REG_SEDFALL1H    0x58    // SED Data Port 1 Falling Edge Status High 
#define  DT_AD9129REG_PARITYCTRL   0x5C    // Parity Control 
#define  DT_AD9129REG_PARRISECOUNT 0x5D    // Parity Rising Edge Count 
#define  DT_AD9129REG_PARFALCOUNT  0x5E    // Parity Falling Edge Count 
#define  DT_AD9129REG_DELAYCTRL0   0x70    // Delay Control  0
#define  DT_AD9129REG_DELAYCTRL1   0x71    // Delay Control  1
#define  DT_AD9129REG_DRIVSTRENGTH 0x7C    // Drive Strength 
#define  DT_AD9129REG_PARTID       0x7F    // Part ID 

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaAd9129Init(DtaNonIpPort*  pNonIpPort);
DtStatus  DtaAd9129StartUpDac(UInt8*  pRfRegs);
DtStatus  DtaAd9129ResetDac(UInt8*  pRfRegs);
DtStatus  DtaAd9129Write(UInt8*  pRfRegs, UInt RegAddr, UInt8 Data);
DtStatus  DtaAd9129SpiWait(UInt8*  pRfRegs);
UInt  DtaRfRegRead(UInt8* pRfRegs, UInt Offset);
UInt  DtaRfRegReadMasked(UInt8* pRfRegs, UInt Offset, UInt Mask, UInt Shift);
void  DtaRfRegWrite(UInt8* pRfRegs, UInt Offset, UInt Value);
void  DtaRfRegWriteMasked(UInt8* pRfRegs, UInt Offset, UInt Mask, UInt Shift, UInt Value);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRfDacInitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtaRfDacInitPowerup(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    // Only needed for DTA-2115's AD9129 RF-DAC
    if (pDvcData->m_DevInfo.m_TypeNumber != 2115)
        return DT_STATUS_OK;

    // Only one port needs to be initialized
    Status = DtaAd9129Init(&pDvcData->m_pNonIpPorts[0]);

    return Status;
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpAd9129Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaAd9129Init(DtaNonIpPort*  pNonIpPort)
{
    Int LoopCount = 5;  // Maximum number of retries
    UInt DacIfStatus = 0;
    UInt8* pRfRegs = (UInt8*)pNonIpPort->m_pRfRegs;
    if (pRfRegs == NULL)
    {
        DtDbgOut(ERR, DAC, "AD9129 RF-registers not initialized"); 
        return DT_STATUS_NOT_INITIALISED;
    }

    do
    {
        DtDbgOut(AVG, DAC, "AD9129 Initializing :%d", LoopCount ); 
        // Select PLL frequency 2400MHz
        DtaRfRegWriteMasked(pRfRegs, DT_RF_REG_RF_CONTROL3, DT_RFCTRL3_CLKFRESEL_MSK, 
                                              DT_RFCTRL3_CLKFRESEL_SH, DT_RFCLKFREQ_2400); 
                      
        // Wait 10ms for lock
        DtSleep(10);  

        // Reset DAC's IF-status 
        DtaRfRegWriteMasked(pRfRegs, DT_RF_REG_DACITF_CONTROL, 
                                                          DT_RFDACITFCTRL_RESETDET_MSK,
                                                          DT_RFDACITFCTRL_RESETDET_SH, 1);

        // Reset DAC
        DT_RETURN_ON_ERROR(DtaAd9129ResetDac(pRfRegs));

        // Perform startup sequence
        DT_RETURN_ON_ERROR(DtaAd9129StartUpDac(pRfRegs));
        
        // Wait 50ms for lock
        DtSleep(50);
        
        // Reset DAC's IF-status 
        DtaRfRegWriteMasked(pRfRegs, DT_RF_REG_DACITF_CONTROL, 
                           DT_RFDACITFCTRL_RESETDET_MSK,  DT_RFDACITFCTRL_RESETDET_SH, 1);

        // MixMode off
        DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_DECODERCTRL, 0x00));

        // Select low pass filter ( 1036MHz)
        DtaRfRegWriteMasked(pRfRegs, DT_RF_REG_RF_CONTROL3, DT_RFCTRL3_FILTERSEL_MSK,
                                             DT_RFCTRL3_FILTERSEL_SH, DT_RFFILTER_BAND_1);  
        DtSleep(20);

        // Set Normal mode
        DtaRfRegWriteMasked(pRfRegs, DT_RF_REG_DACITF_CONTROL, DT_RFDACITFCTRL_MODE_MSK, 
                                      DT_RFDACITFCTRL_MODE_SH, DT_RFDACITF_MODE_NORMAL);

        // SKIP 3 LSB
        DtaRfRegWriteMasked(pRfRegs, DT_RF_REG_DACITF_CONTROL,  DT_RFDACITFCTRL_GAIN_MSK, 
                                        DT_RFDACITFCTRL_GAIN_SH, DT_RFDACITF_GAIN_1_14_3);        
    
        // Check DAC status
        DacIfStatus = DtaRfRegReadMasked(pRfRegs, DT_RF_REG_DACITF_CONTROL, 
                               DT_RFDACITFCTRL_RESETDET_MSK, DT_RFDACITFCTRL_RESETDET_SH);
        LoopCount--;
    } while(DacIfStatus!=0 && LoopCount>=0);

    if (LoopCount < 0)
    {
        DtDbgOut(ERR, DAC, "AD9129 Initialization failed"); 
        return DT_STATUS_FAIL;
    }
    else
    {
        DtDbgOut(AVG, DAC, "AD9129 Initialized"); 
        return DT_STATUS_OK;
    }
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaAd9129StartUpDac -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Execution of DAC start-up sequence
//
DtStatus  DtaAd9129StartUpDac(UInt8*  pRfRegs)
{
    // 4-wire SPI, MSB-first packing, short addressing mode
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_SPICONTROL, 0x00));
    // Enable cross control, cross location = 7 dec, duty cycle correction off
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_CLOCKCTRL1, 0x5C));
    // Set DLL minimum delay = 4 dec, enable DCO
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_DATACTRL2, 0x64));
    // Set clock divider to DCI/512
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_DATACTRL1, 0x39));
    // Set bias power-down
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_POWERDOWN, 0x68));
    // Set PLL mode for 2× mode;FIR40
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_RETIMERCTRL1, 0x5D));
    // Enable bias
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_POWERDOWN, 0x48));
    // Initialize PLL to phasestep = 1 dec
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_RETIMERCTRL0, 0x13));
    // Select PFD, set PLL phase step, keep PLL lost bit cleared
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_RETIMERCTRL0, 0xD8));
    // Deassert the PLL lost bit, keeping the phase step
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_RETIMERCTRL0, 0xD0));
    // Set duty correction bandwidth to lowest
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_DATACTRL3, 0x06));
    // Enable DLL
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_DATACTRL0, 0xC0));
     // Enable 2x interpol filter, select input data format signed and 40 dB OOB rejection
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_DATAMODECTRL, 0xA0));
    // Set full-scale current to 33 mA
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_FSCADJUST1, 0xC6));
    // Complete the setting of FSC
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_FSCADJUST2, 0x03));
     // Enable cross control, cross location = 1 dec,  enable duty cycle correction
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_CLOCKCTRL1, 0x46));
    // Set the FIFO pointers
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_FIFOOFFSET, 0x20));
     // Assert FIFO reset
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_FIFOCTRL, 0x81));
    // Deassert FIFO reset
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_FIFOCTRL, 0x01));
    // Enable IREF (DAC output)
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_POWERDOWN, 0x00));

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaAd9129ResetDac -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaAd9129ResetDac(UInt8*  pRfRegs)
{
    // Reset AD9192
    DtaRfRegWrite(pRfRegs, DT_RF_REG_DACSPI_CONTROL, DT_RFDACSPICTRL_SPIRESET_MSK);
    DT_RETURN_ON_ERROR(DtaAd9129SpiWait(pRfRegs));
    
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_SPICONTROL, 0x00));
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_SPICONTROL, 0x00));
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_SPICONTROL, 0x24));
    DT_RETURN_ON_ERROR(DtaAd9129Write(pRfRegs, DT_AD9129REG_SPICONTROL, 0x00));

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaAd9129Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtaAd9129Write(UInt8*  pRfRegs, UInt RegAddr, UInt8 Data)
{
    // Wait until previous SPI transfer to AD9192 is completed
    DT_RETURN_ON_ERROR(DtaAd9129SpiWait(pRfRegs));

    // Write data
    DtaRfRegWrite(pRfRegs, DT_RF_REG_DACSPI_CONTROL, 
                 DT_RFDACSPICTRL_SPIUPDATE_MSK |                  // Update-request, Write  
                 ((RegAddr&0x7F) << DT_RFDACSPICTRL_SPIADDR_SH) | // Register address
                 (Data << DT_RFDACSPICTRL_SPIDATA_SH));           // Data
    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaAd9129SpiWait -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaAd9129SpiWait(UInt8*  pRfRegs)
{
    Int TimeOut;

    // Wait until AD9192 command is completed
    UInt32 DacCtrlReg;
    DacCtrlReg = DtaRfRegRead(pRfRegs, DT_RF_REG_DACSPI_CONTROL);  
    TimeOut = 50;
    while ((DacCtrlReg & DT_RFDACSPICTRL_SPIREADY_MSK)==0 && TimeOut>0)
    {
        DacCtrlReg = DtaRfRegRead(pRfRegs, DT_RF_REG_DACSPI_CONTROL);  
        TimeOut--;
    }
    // If time-out, try again in 1ms step; wait 50 ms max
    if (TimeOut == 0)
    {
        TimeOut = 50;
        while ((DacCtrlReg & DT_RFDACSPICTRL_SPIREADY_MSK)==0 && TimeOut>0)
        {
            DtSleep(1);
            DacCtrlReg = DtaRfRegRead(pRfRegs, DT_RF_REG_DACSPI_CONTROL);   
            TimeOut--;
        }
    }
    // Fail if transfer takes longer than 50 ms
    if (TimeOut == 0)
    {
        DtDbgOut(ERR, DAC, "AD9129 SPI timeout");   
        return DT_STATUS_TIMEOUT;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRfRegRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt  DtaRfRegRead(UInt8* pRfRegs, UInt Offset)
{
    return READ_UINT(pRfRegs, Offset);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRfRegReadMasked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtaRfRegReadMasked(UInt8* pRfRegs, UInt Offset, UInt Mask, UInt Shift)
{
    return READ_UINT_MASKED(pRfRegs, Offset, Mask, Shift);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRfRegWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void   DtaRfRegWrite(UInt8* pRfRegs, UInt Offset, UInt Value)
{
    WRITE_UINT(Value, pRfRegs, Offset);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRfRegWriteMasked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaRfRegWriteMasked(UInt8* pRfRegs, UInt Offset, UInt Mask, UInt Shift, UInt Value)
{
    WRITE_UINT_MASKED(Value, pRfRegs, Offset, Mask, Shift);
}