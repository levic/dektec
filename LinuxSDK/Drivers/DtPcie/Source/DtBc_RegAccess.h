// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBc_RegAccess.h *#*#*#*#*#*#*# (C) 2017-2018 DekTec
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

#ifndef __DT_BC_REGS_H
#define __DT_BC_REGS_H

//+=+=+=+=+=+=+ Helper macro's for defining register/field access functions +=+=+=+=+=+=+=

// Helper for field shioft and mask
#define  DT_BC_FIELD_SHIFT(MSB_BIT, NUM_BITS)  ((UInt32)(MSB_BIT+1-NUM_BITS))
#define  DT_BC_FIELD_MASK(NUM_BITS)            ((UInt32)((1LL<<NUM_BITS)-1))

// Macro defining the body of a register READ function
#define DT_BC_REG_READ_BODY(MSB_BIT, NUM_BITS)                                           \
    do                                                                                   \
    {                                                                                    \
        const UInt32  Shift=DT_BC_FIELD_SHIFT(MSB_BIT, NUM_BITS);                        \
        const UInt32  Mask=DT_BC_FIELD_MASK(NUM_BITS);                                   \
        return ((DtBc_RegRead32((DtBc*)pBc, Offset)>>Shift) & Mask);                     \
    } while(0)

// Macro defining a register READ access function
#define DT_BC_REG_READ(SHORT, BC_TYPE, OFFSET, REG_MNEM, MSB_BIT, NUM_BITS)              \
    static __inline UInt32  SHORT##_##REG_MNEM##_READ(BC_TYPE*  pBc)                     \
    {                                                                                    \
        const UInt32  Offset = OFFSET;                                                   \
        DT_BC_REG_READ_BODY(MSB_BIT, NUM_BITS);                                          \
    }
// Macro defining a register READ access function. With a variable offset
#define DT_BC_REG_READ_VAR(SHORT, BC_TYPE, REG_MNEM, MSB_BIT, NUM_BITS)                  \
    static __inline UInt32  SHORT##_##REG_MNEM##_READ(BC_TYPE*  pBc, UInt32  Offset)     \
    {                                                                                    \
        DT_BC_REG_READ_BODY(MSB_BIT, NUM_BITS);                                          \
    }
// Macro defining a field GET access function
#define DT_BC_FIELD_GET(SHORT, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)                    \
    static __inline UInt32  SHORT##_##REG_MNEM##_GET##_##FLD_MNEM(UInt32  Reg_Value)     \
    {                                                                                    \
        const UInt32  Shift=DT_BC_FIELD_SHIFT(MSB_BIT, NUM_BITS);                        \
        const UInt32  Mask=DT_BC_FIELD_MASK(NUM_BITS);                                   \
        return (Reg_Value >> Shift) & Mask;                                              \
    }

// Macro defining the body of a field READ function
#define DT_BC_FIELD_READ_BODY(SHORT, OFFSET, REG_MNEM, FLD_MNEM)                         \
    do                                                                                   \
    {                                                                                    \
        const UInt32  Reg_Value = DtBc_RegRead32((DtBc*)pBc, OFFSET);                    \
        return SHORT##_##REG_MNEM##_GET##_##FLD_MNEM(Reg_Value);                         \
    } while(0)

// Macro defining a field READ+GET access functions. With a variable register offset
#define DT_BC_FIELD_READ_VAR(SHORT, BC_TYPE, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)      \
    DT_BC_FIELD_GET(SHORT, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)                        \
    static __inline UInt32  SHORT##_##REG_MNEM##_READ##_##FLD_MNEM(BC_TYPE*  pBc,        \
                                                                         UInt32  Offset) \
    {                                                                                    \
        DT_BC_FIELD_READ_BODY(SHORT, Offset, REG_MNEM, FLD_MNEM);                        \
    }
