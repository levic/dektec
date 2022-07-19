// *#*#*#*#*#*#*#*#*#*#*#*#* DtDfGenLockCtrl_CallbackFunction.h *#*#*#*#*# (C) 2019 DekTec
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

#ifndef __DT_DF_GENLOCKCTRL_CALLBACK_FUNCTION_H
#define __DT_DF_GENLOCKCTRL_CALLBACK_FUNCTION_H

typedef enum _DtDfGenLockCtrl_GenRefType
{
    DT_DF_GENLOCKCTRL_GENREF_UNDEFINED,
    DT_DF_GENLOCKCTRL_GENREF_VIRTUAL,
    DT_DF_GENLOCKCTRL_GENREF_LMH1981,
}DtDfGenLockCtrl_GenRefType;

typedef struct _DtDfGenLockCtrl DtDfGenLockCtrl;
void DtDfGenLockCtrl_GenRefStartOfFrameHandler(DtDfGenLockCtrl*, Int PortIdx,
                                           const DtTodTime*, Int SofOffset,
                                           Int VidStd, Int DetectVidStd,
                                           DtDfGenLockCtrl_GenRefType); 
#endif  // #ifndef __DT_DF_GENLOCKCTRL_CALLBACK_FUNCTION_H
