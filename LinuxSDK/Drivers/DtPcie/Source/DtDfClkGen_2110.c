// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfClkGen_2110.c *#*#*#*#*#*#* (C) 2021-2022 DekTec
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
#include "DtDfClkGen_2110.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// No role for the block controllers
#define BC_ROLE_NONE        NULL
#define BC_ROLE_XPLL_1G     "ETH_PLL1_G"
#define BC_ROLE_XPLL_10G    "ETH_PLL10_G"



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfClkGen_2110 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_CLKGEN_2110_DEFAULT_PRECONDITIONS(pDf)      \
                          DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfClkGen_2110))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_CLKGEN_2110_MUST_BE_ENABLED(pDf) DF_MUST_BE_ENABLED_IMPL(CLKGEN_2110, pDf)
static DtStatus  DtDfClkGen_2110_Init(DtDf*);
static DtStatus  DtDfClkGen_2110_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfClkGen_2110_OpenChildren(DtDfClkGen_2110*);
static DtStatus DtDfClkGen_2110_SetRenesas5P49V6965AConfig(DtDfClkGen_2110* pDf);

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtDfClkGen_2110 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkGen_2110_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfClkGen_2110_Close(DtDf*  pDf)
{
    DF_CLKGEN_2110_DEFAULT_PRECONDITIONS(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkGen_2110_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfClkGen_2110*  DtDfClkGen_2110_Open(DtCore*  pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // No stub available
    // Init open parameters
    DT_DF_CLKGEN_2110_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfClkGen_2110, Id, 
                                             DT_FUNC_TYPE_CLKGEN_2110, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfClkGen_2110_Close;
    OpenParams.m_InitFunc = DtDfClkGen_2110_Init;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfClkGen_2110_OnEnablePostChildren;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfClkGen_2110*)DtDf_Open(&OpenParams);
}


// +=+=+=+=+=+=+=+=+=+=+=+=+ DtDfClkGen_2110 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkGen_2110_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfClkGen_2110_Init(DtDf*  pDfBase)
{
    DtDfClkGen_2110*  pDf = (DtDfClkGen_2110*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;

    DtDbgOutDf(MAX, CLKGEN, pDf, "Init");

    // Sanity checks
    DF_CLKGEN_2110_DEFAULT_PRECONDITIONS(pDf);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfClkGen_2110_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CLKGEN, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    DtDbgOutDf(MAX, CLKGEN, pDf, "Exit");
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfClkGen_2110_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfClkGen_2110_OnEnablePostChildren(DtDf* pDfBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfClkGen_2110* pDf = (DtDfClkGen_2110*)pDfBase;
    DtDbgOutDf(MAX, CLKGEN, pDf, "Init");

    // Sanity checks
    DF_CLKGEN_2110_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        Status = DtDfClkGen_2110_SetRenesas5P49V6965AConfig(pDf);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, CLKGEN, pDf, 
                              "ERROR: failed to initialise the Renesas 5P49V6965A clock");
            return Status;
        }
        if (pDf->m_pBcXPLL1G != NULL)
        {
            Status = DtBcXPLL_ResetAndCalibratePll(pDf->m_pBcXPLL1G);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOutDf(ERR, CLKGEN, pDf,
                    "ERROR: failed to reset and calibrate the EthPll1G");
                return Status;
            }
        }
        if (pDf->m_pBcXPLL10G != NULL)
        {
            Status = DtBcXPLL_ResetAndCalibratePll(pDf->m_pBcXPLL10G);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOutDf(ERR, CLKGEN, pDf,
                    "ERROR: failed to reset and calibrate the EthPll10G");
                return Status;
            }
        }
    }
    DtDbgOutDf(MAX, CLKGEN, pDf, "Exit");
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkGen_2110_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfClkGen_2110_OpenChildren(DtDfClkGen_2110*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

     // List of children supported by the the SI534X function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_I2CM, DT_BC_I2CM_NAME, 
                                 BC_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcI2Cm), TRUE},
        {DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_XPLL, DT_BC_XPLL_NAME,
                            BC_ROLE_XPLL_1G, (DtObjectBcOrDf**)(&pDf->m_pBcXPLL1G), TRUE},
        {DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_XPLL, DT_BC_XPLL_NAME,
                          BC_ROLE_XPLL_10G, (DtObjectBcOrDf**)(&pDf->m_pBcXPLL10G), TRUE},
    };

    DF_CLKGEN_2110_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
     if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pDf->m_pBcI2Cm != NULL);
    DT_ASSERT(pDf->m_pBcXPLL1G != NULL);
    DT_ASSERT(pDf->m_pBcXPLL10G != NULL);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.- DtDfClkGen_2110_SetRenesas5P49V6965AConfig -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfClkGen_2110_SetRenesas5P49V6965AConfig(DtDfClkGen_2110* pDf)
{
    DtStatus Status = DT_STATUS_OK;
    UInt SlaveAddr = 0x6a;
    // DTA-2110: Configuration data Renesas 5P49V6965A
    const UInt8  I2cConfigData[] =
    {  0x00, // Start address
       0x61, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfd, 0xc0, 0x00, 0xb6,
       0xb4, 0x92, 0xaa, 0xcc, 0x81, 0x80, 0x00, 0x03, 0x8c, 0x03, 0x40, 0x00, 0x00, 0x00,
       0x9f, 0xff, 0xf0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x03, 0x7f, 0xfc, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x40, 0x38, 0x00, 0x81, 0x01, 0x99, 0x99, 0x98,
       0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xa0, 0x1a, 0x00, 0x81, 0x00, 0x22,
       0xc1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x40, 0x02, 0xa3, 0x04,
       0x63, 0x05, 0xd3, 0x05, 0xd3, 0x05, 0xfc, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0,
       0x00, 0x60, 0xf0, 0x00, 0x4e, 0x00, 0x63, 0x00, 0x00, 0x10, 0x02, 0x08, 0x6e, 0x05,
       0x14, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    // Sanity checks
    DF_CLKGEN_2110_DEFAULT_PRECONDITIONS(pDf);

    DT_ASSERT(pDf->m_pBcI2Cm != NULL);
    
    if (pDf->m_pBcI2Cm == NULL)
        DtDbgOutDf(ERR, CLKGEN, pDf, "ERROR: I2CM ClkGen not found.");
    else {
        UInt8  ReadBuffer[1];
        UInt8  WriteBuffer[2];
        
        // Skip initializing clock chip if it's already programmed
        // We check the Primary Source and shutdown register at offset 0x10 if the output
        // is active
        WriteBuffer[0] = 0x10;
        Status = DtBcI2CM_WriteRead(pDf->m_pBcI2Cm, SlaveAddr, 1, WriteBuffer, 1,
                                                                              ReadBuffer);
        if (DT_SUCCESS(Status) && ReadBuffer[0]==I2cConfigData[WriteBuffer[0]+1])
            return DT_STATUS_OK;
        
        // Write configuration
        Status = DtBcI2CM_Write(pDf->m_pBcI2Cm, SlaveAddr, sizeof(I2cConfigData),
                                                                           I2cConfigData);
        if (!DT_SUCCESS(Status))
            DtDbgOutDf(ERR, CLKGEN, pDf, "ERROR: Error writing RAM bytes to ClkGen."
                                                                    " Status:%x", Status);
        if (DT_SUCCESS(Status))
        {
            UInt8  CalValue;
            Int  Retry = 0;
            
            // VCO Monitoring
            WriteBuffer[0] = 0x1d;
            Status = DtBcI2CM_WriteRead(pDf->m_pBcI2Cm, SlaveAddr, 1, WriteBuffer, 1, 
                                                                              ReadBuffer);
            WriteBuffer[1] = ReadBuffer[0] & (~(1 << 1));
            Status = DtBcI2CM_Write(pDf->m_pBcI2Cm, SlaveAddr, 2, WriteBuffer);

            // AFC Enable
            WriteBuffer[0] = 0x16;
            Status = DtBcI2CM_WriteRead(pDf->m_pBcI2Cm, SlaveAddr, 1, WriteBuffer, 1, 
                                                                              ReadBuffer);
            WriteBuffer[1] = ReadBuffer[0] & (~(1 << 3));
            Status = DtBcI2CM_Write(pDf->m_pBcI2Cm, SlaveAddr, 2, WriteBuffer);

            // TEST Mode
            WriteBuffer[0] = 0x11;
            Status = DtBcI2CM_WriteRead(pDf->m_pBcI2Cm, SlaveAddr, 1, WriteBuffer, 1, 
                                                                              ReadBuffer);
            WriteBuffer[1] = ReadBuffer[0] & (~(1 << 5));
            Status = DtBcI2CM_Write(pDf->m_pBcI2Cm, SlaveAddr, 2, WriteBuffer);

            while (Retry < 10)
            {
                // Enable calibration by toggling bit7
                WriteBuffer[0] = 0x1C;
                Status = DtBcI2CM_WriteRead(pDf->m_pBcI2Cm, SlaveAddr, 1, WriteBuffer, 1, 
                                                                              ReadBuffer);
                WriteBuffer[1] = ReadBuffer[0] & (~(1 << 7));
                Status = DtBcI2CM_Write(pDf->m_pBcI2Cm, SlaveAddr, 2, WriteBuffer);
                WriteBuffer[0] = 0x1C;
                Status = DtBcI2CM_WriteRead(pDf->m_pBcI2Cm, SlaveAddr, 1, WriteBuffer, 1, 
                                                                              ReadBuffer);
                WriteBuffer[1] = ReadBuffer[0] | ((1 << 7));
                Status = DtBcI2CM_Write(pDf->m_pBcI2Cm, SlaveAddr, 2, WriteBuffer);
                WriteBuffer[0] = 0x1C;
                Status = DtBcI2CM_WriteRead(pDf->m_pBcI2Cm, SlaveAddr, 1, WriteBuffer, 1, 
                                                                              ReadBuffer);
                WriteBuffer[1] = ReadBuffer[0] & (~(1 << 7));
                Status = DtBcI2CM_Write(pDf->m_pBcI2Cm, SlaveAddr, 2, WriteBuffer);
                // Wait
                DtSleep(100);
                // Read calibrated value
                WriteBuffer[0] = 0x99;
                Status = DtBcI2CM_WriteRead(pDf->m_pBcI2Cm, SlaveAddr, 1, WriteBuffer, 1, 
                                                                              ReadBuffer);
                DtDbgOutDf(MAX, CLKGEN, pDf, "Read %xh value: %xh. (Callibration value)", 
                                                           WriteBuffer[0], ReadBuffer[0]);
                // Did the calibration finish successfully
                CalValue = (ReadBuffer[0] >> 3) & 0x1f;
                if (ReadBuffer[0] != 0 && ReadBuffer[0] != 23)
                    break;
                Retry++;
            }
            if (Retry == 10)
            {
                DtDbgOutDf(ERR, CLKGEN, pDf, "Calibration did not finish successfull");
                Status = DT_STATUS_FAIL;
            }
            
            // Reset calibration and test mode.
            WriteBuffer[0] = 0x11;
            Status = DtBcI2CM_WriteRead(pDf->m_pBcI2Cm, SlaveAddr, 1, WriteBuffer, 1, 
                                                                              ReadBuffer);
            WriteBuffer[1] = (1 << 5) | CalValue;
            Status = DtBcI2CM_Write(pDf->m_pBcI2Cm, SlaveAddr, 2, WriteBuffer);
            DtSleep(1);
        }
    }
    return Status;
}
