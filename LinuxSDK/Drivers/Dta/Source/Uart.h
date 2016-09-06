//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Uart.h *#*#*#*#*#*#*#*#*#* (C) 2015-2016 DekTec
//
// Dta driver - Declares UART related functions
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2015-2016 DekTec Digital Video B.V.
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

#ifndef __UART_H
#define __UART_H

// Interrupt reasons, stored in DtDpcArgs::m_Data1.m_UInt32_1
#define  DTA_UART_INT_RX_IDLE       1
#define  DTA_UART_INT_RX_HALFFULL   2
#define  DTA_UART_INT_TX_EMPTY      4
#define  DTA_UART_INT_TX_HALFEMPTY  8

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaUartPort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct
{
    const DtFwbSerialCommCtrl*  m_pFwbUart;
    volatile UInt8*  m_pFwbRegs;
    //DtSpinLock  m_StateLock;       // Spinlock to protect state
    DtDpc  m_IntDpc;
    DtEvent  m_TxEvent;
    DtEvent  m_RxEvent;
} DtaUartPort;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
 
DtStatus  DtaUartInitPowerup(DtaUartPort* pUart, volatile UInt8*  pFwbRegs);
DtStatus  DtaUartInit(DtaUartPort* pUart, const DtFwbSerialCommCtrl* pFwbUart);
Bool  DtaUartInterrupt(DtaUartPort*  pUart);
DtStatus  DtaUartReset(DtaUartPort* pUart);
DtStatus  DtaUartWrite(DtaUartPort* pUart, UInt8* pBuf, Int BytesToWrite, Int* pTimeout);
DtStatus  DtaUartRead(DtaUartPort* pUart, UInt8* pBuf, Int BytesToRead, Int Timeout,
                                                                      Int* pNumBytesRead);
#endif // __UART_H
