//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Genlock.c *#*#*#*#*#*#*#*#*# (C) 2008-2009 DekTec
//
// Routines for handling Genlock


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#ifdef WINBUILD
	#include "Dta1xx.h"
#else
	#include "../Include/Dta1xx.h"
	#include "../Include/Dta1xxRegs.h"
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetVcxoSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// range 0 .. 1M
//
NTSTATUS Dta1xxSetVcxoSpeed(PDTA1XX_FDO_EXTENSION  pFdoExt)
{
	volatile UInt8*  pGenRegs;	
	UInt Speed;
	UInt8 I2CData[2];
#ifdef WINBUILD
	pGenRegs = (volatile UInt8*)pFdoExt->m_pDta1xxGen;
#else
	pGenRegs = pFdoExt->m_pGenRegs;
#endif
	Speed = 0;

	if (pFdoExt->m_TypeNumber != 2144) {
		Speed = pFdoExt->m_VcxoCtrl / 1000; // convert range to 0 .. 1000, 512 is center
		Dta1xxGenSetVcxoSpeed(pGenRegs, Speed);
	}
	else 
	{
		Speed = pFdoExt->m_VcxoCtrl / 250; // convert range to 0 .. 4000, 2048 is center

#if LOG_LEVEL_GENLOCK > 1
		DTA1XX_LOG(KERN_INFO, "Dta1xxSetVcxoSpeed: Speed = %d", Speed);				
#endif
		if (pFdoExt->m_I2cBusy == 1)
		{
			if (Dta1xxGenI2cStatusRegGetRdyInt(pGenRegs) == 0)
			{
				if (pFdoExt->m_I2cWait < 25)
				{
					pFdoExt->m_I2cWait++;
#if LOG_LEVEL_GENLOCK > 1
					DTA1XX_LOG(KERN_INFO, "Dta1xxSetVcxoSpeed: I2C Wait %d",
						       pFdoExt->m_I2cWait);
#endif
					return STATUS_SUCCESS;
				}
			}
		}

		I2CData[0] = (char) ((Speed & 0xff00) >> 8);	// MSB VCXO setting	
		I2CData[1] = (char) (Speed & 0xff);				// LSB VCXO setting

		Speed = (I2CData[1] << 8) + I2CData[0];

#if LOG_LEVEL_GENLOCK > 1
		DTA1XX_LOG(KERN_INFO, "Dta1xxSetVcxoSpeed: Write I2C Data MSB %x LSB %x",
				   I2CData[1], I2CData[0]);				
#endif

		// First write I2C data to onboard I2C buffer (4 bytes at a time)
		Dta1xxGenSetI2cData(pGenRegs, Speed);	

#if LOG_LEVEL_GENLOCK > 1
		DTA1XX_LOG(KERN_INFO, "Dta1xxSetVcxoSpeed: I2C Command device write address=%x, "
			       "length=%x", 0x18, 2);
#endif

		// Start I2C write transfer by writing device adress (7 MSBs only) and transfer count
		Dta1xxGenI2cAddrCntRegSetWrAddrAndCount(pGenRegs, (0x18>>1), 2);

		pFdoExt->m_I2cBusy = 1;			
		pFdoExt->m_I2cWait = 0;	
	}
	return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxUpdateGenlockTiming -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxUpdateGenlockTiming(IN PDTA1XX_FDO_EXTENSION  pFdoExt)
{
	UInt64 RefCnt;
	volatile UInt8*  pGenRegs;
	pFdoExt->m_ValidRefCnt = FALSE;

#ifdef WINBUILD
	pGenRegs = (volatile UInt8*)pFdoExt->m_pDta1xxGen;
#else
	pGenRegs = pFdoExt->m_pGenRegs;
#endif

	// obtain periodic interrupt latched reference count
	RefCnt = Dta1xxGenGetRefClkCntLReg64(pGenRegs);

	// DTA-145/2144/2145: 
	// Divide by 2 to convert from 54Mhz to to 27Mhz
	RefCnt = RefCnt/2;					

	// ****************************
	// Videostandard Helper
	// ****************************
	if (pFdoExt->m_Genlock_vidstd == DTA1XX_GENLOCK_SDI625) 
	{
		pFdoExt->m_LinesPerFrame = 625;
		pFdoExt->m_SamplesPerLine = 1728;
	}
	else
	{
		pFdoExt->m_LinesPerFrame = 525;
		pFdoExt->m_SamplesPerLine = 1716;
	}

	// ****************************
	// Determine time passed since last Update
	// ****************************

	if (RefCnt < pFdoExt->m_RefCntprev) 
	{		

#if LOG_LEVEL_GENLOCK > 1
		DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTiming: Notice: RefCnt WRAP, Warning: "
			       "Skipping one control iteration");
#endif						
		// When the reference counters wraps, skip further handling		
	}
	else 
	{
		pFdoExt->m_RefCntDelta = (RefCnt - pFdoExt->m_RefCntprev);		

		// 27Mhz Refcounter converted to number of SDI LinesPerFrame lines
#ifdef WINBUILD
		pFdoExt->m_RefLinesDelta = (pFdoExt->m_RefCntDelta) /
											pFdoExt->m_SamplesPerLine;
#else
		// 64-bit division are not allowed in the kernel => use Dta1xxBinDiv function
		pFdoExt->m_RefLinesDelta = Dta1xxBinDiv(pFdoExt->m_RefCntDelta,
												(Int64)pFdoExt->m_SamplesPerLine,
												NULL);
#endif

#if LOG_LEVEL_GENLOCK > 1
		DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTiming: RefLines=%d",
			       (Int)pFdoExt->m_RefLinesDelta);				
#endif

		pFdoExt->m_ValidRefCnt = TRUE;
	}

	pFdoExt->m_RefCntprev = RefCnt;

	return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxUpdateGenlockRx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxUpdateGenlockRx(IN PDTA1XX_FDO_EXTENSION  pFdoExt)
{
	Channel*  pCh;	// Channel pointer
	BOOLEAN ValidRxSignals = TRUE;
	UInt32 RxFrame, RxLine, RxSamp;


	Int64 RxPos, RxCount;
	UInt RxLineCount;	

	pCh = &pFdoExt->m_Channel[0];

	pFdoExt->m_ValidRxSignals = FALSE;

	// ********************************
	// Entry checks
	// ********************************
	if (pFdoExt->m_ValidRefCnt != TRUE)
		return STATUS_SUCCESS;

	// obtain periodic interrupt latched RxCounts
	RxFrame = Dta1xxRxGetSdiPos(pFdoExt->m_Channel[0].m_pRegBase, &RxLine, &RxSamp);

#if LOG_LEVEL_GENLOCK > 1
	DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockRx: Rx=%d.%d.%d", RxFrame, RxLine, RxSamp);				
#endif

	// ********************************
	// Sanity checks
	// ********************************

	// prevent wrap around problems
	if (RxFrame < pCh->m_RxFramePrev) {		
#if LOG_LEVEL_GENLOCK > 1
		DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockRx: Notice: RxFrame WRAP");
#endif

		pCh->m_RxFramePrev -= 2048;			// Max 2^11 (~ 40 seconden)
		pCh->m_RxPosPrev -= (2048 * pFdoExt->m_LinesPerFrame * pFdoExt->m_SamplesPerLine);

		// Now we can continue handling as normal
	}

	// Absolute sample count measured in 27Mhz variable
	RxPos = (((RxFrame * pFdoExt->m_LinesPerFrame) + RxLine) * pFdoExt->m_SamplesPerLine)
				+ RxSamp;

	RxCount = RxPos - pCh->m_RxPosPrev;
	pCh->m_RxPosPrev = RxPos;

#ifdef WINBUILD
	RxLineCount = (UInt) (RxCount / pFdoExt->m_SamplesPerLine);					
#else
	// 64-bit division are not allowed in the kernel => use Dta1xxBinDiv function
	RxLineCount = (UInt)Dta1xxBinDiv(RxCount, (Int64)pFdoExt->m_SamplesPerLine, NULL);
#endif

#if LOG_LEVEL_GENLOCK > 1
	DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockRx: RxCount = %d / %d lines", 
			   (Int) RxCount, (Int) RxLineCount);	
