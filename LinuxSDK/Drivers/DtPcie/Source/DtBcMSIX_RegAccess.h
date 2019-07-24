//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcMSIX_RegAccess.h *#*#*#*#*#*#*#*#* (C) 2017 DekTec
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


#ifndef __DT_BC_MSIX_REGS_H
#define __DT_BC_MSIX_REGS_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc_RegAccess.h"

//+=+=+=+=+ Helper macro's for defining MsixController register access functions +=+=+=+=+

#define  MSIX_REG_READ(OFFSET, REG_MNEM, MSB_BIT, NUM_BITS)                              \
    DT_BC_REG_READ(MSIX, DtBcMSIX, OFFSET, REG_MNEM, MSB_BIT, NUM_BITS)

#define  MSIX_FIELD_GET(REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)                           \
    DT_BC_FIELD_GET(MSIX, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)

#define  MSIX_FIELD_READ(OFFSET, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)                  \
    DT_BC_FIELD_READ(MSIX, DtBcMSIX, OFFSET, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)

#define  MSIX_REG_WRITE(OFFSET, REG_MNEM, MSB_BIT, NUM_BITS)                             \
    DT_BC_REG_WRITE(MSIX, DtBcMSIX, OFFSET, REG_MNEM, MSB_BIT, NUM_BITS)

#define  MSIX_FIELD_SET(REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)                           \
    DT_BC_FIELD_SET(MSIX, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)

#define  MSIX_FIELD_WRITE(OFFSET, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)                 \
    DT_BC_FIELD_WRITE(MSIX, DtBcMSIX, OFFSET, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)

#define  MSIX_FIELD_RW(OFFSET, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)                    \
    DT_BC_FIELD_RW(MSIX, DtBcMSIX, OFFSET, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Register access functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Block ID register: Offset=0x00
// See DtBc_Regs.h

// Interrupt Configuration register: Offset=0x04
static const Int  MSIX_REG_CONF = 0x04;
MSIX_REG_READ(MSIX_REG_CONF, Configuration, 31, 32)
MSIX_FIELD_READ(MSIX_REG_CONF, Configuration, NumVectors, 11, 12)
MSIX_FIELD_READ(MSIX_REG_CONF, Configuration, NumInterrupts, 31, 16)

// Interrupt Status register: Offset=0x08
static const Int  MSIX_REG_STATUS = 0x08;
MSIX_REG_READ(MSIX_REG_STATUS, Status, 31, 32)
MSIX_FIELD_READ(MSIX_REG_STATUS, Status, CfgFunctionMask, 0, 1)
MSIX_FIELD_READ(MSIX_REG_STATUS, Status, CfgMsixEnable, 1, 1)

// Interrupt Control register: Offset=0x0C
static const Int  MSIX_REG_CONTROL = 0x0C;
MSIX_REG_READ(MSIX_REG_CONTROL, Control, 31, 32)
MSIX_FIELD_RW(MSIX_REG_CONTROL, Control, IsSingleVector, 0, 1)

// Current Interrupt Vector 0 register: Offset=0x10
static const Int  MSIX_REG_CURRINT0 = 0x10;
MSIX_REG_READ(MSIX_REG_CURRINT0, CurrInt0, 31, 32)
MSIX_REG_WRITE(MSIX_REG_CURRINT0, CurrInt0, 31, 32)
MSIX_FIELD_WRITE(MSIX_REG_CURRINT0, CurrInt0, Arm,  0, 1)
MSIX_FIELD_READ(MSIX_REG_CURRINT0, CurrInt0, InterruptIdx,  15, 12)
MSIX_FIELD_READ(MSIX_REG_CURRINT0, CurrInt0, InterruptId, 31, 16)

// Current Interrupt Vector 1 register: Offset=0x14
static const Int  MSIX_REG_CURRINT1 = 0x14;
MSIX_REG_READ(MSIX_REG_CURRINT1, CurrInt1, 31, 32)
MSIX_REG_WRITE(MSIX_REG_CURRINT1, CurrInt1, 31, 32)
MSIX_FIELD_WRITE(MSIX_REG_CURRINT1, CurrInt1, Arm, 0, 1)
MSIX_FIELD_READ(MSIX_REG_CURRINT1, CurrInt1, InterruptIdx, 15, 12)
MSIX_FIELD_READ(MSIX_REG_CURRINT1, CurrInt1, InterruptId, 31, 16)

// Address Lower 0 register: Offset=0x20
static const Int  MSIX_REG_ADDRLOW0 = 0x20;
MSIX_REG_READ(MSIX_REG_ADDRLOW0, AddressLower0, 31, 32)
MSIX_REG_WRITE(MSIX_REG_ADDRLOW0, AddressLower0, 31, 32)

// Address Upper 0 register: Offset=0x24
static const Int  MSIX_REG_ADDRUPP0 = 0x24;
MSIX_REG_READ(MSIX_REG_ADDRUPP0, AddressUpper0, 31, 32)
MSIX_REG_WRITE(MSIX_REG_ADDRUPP0, AddressUpper0, 31, 32)

// Data 0 register: Offset=0x28
static const Int  MSIX_REG_DATA0 = 0x28;
MSIX_REG_READ(MSIX_REG_DATA0, Data0, 31, 32)
MSIX_REG_WRITE(MSIX_REG_DATA0, Data0, 31, 32)

// Vector Control 0 register: Offset=0x2C
static const Int  MSIX_REG_VECCTRL0 = 0x2C;
MSIX_REG_READ(MSIX_REG_VECCTRL0, VectorControl0, 31, 32)
MSIX_REG_WRITE(MSIX_REG_VECCTRL0, VectorControl0, 31, 32)
MSIX_FIELD_RW(MSIX_REG_VECCTRL0, VectorControl0, Mask, 0, 1)

// Address Lower 1 register: Offset=0x30
static const Int  MSIX_REG_ADDRLOW1 = 0x30;
MSIX_REG_READ(MSIX_REG_ADDRLOW1, AddressLower1, 31, 32)
MSIX_REG_WRITE(MSIX_REG_ADDRLOW1, AddressLower1, 31, 32)

// Address Upper 1 register: Offset=0x34
static const Int  MSIX_REG_ADDRUPP1 = 0x34;
MSIX_REG_READ(MSIX_REG_ADDRUPP1, AddressUpper1, 31, 32)
MSIX_REG_WRITE(MSIX_REG_ADDRUPP1, AddressUpper1, 31, 32)

// Data 1 register: Offset=0x38
static const Int  MSIX_REG_DATA1 = 0x38;
MSIX_REG_READ(MSIX_REG_DATA1, Data1, 31, 32)
MSIX_REG_WRITE(MSIX_REG_DATA1, Data1, 31, 32)

// Vector Control 1 register: Offset=0x3C
static const Int  MSIX_REG_VECCTRL1 = 0x3C;
MSIX_REG_READ(MSIX_REG_VECCTRL1, VectorControl1, 31, 32)
MSIX_REG_WRITE(MSIX_REG_VECCTRL1, VectorControl1, 31, 32)
MSIX_FIELD_RW(MSIX_REG_VECCTRL1, VectorControl1, Mask, 0, 1)


#endif  // #ifndef __DT_BC_MSIX_REGS_H