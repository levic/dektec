//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuRegs.c *#*#*#*#*#*#*#*#*# (C) 2011-2016 DekTec
//
// Dtu driver - Operational register access functions
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
// Must define this in one .c file before including DtuIncludes.h
#define  DEFINE_FWFIELDS 1
#include <DtuIncludes.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuRegRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuRegRead(
    DtuDeviceData*  pDvcData,
    Int  RegAddr,       // Absolute address of register to read
    UInt32*  pValue)    // Read register value
{
    DtStatus Status;

    void*  pTempBuf = NULL;

    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        DtDbgOut(ERR, DTU, "Attempt to read USB2 register on USB3 device");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Init to zero
    *pValue = 0;

    // Cannot use value from stack as buffer, in vendor request => allocate temp buffer
    // for reading register value from device

    pTempBuf = DtMemAllocPool(DtPoolNonPaged, sizeof(UInt32), DTU_TAG);
    if (pTempBuf == NULL)
        return DT_STATUS_OUT_OF_MEMORY;

    // Get register value from DTU
    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB_READ_REG, 
                                      (UInt16)RegAddr, 0, DT_USB_DEVICE_TO_HOST, pTempBuf,
                                      sizeof(UInt32), NULL, MAX_USB_REQ_TIMEOUT);

    // Copy value
    if (DT_SUCCESS(Status))
        *pValue = *((UInt32*)pTempBuf);
    // Free buffer
    DtMemFreePool(pTempBuf, DTU_TAG);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuRegWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuRegWrite(
    DtuDeviceData*  pDvcData,
    Int  RegAddr,       // Absolute address of register to write
    UInt32  Value)      // Value to write to register
{
    DtStatus  Status;
    UInt8*  pTempBuf = NULL;

    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        DtDbgOut(ERR, DTU, "Attempt to write USB2 register on USB3 device");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Cannot use value from stack as buffer, in vendor request => allocate temp buffer
    // for writing register value to device
    pTempBuf = DtMemAllocPool(DtPoolNonPaged, sizeof(UInt32), DTU_TAG);
    if (pTempBuf == NULL)
        return DT_STATUS_OUT_OF_MEMORY;
    // Copy value
    *((UInt32*)pTempBuf) = Value;

    DtDbgOut(MAX, DTU, "Reg write Address: 0x%04X, Data: 0x%08X", RegAddr, Value); 
    
    // Write register value to DTU
    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB_WRITE_REG, 
                                      (UInt16)RegAddr, 0, DT_USB_HOST_TO_DEVICE, pTempBuf,
                                      sizeof(UInt32), NULL, MAX_USB_REQ_TIMEOUT);

    // Free buffer
    DtMemFreePool(pTempBuf, DTU_TAG);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuRegWriteMasked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuRegWriteMasked(
    DtuDeviceData*  pDvcData,
    Int  RegAddr,       // Absolute address of register to write
    UInt32  Mask,       // AND-mask to get field value in register
    UInt32  Shift,      // Position of field in register (#times to shift left)
    UInt32  Value)      // Value to write in field
{
    DtStatus  Status;
    UInt32  RegValue=0;

    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        DtDbgOut(ERR, DTU, "Attempt to write USB2 register on USB3 device");
        return DT_STATUS_NOT_SUPPORTED;
    }

    if (Mask==0xFFFFFFFF && Shift==0)
        // No need to read register
        RegValue = Value;
    else
    {
        // Get current register value
        Status = DtuRegRead(pDvcData, RegAddr, &RegValue);
        if (!DT_SUCCESS(Status))
            return Status;

        // Clear field
        RegValue &= ~Mask;
        // Set field
        RegValue |= (Value<<Shift) & Mask;
    }

    // Write new register value to card
    return DtuRegWrite(pDvcData, RegAddr, RegValue);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu3RegReadRaw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  Dtu3RegReadRaw(
    DtuDeviceData*  pDvcData,
    Int  RegAddr,       // Absolute address of register to read
    UInt32*  pValue)    // Read register value
{
    DtStatus Status;

    void*  pTempBuf = NULL;
    
    if (pDvcData->m_DevInfo.m_TypeNumber<300 || pDvcData->m_DevInfo.m_TypeNumber>=400)
    {
        DtDbgOut(ERR, DTU, "Attempt to read USB3 register on USB2 device");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Init to zero
    *pValue = 0;

    // Cannot use value from stack as buffer, in vendor request => allocate temp buffer
    // for reading register value from device

    pTempBuf = DtMemAllocPool(DtPoolNonPaged, sizeof(UInt32), DTU_TAG);
    if (pTempBuf == NULL)
        return DT_STATUS_OUT_OF_MEMORY;

    // Get register value from DTU
    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_READ_VALUE,
                                     RegAddr&0xFFFF, (RegAddr>>16), DT_USB_DEVICE_TO_HOST,
                                     pTempBuf, sizeof(UInt32), NULL, MAX_USB_REQ_TIMEOUT);

    // Copy value
    if (DT_SUCCESS(Status))
        *pValue = *((UInt32*)pTempBuf);
    // Free buffer
    DtMemFreePool(pTempBuf, DTU_TAG);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu3RegWriteRaw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  Dtu3RegWriteRaw(
    DtuDeviceData*  pDvcData,
    Int  RegAddr,       // Absolute address of register to write
    UInt32  Value)      // Value to write to register
{
    DtStatus  Status;
    UInt8*  pTempBuf = NULL;
    
    if (pDvcData->m_DevInfo.m_TypeNumber<300 || pDvcData->m_DevInfo.m_TypeNumber>=400)
    {
        DtDbgOut(ERR, DTU, "Attempt to write USB3 register on USB2 device");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Cannot use value from stack as buffer, in vendor request => allocate temp buffer
    // for writing register value to device
    pTempBuf = DtMemAllocPool(DtPoolNonPaged, sizeof(UInt32), DTU_TAG);
    if (pTempBuf == NULL)
        return DT_STATUS_OUT_OF_MEMORY;
    // Copy value
    *((UInt32*)pTempBuf) = Value;

    DtDbgOut(MAX, DTU, "Reg write Address: 0x%08X, Data: 0x%08X", RegAddr, Value); 
    
    // Write register value to DTU
    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_WRITE_VALUE,
                                     RegAddr&0xFFFF, (RegAddr>>16), DT_USB_HOST_TO_DEVICE,
                                     pTempBuf, sizeof(UInt32), NULL, MAX_USB_REQ_TIMEOUT);

    // Free buffer
    DtMemFreePool(pTempBuf, DTU_TAG);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu3RegWriteMasked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  Dtu3RegWriteMaskedRaw(
    DtuDeviceData*  pDvcData,
    Int  RegAddr,       // Absolute address of register to write
    UInt32  Mask,       // AND-mask to get field value in register
    UInt32  Shift,      // Position of field in register (#times to shift left)
    UInt32  Value)      // Value to write in field
{
    DtStatus  Status;
    UInt32  RegValue=0;

    if (pDvcData->m_DevInfo.m_TypeNumber<300 || pDvcData->m_DevInfo.m_TypeNumber>=400)
    {
        DtDbgOut(ERR, DTU, "Attempt to write USB3 register on USB2 device");
        return DT_STATUS_NOT_SUPPORTED;
    }

    if (Mask==0xFFFFFFFF && Shift==0)
        // No need to read register
        RegValue = Value;
    else
    {
        // Get current register value
        Status = Dtu3RegReadRaw(pDvcData, RegAddr, &RegValue);
        if (!DT_SUCCESS(Status))
            return Status;

        // Clear field
        RegValue &= ~Mask;
        // Set field
        RegValue |= (Value<<Shift) & Mask;
    }

    // Write new register value to card
    return Dtu3RegWriteRaw(pDvcData, RegAddr, RegValue);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu3RegRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  Dtu3RegRead(
    DtuDeviceData*  pDvcData,
    Int  BlockOffset,
    const DtFwField*  pField,
    UInt32*  pValue)
{
    DT_ASSERT((pField->Operation&FWFIELDOP_R) == FWFIELDOP_R);
    DT_RETURN_ON_ERROR(Dtu3RegReadRaw(pDvcData, BlockOffset+pField->Offset, pValue));
    if (pField->NumBits < 32)
        *pValue = (*pValue >> pField->StartBit) & ((1<<pField->NumBits)-1);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu3RegClear -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  Dtu3RegClear(
    DtuDeviceData*  pDvcData,
    Int  BlockOffset,
    const DtFwField*  pField)
{
    UInt32 Value = ~0;
    DT_ASSERT((pField->Operation&FWFIELDOP_C) == FWFIELDOP_C);

    if (pField->NumBits < 32)
        Value = ((1<<pField->NumBits)-1) << pField->StartBit;
    DT_RETURN_ON_ERROR(Dtu3RegWriteRaw(pDvcData, BlockOffset+pField->Offset, Value));
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu3RegWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  Dtu3RegWrite(
    DtuDeviceData*  pDvcData,
    Int  BlockOffset,
    const DtFwField*  pField,
    UInt32  Value)
{
    UInt32 Mask = ~0;
    DT_ASSERT((pField->Operation&FWFIELDOP_W) == FWFIELDOP_W);

    if (pField->NumBits == 32)
    {
        DT_RETURN_ON_ERROR(Dtu3RegWriteRaw(pDvcData, BlockOffset+pField->Offset, Value));
        return DT_STATUS_OK;
    }

    Mask = ((1<<pField->NumBits)-1);
    DT_ASSERT((Value & ~Mask) == 0);
    Mask <<= pField->StartBit;

    if (!pField->Exclusive)
    {
        DT_RETURN_ON_ERROR(Dtu3RegWriteMaskedRaw(pDvcData, BlockOffset+pField->Offset,
                                                          Mask, pField->StartBit, Value));
    } else {
        Value = ((Value & ((1<<pField->NumBits)-1)) << pField->StartBit);
        DT_RETURN_ON_ERROR(Dtu3RegWriteRaw(pDvcData, BlockOffset+pField->Offset, Value));
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu35xRegRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  Dtu35xRegRead(
    DtuDeviceData*  pDvcData,
    UInt16  Dvc,
    UInt16  RegAddr,
    UInt16* pValue)
{
    DtStatus Status = DT_STATUS_OK;
    Int  Len = -1;
    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_READ_VALUE,
                                            RegAddr, Dvc, DT_USB_DEVICE_TO_HOST,
                                            (UInt8*)pValue, 2, &Len, MAX_USB_REQ_TIMEOUT);
    if (Len != 2)
        return DT_STATUS_FAIL;
    return Status;

    // TODOGS: Code for DTU-351 FPGA FW v2
    // TODOGS: Deprecate and use Dtu3RegRead
    //DtStatus Status = DT_STATUS_OK;
    //Int  Len = -1;
    //UInt16  Index = (Dvc<<8) | ((RegAddr&0xFF0000)>>16);
    //Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_READ_VALUE,
    //                                        RegAddr&0xFFFF, Index, 
    //                                        DT_USB_DEVICE_TO_HOST, (UInt8*)pValue, 
    //                                        sizeof(UInt32), &Len, MAX_USB_REQ_TIMEOUT);
    //if (Len != sizeof(UInt32))
    //{
    //    DtDbgOut(ERR, DTU, "Reg read Device: 0x%X, Address: 0x%04X failed", Dvc, RegAddr); 
    //    return DT_STATUS_FAIL;
    //} else 
    //    DtDbgOut(MAX, DTU, "Reg read Device: 0x%X, Address: 0x%04X, Data: 0x%08X", 
    //                                                               Dvc, RegAddr, *pValue);
    //return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu3RegWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  Dtu35xRegWrite(
    DtuDeviceData*  pDvcData,
    UInt16  Dvc,
    UInt16  RegAddr,
    UInt16  Value)
{
    DtStatus Status = DT_STATUS_OK;
    Int  Len = -1;
    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_WRITE_VALUE,
                                            RegAddr, Dvc, DT_USB_HOST_TO_DEVICE,
                                            (UInt8*)&Value, 2, &Len, MAX_USB_REQ_TIMEOUT);
    if (Len != 2)
        return DT_STATUS_FAIL;
    return Status;

    // TODOGS: Code for DTU-351 FPGA FW v2
    // TODOGS: Deprecate and use Dtu3RegWrite
    //DtStatus Status = DT_STATUS_OK;
    //Int  Len = -1;
    //UInt16  Index = (Dvc<<8) | ((RegAddr&0xFF0000)>>16);
    //Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_WRITE_VALUE,
    //                                        RegAddr&0xFFFF, Index, 
    //                                        DT_USB_HOST_TO_DEVICE, (UInt8*)&Value, 
    //                                        sizeof(UInt32), &Len, MAX_USB_REQ_TIMEOUT);
    //if (Len != sizeof(UInt32))
    //{
    //    DtDbgOut(ERR, DTU, "Reg write Device: 0x%X, Address: 0x%04X, Data: 0x%08X failed", 
    //                                                                 Dvc, RegAddr, Value); 
    //    return DT_STATUS_FAIL;
    //} else 
    //    DtDbgOut(MAX, DTU, "Reg write Device: 0x%X, Address: 0x%04X, Data: 0x%08X", 
    //                                                                 Dvc, RegAddr, Value); 
    //return Status;
}
