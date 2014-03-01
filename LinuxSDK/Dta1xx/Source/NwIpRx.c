//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NwIpRx.c *#*#*#*#*#*#*#*#*# (C) 2005-2006 DekTec
//
// For NETWORK cards: IP Receive routines
//
// This file contains the IP Receive worker thread, and routines to start, stop and
// control it.
// The worker thread reads IP packets from the DVB queue on a network card.
// IP packets are parsed and stored in the appropriate queues.
//
// TO DO:
//	- Time out op bit-rate meting: als er gedurende b.v. 200ms geen enkel IP pakket is
//	  ontvangen, moet bit rate terug naar nul gezet worden, en ClkDet naar false.
//	  Wellicht in periodieke interrupt?

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	2005.12.21	RD	Added RTP + FEC reconstruction
//	2005.08.09	SD	Moved IP Receive code to NwIpRx.c
//	2005.07.08	RD	Created	


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"
#include "../Include/EthPrtcls.h"
//#include <ntddndis.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define MAX_OUTOFSYNC_SEQ_NUM	12

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
NTSTATUS  Dta1xxNwIpRxAttach(IN PDTA1XX_FDO, IN Int, IN PFILEOBJECT,
							 IN DTA1XX_IPRX_ATTACH_CTRL*, OUT DTA1XX_IPRX_ATTACH_RTRN*);
NTSTATUS  Dta1xxNwIpRxClearFlags(IN PDTA1XX_FDO, IN UserIpRxChannel*,
								 IN DTA1XX_IPRX_FLAGS_CTRL*, OUT DTA1XX_IPRX_FLAGS_RTRN*);
NTSTATUS  Dta1xxNwIpRxDetach(IN PDTA1XX_FDO, IN UserIpRxChannel*,
							 IN DTA1XX_IPRX_DETACH_CTRL*, OUT DTA1XX_IPRX_DETACH_RTRN*);
UserIpRxChannel*  Dta1xxNwIpRxGetChannelUnsafe(IN PDTA1XX_FDO, IN Int);
NTSTATUS  Dta1xxNwIpRxGetFifoLoad(IN PDTA1XX_FDO, IN UserIpRxChannel*,
								  IN DTA1XX_IPRX_GETFIFOLOAD_CTRL*,
								  OUT DTA1XX_IPRX_GETFIFOLOAD_RTRN*);
NTSTATUS  Dta1xxNwIpRxGetFlags(IN PDTA1XX_FDO, IN UserIpRxChannel*,
							   IN DTA1XX_IPRX_FLAGS_CTRL*, OUT DTA1XX_IPRX_FLAGS_RTRN*);
NTSTATUS  Dta1xxNwIpRxGetStatus(IN PDTA1XX_FDO, IN UserIpRxChannel*,
								IN DTA1XX_IPRX_GETSTATUS_CTRL*,
								OUT DTA1XX_IPRX_GETSTATUS_RTRN*);
NTSTATUS  Dta1xxNwIpRxGetTsRate(IN PDTA1XX_FDO, IN UserIpRxChannel*,
								IN DTA1XX_IPRX_GETTSRATE_CTRL*,
								OUT DTA1XX_IPRX_GETTSRATE_RTRN*);
NTSTATUS  Dta1xxNwIpRxRead(IN PDTA1XX_FDO, IN UserIpRxChannel*,
						   IN DTA1XX_IPRX_READ_CTRL*, OUT DTA1XX_IPRX_READ_RTRN*);
void  Dta1xxNwIpRxRemoveChannel(IN PDTA1XX_FDO, IN UserIpRxChannel*);
NTSTATUS  Dta1xxNwIpRxReset(IN PDTA1XX_FDO, IN UserIpRxChannel*,
						    IN DTA1XX_IPRX_RESET_CTRL*, OUT DTA1XX_IPRX_RESET_RTRN*);
void Dta1xxNwIpRxResetRtpBuffers(IN  UserIpRxChannel*  pIpRx);
NTSTATUS  Dta1xxNwIpRxGetIpPars(IN PDTA1XX_FDO, IN UserIpRxChannel*,
								IN DTA1XX_IPRX_IPPARS_CTRL*,
								OUT DTA1XX_IPRX_IPPARS_RTRN*);
NTSTATUS  Dta1xxNwIpRxSetIpPars(IN PDTA1XX_FDO, IN UserIpRxChannel*,
								IN DTA1XX_IPRX_IPPARS_CTRL*,
								OUT DTA1XX_IPRX_IPPARS_RTRN*);
NTSTATUS  Dta1xxNwIpRxSetRxControl(IN PDTA1XX_FDO, IN UserIpRxChannel*,
								   IN DTA1XX_IPRX_SETRXCONTROL_CTRL*,
								   OUT DTA1XX_IPRX_SETRXCONTROL_RTRN*);
NTSTATUS  Dta1xxNwIpRxSetRxMode(IN PDTA1XX_FDO, IN UserIpRxChannel*,
								IN DTA1XX_IPRX_SETRXMODE_CTRL*,
								OUT DTA1XX_IPRX_SETRXMODE_RTRN*);
NTSTATUS  Dta1xxNwIpRxGetIpStat(IN  PDTA1XX_FDO,
								IN  UserIpRxChannel*,
								IN  DTA1XX_IPRX_IPSTAT_CTRL*,
								OUT DTA1XX_IPRX_IPSTAT_RTRN*);
void  Dta1xxNwIpRxChannelDestructor(IN PDTA1XX_FDO, IN UserIpRxChannel*);
void  Dta1xxNwIpRxChannelInit(IN PDTA1XX_FDO, IN UserIpRxChannel*, IN Int RxMode);
void  Dta1xxNwIpRxBrmProcessSample(IN UserIpRxChannel*, IN UInt64);
UInt16 Dta1xxNwIpRxGetMaxOutOfSync(IN UserIpRxChannel* pIpRx); 
#ifdef WINBUILD
void Dta1xxNwIpRxReconstructThread(IN PDTA1XX_FDO  pFdoExt);
#else
int Dta1xxNwIpRxReconstructThread(IN void* pData);
#endif
RtpListEntry*  Dta1xxNwIpRxRtpGetFreeEntry(IN UserIpRxChannel* pIpRx);
BOOLEAN  Dta1xxNwIpRxRtpAddEntryToList(IN UserIpRxChannel* pIpRx, 
									   IN PLIST_ENTRY pRtpListHead,
									   IN RtpListEntry* pRtpEntry);
RtpListEntry* Dta1xxNwIpRxRtpGetEntry(IN  UserIpRxChannel*  pIpRx, 
									  IN  PLIST_ENTRY   pRtpListHead);
void Dta1xxNwIpRxRtpSetEntryToEmptyList(IN  UserIpRxChannel*  pIpRx,
										RtpListEntry* pRtpEntry);
#ifdef WINBUILD
void  Dta1xxNwIpRxWorkerThread(IN NwStore* pNwStore);
#else
int  Dta1xxNwIpRxWorkerThread(IN void* pData);
#endif
void  Dta1xxNwIpRxProcessDvbWithoutFEC(IN  UserIpRxChannel*  pIpRx);
BOOLEAN  Dta1xxNwIpRxReconstructPacket(IN  UserIpRxChannel*  pIpRx,
									   OUT RtpListEntry* pRtpDestEntry, 
									   IN RtpListEntry* pRtpFecEntry, 
									   IN RtpListEntry* pReconstructList[MAX_FEC_RECONSTR_ELEMENTS]);
void  Dta1xxNwIpRxProcessDvbWithFEC(IN  UserIpRxChannel*  pIpRx);
void  Dta1xxNwIpRxRemoveOldFecPackets(IN UserIpRxChannel*  pIpRx,
									  IN LIST_ENTRY* pFecList,
									  IN UInt16 LastSequenceNumber);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IP RX IOCTL +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Implementation of IOCTL_DTA1XX_IPRX_CTRL
//
NTSTATUS  Dta1xxNwIpRxIoctl(
	IN PDTA1XX_FDO  pFdoExt,	// Device extension
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
	UserIpRxChannel*  pIpRx = NULL;

	// Base size for input buffer is 3x Int located in DTA1XX_IP_RXTX_CTRL
	InReqSize = 3 * sizeof(Int);
	
	// Determine expected length of data buffers
	switch (pCtrl->m_IpRxTxIoctl)	{
		
	case IOCTL_DTA1XX_IPRXCTRL_ATTACH:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_ATTACH";
		InReqSize += sizeof(DTA1XX_IPRX_ATTACH_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_ATTACH_RTRN);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_CLEARFLAGS:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_CLEARFLAGS";
		InReqSize += sizeof(DTA1XX_IPRX_FLAGS_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_FLAGS_RTRN);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_DETACH:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_DETACH";
		InReqSize += sizeof(DTA1XX_IPRX_DETACH_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_DETACH_RTRN);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_GETFIFOLOAD:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_GETFIFOLOAD";
		InReqSize += sizeof(DTA1XX_IPRX_GETFIFOLOAD_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_GETFIFOLOAD_RTRN);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_GETFLAGS:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_GETFLAGS";
		InReqSize += sizeof(DTA1XX_IPRX_FLAGS_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_FLAGS_RTRN);
		break;
	
	case IOCTL_DTA1XX_IPRXCTRL_GETIPPARS:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_GETIPPARS";
		InReqSize += sizeof(DTA1XX_IPRX_IPPARS_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_IPPARS_RTRN);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_GETIPSTAT:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_GETIPSTAT";
		InReqSize += sizeof(DTA1XX_IPRX_IPSTAT_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_IPSTAT_RTRN);
		break;
	
	case IOCTL_DTA1XX_IPRXCTRL_GETSTATUS:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_GETSTATUS";
		InReqSize += sizeof(DTA1XX_IPRX_GETSTATUS_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_GETSTATUS_RTRN);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_GETTSRATE:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_GETTSRATE";
		InReqSize += sizeof(DTA1XX_IPRX_GETTSRATE_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_GETTSRATE_RTRN);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_READ:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_READ";
		InReqSize += sizeof(DTA1XX_IPRX_READ_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_READ_RTRN);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_RESET:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_RESET";
		InReqSize += sizeof(DTA1XX_IPRX_RESET_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_RESET_RTRN);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_SETIPPARS:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_SETIPPARS";
		InReqSize += sizeof(DTA1XX_IPRX_IPPARS_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_IPPARS_RTRN);
		break;

	case IOCTL_DTA1XX_IPRXCTRL_SETRXCONTROL:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_SETRXCONTROL";
		InReqSize += sizeof(DTA1XX_IPRX_SETRXCONTROL_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_SETRXCONTROL_RTRN);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_SETRXMODE:
		IoctlStr = "IOCTL_DTA1XX_IPRXCTRL_SETRXMODE";
		InReqSize += sizeof(DTA1XX_IPRX_SETRXMODE_CTRL);
		OutReqSize = sizeof(DTA1XX_IPRX_SETRXMODE_RTRN);
		break;

	default:
		IoctlStr = "??UNKNOWN IPRXIOCTL CODE??";
	}

	// Check buffer sizes
	if (InBufferSize < InReqSize) {
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxIoctl: %s, INPUT BUFFER TOO SMALL Size=%u Req=%u",
				 IoctlStr, InBufferSize, InReqSize);
		return STATUS_INVALID_PARAMETER;
	}
	if (OutBufferSize < OutReqSize) {
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxIoctl: OUTPUT BUFFER TOO SMALL!");
		return STATUS_INVALID_PARAMETER;
	}
#if LOG_LEVEL_IPRX > 1
	DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxIoctl: %s, In=%lu (Rq=%lu), Out=%lu (Rq=%lu)",
			 IoctlStr, InBufferSize, InReqSize, OutBufferSize, OutReqSize);
#endif

	// Check channel index
	if (pCtrl->m_ChannelIndex<0) {
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxIoctl: ChannelIndex=%d IS OUT OF RANGE!!",
				 pCtrl->m_ChannelIndex);
		return STATUS_INVALID_PARAMETER;
	}

	// Compute IP Rx channel pointer
	pIpRx = Dta1xxNwIpRxGetChannel(pFdoExt, pCtrl->m_ChannelIndex);

	// Verify that channel exists
	if (  pIpRx==NULL
	   && pCtrl->m_IpRxTxIoctl!=IOCTL_DTA1XX_IPRXCTRL_ATTACH) {
		DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxIoctl: CHANNEL POINTER IS NULL!!",
				 pCtrl->m_ChannelIndex);
		return STATUS_INVALID_PARAMETER;
	}

	//-.-.-.-.-.-.-.-.-.-.-.- Big Switch to Execute The IpRx IOCTL -.-.-.-.-.-.-.-.-.-.-.-

	switch (pCtrl->m_IpRxTxIoctl)	{
		
	case IOCTL_DTA1XX_IPRXCTRL_ATTACH:
		ReturnStatus = Dta1xxNwIpRxAttach(
							pFdoExt, pCtrl->m_IpPortIndex, pFileObject,
							&pCtrl->m_Data.m_AttachCtrl, &pRtrn->m_Data.m_AttachRtrn);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_CLEARFLAGS:
		ReturnStatus = Dta1xxNwIpRxClearFlags(
							pFdoExt, pIpRx,
							&pCtrl->m_Data.m_FlagsCtrl, &pRtrn->m_Data.m_FlagsRtrn);
		break;

	case IOCTL_DTA1XX_IPRXCTRL_DETACH:
		ReturnStatus = Dta1xxNwIpRxDetach(
							pFdoExt, pIpRx,
							&pCtrl->m_Data.m_DetachCtrl, &pRtrn->m_Data.m_DetachRtrn);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_GETFIFOLOAD:
		ReturnStatus = Dta1xxNwIpRxGetFifoLoad(
							pFdoExt, pIpRx,
							&pCtrl->m_Data.m_GetFifoLoadCtrl,
							&pRtrn->m_Data.m_GetFifoLoadRtrn);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_GETFLAGS:
		ReturnStatus = Dta1xxNwIpRxGetFlags(
							pFdoExt, pIpRx,
							&pCtrl->m_Data.m_FlagsCtrl, &pRtrn->m_Data.m_FlagsRtrn);
		break;
	
	case IOCTL_DTA1XX_IPRXCTRL_GETIPPARS:
		ReturnStatus = Dta1xxNwIpRxGetIpPars(
							pFdoExt, pIpRx,
							&pCtrl->m_Data.m_IpParsCtrl, &pRtrn->m_Data.m_IpParsRtrn);
		break;

	case IOCTL_DTA1XX_IPRXCTRL_GETIPSTAT:
		ReturnStatus = Dta1xxNwIpRxGetIpStat(
							pFdoExt, pIpRx,
							&pCtrl->m_Data.m_IpStatCtrl, &pRtrn->m_Data.m_IpStatRtrn);
		break;

	case IOCTL_DTA1XX_IPRXCTRL_GETSTATUS:
		ReturnStatus = Dta1xxNwIpRxGetStatus(
							pFdoExt, pIpRx,
							&pCtrl->m_Data.m_GetStatusCtrl,
							&pRtrn->m_Data.m_GetStatusRtrn);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_GETTSRATE:
		ReturnStatus = Dta1xxNwIpRxGetTsRate(
							pFdoExt, pIpRx,
							&pCtrl->m_Data.m_GetTsRateCtrl,
							&pRtrn->m_Data.m_GetTsRateRtrn);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_READ:
		ReturnStatus = Dta1xxNwIpRxRead(
							pFdoExt, pIpRx,
							&pCtrl->m_Data.m_ReadCtrl, &pRtrn->m_Data.m_ReadRtrn);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_RESET:
		ReturnStatus = Dta1xxNwIpRxReset(
							pFdoExt, pIpRx,
							&pCtrl->m_Data.m_ResetCtrl, &pRtrn->m_Data.m_ResetRtrn);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_SETIPPARS:
		ReturnStatus = Dta1xxNwIpRxSetIpPars(
							pFdoExt, pIpRx,
							&pCtrl->m_Data.m_IpParsCtrl, &pRtrn->m_Data.m_IpParsRtrn);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_SETRXCONTROL:
		ReturnStatus = Dta1xxNwIpRxSetRxControl(
							pFdoExt, pIpRx,
							&pCtrl->m_Data.m_SetRxControlCtrl,
							&pRtrn->m_Data.m_SetRxControlRtrn);
		break;
		
	case IOCTL_DTA1XX_IPRXCTRL_SETRXMODE:
		ReturnStatus = Dta1xxNwIpRxSetRxMode(pFdoExt, pIpRx,
							&pCtrl->m_Data.m_SetRxModeCtrl,
							&pRtrn->m_Data.m_SetRxModeRtrn);
		break;

	default:
		ReturnStatus = STATUS_NOT_IMPLEMENTED;
	}

	return ReturnStatus;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IP RX IOCTL IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// PRE-CONDITION for all IOCTL implementation routines:
//	- ChannelIndex is valid (except for ATTACH)
//	- Channel exists (except for ATTACH)


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxAttach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Implement IOCTL to attach to an IP Rx channel.
//
NTSTATUS  Dta1xxNwIpRxAttach(
	IN  PDTA1XX_FDO  pFdoExt,					// Device extension
	IN  Int  PortIndex,							// IP port index
	IN PFILEOBJECT  pFileObject,				// Current process
	IN  DTA1XX_IPRX_ATTACH_CTRL*  pAttachCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_ATTACH_RTRN*  pAttachRtrn)	// IOCTL return value
{
	Int  Idx;
	SIZE_T  AllocSize = sizeof(UserIpRxChannel);
	UserIpRxChannel* pIpRx;						// Next element
	UserIpRxChannel* pIpRxPrev = NULL;			// Previous element
	UserIpRxChannel* pIpRxNew;					// New element
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success
	KIRQL  OldIrql;								// OLD IRQ level
	
	// Allocate and initialise UserIpRxChannel before locks are acquired
	pIpRxNew = ExAllocatePoolWithTag(NonPagedPool, AllocSize, '1ATD');
	if (pIpRxNew == NULL) {
		
		DTA1XX_LOG(KERN_INFO, "[IpRx??] Dta1xxNwIpRxAttach: Allocation of "
							  "UserIpRxChannel FAILS");
		return STATUS_INSUFFICIENT_RESOURCES;
	}
		
	RtlZeroMemory(pIpRxNew, AllocSize);
	
	// Identity
	pIpRxNew->m_IpPortIndex = PortIndex;
	pIpRxNew->m_FileObject = pFileObject;	// For purging in Close

	// Rx buffer initialisation
	pIpRxNew->m_SharedBuf.m_pBuf = NULL;
	pIpRxNew->m_SharedBuf.m_Allocated = FALSE;
	pIpRxNew->m_SharedBuf.m_PageList = NULL;
	pIpRxNew->m_SharedBuf.m_SglAllocated = FALSE;

	// Other initialisation
	pIpRxNew->m_IpParsValid = FALSE;

	// Initialise use count here to prevent resetting usecount when parsing ethernet packets
	atomic_set(&pIpRxNew->m_UseCount, 0);
	
	Dta1xxNwIpRxChannelInit(pFdoExt, pIpRxNew, DTA1XX_RXMODE_ST188);

	// Protect attachment code by fast mutex
	ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex);
	
	// Protect attachment code by fast mutex2
	ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex2);

	// Protect attachment code by m_IpRxBrmSpinlock and BOOLEAN
	// Remark: The spinlock can not be held when we use the Dta1xxWaitMs function
	// It's not used in the attach, but we do the same also here
	KeAcquireSpinLock(&pFdoExt->m_IpRxBrmSpinlock, &OldIrql);
	pFdoExt->m_IpRxBrmSkip = TRUE;
	KeReleaseSpinLock(&pFdoExt->m_IpRxBrmSpinlock, OldIrql);
	
	pIpRx= pFdoExt->m_pIpRx;

	// Search for first free index number.
	Idx = 0;
	while (pIpRx) {
		if (Idx != pIpRx->m_ChannelIndex)
			break;
		Idx++;
		pIpRxPrev = pIpRx;
		pIpRx = pIpRx->m_pNext;
	}

	// We now know the index value
	pIpRxNew->m_ChannelIndex = pAttachRtrn->m_ChannelIndex = Idx;
	
	// Now insert new element into linked list
	if (pFdoExt->m_pIpRx) {
		// There's already an element available
		if (pIpRx) {
			// Insert element before pIpRx
			if (pIpRx->m_pPrev) {
				// Not first element
				pIpRx->m_pPrev->m_pNext = pIpRxNew;
				pIpRxNew->m_pPrev = pIpRx->m_pPrev;
				
			} else {
				// First element
				pFdoExt->m_pIpRx = pIpRxNew;
				pIpRxNew->m_pPrev = NULL;
			}
			pIpRx->m_pPrev = pIpRxNew;
			pIpRxNew->m_pNext = pIpRx;
			
		} else {
			// Insert element at end
			ASSERT(pIpRxPrev->m_pNext == NULL);
			pIpRxPrev->m_pNext = pIpRxNew;
			pIpRxNew->m_pPrev = pIpRxPrev;
			pIpRxNew->m_pNext = NULL;
		}
	} else {
		pIpRxNew->m_pNext = NULL;
		pIpRxNew->m_pPrev = NULL;
		pFdoExt->m_pIpRx = pIpRxNew;
	}
	
	// We're done, release
	KeAcquireSpinLock(&pFdoExt->m_IpRxBrmSpinlock, &OldIrql);
	pFdoExt->m_IpRxBrmSkip = FALSE;
	KeReleaseSpinLock(&pFdoExt->m_IpRxBrmSpinlock, OldIrql);
	ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex2);
	ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex);

	return ReturnStatus;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxClearFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Clear latched status flags.
