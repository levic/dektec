//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcDATASTMUX_2132.c *#*#*#*#*#*#*#*#* (C) 2018 DekTec
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
#include "DtBcDATASTMUX_2132.h"
#include "DtBcDATASTMUX_2132_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDATASTMUX_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_DATASTMUX_2132_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcDATASTMUX_2132))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_DATASTMUX_2132_MUST_BE_ENABLED(pBc)  BC_MUST_BE_ENABLED_IMPL(DATASTMUX_2132,  \
                                                                                      pBc)

// Helper macro to cast a DtBc* to a DtBcDATASTMUX_2132*
#define BC_DATASTMUX_2132         ((DtBcDATASTMUX_2132*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcDATASTMUX_2132_Init(DtBc*);

//=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDATASTMUX_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATASTMUX_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcDATASTMUX_2132_Close(DtBc*  pBc)
{
    BC_DATASTMUX_2132_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATASTMUX_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcDATASTMUX_2132*  DtBcDATASTMUX_2132_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_DATASTMUX_2132_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcDATASTMUX_2132, Id,
                           DT_BLOCK_TYPE_DATASTMUX_2132, Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcDATASTMUX_2132_Close;
    OpenParams.m_InitFunc = DtBcDATASTMUX_2132_Init;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcDATASTMUX_2132*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATASTMUX_2132_GetSelection -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDATASTMUX_2132_GetSelection(DtBcDATASTMUX_2132* pBc, Int* pSelect)
{
    // Sanity check
    BC_DATASTMUX_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pSelect == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return cached setting
    *pSelect = pBc->m_Selection;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATASTMUX_2132_SetSelection -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDATASTMUX_2132_SetSelection(DtBcDATASTMUX_2132* pBc, Int Select)
{
    UInt32  RegSelect;
    // Sanity check
    BC_DATASTMUX_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (Select!=DT_DATASTMUX_2132_SELECT_IQ && Select!=DT_DATASTMUX_2132_SELECT_L3)
        return DT_STATUS_INVALID_PARAMETER;

    // Set register
    switch (Select)
    {
    case DT_DATASTMUX_2132_SELECT_IQ: RegSelect = DATASTMUX_CMD_IQSamples; break;
    case DT_DATASTMUX_2132_SELECT_L3: RegSelect = DATASTMUX_CMD_L3Frames; break;
    }
    DATASTMUX_SelectInput_WRITE(pBc, RegSelect);

    // Update cache
    pBc->m_Selection = Select;

    return DT_STATUS_OK;
}
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDATASTMUX_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATASTMUX_2132_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcDATASTMUX_2132_Init(DtBc*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    BC_DATASTMUX_2132_DEFAULT_PRECONDITIONS(pBc);

    // Initialize the selection
    BC_DATASTMUX_2132->m_Selection = DT_DATASTMUX_2132_SELECT_IQ;
    DATASTMUX_SelectInput_WRITE(BC_DATASTMUX_2132, DATASTMUX_CMD_IQSamples);

    return Status;
}



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDATASTMUX_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcDATASTMUX_2132
#define DATASTMUX_2132_STUB   ((DtIoStubBcDATASTMUX_2132*)pStub)
#define DATASTMUX_2132_BC  ((DtBcDATASTMUX_2132*)DATASTMUX_2132_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcDATASTMUX_2132_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus DtIoStubBcDATASTMUX_2132_OnCmdGetSelection(
                                             const DtIoStubBcDATASTMUX_2132*, 
                                             DtIoctlDataStMuxCmd_2132GetSelectionOutput*);
static DtStatus DtIoStubBcDATASTMUX_2132_OnCmdSetSelection(
                                        const DtIoStubBcDATASTMUX_2132*,
                                        const DtIoctlDataStMuxCmd_2132SetSelectionInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_DATASTMUX_2132;

static const DtIoctlProperties  IOSTUB_BC_DATASTMUX_2132_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_DATASTMUX_CMD_2132(
        DtIoStubBcDATASTMUX_2132_OnCmd,
        NULL, NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDATASTMUX_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATASTMUX_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcDATASTMUX_2132_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDATASTMUX_2132));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATASTMUX_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcDATASTMUX_2132*  DtIoStubBcDATASTMUX_2132_Open(DtBc*  pBc)
{
    DtIoStubBcDATASTMUX_2132*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcDATASTMUX_2132));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcDATASTMUX_2132, pBc, NULL, 
                                                         DtIoStubBcDATASTMUX_2132_Close,
                                                         NULL,  // Use default IOCTL
                                                         IOSTUB_BC_DATASTMUX_2132_IOCTLS);
    pStub = (DtIoStubBcDATASTMUX_2132*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDATASTMUX_2132- Private functions +=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATASTMUX_2132_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcDATASTMUX_2132_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlDataStMuxCmdInput_2132*  pInData = NULL;
    DtIoctlDataStMuxCmdOutput_2132*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDATASTMUX_2132));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_DATASTMUX_CMD_2132);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, DATASTMUX_2132, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_DataStMuxCmd_2132;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_DataStMuxCmd_2132;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_DATASTMUX_CMD_2132_GET_SELECTION:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcDATASTMUX_2132_OnCmdGetSelection(DATASTMUX_2132_STUB,  
                                                               &pOutData->m_GetSelection);
        break;
    case DT_DATASTMUX_CMD_2132_SET_SELECTION:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcDATASTMUX_2132_OnCmdSetSelection(DATASTMUX_2132_STUB, 
                                                                &pInData->m_SetSelection);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATASTMUX_2132_OnCmdGetSelection -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcDATASTMUX_2132_OnCmdGetSelection(
    const DtIoStubBcDATASTMUX_2132* pStub,
    DtIoctlDataStMuxCmd_2132GetSelectionOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDATASTMUX_2132));
    DT_ASSERT(pOutData != NULL);

    return DtBcDATASTMUX_2132_GetSelection(DATASTMUX_2132_BC, &pOutData->m_Selection);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATASTMUX_2132_OnCmdSetSelection -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcDATASTMUX_2132_OnCmdSetSelection(
    const DtIoStubBcDATASTMUX_2132* pStub,
    const DtIoctlDataStMuxCmd_2132SetSelectionInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDATASTMUX_2132));
    DT_ASSERT(pInData != NULL);

    return DtBcDATASTMUX_2132_SetSelection(DATASTMUX_2132_BC, pInData->m_Selection);
}