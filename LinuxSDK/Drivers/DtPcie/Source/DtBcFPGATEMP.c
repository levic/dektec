//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcFPGATEMP.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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
#include "DtBcFPGATEMP.h"
#include "DtBcFPGATEMP_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcFPGATEMP implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_FPGATEMP_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcFPGATEMP))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_FPGATEMP_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(FPGATEMP, pBc)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcFPGATEMP - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFPGATEMP_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcFPGATEMP_Close(DtBc*  pBc)
{
    // Sanity check
    BC_FPGATEMP_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFPGATEMP_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcFPGATEMP*  DtBcFPGATEMP_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_FPGATEMP_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcFPGATEMP, Id, DT_BLOCK_TYPE_FPGATEMP,
                                                              Address, pPt, FALSE, pCore);

    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcFPGATEMP_Close;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcFPGATEMP*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFPGATEMP_GetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtBcFPGATEMP_GetTemperature(DtBcFPGATEMP* pBc, Int* pTemperature)
{
    UInt32 Temp;

    // Sanity check
    BC_FPGATEMP_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_FPGATEMP_MUST_BE_ENABLED(pBc) ;

    // Check parameters
    if (pTemperature == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Get temperature
    Temp = FPGATEMP_Status_READ_Temperature(pBc);

    // Convert to integer degrees Celsius
    // Check sign bit
    if ((Temp & 0x80) != 0)
         *pTemperature = ~0xFF | Temp;
    else
         *pTemperature = Temp;
    return DT_STATUS_OK;
}
