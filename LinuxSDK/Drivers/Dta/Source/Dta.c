//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dta.c *#*#*#*#*#*#*#*#*#*# (C) 2010-2013 DekTec
//
// Dta driver - Interface for the Dta common driver, used by the IAL.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2013 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#include <DtaIncludes.h>            // Standard driver includes

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Forward declarations
static void  DtaGeneralPeriodicIntDpc(DtDpcArgs* pArgs);
static DtStatus  DtaGetAddressRegs(DtaDeviceData* pDvcData, DtFileObject* pFile,
            Int PortIndex, Int RegsType, void* pPaMmap, Int* pSize, void** ppUserVirtual);
static DtStatus  DtaWaitUntilFpgaIsReady(DtaDeviceData* pDvcData);
static DtStatus  DtaInitUserMapping(DtaDeviceData* pDvcData);
static DtStatus  DtaInterruptSofIntHandler(DtaDeviceData* pDvcData);
static DtStatus  DtaInterruptLastFrameIntHandler(DtaNonIpPort*  pNonIpPort);
static DtStatus  DtaReleaseAddressRegs(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                Int PortIndex, Int RegsType, void** ppPaMmap, Int* pSize);

DtStatus  DtaCalculateAndCreatePortsStruct(DtaDeviceData* pDvcData);
void  DtaCleanupPortStructs(DtaDeviceData* pDvcData);
DtStatus  DtaDeviceGenlockIoctl(DtaDeviceData* pDvcData, Int* pState, Int* pRefVidStd);
DtStatus  DtaGetAddressRegsForUserspace(DtaDeviceData* pDvcData, DtFileObject* pFile,
                           Int PortIndex, void* pPaMap, Int* pSize, void** ppUserVirtual);
void  DtaPortsCleanUp(DtaDeviceData* pDvcData);
DtStatus  DtaPortsCleanUpPre(DtaDeviceData* pDvcData);
DtStatus  DtaPortsInit(DtaDeviceData* pDvcData);
DtStatus  DtaPowerupPorts(DtaDeviceData* pDvcData);
DtStatus  DtaPowerdownPorts(DtaDeviceData* pDvcData);
DtStatus  DtaPowerdownPortsPre(DtaDeviceData* pDvcData);
DtStatus  DtaRebootFirmware(DtaDeviceData* pDvcData);
DtStatus  DtaReleaseAddressRegsForUserspace(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                               Int PortIndex, void** ppPaMap, Int* pSize);
DtStatus  DtaSetMemoryTestMode(DtaDeviceData* pDvcData, Bool On);

// Imported functions
DtStatus  DtaInitChildDevices(DtaDeviceData* pDvcData);
DtStatus  DtaRemoveChildDevices(DtaDeviceData* pDvcData);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDriverInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL and initializes the common (host independent) part
// of the driver.
//
DtStatus  DtaDriverInit(DtEvtLog* pEvtObject)
{
    DtStatus  Status = DT_STATUS_OK;
    DtString  DtStrVersion;
    DtStringChar  DtStrVersionBuffer[32];
    DT_STRING_DECL(DtStrDot, ".");

    // Connect DtStrVersionBuffer to DtStrVersion
    DT_STRING_INIT(DtStrVersion, DtStrVersionBuffer, 32);

    // Create driver version string
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTA_VERSION_MAJOR);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTA_VERSION_MINOR);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTA_VERSION_MICRO);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTA_VERSION_BUILD);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Finally report the event
    DtEvtLogReport(pEvtObject, DTA_LOG_DRIVER_LOADED, &DtStrVersion, NULL, NULL);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDriverExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL when the driver is unloaded. This function should
