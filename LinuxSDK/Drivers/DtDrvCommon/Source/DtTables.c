//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtTables.c *#*#*#*#*#*#*#*#* (C) 2011-2016 DekTec
//
// Driver common - Table get funcions.
//

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


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtDrvCommon.h>

// Generated table store
extern const Int  DtTableStoreCount;
extern const DtTableStore  DtTableStores[];

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtTablesInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtTablesInit(DtPropertyData* pPropData)
{
    Int  Index;

    // Default no table store found for our device
    pPropData->m_pTableStore = NULL;

    // Find the table store for our device
    for (Index=0; Index<DtTableStoreCount; Index++)
    {
        if (DtTableStores[Index].m_TypeNumber==pPropData->m_TypeNumber && 
                                       DtTableStores[Index].m_SubDvc==pPropData->m_SubDvc)
        {
            pPropData->m_pTableStore = (void*)&DtTableStores[Index];
            break;
        }
    }

    // It's not a fault if no table store is found. Not all cards have tables
    if (pPropData->m_pTableStore == NULL)
        DtDbgOut(MIN, TABLE, "Tablestore not found for %s-%d:%d", pPropData->m_TypeName,
                                                                  pPropData->m_TypeNumber,
                                                                     pPropData->m_SubDvc);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtTableGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtTableGet(
    DtPropertyData*  pPropData,
    const char*  pTableName,
    Int  PortIndex,
    UInt  MaxNumEntries,
    UInt*  pNumEntries,
    DtTableEntry*  pTableEntry2,
    UInt OutBufSize)
{
    DtStatus  Status = DT_STATUS_OK;
    const DtTableStore*  pStore = (DtTableStore*)pPropData->m_pTableStore;
    Int  HwRevision = pPropData->m_HardwareRevision;
    Int  FwVariant = pPropData->m_FirmwareVariant;

    Bool  TableNameFound = FALSE;
    Int  FindCount = 0;
    UInt  Index;
    DtTableLink*  pTableLinkFound = NULL;

    // For devices with no 'EC' VPD resource and no registry key which forces the
    // hardware revision, treat the hardware revision as 0
    if (HwRevision < 0)
        HwRevision = 0;
    
    *pNumEntries = 0;
    if (pPropData->m_pTableStore == NULL)
    {
        DtDbgOut(ERR, TABLE, "There are no tables defined at all for DTX-%d", 
                                                                 pPropData->m_TypeNumber);
        return DT_STATUS_NOT_FOUND;
    }

    // If the property is not found for a specific fw-variant try a second time
    // without specifying a specific fw-variant
    for (FindCount=0; FindCount<2 && pTableLinkFound==NULL; FindCount++)
    {
        if (FindCount == 1)
            FwVariant = -1;

        TableNameFound = FALSE;

        // Search all tables
        for (Index=0; Index<pStore->m_TableLinkCount; Index++)
        {
            const DtTableLink*  pTableLink = &pStore->m_pTableLink[Index];

            // Check if the table name was already found. If so, we stop if
            // name <> NULL.
            if (TableNameFound)
            {
                // When the table name was found earlier, only accept entries without
                // a name. We just stop when (another) named entry is found.
                if (pTableLink->m_pName != NULL)
                    break;
            } else {
                // Compare name to check if we found the first occurrence
                if (DtAnsiCharArrayIsEqual(pTableName, pTableLink->m_pName))
                    TableNameFound = TRUE;
            }

            if (TableNameFound)
            {
                // Check port number and firmware variant
                if (PortIndex==pTableLink->m_PortIndex
                                                    && FwVariant==pTableLink->m_FwVariant)
                {
                    // Check minimal firmware version
                    if (pPropData->m_FirmwareVersion >= pTableLink->m_MinFw)
                    {
                        // Check minimal hardware version
                        if (HwRevision >= pTableLink->m_MinHw)
                        {
                            pTableLinkFound = (DtTableLink*)pTableLink;

                            // We can stop here since the parser has ordened each
                            // property by minimal firmware version/hardware version.
                            // This means the first hit is the best one
                            break;
                        }
                    }
                }
            }
        }
    }

    if (!TableNameFound)
    {   
        DtDbgOut(ERR, TABLE, "Table %s is not found at all for DTX-%d",
                                                        pTableName, pStore->m_TypeNumber);
        Status = DT_STATUS_NOT_FOUND;
    }
        
    // Check if the table was found
    if (pTableLinkFound == NULL)
    {
        Status = DT_STATUS_NOT_FOUND;
        DtDbgOut(ERR, TABLE, "Failed to get table %s for DTX-%d:%d, FW %d, HW %d port %i", 
                                                  pTableName,
                                                  pPropData->m_TypeNumber,
                                                  pPropData->m_SubDvc,
                                                  pPropData->m_FirmwareVersion,
                                                  pPropData->m_HardwareRevision,
                                                  PortIndex);
    }
    
    if (DT_SUCCESS(Status) && pTableLinkFound!=NULL)
    {
        DtDbgOut(MAX, TABLE, "Found table %s for DTX-%d:%d, FW %d, HW %d. #EL:%i #MAX:%i" ,
                                                  pTableName,
                                                  pPropData->m_TypeNumber,
                                                  pPropData->m_SubDvc,
                                                  pPropData->m_FirmwareVersion,
                                                  pPropData->m_HardwareRevision,
                                                  pTableLinkFound->m_TableEntryCount,
                                                  MaxNumEntries);
        *pNumEntries =  pTableLinkFound->m_TableEntryCount;
        
        // Check if enough space for the table
        if (MaxNumEntries < pTableLinkFound->m_TableEntryCount)
        {
            if (MaxNumEntries != 0)
                DtDbgOut(ERR, TABLE, "Max. number of entries to small. Needed:%i, "
                       "Space for:%i", pTableLinkFound->m_TableEntryCount, MaxNumEntries);
        }
        else if (OutBufSize < pTableLinkFound->m_TableEntryCount * sizeof(DtTableEntry)) {
            DtDbgOut(ERR, TABLE, "Buffer smaller than indicated by MaxNumEntries");
            Status = DT_STATUS_INVALID_PARAMETER;
        } else {
            // Copy table
            DtMemCopy(pTableEntry2, (void*)pTableLinkFound->m_pTableEntries, 
                             pTableLinkFound->m_TableEntryCount * sizeof(DtTableEntry));
        }
    } 
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtTableGetForType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtTableGetForType(
    const char*  pTypeName,
    Int  TypeNumber, 
    Int  SubDvc,
    Int  HwRev,
    Int  FwVer,
    Int  FwVariant,
    const char*  pTableName, 
    Int  PortIndex, 
    UInt  MaxNumEntries,
    UInt* pNumEntries,
    DtTableEntry* pTableEntry2,
    UInt OutBufSize)
{
    DtPropertyData  PropData;
    DtStatus  Status = DT_STATUS_OK;
        
    // Init property data structure
    PropData.m_TypeNumber = TypeNumber;
    PropData.m_SubDvc = SubDvc;
    PropData.m_FirmwareVariant = FwVariant;
    PropData.m_FirmwareVersion = FwVer;
    PropData.m_HardwareRevision = HwRev;
    PropData.m_TypeName = (char*)pTypeName;

    Status = DtTablesInit(&PropData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Now find the table
    return DtTableGet(&PropData, pTableName, PortIndex, MaxNumEntries, pNumEntries,
                                                                pTableEntry2, OutBufSize);
}

