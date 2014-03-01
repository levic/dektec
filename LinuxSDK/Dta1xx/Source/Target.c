//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Target.c *#*#*#*#*#*#*#*#*# (C) 2000-2007 DekTec

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dta1xx driver, please refer to the Dta1xx readme

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"
#include <linux/delay.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define	 GROUNDED		0		// DVB-SPI cable connected => sense input is grounded
#define  OPEN_CIRCUIT	36		// Nothing connected to 25p subD connector

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local Routines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  Dta1xxReadTargetId(DTA1XX_FDO* pFdo, Int* pnTargetId);
void  Dta1xxSenseAtoDUsingDacA(DTA1XX_FDO* pFdo, Int* pAdcValue);
void  Dta1xxSenseAtoDUsingDacB(DTA1XX_FDO* pFdo, Int* pAdcValue);
void  Dta1xxSetTargetIdWindow(DTA1XX_FDO* pFdo);
void  Dta1xxSetWindowForGrounded(DTA1XX_FDO* pFdo);
void  Dta1xxSetWindowForOpenCircuit(DTA1XX_FDO* pFdo);
void  Dta1xxWaitForUpdateDone(UInt8* pTxRegs);


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxGetTargetId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read results of the target-ID detection process.
//
Int  Dta1xxGetTargetId(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  ChannelType,		// DTA1XX_TS_TX_CHANNEL / DTA1XX_TS_RX_CHANNEL
	Int  PortIndex,			// Port index
	Int*  pPresent,			// Target adapter is present (best guess)
	Int*  pTargetId)		// Target adapter ID (best guess)
{
	// Do no crash on erroneous port index
	if (PortIndex<0 || PortIndex>=pFdo->m_NumNonIpChannels) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxGetTargetId PortIndex=%d INVALID", PortIndex );
#endif
		return -EFAULT;
	}

	if (ChannelType == DTA1XX_TS_TX_CHANNEL) {
		*pPresent =	pFdo->m_Channel[PortIndex].m_TrgIdDet.m_Present;
		*pTargetId = pFdo->m_Channel[PortIndex].m_TrgIdDet.m_TargetId;
	}
	else if (ChannelType == DTA1XX_TS_RX_CHANNEL) {
		*pPresent =	pFdo->m_Channel[PortIndex].m_TrgIdDet.m_Present;
		*pTargetId = pFdo->m_Channel[PortIndex].m_TrgIdDet.m_TargetId;
	} else {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxClearFlags: ChannelType=%d ILLEGAL",
					PortIndex, ChannelType );
#endif
		return -EFAULT;
	}

#if LOG_LEVEL>1
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxGetTargetId Present=%d Target=%d",
			    PortIndex, *pPresent, *pTargetId );
#endif
	return 0;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ TARGET DETECTION +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTargetDetectStateMachine -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Main state machine for target-adapter detection.
