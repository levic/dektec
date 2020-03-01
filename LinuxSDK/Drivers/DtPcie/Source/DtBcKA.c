//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcKA.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
//
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
#include "DtBcKA.h"
#include "DtBcKA_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcKA implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_KA_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcKA))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_KA_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(KA, pBc)

// Helper macro to cast a DtBc* to a DtBcKA*
#define BC_KA         ((DtBcKA*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcKA_Init(DtBc*);
static void  DtBcKA_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);
static DtStatus  DtBcKA_SetKaConfig(DtBcKA*, Bool AutoKeepAlive, Bool FailSafeEnable,
                                                                             Int Timeout);
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcKA - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcKA_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcKA_Close(DtBc*  pBc)
{
     BC_KA_DEFAULT_PRECONDITIONS(pBc);

    //Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pBc->m_pCore, (DtObject*)pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcKA_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcKA*  DtBcKA_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_KA_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcKA, Id, DT_BLOCK_TYPE_KA, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcKA_Close;
    OpenParams.m_InitFunc = DtBcKA_Init;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcKA*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcKA_GetFailSafeInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcKA_GetFailSafeInfo(DtBcKA* pBc, Bool* pFailSafeEnable, Int* pTimeout,
                                                                             Bool* pAlive)
{
    // Sanity check
    BC_KA_DEFAULT_PRECONDITIONS(pBc);

    // Must be eneabled
    BC_KA_MUST_BE_ENABLED(pBc);

    // Check parameters
    if (pFailSafeEnable==NULL || pTimeout==NULL || pAlive==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Get keep-alive information
    DtSpinLockAcquire(&pBc->m_KaLock);
    *pFailSafeEnable = pBc->m_FailSafeEnabled;
    *pTimeout = pBc->m_WatchDogTimeout;
    *pAlive = ((pBc->m_AliveCnt < pBc->m_AliveTimeoutCnt) || pBc->m_AutoKeepAlive);
    DtSpinLockRelease(&pBc->m_KaLock);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcKA_Pulse -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcKA_Pulse(DtBcKA * pBc)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_KA_DEFAULT_PRECONDITIONS(pBc);

    // Must be eneabled
    BC_KA_MUST_BE_ENABLED(pBc);

    DtSpinLockAcquire(&pBc->m_KaLock);
    // Keep-alive not used?
    if (pBc->m_AutoKeepAlive)
        Status = DT_STATUS_CONFIG_ERROR;
    else
        // Reset alive count
        pBc->m_AliveCnt = 0;
    DtSpinLockRelease(&pBc->m_KaLock);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcKA_SetAutoKeepAlive -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcKA_SetAutoKeepAlive(DtBcKA* pBc, Bool AutoKeepAlive, Int Timeout)
{
    // Sanity check
    BC_KA_DEFAULT_PRECONDITIONS(pBc);

    // Must be eneabled
    BC_KA_MUST_BE_ENABLED(pBc);
    
    return DtBcKA_SetKaConfig(pBc, AutoKeepAlive, pBc->m_FailSafeEnabled, Timeout);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcKA_SetFailSafeConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcKA_SetFailSafeConfig(DtBcKA* pBc, Bool FailSafeEnable, Int Timeout)
{
    // Sanity check
    BC_KA_DEFAULT_PRECONDITIONS(pBc);
    
    // Must be eneabled
    BC_KA_MUST_BE_ENABLED(pBc);
    
    return DtBcKA_SetKaConfig(pBc, pBc->m_AutoKeepAlive, FailSafeEnable, Timeout);
}


// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcKA - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcKA_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcKA_Init(DtBc*  pBc)
{
    DtOnPeriodicIntervalRegData  RegData;
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    BC_KA_DEFAULT_PRECONDITIONS(pBc);

    
    // Get TOD period
    Status = DtCore_TOD_GetPeriodicItv(pBc->m_pCore, &BC_KA->m_TodPeriod);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, KA, pBc, "ERROR: DtCore_TOD_GetPeriodicItv failed");
        return Status;
    }

    // Initialize spinlock
    DtSpinLockInit(&BC_KA->m_KaLock);
    
    // Set defaults
    BC_KA->m_AutoKeepAlive = FALSE;
    BC_KA->m_FailSafeEnabled = FALSE;
    BC_KA->m_WatchDogTimeout = 200;       // 200 ms timeout
    BC_KA->m_AliveCnt = 0;
    BC_KA->m_AliveTimeoutCnt = (BC_KA->m_WatchDogTimeout + BC_KA->m_TodPeriod-1)
                                                                      /BC_KA->m_TodPeriod;

      // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtBcKA_PeriodicIntervalHandler;
    RegData.m_pObject = (DtObject*)pBc;
    Status = DtCore_TOD_PeriodicItvRegister(pBc->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    { 
        DtDbgOutBc(ERR, KA, pBc, "DtCore_TOD_PeriodicItvRegister failed");
        return Status;
    }
    
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcKA_PeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcKA_PeriodicIntervalHandler(DtObject* pObj, DtTodTime Time)
{
    DtBcKA*  pBc = (DtBcKA*)pObj;
    UInt32  RegControl;

    // Sanity checks
    BC_KA_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    if (!DtBc_IsEnabled((DtBc*)pBc))
        return;
    DtSpinLockAcquireAtDpc(&pBc->m_KaLock);
    if (!pBc->m_AutoKeepAlive)
    {
        // Check if failsafe is enabled and application is still alive
        if (pBc->m_FailSafeEnabled && pBc->m_AliveCnt<pBc->m_AliveTimeoutCnt)
        {
            pBc->m_AliveCnt++;
            RegControl = KA_Control_READ(pBc);
            RegControl = KA_Control_SET_Keepalive(RegControl, 1);
            KA_Control_WRITE(pBc,RegControl);
        }
        // else: keep-alive disabled or application did not respond within time 
    }
    else
    {
        // Automatic keep-alive, pulse always
        RegControl = KA_Control_READ(pBc);
        RegControl = KA_Control_SET_Keepalive(RegControl, 1);
        KA_Control_WRITE(pBc, RegControl);
    }
    DtSpinLockReleaseFromDpc(&pBc->m_KaLock);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcKA_SetKaConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcKA_SetKaConfig(DtBcKA* pBc, Bool AutoKeepAlive, Bool FailSafeEnable,
                                                                              Int Timeout)
{
    Int TimeOutCnt;

    // Check paramters
    if (Timeout < 0)
        return DT_STATUS_INVALID_PARAMETER;

    DtSpinLockAcquire(&pBc->m_KaLock);

    // No change?
    if (pBc->m_AutoKeepAlive==AutoKeepAlive && pBc->m_FailSafeEnabled==FailSafeEnable
                                       && (pBc->m_WatchDogTimeout==Timeout || Timeout==0))
    { 
        DtSpinLockRelease(&pBc->m_KaLock);
        return DT_STATUS_OK;
    }
    
    // Ignore Timeout?
    if (Timeout == 0)
    { 
        // Use current timeout settings
        Timeout = pBc->m_WatchDogTimeout;
        TimeOutCnt = pBc->m_AliveTimeoutCnt;
    }
    else
    {
        // Compute after how many TOD-periods the timeout is reached
        TimeOutCnt = (Timeout + pBc->m_TodPeriod-1) / pBc->m_TodPeriod;
        if (TimeOutCnt <= 0)
        { 
            DtSpinLockRelease(&pBc->m_KaLock);
            return DT_STATUS_INVALID_PARAMETER;
        }
    }

    // Enable/disable keep-alive and set new timeout count

    pBc->m_AutoKeepAlive = AutoKeepAlive;
    pBc->m_FailSafeEnabled = FailSafeEnable;
    pBc->m_WatchDogTimeout = Timeout;
    pBc->m_AliveCnt = 0;
    pBc->m_AliveTimeoutCnt = TimeOutCnt;
    DtSpinLockRelease(&pBc->m_KaLock);

    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcKA implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcKA
#define KA_STUB   ((DtIoStubBcKA*)pStub)
#define KA_BC  ((DtBcKA*)KA_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcKA_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcKA_OnCmdGetFailSafeInfo(const DtIoStubBcKA*, 
                                                      DtIoctlKaCmdGetFailSafeInfoOutput*);
static DtStatus  DtIoStubBcKA_OnCmdPulse(const DtIoStubBcKA*);
static DtStatus  DtIoStubBcKA_OnCmdSetFailSafeConfig(const DtIoStubBcKA*, 
                                               const DtIoctlKaCmdSetFailSafeConfigInput*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_KA;

static const DtIoctlProperties  IOSTUB_BC_KA_IOCTLS[] = 
{
    DT_IOCTL_PROPS_KA_CMD(
        DtIoStubBcKA_OnCmd,
        NULL,  NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcKA - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcKA_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcKA_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcKA));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcKA_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcKA*  DtIoStubBcKA_Open(DtBc*  pBc)
{
    DtIoStubBcKA*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcKA));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcKA, pBc, NULL, 
                                                             DtIoStubBcKA_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_KA_IOCTLS);
    pStub = (DtIoStubBcKA*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcKA- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcKA_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcKA_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlKaCmdInput*  pInData = NULL;
    DtIoctlKaCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcKA));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_KA_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, KA, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_KaCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_KaCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-
    switch (pIoParams->m_Cmd)
    {
    case DT_KA_CMD_GET_FAILSAFE_INFO:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcKA_OnCmdGetFailSafeInfo(KA_STUB, &pOutData->m_GetInfo);
        break;
    case DT_KA_CMD_PULSE:
        Status = DtIoStubBcKA_OnCmdPulse(KA_STUB);
        break;
    case DT_KA_CMD_SET_FAILSAFE_CONFIG:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcKA_OnCmdSetFailSafeConfig(KA_STUB, &pInData->m_SetConfig);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcKA_OnCmdGetFailSafeInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcKA_OnCmdGetFailSafeInfo(
    const DtIoStubBcKA* pStub,
    DtIoctlKaCmdGetFailSafeInfoOutput* pOutData)
{
    Bool Enabled, Alive;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcKA));
    DT_ASSERT(pOutData != NULL);

    DT_RETURN_ON_ERROR(DtBcKA_GetFailSafeInfo(KA_BC, &Enabled, &pOutData->m_Timeout,
                                                                                 &Alive));
    pOutData->m_Enabled  = (Int)Enabled;
    pOutData->m_Alive  = (Int)Alive;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcKA_OnCmdPulse -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcKA_OnCmdPulse(const DtIoStubBcKA*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcKA));

    return DtBcKA_Pulse(KA_BC);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcKA_OnCmdSetFailSafeConfig -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcKA_OnCmdSetFailSafeConfig(
    const DtIoStubBcKA*  pStub,
    const DtIoctlKaCmdSetFailSafeConfigInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcKA));
    DT_ASSERT(pInData != NULL);

    return DtBcKA_SetFailSafeConfig(KA_BC, pInData->m_Enable==(Int)TRUE,
                                                                      pInData->m_Timeout);
}