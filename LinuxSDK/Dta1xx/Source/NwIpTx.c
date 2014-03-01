//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NwIpTx.c *#*#*#*#*#*#*#*#*# (C) 2005-2006 DekTec
//
// For NETWORK cards: IP Transmit routines
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	2005.09.01	RD	Created	


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"
#include "../Include/EthPrtcls.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
NTSTATUS  Dta1xxNwIpTxAttach(IN PDTA1XX_FDO, IN Int, IN PFILEOBJECT,
							 IN DTA1XX_IPTX_ATTACH_CTRL*, OUT DTA1XX_IPTX_ATTACH_RTRN*);
NTSTATUS  Dta1xxNwIpTxClearFlags(IN PDTA1XX_FDO, IN UserIpTxChannel*,
								 IN DTA1XX_IPTX_FLAGS_CTRL*, OUT DTA1XX_IPTX_FLAGS_RTRN*);
NTSTATUS  Dta1xxNwIpTxDetach(IN PDTA1XX_FDO, IN UserIpTxChannel*,
							 IN DTA1XX_IPTX_DETACH_CTRL*, OUT DTA1XX_IPTX_DETACH_RTRN*);
NTSTATUS  Dta1xxNwIpTxGetFlags(IN PDTA1XX_FDO, IN UserIpTxChannel*,
							   IN DTA1XX_IPTX_FLAGS_CTRL*, OUT DTA1XX_IPTX_FLAGS_RTRN*);
void  Dta1xxNwIpTxRemoveChannel(IN PDTA1XX_FDO, IN UserIpTxChannel* pIpTx);
NTSTATUS  Dta1xxNwIpTxReset(IN PDTA1XX_FDO, IN UserIpTxChannel*,
						    IN DTA1XX_IPTX_RESET_CTRL*, OUT DTA1XX_IPTX_RESET_RTRN*);
NTSTATUS  Dta1xxNwIpTxSetIpPars(IN PDTA1XX_FDO, IN UserIpTxChannel*,
								IN DTA1XX_IPTX_IPPARS_CTRL*,
								OUT DTA1XX_IPTX_IPPARS_RTRN*);
NTSTATUS  Dta1xxNwIpTxSetTxControl(IN PDTA1XX_FDO, IN UserIpTxChannel*,
								   IN DTA1XX_IPTX_SETTXCONTROL_CTRL*,
								   OUT DTA1XX_IPTX_SETTXCONTROL_RTRN*);
void Dta1xxNwIpTxSetTxControlToIdle(IN  PDTA1XX_FDO  pFdo,	
									IN  UserIpTxChannel*  pIpTx);
NTSTATUS  Dta1xxNwIpTxSetTxMode(IN PDTA1XX_FDO, IN UserIpTxChannel*,
								IN DTA1XX_IPTX_SETTXMODE_CTRL*,
								OUT DTA1XX_IPTX_SETTXMODE_RTRN*);
void  Dta1xxNwIpTxChannelDestructor(IN PDTA1XX_FDO, IN UserIpTxChannel*);
void  Dta1xxNwIpTxChannelInit(IN PDTA1XX_FDO, IN UserIpTxChannel*);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IP TX IOCTL +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Implementation of IOCTL_DTA1XX_IPTX_CTRL
//
NTSTATUS  Dta1xxNwIpTxIoctl(
	IN PDTA1XX_FDO  pFdo,				// Device extension
	IN PFILEOBJECT  pFileObject,		// Current process
	IN ULONG  InBufferSize,				// Size of input buffer (pCtrl) in bytes
	IN ULONG  OutBufferSize,			// Size of output buffer (pRtrn) in bytes
	IN PDTA1XX_IP_RXTX_CTRL  pCtrl,		// Input buffer with control data
	OUT PDTA1XX_IP_RXTX_RTRN  pRtrn)	// Output buffer with return values
{
	char*  IoctlStr;					// Mnemonic string for IOCTL code
	ULONG  InReqSize;					// Required length of input buffer
	ULONG  OutReqSize = 0;				// Required length of output buffer
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;
	UserIpTxChannel*  pIpTx = NULL;

	// Base size for input buffer is 3x Int located in DTA1XX_IP_RXTX_CTRL
	InReqSize = 3 * sizeof(Int);
	
	// Determine expected length of data buffers
	switch (pCtrl->m_IpRxTxIoctl)	{
		
	case IOCTL_DTA1XX_IPTXCTRL_ATTACH:
		IoctlStr = "IOCTL_DTA1XX_IPTXCTRL_ATTACH";
		InReqSize += sizeof(DTA1XX_IPTX_ATTACH_CTRL);
		OutReqSize = sizeof(DTA1XX_IPTX_ATTACH_RTRN);
		break;

	case IOCTL_DTA1XX_IPTXCTRL_CLEARFLAGS:
		IoctlStr = "IOCTL_DTA1XX_IPTXCTRL_CLEARFLAGS";
		InReqSize += sizeof(DTA1XX_IPTX_FLAGS_CTRL);
		OutReqSize = sizeof(DTA1XX_IPTX_FLAGS_RTRN);
		break;

	case IOCTL_DTA1XX_IPTXCTRL_DETACH:
		IoctlStr = "IOCTL_DTA1XX_IPTXCTRL_DETACH";
		InReqSize += sizeof(DTA1XX_IPTX_DETACH_CTRL);
		OutReqSize = sizeof(DTA1XX_IPTX_DETACH_RTRN);
		break;

	case IOCTL_DTA1XX_IPTXCTRL_GETFIFOLOAD:
		IoctlStr = "IOCTL_DTA1XX_IPTXCTRL_GETFIFOLOAD";
		InReqSize += sizeof(DTA1XX_IPTX_GETFIFOLOAD_CTRL);
		OutReqSize = sizeof(DTA1XX_IPTX_GETFIFOLOAD_RTRN);
		break;

	case IOCTL_DTA1XX_IPTXCTRL_GETFLAGS:
		IoctlStr = "IOCTL_DTA1XX_IPTXCTRL_GETFLAGS";
		InReqSize += sizeof(DTA1XX_IPTX_FLAGS_CTRL);
		OutReqSize = sizeof(DTA1XX_IPTX_FLAGS_RTRN);
		break;

	case IOCTL_DTA1XX_IPTXCTRL_RESET:
		IoctlStr = "IOCTL_DTA1XX_IPTXCTRL_RESET";
		InReqSize += sizeof(DTA1XX_IPTX_RESET_CTRL);
		OutReqSize = sizeof(DTA1XX_IPTX_RESET_RTRN);
		break;

	case IOCTL_DTA1XX_IPTXCTRL_SETIPPARS:
		IoctlStr = "IOCTL_DTA1XX_IPTXCTRL_SETIPPARS";
		InReqSize += sizeof(DTA1XX_IPTX_IPPARS_CTRL);
		OutReqSize = sizeof(DTA1XX_IPTX_IPPARS_RTRN);
		break;

	case IOCTL_DTA1XX_IPTXCTRL_SETTXCONTROL:
		IoctlStr = "IOCTL_DTA1XX_IPTXCTRL_SETTXCONTROL";
		InReqSize += sizeof(DTA1XX_IPTX_SETTXCONTROL_CTRL);
		OutReqSize = sizeof(DTA1XX_IPTX_SETTXCONTROL_RTRN);
		break;

	case IOCTL_DTA1XX_IPTXCTRL_SETTXMODE:
		IoctlStr = "IOCTL_DTA1XX_IPTXCTRL_SETTXMODE";
		InReqSize += sizeof(DTA1XX_IPTX_SETTXMODE_CTRL);
		OutReqSize = sizeof(DTA1XX_IPTX_SETTXMODE_RTRN);
		break;

	default:
		IoctlStr = "??UNKNOWN IPTXIOCTL CODE??";
	}

	// Check buffer sizes
	if (InBufferSize < InReqSize) {
		DTA1XX_LOG(KERN_INFO, "[IpTx?] %s: INPUT BUFFER TOO SMALL Size=%u Req=%u",
				 IoctlStr, InBufferSize, InReqSize);
		return STATUS_INVALID_PARAMETER;
	}
	if (OutBufferSize < OutReqSize) {
		DTA1XX_LOG(KERN_INFO, "[IpTx?] %s: OUTPUT BUFFER TOO SMALL Size=%u Req=%u",
				 IoctlStr, OutBufferSize, OutReqSize);
		return STATUS_INVALID_PARAMETER;
	}

	// Check channel index
	if (pCtrl->m_ChannelIndex<0) {
		DTA1XX_LOG(KERN_INFO, "[IpTx?] Dta1xxNwIpTxIoctl: ChannelIndex=%d IS OUT OF RANGE!!",
				 pCtrl->m_ChannelIndex);
		return STATUS_INVALID_PARAMETER;
	}

#if LOG_LEVEL_IPTX > 1
	DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] %s: In=%d (Rq=%d), Out=%d (Rq=%d)",
			 pCtrl->m_ChannelIndex, IoctlStr, InBufferSize, InReqSize,
			 OutBufferSize, OutReqSize);
