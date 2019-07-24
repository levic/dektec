//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcMSIX.c *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#include "DtDf.h"
#include "DtBcMSIX.h"
#include "DtBcMSIX_RegAccess.h"


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcMSIX implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Helper macro to cast a DtBc* to a DtBcMSIX*
#define  BC_MSIX          ((DtBcMSIX*)pBc)

// Macro with default precondition checks for the BcMSIX block-controller
#define BC_MSIX_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcMSIX))

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcMSIX_InterruptsDisable(DtBcMSIX*);
static DtStatus  DtBcMSIX_InterruptsEnable(DtBcMSIX*);
static DtStatus  DtBcMSIX_Init(DtBc*);
static DtStatus  DtBcMSIX_OnEnable(DtBc*, Bool  Enable);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcMSIX_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcMSIX_Close(DtBc*  pBc)
{
    BC_MSIX_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcMSIX_OnInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtBcMSIX_OnInterrupt(DtBcMSIX*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;
    Int  Id=0, Index=0;

    BC_MSIX_DEFAULT_PRECONDITIONS(pBc);

    Id = MSIX_CurrInt0_READ_InterruptId(pBc);
    Index = MSIX_CurrInt0_READ_InterruptIdx(pBc);

    // Check for valid ID
    if (Id == 0)
    {
        DtDbgOutBc(MAX, MSIX, pBc, "Interrupt: id=0x%04X, index=%d", Id, Index);
        return FALSE;
    }
    // Check for valid index
    if(Index<0 || Index>=pBc->m_ConfNumInterrupts)
        return FALSE;

    // Do we have an interrupt handler?
    if(pBc->m_OnHandlerFunc != NULL)
    {
        Status = pBc->m_OnHandlerFunc(0, Id, Index, pBc->m_pOnHandlerContext);
    }
    if (Id != 1)
        DtDbgOutBc(MAX, MSIX, pBc, "Interrupt: id=0x%04X, index=%d", Id, Index);
    MSIX_CurrInt0_WRITE_Arm(pBc, 1);
    return TRUE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcMSIX_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcMSIX*  DtBcMSIX_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_MSIX_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcMSIX, Id, DT_BLOCK_TYPE_MSIX, Address,
                                                                       pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcMSIX_Close;
    OpenParams.m_InitFunc = DtBcMSIX_Init;
    OpenParams.m_OnEnableFunc = DtBcMSIX_OnEnable;
        
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcMSIX*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcMSIX_HandlerRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcMSIX_HandlerRegister(
    DtBcMSIX*  pBc,
    DtBcMSIXOnHandlerFunc  pFunc,
    void*  pContext)
{
    BC_MSIX_DEFAULT_PRECONDITIONS(pBc);

    pBc->m_pOnHandlerContext = pContext;
    pBc->m_OnHandlerFunc = pFunc;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcMSIX_HandlerUnregister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcMSIX_HandlerUnregister(DtBcMSIX*  pBc)
{
    BC_MSIX_DEFAULT_PRECONDITIONS(pBc);


    pBc->m_OnHandlerFunc = NULL;
    pBc->m_pOnHandlerContext = NULL;

    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcMSIX_InterruptsDisable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcMSIX_InterruptsDisable(DtBcMSIX*  pBc)
{
    BC_MSIX_DEFAULT_PRECONDITIONS(pBc);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcMSIX_InterruptsEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcMSIX_InterruptsEnable(DtBcMSIX*  pBc)
{
    BC_MSIX_DEFAULT_PRECONDITIONS(pBc);

    // Configure for single-vector mode
    MSIX_Control_WRITE_IsSingleVector((DtBcMSIX*)pBc, 1);
    // Arm the interrupt
    MSIX_CurrInt0_WRITE_Arm((DtBcMSIX*)pBc, 1);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcMSIX_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcMSIX_Init(DtBc*  pBc)
{
    Int  i=0;
    UInt32  Reg_MSIX = 0;

    BC_MSIX_DEFAULT_PRECONDITIONS(pBc);

    //.-.-.-.-.-.-.-.-.-.-.-.- Read MSIX-controller configuration -.-.-.-.-.-.-.-.-.-.-.-.

    // Cache configuration register
    Reg_MSIX = MSIX_Configuration_READ(BC_MSIX);
    // Configuration - Number of Vectors
    BC_MSIX->m_ConfNumVectors = MSIX_Configuration_GET_NumVectors(Reg_MSIX);
    DT_ASSERT(BC_MSIX->m_ConfNumVectors>=1 && 
                                BC_MSIX->m_ConfNumInterrupts<=DT_BC_MSIX_MAX_NUM_VECTORS);
    // Configuration - Number of Interrupts
    BC_MSIX->m_ConfNumInterrupts = MSIX_Configuration_GET_NumInterrupts(Reg_MSIX);
    DT_ASSERT(BC_MSIX->m_ConfNumInterrupts>=1 && BC_MSIX->m_ConfNumInterrupts<=65535);

    // Print configuration
    DtDbgOutBc(AVG, MSIX, pBc, "Configuration: #vectors=%d, #interrupts=%d",
                                                         BC_MSIX->m_ConfNumVectors,
                                                         BC_MSIX->m_ConfNumInterrupts);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Read vector configuration -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    for (i=0; i<BC_MSIX->m_ConfNumVectors; i++)
    {
        BC_MSIX->m_Vectors[i].m_Address = (UInt64)MSIX_AddressLower0_READ(BC_MSIX);
        BC_MSIX->m_Vectors[i].m_Address |= (UInt64)MSIX_AddressUpper0_READ(BC_MSIX) << 32;
        BC_MSIX->m_Vectors[i].m_Data = MSIX_Data0_READ(BC_MSIX);
        BC_MSIX->m_Vectors[i].m_Mask = MSIX_VectorControl0_READ_Mask(BC_MSIX);
    }

    // Print configuration
    for (i=0; i<BC_MSIX->m_ConfNumVectors; i++)
    {
        DtDbgOutBc(AVG, MSIX, pBc, "Vector%d: address=0x%016llX, data=0x%08X, mask=%d",
                                                         i,
                                                         BC_MSIX->m_Vectors[i].m_Address,
                                                         BC_MSIX->m_Vectors[i].m_Data,
                                                         BC_MSIX->m_Vectors[i].m_Mask);
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcMSIX_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcMSIX_OnEnable(DtBc*  pBc, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;

    // Should we enable or disable the BC
    if (Enable)
    {
        // Enable interrupt generation
        Status = DtBcMSIX_InterruptsEnable(BC_MSIX);
    }
    else 
    {
        // Disable interrupt generation
        Status = DtBcMSIX_InterruptsDisable(BC_MSIX);
    }

    return Status;
}

