// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfChSdiRx.c *#*#*#*#*#*#*#*#*#*# (C) 2022 DekTec
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
#include "DtDfChSdiRx.h"

// Steno notation for DT_RETURN_ON_ERROR
#define RET_ON_ERR(Expr) DT_RETURN_ON_ERROR(Expr)

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfChSdiRx implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// MACRO with default precondition checks for the DtDfChSdiRx function
#define DF_CH_DEFAULT_PRECONDITIONS(pDf)        \
            DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfChSdiRx))

// MACRO that checks the function has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_CH_MUST_BE_ENABLED(pDf)              \
            DF_MUST_BE_ENABLED_IMPL(CHSDIRX, pDf)

// Helper macro to cast a DtDf* to a DtDfChSdiRx*
#define DF_TO_THIS      ((DtDfChSdiRx*)pDf)

// Tag for memory allocations
#define DF_CHSDIRX_TAG  0x48435253   // 'HCRS'

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static const int StateLockTimeout = 500;  // Timeout for acquiring the user lock (in ms)
static const int MaxNumUsersSupported = 8;  // Max of 8 users may share this channel

static const int PageSize = 4096;
static const int MaxDmaBufferSize = 256*1024*1024;

// .-.-.-.-.-.-.-.-.-.- DtDfChSdiRx - Forwards of private functions -.-.-.-.-.-.-.-.-.-.
static DtStatus DtDfChSdiRx_AllocateDmaBuffer(DtDfChSdiRx*, Int MinSize);
static DtStatus DtDfChSdiRx_Attach(DtDfChSdiRx*, const DtFileObject*, 
                                      const DtExclAccessObject*, 
                                      Bool ReqExclusiveAccess, const char* pFriendlyName);
static DtStatus DtDfChSdiRx_CheckAndUpdateReadOffset(DtDfChSdiRx*, Bool ForceUpdate);
static void DtDfChSdiRx_CloseUser(DtDfCh*, DtDfChUser*, const DtFileObject*);
static DtStatus DtDfChSdiRx_Configure(DtDfChSdiRx*, const DtDfChSdiRxConfig*);
static DtStatus DtDfChSdiRx_Configure_Quadlink(
                                              DtDfChSdiRx*, const DtDfChSdiRxConfig*);
static DtStatus DtDfChSdiRx_ConfigureUser(DtDfChSdiRx*, const DtFileObject*, 
                                                                const DtDfChSdiRxConfig*);
static DtStatus DtDfChSdiRx_Detach(
                          DtDfChSdiRx*, const DtExclAccessObject*, const DtFileObject*);
static DtStatus DtDfChSdiRx_DoIdleToStandy(DtDfCh*);
static DtStatus DtDfChSdiRx_DoIdleToStandy_Quadlink(DtDfChSdiRx*);
static DtStatus DtDfChSdiRx_DoRunToStandby(DtDfCh*);
static DtStatus DtDfChSdiRx_DoRunToStandby_Quadlink(DtDfChSdiRx*);
static DtStatus DtDfChSdiRx_DoStandbyToIdle(DtDfCh*);
static DtStatus DtDfChSdiRx_DoStandbyToIdle_Quadlink(DtDfChSdiRx*);
static DtStatus DtDfChSdiRx_DoStandbyToRun(DtDfCh*);
static DtStatus DtDfChSdiRx_DoStandbyToRun_Quadlink(DtDfChSdiRx*);
static DtFuncOpMode DtDfChSdiRx_FindHighestOpModeRequested(const DtDfChSdiRx*);
static DtDfChSdiRxUser* DtDfChSdiRx_FindUser(DtDfChSdiRx*, const DtFileObject*);
static DtStatus DtDfChSdiRx_FreeDmaBuffer(DtDfChSdiRx*);
static DtStatus DtDfChSdiRx_GetDmaProperties(DtDfChSdiRx*, 
                                            UInt32* pCapabilities, Int* pPrefetchSize,
                                            Int* pPcieItfDataWidth, Int* pReorderBufSize);
static DtStatus DtDfChSdiRx_GetSdiStatus(DtDfChSdiRx*, 
                                 Int* pCarrierDetect, Int* pSdiLock, Int* pLineLock,
                                 Int* pValid, Int* pSdiRate, Int* pNumSymsHanc, 
                                 Int* pNumSymsVidVanc, Int* pNumLinesF1, Int* pNumLinesF2,
                                 Int* pIsLevelB, UInt32* pPayloadId, Int* pFramePeriod);
static DtStatus DtDfChSdiRx_GetStreamAlignment(DtDfChSdiRx*, Int* pAlignment);
static DtStatus DtDfChSdiRx_GetUserOperationalMode(DtDfChSdiRx*, 
                                                      const DtFileObject*, DtFuncOpMode*);
static DtStatus DtDfChSdiRx_GetWriteOffset(DtDfChSdiRx*, UInt32* pOffset);
static DtStatus DtDfChSdiRx_Init(DtDfCh*);
static Bool DtDfChSdiRx_IsConfigCompatible(
                                            const DtDfChSdiRx*, const DtDfChSdiRxConfig*);
static DtStatus DtDfChSdiRx_MapDmaBufToUser(DtDfChSdiRx*, const DtFileObject*, 
                               UInt64* pBufferAddr, Int* pBufSize, Int* pMaxLoad, DtVma*);
static DtStatus DtDfChSdiRx_OnDetachLastUser(DtDfCh*);
static void DtDfChSdiRx_OnFormatEvent(DtDf*, Int FrameId, Int SeqNumber, Bool InSync);
static DtStatus DtDfChSdiRx_OpenChildren(DtDfCh*);
static DtStatus DtDfChSdiRx_OpenChildren_Quadlink(DtDfChSdiRx*);

static DtDfChUser* DtDfChSdiRx_OpenUser(DtDfCh*, const DtFileObject*, Bool, const char*);
static DtStatus DtDfChSdiRx_SetOperationalMode(DtDfChSdiRx*, DtFuncOpMode);
static DtStatus DtDfChSdiRx_SetReadOffset(
                                         DtDfChSdiRx*, UInt32 Offset, Bool ForceUpdate);
static DtStatus DtDfChSdiRx_SetUserOperationalMode(DtDfChSdiRx*, 
                                                       const DtFileObject*, DtFuncOpMode);
static DtStatus DtDfChSdiRx_SetUserReadOffset(
                                      DtDfChSdiRx*, const DtFileObject*, UInt32 Offset);
static DtStatus DtDfChSdiRx_Unconfigure(DtDfChSdiRx*);
static DtStatus DtDfChSdiRx_WaitForFmtEvent(DtDfChSdiRx*, const DtFileObject*,
                               Int Timeout, Int* pFrameId, Int* pSeqNumber, Int* pInSync);




// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfChSdiRx - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfChSdiRx_Close(DtDf* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Unregister format event callback
    if (DF_TO_THIS->m_pBcSDIRXF)
        DtBcSDIRXF_UnregisterFormFormatEvent(DF_TO_THIS->m_pBcSDIRXF, pDf);

    // Free out DMA buffer
    DtDfChSdiRx_FreeDmaBuffer(DF_TO_THIS);

    // Let base function perform final clean-up
    DtDfCh_Close((DtDfCh*)pDf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_Lock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Acquires the channel state lock
//
DtStatus DtDfChSdiRx_Lock(DtDfChSdiRx* pDf, int Timeout)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    // Let base implementation do the heavy lifting
    return DtDfCh_Lock((DtDfCh*)pDf, Timeout);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfChSdiRx*  DtDfChSdiRx_Open(DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                  Int Instance, Int Uuid, Bool CreateStub)
{
    DtDfId Id;
    DtDfChOpenParams OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_CHSDIRX_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_CH_INIT_OPEN_PARAMS(OpenParams, DtDfChSdiRx, Id, DT_FUNC_TYPE_CHSDIRX, pPt,
                                                                       CreateStub, pCore);
    // Register function callbacks
    OpenParams.m_DfParams.m_CloseFunc = DtDfChSdiRx_Close;
    OpenParams.m_DfParams.m_OnCloseFileFunc = DtDfCh_OnCloseFile;  // Use default

    OpenParams.m_CloseUserFunc = DtDfChSdiRx_CloseUser;
    OpenParams.m_DoIdleToStandyFunc = DtDfChSdiRx_DoIdleToStandy;
    OpenParams.m_DoRunToStandbyFunc = DtDfChSdiRx_DoRunToStandby;
    OpenParams.m_DoStandbyToIdleFunc = DtDfChSdiRx_DoStandbyToIdle;
    OpenParams.m_DoStandbyToRunFunc = DtDfChSdiRx_DoStandbyToRun;
    OpenParams.m_InitFunc = DtDfChSdiRx_Init;
    OpenParams.m_OnDetachLastUserFunc = DtDfChSdiRx_OnDetachLastUser;
    OpenParams.m_OpenChildrenFunc = DtDfChSdiRx_OpenChildren;
    OpenParams.m_OpenUserFunc = DtDfChSdiRx_OpenUser;


    // Set maximum simultaneous users
    OpenParams.m_MaxNumUsersSupported = MaxNumUsersSupported;

    // Use base function to allocate and perform standard initialization of function data
    return (DtDfChSdiRx*)DtDfCh_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_Lock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Releases the channel state lock.
//
void DtDfChSdiRx_Unlock(DtDfChSdiRx* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    // Let base implementation do the heavy lifting
    DtDfCh_Unlock((DtDfCh*)pDf);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+ DtDfChSdiRx - Public functions - Linux specific +=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#ifdef LINBUILD

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_Mmap -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Performs a mmap of the channels DMA buffer
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_Mmap(DtDfChSdiRx* pDf, const DtFileObject* pFile, DtVma* pVma)
{
    DtStatus Status = DT_STATUS_OK;
    DtDfChSdiRxUser* pUser=NULL;
    Int VmaSize = 0;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    
    // Must have been enabled
    DF_CH_MUST_BE_ENABLED(pDf);

    DT_ASSERT(pFile!=NULL && pVma!=NULL);

    // Must have a DMA buffer to map
    if (!pDf->m_IsConfigured || !pDf->m_DmaBuf.m_pBuffer || pDf->m_DmaBuf.m_Size==0)
        return DT_STATUS_NOT_INITIALISED;
    
    // Find the user and return the user operational mode
    pUser = DtDfChSdiRx_FindUser(pDf, pFile);
    if (pUser == NULL)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: could not find a user for '%p'", pFile);
        return DT_STATUS_NOT_FOUND;
    }

    // Get size of memory to map
    VmaSize = (Int)(pVma->m_pVma->vm_end - pVma->m_pVma->vm_start);
    DtDbgOutDf(AVG, CHSDIRX, pDf, "User '%s' is requesting to mmap %dkB", 
                                                     pUser->m_FriendlyName, VmaSize/1024);

    // Check if the memory already was mapped
    if (pUser->m_pMappedDmaAddr)
    {
        DtDbgOutDf(MIN,CHSDIRX,pDf, "WARNING: already mapped memory for user '%s' to %p",
                                          pUser->m_FriendlyName, pUser->m_pMappedDmaAddr);
        return DT_STATUS_OK;
    }

    // Let DMAC do the heavy lifting
    Status = DtDfChSdiRx_MapDmaBufToUser(pDf, pFile, NULL, NULL, NULL, pVma);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: failed to map %dkB for user '%s'", 
                                                     VmaSize/1024, pUser->m_FriendlyName);
        return Status;
    }
    DtDbgOutDf(MIN, CHSDIRX, pDf, "Mapped %d-kB for user '%s' to %p", VmaSize/1024, 
                                          pUser->m_FriendlyName, pUser->m_pMappedDmaAddr);
    return DT_STATUS_OK;
}

