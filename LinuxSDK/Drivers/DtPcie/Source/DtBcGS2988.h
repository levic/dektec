// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcGS2988.h *#*#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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


#ifndef __DT_BC_GS2988_H
#define __DT_BC_GS2988_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Gs2988Ctrl block (must match block ID)
#define DT_BC_GS2988_NAME        "Gs2988Ctrl"
#define DT_BC_GS2988_SHORTNAME   "GS2988"

// MACRO: to init an block-controller-ID for the GS2988-BC
#define DT_BC_GS2988_INIT_ID(ID, ROLE, INSTANCE, UUID)                                    \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_GS2988_NAME, DT_BC_GS2988_SHORTNAME, ROLE, INSTANCE, UUID);  \
}                                                                                        \
while (0)

// DT GS2988 SDI mode
#define DT_GS2988_SDIMODE_SD      0   // SD-SDI/ASI
#define DT_GS2988_SDIMODE_HD_3G   1   // HD/3G-SDI


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcGS2988 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGS2988 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcGS2988
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
} DtBcGS2988;


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGS2988 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcGS2988_Close(DtBc*);
DtBcGS2988*  DtBcGS2988_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcGS2988_SetSdiMode(DtBcGS2988*, Int  Mode);

#endif // #ifndef __DT_BC_GS2988_H
