//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Vpd.c *#*#*#*#*#*#*#*#*#*# (C) 2000-2008 DekTec
//
// Vital Product Data (VPD) read/write routines.

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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Tunable Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define VPD_WRARR_MAX_RETRIES	5		// Max. #times an array write is retried
#define VPD_WRITE_MAX_RETRIES	5		// Max. #times a VPD write is retried


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local Forwards -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
UInt  Dta1xxVpdGetSectionLength(UInt8*, UInt Length);
BOOLEAN  Dta1xxVpdFindResource(DTA1XX_FDO*, char*, UInt8*, UInt, UInt8**);
BOOLEAN  Dta1xxVpdIsValidResource(UInt8*, UInt8*, UInt);
Int  Dta1xxVpdDeleteResource(DTA1XX_FDO*, char*, UInt8*, UInt);
Int  Dta1xxVpdFindEndOfRoSection(DTA1XX_FDO*, UInt8**);
Int  Dta1xxVpdFindStartOfRoSection(DTA1XX_FDO*, UInt8**);
Int  Dta1xxVpdReadItemRo(DTA1XX_FDO*, char*, char*, UInt*);
Int  Dta1xxVpdReadItemRw(DTA1XX_FDO*, char*, char*, UInt*);
Int  Dta1xxVpdUpdateEepromFromCache(DTA1XX_FDO*,  UInt,  UInt);
Int  Dta1xxVpdWriteArray(DTA1XX_FDO*, UInt16*, UInt, UInt, UInt16*);
Int  Dta1xxVpdWriteWordWithCheck(DTA1XX_FDO*, UInt, UInt16, UInt16*);
Int  Dta1xxVpdDeleteItem(DTA1XX_FDO*, DTA1XX_VPD_ITEM*);
Int  Dta1xxVpdReadItem(DTA1XX_FDO*, DTA1XX_VPD_ITEM*);
Int  Dta1xxVpdWriteItem(DTA1XX_FDO*, DTA1XX_VPD_ITEM*);


// Local helpers
inline UInt16 UInt16ByteSwap(UInt16 a) { return ((a&0xFF)<<8) + ((a&0xFF00)>>8); }

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdInitOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Locate the start of the VpdId item.
// 9056 boards: offset PLX_VPD_ID_START_NEW (0x63)
// Other boards: offset PLX_VPD_ID_START (0x58)
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
void  Dta1xxVpdInitOffset(
	DTA1XX_FDO*  pFdo)		// Functional device object, representing the DTA-1xx card
{
	if (pFdo->m_pVpdEeprom[PLX_VPD_ID_START_NEW] == 0x82)
	{
		pFdo->m_VpdIdOffset = PLX_VPD_ID_START_NEW;
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdInitOffset: VpdOffset: 0x%04X",
					pFdo->m_VpdIdOffset);
#endif
	}
	else if (pFdo->m_pVpdEeprom[PLX_VPD_ID_START] == 0x82)
	{
		pFdo->m_VpdIdOffset = PLX_VPD_ID_START;
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdInitOffset: VpdOffset: 0x%04X",
					pFdo->m_VpdIdOffset );