#endif

	if (RxLineCount == 0) 
	{
#if LOG_LEVEL_GENLOCK > 1
		DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockRx: Warning: No RX input");
#endif
		ValidRxSignals = FALSE;		
	}
	else if ( (RxLineCount < (pFdoExt->m_RefLinesDelta - 2)) ||
		      (RxLineCount > (pFdoExt->m_RefLinesDelta + 2)))
	{
			// The firmware contains in the RX channel (sdicompress) 
			// logic for determining the framesync
			// This introduces one or multiple clocks (>>4) delay in the framecounter 
			// Resulting in incorrect Rx frame compared to the line and sample counters

			// Workaround below detects and ignores the current samples

			if (RxLine == 0) 
			{
#if LOG_LEVEL_GENLOCK > 1
				DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockRx: Rxline == 0, Warning: "
					       "Skipping one control iteration");
#endif
				ValidRxSignals = FALSE;
			}
			else if (pCh->m_RxLinePrev == 0)
			{
#if LOG_LEVEL_GENLOCK > 1
				DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockRx: RxlinePrev == 0, Warning: "
					       "Skipping one control iteration");
#endif
				ValidRxSignals = FALSE;						
			}
			else 
			{
#if LOG_LEVEL_GENLOCK > 1
				DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockRx: Warning: Bad Rx Count");
#endif
				ValidRxSignals = FALSE;
			}
		} 

