//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtUtility.h *#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
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

#ifndef __DT_UTILITY_H
#define __DT_UTILITY_H

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Macros +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define  OFFSETOF(Type, Field)  ((UInt32)(size_t)&(((Type*)0)->Field))

// Use to mark a function or variable as potentially unused. Suppresses warnings about
// unsused variables or functions
#ifdef WINBUILD
#define DT_UNUSED
#else
#define DT_UNUSED  __attribute__  ((unused))
#endif

#ifdef WINBUILD
#define  DT_ASSERT  ASSERT
#else
#define  INFINITE  ((UInt32)-1)
#ifdef DEBUG
#define  DT_ASSERT(e) if (!(e)) printk(KERN_DEBUG "Assertion failed in file " __FILE__ ", \
                                                            line %d ("  #e ").", __LINE__)
#else
#define  DT_ASSERT(e)  do {} while (0)
#endif // DEBUG
#endif // WINBUILD


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Debug logging +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// LOG LEVEL defines
#define  LOG_ERR     0       //  Off
#define  LOG_MIN     1       //  Minimal (setting-up / breaking-down of channels)
#define  LOG_AVG     2       //  Average (up to 10 per second)
#define  LOG_MAX     3       //  All
#define  LOG_OFF     LOG_ERR //  Off, Errors only

