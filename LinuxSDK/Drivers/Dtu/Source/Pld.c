//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Pld.c *#*#*#*#*#*#*#*#*#*# (C) 2011-2012 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2012 DekTec Digital Video B.V.
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
#include <DtuIncludes.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuPldInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuPldInit(DtuDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  PldFwSize = 0;
    const UInt8*  pPldFirmware = NULL;
    
    // Load firmware for PLD
    DtDbgOut(MIN, DTU, "Uploading PLD firmware");
    pPldFirmware = DtuGetPldFirmware(pDvcData->m_DevInfo.m_ProductId,
                                      pDvcData->m_DevInfo.m_HardwareRevision, &PldFwSize);
    if (pPldFirmware == NULL)
    {
        DtDbgOut(ERR, DTU, "No PLD firmware available for DTU-%d", 
                                                        pDvcData->m_DevInfo.m_TypeNumber);
        return DT_STATUS_FAIL;
    }
        
    Status = DtuLoadPldFirmware(pDvcData, pPldFirmware, PldFwSize);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTU, "Failed to upload PLD firmware (Status=0x%08X)", Status);
        return Status;
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

    //-.-.-.-.-.-.-.-.-.- Read all bytes and send them to the DTU-2xx -.-.-.-.-.-.-.-.-.-.

    // Init counters
    Total = BitNr = 0;

    // Add 10 bytes extra to have 80 (=10*8) extra clock cycles to init Altera
    for (i=0; i<Size+10; i++)
    {
        if (i<Size)
            BitTest = pFirmware[i];
        else
            BitTest = 0; // Do not really care about data here; we only want some extra
        // clock cycles to make sure the ALTERA intialises

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
    // Write last chuck
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