// Macro defining a field READ+GET access functions
#define DT_BC_FIELD_READ(SHORT, BC_TYPE, OFFSET, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)  \
    DT_BC_FIELD_GET(SHORT, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)                \
    static __inline UInt32  SHORT##_##REG_MNEM##_READ##_##FLD_MNEM(BC_TYPE*  pBc)        \
    {                                                                                    \
        DT_BC_FIELD_READ_BODY(SHORT, OFFSET, REG_MNEM, FLD_MNEM);                        \
    }

// Macro defining the body of a register WRITE function
#define DT_BC_REG_WRITE_BODY(MSB_BIT, NUM_BITS)                                          \
    do                                                                                   \
    {                                                                                    \
        DT_ASSERT((Value & ~(DT_BC_FIELD_MASK(NUM_BITS)                                  \
                                       << DT_BC_FIELD_SHIFT(MSB_BIT, NUM_BITS))) == 0);  \
        DtBc_RegWrite32((DtBc*)pBc, Offset, Value);                                      \
    } while(0)

// Macro defining a register WRITE access function
#define DT_BC_REG_WRITE(SHORT, BC_TYPE, OFFSET, REG_MNEM, MSB_BIT, NUM_BITS)             \
    static __inline void  SHORT##_##REG_MNEM##_WRITE(BC_TYPE*  pBc, UInt32  Value)       \
    {                                                                                    \
        const UInt32  Offset = OFFSET;                                                   \
        DT_BC_REG_WRITE_BODY(MSB_BIT, NUM_BITS);                                         \
    }
// Macro defining a register WRITE access function. With a varaible offset
#define DT_BC_REG_WRITE_VAR(SHORT, BC_TYPE, REG_MNEM, MSB_BIT, NUM_BITS)                 \
    static __inline void  SHORT##_##REG_MNEM##_WRITE(BC_TYPE*  pBc, UInt32  Offset,      \
                                                                         UInt32  Value)  \
    {                                                                                    \
        DT_BC_REG_WRITE_BODY(MSB_BIT, NUM_BITS);                                         \
    }

// Macro defining the body of a field WRITE function
#define DT_BC_FIELD_WRITE_BODY(SHORT, OFFSET, REG_MNEM, FLD_MNEM)                        \
    do                                                                                   \
    {                                                                                    \
        UInt32  Reg_Value = DtBc_RegRead32((DtBc*)pBc, OFFSET);                          \
        Reg_Value = SHORT##_##REG_MNEM##_SET##_##FLD_MNEM(Reg_Value, Value);             \
        DtBc_RegWrite32((DtBc*)pBc, OFFSET, Reg_Value);                                  \
    } while (0)

// Macro defining a field SET access function
#define DT_BC_FIELD_SET(SHORT, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)                    \
    static __inline UInt32  SHORT##_##REG_MNEM##_SET##_##FLD_MNEM(                       \
                                                       UInt32  Reg_Value, UInt32  Value) \
    {                                                                                    \
        const UInt32  Shift=DT_BC_FIELD_SHIFT(MSB_BIT, NUM_BITS);                        \
        const UInt32  Mask=DT_BC_FIELD_MASK(NUM_BITS);                                   \
        Reg_Value &= ~((Mask)<<Shift);                                                   \
        Reg_Value |= (Value&Mask)<<Shift;                                                \
        return Reg_Value;                                                                \
    }
// Macro defining a field WRITE+SET access functions
#define DT_BC_FIELD_WRITE(SHORT, BC_TYPE, OFFSET, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS) \
    DT_BC_FIELD_SET(SHORT, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)                        \
    static __inline void  SHORT##_##REG_MNEM##_WRITE##_##FLD_MNEM(                       \
                                                           BC_TYPE*  pBc, UInt32  Value) \
    {                                                                                    \
        DT_BC_FIELD_WRITE_BODY(SHORT, OFFSET, REG_MNEM, FLD_MNEM);                       \
    }
