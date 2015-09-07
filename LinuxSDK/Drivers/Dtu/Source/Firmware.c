//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Firmware.c *#*#*#*#*#*#*#*#* (C) 2011-2015 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2015 DekTec Digital Video B.V.
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

extern const Int  DtuEzUsbFirmwareStoreCount;
extern const DtuEzUsbFirmwareStore  DtuEzUsbFirmwareStores[];

extern const Int  DtuFx3FirmwareStoreCount;
extern const DtuFx3FirmwareStore  DtuFx3FirmwareStores[];

extern const Int  DtuPldFirmwareStoreCount;
extern const DtuPldFirmwareStore  DtuPldFirmwareStores[];

extern const Int  DtuDemodFirmwareStoreCount;
extern const DtuDemodFirmwareStore DtuDemodFirmwareStores[];

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuGetPldFirmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
const UInt8*  DtuGetPldFirmware(Int ProductId, Int FwVariant, Int HwRev, Int* pSize)
{
    Int  Size = 0;
    const UInt8*  pFirmware = NULL;
    Int  MinHwRev = -1;
    Int  i=0;
    for (i=0; i<DtuPldFirmwareStoreCount; i++)
    {
        if (DtuPldFirmwareStores[i].m_ProductId==ProductId
                           && DtuPldFirmwareStores[i].m_FwVariant==FwVariant
                           && DtuPldFirmwareStores[i].m_MinHwRev<=HwRev && MinHwRev<HwRev)
        {
            MinHwRev = DtuPldFirmwareStores[i].m_MinHwRev;
            Size = DtuPldFirmwareStores[i].m_Size;
            pFirmware = DtuPldFirmwareStores[i].m_pFirmware;
        }
    }
    if (pFirmware == NULL)
    {
        // FW-variant specific firmware not found, try to find the non FW-variant specific 
        for (i=0; i<DtuPldFirmwareStoreCount; i++)
        {
            if (DtuPldFirmwareStores[i].m_ProductId==ProductId
                           && DtuPldFirmwareStores[i].m_FwVariant==-1
                           && DtuPldFirmwareStores[i].m_MinHwRev<=HwRev && MinHwRev<HwRev)
            {
                MinHwRev = DtuPldFirmwareStores[i].m_MinHwRev;
                Size = DtuPldFirmwareStores[i].m_Size;
                pFirmware = DtuPldFirmwareStores[i].m_pFirmware;
            }
        }
    }
    if (pFirmware != NULL)
        DtDbgOut(MAX, DTU, "Found PLD FW for : PID 0x%04X HW_REV >= %d", ProductId, 
                                                                                MinHwRev);
    *pSize = Size;
    return pFirmware;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuGetEzUsbFirmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
const DtuIntelHexRecord*  DtuGetEzUsbFirmware(Int ProductId, Int FwVariant, Int HwRev)
{
    const DtuIntelHexRecord*  pFirmware = NULL;
    Int  MinHwRev = -1;
    Int  i=0;
    for (i=0; i<DtuEzUsbFirmwareStoreCount; i++)
    {
        if (DtuEzUsbFirmwareStores[i].m_ProductId==ProductId
                         && DtuEzUsbFirmwareStores[i].m_FwVariant==FwVariant
                         && DtuEzUsbFirmwareStores[i].m_MinHwRev<=HwRev && MinHwRev<HwRev)
        {
            MinHwRev = DtuEzUsbFirmwareStores[i].m_MinHwRev;
            pFirmware = DtuEzUsbFirmwareStores[i].m_pFirmware;
        }
    }
    
    if (pFirmware == NULL)
    {
        // FW-variant specific firmware not found, try to find the non FW-variant specific 
        for (i=0; i<DtuEzUsbFirmwareStoreCount; i++)
        {
            if (DtuEzUsbFirmwareStores[i].m_ProductId==ProductId
                         && DtuEzUsbFirmwareStores[i].m_FwVariant==-1
                         && DtuEzUsbFirmwareStores[i].m_MinHwRev<=HwRev && MinHwRev<HwRev)
            {
                MinHwRev = DtuEzUsbFirmwareStores[i].m_MinHwRev;
                pFirmware = DtuEzUsbFirmwareStores[i].m_pFirmware;
            }
        }
    }

    if (pFirmware != NULL)
        DtDbgOut(MAX, DTU, "Found USB FW for : PID 0x%04X HW_REV >= %d", ProductId, 
                                                                                MinHwRev);
    return pFirmware;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu3GetFx3Firmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
const DtuFx3HexRecord*  Dtu3GetFx3Firmware(Int TypeNumber, Int FwVariant, Int HwRev)
{
    const DtuFx3HexRecord*  pFirmware = NULL;
    Int  MinHwRev = -1;
    Int  i=0;
    for (i=0; i<DtuFx3FirmwareStoreCount; i++)
    {
        if (DtuFx3FirmwareStores[i].m_ProductId==TypeNumber
                         && DtuFx3FirmwareStores[i].m_FwVariant==FwVariant
                         && DtuFx3FirmwareStores[i].m_MinHwRev<=HwRev && MinHwRev<HwRev)
        {
            MinHwRev = DtuFx3FirmwareStores[i].m_MinHwRev;
            pFirmware = DtuFx3FirmwareStores[i].m_pFirmware;
        }
    }
        
    if (pFirmware == NULL)
    {
        // FW-variant specific firmware not found, try to find the non FW-variant specific 
        for (i=0; i<DtuFx3FirmwareStoreCount; i++)
        {
            if (DtuFx3FirmwareStores[i].m_ProductId==TypeNumber
                           && DtuFx3FirmwareStores[i].m_FwVariant==-1
                           && DtuFx3FirmwareStores[i].m_MinHwRev<=HwRev && MinHwRev<HwRev)
            {
                MinHwRev = DtuFx3FirmwareStores[i].m_MinHwRev;
                pFirmware = DtuFx3FirmwareStores[i].m_pFirmware;
            }
        }
    }

    if (pFirmware != NULL)
        DtDbgOut(MAX, DTU, "Found USB FW for : Type %d HW_REV >= %d", TypeNumber, 
                                                                                MinHwRev);
    return pFirmware;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuGetDemodFirmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
const DtuDemodFirmwareStore*  DtuGetDemodFirmware(Int ProductId, Int HwRev)
{
    const DtuDemodFirmwareStore*  pFirmware = NULL;
    Int  MinHwRev = -1;
    Int  i=0;
    for (i=0; i<DtuDemodFirmwareStoreCount; i++)
    {
        if (DtuDemodFirmwareStores[i].m_ProductId==ProductId
                         && DtuDemodFirmwareStores[i].m_MinHwRev<=HwRev && MinHwRev<HwRev)
        {
            MinHwRev = DtuDemodFirmwareStores[i].m_MinHwRev;
            pFirmware = &DtuDemodFirmwareStores[i];
        }
    }
    if (pFirmware != NULL)
        DtDbgOut(MAX, DTU, "Found DEMOD FW for : PID 0x%04X HW_REV >= %d", ProductId, 
                                                                                MinHwRev);
    return pFirmware;
}
