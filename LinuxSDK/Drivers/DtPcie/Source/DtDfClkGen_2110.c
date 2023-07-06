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
#include "DtDfVpd.h"
#include "Messages.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Roles for the block controllers
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
    DtDfClkGen_2110*  pDf = (DtDfClkGen_2110*)pDfBase;
    DtDbgOutDf(MAX, CLKGEN, pDf, "Init");

    // Sanity checks
    DF_CLKGEN_2110_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        UInt8  VpdBuf[10];
        Int  NumRead = 0;

        // For the PLL calibration process to succeed we need a clock from the clock chip.
        // To check if the clock chip is programmed, we must use the I2C interface to 
        // the clock chip, but this interface is only available from the DTAPI at this
        // moment.
        // 
        // The clock chip is programmed in the factory with the DtBoardTest
        // application just after the VPD is programmed. So a good workaround for checking
        // if the clock chip is programmed is to check if the VPD is programmed.
        // We will check the serialnumber of the card and the 'ProductionDate' in the 
        // read-only section of the VPD.
        
        if (pDf->m_pCore->m_pDevInfo->m_Serial==-1 ||
                         DtDfVpd_ItemRead(pDf->m_pCore->m_pDfVpd, DT_VPD_SECT_RO, 2, "PD", 
                                          0, 10, VpdBuf, &NumRead) == DT_STATUS_NOT_FOUND)
        {
            DtString  Str;
            DtDbgOutDf(ERR, CLKGEN, pDf, "Skipped EthPll1G and EthPll10G PLL calibration."
                                                     " Clock chip (VPD) not programmed.");

            DtStringAlloc(&Str, 256);

            DtStringAppendChars(&Str, "[SN=");
            DtStringUInt64ToDtStringAppend(&Str, 10, pDf->m_pCore->m_pDevInfo->m_Serial);
            DtStringAppendChars(&Str, "] ");
            DtStringAppendChars(&Str, "Skipped EthPll1G and EthPll10G PLL calibration."
                                                     " Clock chip (VPD) not programmed.");
            DtEvtLogReport(&pDf->m_pCore->m_Device.m_EvtObject,
                                              DTPCIE_LOG_ERROR_GENERIC, &Str, NULL, NULL);
            DtStringFree(&Str);
            // We can't fail here because otherwise driver is not started.
            return DT_STATUS_OK;
        }
        // 
        // Wait shortly, before resetting/calibrating the PLLs, to allow ALTERA 
        // power-up calibration to complete. Starting a user calibration, while 
        // power-up calibration is busy results in calibration to timeout (fail).
        DtSleep(500);
        if (pDf->m_pBcXPLL1G != NULL)
        {
            Status = DtBcXPLL_ResetAndCalibratePll(pDf->m_pBcXPLL1G);
            if (!DT_SUCCESS(Status))
            {
                DtString  Str;
                DtDbgOutDf(ERR, CLKGEN, pDf,
                                    "ERROR: failed to reset and calibrate the EthPll1G");
                
                DtStringAlloc(&Str, 256);

                DtStringAppendChars(&Str, "[SN=");
                DtStringUInt64ToDtStringAppend(&Str, 10,
                                                      pDf->m_pCore->m_pDevInfo->m_Serial);
                DtStringAppendChars(&Str, "] ");
                DtStringAppendChars(&Str, "Failed to reset and calibrate the EthPll1G");
                DtEvtLogReport(&pDf->m_pCore->m_Device.m_EvtObject,
                                              DTPCIE_LOG_ERROR_GENERIC, &Str, NULL, NULL);
                DtStringFree(&Str);
                // Probably no clock programmed
                // We can't fail here because otherwise driver is not started.
                Status = DT_STATUS_OK;
            }
        }
        if (pDf->m_pBcXPLL10G != NULL)
        {
            Status = DtBcXPLL_ResetAndCalibratePll(pDf->m_pBcXPLL10G);
            if (!DT_SUCCESS(Status))
            {
                DtString  Str;
                DtDbgOutDf(ERR, CLKGEN, pDf,
                                    "ERROR: failed to reset and calibrate the EthPll10G");
                // Probably no clock available. We check the clock state later
                DtStringAlloc(&Str, 256);

                DtStringAppendChars(&Str, "[SN=");
                DtStringUInt64ToDtStringAppend(&Str, 10,
                                                      pDf->m_pCore->m_pDevInfo->m_Serial);
                DtStringAppendChars(&Str, "] ");
                DtStringAppendChars(&Str, "Failed to reset and calibrate the EthPll10G");
                DtEvtLogReport(&pDf->m_pCore->m_Device.m_EvtObject,
                                              DTPCIE_LOG_ERROR_GENERIC, &Str, NULL, NULL);
                DtStringFree(&Str);
                // Probably no clock programmed
                // We can't fail here because otherwise driver is not started
                Status = DT_STATUS_OK;
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
    DT_ASSERT(pDf->m_pBcXPLL1G != NULL);
    DT_ASSERT(pDf->m_pBcXPLL10G != NULL);
    return DT_STATUS_OK;
}
