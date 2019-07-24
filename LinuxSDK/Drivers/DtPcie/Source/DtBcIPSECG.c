// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIPSECG.c *#*#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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
#include "DtBcIPSECG.h"
#include "DtBcIPSECG_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPSECG implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IPSECG_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIPSECG))

// Helper macro to cast a DtBc* to a DtBcIPSECG*
#define BC_IPSECG         ((DtBcIPSECG*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIPSECG_Init(DtBc*);
//static DtStatus  DtBcIPSECG_OnCloseFile(DtBc*, const DtFileObject*);


// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPSECG - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSECG_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIPSECG_Close(DtBc*  pBc)
{
    // Sanity checks
    BC_IPSECG_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSECG_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIPSECG*  DtBcIPSECG_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_IPSECG_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIPSECG, Id, DT_BLOCK_TYPE_IPSECG, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_InitFunc = DtBcIPSECG_Init;
    OpenParams.m_CloseFunc = DtBcIPSECG_Close;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIPSECG*)DtBc_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSECG_Challenge -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIPSECG_Challenge(
    DtBcIPSECG*  pBc, 
    const UInt32  Challenge[3],
    UInt32  Response[3])
{
    Int  Timeout=0;
    // Sanity checks
    BC_IPSECG_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(Challenge!=NULL && Response!=NULL);

    // Protect against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Must be idle
    if (IPSECG_Status_READ_IsAlgoBusy(pBc)!=0)
    {
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return DT_STATUS_BUSY;
    }
        
    // Write challenge
    IPSECG_Challenge0_WRITE(pBc, Challenge[0]);
    IPSECG_Challenge1_WRITE(pBc, Challenge[1]);
    IPSECG_Challenge2_WRITE(pBc, Challenge[2]);

    // Issue command
    IPSECG_Command_WRITE(pBc, IPSECG_CMD_EncryptChallenge);
    // Wait for command to complete (wait maximal 10ms)
    Timeout = 10;
    while (IPSECG_Status_READ_IsAlgoBusy(pBc)!=0 && Timeout>0)
    {
        DtSleep(1);
        Timeout--;
    }
    if (Timeout <= 0)
    {
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return DT_STATUS_TIMEOUT;
    }
    
    // Read response
    Response[0] = IPSECG_Response0_READ(pBc);
    Response[1] = IPSECG_Response1_READ(pBc);
    Response[2] = IPSECG_Response2_READ(pBc);

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSECG_Check -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIPSECG_Check(DtBcIPSECG*  pBc, const UInt32*  pData, Int  NumWords)
{
    Int  i=0, Timeout = 0;

    // Sanity checks
    BC_IPSECG_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(NumWords==0 || (pData!=NULL && NumWords>0));

    // Protect against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Must be idle
    if (IPSECG_Status_READ_IsAlgoBusy(pBc)!=0)
    {
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return DT_STATUS_BUSY;
    }

    // Issue command
    IPSECG_Command_WRITE(pBc, IPSECG_CMD_CheckBoardId);
    
    // Write data
    for (i=0; i<NumWords; i++)
        IPSECG_EncryptedStr_WRITE(pBc, pData[i]);

    // Wait for command to complete (wait maximal 10ms)
    Timeout = 10;
    while (IPSECG_Status_READ_IsAlgoBusy(pBc)!=0 && Timeout>0)
    {
        DtSleep(1);
        Timeout--;
    }
    if (Timeout <= 0)
    {
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return DT_STATUS_TIMEOUT;
    }

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSECG_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIPSECG_GetStatus(
    DtBcIPSECG*  pBc, 
    Int64* pBoardId,
    Bool* pIsBusy,
    Bool*  pIsOk)
{
    UInt32  Reg_IPSECG=0;
    
    // Sanity checks
    BC_IPSECG_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pBoardId!=NULL && pIsBusy!=NULL && pIsOk!=NULL);
    
    // Cache Status register
    Reg_IPSECG = IPSECG_Status_READ(pBc);
    *pIsOk = (IPSECG_Status_GET_IsSecurityOk(Reg_IPSECG)!=0);
    *pIsBusy = (IPSECG_Status_GET_IsAlgoBusy(Reg_IPSECG)!=0);

    // Get board ID
    *pBoardId = IPSECG_BoardId0_READ(pBc);
    *pBoardId |= ((Int64)IPSECG_BoardId1_READ(pBc))<<32;
    
    return DT_STATUS_OK;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPSECG - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSECG_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIPSECG_Init(DtBc*  pBc)
{
    // Sanity checks
    BC_IPSECG_DEFAULT_PRECONDITIONS(pBc);

    // Initialize mutex against concurrent access
    DtFastMutexInit(&BC_IPSECG->m_AccessMutex);
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIPSECG implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcIPSECG
#define IPSECG_STUB   ((DtIoStubBcIPSECG*)pStub)
#define IPSECG_BC     ((DtBcIPSECG*)IPSECG_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIPSECG_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubBcIPSECG_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcIPSECG_OnCmdGetStatus(const DtIoStubBcIPSECG*, 
                                                        DtIoctlIpSecGCmdGetStatusOutput*);
static DtStatus  DtIoStubBcIPSECG_OnCmdCheck(const DtIoStubBcIPSECG*, 
                                                       const DtIoctlIpSecGCmdCheckInput*);
static DtStatus  DtIoStubBcIPSECG_OnCmdChallenge(const DtIoStubBcIPSECG*, 
                                                    const DtIoctlIpSecGCmdChallengeInput*,
                                                    DtIoctlIpSecGCmdChallengeOutput*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IPSECG_CMD;

static const DtIoctlProperties  IOSTUB_BC_IPSECG_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IPSECG_CMD(
        DtIoStubBcIPSECG_OnCmd,
        DtIoStubBcIPSECG_AppendDynamicSize,
        NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIPSECG - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIPSECG_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcIPSECG_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIPSECG));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIPSECG_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIPSECG*  DtIoStubBcIPSECG_Open(DtBc*  pBc)
{
    DtIoStubBcIPSECG*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIPSECG));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIPSECG, pBc, NULL, 
                                                               DtIoStubBcIPSECG_Close,
                                                               NULL,  // Use default IOCTL
                                                               IOSTUB_BC_IPSECG_IOCTLS);
    pStub = (DtIoStubBcIPSECG*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIPSECG- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIPSECG_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIPSECG_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIpSecGCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIPSECG));
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_IPSECG_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IpSecGCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_IPSECG_CMD_CHECK:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlIpSecGCmdCheckInput));
        // Add dynamic size (i.e. #numword to check)
        pIoParams->m_InReqSize += (pInData->m_Check.m_NumWords * sizeof(UInt32));
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIPSECG_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIPSECG_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIpSecGCmdInput*  pInData = NULL;
    DtIoctlIpSecGCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIPSECG));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_IPSECG_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IPSECG, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IpSecGCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IpSecGCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IPSECG_CMD_GET_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIPSECG_OnCmdGetStatus(IPSECG_STUB, &pOutData->m_GetStatus);
        break;

    case DT_IPSECG_CMD_CHECK:
        Status = DtIoStubBcIPSECG_OnCmdCheck(IPSECG_STUB, &pInData->m_Check);
        break;

    case DT_IPSECG_CMD_CHALLENGE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIPSECG_OnCmdChallenge(IPSECG_STUB, &pInData->m_Challenge, 
                                                                  &pOutData->m_Challenge);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIPSECG_OnCmdGetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIPSECG_OnCmdGetStatus(
    const DtIoStubBcIPSECG*  pStub, 
    DtIoctlIpSecGCmdGetStatusOutput*  pOutData)
{
    DtStatus  Status;
    Bool  IsBusy, IsOk;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIPSECG));
    DT_ASSERT(pOutData != NULL);
    
    // Forward command to BC
    Status =  DtBcIPSECG_GetStatus(IPSECG_BC, &pOutData->m_BoardId, &IsBusy, &IsOk);
    pOutData->m_IsBusy = (Int)IsBusy;
    pOutData->m_IsOk = (Int)IsOk;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIPSECG_OnCmdCheck -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIPSECG_OnCmdCheck(
    const DtIoStubBcIPSECG*  pStub,
    const DtIoctlIpSecGCmdCheckInput*  pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIPSECG));
    DT_ASSERT(pInData != NULL);

    // Forward command to BC
    return DtBcIPSECG_Check(IPSECG_BC, pInData->m_Data, pInData->m_NumWords);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIPSECG_OnCmdChallenge -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIPSECG_OnCmdChallenge(
    const DtIoStubBcIPSECG*  pStub, 
    const DtIoctlIpSecGCmdChallengeInput*  pInData,
    DtIoctlIpSecGCmdChallengeOutput*  pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIPSECG));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    // Forward command to BC
    return DtBcIPSECG_Challenge(IPSECG_BC, pInData->m_Data, pOutData->m_Data);
}