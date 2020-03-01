//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQPWR_2132.c *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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
#include "DtBcIQPWR_2132.h"
#include "DtBcIQPWR_2132_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQPWR_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQPWR_2132_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQPWR_2132))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQPWR_2132_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQPWR_2132, pBc)

// Helper macro to cast a DtBc* to a DtBcIQPWR_2132*
#define BC_IQPWR_2132         ((DtBcIQPWR_2132*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQPWR_2132_Init(DtBc*);

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQPWR_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQPWR_2132_Close(DtBc*  pBc)
{
    BC_IQPWR_2132_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcIQPWR_2132*  DtBcIQPWR_2132_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_IQPWR_2132_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQPWR_2132, Id, DT_BLOCK_TYPE_IQPWR_2132, 
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQPWR_2132_Close;
    OpenParams.m_InitFunc = DtBcIQPWR_2132_Init;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQPWR_2132*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_2132_GetEstPower -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQPWR_2132_GetEstPower(DtBcIQPWR_2132* pBc, UInt32* pPower)
{
    // Sanity check
    BC_IQPWR_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pPower == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Read the power
    *pPower = IQPWR_Power_READ(pBc);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_2132_GetEstWindow -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQPWR_2132_GetEstWindow(DtBcIQPWR_2132* pBc, Int* pWindow)
{
    // Sanity check
    BC_IQPWR_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pWindow == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return cached value
    *pWindow = pBc->m_EstWindow;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_2132_SetEstWindow -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQPWR_2132_SetEstWindow(DtBcIQPWR_2132* pBc, Int Window)
{
    // Sanity check
    BC_IQPWR_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (Window<0 || Window>0x1F)
        return DT_STATUS_INVALID_PARAMETER;

    IQPWR_Window_WRITE(pBc, (UInt32)Window);

    // Update cache
    pBc->m_EstWindow = Window;

    return DT_STATUS_OK;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQPWR_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_2132_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQPWR_2132_Init(DtBc*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    BC_IQPWR_2132_DEFAULT_PRECONDITIONS(pBc);

    // Initialize the estimation window
    BC_IQPWR_2132->m_EstWindow = 8;
    IQPWR_Window_WRITE(BC_IQPWR_2132, (UInt32)BC_IQPWR_2132->m_EstWindow);

    return Status;
}



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQPWR_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcIQPWR_2132
#define IQPWR_2132_STUB   ((DtIoStubBcIQPWR_2132*)pStub)
#define IQPWR_2132_BC  ((DtBcIQPWR_2132*)IQPWR_2132_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQPWR_2132_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus DtIoStubBcIQPWR_2132_OnCmdGetEstPower(const DtIoStubBcIQPWR_2132* , 
                                                  DtIoctlIqPwrCmd_2132GetEstPowerOutput*);
static DtStatus DtIoStubBcIQPWR_2132_OnCmdGetEstWindow(const DtIoStubBcIQPWR_2132*, 
                                                 DtIoctlIqPwrCmd_2132GetEstWindowOutput*);
static DtStatus DtIoStubBcIQPWR_2132_OnCmdSetEstWindow(const DtIoStubBcIQPWR_2132*,
                                            const DtIoctlIqPwrCmd_2132SetEstWindowInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQPWR_2132;

static const DtIoctlProperties  IOSTUB_BC_IQPWR_2132_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQPWR_CMD_2132(
        DtIoStubBcIQPWR_2132_OnCmd,
        NULL, NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQPWR_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcIQPWR_2132_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQPWR_2132));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcIQPWR_2132*  DtIoStubBcIQPWR_2132_Open(DtBc*  pBc)
{
    DtIoStubBcIQPWR_2132*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQPWR_2132));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQPWR_2132, pBc, NULL, 
                                                           DtIoStubBcIQPWR_2132_Close,
                                                           NULL,  // Use default IOCTL
                                                           IOSTUB_BC_IQPWR_2132_IOCTLS);
    pStub = (DtIoStubBcIQPWR_2132*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQPWR_2132- Private functions +=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_2132_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQPWR_2132_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqPwrCmdInput_2132*  pInData = NULL;
    DtIoctlIqPwrCmdOutput_2132*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQPWR_2132));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQPWR_CMD_2132);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQPWR_2132, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqPwrCmd_2132;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqPwrCmd_2132;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQPWR_CMD_2132_GET_EST_POWER:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQPWR_2132_OnCmdGetEstPower(IQPWR_2132_STUB,  
                                                                &pOutData->m_GetEstPower);
        break;
    case DT_IQPWR_CMD_2132_GET_EST_WINDOW:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQPWR_2132_OnCmdGetEstWindow(IQPWR_2132_STUB,  
                                                               &pOutData->m_GetEstWindow);
        break;
    case DT_IQPWR_CMD_2132_SET_EST_WINDOW:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQPWR_2132_OnCmdSetEstWindow(IQPWR_2132_STUB, 
                                                                &pInData->m_SetEstWindow);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_2132_OnCmdGetEstPower -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQPWR_2132_OnCmdGetEstPower(
    const DtIoStubBcIQPWR_2132* pStub,
    DtIoctlIqPwrCmd_2132GetEstPowerOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQPWR_2132));
    DT_ASSERT(pOutData != NULL);

    return DtBcIQPWR_2132_GetEstPower(IQPWR_2132_BC, &pOutData->m_EstPower);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_2132_OnCmdGetEstWindow -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQPWR_2132_OnCmdGetEstWindow(
    const DtIoStubBcIQPWR_2132* pStub,
    DtIoctlIqPwrCmd_2132GetEstWindowOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQPWR_2132));
    DT_ASSERT(pOutData != NULL);

    return DtBcIQPWR_2132_GetEstWindow(IQPWR_2132_BC, &pOutData->m_EstWindow);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_2132_OnCmdSetEstWindow -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQPWR_2132_OnCmdSetEstWindow(
    const DtIoStubBcIQPWR_2132* pStub,
    const DtIoctlIqPwrCmd_2132SetEstWindowInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQPWR_2132));
    DT_ASSERT(pInData != NULL);

    return DtBcIQPWR_2132_SetEstWindow(IQPWR_2132_BC, pInData->m_EstWindow);
}