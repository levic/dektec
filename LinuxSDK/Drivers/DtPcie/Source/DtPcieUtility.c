//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPcieUtility.c *#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
//
// DtPcie driver - DTA utility functions.
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
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtPcieIncludes.h"

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieDeviceId2TypeNumber -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtPcieDeviceId2TypeNumber(Int DeviceId)
{
    return DeviceId & 0xFFF;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieDeviceId2SubType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtPcieDeviceId2SubType(Int DeviceId)
{
    Int  SubType =((DeviceId & 0xF000) >> 12);
    SubType = (SubType<=9) ? 0 : (SubType - 9); // 0x0 = SubType 0; 0xA = SubType 1; etc
    return SubType;
}

////-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieDeviceId2SubDvcNumber -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
////
//Int  DtPcieDeviceId2SubDvcNumber(Int TypeNumber, Int  DeviceId)
//{
//    // Can only deduce type number from device ID if type number is encoded in bottom
//    // 12-bits of the DeviceId
//    if ((DeviceId&0X0FFF) == TypeNumber)
//    {
//        // Sub device number is encoded in bits 12..13 of device ID
//        // and the DTA-2179 it is encoded in bits 12..14
//        // NOTE: 0=master, 1=slave1, 2=slavee, etc, etc
//        if (TypeNumber == 2179)
//            return (DeviceId>>12 & 0x0007);
//        else
//            return (DeviceId>>12 & 0x0003);
//    }   
//    return 0;   // assume this is the one and only sub-device
//}

////.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieWatchdogToggle -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
////
//void DtPcieWatchdogToggle(volatile UInt8* pBase)
//{
//    if (DtPcieRegControl1GetWatchdog(pBase) == 1)
//        DtPcieRegControl1SetWatchdog(pBase, 0);
//    else
//        DtPcieRegControl1SetWatchdog(pBase, 1);
//}
//
////-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieWatchdogPulse -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
////
//void DtPcieWatchdogPulse(volatile UInt8* pBase)
//{
//    UInt  Val = DtPcieRegControl1GetWatchdog(pBase);
//    DtPcieWatchdogToggle(pBase);
//    DtPcieRegControl1SetWatchdog(pBase, Val);
//}
//
////.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieGetPerIntItvUS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//// 
//// Returns the current periodic interval in micro seconds
////
//UInt32 DtPcieGetPerIntItvUS(DtCorePcie* pCore)
//{
//    UInt64  Value64;
//
//    DT_ASSERT(pCore->m_DevInfo.m_PerIntClkBit >= 17);
//    Value64 = 1000000 * ((UInt64)1<<pCore->m_DevInfo.m_PerIntClkBit);
//    return (UInt32)DtDivide64(Value64, pCore->m_DevInfo.m_RefClk, NULL);
//}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPciePropertiesInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPciePropertiesInit(DtCorePcie* pCore)
{
    //DtString  DtStrType;
    //DtStringChar  DtStrTypeBuffer[9];
    //DtPropertyData*  pPropData = &pCore->m_PropData;
    //DtEvtLog*  pEvtObject = &pCore->m_Device.m_EvtObject;
    //DtStatus  Status;
    //
    //// Connect DtStrTypeBuffer to DtStrType
    //DT_STRING_INIT(DtStrType, DtStrTypeBuffer,
    //                                        sizeof(DtStrTypeBuffer)/sizeof(DtStringChar));
    //
    //// Init properties failed?
    //Status = DtPropertiesInit(pPropData);
    //if (Status != DT_STATUS_OK)
    //{
    //     // Use event to report driver errors 
    //    DtStringAppendChars(&DtStrType, "DTA-");
    //    DtStringUIntegerToDtStringAppend(&DtStrType, 10, pPropData->m_TypeNumber);
    //    DtEvtLogReport(pEvtObject, DTPCIE_LOG_PROP_STORE_NOT_FOUND, &DtStrType, NULL, NULL);
    //}
    //return Status;
    return DT_STATUS_OK;
}



//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPciePropertiesReportDriverErrors -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPciePropertiesReportDriverErrors(DtCorePcie* pCore)
 {
    //DtPropertyData*  pPropData = &pCore->m_PropData;
    //DtEvtLog*  pEvtObject = &pCore->m_Device.m_EvtObject;
    //
    //// Errors to report?
    //DtStatus  Status = DtPropertiesReportDriverErrors(pPropData);
    //if (Status != DT_STATUS_OK)
    //{
    //    // Use event to report driver errors 
    //    DtEvtLogReport(pEvtObject, DTPCIE_LOG_CONFIG_FAILED,
    //                                    &pPropData->m_PropertyNotFoundString, NULL, NULL);
    //}
    //return Status;
    return DT_STATUS_OK;
 }

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Video Standard helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

////-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieIoStd2VidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
////
//Int  DtPcieIoStd2VidStd(Int  Value, Int  SubValue)
//{
//    switch (SubValue)
//    {
//    case DT_IOCONFIG_1080P50:       return DT_VIDSTD_1080P50;
//    case DT_IOCONFIG_1080P50B:      return DT_VIDSTD_1080P50B;
//    case DT_IOCONFIG_1080P59_94:    return DT_VIDSTD_1080P59_94;
//    case DT_IOCONFIG_1080P59_94B:   return DT_VIDSTD_1080P59_94B;
//    case DT_IOCONFIG_1080P60:       return DT_VIDSTD_1080P60;
//    case DT_IOCONFIG_1080P60B:      return DT_VIDSTD_1080P60B;
//    case DT_IOCONFIG_1080I50:       return DT_VIDSTD_1080I50;
//    case DT_IOCONFIG_1080I59_94:    return DT_VIDSTD_1080I59_94;
//    case DT_IOCONFIG_1080I60:       return DT_VIDSTD_1080I60;
//    case DT_IOCONFIG_1080P23_98:    return DT_VIDSTD_1080P23_98;
//    case DT_IOCONFIG_1080P24:       return DT_VIDSTD_1080P24;
//    case DT_IOCONFIG_1080P25:       return DT_VIDSTD_1080P25;
//    case DT_IOCONFIG_1080P29_97:    return DT_VIDSTD_1080P29_97;
//    case DT_IOCONFIG_1080P30:       return DT_VIDSTD_1080P30;
//    case DT_IOCONFIG_1080PSF23_98:  return DT_VIDSTD_1080PSF23_98;
//    case DT_IOCONFIG_1080PSF24:     return DT_VIDSTD_1080PSF24;
//    case DT_IOCONFIG_1080PSF25:     return DT_VIDSTD_1080PSF25;
//    case DT_IOCONFIG_1080PSF29_97:  return DT_VIDSTD_1080PSF29_97;
//    case DT_IOCONFIG_1080PSF30:     return DT_VIDSTD_1080PSF30;
//    case DT_IOCONFIG_720P23_98:     return DT_VIDSTD_720P23_98;
//    case DT_IOCONFIG_720P24:        return DT_VIDSTD_720P24;
//    case DT_IOCONFIG_720P25:        return DT_VIDSTD_720P25;
//    case DT_IOCONFIG_720P29_97:     return DT_VIDSTD_720P29_97;
//    case DT_IOCONFIG_720P30:        return DT_VIDSTD_720P30;
//    case DT_IOCONFIG_720P50:        return DT_VIDSTD_720P50;
//    case DT_IOCONFIG_720P59_94:     return DT_VIDSTD_720P59_94;
//    case DT_IOCONFIG_720P60:        return DT_VIDSTD_720P60;
//    case DT_IOCONFIG_525I59_94:     return DT_VIDSTD_525I59_94;
//    case DT_IOCONFIG_625I50:        return DT_VIDSTD_625I50;
//    }
//    return DT_VIDSTD_UNKNOWN;
//}
//
////.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieVidStd2Fps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
////
//Int  DtPcieVidStd2Fps(Int  VidStd)
//{
//    switch (VidStd)
//    {
//    case DT_VIDSTD_720P59_94:
//    case DT_VIDSTD_720P60:
//    case DT_VIDSTD_1080P59_94:
//    case DT_VIDSTD_1080P59_94B:
//    case DT_VIDSTD_1080P60:
//    case DT_VIDSTD_1080P60B:
//    case DT_VIDSTD_480P59_94:
//    case DT_VIDSTD_525P59_94:
//        return 60;
//
//    case DT_VIDSTD_720P50:
//    case DT_VIDSTD_1080P50:
//    case DT_VIDSTD_1080P50B:
//    case DT_VIDSTD_625P50:
//        return 50;
//
//    case DT_VIDSTD_525I59_94:
//    case DT_VIDSTD_720P29_97:
//    case DT_VIDSTD_720P30:
//    case DT_VIDSTD_1080P30:
//    case DT_VIDSTD_1080P29_97:
//    case DT_VIDSTD_1080PSF30:
//    case DT_VIDSTD_1080PSF29_97:
//    case DT_VIDSTD_1080I59_94:
//    case DT_VIDSTD_1080I60: 
//        return 30;
//
//    case DT_VIDSTD_625I50:
//    case DT_VIDSTD_720P25:
//    case DT_VIDSTD_1080P25:
//    case DT_VIDSTD_1080PSF25:
//    case DT_VIDSTD_1080I50:
//        return 25;
//    
//    case DT_VIDSTD_720P23_98:
//    case DT_VIDSTD_720P24:
//    case DT_VIDSTD_1080P24:
//    case DT_VIDSTD_1080P23_98:
//    case DT_VIDSTD_1080PSF24:
//    case DT_VIDSTD_1080PSF23_98:
//        return 24;
//
//   default:
//        break;
//    }
//    return -1;
//}
//
////.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieVidStdIsFractional -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
////
//Bool  DtPcieVidStdIsFractional(Int  VidStd)
//{
//    switch (VidStd)
//    {
//    case DT_VIDSTD_720P59_94:
//    case DT_VIDSTD_1080P59_94:
//    case DT_VIDSTD_1080P59_94B:
//    case DT_VIDSTD_525I59_94:
//    case DT_VIDSTD_720P29_97:
//    case DT_VIDSTD_1080P29_97:
//    case DT_VIDSTD_1080PSF29_97:
//    case DT_VIDSTD_1080I59_94:
//    case DT_VIDSTD_720P23_98:
//    case DT_VIDSTD_1080P23_98:
//    case DT_VIDSTD_1080PSF23_98:
//    case DT_VIDSTD_480P59_94:
//    case DT_VIDSTD_525P59_94:
//        return TRUE;
//    }
//    return FALSE;
//
//}
//
////.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieVidStdIsInterlaced -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
////
//Bool  DtPcieVidStdIsInterlaced(Int  VidStd)
//{
//    switch (VidStd)
//    {
//    case DT_VIDSTD_525I59_94:
//    case DT_VIDSTD_625I50:
//    case DT_VIDSTD_1080I50:
//    case DT_VIDSTD_1080I59_94:
//    case DT_VIDSTD_1080I60:
//    case DT_VIDSTD_1080PSF23_98:
//    case DT_VIDSTD_1080PSF24:
//    case DT_VIDSTD_1080PSF25:
//    case DT_VIDSTD_1080PSF29_97:
//    case DT_VIDSTD_1080PSF30:
//        return TRUE;
//    }
//    return FALSE;
//}
//
////.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieVidStdIsPsf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
////
//Bool  DtPcieVidStdIsPsf(Int  VidStd)
//{
//    switch (VidStd)
//    {
//    case DT_VIDSTD_1080PSF30:
//    case DT_VIDSTD_1080PSF29_97:
//    case DT_VIDSTD_1080PSF25:
//    case DT_VIDSTD_1080PSF24:
//    case DT_VIDSTD_1080PSF23_98:
//        return TRUE;
//    }
//    return FALSE;
//}
//
////-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieVidStdIs3gSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
////
//Bool  DtPcieVidStdIs3gSdi(Int  VidStd)
//{
//    switch (VidStd)
//    {
//    case DT_VIDSTD_1080P50:
//    case DT_VIDSTD_1080P50B:
//    case DT_VIDSTD_1080P59_94:
//    case DT_VIDSTD_1080P59_94B:
//    case DT_VIDSTD_1080P60:
//    case DT_VIDSTD_1080P60B:
//        return TRUE;
//    }
//    return FALSE;
//}
//
////-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieVidStdIs3glvlBSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
////
//Bool  DtPcieVidStdIs3glvlBSdi(Int  VidStd)
//{
//    switch (VidStd)
//    {
//    case DT_VIDSTD_1080P50B:
//    case DT_VIDSTD_1080P59_94B:
//    case DT_VIDSTD_1080P60B:
//        return TRUE;
//    }
//    return FALSE;
//}
//
////-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieVidStdIsHdSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
////
//Bool  DtPcieVidStdIsHdSdi(Int  VidStd)
//{
//    // Not 3G-SDI and also not SD-SDI, than it must be HD-SDI
//    return (!DtPcieVidStdIs3gSdi(VidStd) && !DtPcieVidStdIsSdSdi(VidStd));
//}
//
////-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieVidStdIsSdSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
////
//Bool  DtPcieVidStdIsSdSdi(Int  VidStd)
//{
//    switch (VidStd)
//    {
//    case DT_VIDSTD_525I59_94:
//    case DT_VIDSTD_625I50:
//        return TRUE;
//    }
//    return FALSE;
//}