#if LOG_LEVEL_GENLOCK > 0
		if ((pFdoExt->m_Genlock == 1) && (pFdoExt->m_GenlockPrev == 0)) {
			DTA1XX_LOG(KERN_INFO, "Genlock Acquired");
		}
		else if ((pFdoExt->m_Genlock == 0) && (pFdoExt->m_GenlockPrev == 1)) {
			DTA1XX_LOG(KERN_INFO, "Genlock Lost");
		}
#endif

		pFdoExt->m_GenlockPrev = pFdoExt->m_Genlock;

		// "Publish" the Rx position device wide
		pFdoExt->m_RxLine = RxLine;
		pFdoExt->m_RxSamp = RxSamp;
		pFdoExt->m_ValidRxSignals = ValidRxSignals;

		// Store Rx frame + line/sample position locally within channel
		pCh->m_RxFramePrev = RxFrame;
		pCh->m_RxLinePrev = RxLine;
		pCh->m_RxSampPrev = RxSamp;

		return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxUpdateGenlockTx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxUpdateGenlockTx(IN PDTA1XX_FDO_EXTENSION  pFdoExt)
{
	volatile UInt8*  pGenRegs;

	UInt32 TxFrame, TxLine, TxSamp;

	Int64 TxPos, TxCount;
	Int64 RxPos;

	UInt TxLineCount;
	Int TxMinRx;
	Int dTxMinRxdt;

	UInt TxMinRef;
	Int dTxMinRefdt;

	Int SpdDelta;

	BOOLEAN ValidTxSignals = TRUE;

	// ********************************
	// Entry checks
	// ********************************

	if (pFdoExt->m_ValidRefCnt != TRUE)
		return STATUS_SUCCESS;


	SpdDelta = 0;
#ifdef WINBUILD
	pGenRegs = (volatile UInt8*)pFdoExt->m_pDta1xxGen;
#else
	pGenRegs = pFdoExt->m_pGenRegs;
#endif
	TxFrame = Dta1xxGenGetTxSdiPos(pGenRegs, &TxLine, &TxSamp);

#if LOG_LEVEL_GENLOCK > 1
	DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx: Tx=%d.%d.%d", TxFrame, TxLine, TxSamp);
#endif

	// ********************************
	// Position calculation
	// ********************************

	if (TxFrame < pFdoExt->m_TxFramePrev)
	{		
#if LOG_LEVEL_GENLOCK > 1
		DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx: Notice: TxFrame WRAP");		
#endif
		pFdoExt->m_TxFramePrev -= 2048;			// Max 2^11
		pFdoExt->m_TxPosPrev -= (2048 * pFdoExt->m_LinesPerFrame * pFdoExt->m_SamplesPerLine);

		// Now we can continue handling as normal
	} 

	// Compute absolute Rx/Txpos
	TxPos = (((TxFrame  * pFdoExt->m_LinesPerFrame) + TxLine) * pFdoExt->m_SamplesPerLine) + TxSamp;			// Absolute sample count measured in 27Mhz
	TxCount = TxPos - pFdoExt->m_TxPosPrev;
	pFdoExt->m_TxPosPrev = TxPos;

#ifdef WINBUILD
	TxLineCount = (UInt) (TxCount / pFdoExt->m_SamplesPerLine);
#else
	// 64-bit division are not allowed in the kernel => use Dta1xxBinDiv function
	TxLineCount = (UInt)Dta1xxBinDiv(TxCount, (Int64)pFdoExt->m_SamplesPerLine, NULL);
#endif

#if LOG_LEVEL_GENLOCK > 1
	DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx: TxCount = %d / %d lines",
		       (Int)TxCount, (Int)TxLineCount);	