#endif

	// Compute IP Tx channel pointer
	pIpTx = Dta1xxNwIpTxGetChannel(pFdo, pCtrl->m_ChannelIndex);

	// Verify that channel exists
	if (  pIpTx==NULL
	   && pCtrl->m_IpRxTxIoctl!=IOCTL_DTA1XX_IPTXCTRL_ATTACH) {
		DTA1XX_LOG(KERN_INFO, "[IpTx%d] Dta1xxNwIpTxIoctl: Channel pointer is NULL!!",
				 pCtrl->m_ChannelIndex);
		return STATUS_INVALID_PARAMETER;
	}

	//-.-.-.-.-.-.-.-.-.-.-.- Big Switch to Execute The IpTx IOCTL -.-.-.-.-.-.-.-.-.-.-.-

	switch (pCtrl->m_IpRxTxIoctl)	{
		
	case IOCTL_DTA1XX_IPTXCTRL_ATTACH:
		// We want to attach to a new channel, so unlock this channel if already locked
		ReturnStatus = Dta1xxNwIpTxAttach(
							pFdo, pCtrl->m_IpPortIndex, pFileObject,
							&pCtrl->m_Data.m_TxAttachCtrl, &pRtrn->m_Data.m_TxAttachRtrn);
		break;
		
	case IOCTL_DTA1XX_IPTXCTRL_CLEARFLAGS:
		ReturnStatus = Dta1xxNwIpTxClearFlags(
							pFdo, pIpTx,
							&pCtrl->m_Data.m_TxFlagsCtrl, &pRtrn->m_Data.m_TxFlagsRtrn);
		break;

	case IOCTL_DTA1XX_IPTXCTRL_DETACH:
		ReturnStatus = Dta1xxNwIpTxDetach(
							pFdo, pIpTx,
							&pCtrl->m_Data.m_TxDetachCtrl, &pRtrn->m_Data.m_TxDetachRtrn);
		break;
		
	case IOCTL_DTA1XX_IPTXCTRL_GETFIFOLOAD:
		// This command is no longer used/support 
		ReturnStatus = STATUS_NOT_IMPLEMENTED;
		break;

	case IOCTL_DTA1XX_IPTXCTRL_GETFLAGS:
		ReturnStatus = Dta1xxNwIpTxGetFlags(
							pFdo, pIpTx,
							&pCtrl->m_Data.m_TxFlagsCtrl, &pRtrn->m_Data.m_TxFlagsRtrn);
		break;
	
	case IOCTL_DTA1XX_IPTXCTRL_RESET:
		ReturnStatus = Dta1xxNwIpTxReset(
							pFdo, pIpTx,
							&pCtrl->m_Data.m_TxResetCtrl, &pRtrn->m_Data.m_TxResetRtrn);
		break;
		
	case IOCTL_DTA1XX_IPTXCTRL_SETIPPARS:
		ReturnStatus = Dta1xxNwIpTxSetIpPars(
							pFdo, pIpTx,
							&pCtrl->m_Data.m_TxIpParsCtrl, &pRtrn->m_Data.m_TxIpParsRtrn);
		break;
		
	case IOCTL_DTA1XX_IPTXCTRL_SETTXCONTROL:
		ReturnStatus = Dta1xxNwIpTxSetTxControl(
							pFdo, pIpTx,
							&pCtrl->m_Data.m_TxSetTxControlCtrl,
							&pRtrn->m_Data.m_TxSetTxControlRtrn);
		break;
		
	case IOCTL_DTA1XX_IPTXCTRL_SETTXMODE:
		ReturnStatus = Dta1xxNwIpTxSetTxMode(pFdo, pIpTx,
							&pCtrl->m_Data.m_TxSetTxModeCtrl,
							&pRtrn->m_Data.m_TxSetTxModeRtrn);
		break;

	default:
		ReturnStatus = STATUS_NOT_IMPLEMENTED;
	}
	return ReturnStatus;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IP TX IOCTL IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// PRE-CONDITION for all IOCTL implementation routines:
//	- ChannelIndex is valid (except for ATTACH)
//	- Channel exists (except for ATTACH)


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxAttach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Implement IOCTL to attach to an IP Tx channel.
// Pre: IpTxFastMutex not locked
//
NTSTATUS  Dta1xxNwIpTxAttach(
	IN  PDTA1XX_FDO  pFdo,						// Device extension
	IN  Int  PortIndex,							// IP port index
	IN  PFILEOBJECT pFileObject,				// Current process
	IN  DTA1XX_IPTX_ATTACH_CTRL*  pAttachCtrl,	// IOCTL input data
	OUT DTA1XX_IPTX_ATTACH_RTRN*  pAttachRtrn)	// IOCTL return value
{
	Int  Idx;
	KIRQL  OldIrqL;
	SIZE_T  AllocSize = sizeof(UserIpTxChannel);
	UserIpTxChannel* pIpTx;						// Next element
	UserIpTxChannel* pIpTxPrev= NULL;			// Previous element
	UserIpTxChannel* pIpTxNew;					// New element
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success
	
	// Protect list by the fast mutex
	ExAcquireFastMutex(&pFdo->m_IpTxFastMutex);
	
	pIpTx = pFdo->m_pIpTx;

	// Search for first free index number.
	Idx = 0;
	while (pIpTx) {
		if (Idx != pIpTx->m_ChannelIndex)
			break;
		Idx++;
		pIpTxPrev = pIpTx;
		pIpTx = pIpTx->m_pNext;
	}

#if LOG_LEVEL_IPTX > 0
	DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] Dta1xxNwIpTxAttach: Found free index", Idx);
#endif

	pIpTxNew = ExAllocatePoolWithTag(NonPagedPool, AllocSize, '1ATD');
	if (pIpTxNew != NULL) {
		
		//-.-.-.-.-.-.-.-.-.-.-.-.- Initialise IP Tx Channel -.-.-.-.-.-.-.-.-.-.-.-.-
#if LOG_LEVEL_IPTX > 0
		DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] Dta1xxNwIpTxAttach: Initialise UserIpTxChannel",
					Idx);
#endif
		RtlZeroMemory(pIpTxNew, AllocSize);

		// Identity
		pIpTxNew->m_IpPortIndex = PortIndex;
		pIpTxNew->m_ChannelIndex = pAttachRtrn->m_ChannelIndex = Idx;
		pIpTxNew->m_FileObject = pFileObject;	// For purging in Close

		// Tx buffer initialisation
		pIpTxNew->m_SharedBuf.m_pBuf = NULL;
		pIpTxNew->m_SharedBuf.m_pUserBuf = NULL;
		pIpTxNew->m_SharedBuf.m_Allocated = FALSE;
		pIpTxNew->m_SharedBuf.m_PageList = NULL;
		pIpTxNew->m_SharedBuf.m_SglAllocated = FALSE;

		// Status spin lock for access protection
		KeInitializeSpinLock(&pIpTxNew->m_FlagsSpinLock);

		// Other initialisation
		Dta1xxNwIpTxChannelInit(pFdo, pIpTxNew);

		// Now aquire the SpinLock before inserting into the linked list
		KeAcquireSpinLock(&pFdo->m_IpTxSpinLock, &OldIrqL);

		if (pFdo->m_pIpTx) {
			// There's already an element available
			if (pIpTx) {
				// Insert element before pIpTx
				if (pIpTx->m_pPrev) {
					// Not first element
					pIpTx->m_pPrev->m_pNext = pIpTxNew;
					pIpTxNew->m_pPrev = pIpTx->m_pPrev;
					
				} else {
					// First element
					pFdo->m_pIpTx = pIpTxNew;
					pIpTxNew->m_pPrev = NULL;
				}
				pIpTx->m_pPrev = pIpTxNew;
				pIpTxNew->m_pNext = pIpTx;
				
			} else {
				// Insert element at end
				ASSERT(pIpTxPrev->m_pNext == NULL);
				pIpTxPrev->m_pNext = pIpTxNew;
				pIpTxNew->m_pPrev = pIpTxPrev;
				pIpTxNew->m_pNext = NULL;
			}
		} else {
			pIpTxNew->m_pNext = NULL;
			pIpTxNew->m_pPrev = NULL;
			pFdo->m_pIpTx = pIpTxNew;
		}
		
		// We're done, release the SpinLock
		KeReleaseSpinLock(&pFdo->m_IpTxSpinLock, OldIrqL);
	} else {
		DTA1XX_LOG(KERN_INFO, "[IpTx%d] Dta1xxNwIpTxAttach: Allocation of "
					"UserIpTxChannel FAILS", Idx);
		ReturnStatus = STATUS_INSUFFICIENT_RESOURCES;
	}

	ExReleaseFastMutex(&pFdo->m_IpTxFastMutex);
	return ReturnStatus;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxClearFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Clear latched status flags.
