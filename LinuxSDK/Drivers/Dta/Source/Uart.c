//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Uart.c *#*#*#*#*#*#*#*#*#* (C) 2015-2016 DekTec
//
// Dta driver -  UART read/write operations
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtaIncludes.h>
#include <Uart.h>


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static void  DtaUartIntDpc(DtDpcArgs* pArgs);

// UART states
#define UART_TX_IDLE            0
#define UART_TX_TRANSMIT        1
#define UART_RX_IDLE            0
#define UART_RX_RECEIVE         1

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaUartInitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaUartInitPowerup(DtaUartPort* pUart, volatile UInt8*  pFwbRegs)
{
    pUart->m_pFwbRegs = pFwbRegs;

    return DtaUartReset(pUart);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaUartInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaUartInit(DtaUartPort* pUart, const DtFwbSerialCommCtrl* pFwbUart)
{
    pUart->m_pFwbUart = pFwbUart;
    
    DtDpcInit(&pUart->m_IntDpc, DtaUartIntDpc, TRUE);

    // Init transmitter
    DtEventInit(&pUart->m_TxEvent, TRUE);

    // Init receiver
    DtEventInit(&pUart->m_RxEvent, TRUE);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaUartReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaUartReset(DtaUartPort* pUart)
{
    // Init transmitter
    DtaFwbRegWrite(pUart->m_pFwbRegs, &pUart->m_pFwbUart->TxCtrl_Reset, 1);

    // Init receiver
    DtaFwbRegClear(pUart->m_pFwbRegs, &pUart->m_pFwbUart->RxStat_FifoOverflow);
    DtaFwbRegWrite(pUart->m_pFwbRegs, &pUart->m_pFwbUart->RxCtrl_Reset, 1);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaUartInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaUartInterrupt(DtaUartPort*  pUart)
{
    volatile UInt8* pFwbRegs = pUart->m_pFwbRegs;
    const DtFwbSerialCommCtrl* pFwbUart = pUart->m_pFwbUart;
    UInt32  IntReason = 0;
    // Check for RX idle interrupt
    if (DtaFwbRegRead(pFwbRegs, &pFwbUart->RxStat_DataIdleIntStatus))
    {
        // Disable and clear interrupts
        DtaFwbRegWrite(pFwbRegs, &pFwbUart->RxCtrl_DataIdleIntEnable, 0);
        DtaFwbRegClear(pFwbRegs, &pFwbUart->RxStat_DataIdleIntStatus);
        IntReason |= DTA_UART_INT_RX_IDLE;
    }
    // Check for RX half full interrupt
    if (DtaFwbRegRead(pFwbRegs, &pFwbUart->RxStat_HalfFullIntStatus))
    {
        // Disable and clear interrupts
        DtaFwbRegWrite(pFwbRegs, &pFwbUart->RxCtrl_HalfFullIntEnable, 0);
        DtaFwbRegClear(pFwbRegs, &pFwbUart->RxStat_HalfFullIntStatus);
        IntReason |= DTA_UART_INT_RX_HALFFULL;
    }
    // Check for TX empty interrupt
    if (DtaFwbRegRead(pFwbRegs, &pFwbUart->TxStat_EmptyIntStatus))
    {
        // Disable and clear interrupts
        DtaFwbRegWrite(pFwbRegs, &pFwbUart->TxCtrl_EmptyIntEnable, 0);
        DtaFwbRegClear(pFwbRegs, &pFwbUart->TxStat_EmptyIntStatus);
        IntReason |= DTA_UART_INT_TX_EMPTY;
    }
    // Check for TX half empty interrupt
    if (DtaFwbRegRead(pFwbRegs, &pFwbUart->TxStat_HalfEmptyIntStatus))
    {
        // Disable and clear interrupts
        DtaFwbRegWrite(pFwbRegs, &pFwbUart->TxCtrl_HalfEmptyIntEnable, 0);
        DtaFwbRegClear(pFwbRegs, &pFwbUart->TxStat_HalfEmptyIntStatus);
        IntReason |= DTA_UART_INT_TX_HALFEMPTY;
    }
    if (IntReason != 0)
    {
        // Schedule DPC to handle the interrupt.
        DtDpcArgs  DpcArgs;
        DpcArgs.m_pContext = pUart;
        DpcArgs.m_Data1.m_UInt32_1 = IntReason; // Interrupt reason
        DpcArgs.m_Data1.m_UInt32_2 = 0;
        DtDpcSchedule(&pUart->m_IntDpc, &DpcArgs);

        // Interrupt was ours
        return TRUE;
    }
    return FALSE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaUartIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaUartIntDpc(DtDpcArgs* pArgs)
{
    DtaUartPort*  pUart = (DtaUartPort*)pArgs->m_pContext;
    UInt32 IntReason = pArgs->m_Data1.m_UInt32_1;

    // Check for TX interrupt
    if ((IntReason&DTA_UART_INT_TX_EMPTY)!=0 || (IntReason&DTA_UART_INT_TX_HALFEMPTY)!=0)
        DtEventSet(&pUart->m_TxEvent);

    // Check for RX interrupt
    if ((IntReason&DTA_UART_INT_RX_IDLE)!=0 || (IntReason&DTA_UART_INT_RX_HALFFULL)!=0)
        DtEventSet(&pUart->m_RxEvent);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaUartWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaUartRead(
    DtaUartPort*  pUart,
    UInt8*  pBuf,
    Int  BytesToRead,
    Int  Timeout,
    Int*  pNumBytesRead)
{
    DtStatus  Status = DT_STATUS_OK; 
    Int  NumBytesAvail, NumCanRead, FifoSize, i;
    Int Idx = 0;
    volatile UInt8* pFwbRegs = pUart->m_pFwbRegs;
#ifdef WINBUILD
    LARGE_INTEGER  StartTime, CurTime;
#else
    struct timespec  StartTime, CurTime;
#endif
    Int  TimeElapsed;
    Int  OrigTimeout = Timeout;

    *pNumBytesRead = 0;
    
    // Check timeout
    if (Timeout < -1)
        return DT_STATUS_INVALID_PARAMETER;


#ifdef WINBUILD
    KeQueryTickCount(&StartTime);
#else
    getnstimeofday(&StartTime);
#endif

    FifoSize = DtaFwbRegRead(pFwbRegs,  &pUart->m_pFwbUart->Config_RxFifoSize);
    NumBytesAvail = DtaFwbRegRead(pFwbRegs, &pUart->m_pFwbUart->RxStat_FifoLoad);
    do
    {
        NumCanRead = (BytesToRead < NumBytesAvail) ? BytesToRead : NumBytesAvail;
        // Read available data from FIFO
        for (i=0; i<NumCanRead; i++)
            pBuf[Idx++] = (UInt8)DtaFwbRegRead(pFwbRegs, &pUart->m_pFwbUart->RxData);

        BytesToRead -= NumCanRead;
        *pNumBytesRead += NumCanRead;

        // Ready?
        if (BytesToRead==0 || Timeout==0)
            break;

        // Wait for data available event
        DtEventReset(&pUart->m_RxEvent);
        if (BytesToRead < FifoSize/2)
            DtaFwbRegWrite(pFwbRegs, &pUart->m_pFwbUart->RxCtrl_DataIdleIntEnable, 1);
        else
            DtaFwbRegWrite(pFwbRegs, &pUart->m_pFwbUart->RxCtrl_HalfFullIntEnable, 1);
        Status = DtEventWait(&pUart->m_RxEvent, Timeout);
        if (!DT_SUCCESS(Status))
            break;

        // Check how much is availble now
        NumBytesAvail = DtaFwbRegRead(pFwbRegs, &pUart->m_pFwbUart->RxStat_FifoLoad);

        if (Timeout == -1)
            continue;

#ifdef WINBUILD
        KeQueryTickCount(&CurTime);
        TimeElapsed = (Int)((CurTime.QuadPart - StartTime.QuadPart)*KeQueryTimeIncrement()
                                                                                 / 10000);
#else
        getnstimeofday(&CurTime);
        TimeElapsed = DtDivide64(((CurTime.tv_sec-StartTime.tv_sec)*1000000000LL
                                   + (CurTime.tv_nsec-StartTime.tv_nsec)), 1000000, NULL);
#endif

        Timeout = OrigTimeout - TimeElapsed;
    } while (Timeout > 0);
    
    // Reception ready
    DtaFwbRegWrite(pFwbRegs, &pUart->m_pFwbUart->RxCtrl_DataIdleIntEnable, 0);
    DtaFwbRegWrite(pFwbRegs, &pUart->m_pFwbUart->RxCtrl_HalfFullIntEnable, 0);

    // A timeout is not an error in this case. Let userspace deal with the fact that
    // the read operation is not complete.
    if (Status == DT_STATUS_TIMEOUT)
        Status = DT_STATUS_OK;

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaUartWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaUartWrite(
    DtaUartPort*  pUart,
    UInt8*  pBuf,
    Int  NumBytes,
    Int*  pTimeout)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  FifoSize, FifoFree;
    volatile UInt8* pFwbRegs = pUart->m_pFwbRegs;
#ifdef WINBUILD
    LARGE_INTEGER  StartTime, CurTime;
#else
    struct timespec  StartTime, CurTime;
#endif
    Int  TimeElapsed = 0;

#ifdef WINBUILD
    KeQueryTickCount(&StartTime);
#else
    getnstimeofday(&StartTime);
#endif

    if (pTimeout==NULL || *pTimeout<=0)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Get current space in FIFO
    FifoSize = DtaFwbRegRead(pFwbRegs,  &pUart->m_pFwbUart->Config_TxFifoSize);
    FifoFree = FifoSize - DtaFwbRegRead(pFwbRegs, &pUart->m_pFwbUart->TxStat_FifoLoad);

    while (NumBytes > FifoFree)
    {
        // Write as many bytes as fit in the buffer
        while (FifoFree-- > 0)
        {
            DtaFwbRegWrite(pFwbRegs, &pUart->m_pFwbUart->TxData, *pBuf++);
            NumBytes--;
        }
        DtaFwbRegWrite(pFwbRegs, &pUart->m_pFwbUart->TxCtrl_HalfEmptyIntEnable, 1);
        Status = DtEventWaitUnInt(&pUart->m_TxEvent, *pTimeout - TimeElapsed);
#ifdef WINBUILD
        KeQueryTickCount(&CurTime);
        TimeElapsed = (Int)((CurTime.QuadPart - StartTime.QuadPart)*KeQueryTimeIncrement()
                                                                                 / 10000);
#else
        getnstimeofday(&CurTime);
        TimeElapsed = DtDivide64(((CurTime.tv_sec-StartTime.tv_sec)*1000000000LL
                                   + (CurTime.tv_nsec-StartTime.tv_nsec)), 1000000, NULL);
#endif
        if (TimeElapsed > *pTimeout)
            return DT_STATUS_TIMEOUT;

        FifoFree = FifoSize - DtaFwbRegRead(pFwbRegs,&pUart->m_pFwbUart->TxStat_FifoLoad);
    }
    DT_ASSERT(NumBytes <= FifoFree);
    while (NumBytes-- > 0)
    {
        DtaFwbRegWrite(pFwbRegs, &pUart->m_pFwbUart->TxData, *pBuf++);
    }
    
#ifdef WINBUILD
        KeQueryTickCount(&CurTime);
        TimeElapsed = (Int)((CurTime.QuadPart - StartTime.QuadPart)*KeQueryTimeIncrement()
                                                                                 / 10000);
#else
        getnstimeofday(&CurTime);
        TimeElapsed = DtDivide64(((CurTime.tv_sec-StartTime.tv_sec)*1000000000LL
                                   + (CurTime.tv_nsec-StartTime.tv_nsec)), 1000000, NULL);
#endif
    if (TimeElapsed > *pTimeout)
        return DT_STATUS_TIMEOUT;

    // Wait for completion
    DtEventReset(&pUart->m_TxEvent);
    DtaFwbRegWrite(pFwbRegs, &pUart->m_pFwbUart->TxCtrl_EmptyIntEnable, 1);
    Status = DtEventWaitUnInt(&pUart->m_TxEvent, *pTimeout - TimeElapsed);

    if (!DT_SUCCESS(Status))
    {
#ifdef WINBUILD
        KeQueryTickCount(&CurTime);
        TimeElapsed = (Int)((CurTime.QuadPart - StartTime.QuadPart)*KeQueryTimeIncrement()
                                                                                 / 10000);
#else
        getnstimeofday(&CurTime);
        TimeElapsed = DtDivide64(((CurTime.tv_sec-StartTime.tv_sec)*1000000000LL
                                   + (CurTime.tv_nsec-StartTime.tv_nsec)), 1000000, NULL);
#endif
        *pTimeout -= TimeElapsed;
        return Status;
    }
    return DT_STATUS_OK;
}
