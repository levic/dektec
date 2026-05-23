// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcCLKCNT.c *#*#*#*#*#*#*#*#*#*#* (C) 2023 DekTec
//

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
#include "DtBc.h"
#include "DtBcCLKCNT.h"
#include "DtBcCLKCNT_RegAccess.h"

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCLKCNT implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// MACRO with default precondition checks for the Bc
#define BC_CLKCNT_DEFAULT_PRECONDITIONS(pBc)      \
                                DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcCLKCNT))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_CLKCNT_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(CLKCNT, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus DtBcCLKCNT_Init(DtBc*);

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCLKCNT - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCLKCNT_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcCLKCNT_Close(DtBc*  pBc)
{
    BC_CLKCNT_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCLKCNT_GetTickCountAndFreq -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCLKCNT_GetTickCountAndFreq(
                                   DtBcCLKCNT* pBc, UInt32* pTickCount, Int* pClockFreqHz)
{
    // Sanity check
    BC_CLKCNT_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (!pTickCount || !pClockFreqHz)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CLKCNT_MUST_BE_ENABLED(pBc);

    // Read tick count from block and returned static cached clock frequency.
    *pTickCount = CLKCNT_TickCount_READ(pBc);
    *pClockFreqHz = pBc->m_ClockFreqHz;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCLKCNT_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcCLKCNT*  DtBcCLKCNT_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_CLKCNT_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcCLKCNT, Id, DT_BLOCK_TYPE_CLKCNT,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcCLKCNT_Close;
    OpenParams.m_InitFunc = DtBcCLKCNT_Init;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcCLKCNT*)DtBc_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCLKCNT - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCLKCNT_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCLKCNT_Init(DtBc* pBcBase)
{
    DtBcCLKCNT* pBc = (DtBcCLKCNT*)pBcBase;

    // Sanity check
    BC_CLKCNT_DEFAULT_PRECONDITIONS(pBc);

    // Cache clock frequency
    pBc->m_ClockFreqHz = CLKCNT_Config_READ_ClockFreq(pBc);

    return DT_STATUS_OK;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCLKCNT implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// MACRO with default precondition checks for the Bc's stub
#define CLKCNT_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                      DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCLKCNT))
// Helper macro to cast stub's DtBc member to DtBcCLKCNT
#define CLKCNT_STUB  ((DtIoStubBcCLKCNT*)pStub)
#define CLKCNT_BC  ((DtBcCLKCNT*)CLKCNT_STUB->m_pBc)

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtIoStubBcCLKCNT_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus DtIoStubBcCLKCNT_OnCmdGetTickCount(const DtIoStubBcCLKCNT*, 
                                                      DtIoctlClkCntCmdGetTickCountOutput*);

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_CLKCNT;

static const DtIoctlProperties  IOSTUB_BC_CLKCNT_IOCTLS[] =
{
    DT_IOCTL_PROPS_CLKCNT_CMD(DtIoStubBcCLKCNT_OnCmd, NULL, NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCLKCNT - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCLKCNT_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcCLKCNT_Close(DtIoStub* pStub)
{
    CLKCNT_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCLKCNT_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcCLKCNT* DtIoStubBcCLKCNT_Open(DtBc* pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_CLKCNT_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcCLKCNT, pBc, NULL,
                                 DtIoStubBcCLKCNT_Close,
                                 NULL,  // Use default IOCTL
                                 IOSTUB_BC_CLKCNT_IOCTLS);
    return (DtIoStubBcCLKCNT*)DtIoStubBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCLKCNT - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCLKCNT_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcCLKCNT_OnCmd(const DtIoStub*  pStub,
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlClkCntCmdInput*  pInData = NULL;
    DtIoctlClkCntCmdOutput*  pOutData = NULL;

    CLKCNT_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_CLKCNT_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_ClkCntCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_ClkCntCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_CLKCNT_CMD_GET_TICK_COUNT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCLKCNT_OnCmdGetTickCount(CLKCNT_STUB, 
                                                               &pOutData->m_GetTickCount);

        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCLKCNT_OnCmdGetTickCount -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcCLKCNT_OnCmdGetTickCount(const DtIoStubBcCLKCNT* pStub, 
                                             DtIoctlClkCntCmdGetTickCountOutput* pOutData)
{
    CLKCNT_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData);

    return DtBcCLKCNT_GetTickCountAndFreq(
                             CLKCNT_BC, &pOutData->m_TickCount, &pOutData->m_ClockFreqHz);
}