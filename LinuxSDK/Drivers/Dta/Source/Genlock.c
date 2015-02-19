//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Genlock.c *#*#*#*#*#*#*#*#*# (C) 2012-2015 DekTec
//
// Dta driver - Implements genlock support
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012-2015 DekTec Digital Video B.V.
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
#include <DtaIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockCleanupPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaGenlockCleanupPre(DtaDeviceData* pDvcData)
{
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaGenlockCleanup(DtaDeviceData* pDvcData)
{
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaGenlockInit(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    // Initialize properties    
    DtPropertyData*  pPropData = &pDvcData->m_PropData;

    // Assume genlock is not supported
    pDvcData->m_Genlock.m_IsSupported = FALSE;
    pDvcData->m_Genlock.m_OpModeIntSrc = GENLOCK_OPMODE_INTSRC_UNDEF;

    pDvcData->m_Genlock.m_GenlArch = DtPropertiesGetInt(pPropData, "GENLOCK_ARCH", -1);
    // Do we have a valid architecture
    if (pDvcData->m_Genlock.m_GenlArch > 0)
        pDvcData->m_Genlock.m_IsSupported = TRUE;
    
    pDvcData->m_Genlock.m_AsyncPortIndex = DtPropertiesGetInt(pPropData, 
                                                                "GENLOCK_ASYNC_PORT", -1);
           
    if (pDvcData->m_Genlock.m_AsyncPortIndex > 0)
        pDvcData->m_Genlock.m_AsyncPortIndex--;     // Convert to port-index
    else
        pDvcData->m_Genlock.m_AsyncPortIndex = -1;  // No async port

    // Get internal genref port
    pDvcData->m_Genlock.m_IntGenrefPortIndex = DtPropertiesGetInt(pPropData, 
                                                           "GENLOCK_INT_GENREF_PORT", -1);
    if (pDvcData->m_Genlock.m_IntGenrefPortIndex > 0)
        pDvcData->m_Genlock.m_IntGenrefPortIndex--;     // Convert to port-index
    else
        pDvcData->m_Genlock.m_IntGenrefPortIndex = -1;  // No internal genref port


    // Init to 'safe' default values
    pDvcData->m_Genlock.m_FracMode = DTA_GENLOCK_FRACMODE_NA;
    pDvcData->m_Genlock.m_RefPortIndex = pDvcData->m_Genlock.m_IntGenrefPortIndex;
    pDvcData->m_Genlock.m_RefVidStd = DT_VIDSTD_625I50;
    pDvcData->m_Genlock.m_OutVidStd = pDvcData->m_Genlock.m_RefVidStd;
    pDvcData->m_Genlock.m_RefLineDurationNs = 1; // 1ns
    pDvcData->m_Genlock.m_InDelayNs = 0;
    pDvcData->m_Genlock.m_LineOffset = 0;

    if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2152 || 
                                  pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2154)
    {
        UInt16  Offset = -1;
        Offset = DtPropertiesGetUInt16(&pDvcData->m_PropData, "REGISTERS_GENL", -1);
        if (Offset == (UInt16)-1)
        {
            DtDbgOut(ERR, GENL, "Failed to get genlock register offset property");
            return DT_STATUS_FAIL;
        }
        pDvcData->m_Genlock.m_pGenlRegs = (UInt8*)pDvcData->m_DtaRegs.m_pKernel + Offset;
    }
    else
        pDvcData->m_Genlock.m_pGenlRegs = NULL;
        
    if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2152)
    {
        Status = DtaLmh1982Init(pDvcData, &pDvcData->m_Genlock.m_Lmh1982);
        if (!DT_SUCCESS(Status))
            DtDbgOut(ERR, GENL, "Failed to init LMH-1982 module");
    }
    else if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2154)
    {
        // Must have an operational mode
        Int  OpMode = DtPropertiesGetInt(pPropData, "GENLOCK_OPMODE_INTSRC", -1);
        if (OpMode!=GENLOCK_OPMODE_INTSRC_FREE_RUN && OpMode!=GENLOCK_OPMODE_INTSRC_AFD)
        {
            DtDbgOut(ERR, GENL, "Invalid value (%d) for 'GENLOCK_OPMODE_INTSRC' property",
                                                                                  OpMode);
            return DT_STATUS_FAIL;
        }
        pDvcData->m_Genlock.m_OpModeIntSrc = OpMode;
        pDvcData->m_Genlock.m_VcxoValue = -1;
        pDvcData->m_Genlock.m_pVcxoOwner = NULL;

        
        Status = DtaLmh1983Init(pDvcData, &pDvcData->m_Genlock.m_Lmh1983);
        if (!DT_SUCCESS(Status))
            DtDbgOut(ERR, GENL, "Failed to init LMH-1983 module");
    }

    // 145/2145/2144 architecture
    if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2144 || 
                                   pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_145)
        Status = DtaFpgaGenlockInit(pDvcData, &pDvcData->m_Genlock.m_FpgaGenlock);
    
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockInitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaGenlockInitPowerup(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    // Do nothing if Genlock is not supported
    if (!pDvcData->m_Genlock.m_IsSupported)
        return DT_STATUS_OK;

    if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2152)
        Status = DtaLmh1982InitPowerup(&pDvcData->m_Genlock.m_Lmh1982);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockPowerDown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaGenlockPowerDown(DtaDeviceData* pDvcData)
{
    if (!pDvcData->m_Genlock.m_IsSupported)
        return;     // Nothing to do

    if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2152)
        DtaLmh1982Powerdown(&pDvcData->m_Genlock.m_Lmh1982);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockPowerDownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaGenlockPowerDownPre(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    if (!pDvcData->m_Genlock.m_IsSupported)
        return DT_STATUS_OK;

    if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2152)
        Status = DtaLmh1982PowerdownPre(&pDvcData->m_Genlock.m_Lmh1982);
    else if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2154)
        Status = DtaLmh1983PowerdownPre(&pDvcData->m_Genlock.m_Lmh1983);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockResetVcxo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaGenlockSetVcxo(DtaDeviceData* pDvcData, DtFileObject* pFile, Int  VcxoValue)
{
    Bool  SetDevCtrl = (pDvcData->m_Genlock.m_VcxoValue == -1);
    pDvcData->m_Genlock.m_VcxoValue = VcxoValue;
    pDvcData->m_Genlock.m_pVcxoOwner = DtFileGetHandle(pFile);
    DtaLmh1983SetVcxoValue(&pDvcData->m_Genlock.m_Lmh1983,
                                                         pDvcData->m_Genlock.m_VcxoValue);
    if (SetDevCtrl)
        DtaLmh1983SetDevCtrl(&pDvcData->m_Genlock.m_Lmh1983);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockResetVcxo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaGenlockResetVcxo(DtaDeviceData* pDvcData)
{
    if (pDvcData->m_Genlock.m_VcxoValue == -1)
        return;
    pDvcData->m_Genlock.m_VcxoValue = -1;
    pDvcData->m_Genlock.m_pVcxoOwner = NULL;
    // Reset vcxo register to default value
    DtaLmh1983SetVcxoValue(&pDvcData->m_Genlock.m_Lmh1983, 0x1FF);
    // Set LMH-1983 to use default method for locking to clock
    DtaLmh1983SetDevCtrl(&pDvcData->m_Genlock.m_Lmh1983);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaGenlockClose(DtaDeviceData* pDvcData, DtFileObject* pFile)
{
    if (pDvcData->m_Genlock.m_GenlArch != DTA_GENLOCK_ARCH_2154)
        return;
    if (pDvcData->m_Genlock.m_pVcxoOwner != DtFileGetHandle(pFile))
        return;
    DtaGenlockResetVcxo(pDvcData);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockApplyGenRefConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaGenlockApplyGenRefConfig(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtAvFrameProps  RefAvProps;
    Int  i, RefPortIndex, RefVidStd, OutVidStd, Fps;
    DtaNonIpPort*  pNonIpPort = NULL;
    DtaGenlock*  pGenlData = &pDvcData->m_Genlock;
    Bool  EnableGenRef=FALSE, HasCapMatrix2=FALSE;
    
    DT_ASSERT(pGenlData->m_IsSupported);

    // Check if hardware supports Matrix 2.0
    HasCapMatrix2 = FALSE;
    for (i=0; i<pDvcData->m_NumNonIpPorts && !HasCapMatrix2; i++)
    {
        DT_ASSERT(pDvcData->m_pNonIpPorts != NULL);
        // Does the port support matrix 2.0
        if (!pDvcData->m_pNonIpPorts[i].m_CapMatrix2)
            continue;
        HasCapMatrix2 = TRUE;
    }
    
    // Scan all ports to see if there is one that is enabled as genlock reference
    RefPortIndex = pGenlData->m_IntGenrefPortIndex; // Assume internal genref is used
    OutVidStd = RefVidStd = DT_VIDSTD_625I50;
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[i];
        if (!pNonIpPort->m_CapGenRef)
            continue;   // skip if port cannot act as genlock reference
        if (pNonIpPort->m_IoCfg[DT_IOCONFIG_GENREF].m_Value == DT_IOCONFIG_FALSE)
            continue;   // keep looking for a port with genref enabled

        // Copy settings from port io-config
        RefPortIndex = pNonIpPort->m_PortIndex;
        break;
    }
    
    // Did we find a genref port
    if (RefPortIndex != -1)
    {
        DT_ASSERT(RefPortIndex>=0 && RefPortIndex<pDvcData->m_NumNonIpPorts);
        DT_ASSERT(pNonIpPort->m_IoCfg[DT_IOCONFIG_GENREF].m_Value == DT_IOCONFIG_TRUE);
        
        // Reference standard is IO-STD to which this port is configured
        RefVidStd = DtaIoStd2VidStd(pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value, 
                                       pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue);
        // Check the standard is supported
        switch (RefVidStd)
        {
        case DT_VIDSTD_1080P50:     DT_ASSERT(pNonIpPort->m_Cap1080P50); break;
        case DT_VIDSTD_1080P59_94:  DT_ASSERT(pNonIpPort->m_Cap1080P59_94); break;
        case DT_VIDSTD_1080P60:     DT_ASSERT(pNonIpPort->m_Cap1080P60); break;
        case DT_VIDSTD_1080I50:     DT_ASSERT(pNonIpPort->m_Cap1080I50); break;
        case DT_VIDSTD_1080I59_94:  DT_ASSERT(pNonIpPort->m_Cap1080I59_94); break;
        case DT_VIDSTD_1080I60:     DT_ASSERT(pNonIpPort->m_Cap1080I60); break;
        case DT_VIDSTD_1080P23_98:  DT_ASSERT(pNonIpPort->m_Cap1080P23_98); break;
        case DT_VIDSTD_1080P24:     DT_ASSERT(pNonIpPort->m_Cap1080P24); break;
        case DT_VIDSTD_1080P25:     DT_ASSERT(pNonIpPort->m_Cap1080P25); break;
        case DT_VIDSTD_1080P29_97:  DT_ASSERT(pNonIpPort->m_Cap1080P29_97); break;
        case DT_VIDSTD_1080P30:     DT_ASSERT(pNonIpPort->m_Cap1080P30); break;
        case DT_VIDSTD_720P23_98:   DT_ASSERT(pNonIpPort->m_Cap720P23_98); break;
        case DT_VIDSTD_720P24:      DT_ASSERT(pNonIpPort->m_Cap720P24); break;
        case DT_VIDSTD_720P25:      DT_ASSERT(pNonIpPort->m_Cap720P25); break;
        case DT_VIDSTD_720P29_97:   DT_ASSERT(pNonIpPort->m_Cap720P29_97); break;
        case DT_VIDSTD_720P30:      DT_ASSERT(pNonIpPort->m_Cap720P30); break;
        case DT_VIDSTD_720P50:      DT_ASSERT(pNonIpPort->m_Cap720P50); break;
        case DT_VIDSTD_720P59_94:   DT_ASSERT(pNonIpPort->m_Cap720P59_94); break;
        case DT_VIDSTD_720P60:      DT_ASSERT(pNonIpPort->m_Cap720P60); break;
        case DT_VIDSTD_525I59_94:   DT_ASSERT(pNonIpPort->m_Cap525I59_94); break;
        case DT_VIDSTD_625I50:      DT_ASSERT(pNonIpPort->m_Cap625I50); break;
        default:
            DtDbgOut(ERR, NONIP, "Unknown video standard: %d", RefVidStd);
            return DT_STATUS_NOT_SUPPORTED;
        }
        // FOR NOW: we do not support crosslocking, hence output stndard is input standard
        OutVidStd = RefVidStd;  // No cross-lock => out std is ref std
        if (RefPortIndex != pGenlData->m_IntGenrefPortIndex)
            EnableGenRef = TRUE;
    }

    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[i];
        if (!pNonIpPort->m_CapGenRef)
            continue;   // skip if port cannot act as genlock reference
        if (pNonIpPort->m_IoCfg[DT_IOCONFIG_GENREF].m_Value == DT_IOCONFIG_FALSE)
            continue;   // keep looking for a port with genref enabled

        // Copy settings from port io-config
        RefPortIndex = pNonIpPort->m_PortIndex;
        break;
    }

    // Are we using the internal reference?
    if (RefPortIndex == pGenlData->m_IntGenrefPortIndex)
    {
        // Overrule internal reference video standard, is hard-coded based on capabilities
        // Matrix 2.0 hardware uses 720p60 as internal reference, 'old' matrix HW uses 
        // 625I50 as internal reference
        if (HasCapMatrix2)
            RefVidStd = DT_VIDSTD_720P60;
        else
            RefVidStd = DT_VIDSTD_625I50;

        // Overrule output video standard based on fracmode state
        if (pDvcData->m_Genlock.m_FracMode == DTA_GENLOCK_FRACMODE_ON)
            OutVidStd = DT_VIDSTD_525I59_94;   // Set a fractional video standard
        else if (pDvcData->m_Genlock.m_FracMode == DTA_GENLOCK_FRACMODE_OFF)
            OutVidStd = DT_VIDSTD_625I50;   // Set a non-fractional video standard
        // else, i.e hardware has no fractional-mode capability (limitation): the output 
        // video standard is selected by the IO-standard for the GENREF port 
        // (see code above)
    }
    DtDbgOut(MIN, NONIP, "[Port=%d] Ref-video-standard=%d, out-video-standard=%d", 
                                                    RefVidStd, OutVidStd, RefPortIndex+1);
  
    // Cache settings
    pGenlData->m_RefPortIndex = RefPortIndex;
    pGenlData->m_RefVidStd = RefVidStd;
    pGenlData->m_OutVidStd = OutVidStd;
    pGenlData->m_InDelayNs = 0;
    pGenlData->m_LineOffset = 0;

    // Get frame properties for reference signal
    Status = DtAvGetFrameProps(RefVidStd, &RefAvProps);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Compute duration of a single line
    Fps = DtaVidStd2Fps(RefVidStd);
    DT_ASSERT(Fps > 0);
    pGenlData->m_RefLineDurationNs = (1000000000 / Fps) / RefAvProps.m_NumLines;
    if (RefAvProps.m_IsFractional)
        pGenlData->m_RefLineDurationNs = (pGenlData->m_RefLineDurationNs*1001) / 1000;

    // Check architecture type
    if (pGenlData->m_GenlArch == DTA_GENLOCK_ARCH_2152)
    {
        // Input delay depends on which genlock reference is used on reference video 
        // standard
        if (pGenlData->m_RefPortIndex == pGenlData->m_IntGenrefPortIndex)
        {
            // Using the internally generate reference:
            //
            // The internal reference does not introduce any input delay

            pGenlData->m_InDelayNs = 0;
        }
        else if (pGenlData->m_RefPortIndex == pGenlData->m_AsyncPortIndex)
        {
            // Using the analog input:
            //
            // This means the ref signal goes through the LMH-1981 (sync seperator), 
            // which introduces 0.5 line delay for interlaced and a full line delay 
            // for progressive signals

            if (RefAvProps.m_IsInterlaced)
                pGenlData->m_InDelayNs = pGenlData->m_RefLineDurationNs / 2;
            else
                pGenlData->m_InDelayNs = pGenlData->m_RefLineDurationNs;
        }
        else
        {
            // Using one of the SDI ports as reference:
            //
            // This means the signal has to pass through the Gennum/Semtech deserialiser
            // which introduces a delay of xxxx

            pGenlData->m_InDelayNs = 0;

        }
        
        // A change to genref state requires a reset of the LMH1982 state machine (chip 
        // must be re-initialised)
        Status = DtaLmh1982ResetStateMachine(&pGenlData->m_Lmh1982);
        if (!DT_SUCCESS(Status))
            return Status;
    }
    else if (pGenlData->m_GenlArch == DTA_GENLOCK_ARCH_2154)
    {
        // A change to genref state requires a reset of the LMH1983 state machine (chip 
        // must be re-initialised)
        Status = DtaLmh1983ResetStateMachine(&pGenlData->m_Lmh1983);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2144 || 
                                   pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_145)
    {
      // always disable
      Status = DtaFpgaGenlockDisable(pDvcData, &pGenlData->m_FpgaGenlock);
      if (!DT_SUCCESS(Status))
              return Status;

      if (EnableGenRef)
      {
        // Enable genlock      
        Status = DtaFpgaGenlockEnable(pDvcData, &pGenlData->m_FpgaGenlock);
        if (!DT_SUCCESS(Status))
              return Status;
      }      
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockApplyFracModeConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaGenlockApplyFracModeConfig(DtaDeviceData* pDvcData)
{
    Int  i, NewFracMode;
    DtaNonIpPort*  pNonIpPort = NULL;
    DtaGenlock*  pGenlData = &pDvcData->m_Genlock;

    DT_ASSERT(pGenlData->m_IsSupported);

    // Scan all ports to see if there is one for which frac mode is enabled 
    NewFracMode = DTA_GENLOCK_FRACMODE_NA;
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[i];
        if (!pNonIpPort->m_CapFracMode)
            continue;   // skip if port it doesnot support fractional mode

        if (pNonIpPort->m_IoCfg[DT_IOCONFIG_FRACMODE].m_Value == DT_IOCONFIG_FALSE)
            NewFracMode = DTA_GENLOCK_FRACMODE_OFF;
        else if (pNonIpPort->m_IoCfg[DT_IOCONFIG_FRACMODE].m_Value == DT_IOCONFIG_TRUE)
            NewFracMode = DTA_GENLOCK_FRACMODE_ON;

        // Currently fracmode must be the same for all ports, so we can exit the loop here
        break;
    }

    // If new mode is the same as current do nothing
    if (NewFracMode == pGenlData->m_FracMode)
        return DT_STATUS_OK;

    // Cache setting
    pGenlData->m_FracMode = NewFracMode;
    
    // Re-apply genref config to reflect new mode
    return DtaGenlockApplyGenRefConfig(pDvcData);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockGetRefState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Return lock status
//
DtStatus  DtaGenlockGetRefState(DtaDeviceData* pDvcData, 
  Int  PortIndex, 
  Int  *pEnabled,
  Int  *pInLock)
{ 
    DtStatus  Status = DT_STATUS_OK;

#ifdef _DEBUG
    Int  GenRefPortIndex = pDvcData->m_Genlock.m_RefPortIndex;
#endif
    Bool  InLock = FALSE;
    Bool  Enabled = FALSE;
    if(pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2144 ||
                                   pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_145)
    {
        Status = DtaFpgaGenlockGetRefState(pDvcData, PortIndex, &Enabled, &InLock);
        *pEnabled = Enabled == TRUE ? 1 : 0;
        *pInLock = InLock == TRUE ? 1 : 0;
        return Status;
    }

    // TODO: Should we also implement this for HDSDI?
    //if(pDvcData->m_Genlock.m_GenlArch != DTA_GENLOCK_ARCH_2152)
    //  return DtaLmh1982GenlockGetRefState(pDvcData, pInLock);

    DtDbgOut(ERR, GENL, "[%d] GENREF NOT SUPPORTED", GenRefPortIndex);
    return DT_STATUS_NOT_SUPPORTED;
}
