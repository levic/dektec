//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCorePcie_DEVICE.c *#*#*#*#*#*#*#*#*# (C) 2017 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017 DekTec Digital Video B.V.
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
#include "DtPcieIncludes.h"            // Standard driver includes

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCorePcie_DEVICE implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtCorePcie_DEVICE_OpenChildren(DtCore*);
static void  DtCorePcie_DEVICE_CloseChildren(DtCore*);
static DtStatus  DtCorePcie_DEVICE_ParsePciConfig(DtCorePcie* pCore, UInt8* pPciConfig);
static UInt32  DtCorePcie_DEVICE_RegRead32(DtCore*, UInt32  ByteOffset);
static void  DtCorePcie_DEVICE_RegWrite32(DtCore*, UInt32  ByteOffset, UInt32  Value);
static DtStatus  DtCorePcie_DEVICE_BulkRead32(DtCore*, UInt32  Offset, Int NumToRead,
                                                                        UInt32*  pBuffer);

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtCorePcie_DEVICE - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the device is found in the system by the bus
// driver. Typenumber cannot be determined yet (for uninitialized PLX devices).
// This function therefore cannot perform device type specific actions. The device 
// hardware will be initialized later and is not mapped yet, only PCI config space can be
// accessed (but this is already done in the IAL)!
//
DtStatus  DtCorePcie_DEVICE_Init(DtCorePcie* pCore)
{
    DtStatus  Status;
    
    DtDbgOut(MAX, CORE, "Start");
    
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    DtDbgOut(MIN, CORE, "Found: device 0x%x, vendor 0x%x, subsystemdevice 0x%x, "
                        "subsystemvendor 0x%x", pCore->m_DevInfo.m_DeviceId,
                        pCore->m_DevInfo.m_VendorId, pCore->m_DevInfo.m_SubSystemId,
                        pCore->m_DevInfo.m_SubVendorId);

    // Type number is derived from DeviceId
    pCore->m_DevInfo.m_TypeNumber = DtPcieDeviceId2TypeNumber(
                                                             pCore->m_DevInfo.m_DeviceId);
    // Sub type number is derived from DeviceId
    pCore->m_DevInfo.m_SubType = DtPcieDeviceId2SubType(pCore->m_DevInfo.m_DeviceId);

    // Subdevices are not yet supported
    pCore->m_DevInfo.m_SubDvc =  0;

    // Perform base initialisation
    Status = DtCore_DEVICE_Init((DtCore*)pCore, DtIoStubCorePcie_Open);
    if (!DT_SUCCESS(Status))
        return Status;

    // The first powerup is special to initialize resources that need the hardware
    pCore->m_IntEnableState = INT_DISABLED; // Not enabled yet

    // Initialize Exclusive access fast mutex
    DtFastMutexInit(&pCore->m_ExclAccessMutex);
    
    DtDbgOut(MAX, CORE, "Exit (device: DTA-%d)", pCore->m_DevInfo.m_TypeNumber);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_PowerUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device must enter the active (D0) state. It should
// wake up the corresponding device and initialize it into active state. It should restore
// any saved settings. These settings can be saved during power down. This function can be
// executed in two different scenarios.
//
DtStatus  DtCorePcie_DEVICE_PowerUp(DtCorePcie* pCore)
{
    DtStatus  Status;
        
    DtDbgOut(MAX, CORE, "Start");

    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    // Initialize registers pointer, before calling base implementtaion
    pCore->m_pRegBase = (UInt8*)pCore->m_DtPcieRegs.m_pKernel;

    // Register call-backs for common interface
    pCore->m_RegRead32Func = DtCorePcie_DEVICE_RegRead32;
    pCore->m_RegWrite32Func = DtCorePcie_DEVICE_RegWrite32;
    pCore->m_BulkRead32Func = DtCorePcie_DEVICE_BulkRead32;
    pCore->m_OpenChildrenFunc = DtCorePcie_DEVICE_OpenChildren;
    pCore->m_CloseChildrenFunc = DtCorePcie_DEVICE_CloseChildren;
    
    // Let base class do the rest
    Status = DtCore_DEVICE_PowerUp((DtCore*)pCore);
    if (!DT_SUCCESS(Status))
        return Status;

    DtDbgOut(MAX, CORE, "Exit");

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_InterruptEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device interrupts must be enabled.
// This is done between PowerUp and PowerUpPost
//
DtStatus  DtCorePcie_DEVICE_InterruptEnable(DtCorePcie* pCore)
{
    DtStatus   Status=DT_STATUS_OK;
    DtCfInt*  pCfInt = NULL;

    DtDbgOut(MAX, CORE, "Start");

    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);
    pCfInt = pCore->m_pCfInt;

    // Enable interrupts through enabling the INTERRUPT-CF
    if (pCfInt == NULL)
    {
        DtDbgOut(ERR, CORE, "ERROR: no interrupt-CF => cannot enable interrupts");
        return DT_STATUS_FAIL;
    }
    DT_ASSERT(pCfInt->m_EnableFunc != NULL);
    Status = pCfInt->m_EnableFunc((DtDf*)pCfInt, TRUE);
    if (DT_SUCCESS(Status))
    {
        // Interrupts are up
        pCore->m_IntEnableState = INT_ENABLED;
    }
    DtDbgOut(MAX, CORE, "Exit");

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_PowerUpPost -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device must enter the active (D0) state after PowerUp
// is called and the interrupts are enabled. It should only perform actions to put the
// device in power up mode that need the interrupts to be enabled.
//
DtStatus  DtCorePcie_DEVICE_PowerUpPost(DtCorePcie* pCore)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  PciConfigData[256];

    DtDbgOut(MAX, CORE, "Start");

    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    //-.-.-.-.-.-.- Extract PCIe express properties from configuartion space -.-.-.-.-.-.-

    Status = DtReadConfigSpace(&pCore->m_Device, 0, sizeof(PciConfigData), PciConfigData);
    if (DT_SUCCESS(Status))
    {
        Status = DtCorePcie_DEVICE_ParsePciConfig(pCore, PciConfigData);
        if (!DT_SUCCESS(Status))
            DtDbgOut(ERR, CORE, "Failed parsing PCI configuration space (0x%X)", Status);
    }
    else
        DtDbgOut(ERR, CORE, "Failed reading PCI configuration space (0x%X)", Status);

    // Let base class do the rest
    Status = DtCore_DEVICE_PowerUpPost((DtCore*)pCore);
    if (!DT_SUCCESS(Status))
        return Status;
    
    DtDbgOut(MAX, CORE, "Exit");

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_PowerDownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called when the device must leave the active (D0) state before the
// interrupts are disabled. It should only perform actions to put the device in power
// down mode that need the interrupts to be enabled.
//
DtStatus  DtCorePcie_DEVICE_PowerDownPre(DtCorePcie* pCore)
{
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    // Call base implementation
    return DtCore_DEVICE_PowerDownPre((DtCore*)pCore);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_InterruptDisable -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called when the device interrupts must be disabled. This is done
// Between DevicePowerDownPre and DevicePowerDown.
//
DtStatus  DtCorePcie_DEVICE_InterruptDisable(DtCorePcie* pCore)
{
    DtStatus   Status=DT_STATUS_OK;
    DtCfInt*  pCfInt = NULL;

    DtDbgOut(MAX, CORE, "Start");

    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);
    pCfInt = pCore->m_pCfInt;

    // Diasble interrupts through enabling the INTERRUPT-CF
    if (pCfInt == NULL)
    {
        DtDbgOut(ERR, CORE, "ERROR: no interrupt-CF => cannot disable interrupts");
        return DT_STATUS_FAIL;
    }
    DT_ASSERT(pCfInt->m_EnableFunc != NULL);
    Status = pCfInt->m_EnableFunc((DtDf*)pCfInt, FALSE);
    // Interrupts are down
    pCore->m_IntEnableState = INT_DISABLED;
    DtDbgOut(MAX, CORE, "Exit");

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_PowerDown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called when the device must leave the active (D0) state. This
// function is called after PowerDownPre is called and the interrupts are disabled. It
// should stop receive / transmit threads, store the device state and put the device in
// low power mode.
//
DtStatus  DtCorePcie_DEVICE_PowerDown(DtCorePcie* pCore)
{
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    // Call base implementation
    return DtCore_DEVICE_PowerDown((DtCore*)pCore);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_ExitPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is executed before the device is removed from the system. All objects
// must not be used anymore and set in the idle/done state. All active transactions must
// be stopped.
//
DtStatus  DtCorePcie_DEVICE_ExitPre(DtCorePcie* pCore)
{
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    // Call base implementation
    return DtCore_DEVICE_ExitPre((DtCore*)pCore);;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_Exit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the device is being disconnected. This
// function should undo everything done by DeviceInitPost and DeviceInit.
//
void  DtCorePcie_DEVICE_Exit(DtCorePcie* pCore)
{   
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    // Call base implementation
    DtCore_DEVICE_Exit((DtCore*)pCore);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_PowerUpDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCorePcie_DEVICE_PowerUpDevice(DtCorePcie* pCore)
{   
    DtStatus  Status = DT_STATUS_OK;
    // keep track of last encountered error
    DtStatus  StatusPu = DT_STATUS_OK;

    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    Status = DtCorePcie_DEVICE_PowerUp(pCore);
    if (!DT_SUCCESS(Status))
    {   
        DtDbgOut(ERR, CORE, "Error executing DtCorePcie_DEVICE_PowerUp");
        StatusPu = Status;     
    }
    
    Status = DtCorePcie_DEVICE_InterruptEnable(pCore);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, CORE, "Error executing DtCorePcie_DEVICE_InterruptEnable");
        StatusPu = Status;
    }
  
    Status = DtCorePcie_DEVICE_PowerUpPost(pCore);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, CORE, "Error executing DtCorePcie_DEVICE_PowerUpPost");
        StatusPu = Status;
    }
    return StatusPu;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_Interrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtCorePcie_DEVICE_Interrupt(DtCorePcie*  pCore, Int  MessageId)
{
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    // Check we have an INT-CF
    if (pCore->m_pCfInt == NULL)
    {
        DtDbgOut(ERR, CORE, "ERROR: received interrupt (0x%08X) while INT-CF is "
                                                                "unavailable", MessageId);
        return FALSE;
    }
    return DtCfInt_OnInterrupt(pCore->m_pCfInt);
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtCorePcie_DEVICE - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCorePcie_DEVICE_OpenChildren(DtCore*  pCore)
{
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);
    // FOR NOW: NOTHING SPECIAL TODO => LET BASE CLASS DO THE HEAVY LIFTING
    return DtCore_OpenChildren(pCore);

}
//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_CloseChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtCorePcie_DEVICE_CloseChildren(DtCore*  pCore)
{
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);
    // FOR NOW: NOTHING SPECIAL TODO => LET BASE CLASS DO THE HEAVY LIFTING
    DtCore_CloseChildren(pCore);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_ParsePciConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCorePcie_DEVICE_ParsePciConfig(DtCorePcie* pCore, UInt8* pPciConfig)
{
    Bool  Visited[256];
    Int  i;
    UInt8  NextCapAddr=pPciConfig[0x34]&0xFC, CapId=0;
    UInt8*  pCapBuf;

    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    // Make sure all fields are initialized.
    pCore->m_DevInfo.m_PcieNumLanes = -1;
    pCore->m_DevInfo.m_PcieMaxLanes = -1;
    pCore->m_DevInfo.m_PcieLinkSpeed = -1;
    pCore->m_DevInfo.m_PcieMaxSpeed = -1;
    pCore->m_DevInfo.m_PcieMaxPayloadSize = -1;
    pCore->m_DevInfo.m_PcieMaxReadRequestSize = -1;


    // We keep track of which bytes we've used as start of a capability struct to make
    // sure we don't end in an infinite loop. 
    for (i=0; i<256; i++)
        Visited[i] = (i<64) ? TRUE : FALSE;

    if ((pPciConfig[6]&0x10) == 0)
    {
        DtDbgOut(ERR, CORE, "New capabilities list not available in PCI config");
        return DT_STATUS_OK;
    }

    while (!Visited[NextCapAddr])
    {
        Visited[NextCapAddr] = TRUE;
        pCapBuf = &pPciConfig[NextCapAddr];
        NextCapAddr = pCapBuf[1]&0xFC;
        CapId = pCapBuf[0];
        switch (CapId)
        {
        case 0x10: // PCI Express Capability structure
            pCore->m_DevInfo.m_PcieNumLanes = 
                                            (pCapBuf[0x12]>>4) | ((pCapBuf[0x13]&0x3)<<4);
            pCore->m_DevInfo.m_PcieLinkSpeed = pCapBuf[0x12]&0xF;

            pCore->m_DevInfo.m_PcieMaxLanes = 
                                            (pCapBuf[0x0C]>>4) | ((pCapBuf[0x0D]&0x3)<<4);
            pCore->m_DevInfo.m_PcieMaxSpeed = pCapBuf[0x0C]&0xF;

            // Get MPS from Device-Control-Register (bits 7..5)
            switch ((pCapBuf[0x8]>>5)&0x7)
            {
            case 0:   pCore->m_DevInfo.m_PcieMaxPayloadSize = 128; break;
            case 1:   pCore->m_DevInfo.m_PcieMaxPayloadSize = 256; break;
            case 2:   pCore->m_DevInfo.m_PcieMaxPayloadSize = 512; break;
            case 3:   pCore->m_DevInfo.m_PcieMaxPayloadSize = 1024; break;
            case 4:   pCore->m_DevInfo.m_PcieMaxPayloadSize = 2048; break;
            case 5:   pCore->m_DevInfo.m_PcieMaxPayloadSize = 4096; break;
            default:  pCore->m_DevInfo.m_PcieMaxPayloadSize = -1; break;
            }
            // Get MRRS from Device-Control-Register (bits 14..12)
            switch ((pCapBuf[0x9]>>4)&0x7)
            {
            case 0:   pCore->m_DevInfo.m_PcieMaxReadRequestSize = 128; break;
            case 1:   pCore->m_DevInfo.m_PcieMaxReadRequestSize = 256; break;
            case 2:   pCore->m_DevInfo.m_PcieMaxReadRequestSize = 512; break;
            case 3:   pCore->m_DevInfo.m_PcieMaxReadRequestSize = 1024; break;
            case 4:   pCore->m_DevInfo.m_PcieMaxReadRequestSize = 2048; break;
            case 5:   pCore->m_DevInfo.m_PcieMaxReadRequestSize = 4096; break;
            default:  pCore->m_DevInfo.m_PcieMaxReadRequestSize = -1; break;
            }
            
            DtDbgOut(AVG, CORE, "Num/Max lanes: %d / %d",
                        pCore->m_DevInfo.m_PcieNumLanes, pCore->m_DevInfo.m_PcieMaxLanes);
            DtDbgOut(AVG, CORE, "Cur/Max speed: %d / %d",
                       pCore->m_DevInfo.m_PcieLinkSpeed, pCore->m_DevInfo.m_PcieMaxSpeed);
            DtDbgOut(AVG, CORE, "MPS=%d-bytes, MRRS=%d-bytes",
                                               pCore->m_DevInfo.m_PcieMaxPayloadSize, 
                                               pCore->m_DevInfo.m_PcieMaxReadRequestSize);

            break;
        }
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_RegRead32 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt32  DtCorePcie_DEVICE_RegRead32(DtCore*  pCore, UInt32  ByteOffset)
{
    volatile UInt8*  pRegBase = NULL;
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);
    if ((ByteOffset%4)!=0 || ByteOffset>=((DtCorePcie*)pCore)->m_DtPcieRegs.m_Length)
    {
        DT_ASSERT(FALSE);
        return 0;
    }
    pRegBase = ((DtCorePcie*)pCore)->m_pRegBase;
    DT_ASSERT(pRegBase != NULL);
    return *(volatile UInt32*)(pRegBase+ByteOffset);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_RegWrite32 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtCorePcie_DEVICE_RegWrite32(DtCore*  pCore, UInt32  ByteOffset, UInt32  Value)
{
    volatile UInt8*  pRegBase = NULL;

    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);
    if ((ByteOffset%4)!=0 || ByteOffset>=((DtCorePcie*)pCore)->m_DtPcieRegs.m_Length)
    {
        DT_ASSERT(FALSE);
        return;
    }
    pRegBase = ((DtCorePcie*)pCore)->m_pRegBase;
    DT_ASSERT(pRegBase != NULL);
    *(volatile UInt32*)(pRegBase+ByteOffset) = Value;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DEVICE_BulkRead32 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtCorePcie_DEVICE_BulkRead32(DtCore* pCore, UInt32 Offset, Int NumToRead, 
                                                                         UInt32* pBuffer)
{
    Int i;
    UInt64*  pSrc64 = NULL;
    UInt64*  pDest64 = (UInt64*)pBuffer;
    volatile UInt8*  pRegBase = NULL;
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    // Parameter check
    if ((Offset%8)!=0 || NumToRead<0 || (NumToRead%2)!=0 || pBuffer==NULL 
                                                                || ((UInt64)pBuffer)%8!=0)
        return DT_STATUS_INVALID_PARAMETER;

    if ((Offset+NumToRead*sizeof(UInt32)) > ((DtCorePcie*)pCore)->m_DtPcieRegs.m_Length)
        return DT_STATUS_INVALID_PARAMETER;

    pRegBase = ((DtCorePcie*)pCore)->m_pRegBase;
    DT_ASSERT(pRegBase != NULL);
    pSrc64 = (UInt64*)(pRegBase+Offset);

    // Bulk read
    for (i=NumToRead/2; i>0; i--)
        *pDest64++ = *pSrc64++;
    return DT_STATUS_OK;
}