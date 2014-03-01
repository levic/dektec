//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NwPhyMac.c *#*#*#*#*#*#*#*#* (C) 2005-2006 DekTec
//
// For NETWORK cards: PHY and MAC management routines
//
// NOTE: Routines are in alphabetic order
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	2005.08.09	SD	Moved PHY/MAC code to NwPhyMac.c	
//	2005.07.08	RD	Created	


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void Dta1xxNwMacClearInterruptsInitial(IN PNwStore pNwStore);
void Dta1xxNwMacInitPhy(IN PNwStore pNwStore);
void Dta1xxNwMacMiiSmGetLinkStatus(IN PNwStore pNwStore);
void Dta1xxNwMacMiiSmGetSetStatCnt(IN PNwStore pNwStore);
void Dta1xxNwMacMiiSmGetPhyId1(IN PNwStore pNwStore);
void Dta1xxNwMacMiiSmGetPhyId2(IN PNwStore pNwStore);
void Dta1xxNwMacMiiSmRead(UInt8* pBase, UInt8 RegisterAddress);
void Dta1xxNwMacMiiSmSetClockDivision(IN PNwStore pNwStore);
void Dta1xxNwMacMiiSmSetExtPhyControl(IN PNwStore pNwStore);
void Dta1xxNwMacMiiSmSetLoopbackSpeed(IN PNwStore pNwStore);
void Dta1xxNwMacMiiSmSetPhyControl(IN PNwStore pNwStore, IN UInt EnableLoopbackNow);
void Dta1xxNwMacMiiSmSetPhyControlInit(IN PNwStore pNwStore, IN UInt8 SubState);
void Dta1xxNwMacMiiSetCopperSpecControlRegister(IN PNwStore pNwStore);
void Dta1xxNwMacMiiSmSetPhySpecificInit(IN PNwStore pNwStore);
void Dta1xxNwMacMiiSetPageRegister(IN PNwStore pNwStore, UInt16 Page, UInt8 Cmd, UInt8 State);
void Dta1xxNwMacMiiProgramLedFunctionControl(IN PNwStore pNwStore);
void Dta1xxNwMacMiiProgramLedPolarityControl(IN PNwStore pNwStore);
void Dta1xxNwMacMiiProgramLedTimerControl(IN PNwStore pNwStore);
void Dta1xxNwMacMiiSmSet1GBControl(IN PNwStore pNwStore);
void Dta1xxNwMacMiiSmWrite(UInt8* pBase, UInt8 RegisterAddress, UInt16 Data);
NTSTATUS Dta1xxNwMacSendSetupFrame(IN PNwStore pNwStore);
#ifdef WINBUILD
void Dta1xxNwMacSendSetupFrameWI(IN PDEVICE_OBJECT pFdo, IN PVOID Context);
#else
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
	static void Dta1xxNwMacSendSetupFrameWI(IN PIO_WORKITEM pWork);
	#else
	static void Dta1xxNwMacSendSetupFrameWI(IN PVOID pContext);
	#endif
#endif
void Dta1xxNwMacSetIntMask(IN PNwStore pNwStore);
#ifdef WINBUILD
void Dta1xxNwStatCntCompletedDpc(IN PKDPC Dpc, IN PVOID DeferredContext, 
								 IN PVOID SystemArgument1, IN PVOID SystemArgument2);
#else
void Dta1xxNwStatCntCompletedDpc(IN unsigned long DeferredContext);
#endif

UInt32 Dta1xxNwMacCalcCrc(UInt8* pBuffer, UInt32 BufLen);