#endif
	}
	else
	{
		pFdo->m_VpdIdOffset = 0;
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdInitOffset: Empty EEPROM or illegal layout" );
#endif
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Implementation of IOCTL_DTA1XX_VPD_DATA
//
Int  Dta1xxVpdIoctl(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	DTA1XX_VPD_DATA*  pVpdData)
{
	char*  CmdStr;					// Mnemonic string for Command
	Int  ReturnStatus = 0;

	switch (pVpdData->m_Command)
	{

	case DTA1XX_VPDCMD_ITEM_DELETE:
		CmdStr = "DTA1XX_VPDCMD_ITEM_DELETE";
		break;

	case DTA1XX_VPDCMD_ITEM_READ:
		CmdStr = "DTA1XX_VPDCMD_ITEM_READ";
		break;

	case DTA1XX_VPDCMD_ITEM_WRITE:
		CmdStr = "DTA1XX_VPDCMD_ITEM_WRITE";
		break;

	case DTA1XX_VPDCMD_RAW_READ:
		CmdStr = "DTA1XX_VPDCMD_RAW_READ";
		break;

	case DTA1XX_VPDCMD_RAW_WRITE:
		CmdStr = "DTA1XX_VPDCMD_RAW_WRITE";
		break;

	default:
		CmdStr = "??UNKNOWN VPDCMD CODE??";
	}

#if LOG_LEVEL > 1
	DTA1XX_LOG( KERN_INFO, "Dta1xxVpdDataIoctl: command=%s", CmdStr );
#endif

	switch (pVpdData->m_Command)
	{

	case DTA1XX_VPDCMD_ITEM_DELETE:
		ReturnStatus = Dta1xxVpdDeleteItem(pFdo, &pVpdData->m_Data.m_VpdItem);
		break;

	case DTA1XX_VPDCMD_ITEM_READ:
		ReturnStatus = Dta1xxVpdReadItem(pFdo, &pVpdData->m_Data.m_VpdItem);
		break;

	case DTA1XX_VPDCMD_ITEM_WRITE:
		ReturnStatus = Dta1xxVpdWriteItem(pFdo, &pVpdData->m_Data.m_VpdItem);
		break;

	case DTA1XX_VPDCMD_RAW_READ:
		ReturnStatus = Dta1xxVpdReadFromCache(pFdo,
											  pVpdData->m_Data.m_VpdEeprom.m_VpdBuf,
											  pVpdData->m_Data.m_VpdEeprom.m_Start,
											  pVpdData->m_Data.m_VpdEeprom.m_Length);
		break;

	case DTA1XX_VPDCMD_RAW_WRITE:
		ReturnStatus = Dta1xxVpdWrite(pFdo,
									  pVpdData->m_Data.m_VpdEeprom.m_VpdBuf,
									  pVpdData->m_Data.m_VpdEeprom.m_Start,
									  pVpdData->m_Data.m_VpdEeprom.m_Length);
		break;

	default:
		ReturnStatus = -ENOTTY;		// Unknown command
	}
	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdReadItemRoRw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read VPD item from Ro + Rw section into a caller-allocated string buffer.
// This routine is used to read PCI-card specific configuration data at
// driver-initialisation time.
//
// The pVpdItem will be null terminated
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dta1xxVpdReadItemRoRw(
	 DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	 char*  pKeyword,		// Name of VPD item to be read
	char*  pVpdItem)		// String buffer for storing VPD
							// Must be at least VPD_ITEM_MAX_LENGTH+1 long
{
	UInt  Length = DTA1XX_VPD_ITEM_MAX_LENGTH + 1;
	Int  Status;

	// Try to read item from read-only section, then from read-write section
	Status = Dta1xxVpdReadItemRo(pFdo, pKeyword, pVpdItem, &Length);
	if (Status != 0)
		Status = Dta1xxVpdReadItemRw(pFdo, pKeyword, pVpdItem, &Length);

	// '\0'-terminate item string
	if (Status == 0)
		pVpdItem[Length] = '\0';

	return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdReadId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read the VPD Id into a caller-allocated string buffer.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dta1xxVpdReadId(
	DTA1XX_FDO*  pFdo,	// Functional device object, representing the DTA-1xx card
	char*  pVpdId,		// String buffer for storing VPD Id
	UInt*  pLength)		// In: Length of pVpdId buffer
						// Out: Length of string
{
	char*  pFrom;				// Helper character pointer
	char*  pTo;					// Helper character pointer
	UInt  i, Length;
	UInt8*  pVpd;				// Byte pointer in VPD data

	// Add parameter checks here
	if (*pLength == 0) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdReadId: Invalid length: 0" );
#endif
		return -EFAULT;
	}

	// Initialisation
	pTo = pVpdId;				// For later copying to VPD item

	// Start at beginning of VPD-ID + read-only section
	pVpd = &pFdo->m_pVpdEeprom[pFdo->m_VpdIdOffset];
	if (pVpd[0] != 0x82) {
#if LOG_LEVEL > 1
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdReadId: ERROR: VPD-ID section starts with "
					"0x%02x i.s.o. 0x82", pVpd[0] );
#endif
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
#if LOG_LEVEL > 1
	DTA1XX_LOG( KERN_INFO, "Dta1xxVpdReadId: Found VPD Id, value='%s'", pVpdId );
#endif

	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdReadItemRo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read VPD item into a caller-allocated string buffer from the read-only section.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dta1xxVpdReadItemRo(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	char*  pKeyword,		// Name of VPD item to be read
	char*  pVpdItem,		// String buffer for storing VPD item
	UInt*  pLength)			// In: Length of pVpdItem buffer
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
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdReadItemRo: Invalid length: 0" );
#endif
		return -EFAULT;
	}

	// Compute a number of helper variables
	pVpdRoEnd = &pFdo->m_pVpdEeprom[PLX_VPD_RW_START];

	// Initialisation
	pTo = pVpdItem;				// For later copying to VPD item

	if (Dta1xxVpdFindStartOfRoSection(pFdo, &pVpd) != 0)
		return -EFAULT;

	// Length of RO section
	RoLength = (UInt)(pVpdRoEnd - pVpd);

	// Check whether we can find resource in RO section
	if ( Dta1xxVpdFindResource(pFdo, pKeyword, pVpd, RoLength, &pResource) )
	{
 		pFrom = pResource + 3;
		Length = pResource[2];
		// Protect against invalid content of EEPROM or too small buffer
		if ( Length > *pLength )
			Length = *pLength;
		for ( i=0; i<Length; i++ )
			*pTo++ = *pFrom++;
		*pLength = Length;
		return 0;
	}

	// Not found
#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxVpdReadItemRo: Can't find VPD item '%s'", pKeyword );
#endif
	return -EFAULT;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdReadItemRw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read VPD item into a caller-allocated string buffer from the read-write section.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dta1xxVpdReadItemRw(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	char*  pKeyword,		// Name of VPD item to be read
	char*  pVpdItem,		// String buffer for storing VPD item
	UInt*  pLength)			// In: Length of pVpdItem buffer
							// Out: Length of string
{
	UInt  i, Length;
	char*  pFrom;				// Helper character pointer
	char*  pTo;					// Helper character pointer
	UInt8*  pVpd;				// Byte pointer in VPD data
	UInt8*  pResource;			// Resource to be read

	// Add parameter checks here
	if (*pLength == 0) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdReadItemRw: Invalid length: 0" );
#endif
		return -EFAULT;
	}

	// Initialisation
	pTo = pVpdItem;				// For later copying to VPD item

	// Check whether we can find resource in RW section
	pVpd = &pFdo->m_pVpdEeprom[PLX_VPD_RW_START];

	if (pVpd[0] != 0x91) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdReadItemRw: ERROR: VPD-RW section starts with "
					"0x%02x i.s.o. 0x91", pVpd[0] );
#endif
		return -EFAULT;
	}

	if ( Dta1xxVpdFindResource(pFdo, pKeyword, pVpd, PLX_VPD_RW_LENGTH, &pResource) )
	{
 		pFrom = pResource + 3;
		Length = pResource[2];
		// Protect against invalid content of EEPROM or too small buffer
		if ( Length > *pLength )
			Length = *pLength;
		for ( i=0; i<Length; i++ )
			*pTo++ = *pFrom++;
		*pLength = Length;
		return 0;
	}

	// Not found
