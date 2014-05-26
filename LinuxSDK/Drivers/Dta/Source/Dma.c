//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dma.c *#*#*#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtaIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Internal Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// State Defines
#define  DTA_DMA_STATE_IDLE         0
#define  DTA_DMA_STATE_INIT         1
//#define  DTA_DMA_STATE_INIT_START   2
#define  DTA_DMA_STATE_STARTED      4
#define  DTA_DMA_STATE_ABORT        8

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Forward declarations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

void  DtaDmaCompletedDpc(DtDpcArgs* pArgs);
DtStatus  DtaDmaFillSgList(DmaChannel* pDmaCh, UInt DataOffset, UInt DataLength, 
                                         UInt LocalAddressCurrent, UInt LocalAddressStart,
                                         UInt LocalAddressEnd);
static DtStatus  DtaDmaInitTransfer(DmaChannel* pDmaCh, DtPageList* pPageList, 
                                    UInt BufType, UInt Direction, void* pBuffer, 
                                    UInt TransferSize, UInt TransferOffset, 
                                    UInt8* pLocalAddress, UInt LocalAddressBufStart, 
                                    UInt LocalAddressBufEnd, Bool ReUseDataBuffer);
static DtStatus  DtaDmaProgramTransfer(DmaChannel* pDmaCh);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ExecuteDmaCompletedFromDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  ExecuteDmaCompletedFromDpc(DmaChannel* pDmaChannel)
{
    DtDpcArgs  Args;
    
    Args.m_pContext = pDmaChannel;
    DtaDmaCompletedDpc(&Args);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaDmaInit(
    DtaDeviceData*  pDvcData)
{   
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    DtStatus  Status = DT_STATUS_OK;
    DmaOpt*  pDmaOptions = &pDvcData->m_DmaOptions;
    Int  NumNonSharedDmaChannels = 0;
    Int  i;
        

    // Get DMA properties
    pDmaOptions->m_UseDmaInFpga = DtPropertiesGetBool(pPropData, "DMA_IN_FPGA", -1);
    pDmaOptions->m_Supports64Bit = DtPropertiesGetBool(pPropData, "DMA_SUPPORTS_64BIT",
                                                                                      -1);

    if (pDvcData->m_DevInfo.m_UsesPlxChip)
    {
        pDmaOptions->m_PlxDmaChannelCount = DtPropertiesGetInt(pPropData, 
                                                                "PLX_DMA_CHAN_COUNT", -1);
        pDmaOptions->m_PlxDmaThreshold = DtPropertiesGetUInt32(pPropData, 
                                                                 "PLX_DMA_THRESHOLD", -1);
    }
    
    // Check if no property error occurred
    Status = DtaPropertiesReportDriverErrors(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    DT_ASSERT(pDvcData->m_DevInfo.m_UsesPlxChip || pDmaOptions->m_UseDmaInFpga);
    
    if (pDvcData->m_DevInfo.m_UsesPlxChip)
    {
        pDmaOptions->m_PlxDmaChannelPort[0] = -1;
        pDmaOptions->m_PlxDmaChannelPort[1] = -1;
        
        DT_ASSERT(pDmaOptions->m_PlxDmaChannelCount>0 && 
                                                    pDmaOptions->m_PlxDmaChannelCount<=2);

        for (i=0; i<DTA_DMA_MAX_PLX_DMA_CONTROLLERS; i++)
        {
            pDmaOptions->m_PlxDmaChannelMode[i] = DtPropertiesGetUInt32(pPropData, 
                                                               (i==0?"PLX_DMA_CHAN1_MODE":
                                                               "PLX_DMA_CHAN2_MODE"), -1);
            pDmaOptions->m_PlxDmaChannelPort[i] = DtPropertiesGetInt(pPropData, 
                                                               (i==0?"PLX_DMA_CHAN1_PORT":
                                                               "PLX_DMA_CHAN2_PORT"), -1);
            if (pDmaOptions->m_PlxDmaChannelPort[i] != -1)
            {
                NumNonSharedDmaChannels++;

                // The port index in the XML file starts form 1. Fix it here.
                DT_ASSERT(pDmaOptions->m_PlxDmaChannelPort[i] != 0);
                pDmaOptions->m_PlxDmaChannelPort[i]--;
            }
        }

        // Check if no property error occurred
        Status = DtaPropertiesReportDriverErrors(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;
    }
    DT_ASSERT((NumNonSharedDmaChannels!=0 && pDvcData->m_DevInfo.m_UsesPlxChip && 
                            !pDmaOptions->m_UseDmaInFpga && 
                            NumNonSharedDmaChannels==pDmaOptions->m_PlxDmaChannelCount) ||
                            (NumNonSharedDmaChannels==0 && pDmaOptions->m_UseDmaInFpga));
    
    // Initialise the DMA adapter
    if (DT_SUCCESS(Status))
        Status = DtDmaInit(&pDvcData->m_Device, &pDvcData->m_DmaDevice, 
                                 DTA_DMA_MAX_TRANSFER_SIZE, pDmaOptions->m_Supports64Bit);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaInitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaDmaInitPowerup(
    DtaDeviceData*  pDvcData)
{   
    DtStatus  Status = DT_STATUS_OK;
    DmaOpt*  pDmaOptions = &pDvcData->m_DmaOptions;

    if (pDvcData->m_DevInfo.m_UsesPlxChip)
    {
        volatile UInt8*  p905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;
        Int  i;
        UInt32  DmaMode;
        
        // Update PLX DMA registers
        // The DMA priority is already written during the generic PLX905X intilialization

        // Write the DMA threshold to the PLX device
        WRITE_UINT32(p905XRegs, PCI905X_DMA_THRESHOLD, pDmaOptions->m_PlxDmaThreshold);
        
        if (pDmaOptions->m_UseDmaInFpga)
        {
            for (i=0; i<pDmaOptions->m_PlxDmaChannelCount; i++)
            {
                // Write the DMA mode to the PLX device. If the device supports 64-bit DMA
                // we have to enable dual address cycle
                DmaMode = pDmaOptions->m_PlxDmaChannelMode[i];
                if (pDmaOptions->m_Supports64Bit)
                    DmaMode |= PCI905X_DMAMODE_DAC;
                else
                    DT_ASSERT((DmaMode&PCI905X_DMAMODE_DAC) == 0);
                    
                WRITE_UINT32(p905XRegs, (i==0?PCI905X_DMA0_MODE:PCI905X_DMA1_MODE), 
                                                                                 DmaMode);
            }
        } else {
            for (i=0; i<DTA_DMA_MAX_PLX_DMA_CONTROLLERS; i++)
            {
                // Skip the DMA channel not used;
                if (pDmaOptions->m_PlxDmaChannelPort[i] == -1)
                    continue;

                // Write the DMA mode to the PLX device. If the device supports 64-bit DMA
                // we have to enable dual address cycle
                DmaMode = pDmaOptions->m_PlxDmaChannelMode[i];
                if (pDmaOptions->m_Supports64Bit)
                    DmaMode |= PCI905X_DMAMODE_DAC;
                else
                    DT_ASSERT((DmaMode&PCI905X_DMAMODE_DAC) == 0);
                    
                WRITE_UINT32(p905XRegs, (i==0?PCI905X_DMA0_MODE:PCI905X_DMA1_MODE), 
                                                                                 DmaMode);
            }
        }
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaInitCh -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// pDmaFinischFunc must be NULL if flags has DTA_DMA_FLAGS_BLOCKING enabled.
// The pDmaFinischFunc function is running at DPC level.
//
DtStatus  DtaDmaInitCh(
    DtaDeviceData*  pDvcData,
    Int  PortIndex,
    UInt  MaxDmaLength,
    UInt  DmaMode,
    UInt  DmaRegsOffset,        // The actual DMA register offset for fpga controlled DMA,
                                // or the DMA controller index if PLX is used.
    UInt  DmaFlags,
    Int  Timeout,               // -1 or 2^14 clock cycles
    DmaCallbackFunc  pDmaFinishFunc,
    void*  pDmaFinishContext,
    DmaChannel*  pDmaCh,
    Bool  FixedLocalAddress)    // TRUE: DMA Local address = fixed
{
    DtStatus  Status = DT_STATUS_OK;
    Int  DmaSglListSize;
    DmaOpt*  pDmaOptions = &pDvcData->m_DmaOptions;

    DT_ASSERT((pDmaFinishFunc!=NULL && ((DmaFlags&DTA_DMA_FLAGS_BLOCKING) == 0))
                                                                 || pDmaFinishFunc==NULL);
    DT_ASSERT((Timeout==-1 && (DmaMode&DTA_DMA_MODE_TIMEOUT_ENABLE)==0) ||
                               (Timeout!=-1 && (DmaMode&DTA_DMA_MODE_TIMEOUT_ENABLE)!=0));
    DT_ASSERT(pDvcData->m_DmaOptions.m_UseDmaInFpga || 
                                                (DmaMode&DTA_DMA_MODE_TIMEOUT_ENABLE)==0);
    DT_ASSERT(pDvcData->m_DmaOptions.m_UseDmaInFpga || 
                                               (DmaMode&DTA_DMA_FLAGS_DESCR_PREFETCH)==0);
    pDmaCh->m_PortIndex = PortIndex;
    pDmaCh->m_pDvcData = pDvcData;
    pDmaCh->m_MaxDmaLength = MaxDmaLength;
    pDmaCh->m_DmaMode = DmaMode;
    pDmaCh->m_RegsOffset = DmaRegsOffset;
    pDmaCh->m_DmaFlags = DmaFlags;
    pDmaCh->m_Timeout = Timeout;
    pDmaCh->m_pDmaFinishFunc = pDmaFinishFunc;
    pDmaCh->m_pDmaFinishContext = pDmaFinishContext;
    
    DT_ASSERT((!pDvcData->m_DmaOptions.m_UseDmaInFpga && pDmaCh->m_RegsOffset<2) || 
                                                   pDvcData->m_DmaOptions.m_UseDmaInFpga);
    pDmaCh->m_FixedLocalAddress = FixedLocalAddress;

    Status = DtaPropertiesReportDriverErrors(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    Status = DtEventInit(&pDmaCh->m_DmaDoneEvent, FALSE);
    if (!DT_SUCCESS(Status))
        return Status;
    
    Status = DtDpcInit(&pDmaCh->m_DmaCompletedDpc, DtaDmaCompletedDpc, TRUE);
    if (!DT_SUCCESS(Status))
        return Status;

    // Check if we have enough map registers to do SGL transfer
    pDmaCh->m_UseDirectBufDma = pDvcData->m_DmaDevice.m_NotEnoughMapRegisters;

    // The size of one SGL element must be less then 8MB.
    //if (pDmaCh->m_UseDirectBufDma && pDmaCh->m_MaxDmaLength>0x7FFFFC)
    //    pDmaCh->m_MaxDmaLength = 0x7FFFFC;

    if ((DmaFlags & DTA_DMA_FLAGS_DATA_BUF_NO_COPY) == 0)
    {
        // Limit max. DMA length to 1MB if using direct buffer DMA so we don't use
        // too much resources.
        if (pDmaCh->m_UseDirectBufDma && pDmaCh->m_MaxDmaLength>0x100000)
            pDmaCh->m_MaxDmaLength = 0x100000;
    }


    pDmaCh->m_Use64BitDma = FALSE;
       
    if (pDmaCh->m_UseDirectBufDma)
        DmaSglListSize = sizeof(SglDesc);
    else {
        Int NumSglElements = (MaxDmaLength / PAGE_SIZE) + 1;
        if (pDmaOptions->m_Supports64Bit)
        {
            pDmaCh->m_Use64BitDma = TRUE;
            DmaSglListSize = sizeof(SglDesc64) * NumSglElements;
        }
        else
            DmaSglListSize = sizeof(SglDesc) * NumSglElements;
    }

    // Create Common buffer to store SGL descriptors
    // The lowest 4 bits are used for status flags in the DMA controller, so the SGL
    // buffer must be aligned to 16 bytes.
    Status = DtDmaCreateDirectBuffer(&pDvcData->m_Device, &pDvcData->m_DmaDevice, 
                                                &pDmaCh->m_SGListBuf, DmaSglListSize, 16);
    if (!DT_SUCCESS(Status))
        return Status;

    // We need a second common buffer if SGL is not possible due to >4GB memory and
    // 32-bit DMA
    if (pDmaCh->m_UseDirectBufDma)
        Status = DtDmaCreateDirectBuffer(&pDvcData->m_Device, &pDvcData->m_DmaDevice, 
                                    &pDmaCh->m_Data.m_DmaData, pDmaCh->m_MaxDmaLength, 4);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaCleanupCh -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaDmaCleanupCh(DtaDeviceData* pDvcData, DmaChannel* pDmaCh)
{
    // free possibly used common buffer
    if (pDmaCh->m_UseDirectBufDma)
        DtDmaFreeDirectBuffer(&pDvcData->m_Device, &pDmaCh->m_Data.m_DmaData);

    // free SGL list
    DtDmaFreeDirectBuffer(&pDvcData->m_Device, &pDmaCh->m_SGListBuf);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaInitChPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaDmaInitChPowerup(
    DmaChannel*  pDmaCh)
{
    DtaDeviceData*  pDvcData = pDmaCh->m_pDvcData;
    
    Int  TypeNumber = pDvcData->m_DevInfo.m_TypeNumber;
    Int  FwVersion = pDvcData->m_DevInfo.m_FirmwareVersion;

    pDmaCh->m_State = DTA_DMA_STATE_IDLE;
        
    if (pDvcData->m_DmaOptions.m_UseDmaInFpga)
    {   // Using DMA controller in FPGA
        pDmaCh->m_pRegBase = pDvcData->m_pGenRegs + pDmaCh->m_RegsOffset;
        pDmaCh->m_pRegDmaMode = 0; // Not used

        // Legacy DMA register interface
        if (TypeNumber==124 && FwVersion==0)
        {
            pDmaCh->m_pRegCmdStat = pDmaCh->m_pRegBase + 0x48;
            pDmaCh->m_pRegDmaDescLow = pDmaCh->m_pRegBase + 0x44;
            pDmaCh->m_pRegDmaDescHigh = 0;  // Not supported
        } else {
            pDmaCh->m_pRegCmdStat = pDmaCh->m_pRegBase + 0x8;
            pDmaCh->m_pRegDmaDescLow = pDmaCh->m_pRegBase + 0x4;
            pDmaCh->m_pRegDmaDescHigh = pDmaCh->m_pRegBase + 0xc;
        }

        if ((pDmaCh->m_DmaMode & DTA_DMA_MODE_TIMEOUT_ENABLE) != 0)
            DtaDmaTimeOutSet(pDmaCh->m_pRegBase, pDmaCh->m_Timeout);
        else // Disable timeout
            DtaDmaTimeOutSet(pDmaCh->m_pRegBase, 0);

        // Enable descriptor prefetch
        if ((pDmaCh->m_DmaFlags & DTA_DMA_FLAGS_DESCR_PREFETCH) != 0)
            DtaDmaDescrPrefetchEn(pDmaCh->m_pRegBase, 1, (pDmaCh->m_Use64BitDma?1:0));

    } else {
        // No DMA controllers in FPGA, only use PLX DMA controllers
        pDmaCh->m_pRegBase = pDmaCh->m_pDvcData->m_Pci905XConfRegs.m_pKernel;
        if (pDmaCh->m_RegsOffset == 0) // DMA channel 0
        {
            pDmaCh->m_pRegCmdStat = pDmaCh->m_pRegBase + PCI905X_DMA0_COMMAND_STAT;
            pDmaCh->m_pRegDmaDescLow = pDmaCh->m_pRegBase + PCI905X_DMA0_DESC_PTR;
            pDmaCh->m_pRegDmaMode = pDmaCh->m_pRegBase + PCI905X_DMA0_MODE;
        } else { // DMA channel 1
            pDmaCh->m_pRegCmdStat = pDmaCh->m_pRegBase + PCI905X_DMA1_COMMAND_STAT;
            pDmaCh->m_pRegDmaDescLow = pDmaCh->m_pRegBase + PCI905X_DMA1_DESC_PTR;
            pDmaCh->m_pRegDmaMode = pDmaCh->m_pRegBase + PCI905X_DMA1_MODE;
        }
        pDmaCh->m_pRegDmaDescHigh = 0;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaPrepareDataBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaDmaPrepareDataBuffer(
    DtPageList*  pPageList,
    void*  pBuffer, 
    UInt  BufSize,
    UInt  BufType,
    DmaChannel*  pDmaCh,
    UInt  Direction)
{
    DtStatus  Status = DT_STATUS_OK;
    DtPageList*  pPageListDma;
    
    pDmaCh->m_DmaDirection = Direction;
    pDmaCh->m_pBuffer = pBuffer;

    // Get PageList pointer
    if (pDmaCh->m_UseDirectBufDma)
    {
        pPageListDma = &pDmaCh->m_Data.m_DmaData.m_PageList;
        Status = DtCreatePageList(pBuffer, BufSize, BufType, pPageListDma);
    } else {
        pPageListDma = &pDmaCh->m_Data.m_OsSgList.m_PageList;
        if (pPageList != NULL)
            Status = DtCopyPageList(pPageList, pPageListDma, FALSE);
        else
            Status = DtCreatePageList(pBuffer, BufSize, BufType, pPageListDma);
    }

    
    // Create or copy pagelist
    if (!DT_SUCCESS(Status))
        return Status;

    if (pDmaCh->m_UseDirectBufDma)
    {
        if (BufType == DT_BUFTYPE_USER)
        {
            // Lock buffer into kernel memory
            Status = DtLockUserBuffer(pPageListDma, pBuffer);
            if (DT_SUCCESS(Status))
                pDmaCh->m_pBuffer = pPageListDma->m_pVirtualKernel;
        }
    } else {
        // Create SGList from buffer
        Status = DtDmaCreateSgList(&pDmaCh->m_pDvcData->m_Device,
                                           &pDmaCh->m_pDvcData->m_DmaDevice, pBuffer,
                                           BufSize, Direction, &pDmaCh->m_Data.m_OsSgList);
    }

    if (!DT_SUCCESS(Status))
        DtDeletePageList(pPageListDma);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaCleanupDataBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaDmaCleanupDataBuffer(
    DmaChannel*  pDmaCh)
{
    DtStatus  Status = DT_STATUS_OK;
    DtPageList*  pPageList;
    
    if (pDmaCh->m_UseDirectBufDma)
    {
        pPageList = &pDmaCh->m_Data.m_DmaData.m_PageList;
        if (pPageList->m_BufType == DT_BUFTYPE_USER)
        {
            // Unlock buffer from kernel memory
            Status = DtUnlockUserBuffer(pPageList);
        }
        
    } else {
        // Cleanup buffers
        DtDmaFreeSgList(&pDmaCh->m_pDvcData->m_Device, &pDmaCh->m_Data.m_OsSgList, 
                                                                  pDmaCh->m_DmaDirection);
        pPageList = &pDmaCh->m_Data.m_OsSgList.m_PageList;
    }
    // Delete pagelist    
    DtDeletePageList(pPageList);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaInitTransfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaDmaInitTransfer(
    DmaChannel*  pDmaCh,
    DtPageList*  pPageList,     // Not used if ReUseDataBuffer == TRUE
    UInt  BufType,              // Not used if ReUseDataBuffer == TRUE
    UInt  Direction,            // Not used if ReUseDataBuffer == TRUE 
    void*  pBuffer, 
    UInt  TransferSize,
    UInt  TransferOffset,
    UInt8*  pLocalAddress,      // FIFO address if pDmaCh->m_FixedLocalAddress==TRUE
                                // or memory (slice-)buffer pointer address otherwise
    UInt  LocalAddressBufStart, // Only used if pDmaCh->m_FixedLocalAddress==FALSE
    UInt  LocalAddressBufEnd,   // Only used if pDmaCh->m_FixedLocalAddress==FALSE
    Bool  ReUseDataBuffer)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  OldState;

    // First try to set the init state, and check if we have to abort
    OldState = DtAtomicCompareExchange((Int*)&pDmaCh->m_State, DTA_DMA_STATE_IDLE, 
                                                                      DTA_DMA_STATE_INIT);
    if (OldState == DTA_DMA_STATE_ABORT)
    {
        DtDbgOut(ERR, DTA, "New DMA cancelled due to abort flag");
        return DT_STATUS_CANCELLED;
    }
    DT_ASSERT(OldState == DTA_DMA_STATE_IDLE);

    // Create SglList and pagelist if databuffer is not re-used (HP Transfers)
    if (!ReUseDataBuffer)
        Status = DtaDmaPrepareDataBuffer(pPageList, pBuffer, TransferSize, BufType, 
                                                                       pDmaCh, Direction);
    if (!DT_SUCCESS(Status))
        return Status;

    DT_ASSERT(pDmaCh->m_DmaDirection==DT_DMA_DIRECTION_FROM_DEVICE || (
                                     pDmaCh->m_DmaDirection==DT_DMA_DIRECTION_TO_DEVICE &&
                                     (pDmaCh->m_DmaMode&DTA_DMA_MODE_TIMEOUT_ENABLE)==0));

    DtEventReset(&pDmaCh->m_DmaDoneEvent);

    // Set destination local addresses
    pDmaCh->m_pLocalAddress = pLocalAddress;
    pDmaCh->m_LocalAddressStart = LocalAddressBufStart;
    pDmaCh->m_LocalAddressEnd = LocalAddressBufEnd;

    // Initialise transfer data
    pDmaCh->m_FinalTransfer = FALSE;
    pDmaCh->m_TransferNumber = 0;
    pDmaCh->m_TransferSize = TransferSize;
    pDmaCh->m_TransferOffet = TransferOffset;
    pDmaCh->m_ReUseDataBuffer = ReUseDataBuffer;
    
    DtDbgOut(MAX, DMA, "DMA Start");

    // Flush cache
    if (!pDmaCh->m_UseDirectBufDma)
        DtDmaFlushCacheSglPreDma(&pDmaCh->m_pDvcData->m_Device, 
                                      &pDmaCh->m_Data.m_OsSgList, pDmaCh->m_DmaDirection);
    
    Status = DtaDmaProgramTransfer(pDmaCh);
    
    if ((pDmaCh->m_DmaFlags&DTA_DMA_FLAGS_BLOCKING) != 0)
    {
        // Wait for DMA to be completed
        if (DT_SUCCESS(Status))
        {
            Status = DtEventWait(&pDmaCh->m_DmaDoneEvent, -1);
            if (Status == DT_STATUS_CANCELLED)
            {
                DtDbgOut(ERR, DMA, "DtEventWait returned DT_STATUS_CANCELLED");
                Status = DtaDmaAbortDma(pDmaCh);
                if (DT_SUCCESS(Status))
                {
                    DtDbgOut(ERR, DMA, "DtaDmaAbortDma succeeded");
                    Status = DtEventWait(&pDmaCh->m_DmaDoneEvent, 10);
                    DtDbgOut(ERR, DMA, "DtEventWait Status after abort: %d", Status);
                }
            }
        }

        // Finalize dma transfer and clean databuffer if buffer not re-used
        if (!pDmaCh->m_ReUseDataBuffer)
        {
            if (!pDmaCh->m_UseDirectBufDma)
                DtDmaFinalSglTransfer(&pDmaCh->m_Data.m_OsSgList);
            DtaDmaCleanupDataBuffer(pDmaCh);
        }

        DtDbgOut(MAX, DMA, "Blocking DMA Done");
    }
                   
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaFinalTransfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaDmaFinalTransfer(
    DmaChannel*  pDmaCh)
{
    DtStatus  Status = DT_STATUS_OK;
    if (!pDmaCh->m_UseDirectBufDma)
    {
        if (pDmaCh->m_Data.m_OsSgList.m_Allocated)
            Status = DtDmaFinalSglTransfer(&pDmaCh->m_Data.m_OsSgList);
    }

    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaProgramTransfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function will copy the SGL list to common buffer, programs the PLX/FPGA's DMA 
// controller and starts the actual DMA transfer
//
static DtStatus  DtaDmaProgramTransfer(
    DmaChannel*  pDmaCh)
{
    DtStatus  Status;
    DtaDeviceData*  pDvcData = pDmaCh->m_pDvcData;
    UInt  TransferSize = pDmaCh->m_TransferSize;
    DmaOpt*  pDmaOpt = &pDvcData->m_DmaOptions;
    DtPhysicalAddress  SglPhysicalAddress;
    UInt  LocalAddress = 0;
    Int  OldState;
     
    // Calculate transfer length
    pDmaCh->m_CurrentTransferLength = TransferSize - (pDmaCh->m_TransferNumber * 
                                                                  pDmaCh->m_MaxDmaLength);

    if (pDmaCh->m_CurrentTransferLength > pDmaCh->m_MaxDmaLength)
        pDmaCh->m_CurrentTransferLength = pDmaCh->m_MaxDmaLength;

    LocalAddress = (UInt)(size_t)pDmaCh->m_pLocalAddress;

    Status = DtaDmaFillSgList(pDmaCh, pDmaCh->m_TransferNumber * pDmaCh->m_MaxDmaLength +
                                 pDmaCh->m_TransferOffet, pDmaCh->m_CurrentTransferLength,
                                 LocalAddress, pDmaCh->m_LocalAddressStart, 
                                 pDmaCh->m_LocalAddressEnd);
    if (!DT_SUCCESS(Status))
        return Status;

    TransferSize = TransferSize - (pDmaCh->m_CurrentTransferLength + 
                                         pDmaCh->m_TransferNumber*pDmaCh->m_MaxDmaLength);
    pDmaCh->m_FinalTransfer = (TransferSize == 0);
    SglPhysicalAddress = pDmaCh->m_SGListBuf.m_PhysicalAddress;

    if (pDmaCh->m_DmaDirection == DT_DMA_DIRECTION_TO_DEVICE)
    {
        // In some situations the common buffer is already filled
        // No need to copy data in this case
        if (pDmaCh->m_UseDirectBufDma && 
                             ((pDmaCh->m_DmaFlags & DTA_DMA_FLAGS_DATA_BUF_NO_COPY) == 0))
        {
            // Copy data to common buffer
            DtMemCopy(pDmaCh->m_Data.m_DmaData.m_pVirtualAddress, 
                               (UInt8*)pDmaCh->m_pBuffer +
                               (pDmaCh->m_TransferNumber * pDmaCh->m_MaxDmaLength +
                               pDmaCh->m_TransferOffet), pDmaCh->m_CurrentTransferLength);
        }
        pDmaCh->m_TransferNumber++;
    }

    // We set the started flag here, in case the DMA is finished immediately.
    OldState = DtAtomicCompareExchange((Int*)&pDmaCh->m_State, DTA_DMA_STATE_INIT, 
                                                                   DTA_DMA_STATE_STARTED);
    if (OldState == (DTA_DMA_STATE_INIT | DTA_DMA_STATE_ABORT))
    {
        DtDpcArgs  DpcArgs;
        // We have an abort
        DtDbgOut(MIN, DMA, "[%d] DMA aborted before started", pDmaCh->m_PortIndex);
        DpcArgs.m_pContext = pDmaCh;
        DtDpcSchedule(&pDmaCh->m_DmaCompletedDpc, &DpcArgs);
        return DT_STATUS_OK;
    }

    DT_ASSERT(OldState == DTA_DMA_STATE_INIT);
    
    // Program DMA controller
    if (pDmaOpt->m_UseDmaInFpga)
    {
        UInt32  CmdStat;
        UInt  DmaDesc;

        DmaDesc = SglPhysicalAddress.LowPart | PCI905X_DMADPR_DESCLOC_PCI;
        if (pDmaCh->m_DmaDirection == DT_DMA_DIRECTION_FROM_DEVICE)
            DmaDesc |= PCI905X_DMADPR_DIROFTFR;

        // Write descriptor to FPGA
        WRITE_UINT32(pDmaCh->m_pRegDmaDescLow, 0, DmaDesc);

        // We have to write the DAC address in case 64-bit DMA is enabled
        if (pDmaCh->m_Use64BitDma) 
        {
            WRITE_UINT32(pDmaCh->m_pRegDmaDescHigh, 0, SglPhysicalAddress.HighPart);
            DtDbgOut(MAX, DMA, "[%d] SGL descriptor addr. Low:0x%08x High:0x%08x"
                          " (Offset:%x)", pDmaCh->m_PortIndex, SglPhysicalAddress.LowPart,
                          SglPhysicalAddress.HighPart, pDmaCh->m_RegsOffset);
        } else if (pDmaOpt->m_Supports64Bit) 
        {
            WRITE_UINT32(pDmaCh->m_pRegDmaDescHigh, 0, 0);
            DtDbgOut(MAX, DMA, "[%d] SGL descriptor addr. Low:0x%08x High:0 (Offset:%x)",
                                          pDmaCh->m_PortIndex, SglPhysicalAddress.LowPart,
                                          pDmaCh->m_RegsOffset);
        } else {
            DtDbgOut(MAX, DMA, "[%d] SGL descriptor addr. Low:0x%08x (Offset:%x)",
                                          pDmaCh->m_PortIndex, SglPhysicalAddress.LowPart,
                                          pDmaCh->m_RegsOffset);
        }
        

        CmdStat = 0;
        // Check if we need to set the size enable and transfer length
        if ((pDmaCh->m_DmaMode&DTA_DMA_MODE_SIZE_ENABLE)!=0 ||
                                       (pDmaCh->m_DmaMode&DTA_DMA_MODE_TIMEOUT_ENABLE)!=0)
        {
            // Set the DMA size and DMA size enable bits in the command/status register
            // so if a timeout occurs, we can read back the number of bytes
            // not transfered and we can do partly SGL (ringbuffer) transfers.
            CmdStat = ((pDmaCh->m_CurrentTransferLength<<8)&DTA_DMA_CMDSTAT_SIZE_MASK) 
                                                                | DTA_DMA_CMDSTAT_SIZE_EN;
        }

        // Enable dual address cycles (64-bit DMA)
        if (pDmaCh->m_Use64BitDma) 
            CmdStat |= DTA_DMA_CMDSTAT_DAC_EN;

        CmdStat |= PCI905X_DMACSR_ENABLE | PCI905X_DMACSR_START;

        // Start transfer
        WRITE_UINT32(pDmaCh->m_pRegCmdStat, 0, CmdStat);
    } else {    // PLX
        Int  DmaChannelNum;
        DmaChannelNum = pDmaCh->m_RegsOffset;
        DT_ASSERT(DmaChannelNum < 2);
        DT_ASSERT(pDmaCh->m_DmaMode == DTA_DMA_MODE_DEFAULT);

        
        // Enable Scatter/Gather Mode in PCI 9054/9056
        WRITE_UINT32(pDmaCh->m_pRegDmaMode, 0, 
                                             pDmaOpt->m_PlxDmaChannelMode[DmaChannelNum]);

        DT_ASSERT(SglPhysicalAddress.HighPart == 0);
        
        // Write pointer to first SGL in DMA channel decriptor pointer register
        WRITE_UINT32(pDmaCh->m_pRegDmaDescLow, 0, SglPhysicalAddress.LowPart | 
                                                              PCI905X_DMADPR_DESCLOC_PCI);

        // Start transfer
        WRITE_UINT8(pDmaCh->m_pRegCmdStat, 0, 
                                            PCI905X_DMACSR_ENABLE | PCI905X_DMACSR_START);
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaFillSgList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function will fill the common buffer with descriptors from the 'OS'-generated
// SG list or the common data buffer. It will use 64-bit descriptors if available.
// With the DataOffset and DataLength part of the SG-list or common data buffer can be
// copied. The 'OS' generated SG-list must be build with the DtDmaCreateSgList function.
//
DtStatus  DtaDmaFillSgList(
    DmaChannel*  pDmaCh,
    UInt  DataOffset,
    UInt  DataLength,
    UInt  LocalAddressCurrent,      
    UInt  LocalAddressStart,        // Only used if m_FixedLocalAddress==FALSE
    UInt  LocalAddressEnd)          // Only used if m_FixedLocalAddress==FALSE
{
    DmaOpt*  pDmaOpt = &pDmaCh->m_pDvcData->m_DmaOptions;
    UInt  LocalAddress = LocalAddressCurrent;
    DtPhysicalAddress  PhysicalAddess;

    DT_ASSERT(!(pDmaCh->m_Use64BitDma && pDmaCh->m_UseDirectBufDma));
    
    // Check length
    if (pDmaCh->m_UseDirectBufDma)
        DT_ASSERT(DataLength <= pDmaCh->m_Data.m_DmaData.m_BufferLength);
    else
        DT_ASSERT(DataOffset+DataLength <= pDmaCh->m_Data.m_OsSgList.m_BufferLength);
    
    if (pDmaCh->m_UseDirectBufDma)
    {
        SglDesc*  pSglDesc;
        PhysicalAddess = pDmaCh->m_Data.m_DmaData.m_PhysicalAddress;
        DT_ASSERT(PhysicalAddess.HighPart == 0);

        // We only need to build 1 SG element. We always use 32-bit descriptors
        // Fill SG element
        pSglDesc = (SglDesc*)pDmaCh->m_SGListBuf.m_pVirtualAddress;
        pSglDesc->m_32LocalAddress = LocalAddress;
        // Check if the caller already copied the data to the correct location in the
        // common buffer. If not, the data will be copied to the start of the common buffer later
        // and we don't need to include the DataOffset
        if ((pDmaCh->m_DmaFlags & DTA_DMA_FLAGS_DATA_BUF_NO_COPY) == 0)
            pSglDesc->m_32PciAddress = PhysicalAddess.LowPart;
        else
            pSglDesc->m_32PciAddress = PhysicalAddess.LowPart + DataOffset;
        pSglDesc->m_32TransferSize = DataLength;

        if ((pDmaCh->m_DmaMode & DTA_DMA_MODE_RING_BUFFER) == 0)
        {
            // No next descriptor
            pSglDesc->m_32NextSglDesc = 0 | PCI905X_DMADPR_DESCLOC_PCI;

            // It's always the last element if not in ringbuffer mode
            pSglDesc->m_32NextSglDesc |= PCI905X_DMADPR_ENDOFCHAIN;
            
            // Use "Interrupt after Terminal Count" as DMA-done indicator for PCI cards
            // that use a DMA controller in FPGA
            if (pDmaOpt->m_UseDmaInFpga && 
                                 (pDmaCh->m_DmaFlags & DTA_DMA_FLAGS_NOINT_AFTER_DONE)==0)
                pSglDesc->m_32NextSglDesc |= PCI905X_DMADPR_INTAFTERTC;
        } else
            // We set the next descriptor to the first one
            pSglDesc->m_32NextSglDesc = pDmaCh->m_SGListBuf.m_PhysicalAddress.LowPart
                                                             | PCI905X_DMADPR_DESCLOC_PCI;
       
        if (pDmaCh->m_DmaDirection == DT_DMA_DIRECTION_FROM_DEVICE)
            pSglDesc->m_32NextSglDesc |= PCI905X_DMADPR_DIROFTFR;

        DtDbgOut(MAX, DMA, "[%d] SGL#%3d: Pci=0x%08x Local=0x%08x Size=%4d"
                                   " Nxt=0x%08x", pDmaCh->m_PortIndex, 0,
                                   pSglDesc->m_32PciAddress, pSglDesc->m_32LocalAddress, 
                                   pSglDesc->m_32TransferSize, pSglDesc->m_32NextSglDesc);
    } else {
        SglDesc64*  pSglDesc64;
        SglDesc*  pSglDesc;
        UInt  CurIndex = 0;
        Bool  FoundStart = (DataOffset == 0);
        UInt  Length;
        DtPhysicalAddress  PciAddress;
        UInt  SglAddress;

        pSglDesc = (SglDesc*)pDmaCh->m_SGListBuf.m_pVirtualAddress;
        pSglDesc64 = (SglDesc64*)pDmaCh->m_SGListBuf.m_pVirtualAddress;

        SglAddress = pDmaCh->m_SGListBuf.m_PhysicalAddress.LowPart; 
        
        if (pDmaCh->m_Use64BitDma)
            SglAddress += sizeof(SglDesc64);
        else
            SglAddress += sizeof(SglDesc);

        while (DataLength != 0)
        {
            DT_ASSERT(CurIndex < DtDmaOsSglGetNumElements(&pDmaCh->m_Data.m_OsSgList));

            PciAddress = DtDmaOsSglGetAddress(&pDmaCh->m_Data.m_OsSgList, CurIndex);
            Length = DtDmaOsSglGetLength(&pDmaCh->m_Data.m_OsSgList, CurIndex);

            if (!FoundStart) 
            {
                // Find start of data buffer
                if (Length <= DataOffset)
                {
                    DataOffset -= Length;
                    if (DataOffset == 0)
                        FoundStart = TRUE;
                    CurIndex++;
                    continue;
                } else {
                    // Start of data buffer found.
                    PciAddress.QuadPart += DataOffset;
                    Length -= DataOffset;
                    FoundStart = TRUE;
                }
            }

            // Copy elements to SG-list
            if (pDmaCh->m_Use64BitDma)
            {
                // The start of the 32-bit descriptors are the same as the 64-bit 
                // descriptors.
                pSglDesc = (SglDesc*)pSglDesc64;

                // 64-bit specific members
                pSglDesc64->m_DacPciAddress = PciAddress.HighPart;
                pSglDesc64->m_Dummy[0] = 0;
                pSglDesc64->m_Dummy[1] = 0;
                pSglDesc64->m_Dummy[2] = 0;
            } else
                DT_ASSERT(PciAddress.HighPart == 0);

            // Check if we only need a part of the last page
            if (Length > DataLength)
                Length = DataLength;
            pSglDesc->m_32PciAddress = PciAddress.LowPart;
            pSglDesc->m_32LocalAddress = LocalAddress;
            pSglDesc->m_32TransferSize = Length;

            DataLength -= Length;

            // If this is the last SGL element, and we are using a ringbuffer,
            // we set the next element to the first one.
            if (DataLength==0 && ((pDmaCh->m_DmaMode & DTA_DMA_MODE_RING_BUFFER) != 0))
                SglAddress = pDmaCh->m_SGListBuf.m_PhysicalAddress.LowPart; 
            pSglDesc->m_32NextSglDesc = SglAddress | PCI905X_DMADPR_DESCLOC_PCI;

            if (pDmaCh->m_DmaDirection == DT_DMA_DIRECTION_FROM_DEVICE)
                pSglDesc->m_32NextSglDesc |= PCI905X_DMADPR_DIROFTFR;

            // If this is the last SGL element.and we are not using a ringbuffer,
            // we set the end of chain and interrupt after terminal count flags.
            if (DataLength==0 &&((pDmaCh->m_DmaMode & DTA_DMA_MODE_RING_BUFFER) == 0))
            {
                pSglDesc->m_32NextSglDesc |= PCI905X_DMADPR_ENDOFCHAIN;

                // Use "Interrupt after Terminal Count" as DMA-done indicator for PCI 
                // cards that use a DMA controller in FPGA
                if (pDmaOpt->m_UseDmaInFpga && 
                                 (pDmaCh->m_DmaFlags & DTA_DMA_FLAGS_NOINT_AFTER_DONE)==0)
                    pSglDesc->m_32NextSglDesc |= PCI905X_DMADPR_INTAFTERTC;
            }
            if (CurIndex < 20)
            {
                if (pDmaCh->m_Use64BitDma)
                    DtDbgOut(MAX, DMA, "[%d] SGL#%3d: Pci=0x%08x Dac=0x%08x Local=0x%08x"
                                   " Size=%4d Nxt=0x%08x", pDmaCh->m_PortIndex, 
                                   CurIndex, pSglDesc->m_32PciAddress, 
                                   pSglDesc64->m_DacPciAddress, 
                                   pSglDesc->m_32LocalAddress, pSglDesc->m_32TransferSize,
                                   pSglDesc->m_32NextSglDesc);
                else
                    DtDbgOut(MAX, DMA, "[%d] SGL#%3d: Pci=0x%08x Local=0x%08x Size=%4d"
                                   " Nxt=0x%08x", pDmaCh->m_PortIndex, CurIndex,
                                   pSglDesc->m_32PciAddress, pSglDesc->m_32LocalAddress,
                                   pSglDesc->m_32TransferSize, pSglDesc->m_32NextSglDesc);
            } else if (CurIndex == 20) {
                DtDbgOut(MAX, DMA, "[%d] Skipping displaying >20 descriptors!",
                                                                     pDmaCh->m_PortIndex);
            }
            
            if (pDmaCh->m_Use64BitDma)
            {
                SglAddress += sizeof(SglDesc64);
                pSglDesc64++;
            } else {
                SglAddress += sizeof(SglDesc);
                pSglDesc++;
            }

            if (!pDmaCh->m_FixedLocalAddress)
            {
                LocalAddress += Length;
                if (LocalAddress >= LocalAddressEnd) // AddressEnd includes last item.
                    LocalAddress = LocalAddressStart + LocalAddressEnd - LocalAddress;
            }
            CurIndex++;
        }
    }
    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaStartTransfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This is the main entry point for starting a DMA transfer. If no 
// pPageList is provided, a pPageList is created from the pBuffer, Size and BufType.
//
DtStatus  DtaDmaStartTransfer(
    DmaChannel*  pDmaCh,
    DtPageList*  pPageList,
    Int  BufType,
    UInt  Direction,
    UInt8*  pBuffer,
    Int  TransferSize,
    Int  TransferOffset,
    UInt8*  pLocalAddress,      // FIFO address if pDmaCh->m_FixedLocalAddress==TRUE
                                // or memory (slice-)buffer pointer address otherwise
    UInt  LocalAddressBufStart, // Only used if pDmaCh->m_FixedLocalAddress==FALSE
    UInt  LocalAddressBufEnd,   // Only used if pDmaCh->m_FixedLocalAddress==FALSE
    Bool  ReuseDataBuffer,
    Int*  pNumBytesRead)
{    
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT((ReuseDataBuffer && pPageList==NULL) || !ReuseDataBuffer);
    
    DtDbgOut(MAX, DMA, "pVirtualAddress: %p TransferOffset: %d, TransferSize: %d", 
                                                   pBuffer, TransferOffset, TransferSize);

    
    Status = DtaDmaInitTransfer(pDmaCh, pPageList, BufType, Direction, pBuffer,
                                                     TransferSize, TransferOffset,
                                                     pLocalAddress, LocalAddressBufStart,
                                                     LocalAddressBufEnd, ReuseDataBuffer);
    if (pNumBytesRead != NULL)
    {
        if (DT_SUCCESS(Status))
            *pNumBytesRead = pDmaCh->m_NumBytesRead;
        else 
            *pNumBytesRead = 0;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaStartHpTransfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This is the main entry point for starting a HP DMA transfer. 
// Pre: The buffer is already prepared to be used for DMA. 
//
DtStatus  DtaDmaStartHpTransfer(
    DtaShBuffer*  pShBuffer,
    Int  TransferSize,
    Int  TransferOffset,
    UInt8*  pLocalAddress,      // FIFO address if pDmaCh->m_FixedLocalAddress==TRUE
                                // or memory (slice-)buffer pointer address otherwise
    UInt  LocalAddressBufStart, // Only used if pDmaCh->m_FixedLocalAddress==FALSE
    UInt  LocalAddressBufEnd)   // Only used if pDmaCh->m_FixedLocalAddress==FALSE
{
    return DtaDmaStartTransfer(pShBuffer->m_pDmaCh, NULL, 0, 0, pShBuffer->m_pBuffer, 
                        TransferSize, TransferOffset, pLocalAddress, LocalAddressBufStart,
                        LocalAddressBufEnd, TRUE, NULL);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaStartKernelBufTransfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This is the main entry point for starting a kernel buffer DMA transfer. 
// Pre: The buffer is already prepared to be used for DMA. 
//
DtStatus  DtaDmaStartKernelBufTransfer(
    DmaChannel*  pDmaCh,
    UInt8*  pBuffer,
    Int  TransferSize,
    Int  TransferOffset,
    UInt8*  pLocalAddress,      // FIFO address if pDmaCh->m_FixedLocalAddress==TRUE
                                // or memory (slice-)buffer pointer address otherwise
    UInt  LocalAddressBufStart, // Only used if pDmaCh->m_FixedLocalAddress==FALSE
    UInt  LocalAddressBufEnd)   // Only used if pDmaCh->m_FixedLocalAddress==FALSE
{
    return DtaDmaStartTransfer(pDmaCh, NULL, 0, 0, pBuffer, 
                        TransferSize, TransferOffset, pLocalAddress, LocalAddressBufStart,
                        LocalAddressBufEnd, TRUE, NULL);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaCompletedDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtaDmaCompletedDpc(DtDpcArgs* pArgs)
{
    DmaChannel*  pDmaCh = (DmaChannel*)pArgs->m_pContext;

    // Check for DMA abort before start
    if (pDmaCh->m_State == (DTA_DMA_STATE_INIT | DTA_DMA_STATE_ABORT))
        DtDbgOut(ERR, DMA, "[%d] DMA ABORTED BEFORE START (Offset:%x)", 
                                               pDmaCh->m_PortIndex, pDmaCh->m_RegsOffset);
    else if ((pDmaCh->m_State & DTA_DMA_STATE_STARTED) == 0)
    {
        // DMA was not started so the interrupt was invalid.
        // Report error in event viewer and stop. We don't want to see the BSOD.
        DtEvtLogReport(&pDmaCh->m_pDvcData->m_Device.m_EvtObject, DTA_LOG_FAKE_DMA_INT,
                                                                        NULL, NULL, NULL);
        DtDbgOut(ERR, DMA, "[%d] DMA interrupt occured, but DMA not started"
                                        " (State=0x%04X, Offset:%x)", pDmaCh->m_PortIndex,
                                        pDmaCh->m_State, pDmaCh->m_RegsOffset);
        DT_ASSERT(FALSE);
        return;
    }

    // Check if the DMA is aborted
    if ((pDmaCh->m_State & DTA_DMA_STATE_ABORT) != 0)
        pDmaCh->m_NumBytesRead = 0;
    else {
        if (pDmaCh->m_DmaDirection == DT_DMA_DIRECTION_FROM_DEVICE)
        {
            if ((pDmaCh->m_DmaMode & DTA_DMA_MODE_TIMEOUT_ENABLE) == 0)
                pDmaCh->m_NumBytesRead = pDmaCh->m_TransferSize;
            /* The following is updated in the ISR.
            else {
                UInt32  CmdStat;
                DT_ASSERT(pDmaCh->m_FinalTransfer);
            
                // Readback actual size transferred.
                CmdStat = READ_UINT32(pDmaCh->m_pRegCmdStat, 0);
                
                // We have to read back to actual size
                pDmaCh->m_NumBytesRead = pDmaCh->m_TransferSize - 
                                             ((CmdStat & DTA_DMA_CMDSTAT_SIZE_MASK) >> 8);
            } */
            DtDbgOut(MAX, DMA, "DMA TransferSize: %u, DMA ReadSize: %u", 
                                          pDmaCh->m_TransferSize, pDmaCh->m_NumBytesRead);
            if (pDmaCh->m_UseDirectBufDma &&
                             ((pDmaCh->m_DmaFlags & DTA_DMA_FLAGS_DATA_BUF_NO_COPY) == 0))
            {
                // Copy data from common buffer to user buffer
                DtMemCopy((UInt8*)pDmaCh->m_pBuffer + 
                                      (pDmaCh->m_TransferNumber * pDmaCh->m_MaxDmaLength +
                                      pDmaCh->m_TransferOffet), 
                                      pDmaCh->m_Data.m_DmaData.m_pVirtualAddress,
                                      pDmaCh->m_CurrentTransferLength);
            }
            pDmaCh->m_TransferNumber++;
        }
        
        if (!pDmaCh->m_FinalTransfer)
        {
            Int  OldState;
            DT_ASSERT(pDmaCh->m_DmaMode == DTA_DMA_MODE_DEFAULT);
            
            // Initialise the state for DtaDmaProgramTransfer
            OldState = DtAtomicCompareExchange((Int*)&pDmaCh->m_State, 
                                               DTA_DMA_STATE_STARTED, DTA_DMA_STATE_INIT);
            if ((OldState & DTA_DMA_STATE_ABORT) != 0)
                DtDbgOut(MAX, DMA, "[%d] DMA restart next SKIPPED...aborting (Offset:%x)",
                                               pDmaCh->m_PortIndex, pDmaCh->m_RegsOffset);
            else {
                DtDbgOut(MAX, DMA, "[%d] DMA restart next (Offset:%x)", 
                                               pDmaCh->m_PortIndex, pDmaCh->m_RegsOffset);

                DT_ASSERT(OldState == DTA_DMA_STATE_STARTED);
                // We start the next DMA transfer
                DtaDmaProgramTransfer(pDmaCh);
                return;
            }
        }
    }
    
    // Reset the flags but leave the abort flag
    DtAtomicSet((Int*)&pDmaCh->m_State, pDmaCh->m_State & DTA_DMA_STATE_ABORT);
    
    if ((pDmaCh->m_DmaFlags&DTA_DMA_FLAGS_BLOCKING) == 0)
    {
        // Finalize DMA transfer and clean databuffer if buffer not re-used and blocking
        if (!pDmaCh->m_ReUseDataBuffer)
        {
            if (!pDmaCh->m_UseDirectBufDma)
                DtDmaFinalSglTransfer(&pDmaCh->m_Data.m_OsSgList);
            DtaDmaCleanupDataBuffer(pDmaCh);
        }
    }

    if ((pDmaCh->m_State & DTA_DMA_STATE_ABORT) != 0)
        DtDbgOut(MAX, DMA, "[%d] DMA abort event (Offset:%x)", pDmaCh->m_PortIndex, 
                                                                    pDmaCh->m_RegsOffset);
    else
        DtDbgOut(MAX, DMA, "[%d] DMA completed event (Offset:%x)", pDmaCh->m_PortIndex, 
                                                                    pDmaCh->m_RegsOffset);

    if (pDmaCh->m_pDmaFinishFunc != NULL)
        pDmaCh->m_pDmaFinishFunc(pDmaCh, pDmaCh->m_pDmaFinishContext);
    
    // Trigger done event
    DtEventSet(&pDmaCh->m_DmaDoneEvent);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaGetDmaChannelPlx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This helper function will return the DmaChannel structure used for the given PLX DMA 
// controller.
//
DtStatus  DtaDmaGetDmaChannelPlx(
    DtaDeviceData*  pDvcData,
    Int  PlxDmaChannel,
    DmaChannel** ppDmaCh)
{
    DmaOpt*  pDmaOptions = &pDvcData->m_DmaOptions;
    Int  PortIndex;
    Int  ChannelIndex;
    
    *ppDmaCh = NULL;
    
    // Lookup channel index
    PortIndex = pDmaOptions->m_PlxDmaChannelPort[PlxDmaChannel];

    if (PortIndex==-1 || PortIndex>=pDvcData->m_NumPorts)
    {
        // This channel is not used but interrupt was active
        DT_ASSERT(FALSE);
        return DT_STATUS_FAIL;
    }

    ChannelIndex = pDvcData->m_pPortLookup[PortIndex].m_Index;
    if (pDvcData->m_pPortLookup[PortIndex].m_PortType == DTA_PORT_TYPE_NONIP)
    {
        *ppDmaCh = &pDvcData->m_pNonIpPorts[ChannelIndex].m_DmaChannel;
    } else {
        // Unknown channel type
        DT_ASSERT(FALSE);
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaCreateKernelBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function creates a kernel buffer that can be used directly for DMA transfer
//
DtStatus  DtaDmaCreateKernelBuffer(
    DmaChannel*  pDmaCh,
    UInt  DmaDirection,
    void**  ppBuffer,
    DtPageList**  ppPageList,
    UInt  BufSize,
    UInt  Tag)
{
    DtStatus  Status = DT_STATUS_OK;
    DtPageList*  pPageList = NULL;

    // Use the created common buffer as our buffer
    if (pDmaCh->m_UseDirectBufDma)
    {   
        // Use the created common buffer (created in DtaDmaInit) as our buffer
        *ppBuffer = pDmaCh->m_Data.m_DmaData.m_pVirtualAddress;
        DT_ASSERT(BufSize <= pDmaCh->m_Data.m_DmaData.m_BufferLength);
    } else {
        // Otherwise create a buffer, optionally create a pagelist for
        // non-contiguous memory if there is a shortage of contiguous memory.
        // note: *ppPageList will be set to NULL if we do not need large memory
        *ppBuffer = DtMemAllocPoolLarge(DtPoolNonPaged, BufSize, Tag, ppPageList);

        if (*ppBuffer == NULL)
            return DT_STATUS_OUT_OF_MEMORY;

        // use optional pagelist
        if (ppPageList != NULL)
            pPageList = *ppPageList;
    }

    // Create pagelist and create SglList
    Status = DtaDmaPrepareDataBuffer(pPageList, *ppBuffer, BufSize, DT_BUFTYPE_KERNEL,
                                                                    pDmaCh, DmaDirection);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaCleanupKernelBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaDmaCleanupKernelBuffer(
    DmaChannel*  pDmaCh,
    void*  pBuffer,
    DtPageList*  pPageList,
    UInt  Tag)
{
    DtaDmaCleanupDataBuffer(pDmaCh);
    if (!pDmaCh->m_UseDirectBufDma)
    {
        if (pBuffer != NULL)
            DtMemFreePoolLarge(pBuffer, Tag, pPageList);
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaGetDoneEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtEvent*  DtaDmaGetDoneEvent(
    DmaChannel*  pDmaCh)
{
    return &pDmaCh->m_DmaDoneEvent;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaIsReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaDmaIsReady(
    DmaChannel*  pDmaCh)
{
    return ((pDmaCh->m_State & (DTA_DMA_STATE_INIT | DTA_DMA_STATE_STARTED)) == 0);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaGetBytesReceived -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtaDmaGetBytesReceived(
    DmaChannel*  pDmaCh)
{
    return pDmaCh->m_NumBytesRead;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaAbortDma -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// if the return value == DT_STATUS_NOT_STARTED, we do not have to wait for the done event.
// The DMA is not started and will not be started
//
DtStatus  DtaDmaAbortDma(
    DmaChannel*  pDmaCh)
{
    DtStatus  Status = DT_STATUS_OK;
    volatile Int  OldState = pDmaCh->m_State;
    Bool  UsesDmaInFpga = pDmaCh->m_pDvcData->m_DmaOptions.m_UseDmaInFpga;
        

    DtDbgOut(MAX, DMA, "Init");

    while ((pDmaCh->m_State & DTA_DMA_STATE_ABORT) == 0)
    {
        OldState = DtAtomicCompareExchange((Int*)&pDmaCh->m_State, OldState, 
                                                    OldState | DTA_DMA_STATE_ABORT);
    }
    if (pDmaCh->m_State == DTA_DMA_STATE_ABORT)
        return DT_STATUS_NOT_STARTED;

    // Prevent race condition writing to the CMD/STAT registers during abort
    if (OldState == DTA_DMA_STATE_STARTED) 
    {
        UInt32  CmdStat = 0;
        UInt  TimeoutCount = 0;
            
        while (TimeoutCount<10 && (CmdStat&PCI905X_DMACSR_ENABLE)==0)
        {
            if (UsesDmaInFpga)
                CmdStat = READ_UINT32(pDmaCh->m_pRegCmdStat, 0);
            else
                CmdStat = READ_UINT8(pDmaCh->m_pRegCmdStat, 0);
        
            DtSleep(1);
            TimeoutCount++;
        }
        DT_ASSERT(TimeoutCount < 10);
    }
    
    if ((pDmaCh->m_State & DTA_DMA_STATE_STARTED) != 0)
    {
        Int  NonIpPortIndex = -1;
        DtaNonIpPort*  pNonIpPort = NULL;
        UInt32  CmdStat;
        
        DtDbgOut(AVG, DMA, "[%d] Stop DMA channel (Offset:%x)", pDmaCh->m_PortIndex, 
                                                                    pDmaCh->m_RegsOffset);
        // Stop DMA channel
        if (UsesDmaInFpga)
            CmdStat = READ_UINT32(pDmaCh->m_pRegCmdStat, 0);
        else
            CmdStat = READ_UINT8(pDmaCh->m_pRegCmdStat, 0);

        if ((CmdStat & DTA_DMA_CMDSTAT_DONE) == 0)
        {
            // Abort
            CmdStat |= DTA_DMA_CMDSTAT_ABORT;
            
            if (UsesDmaInFpga)
                WRITE_UINT32(pDmaCh->m_pRegCmdStat, 0, CmdStat);
            else
                WRITE_UINT8(pDmaCh->m_pRegCmdStat, 0, CmdStat);

            // For Matrix capable ports, also need to set the mem-transfer abort bit
            pNonIpPort = NULL; NonIpPortIndex=-1;
            Status = DtaGetNonIpPortIndex(pDmaCh->m_pDvcData, pDmaCh->m_PortIndex, 
                                                                         &NonIpPortIndex);
            if (DT_SUCCESS(Status))
            {
                DT_ASSERT(NonIpPortIndex>=0 && 
                                      NonIpPortIndex<pDmaCh->m_pDvcData->m_NumNonIpPorts);
                pNonIpPort = &pDmaCh->m_pDvcData->m_pNonIpPorts[NonIpPortIndex];
                DT_ASSERT(pNonIpPort != NULL);
                if (pNonIpPort->m_CapMatrix)
                    DtaRegHdMemTrControlSetAbort(pNonIpPort->m_pRxRegs, 1);
            }
        }
    } else 
    {
        DtDbgOut(MAX, DMA, "[%d] DMA channel not started (Offset:%x)", 
                                               pDmaCh->m_PortIndex, pDmaCh->m_RegsOffset);
    }

    DtDbgOut(MAX, DMA, "Exit");

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaIsAbortActive -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaDmaIsAbortActive(
    DmaChannel*  pDmaCh)
{
    return ((pDmaCh->m_State & DTA_DMA_STATE_ABORT) != 0);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaClearAbortFlag -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaDmaClearAbortFlag(DmaChannel* pDmaCh)
{
    Int  LoopCnt = 10; // Max 10 loops
    volatile Int OldState = pDmaCh->m_State;

    // Use loop to make sure only the abort flag is cleared (other threads could 
    // change one of the other flags which we want to leave untouched)
    while (LoopCnt>0 && (pDmaCh->m_State & DTA_DMA_STATE_ABORT)!=0)
    {
        OldState = DtAtomicCompareExchange((Int*)&pDmaCh->m_State, OldState, 
                                                         OldState & ~DTA_DMA_STATE_ABORT);
        LoopCnt--;
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDmaReInitCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaDmaReInitCallback(
    DmaChannel*  pDmaCh,
    DmaCallbackFunc  pDmaFinishFunc,
    void*  pDmaFinishContext)
{
    DT_ASSERT((pDmaFinishFunc!=NULL && ((pDmaCh->m_DmaFlags&DTA_DMA_FLAGS_BLOCKING) == 0))
                                                                 || pDmaFinishFunc==NULL);
    DT_ASSERT(pDmaCh->m_State == DTA_DMA_STATE_IDLE);
    pDmaCh->m_pDmaFinishFunc = pDmaFinishFunc;
    pDmaCh->m_pDmaFinishContext = pDmaFinishContext;
}
