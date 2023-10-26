// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcEMAC10G.c *#*#*#*#*#*#*#* (C) 2020-2022 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "TEMPLATE" by the building block's shortname (all uppercase)
// and replace "Template" by the building block's shortname in camel case. 
// 

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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
#include "DtBc.h"
#include "DtBcEMAC10G.h"
#include "DtBcEMAC10G_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcEMAC10G implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define MAX_DELAY_NS 2000          // 2us: Should be at least the same as the scheduler
                                   // offset size
#define SCHED_OFFSET_NS 278        // 278ns measured with TX on time+RX timestamp back



// MACRO with default precondition checks for the Bc
#define BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcEMAC10G))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_EMAC10G_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(EMAC10G, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcEMAC10G_Init(DtBc*);
static DtStatus  DtBcEMAC10G_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcEMAC10G_OnEnable(DtBc*, Bool  Enable);
static DtStatus  DtBcEMAC10G_RegisterIntHandlers(DtBcEMAC10G*);
static void  DtBcEMAC10G_InterruptDpcDone(DtDpcArgs* pArgs);
static DtStatus  DtBcEMAC10G_InterruptHandler(DtBc*, Int, Int, void*);
static void  DtBcEMAC10G_SetControlRegs(DtBcEMAC10G*, Bool BlkEna, Int OpMode);
static DtStatus  DtBcEMAC10G_ReadMacAddressFromVPD(DtBcEMAC10G* pBc);