// Macro defining a field WRITE+SET access functions. With variable offset
#define DT_BC_FIELD_WRITE_VAR(SHORT, BC_TYPE, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS) \
    DT_BC_FIELD_SET(SHORT, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)                        \
    static __inline void  SHORT##_##REG_MNEM##_WRITE##_##FLD_MNEM(BC_TYPE*  pBc,         \
                                                          UInt32  Offset, UInt32  Value) \
    {                                                                                    \
        DT_BC_FIELD_WRITE_BODY(SHORT, Offset, REG_MNEM, FLD_MNEM);                       \
    }

// Macro defining register READ+WRITE access functions
#define DT_BC_REG_RW(SHORT, BC_TYPE, OFFSET, REG_MNEM, MSB_BIT, NUM_BITS)                \
    DT_BC_REG_READ(SHORT, BC_TYPE, OFFSET, REG_MNEM, MSB_BIT, NUM_BITS)                  \
    DT_BC_REG_WRITE(SHORT, BC_TYPE, OFFSET, REG_MNEM, MSB_BIT, NUM_BITS)
// Macro defining register READ+WRITE access functions. With variable offset
#define DT_BC_REG_RW_VAR(SHORT, BC_TYPE, REG_MNEM, MSB_BIT, NUM_BITS)                    \
    DT_BC_REG_READ_VAR(SHORT, BC_TYPE, REG_MNEM, MSB_BIT, NUM_BITS)                      \
    DT_BC_REG_WRITE_VAR(SHORT, BC_TYPE, REG_MNEM, MSB_BIT, NUM_BITS)

// Macro defining field READ+GET + WRITE+SET access functions
#define DT_BC_FIELD_RW(SHORT, BC_TYPE, OFFSET, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)    \
    DT_BC_FIELD_READ(SHORT, BC_TYPE, OFFSET, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)      \
    DT_BC_FIELD_WRITE(SHORT, BC_TYPE, OFFSET, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)
// Macro defining field READ+GET + WRITE+SET access functions. With variable offset
#define DT_BC_FIELD_RW_VAR(SHORT, BC_TYPE, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)        \
    DT_BC_FIELD_READ_VAR(SHORT, BC_TYPE, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)          \
    DT_BC_FIELD_WRITE_VAR(SHORT, BC_TYPE, REG_MNEM, FLD_MNEM, MSB_BIT, NUM_BITS)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Common Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Generic 'BlockId' Register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Block ID register: Offset=0x00
static const Int BC_REG_BLOCKID = 0x00;

// Register READ+WRITE function
DT_BC_REG_RW(BC, DtBc, BC_REG_BLOCKID, BlockId, 31, 32)
// Field READ+GET functions
DT_BC_FIELD_READ(BC, DtBc, BC_REG_BLOCKID, BlockId, Version, 7, 8)
DT_BC_FIELD_READ(BC, DtBc, BC_REG_BLOCKID, BlockId, NumInterrupts, 11, 4)
DT_BC_FIELD_READ(BC, DtBc, BC_REG_BLOCKID, BlockId, IdChar, 23, 8)
// Field READ+GET + WRITE+SET functions
DT_BC_FIELD_RW(BC, DtBc, BC_REG_BLOCKID, BlockId, IdIndex, 28, 5)

//-.-.-.-.-.-.-.-.-.-.-.-.- Generic 'Interrupt-Status' Register -.-.-.-.-.-.-.-.-.-.-.-.-.
// NOTE: Multiple Interrupt-Status registers may exists => use variable offset functions

// Register READ+WRITE function
DT_BC_REG_RW_VAR(BC, DtBc, InterruptStatus, 31, 32)
// Field READ+GET functions
DT_BC_FIELD_READ_VAR(BC, DtBc, InterruptStatus, Status, 0, 1)
DT_BC_FIELD_READ_VAR(BC, DtBc, InterruptStatus, InterruptIndex, 15, 12)
DT_BC_FIELD_READ_VAR(BC, DtBc, InterruptStatus, InterruptId, 31, 16)
// Field READ+GET + WRITE+SET functions
DT_BC_FIELD_RW_VAR(BC, DtBc, InterruptStatus, Enable, 1, 1)

#endif  // #ifndef __DT_BC_REGS_H
