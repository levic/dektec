//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQCGRAB_2132.c *#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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
#include "DtBcIQCGRAB_2132.h"
#include "DtBcIQCGRAB_2132_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQCGRAB_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQCGRAB_2132_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQCGRAB_2132))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQCGRAB_2132_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQCGRAB_2132, pBc)

// Helper macro to cast a DtBc* to a DtBcIQCGRAB_2132*
#define BC_IQCGRAB_2132         ((DtBcIQCGRAB_2132*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQCGRAB_2132_Init(DtBc*);
static void  DtBcIQCGRAB_2132_InterruptDpc(DtDpcArgs* pArgs);
static DtStatus  DtBcIQCGRAB_2132_InterruptHandler(DtBc*, Int, Int, void*);
static DtStatus  DtBcIQCGRAB_2132_RegisterIntHandlers(DtBcIQCGRAB_2132*);

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQCGRAB_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQCGRAB_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQCGRAB_2132_Close(DtBc*  pBc)
{
    BC_IQCGRAB_2132_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQCGRAB_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIQCGRAB_2132*  DtBcIQCGRAB_2132_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_IQCGRAB_2132_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQCGRAB_2132, Id, DT_BLOCK_TYPE_IQCGRAB_2132, 
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQCGRAB_2132_Close;
    OpenParams.m_InitFunc = DtBcIQCGRAB_2132_Init;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQCGRAB_2132*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQCGRAB_2132_GetIqConstPoints -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQCGRAB_2132_GetIqConstPoints(DtBcIQCGRAB_2132* pBc, Int StreamId,
                                  Int NumIqPoints, Int16* pIqPoints, Int* pNumIqPointsOut)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  RegCapSettings;
    Int i;

    // Sanity check
    BC_IQCGRAB_2132_DEFAULT_PRECONDITIONS(pBc);

    *pNumIqPointsOut = 0;

    // Check parameters
    if (StreamId<0 || StreamId>0xFF)
        return DT_STATUS_INVALID_PARAMETER;
    if (pIqPoints==NULL || NumIqPoints<=0 || NumIqPoints>pBc->m_FifoSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Protect IQ grabber access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Reset done event
    DtEventReset(&pBc->m_DoneEvent);

    // Enable interrupts
    Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_IQCGRAB_2132_DONE);
    if (!DT_SUCCESS(Status))
    {
        // Release mutex
        DtFastMutexRelease(&pBc->m_AccessMutex);
        DT_ASSERT(FALSE);
        return Status;
    }

    // Set stream-ID and num to get and start capturing
    RegCapSettings = IQCGRAB_CaptureSettings_SET_NumSamples(0, (UInt)NumIqPoints);
    RegCapSettings = IQCGRAB_CaptureSettings_SET_StreamID(RegCapSettings, (UInt)StreamId);
    IQCGRAB_CaptureSettings_WRITE(pBc, RegCapSettings);
    IQCGRAB_CaptureControl_PULSE_Start(pBc);

    // Wait for done interrupt
    Status = DtEventWaitUnInt(&pBc->m_DoneEvent, 5000);
    if (!DT_SUCCESS(Status))
    {
        // Release mutex
        DtFastMutexRelease(&pBc->m_AccessMutex);
        DtDbgOutBc(ERR, IQCGRAB_2132, pBc, 
                                   "ERROR: Wait for done failed (Status=0x%08X)", Status);
        return Status;
    }

    // Read the data
    for (i=0; i<NumIqPoints; i++)
    {
        UInt32 RegIq = IQCGRAB_ConstPointFifo_READ(pBc);
        pIqPoints[i*2] = (Int16)(((Int32)IQCGRAB_ConstPointFifo_GET_I(RegIq)<<20) >>20);
        pIqPoints[i*2 + 1] = 
                           (Int16)(((Int32)IQCGRAB_ConstPointFifo_GET_Q(RegIq)<<20) >>20);
    }
    *pNumIqPointsOut = NumIqPoints;
    // Release mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQCGRAB_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQCGRAB_2132_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQCGRAB_2132_Init(DtBc*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    BC_IQCGRAB_2132_DEFAULT_PRECONDITIONS(pBc);

    // Init interrupt DPC
    Status = DtDpcInit(&BC_IQCGRAB_2132->m_IntDpc, DtBcIQCGRAB_2132_InterruptDpc, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, IQCGRAB_2132, pBc, 
                                     "ERROR: failed to init DPC (Status=0x%08X)", Status);
        return Status;
    }

    // Read config
    BC_IQCGRAB_2132->m_FifoSize = IQCGRAB_Configuration_READ_FifoSize(BC_IQCGRAB_2132);

    // Init mutex
    DtFastMutexInit(&BC_IQCGRAB_2132->m_AccessMutex);

    // Init completion event
    Status = DtEventInit(&BC_IQCGRAB_2132->m_DoneEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, IQCGRAB_2132, pBc, 
                              "ERROR: failed to init done event (Status=0x%08X)", Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Status = DtBcIQCGRAB_2132_RegisterIntHandlers(BC_IQCGRAB_2132);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, IQCGRAB_2132, pBc, "ERROR: failed to register interrupt handler");
        return Status;
    }

    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQCGRAB_2132_InterruptDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcIQCGRAB_2132_InterruptDpc(DtDpcArgs * pArgs)
{
    DtBcIQCGRAB_2132*  pBc = (DtBcIQCGRAB_2132*)pArgs->m_pContext;
    DtDbgOutBc(MAX, IQCGRAB_2132, pBc, "Transfer processed");
    DtEventSet(&pBc->m_DoneEvent);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQCGRAB_2132_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQCGRAB_2132_InterruptHandler(DtBc*  pBc, Int Id, Int Index, void*  pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQCGRAB_2132));

    // Check ID is one of ours
    if (Id != DT_INTERRUPT_IQCGRAB_2132_DONE)
    {
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Disable the IQCGRAB interrupt that was fired. 
    // NOTE: is re-enabled upon next transfer
    DtBc_InterruptDisable(pBc, Id);

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcIQCGRAB_2132*)pBc)->m_IntDpc, &DpcArgs);
    DT_ASSERT(DT_SUCCESS(Status));

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQCGRAB_2132_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQCGRAB_2132_RegisterIntHandlers(DtBcIQCGRAB_2132* pBc)
{
    Int  i=0;
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_IQCGRAB_2132_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_IQCGRAB_2132_DONE:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id,
                                                DtBcIQCGRAB_2132_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, IQCGRAB_2132, pBc,
                               "ERROR: failed to register interrupt handler (ID=%d)", Id);
            }
            break;

        default:
            DT_ASSERT(FALSE);      // Unknown ID. New version of the block??
            break;
        }
    }
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQCGRAB_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcIQCGRAB_2132
#define IQCGRAB_2132_STUB   ((DtIoStubBcIQCGRAB_2132*)pStub)
#define IQCGRAB_2132_BC  ((DtBcIQCGRAB_2132*)IQCGRAB_2132_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQCGRAB_2132_AppendDynamicSize(const DtIoStub*, 
                                                                      DtIoStubIoParams *);