// Two flags are supported: DTAPI_RX_FIFO_OVF and DTAPI_RX_SYNC_ERR.
//
NTSTATUS  Dta1xxNwIpRxClearFlags(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_FLAGS_CTRL*  pFlagsCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_FLAGS_RTRN*  pFlagsRtrn)	// IOCTL return value
{
	KIRQL  OldIrqL;								// OLD IRQ level
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

	// Clear flags specified in IOCTL
	KeAcquireSpinLock(&pIpRx->m_FlagsSpinLock, &OldIrqL);
	pIpRx->m_LatchedFlags &= ~pFlagsCtrl->m_Flags;
	KeReleaseSpinLock(&pIpRx->m_FlagsSpinLock, OldIrqL);

#if LOG_LEVEL_IPRX > 1
	DTA1XX_LOG( KERN_INFO,
				"[IpRx%d] Dta1xxNwIpRxClearFlags: Flags=0x%x Latched=0x%x",
				pIpRx->m_ChannelIndex, pIpRx->m_Flags, pIpRx->m_LatchedFlags );
#endif

	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxDetach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Implement IOCTL to detach from an IP Rx channel.
//
NTSTATUS  Dta1xxNwIpRxDetach(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_DETACH_CTRL*  pDetachCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_DETACH_RTRN*  pDetachRtrn)	// IOCTL return value
{
	KIRQL OldIrql;

#if LOG_LEVEL_IPRX > 0
	Int  ChannelIndex = pIpRx->m_ChannelIndex;
#endif
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

	// Protect detachment code by fast mutex
	ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex);
	
	// Protect detachment code by fast mutex2
	ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex2);
	
	// Protect attachment code by m_IpRxBrmSpinlock and BOOLEAN
	// Remark: The spinlock can not be held when we use the Dta1xxWaitMs function
	KeAcquireSpinLock(&pFdoExt->m_IpRxBrmSpinlock, &OldIrql);
	pFdoExt->m_IpRxBrmSkip = TRUE;
	KeReleaseSpinLock(&pFdoExt->m_IpRxBrmSpinlock, OldIrql);

	// The m_UseCount is checked in Dta1xxNwIpRxChannelDestructor 

	// Free memory, after this function pIpRx is invalid
	Dta1xxNwIpRxChannelDestructor(pFdoExt, pIpRx);
	pIpRx = NULL;

	// We're done, release 
	KeAcquireSpinLock(&pFdoExt->m_IpRxBrmSpinlock, &OldIrql);
	pFdoExt->m_IpRxBrmSkip = FALSE;
	KeReleaseSpinLock(&pFdoExt->m_IpRxBrmSpinlock, OldIrql);
	ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex2);
	ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex);

#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxDetach returns with status %lu",
			 ChannelIndex, ReturnStatus);
#endif

	return ReturnStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxGetFifoLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get FIFO load. Implemented by subtracting write pointer from read pointer.
//
NTSTATUS  Dta1xxNwIpRxGetFifoLoad(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_GETFIFOLOAD_CTRL*  pGetFifoLoadCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_GETFIFOLOAD_RTRN*  pGetFifoLoadRtrn)	// IOCTL return value
{
	UInt8*  pRead;				// Read pointer
	UInt8*  pWrite;				// Write pointer
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

	// Compute free buffer space
	// Take local copies of pointer for ease of reference, and to be sure they will not
	// be changed by another thread (for pRead only)
	pRead = (UInt8*)pIpRx->m_pRead;
	pWrite = (UInt8*)pIpRx->m_pWrite;
	pGetFifoLoadRtrn->m_FifoLoad = (Int)(pRead > pWrite ?
									pIpRx->m_BufSize - (pRead - pWrite) :
									pWrite - pRead);
#if LOG_LEVEL_IPRX > 2
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxGetFifoLoad: FifoLoad=%d pRead=0x%p pWrite=0x%p",
			 pIpRx->m_ChannelIndex, pGetFifoLoadRtrn->m_FifoLoad, pRead, pWrite);
#endif

	return ReturnStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxGetFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get status flags (DTAPI_RX_FIFO_OVF, DTAPI_RX_SYNC_ERR)
// Status flags are updated by worker thread
//
NTSTATUS  Dta1xxNwIpRxGetFlags(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_FLAGS_CTRL*  pFlagsCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_FLAGS_RTRN*  pFlagsRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

	// Copy flags to IOCTL return value
	pFlagsRtrn->m_Flags = pIpRx->m_Flags;
	pFlagsRtrn->m_LatchedFlags = pIpRx->m_LatchedFlags;

#if LOG_LEVEL_IPRX > 1
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxGetFlags: Flags=0x%x Latched=0x%x",
			 pIpRx->m_ChannelIndex, pFlagsRtrn->m_Flags, pFlagsRtrn->m_LatchedFlags);
#endif

	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxGetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get input status: packet size and clock detect
// Input status is updated by worker thread
//
NTSTATUS  Dta1xxNwIpRxGetStatus(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_GETSTATUS_CTRL*  pGetStatusCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_GETSTATUS_RTRN*  pGetStatusRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

	// Received packet size
	if (pIpRx->m_PckSizeSrc == 188)
		pGetStatusRtrn->m_PacketSize = DTA1XX_PCKSIZE_188;
	else if (pIpRx->m_PckSizeSrc == 204)
		pGetStatusRtrn->m_PacketSize = DTA1XX_PCKSIZE_204;
	else
		pGetStatusRtrn->m_PacketSize = DTA1XX_PCKSIZE_INV;

	// ClkDet
	pGetStatusRtrn->m_ClkDet = (pIpRx->m_BrmEstimate == 0 ? 0 : 1);

#if LOG_LEVEL_IPRX > 1
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxGetStatus: PacketSize=%d (0=INV;2=188;3=204) "
			 "ClkDet=%d", pIpRx->m_ChannelIndex,
			 pGetStatusRtrn->m_PacketSize, pGetStatusRtrn->m_ClkDet);
#endif

	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxGetTsRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get (estimate of) Transport-Stream rate, as computed by worker thread
//
NTSTATUS  Dta1xxNwIpRxGetTsRate(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_GETTSRATE_CTRL*  pGetTsRateCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_GETTSRATE_RTRN*  pGetTsRateRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

	pGetTsRateRtrn->m_TsRate = pIpRx->m_BrmEstimate;

#if LOG_LEVEL_IPRX > 1
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxGetTsRate: TsRate=%d",
			 pIpRx->m_ChannelIndex, pGetTsRateRtrn->m_TsRate);
#endif

	return ReturnStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS  Dta1xxNwIpRxRead(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_READ_CTRL*  pReadCtrl,		// IOCTL input data
	OUT DTA1XX_IPRX_READ_RTRN*  pReadRtrn)		// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

#if LOG_LEVEL_IPRX > 1
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxRead: Cmd=%d #bytes=%d",
			 pIpRx->m_ChannelIndex, pReadCtrl->m_ReadCmd, pReadCtrl->m_NumBytesToRead);
#endif

	// Decode read command
	switch (pReadCtrl->m_ReadCmd) {

	case 1:
		// Get read index
		pReadRtrn->m_ReadIdx = (Int)(pIpRx->m_pRead - pIpRx->m_SharedBuf.m_pBuf);
#if LOG_LEVEL_IPRX > 1
		DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxRead: Cmd=1, returns ReadIdx=%d",
				 pIpRx->m_ChannelIndex, pReadRtrn->m_ReadIdx);
#endif
		break;

	case 2:
		// Update read pointer
		pIpRx->m_pRead += pReadCtrl->m_NumBytesToRead;
		if (pIpRx->m_pRead >= pIpRx->m_pEndOfBuffer)
			pIpRx->m_pRead -= pIpRx->m_BufSize;
#if LOG_LEVEL_IPRX > 1
		DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxRead, Cmd=2 NumBytesToRead=%d "
				 "m_pRead=0x%p", pIpRx->m_ChannelIndex,
				 pReadCtrl->m_NumBytesToRead, pIpRx->m_pRead);
#endif
		break;

	default:
		DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxRead ILLEGAL COMMAND %d",
				 pIpRx->m_ChannelIndex, pReadCtrl->m_ReadCmd);
	}
	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Reset IP Rx channel
//
NTSTATUS  Dta1xxNwIpRxReset(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_RESET_CTRL*  pResetCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_RESET_RTRN*  pResetRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success
	DTA1XX_IPRX_SETRXCONTROL_CTRL  SetRxCtrlCtrl;
	DTA1XX_IPRX_SETRXCONTROL_RTRN  SetRxCtrlRtrn;

#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxReset: ResetMode=%d",
			 pIpRx->m_ChannelIndex, pResetCtrl->m_ResetMode);
#endif

	// Reset Receive-Control state to IDLE
	SetRxCtrlCtrl.m_RxControl = DTA1XX_RXCTRL_IDLE;
	Dta1xxNwIpRxSetRxControl(pFdoExt, pIpRx, &SetRxCtrlCtrl, &SetRxCtrlRtrn);

	// Re-initialise channel. A reset we should leave te rx-mode untouched
	Dta1xxNwIpRxChannelInit(pFdoExt, pIpRx, pIpRx->m_RxMode);

	if (pIpRx->m_SharedBuf.m_Allocated) {
		Dta1xxNwIpRxResetRtpBuffers(pIpRx);
	}
	
	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxGetIpPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get IP-level parametersg
//
NTSTATUS  Dta1xxNwIpRxGetIpPars(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_IPPARS_CTRL*  pIpParsCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_IPPARS_RTRN*  pIpParsRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success
	
	pIpParsRtrn->m_FecNumCols = pIpRx->m_DetFecNumColumns;
	pIpParsRtrn->m_FecNumRows = pIpRx->m_DetFecNumRows;
    pIpParsRtrn->m_NumTpPerIp = pIpRx->m_DetNumTpPerIp;
	pIpParsRtrn->m_Protocol   = pIpRx->m_DetProtocol;
	
#if LOG_LEVEL_IPRX > 1
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxGetIpPars: NumCols=%d; NumRows=%d; "
			  "NumTpPerIp=%d; m_Protocol=%d",
			 pIpRx->m_ChannelIndex, pIpParsRtrn->m_FecNumCols, pIpParsRtrn->m_FecNumRows,
			 pIpParsRtrn->m_NumTpPerIp, pIpParsRtrn->m_Protocol);
#endif

	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxGetIpStat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get IP-level statistics
//
NTSTATUS  Dta1xxNwIpRxGetIpStat(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_IPSTAT_CTRL*  pIpStatCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_IPSTAT_RTRN*  pIpStatRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success
	
	KIRQL OldIrql;

	KeAcquireSpinLock(&pIpRx->m_StatisticsSpinLock, &OldIrql);
	pIpStatRtrn->m_TotNumIPPackets = pIpRx->m_TotNumPackets;
	pIpStatRtrn->m_LostIPPacketsAfterFec = pIpRx->m_NumPacketsNotReconstructed;
	pIpStatRtrn->m_LostIPPacketsBeforeFec = pIpRx->m_NumPacketsNotReconstructed +
										    pIpRx->m_NumPacketsReconstructed;
	
	// TO DO!!!!
	pIpStatRtrn->m_LostIPPacketsAfterSort = 0;
	pIpStatRtrn->m_LostIPPacketsBeforeSort = 0;

	pIpRx->m_NumPacketsNotReconstructed = 0;
	pIpRx->m_NumPacketsReconstructed = 0;
	pIpRx->m_TotNumPackets = 0;

	KeReleaseSpinLock(&pIpRx->m_StatisticsSpinLock, OldIrql);


#if LOG_LEVEL_IPRX > 1
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxGetIpStat: TotNumPkt=%d;"
			 "LostIPPktAfterFEC=%d; LostIPPktBeforeFEC=%d; "
			 "NumLPAfterSort=%d; NumLPBeforeSort=%d",
			 pIpRx->m_ChannelIndex, pIpStatRtrn->m_TotNumIPPackets,
			 pIpStatRtrn->m_LostIPPacketsAfterFec, pIpStatRtrn->m_LostIPPacketsBeforeFec,
			 pIpStatRtrn->m_LostIPPacketsAfterSort, pIpStatRtrn->m_LostIPPacketsBeforeSort);
#endif

	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxSetIpPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set IP-level parameters
//
NTSTATUS  Dta1xxNwIpRxSetIpPars(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_IPPARS_CTRL*  pIpParsCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_IPPARS_RTRN*  pIpParsRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success
	
	Int i;

	// Only allowed in Receive-Control state IDLE
	if (pIpRx->m_RxControl != DTA1XX_RXCTRL_IDLE) {
#if LOG_LEVEL_IPRX > 0
		DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxSetIpPars NOT ALLOWED BECAUSE "
				 "RX CONTROL IS %d", pIpRx->m_ChannelIndex, pIpRx->m_RxControl);
#endif
		return STATUS_UNSUCCESSFUL;
	}

	pIpRx->m_SrcPort = pIpParsCtrl->m_SrcPort;
	pIpRx->m_DstPort = pIpParsCtrl->m_DstPort;

	for (i = 0; i < 4; i++) {
		pIpRx->m_SrcIPAddress[i] = pIpParsCtrl->m_SrcIp[i];
		pIpRx->m_DstIPAddress[i] = pIpParsCtrl->m_DstIp[i];
	}

	pIpRx->m_FecMode = pIpParsCtrl->m_FecMode;
	pIpRx->m_IpParsValid = TRUE;

#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxSetIpPars: SrcPort=%d; DstPort=%d " \
			"SrcIP: %d.%d.%d.%d DstIP: %d.%d.%d.%d",
			pIpRx->m_ChannelIndex, pIpParsCtrl->m_SrcPort, pIpParsCtrl->m_DstPort,
			pIpRx->m_SrcIPAddress[0],pIpRx->m_SrcIPAddress[1],
			pIpRx->m_SrcIPAddress[2],pIpRx->m_SrcIPAddress[3],
			pIpRx->m_DstIPAddress[0],pIpRx->m_DstIPAddress[1],
			pIpRx->m_DstIPAddress[2],pIpRx->m_DstIPAddress[3]);
#endif

	return ReturnStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxSetRxControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set Receive-Control state
//
NTSTATUS  Dta1xxNwIpRxSetRxControl(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_SETRXCONTROL_CTRL*  pSetRxControlCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_SETRXCONTROL_RTRN*  pSetRxControlRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxSetRxControl: RxControl=%d",
			 pIpRx->m_ChannelIndex, pSetRxControlCtrl->m_RxControl);
#endif

	// Going to RCV is easy
	if (pSetRxControlCtrl->m_RxControl == DTA1XX_RXCTRL_RCV) {
		if (!pIpRx->m_SharedBuf.m_Allocated) {
			DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxSetTxControl Rx buffer not allocated",
				pIpRx->m_ChannelIndex);
			return STATUS_UNSUCCESSFUL;
		}
		pIpRx->m_RxControl = DTA1XX_RXCTRL_RCV;
	} else {
		// Going to IDLE: if Worker thread happens to be writing a packet,
		// wait until it is finished

		pIpRx->m_RxControl = DTA1XX_RXCTRL_IDLE;
		ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex2);
	
		while (InterlockedIncrement(&pIpRx->m_UseCount) != 1) {
			InterlockedDecrement(&pIpRx->m_UseCount);
			// For single processor system with real time priority application
			// give some time to other threads to complete.
			Dta1xxWaitMs(1);
		}
		InterlockedDecrement(&pIpRx->m_UseCount);	// Set count back to 0

		ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex2);
	}

	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxSetRxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set Receive Mode
//
NTSTATUS  Dta1xxNwIpRxSetRxMode(
	IN  PDTA1XX_FDO  pFdoExt,			// Device extension
	IN  UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN  DTA1XX_IPRX_SETRXMODE_CTRL*  pSetRxModeCtrl,	// IOCTL input data
	OUT DTA1XX_IPRX_SETRXMODE_RTRN*  pSetRxModeRtrn)	// IOCTL return value
{
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;	// Assume success

#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxSetRxMode: RxMode=%d",
			 pIpRx->m_ChannelIndex, pIpRx->m_RxMode);
#endif

	// Can change Receive Mode only if Receive-Control state is IDLE
	if (pIpRx->m_RxControl != DTA1XX_RXCTRL_IDLE)
		return STATUS_UNSUCCESSFUL;

	// Set Rx Mode
	pIpRx->m_RxMode = pSetRxModeCtrl->m_RxMode;

	return ReturnStatus;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IP RX IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxChannelDestructor -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Free resources used by IP Rx channel object
// Pre: The pFdoExt->m_IpRxFastMutex2 must be locked
//
void  Dta1xxNwIpRxChannelDestructor(
	IN PDTA1XX_FDO  pFdoExt,		// Device extension
	IN UserIpRxChannel*  pIpRx)				// IP Rx channel
{
#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxChannelDestructor", pIpRx->m_ChannelIndex);
#endif

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Deactivate Worker Thread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	// When Worker thread happens to be writing a packet (or measurement data), wait
	// until it is finished. 

	// m_IpRxFastMutex2 must be locked before the destructor is executed, so skip this here
	//KeWaitForSingleObject(&pFdoExt->m_IpRxMutex, Executive, KernelMode, FALSE, NULL);

	while (InterlockedIncrement(&pIpRx->m_UseCount) != 1) {
		InterlockedDecrement(&pIpRx->m_UseCount);
		// For single processor system with real time priority application
		// give some time to other threads to complete.
		Dta1xxWaitMs(1);
	}
	
	// Free the Receive FIFO buffer
	Dta1xxNwIpRxReleaseBuf(pFdoExt, pIpRx);

	// Remove channel from linked list
	Dta1xxNwIpRxRemoveChannel(pFdoExt, pIpRx);		
	
	// Free IP Rx channel object
#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxChannelDestructor: Free channel object",
			 pIpRx->m_ChannelIndex);
#endif
	ExFreePool(pIpRx);
	//KeReleaseMutex(&pFdoExt->m_IpRxMutex, FALSE);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxChannelInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Initialise state variables in IP Rx channel.
// Identity variables are NOT initialised here, but in Attach. 
//
void Dta1xxNwIpRxChannelInit(
	IN PDTA1XX_FDO  pFdoExt,		// Device extension
	IN UserIpRxChannel*  pIpRx,				// IP Rx channel
	IN Int RxMode)							// Initial value for receive-mode

{
#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxChannelInit", pIpRx->m_ChannelIndex);
#endif

	// Buffer and buffer state
	pIpRx->m_pRead  = pIpRx->m_SharedBuf.m_pBuf;
	pIpRx->m_pWrite = pIpRx->m_SharedBuf.m_pBuf;
    
	// Handshaking state
	pIpRx->m_RxIncomingPackets = FALSE;
	pIpRx->m_RtpFirstPacket = TRUE;
		
	// FEC Statistics
	KeInitializeSpinLock(&pIpRx->m_StatisticsSpinLock);
	pIpRx->m_NumPacketsNotReconstructed = 0;
	pIpRx->m_NumPacketsReconstructed = 0;
	pIpRx->m_TotNumPackets = 0;

	// Fragmented packets
	pIpRx->m_FragmentId = 0;
	pIpRx->m_FragmentOffset = 0;
	
	// Receive control and receive state
	pIpRx->m_RxControl = DTA1XX_RXCTRL_IDLE;
	pIpRx->m_RxMode = RxMode;

	// Status flags and spin lock for access protection
	KeInitializeSpinLock(&pIpRx->m_FlagsSpinLock);
	pIpRx->m_Flags = 0;				// Clear DTA1XX_RX_FIFO_OVF, DTA1XX_RX_SYNC_ERR
	pIpRx->m_LatchedFlags = 0;
	pIpRx->m_PckSizeSrc = 0;		// UNKNOWN
	pIpRx->m_TrpModePckSeqCount = 0;	// Start with sequence number zero

	// Bit-Rate Management
	pIpRx->m_NumPckBytesRcvd = 0;
	pIpRx->m_BrmNumSamples = 0;
	pIpRx->m_BrmIdxLastSample = 0;
	pIpRx->m_BrmEstimate = 0;

	// Detection status bytes
	pIpRx->m_DetFecNumRows = 0;
	pIpRx->m_DetFecNumColumns = 0;
	pIpRx->m_DetNumTpPerIp = 0;
	pIpRx->m_DetProtocol = DTA1XX_IP_PROTO_UNKNOWN;
	pIpRx->m_RstCntFecRow = 0;
	pIpRx->m_RstCntFecColumn = 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxPurgeLeftOverChannels -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This routine is called after a CLOSE, to purge IP Rx channels that have not been
// deleted neatly.
//
void  Dta1xxNwIpRxPurgeLeftOverChannels(
	IN PDTA1XX_FDO  pFdoExt,				// Device extension
	IN PFILEOBJECT  pFileObject)			// Current process
{
	UserIpRxChannel*  pIpRx;
	UserIpRxChannel*  pIpRxTmp;
	KIRQL OldIrql;
	
	if (pFdoExt->m_pIpRx == NULL)
		return;

#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxPurgeLeftOverChannels");
#endif

	// Loop through all IP Rx channels, check whether they belong to the current session
	// (by checking FileObject) and if so delete them
	
	// Wait until the Rx structure is available
	ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex);
	ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex2);
	// Protect attachment code by m_IpRxBrmSpinlock and BOOLEAN
	// Remark: The spinlock can not be held when we use the Dta1xxWaitMs function
	KeAcquireSpinLock(&pFdoExt->m_IpRxBrmSpinlock, &OldIrql);
	pFdoExt->m_IpRxBrmSkip = TRUE;
	KeReleaseSpinLock(&pFdoExt->m_IpRxBrmSpinlock, OldIrql);

	pIpRx = pFdoExt->m_pIpRx;
	
	// The m_UseCount is checked in Dta1xxNwIpRxChannelDestructor 
	while (pIpRx) {
        if (pIpRx->m_FileObject==pFileObject) {
#if LOG_LEVEL_IPRX > 0
			DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxPurgeLeftOverChannels: Purge IP Rx "
					 "channel #%d", pIpRx->m_ChannelIndex);
#endif
			pIpRxTmp = pIpRx->m_pNext;

			// Free memory, after this function pIpRx is invalid
			Dta1xxNwIpRxChannelDestructor(pFdoExt, pIpRx);
			pIpRx = NULL;

			pIpRx = pIpRxTmp;
		} else
			pIpRx = pIpRx->m_pNext;
	}
	KeAcquireSpinLock(&pFdoExt->m_IpRxBrmSpinlock, &OldIrql);
	pFdoExt->m_IpRxBrmSkip = FALSE;
	KeReleaseSpinLock(&pFdoExt->m_IpRxBrmSpinlock, OldIrql);
	ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex2);
	ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DATA PROCESSING +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxProcessDvbPayLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Process the payload of an IP packet for a given IP Rx channel.
