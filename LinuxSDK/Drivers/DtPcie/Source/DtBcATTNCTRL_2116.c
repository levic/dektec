//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcATTNCTRL_2116.c *#*#*#*#*#*#*#*#*#*# (C) 2021 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Copyright (C) 2021 DekTec Digital Video B.V.
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
#include "DtBcATTNCTRL_2116.h"
#include "DtBcATTNCTRL_2116_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcATTNCTRL_2116 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_ATTNCTRL_2116_DEFAULT_PRECONDITIONS(pBc)      \
                            DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcATTNCTRL_2116))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_ATTNCTRL_2116_MUST_BE_ENABLED(pBc)  BC_MUST_BE_ENABLED_IMPL(ATTNCTRL_2116, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcATTNCTRL_2116_Init(DtBc*);
static DtStatus  DtBcATTNCTRL_2116_OnCloseFile(DtBc*, const DtFileObject*);
DtStatus DtBcATTNCTRL_2116_GetAttenuators(DtBcATTNCTRL_2116 *pBc, UInt8* pFirst, 
                                                            UInt8* pMiddle, UInt8* pLast);
DtStatus DtBcATTNCTRL_2116_SetAttenuators(DtBcATTNCTRL_2116 *pBc, UInt8 First, 
                                                                UInt8 Middle, UInt8 Last);

// =+=+=+=+=+=+=+=+=+=+=+=+ DtBcATTNCTRL_2116 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcATTNCTRL_2116_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcATTNCTRL_2116_Close(DtBc* pBc)
{
    BC_ATTNCTRL_2116_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcATTNCTRL_2116_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcATTNCTRL_2116*  DtBcATTNCTRL_2116_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_ATTNCTRL_2116_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcATTNCTRL_2116, Id, DT_BLOCK_TYPE_ATTNCTRL_2116,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcATTNCTRL_2116_Close;
    OpenParams.m_InitFunc = DtBcATTNCTRL_2116_Init;
    OpenParams.m_OnCloseFileFunc = DtBcATTNCTRL_2116_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcATTNCTRL_2116*)DtBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtBcATTNCTRL_2116 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcATTNCTRL_2116_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcATTNCTRL_2116_Init(DtBc* pBcBase)
{
    DtBcATTNCTRL_2116* pBc = (DtBcATTNCTRL_2116*)pBcBase;

    // Sanity checks
    BC_ATTNCTRL_2116_DEFAULT_PRECONDITIONS(pBc);

    // Set defaults (max. attenuation)
    DtBcATTNCTRL_2116_SetAttenuators(pBc, 63, 63, 63);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcATTNCTRL_2116_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcATTNCTRL_2116_OnCloseFile(DtBc* pBc, const DtFileObject* pFile)
{
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_ATTNCTRL_2116_DEFAULT_PRECONDITIONS(pBc);
   
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcATTNCTRL_2116_GetAttenuators -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcATTNCTRL_2116_GetAttenuators(DtBcATTNCTRL_2116 *pBc, UInt8* pFirst, 
                                                             UInt8* pMiddle, UInt8* pLast)
{
    // Sanity check
    BC_ATTNCTRL_2116_DEFAULT_PRECONDITIONS(pBc);

    // return cached values
    *pFirst = pBc->m_FirstAttenuator;
    *pMiddle = pBc->m_MiddleAttenuator;
    *pLast = pBc->m_LastAttenuator;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcATTNCTRL_2116_SetAttenuators -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcATTNCTRL_2116_SetAttenuators(DtBcATTNCTRL_2116 *pBc, UInt8 First, 
                                                                 UInt8 Middle, UInt8 Last)
{
     UInt32 RegData=0;

    // Sanity check
    BC_ATTNCTRL_2116_DEFAULT_PRECONDITIONS(pBc);

    if (First > 63 || Middle > 63 || Last>63)
        return DT_STATUS_INVALID_PARAMETER;

    pBc->m_FirstAttenuator = First;
    pBc->m_MiddleAttenuator = Middle;
    pBc->m_LastAttenuator = Last;

    RegData = ATTNCTRL_2116_Config_SET_FirstAttenuator(RegData, First);
    RegData = ATTNCTRL_2116_Config_SET_MiddleAttenuator(RegData, Middle);
    RegData = ATTNCTRL_2116_Config_SET_LastAttenuator(RegData, Last);
    ATTNCTRL_2116_Config_WRITE(pBc, RegData);
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcATTNCTRL_2116 implementation +=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define ATTNCTRL_2116_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                  DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcATTNCTRL_2116))
// Helper macro to cast stub's DtBc member to DtBcATTNCTRL_2116
#define ATTNCTRL_2116_STUB   ((DtIoStubBcATTNCTRL_2116*)pStub)
#define ATTNCTRL_2116_BC  ((DtBcATTNCTRL_2116*)ATTNCTRL_2116_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcATTNCTRL_2116_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcATTNCTRL_2116_OnCmdGetAttenuators(
                                     const DtIoStubBcATTNCTRL_2116*,
                                     DtIoctlAttenuatorsCtrlCmd_2116GetAttenuatorsOutput*);
static DtStatus  DtIoStubBcATTNCTRL_2116_OnCmdSetAttenuators(
                                const DtIoStubBcATTNCTRL_2116*,
                                const DtIoctlAttenuatorsCtrlCmd_2116SetAttenuatorsInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_ATTNCTRL_2116;

static const DtIoctlProperties  IOSTUB_BC_ATTNCTRL_2116_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_ATTNCTRL_2116_CMD(
        DtIoStubBcATTNCTRL_2116_OnCmd,
        NULL, NULL),
};

// +=+=+=+=+=+=+=+=+=+=+ DtIoStubBcATTNCTRL_2116 - Public functions +=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcATTNCTRL_2116_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcATTNCTRL_2116_Close(DtIoStub* pStub)
{
    ATTNCTRL_2116_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcATTNCTRL_2116_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcATTNCTRL_2116*  DtIoStubBcATTNCTRL_2116_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_ATTNCTRL_2116_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcATTNCTRL_2116, pBc, NULL, 
                                                          DtIoStubBcATTNCTRL_2116_Close,
                                                          NULL,  // Use default IOCTL
                                                          IOSTUB_BC_ATTNCTRL_2116_IOCTLS);
    return (DtIoStubBcATTNCTRL_2116*)DtIoStubBc_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+ DtIoStubBcATTNCTRL_2116 - Private functions +=+=+=+=+=+=+=+=+=+=+


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcATTNCTRL_2116_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcATTNCTRL_2116_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlAttenuatorsCtrlCmdInput_2116*  pInData = NULL;
    DtIoctlAttenuatorsCtrlCmdOutput_2116*  pOutData = NULL;

    ATTNCTRL_2116_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_ATTNCTRL_CMD_2116);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, ATTNCTRL_2116, pStub,
                                                      "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_AttenuatorsCtrlCmd_2116;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_AttenuatorsCtrlCmd_2116;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-
    switch (pIoParams->m_Cmd)
    {
    case DT_ATTNCTRL_CMD_2116_GET_ATTENUATORS:
         DT_ASSERT(pOutData != NULL);
         Status = DtIoStubBcATTNCTRL_2116_OnCmdGetAttenuators(ATTNCTRL_2116_STUB,
                                                             &pOutData->m_GetAttenuators);
        break;
    case DT_ATTNCTRL_CMD_2116_SET_ATTENUATORS:
         DT_ASSERT(pInData != NULL);
         Status = DtIoStubBcATTNCTRL_2116_OnCmdSetAttenuators(ATTNCTRL_2116_STUB,
                                                              &pInData->m_SetAttenuators);
         break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcATTNCTRL_2116_OnCmdGetAttenuators -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcATTNCTRL_2116_OnCmdGetAttenuators(
                             const DtIoStubBcATTNCTRL_2116* pStub,
                             DtIoctlAttenuatorsCtrlCmd_2116GetAttenuatorsOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcATTNCTRL_2116));
    DT_ASSERT(pOutData!=NULL);

    return DtBcATTNCTRL_2116_GetAttenuators(ATTNCTRL_2116_BC,
                                                            &pOutData->m_FirstAttenuator,
                                                            &pOutData->m_MiddleAttenuator,
                                                            &pOutData->m_LastAttenuator);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcATTNCTRL_2116_OnCmdSetAttenuators -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcATTNCTRL_2116_OnCmdSetAttenuators(
                        const DtIoStubBcATTNCTRL_2116* pStub,
                         const DtIoctlAttenuatorsCtrlCmd_2116SetAttenuatorsInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcATTNCTRL_2116));
    DT_ASSERT(pInData!=NULL);

    return DtBcATTNCTRL_2116_SetAttenuators(ATTNCTRL_2116_BC, pInData->m_FirstAttenuator,
                                                              pInData->m_MiddleAttenuator,
                                                              pInData->m_LastAttenuator);
}