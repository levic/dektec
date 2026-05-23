// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSfp.c *#*#*#*#*#*#*#*#*#*#*# (C) 2020 DekTec
//
// This file is not part of the driver. It can be used as template to create a new
// driver function. Replace "TEMPLATE" by the driver function's short name 
// (all uppercase) and replace "Template" by the driver function's short name in camel
// case.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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
#include "DtDfSfp.h"
#include "Messages.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
#define BC_I2CM_ROLE_NONE    NULL

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EEPROM OFFSETS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define OFFS_IDENTIFIER         0
#define OFFS_CHECKSUM_BASE      63
#define OFFS_CHECKSUM_EXT       95

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSfp implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtDfIpNrt function
#define DF_SFP_DEFAULT_PRECONDITIONS(pDf)      \
                                      DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfSfp))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_SFP_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(SFP, pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfSfp_Init(DtDf*);

static DtStatus  DtDfSfp_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfSfp_OpenChildren(DtDfSfp*);
static DtStatus  DtDfSfp_ReadSfpCapabilities(DtDfSfp* pDf);


// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSfp - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfp_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfSfp_Close(DtDf* pDfBase)
{
    //DtDfSfp* pDf = (DtDfSfp*)pDfBase;

    //DF_SFP_DEFAULT_PRECONDITIONS(pDf);
    // Let base function perform final clean-up
    DtDf_Close(pDfBase);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfp_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfSfp*  DtDfSfp_Open(DtCore* pCore, DtPt* pPt, const char* pRole, Int Instance, 
                                                                                 Int Uuid)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_SFP_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfSfp, Id, DT_FUNC_TYPE_SFP, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfSfp_Close;
    OpenParams.m_InitFunc = DtDfSfp_Init;
    OpenParams.m_OnCloseFileFunc = DtDfSfp_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfSfp*)DtDf_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfp_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSfp_GetOperationalMode(DtDfSfp* pDf, Int* pOpMode)
{
    // Sanity check
    DF_SFP_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SFP_MUST_BE_ENABLED(pDf);

    // Return cached operational mode
    *pOpMode = pDf->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfp_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSfp_SetOperationalMode(DtDfSfp* pDf, Int OpMode)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_SFP_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (OpMode!=DT_FUNC_OPMODE_IDLE && OpMode!=DT_FUNC_OPMODE_STANDBY
                                                            && OpMode!=DT_FUNC_OPMODE_RUN)
    {
        DtDbgOutDf(ERR, SFP, pDf, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    DF_SFP_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Save new operational mode
    if (DT_SUCCESS(Status))
        pDf->m_OperationalMode = OpMode;

    return Status;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfIpNrt - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfp_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSfp_Init(DtDf* pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSfp* pDf = (DtDfSfp*)pDfBase;
    
    // Sanity checks
    DF_SFP_DEFAULT_PRECONDITIONS(pDf);
    
    // Set defaults
    pDf->m_OperationalMode = DT_FUNC_OPMODE_IDLE;

    //Open children
    Status = DtDfSfp_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SFP, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfp_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtDfSfp_OnCloseFile(DtDf* pDfBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSfp* pDf = (DtDfSfp*)pDfBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_SFP_DEFAULT_PRECONDITIONS(pDf);

    // Check if the owner closed the file handle
    Status = DtDf_ExclAccessCheck((DtDf*)pDf, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutDf(AVG, SFP, pDf, "Go to IDLE");

        // Go to IDLE
        Status = DtDfSfp_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, SFP, pDf, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtDf_OnCloseFile((DtDf*)pDf, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfp_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSfp_OpenChildren(DtDfSfp*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the the IPNRT function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_I2CM, DT_BC_I2CM_NAME,
                            BC_I2CM_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcI2CM), TRUE},
    };

    DF_SFP_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    DT_ASSERT(pDf->m_pBcI2CM != NULL);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfp_ReadSfpEeprom -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSfp_ReadEeprom(DtDfSfp* pDf, UInt8* pBuffer, Int StartAddr, Int Size)
{
    DtStatus Status = DT_STATUS_OK;
    const Int SfpI2cAddrBlock0 = 0x50;
    const Int SfpI2cAddrBlock1 = 0x51;

    Int NumToReadBlock0 = 0;
    Int NumToReadBlock1 = 0;
    UInt8 Start[1] = {0};

    DT_ASSERT(pDf->m_pBcI2CM != NULL);

    if (pDf->m_pBcI2CM == NULL)
        return DT_STATUS_NOT_SUPPORTED;
    if (StartAddr<0 || StartAddr>511)
        return DT_STATUS_INVALID_PARAMETER;
    if (Size<=0 || StartAddr+Size>512)
        return DT_STATUS_INVALID_PARAMETER;

    if (StartAddr <= 255)
    {
        // Read from the first data block
        NumToReadBlock0 = 256 - StartAddr;
        if (NumToReadBlock0 > Size)
            NumToReadBlock0 = Size;
        Start[0] = (UInt8)StartAddr;
        Status = DtBcI2CM_WriteRead(pDf->m_pBcI2CM, SfpI2cAddrBlock0, 1, Start, 
                                                                NumToReadBlock0, pBuffer);
        if (!DT_SUCCESS(Status))
            return Status;
        StartAddr += NumToReadBlock0;
    }
    NumToReadBlock1 = Size - NumToReadBlock0;
    if (NumToReadBlock1 > 0)
    {
        // Read from the second data block
        Start[0] = (UInt8)(StartAddr - 256);
        Status = DtBcI2CM_WriteRead(pDf->m_pBcI2CM, SfpI2cAddrBlock1, 1, Start,
                                                NumToReadBlock1, pBuffer+NumToReadBlock0);
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfp_GetCapabilities -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSfp_GetSfpCapabilities(DtDfSfp* pDf, UInt* pCapabilities)
{
    DtStatus Status = DtDfSfp_ReadSfpCapabilities(pDf);
    *pCapabilities = pDf->m_SfpCapabilities;
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfpCalculateChecksum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt8 DtDfSfpCalculateChecksum(UInt8* pData, Int Size)
{
    Int i = 0;
    UInt8 Checksum = 0;
    for (i = 0; i < Size; i++)
        Checksum += pData[i];
    return Checksum;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSfp_ReportError -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtDfSfp_ReportError(DtDfSfp* pDf, const char* pErrorStr)
{
    DtString  Str;
    DtStringAlloc(&Str, 256);
    DtStringAppendChars(&Str, "[SN=");
    DtStringUInt64ToDtStringAppend(&Str, 10, pDf->m_pCore->m_pDevInfo->m_Serial);
    DtStringAppendChars(&Str, "] ");
    DtStringAppendChars(&Str, pErrorStr);
    DtEvtLogReport(&pDf->m_pCore->m_Device.m_EvtObject,
                                               DTPCIE_LOG_INFO_GENERIC, &Str, NULL, NULL);
    DtStringFree(&Str);

}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfUpdateSfpCapabilities -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSfp_ReadSfpCapabilities(DtDfSfp* pDf)
{
    DtStatus Status = DT_STATUS_OK;
    UInt8 Eeprom[96];
    Bool ChecksumOkBase = FALSE;
    Bool ChecksumOkExtended = FALSE;
    UInt32 RetryCount = 0;
    const Int EepromReadSize = 96;
    pDf->m_SfpCapabilities = 0;

    // Read SFP EEPROM content the base content and extended area
    // In SFF-8431 Rev 4.1 + Addendum:
    // table7: Time to initialize 2-wire interface (t_2w_start_up) is max 300ms.
    // After inserting a new SFP, we wait 300ms in the DtDfNw driver function
    do
    {
        DtDbgOutDf(AVG, SFP, pDf, "Read SFP EEPROM. RetryCount:%i", RetryCount);

        Status = DtDfSfp_ReadEeprom(pDf, Eeprom, 0, EepromReadSize);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, SFP, pDf, "Error reading EEPROM. RetryCount:%i", RetryCount);
            RetryCount++;
            DtSleep(10);
        }

    } while ((RetryCount < 10) && !DT_SUCCESS(Status));
    
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SFP, pDf, "SFP ERROR: Error reading EEPROM. Status:%xh", Status);
        DtDfSfp_ReportError(pDf, "SFP ERROR: Error reading EEPROM.");
        return Status;
    }

    // Check checksum base: 0..62
    ChecksumOkBase = DtDfSfpCalculateChecksum(Eeprom, 63) == Eeprom[OFFS_CHECKSUM_BASE];
        
    // Check checksum extended: 64..94
    ChecksumOkExtended = DtDfSfpCalculateChecksum(Eeprom+64, 31) ==
                                                                Eeprom[OFFS_CHECKSUM_EXT];

    if (!ChecksumOkBase || !ChecksumOkExtended)
    {
        if (!ChecksumOkBase && !ChecksumOkExtended)
        {
            DtDbgOutDf(ERR, SFP, pDf, "SFP ERROR: Checksum error BASE+EXTENDED area");
            DtDfSfp_ReportError(pDf, "SFP ERROR: Checksum error BASE+EXTENDED area");
        }
        else if (!ChecksumOkBase)
        {
            DtDbgOutDf(ERR, SFP, pDf, "SFP ERROR: Checksum error BASE area");
            DtDfSfp_ReportError(pDf, "SFP ERROR: Checksum error BASE area");
        }
        else
        {
            DtDbgOutDf(ERR, SFP, pDf, "SFP ERROR: Checksum error EXTENDED area");
            DtDfSfp_ReportError(pDf, "SFP ERROR: Checksum error EXTENDED area");
        }
        return DT_STATUS_FAIL;
    }
    
    if (Eeprom[OFFS_IDENTIFIER] != 0x3) // SFP/SFP+
    {
        DtDbgOutDf(ERR, SFP, pDf, "SFP ERROR:  SFP module is not of type SFP/SFP+.");
        DtDfSfp_ReportError(pDf, "SFP ERROR:  SFP module is not of type SFP/SFP+.");
        return DT_STATUS_FAIL;
    }
    // Eeprom[3:10] contain bit fields indicating supported technologies
    // (See SFF-8472, table 5-3)
    // Note for future: A passive direct attach cable is indicated by byte 8 bit 2
    // an active direct attach cable is indicated by byte 8 bit 3. 
    // Bytes 60 and 61 then give information about which standard the cable is compliant
    // with
    if ((Eeprom[8] & 0x05) != 0x00)
    {
        // byte 8, bits 2 and 3: direct attach cable
        DtDbgOutDf(ERR, SFP, pDf, "Direct attached cable detected. It's not supported.");
        DtDfSfp_ReportError(pDf, "Direct attached cable detected. It's not supported.");
        return DT_STATUS_FAIL;
    }
    if ((Eeprom[3] & 0xF0) != 0x00)
    {   // byte 4, bit 7: 10GBASE-ER, bit 6: 10GBASE-LRM, bit 5: 10GBASE-LR, bit 4: 10GBASE-SR
        pDf->m_SfpCapabilities |= SFP_CAP_10G;
    }

    // Eeprom[36] contains a byte-encoded supported technology (See SFF-8024, table 4-4)
    switch (Eeprom[36])
    {
    case 0x16: pDf->m_SfpCapabilities |= SFP_CAP_10G; break; // 10GBASE-T
    case 0x1C: pDf->m_SfpCapabilities |= SFP_CAP_10G; break; // 10GBASE-T Short Reach (30m)
    case 0x37: pDf->m_SfpCapabilities |= SFP_CAP_10G; break; // 10GBASE-BR
    case 0x02: pDf->m_SfpCapabilities |= SFP_CAP_25G; break; // 25GBASE-SR
    case 0x03: pDf->m_SfpCapabilities |= SFP_CAP_25G; break; // 25GBASE-LR
    case 0x04: pDf->m_SfpCapabilities |= SFP_CAP_25G; break; // 25GBASE-ER
    case 0x0D: pDf->m_SfpCapabilities |= SFP_CAP_25G; break; // 25GBASE-CR CA-25G-N without FEC-ER
    case 0x38: pDf->m_SfpCapabilities |= SFP_CAP_25G; break; // 25GBASE_BR
    }
    if (pDf->m_SfpCapabilities == 0)
    {
        DtDbgOutDf(ERR, SFP, pDf, "SFP Module does not support 10Gb or 25Gb Ethernet");
        DtDfSfp_ReportError(pDf, "SFP Module does not support 10Gb or 25Gb Ethernet");
        return DT_STATUS_OK;
    }
    if (Eeprom[12] == 0x00) 
    {
        // No nominal link rate given, derive from transceiver technology
        if ((pDf->m_SfpCapabilities & SFP_CAP_10G) != 0)
            pDf->m_SfpCapabilities |= SFP_CAP_10G_RATE;
        if ((pDf->m_SfpCapabilities & SFP_CAP_25G) != 0)
            pDf->m_SfpCapabilities |= SFP_CAP_25G_RATE;
        DtDbgOutDf(AVG, SFP, pDf, "No nominal link rate speficied");
    }
    else
    {
        UInt MinRateMb = 0;
        UInt MaxRateMb = 0;
        UInt NomRateMb = 0;
        if (Eeprom[12] == 0xFF)
        {
            // Nominal link rate > 25.4Gb/s in units of 250Mb/s,
            // rounded to the nearest 250Mb/s
            NomRateMb = (UInt)Eeprom[66] * (UInt)250;
            if (Eeprom[67] == 0) 
            {
                // Not set: assume no lower limit and round upper limit for inaccuracy of 
                // Eeprom[66]
                MinRateMb = 0;
                MaxRateMb = NomRateMb + (UInt)125;
            }
            else 
            {
                // Units of 1% above or below the nominal rate
                MinRateMb = (UInt)(NomRateMb - ((NomRateMb * Eeprom[67]) / 100));
                MaxRateMb = (UInt)(NomRateMb + ((NomRateMb * Eeprom[67]) / 100));
            }
        }
        else
        {
            // Nominal link rate <= 25.4Gb/s in units of 100MB/s,
            // rounded to the nearest 100MB/s
            NomRateMb = (UInt)Eeprom[12] * 100;
            if (Eeprom[66] == 0)
                // Not set, use nominal rate, rounded up for inaccuracy of Eeprom[12]
                MaxRateMb = NomRateMb + 50;
            else
                // Units of 1% above the nominal rate
                MaxRateMb = (UInt)(NomRateMb + ((NomRateMb * Eeprom[66]) / 100));
            if (Eeprom[67] == 0)
                // Not set, assume no lower limit
                MinRateMb = 0;
            else
                // Units of 1% below the nominal rate
                MinRateMb = (UInt)(NomRateMb - ((NomRateMb * Eeprom[67]) / 100));
        }
        DtDbgOutDf(AVG, SFP, pDf, "MinRate:%uMb MaxRate:%uMb NomRate:%uMb", MinRateMb, 
                                                                    MaxRateMb, NomRateMb);

        // Check if the required line rate falls within the module supported line rate
        // bounds. Required line rate is 10G or 25G multiplied by 66/64 because of 64B66B
        // line encoding
        if ((MinRateMb <= 10000 * 66 / 64) && (MaxRateMb>= 10000 * 66 / 64))
            pDf->m_SfpCapabilities |= SFP_CAP_10G_RATE;
        if ((MinRateMb <= 25000 * 66 / 64) && (MaxRateMb >= 25000 * 66 / 64))
            pDf->m_SfpCapabilities |= SFP_CAP_25G_RATE;
    }
    if (((pDf->m_SfpCapabilities&SFP_CAP_10G)!=0) && 
                                           ((pDf->m_SfpCapabilities&SFP_CAP_10G_RATE)==0))
    {
        DtDbgOutDf(ERR, SFP, pDf, 
                               "SFP Module does support 10Gb but not the 10Gb line rate");
        DtDfSfp_ReportError(pDf, 
                               "SFP Module does support 10Gb but not the 10Gb line rate");
    }
    if (((pDf->m_SfpCapabilities & SFP_CAP_25G) != 0) &&
                                       ((pDf->m_SfpCapabilities & SFP_CAP_25G_RATE) == 0))
    {
        DtDbgOutDf(ERR, SFP, pDf, 
                               "SFP Module does support 25Gb but not the 25Gb line rate");
        DtDfSfp_ReportError(pDf, 
                               "SFP Module does support 25Gb but not the 25Gb line rate");
    }
    return DT_STATUS_OK;
}