#endif // #ifdef LINBUILD

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfChSdiRx - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_AllocateDmaBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Allocates a DMA buffer with as minimum size the requested size. Also registers the 
// buffer with the CDMAC.
//
// Note: caller should hold the state lock.
// 
DtStatus DtDfChSdiRx_AllocateDmaBuffer(DtDfChSdiRx* pDf, Int ReqMinSize)
{
    DtStatus Status=DT_STATUS_OK;
    UInt32 DmaCapabilities=0;
    Int PrefetchSize=0, PcieItfDataWidth=0, ReorderBufSize=0;
    Int MinDmaBufSize=0, AllocSize=0;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    DtDbgOutDf(MAX, CHSDIRX, pDf, "ENTER");

    // Cannot allocate twice
    if (pDf->m_DmaBuf.m_pBuffer || pDf->m_DmaBuf.m_pPageList)
        return DT_STATUS_FAIL;
    // Must have a CDMAC
    if (!pDf->m_pBcCDMAC)
        return DT_STATUS_NOT_INITIALISED;

    // Get DMA properties and compute minimum DMA buffer size.
    RET_ON_ERR(DtBcCDMAC_GetProperties(pDf->m_pBcCDMAC, &DmaCapabilities, 
                                                         &PrefetchSize, &PcieItfDataWidth,
                                                         &ReorderBufSize));
    MinDmaBufSize = PageSize*PrefetchSize;
    if (ReqMinSize<MinDmaBufSize || ReqMinSize>MaxDmaBufferSize)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, 
            "ERROR: %d is an invalid size (valid range: %d..%d)",
                                             ReqMinSize, MinDmaBufSize, MaxDmaBufferSize);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Ensure our buffer size will be both a multiple of pages and of the pre-fetch size
    AllocSize = ((ReqMinSize + MinDmaBufSize - 1) / MinDmaBufSize) * MinDmaBufSize;

    // Allocated our kernel buffer
    pDf->m_DmaBuf.m_pBuffer = (UInt8*)DtMemAllocPoolLarge(
                                             DtPoolNonPaged, AllocSize, 
                                             DF_CHSDIRX_TAG, &pDf->m_DmaBuf.m_pPageList);
    if (!pDf->m_DmaBuf.m_pBuffer)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, 
                             "ERROR: failed to allocate DMA buffer (size=%d)", AllocSize);
        return DT_STATUS_OUT_OF_MEMORY;
    }
    pDf->m_DmaBuf.m_Size = AllocSize;
    pDf->m_DmaBuf.m_ReadOffset = 0;
    // One data-word is reserved for buffer-full detection
    DT_ASSERT((PcieItfDataWidth&3) == 0);
    pDf->m_DmaBuf.m_MaxLoad = pDf->m_DmaBuf.m_Size - (PcieItfDataWidth/8);

    DtDbgOutDf(MAX, CHSDIRX, pDf, "Allocated a %d-byte buffer for DMA (Max load=%d)",
                                           pDf->m_DmaBuf.m_Size, pDf->m_DmaBuf.m_MaxLoad);

    // Register DMA buffer with CDMAC
    Status = DtBcCDMAC_AllocateBuffer(pDf->m_pBcCDMAC, DT_CDMAC_DIR_RX, 
                                            pDf->m_DmaBuf.m_pBuffer, pDf->m_DmaBuf.m_Size,
                                            DT_BUFTYPE_KERNEL, pDf->m_DmaBuf.m_pPageList);
    if (!DT_SUCCESS(Status))
    {
        DtDfChSdiRx_FreeDmaBuffer(pDf);
        DtDbgOutDf(ERR,CHSDIRX,pDf, "ERROR: failed to register DMA buffer with CDMAC");
        return Status;
    }
    // Set the initial read offset unconditionally
    DtDfChSdiRx_SetReadOffset(pDf, 0, TRUE);

    DtDbgOutDf(MAX, CHSDIRX, pDf, "EXIT");
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_Attach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Attaches a user to the channel.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_Attach(
    DtDfChSdiRx* pDf,
    const DtFileObject* pFile, 
    const DtExclAccessObject* pObject,
    Bool ReqExclusiveAccess,
    const char* pFriendlyName)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    // Let base implementation do heavy lifting
    return DtDfCh_Attach((DtDfCh*)pDf, pFile, pObject, ReqExclusiveAccess, pFriendlyName);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_CheckAndUpdateReadOffset -.-.-.-.-.-.-.-.-.-.-.-.-
//
// Check the read offset of all active users and update the overall channel read offset 
// to the position reached by all running channels.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_CheckAndUpdateReadOffset(DtDfChSdiRx* pDf, Bool ForceUpdate)
{
    DtStatus Status = DT_STATUS_OK;
    Int i=0, NumUsers=0, MaxLoad=0;
    UInt32 CurWriteOffset=0, NewReadOffset=0;
    
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must have been configured and we need our CDMAC
    if (!pDf->m_IsConfigured || !pDf->m_pBcCDMAC)
        return DT_STATUS_NOT_INITIALISED;

    // Get the current write and read offsets
    Status = DtBcCDMAC_GetRxWriteOffset(pDf->m_pBcCDMAC, &CurWriteOffset);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: failed get DMA write offset");
        return Status; 
    }
    NewReadOffset = pDf->m_DmaBuf.m_ReadOffset;

    // Iterate over users and determine the common read offset (i.e. 'lowest' offset or 
    // offset they have all reached). We do this by computing buffer load for each user; 
    // the user with the highest load is the one furthest behind.
    DtDfCh_LockUsers((DtDfCh*)pDf);
    NumUsers = DtVectorDfChUser_Size(pDf->m_pUsers);
    for (i=0; i<NumUsers; i++)
    {
        Int Load=0;
        const Int DmaBufSize=pDf->m_DmaBuf.m_Size;
        const DtDfChSdiRxUser* pUser = 
                                (const DtDfChSdiRxUser*)DtVectorDfChUser_At(pDf->m_pUsers, i);

        // Skip IDLE or un-configured users
        if (pUser->m_OpMode==DT_FUNC_OPMODE_IDLE || !pUser->m_IsConfigured)
            continue;

        // Compute load for this user and check if it the furthest behind so far
        Load = (Int)((CurWriteOffset+DmaBufSize-pUser->m_DmaReadOffset) % DmaBufSize);
        if (Load > MaxLoad)
        {
            NewReadOffset = pUser->m_DmaReadOffset;
            MaxLoad = Load;
        }
    }
    DtDfCh_UnlockUsers((DtDfCh*)pDf);
    // Set the new read offset
    return DtDfChSdiRx_SetReadOffset(pDf, NewReadOffset, ForceUpdate);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_CloseUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Closes a user object but first releases any resources it may hold.
