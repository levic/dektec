//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Hdmi.c *#*#*#*#*#*#*#*#*#* (C) 2015-2016 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2015-2016 DekTec Digital Video B.V.
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


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static void  DtaHdmiClearStatus(DtaHdmiStatus* pStatus);
static void  DtaHdmiLogStatus(DtaHdmiStatus* pStatus);
static DtStatus  DtaHdmiDoDetectVidStd(DtaHdmiStatus*  pStatus, Int*  pVidStd);
static DtStatus  DtaHdmiConfigForSd(DtaHdmi*  pHdmi);
static DtStatus  DtaHdmiConfigForHd(DtaHdmi*  pHdmi);
static void  DtaHdmiHandleIntDpc(DtDpcArgs* pArgs);
static void  DtaHdmiControllerThread(DtThread* pThread, void* pContext);
static DtStatus  DtaHdmiWriteReg(DtaHdmi* pHdmi, UInt DvcAddr, UInt8 Addr, UInt8 Value);
static DtStatus  DtaHdmiReadReg(DtaHdmi* pHdmi, UInt DvcAddr, UInt8 Addr, UInt8* pValue);

// Helper macros to avoid repeating registerblock names while at the same time
// keeping the prefix for the defines.
#define ADV7610_WRITE_REG(BLOCK, REGNAME, VALUE) \
       DtaHdmiWriteReg(pHdmi, ADV7610_I2CADDR_##BLOCK, ADV7610_##BLOCK##_##REGNAME, VALUE)
#define ADV7610_READ_REG(BLOCK, REGNAME, VALUE) \
        DtaHdmiReadReg(pHdmi, ADV7610_I2CADDR_##BLOCK, ADV7610_##BLOCK##_##REGNAME, VALUE)

// 8-bit device addresses
#define ADV7610_I2CADDR_GENERAL    0x98
#define ADV7610_I2CADDR_CEC        0x20
#define ADV7610_I2CADDR_INFOFRAME  0x22
#define ADV7610_I2CADDR_KSV        0x24
#define ADV7610_I2CADDR_EDID       0x26
#define ADV7610_I2CADDR_HDMI       0x28
#define ADV7610_I2CADDR_CP         0x2A

// IO/General register block definition
#define ADV7610_GENERAL_VIDEO_STANDARD           0x00
#define ADV7610_GENERAL_PRIMARY_MODE             0x01
#define ADV7610_GENERAL_IO_REG_02                0x02
#define ADV7610_GENERAL_IO_REG_03                0x03
#define ADV7610_GENERAL_IO_REG_04                0x04
#define ADV7610_GENERAL_IO_REG_05                0x05
#define ADV7610_GENERAL_IO_REG_06                0x06
#define ADV7610_GENERAL_IO_REG_0C                0x0C
#define ADV7610_GENERAL_IO_REG_15                0x15
#define ADV7610_GENERAL_LLC_DLL                  0x19
#define ADV7610_GENERAL_HPA_REG2                 0x21
#define ADV7610_GENERAL_IO_REG_33                0x33
#define ADV7610_GENERAL_INT1_CONFIGURATION       0x40
#define ADV7610_GENERAL_HDMI_LVL_RAW_STATUS1     0x60
#define ADV7610_GENERAL_HDMI_LVL_INT_STATUS1     0x61
#define ADV7610_GENERAL_HDMI_LVL_INT_CLR1        0x62
#define ADV7610_GENERAL_HDMI_LVL_INT_MASKB1      0x64
#define ADV7610_GENERAL_HDMI_LVL_RAW_STATUS2     0x65
#define ADV7610_GENERAL_HDMI_LVL_INT_STATUS2     0x66
#define ADV7610_GENERAL_HDMI_LVL_INT_CLR2        0x67
#define ADV7610_GENERAL_HDMI_LVL_INT_MASKB2      0x69
#define ADV7610_GENERAL_HDMI_LVL_RAW_STATUS3     0x6A
#define ADV7610_GENERAL_HDMI_LVL_INT_STATUS3     0x6B
#define ADV7610_GENERAL_HDMI_LVL_INT_CLR3        0x6C
#define ADV7610_GENERAL_HDMI_LVL_INT_MASKB3      0x6E
#define ADV7610_GENERAL_HDMI_LVL_RAW_STATUS4     0x6F
#define ADV7610_GENERAL_HDMI_LVL_INT_STATUS4     0x70
#define ADV7610_GENERAL_HDMI_LVL_INT_CLR4        0x71
#define ADV7610_GENERAL_HDMI_LVL_INT_MASKB4      0x73
#define ADV7610_GENERAL_HDMI_EDG_RAW_STATUS3     0x83
#define ADV7610_GENERAL_HDMI_EDG_STATUS3         0x84
#define ADV7610_GENERAL_HDMI_EDG_INT_CLR3        0x85
#define ADV7610_GENERAL_HDMI_EDG_INT_MASKB3      0x87
#define ADV7610_GENERAL_CEC_SLAVE_ADDRESS        0xF4
#define ADV7610_GENERAL_INFOFRAME_SLAVE_ADDRESS  0xF5
#define ADV7610_GENERAL_KSV_SLAVE_ADDRESS        0xF9
#define ADV7610_GENERAL_EDID_SLAVE_ADDRESS       0xFA
#define ADV7610_GENERAL_HDMI_SLAVE_ADDRESS       0xFB
#define ADV7610_GENERAL_CP_SLAVE_ADDRESS         0xFD
#define ADV7610_GENERAL_IO_REG_FF                0xFF

// HDMI register block definition
#define ADV7610_HDMI_REGISTER_00                 0x00
#define ADV7610_HDMI_REGISTER_01                 0x01
#define ADV7610_HDMI_REGISTER_03                 0x03
#define ADV7610_HDMI_REGISTER_04                 0x04
#define ADV7610_HDMI_REGISTER_05                 0x05
#define ADV7610_HDMI_LINE_WIDTH1                 0x07
#define ADV7610_HDMI_LINE_WIDTH2                 0x08
#define ADV7610_HDMI_FIELD0_HEIGHT1              0x09
#define ADV7610_HDMI_FIELD0_HEIGHT2              0x0A
#define ADV7610_HDMI_FIELD1_HEIGHT1              0x0B
#define ADV7610_HDMI_FIELD1_HEIGHT2              0x0C
#define ADV7610_HDMI_MUTE_MASK21_16              0x14
#define ADV7610_HDMI_MUTE_MASK15_8               0x15
#define ADV7610_HDMI_MUTE_MASK7_0                0x16
#define ADV7610_HDMI_PACKETS_DETECTED_2          0x18
#define ADV7610_HDMI_PACKETS_DETECTED_3          0x19
#define ADV7610_HDMI_CHANNEL_STATUS_DATA_1       0x36
#define ADV7610_HDMI_CHANNEL_STATUS_DATA_2       0x37
#define ADV7610_HDMI_CHANNEL_STATUS_DATA_3       0x38
#define ADV7610_HDMI_CHANNEL_STATUS_DATA_4       0x39
#define ADV7610_HDMI_CHANNEL_STATUS_DATA_5       0x3A
#define ADV7610_HDMI_REGISTER_48                 0x48
#define ADV7610_HDMI_REGISTER_51                 0x51
#define ADV7610_HDMI_REGISTER_52                 0x52
#define ADV7610_HDMI_HDMI_COLORSPACE             0x53
#define ADV7610_HDMI_REGISTER_02                 0x83 // Not a typo.REGISTER_02 is at 0x83
#define ADV7610_HDMI_EQ_DYNAMIC_ENABLE           0x96

// CP register block definition
#define ADV7610_CP_SYNC_DET_CNTRL_CH1_RB_3       0xB1
#define ADV7610_CP_SYNC_DET_CNTRL_CH1_RB_6_1     0xB8
#define ADV7610_CP_SYNC_DET_CNTRL_CH1_RB_6_2     0xB9
#define ADV7610_CP_HDMI_CP_CNTRL_1               0xBA
#define ADV7610_CP_REG_FF                        0xFF

// KSV/Repeater register block definition
#define ADV7610_KSV_HDCP_EDID_CONTROLS           0x74

// Infoframe register block definition
#define ADV7610_INFOFRAME_AVI_INF_PB_0_START     0x00
#define ADV7610_INFOFRAME_AUD_INF_PB_0_START     0x1C
#define ADV7610_INFOFRAME_AVI_PACKET_ID          0xE0

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- HdmiVidStdInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _HdmiVidStdInfo
{
    Int  m_VidStd;
    Int  m_Width;
    Int  m_Height;
    Bool  m_Interlaced;
    Int  m_MinFieldRate;
    Int  m_MaxFieldRate;
} HdmiVidStdInfo;

static const HdmiVidStdInfo  g_HdmiVidInfo[] = {
    {DT_VIDSTD_525I59_94,   720,  480,  TRUE, 59640, 60300},
    {DT_VIDSTD_625I50,      720,  480,  TRUE, 49750, 50250},
    {DT_VIDSTD_720P23_98,  1280,  720, FALSE, 23856, 23988},
    {DT_VIDSTD_720P24,     1280,  720, FALSE, 23988, 24120},
    {DT_VIDSTD_720P25,     1280,  720, FALSE, 24875, 25125},
    {DT_VIDSTD_720P29_97,  1280,  720, FALSE, 29820, 29985},
    {DT_VIDSTD_720P30,     1280,  720, FALSE, 29985, 30150},
    {DT_VIDSTD_720P50,     1280,  720, FALSE, 49750, 50250},
    {DT_VIDSTD_720P59_94,  1280,  720, FALSE, 59640, 59970},
    {DT_VIDSTD_720P60,     1280,  720, FALSE, 59970, 60300},
    {DT_VIDSTD_1080P23_98, 1920, 1080, FALSE, 23856, 23988},
    {DT_VIDSTD_1080P24,    1920, 1080, FALSE, 23988, 24120},
    {DT_VIDSTD_1080P25,    1920, 1080, FALSE, 24875, 25125},
    {DT_VIDSTD_1080P29_97, 1920, 1080, FALSE, 29820, 29985},
    {DT_VIDSTD_1080P30,    1920, 1080, FALSE, 29985, 30150},
    {DT_VIDSTD_1080I50,    1920, 1080,  TRUE, 49750, 50250},
    {DT_VIDSTD_1080I59_94, 1920, 1080,  TRUE, 59640, 59970},
    {DT_VIDSTD_1080I60,    1920, 1080,  TRUE, 59970, 60300},
    {DT_VIDSTD_1080P50,    1920, 1080, FALSE, 49750, 50250},
    {DT_VIDSTD_1080P59_94, 1920, 1080, FALSE, 59640, 59970},
    {DT_VIDSTD_1080P60,    1920, 1080, FALSE, 59970, 60300},
    {DT_VIDSTD_480P59_94,   640,  480, FALSE, 59640, 60300},
    {DT_VIDSTD_525P59_94,   720,  480, FALSE, 59640, 60300},
    {DT_VIDSTD_625P50,      720,  576, FALSE, 49750, 50250},
};
static const Int  HDMI_VID_INFO_COUNT = sizeof(g_HdmiVidInfo)/sizeof(g_HdmiVidInfo[0]);


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EDID_Block -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static const UInt8 EDID_Block[256] = {
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
    0x12, 0x81, 0x84, 0x08, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0x19, 0x01, 0x03, 0x80, 0x4F, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x01, 0x00,
    0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
    0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x1D,
    0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x07, 0x00,
    0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFD, 0x00, 0x17, 0x3C, 0x1F,
    0x2D, 0x00, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x44,
    0x54, 0x41, 0x2D, 0x32, 0x31, 0x38, 0x30, 0x0A,
    0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE1,
    
    0x02, 0x03, 0x32, 0xF0, 0x50, 0x01, 0x02, 0x03,
    0x84, 0x85, 0x86, 0x87, 0x11, 0x12, 0x93, 0x94,
    0x95, 0x96, 0xA0, 0xA1, 0xA2, 0x32, 0x0D, 0x07,
    0x07, 0x15, 0x04, 0x50, 0x29, 0x04, 0x30, 0x7D,
    0x05, 0x22, 0x79, 0x05, 0x2A, 0x7D, 0x05, 0x32,
    0x83, 0x0F, 0x00, 0x00, 0x65, 0x03, 0x0C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpHdmiInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpHdmiInit(DtaNonIpPort* pNonIpPort)
{
    DtaHdmi*  pHdmi = &pNonIpPort->m_Hdmi;

    pHdmi->m_pFwbHdmiAdv7610Ctrl = &FwbHdmiAdv7610AudioSrc.HdmiAdv7610Ctrl;

    DT_RETURN_ON_ERROR(DtaI2cmInit(&pHdmi->m_I2c,
                                               &pHdmi->m_pFwbHdmiAdv7610Ctrl->I2cMaster));
    
    DtSpinLockInit(&pHdmi->m_StatusLock);

    // Init controller thread
    DtThreadInit(&pHdmi->m_ControlThread, DtaHdmiControllerThread, pNonIpPort);

    DtaHdmiClearStatus(&pNonIpPort->m_Hdmi.m_Status);

    DtEventInit(&pHdmi->m_IntEvent, TRUE);

    DtDpcInit(&pHdmi->m_HandleIntDpc, DtaHdmiHandleIntDpc, TRUE);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpHdmiInitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpHdmiInitPowerup(DtaNonIpPort* pNonIpPort)
{
    DT_RETURN_ON_ERROR(DtaI2cmInitPowerup(&pNonIpPort->m_Hdmi.m_I2c,
                                                                 pNonIpPort->m_pFwbRegs));

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpHdmiInitPowerUpPost -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpHdmiInitPowerUpPost(DtaNonIpPort* pNonIpPort)
{
    return DtThreadStart(&pNonIpPort->m_Hdmi.m_ControlThread);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpHdmiPowerdownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpHdmiPowerdownPre(DtaNonIpPort* pNonIpPort)
{
    return DtThreadStop(&pNonIpPort->m_Hdmi.m_ControlThread);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpHdmiInterruptEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpHdmiInterruptEnable(DtaNonIpPort* pNonIpPort)
{
    DT_ASSERT(pNonIpPort->m_CapHdmi);

    DT_RETURN_ON_ERROR(DtaI2cmInterruptEnable(&pNonIpPort->m_Hdmi.m_I2c));

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpHdmiInterruptDisable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpHdmiInterruptDisable(DtaNonIpPort* pNonIpPort)
{
    DT_ASSERT(pNonIpPort->m_CapHdmi);

    DT_RETURN_ON_ERROR(DtaI2cmInterruptDisable(&pNonIpPort->m_Hdmi.m_I2c));

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpHdmiInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaNonIpHdmiInterrupt(DtaNonIpPort*  pNonIpPort)
{
    DtaHdmi*  pHdmi = &pNonIpPort->m_Hdmi;
    Bool  IrqHandled = FALSE;

    IrqHandled |= DtaI2cmInterrupt(&pNonIpPort->m_Hdmi.m_I2c);

    if (DtaFwbRegRead(pNonIpPort->m_pFwbRegs,
                                    &pHdmi->m_pFwbHdmiAdv7610Ctrl->Status_IntStatus) == 1)
    {
        DtDpcArgs  DpcArgs;
        DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                                     &pHdmi->m_pFwbHdmiAdv7610Ctrl->Control_IntEnable, 0);
        DpcArgs.m_pContext = pNonIpPort;
        DtDpcSchedule(&pHdmi->m_HandleIntDpc, &DpcArgs);
        IrqHandled = TRUE;
    }

    return IrqHandled;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaHdmiControllerThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaHdmiControllerThread(DtThread* pThread, void* pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaNonIpPort* pNonIpPort = (DtaNonIpPort*)pContext;
    DtaHdmi*  pHdmi = &pNonIpPort->m_Hdmi;
    Bool  ConfigForHd = TRUE;

    DtDbgOut(MAX, HDMI, "Thread begin");
    
    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs, &pHdmi->m_pFwbHdmiAdv7610Ctrl->Control_Reset, 1);
    DtSleep(5);
    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs, &pHdmi->m_pFwbHdmiAdv7610Ctrl->Control_Reset, 0);
    DtSleep(10);
    
    // Disable VGA framing
    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                &pHdmi->m_pFwbHdmiAdv7610Ctrl->Control_VgaFraming, DTFWB_HDMI_NO_FRAMING);

    //  Disable powerdown bit
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, IO_REG_0C, 0x42);
    
    DtSleep(10);

    if (Status != DT_STATUS_OK)
    {
        DtDbgOut(ERR, HDMI, "Failed to disable powerdown bit: 0x%X", Status);
        DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                                         &pHdmi->m_pFwbHdmiAdv7610Ctrl->Control_Reset, 1);
        DtThreadWaitForStop(pThread);
        return;
    }

    // Initialize I2C addresses of other register pages
    
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, CEC_SLAVE_ADDRESS, ADV7610_I2CADDR_CEC);
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, INFOFRAME_SLAVE_ADDRESS, ADV7610_I2CADDR_INFOFRAME);
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, KSV_SLAVE_ADDRESS, ADV7610_I2CADDR_KSV);
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, EDID_SLAVE_ADDRESS, ADV7610_I2CADDR_EDID);
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, HDMI_SLAVE_ADDRESS, ADV7610_I2CADDR_HDMI);
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, CP_SLAVE_ADDRESS, ADV7610_I2CADDR_CP);

    if (Status != DT_STATUS_OK)
        DtDbgOut(ERR, HDMI, "Failed to set slave I2C addresses: 0x%X", Status);

    // Setting clock frequency to 27Mhz and pixel bus to mode 2
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, IO_REG_04, 0x60);
    // color space settings
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, IO_REG_02, 0xF4);
    // set IO_REG_05
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, IO_REG_05, 0x2C);
    // enabling output
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, IO_REG_15, 0xA8);
    // disable free run mode
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(CP, HDMI_CP_CNTRL_1, 0x00);
    // disable cable detect reset
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(HDMI, REGISTER_48, 0x40);
    // enable termination automatic control
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(HDMI, REGISTER_01, 0x01);
    // enable clock termination on port a
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(HDMI, REGISTER_02, 0xFE);
    // enable dynamic eq control
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(HDMI, EQ_DYNAMIC_ENABLE, 0x01);
    // Set bit width for right justified mode on I2S interface
    //if (Status == DT_STATUS_OK)
    //    Status = ADV7610_WRITE_REG(HDMI, REGISTER_03, 0x10);
    // Disable "mute if compressed audio is detected" bit
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(HDMI, MUTE_MASK21_16, 0x1F);
    
    if (Status != DT_STATUS_OK)
        DtDbgOut(ERR, HDMI, "Failed to initialize general registers");

    // Write EDID data
    if (Status == DT_STATUS_OK)
    {
        Int  i;
        UInt8  Sum = 0;
        UInt8  Edid[257];
        UInt32  Serial;
        Edid[0] = 0; // Address byte
        DtMemCopy(Edid+1, EDID_Block, sizeof(EDID_Block));
        Serial = (UInt32)pNonIpPort->m_pDvcData->m_DevInfo.m_Serial;
        DtMemCopy(Edid+1+0x0C, &Serial, sizeof(Serial));
        for (i=0; i<127; i++)
            Sum += Edid[i+1];
        Edid[0x80] = (255 - Sum + 1);
        Status = DtaI2cmWrite(&pHdmi->m_I2c, ADV7610_I2CADDR_EDID, sizeof(Edid), Edid);
    }

    if (Status != DT_STATUS_OK)
        DtDbgOut(ERR, HDMI, "Write EDID failure");

    // enable EDID for port a
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(KSV, HDCP_EDID_CONTROLS, 0x01);

    if (Status == DT_STATUS_OK)
        Status = DtaHdmiConfigForHd(pHdmi);
    ConfigForHd = TRUE;
    
    // Set interrupt signal duration to '11': Active until cleared
    // Set signal configuration '10': drives high when active
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, INT1_CONFIGURATION, 0xE2);
    
    // Enable cable detect / encrypted signal interrupts
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, HDMI_LVL_INT_MASKB4, 0x05);
    // Enable Video clock changed, Audio mode changed, new sample rate,
    // TMDS freq changed interrupts
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, HDMI_EDG_INT_MASKB3, 0x6A);
    // Enable TMDS-PLL locked, TMDS clock detect, Vertical sync lock, DE regeneration
    // lock interrupts
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, HDMI_LVL_INT_MASKB3, 0x53);
    // Enable NEW_AVI_INFO and NEW_AUDIO_INFO interrupts
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, HDMI_LVL_INT_MASKB1, 0x03);

    if (Status != DT_STATUS_OK)
    {
        DtDbgOut(ERR, HDMI, "HDMI initialization failed.");
        DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                                         &pHdmi->m_pFwbHdmiAdv7610Ctrl->Control_Reset, 1);
        DtThreadWaitForStop(pThread);
        return;
    }

    DtDbgOut(AVG, HDMI, "HDMI initialization complete");

    while (!pThread->m_StopThread)
    {
        UInt8  Value;
        DtaHdmiStatus  NewStatus;

        DtaHdmiClearStatus(&NewStatus);

        // Read/clear cable detect / encrypted signal interrupts
        Status = ADV7610_READ_REG(GENERAL, HDMI_LVL_INT_STATUS4, &Value);
        if (Status==DT_STATUS_OK && Value!=0)
            ADV7610_WRITE_REG(GENERAL, HDMI_LVL_INT_CLR4, Value&0x05);
        
        Status = ADV7610_READ_REG(GENERAL, HDMI_LVL_RAW_STATUS4, &Value);
        if (Status == DT_STATUS_OK)
        {
            NewStatus.m_CableDet = (Value&0x01)!=0;
            NewStatus.m_Encrypted = (Value&0x04)!=0;
        }

        // Read/clear TMDS freq changed interrupt
        Status = ADV7610_READ_REG(GENERAL, HDMI_EDG_STATUS3, &Value);
        if (Status==DT_STATUS_OK && Value!=0)
            ADV7610_WRITE_REG(GENERAL, HDMI_EDG_INT_CLR3, Value);
        if (NewStatus.m_CableDet)
        {
            Status = ADV7610_READ_REG(HDMI, REGISTER_04, &Value);
            NewStatus.m_TmdsPllLocked = (Value&0x02)!=0;
            NewStatus.m_AudioLocked = (Value&0x01)!=0;
        }
        // Read/clear TMDS-PLL locked, TMDS clock detect, Vertical sync lock, 
        // DE regeneration lock interrupts
        Status = ADV7610_READ_REG(GENERAL, HDMI_LVL_INT_STATUS3, &Value);
        if (Status==DT_STATUS_OK && Value!=0)
            ADV7610_WRITE_REG(GENERAL, HDMI_LVL_INT_CLR3, Value);

        // Read/clear new audio info interrupt
        Status = ADV7610_READ_REG(GENERAL, HDMI_LVL_INT_STATUS1, &Value);
        if (Status==DT_STATUS_OK && Value!=0)
            ADV7610_WRITE_REG(GENERAL, HDMI_LVL_INT_CLR1, Value);

        if (NewStatus.m_TmdsPllLocked)
        {
            Status = ADV7610_READ_REG(HDMI, HDMI_COLORSPACE, &Value);
            NewStatus.m_InpColorspace = Value&0x0F;
            
            Status = ADV7610_READ_REG(HDMI, REGISTER_05, &Value);
            NewStatus.m_PixelRepetition = Value&0x0F;
            
            Status = ADV7610_READ_REG(HDMI, LINE_WIDTH1, &Value);
            NewStatus.m_VertFilterLocked = (Value&0x80)!=0;
            NewStatus.m_DeRegenFilterLocked = (Value&0x20)!=0;
            NewStatus.m_VidWidth = (Value&0x01F)<<8;
            Status = ADV7610_READ_REG(HDMI, LINE_WIDTH2, &Value);
            NewStatus.m_VidWidth |= Value;
            
            Status = ADV7610_READ_REG(HDMI, FIELD0_HEIGHT1, &Value);
            NewStatus.m_VidHeight1 = (Value&0x01F)<<8;
            Status = ADV7610_READ_REG(HDMI, FIELD0_HEIGHT2, &Value);
            NewStatus.m_VidHeight1 |= Value;
            
            Status = ADV7610_READ_REG(HDMI, REGISTER_51, &Value);
            NewStatus.m_TmdsFreq = Value<<8;
            Status = ADV7610_READ_REG(HDMI, REGISTER_52, &Value);
            NewStatus.m_TmdsFreq |= Value;
            
            Status = ADV7610_READ_REG(HDMI, FIELD1_HEIGHT1, &Value);
            switch (Value>>6)
            {
            case 0: NewStatus.m_BitsPerChannel = 8; break;
            case 1: NewStatus.m_BitsPerChannel = 10; break;
            case 2: NewStatus.m_BitsPerChannel = 12; break;
            case 3: NewStatus.m_BitsPerChannel = 16; break;
            }
            NewStatus.m_Interlaced = (Value&0x20)!=0;
            NewStatus.m_VidHeight2 = (Value&0x01F)<<8;
            Status = ADV7610_READ_REG(HDMI, FIELD1_HEIGHT2, &Value);
            NewStatus.m_VidHeight2 |= Value;
            
            Status = ADV7610_READ_REG(GENERAL, HDMI_LVL_RAW_STATUS2, &Value);
            NewStatus.m_AudioChanStatusValid = (Value&0x80)!=0;
            if (NewStatus.m_AudioChanStatusValid)
            {
                UInt8  RegAddr = ADV7610_HDMI_CHANNEL_STATUS_DATA_1;
                DtaI2cmWriteRead(&pHdmi->m_I2c, ADV7610_I2CADDR_HDMI, 1, &RegAddr,
                                                   sizeof(NewStatus.m_AudioChannelStatus),
                                                   NewStatus.m_AudioChannelStatus);
            }
        }

        if (NewStatus.m_AudioLocked)
        {
            Status = ADV7610_READ_REG(HDMI, LINE_WIDTH1, &Value);
            NewStatus.m_AudioChannelMode = (Value>>6)&1;
            Status = ADV7610_READ_REG(HDMI, PACKETS_DETECTED_2, &Value);
            if ((Value&0x01) != 0)
                NewStatus.m_AudioType = 0;
            else if ((Value&0x02) != 0)
                NewStatus.m_AudioType = 1;
            else if ((Value&0x04) != 0)
                NewStatus.m_AudioType = 2;
            else if ((Value&0x08) != 0)
                NewStatus.m_AudioType = 3;
        }
        
        Status = ADV7610_READ_REG(GENERAL, HDMI_LVL_RAW_STATUS1, &Value);
        NewStatus.m_AviInfoFrameValid = (Value&0x01)!=0;
        NewStatus.m_AudioInfoFrameValid = (Value&0x02)!=0;
        if (NewStatus.m_AviInfoFrameValid)
        {
            UInt8  RegAddr = ADV7610_INFOFRAME_AVI_PACKET_ID;
            DtaI2cmWriteRead(&pHdmi->m_I2c, ADV7610_I2CADDR_INFOFRAME, 1, &RegAddr,
                                                             3, NewStatus.m_AviInfoFrame);
            RegAddr = ADV7610_INFOFRAME_AVI_INF_PB_0_START;
            DtaI2cmWriteRead(&pHdmi->m_I2c, ADV7610_I2CADDR_INFOFRAME, 1, &RegAddr,
                        sizeof(NewStatus.m_AviInfoFrame) - 3, NewStatus.m_AviInfoFrame+3);
        }
        if (NewStatus.m_AudioInfoFrameValid)
        {
            UInt8  RegAddr = ADV7610_INFOFRAME_AUD_INF_PB_0_START;
            DtaI2cmWriteRead(&pHdmi->m_I2c, ADV7610_I2CADDR_INFOFRAME, 1, &RegAddr,
                          sizeof(NewStatus.m_AudioInfoFrame), NewStatus.m_AudioInfoFrame);
        }
        
        Status = ADV7610_READ_REG(GENERAL, HPA_REG2, &Value);
        NewStatus.m_HpaApplied = (Value&0x08)!=0;
        
        Status = ADV7610_READ_REG(CP, REG_FF, &Value);
        NewStatus.m_CpFreeRun = (Value&0x10)!=0;
        
        Status = ADV7610_READ_REG(CP, SYNC_DET_CNTRL_CH1_RB_3, &Value);
        NewStatus.m_StdiValid = (Value&0x80)!=0;

        if (NewStatus.m_StdiValid)
        {
            NewStatus.m_StdiInterlaced = (Value&0x40)!=0;
        
            Status = ADV7610_READ_REG(CP, SYNC_DET_CNTRL_CH1_RB_6_1, &Value);
            NewStatus.m_StdiFcl = (Value&0x01F)<<8;
            Status = ADV7610_READ_REG(CP, SYNC_DET_CNTRL_CH1_RB_6_2, &Value);
            NewStatus.m_StdiFcl |= Value;
        }

        if (TRUE) // TODO: if(StatusChanged)
        {
            const DtFwbAudioSrcWithDpll*  pAudioSrc =
                                                 &FwbHdmiAdv7610AudioSrc.AudioSrcWithDpll;
            Int  SampleRate = 44100;
            Int  NewVidStd = DT_VIDSTD_UNKNOWN;

            DtaHdmiLogStatus(&NewStatus);

            DtSpinLockAcquire(&pHdmi->m_StatusLock);
            pHdmi->m_Status = NewStatus;
            DtSpinLockRelease(&pHdmi->m_StatusLock);

            if (NewStatus.m_TmdsPllLocked)
            {
                Int  VgaFraming = DTFWB_HDMI_NO_FRAMING;
                Int  TmdsFreqMhz = (NewStatus.m_TmdsFreq>>7);
                if (TmdsFreqMhz<30 && ConfigForHd)
                {
                    DtaHdmiConfigForSd(pHdmi);
                    ConfigForHd = FALSE;
                }
                if (TmdsFreqMhz>30 && !ConfigForHd)
                {
                    DtaHdmiConfigForHd(pHdmi);
                    ConfigForHd = TRUE;
                }
                Status = DtaHdmiDoDetectVidStd(&NewStatus, &NewVidStd);
                if (!ConfigForHd && Status==DT_STATUS_OK)
                {
                    if (NewVidStd == DT_VIDSTD_480P59_94)
                        VgaFraming = DTFWB_HDMI_FRAME_VGA;
                    if (NewVidStd == DT_VIDSTD_525P59_94)
                        VgaFraming = DTFWB_HDMI_FRAME_WVGA;
                    if (NewVidStd == DT_VIDSTD_625P50)
                        VgaFraming = DTFWB_HDMI_FRAME_EDTV;
                }
                DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                           &pHdmi->m_pFwbHdmiAdv7610Ctrl->Control_VgaFraming, VgaFraming);
            }
            DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                                                  &pAudioSrc->Control_OperationalMode, 0);
            if (NewStatus.m_AudioLocked && NewStatus.m_AudioChanStatusValid)
            {
                switch (NewStatus.m_AudioChannelStatus[3] & 0x0F)
                {
                case 0: SampleRate = 44100; break;
                case 2: SampleRate = 48000; break;
                case 3: SampleRate = 32000; break;
                case 8: SampleRate = 88200; break;
                case 9: SampleRate = 768000; break;
                case 10: SampleRate = 96000; break;
                case 12: SampleRate = 176000; break;
                case 14: SampleRate = 192000; break;
                }
            }
            if (NewStatus.m_AudioLocked && (SampleRate==32000 || SampleRate==44100))
            {
                Int  RefClockRate;
                Int  PhaseIncr;
                DtaFwbRegWrite(pNonIpPort->m_pFwbRegs, &pAudioSrc->Control_Bypass, 0);
                DtaFwbRegWrite(pNonIpPort->m_pFwbRegs, &pAudioSrc->Control_InputRate,
                                                               SampleRate==32000 ? 0 : 1);
                RefClockRate = DtaFwbRegRead(pNonIpPort->m_pFwbRegs,
                              &pAudioSrc->SlowClockSynthesizer.Config_ReferenceClockRate);
                PhaseIncr = (Int)DtDivide64(64LL*(48<<24), RefClockRate*100, NULL);
                // Add 0.5% for more stable output
                PhaseIncr = (Int)DtDivide64(PhaseIncr*1005LL, 1000, NULL);
                DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                      &pAudioSrc->SlowClockSynthesizer.Control_PhaseIncrement, PhaseIncr);
                if (SampleRate == 32000)
                {
                    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                             &pAudioSrc->SlowClockSynthesizer.PhaseLock_InputDivide, 100);
                    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                           &pAudioSrc->SlowClockSynthesizer.PhaseLock_InputMultiply, 150);
                } else {
                    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                             &pAudioSrc->SlowClockSynthesizer.PhaseLock_InputDivide, 147);
                    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                           &pAudioSrc->SlowClockSynthesizer.PhaseLock_InputMultiply, 160);
                }
                DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                           &pAudioSrc->SlowClockSynthesizer.Control_OutputClockEnable, 1);
            } else {
                DtaFwbRegWrite(pNonIpPort->m_pFwbRegs, &pAudioSrc->Control_Bypass, 1);
                DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                           &pAudioSrc->SlowClockSynthesizer.Control_OutputClockEnable, 0);
            }
            DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                                                  &pAudioSrc->Control_OperationalMode, 1);


            pHdmi->m_NewVidStdCb(pHdmi->m_pPortCb, pNonIpPort->m_PortIndex, NewVidStd);
        }

        // Enable interrupt
        DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                                     &pHdmi->m_pFwbHdmiAdv7610Ctrl->Control_IntEnable, 1);
        // Wait for exit or HDMI interrupt
        DtThreadWaitForStopOrEvent(pThread, &pHdmi->m_IntEvent);
    }
    
    // Disable interrupt and put chip back in reset
    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                                     &pHdmi->m_pFwbHdmiAdv7610Ctrl->Control_IntEnable, 0);
    DtaFwbRegWrite(pNonIpPort->m_pFwbRegs,
                                         &pHdmi->m_pFwbHdmiAdv7610Ctrl->Control_Reset, 1);

    DtDbgOut(MAX, HDMI, "Thread exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaHdmiHandleIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaHdmiHandleIntDpc(DtDpcArgs* pArgs)
{
    DtaNonIpPort*  pNonIpPort = (DtaNonIpPort*)pArgs->m_pContext;
    DtaHdmi*  pHdmi = &pNonIpPort->m_Hdmi;
    DtDbgOut(MAX, HDMI, "HDMI interrupt processed");
    DtEventSet(&pHdmi->m_IntEvent);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaHdmiConfigForSd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaHdmiConfigForSd(DtaHdmi*  pHdmi)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDbgOut(AVG, HDMI, "Configuring chip for SD input");
    // Setting video PRIM_MODE to 0x6
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, PRIMARY_MODE, 0x06);
    // Setting video std to 0x08
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, VIDEO_STANDARD, 0x08);
    // setting output to 12 bit mode SDR ITU mode 1
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, IO_REG_03, 0x0A);
    // setting DLL enable and doubler : addr 0x19 in IO reg = 11000000
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, LLC_DLL, 0xC0);
    // setting DLL mux : addr 0x33 bit 6 = 1
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, IO_REG_33, 0x40);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaHdmiConfigForHd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaHdmiConfigForHd(DtaHdmi*  pHdmi)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDbgOut(AVG, HDMI, "Configuring chip for HD input");
    // Setting video PRIM_MODE to 0x5
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, PRIMARY_MODE, 0x05);
    // Setting video std to 0x13
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, VIDEO_STANDARD, 0x13);
    // setting output to 12 bit mode SDR ITU mode 2
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, IO_REG_03, 0x8A);
    // Disable DLL enable and doubler : addr 0x19 in IO reg = 00000000
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, LLC_DLL, 0x00);
    // Disable DLL mux : addr 0x33 bit 6 = 0
    if (Status == DT_STATUS_OK)
        Status = ADV7610_WRITE_REG(GENERAL, IO_REG_33, 0x00);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpHdmiSetNewVidStdCb -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpHdmiSetNewVidStdCb(DtaNonIpPort* pNonIpPort,
                                       pDtaEnDecNewInputVidStd Cb, DtaNonIpPort* pPortCb)
{
    DtaHdmi*  pHdmi = &pNonIpPort->m_Hdmi;
    pHdmi->m_NewVidStdCb = Cb;
    pHdmi->m_pPortCb = pPortCb;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaHdmiDetectVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaHdmiDoDetectVidStd(DtaHdmiStatus*  pStatus, Int*  pVidStd)
{
    // Init to 'safe' value
    *pVidStd = DT_VIDSTD_UNKNOWN;
    if (pStatus->m_VertFilterLocked && pStatus->m_StdiValid)
    {
        Int  i;
        Int  Width = pStatus->m_VidWidth;
        Int  Height = pStatus->m_VidHeight1;
        Int  FieldRate = 105468750 / pStatus->m_StdiFcl; // 105468750=27Mhz*1000/256
        if (pStatus->m_Interlaced)
            Height += pStatus->m_VidHeight2;

        DtDbgOut(AVG, HDMI, "Detect vid std: %dx%d. Fieldrate=%d. Interlaced=%d", 
                                                        pStatus->m_VidWidth, Height,
                                                        FieldRate, pStatus->m_Interlaced);

        for (i=0; i<HDMI_VID_INFO_COUNT; i++)
        {
            const HdmiVidStdInfo* pInfo = &g_HdmiVidInfo[i];
            if (pInfo->m_Width==Width && pInfo->m_Height==Height
                 && pInfo->m_Interlaced==pStatus->m_Interlaced
                 && pInfo->m_MinFieldRate<=FieldRate && pInfo->m_MaxFieldRate>=FieldRate)
            {
                *pVidStd = pInfo->m_VidStd;
                DtDbgOut(AVG, HDMI, "Detected as 0x%X", *pVidStd);
                break;
            }
        }
        if (*pVidStd == DT_VIDSTD_UNKNOWN)
            DtDbgOut(AVG, HDMI, "Not recognized as valid Vid std");
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpHdmiDetectVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpHdmiDetectVidStd(
    DtaNonIpPort*  pNonIpPort,
    Int*  pVidStd,
    Int*  pAspectRatio)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaHdmi*  pHdmi;
    
    DT_ASSERT(pNonIpPort!=NULL && pVidStd!=NULL);
    DT_ASSERT(pNonIpPort->m_CapHdmi);

    pHdmi = &pNonIpPort->m_Hdmi;

    DtSpinLockAcquire(&pHdmi->m_StatusLock);
    Status = DtaHdmiDoDetectVidStd(&pHdmi->m_Status, pVidStd);

    if (pHdmi->m_Status.m_AviInfoFrameValid)
    {
        if (pAspectRatio != NULL)
        {
            Int  PictureAspectRatio = (pHdmi->m_Status.m_AviInfoFrame[5]>>4)&0x03;
            switch (PictureAspectRatio)
            {
            case 1: *pAspectRatio = DTA_AR_4_3; break;
            case 2: *pAspectRatio = DTA_AR_16_9; break;
            default: *pAspectRatio = DTA_AR_UNKNOWN; break;
            }
        }
    }

    DtSpinLockRelease(&pHdmi->m_StatusLock);

    // 640x480P59.94 and 720x480P59.94 are converted by firmware to 720x480i59.94
    if (*pVidStd==DT_VIDSTD_480P59_94 || *pVidStd==DT_VIDSTD_525P59_94)
        *pVidStd = DT_VIDSTD_525I59_94;
    // 720x576P50 is converted by firmware to 720x576i50
    if (*pVidStd==DT_VIDSTD_625P50)
        *pVidStd = DT_VIDSTD_625I50;

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpHdmiGetAudioStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpHdmiGetAudioStatus(
    DtaNonIpPort*  pNonIpPort,
    DtaIoctlNonIpCmdGetAudioStatusOutput*  pOut)
{
    DtaHdmiStatus*  pStatus = &pNonIpPort->m_Hdmi.m_Status;
    Int  i;

    pOut->m_NumAudioChannels = 0;

    DtSpinLockAcquire(&pNonIpPort->m_Hdmi.m_StatusLock);

    if (pStatus->m_AudioLocked && pStatus->m_AudioInfoFrameValid)
    {
        Int  SampleRate = 44100;
        if (pStatus->m_AudioChanStatusValid)
        {
            switch (pStatus->m_AudioChannelStatus[3] & 0x0F)
            {
            case 0: SampleRate = 44100; break;
            case 2: SampleRate = 48000; break;
            case 3: SampleRate = 32000; break;
            case 8: SampleRate = 88200; break;
            case 9: SampleRate = 768000; break;
            case 10: SampleRate = 96000; break;
            case 12: SampleRate = 176000; break;
            case 14: SampleRate = 192000; break;
            }
        }
        if (pStatus->m_AudioChannelMode == 0)
        {
            // Audio packet have layout 1. Max 2 channels
            pOut->m_NumAudioChannels = 2;
        } else {
            // Audio packet have layout 0. Up to 8 channels.
            pOut->m_NumAudioChannels = (pStatus->m_AudioInfoFrame[1] & 0x07) + 1;
        }
        for (i=0; i<pOut->m_NumAudioChannels; i++)
        {
            pOut->m_AudioChanStatus[i].m_ChanIdx = i;
            pOut->m_AudioChanStatus[i].m_IsAsynchronous = 1;
            pOut->m_AudioChanStatus[i].m_Rate = SampleRate;
            if (pStatus->m_AudioChanStatusValid)
            {
                pOut->m_AudioChanStatus[i].m_ChanStatusNumValid =
                                                    sizeof(pStatus->m_AudioChannelStatus);
                DtMemCopy(pOut->m_AudioChanStatus[i].m_ChanStatusData,
                                         pStatus->m_AudioChannelStatus,
                                         pOut->m_AudioChanStatus[i].m_ChanStatusNumValid);
            } else {
                pOut->m_AudioChanStatus[i].m_ChanStatusNumValid = 0;
            }
        }
    }

    DtSpinLockRelease(&pNonIpPort->m_Hdmi.m_StatusLock);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaHdmiWriteReg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaHdmiWriteReg(DtaHdmi* pHdmi, UInt DvcAddr, UInt8 Addr, UInt8 Value)
{
    UInt8  Buf[2] = {Addr, Value};
    return DtaI2cmWrite(&pHdmi->m_I2c, DvcAddr, 2, Buf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaHdmiReadReg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaHdmiReadReg(DtaHdmi* pHdmi, UInt DvcAddr, UInt8 Addr, UInt8* pValue)
{
    return DtaI2cmWriteRead(&pHdmi->m_I2c, DvcAddr, 1, &Addr, 1, pValue);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaHdmiClearStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaHdmiClearStatus(DtaHdmiStatus* pStatus)
{
    Int  i;
    pStatus->m_CableDet = FALSE;
    pStatus->m_Encrypted = FALSE;
    pStatus->m_TmdsPllLocked = FALSE;
    pStatus->m_AudioLocked = FALSE;
    pStatus->m_AudioChannelMode = -1;
    pStatus->m_AudioType = -1;
    pStatus->m_AudioChanStatusValid = FALSE;
    for (i=0; i<5; i++)
        pStatus->m_AudioChannelStatus[i] = 0;
    pStatus->m_AviInfoFrameValid = FALSE;
    for (i=0; i<(Int)sizeof(pStatus->m_AviInfoFrame); i++)
        pStatus->m_AviInfoFrame[i] = 0;
    pStatus->m_AudioInfoFrameValid = FALSE;
    for (i=0; i<(Int)sizeof(pStatus->m_AudioInfoFrame); i++)
        pStatus->m_AudioInfoFrame[i] = 0;
    pStatus->m_VertFilterLocked = FALSE;
    pStatus->m_DeRegenFilterLocked = FALSE;
    pStatus->m_PixelRepetition = -1;
    pStatus->m_InpColorspace = -1;
    pStatus->m_VidWidth = -1;
    pStatus->m_VidHeight1 = -1;
    pStatus->m_VidHeight2 = -1;
    pStatus->m_Interlaced = FALSE;
    pStatus->m_BitsPerChannel = -1;
    pStatus->m_TmdsFreq = -1;
    pStatus->m_CpFreeRun = FALSE;
    pStatus->m_HpaApplied = FALSE;
    pStatus->m_StdiValid = FALSE;
    pStatus->m_StdiInterlaced = FALSE;
    pStatus->m_StdiFcl = -1;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaHdmiLogStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaHdmiLogStatus(DtaHdmiStatus* pStatus)
{
    DtDbgOut(MAX, HDMI, "Cable det: %d. TmdsPllLocked=%d", pStatus->m_CableDet,
                                                                pStatus->m_TmdsPllLocked);
    if (pStatus->m_TmdsPllLocked)
    {
        const char* pStr = NULL;

        if (!pStatus->m_VertFilterLocked)
            DtDbgOut(MAX, HDMI, "Vertical filter not locked");
        if (!pStatus->m_DeRegenFilterLocked)
            DtDbgOut(MAX, HDMI, "DE regen filter not locked");
        DtDbgOut(MAX, HDMI, "Pixel repetition: %d", pStatus->m_PixelRepetition);

        DtDbgOut(MAX, HDMI, "Tmds freq: %d.%03d Mhz", pStatus->m_TmdsFreq>>7,
                                                     (pStatus->m_TmdsFreq&0x7F)*1000/128);
        if (pStatus->m_Interlaced)
            DtDbgOut(MAX, HDMI, "Resolution: %dx%d (interlaced)", pStatus->m_VidWidth,
                                             pStatus->m_VidHeight1+pStatus->m_VidHeight2);
        else
            DtDbgOut(MAX, HDMI, "Resolution: %dx%d", pStatus->m_VidWidth,
                                                                   pStatus->m_VidHeight1);

        switch (pStatus->m_InpColorspace)
        {
        case 0: pStr = "RGB_LIMITED"; break;
        case 1: pStr = "RGB_FULL"; break;
        case 2: pStr = "YUV_601"; break;
        case 3: pStr = "YUV_709"; break;
        case 4: pStr = "XVYCC_601"; break;
        case 5: pStr = "XVYCC_709"; break;
        case 6: pStr = "YUV_601_FULL"; break;
        case 7: pStr = "YUV_709_FULL"; break;
        case 8: pStr = "sYCC_601"; break;
        case 9: pStr = "ADOBE_YCC_601"; break;
        case 10: pStr = "ADOBE_RGB"; break;
        default: pStr = "UNKNOWN"; break;
        }
        DtDbgOut(MAX, HDMI, "Colorspace: %s", pStr);
        DtDbgOut(MAX, HDMI, "Bits per channel: %d", pStatus->m_BitsPerChannel);
    }
    if (pStatus->m_StdiValid)
    {
        Int  Framerate;
        DtDbgOut(MAX, HDMI, "STDI valid. Interlaced=%s", pStatus->m_StdiInterlaced ?
                                                                        "TRUE" : "FALSE");
        DtDbgOut(MAX, HDMI, "STDI FCL=%d. FCL*256=%d", pStatus->m_StdiFcl,
                                                                  pStatus->m_StdiFcl*256);
        if (pStatus->m_StdiFcl > 0)
        {
            Framerate = 2700000000 / (pStatus->m_StdiFcl*256);
            DtDbgOut(MAX, HDMI, "Framerate = %d.%02d", Framerate/100, Framerate%100);
        }
    }
    if (pStatus->m_AudioLocked)
    {
        DtDbgOut(MAX, HDMI, "AudioChannelMode=%d. Type=%d", pStatus->m_AudioChannelMode,
                                                                    pStatus->m_AudioType);
        if (pStatus->m_AudioChanStatusValid)
        {
            DtDbgOut(MAX, HDMI, "Audio channel status: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                       pStatus->m_AudioChannelStatus[0], pStatus->m_AudioChannelStatus[1],
                       pStatus->m_AudioChannelStatus[2], pStatus->m_AudioChannelStatus[3],
                       pStatus->m_AudioChannelStatus[4]);
        }
        if (pStatus->m_AudioInfoFrameValid)
        {
            DtDbgOut(MAX, HDMI, "Audio InfoFrame data: 0x%02X 0x%02X 0x%02X 0x%02X",
                                                            pStatus->m_AudioInfoFrame[0],
                                                            pStatus->m_AudioInfoFrame[1],
                                                            pStatus->m_AudioInfoFrame[2],
                                                            pStatus->m_AudioInfoFrame[3]);
        }
    } else {
        DtDbgOut(MAX, HDMI, "Audio not locked");
    }
    if (pStatus->m_AviInfoFrameValid)
    {
            DtDbgOut(MAX, HDMI, "AVI InfoFrame data: 0x%02X 0x%02X 0x%02X 0x%02X",
                                                              pStatus->m_AviInfoFrame[4],
                                                              pStatus->m_AviInfoFrame[5],
                                                              pStatus->m_AviInfoFrame[6],
                                                              pStatus->m_AviInfoFrame[7]);
    }
    if (pStatus->m_CpFreeRun)
        DtDbgOut(MAX, HDMI, "FreeRun enabled");
    if (pStatus->m_HpaApplied)
        DtDbgOut(MAX, HDMI, "+5V applied to HPA_A pin");
}
