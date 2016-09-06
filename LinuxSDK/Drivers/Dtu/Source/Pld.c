//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Pld.c *#*#*#*#*#*#*#*#*#*# (C) 2011-2016 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2016 DekTec Digital Video B.V.
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
#include <DtuIncludes.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuPldInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuPldInit(DtuDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  PldFwSize = 0;
    const UInt8*  pPldFirmware = NULL;

    if ((pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
                                                     && pDvcData->m_DevInfo.m_UsbSpeed!=2)
    {
        DtDbgOut(MIN, DTU, "Skipping PLD firmware upload for USB3 device connected to"
                                                                                 " USB2");
        return DT_STATUS_OK;
    }

    // TODOTD: Replace with TypeNumber==315 with DelayedFwLoad property
    if (pDvcData->m_DevInfo.m_TypeNumber == 315)
    {
        DtDbgOut(MIN, DTU, "Skipping PLD firmware upload for DTU-315 (loaded by DTAPI)");
        return DT_STATUS_OK;
    }
    
    // Load firmware for PLD (assuming no firmware variants apply for this device)
    DtDbgOut(MIN, DTU, "Uploading PLD firmware");
    pPldFirmware = DtuGetPldFirmware(pDvcData->m_DevInfo.m_ProductId, -1,
                                      pDvcData->m_DevInfo.m_HardwareRevision, &PldFwSize);
    if (pPldFirmware == NULL)
    {
        DtDbgOut(ERR, DTU, "No PLD firmware available for DTU-%d", 
                                                        pDvcData->m_DevInfo.m_TypeNumber);
        return DT_STATUS_FAIL;
    }
    
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
        Status = DtuFx3LoadPldFirmware(pDvcData, pPldFirmware, PldFwSize);
    else
        Status = DtuLoadPldFirmware(pDvcData, pPldFirmware, PldFwSize);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTU, "Failed to upload PLD firmware (Status=0x%08X)", Status);
        return Status;
    }
    DtDbgOut(MIN, DTU, "Uploading PLD firmware done");
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuFx3LoadPldFirmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuFx3LoadPldFirmware(
    DtuDeviceData*  pDvcData, 
    const UInt8*  pFirmware,
    Int  Size)
{
    DtStatus  Status;
    Int  Len;
    Int  Dummy;
    UInt32  TypeNumber;
    Int  MaxChunkSize = DtUsbGetCtrlMaxPacketSize(&pDvcData->m_Device,
                                                          pDvcData->m_DevInfo.m_UsbSpeed);

    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_PNP_CMD,
                                             DTU_PNP_CMD_FPGA_UPL_CMD, DTU_FPGA_UPL_START,
                                             DT_USB_HOST_TO_DEVICE,
                                             NULL, 0, NULL, MAX_USB_REQ_TIMEOUT);
    if (!DT_SUCCESS(Status))
        return Status;

    Len = MaxChunkSize;

    if (pDvcData->m_DevInfo.m_TypeNumber == 351)
    {
        // DTU-351: Expect PLD firmware data through vendor command
        while (Size > 0)
        {
            Len = Size < MaxChunkSize ? Size : MaxChunkSize;
            if (Len > MaxChunkSize)
                Len = MaxChunkSize;
            Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_FPGA_UPLOAD, 
                                        0, 0, DT_USB_HOST_TO_DEVICE, (UInt8*)pFirmware, 
                                        Len, &Dummy, MAX_USB_REQ_TIMEOUT);
            if (!DT_SUCCESS(Status))
                return Status;
            pFirmware += Len;
            Size -= Len;
        }
    } else {
        // Other device: Expect PLD firmware data through bulk data endpoint
        Int  Pipe = pDvcData->m_EzUsb.m_WritePipe;
        UInt8*  pBuffer = (UInt8*) DtMemAllocPool(DtPoolNonPaged, DTU_BULK_PACKETSIZE, 
                                                                                 DTU_TAG);
        Int  i;
        Int Temp = 0;

        // Clear endpoint buffers
        Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_PNP_CMD,
                             DTU_PNP_CMD_RESET, DTU_RESET_DMA_OUT, DT_USB_HOST_TO_DEVICE,
                             NULL, 0, NULL, MAX_USB_REQ_TIMEOUT);
        if (!DT_SUCCESS(Status))
            return Status;

        // Write data to bulk data endpoint
        while (Size > 0)
        {
            // Copy PLD firmware bit by bit to 32-bit GPIF data word
            // 32-bit GPIF bus connection: DQ[0] = DATA0, DQ[1] = DCLK
            Int  BitCount = 0;
            UInt32*  pWord = (UInt32*)pBuffer;
            Len = DTU_BULK_PACKETSIZE;
            if (Len > Size*8*8)
                Len = Size*8*8;
            
            for (i=0; i<Len/4; i+=2)
            {
                // Satisfy hold time by setting data bit early with respect to the clock
                // Toggle the clock from low to high, in order to end with a high state, 
                // otherwise the FX3 will pull-up the IO lines to '1' when it is not 
                // transmitting data (between EndpointSize transfers), which looks like 
                // an extra clock cycle

                // Big endian busses
                pWord[i] = ((*pFirmware>>BitCount)&0x01);
                pWord[i+1] = (1<<1) | ((*pFirmware>>BitCount)&0x01);

                // Little endian busses
                pWord[i] |= ((*pFirmware>>BitCount)&0x01)<<24;
                pWord[i+1] |= ((1<<1) | ((*pFirmware>>BitCount)&0x01))<<24;

                BitCount++;
                if (BitCount > 7)
                {
                    pFirmware++;
                    Size--;
                    BitCount = 0;
                }
            }

            Status = DtUsbPipeWrite(&pDvcData->m_Device, NULL, Pipe, pBuffer, Len, 
                                                                      MAX_USB_RW_TIMEOUT);
            if (!DT_SUCCESS(Status))
            {
                DtMemFreePool(pBuffer, DTU_TAG);
                return Status;
            }
        }

        DtMemFreePool(pBuffer, DTU_TAG);
    }

    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_PNP_CMD,
                                            DTU_PNP_CMD_FPGA_UPL_CMD, DTU_FPGA_UPL_FINISH,
                                            DT_USB_HOST_TO_DEVICE,
                                            NULL, 0, NULL, MAX_USB_REQ_TIMEOUT);
    if (!DT_SUCCESS(Status))
        return Status;

    DtSleep(100);
    
    // Verify firmware upload
    if (pDvcData->m_DevInfo.m_TypeNumber == 351)
    {
        UInt16  TypeNumber16;
        Status = Dtu35xRegRead(pDvcData, DTU_USB3_DEV_FPGA, 0x00, &TypeNumber16);
        if (!DT_SUCCESS(Status))
            return Status;
        TypeNumber = TypeNumber16;
    } else {
        // TODO: replace 0 by gen reg offset
        Status = Dtu3RegRead(pDvcData, 0, &FwbUsb3GenReg.TypeNum_TypeNum, &TypeNumber);
        if (!DT_SUCCESS(Status))
            return Status;
    }
    if (TypeNumber != pDvcData->m_DevInfo.m_TypeNumber)
    {
        DtDbgOut(ERR, DTU, "Incorrect typenumber read from FPGA (%d)", TypeNumber);
        return DT_STATUS_FAIL;
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuLoadPldFirmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuLoadPldFirmware(
    DtuDeviceData*  pDvcData, 
    const UInt8*  pFirmware,
    Int  Size)
{
    DtStatus  Status;
    UInt8  BitTest;
    Int  i, Total, l, BitNr, Dummy;
    UInt16  WordBits[1024];
    
    // Get pipe for firmware uploading
    UInt8  Pipe = (UInt8)pDvcData->m_EzUsb.m_FirmwarePipe;

    // Check if we need to use fast passive programming (only for 245B type devices).
    Bool  UseFpp = pDvcData->m_DevInfo.m_TypeNumber==245 
                                             && pDvcData->m_DevInfo.m_HardwareRevision>=6;
    DtDbgOut(MIN, DTU, "Programming Pld Firmware using FPP = %d, PID: 0x%04X, HWREV=%d", 
                                                  UseFpp, 
                                                  pDvcData->m_DevInfo.m_ProductId,
                                                  pDvcData->m_DevInfo.m_HardwareRevision);

    //.-.-.-.-.-.-.-.-.-.- Reset the ALTERA and activate program mode -.-.-.-.-.-.-.-.-.-.
    //
    // Toggles nConfig on the ALTERA

    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB_SET_PROG_MODE, 
                                            DTU_STOP_PROG_CYCLE, 0, DT_USB_HOST_TO_DEVICE,
                                            NULL, 0, &Dummy, MAX_USB_REQ_TIMEOUT);
    if (!DT_SUCCESS(Status))
        return Status;
   
    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB_SET_PROG_MODE, 
                                           DTU_START_PROG_CYCLE, 0, DT_USB_HOST_TO_DEVICE,
                                           NULL, 0, &Dummy, MAX_USB_REQ_TIMEOUT);
    if (!DT_SUCCESS(Status))
        return Status;

    // For parallel programming we must wait at least 1,5 ms before starting actual 
    // programming.
    if (UseFpp)
      DtSleep(10);
        
    //-.-.-.-.-.-.-.-.-.- Read all bytes and send them to the DTU-2xx -.-.-.-.-.-.-.-.-.-.

    // Init counters
    Total = BitNr = 0;

    // Add 10 bytes extra to have 80 (=10*8) or 10 (FPP) extra clock cycles to init Altera
    for (i=0; i<Size+10; i++)
    {
        if (i<Size)
            BitTest = pFirmware[i];
        else
            BitTest = 0; // Do not really care about data here; we only want some extra
        
        // clock cycles to make sure the ALTERA intialises
        if (UseFpp)
        {
          // Generate clock data waveform for byte
          // Note: For 8-bits FPP with compression. 
          // If DataByte=d7..d0 then WordBits: b0=d0, b1=clock level, b2..b8 = d1..d7
          // Clk0: xxxxxxx0x
          // Clk1: xxxxxxx1x
          // Clk2: xxxxxxx0x
          // Clk3: xxxxxxx1x
          WordBits[BitNr] = BitTest;
          WordBits[BitNr] = WordBits[BitNr] << 1;
          WordBits[BitNr] &= 0xFFFC;
          WordBits[BitNr] |= BitTest&1;

          WordBits[BitNr+1] = WordBits[BitNr];    // same data as previous word
          WordBits[BitNr+1] |= 2;                 // but with the clock high

          WordBits[BitNr+2] = WordBits[BitNr];    // same data as previous word

          WordBits[BitNr+3] = WordBits[BitNr];    // same data as previous word
          WordBits[BitNr+3] |= 2;                 // but with the clock high
        
          BitNr+=4;
        } else {
          // Generate clock data waveform for byte
          for ( l=0; l<8; l++)
          {
              if (BitTest&0x01)
              {
                  WordBits[BitNr]=1;
                  WordBits[BitNr+1]=3;
              } else
              {
                  WordBits[BitNr]=0;
                  WordBits[BitNr+1]=2;
              }
              BitNr+=2;
              BitTest>>=1;
          }
        }

        // Max number of bits (send data to DTU-2xx)
        if (BitNr == 1024)
        {
            Status = DtUsbPipeWrite(&pDvcData->m_Device, NULL, Pipe, (UInt8*)WordBits, 
                                                           (BitNr*2), MAX_USB_RW_TIMEOUT);
            if (!DT_SUCCESS(Status))
                return Status;
            BitNr=0;
        }
    } 
    // Write last chunck
    Status = DtUsbPipeWrite(&pDvcData->m_Device, NULL, Pipe, (UInt8*)WordBits, (BitNr*2),
                                                                      MAX_USB_RW_TIMEOUT);
    if (!DT_SUCCESS(Status))
        return Status;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Signal end of program -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB_PROGRAM_DONE, 
                                                     1, 0, DT_USB_HOST_TO_DEVICE, NULL, 0,
                                                     &Dummy, MAX_USB_REQ_TIMEOUT);
    if (!DT_SUCCESS(Status))
        return Status;

    // Issue a reset to init ALTERA
    Status = DtuDvcReset(pDvcData);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuPldIsFirmwareLoaded -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Deduces if the PLD firmware is loaded, by checking if we can read the type-number from 
// the general control register
//
Bool  DtuPldIsFirmwareLoaded(DtuDeviceData*  pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  TypeInPld = -1;

    // Read general-control register
    UInt32  RegValue = -1;
    Status = DtuRegRead(pDvcData, DT_GEN_REG_GENCTRL, &RegValue);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(MIN, DTU, "Failed to read general-status register");
        return FALSE;
    }
    TypeInPld = (RegValue & DT_GENCTRL_TYPENUM_MSK) >> DT_GENCTRL_TYPENUM_SH;
    DtDbgOut(MIN, DTU, "Type read from PLD=%d", TypeInPld);

    // Check if type matches expected type
    return (TypeInPld == pDvcData->m_DevInfo.m_TypeNumber);

}
