//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtList.c *#*#*#*#*#*#*#*#*# (C) 2011-2012 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2012 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
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

// Include files
#include <DtSal.h>

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtList +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtListHeadInitialize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtListHeadInitialize(DtListEntry* pHead)
{
#ifdef WINBUILD
    InitializeListHead(pHead);
#else
    INIT_LIST_HEAD(pHead);
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtTailListInsert -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtTailListInsert(DtListEntry* pHead, DtListEntry* pEntry)
{
#ifdef WINBUILD
    InsertTailList(pHead, pEntry);
#else
    list_add_tail(pEntry, pHead);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIsListEmpty -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtIsListEmpty(DtListEntry* pHead)
{
#ifdef WINBUILD
    return IsListEmpty(pHead);
#else
    return list_empty(pHead);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtListNextGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtListEntry*  DtListNextGet(DtListEntry* pEntry)
{
#ifdef WINBUILD
    return pEntry->Flink;
#else
    return pEntry->next;
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtListPrevGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtListEntry*  DtListPrevGet(DtListEntry* pEntry)
{
#ifdef WINBUILD
    return pEntry->Blink;
#else
    return pEntry->prev;
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtListEntryRemove -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtListEntryRemove(DtListEntry* pEntry)
{
#ifdef WINBUILD
    RemoveEntryList(pEntry);
#else
    list_del(pEntry);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- HeadListRemove -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtListEntry*  DtHeadListRemove(DtListEntry* pListHead)
{
#ifdef WINBUILD
    return RemoveHeadList(pListHead);
#else
    DtListEntry* pListEntry = pListHead->next;
    list_del(pListEntry);
    return pListEntry;
#endif
}