#endif


	// ********************************
	// Sanity checks
	// ********************************


	if ( (TxLineCount < (pFdoExt->m_RefLinesDelta - 2)) ||
		 (TxLineCount > (pFdoExt->m_RefLinesDelta + 2)) )
	{
#if LOG_LEVEL_GENLOCK > 1
		DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx: TX Genlock Warning: Bad Tx Count");
#endif
		ValidTxSignals = FALSE;
	} 
	
	// ************************
	// Genlock Tx to 27Mhz
	// ************************
	if (ValidTxSignals == FALSE)
	{			

		// ********************************
		// ERROR: No valid Tx counters
		// ********************************
		if(pFdoExt->m_GraceCnt != 10) {
#if LOG_LEVEL_GENLOCK > 1
			DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx: Note: Grace %d",
					   pFdoExt->m_GraceCnt);
#endif
			pFdoExt->m_GraceCnt++;
			pFdoExt->m_TxMinRxPrev = 0;		// prevent errors
			pFdoExt->m_TxMinRefPrev = 0;	// prevent errors
		} else { 
			// We can't do anything if we don't have valid TX signals
			// Center Vcxo and hope for the best
			if (pFdoExt->m_VcxoCtrl != 512000) {
				pFdoExt->m_VcxoCtrl = 512000;
				Dta1xxSetVcxoSpeed(pFdoExt);

#if LOG_LEVEL_GENLOCK > 1
				DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx: Force Reset VCXO to %d",
						   pFdoExt->m_VcxoCtrl);				
#endif
			}

			// Note: revise code below for crossboard genlock
			pFdoExt->m_Genlock = 0;		// indicate out of lock							
		}	

	}
	else if (pFdoExt->m_ValidRxSignals == FALSE) {

		// ********************************
		// WARNING: No valid Rx, GENLOCK to 27Mhz
		// ********************************
		if (pFdoExt->m_GraceCnt != 10) {
			// Delay locking to 27Mhz until rx lock permanently lost

#if LOG_LEVEL_GENLOCK > 1
			DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx: Note: Grace %d",
					   pFdoExt->m_GraceCnt);
#endif
			pFdoExt->m_GraceCnt++;

			pFdoExt->m_TxMinRxPrev = 0;			// prevent errors
			pFdoExt->m_TxMinRefPrev = 0;		// prevent errors
		}
		else 
		{
			// RefCntDelta in 27Mhz units, equals number of samples possible in 
			// previous period
			TxMinRef = (Int) (TxCount - pFdoExt->m_RefCntDelta);
			
#if LOG_LEVEL_GENLOCK > 1
			DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx: #27Mhz Reference Cycles: %d, "
					   "TxCount: %d", (Int)pFdoExt->m_RefCntDelta, (Int)TxCount);
#endif
			// Compute derivative of difference between Tx SDI pos and Rx SDI pos
			dTxMinRefdt = pFdoExt->m_TxMinRefPrev - (Int) TxMinRef;
			pFdoExt->m_TxMinRefPrev = (Int) TxMinRef;						

#if LOG_LEVEL_GENLOCK > 1
			DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx: 27Mhz Xtal/VCO cycle difference "
					   "%d (%d)", TxMinRef, dTxMinRefdt);
