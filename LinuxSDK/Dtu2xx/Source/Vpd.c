//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Vpd.c *#*#*#*#*#*#*#*#*#*# (C) 2000-2008 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dtu2xx driver, please refer to the Dta1xx readme

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "Dtu2xx.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local Forwards -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
UInt  Dtu2xxVpdGetSectionLength(UInt8*, UInt Length);
BOOLEAN  Dtu2xxVpdFindResource(IN PDTU2XX_FDO, char*, UInt8*, UInt, UInt8**);
BOOLEAN  Dtu2xxVpdIsValidResource(UInt8*, UInt8*, UInt);
Int  Dtu2xxVpdDeleteResource(IN PDTU2XX_FDO, char*, UInt8*, UInt);
Int  Dtu2xxVpdFindEndOfRoSection(IN PDTU2XX_FDO, OUT UInt8**);
Int  Dtu2xxVpdFindStartOfRoSection(IN PDTU2XX_FDO, OUT UInt8**);
Int  Dtu2xxVpdReadItemRo(PDTU2XX_FDO, char*, char*, UInt*);
Int  Dtu2xxVpdReadItemRw(PDTU2XX_FDO, char*, char*, UInt*);
Int  Dtu2xxVpdUpdateEepromFromCache(IN PDTU2XX_FDO, IN  UInt, IN  UInt);
Int  Dtu2xxVpdWriteArray(PDTU2XX_FDO, UInt16*, UInt, UInt, UInt16*);
Int  Dtu2xxVpdWriteWordWithCheck(PDTU2XX_FDO, UInt, UInt16, UInt16*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdInitOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Locate the start of the VpdId item.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache
//
void  Dtu2xxVpdInitOffset(
	IN  PDTU2XX_FDO  pFdo)	// Device extension
{
	if (pFdo->m_pVpdEeprom[DTU2XX_VPD_ID_START] == 0x82) {
		pFdo->m_VpdIdOffset = DTU2XX_VPD_ID_START;
#if LOG_LEVEL > 0
		DTU2XX_LOG( KERN_INFO,
					"Dtu2xxVpdInitOffset: VpdOffset: 0x%02X",
					pFdo->m_VpdIdOffset);
#endif
	}
	else {
		pFdo->m_VpdIdOffset = 0;
		DTU2XX_LOG( KERN_INFO, "Dtu2xxVpdInitOffset: Empty EEPROM or illegal layout");
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdReadItemRoRw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read VPD item from Ro + Rw section into a caller-allocated string buffer.
// This routine is used to read PCI-card specific configuration data at
// driver-initialisation time.
//
// The pVpdItem will be null terminated
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dtu2xxVpdReadItemRoRw(
	IN  PDTU2XX_FDO  pFdo,	// Device extension
	IN  char*  pKeyword,				// Name of VPD item to be read
	OUT char*  pVpdItem)				// String buffer for storing VPD
										// Must be at least VPD_ITEM_MAX_LENGTH+1 long
{
	UInt  Length = DTU2XX_VPD_ITEM_MAX_LENGTH + 1;
	Int  Status;

	// Try to read item from read-only section, then from read-write section
	Status = Dtu2xxVpdReadItemRo(pFdo, pKeyword, pVpdItem, &Length);
	if (Status != 0)
		Status = Dtu2xxVpdReadItemRw(pFdo, pKeyword, pVpdItem, &Length);

	// '\0'-terminate item string
	if (Status == 0)
		pVpdItem[Length] = '\0';

	return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdReadId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read the VPD Id into a caller-allocated string buffer.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dtu2xxVpdReadId(
	IN  PDTU2XX_FDO  pFdo,	// Device extension
	OUT char*  pVpdId,					// String buffer for storing VPD Id
	OUT UInt*  pLength)					// In: Length of pVpdId buffer
										// Out: Length of string
{
	char*  pFrom;				// Helper character pointer
	char*  pTo;					// Helper character pointer
	UInt  i, Length;
	UInt8*  pVpd;				// Byte pointer in VPD data

	// Add parameter checks here
	if (*pLength == 0) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdReadId: Invalid length: 0");
	}

	// Initialisation
	pTo = pVpdId;				// For later copying to VPD item
	memset(pVpdId, 0, *pLength);

	// Start at beginning of VPD-ID + read-only section
	pVpd = &pFdo->m_pVpdEeprom[pFdo->m_VpdIdOffset];
	if (pVpd[0] != 0x82) {
		DTU2XX_LOG(KERN_INFO,
				   "Dtu2xxVpdReadId: ERROR: VPD-ID section starts with 0x%02X "
				   "i.s.o. 0x82", pVpd[0]);
		return -EFAULT;
	}

	Length = pVpd[1] + (pVpd[2]<<8);
	pFrom = pVpd + 3;

	// Protect against invalid content of EEPROM or too small buffer
	if (Length > *pLength)
		Length = *pLength;
	for (i=0; i<Length; i++)
		*pTo++ = *pFrom++;
	*pLength = Length;
#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdReadId: Found VPD Id, value='%s'", pVpdId);
#endif

	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdReadItemRo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read VPD item into a caller-allocated string buffer from the read-only section.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dtu2xxVpdReadItemRo(
	IN  PDTU2XX_FDO  pFdo,	// Device extension
	IN  char*  pKeyword,				// Name of VPD item to be read
	OUT char*  pVpdItem,				// String buffer for storing VPD item
	OUT UInt*  pLength)					// In: Length of pVpdItem buffer
										// Out: Length of string
{
	UInt  i, Length, RoLength;
	char*  pFrom;				// Helper character pointer
	char*  pTo;					// Helper character pointer
	UInt8*  pVpd;				// Byte pointer in VPD data
	UInt8*  pVpdRoEnd;			// Pointer one after RO section
	UInt8*  pResource;			// Resource to be read

	// Parameter check
	if (*pLength == 0) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdReadItemRo: Invalid length: 0");
	}

	// Compute a number of helper variables
	pVpdRoEnd = &pFdo->m_pVpdEeprom[DTU2XX_VPD_RW_START];

	// Initialisation
	pTo = pVpdItem;				// For later copying to VPD item
	memset(pVpdItem, 0, *pLength);

	if (Dtu2xxVpdFindStartOfRoSection(pFdo, &pVpd) != 0)
		return -EFAULT;

	// Length of RO section
	RoLength = (UInt)(pVpdRoEnd - pVpd);

	// Check whether we can find resource in RO section
	if (Dtu2xxVpdFindResource(pFdo, pKeyword, pVpd, RoLength, &pResource)) {
 		pFrom = pResource + 3;
		Length = pResource[2];
		// Protect against invalid content of EEPROM or too small buffer
		if (Length > *pLength)
			Length = *pLength;
		for (i=0; i<Length; i++)
			*pTo++ = *pFrom++;
		*pLength = Length;
#if LOG_LEVEL > 0
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdReadItemRo: Found VPD item '%s' in RO section, "
				   "value='%s'", pKeyword, pVpdItem);
#endif
		return 0;
	}

	// Not found
#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdReadItemRo: Can't find VPD item '%s'", pKeyword);
#endif

	return -EFAULT;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdReadItemRw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read VPD item into a caller-allocated string buffer from the read-write section.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dtu2xxVpdReadItemRw(
	IN  PDTU2XX_FDO  pFdo,	// Device extension
	IN  char*  pKeyword,				// Name of VPD item to be read
	OUT char*  pVpdItem,				// String buffer for storing VPD item
	OUT UInt*  pLength)					// In: Length of pVpdItem buffer
										// Out: Length of string
{
	UInt  i, Length;
	char*  pFrom;				// Helper character pointer
	char*  pTo;					// Helper character pointer
	UInt8*  pVpd;				// Byte pointer in VPD data
	UInt8*  pResource;			// Resource to be read

	// Add parameter checks here
	if (*pLength == 0) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdReadItemRw: Invalid length: 0");
	}

	// Initialisation
	pTo = pVpdItem;				// For later copying to VPD item
	memset(pVpdItem, 0, *pLength);

	// Check whether we can find resource in RW section
	pVpd = &pFdo->m_pVpdEeprom[DTU2XX_VPD_RW_START];

	if (pVpd[0] != 0x91) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdReadItemRw: ERROR: VPD-RW section starts with 0x%02x "
				   "i.s.o. 0x91", pVpd[0]);

		return -EFAULT;
	}

	if (Dtu2xxVpdFindResource(pFdo, pKeyword, pVpd, DTU2XX_VPD_RW_LENGTH, &pResource)) {
 		pFrom = pResource + 3;
		Length = pResource[2];
		// Protect against invalid content of EEPROM or too small buffer
		if (Length > *pLength)
			Length = *pLength;
		for (i=0; i<Length; i++)
			*pTo++ = *pFrom++;
		*pLength = Length;
#if LOG_LEVEL > 0
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdReadItemRw: Found VPD item '%s' in RW section, "
				   "value='%s'", pKeyword, pVpdItem);
#endif
		return 0;
	}

	// Not found