// undo everything done by DtaDriverInit.
//
void  DtaDriverExit()
{
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL when the device is found in the system by the bus
// driver. Typenumber cannot be determined yet (for uninitialized PLX devices). This
// function therefore cannot perform device type specific actions.
// The device hardware will be initialized later and is not mapped yet, only PCI config
// space can be accessed (but this is already done in the IAL)!
//
DtStatus  DtaDeviceInit(DtaDeviceData* pDvcData)
{
    DtStatus  Status;
    
    DtDbgOut(MAX, DTA, "Start");
    
    DtDbgOut(MIN, DTA, "Found: device 0x%x, vendor 0x%x, subsystemdevice 0x%x, "
                        "subsystemvendor 0x%x", pDvcData->m_DevInfo.m_DeviceId,
                        pDvcData->m_DevInfo.m_VendorId, pDvcData->m_DevInfo.m_SubSystemId,
                        pDvcData->m_DevInfo.m_SubVendorId);

    // The first powerup is special to initialize resources that need the hardware
    pDvcData->m_InitialPowerup = TRUE;
    pDvcData->m_IntEnableState = INT_DISABLED; // Not enabled yet

    // Initialize general periodic interrupt DPC
    Status = DtDpcInit(&pDvcData->m_GenPerIntDpc, DtaGeneralPeriodicIntDpc, TRUE);
    if (!DT_SUCCESS(Status))
        return Status;

    // Initialize file handle info mutex
    DtFastMutexInit(&pDvcData->m_FileHandleInfoMutex);
    // Initialize file handle info
    pDvcData->m_pFileHandleInfo = NULL;
    
    // Initialize Exclusive access fast mutex
    DtFastMutexInit(&pDvcData->m_ExclAccessMutex);

    // Initialize events
    DtaEventsInit(pDvcData);

    // Determine whether this PCI card uses a PLX bridge chip (PCI9054/9056)
    pDvcData->m_DevInfo.m_UsesPlxChip = (pDvcData->m_DevInfo.m_VendorId == 0x10B5);
    if (pDvcData->m_DevInfo.m_UsesPlxChip)
    {
        pDvcData->m_DevInfo.m_Uses9056 = (pDvcData->m_DevInfo.m_DeviceId == 0x9056);
        pDvcData->m_DevInfo.m_HasUninitializedPlx = (pDvcData->m_DevInfo.m_SubVendorId ==
                                                                                  0x10B5);
    }

    if (!pDvcData->m_DevInfo.m_UsesPlxChip || !pDvcData->m_DevInfo.m_HasUninitializedPlx)
    {
        // Type number is derived from SubSystemId for cards with a PLX chip and 
        // from DeviceId for new cards
        // ATTENTION: This typenumber can be wrong in case of an uninitialised EEPROM,
        //            this will be fixed in the uninitialized EEPROM handler.
        pDvcData->m_DevInfo.m_TypeNumber = DtaSubsystemId2TypeNumber(
                                                pDvcData->m_DevInfo.m_UsesPlxChip ?
                                                pDvcData->m_DevInfo.m_SubSystemId :
                                                (pDvcData->m_DevInfo.m_DeviceId & 0xFFF));

        // Deduce sub-device number
        pDvcData->m_DevInfo.m_SubDvc = DtaDeviceId2SubDvcNumber(
                                                         pDvcData->m_DevInfo.m_TypeNumber,
                                                          pDvcData->m_DevInfo.m_DeviceId);

        // Initialize property data object
        pDvcData->m_PropData.m_pPropertyStore = NULL;
        pDvcData->m_PropData.m_PropertyNotFoundCounter = 0;
        //pDvcData->m_PropData.m_PropertyNotFoundString = ?;
        pDvcData->m_PropData.m_pTableStore = NULL;
        pDvcData->m_PropData.m_TypeName = "DTA";
        pDvcData->m_PropData.m_TypeNumber = pDvcData->m_DevInfo.m_TypeNumber;
        pDvcData->m_PropData.m_SubDvc = pDvcData->m_DevInfo.m_SubDvc;
        pDvcData->m_PropData.m_FirmwareVersion = pDvcData->m_DevInfo.m_FirmwareVersion;
        pDvcData->m_PropData.m_HardwareRevision = pDvcData->m_DevInfo.m_HardwareRevision;

        // Initialise the property store
        Status = DtaPropertiesInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

        // Initialise the number and type of ports the card supports. 
        Status = DtaCalculateAndCreatePortsStruct(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

        // Initialise the table store
        Status = DtTablesInit(&pDvcData->m_PropData);
        if (!DT_SUCCESS(Status))
            return Status;

        if (pDvcData->m_NumIpPorts != 0) 
        {
            // Create the network devices
            Status = DtaInitChildDevices(pDvcData);
            if (!DT_SUCCESS(Status))
                return Status;
        }
    }

    DtDbgOut(MAX, DTA, "Exit (device: DTA-%d)", pDvcData->m_DevInfo.m_TypeNumber);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceInitPci905X -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL for devices with a PLX chip after BAR 0 is mapped,
// but before BAR 2 is mapped.
// In principle, initialising the PCI-9054 or PCI-9056 is required only if the serial
// EEPROM has not been programmed yet.
// However, due to a bug in the PCI-9054 chip (see Errata #9 in 9054ABerrata-16.pdf),
// the serial EEPROM is not reloaded when the Power Management State is changed from
// D3hot to D0. This is a problem e.g. when disabling and enabling a device driver.
// This function should therefore always initialise the PLX chip.
//
// REGISTER VALUES MUST MATCH VALUES PROGRAMMED INTO THE SERIAL EEPROM!
//
DtStatus  DtaDeviceInitPci905X(DtaDeviceData* pDvcData)
{
    DtStatus  Status;
    UInt16  RegsAddrMask;
    Int  DmaPrio;
    UInt32  LAS0RR;
    UInt32  MARBR;

    // Get PCI-9054/9056 registers
    volatile UInt8*  p905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;

    // LAS0RR
    if (!pDvcData->m_DevInfo.m_HasUninitializedPlx)
    {
        // Get HARDWARE_REGS_SIZE value
        RegsAddrMask = DtPropertiesGetUInt16(pPropData, "PLX_REGS_ADDR_MASK", -1);
        // Get HARDWARE_DMA_PRIO value
        DmaPrio = DtPropertiesGetInt(pPropData, "PLX_DMA_PRIO", -1);
        // Check if no property errors occurred
        Status = DtaPropertiesReportDriverErrors(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

        // Write LAS0RR
        LAS0RR = (UInt32)RegsAddrMask;
        LAS0RR = ~LAS0RR & 0xFFFFFFF0;
        WRITE_UINT32(p905XRegs, PCI905X_SPACE0_RANGE, LAS0RR);

        // Write MARBR
        MARBR = 0x02000000;
        switch (DmaPrio)
        {
            case DMA_PRIO_ROTATING: break;
            case DMA_PRIO_CHAN0:    MARBR |= 0x00080000; break;
            case DMA_PRIO_CHAN1:    MARBR |= 0x00100000; break;
            default: return DT_STATUS_CONFIG_ERROR;
        }
        WRITE_UINT32(p905XRegs, PCI905X_LOCAL_DMA_ARBIT, MARBR);
    } else {
        // Write default 0xFFFFF000?
        //WRITE_UINT32(p905XRegs, PCI905X_SPACE0_RANGE, 0xFFFFF000);

        // Dma priority channel 0 (DTA-124 firmware version == 0)
        WRITE_UINT32(p905XRegs, PCI905X_LOCAL_DMA_ARBIT, 0x02080000);
    }

    WRITE_UINT32(p905XRegs, PCI905X_SPACE0_REMAP, 1);               // LAS0BA
    WRITE_UINT8(p905XRegs, PCI905X_ENDIAN_DESC, 0);                 // BIGEND
    WRITE_UINT8(p905XRegs, PCI905X_LOCAL_MISC, 0x04);               // LMISC
    WRITE_UINT16(p905XRegs, PCI905X_PROT_AREA, 0x007F);             // PROT_AREA
    WRITE_UINT32(p905XRegs, PCI905X_EXP_ROM_RANGE, 0x0000);         // EROMRR
    WRITE_UINT32(p905XRegs, PCI905X_EXP_ROM_REMAP, 0x0000);         // EROMBA
//  WRITE_UINT32(p905XRegs, PCI905X_SPACE0_ROM_DESC, 0x43430143);   // LBRD0
    WRITE_UINT32(p905XRegs, PCI905X_SPACE0_ROM_DESC, 0xF3430143);   // LBRD0

    WRITE_UINT32(p905XRegs, PCI905X_DM_RANGE, 0);                   // DMRR
    WRITE_UINT32(p905XRegs, PCI905X_DM_MEM_BASE, 0);                // DMLBAM
    WRITE_UINT32(p905XRegs, PCI905X_DM_IO_BASE, 0);                 // DMLBAI
    WRITE_UINT32(p905XRegs, PCI905X_DM_PCI_MEM_REMAP, 0);           // DMPBAM
    WRITE_UINT32(p905XRegs, PCI905X_DM_PCI_IO_CONFIG, 0);           // DMCFGA

    WRITE_UINT32(p905XRegs, PCI905X_SPACE1_RANGE, 0);               // LAS1RR
    WRITE_UINT32(p905XRegs, PCI905X_SPACE1_REMAP, 0);               // LAS1BA
    WRITE_UINT32(p905XRegs, PCI905X_SPACE1_DESC, 0x03430000);       // LBRD1

    if (pDvcData->m_DevInfo.m_Uses9056)
    {
        UInt32  CntrlReg;

        // Set general purpose output if PCI9056 to '1'
        CntrlReg = READ_UINT32(p905XRegs, PCI905X_EEPROM_CTRL_STAT);
        CntrlReg |= PCI905X_CNTRL_GENPURPOUTP;
        WRITE_UINT32(p905XRegs, PCI905X_EEPROM_CTRL_STAT, CntrlReg);
    }

    // For devices with a PLX set the DMA read and write command
    // Write: B"0111" => PCI Write Command
    // Read:  B"1100" => PCI Memory Read Multiple Command
    WRITE_UINT8(p905XRegs, PCI905X_EEPROM_CTRL_STAT, 0x7C);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDevicePowerUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device must enter the active (D0) state. It should
// wake up the corresponding device and initialize it into active state. It should restore
// any saved settings. These settings can be saved during power down.
// This function can be executed in two different scenarios. The first one is during the
// initial powerup. In this case it checks whether the device is supported and allocates
// all the software resources, for example continuous memory buffers for DMA and DPC
// objects. The second scenario is the powerup after powerdown. In this case the
// resources are already allocated. The device hardware only needs to be initialized.
// Since we don't support streams to resume after a powerdown, the channel HW doesn't
// have to be initialized, but the ethernet for example must be re-inintialized with the
// same setttings (like multicast list) that it was using before the power down.
// It is possible that resources have changed during powerdown, therefore the register
// mapping to userspace must be destroyed during powerdown. Also the register pointers
// in the driver must be reinitialized durint the powerup.
//
DtStatus  DtaDevicePowerUp(DtaDeviceData* pDvcData)
{
    char  Buffer[64];
    UInt  BufLen = sizeof(Buffer);
    Int64  ForcedHardwareRevision;
    UInt8  LatencyTimer;
    DtStatus  Status;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;

    DtDbgOut(MAX, DTA, "Start");

    // Make sure property not found counter is reset after a power resume
    DtResetPropertiesNotFoundCounter(&pDvcData->m_PropData);

    // Initialize general registers pointer
    pDvcData->m_pGenRegs = (UInt8*)pDvcData->m_DtaRegs.m_pKernel;
    
    if (pDvcData->m_InitialPowerup)
    {
        // Fix typenumber in case of uninitialised PLX EEPROM
        // And finally initialize the property storage
        if (pDvcData->m_DevInfo.m_HasUninitializedPlx)
        {
            // Make sure FPGA is alive
            DtSleep(1000);
            // Fix typenumber
            pDvcData->m_DevInfo.m_TypeNumber = DtaRegGenCtrlGetTypeNumber(
                                                                    pDvcData->m_pGenRegs);
            
            // Initialize property data object
            pDvcData->m_PropData.m_pPropertyStore = NULL;
            pDvcData->m_PropData.m_PropertyNotFoundCounter = 0;
            //pDvcData->m_PropData.m_PropertyNotFoundString = ?;
            pDvcData->m_PropData.m_pTableStore = NULL;
            pDvcData->m_PropData.m_TypeName = "DTA";
            pDvcData->m_PropData.m_TypeNumber = pDvcData->m_DevInfo.m_TypeNumber;
            pDvcData->m_PropData.m_FirmwareVersion = pDvcData->m_DevInfo.m_FirmwareVersion;
            pDvcData->m_PropData.m_HardwareRevision = pDvcData->m_DevInfo.m_HardwareRevision;

            // Initialise the property store
            Status = DtaPropertiesInit(pDvcData);
            if (!DT_SUCCESS(Status))
                return Status;
            
            // Initialise the number and type of ports the card supports. 
            Status = DtaCalculateAndCreatePortsStruct(pDvcData);
            if (!DT_SUCCESS(Status))
                return Status;

            // Initialise the table store
            Status = DtTablesInit(&pDvcData->m_PropData);
            if (!DT_SUCCESS(Status))
                return Status;

            if (pDvcData->m_NumIpPorts != 0) 
            {
                // Create the network devices
                Status = DtaInitChildDevices(pDvcData);
                if (!DT_SUCCESS(Status))
                    return Status;
            }
        }
    }
                
    // Wait until FPGA is ready
    if (pDvcData->m_DevInfo.m_UsesPlxChip)
        DtaWaitUntilFpgaIsReady(pDvcData);
    
    if (pDvcData->m_InitialPowerup)
    {
        // Get hardware revision
        pDvcData->m_DevInfo.m_HardwareRevision = 0;
        
        // Get firmware version
        pDvcData->m_DevInfo.m_FirmwareVersion = 
                                              DtaRegGenCtrlGetFwRev(pDvcData->m_pGenRegs);
        // Get firmware variant
        pDvcData->m_DevInfo.m_FirmwareVariant = 
                                          DtaRegGenStatGetFwVariant(pDvcData->m_pGenRegs);
        // Update propery data fields
        pDvcData->m_PropData.m_FirmwareVersion = pDvcData->m_DevInfo.m_FirmwareVersion;
        pDvcData->m_PropData.m_HardwareRevision = pDvcData->m_DevInfo.m_HardwareRevision;

        // Fix subvendor/subdevice in case of uninitialised PLX EEPROM
        // This can only be done after the properties are initialized
        if (pDvcData->m_DevInfo.m_SubVendorId == 0x10B5)
        {
            pDvcData->m_DevInfo.m_SubVendorId = DtPropertiesGetUInt16(pPropData,
                                                                 "PCI_SUB_VENDOR_ID", -1);
            pDvcData->m_DevInfo.m_SubSystemId = DtPropertiesGetUInt16(pPropData,
                                                                 "PCI_SUB_SYSTEM_ID", -1);
        }

        // Load reference clock/periodic interrupt properties
        pDvcData->m_DevInfo.m_RefClk = DtPropertiesGetUInt(pPropData, "REF_CLK_FREQ", -1);
        pDvcData->m_DevInfo.m_PerIntClkBit = DtPropertiesGetUInt8(pPropData,
                                                         "INTERRUPT_PER_INT_CLK_BIT", -1);
        pDvcData->m_DevInfo.m_HasPerIntItvReg = DtPropertiesGetBool(pPropData,
                                                     "INTERRUPT_HAS_PER_INT_ITV_REG", -1);
        DT_ASSERT(pDvcData->m_DevInfo.m_PerIntClkBit >= 17);

        pDvcData->m_DevInfo.m_HasExtClockFreq = DtPropertiesGetBool(pPropData,
                                                         "HARDWARE_HAS_EXT_CLK_FREQ", -1);
        
        // Check if no property errors occurred
        Status = DtaPropertiesReportDriverErrors(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

        // Init DMA properties and DMA objects
        Status = DtaDmaInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

        // Compute periodic interrupt interval in micro seconds
        pDvcData->m_DevInfo.m_PerIntItvUS = DtaGetPerIntItvUS(pDvcData);

        // Initialize VPD module
        Status = DtaVpdInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

        // Initialize I2C module
        Status = DtaI2cInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;
        
        // Initialize user mappings
        Status = DtaInitUserMapping(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;
        
        if (pDvcData->m_DevInfo.m_SubDvc > 0)
        {
            // NOTE: we need a unique SN for persistence of IO-configs => since slaves do 
            // not have a VPD with SN we use the board-ID as alternative
            pDvcData->m_DevInfo.m_Serial = DtaRegBoardId(pDvcData->m_pGenRegs);
            // Set MSB bit to make sure board ID does not conflict with a normal SN
            pDvcData->m_DevInfo.m_Serial |= 0x8000000000000000LL;
        } 
        else 
        {
            // Get serial number
            pDvcData->m_DevInfo.m_Serial = -1;
            BufLen = sizeof(Buffer);
            Status = DtaVpdReadItemRo(pDvcData, 2, "SN", Buffer, &BufLen);
            if (DT_SUCCESS(Status))
                pDvcData->m_DevInfo.m_Serial = DtAnsiCharArrayToUInt64(Buffer, BufLen,10);
            else {
                DtDbgOut(ERR, DTA, "Failed to read serial from VPD (0x%x)", Status);
                DtEvtLogReport(&pDvcData->m_Device.m_EvtObject,
                                                 DTA_LOG_SERIAL_FAILED, NULL, NULL, NULL);
            }
        }
                
        // Get hardware revision from VPD "EC" resource
        // Initialize hardware revision to -1, indicating that it is not defined yet
        pDvcData->m_DevInfo.m_HardwareRevision = -1;
        BufLen = sizeof(Buffer);
        Status = DtaVpdReadItemRo(pDvcData, 2, "EC", Buffer, &BufLen);
        if (DT_SUCCESS(Status))
        {
            // VPD "EC" resource can be read.
            Status = DtUtilitiesDeduceHardwareRevision(Buffer, BufLen, 
                                                 &pDvcData->m_DevInfo.m_HardwareRevision); 
            if (DT_SUCCESS(Status))
            {
                DtDbgOut(MIN, DTA, "'EC' is %s, hence deduced hardware rev is %d", 
                                          Buffer, pDvcData->m_DevInfo.m_HardwareRevision);
            } else {
                // VPD "EC" resource is formatted incorrectly
                // Consider this a non-critical error
                pDvcData->m_DevInfo.m_HardwareRevision = 0;
                DtDbgOut(MIN, DTA, "'EC' resouce is formatted incorrectly (%s), "
                                   "hence hardware rev set to 0", Buffer);
            }
        } else {
            // VPD "EC" resource cannot be read, probably because this is a newly 
            // produced card with blank VPD PROM.
            // Check for registry key which defines the hardware revision
            Status = DtNonVolatileManufSettingsRead(&pDvcData->m_Driver, 
                              pDvcData->m_PropData.m_TypeNumber, "ForcedHardwareRevision", 
                                                                 &ForcedHardwareRevision);
            if (DT_SUCCESS(Status))
            {
                pDvcData->m_DevInfo.m_HardwareRevision = (Int)(ForcedHardwareRevision);
                DtDbgOut(MIN, DTA, "Hardware revision set to %d from registry key "
                                   "'ForcedHardwareRevision'", 
                                                  pDvcData->m_DevInfo.m_HardwareRevision);
            } else {
                DtDbgOut(MIN, DTA, "Can't find registry key 'ForcedHardwareRevision'");
            }
        }

        // Verify the hardware revision 
        if (pDvcData->m_DevInfo.m_HardwareRevision < 0)
        {
            // Hardware revision could not be set (no 'EC' resource and no registry key)
            // Consider this a non-critical error
            DtDbgOut(MIN, DTA, "Hardware revision could not be set (no 'EC' resource "
                                "and no registry key 'ForcedHardwareRevision'), "
                                "hence hardware rev set to -1");
            pDvcData->m_DevInfo.m_HardwareRevision = -1;
        }

        pDvcData->m_PropData.m_HardwareRevision = pDvcData->m_DevInfo.m_HardwareRevision;
        
        // *** Set device subtype (-1=undefined, 0=none, 1=A, ...)

        // Initialize subtype to -1, indicating that subtype is not defined yet
        pDvcData->m_DevInfo.m_SubType = -1;

        Status = DtUtilitiesDeriveSubType(&pDvcData->m_PropData, 
                  pDvcData->m_DevInfo.m_HardwareRevision, &pDvcData->m_DevInfo.m_SubType);
        if (DT_SUCCESS(Status))
            DtDbgOut(MIN, DTA, "SubType set to %d", pDvcData->m_DevInfo.m_SubType);
        else
            // Consider this a non-critical error
            DtDbgOut(MIN, DTA, "SubType could not be set");


        // Initialize ports (software)
        Status = DtaPortsInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

        // Initialize matrix API
        Status = DtaMatrixInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

        // Initialize genlock (software)
        Status = DtaGenlockInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;
    }
            
    // Set PCI latency timer, use 64 as default for all devices
    LatencyTimer = 64;
    // PLX devices may have a different latency.
    if (pDvcData->m_DevInfo.m_UsesPlxChip)
    {
        DtResetPropertiesNotFoundCounter(&pDvcData->m_PropData);
        LatencyTimer = DtPropertiesGetUInt8(pPropData, "PLX_LATENCY_TIMER", -1);    
        // Check if no property errors occurred
        Status = DtaPropertiesReportDriverErrors(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    DtDbgOut(MIN, DTA, "Writing PCI latency timer 0x%x", LatencyTimer);
    Status = DtWriteConfigSpace(&pDvcData->m_Device, PCI905X_LATENCY_TIMER, 1, 
                                                                           &LatencyTimer);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTA, "Failed writing PCI latency timer 0x%x (0x%x)", 
                                                                    LatencyTimer, Status);
        return Status;
    }

    // Configure periodic interrupt
    if (pDvcData->m_DevInfo.m_HasPerIntItvReg)
        DtaRegGenCtrlSetPerIntInterval(pDvcData->m_pGenRegs,
                                                 pDvcData->m_DevInfo.m_PerIntClkBit - 17);

    // Initialize DMA (hardware)
    Status = DtaDmaInitPowerup(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Initialize ports (hardware)
    Status = DtaPowerupPorts(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Perform IO configurations
    Status = DtaIoConfigInit(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Initialize I2C
    Status = DtaI2cPowerUp(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Initialise genlock (hardware)
    Status = DtaGenlockInitPowerup(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    // First powerup is done
    pDvcData->m_InitialPowerup = FALSE;

    // Set power up event
    DtaEventsSet(pDvcData, NULL, DTA_EVENT_TYPE_POWER, DTA_EVENT_VALUE1_POWER_UP, 0);

    DtDbgOut(MAX, DTA, "Exit");

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceInterruptEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device interrupts must be enabled. This is done
// Between PowerUp and PowerUpPost.
//
DtStatus  DtaDeviceInterruptEnable(DtaDeviceData* pDvcData)
{
    Int  i;
    DtaNonIpPort*  pNonIpPort = NULL;
    DtaIpPort*  pIpPort = NULL;
    
    // We are about to enable our interrupts
    pDvcData->m_IntEnableState = INT_ENABLING;

    //-.-.-.-.-.-.-.-.-.-.- Step 1:  Enable device-level interrupts -.-.-.-.-.-.-.-.-.-.-.

    // Enable interrupts in PLX905x
    if (pDvcData->m_DevInfo.m_UsesPlxChip) 
    {   volatile UInt8*  p905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;
        UInt32  IntCsr;
        
        // Update the interrupt control register
        IntCsr = READ_UINT32(p905XRegs, PCI905X_INT_CTRL_STAT);
        
        // Always enable the PCI and local interrupt enable
        IntCsr |= PCI905X_INTCSR_PCI_INTEN | PCI905X_INTCSR_LOCAL_INTEN;
        
        DT_ASSERT(DTA_DMA_MAX_PLX_DMA_CONTROLLERS < 3);

        if (pDvcData->m_DmaOptions.m_UseDmaInFpga)
        {
            DT_ASSERT(pDvcData->m_DmaOptions.m_PlxDmaChannelCount > 0);

            // Always enable DMA channel 0 interrupt enable
            IntCsr |= PCI905X_INTCSR_DMA0_INTEN;

            // Enable DMA channel 1 interrupt enable
            if (pDvcData->m_DmaOptions.m_PlxDmaChannelCount > 1)
                IntCsr |= PCI905X_INTCSR_DMA1_INTEN;
        } else {
            if (pDvcData->m_DmaOptions.m_PlxDmaChannelPort[0] != -1)
                // Enable DMA channel 0 interrupt enable
                IntCsr |= PCI905X_INTCSR_DMA0_INTEN;
            if (pDvcData->m_DmaOptions.m_PlxDmaChannelPort[1] != -1)
                // Enable DMA channel 1 interrupt enable
                IntCsr |= PCI905X_INTCSR_DMA1_INTEN;
        }
               
        
        WRITE_UINT32(p905XRegs, PCI905X_INT_CTRL_STAT, IntCsr);
    }

    // Enable periodic interrupt
    if (USES_GENREGS(pDvcData))
        DtaRegGenCtrlSetPerIntEn(pDvcData->m_pGenRegs, 1);
    else if (pDvcData->m_DevInfo.m_TypeNumber==122 && pDvcData->m_pNonIpPorts!=NULL)
        DtaRegRxCtrlSetRxPerIntEn(pDvcData->m_pNonIpPorts[0].m_pRxRegs, 1);
    else if (pDvcData->m_pNonIpPorts!=NULL)
        DtaRegTxCtrlSetTxPerIntEn(pDvcData->m_pNonIpPorts[0].m_pTxRegs, 1);
        
     // Enable I2c interrupt on device level
    if (pDvcData->m_I2c.m_IsSupported)
        DtaRegI2cCtrlSetRdyIntEn(pDvcData->m_I2c.m_pI2cRegs, 1);

    // SOF-frame interrupts
    if (pDvcData->m_Genlock.m_IsSupported &&
                                 (pDvcData->m_Genlock.m_GenlArch==DTA_GENLOCK_ARCH_2152 ||
                                  pDvcData->m_Genlock.m_GenlArch==DTA_GENLOCK_ARCH_2154))
        DtaRegHdGenlCtrlSetSofIntEna(pDvcData->m_Genlock.m_pGenlRegs, 1);

    //-.-.-.-.-.-.-.-.-.-.-.- Step 2: Enable port-level interrupts -.-.-.-.-.-.-.-.-.-.-.-

    // Enable Non-IP port specific interrupts
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[i];
        DtaNonIpInterruptEnable(pNonIpPort);
    }
    
    // Enable IP-port specific interrupts
    for (i=0; i<pDvcData->m_NumIpPorts; i++)
    {
        pIpPort = &pDvcData->m_IpDevice.m_pIpPorts[i];
        DtaIpEnableInterrupts(pIpPort);
    }

    // All our interrupts are up
    pDvcData->m_IntEnableState = INT_ENABLED;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDevicePowerUpPost -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device must enter the active (D0) state after PowerUp
// is called and the interrupts are enabled. It should only perform actions to put the
// device in power up mode that need the interrupts to be enabled.
//
DtStatus  DtaDevicePowerUpPost(DtaDeviceData* pDvcData)
{
    Int  i;
    DtStatus  Status = DT_STATUS_OK;

    if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2154)
    {
        Status = DtaLmh1983InitPowerup(&pDvcData->m_Genlock.m_Lmh1983);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    // Perform post power-up initialisation for all Non-IP ports
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        Status = DtaNonIpPowerUpPost(&pDvcData->m_pNonIpPorts[i]);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTA, "Error initialising NonIp Port %i Power-Up-post.", i);
            return Status;
        }
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDevicePowerDownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device must leave the active (D0) state before the
// interrupts are disabled. It should only perform actions to put the device in power
// down mode that need the interrupts to be enabled.
//
DtStatus  DtaDevicePowerDownPre(DtaDeviceData* pDvcData)
{
    // Powerdown genlock module
    DtaGenlockPowerDownPre(pDvcData);

    // Powerdown ports
    DtaPowerdownPortsPre(pDvcData);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceInterruptDisable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device interrupts must be disabled. This is done
// Between DevicePowerDownPre and DevicePowerDown.
//
DtStatus  DtaDeviceInterruptDisable(DtaDeviceData* pDvcData)
{
    Int  i;
    DtaNonIpPort*  pNonIpPort = NULL;
    DtaIpPort*  pIpPort = NULL;

    // Interrupts are going down
    pDvcData->m_IntEnableState = INT_DISABLING;
    
    //-.-.-.-.-.-.-.-.-.-.- Step 1: disable device-level interrupts -.-.-.-.-.-.-.-.-.-.-.

    // Disable interrupts in PLX905x
    if (pDvcData->m_DevInfo.m_UsesPlxChip) 
    {
        volatile UInt8*  p905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;
        UInt32  IntCsr;
        
        // Update the interrupt control register
        IntCsr = READ_UINT32(p905XRegs, PCI905X_INT_CTRL_STAT);
        
        // Always enable DMA channel 0 interrupt
        IntCsr &= ~(PCI905X_INTCSR_PCI_INTEN | PCI905X_INTCSR_LOCAL_INTEN);
        WRITE_UINT32(p905XRegs, PCI905X_INT_CTRL_STAT, IntCsr);
    }
        
    // Disable periodic interrupt
    if (USES_GENREGS(pDvcData))
        DtaRegGenCtrlSetPerIntEn(pDvcData->m_pGenRegs, 0);
    else if (pDvcData->m_DevInfo.m_TypeNumber==122 && pDvcData->m_pNonIpPorts!=NULL)
        DtaRegRxCtrlSetRxPerIntEn(pDvcData->m_pNonIpPorts[0].m_pRxRegs, 0);
    else if (pDvcData->m_pNonIpPorts!=NULL)
        DtaRegTxCtrlSetTxPerIntEn(pDvcData->m_pNonIpPorts[0].m_pTxRegs, 0);

    // Disable I2c interrupt on device level
    if (pDvcData->m_I2c.m_IsSupported)    
        DtaRegI2cCtrlSetRdyIntEn(pDvcData->m_I2c.m_pI2cRegs, 0);

    // Matrix interrupts
    if (pDvcData->m_Genlock.m_IsSupported &&
                                 (pDvcData->m_Genlock.m_GenlArch==DTA_GENLOCK_ARCH_2152 ||
                                  pDvcData->m_Genlock.m_GenlArch==DTA_GENLOCK_ARCH_2154))
        DtaRegHdGenlCtrlSetSofIntEna(pDvcData->m_Genlock.m_pGenlRegs, 0); // Disable SOF interrupt

    //.-.-.-.-.-.-.-.-.-.-.- Step 2: disable port-level interrupts -.-.-.-.-.-.-.-.-.-.-.-

    // Disable Non-IP port specific interrupts
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[i];
        DtaNonIpInterruptDisable(pNonIpPort);
    }    


    // Disable IP-port specific interrupts
    for (i=0; i<pDvcData->m_NumIpPorts; i++)
    {
        pIpPort = &pDvcData->m_IpDevice.m_pIpPorts[i];
        DtaIpDisableInterrupts(pIpPort);
    }    

    // Interrupts are down
    pDvcData->m_IntEnableState = INT_DISABLED;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDevicePowerDown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called when the device must leave the active (D0) state. This
// function is called after PowerDownPre is called and the interrupts are disabled. It
// should stop receive / transmit threads, store the device state and put the device in
// low power mode.
//
DtStatus  DtaDevicePowerDown(DtaDeviceData* pDvcData)
{
    DtaFileHandleInfo*  pFileHandleInfo;
    DtDbgOut(MAX, DTA, "Start");

    // Wait for general periodic DPC to finish before hardware is released
    // Interrupts are disabled at this point to prevent new periodic DPC's to be scheduled
    DtDpcWaitForCompletion(&pDvcData->m_GenPerIntDpc);

    // Powerdown genlock module
    DtaGenlockPowerDown(pDvcData);
    
    // Powerdown ports
    DtaPowerdownPorts(pDvcData);
    
    // Set power down pending to prevent new IOCTL's
    DtFastMutexAcquire(&pDvcData->m_FileHandleInfoMutex);

    pFileHandleInfo = pDvcData->m_pFileHandleInfo;
    while (pFileHandleInfo != NULL)
    {
        pFileHandleInfo->m_PowerDownPending = TRUE;
        pFileHandleInfo = pFileHandleInfo->m_pNext;
    }

    DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);

    // Set power down event
    DtaEventsSet(pDvcData, NULL, DTA_EVENT_TYPE_POWER, DTA_EVENT_VALUE1_POWER_DOWN, 0);
    DtDbgOut(MAX, DTA, "Exit");

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceExitPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is executed before the device is removed from the system. All objects
// must not be used anymore and set in the idle/done state. All active transactions must
// be stopped.
//
DtStatus  DtaDeviceExitPre(DtaDeviceData* pDvcData)
{    
    return DtaPortsCleanUpPre(pDvcData);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL when the device is being disconnected. This
// function should undo everything done by DeviceInitPost and DeviceInit.
//
void  DtaDeviceExit(DtaDeviceData* pDvcData)
{       
    // Cleanup child devices
    if (pDvcData->m_NumIpPorts != 0) 
        DtaRemoveChildDevices(pDvcData);

    // Cleanup ports
    DtaPortsCleanUp(pDvcData);

    // Cleanup port structs
    DtaCleanupPortStructs(pDvcData);
    
    // Cleanup I2C module
    DtaI2cCleanup(pDvcData);

    // Cleanup VPD module
    DtaVpdCleanup(pDvcData);

    // Cleanup Properties module
    DtPropertiesCleanup(&pDvcData->m_PropData);

    // Cleanup events
    DtaEventsCleanup(pDvcData);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function will be called by the IAL every time an application opens a handle to the
// device.
//
DtStatus  DtaDeviceOpen(DtaDeviceData* pDvcData, DtFileObject* pFile)
{
    DtaFileHandleInfo*  pFileHandleInfo;

    // Add file handle to the filehandle info array
    DtFastMutexAcquire(&pDvcData->m_FileHandleInfoMutex);
    pFileHandleInfo = (DtaFileHandleInfo*)DtMemAllocPool(DtPoolPaged, 
                                                      sizeof(DtaFileHandleInfo), DTA_TAG);
    if (pFileHandleInfo == NULL)
    {
        DtDbgOut(ERR, DTA, "Out of memory creating new FileHandleInfo struct");
        DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);
        return DT_STATUS_OUT_OF_MEMORY;
    }
    pFileHandleInfo->m_pHandle = DtFileGetHandle(pFile);
    pFileHandleInfo->m_PowerDownPending = FALSE;

    // Insert at start of list
    pFileHandleInfo->m_pPrev = NULL;
    pFileHandleInfo->m_pNext = NULL;
    if (pDvcData->m_pFileHandleInfo != NULL) 
    {
        pFileHandleInfo->m_pNext = pDvcData->m_pFileHandleInfo;
        pFileHandleInfo->m_pNext->m_pPrev = pFileHandleInfo;
    }
    pDvcData->m_pFileHandleInfo = pFileHandleInfo;
    
    DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when an application closes a device which it has
// opened before. All allocated resources which were not deallocated synchronously must
// be checked by this function. If a resource was not deallocated yet it must be done by
// this function. Also exclusive access for a channel held by the current handle to be
// closed will be released.
//
DtStatus  DtaDeviceClose(DtaDeviceData* pDvcData, DtFileObject* pFile)
{
    Int  i;
    DtaFileHandleInfo*  pFileHandleInfo;

    // Unlock all recursive I2C locks for this file object
    if (pDvcData->m_I2c.m_IsSupported)
        DtaI2cUnlock(pDvcData, -1, pFile, TRUE);

    // Release NonIp specific resources
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        DtaNonIpClose(&pDvcData->m_pNonIpPorts[i], pFile);
    }

    // Remove file handle from the filehandle info array
    DtFastMutexAcquire(&pDvcData->m_FileHandleInfoMutex);
    pFileHandleInfo = pDvcData->m_pFileHandleInfo;
    while (pFileHandleInfo != NULL)
    {
        // Search in list and remove entry if found
        if (pFileHandleInfo->m_pHandle == DtFileGetHandle(pFile))
        {
            if (pFileHandleInfo->m_pPrev != NULL)
                pFileHandleInfo->m_pPrev->m_pNext = pFileHandleInfo->m_pNext;
            else
                pDvcData->m_pFileHandleInfo = pFileHandleInfo->m_pNext;
            if (pFileHandleInfo->m_pNext != NULL)
                pFileHandleInfo->m_pNext->m_pPrev = pFileHandleInfo->m_pPrev;
            DtMemFreePool(pFileHandleInfo, DTA_TAG);
            // No duplicate filehandles are in the list
            break;
        }
        pFileHandleInfo = pFileHandleInfo->m_pNext;
    }
    
    DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);

    // Unregister events
    DtaEventsUnregister(pDvcData, pFile);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called for every IO request an application sends to the device driver.
// This function handles all the functional requests. If the request is a DMA transfer,
// it should be queued to the DMA channel queue. This queue must be power managed if
// supported by the host implementation.
//
DtStatus  DtaDeviceIoctl(DtaDeviceData* pDvcData, 
                                               DtFileObject* pFile, DtIoctlObject* pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pIoctlStr = NULL;
    UInt  InReqSize = 0;
    UInt  OutReqSize = 0;
    DtaIoctlInputData*  pInBuf = (DtaIoctlInputData*)pIoctl->m_pInputBuffer;
    DtaIoctlOutputData*  pOutBuf = (DtaIoctlOutputData*)pIoctl->m_pOutputBuffer;
    DtaFileHandleInfo*  pFileHandleInfo;
    Bool  PowerDownPending = FALSE;

    // Get powerdown pending flag for this file handle from the filehandle info array
    DtFastMutexAcquire(&pDvcData->m_FileHandleInfoMutex);
    pFileHandleInfo = pDvcData->m_pFileHandleInfo;
    while (pFileHandleInfo != NULL)
    {
        if (pFileHandleInfo->m_pHandle == DtFileGetHandle(pFile))
        {
            PowerDownPending = pFileHandleInfo->m_PowerDownPending;
            break;
        }
        pFileHandleInfo = pFileHandleInfo->m_pNext;
    }
    DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);
    
    // Get minimal input/output buffer sizes
    switch (pIoctl->m_IoctlCode)
    {
    case DTA_IOCTL_GET_PROPERTY:
        pIoctlStr = "DTA_IOCTL_GET_PROPERTY";
        InReqSize = sizeof(DtaIoctlGetPropertyInput);
        OutReqSize = sizeof(DtaIoctlGetPropertyOutput);
        break;
    case DTA_IOCTL_GET_DEV_INFO:
        pIoctlStr = "DTA_IOCTL_GET_DEV_INFO";
        InReqSize = 0;
        OutReqSize = sizeof(DtaIoctlGetDevInfoOutput);
        break;
    case DTA_IOCTL_GET_DEV_INFO2:
        pIoctlStr = "DTA_IOCTL_GET_DEV_INFO2";
        InReqSize = 0;
        OutReqSize = sizeof(DtaIoctlGetDevInfoOutput2);
        break;
    case DTA_IOCTL_VPD_CMD:
        pIoctlStr = "DTA_IOCTL_VPD_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTA_IOCTL_GET_DRIVER_VERSION:
        pIoctlStr = "DTA_IOCTL_GET_DRIVER_VERSION";
        InReqSize = 0;
        OutReqSize = sizeof(DtaIoctlGetDriverVersionOutput);
        break;
    case DTA_IOCTL_GET_NWDRIVER_VERSION:
        pIoctlStr = "DTA_IOCTL_GET_NWDRIVER_VERSION";
        InReqSize = 0;
        OutReqSize = sizeof(DtaIoctlGetNwDriverVersionOutput);
        break;
    case DTA_IOCTL_I2C_CMD:
        pIoctlStr = "DTA_IOCTL_I2C_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTA_IOCTL_GET_ADDRESS_REGS:
        pIoctlStr = "DTA_IOCTL_GET_ADDRESS_REGS";
        InReqSize = sizeof(DtaIoctlGetAddressRegsInput);
        OutReqSize = sizeof(DtaIoctlGetAddressRegsOutput);
        break;
    case DTA_IOCTL_RELEASE_ADDRESS_REGS:
        pIoctlStr = "DTA_IOCTL_RELEASE_ADDRESS_REGS";
        InReqSize = sizeof(DtaIoctlReleaseAddressRegsInput);
        // The check below should not be in the generic code, but this is an exception
        // to prevent more complicated code 
#ifdef WINBUILD     
        OutReqSize = 0;
#else
        OutReqSize = sizeof(DtaIoctlReleaseAddressRegsOutput);
#endif
        break;
    case DTA_IOCTL_GET_EVENT:
        pIoctlStr = "DTA_IOCTL_GET_EVENT";
        InReqSize = 0;
        OutReqSize = sizeof(DtaIoctlGetEventOutput);
        break;
    case DTA_IOCTL_REGISTER_EVENTS:
        pIoctlStr = "DTA_IOCTL_REGISTER_EVENTS";
        InReqSize = sizeof(DtaIoctlRegisterEventsInput);
        OutReqSize = 0;
        break;
    case DTA_IOCTL_UNREGISTER_EVENTS:
        pIoctlStr = "DTA_IOCTL_UNREGISTER_EVENTS";
        InReqSize = 0;
        OutReqSize = 0;
        break;
    case DTA_IOCTL_DMA_READ:
        pIoctlStr = "DTA_IOCTL_DMA_READ";
        InReqSize = sizeof(DtaIoctlDmaReadInput);
#ifdef WINBUILD
        OutReqSize = 0;
#else
        OutReqSize = sizeof(DtaIoctlDmaReadOutput);
#endif
        break;
    case DTA_IOCTL_DMA_WRITE:
        pIoctlStr = "DTA_IOCTL_DMA_WRITE";
        InReqSize = sizeof(DtaIoctlDmaWriteInput);
        OutReqSize = 0;
        break;
    case DTA_IOCTL_SH_BUF_CMD:
        pIoctlStr = "DTA_IOCTL_SH_BUF_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTA_IOCTL_GET_IOCONFIG:
        pIoctlStr = "DTA_IOCTL_GET_IOCONFIG";
        InReqSize = OFFSETOF(DtaIoctlGetIoConfigInput, m_IoCfgId);
        OutReqSize = OFFSETOF(DtaIoctlGetIoConfigOutput, m_IoCfgValue);
        break;
    case DTA_IOCTL_SET_IOCONFIG:
        pIoctlStr = "DTA_IOCTL_SET_IOCONFIG";
        InReqSize = OFFSETOF(DtaIoctlSetIoConfigInput, m_IoConfig);
        OutReqSize = 0;
        break;
    case DTA_IOCTL_NONIP_CMD:
        pIoctlStr = "DTA_IOCTL_NONIP_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTA_IOCTL_NONIP_TX_CMD:
        pIoctlStr = "DTA_IOCTL_NONIP_TX_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTA_IOCTL_NONIP_RX_CMD:
        pIoctlStr = "DTA_IOCTL_NONIP_RX_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTA_IOCTL_IP_CMD:
        pIoctlStr = "DTA_IOCTL_IP_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTA_IOCTL_IP_TX_CMD:
        pIoctlStr = "DTA_IOCTL_IP_TX_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTA_IOCTL_IP_RX_CMD:
        pIoctlStr = "DTA_IOCTL_IP_RX_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;    
    case DTA_IOCTL_GET_TABLE:
        pIoctlStr = "DTA_IOCTL_GET_TABLE";
        InReqSize = sizeof(DtaIoctlGetTableInput);
        OutReqSize = sizeof(DtaIoctlGetTableOutput);
        break;
    case DTA_IOCTL_GET_TABLE2:
        pIoctlStr = "DTA_IOCTL_GET_TABLE2";
        InReqSize = sizeof(DtaIoctlGetTable2Input);
        OutReqSize = sizeof(DtaIoctlGetTableOutput);
        break;
    case DTA_IOCTL_REBOOT_FIRMWARE:
        pIoctlStr = "DTA_IOCTL_REBOOT_FIRMWARE";
        InReqSize = sizeof(DtaIoctlRebootFirmwareInput);
        OutReqSize = 0;
        break;
    case DTA_IOCTL_SET_MEMORY_TESTMODE:
        pIoctlStr = "DTA_IOCTL_SET_DEVICE_POWERSTATE";
        InReqSize = sizeof(DtaIoctlSetMemoryTestModeInput);
        OutReqSize = 0;
        break;
    case DTA_IOCTL_GET_STR_PROPERTY:
        pIoctlStr = "DTA_IOCTL_GET_STR_PROPERTY";
        InReqSize = sizeof(DtaIoctlGetStrPropertyInput);
        OutReqSize = sizeof(DtaIoctlGetStrPropertyOutput);
        break;
    case DTA_IOCTL_GET_STR_PROPERTY2:
        pIoctlStr = "DTA_IOCTL_GET_STR_PROPERTY2";
        InReqSize = sizeof(DtaIoctlGetStrProperty2Input);
        OutReqSize = sizeof(DtaIoctlGetStrPropertyOutput);
        break;
    case DTA_IOCTL_MATRIX_CMD:
        pIoctlStr = "DTA_IOCTL_MATRIX_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTA_IOCTL_GET_DEV_SUBTYPE:
        pIoctlStr = "DTA_IOCTL_GET_DEV_SUBTYPE";
        InReqSize = 0;
        OutReqSize = sizeof(DtaIoctlGetDevSubTypeOutput);
        break;
    case DTA_IOCTL_GET_PROPERTY2:
        pIoctlStr = "DTA_IOCTL_GET_PROPERTY2";
        InReqSize = sizeof(DtaIoctlGetProperty2Input);
        OutReqSize = sizeof(DtaIoctlGetPropertyOutput);
        break;
    case DTA_IOCTL_GET_DEV_GENLOCKSTATE:
        pIoctlStr = "DTA_IOCTL_GET_DEV_GENLOCKSTATE";
        InReqSize = 0;
        OutReqSize = sizeof(DtaIoctlGetGenlockStateOutput);
        break;
    case DTA_IOCTL_GET_PROPERTY3:
        pIoctlStr = "DTA_IOCTL_GET_PROPERTY3";
        InReqSize = sizeof(DtaIoctlGetProperty3Input);
        OutReqSize = sizeof(DtaIoctlGetPropertyOutput);
        break;
    case DTA_IOCTL_RS422_CMD:
        pIoctlStr = "DTA_IOCTL_RS422_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    default:
        Status = DT_STATUS_NOT_SUPPORTED;
        break;
    }

    if (PowerDownPending)
    {
        // Only skip channel specific IOCTL's
        switch (pIoctl->m_IoctlCode)
        {
            case DTA_IOCTL_DMA_READ:
            case DTA_IOCTL_DMA_WRITE:
            case DTA_IOCTL_NONIP_CMD:
            case DTA_IOCTL_NONIP_TX_CMD:
            case DTA_IOCTL_NONIP_RX_CMD:
            case DTA_IOCTL_IP_CMD:
            case DTA_IOCTL_IP_TX_CMD:
            case DTA_IOCTL_IP_RX_CMD:
            case DTA_IOCTL_SH_BUF_CMD:
            case DTA_IOCTL_MATRIX_CMD:
            case DTA_IOCTL_RS422_CMD:
                DtDbgOut(ERR, DTA, "%s: Skipping IOCTL because powerdown  occured!",
                                                                               pIoctlStr);
                return DT_STATUS_POWERDOWN;
        } 
    }

    // Check if buffer sizes satisfy
    if (DT_SUCCESS(Status))
    {
        if (pIoctl->m_InputBufferSize < InReqSize) 
        {
            DtDbgOut(ERR, DTA, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) INPUT BUFFER TOO SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
            Status = DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize) 
        {
            DtDbgOut(ERR, DTA, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) OUTPUT BUFFER TOO"
                                          " SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
            Status = DT_STATUS_INVALID_PARAMETER;
        }
        if (DT_SUCCESS(Status))
            DtDbgOut(MAX, DTA, "%s: In=%d (Rq=%d) Out=%d (Rq=%d)",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;
    
    // Handle IOCTLS
    if (DT_SUCCESS(Status))
    {
        switch (pIoctl->m_IoctlCode)
        {
        case DTA_IOCTL_GET_PROPERTY:
            // Get for specific type or for the attached devices
            if (pInBuf->m_GetProperty.m_TypeNumber == -1)
            {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;

                // Get the property for the current device
                Status = DtPropertiesGet(&pDvcData->m_PropData,
                                                        pInBuf->m_GetProperty.m_Name,
                                                        pInBuf->m_GetProperty.m_PortIndex,
                                                        &Value, &Type, &Scope,
                                                        0, 0, 0);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            } else {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;
                
                // Property for a specific type was requested
                Status = DtPropertiesGetForType("DTA", 
                                                 pInBuf->m_GetProperty.m_TypeNumber,
                                                 pDvcData->m_PropData.m_SubDvc,
                                                 pInBuf->m_GetProperty.m_HardwareRevision,
                                                 pInBuf->m_GetProperty.m_FirmwareVersion,
                                                 pInBuf->m_GetProperty.m_Name,
                                                 pInBuf->m_GetProperty.m_PortIndex,
                                                 &Value, &Type, &Scope,
                                                 0, 0, 0);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            }
            
            if (DT_SUCCESS(Status))
            {
                DT_ASSERT((pOutBuf->m_GetProperty.m_Scope&PROPERTY_SCOPE_DTAPI) ==
                                                                    PROPERTY_SCOPE_DTAPI);
            }
            break;
        case DTA_IOCTL_GET_DEV_INFO:
            pOutBuf->m_GetDevInfo.m_DeviceId = pDvcData->m_DevInfo.m_DeviceId;
            pOutBuf->m_GetDevInfo.m_VendorId = pDvcData->m_DevInfo.m_VendorId;
            pOutBuf->m_GetDevInfo.m_SubSystemId = pDvcData->m_DevInfo.m_SubSystemId;
            pOutBuf->m_GetDevInfo.m_SubVendorId = pDvcData->m_DevInfo.m_SubVendorId;
            pOutBuf->m_GetDevInfo.m_BusNumber = pDvcData->m_DevInfo.m_BusNumber;
            pOutBuf->m_GetDevInfo.m_SlotNumber = pDvcData->m_DevInfo.m_SlotNumber;
            pOutBuf->m_GetDevInfo.m_TypeNumber = pDvcData->m_DevInfo.m_TypeNumber;
            pOutBuf->m_GetDevInfo.m_HardwareRevision =
                                                   pDvcData->m_DevInfo.m_HardwareRevision;
            pOutBuf->m_GetDevInfo.m_FirmwareVersion =
                                                    pDvcData->m_DevInfo.m_FirmwareVersion;
            pOutBuf->m_GetDevInfo.m_FirmwareVariant =
                                                    pDvcData->m_DevInfo.m_FirmwareVariant;
            pOutBuf->m_GetDevInfo.m_Serial = pDvcData->m_DevInfo.m_Serial;
            break;
        case DTA_IOCTL_GET_DEV_INFO2:
            pOutBuf->m_GetDevInfo2.m_DeviceId = pDvcData->m_DevInfo.m_DeviceId;
            pOutBuf->m_GetDevInfo2.m_VendorId = pDvcData->m_DevInfo.m_VendorId;
            pOutBuf->m_GetDevInfo2.m_SubSystemId = pDvcData->m_DevInfo.m_SubSystemId;
            pOutBuf->m_GetDevInfo2.m_SubVendorId = pDvcData->m_DevInfo.m_SubVendorId;
            pOutBuf->m_GetDevInfo2.m_BusNumber = pDvcData->m_DevInfo.m_BusNumber;
            pOutBuf->m_GetDevInfo2.m_SlotNumber = pDvcData->m_DevInfo.m_SlotNumber;
            pOutBuf->m_GetDevInfo2.m_TypeNumber = pDvcData->m_DevInfo.m_TypeNumber;
            pOutBuf->m_GetDevInfo2.m_SubDvc = pDvcData->m_DevInfo.m_SubDvc;
            pOutBuf->m_GetDevInfo2.m_HardwareRevision =
                                                   pDvcData->m_DevInfo.m_HardwareRevision;
            pOutBuf->m_GetDevInfo2.m_FirmwareVersion =
                                                    pDvcData->m_DevInfo.m_FirmwareVersion;
            pOutBuf->m_GetDevInfo2.m_FirmwareVariant =
                                                    pDvcData->m_DevInfo.m_FirmwareVariant;
            pOutBuf->m_GetDevInfo2.m_Serial = pDvcData->m_DevInfo.m_Serial;
            break;
        case DTA_IOCTL_VPD_CMD:
            Status = DtaVpdIoctl(pDvcData, pIoctl);
            break;
        case DTA_IOCTL_GET_DRIVER_VERSION:
            pOutBuf->m_GetDriverVersion.m_Major = DTA_VERSION_MAJOR;
            pOutBuf->m_GetDriverVersion.m_Minor = DTA_VERSION_MINOR;
            pOutBuf->m_GetDriverVersion.m_Micro = DTA_VERSION_MICRO;
            pOutBuf->m_GetDriverVersion.m_Build = DTA_VERSION_BUILD;
            break;
        case DTA_IOCTL_GET_NWDRIVER_VERSION:
            pOutBuf->m_GetNwDriverVersion.m_Major = g_NwDrvVersionMajor;
            pOutBuf->m_GetNwDriverVersion.m_Minor = g_NwDrvVersionMinor;
            pOutBuf->m_GetNwDriverVersion.m_Micro = g_NwDrvVersionMicro;
            pOutBuf->m_GetNwDriverVersion.m_Build = g_NwDrvVersionBuild;
            break;
        case DTA_IOCTL_I2C_CMD:
            Status = DtaI2cIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTA_IOCTL_GET_ADDRESS_REGS:
            {
                void*  pPaMmap = NULL;
                void*  pUserVirt = NULL;
#ifdef LINBUILD
#ifdef LIN32
                pPaMmap = (void*)(UInt32)pInBuf->m_GetAddrRegs.m_pPaMmap;
#else
                pPaMmap = (void*)pInBuf->m_GetAddrRegs.m_pPaMmap;
#endif
#endif
                Status = DtaGetAddressRegs(pDvcData, pFile,
                                                pInBuf->m_GetAddrRegs.m_PortIndex,
                                                pInBuf->m_GetAddrRegs.m_RegsType, pPaMmap,
                                                &pOutBuf->m_GetAddrRegs.m_Size,
                                                (void**)&pUserVirt);
#if defined(WIN64) || defined(LIN64)
                pOutBuf->m_GetAddrRegs.m_pUserVirtual = (UInt64)pUserVirt;
#else
                pOutBuf->m_GetAddrRegs.m_pUserVirtual = (UInt32)pUserVirt;
#endif
                
#ifdef LINBUILD
                // Special case
                pOutBuf->m_GetAddrRegs.m_NotMapped = 0;
                if (Status == DT_STATUS_NOT_INITIALISED)
                {
                    pOutBuf->m_GetAddrRegs.m_NotMapped = 1;
                    Status = DT_STATUS_OK;
                }
#endif
            }   
            break;
        case DTA_IOCTL_RELEASE_ADDRESS_REGS:
            {
                void* pPaMmap = NULL;
                Int Size = 0;
                Status = DtaReleaseAddressRegs(pDvcData, pFile, 
                                                    pInBuf->m_ReleaseAddrRegs.m_PortIndex,
                                                    pInBuf->m_ReleaseAddrRegs.m_RegsType,
                                                    &pPaMmap, &Size);
#ifdef LINBUILD
                pOutBuf->m_ReleaseAddrRegs.m_Size = Size;
#ifdef LIN32
                pOutBuf->m_ReleaseAddrRegs.m_pPaMmap = (UInt32)pPaMmap;
#else
                pOutBuf->m_ReleaseAddrRegs.m_pPaMmap = (UInt64)pPaMmap;
#endif
                // Special case
                pOutBuf->m_ReleaseAddrRegs.m_NotInUse = 1;
                if (Status == DT_STATUS_IN_USE)
                {
                    pOutBuf->m_ReleaseAddrRegs.m_NotInUse = 0;
                    Status = DT_STATUS_OK;
                }
#endif
            }
            break;
        case DTA_IOCTL_GET_EVENT:
            Status = DtaEventsGet(pDvcData, pFile, NULL, &pOutBuf->m_GetEvent.m_EventType,
                                                    &pOutBuf->m_GetEvent.m_Value1,
                                                    &pOutBuf->m_GetEvent.m_Value2, FALSE);
            break;
        case DTA_IOCTL_REGISTER_EVENTS:
            Status = DtaEventsRegister(pDvcData, pFile,
                                                pInBuf->m_RegisterEvents.m_EventTypeMask);
            break;
        case DTA_IOCTL_UNREGISTER_EVENTS:
            DtaEventsUnregister(pDvcData, pFile);
            break;
         case DTA_IOCTL_DMA_READ:
            {
                char*  pBuffer;
                UInt  Size;
                DtPageList*  pPageList = NULL;
                Int  NonIpPortIndex;
                UInt8*  pLocalAddress;
                DmaChannel*  pDmaCh = NULL;

#if defined(WINBUILD)
                DtPageList  PageList;
                PMDL  pMdl;
                NTSTATUS  NtStatus;
                DtaWdfRequestContext*  pRequestContext;
                WDF_OBJECT_ATTRIBUTES  ObjAttr;

                // Retrieve MDL and virtual buffer from request object
                NtStatus = WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
                if (NtStatus != STATUS_SUCCESS)
                {
                    DtDbgOut(ERR, DTA, "WdfRequestRetrieveOutputWdmMdl error: %08x", 
                                                                                NtStatus);
                    Status = DT_STATUS_OUT_OF_RESOURCES;
                }
                if (DT_SUCCESS(Status))
                {
                    pBuffer = MmGetMdlVirtualAddress(pMdl);
                    if (pBuffer == NULL)
                        Status = DT_STATUS_OUT_OF_MEMORY;
                    Size = MmGetMdlByteCount(pMdl);

                    // Build pagelist object for user space buffer
                    pPageList = &PageList;
                    pPageList->m_BufType = DT_BUFTYPE_USER;
                    pPageList->m_OwnedByOs = TRUE;
                    pPageList->m_pMdl = pMdl;
                    pPageList->m_pVirtualKernel = NULL;
                }
#else // LINBUILD
                Size = pInBuf->m_DmaRead.m_NumBytesToRead;
#if defined(LIN32)
                pBuffer = (char*)(UInt32)pInBuf->m_DmaRead.m_BufferAddr;
#else
                pBuffer = (char*)(UInt64)pInBuf->m_DmaRead.m_BufferAddr;
#endif
#endif
                // Check if the PortIndex is from the NonIpPort
                if (DT_SUCCESS(Status))
                    Status = DtaGetNonIpPortIndex(pDvcData, pInBuf->m_DmaRead.m_PortIndex, 
                                                                         &NonIpPortIndex);
                if (DT_SUCCESS(Status))
                {
                    pDmaCh = &pDvcData->m_pNonIpPorts[NonIpPortIndex].m_DmaChannel;
                    pLocalAddress = 
                     (UInt8*)(size_t)pDvcData->m_pNonIpPorts[NonIpPortIndex].m_FifoOffset;
#ifdef WINBUILD
                    // Store the NonIpPort pointer in the request object
                    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&ObjAttr, 
                                                                    DtaWdfRequestContext);
                    if (!NT_SUCCESS(WdfObjectAllocateContext(pIoctl->m_WdfRequest, 
                                                     &ObjAttr, (PVOID*)&pRequestContext)))
                        Status = DT_STATUS_OUT_OF_RESOURCES;
                    else 
                        pRequestContext->m_pData = 
                                                 &pDvcData->m_pNonIpPorts[NonIpPortIndex];

                    if (DT_SUCCESS(Status))
                    {
                        // Register a cancel routine
                        if (!NT_SUCCESS(WdfRequestMarkCancelableEx(pIoctl->m_WdfRequest,
                                                          DtaNonIpRxEvtRequestCancelDma)))
                            Status = DT_STATUS_CANCELLED;
                        
                        // Store the WdfRequest for the completion routine
                        DtaDmaReInitCallback(pDmaCh, DtaNonIpDmaCompletedWindows, 
                                                                    pIoctl->m_WdfRequest);
                    }
#endif
                }

                if (DT_SUCCESS(Status))
                    Status = DtaDmaStartTransfer(pDmaCh, pPageList, DT_BUFTYPE_USER, 
                                           DT_DMA_DIRECTION_FROM_DEVICE, pBuffer, Size, 0,
                                           pLocalAddress, 0, 0, FALSE, &Size);
#if defined(WINBUILD)
                // Mark the IO request pending, we complete the request in the DMA 
                // completion routine. This is an internal status and will be handled by
                // the IAL
                if (DT_SUCCESS(Status))
                    Status = DT_STATUS_IO_PENDING;
                else {
                    WdfRequestUnmarkCancelable(pIoctl->m_WdfRequest);
                    pIoctl->m_OutputBufferBytesWritten = 0;
                }
#else
                // For Linux, the pIoctl->m_OutputBufferBytesWritten contains the output
                // struct size
                if (DT_SUCCESS(Status))
                    pOutBuf->m_DmaRead.m_NumBytesRead = Size;
                else
                    pOutBuf->m_DmaRead.m_NumBytesRead = 0;
#endif
            }

            break;
        case DTA_IOCTL_DMA_WRITE:
            {
                char*  pBuffer;
                UInt  Size;
                DtPageList*  pPageList = NULL;
                Int  NonIpPortIndex;
                UInt8*  pLocalAddress;
                DmaChannel*  pDmaCh = NULL;

#if defined(WINBUILD)
                DtPageList  PageList;
                PMDL  pMdl;
                NTSTATUS  NtStatus;
                DtaWdfRequestContext*  pRequestContext;
                WDF_OBJECT_ATTRIBUTES  ObjAttr;

                // Retrieve MDL and virtual buffer from request object
                NtStatus = WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
                if (NtStatus != STATUS_SUCCESS)
                {
                    DtDbgOut(ERR, DTA, "WdfRequestRetrieveOutputWdmMdl error: %08x", 
                                                                                NtStatus);
                    Status = DT_STATUS_OUT_OF_RESOURCES;
                }
                if (DT_SUCCESS(Status))
                {
                    pBuffer = MmGetMdlVirtualAddress(pMdl);
                    if (pBuffer == NULL)
                        Status = DT_STATUS_OUT_OF_MEMORY;
                
                    Size = MmGetMdlByteCount(pMdl);

                    // Build pagelist object for user space buffer
                    pPageList = &PageList;
                    pPageList->m_BufType = DT_BUFTYPE_USER;
                    pPageList->m_OwnedByOs = TRUE;
                    pPageList->m_pMdl = pMdl;
                    pPageList->m_pVirtualKernel = NULL;
                }
#else // LINBUILD
                Size = pInBuf->m_DmaWrite.m_NumBytesToWrite;
#if defined(LIN32)
                pBuffer = (char*)(UInt32)pInBuf->m_DmaWrite.m_BufferAddr;
#else
                pBuffer = (char*)(UInt64)pInBuf->m_DmaWrite.m_BufferAddr;
#endif
#endif
                // Check if the PortIndex is from the NonIpPort
                if (DT_SUCCESS(Status))
                    Status = DtaGetNonIpPortIndex(pDvcData, 
                                         pInBuf->m_DmaWrite.m_PortIndex, &NonIpPortIndex);
                if (DT_SUCCESS(Status))
                {
                    pDmaCh = &pDvcData->m_pNonIpPorts[NonIpPortIndex].m_DmaChannel;
                    pLocalAddress = 
                     (UInt8*)(size_t)pDvcData->m_pNonIpPorts[NonIpPortIndex].m_FifoOffset;
#ifdef WINBUILD
                    // Store the NonIpPort pointer in the request object
                    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&ObjAttr, 
                                                                    DtaWdfRequestContext);
                    if (!NT_SUCCESS(WdfObjectAllocateContext(pIoctl->m_WdfRequest, 
                                                     &ObjAttr, (PVOID*)&pRequestContext)))
                        Status = DT_STATUS_OUT_OF_RESOURCES;
                    else 
                        pRequestContext->m_pData = 
                                                 &pDvcData->m_pNonIpPorts[NonIpPortIndex];
                    
                    if (DT_SUCCESS(Status))
                    {
                        // Register a cancel routine
                        if (!NT_SUCCESS(WdfRequestMarkCancelableEx(pIoctl->m_WdfRequest,
                                                          DtaNonIpTxEvtRequestCancelDma)))
                            Status = DT_STATUS_CANCELLED;
                        
                        // Store the WdfRequest for the completion routine
                        DtaDmaReInitCallback(pDmaCh, DtaNonIpDmaCompletedWindows, 
                                                                    pIoctl->m_WdfRequest);
                    }
#endif
                }

                if (DT_SUCCESS(Status))
                    Status = DtaDmaStartTransfer(pDmaCh, pPageList, DT_BUFTYPE_USER, 
                                             DT_DMA_DIRECTION_TO_DEVICE, pBuffer, Size, 0,
                                             pLocalAddress, 0, 0, FALSE, &Size);
#ifdef WINBUILD
                // Mark the IO request pending, we complete the request in the DMA 
                // completion routine. This is an internal status and will be handled by
                // the IAL
                if (DT_SUCCESS(Status))
                    Status = DT_STATUS_IO_PENDING;
                else
                    WdfRequestUnmarkCancelable(pIoctl->m_WdfRequest);
#endif
            }
            break;

        case DTA_IOCTL_SH_BUF_CMD:
            Status = DtaShBufferIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTA_IOCTL_GET_IOCONFIG:

            // Update required buffer sizes
            InReqSize += pInBuf->m_GetIoConfig.m_IoConfigCount * sizeof(DtIoConfigId);
            OutReqSize += pInBuf->m_GetIoConfig.m_IoConfigCount * sizeof(DtIoConfigValue);

            // Check input+output parameter size
            if (pIoctl->m_InputBufferSize < InReqSize)
            {
                DtDbgOut(ERR, DTA, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) OUTPUT BUFFER TOO"
                                          " SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
                Status = DT_STATUS_INVALID_PARAMETER;
            }
            // check output buffer size
            else if (pIoctl->m_OutputBufferSize < OutReqSize)
            {
                DtDbgOut(ERR, DTA, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) OUTPUT BUFFER TOO"
                                          " SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
                Status = DT_STATUS_INVALID_PARAMETER;

            } else {
                Status = DtaIoConfigGet(pDvcData, pFile, 
                                                   pInBuf->m_GetIoConfig.m_IoCfgId,
                                                   pOutBuf->m_GetIoConfig.m_IoCfgValue,
                                                   pInBuf->m_GetIoConfig.m_IoConfigCount);

                // Set actual number of bytes written/returned
                if (DT_SUCCESS(Status))
                    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

            }
            break;
        case DTA_IOCTL_SET_IOCONFIG:
            if (pIoctl->m_InputBufferSize < sizeof(DtaIoctlSetIoConfigInput) +
                             pInBuf->m_SetIoConfig.m_IoConfigCount*sizeof(DtIoConfigPars))
                Status = DT_STATUS_INVALID_PARAMETER;
            else
                Status = DtaIoConfigSet(pDvcData, pFile,
                                                   pInBuf->m_SetIoConfig.m_IoConfig,
                                                   pInBuf->m_SetIoConfig.m_IoConfigCount);
            break;
        case DTA_IOCTL_NONIP_CMD:
            Status = DtaNonIpIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTA_IOCTL_NONIP_TX_CMD:
            Status = DtaNonIpTxIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTA_IOCTL_NONIP_RX_CMD:
            Status = DtaNonIpRxIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTA_IOCTL_IP_CMD:
            Status = DtaIpIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTA_IOCTL_IP_TX_CMD:
            Status = DtaIpTxIoctl(pDvcData, pFile, pIoctl);
            break;   
        case DTA_IOCTL_IP_RX_CMD:
            Status = DtaIpRxIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTA_IOCTL_GET_TABLE:
            Status = DtTableGet(&pDvcData->m_PropData, pInBuf->m_GetTable.m_Name,
                     pInBuf->m_GetTable.m_PortIndex, pInBuf->m_GetTable.m_MaxNumEntries,
                     &pOutBuf->m_GetTable.m_NumEntries, pOutBuf->m_GetTable.m_TableEntry,
               pIoctl->m_OutputBufferSize-OFFSETOF(DtaIoctlGetTableOutput, m_TableEntry));
            if (DT_SUCCESS(Status)) 
            {
                if (pInBuf->m_GetTable.m_MaxNumEntries>=pOutBuf->m_GetTable.m_NumEntries)
                    pIoctl->m_OutputBufferBytesWritten += 
                                  pOutBuf->m_GetTable.m_NumEntries * sizeof(DtTableEntry);
            }
            break;
        case DTA_IOCTL_GET_TABLE2:

            if (pInBuf->m_GetTable2.m_TypeNumber==-1 || pInBuf->m_GetTable2.m_SubDvc==-1)
            {
                Status = DtTableGet(
                            &pDvcData->m_PropData,
                            pInBuf->m_GetTable2.m_Name,
                            pInBuf->m_GetTable2.m_PortIndex,
                            pInBuf->m_GetTable2.m_MaxNumEntries,
                            &pOutBuf->m_GetTable.m_NumEntries,
                            pOutBuf->m_GetTable.m_TableEntry,
                            pIoctl->m_OutputBufferSize-OFFSETOF(DtaIoctlGetTableOutput, 
                                                                           m_TableEntry));
            }
            else
            {
                Status = DtTableGetForType(
                            "DTA",
                            pInBuf->m_GetTable2.m_TypeNumber,
                            pInBuf->m_GetTable2.m_SubDvc,
                            pInBuf->m_GetTable2.m_HardwareRevision,
                            pInBuf->m_GetTable2.m_FirmwareVersion,
                            pInBuf->m_GetTable2.m_Name,
                            pInBuf->m_GetTable2.m_PortIndex,
                            pInBuf->m_GetTable2.m_MaxNumEntries,
                            &pOutBuf->m_GetTable.m_NumEntries,
                            pOutBuf->m_GetTable.m_TableEntry,
                            pIoctl->m_OutputBufferSize-OFFSETOF(DtaIoctlGetTableOutput, 
                                                                           m_TableEntry));
            }
            if (DT_SUCCESS(Status)) 
            {
                if (pInBuf->m_GetTable2.m_MaxNumEntries>=pOutBuf->m_GetTable.m_NumEntries)
                    pIoctl->m_OutputBufferBytesWritten += 
                                  pOutBuf->m_GetTable.m_NumEntries * sizeof(DtTableEntry);
            }
            break;

        case DTA_IOCTL_SET_MEMORY_TESTMODE:
            Status = DtaSetMemoryTestMode(pDvcData, 
                                                (pInBuf->m_SetMemoryTestMode.m_On > 0));
            break;
        case DTA_IOCTL_REBOOT_FIRMWARE:
            Status = DtaRebootFirmware(pDvcData);
            break;
        case DTA_IOCTL_GET_STR_PROPERTY:
            // Get for specific type or for the attached devices
            if (pInBuf->m_GetStrProperty.m_TypeNumber == -1)
            {
                // Get the property for the current device
                Status = DtPropertiesStrGet(&pDvcData->m_PropData,
                                                     pInBuf->m_GetStrProperty.m_Name,
                                                     pInBuf->m_GetStrProperty.m_PortIndex,
                                                     pOutBuf->m_GetStrProperty.m_Str,
                                                     &pOutBuf->m_GetStrProperty.m_Scope,
                                                     0, 0, 0);
            } else {
                // Property for a specific type was requested
                Status = DtPropertiesStrGetForType("DTA",
                                              pInBuf->m_GetStrProperty.m_TypeNumber,
                                              pDvcData->m_PropData.m_SubDvc, 
                                              pInBuf->m_GetStrProperty.m_HardwareRevision, 
                                              pInBuf->m_GetStrProperty.m_FirmwareVersion,
                                              pInBuf->m_GetStrProperty.m_Name,
                                              pInBuf->m_GetStrProperty.m_PortIndex,
                                              pOutBuf->m_GetStrProperty.m_Str,
                                              &pOutBuf->m_GetStrProperty.m_Scope,
                                              0, 0, 0);
            }

            if (DT_SUCCESS(Status))
            {
                DT_ASSERT((pOutBuf->m_GetProperty.m_Scope&PROPERTY_SCOPE_DTAPI) ==
                                                                    PROPERTY_SCOPE_DTAPI);
            }
            break;
        
        case DTA_IOCTL_GET_STR_PROPERTY2:
            // Get for specific type or for the attached devices
            if (pInBuf->m_GetStrProperty2.m_TypeNumber==-1 
                                                || pInBuf->m_GetStrProperty2.m_SubDvc==-1)
            {
                // Get the property for the current device
                Status = DtPropertiesStrGet(&pDvcData->m_PropData,
                                                 pInBuf->m_GetStrProperty2.m_Name,
                                                 pInBuf->m_GetStrProperty2.m_PortIndex,
                                                 pOutBuf->m_GetStrProperty.m_Str,
                                                 &pOutBuf->m_GetStrProperty.m_Scope,
                                                 pInBuf->m_GetStrProperty2.m_DtapiMaj,
                                                 pInBuf->m_GetStrProperty2.m_DtapiMin,
                                                 pInBuf->m_GetStrProperty2.m_DtapiBugfix);
            } else {
                // Property for a specific type was requested
                Status = DtPropertiesStrGetForType("DTA",
                                              pInBuf->m_GetStrProperty2.m_TypeNumber,
                                              pInBuf->m_GetStrProperty2.m_SubDvc, 
                                              pInBuf->m_GetStrProperty2.m_HardwareRevision, 
                                              pInBuf->m_GetStrProperty2.m_FirmwareVersion,
                                              pInBuf->m_GetStrProperty2.m_Name,
                                              pInBuf->m_GetStrProperty2.m_PortIndex,
                                              pOutBuf->m_GetStrProperty.m_Str,
                                              &pOutBuf->m_GetStrProperty.m_Scope,
                                              pInBuf->m_GetStrProperty2.m_DtapiMaj,
                                              pInBuf->m_GetStrProperty2.m_DtapiMin,
                                              pInBuf->m_GetStrProperty2.m_DtapiBugfix);
            }

            if (DT_SUCCESS(Status))
            {
                DT_ASSERT((pOutBuf->m_GetProperty.m_Scope&PROPERTY_SCOPE_DTAPI) ==
                                                                    PROPERTY_SCOPE_DTAPI);
            }
            break;

        case DTA_IOCTL_MATRIX_CMD:
            Status = DtaMatrixIoctl(pDvcData, pFile, pIoctl, PowerDownPending);
            break;

        case DTA_IOCTL_GET_DEV_SUBTYPE:
            pOutBuf->m_GetDevSubType.m_SubType = pDvcData->m_DevInfo.m_SubType;
            break;

        case DTA_IOCTL_GET_PROPERTY2:
            // Get for specific type or for the attached devices
            if (pInBuf->m_GetProperty2.m_TypeNumber == -1)
            {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;

                // Get the property for the current device
                Status = DtPropertiesGet(&pDvcData->m_PropData,
                                                    pInBuf->m_GetProperty2.m_Name,
                                                    pInBuf->m_GetProperty2.m_PortIndex,
                                                    &Value, &Type, &Scope,
                                                    pInBuf->m_GetProperty2.m_DtapiMaj,
                                                    pInBuf->m_GetProperty2.m_DtapiMin,
                                                    pInBuf->m_GetProperty2.m_DtapiBugfix);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            } else {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;
                
                // Property for a specific type was requested
                Status = DtPropertiesGetForType("DTA",
                                                pInBuf->m_GetProperty2.m_TypeNumber,
                                                pDvcData->m_PropData.m_SubDvc,
                                                pInBuf->m_GetProperty2.m_HardwareRevision,
                                                pInBuf->m_GetProperty2.m_FirmwareVersion,
                                                pInBuf->m_GetProperty2.m_Name,
                                                pInBuf->m_GetProperty2.m_PortIndex,
                                                &Value, &Type, &Scope,
                                                pInBuf->m_GetProperty2.m_DtapiMaj,
                                                pInBuf->m_GetProperty2.m_DtapiMin,
                                                pInBuf->m_GetProperty2.m_DtapiBugfix);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            }
            
            if (DT_SUCCESS(Status))
            {
                DT_ASSERT((pOutBuf->m_GetProperty.m_Scope&PROPERTY_SCOPE_DTAPI) ==
                                                                    PROPERTY_SCOPE_DTAPI);
            }
            break;

        case DTA_IOCTL_GET_DEV_GENLOCKSTATE:
            {
                Int  State = DTA_GENLOCKSTATE_NO_REF;
                Int  RefVidStd = DT_VIDSTD_UNKNOWN;
                Status = DtaDeviceGenlockIoctl(pDvcData, &State, &RefVidStd);
                if (DT_SUCCESS(Status))
                {
                    pOutBuf->m_GetGenlockstate.m_State = State;
                    pOutBuf->m_GetGenlockstate.m_RefVidStd = RefVidStd;
                }
            }
            break;

        case DTA_IOCTL_GET_PROPERTY3:
            // Get for specific type or for the attached devices
            if (pInBuf->m_GetProperty3.m_TypeNumber==-1 || 
                                                      pInBuf->m_GetProperty3.m_SubDvc==-1)
            {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;

                // Get the property for the current device
                Status = DtPropertiesGet(&pDvcData->m_PropData,
                                                    pInBuf->m_GetProperty3.m_Name,
                                                    pInBuf->m_GetProperty3.m_PortIndex,
                                                    &Value, &Type, &Scope,
                                                    pInBuf->m_GetProperty3.m_DtapiMaj,
                                                    pInBuf->m_GetProperty3.m_DtapiMin,
                                                    pInBuf->m_GetProperty3.m_DtapiBugfix);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            } else {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;
                
                // Property for a specific type was requested
                Status = DtPropertiesGetForType("DTA",
                                                pInBuf->m_GetProperty3.m_TypeNumber,
                                                pInBuf->m_GetProperty3.m_SubDvc,
                                                pInBuf->m_GetProperty3.m_HardwareRevision,
                                                pInBuf->m_GetProperty3.m_FirmwareVersion,
                                                pInBuf->m_GetProperty3.m_Name,
                                                pInBuf->m_GetProperty3.m_PortIndex,
                                                &Value, &Type, &Scope,
                                                pInBuf->m_GetProperty3.m_DtapiMaj,
                                                pInBuf->m_GetProperty3.m_DtapiMin,
                                                pInBuf->m_GetProperty3.m_DtapiBugfix);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            }
            
            if (DT_SUCCESS(Status))
            {
                DT_ASSERT((pOutBuf->m_GetProperty.m_Scope&PROPERTY_SCOPE_DTAPI) ==
                                                                    PROPERTY_SCOPE_DTAPI);
            }
            break;

        case DTA_IOCTL_RS422_CMD:
            Status = DtaRs422Ioctl(pDvcData, pFile, pIoctl, PowerDownPending);
            break;

        default:
            Status = DT_STATUS_NOT_SUPPORTED;
            break;
        }
    }
    
    // If we failed, no data has te be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED) 
            DtDbgOut(MIN, DTA, "Ioctl: %xh NOT SUPPORTED", pIoctl->m_IoctlCode);
        else if (pIoctl->m_IoctlCode==DTA_IOCTL_VPD_CMD || Status==DT_STATUS_IO_PENDING)
            DtDbgOut(MAX, DTA, "Ioctl: %xh ERROR %xh", pIoctl->m_IoctlCode, Status);
        else
            DtDbgOut(MIN, DTA, "Ioctl: %xh ERROR %xh", pIoctl->m_IoctlCode, Status);
    }
    
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceIoctlChild -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called for every IO request a child driver sends to this driver.
// This function handles all the functional requests. If the request is a DMA transfer,
// it should be queued to the DMA channel queue. This queue must be power managed if
// supported by the host implementation.
//
DtStatus  DtaDeviceIoctlChild(DtaChildDeviceData* pDvcData,
                                               DtFileObject* pFile, DtIoctlObject* pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pIoctlStr = NULL;
    UInt  InReqSize = 0;
    UInt  OutReqSize = 0;
    DtaIoctlInputData*  pInBuf = (DtaIoctlInputData*)pIoctl->m_pInputBuffer;
    DtaIoctlOutputData*  pOutBuf = (DtaIoctlOutputData*)pIoctl->m_pOutputBuffer;
    Int  IpPortIndex = -1;
    Int  PortIndex = -1;
    DtaDeviceData*  pParentDeviceData = pDvcData->m_pParentDeviceData;
    
    // Get minimal input/output buffer sizes
    switch (pIoctl->m_IoctlCode)
    {
    case DTA_IOCTL_GET_DRIVER_VERSION:
        pIoctlStr = "DTA_IOCTL_GET_DRIVER_VERSION";
        InReqSize = 0;
        OutReqSize = sizeof(DtaIoctlGetDriverVersionOutput);
        break;
    case DTA_IOCTL_GET_PROPERTY:
        pIoctlStr = "DTA_IOCTL_GET_PROPERTY";
        InReqSize = sizeof(DtaIoctlGetPropertyInput);
        OutReqSize = sizeof(DtaIoctlGetPropertyOutput);
        break;
    case DTA_IOCTL_GET_DEV_INFO:
        pIoctlStr = "DTA_IOCTL_GET_DEV_INFO";
        InReqSize = 0;
        OutReqSize = sizeof(DtaIoctlGetDevInfoOutput);
        break;
    case DTA_IOCTL_GET_DEV_INFO2:
        pIoctlStr = "DTA_IOCTL_GET_DEV_INFO2";
        InReqSize = 0;
        OutReqSize = sizeof(DtaIoctlGetDevInfoOutput2);
        break;
    case DTA_IOCTL_PHYMAC_CMD:
        pIoctlStr = "DTA_IOCTL_PHYMAC_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTA_IOCTL_IP_NW_CMD:
        pIoctlStr = "DTA_IOCTL_IP_NW_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTA_IOCTL_GET_DEV_SUBTYPE:
        pIoctlStr = "DTA_IOCTL_GET_DEV_SUBTYPE";
        InReqSize = 0;
        OutReqSize = sizeof(DtaIoctlGetDevSubTypeOutput);
        break;
    default:
        Status = DT_STATUS_NOT_SUPPORTED;
        break;
    }

    // Check if buffer sizes satisfy
    if (DT_SUCCESS(Status))
    {
        if (pIoctl->m_InputBufferSize < InReqSize) 
        {
            DtDbgOut(ERR, DTA, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) INPUT BUFFER TOO SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
            Status = DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize) 
        {
            DtDbgOut(ERR, DTA, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) OUTPUT BUFFER TOO"
                                          " SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
            Status = DT_STATUS_INVALID_PARAMETER;
        }
        if (DT_SUCCESS(Status))
            DtDbgOut(MAX, DTA, "%s: In=%d (Rq=%d) Out=%d (Rq=%d)",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;
    
    // Handle IOCTLS
    if (DT_SUCCESS(Status))
    {
        switch (pIoctl->m_IoctlCode)
        {
        case DTA_IOCTL_GET_DRIVER_VERSION:
            pOutBuf->m_GetDriverVersion.m_Major = DTA_VERSION_MAJOR;
            pOutBuf->m_GetDriverVersion.m_Minor = DTA_VERSION_MINOR;
            pOutBuf->m_GetDriverVersion.m_Micro = DTA_VERSION_MICRO;
            pOutBuf->m_GetDriverVersion.m_Build = DTA_VERSION_BUILD;
            break;
        
        case DTA_IOCTL_GET_PROPERTY:
            // The index of the child devices is the network port index. Convert to
            // port index.
            {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;

                IpPortIndex = pInBuf->m_GetProperty.m_PortIndex;
                if (IpPortIndex >= pParentDeviceData->m_NumIpPorts)
                    return DT_STATUS_INVALID_PARAMETER;
            
                PortIndex = 
                        pParentDeviceData->m_IpDevice.m_pIpPorts[IpPortIndex].m_PortIndex;
                Status = DtPropertiesGet(&pParentDeviceData->m_PropData, 
                                                             pInBuf->m_GetProperty.m_Name,
                                                             PortIndex,
                                                             &Value, &Type, &Scope,
                                                             -1, -1, -1);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            }
            if (DT_SUCCESS(Status))
            {
                DT_ASSERT((pOutBuf->m_GetProperty.m_Scope&PROPERTY_SCOPE_DRIVER) ==
                                                                   PROPERTY_SCOPE_DRIVER);
            }
            break;
        case DTA_IOCTL_GET_DEV_INFO:
            pOutBuf->m_GetDevInfo.m_DeviceId = pParentDeviceData->m_DevInfo.m_DeviceId;
            pOutBuf->m_GetDevInfo.m_VendorId = pParentDeviceData->m_DevInfo.m_VendorId;
            pOutBuf->m_GetDevInfo.m_SubSystemId = 
                                               pParentDeviceData->m_DevInfo.m_SubSystemId;
            pOutBuf->m_GetDevInfo.m_SubVendorId = 
                                               pParentDeviceData->m_DevInfo.m_SubVendorId;
            pOutBuf->m_GetDevInfo.m_BusNumber = pParentDeviceData->m_DevInfo.m_BusNumber;
            pOutBuf->m_GetDevInfo.m_SlotNumber = 
                                                pParentDeviceData->m_DevInfo.m_SlotNumber;
            pOutBuf->m_GetDevInfo.m_TypeNumber = 
                                                pParentDeviceData->m_DevInfo.m_TypeNumber;
            pOutBuf->m_GetDevInfo.m_HardwareRevision =
                                          pParentDeviceData->m_DevInfo.m_HardwareRevision;
            pOutBuf->m_GetDevInfo.m_FirmwareVersion =
                                           pParentDeviceData->m_DevInfo.m_FirmwareVersion;
            pOutBuf->m_GetDevInfo.m_FirmwareVariant =
                                           pParentDeviceData->m_DevInfo.m_FirmwareVariant;
            pOutBuf->m_GetDevInfo.m_Serial = pParentDeviceData->m_DevInfo.m_Serial;
            break;

        case DTA_IOCTL_GET_DEV_INFO2:
            pOutBuf->m_GetDevInfo2.m_DeviceId = pParentDeviceData->m_DevInfo.m_DeviceId;
            pOutBuf->m_GetDevInfo2.m_VendorId = pParentDeviceData->m_DevInfo.m_VendorId;
            pOutBuf->m_GetDevInfo2.m_SubSystemId = 
                                               pParentDeviceData->m_DevInfo.m_SubSystemId;
            pOutBuf->m_GetDevInfo2.m_SubVendorId = 
                                               pParentDeviceData->m_DevInfo.m_SubVendorId;
            pOutBuf->m_GetDevInfo2.m_BusNumber = pParentDeviceData->m_DevInfo.m_BusNumber;
            pOutBuf->m_GetDevInfo2.m_SlotNumber = 
                                                pParentDeviceData->m_DevInfo.m_SlotNumber;
            pOutBuf->m_GetDevInfo2.m_TypeNumber = 
                                                pParentDeviceData->m_DevInfo.m_TypeNumber;
            pOutBuf->m_GetDevInfo2.m_HardwareRevision =
                                          pParentDeviceData->m_DevInfo.m_HardwareRevision;
            pOutBuf->m_GetDevInfo2.m_FirmwareVersion =
                                           pParentDeviceData->m_DevInfo.m_FirmwareVersion;
            pOutBuf->m_GetDevInfo2.m_FirmwareVariant =
                                           pParentDeviceData->m_DevInfo.m_FirmwareVariant;
            pOutBuf->m_GetDevInfo2.m_Serial = pParentDeviceData->m_DevInfo.m_Serial;
            break;

        case DTA_IOCTL_PHYMAC_CMD:
            Status = DtaPhyMacIoctl(pParentDeviceData, pIoctl);
            break;

        case DTA_IOCTL_IP_NW_CMD:
            Status = DtaIpNwIoctl(pParentDeviceData, pIoctl);
            break;

        case DTA_IOCTL_GET_DEV_SUBTYPE:
            pOutBuf->m_GetDevSubType.m_SubType = pParentDeviceData->m_DevInfo.m_SubType;
            break;

        default:
            Status = DT_STATUS_NOT_SUPPORTED;
            break;
        }
    }
    
    // If we failed, no data has te be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED) 
            DtDbgOut(MIN, DTA, "Ioctl: %xh NOT SUPPORTED", pIoctl->m_IoctlCode);
        else
            DtDbgOut(MIN, DTA, "Ioctl: %xh ERROR %xh", pIoctl->m_IoctlCode,  Status);
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaDeviceInterrupt(DtaDeviceData* pDvcData)
{
    Bool  IrqHandled = FALSE;
    DtDpcArgs  DpcArgs;
    Int  i, j;
    Bool  FoundGenlockRef = FALSE;
    UInt32  PlxIntCsr = 0;
    UInt8*  pPci905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;
    Bool  UseGenregs = USES_GENREGS(pDvcData);

    // If a PLX chip is used, we can check if an interrupt is pending without looking at
    // every status bit
    if (pDvcData->m_DevInfo.m_UsesPlxChip)
    {
        // The interrupt is cleared automatically when reading the interrupt status 
        // register
        PlxIntCsr = Pci905xIntCtrlStatGet(pPci905XRegs);

        // Check for master PCI interrupt enable
        if ((PlxIntCsr & PCI905X_INTCSR_PCI_INTEN) == 0)
            return FALSE; // Can't be ours
    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Periodic interrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // 
    if ((UseGenregs && DtaRegGenStatGetPerInt(pDvcData->m_pGenRegs)!=0)
        || (!UseGenregs && pDvcData->m_pNonIpPorts!=NULL &&
                           DtaRegTxStatGetPerInt(pDvcData->m_pNonIpPorts[0].m_pTxRegs)!=0)
        || (!UseGenregs && (pDvcData->m_DevInfo.m_TypeNumber==122) && 
                          pDvcData->m_pNonIpPorts!=NULL &&
                          DtaRegRxStatGetPerInt(pDvcData->m_pNonIpPorts[0].m_pRxRegs)!=0))
    {
        // Clear periodic interrupt
        if (UseGenregs)
            DtaRegGenStatClrPerInt(pDvcData->m_pGenRegs);
        // Special cases for old cards with old register layout
        else if (pDvcData->m_DevInfo.m_TypeNumber==122 && pDvcData->m_pNonIpPorts!=NULL)
            DtaRegRxStatClrPerInt(pDvcData->m_pNonIpPorts[0].m_pRxRegs);
        else if (pDvcData->m_pNonIpPorts!=NULL)
            DtaRegTxStatClrPerInt(pDvcData->m_pNonIpPorts[0].m_pTxRegs);

        //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Genlock processing -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        //
        // NOTE: 
        //  Only execute code for boards that support genlocking and use a 145/2144
        //  genlocking architecture

        if (pDvcData->m_Genlock.m_IsSupported && 
                                (pDvcData->m_Genlock.m_GenlArch==DTA_GENLOCK_ARCH_145 
                                || pDvcData->m_Genlock.m_GenlArch==DTA_GENLOCK_ARCH_2144))
        {
            // Check for reference channels
            FoundGenlockRef = FALSE;
            for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
            {
                if (pDvcData->m_pNonIpPorts[i].m_IoCfg[DT_IOCONFIG_GENREF].m_Value 
                                                                      == DT_IOCONFIG_TRUE)
                {
                    DtaFpgaGenlockUpdateTiming(&pDvcData->m_Genlock.m_FpgaGenlock);
                    DtaFpgaGenlockUpdateRx(&pDvcData->m_Genlock.m_FpgaGenlock, i);
                    FoundGenlockRef = TRUE;
                    break;  // Only need to call methods above once 
                }
            }

            // Check for genlocked outputs
            for (i=0; i<pDvcData->m_NumNonIpPorts && FoundGenlockRef; i++)
            {
                if (pDvcData->m_pNonIpPorts[i].m_IoCfg[DT_IOCONFIG_GENLOCKED].m_Value 
                                                                      == DT_IOCONFIG_TRUE)
                {
                    DtaFpgaGenlockUpdateTx(&pDvcData->m_Genlock.m_FpgaGenlock, i);
                    break;  // Only need to call methods above once 
                }
            }
            // note: revise code above for crossboard genlock
        }

        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Failsafe processing -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

        for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
        {
            // Skip ports with Matrix-API interface; below code does not apply to them
            if (pDvcData->m_pNonIpPorts[i].m_CapMatrix)
                continue;

            // Toggle failsafe watchdog
            if (pDvcData->m_pNonIpPorts[i].m_CapFailSafe)
            {
                if (pDvcData->m_pNonIpPorts[i].m_IoCfg[DT_IOCONFIG_FAILSAFE].m_Value 
                                                                    == DT_IOCONFIG_TRUE)
                {
                    // Check if application is still alive
                    if ((pDvcData->m_pNonIpPorts[i].m_FailsafeTimeoutCnt > 
                                            pDvcData->m_pNonIpPorts[i].m_FailsafeCnt) && 
                                            (pDvcData->m_pNonIpPorts[i].m_FailsafeEnable))
                    {
                        DtAtomicIncrement(&pDvcData->m_pNonIpPorts[i].m_FailsafeCnt);
                        DtaWatchdogPulse(pDvcData->m_pGenRegs);
                    } // else: Failsafe not enabled or Application did not respond within
                      //       time or Failsafe disabled
                } else
                    // No failsafe enabled, toggle always
                    DtaWatchdogPulse(pDvcData->m_pGenRegs);
            }
            
            if (pDvcData->m_pNonIpPorts[i].m_IoCfg[DT_IOCONFIG_IODIR].m_Value == 
                                                                      DT_IOCONFIG_OUTPUT)
            {
                // If the output is using an external clock, and the FW does not 
                // support the external clock frequency register, we need to estimate the
                // external clock.
                if (pDvcData->m_pNonIpPorts[i].m_CapExtTsRate
                      && !pDvcData->m_DevInfo.m_HasExtClockFreq
                      && pDvcData->m_pNonIpPorts[i].m_IoCfg[DT_IOCONFIG_TSRATESEL].m_Value
                                                                 == DT_IOCONFIG_EXTTSRATE)
                {
                    // Read input clock freq counter register. Value is processed in DPC
                    pDvcData->m_pNonIpPorts[i].m_BitrateMeasure.m_ValidCountSample =
                                DtaRegTxClkCountGet(pDvcData->m_pNonIpPorts[i].m_pTxRegs);
                }
            }

            if (pDvcData->m_pNonIpPorts[i].m_IoCfg[DT_IOCONFIG_IODIR].m_Value == 
                                                                       DT_IOCONFIG_INPUT)
            {
                // Read valid counter register. Value is processed in DPC
                pDvcData->m_pNonIpPorts[i].m_BitrateMeasure.m_ValidCountSample =
                              DtaRegRxValidCountGet(pDvcData->m_pNonIpPorts[i].m_pRxRegs);
            }
        }

        // For now always schedule general periodic interrupt DPC
        DpcArgs.m_pContext = pDvcData;
        DtDpcSchedule(&pDvcData->m_GenPerIntDpc, &DpcArgs);

        if (pDvcData->m_NumIpPorts != 0)
        {
            DpcArgs.m_pContext = &pDvcData->m_IpDevice;

            if (pDvcData->m_IpDevice.m_PortType == DTA_IPPORT_TYPE1)
                // Schedule a DPC to handle IpRtTx packets for all ports and channels
                DtDpcSchedule(&pDvcData->m_IpDevice.m_IpRtTxDpc, &DpcArgs);

            // Schedule a DPC to handle bit rate measurements for all channels
            DtDpcSchedule(&pDvcData->m_IpDevice.m_IpRtRxBrmDpc, &DpcArgs);
        }

        // Interrupt was ours
        IrqHandled = TRUE;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I2C interrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Check for device level I2c interrupts
    if (pDvcData->m_I2c.m_IsSupported && 
                                  DtaRegI2cStatusGetRdyInt(pDvcData->m_I2c.m_pI2cRegs)!=0)
    {   
        // This is the I2C interrupt. Clear it
        DtaRegI2cStatusClrRdyInt(pDvcData->m_I2c.m_pI2cRegs);
        
        // Schedule to DPC
        DpcArgs.m_pContext = &pDvcData->m_I2c;
        DtDpcSchedule(&pDvcData->m_I2c.m_I2cCompletedDpc, &DpcArgs);

        // Interrupt was ours
        IrqHandled = TRUE;
    }

    // Check for port level I2c interrupts
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        // Check port support matrix
        if (!pDvcData->m_pNonIpPorts[i].m_I2c.m_IsSupported)
            continue;

        if (DtaRegI2cStatusGetRdyInt(pDvcData->m_pNonIpPorts[i].m_I2c.m_pI2cRegs)!=0)
        {
            // This is the I2C interrupt. Clear it
            DtaRegI2cStatusClrRdyInt(pDvcData->m_pNonIpPorts[i].m_I2c.m_pI2cRegs);

            // Schedule to DPC
            DpcArgs.m_pContext = &pDvcData->m_pNonIpPorts[i].m_I2c;
            DtDpcSchedule(&pDvcData->m_pNonIpPorts[i].m_I2c.m_I2cCompletedDpc, 
                                                                            &DpcArgs);
            // Interrupt was ours
            IrqHandled = TRUE;
        }
    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Matrix (HD-SDI) interrupts -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    //
    if (pDvcData->m_Matrix.m_IsSupported)
    {
        // Check for SOF (start-of-frame) interrupt
        if ((pDvcData->m_Genlock.m_GenlArch==DTA_GENLOCK_ARCH_2152 ||
                          pDvcData->m_Genlock.m_GenlArch==DTA_GENLOCK_ARCH_2154) &&
                          DtaRegHdGenlStatGetSofInt(pDvcData->m_Genlock.m_pGenlRegs) != 0)
        {
            // Call handler. NOTE: the handler will clear the interrupt flag
            DtaInterruptSofIntHandler(pDvcData);

            // Interrupt was ours
            IrqHandled = TRUE;
        }

        // Check for channel channel Frame interrputs
        for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
        {
            // Check port support matrix
            if (!pDvcData->m_pNonIpPorts[i].m_CapMatrix)
                continue;
            if (DtaRegHdStatGetLastFrameInt(pDvcData->m_pNonIpPorts[i].m_pRxRegs) != 0)
            {
                // Call handler. NOTE: the handler will clear the interrupt flag
                DtaInterruptLastFrameIntHandler(&pDvcData->m_pNonIpPorts[i]);
                // Interrupt was ours
                IrqHandled = TRUE;
            }

            if (DtaRegHdStatGetRxSyncErrInt(pDvcData->m_pNonIpPorts[i].m_pRxRegs) != 0 ||
                     DtaRegHdStatGetRxOvfErrInt(pDvcData->m_pNonIpPorts[i].m_pRxRegs) != 0)
            {
                DtDpcArgs  DpcArgs;
                DpcArgs.m_pContext = &pDvcData->m_pNonIpPorts[i];
                // Schedule DPC to handle the interrupt since we need to acquire
                // a spinlock to store the flags. The DPC will also clear the interrupt
                // flag.
                DtDpcSchedule(&pDvcData->m_pNonIpPorts[i].m_Matrix.m_RxErrIntDpc,&DpcArgs);
                // Interrupt was ours
                IrqHandled = TRUE;
            }
        }
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- RS-422 interrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    //
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        if (pDvcData->m_pNonIpPorts[i].m_CapRs422)
        {
            if (DtaRegRs422StatGetTxReadyInt(pDvcData->m_pNonIpPorts[i].m_pRs422Regs) || 
                DtaRegRs422StatGetRxDataAvailInt(pDvcData->m_pNonIpPorts[i].m_pRs422Regs))
            {
                DtDpcArgs  DpcArgs;
                DpcArgs.m_pContext = &pDvcData->m_pNonIpPorts[i];
                // Schedule DPC to handle the interrupt. The DPC will clear the int flag.
                DtDpcSchedule(&pDvcData->m_pNonIpPorts[i].m_Rs422.m_IntDpc, &DpcArgs);
                // Interrupt was ours
                IrqHandled = TRUE;
            }
        }
    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DMA interrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    //
    if (pDvcData->m_DmaOptions.m_UseDmaInFpga) 
    {
        UInt32  CmdStat;
        DmaChannel*  pDmaChannel = NULL;
        
        for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
        {
            // Skip non-functional ports
            if (pDvcData->m_pNonIpPorts[i].m_IsNonFuntional)
                continue;

            pDmaChannel = &pDvcData->m_pNonIpPorts[i].m_DmaChannel;
            // Check if a DMA interrupt is pending
            CmdStat = READ_UINT32(pDmaChannel->m_pRegCmdStat, 0);
            if ((CmdStat & DTA_DMA_CMDSTAT_INTACT) != 0)
            {
                // Clear DMA Channel interrupt (disables the channel, too)
                WRITE_UINT32(pDmaChannel->m_pRegCmdStat, 0, DTA_DMA_CMDSTAT_INTACT);
                
                DpcArgs.m_pContext = pDmaChannel;
                DtDpcSchedule(&pDmaChannel->m_DmaCompletedDpc, &DpcArgs);

                // Interrupt was ours
                IrqHandled = TRUE;
            } else if ((CmdStat & DTA_DMA_CMDSTAT_DONE)!=0 && 
                          DtaDmaIsAbortActive(pDmaChannel) && !DtaDmaIsReady(pDmaChannel))
            {
                // No DMA interrupt active, but DMA is ready and still pending in driver.
                DpcArgs.m_pContext = pDmaChannel;
                DtDpcSchedule(&pDmaChannel->m_DmaCompletedDpc, &DpcArgs);
            }
        }

        for (i=0; i<pDvcData->m_NumIpPorts; i++)
        {      
            DtaIpPort*  pIpPort = &pDvcData->m_IpDevice.m_pIpPorts[i];
            for (j=0; j<pIpPort->m_NumDmaChannelsIsr; j++)
            {
                pDmaChannel = pIpPort->m_pDmaChannelsIsr[j];
            
                // Check if a DMA interrupt is pending
                CmdStat = READ_UINT32(pDmaChannel->m_pRegCmdStat, 0);
                if ((CmdStat & DTA_DMA_CMDSTAT_INTACT) != 0)
                {
                    // Clear DMA Channel interrupt (disables the channel too)
                    WRITE_UINT32(pDmaChannel->m_pRegCmdStat, 0, DTA_DMA_CMDSTAT_INTACT);
                    
                    DpcArgs.m_pContext = pDmaChannel;
                    if ((pDmaChannel->m_DmaMode & DTA_DMA_MODE_TIMEOUT_ENABLE) != 0)
                        pDmaChannel->m_NumBytesRead = pDmaChannel->m_TransferSize - 
                                             ((CmdStat & DTA_DMA_CMDSTAT_SIZE_MASK) >> 8);
                    DtDpcSchedule(&pDmaChannel->m_DmaCompletedDpc, &DpcArgs);

                    // Interrupt was ours
                    IrqHandled = TRUE;
                } else if ((CmdStat & DTA_DMA_CMDSTAT_DONE)!=0 && 
                          DtaDmaIsAbortActive(pDmaChannel) && !DtaDmaIsReady(pDmaChannel))
                {
                    // No DMA interrupt active, but DMA is ready and still pending in driver.
                    DpcArgs.m_pContext = pDmaChannel;
                    if ((pDmaChannel->m_DmaMode & DTA_DMA_MODE_TIMEOUT_ENABLE) != 0)
                        pDmaChannel->m_NumBytesRead = pDmaChannel->m_TransferSize - 
                                             ((CmdStat & DTA_DMA_CMDSTAT_SIZE_MASK) >> 8);
                    DtDpcSchedule(&pDmaChannel->m_DmaCompletedDpc, &DpcArgs);
                }
            }
        }
    } else {
        DmaChannel*  pDmaChannel = NULL;
        DtDpcArgs  DpcArgs;
            
        DT_ASSERT(pDvcData->m_DevInfo.m_UsesPlxChip);
        // Check if a DMA interrupt is pending
        if ((PlxIntCsr & PCI905X_INTCSR_DMA0_INTACT) != 0)
        {   
            // Clear DMA Channel interrupt
            Pci905xCmdStatDma0ClearInt(pPci905XRegs);
            if (DT_SUCCESS(DtaDmaGetDmaChannelPlx(pDvcData, 0, &pDmaChannel)))
            {
                DpcArgs.m_pContext = pDmaChannel;
                DtDpcSchedule(&pDmaChannel->m_DmaCompletedDpc, &DpcArgs);
            }

            // Interrupt was ours
            IrqHandled = TRUE;
        }
        if ((PlxIntCsr & PCI905X_INTCSR_DMA1_INTACT) != 0)
        {
            // Clear DMA Channel interrupt
            Pci905xCmdStatDma1ClearInt(pPci905XRegs);
            if (DT_SUCCESS(DtaDmaGetDmaChannelPlx(pDvcData, 1, &pDmaChannel)))
            {
                DpcArgs.m_pContext = pDmaChannel;
                DtDpcSchedule(&pDmaChannel->m_DmaCompletedDpc, &DpcArgs);
            }
            // Interrupt was ours
            IrqHandled = TRUE;
        }
    }

    for (i=0; i<pDvcData->m_NumIpPorts; i++) 
    {
        DtaIpPort*  pIpPort = &pDvcData->m_IpDevice.m_pIpPorts[i];
        // Check PHY interrupt
        if (DtaNwStatGetMdioInt(pIpPort->m_pGenNwRegs)!=0 && 
                                          DtaNwCtrlGetMdioIntEn(pIpPort->m_pGenNwRegs)!=0)
        {
            // PHY interrupt occured.
            // Disable interrupt enable-bit because the interrupt must be cleared
            // later in the PHY and will be enabled again there.
            DtaNwCtrlSetMdioIntEn(pIpPort->m_pGenNwRegs, 0);

            // We schedule a DPC to handle the interrupt
            DpcArgs.m_pContext = &pIpPort->m_PhyMac;
            DtDpcSchedule(&pIpPort->m_PhyMac.m_PhyIntDpc, &DpcArgs);

            // Interrupt was ours
            IrqHandled = TRUE;
        }

        // Check receive status network interrupts
        if (pIpPort->m_PortType == DTA_IPPORT_TYPE1) 
        {
            UInt32  RxStat;
            // Due to a bug in firmware the IpRxFifoOvfInt can be set when disabled
            // So clear this flag if enabled
            RxStat = DtaNwRxStatGet(pIpPort->m_IpPortType1.m_Rx.m_pRegs);
            RxStat = RxStat & (DTA_NWRX_STAT_VAL_CNT_OVF_INT | 
                                                           DTA_NWRX_STAT_INV_CNT_OVF_INT |
                                                           DTA_NWRX_STAT_RX_FIFO_OVF_INT);
            if ((RxStat & DTA_NWRX_STAT_VAL_CNT_OVF_INT) != 0)
                pIpPort->m_NumRxFifoOverflow++;
            
            // Reset the receive status interrupts
            DtaNwRxStatSet(pIpPort->m_IpPortType1.m_Rx.m_pRegs, RxStat);
            
            // Do not handle these interrupts if they are supposed to be disabled.
            // Note: See also issue D120.            
        }
        else if (pIpPort->m_IpPortType2.m_InterruptsEnabled) 
        {
            UInt32  RxStat;
            UInt32  TxStat;
            Bool  SliceOverflow = FALSE;
            RxStat = DtaNwRxStatGet(pIpPort->m_IpPortType2.m_RxRt.m_pRegs);
            RxStat = RxStat & (DTA_NWRX_STAT_VAL_CNT_OVF_INT
                                                           | DTA_NWRX_STAT_INV_CNT_OVF_INT
                                                           | DTA_NWRX_STAT_RX_FIFO_OVF_INT
                                                           | DTA_NWRX_STAT_SLICE_AVAIL_INT
                                                           | DTA_NWRX_STAT_SLICE_OVF_INT);
            if ((RxStat & DTA_NWRX_STAT_SLICE_OVF_INT) != 0)
            {
                SliceOverflow = TRUE;
                pIpPort->m_NumRxFifoOverflow++;
            }

            if ((RxStat & DTA_NWRX_STAT_SLICE_AVAIL_INT) != 0)
                DtaIpRxRtUpdateSlicePointer(pIpPort, SliceOverflow);
            
            // Reset the receive status interrupts
            DtaNwRxStatSet(pIpPort->m_IpPortType2.m_RxRt.m_pRegs, RxStat);


            TxStat = DtaNwTxStatGet(pIpPort->m_IpPortType2.m_TxRt.m_pRegs);
            TxStat = TxStat & DTA_NWTX_STAT_DMA_READY_INT;
            // Check IpTx Slice interrupt / DMA ready
            if ((TxStat & DTA_NWTX_STAT_DMA_READY_INT) != 0)
            {
                DpcArgs.m_pContext = pIpPort;
                DtDpcSchedule(pIpPort->m_IpPortType2.m_pIpRtTxDpc, &DpcArgs);
                // This interrupt is enabled, so we have to report it.
                IrqHandled = TRUE;
            }

            // Reset the transmit status interrupts
            DtaNwTxStatSet(pIpPort->m_IpPortType2.m_TxRt.m_pRegs, TxStat);
        }
    }
    return IrqHandled;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaInterruptSofIntHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaInterruptSofIntHandler(DtaDeviceData* pDvcData)
{
    Int  i;
    DtDpcArgs  DpcArgs;

    DT_ASSERT(pDvcData->m_Matrix.m_IsSupported);
    
    // Latch Reference SOF frame
    pDvcData->m_Matrix.m_SofFrame = DtaRegHdGenlSofFrameGet(
                                                         pDvcData->m_Genlock.m_pGenlRegs);
    
    // Latch channel SOF frame and line registers
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        // Skip if port does not support the Matrix-API interface
        if (!pDvcData->m_pNonIpPorts[i].m_CapMatrix)
            continue;
        pDvcData->m_pNonIpPorts[i].m_Matrix.m_SofFrame = 
                              DtaRegHdSofFrameGet(pDvcData->m_pNonIpPorts[i].m_pRxRegs);
        pDvcData->m_pNonIpPorts[i].m_Matrix.m_SofLine = 
                             DtaRegHdSofLineGetLine(pDvcData->m_pNonIpPorts[i].m_pRxRegs);
    }

    // Schedule DPC for handling of "low-prio" part of SOF interrupt
    DpcArgs.m_pContext = pDvcData;
    DpcArgs.m_Data1.m_UInt64 = (UInt64)pDvcData->m_Matrix.m_SofFrame;
    DtDpcSchedule(&pDvcData->m_Matrix.m_SofFrameIntDpc, &DpcArgs);

    // Finaly: clear SOF interrupt
    DtaRegHdGenlStatClrSofInt(pDvcData->m_Genlock.m_pGenlRegs);
    
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaInterruptLastFrameIntHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaInterruptLastFrameIntHandler(DtaNonIpPort*  pNonIpPort)
{
    DtDpcArgs  DpcArgs;
    Int  NextFrmIdx=0;
    Int  CurFrmIdx = 0;
    Bool  IsRunning = TRUE;
    DtaFrameBufSectionConfig*  pSections = pNonIpPort->m_Matrix.m_BufConfig.m_Sections;

    DT_ASSERT(pNonIpPort->m_CapMatrix);

    // If the port is configured for ASI => "ignore" this interrupt
    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_ASI)
    {
        // Clear interrupt flag and return
        DtaRegHdStatClrLastFrameInt(pNonIpPort->m_pRxRegs);
        return DT_STATUS_OK;
    }
        
    // In the legacy HD-channel register interface, the last/current frame counters are
    // maintained by the hardware as opposed to being maintained in the driver
    if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
    {
        // Store last frame
        //
        // NOTE: Due to a firmware bug hardware does not latch the last frame, but the 
        // current frame => last-frame is effectively the same as the current frame.
        //
        // AS WORKARROUND, we therefore now read the current frame register and subtract 
        // 1 to get the last frame
        pNonIpPort->m_Matrix.m_LastFrame = 
                                       DtaRegHdCurrentFrameGet(pNonIpPort->m_pRxRegs) - 1;
    }
    else
    {
        Int64  NextFrame;
        Bool  ChangeToRun=FALSE, ChangeToHold=FALSE;
        DtaMatrixPortState  NewState = pNonIpPort->m_Matrix.m_State;

        pNonIpPort->m_Matrix.m_LastFrame = pNonIpPort->m_Matrix.m_CurFrame;
        
        // Check for state change
        IsRunning = (pNonIpPort->m_Matrix.m_LastStateAtInt == MATRIX_PORT_RUN);
        ChangeToRun = !IsRunning && (NewState==MATRIX_PORT_RUN);
        ChangeToHold = NewState==MATRIX_PORT_HOLD && 
                                (pNonIpPort->m_Matrix.m_LastStateAtInt!=MATRIX_PORT_HOLD);

        if (ChangeToRun)
        {
            NextFrame = pNonIpPort->m_Matrix.m_NextFrame;
            DT_ASSERT(NextFrame != -1);
            pNonIpPort->m_Matrix.m_CurFrame = NextFrame - 1;
            
            // Clear forced next frame
            pNonIpPort->m_Matrix.m_NextFrame = -1;

            DtDbgOut(AVG, NONIP, "Enter run state; Starting with frame: %lld", NextFrame); 
        }
        else if (NewState == MATRIX_PORT_RUN)
        {
            // Update current frame
            pNonIpPort->m_Matrix.m_CurFrame++;  
            NextFrame = pNonIpPort->m_Matrix.m_CurFrame+1;  // set next
        }
        else
        {
            // Next frame shall be the hold frame (i.e. a black frame) 
            NextFrame = DTA_FRMBUF_HOLD_FRAME;

            if (ChangeToHold)
                DtDbgOut(AVG, NONIP, "Enter hold state; Repeating frame: %lld",
                                                                               NextFrame); 
        }

        if (NewState == MATRIX_PORT_RUN)
        {
            // Set reference clock for current frame
            CurFrmIdx = DtaNonIpMatrixFrame2Index(pNonIpPort,
                                                         pNonIpPort->m_Matrix.m_CurFrame);
            pNonIpPort->m_Matrix.m_FrameInfo[CurFrmIdx].m_RefClk =
                                 DtaRegRefClkCntGet64(pNonIpPort->m_pDvcData->m_pGenRegs);
        }
    
        if (NewState >= MATRIX_PORT_IDLE)
        {
            // Set address for the next frame to be transmited/received
            NextFrmIdx = DtaNonIpMatrixFrame2Index(pNonIpPort, NextFrame);

            DT_ASSERT(pSections[0].m_FrameStartAddr[NextFrmIdx] != -1);
            DtaRegHdS0NextFrmAddrSet(pNonIpPort->m_pRxRegs, 
                                               pSections[0].m_FrameStartAddr[NextFrmIdx]);
            DT_ASSERT(pSections[1].m_FrameStartAddr[NextFrmIdx] != -1);
            DtaRegHdS1NextFrmAddrSet(pNonIpPort->m_pRxRegs, 
                                               pSections[1].m_FrameStartAddr[NextFrmIdx]);
        }

        // Save last state seen by interrupt
        pNonIpPort->m_Matrix.m_LastStateAtInt = NewState;
    }

    // Schedule DPC for handling of "low-prio" part of Last Frame interrupt, burt only if 
    // the channel is in running state
    if (IsRunning)
    {
        DpcArgs.m_pContext = pNonIpPort;
        DpcArgs.m_Data1.m_UInt64 = (UInt64)pNonIpPort->m_Matrix.m_LastFrame;
        DtDpcSchedule(&pNonIpPort->m_Matrix.m_LastFrameIntDpc, &DpcArgs);
    }
    
    // Finaly: clear last frame interrupt
    DtaRegHdStatClrLastFrameInt(pNonIpPort->m_pRxRegs);

    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGetNonIpPortIndex -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaGetNonIpPortIndex(DtaDeviceData* pDvcData,
                                                      Int PortIndex, Int* pNonIpPortIndex)
{
    if (PortIndex >= pDvcData->m_NumPorts)
        return DT_STATUS_NOT_FOUND;
    if (PortIndex < 0)
        return DT_STATUS_NOT_FOUND;
    if (pDvcData->m_pPortLookup[PortIndex].m_PortType != DTA_PORT_TYPE_NONIP)
        return DT_STATUS_NOT_FOUND;

    *pNonIpPortIndex = pDvcData->m_pPortLookup[PortIndex].m_Index;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGetPortIndexNonIp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaGetPortIndexNonIp(DtaDeviceData* pDvcData,
                                                      Int NonIpPortIndex, Int* pPortIndex)
{
    Int i;
    Int NumNonIpPorts = 0;

    if (NonIpPortIndex >= pDvcData->m_NumNonIpPorts)
        return DT_STATUS_NOT_FOUND;
    if (NonIpPortIndex < 0)
        return DT_STATUS_NOT_FOUND;
    
    for (i=0; i<pDvcData->m_NumPorts;i++)
    {
        if (pDvcData->m_pPortLookup[i].m_PortType == DTA_PORT_TYPE_NONIP)
        {
            if (NumNonIpPorts == NonIpPortIndex)
            {
                *pPortIndex = i;
                return DT_STATUS_OK;
            }
            NumNonIpPorts++;
        }
    }
    return DT_STATUS_NOT_FOUND;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGetIpPortIndex -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaGetIpPortIndex(DtaDeviceData* pDvcData, Int PortIndex, Int* pIpPortIndex)
{
    if (PortIndex >= pDvcData->m_NumPorts)
        return DT_STATUS_NOT_FOUND;
    if (PortIndex < 0)
        return DT_STATUS_NOT_FOUND;
    if (pDvcData->m_pPortLookup[PortIndex].m_PortType != DTA_PORT_TYPE_IP)
        return DT_STATUS_NOT_FOUND;

    *pIpPortIndex = pDvcData->m_pPortLookup[PortIndex].m_Index;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGetPortNumber -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaGetPortNumber(DtaDeviceData* pDvcData, Int PortIndex, Int* pPortNumber)
{
    if (PortIndex >= pDvcData->m_NumPorts)
        return DT_STATUS_NOT_FOUND;
    if (PortIndex < 0)
        return DT_STATUS_NOT_FOUND;

    *pPortNumber = pDvcData->m_pPortLookup[PortIndex].m_PortNumber;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGetPortIndexIp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaGetPortIndexIp(DtaDeviceData* pDvcData, Int IpPortIndex, Int* pPortIndex)
{
    Int  i;
    Int  NumIpPorts = 0;
    if (IpPortIndex >= pDvcData->m_NumIpPorts)
        return DT_STATUS_NOT_FOUND;
    if (IpPortIndex < 0)
        return DT_STATUS_NOT_FOUND;
    
    for (i=0; i<pDvcData->m_NumPorts;i++)
    {
        if (pDvcData->m_pPortLookup[i].m_PortType == DTA_PORT_TYPE_IP)
        {
            if (NumIpPorts == IpPortIndex)
            {
                *pPortIndex = i;
                return DT_STATUS_OK;
            }
            NumIpPorts++;
        }
    }
    return DT_STATUS_NOT_FOUND;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGeneralPeriodicIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtaGeneralPeriodicIntDpc(DtDpcArgs* pArgs)
{
    Int  i;
    DtaDeviceData*  pDvcData = (DtaDeviceData*)pArgs->m_pContext;
    DtaIoConfigValue  CfgValue;
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        // Skip ports with Matrix-API interface; below code does not apply to them
        if (pDvcData->m_pNonIpPorts[i].m_CapMatrix)
        {
            DtaNonIpMatrixPeriodicInt(&pDvcData->m_pNonIpPorts[i]);
            continue;
        }

        DtaNonIpIoConfigGet(&pDvcData->m_pNonIpPorts[i], DT_IOCONFIG_IODIR, &CfgValue);
        if (CfgValue.m_Value == DT_IOCONFIG_OUTPUT)
        {
            // Process TX flags
            DtaNonIpTxProcessFlagsFromDpc(&pDvcData->m_pNonIpPorts[i]);

            // If the output is using an external clock, and the FW does not support the
            // external clock frequency register, we need to estimate the external clock.
            if (pDvcData->m_pNonIpPorts[i].m_CapExtTsRate && 
                                                   !pDvcData->m_DevInfo.m_HasExtClockFreq)
            {
                // Check if external clock is selected
                DtaNonIpIoConfigGet(&pDvcData->m_pNonIpPorts[i], DT_IOCONFIG_TSRATESEL,
                                                                               &CfgValue);
                if (CfgValue.m_Value == DT_IOCONFIG_EXTTSRATE)
                    DtaNonIpEstimateRate(&pDvcData->m_pNonIpPorts[i]);
            }
        }

        if (CfgValue.m_Value == DT_IOCONFIG_INPUT)
        {
            // Process RX flags
            DtaNonIpRxProcessFlagsFromDpc(&pDvcData->m_pNonIpPorts[i]);

            // Estimate rate of Transport-Stream input
            DtaNonIpEstimateRate(&pDvcData->m_pNonIpPorts[i]);
        }

        // Target detection
        if (pDvcData->m_DevInfo.m_TypeNumber==102
                                                 || pDvcData->m_DevInfo.m_TypeNumber==122)
            DtaNonIpTargetIdDetectStateMachine(&pDvcData->m_pNonIpPorts[i]);
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaWaitUntilFpgaIsReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Wait until the FPGA is configured.
// If the FPA is not responding within time, a warning message is written in the event log
//
static DtStatus  DtaWaitUntilFpgaIsReady(DtaDeviceData* pDvcData)
{
    UInt32  Timeout = 0;
    UInt  TypeNumber;
    UInt  TypeNumberExt;

    DtSleep(300);

    TypeNumber = DtaRegGenCtrlGetTypeNumber(pDvcData->m_pGenRegs);
    TypeNumberExt = DtaRegGenStatGetTypeNumExt(pDvcData->m_pGenRegs);

    // Wait until type number is read correctly or timeout occurs
    while (TypeNumber!=(pDvcData->m_DevInfo.m_TypeNumber - (TypeNumberExt==1 ? 2100 :
                                           (TypeNumberExt==2 ? 500 : 0))) && Timeout<1000)
    {
        DtSleep(1);
        Timeout++;

        // Read typenumber again
        TypeNumber = DtaRegGenCtrlGetTypeNumber(pDvcData->m_pGenRegs);
        TypeNumberExt = DtaRegGenStatGetTypeNumExt(pDvcData->m_pGenRegs);
    }

    if (Timeout >= 1000)
    {
        DtDbgOut(ERR, DTA, "FPGA DID NOT RESPOND WITHIN 1000ms");
        DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, DTA_LOG_FPGA_TIMEOUT, NULL, NULL,
                                                                                    NULL);
        return DT_STATUS_TIMEOUT;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaInitUserMapping -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaInitUserMapping(DtaDeviceData* pDvcData)
{
    pDvcData->m_pUserMapping = NULL;
    DtFastMutexInit(&pDvcData->m_UserMapMutex);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGetAddressRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Returns a user-space pointer to a register block.
//
DtStatus  DtaGetAddressRegs(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    Int  PortIndex,
    Int  RegsType,
    void*  pPaMmap,              // Virtual address returned by mmap, or NULL if not used.
    Int*  pSize,                 // Size of register block
    void**  ppUserVirtual)
{
    DtStatus  Status;

    if (RegsType != DTA_REGS_TYPE_GEN)
    {
        DtDbgOut(ERR, DTA, "[%d] Invalid RegsType %d", PortIndex, RegsType);
        return DT_STATUS_INVALID_PARAMETER;
    }
    Status = DtaGetAddressRegsForUserspace(pDvcData, pFile, PortIndex, pPaMmap, pSize, 
                                                                           ppUserVirtual);
    if (!DT_SUCCESS(Status))
        return Status;
    DtDbgOut(MIN, DTA, "[%d] RegsType= %d, pUserVirtual= 0x%p Size= %d", PortIndex, 
                                                         RegsType, *ppUserVirtual,*pSize);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaReleaseAddressRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Releases a userspace pointer to a register block.
//
DtStatus  DtaReleaseAddressRegs(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    Int  PortIndex,
    Int  RegsType,
    void**  ppPaMmap,            // Virtual address returned by mmap, or NULL if not used.
    Int*  pSize)                 // Size of register block
{
    if (RegsType != DTA_REGS_TYPE_GEN)
    {
        DtDbgOut(ERR, DTA, "[%d] Invalid RegsType %d", PortIndex, RegsType);
        return DT_STATUS_INVALID_PARAMETER;
    }
    return DtaReleaseAddressRegsForUserspace(pDvcData, pFile, PortIndex, ppPaMmap, pSize);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FindPortNumber -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtaFindPortNumber(DtTableEntry*  pPortMap, Int  PortMapSize, Int  PortIndex)
{
    Int  i;
    for (i=0; i<PortMapSize; i++)
    {
        if (pPortMap[i].m_Y == PortIndex+1)
            return (Int)pPortMap[i].m_X;
    }
    // Fall back to default: PortIndex+1
    return PortIndex + 1;

}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaCalculateAndCreatePortsStruct -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function calculates the number of NonIp and IpPorts, allocates the port structures
// and fills the reverse lookup table
//
DtStatus  DtaCalculateAndCreatePortsStruct(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  PortCount;
    Bool  CapAsi, CapSdi, CapHdSdi, CapSpi, CapIp, CapMod, CapDemod, CapIfAdc, CapGenRef;
    Bool  CapRs422;
    Int  i;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    DtTableEntry  PortMap[16];
    UInt  PortMapSize = 0;
    DtStatus  PortMapStatus;
    
    pDvcData->m_NumNonIpPorts = 0;
    pDvcData->m_NumIpPorts = 0;

    // Get number of ports property
    PortCount = DtPropertiesGetInt(pPropData, "PORT_COUNT", -1);

    PortMapStatus = DtTableGet(pPropData, "SUBDVC_PORT_MAP", -1,
                                                  sizeof(PortMap)/sizeof(PortMap[0]),
                                                  &PortMapSize, PortMap, sizeof(PortMap));
    if (!DT_SUCCESS(PortMapStatus))
        PortMapSize = 0;

    // Allocate memory for PortLookup
    DT_ASSERT(pDvcData->m_pPortLookup == NULL);
    pDvcData->m_pPortLookup = (DtaPortLookup*)
               DtMemAllocPool(DtPoolNonPaged, sizeof(DtaPortLookup) * PortCount, DTA_TAG);
    if (pDvcData->m_pPortLookup == NULL)
        return DT_STATUS_OUT_OF_MEMORY;

    // Initialize reverse lookup structures
    for (i=0; i<PortCount; i++)
    {
        CapAsi = DtPropertiesGetBool(pPropData, "CAP_ASI", i);
        CapSdi = DtPropertiesGetBool(pPropData, "CAP_SDI", i);
        CapHdSdi = DtPropertiesGetBool(pPropData, "CAP_HDSDI", i);
        CapMod = DtPropertiesGetBool(pPropData, "CAP_MOD", i);
        CapDemod = DtPropertiesGetBool(pPropData, "CAP_DEMOD", i);
        CapSpi = DtPropertiesGetBool(pPropData, "CAP_SPI", i);
        CapIp = DtPropertiesGetBool(pPropData, "CAP_IP", i);
        CapIfAdc = DtPropertiesGetBool(pPropData, "CAP_IFADC", i);
        CapGenRef = DtPropertiesGetBool(pPropData, "CAP_GENREF", i);
        CapRs422 = DtPropertiesGetBool(pPropData, "CAP_RS422", i);
        if ((CapAsi || CapSdi || CapHdSdi || CapDemod || CapMod || CapSpi || 
                                             CapIfAdc || CapGenRef || CapRs422) && !CapIp)
        {
            // Setup reverse lookup
            pDvcData->m_pPortLookup[i].m_PortType = DTA_PORT_TYPE_NONIP;
            pDvcData->m_pPortLookup[i].m_Index = pDvcData->m_NumNonIpPorts;
            pDvcData->m_pPortLookup[i].m_PortNumber = DtaFindPortNumber(PortMap,
                                                                          PortMapSize, i);

            // We found one, increment number of non-IP ports
            pDvcData->m_NumNonIpPorts++;
        }
        else if (CapIp)
        {
            // Setup reverse lookup
            pDvcData->m_pPortLookup[i].m_PortType = DTA_PORT_TYPE_IP;
            pDvcData->m_pPortLookup[i].m_Index = pDvcData->m_NumIpPorts;
            pDvcData->m_pPortLookup[i].m_PortNumber = DtaFindPortNumber(PortMap,
                                                                          PortMapSize, i);

            // We found one, increment number of IP ports
            pDvcData->m_NumIpPorts++;
        } else {
            pDvcData->m_NumNonIpPorts = 0;
            pDvcData->m_NumIpPorts = 0;
            PortCount = 0;
            DtDbgOut(ERR, DTA, "[%d] Port type not supported", i);
            Status = DT_STATUS_NOT_SUPPORTED;
            break;
        }
    }
    pDvcData->m_NumPorts = PortCount;
    
    // Allocate memory for NonIp port array
    DT_ASSERT(pDvcData->m_pNonIpPorts == NULL);
    if (pDvcData->m_NumNonIpPorts != 0)
    {
        pDvcData->m_pNonIpPorts = (DtaNonIpPort*)DtMemAllocPool(DtPoolNonPaged, 
                               sizeof(DtaNonIpPort) * pDvcData->m_NumNonIpPorts, DTA_TAG);
        if (pDvcData->m_pNonIpPorts == NULL)
        {
            pDvcData->m_NumNonIpPorts = 0;
            return DT_STATUS_OUT_OF_MEMORY;
        }
        DtMemZero(pDvcData->m_pNonIpPorts, 
                                        sizeof(DtaNonIpPort) * pDvcData->m_NumNonIpPorts);
    }

    // Allocate memory for Ip port array
    DT_ASSERT(pDvcData->m_IpDevice.m_pIpPorts == NULL);
    if (pDvcData->m_NumIpPorts != 0)
    {
        pDvcData->m_IpDevice.m_pIpPorts = (DtaIpPort*)DtMemAllocPool(DtPoolNonPaged, 
                                     sizeof(DtaIpPort) * pDvcData->m_NumIpPorts, DTA_TAG);
        if (pDvcData->m_IpDevice.m_pIpPorts == NULL)
        {
            pDvcData->m_NumIpPorts = 0;
            return DT_STATUS_OUT_OF_MEMORY;
        }
        DtMemZero(pDvcData->m_IpDevice.m_pIpPorts, 
                                              sizeof(DtaIpPort) * pDvcData->m_NumIpPorts);
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaCleanupPortStructs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaCleanupPortStructs(DtaDeviceData* pDvcData)
{
    pDvcData->m_NumNonIpPorts = 0;
    pDvcData->m_NumIpPorts = 0;
    pDvcData->m_NumPorts = 0;

    if (pDvcData->m_pNonIpPorts != NULL)
        DtMemFreePool(pDvcData->m_pNonIpPorts, DTA_TAG);
    pDvcData->m_pNonIpPorts = NULL;
    
    if (pDvcData->m_IpDevice.m_pIpPorts != NULL)
        DtMemFreePool(pDvcData->m_IpDevice.m_pIpPorts, DTA_TAG);
    pDvcData->m_IpDevice.m_pIpPorts = NULL;

    if (pDvcData->m_pPortLookup != NULL)
        DtMemFreePool(pDvcData->m_pPortLookup, DTA_TAG);
    pDvcData->m_pPortLookup = NULL;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceGenlockIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function gets the genlock state and reference video standard and converts
// the architecture specific genlock states to the common genlock states
//
DtStatus  DtaDeviceGenlockIoctl(DtaDeviceData* pDvcData, Int* pState, Int* pRefVidStd)
{
    // Check architecture type
    if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2152)
    {
        // Get and convert genlock state
        DtFastMutexAcquire(&pDvcData->m_Genlock.m_Lmh1982.m_StateMutex);
        DtDbgOut(AVG, DTA, "DtaDeviceGenlockIoctl: Lmh1982 state is %d", pDvcData->m_Genlock.m_Lmh1982.m_State);
        switch (pDvcData->m_Genlock.m_Lmh1982.m_State)
        {
        case DTA_LMH1982_STATE_PLL_LOCKED:
        case DTA_LMH1982_STATE_ALIGN_TOF:
        case DTA_LMH1982_STATE_HOLD_OVER1:
        case DTA_LMH1982_STATE_HOLD_OVER2:
        case DTA_LMH1982_STATE_PLL_LOCKING:
            *pState = DTA_GENLOCKSTATE_LOCKING;
            break;

        case DTA_LMH1982_STATE_FULL_LOCK:
            *pState = DTA_GENLOCKSTATE_LOCKED;
            break;

        case DTA_LMH1982_STATE_INIT:
        case DTA_LMH1982_STATE_NOREF:
        default:
            *pState = DTA_GENLOCKSTATE_NO_REF;
            break;
        }
        DtFastMutexRelease(&pDvcData->m_Genlock.m_Lmh1982.m_StateMutex);
        
        *pRefVidStd = pDvcData->m_Genlock.m_RefVidStd;  // Get reference video standard

        return DT_STATUS_OK;
    }
    else if (pDvcData->m_Genlock.m_GenlArch == DTA_GENLOCK_ARCH_2154)
    {
        // Get and convert genlock state
        DtFastMutexAcquire(&pDvcData->m_Genlock.m_Lmh1983.m_StateMutex);
        DtDbgOut(AVG, DTA, "DtaDeviceGenlockIoctl: Lmh1983 state is %d",
                                                   pDvcData->m_Genlock.m_Lmh1983.m_State);
        switch (pDvcData->m_Genlock.m_Lmh1983.m_State)
        {
        case DTA_LMH1983_STATE_PLL_LOCKED:
        case DTA_LMH1983_STATE_ALIGN_TOF:
        case DTA_LMH1983_STATE_HOLD_OVER1:
        case DTA_LMH1983_STATE_HOLD_OVER2:
        case DTA_LMH1983_STATE_PLL_LOCKING:
            *pState = DTA_GENLOCKSTATE_LOCKING;
            break;

        case DTA_LMH1983_STATE_FULL_LOCK:
            *pState = DTA_GENLOCKSTATE_LOCKED;
            break;

        case DTA_LMH1983_STATE_INIT:
        case DTA_LMH1983_STATE_NOREF:
        default:
            *pState = DTA_GENLOCKSTATE_NO_REF;
            break;
        }
        DtFastMutexRelease(&pDvcData->m_Genlock.m_Lmh1983.m_StateMutex);
        
        *pRefVidStd = pDvcData->m_Genlock.m_RefVidStd;  // Get reference video standard

        return DT_STATUS_OK;
    }

    return DT_STATUS_NOT_SUPPORTED;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPortsCleanUpPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Put all objects in idle state before the OS cleans up the objects when 
// the device exits.
//
DtStatus  DtaPortsCleanUpPre(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i;
    Int  PortIndex;
    
    // Cleanup NonIp port structures
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        if (DT_SUCCESS(DtaGetPortIndexNonIp(pDvcData, i, &PortIndex)))
            DtaNonIpCleanupPre(pDvcData, PortIndex, &pDvcData->m_pNonIpPorts[i]);
        else
            DT_ASSERT(FALSE);
    }

    // Cleanup Ip port structures
    for (i=0; i<pDvcData->m_NumIpPorts; i++)
    {
        if (DT_SUCCESS(DtaGetPortIndexIp(pDvcData, i, &PortIndex)))
            DtaIpCleanupPre(pDvcData, PortIndex, i, &pDvcData->m_IpDevice);
        else
            DT_ASSERT(FALSE);
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPortsCleanUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaPortsCleanUp(DtaDeviceData* pDvcData)
{ 
    Int  i;
    Int  PortIndex;
    
    // Cleanup NonIp port structures
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        if (DT_SUCCESS(DtaGetPortIndexNonIp(pDvcData, i, &PortIndex)))
            DtaNonIpCleanup(pDvcData, PortIndex, &pDvcData->m_pNonIpPorts[i]);
        else
            DT_ASSERT(FALSE);
    }

    // Cleanup Ip port structures
    for (i=0; i<pDvcData->m_NumIpPorts; i++)
    {
        if (DT_SUCCESS(DtaGetPortIndexIp(pDvcData, i, &PortIndex)))
            DtaIpCleanup(pDvcData, PortIndex, i, &pDvcData->m_IpDevice);
        else
            DT_ASSERT(FALSE);
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPortsInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaPortsInit(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i;
    Int  PortIndex;

    // Initialize non-IP port structures
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        if (DT_SUCCESS(DtaGetPortIndexNonIp(pDvcData, i, &PortIndex)))
        {
            Status = DtaNonIpInit(pDvcData, PortIndex, &pDvcData->m_pNonIpPorts[i]);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, DTA, "[%d] Error initialising NonIp Port %i.", i, 
                                                                               PortIndex);
                return Status;
            }
        } else
            DT_ASSERT(FALSE);
    }

    // Initialize IP port structures
    for (i=0; i<pDvcData->m_NumIpPorts; i++)
    {
        if (DT_SUCCESS(DtaGetPortIndexIp(pDvcData, i, &PortIndex)))
        {
            Status = DtaIpInit(pDvcData, PortIndex, i, &pDvcData->m_IpDevice);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, DTA, "[%d] Error initialising Ip Port %i.", PortIndex, i);
                return Status;
            }
        } else
            DT_ASSERT(FALSE);
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPowerupPorts -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaPowerupPorts(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i;

    // Initialize non IP ports HW
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        Status = DtaNonIpPowerup(&pDvcData->m_pNonIpPorts[i]);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTA, "Error initialising NonIp Port %i powerup.", i);
            return Status;
        }
    }

    // Initialize IP ports HW
    for (i=0; i<pDvcData->m_NumIpPorts; i++)
    {
        Status = DtaIpPowerup(&pDvcData->m_IpDevice.m_pIpPorts[i]);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTA, "Error initialising Ip Port %i powerup.", i);
            return Status;
        }
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPowerdownPortsPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Do port powerdown actions before interrupts are disabled
//
DtStatus  DtaPowerdownPortsPre(DtaDeviceData* pDvcData)
{
    Int  i;

    // Powerdown non IP ports HW
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
        DtaNonIpPowerdownPre(&pDvcData->m_pNonIpPorts[i]);
    
    // Powerdown IP ports HW
    for (i=0; i<pDvcData->m_NumIpPorts; i++)
        DtaIpPowerdownPre(&pDvcData->m_IpDevice.m_pIpPorts[i]);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPowerdownPorts -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaPowerdownPorts(DtaDeviceData* pDvcData)
{
    Int  i;

    // Powerdown non IP ports HW
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
        DtaNonIpPowerdown(&pDvcData->m_pNonIpPorts[i]);
    
    // Powerdown IP ports HW
    for (i=0; i<pDvcData->m_NumIpPorts; i++)
        DtaIpPowerdown(&pDvcData->m_IpDevice, &pDvcData->m_IpDevice.m_pIpPorts[i]);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPowerDownDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// If failed try to recover and put the device back in up state
//
DtStatus  DtaPowerDownDevice(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    Status = DtaDevicePowerDownPre(pDvcData);    
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTA, "Error executing DtaDevicePowerDownPre");        
        return Status;
    }
                    
    Status = DtaDeviceInterruptDisable(pDvcData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTA, "Error executing DtaDeviceInterruptDisable");

        if (!DT_SUCCESS(DtaDevicePowerUpPost(pDvcData)))
            DtDbgOut(ERR, DTA, "Error executing DtaDevicePowerUpPost");

        return Status;
    }
        
    Status = DtaDevicePowerDown(pDvcData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTA, "Error executing DtaDevicePowerDown");

        if (!DT_SUCCESS(DtaDeviceInterruptEnable(pDvcData)))
            DtDbgOut(ERR, DTA, "Error executing DtaDeviceInterruptEnable");
        if (!DT_SUCCESS(DtaDevicePowerUpPost(pDvcData)))
            DtDbgOut(ERR, DTA, "Error executing DtaDevicePowerUp");

        return Status;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPowerUpDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaPowerUpDevice(DtaDeviceData* pDvcData)
{   
    DtStatus  Status = DT_STATUS_OK;

    // keep track of last encountered error
    DtStatus  StatusPu = DT_STATUS_OK;

    Status = DtaDevicePowerUp(pDvcData);
    if (!DT_SUCCESS(Status))
    {   
        DtDbgOut(ERR, DTA, "Error executing DtaDevicePowerUp");
        StatusPu = Status;     
    }
    
    Status = DtaDeviceInterruptEnable(pDvcData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTA, "Error executing DtaDeviceInterruptEnable");
        StatusPu = Status;
    }
  
    Status = DtaDevicePowerUpPost(pDvcData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTA, "Error executing DtaDevicePowerUpPost");
        StatusPu = Status;
    }
    return StatusPu;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSetMemoryTestMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Is called just before a memory test is executed.
// Currently in use for the Dta2162 only.
//
DtStatus  DtaSetMemoryTestMode(DtaDeviceData* pDvcData, Bool On)
{   
    DtStatus  Status = DT_STATUS_OK;
    Int i;
    DtaIpPort*  pIpPort = NULL;

    // Only IP ports type 2 must be disabled while doing a memory test
    if (pDvcData->m_NumIpPorts==0)
        return DT_STATUS_OK;

    // Enable testmode: disable ip-ports
    if (On)
    {        
        for (i=0; i<pDvcData->m_NumIpPorts; i++)
        { 
            pIpPort = &pDvcData->m_IpDevice.m_pIpPorts[i];
            if (pIpPort->m_PortType != DTA_IPPORT_TYPE2)
                    continue;

            Status = DtaIpPowerdownPre(pIpPort);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, IP, "Error stopping IP port %i", i);
                return Status;
            }
            DtaIpDisableInterrupts(pIpPort);           
            DtaIpPowerdown(&pDvcData->m_IpDevice, pIpPort);
        } 
        return DT_STATUS_OK;
    } 

    // Leave testmode: enable ip-ports
    for (i=0; i<pDvcData->m_NumIpPorts; i++)
    { 
        pIpPort = &pDvcData->m_IpDevice.m_pIpPorts[i];
        // Only IP ports type 2 must be disabled while doing a memory test
        if (pIpPort->m_PortType != DTA_IPPORT_TYPE2)
            continue;

        Status = DtaIpPowerup(pIpPort);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, IP, "Error starting DMA and port threads %i", i);
            return Status;
        }        
        DtaIpEnableInterrupts(pIpPort);
    }   
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRebootFirmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaRebootFirmware(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtStatus  StatusReboot = DT_STATUS_OK;

    // Currently rebooting is only supported on cards with a PLX9054/9056
    if (!pDvcData->m_DevInfo.m_UsesPlxChip)
        return DT_STATUS_NOT_SUPPORTED;

    // Power down the device
    StatusReboot = DtaPowerDownDevice(pDvcData);
    if (DT_SUCCESS(StatusReboot))
    {
        volatile UInt8*  p905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;
        UInt32  CntrlReg;
    
        // Issue firmware reload by generating a negative going pulse on the 
        // General Purpose Output of the PCI9056
        
        // High-to-low edge
        CntrlReg = READ_UINT32(p905XRegs, PCI905X_EEPROM_CTRL_STAT);
        CntrlReg &= ~PCI905X_CNTRL_GENPURPOUTP;
        WRITE_UINT32(p905XRegs, PCI905X_EEPROM_CTRL_STAT, CntrlReg);

        // Delay 50 microsec
        DtWaitBlock(50);
        
        // Low-to-high edge
        CntrlReg |= PCI905X_CNTRL_GENPURPOUTP;
        WRITE_UINT32(p905XRegs, PCI905X_EEPROM_CTRL_STAT, CntrlReg);
        
        DtDbgOut(MAX, DTA, "Firmware-reboot pulse has been generated");
        
        // Wait 300ms until firmware is reloaded
        DtSleep(300);

        DtDbgOut(MAX, DTA, "End of 300ms wait period");
        
        // Local bus may hang. Issue local bus reset
        CntrlReg |= PCI9056_CNTRL_SOFTW_RESET;
        WRITE_UINT32(p905XRegs, PCI905X_EEPROM_CTRL_STAT, CntrlReg);
        CntrlReg &= ~PCI9056_CNTRL_SOFTW_RESET;
        WRITE_UINT32(p905XRegs, PCI905X_EEPROM_CTRL_STAT, CntrlReg);
        DtDbgOut(MAX, DTA, "Local bus reset finished");

        // Re-initialise local configuration registers
        Status = DtaDeviceInitPci905X(pDvcData);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTA, "Error executing DtaDeviceInitPci905X");
            // Keep track of the last encountered error
            StatusReboot = Status;
        }
    }
    
    // Start powerup sequence
    Status = DtaPowerUpDevice(pDvcData);
    if (!DT_SUCCESS(Status))
        StatusReboot = Status;
    
    return StatusReboot;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRfPwrMeasLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Pauses/continues the RF-power measurement via the I2c-bus
//
void  DtaRfPwrMeasLock(DtaDeviceData* pDvcData, Int PortIndex, Int Lock)
{ 
    UInt8*  pBase;

    if (pDvcData == NULL)
        return;

    // Get register base address pointer
    pBase = pDvcData->m_DtaRegs.m_pKernel;
    if (pBase == NULL)
        return;

    if (Lock)
    {
        DtaRegRpmCtrlSetHaltMeas(pBase, 1);      // Pause
        // Wait until power measurement is stopped
        // Busy wait
        while ((DtaRegRpmStatIsBusy(pBase) != 0))
            ;
    } else
        DtaRegRpmCtrlSetHaltMeas(pBase, 0);      // Continue
}