#endif
			// Compute new control value
			SpdDelta = - (Int) TxMinRef * 10 + dTxMinRefdt * 200;
			pFdoExt->m_Genlock = 0;
		}
	} 
	else 
	{
		// ********************************
		// Genlock Tx to Rx
		// ********************************

		// Computer TxMinRx, maximum difference 1 frame
		// TxMinRx = (Int) ((TxPos - RxPos) % (LinesPerFrame * SamplesPerLine));
		// Compute Rx/Txpos for locking (we wan't to lock to same linenumber.samplenumber)
		// Absolute sample count measured in 27Mhz
		TxPos = ( TxLine * pFdoExt->m_SamplesPerLine) + TxSamp;

		// Use "published" RX line/sample for to lock to
		// Absolute sample count measured in 27Mhz variable
		RxPos = ( pFdoExt->m_RxLine * pFdoExt->m_SamplesPerLine) + pFdoExt->m_RxSamp;

		
		// Check for Firmware version 3 of the (2)145 or version 0 of the 2144
		// Correct for the linecounter offset in this firmware version
		if (
			((pFdoExt->m_FirmwareVersion == 0) && (pFdoExt->m_TypeNumber == 2144)) ||
			((pFdoExt->m_FirmwareVersion == 3) && (pFdoExt->m_TypeNumber == 2145)) ||
			((pFdoExt->m_FirmwareVersion == 3) && (pFdoExt->m_TypeNumber == 145))
			){
				RxPos -= pFdoExt->m_SamplesPerLine;		// correct for 1 line offset
		}		

		// 20 samples tuning to align them properly
		// Source: GenlockMeasure using DTA-160
		RxPos += 20;	// correct for 20 samples

		// Wrap around
#ifdef WINBUILD
		RxPos = RxPos % (pFdoExt->m_LinesPerFrame * pFdoExt->m_SamplesPerLine);
#else
		// 64-bit division are not allowed in the kernel => use Dta1xxBinDiv function
		Dta1xxBinDiv(RxPos, 
					 (Int64)(pFdoExt->m_LinesPerFrame * pFdoExt->m_SamplesPerLine),
					 &RxPos);
#endif

		TxMinRx = (Int) (TxPos - RxPos);
		
		if (TxMinRx < (pFdoExt->m_TxMinRxPrev - (pFdoExt->m_LinesPerFrame * pFdoExt->m_SamplesPerLine)/2))
			TxMinRx += (pFdoExt->m_LinesPerFrame * pFdoExt->m_SamplesPerLine);
		else if (TxMinRx > (pFdoExt->m_TxMinRxPrev + (pFdoExt->m_LinesPerFrame * pFdoExt->m_SamplesPerLine)/2))
			TxMinRx -= (pFdoExt->m_LinesPerFrame * pFdoExt->m_SamplesPerLine);

#if LOG_LEVEL_GENLOCK > 1
		DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx:  >> Dta1xx: Diff: %d.%d lines <<",
				   TxMinRx / pFdoExt->m_SamplesPerLine,
				   TxMinRx % pFdoExt->m_SamplesPerLine);
#endif
		if (abs(TxMinRx/pFdoExt->m_SamplesPerLine) > 5)
		{	
			// ********************************
			// Out of GENLOCK
			// ********************************
			if (pFdoExt->m_GraceCnt != 10)
			{
#if LOG_LEVEL_GENLOCK > 1
				DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx: Grace %d", pFdoExt->m_GraceCnt);
#endif
				pFdoExt->m_GraceCnt++;
			}
			else
			{
				UInt NewTxLine;
				UInt NewTxSamp;

				// Use "published" RX line/sample for preloading
				NewTxLine = ((pFdoExt->m_LinesPerFrame - TxLine) + pFdoExt->m_RxLine) % pFdoExt->m_LinesPerFrame;
				NewTxSamp = ((pFdoExt->m_SamplesPerLine - TxSamp) + pFdoExt->m_RxSamp) % pFdoExt->m_SamplesPerLine;
#if LOG_LEVEL_GENLOCK > 0
				DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx:  >> Resyncing TX Counter <<");
#endif

#if LOG_LEVEL_GENLOCK > 1
				DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx:\nTX Line counter step: "
						   "%d -> %d TX Sample counter step: %d -> %d",  TxLine,
						   NewTxLine, TxSamp, NewTxSamp);
#endif
				Dta1xxGenSetTxSdiPos(pGenRegs, NewTxLine, NewTxSamp);

				pFdoExt->m_VcxoCtrl = 512000;
				Dta1xxSetVcxoSpeed(pFdoExt);

				pFdoExt->m_TxMinRxPrev = 0;							
				pFdoExt->m_TxMinRefPrev = 0;		// prevent errors

				pFdoExt->m_Genlock = 0;			// indicate out of lock
				pFdoExt->m_GraceCnt = 0;		// allow channel to sync
			}
		}
		else 
		{
			// ********************************
			// In GENLOCK
			// ********************************
#if LOG_LEVEL_GENLOCK > 0
			DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx: Genlock delta: %d.%d lines",
					   TxMinRx / pFdoExt->m_SamplesPerLine, 
					   TxMinRx % pFdoExt->m_SamplesPerLine);