#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdReadItemRw: Can't find VPD item '%s'", pKeyword);
#endif

	return -EFAULT;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdReadItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int Dtu2xxVpdReadItem(
    IN PDTU2XX_FDO  pFdo,			// Device extension
	DTU2XX_VPD_ITEM*  pVpdItem)
{
	char  Keyword[3];		// For '\0'-terminated keyword

	// Create '\0'-terminated keyword
	Keyword[0] = pVpdItem->m_VpdKeyword[0];
	Keyword[1] = pVpdItem->m_VpdKeyword[1];
	Keyword[2] = '\0';

	// Read from the right section
	if (pVpdItem->m_VpdSectionType == DTU2XX_VPDSECT_ID)
		return Dtu2xxVpdReadId(pFdo, pVpdItem->m_VpdBuf, &(pVpdItem->m_Length));

	if ((pVpdItem->m_VpdSectionType & DTU2XX_VPDSECT_RO) != 0) {
		if (Dtu2xxVpdReadItemRo(pFdo, Keyword, pVpdItem->m_VpdBuf,
												 &(pVpdItem->m_Length)) == 0)
			return 0;
	}

	if (pVpdItem->m_VpdSectionType & DTU2XX_VPDSECT_RW) {
		if (Dtu2xxVpdReadItemRw(pFdo, Keyword, pVpdItem->m_VpdBuf,
												 &(pVpdItem->m_Length)) == 0)
			return 0;
	}
	return -EFAULT;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdDeleteItemRo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Delete all VPD items with keyword pKeyword from the read-only section.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dtu2xxVpdDeleteItemRo(
	IN  PDTU2XX_FDO  pFdo,	// Device extension
	IN  char*  pKeyword)				// Name of VPD item to be deleted
{
	UInt  RoLength;
	UInt8*  pVpd;				// Byte pointer in VPD data
	UInt8*  pVpdRoEnd;			// Pointer one after RO section
	Int  Status;

	// Compute a number of helper variables
	pVpdRoEnd = &pFdo->m_pVpdEeprom[DTU2XX_VPD_RW_START];

	// Start at beginning of VPD-ID + read-only section
	Status = Dtu2xxVpdFindStartOfRoSection(pFdo, &pVpd);
	if (Status != 0)
		return Status;

	// Length of RO section
	RoLength = (UInt)(pVpdRoEnd - pVpd);

	return Dtu2xxVpdDeleteResource(pFdo,	pKeyword, pVpd, RoLength);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdDeleteItemRw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Delete all VPD items with keyword pKeyword from the read-write section.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dtu2xxVpdDeleteItemRw(
	IN  PDTU2XX_FDO  pFdo,	// Device extension
	IN  char*  pKeyword)				// Name of VPD item to be deleted
{
	UInt8*  pVpd;				// Byte pointer in VPD data

	// Check whether we can find resource in RW section
	pVpd = &pFdo->m_pVpdEeprom[DTU2XX_VPD_RW_START];
	return Dtu2xxVpdDeleteResource(pFdo,	pKeyword, pVpd, DTU2XX_VPD_RW_LENGTH);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdDeleteItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Delete all VPD items in the specified sections
//
Int  Dtu2xxVpdDeleteItem(
    IN PDTU2XX_FDO  pFdo,	// Device extension
	DTU2XX_VPD_ITEM*  pVpdItem)			// Name of VPD item to be deleted
{
	char  Keyword[3];		// For '\0'-terminated keyword
	Int  Status = 0;
	UInt  StartAddr = DTU2XX_VPD_RW_START;
	UInt  Length = 0;

	// Create '\0'-terminated keyword
	Keyword[0] = pVpdItem->m_VpdKeyword[0];
	Keyword[1] = pVpdItem->m_VpdKeyword[1];
	Keyword[2] = '\0';

	if ((pVpdItem->m_VpdSectionType & DTU2XX_VPDSECT_RO) != 0) {
		Status = Dtu2xxVpdDeleteItemRo(pFdo, Keyword);
		if (Status != 0)
			return Status;
		StartAddr = pFdo->m_VpdIdOffset;
		Length+= (DTU2XX_VPD_RW_START - pFdo->m_VpdIdOffset);
	}

	if ((pVpdItem->m_VpdSectionType & DTU2XX_VPDSECT_RW) != 0) {
		Status = Dtu2xxVpdDeleteItemRw(pFdo, Keyword);
		if (Status != 0)
			return Status;
		Length+= DTU2XX_VPD_RW_LENGTH;
	}

	// Update RO and RW section if needed
	if (Length != 0)
		Status = Dtu2xxVpdUpdateEepromFromCache(pFdo, StartAddr, Length);

	return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdCreateRV -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Create a RV item in the RO section.
//
Int  Dtu2xxVpdCreateRV(
	IN  PDTU2XX_FDO  pFdo)	// Device extension
{
	UInt  CheckSum = 0;
	UInt8*  pVpd;						// Byte pointer in VPD data
	UInt8*  pRv;
	Int  Status;

	Status = Dtu2xxVpdFindEndOfRoSection(pFdo, &pRv);
	if (Status != 0)
		return Status;

	// Create "RV" item
	pRv[0] = 'R';
	pRv[1] = 'V';
	pRv[2] = (&pFdo->m_pVpdEeprom[DTU2XX_VPD_RW_START] - &pRv[3]);

	// Compute checksum
	for (pVpd=pFdo->m_pVpdEeprom+pFdo->m_VpdIdOffset; pVpd<=pRv+2; pVpd++)
		CheckSum += *pVpd;
	pRv[3] = (~CheckSum + 1) & 0xFF;

	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdWriteId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write the VPD Id into the cache/EEPROM. If the VPD Id already exists,
// the old ID will be erased and the RO section will be moved.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dtu2xxVpdWriteId(
	IN  PDTU2XX_FDO  pFdo,	// Device extension
	IN char*  pVpdId,					// String buffer with VPD Id
	IN UInt   IdLength)
{
	UInt  Length, SizeAvail;
	UInt8*  pVpd;						// Byte pointer in VPD data
	UInt8*  pEndOfRo;

	// Initialisation
	SizeAvail = DTU2XX_VPD_RW_START - pFdo->m_VpdIdOffset - 3;
	Length = 0;

	// Start at beginning of VPD-ID + read-only section
	pVpd = &pFdo->m_pVpdEeprom[pFdo->m_VpdIdOffset];
	if (pVpd[0] == 0x82) {
		// VPDID already exists. Check if enough space is available.
		Length = pVpd[1] + ((UInt)pVpd[2]<<8);

		// Find last item
		pEndOfRo = pVpd + 3 + Length;

		if (pEndOfRo[0] == 0x90) {
			Int Size;
			UInt8* pSize;
			if (Dtu2xxVpdFindEndOfRoSection(pFdo, &pEndOfRo) == 0) {
				SizeAvail = DTU2XX_VPD_RW_START - (pEndOfRo - pFdo->m_pVpdEeprom) - 4;
			} else {
				SizeAvail = 0;
			}

			if ((SizeAvail + Length) < IdLength) {
				DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdWriteId: VPD RO SECTION FULL");
				return -EFAULT;
			}

			// Move existing RO section
			memmove(pVpd + 3 + IdLength, pVpd + 3 + Length,
					(DTU2XX_VPD_RW_START - pFdo->m_VpdIdOffset - SizeAvail - 3 - Length));

			// Update size of RO section.
			pSize = pVpd + 3 + IdLength;
			Size = pSize[1] + ((UInt)pSize[2] << 8);
			Size = Size + Length - IdLength;
			pSize[1] = Size & 0xff;
			pSize[2] = Size >> 8;

			// Set all bytes left over to 0
			if (Length > IdLength) {
				memset(pEndOfRo + 4 - (Length - IdLength), 0, Length - IdLength);
			}
		} else {
			SizeAvail = DTU2XX_VPD_RW_START - (pEndOfRo - pFdo->m_pVpdEeprom) - 3;
		}
	}

	// Write VPDID
	if (IdLength  > (SizeAvail + Length)) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdWriteId: VPD RO SECTION VPDID too long");
		return -EFAULT;
	}
	pVpd[0] = 0x82;		// ID-String tag
	pVpd[1] = IdLength & 0xff;
	pVpd[2] = (IdLength >> 8);
	memcpy(pVpd + 3, pVpdId, IdLength);

	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdWriteItemRo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write a VPD Item into the RO section of the VPD cache. If the Item already exists,
// the old Item will be erased.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dtu2xxVpdWriteItemRo(
	IN  PDTU2XX_FDO  pFdo,	// Device extension
	IN char*  pKeyword,					// String buffer with 2 bytes keyword
	IN char*  pVpdItem,					// String buffer with VPD Item
	IN UInt  ItemLength)				// Length of pVpdItem
{
	UInt  SizeAvail;
	UInt8*  pEndOfRo;
	Int  Status;

	Dtu2xxVpdDeleteItemRo(pFdo, pKeyword);

	Status = Dtu2xxVpdFindEndOfRoSection(pFdo, &pEndOfRo);
	if (Status != 0)
		return Status;

	SizeAvail = DTU2XX_VPD_RW_START - (pEndOfRo - pFdo->m_pVpdEeprom);
	if ((pKeyword[0] != 'R') || (pKeyword[1] != 'V')) {
		SizeAvail -= 4;
	}

	if (SizeAvail < (ItemLength + 3)) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdWriteItemRo: VPD RO SECTION FULL");
		return -EFAULT;
	}

	pEndOfRo[0] = pKeyword[0];
	pEndOfRo[1] = pKeyword[1];
	pEndOfRo[2] = (UInt8)ItemLength;

	memcpy(pEndOfRo + 3, pVpdItem, ItemLength);
	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdWriteItemRw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write a VPD Item into the RW section of the VPD cache. If the VPD Item already exists,
// the old Item will be erased.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dtu2xxVpdWriteItemRw(
	IN  PDTU2XX_FDO  pFdo,	// Device extension
	IN char*  pKeyword,					// String buffer with 2 bytes keyword
	IN char*  pVpdItem,					// String buffer with VPD Item
	IN UInt  ItemLength)				// Length of pVpdItem
{
	UInt  BytesUsed, SizeAvail;
	UInt8*  pVpdRw;						// Byte pointer in VPD data

	pVpdRw = &pFdo->m_pVpdEeprom[DTU2XX_VPD_RW_START];

	// Check structure in serial EEPROM
	if (pVpdRw[0]!=0x91 || pVpdRw[1]!=0xFC || pVpdRw[2]!=0x00 ||
		pVpdRw[0xFF]!=0x78) {

		// Basic VPD Read/Write Resources data structure not present. Create it.
		memset(pVpdRw, 0, DTU2XX_VPD_RW_LENGTH);
		pVpdRw[0] = 0x91;
		pVpdRw[1] = 0xFC;
		pVpdRw[0xFF] = 0x78;
	}

	Dtu2xxVpdDeleteItemRw(pFdo, pKeyword);

	// Calculate available bytes
	BytesUsed = Dtu2xxVpdGetSectionLength(pVpdRw, DTU2XX_VPD_RW_LENGTH);
	SizeAvail = DTU2XX_VPD_RW_LENGTH - BytesUsed;

	if (SizeAvail < (ItemLength + 3)) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdWriteItemRw: VPD RW SECTION FULL");
		return -EFAULT;
	}

	pVpdRw+= BytesUsed;
	pVpdRw[0] = pKeyword[0];
	pVpdRw[1] = pKeyword[1];
	pVpdRw[2] = (UInt8)ItemLength;

	memcpy(pVpdRw + 3, pVpdItem, ItemLength);
	return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdWriteItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int Dtu2xxVpdWriteItem(
    IN PDTU2XX_FDO  pFdo,		// Device extension
	DTU2XX_VPD_ITEM*  pVpdItem)				// VPD item to be written
{
	char  Keyword[3];		// For '\0'-terminated keyword
	BOOLEAN  UpdateEeprom = (pVpdItem->m_Flag & DTU2XX_VPDFLAG_UPDATE_EEPROM) != 0;
	Int  Status = -EFAULT;

	// Create '\0'-terminated keyword
	Keyword[0] = pVpdItem->m_VpdKeyword[0];
	Keyword[1] = pVpdItem->m_VpdKeyword[1];
	Keyword[2] = '\0';

	if ((pVpdItem->m_Flag & DTU2XX_VPDFLAG_RO_WRITE_ALLOWED) != 0) {

		if (pVpdItem->m_VpdSectionType == DTU2XX_VPDSECT_ID ||
			pVpdItem->m_VpdSectionType == DTU2XX_VPDSECT_RO) {

			switch (pVpdItem->m_VpdSectionType) {

			case DTU2XX_VPDSECT_ID:
				Status =  Dtu2xxVpdWriteId(pFdo,
										   pVpdItem->m_VpdBuf, pVpdItem->m_Length);
				break;
			case DTU2XX_VPDSECT_RO:
				Status = Dtu2xxVpdWriteItemRo(pFdo, Keyword,
											  pVpdItem->m_VpdBuf, pVpdItem->m_Length);
				break;
			}
			if (Status != 0)
				return Status;

			Status = Dtu2xxVpdCreateRV(pFdo);
			if (Status != 0)
				return Status;

			if (UpdateEeprom) {
				// Update RO section
				return Dtu2xxVpdUpdateEepromFromCache(pFdo, pFdo->m_VpdIdOffset,
									DTU2XX_VPD_RW_START - pFdo->m_VpdIdOffset);
			}
		}
	}

	if (pVpdItem->m_VpdSectionType == DTU2XX_VPDSECT_RW) {
		Status =  Dtu2xxVpdWriteItemRw(pFdo, Keyword,
									   pVpdItem->m_VpdBuf, pVpdItem->m_Length);
		if (Status != 0)
			return Status;

		if (UpdateEeprom) {
			// Update RW section
			return Dtu2xxVpdUpdateEepromFromCache(pFdo,
										DTU2XX_VPD_RW_START, DTU2XX_VPD_RW_LENGTH);
		}
	}
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Implementation of IOCTL_DTU2XX_VPD_DATA
//
Int Dtu2xxVpdIoctl(
	IN PDTU2XX_FDO  pFdo,			// Device extension
	IN UInt  InBufferSize,
	IN UInt  OutBufferSize,
	DTU2XX_VPD_DATA*  pVpdData)
{
	char*  CmdStr;						// Mnemonic string for Command
	Int  ReturnStatus = 0;

	switch (pVpdData->m_Command) {

	case DTU2XX_VPDCMD_ITEM_DELETE:
		CmdStr = "DTU2XX_VPDCMD_ITEM_DELETE";
		break;

	case DTU2XX_VPDCMD_ITEM_READ:
		CmdStr = "DTU2XX_VPDCMD_ITEM_READ";
		break;

	case DTU2XX_VPDCMD_ITEM_WRITE:
		CmdStr = "DTU2XX_VPDCMD_ITEM_WRITE";
		break;

	case DTU2XX_VPDCMD_RAW_READ:
		CmdStr = "DTU2XX_VPDCMD_RAW_READ";
		break;

	case DTU2XX_VPDCMD_RAW_WRITE:
		CmdStr = "DTU2XX_VPDCMD_RAW_WRITE";
		break;

	default:
		CmdStr = "??UNKNOWN VPDCMD CODE??";
	}


#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdDataIoctl: %s, In=%d , Out=%d",
			   CmdStr, InBufferSize, OutBufferSize);
#endif

	switch (pVpdData->m_Command) {

	case DTU2XX_VPDCMD_ITEM_DELETE:
		ReturnStatus = Dtu2xxVpdDeleteItem(pFdo, &pVpdData->m_Data.m_VpdItem);
		break;

	case DTU2XX_VPDCMD_ITEM_READ:
		ReturnStatus = Dtu2xxVpdReadItem(pFdo, &pVpdData->m_Data.m_VpdItem);
		break;

	case DTU2XX_VPDCMD_ITEM_WRITE:
		ReturnStatus = Dtu2xxVpdWriteItem(pFdo, &pVpdData->m_Data.m_VpdItem);
		break;

	case DTU2XX_VPDCMD_RAW_READ:
		ReturnStatus = Dtu2xxVpdReadFromCache(pFdo,
											  pVpdData->m_Data.m_VpdEeprom.m_VpdBuf,
											  pVpdData->m_Data.m_VpdEeprom.m_Start,
											  pVpdData->m_Data.m_VpdEeprom.m_Length);
		break;

	case DTU2XX_VPDCMD_RAW_WRITE:
		ReturnStatus = Dtu2xxVpdWrite(pFdo,
									  pVpdData->m_Data.m_VpdEeprom.m_VpdBuf,
									  pVpdData->m_Data.m_VpdEeprom.m_Start,
									  pVpdData->m_Data.m_VpdEeprom.m_Length);
		break;

	default:
		ReturnStatus = -ENOTTY;
	}
	return ReturnStatus;
}



//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read an array of bytes from the serial EEPROM.
//
Int  Dtu2xxVpdRead(
	IN PDTU2XX_FDO  pFdo,	// Device extension
	OUT UInt8*	pBuf,					// Buffer for data read from serial EEPROM
	IN  UInt  StartAddr,				// Start address (BYTE address)
	IN  UInt  Length)					// Number of BYTES to read
{
#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdRead: StartAddr=0x%03x Length=%d", StartAddr, Length);
#endif

	// Check parameters
	if (StartAddr >= DTU2XX_VPD_BUF_SIZE) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdRead: StartAddr=0x%03x INVALID", StartAddr);
		return -EFAULT;
	}
	if (StartAddr + Length > DTU2XX_VPD_BUF_SIZE) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdRead: Length=%d INVALID", Length);
		return -EFAULT;
	}

	// Let's be robust
	if (Length == 0)
		return 0;

	// Read directly from EEPROM
	return Dtu2xxReadEeProm(pFdo, StartAddr, Length, pBuf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdReadFromCache -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read an array of bytes from the EEPROM cache.
//
Int  Dtu2xxVpdReadFromCache(
	IN PDTU2XX_FDO  pFdo,	// Device extension
	OUT UInt8*	pBuf,					// Buffer for data read from serial EEPROM
	IN  UInt  StartAddr,				// Start address (BYTE address)
	IN  UInt  Length)					// Number of BYTES to read
{
#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdRead: StartAddr=0x%03x Length=%d", StartAddr, Length);
#endif

	// Check parameters
	if (StartAddr >= DTU2XX_VPD_BUF_SIZE) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdRead: StartAddr=0x%03x INVALID", StartAddr);
		return -EFAULT;
	}
	if (StartAddr + Length > DTU2XX_VPD_BUF_SIZE) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdRead: Length=%d INVALID", Length);
		return -EFAULT;
	}

	// Let's be robust
	if (Length == 0)
		return 0;

	// Read from VPD cache for all cards
	memcpy(pBuf, &pFdo->m_pVpdEeprom[StartAddr], Length);
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write an array of bytes to the serial EEPROM.
//
Int  Dtu2xxVpdWrite(
	IN PDTU2XX_FDO  pFdo,	// Device extension
	IN  UInt8*  pBuf,					// Buffer with data to write to serial EEPROM
	IN  UInt  StartAddr,				// Start address (BYTE address)
	IN  UInt  Length)					// Number of BYTES to read
{
	UInt  Addr;
	UInt  ByteIndex;
#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdWrite: StartAddr=0x%03x Length=%d", StartAddr, Length);
#endif

	// Nothing to write
	if ( Length == 0 )
		return 0;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Write into VPD cache -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	Addr = StartAddr;
	for (ByteIndex=0; ByteIndex<Length; ByteIndex++) {
		pFdo->m_pVpdEeprom[Addr++] = pBuf[ByteIndex];
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Update EEPROM -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	return Dtu2xxVpdUpdateEepromFromCache(pFdo, StartAddr, Length);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdUpdateEepromFromCache -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write an array of bytes to the serial EEPROM.
//
Int Dtu2xxVpdUpdateEepromFromCache(
	IN PDTU2XX_FDO  pFdo,	// Device extension
	IN  UInt  StartAddr,				// Start address (BYTE address)
	IN  UInt  Length)					// Number of BYTES to write
{
	Int  Status = 0;
	UInt8*  pBuf = pFdo->m_pVpdEeprom + StartAddr;

	// Write data to EEPROM
	Status = Dtu2xxWriteEeProm(pFdo, StartAddr, Length, pBuf);
	if ( Status != 0 )
		return Status;

	// Recalculate offset
	Dtu2xxVpdInitOffset(pFdo);
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdGetSectionLength -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt  Dtu2xxVpdGetSectionLength(
	UInt8*  pSection,		// Points to VPD-tag that starts the section
	UInt    Length)			// Total length of section, including stuffing
{
	UInt  LengthSoFar = 3;
	UInt8*  pResource = pSection + 3;				// Skip VPD-tag and 2-byte length

	while (Dtu2xxVpdIsValidResource(pResource, pSection, Length)) {
		UInt  ResLength = 3 + pResource[2];			// 3 for keyword + 1-byte length
		pResource += ResLength;
		LengthSoFar += ResLength;
	}
	return LengthSoFar;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdFindStartOfRoSection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dtu2xxVpdFindStartOfRoSection(
	IN  PDTU2XX_FDO  pFdo,	// Device extension
	OUT UInt8** pRoStart)
{
	UInt8* pVpd;
	UInt8* pVpdRoEnd;

	// Compute a number of helper variables
	pVpdRoEnd = &pFdo->m_pVpdEeprom[DTU2XX_VPD_RW_START];

	// Start at beginning of VPD-ID + read-only section
	pVpd = &pFdo->m_pVpdEeprom[pFdo->m_VpdIdOffset];

	if (pVpd[0] != 0x82) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdFindStartOfRoSection: ERROR: VPD-ID section starts "
				   "with 0x%02x i.s.o. 0x82", pVpd[0]);
		return -EFAULT;
	}

	// Move to start of VPD read-only section
	pVpd += 3 + pVpd[1] + (pVpd[2]<<8);

	// Consistency checks
	if (pVpd >= pVpdRoEnd) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdFindStartOfRoSection: ERROR: VPD-RO section starts "
				   "beyond max. end address");
		return -EFAULT;
	}
	if (pVpd[0] != 0x90) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxVpdFindStartOfRoSection: ERROR: VPD-RO section starts "
				   "with 0x%02x i.s.o. 0x90", pVpd[0]);
		return -EFAULT;
	}

	*pRoStart = pVpd;
	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdFindEndOfRoSection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dtu2xxVpdFindEndOfRoSection(
	IN  PDTU2XX_FDO  pFdo,		// Device extension
	OUT UInt8**  pItem)
{
	UInt8*  pEndOfRo;
	Int Status = 0;

	*pItem = NULL;

	Status = Dtu2xxVpdFindStartOfRoSection(pFdo, &pEndOfRo);
	if (Status != 0)
		return Status;

	while ((pEndOfRo < (pFdo->m_pVpdEeprom + DTU2XX_VPD_RW_START - 3)) &&
			(pEndOfRo[0]!='\0') && (!(pEndOfRo[0]=='R' && pEndOfRo[1]=='V'))) {
		pEndOfRo = pEndOfRo + 3 + pEndOfRo[2];
	}

	if (pEndOfRo > pFdo->m_pVpdEeprom + DTU2XX_VPD_RW_START - 4)
		return -EFAULT;

	*pItem = pEndOfRo;
	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdDeleteResource -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Keyword-based VPD-resource delete in given VPD section (RO or RW).
// All VPD-resources with the pKeyword found are deleted.
//
Int Dtu2xxVpdDeleteResource(
	IN PDTU2XX_FDO  pFdo,	// Device extension
	char*  pKeyword,					// Keyword to be found
	UInt8*  pSection,					// VPD section (starts with VPD tag)
	UInt  SectLength)					// Real section length
{
	UInt8*  pRes;			// Running resource pointer

	// Loop through resources
	if (!Dtu2xxVpdFindResource(pFdo, pKeyword, pSection, SectLength, &pRes))
		return -EFAULT;

	// Delete resource in a loop, so that multiple instances are deleted too.
	do {
		UInt8*  pTo = pRes;
		UInt8*  pNext = pRes + 3 + pRes[2];		// Next resource
        UInt  i;
		while (Dtu2xxVpdIsValidResource(pNext, pSection, SectLength)) {
			UInt  NextResourceLength = 3 + pNext[2];
			for (i = 0; i < NextResourceLength; i++)
				*pTo++ = *pNext++;
		}
		// Fill rest of section with zero's
		while (pTo < pSection + SectLength)
			*pTo++ = 0;
	} while (Dtu2xxVpdFindResource(pFdo, pKeyword, pSection, SectLength, &pRes));

	return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdFindResource -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Keyword-based VPD-resource search in given VPD section (RO or RW)
//
BOOLEAN  Dtu2xxVpdFindResource(
	IN PDTU2XX_FDO  pFdo,	// Device extension
	char*  pKeyword,					// Keyword to be found
	UInt8*  pSection,					// VPD section (starts with VPD tag)
	UInt  SectLength,					// Real section length
	UInt8**  ppResource)				// Points to VPD resource if keyword is found
{
	UInt8*  pRes;			// Running resource pointer
	UInt  ResLength;		// Resource length

	pRes = pSection + 3;

	// Loop through resources
	while (Dtu2xxVpdIsValidResource(pRes, pSection, SectLength)) {
		if (strncmp((char*) pRes, (char*) pKeyword, 2) == 0) {
			*ppResource = pRes;
			return TRUE;
		}
		ResLength = pRes[2] + 3;
		pRes += ResLength;
	}

	// No more valid keywords => can't find keyword
	return FALSE;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVpdIsValidResource -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Check whether pResource points to a correctly formatted VPD resource.
//
BOOLEAN  Dtu2xxVpdIsValidResource(
	UInt8*  pResource,		// Resource to be checked for validity
	UInt8*  pSection,		// Section in which keyword is located
	UInt    SectLength)		// Real section length (should correspond to pSection[1..2]+3
{
	if (pResource > pSection + SectLength - 3)
		return FALSE;
	if (!VALID_VPD_KEYW_CHAR(pResource[0]) || !VALID_VPD_KEYW_CHAR(pResource[1]))
		return FALSE;
	return (pResource + 3 + (UInt)(pResource[2])) <= (pSection + SectLength);
}
