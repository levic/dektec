//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcDfCommon.h *#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef  __DT_BC_DF_COMMON_H
#define  __DT_BC_DF_COMMON_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtObject.h"

// Maximum size of a BC/DF name/short-name
#define DT_BCORDF_NAME_MAX_SIZE             64
// Maximum size of a BC/DF instance ID string
#define DT_BCORDF_INSTANCE_ID_MAX_SIZE      64

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcOrDfId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Block-controller/Driver-function ID
typedef struct  _DtBcOrDfId
{
    const char*  m_pName;           // Name of the BC/DF
    const char*  m_pShortName;      // Optional: Short-name of the BC/DF
    const char*  m_pRole;           // Optional: DF/DF role
    Int  m_Instance;                // BC/DF instance number (i.e. Nth occurance of BC/DF)
    Int  m_Uuid;                    // Unique ID of the the BC/DF
}  DtBcOrDfId;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtObjectBcOrDf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Base class for a BC or DF objects
#define DT_OBJECT_BC_OR_DF_COMMON_DATA                                                   \
    DT_OBJECT_COMMON_DATA;          /*  Derive from DtObject */                          \
    DtBcOrDfId  m_Id                /* ID of the object */
// The struct
struct _DtObjectBcOrDf
{
    DT_OBJECT_BC_OR_DF_COMMON_DATA;
};


#endif  // #ifndef  __DT_BC_DF_COMMON_H