// 
// Note1: caller should hold the state lock.
// Note2: this function should be from within the context of the user process that mapped 
//        the DMA memory for this user, otherwise DtBcCDMAC_UpmapBufferFromUserSpace 
//        could un-map the address space of a random process.
//
static void DtDfChSdiRx_CloseUserImpl(DtDfChSdiRx*, DtDfChSdiRxUser*, const DtFileObject*);
void DtDfChSdiRx_CloseUser(DtDfCh* pDf, DtDfChUser* pUser, const DtFileObject* pFile)
{
    // Let implementation do the heavy lifting
    DtDfChSdiRx_CloseUserImpl((DtDfChSdiRx*)pDf, (DtDfChSdiRxUser*)pUser, pFile);
}
// Actual implementation
void DtDfChSdiRx_CloseUserImpl(
                          DtDfChSdiRx* pDf, DtDfChSdiRxUser* pUser, const DtFileObject* pFile)
{
    if (!pUser)
        return;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pUser->m_Size == sizeof(DtDfChSdiRxUser));

    // Unmap DMA buffer if it was mapped for this user
    if (pUser->m_pMappedDmaAddr)
    {
        // Make sure we are in the context of the original caller. See remarks in 
        // function header about user process context
        if (DtFileCompare(pFile, &pUser->m_File))
        {
            if (pDf->m_pBcCDMAC)
            {
                DtStatus Status = DtBcCDMAC_UpmapBufferFromUserSpace(
                                                pDf->m_pBcCDMAC, pUser->m_pMappedDmaAddr);
                if (!DT_SUCCESS(Status))
                {
                    DtDbgOutDf(ERR, CHSDIRX, pDf, 
                                        "ERROR: failed to unmap DMA buffer for user '%s'",
                                        pUser->m_FriendlyName);
                }
            }
        }
        else
        {
            DtDbgOutDf(ERR, CHSDIRX, pDf, 
                      "ERROR: cannot unmap memory for user '%s' from the current context",
                      pUser->m_FriendlyName);
        }
        pUser->m_pMappedDmaAddr = NULL;
    }
    // Finally, let base implementation free the user
    DtDfCh_CloseUser((DtDfCh*)pDf, (DtDfChUser*)pUser, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_Configure -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Configures the channel. Typically, called when the first user sets a configuration.
//
// Note: caller should hold the state lock
//
DtStatus DtDfChSdiRx_Configure(DtDfChSdiRx* pDf, const DtDfChSdiRxConfig* pCfg)
{
    DtStatus Status = DT_STATUS_OK;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must have been enabled
    DF_CH_MUST_BE_ENABLED(pDf);

    if (!pCfg)
        return DT_STATUS_INVALID_PARAMETER;

    DtDbgOutDf(MAX, CHSDIRX, pDf, "Applying channel configuration:");
    if (pCfg->m_NumPhysicalPorts == 1)
    {
        DtDbgOutDf(MAX, CHSDIRX, pDf, "- Port: %d", pCfg->m_PhysicalPorts[0]+1);
    }
    else
    {
        DtDbgOutDf(MAX, CHSDIRX, pDf, "- Ports: %d, %d, %d and %d", 
                                                              pCfg->m_PhysicalPorts[0]+1,
                                                              pCfg->m_PhysicalPorts[1]+1,
                                                              pCfg->m_PhysicalPorts[2]+1,
                                                              pCfg->m_PhysicalPorts[3]+1);
    }
    DtDbgOutDf(MAX, CHSDIRX, pDf, "- DmaBuf: minsize=%d", pCfg->m_DmaBuf.m_MinSize);
    DtDbgOutDf(MAX, CHSDIRX, pDf, "- FmtEvt: delay=%dus, itv=%dus, #ints=%d", 
                                                         pCfg->m_FmtEvt.m_IntDelay,
                                                         pCfg->m_FmtEvt.m_IntInterval,
                                                         pCfg->m_FmtEvt.m_NumIntsPerFrame);
    DtDbgOutDf(MAX, CHSDIRX, pDf, "- FrameProps: #lines=%d, #syms=%d|%d, rate=%d",
                                                      pCfg->m_FrameProps.m_NumLines,
                                                      pCfg->m_FrameProps.m_NumSymsHanc,
                                                      pCfg->m_FrameProps.m_NumSymsVidVanc,
                                                      pCfg->m_FrameProps.m_SdiRate);
    // Cannot configure twice
    if (pDf->m_IsConfigured)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: channel is already configured");
        return DT_STATUS_CONFIG_ERROR;
    }

    // Allocate a DMA buffer
    Status = DtDfChSdiRx_AllocateDmaBuffer(pDf, pCfg->m_DmaBuf.m_MinSize);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR,CHSDIRX,pDf, "ERROR: failed to allocate a %d-byte DMA buffer",
                                                                pCfg->m_DmaBuf.m_MinSize);
        return Status;
    }

    // Configure SdiRxFmtSimple block
    Status = DtBcSDIRXF_SetFmtEventTiming(pDf->m_pBcSDIRXF, 
                                                         pCfg->m_FmtEvt.m_IntInterval,
                                                         pCfg->m_FmtEvt.m_IntDelay,
                                                         pCfg->m_FmtEvt.m_NumIntsPerFrame);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, 
                                      "ERROR: failed set format-event-timing for SDIRXF");
        return Status;
    }
    Status = DtBcSDIRXF_SetFrameProperties(pDf->m_pBcSDIRXF, 
                                                   pCfg->m_FrameProps.m_NumSymsHanc,
                                                   pCfg->m_FrameProps.m_NumSymsVidVanc,
                                                   pCfg->m_FrameProps.m_NumLines,
                                                   pCfg->m_FrameProps.m_SdiRate,
                                                   pCfg->m_FrameProps.m_AssumeInterlaced);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: failed set frame properties for SDIRXF");
        return Status;
    }

    // Configure SDIRX function
    Status = DtDfSdiRx_SetSdiRate(pDf->m_pDfSdiRx, pCfg->m_FrameProps.m_SdiRate);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: failed set SDI rate for SDIRX");
        return Status;
    }

    // Configure channels quad-link "extension" if supported
    if (pDf->m_CapQuadlink)
    {
        Status = DtDfChSdiRx_Configure_Quadlink(pDf, pCfg);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR,CHSDIRX,pDf, "ERROR: failed configure quad-link extension");
            return Status;
        }
    }

    // Copy configuration
    pDf->m_Config = *pCfg;
    pDf->m_IsConfigured = TRUE;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_Configure_Quadlink -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Configures the quad-link "extension" of the channel.
// 
// Note: caller should hold the state lock
//
DtStatus DtDfChSdiRx_Configure_Quadlink(
                                      DtDfChSdiRx* pDf, const DtDfChSdiRxConfig* pCfg)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_CapQuadlink);

    // Check parameters
    if (!pCfg)
        return DT_STATUS_INVALID_PARAMETER;
    if (pCfg->m_NumPhysicalPorts!=1 && pCfg->m_NumPhysicalPorts!=4)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf,
                      "ERROR: '%d' is an invalid number of physical ports (valid 1 or 4)",
                      pCfg->m_NumPhysicalPorts);
    }

    // Set the configuration
    pDf->m_Quadlink.m_Config = (pCfg->m_NumPhysicalPorts==4)
                                  ? SDIRXCHAN_QL_CONFIG_QUAD : SDIRXCHAN_QL_CONFIG_SINGLE;

    // Single or quad link?
    if (pDf->m_Quadlink.m_Config == SDIRXCHAN_QL_CONFIG_SINGLE)
    {
        // Switch to bypass path (i.e. bypass the mux)
        RET_ON_ERR(DtBcSWITCH_SetPosition(pDf->m_Quadlink.m_pBcSWITCH_SdiMuxIn, 0, 0));
        RET_ON_ERR(DtBcSWITCH_SetPosition(pDf->m_Quadlink.m_pBcSWITCH_SdiMuxOut, 0, 0));
    }
    else if (pDf->m_Quadlink.m_Config == SDIRXCHAN_QL_CONFIG_QUAD)
    {
        Int i=0;
        UInt8 LinkIn[4];

        // Switch to path with the mux
        RET_ON_ERR(DtBcSWITCH_SetPosition(pDf->m_Quadlink.m_pBcSWITCH_SdiMuxIn, 0, 1));
        RET_ON_ERR(DtBcSWITCH_SetPosition(pDf->m_Quadlink.m_pBcSWITCH_SdiMuxOut, 1, 0));

        // The order of the physical links (buddy ports) will be order we set for the 
        // logical links
        for (i=0; i<4; i++)
            LinkIn[i] = (pCfg->m_PhysicalPorts[i] % 4);

        RET_ON_ERR(DtBcST425LR_SetLinkOrder(pDf->m_Quadlink.m_pBcST425LR, LinkIn));
    }
    else
        DT_ASSERT(FALSE);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_ConfigureUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Applies the configuration for a specific user. Function will fail if the configuration
