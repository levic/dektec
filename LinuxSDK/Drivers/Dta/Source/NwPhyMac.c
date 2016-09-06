//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NwPhyMac.c *#*#*#*#*#*#*#*#* (C) 2011-2016 DekTec
//
// Dta driver - Network Phy and Mac functionality - 
// Implementation of Altera and Cast Mac and National/Marvel phy
//
// Supported PHY's:
// National PHY:  DP83865:  DTA-160
// Marvel PHY:    88E1111:  DTA-160
//                88E1119R: DTA-2160
//                88E1121R: DTA-2162

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2016 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL DEKTEC DIGITAL VIDEO BV, ITS AGENTS OR ITS EMPLOYEES BE LIABLE FOR
// ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL, OR OTHER DAMAGES (INCLUDING DAMAGES
// FOR THE LOSS OF USE, INFORMATION, GOODWILL, PROFIT, WORK STOPPAGE, DATA, BUSINESS OR
// REVENUE) UNDER ANY CIRCUMSTANCES, OR UNDER ANY LEGAL THEORY, WHETHER IN CONTRACT, IN
// TORT, IN NEGLIGENCE, OR OTHERWISE, ARISING FROM THE USE OF, OR INABILITY TO USE THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtaIncludes.h>

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// Ethernet CRC-32 lookup table for fast CRC calculation
// Polinomial: 0x04C11DB7
//
static const UInt32  g_CrcTable[256] =
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

