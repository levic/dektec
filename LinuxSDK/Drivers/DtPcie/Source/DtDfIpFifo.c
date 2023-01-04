// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfIpFifo.c *#*#*#*#*#*#*#*# (C) 2018-2021 DekTec
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
#include "DtDfIpFifo.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
#define BC_IPFIFO_ROLE_NONE      NULL


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfIpFifo implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_IPFIFO_DEFAULT_PRECONDITIONS(pDf)      \
                                 DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfIpFifo))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_IPFIFO_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(IPFIFO, pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfIpFifo_Init(DtDf*);

static DtStatus  DtDfIpFifo_LoadParameters(DtDf*);
static DtStatus  DtDfIpFifo_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfIpFifo_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfIpFifo_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfIpFifo_OpenChildren(DtDfIpFifo*);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfIpFifo - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfIpFifo_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfIpFifo_Close(DtDf*  pDf)
{
    DF_IPFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfIpFifo_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfIpFifo*  DtDfIpFifo_Open(DtCore* pCore, DtPt* pPt, const char* pRole, Int Instance, 
                                                                                 Int Uuid)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_IPFIFO_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfIpFifo, Id, DT_FUNC_TYPE_IPFIFO, pPt, FALSE, 
                                                                                   pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfIpFifo_Close;
    OpenParams.m_InitFunc = DtDfIpFifo_Init;
    OpenParams.m_LoadParsFunc = DtDfIpFifo_LoadParameters;
    OpenParams.m_OnCloseFileFunc = DtDfIpFifo_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfIpFifo_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfIpFifo_OnEnablePreChildren;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfIpFifo*)DtDf_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfIpFifo_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfIpFifo_GetOperationalMode(DtDfIpFifo* pDf, Int* pOpMode)
{
    // Sanity check
    DF_IPFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_IPFIFO_MUST_BE_ENABLED(pDf);

    // Return cached operational mode
    *pOpMode = pDf->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfIpFifo_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfIpFifo_SetOperationalMode(DtDfIpFifo* pDf, Int OpMode)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_IPFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (OpMode!=DT_FUNC_OPMODE_IDLE && OpMode!=DT_FUNC_OPMODE_STANDBY
                                                            && OpMode!=DT_FUNC_OPMODE_RUN)
    {
        DtDbgOutDf(ERR, IPFIFO, pDf, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    DF_IPFIFO_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_OperationalMode == OpMode)
        return DT_STATUS_OK;
    if (pDf->m_pBcIpFifo != NULL)
        Status = DtBcIPFIFO_SetOperationalMode(pDf->m_pBcIpFifo, OpMode);

    // Save new operational mode
    if (DT_SUCCESS(Status))
        pDf->m_OperationalMode = OpMode;

    return Status;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfIpFifo - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfIpFifo_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfIpFifo_Init(DtDf* pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfIpFifo*  pDf = (DtDfIpFifo*)pDfBase;
    
    // Sanity checks
    DF_IPFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_OperationalMode = DT_FUNC_OPMODE_IDLE;

    //Open children
    Status = DtDfIpFifo_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, IPFIFO, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    Status = DtBcIPFIFO_SetFifoAddr(pDf->m_pBcIpFifo, pDf->m_FifoAddr);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, IPFIFO, pDf, "ERROR: failed to set FifoAddr");
        return DT_STATUS_FAIL;
    }
    Status = DtBcIPFIFO_SetFifoSize(pDf->m_pBcIpFifo, pDf->m_FifoSize);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, IPFIFO, pDf, "ERROR: failed to set FifoSize");
        return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfIpFifo_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Note: LoadParameters() is called before the Init(). The loaded parameters can be used
// in the Init().
//
DtStatus  DtDfIpFifo_LoadParameters(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfIpFifo* pDf = (DtDfIpFifo*)pDfBase;

    // List of IpFifo function parameters
    DtDfParameters  DFIPFIFO_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "FIFO_ADDR", PROPERTY_VALUE_TYPE_UINT32, &pDf->m_FifoAddr },
        { "FIFO_SIZE", PROPERTY_VALUE_TYPE_UINT32, &pDf->m_FifoSize },
    };

    // Sanity checks
    DF_IPFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_FifoAddr = -1;
    pDf->m_FifoSize = -1;
    
    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFIPFIFO_PARS), DFIPFIFO_PARS);
    if (!DT_SUCCESS(Status))
        return Status;

    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfIpFifo_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfIpFifo_OnEnablePostChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfIpFifo* pDf = (DtDfIpFifo*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
     
    // Sanity check
    DF_IPFIFO_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutDf(AVG, IPFIFO, pDf, "DISABLE -> ENABLE");

        // Operational mode must be IDLE
        DT_ASSERT(pDf->m_OperationalMode == DT_FUNC_OPMODE_IDLE);

        // Set operational mode of BLKCTRL to IDLE
        DT_RETURN_ON_ERROR(DtBcIPFIFO_SetOperationalMode(pDf->m_pBcIpFifo,
                                                                   DT_BLOCK_OPMODE_IDLE));
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfIpFifo_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfIpFifo_OnEnablePreChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfIpFifo* pDf = (DtDfIpFifo*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
    // Sanity check
    DF_IPFIFO_DEFAULT_PRECONDITIONS(pDf);
    if (!Enable)
    {
        // ENABLE -> DISABLE
        DtDbgOutDf(AVG, IPFIFO, pDf, "ENABLE -> DISABLE");

        // Return to IDLE
        DT_RETURN_ON_ERROR(DtDfIpFifo_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE));
    }

    return Status;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfIpFifo_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtDfIpFifo_OnCloseFile(DtDf* pDfBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfIpFifo* pDf = (DtDfIpFifo*)pDfBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_IPFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Check if the owner closed the file handle
    Status = DtDf_ExclAccessCheck((DtDf*)pDf, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutDf(AVG, IPFIFO, pDf, "Go to IDLE");

        // Go to IDLE
        Status = DtDfIpFifo_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, IPFIFO, pDf, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtDf_OnCloseFile((DtDf*)pDf, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfIpFifo_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfIpFifo_OpenChildren(DtDfIpFifo*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the the IPFIFO function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_IPFIFO, DT_BC_IPFIFO_NAME,
                        BC_IPFIFO_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcIpFifo), TRUE},
    };

    DF_IPFIFO_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // BcIpfifo should be present
    DT_ASSERT(pDf->m_pBcIpFifo != NULL);

    return DT_STATUS_OK;
}