#ifdef _DEBUG
#ifdef DEBUG_INCLUDE_FILENAME
#ifdef WINBUILD
#define  DtDbgOut(Level, Module, Msg, ...)                                               \
    do {                                                                                 \
        if (LOG_LEVEL_##Module >= LOG_##Level)                                           \
        DbgPrint(DRIVER_NAME ": <" __FILE__ " - "__FUNCTION__ " (Line %d)> " Msg "\n",   \
                                                        __LINE__, ##__VA_ARGS__); \
    } while (0)
#else // WINBUILD
#define DtDbgOut(Level, Module, Msg, ...)                                                \
    do {                                                                                 \
        if (LOG_LEVEL_##Module >= LOG_##Level)                                           \
            printk(KERN_DEBUG DRIVER_NAME ": <" __FILE__ " - %s (Line %d)> "             \
                                           Msg "\n", __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#endif
#else // DEBUG_INCLUDE_FILENAME
#ifdef WINBUILD
#define DtDbgOut(Level, Module, Msg, ...)                                                \
    do {                                                                                 \
        if (LOG_LEVEL_##Module >= LOG_##Level)                                           \
        DbgPrint(DRIVER_NAME ": <" __FUNCTION__ "> " Msg "\n",                           \
                                                        ##__VA_ARGS__);                  \
    } while (0)
#else // WINBUILD
#define DtDbgOut(Level, Module, Msg, ...)                                                \
    do {                                                                                 \
        if (LOG_LEVEL_##Module >= LOG_##Level)                                           \
            printk(KERN_DEBUG DRIVER_NAME ": < %s > "                                    \
                                                     Msg "\n", __func__, ##__VA_ARGS__); \
    } while (0)
#endif
#endif //DEBUG_INCLUDE_FILENAME
#else // _DEBUG
#define DtDbgOut(Level, Module, Msg, ...) do { } while (0)
#endif

// Use when the Module name must be prefixed. For example all block-controller module 
// names have the BC_ prefix (e.g. BC_SPIMF, BC_VVI)
#define DtDbgOut_Prefix(Level, Prefix, Module, Msg, ...)                                 \
                DtDbgOut(Level, Prefix##Module, Msg, ##__VA_ARGS__)


// Dta driver specific DtDbgOut 
#define DtDbgOutPort(Level, Module, pPort, Msg, ...)                                     \
                        DtDbgOut(Level, Module, "[%d:%d:%d] " Msg,                       \
                                              pPort->m_pDvcData->m_DevInfo.m_TypeNumber, \
                                              pPort->m_pDvcData->m_DevInfo.m_SubDvc,     \
                                              pPort->m_PortIndex, ##__VA_ARGS__)
#define DtDbgOutDma(Level, Module, pDmaCh, Msg, ...)                                     \
                        DtDbgOut(Level, Module, "[%d:%d:%d] " Msg,                       \
                                              pDmaCh->m_pDvcData->m_DevInfo.m_TypeNumber,\
                                              pDmaCh->m_pDvcData->m_DevInfo.m_SubDvc,    \
                                              pDmaCh->m_PortIndex, ##__VA_ARGS__)
#define DtDbgOutDev(Level, Module, pDvcData, Msg, ...)                                   \
                        DtDbgOut(Level, Module, "[%d:%d] " Msg,                          \
                                              pDvcData->m_DevInfo.m_TypeNumber,          \
                                              pDvcData->m_DevInfo.m_SubDvc,              \
                                              ##__VA_ARGS__)

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Event Logging +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtEvtLog -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Struct for OS specific event messaging info.
//

#ifndef WINBUILD
 typedef Bool (EvtGetMessageFunc)(Int, const char**, const char**);
#endif

typedef struct _DtEvtLog
{
#ifdef WINBUILD
    void*  m_pIoObject;
#else
    EvtGetMessageFunc*  m_pEvtGetMessage;
#endif
} DtEvtLog;

#ifdef WINBUILD
void  DtEvtLogInit(DtEvtLog* pEvtObject, void* pIoObject);
#else
void  DtEvtLogInit(DtEvtLog* pEvtObject, EvtGetMessageFunc* pGetMsg);
#endif
void  DtEvtLogReport(DtEvtLog* pEvtObject, UInt32 ErrorCode, DtString* pInsert1, 
                    DtString* pInsert2, DtString* pInsert3);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Memory functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

typedef enum _DtPoolType
{
    DtPoolNonPaged = 0,
    DtPoolPaged
} DtPoolType;

void*  DtMemAllocPoolLarge(DtPoolType Type, UInt Size, UInt32 Tag, 
                                                                  DtPageList** pPageList);
void  DtMemFreePoolLarge(void* pPtr, UInt32 Tag, DtPageList* pPageList);
void*  DtMemAllocPool(DtPoolType Type, UInt Size, UInt32 Tag);
void  DtMemFreePool(void* pPtr, UInt32 Tag);
void  DtMemZero(void* pPtr, UInt Size);
void  DtMemCopy(void* pDest, const void* pSrc, UInt Size);
void  DtMemCopyFromUserBuf(void* pDest, void* pSrc, UInt Size);
void  DtMemCopyToUserBuf(void* pDest, void* pSrc, UInt Size);
void  DtMemMove(void* pDest, void* pSrc, UInt Size);


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ String/Character functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtrToUInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline UInt  DtPtrToUInt(void* pPtr)
{
#ifdef WINBUILD
    return PtrToUlong(pPtr);
#else
    return (UInt)(unsigned long)(pPtr);
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUInt16ByteSwap -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline UInt16  DtUInt16ByteSwap(UInt16 Word)
{
#ifdef WINBUILD
    return RtlUshortByteSwap(Word);
#else
    return (Word << 8) | (Word >> 8);
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUInt32ByteSwap -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline UInt32  DtUInt32ByteSwap(UInt32 Dword)
{
#ifdef WINBUILD
    return RtlUlongByteSwap(Dword);
#else
    return (Dword >> 24) | ((Dword >> 8) & 0x0000FF00 ) | ((Dword << 8) & 0x00FF0000) |
                                                                            (Dword << 24);
#endif
}

UInt  DtAnsiCharToUInt(char Char);
UInt  DtAnsiCharArrayToUInt(const char* pUIntString, UInt StrLen, UInt Base);
UInt64  DtAnsiCharArrayToUInt64(const char* pUIntString, UInt StrLen, UInt Base);
Bool  DtAnsiCharArrayIsEqual(const char* pStr1, const char* pStr2);
Bool  DtAnsiCharArrayStartsWith(const char* pStr, const char* pStart);
Int  DtAnsiCharArrayStrLength(const char* pStr, Int  MaxLength);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Timing functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

void  DtSleep(Int MSec);
void  DtWaitBlock(Int USec);
UInt64  DtGetTickCount(void);
UInt64  DtGetTickCountUSec(void);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Memory (Page) functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type of buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#define  DT_BUFTYPE_USER                1
#define  DT_BUFTYPE_KERNEL              2

DtStatus  DtCreatePageList(void* pBuffer, UInt BufSize, UInt BufType, 
                                                                   DtPageList* pPageList);
DtStatus  DtDeletePageList(DtPageList* pPageList);
DtStatus  DtLockUserBuffer(DtPageList* pPageList, UInt8* pBuffer);
DtStatus  DtUnlockUserBuffer(DtPageList* pPageList);

#ifdef WINBUILD
#else
UInt  DtGetPagesUserBufferAndLock(struct task_struct* pTask, void* pBuffer, UInt NumPages,
                                                                   DtPageList* pPageList);
void  DtFreePagesUserBuffer(DtPageList* pPageList);
UInt  DtGetPagesKernelBuffer(void* pBuffer, DtPageList* pPageList);
#endif

DtStatus  DtGetUserBuffer(DtIoctlObject* pIoctl, UInt8** pBuffer, Int* pSize, 
                                                                        Bool UseCopyFunc);
void  DtReleaseUserBuffer(DtIoctlObject* pIoctl, UInt8* pBuffer);



//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ General utility functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

UInt32  DtDjb2(const char* pStr);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtContainingRecod -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Finds the container of a member based on:
// Pointer to a field in a struct, type of the struct and the name of the field
// usage: DtContainingRecord(pField, struct type, field name)
// Note: we cannot encapsulate this macro in a function due to the passing of a type.
#ifdef WINBUILD
    #define DtContainingRecord CONTAINING_RECORD
#else
    #define DtContainingRecord container_of
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDivide64 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// 64 bit division returns num/denom and rest
//
static __inline UInt64  DtDivide64(UInt64 Num, UInt64 Denom, UInt64* pRest)
{
#ifdef WINBUILD
    if (pRest != NULL)
        *pRest = Num % Denom;
    return Num / Denom;
#else
    UInt32  Rest;
    Rest = do_div(Num, Denom);
    if (pRest != NULL)
        *pRest = Rest;
    return Num;

    //Int  DenomBits, NumBits ;
    //Int64  RestBits, RestMask, Rest;
    //Int64  Answer = 0;
    //Int  Sign = 0;

    //if (Num < 0)
    //{
    //    Num = -Num;
    //    Sign = 1;
    //}
    //if (Denom < 0)
    //{
    //    Denom = -Denom;
    //    Sign ^= 1;
    //}

    //if (Num < Denom)
    //    return 0;

    //DenomBits = 0;
    //while ((Int64)1<<DenomBits < Denom)
    //    DenomBits++;

    //NumBits = DenomBits;
    //while ((Int64)1<<NumBits < Num)
    //    NumBits++;

    //RestBits = NumBits - DenomBits;
    //RestMask = ((Int64)1<<RestBits) - 1;
    //Rest = Num >> RestBits;
    //Num = Num & RestMask;

    //while (TRUE)
    //{
    //    Answer <<= 1;
    //    if (Rest >= Denom)
    //    {
    //        Answer |= 1;
    //        Rest -= Denom;
    //    }

    //    if (RestBits == 0)
    //        break;
    //    RestBits--;
    //    RestMask >>= 1;
    //    Rest = (Rest<<1) | (Num>>RestBits);
    //    Num = Num & RestMask;
    //}

    //if (pRest != NULL)
    //    *pRest = Rest;

    //if (Sign != 0)
    //    return -Answer;
    //
    //return Answer;
#endif
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDivideS64 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// 64 bit integer division returns num/denom
//
static __inline Int64  DtDivideS64(Int64 Num, Int64 Denom)
{
#ifdef WINBUILD
    return Num / Denom;
#else
    return div64_s64(Num, Denom);
#endif
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtModuloS64 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// returns remainder of 64 bit integer division of num/denom;
//
static __inline Int64  DtModuloS64(Int64 Num, Int64 Denom)
{
#ifdef WINBUILD
    return Num % Denom;
#else
    Int64 Rest = Num - div64_s64(Num, Denom)*Denom;
    if (Rest < 0)
        return -Rest;
    else
        return Rest;
#endif
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAtomicCompareExchange -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function returns the original value
//
static __inline Int  DtAtomicCompareExchange(Int* pDest, Int ExpectedOld, Int New)
{
#ifdef WINBUILD
    return InterlockedCompareExchange(pDest, New, ExpectedOld);
#else
    atomic_t*  pAtomicInt = (atomic_t*)pDest;
    return atomic_cmpxchg(pAtomicInt, ExpectedOld, New);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAtomicIncrement -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline void  DtAtomicIncrement(Int* pVar)
{
#ifdef WINBUILD
    InterlockedIncrement(pVar);
#else
    atomic_t*  pAtomicInt = (atomic_t*)pVar;
    atomic_inc(pAtomicInt);
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAtomicIncrementReturn -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static  __inline Int  DtAtomicIncrementReturn(Int* pVar)
{
#ifdef WINBUILD
    return InterlockedIncrement(pVar);
#else
    atomic_t*  pAtomicInt = (atomic_t*)pVar;
    return atomic_inc_return(pAtomicInt);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAtomicDecrement -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline void  DtAtomicDecrement(Int* pVar)
{
#ifdef WINBUILD
    InterlockedDecrement(pVar);
#else
    atomic_t*  pAtomicInt = (atomic_t*)pVar;
    atomic_dec(pAtomicInt);
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAtomicDecrementReturn -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static  __inline Int  DtAtomicDecrementReturn(Int* pVar)
{
#ifdef WINBUILD
    return InterlockedDecrement(pVar);
#else
    atomic_t*  pAtomicInt = (atomic_t*)pVar;
    return atomic_dec_return(pAtomicInt);
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAtomicSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline void  DtAtomicSet(Int* pVar, Int Value)
{
#ifdef WINBUILD
    InterlockedExchange(pVar, Value);
#else
    atomic_t*  pAtomicInt = (atomic_t*)pVar;
    atomic_set(pAtomicInt, Value);
#endif
}

DtStatus  DtMacAddress2DtString(UInt8* pMacAddress, DtString* pDtString);

//=+=+=+=+=+=+=+=+=+=+=+=+ Internal driver-driver  communication +=+=+=+=+=+=+=+=+=+=+=+=+
typedef struct _DtIoCtrlParent
{
#ifdef WINBUILD
#ifdef USES_KMDF
    WDFDEVICE  m_WdfDevice;
#else
    DEVICE_OBJECT*  m_pWdmDevice;
#endif // USES_KMDF
#else
    void*  m_pParentFunc;
    void*  m_pContext;
#endif
} DtIoCtrlParent;

DtStatus  DtIoctl(DtIoCtrlParent* pParent, UInt32 IoctlCode, UInt InputBufferSize, 
                                    void* pInputBuffer, UInt OutputBufferSize,
                                    void* pOutputBuffer, UInt* pOutputBufferBytesWritten);

#ifdef WINBUILD
#else
typedef DtStatus  (*IoCtrlFunc)(UInt32 IoctlCode, UInt InputBufferSize, void* pInputBuffer,
                                         UInt OutputBufferSize, void* pOutputBuffer,
                                         UInt* pOutputBufferBytesWritten, void* pContext);
#endif // WINBUILD

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ PCI Device helper functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
DtStatus  DtReadConfigSpace(DtDvcObject* pDevice, UInt32 Offset, UInt32 Length, 
                                                                          UInt8* pBuffer);
DtStatus  DtWriteConfigSpace(DtDvcObject* pDevice, UInt32 Offset, UInt32 Length, 
                                                                          UInt8* pBuffer);

#ifdef NW_DRIVER
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ Multicast list Helper functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

typedef struct _DtMulticastList
{
#ifdef WINBUILD
    UInt8*  m_pData;
#else
    // For Linux, the Multicast list is in the net_device struct.
    // We use Linux helper macro's to get the multicast list from this item
    struct net_device*  m_pNetDevice;
#endif // WINBUILD
} DtMulticastList;

void  DtCopyMulticastListItem(UInt ItemNum, DtMulticastList* pMulticastList, UInt8* pDst);
void  DtCopyMulticastListItems(UInt NumItems, DtMulticastList* pMulticastList, 
                                                                             UInt8* pDst);
#endif // NW_DRIVER

#endif // __DT_UTILITY_H
