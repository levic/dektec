//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDma.h *#*#*#*#*#*#*#*#*#* (C) 2010-2015 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2015 DekTec Digital Video B.V.
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

#ifndef __DT_DMA_H
#define __DT_DMA_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtSal.h"

#ifndef SKIP_DMA
typedef struct _DmaDevice
{
    Bool  m_NotEnoughMapRegisters;           // True if there are not enough map registers
                                             // to do SGL transfer for the complete memory
#ifdef WINBUILD
    WDFDMAENABLER  m_DmaObject;
#else
#endif
} DmaDevice;

typedef struct _DmaDirectBuffer
{
    Bool  m_Allocated;
    UInt  m_BufferLength;
    UInt8*  m_pVirtualAddress;               // Virtual kernel start address of the buffer
    DtPhysicalAddress  m_PhysicalAddress;    // Physical start address of the buffer
    DtPageList  m_PageList;                  // Pagelist of user buffer
#ifdef WINBUILD
    WDFCOMMONBUFFER  m_CommonBuffer;
#else
    UInt8*  m_pVirtualAddressUnaligned;      // Virtual kernel start address of the buffer
    DtPhysicalAddress  m_PhysicalAddressUnaligned;
                                             // Physical start address of the buffer
#endif
} DmaDirectBuffer;

typedef struct _DmaOsSgl
{
    Bool  m_Allocated;
    UInt  m_BufType;
    UInt  m_BufferLength;
    DtPageList  m_PageList;                  // Pagelist of user buffer
#ifdef WINBUILD
    PSCATTER_GATHER_LIST  m_pSgList;
    WDFDMATRANSACTION  m_WdfTransaction;
    KEVENT  m_DmaPrepareDoneEvent;
#else
    UInt  m_NumSglEntries;
    struct scatterlist*  m_pSgList;
#endif
} DmaOsSgl;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DMA direction -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#define  DT_DMA_DIRECTION_FROM_DEVICE    1
#define  DT_DMA_DIRECTION_TO_DEVICE      2


DtStatus  DtDmaInit(DtDvcObject* pDevice, DmaDevice* pDmaDevice, UInt MaxDmaTransfer,
                                                                      Bool Supports64Bit);
DtStatus  DtDmaCreateDirectBuffer(DtDvcObject* pDevice, DmaDevice* pDmaDevice, 
                                  DmaDirectBuffer* pDmaBuffer, UInt BufSize, UInt Align);
DtStatus  DtDmaFreeDirectBuffer(DtDvcObject* pDevice,DmaDirectBuffer* pDmaBuffer);
DtStatus  DtDmaCreateSgList(DtDvcObject* pDevice, DmaDevice* pDmaDevice, void* pBuffer,
                                      UInt BufSize, UInt Direction, DmaOsSgl* pOsSgl);
DtStatus  DtDmaFinalSglTransfer(DmaOsSgl* pOsSgl);
DtStatus  DtDmaFlushCacheSglPreDma(DtDvcObject* pDevice, DmaOsSgl* pOsSgl, 
                                                                          UInt Direction);
DtStatus  DtDmaFreeSgList(DtDvcObject* pDevice, DmaOsSgl* pOsSgl,UInt Direction);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDmaOsSglGetNumElements -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline UInt  DtDmaOsSglGetNumElements(DmaOsSgl* pDmaOsSgl)
{
#ifdef WINBUILD
    return pDmaOsSgl->m_pSgList->NumberOfElements;
#else
    return pDmaOsSgl->m_NumSglEntries;
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDmaOsSglGetAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline DtPhysicalAddress  DtDmaOsSglGetAddress(DmaOsSgl* pDmaOsSgl, UInt Index)
{
#ifdef WINBUILD
    return pDmaOsSgl->m_pSgList->Elements[Index].Address;
#else
    DtPhysicalAddress  PhysicalAddress;
    PhysicalAddress.QuadPart = sg_dma_address(&pDmaOsSgl->m_pSgList[Index]);
    return PhysicalAddress;
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDmaOsSglGetLength -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline UInt  DtDmaOsSglGetLength(DmaOsSgl* pDmaOsSgl, UInt Index)
{
#ifdef WINBUILD
    return pDmaOsSgl->m_pSgList->Elements[Index].Length;
#else
    return sg_dma_len(&pDmaOsSgl->m_pSgList[Index]);
#endif
}
#endif  // SKIP_DMA
#endif  // __DT_DMA_H