// Called from periodic interrupt (=every 77ms).
//
// See DTA-102 Design Document for graphical state diagram.
//
void  Dta1xxTargetDetectStateMachine(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
/*
	Int  NewTdState, TargetIdRead;
	char*  StrState;
    Dta1xxTx*  pDta1xxTx;
	TargetIdDetection*  pTrg;				// Target-ID detection variables

	// Select the right set of target-ID detection variables
	// We cheat a little bit: register lay-out for DTA-102 and DTA-122 is the same.
	if (pFdo->m_TypeNumber == 102) {
		pTrg = &pFdo->m_Channel[0].m_TrgIdDet;
		pDta1xxTx = (Dta1xxTx*) (pFdo->m_Channel[0].m_pRegBase);
	} else {
		pTrg = &pFdo->m_Channel[0].m_TrgIdDet;
		pDta1xxTx = (Dta1xxTx*) (pFdo->m_Channel[0].m_pRegBase);
	}

	// Default for new state: same as old state
	NewTdState = pTrg->m_TdState;

	switch (pTrg->m_TdState) {

	case DTA1XX_TD_INITIAL:
to_find:
		NewTdState = DTA1XX_TD_FIND_TARGETID;
		pTrg->m_Present = DTA1XX_TARGET_UNKNOWN;
		pTrg->m_NumTimesInFind = 0;
		pTrg->m_StableCount = 0;
		Dta1xxReadTargetId(pFdo, &pTrg->m_LastTargetId);
		break;

	case DTA1XX_TD_FIND_TARGETID:
		// If we are at >8 times in this state, TARGET_ADAPTER becomes unknown
		if (pTrg->m_NumTimesInFind++ > 8)
			pTrg->m_Present = DTA1XX_TARGET_UNKNOWN;

		// Read target ID and check whether it is stable
		Dta1xxReadTargetId(pFdo, &TargetIdRead);
		if (TargetIdRead != pTrg->m_LastTargetId) {
			pTrg->m_LastTargetId = TargetIdRead;			// Not stable
			pTrg->m_StableCount = 0;
		}

		if (pTrg->m_StableCount++ > 3) {					// Stable
			if (TargetIdRead == GROUNDED) {
				NewTdState = DTA1XX_TD_SPI_SINK;
				pTrg->m_Present = DTA1XX_DVB_SPI_SINK;
				Dta1xxSetWindowForGrounded(pFdo);
			} else if (TargetIdRead == OPEN_CIRCUIT) {
				NewTdState = DTA1XX_TD_OPEN_CIRCUIT;
				pTrg->m_Present = DTA1XX_NO_CONNECTION;
				Dta1xxSetWindowForOpenCircuit(pFdo);
			} else {
				NewTdState = DTA1XX_TD_TARGETID_STABLE;
				pTrg->m_StableCount = 0;
				pTrg->m_Present = DTA1XX_TARGET_PRESENT;
				pTrg->m_TargetId = TargetIdRead;
				Dta1xxSetTargetIdWindow(pFdo);
			}
		}
		break;

	case DTA1XX_TD_OPEN_CIRCUIT:
		if (pDta1xxTx->m_ThreshControl.m_CmpA == 0)		// Window crossed?
			goto to_find;
		break;

	case DTA1XX_TD_SPI_SINK:
		if (pDta1xxTx->m_ThreshControl.m_CmpA != 0)		// Window crossed?
			goto to_find;
		break;

	case DTA1XX_TD_TARGETID_STABLE:
		if (pDta1xxTx->m_ThreshControl.m_CmpB != 0)			// Window crossed (high)?
			goto to_find;
		else if (pDta1xxTx->m_ThreshControl.m_CmpA == 0) {	// Window crossed (low)?
			if (pTrg->m_StableCount++ > 1) {				// Stable?
				Dta1xxReadTargetId(pFdo, &TargetIdRead);
				if (TargetIdRead == 0) {
					NewTdState = DTA1XX_TD_TARGET_ERROR;
					Dta1xxSetWindowForGrounded(pFdo);
					// Note: target-error flag is cleared in Dta1xxTxProcessStatusFlags
					pTrg->m_TargetError = TRUE;
				} else
					goto to_find;
			}
		} else
			pTrg->m_StableCount = 0;
		break;

	case DTA1XX_TD_TARGET_ERROR:
		if (pDta1xxTx->m_ThreshControl.m_CmpA != 0) {		// Window crossed?
			Dta1xxReadTargetId(pFdo, &TargetIdRead);
			if (TargetIdRead == pTrg->m_TargetId) {
				NewTdState = DTA1XX_TD_TARGETID_STABLE;
				pTrg->m_StableCount = 0;
				Dta1xxSetTargetIdWindow(pFdo);
			} else
				goto to_find;
		}
		break;
	}

	if (NewTdState != pTrg->m_TdState) {
		StrState = NULL;
		switch (NewTdState) {
		case DTA1XX_TD_FIND_TARGETID:	StrState = "DTA1XX_TD_FIND_TARGETID";	break;
		case DTA1XX_TD_SPI_SINK:		StrState = "DTA1XX_TD_SPI_SINK";		break;
		case DTA1XX_TD_OPEN_CIRCUIT:	StrState = "DTA1XX_TD_OPEN_CIRCUIT";	break;
		case DTA1XX_TD_TARGETID_STABLE:	StrState = "DTA1XX_TD_TARGETID_STABLE";	break;
		case DTA1XX_TD_TARGET_ERROR:	StrState = "DTA1XX_TD_TARGET_ERROR";	break;
		}
		if (StrState == NULL)
			DTA1XX_LOG( KERN_INFO, "Target detection, state := UNKNOWN (%d)", NewTdState));
		else
			DTA1XX_LOG( KERN_INFO, "Target detection, state := %s", StrState));
	}
	pTrg->m_TdState = NewTdState;
*/
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxReadTargetId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  Dta1xxReadTargetId(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int*  pTargetId)		// Target ID 1..35; 0=GROUNDED; 36=OPEN_CIRCUIT
{
/*
	Int  AtoDVal, TargetId;

	Dta1xxSenseAtoDUsingDacA(pFdo, &AtoDVal);
	TargetId = AtoDVal / 6;
	if (TargetId > OPEN_CIRCUIT)
		TargetId = OPEN_CIRCUIT;
	*pTargetId = TargetId;
*/
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSenseAtoDUsingDacA -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Emulate A-to-D conversion on sense input, by performing a binary search with the DAC
// and monitoring the comparator output. Uses DAC A.
//
void  Dta1xxSenseAtoDUsingDacA(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int*  pAdcValue)		// Measured value for voltage on sense input
							// 0=0V ... 255=5V
{
/*
	Int  Mask;
	Int  Val = 0;
    Dta1xxTx*  pDta1xxTx;

	// We cheat a little bit: register lay-out for DTA-102 and DTA-122 is the same.
	pDta1xxTx = (Dta1xxTx*) (pFdo->m_Channel[0].m_pRegBase);

	pDta1xxTx->m_ThreshControl.m_AorB = 0;	// Use DAC A
	for (Mask=0x80; Mask!=0; Mask>>=1) {
		pDta1xxTx->m_ThreshControl.m_ThreshA = Val | Mask;
		Dta1xxWaitForUpdateDone(pDta1xxTx);
		udelay(8);		// Wait 8us (max DAC settling time)
		if (pDta1xxTx->m_ThreshControl.m_CmpA != 0)
			Val |= Mask;
	}
	*pAdcValue = Val;
*/
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSenseAtoDUsingDacB -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Idem, with DAC B.
//
void  Dta1xxSenseAtoDUsingDacB(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int*  pAdcValue)		// Measured value for voltage on sense input
							// 0=0V ... 255=5V
{
/*
	Int  Mask;
	Int  Val = 0;
    Dta1xxTx*  pDta1xxTx;

	// We cheat a little bit: register lay-out for DTA-102 and DTA-122 is the same.
	pDta1xxTx = (Dta1xxTx*) (pFdo->m_Channel[0].m_pRegBase);

	pDta1xxTx->m_ThreshControl.m_AorB = 1;	// Use DAC B
	for (Mask=0x80; Mask!=0; Mask>>=1) {
		pDta1xxTx->m_ThreshControl.m_ThreshB = Val | Mask;
		Dta1xxWaitForUpdateDone(pDta1xxTx);
		udelay(8);		// Wait 8us (max DAC settling time)
		if (pDta1xxTx->m_ThreshControl.m_CmpB != 0)
			Val |= Mask;
	}
	*pAdcValue = Val;
*/
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetTargetIdWindow -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set a window of +/- 3 LSBs around currently measured voltage on sense pin.
// Comparator A implements lower threshold. 1=ok, 0=below threshold.
// Comparator B implements upper threshold. 0=ok, 1=above threshold.
//
void  Dta1xxSetTargetIdWindow(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
/*
	int  ValDacA, ValDacB, AbsDiff;
    Dta1xxTx*  pDta1xxTx;

	// Select the right set of target-ID detection variables
	// We cheat a little bit: register lay-out for DTA-102 and DTA-122 is the same.
	pDta1xxTx = (Dta1xxTx*) (pFdo->m_Channel[0].m_pRegBase);

	Dta1xxSenseAtoDUsingDacA(pFdo, &ValDacA);
	Dta1xxSenseAtoDUsingDacB(pFdo, &ValDacB);

	// For robustness:
	AbsDiff = (ValDacA>ValDacB) ? (ValDacA-ValDacB) : (ValDacB-ValDacA);
	if (AbsDiff > 4) ValDacA = ValDacB;
	if (ValDacA < 5) ValDacA = 5;
	if (ValDacB < 5) ValDacB = 5;
	if (ValDacA > 240) ValDacA = 240;
	if (ValDacB > 240) ValDacB = 240;

	// Set window
	pDta1xxTx->m_ThreshControl.m_AorB = 0;
	pDta1xxTx->m_ThreshControl.m_ThreshA = ValDacA - 3;
	Dta1xxWaitForUpdateDone(pDta1xxTx);

	pDta1xxTx->m_ThreshControl.m_AorB = 1;
	pDta1xxTx->m_ThreshControl.m_ThreshB = ValDacB + 3;
	Dta1xxWaitForUpdateDone(pDta1xxTx);

	DTA1XX_LOG( KERN_INFO, "Dta1xxSetTargetIdWindow A-threshold=%d, B-threshold=%d",
				 ValDacA-3, ValDacB+3 );
 */
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetWindowForGrounded -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set comparator A, such that 0=GROUNDED, 1=not grounded anymore
//
void  Dta1xxSetWindowForGrounded(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
/*
	Dta1xxTx*  pDta1xxTx;

	// Select the right set of target-ID detection variables
	// We cheat a little bit: register lay-out for DTA-102 and DTA-122 is the same.
	pDta1xxTx = (Dta1xxTx*) (pFdo->m_Channel[0].m_pRegBase);

	pDta1xxTx->m_ThreshControl.m_AorB = 0;
	pDta1xxTx->m_ThreshControl.m_ThreshA = 6;
	Dta1xxWaitForUpdateDone(pDta1xxTx);
	DTA1XX_LOG( KERN_INFO, "Dta1xxSetWindowForGrounded A-threshold=6" );
*/
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetWindowForOpenCircuit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set comparator A, such that 1=OPEN_CIRCUIT, 0=something connected
//
void  Dta1xxSetWindowForOpenCircuit(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
/*
    Dta1xxTx*  pDta1xxTx;

	// Select the right set of target-ID detection variables
	// We cheat a little bit: register lay-out for DTA-102 and DTA-122 is the same.
	pDta1xxTx = (Dta1xxTx*) (pFdo->m_Channel[0].m_pRegBase);

	pDta1xxTx->m_ThreshControl.m_AorB = 0;
	pDta1xxTx->m_ThreshControl.m_ThreshA = OPEN_CIRCUIT*6;
	Dta1xxWaitForUpdateDone(pDta1xxTx);
	DTA1XX_LOG( KERN_INFO, "Dta1xxSetWindowForOpenCircuit A-threshold=%d",
				 OPEN_CIRCUIT*6 );
*/
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxWaitForUpdateDone -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Wait until DAC has been updated. Don't wait too long, to avoid system crash if
// something is wrong with the DAC...
//
void  Dta1xxWaitForUpdateDone(
	UInt8*  pTxRegs)
{
/*
	Int  TimeOut = 0;
	while (pDta1xxTx->m_ThreshControl.m_UpdBusy && TimeOut++<100)
		;
*/
}
