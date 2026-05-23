//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcGPIO.h *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_BC_GPIO_H
#define __DT_BC_GPIO_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the GPIOrotocol block (must match block ID)
#define DT_BC_GPIO_NAME        "GPIO"
#define DT_BC_GPIO_SHORTNAME   "GPIO"

// MACRO: to init an block-controller-ID for the GPIO-BC
#define DT_BC_GPIO_INIT_ID(ID, ROLE, INSTANCE, UUID)                                     \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_GPIO_NAME, DT_BC_GPIO_SHORTNAME, ROLE, INSTANCE, UUID);      \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcGPIO definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=



// GPIO block Input/Output properties
typedef struct _DtBcGPIO_IoProps
{
    const char* m_Mnemonic;     // Mnemonic of the Input/Output
    Bool m_IsOutput;            // Output: TRUE, Input: FALSE
    UInt32  m_Mask;             // Mask = NumBits-1
    Int  m_RegisterOffset;      // Register offset
}DtBcGPIO_IoProps;

// Properties of a GPIO block instance
typedef struct _DtBcGPIO_GPIO
{
    const char* m_Role;             // Role of the GPIO
    const DtBcGPIO_IoProps* m_IoProps;    // GPIO Input/Output properties
    Int     m_NumIoProps;           // Number of Input/Output
}DtBcGPIO_GPIO;


typedef  struct DtBcGPIO
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached configuration data
    const DtBcGPIO_IoProps*  m_pIoProps;  // Pointer to the IO-properties
    Int  m_NumIoProps;                    // Number of IO-properties
}  DtBcGPIO;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPIO public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcGPIO_Close(DtBc*);
DtBcGPIO * DtBcGPIO_Open(Int Address, DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                Int Instance, Int Uuid, Bool CreateStub);
DtStatus  DtBcGPIO_Read(DtBcGPIO*, const char* FieldMnem, UInt32* pValue);
DtStatus  DtBcGPIO_Write(DtBcGPIO*, const char* FieldMnem, UInt32 Value);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcGPIO definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPIO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an GPIO Block
typedef struct _DtIoStubBcGPIO
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcGPIO;


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPIO public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcGPIO_Close(DtIoStub*);
DtIoStubBcGPIO*  DtIoStubBcGPIO_Open(DtBc*);

#endif  // #ifndef __DT_BC_GPIO_H
