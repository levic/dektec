// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfChSdiRxPhyOnly.c *#*#*#*#*#*#*#*#* (C) 2022 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2022 DekTec Digital Video B.V.
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
#include "DtDfChSdiRxPhyOnly.h"

// Steno notation for DT_RETURN_ON_ERROR
#define RET_ON_ERR(Expr) DT_RETURN_ON_ERROR(Expr)

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfChSdiRxPhyOnly implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// MACRO with default precondition checks for the DtDfChSdiRxPhyOnly function
#define DF_CH_DEFAULT_PRECONDITIONS(pDf)        \
            DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfChSdiRxPhyOnly))

// MACRO that checks the function has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_CH_MUST_BE_ENABLED(pDf)              \
            DF_MUST_BE_ENABLED_IMPL(CHSDIRXPHYONLY, pDf)

// Helper macro to cast a DtDf* to a DtDfChSdiRxPhyOnly*
#define DF_TO_THIS      ((DtDfChSdiRxPhyOnly*)pDf)

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static const int StateLockTimeout = 500; // Timeout for acquiring the user lock (in ms)
static const int MaxNumUsersSupported = 8;  // Max of 8 users may share this channel

// .-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly - Forwards of private functions -.-.-.-.-.-.-.-.-
static DtStatus DtDfChSdiRxPhyOnly_Attach(DtDfChSdiRxPhyOnly*, const DtFileObject*, 
                                      const DtExclAccessObject*, 
                                      Bool ReqExclusiveAccess, const char* pFriendlyName);
static void DtDfChSdiRxPhyOnly_CloseUser(DtDfCh*, DtDfChUser*, const DtFileObject*);
static DtStatus DtDfChSdiRxPhyOnly_Configure(
                                    DtDfChSdiRxPhyOnly*, const DtDfChSdiRxPhyOnlyConfig*);
static DtStatus DtDfChSdiRxPhyOnly_ConfigureUser(DtDfChSdiRxPhyOnly*,
                                    const DtFileObject*, const DtDfChSdiRxPhyOnlyConfig*);
static DtStatus DtDfChSdiRxPhyOnly_Detach(
                   DtDfChSdiRxPhyOnly*, const DtExclAccessObject*, const DtFileObject*);
static DtStatus DtDfChSdiRxPhyOnly_DoIdleToStandy(DtDfCh*);
static DtStatus DtDfChSdiRxPhyOnly_DoRunToStandby(DtDfCh*);
static DtStatus DtDfChSdiRxPhyOnly_DoStandbyToIdle(DtDfCh*);
static DtStatus DtDfChSdiRxPhyOnly_DoStandbyToRun(DtDfCh*);
static DtFuncOpMode DtDfChSdiRxPhyOnly_FindHighestOpModeRequested(
                                                               const DtDfChSdiRxPhyOnly*);
static DtDfChSdiRxPhyOnlyUser* DtDfChSdiRxPhyOnly_FindUser(
                                                DtDfChSdiRxPhyOnly*, const DtFileObject*);
static DtStatus DtDfChSdiRxPhyOnly_GetSdiStatus(DtDfChSdiRxPhyOnly*, 
                                 Int* pCarrierDetect, Int* pSdiLock, Int* pLineLock,
                                 Int* pValid, Int* pSdiRate, Int* pNumSymsHanc, 
                                 Int* pNumSymsVidVanc, Int* pNumLinesF1, Int* pNumLinesF2,
                                 Int* pIsLevelB, UInt32* pPayloadId, Int* pFramePeriod);
static DtStatus DtDfChSdiRxPhyOnly_GetUserOperationalMode(DtDfChSdiRxPhyOnly*, 
                                                      const DtFileObject*, DtFuncOpMode*);
static Bool DtDfChSdiRxPhyOnly_IsConfigCompatible(
                              const DtDfChSdiRxPhyOnly*, const DtDfChSdiRxPhyOnlyConfig*);
static DtStatus DtDfChSdiRxPhyOnly_OpenChildren(DtDfCh*);
static DtDfChUser* DtDfChSdiRxPhyOnly_OpenUser(
                                         DtDfCh*, const DtFileObject*, Bool, const char*);
static DtStatus DtDfChSdiRxPhyOnly_SetOperationalMode(DtDfChSdiRxPhyOnly*, DtFuncOpMode);
static DtStatus DtDfChSdiRxPhyOnly_SetUserOperationalMode(DtDfChSdiRxPhyOnly*, 
                                                       const DtFileObject*, DtFuncOpMode);
