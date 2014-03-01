//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Utility.c *#*#*#*#*#*#*#*#*# (C) 2000-2008 DekTec
//
// Miscellaneous utility routines for the Dtu2xx driver.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dtu2xx driver, please refer to the Dtu2xx readme

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "Dtu2xx.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxMode2PacketSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Computes the packet size based on the packet transmit-mode
//
Int Dtu2xxTxMode2PacketSize(
	Int TxPacketMode)		// Packet transmit mode (TxMode, BurstMode)
{
	switch (TxPacketMode & DTU2XX_TXMODE_MASK)
	{
	case DTU2XX_TXMODE_188:		return 188;
	case DTU2XX_TXMODE_192:		return 192;
	case DTU2XX_TXMODE_204:		return 204;
	case DTU2XX_TXMODE_ADD16:	return 204;
	case DTU2XX_TXMODE_RAW:		return 188;
	case DTU2XX_TXMODE_MIN16:	return 204;
	default:					return 188;		// Should not occur
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxBuf2RingBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Copies data from a contigious buffer to a ring buffer.
//
// Returns to number of copied bytes
//
//
Int Dtu2xxBuf2RingBuffer(
	IN UInt8* pBuf,						// buffer with data to copy
	IN Int NumToCopy,					// Number of bytes to copy
	IN PDTU2XX_RING_BUFFER pRingBuf)	// our ring buffer
{
	Int CopyCount, BytesToWrap;
	if ( NumToCopy == 0 ) 
		return 0;

	// Pre-conditions
	//ASSERT ( (pRingBuf->m_Load >= 0) && (pRingBuf->m_Load <= pRingBuf->m_TotalSize) );
	//ASSERT ( pRingBuf->m_pWrPtr >= pRingBuf->m_pStart );
	//ASSERT ( pRingBuf->m_pWrPtr < (pRingBuf->m_pStart+pRingBuf->m_TotalSize) );
	//ASSERT ( pRingBuf->m_pRdPtr >= pRingBuf->m_pStart );
	//ASSERT ( pRingBuf->m_pRdPtr < (pRingBuf->m_pStart+pRingBuf->m_TotalSize) );
	
	// Check for overflow
	if ( NumToCopy > (pRingBuf->m_TotalSize - pRingBuf->m_Load) )
		CopyCount = (pRingBuf->m_TotalSize - pRingBuf->m_Load);
	else
		CopyCount = NumToCopy;

	if (CopyCount == 0 )
		return 0;

	// Check when the buffer wraps
	BytesToWrap = pRingBuf->m_TotalSize - (pRingBuf->m_pWrPtr - pRingBuf->m_pStart);

	if ( BytesToWrap >= CopyCount ) {
		// No wrap problem => copy all in one go
		memcpy(pRingBuf->m_pWrPtr, pBuf, CopyCount);

		// Update write pointer
		if ( CopyCount == BytesToWrap )
			pRingBuf->m_pWrPtr = pRingBuf->m_pStart;
		else
			pRingBuf->m_pWrPtr += CopyCount;
	}
	else {
#if LOG_LEVEL_DMA > 2
		DTU2XX_LOG(KERN_INFO, "Dtu2xxBuf2RingBuffer: Ring buffer wraps");
#endif

        // First copy upto wrap point
		memcpy(pRingBuf->m_pWrPtr, pBuf, BytesToWrap);
		pRingBuf->m_pWrPtr = pRingBuf->m_pStart;
		// Copy remainder
		memcpy(pRingBuf->m_pWrPtr, &(pBuf[BytesToWrap]), (CopyCount-BytesToWrap));
		pRingBuf->m_pWrPtr += (CopyCount-BytesToWrap);
	}
	// Update load
	pRingBuf->m_Load += CopyCount;

	// Post-conditions
	//ASSERT ( (pRingBuf->m_Load >= 0) && (pRingBuf->m_Load <= pRingBuf->m_TotalSize) );
	//ASSERT ( pRingBuf->m_pWrPtr >= pRingBuf->m_pStart );
	//ASSERT ( pRingBuf->m_pWrPtr < (pRingBuf->m_pStart+pRingBuf->m_TotalSize) );
	//ASSERT ( pRingBuf->m_pRdPtr >= pRingBuf->m_pStart );
	//ASSERT ( pRingBuf->m_pRdPtr < (pRingBuf->m_pStart+pRingBuf->m_TotalSize) );

	return CopyCount; 
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRingBuffer2Buf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Copies data from a ring buffer to a contigious buffer
//
// Returns to number of copied bytes
//
Int Dtu2xxRingBuffer2Buf(
	IN PDTU2XX_RING_BUFFER pRingBuf,	// our ring buffer
	IN Int NumToCopy,					// Number of bytes to copy		
	OUT UInt8* pBuf)					// buffer to receive the data
{
	Int CopyCount, BytesToWrap;
	if ( NumToCopy == 0 ) 
		return 0;

	// Pre-conditions
	//ASSERT ( (pRingBuf->m_Load >= 0) && (pRingBuf->m_Load <= pRingBuf->m_TotalSize) );
	//ASSERT ( pRingBuf->m_pWrPtr >= pRingBuf->m_pStart );
	//ASSERT ( pRingBuf->m_pWrPtr < (pRingBuf->m_pStart+pRingBuf->m_TotalSize) );
	//ASSERT ( pRingBuf->m_pRdPtr >= pRingBuf->m_pStart );
	//ASSERT ( pRingBuf->m_pRdPtr < (pRingBuf->m_pStart+pRingBuf->m_TotalSize) );
	
	// Check for underflow
	if ( NumToCopy > pRingBuf->m_Load )
		CopyCount = pRingBuf->m_Load;
	else
		CopyCount = NumToCopy;

	if (CopyCount == 0 )
		return 0;

	// Check when the buffer wraps
	BytesToWrap = pRingBuf->m_TotalSize - (pRingBuf->m_pRdPtr - pRingBuf->m_pStart);

	if ( BytesToWrap >= CopyCount ) {
		// No wrap problem => copy all in one go
		memcpy(pBuf, pRingBuf->m_pRdPtr, CopyCount);

		// Update write pointer
		if ( CopyCount == BytesToWrap )
			pRingBuf->m_pRdPtr = pRingBuf->m_pStart;
		else
			pRingBuf->m_pRdPtr += CopyCount;
	}
	else {

#if LOG_LEVEL_DMA > 2
		DTU2XX_LOG(KERN_INFO, "Dtu2xxRingBuffer2Buf: Ring buffer wraps");
#endif
        // First copy upto wrap point
		memcpy(pBuf, pRingBuf->m_pRdPtr, BytesToWrap);
		pRingBuf->m_pRdPtr = pRingBuf->m_pStart;
		// Copy remainder
		memcpy(&(pBuf[BytesToWrap]), pRingBuf->m_pRdPtr, (CopyCount-BytesToWrap));
		pRingBuf->m_pRdPtr += (CopyCount-BytesToWrap);
	}
	// Update load
	pRingBuf->m_Load -= CopyCount;

	// Post-conditions
	//ASSERT ( (pRingBuf->m_Load >= 0) && (pRingBuf->m_Load <= pRingBuf->m_TotalSize) );
	//ASSERT ( pRingBuf->m_pWrPtr >= pRingBuf->m_pStart );
	//ASSERT ( pRingBuf->m_pWrPtr < (pRingBuf->m_pStart+pRingBuf->m_TotalSize) );
	//ASSERT ( pRingBuf->m_pRdPtr >= pRingBuf->m_pStart );
	//ASSERT ( pRingBuf->m_pRdPtr < (pRingBuf->m_pStart+pRingBuf->m_TotalSize) );
	
	return CopyCount; 
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxBinDiv -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// 64 bit division  returns  num/denom and rest
//
Int64 Dtu2xxBinDiv(Int64 num, Int64 denom, Int64* pRest)
{

    Int denombits, numbits ;
    Int64 restbits, restmask, rest;
    Int64 answer = 0;
    Int sign = 0;

    if (num < 0) {
        num = -num;
        sign = 1;
    }
    if (denom < 0) {
        denom = -denom;
        sign ^= 1;
    }

    if (num < denom)
        return 0;

    denombits = 0;
    while ((Int64)1 << denombits < denom)
        denombits++;

    numbits = denombits;
    while ((Int64)1 << numbits < num)
        numbits++;

    restbits = numbits - denombits;
    restmask = ((Int64)1 << restbits) - 1;
    rest = num >> restbits;
    num = num & restmask;

    while (1)
    {
        answer <<= 1;
        if (rest >= denom)
        {
            answer |= 1;
            rest -= denom;
        }

        if (restbits == 0)
            break;
        restbits--;
        restmask >>= 1;
        rest = (rest << 1) | (num >> restbits);
        num = num & restmask;
    }

    if (pRest)
        *pRest = rest;

    if (sign)
        return -answer;
    return answer;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxBinUDiv -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// 64 bit unsigned division  returns  num/denom and rest
//
UInt64 Dtu2xxBinUDiv(UInt64 num, UInt64 denom, UInt64* pRest)
{

    Int denombits, numbits ;
    UInt64 restbits, restmask, rest;
    UInt64 answer = 0;
    
    if (num < denom)
        return 0;

    denombits = 0;
    while ((UInt64)1 << denombits < denom)
        denombits++;

    numbits = denombits;
    while ((UInt64)1 << numbits < num)
        numbits++;

    restbits = numbits - denombits;
    restmask = ((UInt64)1 << restbits) - 1;
    rest = num >> restbits;
    num = num & restmask;

    while (1)
    {
        answer <<= 1;
        if (rest >= denom)
        {
            answer |= 1;
            rest -= denom;
        }

        if (restbits == 0)
            break;
        restbits--;
        restmask >>= 1;
        rest = (rest << 1) | (num >> restbits);
        num = num & restmask;
    }

    if (pRest)
        *pRest = rest;

    return answer;
}
