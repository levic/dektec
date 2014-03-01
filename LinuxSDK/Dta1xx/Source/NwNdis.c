//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NwNdis.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2005 DekTec
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	2005.08.11	RD	Created	

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwNdisSetNdisValue -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Helperfunction to store ULONG64 value into a result buffer
//
VOID  Dta1xxNwNdisSetNdisValue(
	ULONG64  Value,
	void*  pResult,
	ULONG*  OutResultLength)
{
	ULONG BufSize = (*OutResultLength);
	if (BufSize >= sizeof(ULONG64)) {
		*((ULONG64*)pResult) = Value;
		BufSize = sizeof(ULONG64);
	} else {
		*((ULONG*)pResult) = (ULONG)Value;
		BufSize = sizeof(ULONG);
	}
	*OutResultLength = BufSize;

#if LOG_LEVEL_NDIS > 2
	DTA1XX_LOG(KERN_INFO, "Dta1xxNwNdisSetNdisValue -->***Value = 0x%x***<--, Bufsize: 0x%x",
			 (ULONG)Value, (ULONG)BufSize);
#endif
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwNdisIoctlQueryInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS  Dta1xxNwNdisIoctlQueryInfo(
	ULONG  InBufferSize, 
	ULONG*  OutBufferSize, 
	PDTA1XX_NWI_INFO  pInfo,
	PDTA1XX_FDO  pFdoExt)
{
	Int  Index = pInfo->m_IpPortIndex;
	ULONG  InReqSize  = 0;				// Required length of input buffer
	ULONG  OutReqMinSize = 0;			// Minimal required length of output buffer
	ULONG  OutReqSize = 0;				// Required length of output buffer
	char*  IoctlStr;					// Mnemonic string for IOCTL code
	PNwStore  pNwStore;					// Network port attributes
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;
	ULONG  BytesWritten;


	// Common in/out sizes
	InReqSize = sizeof(DTA1XX_NWI_INFO) - sizeof(ULONG);
	OutReqSize = sizeof(DTA1XX_NWI_INFO) - sizeof(ULONG);
	OutReqMinSize = OutReqSize;
	BytesWritten = (*OutBufferSize) - sizeof(DTA1XX_NWI_INFO) + sizeof(ULONG);
	
	// Determine expected length of data buffer
	switch (pInfo->m_Oid) {

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_PERMANENT_ADDRESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_802_3_PERMANENT_ADDRESS:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_802_3_PERMANENT_ADDRESS";
		InReqSize += 6;
		OutReqSize += 6;
		OutReqMinSize = OutReqSize;
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_CURRENT_ADDRESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_CURRENT_ADDRESS:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_802_3_CURRENT_ADDRESS";
		InReqSize+= 6;
		OutReqSize+= 6;
		OutReqMinSize = OutReqSize;
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_MAXIMUM_LIST_SIZE -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_802_3_MAXIMUM_LIST_SIZE:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_802_3_MAXIMUM_LIST_SIZE";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG);
		OutReqMinSize = OutReqSize;
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_LINK_SPEED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_GEN_LINK_SPEED:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_GEN_LINK_SPEED";
	
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG);
		OutReqMinSize = OutReqSize;
		break;
	
	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_XMIT_OK -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_GEN_XMIT_OK:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_GEN_XMIT_OK";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;
	
	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_RCV_OK -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_GEN_RCV_OK:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_GEN_RCV_OK";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_XMIT_ERROR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_GEN_XMIT_ERROR:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_GEN_XMIT_ERROR";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_RCV_ERROR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_GEN_RCV_ERROR:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_GEN_RCV_ERROR";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;
	
	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_RCV_CRC_ERROR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_GEN_RCV_CRC_ERROR:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_GEN_RCV_CRC_ERROR";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;
	
	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_RCV_ERROR_ALIGNMENT -.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_RCV_ERROR_ALIGNMENT:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_802_3_RCV_ERROR_ALIGNMENT";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_ONE_COLLISION -.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_XMIT_ONE_COLLISION:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_802_3_XMIT_ONE_COLLISION";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_MORE_COLLISIONS -.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_802_3_XMIT_MORE_COLLISIONS:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_802_3_XMIT_MORE_COLLISIONS";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_DEFERRED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_802_3_XMIT_DEFERRED:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_802_3_XMIT_DEFERRED";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_MAX_COLLISIONS -.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_XMIT_MAX_COLLISIONS:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_802_3_XMIT_MAX_COLLISIONS";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_UNDERRUN -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_802_3_XMIT_UNDERRUN:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_802_3_XMIT_UNDERRUN";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_TIMES_CRS_LOST -.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_XMIT_TIMES_CRS_LOST:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_802_3_XMIT_TIMES_CRS_LOST";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_LATE_COLLISIONS -.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_802_3_XMIT_LATE_COLLISIONS:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_802_3_XMIT_LATE_COLLISIONS";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_RCV_NO_BUFFER -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_GEN_RCV_NO_BUFFER:
        IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_GEN_RCV_NO_BUFFER";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_CURRENT_PACKET_FILTER -.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_GEN_CURRENT_PACKET_FILTER:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_GEN_CURRENT_PACKET_FILTER";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_BYTES_RCV -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_GEN_BYTES_RCV:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_GEN_BYTES_RCV";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_BYTES_XMIT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_GEN_BYTES_XMIT:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_GEN_BYTES_XMIT";
		InReqSize += sizeof(ULONG);
		OutReqSize += sizeof(ULONG64);
		OutReqMinSize += sizeof(ULONG);
		break;

	default:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO: ??UNKNOWN OID CODE??";
		ReturnStatus = STATUS_NOT_IMPLEMENTED;
		
	}
	*OutBufferSize = OutReqSize;

#if LOG_LEVEL_NDIS > 1
	DTA1XX_LOG(KERN_INFO, "%s, In=%d (Rq=%d), Out=%d (Rq=%d)", IoctlStr,
			 InBufferSize, InReqSize, *OutBufferSize, OutReqSize);
#endif

	// Check buffer sizes
	if (InBufferSize < InReqSize) {
		DTA1XX_LOG(KERN_INFO, "IOCTL_DTA1XX_NWI_QUERY_INFO: INPUT BUFFER TOO SMALL!");
		return STATUS_INVALID_PARAMETER;
	}
	if ((*OutBufferSize) < OutReqMinSize) {
		DTA1XX_LOG(KERN_INFO, "IOCTL_DTA1XX_NWI_QUERY_INFO: OUTPUT BUFFER TOO SMALL!");
		return STATUS_INVALID_PARAMETER;
	}

	// Check port index
	if (Index<0 || Index>=pFdoExt->m_NumIpPorts) {
		DTA1XX_LOG(KERN_INFO, "IOCTL_DTA1XX_NWI_QUERY_INFO: PortIndex=%d IS OUT OF RANGE!!",
				 Index);
		return STATUS_INVALID_PARAMETER;
	}

	// Get pointer to MAC/PHY related attributes
	pNwStore = &(pFdoExt->m_NwStore[Index]);

	//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Execute IOCTL +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

	switch (pInfo->m_Oid) {
	
	//-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_PERMANENT_ADDRESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_802_3_PERMANENT_ADDRESS:
		{
			UInt8*  pAddress = &(pInfo->m_Data[0]);
			pAddress[0] = pNwStore->m_MacAddrPer[0];
			pAddress[1] = pNwStore->m_MacAddrPer[1];
			pAddress[2] = pNwStore->m_MacAddrPer[2];
			pAddress[3] = pNwStore->m_MacAddrPer[3];
			pAddress[4] = pNwStore->m_MacAddrPer[4];
			pAddress[5] = pNwStore->m_MacAddrPer[5];
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_CURRENT_ADDRESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_CURRENT_ADDRESS:
		{
			UInt8*  pAddress = &(pInfo->m_Data[0]);
			pAddress[0] = pNwStore->m_MacAddrCur[0];
			pAddress[1] = pNwStore->m_MacAddrCur[1];
			pAddress[2] = pNwStore->m_MacAddrCur[2];
			pAddress[3] = pNwStore->m_MacAddrCur[3];
			pAddress[4] = pNwStore->m_MacAddrCur[4];
			pAddress[5] = pNwStore->m_MacAddrCur[5];
		}
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_MAXIMUM_LIST_SIZE -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_802_3_MAXIMUM_LIST_SIZE:
		{
			// All protocol drivers can set a multicast address, so full is 
			// always returned if the 32 mac addresses is reached 
			ULONG*  pMaxListSize = (ULONG*)&(pInfo->m_Data[0]);
			*pMaxListSize = MAX_NUM_MC_ADDRESSES;
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_LINK_SPEED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_GEN_LINK_SPEED:
		{
			UInt  Speed = pNwStore->m_PhySpeed;
			ULONG*  pLinkSpeed = (ULONG*)&(pInfo->m_Data[0]);

			// LinkSpeed units is 100bps
			switch (Speed) {
			case 0:  *pLinkSpeed =   100000; break;		// 10Mbps
			case 1:  *pLinkSpeed =  1000000; break;		// 100Mbps
			case 2:  *pLinkSpeed = 10000000; break;		// 1000Mbps
			default: *pLinkSpeed =        0; break;		// Not supported
			}
		}
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_XMIT_OK -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_GEN_XMIT_OK:
		{
			ULONG  Counter = Dta1xxEthGetSingleCounter(pNwStore,
													   DTA1XX_SCM_REG_TXFRMOK);
			Dta1xxNwNdisSetNdisValue(Counter, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_RCV_OK -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_GEN_RCV_OK:
		{
			ULONG  Counter = Dta1xxEthGetSingleCounter(pNwStore,
													   DTA1XX_SCM_REG_RXFRMOK);
			Dta1xxNwNdisSetNdisValue(Counter, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_XMIT_ERROR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_GEN_XMIT_ERROR:
		{
			ULONG64  Error;
			Dta1xxEthLockCounters(pNwStore);
			Error  = Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL16);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_TXLCERR);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_TXMACERR);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_TXCSERR);
			Dta1xxEthUnLockCounters(pNwStore);

			Dta1xxNwNdisSetNdisValue(Error, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_RCV_ERROR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_GEN_RCV_ERROR:
		{
			ULONG64  Error;

			Dta1xxEthLockCounters(pNwStore);
			Error  = Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_RXALIGNERR);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_RXFCSERR);
			Dta1xxEthUnLockCounters(pNwStore);

			Dta1xxNwNdisSetNdisValue(Error, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_RCV_CRC_ERROR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_GEN_RCV_CRC_ERROR:
		{
			ULONG64  Error;
			Error = Dta1xxEthGetSingleCounter(pNwStore,
											  DTA1XX_SCM_REG_RXFCSERR);
			// Include all packets discarded by FPGA
			Error+= Dta1xxNwRxInvCntGet(pFdoExt->m_IpMqRx[Index]->m_pRegBase);

			Dta1xxNwNdisSetNdisValue(Error, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_RCV_ERROR_ALIGNMENT -.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_RCV_ERROR_ALIGNMENT:
		{
			ULONG64  Error;
			Error  = Dta1xxEthGetSingleCounter(pNwStore,
											   DTA1XX_SCM_REG_RXALIGNERR);
			Dta1xxNwNdisSetNdisValue(Error, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_ONE_COLLISION -.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_XMIT_ONE_COLLISION:
		{
			ULONG64  Error;
			Error  = Dta1xxEthGetSingleCounter(pNwStore,
											   DTA1XX_SCM_REG_COLL1);
			Dta1xxNwNdisSetNdisValue(Error, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;
	
	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_MORE_COLLISIONS -.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_802_3_XMIT_MORE_COLLISIONS:
		{
			ULONG64  Error;
			Dta1xxEthLockCounters(pNwStore);
			
			Error  = Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL2);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL3);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL4);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL5);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL6);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL7);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL8);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL9);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL10);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL11);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL12);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL13);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL14);
			Error += Dta1xxEthGetCounter(pNwStore,
										DTA1XX_SCM_REG_COLL15);

			Dta1xxEthUnLockCounters(pNwStore);

			Dta1xxNwNdisSetNdisValue(Error, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_DEFERRED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_802_3_XMIT_DEFERRED:
		{
			ULONG64  Error;
			Error  = Dta1xxEthGetSingleCounter(pNwStore, DTA1XX_SCM_REG_TXDEF);
			Dta1xxNwNdisSetNdisValue(Error, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_MAX_COLLISIONS -.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_XMIT_MAX_COLLISIONS:
		{
			ULONG64  Error;
			Error  = Dta1xxEthGetSingleCounter(pNwStore, DTA1XX_SCM_REG_COLL16);
			Dta1xxNwNdisSetNdisValue(Error, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_UNDERRUN -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_802_3_XMIT_UNDERRUN:
		{
			ULONG64  Error;
			Error  = Dta1xxEthGetSingleCounter(pNwStore, DTA1XX_SCM_REG_TXMACERR);
			Dta1xxNwNdisSetNdisValue(Error, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_TIMES_CRS_LOST -.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_XMIT_TIMES_CRS_LOST:
		{
			ULONG64  Error;
			Error  = Dta1xxEthGetSingleCounter(pNwStore, DTA1XX_SCM_REG_TXCSERR);
			Dta1xxNwNdisSetNdisValue(Error, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_XMIT_LATE_COLLISIONS -.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_802_3_XMIT_LATE_COLLISIONS:
		{
			ULONG64  Error;
			Error  = Dta1xxEthGetSingleCounter(pNwStore, DTA1XX_SCM_REG_TXLCERR);
			Dta1xxNwNdisSetNdisValue(Error, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_RCV_NO_BUFFER -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_GEN_RCV_NO_BUFFER:
		{
			ULONG64  Error;
			Error  = Dta1xxEthMissedFramesCounterGet(pNwStore->m_pRegBase);
			Error+=  pFdoExt->m_NwStore[Index].m_NumRxFifoOverflows;

#if LOG_LEVEL_NDIS > 2
			DTA1XX_LOG(KERN_INFO, "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_GEN_RCV_NO_BUFFER: %08x",
				(ULONG)Error);
#endif

			Dta1xxNwNdisSetNdisValue(Error, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_CURRENT_PACKET_FILTER -.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_GEN_CURRENT_PACKET_FILTER:
		{
			ULONG*  PacketFilter = (ULONG*)&(pInfo->m_Data[0]);
			*PacketFilter = 0;
			if ((pNwStore->m_MacPacketFilter&0xffff) != 0)
				*PacketFilter = NW_PACKET_TYPE_DIRECTED;
			if ((pNwStore->m_MacPacketFilter & NW_PACKET_TYPE_ALL_MULTICAST) != 0)
				*PacketFilter |= NW_PACKET_TYPE_ALL_MULTICAST;
			if ((pNwStore->m_MacPacketFilter & NW_PACKET_TYPE_PROMISCUOUS) != 0)
				*PacketFilter |= NW_PACKET_TYPE_PROMISCUOUS;
#if LOG_LEVEL_NDIS > 0
			DTA1XX_LOG(KERN_INFO, "IOCTL_DTA1XX_NWI_QUERY_INFO: OID_GEN_CURRENT_PACKET_FILTER, Filter: %08x",
				(ULONG)*PacketFilter);
#endif

		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_BYTES_RCV -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_GEN_BYTES_RCV:
		{
			ULONG64  Counter;
			Counter  = Dta1xxEthGetSingleCounter(pNwStore,
											   DTA1XX_SCM_REG_RXOCTOK);
			Dta1xxNwNdisSetNdisValue(Counter, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_BYTES_XMIT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case OID_GEN_BYTES_XMIT:
		{
			ULONG64  Counter;
			Counter  = Dta1xxEthGetSingleCounter(pNwStore,
											   DTA1XX_SCM_REG_TXOCTOK);
			Dta1xxNwNdisSetNdisValue(Counter, &(pInfo->m_Data[0]), &BytesWritten);
		}
		break;

	default:
#if LOG_LEVEL_NDIS > 0
		DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_NWI_QUERY_INFO: OID = 0x%x is not implemented",
				    (UInt)pInfo->m_Oid);
#endif
		ReturnStatus = STATUS_NOT_SUPPORTED;
	}

	pInfo->m_BytesWritten = BytesWritten;

	return ReturnStatus;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwNdisIoctlSetInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS  Dta1xxNwNdisIoctlSetInfo(
	ULONG  InBufferSize, 
	ULONG  OutBufferSize, 
	PDTA1XX_NWI_INFO  pInfo,
	PDTA1XX_FDO  pFdoExt)
{
	Int  Index = pInfo->m_IpPortIndex;
	char*  IoctlStr;					// Mnemonic string for IOCTL code
	ULONG  InReqSize  = 0;				// Required length of input buffer
	ULONG  OutReqSize = 0;				// Required length of output buffer
	NwStore*  pNwStore;					// Network attributes
	NTSTATUS  ReturnStatus = STATUS_SUCCESS;

	InReqSize = sizeof(DTA1XX_NWI_INFO) - sizeof(ULONG);
	
	// Determine expected length of data buffer,
	switch (pInfo->m_Oid) {	
	
	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_CURRENT_PACKET_FILTER -.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_GEN_CURRENT_PACKET_FILTER:
		IoctlStr = "IOCTL_DTA1XX_NWI_SET_INFO: OID_GEN_CURRENT_PACKET_FILTER";
		InReqSize += sizeof(ULONG);
		OutReqSize = 0;
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_MULTICAST_LIST -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_MULTICAST_LIST:
		IoctlStr = "IOCTL_DTA1XX_NWI_SET_INFO: OID_802_3_MULTICAST_LIST";
		// Sizes will be checked later
		OutReqSize = 0;
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_CURRENT_ADDRESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_CURRENT_ADDRESS:
		IoctlStr = "IOCTL_DTA1XX_NWI_SET_INFO: OID_802_3_CURRENT_ADDRESS";
		InReqSize += 6;
		OutReqSize = 0;
		break;

	default:
		IoctlStr = "IOCTL_DTA1XX_NWI_SET_INFO: ??UNKNOWN OID CODE??";
		ReturnStatus = STATUS_NOT_IMPLEMENTED;
	}

#if LOG_LEVEL_NDIS > 1
	DTA1XX_LOG( KERN_INFO, "%s, In=%d (Rq=%d), Out=%d (Rq=%d)", IoctlStr,
			    InBufferSize, InReqSize, OutBufferSize, OutReqSize);
#endif

	// Check buffer sizes
	if (InBufferSize < InReqSize) {
		DTA1XX_LOG(KERN_INFO, "IOCTL_DTA1XX_NWI_SET_INFO: INPUT BUFFER TOO SMALL!");
		return STATUS_INVALID_PARAMETER;
	}
	if (OutBufferSize < OutReqSize) {
		DTA1XX_LOG(KERN_INFO, "IOCTL_DTA1XX_NWI_SET_INFO: OUTPUT BUFFER TOO SMALL!");
		return STATUS_INVALID_PARAMETER;
	}

	// Check port index
	if (Index<0 || Index>=pFdoExt->m_NumIpPorts) {
		DTA1XX_LOG(KERN_INFO, "IOCTL_DTA1XX_NWI_SET_INFO: PortIndex=%d IS OUT OF RANGE!!",
				 Index);
		return STATUS_INVALID_PARAMETER;
	}

	// Get pointer to MAC/PHY related attributes
	pNwStore = &(pFdoExt->m_NwStore[Index]);

	//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Execute IOCTL +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

	switch (pInfo->m_Oid) {

	//.-.-.-.-.-.-.-.-.-.-.-.-.- OID_GEN_CURRENT_PACKET_FILTER -.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_GEN_CURRENT_PACKET_FILTER:
		{
			ULONG*  PacketFilter = (ULONG*)(&(pInfo->m_Data[0]));
			pNwStore->m_NwiPacketFilter = *PacketFilter;

			if (pNwStore->m_NwiPacketFilter & NW_PACKET_TYPE_PROMISCUOUS) {
				// Disable deletion of incorrect packets by FPGA
				Dta1xxNwRxCtrlSetDelPckFpga(pFdoExt->m_IpMqRx[Index]->m_pRegBase, 0);
#ifndef _DEBUG
				// Enable deletion of packets with packet length error
				// Only enabled in a release driver version
				// so NDIS test works with a release version
				Dta1xxNwRxCtrlSetDelLenPckFpga(pFdoExt->m_IpMqRx[Index]->m_pRegBase);
#endif
			} else {
				// Enable deletion of incorrect packets by FPGA
				Dta1xxNwRxCtrlSetDelPckFpga(pFdoExt->m_IpMqRx[Index]->m_pRegBase, 1);
			}
			
			if (pNwStore->m_NwiPacketFilter == 0) {
				// Disable NDis receive
				pNwStore->m_EnableNwiRx = FALSE;
#if LOG_LEVEL_NDIS > 0
				DTA1XX_LOG( KERN_INFO,
							"[NwRx%d] ****NDISPACKETFILTER = 0. NWI Rx DISABLED****",
							Index );
#endif
			} else {
				// Enable NDis receive
				pNwStore->m_EnableNwiRx = TRUE;
#if LOG_LEVEL_NDIS > 0
				DTA1XX_LOG( KERN_INFO,
							"[NwRx%d] ****NDISPACKETFILTER != 0. NWI Rx ENABLED****",
							Index );
#endif
			}
			Dta1xxNwQueueLinkAutoNegChangeDpc(pNwStore);
		}
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_MULTICAST_LIST -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_MULTICAST_LIST:
		InBufferSize = InBufferSize - InReqSize;

		// Check input buffer
		if (InBufferSize % 6) {
			ReturnStatus = NDIS_STATUS_INVALID_LENGTH;
			break;
		}

        if (InBufferSize > (MAX_NUM_MC_ADDRESSES * 6)) {
            ReturnStatus = NDIS_STATUS_MULTICAST_FULL;
            break;
        }
		// Store multicast list
		pNwStore->m_NumMulticast = (InBufferSize / 6);
		RtlCopyMemory(pNwStore->m_MCList, &(pInfo->m_Data[0]), InBufferSize);
		
		// Send setup frame
		if (!InsertQueueDpc(&pNwStore->m_SetupFrameDpc)) {
			// The SetupFrame is already in the DPC queue
#if LOG_LEVEL_NDIS > 0
			DTA1XX_LOG(KERN_INFO, "Skip sending setup frame. Already in DPC queue");
#endif
			pNwStore->m_SetupFramePending = TRUE;
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- OID_802_3_CURRENT_ADDRESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	case OID_802_3_CURRENT_ADDRESS:
		{
			// This OID is not send by the NDIS library but is initiated by the Dta1xxNdis
			// driver directly. The current address is read from the registry during start
			// of the Dta1xxNdis driver and sent to the Dta1xx.
			UInt8*  pAddress = &(pInfo->m_Data[0]);
			pNwStore->m_MacAddrCur[0] = pAddress[0];
			pNwStore->m_MacAddrCur[1] = pAddress[1];
			pNwStore->m_MacAddrCur[2] = pAddress[2];
			pNwStore->m_MacAddrCur[3] = pAddress[3];
			pNwStore->m_MacAddrCur[4] = pAddress[4];
			pNwStore->m_MacAddrCur[5] = pAddress[5];

			// Update the MAC address in the MAC
			Dta1xxEthMacAddressSet(pNwStore->m_pRegBase, pNwStore->m_MacAddrCur);

			// Send setup frame
			if (!InsertQueueDpc(&pNwStore->m_SetupFrameDpc)) {
				// The SetupFrame is already in the DPC queue
#if LOG_LEVEL_NDIS > 0
				DTA1XX_LOG( KERN_INFO, "Skip sending setup frame. Already in DPC queue" );
#endif
				pNwStore->m_SetupFramePending = TRUE;
			}
		}
		break;

	default:
#if LOG_LEVEL_NDIS > 0
		DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_NWI_SET_INFO: OID = 0x%x is not implemented",
				    (UInt)pInfo->m_Oid );
#endif
		ReturnStatus = STATUS_NOT_SUPPORTED;
	}

	return ReturnStatus;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwNdisIndicateLinkStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#ifdef WINBUILD
void Dta1xxNwIndicateLinkStatusDpc(
    IN PKDPC  Dpc,					// Supplies the DPC object
	IN PVOID  DeferredContext,		// Pointer to NwStore
									// Passed when KeInitializeDpc was called
	IN PVOID  SystemArgument1,		// NotUsed
	IN PVOID  SystemArgument2)		// NotUsed
{
	NDIS_STATUS  LinkStatus = NW_STATUS_MEDIA_DISCONNECT;
	PNwStore  pNwStore = (PNwStore)DeferredContext;

	if (pNwStore->m_PhyLink == DTA1XX_ETH_NW_LINK_UP)
		LinkStatus = NW_STATUS_MEDIA_CONNECT;

	Dta1xxNwIndicateStatusIrp(pNwStore, LinkStatus, NULL, 0, 0);

	pNwStore->m_LinkStatDPCBusy = FALSE;
}
#else
void Dta1xxNwIndicateLinkStatusDpc(
    IN unsigned long  DeferredContext)		// Pointer to NwStore
									// Passed when KeInitializeDpc was called
{
	NDIS_STATUS  LinkStatus = NW_STATUS_MEDIA_DISCONNECT;
	PNwStore  pNwStore = (PNwStore)DeferredContext;
	
	if (pNwStore->m_PhyLink == DTA1XX_ETH_NW_LINK_UP)
		LinkStatus = NW_STATUS_MEDIA_CONNECT;

	// Trigger network driver if still enabled
	if (pNwStore->m_EnableNwiRx && pNwStore->m_pStatus) {
		pNwStore->m_pStatus->m_GeneralStatus = LinkStatus;
		
		set_bit(0, (void*)&pNwStore->m_pStatus->m_LinkStatus.m_Avail);
		KeSetEvent(pNwStore->m_pStatus->m_LinkStatus.m_StatusAvail, 0, FALSE);
	}
	pNwStore->m_LinkStatDPCBusy = FALSE;
}
#endif

#ifdef WINBUILD
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIndicateStatusIrp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt  Dta1xxNwIndicateStatusIrp(
	IN  PNwStore  pNwStore,				// Network port attributes
	IN NDIS_STATUS  GeneralStatus,
	IN PVOID  StatusBuffer,
	IN UInt  StatusBufferSize,
	IN UInt  Cancel)
{
	UInt  StructSize;
	UInt  ReturnStat = 1;
    PIRP  pIrp = NULL;
    ULONG  Bytes = 0;
    ULONG  InBufLength, OutBufLength;
	KIRQL  OldIrql;
    NTSTATUS  NtStatus;
    PIO_STACK_LOCATION  pStack = NULL;    
    PDTA1XX_NDIS_STATUS  pIndicateStatus = NULL;
	PDTA1XX_FDO  pFdoExt = pNwStore->m_pFdoExt;

#if LOG_LEVEL_NDIS > 2
	DTA1XX_LOG(KERN_INFO, "Dta1xxNwIndicateStatusIrp");
#endif
    
	KeAcquireSpinLock(&pNwStore->m_StatusSpinLock, &OldIrql);

    pIrp = pNwStore->m_StatusIrp;
    
	if (pIrp != NULL) {
		pStack = IoGetCurrentIrpStackLocation(pIrp);
		pIndicateStatus = pIrp->AssociatedIrp.SystemBuffer;
		InBufLength = pStack->Parameters.DeviceIoControl.InputBufferLength;
		OutBufLength = pStack->Parameters.DeviceIoControl.OutputBufferLength;            
		StructSize =  sizeof(DTA1XX_NDIS_STATUS)
					- sizeof(pIndicateStatus->m_StatusBuffer);
		
		// Clear the cancel routine.
		if (IoSetCancelRoutine(pIrp, NULL) != NULL) {
			
			NtStatus = STATUS_CANCELLED;
			if (!Cancel) {

				// Check whether the buffer is large enough
				if (OutBufLength >= sizeof(StructSize) &&
				   (OutBufLength - sizeof(StructSize)) >= StatusBufferSize) {
	                
					pIndicateStatus->m_GeneralStatus = GeneralStatus;
					pIndicateStatus->m_StatusBufferLength = StatusBufferSize;
					RtlCopyBytes(pIndicateStatus->m_StatusBuffer,
								 StatusBuffer, StatusBufferSize);
					NtStatus = STATUS_SUCCESS;
				}
				else {
					NtStatus = STATUS_BUFFER_OVERFLOW;
				}
			}

			// Since we are completing the IRP below, clear this field.
			pNwStore->m_StatusIrp = NULL;                

			// Number of bytes copied or number of bytes required.
			Bytes = StructSize + StatusBufferSize;
		}
		else {
			// Cancel routine is running
			pIrp = NULL;
		}
	}
	else {
		ReturnStat = 0;
	}
    
    KeReleaseSpinLock(&pNwStore->m_StatusSpinLock, OldIrql);

    if (pIrp != NULL) {
		// Release removal lock
		IoReleaseRemoveLock(&pFdoExt->m_RemoveLock, pIrp);
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwIndicateStatusIrp IoCompleteRequest: GenStatus: %x", GeneralStatus);
        pIrp->IoStatus.Information = Bytes;
        pIrp->IoStatus.Status = NtStatus;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		
	} 
    return ReturnStat;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwNdisCancelStatusIrp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
VOID  Dta1xxNwNdisCancelStatusIrp(
	IN PDEVICE_OBJECT  pFdo,		// Functional Device Object
	IN PIRP  pIrp)					// IRP
{
	KIRQL  OldIrql;
	NwStore*  pNwStore;
	PIO_STACK_LOCATION  pStack = NULL;
	PDTA1XX_FDO  pFdoExt = (PDTA1XX_FDO) pFdo->DeviceExtension;
	PDTA1XX_PDO_EXTENSION  pPdoExt = (PDTA1XX_PDO_EXTENSION) pFdo->DeviceExtension;
	
	DTA1XX_LOG(KERN_INFO, "Dta1xxNwNdisCancelStatusIrp");

	if ((pFdoExt->m_Flags & FLAG_PDO) != 0)
		pFdoExt = pPdoExt->m_pFdo->DeviceExtension;

	IoReleaseCancelSpinLock(pIrp->CancelIrql);

	pStack = IoGetCurrentIrpStackLocation(pIrp);
	pNwStore = pIrp->Tail.Overlay.DriverContext[0]; 
	KeAcquireSpinLock(&pNwStore->m_StatusSpinLock, &OldIrql);
			
	pNwStore->m_StatusIrp = NULL;

	KeReleaseSpinLock(&pNwStore->m_StatusSpinLock, OldIrql);

	// Release removal lock
	IoReleaseRemoveLock(&pFdoExt->m_RemoveLock, pIrp);

	pIrp->IoStatus.Information = 0;
	pIrp->IoStatus.Status = STATUS_CANCELLED;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwNdisQueueStatusIrp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS  Dta1xxNwNdisQueueStatusIrp(
	IN PNwStore  pNwStore,			// Network port attributes
	IN PIRP  pIrp)					// IRP
{
	KIRQL  OldIrql;
	NTSTATUS  Status;

	KeAcquireSpinLock(&pNwStore->m_StatusSpinLock, &OldIrql);

	if (pNwStore->m_StatusIrp != NULL) {
		Status = STATUS_DEVICE_BUSY;
	}
	else {
		IoSetCancelRoutine(pIrp, Dta1xxNwNdisCancelStatusIrp);
		if (pIrp->Cancel && IoSetCancelRoutine(pIrp, NULL)) {
			Status = STATUS_CANCELLED;
		}
		else {
			IoMarkIrpPending(pIrp);
			pNwStore->m_StatusIrp = pIrp;
			pIrp->Tail.Overlay.DriverContext[0] = pNwStore; 
			Status = STATUS_PENDING;
		}
	}
	KeReleaseSpinLock(&pNwStore->m_StatusSpinLock, OldIrql);

	// Skip this. If this is send and the link is down, Ndis does not send the 
	// "Set OID_GEN_CURRENT_PACKET_FILTER" IRP.
	// The NdisMIndicateStatus function may not be executed in the MPInitialize function
	// in the Ndis driver.

	// Send 1st link status to Ndis driver
	if ((Status==STATUS_PENDING) && (pNwStore->m_NwiLink == 2)) {
		if (pNwStore->m_PhyLink == 2) {
			pNwStore->m_NwiLink = DTA1XX_ETH_NW_LINK_DOWN;
		} else {
			pNwStore->m_NwiLink = pNwStore->m_PhyLink;
		}
		DTA1XX_LOG(KERN_INFO, "Dta1xxNwNdisQueueStatusIrp FIRST TIME");
		pNwStore->m_LinkPending = TRUE;
		//Dta1xxNwNdisIndicateLinkStatus(pNwStore);
	} //else 

	return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwNdisHandleQueryRelationsFdo -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Handles Query Relations as a Fdo
//
NTSTATUS  Dta1xxNwNdisHandleQueryRelationsFdo(
	PDEVICE_OBJECT  pFdo,			// Functional Device Object
	PIRP  pIrp)						// IRP
{
	Int  i;
	ULONG  Size;
	NTSTATUS  Status = STATUS_SUCCESS;
	PDEVICE_RELATIONS  pOldRelations = (PDEVICE_RELATIONS) pIrp->IoStatus.Information;
	PDEVICE_RELATIONS  pRelations;
	PDTA1XX_FDO  pFdoExt = (PDTA1XX_FDO) pFdo->DeviceExtension;

	ASSERT(pFdoExt->m_NumIpPorts>=1);

	if (pOldRelations != NULL) {
		// There are already device objects from the upper driver(s)
		// Add our own device objects to the list 
		Size = sizeof(DEVICE_RELATIONS) + 
			   (pOldRelations->Count) * sizeof(PDEVICE_OBJECT) +
			   (pFdoExt->m_NumIpPorts - 1) * sizeof(PDEVICE_OBJECT);
							// -1 because the DEVICE_RELATIONS already contains 1 pointer

		pRelations = (PDEVICE_RELATIONS) ExAllocatePool(PagedPool, Size);

		if (pRelations != NULL) {
			RtlCopyMemory(pRelations, pOldRelations,
						  Size - (pFdoExt->m_NumIpPorts * sizeof(PDEVICE_OBJECT)));
			ExFreePool(pOldRelations);
		}
	} else {
		// No device objects from upper drivers inserted. Create an empty one
		Size = sizeof(DEVICE_RELATIONS) + 
			   (pFdoExt->m_NumIpPorts - 1) * sizeof(PDEVICE_OBJECT);
							// -1 because the DEVICE_RELATIONS already contains 1 pointer
		pRelations = (PDEVICE_RELATIONS) ExAllocatePool(PagedPool, Size);
		
		if (pRelations != NULL) {
			pRelations->Count = 0;
		}
	}

	if (pRelations == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	for (i=0; i<pFdoExt->m_NumIpPorts; i++) {
		pRelations->Objects[pRelations->Count + i] = pFdoExt->m_NwStore[i].m_NdisDO;
		ObReferenceObject(pFdoExt->m_NwStore[i].m_NdisDO);
	}
	pRelations->Count+= pFdoExt->m_NumIpPorts;
	
	pIrp->IoStatus.Information = (ULONG_PTR) pRelations;
	return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwNdisHandleQueryRelationsPdo -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Handles Query Relations as a Pdo
//
NTSTATUS Dta1xxNwNdisHandleQueryRelationsPdo(
	PDEVICE_OBJECT  pPdo,			// Physical Device Object
	PIRP  pIrp)						// IRP
{
	NTSTATUS Status = STATUS_SUCCESS;
	PDEVICE_RELATIONS  pRelations;

	pRelations = (PDEVICE_RELATIONS) ExAllocatePool(PagedPool, sizeof(DEVICE_RELATIONS));
	
	if (pRelations == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	pRelations->Count = 1;
	pRelations->Objects[0] = pPdo;
	ObReferenceObject(pPdo);

	pIrp->IoStatus.Information = (ULONG_PTR) pRelations;
	return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwNdisCreateNdisDeviceObjects -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Executed during AddDevice
//
NTSTATUS Dta1xxNwNdisCreateNdisDeviceObjects(
	PDEVICE_OBJECT  pFdo)			// Functional Device Object
{
	Int  i;
	PDTA1XX_FDO  pFdoExt = (PDTA1XX_FDO) pFdo->DeviceExtension;
	PDTA1XX_PDO_EXTENSION  pPdoExt;	

	for (i=0; i<pFdoExt->m_NumIpPorts; i++) {
		IoCreateDevice(pFdo->DriverObject, sizeof(DTA1XX_PDO_EXTENSION), NULL,
					   FILE_DEVICE_NETWORK,
					   FILE_AUTOGENERATED_DEVICE_NAME | FILE_DEVICE_SECURE_OPEN,
					   FALSE, &(pFdoExt->m_NwStore[i].m_NdisDO));
		
		pPdoExt = (PDTA1XX_PDO_EXTENSION)pFdoExt->m_NwStore[i].m_NdisDO->DeviceExtension;

		pPdoExt->m_Flags = FLAG_PDO;
		pPdoExt->m_pPdo  = pFdoExt->m_NwStore[i].m_NdisDO;
		pPdoExt->m_pFdo	 = pFdo;
		pPdoExt->m_Index = i;
		pPdoExt->m_pPdo->Flags &= ~DO_DEVICE_INITIALIZING;
		// Indicate that read/write requests should proceed using direct I/O.
		pPdoExt->m_pPdo->Flags |= DO_DIRECT_IO;
	}

	return STATUS_SUCCESS;
}


//-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwNdisRemoveNdisDeviceObjects -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Executed during RemoveDevice
//
NTSTATUS Dta1xxNwNdisRemoveNdisDeviceObjects(
	PDEVICE_OBJECT  pFdo)			// Functional Device Object
{
	Int  i;
	PDTA1XX_FDO  pFdoExt = (PDTA1XX_FDO) pFdo->DeviceExtension;

	for (i=0; i<pFdoExt->m_NumIpPorts; i++) {
		if (pFdoExt->m_NwStore[i].m_NdisDO != NULL)
			IoDeleteDevice(pFdoExt->m_NwStore[i].m_NdisDO);
		pFdoExt->m_NwStore[i].m_NdisDO = NULL;
	}

	return STATUS_SUCCESS;
}
#endif

