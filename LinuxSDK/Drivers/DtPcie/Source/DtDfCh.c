// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfCh.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2022 DekTec
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
#include "DtDfCh.h"

// Steno notation for DT_RETURN_ON_ERROR
#define RET_ON_ERR(Expr) DT_RETURN_ON_ERROR(Expr)

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfCh implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// MACRO with default precondition checks for the DtDfCh function
#define DF_CH_DEFAULT_PRECONDITIONS(pDf)        \
            DT_ASSERT(pDf!=NULL && pDf->m_Size>=sizeof(DtDfCh))

// MACRO that checks the function has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_CH_MUST_BE_ENABLED(pDf)              \
            DF_MUST_BE_ENABLED_IMPL(COMMON, pDf)

// Helper macro to cast a DtDf* to a DtDfCh*
#define DF_CH     ((DtDfCh*)pDf)

// Tag for memory allocations
#define DF_CH_TAG  0x4E414843       // 'NAHC'

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static const int StateLockTimeout = 5000;  // Timeout for acquiring the user lock (in ms)

// -.-.-.-.-.-.-.-.-.-.-.- DtDfCh - Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.
static DtDfChUser* DtDfCh_OpenUser_Default(
             DtDfCh*, const DtFileObject*, Bool HasExclAccess, const char* pFriendlyName);
