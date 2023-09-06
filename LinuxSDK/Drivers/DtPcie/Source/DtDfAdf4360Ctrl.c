// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfAdf4360Ctrl.c *#*#*#*#*#*#*#*#*# (C) 2023 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2023 DekTec Digital Video B.V.
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
#include "DtDfAdf4360Ctrl.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// SPI Master role
#define SPIM_ROLE_NONE        NULL


#define ADF4360_CTRL_LATCH       0
#define ADF4360_R_CNTR_LATCH     1
#define ADF4360_N_CNTR_LATCH     2

// Latch values for DTA-2111B clock architecture
const UInt32 DtDfAdf4360Ctr_DTA_2111B_ARCH[] =
{
    0x0FF928,   // Control Latch
    0x340041,   // R-Counter Latch
    0x012C02    // N-Counter Latch
};


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfAdf4360Ctrl implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_ADF4360CTRL_DEFAULT_PRECONDITIONS(pDf)                                        \
                          DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfAdf4360Ctrl))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_ADF4360CTRL_MUST_BE_ENABLED(pDf) DF_MUST_BE_ENABLED_IMPL(ADF4360CTRL, pDf)
static DtStatus  DtDfAdf4360Ctrl_Init(DtDf*);
static DtStatus  DtDfAdf4360Ctrl_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfAdf4360Ctrl_OpenChildren(DtDfAdf4360Ctrl*);
static DtStatus  DtDfAdf4360Ctrl_InitClock(DtDfAdf4360Ctrl* pDf);
static DtStatus  DtDfAdf4360Ctrl_WriteLatch(DtDfAdf4360Ctrl*, UInt32 LatchValue);

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtDfAdf4360Ctrl - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAdf4360Ctrl_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfAdf4360Ctrl_Close(DtDf*  pDf)
{
    DF_ADF4360CTRL_DEFAULT_PRECONDITIONS(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAdf4360Ctrl_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfAdf4360Ctrl*  DtDfAdf4360Ctrl_Open(DtCore*  pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // No stub available
    // Init open parameters
    DT_DF_ADF4360CTRL_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfAdf4360Ctrl, Id, DT_FUNC_TYPE_ADF4360CTRL, 
                                                                       pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfAdf4360Ctrl_Close;
    OpenParams.m_InitFunc = DtDfAdf4360Ctrl_Init;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfAdf4360Ctrl_OnEnablePostChildren;

    // Use base function to allocate and perform standard initialization of function data
    return (DtDfAdf4360Ctrl*)DtDf_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtDfAdf4360Ctrl - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAdf4360Ctrl_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfAdf4360Ctrl_Init(DtDf*  pDfBase)
{
    DtDfAdf4360Ctrl*  pDf = (DtDfAdf4360Ctrl*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_ADF4360CTRL_DEFAULT_PRECONDITIONS(pDf);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfAdf4360Ctrl_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SI534X, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    // Currently only one configuration is supported
    pDf->m_ConfigData = DtDfAdf4360Ctr_DTA_2111B_ARCH;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfAdf4360Ctrl_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfAdf4360Ctrl_OnEnablePostChildren(DtDf* pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfAdf4360Ctrl* pDf = (DtDfAdf4360Ctrl*)pDfBase;
    
    // Sanity checks
    DF_ADF4360CTRL_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
        Status = DtDfAdf4360Ctrl_InitClock(pDf);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAdf4360Ctrl_InitClock -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAdf4360Ctrl_InitClock(DtDfAdf4360Ctrl* pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDbgOutDf(MAX, ADF4360CTRL, pDf, "Init clock generator.");


    // Program ADF4360
    Status = DtDfAdf4360Ctrl_WriteLatch(pDf, pDf->m_ConfigData[ADF4360_R_CNTR_LATCH]);
    if (Status == DT_STATUS_OK)
        Status = DtDfAdf4360Ctrl_WriteLatch(pDf, pDf->m_ConfigData[ADF4360_CTRL_LATCH]);
    // Interval between Control Latch and N Counter Latch write
    if (Status == DT_STATUS_OK)
        DtSleep(50);
    if (Status == DT_STATUS_OK)
        Status = DtDfAdf4360Ctrl_WriteLatch(pDf, pDf->m_ConfigData[ADF4360_N_CNTR_LATCH]);
    if (Status != DT_STATUS_OK)
        DtDbgOutDf(ERR, ADF4360CTRL, pDf, "ERROR: Failed to write latch");
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAdf4360Ctrl_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfAdf4360Ctrl_OpenChildren(DtDfAdf4360Ctrl* pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SPIM, DT_BC_SPIM_NAME, 
                               SPIM_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcSpiM), TRUE }
    };

    DF_ADF4360CTRL_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
     if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pDf->m_pBcSpiM != NULL);
    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAdf4360Ctrl_WriteLatch -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfAdf4360Ctrl_WriteLatch(DtDfAdf4360Ctrl* pDf, UInt32 LatchValue)
{
    UInt8 Buffer[3];

    // Write latch value
    Buffer[0] = (UInt8)(LatchValue & 0xFF);
    Buffer[1] = (UInt8)((LatchValue>>8) & 0xFF);
    Buffer[2] = (UInt8)((LatchValue>>16) & 0xFF);
    return DtBcSPIM_Write(pDf->m_pBcSpiM, sizeof(Buffer), Buffer);
}
