//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Genlock.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2012 DekTec
//
// Dta driver - Implements genlock support

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012 DekTec Digital Video B.V.
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

    // Init to 'safe' default values
    pDvcData->m_Genlock.m_FracMode = DTA_GENLOCK_FRACMODE_NA;
    pDvcData->m_Genlock.m_RefPortIndex = DTA_GENLOCK_REFPORT_INT;
    pDvcData->m_Genlock.m_RefVidStd = DT_VIDSTD_625I50;
    
    if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2152)
    {
        Status = DtaLmh1982Init(pDvcData, &pDvcData->m_Genlock.m_Lmh1982);
        if (!DT_SUCCESS(Status))
            DtDbgOut(ERR, GENL, "Failed to init LMH-1982 module");       
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
        DtaLmh1982PowerdownPre(&pDvcData->m_Genlock.m_Lmh1982);
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
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGenlockApplyGenRefConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaGenlockApplyGenRefConfig(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i, RefPortIndex, RefVidStd;
    DtaNonIpPort*  pNonIpPort = NULL;
    DtaGenlock*  pGenlData = &pDvcData->m_Genlock;
    Bool  EnableGenRef = FALSE;

    DT_ASSERT(pGenlData->m_IsSupported);
    
    // Scan all ports to see if there is one that is enabled as genlock reference
    RefPortIndex = DTA_GENLOCK_REFPORT_INT;
    RefVidStd = DT_VIDSTD_625I50;
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        pNonIpPort = &pDvcData->m_NonIpPorts[i];
        if (!pNonIpPort->m_CapGenRef)
            continue;   // skip if port cannot act as genlock reference
        if (pNonIpPort->m_IoCfg[DT_IOCONFIG_GENREF].m_Value == DT_IOCONFIG_FALSE)
            continue;   // keep locking for a port with genref enabled

        // Copy settings from port io-config
        RefPortIndex = pNonIpPort->m_PortIndex;
        RefVidStd = (Int)pNonIpPort->m_IoCfg[DT_IOCONFIG_GENREF].m_ParXtra[0];
        EnableGenRef = TRUE;
        break;
    }

    // If internal genlock reference auto set reference video standard based on fracmode
    // state
    if (RefPortIndex == DTA_GENLOCK_REFPORT_INT)
    {
        if (pDvcData->m_Genlock.m_FracMode == DTA_GENLOCK_FRACMODE_ON)
            RefVidStd = DT_VIDSTD_525I59_94;   // Set a fractional video standard
        else if (pDvcData->m_Genlock.m_FracMode == DTA_GENLOCK_FRACMODE_OFF)
            RefVidStd = DT_VIDSTD_625I50;   // Set a non-fractional video standard
    }

     // Cache settings
    pGenlData->m_RefPortIndex = RefPortIndex;
    pGenlData->m_RefVidStd = RefVidStd;

    // Check architecture type
    if (pGenlData->m_GenlArch == DTA_GENLOCK_ARCH_2152)
    {
        // A change to genref state requires a reset of the LMH1982 state machine (chip 
        // must be re-initialised)
        Status = DtaLmh1982ResetStateMachine(&pGenlData->m_Lmh1982);
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
        pNonIpPort = &pDvcData->m_NonIpPorts[i];
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

  Int  GenRefPortIndex = pDvcData->m_Genlock.m_RefPortIndex;
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
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