static DtStatus  DtIoStubBcIQCGRAB_2132_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus DtIoStubBcIQCGRAB_2132_OnCmdGetIqConstPoints(
                                       const DtIoStubBcIQCGRAB_2132* , 
                                       const DtIoctlIqCGrabCmd_2132GetIqPointsInput*,
                                       DtIoctlIqCGrabCmd_2132GetIqPointsOutput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQCGRAB_2132;

static const DtIoctlProperties  IOSTUB_BC_IQCGRAB_2132_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQCGRAB_CMD_2132(
        DtIoStubBcIQCGRAB_2132_OnCmd,
        DtIoStubBcIQCGRAB_2132_AppendDynamicSize,
        NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQCGRAB_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQCGRAB_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcIQCGRAB_2132_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQCGRAB_2132));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQCGRAB_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcIQCGRAB_2132*  DtIoStubBcIQCGRAB_2132_Open(DtBc*  pBc)
{
    DtIoStubBcIQCGRAB_2132*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQCGRAB_2132));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQCGRAB_2132, pBc, NULL, 
                                                           DtIoStubBcIQCGRAB_2132_Close,
                                                           NULL,  // Use default IOCTL
                                                           IOSTUB_BC_IQCGRAB_2132_IOCTLS);
    pStub = (DtIoStubBcIQCGRAB_2132*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQCGRAB_2132- Private functions -.-.-.-.-.-.-.-.-.-.-.-


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQCGRAB_2132_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQCGRAB_2132_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqCGrabCmdInput_2132*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQCGRAB_2132));
    DT_ASSERT(pIoParams != NULL);                 
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_IQCGRAB_CMD_2132);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqCGrabCmd_2132;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_IQCGRAB_CMD_2132_GET_IQ_POINTS:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize 
                                       >= sizeof(DtIoctlIqCGrabCmd_2132GetIqPointsInput));
        DT_ASSERT(pIoParams->m_OutReqSize 
                                      >= sizeof(DtIoctlIqCGrabCmd_2132GetIqPointsOutput));
        // Add dynamic size (i.e. #bytes to get)
        pIoParams->m_OutReqSize += (pInData->m_GetIqPoints.m_NumIqPointsToGet 
                                                                     * 2 * sizeof(Int16));
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQCGRAB_2132_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQCGRAB_2132_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqCGrabCmdInput_2132*  pInData = NULL;
    DtIoctlIqCGrabCmdOutput_2132*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQCGRAB_2132));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_IQCGRAB_CMD_2132);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQCGRAB_2132, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqCGrabCmd_2132;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqCGrabCmd_2132;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQCGRAB_CMD_2132_GET_IQ_POINTS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQCGRAB_2132_OnCmdGetIqConstPoints(IQCGRAB_2132_STUB,
                                       &pInData->m_GetIqPoints, &pOutData->m_GetIqPoints);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQCGRAB_2132_OnCmdGetIqConstPoints -.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcIQCGRAB_2132_OnCmdGetIqConstPoints(
    const DtIoStubBcIQCGRAB_2132* pStub,
    const DtIoctlIqCGrabCmd_2132GetIqPointsInput* pInData,
    DtIoctlIqCGrabCmd_2132GetIqPointsOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQCGRAB_2132));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    return DtBcIQCGRAB_2132_GetIqConstPoints(IQCGRAB_2132_BC, pInData->m_StreamId,
            pInData->m_NumIqPointsToGet, pOutData->m_pIqPoints, &pOutData->m_NumIqPoints);

}
