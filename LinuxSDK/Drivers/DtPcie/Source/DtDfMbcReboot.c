// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfMbcReboot.c *#*#*#*#*#*#*#*#*#* (C) 2024 DekTec
//
//
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2024 DekTec Digital Video B.V.
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
#include "DtDfMbcReboot.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// FMBC Master role
#define FMBC_ROLE_NONE        NULL

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMbcReboot implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtDfMbcReboot function
#define DF_MBCREBOOT_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfMbcReboot))


// Helper macro to cast a DtDf* to a DtDfMbcReboot*
#define DF_MBCREBOOT      ((DtDfMbcReboot*)pDf)

static DtStatus  DtDfMbcReboot_Init(DtDf*);
static DtStatus  DtDfMbcReboot_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfMbcReboot_OpenChildren(DtDfMbcReboot*);

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMbcReboot - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcReboot_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfMbcReboot_Close(DtDf* pDf)
{
    DF_MBCREBOOT_DEFAULT_PRECONDITIONS(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcReboot_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfMbcReboot* DtDfMbcReboot_Open(DtCore* pCore, DtPt* pPt,
    const char* pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // No stub available

    // Init open parameters
    DT_DF_MBCREBOOT_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfMbcReboot, Id, DT_FUNC_TYPE_MBCREBOOT, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfMbcReboot_Close;
    OpenParams.m_InitFunc = DtDfMbcReboot_Init;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfMbcReboot_OnEnablePreChildren;

    // Use base function to allocate and perform standard initialization of function data
    return (DtDfMbcReboot*)DtDf_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcReboot_Reboot -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfMbcReboot_Reboot(DtDfMbcReboot* pDf, UInt64 Address)
{
    // Sanity check
    DF_MBCREBOOT_DEFAULT_PRECONDITIONS(pDf);

    // Check state
    if (!DtDf_IsEnabled((DtDf*)pDf))
        return DT_STATUS_NOT_ENABLED;

    // Store request
    pDf->m_Reboot = TRUE;
    pDf->m_ImageAddress = Address;

    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcReboot_GetSptAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfMbcReboot_GetSptAddress(DtDfMbcReboot* pDf, UInt64* pAddress1, 
                                                                        UInt64* pAddress2)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32 RdData[4];
 
    // Sanity check
    DF_MBCREBOOT_DEFAULT_PRECONDITIONS(pDf);

    // Check state
    if (!DtDf_IsEnabled((DtDf*)pDf))
        return DT_STATUS_NOT_ENABLED;

    // Get SPT
    Status = DtBcFMBC_WriteRead(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_RSU_GET_SPT, 0, NULL,
                                                                          4, RdData, 100);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, MBCREBOOT, pDf, "ERROR: failed to get SPT");
        return Status;
    }
    *pAddress1 = ((UInt64)RdData[0]) << 32;
    *pAddress1 |= RdData[1];
    *pAddress2 = ((UInt64)RdData[2]) << 32;
    *pAddress2 |= RdData[3];

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcReboot_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfMbcReboot_GetStatus(DtDfMbcReboot* pDf, DtDfMbcRebootStatus* pStatus)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32 RdData[9];

    // Sanity check
    DF_MBCREBOOT_DEFAULT_PRECONDITIONS(pDf);

    // Check state
    if (!DtDf_IsEnabled((DtDf*)pDf))
        return DT_STATUS_NOT_ENABLED;

    // Get current remote system upgrade status.
    Status = DtBcFMBC_WriteRead(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_RSU_STATUS, 0, NULL,
                                                                          9, RdData, 100);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, MBCREBOOT, pDf, "ERROR: failed to get RSU status");
        return Status;
    }
    pStatus->m_CurrentImage = ((UInt64)RdData[0]) << 32;
    pStatus->m_CurrentImage |= RdData[1];
    pStatus->m_FailingImage = ((UInt64)RdData[2]) << 32;
    pStatus->m_FailingImage |= RdData[3];
    pStatus->m_State = RdData[4];
    pStatus->m_Version = RdData[5];
    pStatus->m_ErrorLocation = RdData[6];
    pStatus->m_ErrorDetails = RdData[7];
    pStatus->m_RetryCounter = RdData[8];

    return DT_STATUS_OK;
}


// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMbcReboot - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcReboot_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMbcReboot_Init(DtDf* pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMbcReboot* pDf = (DtDfMbcReboot*)pDfBase;

    // Sanity checks
    DF_MBCREBOOT_DEFAULT_PRECONDITIONS(pDf);

    // Disable reboot
    pDf->m_Reboot = FALSE;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfMbcReboot_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, MBCREBOOT, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcReboot_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMbcReboot_OnEnablePreChildren(DtDf* pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMbcReboot* pDf = (DtDfMbcReboot*)pDfBase;

    // Sanity checks
    DF_MBCREBOOT_DEFAULT_PRECONDITIONS(pDf);

    if (!Enable && pDf->m_Reboot)
    {
        // Issue image update
        UInt32 WrData[2];
        WrData[0] = (UInt32)pDf->m_ImageAddress;         // Start address
        WrData[1] = (UInt32)(pDf->m_ImageAddress >> 32); // Assumed to be 0

        Status = DtBcFMBC_Write(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_RSU_IMAGE_UPDATE,
                                                                          2, WrData, 100);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, MBCREBOOT, pDf, "ERROR: failed to issue image update");
            return Status;
        }
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcReboot_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMbcReboot_OpenChildren(DtDfMbcReboot* pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the the MBCREBOOT function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] =
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_FMBC, DT_BC_FMBC_NAME,
                               FMBC_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcFmbc), TRUE}
    };

    DF_MBCREBOOT_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
        DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pDf->m_pBcFmbc != NULL);
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfMbcReboot implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtDf member to DtDfMbcReboot
#define MBCREBOOT_STUB   ((DtIoStubDfMbcReboot*)pStub)
#define MBCREBOOT_DF  ((DtDfMbcReboot*)MBCREBOOT_STUB->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubDfMbcReboot_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_MBCREBOOT;

static const DtIoctlProperties  IOSTUB_DF_MBCREBOOT_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubDf_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_MBCREBOOT_CMD(
        DtIoStubDfMbcReboot_OnCmd,
        NULL,  NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfMbcReboot - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcReboot_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubDfMbcReboot_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfMbcReboot));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcReboot_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubDfMbcReboot*  DtIoStubDfMbcReboot_Open(DtDf*  pDf)
{
    DtIoStubDfMbcReboot*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfMbcReboot));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfMbcReboot, pDf, NULL,
                                                              DtIoStubDfMbcReboot_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_DF_MBCREBOOT_IOCTLS);
    pStub = (DtIoStubDfMbcReboot*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfMbcReboot- Private functions +=+=+=+=+=+=+=+=+=+=+=+=


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcReboot_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfMbcReboot_OnCmd(const DtIoStub* pStub, DtIoStubIoParams* pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlMbcRebootCmdInput*  pInData = NULL;
    DtIoctlMbcRebootCmdOutput* pOutData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfMbcReboot));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_MBCREBOOT_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, MBCREBOOT, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_MbcRebootCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_MbcRebootCmd;
    }
    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_MBCREBOOT_CMD_GET_SPT_ADDRESS:
        DT_ASSERT(pOutData != NULL);
        Status = DtDfMbcReboot_GetSptAddress(MBCREBOOT_DF,
                                                       &pOutData->m_GetSptAddress.m_Spt0,
                                                       &pOutData->m_GetSptAddress.m_Spt1);
        break;
    case DT_MBCREBOOT_CMD_GET_STATUS:
    {
        DtDfMbcRebootStatus RcfgStatus;
        DT_ASSERT(pOutData != NULL);
        Status = DtDfMbcReboot_GetStatus(MBCREBOOT_DF, &RcfgStatus);
        if (Status == DT_STATUS_OK)
        {
            pOutData->m_GetStatus.m_CurrentImage = RcfgStatus.m_CurrentImage;
            pOutData->m_GetStatus.m_FailingImage = RcfgStatus.m_CurrentImage;
            pOutData->m_GetStatus.m_State = RcfgStatus.m_State;
            pOutData->m_GetStatus.m_Version = RcfgStatus.m_Version;
            pOutData->m_GetStatus.m_ErrorLocation = RcfgStatus.m_ErrorLocation;
            pOutData->m_GetStatus.m_ErrorDetails = RcfgStatus.m_ErrorDetails;
            pOutData->m_GetStatus.m_ErrorDetails = RcfgStatus.m_ErrorDetails;
        }
    }
    break;
    case DT_MBCREBOOT_CMD_REBOOT:
        DT_ASSERT(pInData != NULL);
        Status = DtDfMbcReboot_Reboot(MBCREBOOT_DF, pInData->m_Reboot.m_ImageAddress);
    break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}
