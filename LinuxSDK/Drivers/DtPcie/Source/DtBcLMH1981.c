// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcLMH1981.c *#*#*#*#*#*#*#*#*#*# (C) 2019 DekTec
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
#include "DtBc.h"
#include "DtBcLMH1981.h"
#include "DtBcLMH1981_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLMH1981 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_LMH1981_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcLMH1981))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_LMH1981_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(LMH1981, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcLMH1981_Init(DtBc*);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLMH1981 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcLMH1981_Close(DtBc*  pBc)
{
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcLMH1981*  DtBcLMH1981_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_LMH1981_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcLMH1981, Id, DT_BLOCK_TYPE_LMH1981,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcLMH1981_Close;
    OpenParams.m_InitFunc = DtBcLMH1981_Init;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcLMH1981*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_GetInputStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcLMH1981_GetInputStatus(DtBcLMH1981 * pBc, DtBcLMH1981_Status * pStatus)
{
    UInt32  RegData;
    Int* pVf;

    // Sanity check
    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pStatus == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_LMH1981_MUST_BE_ENABLED(pBc);

    // Determine Status
    RegData = LMH1981_SyncRate_READ(pBc);
    pStatus->m_Valid = LMH1981_SyncRate_GET_HsRate(RegData) != 0;
    // Line period and Field period in nano seconds
    pStatus->m_LinePeriod = (LMH1981_SyncRate_GET_HsRate(RegData) * 1000)/256;
    pStatus->m_FieldPeriod = LMH1981_SyncRate_GET_VsRate(RegData) * 1000;
    RegData = LMH1981_NumLines_READ(pBc);
    pStatus->m_NumLinesF1 = LMH1981_NumLines_GET_Field1(RegData);
    pStatus->m_NumLinesF2 = LMH1981_NumLines_GET_Field2(RegData);
    RegData = LMH1981_VideoFormat_READ(pBc);
    pVf = &pStatus->m_VideoFormat;
    switch(RegData)
    {
        case LMH1981_FORMAT_NtscF1:  *pVf = DT_LMH1981_FORMAT_525I_F1; break;
        case LMH1981_FORMAT_NtscF2:  *pVf = DT_LMH1981_FORMAT_525I_F2; break;
        case LMH1981_FORMAT_PalF1:   *pVf = DT_LMH1981_FORMAT_625I_F1; break;
        case LMH1981_FORMAT_PalF2:   *pVf = DT_LMH1981_FORMAT_625I_F2; break;
        case LMH1981_FORMAT_480p:    *pVf = DT_LMH1981_FORMAT_480P; break;
        case LMH1981_FORMAT_576p:    *pVf = DT_LMH1981_FORMAT_576P; break;
        case LMH1981_FORMAT_720p:    *pVf = DT_LMH1981_FORMAT_720P; break;
        case LMH1981_FORMAT_1080iF1: *pVf = DT_LMH1981_FORMAT_1080I_F1; break;
        case LMH1981_FORMAT_1080iF2: *pVf = DT_LMH1981_FORMAT_1080I_F2; break;
        case LMH1981_FORMAT_1080p:   *pVf = DT_LMH1981_FORMAT_1080P; break;
        default:                     *pVf = DT_LMH1981_FORMAT_UNKNOWN; break;
    }
    return DT_STATUS_OK;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLMH1981 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcLMH1981_Init(DtBc* pBcBase)
{
    // No actions needed yet

    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLMH1981 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define LMH1981_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcLMH1981))
// Helper macro to cast stub's DtBc member to DtBcLMH1981
#define LMH1981_STUB   ((DtIoStubBcLMH1981*)pStub)
#define LMH1981_BC  ((DtBcLMH1981*)LMH1981_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcLMH1981_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcLMH1981_OnCmdGetInputStatus(
                                                   const DtIoStubBcLMH1981*, 
                                                  DtIoctlLmh1981CmdGetInputStatusOutput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_LMH1981;

static const DtIoctlProperties  IOSTUB_BC_LMH1981_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_LMH1981_CMD(
        DtIoStubBcLMH1981_OnCmd,
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLMH1981 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLMH1981_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcLMH1981_Close(DtIoStub* pStub)
{
    LMH1981_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLMH1981_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcLMH1981*  DtIoStubBcLMH1981_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_LMH1981_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcLMH1981, pBc, NULL, 
                                                            DtIoStubBcLMH1981_Close,
                                                            NULL,  // Use default IOCTL
                                                            IOSTUB_BC_LMH1981_IOCTLS);
    return (DtIoStubBcLMH1981*)DtIoStubBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLMH1981 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLMH1981_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcLMH1981_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    DtIoctlLmh1981CmdOutput*  pOutData = NULL;

    LMH1981_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_LMH1981_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, LMH1981, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_Lmh1981Cmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_LMH1981_CMD_GET_INPUT_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcLMH1981_OnCmdGetInputStatus(LMH1981_STUB,  
                                                               &pOutData->m_GetInpStatus);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLMH1981_OnCmdGetInputStatus -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcLMH1981_OnCmdGetInputStatus(
    const DtIoStubBcLMH1981* pStub,
    DtIoctlLmh1981CmdGetInputStatusOutput* pOutData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcLMH1981_Status  InpStatus;
    LMH1981_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    Status =  DtBcLMH1981_GetInputStatus(LMH1981_BC, &InpStatus);
    if (DT_SUCCESS(Status))
    {
        pOutData->m_Valid = (Int)(InpStatus.m_Valid);
        pOutData->m_LinePeriod = InpStatus.m_LinePeriod;
        pOutData->m_FieldPeriod = InpStatus.m_FieldPeriod;
        pOutData->m_NumLinesF1 = InpStatus.m_NumLinesF1;
        pOutData->m_NumLinesF2 = InpStatus.m_NumLinesF2;
        pOutData->m_VideoFormat = InpStatus.m_VideoFormat;
    }
    return Status;
}
