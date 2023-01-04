// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCoreIncludes.h *#*#*#*#*#*#*# (C) 2017-2022 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017-2022 DekTec Digital Video B.V.
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

#ifndef __DTCORE_INCLUDES_H
#define __DTCORE_INCLUDES_H

#ifdef WINBUILD
// When building under linux we get warnings for unused variables. Enable this specific
// warning as well when building for windows so it gets spotted earlier.
#pragma warning(3 : 4101)
#endif

// OS wrapper
#include "DtSal.h"
#include "DtIal.h"

// Driver common
#include "DtDrvCommon.h"

// Core headers
#include "Messages.h"
#include "DtCore.h"
#define DT_BC_ADD_BLOCK_INCLUDES
#include "DtBc.h"
#include "DtVectorBc.h"
#define DT_DF_ADD_FUNCTION_INCLUDES
#include "DtDf.h"
#include "DtVectorDf.h"
#include "DtPt.h"
#include "DtVectorPt.h"

#endif  // #ifndef __DTCORE_INCLUDES_H
