//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcVVI.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017 DekTec Digital Video B.V.
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
#include "DtBcVVI.h"
#include "DtBcVVI_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcVVI implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Helper macro to cast a DtBc* to a DtBcVVI*
#define BC_VVI           ((DtBcVVI*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcVVI_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcVVI_Close(DtBc*  pBc)
{
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcVVI));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcVVI_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcVVI*  DtBcVVI_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_VVI_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcVVI, Id, DT_BLOCK_TYPE_VVI, Address,
                                                                       pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcVVI_Close;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcVVI*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcVVI_InitFirmwareVersionInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcVVI_InitFirmwareVersionInfo(
    DtBcVVI*  pBc,
    DtDeviceInfo*  pDevInfo)
{
    Int  TypeNumber = 0;
    UInt32  Reg_VVI=0;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcVVI));
    DT_ASSERT(pDevInfo != NULL);

    // Read version info from firmware
    TypeNumber = VVI_TypeNumber_READ(pBc);

    // Cache firmware build register
    Reg_VVI = VVI_FirmwareBuild_READ(pBc);
    // Firmware Build - Firmware Version
    pDevInfo->m_FirmwareVersion = VVI_FirmwareBuild_GET_Version(Reg_VVI);
    // Firmware Build - Firmware Variant
    pDevInfo->m_FirmwareVariant = VVI_FirmwareBuild_GET_Variant(Reg_VVI);
    // Firmware Package Version - NA 
    pDevInfo->m_FwPackageVersion = -1;
    // Firmware Status - undefined (will be set when the property store is active)
    pDevInfo->m_FirmwareStatus = DT_FWSTATUS_UNDEFINED;
    // Cache firmware build date register
    Reg_VVI = VVI_FirmwareBuildDate_READ(pBc);
    // Firmware Build - Year
    pDevInfo->m_FwBuildDate.m_Year = (UInt16)VVI_FirmwareBuildDate_GET_Year(Reg_VVI);
    // Firmware Build - Month
    pDevInfo->m_FwBuildDate.m_Month = (UInt8)VVI_FirmwareBuildDate_GET_Month(Reg_VVI);
    // Firmware Build - Day
    pDevInfo->m_FwBuildDate.m_Day = (UInt8)VVI_FirmwareBuildDate_GET_Day(Reg_VVI);
    // Firmware Build - Hour
    pDevInfo->m_FwBuildDate.m_Hour = (UInt8)VVI_FirmwareBuildDate_GET_Hour(Reg_VVI);
    // Firmware Build - Minute
    pDevInfo->m_FwBuildDate.m_Minute = (UInt8)VVI_FirmwareBuildDate_GET_Minute(Reg_VVI);

    DtDbgOutBc(MIN, VVI, pBc, "TypeNum=%d, FwVer=%d, Fwvar=%d, "
                                                        "Date=%d.%02d.%02d-%02dh%02dm",
                                                        TypeNumber, 
                                                        pDevInfo->m_FirmwareVersion,
                                                        pDevInfo->m_FirmwareVariant,
                                                        pDevInfo->m_FwBuildDate.m_Year,
                                                        pDevInfo->m_FwBuildDate.m_Month,
                                                        pDevInfo->m_FwBuildDate.m_Day,
                                                        pDevInfo->m_FwBuildDate.m_Hour,
                                                        pDevInfo->m_FwBuildDate.m_Minute);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcVVI_WaitUntilFpgaIsReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcVVI_WaitUntilFpgaIsReady(DtBcVVI*  pBc)
{
    UInt32  Timeout = 0;
    UInt  TypeNumber=-1;
    DtDeviceInfo*  pDvcInfo = NULL;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcVVI));
    DT_ASSERT(pBc->m_pCore != NULL);

    pDvcInfo = pBc->m_pCore->m_pDevInfo;
    DT_ASSERT(pDvcInfo != NULL);

    // Wait shortly to allow firmware to boot
    DtSleep(300);
  
    // Wait until type number is read correctly or timeout occurs
    TypeNumber = VVI_TypeNumber_READ(pBc);
    while (TypeNumber!=pDvcInfo->m_TypeNumber && Timeout<1000)
    {
        DtSleep(1);
        Timeout++;

        // Read typenumber again
        TypeNumber = VVI_TypeNumber_READ(pBc);
    }

    if (Timeout >= 1000)
    {
        DtDbgOutBc(ERR, VVI, pBc, "FPGA DID NOT RESPOND WITHIN 1000ms");
        return DT_STATUS_TIMEOUT;
    }
    return DT_STATUS_OK;
}