// Utility functions for MAC-address conversion (also used in this file)
static __inline  UInt32  MAC_ADDRESS_LOW(UInt8* MacAddr) {
    return (UInt32)(MacAddr[0] | (MacAddr[1]<<8) | (MacAddr[2]<<16) | (MacAddr[3]<<24));
}
static __inline  UInt32  MAC_ADDRESS_HIGH(UInt8* MacAddr) {
    return (UInt32)(MacAddr[4] | (MacAddr[5] << 8));
}
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcEMAC10G - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcEMAC10G_Close(DtBc*  pBc)
{
    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcEMAC10G*  DtBcEMAC10G_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                                                const char* pRole, Int Instance, Int Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_EMAC10G_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcEMAC10G, Id, DT_BLOCK_TYPE_EMAC10G,
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcEMAC10G_Close;
    OpenParams.m_InitFunc = DtBcEMAC10G_Init;
    OpenParams.m_OnEnableFunc = DtBcEMAC10G_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcEMAC10G_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcEMAC10G*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcEMAC10G_GetOperationalMode(DtBcEMAC10G* pBc, Int* pOpMode)
{
    // Sanity check
    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_EMAC10G_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcEMAC10G_SetOperationalMode(DtBcEMAC10G* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_EMAC10G_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Change Operational Mode
    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        // IDLE or STANDBY -> RUN
        // Enable interrupt
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_EMAC10G_LINK_STATUS);
    }
    else if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    {
        // RUN -> STANDBY or IDLE
        // Disable interrupt
        Status = DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_EMAC10G_LINK_STATUS);
    }

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcEMAC10G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcEMAC10G - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC10G_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcEMAC10G* pBc = (DtBcEMAC10G*)pBcBase;

    DtDbgOutBc(MAX, EMAC10G, pBc, "DtBcEMAC10G_Init");

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_LinkStatCB = NULL;
    DtSpinLockInit(&pBc->m_LinkStatCBSpinlock);
    
    // Initialize Done event
    Status = DtEventInit(&pBc->m_DoneEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, EMAC10G, pBc, 
                              "ERROR: failed to init done event (Status=0x%08X)", Status);
        return Status;
    }
    
    // Init interrupt DPC
    Status = DtDpcInit(&pBc->m_IntDpc, DtBcEMAC10G_InterruptDpcDone, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, EMAC10G, pBc, "ERROR: failed to init DPC (Status=0x%08X)",
                                                                                  Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Status = DtBcEMAC10G_RegisterIntHandlers(pBc);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, EMAC10G, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    Status = DtBcEMAC10G_ReadMacAddressFromVPD(pBc);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, EMAC10G, pBc, "ERROR: failed to get permanent MAC address");
        return Status;
    }

    // Set control register
    DtBcEMAC10G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    EMAC10G_SchedulingOffset_WRITE(pBc, SCHED_OFFSET_NS);
    DtDbgOutBc(MAX, EMAC10G, pBc, "DtBcEMAC10G_Init done");

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_MacInitAfterClkEnable -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcEMAC10G_MacInitAfterClkEnable(DtBcEMAC10G* pBc)
{
    // Initialise MAC
    DtStatus  Status = DT_STATUS_OK;
    UInt32  RegData;

    DtDbgOutBc(MAX, EMAC10G, pBc, "DtBcEMAC10G_MacInitAfterClkEnable");

    // Reset MAC
    RegData = EMAC10G_LlEthMac1G10GA10_MacRstCtrl_READ(pBc);
    EMAC10G_LlEthMac1G10GA10_MacRstCtrl_SET_TxDpRst(RegData, 1);
    EMAC10G_LlEthMac1G10GA10_MacRstCtrl_SET_RxDpRst(RegData, 1);
    EMAC10G_LlEthMac1G10GA10_MacRstCtrl_WRITE(pBc, RegData);
    DtSleep(1);
    // Release reseet
    EMAC10G_LlEthMac1G10GA10_MacRstCtrl_SET_TxDpRst(RegData, 0);
    EMAC10G_LlEthMac1G10GA10_MacRstCtrl_SET_RxDpRst(RegData, 0);
    EMAC10G_LlEthMac1G10GA10_MacRstCtrl_WRITE(pBc, RegData);
    // Wait for at least 500ns
    DtSleep(1); 

    DtBcEMAC10G_SetMacAddress(pBc, pBc->m_MacAddrPer);

    // Enable transmit and receive path
    EMAC10G_LlEthMac1G10GA10_TxPckCtrl_WRITE_TxPckCtrl(pBc, 1);
    EMAC10G_LlEthMac1G10GA10_RxTransferCtrl_WRITE_RxTransferCtrl(pBc, 1);

    // Initialise TX path
    EMAC10G_LlEthMac1G10GA10_TxPadCtrl_WRITE_TxPadCtrl(pBc, EMAC10G_PAD_ENABLE);
    
    // Initialise RX Path

    // Enable receiving all unicasts and all multicasts from the network. Skip MAC address
    // filtering
    RegData = 0;
    RegData = EMAC10G_LlEthMac1G10GA10_RxFrameCtrl_SET_EnAllUCast(RegData, 1);
    RegData = EMAC10G_LlEthMac1G10GA10_RxFrameCtrl_SET_EnAllMCast(RegData, 1);
    RegData = EMAC10G_LlEthMac1G10GA10_RxFrameCtrl_SET_IgnorePause(RegData, 1);
    EMAC10G_LlEthMac1G10GA10_RxFrameCtrl_WRITE(pBc, RegData);

    // Enable receiving CRC and not stripping padding. Finally stripping must be on.
    EMAC10G_LlEthMac1G10GA10_RxPadCrcCtrl_WRITE_RxPadCrcCtrl(pBc, 0);

    // Enable start transmiting and receiving
    EMAC10G_LlEthMac1G10GA10_TxPckCtrl_WRITE_TxPckCtrl(pBc, 0);
    EMAC10G_LlEthMac1G10GA10_RxTransferCtrl_WRITE_RxTransferCtrl(pBc, 0);
    DtDbgOutBc(MAX, EMAC10G, pBc, "DtBcEMAC10G_MacInitAfterClkEnable Exit");
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcEMAC10G_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcEMAC10G* pBc = (DtBcEMAC10G*)pBcBase;

    // Sanity check
    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, EMAC10G, pBc, "OnEnable from disable -> enable");
        
        // Program MAC registers.
        Status = DtBcEMAC10G_MacInitAfterClkEnable(pBc);
        if (!DT_SUCCESS(Status))
            DtDbgOutBc(ERR, EMAC10G, pBc, "Error initializing MAC post clock enable");


        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, EMAC10G, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcEMAC10G_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, EMAC10G, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcEMAC10G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC10G_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcEMAC10G* pBc = (DtBcEMAC10G*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, EMAC10G, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcEMAC10G_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, EMAC10G, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC10G_RegisterIntHandlers(DtBcEMAC10G* pBc)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i = 0;

    // Sanity check
    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks, but do not enable the interrupts. We will enable 
    // them on demand
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_EMAC10G_LINK_STATUS:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id, 
                                                     DtBcEMAC10G_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, EMAC10G, pBc,
                               "ERROR: failed to register interrupt handler (ID=%d)", Id);
            }
            break;

        default:
            DT_ASSERT(FALSE);      // Unknown ID. New version of the block??
            break;
        }
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_InterruptDpcDone -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcEMAC10G_InterruptDpcDone(DtDpcArgs* pArgs)
{
    DtBcEMAC10G*  pBc = (DtBcEMAC10G*)pArgs->m_pContext;

    // Sanity check
#ifdef _DEBUG
    UInt32  IntId = pArgs->m_Data1.m_UInt32_1;
    DT_ASSERT(IntId == DT_INTERRUPT_EMAC10G_LINK_STATUS);
#endif  // #ifdef _DEBUG
    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);
    // TRIGGER callback
    DtBcEMAC10G_GetLinkStatus(pBc);

    // If necessary, re-enable the interrupt that was fired
    // In that case a spinlock is required for checking the current operational mode or
    // state, to prevent an interrupt is re-enabled while the driver is shutting down.

    DtEventSet(&pBc->m_DoneEvent);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC10G_InterruptHandler(DtBc* pBcBase, Int Id, Int Index, void* pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcEMAC10G* pBc = (DtBcEMAC10G*)pBcBase;
    DtDpcArgs  DpcArgs;

    // Sanity check
    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);

    // Check ID is one of ours
    switch (Id)
    {
    case DT_INTERRUPT_EMAC10G_LINK_STATUS:
        // Reset interrupt state
        DtDbgOutBc(ERR, EMAC10G, pBc, "Link stat interrupt occured (ID=%d)", Id);
        break;

       // Not an EMAC10G interrupt
    default:
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;

    // If necessary, disable the interrupt that was fired. 
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&pBc->m_IntDpc, &DpcArgs);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcEMAC10G_SetControlRegs(DtBcEMAC10G* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData=0, FwBlkEna=0, FwOpMode=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? EMAC10G_BLKENA_ENABLED : EMAC10G_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = EMAC10G_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = EMAC10G_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = EMAC10G_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    RegData = EMAC10G_Control_READ(pBc);
    RegData = EMAC10G_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = EMAC10G_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = EMAC10G_Control_SET_MaxDelay(RegData, MAX_DELAY_NS);
    EMAC10G_Control_WRITE(pBc, RegData);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_ReadMacAddressFromVPD -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC10G_ReadMacAddressFromVPD(DtBcEMAC10G* pBc)
{
    DtStatus  Status = DT_STATUS_OK;
    char  Buffer[10];
    UInt  MacSeq, MacSeq1, MacSeq2, MacSeq3, MacSeq4;
    UInt  MacAddrLow, MacAddrHigh;
    Int  NumRead;
    DT_ASSERT(pBc->m_pCore->m_pDfVpd != NULL);

    NumRead = 0;
    Status = DtCore_VPD_ReadItemRo(pBc->m_pCore, "MC", Buffer, sizeof(Buffer), &NumRead);
    if (DT_SUCCESS(Status))
    {
        // Convert MC-string to 24-bit MAC sequence number
        MacSeq1 = Buffer[0] - 0x20;
        MacSeq2 = Buffer[1] - 0x20;
        MacSeq3 = Buffer[2] - 0x20;
        MacSeq4 = Buffer[3] - 0x20;
        MacSeq = (MacSeq1 << 18) | (MacSeq2 << 12) | (MacSeq3 << 6) | MacSeq4;
    }
    else {
        // No "MC" resource found: use 0 as MAC sequence number and 0x3FF as range
        // This is not an error, but the initial situation when a DTA-xxx is
        // powered up for the first time after manufacturing. In this case, the
        // board should start up with MAC address 00-14-F4-00-00-00
#ifdef WINBUILD
        MacSeq = 1;
#else
        MacSeq = 0;
#endif
        MacSeq += 0x3FF << 14;    // Reserved MAC range
        DtDbgOutBc(ERR, EMAC10G, pBc, "Failed to read MAC address from VPD (0x%x)", 
                                                                                  Status);
        Status = DT_STATUS_OK;
    }

    // If 2 phy's are on the board, the LSB bit of the MAC-address will be set to Port 0/1
    // Convert to 48-bit MAC address by prepending DekTec OUI (00-14-F4)
    // MSB/LSB is swapped!
    MacAddrHigh = 0xF4000000 | MacSeq;
    MacAddrLow = 0x0014;

    // Store MAC address
    pBc->m_MacAddrPer[5] = (MacAddrHigh & 0xff);
    pBc->m_MacAddrPer[4] = (MacAddrHigh >> 8) & 0xff;
    pBc->m_MacAddrPer[3] = (MacAddrHigh >> 16) & 0xff;
    pBc->m_MacAddrPer[2] = (MacAddrHigh >> 24) & 0xff;
    pBc->m_MacAddrPer[1] = (MacAddrLow & 0xff);
    pBc->m_MacAddrPer[0] = (MacAddrLow >> 8) & 0xff;

    DtDbgOutBc(MIN, EMAC10G, pBc, "MAC address: %02x:%02x:%02x:%02x:%02x:%02x",
                        pBc->m_MacAddrPer[0], pBc->m_MacAddrPer[1], pBc->m_MacAddrPer[2],
                        pBc->m_MacAddrPer[3], pBc->m_MacAddrPer[4], pBc->m_MacAddrPer[5]);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_GetMacAddressPerm -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC10G_GetMacAddressPerm(DtBcEMAC10G* pBc, UInt8* pMacAddress)
{
    DtMemCopy(pMacAddress, pBc->m_MacAddrPer, sizeof(pBc->m_MacAddrPer));
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_GetMacAddressCurr -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC10G_GetMacAddressCurr(DtBcEMAC10G* pBc, UInt8* pMacAddress)
{
    DtMemCopy(pMacAddress, pBc->m_MacAddrCur, sizeof(pBc->m_MacAddrCur));
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_SetMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC10G_SetMacAddress(DtBcEMAC10G* pBc, UInt8* pMacAddress)
{
    DtMemCopy(pBc->m_MacAddrCur, pMacAddress, sizeof(pBc->m_MacAddrCur));
    DtDbgOutBc(MIN, EMAC10G, pBc, "MAC address: %02x:%02x:%02x:%02x:%02x:%02x",
                        pBc->m_MacAddrCur[0], pBc->m_MacAddrCur[1], pBc->m_MacAddrCur[2],
                        pBc->m_MacAddrCur[3], pBc->m_MacAddrCur[4], pBc->m_MacAddrCur[5]);

    EMAC10G_LlEthMac1G10GA10_PrimMacAddrL_WRITE_PrimMacAddrL(pBc, 
                                                      MAC_ADDRESS_LOW(pBc->m_MacAddrCur));
    EMAC10G_LlEthMac1G10GA10_PrimMacAddrH_WRITE_PrimMacAddrH(pBc, 
                                                     MAC_ADDRESS_HIGH(pBc->m_MacAddrCur));
    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtbBcEMAC10G_GetCounter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC10G_GetCounter(DtBcEMAC10G* pBc, 
    const UInt  CounterId,
    UInt64*  pValue)
{
    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pValue != NULL);
    *pValue = 0;

    switch (CounterId)
    {
    case DT_MAC_CNT_GEN_BYTES_RCV:
        *pValue = EMAC10G_LlEthMac1G10GA10_RxStatsOctetsOkL_READ_RxStatsOctetsOkL(pBc) |
         (UInt64)EMAC10G_LlEthMac1G10GA10_RxStatsOctetsOkH_READ_RxStatsOctetsOkH(pBc)<<32;
        break;

    case DT_MAC_CNT_GEN_BYTES_XMIT:
        *pValue = EMAC10G_LlEthMac1G10GA10_TxStatsOctetsOkL_READ_TxStatsOctetsOkL(pBc) |
         (UInt64)EMAC10G_LlEthMac1G10GA10_TxStatsOctetsOkH_READ_TxStatsOctetsOkH(pBc)<<32;
        break;
    case DT_MAC_CNT_GEN_XMIT_OK:
        *pValue = EMAC10G_LlEthMac1G10GA10_TxStatsFrameOkL_READ_TxStatsFrameOkL(pBc) |
           (UInt64)EMAC10G_LlEthMac1G10GA10_TxStatsFrameOkH_READ_TxStatsFrameOkH(pBc)<<32;
        break;

    case DT_MAC_CNT_GEN_RCV_OK:
        *pValue = EMAC10G_LlEthMac1G10GA10_RxStatsFrameOkL_READ_RxStatsFrameOkL(pBc) |
           (UInt64)EMAC10G_LlEthMac1G10GA10_RxStatsFrameOkH_READ_RxStatsFrameOkH(pBc)<<32;
        break;
    case DT_MAC_CNT_GEN_XMIT_ERROR:
        *pValue = EMAC10G_LlEthMac1G10GA10_TxStatsFramErrL_READ_TxStatsFramErrL(pBc) |
         (UInt64)EMAC10G_LlEthMac1G10GA10_TxStatsFrameErrH_READ_TxStatsFrameErrH(pBc)<<32;
        break;
    case DT_MAC_CNT_GEN_RCV_ERROR:
        *pValue = EMAC10G_LlEthMac1G10GA10_RxStatsFramErrL_READ_RxStatsFramErrL(pBc) |
        (UInt64)EMAC10G_LlEthMac1G10GA10_RxStatsFrameErrH_READ_RxStatsFrameErrH(pBc)<<32;
        break;
    case DT_MAC_CNT_GEN_RCV_NO_BUFFER:
        *pValue = EMAC10G_LlEthMac1G10GA10_RxOflowDropCntL_READ_RxOflowDropCntL(pBc) |
           (UInt64)EMAC10G_LlEthMac1G10GA10_RxOflowDropCntH_READ_RxOflowDropCntH(pBc)<<32;
        break;
    case DT_MAC_CNT_GEN_RCV_CRC_ERROR:
        *pValue = EMAC10G_LlEthMac1G10GA10_RxStatsCrcErrL_READ_RxStatsCrcErrL(pBc) |
             (UInt64)EMAC10G_LlEthMac1G10GA10_RxStatsCrcErrH_READ_RxStatsCrcErrH(pBc)<<32;
        break;
/*#define DT_MAC_CNT_GEN_TRANSMIT_QUEUE_LENGTH      9
#define DT_MAC_CNT_802_3_RCV_ERROR_ALIGNMENT      10
#define DT_MAC_CNT_802_3_XMIT_ONE_COLLISION       11
#define DT_MAC_CNT_802_3_XMIT_MORE_COLLISIONS     12
#define DT_MAC_CNT_802_3_XMIT_DEFERRED            13
#define DT_MAC_CNT_802_3_XMIT_MAX_COLLISIONS      14
#define DT_MAC_CNT_802_3_RCV_OVERRUN              15
#define DT_MAC_CNT_802_3_XMIT_UNDERRUN            16
#define DT_MAC_CNT_802_3_XMIT_HEARTBEAT_FAILURE   17
#define DT_MAC_CNT_802_3_XMIT_TIMES_CRS_LOST      18
#define DT_MAC_CNT_802_3_XMIT_LATE_COLLISIONS     19
#define DT_MAC_CNT_GEN_RCV_HDR_ERROR              20*/
    default:
        DtDbgOutBc(AVG, EMAC10G, pBc, "Counter with ID: %d not supported", CounterId);
        return DT_STATUS_NOT_SUPPORTED;
    }
    DtDbgOutBc(MAX, EMAC10G, pBc, "Counter %2d, value: %lld", CounterId, *pValue);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_GetPhySpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC10G_GetPhySpeed(DtBcEMAC10G* pBc, UInt* pPhySpeed)
{
    UInt32  RegStatus, LinkSpeed, LinkStat;
    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pPhySpeed != NULL);
    RegStatus = EMAC10G_Status_READ(pBc);
    LinkStat = EMAC10G_Status_GET_LinkStat(RegStatus);
    LinkSpeed = EMAC10G_Status_GET_LinkSpeed(RegStatus);
    if (LinkStat == EMAC10G_LINKSTAT_DOWN)
        *pPhySpeed = DT_PHY_SPEED_NO_LINK;
    else
        *pPhySpeed = (LinkSpeed==EMAC10G_LINKSPEED_10G ? DT_PHY_SPEED_10000 : 
                                                                DT_PHY_SPEED_1000_MASTER);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_RegisterCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC10G_RegisterCallback(DtBcEMAC10G* pBc, DtLinkStatFunc LinkStatCB,
                                                                           void* pContext)
{
    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);
    DtDbgOutBc(ERR, EMAC10G, pBc, "Start");

    if (pBc->m_LinkStatCB != NULL)
        return DT_STATUS_IN_USE;
    DtSpinLockAcquire(&pBc->m_LinkStatCBSpinlock);
    pBc->m_LinkStatCB = LinkStatCB;
    pBc->m_pContexLinkStatCB = pContext;
    DtSpinLockRelease(&pBc->m_LinkStatCBSpinlock);
    DtDbgOutBc(ERR, EMAC10G, pBc, "Exit");
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_UnregisterCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC10G_UnregisterCallback(DtBcEMAC10G* pBc, void* pContext)
{
    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);
    DtDbgOutBc(ERR, EMAC10G, pBc, "Start");
    if (pBc->m_LinkStatCB == NULL)
        return DT_STATUS_NOT_INITIALISED;
    if (pBc->m_pContexLinkStatCB != pContext)
        return DT_STATUS_INVALID_PARAMETER;
    DtSpinLockAcquire(&pBc->m_LinkStatCBSpinlock);
    pBc->m_LinkStatCB = NULL;
    pBc->m_pContexLinkStatCB = NULL;
    DtSpinLockRelease(&pBc->m_LinkStatCBSpinlock);
    DtDbgOutBc(ERR, EMAC10G, pBc, "Exit");
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G_GetLinkStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC10G_GetLinkStatus(DtBcEMAC10G* pBc)
{
    UInt32 LinkStatReg;
    UInt32 LinkUp;
    BC_EMAC10G_DEFAULT_PRECONDITIONS(pBc);
    DtDbgOutBc(ERR, EMAC10G, pBc, "Start");
    DtSpinLockAcquire(&pBc->m_LinkStatCBSpinlock);
    if (pBc->m_LinkStatCB == NULL)
    {
        DtSpinLockRelease(&pBc->m_LinkStatCBSpinlock);
        return DT_STATUS_NOT_INITIALISED;
    }
    LinkStatReg = EMAC10G_Status_READ(pBc);
    LinkUp = EMAC10G_Status_GET_LinkStat(LinkStatReg);
    pBc->m_LinkStatCB((LinkUp!=0?DT_PHY_LINK_UP:DT_PHY_LINK_DOWN), pBc->m_pContexLinkStatCB);
    DtSpinLockRelease(&pBc->m_LinkStatCBSpinlock);
    DtDbgOutBc(ERR, EMAC10G, pBc, "Exit");
    return DT_STATUS_OK;
}