#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxVpdReadItemRw: Can't find VPD item '%s'", pKeyword );
#endif

	return -EFAULT;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdReadItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxVpdReadItem(
    DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	DTA1XX_VPD_ITEM*  pVpdItem)
{
	char  Keyword[3];		// For '\0'-terminated keyword

	// Create '\0'-terminated keyword
	Keyword[0] = pVpdItem->m_VpdKeyword[0];
	Keyword[1] = pVpdItem->m_VpdKeyword[1];
	Keyword[2] = '\0';

	// Read from the right section
	if ( pVpdItem->m_VpdSectionType == DTA1XX_VPDSECT_ID )
		return Dta1xxVpdReadId( pFdo, pVpdItem->m_VpdBuf, &(pVpdItem->m_Length) );

	if ( 0!=(pVpdItem->m_VpdSectionType & DTA1XX_VPDSECT_RO) )
	{
		if ( 0 == Dta1xxVpdReadItemRo(pFdo, Keyword, pVpdItem->m_VpdBuf,
									  &(pVpdItem->m_Length)) )
			return 0;
	}

	if ( 0!=(pVpdItem->m_VpdSectionType & DTA1XX_VPDSECT_RW) )
	{
		if ( 0 == Dta1xxVpdReadItemRw(pFdo, Keyword, pVpdItem->m_VpdBuf,
									  &(pVpdItem->m_Length)) )
			return 0;
	}
	return -EFAULT;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdDeleteItemRo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Delete all VPD items with keyword pKeyword from the read-only section.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dta1xxVpdDeleteItemRo(
	 DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	 char*  pKeyword)		// Name of VPD item to be deleted
{
	UInt  RoLength;
	UInt8*  pVpd;				// Byte pointer in VPD data
	UInt8*  pVpdRoEnd;			// Pointer one after RO section
	Int  Status;

	// Compute a number of helper variables
	pVpdRoEnd = &pFdo->m_pVpdEeprom[PLX_VPD_RW_START];

	// Start at beginning of VPD-ID + read-only section
	Status = Dta1xxVpdFindStartOfRoSection(pFdo, &pVpd);
	if (Status != 0)
		return Status;

	// Length of RO section
	RoLength = (UInt)(pVpdRoEnd - pVpd);

	return Dta1xxVpdDeleteResource( pFdo, pKeyword, pVpd, RoLength );
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdDeleteItemRw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Delete all VPD items with keyword pKeyword from the read-write section.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dta1xxVpdDeleteItemRw(
	 DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	 char*  pKeyword)		// Name of VPD item to be deleted
{
	UInt8*  pVpd;				// Byte pointer in VPD data

	// Check whether we can find resource in RW section
	pVpd = &pFdo->m_pVpdEeprom[PLX_VPD_RW_START];
	return Dta1xxVpdDeleteResource( pFdo, pKeyword, pVpd, PLX_VPD_RW_LENGTH );
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdDeleteItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Delete all VPD items in the specified sections
//
Int  Dta1xxVpdDeleteItem(
    DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	DTA1XX_VPD_ITEM*  pVpdItem)		// Name of VPD item to be deleted
{
	char  Keyword[3];		// For '\0'-terminated keyword
	Int  Status = 0;
	UInt  StartAddr = PLX_VPD_RW_START;
	UInt  Length = 0;

	// Create '\0'-terminated keyword
	Keyword[0] = pVpdItem->m_VpdKeyword[0];
	Keyword[1] = pVpdItem->m_VpdKeyword[1];
	Keyword[2] = '\0';

	if ( 0!=(pVpdItem->m_VpdSectionType & DTA1XX_VPDSECT_RO) )
	{
		Status = Dta1xxVpdDeleteItemRo(pFdo, Keyword);
		if ( Status != 0 )
			return Status;
		StartAddr = pFdo->m_VpdIdOffset;
		Length += (PLX_VPD_RW_START - pFdo->m_VpdIdOffset);
	}

	if ( 0!=(pVpdItem->m_VpdSectionType & DTA1XX_VPDSECT_RW) )
	{
		Status = Dta1xxVpdDeleteItemRw(pFdo, Keyword);
		if (Status != 0)
			return Status;
		Length+= PLX_VPD_RW_LENGTH;
	}

	// Update RO and RW section if needed
	if ( Length != 0 )
		Status = Dta1xxVpdUpdateEepromFromCache(pFdo, StartAddr, Length);

	return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdCreateRV -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Create a RV item in the RO section.
//
Int  Dta1xxVpdCreateRV(
	 DTA1XX_FDO*  pFdo)		// Functional device object, representing the DTA-1xx card
{
	UInt  CheckSum = 0;
	UInt8*  pVpd;						// Byte pointer in VPD data
	UInt8*  pRv;
	Int  Status;

	Status = Dta1xxVpdFindEndOfRoSection(pFdo, &pRv);
	if ( Status != 0 )
		return Status;

	// Create "RV" item
	pRv[0] = 'R';
	pRv[1] = 'V';
	pRv[2] = (&pFdo->m_pVpdEeprom[PLX_VPD_RW_START] - &pRv[3]);

	// Compute checksum
	for ( pVpd=pFdo->m_pVpdEeprom+pFdo->m_VpdIdOffset; pVpd<=pRv+2; pVpd++ )
		CheckSum += *pVpd;
	pRv[3] = (~CheckSum + 1) & 0xFF;

	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdWriteId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write the VPD Id into the cache/EEPROM. If the VPD Id already exists,
// the old ID will be erased and the RO section will be moved.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dta1xxVpdWriteId(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	char*  pVpdId,			// String buffer with VPD Id
	UInt   IdLength)
{
	UInt  Length, SizeAvail;
	UInt8*  pVpd;						// Byte pointer in VPD data
	UInt8*  pEndOfRo;

	// Initialisation
	SizeAvail = PLX_VPD_RW_START - pFdo->m_VpdIdOffset - 3;
	Length = 0;

	// Start at beginning of VPD-ID + read-only section
	pVpd = &pFdo->m_pVpdEeprom[pFdo->m_VpdIdOffset];
	if ( pVpd[0] == 0x82 )
	{
		// VPDID already exists. Check if enough space is available.
		Length = pVpd[1] + ((UInt)pVpd[2]<<8);

		// Find last item
		pEndOfRo = pVpd + 3 + Length;

		if (pEndOfRo[0] == 0x90)
		{
			Int Size;
			UInt8* pSize;
			if ( 0 == Dta1xxVpdFindEndOfRoSection(pFdo, &pEndOfRo) )
				SizeAvail = PLX_VPD_RW_START - (pEndOfRo - pFdo->m_pVpdEeprom) - 4;
			else
				SizeAvail = 0;

			if ( (SizeAvail + Length) < IdLength )
			{
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "Dta1xxVpdWriteId: VPD RO SECTION FULL" );
#endif
				return -EFAULT;
			}

			// Move existing RO section
			memmove(pVpd + 3 + IdLength, pVpd + 3 + Length,
					(PLX_VPD_RW_START - pFdo->m_VpdIdOffset - SizeAvail - 3 - Length));

			// Update size of RO section.
			pSize = pVpd + 3 + IdLength;
			Size = pSize[1] + ((UInt)pSize[2] << 8);
			Size = Size + Length - IdLength;
			pSize[1] = Size & 0xff;
			pSize[2] = Size >> 8;

			// Set all bytes left over to 0
			if ( Length > IdLength )
				memset(pEndOfRo + 4 - (Length - IdLength), 0, Length - IdLength);
		}
		else
			SizeAvail = PLX_VPD_RW_START - (pEndOfRo - pFdo->m_pVpdEeprom) - 3;
	}

	// Write VPDID
	if ( IdLength  > (SizeAvail + Length) )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdWriteId: VPD RO SECTION VPDID too long" );
#endif
		return -EFAULT;
	}
	pVpd[0] = 0x82;		// ID-String tag
	pVpd[1] = IdLength & 0xff;
	pVpd[2] = (IdLength >> 8);
	memcpy(pVpd + 3, pVpdId, IdLength);

	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdWriteItemRo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write a VPD Item into the RO section of the VPD cache. If the Item already exists,
// the old Item will be erased.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dta1xxVpdWriteItemRo(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	char*  pKeyword,		// String buffer with 2 bytes keyword
	char*  pVpdItem,		// String buffer with VPD Item
	UInt  ItemLength)		// Length of pVpdItem
{
	UInt  SizeAvail;
	UInt8*  pEndOfRo;
	Int  Status;

	Dta1xxVpdDeleteItemRo(pFdo, pKeyword);

	Status = Dta1xxVpdFindEndOfRoSection(pFdo, &pEndOfRo);
	if (Status != 0)
		return Status;

	SizeAvail = PLX_VPD_RW_START - (pEndOfRo - pFdo->m_pVpdEeprom);
	if ( (pKeyword[0] != 'R') || (pKeyword[1] != 'V') )
		SizeAvail -= 4;

	if ( SizeAvail < (ItemLength + 3) )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdWriteItemRo: VPD RO SECTION FULL" );
#endif
		return -EFAULT;
	}

	pEndOfRo[0] = pKeyword[0];
	pEndOfRo[1] = pKeyword[1];
	pEndOfRo[2] = (UInt8)ItemLength;

	memcpy(pEndOfRo + 3, pVpdItem, ItemLength);
	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdWriteItemRw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write a VPD Item into the RW section of the VPD cache. If the VPD Item already exists,
// the old Item will be erased.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
Int  Dta1xxVpdWriteItemRw(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	char*  pKeyword,		// String buffer with 2 bytes keyword
	char*  pVpdItem,		// String buffer with VPD Item
	UInt  ItemLength)		// Length of pVpdItem
{
	UInt  BytesUsed, SizeAvail;
	UInt8*  pVpdRw;						// Byte pointer in VPD data

	pVpdRw = &pFdo->m_pVpdEeprom[PLX_VPD_RW_START];

	// Check structure in serial EEPROM
	if ( pVpdRw[0]!=0x91 || pVpdRw[1]!=0xFC || pVpdRw[2]!=0x00 || pVpdRw[0xFF]!=0x78 )
	{
		// Basic VPD Read/Write Resources data structure not present. Create it.
		memset(pVpdRw, 0, PLX_VPD_RW_LENGTH);
		pVpdRw[0] = 0x91;
		pVpdRw[1] = 0xFC;
		pVpdRw[0xFF] = 0x78;
	}

	Dta1xxVpdDeleteItemRw(pFdo, pKeyword);

	// Calculate available bytes
	BytesUsed = Dta1xxVpdGetSectionLength(pVpdRw, PLX_VPD_RW_LENGTH);
	SizeAvail = PLX_VPD_RW_LENGTH - BytesUsed;

	if ( SizeAvail < (ItemLength + 3) )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdWriteItemRw: VPD RW SECTION FULL" );
#endif
		return -EFAULT;
	}

	pVpdRw+= BytesUsed;
	pVpdRw[0] = pKeyword[0];
	pVpdRw[1] = pKeyword[1];
	pVpdRw[2] = (UInt8)ItemLength;

	memcpy(pVpdRw + 3, pVpdItem, ItemLength);
	return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdWriteItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dta1xxVpdWriteItem(
    DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	DTA1XX_VPD_ITEM*  pVpdItem)		// VPD item to be written
{
	char  Keyword[3];		// For '\0'-terminated keyword
	BOOLEAN  UpdateEeprom = (pVpdItem->m_Flag & DTA1XX_VPDFLAG_UPDATE_EEPROM) != 0;
	Int  Status = -EFAULT;

	// Create '\0'-terminated keyword
	Keyword[0] = pVpdItem->m_VpdKeyword[0];
	Keyword[1] = pVpdItem->m_VpdKeyword[1];
	Keyword[2] = '\0';

	if ( 0!=(pVpdItem->m_Flag & DTA1XX_VPDFLAG_RO_WRITE_ALLOWED) )
	{
		if (pVpdItem->m_VpdSectionType == DTA1XX_VPDSECT_ID ||
			pVpdItem->m_VpdSectionType == DTA1XX_VPDSECT_RO)
		{

			switch (pVpdItem->m_VpdSectionType)
			{

			case DTA1XX_VPDSECT_ID:
				Status =  Dta1xxVpdWriteId(pFdo, pVpdItem->m_VpdBuf,
										   pVpdItem->m_Length);
				break;
			case DTA1XX_VPDSECT_RO:
				Status = Dta1xxVpdWriteItemRo(pFdo, Keyword, pVpdItem->m_VpdBuf,
											  pVpdItem->m_Length);
				break;
			}
			if (Status != 0)
				return Status;

			Status = Dta1xxVpdCreateRV(pFdo);
			if (Status != 0)
				return Status;

			if (UpdateEeprom)
			{
				// Update RO section
				return Dta1xxVpdUpdateEepromFromCache(pFdo, pFdo->m_VpdIdOffset,
												PLX_VPD_RW_START - pFdo->m_VpdIdOffset);
			}
		}
	}

	if ( pVpdItem->m_VpdSectionType == DTA1XX_VPDSECT_RW )
	{
		Status =  Dta1xxVpdWriteItemRw(pFdo, Keyword, pVpdItem->m_VpdBuf,
									   pVpdItem->m_Length);
		if (Status != 0)
			return Status;

		if (UpdateEeprom)
		{
			// Update RW section
			return Dta1xxVpdUpdateEepromFromCache(pFdo,
												  PLX_VPD_RW_START, PLX_VPD_RW_LENGTH);
		}
	}
	return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Reading and Writing Raw Bytes +=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read an array of bytes from the serial EEPROM.
//
Int  Dta1xxVpdRead(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	UInt8*	pBuf,			// Buffer for data read from serial EEPROM
	UInt  StartAddr,		// Start address (BYTE address)
	UInt  Length)			// Number of BYTES to read
{
	UInt  RelAddr, WordAddr, WordAddrLast;
	UInt16  Word16;
	UInt32  VpdDword;
	Int  Status;

#if LOG_LEVEL > 1
	DTA1XX_LOG( KERN_INFO, "Dta1xxVpdRead: StartAddr=0x%03x Length=%d", StartAddr, Length );
#endif

	// Check parameters
	if (StartAddr >= DTA1XX_VPD_BUF_SIZE)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdRead: StartAddr=0x%03x INVALID", StartAddr );
#endif
		return -EFAULT;
	}
	if (StartAddr + Length > DTA1XX_VPD_BUF_SIZE)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdRead: Length=%d INVALID", Length );
#endif
		return -EFAULT;
	}

	// Let's be robust
	if (Length == 0)
		return 0;

	if ( !pFdo->m_UsesPlxChip )
	{
		// First read complete EEPROM then use Dta1xxVpdReadFromCache to read from cache
		if (pFdo->m_UseFpgaForVpd)
		{
			// Read from EEPROM
			Dta1xxEepromReadLastPage(pFdo);
			return Dta1xxVpdReadFromCache(pFdo, pBuf, StartAddr, Length);
		}
		else
		{
			// Read from EEPROM
			Dta1xxEepromSpiRead(pFdo);
			return Dta1xxVpdReadFromCache(pFdo, pBuf, StartAddr, Length);
		}
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdRead: Not supported" );
#endif
		return -EFAULT;
	}

	// Compute start- and last 32-bit word addresses
	WordAddr = StartAddr / 4;
	RelAddr = StartAddr % 4;
	WordAddrLast = (StartAddr + Length - 1) / 4;

	// Read 32-bit word by word and extract bytes
	for ( ; WordAddr<=WordAddrLast; WordAddr++ )
	{
		// Read two 16-bit words, into 32-bit VpdWord
		Status = Dta1xxEepromReadWord16(pFdo, WordAddr*2, &Word16);
		if (Status != 0)
			return (Status);
		VpdDword = Word16<<16;
		Status = Dta1xxEepromReadWord16(pFdo, WordAddr*2+1, &Word16);
		if (Status != 0)
			return (Status);
		VpdDword |= Word16;

		// A little bit of magic to convert 32-bit word to bytes
		while (RelAddr<4 && Length!=0)
		{
			*pBuf++ = (UInt8) ((VpdDword >> (8 * (3-RelAddr))) & 0xFF);
			RelAddr++;
			Length--;
		}
		RelAddr = 0;
	}
	return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdReadFromCache -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read an array of bytes from the EEPROM cache.
//
Int  Dta1xxVpdReadFromCache(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	UInt8*	pBuf,			// Buffer for data read from serial EEPROM
	 UInt  StartAddr,		// Start address (BYTE address)
	 UInt  Length)			// Number of BYTES to read
{

#if LOG_LEVEL > 1
	DTA1XX_LOG( KERN_INFO, "Dta1xxVpdRead: StartAddr=0x%03x Length=%d", StartAddr, Length );
#endif

	// Check parameters
	if (StartAddr >= DTA1XX_VPD_BUF_SIZE)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdReadFromCache: StartAddr=0x%03x INVALID",
					StartAddr );
#endif
		return -EFAULT;
	}
	if (StartAddr + Length > DTA1XX_VPD_BUF_SIZE)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdReadFromCache: Length=%d INVALID", Length );
#endif
		return -EFAULT;
	}

	// Let's be robust
	if (Length == 0)
		return 0;

	// Read from VPD cache for all cards
	memcpy(pBuf, &pFdo->m_pVpdEeprom[StartAddr], Length);
	return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdUpdateEepromFromCache -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write an array of bytes to the serial EEPROM.