//
// NOTE:
// This routine must be called even if Receive-Control state is IDLE, so that bit-rate
// measurement and status-flag computation are active at all times.
//
void  Dta1xxNwIpRxProcessDvbPayLoad(
	IN UserIpRxChannel*  pIpRx,			// IP Rx channel
	IN UInt8*  pPayLoad,				// IP payload with packet data
	IN Int  PayLoadSize,				// Size of payload in #bytes
	IN UInt64  TimeStamp)				// Time stamp @54MHz
{
	Int  Add16Zeros = 0;		// Add 16 zero bytes
	Int  NumFree;				// Number of free bytes in buffer
	Int  NumPckts;				// Number of TPs in IP packet
	Int  PckSizeToCopy;			// #bytes to copy for each packet
	Int  PckSizeDst;			// Size of TPs (+opt. time stamp) written to buffer
	Int  SyncErr;				// Synchronisation Error
	UInt8*  pRead;				// Read pointer
	UInt8*  pWrite;				// Write pointer
	KIRQL  OldIrqL;				// OLD IRQ level
	UInt8  NumTpPerIp = 0;		// Calculated number of Tp packets per Ip.
	Int  PckSizeSrc;			// Size of TPs calculated from source buffer

	// Determine Transport-Packet size from IP-packet size
	if ((PayLoadSize % 188) == 0) {
		PckSizeSrc = 188;
		NumPckts = PayLoadSize / 188;
	} else if ((PayLoadSize % 204) == 0) {
		PckSizeSrc = 204;
		NumPckts = PayLoadSize / 204;
	} else {
		pIpRx->m_PckSizeSrc = -1;	// INVALID
		return;					// Not a valid IP packet
	}

	// Note: pIpRx->m_PckSizeSrc can be reset by the bitrate calculation thread
	pIpRx->m_PckSizeSrc = PckSizeSrc;
	
	// Update last Rx timestamp for bitrate calculations
	pIpRx->m_BrmLastRxTimeStamp = TimeStamp;
	
	// Update total #packet bytes received for bitrate calculations
	if ((pIpRx->m_RxMode & DTA1XX_RXMODE_MASK) == DTA1XX_RXMODE_STRAW) {
		pIpRx->m_NumPckBytesRcvd += NumPckts*PckSizeSrc;
	} else {
		// Always based on 188-byte packets if not in RAW mode.
		pIpRx->m_NumPckBytesRcvd += NumPckts*188;
	}

	// Compute "destination" packet size and PckSizeToCopy: this is the #packet bytes
	// copied from IP-payload to channel buffer = min(PckSizeSrc, PckSizeDst)
	switch (pIpRx->m_RxMode & DTA1XX_RXMODE_MASK) {

	case DTA1XX_RXMODE_ST188:
		PckSizeDst = PckSizeToCopy = 188;
		break;

	case DTA1XX_RXMODE_ST204:
		PckSizeDst = 204;
		if (PckSizeSrc == 188) {
			PckSizeToCopy = 188;
			Add16Zeros = 1;
		} else
			PckSizeToCopy = 204;
		break;

	case DTA1XX_RXMODE_STMP2:
	case DTA1XX_RXMODE_STRAW:
		PckSizeDst = PckSizeToCopy = PckSizeSrc;
		break;

	case DTA1XX_RXMODE_STTRP:
		PckSizeDst = 208; // Transparent-mode-packet are 208 byte long
		PckSizeToCopy = PckSizeSrc;
		if ( PckSizeSrc == 188 )
			Add16Zeros = 1;
		break;

	default:
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxProcessDvbPayLoad: INVALID RX MODE %d",
				 pIpRx->m_RxMode);
		return;			// Invalid packet mode
	}
	// Add space for time stamp, if required
	if ((pIpRx->m_RxMode & DTA1XX_RX_TIMESTAMP) != 0) 
		PckSizeDst += 4;

#if LOG_LEVEL_IPRX > 2
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxProcessDvbPayLoad: PckSizeSrc=%d "
			 "PckSizeDst=%d", pIpRx->m_ChannelIndex, PckSizeSrc, PckSizeDst);
#endif 

	// Don't store packet when Receive-Control state is IDLE
	if (pIpRx->m_RxControl == DTA1XX_RXCTRL_IDLE)
		return;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Overflow Check -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// Compute free buffer space
	// Take local copies of pointer for ease of reference, and to be sure they will not
	// be changed by another thread (for pRead only)
	pRead = (UInt8*)pIpRx->m_pRead;
	pWrite = (UInt8*)pIpRx->m_pWrite;
	NumFree = (Int)(pRead > pWrite ? pRead - pWrite : pIpRx->m_BufSize - (pWrite - pRead));

#if LOG_LEVEL_IPRX > 2
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxProcessDvbPayLoad: NumFree=%d pRead=0x%p "
			 "pWrite=0x%p", pIpRx->m_ChannelIndex, NumFree, pRead, pWrite);
#endif 

	// Check for overflow
	// Compare with the maximum #bytes that may be written for a single IP packet
	if (NumFree < (7*208 + 1)) {	// PckSizeToCopy + TimeStamp, +1 to prevent pRead == pWrite
#if LOG_LEVEL_IPRX > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxProcessDvbPayLoad: OVERFLOW");
#endif
		// Overflow!
		// No need for spinlock: m_Flags is only written from Worker thread
		pIpRx->m_Flags |= DTA1XX_RX_FIFO_OVF;

		// Acquire spin lock for updating latched flags
		KeAcquireSpinLock(&pIpRx->m_FlagsSpinLock, &OldIrqL);
		pIpRx->m_LatchedFlags |= DTA1XX_RX_FIFO_OVF;
		KeReleaseSpinLock(&pIpRx->m_FlagsSpinLock, OldIrqL);
		return;
	}
	// No overflow
	pIpRx->m_Flags &= ~DTA1XX_RX_FIFO_OVF;


	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Write Packet to Buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// The code below works because the buffer is made 256 bytes longer than the size
	// that is actually used. This way, packets that would wrap the buffer are written
	// partly in the extra 256 bytes. The overflow bytes are then copied back to the
	// beginning of the buffer.

	// Walk through payload. pPayLoad is running source pointer, payload
	// size remaining is kept in PayLoadSize, buffer write pointer in m_pWrite
	SyncErr = 0;
	while (PayLoadSize > 0) {

		// Check for synchronisation error
		if (*pPayLoad != 0x47) {
#if LOG_LEVEL_IPRX > 2
			DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxProcessDvbPayLoad: SYNC ERROR");
#endif
			SyncErr++;
		}
		NumTpPerIp++;

		// Start with writing time stamp, if specified to do so
		if ((pIpRx->m_RxMode & DTA1XX_RX_TIMESTAMP) != 0) {
			*(UInt32*)pWrite = (UInt32)TimeStamp;
			pWrite += 4;
		}

		// Payload
		memcpy(pWrite, pPayLoad, PckSizeToCopy);
		pPayLoad += PckSizeSrc;
		PayLoadSize -= PckSizeSrc;
		pWrite += PckSizeToCopy;

		// Adding 16-filler bytes
		if (Add16Zeros != 0) {
			memset(pWrite, 0, 16);
			pWrite += 16;
		}

		// Add Transparent-Mode-Packet trailer, if needed
		if ( (pIpRx->m_RxMode & DTA1XX_RXMODE_MASK) == DTA1XX_RXMODE_STTRP ) {
			*pWrite++ = 0x58;	// Sync nibble ("0101"), packet_sync and reserved bits
			*pWrite++ = (UInt8)PckSizeSrc;	// Valid count
			*pWrite++ = pIpRx->m_TrpModePckSeqCount&0xFF;	// Sequence count (LSB)
			*pWrite++ = (pIpRx->m_TrpModePckSeqCount>>8)&0xFF;	// Sequence count (MSB)
			pIpRx->m_TrpModePckSeqCount++; // Increment packet seuqence count
		}

		// Check whether packet extends beyond end-of-buffer
		// If so, copy excess bytes back to start of buffer
		if (pWrite >= pIpRx->m_pEndOfBuffer) {
			memcpy(pIpRx->m_SharedBuf.m_pBuf, pIpRx->m_pEndOfBuffer, pWrite-pIpRx->m_pEndOfBuffer);
			pWrite -= pIpRx->m_BufSize;
		}
	}		
	pIpRx->m_pWrite = pWrite;		// Save updated pWrite in IP Rx channel struct

	// Update Synchronisation-Error flag
	if (SyncErr != 0) {
		// Sync error!
		// No need for spinlock: m_Flags is only written from Worker thread
		pIpRx->m_Flags |= DTA1XX_RX_SYNC_ERR;

		// Acquire spin lock for updating latched flags
		KeAcquireSpinLock(&pIpRx->m_FlagsSpinLock, &OldIrqL);
		pIpRx->m_LatchedFlags |= DTA1XX_RX_SYNC_ERR;
		KeReleaseSpinLock(&pIpRx->m_FlagsSpinLock, OldIrqL);
	} else {
		pIpRx->m_Flags &= DTA1XX_RX_SYNC_ERR;
		pIpRx->m_DetNumTpPerIp = NumTpPerIp;
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxProcessIpRawPayLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Store the raw IP packet into the user buffer including a new header
//
void  Dta1xxNwIpRxProcessIpRawPayLoad(
	IN UserIpRxChannel*  pIpRx,			// IP Rx channel
	IN UInt8*  pPayLoad,				// IP payload with packet data
	IN Int  PayLoadSize,				// Size of payload in #bytes
	IN UInt64  TimeStamp,				// Time stamp @54MHz
	IN BOOLEAN Header)					// Include header
{
	Dta1xxIpRawHeader  IpRawHeader;
	Int  NumBytesToEnd;			// Number of bytes from pWrite till end of buffer
	Int  NumBytesToCopy;		// Number of bytes to copy
	Int  NumBytes;				// Temp. store
	UInt8*  pRead;				// Read pointer
	UInt8*  pWrite;				// Write pointer
	Int  NumFree;				// Number of free bytes in buffer
	KIRQL  OldIrqL;				// OLD IRQ level
	
	// Update last Rx timestamp for bitrate calculations
	pIpRx->m_BrmLastRxTimeStamp = TimeStamp;
	
	// Update total #packet bytes received
	// NOTE: Includes FEC packets + all IP Headers
	pIpRx->m_NumPckBytesRcvd += PayLoadSize;

	// Don't store packet when Receive-Control state is IDLE
	if (pIpRx->m_RxControl == DTA1XX_RXCTRL_IDLE)
		return;

	IpRawHeader.Tag = 0x44A0;
	IpRawHeader.TimeStamp = (UInt32)TimeStamp;
	IpRawHeader.Length = (UInt16)PayLoadSize; 

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Overflow Check -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// Compute free buffer space
	// Take local copies of pointer for ease of reference, and to be sure they will not
	// be changed by another thread (for pRead only)
	pRead = (UInt8*)pIpRx->m_pRead;
	pWrite = (UInt8*)pIpRx->m_pWrite;
	NumFree = (Int)(pRead > pWrite ? pRead - pWrite : pIpRx->m_BufSize - (pWrite - pRead));

#if LOG_LEVEL_IPRX > 2
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxProcessIpRawPayLoad: NumFree=%d pRead=0x%x "
			 "pWrite=0x%x", pIpRx->m_ChannelIndex, NumFree, pRead, pWrite);
#endif 

	// Check for overflow
	// Compare with the maximum #bytes that may be written for a single IP packet
	if (NumFree < (Int)(sizeof(Dta1xxIpRawHeader)+ PayLoadSize+1)) { // +1 to prevent pRead == pWrite
#if LOG_LEVEL_IPRX > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxProcessIpRawPayLoad: OVERFLOW");
#endif
		// Overflow!
		// No need for spinlock: m_Flags is only written from Worker thread
		pIpRx->m_Flags |= DTA1XX_RX_FIFO_OVF;

		// Acquire spin lock for updating latched flags
		KeAcquireSpinLock(&pIpRx->m_FlagsSpinLock, &OldIrqL);
		pIpRx->m_LatchedFlags |= DTA1XX_RX_FIFO_OVF;
		KeReleaseSpinLock(&pIpRx->m_FlagsSpinLock, OldIrqL);
		return;
	}
	// No overflow
	pIpRx->m_Flags &= ~DTA1XX_RX_FIFO_OVF;


	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Write Packet to Buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// The code below works because the buffer is made 256 bytes longer than the size
	// that is actually used. 

	if (Header) {
		// Copy header to buffer
		memcpy(pWrite, &IpRawHeader, sizeof(Dta1xxIpRawHeader));
		pWrite+= sizeof(Dta1xxIpRawHeader);

		// Check whether header extends beyond end-of-buffer
		// If so, copy excess bytes back to start of buffer
		if (pWrite >= pIpRx->m_pEndOfBuffer) {
			memcpy(pIpRx->m_SharedBuf.m_pBuf, pIpRx->m_pEndOfBuffer, pWrite-pIpRx->m_pEndOfBuffer);
			pWrite -= pIpRx->m_BufSize;
		}
	}

	// Ethernet packet can be larger then the extra 256 bytes, 
	// so split the copy if packet is larger then NumBytesToEnd.
	NumBytesToCopy = PayLoadSize;

	while (NumBytesToCopy) {
		
		NumBytesToEnd = (pIpRx->m_pEndOfBuffer - pWrite);
		NumBytes = (NumBytesToEnd<NumBytesToCopy?NumBytesToEnd:NumBytesToCopy);

		memcpy(pWrite, pPayLoad, NumBytes);
		NumBytesToCopy-= NumBytes;
		// Move write pointer
		pWrite+=NumBytes;
		// Move position of source data pointer
		pPayLoad+=NumBytes;

		if (pWrite >= pIpRx->m_pEndOfBuffer) {
			// Set write pointer to start of buffer
			pWrite = pIpRx->m_SharedBuf.m_pBuf;
		}
	}
	pIpRx->m_pWrite = pWrite;		// Save updated pWrite in IP Rx channel struct
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxBrmPerIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#ifdef WINBUILD
void Dta1xxNwIpRxBrmPerIntDpc(
	IN PKDPC  Dpc,					// Supplies the DPC object
	IN PVOID  DeferredContext,		// Device Extension
	IN PVOID  Arg1,					// Not used
	IN PVOID  Arg2)					// Not used
#else
void Dta1xxNwIpRxBrmPerIntDpc(
	unsigned long DeferredContext)		// Device Extension
#endif
{
	PDTA1XX_FDO  pFdoExt = (PDTA1XX_FDO)DeferredContext;
	UserIpRxChannel* pIpRx;
	UInt32 NumPckBytesRcvd;
	UInt64 BrmLastRxTimeStamp;
	UInt64 CurTime = Dta1xxGenGetRefClkCntReg64((UInt8*)pFdoExt->m_pGenRegs);
	
	KeAcquireSpinLockAtDpcLevel(&pFdoExt->m_IpRxBrmSpinlock);
	if (pFdoExt->m_IpRxBrmSkip) {
		// We can't access the m_pIpRx struct. It's in use
		KeReleaseSpinLockFromDpcLevel(&pFdoExt->m_IpRxBrmSpinlock);
		return;
	}
	pIpRx = pFdoExt->m_pIpRx;
	
	while (pIpRx) {
		BrmLastRxTimeStamp = pIpRx->m_BrmLastRxTimeStamp;
		NumPckBytesRcvd = pIpRx->m_NumPckBytesRcvd;

		if (pIpRx->m_BrmNumSamples == 0) {
			pIpRx->m_BrmEstimate = 0;
			if (NumPckBytesRcvd != 0){
#if LOG_LEVEL_IPRX > 0
				DTA1XX_LOG(KERN_INFO,"[IpRx%d] Dta1xxNwIpRxBrmPerIntDpc: First BRM sample",
						 pIpRx->m_ChannelIndex);
#endif
				pIpRx->m_BrmSampleNumPckBytes[0] = NumPckBytesRcvd;
				pIpRx->m_BrmSampleTimeStamp[0] = BrmLastRxTimeStamp;
				pIpRx->m_BrmIdxLastSample = 0;
				pIpRx->m_BrmNumSamples = 1;
			}
		} else {
			// Not first sample: check whether NUM_IPRX_MIN_PERIOD has passed
			UInt64  Period;
			// Timestamp can never wrap (55 bits = 21 year)
			Period = BrmLastRxTimeStamp - pIpRx->m_BrmSampleTimeStamp[pIpRx->m_BrmIdxLastSample];
	#if LOG_LEVEL_IPRX > 1
			DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxBrmPerIntDpc: Period=%I64u",
					pIpRx->m_ChannelIndex, Period);
	#endif
			if (Period > NUM_IPRX_MIN_PERIOD)
				Dta1xxNwIpRxBrmProcessSample(pIpRx, BrmLastRxTimeStamp);
			else {
				UInt64 DiffTime;
                // No samples received sinds last per. int
				// Timestamp can never wrap (55 bits = 21 year)
				DiffTime = CurTime - BrmLastRxTimeStamp;
				if (DiffTime >= (10 * NUM_IPRX_MIN_PERIOD)) {
					// We did not get any data for 2 s, we reset the bitrate measurement
					// and other status signals
					pIpRx->m_BrmIdxLastSample = 0;
					pIpRx->m_NumPckBytesRcvd = 0;
					pIpRx->m_BrmNumSamples = 0;
					pIpRx->m_BrmEstimate = 0;
					pIpRx->m_PckSizeSrc = 0;		// UNKNOWN
				}
			}
		}

		pIpRx = pIpRx->m_pNext;
	}
	
	KeReleaseSpinLockFromDpcLevel(&pFdoExt->m_IpRxBrmSpinlock);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxBrmProcessSample -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Process TimeStamp for bit-rate measurement.
// For the moment: SIMPLE algorithm
//
// PRE-CONDITION:
//	- This is not the first sample: pIpRx->m_BrmNumSamples!=0
//	- Time passed since previous sample is at least NUM_IPRX_MIN_PERIOD
//
void  Dta1xxNwIpRxBrmProcessSample(
	IN UserIpRxChannel*  pIpRx,			// IP Rx channel
	IN UInt64  TimeStamp)				// Time stamp @54MHz
{
	Int  IdxEldest, IdxNew;
	UInt64  DiffTs;
	UInt64  DiffNumPckBytes;
	UInt64  DiffNumPckTime;

	// Compute index of location where to store new sample, with wrap-around
	IdxNew = pIpRx->m_BrmIdxLastSample + 1;
	if (IdxNew >= NUM_IPRX_MAX_SAMPLES)
			IdxNew = 0;
	pIpRx->m_BrmIdxLastSample = IdxNew;

	// Store new sample
	pIpRx->m_BrmSampleNumPckBytes[IdxNew] = pIpRx->m_NumPckBytesRcvd;
	pIpRx->m_BrmSampleTimeStamp[IdxNew] = TimeStamp;

	// Increment #valid samples, unless maximum has been reached
	if (pIpRx->m_BrmNumSamples < NUM_IPRX_MAX_SAMPLES)
		pIpRx->m_BrmNumSamples++;

	// Compute index of eldest sample
	IdxEldest = IdxNew + 1 - pIpRx->m_BrmNumSamples;
	if (IdxEldest < 0)
		IdxEldest += NUM_IPRX_MAX_SAMPLES;

#if LOG_LEVEL_IPRX > 1
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxBrmProcessSample: IdxNew=%d IdxEldest=%d "
			 "#samples=%d", pIpRx->m_ChannelIndex, IdxNew, IdxEldest,
			 pIpRx->m_BrmNumSamples);
#endif

	// Compute bit rate
	// Timestamp can never wrap (55 bits = 21 year)
	DiffTs = TimeStamp - pIpRx->m_BrmSampleTimeStamp[IdxEldest];
	
	if (pIpRx->m_NumPckBytesRcvd < pIpRx->m_BrmSampleNumPckBytes[IdxEldest]) {
		DiffNumPckBytes = 0xffffffff + pIpRx->m_NumPckBytesRcvd - pIpRx->m_BrmSampleNumPckBytes[IdxEldest];
	} else {
		DiffNumPckBytes = pIpRx->m_NumPckBytesRcvd - pIpRx->m_BrmSampleNumPckBytes[IdxEldest];
	}
	DiffNumPckTime = (8*DiffNumPckBytes*DTA160_REF_CLOCK*1000);
	//do_div(DiffNumPckTime, DiffTs);
	// Skip bitrate calculation if DiffTs == 0 to prevent division by zero exception
	if (DiffTs != 0) {
		DiffNumPckTime = Dta1xxBinUDiv(DiffNumPckTime, DiffTs, NULL);
		pIpRx->m_BrmEstimate = (Int)DiffNumPckTime;
	}

#if LOG_LEVEL_IPRX > 1
	DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxBrmProcessSample: DiffTs=%llu "
			 "DiffNumPckBytes=%llu BrmEstimate=%d", pIpRx->m_ChannelIndex,
			 DiffTs, DiffNumPckBytes, pIpRx->m_BrmEstimate);
#endif
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IP PACKET PARSING +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxIsPacketForDVB -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Checks if the received IP packets is for one of the DVB streams. If so, an array
// with the ChannelIndex'es and the number of channels found is returned
//
UInt Dta1xxNwIpRxIsPacketForDVB(
	PDTA1XX_FDO  pFdoExt,
	UInt8* IPSrc,
	UInt8* IPDst, 
	UInt16 SrcPort,
	UInt16 DstPort,
	BOOLEAN MulticastPkt,			// True: If packet received is a Multicast packet
	UserIpRxChannel** pIpRxA)
{
	Int i;
	UserIpRxChannel*  pIpRx;
	BOOLEAN Stop;
	Int Index = 0;

	// Wait until the Rx structure is available
	ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex2);
	
	// Search the Rx channels for the DVB stream and return the ChannelIndex
	// if found
	pIpRx = pFdoExt->m_pIpRx;
	while (pIpRx) {
		
		// Check if the channel is the one we are searching for
		
		// For bitrate calculation to work in idle state, 
		// we check the RxControl somewhere else and only check here 
		// if the parameters are set. 
		if (!pIpRx->m_IpParsValid) {
			pIpRx = pIpRx->m_pNext;
			continue;
		}

		// Check if source + destination IP address is correct.
		Stop = FALSE;
		for (i = 0; (i < 4) && (!Stop); i++) {
			if ((pIpRx->m_SrcIPAddress[i] != 0) && 
				(pIpRx->m_SrcIPAddress[i] != IPSrc[i])) {
				Stop = TRUE;
			}

			// If a multicast packet is received, 
			// the m_DstIPAddress may not be 0.0.0.0
			if (((pIpRx->m_DstIPAddress[i] != 0) || MulticastPkt) && 
				(pIpRx->m_DstIPAddress[i] != IPDst[i])) {
				Stop = TRUE;
			}
		}
		if (Stop) {
			pIpRx = pIpRx->m_pNext;
			continue;
		}
		
		// Check if the source port number is correct
		if ((pIpRx->m_SrcPort != 0) && 
			(pIpRx->m_SrcPort != SrcPort)) {
			pIpRx = pIpRx->m_pNext;
			continue;
		}
		
		// Check if the destination port number is equal to the current channel
		// It may be a FEC port number if FEC is enabled
		if ((pIpRx->m_DstPort == DstPort) ||
			((pIpRx->m_FecMode != DTA1XX_IP_FEC_DISABLE) && 
				(((pIpRx->m_DstPort + FEC_INC_COLUMN_PORT) == DstPort) || 
					((pIpRx->m_DstPort + FEC_INC_ROW_PORT) == DstPort))
			)){

			pIpRxA[Index++] = pIpRx;
			
			// Increment use count
			InterlockedIncrement(&pIpRx->m_UseCount);
			
			if (Index == MAX_NUM_IPRX_LISTEN) {
				ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex2);
				return MAX_NUM_IPRX_LISTEN;
			}
		}
		pIpRx = pIpRx->m_pNext;
	}
	ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex2);
	return Index;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxIsFragmentedPacketForDVB -.-.-.-.-.-.-.-.-.-.-.-.-
