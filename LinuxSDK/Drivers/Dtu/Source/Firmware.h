//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Firmware.h *#*#*#*#*#*#*#*#* (C) 2011-2015 DekTec

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

#ifndef __FIRMWARE_H
#define __FIRMWARE_H


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIntelHexRecord -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// INTEL hex record structure used for EzUsb firmware
//
#define DTU_MAX_INTEL_HEX_RECORD_LENGTH 16
typedef struct _DtuIntelHexRecord
{
   UInt8  Length;
   UInt16  Address;
   UInt8  Type;
   UInt8  Data[DTU_MAX_INTEL_HEX_RECORD_LENGTH];
} DtuIntelHexRecord;

typedef struct _DtuFx3HexRecord
{
   UInt32  Length;
   UInt32  Address;
   const UInt8*  Data;
} DtuFx3HexRecord;

typedef struct  _DtuEzUsbFirmwareStore
{
    Int  m_ProductId;
    Int  m_FwVariant;
    Int  m_MinHwRev;
    const DtuIntelHexRecord*  m_pFirmware;
} DtuEzUsbFirmwareStore;

typedef struct  _DtuFx3FirmwareStore
{
    Int  m_ProductId;
    Int  m_FwVariant;
    Int  m_MinHwRev;
    const DtuFx3HexRecord*  m_pFirmware;
} DtuFx3FirmwareStore;

typedef struct  _DtuPldFirmwareStore
{
    Int  m_ProductId;
    Int  m_FwVariant;
    Int  m_MinHwRev;
    const UInt8*  m_pFirmware;
    const Int  m_Size;
} DtuPldFirmwareStore;

extern const Int  DtuPldFirmwareStoreCount;
extern const DtuPldFirmwareStore  DtuPldFirmwareStores[];

typedef struct _DtuHexBlockStruct
{
    Int  m_Address;
    Int  m_DataCount;
    UInt8  m_Data[4096];
} DtuHexBlockStruct;

typedef struct _DtuHexStruct
{
    DtuHexBlockStruct  m_HexBlock[16];
    Int  m_HexBlockCount;
} DtuHexStruct;


typedef struct _DtuRegisterDataStruct
{
    UInt8  m_Data[2];
} DtuRegisterDataStruct;

typedef struct _DtuInitRegisterStruct
{
    DtuRegisterDataStruct  m_RegData[10];
    Int  m_RegisterDataCount;
} DtuInitRegisterStruct;

typedef struct  _DtuDemodFirmwareStore
{
    Int  m_ProductId;
    Int  m_MinHwRev;
    Int  m_DemodI2cAddress;
    Int  m_AddrRegH;        // MADRH
    Int  m_AddrRegL;        // MADRL
    Int  m_DataReg;         // MDATA
    Int  m_ApStatReg;       // APSTAT1
    const DtuInitRegisterStruct*  m_pPreUpload;
    const DtuInitRegisterStruct*  m_pStopUpload;
    const DtuInitRegisterStruct*  m_pPostUpload;
    const DtuHexStruct*  m_pFirmware;
} DtuDemodFirmwareStore;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
const DtuIntelHexRecord*  DtuGetEzUsbFirmware(Int ProductId, Int FwVariant, Int HwRev);
const DtuFx3HexRecord*  Dtu3GetFx3Firmware(Int TypeNumber, Int FwVariant, Int HwRev);
const UInt8*  DtuGetPldFirmware(Int ProductId, Int FwVariant, Int HwRev,  Int* pSize);
const DtuDemodFirmwareStore*  DtuGetDemodFirmware(Int ProductId, Int HwRev);

#endif // #ifndef __FIRMWARE_H
