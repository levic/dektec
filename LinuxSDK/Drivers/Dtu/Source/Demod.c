//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Demod.c *#*#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtuLoadDemodFirmware(DtuDeviceData* pDvcData, 
                                              const DtuDemodFirmwareStore* pDemodFirmware);



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDemodInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuDemodInit(DtuDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    const DtuDemodFirmwareStore*  pDemodFirmware = NULL;
    
    pDemodFirmware = DtuGetDemodFirmware(pDvcData->m_DevInfo.m_ProductId,
                                                  pDvcData->m_DevInfo.m_HardwareRevision);
    if (pDemodFirmware == NULL)
        return DT_STATUS_OK;
    
    Status =  DtuLoadDemodFirmware(pDvcData, pDemodFirmware);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTU, "Failed to upload demodulator firmware (Status=0x%08X)", 
                                                                                  Status);
        return Status;
    }
    DtDbgOut(AVG, DTU, "Demodulator firmware uploaded successfully");
                                                                                  
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuLoadDemodFirmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuLoadDemodFirmware(
    DtuDeviceData*  pDvcData, 
    const DtuDemodFirmwareStore*  pDemodFirmware)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  Buffer[2];
    Int  i, j;
    UInt DvcAddr = pDemodFirmware->m_DemodI2cAddress;
    
    DT_ASSERT(pDemodFirmware != NULL);
    
    // First do the PRE-upload register writes
    if (pDemodFirmware->m_pPreUpload != NULL)
    {
        const DtuInitRegisterStruct*  pInitRegisterData = pDemodFirmware->m_pPreUpload;

        for (i=0; i<pInitRegisterData->m_RegisterDataCount; i++)
        {
            Buffer[0] = pInitRegisterData->m_RegData[i].m_Data[0];
            Buffer[1] = pInitRegisterData->m_RegData[i].m_Data[1];
            Status = DtuI2cWrite(pDvcData, NULL, DvcAddr, 2, Buffer);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, DTU, "Error writing I2C pre-upload data. (Status=0x%08X)",
                                                                                  Status);
                return Status;
            }
        }
    }

    // Upload demodulator firmware
    if (pDemodFirmware->m_pFirmware != NULL)
    {
        const Int  MAX_BYTES_TO_TRY = 60;
        UInt8  AddrHigh;
        UInt8  AddrLow;
        Int  DataCount;
        Int  BytesToTry;
        const DtuHexStruct* pFirmware = pDemodFirmware->m_pFirmware;
        UInt8* pHexData=NULL;

        // First allocate memory for temporary helper buffer
        pHexData = DtMemAllocPool(DtPoolNonPaged, 4096, DTU_TAG);
        if (pHexData == NULL)
            return DT_STATUS_OUT_OF_MEMORY;
    
        for (i=0; i<pFirmware->m_HexBlockCount; i++)
        {
            const DtuHexBlockStruct*  pHexBlock = &(pFirmware->m_HexBlock[i]);
            DataCount = pHexBlock->m_DataCount;

            BytesToTry = 0;

            while (DataCount > 0)
            {
                AddrHigh = (UInt8)((pHexBlock->m_Address +
                                                  pHexBlock->m_DataCount-DataCount) >> 8);
                AddrLow = (UInt8)(pHexBlock->m_Address +
                                                        pHexBlock->m_DataCount-DataCount);
                // Write high address
                Buffer[0] = (UInt8)pDemodFirmware->m_AddrRegH;
                Buffer[1] = AddrHigh;
                Status = DtuI2cWrite(pDvcData, NULL, DvcAddr, 2, Buffer);
                if (!DT_SUCCESS(Status))
                {
                    DtDbgOut(ERR, DTU, "Error writing I2C high address. (Status=0x%08X)",
                                                                                  Status);
                    DtMemFreePool(pHexData, DTU_TAG);
                    return Status;
                }

                // Write low address
                Buffer[0] = (UInt8)pDemodFirmware->m_AddrRegL;
                Buffer[1] = AddrLow;
                Status = DtuI2cWrite(pDvcData, NULL, DvcAddr, 2, Buffer);
                if (!DT_SUCCESS(Status))
                {
                    DtDbgOut(ERR, DTU, "Error writing I2C low address. (Status=0x%08X)",
                                                                                  Status);
                    DtMemFreePool(pHexData, DTU_TAG);
                    return Status;
                }

                // Write data
                pHexData[0] = (UInt8)pDemodFirmware->m_DataReg;
                BytesToTry = (DataCount<=MAX_BYTES_TO_TRY ? DataCount : MAX_BYTES_TO_TRY);
                for (j=0; j<BytesToTry; j++) 
                    pHexData[j + 1] = pHexBlock->m_Data[j + pHexBlock->m_DataCount - 
                                                                               DataCount];
                Status = DtuI2cWrite(pDvcData, NULL, DvcAddr, BytesToTry+1, pHexData);
                if (!DT_SUCCESS(Status))
                {
                    DtDbgOut(ERR, DTU, "Error writing I2C data. (Status=0x%08X)", Status);
                    DtMemFreePool(pHexData, DTU_TAG);
                    return Status;
                }
                DataCount -= BytesToTry;
            }

        }
        DtMemFreePool(pHexData, DTU_TAG);
    }

    // Do the STOP-upload register writes
    if (pDemodFirmware->m_pStopUpload != NULL)
    {
        const DtuInitRegisterStruct*  pInitRegisterData = pDemodFirmware->m_pStopUpload;

        for (i=0; i<pInitRegisterData->m_RegisterDataCount; i++)
        {
            Buffer[0] = pInitRegisterData->m_RegData[i].m_Data[0];
            Buffer[1] = pInitRegisterData->m_RegData[i].m_Data[1];
            Status = DtuI2cWrite(pDvcData, NULL, DvcAddr, 2, Buffer);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, DTU, "Error writing I2C stop-upload data. (Status=0x%08X)",
                                                                                  Status);
                return Status;
            }
        }
    }

    // Wait until AP is running
    if (pDemodFirmware->m_pFirmware != NULL)
    {   
        Int  TimeOut;
        Buffer[0] = (UInt8)pDemodFirmware->m_ApStatReg;
        Status = DtuI2cWrite(pDvcData, NULL, DvcAddr, 1, Buffer);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTU, "Error writing I2C apstat data. (Status=0x%08X)", Status);
            return Status;
        }

        // Wait until ready
        Buffer[0] = 1;
        TimeOut = 0;
        while (Buffer[0]==1 && TimeOut<250) // Wait max. 250ms
        {
            DtSleep(10);
            TimeOut += 10;
            // Read the app status
            Status = DtuI2cRead(pDvcData, NULL, DvcAddr, 1, Buffer);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, DTU, "Error reading I2C apstat data. (Status=0x%08X)",
                                                                                  Status);
                return Status;
            }
        }
        if (Buffer[0] == 1)
        {
            DtDbgOut(ERR, DTU, "TIMEOUT! APSTAT = %x.", (Int)(Buffer[0]));
        }
    }

    //Do the post-upload register writes
    if (pDemodFirmware->m_pPostUpload != NULL)
    {
        const DtuInitRegisterStruct*  pInitRegisterData = pDemodFirmware->m_pPostUpload;

        for (i=0; i<pInitRegisterData->m_RegisterDataCount; i++)
        {
            Buffer[0] = pInitRegisterData->m_RegData[i].m_Data[0];
            Buffer[1] = pInitRegisterData->m_RegData[i].m_Data[1];
            Status = DtuI2cWrite(pDvcData, NULL, DvcAddr, 2, Buffer);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, DTU, "Error writing I2C post-upload data. (Status=0x%08X)",
                                                                                  Status);
                return Status;
            }
        }
    }
    return Status;
}