static DtStatus DtDfChSdiRxPhyOnly_Unconfigure(DtDfChSdiRxPhyOnly*);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+ DtDfChSdiRxPhyOnly - Public functions +=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfChSdiRxPhyOnly_Close(DtDf* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
   
    // Let base function perform final clean-up
    DtDfCh_Close((DtDfCh*)pDf);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_Lock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Acquires the channel state lock
//
DtStatus DtDfChSdiRxPhyOnly_Lock(DtDfChSdiRxPhyOnly* pDf, int Timeout)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    // Let base implementation do the heavy lifting
    return DtDfCh_Lock((DtDfCh*)pDf, Timeout);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfChSdiRxPhyOnly* DtDfChSdiRxPhyOnly_Open(DtCore* pCore, DtPt* pPt, 
                                                          const char* pRole, Int Instance,
                                                          Int Uuid, Bool CreateStub)
{
    DtDfId Id;
    DtDfChOpenParams OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_CHSDIRXPHYONLY_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_CH_INIT_OPEN_PARAMS(OpenParams, DtDfChSdiRxPhyOnly, Id, 
                                   DT_FUNC_TYPE_CHSDIRXPHYONLY, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_DfParams.m_CloseFunc = DtDfChSdiRxPhyOnly_Close;
    OpenParams.m_DfParams.m_OnCloseFileFunc = DtDfCh_OnCloseFile;   // Use DtDfCh default

    // Channel specific callbacks
    OpenParams.m_CloseUserFunc = DtDfChSdiRxPhyOnly_CloseUser;
    OpenParams.m_DoIdleToStandyFunc = DtDfChSdiRxPhyOnly_DoIdleToStandy;
    OpenParams.m_DoRunToStandbyFunc = DtDfChSdiRxPhyOnly_DoRunToStandby;
    OpenParams.m_DoStandbyToIdleFunc = DtDfChSdiRxPhyOnly_DoStandbyToIdle;
    OpenParams.m_DoStandbyToRunFunc = DtDfChSdiRxPhyOnly_DoStandbyToRun;
    OpenParams.m_InitFunc = NULL;  // Base initialization will do
    OpenParams.m_OpenUserFunc = DtDfChSdiRxPhyOnly_OpenUser;
    OpenParams.m_OpenChildrenFunc = DtDfChSdiRxPhyOnly_OpenChildren;
    OpenParams.m_OnDetachLastUserFunc = NULL;

    // Set maximum simultaneous users
    OpenParams.m_MaxNumUsersSupported = MaxNumUsersSupported;

    // Use base function to allocate and perform standard initialization of function data
    return (DtDfChSdiRxPhyOnly*)DtDfCh_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_Lock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Releases the channel state lock.
//
void DtDfChSdiRxPhyOnly_Unlock(DtDfChSdiRxPhyOnly* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DtMutexRelease(&pDf->m_StateLock);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+ DtDfChSdiRxPhyOnly - Private functions +=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_Attach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfChSdiRxPhyOnly_Attach(
    DtDfChSdiRxPhyOnly* pDf, 
    const DtFileObject* pFile,
    const DtExclAccessObject* pObject, 
    Bool ReqExclusiveAccess, 
    const char* pFriendlyName)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Let base implementation do heavy lifting
    return DtDfCh_Attach((DtDfCh*)pDf, pFile, pObject, ReqExclusiveAccess, pFriendlyName);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_CloseUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Closes a user object but first releases any resources it may hold.
// 
// Note: caller should hold the state lock.
//
void DtDfChSdiRxPhyOnly_CloseUser(
                                DtDfCh* pDf, DtDfChUser* pUser, const DtFileObject* pFile)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Let base implementation do the heavy lifting
    DtDfCh_CloseUser(pDf, pUser, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_Configure -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Configures the channel. Typically, called when the first user sets a configuration.
//
// Note: caller should hold the state lock
//
DtStatus DtDfChSdiRxPhyOnly_Configure(
                            DtDfChSdiRxPhyOnly* pDf, const DtDfChSdiRxPhyOnlyConfig* pCfg)
{
    DtStatus Status = DT_STATUS_OK;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must have been enabled
    DF_CH_MUST_BE_ENABLED(pDf);

    if (!pCfg)
        return DT_STATUS_INVALID_PARAMETER;

    DtDbgOutDf(MAX, CHSDIRXPHYONLY, pDf, "Applying channel configuration:");
    DtDbgOutDf(MAX, CHSDIRXPHYONLY, pDf, "- SDI rate=%d", pCfg->m_SdiRate);
    // Cannot configure twice
    if (pDf->m_IsConfigured)
    {
        DtDbgOutDf(ERR, CHSDIRXPHYONLY, pDf, "ERROR: channel is already configured");
        return DT_STATUS_CONFIG_ERROR;
    }

    // Configure SDIRX function
    Status = DtDfSdiRx_SetSdiRate(pDf->m_pDfSdiRx, pCfg->m_SdiRate);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRXPHYONLY, pDf, "ERROR: failed set SDI rate for SDIRX");
        return Status;
    }

    // Copy configuration
    pDf->m_Config = *pCfg;
    pDf->m_IsConfigured = TRUE;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_ConfigureUser -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Applies the configuration for a specific user. Function will fail if the configuration
// is not compatible with the active configuration. If no configuration was applied yet 
// this user's configuration will become the active configuration.
//
// Note: caller should hold the state lock
//
DtStatus DtDfChSdiRxPhyOnly_ConfigureUser(
    DtDfChSdiRxPhyOnly* pDf,
    const DtFileObject* pFile, 
    const DtDfChSdiRxPhyOnlyConfig* pCfg)
{
    DtStatus Status=DT_STATUS_OK;
    DtDfChSdiRxPhyOnlyUser* pUser=NULL;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pFile != NULL);
    DT_ASSERT(pCfg != NULL);

    // Must have been enabled
    DF_CH_MUST_BE_ENABLED(pDf);

    // Find the user and return the user operational mode
    pUser = DtDfChSdiRxPhyOnly_FindUser(pDf, pFile);
    if (pUser == NULL)
    {
        DtDbgOutDf(ERR,CHSDIRXPHYONLY,pDf, "ERROR: could not find a user for '%p'",pFile);
        return DT_STATUS_NOT_FOUND;
    }

    DtDbgOutDf(AVG, CHSDIRXPHYONLY, pDf, "User '%s' is configuring the channel", 
                                                                   pUser->m_FriendlyName);

    // User must be IDLE
    if (pUser->m_OpMode != DT_FUNC_OPMODE_IDLE)
    {
        DtDbgOutDf(ERR, CHSDIRXPHYONLY, pDf, 
                                        "ERROR: cannot configure user '%s' when not IDLE",
                                        pUser->m_FriendlyName);
        return DT_STATUS_FAIL;
    }
    // Assume the worst
    pUser->m_IsConfigured = FALSE;

    // Did we already configure the channel? If not, apply the configuration and if yes 
    // check the configuration is compatible with the existing one.
    if (pDf->m_IsConfigured)
    {
        // Is we have only user, it is allowed to reconfigure the channel
        Int NumUsers = DtVectorDfChUser_Size(pDf->m_pUsers);
        if (NumUsers == 1)
        {
            // If configuration is compatible (same), no action is required, otherwise
            // unconfigure and apply new configuration 
            if (!DtDfChSdiRxPhyOnly_IsConfigCompatible(pDf, pCfg))
            {
                Status = DtDfChSdiRxPhyOnly_Unconfigure(pDf);
                Status = DtDfChSdiRxPhyOnly_Configure(pDf, pCfg);
            }
            else
                Status = DT_STATUS_OK;
        }
        else
        {
            // Multiple user, so this user must request a compatible configuration
            if (!DtDfChSdiRxPhyOnly_IsConfigCompatible(pDf, pCfg))
            {
                DtDbgOutDf(ERR, CHSDIRXPHYONLY, pDf, 
                          "ERROR: user '%s' tries to apply an incompatible configuration",
                          pUser->m_FriendlyName);
                return DT_STATUS_CONFIG_ERROR;
            }
            else
                Status = DT_STATUS_OK;
        }
    }
    else
        Status = DtDfChSdiRxPhyOnly_Configure(pDf, pCfg);

    // Mark user as configured upon success
    if (DT_SUCCESS(Status))
        pUser->m_IsConfigured = TRUE;

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_Detach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Detaches a user from the channel.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRxPhyOnly_Detach(
    DtDfChSdiRxPhyOnly* pDf, 
    const DtExclAccessObject* pObject, 
    const DtFileObject* pFile)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Let base implementation do heavy lifting
    return DtDfCh_Detach((DtDfCh*)pDf, pObject, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_DoIdleToStandy -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Executes an operational mode transition from IDLE to STANDBY.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRxPhyOnly_DoIdleToStandy(DtDfCh* pDfBase)
{
    DtDfChSdiRxPhyOnly* pDf = (DtDfChSdiRxPhyOnly*)pDfBase;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must be IDLE
    DT_ASSERT(pDf->m_OpMode == DT_FUNC_OPMODE_IDLE);

    RET_ON_ERR(DtDfSdiRx_SetOperationalMode(pDf->m_pDfSdiRx, DT_FUNC_OPMODE_RUN));

    // We are now in STANDBY
    pDf->m_OpMode = DT_FUNC_OPMODE_STANDBY;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_DoRunToStandby -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Executes an operational mode transition from RUN to STANDBY.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRxPhyOnly_DoRunToStandby(DtDfCh* pDfBase)
{
    DtDfChSdiRxPhyOnly* pDf = (DtDfChSdiRxPhyOnly*)pDfBase;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must be in RUN
    DT_ASSERT(DF_TO_THIS->m_OpMode == DT_FUNC_OPMODE_RUN);

    // Make data flow up to the formatter
    RET_ON_ERR(DtDfSdiRx_SetOperationalMode(pDf->m_pDfSdiRx, DT_FUNC_OPMODE_RUN));
    
    // We are now in STANDBY
    pDf->m_OpMode = DT_FUNC_OPMODE_STANDBY;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_DoStandbyToIdle -.-.-.-.-.-.-.-.-.-.-.-.-.
// 
// Executes an operational mode transition from STANDBY to IDLE.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRxPhyOnly_DoStandbyToIdle(DtDfCh* pDfBase)
{
    DtDfChSdiRxPhyOnly* pDf = (DtDfChSdiRxPhyOnly*)pDfBase;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must be in STANDBY
    DT_ASSERT(pDf->m_OpMode == DT_FUNC_OPMODE_STANDBY);

    RET_ON_ERR(DtDfSdiRx_SetOperationalMode(pDf->m_pDfSdiRx, DT_FUNC_OPMODE_IDLE));

    // We are now IDLE
    pDf->m_OpMode = DT_FUNC_OPMODE_IDLE;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_DoStandbyToRun -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Executes an operational mode transition from STANDY to RUN.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRxPhyOnly_DoStandbyToRun(DtDfCh* pDfBase)
{
    DtDfChSdiRxPhyOnly* pDf = (DtDfChSdiRxPhyOnly*)pDfBase;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must be in STANDBY
    DT_ASSERT(pDf->m_OpMode == DT_FUNC_OPMODE_STANDBY);

    // Make the data flow up to the formatter
    RET_ON_ERR(DtDfSdiRx_SetOperationalMode(pDf->m_pDfSdiRx, DT_FUNC_OPMODE_RUN));

    // We are now in RUN
    pDf->m_OpMode = DT_FUNC_OPMODE_RUN;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_FindHighestOpModeRequested -.-.-.-.-.-.-.-.-.-
//
//
// Find the 'highest' (RUN > STANDBY > IDLE) operational mode requested by the users.
//
// Note: caller should hold the state lock.
//
DtFuncOpMode DtDfChSdiRxPhyOnly_FindHighestOpModeRequested(
                                                          const DtDfChSdiRxPhyOnly* pDf)
{
    // Let base implementation do the heavy lifting 
    return DtDfCh_FindHighestOpModeRequested((DtDfCh*)pDf);
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_FindUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Find the user associated with the file handle. 
// 
// Note: caller should hold the state lock.
//
DtDfChSdiRxPhyOnlyUser* DtDfChSdiRxPhyOnly_FindUser(
                                       DtDfChSdiRxPhyOnly* pDf, const DtFileObject* pFile)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    // Let base implementation do the heavy lifting 
    return (DtDfChSdiRxPhyOnlyUser*)DtDfCh_FindUser((DtDfCh*)pDf, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_GetSdiStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the SDI status (i.e. detected #lines, field sizes, signal lock status, etc) for 
// the channel. 
//
// Note: caller should hold the state lock.
// 
DtStatus DtDfChSdiRxPhyOnly_GetSdiStatus(DtDfChSdiRxPhyOnly* pDf, 
                                 Int* pCarrierDetect, Int* pSdiLock, Int* pLineLock,
                                 Int* pValid, Int* pSdiRate, Int* pNumSymsHanc, 
                                 Int* pNumSymsVidVanc, Int* pNumLinesF1, Int* pNumLinesF2,
                                 Int* pIsLevelB, UInt32* pPayloadId, Int* pFramePeriod)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Do we have a SDIRX?
    if (pDf->m_pDfSdiRx == NULL)
        return DT_STATUS_NOT_INITIALISED;

    return DtDfSdiRx_GetSdiStatus(pDf->m_pDfSdiRx, pCarrierDetect, pSdiLock, pLineLock,
                                          pValid, pSdiRate, pNumSymsHanc, pNumSymsVidVanc,
                                          pNumLinesF1, pNumLinesF2,
                                          pIsLevelB, pPayloadId, pFramePeriod);
}

// .-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_GetUserOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
// Get the current operational mode for the user associated with the file.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRxPhyOnly_GetUserOperationalMode(
                DtDfChSdiRxPhyOnly* pDf, const DtFileObject* pFile, DtFuncOpMode* pOpMode)
{
    DtDfChSdiRxPhyOnlyUser* pUser=NULL;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pFile != NULL);
    DT_ASSERT(pOpMode != NULL);
      
    // Find the user and return the user operational mode
    pUser = DtDfChSdiRxPhyOnly_FindUser(pDf, pFile);
    if (pUser == NULL)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: could not find a user for '%p'", pFile);
        return DT_STATUS_NOT_FOUND;
    }
    *pOpMode = pUser->m_OpMode;

    DtDbgOutDf(MAX, CHSDIRX, pDf, "Operational mode for user '%s' is:  %d", 
                                                  pUser->m_FriendlyName, pUser->m_OpMode);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_IsConfigCompatible -.-.-.-.-.-.-.-.-.-.-.-.
//
// Check if the given channel configuration is compatible with the already applied 
// configuration. Basically, checks all configuration settings are the same with 
// exception of the DMA buffer pointer and address, which are unique by definition.
// 
// Note: caller should hold the state lock.
//
Bool DtDfChSdiRxPhyOnly_IsConfigCompatible(
                      const DtDfChSdiRxPhyOnly* pDf, const DtDfChSdiRxPhyOnlyConfig* pCfg)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    if (pCfg == NULL)               return FALSE; // Must have something to compare
    if (!pDf->m_IsConfigured)       return TRUE;  // No configuration yet, so any will do
    
    return (pCfg->m_SdiRate == pDf->m_Config.m_SdiRate);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Opens all children associated with this channel.
//
DtStatus DtDfChSdiRxPhyOnly_OpenChildren(DtDfCh* pDf)
{
    // List of core children supported by the SDIRXPHYONLYCHAN function
    const DtDfSupportedChild SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_DF, DT_FUNC_TYPE_SDIRX, DT_DF_SDIRX_NAME, NULL, 
                               (DtObjectBcOrDf**)(&DF_TO_THIS->m_pDfSdiRx), TRUE },
    };
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Let base implementation do the actual opening
    return DtDfCh_OpenChildren(pDf, SUPPORTED_CHILDREN, 
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_OpenUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Opens a new user and initializes its members to safe initial defaults.
//
DtDfChUser* DtDfChSdiRxPhyOnly_OpenUser(
    DtDfCh* pDf,
    const DtFileObject* pFile, 
    Bool HasExclAccess, 
    const char* pFriendlyName)
{
    DtDfChSdiRxPhyOnlyUser* pUser=NULL;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Let base do the creating
    pUser = (DtDfChSdiRxPhyOnlyUser*)DtDfCh_OpenUser(pDf, sizeof(DtDfChSdiRxPhyOnlyUser),
                                                     pFile, HasExclAccess, pFriendlyName);
    if (!pUser) return NULL;

    // Initialize DtDfChSdiRxPhyOnlyUser specific members. 
    // Note: DtDfCh_OpenUser has initialize base members.
    pUser->m_IsConfigured = FALSE;

    return (DtDfChUser*)pUser;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.
//
// Applies a new operational mode.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRxPhyOnly_SetOperationalMode(
                                          DtDfChSdiRxPhyOnly* pDf, DtFuncOpMode NewOpMode)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must have been enabled
    DF_CH_MUST_BE_ENABLED(pDf);

    // Must be configured to set to any mode other than IDLE
    if (!pDf->m_IsConfigured && NewOpMode!=DT_FUNC_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Let base implementation do the rest
    return DtDfCh_SetOperationalMode((DtDfCh*)pDf, NewOpMode);
}

// .-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_SetUserOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
// Set the current operational mode for the user associated with the file.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRxPhyOnly_SetUserOperationalMode(
    DtDfChSdiRxPhyOnly* pDf, 
    const DtFileObject* pFile, 
    DtFuncOpMode NewOpMode)
{
    DtStatus Status=DT_STATUS_OK;
    DtDfChSdiRxPhyOnlyUser* pUser=NULL;
    DtFuncOpMode OldUserOpMode=DT_FUNC_OPMODE_IDLE, HighestOpMode=DT_FUNC_OPMODE_IDLE;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pFile != NULL);

    DtDbgOutDf(MAX, CHSDIRXPHYONLY, pDf, "ENTER");

    // Must have been enabled
    DF_CH_MUST_BE_ENABLED(pDf);

    // Check for valid mode
    if (NewOpMode!=DT_FUNC_OPMODE_IDLE && NewOpMode!=DT_FUNC_OPMODE_STANDBY 
                                                         && NewOpMode!=DT_FUNC_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Find the user requesting the mode change
    pUser = DtDfChSdiRxPhyOnly_FindUser(pDf, pFile);
    if (pUser == NULL)
    {
        DtDbgOutDf(ERR,CHSDIRXPHYONLY,pDf,"ERROR: could not find a user for '%p'", pFile);
        return DT_STATUS_NOT_FOUND;
    }

    DtDbgOutDf(MAX, CHSDIRXPHYONLY, pDf, 
                              "User '%s' is requesting to change its op-mode from %d->%d",
                              pUser->m_FriendlyName, pUser->m_OpMode, NewOpMode);

    // Only allowed mode while not configured is IDLE
    if (NewOpMode!=DT_FUNC_OPMODE_IDLE && !pUser->m_IsConfigured)
    {
        DtDbgOutDf(ERR, CHSDIRXPHYONLY, pDf, 
                 "ERROR: cannot change op-mode to '%d' while user '%s' is not configured",
                 NewOpMode, pUser->m_FriendlyName);
        return DT_STATUS_NOT_INITIALISED;
    }

    // Store the mode requested by this user and than check what the 'highest' mode 
    // requested by all users. If the new mode is the highest than we will apply it
    OldUserOpMode = pUser->m_OpMode;    // Save old mode
    pUser->m_OpMode = NewOpMode;
    HighestOpMode = DtDfChSdiRxPhyOnly_FindHighestOpModeRequested(pDf);
    if (HighestOpMode == NewOpMode)
    {
        Status = DtDfChSdiRxPhyOnly_SetOperationalMode(pDf, NewOpMode);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, CHSDIRXPHYONLY, pDf, 
                                     "ERROR: failed to set op-mode to '%d' for user '%s'",
                                     NewOpMode, pUser->m_FriendlyName);
            // Restore old user mode
            pUser->m_OpMode = OldUserOpMode;
        }
    }
    else
    {
        DtDbgOutDf(MAX, CHSDIRXPHYONLY, pDf, 
                            "Op-mode (%d) requested by user '%s' is not the highest (%d)",
                            NewOpMode, pUser->m_FriendlyName, HighestOpMode);
        Status = DT_STATUS_OK;
    }
    DtDbgOutDf(MAX, CHSDIRXPHYONLY, pDf, "EXIT");

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRxPhyOnly_Unconfigure -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfChSdiRxPhyOnly_Unconfigure(DtDfChSdiRxPhyOnly* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Nothing really TODO other than marking channel as unconfigured
    pDf->m_IsConfigured = FALSE;
    return DT_STATUS_OK;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfChSdiRxPhyOnly implementation +=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Helper macro to cast stub's DtDf member to DtDfChSdiRxPhyOnly
#define IO_STUB  ((DtIoStubDfChSdiRxPhyOnly*)pStub)
#define STUB_DF  ((DtDfChSdiRxPhyOnly*)IO_STUB->m_pDf)

// -.-.-.-.-.-.- DtIoStubDfChSdiRxPhyOnly - Forwards of private functions -.-.-.-.-.-.-.
static DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd(
                                       const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
static DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd_Attach(
                                              const DtIoStubDfChSdiRxPhyOnly*,
                                              const DtFileObject*,
                                              const DtExclAccessObject*,
                                              const DtIoctlChSdiRxPhyOnlyCmdAttachInput*);
static DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd_Configure(
                                           const DtIoStubDfChSdiRxPhyOnly*, 
                                           const DtFileObject*,
                                           const DtIoctlChSdiRxPhyOnlyCmdConfigureInput*);
static DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd_Detach(
                                              const DtIoStubDfChSdiRxPhyOnly*,
                                              const DtFileObject*,
                                              const DtExclAccessObject*,
                                              const DtIoctlChSdiRxPhyOnlyCmdDetachInput*);
static DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd_GetOpMode(
                                                const DtIoStubDfChSdiRxPhyOnly*,
                                                const DtFileObject*,
                                                DtIoctlChSdiRxPhyOnlyCmdGetOpModeOutput*);
static DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd_GetSdiStatus(
                                            const DtIoStubDfChSdiRxPhyOnly*, 
                                             DtIoctlChSdiRxPhyOnlyCmdGetSdiStatusOutput*);
static DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd_SetOpMode(
                                           const DtIoStubDfChSdiRxPhyOnly*,
                                           const DtFileObject*,
                                           const DtIoctlChSdiRxPhyOnlyCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY;

static const DtIoctlProperties IOSTUB_DF_CHSDIRXPHYONLY_IOCTLS[] = 
{
    DT_IOCTL_PROPS_CHSDIRXPHYONLY_CMD(
        DtIoStubDfChSdiRxPhyOnly_OnCmd,
        NULL, NULL)
};

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+ DtIoStubDfChSdiRxPhyOnly - Public functions +=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// 
// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRxPhyOnly_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtIoStubDfChSdiRxPhyOnly_Close(DtIoStub* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRxPhyOnly));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRxPhyOnly_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubDfChSdiRxPhyOnly* DtIoStubDfChSdiRxPhyOnly_Open(DtDf* pDf)
{
    DtIoStubDfChSdiRxPhyOnly* pStub = NULL;
    DtIoStubDfOpenParams OpenParams;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialization of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfChSdiRxPhyOnly, pDf, NULL, 
                                                         DtIoStubDfChSdiRxPhyOnly_Close,
                                                         NULL,  // Use default IOCTL
                                                         IOSTUB_DF_CHSDIRXPHYONLY_IOCTLS);
    pStub = (DtIoStubDfChSdiRxPhyOnly*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+ DtIoStubDfChSdiRxPhyOnly - Private functions +=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRxPhyOnly_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd(
    const DtIoStub* pStub,
    DtIoStubIoParams* pIoParams, 
    Int* pOutSize)
{
    DtStatus Status=DT_STATUS_OK;
    const DtIoctlChSdiRxPhyOnlyCmdInput* pInData=NULL;
    DtIoctlChSdiRxPhyOnlyCmdOutput* pOutData=NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRxPhyOnly));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_CHSDIRXPHYONLY_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exclusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutIoStubDf(ERR, CHSDIRXPHYONLY, pStub, 
                                                   "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_ChSdiRxPhyOnlyCmd;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_ChSdiRxPhyOnlyCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_CHSDIRXPHYONLY_CMD_ATTACH:
        DT_ASSERT(pInData!=NULL);
        Status = DtIoStubDfChSdiRxPhyOnly_OnCmd_Attach(IO_STUB, 
                                          pIoParams->m_pFile, &pIoParams->m_ExclAccessObj,
                                          &pInData->m_Attach);
        break;
    case DT_CHSDIRXPHYONLY_CMD_CONFIGURE:
        DT_ASSERT(pInData!=NULL);
        Status = DtIoStubDfChSdiRxPhyOnly_OnCmd_Configure(
                                      IO_STUB, pIoParams->m_pFile, &pInData->m_Configure);
        break;
    case DT_CHSDIRXPHYONLY_CMD_DETACH:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubDfChSdiRxPhyOnly_OnCmd_Detach(IO_STUB, 
                                          pIoParams->m_pFile, &pIoParams->m_ExclAccessObj,
                                          &pInData->m_Detach);
        break;
    case DT_CHSDIRXPHYONLY_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubDfChSdiRxPhyOnly_OnCmd_GetOpMode(
                                     IO_STUB, pIoParams->m_pFile, &pOutData->m_GetOpMode);
        break;
    case DT_CHSDIRXPHYONLY_CMD_GET_SDI_STATUS:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubDfChSdiRxPhyOnly_OnCmd_GetSdiStatus(
                                                      IO_STUB, &pOutData->m_GetSdiStatus);
        break;
    case DT_CHSDIRXPHYONLY_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubDfChSdiRxPhyOnly_OnCmd_SetOpMode(
                                      IO_STUB, pIoParams->m_pFile, &pInData->m_SetOpMode);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRxPhyOnly_OnCmd_Attach -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd_Attach(
    const DtIoStubDfChSdiRxPhyOnly* pStub,
    const DtFileObject* pFile,
    const DtExclAccessObject* pObject,
    const DtIoctlChSdiRxPhyOnlyCmdAttachInput* pInData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRxPhyOnly));
    DT_ASSERT(pInData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRxPhyOnly_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRXPHYONLY, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command.
    Status = DtDfChSdiRxPhyOnly_Attach(STUB_DF, pFile, pObject, 
                                          pInData->m_ReqExclusiveAccess!=0 ? TRUE : FALSE,
                                          pInData->m_FriendlyName);

    // Do not forget to unlock the channel
    DtDfChSdiRxPhyOnly_Unlock(STUB_DF);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRxPhyOnly_OnCmd_Configure -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd_Configure(
    const DtIoStubDfChSdiRxPhyOnly* pStub, 
    const DtFileObject* pFile,
    const DtIoctlChSdiRxPhyOnlyCmdConfigureInput* pInData)
{
    DtStatus Status=DT_STATUS_OK;
    DtDfChSdiRxPhyOnlyConfig Cfg;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRxPhyOnly));
    DT_ASSERT(pInData != NULL);

    // Zero configuration
    DtMemZero(&Cfg, sizeof(Cfg));
        
    // Copy SDI rate
    Cfg.m_SdiRate = pInData->m_SdiRate;

    // Must lock the channel first
    Status = DtDfChSdiRxPhyOnly_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRXPHYONLY, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRxPhyOnly_ConfigureUser(STUB_DF, pFile, &Cfg);

    // Do not forget to unlock the channel
    DtDfChSdiRxPhyOnly_Unlock(STUB_DF);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRxPhyOnly_OnCmd_Detach -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd_Detach(
    const DtIoStubDfChSdiRxPhyOnly* pStub,
    const DtFileObject* pFile,
    const DtExclAccessObject* pObject,
    const DtIoctlChSdiRxPhyOnlyCmdDetachInput* pInData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRxPhyOnly));
    DT_ASSERT(pInData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRxPhyOnly_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRXPHYONLY, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRxPhyOnly_Detach(STUB_DF, pObject, pFile);

     // Do not forget to unlock the channel
    DtDfChSdiRxPhyOnly_Unlock(STUB_DF);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRxPhyOnly_OnCmd_GetOpMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd_GetOpMode(
    const DtIoStubDfChSdiRxPhyOnly* pStub,
    const DtFileObject* pFile,
    DtIoctlChSdiRxPhyOnlyCmdGetOpModeOutput* pOutData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRxPhyOnly));
    DT_ASSERT(pOutData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRxPhyOnly_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRXPHYONLY, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRxPhyOnly_GetUserOperationalMode(
                                      STUB_DF, pFile, (DtFuncOpMode*)&pOutData->m_OpMode);

     // Do not forget to unlock the channel
    DtDfChSdiRxPhyOnly_Unlock(STUB_DF);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRxPhyOnly_OnCmd_GetSdiStatus -.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd_GetSdiStatus(
    const DtIoStubDfChSdiRxPhyOnly* pStub,
    DtIoctlChSdiRxPhyOnlyCmdGetSdiStatusOutput* pOutData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRxPhyOnly));
    DT_ASSERT(pOutData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRxPhyOnly_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRXPHYONLY, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRxPhyOnly_GetSdiStatus(STUB_DF, &pOutData->m_CarrierDetect,
              &pOutData->m_SdiLock, &pOutData->m_LineLock, &pOutData->m_Valid,
              &pOutData->m_SdiRate, &pOutData->m_NumSymsHanc, &pOutData->m_NumSymsVidVanc,
              &pOutData->m_NumLinesF1, &pOutData->m_NumLinesF2, &pOutData->m_IsLevelB,
              &pOutData->m_PayloadId, &pOutData->m_FramePeriod);

     // Do not forget to unlock the channel
    DtDfChSdiRxPhyOnly_Unlock(STUB_DF);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRxPhyOnly_OnCmd_SetOpMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfChSdiRxPhyOnly_OnCmd_SetOpMode(
    const DtIoStubDfChSdiRxPhyOnly* pStub,
    const DtFileObject* pFile,
    const DtIoctlChSdiRxPhyOnlyCmdSetOpModeInput* pInData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRxPhyOnly));
    DT_ASSERT(pInData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRxPhyOnly_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRXPHYONLY, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRxPhyOnly_SetUserOperationalMode(
                                         STUB_DF, pFile, (DtFuncOpMode)pInData->m_OpMode);

    // Do not forget to unlock the channel
    DtDfChSdiRxPhyOnly_Unlock(STUB_DF);
    return Status;
}