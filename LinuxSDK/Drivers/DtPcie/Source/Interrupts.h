// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Interrupts.h *#*#*#*#*#*#*#*# (C) 2017-2022 DekTec
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

#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants / Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Interrupt IDs
#define  DT_INTERRUPT_TOD_PERIODIC          1
#define  DT_INTERRUPT_SPIMF_CMDDONE         2
#define  DT_INTERRUPT_SPIMF_TXHALFEMPTY     3
#define  DT_INTERRUPT_SPIMF_RXHALFFULL      4
// #define  DT_INTERRUPT_SDIRXF_SOF          5 (Not used anymore)
#define  DT_INTERRUPT_SDIRXF_FMTEVENT       6
#define  DT_INTERRUPT_I2CM_CMDDONE          7
#define  DT_INTERRUPT_LNBH25_2132_TXDONE    8
#define  DT_INTERRUPT_LNBH25_2132_RXDONE    9
#define  DT_INTERRUPT_LNBH25_2132_FAULT     10
#define  DT_INTERRUPT_DISEQC_TXDONE         8
#define  DT_INTERRUPT_DISEQC_RXDONE         9
#define  DT_INTERRUPT_DISEQC_FAULT          10
#define  DT_INTERRUPT_S2STATS_2132_FIFOHALFFULL  11
#define  DT_INTERRUPT_IQCGRAB_2132_DONE     12
#define  DT_INTERRUPT_S2DEMOD_2132_CREONIX  13
#define  DT_INTERRUPT_SDITXF_FMTEVENT       15
#define  DT_INTERRUPT_CONSTSOURCE_TESTDONE  16
#define  DT_INTERRUPT_CONSTSINK_TESTDONE    17
#define  DT_INTERRUPT_GENL_STARTOFFRAME     18
#define  DT_INTERRUPT_LMH1981_STARTOFFRAME  19
#define  DT_INTERRUPT_EMAC10G_LINK_STATUS   21


// Signature of the interrupt handler callback function
typedef  DtStatus  (*DtBcIntHandlerFunc)(DtBc*, Int  Id, Int  Index, void*);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIntHandlerRegData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Registration data for an interrupt handler
typedef struct  _DtBcIntHandlerRegData
{
    DT_INT_HANDLER_REGDATA_COMMON_DATA;
    Int  m_Id;                  // Interrupt ID
    Int  m_Index;               // Interrupt index
    DtBc*  m_pBc;               // Block for which the handler is called
    DtBcIntHandlerFunc  m_IntHandlerFunc;  // The handler function to call
}  DtBcIntHandlerRegData;
#define INIT_DT_BC_INT_HANDLER_REG_DATA(DATA, ID, INDEX, BLOCK, HANDLER, CONTEXT)      \
do                                                                                       \
{                                                                                        \
    INIT_DT_INT_HANDLER_REG_DATA(DATA, CONTEXT);                                         \
    DATA.m_Id = ID;                                                                      \
    DATA.m_Index = INDEX;                                                                \
    DATA.m_pBc = BLOCK;                                                            \
    DATA.m_IntHandlerFunc = HANDLER;                                                     \
} while (0)

#endif  // #ifndef __INTERRUPTS_H
