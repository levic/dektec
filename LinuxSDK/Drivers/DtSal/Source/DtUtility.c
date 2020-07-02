//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtUtility.c *#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2016 DekTec Digital Video B.V.
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
#include <DtSal.h>

#ifdef WINBUILD
#include <wdmguid.h>            // Required for GUID in interface queries.
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Debug logging +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Event Logging +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#ifdef WINBUILD
void  DtEvtLogInit(DtEvtLog* pEvtObject, void* pIoObject)
#else
void  DtEvtLogInit(DtEvtLog* pEvtObject, EvtGetMessageFunc* pGetMsg)
#endif
{
#ifdef WINBUILD
    pEvtObject->m_pIoObject = pIoObject;
#else
    pEvtObject->m_pEvtGetMessage = pGetMsg;
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtEvtLogReport -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtEvtLogReport(DtEvtLog* pEvtObject, UInt32 ErrorCode, DtString* pInsert1,
                    DtString* pInsert2, DtString* pInsert3)
{
#ifdef WINBUILD
    UInt  LengthOfInsert1 = 0;
    UInt  LengthOfInsert2 = 0;
    UInt  LengthOfInsert3 = 0;
    UInt8  PacketSize;
    PIO_ERROR_LOG_PACKET  pPacket;

    if (pEvtObject == NULL)
        return;
    
    // Compute length of insertion strings
    if (pInsert1 != NULL)
        LengthOfInsert1 = (pInsert1->Length == 0 ? 0 : pInsert1->Length + sizeof(WCHAR));
    if (pInsert2 != NULL)
        LengthOfInsert2 = (pInsert2->Length == 0 ? 0 : pInsert2->Length + sizeof(WCHAR));
    if (pInsert3 != NULL)
        LengthOfInsert3 = (pInsert3->Length == 0 ? 0 : pInsert3->Length + sizeof(WCHAR));
    
    // Allocate the error-log packet
    PacketSize = sizeof(IO_ERROR_LOG_PACKET) +
                 LengthOfInsert1 + LengthOfInsert2 + LengthOfInsert3;
    pPacket = IoAllocateErrorLogEntry(pEvtObject->m_pIoObject, PacketSize);
    if (pPacket != NULL)
    {
        // Fill in common parts of the packet
        pPacket->RetryCount = 0;            // Allways first time
        pPacket->DumpDataSize = 0;            // No dump data
        pPacket->StringOffset = (USHORT)((PUCHAR) pPacket->DumpData - (PUCHAR) pPacket);
        pPacket->EventCategory = 0;            // ?!
        pPacket->ErrorCode = ErrorCode;
        pPacket->UniqueErrorValue = 0;        // Not used
        pPacket->SequenceNumber = 0;        // Not used
        pPacket->MajorFunctionCode = 0;
        pPacket->FinalStatus = 0;
        pPacket->IoControlCode = 0;

        // Insertion strings
        if (LengthOfInsert1 > 0)
        {
            pPacket->NumberOfStrings = 1;
            RtlCopyMemory((PUCHAR) pPacket->DumpData, pInsert1->Buffer, LengthOfInsert1);
            if (LengthOfInsert2 > 0)
            {
                pPacket->NumberOfStrings = 2;
                RtlCopyMemory((PUCHAR) pPacket->DumpData+LengthOfInsert1,
                              pInsert2->Buffer, LengthOfInsert2);
                if (LengthOfInsert3 > 0)
                {
                    pPacket->NumberOfStrings = 3;
                    RtlCopyMemory((PUCHAR) pPacket->DumpData+LengthOfInsert1+LengthOfInsert2,
                                  pInsert3->Buffer, LengthOfInsert3);
                }
            }
        }

        // Log the message
        IoWriteErrorLogEntry(pPacket);
    }

#else
    static const Int  MAX_MSG_LENGTH = 512;
    const char*  pMsg;
    const char*  pLevel;
    char  MsgBuf[MAX_MSG_LENGTH];
    UInt  MsgBufLen;

    // First check if we have a GetEvtMessage function
    if (pEvtObject == NULL || pEvtObject->m_pEvtGetMessage == NULL)
        return;
    
    // Get message from the project
    if (pEvtObject->m_pEvtGetMessage(ErrorCode, &pLevel, &pMsg))
    {
        // Determine total message string length (with inserts substituted)
        MsgBufLen = strlen(pMsg);
        MsgBufLen += (pInsert1 != NULL ? strlen(pInsert1->m_Buffer) : 0);
        MsgBufLen += (pInsert2 != NULL ? strlen(pInsert2->m_Buffer) : 0);
        MsgBufLen += (pInsert3 != NULL ? strlen(pInsert3->m_Buffer) : 0);
        
        if (MsgBufLen <= (MAX_MSG_LENGTH-1))
        {
            // Substitute inserts in msg string
            snprintf(MsgBuf, MAX_MSG_LENGTH, pMsg,
                                          (pInsert1 != NULL ? pInsert1->m_Buffer : NULL),
                                          (pInsert2 != NULL ? pInsert2->m_Buffer : NULL),
                                          (pInsert3 != NULL ? pInsert3->m_Buffer : NULL));
                   
            // Use printk to write debug message
            printk("%s" DRIVER_NAME ": %s\n", pLevel, MsgBuf);
        }
    }
#endif
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Memory functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMemAllocPoolLarge -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void*  DtMemAllocPoolLarge(DtPoolType Type, UInt Size, UInt32 Tag, DtPageList** ppPageList)
{
#ifdef WINBUILD

    // Windows: always use plain kernel memory
    if(ppPageList != NULL)
        *ppPageList = NULL;    
    
    return DtMemAllocPool(Type, Size, Tag);
#else

    Int  i,j;
    UInt  BufSize;   
    DtPageList*  pPageList = NULL;

    // On request, allocate plain memory
    if (ppPageList == NULL)
        return DtMemAllocPool(Type, Size, Tag);
    
    // Check if we can allocate one contiguous piece of memory 
    if (get_order(Size) < MAX_ORDER )   
    {   
        *ppPageList = NULL;
        return DtMemAllocPool(Type, Size, Tag);    
    }

    // Round up to PAGE_SIZE
    BufSize = Size;
    if (BufSize % PAGE_SIZE != 0)
        BufSize = ((BufSize / PAGE_SIZE) + 1) * PAGE_SIZE;

    // Create a pagelist object
    pPageList = kmalloc(sizeof(DtPageList), GFP_KERNEL);
    if (pPageList == NULL)
    {
        DtDbgOut(ERR, SAL, "Unable to allocate a pagelist for a large kernel buffer.");
        return NULL;
    }

    // Allocate a buffer to store the page pointers of the user/kernel buffer

    // Prevent deleting by DtDeletePageList, act as an OS owned buffer
    // Explicitly use DtMemFreePoolLarge to cleanup this buffer.
    pPageList->m_BufType = DT_BUFTYPE_KERNEL;
    pPageList->m_OwnedByUs = FALSE;   
    pPageList->m_NumPages = BufSize / PAGE_SIZE;
    pPageList->m_pPages = kmalloc(sizeof(struct page*) * pPageList->m_NumPages, 
                                                                              GFP_KERNEL);
    if (pPageList->m_pPages == NULL)
    {  
        kfree(pPageList);
        *ppPageList = NULL;
        DtDbgOut(ERR, SAL, "Unable to allocate pages for a large kernel buffer.");
        return NULL;
    }

    // allocate memory located < 4GB and create our own page table
    for (i = 0; i < pPageList->m_NumPages; i++)
    {
        pPageList->m_pPages[i] = virt_to_page(__get_free_page(GFP_KERNEL | GFP_DMA32));
        if(pPageList->m_pPages[i] == NULL)
        {
            // on error free allocated pages
            for (j=0; j < i; j++)            
                free_page((unsigned long)page_address(pPageList->m_pPages[j]));

            kfree(pPageList->m_pPages);
            kfree(pPageList);
            *ppPageList = NULL;
            DtDbgOut(ERR, SAL, "Unable to map a page for a large kernel buffer.");
            return NULL;
        }
    }

    // map to virtual memory
    pPageList->m_pVirtualKernel = vmap(pPageList->m_pPages, 
                                pPageList->m_NumPages, VM_READ | VM_WRITE, PAGE_KERNEL);

    // on error free allocated memory
    if (pPageList->m_pVirtualKernel == NULL)
    {
        for (i = 0; i < pPageList->m_NumPages; i++)
            free_page((unsigned long)page_address(pPageList->m_pPages[i]));
 
        kfree(pPageList->m_pPages);
        kfree(pPageList);
        *ppPageList = NULL;
        DtDbgOut(ERR, SAL, "Unable to map memory for a large kernel buffer.");
        return NULL;
    }
    
    *ppPageList = pPageList;    
    return pPageList->m_pVirtualKernel;

#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMemFreePoolLarge -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtMemFreePoolLarge(void* pPtr, UInt32 Tag, DtPageList* pPageList)
{
#ifdef WINBUILD
    DtMemFreePool(pPtr, Tag);
#else
    Int  i;
    void*  pPage;

    if (pPageList == NULL)
        return DtMemFreePool(pPtr, Tag);

    DT_ASSERT(pPtr == pPageList->m_pVirtualKernel);    

    // Note: It is not allowed to use vunmap in an interrupt OR
    //       when we hold a spinlock (spin_lock_bh).    
    DT_ASSERT(!in_interrupt());
    vunmap(pPageList->m_pVirtualKernel);

    pPageList->m_pVirtualKernel = NULL; 
    
    // Release pages
    DT_ASSERT(pPageList->m_pPages != NULL);	
    for (i = 0; i < pPageList->m_NumPages; i++) 
    {
        pPage = page_address(pPageList->m_pPages[i]);
        free_page((unsigned long)pPage);
    }
    
    kfree(pPageList->m_pPages);    
    kfree(pPageList);

#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMemAllocPool -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void*  DtMemAllocPool(DtPoolType Type, UInt Size, UInt32 Tag)
{
#ifdef WINBUILD
#ifdef USES_NDIS
    NDIS_STATUS  Status;
    void*  pBuf;
    Status = NdisAllocateMemoryWithTag(&pBuf, Size, Tag);
    if (Status != NDIS_STATUS_SUCCESS)
        return NULL;
    return pBuf;
#else
    POOL_TYPE MsPoolType;
    
    // Determine Microsoft's pool type
    switch (Type)
    {
    case DtPoolNonPaged: MsPoolType = NonPagedPool; break;
    case DtPoolPaged: MsPoolType = PagedPool; break;
    default: return NULL;
    }
    
    return ExAllocatePoolWithTag(MsPoolType, Size, Tag);
#endif // USES_NDIS
#else
    if (in_interrupt())
        return kmalloc(Size, GFP_ATOMIC);
    return kmalloc(Size, GFP_KERNEL);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMemFreePool -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtMemFreePool(void* pPtr, UInt32 Tag)
{
#ifdef WINBUILD
#ifdef USES_NDIS
    NdisFreeMemory(pPtr, 0, 0);

#else
    ExFreePoolWithTag(pPtr, Tag);
#endif // USES_NDIS
#else
    kfree(pPtr);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMemZero -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtMemZero(void* pPtr, UInt Size)
{
#ifdef WINBUILD
#ifdef USES_NDIS
    NdisZeroMemory(pPtr, Size);
#else
    RtlZeroMemory(pPtr, Size);
#endif // USES_NDIS
#else
    memset(pPtr, 0, Size);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMemCopy -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtMemCopy(void* pDest, const void* pSrc, UInt Size)
{
#ifdef WINBUILD
    RtlCopyMemory(pDest, pSrc, Size);
#else
    memcpy(pDest, pSrc, Size);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMemCopyToUserBuf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Pre: For windows, the userbuffer is already locked in kernel space memory
//
void  DtMemCopyToUserBuf(void* pDest, void* pSrc, UInt Size)
{
#ifdef WINBUILD
    RtlCopyMemory(pDest, pSrc, Size);
#else
    Int  Status;
    Status = copy_to_user(pDest, pSrc, Size);
    DT_ASSERT(Status >= 0);
            
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMemCopyFromUserBuf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Pre: For windows, the userbuffer is already locked in kernel space memory
//
void  DtMemCopyFromUserBuf(void* pDest, void* pSrc, UInt Size)
{
#ifdef WINBUILD
    RtlCopyMemory(pDest, pSrc, Size);
#else
    Int  Status;
    Status = copy_from_user(pDest, pSrc, Size);
    DT_ASSERT(Status >= 0);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMemMove -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtMemMove(void* pDest, void* pSrc, UInt Size)
{
#ifdef WINBUILD    
    RtlMoveMemory(pDest, pSrc, Size);
#else
    memcpy(pDest, pSrc, Size);
#endif
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ String/Character functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAnsiCharToUInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt  DtAnsiCharToUInt(char Char)
{
    if (Char>='0' && Char<='9')
        return Char-48;
    if (Char>='A' && Char<='F')
        return Char-55;
    if (Char>='a' && Char<='f')
        return Char-87;
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAnsiCharArrayToUInt64 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt64  DtAnsiCharArrayToUInt64(const char* pUIntString, UInt StrLen, UInt Base)
{
    Int  Index;
    UInt64  Multiplier = 1;
    UInt64  Result = 0;
    for(Index=StrLen-1; Index>=0; Index--)
    {
        if (pUIntString[Index]=='.' || pUIntString[Index]==',')
        {
            // Reset, we skip the fractial part
            Multiplier = 1;
            Result = 0;
            continue;
        }
        
        Result += Multiplier * DtAnsiCharToUInt(pUIntString[Index]);
        
        // We do not wast a 64 bit multiplication...
        if (Index > 0)
            Multiplier *= Base;
    }

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAnsiCharArrayToUInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt  DtAnsiCharArrayToUInt(const char* pUIntString, UInt StrLen, UInt Base)
{
    Int  Index;
    UInt  Multiplier = 1;
    UInt  Result = 0;
    for(Index=StrLen-1; Index>=0; Index--)
    {
        if (pUIntString[Index]=='.' || pUIntString[Index]==',')
        {
            // Reset, we skip the fractial part
            Multiplier = 1;
            Result = 0;
            continue;
        }
         
        Result += Multiplier * DtAnsiCharToUInt(pUIntString[Index]);
        
        // We do not wast a 64 bit multiplication...
        if (Index > 0)
            Multiplier *= Base;
    }

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAnsiCharArrayIsEqual -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtAnsiCharArrayIsEqual(const char* pStr1, const char* pStr2)
{
    if (pStr1 == pStr2)
        return TRUE;
    else if (pStr1==NULL || pStr2==NULL)
        return FALSE;

    while(TRUE)
    {
        // Just compare current character
        if (*pStr1 != *pStr2)
            return FALSE;

        // Check for end of string(s) (current characters are equal...)
        if (*pStr1=='\0' && *pStr2!='\0')
            return FALSE;

        if (*pStr1=='\0')
            break;

        // Else compare next character
        pStr1++;
        pStr2++;
    }

    return TRUE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAnsiCharArrayStartsWith -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtAnsiCharArrayStartsWith(const char* pStr, const char* pStart)
{
    if (pStr==NULL || pStart==NULL)
        return FALSE;

    while(TRUE)
    {
        // Check for end of strings

        // Is end of first string only?
        if (*pStr=='\0' && *pStart!='\0')
            return FALSE;
        
        // Is complete string found?
        if (*pStart == '\0')
            break;

        // Just compare current character
        if (*pStr != *pStart)
            return FALSE;

        // Else compare next character
        pStr++;
        pStart++;
    }
    return TRUE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAnsiCharArrayStrLength -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtAnsiCharArrayStrLength(const char* pStr, Int  MaxLength)
{
    Int  Len = 0;
    if (pStr == NULL)
        return -1;

    for (Len=0; Len<MaxLength; Len++)
    {
        // Reach end of string? 
        if (pStr[Len] == '\0')
            return Len;
    }
    return  -1;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Timing functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtSleep -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtSleep(Int MSec)
{
#ifdef WINBUILD
    //DT_ASSERT((KeGetCurrentIrql() == PASSIVE_LEVEL));
    LARGE_INTEGER  WaitTime;
    WaitTime.QuadPart = -10000 * MSec;
    KeDelayExecutionThread(KernelMode, FALSE, &WaitTime);
#else
    msleep(MSec);
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtWaitBlock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtWaitBlock(Int USec)
{
#ifdef WINBUILD
    KeStallExecutionProcessor(USec);
#else
    udelay(USec);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtGetTickCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Return the the number of ms elapsed since the system start 
//
UInt64  DtGetTickCount()
{
#ifdef WINBUILD
   LARGE_INTEGER TickCount;
   KeQueryTickCount(&TickCount);
   // Convert to ms (NOTE: KeQueryTimeIncrement return number of 100ns in one tick)
   return (UInt64)((TickCount.QuadPart * KeQueryTimeIncrement()) / 10000LL);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
    struct timespec64  TickCount;
    ktime_get_ts64(&TickCount);
#else
    struct timespec  TickCount;
    ktime_get_ts(&TickCount);
#endif
    return (UInt64)DtDivide64(((TickCount.tv_sec)*1000000000LL + (TickCount.tv_nsec)),
                                                                           1000000, NULL);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtGetTickCountUSec -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Return the the number of Us elapsed since the system start 
//
UInt64  DtGetTickCountUSec()
{
#ifdef WINBUILD
    LARGE_INTEGER  Freq;
    LARGE_INTEGER  Counter = KeQueryPerformanceCounter(&Freq);
    return (UInt64)DtDivide64(Counter.QuadPart * 1000000, Freq.QuadPart, NULL);
    //LARGE_INTEGER TickCount;
    //KeQueryTickCount(&TickCount);
    //// Convert to us (NOTE: KeQueryTimeIncrement return number of 100ns in one tick)
    //return (UInt64)((TickCount.QuadPart * KeQueryTimeIncrement()) / 10LL);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
    struct timespec64  TickCount;
    ktime_get_ts64(&TickCount);
#else
    struct timespec  TickCount;
    ktime_get_ts(&TickCount);
#endif
    return (UInt64)DtDivide64(((TickCount.tv_sec)*1000000000LL + (TickCount.tv_nsec)),
                                                                              1000, NULL);
#endif
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Memory (Page) functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtLockUserBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtLockUserBuffer(DtPageList* pPageList, UInt8* pBuffer)
{
#ifdef WINBUILD
    try {
        MmProbeAndLockPages(pPageList->m_pMdl, UserMode, IoModifyAccess);
    } except (EXCEPTION_EXECUTE_HANDLER) {
        return DT_STATUS_OUT_OF_RESOURCES;
    }
    pPageList->m_pVirtualKernel = 
                      MmGetSystemAddressForMdlSafe(pPageList->m_pMdl, NormalPagePriority);
        
    if (pPageList->m_pVirtualKernel == NULL)
    {  
        DtDbgOut(ERR, SAL, "Error getting system address for pagelist MDL: %p", 
                                                                       pPageList->m_pMdl);
        MmUnlockPages(pPageList->m_pMdl);
        return DT_STATUS_OUT_OF_RESOURCES;
    }
#else
    unsigned long Offset = 0;

    // For linux the DtPageList is always locked into memory during creation of the buffer
    // No separate function is needed to lock the buffer
    pPageList->m_pVirtualKernel = vmap(pPageList->m_pPages, pPageList->m_NumPages, 
                                             VM_SHARED | VM_READ | VM_WRITE, PAGE_KERNEL);
    if (pPageList->m_pVirtualKernel == NULL)
        return DT_STATUS_OUT_OF_RESOURCES;

    // vmap uses a page aligned buffer, truncated in DtGetPagesUserBufferAndLock()
    // make shure the virtual kernel pointer points to the actual buffer address.
    Offset = (unsigned long)(pBuffer) % PAGE_SIZE;
    pPageList->m_pVirtualKernel += Offset;

#endif
    
    DtDbgOut(MAX, SAL, "Lock user buffer: pVirtual: %p", pPageList->m_pVirtualKernel);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUnlockUserBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtUnlockUserBuffer(DtPageList* pPageList)
{
#ifdef WINBUILD

    MmUnlockPages(pPageList->m_pMdl);
#else

    unsigned long Truncate;

    // For linux the DtPageList is automatically unlocked when the DtPageList buffer is
    // cleaned up
    // No separate function is needed
    // Note: It is not allowed to use vunmap in an interrupt OR
    //       when we hold a spinlock (spin_lock_bh).
    DT_ASSERT(!in_interrupt());

    // vmap returns a page aligned virtual address.
    // Make shure vunmap also gets this address.
    Truncate = PAGE_SIZE-1;
    pPageList->m_pVirtualKernel = 
                      (UInt8 *)((unsigned long)pPageList->m_pVirtualKernel & (~Truncate));

    vunmap(pPageList->m_pVirtualKernel); 
#endif
    DtDbgOut(MAX, SAL, "Unlock user buffer: pVirtual: %p", pPageList->m_pVirtualKernel);
    return DT_STATUS_OK;
}

#ifdef LINBUILD
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtGetPagesUserBufferAndLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function does not lock the user buffer if the NumPages are not equal
// to the NumPagesRet.
// In that case, the caller must not use the buffer and cleanup everything
//
UInt  DtGetPagesUserBufferAndLock(
    struct task_struct*  pTask, // Task that created the buffer
    void*  pBuffer,             // Pointer to user buffer
    UInt  NumPages,             // Length of buffer in page size
    DtPageList* pPageList)
{
    Int  NumPagesRet;
    NumPagesRet = get_user_pages_fast(
                    (unsigned long)pBuffer,
                    NumPages,           // Length of the buffer in pages
                    1,
                    pPageList->m_pPages// Will contain list of page pointers describing buffer
                    );
    if (NumPagesRet < 0)
    {
        NumPagesRet = 0;
        DtDbgOut(ERR, SAL, "get_user_pages returned %i", NumPagesRet);
    }
    else if (NumPagesRet != NumPages)
    {
        DtDbgOut(ERR, SAL, "Numpages(%i) != NumPagesRet(%i).", NumPages, NumPagesRet);
    }
    return NumPagesRet;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFreePagesUserBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtFreePagesUserBuffer(
    DtPageList* pPageList)
{
    Int i;
    for (i=0; i<pPageList->m_NumPages; i++) 
    {
        if (!PageReserved(pPageList->m_pPages[i]))
            SetPageDirty(pPageList->m_pPages[i]);
        put_page(pPageList->m_pPages[i]);
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtGetPagesKernelBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Pre: pPageList is initialised and page pointers allocated
//
UInt  DtGetPagesKernelBuffer(
    void*  pBuffer,             // Pointer to user buffer
    DtPageList*  pPageList)
{
    Int  i;

    for (i= 0; i<pPageList->m_NumPages; i++)
        pPageList->m_pPages[i] = virt_to_page((UInt8*)pBuffer + i*PAGE_SIZE);
    return pPageList->m_NumPages;
}
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCreatePageList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function creates a list of pages from a virtual kernel/user buffer
//
DtStatus  DtCreatePageList(
    void*  pBuffer, 
    UInt  BufSize,
    UInt  BufType,
    DtPageList*  pPageList)
{
#ifdef WINBUILD
#else
    Int  BytesFirstPage;
#endif
    pPageList->m_BufType = BufType;
    pPageList->m_OwnedByUs = TRUE;
#ifdef WINBUILD    
    pPageList->m_pMdl = IoAllocateMdl(pBuffer, BufSize, FALSE, FALSE, NULL);
    if (pPageList->m_pMdl == NULL)
        return DT_STATUS_OUT_OF_MEMORY;
    
    if (BufType == DT_BUFTYPE_KERNEL)
        MmBuildMdlForNonPagedPool(pPageList->m_pMdl); 
#else
    // Calculate number of pages needed to store the page list.

    
    BytesFirstPage = PAGE_SIZE - (DtPtrToUInt(pBuffer) % PAGE_SIZE);
    if (BytesFirstPage > BufSize)
        BytesFirstPage = BufSize;
    pPageList->m_NumPages = (BufSize - BytesFirstPage) / PAGE_SIZE;
    
    // Take the page boundaries of first and last element into account
    // First Element
    if (BytesFirstPage != 0)
        pPageList->m_NumPages++;
    // Last element
    if ((BufSize-BytesFirstPage)%PAGE_SIZE != 0)
        pPageList->m_NumPages++;
    
    // Allocate a buffer to store the page pointers of the user/kernel buffer
    pPageList->m_pPages = kmalloc(sizeof(struct page*)*pPageList->m_NumPages, GFP_KERNEL);
    if (pPageList->m_pPages == NULL)
        return DT_STATUS_OUT_OF_MEMORY;
    
    // Fill the page table
    if (BufType == DT_BUFTYPE_USER) 
    {   
        UInt NumPagesRet;
        // The pPageList->m_NumPages is not updated by the DtGetPagesUserBufferAndLock
        // and should be equal to the NumPagesRet. If not, we can't use the pages
        // and must fail.
        NumPagesRet = DtGetPagesUserBufferAndLock(current, pBuffer, pPageList->m_NumPages,
                                                                               pPageList);
        if (NumPagesRet != pPageList->m_NumPages)
        {
            // First update the m_NumPages to the actual number of pages the 
            // DtGetUserBufferAndLock returned before cleaning up.
            pPageList->m_NumPages = NumPagesRet;
            if (NumPagesRet != 0)
                DtFreePagesUserBuffer(pPageList);
            kfree(pPageList->m_pPages);
            pPageList->m_pPages = NULL;
            return DT_STATUS_OUT_OF_MEMORY;
        }
    } else
        DtGetPagesKernelBuffer(pBuffer, pPageList);
#endif
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDeletePageList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDeletePageList(
    DtPageList*  pPageList)
{
    // Check if we are the owner. If not, we should not delete the page list
    if (!pPageList->m_OwnedByUs)
    {
        return DT_STATUS_OK;
    }
#ifdef WINBUILD
    if (pPageList->m_pMdl != NULL)
        IoFreeMdl(pPageList->m_pMdl);
    pPageList->m_pMdl = NULL;
#else
    if (pPageList->m_pPages == NULL)
        return DT_STATUS_OK;

    if (pPageList->m_BufType == DT_BUFTYPE_USER)
        DtFreePagesUserBuffer(pPageList);

    kfree(pPageList->m_pPages);
    pPageList->m_pPages = NULL;
#endif
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtGetUserBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Pre: pIoctl->m_pInputBuffer must be of type DtUserBufferPort or DtUserBuffer
//
// UseCopyFunc: TRUE: DtMemCopyToUserBuf/DtMemCopyFromUserBuf must be used to copy the
//                    data to/from user buffer
//             FALSE: Buffer can be accesses directly
//
DtStatus  DtGetUserBuffer(DtIoctlObject* pIoctl, UInt8** pBuffer, Int* pSize,
                                                                         Bool UseCopyFunc)
{
#ifdef WINBUILD
#ifdef  USES_KMDF
    PMDL  pMdl;
    NTSTATUS  NtStatus;
    // Retrieve MDL and buffer from request object
    NtStatus = WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
    if (NtStatus != STATUS_SUCCESS)
    {
        DtDbgOut(ERR, SAL, "WdfRequestRetrieveOutputWdmMdl error: %08x", NtStatus);
        return DT_STATUS_OUT_OF_RESOURCES;
    }
    *pBuffer = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
    if (*pBuffer == NULL)
        return DT_STATUS_OUT_OF_MEMORY;
    *pSize = MmGetMdlByteCount(pMdl);
    return DT_STATUS_OK;
#else
    return DT_STATUS_NOT_SUPPORTED;
#endif // USES_KMDF
#else  // LINBUILD
    pIoctl->m_pContext = NULL;
    if (pIoctl->m_InputBufferSize == sizeof(DtUserBufferPort))
    {
        DtUserBufferPort*  pUserBuf = (DtUserBufferPort*)pIoctl->m_pInputBuffer;
        *pSize = pUserBuf->m_NumBytes;
        *pBuffer = (void*)(UIntPtr)pUserBuf->m_BufferAddr;
    }
    else if (pIoctl->m_InputBufferSize == sizeof(DtUserBuffer))
    {
        DtUserBuffer*  pUserBuf = (DtUserBuffer*)pIoctl->m_pInputBuffer;
        *pSize = pUserBuf->m_NumBytes;
        *pBuffer = (void*)(UIntPtr)pUserBuf->m_BufferAddr;
    }
    else
        return DT_STATUS_INVALID_PARAMETER;
    if (!UseCopyFunc)
    {
        DtStatus  Status;
        DtPageList*  pPageList = DtMemAllocPool(DtPoolNonPaged, sizeof(DtPageList), SAL_TAG);
        if (pPageList == NULL)
            return DT_STATUS_OUT_OF_MEMORY;
        Status = DtCreatePageList(*pBuffer, *pSize, DT_BUFTYPE_USER, pPageList);
        if (!DT_SUCCESS(Status))
        {
            DtMemFreePool(pPageList, SAL_TAG);
            return Status;
        }
        // Lock buffer into kernel memory
        Status = DtLockUserBuffer(pPageList, *pBuffer);
        if (!DT_SUCCESS(Status))
        {
            DtDeletePageList(pPageList);
            DtMemFreePool(pPageList, SAL_TAG);
            return Status;
        }
        *pBuffer = pPageList->m_pVirtualKernel;
        pIoctl->m_pContext = pPageList;
    }
    return DT_STATUS_OK;
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtReleaseUserBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtReleaseUserBuffer(DtIoctlObject* pIoctl, UInt8* pBuffer)
{
#ifdef WINBUILD
    // Nothing to do at this moment
    // Will be done automatically when request object is released
#else
    if (pIoctl->m_pContext != NULL)
    {
        DtPageList*  pPageList = (DtPageList*)pIoctl->m_pContext;
        DtUnlockUserBuffer(pPageList);
        DtDeletePageList(pPageList);
        DtMemFreePool(pPageList, SAL_TAG);
        pIoctl->m_pContext = NULL;
    }
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMacAddress2DtString -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtMacAddress2DtString(UInt8* pMacAddress, DtString* pDtString)
{
    Int i;
    DtString  DtStrTmp;
    DtStringChar  DtStrTmpBuffer[32];
    DT_STRING_DECL(DtStrZero, "0");
    DT_STRING_DECL(DtStrDoubleColon, ":");
    DtStatus  Status = DT_STATUS_OK;

    // Connect DtStrTmpBuffer to DtStrTmp
    DT_STRING_INIT(DtStrTmp, DtStrTmpBuffer, 32);
    for (i=0; i<6; i++)
    {
        Status = DtStringUIntegerToDtString(&DtStrTmp, 16, pMacAddress[i]);
        if (!DT_SUCCESS(Status))
            return Status;
        if (DtStringGetStringLength(&DtStrTmp) == 1)
            Status = DtStringAppendDtString(pDtString, &DtStrZero);
        if (!DT_SUCCESS(Status))
            return Status;
        Status = DtStringAppendDtString(pDtString, &DtStrTmp);
        if (!DT_SUCCESS(Status))
            return Status;
        if (i < 5)
            Status = DtStringAppendDtString(pDtString, &DtStrDoubleColon);
        if (!DT_SUCCESS(Status))
            return Status;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoctlCompletion -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#ifdef WINBUILD
IO_COMPLETION_ROUTINE  DtIoctlCompletion;
static NTSTATUS  DtIoctlCompletion(
    PDEVICE_OBJECT  pDeviceObject,
    PIRP  pIrp,
    void*  pContext)
{
    KeSetEvent((PKEVENT)pContext, IO_NO_INCREMENT, FALSE);
    return STATUS_MORE_PROCESSING_REQUIRED;
}
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function must be called at IRQL level < DPC_LEVEL
//
DtStatus  DtIoctl(
    DtIoCtrlParent*  pParent,
    UInt32  IoctlCode,
    UInt  InputBufferSize,
    void*  pInputBuffer,
    UInt  OutputBufferSize,
    void*  pOutputBuffer,
    UInt*  pOutputBufferBytesWritten)
{
    DtStatus  Status = DT_STATUS_OK;
#ifdef WINBUILD
    KEVENT  Event;
    PIRP  pIrp;
    PIO_STACK_LOCATION  pIrpStack;
    NTSTATUS  NtStatus;
    UInt  DataLength = 0;
    void*  pBuffer = NULL;
    DEVICE_OBJECT*  pDeviceObject;
    PMDL  pMdl = NULL;
    
#ifdef USES_KMDF
    pDeviceObject = WdfDeviceWdmGetDeviceObject(pParent->m_WdfDevice);
#else
    pDeviceObject = pParent->m_pWdmDevice;
#endif

    if (pOutputBufferBytesWritten != NULL)
        *pOutputBufferBytesWritten = 0;

    KeInitializeEvent(&Event, NotificationEvent, FALSE);

    // Allocate IRP
    pIrp = IoAllocateIrp(pDeviceObject->StackSize, FALSE);
    if (pIrp == NULL)
    {
        DtDbgOut(ERR, SAL, "Error allocating IRP");
        return DT_STATUS_OUT_OF_RESOURCES;
    }

    // Fill in IRP parameters for DeviceControl
    pIrpStack = IoGetNextIrpStackLocation(pIrp);
    pIrpStack->MajorFunction = IRP_MJ_DEVICE_CONTROL;
    pIrpStack->Parameters.DeviceIoControl.OutputBufferLength = OutputBufferSize;
    pIrpStack->Parameters.DeviceIoControl.InputBufferLength = InputBufferSize;
    pIrpStack->Parameters.DeviceIoControl.IoControlCode = IoctlCode;
    pIrpStack->FileObject = NULL;
    pIrp->MdlAddress = NULL;
    pIrp->AssociatedIrp.SystemBuffer = NULL;

    
    // Allocate a buffer that is large enough to contain
    // both the input and the output buffers.  Copy the input buffer
    // to the allocated buffer and set the appropriate IRP fields.
    if (InputBufferSize!=0 || OutputBufferSize!=0) 
    {
        DataLength = (InputBufferSize>OutputBufferSize ? 
                                                        InputBufferSize:OutputBufferSize);
        pBuffer = DtMemAllocPool(DtPoolNonPaged, DataLength, SAL_TAG);
        if (pBuffer == NULL)
        {
            IoFreeIrp(pIrp);
            return DT_STATUS_OUT_OF_MEMORY;
        }

        // Copy input buffer
        if (pInputBuffer != NULL)
            DtMemCopy(pBuffer, pInputBuffer, InputBufferSize);
        
        // Create an MDL for the output buffer 
        if (OutputBufferSize!=0 && (IoctlCode&3)!=METHOD_BUFFERED)
        {
            pMdl = IoAllocateMdl(pBuffer, OutputBufferSize, FALSE, TRUE, pIrp);
            MmBuildMdlForNonPagedPool(pMdl);
        }

    }
    pIrp->AssociatedIrp.SystemBuffer = pBuffer;
    
    IoSetCompletionRoutine(pIrp, DtIoctlCompletion, &Event, TRUE, TRUE, TRUE);
    
    // Call parent device's IoCtrl function
    IoCallDriver(pDeviceObject, pIrp);

    // Wait for the event to be signalled by the completion routine.
    KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE, NULL);
    
    // Copy the data of system buffer to output buffer.
    if (pOutputBuffer!=NULL && pBuffer!=NULL)
        DtMemCopy(pOutputBuffer, pBuffer, OutputBufferSize);

    NtStatus = pIrp->IoStatus.Status;
    if (NT_SUCCESS(NtStatus))
    {
        if (pOutputBufferBytesWritten != NULL)
            *pOutputBufferBytesWritten = (UInt)pIrp->IoStatus.Information;;
    } else {
        DtDbgOut(ERR, SAL, "Error executing internal IoCtl. Code: %xh, NtError: %xh",
                                                                     IoctlCode, NtStatus);
        Status = DT_STATUS_FAIL;
    }

    if (pMdl != NULL)
        IoFreeMdl(pMdl);

    if(pBuffer)
        DtMemFreePool(pBuffer, SAL_TAG);
        
    IoFreeIrp(pIrp);
    
#else
    IoCtrlFunc  IoCtrl;
    IoCtrl = pParent->m_pParentFunc;

    // Call parent device's IoCtrl function
    Status = IoCtrl(IoctlCode, InputBufferSize, pInputBuffer, OutputBufferSize, 
                    pOutputBuffer, pOutputBufferBytesWritten, pParent->m_pContext);
#endif
    return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ General utility functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDjb2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Djb2 string hash function
//
UInt32  DtDjb2(const char* pStr)
{
    UInt32 Hash = 5381;
    char c;

    if (pStr != NULL)
    {
        c = *pStr++;
        while(c != '\0')
        {
            Hash = (Hash << 5) + Hash + c;
            c = *pStr++;
        }
    }

    return Hash;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtReadConfigSpace -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtReadConfigSpace(
    DtDvcObject*  pDevice, 
    UInt32  Offset, 
    UInt32  Length, 
    UInt8*  pBuffer)
{
#if defined (USB_DRIVER) || defined (NW_DRIVER)
    return DT_STATUS_NOT_SUPPORTED;
#else

#ifdef WINBUILD
#ifdef USES_KMDF
    NTSTATUS  NtStatus;
    BUS_INTERFACE_STANDARD  BusInterfaceStandard;
    WDFDEVICE  parentDevice;
#endif
#else
    Int i=0;
    Int Result = 0;
#endif

    // Check for valid pointer
    if (pBuffer == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Nothing to transfer
    if (Length == 0)
        return DT_STATUS_OK;

#ifdef WINBUILD
#ifdef USES_KMDF
    parentDevice = WdfPdoGetParent(pDevice->m_WdfDevice);
    NtStatus = WdfFdoQueryForInterface(pDevice->m_WdfDevice, 
                          &GUID_BUS_INTERFACE_STANDARD, (PINTERFACE)&BusInterfaceStandard, 
                          sizeof(BUS_INTERFACE_STANDARD), 1, NULL);

    if (!NT_SUCCESS(NtStatus))
        return DT_STATUS_FAIL;

    if (BusInterfaceStandard.GetBusData(BusInterfaceStandard.Context, 
                                     PCI_WHICHSPACE_CONFIG, pBuffer, Offset, Length) == 0)
        return DT_STATUS_FAIL;
#else
    return DT_STATUS_NOT_SUPPORTED;
#endif
#else
    // Read bytes
    for (i=0; i<Length; i++)
    {        
        Result = pci_read_config_byte(pDevice->m_pPciDev, Offset+i, &pBuffer[i]);
        if ( Result != 0 )
            return DT_STATUS_FAIL;
    }
#endif
    return DT_STATUS_OK;
#endif // defined (USB_DRIVER) || defined (NW_DRIVER)
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtWriteConfigSpace -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtWriteConfigSpace(
    DtDvcObject*  pDevice, 
    UInt32  Offset, 
    UInt32  Length, 
    UInt8*  pBuffer)
{

#ifndef USB_DRIVER

#ifdef WINBUILD
#ifdef USES_KMDF
    NTSTATUS  NtStatus;
    BUS_INTERFACE_STANDARD  BusInterfaceStandard;
#endif
#else
    Int i=0;
    Int Result = 0;
#endif

    // Check for valid pointer
    if (pBuffer == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Nothing to transfer
    if (Length == 0)
        return DT_STATUS_OK;

#ifdef WINBUILD
#ifdef USES_KMDF
    NtStatus = WdfFdoQueryForInterface(pDevice->m_WdfDevice, 
                          &GUID_BUS_INTERFACE_STANDARD, (PINTERFACE)&BusInterfaceStandard, 
                          sizeof(BUS_INTERFACE_STANDARD), 1, NULL);

    if (!NT_SUCCESS(NtStatus))
        return DT_STATUS_FAIL;

    if (BusInterfaceStandard.SetBusData(BusInterfaceStandard.Context, 
                                     PCI_WHICHSPACE_CONFIG, pBuffer, Offset, Length) == 0)
        return DT_STATUS_FAIL;
#endif
#else
    // Read bytes
    for (i=0; i<Length; i++)
    {        
        Result = pci_write_config_byte(pDevice->m_pPciDev, Offset+i, pBuffer[i]);
        if ( Result != 0 )
            return DT_STATUS_FAIL;
    }
#endif

#endif // USB_DRIVER

    return DT_STATUS_OK;
}

#ifdef NW_DRIVER
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ Multicast list Helper functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

#ifdef LINBUILD
// Linux specific defines for multicast handling
#ifndef netdev_for_each_mc_addr
    #define netdev_for_each_mc_addr(mclist, dev) \
        for (mclist=dev->mc_list; mclist; mclist=mclist->next)
#endif

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
    #define  DtLinMcAddr  struct dev_mc_list
    #define  DtLinMcAddrPtrGet(x)  x->dmi_addr
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,23) && LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,34)
    #define  DtLinMcAddr  struct dev_addr_list
    #define  DtLinMcAddrPtrGet(x)  x->da_addr
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
    #define  DtLinMcAddr  struct netdev_hw_addr
    #define  DtLinMcAddrPtrGet(x)  x->addr
#endif
#endif // LINBUILD


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCopyMulticastListItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtCopyMulticastListItem(
    UInt  ItemNum,
    DtMulticastList*  pMulticastList, 
    UInt8*  pDst)
{
#ifdef WINBUILD
    UInt8*  pData = pMulticastList->m_pData;

    DT_ASSERT(ItemNum > 0);
    DtMemCopy(pDst, pData+(ItemNum - 1) * 6, 6);
#else
    Int  CurItem;
    DtLinMcAddr*  pMCAddr;
    Int  i;
    
    DT_ASSERT(ItemNum > 0);
    CurItem = 0;

    // Get Multicast item
    netdev_for_each_mc_addr(pMCAddr, pMulticastList->m_pNetDevice)
    {
        CurItem++;
        if (CurItem == ItemNum)
            break;
    }

    DT_ASSERT(CurItem == ItemNum);

    for (i=0; i<6; i++)
    {
        *pDst++ = DtLinMcAddrPtrGet(pMCAddr)[i];
    }
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCopyMulticastListItems -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtCopyMulticastListItems(
    UInt  NumItems, 
    DtMulticastList*  pMulticastList, 
    UInt8*  pDst)
{
#ifdef WINBUILD
    UInt8*  pData = pMulticastList->m_pData;
    DtMemCopy(pDst, pData, NumItems*6);
    
#else
    Int  i;
    DtLinMcAddr*  pMCAddr;
    netdev_for_each_mc_addr(pMCAddr, pMulticastList->m_pNetDevice)
    {
        for (i=0; i<6; i++)
            *pDst++ = DtLinMcAddrPtrGet(pMCAddr)[i];
    }
#endif
}
#endif // NW_DRIVER

