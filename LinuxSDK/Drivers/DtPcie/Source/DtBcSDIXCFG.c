//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIXCFG.c *#*#*#*#*#*#*#*# (C) 2017-2018 DekTec
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
#include "DtBcSDIXCFG.h"
#include "DtBcSDIXCFG_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIXCFG implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define BUSY_TIMEOUT   100          // 100*10 microseconds
#define DATA_ML_CNT_ADDR  0x0E      // Address of the Data MLCounter

// MACRO with default precondition checks for the Bc
#define BC_SDIXCFG_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDIXCFG))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SDIXCFG_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SDIXCFG, pBc)

// Helper macro to cast a DtBc* to a DtBcSDIXCFG*
#define BC_SDIXCFG         ((DtBcSDIXCFG*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSDIXCFG_Init(DtBc*);
static DtStatus  DtBcSDIXCFG_SetClock(DtBcSDIXCFG * pBc, Int LogChanNum, Int Mode);
static DtStatus  DtBcSDIXCFG_SetRate(DtBcSDIXCFG * pBc, Int LogChanNum, Int Mode);
static DtStatus  DtBcSDIXCFG_ReadData(DtBcSDIXCFG* pBc, UInt LogChan,
                                                                UInt Offset, UInt* pData);
static DtStatus  DtBcSDIXCFG_WriteData(DtBcSDIXCFG* pBc, UInt LogChan,
                                                                  UInt Offset, UInt Data);
