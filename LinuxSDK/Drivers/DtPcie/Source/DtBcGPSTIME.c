//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcGPSTIME.c *#*#*#*#*#*#*#*#*#*# (C) 2021 DekTec
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
#include "DtBcGPSTIME.h"
#include "DtBcGPSTIME_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcGPSTIME implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_GPSTIME_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcGPSTIME))

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcGPSTIME_Init(DtBc*);
static DtStatus  DtBcGPSTIME_OnCloseFile(DtBc*, const DtFileObject*);
DtStatus DtBcGPSTIME_GetStatus(DtBcGPSTIME *pBc,Int* pOnePpsErrorCount, 
                 Bool* pOnePpsValid, Int* pOnePpsTrailingTime, Int* p10MhzPeriodCount);
DtStatus DtBcGPSTIME_GetTime(DtBcGPSTIME *pBc, Int* pTime);
DtStatus DtBcGPSTIME_GetHoldOff(DtBcGPSTIME *pBc, Bool* pHoldOff);
DtStatus DtBcGPSTIME_SetHoldOff(DtBcGPSTIME *pBc, Bool HoldOff);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcGPSTIME - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPSTIME_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcGPSTIME_Close(DtBc* pBc)
{
    BC_GPSTIME_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPSTIME_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcGPSTIME*  DtBcGPSTIME_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_GPSTIME_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcGPSTIME, Id, DT_BLOCK_TYPE_GPSTIME,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcGPSTIME_Close;
    OpenParams.m_InitFunc = DtBcGPSTIME_Init;
    OpenParams.m_OnCloseFileFunc = DtBcGPSTIME_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcGPSTIME*)DtBc_Open(&OpenParams);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcGPSTIME - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPSTIME_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcGPSTIME_Init(DtBc* pBcBase)
{
    DtBcGPSTIME* pBc = (DtBcGPSTIME*)pBcBase;
    UInt32 RegData=0;

    // Sanity checks
    BC_GPSTIME_DEFAULT_PRECONDITIONS(pBc);

    // Set defaults
    pBc->m_OnePpsHoldOff = TRUE;
    GPSTIME_ControlOnePps_SET_HoldOffOnePps(RegData, 1);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPSTIME_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcGPSTIME_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPSTIME_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcGPSTIME_GetStatus(DtBcGPSTIME *pBc,
    Int* pOnePpsErrorCount, 
    Bool* pOnePpsValid, 
    Int* pOnePpsTrailingTime, 
    Int* p10MhzPeriodCount)
{
    UInt32  RegValue;

    // Sanity check
    BC_GPSTIME_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOnePpsErrorCount==NULL || pOnePpsValid==NULL || pOnePpsTrailingTime==NULL || 
                                                                  p10MhzPeriodCount==NULL)
        return DT_STATUS_INVALID_PARAMETER;

     RegValue = GPSTIME_StatusOnePps_READ(pBc);
    *pOnePpsErrorCount = (Int)GPSTIME_StatusOnePps_GET_OnePpsErrCnt(RegValue);
    *pOnePpsValid = (GPSTIME_StatusOnePps_GET_OnePpsValid(RegValue) != 0);
    *pOnePpsTrailingTime = (Int)GPSTIME_StatusOnePps_GET_OnePpsTrailingTime(RegValue);
    *p10MhzPeriodCount = (Int)GPSTIME_StatusTenMHz_READ_TenMHzPeriodCnt(pBc);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPSTIME_GetTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcGPSTIME_GetTime(DtBcGPSTIME *pBc, Int* pTime)
{

    // Sanity check
    BC_GPSTIME_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pTime == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    *pTime = (Int)GPSTIME_Time_READ(pBc);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPSTIME_GetHoldOff -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcGPSTIME_GetHoldOff(DtBcGPSTIME *pBc, Bool* pHoldOff)
{
    // Sanity check
    BC_GPSTIME_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pHoldOff == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return cached status
    *pHoldOff = pBc->m_OnePpsHoldOff;
    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPSTIME_SetHoldOff -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcGPSTIME_SetHoldOff(DtBcGPSTIME *pBc, Bool HoldOff)
{
    UInt32  RegData =0;
    
    // Sanity check
    BC_GPSTIME_DEFAULT_PRECONDITIONS(pBc);

    pBc->m_OnePpsHoldOff = HoldOff;
    RegData = GPSTIME_ControlOnePps_SET_HoldOffOnePps(RegData, HoldOff ? 1 : 0);
    GPSTIME_ControlOnePps_WRITE(pBc, RegData);
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcGPSTIME implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define GPSTIME_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcGPSTIME))
// Helper macro to cast stub's DtBc member to DtBcGPSTIME
#define GPSTIME_STUB   ((DtIoStubBcGPSTIME*)pStub)
#define GPSTIME_BC  ((DtBcGPSTIME*)GPSTIME_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcGPSTIME_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcGPSTIME_OnCmdGetStatus(const DtIoStubBcGPSTIME*,
                                                      DtIoctlGpsTimeCmdGetStatusOutput* );
static DtStatus  DtIoStubBcGPSTIME_OnCmdGetTime(const DtIoStubBcGPSTIME*, Int *);
static DtStatus  DtIoStubBcGPSTIME_OnCmdGetHoldOff(const DtIoStubBcGPSTIME*, Int *);
static DtStatus  DtIoStubBcGPSTIME_OnCmdSetHoldOff(const DtIoStubBcGPSTIME*, Int);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_GPSTIME;

static const DtIoctlProperties  IOSTUB_BC_GPSTIME_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_GPSTIME_CMD(
        DtIoStubBcGPSTIME_OnCmd,
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcGPSTIME - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPSTIME_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcGPSTIME_Close(DtIoStub* pStub)
{
    GPSTIME_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPSTIME_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcGPSTIME*  DtIoStubBcGPSTIME_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_GPSTIME_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcGPSTIME, pBc, NULL, 
                                                            DtIoStubBcGPSTIME_Close,
                                                            NULL,  // Use default IOCTL
                                                            IOSTUB_BC_GPSTIME_IOCTLS);
    return (DtIoStubBcGPSTIME*)DtIoStubBc_Open(&OpenParams);
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcGPSTIME - Private functions +=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPSTIME_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcGPSTIME_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlGpsTimeCmdInput*  pInData = NULL;
    DtIoctlGpsTimeCmdOutput*  pOutData = NULL;

    GPSTIME_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_GPSTIME_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, GPSTIME, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_GpsTimeCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_GpsTimeCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-
    switch (pIoParams->m_Cmd)
    {
    case DT_GPSTIME_CMD_GET_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcGPSTIME_OnCmdGetStatus(GPSTIME_STUB, &pOutData->m_GetStatus);
        break;
    case DT_GPSTIME_CMD_GET_TIME:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcGPSTIME_OnCmdGetTime(GPSTIME_STUB, &pOutData->m_GetTime.m_Time);
        break;
    case DT_GPSTIME_CMD_GET_HOLDOFF:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcGPSTIME_OnCmdGetHoldOff(GPSTIME_STUB, 
                                                       &pOutData->m_GetHoldOff.m_HoldOff);
        break;
    case DT_GPSTIME_CMD_SET_HOLDOFF:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcGPSTIME_OnCmdSetHoldOff(GPSTIME_STUB, 
                                                         pInData->m_SetHoldOff.m_HoldOff);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPSTIME_OnCmdGetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcGPSTIME_OnCmdGetStatus(
    const DtIoStubBcGPSTIME* pStub,
    DtIoctlGpsTimeCmdGetStatusOutput* pOutData)
{
    DtStatus Status = DT_STATUS_OK;
    Bool OnePpsValid;

    DT_ASSERT(pStub != NULL && pStub->m_Size == sizeof(DtIoStubBcGPSTIME));
    DT_ASSERT(pOutData != NULL);


    Status = DtBcGPSTIME_GetStatus(GPSTIME_BC, &pOutData->m_OnePpsErrorCount,
                                               &OnePpsValid,
                                               &pOutData->m_OnePpsTrailingTime,
                                               &pOutData->m_10MhzPeriodCount);
    pOutData->m_OnePpsValid = (Int)OnePpsValid;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPSTIME_OnCmdGetTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtIoStubBcGPSTIME_OnCmdGetTime(
    const DtIoStubBcGPSTIME* pStub, 
    Int *pTime)
{
    GPSTIME_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pTime != NULL);

    return DtBcGPSTIME_GetTime(GPSTIME_BC, pTime);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPSTIME_OnCmdGetHoldOff -.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtIoStubBcGPSTIME_OnCmdGetHoldOff(
    const DtIoStubBcGPSTIME* pStub, Int * pHoldOff)
{
    DtStatus Status = DT_STATUS_OK;
    Bool HoldOff = FALSE;

    GPSTIME_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pHoldOff != NULL);

    Status = DtBcGPSTIME_GetHoldOff(GPSTIME_BC, &HoldOff);
    *pHoldOff = HoldOff ? TRUE : FALSE;
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPSTIME_OnCmdSetHoldOff -.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtIoStubBcGPSTIME_OnCmdSetHoldOff(
    const DtIoStubBcGPSTIME* pStub, 
    Int HoldOff)
{
    GPSTIME_STUB_DEFAULT_PRECONDITIONS(pStub);
    return DtBcGPSTIME_SetHoldOff(GPSTIME_BC, (HoldOff == 1));
}