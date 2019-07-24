//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcMSIX.h *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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


#ifndef __DT_BC_MSIX_H
#define __DT_BC_MSIX_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"


// Name and short-name of the MSIX-Controller block (must match block ID)
#define  DT_BC_MSIX_NAME          "MsixController"
#define  DT_BC_MSIX_SHORTNAME     "MSIX"

// Maximum number of vectors supported
#define  DT_BC_MSIX_MAX_NUM_VECTORS  16

// MACRO: to init an block-controller-ID for the MSIX-BC
#define DT_BC_MSIX_INIT_ID(ID, ROLE, INSTANCE, UUID)                                     \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_MSIX_NAME, DT_BC_MSIX_SHORTNAME, ROLE, INSTANCE, UUID);      \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcMSIX definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcMSIXVectorInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtBcMSIXVectorInfo
{
    UInt64  m_Address;
    UInt32  m_Data;
    UInt32  m_Mask;
} DtBcMSIXVectorInfo;

// DtBcMISX callbacks
typedef DtStatus (*DtBcMSIXOnHandlerFunc)(Int Vector, Int Id, Int Index, void*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcMSIX -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Block-controller for the MSIX-controller block
struct  _DtBcMSIX
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Configuration data read from block
    Int  m_ConfNumVectors;          // Number of MSI-X interrupt vectors
    Int  m_ConfNumInterrupts;       // Number of interrupts

    DtBcMSIXVectorInfo  m_Vectors[DT_BC_MSIX_MAX_NUM_VECTORS];
    
    DtBcMSIXOnHandlerFunc  m_OnHandlerFunc;
    void*  m_pOnHandlerContext;

};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcMSIX - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcMSIX_Close(DtBc*);
DtBcMSIX*  DtBcMSIX_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                            const char*  pRole, Int  Instance, Int  Uuid);
Bool  DtBcMSIX_OnInterrupt(DtBcMSIX*);
DtStatus  DtBcMSIX_HandlerRegister(DtBcMSIX*, DtBcMSIXOnHandlerFunc, void*);
DtStatus  DtBcMSIX_HandlerUnregister(DtBcMSIX*);

#endif  // #ifndef __DT_BC_MSIX_H