// Ethernet CRC-32 lookup table for fast CRC calculation
// Polinomial: 0x04C11DB7
//
static const UInt32 CrcTable[256] =
{
 0x00000000L, 0x77073096L, 0xEE0E612CL, 0x990951BAL,
 0x076DC419L, 0x706AF48FL, 0xE963A535L, 0x9E6495A3L,
 0x0EDB8832L, 0x79DCB8A4L, 0xE0D5E91EL, 0x97D2D988L,
 0x09B64C2BL, 0x7EB17CBDL, 0xE7B82D07L, 0x90BF1D91L,
 0x1DB71064L, 0x6AB020F2L, 0xF3B97148L, 0x84BE41DEL,
 0x1ADAD47DL, 0x6DDDE4EBL, 0xF4D4B551L, 0x83D385C7L,
 0x136C9856L, 0x646BA8C0L, 0xFD62F97AL, 0x8A65C9ECL,
 0x14015C4FL, 0x63066CD9L, 0xFA0F3D63L, 0x8D080DF5L,
 0x3B6E20C8L, 0x4C69105EL, 0xD56041E4L, 0xA2677172L,
 0x3C03E4D1L, 0x4B04D447L, 0xD20D85FDL, 0xA50AB56BL,
 0x35B5A8FAL, 0x42B2986CL, 0xDBBBC9D6L, 0xACBCF940L,
 0x32D86CE3L, 0x45DF5C75L, 0xDCD60DCFL, 0xABD13D59L,
 0x26D930ACL, 0x51DE003AL, 0xC8D75180L, 0xBFD06116L,
 0x21B4F4B5L, 0x56B3C423L, 0xCFBA9599L, 0xB8BDA50FL,
 0x2802B89EL, 0x5F058808L, 0xC60CD9B2L, 0xB10BE924L,
 0x2F6F7C87L, 0x58684C11L, 0xC1611DABL, 0xB6662D3DL,
 0x76DC4190L, 0x01DB7106L, 0x98D220BCL, 0xEFD5102AL,
 0x71B18589L, 0x06B6B51FL, 0x9FBFE4A5L, 0xE8B8D433L,
 0x7807C9A2L, 0x0F00F934L, 0x9609A88EL, 0xE10E9818L,
 0x7F6A0DBBL, 0x086D3D2DL, 0x91646C97L, 0xE6635C01L,
 0x6B6B51F4L, 0x1C6C6162L, 0x856530D8L, 0xF262004EL,
 0x6C0695EDL, 0x1B01A57BL, 0x8208F4C1L, 0xF50FC457L,
 0x65B0D9C6L, 0x12B7E950L, 0x8BBEB8EAL, 0xFCB9887CL,
 0x62DD1DDFL, 0x15DA2D49L, 0x8CD37CF3L, 0xFBD44C65L,
 0x4DB26158L, 0x3AB551CEL, 0xA3BC0074L, 0xD4BB30E2L,
 0x4ADFA541L, 0x3DD895D7L, 0xA4D1C46DL, 0xD3D6F4FBL,
 0x4369E96AL, 0x346ED9FCL, 0xAD678846L, 0xDA60B8D0L,
 0x44042D73L, 0x33031DE5L, 0xAA0A4C5FL, 0xDD0D7CC9L,
 0x5005713CL, 0x270241AAL, 0xBE0B1010L, 0xC90C2086L,
 0x5768B525L, 0x206F85B3L, 0xB966D409L, 0xCE61E49FL,
 0x5EDEF90EL, 0x29D9C998L, 0xB0D09822L, 0xC7D7A8B4L,
 0x59B33D17L, 0x2EB40D81L, 0xB7BD5C3BL, 0xC0BA6CADL,
 0xEDB88320L, 0x9ABFB3B6L, 0x03B6E20CL, 0x74B1D29AL,
 0xEAD54739L, 0x9DD277AFL, 0x04DB2615L, 0x73DC1683L,
 0xE3630B12L, 0x94643B84L, 0x0D6D6A3EL, 0x7A6A5AA8L,
 0xE40ECF0BL, 0x9309FF9DL, 0x0A00AE27L, 0x7D079EB1L,
 0xF00F9344L, 0x8708A3D2L, 0x1E01F268L, 0x6906C2FEL,
 0xF762575DL, 0x806567CBL, 0x196C3671L, 0x6E6B06E7L,
 0xFED41B76L, 0x89D32BE0L, 0x10DA7A5AL, 0x67DD4ACCL,
 0xF9B9DF6FL, 0x8EBEEFF9L, 0x17B7BE43L, 0x60B08ED5L,
 0xD6D6A3E8L, 0xA1D1937EL, 0x38D8C2C4L, 0x4FDFF252L,
 0xD1BB67F1L, 0xA6BC5767L, 0x3FB506DDL, 0x48B2364BL,
 0xD80D2BDAL, 0xAF0A1B4CL, 0x36034AF6L, 0x41047A60L,
 0xDF60EFC3L, 0xA867DF55L, 0x316E8EEFL, 0x4669BE79L,
 0xCB61B38CL, 0xBC66831AL, 0x256FD2A0L, 0x5268E236L,
 0xCC0C7795L, 0xBB0B4703L, 0x220216B9L, 0x5505262FL,
 0xC5BA3BBEL, 0xB2BD0B28L, 0x2BB45A92L, 0x5CB36A04L,
 0xC2D7FFA7L, 0xB5D0CF31L, 0x2CD99E8BL, 0x5BDEAE1DL,
 0x9B64C2B0L, 0xEC63F226L, 0x756AA39CL, 0x026D930AL,
 0x9C0906A9L, 0xEB0E363FL, 0x72076785L, 0x05005713L,
 0x95BF4A82L, 0xE2B87A14L, 0x7BB12BAEL, 0x0CB61B38L,
 0x92D28E9BL, 0xE5D5BE0DL, 0x7CDCEFB7L, 0x0BDBDF21L,
 0x86D3D2D4L, 0xF1D4E242L, 0x68DDB3F8L, 0x1FDA836EL,
 0x81BE16CDL, 0xF6B9265BL, 0x6FB077E1L, 0x18B74777L,
 0x88085AE6L, 0xFF0F6A70L, 0x66063BCAL, 0x11010B5CL,
 0x8F659EFFL, 0xF862AE69L, 0x616BFFD3L, 0x166CCF45L,
 0xA00AE278L, 0xD70DD2EEL, 0x4E048354L, 0x3903B3C2L,
 0xA7672661L, 0xD06016F7L, 0x4969474DL, 0x3E6E77DBL,
 0xAED16A4AL, 0xD9D65ADCL, 0x40DF0B66L, 0x37D83BF0L,
 0xA9BCAE53L, 0xDEBB9EC5L, 0x47B2CF7FL, 0x30B5FFE9L,
 0xBDBDF21CL, 0xCABAC28AL, 0x53B39330L, 0x24B4A3A6L,
 0xBAD03605L, 0xCDD70693L, 0x54DE5729L, 0x23D967BFL,
 0xB3667A2EL, 0xC4614AB8L, 0x5D681B02L, 0x2A6F2B94L,
 0xB40BBE37L, 0xC30C8EA1L, 0x5A05DF1BL, 0x2D02EF8DL
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-. Dta1xxNwMacCalcCrc .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Calculates 32-bit Ethernet Crc over a provided data buffer
//
UInt32 Dta1xxNwMacCalcCrc(UInt8* pBuffer, UInt32 BufLen)
{
	UInt32 Crc = 0xFFFFFFFF;
	UInt32 Index;

	for (Index = 0; Index < BufLen; Index++)
	{
		Crc = (Crc >> 8) ^ CrcTable[(UInt8)((Crc & 0xFF) ^ pBuffer[Index])];
	}

	return Crc;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEthGetCounter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Pre: Counters must be locked with function Dta1xxEthLockCounters
//
UInt32 Dta1xxEthGetCounter(IN PNwStore pNwStore, UInt Register)
{	UInt32 Value1 = 0;	// Initial value: 0
	UInt32 Value2 = 1;	// Initial value: 1

	// Workaround for R/W MAC problem in FPGA
	// Due to a problem in the FPGA with simultaneously read/write from/to the MAC by
	// driver and FPGA the following code is needed.
	// The FPGA is returning 0 if the read is not successfull.
	while ((Value1 != Value2) && (Value1 == 0 || Value2 == 0)) {
		Value1 = Dta1xxEthScModeGetCounterValue(pNwStore->m_pRegBase, Register);
		Value2 = Dta1xxEthScModeGetCounterValue(pNwStore->m_pRegBase, Register);
	}
	return Value2;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEthLockCounters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxEthLockCounters(IN PNwStore pNwStore)
{
	ExAcquireFastMutex(&pNwStore->m_FastMutexCounters);
	Dta1xxEthScModeLockCounters(pNwStore->m_pRegBase);
	while (!Dta1xxEthScModeAreCountersLocked(pNwStore->m_pRegBase)) {
		// Workaround for R/W MAC problem in FPGA
		// Write lock again due to problem in FPGA
		Dta1xxEthScModeLockCounters(pNwStore->m_pRegBase);
		Dta1xxWaitMs(1);
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEthGetSingleCounter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt32 Dta1xxEthGetSingleCounter(IN PNwStore pNwStore, UInt Register)
{	UInt32 Value1 = 0;	// Initial value: 0
	UInt32 Value2 = 1;	// Initial value: 1

	Dta1xxEthLockCounters(pNwStore);

    // Workaround for R/W MAC problem
	// Due to a problem in the FPGA with simultaneously read/write from/to the MAC by
	// driver and FPGA the following code is needed.
	// The FPGA is returning 0 if the read is not successfull.
	while ((Value1 != Value2) && (Value1 == 0 || Value2 == 0)) {
		Value1 = Dta1xxEthScModeGetCounterValue(pNwStore->m_pRegBase, Register);
		Value2 = Dta1xxEthScModeGetCounterValue(pNwStore->m_pRegBase, Register);
	}

	Dta1xxEthUnLockCounters(pNwStore);
	return Value2;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEthUnLockCounters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void Dta1xxEthUnLockCounters(IN PNwStore pNwStore)
{
	Dta1xxEthScModeUnLockCounters(pNwStore->m_pRegBase);
	while (!Dta1xxEthScModeAreCountersUnLocked(pNwStore->m_pRegBase)) {
		// Workaround for R/W MAC problem
		// Write unlock again due to problem in FPGA
		Dta1xxEthScModeUnLockCounters(pNwStore->m_pRegBase);
		Dta1xxWaitMs(1);
	}
	ExReleaseFastMutex(&pNwStore->m_FastMutexCounters);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEthWaitMacReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxEthWaitMacReady(IN PNwStore pNwStore)
{
	UInt32  Retry = 0;

	// Wait until reset is complete and MAC is ready
	// Wait max. 1 second
	while (((Dta1xxEthBusModeGet(pNwStore->m_pRegBase) & 0xFF000001) != 0xFE000000) && (Retry < 100)) {
		Dta1xxWaitMs(10);
		Retry++;
	}
	if (Retry >= 100) {
		DTA1XX_LOG(KERN_ERR, "Dta1xxEthWaitMacReady: ERROR WAITING FOR MAC to be ready!!");
		ASSERT(FALSE);
	} 
#if LOG_LEVEL_MAC > 2
	else
		DTA1XX_LOG(KERN_DEBUG, "Dta1xxEthWaitMacReady: Retry=%u!!", Retry);
#endif
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEthResetMac -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxEthResetMac(IN PNwStore pNwStore)
{
	// Software reset MAC
	Dta1xxEthBusModeSet(pNwStore->m_pRegBase, 0x1);
	
	// Wait until reset is complete
	Dta1xxEthWaitMacReady(pNwStore);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwResetPhyMacPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Reset Phy + Mac after powerup. Workaround because Mac is not handling this situation
// correctly if the clocks are not available during reset.
//
// Pre: Dta1xxNwMacInitMiiSm must be executed and ISR must be connected
//
void Dta1xxNwResetPhyMacPowerup(IN PNwStore pNwStore)
{
	// Set speed: autodetect
	pNwStore->m_NwiSpeed = 0;
	pNwStore->m_ForceSpeedEnable = FALSE;
	pNwStore->m_LoopbackEnable = FALSE;

    // Wait until Mac is powerd up
	Dta1xxEthWaitMacReady(pNwStore);
	
	// Enable phy so clocks are available
	Dta1xxNwMacMiiSmSetPhyControlInit(pNwStore, 0);

	// Wait for IDLE state
	while (pNwStore->m_MiiSm.SubState != 2)
		Dta1xxWaitMs(1);

	// Reset SubState
	pNwStore->m_MiiSm.State = MIISM_STATE_INITIALIZING;
	pNwStore->m_MiiSm.SubState = 0;

	Dta1xxWaitMs(10);
				
	// Reset MAC, Phy is autom. disabled by FPGA
	Dta1xxEthResetMac(pNwStore);

	// Lets init phy...
	Dta1xxNwMacInitPhy(pNwStore);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacClearInterrupts -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Clear the PHY interrupts
//
void  Dta1xxNwMacClearInterrupts(IN PNwStore pNwStore)
{	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;

	if (pNwStore->m_PhyType == PHY_NATIONAL) {
		Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_NAT_INT_CLR,
													DTA1XX_ETH_NW_NAT_INT_MASK_BITS);
	}  else {
		Dta1xxNwMacMiiSmRead(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_MAR_INT_CLR);
	}
	// We use the WRITE_WAIT also for Marvell so we don't need another state in the state machine.
	pNwStore->m_MiiSm.Command = MIISM_CMD_SET_CLR_INT;
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacClearInterruptsInitial -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Clear Phy Interrupts after initialisation. The m_MiiSm.Command is different for the
// nomal clear interrupt for the state machine.
//
void  Dta1xxNwMacClearInterruptsInitial(IN PNwStore pNwStore)
{	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;
	
	if (pNwStore->m_PhyType == PHY_NATIONAL) {
		Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_NAT_INT_CLR,
													DTA1XX_ETH_NW_NAT_INT_MASK_BITS);
	} else {
		Dta1xxNwMacMiiSmRead(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_MAR_INT_CLR);
	}
	// We use the WRITE_WAIT also for Marvell so we don't need another state in the state machine.
	pNwStore->m_MiiSm.Command = MIISM_CMD_SET_CLR_INT_INIT;
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSmCheckState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Executed from ISR: Periodic interrupt.
// Check if a previous read/write has been finished and handle the read/write.
//
UInt8  Dta1xxNwMacMiiSmCheckState(IN PNwStore pNwStore)
{
	UInt32  MiiCommand;
	
	switch (pNwStore->m_MiiSm.State) {
	case MIISM_STATE_PREPARING:
	case MIISM_STATE_INITIALIZING:
		// Don't do anything.
		return 0;

	case MIISM_STATE_IDLE:
		// If a command is pending, handle it

		if (pNwStore->m_SetupFramePending) {
			if (InsertQueueDpc(&pNwStore->m_SetupFrameDpc)) {
				// Handle pending setup frame
				// SetupFrame pending is reset in the Dpc routine
#if LOG_LEVEL_MAC > 0
				DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacCheckMiiSmState: "
						 "Handle pending setup frame");
#endif
			}
		}

		if (pNwStore->m_MacPending) {
			if (InsertQueueDpc(&pNwStore->m_MacDpc)) {
				// Handle pending Mac status
				// MacPending is reset in the Dpc routine
#if LOG_LEVEL_MAC > 0
				DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacCheckMiiSmState: "
						 "Handle pending MAC status");
#endif
			}
		}

		if (pNwStore->m_LinkPending && pNwStore->m_EnableNwiRx && (!pNwStore->m_LinkStatDPCBusy)) {
			pNwStore->m_LinkPending = FALSE;
			if (InsertQueueDpc(&pNwStore->m_LinkStatDPC)) {
				pNwStore->m_LinkStatDPCBusy = TRUE;
#if LOG_LEVEL_MAC > 0
				DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacCheckMiiSmState: "
						 "Handle pending link status to NW driver");
#endif
			}
		}
		
		if (pNwStore->m_InterruptPending) {
#if LOG_LEVEL_MAC > 2
			DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacCheckMiiSmState: "
					 "Handle pending MDIO interrupt");
#endif
			pNwStore->m_InterruptPending = FALSE;
			Dta1xxNwMacClearInterrupts(pNwStore);
			return 0;
		}

		if (pNwStore->m_SpeedPending) {
#if LOG_LEVEL_MAC > 0
			DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacCheckMiiSmState: "
					 "Handle pending speed setting");
#endif
			pNwStore->m_SpeedPending = FALSE;
			// First update the Auto/Manual 1Gb select,
			// thereafter set the Phy Control to force a software reset
			// for the Marvell Phy.
			Dta1xxNwMacMiiSmSet1GBControl(pNwStore);
			return 0;
		}

		if (pNwStore->m_StatCntPending) {
#if LOG_LEVEL_MAC > 2
			DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacCheckMiiSmState: "
				"Handle status/counter pending");
#endif
			pNwStore->m_StatCntPending = FALSE;
			Dta1xxNwMacMiiSmGetSetStatCnt(pNwStore);
			return 0;
		}
		return 1;
	
	case MIISM_STATE_WRITE_WAIT_RDY:
		// Check if write is finished
		MiiCommand = Dta1xxEthMiiSmRegGet(pNwStore->m_pRegBase);
		
		// Workaround for R/W MAC problem in FPGA
		if (MiiCommand == 0) {
			// Read for second time
			MiiCommand = Dta1xxEthMiiSmRegGet(pNwStore->m_pRegBase);
			if (MiiCommand == 0) {
				// Wait for next timer interrupt
				return 0;
			}
		}

		if ((MiiCommand & DTA1XX_ETH_MIISM_START_BUSY_MASK) == 0) {
			// Write finished
			if ((pNwStore->m_MiiSm.SubState != MIISM_SUBSTATE_BUSY) &&
				(pNwStore->m_MiiSm.SubState != MIISM_SUBSTATE_BUSYRST)){
				// Skip message if already busy.
#if LOG_LEVEL_MAC > 1
				DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacCheckMiiSmState: Write finished, "
						 "command=0x%x", pNwStore->m_MiiSm.Command);
#endif
			}

			// Prevent going to IDLE state in case of first PHY_CONTROL_INIT
			if ((pNwStore->m_MiiSm.Command == MIISM_CMD_SET_PHY_CONTROL_INIT) &&
				((pNwStore->m_MiiSm.SubState == 0) || (pNwStore->m_MiiSm.SubState == 2))){
				pNwStore->m_MiiSm.SubState = 2;
				return 0;
			}
			
			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_CLK_DIVIDER) {
				Dta1xxNwMacMiiSmGetPhyId1(pNwStore);
				return 0;
			}

			// MIISM_CMD_SET_CLR_INT_INIT is executed after MIISM_CMD_GET_PHY_ID1
            if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_CLR_INT_INIT) {
				// Enable int mask
				Dta1xxNwMacSetIntMask(pNwStore);
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_INT_MASK) {
				// Initialise Phy specific Settings
				Dta1xxNwMacMiiSmSetPhySpecificInit(pNwStore);
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_COP_SPEC_CTRL) {
				// We go to page 3 programming for Marvel 88E1119R
				Dta1xxNwMacMiiSetPageRegister(pNwStore, 3, MIISM_CMD_SET_PHY_PAGE_ADDR_LED, 
															MIISM_STATE_WRITE_WAIT_RDY);
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_PHY_PAGE_ADDR_LED) {
				// Page-address set, go further with led function control
				Dta1xxNwMacMiiProgramLedFunctionControl(pNwStore);
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_PHY_LED_FUNC_CTRL) {
				Dta1xxNwMacMiiProgramLedPolarityControl(pNwStore);
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_PHY_LED_POL_CTRL) {
				Dta1xxNwMacMiiProgramLedTimerControl(pNwStore);
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_PHY_LED_TIM_CTRL) {
				// Restore page 0 and return to phy_specific state
				Dta1xxNwMacMiiSetPageRegister(pNwStore, 0, MIISM_CMD_SET_PHY_SPECIFIC_INT,
															MIISM_STATE_WRITE_WAIT_RDY); 
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_PHY_SPECIFIC_INT) {
				// Initialise MAC address
				Dta1xxEthMacAddressSet(pNwStore->m_pRegBase, pNwStore->m_MacAddrCur);

				// Set BCMR Register
				Dta1xxNwMacMiiSmSetPhyControlInit(pNwStore, 1);
				return 0;
			}

			if ((pNwStore->m_MiiSm.Command == MIISM_CMD_SET_PHY_CONTROL_INIT) &&
				(pNwStore->m_MiiSm.SubState == 1)){
				// Send setup frame
				pNwStore->m_MiiSm.Command = MIISM_CMD_SET_SETUP_FRAME;
				pNwStore->m_MiiSm.SubState = MIISM_SUBSTATE_BUSY;
				InsertQueueDpc(&pNwStore->m_SetupFrameDpc);
#if LOG_LEVEL_MAC > 0
				DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacCheckMiiSmState: "
						 "Waiting for Setup frame to be completed");
#endif
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_SETUP_FRAME){
				
				if (pNwStore->m_MiiSm.SubState == MIISM_SUBSTATE_BUSY){
					return 0;
				} else {
#if LOG_LEVEL_MAC > 0
					DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacCheckMiiSmState: "
							 "Setup frame completed");
#endif
					// Last state...
					Dta1xxNwIntCtrlSetMdioIntEn(pNwStore->m_pRegBase, FALSE);
		
					// Clear interrupt and get Link Status
					Dta1xxNwMacClearInterrupts(pNwStore);
					return 0;
				}
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_CLR_INT) {
				// Set Mdio interrupt enable
				Dta1xxNwIntCtrlSetMdioIntEn(pNwStore->m_pRegBase, TRUE);
				
				// Get link status
				Dta1xxNwMacMiiSmGetLinkStatus(pNwStore);
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_1GB_CTRL) {
				// After updating manual 1GB Master/Slave mode, update speed setting
				// to force a software reset for the Marvell phy
				Dta1xxNwMacMiiSmSetPhyControl(pNwStore, 0);
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_STATCNT) {
				// We don't need to do any other MiiSm communication, so
				// we don't need to return 0.
				InsertQueueDpc(&pNwStore->m_StatCntDPC);
			}

			// For Marvell after setting speed, check if loopback mode must be enabled
			if ((pNwStore->m_MiiSm.Command == MIISM_CMD_SET_PHY_CONTROL) && 
				(pNwStore->m_PhyType == PHY_MARVELL) && pNwStore->m_LoopbackEnable) {

				pNwStore->m_MiiSm.Count = 0;
				// Enable loopback
				// For Marvel 88E3339R we have to go to page 2
				if (pNwStore->m_PhyModel == 0x28) {
					// We go to page 2 programming for Marvel 88E1119R
					Dta1xxNwMacMiiSetPageRegister(pNwStore, 2, MIISM_CMD_SET_PHY_PAGE_ADDR_LPB, 
																MIISM_STATE_WRITE_WAIT_RDY);
				
				} else {
					// First set the loopback speed
					Dta1xxNwMacMiiSmSetExtPhyControl(pNwStore);
				}
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_PHY_PAGE_ADDR_LPB) {
				Dta1xxNwMacMiiSmSetLoopbackSpeed(pNwStore);
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_LOOPBACKSPEED) {
				// Restore page 0 and return to phy ext control
				Dta1xxNwMacMiiSetPageRegister(pNwStore, 0, MIISM_CMD_SET_PHY_EXTCONTROL,
															MIISM_STATE_WRITE_WAIT_RDY); 
				return 0;
			}

			if (pNwStore->m_MiiSm.Command == MIISM_CMD_SET_PHY_EXTCONTROL) {
				// Wait minimal 700 ms before enabling loopback. 
				// See Marvell Release notes - 88E1111 Rev B2.
				pNwStore->m_MiiSm.Count++;
				if (pNwStore->m_MiiSm.Count >= (800 / DTA1XX_IPTX_PERINT)) {
					Dta1xxNwMacMiiSmSetPhyControl(pNwStore, 1);
				}
				return 0;
			}

			pNwStore->m_MiiSm.SubState = 0;
			pNwStore->m_MiiSm.State = MIISM_STATE_IDLE;
			return 1;
		}
		break;

	case MIISM_STATE_READ_WAIT_RDY:
		// Check if read is finished
		MiiCommand = Dta1xxEthMiiSmRegGet(pNwStore->m_pRegBase);

		// Workaround for R/W MAC problem in FPGA
		if (MiiCommand == 0) {
			// Read for second time
			MiiCommand = Dta1xxEthMiiSmRegGet(pNwStore->m_pRegBase);
			if (MiiCommand == 0) {
				// Wait for next timer interrupt
				return 0;
			}
		}

		if ((MiiCommand & DTA1XX_ETH_MIISM_START_BUSY_MASK) == 0) {
			// Read finished
#if LOG_LEVEL_MAC > 1
			DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacCheckMiiSmState: Read finished Command=0x%x",
					 pNwStore->m_MiiSm.Command);
#endif

			// Store value read
			switch (pNwStore->m_MiiSm.Command) {
			case MIISM_CMD_GET_PHY_ID1:
				switch (MiiCommand & 0xffff) {
					case 0x2000: 
						pNwStore->m_PhyType = PHY_NATIONAL;
						break;
					default:
						pNwStore->m_PhyType = PHY_MARVELL;
				}
#if LOG_LEVEL_MAC > 0
				DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacCheckMiiSmState: Command=0x%x PhyType: %s", 
							MiiCommand,
							pNwStore->m_PhyType == PHY_MARVELL ?"MARVELL\0":"NATIONAL\0");
#endif
				
				// Execute following state
				// GetPhyID2
				Dta1xxNwMacMiiSmGetPhyId2(pNwStore);
				return 0;
			case MIISM_CMD_GET_PHY_ID2:
				pNwStore->m_PhyModel = (UInt8)((MiiCommand >> 4) & 0x3f);
				
				DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacCheckMiiSmState: Command=0x%x PhyModel: %x\n", 
							MiiCommand,
							pNwStore->m_PhyModel);
				
				// Execute following state.
				// Reset interrupt status
				Dta1xxNwMacClearInterruptsInitial(pNwStore);
				return 0;

			case MIISM_CMD_GET_LINK_AUTONEG:
				if (pNwStore->m_PhyType == PHY_NATIONAL) {
					pNwStore->m_PhySpeed	= (UInt8)((MiiCommand & DTA1XX_ETH_LINK_AN_NAT_SPEEDST_MASK) >> DTA1XX_ETH_LINK_AN_NAT_SPEEDST_SHIFT);
					pNwStore->m_PhyLink		= (UInt8)((MiiCommand & DTA1XX_ETH_LINK_AN_NAT_LINKST_MASK)  >> DTA1XX_ETH_LINK_AN_NAT_LINKST_SHIFT);
					pNwStore->m_PhyDuplex	= (UInt8)((MiiCommand & DTA1XX_ETH_LINK_AN_NAT_DUPLEXST_MASK) >> DTA1XX_ETH_LINK_AN_NAT_DUPLEXST_SHIFT);
					pNwStore->m_PhyMaster	= (UInt8)((MiiCommand & DTA1XX_ETH_LINK_AN_NAT_MASTER_MASK) >> DTA1XX_ETH_LINK_AN_NAT_MASTER_SHIFT);
				} else {
					pNwStore->m_PhySpeed	= (UInt8)((MiiCommand & DTA1XX_ETH_LINK_AN_MAR_SPEEDST_MASK) >> DTA1XX_ETH_LINK_AN_MAR_SPEEDST_SHIFT);
					pNwStore->m_PhyLink		= (UInt8)((MiiCommand & DTA1XX_ETH_LINK_AN_MAR_LINKST_MASK)  >> DTA1XX_ETH_LINK_AN_MAR_LINKST_SHIFT);
					pNwStore->m_PhyDuplex	= (UInt8)((MiiCommand & DTA1XX_ETH_LINK_AN_MAR_DUPLEXST_MASK) >> DTA1XX_ETH_LINK_AN_MAR_DUPLEXST_SHIFT);
					pNwStore->m_PhyMaster	= 0;//(UInt8)((MiiCommand & DTA1XX_ETH_LINK_AN_MASTER_MASK) >> DTA1XX_ETH_LINK_AN_MASTER_SHIFT);
				}
				Dta1xxNwIntCtrlSetLinkValid(pNwStore->m_pRegBase, pNwStore->m_PhyLink == DTA1XX_ETH_NW_LINK_UP);