//
// Checks if the received fragmented IP packet is for one of the DVB streams. If so,
// an array with the ChannelIndex'es and the number of channels found is returned
//
UInt Dta1xxNwIpRxIsFragmentedPacketForDVB(
	PDTA1XX_FDO  pFdoExt,
	UInt16 FragmentID,
	UInt16 FragmentOffset,
	UserIpRxChannel** pIpRxA)
{
	UserIpRxChannel*  pIpRx;
	Int Index = 0;
	
	// Wait until the Rx structure is available
	ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex2);
	
	// Search the Rx channels for the DVB stream and return the ChannelIndex
	// if found
	pIpRx = pFdoExt->m_pIpRx;
	while (pIpRx) {
		
		// Check if the channel is the one we are searching for
		
		// For bitrate calculation to work in idle state, 
		// we check the RxControl somewhere else and only check here 
		// if the parameters are set. 
		if (!pIpRx->m_IpParsValid) {
			pIpRx = pIpRx->m_pNext;
			continue;
		}

		// Check if frament identification is identical.
		if (pIpRx->m_FragmentId != FragmentID) {
			pIpRx = pIpRx->m_pNext;
			continue;
		}
		
		// Check if the fragment offset is not
		// exceeding max. packet length of 64kb
		if (be16_to_cpu(FragmentOffset) < 0x2000) {
			pIpRxA[Index++] = pIpRx;
			
			// Increment use count
			InterlockedIncrement(&pIpRx->m_UseCount);
			
			if (Index == MAX_NUM_IPRX_LISTEN) {
				ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex2);
				return MAX_NUM_IPRX_LISTEN;
			}
		}
		pIpRx = pIpRx->m_pNext;
	}
	ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex2);
	return Index;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxCalculateChecksum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Calculates UDP checksum. 