#endif
			if (TxMinRx > pFdoExt->m_SamplesPerLine)
				TxMinRx = pFdoExt->m_SamplesPerLine;
			else if (TxMinRx < - pFdoExt->m_SamplesPerLine)
				TxMinRx = - pFdoExt->m_SamplesPerLine;
			
			// Compute derivative of difference between Tx SDI pos and Rx SDI pos
			dTxMinRxdt = pFdoExt->m_TxMinRxPrev - TxMinRx;
			pFdoExt->m_TxMinRxPrev = TxMinRx;						

			// Compute new control value
			SpdDelta = - TxMinRx * 10 + dTxMinRxdt * 200;

			pFdoExt->m_Genlock = 1;				// indicate locked
			pFdoExt->m_GraceCnt = 0;			// reset grace counter
		}
	}


	// ************************
	// Adjust VCXO
	// ************************
	if (SpdDelta != 0)
	{				
#if LOG_LEVEL_GENLOCK > 1
		DTA1XX_LOG(KERN_INFO, "Dta1xxUpdateGenlockTx: SpdDelta=%d", SpdDelta);				
#endif
		// Update VCXO control
		pFdoExt->m_VcxoCtrl += SpdDelta;
		if (pFdoExt->m_VcxoCtrl < 0)
			pFdoExt->m_VcxoCtrl = 0;
		if (pFdoExt->m_VcxoCtrl > 1000000)
			pFdoExt->m_VcxoCtrl = 1000000;			
		Dta1xxSetVcxoSpeed(pFdoExt);	
	}

	pFdoExt->m_TxFramePrev	= TxFrame;
	pFdoExt->m_TxLinePrev	= TxLine;
	pFdoExt->m_TxSampPrev	= TxSamp;

	return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxGetGenlockState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get the Genlock reference input state of a device
//
NTSTATUS  Dta1xxGetGenRefState(
	IN PDTA1XX_FDO_EXTENSION  pFdoExt,		// Device Extension
	IN DTA1XX_GENLOCK*  pGenlock
	)
{
	Channel*  pCh;

	// Get channel pointer
	pCh = &pFdoExt->m_Channel[pGenlock->m_PortIndex];
	pGenlock->m_Mode = 0;				// -1 = 27Mhz Genlock, 0 = Unknown, 1 = SDI Frame
	pGenlock->m_Enable = 0;				// 
	pGenlock->m_OffsetSamples = 0;		// Not implemented

	// Only supported on genlock input ports
	if ((pCh->m_Capability & DTA1XX_CHCAP_GENREF) == 0) {
#if LOG_LEVEL_GENLOCK > 0
		DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxGetGenRefState: GENREF NOT SUPPORTED",
				   pGenlock->m_PortIndex);
#endif

		return STATUS_NOT_SUPPORTED;
	}

	// check channel 0 is in genlock mode
	if (pCh->m_IoConfig != DTA1XX_IOCONFIG_GENREF) {
#if LOG_LEVEL_GENLOCK > 0
		DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxGetGenRefState: GENREF NOT ENABLED",
				   pGenlock->m_PortIndex);
#endif

		return STATUS_SUCCESS;
	}
	
	// Check current lock state
	if (pFdoExt->m_Genlock == 1) {
#if LOG_LEVEL_GENLOCK > 0
		DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxGetGenRefState: IN LOCK",
				   pGenlock->m_PortIndex);
#endif

		pGenlock->m_Mode = 1;				// -1 = 27Mhz Genlock, 0 = Unknown, 1 = SDI Frame
		pGenlock->m_Enable = 1;				// 
		pGenlock->m_OffsetSamples = 0;		// Not implemented

		return STATUS_SUCCESS;
	} else {
#if LOG_LEVEL_GENLOCK > 0
		DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxGetGenRefState: OUT OF LOCK",
				   pGenlock->m_PortIndex);
#endif

		pGenlock->m_Mode = -1;				// -1 = 27Mhz Genlock, 0 = Unknown, 1 = SDI Frame
		pGenlock->m_Enable = 1;				// 
		pGenlock->m_OffsetSamples = 0;		// Not implemented

		return STATUS_SUCCESS;
	}
}
