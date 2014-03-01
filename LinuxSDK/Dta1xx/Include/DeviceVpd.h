//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DeviceVpd.h *#*#*#*#*#*#*#*#* (C) 2000-2007 DekTec
//
// Header file which defines the common VPD data in the serial EEPROMs of DTA-1XX and
// DTU-2XX devices.
// Furthermore, a number of other VPD-related constant are defined.
//
// NOTE: DTU-2XX devices have a bigger EEPROM, but are for the moment used
//		 just to address 0x1FF

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	RZ	2003.11.19	Created	from Dta1xxVpd.h

//#pragma once
#ifndef __DTAPIVPD_H
#define __DTAPIVPD_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Start of VPD sections
#define EEPROM_VPD_ID_START		0x58		// Read-only section directly follows VPD-ID
#define EEPROM_VPD_ID_START_NEW	0x63		// Read-only section directly follows VPD-ID
#define EEPROM_VPD_RW_START		0x100
#define EEPROM_VPD_END			0x1FF

// Section length
#define EEPROM_VPD_ID_RO_LENGTH		(EEPROM_VPD_RW_START - EEPROM_VPD_ID_START)
#define EEPROM_VPD_RW_LENGTH		(EEPROM_VPD_END - EEPROM_VPD_RW_START)

// Total length. The final +1 is for VPD End tag
#define EEPROM_VPD_TOTAL_LENGTH		(EEPROM_VPD_ID_RO_LENGTH + EEPROM_VPD_RW_LENGTH + 1)

// Maximum length of VPD-resource string
#define VPD_ITEM_MAX_LENGTH		63			// 64 if trailing '\0' is factored in
#define VPD_ITEM_MAX_LENGTH_DTE	255			// 256 if trailing '\0' is factored in


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Macro's -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#define VALID_VPD_KEYW_CHAR(c) 	(((c)>='0' && (c)<='9') || ((c)>='A' && (c)<='Z'))

#endif	// #ifndef __DTAPIVPD_H
