//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Ad9789.h *#*#*#*#*#*#*#*# (C) 2000-2003 DEKTEC
//
// Interface for AD9789 support.

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	AT	2010.01.29	Created 

#ifndef __AD9789_H
#define __AD9789_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Interface functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Ad9789Init(
	Channel *  pCh);		// Channel to initialise
Int  Ad9789GetRfControl(
	Channel *  pCh,			// Our channel object
	Int64 *  pRfRate,		// RF rate
	Int *  pLockStatus);	// PLL lock status
Int  Ad9789SetModControl(
	Channel *  pCh,			// Our channel object
	Int  ModType,			// Modulation type
	Int  ParXtra0,			// Extra parameter #0
	Int  ParXtra1,			// Extra parameter #1
	Int  ParXtra2);			// Extra parameter #2
Int  Ad9789SetRfControl(
	Channel *  pCh,			// Our channel object
	Int64  RfFreq);			// RF rate
Int  Ad9789SetRfMode(
	Channel *  pCh,			// Our channel object
	Int  RfMode);			// RF mode
Int  Ad9789SetSymSampleRate(
	Channel *  pCh,			// Our channel object
	Int  SymSamplRate);

#endif // #ifndef __AD9789_H
