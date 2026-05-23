// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcEMAC25G.c *#*#*#*#*#*#*#* (C) 2020-2023 DekTec
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
#include "DtBcEMAC25G.h"
#include "DtBcEMAC25G_RegAccess.h"
#include "DtDfSfp.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcEMAC25G implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define MAX_DELAY_NS 2000          // 2us: Should be at least the same as the scheduler
                                   // offset size
#define SCHED_OFFSET_NS 278        // 278ns measured with TX on time+RX timestamp back

#define EMAC25G_LINKSPEED_NOT_SET  0xFFFFFFFF
#define EMAC25G_LINKSPEED_ERROR  (EMAC25G_LINKSPEED_NOT_SET - 1)

// MACRO with default precondition checks for the Bc
#define BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc)      \
                                  DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcEMAC25G))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_EMAC25G_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(EMAC25G, pBc)


#define BC_EMAC25G_SFP_CAP_10G  1
#define BC_EMAC25G_SFP_CAP_25G  2

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcEMAC25G_Init(DtBc*);
static DtStatus  DtBcEMAC25G_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcEMAC25G_OnEnable(DtBc*, Bool  Enable);
static DtStatus  DtBcEMAC25G_RegisterIntHandlers(DtBcEMAC25G*);
static void  DtBcEMAC25G_LinkChangeDpc(DtDpcArgs* pArgs);
static DtStatus  DtBcEMAC25G_InterruptHandler(DtBc*, Int, Int, void*);
static void  DtBcEMAC25G_SetControlRegs(DtBcEMAC25G*, Bool BlkEna, Int OpMode);
static DtStatus  DtBcEMAC25G_ReadMacAddressFromVPD(DtBcEMAC25G* pBc);
static DtStatus  DtBcEMAC25G_SetNetworkRate(DtBcEMAC25G* pBc, Int PhySpeed);
//static DtStatus  DtBcEMAC25G_DetectLinkSpeed(DtBcEMAC25G* pBc);
static void  DtBcEMAC25G_PeriodicInterruptHandler(DtObject* pObj, DtTodTime Time);
static void  DtBcEMAC25G_LinkDetectStateMachine(DtBcEMAC25G* pBc);
static void  DtBcEMAC25G_SetNetworkRateDpc(DtDpcArgs* pArgs);
static Bool  DtBcEMAC25G_SetNewLinkSpeedTry(DtBcEMAC25G* pBc);
static void  DtBcEMAC25G_LinkDetectSetState(DtBcEMAC25G* pBc, LinkDetectStateEmac25G NewState);
static void  DtBcEMAC25G_SfpIntWi(DtWorkItemArgs* pArgs);

