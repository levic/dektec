//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NonIp.c *#*#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// Dtu driver - Non IP functionality - Implementation of generic non IP port functionality
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtuIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuNonIpInit(
    DtuDeviceData*  pDvcData,
    Int  PortIndex,                     // Physical port index
    DtuNonIpPort*  pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  IoConfig, ParXtra;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
        
    // Initialize common members
    pNonIpPort->m_pDvcData = pDvcData;
    pNonIpPort->m_PortIndex = PortIndex;

    // Capabilities
    // IODIR (I/O direction) - Capabilities
    pNonIpPort->m_CapDisabled = DtPropertiesGetBool(pPropData, "CAP_DISABLED",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapInput = DtPropertiesGetBool(pPropData, "CAP_INPUT",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapOutput = DtPropertiesGetBool(pPropData, "CAP_OUTPUT",
                                                                 pNonIpPort->m_PortIndex);
    // IODIR - INPUT (Uni-directional input) - Sub capabilities
    pNonIpPort->m_CapSharedAnt = DtPropertiesGetBool(pPropData, "CAP_SHAREDANT",
                                                                 pNonIpPort->m_PortIndex);
    // IODIR - OUTPUT (Uni-directional output) - Sub capabilities
    pNonIpPort->m_CapDblBuf = DtPropertiesGetBool(pPropData, "CAP_DBLBUF",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapLoopS2L3 = DtPropertiesGetBool(pPropData, "CAP_LOOPS2L3",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapLoopS2Ts = DtPropertiesGetBool(pPropData, "CAP_LOOPS2TS",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapLoopThr = DtPropertiesGetBool(pPropData, "CAP_LOOPTHR",
                                                                 pNonIpPort->m_PortIndex);
    // IOSTD (I/O standard) - Capabilities
    pNonIpPort->m_CapAsi = DtPropertiesGetBool(pPropData, "CAP_ASI",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapDemod = DtPropertiesGetBool(pPropData, "CAP_DEMOD",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapIp = DtPropertiesGetBool(pPropData, "CAP_IP",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapMod = DtPropertiesGetBool(pPropData, "CAP_MOD",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSdi = DtPropertiesGetBool(pPropData, "CAP_SDI",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpi = DtPropertiesGetBool(pPropData, "CAP_SPI",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiSdi = DtPropertiesGetBool(pPropData, "CAP_SPISDI",
                                                                 pNonIpPort->m_PortIndex);
    // IOSTD - SDI (SD-SDI) - Sub capabilities
    pNonIpPort->m_Cap525I59_94 = DtPropertiesGetBool(pPropData, "CAP_525I59_94",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap625I50 = DtPropertiesGetBool(pPropData, "CAP_625I50",
                                                                 pNonIpPort->m_PortIndex);
    // RFCLKSEL (RF clock source selection) - Capabilities
    pNonIpPort->m_CapRfClkExt = DtPropertiesGetBool(pPropData, "CAP_RFCLKEXT",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapRfClkInt = DtPropertiesGetBool(pPropData, "CAP_RFCLKINT",
                                                                 pNonIpPort->m_PortIndex);
    // SPICLKSEL (Parallel port clock source selection) - Capabilities
    pNonIpPort->m_CapSpiClkExt = DtPropertiesGetBool(pPropData, "CAP_SPICLKEXT",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiClkInt = DtPropertiesGetBool(pPropData, "CAP_SPICLKINT",
                                                                 pNonIpPort->m_PortIndex);
    // SPIMODE (Parallel port mode) - Capabilities
    pNonIpPort->m_CapSpiFixedClk = DtPropertiesGetBool(pPropData, "CAP_SPIFIXEDCLK",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiDvbMode = DtPropertiesGetBool(pPropData, "CAP_SPIDVBMODE",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiSer8B = DtPropertiesGetBool(pPropData, "CAP_SPISER8B",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiSer10B = DtPropertiesGetBool(pPropData, "CAP_SPISER10B",
                                                                 pNonIpPort->m_PortIndex);
    // SPISTD (Parallel port I/O standard) - Capabilities
    pNonIpPort->m_CapSpiLvds1 = DtPropertiesGetBool(pPropData, "CAP_SPILVDS1",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiLvds2 = DtPropertiesGetBool(pPropData, "CAP_SPILVDS2",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiLvttl = DtPropertiesGetBool(pPropData, "CAP_SPILVTTL",
                                                                 pNonIpPort->m_PortIndex);
    // TSRATESEL (Transport-stream rate selection) - Capabilities
    pNonIpPort->m_CapExtTsRate = DtPropertiesGetBool(pPropData, "CAP_EXTTSRATE",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapExtRatio = DtPropertiesGetBool(pPropData, "CAP_EXTRATIO",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapIntTsRate = DtPropertiesGetBool(pPropData, "CAP_INTTSRATE",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapLock2Inp = DtPropertiesGetBool(pPropData, "CAP_LOCK2INP",
                                                                 pNonIpPort->m_PortIndex);
    // BOOLIO (Boolean I/O capabilities) - Capabilities
    pNonIpPort->m_CapFailSafe = DtPropertiesGetBool(pPropData, "CAP_FAILSAFE",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapGenLocked = DtPropertiesGetBool(pPropData, "CAP_GENLOCKED",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapGenRef = DtPropertiesGetBool(pPropData, "CAP_GENREF",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSwS2Apsk = DtPropertiesGetBool(pPropData, "CAP_SWS2APSK",
                                                                 pNonIpPort->m_PortIndex);  

    // Is it a bidir port?
    pNonIpPort->m_IsBidir = (pNonIpPort->m_CapInput && pNonIpPort->m_CapOutput);

    // Initial default IO configurations
    // Initialze all parxtra to -1 as default value to prevent SetIoConfig from detecting
    // ParXtra changes while parxtra is not used in the current IO configuration
    for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
    {
        pNonIpPort->m_IoCfg[IoConfig].m_Value = DT_IOCONFIG_NONE;
        pNonIpPort->m_IoCfg[IoConfig].m_SubValue = DT_IOCONFIG_NONE;

        for (ParXtra=0; ParXtra<DT_MAX_PARXTRA_COUNT; ParXtra++)
        {
            pNonIpPort->m_IoCfg[IoConfig].m_ParXtra[ParXtra] = -1;
        }
    }
    // DTu_IOCONFIG_IOCONFIG
    DT_ASSERT(pNonIpPort->m_CapInput || pNonIpPort->m_CapOutput 
                                  || pNonIpPort->m_CapDblBuf || pNonIpPort->m_CapLoopThr);
    
    // If the port is not a configurable input or output port,
    // we don't have a DEFAULT_IOCONFIG property.
    if (pNonIpPort->m_CapDblBuf || pNonIpPort->m_CapLoopThr || pNonIpPort->m_IsBidir
        || pNonIpPort->m_CapDisabled)
    {
        Int ParXtra = -1;
        Int IodirVal = DtPropertiesGetInt(pPropData, "DEFAULT_IODIR", 
                                                                 pNonIpPort->m_PortIndex);
        switch (IodirVal)
        {
        case DT_IOCONFIG_DISABLED:
            DT_ASSERT(pNonIpPort->m_CapDisabled);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_DISABLED;
            break;
        case DT_IOCONFIG_INPUT:
            DT_ASSERT(pNonIpPort->m_CapInput);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_INPUT;
            break;
        case DT_IOCONFIG_SHAREDANT:
            DT_ASSERT(pNonIpPort->m_CapSharedAnt);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_SHAREDANT;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA0", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        case DT_IOCONFIG_OUTPUT:
            DT_ASSERT(pNonIpPort->m_CapOutput);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_OUTPUT;
            break;
        case DT_IOCONFIG_DBLBUF:
            DT_ASSERT(pNonIpPort->m_CapDblBuf);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_DBLBUF;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA0", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        case DT_IOCONFIG_LOOPS2L3:
            DT_ASSERT(pNonIpPort->m_CapLoopS2L3);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_LOOPS2L3;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA0", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        case DT_IOCONFIG_LOOPS2TS:
            DT_ASSERT(pNonIpPort->m_CapLoopS2Ts);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_LOOPS2TS;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA0", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA1", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[1] = ParXtra;
            break;
        case DT_IOCONFIG_LOOPTHR:
            DT_ASSERT(pNonIpPort->m_CapLoopThr);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_LOOPTHR;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA0", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        default:
            DtDbgOut(ERR, NONIP, "Unsupported DEFAULT_IODIR for board type: %d port: %d",
                                      pPropData->m_TypeNumber, pNonIpPort->m_PortIndex+1);
            DT_ASSERT(FALSE);
            break;
        }
    }
    else if (pNonIpPort->m_CapInput)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INPUT;
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_INPUT;
    }
    else if (pNonIpPort->m_CapOutput)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_OUTPUT;
    }
    else
    {
        // IODIR must have a value
        DT_ASSERT(FALSE);
    }

    // DT_IOCONFIG_IOSTD
    if (pNonIpPort->m_CapAsi)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_ASI;
    else if (pNonIpPort->m_CapSdi)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDI;
        if (!pNonIpPort->m_Cap625I50 && pNonIpPort->m_Cap525I59_94)
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_525I59_94;
    else
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_625I50;
    }
    else if (pNonIpPort->m_CapSpi)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPI;
    else if (pNonIpPort->m_CapSpiSdi)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPISDI;
        if (!pNonIpPort->m_Cap625I50 && pNonIpPort->m_Cap525I59_94)
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_525I59_94;
        else
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_625I50;
    }
    else if (pNonIpPort->m_CapDemod)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_DEMOD;
    else if (pNonIpPort->m_CapIp)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_IP;
    else if (pNonIpPort->m_CapMod)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_MOD;
    else 
    {
        // IOSTD must have a value
        DT_ASSERT(FALSE);
    }

    // DT_IOCONFIG_RFCLKSEL
    if (pNonIpPort->m_CapRfClkInt) 
        pNonIpPort->m_IoCfg[DT_IOCONFIG_RFCLKSEL].m_Value = DT_IOCONFIG_RFCLKINT;
    else if (pNonIpPort->m_CapRfClkExt)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_RFCLKSEL].m_Value = DT_IOCONFIG_RFCLKEXT;

    // DT_IOCONFIG_SPICLKSEL
    if (pNonIpPort->m_CapSpiClkInt)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPICLKSEL].m_Value = DT_IOCONFIG_SPICLKINT;
    else if (pNonIpPort->m_CapSpiClkExt)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPICLKSEL].m_Value = DT_IOCONFIG_SPICLKEXT;

    // DT_IOCONFIG_SPIMODE
    if (pNonIpPort->m_CapSpiDvbMode)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPIMODE].m_Value = DT_IOCONFIG_SPIDVBMODE;
    else if (pNonIpPort->m_CapSpiFixedClk)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPIMODE].m_Value = DT_IOCONFIG_SPIFIXEDCLK;
    else if (pNonIpPort->m_CapSpiSer8B)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPIMODE].m_Value = DT_IOCONFIG_SPISER8B;
    else if (pNonIpPort->m_CapSpiSer10B)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPIMODE].m_Value = DT_IOCONFIG_SPISER10B;

    // DT_IOCONFIG_SPISTD
    if (pNonIpPort->m_CapSpi)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPISTD].m_Value = DtPropertiesGetInt(
                                    pPropData, "DEFAULT_SPISTD", pNonIpPort->m_PortIndex);

    // DT_IOCONFIG_TSRATESEL
    if (pNonIpPort->m_CapIntTsRate)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_TSRATESEL].m_Value = DT_IOCONFIG_INTTSRATE;
    else if (pNonIpPort->m_CapExtTsRate)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_TSRATESEL].m_Value = DT_IOCONFIG_EXTTSRATE;
    else if (pNonIpPort->m_CapExtRatio)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_TSRATESEL].m_Value = DT_IOCONFIG_EXTRATIO;
    else if (pNonIpPort->m_CapLock2Inp)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_TSRATESEL].m_Value = DT_IOCONFIG_LOCK2INP;

    // DT_IOCONFIG_GENREF
    if (pNonIpPort->m_CapGenRef)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_GENREF].m_Value = DT_IOCONFIG_FALSE;
    
    // DT_IOCONFIG_SWS2APSK
    if (pNonIpPort->m_CapSwS2Apsk)   
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SWS2APSK].m_Value = DT_IOCONFIG_FALSE;
    
    // DT_IOCONFIG_FAILSAFE
    if (pNonIpPort->m_CapFailSafe)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_FAILSAFE].m_Value = DT_IOCONFIG_FALSE;
    
    // DT_IOCONFIG_GENLOCK
    if (pNonIpPort->m_CapGenLocked)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_GENLOCKED].m_Value = DT_IOCONFIG_FALSE;


    // Initialize register mappings
    pNonIpPort->m_RxRegsOffset = -1;
    pNonIpPort->m_TxRegsOffset = -1;
    if (pNonIpPort->m_CapInput)
        pNonIpPort->m_RxRegsOffset = (Int)DtPropertiesGetUInt16(pPropData, "REGISTERS_RX",
                                                                 pNonIpPort->m_PortIndex);
    if (pNonIpPort->m_CapOutput)
        pNonIpPort->m_TxRegsOffset = (Int)DtPropertiesGetUInt16(pPropData, "REGISTERS_TX",
                                                                 pNonIpPort->m_PortIndex);

    // Initialize temporariy buffer
    pNonIpPort->m_TempBufWrIndex = 0;
    pNonIpPort->m_TempBufRdIndex = 0;
    pNonIpPort->m_pTempBuf = (UInt8*)DtMemAllocPool(DtPoolNonPaged,
                                                            DTU_BULK_PACKETSIZE, DTU_TAG);
    if (pNonIpPort->m_pTempBuf == NULL)
        Status = DT_STATUS_OUT_OF_MEMORY;

    // Report configuration errors
    if (DT_SUCCESS(Status))
        // Check if no property error occurred
        Status = DtuPropertiesReportDriverErrors(pNonIpPort->m_pDvcData);

    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpCleanupPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtuNonIpCleanupPre(
    DtuDeviceData*  pDvcData, 
    Int  PortIndex,
    DtuNonIpPort*  pNonIpPort)
{
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtuNonIpCleanup(DtuDeviceData* pDvcData, Int PortIndex, DtuNonIpPort* pNonIpPort)
{
    // Free buffer
    if (pNonIpPort->m_pTempBuf != NULL)
        DtMemFreePool(pNonIpPort->m_pTempBuf, DTU_TAG);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpUpdateSdiModes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Helper function to update the SDI line mode and SDI enable after switching the 
// direction of a Bi-dir port or switching from ASI to SDI mode
//
DtStatus  DtuNonIpUpdateSdiModes(
    DtuNonIpPort*  pNonIpPort,
    Bool  IsInput,
    Bool  IsSdi)
{
    if (IsInput)
    {
        // Update TS Stream format
        if (IsSdi)
            DtuNonIpRxRegWriteMasked(pNonIpPort, DT_RX_REG_RXCTRL, 
                                          DT_RXCTRL_SDIMODE_MSK, DT_RXCTRL_SDIMODE_SH, 1);
        else
            DtuNonIpRxRegWriteMasked(pNonIpPort, DT_RX_REG_RXCTRL, 
                                         DT_RXCTRL_SDIMODE_MSK, DT_RXCTRL_SDIMODE_SH, 0);
    } else {   
        // Update TS Stream format
        if (IsSdi)
        {
            DtuNonIpTxRegWriteMasked(pNonIpPort, DT_TX_REG_TXCTRL, 
                                        DT_TXCTRL_SDIMODE_MSK, DT_TXCTRL_SDIMODE_SH, 1);

            // Update SDI-line mode
            if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue 
                                                                 == DT_IOCONFIG_525I59_94)
                DtuNonIpTxRegWriteMasked(pNonIpPort, DT_TX_REG_TXCTRL, 
                              DT_TXCTRL_SDI_LINEMODE_MSK, DT_TXCTRL_SDI_LINEMODE_SH, 1);
            else
                DtuNonIpTxRegWriteMasked(pNonIpPort, DT_TX_REG_TXCTRL, 
                              DT_TXCTRL_SDI_LINEMODE_MSK, DT_TXCTRL_SDI_LINEMODE_SH, 0);
        } else
            DtuNonIpTxRegWriteMasked(pNonIpPort, DT_TX_REG_TXCTRL, 
                                        DT_TXCTRL_SDIMODE_MSK, DT_TXCTRL_SDIMODE_SH, 0);
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpIoConfigGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuNonIpIoConfigGet(
    DtuNonIpPort*  pNonIpPort,
    Int  Code,
    DtuIoConfigValue*  pCfgValue)
{
    *pCfgValue = pNonIpPort->m_IoCfg[Code];
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpIoConfigSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuNonIpIoConfigSet(
    DtuNonIpPort*  pNonIpPort,
    Int  Group,
    DtuIoConfigValue  CfgValue)
{
    DtuIoConfigValue  IoDirValue;
    DtuNonIpIoConfigGet(pNonIpPort, DT_IOCONFIG_IODIR, &IoDirValue);
    switch (Group)
    {
        // IO-direction
    case DT_IOCONFIG_IODIR:
        switch (CfgValue.m_Value)
        {
        case DT_IOCONFIG_DISABLED:
            // New value (DT_IOCONFIG_DISABLED) is saved at the end...
            break;

        case DT_IOCONFIG_INPUT:
            DT_ASSERT(pNonIpPort->m_CapInput);
            switch (CfgValue.m_SubValue)
            {
            case DT_IOCONFIG_INPUT:
               DT_ASSERT(pNonIpPort->m_CapInput);
                // For DTU-236 set DEMOD/ASI input mode
                if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber==236)
                    Dtu236DemodModeSet(pNonIpPort->m_pDvcData, pNonIpPort->m_CapDemod);
                break;
            case DT_IOCONFIG_SHAREDANT:
            default:
                DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
                return DT_STATUS_NOT_SUPPORTED;
                break;
            }
            break;

        case DT_IOCONFIG_OUTPUT:
            DT_ASSERT(pNonIpPort->m_CapOutput);

            switch (CfgValue.m_SubValue)
            {
            case DT_IOCONFIG_OUTPUT:

                // Switch off loop though 
                if (pNonIpPort->m_CapLoopThr)
                    DtuNonIpTxRegWriteMasked(pNonIpPort, DT_TX_REG_TXCTRL2, 
                                              DT_TXCTRL2_SRCSEL_MSK, DT_TXCTRL2_SRCSEL_SH,
                                                                 pNonIpPort->m_PortIndex);
                // else no action required
                break;

            case DT_IOCONFIG_DBLBUF:
            case DT_IOCONFIG_LOOPS2L3:
            case DT_IOCONFIG_LOOPS2TS:
                DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
                return DT_STATUS_NOT_SUPPORTED;
                break;

            case DT_IOCONFIG_LOOPTHR:
                DT_ASSERT(pNonIpPort->m_CapLoopThr);
                // Route output to loop-through-buddy
                DtuNonIpTxRegWriteMasked(pNonIpPort, DT_TX_REG_TXCTRL2, 
                                              DT_TXCTRL2_SRCSEL_MSK, DT_TXCTRL2_SRCSEL_SH,
                                                           (UInt32)CfgValue.m_ParXtra[0]);
                break;
            default:
                DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
                return DT_STATUS_NOT_SUPPORTED;
            }
            break;
        default:
            DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
            return DT_STATUS_NOT_SUPPORTED;
        }
        break;

        // I/O standard
    case DT_IOCONFIG_IOSTD:
        switch (CfgValue.m_Value)
        {
        case DT_IOCONFIG_ASI:
            DT_ASSERT(pNonIpPort->m_CapAsi);
            // Switch off SDI
            if (pNonIpPort->m_CapSdi)
            {
                if (IoDirValue.m_Value == DT_IOCONFIG_OUTPUT)
                    DtuNonIpTxRegWriteMasked(pNonIpPort, DT_TX_REG_TXCTRL, 
                                          DT_TXCTRL_SDIMODE_MSK, DT_TXCTRL_SDIMODE_SH, 0);
                else if (IoDirValue.m_Value == DT_IOCONFIG_INPUT)
                    DtuNonIpRxRegWriteMasked(pNonIpPort, DT_RX_REG_RXCTRL, 
                                          DT_RXCTRL_SDIMODE_MSK, DT_RXCTRL_SDIMODE_SH, 0);
            }
            break;
        case DT_IOCONFIG_SDI:
            DT_ASSERT(pNonIpPort->m_CapSdi);
            DtuNonIpUpdateSdiModes(pNonIpPort, 
                                      IoDirValue.m_Value==DT_IOCONFIG_INPUT, TRUE);

            switch (CfgValue.m_SubValue)
            {
            case DT_IOCONFIG_525I59_94:
                DT_ASSERT(pNonIpPort->m_Cap525I59_94);
                DT_ASSERT(IoDirValue.m_Value == DT_IOCONFIG_OUTPUT ||
                          IoDirValue.m_Value == DT_IOCONFIG_INPUT);
                // Set 525 line mode
                if (IoDirValue.m_Value == DT_IOCONFIG_OUTPUT)
                    DtuNonIpTxRegWriteMasked(pNonIpPort, DT_TX_REG_TXCTRL, 
                              DT_TXCTRL_SDI_LINEMODE_MSK, DT_TXCTRL_SDI_LINEMODE_SH, 1);
                break;
            case DT_IOCONFIG_625I50:
                DT_ASSERT(pNonIpPort->m_Cap625I50);
                DT_ASSERT(IoDirValue.m_Value == DT_IOCONFIG_OUTPUT ||
                          IoDirValue.m_Value == DT_IOCONFIG_INPUT);
                // Set 625 line mode
                if (IoDirValue.m_Value == DT_IOCONFIG_OUTPUT)
                    DtuNonIpTxRegWriteMasked(pNonIpPort, DT_TX_REG_TXCTRL, 
                              DT_TXCTRL_SDI_LINEMODE_MSK, DT_TXCTRL_SDI_LINEMODE_SH, 0);
                break;
            default:
                DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
                return DT_STATUS_NOT_SUPPORTED;
            }
            break;
        case DT_IOCONFIG_SPI:
        case DT_IOCONFIG_SPISDI:
        case DT_IOCONFIG_IP:
            DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
            return DT_STATUS_NOT_SUPPORTED;
            break;
        case DT_IOCONFIG_MOD:
        case DT_IOCONFIG_DEMOD:
            // No action required
            break;
        default:
            DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
            return DT_STATUS_NOT_SUPPORTED;
        }
        break;

        // RF clock source selection
    case DT_IOCONFIG_RFCLKSEL:
        switch (CfgValue.m_Value)
        {
        case DT_IOCONFIG_RFCLKINT:
            DT_ASSERT(pNonIpPort->m_CapRfClkInt);
            // No actions required
            break;
        case DT_IOCONFIG_RFCLKEXT:
        default:
            DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
            return DT_STATUS_NOT_SUPPORTED;
        }
        break;

        // Parallel port clock source selection
    case DT_IOCONFIG_SPICLKSEL:  
        // Parallel port mode
    case DT_IOCONFIG_SPIMODE:
        // Parallel port I/O standard
    case DT_IOCONFIG_SPISTD:
        DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;

        // Transport-stream rate selection
    case DT_IOCONFIG_TSRATESEL:
        switch (CfgValue.m_Value)
        {
        case DT_IOCONFIG_INTTSRATE:
            // No actions required
            break;
        case DT_IOCONFIG_EXTTSRATE:
        case DT_IOCONFIG_EXTRATIO:
        case DT_IOCONFIG_LOCK2INP:
        default:
            DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
            return DT_STATUS_NOT_SUPPORTED;
        }
        break;

        // Fail-over relais available
    case DT_IOCONFIG_FAILSAFE:
        // Locked to a genlock reference
    case DT_IOCONFIG_GENLOCKED:
        // Genlock reference input
    case DT_IOCONFIG_GENREF:
        // DVB-S2 APSK switch
    case DT_IOCONFIG_SWS2APSK:
    default:
        DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Save new config to the cache
    pNonIpPort->m_IoCfg[Group] = CfgValue;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuNonIpIoctl(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;
    Int  NonIpPortIndex;        // Index in the nonip port struct
    DtuIoctlNonIpCmdInput* pNonIpCmdInput = 
                                           (DtuIoctlNonIpCmdInput*)pIoctl->m_pInputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtuIoctlNonIpCmdInput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    if (!DT_SUCCESS(DtuGetNonIpPortIndex(pDvcData, pNonIpCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
        return DT_STATUS_INVALID_PARAMETER;

    // Determine final required output/input sizes
    switch (pNonIpCmdInput->m_Cmd)
    {
    case DTU_NONIP_CMD_EXCLUSIVE_ACCESS:
        pCmdStr = "DTU_NONIP_CMD_EXCLUSIVE_ACCESS";
        InReqSize += sizeof(DtuIoctlNonIpCmdExclusiveAccessInput);
        break;
    default:
        pCmdStr = "??UNKNOWN VPDCMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, NONIP, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, NONIP, "%s: In=%d (Rq=%d)", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pNonIpCmdInput->m_Cmd)
        {
        case DTU_NONIP_CMD_EXCLUSIVE_ACCESS:
            Status = DtuNonIpExclusiveAccess(
                                   &pDvcData->m_NonIpPorts[NonIpPortIndex],
                                   pFile, pNonIpCmdInput->m_Data.m_ExclusiveAccess.m_Cmd);
            break;
        default:
            Status = DT_STATUS_NOT_SUPPORTED;
        }
    }

    // If we failed, no data has te be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, NONIP, "NonIpCmd=0x%x: NOT SUPPORTED", pNonIpCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, NONIP, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpRxIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuNonIpRxIoctl(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;
    Int  NonIpPortIndex;        // Index in the nonip port struct
    DtuIoctlNonIpRxCmdInput* pNonIpRxCmdInput = 
                                           (DtuIoctlNonIpRxCmdInput*)pIoctl->m_pInputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtuIoctlNonIpRxCmdInput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    if (!DT_SUCCESS(DtuGetNonIpPortIndex(pDvcData, pNonIpRxCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
        return DT_STATUS_INVALID_PARAMETER;

    // Determine final required output/input sizes
    switch (pNonIpRxCmdInput->m_Cmd)
    {
    case DTU_NONIP_XX_CMD_RESET:
        pCmdStr = "DTU_NONIP_XX_CMD_RESET";
        InReqSize += sizeof(DtuIoctlNonIpXxCmdResetInput);
        break;
    default:
        pCmdStr = "??UNKNOWN NONIP RX CMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, NONIP, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, NONIP, "%s: In=%d (Rq=%d)", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pNonIpRxCmdInput->m_Cmd)
        {
        case DTU_NONIP_XX_CMD_RESET:
            Status = DtuNonIpRxReset(pDvcData, &pDvcData->m_NonIpPorts[NonIpPortIndex],
                                            pNonIpRxCmdInput->m_Data.m_Reset.m_ResetMode);
            break;
        default:
            Status = DT_STATUS_NOT_SUPPORTED;
        }
    }

    // If we failed, no data has te be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, NONIP, "NonIpRxCmd=0x%x: NOT SUPPORTED", pNonIpRxCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, NONIP, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpTxIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuNonIpTxIoctl(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;
    Int  NonIpPortIndex;        // Index in the nonip port struct
    DtuIoctlNonIpTxCmdInput* pNonIpTxCmdInput = 
                                           (DtuIoctlNonIpTxCmdInput*)pIoctl->m_pInputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtuIoctlNonIpTxCmdInput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    if (!DT_SUCCESS(DtuGetNonIpPortIndex(pDvcData, pNonIpTxCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
        return DT_STATUS_INVALID_PARAMETER;

    // Determine final required output/input sizes
    switch (pNonIpTxCmdInput->m_Cmd)
    {
    case DTU_NONIP_XX_CMD_RESET:
        pCmdStr = "DTU_NONIP_XX_CMD_RESET";
        InReqSize += sizeof(DtuIoctlNonIpXxCmdResetInput);
        break;
    default:
        pCmdStr = "??UNKNOWN NONIP TX CMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, NONIP, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, NONIP, "%s: In=%d (Rq=%d)", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pNonIpTxCmdInput->m_Cmd)
        {
        case DTU_NONIP_XX_CMD_RESET:
            Status = DtuNonIpTxReset( pDvcData, &pDvcData->m_NonIpPorts[NonIpPortIndex],
                                           pNonIpTxCmdInput->m_Data.m_Reset.m_ResetMode);
            break;
        default:
            Status = DT_STATUS_NOT_SUPPORTED;
        }
    }

    // If we failed, no data has te be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, NONIP, "NonIpTxCmd=0x%x: NOT SUPPORTED", pNonIpTxCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, NONIP, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Exclusive access -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpExclusiveAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuNonIpExclusiveAccess(
    DtuNonIpPort*  pNonIpPort,
    DtFileObject*  pFile,
    Int  Cmd)
{
    DtStatus  Result = DT_STATUS_OK;

    DtFastMutexAcquire(&pNonIpPort->m_pDvcData->m_ExclAccessMutex);

    if (Cmd == DTU_EXCLUSIVE_ACCESS_CMD_ACQUIRE)
    {
        if (!pNonIpPort->m_ExclAccess)
        {
            pNonIpPort->m_ExclAccess = TRUE;
            pNonIpPort->m_ExclAccessOwner = *pFile;
        } else
            Result = DT_STATUS_IN_USE;
    } else if (Cmd == DTU_EXCLUSIVE_ACCESS_CMD_RELEASE)
    {
        if (pNonIpPort->m_ExclAccess)
        {
            if (DtFileCompare(&pNonIpPort->m_ExclAccessOwner, pFile))
                pNonIpPort->m_ExclAccess = FALSE;
            else
                Result = DT_STATUS_IN_USE;
        }
    } else if (Cmd == DTU_EXCLUSIVE_ACCESS_CMD_PROBE)
    {
        if (pNonIpPort->m_ExclAccess)
            Result = DT_STATUS_IN_USE;
    } else
        Result = DT_STATUS_NOT_SUPPORTED;

    DtFastMutexRelease(&pNonIpPort->m_pDvcData->m_ExclAccessMutex);

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpHasAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// PRE: ExclAccessMutex mutex must be acquired!!!
//
DtStatus  DtuNonIpHasAccess(DtuNonIpPort* pNonIpPort, DtFileObject* pFile)
{
    if (pFile == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    if (pNonIpPort->m_ExclAccess)
    {
        if (!DtFileCompare(&pNonIpPort->m_ExclAccessOwner, pFile))
            return DT_STATUS_IN_USE;
    }
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpReleaseResourceFromFileObject -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuNonIpReleaseResourceFromFileObject(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile)
{
    Int i;
    for (i = 0; i < pDvcData->m_NumNonIpPorts; i++) 
    {
        // Release exclusive access
        if (pDvcData->m_NonIpPorts[i].m_ExclAccess)
        {
            DtFastMutexAcquire(&pDvcData->m_ExclAccessMutex);
            if (DtFileCompare(&pDvcData->m_NonIpPorts[i].m_ExclAccessOwner, pFile) && 
                                                   pDvcData->m_NonIpPorts[i].m_ExclAccess)
            {
                pDvcData->m_NonIpPorts[i].m_ExclAccess = FALSE;
                DtDbgOut(AVG, DTU, "Release exclusive access for port %i", 
                                                   pDvcData->m_NonIpPorts[i].m_PortIndex);
            }
            DtFastMutexRelease(&pDvcData->m_ExclAccessMutex);
        }
    }
    return DT_STATUS_OK; 
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpRxRegWriteMasked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtuNonIpRxRegWriteMasked(DtuNonIpPort* pNonIpPort,UInt32 Addr, UInt32 Mask,
                                                               UInt32 Shift, UInt32 Value)
{
    DT_ASSERT(pNonIpPort->m_RxRegsOffset != -1);
    return DtuRegWriteMasked(pNonIpPort->m_pDvcData, pNonIpPort->m_RxRegsOffset + Addr, 
                                                                      Mask, Shift, Value);
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpTxRegWriteMasked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtuNonIpTxRegWriteMasked(DtuNonIpPort* pNonIpPort,UInt32 Addr, UInt32 Mask,
                                                               UInt32 Shift, UInt32 Value)
{
    DT_ASSERT(pNonIpPort->m_TxRegsOffset != -1);
    return DtuRegWriteMasked(pNonIpPort->m_pDvcData, pNonIpPort->m_TxRegsOffset + Addr, 
                                                                      Mask, Shift, Value);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpRxReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuNonIpRxReset( DtuDeviceData*  pDvcData, DtuNonIpPort*  pNonIpPort, 
                                                                            Int ResetMode)
{
    DtStatus  Result = DT_STATUS_OK;
    UInt8 VendCmd;
    Int Dummy;

    // Check for valid value
    if ( ResetMode!=DTU_RST_CHANNEL && ResetMode!=DTU_RST_CLEARFIFO )
        return DT_STATUS_INVALID_PARAMETER;

    if (ResetMode == DTU_RST_CLEARFIFO)
        VendCmd = DTU_USB_RX_CLEAR_FIFO;
    else
        VendCmd = DTU_USB_RX_RESET;

    // Clear temporariy buffer
    pNonIpPort->m_TempBufWrIndex = 0;
    pNonIpPort->m_TempBufRdIndex = 0;


    // Simply send the vendor command to the device
    Result = DtUsbVendorRequest(&pDvcData->m_Device, NULL, VendCmd, 
                                                     0, 0, DT_USB_HOST_TO_DEVICE, NULL, 0,
                                                     &Dummy, MAX_USB_REQ_TIMEOUT);

    DtDbgOut(MAX, NONIP, "RX RESET, Mode: %d", ResetMode);
    return Result;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpTxReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuNonIpTxReset(DtuDeviceData*  pDvcData, DtuNonIpPort*  pNonIpPort, 
                                                                            Int ResetMode)
{
    DtStatus  Result = DT_STATUS_OK;
    UInt8 VendCmd;
    Int Dummy;

    // Check for valid value
    if ( ResetMode!=DTU_RST_CHANNEL && ResetMode!=DTU_RST_CLEARFIFO )
        return DT_STATUS_INVALID_PARAMETER;

    if (ResetMode == DTU_RST_CLEARFIFO)
        VendCmd = DTU_USB_TX_CLEAR_FIFO;
    else
        VendCmd = DTU_USB_TX_RESET;

    // Clear temporariy buffer
    pNonIpPort->m_TempBufWrIndex = 0;
    pNonIpPort->m_TempBufRdIndex = 0;

    // Simply send the vendor command to the device
    Result = DtUsbVendorRequest(&pDvcData->m_Device, NULL, VendCmd, 
                                                     0, 0, DT_USB_HOST_TO_DEVICE, NULL, 0,
                                                     &Dummy, MAX_USB_REQ_TIMEOUT);

    DtDbgOut(MAX, NONIP, "TX RESET, Mode: %d", ResetMode);

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu236DemodModeSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  Dtu236DemodModeSet(DtuDeviceData*  pDvcData, Bool DemodMode)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  RegValue=0;

    DtDbgOut(MAX, NONIP, "New DTU-236, Mode: %d", DemodMode);

    // Get current general control register value
    Status = DtuRegRead(pDvcData, DT_GEN_REG_CONTROL0, &RegValue);
    if (!DT_SUCCESS(Status))
        return Status;

    if (DemodMode)
    {
       // Enable DEMOD mode and disable ASI power
       RegValue |=  DT_GEN_CONTROL0_DEMOD_MSK;
       RegValue &= ~DT_GEN_CONTROL0_VCOEN_MSK;          
    }
    else
    {
       // Disable DEMOD mode off and enable ASI power
       RegValue &= ~DT_GEN_CONTROL0_DEMOD_MSK;
       RegValue |=  DT_GEN_CONTROL0_VCOEN_MSK;          
    }

    // Write new general control register value
    return DtuRegWrite(pDvcData, DT_GEN_REG_CONTROL0, RegValue);
}