// Writing is complicated by the fact that we can only write 32-bit words to the
// PLX EEPROM.
//
Int  Dta1xxVpdUpdateEepromFromCache(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	UInt  StartAddr,		// Start address (BYTE address)
	UInt  Length)			// Number of BYTES to write
{
	Int  Status;
	UInt8*  pBuf = pFdo->m_pVpdEeprom + StartAddr;

	if (pFdo->m_UsesPlxChip)
	{
		Int  Try;
		UInt  ByteIndex, Mask, RelAddr, RelAddrStart, RelAddrLast;
		UInt  WordAddrStart, WordAddrLast;
		UInt16  WordBuf[DTA1XX_VPD_BUF_SIZE / 2];
		UInt16*  pWord;
		UInt16*  pWordCache = (UInt16*)pFdo->m_pVpdEeprom;

		//-.-.-.-.-.-.-.-.- Convert byte array to array of 16-bit words -.-.-.-.-.-.-.-.-.

		// Compute start and end 16-bit word addresses
		WordAddrStart = StartAddr / 2;
		RelAddrStart = StartAddr % 2;
		WordAddrLast = (StartAddr + Length - 1) / 2;
		RelAddrLast = (StartAddr + Length - 1) % 2;

		// Get first 16-bit word if we have a "partial" word at the beginning
		if (RelAddrStart != 0)
			WordBuf[WordAddrStart] = UInt16ByteSwap(pWordCache[WordAddrStart]);

		// Get last word if we have a "partial" word at the end
		if (RelAddrLast != 3)
			WordBuf[WordAddrLast] = UInt16ByteSwap(pWordCache[WordAddrLast]);

		// Construct array of 16-bit words to be written to the serial EEPROM
		// Array is Little Endian
		pWord = &WordBuf[WordAddrStart];
		RelAddr = RelAddrStart;
		for (ByteIndex=0; ByteIndex<Length; ByteIndex++)
		{
			Mask = 0xFF << (8 * (1-RelAddr));
			*pWord &= ~Mask;
			*pWord |= pBuf[ByteIndex] << (8 * (1-RelAddr));
			*pWord &= 0xFFFF;
			if (++RelAddr == 2)
			{
				RelAddr = 0;
				pWord++;
			}
		}

		//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Program serial EEPROM -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

		// Change "write-protected area" address in PCI 9054
		//	WRITE_UINT16(p905XRegs, PCI905X_PROT_AREA, WordAddrStart);

		// Set PE (Serial EEPROM Program Enable)
		//	pFdo->m_pDta1xxGen->m_GenControl.m_PE = 1;

		// Write until success, with a maximum number of retries
		for (Try=0; Try<VPD_WRARR_MAX_RETRIES; Try++)
		{
			// Write and readback into cache
			Status = Dta1xxVpdWriteArray(pFdo, WordBuf, WordAddrStart, WordAddrLast,
										 pWordCache);
			if ( Status==0 )
				break;
		}

		// Clear PE (Serial EEPROM Program Enable)
		// pFdo->m_pDta1xxGen->m_GenControl.m_PE = 0;

		// Disable "write-protected area" address in PCI 9054
		// WRITE_UINT16(p905XRegs, PCI905X_PROT_AREA, 0x7F);

		// Recalculate offset
		Dta1xxVpdInitOffset(pFdo);
		return Status;
	}

	if (pFdo->m_UseFpgaForVpd)
	{
		// Reprogram last sector with the VPD data
		Status = Dta1xxEepromWriteLastPage(pFdo);

		// Read back written content into cache
		Dta1xxEepromReadLastPage(pFdo);

		// Recalculate offset
		Dta1xxVpdInitOffset(pFdo);
		return Status;
	}

	// Write VPD data from cache
	Status = Dta1xxEepromSpiWrite(pFdo);

	// Read back written content into cache
	Dta1xxEepromSpiRead(pFdo);

	// Recalculate offset
	Dta1xxVpdInitOffset(pFdo);
    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write an array of bytes to the VPD cache and update the serial EEPROM.
//
Int  Dta1xxVpdWrite(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	UInt8*  pBuf,			// Buffer with data to write to serial EEPROM
	UInt  StartAddr,		// Start address (BYTE address)
	UInt  Length)			// Number of BYTES to write
{
	UInt  Addr;
	UInt  ByteIndex;

#if LOG_LEVEL > 1
	DTA1XX_LOG( KERN_INFO, "Dta1xxVpdWrite: StartAddr=0x%03x Length=%d pBuf=0x%p",
			    StartAddr, Length, pBuf );
#endif

	// Check parameters
	if (StartAddr >= DTA1XX_VPD_BUF_SIZE)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdWrite: StartAddr=0x%03x INVALID", StartAddr );
#endif
		return -EFAULT;
	}
	if (StartAddr + Length > DTA1XX_VPD_BUF_SIZE)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdWrite: Length=%d INVALID", Length );
#endif
		return -EFAULT;
	}

	// Let's be robust
	if (Length == 0)
		return 0;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Write into VPD cache -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	Addr = StartAddr;
	for (ByteIndex=0; ByteIndex<Length; ByteIndex++)
		pFdo->m_pVpdEeprom[Addr++] = pBuf[ByteIndex];

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Update EEPROM -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	return Dta1xxVpdUpdateEepromFromCache(pFdo, StartAddr, Length);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdGetSectionLength -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt  Dta1xxVpdGetSectionLength(
	UInt8*  pSection,		// Points to VPD-tag that starts the section
	UInt    Length)			// Total length of section, including stuffing
{
	UInt  LengthSoFar = 3;
	UInt8*  pResource = pSection + 3;				// Skip VPD-tag and 2-byte length

	while ( Dta1xxVpdIsValidResource(pResource, pSection, Length) )
	{
		UInt  ResLength = 3 + pResource[2];			// 3 for keyword + 1-byte length
		pResource += ResLength;
		LengthSoFar += ResLength;
	}
	return LengthSoFar;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdFindStartOfRoSection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxVpdFindStartOfRoSection(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	UInt8** pRoStart)
{
	UInt8* pVpd;
	UInt8* pVpdRoEnd;

	// Compute a number of helper variables
	pVpdRoEnd = &pFdo->m_pVpdEeprom[PLX_VPD_RW_START];

	// Start at beginning of VPD-ID + read-only section
	pVpd = &pFdo->m_pVpdEeprom[pFdo->m_VpdIdOffset];

	if (pVpd[0] != 0x82)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdFindStartOfRoSection: ERROR: VPD-ID section starts "
					"with 0x%02x i.s.o. 0x82", pVpd[0] );
#endif
		return -EFAULT;
	}

	// Move to start of VPD read-only section
	pVpd += 3 + pVpd[1] + (pVpd[2]<<8);

	// Consistency checks
	if (pVpd >= pVpdRoEnd)
	{
#if LOG_LEVEL > 1
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdFindStartOfRoSection: ERROR: VPD-RO section "
					"starts beyond max. end address" );