static DtStatus DtDfCh_ExclAccessAcquireChildren(DtDfCh*);
static DtStatus DtDfCh_ExclAccessReleaseChildren(DtDfCh*);

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_AddUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Adds a user to the list.
//
// Note: caller should hold the state lock.
// 
DtStatus DtDfCh_AddUser(
    DtDfCh* pDf, 
    const DtFileObject* pFile, 
    Bool HasExclAccess, 
    const char* pFriendlyName)
{
    DtStatus Status = DT_STATUS_OK;
    DtDfChUser* pNewUser= NULL;
    
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Check paramaters
    if (!pFile || !pFriendlyName)
        return DT_STATUS_INVALID_PARAMETER;
    // Check we have an open user function
    if (!pDf->m_OpenUserFunc)
        return DT_STATUS_NOT_INITIALISED;

    // Create the new user
    pNewUser = pDf->m_OpenUserFunc(pDf, pFile, HasExclAccess, pFriendlyName);
    if (!pNewUser)
        return DT_STATUS_OUT_OF_MEMORY;
    DT_ASSERT(pNewUser->m_Size >= sizeof(DtDfChUser));

    // Lock users list before we update it
    DtDfCh_LockUsers(pDf);
    Status = DtVectorDfChUser_PushBack(pDf->m_pUsers, pNewUser);
    DtDfCh_UnlockUsers(pDf);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_Attach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Attaches a user to the channel.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfCh_Attach(
    DtDfCh* pDf,
    const DtFileObject* pFile, 
    const DtExclAccessObject* pObject,
    Bool ReqExclusiveAccess, 
    const char* pFriendlyName)
{
    DtStatus Status=DT_STATUS_OK;
    Int NumUsers=0;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    if (!pFile || !pObject || !pFriendlyName)
        return DT_STATUS_INVALID_PARAMETER;

    // Check if user already exists (cannot attach twice)
    if (DtDfCh_FindUser(pDf, pFile) != NULL)
    {
        DtDbgOutDf(ERR,COMMON,pDf, "ERROR: user '%s' is already attached", pFriendlyName);
        return DT_STATUS_FAIL;
    }

    // Check if we can accept another user
    NumUsers = DtVectorDfChUser_Size(pDf->m_pUsers);
    if (NumUsers+1 > pDf->m_MaxNumUsersSupported)
    {
        DtDbgOutDf(ERR, COMMON, pDf, "ERROR: user '%s' cannot attach, maximum number "
                                                "of users (=%d) has already been reached",
                                                pFriendlyName, 
                                                pDf->m_MaxNumUsersSupported);
        return DT_STATUS_FAIL;
    }

    // Does the user want to attach exclusively
    if (ReqExclusiveAccess)
    {
        // Cannot attach exclusive when others are already attached
        if (NumUsers >= 1)
        {
            DtDbgOutDf(ERR, COMMON, pDf, 
                                   "ERROR: user '%s' cannot attach exclusive, others are "
                                   "attached already", pFriendlyName);
            return DT_STATUS_IN_USE;
        }

        // Try and acquire exclusive access
        Status = DtDf_ExclAccessAcquire((DtDf*)pDf, pObject);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, COMMON, pDf, 
                                "ERROR: failed to acquire exclusive access for user '%s'",
                                pFriendlyName);
            return Status;
        }
    }
    else
    {
        // Check is anyone else has exclusive access to this channel
        Status = DtDf_ExclAccessCheck((DtDf*)pDf, pObject);
        if (Status!=DT_STATUS_OK && Status!=DT_STATUS_EXCL_ACCESS_REQD)
        {
            DtDbgOutDf(ERR,COMMON,pDf, "ERROR: cannot attach user '%s', someone else "
                                   "has exclusive access to this channel", pFriendlyName);
            return Status;
        }
    }

    // Add user to list
    Status = DtDfCh_AddUser(pDf, pFile, ReqExclusiveAccess, pFriendlyName);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to add user '%s' to user list", 
                                                                           pFriendlyName);
        return Status;
    }

    // If this is our first user, claim exclusive access over all children, so that no one
    // else can use them.
    NumUsers = DtVectorDfChUser_Size(pDf->m_pUsers);
    if (NumUsers == 1)
    {
        Status = DtDfCh_ExclAccessAcquireChildren(pDf);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, COMMON, pDf, 
                                "ERROR: failed to acquire exclusive access for children");

            // Detach this user again
            DtDfCh_Detach(pDf, pObject, pFile);

            return Status;
        }
    }

    DtDbgOutDf(MIN, COMMON, pDf, 
                              "User '%s' (excl=%s) has attached successfully (#users=%d)",
                              pFriendlyName, ReqExclusiveAccess ? "yes" : "no", NumUsers);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtDfCh_Close(DtDfCh* pDf)
{
    if (pDf == NULL)
        return;     // Nothing to close

    DtDbgOutDf(MIN, COMMON, pDf, "Closing CH");

    // Cleanup user list
    if (pDf->m_pUsers != NULL)
    {
        DtVectorDfChUser_Cleanup(pDf->m_pUsers);
        pDf->m_pUsers = NULL;
    }

    // Let base function perform final clean-up
    DtDf_Close((DtDf*)pDf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_CloseUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Closes a user object but first releases any resources it may hold.
//
void DtDfCh_CloseUser(DtDfCh* pDf, DtDfChUser* pUser, const DtFileObject* pFile)
{
    if (!pUser)
        return;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pUser->m_Size >= sizeof(DtDfChUser));

    DtMemFreePool(pUser, DF_CH_TAG);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_Detach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Detaches a user from the channel.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfCh_Detach(
                  DtDfCh* pDf, const DtExclAccessObject* pObject, const DtFileObject* pFile)
{
    DtStatus Status = DT_STATUS_OK;
    Bool UserHadExclAccess=FALSE;
    Int NumUsers=0, StrLen=0;
    DtDfChUser* pUser=NULL;
    char FriendlyName[DT_CHAN_FRIENDLY_NAME_MAX_LENGTH+1];
    
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    if (!pFile || !pObject)
        return DT_STATUS_INVALID_PARAMETER;

    // Check if we can find our user
    pUser = DtDfCh_FindUser(pDf, pFile);
    if (pUser == NULL)
    {
        DtDbgOutDf(ERR, COMMON, pDf, "ERROR: no user '%p' was found", pFile);
        return DT_STATUS_NOT_FOUND;
    }

    // Cache friendly name and exclusive access state, so that we can use these variables
    // after removing (destroying) the user.
    UserHadExclAccess = pUser->m_HasExclAccess;
    StrLen = DtAnsiCharArrayStrLength(pUser->m_FriendlyName, 
                                                        sizeof(pUser->m_FriendlyName));
    if (StrLen>0 && StrLen<sizeof(FriendlyName))
    {
        DtMemCopy(FriendlyName, pUser->m_FriendlyName, StrLen);
        FriendlyName[StrLen] = '\0';
    }
    else
        FriendlyName[0] = '\0';

    // Remove/destroy our user.
    // NOTE: pUser is no longer valid and cannot be used after this point => hence we 
    // cached some state variable above.
    Status = DtDfCh_RemoveUser(pDf, pFile, pUser);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to remove user '%s' from "
                                                            "user list", FriendlyName);
        return Status;
    }
    pUser = NULL;   // Invalidate user pointer.

    // Check if any users will be left after this user detaches
    NumUsers = DtVectorDfChUser_Size(pDf->m_pUsers);
    if (NumUsers == 0)
    {
        // Notify derived class that the last user is detaching
        if (pDf->m_OnDetachLastUserFunc)
            Status = pDf->m_OnDetachLastUserFunc(pDf);

        // Release exclusive access to all children
        Status = DtDfCh_ExclAccessReleaseChildren(pDf);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, COMMON, pDf, 
                                "ERROR: failed to release exclusive access for children");
        }
        
        // Release exclusive access to this function (if this user had exclusive access)
        if (UserHadExclAccess)
        {
            Status = DtDf_ExclAccessRelease((DtDf*)pDf, pObject);
            if (!DT_SUCCESS(Status))
                DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to release exclusive access");
        }
    }
    DtDbgOutDf(MIN, COMMON, pDf, "User '%s' has detached successfully", FriendlyName);
    return DT_STATUS_OK;
}

