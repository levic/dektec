//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcACCUFIFO.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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
#include "DtBcACCUFIFO.h"
#include "DtBcACCUFIFO_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcACCUFIFO implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_ACCUFIFO_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcACCUFIFO))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_ACCUFIFO_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(ACCUFIFO, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcACCUFIFO_Init(DtBc*);
static DtStatus  DtBcACCUFIFO_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcACCUFIFO_OnEnable(DtBc*, Bool  Enable);
static void  DtBcACCUFIFO_SetControlRegs(DtBcACCUFIFO*, Bool BlkEna, Int OpMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcACCUFIFO - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcACCUFIFO_Close(DtBc*  pBc)
{
    BC_ACCUFIFO_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcACCUFIFO*  DtBcACCUFIFO_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_ACCUFIFO_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcACCUFIFO, Id, DT_BLOCK_TYPE_ACCUFIFO,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcACCUFIFO_Close;
    OpenParams.m_InitFunc = DtBcACCUFIFO_Init;
    OpenParams.m_OnEnableFunc = DtBcACCUFIFO_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcACCUFIFO_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcACCUFIFO*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_GetAccuPeriod -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcACCUFIFO_GetAccuPeriod(DtBcACCUFIFO * pBc, Int * pPeriodNs)
{
    // Sanity check
    BC_ACCUFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pPeriodNs == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ACCUFIFO_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pPeriodNs = (Int)(((Int64)pBc->m_AccuPeriodNumClks * 1000LL*1000*1000) /
                                                                   pBc->m_ClockFrequency);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_GetMaxNumMeasurements -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcACCUFIFO_GetMaxNumMeasurements(DtBcACCUFIFO* pBc, Int* pMaxNumMeasm)
{
    // Sanity check
    BC_ACCUFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pMaxNumMeasm == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ACCUFIFO_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pMaxNumMeasm = pBc->m_MaxNumMeasurements;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_GetNumMeasurements -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcACCUFIFO_GetNumMeasurements(DtBcACCUFIFO* pBc, Int* pNumMeasm)
{
    // Sanity check
    BC_ACCUFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pNumMeasm == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ACCUFIFO_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pNumMeasm = pBc->m_NumMeasurements;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_GetMeasurementStatus -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcACCUFIFO_GetMeasurementStatus(DtBcACCUFIFO* pBc, Int* pNumAva, Bool* pDone)
{
    UInt32  RegStatus;
    // Sanity check
    BC_ACCUFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pNumAva==NULL || pDone==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ACCUFIFO_MUST_BE_ENABLED(pBc);

    // Read status
    RegStatus = ACCUFIFO_Status_READ(pBc);
    *pNumAva = (Int)ACCUFIFO_Status_GET_FifoLoad(RegStatus);
    *pDone = ACCUFIFO_Status_GET_MeasurementDone(RegStatus) != 0;
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_ReadMeasurements -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcACCUFIFO_ReadMeasurements(DtBcACCUFIFO* pBc, Int* pMeasmBuf, 
                                                             Int NumToRead, Int* pNumRead)
{
    Int  Load, i, NumToDo;
    // Sanity check
    BC_ACCUFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pMeasmBuf ==NULL || pNumRead == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    if (NumToRead<0 || NumToRead>pBc->m_NumMeasurements)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ACCUFIFO_MUST_BE_ENABLED(pBc);

    // Get Load and determine num to read
    Load = (Int)ACCUFIFO_Status_READ_FifoLoad(pBc);
    NumToDo = (Load<NumToRead) ? Load : NumToRead;
    *pNumRead = NumToDo;
    
    // Read measurments
    for (i=0; i<NumToDo; i++)
        (*pMeasmBuf++) = (Int)ACCUFIFO_Measurement_READ(pBc);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_RestartMeasurement -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcACCUFIFO_RestartMeasurement(DtBcACCUFIFO * pBc)
{
    // Sanity check
    BC_ACCUFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_ACCUFIFO_MUST_BE_ENABLED(pBc);

    // Set control register
    DtBcACCUFIFO_SetControlRegs(pBc, TRUE, pBc->m_AccuPeriodNumClks);

    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_SetAccuPeriod -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcACCUFIFO_SetAccuPeriod(DtBcACCUFIFO* pBc, Int PeriodNs)
{
    Int  NumClks;

    // Sanity check
    BC_ACCUFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters (rough)
    if (PeriodNs<1000 || PeriodNs>1000*1000*1000)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ACCUFIFO_MUST_BE_ENABLED(pBc);

    // Compute number of clock frequency ticks
    NumClks = (Int)((pBc->m_ClockFrequency * PeriodNs) / (1000LL*1000*1000));

    // No change?
    if (pBc->m_AccuPeriodNumClks == NumClks)
        return DT_STATUS_OK;
    
    // Check result
    if (NumClks >= (1 << 16))
        return DT_STATUS_INVALID_PARAMETER;

    // Cache value
    pBc->m_AccuPeriodNumClks = NumClks;

    // Set control register
    DtBcACCUFIFO_SetControlRegs(pBc, FALSE, pBc->m_AccuPeriodNumClks);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_SetNumMeasurements -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcACCUFIFO_SetNumMeasurements(DtBcACCUFIFO* pBc, Int NumMeasm)
{
    // Sanity check
    BC_ACCUFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (NumMeasm <=0 || NumMeasm >pBc->m_MaxNumMeasurements)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ACCUFIFO_MUST_BE_ENABLED(pBc);

    // Cache value
    pBc->m_NumMeasurements = NumMeasm;
    
    // Update register
    ACCUFIFO_NumMeasurements_WRITE(pBc, (UInt32)NumMeasm);

    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcACCUFIFO - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcACCUFIFO_Init(DtBc* pBcBase)
{
    DtBcACCUFIFO* pBc = (DtBcACCUFIFO*)pBcBase;

    // Read Configuration into Cache
    pBc->m_ClockFrequency = 125000000; // TODOTD (Int64)ACCUFIFO_Config2_READ_ClkFreq(pBc);
    pBc->m_MaxNumMeasurements = (Int)ACCUFIFO_Config1_READ_FifoDepth(pBc);
    
    // Set defaults
    pBc->m_NumMeasurements = pBc->m_MaxNumMeasurements;
    pBc->m_AccuPeriodNumClks = (Int)((pBc->m_ClockFrequency*5000) / (1000LL*1000*1000));

    // Set control registers
    DtBcACCUFIFO_SetControlRegs(pBc, FALSE, pBc->m_AccuPeriodNumClks);
    ACCUFIFO_NumMeasurements_WRITE(pBc, (UInt32)pBc->m_NumMeasurements);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcACCUFIFO_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;

    // No action needed

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcACCUFIFO_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcACCUFIFO_SetControlRegs(DtBcACCUFIFO* pBc, Bool Restart, Int PeriodClks)
{
    UInt32  RegData = 0;
    // Update control register
    RegData = ACCUFIFO_Control1_READ(pBc);
    RegData = ACCUFIFO_Control1_SET_Start(RegData, Restart?1:0);
    RegData = ACCUFIFO_Control1_SET_AccumulationPeriod(RegData, (UInt32)PeriodClks);
    ACCUFIFO_Control1_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcACCUFIFO implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define ACCUFIFO_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcACCUFIFO))
// Helper macro to cast stub's DtBc member to DtBcACCUFIFO
#define ACCUFIFO_STUB   ((DtIoStubBcACCUFIFO*)pStub)
#define ACCUFIFO_BC  ((DtBcACCUFIFO*)ACCUFIFO_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcACCUFIFO_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubBcACCUFIFO_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcACCUFIFO_OnCmdGetAccuPeriod(
                                                  const DtIoStubBcACCUFIFO*, 
                                                  DtIoctlAccuFifoCmdGetAccuPeriodOutput*);
static DtStatus  DtIoStubBcACCUFIFO_OnCmdGetMaxNumMeasurements(
                                                 const DtIoStubBcACCUFIFO*,
                                                 DtIoctlAccuFifoCmdGetMaxNumMeasmOutput*); 
static DtStatus  DtIoStubBcACCUFIFO_OnCmdGetMeasurementStatus(
                                                 const DtIoStubBcACCUFIFO*,
                                                 DtIoctlAccuFifoCmdGetMeasmStatusOutput*); 
static DtStatus  DtIoStubBcACCUFIFO_OnCmdGetNumMeasurements(
                                                    const DtIoStubBcACCUFIFO*,
                                                    DtIoctlAccuFifoCmdGetNumMeasmOutput*);
static DtStatus  DtIoStubBcACCUFIFO_OnCmdReadMeasurements(
                                                 const DtIoStubBcACCUFIFO*,
                                                 const DtIoctlAccuFifoCmdReadMeasmsInput*, 
                                                 DtIoctlAccuFifoCmdReadMeasmsOutput*);
static DtStatus  DtIoStubBcACCUFIFO_OnCmdRestartMeasurement(const DtIoStubBcACCUFIFO*);
static DtStatus  DtIoStubBcACCUFIFO_OnCmdSetAccuPeriod(
                                             const DtIoStubBcACCUFIFO*,
                                             const DtIoctlAccuFifoCmdSetAccuPeriodInput*);
static DtStatus  DtIoStubBcACCUFIFO_OnCmdSetNumMeasurements(
                                               const DtIoStubBcACCUFIFO*,
                                               const DtIoctlAccuFifoCmdSetNumMeasmInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_ACCUFIFO;

static const DtIoctlProperties  IOSTUB_BC_ACCUFIFO_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_ACCUFIFO_CMD(
        DtIoStubBcACCUFIFO_OnCmd,
        DtIoStubBcACCUFIFO_AppendDynamicSize,
        NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcACCUFIFO - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcACCUFIFO_Close(DtIoStub* pStub)
{
    ACCUFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcACCUFIFO*  DtIoStubBcACCUFIFO_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_ACCUFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcACCUFIFO, pBc, NULL, 
                                                            DtIoStubBcACCUFIFO_Close,
                                                            NULL,  // Use default IOCTL
                                                            IOSTUB_BC_ACCUFIFO_IOCTLS);
    return (DtIoStubBcACCUFIFO*)DtIoStubBc_Open(&OpenParams);
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcACCUFIFO - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcACCUFIFO_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlAccuFifoCmdInput*  pInData = NULL;

    ACCUFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_ACCUFIFO_CMD);
    
    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_AccuFifoCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_ACCUFIFO_CMD_READ_MEASMS:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlAccuFifoCmdReadMeasmsInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlAccuFifoCmdReadMeasmsOutput));
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += pInData->m_ReadMeasms.m_NumToRead*sizeof(Int);
        break;
    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcACCUFIFO_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlAccuFifoCmdInput*  pInData = NULL;
    DtIoctlAccuFifoCmdOutput*  pOutData = NULL;

    ACCUFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_ACCUFIFO_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, ACCUFIFO, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_AccuFifoCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_AccuFifoCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_ACCUFIFO_CMD_GET_ACCU_PERIOD:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcACCUFIFO_OnCmdGetAccuPeriod(ACCUFIFO_STUB,
                                                              &pOutData->m_GetAccuPeriod);
        break;
    case DT_ACCUFIFO_CMD_GET_MAX_NUM_MEASM:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcACCUFIFO_OnCmdGetMaxNumMeasurements(ACCUFIFO_STUB,
                                                             &pOutData->m_GetMaxNumMeasm);
        break;
    case DT_ACCUFIFO_CMD_GET_MEASM_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcACCUFIFO_OnCmdGetMeasurementStatus(ACCUFIFO_STUB,
                                                             &pOutData->m_GetMeasmStatus);
        break;
    case DT_ACCUFIFO_CMD_GET_NUM_MEASM:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcACCUFIFO_OnCmdGetNumMeasurements(ACCUFIFO_STUB,
                                                                &pOutData->m_GetNumMeasm);
        break;
    case DT_ACCUFIFO_CMD_READ_MEASMS:
        DT_ASSERT(pInData != NULL && pOutData != NULL);
        Status = DtIoStubBcACCUFIFO_OnCmdReadMeasurements(ACCUFIFO_STUB,
                                         &pInData->m_ReadMeasms, &pOutData->m_ReadMeasms);
        break;
    case DT_ACCUFIFO_CMD_RESTART_MEASM:
        Status = DtIoStubBcACCUFIFO_OnCmdRestartMeasurement(ACCUFIFO_STUB);
        break;
    case DT_ACCUFIFO_CMD_SET_ACCU_PERIOD:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcACCUFIFO_OnCmdSetAccuPeriod(ACCUFIFO_STUB,
                                                               &pInData->m_SetAccuPeriod);
        break;
    case DT_ACCUFIFO_CMD_SET_NUM_MEASM:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcACCUFIFO_OnCmdSetNumMeasurements(ACCUFIFO_STUB,
                                                                 &pInData->m_SetNumMeasm);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO_OnCmdGetAccuPeriod -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcACCUFIFO_OnCmdGetAccuPeriod(
    const DtIoStubBcACCUFIFO*  pStub,
    DtIoctlAccuFifoCmdGetAccuPeriodOutput*  pOutData)
{
    ACCUFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtBcACCUFIFO_GetAccuPeriod(ACCUFIFO_BC, &pOutData->m_AccuPeriod);
}

// .-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO_OnCmdGetMaxNumMeasurements -.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcACCUFIFO_OnCmdGetMaxNumMeasurements(
    const DtIoStubBcACCUFIFO*  pStub,
    DtIoctlAccuFifoCmdGetMaxNumMeasmOutput*  pOutData)
{
    ACCUFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtBcACCUFIFO_GetMaxNumMeasurements(ACCUFIFO_BC, &pOutData->m_MaxNumMeasm);
}

DtStatus DtIoStubBcACCUFIFO_OnCmdGetMeasurementStatus(
    const DtIoStubBcACCUFIFO*  pStub,
    DtIoctlAccuFifoCmdGetMeasmStatusOutput*  pOutData)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool DoneFlag = FALSE;

    ACCUFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    Status = DtBcACCUFIFO_GetMeasurementStatus(ACCUFIFO_BC, &pOutData->m_NumAvailable,
                                                                               &DoneFlag);
    pOutData->m_MeasmDone = (Int)DoneFlag;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO_OnCmdGetNumMeasurements -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcACCUFIFO_OnCmdGetNumMeasurements(
    const DtIoStubBcACCUFIFO*  pStub,
    DtIoctlAccuFifoCmdGetNumMeasmOutput*  pOutData)
{
    ACCUFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtBcACCUFIFO_GetNumMeasurements(ACCUFIFO_BC, &pOutData->m_NumMeasm);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO_OnCmdReadMeasurements -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcACCUFIFO_OnCmdReadMeasurements(
    const DtIoStubBcACCUFIFO*  pStub,
    const DtIoctlAccuFifoCmdReadMeasmsInput*  pInData,
    DtIoctlAccuFifoCmdReadMeasmsOutput*  pOutData)
{
    ACCUFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);
    return DtBcACCUFIFO_ReadMeasurements(ACCUFIFO_BC, pOutData->m_Buf,
                                              pInData->m_NumToRead, &pOutData->m_NumRead);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO_OnCmdRestartMeasurement -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcACCUFIFO_OnCmdRestartMeasurement(const DtIoStubBcACCUFIFO* pStub)
{
    ACCUFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    return DtBcACCUFIFO_RestartMeasurement(ACCUFIFO_BC);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO_OnCmdSetAccuPeriod -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcACCUFIFO_OnCmdSetAccuPeriod(
    const DtIoStubBcACCUFIFO*  pStub,
    const DtIoctlAccuFifoCmdSetAccuPeriodInput*  pInData)
{
    ACCUFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    return DtBcACCUFIFO_SetAccuPeriod(ACCUFIFO_BC, pInData->m_AccuPeriod);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO_OnCmdSetNumMeasurements -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcACCUFIFO_OnCmdSetNumMeasurements(
    const DtIoStubBcACCUFIFO*  pStub,
    const DtIoctlAccuFifoCmdSetNumMeasmInput*  pInData)
{
    ACCUFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    return DtBcACCUFIFO_SetNumMeasurements(ACCUFIFO_BC, pInData->m_NumMeasm);
}