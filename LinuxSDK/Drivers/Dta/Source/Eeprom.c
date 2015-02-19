//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Eeprom.c *#*#*#*#*#*#*#*#*# (C) 2010-2015 DekTec
//
// Dta driver - Low level EEPROM write/read functions.
//
// Low-level serial EEPROM routines:
//  - For reading and writing to 93C66 (PCI cards with PLX chip)
//  - For reading and writing EPCS1, EPCS4 (FPGA)
//  - For reading and writing SPI (separate EEPROM)
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2015 DekTec Digital Video B.V.
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
#include <DtaIncludes.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaEepromInit(DtaDeviceData* pDvcData)
{
    // Initialisation PLX IO mutex
    DtFastMutexInit(&pDvcData->m_Vpd.m_EepromMutex);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaEepromCleanup(DtaDeviceData* pDvcData)
{
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ 93C66 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Read and write serial EEPROM by directly using SK (Clock), CS (Chip Select), DI (Data
// Input) and DO (Data Output) pins of the serial EEPROM, instead of VPD routines in
// PCI9054/9056 (which are somewhat unreliable).
//
// WARNING
// =======
// Code to protect/unprotect writing to serial EEPROM CANNOT WORK:
//
//  1. The EEPROM in use on the DTA-100/102/120/122 is the 93C66L. This type does not
//     support the Protect Register at all! The 93CS66L is required.
//
//  2. Even if the correct type of EEPROM is used, addresses ABOVE a threshold are
//     protected. Unfortunately, the lay-out of the EEPROM (VPD read/write section at
//     the end) was based on the assumption that address BELOW a certain value would be
//     protected.
//     So, if we want to protect the VPD read-only section, the VPD read/write section
//     becomes read-only too.

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Tunable Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#define  EEPROM_PLX_MAX_SIZE             512
#define  EEPROM_PLX_WRARR_MAX_RETRIES    5     // Max. #times an array write is retried
#define  EEPROM_PLX_WRITE_MAX_RETRIES    5     // Max. #times a VPD write is retried

// Serial EEPROM (minimum) data set-up time in microseconds
#define  EEPROM_PLX_DELAY_DATA_SETUP_US  2

// Serial EEPROM (minimum) data hold time in microseconds
#define  EEPROM_PLX_DELAY_DATA_HOLD_US   2

// Minimum CS low time in microseconds
#define  EEPROM_PLX_MINIMUM_CS_LOW       4

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local Forwards -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

static DtStatus  DtaEepromPlxWriteArray(DtaDeviceData* pDvcData, UInt16* pWordBuf,
                                                   UInt WordAddrStart, UInt WordAddrLast);
static DtStatus  DtaEepromPlxWriteWordWithCheck(DtaDeviceData* pDvcData, UInt WordAddr,
                                                        UInt16 DataWord, UInt16* VpdWord);
static DtStatus  DtaEepromPlxReadWord16(DtaDeviceData* pDvcData, UInt WordAddr,
                                                                           UInt16* pData);
static DtStatus  DtaEepromPlxWriteWord16(DtaDeviceData* pDvcData, UInt WordAddr,
                                                                               UInt Data);
static void  DtaEepromPlxClearSkDoCs(DtaDeviceData* pDvcData);
static void  DtaEepromPlxClockPulse(DtaDeviceData* pDvcData);
static void  DtaEepromPlxCsFallingEdge(DtaDeviceData* pDvcData);
static void  DtaEepromPlxEnableEedoPin(DtaDeviceData* pDvcData);
static void  DtaEepromPlxFloatEedoPin(DtaDeviceData* pDvcData);
static void  DtaEepromPlxInstruction(DtaDeviceData* pDvcData, Int Command, Int Address,
                                                                               Int Close);
static UInt  DtaEepromPlxReadDi(DtaDeviceData* pDvcData);
static void  DtaEepromPlxSetCs(DtaDeviceData* pDvcData, Int CsValue);
static void  DtaEepromPlxSetCsDo(DtaDeviceData* pDvcData, Int CsValue, Int DoValue);
static void  DtaEepromPlxSetDoAndClk(DtaDeviceData* pDvcData, Int DoValue);
static void  DtaEepromPlxSetPeAndPre(DtaDeviceData* pDvcData, Int Pe, Int Pre);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read an array of bytes from the serial EEPROM.
//
DtStatus  DtaEepromPlxRead(
    DtaDeviceData*  pDvcData,   // Device extension
    UInt  StartOffset,          // Start address (BYTE address)
    UInt8*  pBuf,               // Buffer for data read from serial EEPROM
    UInt  Length)               // Number of BYTES to read
{
    UInt  RelAddr, WordAddr, WordAddrLast;
    UInt16  Word16; 
    UInt32  VpdDword;
    DtStatus  Status;

    DtDbgOut(MAX, VPD, "StartOffset=0x%03x Length=%d", StartOffset, Length);

    // Let's be robust
    if (Length == 0)
        return DT_STATUS_OK;

    // Compute start- and last 32-bit word addresses
    WordAddr = StartOffset / 4;
    RelAddr = StartOffset % 4;
    WordAddrLast = (StartOffset + Length - 1) / 4;

    // Read 32-bit word by word and extract bytes
    for (; WordAddr<=WordAddrLast; WordAddr++)
    {
        // Read two 16-bit words, into 32-bit VpdWord
        Status = DtaEepromPlxReadWord16(pDvcData, WordAddr*2, &Word16);
        if (!DT_SUCCESS(Status))
            return (Status);
        VpdDword = Word16<<16;
        Status = DtaEepromPlxReadWord16(pDvcData, WordAddr*2+1, &Word16);
        if (!DT_SUCCESS(Status))
            return (Status);
        VpdDword |= Word16;

        // A little bit of magic to convert 32-bit word to bytes
        while (RelAddr<4 && Length!=0)
        {
            *pBuf++ = (UInt8)((VpdDword >> (8 * (3-RelAddr))) & 0xFF);
            RelAddr++;
            Length--;
        }
        RelAddr = 0;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write an array of bytes to the serial PLX EEPROM. The StartAddr and Length parameters
// must be even, because writes to the PLX eeprom are done with 16-bit words. When the
// StartAddr and Length are both even there is no need to merge the first or last byte
// with the current data in the eeprom. This will eventually speed up the writing a bit
// and makes this function easier. Since this function is only used by the VPD
// implementation their is no real need to write odd length buffers to the PLX eeprom.
//
DtStatus  DtaEepromPlxWrite(
    DtaDeviceData*  pDvcData,   // Device extension
    UInt  StartOffset,          // Start address of destination(even BYTE address)
    UInt8*  pBuf,               // Buffer for data read from serial EEPROM
    UInt  Length)               // Number of BYTES to read (even)
{
    DtStatus  Status;
    Int  Try;
    UInt16*  pWordBuf = (UInt16*)pBuf;
    UInt  WordIndex = 0;
    UInt16  WordBuf[EEPROM_PLX_MAX_SIZE / 2];

    // We do not support odd byte writing
    if (StartOffset%2 != 0 || Length%2 != 0)
        return DT_STATUS_INVALID_PARAMETER;

    // Check max length
    if (StartOffset+Length > EEPROM_PLX_MAX_SIZE)
        return DT_STATUS_INVALID_PARAMETER;

    // Construct array of 16-bit words to be written to the serial EEPROM
    // Array is Little Endian
    for (WordIndex=0; WordIndex<(Length/2); WordIndex++)
    {
        WordBuf[WordIndex] = DtUInt16ByteSwap(pWordBuf[WordIndex]);
    }

    // Write until success, with a maximum number of retries
    for (Try=0; Try<EEPROM_PLX_WRARR_MAX_RETRIES; Try++)
    {
        // Write and readback into cache
        Status = DtaEepromPlxWriteArray(pDvcData, WordBuf, StartOffset/2,
                                                                (StartOffset+Length)/2-1);
        if (DT_SUCCESS(Status))
            break;
    }

    return Status;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ 93C66 - static functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxWriteArray -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write an array of 16-bit words to the serial EEPROM.
// Assumes PROT_AREA in PCI 9054 and PE in Altera have been enabled.
//
static DtStatus  DtaEepromPlxWriteArray(
    DtaDeviceData*  pDvcData,   // Device extension
    UInt16*  pWordBuf,          // 16-bit data to be written (Little Endian)
    UInt  WordAddrStart,        // First 16-bit word in pWordBuf to be written
    UInt  WordAddrLast)         // Last 16-bit word in pWordBuf to be written
{
    UInt  WordAddr;
    UInt16  VpdWord;
    Bool  OkFlag = TRUE;
    DtStatus  Status;

    // Loop through 16-bit words
    for (WordAddr=WordAddrStart; WordAddr<=WordAddrLast; WordAddr++)
    {
        // Read word first. Program only if word is required to change
        Status = DtaEepromPlxReadWord16(pDvcData, WordAddr, &VpdWord);
        if (!DT_SUCCESS(Status))
            return Status;      // Reads must succeed
        if (VpdWord == pWordBuf[WordAddr - WordAddrStart])
        {
            DtDbgOut(MAX, VPD, "WordAddr=0x%03x already contains 0x%08x", WordAddr, 
                                                                                 VpdWord);
            continue;
        }

        DtDbgOut(MAX, VPD, "Program WordAddr=0x%03x with 0x%08x", WordAddr, 
                                                                      pWordBuf[WordAddr]);
        // Program 16-bit word to the serial EEPROM
        Status = DtaEepromPlxWriteWordWithCheck(pDvcData, WordAddr,
                                            pWordBuf[WordAddr - WordAddrStart], &VpdWord);
        if (!DT_SUCCESS(Status))
            OkFlag = FALSE;
    }
    return (OkFlag ? DT_STATUS_OK : DT_STATUS_FAIL);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxWriteWordWithCheck -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write one 16-bit word to the serial EEPROM and check whether the word has been written
// successfully. Returns the word read.
// Retry max. VPD_WRITE_MAX_RETRIES times.
//
static DtStatus  DtaEepromPlxWriteWordWithCheck(
    DtaDeviceData*  pDvcData,   // Device extension
    UInt  WordAddr,             // 16-bit word address
    UInt16  DataWord,           // 16-bit data to be written
    UInt16*  VpdWord)           // 16-bit data read back
{
    Int  Try;
    Bool  OkFlag = FALSE;
    DtStatus  Status;

    for (Try=0; Try<EEPROM_PLX_WRITE_MAX_RETRIES; Try++)
    {
        // Write word
        Status = DtaEepromPlxWriteWord16(pDvcData, WordAddr, DataWord);
        if (!DT_SUCCESS(Status))
            continue;                   // Try again if write fails

        // Read and check word
        Status = DtaEepromPlxReadWord16(pDvcData, WordAddr, VpdWord);
        if (!DT_SUCCESS(Status))
            return Status;            // Reads must succeed
        if (*VpdWord == DataWord) {
            OkFlag = TRUE;
            break;
        }
        DtDbgOut(MAX, VPD, "Write FAILS: VpdWord=0x%03x", *VpdWord);
    }

    return (OkFlag ? DT_STATUS_OK : DT_STATUS_FAIL);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxReadWord16 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read one 16-bit word from the serial EEPROM.
//
static DtStatus  DtaEepromPlxReadWord16(
    DtaDeviceData*  pDvcData,   // Device extension
    UInt  WordAddr,             // 16-bit word address
    UInt16*  pData)             // Data read from serial EEPROM
{
    DtStatus  Status;
    Int  i;
    UInt  Word;

    // Protect access to EEPROM i/f lines on PLX
    Status = DtFastMutexAcquire(&pDvcData->m_Vpd.m_EepromMutex);
    if (!DT_SUCCESS(Status))
        return Status;

    // Initialisation
    DtaEepromPlxEnableEedoPin(pDvcData);      // Make DO output (for 9056)
    DtaEepromPlxSetPeAndPre(pDvcData, 0, 0);  // PE = 0, PRE = 0

    // READ Instruction, first part
    DtaEepromPlxInstruction(pDvcData, 2, WordAddr, 0);

    // Turn around data line
    DtaEepromPlxFloatEedoPin(pDvcData);       // Float DO pin
    DtaEepromPlxClockPulse(pDvcData);         // Clock start bit (should be 0)

    // Read 16 bits into Word
    for (Word=0, i=0; i<16; i++)
    {
        Word = (Word<<1) | DtaEepromPlxReadDi(pDvcData);
        DtaEepromPlxClockPulse(pDvcData);
    }

    // Copy data to output parameter
    *pData = (UInt16)Word;

    // Finally: falling edge on CS
    DtaEepromPlxCsFallingEdge(pDvcData);

    // Release EEPROM mutex
    DtFastMutexRelease(&pDvcData->m_Vpd.m_EepromMutex);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxWriteWord16 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write one 16-bit word to the serial EEPROM.
//
static DtStatus  DtaEepromPlxWriteWord16(
    DtaDeviceData*  pDvcData,   // Device extension
    UInt  WordAddr,             // 16-bit word address
    UInt  Data)                 // Data to be written to serial EEPROM
{
    DtStatus  Status;
    Int  Count, i, Mask;

    DtDbgOut(MAX, VPD, "Addr=0x%x Data=0x%04x", WordAddr, Data);

    // Protect access to EEPROM i/f lines on PLX
    Status = DtFastMutexAcquire(&pDvcData->m_Vpd.m_EepromMutex);
    if (!DT_SUCCESS(Status))
        return Status;

    // Initialisation
    DtaEepromPlxEnableEedoPin(pDvcData);      // Make DO output (for 9056)
    DtaEepromPlxSetPeAndPre(pDvcData, 1, 0);  // PE = 1, PRE = 0

    // WEN Instruction
    DtaEepromPlxInstruction(pDvcData, 0, 0xFF, 1);

    // WRITE Instruction
    DtaEepromPlxInstruction(pDvcData, 1, WordAddr, 0);

    // Data D15..D0
    for (Mask=0x8000, i=0; i<16; i++, Mask>>=1)
        DtaEepromPlxSetDoAndClk(pDvcData, ((Data&Mask)==0 ? 0 : 1));

    // Falling edge on CS triggers programming action
    DtaEepromPlxCsFallingEdge(pDvcData);      // Falling edge on CS
    DtaEepromPlxFloatEedoPin(pDvcData);       // Float DO pin

    // Set CS to 1, to wait for EEPROM Ready signal
    DtaEepromPlxSetCs(pDvcData, 1);

    // Wait while EEPROM signals Busy (DI==0), in 10ms chunks, with a maximum of 100ms
    Count = 0;
    do
    {
        DtSleep(10);
    } while (DtaEepromPlxReadDi(pDvcData)==0 && ++Count<10);

    // Return unsuccessful if DI still signals Busy (0)
    if (DtaEepromPlxReadDi(pDvcData) == 0)
    {
        // Don't forget to release EEPROM mutex
        DtFastMutexRelease(&pDvcData->m_Vpd.m_EepromMutex);
        return DT_STATUS_FAIL;
    }

    // WDS Instruction
    DtaEepromPlxInstruction(pDvcData, 3, 0, 1);

    // Release PE (write) signal
    DtaEepromPlxSetPeAndPre(pDvcData, 0, 0);  // PE = 0, PRE = 0
    
    // Release EEPROM mutex
    DtFastMutexRelease(&pDvcData->m_Vpd.m_EepromMutex);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxClearSkDoCs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Clear serial EEPROM lines: SK, CS, DO.
//
static void  DtaEepromPlxClearSkDoCs(DtaDeviceData* pDvcData)
{
    UInt32  Control;
    volatile UInt8*  pPci905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;

    // Clear SK, CS, DO
    Control = Pci905xReadUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
    Control &= ~(PCI905X_CNTRL_EESK | PCI905X_CNTRL_EECS | PCI905X_CNTRL_EEDO);
    Pci905xWriteUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxClockPulse -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Issue a clock pulse on the SK line of the serial EEPROM.
//
static void  DtaEepromPlxClockPulse(DtaDeviceData* pDvcData)
{
    UInt32  Control;
    volatile UInt8*  pPci905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;

    // Let current value on DO stabilise (data set-up time)
    DtWaitBlock(EEPROM_PLX_DELAY_DATA_SETUP_US);

    // Write 1 to SK and wait
    Control = Pci905xReadUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
    Control |= PCI905X_CNTRL_EESK;
    Pci905xWriteUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
    DtWaitBlock(EEPROM_PLX_DELAY_DATA_HOLD_US);

    // Write 0 to SK
    Control &= ~PCI905X_CNTRL_EESK;
    Pci905xWriteUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxCsFallingEdge -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Generate falling edge on CS and wait minimum CS time.
// Clear DO, too.
//
static void  DtaEepromPlxCsFallingEdge(DtaDeviceData* pDvcData)
{
    DtWaitBlock(EEPROM_PLX_DELAY_DATA_HOLD_US);
    DtaEepromPlxSetCsDo(pDvcData, 0, 0);
    DtWaitBlock(EEPROM_PLX_MINIMUM_CS_LOW);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxEnableEedoPin -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// For boards with the PCI9056, clear EEDO Input Enable.
// The PCI9054 does not have an enable, but an open-drain output. Therefore, nothing
// needs to be done when the output needs to be enabled.
//
static void  DtaEepromPlxEnableEedoPin(DtaDeviceData* pDvcData)
{
    UInt32  Control;
    volatile UInt8*  pPci905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;

    if (pDvcData->m_DevInfo.m_Uses9056)
    {
        Control  = Pci905xReadUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
        Control &= ~(PCI9056_CNTRL_EEDO_INPEN);
        Pci905xWriteUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxFloatEedoPin -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set DO (for 9054) and EEDO Input Enable (for 9056)
//
static void  DtaEepromPlxFloatEedoPin(DtaDeviceData* pDvcData)
{
    UInt32  Control;
    volatile UInt8*  pPci905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;

    Control  = Pci905xReadUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
    if (pDvcData->m_DevInfo.m_Uses9056)
        Control |= PCI9056_CNTRL_EEDO_INPEN;
    else
        Control |= PCI905X_CNTRL_EEDO;
    Pci905xWriteUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxInstruction -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Issue instruction to serial EEPROM.
//
void  DtaEepromPlxInstruction(
    DtaDeviceData*  pDvcData,   // Device extension
    Int  Command,               // Command (2 bits)
    Int  Address,               // Address bits
    Int  Close)                 // Close instruction: must be 1 for WEN, WDS
{
    Int  i, Mask;

    // Make sure CS is low for at least EEPROM_PLX_MINIMUM_CS_LOW microseconds
    DtaEepromPlxClearSkDoCs(pDvcData);    // SK = DO = CS = 0
    DtWaitBlock(EEPROM_PLX_MINIMUM_CS_LOW);

    // Start bit
    DtaEepromPlxSetCsDo(pDvcData, 1, 1);  // CS = 1, DO = 1; Clock (Start bit)
    DtaEepromPlxClockPulse(pDvcData);

    // Command
    DtaEepromPlxSetDoAndClk(pDvcData, (Command>>1)&1);
    DtaEepromPlxSetDoAndClk(pDvcData, Command&1);

    // Address
    for (Mask=0x80, i=0; i<8; i++, Mask>>=1)
        DtaEepromPlxSetDoAndClk(pDvcData, ((Address&Mask)==0 ? 0 : 1));

    // For 11-clock instructions: falling edge on CS
    if (Close)
        DtaEepromPlxCsFallingEdge(pDvcData);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxReadDi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read DI bit (which is DO of the serial EEPROM).
//
static UInt  DtaEepromPlxReadDi(DtaDeviceData* pDvcData)
{
    volatile UInt8*  pPci905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;

    if ((Pci905xReadUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT) & PCI905X_CNTRL_EEDI) == 0)
        return 0;
    else
        return 1;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxSetCs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Give a value to CS.
//
static void  DtaEepromPlxSetCs(DtaDeviceData* pDvcData, Int CsValue)
{
    UInt32  Control;
    volatile UInt8*  pPci905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;

    Control = Pci905xReadUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
    Control &= ~PCI905X_CNTRL_EECS;
    if (CsValue != 0)
        Control |= PCI905X_CNTRL_EECS;
    Pci905xWriteUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxSetCsDo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Give a value to CS and Do.
//
static void  DtaEepromPlxSetCsDo(DtaDeviceData* pDvcData, Int CsValue, Int DoValue)
{
    UInt32  Control;
    volatile UInt8*  pPci905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;

    Control = Pci905xReadUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
    Control &= ~(PCI905X_CNTRL_EECS | PCI905X_CNTRL_EEDO);
    if (CsValue != 0)
        Control |= PCI905X_CNTRL_EECS;
    if (DoValue != 0)
        Control |= PCI905X_CNTRL_EEDO;
    Pci905xWriteUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxSetDoAndClk -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Give a value to DO and issue a clock pulse.
//
static void  DtaEepromPlxSetDoAndClk(DtaDeviceData* pDvcData, Int DoValue)
{
    UInt32  Control;
    volatile UInt8*  pPci905XRegs = pDvcData->m_Pci905XConfRegs.m_pKernel;

    // Set DO
    Control = Pci905xReadUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
    Control &= ~PCI905X_CNTRL_EEDO;
    if (DoValue != 0)
        Control |= PCI905X_CNTRL_EEDO;
    Pci905xWriteUInt32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);

    // Issue clock pulse
    DtaEepromPlxClockPulse(pDvcData);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromPlxSetPeAndPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtaEepromPlxSetPeAndPre(
    DtaDeviceData*  pDvcData,
    Int  Pe,                        // PE (write enable): 0 or 1
    Int  Pre)                       // PRE (Protect Register Enable): 0 or 1
{
    DtaRegGenCtrlSetPE(pDvcData->m_pGenRegs, Pe);
    DtaRegGenCtrlSetPRE(pDvcData->m_pGenRegs, Pre);
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ SPI Read/Write +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  EEPROM_SPI_MAX_READ_BUSY_COUNT  10000   // Maximum busy count for read
#define  EEPROM_SPI_MAX_PROG_BUSY_COUNT  100000  // Maximum busy count for program

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static void  DtaEepromSpiLock(DtaDeviceData* pDvcData);
static void  DtaEepromSpiUnlock(DtaDeviceData* pDvcData);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromSpiRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read EEPROM into cache.
//
DtStatus  DtaEepromSpiRead(
    DtaDeviceData*  pDvcData,
    UInt  StartOffset,          // Start address (BYTE address)
    UInt8*  pBuf,
    UInt  Length)
{
    UInt  Addr, i;
    UInt  w;
    volatile UInt8*  pBase = pDvcData->m_pGenRegs;
    UInt8*  pData = pBuf;

     // We only support writing of full pages...
    if (StartOffset%pDvcData->m_Vpd.m_EepromPageSize != 0
        || Length%pDvcData->m_Vpd.m_EepromPageSize != 0)
        return DT_STATUS_INVALID_PARAMETER;

    // Unlock access to SPI interface
    DtaEepromSpiUnlock(pDvcData);

    DtDbgOut(MAX, VPD, "Start reading EEPROM");

    // Read data in page size bursts
    for (Addr=StartOffset; Addr<StartOffset+Length;
                                           Addr+=pDvcData->m_Vpd.m_EepromPageSize)
    {
        // Issue EEPROM READ command
        DtaRegSpiCtrlSetAddrRead(pBase, Addr);

        // Wait until busy is released
        i = 0;
        while (DtaRegSpiCtrlGetBusy(pBase) && i<EEPROM_SPI_MAX_READ_BUSY_COUNT)
            i++;

        // Read data from FIFO
        for (i = 0; i < (pDvcData->m_Vpd.m_EepromPageSize / 4); i++)
        {
            w = DtaRegSpiDataGet(pBase);
            *pData++ = (w>>24) & 0xFF;
            *pData++ = (w>>16) & 0xFF;
            *pData++ = (w>>8) & 0xFF;
            *pData++ = (w) & 0xFF;
        }
    }

    DtDbgOut(MIN, VPD, "Reading of EEPROM completed");

    // Lock EEPROM again
    DtaEepromSpiLock(pDvcData);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromSpiWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write EEPROM from cache.
//
DtStatus  DtaEepromSpiWrite(
    DtaDeviceData*  pDvcData,
    UInt  StartOffset,          // Start address (BYTE address)
    UInt8*  pBuf,
    UInt  Length)
{
    UInt  Addr, i;
    UInt  w;
    volatile UInt8*  pBase = pDvcData->m_pGenRegs;
    UInt8*  pData;

    // We only support writing of full pages...
    if (StartOffset%pDvcData->m_Vpd.m_EepromPageSize != 0
        || Length%pDvcData->m_Vpd.m_EepromPageSize != 0)
        return DT_STATUS_INVALID_PARAMETER;

    // Unlock access to SPI interface
    DtaEepromSpiUnlock(pDvcData);

    DtDbgOut(MAX, VPD, "Writing EEPROM");

    // Reprogram EEPROM in page size bursts
    pData = pBuf;
    for (Addr=StartOffset; Addr<StartOffset+Length; 
                                                   Addr+=pDvcData->m_Vpd.m_EepromPageSize)
    {
        // Write data to FIFO
        for (i=0; i<pDvcData->m_Vpd.m_EepromPageSize/4; i++)
        {
            w = (pData[0]<<24) +  (pData[1]<<16) + (pData[2]<<8) + pData[3];
            DtaRegSpiDataSet(pBase, w);
            pData += 4;
        }

        // Issue EEPROM PROGRAM command
        DtaRegSpiCtrlSetAddrProg(pBase, Addr);

        // Wait until busy is released
        i = 0;
        while (DtaRegSpiCtrlGetBusy(pBase) && i<EEPROM_SPI_MAX_PROG_BUSY_COUNT)
            i++;
    }

    DtDbgOut(MIN, VPD, "Writing of EEPROM completed");

    // Lock EEPROM again
    DtaEepromSpiLock(pDvcData);

    return DT_STATUS_OK;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+ SPI Read/Write - static functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromSpiLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Generate lock sequence, for closing SPI read/write operations
//
static void  DtaEepromSpiLock(DtaDeviceData* pDvcData)
{
    volatile UInt8*  pBase = pDvcData->m_pGenRegs;

    DtDbgOut(AVG, VPD, "Lock SPI");

    // Write '1' to let FPGA go to the Idle state (=locked)
    DtaRegSpiCtrlSetUnlock(pBase, 0x1);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEepromSpiUnlock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Unlock sequence, so that we can start reading/writing the SPI
//
static void  DtaEepromSpiUnlock(DtaDeviceData* pDvcData)
{
    volatile UInt8*  pBase = pDvcData->m_pGenRegs;

    DtDbgOut(AVG, VPD, "Unlock SPI");

    // Init: write '1' to let FPGA go to the Idle state
    DtaRegSpiCtrlSetUnlock(pBase, 0x1);

    DtaRegSpiCtrlSetUnlock(pBase, 0x3);
    DtaRegSpiCtrlSetUnlock(pBase, 0xD);
    DtaRegSpiCtrlSetUnlock(pBase, 0x6);
    DtaRegSpiCtrlSetUnlock(pBase, 0xC);
}
