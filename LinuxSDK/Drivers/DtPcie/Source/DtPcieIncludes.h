//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPcieIncludes.h *#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
//
// DtPcie driver - Header file containing all standard driver includes.
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

#ifndef __DTPCIE_INCLUDES_H
#define __DTPCIE_INCLUDES_H

#ifdef WINBUILD
// When building under linux we get warnings for unused variables. Enable this specific
// warning as well when building for windows so it gets spotted earlier.
#pragma warning(3 : 4101)
#endif

// Core includes
#include "DtCoreIncludes.h"

// Forward declaration of the device context
typedef struct _DtCorePcie DtCorePcie;
typedef struct _DtPcieChildDeviceData  DtPcieChildDeviceData;

// DtPcie specific headers
#include "DtPcieUtility.h"
#include "DtPcieCommon.h"
#include "Messages.h"

// DtCorePcie.h
#include "DtCorePcie.h"

#endif // __DTPCIE_INCLUDES_H