#endif
		return -EFAULT;
	}
	if (pVpd[0] != 0x90)
	{
#if LOG_LEVEL > 1
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdFindStartOfRoSection: ERROR: VPD-RO section "
					"starts with 0x%02x i.s.o. 0x90", pVpd[0] );
#endif
		return -EFAULT;
	}

	*pRoStart = pVpd;
	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdFindEndOfRoSection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dta1xxVpdFindEndOfRoSection(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	UInt8**  pItem)
{
	UInt8*  pEndOfRo;
	Int Status = 0;

	*pItem = NULL;

	Status = Dta1xxVpdFindStartOfRoSection(pFdo, &pEndOfRo);
	if (Status != 0)
		return Status;

	while (    (pEndOfRo < (pFdo->m_pVpdEeprom + PLX_VPD_RW_START - 3))
			&& (pEndOfRo[0]!='\0') && (!(pEndOfRo[0]=='R' && pEndOfRo[1]=='V')) )
	{
		pEndOfRo = pEndOfRo + 3 + pEndOfRo[2];
	}

	if (pEndOfRo > pFdo->m_pVpdEeprom + PLX_VPD_RW_START - 4)
		return -EFAULT;

	*pItem = pEndOfRo;
	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdDeleteResource -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Keyword-based VPD-resource delete in given VPD section (RO or RW).
// All VPD-resources with the pKeyword found are deleted.
//
Int  Dta1xxVpdDeleteResource(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	char*  pKeyword,		// Keyword to be found
	UInt8*  pSection,		// VPD section (starts with VPD tag)
	UInt  SectLength)		// Real section length
{
	UInt8*  pRes;			// Running resource pointer

	// Loop through resources
	if ( !Dta1xxVpdFindResource(pFdo, pKeyword, pSection, SectLength, &pRes) )
		return -EFAULT;

	// Delete resource in a loop, so that multiple instances are deleted too.
	do
	{
		UInt8*  pTo = pRes;
		UInt8*  pNext = pRes + 3 + pRes[2];		// Next resource
        UInt  i;
		while ( Dta1xxVpdIsValidResource(pNext, pSection, SectLength) )
		{
			UInt  NextResourceLength = 3 + pNext[2];
			for (i = 0; i < NextResourceLength; i++)
				*pTo++ = *pNext++;
		}
		// Fill rest of section with zero's
		while (pTo < pSection + SectLength)
			*pTo++ = 0;
	} while (Dta1xxVpdFindResource(pFdo, pKeyword, pSection, SectLength, &pRes));

	return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdFindResource -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Keyword-based VPD-resource search in given VPD section (RO or RW)
//
BOOLEAN  Dta1xxVpdFindResource(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	char*  pKeyword,		// Keyword to be found
	UInt8*  pSection,		// VPD section (starts with VPD tag)
	UInt  SectLength,		// Real section length
	UInt8**  ppResource)	// Points to VPD resource if keyword is found
{
	UInt8*  pRes;			// Running resource pointer
	UInt  ResLength;		// Resource length

	pRes = pSection + 3;

	// Loop through resources
	while ( Dta1xxVpdIsValidResource(pRes, pSection, SectLength) )
	{
		if (strncmp((char*) pRes, (char*) pKeyword, 2) == 0)
		{
			*ppResource = pRes;
			return TRUE;
		}
		ResLength = pRes[2] + 3;
		pRes += ResLength;
	}

	// No more valid keywords => can't find keyword
	return FALSE;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdIsValidResource -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Check whether pResource points to a correctly formatted VPD resource.
//
BOOLEAN  Dta1xxVpdIsValidResource(
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


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdWriteArray -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write an array of 16-bit words to the serial EEPROM and readback into cache.
// Assumes PROT_AREA in PCI 9054 and PE in Altera have been enabled.
//
Int  Dta1xxVpdWriteArray(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	UInt16*  pWordBuf,		// 16-bit data to be written (Little Endian)
	UInt  WordAddrStart,	// First 16-bit word in pWordBuf to be written
	UInt  WordAddrLast,		// Last 16-bit word in pWordBuf to be written
	UInt16*  pWordCacheBuf)	// 16-bit cache for readback data (Big Endian)
{
	UInt  WordAddr;
	UInt16 VpdWord;
	BOOLEAN  OkFlag = TRUE;
	Int  Status;

	// Loop through 16-bit words
	for (WordAddr=WordAddrStart; WordAddr<=WordAddrLast; WordAddr++)
	{
		// Read word first. Program only if word is required to change
		Status = Dta1xxEepromReadWord16(pFdo, WordAddr, &VpdWord);
		if (Status != 0)
			return Status;		// Reads must succeed
		if (VpdWord == pWordBuf[WordAddr])
		{
#if LOG_LEVEL > 2
			DTA1XX_LOG( KERN_INFO, "Dta1xxVpdWriteArray: WordAddr=0x%03x already "
						"contains 0x%08x", WordAddr, VpdWord );
#endif
			continue;
		}

#if LOG_LEVEL > 1
		DTA1XX_LOG( KERN_INFO, "Dta1xxVpdWriteArray: Program WordAddr=0x%03x with "
					"0x%08x", WordAddr, pWordBuf[WordAddr] );
#endif
		// Program 16-bit word to the serial EEPROM
		if ( 0!= (Dta1xxVpdWriteWordWithCheck(pFdo, WordAddr, pWordBuf[WordAddr],
											  &VpdWord)) )
		{
			OkFlag = FALSE;
		}

		// Store word into cache
		pWordCacheBuf[WordAddr] = UInt16ByteSwap(VpdWord);
	}
	return OkFlag ? 0 : -EFAULT;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxVpdWriteWordWithCheck -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write one 16-bit word to the serial EEPROM and check whether the word has been written
// successfully. Returns the word read.
// Retry max. VPD_WRITE_MAX_RETRIES times.
//
Int  Dta1xxVpdWriteWordWithCheck(
	DTA1XX_FDO*  pFdo,		// Functional device object, representing the DTA-1xx card
	UInt  WordAddr,			// 16-bit word address
	UInt16  DataWord,		// 16-bit data to be written
	UInt16*  VpdWord)		// 16-bit data read back
{
	Int  Try;
	BOOLEAN  OkFlag = FALSE;
	Int  Status;

	for ( Try=0; Try<VPD_WRITE_MAX_RETRIES; Try++ )
	{
		// Write word
		Status = Dta1xxEepromWriteWord16(pFdo, WordAddr, DataWord);
		if ( Status != 0 )
			continue;					// Try again if write fails

		// Read and check word
		Status = Dta1xxEepromReadWord16(pFdo, WordAddr, VpdWord);
		if (Status != 0)
			return Status;			// Reads must succeed
		if (*VpdWord == DataWord)
		{
			OkFlag = TRUE;
			break;
		}
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "Dta1xxVpdWriteWordWithCheck: Write FAILS: "
						"VpdWord=0x%03x", *VpdWord );
#endif
	}
	return OkFlag ? 0 : -EFAULT;
}
