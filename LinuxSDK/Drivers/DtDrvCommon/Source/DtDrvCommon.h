//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDrvCommon.h *#*#*#*#*#*#*#*# (C) 2011-2016 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2016 DekTec Digital Video B.V.
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

#ifndef __DT_DRV_COMMON_H
#define __DT_DRV_COMMON_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#include <DtSal.h>
#include <DtProperties.h>
#include <DtTables.h>
#include <DtUtilities.h>
#include <DtIoConfigDefs.h>
#include <IoConfigCodes.h>
#include <DtIoConfig.h>
#include <DtAudioVideo.h>
#include <VpdCommon.h>


// COMMON related logging
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- LOG levels -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// TABLE
#define  LOG_LEVEL_TABLE         LOG_MIN

// Properties related
#define  LOG_LEVEL_PROP          LOG_MIN

// AV related
#define  LOG_LEVEL_AV            LOG_MIN

// IoConfig
#define  LOG_LEVEL_IOCONFIG      LOG_MAX

// VPD related
#define  LOG_LEVEL_VPD           LOG_MIN

#endif // __DT_DRV_COMMON_H
