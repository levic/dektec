//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQFIR_2132.c *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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
#include "DtBcIQFIR_2132.h"
#include "DtBcIQFIR_2132_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQFIR_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQFIR_2132_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQFIR_2132))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQFIR_2132_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQFIR_2132, pBc)

// Helper macro to cast a DtBc* to a DtBcIQFIR_2132*
#define BC_IQFIR_2132         ((DtBcIQFIR_2132*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQFIR_2132_Init(DtBc*);
static DtStatus  DtBcIQFIR_2132_SetControl(DtBcIQFIR_2132* pBc, Bool ClearFifo, 
                                                               Bool Start, Int Direction);

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQFIR_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQFIR_2132_Close(DtBc*  pBc)
{
    BC_IQFIR_2132_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcIQFIR_2132*  DtBcIQFIR_2132_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_IQFIR_2132_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQFIR_2132, Id, DT_BLOCK_TYPE_IQFIR_2132, 
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQFIR_2132_Close;
    OpenParams.m_InitFunc = DtBcIQFIR_2132_Init;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQFIR_2132*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_2132_GetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQFIR_2132_GetConfig(DtBcIQFIR_2132* pBc, Int* pNumCoefs, Int* pCoefSize)
{
    // Sanity check
    BC_IQFIR_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pNumCoefs==NULL || pCoefSize==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return cached configuration
    *pNumCoefs = pBc->m_NumberCoefficients;
    *pCoefSize = pBc->m_CoefficientSize;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_2132_GetCoefficients -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQFIR_2132_GetCoefficients(DtBcIQFIR_2132* pBc, Int* pCoeffs, Int NumToGet)
{
    DtStatus  Status = DT_STATUS_OK;
    Int i, NumShift;

    // Sanity check
    BC_IQFIR_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pCoeffs==NULL || NumToGet!=pBc->m_NumberCoefficients)
        return DT_STATUS_INVALID_PARAMETER;

    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Clear FIFO
    Status = DtBcIQFIR_2132_SetControl(pBc, TRUE, FALSE, IQFIR_CMD_READ);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, IQFIR_2132, pBc, "Clear FIFO failed");
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return Status;
    }

    // Start transfer of filter data
    Status = DtBcIQFIR_2132_SetControl(pBc, FALSE, TRUE, IQFIR_CMD_READ);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, IQFIR_2132, pBc, "Start transfer failed");
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return Status;
    }

    // Read coefficients
    NumShift = 32 - pBc->m_CoefficientSize;
    for (i=0; i<NumToGet; i++)
    {
        // Read with sign extension
        UInt32  Coef = IQFIR_FilterCoefData_READ(pBc);
        pCoeffs[i] = (Int)((Coef << NumShift) >> NumShift);
    }

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_2132_SetCoefficients -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQFIR_2132_SetCoefficients(DtBcIQFIR_2132* pBc, const Int* pCoeffs,
                                                                             Int NumToSet)
{
    UInt32  Mask;
    Int  i;
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    BC_IQFIR_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pCoeffs==NULL || NumToSet!=pBc->m_NumberCoefficients)
        return DT_STATUS_INVALID_PARAMETER;

    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Clear FIFO
    Status = DtBcIQFIR_2132_SetControl(pBc, TRUE, FALSE, IQFIR_CMD_WRITE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, IQFIR_2132, pBc, "Clear FIFO failed");
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return Status;
    }

    // Write coefficients
    Mask = (1 << pBc->m_CoefficientSize) - 1;
    for (i=0; i<NumToSet; i++)
    {
        IQFIR_FilterCoefData_WRITE(pBc, pCoeffs[i] & Mask);
    }

    // Start transfer of filter data
    Status = DtBcIQFIR_2132_SetControl(pBc, FALSE, TRUE, IQFIR_CMD_WRITE);
    if (!DT_SUCCESS(Status))
        DtDbgOutBc(ERR, IQFIR_2132, pBc, "Start transfer failed");

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return Status;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQFIR_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_2132_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQFIR_2132_Init(DtBc*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    BC_IQFIR_2132_DEFAULT_PRECONDITIONS(pBc);

    // Read config
    BC_IQFIR_2132->m_NumberCoefficients = 
                               IQFIR_Configuration_READ_NumberCoefficients(BC_IQFIR_2132);
    BC_IQFIR_2132->m_CoefficientSize = 
                                  IQFIR_Configuration_READ_CoefficientSize(BC_IQFIR_2132);

    // Init mutex
    DtFastMutexInit(&BC_IQFIR_2132->m_AccessMutex);

    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_2132_WaitForReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQFIR_2132_SetControl(DtBcIQFIR_2132* pBc, Bool ClearFifo,  Bool Start, 
                                                                            Int Direction)
{
    UInt32 FldReady, RegControl;
    Int i;

    // Parameters should be valid
    DT_ASSERT(ClearFifo || Start && !(ClearFifo && Start));
    DT_ASSERT(Direction==IQFIR_CMD_READ || Direction==IQFIR_CMD_WRITE);
    
    // Filter should be ready for transfer
    FldReady = IQFIR_FilterCoefControl_READ_Ready(pBc);
    if (FldReady != 1)
        return DT_STATUS_BUSY;

    // Set control
    RegControl = IQFIR_FilterCoefControl_SET_ClearFifo(0, ClearFifo ? 1 : 0);
    RegControl = IQFIR_FilterCoefControl_SET_DataDirection(RegControl, Direction);
    RegControl = IQFIR_FilterCoefControl_SET_Start(RegControl, Start ? 1 : 0);
    IQFIR_FilterCoefControl_WRITE(pBc, RegControl);

    // Wait for ready
    FldReady = IQFIR_FilterCoefControl_READ_Ready(pBc);
    for (i=0; i<10 && FldReady!=1; i++)
    {
        DtSleep(1);
        FldReady = IQFIR_FilterCoefControl_READ_Ready(pBc);
    }

    // Not ready?
    if (FldReady != 1)
        return DT_STATUS_TIMEOUT;
    else
        return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQFIR_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcIQFIR_2132
#define IQFIR_2132_STUB   ((DtIoStubBcIQFIR_2132*)pStub)
#define IQFIR_2132_BC  ((DtBcIQFIR_2132*)IQFIR_2132_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQFIR_2132_AppendDynamicSize(const DtIoStub*, 
                                                                      DtIoStubIoParams *);
static DtStatus  DtIoStubBcIQFIR_2132_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus DtIoStubBcIQFIR_2132_OnCmdGetConfig(const DtIoStubBcIQFIR_2132* , 
                                                    DtIoctlIqFirCmd_2132GetConfigOutput*);
static DtStatus DtIoStubBcIQFIR_2132_OnCmdGetCoefficients(const DtIoStubBcIQFIR_2132*, 
                                                 const DtIoctlIqFirCmd_2132GetCoefsInput*,
                                                 DtIoctlIqFirCmd_2132GetCoefsOutput*);
static DtStatus DtIoStubBcIQFIR_2132_OnCmdSetCoefficients(const DtIoStubBcIQFIR_2132*,
                                                const DtIoctlIqFirCmd_2132SetCoefsInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQFIR_2132;

static const DtIoctlProperties  IOSTUB_BC_IQFIR_2132_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQFIR_CMD_2132(
        DtIoStubBcIQFIR_2132_OnCmd,
        DtIoStubBcIQFIR_2132_AppendDynamicSize,
        NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQFIR_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcIQFIR_2132_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR_2132));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcIQFIR_2132*  DtIoStubBcIQFIR_2132_Open(DtBc*  pBc)
{
    DtIoStubBcIQFIR_2132*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQFIR_2132));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQFIR_2132, pBc, NULL, 
                                                           DtIoStubBcIQFIR_2132_Close,
                                                           NULL,  // Use default IOCTL
                                                           IOSTUB_BC_IQFIR_2132_IOCTLS);
    pStub = (DtIoStubBcIQFIR_2132*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQFIR_2132- Private functions +=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_2132_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQFIR_2132_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqFirCmdInput_2132*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR_2132));
    DT_ASSERT(pIoParams != NULL);                 
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_IQFIR_CMD_2132);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqFirCmd_2132;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_IQFIR_CMD_2132_GET_COEFS:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlIqFirCmd_2132GetCoefsInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlIqFirCmd_2132GetCoefsOutput));
        // Add dynamic size (i.e. #bytes to get)
        pIoParams->m_OutReqSize += (pInData->m_GetCoefs.m_NumCoefsToGet * sizeof(Int));
        break;

    case DT_IQFIR_CMD_2132_SET_COEFS:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlIqFirCmd_2132SetCoefsInput));
        // Add dynamic size (i.e. #bytes to set)
        pIoParams->m_InReqSize += (pInData->m_SetCoefs.m_NumCoefsToSet * sizeof(Int));
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_2132_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQFIR_2132_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqFirCmdInput_2132*  pInData = NULL;
    DtIoctlIqFirCmdOutput_2132*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR_2132));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_IQFIR_CMD_2132);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQFIR_2132, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqFirCmd_2132;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqFirCmd_2132;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQFIR_CMD_2132_GET_CONFIG:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQFIR_2132_OnCmdGetConfig(IQFIR_2132_STUB,  
                                                                  &pOutData->m_GetConfig);
        break;
    case DT_IQFIR_CMD_2132_GET_COEFS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQFIR_2132_OnCmdGetCoefficients(IQFIR_2132_STUB,  
                                            &pInData->m_GetCoefs,  &pOutData->m_GetCoefs);
        break;
    case DT_IQFIR_CMD_2132_SET_COEFS:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQFIR_2132_OnCmdSetCoefficients(IQFIR_2132_STUB, 
                                                                    &pInData->m_SetCoefs);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_2132_OnCmdGetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQFIR_2132_OnCmdGetConfig(
    const DtIoStubBcIQFIR_2132* pStub,
    DtIoctlIqFirCmd_2132GetConfigOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR_2132));
    DT_ASSERT(pOutData != NULL);

    return DtBcIQFIR_2132_GetConfig(IQFIR_2132_BC, &pOutData->m_NumberCoefs, 
                                                                   &pOutData->m_CoefSize);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_2132_OnCmdGetCoefficients -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQFIR_2132_OnCmdGetCoefficients(
    const DtIoStubBcIQFIR_2132* pStub,
    const DtIoctlIqFirCmd_2132GetCoefsInput* pInData,
    DtIoctlIqFirCmd_2132GetCoefsOutput* pOutData)
{
    DtStatus Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR_2132));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    Status = DtBcIQFIR_2132_GetCoefficients(IQFIR_2132_BC, pOutData->m_pCoefs, 
                                                                pInData->m_NumCoefsToGet);
    if (DT_SUCCESS(Status))
        pOutData->m_NumCoefs = pInData->m_NumCoefsToGet;
    else
        pOutData->m_NumCoefs = 0;

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_2132_OnCmdSetCoefficients -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQFIR_2132_OnCmdSetCoefficients(
    const DtIoStubBcIQFIR_2132* pStub,
    const DtIoctlIqFirCmd_2132SetCoefsInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR_2132));
    DT_ASSERT(pInData != NULL);

    return DtBcIQFIR_2132_SetCoefficients(IQFIR_2132_BC, pInData->m_pCoefs, 
                                                                pInData->m_NumCoefsToSet);
}
