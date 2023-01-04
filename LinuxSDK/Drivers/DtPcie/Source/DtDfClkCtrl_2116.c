// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfTxClkCtrl_2116.c *#*#*#*#*#*#*#*#* (C) 2021 DekTec
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
#include "DtDfClkCtrl_2116.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// SPI Master role
#define SPIM_ROLE_NONE        NULL
#define LTC6952_STATUS_OK     0x15

// Remark:
//  The optimimum for ADEL3 (register 0x1B, DAC analog sysref clock delay) = 16.
//  This value is determined based on the average of the optimum from 3 devices.
const UInt8 LTC6952InitValues[] =
{
    0x00, 0xaa, 0x08, 0x03, 0xfc, 0x3f, 0x8c, 0x01, 0x01, 0x2c, 
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x80, 0x00, 0x00, 
    0x58, 0x80, 0x00, 0x00, 0xbc, 0x80, 0x00, 0x10, 0xbc, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
};

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfClkCtrl_2116 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_CLKCTRL_2116_DEFAULT_PRECONDITIONS(pDf)      \
                          DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfClkCtrl_2116))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_CLKCTRL_2116_MUST_BE_ENABLED(pDf) DF_MUST_BE_ENABLED_IMPL(CLKCTRL_2116, pDf)
static DtStatus  DtDfClkCtrl_2116_Init(DtDf*);
static DtStatus  DtDfClkCtrl_2116_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfClkCtrl_2116_OpenChildren(DtDfClkCtrl_2116*);
static DtStatus  DtDfClkCtrl_2116_ReadRegister(DtDfClkCtrl_2116*, UInt8 Address, 
                                                                           UInt8 *pValue);
static DtStatus  DtDfClkCtrl_2116_WriteRegister(DtDfClkCtrl_2116*, UInt8 Address, 
                                                                             UInt8 Value);
static DtStatus  DtDfClkCtrl_2116_GetStatus(DtDfClkCtrl_2116*);
static DtStatus  DtDfClkCtrl_2116_InitClock(DtDfClkCtrl_2116*);

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtDfClkCtrl_2116 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkCtrl_2116_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfClkCtrl_2116_Close(DtDf*  pDf)
{
    DF_CLKCTRL_2116_DEFAULT_PRECONDITIONS(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkCtrl_2116_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfClkCtrl_2116*  DtDfClkCtrl_2116_Open(DtCore*  pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // No stub available
    // Init open parameters
    DT_DF_CLKCTRL_2116_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfClkCtrl_2116, Id, DT_FUNC_TYPE_CLKCTRL_2116, 
                                                                       pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfClkCtrl_2116_Close;
    OpenParams.m_InitFunc = DtDfClkCtrl_2116_Init;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfClkCtrl_2116_OnEnablePostChildren;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfClkCtrl_2116*)DtDf_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtDfClkCtrl_2116 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkCtrl_2116_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfClkCtrl_2116_Init(DtDf*  pDfBase)
{
    DtDfClkCtrl_2116*  pDf = (DtDfClkCtrl_2116*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_CLKCTRL_2116_DEFAULT_PRECONDITIONS(pDf);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfClkCtrl_2116_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SI534X, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfClkCtrl_2116_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfClkCtrl_2116_OnEnablePostChildren(DtDf* pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfClkCtrl_2116* pDf = (DtDfClkCtrl_2116*)pDfBase;
    
    // Sanity checks
    DF_CLKCTRL_2116_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
        Status = DtDfClkCtrl_2116_InitClock(pDf);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkCtrl_2116_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfClkCtrl_2116_GetStatus(DtDfClkCtrl_2116* pDf)
{
    Int i;
    UInt8  Value;

    // Wait for expected status
    for (i = 0; i < 200; i++)
    {
        DT_RETURN_ON_ERROR(DtDfClkCtrl_2116_ReadRegister(pDf, 0x00, &Value));
        if(Value == LTC6952_STATUS_OK)
        {
            DtDbgOutDf(MAX, CLKCTRL_2116, pDf, "Dac clock generator has lock");
            return DT_STATUS_OK;
        }
        DtSleep(10);
    }
    DtDbgOutDf(ERR, CLKCTRL_2116, pDf, "DAC clock generator lock timeout");
    return DT_STATUS_TIMEOUT;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkCtrl_2116_InitClock -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfClkCtrl_2116_InitClock(DtDfClkCtrl_2116* pDf)
{
    UInt8 Buffer[100];
    
    DtDbgOutDf(MAX, CLKCTRL_2116, pDf, "Init clock generator.");


    // Reset all blocks
    DT_RETURN_ON_ERROR(DtDfClkCtrl_2116_WriteRegister(pDf, 0x02, 0x01));
    DT_RETURN_ON_ERROR(DtDfClkCtrl_2116_WriteRegister(pDf, 0x02, 0x00));

    // Wait for reset done
    DtSleep(500);

    // Writing is done in one sequence: address, data
    // Write address 0x00 (7-bits)
    Buffer[0]=0;
    memcpy(&Buffer[1], LTC6952InitValues, sizeof(LTC6952InitValues));

    // Write data
    DT_RETURN_ON_ERROR(DtBcSPIM_Write(pDf->m_pBcSpiM, sizeof(LTC6952InitValues)+1, 
                                                                                 Buffer));
 
    // Set SYSREF request (set SSRQ bit: 0x0b, b0) to sync DAC sysref and FPGA sysref
    DT_RETURN_ON_ERROR(DtDfClkCtrl_2116_WriteRegister(pDf, 0x0b, 0x01));
    DtSleep(1); // Wait at least 1 ms

    // Clear SYSREF request (set SSRQ bit: 0x0b, b0) 
    DT_RETURN_ON_ERROR(DtDfClkCtrl_2116_WriteRegister(pDf, 0x0b, 0x00));

    // Get PLL status
    return DtDfClkCtrl_2116_GetStatus(pDf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkCtrl_2116_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfClkCtrl_2116_OpenChildren(DtDfClkCtrl_2116* pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SPIM, DT_BC_SPIM_NAME, 
                               SPIM_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcSpiM), TRUE }
    };

    DF_CLKCTRL_2116_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
     if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pDf->m_pBcSpiM != NULL);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkCtrl_2116_ReadRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfClkCtrl_2116_ReadRegister(DtDfClkCtrl_2116* pDf, UInt8 Address, 
                                                                            UInt8 *pValue)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8 WrBuffer[1];
    UInt8 RdBuffer[1];

    // Address is 7 bits, b0=1
    Address = Address << 1 | 0x01;

    WrBuffer[0] = Address;
    Status = DtBcSPIM_WriteRead(pDf->m_pBcSpiM, sizeof(WrBuffer), WrBuffer, 
                                                              sizeof(RdBuffer), RdBuffer);
    *pValue = RdBuffer[0];
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkCtrl_2116_WriteRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfClkCtrl_2116_WriteRegister(DtDfClkCtrl_2116* pDf, UInt8 Address, 
                                                                              UInt8 Value)
{
    UInt8 Buffer[3];

    // Address is 7 bits, b0=write
    Address = Address<<1;

    // Write Address
    Buffer[0] = Address;
    Buffer[1] = Value;
    Buffer[2] = 0; // LTC6952 requires an extra dummy write
    return DtBcSPIM_Write(pDf->m_pBcSpiM, sizeof(Buffer), Buffer);
}
