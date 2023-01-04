//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtObject.h *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_OBJECT_H
#define __DT_OBJECT_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Base for all objects
typedef enum _DtObjectType
{
    DT_OBJECT_TYPE_BC,          // Block-controller object
    DT_OBJECT_TYPE_DF,          // Driver-function object
    DT_OBJECT_TYPE_CM,          // Core-module object
    DT_OBJECT_TYPE_IOSTUB,      // IO-STUB object
    DT_OBJECT_TYPE_IOSTUB_BC,   // Block-controller IO-STUB object
    DT_OBJECT_TYPE_IOSTUB_DF,   // Driver-function IO-STUB object
    DT_OBJECT_TYPE_PT,          // Port object
    DT_OBJECT_TYPE_CD,          // Child device object
    DT_OBJECT_TYPE_PIPE,        // Pipe object
}  DtObjectType;
// Common data for a driver object
#define DT_OBJECT_COMMON_DATA                                                            \
    Int  m_Size;            /* Set to size of this struct OR of a 'derived' struct */    \
    Int  m_ObjectType       /* Type of object */
// The struct
typedef struct _DtObject
{
    DT_OBJECT_COMMON_DATA;
}  DtObject;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtObject - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtObject_Close(DtObject*, UInt32  Tag);
DtObject*  DtObject_Open(DtObjectType  Type, Int  Size, UInt32  Tag);


#endif  // #ifndef __DT_OBJECT_H