// Flags supported: 
//
NTSTATUS  Dta1xxNwIpTxClearFlags(
	IN  PDTA1XX_FDO  pFdo,						// Device extension
	IN  UserIpTxChannel*  pIpTx,				// IP Tx channel
	IN  DTA1XX_IPTX_FLAGS_CTRL*  pFlagsCtrl,	// IOCTL input data
	OUT DTA1XX_IPTX_FLAGS_RTRN*  pFlagsRtrn)	// IOCTL return value
{
	KIRQL  OldIrqL;								// OLD IRQ level
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

	// Clear flags specified in IOCTL
	KeAcquireSpinLock(&pIpTx->m_FlagsSpinLock, &OldIrqL);
	pIpTx->m_LatchedFlags &= ~pFlagsCtrl->m_Flags;
	KeReleaseSpinLock(&pIpTx->m_FlagsSpinLock, OldIrqL);
#if LOG_LEVEL_IPTX > 0
	DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] After Dta1xxNwIpTxClearFlags Flags=0x%x Latched=0x%x",
			 pIpTx->m_ChannelIndex, pIpTx->m_Flags, pIpTx->m_LatchedFlags);
#endif

	return ReturnStatus;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxDetach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Implement IOCTL to detach from an IP Tx channel.
//
NTSTATUS  Dta1xxNwIpTxDetach(
	IN  PDTA1XX_FDO  pFdo,						// Device extension
	IN  UserIpTxChannel*  pIpTx,				// IP Tx channel
	IN  DTA1XX_IPTX_DETACH_CTRL*  pDetachCtrl,	// IOCTL input data
	OUT DTA1XX_IPTX_DETACH_RTRN*  pDetachRtrn)	// IOCTL return value
{
	KIRQL  OldIrqL;								// OLD IRQ level
	
	// Reset transmit control to IDLE
	Dta1xxNwIpTxSetTxControlToIdle(pFdo, pIpTx);

	// Protect list by the fast mutex
	ExAcquireFastMutex(&pFdo->m_IpTxFastMutex);

	// We are going to change m_pIpTx[X] so we need to acquire the IP-Tx spinlock
	KeAcquireSpinLock(&pFdo->m_IpTxSpinLock, &OldIrqL);

	// Free memory, after this function pIpTx is invalid
	Dta1xxNwIpTxChannelDestructor(pFdo, pIpTx);	
	pIpTx = NULL;
	
	// We're done, release the SpinLock again
	KeReleaseSpinLock(&pFdo->m_IpTxSpinLock, OldIrqL);

	ExReleaseFastMutex(&pFdo->m_IpTxFastMutex);

	return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxGetFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get status flags (...)
//
NTSTATUS  Dta1xxNwIpTxGetFlags(
	IN  PDTA1XX_FDO  pFdo,						// Device extension
	IN  UserIpTxChannel*  pIpTx,				// IP Tx channel
	IN  DTA1XX_IPTX_FLAGS_CTRL*  pFlagsCtrl,	// IOCTL input data
	OUT DTA1XX_IPTX_FLAGS_RTRN*  pFlagsRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

	// Copy flags to IOCTL return value
	pFlagsRtrn->m_Flags = pIpTx->m_Flags;
	pFlagsRtrn->m_LatchedFlags = pIpTx->m_LatchedFlags;
#if LOG_LEVEL_IPTX > 1
	DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] Dta1xxNwIpTxGetFlags Flags=0x%x Latched=0x%x",
			 pIpTx->m_ChannelIndex, pFlagsRtrn->m_Flags, pFlagsRtrn->m_LatchedFlags);
#endif
	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Reset IP Tx channel
//
NTSTATUS  Dta1xxNwIpTxReset(
	IN  PDTA1XX_FDO  pFdo,						// Device extension
	IN  UserIpTxChannel*  pIpTx,				// IP Tx channel
	IN  DTA1XX_IPTX_RESET_CTRL*  pResetCtrl,	// IOCTL input data
	OUT DTA1XX_IPTX_RESET_RTRN*  pResetRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success
	DTA1XX_IPTX_SETTXCONTROL_CTRL  SetTxCtrlCtrl;
	DTA1XX_IPTX_SETTXCONTROL_RTRN  SetTxCtrlRtrn;
#if LOG_LEVEL_IPTX > 0
	DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] Dta1xxNwIpTxReset ResetMode=%d",
			 pIpTx->m_ChannelIndex, pResetCtrl->m_ResetMode);
#endif

	// Reset Transmit-Control state to IDLE
	SetTxCtrlCtrl.m_TxControl = DTA1XX_TXCTRL_IDLE;
	Dta1xxNwIpTxSetTxControl(pFdo, pIpTx, &SetTxCtrlCtrl, &SetTxCtrlRtrn);

	// Re-initialise channel
	Dta1xxNwIpTxChannelInit(pFdo, pIpTx);

	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxSetIpPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set IP-level parameters
//
NTSTATUS  Dta1xxNwIpTxSetIpPars(
	IN  PDTA1XX_FDO  pFdo,						// Device extension
	IN  UserIpTxChannel*  pIpTx,				// IP Tx channel
	IN  DTA1XX_IPTX_IPPARS_CTRL*  pIpParsCtrl,	// IOCTL input data
	OUT DTA1XX_IPTX_IPPARS_RTRN*  pIpParsRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

#if LOG_LEVEL_IPTX > 0
	DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] Dta1xxNwIpTxSetIpPars", pIpTx->m_ChannelIndex);
