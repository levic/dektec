//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dtu.h *#*#*#*#*#*#*#*#*#*# (C) 2011-2012 DekTec
//
// Dtu driver - Interface for the Dtu common driver, used by the IAL.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2012 DekTec Digital Video B.V.
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

#ifndef __DTU_H
#define __DTU_H

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Defines +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// DTU TAG
#define  DTU_TAG        0x32555444  // '2UTD'

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU log levels -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// IAL related
#define  LOG_LEVEL_IAL           LOG_AVG
// Toplevel related
#define  LOG_LEVEL_DTU           LOG_AVG
// VPD related
#define  LOG_LEVEL_VPD           LOG_AVG
// I2C related
#define  LOG_LEVEL_I2C           LOG_AVG
// Events
#define  LOG_LEVEL_EVENTS        LOG_AVG
// Read/Write data
#define  LOG_LEVEL_RDWR          LOG_AVG
// IoConfig
#define  LOG_LEVEL_IOCONFIG      LOG_AVG
// Shared Buffer
#define  LOG_LEVEL_SHBUF         LOG_MAX
// NONIP
#define  LOG_LEVEL_NONIP         LOG_AVG


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Max file handles -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  MAX_NUM_FILE_HANDLES    256

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- USB timeouts -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  MAX_USB_RW_TIMEOUT      2000        // bulk read/write timeouts in ms (2 seconds)
#define  MAX_USB_REQ_TIMEOUT     10000       // vendor request timeouts in ms (10 seconds)

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTU device context +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtuDeviceInfo
{
    Int  m_TypeNumber;

    // USB properties, loaded by IAL
    UInt16  m_ProductId;        // Product ID
    UInt16  m_VendorId;         // Vendor ID
    UInt  m_UsbAddress;         // USB address
    
    // Info determined during DtuDeviceInitPost
    Int  m_HardwareRevision;    // Hardware revision (e.g. 302 = 3.2)
    Int  m_FirmwareVersion;     // Firmware Version (= Altera revision), e.g. 3 for
                                // "Firmware Version 3"
    Int  m_FirmwareVariant;     // Firmware Variant, e.g. to distinguish between
                                // firmware with different #inputs/#outputs
    UInt64  m_Serial;           // Serial number
    Int  m_SubType;             // Device subtype (0=none, 1=A, ...)
    Int  m_UsbSpeed;            // USB speed

} DtuDeviceInfo;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuFileHandle -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtuFileHandleInfo
{
    void*  m_pHandle;
    Bool  m_PowerDownPending;
    //DtuEvents  m_DtuEvents;
} DtuFileHandleInfo;

// Align bulk transfer to:
#define DTU_BULK_TRANSFER_ALIGN     1024
// Maximum bulk transfer
#define DTU_BULK_PACKETSIZE         0xFC00

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuPortLookup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#define  DTU_PORT_TYPE_NONE   0
#define  DTU_PORT_TYPE_NONIP  1
#define  DTU_PORT_TYPE_IP     2
typedef struct _DtuPortLookup
{
    Int  m_PortType;
    Int  m_Index;
} DtuPortLookup;

#define DTU3_PID_UNIT_EEPROM     0x00F3      // PID of an uninitialised DTU-3xx 
                                                // (=PID of FX3)

#define DTU_DEV_FLAG_VPD_CORRUPT     0x01
#define DTU_DEV_FLAG_NO_SERIAL       0x02
#define DTU_DEV_FLAG_NO_USB3         0x04
#define DTU_DEV_FLAG_SLEEPING        0x08 //TODOTM: implement

#define DTU_BOOTSTATE_WARM           0
#define DTU_BOOTSTATE_COLD           1
#define DTU_BOOTSTATE_FACTORY_WARM   2
#define DTU_BOOTSTATE_FACTORY_COLD   3

typedef struct  _DtuAltSetting
{
    Int  m_Bitrate;
} DtuAltSetting;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
struct _DtuDeviceData
{
    // IAL data
    DtuIalData  m_IalData;
    
    // Driver / Device objects
    DtDrvObject  m_Driver;
    DtDvcObject  m_Device;
  
    // General device information
    DtuDeviceInfo  m_DevInfo;

    // File handles
    DtFastMutex  m_FileHandleInfoMutex;
    DtuFileHandleInfo  m_FileHandleInfo[MAX_NUM_FILE_HANDLES];

    // Events
    DtuEvents  m_Events[MAX_NUM_FILE_HANDLES];
    
    // Used during powerup to determine what should be initialised
    // and to prevent user access if not completely initialised
    Bool  m_InitialPowerup;

    // Registry
    Bool  m_RegistryWriteBusy;

    // VPD
    DtuVpd  m_Vpd;

    // I2C
    DtuI2c  m_I2c;

    // EzUsb
    DtuEzUsb  m_EzUsb;

    // Port lookup
    DtuPortLookup*  m_pPortLookup;
    Int  m_NumPorts;

    // Non IP ports (NOTE: currently teher are no IP port in the DTU-series, but for 
    // future proving we use the same naming convention as in the Dta driver)
    DtuNonIpPort*  m_pNonIpPorts;
    DtFastMutex  m_ExclAccessMutex;
    Int  m_NumNonIpPorts;

    // IP ports
    // NOTE: DTU series doesnot have devices with IP ports yet, hence the following
    // members are commented out/not used for now
    //DtuIpDevice  m_IpDevice;
    Int  m_NumIpPorts;

    // Property Data
    DtPropertyData  m_PropData;

    Int  m_StateFlags;
    
    DtuAltSetting  m_AltSetting[MAX_ISO_ALTSETTINGS];
    Int  m_NumAltSettings;
    Int  m_CurAltSetting;

    Int  m_BootState;
};


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Dtu Common interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

DtStatus  DtuDriverInit(DtEvtLog* pEvtObject);
void  DtuDriverExit(void);

#define DT_STATE_D0 0
#define DT_STATE_D1 1
#define DT_STATE_D3 2

DtStatus  DtuDeviceInit(DtuDeviceData* pDvcData);
DtStatus  DtuDevicePowerUp(DtuDeviceData* pDvcData);
DtStatus  DtuDevicePowerDown(DtuDeviceData* pDvcData, Int TargetState);
void  DtuDeviceExit(DtuDeviceData* pDvcData);
void  Dtu3Shutdown(DtuDeviceData* pDvcData);

DtStatus  DtuDeviceOpen(DtuDeviceData* pDvcData, DtFileObject* pFile);
DtStatus  DtuDeviceClose(DtuDeviceData* pDvcData, DtFileObject* pFile);
DtStatus  DtuDeviceIoctl(DtuDeviceData* pDvcData, DtFileObject* pFile,
                                                                   DtIoctlObject* pIoctl);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
DtStatus  DtuGetNonIpPortIndex(DtuDeviceData* pDvcData, Int PortIndex,
                                                                    Int* pNonIpPortIndex);
DtStatus  DtuGetPortIndexNonIp(DtuDeviceData*  pDvcData, Int  NonIpPortIndex,
                                                                        Int*  pPortIndex);

#endif // #ifndef __DTU_H