// is not compatible with the active configuration. If no configuration was applied yet 
// this user's configuration will become the active configuration.
//
// Note: caller should hold the state lock
//
DtStatus DtDfChSdiRx_ConfigureUser(
          DtDfChSdiRx* pDf, const DtFileObject* pFile, const DtDfChSdiRxConfig* pCfg)
{
    DtStatus Status=DT_STATUS_OK;
    DtDfChSdiRxUser* pUser=NULL;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pFile != NULL);
    DT_ASSERT(pCfg != NULL);

    // Must have been enabled
    DF_CH_MUST_BE_ENABLED(pDf);

    // Find the user and return the user operational mode
    pUser = DtDfChSdiRx_FindUser(pDf, pFile);
    if (pUser == NULL)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: could not find a user for '%p'", pFile);
        return DT_STATUS_NOT_FOUND;
    }

    DtDbgOutDf(AVG, CHSDIRX, pDf, "User '%s' is configuring the channel", 
                                                                   pUser->m_FriendlyName);

    // User must be IDLE
    if (pUser->m_OpMode != DT_FUNC_OPMODE_IDLE)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: cannot configure user '%s' when not IDLE",
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
            if (!DtDfChSdiRx_IsConfigCompatible(pDf, pCfg))
            {
                Status = DtDfChSdiRx_Unconfigure(pDf);
                Status = DtDfChSdiRx_Configure(pDf, pCfg);
            }
            else
                Status = DT_STATUS_OK;
        }
        else
        {
            // Multiple user, so this user must request a compatible configuration
            if (!DtDfChSdiRx_IsConfigCompatible(pDf, pCfg))
            {
                DtDbgOutDf(ERR, CHSDIRX, pDf, 
                          "ERROR: user '%s' tries to apply an incompatible configuration",
                          pUser->m_FriendlyName);
                return DT_STATUS_CONFIG_ERROR;
            }
            else
                Status = DT_STATUS_OK;
        }
    }
    else
        Status = DtDfChSdiRx_Configure(pDf, pCfg);

    // Mark user as configured upon success
    if (DT_SUCCESS(Status))
        pUser->m_IsConfigured = TRUE;

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_Detach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Detaches a user from the channel.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_Detach(
           DtDfChSdiRx* pDf, const DtExclAccessObject* pObject, const DtFileObject* pFile)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    // Let base implementation do heavy lifting
    return DtDfCh_Detach((DtDfCh*)pDf, pObject, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_DoIdleToStandy -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Executes an operational mode transition from IDLE to STANDBY.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_DoIdleToStandy(DtDfCh* pDfBase)
{
    DtDfChSdiRx* pDf = (DtDfChSdiRx*)pDfBase;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must be IDLE
    DT_ASSERT(pDf->m_OpMode == DT_FUNC_OPMODE_IDLE);

    // Make the data flow up to the formatter
    RET_ON_ERR(DtBcCDMAC_SetOperationalMode(pDf->m_pBcCDMAC, DT_BLOCK_OPMODE_IDLE));
    RET_ON_ERR(DtBcBURSTFIFO_SetOperationalMode(
                                              pDf->m_pBcBURSTFIFO, DT_BLOCK_OPMODE_IDLE));
    RET_ON_ERR(DtBcSDIRXF_SetOperationalMode(pDf->m_pBcSDIRXF, DT_BLOCK_OPMODE_STANDBY));
    if (pDf->m_CapQuadlink)
        RET_ON_ERR(DtDfChSdiRx_DoIdleToStandy_Quadlink(pDf));
    RET_ON_ERR(DtDfSdiRx_SetOperationalMode(pDf->m_pDfSdiRx, DT_FUNC_OPMODE_RUN));

    // We are now in STANDBY
    pDf->m_OpMode = DT_FUNC_OPMODE_STANDBY;
    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_DoIdleToStandy_Quadlink -.-.-.-.-.-.-.-.-.-.-.-.-
//
// Executes an operational mode transition from IDLE to STANDBY for the quad-link 
// "extension".
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_DoIdleToStandy_Quadlink(DtDfChSdiRx* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_CapQuadlink);

    RET_ON_ERR(DtBcSWITCH_SetOperationalMode(
                              pDf->m_Quadlink.m_pBcSWITCH_SdiMuxIn, DT_BLOCK_OPMODE_RUN));
    
    // The MUX and LINK REORDER only partake in a multi-link configuration
    if (pDf->m_Quadlink.m_Config == SDIRXCHAN_QL_CONFIG_QUAD)
    {
        RET_ON_ERR(DtBcSDIMUX12G_SetOperationalMode(
                                    pDf->m_Quadlink.m_pBcSDIMUX12G, DT_BLOCK_OPMODE_RUN));
        RET_ON_ERR(DtBcST425LR_SetOperationalMode(
                                      pDf->m_Quadlink.m_pBcST425LR, DT_BLOCK_OPMODE_RUN));
    }
    else
    {
        RET_ON_ERR(DtBcSDIMUX12G_SetOperationalMode(
                                   pDf->m_Quadlink.m_pBcSDIMUX12G, DT_BLOCK_OPMODE_IDLE));
        RET_ON_ERR(DtBcST425LR_SetOperationalMode(
                                     pDf->m_Quadlink.m_pBcST425LR, DT_BLOCK_OPMODE_IDLE));
    }
    RET_ON_ERR(DtBcSWITCH_SetOperationalMode(
                             pDf->m_Quadlink.m_pBcSWITCH_SdiMuxOut, DT_BLOCK_OPMODE_RUN));

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_DoRunToStandby -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Executes an operational mode transition from RUN to STANDBY.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_DoRunToStandby(DtDfCh* pDfBase)
{
    DtDfChSdiRx* pDf = (DtDfChSdiRx*)pDfBase;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must be in RUN
    DT_ASSERT(DF_TO_THIS->m_OpMode == DT_FUNC_OPMODE_RUN);

    // Make data flow up to the formatter
    RET_ON_ERR(DtDfSdiRx_SetOperationalMode(pDf->m_pDfSdiRx, DT_FUNC_OPMODE_RUN));
    if (pDf->m_CapQuadlink)
        RET_ON_ERR(DtDfChSdiRx_DoRunToStandby_Quadlink(pDf));
    RET_ON_ERR(DtBcSDIRXF_SetOperationalMode(pDf->m_pBcSDIRXF, DT_BLOCK_OPMODE_STANDBY));
    RET_ON_ERR(DtBcBURSTFIFO_SetOperationalMode(
                                              pDf->m_pBcBURSTFIFO, DT_BLOCK_OPMODE_IDLE));
    RET_ON_ERR(DtBcCDMAC_SetOperationalMode(pDf->m_pBcCDMAC, DT_BLOCK_OPMODE_IDLE));

    // We are now in STANDBY
    pDf->m_OpMode = DT_FUNC_OPMODE_STANDBY;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_DoRunToStandby_Quadlink -.-.-.-.-.-.-.-.-.-.-.-.-
//
// Executes an operational mode transition from RUN to STANDBY for the quad-link 
// "extension".
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_DoRunToStandby_Quadlink(DtDfChSdiRx* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_CapQuadlink);

    RET_ON_ERR(DtBcSWITCH_SetOperationalMode(
                              pDf->m_Quadlink.m_pBcSWITCH_SdiMuxIn, DT_BLOCK_OPMODE_RUN));
    
    // The MUX and LINK REORDER only partake in a multi-link configuration
    if (pDf->m_Quadlink.m_Config == SDIRXCHAN_QL_CONFIG_QUAD)
    {
        RET_ON_ERR(DtBcSDIMUX12G_SetOperationalMode(
                                    pDf->m_Quadlink.m_pBcSDIMUX12G, DT_BLOCK_OPMODE_RUN));
        RET_ON_ERR(DtBcST425LR_SetOperationalMode(
                                      pDf->m_Quadlink.m_pBcST425LR, DT_BLOCK_OPMODE_RUN));
    }
    else
    {
        RET_ON_ERR(DtBcSDIMUX12G_SetOperationalMode(
                                   pDf->m_Quadlink.m_pBcSDIMUX12G, DT_BLOCK_OPMODE_IDLE));
        RET_ON_ERR(DtBcST425LR_SetOperationalMode(
                                     pDf->m_Quadlink.m_pBcST425LR, DT_BLOCK_OPMODE_IDLE));
    }
    RET_ON_ERR(DtBcSWITCH_SetOperationalMode(
                             pDf->m_Quadlink.m_pBcSWITCH_SdiMuxOut, DT_BLOCK_OPMODE_RUN));

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_DoStandbyToIdle -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// 
// Executes an operational mode transition from STANDBY to IDLE.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_DoStandbyToIdle(DtDfCh* pDfBase)
{
    DtDfChSdiRx* pDf = (DtDfChSdiRx*)pDfBase;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must be in STANDBY
    DT_ASSERT(pDf->m_OpMode == DT_FUNC_OPMODE_STANDBY);

    RET_ON_ERR(DtDfSdiRx_SetOperationalMode(pDf->m_pDfSdiRx, DT_FUNC_OPMODE_IDLE));
    if (pDf->m_CapQuadlink)
        RET_ON_ERR(DtDfChSdiRx_DoStandbyToIdle_Quadlink(pDf));
    RET_ON_ERR(DtBcSDIRXF_SetOperationalMode(pDf->m_pBcSDIRXF, DT_BLOCK_OPMODE_IDLE));
    RET_ON_ERR(DtBcBURSTFIFO_SetOperationalMode(pDf->m_pBcBURSTFIFO, DT_BLOCK_OPMODE_IDLE));
    RET_ON_ERR(DtBcCDMAC_SetOperationalMode(pDf->m_pBcCDMAC, DT_BLOCK_OPMODE_IDLE));

    // We are now IDLE
    pDf->m_OpMode = DT_FUNC_OPMODE_IDLE;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_DoStandbyToIdle_Quadlink -.-.-.-.-.-.-.-.-.-.-.-.-
//
// Executes an operational mode transition from STANBY to IDLE for the quad-link 
// "extension".
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_DoStandbyToIdle_Quadlink(DtDfChSdiRx* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_CapQuadlink);

    RET_ON_ERR(DtBcSWITCH_SetOperationalMode(
                             pDf->m_Quadlink.m_pBcSWITCH_SdiMuxIn, DT_BLOCK_OPMODE_IDLE));
    
    RET_ON_ERR(DtBcSDIMUX12G_SetOperationalMode(
                                   pDf->m_Quadlink.m_pBcSDIMUX12G, DT_BLOCK_OPMODE_IDLE));
    RET_ON_ERR(DtBcST425LR_SetOperationalMode(
                                     pDf->m_Quadlink.m_pBcST425LR, DT_BLOCK_OPMODE_IDLE));
    RET_ON_ERR(DtBcSWITCH_SetOperationalMode(
                            pDf->m_Quadlink.m_pBcSWITCH_SdiMuxOut, DT_BLOCK_OPMODE_IDLE));
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_DoStandbyToRun -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Executes an operational mode transition from STANDY to RUN.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_DoStandbyToRun(DtDfCh* pDfBase)
{
    DtDfChSdiRx* pDf = (DtDfChSdiRx*)pDfBase;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must be in STANDBY
    DT_ASSERT(pDf->m_OpMode == DT_FUNC_OPMODE_STANDBY);

    // Make the data flow up to the formatter
    RET_ON_ERR(DtBcCDMAC_SetOperationalMode(pDf->m_pBcCDMAC, DT_BLOCK_OPMODE_RUN));
    RET_ON_ERR(DtBcCDMAC_ClearReorderBufMinMax(pDf->m_pBcCDMAC));
    RET_ON_ERR(DtBcBURSTFIFO_SetOperationalMode(pDf->m_pBcBURSTFIFO,DT_BLOCK_OPMODE_RUN));
    RET_ON_ERR(DtBcBURSTFIFO_ClearFifoMax(pDf->m_pBcBURSTFIFO, TRUE, TRUE));
    if (pDf->m_CapQuadlink)
        RET_ON_ERR(DtDfChSdiRx_DoStandbyToRun_Quadlink(pDf));
    RET_ON_ERR(DtBcSDIRXF_SetOperationalMode(pDf->m_pBcSDIRXF, DT_BLOCK_OPMODE_RUN));
    RET_ON_ERR(DtDfSdiRx_SetOperationalMode(pDf->m_pDfSdiRx, DT_FUNC_OPMODE_RUN));

    // We are now in RUN
    pDf->m_OpMode = DT_FUNC_OPMODE_RUN;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_DoStandbyToRun_Quadlink -.-.-.-.-.-.-.-.-.-.-.-.-
//
// Executes an operational mode transition from STANDY to RUN for the quad-link 
// "extension".
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_DoStandbyToRun_Quadlink(DtDfChSdiRx* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_CapQuadlink);

     RET_ON_ERR(DtBcSWITCH_SetOperationalMode(
                              pDf->m_Quadlink.m_pBcSWITCH_SdiMuxOut, DT_BLOCK_OPMODE_RUN));
    
    // The MUX and LINK REORDER only partake in a multi-link configuration
    if (pDf->m_Quadlink.m_Config == SDIRXCHAN_QL_CONFIG_QUAD)
    {
        RET_ON_ERR(DtBcSDIMUX12G_SetOperationalMode(
                                     pDf->m_Quadlink.m_pBcSDIMUX12G, DT_BLOCK_OPMODE_RUN));
        RET_ON_ERR(DtBcST425LR_SetOperationalMode(
                                       pDf->m_Quadlink.m_pBcST425LR, DT_BLOCK_OPMODE_RUN));
    }
    else
    {
        RET_ON_ERR(DtBcSDIMUX12G_SetOperationalMode(
                                    pDf->m_Quadlink.m_pBcSDIMUX12G, DT_BLOCK_OPMODE_IDLE));
        RET_ON_ERR(DtBcST425LR_SetOperationalMode(
                                      pDf->m_Quadlink.m_pBcST425LR, DT_BLOCK_OPMODE_IDLE));
    }
    RET_ON_ERR(DtBcSWITCH_SetOperationalMode(
                             pDf->m_Quadlink.m_pBcSWITCH_SdiMuxIn, DT_BLOCK_OPMODE_RUN));

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_FindHighestOpModeRequested -.-.-.-.-.-.-.-.-.-.-.-.
//
// Find the 'highest' (RUN > STANDBY > IDLE) operational mode requested by the users.
//
// Note: caller should hold the state lock.
//
DtFuncOpMode DtDfChSdiRx_FindHighestOpModeRequested(const DtDfChSdiRx* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    // Let base implementation do the heavy lifting 
    return DtDfCh_FindHighestOpModeRequested((DtDfCh*)pDf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_FindUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Find the user associated with the file handle. 
// 
// Note: caller should hold the state lock.
//
DtDfChSdiRxUser* DtDfChSdiRx_FindUser(DtDfChSdiRx* pDf, const DtFileObject* pFile)
{
    // Let base implementation do the heavy lifting 
    return (DtDfChSdiRxUser*)DtDfCh_FindUser((DtDfCh*)pDf, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_FreeDmaBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Frees the DMA buffer and unregisters the buffer with the CDMAC.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_FreeDmaBuffer(DtDfChSdiRx* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    if (!pDf->m_DmaBuf.m_pBuffer)
        return DT_STATUS_OK;

    DtDbgOutDf(MIN, CHSDIRX, pDf, "Freeing DMA buffer");

    // De-register DMA buffer from CDMAC
    if (pDf->m_pBcCDMAC)
        DtBcCDMAC_FreeBuffer(pDf->m_pBcCDMAC);

    // Free the buffer
    DtMemFreePoolLarge(pDf->m_DmaBuf.m_pBuffer, DF_CHSDIRX_TAG, pDf->m_DmaBuf.m_pPageList);
    pDf->m_DmaBuf.m_pBuffer = NULL;
    pDf->m_DmaBuf.m_Size = 0;
    pDf->m_DmaBuf.m_ReadOffset = 0;
    pDf->m_DmaBuf.m_MaxLoad = 0;
    pDf->m_DmaBuf.m_pPageList = NULL;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_GetDmaProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the DMA properties for this channel.
//
// Note: caller should hold the state lock
//
DtStatus DtDfChSdiRx_GetDmaProperties(DtDfChSdiRx* pDf, 
                                             UInt32* pCapabilities, Int* pPrefetchSize,
                                             Int* pPcieItfDataWidth, Int* pReorderBufSize)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pCapabilities!=NULL && pPrefetchSize!=NULL 
                                     && pPcieItfDataWidth!=NULL && pReorderBufSize!=NULL);

    // Do we have a CDMAC?
    if (pDf->m_pBcCDMAC == NULL)
        return DT_STATUS_NOT_INITIALISED;

    return DtBcCDMAC_GetProperties(pDf->m_pBcCDMAC, pCapabilities, pPrefetchSize, 
                                                      pPcieItfDataWidth, pReorderBufSize);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_GetSdiStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the SDI status (i.e. detected #lines, field sizes, signal lock status, etc) for 
// the channel. 
//
// Note: caller should hold the state lock.
// 
DtStatus DtDfChSdiRx_GetSdiStatus(DtDfChSdiRx* pDf, 
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

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_GetStreamAlignment -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get the required steam alignment for the channel.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_GetStreamAlignment(DtDfChSdiRx* pDf, Int* pStreamAlignment)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pStreamAlignment != NULL);

    // Do we have a SDIRXF?
    if (pDf->m_pBcSDIRXF == NULL)
        return DT_STATUS_NOT_INITIALISED;

    return DtBcSDIRXF_GetStreamAlignment(pDf->m_pBcSDIRXF, pStreamAlignment);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_GetUserOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get the current operational mode for the user associated with the file.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_GetUserOperationalMode(
                     DtDfChSdiRx* pDf, const DtFileObject* pFile, DtFuncOpMode* pOpMode)
{
    DtDfChSdiRxUser* pUser=NULL;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pFile != NULL);
    DT_ASSERT(pOpMode != NULL);
      
    // Find the user and return the user operational mode
    pUser = DtDfChSdiRx_FindUser(pDf, pFile);
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


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_GetWriteOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Gets the current offset for the DMA write pointer. The channel must have been 
// configured.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_GetWriteOffset(DtDfChSdiRx* pDf, UInt32* pOffset)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // We must be configured
    if (!pDf->m_IsConfigured || !pDf->m_pBcCDMAC)
        return DT_STATUS_NOT_INITIALISED;
    if (!pOffset)
        return DT_STATUS_INVALID_PARAMETER;

    return DtBcCDMAC_GetRxWriteOffset(pDf->m_pBcCDMAC, pOffset);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initializes the channel's resources.
//
DtStatus DtDfChSdiRx_Init(DtDfCh* pDf)
{
    DtIoCaps IoCaps;
    DtStatus Status=DT_STATUS_OK;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    DtDbgOutDf(MAX, CHSDIRX, pDf, "ENTER");

    // Get hardware capabilities before we call the DtDfCh_Init. Need the capabilities in 
    // the DtDfChSdiRx_OpenChildren called by DtDfCh_Init
    Status = DtCore_PT_GetIoCaps(pDf->m_pPt, &IoCaps);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: failed get ports io-capabilities");
        return Status;
    }
    DF_TO_THIS->m_CapScale12To3G = DtIoCapsHasCap(&IoCaps, DT_IOCAP_SCALE_12GTO3G);
    DF_TO_THIS->m_CapQuadlink = DtIoCapsHasCap(&IoCaps, DT_IOCAP_QUADLINK);

    // Call base implementation
    RET_ON_ERR(DtDfCh_Init(pDf));

    // Not configured yet
    DF_TO_THIS->m_IsConfigured = FALSE;

    // Initialize format event data and register our callback
    DtSpinLockInit(&DF_TO_THIS->m_FmtEvent.m_SpinLock);

    DF_TO_THIS->m_FmtEvent.m_FrameId = 0;
    DF_TO_THIS->m_FmtEvent.m_SeqNumber = 0;
    DF_TO_THIS->m_FmtEvent.m_InSync = FALSE;

    if (DF_TO_THIS->m_pBcSDIRXF)
    {
        Status = DtBcSDIRXF_RegisterForFormatEvent(
                          DF_TO_THIS->m_pBcSDIRXF, DtDfChSdiRx_OnFormatEvent, (DtDf*)pDf);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: failed to register for format events");
            return Status;
        }
    }

    DtDbgOutDf(MAX, CHSDIRX, pDf, "EXIT");

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_IsConfigCompatible -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Check if the given channel configuration is compatible with the already applied 
// configuration. Basically, checks all configuration settings are the same with 
// exception of the DMA buffer pointer and address, which are unique by definition.
// 
// Note: caller should hold the state lock.
//
Bool DtDfChSdiRx_IsConfigCompatible(const DtDfChSdiRx* pDf, const DtDfChSdiRxConfig* pCfg)
{
    Int i=0;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    if (pCfg == NULL)               return FALSE; // Must have something to compare
    if (!pDf->m_IsConfigured)       return TRUE;  // No configuration yet, so any will do

    if (pCfg->m_NumPhysicalPorts != pDf->m_Config.m_NumPhysicalPorts)
        return FALSE;
    DT_ASSERT(pCfg->m_NumPhysicalPorts>0 && pCfg->m_NumPhysicalPorts<=4);
    for (i=0; i<pCfg->m_NumPhysicalPorts; i++)
    {
        if (pCfg->m_PhysicalPorts[i] != pDf->m_Config.m_PhysicalPorts[i])
            return FALSE;
    }

    if (pCfg->m_DmaBuf.m_MinSize != pDf->m_Config.m_DmaBuf.m_MinSize)
        return FALSE;

    if (   pCfg->m_FmtEvt.m_IntDelay!=pDf->m_Config.m_FmtEvt.m_IntDelay
        || pCfg->m_FmtEvt.m_IntInterval!=pDf->m_Config.m_FmtEvt.m_IntInterval
        || pCfg->m_FmtEvt.m_NumIntsPerFrame !=pDf->m_Config.m_FmtEvt.m_NumIntsPerFrame)
        return FALSE;

    if (   pCfg->m_FrameProps.m_AssumeInterlaced
                                           !=pDf->m_Config.m_FrameProps.m_AssumeInterlaced
        || pCfg->m_FrameProps.m_NumLines!=pDf->m_Config.m_FrameProps.m_NumLines
        || pCfg->m_FrameProps.m_NumSymsHanc!=pDf->m_Config.m_FrameProps.m_NumSymsHanc
        || pCfg->m_FrameProps.m_NumSymsVidVanc
                                             !=pDf->m_Config.m_FrameProps.m_NumSymsVidVanc
        || pCfg->m_FrameProps.m_SdiRate!=pDf->m_Config.m_FrameProps.m_SdiRate)
        return FALSE;

    return TRUE;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_MapDmaBufToUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Map the DMA buffer to the process memory space of the calling user. 
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_MapDmaBufToUser(
    DtDfChSdiRx* pDf,
    const DtFileObject* pFile,
    UInt64* pBufferAddr, 
    Int* pBufSize, 
    Int* pMaxLoad,
    DtVma* pVma)                    // Only used for Linux on mmap call
{
    DtStatus Status=DT_STATUS_OK;
    DtDfChSdiRxUser* pUser=NULL;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pFile != NULL);

    // Must have a DMA buffer to map
    if (!pDf->m_IsConfigured || !pDf->m_DmaBuf.m_pBuffer || pDf->m_DmaBuf.m_Size==0)
        return DT_STATUS_NOT_INITIALISED;
    // We need our CDMAC
    if (!pDf->m_pBcCDMAC)
        return DT_STATUS_NOT_INITIALISED;

    // Find the user and return the user operational mode
    pUser = DtDfChSdiRx_FindUser(pDf, pFile);
    if (pUser == NULL)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: could not find a user for '%p'", pFile);
        return DT_STATUS_NOT_FOUND;
    }
    
    // Did we already map the buffer address?
    Status = DT_STATUS_OK;
    if (!pUser->m_pMappedDmaAddr)
    {
        // Ask CDMAC to map the DMA buffer to user space
        Int Dir=0, Size=0;
        UInt64 UserSpaceAddress=0;
        Status = DtBcCDMAC_MapBufferToUserSpace(
                                   pDf->m_pBcCDMAC, &Dir, &Size, &UserSpaceAddress, pVma);
        // For Linux, DtBcCDMAC_MapBufferToUserSpace fails with re-queue error if mmap 
        // has not been called first to actually map the memory
    #ifdef LINBUILD
        if (Status == DT_STATUS_REQUEUE)
            DtDbgOutDf(MIN, CHSDIRX, pDf, "For Linux call first call mmap first");
        else
    #endif // #ifdef LINBUILD
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: failed to map DMA buffer for '%s'",
                                                                   pUser->m_FriendlyName);
            return Status;
        }
        // Set the mapped buffer address
        pUser->m_pMappedDmaAddr = (UInt64*)UserSpaceAddress;
    }
    if (DT_SUCCESS(Status))
        DtDbgOutDf(AVG, CHSDIRX, pDf, "Mapped DMA buffer for user '%s' to %p", 
                                          pUser->m_FriendlyName, pUser->m_pMappedDmaAddr);

    // Set the buffer parameters (if parameters are non-NULL)
    if (pBufferAddr)  *pBufferAddr = (UInt64)pUser->m_pMappedDmaAddr;
    if (pBufSize)     *pBufSize = pDf->m_DmaBuf.m_Size;
    if (pMaxLoad)     *pMaxLoad = pDf->m_DmaBuf.m_MaxLoad;

    return DT_STATUS_OK;

}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_OnDetachLastUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Handler for the detach from last user event.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_OnDetachLastUser(DtDfCh* pDf)
{
    DtStatus Status = DT_STATUS_OK;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    DtDbgOutDf(AVG, CHSDIRX, pDf, "No users left => unconfiguring channel");
    Status = DtDfChSdiRx_Unconfigure(DF_TO_THIS);
    if (!DT_SUCCESS(Status))
        DtDbgOutDf(MIN, CHSDIRX, pDf, "WARN: unconfigure of channel failed");

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_OnFormatEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Handler for format-event events from the SDIRXF block.
// 
// Note: caller should NOT hold state lock as that would result in a potential dead-lock.
//
void DtDfChSdiRx_OnFormatEvent(DtDf* pDf, Int FrameId, Int SeqNumber, Bool InSync)
{
    Int i=0, NumUsers=0;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    if (!pDf) return;

    // Set event data and notify all users
    DtSpinLockAcquire(&DF_TO_THIS->m_FmtEvent.m_SpinLock);
    DF_TO_THIS->m_FmtEvent.m_FrameId = FrameId;
    DF_TO_THIS->m_FmtEvent.m_SeqNumber = SeqNumber;
    DF_TO_THIS->m_FmtEvent.m_InSync = InSync;
    DtSpinLockRelease(&DF_TO_THIS->m_FmtEvent.m_SpinLock);

    // Lock users, so that cannot disappear while looping of teh user list
    DtDfCh_LockUsers((DtDfCh*)pDf);

    NumUsers = DtVectorDfChUser_Size(DF_TO_THIS->m_pUsers);
    for (i=0; i<NumUsers; i++)
    {
        DtDfChSdiRxUser* pUser;
        pUser = (DtDfChSdiRxUser*)DtVectorDfChUser_At(DF_TO_THIS->m_pUsers, i);
        if (!pUser)
            continue;
        DtEventSet(&pUser->m_FmtEvent);
    }
    // Release users lock
    DtDfCh_UnlockUsers((DtDfCh*)pDf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Opens all children associated with this channel.
//
static DtStatus DtDfChSdiRx_OpenChildrenImpl(DtDfChSdiRx*);
DtStatus DtDfChSdiRx_OpenChildren(DtDfCh* pDf)
{
    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    // Let implementation do the actual work
    return DtDfChSdiRx_OpenChildrenImpl(DF_TO_THIS);
}
// Actual implementation
DtStatus DtDfChSdiRx_OpenChildrenImpl(DtDfChSdiRx* pDf)
{
    DtStatus Status = DT_STATUS_OK;

    // List of core children supported by the CHSDIRX function
    const DtDfSupportedChild SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_CDMAC, DT_BC_CDMAC_NAME, 
                                        NULL, (DtObjectBcOrDf**)(&pDf->m_pBcCDMAC), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_BURSTFIFO, DT_BC_BURSTFIFO_NAME, 
                                    NULL, (DtObjectBcOrDf**)(&pDf->m_pBcBURSTFIFO), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SDIRXF, DT_BC_SDIRXF_NAME, 
                                       NULL, (DtObjectBcOrDf**)(&pDf->m_pBcSDIRXF), TRUE},
        { DT_OBJECT_TYPE_DF, DT_FUNC_TYPE_SDIRX, DT_DF_SDIRX_NAME, 
                                        NULL, (DtObjectBcOrDf**)(&pDf->m_pDfSdiRx), TRUE},
    };

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Open core children
    Status = DtDfCh_OpenChildren((DtDfCh*)pDf, SUPPORTED_CHILDREN, 
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: failed load core children");
        return Status;
    }

    // Load quad link children if we support quad-link configurations?
    if (pDf->m_CapQuadlink)
    {
        Status = DtDfChSdiRx_OpenChildren_Quadlink(pDf);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: failed load quad-link children");
            return Status;
        }
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_OpenChildren_Quadlink -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Opens children for the quad-link extension.
//
DtStatus DtDfChSdiRx_OpenChildren_Quadlink(DtDfChSdiRx* pDf)
{
    // List of quad-link related children supported by the CHSDIRX function
    const DtDfSupportedChild SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SWITCH, DT_BC_SWITCH_NAME, "SDI_MUX_IN", 
                         (DtObjectBcOrDf**)(&pDf->m_Quadlink.m_pBcSWITCH_SdiMuxIn), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_ST425LR, DT_BC_ST425LR_NAME, NULL, 
                                 (DtObjectBcOrDf**)(&pDf->m_Quadlink.m_pBcST425LR), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SDIMUX12G, DT_BC_SDIMUX12G_NAME, NULL, 
                               (DtObjectBcOrDf**)(&pDf->m_Quadlink.m_pBcSDIMUX12G), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SWITCH, DT_BC_SWITCH_NAME, "SDI_MUX_OUT", 
                        (DtObjectBcOrDf**)(&pDf->m_Quadlink.m_pBcSWITCH_SdiMuxOut), TRUE},
    };

    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_CapQuadlink);

    // Open core children
    return DtDfCh_OpenChildren((DtDfCh*)pDf, SUPPORTED_CHILDREN, 
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_OpenUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Opens a new user and initializes its members to safe initial defaults.
//
DtDfChUser* DtDfChSdiRx_OpenUser(
    DtDfCh* pDf,
    const DtFileObject* pFile, 
    Bool HasExclAccess, 
    const char* pFriendlyName)
{
    DtStatus Status=DT_STATUS_OK;
    DtDfChSdiRxUser* pUser=NULL;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Let base do the creating
    pUser = (DtDfChSdiRxUser*)DtDfCh_OpenUser(pDf, sizeof(DtDfChSdiRxUser),
                                                     pFile, HasExclAccess, pFriendlyName);
    if (!pUser) return NULL;

    // Initialize DtDfChSdiRxUser specific members. 
    // Note: DtDfCh_OpenUser has initialize base members.
    pUser->m_DmaReadOffset = 0;
    pUser->m_IsConfigured = FALSE;
    pUser->m_pMappedDmaAddr = NULL;

    // Initialize format event as auto reset.
    Status = DtEventInit(&pUser->m_FmtEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, 
                            "ERROR: failed to init format-event (Status=0x%08X)", Status);
        DtDfChSdiRx_CloseUser(pDf, (DtDfChUser*)pUser, pFile);
        return NULL;
    }
    return (DtDfChUser*)pUser;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Applies a new operational mode.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_SetOperationalMode(DtDfChSdiRx* pDf, DtFuncOpMode NewOpMode)
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

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_SetReadOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Sets the channel's DMA read offset.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_SetReadOffset(DtDfChSdiRx* pDf, UInt32 Offset, Bool ForceUpdate)
{
    DtStatus Status=DT_STATUS_OK;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must have CDMAC
    if (!pDf->m_pBcCDMAC)
        return DT_STATUS_NOT_INITIALISED;

    // Check if the offset has changed or if are forced to update
    if (!ForceUpdate && Offset==pDf->m_DmaBuf.m_ReadOffset)
        return DT_STATUS_OK;    // Nothing to update

    Status = DtBcCDMAC_SetRxReadOffset(pDf->m_pBcCDMAC, Offset);
    if (!DT_SUCCESS(Status))
        return Status;
    // Store new read offset
    pDf->m_DmaBuf.m_ReadOffset = Offset;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_SetUserOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set the current operational mode for the user associated with the file.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_SetUserOperationalMode(
                      DtDfChSdiRx* pDf, const DtFileObject* pFile, DtFuncOpMode NewOpMode)
{
    DtStatus Status=DT_STATUS_OK;
    DtDfChSdiRxUser* pUser=NULL;
    DtFuncOpMode OldUserOpMode=DT_FUNC_OPMODE_IDLE, HighestOpMode=DT_FUNC_OPMODE_IDLE;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pFile != NULL);

    DtDbgOutDf(MAX, CHSDIRX, pDf, "ENTER");

    // Must have been enabled
    DF_CH_MUST_BE_ENABLED(pDf);

    // Check for valid mode
    if (NewOpMode!=DT_FUNC_OPMODE_IDLE && NewOpMode!=DT_FUNC_OPMODE_STANDBY 
                                                         && NewOpMode!=DT_FUNC_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Find the user requesting the mode change
    pUser = DtDfChSdiRx_FindUser(pDf, pFile);
    if (pUser == NULL)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: could not find a user for '%p'", pFile);
        return DT_STATUS_NOT_FOUND;
    }

    DtDbgOutDf(MAX, CHSDIRX, pDf, 
                              "User '%s' is requesting to change its op-mode from %d->%d",
                              pUser->m_FriendlyName, pUser->m_OpMode, NewOpMode);

    // Only allowed mode while not configured is IDLE
    if (NewOpMode!=DT_FUNC_OPMODE_IDLE && !pUser->m_IsConfigured)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, 
                 "ERROR: cannot change op-mode to '%d' while user '%s' is not configured",
                 NewOpMode, pUser->m_FriendlyName);
        return DT_STATUS_NOT_INITIALISED;
    }

    // Store the mode requested by this user and than check what the 'highest' mode 
    // requested by all users. If the new mode is the highest than we will apply it
    OldUserOpMode = pUser->m_OpMode;    // Save old mode
    pUser->m_OpMode = NewOpMode;
    HighestOpMode = DtDfChSdiRx_FindHighestOpModeRequested(pDf);
    if (HighestOpMode == NewOpMode)
    {
        Status = DtDfChSdiRx_SetOperationalMode(pDf, NewOpMode);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, CHSDIRX, pDf, 
                                     "ERROR: failed to set op-mode to '%d' for user '%s'",
                                     NewOpMode, pUser->m_FriendlyName);
            // Restore old user mode
            pUser->m_OpMode = OldUserOpMode;
        }
    }
    else
    {
        DtDbgOutDf(MAX, CHSDIRX, pDf, 
                            "Op-mode (%d) requested by user '%s' is not the highest (%d)",
                            NewOpMode, pUser->m_FriendlyName, HighestOpMode);
        Status = DT_STATUS_OK;
    }
    DtDbgOutDf(MAX, CHSDIRX, pDf, "EXIT");

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_SetUserReadOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set the DMA read offset for the user associated with the file handle. After settings 
// the users offset this function will check if the overall channel read offset can be 
// updated as well.
// 
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_SetUserReadOffset(
                             DtDfChSdiRx* pDf, const DtFileObject* pFile, UInt32 Offset)
{
    DtDfChSdiRxUser* pUser=NULL;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pFile != NULL);

    // Must have been enabled
    DF_CH_MUST_BE_ENABLED(pDf);

    // Find the user requesting the mode change
    pUser = DtDfChSdiRx_FindUser(pDf, pFile);
    if (pUser == NULL)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: could not find a user for '%p'", pFile);
        return DT_STATUS_NOT_FOUND;
    }
    // Store user's read offset and update overall channel offset (no forced update)
    pUser->m_DmaReadOffset = Offset;
    return DtDfChSdiRx_CheckAndUpdateReadOffset(pDf, FALSE);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_Unconfigure -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Undoes the configures of the channel. Typically, called when the last user detaches.
//
// Note: caller should hold the state lock.
//
DtStatus DtDfChSdiRx_Unconfigure(DtDfChSdiRx* pDf)
{
    DtStatus Status = DT_STATUS_OK;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    DtDbgOutDf(MAX, CHSDIRX, pDf, "Unconfigurating channel");

    // Any configuration to undo?
    if (!pDf->m_IsConfigured)
    {
        DtDbgOutDf(MIN, CHSDIRX, pDf, "Nothing to unconfigure");
        return DT_STATUS_OK;
    }

    // Channel must be IDLE, so that we can free resources
    Status = DtDfChSdiRx_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE);
    if (!DT_SUCCESS(Status))
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: failed to set channel to IDLE");

    // Free DMA buffer
    if (pDf->m_DmaBuf.m_pBuffer)
    {
        Status = DtDfChSdiRx_FreeDmaBuffer(pDf);
        if (!DT_SUCCESS(Status))
            DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: failed to free DMA buffer");
    }

    // Clear configuration
    DtMemZero(&pDf->m_Config, sizeof(pDf->m_Config));
    pDf->m_IsConfigured = FALSE;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx_WaitForFmtEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Wait for a format event and returns when the event was fired or when the specified 
// timeout period has been reached.
//
// Note: caller SHOULD NOT hold the state lock as that would result in a dead lock.
//
DtStatus DtDfChSdiRx_WaitForFmtEvent(DtDfChSdiRx* pDf, const DtFileObject* pFile,
                                Int Timeout, Int* pFrameId, Int* pSeqNumber, Int* pInSync)
{
    DtStatus Status=DT_STATUS_OK;
    DtDfChSdiRxUser* pUser=NULL;
    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Must have been enabled
    DF_CH_MUST_BE_ENABLED(pDf);

    // Must been configured and have a SDIRXF
    if (!pDf->m_IsConfigured || !pDf->m_pBcSDIRXF)
        return DT_STATUS_NOT_INITIALISED;

    // Check parameters
    if (Timeout<-1 || Timeout>1000)
        return DT_STATUS_INVALID_PARAMETER;
    if (pFrameId== NULL || pSeqNumber==NULL || pInSync==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Lock channel first
    Status = DtDfChSdiRx_Lock(pDf, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: failed to lock channel");
        return Status;
    }

    // Find the user
    pUser = DtDfChSdiRx_FindUser(pDf, pFile);
    if (!pUser)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: could not find a user for '%p'", pFile);
        DtDfChSdiRx_Unlock(pDf);
        return DT_STATUS_NOT_FOUND;
    }

    // User must be configured and NOT IDLE
    if (!pUser->m_IsConfigured || pUser->m_OpMode==DT_FUNC_OPMODE_IDLE)
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: user '%s' is not configured or IDLE",
                                                                   pUser->m_FriendlyName);
        DtDfChSdiRx_Unlock(pDf);
        return DT_STATUS_NOT_INITIALISED;
    }

    // Do not forget to unlock channel, before we start our waiting
    DtDfChSdiRx_Unlock(pDf);

    // Wait for next format-event 
    Status = DtEventWaitUnInt(&pUser->m_FmtEvent, Timeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, pDf, "ERROR: wait on event failure, Status:0x%X",Status);
        return Status;
    }

    // Copy the last formatter event data
    DtSpinLockAcquire(&DF_TO_THIS->m_FmtEvent.m_SpinLock);
    *pFrameId = DF_TO_THIS->m_FmtEvent.m_FrameId;
    *pSeqNumber = DF_TO_THIS->m_FmtEvent.m_SeqNumber;
    *pInSync = (Int)DF_TO_THIS->m_FmtEvent.m_InSync;
    DtSpinLockRelease(&DF_TO_THIS->m_FmtEvent.m_SpinLock);

    return DT_STATUS_OK;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfChSdiRx implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Helper macro to cast stub's DtDfCh member to DtDfChSdiRx
#define IO_STUB  ((DtIoStubDfChSdiRx*)pStub)
#define STUB_DF  ((DtDfChSdiRx*)IO_STUB->m_pDf)

// .-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx - Forwards of private functions -.-.-.-.-.-.-.-.-.
static DtStatus DtIoStubDfChSdiRx_OnCmd(
                                       const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
static DtStatus DtIoStubDfChSdiRx_OnCmd_Attach(const DtIoStubDfChSdiRx*,
                                                     const DtFileObject*,
                                                     const DtExclAccessObject*,
                                                     const DtIoctlChSdiRxCmdAttachInput*);
static DtStatus DtIoStubDfChSdiRx_OnCmd_Configure(const DtIoStubDfChSdiRx*, 
                                                  const DtFileObject*,
                                                  const DtIoctlChSdiRxCmdConfigureInput*);
static DtStatus DtIoStubDfChSdiRx_OnCmd_Detach(const DtIoStubDfChSdiRx*,
                                                     const DtFileObject*,
                                                     const DtExclAccessObject*,
                                                     const DtIoctlChSdiRxCmdDetachInput*);
static DtStatus DtIoStubDfChSdiRx_OnCmd_GetOpMode(const DtIoStubDfChSdiRx*,
                                                       const DtFileObject*,
                                                       DtIoctlChSdiRxCmdGetOpModeOutput*);
static DtStatus DtIoStubDfChSdiRx_OnCmd_GetProps(const DtIoStubDfChSdiRx*,
                                                        DtIoctlChSdiRxCmdGetPropsOutput*);
static DtStatus DtIoStubDfChSdiRx_OnCmd_GetSdiStatus(const DtIoStubDfChSdiRx*,
                                                    DtIoctlChSdiRxCmdGetSdiStatusOutput*);
static DtStatus DtIoStubDfChSdiRx_OnCmd_GetWriteOffset(
                       const DtIoStubDfChSdiRx*, DtIoctlChSdiRxCmdGetWrOffsetOutput*);
static DtStatus DtIoStubDfChSdiRx_OnCmd_MapDmaBufToUser(
                                                 const DtIoStubDfChSdiRx*, 
                                                 const DtFileObject*,
                                                 DtIoctlChSdiRxCmdMapDmaBufToUserOutput*);
static DtStatus DtIoStubDfChSdiRx_OnCmd_SetOpMode(const DtIoStubDfChSdiRx*,
                                                  const DtFileObject*,
                                                  const DtIoctlChSdiRxCmdSetOpModeInput*);
static DtStatus DtIoStubDfChSdiRx_OnCmd_SetReadOffset(const DtIoStubDfChSdiRx*,
                                                const DtFileObject*,
                                                const DtIoctlChSdiRxCmdSetRdOffsetInput*);
static DtStatus DtIoStubDfChSdiRx_OnCmd_WaitForFmtEvent(const DtIoStubDfChSdiRx*,
                                             const DtFileObject*,
                                             const DtIoctlChSdiRxCmdWaitForFmtEventInput*,
                                             DtIoctlChSdiRxCmdWaitForFmtEventOutput*);

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_CHSDIRX;

static const DtIoctlProperties IOSTUB_DF_SDIRX_IOCTLS[] = 
{
    DT_IOCTL_PROPS_CHSDIRX_CMD(DtIoStubDfChSdiRx_OnCmd, NULL, NULL)
};

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfChSdiRx - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// 
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtIoStubDfChSdiRx_Close(DtIoStub* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubDfChSdiRx* DtIoStubDfChSdiRx_Open(DtDf* pDf)
{
    DtIoStubDfChSdiRx* pStub = NULL;
    DtIoStubDfOpenParams OpenParams;

    DF_CH_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialization of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfChSdiRx, pDf, NULL, 
                                                              DtIoStubDfChSdiRx_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_DF_SDIRX_IOCTLS);
    pStub = (DtIoStubDfChSdiRx*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfChSdiRx - Private functions +=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfChSdiRx_OnCmd(
    const DtIoStub* pStub, 
    DtIoStubIoParams* pIoParams, 
    Int* pOutSize)
{
    DtStatus Status=DT_STATUS_OK;
    const DtIoctlChSdiRxCmdInput* pInData=NULL;
    DtIoctlChSdiRxCmdOutput* pOutData=NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_CHSDIRX_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exclusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutIoStubDf(ERR, CHSDIRX, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_ChSdiRxCmd;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_ChSdiRxCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_CHSDIRX_CMD_ATTACH:
        DT_ASSERT(pInData!=NULL);
        Status = DtIoStubDfChSdiRx_OnCmd_Attach(IO_STUB, 
                                          pIoParams->m_pFile, &pIoParams->m_ExclAccessObj,
                                          &pInData->m_Attach);
        break;
    case DT_CHSDIRX_CMD_CONFIGURE:
        DT_ASSERT(pInData!=NULL);
        Status = DtIoStubDfChSdiRx_OnCmd_Configure(IO_STUB,
                                               pIoParams->m_pFile, &pInData->m_Configure);
        break;
    case DT_CHSDIRX_CMD_DETACH:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubDfChSdiRx_OnCmd_Detach(IO_STUB, 
                                          pIoParams->m_pFile, &pIoParams->m_ExclAccessObj,
                                          &pInData->m_Detach);
        break;
    case DT_CHSDIRX_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubDfChSdiRx_OnCmd_GetOpMode(
                              IO_STUB, pIoParams->m_pFile, &pOutData->m_GetOpMode);
        break;
    case DT_CHSDIRX_CMD_GET_PROPS:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubDfChSdiRx_OnCmd_GetProps(IO_STUB, &pOutData->m_GetProps);
        break;
    case DT_CHSDIRX_CMD_GET_SDI_STATUS:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubDfChSdiRx_OnCmd_GetSdiStatus(
                                               IO_STUB, &pOutData->m_GetSdiStatus);
        break;
    case DT_CHSDIRX_CMD_GET_WRITE_OFFSET:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubDfChSdiRx_OnCmd_GetWriteOffset(
                                             IO_STUB, &pOutData->m_GetWriteOffset);
        break;
    case DT_CHSDIRX_CMD_MAP_DMA_BUF_TO_USER:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubDfChSdiRx_OnCmd_MapDmaBufToUser(
                           IO_STUB, pIoParams->m_pFile, &pOutData->m_DmaBufToUser);
        break;
    case DT_CHSDIRX_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubDfChSdiRx_OnCmd_SetOpMode(
                               IO_STUB, pIoParams->m_pFile, &pInData->m_SetOpMode);
        break;
    case DT_CHSDIRX_CMD_SET_READ_OFFSET:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubDfChSdiRx_OnCmd_SetReadOffset(
                           IO_STUB, pIoParams->m_pFile, &pInData->m_SetReadOffset);
        break;
    case DT_CHSDIRX_CMD_WAIT_FOR_FMT_EVENT:
        DT_ASSERT(pInData!=NULL && pOutData!=NULL);
        Status = DtIoStubDfChSdiRx_OnCmd_WaitForFmtEvent(IO_STUB, 
                                                            pIoParams->m_pFile, 
                                                            &pInData->m_WaitForFmtEvent,
                                                            &pOutData->m_WaitForFmtEvent);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_OnCmd_Attach -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfChSdiRx_OnCmd_Attach(
    const DtIoStubDfChSdiRx* pStub,
    const DtFileObject* pFile,
    const DtExclAccessObject* pObject,
    const DtIoctlChSdiRxCmdAttachInput* pInData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));
    DT_ASSERT(pInData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRx_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command.
    Status = DtDfChSdiRx_Attach(STUB_DF, pFile, pObject, 
                                          pInData->m_ReqExclusiveAccess!=0 ? TRUE : FALSE,
                                          pInData->m_FriendlyName);

    // Do not forget to unlock the channel
    DtDfChSdiRx_Unlock(STUB_DF);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_OnCmd_Configure -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfChSdiRx_OnCmd_Configure(
    const DtIoStubDfChSdiRx* pStub, 
    const DtFileObject* pFile,
    const DtIoctlChSdiRxCmdConfigureInput* pInData)
{
    DtStatus Status=DT_STATUS_OK;
    Int i=0;
    DtDfChSdiRxConfig Cfg;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));
    DT_ASSERT(pInData != NULL);

    // Zero configuration
    DtMemZero(&Cfg, sizeof(Cfg));

    // Copy ports that partake in reception of signal
    Cfg.m_NumPhysicalPorts = pInData->m_NumPhysicalPorts;
    for (i=0; i<4; i++)
        Cfg.m_PhysicalPorts[i] = pInData->m_PhysicalPorts[i];

    // Copy DMA configuration
    Cfg.m_DmaBuf.m_MinSize = pInData->m_DmaBuf.m_MinSize;

    // Copy format event properties
    Cfg.m_FmtEvt.m_IntDelay = pInData->m_FmtEvt.m_IntDelay;
    Cfg.m_FmtEvt.m_IntInterval = pInData->m_FmtEvt.m_IntInterval;
    Cfg.m_FmtEvt.m_NumIntsPerFrame = pInData->m_FmtEvt.m_NumIntsPerFrame;

    // Copy frame properties
    Cfg.m_FrameProps.m_AssumeInterlaced = pInData->m_FrameProps.m_AssumeInterlaced;
    Cfg.m_FrameProps.m_NumLines = pInData->m_FrameProps.m_NumLines;
    Cfg.m_FrameProps.m_NumSymsHanc = pInData->m_FrameProps.m_NumSymsHanc;
    Cfg.m_FrameProps.m_NumSymsVidVanc = pInData->m_FrameProps.m_NumSymsVidVanc;
    Cfg.m_FrameProps.m_SdiRate = pInData->m_FrameProps.m_SdiRate;

    // Must lock the channel first
    Status = DtDfChSdiRx_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRx_ConfigureUser(STUB_DF, pFile, &Cfg);

    // Do not forget to unlock the channel
    DtDfChSdiRx_Unlock(STUB_DF);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_OnCmd_Detach -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfChSdiRx_OnCmd_Detach(
    const DtIoStubDfChSdiRx* pStub,
    const DtFileObject* pFile,
    const DtExclAccessObject* pObject,
    const DtIoctlChSdiRxCmdDetachInput* pInData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));
    DT_ASSERT(pInData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRx_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRx_Detach(STUB_DF, pObject, pFile);

     // Do not forget to unlock the channel
    DtDfChSdiRx_Unlock(STUB_DF);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_OnCmd_GetOpMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfChSdiRx_OnCmd_GetOpMode(
    const DtIoStubDfChSdiRx* pStub,
    const DtFileObject* pFile,
    DtIoctlChSdiRxCmdGetOpModeOutput* pOutData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));
    DT_ASSERT(pOutData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRx_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRx_GetUserOperationalMode(
                                      STUB_DF, pFile, (DtFuncOpMode*)&pOutData->m_OpMode);

     // Do not forget to unlock the channel
    DtDfChSdiRx_Unlock(STUB_DF);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_OnCmd_GetProps -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfChSdiRx_OnCmd_GetProps(
    const DtIoStubDfChSdiRx* pStub,
    DtIoctlChSdiRxCmdGetPropsOutput* pOutData)
{
    DtStatus Status = DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));
    DT_ASSERT(pOutData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRx_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Get the DMA properties
    Status = DtDfChSdiRx_GetDmaProperties(STUB_DF,
                     &pOutData->m_Dma.m_Caps, &pOutData->m_Dma.m_PrefetchSize,
                     &pOutData->m_Dma.m_PcieDataWidth, &pOutData->m_Dma.m_ReorderBufSize);
    if (DT_SUCCESS(Status))
    {
        // Get the stream alignment
        Status = DtDfChSdiRx_GetStreamAlignment(STUB_DF, &pOutData->m_StreamAlignment);
    }

    // Do not forget to unlock the channel
    DtDfChSdiRx_Unlock(STUB_DF);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_OnCmd_GetSdiStatus -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfChSdiRx_OnCmd_GetSdiStatus(
    const DtIoStubDfChSdiRx* pStub,
    DtIoctlChSdiRxCmdGetSdiStatusOutput* pOutData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));
    DT_ASSERT(pOutData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRx_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRx_GetSdiStatus(STUB_DF, &pOutData->m_CarrierDetect,
              &pOutData->m_SdiLock, &pOutData->m_LineLock, &pOutData->m_Valid,
              &pOutData->m_SdiRate, &pOutData->m_NumSymsHanc, &pOutData->m_NumSymsVidVanc,
              &pOutData->m_NumLinesF1, &pOutData->m_NumLinesF2, &pOutData->m_IsLevelB,
              &pOutData->m_PayloadId, &pOutData->m_FramePeriod);

     // Do not forget to unlock the channel
    DtDfChSdiRx_Unlock(STUB_DF);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_OnCmd_GetWriteOffset -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfChSdiRx_OnCmd_GetWriteOffset(
    const DtIoStubDfChSdiRx* pStub, 
    DtIoctlChSdiRxCmdGetWrOffsetOutput* pOutData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));
    DT_ASSERT(pOutData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRx_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRx_GetWriteOffset(STUB_DF, &pOutData->m_WriteOffset);

     // Do not forget to unlock the channel
    DtDfChSdiRx_Unlock(STUB_DF);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_OnCmd_MapDmaBufToUser -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfChSdiRx_OnCmd_MapDmaBufToUser(
    const DtIoStubDfChSdiRx* pStub, 
    const DtFileObject* pFile,
    DtIoctlChSdiRxCmdMapDmaBufToUserOutput* pOutData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));
    DT_ASSERT(pOutData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRx_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRx_MapDmaBufToUser(STUB_DF, pFile, 
                                              &pOutData->m_BufferAddr, 
                                              &pOutData->m_BufSize, &pOutData->m_MaxLoad,
                                              NULL /* No vma, only available via mmap */);
    // Do not forget to unlock the channel
    DtDfChSdiRx_Unlock(STUB_DF);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_OnCmd_SetOpMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfChSdiRx_OnCmd_SetOpMode(
    const DtIoStubDfChSdiRx* pStub,
    const DtFileObject* pFile,
    const DtIoctlChSdiRxCmdSetOpModeInput* pInData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));
    DT_ASSERT(pInData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRx_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRx_SetUserOperationalMode(
                                         STUB_DF, pFile, (DtFuncOpMode)pInData->m_OpMode);

    // Do not forget to unlock the channel
    DtDfChSdiRx_Unlock(STUB_DF);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_OnCmd_SetReadOffset -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfChSdiRx_OnCmd_SetReadOffset(
    const DtIoStubDfChSdiRx* pStub,
    const DtFileObject* pFile,
    const DtIoctlChSdiRxCmdSetRdOffsetInput* pInData)
{
    DtStatus Status=DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));
    DT_ASSERT(pInData != NULL);

    // Must lock the channel first
    Status = DtDfChSdiRx_Lock(STUB_DF, StateLockTimeout);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, CHSDIRX, STUB_DF, "ERROR: failed to lock channel");
        return Status;
    }

    // Execute command
    Status = DtDfChSdiRx_SetUserReadOffset(STUB_DF, pFile, pInData->m_ReadOffset);

    // Do not forget to unlock the channel
    DtDfChSdiRx_Unlock(STUB_DF);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx_OnCmd_WaitForFmtEvent -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfChSdiRx_OnCmd_WaitForFmtEvent(
    const DtIoStubDfChSdiRx* pStub,
    const DtFileObject* pFile,
    const DtIoctlChSdiRxCmdWaitForFmtEventInput* pInData,
    DtIoctlChSdiRxCmdWaitForFmtEventOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfChSdiRx));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    // Set data timeout
    return DtDfChSdiRx_WaitForFmtEvent(STUB_DF, pFile, 
                                        pInData->m_Timeout,
                                       &pOutData->m_FrameId, &pOutData->m_SeqNumber, 
                                       &pOutData->m_InSync);

}
