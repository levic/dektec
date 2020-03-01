//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcREBOOT.c *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtBc.h"
#include "DtBcREBOOT.h"
#include "DtBcREBOOT_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcREBOOT implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_REBOOT_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcREBOOT))

// Helper macro to cast a DtBc* to a DtBcREBOOT*
#define BC_REBOOT         ((DtBcREBOOT*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcREBOOT_Init(DtBc*);
static DtStatus  DtBcREBOOT_OnEnable(DtBc*, Bool  Enable);
static DtStatus  DtBcREBOOT_CheckRebootRequest(DtBcREBOOT*);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcREBOOT - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcREBOOT_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcREBOOT_Close(DtBc*  pBc)
{
    BC_REBOOT_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcREBOOT_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcREBOOT*  DtBcREBOOT_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_REBOOT_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcREBOOT, Id, DT_BLOCK_TYPE_REBOOT, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcREBOOT_Close;
    OpenParams.m_InitFunc = DtBcREBOOT_Init;
    OpenParams.m_OnEnableFunc = DtBcREBOOT_OnEnable;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcREBOOT*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcREBOOT_Reboot -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcREBOOT_Reboot(DtBcREBOOT*  pBc, const DtFileObject* pFile, Int RebootDelay)
{
    // Sanity check    
    BC_REBOOT_DEFAULT_PRECONDITIONS(pBc);

    // Check state
    if (!DtBc_IsEnabled((DtBc*)pBc))
        return DT_STATUS_NOT_ENABLED;

    // Check parameter
    if (RebootDelay<0 || RebootDelay>=512)
        return DT_STATUS_INVALID_PARAMETER;

    // Save FPGA reboot request
    pBc->m_RebootDelay = RebootDelay;

    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcREBOOT - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcREBOOT_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcREBOOT_Init(DtBc*  pBc)
{
    // Set reboot delay to not set (=-1)
    BC_REBOOT->m_RebootDelay = -1;

    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcREBOOT_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcREBOOT_OnEnable(DtBc* pBc, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    // On disable check FPGA reboot request
    if (!Enable)
        Status = DtBcREBOOT_CheckRebootRequest(BC_REBOOT);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcREBOOT_CheckRebootRequest -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcREBOOT_CheckRebootRequest(DtBcREBOOT*  pBc)
{
    // Check whether there is a request for reboot
    if (pBc->m_RebootDelay >= 0)
    {
        DtDbgOutBc(MIN, REBOOT, pBc, "FPGA reboots after %d ms",  pBc->m_RebootDelay);
 
        // Initiate FPGA reboot
        REBOOT_RebootDelay_WRITE(pBc, (UInt32)pBc->m_RebootDelay);
    }
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcREBOOT implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcREBOOT
#define REBOOT_STUB   ((DtIoStubBcREBOOT*)pStub)
#define REBOOT_BC  ((DtBcREBOOT*)REBOOT_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcREBOOT_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_REBOOT;

static const DtIoctlProperties  IOSTUB_BC_REBOOT_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_REBOOT_CMD(
        DtIoStubBcREBOOT_OnCmd,
        NULL,  NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcREBOOT - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcREBOOT_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcREBOOT_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcREBOOT));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcREBOOT_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcREBOOT*  DtIoStubBcREBOOT_Open(DtBc*  pBc)
{
    DtIoStubBcREBOOT*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcREBOOT));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcREBOOT, pBc, NULL, 
                                                              DtIoStubBcREBOOT_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_BC_REBOOT_IOCTLS);
    pStub = (DtIoStubBcREBOOT*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcREBOOT- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcREBOOT_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcREBOOT_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlRebootCmdInput*  pInData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcREBOOT));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_REBOOT_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, REBOOT, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_Reboot;

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_REBOOT_CMD_FPGA_REBOOT:
        DT_ASSERT(pInData != NULL);
        Status = DtBcREBOOT_Reboot(REBOOT_BC, pIoParams->m_pFile, 
                                                     pInData->m_FpgaReboot.m_RebootDelay);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}