#endif

	pIpTx->m_DstIp[0] = pIpParsCtrl->m_DstIp[0];
	pIpTx->m_DstIp[1] = pIpParsCtrl->m_DstIp[1];
	pIpTx->m_DstIp[2] = pIpParsCtrl->m_DstIp[2];
	pIpTx->m_DstIp[3] = pIpParsCtrl->m_DstIp[3];

	pIpTx->m_DstPort  = pIpParsCtrl->m_DstPort;
	pIpTx->m_Protocol = pIpParsCtrl->m_Protocol;
	pIpTx->m_FecMode  = pIpParsCtrl->m_FecMode;

	return ReturnStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxSetTxControlIdle -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxNwIpTxSetTxControlToIdle(
	IN  PDTA1XX_FDO  pFdo,						// Device extension
	IN  UserIpTxChannel*  pIpTx)
{
	KIRQL  OldIrqL;	// OLD IRQ level
	
	if (pIpTx->m_TxControl == DTA1XX_TXCTRL_IDLE) {
		return;
	}
	
	// First set to hold to stop the transmitting thread
	KeAcquireSpinLock(&pFdo->m_IpTxSpinLock, &OldIrqL);
	pIpTx->m_TxControl = DTA1XX_TXCTRL_IDLE;
	KeReleaseSpinLock(&pFdo->m_IpTxSpinLock, OldIrqL);
	
	// Reset the read and write pointer to the initial state. 
	// The pointers are reset in the Dtapi also.
	Dta1xxNwIpTxChannelInit(pFdo, pIpTx);
    
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxSetTxControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set Transmit-Control state
//
NTSTATUS  Dta1xxNwIpTxSetTxControl(
	IN  PDTA1XX_FDO  pFdo,						// Device extension
	IN  UserIpTxChannel*  pIpTx,				// IP Tx channel
	IN  DTA1XX_IPTX_SETTXCONTROL_CTRL*  pSetTxControlCtrl,	// IOCTL input data
	OUT DTA1XX_IPTX_SETTXCONTROL_RTRN*  pSetTxControlRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

#if LOG_LEVEL_IPTX > 0
	DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] Dta1xxNwIpTxSetTxControl TxControl=%d",
				pIpTx->m_ChannelIndex, pSetTxControlCtrl->m_TxControl);
#endif

	if (pSetTxControlCtrl->m_TxControl == DTA1XX_TXCTRL_SEND) {
		if (pIpTx->m_SharedBuf.m_Allocated == FALSE) {
			DTA1XX_LOG(KERN_INFO, "[IpTx%d] Dta1xxNwIpTxSetTxControl Tx buffer not allocated",
				pIpTx->m_ChannelIndex);
			return STATUS_UNSUCCESSFUL;
		}
		// Set the initial reference time stamp.
		pIpTx->m_RefTimestamp = Dta1xxGenGetRefClkCntReg64(pFdo->m_pGenRegs)+ 
								(5 * DTA1XX_IPTX_PERINT * DTA160_REF_CLOCK);
		pIpTx->m_pCommon->m_StartTimestamp = pIpTx->m_RefTimestamp;	

		pIpTx->m_LastTimestampOffs = 0;
		pIpTx->m_TxControl = DTA1XX_TXCTRL_SEND;
		
	}
	else if (pSetTxControlCtrl->m_TxControl == DTA1XX_TXCTRL_HOLD) {
		if (pIpTx->m_SharedBuf.m_Allocated == FALSE) {
			DTA1XX_LOG(KERN_INFO, "[IpTx%d] Dta1xxNwIpTxSetTxControl Tx buffer not allocated",
						pIpTx->m_ChannelIndex);
			return STATUS_UNSUCCESSFUL;
		}
		pIpTx->m_TxControl = DTA1XX_TXCTRL_HOLD;
		pIpTx->m_Flags &= ~DTA1XX_TX_FIFO_UFL;
	}
	else {
		Dta1xxNwIpTxSetTxControlToIdle(pFdo, pIpTx);
		pIpTx->m_Flags &= ~DTA1XX_TX_FIFO_UFL;
	}

	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxSetTxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set Transmit Mode
//
NTSTATUS  Dta1xxNwIpTxSetTxMode(
	IN  PDTA1XX_FDO  pFdo,						// Device extension
	IN  UserIpTxChannel*  pIpTx,				// IP Tx channel
	IN  DTA1XX_IPTX_SETTXMODE_CTRL*  pSetTxModeCtrl,	// IOCTL input data
	OUT DTA1XX_IPTX_SETTXMODE_RTRN*  pSetTxModeRtrn)	// IOCTL return value
{
#if LOG_LEVEL_IPTX > 0
	DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] Dta1xxNwIpTxSetTxMode TxMode=%d",
			 pIpTx->m_ChannelIndex, pIpTx->m_TxMode);
#endif

	return STATUS_UNSUCCESSFUL;//ReturnStatus;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IP TX IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxChannelDestructor -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Free resources used by IP Tx channel object
//
void  Dta1xxNwIpTxChannelDestructor(
	IN PDTA1XX_FDO  pFdo,					// Device extension
	IN UserIpTxChannel*  pIpTx)				// IP Tx channel
{
#if LOG_LEVEL_IPTX > 0
	DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] Dta1xxNwIpTxChannelDestructor", pIpTx->m_ChannelIndex);
#endif

	// Free the Transmit-FIFO Packet Buffer, if it has been allocated
	// (This buffer is allocated when SetIpPars is called; if the channel is detached
	//  before that, no buffer is allocated)
	if (pIpTx->m_SharedBuf.m_Allocated == TRUE) {
#if LOG_LEVEL_IPTX > 0
		DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] Dta1xxNwIpTxChannelDestructor: AUTORELEASE BUFFER",
				 pIpTx->m_ChannelIndex);
#endif
		Dta1xxNwIpTxReleaseBuf(pFdo, pIpTx);
	}

	// Remove channel from linked list
	Dta1xxNwIpTxRemoveChannel(pFdo, pIpTx);		
			
	// Free IP Tx channel object
#if LOG_LEVEL_IPTX > 0
	DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] Free channel object", pIpTx->m_ChannelIndex);
#endif
	ExFreePool(pIpTx);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxChannelInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Initialise state variables in IP Tx channel.
// Be carefull for adding some initialisation code here.
// This function will be used in several places
//
void  Dta1xxNwIpTxChannelInit(
	IN PDTA1XX_FDO  pFdo,					// Device extension
	IN UserIpTxChannel*  pIpTx)				// IP Tx channel
{
#if LOG_LEVEL_IPTX > 0
	DTA1XX_LOG(KERN_DEBUG, "[IpTx%d] Dta1xxNwIpTxChannelInit", pIpTx->m_ChannelIndex);
#endif

	if (pIpTx->m_pCommon) {
		// Initialise read/write pointers
		pIpTx->m_pCommon->m_ReadOffset = 0;
		pIpTx->m_pCommon->m_WriteOffset = 0;
	}
}


