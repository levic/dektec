//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcLEDB.c *#*#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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
#include "DtBcLEDB.h"
#include "DtBcLEDB_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLEDB implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_LEDB_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcLEDB))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_LEDB_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(LEDB, pBc)

// Helper macro to cast a DtBc* to a DtBcLEDB*
#define BC_LEDB         ((DtBcLEDB*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcLEDB_Init(DtBc*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLEDB - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLEDB_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcLEDB_Close(DtBc*  pBc)
{
     BC_LEDB_DEFAULT_PRECONDITIONS(pBc);
        
    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLEDB_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcLEDB*  DtBcLEDB_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_LEDB_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcLEDB, Id, DT_BLOCK_TYPE_LEDB, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcLEDB_Close;
    OpenParams.m_InitFunc = DtBcLEDB_Init;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcLEDB*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLEDB_GetLedControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcLEDB_GetLedControl(DtBcLEDB* pBc, Bool* pOverrule, Bool* pGreen, Bool* pRed, 
                                                                              Bool* pBlue)
{
    // Sanity check
    BC_LEDB_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOverrule==NULL || pGreen==NULL|| pRed==NULL|| pBlue==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return cached configuration
    *pOverrule = pBc->m_Overrule;
    *pGreen = pBc->m_GreenOn;
    *pRed = pBc->m_RedOn;
    *pBlue = pBc->m_BlueOn;
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLEDB_SetLedControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcLEDB_SetLedControl(DtBcLEDB* pBc, Bool Overrule, Bool Green, Bool Red,
                                                                                Bool Blue)
{
    UInt32  RegData;

    // Sanity check
    BC_LEDB_DEFAULT_PRECONDITIONS(pBc);

    // Set new LED control
    RegData = LEDB_Control_READ(pBc);
    RegData = LEDB_Control_SET_Overrule( RegData, Overrule ? 1 : 0);
    RegData = LEDB_Control_SET_LedGreen( RegData, Green ? 1 : 0);
    RegData = LEDB_Control_SET_LedRed( RegData, Red ? 1 : 0);
    RegData = LEDB_Control_SET_LedBlue( RegData, Blue ? 1 : 0);
    LEDB_Control_WRITE(pBc, RegData);

    // Save new settings
    pBc->m_Overrule = Overrule;
    pBc->m_GreenOn = Green;
    pBc->m_RedOn = Red;
    pBc->m_BlueOn = Blue;
    return DT_STATUS_OK;
}


// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLEDB - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLEDB_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcLEDB_Init(DtBc*  pBc)
{
    // Sanity checks
    BC_LEDB_DEFAULT_PRECONDITIONS(pBc);

    // Set default LEDs
    return DtBcLEDB_SetLedControl(BC_LEDB, FALSE, FALSE, FALSE, FALSE);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLEDB implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcLEDB
#define LEDB_STUB   ((DtIoStubBcLEDB*)pStub)
#define LEDB_BC  ((DtBcLEDB*)LEDB_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcLEDB_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcLEDB_OnCmdGetLedControl(const DtIoStubBcLEDB*, 
                                                      DtIoctlLedBCmdGetLedControlOutput*);
static DtStatus  DtIoStubBcLEDB_OnCmdSetLedControl(const DtIoStubBcLEDB*, 
                                                 const DtIoctlLedBCmdSetLedControlInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_LEDB;

static const DtIoctlProperties  IOSTUB_BC_LEDB_IOCTLS[] = 
{
    DT_IOCTL_PROPS_LEDB_CMD(
        DtIoStubBcLEDB_OnCmd,
        NULL,  NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLEDB - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLEDB_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcLEDB_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcLEDB));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLEDB_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcLEDB*  DtIoStubBcLEDB_Open(DtBc*  pBc)
{
    DtIoStubBcLEDB*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcLEDB));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcLEDB, pBc, NULL, 
                                                             DtIoStubBcLEDB_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_LEDB_IOCTLS);
    pStub = (DtIoStubBcLEDB*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLEDB- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLEDB_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcLEDB_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlLedBCmdInput*  pInData = NULL;
    DtIoctlLedBCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcLEDB));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_LEDB_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, LEDB, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_LedBCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_LedBCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_LEDB_CMD_GET_LED_CONTROL:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcLEDB_OnCmdGetLedControl(LEDB_STUB, &pOutData->m_GetLedControl);
        break;
    case DT_LEDB_CMD_SET_LED_CONTROL:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcLEDB_OnCmdSetLedControl(LEDB_STUB, &pInData->m_SetLedControl);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLEDB_OnCmdGetLedControl -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcLEDB_OnCmdGetLedControl(
    const DtIoStubBcLEDB* pStub,
    DtIoctlLedBCmdGetLedControlOutput* pOutData)
{
    Bool Overrule, Green, Red, Blue;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcLEDB));
    DT_ASSERT(pOutData != NULL);

    DT_RETURN_ON_ERROR(DtBcLEDB_GetLedControl(LEDB_BC, &Overrule, &Green, &Red, &Blue));
    pOutData->m_Overrule = Overrule;
    pOutData->m_GreenOn = Green;
    pOutData->m_RedOn = Red;
    pOutData->m_BlueOn = Blue;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLEDB_OnCmdSetLedControl -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcLEDB_OnCmdSetLedControl(
    const DtIoStubBcLEDB* pStub,
    const DtIoctlLedBCmdSetLedControlInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcLEDB));
    DT_ASSERT(pInData != NULL);

    return DtBcLEDB_SetLedControl(LEDB_BC, pInData->m_Overrule==(Int)TRUE,
                                           pInData->m_GreenOn==(Int)TRUE,
                                           pInData->m_RedOn==(Int)TRUE,
                                           pInData->m_BlueOn==(Int)TRUE);
}