//
UInt16 Dta1xxNwIpRxCalculateUdpChecksum(IpHeaderV4* pIpHeader, UdpHeader* pUdpHeader, 
										UInt8* pRxData, UInt32 PayloadLength)
{
	UInt32  Checksum = 0;
	UInt32  ChecksumCs = 0;
	UInt Counter;
	
	// For testing purposes only. FPGA can do UDP checksum in final version and
	// skips the packet if checksum is incorrect
	// Calculate checksum for the UDP packet
	// Pseudo header
	Checksum  = be16_to_cpu(pIpHeader->m_Protocol);
	Checksum += ((UInt16*)pIpHeader->m_SrcAddress)[0] & 0xffff;
	Checksum += ((UInt16*)pIpHeader->m_SrcAddress)[1] & 0xffff;
	Checksum += ((UInt16*)pIpHeader->m_DstAddress)[0] & 0xffff;
	Checksum += ((UInt16*)pIpHeader->m_DstAddress)[1] & 0xffff;
	Checksum += pUdpHeader->m_Length;	

	// Checksum UDP Header
	Checksum += pUdpHeader->m_SourcePort;
	Checksum += pUdpHeader->m_DestinationPort;
	Checksum += pUdpHeader->m_Length;

	// Calculate Payload checksum
	for (Counter = 0; Counter < (PayloadLength / 2); Counter++) {
		Checksum+= ((UInt16*)pRxData)[Counter];
	}
	ChecksumCs  = (Checksum >> 16) & 0xffff;        // include carries - I
	Checksum  = (Checksum & 0xffff) + ChecksumCs;	// Add High words to low words.
	ChecksumCs  = (Checksum >> 16) & 0xffff;        // include carries - II
	Checksum  = (Checksum & 0xffff) + ChecksumCs;
	Checksum ^= -1;
	
	Checksum = Checksum & 0xffff;
	if (Checksum == 0){
		Checksum = 0xffff;
	}

	return (UInt16)Checksum;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxAddPacketToNwQueue -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxNwIpRxAddPacketToNwQueue(
	NwStore* pNwStore,
	UInt8* pSrcData,
	UInt32 Size)
{
#ifdef __WINBUILD
	PIRP pIrp;
	
#else
	UInt FreeSpaceUntilEnd;
	UInt FreeSpaceFromBegin;
	UInt ReadOffset;
	UInt WriteOffset;
	UInt PktLength;
	Dta1xxNwBuf* pNwRx = pNwStore->m_pNwRxQueue;
	
#endif
	UInt8* pDstData;
	
	if (!pNwStore->m_EnableNwiRx) {
#if LOG_LEVEL_IPRX > 2
		DTA1XX_LOG(KERN_INFO, "[NwRx%d] Dta1xxNwIpRxAddToNwQueue: Nwi receive disabled",
						  pNwStore->m_IpPortIndex);
#endif
		return;
	}

	if (Size > 2048/*NIC_BUFFER_SIZE*/) {
#if LOG_LEVEL_IPRX > 2
		DTA1XX_LOG(KERN_INFO, "[NwRx%d] Dta1xxNwIpRxAddToNwQueue: Packet too big for Nw packet buffer.",
						  pNwStore->m_IpPortIndex);
#endif
		return;

	}

#ifdef __WINBUILD
	// Windows
	pIrp = Dta1xxGetIrpInternalQueue(&pNwStore->m_NwRxQueue);
	if (pIrp == NULL) {
#if LOG_LEVEL_IPRX > 0
		DTA1XX_LOG(KERN_INFO, "[NwRx%d] Dta1xxNwIpRxAddToNwQueue: Nw receive NO BUFFERS AVAILABLE",
						  pNwStore->m_IpPortIndex);
#endif
		return;
	}
	pDstData = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority);
	pIrp->IoStatus.Information = Size;
	
	// Copy packet into IRP
	RtlCopyMemory(pDstData, pSrcData, Size);

	// Complete IRP
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
#else
	// Linux
	if (pNwStore->m_pNwRxQueue == NULL) {
#if LOG_LEVEL_IPRX > 0
		DTA1XX_LOG(KERN_INFO, "[NwRx%d] Dta1xxNwIpRxAddToNwQueue: Nwi no receive queue available",
						  pNwStore->m_IpPortIndex);
#endif
		return;
	}

	// Add Rx header
	PktLength = Size + sizeof(Dta1xxDmaRxHeader) + 4;

	// Check if we have room in our Tx buffer
	ReadOffset = pNwRx->m_ReadOffset;
	WriteOffset = pNwRx->m_WriteOffset;
	if (WriteOffset < ReadOffset) {
		FreeSpaceFromBegin = 0;
		FreeSpaceUntilEnd = ReadOffset - WriteOffset - 1;
	} else {
		FreeSpaceUntilEnd = pNwRx->m_BufSize - WriteOffset - 1;
		if (ReadOffset == 0)
			FreeSpaceFromBegin = 0;
		else
			FreeSpaceFromBegin = ReadOffset - 1;
	}

		
	if (FreeSpaceUntilEnd >= PktLength) {
		pDstData = (pNwRx->m_pBuffer + WriteOffset);

	} else if (FreeSpaceFromBegin >= PktLength) {
		pNwRx->m_pBuffer[WriteOffset] = '*';
		WriteOffset = 0;
		pDstData = (pNwRx->m_pBuffer + WriteOffset);
	} else {
		// Not enough room, skip pakket
		return;
	}

	memcpy(pDstData, pSrcData-sizeof(Dta1xxDmaRxHeader), PktLength);

	// Make sure we allign on multiples of 4
	if ( (PktLength%4)!=0 )
		PktLength += (4-(PktLength%4));

	WriteOffset+= PktLength;
	if (WriteOffset == pNwRx->m_BufSize) {
		// Can not occur due to size check above. 
		//If occurs then WriteOffset==ReadOffset can not be prevented!!
		WriteOffset = 0;
	}

	pNwRx->m_WriteOffset = WriteOffset;
	
	set_bit(0, (void*)&pNwStore->m_pNwRxQueue->m_PktAvail);
	KeSetEvent(&pNwStore->m_pNwRxQueue->m_WaitQueue, 0, FALSE);
#endif
	
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwIpRxHandleLoopback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
BOOLEAN Dta1xxNwIpRxHandleLoopback(
	NwStore* pNwStore,
	UInt8* pRxData,
	Int PacketSize)
{
	PDTA1XX_FDO  pFdoExt = pNwStore->m_pFdo;
	UserIpRxChannel*  pIpRx;
	BOOLEAN DataHandled = FALSE;
		
	// All data must be copied directly to a enabled channel
		
	// Wait until the Rx structure is available
	ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex2);
	
	pIpRx = pFdoExt->m_pIpRx;

	// Search the Rx channels to be enabled for loopback mode
	while (pIpRx) {
		// Check if the channel is the one we are searching for
		
		// Is the channel receiving?
		if ((pIpRx->m_RxControl == DTA1XX_RXCTRL_RCV) &&
			((pIpRx->m_RxMode & DTA1XX_RXMODE_MASK) == DTA1XX_RXMODE_IPRAW)){
			// Copy data into buffer without a header
			Dta1xxNwIpRxProcessIpRawPayLoad(pIpRx, pRxData, PacketSize, 
											0, FALSE);
			DataHandled = TRUE;
		}
		pIpRx = pIpRx->m_pNext;
	}
	
	ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex2);
	return DataHandled;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DecrementUseCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DecrementUseCount(
	UInt NumChannels, 
	UserIpRxChannel** pIpRx, 
	UserIpRxChannel* pIpRxFragmented)
{	UInt i;
	for (i = 0; i < NumChannels; i++) {
		// Decrement use count
		InterlockedDecrement(&(pIpRx[i]->m_UseCount));
	}
	if (pIpRxFragmented != NULL) {
		InterlockedDecrement(&pIpRxFragmented->m_UseCount);
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxParsePacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Strip headers of received IP packets with DVB data and store in the correct list.
// Unknown packets are set to the Nw Rx queue
//
void  Dta1xxNwIpRxParsePacket(
	NwStore* pNwStore,
	UInt32  Size,			// Packet size
	UInt8* pSrcData)
{	
	PDTA1XX_FDO  pFdoExt = pNwStore->m_pFdo;
	UInt  i, DvbTotalLength, ProtocolType;
	UInt16 DvbOffset;
	UInt16 DestPort;
	BOOLEAN  RtpExt = FALSE;
	BOOLEAN  MulticastPkt = FALSE;
	BOOLEAN  Fragmented = FALSE;
	IpHeaderV4* pIpHeader;
	UdpHeader*  pUdpHeader;
	RtpHeader*	 pRtpHeader;
	FecHeader*	 pFecHeader;
	RtpExtension*  pRtpHeaderExtension;
	RtpListEntry*	pRtpListEntry;
	UserIpRxChannel*  pIpRx;
	UserIpRxChannel*  pIpRxFragment = NULL;
	UserIpRxChannel*  pIpRxA[MAX_NUM_IPRX_LISTEN];
	UInt16 Checksum;
	UInt NumChannels;
	Int PacketSize;
	UInt8* pDestMac;
	UInt FrameLength;

	Dta1xxDmaRxHeader* pDmaRxHeader = (Dta1xxDmaRxHeader*)pSrcData;
	UInt8* pRxData = (UInt8*)pDmaRxHeader + sizeof(Dta1xxDmaRxHeader);
	EthernetIIHeader* pEthernetIIHeader = (EthernetIIHeader*)pRxData;
	Ethernet802_3Header* pEthernet802_3Header = (Ethernet802_3Header*)pRxData;
	
	ASSERT(Size <= 2048);
	PacketSize = Size - 4 - sizeof(Dta1xxDmaRxHeader);

#if LOG_LEVEL_IPRX > 2
	DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxParsePacket: PacketSize=%d", PacketSize);
#endif

	// If loopback is enabled, copy raw data if some channel is asking for it
	// otherwise do not raw data
	if (pNwStore->m_LoopbackEnable) {
		if (Dta1xxNwIpRxHandleLoopback(pNwStore, pRxData, PacketSize))
			return;
	}

	// Check if the network is administrative up, if not we discard all received packets
	if (pNwStore->m_AdminStatus != DTA1XX_NW_ADMINSTATUS_UP)
		return;

	// Determine the encapsulation method by checking the ethernet type of the packet
	if (pEthernetIIHeader->m_Type == ETH_TYPE_IPV4_BE) {
		// Ethernet II
		pIpHeader = (IpHeaderV4*) (pRxData + sizeof(EthernetIIHeader));
		pDestMac = (UInt8*)pEthernetIIHeader->m_DstMac;
	}
	else if ((pEthernet802_3Header->m_DSAP == 0xAA) && 
		     (pEthernet802_3Header->m_SSAP == 0xAA) &&
			 (pEthernet802_3Header->m_Type == ETH_TYPE_IPV4_BE)) {
		// 802.2/802.3 encapsulation
		pIpHeader = (IpHeaderV4*) (pRxData + sizeof(Ethernet802_3Header));
		pDestMac = (UInt8*)pEthernet802_3Header->m_DstMac; 
	}
	else {
		// Unknown packet type.....
		Dta1xxNwIpRxAddPacketToNwQueue(pNwStore, pRxData, PacketSize);
		return;
	}

	// Check if the destination mac is a multicast/broadcast address and
	// Rx is enabled.
	if (pDestMac[0] & 1) {	// It's a multicast or broadcast address
		if ((pDestMac[0] == 0xff) && (pDestMac[1] == 0xff)) {
			// Broadcast address
			if (((pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_BROADCAST) == 0) &&
				((pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_PROMISCUOUS) == 0)){
				// Broadcast Rx disabled
#if LOG_LEVEL_IPRX > 0
				DTA1XX_LOG(KERN_INFO, "[NwRx%d] Dta1xxNwIpRxParsePacket: Broadcast Rx disabled.", 
					pNwStore->m_IpPortIndex);	
#endif
				return;
			}
		} else if (((pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_PROMISCUOUS) == 0) &&
					((pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_ALL_MULTICAST) == 0) &&
					((pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_MULTICAST) == 0)) {
				// Skip this packet, multicast receive disabled
#if LOG_LEVEL_IPRX > 0
				DTA1XX_LOG(KERN_INFO, "[NwRx%d] Dta1xxNwIpRxParsePacket: Multicast Rx disabled. "
						 "MC: %02x-%02x-%02x-%02x-%02x-%02x", 
							pNwStore->m_IpPortIndex,
							pDestMac[0], pDestMac[1], pDestMac[2],
							pDestMac[3], pDestMac[4], pDestMac[5]);	
#endif
				return;
		} else if (((pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_PROMISCUOUS) == 0) &&
					((pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_ALL_MULTICAST) == 0) &&
					(pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_MULTICAST)) {

				// Check if the destination MAC address is in the Multicast list
				BOOLEAN Found = FALSE;
				UInt NumItems = 0;
				Int i;
				while ((!Found) && (NumItems < pNwStore->m_NumMulticast)) {
					Found = TRUE;
					for (i=0; (i<6) && Found; i++) {
						if (pNwStore->m_MCList[NumItems][i] != pDestMac[i]) {
							Found = FALSE;
						}
					}
					NumItems++;
				}
				if (!Found) {
#if LOG_LEVEL_IPRX > 0
					DTA1XX_LOG(KERN_INFO, "[NwRx%d] Dta1xxNwIpRxParsePacket: Multicast address not in the list. "
							"MC: %02x-%02x-%02x-%02x-%02x-%02x", 
							pNwStore->m_IpPortIndex,
							pDestMac[0],pDestMac[1], pDestMac[2],
							pDestMac[3], pDestMac[4], pDestMac[5]);	
#endif
					return;
				} else {
#if LOG_LEVEL_IPRX > 1
					DTA1XX_LOG(KERN_INFO, "[NwRx%d] Dta1xxNwIpRxParsePacket: Multicast address Rx. "
								"MC: %02x-%02x-%02x-%02x-%02x-%02x", 
								pNwStore->m_IpPortIndex,
								pDestMac[0],pDestMac[1], pDestMac[2],
								pDestMac[3], pDestMac[4], pDestMac[5]);	
#endif
					MulticastPkt = TRUE;					
				}
		}
	}

	// Check if we have to deal with fragmented packets
	if ((pIpHeader->m_Flags & IPV4_HDR_FLAGS_MOREFRAG) != 0 ) {
		// Fragmented packet
		if ((pIpHeader->m_FragmentOffsetL == 0) && (pIpHeader->m_FragmentOffsetH == 0)) {
			// First fragmented packet, we need to check if the UDP header is correct
			Fragmented = TRUE;
		} else {
			UInt8* pBuf;
			// Not first packet. Check all channels to see if current fragment belongs to 
			// a particular channel
			// Channels usecount is incremented!
			NumChannels = Dta1xxNwIpRxIsFragmentedPacketForDVB(pFdoExt,
											pIpHeader->m_Identification, 
											((UInt16)pIpHeader->m_FragmentOffsetH << 5) + 
													 pIpHeader->m_FragmentOffsetL,
                                            pIpRxA);
			
			if (NumChannels == 0) {
				Dta1xxNwIpRxAddPacketToNwQueue(pNwStore, pRxData, PacketSize);
				return;
			}
			// We have to copy the fragmented packet into the buffer
			pBuf = (UInt8*)pIpHeader + (pIpHeader->m_HeaderLength * 4);
			Size = Size - (pBuf - pSrcData) - 4; // -4 checksum
			for (i = 0; i < NumChannels; i++) {
				pIpRx = pIpRxA[i];
				
				// Packet size max. 64kb
				if ((Size + pIpRx->m_FragmentOffset) > 0xffff) {
					// Size too big, skip packet;
					pIpRx->m_FragmentId = 0;
					pIpRx->m_FragmentOffset = 0;
				} else {
					// Copy data into temp. buffer
					memcpy(pIpRx->m_pRtpBuffer + 2 + pIpRx->m_FragmentOffset, pBuf, Size);	
					pIpRx->m_FragmentOffset+= (UInt16)Size;
				}

				// Decrement use count
				InterlockedDecrement(&pIpRx->m_UseCount);
			}
			return;
		}
	} else if ((pIpHeader->m_FragmentOffsetL != 0) || (pIpHeader->m_FragmentOffsetH != 0)) {
		UInt8* pBuf;
		// Last fragment
		// Copy last packet
		// Check all channels to see if current fragment belongs to 
		// a particular channel
		// Channels usecount is incremented!
		NumChannels = Dta1xxNwIpRxIsFragmentedPacketForDVB(pFdoExt,
										pIpHeader->m_Identification, 
										((UInt16)pIpHeader->m_FragmentOffsetH << 5) + 
													pIpHeader->m_FragmentOffsetL,
                                        pIpRxA);
		
		if (NumChannels == 0) {
			Dta1xxNwIpRxAddPacketToNwQueue(pNwStore, pRxData, PacketSize);
			return;
		}
		// We have to copy the fragmented packet into the buffer
		pBuf = (UInt8*)pIpHeader + (pIpHeader->m_HeaderLength * 4);
		Size = Size - (pBuf - pSrcData) - 4;

		for (i = 0; i < NumChannels; i++) {
			pIpRx = pIpRxA[i];

			// Search for the packet that is complete
			if (pIpRxFragment == NULL) {
				UInt UdpLength;
				// Check if this packet is complete. 
				// If so, use this packet
				// Set pointer to the UDP header
				pIpHeader = (IpHeaderV4*)(pIpRx->m_pRtpBuffer + 2 + pIpRx->m_pRtpBuffer[0]);
				pUdpHeader = (UdpHeader*)((UInt8*)pIpHeader + (pIpHeader->m_HeaderLength * 4));
				UdpLength = pIpRx->m_FragmentOffset + Size - sizeof(Dta1xxDmaRxHeader) 
								- sizeof(EthernetIIHeader) - (pIpHeader->m_HeaderLength * 4);

				if (be16_to_cpu(pUdpHeader->m_Length) <= UdpLength) {
					if  (Size+pIpRx->m_FragmentOffset > 0xffff) {
						// Packet to large
						pIpRx->m_FragmentId = 0;
						pIpRx->m_FragmentOffset = 0;
						continue;
					}
					// Copy data into temp. buffer
					memcpy(pIpRx->m_pRtpBuffer + 2 + pIpRx->m_FragmentOffset, pBuf, Size);	
					
					pIpRx->m_FragmentOffset+= (UInt16)Size;

					pIpHeader = (IpHeaderV4*)(pIpRx->m_pRtpBuffer + 2 + pIpRx->m_pRtpBuffer[0]);
					pDestMac = (UInt8*)(pIpRx->m_pRtpBuffer + 2 + pIpRx->m_pRtpBuffer[1]);

					pDmaRxHeader = (Dta1xxDmaRxHeader*)(pIpRx->m_pRtpBuffer + 2);
					pDmaRxHeader->ReceiveStatus.FrameLength = pIpRx->m_FragmentOffset;
					pRxData = (UInt8*)pDmaRxHeader + sizeof(Dta1xxDmaRxHeader);
		
					PacketSize = pIpRx->m_FragmentOffset - sizeof(Dta1xxDmaRxHeader);
					pIpRxFragment = pIpRx;
				}
			}
		}

		for (i = 0; i < NumChannels; i++) {
			pIpRx = pIpRxA[i];

			if (pIpRx != pIpRxFragment){
				// Decrement use count
				InterlockedDecrement(&pIpRx->m_UseCount);
			}
		}

		if (pIpRxFragment == NULL) {
			// No complete packet found
			// skip all and return
			return;
		}
	}
	
	// Set pointer to the UDP header
	pUdpHeader = (UdpHeader*)((UInt8*)pIpHeader + (pIpHeader->m_HeaderLength * 4));
	
	// Now check if the protocol is a User Datagram Protocol packet type
	if (pIpHeader->m_Protocol != UDP_PACKET_TYPE)  {
		// Not a UDP packet type.....
	    Dta1xxNwIpRxAddPacketToNwQueue(pNwStore, pRxData, PacketSize);
		return;
	}

	DestPort = be16_to_cpu(pUdpHeader->m_DestinationPort);

	// Check if the IP packet is a packet registered by an application
	// Channels usecount is incremented!
	NumChannels = Dta1xxNwIpRxIsPacketForDVB(pFdoExt,
									pIpHeader->m_SrcAddress, pIpHeader->m_DstAddress, 
									be16_to_cpu(pUdpHeader->m_SourcePort),
									DestPort, MulticastPkt,
									pIpRxA);
	
	if (NumChannels == 0) {
		Dta1xxNwIpRxAddPacketToNwQueue(pNwStore, pRxData, PacketSize);
		return;
	}


	if (Fragmented) {
		
		// We have to copy the fragmented packet into the buffer
		for (i = 0; i < NumChannels; i++) {
			pIpRx = pIpRxA[i];
			pIpRx->m_FragmentId = pIpHeader->m_Identification;
			// Store IP Header offset and dest. mac address offset for later
			pIpRx->m_pRtpBuffer[0] =  (UInt8*)pIpHeader - pSrcData;
			pIpRx->m_pRtpBuffer[1] =  pDestMac - pSrcData;

			// Copy data into temp. buffer
			memcpy(pIpRx->m_pRtpBuffer+2, pDmaRxHeader, Size - 4);	// excluding checksum
			pIpRx->m_FragmentOffset = (UInt16)Size - 4;
			// Decrement use count
			InterlockedDecrement(&pIpRx->m_UseCount);
		}
		return;
	}
    	
	// #bytes for DVB payload
	DvbTotalLength = be16_to_cpu(pUdpHeader->m_Length);
	
	// Check if frame length stored in the RxHeader is greater or equal to the length stored in UDP header + all bytes
	// before.
	// If not, set packet to Nw queue, packet is too short!!
	FrameLength = DvbTotalLength + PtrToUlong((UInt8*)((UInt8*)pUdpHeader - pRxData)) + 4; // +4 = CRC
    if (FrameLength > pDmaRxHeader->ReceiveStatus.FrameLength && (pIpRxFragment == NULL)) {
#if LOG_LEVEL_IPRX > 0
		DTA1XX_LOG( KERN_INFO,
					"[NwRx%d] Dta1xxNwIpRxParsePacket: UDP Length error. Packet too short",
					pNwStore->m_IpPortIndex );	
#endif
		ASSERT(FALSE);
		DecrementUseCount(NumChannels, pIpRxA, pIpRxFragment);
		if (PacketSize < 1532) {
			Dta1xxNwIpRxAddPacketToNwQueue(pNwStore, pRxData, PacketSize);
		}
		return;
	}

	DvbTotalLength-= sizeof(UdpHeader);
	// Store pre. calculation of DVB offset
	DvbOffset = (Int)(((UInt8*)pUdpHeader + sizeof(UdpHeader)) - pRxData);

	if (pIpRxFragment) {
		// Check UDP checksum
		Checksum = Dta1xxNwIpRxCalculateUdpChecksum(pIpHeader, pUdpHeader, 
										pRxData + DvbOffset, DvbTotalLength);
		
		if ((pUdpHeader->m_Checksum != 0) && (Checksum != pUdpHeader->m_Checksum)) {
#if LOG_LEVEL_IPRX > 0
			DTA1XX_LOG( KERN_INFO, "CHECKSUM ERROR, calculated: %x, received: %x", 
						Checksum, pUdpHeader->m_Checksum );
#endif
			DecrementUseCount(NumChannels, pIpRxA, pIpRxFragment);
			return;
		}
		/*if (Checksum != pUdpHeader->m_Checksum) {
			//read from b00
			UInt32 b= READ_UINT((Int*)pNwStore->m_pFdoExt->m_Dta1xxRegsBase.m_pKernel, 0xb00);
		}
		ASSERT(Checksum == pUdpHeader->m_Checksum);*/
	}

	// Check RTP header
	pRtpHeader = (RtpHeader*)((UInt8*)pUdpHeader + sizeof(UdpHeader));

	if (  (   (pRtpHeader->m_PayloadType != RTP_PAYLOAD_MPEGII)
		   && (pRtpHeader->m_PayloadType != RTP_PAYLOAD_FEC))
		   || (pRtpHeader->m_Version != 2)) {

		// Known payload types not found with RTPVersion of 2.
		// It's a UDP packet.
		ProtocolType = PROT_UDP;

		// Check if the Payload is DVB data (0x47 must be the first byte of the payload
		if (*(pRxData + DvbOffset) != 0x47) {
			// It's not a DVB packet or a DVB packet with unknown header/content
			DecrementUseCount(NumChannels, pIpRxA, pIpRxFragment);
			Dta1xxNwIpRxAddPacketToNwQueue(pNwStore, pRxData, PacketSize);
			return;
		}
	}
	else {
		// RTP Packet with MPEG-II or FEC content received
		if (pIpRxFragment) {
			// We can not handle fragmented RTP packets
			// Discard packets
			DecrementUseCount(NumChannels, pIpRxA, pIpRxFragment);
#if LOG_LEVEL_IPRX > 2
			DTA1XX_LOG( KERN_INFO,
						"[NwRx%d] Dta1xxNwIpRxParsePacket: Fragmented RTP packet discarded",
					    pNwStore->m_IpPortIndex);
#endif

			return;
		}
		// Change packetLength in header if more bytes are received then expected
		// Used in RTP packet reconstruction and DVB extraction
		// So extra bytes not belonging to UDP payload are removed here
		if (pDmaRxHeader->ReceiveStatus.FrameLength != FrameLength) {
#if LOG_LEVEL_IPRX > 2
			DTA1XX_LOG( 
				KERN_INFO,
				"[NwRx%d] Dta1xxNwIpRxParsePacket: Ethernet packet length > Calculated length"
				" Ether=%d Calc=%d",
				pNwStore->m_IpPortIndex, pDmaRxHeader->ReceiveStatus.FrameLength,
				FrameLength );
#endif
			pDmaRxHeader->ReceiveStatus.FrameLength = FrameLength;
		}

		RtpExt = (pRtpHeader->m_Extension != 0);
		
		ProtocolType = PROT_RTP;
		
		// Subtract RTP header size
		DvbTotalLength -= ((pRtpHeader->m_CsrcCount * 4) + sizeof(RtpHeader));

		DvbOffset  += (pRtpHeader->m_CsrcCount * 4) + sizeof(RtpHeader);
	}

	// Check for RTP header extension (RtpExt = FALSE for UDP packets)
	if (RtpExt) {
		pRtpHeaderExtension =  (RtpExtension*)((UInt8*)pRtpHeader
							 + (pRtpHeader->m_CsrcCount * 4) + sizeof(RtpHeader));
		// Subtract header-extention size  
		DvbTotalLength	-= ((be16_to_cpu(pRtpHeaderExtension->m_Length) * 4) + sizeof(RtpExtension));
		DvbOffset += (be16_to_cpu(pRtpHeaderExtension->m_Length) * 4) + sizeof(RtpExtension);
	}

	// Copy the IP packets to all channels which need them
	for (i = 0; i < NumChannels; i++) {

		// The use count of the IpRx channel is already incremented,
		// so the structure is available
		pIpRx = pIpRxA[i];
		pIpRx->m_FragmentId = 0;
		pIpRx->m_FragmentOffset = 0;
			
		
		// Check if we are in Raw IP mode. 
		// If so, the complete packet must be copied 
		// and a header must be added
		if ((pIpRx->m_RxMode & DTA1XX_RXMODE_MASK) == DTA1XX_RXMODE_IPRAW) {
			
			// Remove extra bytes not belonging to UDP payload
			/*if (pDmaRxHeader->ReceiveStatus.FrameLength != FrameLength) {
#if LOG_LEVEL_IPRX > 2
				DTA1XX_LOG(
					KERN_INFO,
					"[NwRx%d] Dta1xxNwIpRxParsePacket: Ethernet packet length > Calculated length"
					" Ether=%d Calc=%d",
					pNwStore->m_IpPortIndex, pDmaRxHeader->ReceiveStatus.FrameLength,
					FrameLength );
#endif
				PacketSize2 = PacketSize - (pDmaRxHeader->ReceiveStatus.FrameLength - FrameLength);
			}*/

			// Process the raw ip data (including extra bytes after the UDP payload if available).
			Dta1xxNwIpRxProcessIpRawPayLoad(pIpRx, pRxData, PacketSize, 
											pDmaRxHeader->Timestamp,
											TRUE);

			// Decrement use count
			InterlockedDecrement(&pIpRx->m_UseCount);
			continue;
		}

		if (ProtocolType == PROT_UDP) {
				
	#if LOG_LEVEL_IPRX > 2
			DTA1XX_LOG( KERN_INFO,
						"[IpRx%d] Dta1xxNwIpRxParsePacket: UDP DVB Packet received. DVB-"
						"DataSize=%i", pIpRx->m_ChannelIndex, DvbTotalLength );
	#endif

			// It's a UDP packet with DVB content. Add to user buffer
			
			ASSERT(*(pRxData + DvbOffset) == 0x47);

			pIpRx->m_DetProtocol = DTA1XX_IP_PROTO_UDP;
			pIpRx->m_DetFecNumColumns = 0;
			pIpRx->m_DetFecNumRows = 0;

			Dta1xxNwIpRxProcessDvbPayLoad(
				pIpRx, pRxData + DvbOffset, DvbTotalLength, pDmaRxHeader->Timestamp);
			
			// Decrement use count
			InterlockedDecrement(&pIpRx->m_UseCount);
			
		} else if (pRtpHeader->m_PayloadType == RTP_PAYLOAD_MPEGII) {
			// It's a RTP packet with DVB content. Add to seq. buffer
				
			pIpRx->m_DetProtocol = DTA1XX_IP_PROTO_RTP;

			// Increment No Fec packets receive counter
			if (++pIpRx->m_RstCntFecColumn >= MAX_NUM_RTP_DVB_PACKETS) {
				pIpRx->m_DetFecNumColumns = 0;
			}
			if (++pIpRx->m_RstCntFecRow >= MAX_NUM_RTP_DVB_PACKETS) {
				pIpRx->m_DetFecNumRows = 0;
			}
				
			// This is for debugging FEC Packets,must be removed in release version
			/*if (((be16_to_cpu(pRtpHeader->m_SequenceNumber) % 101) == 0) || 
				  ((be16_to_cpu(pRtpHeader->m_SequenceNumber) % 202) == 0) || 
				  ((be16_to_cpu(pRtpHeader->m_SequenceNumber) % 203) == 0)) {
				// Skip this packet, packet must be reconstructed.
			} else*/ {	// START ELSE TEMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			if (pIpRx->m_RxControl == DTA1XX_RXCTRL_IDLE) {
				// We only need to calculate the bitrate
				Dta1xxNwIpRxProcessDvbPayLoad(
					pIpRx,
					(UInt8*)pDmaRxHeader + DvbOffset + sizeof(Dta1xxDmaRxHeader),
					pDmaRxHeader->ReceiveStatus.FrameLength - DvbOffset - 4, // -4 for CRC
					pDmaRxHeader->Timestamp);
				
				// Decrement use count
				InterlockedDecrement(&pIpRx->m_UseCount);
				continue;
			}

			pRtpListEntry = Dta1xxNwIpRxRtpGetFreeEntry(pIpRx);

			if (pRtpListEntry) {

				// Fill content
				pRtpListEntry->m_RTPSequenceNumber	= be16_to_cpu(pRtpHeader->m_SequenceNumber);
				pRtpListEntry->m_PayloadOffset		= DvbOffset;
				pRtpListEntry->m_RTPOffset			= (Int)((UInt8*)pRtpHeader - pRxData);

				// Copy complete packet into the buffer
				RtlCopyMemory(pIpRx->m_pRtpBuffer + 
								(pRtpListEntry->m_BufIndex * DTA1XX_IPRX_MAX_PACKET_LENGTH), 
							pDmaRxHeader,
							Size);
				
				// Add packet to the RTP DVB List
				if (!Dta1xxNwIpRxRtpAddEntryToList(pIpRx, &pIpRx->m_RtpDvbList, pRtpListEntry)) {
					// Packet already in list
					// Set packet back to empty list
					Dta1xxNwIpRxRtpSetEntryToEmptyList(pIpRx, pRtpListEntry);
				} else {
					pIpRx->m_RxIncomingPackets = TRUE;
					pFdoExt->m_IPRxPacketAvail = TRUE;
				}
			} else {
				// Out of memory
#if LOG_LEVEL_IPRX > 0
				DTA1XX_LOG( KERN_INFO,
							"[IpRx%d] Dta1xxNwIpRxParsePacket: No queue elements available. Buffer FULL. Seq. Num: %d",
							pIpRx->m_ChannelIndex, be16_to_cpu(pRtpHeader->m_SequenceNumber));
#endif
				// Extra trigger reconstructor to parse the data
				pIpRx->m_RxIncomingPackets = TRUE;
				pFdoExt->m_IPRxPacketAvail = TRUE;
			}
			} //END ELSE TEMP!!!!!!!!!!!!!!!!!!!!!!!
		
			// Decrement use count
			InterlockedDecrement(&pIpRx->m_UseCount);
				
		} else if (pRtpHeader->m_PayloadType == RTP_PAYLOAD_FEC) {
			// It's a FEC packet
			if (pIpRx->m_FecMode == DTA1XX_IP_FEC_DISABLE) {
#if LOG_LEVEL_IPRX > 0
				DTA1XX_LOG( KERN_INFO,
							"[IpRx%d] Dta1xxNwIpRxParsePacket: FEC packet received, but FEC mode is disabled",
							pIpRx->m_ChannelIndex);
#endif

				// Decrement use count
				InterlockedDecrement(&pIpRx->m_UseCount);
				
				//Dta1xxNwIpRxAddPacketToNwQueue(pNwStore, pRxData, PacketSize);
				continue;
			}
						
			pFecHeader = (FecHeader*)&(pRxData[DvbOffset]);
			// Don't do this here, because this will be executed multiple times for each channel!!
			//DvbOffset+= sizeof(FEC_HEADER);

			// Check if the correct port number is used for this FEC Packet
			if (((pFecHeader->m_D == 0) && (DestPort!= (pIpRx->m_DstPort + FEC_INC_COLUMN_PORT))) ||
				((pFecHeader->m_D == 1) && (DestPort!= (pIpRx->m_DstPort + FEC_INC_ROW_PORT)))) {
				// Port number is incorrect for this fec type. 
#if LOG_LEVEL_IPRX > 0
				DTA1XX_LOG( KERN_INFO,
							"[IpRx%d] Dta1xxNwIpRxParsePacket: Incorrect destination port number"
							" for FEC packet. PortNumber: %i, FecType(D): %i", 
							pIpRx->m_ChannelIndex, DestPort, pFecHeader->m_D );
#endif
				// Decrement use count
				DecrementUseCount(NumChannels, pIpRxA, pIpRxFragment);
				Dta1xxNwIpRxAddPacketToNwQueue(pNwStore, pRxData, PacketSize);
				return;
			}

			// Check if the correct error correction type is used
			if (pFecHeader->m_Type != 0) {
				// Fec packet is not XOR Fec. 
#if LOG_LEVEL_IPRX > 0
				DTA1XX_LOG(
					KERN_INFO,
					"[IpRx%d] Dta1xxNwIpRxParsePacket: Incorrect FEC error correction type."
					"Error correction type: %i.", pIpRx->m_ChannelIndex, pFecHeader->m_Type );
#endif
				// Decrement use count
				DecrementUseCount(NumChannels, pIpRxA, pIpRxFragment);
				Dta1xxNwIpRxAddPacketToNwQueue(pNwStore, pRxData, PacketSize);
				return;
			}

			if (pIpRx->m_RxControl == DTA1XX_RXCTRL_IDLE) {
				// We only update status in IDLE state
				if (pFecHeader->m_D == 0) {	// Column
					pIpRx->m_DetFecNumRows = pFecHeader->m_NA;
					pIpRx->m_RstCntFecRow = 0;
				} else {
					pIpRx->m_DetFecNumColumns = pFecHeader->m_NA;
					pIpRx->m_RstCntFecColumn = 0;
				}
				
				// Decrement use count
				InterlockedDecrement(&pIpRx->m_UseCount);
				continue;
			}

			pRtpListEntry = Dta1xxNwIpRxRtpGetFreeEntry(pIpRx);

			if (pRtpListEntry) {

				// Fill content
				pRtpListEntry->m_RTPSequenceNumber	= be16_to_cpu(pRtpHeader->m_SequenceNumber);
				pRtpListEntry->m_PayloadOffset		= DvbOffset + sizeof(FecHeader);
				pRtpListEntry->m_RTPOffset			= (Int)((UInt8*)pRtpHeader - pRxData);

				// Store FEC information
				pRtpListEntry->m_FecNA = pFecHeader->m_NA;
				pRtpListEntry->m_FecOffset = pFecHeader->m_Offset;
				pRtpListEntry->m_FecSNBase = be16_to_cpu(pFecHeader->m_SnBase);
				
				// Copy complete packet into the buffer
				RtlCopyMemory(pIpRx->m_pRtpBuffer + 
								(pRtpListEntry->m_BufIndex * DTA1XX_IPRX_MAX_PACKET_LENGTH), 
								pDmaRxHeader,
								Size);

				// Add packet to correct queue
				if (pFecHeader->m_D == 0) {	// Column
					pIpRx->m_DetFecNumRows = pFecHeader->m_NA;
					pIpRx->m_RstCntFecRow = 0;

					// Add packet to the RTP Column List
					if (!Dta1xxNwIpRxRtpAddEntryToList(pIpRx, &pIpRx->m_RtpFecColumnList, pRtpListEntry)) {
						// Packet already in list
						// Set packet back to empty list
						Dta1xxNwIpRxRtpSetEntryToEmptyList(pIpRx, pRtpListEntry);
					} else {
						pIpRx->m_RxIncomingPackets = TRUE;
						pFdoExt->m_IPRxPacketAvail = TRUE;
					}
				} else {					// Row
					pIpRx->m_DetFecNumColumns = pFecHeader->m_NA;
					pIpRx->m_RstCntFecColumn = 0;

					// Add packet to the RTP Row List
					if (!Dta1xxNwIpRxRtpAddEntryToList(pIpRx, &pIpRx->m_RtpFecRowList, pRtpListEntry)) {
						// Packet already in list
						// Set packet back to empty list
						Dta1xxNwIpRxRtpSetEntryToEmptyList(pIpRx, pRtpListEntry);	
					} else {
						pIpRx->m_RxIncomingPackets = TRUE;
						pFdoExt->m_IPRxPacketAvail = TRUE;
					}
				
				}
			} else {
				// Out of memory
#if LOG_LEVEL_IPRX > 0
				DTA1XX_LOG(
						KERN_INFO,
						"[IpRx%d] Dta1xxNwIpRxParsePacket: No queue elements available. Buffer FULL. FEC",
						pIpRx->m_ChannelIndex);
#endif
				// Extra trigger reconstructor to parse the data
				pIpRx->m_RxIncomingPackets = TRUE;
				pFdoExt->m_IPRxPacketAvail = TRUE;
			}

			// Decrement use count
			InterlockedDecrement(&pIpRx->m_UseCount);

		} else {
			ASSERT(FALSE);
			// Unknown packet type. Maybe corrupted packet and checksum ckeck is off??
#if LOG_LEVEL_IPRX > 0
			DTA1XX_LOG( KERN_INFO,
						"[IpRx%d] Dta1xxNwIpRxParsePacket: UNKNOWN PACKET OR CORRUPTED PACKET??.",
						pIpRx->m_ChannelIndex );
#endif
			
			// Decrement use count
			DecrementUseCount(NumChannels, pIpRxA, pIpRxFragment);
			Dta1xxNwIpRxAddPacketToNwQueue(pNwStore, pRxData, PacketSize);
			return;
		}
	}
	if (pIpRxFragment) {
		// Decrement use count
		InterlockedDecrement(&pIpRxFragment->m_UseCount);
	}

	return;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ WORKER THREAD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxCreateWorkerThreads -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Create worker threads for IP Packet Rx with DVB/Non DVB data
//
NTSTATUS  Dta1xxNwIpRxCreateWorkerThreads(IN PDTA1XX_FDO pFdoExt)
{
	NTSTATUS  Status;
	Int i;
	NwStore* pNwStore;

	KeInitializeEvent(&pFdoExt->m_IPRxThreadStopEvent, NotificationEvent, FALSE);
	KeInitializeEvent(&pFdoExt->m_IPRxPacketAvailEvent, SynchronizationEvent, FALSE);
	
	pFdoExt->m_IPRxPacketAvail = FALSE;

	for (i = 0; i < pFdoExt->m_NumIpPorts; i++) {
		pNwStore = &(pFdoExt->m_NwStore[i]);
		// Create Rx worker thread
		Status = CreateSystemThread(&pNwStore->m_IPRxThread, Dta1xxNwIpRxWorkerThread, pNwStore);
		
		if (!NT_SUCCESS(Status)) {
#if LOG_LEVEL_IPRX > 0
			DTA1XX_LOG( KERN_INFO,
						"Dta1xxNwIpRxWorkerThread: ERROR CREATING thread %i: 0x%lx",
						i, Status);
#endif
			return Status;
		}
	}

	// Create Rx reconstruction thread
	Status = CreateSystemThread(&pFdoExt->m_IPRxRecThread, Dta1xxNwIpRxReconstructThread, pFdoExt);
		
	if (!NT_SUCCESS(Status)) {
#if LOG_LEVEL_IPRX > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxNwIpRxReconstructThread: ERROR CREATING: 0x%lx",
					Status);
#endif
		return Status;
	}
	return STATUS_SUCCESS;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxStopWorkerThreads -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Stops the IP Rx worker threads.
// 
NTSTATUS  Dta1xxNwIpRxStopWorkerThreads(IN PDTA1XX_FDO pFdoExt)
{	Int i;

	if (!pFdoExt->m_NwInitialised)
		return STATUS_SUCCESS;
#ifdef WINBUILD
	KeSetEvent(&pFdoExt->m_IPRxThreadStopEvent, 0, FALSE);

	for (i = 0; i < pFdoExt->m_NumIpPorts; i++) {
		KeWaitForSingleObject(pFdoExt->m_NwStore[i].m_IPRxThread, 
							  Executive, KernelMode, FALSE, NULL);
		ObDereferenceObject(pFdoExt->m_NwStore[i].m_IPRxThread);
	}

	KeWaitForSingleObject(pFdoExt->m_IPRxRecThread, Executive, KernelMode, FALSE, NULL);
	ObDereferenceObject(pFdoExt->m_IPRxRecThread);
#else
	for (i = 0; i < pFdoExt->m_NumIpPorts; i++) {
		// Trigger thread to stop and wait until it is stopped
		Dta1xxKThreadStop(pFdoExt->m_NwStore[i].m_IPRxThread);
	}
	// Trigger thread to stop and wait until it is stopped
	Dta1xxKThreadStop(pFdoExt->m_IPRxRecThread);

#endif
	return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIpRxParseRxBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
inline void Dta1xxIpRxParseRxBuffer(
	IN NwStore* pNwStore,
	UInt8* pRxData,
	UInt Length,
	UInt8* pPrevData, 
	UInt* pPrevDataLength)
{
	PDTA1XX_FDO	pFdoExt = pNwStore->m_pFdo;
	Dta1xxDmaRxHeader* pRxHeader;
	UInt BytesToCopy;
	BOOLEAN CorrPacket;
	UInt8* pTmpRxData = pRxData;
	UInt TmpLength = Length;
	
	// A new DMA transfer is ready
	// Was previous transfer not finished?
	if (*pPrevDataLength) {
			
		// First copy part of header if not yet completely available
		if (*pPrevDataLength < sizeof(Dta1xxDmaRxHeader)) {
			BytesToCopy = sizeof(Dta1xxDmaRxHeader) - *pPrevDataLength;
			if (BytesToCopy > Length) {
				BytesToCopy = Length;
#if LOG_LEVEL_IPRX > 1
				DTA1XX_LOG(
					KERN_INFO,
					"Dta1xxNwIpRxWorkerThread: Header STILL NOT complete. Size: %d.", 
					*pPrevDataLength+BytesToCopy);
#endif
			} 
			RtlCopyMemory(pPrevData+*pPrevDataLength, pRxData, BytesToCopy);
			pRxData+= BytesToCopy;
			Length-=  BytesToCopy;
			*pPrevDataLength += BytesToCopy;
		}
		if (*pPrevDataLength >= sizeof(Dta1xxDmaRxHeader)) {
			// At least header is available
			pRxHeader = (Dta1xxDmaRxHeader*)pPrevData;

			// Check the tag, if tag is not correct, skip packet header and restart with new data
			if ((pRxHeader->Tag == 0x445441A0) && 
				(pRxHeader->Length == sizeof(Dta1xxDmaRxHeader)) &&
				((pRxHeader->ReceiveStatus.FrameLength + sizeof(Dta1xxDmaRxHeader)) < 2000)){
#if LOG_LEVEL_IPRX > 1
				DTA1XX_LOG( KERN_INFO, "Dta1xxNwIpRxWorkerThread: Header complete." );
#endif
				if ((Length+*pPrevDataLength) < (pRxHeader->ReceiveStatus.FrameLength + sizeof(Dta1xxDmaRxHeader))) {
					// Not a complete packet available
					// Copy part of header into temp. buffer for later
					RtlCopyMemory(pPrevData+*pPrevDataLength, pRxData, Length);
					*pPrevDataLength+= Length;
					Length = 0;
#if LOG_LEVEL_IPRX > 1
					DTA1XX_LOG( KERN_INFO,
						"Dta1xxNwIpRxWorkerThread: Partly packet stored for second time. Size: %d. Packet size: %d", 
						*pPrevDataLength, pRxHeader->ReceiveStatus.FrameLength);
#endif
				} else {
					// Complete correct packet available
					ASSERT(pRxHeader->ReceiveStatus.FrameLength < (Length+*pPrevDataLength));
#if LOG_LEVEL_IPRX > 1
#if DBG
					if (pRxHeader->ReceiveStatus.FrameLength >= 1374) {//Possibly RTP
						DTA1XX_LOG( 
							KERN_INFO,
							"Dta1xxNwIpRxWorkerThread: Packet complete. Size: %d. Total size: %d SeqNr: %i", 
							pRxHeader->ReceiveStatus.FrameLength, Length, 
							be16_to_cpu((*((UInt16*)(pPrevData+ sizeof(Dta1xxDmaTxHeader)+0x2c)))));
					} else {
						DTA1XX_LOG( KERN_INFO,
									"Dta1xxNwIpRxWorkerThread: Packet complete. Size: %d. Total size: %d", 
									pRxHeader->ReceiveStatus.FrameLength, Length);
					}
#endif // DBG
#endif

					// Copy next part of packet
					BytesToCopy = pRxHeader->ReceiveStatus.FrameLength + sizeof(Dta1xxDmaRxHeader) - *pPrevDataLength;
					RtlCopyMemory(pPrevData+*pPrevDataLength, pRxData, BytesToCopy);
					*pPrevDataLength+= BytesToCopy;
					
					if ((BytesToCopy % 4) != 0) {
						BytesToCopy += (4 - (BytesToCopy % 4));
					}
					pRxData+= BytesToCopy;
					Length-= BytesToCopy;
					
					if (pRxHeader->ReceiveStatus.FrameLength == 0) {
						// End of chain
						Length = 0;
#if LOG_LEVEL_IPRX > 1
						DTA1XX_LOG(
							KERN_INFO,
							"Dta1xxNwIpRxWorkerThread: End of chain. Time: %06lums",
							Dta1xxGenGetRefClkCntReg(pFdoExt->m_pGenRegs)/DTA160_REF_CLOCK);
#endif
					} else {
						// Store the packet in the DVB/Non DVB receive queue
						Dta1xxNwIpRxParsePacket(pNwStore, *pPrevDataLength, pPrevData);
					}
					*pPrevDataLength = 0;
				} 
			} else {
#if LOG_LEVEL_IPRX > 1
				DTA1XX_LOG( KERN_INFO, "Dta1xxNwIpRxWorkerThread: Header complete but INCORRECT." );
#endif
				// Restart searching header at begin of buffer
				pRxData = pTmpRxData;
				Length	= TmpLength;
				*pPrevDataLength = 0;
			}
		}
	}
	
	// Parse the DMA part
	while (Length) {
		if (Length < sizeof(Dta1xxDmaRxHeader)) {
			// Not a complete packet available
			// Copy part of header into temp. buffer for later
			RtlCopyMemory(pPrevData, pRxData, Length);
			*pPrevDataLength = Length;
#if LOG_LEVEL_IPRX > 1
			DTA1XX_LOG( KERN_INFO, "Dta1xxNwIpRxWorkerThread: Partly packet stored. Size: %d.", 
					    *pPrevDataLength);
#endif
			break;
		}
		pRxHeader = (Dta1xxDmaRxHeader*)pRxData;
		CorrPacket = TRUE;

		// Check if the packet header is correct
		if (((pRxHeader->ReceiveStatus.FrameLength + sizeof(Dta1xxDmaRxHeader)) >= 2000) || 
			(pRxHeader->Tag != 0x445441A0) || 
			(pRxHeader->Length != sizeof(Dta1xxDmaRxHeader))){
			// Error in packet header
			DTA1XX_LOG( KERN_INFO, "Dta1xxNwIpRxWorkerThread: Incorrect packet header" );

			CorrPacket = FALSE;
			// Search for tag in packet
			while ((!CorrPacket) && (Length >= sizeof(Dta1xxDmaRxHeader))) {
				if ((pRxHeader->Tag != 0x445441A0) || 
					(pRxHeader->Length != sizeof(Dta1xxDmaRxHeader)) ||
					((pRxHeader->ReceiveStatus.FrameLength + sizeof(Dta1xxDmaRxHeader)) >= 2000)){
					pRxData++;
					pRxHeader = (Dta1xxDmaRxHeader*)pRxData;
					Length--;
				} else {
					CorrPacket = TRUE;
					DTA1XX_LOG( KERN_INFO, "Dta1xxNwIpRxWorkerThread: Correct packet header found" );
				}
			}
		}

		if (!CorrPacket) {
#if LOG_LEVEL_IPRX > 1
			DTA1XX_LOG( KERN_INFO, "Dta1xxNwIpRxWorkerThread: NO correct packet header found" );
#endif
			break;
		}
		
		if (Length < (pRxHeader->ReceiveStatus.FrameLength + sizeof(Dta1xxDmaRxHeader))) {
			// Not a complete packet available
			// Copy part of header into temp. buffer for later
			RtlCopyMemory(pPrevData, pRxData, Length);
			*pPrevDataLength = Length;
#if LOG_LEVEL_IPRX > 1
			DTA1XX_LOG( KERN_INFO,
						"Dta1xxNwIpRxWorkerThread: Partly packet stored. Size: %d. Packet size: %d", 
						*pPrevDataLength, pRxHeader->ReceiveStatus.FrameLength);
#endif
			break;
		}

		if (pRxHeader->ReceiveStatus.FrameLength == 0) {
			// End of chain
#if LOG_LEVEL_IPRX > 1
			DTA1XX_LOG( KERN_INFO,
						"Dta1xxNwIpRxWorkerThread: End of chain. Time: %06lums",
						Dta1xxGenGetRefClkCntReg(pFdoExt->m_pGenRegs)/DTA160_REF_CLOCK );
#endif
			break;
		}

		// Complete packet available

		BytesToCopy = pRxHeader->ReceiveStatus.FrameLength + sizeof(Dta1xxDmaRxHeader);
		// Store the packet in the DVB/Non DVB receive queue
		Dta1xxNwIpRxParsePacket(pNwStore, BytesToCopy, pRxData);
#if LOG_LEVEL_IPRX > 1
#if DBG
		if (pRxHeader->ReceiveStatus.FrameLength >= 1374) {//Possibly RTP
			DTA1XX_LOG( KERN_INFO,
				"Dta1xxNwIpRxWorkerThread: Packet complete. Size: %d. Total size: %d SeqNr: %i", 
				pRxHeader->ReceiveStatus.FrameLength, Length, 
				be16_to_cpu((*((UInt16*)(pRxData+ sizeof(Dta1xxDmaTxHeader)+0x2c)))) );
		} else {

			DTA1XX_LOG( KERN_INFO,
						"Dta1xxNwIpRxWorkerThread: Packet complete. Size: %d. Total size: %d", 
						pRxHeader->ReceiveStatus.FrameLength, Length);
		}
#endif // DBG
#endif
		
		if ((BytesToCopy % 4) != 0) {
			BytesToCopy += (4 - (BytesToCopy % 4));
		}
		pRxData+= BytesToCopy;

		if (Length <= BytesToCopy) {
			Length = 0;
		} else {
			Length -= BytesToCopy;
		}
	}
	// Check if new packets are added in previous loop
	if (pFdoExt->m_IPRxPacketAvail) {
		pFdoExt->m_IPRxPacketAvail = FALSE;
		set_bit(0, (void*)&pFdoExt->m_IPRxPacketAvailTrig);
		KeSetEvent(&pFdoExt->m_IPRxPacketAvailEvent, 0, FALSE);
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxWorkerThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// IP Rx worker thread.
//
#ifdef WINBUILD
void  Dta1xxNwIpRxWorkerThread(
    IN NwStore* pNwStore)
{
	PDTA1XX_FDO	pFdoExt = pNwStore->m_pFdoExt;
	PIRP					pIrp[NUM_IPRX_DMA_QUEUES];
	PMDL					pMdl[NUM_IPRX_DMA_QUEUES];
	PIO_STACK_LOCATION		pStack;
	IO_STATUS_BLOCK			IOStatus;
	NTSTATUS				Status;
	KEVENT KernelEvent[NUM_IPRX_DMA_QUEUES];
	PVOID Events[] = {
						(PVOID) &pFdoExt->m_IPRxThreadStopEvent,
						(PVOID) &KernelEvent[0],
					};
#else
int  Dta1xxNwIpRxWorkerThread(
    IN void* pData)
{
	NwStore* pNwStore = (NwStore*)pData;
	PDTA1XX_FDO	pFdoExt = pNwStore->m_pFdo;
	Int ret;
#endif
	BOOLEAN	StopThread = FALSE;
	Int Index = pNwStore->m_IpPortIndex;
	Int	CurIndex = 0;
	Int	Counter;
	Channel* pCh = pFdoExt->m_IpMqRx[Index];
	UInt8* pPrevData;
	UInt8* pRxData;
	UInt Length;
	UInt PrevDataLength = 0;

    pPrevData = ExAllocatePoolWithTag(NonPagedPool, 2048, '1ATD');
	if (pPrevData == NULL) {
#if LOG_LEVEL_IPRX > 0
		DTA1XX_LOG( KERN_INFO, "Allocating RxBuffer FAILED" );
#endif
		PsTerminateSystemThread(STATUS_SUCCESS);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG( KERN_INFO, "[NwRx%d] Dta1xxNwIpRxWorkerThread Started", Index );
#endif
#ifdef WINBUILD
	// Windows implementation
	for (Counter = 0; Counter < NUM_IPRX_DMA_QUEUES; Counter++) {
		// Initialize kernel event
		KeInitializeEvent(&KernelEvent[Counter], NotificationEvent, FALSE);

		// Allocate IRP
		// We need 2 stack locations. 1 for the completion routine with data content and one spare.
		// The spare one is needed because IoMarkIrpPending is executed in the completion routine. 
		// (IoMarkIrpPending is not needed for this thread). This needs an extra stack location.

		pIrp[Counter] = IoAllocateIrp(2, FALSE);	
		
		if (pIrp[Counter] == NULL) {
			DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxWorkerThread: ALLOCATING IRP %d FAILED",
					 Counter);
			PsTerminateSystemThread(STATUS_INSUFFICIENT_RESOURCES);
		}

		pMdl[Counter] = IoAllocateMdl(pNwStore->m_pIpRxBuffer[Counter], IPRX_DMA_BUFFER_SIZE, FALSE, TRUE,
									  pIrp[Counter]);
		MmBuildMdlForNonPagedPool(pMdl[Counter]);
	}

	// Queue all IRP's
	for (Counter=0; Counter<NUM_IPRX_DMA_QUEUES; Counter++) {

		// Initialise the IRP
		IoSetNextIrpStackLocation(pIrp[Counter]);

		IoSetCompletionRoutine(pIrp[Counter],
							  (PIO_COMPLETION_ROUTINE) Dta1xxCompletionRoutine,
							  (PVOID) &KernelEvent[Counter], TRUE, TRUE, TRUE);

		IoSetNextIrpStackLocation(pIrp[Counter]);
		pStack = IoGetCurrentIrpStackLocation(pIrp[Counter]);
		pStack->DeviceObject = pFdoExt->m_pFdo; 

		pIrp[Counter]->IoStatus.Status = STATUS_NOT_SUPPORTED;
		pIrp[Counter]->MdlAddress = pMdl[Counter];
		pStack->Parameters.DeviceIoControl.IoControlCode = IOCTL_DTA1XX_DMA_USERMEM_READ;

		// Insert IRP in device queue
		Status = Dta1xxDvcQueueInsertIrp(pFdoExt->m_IpMqRx[Index], pIrp[Counter]);
	}

	CurIndex = 0;
	while (!StopThread) {

		Events[1] = (PVOID) &KernelEvent[CurIndex];

		// Wait for completion
		Status = KeWaitForMultipleObjects(2, Events, WaitAny, Executive, KernelMode,
										  FALSE, NULL, NULL);
		if (Status == (STATUS_WAIT_0)) {
			StopThread = TRUE;
		}
		else if (Status == (STATUS_WAIT_0 + 1)) {

			// Reset the event. Event is used for the next IRP
			KeClearEvent(&KernelEvent[CurIndex]);

			Status = pIrp[CurIndex]->IoStatus.Status;

			if (Status != STATUS_SUCCESS) {
				DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxWorkerThread: IP Rx packet, Not "
							"Success, retry. Status=0x%08x", Status);
				IoReuseIrp(pIrp[CurIndex], STATUS_SUCCESS);
			} else {
				// Calculate packet size
				pRxData = pNwStore->m_pIpRxBuffer[CurIndex];
				
				// The pIrp[CurIrpIndex]->IoStatus.Information field of the IRP helds the 
				// number of bytes not transfered. (due to a timeout).
				Length	= IPRX_DMA_BUFFER_SIZE - pIrp[CurIndex]->IoStatus.Information;

				Dta1xxIpRxParseRxBuffer(pNwStore, pRxData, Length, pPrevData, &PrevDataLength);

				// Re-Initialise IRP
				IoReuseIrp(pIrp[CurIndex], STATUS_SUCCESS);

				// Initialise the IRP
				IoSetNextIrpStackLocation(pIrp[CurIndex]);
				
				IoSetCompletionRoutine(pIrp[CurIndex],
									   (PIO_COMPLETION_ROUTINE) Dta1xxCompletionRoutine,
									   (PVOID) &KernelEvent[CurIndex],
									   TRUE, TRUE, TRUE);

				IoSetNextIrpStackLocation(pIrp[CurIndex]);
				pStack = IoGetCurrentIrpStackLocation(pIrp[CurIndex]);
				pStack->DeviceObject = pFdoExt->m_pFdo; 

				pIrp[CurIndex]->IoStatus.Status = STATUS_NOT_SUPPORTED;
				pIrp[CurIndex]->MdlAddress = pMdl[CurIndex];
				pStack->Parameters.DeviceIoControl.IoControlCode =
															IOCTL_DTA1XX_DMA_USERMEM_READ;
		
				// Insert IRP in device queue
				Status = Dta1xxDvcQueueInsertIrp(pFdoExt->m_IpMqRx[Index],
												 pIrp[CurIndex]);
				CurIndex++;
				if (CurIndex == NUM_IPRX_DMA_QUEUES) {
					CurIndex = 0;
				}
			}
		}
	}

	// The thread must be stopped
	// Cancel the IRP's
	// It's possible that the IRP is already finished, but that's not a problem
	// because the IRP is allocated here and not freed by any other thread.

	for (Counter=0; Counter<NUM_IPRX_DMA_QUEUES; Counter++) {
		IoCancelIrp(pIrp[Counter]);
	}
	for (Counter=0; Counter<NUM_IPRX_DMA_QUEUES; Counter++) {
		KeWaitForSingleObject(&KernelEvent[Counter], Executive, KernelMode , FALSE, NULL);

		// IRP is ready
		IoFreeMdl(pMdl[Counter]);

		// Release the IRP
		IoFreeIrp(pIrp[Counter]);
	}
#else
	// Linux implementation
	for (Counter = 0; Counter < NUM_IPRX_DMA_QUEUES; Counter++) {
		// Nothing to do...
	}

	CurIndex = 0;
	// Start first Rx DMA transfer
	pCh->m_Dma.m_CurrentTransferLength = IPRX_DMA_BUFFER_SIZE;
	Dta1xxCdmaStartSegment(pCh, CurIndex);

	while (!StopThread) {

		// Wait for DMA done interrupt
		ret = wait_event_interruptible(pCh->m_CdmaEvent,
									(test_bit(0, (void*)&pCh->m_CdmaDone)) ||
									Dta1xxKThreadShouldStop());


		if ( Dta1xxKThreadShouldStop() || (ret!=0) /*-ERESTARTSYS*/ ){
			// Have to cancel the current CDMA
			Dta1xxCdmaStop(pCh);
			StopThread = TRUE;
		} else {
			Int NumEntries = pCh->m_CdmaShBuf.m_NumPages / NUM_IPRX_DMA_QUEUES;

			// Update cache
			pci_dma_sync_sg_for_cpu(pCh->m_pFdo->m_pPdo, 
										pCh->m_CdmaShBuf.m_pSglKernel + ( NumEntries * CurIndex), 
										NumEntries,
										DMA_FROM_DEVICE);
			
			// Calculate packet size
			pRxData = pNwStore->m_pIpRxBuffer[CurIndex];
			
			// The pIrp[CurIrpIndex]->IoStatus.Information field of the IRP helds the 
			// number of bytes not transfered. (due to a timeout).
			Length	= IPRX_DMA_BUFFER_SIZE - pCh->m_Dma.m_CurrentTransferLength;
			
			// Start new DMA transfer before parsing the data
			CurIndex++;
			if (CurIndex == NUM_IPRX_DMA_QUEUES) {
				CurIndex = 0;
			}
			pCh->m_Dma.m_CurrentTransferLength = IPRX_DMA_BUFFER_SIZE;
			Dta1xxCdmaStartSegment(pCh, CurIndex);

			// Parse received data
			Dta1xxIpRxParseRxBuffer(pNwStore, pRxData, Length, pPrevData, &PrevDataLength);
		}
	}
	// CDMA is not running anymore
#endif
	ExFreePool(pPrevData);
#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG(KERN_INFO, "[NwRx%d] Dta1xxNwIpRxWorkerThread Stopped", Index);
#endif

	PsTerminateSystemThread(STATUS_SUCCESS);

	return 0;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ RTP Rx / Fec reconstruction +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxReconstructThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#ifdef WINBUILD
void  Dta1xxNwIpRxReconstructThread(
	IN PDTA1XX_FDO  pFdoExt)		// Device extension
{
	NTSTATUS			Status;
	PVOID				Events[] = {
						(PVOID) &pFdoExt->m_IPRxThreadStopEvent,
						(PVOID) &pFdoExt->m_IPRxPacketAvailEvent,
						};
#else
int  Dta1xxNwIpRxReconstructThread(
	IN void* pData)		// Device extension
{
	Int ret;
	PDTA1XX_FDO  pFdoExt = (PDTA1XX_FDO)pData;
#endif
	BOOLEAN				StopThread = FALSE;
	
	Int ChannelIdx;
	UserIpRxChannel* pIpRx;

#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxReconstructThread Started");
#endif

	while (!StopThread) {
#ifdef WINBUILD
		// Wait for new received packets or stop event
		Status = KeWaitForMultipleObjects(2, Events, WaitAny, Executive, KernelMode,
										  FALSE, NULL, NULL);
		if (Status == (STATUS_WAIT_0)) {
			StopThread = TRUE;
		}
		else if (Status == (STATUS_WAIT_0 + 1)) {
#else
		// Wait for new recived packets or sto event
		ret = wait_event_interruptible(pFdoExt->m_IPRxPacketAvailEvent,
									(test_bit(0, (void*)&pFdoExt->m_IPRxPacketAvailTrig)) ||
									Dta1xxKThreadShouldStop());
		clear_bit(0, (void*)&pFdoExt->m_IPRxPacketAvailTrig);

		if ( (ret!=0) /*-ERESTARTSYS*/ || Dta1xxKThreadShouldStop()){
			StopThread = TRUE;
		}
		else {
#endif
			// The event is reset automatically (Synchronisation event)
			
			// Walk through all the IP Rx buffers
			// Wait until the Rx structure is available
			ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex2);
	
			// Search the Rx channels for the DVB stream and return the ChannelIndex
			// if found
			pIpRx = pFdoExt->m_pIpRx;
			while (pIpRx) {
				// Is the channel receiving?
				if (pIpRx->m_RxControl != DTA1XX_RXCTRL_RCV) {
					pIpRx = pIpRx->m_pNext;
					continue;
				}

				if (!pIpRx->m_RxIncomingPackets) {
					pIpRx = pIpRx->m_pNext;
					continue;
				}
				
				pIpRx->m_RxIncomingPackets = FALSE;

				// Do something with the struct, temporarily disable lock after incrementing use count
				// Increment use count
				InterlockedIncrement(&pIpRx->m_UseCount);

				// Retrieve the index of the channel.
				ChannelIdx = pIpRx->m_ChannelIndex;
				
				// Release Mutex
				ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex2);

				if (pIpRx->m_FecMode == DTA1XX_IP_FEC_DISABLE) {
					// No Forward error correction
					Dta1xxNwIpRxProcessDvbWithoutFEC(pIpRx);
				} else {
					// FEC enabled
					Dta1xxNwIpRxProcessDvbWithFEC(pIpRx);
				}
				
				// Decrement use count
				InterlockedDecrement(&pIpRx->m_UseCount);
		
				// Wait until the Rx structure is available again
				ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex2);
	
				pIpRx = Dta1xxNwIpRxGetChannelUnsafe(pFdoExt, ChannelIdx);
				
				// Restart at first element if channel is deleted
				if (pIpRx == NULL) 
					pIpRx = pFdoExt->m_pIpRx;
				else 
					pIpRx = pIpRx->m_pNext;
			}

			// Release Mutex
			ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex2);
		}
	}

#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG(KERN_INFO, "Dta1xxNwIpRxReconstructThread Stopped");
#endif

	PsTerminateSystemThread(STATUS_SUCCESS);
	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxResetRtpBuffers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Move all RTP packets from m_RtpDvbList, m_RtpFecColumnList and m_RtpFecRowList
// to the m_RtpEmptyList list.
//

void Dta1xxNwIpRxResetRtpBuffers(
 	IN  UserIpRxChannel*  pIpRx)		// IP Rx channel
{
	PLIST_ENTRY pListEntry;
	KIRQL OldIrql;

	KeAcquireSpinLock(&pIpRx->m_RtpListSpinLock, &OldIrql);

	while (!IsListEmpty(&pIpRx->m_RtpDvbList)) {
		pListEntry = RemoveHeadList(&pIpRx->m_RtpDvbList);
		InsertTailList(&pIpRx->m_RtpEmptyList, pListEntry);
	}

	while (!IsListEmpty(&pIpRx->m_RtpFecColumnList)) {
		pListEntry = RemoveHeadList(&pIpRx->m_RtpFecColumnList);
		InsertTailList(&pIpRx->m_RtpEmptyList, pListEntry);
	}

	while (!IsListEmpty(&pIpRx->m_RtpFecRowList)) {
		pListEntry = RemoveHeadList(&pIpRx->m_RtpFecRowList);
		InsertTailList(&pIpRx->m_RtpEmptyList, pListEntry);
	}

	KeReleaseSpinLock(&pIpRx->m_RtpListSpinLock, OldIrql);
}
	
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxInitRtpBuffers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS Dta1xxNwIpRxInitRtpBuffers(
	IN  UserIpRxChannel*  pIpRx)		// IP Rx channel
{	UInt i;
	RtpListEntry* pRtpList;
	
	KeInitializeSpinLock(&pIpRx->m_RtpListSpinLock);
	
	// Initialize the double linked lists
	InitializeListHead(&pIpRx->m_RtpEmptyList);
	InitializeListHead(&pIpRx->m_RtpDvbList);
	InitializeListHead(&pIpRx->m_RtpFecColumnList);
	InitializeListHead(&pIpRx->m_RtpFecRowList);

	if (pIpRx->m_pRtpListEntries) {
		ExFreePool(pIpRx->m_pRtpListEntries); 
	}

	pIpRx->m_pRtpListEntries = ExAllocatePoolWithTag(
								NonPagedPool, 
								DTA1XX_IPRX_MAX_RTP_PACKETS * sizeof(RtpListEntry),
								'1ATD');

	if (pIpRx->m_pRtpListEntries == NULL) {
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	pRtpList = (RtpListEntry*)pIpRx->m_pRtpListEntries;
	
	// Add all entries to the empty list
	for (i = 0; i < DTA1XX_IPRX_MAX_RTP_PACKETS; i++) {
		pRtpList->m_BufIndex = (UInt16)i;
		pRtpList->m_InUse = 0;
		InsertTailList(&pIpRx->m_RtpEmptyList, &pRtpList->m_ListEntry);
		pRtpList+=1;
	}

	return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxFreeFecBuffers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void Dta1xxNwIpRxFreeRtpBuffers(
	IN  UserIpRxChannel*  pIpRx)		// IP Rx channel
{
	if (pIpRx->m_pRtpListEntries) {
		ExFreePool(pIpRx->m_pRtpListEntries); 	
	}

	pIpRx->m_pRtpListEntries = NULL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxRtpGetFreeEntry -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns a new RtpListEntry from the m_RtpEmptyList list
// If no entries are available NULL is returned
//
RtpListEntry* Dta1xxNwIpRxRtpGetFreeEntry(
	IN  UserIpRxChannel*  pIpRx)		// IP Rx channel
{
	RtpListEntry* pRtpEntry = NULL;
	PLIST_ENTRY pListEntry;
	KIRQL OldIrql;
	
	KeAcquireSpinLock(&pIpRx->m_RtpListSpinLock, &OldIrql);

	if (!IsListEmpty(&pIpRx->m_RtpEmptyList)) {
		pListEntry = RemoveHeadList(&pIpRx->m_RtpEmptyList);
		pRtpEntry = CONTAINING_RECORD(pListEntry, RtpListEntry, m_ListEntry);
		pRtpEntry->m_InUse = 0;
	}

	KeReleaseSpinLock(&pIpRx->m_RtpListSpinLock, OldIrql);
	return pRtpEntry;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxRtpSetEntryToEmptyList -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Put a RtpListEntry to the m_RtpEmptyList list.
//
void Dta1xxNwIpRxRtpSetEntryToEmptyList(
	IN  UserIpRxChannel*  pIpRx,		// IP Rx channel
	RtpListEntry* pRtpEntry)
{
	KIRQL OldIrql;
	
	KeAcquireSpinLock(&pIpRx->m_RtpListSpinLock, &OldIrql);
	InsertTailList(&pIpRx->m_RtpEmptyList, &pRtpEntry->m_ListEntry);
	KeReleaseSpinLock(&pIpRx->m_RtpListSpinLock, OldIrql);
}

//-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxRtpSetEntryToEmptyListUnsafe -.-.-.-.-.-.-.-.-.-.-.-
//
// Pre: The m_RtpListSpinLock must be aquired
//
void Dta1xxNwIpRxRtpSetEntryToEmptyListUnsafe(
	IN  UserIpRxChannel*  pIpRx,		// IP Rx channel
	RtpListEntry* pRtpEntry)
{
	InsertTailList(&pIpRx->m_RtpEmptyList, &pRtpEntry->m_ListEntry);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxRtpGetEntry -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns the first RtpListEntry from the pRtpListHead list
// If the list is empty, NULL is returned
//
RtpListEntry* Dta1xxNwIpRxRtpGetEntry(
	IN  UserIpRxChannel*  pIpRx,		// IP Rx channel
	IN  PLIST_ENTRY   pRtpListHead)
{
	RtpListEntry* pRtpEntry = NULL;
	PLIST_ENTRY pListEntry;
	KIRQL OldIrql;
	
	KeAcquireSpinLock(&pIpRx->m_RtpListSpinLock, &OldIrql);

	if (!IsListEmpty(pRtpListHead)) {
		pListEntry = RemoveHeadList(pRtpListHead);
		pRtpEntry = CONTAINING_RECORD(pListEntry, RtpListEntry, m_ListEntry);
	}

	KeReleaseSpinLock(&pIpRx->m_RtpListSpinLock, OldIrql);
	return pRtpEntry;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxRtpIsSeqNumLess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Sequence number max. 16-bit
// return TRUE: SeqNum1 < SeqNum2
//        FALSE: SeqNum1 >= SeqNum2
//
BOOLEAN Dta1xxNwIpRxRtpIsSeqNumLess(UInt16 SeqNum1, UInt16 SeqNum2)
{
	if (SeqNum2 > SeqNum1) {   
		if ((SeqNum2 - SeqNum1) < 0xFFF) return TRUE; else return FALSE;
    }        
    else if (SeqNum1 > SeqNum2) {   
		if ((SeqNum1 - SeqNum2) < 0x7FFF) return FALSE;	
		if ((0xffff - SeqNum1 + SeqNum2) < 0xFFF) return TRUE; else return FALSE;
	}
	return FALSE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxRtpAddEntryToListUnsafe -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// pRtpEntry is added and sorted into the pRtpListHead list if the packet not already in
// the list
// FALSE is returned if the Entry was already in the list.
// Pre: The m_RtpListSpinLock must be aquired
//
BOOLEAN Dta1xxNwIpRxRtpAddEntryToListUnsafe(
	IN  PLIST_ENTRY   pRtpListHead,
	IN  RtpListEntry* pRtpEntry)
{
	BOOLEAN		Found;
	BOOLEAN		Exist;
	PLIST_ENTRY pEntry;
	RtpListEntry* pRtpQueuedEntry;

	// Sort the RTP packet
	// Walk through the list and insert into the list.
	// If the packet with the same sequence number already exists return false
	// and skip insertion
	Found = FALSE;
	Exist = FALSE;
	pEntry = ListGetNext(pRtpListHead);

	while ((pEntry != pRtpListHead) && (!Found)) {
		pRtpQueuedEntry = CONTAINING_RECORD(pEntry, RtpListEntry, m_ListEntry);
		if (pRtpEntry->m_RTPSequenceNumber == pRtpQueuedEntry->m_RTPSequenceNumber) {
			Found = TRUE;
			Exist = TRUE;
		} else {
			if (Dta1xxNwIpRxRtpIsSeqNumLess(pRtpEntry->m_RTPSequenceNumber, 
											pRtpQueuedEntry->m_RTPSequenceNumber)) {
				Found = TRUE;
			} else {
				pEntry = ListGetNext(pEntry);
			}
		}
	}

	if (!Exist) {
		if (!Found) {
			// Add element at the end
			InsertTailList(pRtpListHead, &pRtpEntry->m_ListEntry);
		} else {
			// Insert element before entry
			InsertTailList(pEntry, &pRtpEntry->m_ListEntry);
		}
	}

	return !Exist;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxRtpAddEntryToList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// pRtpEntry is added and sorted into the pRtpListHead list if the packet not already in
// the list
// FALSE is returned if the Entry was already in the list.
//
BOOLEAN Dta1xxNwIpRxRtpAddEntryToList(
	IN  UserIpRxChannel*  pIpRx,		// IP Rx channel
	IN  PLIST_ENTRY   pRtpListHead,
	IN  RtpListEntry* pRtpEntry)
{
	BOOLEAN		Added;
	KIRQL OldIrql;
	
	KeAcquireSpinLock(&pIpRx->m_RtpListSpinLock, &OldIrql);
	Added = Dta1xxNwIpRxRtpAddEntryToListUnsafe(pRtpListHead, pRtpEntry);
	KeReleaseSpinLock(&pIpRx->m_RtpListSpinLock, OldIrql);
	return Added;
}

//.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxRtpGetPointerToDvbEntryUnsafe -.-.-.-.-.-.-.-.-.-.-.-
//
// Returns a RtpListEntry for a given SequenceNumber, or NULL if the entry does not exist
// If pLastUsedItem <> NULL, the search starts at pLastUsedItem
// Pre: The m_RtpListSpinLock must be acquired 
// 
RtpListEntry* Dta1xxNwIpRxRtpGetPointerToDvbEntryUnsafe(
	IN  UserIpRxChannel*  pIpRx,		// IP Rx channel
	IN  UInt16 SequenceNumber,
	IN  LIST_ENTRY* pLastUsedItem)
{
	BOOLEAN		Found;
	PLIST_ENTRY pEntry;
	RtpListEntry* pRtpQueuedEntry = NULL;
	
	Found = FALSE;
	if (pLastUsedItem) {
		pEntry = ListGetNext(pLastUsedItem);
	} else {
		pEntry = ListGetNext(&pIpRx->m_RtpDvbList);
	}

	while ((pEntry != &pIpRx->m_RtpDvbList) && (!Found)) {
		pRtpQueuedEntry = CONTAINING_RECORD(pEntry, RtpListEntry, m_ListEntry);
		if (SequenceNumber == pRtpQueuedEntry->m_RTPSequenceNumber) {
			Found = TRUE;
		} else {
			pEntry = ListGetNext(pEntry);
		}
	}
	
	if (Found) {
		return pRtpQueuedEntry;
	}
	return NULL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxGetFecReconstructionList -.-.-.-.-.-.-.-.-.-.-.-.-
//
// Returns the FEC Packet belonging to the SequenceNumber if all packets to reconstruct
// are available. The pReconstructList is filled with all DVB packets needed to reconstruct
// Pre: The m_RtpListSpinLock must be acquired 
//
RtpListEntry* Dta1xxNwIpRxGetFecReconstructionList(
	IN  UserIpRxChannel*  pIpRx,		// IP Rx channel
	IN LIST_ENTRY* pFecList,
	IN UInt16 SequenceNumber,
	OUT RtpListEntry* pReconstructList[MAX_FEC_RECONSTR_ELEMENTS])
{
	LIST_ENTRY* pListEntry;
	LIST_ENTRY* pDvbListEntry = NULL;
	RtpListEntry* pRtpEntry = NULL;
	RtpListEntry* pRtpDvbEntry;
	BOOLEAN Found = FALSE;
	UInt32 EndSeqNum;
	UInt32 SeqNum;
	Int Counter;
	Int Index = 0;
	UInt16 SeqNrOffset;

	pListEntry = ListGetNext(pFecList);

	while ((!Found) && (pListEntry != pFecList)) {
		pRtpEntry = CONTAINING_RECORD(pListEntry, RtpListEntry, m_ListEntry);
		
		//Check if required sequence number is in the current FEC range.
		EndSeqNum = (pRtpEntry->m_FecNA * pRtpEntry->m_FecOffset) + pRtpEntry->m_FecSNBase;

        if (((SequenceNumber >= pRtpEntry->m_FecSNBase) &&
			 (SequenceNumber < EndSeqNum)) ||
			 ((EndSeqNum > 0xffff) && (SequenceNumber < (EndSeqNum % 0x10000)))) {
			// Required sequence number is in the correct range, 
			// Now check if the offset is at the correct position.
			// This check is only needed for column FEC packets,
			// for ROW packets: Offset always 1!!
			SeqNrOffset = (Int)SequenceNumber - (Int)pRtpEntry->m_FecSNBase;
			if ((SeqNrOffset % pRtpEntry->m_FecOffset) == 0) {
            
				// This FEC belongs to the needed sequence number
				Found = TRUE;

				// Set search starting point to NULL, start from begin
				pDvbListEntry = NULL;
				
				for (Counter = 0; (Counter < pRtpEntry->m_FecNA) && Found; Counter++) {

					SeqNum = (UInt32)pRtpEntry->m_FecSNBase + 
							 ((UInt32)pRtpEntry->m_FecOffset * Counter);

					SeqNum = SeqNum % 0x10000;
                    if (SequenceNumber == SeqNum) {
                        //Skip this one, because we need to reconstruct it
                    } else
                    {
						pRtpDvbEntry = Dta1xxNwIpRxRtpGetPointerToDvbEntryUnsafe(pIpRx, (UInt16)SeqNum, pDvbListEntry);
						
						if (pRtpDvbEntry) {
							//InsertHeadList(pReconstructList, &pRtpDvbEntry->m_ListEntry);
							pReconstructList[Index++] = pRtpDvbEntry;

							// Next time start searching from the following point
							pDvbListEntry = &pRtpDvbEntry->m_ListEntry;
						} else Found = FALSE;
                    }
                }
			}
		}
		pListEntry = ListGetNext(pListEntry);
		
	}
	if (Found) { 
		return pRtpEntry;
	}
	return NULL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxTryReconstructPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Pre: The m_RtpListSpinLock must be acquired 
// Post: The m_RtpListSpinLock is released and acquired again!! 
// (New packets can be inserted to the list inbetween the release and acquire.)
//
RtpListEntry* Dta1xxNwIpRxTryReconstructPacket(
	IN  UserIpRxChannel*  pIpRx,		// IP Rx channel
	UInt16 SequenceNumber)
{	RtpListEntry* pRtpEntry = NULL;
	RtpListEntry* pRtpFecEntry = NULL;
	RtpListEntry* ReconstructList[MAX_FEC_RECONSTR_ELEMENTS];
	KIRQL OldIrql;
	Int i;

	pRtpFecEntry = Dta1xxNwIpRxGetFecReconstructionList(pIpRx, &pIpRx->m_RtpFecRowList, SequenceNumber, ReconstructList);
	
	if (pRtpFecEntry == NULL) {
		pRtpFecEntry = Dta1xxNwIpRxGetFecReconstructionList(pIpRx, &pIpRx->m_RtpFecColumnList, SequenceNumber, ReconstructList);
	}

	if (pRtpFecEntry != NULL) {
		if (!IsListEmpty(&pIpRx->m_RtpEmptyList)) {
			LIST_ENTRY* pListEntry;
			pListEntry = RemoveHeadList(&pIpRx->m_RtpEmptyList);

			// Mark all elements used for reconstruction
			pRtpFecEntry->m_InUse = 1;
			for (i = 0; i < pRtpFecEntry->m_FecNA - 1; i++) {
				ReconstructList[i]->m_InUse = 1;
			}
			
			// Release spinlock before reconstruction, so list can be accessed during
			// reconstruction
			KeReleaseSpinLock(&pIpRx->m_RtpListSpinLock, KeGetCurrentIrql());
			pRtpEntry = CONTAINING_RECORD(pListEntry, RtpListEntry, m_ListEntry);
			pRtpEntry->m_RTPSequenceNumber = SequenceNumber;
			if (!Dta1xxNwIpRxReconstructPacket(pIpRx, pRtpEntry, pRtpFecEntry, ReconstructList)) {
				// Reconstruction failed
				InsertTailList(&pIpRx->m_RtpEmptyList, &(pRtpEntry->m_ListEntry));
				pRtpEntry = NULL;
			}
			// Re-Acquire spinlock after reconstruction
			KeAcquireSpinLock(&pIpRx->m_RtpListSpinLock, &OldIrql);

			// Release all elements 
			pRtpFecEntry->m_InUse = 0;
			for (i = 0; i < pRtpFecEntry->m_FecNA - 1; i++) {
				ReconstructList[i]->m_InUse = 0;
			}

		} 
	}

	return pRtpEntry;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIpRxReconstructPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Remark: If a RTP header extension is available, skip the reconstruction because the
// IP packet lengths can have different sizes
//
BOOLEAN Dta1xxNwIpRxReconstructPacket(
	IN  UserIpRxChannel*  pIpRx,		// IP Rx channel
	OUT RtpListEntry* pRtpDestEntry, 
	IN RtpListEntry* pRtpFecEntry, 
	IN RtpListEntry* pReconstructList[MAX_FEC_RECONSTR_ELEMENTS])
{	
	FecHeader* pFecHeader;
	Dta1xxDmaRxHeader* pDmaRxHeader;
	RtpHeader* pRtpHeader;
	RtpHeader* pRtpHeaderDest;
	UdpHeader* pUdpHeaderDest;
	UInt8	PaddingBit;
	UInt8	ExtensionBit;
	UInt8	CCBits;
	UInt8	MarkerBit;
	UInt8	PayloadType;
	UInt32	TimeStamp;
	UInt16	Length;
	UInt8*	pDstData;
	UInt8*	pSrcData;
	UInt32*	pDstPayload;
	UInt32*	pSrcPayload;
	RtpListEntry* pRtpCurEntry;
	Int Counter;
	Int PacketSize;
	Int Index = 0;
	UInt16 RtpOffset;
	UInt16 PayloadOffset;
	
	// Source FEC
	pSrcData = (pIpRx->m_pRtpBuffer + (pRtpFecEntry->m_BufIndex * DTA1XX_IPRX_MAX_PACKET_LENGTH));
	pDmaRxHeader = (Dta1xxDmaRxHeader*)pSrcData;
	pSrcData+= sizeof(Dta1xxDmaRxHeader);

	pFecHeader   = (FecHeader*)(pSrcData + pRtpFecEntry->m_PayloadOffset - sizeof(FecHeader));
	pRtpHeader   = (RtpHeader*)(pSrcData + pRtpFecEntry->m_RTPOffset);
	
	// Skip reconstruction if RTP header extension is available
	if (pRtpHeader->m_Extension)
		return FALSE;

	PacketSize   = pDmaRxHeader->ReceiveStatus.FrameLength - pRtpFecEntry->m_PayloadOffset - 4;
	
	PaddingBit = 0;
	ExtensionBit = 0;
	CCBits = 0;
	MarkerBit = 0;
	
	PayloadType = pFecHeader->m_PtRecovery;
	TimeStamp = pFecHeader->m_TsRecovery;
	Length = pFecHeader->m_LengthRecovery;

	// Destination
	pDstData = (pIpRx->m_pRtpBuffer + 
				(pRtpDestEntry->m_BufIndex * DTA1XX_IPRX_MAX_PACKET_LENGTH));
	
	// Get first reconstruction packet to retrieve the offsets
	// if L == 1, no reconstruction packet available, so take the offsets from the FEC packet
	RtpOffset = pRtpFecEntry->m_RTPOffset;
	PayloadOffset = pRtpFecEntry->m_PayloadOffset - sizeof(FecHeader);

	// Copy start of packet into destination. Only needed for RAW ip mode and debugging
	if (pRtpFecEntry->m_FecNA > 1) {
		pRtpCurEntry = pReconstructList[Index];
		pSrcData = (pIpRx->m_pRtpBuffer + 
					(pRtpCurEntry->m_BufIndex * DTA1XX_IPRX_MAX_PACKET_LENGTH));

		RtlCopyMemory(pDstData, pSrcData, RtpOffset+sizeof(Dta1xxDmaRxHeader));
	} else {
		// No DVB packet available , so take the header from the FEC packet
		RtlCopyMemory(pDstData, pSrcData - sizeof(Dta1xxDmaRxHeader), RtpOffset+sizeof(Dta1xxDmaRxHeader));
	}
	
	pSrcData = (UInt8*)pDmaRxHeader + pRtpFecEntry->m_PayloadOffset + sizeof(Dta1xxDmaRxHeader);
	
	pDmaRxHeader = (Dta1xxDmaRxHeader*)pDstData;
	pDmaRxHeader->Timestamp			= 0;	// No timestamp available for this packet
	pRtpDestEntry->m_PayloadOffset	= PayloadOffset;
	pRtpDestEntry->m_RTPOffset		= RtpOffset;
	
	// Sequence number already filled by main thread, so skip here
	// pRtpDestEntry->m_RTPSequenceNumber
	
	pDstData+= sizeof(Dta1xxDmaRxHeader);
	pRtpHeaderDest   = (RtpHeader*)(pDstData + pRtpDestEntry->m_RTPOffset);
	pUdpHeaderDest	 = (UdpHeader*)(pDstData + pRtpDestEntry->m_RTPOffset - sizeof(UdpHeader));
	pDstData		 = pDstData + pRtpDestEntry->m_PayloadOffset;
	
	// Store the FEC recovery data into the destination packet
	RtlCopyMemory(pDstData, pSrcData, PacketSize);

	pDstPayload = (UInt32*)pDstData;

	while (Index < (pRtpFecEntry->m_FecNA - 1)) {
		pRtpCurEntry = pReconstructList[Index++];
		pSrcData = (pIpRx->m_pRtpBuffer + sizeof(Dta1xxDmaRxHeader) +
					(pRtpCurEntry->m_BufIndex * DTA1XX_IPRX_MAX_PACKET_LENGTH));
		pRtpHeader = (RtpHeader*)(pSrcData + pRtpCurEntry->m_RTPOffset);
		
		// Skip reconstruction if RTP header extension is available
		if (pRtpHeader->m_Extension)
			return FALSE;
		
		pSrcData+= pRtpCurEntry->m_PayloadOffset;
		pSrcPayload = (UInt32*)pSrcData;

		ASSERT ((PacketSize % 4) == 0);

		for (Counter = 0; Counter < (PacketSize / 4); Counter++) {
			pDstPayload[Counter] = pDstPayload[Counter] ^ pSrcPayload[Counter];
		}

		PaddingBit   = PaddingBit ^ pRtpHeader->m_Padding;
		ExtensionBit = ExtensionBit ^ pRtpHeader->m_Extension;
		CCBits = CCBits ^ pRtpHeader->m_CsrcCount;
		MarkerBit = MarkerBit ^ pRtpHeader->m_Marker;
		
		PayloadType = PayloadType ^ pRtpHeader->m_PayloadType;
		TimeStamp = TimeStamp ^ pRtpHeader->m_TimeStamp;
		Length = Length ^ RtlUshortByteSwap((USHORT)PacketSize);
	}
	
	// Reconstruction ready
	// Store header information into destination packet
	pRtpHeaderDest->m_CsrcCount = CCBits;
	pRtpHeaderDest->m_Extension = ExtensionBit;
	pRtpHeaderDest->m_Marker = MarkerBit;
	pRtpHeaderDest->m_Padding = PaddingBit;
	pRtpHeaderDest->m_PayloadType = PayloadType;
	pRtpHeaderDest->m_TimeStamp = TimeStamp;
	pRtpHeaderDest->m_Version = 2;
	pRtpHeaderDest->m_SequenceNumber = cpu_to_be16(pRtpDestEntry->m_RTPSequenceNumber);
	pUdpHeaderDest->m_Checksum = 0;
	pRtpHeaderDest->m_Ssrc = pRtpHeader->m_Ssrc;	// Stream ID

	ASSERT(pRtpHeaderDest->m_Extension == 0);

	// If header extension is active, we discard this packet because reconstruction 
	// may have been failed due to packet length differences
	if (pRtpHeaderDest->m_Extension != 0)
		return FALSE;
	return TRUE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxGetSequenceNumberGap -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt16 Dta1xxNwIpRxGetSequenceNumberGap(UInt16 SequenceNum1, UInt16 SequenceNum2)
{
	if (SequenceNum1 > SequenceNum2) {
		return 0xffff - SequenceNum1 + SequenceNum2 + 1;
	}
	return SequenceNum2 - SequenceNum1;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxGetMaxOutOfSync -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt16 Dta1xxNwIpRxGetMaxOutOfSync(
	IN UserIpRxChannel* pIpRx) 
{
	if ((pIpRx->m_DetFecNumColumns != 0) && (pIpRx->m_DetFecNumRows != 0)) {
		return MAX_OUTOFSYNC_SEQ_NUM + pIpRx->m_DetFecNumColumns * pIpRx->m_DetFecNumRows;
	}
	
	return MAX_OUTOFSYNC_SEQ_NUM + MAX_NUM_RTP_DVB_PACKETS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxProcessRtpDvbPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Copies all Rtp packets received (if in sequence and not needed anymore)
// to the User buffer
//

void Dta1xxNwIpRxProcessRtpDvbPacket(
	IN  UserIpRxChannel*  pIpRx,
	UInt16* pLastSeqNum) 
{
	Dta1xxDmaRxHeader* pDmaRxHeader;
	RtpListEntry* pRtpListEntry = NULL;
	RtpListEntry* pRtpListEntry2;
	KIRQL OldIrql;
	UInt16 LastSeqNum;
	UInt16 MaxOutOfSync;
	UInt16 MinPacketsInBuffer;
	UInt16 Gap;
	UInt NumPackets;
				

	if (pIpRx->m_FecMode == DTA1XX_IP_FEC_DISABLE) {
		MaxOutOfSync = MAX_OUTOFSYNC_SEQ_NUM;
		MinPacketsInBuffer = 0;
	} else {
		MaxOutOfSync = 3* Dta1xxNwIpRxGetMaxOutOfSync(pIpRx);
		
		// Is a Fec row + column packet received
		MinPacketsInBuffer = 3 * Dta1xxNwIpRxGetMaxOutOfSync(pIpRx);
	}

	while (TRUE) {
		KeAcquireSpinLock(&pIpRx->m_RtpListSpinLock, &OldIrql);

		if (pRtpListEntry) {
			// We don't need this packet anymore, add to empty List
			// Check if the entry is still available. It's possible that the
			// entry is already moved to the empty list due to a reset of the IpRx channel.
			if (pRtpListEntry == CONTAINING_RECORD(ListGetNext(&pIpRx->m_RtpDvbList), RtpListEntry, m_ListEntry)) {
				RemoveHeadList(&pIpRx->m_RtpDvbList);
				Dta1xxNwIpRxRtpSetEntryToEmptyListUnsafe(pIpRx, pRtpListEntry);
			}
		}

		if (!IsListEmpty(&pIpRx->m_RtpDvbList)) {
			// Let the entry stay in the list, but get the record
			pRtpListEntry = CONTAINING_RECORD(ListGetNext(&pIpRx->m_RtpDvbList), RtpListEntry, m_ListEntry);
		} else {
			KeReleaseSpinLock(&pIpRx->m_RtpListSpinLock, OldIrql);
			break;
		}

		// Retrieve the last received sequence number
		pRtpListEntry2 = CONTAINING_RECORD(ListGetPrev(&pIpRx->m_RtpDvbList), RtpListEntry, m_ListEntry);
		KeReleaseSpinLock(&pIpRx->m_RtpListSpinLock, OldIrql);
	
		if (pLastSeqNum) {
			LastSeqNum = *pLastSeqNum;
		} else {
			LastSeqNum = pRtpListEntry2->m_RTPSequenceNumber;
		}

		Gap = Dta1xxNwIpRxGetSequenceNumberGap(pRtpListEntry->m_RTPSequenceNumber, LastSeqNum);

        if ((Gap > MaxOutOfSync) ||
		    ((Dta1xxNwIpRxGetSequenceNumberGap(pIpRx->m_RtpLastSeqNum, pRtpListEntry->m_RTPSequenceNumber) == 1) && 
		    (!pIpRx->m_RtpFirstPacket) && 
			(pRtpListEntry->m_InUse == 0) &&
			(Gap >= MinPacketsInBuffer))) {	// Minimal MinPacketsInBuffer packets in buffer for reconstruction
			// Packet ready for user buffer

			// Count number of lost packets
			if (!pIpRx->m_RtpFirstPacket) {
				KeAcquireSpinLock(&pIpRx->m_StatisticsSpinLock, &OldIrql);
				NumPackets = Dta1xxNwIpRxGetSequenceNumberGap(pIpRx->m_RtpLastSeqNum, pRtpListEntry->m_RTPSequenceNumber);
				//ASSERT ((NumPackets > 0) && (NumPackets < 0x6000));
				pIpRx->m_NumPacketsNotReconstructed+=  (NumPackets - 1);
				pIpRx->m_TotNumPackets+= NumPackets;
				KeReleaseSpinLock(&pIpRx->m_StatisticsSpinLock, OldIrql);
#ifdef _DEBUG
	#if LOG_LEVEL_IPRX > 0
				if (NumPackets != 1) {
					DTA1XX_LOG(KERN_INFO, "[IpRx%d] ...RtpDvbPacket.BufIndex:%i,SeqNum:%i,Gap:%i,LSNTx:%i,LSNRx:%i", 
						pIpRx->m_ChannelIndex, pRtpListEntry->m_BufIndex, 
						pRtpListEntry->m_RTPSequenceNumber,
						Gap,
						pIpRx->m_RtpLastSeqNum,
						LastSeqNum);
				}
	#endif
#endif
			}
			pIpRx->m_RtpLastSeqNum = pRtpListEntry->m_RTPSequenceNumber;
			pIpRx->m_RtpFirstPacket = FALSE;

			pDmaRxHeader = (Dta1xxDmaRxHeader*)(pIpRx->m_pRtpBuffer + 
					(pRtpListEntry->m_BufIndex * DTA1XX_IPRX_MAX_PACKET_LENGTH));
#if LOG_LEVEL_IPRX > 2
			DTA1XX_LOG(KERN_INFO, "[IpRx%d] ...RtpDvbPacket.BufIndex:%i,SeqNum:%i,Gap:%i,LSNTx:%i,LSNRx:%i", 
					pIpRx->m_ChannelIndex, pRtpListEntry->m_BufIndex, 
					pRtpListEntry->m_RTPSequenceNumber,
					Gap,
					pIpRx->m_RtpLastSeqNum,
					LastSeqNum);
#endif
			
			// Store packet in DVB User buffer
			Dta1xxNwIpRxProcessDvbPayLoad(
				pIpRx,
				(UInt8*)pDmaRxHeader + pRtpListEntry->m_PayloadOffset + sizeof(Dta1xxDmaRxHeader),
				pDmaRxHeader->ReceiveStatus.FrameLength - pRtpListEntry->m_PayloadOffset - 4, // -4 for CRC
				pDmaRxHeader->Timestamp);
		} else {
			break;
		}
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxProcessDvbWithoutFEC -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxNwIpRxProcessDvbWithoutFEC(
	IN  UserIpRxChannel*  pIpRx)		// IP Rx channel
{
	Dta1xxNwIpRxProcessRtpDvbPacket(pIpRx, NULL); 
}
	
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxProcessDvbWithFEC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxNwIpRxProcessDvbWithFEC(
	IN  UserIpRxChannel*  pIpRx)		// IP Rx channel
{
	RtpListEntry* pRtpListEntry = NULL;
	RtpListEntry* pRtpListEntry2;
	RtpListEntry* pRtpReconstructedEntry;
	KIRQL OldIrql;
	UInt16 LastSeqNum;
	UInt16 CurSeqNum;
	BOOLEAN Stop;
	BOOLEAN PacketReconstructed;
	LIST_ENTRY* pBackElement;
	BOOLEAN SeqNumMissing = FALSE;
	UInt16 MissingSeqNumber = 0;
	BOOLEAN FecListEmpty = FALSE;
	BOOLEAN DvbListEmpty = FALSE;
	UInt16	NumPacketsTried = 0;
	UInt16 Gap;
	UInt16* pLastSeqNumToTx = NULL;
	
	KeAcquireSpinLock(&pIpRx->m_RtpListSpinLock, &OldIrql);
	
	// Try to reconstruct packets
	if ((!pIpRx->m_RtpFirstPacket) && (!IsListEmpty(&pIpRx->m_RtpDvbList))) {
		
		PacketReconstructed = TRUE;	// First time

		while (PacketReconstructed) {
			// Retrieve the last received sequence number
			pRtpListEntry2 = CONTAINING_RECORD(ListGetPrev(&pIpRx->m_RtpDvbList), RtpListEntry, m_ListEntry);
			LastSeqNum = pRtpListEntry2->m_RTPSequenceNumber;
			pLastSeqNumToTx = &LastSeqNum;
		
			PacketReconstructed = FALSE;
			SeqNumMissing		= FALSE;
			pRtpListEntry = CONTAINING_RECORD(ListGetNext(&pIpRx->m_RtpDvbList), RtpListEntry, m_ListEntry); 
			
			// The following works because m_RtpFirstPacket = FALSE;
			CurSeqNum = pIpRx->m_RtpLastSeqNum;
			
			while (TRUE) {
				if (IsListEmpty(&pIpRx->m_RtpFecColumnList) &&
					IsListEmpty(&pIpRx->m_RtpFecRowList)) {
					// No FEC packets available
					// No possibility to reconstruct
					FecListEmpty = TRUE;
					break;
				}

				Stop = FALSE;
				/*Stop = TRUE;
				DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxProcessDvbWithFEC. SKIP RECONSTRUCTION!!!!!!!", 
									pIpRx->m_ChannelIdx);
				*/
				while ((((UInt16)(CurSeqNum+1)) != pRtpListEntry->m_RTPSequenceNumber) && (!Stop)) {
					// We are missing at least one packet
					// Try to reconstruct packet if gap is too big
					if (!SeqNumMissing) {
						SeqNumMissing    = TRUE;
						MissingSeqNumber = CurSeqNum+1;
					}
					Gap = Dta1xxNwIpRxGetSequenceNumberGap((CurSeqNum+1), LastSeqNum);
					if (Gap<= MAX_OUTOFSYNC_SEQ_NUM) {
						// Gap is too small. The packet may be received later
						Stop = TRUE;
					} /*else if (Gap > (2 * Dta1xxNwIpRxGetMaxOutOfSync(pIpRx))){
						// Gap is to big, skip this one
						CurSeqNum++;
					}*/ else{
						// Try to reconstruct this packet
						CurSeqNum++;
						
						//ASSERT(((CurSeqNum % 200) == 0) || ((CurSeqNum % 401) == 0));
#if LOG_LEVEL_IPRX > 0
						DTA1XX_LOG(KERN_INFO, "[IpRx%d] TRY reconstruct SN:%i LSN:%i, NSN: %i", 
									pIpRx->m_ChannelIndex, CurSeqNum, LastSeqNum, pRtpListEntry->m_RTPSequenceNumber);
#endif

						// Store the backward link to known if some elements are inserted before the entry
						pBackElement = ListGetPrev(&pRtpListEntry->m_ListEntry);

						// After this function, the spinlock is released and acquired again
						// So some elements can be inserted inbetween
						pRtpReconstructedEntry = Dta1xxNwIpRxTryReconstructPacket(pIpRx, CurSeqNum);
						if (pRtpReconstructedEntry) {
							KIRQL OldIrql;
							KeAcquireSpinLock(&pIpRx->m_StatisticsSpinLock, &OldIrql);
							pIpRx->m_NumPacketsReconstructed++;
							KeReleaseSpinLock(&pIpRx->m_StatisticsSpinLock, OldIrql);

							PacketReconstructed = TRUE;
#if LOG_LEVEL_IPRX > 0
							DTA1XX_LOG(KERN_INFO, "[IpRx%d] Reconstructed SN:%i", 
									pIpRx->m_ChannelIndex, CurSeqNum);
#endif
							/*{
							UInt8* pData = (pIpRx->m_pRtpBuffer + (pRtpReconstructedEntry->m_BufIndex * DTA1XX_IPRX_MAX_PACKET_LENGTH));
							Dta1xxDmaRxHeader* pDmaRxHeader = (Dta1xxDmaRxHeader*)pData;
							pData+= sizeof(Dta1xxDmaRxHeader);
							Dta1xxNwIpRxAddPacketToNwQueue(tmppNwStore, pData, pDmaRxHeader->ReceiveStatus.FrameLength - 4);
							}*/
							if (pBackElement == ListGetPrev(&pRtpListEntry->m_ListEntry)) {
								// No elements inserted during reconstruction
								// Insert packet into list before entry
								InsertTailList(&pRtpListEntry->m_ListEntry , 
											&pRtpReconstructedEntry->m_ListEntry);
							} else {
								// Sort element into list
								if (!Dta1xxNwIpRxRtpAddEntryToListUnsafe(&pIpRx->m_RtpDvbList, pRtpReconstructedEntry)) {
									// Element already in queue
									// Add to empty list
									Dta1xxNwIpRxRtpSetEntryToEmptyListUnsafe(pIpRx, pRtpReconstructedEntry);
									// Get entry already in queue
									pRtpReconstructedEntry = Dta1xxNwIpRxRtpGetPointerToDvbEntryUnsafe(pIpRx, CurSeqNum, NULL);

								} 
								// Update next packet pointer
								ASSERT(pRtpReconstructedEntry != NULL);
								if (pRtpReconstructedEntry == NULL) {
									pRtpListEntry = NULL;
								} else {
									pRtpListEntry = CONTAINING_RECORD(ListGetNext(&pRtpReconstructedEntry->m_ListEntry), 
																	RtpListEntry, m_ListEntry);
								}
							}
						} else {
#if LOG_LEVEL_IPRX > 2
							DTA1XX_LOG(KERN_INFO, "[IpRx%d] NOT Reconstructed SN:%i", 
									pIpRx->m_ChannelIndex, CurSeqNum);
#endif
						}
					}
					NumPacketsTried++;
					if (NumPacketsTried > (2 * Dta1xxNwIpRxGetMaxOutOfSync(pIpRx))) {
						// Only try to recover the first 2 matrixes.
						Stop = TRUE;
					}
				}
				if (Stop) {
					break;
				}

				if (ListGetNext(&pRtpListEntry->m_ListEntry) == &pIpRx->m_RtpDvbList) {
					// End of list
					break;
				}
				
				pRtpListEntry = CONTAINING_RECORD(ListGetNext(&pRtpListEntry->m_ListEntry), RtpListEntry, m_ListEntry); 
				CurSeqNum++;
				NumPacketsTried++;
					
			}
		}
	} else {
		DvbListEmpty = TRUE;
	}
	
	// Remove old FEC packets
	if (!pIpRx->m_RtpFirstPacket) {
		// Check if reconstruction block has run
		if ((!DvbListEmpty) && (!FecListEmpty)) {
			if (SeqNumMissing) {
				UInt16 Gap;
				if (MissingSeqNumber > LastSeqNum) {
					Gap = 0xffff - MissingSeqNumber + LastSeqNum;
				} else {
					Gap = LastSeqNum - MissingSeqNumber;
					
				}
				if (Gap > 2 * Dta1xxNwIpRxGetMaxOutOfSync(pIpRx)) {
					LastSeqNum = LastSeqNum - 2 * Dta1xxNwIpRxGetMaxOutOfSync(pIpRx);
					pLastSeqNumToTx = NULL; // Let the transmitter decide to transmit the packets
				} else {
					LastSeqNum = MissingSeqNumber;
				}
			}
			// Remove all FEC Packets not needed for packets < LastSeqNum
#if LOG_LEVEL_IPRX > 2
			DTA1XX_LOG(KERN_INFO, "[IpRx%d] Dta1xxNwIpRxRemoveOldFecPackets. LastSeqNum:%d", 
					pIpRx->m_ChannelIndex, LastSeqNum);
#endif
			Dta1xxNwIpRxRemoveOldFecPackets(pIpRx, &pIpRx->m_RtpFecRowList, LastSeqNum);
			Dta1xxNwIpRxRemoveOldFecPackets(pIpRx, &pIpRx->m_RtpFecColumnList, LastSeqNum);
		}
	}
	KeReleaseSpinLock(&pIpRx->m_RtpListSpinLock, OldIrql);

	
	// Send all packets in sequence to user buffer
	Dta1xxNwIpRxProcessRtpDvbPacket(pIpRx, pLastSeqNumToTx); 
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxRemoveOldFecPackets -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Pre: The m_RtpListSpinLock must be acquired 
//
void Dta1xxNwIpRxRemoveOldFecPackets(
	IN UserIpRxChannel*  pIpRx,		// IP Rx channel
	IN LIST_ENTRY* pFecList,
	IN UInt16 LastSequenceNumber)
{
	LIST_ENTRY* pListEntry;
	LIST_ENTRY* pListEntryNext;
	RtpListEntry* pRtpEntry;
	UInt32 EndSeqNum;
	UInt16 MaxGap = Dta1xxNwIpRxGetMaxOutOfSync(pIpRx);
	
	if (IsListEmpty(pFecList)) {
		return;
	}

	pListEntry = ListGetNext(pFecList);

	while (pListEntry != pFecList) {
		pRtpEntry = CONTAINING_RECORD(pListEntry, RtpListEntry, m_ListEntry);
		
		// Get the last sequence number for this fec packet
		EndSeqNum = (pRtpEntry->m_FecNA * pRtpEntry->m_FecOffset) + pRtpEntry->m_FecSNBase;
		pListEntryNext = ListGetNext(pListEntry);
			
		if (Dta1xxNwIpRxRtpIsSeqNumLess((UInt16)EndSeqNum, LastSequenceNumber) || 
			((!Dta1xxNwIpRxRtpIsSeqNumLess((UInt16)EndSeqNum, LastSequenceNumber)) && 
			 (pRtpEntry->m_InUse == 0) &&
			 (Dta1xxNwIpRxGetSequenceNumberGap(LastSequenceNumber, (UInt16)EndSeqNum) > (4 * MaxGap)))) {
#if LOG_LEVEL_IPRX > 2
			DTA1XX_LOG(KERN_INFO, "[IpRx%d] ...RemoveOldFec. BufIndex:%i, SeqNum:%i, MaxSeqNum:%u", 
					pIpRx->m_ChannelIndex, pRtpEntry->m_BufIndex, 
					pRtpEntry->m_RTPSequenceNumber, EndSeqNum);
#endif
			RemoveEntryList(pListEntry);	
			Dta1xxNwIpRxRtpSetEntryToEmptyListUnsafe(pIpRx, pRtpEntry);
		}
        pListEntry = pListEntryNext;
	}
	return;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxGetChannel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UserIpRxChannel* Dta1xxNwIpRxGetChannel(
	IN PDTA1XX_FDO  pFdoExt,		// Device extension
	IN Int ChannelIndex)					// Channel Index
{
	UserIpRxChannel* pIpRx;
	
	ExAcquireFastMutex(&pFdoExt->m_IpRxFastMutex);
	
	pIpRx = pFdoExt->m_pIpRx;
	
	while (pIpRx) {
		if (pIpRx->m_ChannelIndex == ChannelIndex)
			break;
		else 
			pIpRx = pIpRx->m_pNext;
	}
	ExReleaseFastMutex(&pFdoExt->m_IpRxFastMutex);

	return pIpRx;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxGetChannelUnsafe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UserIpRxChannel* Dta1xxNwIpRxGetChannelUnsafe(
	IN PDTA1XX_FDO  pFdoExt,		// Device extension
	IN Int ChannelIndex)					// Channel Index
{
	UserIpRxChannel* pIpRx;
	
	pIpRx = pFdoExt->m_pIpRx;
	
	while (pIpRx) {
		if (pIpRx->m_ChannelIndex == ChannelIndex)
			break;
		else 
			pIpRx = pIpRx->m_pNext;
	}
	return pIpRx;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxRemoveChannel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Dta1xxNwIpRxRemoveChannel(
	IN PDTA1XX_FDO  pFdoExt,		// Device extension
	IN UserIpRxChannel* pIpRx)
{
	if (pIpRx->m_pPrev == NULL) {
		// This was the first element.
		pFdoExt->m_pIpRx = pIpRx->m_pNext;
		if (pIpRx->m_pNext)
			pIpRx->m_pNext->m_pPrev = NULL;
	} else {
		pIpRx->m_pPrev->m_pNext = pIpRx->m_pNext;
		if (pIpRx->m_pNext)
			pIpRx->m_pNext->m_pPrev = pIpRx->m_pPrev;
	}
}
