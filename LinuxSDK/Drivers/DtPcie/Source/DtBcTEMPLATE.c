//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcTEMPLATE.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "TEMPLATE" by the building block's shortname (all uppercase)
// and replace "Template" by the building block's shortname in camel case. 
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
#include "DtBcTEMPLATE.h"
#include "DtBcTEMPLATE_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcTEMPLATE implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_TEMPLATE_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcTEMPLATE))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_TEMPLATE_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(TEMPLATE, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcTEMPLATE_Init(DtBc*);
static DtStatus  DtBcTEMPLATE_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcTEMPLATE_OnEnable(DtBc*, Bool  Enable);
static DtStatus  DtBcTEMPLATE_RegisterIntHandlers(DtBcTEMPLATE*);
static void  DtBcTEMPLATE_InterruptDpcDone(DtDpcArgs* pArgs);
static DtStatus  DtBcTEMPLATE_InterruptHandler(DtBc*, Int, Int, void*);
static void  DtBcTEMPLATE_SetControlRegs(DtBcTEMPLATE*, Bool BlkEna, Int OpMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcTEMPLATE - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTEMPLATE_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcTEMPLATE_Close(DtBc*  pBc)
{
    BC_TEMPLATE_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTEMPLATE_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcTEMPLATE*  DtBcTEMPLATE_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_TEMPLATE_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcTEMPLATE, Id, DT_BLOCK_TYPE_TEMPLATE,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcTEMPLATE_Close;
    OpenParams.m_InitFunc = DtBcTEMPLATE_Init;
    OpenParams.m_OnEnableFunc = DtBcTEMPLATE_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcTEMPLATE_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcTEMPLATE*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTEMPLATE_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcTEMPLATE_GetOperationalMode(DtBcTEMPLATE* pBc, Int* pOpMode)
{
    // Sanity check
    BC_TEMPLATE_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_TEMPLATE_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTEMPLATE_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcTEMPLATE_SetOperationalMode(DtBcTEMPLATE* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_TEMPLATE_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_TEMPLATE_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Change Operational Mode
    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        // IDLE or STANDBY -> RUN
        // Enable interrupt
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_TEMPLATE_DONE);
    }
    else if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    {
        // RUN -> STANDBY or IDLE
        // Disable interrupt
        Status = DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_TEMPLATE_TESTDONE);
    }

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcTEMPLATE_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcTEMPLATE - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTEMPLATE_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcTEMPLATE_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcTEMPLATE* pBc = (DtBcTEMPLATE*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set control register
    DtBcTEMPLATE_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    // Initialize Done event
    Status = DtEventInit(&pBc->m_DoneEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, TEMPLATE, pBc, 
                              "ERROR: failed to init done event (Status=0x%08X)", Status);
        return Status;
    }
    
    // Init interrupt DPC
    Status = DtDpcInit(&pBc->m_IntDpc, DtBcTEMPLATE_InterruptDpcDone, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, TEMPLATE, pBc, "ERROR: failed to init DPC (Status=0x%08X)",
                                                                                  Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Status = DtBcTEMPLATE_RegisterIntHandlers(pBc);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, TEMPLATE, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTEMPLATE_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcTEMPLATE_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcTEMPLATE* pBc = (DtBcTEMPLATE*)pBcBase;

    // Sanity check
    BC_TEMPLATE_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, TEMPLATE, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, TEMPLATE, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcTEMPLATE_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, TEMPLATE, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcTEMPLATE_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTEMPLATE_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcTEMPLATE_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcTEMPLATE* pBc = (DtBcTEMPLATE*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_TEMPLATE_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, TEMPLATE, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcTEMPLATE_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, TEMPLATE, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTEMPLATE_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcTEMPLATE_RegisterIntHandlers(DtBcTEMPLATE* pBc)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i = 0;

    // Sanity check
    BC_TEMPLATE_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks, but do not enable the interrupts. We will enable 
    // them on demand
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_TEMPLATE_TESTDONE:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id, 
                                                     DtBcTEMPLATE_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, TEMPLATE, pBc,
                               "ERROR: failed to register interrupt handler (ID=%d)", Id);
            }
            break;

        default:
            DT_ASSERT(FALSE);      // Unknown ID. New version of the block??
            break;
        }
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTEMPLATE_InterruptDpcDone -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcTEMPLATE_InterruptDpcDone(DtDpcArgs* pArgs)
{
    DtBcTEMPLATE*  pBc = (DtBcTEMPLATE*)pArgs->m_pContext;

    // Sanity check
#ifdef _DEBUG
    UInt32  IntId = pArgs->m_Data1.m_UInt32_1;
    DT_ASSERT(IntId == DT_INTERRUPT_TEMPLATE_TESTDONE);
#endif  // #ifdef _DEBUG
    BC_TEMPLATE_DEFAULT_PRECONDITIONS(pBc);

    // If necessary, re-enable the interrupt that was fired
    // In that case a spinlock is required for checking the current operational mode or
    // state, to prevent an interrupt is re-enabled while the driver is shutting down.

    DtEventSet(&pBc->m_DoneEvent);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTEMPLATE_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcTEMPLATE_InterruptHandler(DtBc* pBcBase, Int Id, Int Index, void* pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcTEMPLATE* pBc = (DtBcTEMPLATE*)pBcBase;
    DtDpcArgs  DpcArgs;

    // Sanity check
    BC_TEMPLATE_DEFAULT_PRECONDITIONS(pBc);

    // Check ID is one of ours
    switch (Id)
    {
    case DT_INTERRUPT_TEMPLATE_TESTDONE:
        break;

       // Not a TEMPLATE interrupt
    default:
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;

    // If necessary, disable the interrupt that was fired. 
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&pBc->m_IntDpc, &DpcArgs);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTEMPLATE_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcTEMPLATE_SetControlRegs(DtBcTEMPLATE* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData=0, FwBlkEna=0, FwOpMode=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? TEMPLATE_BLKENA_ENABLED : TEMPLATE_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = TEMPLATE_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = TEMPLATE_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = TEMPLATE_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = TEMPLATE_Control_READ(pBc);
    RegData = TEMPLATE_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = TEMPLATE_Control_SET_OperationalMode(RegData, FwOpMode);
    TEMPLATE_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcTEMPLATE implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define TEMPLATE_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcTEMPLATE))
// Helper macro to cast stub's DtBc member to DtBcTEMPLATE
#define TEMPLATE_STUB   ((DtIoStubBcTEMPLATE*)pStub)
#define TEMPLATE_BC  ((DtBcTEMPLATE*)TEMPLATE_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcTEMPLATE_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubBcTEMPLATE_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcTEMPLATE_OnCmdGetOperationalMode(
                                                   const DtIoStubBcTEMPLATE*, 
                                                      DtIoctlTemplateCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcTEMPLATE_OnCmdSetOperationalMode(
                                                 const DtIoStubBcTEMPLATE*,
                                                 const DtIoctlTemplateCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_TEMPLATE;

static const DtIoctlProperties  IOSTUB_BC_TEMPLATE_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_TEMPLATE_CMD(
        DtIoStubBcTEMPLATE_OnCmd,
        DtIoStubBcTEMPLATE_AppendDynamicSize,
        NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcTEMPLATE - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTEMPLATE_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcTEMPLATE_Close(DtIoStub* pStub)
{
    TEMPLATE_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTEMPLATE_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcTEMPLATE*  DtIoStubBcTEMPLATE_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_TEMPLATE_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcTEMPLATE, pBc, NULL, 
                                                            DtIoStubBcTEMPLATE_Close,
                                                            NULL,  // Use default IOCTL
                                                            IOSTUB_BC_TEMPLATE_IOCTLS);
    return (DtIoStubBcTEMPLATE*)DtIoStubBc_Open(&OpenParams);
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcTEMPLATE - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTEMPLATE_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcTEMPLATE_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlTemplateCmdInput*  pInData = NULL;

    TEMPLATE_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_TEMPLATE_CMD);
    
    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_TemplateCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTEMPLATE_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcTEMPLATE_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlTemplateCmdInput*  pInData = NULL;
    DtIoctlTemplateCmdOutput*  pOutData = NULL;

    TEMPLATE_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_TEMPLATE_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, TEMPLATE, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_TemplateCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_TemplateCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_TEMPLATE_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcTEMPLATE_OnCmdGetOperationalMode(TEMPLATE_STUB,  
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_TEMPLATE_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcTEMPLATE_OnCmdSetOperationalMode(TEMPLATE_STUB,  
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTEMPLATE_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTEMPLATE_OnCmdGetOperationalMode(
    const DtIoStubBcTEMPLATE* pStub,
    DtIoctlTemplateCmdGetOpModeOutput* pOutData)
{
    TEMPLATE_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcTEMPLATE_GetOperationalMode(TEMPLATE_BC, &pOutData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTEMPLATE_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTEMPLATE_OnCmdSetOperationalMode(
    const DtIoStubBcTEMPLATE* pStub,
    const DtIoctlTemplateCmdSetOpModeInput* pInData)
{
    TEMPLATE_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcTEMPLATE_SetOperationalMode(TEMPLATE_BC, pInData->m_OpMode);
}