static DtStatus  DtBcSDIXCFG_StreamerWaitForCompletion(DtBcSDIXCFG* pBc);
static DtStatus  DtBcSDIXCFG_PllWaitForCompletion(DtBcSDIXCFG* pBc);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIXCFG - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSDIXCFG_Close(DtBc*  pBc)
{
     BC_SDIXCFG_DEFAULT_PRECONDITIONS(pBc);
        
    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcSDIXCFG*  DtBcSDIXCFG_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_SDIXCFG_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSDIXCFG, Id, DT_BLOCK_TYPE_SDIXCFG,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSDIXCFG_Close;
    OpenParams.m_InitFunc = DtBcSDIXCFG_Init;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSDIXCFG*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG_GetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIXCFG_GetConfig(DtBcSDIXCFG* pBc, DtSdiXCfgChanConfig* pConfig,
                                                            Int MaxNumToGet, Int* pNumGot)
{
    Int i;
    // Sanity check
    BC_SDIXCFG_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDIXCFG_MUST_BE_ENABLED(pBc);

    // Parameter check
    if (pConfig==NULL || pNumGot==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    if (MaxNumToGet < pBc->m_NumLogChannels)
        return DT_STATUS_BUF_TOO_SMALL;

    // Copy the configuration
    for (i=0; i<pBc->m_NumLogChannels; i++)
        pConfig[i] = pBc->m_ChanConfig[i];

    *pNumGot = pBc->m_NumLogChannels;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG_GetLogChannel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIXCFG_GetLogChannel(DtBcSDIXCFG* pBc, Int PortIndex, Int ChanType, 
                                                                         Int* pLogChanNum)
{
    Int i;

    *pLogChanNum = -1;
    
    // Sanity check
    BC_SDIXCFG_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDIXCFG_MUST_BE_ENABLED(pBc);

    // Parameter check
    if (PortIndex < 0)
        return DT_STATUS_INVALID_PARAMETER;
    if (ChanType!=DT_SDIXCFG_CHANTYPE_RX_CHAN && ChanType!=DT_SDIXCFG_CHANTYPE_TX_CHAN 
                                                  && ChanType!=DT_SDIXCFG_CHANTYPE_TX_PLL)
        return DT_STATUS_INVALID_PARAMETER;

    // Find the logical channel number
    for (i=0; i<pBc->m_NumLogChannels; i++)
    {
        if (pBc->m_ChanConfig[i].m_ChanType==ChanType
                                           && pBc->m_ChanConfig[i].m_PortIndex==PortIndex)
        {
            // Found
            *pLogChanNum = pBc->m_ChanConfig[i].m_LogChanNum;
            return DT_STATUS_OK;
       }

    }

    return DT_STATUS_NOT_FOUND;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG_SetMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIXCFG_SetMode(DtBcSDIXCFG * pBc, Int LogChanNum, Int Mode)
{
    // Sanity check
    BC_SDIXCFG_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDIXCFG_MUST_BE_ENABLED(pBc);

    // Parameter check
    if (LogChanNum<0 || LogChanNum >= pBc->m_NumLogChannels)
    {
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Invalid log channel: %d (NumChannels: %d)",
                                                       LogChanNum, pBc->m_NumLogChannels);
        return DT_STATUS_INVALID_PARAMETER;
    }

    if (Mode==DT_SDIXCFG_MODE_3G || Mode==DT_SDIXCFG_MODE_1G5)
    {
        return DtBcSDIXCFG_SetRate(pBc, LogChanNum, Mode);
    }
    else if (Mode==DT_SDIXCFG_MODE_NON_FRAC || Mode==DT_SDIXCFG_MODE_FRAC)
    {
        return DtBcSDIXCFG_SetClock(pBc, LogChanNum, Mode);
    }
    else
    {
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Invalid Mode: %d", Mode);
        return DT_STATUS_INVALID_PARAMETER;
    }
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIXCFG - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSDIXCFG_Init(DtBc*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;
    Int  Idx;
    Int  NumChannels;

    // Sanity checks
    BC_SDIXCFG_DEFAULT_PRECONDITIONS(pBc);

    // Initialize spinlock
    DtSpinLockInit(&BC_SDIXCFG->m_ConfigLock);

    // Read the configuration
    NumChannels = (Int)SDIXCFG_Config_READ_NumReconfigs(BC_SDIXCFG);
    DT_ASSERT(NumChannels <= DT_BC_SDIXCFG_MAX_NUM_LOG_CHAN);
    BC_SDIXCFG->m_NumLogChannels = (NumChannels < DT_BC_SDIXCFG_MAX_NUM_LOG_CHAN)
                                           ? NumChannels : DT_BC_SDIXCFG_MAX_NUM_LOG_CHAN;

    for (Idx=0; Idx<NumChannels  && DT_SUCCESS(Status); Idx++)
    { 
        Int ChType;
 //       UInt i;

        // Read config 
        UInt32  RegConfig = SDIXCFG_Config_READ(BC_SDIXCFG, Idx);

        // Set Logical channel number
        BC_SDIXCFG->m_ChanConfig[Idx].m_LogChanNum = Idx;
        // Get associated port-index
        BC_SDIXCFG->m_ChanConfig[Idx].m_PortIndex = SDIXCFG_Config_GET_PortIdx(RegConfig);
        // Get channel type
        switch (SDIXCFG_Config_GET_ChannelType(RegConfig))
        {
        default:
        case SDIXCFG_CHANTYPE_NOT_USED: ChType = DT_SDIXCFG_CHANTYPE_NOT_USED; break;
        case SDIXCFG_CHANTYPE_RX_CHAN:  ChType = DT_SDIXCFG_CHANTYPE_RX_CHAN; break;
        case SDIXCFG_CHANTYPE_TX_CHAN:  ChType = DT_SDIXCFG_CHANTYPE_TX_CHAN; break;
        case SDIXCFG_CHANTYPE_TX_PLL:   ChType = DT_SDIXCFG_CHANTYPE_TX_PLL; break;
        }
        BC_SDIXCFG->m_ChanConfig[Idx].m_ChanType = ChType;

        // Fill cached data ml counters
        if (ChType==DT_SDIXCFG_CHANTYPE_RX_CHAN || ChType==DT_SDIXCFG_CHANTYPE_TX_CHAN)
            Status = DtBcSDIXCFG_ReadData(BC_SDIXCFG, Idx, DATA_ML_CNT_ADDR,
                                                       &BC_SDIXCFG->m_DataMlCounter[Idx]);
        else
            BC_SDIXCFG->m_DataMlCounter[Idx] = 0;

        //for (i=0; i<32; i++)
        //{
        //    UInt32 Data;
        //    Status = DtBcSDIXCFG_ReadData(BC_SDIXCFG, Idx, i, &Data);
        //    if (!DT_SUCCESS(Status))
        //        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Read data failed");
        //    DtDbgOutBc(MIN, SDIXCFG, pBc, "Read LogChannelNum: %d  Offset: %d  Data: 0x%x",
        //        Idx, i, Data);
        //}

    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG_SetClock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIXCFG_SetClock(DtBcSDIXCFG * pBc, Int LogChanNum, Int Mode)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    BC_SDIXCFG_DEFAULT_PRECONDITIONS(pBc);

 
    // Parameter check
    if (LogChanNum<0 || LogChanNum >= pBc->m_NumLogChannels)
    {
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Invalid log channel: %d (NumChannels: %d)",
                                                       LogChanNum, pBc->m_NumLogChannels);
        return DT_STATUS_INVALID_PARAMETER;
    }
    if (pBc->m_ChanConfig[LogChanNum].m_ChanType != SDIXCFG_CHANTYPE_TX_PLL)
    {
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Invalid Mode: %d for this channel ",Mode);
        return DT_STATUS_INVALID_PARAMETER;
    }
    if (Mode!=DT_SDIXCFG_MODE_NON_FRAC && Mode!=DT_SDIXCFG_MODE_FRAC)
    { 
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Invalid Mode: %d for this channel ",Mode);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Prevent mutual reconfiguration
    DtSpinLockAcquire(&pBc->m_ConfigLock);

    // Check busy flag
    if (SDIXCFG_PllControlStatus_READ_Busy(pBc) != 0)
    {
        DtSpinLockRelease(&pBc->m_ConfigLock);
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Reconfiguration is busy");
        return DT_STATUS_BUSY;
    }

    // Set logical channel
    SDIXCFG_PllLogicalChannel_WRITE(pBc, LogChanNum);

    // Set refclk register offset to access
    SDIXCFG_PllOffset_WRITE(pBc, 0x000);

    // Write Data; 0=NonFractional 1=Fractional
    SDIXCFG_PllData_WRITE(pBc, Mode==DT_SDIXCFG_MODE_NON_FRAC ? 0 : 1);
    
    // Execute WRITE
    SDIXCFG_PllControlStatus_WRITE(pBc, 1);

    // Wait for completion
    Status = DtBcSDIXCFG_PllWaitForCompletion(pBc);
    if (!DT_SUCCESS(Status))
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: DtBcSDIXCFG_PllWaitForCompletion failed");

    DtSpinLockRelease(&pBc->m_ConfigLock);


    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG_SetRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIXCFG_SetRate(DtBcSDIXCFG * pBc, Int LogChanNum, Int Mode)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  DataMlCounter;

    // Sanity check
    BC_SDIXCFG_DEFAULT_PRECONDITIONS(pBc);


    DtDbgOutBc(MIN, SDIXCFG, pBc, "LogChannelNum: %d  Mode: %d", LogChanNum, Mode);

    // Parameter check
    if (LogChanNum<0 || LogChanNum >= pBc->m_NumLogChannels)
    {
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Invalid log channel: %d (NumChannels: %d)",
                                                       LogChanNum, pBc->m_NumLogChannels);
        return DT_STATUS_INVALID_PARAMETER;
    }
    if ((pBc->m_ChanConfig[LogChanNum].m_ChanType!=SDIXCFG_CHANTYPE_RX_CHAN)
                  && (pBc->m_ChanConfig[LogChanNum].m_ChanType!=SDIXCFG_CHANTYPE_TX_CHAN))
    {
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Invalid Mode: %d for this channel ",Mode);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Prevent mutual reconfiguration
    DtSpinLockAcquire(&pBc->m_ConfigLock);

    // Get cached DataMlCounter
    DataMlCounter = pBc->m_DataMlCounter[LogChanNum];

    // Modify DataMlCounter
    DataMlCounter &= ~0x0000C000;   // clear bit index 14 and 15
    DataMlCounter &= ~0x00000008;   // clear bit index 3
    DataMlCounter &= ~0x00003FC0;   // clear bit index 6 to 13 
    if (Mode == DT_SDIXCFG_MODE_3G)
    {
        // Configure as 3G-SDI, used to oversample ASI and SD-SDI as well
        DataMlCounter |= (0x5 << 10);   // set to 3Gbps
        DataMlCounter |= (0x5 << 6);    // set to 3Gbps
    }
    else 
    {
        // Configure as HD-SDI
        DataMlCounter |= (0x3 << 10);   // set to 1.5Gbps
        DataMlCounter |= (0xA << 6);    // set to 1.5Gbps
    } 

    // Need to update configuration?
    if (pBc->m_DataMlCounter[LogChanNum] != DataMlCounter)
    {
 //       UInt i=0;
        // Write MlCounter
        DtDbgOutBc(MIN, SDIXCFG, pBc, "Write LogChannelNum: %d  Offset: %d  Data: 0x%x", 
                                             LogChanNum, DATA_ML_CNT_ADDR, DataMlCounter);
        Status = DtBcSDIXCFG_WriteData(pBc, LogChanNum, DATA_ML_CNT_ADDR, DataMlCounter);
        if (!DT_SUCCESS(Status))
        {
            DtSpinLockRelease(&pBc->m_ConfigLock);
            DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Write data failed");
            return DT_STATUS_BUSY;
        }
        pBc->m_DataMlCounter[LogChanNum] = DataMlCounter;

        //for (i=0; i<32; i++)
        //{
        //    UInt32 Data;
        //    Status = DtBcSDIXCFG_ReadData(BC_SDIXCFG, LogChanNum, i, &Data);
        //    if (!DT_SUCCESS(Status))
        //        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Read data failed");
        //    DtDbgOutBc(MIN, SDIXCFG, pBc, "Read LogChannelNum: %d  Offset: %d  Data: 0x%x", 
        //                                                             LogChanNum, i, Data);
        //}
    }

    DtSpinLockRelease(&pBc->m_ConfigLock);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG_ReadData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIXCFG_ReadData(DtBcSDIXCFG* pBc, UInt LogChanNum, UInt Offset, 
                                                                              UInt* pData)
{
    UInt32 RegCtrlStatus;

    // Check busy flag
    if (SDIXCFG_StreamerControlStatus_READ_Busy(pBc) != 0)
    {
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Reconfiguration is busy");
        return DT_STATUS_BUSY;
    }

    // Set logical channel number
    SDIXCFG_StreamerLogicalChannel_WRITE(pBc, (UInt32)LogChanNum);

    // Enable direct mode
    RegCtrlStatus = SDIXCFG_StreamerControlStatus_SET_Mode(0, SDIXCFG_SM_DIRECT);
    SDIXCFG_StreamerControlStatus_WRITE(pBc, RegCtrlStatus);

    // Wait for completion
    DT_RETURN_ON_ERROR(DtBcSDIXCFG_StreamerWaitForCompletion(pBc));

    // Set offet register
    SDIXCFG_StreamerOffset_WRITE(pBc, Offset);

    // Execute Read
    RegCtrlStatus = SDIXCFG_StreamerControlStatus_SET_Mode(0, SDIXCFG_SM_DIRECT);
    RegCtrlStatus = SDIXCFG_StreamerControlStatus_SET_Read(RegCtrlStatus, 1);
    SDIXCFG_StreamerControlStatus_WRITE(pBc, RegCtrlStatus);

    // Wait for completion
    DT_RETURN_ON_ERROR(DtBcSDIXCFG_StreamerWaitForCompletion(pBc));

    // Read Data
    *pData = SDIXCFG_StreamerData_READ(pBc);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG_WriteData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIXCFG_WriteData(DtBcSDIXCFG* pBc, UInt LogChanNum, UInt Offset, UInt Data)
{
    UInt32 RegCtrlStatus;

    // Check busy flag
    if (SDIXCFG_StreamerControlStatus_READ_Busy(pBc) != 0)
    {
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Reconfiguration is busy");
        return DT_STATUS_BUSY;
    }

    // Set logical channel number
    SDIXCFG_StreamerLogicalChannel_WRITE(pBc, (UInt32)LogChanNum*2); //TODOTD remove this

    // Enable direct mode
    RegCtrlStatus = SDIXCFG_StreamerControlStatus_SET_Mode(0, SDIXCFG_SM_DIRECT);
    SDIXCFG_StreamerControlStatus_WRITE(pBc, RegCtrlStatus);

    DT_RETURN_ON_ERROR(DtBcSDIXCFG_StreamerWaitForCompletion(pBc));

    // Set offet register
    SDIXCFG_StreamerOffset_WRITE(pBc, Offset);

    // Write Data
    SDIXCFG_StreamerData_WRITE(pBc, Data);

    // Execute WRITE
    RegCtrlStatus = SDIXCFG_StreamerControlStatus_SET_Mode(0, SDIXCFG_SM_DIRECT);
    RegCtrlStatus = SDIXCFG_StreamerControlStatus_SET_Write(RegCtrlStatus, 1);
    SDIXCFG_StreamerControlStatus_WRITE(pBc, RegCtrlStatus);

    // Wait for completion
    DT_RETURN_ON_ERROR(DtBcSDIXCFG_StreamerWaitForCompletion(pBc));

    // Perform a second write TODOTD workaround 
    SDIXCFG_StreamerData_WRITE(pBc, Data);
    SDIXCFG_StreamerControlStatus_WRITE(pBc, RegCtrlStatus);

    // Wait for completion
    DT_RETURN_ON_ERROR(DtBcSDIXCFG_StreamerWaitForCompletion(pBc));

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG_StreamerWaitForCompletion -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIXCFG_StreamerWaitForCompletion(DtBcSDIXCFG* pBc)
{
    Int TimeoutCnt = BUSY_TIMEOUT;
    while (SDIXCFG_StreamerControlStatus_READ_Busy(pBc)!=0 && TimeoutCnt>0)
    {
        DtWaitBlock(2);
        TimeoutCnt--;
    }
    // Timeout?
    if (SDIXCFG_StreamerControlStatus_READ_Busy(pBc) != 0)
    {
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR:Busy timeout");
        return DT_STATUS_TIMEOUT;
    }
    // Failed?
    if (SDIXCFG_StreamerControlStatus_READ_Error(pBc) != 0)
    {
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Error indication");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIXCFG_PllWaitForCompletion -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIXCFG_PllWaitForCompletion(DtBcSDIXCFG* pBc)
{
    Int TimeoutCnt = BUSY_TIMEOUT;
    while (SDIXCFG_PllControlStatus_READ_Busy(pBc)!=0 && TimeoutCnt>0)
    {
        DtWaitBlock(2);
        TimeoutCnt--;
    }
    // Timeout?
    if (SDIXCFG_PllControlStatus_READ_Busy(pBc) != 0)
    {
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Busy timeout");
        return DT_STATUS_TIMEOUT;
    }
    // Failed?
    if (SDIXCFG_PllControlStatus_READ_Error(pBc) != 0)
    {
        DtDbgOutBc(ERR, SDIXCFG, pBc, "ERROR: Error indication");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIXCFG implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcSDIXCFG
#define SDIXCFG_STUB   ((DtIoStubBcSDIXCFG*)pStub)
#define SDIXCFG_BC  ((DtBcSDIXCFG*)SDIXCFG_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcSDIXCFG_AppendDynamicSize(const DtIoStub*,
                                                                       DtIoStubIoParams*);
static DtStatus  DtIoStubBcSDIXCFG_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcSDIXCFG_OnCmdGetConfig(const DtIoStubBcSDIXCFG*, 
                                               const DtIoctlSdiXCfgCmdGetConfigInput*,
                                               DtIoctlSdiXCfgCmdGetConfigOutput*);
static DtStatus  DtIoStubBcSDIXCFG_OnCmdSetMode(const DtIoStubBcSDIXCFG*, 
                                                const DtIoctlSdiXCfgCmdSetModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_SDIXCFG;

static const DtIoctlProperties  IOSTUB_BC_SDIXCFG_IOCTLS[] = 
{
    DT_IOCTL_PROPS_SDIXCFG_CMD(
        DtIoStubBcSDIXCFG_OnCmd,
        DtIoStubBcSDIXCFG_AppendDynamicSize,
        NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIXCFG - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIXCFG_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcSDIXCFG_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIXCFG));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIXCFG_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcSDIXCFG*  DtIoStubBcSDIXCFG_Open(DtBc*  pBc)
{
    DtIoStubBcSDIXCFG*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDIXCFG));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcSDIXCFG, pBc, NULL, 
                                                            DtIoStubBcSDIXCFG_Close,
                                                            NULL,  // Use default IOCTL
                                                            IOSTUB_BC_SDIXCFG_IOCTLS);
    pStub = (DtIoStubBcSDIXCFG*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIXCFG- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIXCFG_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcSDIXCFG_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSdiXCfgCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIXCFG));
    DT_ASSERT(pIoParams!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_SDIXCFG_CMD);
    
    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SdiXCfgCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_SDIXCFG_CMD_GET_CONFIG:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlSdiXCfgCmdGetConfigInput));
        DT_ASSERT(pIoParams->m_OutReqSize 
                                         >= sizeof(DtIoctlSdiXCfgCmdGetConfigOutput));
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += (pInData->m_GetConfig.m_MaxNumToGet
                                                       * sizeof(DtSdiXCfgChanConfig));
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIXCFG_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIXCFG_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams* pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSdiXCfgCmdInput*  pInData = NULL;
    DtIoctlSdiXCfgCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIXCFG));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_SDIXCFG_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, SDIXCFG, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SdiXCfgCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SdiXCfgCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_SDIXCFG_CMD_GET_CONFIG:
        DT_ASSERT(pInData != NULL);
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIXCFG_OnCmdGetConfig(SDIXCFG_STUB,
                                           &pInData->m_GetConfig, &pOutData->m_GetConfig);
        break;
    case DT_SDIXCFG_CMD_SET_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDIXCFG_OnCmdSetMode(SDIXCFG_STUB,
                                                                     &pInData->m_SetMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIXCFG_OnCmdGetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIXCFG_OnCmdGetConfig(
    const DtIoStubBcSDIXCFG* pStub,
    const DtIoctlSdiXCfgCmdGetConfigInput* pInData,
    DtIoctlSdiXCfgCmdGetConfigOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIXCFG));
    DT_ASSERT(pInData != NULL);
    DT_ASSERT(pOutData != NULL);
    return DtBcSDIXCFG_GetConfig(SDIXCFG_BC, pOutData->m_Buf, 
                                             pInData->m_MaxNumToGet, &pOutData->m_NumGot);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIXCFG_OnCmdSetMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIXCFG_OnCmdSetMode(
    const DtIoStubBcSDIXCFG* pStub,
    const DtIoctlSdiXCfgCmdSetModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIXCFG));
    DT_ASSERT(pInData != NULL);

    return DtBcSDIXCFG_SetMode(SDIXCFG_BC, pInData->m_LogChanNum, pInData->m_Mode);
}