#define  HAS_MACBUG(pPhyMac)  (pPhyMac->m_pDvcData->m_DevInfo.m_TypeNumber==160      \
                                  && pPhyMac->m_pDvcData->m_DevInfo.m_FirmwareVersion<1)


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaMacAUpdateSpeedDuplex(PhyMac* pPhyMac);
DtStatus  DtaMacCUpdateSpeedDuplex(PhyMac* pPhyMac);
DtStatus  DtaMacAUpdatePacketFiltering(PhyMac* pPhyMac);
DtStatus  DtaMacCUpdatePacketFiltering(PhyMac* pPhyMac);
UInt32  DtaMacCGetSingleCounter(PhyMac* pPhyMac, UInt Register);
void  DtaMacCLockCounters(PhyMac* pPhyMac);
void  DtaMacCUnLockCounters(PhyMac* pPhyMac);
UInt32  DtaMacCGetCounter(PhyMac* pPhyMac, UInt Register);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ MAC General +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCalcEthernetCrc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Calculates a 32-bit Ethernet CRC over a provided data buffer
// This function is used for the setup frame for the Cast MAC, but can be used for other
// Ethernet CRC calculations
//
UInt32  DtaMacCalcEthernetCrc(UInt8* pBuffer, UInt32 BufLen)
{
    UInt32  Crc = 0xFFFFFFFF;
    UInt32  Index;

    for (Index=0; Index<BufLen; Index++)
        Crc = (Crc >> 8) ^ g_CrcTable[(UInt8)((Crc & 0xFF) ^ pBuffer[Index])];

    return Crc;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacInitMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaMacInitMacAddress(PhyMac* pPhyMac)
{
    DtStatus  Status = DT_STATUS_OK;
    char  Buffer[10];
    UInt  BufLen = sizeof(Buffer);
    UInt  MacSeq, MacSeq1, MacSeq2, MacSeq3, MacSeq4;
    UInt  MacAddrLow, MacAddrHigh;
    Int  i;
    
    Status = DtaVpdReadItemRo(pPhyMac->m_pDvcData, 2, "MC", Buffer, &BufLen);
    if (DT_SUCCESS(Status))
    {
        // Convert MC-string to 24-bit MAC sequence number
        MacSeq1 = Buffer[0] - 0x20;
        MacSeq2 = Buffer[1] - 0x20;
        MacSeq3 = Buffer[2] - 0x20;
        MacSeq4 = Buffer[3] - 0x20;
        MacSeq = (MacSeq1<<18) | (MacSeq2<<12) | (MacSeq3<<6) | MacSeq4;

        // The first releases of the DTA-2160 only had the first 14-bits stored in
        // the 'MC' VPD item and not the complete 24-bits. Correct it here.
        if (pPhyMac->m_pDvcData->m_DevInfo.m_TypeNumber == 2160)
        {
            if ((MacSeq & (0x3ff << 14)) == 0)
                MacSeq += 0x010 << 14;
        }
    } else {
        // No "MC" resource found: use 0 as MAC sequence number and 0x3FF as range
        // This is not an error, but the initial situation when a DTA-xxx is
        // powered up for the first time after manufacturing. In this case, the
        // board should start up with MAC address 00-14-F4-00-00-00
        MacSeq = 0;
        MacSeq += 0x3FF << 14;    // Reserved MAC range
        DtDbgOut(ERR, PHYMAC, "IpPort %i: Failed to read MAC address from VPD (0x%x)",
                                                          pPhyMac->m_IpPortIndex, Status);
    }

    // If 2 phy's are on the board, the LSB bit of the MAC-address will be set to Port 0/1
    if (pPhyMac->m_pDvcData->m_NumIpPorts == 2)
        MacSeq += pPhyMac->m_IpPortIndex; 
    else if (pPhyMac->m_pDvcData->m_NumIpPorts > 2)
    {
        DT_ASSERT(FALSE);
        return DT_STATUS_CONFIG_ERROR;
    }

    // Convert to 48-bit MAC address by prepending DekTec OUI (00-14-F4)
    // MSB/LSB is swapped!
    MacAddrHigh = 0xF4000000 | MacSeq;
    MacAddrLow = 0x0014;

    // Store MAC address
    pPhyMac->m_MacAddrPer[5] = (MacAddrHigh & 0xff);
    pPhyMac->m_MacAddrPer[4] = (MacAddrHigh >>  8) & 0xff;
    pPhyMac->m_MacAddrPer[3] = (MacAddrHigh >> 16) & 0xff;
    pPhyMac->m_MacAddrPer[2] = (MacAddrHigh >> 24) & 0xff;
    pPhyMac->m_MacAddrPer[1] = (MacAddrLow & 0xff);
    pPhyMac->m_MacAddrPer[0] = (MacAddrLow >> 8) & 0xff;
    
    for (i=0; i<6; i++)
        pPhyMac->m_MacAddrCur[i] = pPhyMac->m_MacAddrPer[i];
    
    DtDbgOut(AVG, PHYMAC, "IpPort %i: MAC address set to: "
                                  "%02x-%02x-%02x-%02x-%02x-%02x", pPhyMac->m_IpPortIndex,
                                  pPhyMac->m_MacAddrCur[0], pPhyMac->m_MacAddrCur[1],
                                  pPhyMac->m_MacAddrCur[2], pPhyMac->m_MacAddrCur[3],
                                  pPhyMac->m_MacAddrCur[4], pPhyMac->m_MacAddrCur[5]);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacUpdateSpeedDuplex -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaMacUpdateSpeedDuplex(PhyMac* pPhyMac)
{
    DtStatus  Status = DT_STATUS_OK;

    // Only update MAC status if link is up. If link is down, the speed and duplex
    // are meaningless. The cast MAC can not be disabled when link is down.
    if (pPhyMac->m_PhyLink != DTA_PHY_LINK_UP)
        return DT_STATUS_OK;

    // Is Speed and/or Duplex changed
    if (pPhyMac->m_MacSpeed==pPhyMac->m_PhySpeed)
        return DT_STATUS_OK;

    if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
        Status = DtaMacAUpdateSpeedDuplex(pPhyMac);
    else
        Status = DtaMacCUpdateSpeedDuplex(pPhyMac);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacUpdatePacketFiltering -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaMacUpdatePacketFiltering(PhyMac* pPhyMac)
{
    DtStatus  Status = DT_STATUS_OK;

    // Is packet filtering changed?
    if (pPhyMac->m_MacPacketFilter == pPhyMac->m_NwDrvPacketFilter) 
        return DT_STATUS_OK;


    if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
        Status = DtaMacAUpdatePacketFiltering(pPhyMac);
    else
        Status = DtaMacCUpdatePacketFiltering(pPhyMac);

    pPhyMac->m_MacPacketFilter = pPhyMac->m_NwDrvPacketFilter;

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacUpdateMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaMacUpdateMacAddress(PhyMac* pPhyMac)
{
    DtaIpPort*  pIpPort = 
                      &pPhyMac->m_pDvcData->m_IpDevice.m_pIpPorts[pPhyMac->m_IpPortIndex];
    
    if (pIpPort->m_PortType == DTA_IPPORT_TYPE2)
    {
        // Update address matcher
        DtaAddrMatchMacAddressSet(pIpPort->m_IpPortType2.m_pAddrMatcherRegs, 
                                                                   pPhyMac->m_MacAddrCur);
        DtaIpUpdateMacAddressFilter(pIpPort);
    }

    if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
        DtaMacAMacAddressSet(pPhyMac->m_pMacRegs, pPhyMac->m_MacAddrCur);
    else {
        DtaIpNrtSharedBuf*  pTxSharedBuf = &pIpPort->m_NrtChannels.m_TxSharedBuf;
        DtaIpNwSharedBufInfo*  pSharedInfo = pTxSharedBuf->m_pSharedInfo;
        pIpPort->m_PhyMac.m_SetupframePending = TRUE;
        DtAtomicIncrement(&pSharedInfo->m_DataAvailableCnt);
        DtEventSet(&pSharedInfo->m_DataAvailableEvent);
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacGetMacAddressCurrent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaMacGetMacAddressCurrent(PhyMac* pPhyMac, UInt8* pAddress)
{
    DtMemCopy(pAddress, pPhyMac->m_MacAddrCur, sizeof(pPhyMac->m_MacAddrCur));
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacSetMacAddressCurrent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaMacSetMacAddressCurrent(PhyMac* pPhyMac, UInt8* pAddress)
{
    DtMemCopy(pPhyMac->m_MacAddrCur, pAddress, sizeof(pPhyMac->m_MacAddrCur));
    return DtaMacUpdateMacAddress(pPhyMac);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacGetMacAddressPermanent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaMacGetMacAddressPermanent(PhyMac* pPhyMac, UInt8* pAddress)
{
    DtMemCopy(pAddress, pPhyMac->m_MacAddrPer, sizeof(pPhyMac->m_MacAddrPer));
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacGetCounter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaMacGetCounter(PhyMac* pPhyMac, UInt CounterId, UInt64* pValue)
{
    DtStatus  Status = DT_STATUS_OK;
    volatile UInt8*  pMacBase = pPhyMac->m_pMacRegs;
    
    *pValue = 0;
    
    switch (CounterId)
    {
    case DTA_MAC_CNT_GEN_BYTES_XMIT:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
            *pValue = DtaMacAGetCounter(pMacBase, DTA_MACA_REG_TXOCTOK);
        else
            *pValue = DtaMacCGetSingleCounter(pPhyMac, DTA_MACC_SCM_TXOCTOK);
        break;
    case DTA_MAC_CNT_GEN_BYTES_RCV:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
            *pValue = DtaMacAGetCounter(pMacBase, DTA_MACA_REG_RXOCTOK);
        else
            *pValue = DtaMacCGetSingleCounter(pPhyMac, DTA_MACC_SCM_RXOCTOK);
        break;
    case DTA_MAC_CNT_GEN_XMIT_OK:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
            *pValue = DtaMacAGetCounter(pMacBase, DTA_MACA_REG_TXFRMOK);
        else
            *pValue = DtaMacCGetSingleCounter(pPhyMac, DTA_MACC_SCM_TXFRMOK);
        break;
    case DTA_MAC_CNT_GEN_RCV_OK:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
            *pValue = DtaMacAGetCounter(pMacBase, DTA_MACA_REG_RXFRMOK);
        else
            *pValue = DtaMacCGetSingleCounter(pPhyMac, DTA_MACC_SCM_RXFRMOK);
        break;
    case DTA_MAC_CNT_GEN_XMIT_ERROR:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
            *pValue = DtaMacAGetCounter(pMacBase, DTA_MACA_REG_TXERRORS);
        else {
            DtaMacCLockCounters(pPhyMac);
            *pValue = DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL16);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_TXLCERR);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_TXMACERR);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_TXCSERR);
            DtaMacCUnLockCounters(pPhyMac);
        }
        break;
    case DTA_MAC_CNT_GEN_RCV_ERROR:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
            *pValue = DtaMacAGetCounter(pMacBase, DTA_MACA_REG_RXERRORS);
        else {
            DtaMacCLockCounters(pPhyMac);
            *pValue = DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_RXALIGNERR);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_RXFCSERR);
            DtaMacCUnLockCounters(pPhyMac);
        }
        break;
    case DTA_MAC_CNT_GEN_RCV_NO_BUFFER:
        *pValue = DtaIpGetNumRxFifoOverflow(
                     &pPhyMac->m_pDvcData->m_IpDevice.m_pIpPorts[pPhyMac->m_IpPortIndex]);
        break;
    case DTA_MAC_CNT_GEN_RCV_HDR_ERROR:
    {
        DtaIpPort*  pMyIpPort;
        pMyIpPort = pPhyMac->m_pDvcData->m_IpDevice.m_pIpPorts + (pPhyMac->m_IpPortIndex);
        *pValue = pMyIpPort->m_NumIncorrectDmaRxHeaders;
        break;
    }
    case DTA_MAC_CNT_GEN_RCV_CRC_ERROR:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA) 
        {
            *pValue = DtaMacAGetCounter(pMacBase, DTA_MACA_REG_RXFCSERR);
            // Include all packets discarded by FPGA TODO
            //*pValue += DtaNwRxInvCntGet(pFdoExt->m_IpMqRx[Index]->m_pRegBase);
        } else {
            *pValue = DtaMacCGetSingleCounter(pPhyMac, DTA_MACC_SCM_RXFCSERR);
            // Include all packets discarded by FPGA TODO
            //*pValue += DtaNwRxInvCntGet(pFdoExt->m_IpMqRx[Index]->m_pRegBase);
        }
        break;
    case DTA_MAC_CNT_802_3_RCV_ERROR_ALIGNMENT:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA) 
            *pValue = DtaMacAGetCounter(pMacBase, DTA_MACA_REG_RXALIGNERR);
        else
            *pValue = DtaMacCGetSingleCounter(pPhyMac, DTA_MACC_SCM_RXALIGNERR);
        break;
    case DTA_MAC_CNT_802_3_XMIT_ONE_COLLISION:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
            Status = DT_STATUS_NOT_FOUND;
        else 
            *pValue = DtaMacCGetSingleCounter(pPhyMac, DTA_MACC_SCM_COLL1);
        break;
    case DTA_MAC_CNT_802_3_XMIT_MORE_COLLISIONS:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
            Status = DT_STATUS_NOT_FOUND;
        else {
            DtaMacCLockCounters(pPhyMac);
            *pValue = DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL2);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL3);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL4);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL5);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL6);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL7);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL8);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL9);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL10);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL11);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL12);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL13);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL14);
            *pValue += DtaMacCGetCounter(pPhyMac, DTA_MACC_SCM_COLL15);
            DtaMacCUnLockCounters(pPhyMac);
        }
        break;
    case DTA_MAC_CNT_802_3_XMIT_DEFERRED:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
            Status = DT_STATUS_NOT_FOUND;
        else
            *pValue = DtaMacCGetSingleCounter(pPhyMac, DTA_MACC_SCM_TXDEF);
        break;
    case DTA_MAC_CNT_802_3_XMIT_MAX_COLLISIONS:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
            Status = DT_STATUS_NOT_FOUND;
        else
            *pValue = DtaMacCGetSingleCounter(pPhyMac, DTA_MACC_SCM_COLL16);
        break;
    case DTA_MAC_CNT_802_3_XMIT_UNDERRUN:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
            Status = DT_STATUS_NOT_FOUND;
        else
            *pValue = DtaMacCGetSingleCounter(pPhyMac, DTA_MACC_SCM_TXMACERR);
        break;
    case DTA_MAC_CNT_802_3_XMIT_TIMES_CRS_LOST:
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
            Status = DT_STATUS_NOT_FOUND;
        else
            *pValue = DtaMacCGetSingleCounter(pPhyMac, DTA_MACC_SCM_TXCSERR);
        break;
    case DTA_MAC_CNT_GEN_TRANSMIT_QUEUE_LENGTH:
    case DTA_MAC_CNT_802_3_RCV_OVERRUN:
    case DTA_MAC_CNT_802_3_XMIT_HEARTBEAT_FAILURE:
    case DTA_MAC_CNT_802_3_XMIT_LATE_COLLISIONS:
    default: Status = DT_STATUS_NOT_FOUND;
    }
    if (Status == DT_STATUS_NOT_FOUND)
        DtDbgOut(ERR, PHYMAC, "IpPort %i: Counter %i NOT SUPPORTED", 
                                                       pPhyMac->m_IpPortIndex, CounterId);
    else
        DtDbgOut(MAX, PHYMAC, "IpPort %i: Counter %i Value: %08x%08xh", 
                                       pPhyMac->m_IpPortIndex, CounterId,
                                       (UInt)(*pValue>>32), (UInt)((*pValue)&0xffffffff));
    
    return Status;
}



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ MAC Cast +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCBuildSetupFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaMacCBuildSetupFrame(PhyMac* pPhyMac, UInt8* pDst, UInt*  pSize)
{
    DtaMacCSetupFrame*  pSetupFrame = (DtaMacCSetupFrame*)pDst;
    DtaDmaTxHeader*  pDmaHeader = &pSetupFrame->m_DmaHeader;
    Int  i;
    UInt8  BCMacAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UInt16  HTIndex;

    DtMemZero(pDst, sizeof(DtaMacCSetupFrame));
    *pSize = sizeof(DtaMacCSetupFrame);

    // Fill in DMA Tx header
    pDmaHeader->m_Tag = 0x445441A0;
    pDmaHeader->m_Length = sizeof(DtaDmaTxHeader);
    pDmaHeader->m_Version = 1;
    pDmaHeader->m_TxHeaderGen.m_TransmitControl.m_PacketLength = 
                                       sizeof(DtaMacCSetupFrame) - sizeof(DtaDmaTxHeader);
    // It's a setup frame
    pDmaHeader->m_TxHeaderGen.m_TransmitControl.m_SetupFrame = 1;

    // Only 16 items can be stored with perfect filtering
    // 1 for own mac-address
    // 1 for broadcast address
    // 14 for multicast addresses
    if (pPhyMac->m_NumMulticasts <= 14)
    {
        // Perfect filtering mode: Setup frame buffer is interpreted
        // as a set of 16 48-bit physical addresses
        pDmaHeader->m_TxHeaderGen.m_TransmitControl.m_FilteringType1 =
                                                                (DTA_FT_PERFECT >> 1) & 1;
        pDmaHeader->m_TxHeaderGen.m_TransmitControl.m_FilteringType0 = DTA_FT_PERFECT & 1;

        // Own mac address
        pSetupFrame->m_PFAddress[0].m_PhAddr0 = 
                       ((UInt16)pPhyMac->m_MacAddrCur[1] << 8) + pPhyMac->m_MacAddrCur[0];
        pSetupFrame->m_PFAddress[0].m_PhAddr1 = 
                       ((UInt16)pPhyMac->m_MacAddrCur[3] << 8) + pPhyMac->m_MacAddrCur[2];
        pSetupFrame->m_PFAddress[0].m_PhAddr2 = 
                       ((UInt16)pPhyMac->m_MacAddrCur[5] << 8) + pPhyMac->m_MacAddrCur[4];
        
        // Broadcast address
        pSetupFrame->m_PFAddress[1].m_PhAddr0 = 0xffff;
        pSetupFrame->m_PFAddress[1].m_PhAddr1 = 0xffff;
        pSetupFrame->m_PFAddress[1].m_PhAddr2 = 0xffff;

        // Multicast addresses
        for (i=0; i<(Int)pPhyMac->m_NumMulticasts; i++) 
        {
            pSetupFrame->m_PFAddress[i+2].m_PhAddr0 = 
                                           ((UInt16)pPhyMac->m_MulticastList[i][1] << 8) +
                                           pPhyMac->m_MulticastList[i][0];
            pSetupFrame->m_PFAddress[i+2].m_PhAddr1 = 
                                           ((UInt16)pPhyMac->m_MulticastList[i][3] << 8) +
                                           pPhyMac->m_MulticastList[i][2];
            pSetupFrame->m_PFAddress[i+2].m_PhAddr2 = 
                                           ((UInt16)pPhyMac->m_MulticastList[i][5] << 8) +
                                           pPhyMac->m_MulticastList[i][4];

            DtDbgOut(AVG, PHYMAC, "ML[%02d]=%02x-%02x-%02x-%02x-%02x-%02x", i,
                          pPhyMac->m_MulticastList[i][0], pPhyMac->m_MulticastList[i][1],
                          pPhyMac->m_MulticastList[i][2], pPhyMac->m_MulticastList[i][3],
                          pPhyMac->m_MulticastList[i][4], pPhyMac->m_MulticastList[i][5]);
        }
    } else {
        // Hash table implementation
        pDmaHeader->m_TxHeaderGen.m_TransmitControl.m_FilteringType1 = 
                                                                   (DTA_FT_HASH >> 1) & 1;
        pDmaHeader->m_TxHeaderGen.m_TransmitControl.m_FilteringType0 = DTA_FT_HASH & 1;

        // Own mac address
        pSetupFrame->m_HTable.m_PFAddress.m_PhAddr0 = 
                       ((UInt16)pPhyMac->m_MacAddrCur[1] << 8) + pPhyMac->m_MacAddrCur[0];
        pSetupFrame->m_HTable.m_PFAddress.m_PhAddr1 = 
                       ((UInt16)pPhyMac->m_MacAddrCur[3] << 8) + pPhyMac->m_MacAddrCur[2];
        pSetupFrame->m_HTable.m_PFAddress.m_PhAddr2 = 
                       ((UInt16)pPhyMac->m_MacAddrCur[5] << 8) + pPhyMac->m_MacAddrCur[4];
        
        // Broadcast address, get 9 LSB from CRC over destination MAC
        HTIndex = (UInt16)(DtaMacCalcEthernetCrc(BCMacAddr, sizeof(BCMacAddr)) & 0x1FF);
        pSetupFrame->m_HTable.m_HFItem[HTIndex / 16].m_Item0 |= 1 << (HTIndex % 16);

        // Multicast addresses
        for (i=0; i<(Int)pPhyMac->m_NumMulticasts; i++)
        {
            // Get 9 LSB from CRC over destination MAC
            // pNwStore->m_MCList[i] points to an array of 6 bytes containing the MC MAC
            HTIndex = (UInt16)(DtaMacCalcEthernetCrc(pPhyMac->m_MulticastList[i], 6) & 
                                                                                   0x1FF);
            pSetupFrame->m_HTable.m_HFItem[HTIndex / 16].m_Item0 |= 1 << (HTIndex % 16);

            DtDbgOut(AVG, PHYMAC, "MC[%02d]=%02x-%02x-%02x-%02x-%02x-%02x, hash %02d", 
                           i,
                           pPhyMac->m_MulticastList[i][0], pPhyMac->m_MulticastList[i][1],
                           pPhyMac->m_MulticastList[i][2], pPhyMac->m_MulticastList[i][3],
                           pPhyMac->m_MulticastList[i][4], pPhyMac->m_MulticastList[i][5],
                           HTIndex);
        }
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCMiiSmRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read from the PHY using the MII Serial Management registers. 
// RegisterAddress = Address of PHY register to read. Use the DtaMacCMiiSmReadValue
// function for reading back to register value.
//
void  DtaMacCMiiSmRead(PhyMac* pPhyMac, UInt8 RegisterAddress)
{
    UInt32  MiiCommand;
    MiiCommand = DTA_MACC_MIISM_START_BUSY_MASK | DTA_MACC_MIISM_OPCODE_READ |
                                        DTA_MACC_MIISM_CLKDIV_32 | DTA_MACC_MIISM_PHYADDR;
    MiiCommand |= (RegisterAddress << DTA_MACC_MIISM_REG_ADDRESS_SHIFT)
                                                        & DTA_MACC_MIISM_REG_ADDRESS_MASK;
    DtaMacCMiiSmRegSet(pPhyMac->m_pMacRegs, MiiCommand, HAS_MACBUG(pPhyMac));
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCMiiSmReadValue -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This functions readsback the value returned from reading the phy register.
// It must be executed after the DtaMacCMiiSmRead function.
// If the MII bus is still busy, this function waits until the value is available
// 
UInt32  DtaMacCMiiSmReadValue(PhyMac* pPhyMac)
{
    UInt32  MiiCommand;
    MiiCommand = DtaMacCMiiSmRegGet(pPhyMac->m_pMacRegs, HAS_MACBUG(pPhyMac));

    while ((MiiCommand & DTA_MACC_MIISM_START_BUSY_MASK) != 0)
    {
        DtSleep(1);
        MiiCommand = DtaMacCMiiSmRegGet(pPhyMac->m_pMacRegs, HAS_MACBUG(pPhyMac));
    }
    return MiiCommand & DTA_MACC_MIISM_DATA_MASK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCMiiSmWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write to the PHY using the MII Serial Management registers. 
// RegisterAddress = Address of PHY register to write to
// Data = Data to write
//
void  DtaMacCMiiSmWrite(PhyMac* pPhyMac, UInt8 RegisterAddress, UInt16 Data)
{
    UInt32 MiiCommand;
    MiiCommand = DTA_MACC_MIISM_START_BUSY_MASK | DTA_MACC_MIISM_OPCODE_WRITE |
                                        DTA_MACC_MIISM_CLKDIV_32 | DTA_MACC_MIISM_PHYADDR;
    MiiCommand |= (RegisterAddress << DTA_MACC_MIISM_REG_ADDRESS_SHIFT)
                                                        & DTA_MACC_MIISM_REG_ADDRESS_MASK;
    MiiCommand |= (Data & DTA_MACC_MIISM_DATA_MASK); 
    DtaMacCMiiSmRegSet(pPhyMac->m_pMacRegs, MiiCommand, HAS_MACBUG(pPhyMac));
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCMiiSmIsReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaMacCMiiSmIsReady(PhyMac* pPhyMac)
{
    return (DtaMacCMiiSmRegGet(pPhyMac->m_pMacRegs, HAS_MACBUG(pPhyMac)) & 
                                                     DTA_MACC_MIISM_START_BUSY_MASK) == 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCSetPhyRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaMacCSetPhyRegister(PhyMac* pPhyMac, UInt32 Reg, UInt32 Value)
{
    DT_ASSERT(Reg<0xff && Value<0xffff);

    // Wait until the MII bus is available
    while (DtAtomicCompareExchange(&pPhyMac->m_PhyBusState, DTA_PHY_BUS_STATE_IDLE, 
                                        DTA_PHY_BUS_STATE_BUSY) != DTA_PHY_BUS_STATE_IDLE)
        DtSleep(1);

    // Write register address and value to write to MII bus
    DtaMacCMiiSmWrite(pPhyMac, (UInt8)Reg, (UInt16)Value);

    // Wait until MII is ready
    while (!DtaMacCMiiSmIsReady(pPhyMac))
        DtSleep(1);

    // Release MII bus
    DtAtomicSet(&pPhyMac->m_PhyBusState, DTA_PHY_BUS_STATE_IDLE);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCGetPhyRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt32  DtaMacCGetPhyRegister(PhyMac* pPhyMac, UInt32 Reg)
{
    UInt32  Value;
    DT_ASSERT(Reg < 0xff);

   // Wait until the MII bus is available
    while (DtAtomicCompareExchange(&pPhyMac->m_PhyBusState, DTA_PHY_BUS_STATE_IDLE, 
                                        DTA_PHY_BUS_STATE_BUSY) != DTA_PHY_BUS_STATE_IDLE)
        DtSleep(1);

    // Write register address to read to MII bus
    DtaMacCMiiSmRead(pPhyMac, (UInt8)Reg);

    // Read back register value. This functions checks if the MII bus is ready and then
    // reads the data.
    Value = DtaMacCMiiSmReadValue(pPhyMac);

    // Release MII bus
    DtAtomicSet(&pPhyMac->m_PhyBusState, DTA_PHY_BUS_STATE_IDLE);
    return Value;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCLockCounters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaMacCLockCounters(PhyMac* pPhyMac)
{
    DtFastMutexAcquire(&pPhyMac->m_FastMutexCounters);
    DtaMacCScModeLockCounters(pPhyMac->m_pMacRegs);
    if (HAS_MACBUG(pPhyMac))
    {
        while (!DtaMacCScModeAreCountersLocked(pPhyMac->m_pMacRegs))
        {
            // Workaround for R/W MAC problem in FPGA
            // Fixed in DTA-160 fw v1
            // Write lock again due to problem in FPGA
            DtaMacCScModeLockCounters(pPhyMac->m_pMacRegs);
            DtSleep(1);
        }
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCUnLockCounters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaMacCUnLockCounters(PhyMac* pPhyMac)
{
    DtaMacCScModeUnLockCounters(pPhyMac->m_pMacRegs);
    if (HAS_MACBUG(pPhyMac))
    {
        while (!DtaMacCScModeAreCountersUnLocked(pPhyMac->m_pMacRegs))
        {
            // Workaround for R/W MAC problem
            // Write unlock again due to problem in FPGA
            // Fixed in DTA-160 fw v1
            DtaMacCScModeUnLockCounters(pPhyMac->m_pMacRegs);
            DtSleep(1);
        }
    }
    DtFastMutexRelease(&pPhyMac->m_FastMutexCounters);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCGetCounter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Pre: Counters must be locked with function DtaMacCLockCounters
//
UInt32  DtaMacCGetCounter(PhyMac* pPhyMac, UInt Register)
{   
    // Workaround for R/W MAC problem in FPGA
    // Due to a problem in the FPGA with simultaneously read/write from/to the MAC by
    // driver and FPGA the following code is needed.
    // The FPGA is returning 0 if the read is not successfull.
    if (HAS_MACBUG(pPhyMac))
    {
        UInt32  Value1 = 0;     // Initial value: 0
        UInt32  Value2 = 1;     // Initial value: 1
        while (Value1!=Value2 && (Value1==0 || Value2==0))
        {
            Value1 = DtaMacCScModeGetCounterValue(pPhyMac->m_pMacRegs, Register,
                                                                     HAS_MACBUG(pPhyMac));
            Value2 = DtaMacCScModeGetCounterValue(pPhyMac->m_pMacRegs, Register,
                                                                     HAS_MACBUG(pPhyMac));
        }
        return Value2;
        
    }
    return DtaMacCScModeGetCounterValue(pPhyMac->m_pMacRegs, Register,
                                                                     HAS_MACBUG(pPhyMac));
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCGetSingleCounter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt32  DtaMacCGetSingleCounter(PhyMac* pPhyMac, UInt Register)
{
    UInt32  Value1 = 0;         // Initial value: 0
        
    DtaMacCLockCounters(pPhyMac);
    if (HAS_MACBUG(pPhyMac))
    {
        UInt32  Value2 = 1;     // Initial value: 1

        // Workaround for R/W MAC problem
        // Due to a problem in the FPGA with simultaneously read/write from/to the MAC by
        // driver and FPGA the following code is needed.
        // The FPGA is returning 0 if the read is not successfull.
        // Fixed in DTA-160 fw v1
        while (Value1!=Value2 && (Value1==0 || Value2==0))
        {
            Value2 = DtaMacCScModeGetCounterValue(pPhyMac->m_pMacRegs, Register,
                                                                     HAS_MACBUG(pPhyMac));
            Value1 = DtaMacCScModeGetCounterValue(pPhyMac->m_pMacRegs, Register,
                                                                     HAS_MACBUG(pPhyMac));
        }
    } else
        Value1 = DtaMacCScModeGetCounterValue(pPhyMac->m_pMacRegs, Register, 
                                                                     HAS_MACBUG(pPhyMac));
    DtaMacCUnLockCounters(pPhyMac);
    return Value1;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCWaitMacReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaMacCWaitMacReady(PhyMac* pPhyMac)
{
    UInt32  Retry = 0;

    // Wait until reset is complete and MAC is ready
    // Wait max. 1 second
    while ((DtaMacCBusModeGet(pPhyMac->m_pMacRegs) & 0xFF000001)!=0xFE000000 && Retry<100)
    {
        DtSleep(10);
        Retry++;
    }
    if (Retry >= 100)
    {
        DtDbgOut(MAX, PHYMAC, "IpPort %i: ERROR WAITING FOR MAC to be ready!!", 
                                                                pPhyMac->m_IpPortIndex);
        DT_ASSERT(FALSE);
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCResetMac -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaMacCResetMac(PhyMac* pPhyMac)
{
    // Software reset MAC
    DtaMacCBusModeSet(pPhyMac->m_pMacRegs, 0x1);
    
    // Wait until reset is complete
    DtaMacCWaitMacReady(pPhyMac);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Reset Phy + Mac after powerup. Workaround because Mac is not handling this situation
// correctly if the clocks are not available during reset.
//
void  DtaMacCInit(PhyMac* pPhyMac)
{
    // Wait until Mac is powerd up
    DtaMacCWaitMacReady(pPhyMac);
    
    // Enable phy so clocks are available
    // Initialise speed to auto negotiate
    DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_BMCR, DTA_PHY_BMCR_AUTO_NEG_EN_MASK
                                                        | DTA_PHY_BMCR_AUTO_NEG_RST_MASK);

    DtSleep(10);

    // Reset MAC, Phy is autom. disabled by FPGA
    DtaMacCResetMac(pPhyMac);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCUpdateSpeedDuplex -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaMacCUpdateSpeedDuplex(PhyMac* pPhyMac)
{
    UInt  Retry = 4;
    UInt  OperModeReg;

    // Disable MAC Transmit and Receive
    DtaMacCEnableTx(pPhyMac->m_pMacRegs, 0);
    DtaMacCEnableRx(pPhyMac->m_pMacRegs, 0);

   // Wait until transmit and receive is disabled
    while ((DtaMacCStatIsRxBusy(pPhyMac->m_pMacRegs, HAS_MACBUG(pPhyMac)) 
            || DtaMacCStatIsTxBusy(pPhyMac->m_pMacRegs, HAS_MACBUG(pPhyMac))) && Retry!=0)
    {
        DtSleep(1);
        Retry--;
    }

    if (Retry == 0)
        DtDbgOut(ERR, PHYMAC, "IpPort %i: Tx/Rx not disabled within time.",
                                                    pPhyMac->m_IpPortIndex);

    // Get opermode register
    OperModeReg = DtaMacCOperModeGet(pPhyMac->m_pMacRegs);

    // Update Speed/Duplex
    if (pPhyMac->m_PhySpeed==DTA_PHY_SPEED_1000_MASTER ||
                                            pPhyMac->m_PhySpeed==DTA_PHY_SPEED_1000_SLAVE)
        // 1 GB network enabled
        OperModeReg = OperModeReg | DTA_MACC_OPMODE_ENABLE_1GB;
    else 
        // 10/100MB network enabled
        OperModeReg = OperModeReg & ~DTA_MACC_OPMODE_ENABLE_1GB;
    
    if (pPhyMac->m_PhySpeed==DTA_PHY_SPEED_10_FULL ||
                                         pPhyMac->m_PhySpeed==DTA_PHY_SPEED_100_FULL ||
                                         pPhyMac->m_PhySpeed==DTA_PHY_SPEED_1000_MASTER ||
                                         pPhyMac->m_PhySpeed==DTA_PHY_SPEED_1000_SLAVE)
        // Full duplex mode
        OperModeReg = OperModeReg | DTA_MACC_OPMODE_ENABLE_FULLDUPLEX;
    else 
        // Half duplex
        OperModeReg = OperModeReg & ~DTA_MACC_OPMODE_ENABLE_FULLDUPLEX;
    
    // Write data back to MAC
    DtaMacCOperModeSet(pPhyMac->m_pMacRegs, OperModeReg);

    if (Retry != 0)
        pPhyMac->m_MacSpeed = pPhyMac->m_PhySpeed;
    else
        pPhyMac->m_MacSpeed  = DTA_PHY_SPEED_NOT_SET;
    
    // Wait minimal 10uS before enabling MAC to wait for stable clock 
    DtSleep(1);

    // Enable MAC Transmit and Receive
    DtaMacCEnableTx(pPhyMac->m_pMacRegs, 1);
    DtaMacCEnableRx(pPhyMac->m_pMacRegs, 1);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacCUpdatePacketFiltering -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaMacCUpdatePacketFiltering(PhyMac* pPhyMac)
{
    UInt  OperModeReg;
    
    // Get opermode register
    OperModeReg = DtaMacCOperModeGet(pPhyMac->m_pMacRegs);

    // Promiscuous
    if ((pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_PROMISCUOUS) != 0) 
    {
        OperModeReg = OperModeReg | DTA_MACC_OPMODE_PROMISCUOUS;
        DtDbgOut(AVG, PHYMAC, "IpPort %i: Promiscuous mode ON", pPhyMac->m_IpPortIndex);
    } else {
        OperModeReg = OperModeReg & ~DTA_MACC_OPMODE_PROMISCUOUS;
        DtDbgOut(AVG, PHYMAC, "IpPort %i: Promiscuous mode OFF", pPhyMac->m_IpPortIndex);
    }

    // Pass all multicast
    if ((pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_ALL_MULTICAST) != 0)
        OperModeReg = OperModeReg | DTA_MACC_OPMODE_PASS_ALL_MULTICAST;
    else 
        OperModeReg = OperModeReg & ~DTA_MACC_OPMODE_PASS_ALL_MULTICAST;

    // Store and forward on
    OperModeReg = OperModeReg | DTA_MACC_OPMODE_STORE_AND_FORWARD;

    // Write data back to MAC
    DtaMacCOperModeSet(pPhyMac->m_pMacRegs, OperModeReg);

    return DT_STATUS_OK;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ MAC Altera +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacAWaitMacReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaMacAWaitMacReady(PhyMac* pPhyMac)
{
    UInt32  Retry = 0;

    // Wait until reset is complete and MAC is ready
    // Wait max. 1 second
    while (DtaMacASwResetGet(pPhyMac->m_pMacRegs)!=0 && Retry<100)
    {
        DtSleep(10);
        Retry++;
    }
    if (Retry >= 100)
    {
        DtDbgOut(MAX, PHYMAC, "IpPort %i: ERROR WAITING FOR MAC to be ready!!", 
                                                                  pPhyMac->m_IpPortIndex);
        DT_ASSERT(FALSE);
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacAInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaMacAInit(PhyMac* pPhyMac)
{
    // Software reset MAC
    DtaMacASwResetSet(pPhyMac->m_pMacRegs);

    // Wait for reset to be cleared
    DtaMacAWaitMacReady(pPhyMac);

    // Set PHY address
    DtaMacASetPhyAddress0(pPhyMac->m_pMacRegs, pPhyMac->m_IpPortIndex);

    // Enable phy so clocks are available
    // Initialise speed to auto negotiate
    DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_BMCR, DTA_PHY_BMCR_AUTO_NEG_EN_MASK
                                                        | DTA_PHY_BMCR_AUTO_NEG_RST_MASK);

    // 16 bytes padding are added by software (Needed by formware)
    DtaMacASetTxCmdStatReg(pPhyMac->m_pMacRegs, DTA_MACA_TXCMDSTAT_SHIFT_16);

    // Add 16 bytes padding before Rx packet (Needed by firmware)
    DtaMacASetRxCmdStatReg(pPhyMac->m_pMacRegs, DTA_MACA_RXCMDSTAT_SHIFT_16);

    // Enable CRC forward
    DtaMacASetCrcFwd(pPhyMac->m_pMacRegs, 1);

    // Set the Rx FIFO full threshold. This is needed because the firmware
    // uses the 'ff_rx_dsav' signal that is asserted when this threshold is reached.
    // A value of 0 disables the 'ff_rx_dsav' value. Value must be >=3 due to internal
    // pipeline latency.
    DtaMacARxSectionEmptySet(pPhyMac->m_pMacRegs, 3);
    DtaMacARxSectionFullSet(pPhyMac->m_pMacRegs, 3);
    DtaMacATxSectionEmptySet(pPhyMac->m_pMacRegs, 3);
    DtaMacATxSectionFullSet(pPhyMac->m_pMacRegs, 16);
    DtaMacARxSectionAlmostEmptySet(pPhyMac->m_pMacRegs, 3);
    DtaMacARxSectionAlmostFullSet(pPhyMac->m_pMacRegs, 3);
    DtaMacATxSectionAlmostEmptySet(pPhyMac->m_pMacRegs, 8);
    DtaMacATxSectionAlmostFullSet(pPhyMac->m_pMacRegs, 5);

    // Disable promiscious mode. We use our own MAC-address filter only for multicast.
    DtaMacASetPromisEn(pPhyMac->m_pMacRegs, 0);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacAUpdateSpeedDuplex -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaMacAUpdateSpeedDuplex(PhyMac* pPhyMac)
{
    UInt  CmdConfig;

    // Disable MAC Transmit and Receive
    DtaMacAEnableTx(pPhyMac->m_pMacRegs, 0);
    DtaMacAEnableRx(pPhyMac->m_pMacRegs, 0);

    // Wait for disable to complete (see MAC documentation)
    while (!DtaMacAIsRxTxDisabled(pPhyMac->m_pMacRegs));

    CmdConfig = DtaMacACmdConfigGet(pPhyMac->m_pMacRegs);

    if (pPhyMac->m_PhySpeed==DTA_PHY_SPEED_1000_MASTER ||
                                            pPhyMac->m_PhySpeed==DTA_PHY_SPEED_1000_SLAVE)
    {
        // 1 GB network enabled
        CmdConfig = CmdConfig | DTA_MACA_CMD_CONFIG_ETH_SPEED;
        CmdConfig = CmdConfig & ~DTA_MACA_CMD_CONFIG_ENA_10;
    } else {
        // 10/100MB network enabled
        CmdConfig = CmdConfig & ~DTA_MACA_CMD_CONFIG_ETH_SPEED;
        if (pPhyMac->m_PhySpeed==DTA_PHY_SPEED_10_HALF || 
                                               pPhyMac->m_PhySpeed==DTA_PHY_SPEED_10_FULL)
            CmdConfig = CmdConfig | DTA_MACA_CMD_CONFIG_ENA_10;
        else
            CmdConfig = CmdConfig & ~DTA_MACA_CMD_CONFIG_ENA_10;
    }

    if (pPhyMac->m_PhySpeed==DTA_PHY_SPEED_10_FULL || 
                                         pPhyMac->m_PhySpeed==DTA_PHY_SPEED_100_FULL ||
                                         pPhyMac->m_PhySpeed==DTA_PHY_SPEED_1000_MASTER ||
                                         pPhyMac->m_PhySpeed==DTA_PHY_SPEED_1000_SLAVE)
        // Full duplex mode
        CmdConfig = CmdConfig & ~DTA_MACA_CMD_CONFIG_HD_ENA;
    else 
        // Half duplex
        CmdConfig = CmdConfig | DTA_MACA_CMD_CONFIG_HD_ENA;

    // Write data back to MAC
    DtaMacACmdConfigSet(pPhyMac->m_pMacRegs, CmdConfig);

    pPhyMac->m_MacSpeed = pPhyMac->m_PhySpeed;
    
    // Reset MAC (see MAC documentation)
    DtaMacASwResetSet(pPhyMac->m_pMacRegs);

    // Wait for reset done
    while (DtaMacASwResetGet(pPhyMac->m_pMacRegs) != 0);

    // Enable MAC Transmit and Receive
    DtaMacAEnableTx(pPhyMac->m_pMacRegs, 1);
    DtaMacAEnableRx(pPhyMac->m_pMacRegs, 1);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacAUpdatePacketFiltering -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaMacAUpdatePacketFiltering(PhyMac* pPhyMac)
{
    DtaIpPort*  pIpPort =  
                      &pPhyMac->m_pDvcData->m_IpDevice.m_pIpPorts[pPhyMac->m_IpPortIndex];
    if ((pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_PROMISCUOUS) != 0)
        DtaMacASetPromisEn(pPhyMac->m_pMacRegs, 1);
    else
        DtaMacASetPromisEn(pPhyMac->m_pMacRegs, 0);

    // The MAC does not filter the multicasts. This is done in a seperate MAC-
    // filter. We have to enable/disable that filter too.
    if (((pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_ALL_MULTICAST) != 0) ||
                          ((pPhyMac->m_NwDrvPacketFilter & DTA_MAC_FLT_PROMISCUOUS) != 0))
        DtaMacAddrFilterEnable(pIpPort->m_IpPortType2.m_pAddrMatcherRegs, 0);
    else
        DtaMacAddrFilterEnable(pIpPort->m_IpPortType2.m_pAddrMatcherRegs, 1);
    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ PHY +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhySetSpeedDtapi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Update the speed the DTAPI wants. If ForceSpeed is false, the speed
// is set to the speed the network driver wants.
//
// The DtaPhySetSpeedDuplex is not thread safe. The network driver is using this
// functions during initialisation. Then the DTAPI is not yet running
//
DtStatus  DtaPhySetSpeedDtapi(PhyMac* pPhyMac, UInt Speed, Bool ForceSpeedEnable)
{
    if (pPhyMac->m_ForceSpeedDtapiEnable!=ForceSpeedEnable || 
                                                      pPhyMac->m_ForcedSpeedDtapi!= Speed)
    {
        pPhyMac->m_ForcedSpeedDtapi = Speed;
        pPhyMac->m_ForceSpeedDtapiEnable = ForceSpeedEnable;
        DtaPhySetSpeedDuplex(pPhyMac);
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhySetSpeedNw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Updates the speed the network driver wants. This will reset the DTAPI speed setting
//
DtStatus  DtaPhySetSpeedNw(PhyMac* pPhyMac, UInt Speed)
{
    // Reset DTAPI states
    pPhyMac->m_ForceSpeedDtapiEnable = FALSE;
    pPhyMac->m_LoopbackEnable = FALSE;
    
    // Update speed
    pPhyMac->m_NwDrvSpeed = Speed;
    DtaPhySetSpeedDuplex(pPhyMac);
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhyGetSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtaPhyGetSpeed(PhyMac* pPhyMac, Bool* pForceSpeedDtapiEnable)
{
    if (pForceSpeedDtapiEnable != NULL)
        *pForceSpeedDtapiEnable = pPhyMac->m_ForceSpeedDtapiEnable;
    
    if (pPhyMac->m_PhyLink == DTA_PHY_LINK_UP)
        return pPhyMac->m_PhySpeed;
    return DTA_PHY_SPEED_NO_LINK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGetPhyRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt32  DtaGetPhyRegister(PhyMac* pPhyMac, UInt32 Reg)
{   UInt32  Data;
    if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
        Data = DtaMacAGetPhyRegister(pPhyMac->m_pMacRegs, Reg*4);
    else
        Data = DtaMacCGetPhyRegister(pPhyMac, Reg);
    DtDbgOut(MAX, PHYMAC, "IpPort %i: Reading PHY register. Reg: %xh(%d) Value: %xh", 
                                                  pPhyMac->m_IpPortIndex, Reg, Reg, Data);
    return Data;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSetPhyRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaSetPhyRegister(PhyMac* pPhyMac, UInt32 Reg, UInt32 Data)
{   
    DtDbgOut(MAX, PHYMAC, "IpPort %i: Writing PHY register. Reg: %xh(%d) Value: %xh", 
                                                  pPhyMac->m_IpPortIndex, Reg, Reg, Data);
    
    if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
        DtaMacASetPhyRegister(pPhyMac->m_pMacRegs, Reg*4, Data);
    else
        DtaMacCSetPhyRegister(pPhyMac, Reg, Data);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhySetLoopbackSpeedSpecificCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set the phy specific control register for loopback speed setting
// For Marvell Phy 88E1119R + 88E1121R
// !! Different code for these phy models
//
void  DtaPhySetLoopbackSpeedSpecificCtrl(PhyMac* pPhyMac)
{
    UInt16  Data;
    UInt  Speed;
    
     if (pPhyMac->m_ForceSpeedDtapiEnable)
        Speed = pPhyMac->m_ForcedSpeedDtapi;
     else
        Speed = pPhyMac->m_NwDrvSpeed;

    DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_PAGE, 2);
            
    
    if (Speed == DTA_PHY_SPEED_AUTO_DETECT)
        // Set speed to 1Gb, auto negotiation is not working in loopback mode.
        Speed = DTA_PHY_SPEED_1000_SLAVE;
    
    if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)    
        Data = 0x1070; // RX CLK transition when data stable +
                        // TX CLK internally delayed
    else
        Data =  0x1040; // (Hardware reset value, excluding speed)
    
    if (pPhyMac->m_PhyModel==0x28)
    {
        // Phy 88E1119R
        switch (Speed)
        {
        case DTA_PHY_SPEED_10_HALF:
        case DTA_PHY_SPEED_10_FULL:
            Data |= DTA_PHY_MAR_MSC_DEF_MAC_SPEED_10_2d5MHZ_2d5MHZ;
            break;
        case DTA_PHY_SPEED_100_HALF:
        case DTA_PHY_SPEED_100_FULL:
            Data |= DTA_PHY_MAR_MSC_DEF_MAC_SPEED_100_25MHZ_25MHZ;
            break;
        case DTA_PHY_SPEED_1000_MASTER:
            Data |= DTA_PHY_MAR_MSC_DEF_MAC_SPEED_1000_2d5MHZ_2d5MHZ;
            break;
        case DTA_PHY_SPEED_1000_SLAVE:
            Data |= DTA_PHY_MAR_MSC_DEF_MAC_SPEED_1000_25MHZ_25MHZ;
            break;
        default:
            DT_ASSERT(FALSE);
        }
    } else {
        switch (Speed)
        {
        case DTA_PHY_SPEED_10_HALF:
        case DTA_PHY_SPEED_10_FULL:
            Data |= DTA_PHY_MAR_MSC_DEF_MAC_SPEED_10;
            break;
        case DTA_PHY_SPEED_100_HALF:
        case DTA_PHY_SPEED_100_FULL:
            Data |= DTA_PHY_MAR_MSC_DEF_MAC_SPEED_100;
            break;
        case DTA_PHY_SPEED_1000_MASTER:
        case DTA_PHY_SPEED_1000_SLAVE:
            Data |= DTA_PHY_MAR_MSC_DEF_MAC_SPEED_1000;
            break;
        default:
            DT_ASSERT(FALSE);
        }
    }
    DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_MAC_SPEC_CTRL, Data);

    DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_PAGE, 0);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtaPhySetLoopbackSpeedExtendedPhyCtrl -.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set the extended phy control register for loopback speed setting
//
void  DtaPhySetLoopbackSpeedExtendedPhyCtrl(PhyMac* pPhyMac)
{
    UInt16  Data = 0x0c00; // Downshift counter = 0x6 (HW reset value)
    UInt  Speed;

    // Enable TX_CLK speed-1000BASE-T link
    // If not enabled, MAC can not stop the transmitter
    // when speed must be changed.
    Data = Data | DTA_PHY_MAR_EPC_DEF_MAC_SPEED_TXCLK;
    
     if (pPhyMac->m_ForceSpeedDtapiEnable)
        Speed = pPhyMac->m_ForcedSpeedDtapi;
     else
        Speed = pPhyMac->m_NwDrvSpeed;

    if (Speed == DTA_PHY_SPEED_AUTO_DETECT)
        // Set speed to 1Gb, auto negotiation is not working in loopback mode.
        Speed = DTA_PHY_SPEED_1000_SLAVE;

    switch (Speed)
    {
    case DTA_PHY_SPEED_10_HALF:
    case DTA_PHY_SPEED_10_FULL:
        Data |= DTA_PHY_MAR_EPC_DEF_MAC_SPEED_10;
        break;
    case DTA_PHY_SPEED_100_HALF:
    case DTA_PHY_SPEED_100_FULL:
        Data |= DTA_PHY_MAR_EPC_DEF_MAC_SPEED_100;
        break;
    case DTA_PHY_SPEED_1000_MASTER:
    case DTA_PHY_SPEED_1000_SLAVE:
        Data |= DTA_PHY_MAR_EPC_DEF_MAC_SPEED_1000;
        break;
    default:
        DT_ASSERT(FALSE);
    }
    DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_EXT_PHY_CTRL, Data);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhySetSpeedDuplex -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This functions sets the Speed/Duplex and loopback mode.
//
DtStatus  DtaPhySetSpeedDuplex(PhyMac* pPhyMac)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt16  Data;
    UInt  Speed;

    //.-.-.-.-.-.-.-.-.-.-.-.- First we set the 1GB Master/Slave -.-.-.-.-.-.-.-.-.-.-.-.-

    // Disable manual master/slave mode and set advertise capability
    Data = DTA_PHY_1KTCR_MSCFG_CAP_MASK;

    // The network driver can not force the speed to 1GB, so we do not need to
    // check the Network driver speed here
    if (pPhyMac->m_ForceSpeedDtapiEnable)
    {
        Speed = pPhyMac->m_ForcedSpeedDtapi;
        
        if (Speed == DTA_PHY_SPEED_1000_MASTER)
        {
            // Set 1GB mode to master
            Data |= DTA_PHY_1KTCR_MSCFG_EN_MASK;
            Data |= DTA_PHY_1KTCR_MSCFG_MASTER_MASK;
        } else if (Speed == DTA_PHY_SPEED_1000_SLAVE)
        {
            // Set 1GB mode to slave
            Data |= DTA_PHY_1KTCR_MSCFG_EN_MASK;
        }
    } else
        Speed = pPhyMac->m_NwDrvSpeed;

    DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_1KTCR, Data);
    
    //-.-.-.-.-.-.-.-.-.-.-.- Then we set the 10/100/1GB HALF/FULL -.-.-.-.-.-.-.-.-.-.-.-

    switch (Speed)
    {
    case DTA_PHY_SPEED_AUTO_DETECT:
        // Enable and restart Auto-Negotiation
        Data = DTA_PHY_BMCR_AUTO_NEG_EN_MASK | DTA_PHY_BMCR_AUTO_NEG_RST_MASK;
        break;
    case DTA_PHY_SPEED_10_HALF:
        Data = 0;
        break;
    case DTA_PHY_SPEED_10_FULL:
        Data = DTA_PHY_BMCR_DUPLEX_MASK;
        break;
    case DTA_PHY_SPEED_100_HALF:
        Data = DTA_PHY_BMCR_SPEED0_MASK;
        break;
    case DTA_PHY_SPEED_100_FULL:
        Data = DTA_PHY_BMCR_SPEED0_MASK | DTA_PHY_BMCR_DUPLEX_MASK;
        break;
    case DTA_PHY_SPEED_1000_MASTER:
        Data = DTA_PHY_BMCR_SPEED1_MASK | DTA_PHY_BMCR_DUPLEX_MASK;
        break;
    case DTA_PHY_SPEED_1000_SLAVE:
        Data = DTA_PHY_BMCR_SPEED1_MASK | DTA_PHY_BMCR_DUPLEX_MASK;
        break;
    default:
        DT_ASSERT(FALSE);
    }

    // For Marvell Phy, the speed is only updated after a software reset.
    // The loopback status is also reset during the software reset. So for Marvell we have
    // to set the loopback after the software reset. For National, no software reset is
    // needed and we can enable the loopback mode here.
    if (pPhyMac->m_LoopbackEnable && pPhyMac->m_PhyType==DTA_PHY_NATIONAL)
        Data = Data | DTA_PHY_BMCR_LOOPBACK_MASK;
    else if (pPhyMac->m_PhyType == DTA_PHY_MARVELL)
        Data = Data | DTA_PHY_BMCR_SWRESET_MASK;

    DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_BMCR, Data);
    
    // For Marvell, we enable loopback now
    if (pPhyMac->m_LoopbackEnable && pPhyMac->m_PhyType==DTA_PHY_MARVELL)
    {
        // Disable software reset
        Data = Data & ~DTA_PHY_BMCR_SWRESET_MASK;

        // First set the loopback speed

        // For Marvel 88E1119R(0x28) or 88E1121R(0x34) we have to set the Specific Ctrl
        // register.
        // !! Different code for 88E119R and 88E1121R.
        if (pPhyMac->m_PhyModel==0x28 || pPhyMac->m_PhyModel==0x34)
            DtaPhySetLoopbackSpeedSpecificCtrl(pPhyMac);
        else
            DtaPhySetLoopbackSpeedExtendedPhyCtrl(pPhyMac);

        // Wait minimal 700 ms before enabling loopback. 
        // See Marvell Release notes - 88E1111 Rev B2.
        DtSleep(800);

        // Enable loopback
        Data = Data | DTA_PHY_BMCR_LOOPBACK_MASK;
        DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_BMCR, Data);
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhyClearInterrupts -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Clear the PHY interrupts
//
void  DtaPhyClearInterrupts(PhyMac* pPhyMac)
{   
    if (pPhyMac->m_PhyType == DTA_PHY_NATIONAL)
        DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_NAT_INT_CLR,
                                                               DTA_PHY_NAT_INT_MASK_BITS);
    else
        DtaGetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_INT_CLR);

}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhyUpdateLinkStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaPhyUpdateLinkStatus(PhyMac* pPhyMac)
{
    UInt32  LinkStatus;
    UInt  Speed;
    UInt  Link;
    UInt  Duplex;
    Bool  Master = FALSE;
    
    LinkStatus = DtaGetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_LINK_AN);
    
    if (pPhyMac->m_PhyType == DTA_PHY_NATIONAL) 
    {
        Speed = (LinkStatus & DTA_PHY_LINK_AN_NAT_SPEEDST_MASK)
                                                     >> DTA_PHY_LINK_AN_NAT_SPEEDST_SHIFT;
        Link = (LinkStatus & DTA_PHY_LINK_AN_NAT_LINKST_MASK)
                                                      >> DTA_PHY_LINK_AN_NAT_LINKST_SHIFT;
        Duplex = (LinkStatus & DTA_PHY_LINK_AN_NAT_DUPLEXST_MASK)
                                                    >> DTA_PHY_LINK_AN_NAT_DUPLEXST_SHIFT;
        Master = (LinkStatus & DTA_PHY_LINK_AN_NAT_MASTER_MASK) != 0;
    } else {
        Speed = (LinkStatus & DTA_PHY_LINK_AN_MAR_SPEEDST_MASK) 
                                                     >> DTA_PHY_LINK_AN_MAR_SPEEDST_SHIFT;
        Link = (LinkStatus & DTA_PHY_LINK_AN_MAR_LINKST_MASK)
                                                      >> DTA_PHY_LINK_AN_MAR_LINKST_SHIFT;
        Duplex = (LinkStatus & DTA_PHY_LINK_AN_MAR_DUPLEXST_MASK)
                                                    >> DTA_PHY_LINK_AN_MAR_DUPLEXST_SHIFT;
    
        if (Speed == DTA_PHY_LINK_AN_SPEED_1000)
        {
            UInt32 GbStatus;
            // For the Marvel PHY, the Master/slave status is stored in another register
            // Get master/slave configuration
            GbStatus = DtaGetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_1KSTSR);
            if ((GbStatus & DTA_PHY_1KSTR_MSCFG_MASTER_MASK) != 0)
                Master = TRUE;
            else 
                Master = FALSE;
        }
    }
    switch(Speed)
    {
    case DTA_PHY_LINK_AN_SPEED_10:
        if (Duplex==DTA_PHY_LINK_AN_DUPLEX_HALF)
            pPhyMac->m_PhySpeed = DTA_PHY_SPEED_10_HALF;
        else
            pPhyMac->m_PhySpeed = DTA_PHY_SPEED_10_FULL;
        break;
    case DTA_PHY_LINK_AN_SPEED_100:
        if (Duplex==DTA_PHY_LINK_AN_DUPLEX_HALF)
            pPhyMac->m_PhySpeed = DTA_PHY_SPEED_100_HALF;
        else
            pPhyMac->m_PhySpeed = DTA_PHY_SPEED_100_FULL;
        break;
    case DTA_PHY_LINK_AN_SPEED_1000:
        if (Master)
            pPhyMac->m_PhySpeed = DTA_PHY_SPEED_1000_MASTER;
        else
            pPhyMac->m_PhySpeed = DTA_PHY_SPEED_1000_SLAVE;
        break;
    case DTA_PHY_LINK_AN_UNDEFINED:
        pPhyMac->m_PhySpeed = DTA_PHY_SPEED_NO_LINK;
        break;
    default:
        DT_ASSERT(FALSE);
    }
    if (Link == DTA_PHY_LINK_AN_LINK_UP)
        pPhyMac->m_PhyLink = DTA_PHY_LINK_UP;
    else
        pPhyMac->m_PhyLink = DTA_PHY_LINK_DOWN;

    // Report immediatly fo firmware that link is down
    if (pPhyMac->m_PhyLink == DTA_PHY_LINK_DOWN)
        DtaNwCtrlSetLinkValid(pPhyMac->m_pGenNwRegs, pPhyMac->m_PhyLink == 
                                                                         DTA_PHY_LINK_UP);

    DtDbgOut(AVG, PHYMAC, "IpPort %i: Speed=%s(%xh) Link=%s(%xh) Duplex=%s(%xh)",
                                     pPhyMac->m_IpPortIndex,
                                     (Speed==DTA_PHY_LINK_AN_SPEED_10?"10Mb":
                                     (Speed==DTA_PHY_LINK_AN_SPEED_100?"100Mb":
                                     (Speed==DTA_PHY_LINK_AN_SPEED_1000?"1Gb": "???"))),
                                     Speed,
                                     (Link==DTA_PHY_LINK_AN_LINK_DOWN?"down":
                                     (Link==DTA_PHY_LINK_AN_LINK_UP?"up": "???")),
                                     Link,
                                     (Duplex==DTA_PHY_LINK_AN_DUPLEX_HALF?"half":
                                     (Duplex==DTA_PHY_LINK_AN_DUPLEX_FULL?"full": "???")),
                                     Duplex);
    
    // Report link status to network driver
    if (pPhyMac->m_NwDrvLink != pPhyMac->m_PhyLink)
    {
        DtFastMutexAcquire(&pPhyMac->m_Callbacks.m_FastMutex);
        if (pPhyMac->m_Callbacks.m_LinkStatusCallback != NULL)
        {
            DT_ASSERT(pPhyMac->m_Callbacks.m_Registered == TRUE);
            pPhyMac->m_Callbacks.m_LinkStatusCallback(pPhyMac->m_PhyLink,
                                                         pPhyMac->m_Callbacks.m_pContext);
            pPhyMac->m_NwDrvLink = pPhyMac->m_PhyLink;
        }
        DtFastMutexRelease(&pPhyMac->m_Callbacks.m_FastMutex);
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhyInterruptWi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaPhyInterruptWi(DtWorkItemArgs* pArgs)
{
    PhyMac*  pPhyMac = (PhyMac*)pArgs->m_pContext;

    // First clear the PHY interrupt
    DtaPhyClearInterrupts(pPhyMac);

    // Enable the phy interrupt again
    DtaNwCtrlSetMdioIntEn(pPhyMac->m_pGenNwRegs, 1);

    // Get the link status and update to Network driver
    DtaPhyUpdateLinkStatus(pPhyMac);

    // Update MAC Speed/Duplex
    DtaMacUpdateSpeedDuplex(pPhyMac);

    // Report link valid to firmware after MAC is updated and ready
    if (pPhyMac->m_PhyLink == DTA_PHY_LINK_UP)
        DtaNwCtrlSetLinkValid(pPhyMac->m_pGenNwRegs, pPhyMac->m_PhyLink == 
                                                                         DTA_PHY_LINK_UP);

    // Update packet filtering. We do it here, to sync it with changing the 
    // Operational Register
    DtaMacUpdatePacketFiltering(pPhyMac);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhyInterruptDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function will run a WorkItem for reading and writing from/to the PHY registers
//
void  DtaPhyInterruptDpc(DtDpcArgs* pArgs)
{
    DtStatus  Status;
    PhyMac*  pPhyMac = (PhyMac*)pArgs->m_pContext;
    DtWorkItemArgs  DtWorkItemArgs;
    DtWorkItemArgs.m_pContext = pPhyMac;
    
    Status = DtWorkItemSchedule(&pPhyMac->m_PhyIntWorkItem, &DtWorkItemArgs);
    if (!DT_SUCCESS(Status))
    {
        if (Status != DT_STATUS_IN_USE)
            // We enable the interrupt because the workitem is not scheduled
            // and nothing in the queue
            DtaNwCtrlSetMdioIntEn(pPhyMac->m_pGenNwRegs, 1);
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaInitPhy -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaInitPhy(PhyMac* pPhyMac)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  Data;
    
    // Get PhyId
    Data = DtaGetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_PHYIDR1);
    switch (Data)
    {   
    case 0x2000: pPhyMac->m_PhyType = DTA_PHY_NATIONAL; break;
    case 0x0141: pPhyMac->m_PhyType = DTA_PHY_MARVELL; break;
    default: 
        DtDbgOut(ERR, PHYMAC, "IpPort %i: PhyType: %s, 0x%x", pPhyMac->m_IpPortIndex,
                                                                       "UNKNOWN\0", Data);
        return DT_STATUS_UNKNOWN_PHY;
    }
    
    // Get PhyModel
    Data = DtaGetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_PHYIDR2);
    pPhyMac->m_PhyModel = (UInt8)((Data >> 4) & 0x3f);

    DtDbgOut(AVG, PHYMAC, "IpPort %i: PhyType: %s, PhyModel: 0x%x", 
                          pPhyMac->m_IpPortIndex, (pPhyMac->m_PhyType == DTA_PHY_MARVELL ?
                          "MARVELL\0":"NATIONAL\0"), pPhyMac->m_PhyModel);
    
    // Disable interrupt enable bit. Will be enabled later.
    DtaNwCtrlSetMdioIntEn(pPhyMac->m_pGenNwRegs, FALSE);

    // Set the Phy interrupt mask to interrupt during link, duplex and speed change
    if (pPhyMac->m_PhyType == DTA_PHY_NATIONAL)
        DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_NAT_INT_MASK,
                                                               DTA_PHY_NAT_INT_MASK_BITS);
    else
        DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_INT_MASK,
                                                               DTA_PHY_MAR_INT_MASK_BITS);

    // Set Phy specific registers
    // Marvell: LED-Control
    // National: AUX_CTRL to force GMII mode (rev.2 board)
    if (pPhyMac->m_PhyType == DTA_PHY_NATIONAL)    // DTA-160
    {
        // Force GMII-mode
        Data = 0x8000;  // Auto-MDIX enable (and GMII-mode)
        DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_NAT_AUX_CTRL, Data);
    } else if ((pPhyMac->m_PhyType == DTA_PHY_MARVELL) && 
                                       (pPhyMac->m_PhyModel ==0xC))    // 88E1111: DTA-160
    {
        // Set LED control
        Data = 0x1140;  // 21ms to 42ms Pulse stretch, 84ms blink rate,
                        // transmit or receive activity
        DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_LED_CTRL, Data);
    } else {    // m_PhyModel == 0x28) // 88E1119R: DTA-2160
                // m_PhyModel == 0x34) // 88E1121R: DTA-2162
        if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
        {
            // Set MAC specific control register 2 page 2.
            // A software reset is needed to take the new value into account.
            // This will be performed later in this function

            // We go to page 2 
            DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_PAGE, 2);
            Data = 0x1070;  // RX CLK transition when data stable +
                            // TX CLK internally delayed
            DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_MAC_SPEC_CTRL, Data);
            
            // Restore page 0
            DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_PAGE, 0);
        }

        // First set the power down to off
        Data = 0x6060;
        DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_COP_SPEC_CTRL1, Data);
        // We go to page 3 programming
        DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_PAGE, 3);
        
        // Page-address set, go further with led function control
        Data = 0x230; // Interrupt(2), on activity,(3) on link(0)
        DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_LED_FUNC_CTRL, Data);
        // Program LED polarity
        DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_LED_POL_CTRL, 0x25);

        // Program LED timer
        Data = 0x1140;  // 21ms to 42ms Pulse stretch, 84ms blink rate,
                        // transmit or receive activity
        DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_LED_TIM_CTRL, Data);

        // Restore page 0
        DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_MAR_PAGE, 0);
    }

    // Initialise speed to auto negotiate. Can be overruled by network driver later.
    // For the Marvell Phy, speed is only updated after a software reset, so we force a SW
    // reset here.
    Data = DTA_PHY_BMCR_AUTO_NEG_EN_MASK | DTA_PHY_BMCR_AUTO_NEG_RST_MASK;
    if (pPhyMac->m_PhyType == DTA_PHY_MARVELL)
        Data = Data | DTA_PHY_BMCR_SWRESET_MASK;
    DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_BMCR, Data);

    DtaMacUpdateMacAddress(pPhyMac);
    
    // Last state
    
    // Clear the Phy interrupt, enable MDIO interrupt and get link status.
    // To synchronise execution with the ISR, we schedule the PhyInt workitem here.
    if (DT_SUCCESS(Status))
    {
        DtWorkItemArgs  WorkItemArgs;
        WorkItemArgs.m_pContext = pPhyMac;
        Status = DtWorkItemSchedule(&pPhyMac->m_PhyIntWorkItem, &WorkItemArgs);
    }
    if (DT_SUCCESS(Status))
        pPhyMac->m_Initialized = TRUE;
    return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ PhyMac +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhyMacRegisterCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaPhyMacRegisterCallback(
    PhyMac*  pPhyMac,
    void*  pContext,
    LinkStatFunc  LinkStatCallback)
{
    DtStatus  Status = DT_STATUS_OK;
    PhyMacCallbackFuncs*  pCallbacks = &pPhyMac->m_Callbacks;
    
    DtFastMutexAcquire(&pCallbacks->m_FastMutex);
    if (pCallbacks->m_Registered)
        Status = DT_STATUS_IN_USE;
    else {
        pCallbacks->m_pContext = pContext;
        pCallbacks->m_LinkStatusCallback = LinkStatCallback;
        pCallbacks->m_Registered = TRUE;
        pPhyMac->m_NwDrvConnected = TRUE;
    }
    DtFastMutexRelease(&pPhyMac->m_Callbacks.m_FastMutex);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhyMacUnregisterCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaPhyMacUnregisterCallback(
    PhyMac*  pPhyMac,
    void*  pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    PhyMacCallbackFuncs*  pCallbacks = &pPhyMac->m_Callbacks;
    
    DtFastMutexAcquire(&pCallbacks->m_FastMutex);
    if (!pCallbacks->m_Registered)
        Status = DT_STATUS_NOT_INITIALISED;
    else if (pCallbacks->m_pContext != pContext)
        Status = DT_STATUS_INVALID_PARAMETER;
    else {
        pCallbacks->m_pContext = NULL;
        pCallbacks->m_LinkStatusCallback = NULL;
        pCallbacks->m_Registered = FALSE;
        pPhyMac->m_NwDrvConnected = FALSE;
    }
    DtFastMutexRelease(&pPhyMac->m_Callbacks.m_FastMutex);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhyMacIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaPhyMacIoctl(
    DtaDeviceData*  pDvcData,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    PhyMac*  pPhyMac = NULL;
    
    DtaIoctlPhyMacCmdInput*  pPhyMacCmdInput = 
                                          (DtaIoctlPhyMacCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlPhyMacCmdOutput*  pPhyMacCmdOutput =
                                        (DtaIoctlPhyMacCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlPhyMacCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlPhyMacCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    if (pPhyMacCmdInput->m_IpPortIndex >= pDvcData->m_NumIpPorts)
        return DT_STATUS_INVALID_PARAMETER;

     pPhyMac = &pDvcData->m_IpDevice.m_pIpPorts[pPhyMacCmdInput->m_IpPortIndex].m_PhyMac;

    // Determine final required output/input sizes
    switch (pPhyMacCmdInput->m_Cmd)
    {
    case DTA_PHYMAC_CMD_GET_MACADDRESS_PERM:
        pCmdStr = "DTA_PHYMAC_CMD_GET_MACADDRESS_PERM";
        InReqSize = 0;
        OutReqSize += sizeof(DtaIoctlPhyMacGetMacAddressPermOutput);
        break;
    case DTA_PHYMAC_CMD_SET_MACADDRESS:
        pCmdStr = "DTA_PHYMAC_CMD_SET_MACADDRESS";
        InReqSize += sizeof(DtaIoctlPhyMacSetMacAddressInput);
        OutReqSize = 0;
        break;
    case DTA_PHYMAC_CMD_GET_MAX_MULTICAST_LIST:
        pCmdStr = "DTA_PHYMAC_CMD_GET_MAX_MULTICAST_LIST";
        InReqSize = 0;
        OutReqSize += sizeof(DtaIoctlPhyMacGetMaxMulticastListOutput);
        break;
    case DTA_PHYMAC_CMD_SET_MULTICAST_LIST:
        pCmdStr = "DTA_PHYMAC_CMD_SET_MULTICAST_LIST";
        InReqSize += sizeof(UInt);
        OutReqSize = 0;
        break;
    case DTA_PHYMAC_CMD_SET_PACKET_FILTER:
        pCmdStr = "DTA_PHYMAC_CMD_SET_PACKET_FILTER";
        InReqSize += sizeof(DtaIoctlPhyMacGetMaxMulticastListOutput);
        OutReqSize = 0;
        break;
    case DTA_PHYMAC_CMD_GET_PACKET_FILTER:
        pCmdStr = "DTA_PHYMAC_CMD_GET_PACKET_FILTER";
        InReqSize = 0;
        OutReqSize += sizeof(DtaIoctlPhyMacGetPacketFilterOutput);
        break;
    case DTA_PHYMAC_CMD_GET_COUNTER:
        pCmdStr = "DTA_PHYMAC_CMD_GET_COUNTER";
        InReqSize += sizeof(DtaIoctlPhyMacGetCounterInput);
        OutReqSize += sizeof(DtaIoctlPhyMacGetCounterOutput);
        break;
    case DTA_PHYMAC_CMD_GET_PHY_SPEED:
        pCmdStr = "DTA_PHYMAC_CMD_GET_PHY_SPEED";
        InReqSize = 0;
        OutReqSize += sizeof(DtaIoctlPhyMacGetPhySpeedOutput);
        break;
    case DTA_PHYMAC_CMD_SET_PHY_SPEED:
        pCmdStr = "DTA_PHYMAC_CMD_SET_PHY_SPEED";
        InReqSize += sizeof(DtaIoctlPhyMacSetPhySpeedInput);
        OutReqSize = 0;
        break;
    case DTA_PHYMAC_CMD_REGISTER_CALLBACK:
        pCmdStr = "DTA_PHYMAC_CMD_REGISTER_CALLBACK";
        InReqSize += sizeof(DtaIoctlPhyMacRegisterCallbackInput);
        OutReqSize = 0;
        break;
    case DTA_PHYMAC_CMD_UNREGISTER_CALLBACK:
        pCmdStr = "DTA_PHYMAC_CMD_UNREGISTER_CALLBACK";
        InReqSize += sizeof(DtaIoctlPhyMacUnregisterCallbackInput);
        OutReqSize = 0;
        break;
    case DTA_PHYMAC_CMD_RESET_NW_STATUS:
        pCmdStr = "DTA_PHYMAC_CMD_RESET_NW_STATUS";
        InReqSize = 0;
        OutReqSize = 0;
        break;
    default:
        pCmdStr = "??UNKNOWN PHYMAC_CMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, PHYMAC, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, PHYMAC, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, PHYMAC, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pPhyMacCmdInput->m_Cmd)
        {
        case DTA_PHYMAC_CMD_GET_MACADDRESS_PERM:
            DtaMacGetMacAddressPermanent(pPhyMac, 
                                  pPhyMacCmdOutput->m_Data.m_GetMacAddressPerm.m_Address);
            break;
        case DTA_PHYMAC_CMD_SET_MACADDRESS:
            Status = DtaMacSetMacAddressCurrent(pPhyMac,
                                       pPhyMacCmdInput->m_Data.m_SetMacAddress.m_Address);
            break;
        case DTA_PHYMAC_CMD_GET_MAX_MULTICAST_LIST:
            pPhyMacCmdOutput->m_Data.m_GetMaxMulticastList.m_MaxListSize =
                                                               DTA_MAC_NUM_MULTICAST_LIST;
            break;
        case DTA_PHYMAC_CMD_SET_MULTICAST_LIST:
            if (((pIoctl->m_InputBufferSize - InReqSize) % 6)!=0)
            {
                Status = DT_STATUS_INVALID_PARAMETER;
                break;
            }
            if (pPhyMacCmdInput->m_Data.m_SetMulticastList.m_NumItems > 
                                                                 pPhyMac->m_MaxMulticasts)
            {
                Status = DT_STATUS_MULTICASTLIST_FULL;
                break;
            }
            pPhyMac->m_NumMulticasts =
                                pPhyMacCmdInput->m_Data.m_SetMulticastList.m_NumItems;
            DtMemCopy(pPhyMac->m_MulticastList,
                                   pPhyMacCmdInput->m_Data.m_SetMulticastList.m_Items,
                                   pPhyMac->m_NumMulticasts * 6);
            Status = DtaMacUpdateMacAddress(pPhyMac);
            break;
        case DTA_PHYMAC_CMD_SET_PACKET_FILTER:
            // Skip if we are in loopbackmode to keep promiscuous mode enabled.
            if (pPhyMac->m_NwDrvPacketFilter != 
                               pPhyMacCmdInput->m_Data.m_SetPacketFilter.m_PacketFilter &&
                               !pPhyMac->m_LoopbackEnable)
            {
                DtWorkItemArgs  WorkItemArgs;
                pPhyMac->m_NwDrvPacketFilter = 
                                 pPhyMacCmdInput->m_Data.m_SetPacketFilter.m_PacketFilter;
                
                DtaIpHandleNewPacketFilter(pPhyMac);

                // Trigger Workitem to update the packet filter
                WorkItemArgs.m_pContext = pPhyMac;
                DtWorkItemSchedule(&pPhyMac->m_PhyIntWorkItem, &WorkItemArgs);
            }
            break;
        case DTA_PHYMAC_CMD_GET_PACKET_FILTER:
            pPhyMacCmdOutput->m_Data.m_GetPacketFilter.m_PacketFilter = 
                                                               pPhyMac->m_MacPacketFilter;
            break;
        case DTA_PHYMAC_CMD_GET_COUNTER:
            Status = DtaMacGetCounter(pPhyMac,
                                         pPhyMacCmdInput->m_Data.m_GetCounter.m_CounterId,
                                         &pPhyMacCmdOutput->m_Data.m_GetCounter.m_Value);
            break;
        case DTA_PHYMAC_CMD_GET_PHY_SPEED:
            pPhyMacCmdOutput->m_Data.m_GetPhySpeed.m_Speed = 
                                                            DtaPhyGetSpeed(pPhyMac, NULL);
            break;
        case DTA_PHYMAC_CMD_SET_PHY_SPEED:  // Not DTAPI FORCE SPEED
            DtaPhySetSpeedNw(pPhyMac, pPhyMacCmdInput->m_Data.m_SetPhySpeed.m_Speed);
            break;
        case DTA_PHYMAC_CMD_REGISTER_CALLBACK:
            Status = DtaPhyMacRegisterCallback(pPhyMac, 
                         pPhyMacCmdInput->m_Data.m_RegisterCallback.m_pContext,
                         pPhyMacCmdInput->m_Data.m_RegisterCallback.m_LinkStatusCallback);
            break;
        case DTA_PHYMAC_CMD_UNREGISTER_CALLBACK:
            Status = DtaPhyMacUnregisterCallback(pPhyMac,
                                 pPhyMacCmdInput->m_Data.m_UnregisterCallback.m_pContext);
            break;
        case DTA_PHYMAC_CMD_RESET_NW_STATUS:
            pPhyMac->m_NwDrvSpeed = DTA_PHY_SPEED_NOT_SET;
            pPhyMac->m_NwDrvLink = DTA_PHY_LINK_NOT_SET;
            pPhyMac->m_NetworkAdminStatus = DTA_NW_ADMIN_STATUS_UP;
            pPhyMac->m_NumMulticasts = 0;
            pPhyMac->m_NwDrvPacketFilter = 0;
            break;
    
        default:
            Status = DT_STATUS_NOT_SUPPORTED;
        }
    }

    // If we failed, no data has te be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, PHYMAC, "PhyMacCmd=0x%x: NOT SUPPORTED", 
                                                                  pPhyMacCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, PHYMAC, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMacInitState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Resets the internal state of the MAC during init and powerup
// 
void  DtaMacInitState(PhyMac* pPhyMac)
{
    pPhyMac->m_MacSpeed = DTA_PHY_SPEED_NOT_SET;
    pPhyMac->m_MacPacketFilter = DTA_MAC_FLT_DIRECTED | DTA_MAC_FLT_BROADCAST;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhyMacInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaPhyMacInit(DtaIpPort* pIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    PhyMac*  pPhyMac = &pIpPort->m_PhyMac;
    DtPropertyData*  pPropData = &pIpPort->m_pDvcData->m_PropData;


    // Get MAC type from XML
    pPhyMac->m_MacType = DtPropertiesGetInt(pPropData, "NETWORK_MAC_TYPE", -1);
    
    // Check if no property error occurred
    Status = DtaPropertiesReportDriverErrors(pIpPort->m_pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Copy members from IpPort
    pPhyMac->m_pDvcData = pIpPort->m_pDvcData;
    pPhyMac->m_IpPortIndex = pIpPort->m_IpPortIndex;
    pPhyMac->m_GenNwRegsOffset = pIpPort->m_GenNwRegsOffset;

    // Set initial values
    pPhyMac->m_ForceSpeedDtapiEnable = FALSE;
    pPhyMac->m_LoopbackEnable = FALSE;
    pPhyMac->m_Initialized = FALSE;
    pPhyMac->m_AlignedPayload = (pIpPort->m_PortType == DTA_IPPORT_TYPE2);

    pPhyMac->m_NwDrvConnected = FALSE;
    pPhyMac->m_MaxMulticasts = DTA_MAC_NUM_MULTICAST_LIST;
     
    pPhyMac->m_NetworkAdminStatus = DTA_NW_ADMIN_STATUS_UP;

    // Network driver init state
    pPhyMac->m_NwDrvPacketFilter = 0;
    pPhyMac->m_NwDrvSpeed = DTA_PHY_SPEED_NOT_SET;
    pPhyMac->m_NwDrvLink = DTA_PHY_LINK_NOT_SET;

    // MAC init state
    // Initialisation of the MAC state could be removed here because it's excecuted also
    // in the power up code. Just to be sure...
    DtaMacInitState(pPhyMac);

    // Initialise callbacks
    DtFastMutexInit(&pPhyMac->m_Callbacks.m_FastMutex);
    pPhyMac->m_Callbacks.m_Registered = FALSE;
    pPhyMac->m_Callbacks.m_LinkStatusCallback = NULL;

    DtFastMutexInit(&pPhyMac->m_FastMutexCounters);

    if (DT_SUCCESS(Status))
    {
        // Initialize register offsets
        pPhyMac->m_MacRegsOffset = DtPropertiesGetUInt16(pPropData, "NW_REGISTERS_MAC",
                                                                    pIpPort->m_PortIndex);

        // Check if no property error occurred
        Status = DtaPropertiesReportDriverErrors(pIpPort->m_pDvcData);
    }

    if (DT_SUCCESS(Status))
        // Initialise Current and Permanent MAC address
        Status = DtaMacInitMacAddress(pPhyMac);

    if (DT_SUCCESS(Status))
        // Initialise Dpc for phy interrupt handling
        Status = DtDpcInit(&pPhyMac->m_PhyIntDpc, DtaPhyInterruptDpc, TRUE);
    if (DT_SUCCESS(Status))
        // Initialise Workitem for phy interrupt handling
        Status = DtWorkItemInit(&pPhyMac->m_PhyIntWorkItem, DtaPhyInterruptWi, TRUE,
                                                          &pPhyMac->m_pDvcData->m_Device);
    
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhyMacPowerUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaPhyMacPowerup(PhyMac* pPhyMac)
{
    DtStatus  Status = DT_STATUS_OK;

    // Reset internal state so all initialisation code is executed also after 
    // powerup from sleep/hibernate
    DtaMacInitState(pPhyMac);

    // Recalculate registers
    pPhyMac->m_pMacRegs = pPhyMac->m_pDvcData->m_pGenRegs + pPhyMac->m_MacRegsOffset;
    pPhyMac->m_pGenNwRegs = pPhyMac->m_pDvcData->m_pGenRegs + pPhyMac->m_GenNwRegsOffset;

    if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
        DtaMacAInit(pPhyMac);
    else
        DtaMacCInit(pPhyMac);

    // Update MAC address in firmware
    if (pPhyMac->m_MacType == MAC_TYPE_ALTERA)
        DtaMacAMacAddressSet(pPhyMac->m_pMacRegs, pPhyMac->m_MacAddrCur);
    else
        DtaMacCMacAddressSet(pPhyMac->m_pMacRegs, pPhyMac->m_MacAddrCur,
                                                                     HAS_MACBUG(pPhyMac));
    
    // Init phy
    Status = DtaInitPhy(pPhyMac);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPhyMacPowerdown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaPhyMacPowerdown(PhyMac* pPhyMac)
{
    // Wait for DPC's and workitems to be completed
    DtDpcWaitForCompletion(&pPhyMac->m_PhyIntDpc);
    DtWorkItemWaitForCompletion(&pPhyMac->m_PhyIntWorkItem);

    // Set link valid to 0. This is needed for the DTA-2162 to disable the leds on the phy
    DtaNwCtrlSetLinkValid(pPhyMac->m_pGenNwRegs, 0);

    // Disable phy
    DtaSetPhyRegister(pPhyMac, DTA_PHY_ADDRESS_BMCR, DTA_PHY_BMCR_POWER_DOWN_MASK);
    return DT_STATUS_OK;
}

