//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Ad9789.c *#*#*#*#*#*#*#*#*# (C) 2009-2011 DekTec
//
// Contains AD9789 specific code

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Timeout for SPI transfers
#define  AD9789_SPI_TIMEOUT             50      // 50ms

// AD9789 SPI control register
typedef union _AD9789_CONTROL
{
    struct
    {                                       
        volatile UInt  m_SpiReady    : 1;   //  Ready for new spi cycle
        volatile UInt  m_SpiStart    : 1;   //  Start a SPI cycle (Write-Only bit)
        volatile UInt  m_SpiNumBytes : 2;   //  Number of bytes to transfer (0=1;1=2;etc.)
        volatile UInt  m_SpiRdOrWrn  : 1;   //  Read ('1') or write ('0') transfer
                 UInt  m_Reserved    : 11;  //
        volatile UInt  m_SpiAddr     : 16;  //  SPI ADDRESS
    }  Fields;                              //  For access to individual fields
    UInt32  All;                            //  For access to complete register
} AD9789_CONTROL;


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxAd9789Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// AD9789 write function. Called from DeviceIoControl.
//
Int  Dta1xxAd9789Write(
    DTA1XX_FDO* pFdo, 
    Int PortIndex,
    Int RegAddr,
    Int NumToWrite,
    UInt8* pData)
{
    Int  i, TimeOut;
    UInt32  Ad9789CtrlReg, Ad9789Data = 0;
    Channel*  pCh;
    Int  Status = STATUS_SUCCESS;
    volatile UInt8*  pTxBase;
    AD9789_CONTROL  Ad9789Control;

    // Check parameters
    if (PortIndex<0 || PortIndex >= pFdo->m_NumChannels)
    {
        DTA1XX_LOG(KERN_INFO, "Dta1xxAd9789Write: PortIndex=%d INVALID", PortIndex);
        return STATUS_INVALID_PARAMETER;
    }
    if (RegAddr<0 || RegAddr>=0x54)
    {
        DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxAd9789Write: RegAddr=0x%X INVALID",
                                                                      PortIndex, RegAddr);
        return STATUS_INVALID_PARAMETER;
    }
    if (NumToWrite<1 || NumToWrite>4)
    {
        DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxAd9789Write: NumToWrite=%d INVALID",
                                                                   PortIndex, NumToWrite);
        return STATUS_INVALID_PARAMETER;
    }

    // Set channel pointer and base address of Tx register block
    pCh = &pFdo->m_Channel[PortIndex];
    pTxBase = pCh->m_pRegBase;

    // Wait until previous SPI transfer to AD9789 is completed
    Ad9789CtrlReg = Dta1xxTxGetRfdacCtrlReg(pTxBase);
    TimeOut = 50;
    while ((Ad9789CtrlReg & DTA1XX_TXRFDACCTRL_SPIREADY_MSK)==0 && TimeOut>0)
    {
        Ad9789CtrlReg = Dta1xxTxGetRfdacCtrlReg(pTxBase);
        TimeOut--;
    }
    // If time-out, try again in 1ms step; wait AD9789_SPI_TIMEOUT ms max
    if (TimeOut == 0)
    {
        TimeOut = AD9789_SPI_TIMEOUT;
        while ((Ad9789CtrlReg & DTA1XX_TXRFDACCTRL_SPIREADY_MSK)==0 && TimeOut>0)
        {
            Dta1xxWaitMs(1);
            Ad9789CtrlReg = Dta1xxTxGetRfdacCtrlReg(pTxBase);
            TimeOut--;
        }
    }
    // Fail if transfer takes longer than AD9789_SPI_TIMEOUT ms
    if (TimeOut == 0)
    {
        DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxAd9789Write: SPI NOT READY AFTER 50ms",
                                                                               PortIndex);
        return Status;
    }

    // First write data to data register
    for (i=0; i<NumToWrite; i++)
        Ad9789Data |= ((UInt32)pData[i])<<(i*8);

#if LOG_LEVEL_MODULATION > 1
    DTA1XX_LOG(KERN_INFO, "[%d] Ad9789SpiRegisterWrite: RegAddr=0x%04X, NumToWrite=%d, "
                        "Data=0x%08X", pCh->m_PortIndex, RegAddr, NumToWrite, Ad9789Data);
#endif
    // First write data to data register
    Dta1xxTxSetRfdacDataReg(pTxBase, Ad9789Data);

    // Setup control register for a write transfer
    // NOTE: In MSB bit first mode addresses will decrement, so we must start the 
    // transfer at MSB address
    Ad9789Control.Fields.m_SpiStart = 1;
    Ad9789Control.Fields.m_SpiNumBytes = NumToWrite - 1;
    Ad9789Control.Fields.m_SpiRdOrWrn = 0;
    Ad9789Control.Fields.m_SpiAddr = RegAddr + NumToWrite - 1;

    Dta1xxTxSetRfdacCtrlReg(pTxBase, (Int)(Ad9789Control.All));

    // Wait for transfer to complete
    return Status;
}