//.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxPurgeLeftOverChannels -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This routine is called after a CLOSE, to purge IP Tx channels that have not been
// deleted neatly.
//
void  Dta1xxNwIpTxPurgeLeftOverChannels(
	IN PDTA1XX_FDO  pFdo,					// Device extension
	IN PFILEOBJECT pFileObject)				// Current process
{
	KIRQL  OldIrqL;								// OLD IRQ level
	UserIpTxChannel*  pIpTx;
	UserIpTxChannel*  pIpTxTmp;
	
	if (pFdo->m_pIpTx == NULL)
		return;

	// Protect list by fast mutex
	ExAcquireFastMutex(&pFdo->m_IpTxFastMutex);
	pIpTx = pFdo->m_pIpTx;

#if LOG_LEVEL_IPTX > 0
	DTA1XX_LOG(KERN_DEBUG, "Dta1xxNwIpTxPurgeLeftOverChannels");
#endif

	// Loop through all IP Tx channels, check whether they belong to the current session
	// (by checking FileObject) and if so delete them
	

	// This may cause a problem because the m_pIpTx array itself is not locked with a spinlock.
	// But because this is executed during a close, the pIpTx can not be freeded by a detach,
	// so this should not cause a problem.
	
	while (pIpTx) {
        if (pIpTx->m_FileObject==pFileObject) {

			Dta1xxNwIpTxSetTxControlToIdle(pFdo, pIpTx);
#if LOG_LEVEL_IPTX > 0
			DTA1XX_LOG(KERN_DEBUG, "Purge IP Tx channel #%d", pIpTx->m_ChannelIndex);
#endif
			
			pIpTxTmp = pIpTx->m_pNext;
			
			KeAcquireSpinLock(&pFdo->m_IpTxSpinLock, &OldIrqL);
			
			// Free memory, after this function pIpTx is invalid
			Dta1xxNwIpTxChannelDestructor(pFdo, pIpTx);
			pIpTx = NULL;
			
			// We're done, release the SpinLock again
			KeReleaseSpinLock(&pFdo->m_IpTxSpinLock, OldIrqL);
			pIpTx = pIpTxTmp;
		} else
			pIpTx = pIpTx->m_pNext;
	}
	ExReleaseFastMutex(&pFdo->m_IpTxFastMutex);
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ WORKER THREAD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#ifdef WINBUILD
void Dta1xxNwIpTxWorkerDpc(IN PKDPC Dpc, IN PVOID DeferredContext, 
						   IN PVOID SystemArgument1, IN PVOID SystemArgument2)
#else
void Dta1xxNwIpTxWorkerDpc(IN unsigned long DeferredContext)
#endif
{	NwStore* pNwStore = (NwStore*)DeferredContext;
	
	PDTA1XX_FDO  pFdo = pNwStore->m_pFdo;
	UserIpTxChannel* pIpTx;
	Dta1xxDmaTxHeader*  pDmaTxHeader = NULL;
	BOOLEAN	NewPkts = TRUE;
	UInt PacketLength;
	
	UInt64 NewPktTime;
	UInt8* pNewPkt;
	UserIpTxChannel* pNewPktIpTx = NULL;
	UInt64 TmpPktTime;
	volatile UInt8*	pBuf;
#ifdef DBG
	UInt64 Delay;
	UInt64 StartTime;
#endif // DBG
	UInt64 MaxPktTime;
	UInt64 CurTime;
	volatile UInt8* pRead;
	volatile UInt8* pWrite;
	LONG NumBytesToSend = 0;
	UInt NumFree;
	Int  NumBytesToEnd;			// Number of bytes from pWrite till end of buffer
	Int  NumBytesToCopy;		// Number of bytes to copy
	Int  NumBytes;				// Temp. store

	
	// Periodic interrupt is about every 20ms
#ifdef DBG
	StartTime = Dta1xxGenGetRefClkCntReg64((Int*)(pNwStore->m_pFdo->m_pGenRegs));
#endif

	KeAcquireSpinLockAtDpcLevel(&pFdo->m_IpTxSpinLock);

	CurTime = Dta1xxGenGetRefClkCntReg64(pNwStore->m_pFdo->m_pGenRegs);
	MaxPktTime = CurTime + (2 * DTA1XX_IPTX_PERINT * DTA160_REF_CLOCK);

	// Periodic interrupt is every 20ms (DTA1XX_IPTX_PERINT)
	while (NewPkts) {
		NewPkts	= FALSE;
		NewPktTime = 0xffffffffffffffffLLU;

		pIpTx = pFdo->m_pIpTx;
		while (pIpTx)  {
			// Found an channel
			// Is the channel sending?
			if (pIpTx->m_TxControl != DTA1XX_TXCTRL_SEND) {
				pIpTx = pIpTx->m_pNext;
				continue;
			}

			pBuf = pIpTx->m_SharedBuf.m_pBuf;

			// Search for available packets
			if (pIpTx->m_pCommon->m_ReadOffset != pIpTx->m_pCommon->m_WriteOffset) {
				// Packets are available for this channel
				pDmaTxHeader = (Dta1xxDmaTxHeader*)(pBuf + pIpTx->m_pCommon->m_ReadOffset);

				// If offset wrapped, increment Reference time stamp
				if (pDmaTxHeader->Timestamp < pIpTx->m_LastTimestampOffs) {
					pIpTx->m_LastTimestampOffs = pDmaTxHeader->Timestamp;
					pIpTx->m_RefTimestamp+= (UInt64)1<<32;
				}

				if (pIpTx->m_Flags & DTA1XX_TX_FIFO_UFL) {
					// Fifo was empty, but not anymore.
					// Recalculate new reference timestamp for this channel.
#if LOG_LEVEL_IPTX > 0
					DTA1XX_LOG(KERN_INFO, "[IpTx%d] Dta1xxNwIpTxWorkerDpc: NO UNDERFLOW ANYMORE", 
							pIpTx->m_ChannelIndex);
#endif
					pIpTx->m_RefTimestamp = 
							Dta1xxGenGetRefClkCntReg64(pFdo->m_pGenRegs) + 
							(5 * DTA1XX_IPTX_PERINT * DTA160_REF_CLOCK) - 
						pDmaTxHeader->Timestamp;
					pIpTx->m_LastTimestampOffs = pDmaTxHeader->Timestamp;
				}

				TmpPktTime = pIpTx->m_RefTimestamp + pDmaTxHeader->Timestamp;
				if ((TmpPktTime <= MaxPktTime) && (TmpPktTime < NewPktTime)){
					// New packet found which must be sent first
					NewPkts	= TRUE;
					NewPktTime = TmpPktTime;
					pNewPkt = (UInt8*)(pBuf + pIpTx->m_pCommon->m_ReadOffset);
					pNewPktIpTx = pIpTx;
				} /*else {
					static UInt64 cTmpPktTime=0;
					if (TmpPktTime != cTmpPktTime) {
						DTA1XX_LOG(KERN_INFO, "Pkt:%llu Max:%llu", TmpPktTime, MaxPktTime);
						cTmpPktTime = TmpPktTime;
					} else {
						DTA1XX_LOG(KERN_INFO, "          Max:%llu", MaxPktTime);
					}
				}*/

				// No overflow
				pIpTx->m_Flags &= ~DTA1XX_TX_FIFO_UFL;

			} else {
				
				if ((pIpTx->m_Flags & DTA1XX_TX_FIFO_UFL) == 0) {
					// Underflow!
#if LOG_LEVEL_IPTX > 0
					DTA1XX_LOG(KERN_INFO, "[IpTx%d] Dta1xxNwIpTxWorkerDpc: UNDERFLOW", 
								pIpTx->m_ChannelIndex);
#endif
					// No need for spinlock: m_Flags is only written from Worker thread
					pIpTx->m_Flags |= DTA1XX_TX_FIFO_UFL;

					// Acquire spin lock for updating latched flags
					KeAcquireSpinLockAtDpcLevel(&pIpTx->m_FlagsSpinLock);
					pIpTx->m_LatchedFlags |= DTA1XX_TX_FIFO_UFL;
					KeReleaseSpinLockFromDpcLevel(&pIpTx->m_FlagsSpinLock);
				}
			}
			pIpTx = pIpTx->m_pNext;
		}
		if (NewPkts) {
			// Copy new packet into cdma buffer
			pIpTx = pNewPktIpTx;  
			pBuf = pIpTx->m_SharedBuf.m_pBuf;

			pDmaTxHeader = (Dta1xxDmaTxHeader*)(pBuf + pIpTx->m_pCommon->m_ReadOffset);
			PacketLength = pDmaTxHeader->TransmitControl.PacketLength + sizeof(Dta1xxDmaTxHeader);
			
			// Round up to DWORD's
			PacketLength+= (PacketLength % 4 == 0 ? 0 : 4 - (PacketLength % 4));
			
			// Compute free Cdma buffer space
			pRead = pNwStore->m_pIpTxRead;
			pWrite = pNwStore->m_pIpTxWrite;
			NumFree = (UInt)(pRead > pWrite ? pRead - pWrite : pNwStore->m_IpTxBufSize - (pWrite - pRead));

			if (NumFree < (PacketLength+1)) { // +1 to prevent pWrite == pRead and thus empty state
#if LOG_LEVEL_IPTX > 0
				DTA1XX_LOG(KERN_INFO, "[NwTx%d] Dta1xxNwIpTxWorkerDpc: Cdma buffer full",
					pNwStore->m_IpPortIndex);
#endif
				break;	// exit while
			}

			// Update timestamp
			pIpTx->m_LastTimestampOffs = pDmaTxHeader->Timestamp;
			pDmaTxHeader->Timestamp = (UInt32)(pIpTx->m_RefTimestamp + 
											   pDmaTxHeader->Timestamp);

			// Check if the network is administrative up, if not we skip this packet
			// and only update the read offset
			if (pNwStore->m_AdminStatus == DTA1XX_NW_ADMINSTATUS_UP) {
			
				// Write packet to Cdma buffer
				// Split the copy if packet is larger then NumBytesToEnd.
				NumBytesToCopy = PacketLength;
				pRead = (pBuf + pIpTx->m_pCommon->m_ReadOffset);

				while (NumBytesToCopy) {
					NumBytesToEnd = (pNwStore->m_pIpTxEndOfBuffer - pWrite);
					NumBytes = (NumBytesToEnd < NumBytesToCopy ? NumBytesToEnd : NumBytesToCopy);

					RtlCopyMemory((UInt8*)pWrite, (UInt8*)pRead, NumBytes);
					NumBytesToCopy-= NumBytes;
					// Move write pointer
					pWrite+= NumBytes;
					// Move position of source data pointer
					pRead+= NumBytes;

					if (pWrite >= pNwStore->m_pIpTxEndOfBuffer) {
						// Set write pointer to start of buffer
						pWrite = pNwStore->m_pIpTxEndOfBuffer - pNwStore->m_IpTxBufSize;
						ASSERT(pWrite == pFdo->m_IpMqTx[pNwStore->m_IpPortIndex]->m_CdmaShBuf.m_pBuf);
					}
				}
				pNwStore->m_pIpTxWrite = (UInt8*)pWrite;

				NumBytesToSend+= PacketLength;
			}

			// Check for ReadOffset wrapping
			if ((pIpTx->m_pCommon->m_ReadOffset + PacketLength) > 
				(pIpTx->m_BufSize - DTA1XX_IPTX_MAX_PACKET_LENGTH)) {
				pIpTx->m_pCommon->m_ReadOffset = 0; 
			} else {
				pIpTx->m_pCommon->m_ReadOffset+= PacketLength;
			}

			if (NumBytesToSend >= IPTX_DMA_MAX_TRANSFER_SIZE) {

                // If new 2MB is available, start new DMA transfer if not already started
				if (InterlockedExchangeAdd(&pNwStore->m_IpTxNumBytesToSend, NumBytesToSend) == 0) {
					Channel* pCh = pFdo->m_IpMqTx[pNwStore->m_IpPortIndex];
#if LOG_LEVEL_IPTX > 0
					DTA1XX_LOG(KERN_DEBUG, "DMA NOT BUSY %llu %d", MaxPktTime, atomic_read(&pNwStore->m_IpTxNumBytesToSend));
#endif
					
					// Set m_CdmaDmaRunning to prevent race condition with aborting dma
					pCh->m_CdmaDmaRunning = TRUE;
					if (!pNwStore->m_IpTxStop) {
						// Start new DMA transfer
						pCh->m_Dma.m_CurrentTransferLength = atomic_read(&pNwStore->m_IpTxNumBytesToSend);
						Dta1xxCdmaStartSize(pCh);
#if LOG_LEVEL_IPTX > 0
						DTA1XX_LOG(KERN_DEBUG, "[NwTx%d] Dta1xxNwIpTxWorkerDpc: Start new 2MB DMA",
							pNwStore->m_IpPortIndex);
#endif
					} else {
						pCh->m_CdmaDmaRunning = FALSE;
						break; // exit while
					}
				} 
				NumBytesToSend = 0;
				
			}
		}
	}

	// Update bytes ready to be send and start new DMA if not already started
	if (NumBytesToSend) {
		if (InterlockedExchangeAdd(&pNwStore->m_IpTxNumBytesToSend, NumBytesToSend) == 0) {
			Channel* pCh = pFdo->m_IpMqTx[pNwStore->m_IpPortIndex];

			// Set m_CdmaDmaRunning to prevent race condition with aborting dma
			pCh->m_CdmaDmaRunning = TRUE;
			if (!pNwStore->m_IpTxStop) {
				// Start new DMA transfer
				pCh->m_Dma.m_CurrentTransferLength = atomic_read(&pNwStore->m_IpTxNumBytesToSend);
				Dta1xxCdmaStartSize(pCh);
			} else {
				pCh->m_CdmaDmaRunning = FALSE;
			}
		}
	}

#ifdef DBG
	CurTime = Dta1xxGenGetRefClkCntReg64((Int*)(pNwStore->m_pFdo->m_pDta1xxGen));
	CurTime = CurTime - StartTime;
	
	Delay = (CurTime * 10000) / DTA160_REF_CLOCK;  
	Delay = Delay / 10000;

	if (Delay > DTA1XX_IPTX_PERINT) {
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpTxWorkerDpc takes TOO LONG!!");
	}
#endif
	KeReleaseSpinLockFromDpcLevel(&pFdo->m_IpTxSpinLock);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxCreateCdmaBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxNwIpTxCreateCdmaBuffer(NwStore* pNwStore) 
{	PDTA1XX_FDO  pFdo = pNwStore->m_pFdo;
	UInt PortIndex = pNwStore->m_IpPortIndex;
	ShBufDesc* pCdmaBuf = &pFdo->m_IpMqTx[PortIndex]->m_CdmaShBuf;
	UInt BufSize = IPTX_DMA_BUFFER_SIZE;
	NTSTATUS Status = STATUS_SUCCESS;
	
	Status = AllocateCdmaBuffer(pFdo->m_IpMqTx[PortIndex], &BufSize);
	if (Status != STATUS_SUCCESS) {
		return Status;
	}

	// Initialise buffer pointers
	pNwStore->m_IpTxBufSize = BufSize;
	pNwStore->m_pIpTxRead = pCdmaBuf->m_pBuf;
	pNwStore->m_pIpTxWrite = pCdmaBuf->m_pBuf;
	pNwStore->m_pIpTxEndOfBuffer = pCdmaBuf->m_pBuf + BufSize;
	atomic_set(&pNwStore->m_IpTxNumBytesToSend, 0);
	
	Status = Dta1xxCdmaSglCreate(pFdo->m_IpMqTx[PortIndex]);
	if (NT_SUCCESS(Status)) pCdmaBuf->m_SglAllocated = TRUE;
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxCleanUpCdmaBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxNwIpTxCleanUpCdmaBuffer(NwStore* pNwStore) 
{	PDTA1XX_FDO  pFdo = pNwStore->m_pFdo;
	UInt PortIndex = pNwStore->m_IpPortIndex;
	Channel* pCh = pFdo->m_IpMqTx[PortIndex];
	ShBufDesc* pCdmaBuf = &pCh->m_CdmaShBuf;

	if (pCh->m_CdmaDmaRunning) {
		// Signal DMA completion routine that no further transmits must be started
		pNwStore->m_IpTxStop = TRUE;
		Dta1xxCdmaStop(pCh);
		// Wait for DMA to be finished before releasing the DMA Sgl list.
		while (pCh->m_CdmaDmaRunning) Dta1xxWaitMs(1);
	}

	if (pCdmaBuf->m_SglAllocated) {
		Dta1xxCdmaSglCleanUp(pCh);
	}

	if (pCdmaBuf->m_Allocated) {
		FreeCdmaBuffer(pCh);
		pCdmaBuf->m_Allocated = FALSE;
	}
	return;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxGetChannel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UserIpTxChannel* Dta1xxNwIpTxGetChannel(
	IN PDTA1XX_FDO  pFdo,					// Device extension
	IN Int ChannelIndex)					// Channel Index
{
	UserIpTxChannel* pIpTx;
	
	ExAcquireFastMutex(&pFdo->m_IpTxFastMutex);
	pIpTx = pFdo->m_pIpTx;
	
	while (pIpTx) {
		if (pIpTx->m_ChannelIndex == ChannelIndex)
			break;
		else 
			pIpTx = pIpTx->m_pNext;
	}
	ExReleaseFastMutex(&pFdo->m_IpTxFastMutex);

	return pIpTx;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxRemoveChannel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxNwIpTxRemoveChannel(
	IN PDTA1XX_FDO  pFdo,					// Device extension
	IN UserIpTxChannel* pIpTx)
{
	if (pIpTx->m_pPrev == NULL) {
		// This was the first element.
		pFdo->m_pIpTx = pIpTx->m_pNext;
		if (pIpTx->m_pNext)
			pIpTx->m_pNext->m_pPrev = NULL;
	} else {
		pIpTx->m_pPrev->m_pNext = pIpTx->m_pNext;
		if (pIpTx->m_pNext)
			pIpTx->m_pNext->m_pPrev = pIpTx->m_pPrev;
	}
}