#if LOG_LEVEL_MAC > 0
				DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacCheckMiiSmState: Command=0x%08x "
						 "Speed=0x%02x Link=0x%x Duplex=0x%x",
							MiiCommand, pNwStore->m_PhySpeed, pNwStore->m_PhyLink,
							pNwStore->m_PhyDuplex);
#endif

				// Update status of Mac and Nw driver
				Dta1xxNwQueueLinkAutoNegChangeDpc(pNwStore);
				break;

			case MIISM_CMD_GET_STATCNT:
				pNwStore->m_StatCntValue = MiiCommand & 0xFFFF;
				InsertQueueDpc(&pNwStore->m_StatCntDPC);
				break;
			}

			pNwStore->m_MiiSm.SubState = 0;
			pNwStore->m_MiiSm.State    = MIISM_STATE_IDLE;
			return 1;
		}
		break;
	}
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSmGetLinkStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the link status of the PHY.
//
void  Dta1xxNwMacMiiSmGetLinkStatus(IN PNwStore pNwStore)
{
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;

	Dta1xxNwMacMiiSmRead(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_LINK_AN);

	pNwStore->m_MiiSm.Command  = MIISM_CMD_GET_LINK_AUTONEG;
	pNwStore->m_MiiSm.State    = MIISM_STATE_READ_WAIT_RDY;
	
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSmGetPhyId1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get Phy identifier 1
//
void  Dta1xxNwMacMiiSmGetPhyId1(IN PNwStore pNwStore)
{
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;

	Dta1xxNwMacMiiSmRead(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_PHYIDR1);

	pNwStore->m_MiiSm.Command  = MIISM_CMD_GET_PHY_ID1;
	pNwStore->m_MiiSm.State    = MIISM_STATE_READ_WAIT_RDY;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSmGetPhyId2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get Phy identifier 2
//
void  Dta1xxNwMacMiiSmGetPhyId2(IN PNwStore pNwStore)
{
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;

	Dta1xxNwMacMiiSmRead(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_PHYIDR2);

	pNwStore->m_MiiSm.Command  = MIISM_CMD_GET_PHY_ID2;
	pNwStore->m_MiiSm.State    = MIISM_STATE_READ_WAIT_RDY;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacGetSetMiiSmStatCnt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get/Set status / counter register from phy.
//
void  Dta1xxNwMacMiiSmGetSetStatCnt(IN PNwStore pNwStore)
{
	DTA1XX_IOCTL_DATA* pIoctlData = 
		(DTA1XX_IOCTL_DATA*) pNwStore->m_StatCntpIrp->AssociatedIrp.SystemBuffer;
	
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;

	if (pNwStore->m_StatCntMode == 1) {
		// Write status / counter
		pNwStore->m_MiiSm.Command  = MIISM_CMD_SET_STATCNT;
		pNwStore->m_MiiSm.State    = MIISM_STATE_WRITE_WAIT_RDY;
		Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, 
							  (UInt8)pIoctlData->m_NwStatCnt.m_Offset,
							  (UInt16)pIoctlData->m_NwStatCnt.m_Value);

	} else {
		// Read status / counter
		pNwStore->m_MiiSm.Command  = MIISM_CMD_GET_STATCNT;
		pNwStore->m_MiiSm.State    = MIISM_STATE_READ_WAIT_RDY;
		Dta1xxNwMacMiiSmRead(pNwStore->m_pRegBase, (UInt8)pIoctlData->m_NwStatCnt.m_Offset);

		
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwQueueLinkAutoNegChangeDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxNwQueueLinkAutoNegChangeDpc(IN PNwStore pNwStore)
{
	// To prevent changing during initialisation
	pNwStore->m_MacPending = TRUE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacSendSetupFrameDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#ifdef WINBUILD
void  Dta1xxNwMacSendSetupFrameDpc(
	IN PKDPC  Dpc,					// Supplies the DPC object
	IN PVOID  DeferredContext,		// Pointer to NwStore
									// Passed when KeInitializeDpc was called
	IN PVOID  SystemArgument1,		// Context1
	IN PVOID  SystemArgument2)		// Context2))
#else
void  Dta1xxNwMacSendSetupFrameDpc(
	unsigned long  DeferredContext)	// Pointer to NwStore
#endif
{
	PNwStore pNwStore = (PNwStore)DeferredContext;
	
	// Protect with a spinlock to prevent multiple access.
	// KeInsertQueueDpc is not SMP safe
	pNwStore->m_SetupFramePending = FALSE;
	
	if (TryLockMutexObject(&pNwStore->m_SetupFrameMutex) == FALSE) {
		pNwStore->m_SetupFramePending = TRUE;
		return;
	}

	if (pNwStore->m_SetupFrameRunning) {
#if LOG_LEVEL_MAC > 2
		DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacSendSetupFrameDpc: Already running");
#endif
		pNwStore->m_SetupFramePending = TRUE;
		ReleaseMutex(&pNwStore->m_SetupFrameMutex);
		return;
	}

#ifdef WINBUILD
	pNwStore->m_pSetupFrameWI = IoAllocateWorkItem(pNwStore->m_pFdo->m_pFdo);

	if (pNwStore->m_pSetupFrameWI == NULL) {
		DTA1XX_LOG(KERN_ERR, "Dta1xxNwMacSendSetupFrameDpc: Unable to create workitem "
				 "for sending Setup Frame");
		if ((pNwStore->m_MiiSm.State   == MIISM_STATE_WRITE_WAIT_RDY) &&
			(pNwStore->m_MiiSm.Command == MIISM_CMD_SET_SETUP_FRAME)) {
			pNwStore->m_MiiSm.SubState = MIISM_SUBSTATE_READY;
			pNwStore->m_SetupFramePending = TRUE;
		}
	} else {
		IoQueueWorkItem(&(pNwStore->m_SetupFrameWI), Dta1xxNwMacSendSetupFrameWI,
						DelayedWorkQueue, pNwStore);
	}

#else
	pNwStore->m_SetupFrameRunning = TRUE;
	IoQueueWorkItem(&(pNwStore->m_SetupFrameWI), Dta1xxNwMacSendSetupFrameWI,
						DelayedWorkQueue, pNwStore);
#endif
	ReleaseMutex(&pNwStore->m_SetupFrameMutex);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacSendSetupFrameWI -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#ifdef WINBUILD
void Dta1xxNwMacSendSetupFrameWI(IN PDEVICE_OBJECT pFdo, IN PVOID Context)
#else

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
static void Dta1xxNwMacSendSetupFrameWI(IN PIO_WORKITEM pWork)
#else
static void Dta1xxNwMacSendSetupFrameWI(IN PVOID pContext)
#endif

#endif
{
	PNwStore pNwStore;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
	pNwStore = container_of(pWork, struct _NwStore, m_SetupFrameWI);
#else
	pNwStore = (PNwStore)pContext;
#endif
		
	Dta1xxNwMacSendSetupFrame(pNwStore);

	// If the setup frame is send by the initialisation process, change the substate to ready
	if ((pNwStore->m_MiiSm.State   == MIISM_STATE_WRITE_WAIT_RDY) &&
		(pNwStore->m_MiiSm.Command == MIISM_CMD_SET_SETUP_FRAME)) {
		pNwStore->m_MiiSm.SubState = MIISM_SUBSTATE_READY;
	}

	pNwStore->m_SetupFrameRunning = FALSE;
}

//.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacHandleLinkAutoNegChangeDpc -.-.-.-.-.-.-.-.-.-.-.-.-
//
// Called when a read of the link status of the PHY has been finished or PacketFilter has been changed. 
// Updating MAC registers and set status to Nw driver.
//
#ifdef WINBUILD
void  Dta1xxNwMacHandleLinkAutoNegChangeDpc(
	IN PKDPC  Dpc,					// Supplies the DPC object
	IN PVOID  DeferredContext,		// Pointer to NwStore
									// Passed when KeInitializeDpc was called
	IN PVOID  SystemArgument1,		// Context1
	IN PVOID  SystemArgument2)		// Context2)
#else
void  Dta1xxNwMacHandleLinkAutoNegChangeDpc(
	unsigned long DeferredContext)  // Pointer to NwStore
#endif
{
	PNwStore pNwStore;
	UInt8*  pBase;
	UInt32  Status, Value;
	//BOOLEAN Timeout;
	UInt  Retry = 3;
	
	pNwStore = (PNwStore)DeferredContext;
	
	// Protect with a spinlock to prevent multiple access.
	// KeInsertQueueDpc is not SMP safe
	pNwStore->m_MacPending = FALSE;

	if (TryLockMutexObject(&pNwStore->m_MacMutex) == FALSE) {
		pNwStore->m_MacPending = TRUE;
		return;
	}
	
	pBase = pNwStore->m_pRegBase;
#if LOG_LEVEL_MAC > 0	
	DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacHandleLinkAutoNegChangeDpc");
#endif
	
	ASSERT(KeGetCurrentIrql() <= DISPATCH_LEVEL);	

	//Indicate link status to the Nw driver if changed
	if ((pNwStore->m_NwiLink != pNwStore->m_PhyLink) &&
		(pNwStore->m_NwiLink != 2)) {	// The first link update must be from the indicate link status
		pNwStore->m_NwiLink = pNwStore->m_PhyLink;
		pNwStore->m_LinkPending = TRUE;
	}
	
	// Is packet filtering changed?
	if (pNwStore->m_MacPacketFilter != pNwStore->m_NwiPacketFilter) {
		// Settings can be changed without stopping Tx/Rx
		
		// Update Mac packet filtering
		Value = Dta1xxEthOperModeGet(pBase);
		
		// Promiscuous
		if (pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_PROMISCUOUS) {
			Value = Value | (1 << 6);
		} else Value = Value & ~(1 << 6);
		
		// Always promiscious
		//Value = Value | (1 << 6);
		//KdPrint(("Promiscious receive is always on"));
		
		// Pass all multicast
		if (pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_ALL_MULTICAST ) {
			Value = Value | (1 << 7);
		} else Value = Value & ~(1 << 7);

		// Always multicast
		//Value = Value | (1 << 7);
		//KdPrint(("Multicast receive is always on"));
		
		// Store and forward on
		Value = Value | (1 << 21);
#if LOG_LEVEL_MAC > 1
		DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacHandleLinkAutoNegChangeDpc: SF transmit on");
#endif

		Dta1xxEthOperModeSet(pNwStore->m_pRegBase, Value);
	
		pNwStore->m_MacPacketFilter = pNwStore->m_NwiPacketFilter;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacHandleLinkAutoNegChangeDpc: "
				 "MacPacketFilter=0x%x Promiscious=%d Multicast=%d",
				 pNwStore->m_MacPacketFilter,
				(pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_PROMISCUOUS) != 0, 
				(pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_ALL_MULTICAST) != 0);
#endif
	}
	
	// Update MAC status only if link is up.
	// Otherwise the MAC will not be disabled.
					
	if (pNwStore->m_PhyLink == DTA1XX_ETH_NW_LINK_UP) {
		// Some data changed?

		if ((pNwStore->m_MacSpeed  != pNwStore->m_PhySpeed) ||
			(pNwStore->m_MacDuplex != pNwStore->m_PhyDuplex)){

			// MAC settings are not identical to PHY, update settings
#if LOG_LEVEL_MAC > 0
			DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacHandleLinkAutoNegChangeDpc: "
					 "Speed=0x%02x Link=0x%x Duplex=0x%x", 
					 pNwStore->m_PhySpeed, pNwStore->m_PhyLink, pNwStore->m_PhyDuplex);
#endif
		
			// Disable MAC Transmit and Receive
			Dta1xxEthEnableTx(pBase, FALSE);
			Dta1xxEthEnableRx(pBase, FALSE);

			// Wait until transmit and receive is disabled
			Status = Dta1xxEthStatusGet(pBase) & DTA1XX_ETH_STATUS_SEND_RECEIVE_MASK;
			
			// Problem: Not disabled if link is down
			while (Status && Retry--) {
				Status = Dta1xxEthStatusGet(pBase) & DTA1XX_ETH_STATUS_SEND_RECEIVE_MASK;
				
				if (Status & DTA1XX_ETH_STATUS_SEND_RECEIVE_MASK) {
					DTA1XX_LOG(KERN_ERR, " Dta1xxNwMacHandleLinkAutoNegChangeDpc: "
							 "Transmit/Receive not disabled! Status=0x%x", Status);
				}
			}

			if (Status) {
				Retry = 0xff;
			}
				
			Value = Dta1xxEthOperModeGet(pBase);

			if (pNwStore->m_PhySpeed == DTA1XX_ETH_NW_SPEED_1000) {
				// 1 GB network enabled
				Value = Value | (1 << 16);
			} else {
				// 10/100MB network enabled
				Value = Value & ~(1 << 16);
			}

			if (pNwStore->m_PhyDuplex == DTA1XX_ETH_NW_DUPLEX_FULL) {
				// Full duplex mode
				Value = Value | (1 << 9);
			} else {
				// Half duplex
				Value = Value & ~(1 << 9);
			}

			// Write result
			Dta1xxEthOperModeSet(pNwStore->m_pRegBase, Value);
			
			if (Retry != 0xff) {
				pNwStore->m_MacSpeed		 = pNwStore->m_PhySpeed;
				pNwStore->m_MacDuplex		 = pNwStore->m_PhyDuplex;
				//pNwStore->m_MacPacketFilter  = pNwStore->m_NwiPacketFilter;
			} else {
				pNwStore->m_MacSpeed  = 4;		// Not set
				pNwStore->m_MacDuplex = 2;		// Not set
			}

			// Wait 10uS before enabling MAC to wait for stable clock 
			KeStallExecutionProcessor(10);

			// Enable MAC Transmit and Receive
			Dta1xxEthEnableTx(pNwStore->m_pRegBase, TRUE);
			Dta1xxEthEnableRx(pNwStore->m_pRegBase, TRUE);
		}
	}
	// <SITO>: IS DIT OK? 
	// <RD> Eigenlijk wel, maar mag niet op de huidige IRQL level!!
	//IoReleaseRemoveLock(&pNwStore->m_pFdo->m_RemoveLock, pNwStore); 
	ReleaseMutex(&pNwStore->m_MacMutex);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacInitMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise MAC address from VPD data.
//
NTSTATUS  Dta1xxNwMacInitMacAddress(IN PNwStore pNwStore)
{
	UInt  MacSeq, MacSeq1, MacSeq2, MacSeq3, MacSeq4;
	UInt  MacAddrLow, MacAddrHigh;
	Int   i;
	NTSTATUS  Status;
	char  StrMC[DTA1XX_VPD_ITEM_MAX_LENGTH+1];	// Buffer for storing VPD item string
#if LOG_LEVEL_MAC > 0
	DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacInitMacAddress");
#endif

	// Read resource "MC" from VPD
	// <RD:> For multiple MACS, other Vpd Item must be used
	Status = Dta1xxVpdReadItemRoRw(pNwStore->m_pFdo, "MC", StrMC);
	if (Status == STATUS_SUCCESS) {
		// Convert MC-string to 24-bit MAC sequence number
		MacSeq1 = StrMC[0] - 0x20;
		MacSeq2 = StrMC[1] - 0x20;
		MacSeq3 = StrMC[2] - 0x20;
		MacSeq4 = StrMC[3] - 0x20;
		MacSeq = (MacSeq1<<18) | (MacSeq2<<12) | (MacSeq3<<6) | MacSeq4;
	} else {
		// No "MC" resource found: use 0 as MAC sequence number
		// This is not an error, but the initial situation when the DTA-160/2160 is
		// powered up for the first time after manufacturing. In this case, the
		// board should start up with MAC address 00-14-F4-00-00-00
		MacSeq = 0;
	}

	// Convert to 48-bit MAC address by prepending DekTec OUI (00-14-F4)
	// MSB/LSB is swapped!
	MacAddrHigh  = 0xF4000000 | MacSeq;
	MacAddrLow   = 0x0014;

	switch(pNwStore->m_pFdo->m_TypeNumber) {
		case 160:	// Nothing to do
			break;
		case 2160:
			MacAddrHigh+= 0x10 << 14;
			break;
	}

	// Store MAC address in device extension
	pNwStore->m_MacAddrPer[5] = (MacAddrHigh & 0xff);
	pNwStore->m_MacAddrPer[4] = (MacAddrHigh >>  8) & 0xff;
	pNwStore->m_MacAddrPer[3] = (MacAddrHigh >> 16) & 0xff;
	pNwStore->m_MacAddrPer[2] = (MacAddrHigh >> 24) & 0xff;
	pNwStore->m_MacAddrPer[1] = (MacAddrLow & 0xff);
	pNwStore->m_MacAddrPer[0] = (MacAddrLow >> 8) & 0xff;
	
	for (i=0; i < 6; i++) {
		pNwStore->m_MacAddrCur[i] = pNwStore->m_MacAddrPer[i];
	}
#if LOG_LEVEL_MAC > 0
	DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacInitMacAddress: Set MAC address to "
			 "%02x-%02x-%02x-%02x-%02x-%02x",
			 pNwStore->m_MacAddrCur[0], pNwStore->m_MacAddrCur[1], pNwStore->m_MacAddrCur[2],
			 pNwStore->m_MacAddrCur[3], pNwStore->m_MacAddrCur[4], pNwStore->m_MacAddrCur[5]);
#endif

	
	// Set MAC address in firmware
	Dta1xxEthMacAddressSet(pNwStore->m_pRegBase, pNwStore->m_MacAddrCur);
	return STATUS_SUCCESS;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacInitMiiSm -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise MII Serial Management registers and create DPC for link auto neg. update
//
void  Dta1xxNwMacInitMiiSm(IN PNwStore pNwStore)
{
	// Set state to initializing, so no pending commands will be executed
	// before the Phy/Mac is initialized
	pNwStore->m_MiiSm.State			= MIISM_STATE_INITIALIZING; 
	pNwStore->m_MiiSm.SubState		= MIISM_SUBSTATE_NO_ACTION; 
	pNwStore->m_MiiSm.Command		= 0;

	pNwStore->m_MacSpeed  = 4;		// Not set
	pNwStore->m_MacDuplex = 2;		// Not set
	pNwStore->m_MacPacketFilter = 0xFFFF0000;  // Not set
	pNwStore->m_NwiLink  = 2;		// Not set
	
	InitializeDpc(&pNwStore->m_MacDpc,
				  (PKDEFERRED_ROUTINE) Dta1xxNwMacHandleLinkAutoNegChangeDpc,
				  pNwStore);

	InitializeDpc(&pNwStore->m_SetupFrameDpc,
				  (PKDEFERRED_ROUTINE) Dta1xxNwMacSendSetupFrameDpc,
				  pNwStore);

	InitializeDpc(&pNwStore->m_LinkStatDPC,
				  (PKDEFERRED_ROUTINE) Dta1xxNwIndicateLinkStatusDpc,
				  pNwStore);

	InitializeMutex(&pNwStore->m_SetupFrameMutex);
	InitializeMutex(&pNwStore->m_MacMutex);
	ExInitializeFastMutex(&pNwStore->m_FastMutexCounters);

	// Initialise status / counter retrieval
	pNwStore->m_StatCntpIrp = NULL;
	pNwStore->m_StatCntPending = FALSE;
	InitializeDpc(&pNwStore->m_StatCntDPC,
				  (PKDEFERRED_ROUTINE) Dta1xxNwStatCntCompletedDpc,
				  pNwStore);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacInitPhy -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Initialise the PHY. Set the clock division of the PHY. After finished, the interupt
// mask is set and the link status is read.
//
void  Dta1xxNwMacInitPhy(IN PNwStore pNwStore)
{
	// Set clock division
	Dta1xxNwMacMiiSmSetClockDivision(pNwStore);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSmRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read from the PHY using the MII Serial Management registers. 
// RegisterAddress = Address of PHY register to read
//
void  Dta1xxNwMacMiiSmRead(UInt8* pBase, UInt8 RegisterAddress)
{
	UInt32  MiiCommand;
#if LOG_LEVEL_MAC > 1
	DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacMiiSmRead: RegisterAddress=0x%x", RegisterAddress);
#endif

	MiiCommand = DTA1XX_ETH_MIISM_START_BUSY_MASK | 
				 DTA1XX_ETH_MIISM_OPCODE_READ |
				 DTA1XX_ETH_MIISM_CLKDIV_32   |
				 DTA1XX_ETH_MIISM_PHYADDR;
	MiiCommand |= (RegisterAddress << DTA1XX_ETH_MIISM_REG_ADDRESS_SHIFT)
									& DTA1XX_ETH_MIISM_REG_ADDRESS_MASK;

    Dta1xxEthMiiSmRegSet(pBase, MiiCommand);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacGenerateSetupFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  Dta1xxNwMacGenerateSetupFrame(
	IN PNwStore pNwStore, 
	IN Dta1xxSetupFrame* pSetupFrame)
{
	UInt	i;
	UInt16	HTIndex;
	UInt8	BCMacAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	
	// Fill the setup frame content
	RtlZeroMemory(pSetupFrame, sizeof(Dta1xxSetupFrame));
	pSetupFrame->Header.Tag = 0x445441A0;
	pSetupFrame->Header.Length = sizeof(Dta1xxDmaTxHeader);
	pSetupFrame->Header.Version = 1;
	pSetupFrame->Header.TransmitControl.PacketLength = sizeof(Dta1xxSetupFrame) - 
													   sizeof(Dta1xxDmaTxHeader);
	// It's a setup frame
	pSetupFrame->Header.TransmitControl.SetupFrame = 1;

	// Only 16 items can be stored with perfect filtering
	// 1 for own mac-address
	// 1 for broadcast address
	// 14 for multicast addresses
	if (pNwStore->m_NumMulticast <= 14) {	
		// Perfect filtering mode: Setup frame buffer is interpreted
		// as a set of 16 48-bit physical addresses
		pSetupFrame->Header.TransmitControl.FilteringType1 = (DTA1XX_FT_PERFECT >> 1) & 1;
		pSetupFrame->Header.TransmitControl.FilteringType0 = (DTA1XX_FT_PERFECT     ) & 1;

		// Own mac address
		pSetupFrame->FT.PFAddress[0].PhAddr0 = 
										((UInt16)pNwStore->m_MacAddrCur[1] << 8)
										+ pNwStore->m_MacAddrCur[0];
		pSetupFrame->FT.PFAddress[0].PhAddr1 = 
										((UInt16)pNwStore->m_MacAddrCur[3] << 8)
										+ pNwStore->m_MacAddrCur[2];
		pSetupFrame->FT.PFAddress[0].PhAddr2 = 
										((UInt16)pNwStore->m_MacAddrCur[5] << 8)
										+ pNwStore->m_MacAddrCur[4];
		
		// Broadcast address
		pSetupFrame->FT.PFAddress[1].PhAddr0 = 0xffff;	
		pSetupFrame->FT.PFAddress[1].PhAddr1 = 0xffff;	
		pSetupFrame->FT.PFAddress[1].PhAddr2 = 0xffff;

		// Multicast addresses
		for (i = 0; i < pNwStore->m_NumMulticast; i++) {
			pSetupFrame->FT.PFAddress[i+2].PhAddr0 = 
										((UInt16)pNwStore->m_MCList[i][1] << 8)
										+ pNwStore->m_MCList[i][0];
			pSetupFrame->FT.PFAddress[i+2].PhAddr1 = 
										((UInt16)pNwStore->m_MCList[i][3] << 8)
										+ pNwStore->m_MCList[i][2];
			pSetupFrame->FT.PFAddress[i+2].PhAddr2 = 
										((UInt16)pNwStore->m_MCList[i][5] << 8)
										+ pNwStore->m_MCList[i][4];
#if LOG_LEVEL_MAC > 0
			DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacSendSetupFrame: ML[%02d]"
					 "=%02x%02x%02x%02x%02x%02x", i,
					 pNwStore->m_MCList[i][0], pNwStore->m_MCList[i][1],
					 pNwStore->m_MCList[i][2], pNwStore->m_MCList[i][3],
					 pNwStore->m_MCList[i][4], pNwStore->m_MCList[i][5]);
#endif
		}
	} else {
		// Hash table implementation
		pSetupFrame->Header.TransmitControl.FilteringType1 = (DTA1XX_FT_HASH >> 1) & 1;
		pSetupFrame->Header.TransmitControl.FilteringType0 = (DTA1XX_FT_HASH     ) & 1;

		// Own mac address
		pSetupFrame->FT.HTable.PFAddress.PhAddr0 = 
										((UInt16)pNwStore->m_MacAddrCur[1] << 8)
										+ pNwStore->m_MacAddrCur[0];
		pSetupFrame->FT.HTable.PFAddress.PhAddr1 = 
										((UInt16)pNwStore->m_MacAddrCur[3] << 8)
										+ pNwStore->m_MacAddrCur[2];
		pSetupFrame->FT.HTable.PFAddress.PhAddr2 = 
										((UInt16)pNwStore->m_MacAddrCur[5] << 8)
										+ pNwStore->m_MacAddrCur[4];
		
		// Broadcast address, get 9 LSB from CRC over destination MAC
		HTIndex = (UInt16)(Dta1xxNwMacCalcCrc(BCMacAddr, sizeof(BCMacAddr)) & 0x1FF);
		pSetupFrame->FT.HTable.HFItem[HTIndex / 16].Item0 |= 1 << (HTIndex % 16);

		// Multicast addresses
		for (i = 0; i < pNwStore->m_NumMulticast; i++)
		{
			// Get 9 LSB from CRC over destination MAC
			// pNwStore->m_MCList[i] points to an array of 6 bytes containing the MC MAC
			HTIndex = (UInt16)(Dta1xxNwMacCalcCrc(pNwStore->m_MCList[i], 6) & 0x1FF);
			pSetupFrame->FT.HTable.HFItem[HTIndex / 16].Item0 |= 1 << (HTIndex % 16);

#if LOG_LEVEL_MAC > 0
			KdPrint(("Dta1xx: Dta1xxNwMacSendSetupFrame: MC[%02d]"
					 "=%02x-%02x-%02x-%02x-%02x-%02x, hash %02d\n", i,
					 pNwStore->m_MCList[i][0], pNwStore->m_MCList[i][1],
					 pNwStore->m_MCList[i][2], pNwStore->m_MCList[i][3],
					 pNwStore->m_MCList[i][4], pNwStore->m_MCList[i][5],
					 HTIndex));
#endif
		}
	}
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacSendSetupFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Pre: DMA channel is enabled, MAC is read from VPD data
//
#ifdef WINBUILD
NTSTATUS  Dta1xxNwMacSendSetupFrame(IN PNwStore pNwStore)
{
	PIRP				pIrp;
	PMDL				pMdl;
	KEVENT				KernelEvent;
	NTSTATUS			Status;
	PIO_STACK_LOCATION  pStack;
	Dta1xxSetupFrame*	pSetupFrame;
	IO_STATUS_BLOCK		IOStatus;
	
	ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);
	ASSERT(pNwStore->m_IpPortIndex < pNwStore->m_pFdo->m_NumIpPorts);
#if LOG_LEVEL_MAC > 0
	DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacSendSetupFrame: Send MAC Setup frame "
			 "MAC Address=%02x%02x%02x%02x%02x%02x",
			 pNwStore->m_MacAddrCur[0], pNwStore->m_MacAddrCur[1],
			 pNwStore->m_MacAddrCur[2], pNwStore->m_MacAddrCur[3],
			 pNwStore->m_MacAddrCur[4], pNwStore->m_MacAddrCur[5]);
#endif

	// Initialize kernel event
	KeInitializeEvent(&KernelEvent, NotificationEvent, FALSE);

	// Allocate IRP
	// We need 2 stack locations. 1 for the completion routine with data content and
	// one spare. The spare one is needed because IoMarkIrpPending is executed in the
	// completion routine. (IoMarkIrpPending is not needed for this thread).
	// This needs an extra stack location.

	pIrp = IoAllocateIrp(2, FALSE);
	if (pIrp == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	// Create SetupFrame buffer
	pSetupFrame = (Dta1xxSetupFrame*) ExAllocatePoolWithTag(NonPagedPool, 
												sizeof(Dta1xxSetupFrame), '1ATD');
	if (!pSetupFrame) {	
		IoFreeIrp(pIrp);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	Dta1xxNwMacGenerateSetupFrame(pNwStore, pSetupFrame);
		
	// Build the Irp
    IoSetNextIrpStackLocation(pIrp);
	IoSetCompletionRoutine(pIrp, (PIO_COMPLETION_ROUTINE) Dta1xxCompletionRoutine,
						(PVOID) &KernelEvent, TRUE, TRUE, TRUE);

	IoSetNextIrpStackLocation(pIrp);

	pMdl = IoAllocateMdl(pSetupFrame, sizeof(Dta1xxSetupFrame), FALSE, TRUE, pIrp);
	MmBuildMdlForNonPagedPool(pMdl);
    	
	pStack = IoGetCurrentIrpStackLocation(pIrp);

	pIrp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	pStack->Parameters.DeviceIoControl.IoControlCode = IOCTL_DTA1XX_DMA_USERMEM_WRITE;

	// Store the packet in the Non DVB transmit queue
	Status = Dta1xxDvcQueueInsertIrp(pNwStore->m_pFdo->m_NwTx[pNwStore->m_IpPortIndex],
									 pIrp);
	
	if (Status == STATUS_PENDING) {		
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacSendSetupFrame: Pending");
#endif
	
		// Wait for completion
		KeWaitForSingleObject(&KernelEvent, Executive, KernelMode, FALSE, NULL);
		Status = pIrp->IoStatus.Status;
	}
#if LOG_LEVEL_MAC > 0
	DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacSendSetupFrame: Finished");
#endif
	
	IoFreeMdl(pMdl);

	ExFreePool(pSetupFrame);

	// Release the IRP
	IoFreeIrp(pIrp);

	return STATUS_SUCCESS;	
}
#else
NTSTATUS  Dta1xxNwMacSendSetupFrame(IN PNwStore pNwStore)
{
	Dta1xxSetupFrame*	pSetupFrame;
#if LOG_LEVEL_MAC > 0	
	DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacSendSetupFrame: Send MAC Setup frame "
			 "MAC Address=%02x%02x%02x%02x%02x%02x",
			 pNwStore->m_MacAddrCur[0], pNwStore->m_MacAddrCur[1],
			 pNwStore->m_MacAddrCur[2], pNwStore->m_MacAddrCur[3],
			 pNwStore->m_MacAddrCur[4], pNwStore->m_MacAddrCur[5]);
#endif
	// Create SetupFrame buffer
	pSetupFrame = (Dta1xxSetupFrame*) kmalloc(sizeof(Dta1xxSetupFrame), GFP_KERNEL);
	if (!pSetupFrame) {	
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	Dta1xxNwMacGenerateSetupFrame(pNwStore, pSetupFrame);
	
	Dta1xxDmaKernelMemWrite(pNwStore->m_pFdo->m_NwTx[pNwStore->m_IpPortIndex], (UInt8*)pSetupFrame, 
							sizeof(Dta1xxSetupFrame));

	kfree(pSetupFrame);

#if LOG_LEVEL_MAC > 0	
	DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacSendSetupFrame: Finished");
#endif
	return STATUS_SUCCESS;
	
}
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacSetIntMask -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set the PHY interrupt mask to interrupt during link, duplex and speed change.
//
void  Dta1xxNwMacSetIntMask(IN PNwStore pNwStore)
{
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;

	if (pNwStore->m_PhyType == PHY_NATIONAL) {
		Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_NAT_INT_MASK,
													DTA1XX_ETH_NW_NAT_INT_MASK_BITS);
	} else {
		Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_MAR_INT_MASK,
													DTA1XX_ETH_NW_MAR_INT_MASK_BITS);

	}
	pNwStore->m_MiiSm.Command = MIISM_CMD_SET_INT_MASK;
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSmSetPhyControlInit -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set the BMCR register of the PHY.
// Enable phy and set speed/duplex
// Used during initializing process
// SubState = 0-->Initial, 1-->Second time
//
void  Dta1xxNwMacMiiSmSetPhyControlInit(IN PNwStore pNwStore, IN UInt8 SubState)
{
	UInt16 Data = DTA1XX_ETH_BMCR_AUTO_NEG_EN_MASK;
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;


	switch (pNwStore->m_NwiSpeed) {

	case 0:	// Auto negotiate
	    Data = DTA1XX_ETH_BMCR_AUTO_NEG_EN_MASK;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControlInit: Auto negotiation");
#endif
		break;

	case 1:	// 10MB-half 
		Data = 0;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControlInit: 10Mb-Half");
#endif
		break;

	case 2:	// 10MB-full
		Data = DTA1XX_ETH_BMCR_DUPLEX_MASK;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControlInit: 10Mb-Full");
#endif
		break;
	
	case 3:	// 100MB-half
		Data = DTA1XX_ETH_BMCR_SPEED0_MASK;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControlInit: 100Mb-Half");
#endif
		break;

	case 4:	// 100MB-full
		Data = DTA1XX_ETH_BMCR_SPEED0_MASK | 
				DTA1XX_ETH_BMCR_DUPLEX_MASK;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControlInit: 100Mb-Full");
#endif
		break;

	case 255: // Power down
		Data = DTA1XX_ETH_BMCR_POWER_DOWN_MASK;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControlInit: Disable network phy");
#endif
		break;
	}

	// For the Marvell Phy, speed is only updated after a software reset.
	// After the software reset, some registers will be initialised to 0
	// (loopback + powerdown)
	// If loopback is enabled, this bit must be updated after the software reset
	if ((pNwStore->m_PhyType == PHY_MARVELL) && (pNwStore->m_NwiSpeed != 255)) {
		Data = Data | DTA1XX_ETH_BMCR_SWRESET_MASK;
	}

	Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_BMCR, Data);

	pNwStore->m_MiiSm.Command  = MIISM_CMD_SET_PHY_CONTROL_INIT;
	pNwStore->m_MiiSm.State    = MIISM_STATE_WRITE_WAIT_RDY;
	pNwStore->m_MiiSm.SubState = SubState;

}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacDisablePhy -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set Phy in power down mode
//
void Dta1xxNwMacDisablePhy(IN PNwStore pNwStore)
{
	pNwStore->m_NwiSpeed = 255;	//Power down mode
	pNwStore->m_SpeedPending = TRUE;
	Dta1xxNwMacMiiSmSetPhyControl(pNwStore, 0);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSmSetPhySpecificInit -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Initialise Phy-specific registers
// Marvell: LED-Control
// National: AUX_CTRL to force GMII mode (rev.2 board)
//
void Dta1xxNwMacMiiSmSetPhySpecificInit(IN PNwStore pNwStore)
{
	UInt16 Data;
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;

	if (pNwStore->m_PhyType == PHY_NATIONAL) {
		// Force GMII-mode
		Data = 0x8000;	// Auto-MDIX enable (and GMII-mode)
		Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, 
							  DTA1XX_ETH_MIISM_ADDRESS_NAT_AUX_CTRL, Data);	
	} else if ((pNwStore->m_PhyType == PHY_MARVELL) && (pNwStore->m_PhyModel == 0x28)) {
		// First set the power down to off
		Dta1xxNwMacMiiSetCopperSpecControlRegister(pNwStore);
		/// When finished, set LED Function,LED Polarity and LED Timer control...
		// All these are in page 3, so we go to page 3 register in the state machine
		return;
	} else {
		// Set LED control
		Data = 0x1140;	// 21ms to 42ms Pulse stretch, 84ms blink rate,
						// transmit or receive activity
		Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, 
							  DTA1XX_ETH_MIISM_ADDRESS_MAR_LED_CTRL, Data);
	}

	pNwStore->m_MiiSm.Command = MIISM_CMD_SET_PHY_SPECIFIC_INT;
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSetPageRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxNwMacMiiSetPageRegister(IN PNwStore pNwStore, UInt16 Page, UInt8 Cmd, UInt8 State)
{
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;
	
	// Set new page address
	Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, 
							  DTA1XX_ETH_MIISM_ADDRESS_MAR_PAGE, Page);
	
	// Update next state
	pNwStore->m_MiiSm.Command = Cmd;
	pNwStore->m_MiiSm.State   = State;

}

//.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSetCopperSpecControlRegister -.-.-.-.-.-.-.-.-.-.-.
//
void Dta1xxNwMacMiiSetCopperSpecControlRegister(IN PNwStore pNwStore)
{	UInt16 Data = 0x6060;
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;
	Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, 
						  DTA1XX_ETH_MIISM_ADDRESS_MAR_COP_SPEC_CTRL1, Data);
	pNwStore->m_MiiSm.Command = MIISM_CMD_SET_COP_SPEC_CTRL;
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;	
}

//-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiProgramLedFunctionControl -.-.-.-.-.-.-.-.-.-.-.-.
//
void Dta1xxNwMacMiiProgramLedFunctionControl(IN PNwStore pNwStore)
{	
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;
	// Interrupt(2), on activity,(3) on link(0)
	Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, 
							  DTA1XX_ETH_MIISM_ADDRESS_MAR_LED_FUNC_CTRL, 0x230);

	pNwStore->m_MiiSm.Command = MIISM_CMD_SET_PHY_LED_FUNC_CTRL;
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;
}

//-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiProgramLedPolarityControl -.-.-.-.-.-.-.-.-.-.-.-.
//
void Dta1xxNwMacMiiProgramLedPolarityControl(IN PNwStore pNwStore)
{	
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;
	// 
	Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, 
							  DTA1XX_ETH_MIISM_ADDRESS_MAR_LED_POL_CTRL, 0x25);


	pNwStore->m_MiiSm.Command = MIISM_CMD_SET_PHY_LED_POL_CTRL;
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiProgramLedTimerControl -.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxNwMacMiiProgramLedTimerControl(IN PNwStore pNwStore)
{	UInt16 Data;
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;
	Data = 0x1140;	// 21ms to 42ms Pulse stretch, 84ms blink rate,
						// transmit or receive activity
	Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, 
							  DTA1XX_ETH_MIISM_ADDRESS_MAR_LED_TIM_CTRL, Data);


	pNwStore->m_MiiSm.Command = MIISM_CMD_SET_PHY_LED_TIM_CTRL;
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSmSetPhyControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set the BMCR register of the PHY.
// Enable phy and set speed/duplex
// For Marvell, Loopback mode must be enabed after the link down speed is set.
// EnableLoopbackNow = 1 only for Marvell Phy after a software reset
//
void  Dta1xxNwMacMiiSmSetPhyControl(IN PNwStore pNwStore, IN UInt EnableLoopbackNow)
{
	UInt16 Data = 0;
	UInt Speed = pNwStore->m_ForceSpeedEnable?pNwStore->m_ForcedSpeed:pNwStore->m_NwiSpeed;

	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;

	switch (Speed) {

	case 0:	// Auto negotiate
	    Data = DTA1XX_ETH_BMCR_AUTO_NEG_EN_MASK |
			   DTA1XX_ETH_BMCR_AUTO_NEG_RST_MASK;	// Restart Auto-Negotiation
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControl: Auto negotiation");
#endif
		break;

	case 1:	// 10MB-half 
		Data = 0;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControl: 10Mb-Half");
#endif
		break;

	case 2:	// 10MB-full
		Data = DTA1XX_ETH_BMCR_DUPLEX_MASK;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControl: 10Mb-Full");
#endif
		break;
	
	case 3:	// 100MB-half
		Data = DTA1XX_ETH_BMCR_SPEED0_MASK;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControl: 100Mb-Half");
#endif
		break;

	case 4:	// 100MB-full
		Data = DTA1XX_ETH_BMCR_SPEED0_MASK | 
				DTA1XX_ETH_BMCR_DUPLEX_MASK;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControl: 100Mb-Full");
#endif
		break;

	case 5: // 1GB-Master
		Data = DTA1XX_ETH_BMCR_SPEED1_MASK |
				DTA1XX_ETH_BMCR_DUPLEX_MASK;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControl: 1GB-Full Master");
#endif
		break;

	case 6: // 1GB-Slave
		Data = DTA1XX_ETH_BMCR_SPEED1_MASK |
				DTA1XX_ETH_BMCR_DUPLEX_MASK;
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControl: 1GB-Full Slave");
#endif
		break;

	case 255: // Power-down
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControl: Disable network phy");
#endif
		Data = DTA1XX_ETH_BMCR_POWER_DOWN_MASK;
		break;
	
	default:
		ASSERT(FALSE);
	}
	

	// For Marvell Phy, the loopback is set after the speed has been set
	if (pNwStore->m_LoopbackEnable && 
		 ((pNwStore->m_PhyType == PHY_NATIONAL) || EnableLoopbackNow)) {
		Data = Data | DTA1XX_ETH_BMCR_LOOPBACK_MASK;
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetPhyControl: Loopback enabled");
	}

	// For the Marvell Phy, speed is only updated after a software reset.
	// After the software reset, some registers will be initialised to 0
	// (loopback + powerdown)
	// If loopback is enabled, the loopback must be updated after the software reset
	if ((pNwStore->m_PhyType == PHY_MARVELL) && (Speed != 255) && (!EnableLoopbackNow)) {
		Data = Data | DTA1XX_ETH_BMCR_SWRESET_MASK;
	}

	Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_BMCR, Data);
	if (EnableLoopbackNow) {
		pNwStore->m_MiiSm.Command = MIISM_CMD_SET_PHY_CONTROL2;
	} else {
		pNwStore->m_MiiSm.Command = MIISM_CMD_SET_PHY_CONTROL;
	}
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSmSetExtPhyControl -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set the extended phy control register for loopback speed setting
// Only for Marvell Phy 88E1111
//
void  Dta1xxNwMacMiiSmSetExtPhyControl(IN PNwStore pNwStore)
{
	UInt16 Data;
	UInt Speed = pNwStore->m_ForceSpeedEnable?pNwStore->m_ForcedSpeed:pNwStore->m_NwiSpeed;
	
	Data = 0x0c00; // Downshift counter = 0x6 (HW reset value)
	Data = Data | 0x4 << 4; // Enable TX_CLK speed-1000BASE-T link
							// If not enabled, MAC can not stop the transmitter
							// when speed must be changed.

	if (Speed == 0) { // Autonegotiate
		// Set speed to 1Gb, auto negotiation is not working in loopback mode.
		Speed = 6;
	}

	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;

	switch (Speed) {

	case 1:	// 10MB-half 
	case 2:	// 10MB-full
		Data = Data | (0 << 4);
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetExtPhyControl: 10Mb-Full");
#endif
		break;
	
	case 3:	// 100MB-half
	case 4:	// 100MB-full
		Data = Data | (1 << 4);
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetExtPhyControl: 100Mb-Full");
#endif
		break;

	case 5: // 1GB-Master
		Data = Data | (3 << 4); 
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetExtPhyControl: 1GB-Full Master");
#endif
		break;

	case 6: // 1GB-Slave
		Data = Data | (3 << 4); 
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetExtPhyControl: 1GB-Full Slave");
#endif
		break;
	}

	Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_MAR_EXT_PHY_CTRL, Data);
	pNwStore->m_MiiSm.Command = MIISM_CMD_SET_PHY_EXTCONTROL;
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSmSetLoopbackSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set the MAC specific control register for loopback speed setting
// Only for Marvell Phy 88E1119R
//
void  Dta1xxNwMacMiiSmSetLoopbackSpeed(IN PNwStore pNwStore)
{
	UInt16 Data;
	UInt Speed = pNwStore->m_ForceSpeedEnable?pNwStore->m_ForcedSpeed:pNwStore->m_NwiSpeed;
	Data = 0x1040; // (HW reset value)
	// Enable TX_CLK speed-1000BASE-T link
	// If not enabled, MAC can not stop the transmitter
	// when speed must be changed.

	if (Speed == 0) { // Autonegotiate
		// Set speed to 1Gb, auto negotiation is not working in loopback mode.
		Speed = 6;
	}

	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;

	switch (Speed) {

	case 1:	// 10MB-half 
	case 2:	// 10MB-full
		Data = Data | (0x4);
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetLoopbackSpeed: 10Mb-Full");
#endif
		break;
	
	case 3:	// 100MB-half
	case 4:	// 100MB-full
		Data = Data | (0x5);
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetLoopbackSpeed: 100Mb-Full");
#endif
		break;

	case 5: // 1GB-Master
		Data = Data | (0x6); 
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetLoopbackSpeed: 1GB-Full Master");
#endif
		break;

	case 6: // 1GB-Slave
		Data = Data | (0x7); 
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacMiiSmSetLoopbackSpeed: 1GB-Full Slave");
#endif
		break;
	}

	Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_MAR_MAC_SPEC_CTRL, Data);
	pNwStore->m_MiiSm.Command = MIISM_CMD_SET_LOOPBACKSPEED;
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSmSet1GBControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set the 1KTCR register of the PHY.
// Enable/disable manual master/slave mode
//
void  Dta1xxNwMacMiiSmSet1GBControl(IN PNwStore pNwStore)
{
	UInt16 Data;

	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;

	// Disable manual master/slave mode and set advertise capability
	Data = DTA1XX_ETH_1KTCR_MSCFG_CAP_MASK;

	// Nw driver can not set the speed to forced 1GB.
    if (pNwStore->m_ForceSpeedEnable) {
		if (pNwStore->m_ForcedSpeed == DTA1XX_NWSPEED_1GB_MASTER) {
			// Set 1GB mode to master
			Data |= DTA1XX_ETH_1KTCR_MSCFG_EN_MASK;
			Data |= DTA1XX_ETH_1KTCR_MSCFG_MASTER_MASK;
#if LOG_LEVEL_MAC > 0
			DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacSet1GBControl: Enable manual 1GB master mode");
#endif
		} else if (pNwStore->m_ForcedSpeed == DTA1XX_NWSPEED_1GB_SLAVE) {
			// Set 1GB mode to slave
			Data |= DTA1XX_ETH_1KTCR_MSCFG_EN_MASK;
#if LOG_LEVEL_MAC > 0
			DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacSet1GBControl: Enable manual 1GB slave mode");
#endif
		} else {
#if LOG_LEVEL_MAC > 0
			DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacSet1GBControl: Disable manual 1GB master/slave mode");
#endif
		}
	} else {
#if LOG_LEVEL_MAC > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwMacSet1GBControl: Disable manual 1GB master/slave mode");
#endif
	}
	
	Dta1xxNwMacMiiSmWrite(pNwStore->m_pRegBase, DTA1XX_ETH_MIISM_ADDRESS_1KTCR, Data);

	pNwStore->m_MiiSm.Command = MIISM_CMD_SET_1GB_CTRL;
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacSetMiiSmClockDivision -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set the clock division of the PHY.
//
void  Dta1xxNwMacMiiSmSetClockDivision(IN PNwStore pNwStore)
{
	UInt32 MiiCommand;
	pNwStore->m_MiiSm.State = MIISM_STATE_PREPARING;

	MiiCommand  = DTA1XX_ETH_MIISM_START_BUSY_MASK | 
				  DTA1XX_ETH_MIISM_OPCODE_CLKDIV |
			      DTA1XX_ETH_MIISM_PHYADDR |
				  DTA1XX_ETH_MIISM_CLKDIV_32;
	
	Dta1xxEthMiiSmRegSet(pNwStore->m_pRegBase, MiiCommand);
	
	pNwStore->m_MiiSm.Command = MIISM_CMD_SET_CLK_DIVIDER;
	pNwStore->m_MiiSm.State   = MIISM_STATE_WRITE_WAIT_RDY;
	
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacMiiSmWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write to the PHY using the MII Serial Management registers. 
// RegisterAddress = Address of PHY register to write to
// Data = Data to write
//
void Dta1xxNwMacMiiSmWrite(UInt8* pBase, UInt8 RegisterAddress, UInt16 Data)
{
	UInt32 MiiCommand;
#if LOG_LEVEL_MAC > 1
	DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwMacMiiSmWrite: RegisterAddress=0x%x", RegisterAddress);
#endif

	MiiCommand = DTA1XX_ETH_MIISM_START_BUSY_MASK | 
				 DTA1XX_ETH_MIISM_OPCODE_WRITE |
				 DTA1XX_ETH_MIISM_CLKDIV_32    |
				 DTA1XX_ETH_MIISM_PHYADDR;
	MiiCommand |= (RegisterAddress << DTA1XX_ETH_MIISM_REG_ADDRESS_SHIFT)
									& DTA1XX_ETH_MIISM_REG_ADDRESS_MASK;
	MiiCommand |= (Data & DTA1XX_ETH_MIISM_DATA_MASK); 
	
    Dta1xxEthMiiSmRegSet(pBase, MiiCommand);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwMacGetMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS  Dta1xxNwMacGetMacAddress(
	IN PNwStore pNwStore,
	DTA1XX_MACADDRESS*  pMacAddress)
{	int i;
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

	for (i=0; i<6; i++) {
		pMacAddress->m_MacAddress[i] = pNwStore->m_MacAddrCur[i];
	}

#if LOG_LEVEL_MAC > 1
	DTA1XX_LOG(KERN_INFO, "[Ip%d] Dta1xxNwMacGetMacAddress: "
			 "MAC Address=%02x%02x%02x%02x%02x%02x",
			 pNwStore->m_IpPortIndex,
			 pNwStore->m_MacAddrCur[0], pNwStore->m_MacAddrCur[1],
			 pNwStore->m_MacAddrCur[2], pNwStore->m_MacAddrCur[3],
			 pNwStore->m_MacAddrCur[4], pNwStore->m_MacAddrCur[5]);
#endif
	return ReturnStatus;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwSetLoopback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS  Dta1xxNwSetLoopback(
	IN PNwStore pNwStore,
	DTA1XX_NWLOOPBACK*  pNwLoopback)
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success
	
	// Update loopback enable status
	if (pNwLoopback->m_LoopbackEnable != pNwStore->m_LoopbackEnable) {
		pNwStore->m_LoopbackEnable = pNwLoopback->m_LoopbackEnable;
		
		// To do: update mac prom. status if needed!
		pNwStore->m_SpeedPending = TRUE;
	}

	return ReturnStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwSetSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxNwSetSpeed(
	IN PNwStore pNwStore,
	DTA1XX_NWSPEED*  pNwSpeed)
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success
	
	if ((pNwStore->m_ForcedSpeed != pNwSpeed->m_Speed) || 
		(pNwSpeed->m_ForceSpeedEnable != pNwStore->m_ForceSpeedEnable)){
		pNwStore->m_ForcedSpeed = pNwSpeed->m_Speed;
		pNwStore->m_ForceSpeedEnable = pNwSpeed->m_ForceSpeedEnable;
		
		pNwStore->m_SpeedPending = TRUE;
	}

	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwStatCntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#ifdef WINBUILD
void  Dta1xxNwStatCntCompletedDpc(
	IN PKDPC  Dpc,					// Supplies the DPC object
	IN PVOID  DeferredContext,		// Pointer to NwStore
									// Passed when KeInitializeDpc was called
	IN PVOID  SystemArgument1,		// Value: returned status / counter value
	IN PVOID  SystemArgument2)		// Context2))
{
	PNwStore pNwStore = (PNwStore)DeferredContext;
	UInt32 Value = pNwStore->m_StatCntValue;;
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pNwStore->m_StatCntpIrp);
	DTA1XX_IOCTL_DATA* pIoctlData =
			(DTA1XX_IOCTL_DATA*) pNwStore->m_StatCntpIrp->AssociatedIrp.SystemBuffer;
	
	IoReleaseRemoveLock(&pNwStore->m_pFdo->m_RemoveLock, pNwStore->m_StatCntpIrp);
	
	
	if (pNwStore->m_StatCntMode == 0) {	// Get
		pIoctlData->m_NwStatCnt.m_Value = Value;
		Dta1xxCompleteRequest(pNwStore->m_StatCntpIrp, STATUS_SUCCESS, sizeof(DTA1XX_NWSTATCNT));
	
	}  else {	// Set
		Dta1xxCompleteRequest(pNwStore->m_StatCntpIrp, STATUS_SUCCESS, 0);
	}

	InterlockedExchangePointer(&pNwStore->m_StatCntpIrp, NULL);
}
#else
void  Dta1xxNwStatCntCompletedDpc(
	IN unsigned long DeferredContext)		// Pointer to NwStore
											// Passed when KeInitializeDpc was called
{
	DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwStatCntCompletedDpc: Not implemented yet.");
}
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwGetSetStatCnt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
NTSTATUS Dta1xxNwGetSetStatCnt(
	IN PNwStore pNwStore, 
	IN PIRP pIrp,
	IN DTA1XX_NWSTATCNT* pNwStatCnt,
	IN UInt8 Mode)
{
	NTSTATUS ReturnStatus = STATUS_SUCCESS;

	// Check if status / counter can be retrieved without pending
	if (pNwStatCnt->m_Offset < 0x20) {
#ifdef WINBUILD
		// We need to use the MiiSm protocol. So we need to pend.
		if (InterlockedCompareExchangePointer(&pNwStore->m_StatCntpIrp, pIrp, NULL) == NULL) {
			IoMarkIrpPending(pIrp);
			pNwStore->m_StatCntpIrp = pIrp;
			pNwStore->m_StatCntMode = Mode;
			pNwStore->m_StatCntPending = TRUE;
			ReturnStatus = STATUS_PENDING;
		} else {
			ReturnStatus = STATUS_UNSUCCESSFUL;
			DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwGetSetStatCnt: already busy.");
		}
#else 
		ReturnStatus = STATUS_UNSUCCESSFUL;
		DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwGetSetStatCnt: Offset < 0x20 Not implemented yet.");
#endif
	} else {

		// Counters without pending

		if (Mode == 0) {
			// Get
			pIrp->IoStatus.Information = sizeof(DTA1XX_NWSTATCNT);
			pNwStatCnt->m_Value = 0;
			pNwStatCnt->m_ValueH = 0;
		}

		switch(pNwStatCnt->m_Offset) {
			case DTA1XX_NWSTATCNT_RXFIFOOVERFL:
				if (Mode == 0)
					pNwStatCnt->m_Value = pNwStore->m_NumRxFifoOverflows;
				else
					pNwStore->m_NumRxFifoOverflows = 0; //pNwStatCnt->m_Value
				break;
			case DTA1XX_NWSTATCNT_ADMINSTATUS:
				if (Mode == 0)
					pNwStatCnt->m_Value = pNwStore->m_AdminStatus;
				else
					pNwStore->m_AdminStatus = (UInt8)pNwStatCnt->m_Value;
				break;
			default:
				ReturnStatus = STATUS_UNSUCCESSFUL;
				pIrp->IoStatus.Information = 0;
		}
	}
	return ReturnStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwGetSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxNwGetSpeed(
	IN PNwStore pNwStore,
	DTA1XX_NWSPEED*  pNwSpeed)
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success
	
	pNwSpeed->m_ForceSpeedEnable = pNwStore->m_ForceSpeedEnable;

	if (pNwStore->m_PhyLink != DTA1XX_ETH_NW_LINK_UP) {
		pNwSpeed->m_Speed = DTA1XX_NWSPEED_NOLINK;
	} else if (pNwStore->m_PhySpeed == DTA1XX_ETH_NW_SPEED_1000) {
		if (pNwStore->m_PhyMaster) {
			pNwSpeed->m_Speed = DTA1XX_NWSPEED_1GB_MASTER;
		} else {
			pNwSpeed->m_Speed = DTA1XX_NWSPEED_1GB_SLAVE;
		}
	} else {
		if (pNwStore->m_PhyDuplex == DTA1XX_ETH_NW_DUPLEX_FULL) {
			// Full duplex
			if (pNwStore->m_PhySpeed == DTA1XX_ETH_NW_SPEED_100) {
				pNwSpeed->m_Speed = DTA1XX_NWSPEED_100MB_FULL;
			} else {
				pNwSpeed->m_Speed = DTA1XX_NWSPEED_10MB_FULL;
			}

		} else {
			// Half duplex
			if (pNwStore->m_PhySpeed == DTA1XX_ETH_NW_SPEED_100) {
				pNwSpeed->m_Speed = DTA1XX_NWSPEED_100MB_HALF;
			} else {
				pNwSpeed->m_Speed = DTA1XX_NWSPEED_10MB_HALF;
			}
		}
	}

	return ReturnStatus;
}