// Find the 'highest' (RUN > STANDBY > IDLE) operational mode requested by the users.
//
// Note: caller should hold the state lock.
//
DtFuncOpMode DtDfCh_FindHighestOpModeRequested(const DtDfCh* pDf)
{
    Int i=0, NumElems=0;
    DtFuncOpMode HighestOpMode = DT_FUNC_OPMODE_IDLE;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    NumElems = DtVectorDfChUser_Size(pDf->m_pUsers);
    for (i=0; i<NumElems && HighestOpMode!=DT_FUNC_OPMODE_RUN; i++)
    {
        DtDfChUser* pUser = (DtDfChUser*)DtVectorDfChUser_At(pDf->m_pUsers, i);
        const DtFuncOpMode UserOpMode = pUser->m_OpMode;
        switch (HighestOpMode)
        {
        case DT_FUNC_OPMODE_IDLE:
            if (UserOpMode==DT_FUNC_OPMODE_STANDBY || UserOpMode==DT_FUNC_OPMODE_RUN)
                HighestOpMode = UserOpMode;
            break;

        case DT_FUNC_OPMODE_STANDBY:
            if (UserOpMode == DT_FUNC_OPMODE_RUN)
                HighestOpMode = UserOpMode;
            break;

        case DT_FUNC_OPMODE_RUN:
            // RUN is highest mode there is
            break;

        default:
            DT_ASSERT(FALSE);
            break;
        }
    }
    return HighestOpMode;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_FindUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Find the user associated with the file handle. 
// 
// Note: caller should hold the state lock.
//
DtDfChUser* DtDfCh_FindUser(DtDfCh* pDf, const DtFileObject* pFile)
{
    Int i=0, NumElems=0;
    DtDfChUser* pUser=NULL;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pFile != NULL);

    // Lock the users, so that they cannot disappear
    DtDfCh_LockUsers(pDf);
    NumElems = DtVectorDfChUser_Size(pDf->m_pUsers);
    for (i=0; i<NumElems; i++)
    {
        pUser = DtVectorDfChUser_At(pDf->m_pUsers, i);
        if (DtFileCompare(&pUser->m_File, pFile))
            break;
        pUser = NULL;
    }
    DtDfCh_UnlockUsers(pDf);
    return pUser;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_HasUsers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Return TRUE when then channel has users
// 
// Note: caller should hold the state lock.
//
Bool DtDfCh_HasUsers(const DtDfCh* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    return (DtVectorDfChUser_Size(pDf->m_pUsers) > 0);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_Lock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Acquires the channel state lock.
//
DtStatus DtDfCh_Lock(DtDfCh* pDf, int Timeout)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    return DtMutexAcquire(&pDf->m_StateLock, Timeout);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_LockUsers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Acquire lock protecting the user list.
// 
// Note: hold this lock as short as possible.
//
void DtDfCh_LockUsers(DtDfCh* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DtSpinLockAcquire(&pDf->m_UsersLock);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Handler for a close file handle event. Checks if the file handle matches on of the 
// users attached to this channel and detaches it.
//
// Note: caller should NOT hold the channel lock.
//
DtStatus DtDfCh_OnCloseFile(DtDf* pDf, const DtFileObject* pFile)
{
    DtStatus Status=DT_STATUS_OK;
    DtDfChUser* pUser=NULL;
    
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Lock channel
    if(DtDfCh_Lock(DF_CH, StateLockTimeout) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    // Check this user is one of ours? If yes, detach it from the channel
    pUser = DtDfCh_FindUser(DF_CH, pFile);
    if (pUser)
    {
        // Create an exclusive access object, so that we can detach
        DtExclAccessObject ExclAccessObject;
        ExclAccessObject.m_Type = DT_EXCL_OBJECT_TYPE_IS_FILE;
        ExclAccessObject.m_Owner.m_File = *pFile;

        DtDbgOutDf(MAX,COMMON,pDf, "Detaching leftover user '%s'", pUser->m_FriendlyName);
        Status = DtDfCh_Detach(DF_CH, &ExclAccessObject, pFile);
        if (!DT_SUCCESS(Status))
        {
            // NOTE: cannot user 'pUser' here as it may no longer exists, hence use pFile
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to detach user '%p'", pFile);
        }
    }
    // Do not forget to unlock
    DtDfCh_Unlock(DF_CH);

    // Finally, call base implementation to release exclusive access on this function.
    return DtDf_OnCloseFile(pDf, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_OnCloseOtherFiles -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Handler for a close other files event. Checks if there are any other files with 
// associated users attached to this channel and detaches them.
//
// Note: caller should NOT hold the channel lock.
//
DtStatus DtDfCh_OnCloseOtherFiles(DtDf* pDf, const DtFileObject* pFile)
{
    Bool AquiredLock=TRUE;
    DtStatus Status=DT_STATUS_OK;

    DtDbgOutDf(ERR, COMMON, pDf, "Attempting to close other users");

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Lock channel. If timeout continue anyway.
    AquiredLock = TRUE; // Assume success.
    if(DtDfCh_Lock(DF_CH, 500) != DT_STATUS_OK)
    {
        DtDbgOutDf(ERR, COMMON, pDf, "Failed to acquire lock => continue anyway");
        AquiredLock = FALSE;
    }

    // Check there are any users to remove
    if (DtDfCh_HasUsers(DF_CH))
    {
        int i=0, NumUsers=0;

        // Create a list of all users to remove.
        DtVectorDfChUser* pUsersToRemove = DtVectorDfChUser_Create(4,2);
        DtDfCh_LockUsers(DF_CH);
        NumUsers = DtVectorDfChUser_Size(DF_CH->m_pUsers);
        for (i=0; i<NumUsers; i++)
        {
            // Add all except ourselves to the list.
            DtDfChUser* pUser = DtVectorDfChUser_At(DF_CH->m_pUsers, i);
            DtDbgOutDf(ERR, COMMON, pDf, "User[%d]='%s'", i, pUser->m_FriendlyName);
            if (DtFileCompare(&pUser->m_File, pFile))
                continue;
            DtVectorDfChUser_PushBack(pUsersToRemove, pUser);
        }
        DtDfCh_UnlockUsers(DF_CH);

        // Remove the users
        NumUsers = DtVectorDfChUser_Size(pUsersToRemove);
        for (i=0; i<NumUsers; i++)
        {
            DtDfChUser* pUser = DtVectorDfChUser_At(pUsersToRemove, i);

            // Create an exclusive access object, so that we can detach
            DtExclAccessObject ExclAccessObject;
            ExclAccessObject.m_Type = DT_EXCL_OBJECT_TYPE_IS_FILE;
            ExclAccessObject.m_Owner.m_File = pUser->m_File;

            DtDbgOutDf(ERR, COMMON, pDf, "Force detach of user '%s'", 
                                                                   pUser->m_FriendlyName);
            Status = DtDfCh_Detach(DF_CH, &ExclAccessObject, &pUser->m_File);
            if (!DT_SUCCESS(Status))
            {
                // NOTE: cannot user 'pUser' here as it may no longer exists, hence use pFile
                DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to detach user '%p'", 
                                                                          &pUser->m_File);
            }
        }
        DtVectorDfChUser_Cleanup(pUsersToRemove);
    }

    // Do not forget to unlock
    if (AquiredLock)
        DtDfCh_Unlock(DF_CH);

    // Finally, call base implementation to force a close of this function.
    return DtDf_OnCloseOtherFiles(pDf, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDf* DtDfCh_Open(const DtDfChOpenParams* pParams)
{
    DtStatus Status=DT_STATUS_OK;
    DtDfCh* pDf=NULL;

    DT_ASSERT(pParams != NULL);

    // Channels should DtDfChOpenParams::m_InitFunc.
    DT_ASSERT(pParams->m_DfParams.m_InitFunc == NULL);

    // Call base open implementation to create the channel driver function
    pDf = (DtDfCh*)DtDf_Open(&pParams->m_DfParams);
    if (!pDf)
        return NULL;
    DT_ASSERT(pDf->m_Size >= sizeof(DtDfCh));

    // Register channel specific callbacks. Note: for callbacks with a default we use the 
    // default one if no specific function was set.
    pDf->m_CloseUserFunc = pParams->m_CloseUserFunc 
                                            ? pParams->m_CloseUserFunc : DtDfCh_CloseUser;

    // Operation mode transition callbacks are required
    DT_ASSERT(pParams->m_DoIdleToStandyFunc);
    pDf->m_DoIdleToStandyFunc = pParams->m_DoIdleToStandyFunc;
    DT_ASSERT(pParams->m_DoRunToStandbyFunc);
    pDf->m_DoRunToStandbyFunc = pParams->m_DoRunToStandbyFunc;
    DT_ASSERT(pParams->m_DoStandbyToIdleFunc);
    pDf->m_DoStandbyToIdleFunc = pParams->m_DoStandbyToIdleFunc;
    DT_ASSERT(pParams->m_DoStandbyToRunFunc);
    pDf->m_DoStandbyToRunFunc = pParams->m_DoStandbyToRunFunc;

    pDf->m_InitChanFunc = pParams->m_InitFunc ? pParams->m_InitFunc : DtDfCh_Init;
    pDf->m_OpenChildrenFunc = pParams->m_OpenChildrenFunc;
    pDf->m_OpenUserFunc = pParams->m_OpenUserFunc 
                                      ? pParams->m_OpenUserFunc : DtDfCh_OpenUser_Default;
    pDf->m_OnDetachLastUserFunc = pParams->m_OnDetachLastUserFunc;

    // Save maximum #users
    pDf->m_MaxNumUsersSupported = pParams->m_MaxNumUsersSupported;

    // Perform initialization (if an init function exists)
    Status = pDf->m_InitChanFunc(pDf);
    if (!DT_SUCCESS(Status))
    {
        DT_ASSERT(pDf->m_CloseFunc != NULL);
        pDf->m_CloseFunc((DtDf*)pDf);
        return NULL;
    }
    return (DtDf*)pDf;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_OpenUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfChUser* DtDfCh_OpenUser(DtDfCh* pDf, Int SizeOfObject, const DtFileObject* pFile, 
                                            Bool HasExclAccess, const char* pFriendlyName)
{
    Int NameLength=0;
    DtDfChUser* pUser=NULL;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    DT_ASSERT(SizeOfObject >= sizeof(DtDfChUser));
    if (SizeOfObject < sizeof(DtDfChUser))
        return NULL;
    DT_ASSERT(pFile && pFriendlyName);
    if (!pFile || !pFriendlyName)
        return NULL;

    pUser = (DtDfChUser*)DtMemAllocPool(DtPoolNonPaged, SizeOfObject, DF_CH_TAG);
    if (!pUser) return NULL;
    DtMemZero(pUser, SizeOfObject);
    pUser->m_Size = SizeOfObject;

    // Initialize base members
    pUser->m_File = *pFile;
    pUser->m_HasExclAccess = HasExclAccess;
    pUser->m_OpMode = DT_FUNC_OPMODE_IDLE;

    // Add the name
    NameLength = DtAnsiCharArrayStrLength(pFriendlyName, sizeof(pUser->m_FriendlyName));
    DT_ASSERT(NameLength <= DT_CHAN_FRIENDLY_NAME_MAX_LENGTH);
    if (NameLength>0 && NameLength<sizeof(pUser->m_FriendlyName))
    {
        DtMemCopy(pUser->m_FriendlyName, pFriendlyName, NameLength);
        pUser->m_FriendlyName[NameLength] = '\0';
    }
    else
        pUser->m_FriendlyName[0] = '\0';

    return pUser;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Opens the list of children supported by the channel. If a child cannot be found and it 
// is marked mandatory this function will fail. 
//
DtStatus DtDfCh_OpenChildren(
    DtDfCh* pDf,
    const DtDfSupportedChild* pSupported,
    Int NumSupported)
{
    Int i=0;
    DtStatus Status=DT_STATUS_OK;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Make sure all driver functions and blocks are loaded, so that we can find them
    if (NumSupported > 0)
    {
        //Status = DtCore_DF_OpenAll(pDf->m_pCore, pDf->m_pPt);
        //if (!DT_SUCCESS(Status))
        //    return Status;
        Status = DtCore_BC_OpenAll(pDf->m_pCore, pDf->m_pPt);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    for (i=0; i<NumSupported; i++)
    {
        Bool FoundChild = FALSE;
        const DtDfSupportedChild* pSuppChild = &(pSupported[i]);

        if (pSuppChild->m_ppChild == NULL)
            continue;
        *pSuppChild->m_ppChild = NULL;

        if (pSuppChild->m_ObjectType == DT_OBJECT_TYPE_BC)
        {
            DtBc* pChildBc = DtCore_BC_Find(pDf->m_pCore, pDf->m_pPt, 
                                                             (DtBcType)pSuppChild->m_Type,
                                                             pSuppChild->m_pRole);
            if (pChildBc != NULL)
            {
                FoundChild = TRUE;

                // Add to child list
                Status = DtDf_AddBc((DtDf*)pDf, pChildBc);
                if (!DT_SUCCESS(Status))
                    return Status;

                if (pSuppChild->m_ppChild != NULL)
                    *pSuppChild->m_ppChild = (DtObjectBcOrDf*)pChildBc;

                DtDbgOutDf(MIN, COMMON, pDf, "Added child '%s'", pChildBc->m_InstanceId);
            }
        }
        else if (pSuppChild->m_ObjectType == DT_OBJECT_TYPE_DF)
        {
            DtDf* pChildDf = DtCore_DF_Find(pDf->m_pCore, pDf->m_pPt, 
                                                       (DtFunctionType)pSuppChild->m_Type,
                                                       pSuppChild->m_pRole);
            if (pChildDf != NULL)
            {
                FoundChild = TRUE;

                // Add to child list
                Status = DtDf_AddDf((DtDf*)pDf, pChildDf);
                if (!DT_SUCCESS(Status))
                    return Status;

                if (pSuppChild->m_ppChild != NULL)
                    *pSuppChild->m_ppChild = (DtObjectBcOrDf*)pChildDf;

                DtDbgOutDf(MIN, COMMON, pDf, "Added child '%s'", pChildDf->m_InstanceId);
            }
        }
        else
        {
            DT_ASSERT(FALSE);
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: unknown object type '%d'", 
                                                                pSuppChild->m_ObjectType);
            return DT_STATUS_FAIL;
        }

        // Fail when a child is not found, while marked as mandatory
        if (!FoundChild && pSuppChild->m_IsMandatory)
        {
            DtDbgOutDf(ERR, COMMON, pDf,
                                        "Could not find mandatory child '%s' (Role='%s')",
                                        pSuppChild->m_pName,
                                        pSuppChild->m_pRole ? pSuppChild->m_pRole : "-");
            return DT_STATUS_NOT_FOUND;
        }
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_RemoveUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Removes a user from the user list and releases any resources it may hold.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfCh_RemoveUser(DtDfCh* pDf, const DtFileObject* pFile, DtDfChUser* pUserToRemove)
{
    Int i=0, NumElems=0;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Do we have a user to clean-up
    if (pUserToRemove == NULL)
        return DT_STATUS_OK;

    DtDbgOutDf(MAX, COMMON, pDf, "Removing user '%s'", pUserToRemove->m_FriendlyName);
        
    // Find user in user list and erase it
    NumElems = DtVectorDfChUser_Size(pDf->m_pUsers);
    for (i=0; i<NumElems; i++)
    {
        DtDfChUser* pUser = DtVectorDfChUser_At(pDf->m_pUsers, i);
        if (pUserToRemove != pUser)
            continue;

        // Erase from list. Lock list before removing the user
        DtDfCh_LockUsers(pDf);
        DtVectorDfChUser_Erase(pDf->m_pUsers, i);
        DtDfCh_UnlockUsers(pDf);
        // Finally, call close user function to clean-up user
        if (pDf->m_CloseUserFunc)
            pDf->m_CloseUserFunc(pDf, pUser, pFile);
        return DT_STATUS_OK;
    }
    return DT_STATUS_NOT_FOUND;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Applies a new operational mode.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfCh_SetOperationalMode(DtDfCh* pDf, DtFuncOpMode NewOpMode)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must have been enabled
    DF_CH_MUST_BE_ENABLED(pDf);

    // Check for valid mode
    if (NewOpMode!=DT_FUNC_OPMODE_IDLE && NewOpMode!=DT_FUNC_OPMODE_STANDBY 
                                                         && NewOpMode!=DT_FUNC_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Check operational transition handlers are registered
    if (!pDf->m_DoIdleToStandyFunc || !pDf->m_DoRunToStandbyFunc 
                             || !pDf->m_DoStandbyToIdleFunc || !pDf->m_DoStandbyToRunFunc)
    {
        DtDbgOutDf(ERR, COMMON, pDf, "ERROR: missing m_DoXxx handler(s)");
        return DT_STATUS_NOT_INITIALISED;
    }

    // Any change
    if (NewOpMode == pDf->m_OpMode)
        return DT_STATUS_OK;

    DtDbgOutDf(MAX, COMMON, pDf, "Changing channel op-mode from %d->%d",
                                                                pDf->m_OpMode, NewOpMode);

    switch (pDf->m_OpMode)
    {
    case DT_FUNC_OPMODE_UNDEF:
        // Treat as if we were in STANDBY
        pDf->m_OpMode = DT_FUNC_OPMODE_STANDBY;
        // Set to IDLE first
        RET_ON_ERR(DtDfCh_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE));
        // Finaly: set to the desired mode
        RET_ON_ERR(DtDfCh_SetOperationalMode(pDf, NewOpMode));
        break;

    case DT_FUNC_OPMODE_IDLE:
        // IDLE -> RUN
        if (NewOpMode == DT_FUNC_OPMODE_RUN)
        {
            // Go to run via standby
            RET_ON_ERR(pDf->m_DoIdleToStandyFunc(pDf));
            RET_ON_ERR(pDf->m_DoStandbyToRunFunc(pDf));
        }
        // IDLE -> STANDBY
        else if (NewOpMode == DT_FUNC_OPMODE_STANDBY)
            RET_ON_ERR(pDf->m_DoIdleToStandyFunc(pDf));
        break;

    case DT_FUNC_OPMODE_STANDBY:
        // STANDBY -> IDLE
        if (NewOpMode == DT_FUNC_OPMODE_IDLE)
            RET_ON_ERR(pDf->m_DoStandbyToIdleFunc(pDf));
        // STANDBY -> RUN
        else if (NewOpMode == DT_FUNC_OPMODE_RUN)
            RET_ON_ERR(pDf->m_DoStandbyToRunFunc(pDf));
        break;

    case DT_FUNC_OPMODE_RUN:
        // RUN -> IDLE
        if (NewOpMode == DT_FUNC_OPMODE_IDLE)
        {
            // Go to idle via STANDBY
            RET_ON_ERR(pDf->m_DoRunToStandbyFunc(pDf));
            RET_ON_ERR(pDf->m_DoStandbyToIdleFunc(pDf));
        }
        // RUN -> STANDBY
        else if (NewOpMode == DT_FUNC_OPMODE_STANDBY)
            RET_ON_ERR(pDf->m_DoRunToStandbyFunc(pDf));
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_INVALID_IN_OPMODE;
    }
    // If we come here the call was successful and the new mode must have been applied
    DT_ASSERT(pDf->m_OpMode == NewOpMode);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_Unlock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Releases the channel state lock.
//
void DtDfCh_Unlock(DtDfCh* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DtMutexRelease(&pDf->m_StateLock);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_UnlockUsers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Release lock protecting the user list.
//
void DtDfCh_UnlockUsers(DtDfCh* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DtSpinLockRelease(&pDf->m_UsersLock);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfCh - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_OpenUser_Default -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Calls DtCh_OpenUser to create the base channel user object.
//
DtDfChUser* DtDfCh_OpenUser_Default(
    DtDfCh* pDf,
    const DtFileObject* pFile,
    Bool HasExclAccess, 
    const char* pFriendlyName)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    return DtDfCh_OpenUser(pDf, sizeof(DtDfChUser), pFile, HasExclAccess, pFriendlyName);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_ExclAccessAcquireChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Acquire exclusive access for all children used by this function, so that no others can 
// use it.
// 
// Note: caller should hold the state lock.
// 
DtStatus DtDfCh_ExclAccessAcquireChildren(DtDfCh* pDf)
{
    DtStatus Status=DT_STATUS_OK;
    Int i=0, NumChildren=0;
    DtExclAccessObject ExclAccessObject;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    DtDbgOutDf(MAX, COMMON, pDf, "Acquiring exclusive access for all children");

    // Set this function as the owner
    ExclAccessObject.m_Type = DT_EXCL_OBJECT_TYPE_IS_DF;
    ExclAccessObject.m_Owner.m_pDf = (DtDf*)pDf;

    // Lock all child blocks
    NumChildren = DtVectorBc_Size(pDf->m_pBcList);
    for (i=0; i<NumChildren; i++)
    {
        DtBc* pChildBc = DtVectorBc_At(pDf->m_pBcList, i);
        if (!pChildBc)
            continue;

        // Check if we already have exclusive access
        Status = DtBc_ExclAccessCheck(pChildBc, &ExclAccessObject);
        if (DT_SUCCESS(Status))
            continue;   // Already have exclusive access => no further action required

        Status = DtBc_ExclAccessAcquire(pChildBc, &ExclAccessObject);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, COMMON, pDf, 
                           "ERROR: failed to acquire exclusive access to BC '%s:%d'",
                            pChildBc->m_Id.m_pName, pChildBc->m_Id.m_Instance);
            break;
        }
    }
    // Upon failure: release exclusive access for the ones that succeeded
    if (!DT_SUCCESS(Status))
    {
        Int j=0;
        for (j=0; j<(i-1); j++)
        {
            DtBc* pChildBc = DtVectorBc_At(pDf->m_pBcList, j);
            if (!pChildBc)
                continue;
            DtBc_ExclAccessRelease(pChildBc, &ExclAccessObject);
        }
        return Status;
    }

    // Lock all child driver functions
    NumChildren = DtVectorDf_Size(pDf->m_pDfList);
    for (i=0; i<NumChildren; i++)
    {
        DtDf* pChildDf = DtVectorDf_At(pDf->m_pDfList, i);
        if (!pChildDf)
            continue;

        // Check if we already have exclusive access
        Status = DtDf_ExclAccessCheck(pChildDf, &ExclAccessObject);
        if (DT_SUCCESS(Status))
            continue;   // Already have exclusive access => no further action required

        Status = DtDf_ExclAccessAcquire(pChildDf, &ExclAccessObject);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, COMMON, pDf, 
                           "ERROR: failed to acquire exclusive access to DF '%s:%d'",
                            pChildDf->m_Id.m_pName, pChildDf->m_Id.m_Instance);
            break;
        }
    }
    // Upon failure: release exclusive access for the ones that succeeded
    if (!DT_SUCCESS(Status))
    {
        Int j=0;
        for (j=0; j<(i-1); j++)
        {
            DtDf* pChildDf = DtVectorDf_At(pDf->m_pBcList, j);
            if (!pChildDf)
                continue;
            DtDf_ExclAccessRelease(pChildDf, &ExclAccessObject);
        }
        return Status;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_ExclAccessReleaseChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Release exclusive access from all children used by this function, so that others can 
// use it again.
// 
// Note: caller should hold the state lock.
// 
DtStatus DtDfCh_ExclAccessReleaseChildren(DtDfCh* pDf)
{
    DtStatus Status=DT_STATUS_OK;
    Int i=0, NumChildren=0;
    Bool Error=FALSE;
    DtExclAccessObject ExclAccessObject;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    DtDbgOutDf(MAX, COMMON, pDf, "Releasing exclusive access on all children");

    // This function is the owner
    ExclAccessObject.m_Type = DT_EXCL_OBJECT_TYPE_IS_DF;
    ExclAccessObject.m_Owner.m_pDf = (DtDf*)pDf;

    // UnLock all child blocks
    NumChildren = DtVectorBc_Size(pDf->m_pBcList);
    for (i=0; i<NumChildren; i++)
    {
        DtBc* pChildBc = DtVectorBc_At(pDf->m_pBcList, i);
        if (!pChildBc)
            continue;

        Status = DtBc_ExclAccessRelease(pChildBc, &ExclAccessObject);
        if (!DT_SUCCESS(Status))
        {
            Error = TRUE;
            DtDbgOutDf(ERR, COMMON, pDf, 
                                "ERROR: failed to release exclusive access to BC '%s:%d'",
                                pChildBc->m_Id.m_pName, pChildBc->m_Id.m_Instance);
        }

    }
    // Unlock all child driver functions
    NumChildren = DtVectorDf_Size(pDf->m_pDfList);
    for (i=0; i<NumChildren; i++)
    {
        DtDf* pChildDf = DtVectorDf_At(pDf->m_pDfList, i);
        if (!pChildDf)
            continue;

        Status = DtDf_ExclAccessRelease(pChildDf, &ExclAccessObject);
        if (!DT_SUCCESS(Status))
        {
            Error = TRUE;
            DtDbgOutDf(ERR, COMMON, pDf, 
                                "ERROR: failed to release exclusive access to DF '%s:%d'",
                                pChildDf->m_Id.m_pName, pChildDf->m_Id.m_Instance);
        }
     
    }
    return Error ? DT_STATUS_FAIL : DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCh_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initializes the channels base resources.
//
DtStatus DtDfCh_Init(DtDfCh* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Initial op-mode is undefined
    pDf->m_OpMode = DT_FUNC_OPMODE_UNDEF;

    // Create the user list (initial capacity=4 and grow=2)
    pDf->m_pUsers = DtVectorDfChUser_Create(4, 2);
    if (pDf->m_pUsers == NULL)
    {
        DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to create user list");
        return DT_STATUS_OUT_OF_MEMORY;
    }
    // Initialize state mutex and users list lock
    DtMutexInit(&pDf->m_StateLock);
    DtSpinLockInit(&pDf->m_UsersLock);

    // Open the children
    if (pDf->m_OpenChildrenFunc)
    {
        DtStatus Status = pDf->m_OpenChildrenFunc(DF_CH);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to open children");
            return DT_STATUS_FAIL;
        }
    }
    return DT_STATUS_OK;
}