// Utility functions for MAC-address conversion (also used in this file)
static __inline  UInt32  MAC_ADDRESS_LOW(UInt8* MacAddr) {
    return (UInt32)(MacAddr[0] | (MacAddr[1]<<8) | (MacAddr[2]<<16) | (MacAddr[3]<<24));
}
static __inline  UInt32  MAC_ADDRESS_HIGH(UInt8* MacAddr) {
    return (UInt32)(MacAddr[4] | (MacAddr[5] << 8));
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- GetLinkSpeedDetectTimeout -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline UInt GetLinkSpeedDetectTimeout(UInt LinkSpeed)
{
    const UInt LinkSpeedDetectTimeoutCnt25G = 100; // PerInt = 10ms*100 = 1 second
    const UInt LinkSpeedDetectTimeoutCnt10G = 70; // PerInt = 10ms*70 = 0,7 second
    if (LinkSpeed == EMAC25G_LINKSPEED_25G)
        return LinkSpeedDetectTimeoutCnt25G;
    return LinkSpeedDetectTimeoutCnt10G;
}
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcEMAC25G - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcEMAC25G_Close(DtBc*  pBc)
{
    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pBc->m_pCore, (DtObject*)pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcEMAC25G*  DtBcEMAC25G_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                                                const char* pRole, Int Instance, Int Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_EMAC25G_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcEMAC25G, Id, DT_BLOCK_TYPE_EMAC25G,
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcEMAC25G_Close;
    OpenParams.m_InitFunc = DtBcEMAC25G_Init;
    OpenParams.m_OnEnableFunc = DtBcEMAC25G_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcEMAC25G_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcEMAC25G*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcEMAC25G_GetOperationalMode(DtBcEMAC25G* pBc, Int* pOpMode)
{
    // Sanity check
    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_EMAC25G_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcEMAC25G_SetOperationalMode(DtBcEMAC25G* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;
    UInt32 SfpGpio = 0;
    // Sanity check
    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_EMAC25G_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Change Operational Mode
    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        // IDLE or STANDBY -> RUN
        
        // Initialize SFP state machine so speed will be set after possible speed change
        DtBcEMAC25G_LinkDetectSetState(pBc, INIT);
        
        // Clear interrupt status
        EMAC25G_Status_CLEAR_LinkStatChanged(pBc);
        EMAC25G_Status_CLEAR_SfpAbsentChanged(pBc);

        // Enable interrupt
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_EMAC25G_LINK_STATUS);

        // Enable SFP
        SfpGpio = EMAC25G_SfpGpio_READ(pBc);
        SfpGpio = EMAC25G_SfpGpio_SET_Disable(SfpGpio, 0);
        EMAC25G_SfpGpio_WRITE(pBc, SfpGpio);
    }
    else if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    {
        // RUN -> STANDBY or IDLE
        // Disable interrupt
        Status = DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_EMAC25G_LINK_STATUS);
    }

    if (OpMode == DT_BLOCK_OPMODE_IDLE)
    {
        
        // Disable SFP
        DtBcEMAC25G_LinkDetectSetState(pBc, IDLE);
        SfpGpio = EMAC25G_SfpGpio_READ(pBc);
        SfpGpio = EMAC25G_SfpGpio_SET_Disable(SfpGpio, 1);
        EMAC25G_SfpGpio_WRITE(pBc, SfpGpio);
    }

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcEMAC25G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcEMAC25G - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC25G_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcEMAC25G* pBc = (DtBcEMAC25G*)pBcBase;
    DtOnPeriodicIntervalRegData RegData;


    DtDbgOutBc(MAX, EMAC25G, pBc, "DtBcEMAC25G_Init");

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_LinkStatCB = NULL;
    pBc->m_LinkSpeedTry = EMAC25G_LINKSPEED_NOT_SET;
    pBc->m_LinkSpeedCurr = EMAC25G_LINKSPEED_NOT_SET;
    pBc->m_LinkSpeedNw = EMAC25G_LINKSPEED_NOT_SET;
    //pBc->m_DisableMac = TRUE;
    pBc->m_SfpCap = 0;
    DtBcEMAC25G_LinkDetectSetState(pBc, IDLE);

    DtSpinLockInit(&pBc->m_LinkStatCBSpinlock);
    
    // Init interrupt DPC's
    Status = DtDpcInit(&pBc->m_LinkChangeDpc, DtBcEMAC25G_LinkChangeDpc, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, EMAC25G, pBc, "ERROR: failed to init LinkChange DPC"
                                                              " (Status=0x%08X)", Status);
        return Status;
    }
    Status = DtDpcInit(&pBc->m_SetNetworkRateDpc, DtBcEMAC25G_SetNetworkRateDpc, FALSE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, EMAC25G, pBc, "ERROR: failed to init SetNetworkRate DPC"
                                                              " (Status=0x%08X)", Status);
        return Status;
    }

    // Initialise Workitem for phy interrupt handling
    Status = DtWorkItemInit(&pBc->m_SfpIntWorkItem, DtBcEMAC25G_SfpIntWi, TRUE, 
                                                                 &pBc->m_pCore->m_Device);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, EMAC25G, pBc, "ERROR: failed to init SfpInt workitem"
                                                              " (Status=0x%08X)", Status);
        return Status;
    }
    // Register interupt handler
    Status = DtBcEMAC25G_RegisterIntHandlers(pBc);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, EMAC25G, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtBcEMAC25G_PeriodicInterruptHandler;
    RegData.m_pObject = (DtObject*)pBc;
    Status = DtCore_TOD_PeriodicItvRegister(pBc->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, EMAC25G, pBc, "DtCore_TOD_PeriodicItvRegister failed");
        return Status;
    }

    Status = DtBcEMAC25G_ReadMacAddressFromVPD(pBc);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, EMAC25G, pBc, "ERROR: failed to get permanent MAC address");
        return Status;
    }

    // Set control register
    DtBcEMAC25G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    EMAC25G_SchedulingOffset_WRITE(pBc, SCHED_OFFSET_NS);
    DtDbgOutBc(MAX, EMAC25G, pBc, "DtBcEMAC25G_Init done");

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_MacInitAfterClkEnable -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcEMAC25G_MacInitAfterClkEnable(DtBcEMAC25G* pBc)
{
    // Initialise MAC
    DtStatus  Status = DT_STATUS_OK;
    UInt32  RegData;

    DtDbgOutBc(MAX, EMAC25G, pBc, "DtBcEMAC25G_MacInitAfterClkEnable");

    // Deassert MAC reset
    RegData = EMAC25G_Control_READ(pBc);
    RegData = EMAC25G_Control_SET_CsrRst(RegData, 0);
    EMAC25G_Control_WRITE(pBc, RegData);

    // Always enable FCS
    EMAC25G_EthMac25GS10_RxMacCrcConfig_WRITE_RxMacCrcConfig(pBc, 1);

    // The state machine starts when network driver is enabled and stopped
    // when network card is disabled
    //DT_ASSERT(pBc->m_LinkDetectState==IDLE);
    //DtBcEMAC25G_LinkDetectSetState(pBc, INIT);
    
    DtDbgOutBc(MAX, EMAC25G, pBc, "DtBcEMAC25G_MacInitAfterClkEnable Exit");
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcEMAC25G_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcEMAC25G* pBc = (DtBcEMAC25G*)pBcBase;
    
    // Sanity check
    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {

        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, EMAC25G, pBc, "OnEnable from disable -> enable");
        
        // Program MAC registers.
        Status = DtBcEMAC25G_MacInitAfterClkEnable(pBc);
        if (!DT_SUCCESS(Status))
            DtDbgOutBc(ERR, EMAC25G, pBc, "Error initializing MAC post clock enable");


        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, EMAC25G, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcEMAC25G_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, EMAC25G, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcEMAC25G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC25G_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcEMAC25G* pBc = (DtBcEMAC25G*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, EMAC25G, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcEMAC25G_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, EMAC25G, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC25G_RegisterIntHandlers(DtBcEMAC25G* pBc)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i = 0;

    // Sanity check
    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks, but do not enable the interrupts. We will enable 
    // them on demand
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_EMAC25G_LINK_STATUS:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id, 
                                                      DtBcEMAC25G_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, EMAC25G, pBc,
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

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_SfpIntWi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcEMAC25G_SfpIntWi(DtWorkItemArgs* pArgs)
{
    DtBcEMAC25G* pBc = (DtBcEMAC25G*)pArgs->m_pContext;
    DT_ASSERT(pBc != NULL && pBc->m_Size == sizeof(DtBcEMAC25G));
    pBc->m_SfpIntCB(pArgs->m_Data1.m_UInt32_1, pBc->m_pContexSfpIntCB);
}


// .-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_PeriodicInterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcEMAC25G_PeriodicInterruptHandler(DtObject* pObj, DtTodTime Time)
{
    DtBcEMAC25G* pBc = (DtBcEMAC25G*)pObj;
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcEMAC25G));

    // Must be enabled and in RUN
    if (!DtBc_IsEnabled((DtBc*)pBc))
        return;
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return;
    DtBcEMAC25G_LinkDetectStateMachine(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_LinkDetectStateMachine -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcEMAC25G_LinkDetectSetState(DtBcEMAC25G* pBc, LinkDetectStateEmac25G NewState)
{
    switch (NewState)
    {
    case IDLE: DtDbgOutBc(AVG, EMAC25G, pBc, "Set state: IDLE"); break;
    case INIT: DtDbgOutBc(AVG, EMAC25G, pBc, "Set state: INIT"); break;
    case WAIT_FOR_SFP: DtDbgOutBc(AVG, EMAC25G, pBc, "Set state: WAIT_FOR_SFP"); break;
    case READ_SFP: DtDbgOutBc(AVG, EMAC25G, pBc, "Set state: READ_SFP"); break;
    case WAIT_FOR_CAP: DtDbgOutBc(AVG, EMAC25G, pBc, "Set state: WAIT_FOR_CAP"); break;
    case SET_SPEED: DtDbgOutBc(AVG, EMAC25G, pBc, "Set state: SET_SPEED"); break;
    case WAIT_FOR_LINKUP: DtDbgOutBc(AVG, EMAC25G, pBc, "Set state: WAIT_FOR_LINKUP"); break;
    case LINKUP: DtDbgOutBc(AVG, EMAC25G, pBc, "Set state: LINKUP"); break;
    case CONFIG_ERROR: DtDbgOutBc(ERR, EMAC25G, pBc, "Set state: CONFIG_ERROR"); break;
    default:
        DtDbgOutBc(ERR, EMAC25G, pBc, "Unknown state: %u", (UInt)NewState);
    }
    pBc->m_LinkDetectState = NewState;
}

void DtBcEMAC25G_LinkDetectStateMachine(DtBcEMAC25G* pBc)
{

    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);

    if (pBc->m_SfpAbsentChanged)
    {
        // SFP state changed, we go to the init state if we're not waiting for reading 
        // the SFP module
        if (pBc->m_LinkDetectState != WAIT_FOR_CAP)
        {
            DtBcEMAC25G_LinkDetectSetState(pBc, INIT);
            pBc->m_SfpAbsentChanged = FALSE;
        }
    }
    
    switch (pBc->m_LinkDetectState)
    {
    case IDLE:
        break;
    case INIT:
        if (EMAC25G_SfpGpio_READ_Absent(pBc) == EMAC25G_SFPABSENT_ABSENT)
            DtBcEMAC25G_LinkDetectSetState(pBc, WAIT_FOR_SFP);
        else
            DtBcEMAC25G_LinkDetectSetState(pBc, READ_SFP);
        break;
    case WAIT_FOR_SFP:
        if (EMAC25G_SfpGpio_READ_Absent(pBc) == EMAC25G_SFPABSENT_PRESENT)
            DtBcEMAC25G_LinkDetectSetState(pBc, READ_SFP);
        break;
    case READ_SFP:
        pBc->m_LinkSpeedTry = EMAC25G_LINKSPEED_NOT_SET;
        if (pBc->m_SfpIntCB != NULL)
        {
            DtWorkItemArgs  WorkItemArgs;
            DtBcEMAC25G_LinkDetectSetState(pBc, WAIT_FOR_CAP);

            // Trigger Workitem to update the packet filter
            WorkItemArgs.m_pContext = pBc;
            WorkItemArgs.m_Data1.m_UInt32_1 = SFP_INT_STAT_PRESENT;
            DtWorkItemSchedule(&pBc->m_SfpIntWorkItem, &WorkItemArgs);
        }
        else
        {
            DT_ASSERT(FALSE);
            DtBcEMAC25G_LinkDetectSetState(pBc, CONFIG_ERROR);
        }
        break;
    case WAIT_FOR_CAP:
        if (pBc->m_LinkSpeedTry == EMAC25G_LINKSPEED_NOT_SET)
            break;
        DtBcEMAC25G_LinkDetectSetState(pBc, SET_SPEED);
        break;
    case SET_SPEED:
        pBc->m_LinkSpeedTryCount = 0;
        if (pBc->m_LinkSpeedTry==EMAC25G_LINKSPEED_25G || 
                                               pBc->m_LinkSpeedTry==EMAC25G_LINKSPEED_10G)
        {
            DtDpcArgs DpcArgs;
            DtBcEMAC25G_LinkDetectSetState(pBc, WAIT_FOR_LINKUP);
            pBc->m_LinkSpeedTryCount = GetLinkSpeedDetectTimeout(pBc->m_LinkSpeedTry);
            DpcArgs.m_pContext = pBc;
            DpcArgs.m_Data1.m_UInt32_1 = pBc->m_LinkSpeedTry;
            DpcArgs.m_Data1.m_UInt32_2 = 0;

            // Trigger set link speed DPC
            DtDpcSchedule(&pBc->m_SetNetworkRateDpc, &DpcArgs);
        } else
            DtBcEMAC25G_LinkDetectSetState(pBc, CONFIG_ERROR);
        break;
    case WAIT_FOR_LINKUP:
        if (pBc->m_LinkStat == DT_PHY_LINK_UP)
            DtBcEMAC25G_LinkDetectSetState(pBc, LINKUP);
        else
        {
            if (pBc->m_LinkSpeedTryCount == 0)// timeout
            {
                if (DtBcEMAC25G_SetNewLinkSpeedTry(pBc))
                    DtBcEMAC25G_LinkDetectSetState(pBc, SET_SPEED);
                else
                    pBc->m_LinkSpeedTryCount = 
                                           GetLinkSpeedDetectTimeout(pBc->m_LinkSpeedTry);
            } else
                pBc->m_LinkSpeedTryCount--;
        }
        break;
    case LINKUP:
        if (pBc->m_LinkStat == DT_PHY_LINK_DOWN)
        {
            pBc->m_LinkSpeedTryCount = GetLinkSpeedDetectTimeout(pBc->m_LinkSpeedTry);
            DtBcEMAC25G_LinkDetectSetState(pBc, WAIT_FOR_LINKUP);
        }
        break;
    case CONFIG_ERROR:
        break;
    }
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_SetNewLinkSpeedTry -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool DtBcEMAC25G_SetNewLinkSpeedTry(DtBcEMAC25G* pBc)
{
    UInt OldLinkSpeedTry = pBc->m_LinkSpeedTry;
    // Single cap or forced speed? Retry speed
    if (((pBc->m_SfpCap & (SFP_CAP_25G | SFP_CAP_10G)) != (SFP_CAP_25G | SFP_CAP_10G)) ||
        pBc->m_LinkSpeedNw != EMAC25G_LINKSPEED_NOT_SET)
        pBc->m_LinkSpeedTry = EMAC25G_LINKSPEED_NOT_SET;
    // Try 25Gb or 10Gb speed
    if ((pBc->m_SfpCap & SFP_CAP_25G)!=0 && pBc->m_LinkSpeedTry!=EMAC25G_LINKSPEED_25G &&
                                         (pBc->m_LinkSpeedNw==EMAC25G_LINKSPEED_NOT_SET || 
                                          pBc->m_LinkSpeedNw==EMAC25G_LINKSPEED_25G))
        pBc->m_LinkSpeedTry = EMAC25G_LINKSPEED_25G;
    else  if ((pBc->m_SfpCap & SFP_CAP_10G) != 0 && 
                                             pBc->m_LinkSpeedTry!=EMAC25G_LINKSPEED_10G &&
                                       (pBc->m_LinkSpeedNw == EMAC25G_LINKSPEED_NOT_SET ||
                                             pBc->m_LinkSpeedNw == EMAC25G_LINKSPEED_10G))

        pBc->m_LinkSpeedTry = EMAC25G_LINKSPEED_10G;
    else
        pBc->m_LinkSpeedTry = EMAC25G_LINKSPEED_ERROR;
    DtDbgOutBc(AVG, EMAC25G, pBc, "%sTrySpeed:%i Forced:%i (value: %i) Cap:%i", 
                   (OldLinkSpeedTry == pBc->m_LinkSpeedTry?"Re-":""), 
                   pBc->m_LinkSpeedTry, pBc->m_LinkSpeedNw==EMAC25G_LINKSPEED_NOT_SET?0:1, 
                   pBc->m_LinkSpeedNw, pBc->m_SfpCap);
    return OldLinkSpeedTry != pBc->m_LinkSpeedTry;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_SetSfpCapabilities -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcEMAC25G_SetSfpCapabilities(DtBcEMAC25G* pBc, UInt Capabilities)
{
    // Only accept 10G/25G capabilities if line rate capability is also detected
    if ((Capabilities & SFP_CAP_10G_RATE) == 0)
        Capabilities &= ~SFP_CAP_10G;
    if ((Capabilities & SFP_CAP_25G_RATE) == 0)
        Capabilities &= ~SFP_CAP_25G;
    pBc->m_SfpCap = Capabilities;

    if (pBc->m_LinkDetectState == WAIT_FOR_CAP)
    {
        DtBcEMAC25G_SetNewLinkSpeedTry(pBc);
    }
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_SetSfpIntCallbackFunc -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcEMAC25G_SetSfpIntCallbackFunc(DtBcEMAC25G* pBc, DtEMAC25GSfpIntFunc SfpIntCB,
                                                                           void* pContext)
{
    pBc->m_pContexSfpIntCB = pContext;
    pBc->m_SfpIntCB = SfpIntCB;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_LinkChangeDpcDone -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcEMAC25G_LinkChangeDpc(DtDpcArgs* pArgs)
{
    DtBcEMAC25G* pBc = (DtBcEMAC25G*)pArgs->m_pContext;
    UInt32 LinkStatReg;
    UInt32 LinkUp;
    Int LinkStat;

    // Sanity check
#ifdef _DEBUG
    UInt32  IntId = pArgs->m_Data1.m_UInt32_1;
    DT_ASSERT(IntId == DT_INTERRUPT_EMAC25G_LINK_STATUS);
#endif  // #ifdef _DEBUG
    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);
    
    LinkStatReg = EMAC25G_Status_READ(pBc);
    LinkUp = EMAC25G_Status_GET_LinkStat(LinkStatReg);

    DtDbgOutBc(AVG, EMAC25G, pBc, "LinkStatReg:%x LinkUp:%i", LinkStatReg, LinkUp);

    // Update state to network driver
    LinkStat = LinkUp != EMAC25G_LINKSTAT_DOWN ? DT_PHY_LINK_UP : DT_PHY_LINK_DOWN;

    DtSpinLockAcquire(&pBc->m_LinkStatCBSpinlock);
    if (pBc->m_LinkStatCB != NULL)
    {
        if (pBc->m_LinkStatNw != LinkStat)
        {
            pBc->m_LinkStatCB(LinkStat, pBc->m_pContexLinkStatCB);
            pBc->m_LinkStatNw = LinkStat;
        }
    }
    pBc->m_LinkStat = LinkStat;
    DtSpinLockRelease(&pBc->m_LinkStatCBSpinlock);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC25G_InterruptHandler(DtBc* pBcBase, Int Id, Int Index, void* pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcEMAC25G* pBc = (DtBcEMAC25G*)pBcBase;
    DtDpcArgs  DpcArgs;
    UInt32 StatusReg = 0;
    
    // Sanity check
    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);

    // Check ID is one of ours
    switch (Id)
    {
    case DT_INTERRUPT_EMAC25G_LINK_STATUS:
        StatusReg = EMAC25G_Status_READ(pBc);

        DtDbgOutBc(AVG, EMAC25G, pBc, "Link stat interrupt occured (ID=%d) StatusRef:%xh", 
                                                                           Id, StatusReg);
        if (EMAC25G_Status_GET_LinkStatChanged(StatusReg) != 0)
        {
            EMAC25G_Status_CLEAR_LinkStatChanged(pBc);

            // Init DPC argument
            DpcArgs.m_pContext = pBc;
            DpcArgs.m_Data1.m_UInt32_1 = Id;
            DpcArgs.m_Data1.m_UInt32_2 = 0;

            // Schedule DPC to handle the interrupt
            Status = DtDpcSchedule(&pBc->m_LinkChangeDpc, &DpcArgs);
        }
        if (EMAC25G_Status_GET_SfpAbsentChanged(StatusReg) != 0)
        {
            EMAC25G_Status_CLEAR_SfpAbsentChanged(pBc);
            pBc->m_SfpAbsentChanged = TRUE;
        }
        break;

       // Not an EMAC25G interrupt
    default:
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcEMAC25G_SetControlRegs(DtBcEMAC25G* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData=0, FwBlkEna=0, FwOpMode=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? EMAC25G_BLKENA_ENABLED : EMAC25G_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = EMAC25G_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = EMAC25G_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = EMAC25G_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    RegData = EMAC25G_Control_READ(pBc);
    RegData = EMAC25G_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = EMAC25G_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = EMAC25G_Control_SET_MaxDelay(RegData, MAX_DELAY_NS);

    EMAC25G_Control_WRITE(pBc, RegData);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_ReadMacAddressFromVPD -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC25G_ReadMacAddressFromVPD(DtBcEMAC25G* pBc)
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
        DtDbgOutBc(ERR, EMAC25G, pBc, "Failed to read MAC address from VPD (0x%x)", 
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

    // Update current MAC address
    DtBcEMAC25G_SetMacAddress(pBc, pBc->m_MacAddrPer);

    DtDbgOutBc(AVG, EMAC25G, pBc, "MAC address: %02x:%02x:%02x:%02x:%02x:%02x",
                        pBc->m_MacAddrPer[0], pBc->m_MacAddrPer[1], pBc->m_MacAddrPer[2],
                        pBc->m_MacAddrPer[3], pBc->m_MacAddrPer[4], pBc->m_MacAddrPer[5]);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_GetMacAddressPerm -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC25G_GetMacAddressPerm(DtBcEMAC25G* pBc, UInt8* pMacAddress)
{
    DtMemCopy(pMacAddress, pBc->m_MacAddrPer, sizeof(pBc->m_MacAddrPer));
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_GetMacAddressCurr -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC25G_GetMacAddressCurr(DtBcEMAC25G* pBc, UInt8* pMacAddress)
{
    DtMemCopy(pMacAddress, pBc->m_MacAddrCur, sizeof(pBc->m_MacAddrCur));
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_SetMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC25G_SetMacAddress(DtBcEMAC25G* pBc, UInt8* pMacAddress)
{
    DtMemCopy(pBc->m_MacAddrCur, pMacAddress, sizeof(pBc->m_MacAddrCur));
    DtDbgOutBc(AVG, EMAC25G, pBc, "MAC address: %02x:%02x:%02x:%02x:%02x:%02x",
                        pBc->m_MacAddrCur[0], pBc->m_MacAddrCur[1], pBc->m_MacAddrCur[2],
                        pBc->m_MacAddrCur[3], pBc->m_MacAddrCur[4], pBc->m_MacAddrCur[5]);
    // EMAC25G does not have a MAC-address filter. Filtering is done in driver
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_GetSupportedCounters -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC25G_GetSupportedCounters(DtBcEMAC25G* pBc, UInt MaxCounterIds,
                                                  UInt* pNumCounterIds, UInt* pCounterIds)
{
    const UInt SupportedIds[] = { DT_MAC_CNT_GEN_BYTES_RCV, DT_MAC_CNT_GEN_BYTES_XMIT, 
              DT_MAC_CNT_GEN_XMIT_OK, DT_MAC_CNT_GEN_RCV_OK, DT_MAC_CNT_GEN_RCV_CRC_ERROR, 
              DT_MAC_CNT_GEN_RCV_ERROR };
    UInt NumToCopy = sizeof(SupportedIds) / sizeof(UInt);
    if (MaxCounterIds == 0)
    {
        *pNumCounterIds = NumToCopy;
        return DT_STATUS_OK;
    }
    if (NumToCopy > MaxCounterIds)
    {
        DT_ASSERT(FALSE);
        NumToCopy = MaxCounterIds;
    }
    *pNumCounterIds = NumToCopy;
    DtMemCopy(pCounterIds, SupportedIds, sizeof(UInt) * NumToCopy);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtbBcEMAC25G_GetCounter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC25G_GetCounter(DtBcEMAC25G* pBc, const UInt  CounterId, UInt64*  pValue)
{
    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pValue != NULL);
    *pValue = 0;
    
    switch (CounterId)
    {
    case DT_MAC_CNT_GEN_BYTES_RCV:
        *pValue = EMAC25G_EthMac25GS10_RxStatPaylOctsOkLo_READ_RxStatPaylOctsOkLo(pBc) |
         (UInt64)EMAC25G_EthMac25GS10_RxStatPaylOctsOkHi_READ_RxStatPaylOctsOkHi(pBc)<<32;
        break;

    case DT_MAC_CNT_GEN_BYTES_XMIT:
        *pValue = EMAC25G_EthMac25GS10_TxStatPaylOctsOkLo_READ_TxStatPaylOctsOkLo(pBc) |
         (UInt64)EMAC25G_EthMac25GS10_TxStatPaylOctsOkHi_READ_TxStatPaylOctsOkHi(pBc)<<32;
        break;
    case DT_MAC_CNT_GEN_XMIT_OK:
        *pValue = EMAC25G_EthMac25GS10_TxStatFrmOctsOkLo_READ_TxStatFrmOctsOkLo(pBc) |
           (UInt64)EMAC25G_EthMac25GS10_TxStatFrmOctsOkHi_READ_TxStatFrmOctsOkHi(pBc)<<32;
        break;

    case DT_MAC_CNT_GEN_RCV_OK:
        *pValue = EMAC25G_EthMac25GS10_RxStatFrmOctsOkLo_READ_RxStatFrmOctsOkLo(pBc) |
           (UInt64)EMAC25G_EthMac25GS10_RxStatFrmOctsOkHi_READ_RxStatFrmOctsOkHi(pBc)<<32;
        break;
    case DT_MAC_CNT_GEN_RCV_ERROR:
        *pValue = EMAC25G_RxDelCnt_READ(pBc);
        break;
    case DT_MAC_CNT_GEN_RCV_CRC_ERROR:
        *pValue = EMAC25G_EthMac25GS10_RxStatCrcErrLo_READ_RxStatCrcErrLo(pBc) |
                 (UInt64)EMAC25G_EthMac25GS10_RxStatCrcErrHi_READ_RxStatCrcErrHi(pBc)<<32;
        break;
    default:
        DtDbgOutBc(MAX, EMAC25G, pBc, "Counter with ID: %d not supported", CounterId);
        return DT_STATUS_NOT_SUPPORTED;
    }
    DtDbgOutBc(MAX, EMAC25G, pBc, "Counter %d, value: %lld", CounterId, *pValue);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_GetPhySpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC25G_GetPhySpeed(DtBcEMAC25G* pBc, UInt* pPhySpeed)
{
    UInt32  RegStatus, LinkStat;
    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pPhySpeed != NULL);
    RegStatus = EMAC25G_Status_READ(pBc);
    LinkStat = EMAC25G_Status_GET_LinkStat(RegStatus);
    if (LinkStat==EMAC25G_LINKSTAT_DOWN || 
                                          pBc->m_LinkSpeedCurr==EMAC25G_LINKSPEED_NOT_SET)
        *pPhySpeed = DT_PHY_SPEED_NO_LINK;
    else
        *pPhySpeed = (pBc->m_LinkSpeedCurr==EMAC25G_LINKSPEED_25G ? DT_PHY_SPEED_25000 :
                                                                      DT_PHY_SPEED_10000);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_SetPhySpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC25G_SetPhySpeed(DtBcEMAC25G* pBc, UInt PhySpeed)
{
    Int LinkSpeedNw = EMAC25G_LINKSPEED_NOT_SET;
    DtDpcArgs  DpcArgs;

    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);
    DtDbgOutBc(MAX, EMAC25G, pBc, "Start");

    switch (PhySpeed)
    {
    case DT_PHY_SPEED_25000:
        LinkSpeedNw = EMAC25G_LINKSPEED_25G;
        break;
    case DT_PHY_SPEED_10000:
        LinkSpeedNw = EMAC25G_LINKSPEED_10G;
        break;
    case DT_PHY_SPEED_AUTO_DETECT:
        LinkSpeedNw = EMAC25G_LINKSPEED_NOT_SET;
        break;
    default:
        DT_ASSERT(FALSE);
        LinkSpeedNw = EMAC25G_LINKSPEED_NOT_SET;
    }

   if (LinkSpeedNw != pBc->m_LinkSpeedNw)
   {
       pBc->m_LinkSpeedNw = LinkSpeedNw;
       pBc->m_LinkSpeedCurr = EMAC25G_LINKSPEED_NOT_SET;
       // Init DPC argument
       DpcArgs.m_pContext = pBc;
       DpcArgs.m_Data1.m_UInt32_1 = DT_INTERRUPT_EMAC25G_LINK_STATUS;
       DpcArgs.m_Data1.m_UInt32_2 = 0;

       // Trigger speed change DPC
       DtDpcSchedule(&pBc->m_LinkChangeDpc, &DpcArgs);
   }
   DtDbgOutBc(MAX, EMAC25G, pBc, "Exit");

   return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_RegisterLinkCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcEMAC25G_RegisterLinkCallback(DtBcEMAC25G* pBc, DtLinkStatFunc LinkStatCB,
                                                                           void* pContext)
{
    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);
    DtDbgOutBc(MAX, EMAC25G, pBc, "Start");

    if (pBc->m_LinkStatCB != NULL)
        return DT_STATUS_IN_USE;
    DtSpinLockAcquire(&pBc->m_LinkStatCBSpinlock);
    pBc->m_LinkStatCB = LinkStatCB;
    pBc->m_pContexLinkStatCB = pContext;
    DtSpinLockRelease(&pBc->m_LinkStatCBSpinlock);
    DtDbgOutBc(MAX, EMAC25G, pBc, "Exit");
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_UnregisterLinkCallback -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC25G_UnregisterLinkCallback(DtBcEMAC25G* pBc, void* pContext)
{
    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);
    DtDbgOutBc(MAX, EMAC25G, pBc, "Start");
    if (pBc->m_LinkStatCB == NULL)
        return DT_STATUS_NOT_INITIALISED;
    if (pBc->m_pContexLinkStatCB != pContext)
        return DT_STATUS_INVALID_PARAMETER;
    DtSpinLockAcquire(&pBc->m_LinkStatCBSpinlock);
    pBc->m_LinkStatCB = NULL;
    pBc->m_pContexLinkStatCB = NULL;
    DtSpinLockRelease(&pBc->m_LinkStatCBSpinlock);
    DtDbgOutBc(MAX, EMAC25G, pBc, "Exit");
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_GetLinkStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC25G_GetLinkStatus(DtBcEMAC25G* pBc)
{
    UInt32 LinkStatReg;
    UInt32 LinkUp;
    BC_EMAC25G_DEFAULT_PRECONDITIONS(pBc);
    DtDbgOutBc(MAX, EMAC25G, pBc, "Start");
    DtSpinLockAcquire(&pBc->m_LinkStatCBSpinlock);
    if (pBc->m_LinkStatCB == NULL)
    {
        DtSpinLockRelease(&pBc->m_LinkStatCBSpinlock);
        return DT_STATUS_NOT_INITIALISED;
    }
    LinkStatReg = EMAC25G_Status_READ(pBc);
    LinkUp = EMAC25G_Status_GET_LinkStat(LinkStatReg);
    pBc->m_LinkStatNw = LinkUp != EMAC25G_LINKSTAT_DOWN ? DT_PHY_LINK_UP : DT_PHY_LINK_DOWN;
    pBc->m_LinkStatCB(pBc->m_LinkStatNw, pBc->m_pContexLinkStatCB);
    DtSpinLockRelease(&pBc->m_LinkStatCBSpinlock);
    DtDbgOutBc(MAX, EMAC25G, pBc, "Exit");
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- WAIT_UNTIL_READ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Macro to improve readability
#define WAIT_UNTIL_READ(read_reg, read_bit, state, timeout_ms, err_msg)       \
{                                                                             \
    Int Count = 0;                                                            \
    do                                                                        \
    {                                                                         \
        UInt32 RegVal = read_reg(pBc);                                        \
        UInt32 Val = read_bit(RegVal);                                        \
        if (Val == state) break;                                              \
        DtWaitBlock(1000);                                                    \
    } while (Count++ < timeout_ms);                                           \
    if (Count >= timeout_ms)                                                  \
    {                                                                         \
        DtDbgOutBc(ERR, EMAC25G, pBc, err_msg);                               \
        return DT_STATUS_TIMEOUT;                                             \
    }                                                                         \
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_SetTransceiverParameters -.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcEMAC25G_SetTransceiverParameters(DtBcEMAC25G* pBc, Int LinkSpeed)
{
    UInt RegData;
    if (LinkSpeed == EMAC25G_LINKSPEED_25G)
    {
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg106_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg106_SET_PmaTxBufDccFinestepEnin(
                                                         RegData, EMAC25G_TXDCCEN_ENABLE);
        EMAC25G_EthTransc25GS10Shim_PhyReg106_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg108_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg108_SET_PmaTxBufTermSel(RegData,
                                                               EMAC25G_TXTERM_TX_TERM_85);
        EMAC25G_EthTransc25GS10Shim_PhyReg108_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11A_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11A_SET_PmaRxBufVgaDcGainLow(RegData,
                                                        EMAC25G_VGAGAINL_VGA_GAIN_L_0_16);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11A_SET_PmaRxBufCtleAcGain(RegData,
                                                           EMAC25G_CTLEGAIN_CTLE_GAIN_12);
        EMAC25G_EthTransc25GS10Shim_PhyReg11A_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11E_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11E_SET_PmaRxBufVgaHalfbwEn(RegData, 
                                                              EMAC25G_VGAHALFBWEN_ENABLE);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11E_SET_PmaRxBufCtleEqGainHigh(
                                        RegData, EMAC25G_CTLEEQGAINHIGH_CTLE_EQ_GAIN_H_0);
        EMAC25G_EthTransc25GS10Shim_PhyReg11E_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11F_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11F_SET_PmaRxBufVgaDcGainHigh(RegData,
                                                          EMAC25G_VGAGAINH_VGA_GAIN_H_16);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11F_SET_PmaRxBufEqBwSel(RegData,
                                                                 EMAC25G_RXEQBW_BW_26_6G);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11F_SET_PmaRxBufRxLfeqEnable(RegData, 
                                                               EMAC25G_LFEQENABLE_ENABLE);
        EMAC25G_EthTransc25GS10Shim_PhyReg11F_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg121_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg121_SET_PmaRxBufConstGmEn(RegData,
                                                                  EMAC25G_CGMEN_CGM_EN_2);
        EMAC25G_EthTransc25GS10Shim_PhyReg121_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg12D_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg12D_SET_PmaRxDfeLatchXcoupleDisable(
                                                     RegData, EMAC25G_LATCHXCDIS_DISABLE);
        EMAC25G_EthTransc25GS10Shim_PhyReg12D_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg12E_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg12E_SET_PmaRxBufCtleTiaIsel(RegData,
                                                             EMAC25G_IBTIABW_IB_TIA_BW_3);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg12E_SET_PmaRxBufEqSel(RegData,
                                                                  EMAC25G_EQSEL_EQ_SEL_3);
        EMAC25G_EthTransc25GS10Shim_PhyReg12E_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg130_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg130_SET_PmaRxDfeTapsumBwSel(RegData,
                                                             EMAC25G_TAPSUMBW_TAPSUM_BW0);
        EMAC25G_EthTransc25GS10Shim_PhyReg130_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg132_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg132_SET_CdrPllSetCdrVcoSpeedFix1(
                                          RegData, EMAC25G_VCOSPEEDFIX1_VCO_SPEED_FIX1_7);
        EMAC25G_EthTransc25GS10Shim_PhyReg132_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg135_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg135_SET_CdrPllLfResistorPfd(RegData,
                                                                  EMAC25G_LFPFD_LF_PFD_3);
        EMAC25G_EthTransc25GS10Shim_PhyReg135_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg136_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg136_SET_CdrPllSetCdrVcoSpeedFix2(
                                          RegData, EMAC25G_VCOSPEEDFIX2_VCO_SPEED_FIX2_8);
        EMAC25G_EthTransc25GS10Shim_PhyReg136_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg137_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg137_SET_CdrPllSetCdrVcoSpeed(RegData,
                                                            EMAC25G_VCOSPEED_VCO_SPEED_0);
        EMAC25G_EthTransc25GS10Shim_PhyReg137_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg139_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg139_SET_CdrPllChgpmpCurrentPfd(RegData,
                                                      EMAC25G_CHGPMPPFD_CP_CURRENT_PFD_1);
        EMAC25G_EthTransc25GS10Shim_PhyReg139_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg13A_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg13A_SET_CdrPllPdLCounter(RegData, 1);
        EMAC25G_EthTransc25GS10Shim_PhyReg13A_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg13B_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg13B_SET_CdrPllMcntDiv(RegData, 20);
        EMAC25G_EthTransc25GS10Shim_PhyReg13B_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg144_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg144_SET_PmaRxOdiMonitorBwSel1(RegData,
                                                                EMAC25G_ODIMONBWSEL_BW_1);
        EMAC25G_EthTransc25GS10Shim_PhyReg144_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg145_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg145_SET_PmaRxOdiMonitorBwSel2(RegData,
                                                                EMAC25G_ODIMONBWSEL_BW_1);
        EMAC25G_EthTransc25GS10Shim_PhyReg145_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg151_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg151_SET_PmaRxBufEqCdgenSel(RegData,
                                                           EMAC25G_EQCDGENSEL_EQ_CDGEN_0);
        EMAC25G_EthTransc25GS10Shim_PhyReg151_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg154_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg154_SET_PmaRxOdiVregVoltageSel(RegData,
                                                              EMAC25G_VREGVOLTSEL_VREG_1);
        EMAC25G_EthTransc25GS10Shim_PhyReg154_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg160_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg160_SET_PmaAdaptAdpAcCtleInitialValue(
                                              RegData, EMAC25G_ADPACVALUE_ADP_AC_VALUE_0);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg160_SET_PmaAdaptAdpDcCtleInitialValue(
                                              RegData, EMAC25G_ADPDCVALUE_ADP_DC_VALUE_3);
        EMAC25G_EthTransc25GS10Shim_PhyReg160_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg162_READ(pBc);
        RegData = 
                 EMAC25G_EthTransc25GS10Shim_PhyReg162_SET_PmaAdaptAdpDfeTap1InitialValue(
                 RegData, EMAC25G_ADPDFETAP1VALUE_ADP_DFE_TAP1_VALUE_2);
        EMAC25G_EthTransc25GS10Shim_PhyReg162_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg165_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg165_SET_PmaAdaptAdpDcCtleMode2H2Limit(
                                                                             RegData, 12);
        EMAC25G_EthTransc25GS10Shim_PhyReg165_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg167_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg167_SET_PmaAdaptAdpVgaInitialValue(
                                            RegData, EMAC25G_ADPVGAVALUE_ADP_VGA_VALUE_3);
        EMAC25G_EthTransc25GS10Shim_PhyReg167_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg22E_READ(pBc);
        RegData = 
              EMAC25G_EthTransc25GS10Shim_PhyReg22E_SET_HssiAibcrTxAibDllStrAlignDyCtlsel(
                                           RegData, EMAC25G_DLLSTRALIGNCYCTLSEL_CTLSEL_0);
        EMAC25G_EthTransc25GS10Shim_PhyReg22E_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg230_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg230_SET_HssiAibcrRxAibRxDccByp(
                                                       RegData, EMAC25G_RXDCCBYP_DISABLE);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg230_SET_HssiAibcrRxDccEn(
                                                         RegData, EMAC25G_RXDCCEN_ENABLE);
        EMAC25G_EthTransc25GS10Shim_PhyReg230_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg32C_READ(pBc);
        RegData = 
              EMAC25G_EthTransc25GS10Shim_PhyReg32C_SET_HssiAibndRxAibDllstrAlignDyCtlsel(
              RegData, EMAC25G_DLLSTRALIGNCYCTLSEL_CTLSEL_0);
        EMAC25G_EthTransc25GS10Shim_PhyReg32C_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg32E_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg32E_SET_HssiAibndTxAibTxDccByp(
                                                  RegData, EMAC25G_TXAIBTXDCCBYP_DISABLE);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg32E_SET_HssiAibndTxDccEn(RegData, 
                                                                  EMAC25G_TXDCCEN_ENABLE);
        EMAC25G_EthTransc25GS10Shim_PhyReg32E_WRITE(pBc, RegData);
    }
    else if (LinkSpeed == EMAC25G_LINKSPEED_10G)
    {
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg106_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg106_SET_PmaTxBufDccFinestepEnin(
                                                        RegData, EMAC25G_TXDCCEN_DISABLE);
        EMAC25G_EthTransc25GS10Shim_PhyReg106_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg108_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg108_SET_PmaTxBufTermSel(RegData,
                                                              EMAC25G_TXTERM_TX_TERM_100);
        EMAC25G_EthTransc25GS10Shim_PhyReg108_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11A_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11A_SET_PmaRxBufVgaDcGainLow(RegData,
                                                        EMAC25G_VGAGAINL_VGA_GAIN_L_4_20);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11A_SET_PmaRxBufCtleAcGain(RegData,
                                                            EMAC25G_CTLEGAIN_CTLE_GAIN_4);
        EMAC25G_EthTransc25GS10Shim_PhyReg11A_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11E_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11E_SET_PmaRxBufVgaHalfbwEn(RegData, 
                                                             EMAC25G_VGAHALFBWEN_DISABLE);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11E_SET_PmaRxBufCtleEqGainHigh(
                                       RegData, EMAC25G_CTLEEQGAINHIGH_CTLE_EQ_GAIN_H_32);
        EMAC25G_EthTransc25GS10Shim_PhyReg11E_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11F_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11F_SET_PmaRxBufVgaDcGainHigh(RegData,
                                                           EMAC25G_VGAGAINH_VGA_GAIN_H_0);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11F_SET_PmaRxBufEqBwSel(RegData,
                                                                 EMAC25G_RXEQBW_BW_12_5G);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg11F_SET_PmaRxBufRxLfeqEnable(RegData, 
                                                              EMAC25G_LFEQENABLE_DISABLE);
        EMAC25G_EthTransc25GS10Shim_PhyReg11F_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg121_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg121_SET_PmaRxBufConstGmEn(RegData,
                                                                  EMAC25G_CGMEN_CGM_EN_3);
        EMAC25G_EthTransc25GS10Shim_PhyReg121_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg12D_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg12D_SET_PmaRxDfeLatchXcoupleDisable(
                                                      RegData, EMAC25G_LATCHXCDIS_ENABLE);
        EMAC25G_EthTransc25GS10Shim_PhyReg12D_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg12E_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg12E_SET_PmaRxBufCtleTiaIsel(RegData,
                                                             EMAC25G_IBTIABW_IB_TIA_BW_1);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg12E_SET_PmaRxBufEqSel(RegData,
                                                                  EMAC25G_EQSEL_EQ_SEL_1);
        EMAC25G_EthTransc25GS10Shim_PhyReg12E_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg130_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg130_SET_PmaRxDfeTapsumBwSel(RegData,
                                                             EMAC25G_TAPSUMBW_TAPSUM_BW1);
        EMAC25G_EthTransc25GS10Shim_PhyReg130_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg132_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg132_SET_CdrPllSetCdrVcoSpeedFix1(
                                          RegData, EMAC25G_VCOSPEEDFIX1_VCO_SPEED_FIX1_5);
        EMAC25G_EthTransc25GS10Shim_PhyReg132_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg135_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg135_SET_CdrPllLfResistorPfd(RegData,
                                                                  EMAC25G_LFPFD_LF_PFD_2);
        EMAC25G_EthTransc25GS10Shim_PhyReg135_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg136_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg136_SET_CdrPllSetCdrVcoSpeedFix2(
                                         RegData, EMAC25G_VCOSPEEDFIX2_VCO_SPEED_FIX2_12);
        EMAC25G_EthTransc25GS10Shim_PhyReg136_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg137_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg137_SET_CdrPllSetCdrVcoSpeed(RegData,
                                                            EMAC25G_VCOSPEED_VCO_SPEED_2);
        EMAC25G_EthTransc25GS10Shim_PhyReg137_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg139_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg139_SET_CdrPllChgpmpCurrentPfd(
                                             RegData, EMAC25G_CHGPMPPFD_CP_CURRENT_PFD_2);
        EMAC25G_EthTransc25GS10Shim_PhyReg139_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg13A_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg13A_SET_CdrPllPdLCounter(RegData, 3);
        EMAC25G_EthTransc25GS10Shim_PhyReg13A_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg13B_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg13B_SET_CdrPllMcntDiv(RegData, 16);
        EMAC25G_EthTransc25GS10Shim_PhyReg13B_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg144_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg144_SET_PmaRxOdiMonitorBwSel1(RegData,
                                                                EMAC25G_ODIMONBWSEL_BW_4);
        EMAC25G_EthTransc25GS10Shim_PhyReg144_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg145_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg145_SET_PmaRxOdiMonitorBwSel2(RegData,
                                                                EMAC25G_ODIMONBWSEL_BW_4);
        EMAC25G_EthTransc25GS10Shim_PhyReg145_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg151_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg151_SET_PmaRxBufEqCdgenSel(RegData,
                                                           EMAC25G_EQCDGENSEL_EQ_CDGEN_2);
        EMAC25G_EthTransc25GS10Shim_PhyReg151_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg154_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg154_SET_PmaRxOdiVregVoltageSel(
                                                     RegData, EMAC25G_VREGVOLTSEL_VREG_0);
        EMAC25G_EthTransc25GS10Shim_PhyReg154_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg160_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg160_SET_PmaAdaptAdpAcCtleInitialValue(
                                              RegData, EMAC25G_ADPACVALUE_ADP_AC_VALUE_3);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg160_SET_PmaAdaptAdpDcCtleInitialValue(
                                              RegData, EMAC25G_ADPDCVALUE_ADP_DC_VALUE_2);
        EMAC25G_EthTransc25GS10Shim_PhyReg160_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg162_READ(pBc);
        RegData = 
                 EMAC25G_EthTransc25GS10Shim_PhyReg162_SET_PmaAdaptAdpDfeTap1InitialValue(
                 RegData, EMAC25G_ADPDFETAP1VALUE_ADP_DFE_TAP1_VALUE_0);
        EMAC25G_EthTransc25GS10Shim_PhyReg162_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg165_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg165_SET_PmaAdaptAdpDcCtleMode2H2Limit(
                                                                              RegData, 7);
        EMAC25G_EthTransc25GS10Shim_PhyReg165_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg167_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg167_SET_PmaAdaptAdpVgaInitialValue(
                                            RegData, EMAC25G_ADPVGAVALUE_ADP_VGA_VALUE_1);
        EMAC25G_EthTransc25GS10Shim_PhyReg167_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg22E_READ(pBc);
        RegData = 
              EMAC25G_EthTransc25GS10Shim_PhyReg22E_SET_HssiAibcrTxAibDllStrAlignDyCtlsel(
                                           RegData, EMAC25G_DLLSTRALIGNCYCTLSEL_CTLSEL_1);
        EMAC25G_EthTransc25GS10Shim_PhyReg22E_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg230_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg230_SET_HssiAibcrRxAibRxDccByp(
                                                   RegData, EMAC25G_TXAIBTXDCCBYP_ENABLE);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg230_SET_HssiAibcrRxDccEn(RegData, 
                                                                 EMAC25G_TXDCCEN_DISABLE);
        EMAC25G_EthTransc25GS10Shim_PhyReg230_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg32C_READ(pBc);
        RegData = 
              EMAC25G_EthTransc25GS10Shim_PhyReg32C_SET_HssiAibndRxAibDllstrAlignDyCtlsel(
              RegData, EMAC25G_DLLSTRALIGNCYCTLSEL_CTLSEL_1);
        EMAC25G_EthTransc25GS10Shim_PhyReg32C_WRITE(pBc, RegData);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg32E_READ(pBc);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg32E_SET_HssiAibndTxAibTxDccByp(
                                                   RegData, EMAC25G_TXAIBTXDCCBYP_ENABLE);
        RegData = EMAC25G_EthTransc25GS10Shim_PhyReg32E_SET_HssiAibndTxDccEn(RegData, 
                                                                 EMAC25G_TXDCCEN_DISABLE);
        EMAC25G_EthTransc25GS10Shim_PhyReg32E_WRITE(pBc, RegData);
    }
    else
        DT_ASSERT(FALSE);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_SetNetworkRateDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcEMAC25G_SetNetworkRateDpc(DtDpcArgs* pArgs)
{
    DtBcEMAC25G* pBc = (DtBcEMAC25G*)pArgs->m_pContext;
    DtBcEMAC25G_SetNetworkRate(pBc, pArgs->m_Data1.m_UInt32_1);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G_SetNetworkRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcEMAC25G_SetNetworkRate(DtBcEMAC25G* pBc, Int LinkSpeed)
{
    UInt32 RegData;
    Int WaitTimeoutMs;
    UInt32 RefClkSource;
    UInt32 X1ClockSelect;
    UInt32 XnClockSelect;

    // Check if speed is current speed and we can return.
    if (LinkSpeed == pBc->m_LinkSpeedCurr)
    {
        DtDbgOutBc(AVG, EMAC25G, pBc, "Skipping setting rate. Rate already set to:%i", LinkSpeed);
        return DT_STATUS_OK;
    }

    DtDbgOutBc(AVG, EMAC25G, pBc, "Start setting rate to:%i", LinkSpeed);
    DT_ASSERT(LinkSpeed == EMAC25G_LINKSPEED_10G || LinkSpeed == EMAC25G_LINKSPEED_25G);
    
    // Set MAC in reset
    RegData = EMAC25G_Control_READ(pBc);
    RegData = EMAC25G_Control_SET_FlushFifos(RegData, 1); // Pulse signal
    RegData = EMAC25G_Control_SET_TxRst(RegData, EMAC25G_RESET_ASSERT);
    RegData = EMAC25G_Control_SET_RxRst(RegData, EMAC25G_RESET_ASSERT);
    RegData = EMAC25G_Control_SET_ChannelReset(RegData, EMAC25G_RESET_ASSERT);
    EMAC25G_Control_WRITE(pBc, RegData);
 
    // Get tranceiver access and disable auto adaptation triggering for
    // EX PMA CTLE/DFE mode
    EMAC25G_EthMac25GS10_PhyTlkitAccess_WRITE_PhyTlkitAccess(pBc, 1);

    // Disable background calibration
    RegData = EMAC25G_EthTransc25GS10Shim_PhyReg542_READ(pBc);
    RegData = EMAC25G_EthTransc25GS10Shim_PhyReg542_SET_EnableBackgroundCal(RegData, 
                                                             EMAC25G_BGCALENABLE_DISABLE);
    EMAC25G_EthTransc25GS10Shim_PhyReg542_WRITE(pBc, RegData);

    // Prepare reconfiguration
    RegData = EMAC25G_EthTransc25GS10Shim_PmaCalibrationControl_READ(pBc);

    // Ask PreSICE to prepare the CDR PLL for reconfiguration by setting the pre_config bit
    RegData = EMAC25G_EthTransc25GS10Shim_PmaCalibrationControl_SET_PreReconfig(RegData, 
                                                              EMAC25G_PREREC_USE_SETTING);
    EMAC25G_EthTransc25GS10Shim_PmaCalibrationControl_WRITE(pBc, RegData);

    // Request access to the transceiver internal reconfiguration bus
    RegData = EMAC25G_EthTransc25GS10Shim_PmaArbitrationControl_READ(pBc);
    RegData = EMAC25G_EthTransc25GS10Shim_PmaArbitrationControl_SET_PcsArbiterCtrl(
                                                            RegData, EMAC25G_ARBREQ_USER);
    EMAC25G_EthTransc25GS10Shim_PmaArbitrationControl_WRITE(pBc, RegData);

    // Wait max. 5ms
    WaitTimeoutMs = 5;
    WAIT_UNTIL_READ(EMAC25G_EthTransc25GS10Shim_PhyReg481_READ, 
                                     EMAC25G_EthTransc25GS10Shim_PhyReg481_GET_ChAvmmBusy,
                                     EMAC25G_ARBOWNER_USER, WaitTimeoutMs,
                                     "ERROR: Waiting for Transceiver-AvmBusy timed out.");
    
    // Update the transceiver paramaters
    DtBcEMAC25G_SetTransceiverParameters(pBc, LinkSpeed);

    // Inform EMAC25G for the link speed
    RegData = EMAC25G_Control_READ(pBc);
    RegData = EMAC25G_Control_SET_SpeedSel(RegData, LinkSpeed);
    EMAC25G_Control_WRITE(pBc, RegData);
    
    // Set the reference clock source:
    // Read the logical to physical PLL mapping
    RegData = EMAC25G_EthTransc25GS10Shim_PhyReg117_READ(pBc);
    if (LinkSpeed == EMAC25G_LINKSPEED_25G)
        RefClkSource = 
                    EMAC25G_EthTransc25GS10Shim_PhyReg117_GET_Scratch0X1ClockSrc(RegData);
    else
        RefClkSource = 
                    EMAC25G_EthTransc25GS10Shim_PhyReg117_GET_Scratch1X1ClockSrc(RegData);
    
    
    // Calculate and configure the clock source selection
    X1ClockSelect = ((RefClkSource & 0x08) << 1) | RefClkSource;
    if ((RefClkSource & 0x08) == 0x08)
        XnClockSelect = ((~RefClkSource & 0x08) >> 1) | (RefClkSource & 0x03);
    else
        XnClockSelect = (~RefClkSource & 0x08) >> 1;

    
    RegData = EMAC25G_EthTransc25GS10Shim_PhyReg111_READ(pBc);
    RegData = EMAC25G_EthTransc25GS10Shim_PhyReg111_SET_X1ClockSourceSelect(RegData, 
                                                                           X1ClockSelect);
    RegData = EMAC25G_EthTransc25GS10Shim_PhyReg111_SET_XnClockSourceSelect(RegData, 
                                                                           XnClockSelect);
    EMAC25G_EthTransc25GS10Shim_PhyReg111_WRITE(pBc, RegData);
    
    // Recalibrate
    RegData = EMAC25G_EthTransc25GS10Shim_PmaCalibrationControl_READ(pBc);
    RegData = EMAC25G_EthTransc25GS10Shim_PmaCalibrationControl_SET_PmCr2TxRxUcRxCal(
                                                           RegData, EMAC25G_CALEN_CAL_ON);
    RegData = EMAC25G_EthTransc25GS10Shim_PmaCalibrationControl_SET_UxTxCal(RegData,
                                                                    EMAC25G_CALEN_CAL_ON);
    EMAC25G_EthTransc25GS10Shim_PmaCalibrationControl_WRITE(pBc, RegData);

    // Set PreSICE to use the normal CDR charge pump bandwidth
    RegData = EMAC25G_EthTransc25GS10Shim_PmaCalibrationControl_READ(pBc);
    RegData = EMAC25G_EthTransc25GS10Shim_PmaCalibrationControl_SET_PreReconfig(RegData,
                                                              EMAC25G_PREREC_USE_DEFAULT);
    EMAC25G_EthTransc25GS10Shim_PmaCalibrationControl_WRITE(pBc, RegData);

    // Reconfigure done
    // Give acces to the internal configuration bus back to PreSICE
    RegData = EMAC25G_EthTransc25GS10Shim_PmaArbitrationControl_READ(pBc);
    RegData = EMAC25G_EthTransc25GS10Shim_PmaArbitrationControl_SET_PcsArbiterCtrl(
                                                         RegData, EMAC25G_ARBREQ_PRESICE);
    EMAC25G_EthTransc25GS10Shim_PmaArbitrationControl_WRITE(pBc, RegData);

    // Wait a little for the calibration status registers to react to start of calibration
    DtWaitBlock(10);
    
    // Wait a maximum of 100ms. 
    // NOTE: typical calibration times are: ATX=25.5ms
    WaitTimeoutMs = 100;
    WAIT_UNTIL_READ(EMAC25G_EthTransc25GS10Shim_PhyReg481_READ,
         EMAC25G_EthTransc25GS10Shim_PhyReg481_GET_ChTxCalBusy, EMAC25G_CALBUSYSTATE_IDLE, 
         WaitTimeoutMs, "ERROR: Waiting for ChTxCalBusy timed out.");
    
    WaitTimeoutMs = 1;
    WAIT_UNTIL_READ(EMAC25G_EthTransc25GS10Shim_PhyReg481_READ,
         EMAC25G_EthTransc25GS10Shim_PhyReg481_GET_ChRxCalBusy, EMAC25G_CALBUSYSTATE_IDLE, 
         WaitTimeoutMs, "ERROR: Waiting for ChRxCalBusy timed out.");

    // Release transceiver access:
    // Enable background calibration
    RegData = EMAC25G_EthTransc25GS10Shim_PhyReg542_READ(pBc);
    RegData = EMAC25G_EthTransc25GS10Shim_PhyReg542_SET_EnableBackgroundCal(RegData,
                                                              EMAC25G_BGCALENABLE_ENABLE);
    EMAC25G_EthTransc25GS10Shim_PhyReg542_WRITE(pBc, RegData);
    // Enable auto adaptation triggering for RX PMA CTLE/DFE mode
    EMAC25G_EthMac25GS10_PhyTlkitAccess_WRITE_PhyTlkitAccess(pBc, 0);
    
    // Release MAC resets
    RegData = EMAC25G_Control_READ(pBc);
    RegData = EMAC25G_Control_SET_TxRst(RegData, EMAC25G_RESET_DEASSERT);
    RegData = EMAC25G_Control_SET_RxRst(RegData, EMAC25G_RESET_DEASSERT);
    RegData = EMAC25G_Control_SET_ChannelReset(RegData, EMAC25G_RESET_DEASSERT);
    EMAC25G_Control_WRITE(pBc, RegData);
    
    pBc->m_LinkSpeedCurr = LinkSpeed;
    DtDbgOutBc(AVG, EMAC25G, pBc, "Link speed set to: %d", pBc->m_LinkSpeedCurr);
    return DT_STATUS_OK;
}