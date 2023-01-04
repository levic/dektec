// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfDataFifo.c *#*#*#*#*#*#*#*# (C) 2018-2021 DekTec
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
#include "DtDfDataFifo.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
#define BC_DATAFIFO_ROLE_NONE      NULL


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfDataFifo implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf)      \
                                 DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfDataFifo))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_DATAFIFO_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(DATAFIFO, pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfDataFifo_Init(DtDf*);
static DtStatus  DtDfDataFifo_ClearFifoMax(DtDfDataFifo*, Bool, Bool);
static DtStatus  DtDfDataFifo_GetDirection(DtDfDataFifo*, Int* );
static DtStatus  DtDfDataFifo_GetFifoStatus(DtDfDataFifo*, Int*, Int*, Int*);
static DtStatus  DtDfDataFifo_GetOverflowCount(DtDfDataFifo*, UInt32* );
static DtStatus  DtDfDataFifo_GetUnderflowCount(DtDfDataFifo*, UInt32* );
static DtStatus  DtDfDataFifo_GetProperties(DtDfDataFifo*, UInt32*, Int*, Int*, Int*, 
                                                                                    Int*);
static DtStatus  DtDfDataFifo_InitController(DtDfDataFifo* pDf);
static DtStatus  DtDfDataFifo_SetDirection(DtDfDataFifo*, Int);
static DtStatus  DtDfDataFifo_LoadParameters(DtDf*);
static DtStatus  DtDfDataFifo_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfDataFifo_OnEnablePostChildren(DtDf*, Bool);
static DtStatus  DtDfDataFifo_OnEnablePreChildren(DtDf*, Bool);
static DtStatus  DtDfDataFifo_OpenChildren(DtDfDataFifo*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfDataFifo - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfDataFifo_Close(DtDf*  pDf)
{
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfDataFifo*  DtDfDataFifo_Open(DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                  Int Instance, Int Uuid, Bool CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_DATAFIFO_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfDataFifo, Id, DT_FUNC_TYPE_DATAFIFO, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfDataFifo_Close;
    OpenParams.m_InitFunc = DtDfDataFifo_Init;
    OpenParams.m_LoadParsFunc = DtDfDataFifo_LoadParameters;
    OpenParams.m_OnCloseFileFunc = DtDfDataFifo_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfDataFifo_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfDataFifo_OnEnablePreChildren;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfDataFifo*)DtDf_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfDataFifo_GetOperationalMode(DtDfDataFifo* pDf, Int* pOpMode)
{
    // Sanity check
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_DATAFIFO_MUST_BE_ENABLED(pDf);

    // Return cached operational mode
    *pOpMode = pDf->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfDataFifo_SetOperationalMode(DtDfDataFifo* pDf, Int OpMode)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (OpMode!=DT_FUNC_OPMODE_IDLE && OpMode!=DT_FUNC_OPMODE_STANDBY
                                                            && OpMode!=DT_FUNC_OPMODE_RUN)
    {
        DtDbgOutDf(ERR, DATAFIFO, pDf, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    DF_DATAFIFO_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_OperationalMode == OpMode)
        return DT_STATUS_OK;
    if (pDf->m_pBcDataFifo != NULL)
        Status = DtBcDATAFIFO_SetOperationalMode(pDf->m_pBcDataFifo, OpMode);

    // Save new operational mode
    if (DT_SUCCESS(Status))
        pDf->m_OperationalMode = OpMode;

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_ClearFifoMax -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfDataFifo_ClearFifoMax(DtDfDataFifo* pDf, Bool ClrMaxFree, Bool ClrMaxLoad)
{
    // Sanity check
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_DATAFIFO_MUST_BE_ENABLED(pDf);

    return DtBcDATAFIFO_ClearFifoMax(pDf->m_pBcDataFifo, ClrMaxFree, ClrMaxLoad);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_GetDirection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfDataFifo_GetDirection(DtDfDataFifo* pDf, Int* pDirection)
{
    // Sanity check
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pDirection == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_DATAFIFO_MUST_BE_ENABLED(pDf);

    return DtBcDATAFIFO_GetDirection(pDf->m_pBcDataFifo, pDirection);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_GetFifoStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfDataFifo_GetFifoStatus(DtDfDataFifo* pDf, Int* pCurLoad, Int* pMaxFree, 
                                                                            Int* pMaxLoad)
{
    // Sanity check
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pCurLoad == NULL || pMaxFree ==NULL || pMaxLoad==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_DATAFIFO_MUST_BE_ENABLED(pDf);

    return DtBcDATAFIFO_GetFifoStatus(pDf->m_pBcDataFifo, pCurLoad, pMaxFree, pMaxLoad);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_GetOverflowCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfDataFifo_GetOverflowCount(DtDfDataFifo* pDf, UInt32* pOvfCount)
{
    // Sanity check
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pOvfCount == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_DATAFIFO_MUST_BE_ENABLED(pDf);

    DT_RETURN_ON_ERROR(DtBcDATAFIFO_GetOverflowCount(pDf->m_pBcDataFifo, pOvfCount));
    
    *pOvfCount *= pDf->m_NumBytesPerDataWord; // to #bytes
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_GetOverflowCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfDataFifo_GetUnderflowCount(DtDfDataFifo* pDf, UInt32* pUflCount)
{

    // Sanity check
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pUflCount == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_DATAFIFO_MUST_BE_ENABLED(pDf);

    DT_RETURN_ON_ERROR(DtBcDATAFIFO_GetUnderflowCount(pDf->m_pBcDataFifo, pUflCount));

    *pUflCount *= pDf->m_NumBytesPerDataWord; // to #bytes
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfDataFifo_GetProperties(DtDfDataFifo* pDf, UInt32* pCapabilities, 
                      Int *pFifoSize, Int* pBurstSize, Int* pMaxFifoSize, Int *pDataWidth)
{
    // Sanity check
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pCapabilities == NULL || pBurstSize == NULL || pMaxFifoSize == NULL ||
                                                  pFifoSize == NULL || pDataWidth == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_DATAFIFO_MUST_BE_ENABLED(pDf);

    *pCapabilities = pDf->m_Capabilities;
    *pBurstSize = pDf->m_BurstSize;
    *pMaxFifoSize = pDf->m_MaxFifoSize;
    *pFifoSize = pDf->m_FifoSize;
    *pDataWidth = pDf->m_DataWidth;
    return DT_STATUS_OK;
}

DtStatus DtDfDataFifo_SetDirection(DtDfDataFifo* pDf, Int Direction)
{
    // Sanity check
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_DATAFIFO_MUST_BE_ENABLED(pDf);

    return DtBcDATAFIFO_SetDirection(pDf->m_pBcDataFifo, Direction);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfDataFifo - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfDataFifo_Init(DtDf* pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfDataFifo*  pDf = (DtDfDataFifo*)pDfBase;

    // Sanity checks
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_OperationalMode = DT_FUNC_OPMODE_IDLE;

    //Open children
    Status = DtDfDataFifo_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, DATAFIFO, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Note: LoadParameters() is called before the Init(). The loaded parameters can be used
// in the Init().
//
DtStatus  DtDfDataFifo_LoadParameters(DtDf* pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfDataFifo* pDf = (DtDfDataFifo*)pDfBase;

    // List of DataFifo function parameters
    DtDfParameters  DFDATAFIFO_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "FIFO_ADDR", PROPERTY_VALUE_TYPE_UINT32, &pDf->m_FifoAddr },
        { "FIFO_SIZE", PROPERTY_VALUE_TYPE_UINT32, &pDf->m_FifoSize },   // #bytes
    };

    // Sanity checks
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_FifoAddr = 0;
    pDf->m_FifoSize = 0;

    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFDATAFIFO_PARS), 
                                                                         DFDATAFIFO_PARS);
    if (!DT_SUCCESS(Status))
        return Status;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_InitController -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfDataFifo_InitController(DtDfDataFifo* pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  BurstSize = 0;
    UInt32  MaxFifoSize = 0;
    UInt32  DataWidth = 0;

    Status = DtBcDATAFIFO_GetProperties(pDf->m_pBcDataFifo, &pDf->m_Capabilities,
                                                    &BurstSize, &MaxFifoSize, &DataWidth);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, DATAFIFO, pDf, "ERROR: failed to get properties");
        return DT_STATUS_FAIL;
    }
    pDf->m_DataWidth = DataWidth;
    pDf->m_NumBytesPerDataWord = DataWidth/8; // into bytes
    pDf->m_BurstSize = ((UInt32)1<<BurstSize) * pDf->m_NumBytesPerDataWord; // #bytes
    pDf->m_MaxFifoSize = ((UInt32)1<<MaxFifoSize) * pDf->m_BurstSize;       // #bytes

    // Set Fifo address (#bytes) as loaded from XML-properties
    Status = DtBcDATAFIFO_SetFifoAddr(pDf->m_pBcDataFifo, pDf->m_FifoAddr);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, DATAFIFO, pDf, "ERROR: failed to set FifoAddr");
        return DT_STATUS_FAIL;
    }

    // set fifo size(bytes) as loaded from XML-properties
    Status = DtBcDATAFIFO_SetFifoSize(pDf->m_pBcDataFifo, pDf->m_FifoSize);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, DATAFIFO, pDf, "ERROR: failed to set FifoSize");
        return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfDataFifo_OnEnablePostChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfDataFifo* pDf = (DtDfDataFifo*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
     
    // Sanity check
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutDf(AVG, DATAFIFO, pDf, "DISABLE -> ENABLE");

        // Operational mode must be IDLE
        DT_ASSERT(pDf->m_OperationalMode == DT_FUNC_OPMODE_IDLE);

        // Set operational mode of BLKCTRL to IDLE
        DT_RETURN_ON_ERROR(DtBcDATAFIFO_SetOperationalMode(pDf->m_pBcDataFifo,
                                                                   DT_BLOCK_OPMODE_IDLE));
        DT_RETURN_ON_ERROR(DtDfDataFifo_InitController(pDf));
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfDataFifo_OnEnablePreChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfDataFifo* pDf = (DtDfDataFifo*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
    // Sanity check
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);
    if (!Enable)
    {
        // ENABLE -> DISABLE
        DtDbgOutDf(AVG, DATAFIFO, pDf, "ENABLE -> DISABLE");

        // Return to IDLE
        DT_RETURN_ON_ERROR(DtDfDataFifo_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE));
    }
    return Status;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtDfDataFifo_OnCloseFile(DtDf* pDfBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfDataFifo* pDf = (DtDfDataFifo*)pDfBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Check if the owner closed the file handle
    Status = DtDf_ExclAccessCheck((DtDf*)pDf, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutDf(AVG, DATAFIFO, pDf, "Go to IDLE");

        // Go to IDLE
        Status = DtDfDataFifo_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, DATAFIFO, pDf, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtDf_OnCloseFile((DtDf*)pDf, pFile);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfDataFifo_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfDataFifo_OpenChildren(DtDfDataFifo*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the the DATAFIFO function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_DATAFIFO, DT_BC_DATAFIFO_NAME,
                    BC_DATAFIFO_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcDataFifo), TRUE},
    };

    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // BcDataFifo should be present
    DT_ASSERT(pDf->m_pBcDataFifo != NULL);

    // Find device level DDR memory controller (mandatory)
    pDf->m_pBcDdrFront = (DtBcDDRFRONT*)DtCore_BC_Find(pDf->m_pCore, NULL, 
                                                              DT_BLOCK_TYPE_DDRFRONT, NULL);

    // DDRFRONT should be present
    DT_ASSERT(pDf->m_pBcDdrFront != NULL);

    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfDataFifo implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Df's stub
#define DATAFIFO_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                      DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfDataFifo));
// Helper macro to cast stub's DtDf member to DtDfDataFifo
#define STUB_DATAFIFO   ((DtIoStubDfDataFifo*)pStub)
#define STUB_DF  ((DtDfDataFifo*)STUB_DATAFIFO->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtIoStubDfDataFifo_OnCmdClearFifoMax(const DtIoStubDfDataFifo*, 
                                             const DtIoctlDataFifoCmdClearFifoMaxInput*);
static DtStatus  DtIoStubDfDataFifo_OnCmdGetDirection(const DtIoStubDfDataFifo* ,
                                                  DtIoctlDataFifoCmdGetDirectionOutput*);
static DtStatus  DtIoStubDfDataFifo_OnCmdGetFifoStatus(const DtIoStubDfDataFifo* ,
                                                DtIoctlDataFifoCmdGetFifoStatusOutput* );
static DtStatus  DtIoStubDfDataFifo_OnCmdGetOverflowCount(const DtIoStubDfDataFifo* ,
                                                   DtIoctlDataFifoCmdGetOvfCountOutput* );
static DtStatus  DtIoStubDfDataFifo_OnCmdGetUnderflowCount(const DtIoStubDfDataFifo* ,
                                                   DtIoctlDataFifoCmdGetUflCountOutput* );
static DtStatus DtIoStubDfDataFifo_OnCmdGetProperties(const DtIoStubDfDataFifo* ,
                                                DtIoctlDataFifoCmdGetPropertiesOutput* );
static DtStatus DtIoStubDfDataFifo_OnCmdSetDirection(const DtIoStubDfDataFifo* ,
                                            const DtIoctlDataFifoCmdSetDirectionInput* );
static DtStatus DtIoStubDfDataFifo_OnCmd(const DtIoStub* , DtIoStubIoParams* , Int * );
static DtStatus  DtIoStubDfDataFifo_OnCmdGetOperationalMode(const DtIoStubDfDataFifo*,
                                                      DtIoctlDataFifoCmdGetOpModeOutput*);
static DtStatus  DtIoStubDfDataFifo_OnCmdSetOperationalMode(const DtIoStubDfDataFifo*,
                                                 const DtIoctlDataFifoCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_DATAFIFO;

static const DtIoctlProperties  IOSTUB_DF_DATAFIFO_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubDf_OnExclAccessCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_DATAFIFO_CMD(
        DtIoStubDfDataFifo_OnCmd, 
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfDataFifo - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfDataFifo_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubDfDataFifo_Close(DtIoStub* pStub)
{
    DATAFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfDataFifo_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubDfDataFifo*  DtIoStubDfDataFifo_Open(DtDf*  pDf)
{
    DtIoStubDfDataFifo*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_DATAFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfDataFifo, pDf, NULL, 
                                                             DtIoStubDfDataFifo_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_DF_DATAFIFO_IOCTLS);
    pStub = (DtIoStubDfDataFifo*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfDataFifo - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfDataFifo_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfDataFifo_OnCmd(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlDataFifoCmdInput*  pInData = NULL;
    DtIoctlDataFifoCmdOutput*  pOutData = NULL;
    
    DATAFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_DATAFIFO_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, DATAFIFO, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_DataFifoCmd;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_DataFifoCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_DATAFIFO_CMD_CLEAR_FIFO_MAX:
        Status = DtIoStubDfDataFifo_OnCmdClearFifoMax(STUB_DATAFIFO, 
                                                                &pInData->m_ClearFifoMax);
        break;
    case DT_DATAFIFO_CMD_GET_DIRECTION:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfDataFifo_OnCmdGetDirection(STUB_DATAFIFO, 
                                                               &pOutData->m_GetDirection);
        break;
    case DT_DATAFIFO_CMD_GET_FIFO_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfDataFifo_OnCmdGetFifoStatus(STUB_DATAFIFO,
                                                              &pOutData->m_GetFifoStatus);
        break;
    case DT_DATAFIFO_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfDataFifo_OnCmdGetOperationalMode(STUB_DATAFIFO, 
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_DATAFIFO_CMD_GET_OVFL_COUNT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfDataFifo_OnCmdGetOverflowCount(STUB_DATAFIFO,
                                                                  &pOutData->m_GetOvfCnt);
        break;
    case DT_DATAFIFO_CMD_GET_UFL_COUNT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfDataFifo_OnCmdGetUnderflowCount(STUB_DATAFIFO,
                                                                  &pOutData->m_GetUflCnt);
        break;
    case DT_DATAFIFO_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfDataFifo_OnCmdGetProperties(STUB_DATAFIFO,
                                                              &pOutData->m_GetProperties);
        break;
    case DT_DATAFIFO_CMD_SET_DIRECTION:
        Status = DtIoStubDfDataFifo_OnCmdSetDirection(STUB_DATAFIFO, 
                                                                &pInData->m_SetDirection);
        break;
    case DT_DATAFIFO_CMD_SET_OPERATIONAL_MODE:
        Status = DtIoStubDfDataFifo_OnCmdSetOperationalMode(STUB_DATAFIFO, 
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfDataFifo_OnCmdClearFifoMax -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfDataFifo_OnCmdClearFifoMax(
    const DtIoStubDfDataFifo* pStub,
    const DtIoctlDataFifoCmdClearFifoMaxInput* pInData)
{
    DATAFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtDfDataFifo_ClearFifoMax(STUB_DF, pInData->m_ClearMaxFree != 0,
        pInData->m_ClearMaxLoad != 0);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfDataFifo_OnCmdGetDirection -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfDataFifo_OnCmdGetDirection(
    const DtIoStubDfDataFifo* pStub,
    DtIoctlDataFifoCmdGetDirectionOutput* pOutData)
{
    DATAFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtDfDataFifo_GetDirection(STUB_DF, &pOutData->m_Direction);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfDataFifo_OnCmdGetFifoStatus -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfDataFifo_OnCmdGetFifoStatus(
    const DtIoStubDfDataFifo* pStub,
    DtIoctlDataFifoCmdGetFifoStatusOutput* pOutData)
{
    DATAFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtDfDataFifo_GetFifoStatus(STUB_DF, &pOutData->m_CurLoad, &pOutData->m_MaxFree, 
                                                                    &pOutData->m_MaxLoad);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfDataFifo_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfDataFifo_OnCmdGetOperationalMode(
    const DtIoStubDfDataFifo*  pStub, 
    DtIoctlDataFifoCmdGetOpModeOutput*  pOutData)
{
    DATAFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtDfDataFifo_GetOperationalMode(STUB_DF, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.- DtIoStubDfDataFifo_OnCmdGetOverUnderflowCount -.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfDataFifo_OnCmdGetOverflowCount(
    const DtIoStubDfDataFifo* pStub,
    DtIoctlDataFifoCmdGetOvfCountOutput* pOutData)
{
    DATAFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtDfDataFifo_GetOverflowCount(STUB_DF, &pOutData->m_OvfCount);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubDfDataFifo_OnCmdGetUnderflowCount -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfDataFifo_OnCmdGetUnderflowCount(
    const DtIoStubDfDataFifo* pStub,
    DtIoctlDataFifoCmdGetUflCountOutput* pOutData)
{
    DATAFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtDfDataFifo_GetUnderflowCount(STUB_DF, &pOutData->m_UflCount);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfDataFifo_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfDataFifo_OnCmdGetProperties(
    const DtIoStubDfDataFifo* pStub,
    DtIoctlDataFifoCmdGetPropertiesOutput* pOutData)
{
    DATAFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtDfDataFifo_GetProperties(STUB_DF, &pOutData->m_Capabilities, 
                                        &pOutData->m_FifoSize, &pOutData->m_BurstSize,
                                        &pOutData->m_MaxFifoSize, &pOutData->m_DataWidth);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfDataFifo_OnCmdSetDirection -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfDataFifo_OnCmdSetDirection(
    const DtIoStubDfDataFifo* pStub,
    const DtIoctlDataFifoCmdSetDirectionInput* pInData)
{
    DATAFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtDfDataFifo_SetDirection(STUB_DF, pInData->m_Direction);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfDataFifo_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfDataFifo_OnCmdSetOperationalMode(
    const DtIoStubDfDataFifo*  pStub, 
    const DtIoctlDataFifoCmdSetOpModeInput*  pInData)
{
    DATAFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtDfDataFifo_SetOperationalMode(STUB_DF, pInData->m_OpMode);
}
