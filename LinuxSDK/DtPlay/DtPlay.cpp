//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPlay.cpp *#*#*#*#*#*#*#*#* (C) 2000-2010 DekTec
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	2006.10.08	MG	Created

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <stdio.h>
#include "DTAPI.h"
#include "DtPlay.h"
#include "DtSdiFileFmt.h"

#ifdef WIN32
	#include <conio.h>
#else
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPlay Version -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTPLAY_VERSION_MAJOR		3
#define DTPLAY_VERSION_MINOR		12
#define DTPLAY_VERSION_BUGFIX		0

// Command line option flags
const char c_LoopCountCmdLineFlag[]			= "l";

const char c_DvcTypeCmdLineFlag[]			= "t";
const char c_DvcNumCmdLineFlag[]			= "n";
const char c_PortCmdLineFlag[]				= "i";
const char c_PortDblBuffFlag[]				= "db";

const char c_TxModeCmdLineFlag[]			= "m";
const char c_TxRateCmdLineFlag[]			= "r";

const char c_DataPidCmdLineFlag[]			= "dP";

const char c_ModTypeCmdLineFlag[]			= "mt";
const char c_CarrierFreqCmdLineFlag[]		= "mf";
const char c_OutpLvlCmdLineFlag[]			= "ml";
const char c_CodeRateCmdLineFlag[]			= "mc";
const char c_QamJ83AnnexFlag[]				= "ma";

const char c_IqInterpFilterFlag[]			= "if";

const char c_OfdmConstellCmdLineFlag[]		= "mC";
const char c_OfdmBandwidthCmdLineFlag[]		= "mB";
const char c_OfdmTxModeCmdLineFlag[]		= "mT";
const char c_OfdmGaurdItvCmdLineFlag[]		= "mG";

const char c_DvbS2EnablePilotCmdLineFlag[]	= "mP";
const char c_DvbS2FecFrameLengthCmdLineFlag[] = "mF";
const char c_DvbS2GoldSequenceInitValFlag[]	= "mI";

const char c_DtmbFrameHdrModeCmdLineFlag[]	= "mH";

const char c_SnrCmdLineFlag[]				= "snr";

const char c_IpAddressCmdLineFlag[]			= "ipa";
const char c_IpProtocolCmdLineFlag[]		= "ipp";
const char c_IpNumTpPerIpCmdLineFlag[]		= "ipn";
const char c_IpTtlCmdLineFlag[]				= "ipt";

const char c_SilentModeCmdLineFlag[]		= "s";

const char c_StuffingModeCmdLineFlag[]		= "mS";

const char c_HelpCmdLineFlag[]				= "?";

const int c_BufSize = 1*1024*1024;		// Data transfer buffer size
const int c_MinFifoLoad	= 3*1024*1024;	// Minimum fifo load before starting DVB transmission

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Error messages -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Command line errors
const char c_CleUnknownOption[]			= "Unknown command-line option (-%s) encountered";
const char c_CleMissingArgument[]		= "Missing argument for command-line option: -%s";
const char c_CleNoArgumentExpected[]	= "-%s command-line option does not have a argument";
const char c_CleInvalidArgument[]		= "Invalid argument for command line option: -%s";
const char c_CleNoPlayFile[]			= "No play file specified";
const char c_CleTxRateNotDefined[]		= "Transport-Stream rate not specified";
const char c_CleModTypeNotDefined[]		= "Mod. Type (-mt) must be defined before (%s)";

// General errors
const char c_ErrDtapiHwScanFailed[]		= "DtapiHwFuncScan failed (ERROR: %s)";
const char c_ErrNoOutputFound[]			= "No output device in the system";
const char c_ErrNoDta1xxFound[]			= "No DTA-%d found";
const char c_ErrNoDtu2xxFound[]			= "No DTU-%d found";
const char c_ErrCouldNotFindOutputN[]	= "Could not find output board (#%d) in the system";
const char c_ErrCouldNotFindDta1xxN[]	= "Could not find DTA-%d (#%d) in the system";
const char c_ErrCouldNotFindDtu2xxN[]	= "Could not find DTU-%d (#%d) in the system";
const char c_ErrFailToAttachToDta1xx[]	= "Failed to attach to the DTA-%d on Bus: %d and Slot: %d";
const char c_ErrFailToAttachToDtu2xx[]	= "Failed to attach to the DTU-%d";
const char c_ErrFailToAttachToChan[]	= "Can't attach to the channel (ERROR: %s)";
const char c_ErrFailToSetIoConfig[]		= "Failed to set IO-configuration (ERROR: %s)";
const char c_ErrFailToGetIoConfig[]		= "Failed to get IO-configuration (ERROR: %s)";
const char c_ErrFailToOpenFile[]		= "Can't open '%s' for reading";
const char c_ErrReadFile[]				= "File read error";
const char c_ErrFailSetTxControl[]		= "SetTxControl failed (ERROR: %s)";
const char c_ErrFailSetTxMode[]			= "SetTxMode failed (ERROR: %s)";
const char c_ErrFailSetModControl[]		= "SetModControl failed (ERROR: %s)";
const char c_ErrFailSetRfControl[]		= "SetRfControl failed (ERROR: %s)";
const char c_ErrFailSetIpPars[]			= "SetIpPars failed (ERROR: %s)";
const char c_ErrFailSetTsRate[]			= "SetTsrateBps failed (ERROR: %s)";
const char c_ErrFailWrite[]				= "Write failed (ERROR: %s)";
const char c_ErrInvalidFileSize[]		= "Invalid file size";
const char c_ErrInvalidDtSdiFileHdr[]	= "Invalid DTSDI file header";
const char c_ErrFailedToSetOutputLevel[]= "Failed to set output level (ERROR: %s)";
const char c_ErrFailedToSetSNR[]		= "Failed to set SNR (ERROR: %s)";
const char c_FailedToInitIdsbtPars[]	= "Failed to initialise ISDB-T parameters (ERROR: %s)";
const char c_ErrFailGetFifoSize[]		= "Failed to get Fifo size (ERROR: %s)";

const char c_ErrDriverInCompPci[] =
	"The current Dta1xx driver (V%d.%d.%d %d) is not compatible with this\n" \
	"version of DtPlay.\n" \
	"Please install the latest version of the Dta1xx driver.";

const char c_ErrDriverInCompUsb[] =
	"The current Dtu2xx driver (V%d.%d.%d %d) is not compatible with this\n" \
	"version of DtPlay.\n" \
	"Please install the latest version of the Dtu2xx driver.";



//+=+=+=+=+=+=+=+=+=+=+=+=+=+ CommandLineParams implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::CommandLineParams -.-.-.-.-.-.-.-.-.-.-.-.-
//
CommandLineParams::CommandLineParams() :
	m_pLastFlag(NULL),
	m_LastFlagReqsArg(false),
	m_Error(false)
{
}

//.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::~CommandLineParams -.-.-.-.-.-.-.-.-.-.-.-.-
//
CommandLineParams::~CommandLineParams()
{
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void CommandLineParams::Init()
{
	m_FileName[0] = '\0';
	m_LoopCnt = 1;

	m_DvcType = -1;
	m_DvcNum = 1;
	m_Port = -1;
	m_DblBuff = false;

	m_PlayDtSdiFile = false;
	m_TxMode = -1;
	m_Stuffing = DTAPI_TXSTUFF_MODE_OFF;
	m_TxRate = -1;
	m_DataPid= -1;

	m_ModType = -1;
	m_QamJ83Annex = DTAPI_MOD_J83_A;
	m_CarrierFreq = -1.0;
	m_OutpLevel = -27.5;
	m_CodeRate = -1;

	m_IqInterpFilter = DTAPI_MOD_INTERPOL_OFDM;

	m_Snr = -1.0;

	m_Constellation = -1;
	m_Bandwidth = -1;
	m_OfdmTxMode = DTAPI_MOD_DVBT_8K;
	m_OfdmGuardItv = DTAPI_MOD_DVBT_G_1_4;

	m_DvbS2GoldSeqInit = 0;
	m_DvbS2Pilots = DTAPI_MOD_S2_NOPILOTS;
	m_DvbS2FecFrameLength = DTAPI_MOD_S2_LONGFRM;

	m_DtmbFrameHdrMode = DTAPI_MOD_DTMB_PN420;

	memset(&m_IpPars, 0, sizeof(m_IpPars));
	// Set default IP parameters
	m_IpPars.m_Port = 5678;
	m_IpPars.m_Protocol = DTAPI_PROTO_UDP;
	m_IpPars.m_NumTpPerIp = 7;

	m_SilentMode = false;
	
	m_ShowHelp = false;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::ParseCommandLine -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void CommandLineParams::ParseCommandLine(int argc, char* argv[])
{
	bool First = true;
	for ( int i=1; i<argc; i++ )
	{
		char* pParam = argv[i];
		bool Flag = false;
		bool Last = ( (i+1)==argc );
#ifdef WIN32
		if ( !m_LastFlagReqsArg && (pParam[0]=='-' || pParam[0] == '/') )
#else
        if ( !m_LastFlagReqsArg && pParam[0]=='-' ) // For Linux only '-' can be used for options
#endif
		{
			// remove flag specifier
			Flag = true;
			++pParam;
		}
		ParseParam(pParam, Flag, First, Last);
		First = false;
	}

	// Check for required parameters
	if ( strlen(m_FileName) == 0 && !m_ShowHelp )
		throw Exc(c_CleNoPlayFile);

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Set dynamic option default -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// Set default settings for parameters which are not defined on the command line

	// TxMode defined?
	if ( m_TxMode == -1 && m_ModType==DTAPI_MOD_ISDBT )
		m_TxMode = DTAPI_TXMODE_204;
	else if ( m_TxMode == -1 )
		m_TxMode = DTAPI_TXMODE_188;

	// Code rate defined
	if ( m_CodeRate == -1 )
	{
		if ( m_ModType == DTAPI_MOD_DTMB )
			m_CodeRate = DTAPI_MOD_DTMB_0_6;
		else
			m_CodeRate = DTAPI_MOD_3_4;

	}

	// Constellation defined?
	if ( m_Constellation == -1 )
	{
		if ( m_ModType == DTAPI_MOD_ATSC )
			m_Constellation = DTAPI_MOD_ATSC_VSB8;
		else if ( m_ModType == DTAPI_MOD_DTMB )
			m_Constellation = DTAPI_MOD_DTMB_QAM64;
		else if ( m_ModType == DTAPI_MOD_DVBT )
			m_Constellation = DTAPI_MOD_DVBT_QAM64;
		else
			m_Constellation = -1; // donot care
	}

	// Bandwitdh 
	if ( m_Bandwidth == -1 )
	{
		if ( m_ModType == DTAPI_MOD_DVBT )
			m_Bandwidth = DTAPI_MOD_DVBT_8MHZ;
		else if ( m_ModType == DTAPI_MOD_DTMB )
			m_Bandwidth = DTAPI_MOD_DTMB_8MHZ;
	}

	// Carrier defined?
	if ( m_CarrierFreq == -1.0 )
	{
		if ( m_ModType==DTAPI_MOD_QPSK || m_ModType==DTAPI_MOD_BPSK )
			m_CarrierFreq = 1915.0;		// L-Band
		else /* ISDBT, DVB-H/DVB-T, QAM */
			m_CarrierFreq = 578.0;		// UHF-band
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::ParseParam -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void CommandLineParams::ParseParam(
	char* pParam,
	bool Flag,
	bool First,
	bool Last)
{
	if ( Flag )
		ParseParamFlag(pParam, First, Last);
	else
		ParseParamNotFlag(pParam, First, Last);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::ParseParamFlag -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  CommandLineParams::ParseParamFlag(char* pParam, bool First, bool Last)
{
	// Did we expect an option??
	if ( m_LastFlagReqsArg && m_pLastFlag != NULL )
		throw Exc(c_CleMissingArgument, m_pLastFlag);


	if ( 0==strcmp(pParam, c_LoopCountCmdLineFlag) )
        m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_DvcTypeCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_DvcNumCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_PortCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_TxModeCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_TxRateCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_DataPidCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_ModTypeCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_QamJ83AnnexFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_CarrierFreqCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_OutpLvlCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_SnrCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_CodeRateCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_OfdmConstellCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_OfdmBandwidthCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_OfdmTxModeCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_OfdmGaurdItvCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_StuffingModeCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_SilentModeCmdLineFlag))
	{
		m_SilentMode = true;
		m_LastFlagReqsArg = false;
	}
	else if (0==strcmp(pParam, c_IqInterpFilterFlag))
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_DvbS2EnablePilotCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_DvbS2FecFrameLengthCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_DvbS2GoldSequenceInitValFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_DtmbFrameHdrModeCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_IpAddressCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_IpProtocolCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_IpNumTpPerIpCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_IpTtlCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_PortDblBuffFlag))
	{
		m_DblBuff = true;
		m_LastFlagReqsArg = false;
	}
	else if (0==strcmp(pParam, c_HelpCmdLineFlag))
	{
		m_ShowHelp = true;
		m_LastFlagReqsArg = false;
	}	else
		throw Exc(c_CleUnknownOption, pParam);

	m_pLastFlag = pParam;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::ParseParamNotFlag -.-.-.-.-.-.-.-.-.-.-.-.-
//
void  CommandLineParams::ParseParamNotFlag(char* pParam, bool First, bool Last)
{
	// First option should be our file name
	if ( First )
	{
		strcpy(m_FileName, pParam);
		m_pLastFlag = NULL;
		m_LastFlagReqsArg = false;
		return;
	}

	// Did we expect and option??
	if ( m_pLastFlag==NULL )
		return;
	if ( !m_LastFlagReqsArg )
		throw Exc(c_CleNoArgumentExpected, m_pLastFlag);


	if ( 0==strcmp(m_pLastFlag, c_LoopCountCmdLineFlag) )
	{
		m_LoopCnt = atoi(pParam);
		if ( m_LoopCnt<0  )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
    else if ( 0==strcmp(m_pLastFlag, c_DvcTypeCmdLineFlag) )
	{
		m_DvcType = atoi(pParam);
		if ( m_DvcType < 100 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_PortCmdLineFlag) )
	{
		m_Port = atoi(pParam);
		if ( m_Port < 1 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_DvcNumCmdLineFlag) )
	{
		m_DvcNum = atoi(pParam);
		if ( m_DvcNum < 1 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_TxModeCmdLineFlag) )
	{
		// Which tx-mode
		if ( 0 == strcmp(pParam, "188")  )
			m_TxMode = DTAPI_TXMODE_188;
		else if ( 0 == strcmp(pParam, "ADD16")  )
			m_TxMode = DTAPI_TXMODE_ADD16;
		else if ( 0 == strcmp(pParam, "130")  )
			m_TxMode = DTAPI_TXMODE_130;
		else if ( 0 == strcmp(pParam, "192")  )
			m_TxMode = DTAPI_TXMODE_192;
		else if ( 0 == strcmp(pParam, "204")  )
			m_TxMode = DTAPI_TXMODE_204;
		else if ( 0 == strcmp(pParam, "RAW")  )
			m_TxMode = DTAPI_TXMODE_RAW;
		else if ( 0 == strcmp(pParam, "MIN16")  )
			m_TxMode = DTAPI_TXMODE_MIN16;
		else if ( 0 == strcmp(pParam, "SDI8B_525")  )
			m_TxMode = DTAPI_TXMODE_SDI_FULL | DTAPI_TXMODE_SDI_525;
		else if ( 0 == strcmp(pParam, "SDI8B_625")  )
			m_TxMode = DTAPI_TXMODE_SDI_FULL | DTAPI_TXMODE_SDI_625;
		else if ( 0 == strcmp(pParam, "DTSDI") )
		{
			m_PlayDtSdiFile = true;
			m_TxMode = 0;
		}
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_TxRateCmdLineFlag) )
	{
		m_TxRate = atoi(pParam);
		if ( m_TxRate<0 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_DataPidCmdLineFlag) )
	{
		m_DataPid = atoi(pParam);
		if ( (m_DataPid < -1) || (m_DataPid > 8191) )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_ModTypeCmdLineFlag) )
	{
		// Which modulation mode
		if ( 0 == strcmp(pParam, "ATSC")  )
			m_ModType = DTAPI_MOD_ATSC;
		else if ( 0 == strcmp(pParam, "CMMB")  )
			m_ModType = DTAPI_MOD_CMMB;
		else if ( 0 == strcmp(pParam, "T2MI")  )
			m_ModType = DTAPI_MOD_T2MI;
		else if ( 0 == strcmp(pParam, "QPSK")  )
			m_ModType = DTAPI_MOD_QPSK;
		else if ( 0 == strcmp(pParam, "BPSK")  )
			m_ModType = DTAPI_MOD_BPSK;
		else if ( 0 == strcmp(pParam, "QAM4")  )
			m_ModType = DTAPI_MOD_QAM4;
		else if ( 0 == strcmp(pParam, "QAM16") )
			m_ModType = DTAPI_MOD_QAM16;
		else if ( 0 == strcmp(pParam, "QAM32") )
			m_ModType = DTAPI_MOD_QAM32;
		else if ( 0 == strcmp(pParam, "QAM64") )
			m_ModType = DTAPI_MOD_QAM64;
		else if ( 0 == strcmp(pParam, "QAM128") )
			m_ModType = DTAPI_MOD_QAM128;
		else if ( 0 == strcmp(pParam, "QAM256") )
			m_ModType = DTAPI_MOD_QAM256;
		else if ( 0 == strcmp(pParam, "DVBT") || 0 == strcmp(pParam, "OFDM") )
			m_ModType = DTAPI_MOD_DVBT;
		else if ( 0 == strcmp(pParam, "ISDBT") )
			m_ModType = DTAPI_MOD_ISDBT;
		else if ( 0 == strcmp(pParam, "DVBS2_QPSK") )
			m_ModType = DTAPI_MOD_DVBS2_QPSK;
		else if ( 0 == strcmp(pParam, "DVBS2_8PSK") )
			m_ModType = DTAPI_MOD_DVBS2_8PSK;
		else if ( 0 == strcmp(pParam, "DTMB") )
			m_ModType = DTAPI_MOD_DTMB;
		else if ( 0 == strcmp(pParam, "IQ") )
			m_ModType = DTAPI_MOD_IQDIRECT;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_CarrierFreqCmdLineFlag) )
	{
		m_CarrierFreq = atof(pParam);
		if ( m_CarrierFreq < 0.0 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_OutpLvlCmdLineFlag) )
	{
		m_OutpLevel = atof(pParam);
		if ( m_OutpLevel < -35.0 || m_OutpLevel > 0.0 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_SnrCmdLineFlag) )
	{
		m_Snr = atof(pParam);
		if ( m_Snr < 0.0 || m_Snr > 36.0 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_CodeRateCmdLineFlag) )
	{
		// Modulation type should be defined first
		if ( m_ModType == -1 )
            throw Exc(c_CleModTypeNotDefined, m_pLastFlag);

		// Which code rate

		// Check for DTMB code rates
		if ( m_ModType == DTAPI_MOD_DTMB )
		{
			if ( 0 == strcmp(pParam, "0.4")  )
				m_CodeRate = DTAPI_MOD_DTMB_0_4;
			else if ( 0 == strcmp(pParam, "0.6")  )
				m_CodeRate = DTAPI_MOD_DTMB_0_6;
			else if ( 0 == strcmp(pParam, "0.8")  )
				m_CodeRate = DTAPI_MOD_DTMB_0_8;
			else
				throw Exc(c_CleInvalidArgument, m_pLastFlag);
		}
		else
		{
			if ( 0 == strcmp(pParam, "1/2")  )
				m_CodeRate = DTAPI_MOD_1_2;
			else if ( 0 == strcmp(pParam, "2/3")  )
				m_CodeRate = DTAPI_MOD_2_3;
			else if ( 0 == strcmp(pParam, "3/4")  )
				m_CodeRate = DTAPI_MOD_3_4;
			else if ( 0 == strcmp(pParam, "4/5")  )
				m_CodeRate = DTAPI_MOD_4_5;
			else if ( 0 == strcmp(pParam, "5/6")  )
				m_CodeRate = DTAPI_MOD_5_6;
			else if ( 0 == strcmp(pParam, "6/7")  )
				m_CodeRate = DTAPI_MOD_6_7;
			else if ( 0 == strcmp(pParam, "7/8")  )
				m_CodeRate = DTAPI_MOD_7_8;
			else if ( 0 == strcmp(pParam, "1/3")  )
				m_CodeRate = DTAPI_MOD_1_3;
			else if ( 0 == strcmp(pParam, "2/5")  )
				m_CodeRate = DTAPI_MOD_2_5;
			else if ( 0 == strcmp(pParam, "3/5")  )
				m_CodeRate = DTAPI_MOD_3_5;
			else if ( 0 == strcmp(pParam, "8/9")  )
				m_CodeRate = DTAPI_MOD_8_9;
			else if ( 0 == strcmp(pParam, "9/10")  )
				m_CodeRate = DTAPI_MOD_9_10;
			else
				throw Exc(c_CleInvalidArgument, m_pLastFlag);
		}
	}
	else if ( 0==strcmp(m_pLastFlag, c_QamJ83AnnexFlag) )
	{
		// Modulation type should be defined first
		if ( m_ModType == -1 )
            throw Exc(c_CleModTypeNotDefined, m_pLastFlag);

		if ( 0 == strcmp(pParam, "A")  )
			m_QamJ83Annex = DTAPI_MOD_J83_A;
		else if ( 0 == strcmp(pParam, "B") )
			m_QamJ83Annex = DTAPI_MOD_J83_B;
		else if ( 0 == strcmp(pParam, "C") )
			m_QamJ83Annex = DTAPI_MOD_J83_C;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_OfdmConstellCmdLineFlag) )
	{
		// Modulation type should be defined first
		if ( m_ModType == -1 )
            throw Exc(c_CleModTypeNotDefined, m_pLastFlag);

		// Check for DTMB, ATSC or OFDM constellation
		if ( m_ModType == DTAPI_MOD_ATSC )
		{
			if ( 0 == strcmp(pParam, "VSB8")  )
				m_Constellation = DTAPI_MOD_ATSC_VSB8;
			else if ( 0 == strcmp(pParam, "VSB16")  )
				m_Constellation = DTAPI_MOD_ATSC_VSB16;
			else
				throw Exc(c_CleInvalidArgument, m_pLastFlag);
		}
		else if ( m_ModType == DTAPI_MOD_DTMB )
		{
			if ( 0 == strcmp(pParam, "QAM4NR")  )
				m_Constellation = DTAPI_MOD_DTMB_QAM4NR;
			else if ( 0 == strcmp(pParam, "QAM4")  )
				m_Constellation = DTAPI_MOD_DTMB_QAM4;
			else if ( 0 == strcmp(pParam, "QAM16")  )
				m_Constellation = DTAPI_MOD_DTMB_QAM16;
			else if ( 0 == strcmp(pParam, "QAM32")  )
				m_Constellation = DTAPI_MOD_DTMB_QAM32;
			else if ( 0 == strcmp(pParam, "QAM64")  )
				m_Constellation = DTAPI_MOD_DTMB_QAM64;
			else
				throw Exc(c_CleInvalidArgument, m_pLastFlag);
		}
		else
		{
			if ( 0 == strcmp(pParam, "QPSK")  )
				m_Constellation = DTAPI_MOD_DVBT_QPSK;
			else if ( 0 == strcmp(pParam, "QAM16") )
				m_Constellation = DTAPI_MOD_DVBT_QAM16;
			else if ( 0 == strcmp(pParam, "QAM64") )
				m_Constellation = DTAPI_MOD_DVBT_QAM64;
			else
				throw Exc(c_CleInvalidArgument, m_pLastFlag);
		}
	}
	else if ( 0==strcmp(m_pLastFlag, c_OfdmBandwidthCmdLineFlag) )
	{
		// Modulation type should be defined first
		if ( m_ModType == -1 )
            throw Exc(c_CleModTypeNotDefined, m_pLastFlag);

	
		if ( 0 == strcmp(pParam, "5") ) {
			if ( m_ModType == DTAPI_MOD_DVBT )
				m_Bandwidth = DTAPI_MOD_DVBT_5MHZ;
			else if ( m_ModType == DTAPI_MOD_DTMB )
				m_Bandwidth = DTAPI_MOD_DTMB_5MHZ;
		}
		else if ( 0 == strcmp(pParam, "6") ) {
			if ( m_ModType == DTAPI_MOD_DVBT )
				m_Bandwidth = DTAPI_MOD_DVBT_6MHZ;
			else if ( m_ModType == DTAPI_MOD_DTMB )
				m_Bandwidth = DTAPI_MOD_DTMB_6MHZ;
		}
		else if ( 0 == strcmp(pParam, "7") ) {
			if ( m_ModType == DTAPI_MOD_DVBT )
				m_Bandwidth = DTAPI_MOD_DVBT_7MHZ;
			else if ( m_ModType == DTAPI_MOD_DTMB)
				m_Bandwidth = DTAPI_MOD_DTMB_7MHZ;
		}
		else if ( 0 == strcmp(pParam, "8") ) {
			if ( m_ModType == DTAPI_MOD_DVBT )
				m_Bandwidth = DTAPI_MOD_DVBT_8MHZ;
			else if ( m_ModType == DTAPI_MOD_DTMB )
				m_Bandwidth = DTAPI_MOD_DTMB_8MHZ;
		}
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);

	}
	else if ( 0==strcmp(m_pLastFlag, c_OfdmTxModeCmdLineFlag) )
	{
		// Modulation type should be defined first
		if ( m_ModType == -1 )
            throw Exc(c_CleModTypeNotDefined, m_pLastFlag);

		if ( 0 == strcmp(pParam, "2k") )
			m_OfdmTxMode = DTAPI_MOD_DVBT_2K;
		else if ( 0 == strcmp(pParam, "4k") )
			m_OfdmTxMode = DTAPI_MOD_DVBT_4K;
		else if ( 0 == strcmp(pParam, "8k") )
			m_OfdmTxMode = DTAPI_MOD_DVBT_8K;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_OfdmGaurdItvCmdLineFlag) )
	{
		// Modulation type should be defined first
		if ( m_ModType == -1 )
            throw Exc(c_CleModTypeNotDefined, m_pLastFlag);

		if ( 0 == strcmp(pParam, "1/4") )
			m_OfdmGuardItv = DTAPI_MOD_DVBT_G_1_4;
		else if ( 0 == strcmp(pParam, "1/8") )
			m_OfdmGuardItv = DTAPI_MOD_DVBT_G_1_8;
		else if ( 0 == strcmp(pParam, "1/16") )
			m_OfdmGuardItv = DTAPI_MOD_DVBT_G_1_16;
		else if ( 0 == strcmp(pParam, "1/32") )
			m_OfdmGuardItv = DTAPI_MOD_DVBT_G_1_32;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_DvbS2EnablePilotCmdLineFlag) )
	{
		// Modulation type should be defined first
		if ( m_ModType == -1 )
            throw Exc(c_CleModTypeNotDefined, m_pLastFlag);

		if ( 0 == strcmp(pParam, "OFF") )
			m_DvbS2Pilots = DTAPI_MOD_S2_NOPILOTS;
		else if ( 0 == strcmp(pParam, "ON") )
			m_DvbS2Pilots = DTAPI_MOD_S2_PILOTS;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_DvbS2FecFrameLengthCmdLineFlag) )
	{
		// Modulation type should be defined first
		if ( m_ModType == -1 )
            throw Exc(c_CleModTypeNotDefined, m_pLastFlag);

		if ( 0 == strcmp(pParam, "SHORT") )
			m_DvbS2FecFrameLength = DTAPI_MOD_S2_SHORTFRM;
		else if ( 0 == strcmp(pParam, "LONG") )
			m_DvbS2FecFrameLength = DTAPI_MOD_S2_LONGFRM;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_DvbS2GoldSequenceInitValFlag) )
	{
		m_DvbS2GoldSeqInit = atoi(pParam);
		if ( m_DvbS2GoldSeqInit<0 || m_DvbS2GoldSeqInit>262143 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_DtmbFrameHdrModeCmdLineFlag) )
	{
		// Modulation type should be defined first
		if ( m_ModType == -1 )
            throw Exc(c_CleModTypeNotDefined, m_pLastFlag);

		if ( 0 == strcmp(pParam, "PN420")  )
			m_DtmbFrameHdrMode = DTAPI_MOD_DTMB_PN420;
		else if ( 0 == strcmp(pParam, "PN595")  )
			m_DtmbFrameHdrMode = DTAPI_MOD_DTMB_PN595;
		else if ( 0 == strcmp(pParam, "PN945")  )
			m_DtmbFrameHdrMode = DTAPI_MOD_DTMB_PN945;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if (0==strcmp(pParam, c_IqInterpFilterFlag))
	{
		if ( 0 == strcmp(pParam, "OFDM")  )
			m_IqInterpFilter = DTAPI_MOD_INTERPOL_OFDM;			
		else if ( 0 == strcmp(pParam, "QAM")  )
			m_IqInterpFilter = DTAPI_MOD_INTERPOL_QAM;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_IpAddressCmdLineFlag) )
	{
		// Look for optional IP port, located after the semicolon
		char*  p2 = strstr(pParam, ":");
		if ( p2 != NULL )
		{
			int IpPort = atoi((p2+1));
			*p2 = '\0';	// Insert NULL charecter at semicolon

			if ( IpPort<0 || IpPort>65535 )
				throw Exc(c_CleInvalidArgument, m_pLastFlag);

			m_IpPars.m_Port = (unsigned short)IpPort;
		}
		// Store IP address
		DTAPI_RESULT dr = ::DtapiInitDtTsIpParsFromIpString(m_IpPars, pParam, NULL);
		if ( dr != DTAPI_OK )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_IpProtocolCmdLineFlag) )
	{
		if ( 0 == strcmp(pParam, "UDP")  )
			m_IpPars.m_Protocol = DTAPI_PROTO_UDP;			
		else if ( 0 == strcmp(pParam, "RTP")  )
			m_IpPars.m_Protocol = DTAPI_PROTO_RTP;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_IpNumTpPerIpCmdLineFlag) )
	{
		m_IpPars.m_NumTpPerIp = atoi(pParam);
		if ( m_IpPars.m_NumTpPerIp<1 || m_IpPars.m_NumTpPerIp>7 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_IpTtlCmdLineFlag) )
	{
		m_IpPars.m_TimeToLive = atoi(pParam);
		if (m_IpPars.m_TimeToLive < 1 || m_IpPars.m_TimeToLive > 65535) 
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_StuffingModeCmdLineFlag) )
	{
	
		if ( 0 == strcmp(pParam, "OFF") )
			m_Stuffing = DTAPI_TXSTUFF_MODE_OFF;
		else if ( 0 == strcmp(pParam, "ON") )
			m_Stuffing = DTAPI_TXSTUFF_MODE_ON;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else
		throw Exc(c_CleUnknownOption, m_pLastFlag);

	// Clear required argument flag
	m_LastFlagReqsArg = false;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::TxMode2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
const char* CommandLineParams::TxMode2Str() const
{
	if ( m_PlayDtSdiFile )
	{
		static char sz[256];
		strcpy(sz, "DTSDI (");

		// Full-frame or active video only?
		if ( ((m_TxMode & DTAPI_TXMODE_MASK) | DTAPI_TXMODE_SDI) == DTAPI_TXMODE_SDI_FULL )
			strcat(sz, "FULL");
		else
			strcat(sz, "ACTVID");
		// 625 or 525?
		if ( 0!=(m_TxMode & DTAPI_TXMODE_SDI_625) )
			strcat(sz, ", 625");
		else
			strcat(sz, ", 525");
		// 10bit or 8bit?
		if ( 0!=(m_TxMode & DTAPI_TXMODE_SDI_10B) )
			strcat(sz, ", 10B");
		else
			strcat(sz, ", 8B");
		// Huffman compressed?
		if ( 0!=(m_TxMode & DTAPI_TXMODE_SDI_HUFFMAN) )
			strcat(sz, ", HUFFMAN");

		if (m_Stuffing == DTAPI_TXSTUFF_MODE_ON)
			strcat(sz, ", STUFFING)");
		else
			strcat(sz, ")");

		return sz;
	}
	else
	{
		if ( m_TxMode & DTAPI_TXMODE_SDI )
			if ( ( m_TxMode & DTAPI_TXMODE_SDI_625 )!=0 )
				return "SDI8B_625";
			else
				return "SDI8B_525";		
		else
		{
			switch( m_TxMode & DTAPI_TXMODE_MASK )
			{
			case DTAPI_TXMODE_188:		return "188";
			case DTAPI_TXMODE_ADD16:	return "ADD16";
			case DTAPI_TXMODE_130:		return "130";
			case DTAPI_TXMODE_192:		return "192";
			case DTAPI_TXMODE_204:		return "204";
			case DTAPI_TXMODE_RAW:		return "RAW";
			case DTAPI_TXMODE_MIN16:	return "MIN16";
			default:					return "?";
			}
		}
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::ModType2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
const char* CommandLineParams::ModType2Str() const
{
	switch( m_ModType )
	{
	case DTAPI_MOD_ATSC:		return "ATSC";
	case DTAPI_MOD_BPSK:		return "BPSK";
	case DTAPI_MOD_CMMB:		return "CMMB";
	case DTAPI_MOD_DTMB:		return "DTMB";
	case DTAPI_MOD_DVBS2_QPSK:	return "QPSK (DVBS2)";
	case DTAPI_MOD_DVBS2_8PSK:	return "8PSK (DVBS2)";
	case DTAPI_MOD_DVBT:		return "DVB-T";
	case DTAPI_MOD_ISDBT:		return "ISDB-T";
	case DTAPI_MOD_QAM4:		return "QAM4";
	case DTAPI_MOD_QAM16:		return "QAM16";
	case DTAPI_MOD_QAM32:		return "QAM32";
	case DTAPI_MOD_QAM64:		return "QAM64";
	case DTAPI_MOD_QAM128:		return "QAM128";
	case DTAPI_MOD_QAM256:		return "QAM256";
	case DTAPI_MOD_QPSK:		return "QPSK";
	case DTAPI_MOD_T2MI:		return "T2MI";
	case DTAPI_MOD_IQDIRECT:	return "IQ";
	default:					return "?";
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::CodeRate2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
const char* CommandLineParams::CodeRate2Str() const
{
	if ( m_ModType == DTAPI_MOD_DTMB )
	{
		switch( m_CodeRate )
		{
		case DTAPI_MOD_DTMB_0_4:	return "0.4";
		case DTAPI_MOD_DTMB_0_6:	return "0.6";
		case DTAPI_MOD_DTMB_0_8:	return "0.8";
		default:					return "?";
		}
	}
	else
	{
		switch( m_CodeRate )
		{
		case DTAPI_MOD_1_2:		return "1/2";
		case DTAPI_MOD_2_3:		return "2/3";
		case DTAPI_MOD_3_4:		return "3/4";
		case DTAPI_MOD_4_5:		return "4/5";
		case DTAPI_MOD_5_6:		return "5/6";
		case DTAPI_MOD_6_7:		return "6/7";
		case DTAPI_MOD_7_8:		return "7/8";
		case DTAPI_MOD_1_3:		return "1/3";
		case DTAPI_MOD_2_5:		return "2/5";
		case DTAPI_MOD_3_5:		return "3/5";
		case DTAPI_MOD_8_9:		return "8/9";
		case DTAPI_MOD_9_10:	return "9/10";
		default:				return "?";
		}
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::QamJ83Annex2Str -.-.-.-.-.-.-.-.-.-.-.-.-.
//
const char* CommandLineParams::QamJ83Annex2Str() const
{
	switch( m_QamJ83Annex )
	{
	case DTAPI_MOD_J83_A:		return "J.83 Annex A";
	case DTAPI_MOD_J83_B:		return "J.83 Annex B";
	case DTAPI_MOD_J83_C:		return "J.83 Annex C";
	default:					return "?";
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::OfdmBandwidth2Str -.-.-.-.-.-.-.-.-.-.-.-.-
//
const char* CommandLineParams::OfdmBandwidth2Str() const
{

	if ( m_ModType == DTAPI_MOD_DTMB )
	{
		switch( m_Bandwidth )
		{
		case DTAPI_MOD_DTMB_5MHZ:	return "5MHz";
		case DTAPI_MOD_DTMB_6MHZ:	return "6MHz";
		case DTAPI_MOD_DTMB_7MHZ:	return "7MHz";
		case DTAPI_MOD_DTMB_8MHZ:	return "8MHz";
		default:					return "?";
		}

	} 
	else
	{
		switch( m_Bandwidth )
		{
		case DTAPI_MOD_DVBT_5MHZ:	return "5MHz";
		case DTAPI_MOD_DVBT_6MHZ:	return "6MHz";
		case DTAPI_MOD_DVBT_7MHZ:	return "7MHz";
		case DTAPI_MOD_DVBT_8MHZ:	return "8MHz";
		default:					return "?";
		}
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::Constellation2Str -.-.-.-.-.-.-.-.-.-.-.-.-
//
const char* CommandLineParams::Constellation2Str() const
{
	if ( m_ModType == DTAPI_MOD_ATSC )
	{
		switch( m_Constellation )
		{
		case DTAPI_MOD_ATSC_VSB8:	return "VSB8";
		case DTAPI_MOD_ATSC_VSB16:	return "VSB16";
		default:					return "?";
		}

	}
	else if ( m_ModType == DTAPI_MOD_DTMB )
	{
		switch( m_Constellation )
		{
		case DTAPI_MOD_DTMB_QAM4NR:	return "QAM4NR";
		case DTAPI_MOD_DTMB_QAM4:	return "QAM4";
		case DTAPI_MOD_DTMB_QAM16:	return "QAM16";
		case DTAPI_MOD_DTMB_QAM32:	return "QAM32";
		case DTAPI_MOD_DTMB_QAM64:	return "QAM64";
		default:					return "?";
		}
	}
	else
	{
		switch( m_Constellation )
		{
		case DTAPI_MOD_DVBT_QPSK:	return "QPSK";
		case DTAPI_MOD_DVBT_QAM16:	return "QAM16";
		case DTAPI_MOD_DVBT_QAM64:	return "QAM64";
		default:					return "?";
		}
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::OfdmGaurdItv2Str -.-.-.-.-.-.-.-.-.-.-.-.-.
//
const char* CommandLineParams::OfdmGaurdItv2Str() const
{
	switch( m_OfdmGuardItv )
	{
	case DTAPI_MOD_DVBT_G_1_4:	return "1/4";
	case DTAPI_MOD_DVBT_G_1_8:	return "1/8";
	case DTAPI_MOD_DVBT_G_1_16:	return "1/16";
	case DTAPI_MOD_DVBT_G_1_32:	return "1/32";
	default:					return "?";
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::OfdmTxMode2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
const char* CommandLineParams::OfdmTxMode2Str() const
{
	switch( m_OfdmTxMode )
	{
	case DTAPI_MOD_DVBT_2K:		return "2k";
	case DTAPI_MOD_DVBT_4K:		return "4k";
	case DTAPI_MOD_DVBT_8K:		return "8k";
	default:					return "?";
	}
}


//.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::DvbS2Pilots2Str -.-.-.-.-.-.-.-.-.-.-.-.-.
//
const char* CommandLineParams::DvbS2Pilots2Str() const
{
	switch ( m_DvbS2Pilots )
	{
	case DTAPI_MOD_S2_NOPILOTS:	return "OFF";
	case DTAPI_MOD_S2_PILOTS:	return "ON";
	default:					return "?";
	}
}

//.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::DvbS2FecFrameLength2Str -.-.-.-.-.-.-.-.-.-.-.
//
const char* CommandLineParams::DvbS2FecFrameLength2Str() const
{
	switch ( m_DvbS2FecFrameLength )
	{
	case DTAPI_MOD_S2_SHORTFRM:	return "SHORT";
	case DTAPI_MOD_S2_LONGFRM:	return "LONG";
	default:					return "?";
	}
}

//-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::DtmbFrameHdrMode2Str -.-.-.-.-.-.-.-.-.-.-.-.
//
const char* CommandLineParams::DtmbFrameHdrMode2Str() const
{
	switch( m_DtmbFrameHdrMode )
	{
	case DTAPI_MOD_DTMB_PN420:	return "PN420";
	case DTAPI_MOD_DTMB_PN595:	return "PN595";
	case DTAPI_MOD_DTMB_PN945:	return "PN945";
	default:					return "?";
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::IqInterpFilter2Str -.-.-.-.-.-.-.-.-.-.-.-.-
//
const char* CommandLineParams::IqInterpFilter2Str() const
{
	switch ( m_IqInterpFilter )
	{
	case DTAPI_MOD_INTERPOL_OFDM: return "OFDM";
	case DTAPI_MOD_INTERPOL_QAM:  return "QAM";
	default:					  return "?";
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::IpProtocol2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
const char* CommandLineParams::IpProtocol2Str() const
{
	switch ( m_IpPars.m_Protocol )
	{
	case DTAPI_PROTO_UDP:		return "UDP";
	case DTAPI_PROTO_RTP:		return "RTP";
	default:					return "?";
	}
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Player implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player::Player -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Player::Player() : m_pFile(NULL), m_pBuf(NULL)
{
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player::~Player -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Player::~Player()
{
	// Detach from channel and device
	m_DtOutp.SetTxControl(DTAPI_TXCTRL_IDLE);
	m_DtOutp.Detach(DTAPI_INSTANT_DETACH);
	m_DtDvc.Detach();

	// Free our buffer
	if ( m_pBuf != NULL )
		delete [] m_pBuf;

	// donot forget to close our file
	if ( m_pFile != NULL )
		::fclose(m_pFile);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player::AttachToOutput -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void Player::AttachToOutput()
{
	DTAPI_RESULT dr;
	int n, NumHwFuncsFound(0);
	DtHwFuncDesc HwFuncs[20];

	// Scan for available DekTec devices
	dr = ::DtapiHwFuncScan(sizeof(HwFuncs)/sizeof(HwFuncs[0]), NumHwFuncsFound, HwFuncs);
	if ( dr != DTAPI_OK )
		throw Exc(c_ErrDtapiHwScanFailed, ::DtapiResult2Str(dr) );

	// Loop through found hardware functions
	bool Found=false, SetIoConfig=false;
	int DeviceNum(0);
	DtHwFuncDesc* pHwf = &HwFuncs[0];
	for ( n=0; n<NumHwFuncsFound; n++, pHwf++ )
	{
		// Skip non-bidir capabale inputs
		if (   0!=(pHwf->m_ChanType & DTAPI_CHAN_INPUT)
			&& 0==(pHwf->m_ChanType & DTAPI_CHAN_OUTPUT)
			&& (pHwf->m_Flags & DTAPI_CAP_BIDIR)==0 )
			continue;

		// Looking for a specific type??
		if ( m_CmdLineParams.m_DvcType!=-1 && m_CmdLineParams.m_Port!=-1 )
		{
			if (   pHwf->m_DvcDesc.m_TypeNumber==m_CmdLineParams.m_DvcType
				&& pHwf->m_Port==m_CmdLineParams.m_Port )
			{
				DeviceNum++;	// only count devices matching our search type
			}
		}
		else if ( pHwf->m_DvcDesc.m_TypeNumber==m_CmdLineParams.m_DvcType )
			DeviceNum++;	// only count devices matching our search type
		else if ( m_CmdLineParams.m_DvcType==-1 )
			DeviceNum++;	// count every device
		
		if ( DeviceNum == m_CmdLineParams.m_DvcNum ) {
			Found = true;

			// Attempt a set IO-config if the IO-port is BIDIR-capable and it
			// is not already an output
			if ( 0==(pHwf->m_ChanType & DTAPI_CHAN_OUTPUT) )
				SetIoConfig = true;
			break;
		}
	}
	// Did we find our device?
	if ( !Found && DeviceNum==0 )
	{
		// Could not find any matching output at all
		if (   (m_CmdLineParams.m_DvcType>=100 && m_CmdLineParams.m_DvcType<200)
			|| (m_CmdLineParams.m_DvcType>=2100 && m_CmdLineParams.m_DvcType<2200) )
			throw Exc(c_ErrNoDta1xxFound, m_CmdLineParams.m_DvcType);
		else if (   (m_CmdLineParams.m_DvcType>=200 && m_CmdLineParams.m_DvcType<300) )
			throw Exc(c_ErrNoDtu2xxFound, m_CmdLineParams.m_DvcType);
		else
			throw Exc(c_ErrNoOutputFound);
	}
	else if ( !Found && m_CmdLineParams.m_DvcNum > DeviceNum )
	{
		// Could no find the Nth matching output
		if (   (m_CmdLineParams.m_DvcType>=100 && m_CmdLineParams.m_DvcType<200)
			|| (m_CmdLineParams.m_DvcType>=2100 && m_CmdLineParams.m_DvcType<2200) )
			throw Exc(c_ErrCouldNotFindDta1xxN, m_CmdLineParams.m_DvcType, m_CmdLineParams.m_DvcNum);
		else if (   (m_CmdLineParams.m_DvcType>=200 && m_CmdLineParams.m_DvcType<300) )
			throw Exc(c_ErrCouldNotFindDtu2xxN, m_CmdLineParams.m_DvcType, m_CmdLineParams.m_DvcNum);
		else
			throw Exc(c_ErrCouldNotFindOutputN, m_CmdLineParams.m_DvcNum);
	}

	// Attach to the device
	dr = m_DtDvc.AttachToSerial(pHwf->m_DvcDesc.m_Serial);
	if ( dr == DTAPI_E_DRIVER_INCOMP )
	{
		// Special case: driver version is not compatible with this version of DtPlay,
		// get the driver version and throw an exception
		int DriverVersionMajor(-1), DriverVersionMinor(-1), DriverVersionBugFix(-1),
			DriverVersionBuild(-1);

		// Get device driver version
		::DtapiGetDeviceDriverVersion(pHwf->m_DvcDesc.m_Category,
									  DriverVersionMajor, DriverVersionMinor,
									  DriverVersionBugFix, DriverVersionBuild);

		// PCI or USB device??
		if ( pHwf->m_DvcDesc.m_Category == DTAPI_CAT_PCI )
		{
			throw Exc( c_ErrDriverInCompPci, DriverVersionMajor, DriverVersionMinor,
					   DriverVersionBugFix, DriverVersionBuild );
		}
		else if ( pHwf->m_DvcDesc.m_Category == DTAPI_CAT_USB )
		{
			throw Exc( c_ErrDriverInCompUsb, DriverVersionMajor, DriverVersionMinor,
					   DriverVersionBugFix, DriverVersionBuild );
		}
	}
	else if ( dr != DTAPI_OK )
	{
		// PCI or USB device??
		if ( pHwf->m_DvcDesc.m_Category == DTAPI_CAT_PCI )
			throw Exc(c_ErrFailToAttachToDta1xx, pHwf->m_DvcDesc.m_TypeNumber,
					  pHwf->m_DvcDesc.m_PciBusNumber, pHwf->m_DvcDesc.m_SlotNumber);
		else
			throw Exc(c_ErrFailToAttachToDtu2xx, pHwf->m_DvcDesc.m_TypeNumber);
	}

	// Set the IO-config to output?
	if ( SetIoConfig )
	{
			dr = m_DtDvc.SetIoConfig(pHwf->m_Port, DTAPI_IOCONFIG_OUTPUT);
			if ( dr != DTAPI_OK )
				throw Exc(c_ErrFailToSetIoConfig, ::DtapiResult2Str(dr));
	}

	if ( m_CmdLineParams.m_DblBuff ) 
	{
		dr = m_DtDvc.SetIoConfig((pHwf->m_Port+1), DTAPI_IOCONFIG_DBLBUF);
		if ( dr != DTAPI_OK )
			LogF ("Configuring doubly buffered mode for port %d failed (%s)",pHwf->m_Port+1,DtapiResult2Str(dr));
	}

	
	// Attach to the output channel
	dr = m_DtOutp.AttachToPort(&m_DtDvc, pHwf->m_Port);
	if ( dr != DTAPI_OK )
		throw Exc(c_ErrFailToAttachToChan, ::DtapiResult2Str(dr));

	// Retrieve and store current IOCONFIG
	dr = m_DtDvc.GetIoConfig(pHwf->m_Port, m_IoConfig, m_ParXtra);
	if ( dr != DTAPI_OK )
		throw Exc(c_ErrFailToGetIoConfig, ::DtapiResult2Str(dr));

	// Check for modulator card (assume no modulator)
	m_Modulator = (m_DtOutp.m_HwFuncDesc.m_StreamType == DTAPI_TS_MOD);
	// Check for IP output
	m_Ip = (m_DtOutp.m_HwFuncDesc.m_StreamType == DTAPI_TS_OVER_IP);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player::AutoDetectSdiFormat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void Player::AutoDetectSdiFormat()
{
	// Load file header from file
	DtSdiFileHdr FileHdr;
	int NumBytesRead = (int)fread(&FileHdr, 1, sizeof(FileHdr), m_pFile);
	// Sanity check
	if ( NumBytesRead != sizeof(FileHdr) )
		throw Exc(c_ErrInvalidFileSize);

	// Check if we have a valid header
	if (   FileHdr.m_MagicCode1!=DTSDI_MAGIC_CODE1 || FileHdr.m_MagicCode2!=DTSDI_MAGIC_CODE2
		|| FileHdr.m_MagicCode3!=DTSDI_MAGIC_CODE3 || FileHdr.m_FmtVersion!=DTSDI_FMT_VERSION )
		throw Exc(c_ErrInvalidDtSdiFileHdr);

	// Init tx-mode from settings in header
	m_CmdLineParams.m_TxMode = 0;
	if ( (FileHdr.m_Flags & 0x3) == DTSDI_SDI_FULL )
		m_CmdLineParams.m_TxMode = DTAPI_TXMODE_SDI_FULL;
	else
		m_CmdLineParams.m_TxMode = DTAPI_TXMODE_SDI_ACTVID;

	if ( 0!=(FileHdr.m_Flags & DTSDI_SDI_10B) )
		m_CmdLineParams.m_TxMode |= DTAPI_TXMODE_SDI_10B;
	if ( 0!=(FileHdr.m_Flags & DTSDI_SDI_HUFFMAN) )
		m_CmdLineParams.m_TxMode |= DTAPI_TXMODE_SDI_HUFFMAN;
	if ( FileHdr.m_DataType == DTSDI_TYPE_SDI_625 )
		m_CmdLineParams.m_TxMode |= DTAPI_TXMODE_SDI_625;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player::DisplayPlayInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void Player::DisplayPlayInfo()
{
	char sz[256];

	LogF("");
	LogF("Start playing");
	LogF("- Play file name        : %s", m_CmdLineParams.m_FileName);
	LogF("- Loop file             : %dx", m_CmdLineParams.m_LoopCnt);

	// Skip bit-rate for DVB-H/DVB-T/ISDBT/IQ/CMMB/DTMB modulator and in case of SDI
	if ( !(   ( m_Modulator
				&& (   m_CmdLineParams.m_ModType==DTAPI_MOD_DVBT
					|| m_CmdLineParams.m_ModType==DTAPI_MOD_CMMB
					|| m_CmdLineParams.m_ModType==DTAPI_MOD_ISDBT
					|| m_CmdLineParams.m_ModType==DTAPI_MOD_IQDIRECT
					|| m_CmdLineParams.m_ModType==DTAPI_MOD_DTMB) )
           || ( !m_Modulator && 0!=(m_CmdLineParams.m_TxMode&DTAPI_TXMODE_SDI)) ) )
	{
        if ( m_CmdLineParams.m_TxRate==0 )
		    LogF("- Transport-Stream rate : TX-on-time", m_CmdLineParams.m_TxRate);
        else
            LogF("- Transport-Stream rate : %d bps", m_CmdLineParams.m_TxRate);
	}
	else if ( m_Modulator && m_CmdLineParams.m_ModType==DTAPI_MOD_IQDIRECT )
	{
		LogF("- Sample rate           : %d Hz", m_CmdLineParams.m_TxRate);
	}
	LogF("- Transmit Mode         : %s", m_CmdLineParams.TxMode2Str() );
	
	if ( m_DtDvc.Category() == DTAPI_CAT_USB )
		sprintf(sz, "DTU-%d", m_DtDvc.TypeNumber() );
	else /*if ( m_DtDvc.Category() == DTAPI_CAT_PCI )*/
		sprintf(sz, "DTA-%d", m_DtDvc.TypeNumber() );
	
	if ( m_CmdLineParams.m_Port != -1 )
	{
		LogF("- Output device         : %s port %d (#%d)", sz, m_CmdLineParams.m_Port,
			 m_CmdLineParams.m_DvcNum );
	}
	else
		LogF("- Output device         : %s (#%d)", sz, m_CmdLineParams.m_DvcNum );

	if (m_IoConfig == DTAPI_IOCONFIG_GENLOCKED)
		LogF("- Genlock reference     : %s port %d (#%d)", sz, m_ParXtra == -1 ? 1 : m_ParXtra,
			 m_CmdLineParams.m_DvcNum );
		

	// Do we have a modulator
	if ( m_Modulator )
	{
		LogF("- Modulation Type       : %s", m_CmdLineParams.ModType2Str() );
		LogF("- Carrier Frequency     : %.2f MHz", m_CmdLineParams.m_CarrierFreq );
		// Show output level (if supported)
		if ( (m_DtOutp.m_HwFuncDesc.m_Flags & DTAPI_CAP_ADJLVL)!=0 )
			LogF("- Output Level          : %.1f dBm", m_CmdLineParams.m_OutpLevel );

		// Show SNR (if supported)
		if ( (m_DtOutp.m_HwFuncDesc.m_Flags & DTAPI_CAP_SNR)!=0 )
		{
			if ( m_CmdLineParams.m_Snr >= 0.0 )
				LogF("- SNR                   : %.1f dB", m_CmdLineParams.m_Snr );
			else
				LogF("- SNR                   : no noise generation" );
		}

		// Code rate: only for DVB-H/DVB-T, DTMB, QPSK and BPSK
		if (   m_CmdLineParams.m_ModType==DTAPI_MOD_DVBT
			|| m_CmdLineParams.m_ModType==DTAPI_MOD_QPSK
			|| m_CmdLineParams.m_ModType==DTAPI_MOD_BPSK
			|| m_CmdLineParams.m_ModType==DTAPI_MOD_DVBS2_QPSK
			|| m_CmdLineParams.m_ModType==DTAPI_MOD_DVBS2_8PSK
			|| m_CmdLineParams.m_ModType==DTAPI_MOD_DTMB
			)
			LogF("- Convolutional Rate    : %s", m_CmdLineParams.CodeRate2Str() );

		// ATSC modulation parameters
		if ( m_CmdLineParams.m_ModType==DTAPI_MOD_ATSC )
		{
			LogF("- Constellation         : %s", m_CmdLineParams.Constellation2Str() );
		}
		// DVB-H/DVB-T modulation parameters
		else if ( m_CmdLineParams.m_ModType==DTAPI_MOD_DVBT )
		{
			LogF("- Bandwitdh             : %s", m_CmdLineParams.OfdmBandwidth2Str() );
			LogF("- Transmission-mode     : %s", m_CmdLineParams.OfdmTxMode2Str() );
			LogF("- Gaurd Interval        : %s", m_CmdLineParams.OfdmGaurdItv2Str() );
			LogF("- Constellation         : %s", m_CmdLineParams.Constellation2Str() );
		}
		// DVB-S2 modulation parameters
		else if (    m_CmdLineParams.m_ModType==DTAPI_MOD_DVBS2_QPSK
			      || m_CmdLineParams.m_ModType==DTAPI_MOD_DVBS2_8PSK )
		{
			LogF("- Pilots                : %s", m_CmdLineParams.DvbS2Pilots2Str() );
			LogF("- FEC Frame Length      : %s", m_CmdLineParams.DvbS2FecFrameLength2Str() );
			LogF("- Gold Seq. Init Value  : %d", m_CmdLineParams.m_DvbS2GoldSeqInit );
		}
		// DTMB modulation parameters
		else if ( m_CmdLineParams.m_ModType==DTAPI_MOD_DTMB )
		{
			LogF("- Bandwitdh             : %s", m_CmdLineParams.OfdmBandwidth2Str() );
			LogF("- Frame-Header-Mode     : %s", m_CmdLineParams.DtmbFrameHdrMode2Str() );
			LogF("- Constellation         : %s", m_CmdLineParams.Constellation2Str() );
		}
		// IQ modulation parameters
		else if ( m_CmdLineParams.m_ModType==DTAPI_MOD_IQDIRECT )
		{
			LogF("- Interpolation Filter  : %s", m_CmdLineParams.IqInterpFilter2Str() );
		}
		// QAM modulation parameters
		else if (    m_CmdLineParams.m_ModType!=DTAPI_MOD_DVBT
				  && m_CmdLineParams.m_ModType!=DTAPI_MOD_ISDBT
				  && m_CmdLineParams.m_ModType!=DTAPI_MOD_IQDIRECT
				  && m_CmdLineParams.m_ModType!=DTAPI_MOD_CMMB
				  && m_CmdLineParams.m_ModType!=DTAPI_MOD_T2MI
				  && m_CmdLineParams.m_ModType!=DTAPI_MOD_QPSK
			      && m_CmdLineParams.m_ModType!=DTAPI_MOD_BPSK )
		{
			LogF("- J.83                  : %s", m_CmdLineParams.QamJ83Annex2Str() );
		}
	}

	// Do we have a IP output
	if ( m_Ip )
	{
		// Log IP parameters
		DtapiIpAddr2Str( sz, sizeof(sz), m_CmdLineParams.m_IpPars.m_Ip );
		LogF("- IP Address            : %s:%d", sz, m_CmdLineParams.m_IpPars.m_Port );
		LogF("- Protocol              : %s", m_CmdLineParams.IpProtocol2Str() );
		LogF("- Num Tp per IP         : %d", m_CmdLineParams.m_IpPars.m_NumTpPerIp );
	}
	
#ifdef WIN32
	Log("");
	Log("Press any key to stop playing");
	Log("");
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player::InitIsdbtPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Player::InitIsdbtPars(DtIsdbtPars& IsdbtPars)
{
	DTAPI_RESULT dr;
	const int c_PckSize=204;
	const int c_TempBufSize = 1*1024*1024;
	char* pTempBuf = new char [c_TempBufSize];

	// Read in a portion of the file
	int NumBytes = (int)::fread(pTempBuf, 1, c_TempBufSize, m_pFile);
	// Reset file pointer
	::fseek(m_pFile, 0, SEEK_SET);

	// Look for synchronisation
	for ( int i=0; i<NumBytes-(c_PckSize*4); i++ )
	{
		// If we find four syncs in a row => we are in sync
		if (   pTempBuf[i]==0x47
			&& pTempBuf[i+(c_PckSize*1)]==0x47
			&& pTempBuf[i+(c_PckSize*2)]==0x47
			&& pTempBuf[i+(c_PckSize*3)]==0x47 )
		{
			// Set sync position
			pTempBuf += i; NumBytes -= i;
			break;
		}
	}
	
	// No muxing wanted, we have a 204 byte stream (with TMCC data) 
	IsdbtPars.m_DoMux = false;
	// Init structure by retrieveing paramters from TMCC information in packets
	dr = IsdbtPars.RetrieveParsFromTs(pTempBuf, NumBytes);
	// Free temp buffer
	delete [] pTempBuf;
	if ( dr != DTAPI_OK )
		throw Exc(c_FailedToInitIdsbtPars, dr);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player::InitOutput -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void Player::InitOutput()
{
	DTAPI_RESULT dr;

	// Init channel to initial 'safe' state
	dr = m_DtOutp.SetTxControl(DTAPI_TXCTRL_IDLE);	// Start in IDLE mode
	if ( dr != DTAPI_OK )
		throw Exc( c_ErrFailSetTxControl, ::DtapiResult2Str(dr) );

	// Set the transmission mode. NOTE: ISDB-T only supports 204 byte mode
    int  TxMode = m_CmdLineParams.m_TxMode;
    // Special case: if tx-rate is set to '0' we want to transmit on timestamp
    TxMode |= (m_CmdLineParams.m_TxRate==0) ? DTAPI_TXMODE_TIMESTAMP: 0;
    dr = m_DtOutp.SetTxMode( TxMode, m_CmdLineParams.m_Stuffing );
	if ( dr != DTAPI_OK )
		throw Exc( c_ErrFailSetTxMode, ::DtapiResult2Str(dr) );

	// Apply IP settings (if we have a IP output)
	if ( m_Ip )
	{
		dr = m_DtOutp.SetIpPars( &m_CmdLineParams.m_IpPars );
		if ( dr != DTAPI_OK )
			throw Exc( c_ErrFailSetIpPars, ::DtapiResult2Str(dr) );
	}

	// Apply modulation settings (if we have a modulator)
	if ( m_Modulator )
	{
		__int64 Freq = (__int64)(m_CmdLineParams.m_CarrierFreq*1E6);
		dr = m_DtOutp.SetRfControl( Freq );
		if ( dr != DTAPI_OK )
			throw Exc( c_ErrFailSetRfControl, ::DtapiResult2Str(dr) );

		// Set modulation control
		if ( m_CmdLineParams.m_ModType==DTAPI_MOD_ATSC )
		{
			dr = m_DtOutp.SetModControl( m_CmdLineParams.m_ModType,
										 m_CmdLineParams.m_Constellation,
										 32 /*32-taps*/, 0);
		}
		else if ( m_CmdLineParams.m_ModType==DTAPI_MOD_CMMB )
		{
			// Fill the CMMB pars
			DtCmmbPars CmmbPars;
			CmmbPars.m_AreaId = 0;
			CmmbPars.m_Bandwidth = 1;
			CmmbPars.m_TsPid = -1;
			CmmbPars.m_TxId = 0;

			// Read a chunk of data to retrieve the TS-rate
			FILE* pFile = fopen( m_CmdLineParams.m_FileName, "rb");
			if ( pFile == NULL )	
				throw Exc(c_ErrFailToOpenFile, m_CmdLineParams.m_FileName);

			// Read a chunk of data and retrieve the TS-rate
			char* pBuf = new char[4*1024*1024];
			int NumRead = (int)fread(pBuf, 1, 4*1024*1024, pFile);
			dr = CmmbPars.RetrieveTsRateFromTs(pBuf, NumRead );
			delete [] pBuf;
			fclose(pFile);
			if (dr != DTAPI_OK)
				throw Exc(c_ErrFailToOpenFile, m_CmdLineParams.m_FileName);

			// Set the CMMB modulation parameters
			dr = m_DtOutp.SetModControl( CmmbPars );
		}
		else if ( m_CmdLineParams.m_ModType==DTAPI_MOD_T2MI )
		{
			// Set the T2MI modulation parameters
			dr = m_DtOutp.SetModControl( m_CmdLineParams.m_ModType, 
										 m_CmdLineParams.m_TxRate, 
										 m_CmdLineParams.m_DataPid,
										 0);
		}
		else if ( m_CmdLineParams.m_ModType==DTAPI_MOD_DVBT )
		{
			int XtraPar1 =    m_CmdLineParams.m_Bandwidth
							| m_CmdLineParams.m_Constellation
							| m_CmdLineParams.m_OfdmTxMode
							| m_CmdLineParams.m_OfdmGuardItv
							| DTAPI_MOD_DVBT_NATIVE;

			dr = m_DtOutp.SetModControl( m_CmdLineParams.m_ModType,
										 m_CmdLineParams.m_CodeRate,
										 XtraPar1, 0);
		}
		else if ( m_CmdLineParams.m_ModType==DTAPI_MOD_ISDBT )
		{
			DtIsdbtPars IsdbtPars;
			// First init ISDB-T parameter structure
			InitIsdbtPars(IsdbtPars);
			dr = m_DtOutp.SetModControl( IsdbtPars );
		}
		else if (m_CmdLineParams.m_ModType==DTAPI_MOD_IQDIRECT )
		{
			// Set IQ direct mode
			dr = m_DtOutp.SetModControl(
						m_CmdLineParams.m_ModType,
						m_CmdLineParams.m_IqInterpFilter,	// Interpolation filter
						m_CmdLineParams.m_TxRate,			// Sample rate
						0);
		}
		else if (   m_CmdLineParams.m_ModType==DTAPI_MOD_QPSK
			     || m_CmdLineParams.m_ModType==DTAPI_MOD_BPSK )
		{
			dr = m_DtOutp.SetModControl( m_CmdLineParams.m_ModType,
										 m_CmdLineParams.m_CodeRate,
										 0, 0 );
		}
		else if (    m_CmdLineParams.m_ModType==DTAPI_MOD_DVBS2_QPSK
                  || m_CmdLineParams.m_ModType==DTAPI_MOD_DVBS2_8PSK )
		{
			int XtraPar1 =   m_CmdLineParams.m_DvbS2Pilots
						   | m_CmdLineParams.m_DvbS2FecFrameLength;
			int XtraPar2 =   m_CmdLineParams.m_DvbS2GoldSeqInit;

			dr = m_DtOutp.SetModControl( m_CmdLineParams.m_ModType,
										 m_CmdLineParams.m_CodeRate,
										 XtraPar1, XtraPar2);
			
		}
		else if ( m_CmdLineParams.m_ModType==DTAPI_MOD_DTMB )
		{
			int XtraPar0 =    m_CmdLineParams.m_Constellation
							| m_CmdLineParams.m_Bandwidth
							| m_CmdLineParams.m_DtmbFrameHdrMode
							| m_CmdLineParams.m_CodeRate
							| DTAPI_MOD_DTMB_IL_2
							| DTAPI_MOD_DTMB_USE_FRM_NO;

			dr = m_DtOutp.SetModControl( m_CmdLineParams.m_ModType,
										 XtraPar0,
										 0, 0);
		}
		else // For now, assume it must be QAM then
		{
			dr = m_DtOutp.SetModControl( m_CmdLineParams.m_ModType,
										 m_CmdLineParams.m_QamJ83Annex,
										 0, 0);
		}
		if ( dr != DTAPI_OK )
			throw Exc( c_ErrFailSetModControl, ::DtapiResult2Str(dr) );
	}

	// Set bit-rate for non-SDI and modulators other than CMMB/T2MI/ISDBT/IQ/DVB-H/DVB-T/DTMB
    if (    (  !m_Modulator && 0==(m_CmdLineParams.m_TxMode&DTAPI_TXMODE_SDI) )
         || (   m_Modulator && m_CmdLineParams.m_ModType!=DTAPI_MOD_DVBT
							&& m_CmdLineParams.m_ModType!=DTAPI_MOD_CMMB
							&& m_CmdLineParams.m_ModType!=DTAPI_MOD_T2MI
							&& m_CmdLineParams.m_ModType!=DTAPI_MOD_ISDBT
							&& m_CmdLineParams.m_ModType!=DTAPI_MOD_IQDIRECT
							&& m_CmdLineParams.m_ModType!=DTAPI_MOD_DTMB) )

	{
        // A rate of '0' is a special case (see code above)
        if ( m_CmdLineParams.m_TxRate > 0 )
        {
		    dr = m_DtOutp.SetTsRateBps( m_CmdLineParams.m_TxRate );
		    if ( dr != DTAPI_OK )
			    throw Exc( c_ErrFailSetTsRate, ::DtapiResult2Str(dr) );
        }
	}

	// Set output level of main output (if supported)
	if ( (m_DtOutp.m_HwFuncDesc.m_Flags & DTAPI_CAP_ADJLVL)!=0 )
	{
		// The SetOutputLevel method expects a level expressed in 0.1dBm units
		int LeveldBm = int(m_CmdLineParams.m_OutpLevel * 10.0);
		dr = m_DtOutp.SetOutputLevel(LeveldBm);
		if ( dr != DTAPI_OK )
			throw Exc( c_ErrFailedToSetOutputLevel, ::DtapiResult2Str(dr) );
	}

	// Set signal-to-noise ratio
	if ( m_Modulator && (m_DtOutp.m_HwFuncDesc.m_Flags & DTAPI_CAP_SNR)!=0 )
	{
		// Negative SNR means no noise generation
		if ( m_CmdLineParams.m_Snr < 0.0 )
			dr = m_DtOutp.SetSNR(DTAPI_NOISE_DISABLED, 0);
		else
		{
			// The SetSNR method expects a level expressed in 0.1dBm units
			int Snr = int(m_CmdLineParams.m_Snr * 10.0);
			dr = m_DtOutp.SetSNR(DTAPI_NOISE_WNG_HW, Snr);
		}
		if ( dr != DTAPI_OK )
			throw Exc( c_ErrFailedToSetSNR, ::DtapiResult2Str(dr) );
	}

	// Final initialisation
	dr = m_DtOutp.SetFifoSizeMax();		// Set FIFO size to maximum
	dr = m_DtOutp.ClearFifo();			// Clear FIFO (i.e. start with zero load)

	// Set exit load. 4k for all none modulator
	m_ExitLoad = m_Modulator ? 1*1024*1024 : 4*1024;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player::Log -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void Player::Log(const char* pMessage,  bool IgnoreSilence)
{
    // Check for silent mode
	if ( !IgnoreSilence && m_CmdLineParams.m_SilentMode )
		return;
	// Print message and add new line
	printf(pMessage); printf("\n");
}
// Overload with optional parameters
void Player::LogF(const char* pMessage, ... )
{
	char sz[512];

	va_list  ArgList;
	va_start(ArgList, pMessage);
	_vsnprintf(sz, sizeof(sz)-1, pMessage, ArgList);
	va_end(ArgList);

	Log(sz, false);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player::Play -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Player::LoopFile()
{
	bool EoF(false), TxStarted(false);
	DTAPI_RESULT dr;
	int NumBytesRead, FifoLoad, FifoSize;
	int MinFifoLoad;

	// Init channel to hold mode
	dr = m_DtOutp.SetTxControl(DTAPI_TXCTRL_HOLD);
	if ( dr != DTAPI_OK )
		throw Exc(c_ErrFailSetTxControl, ::DtapiResult2Str(dr));

	// Load number of times to play file
	int LoopCnt = m_CmdLineParams.m_LoopCnt;

	// Get current FIFO-size
	dr = m_DtOutp.GetFifoSize(FifoSize);
	if ( dr != DTAPI_OK )
		throw Exc(c_ErrFailGetFifoSize, ::DtapiResult2Str(dr));

	// Set Minimum Fifoload for SDI playout to FifoSize / 2
	MinFifoLoad = (m_CmdLineParams.m_PlayDtSdiFile == false) ? c_MinFifoLoad : FifoSize/2;

#ifdef WIN32
	while ( !_kbhit() ) {
#else
	while ( 1 ) {
#endif

		// Check for free space in hardware FIFO.
		dr = m_DtOutp.GetFifoLoad(FifoLoad);
		if ( (FifoLoad + c_BufSize)>=FifoSize )
		{
			// Sleep to wait for some free space
#ifdef WIN32
			Sleep(2);
#else
			usleep(2000);
#endif 
			continue;
		}

		// Read as much bytes as possible into our buffer
		NumBytesRead = (int)fread( m_pBuf, sizeof(char), c_BufSize, m_pFile );
		if ( NumBytesRead < c_BufSize )
		{
			// Read error or simple end-of-file
			if ( 0 != ferror(m_pFile) )
                throw Exc(c_ErrReadFile);
			else
			{
				EoF = true;
				// Account for possibility of the file size not being a multiple of 4
				NumBytesRead &= ~3;
			}
		}

		if (NumBytesRead > 0) {
			// There is data write it to our output
			//
			// NOTE: Write blocks until all data has been transferred to the output
			dr = m_DtOutp.Write( m_pBuf, NumBytesRead );
			if ( dr != DTAPI_OK )
				throw Exc(c_ErrFailWrite, ::DtapiResult2Str(dr));
		}

		// We wait with starting actual transmission until we have build up a minimum FIFO
		// load. If we reach the end-of-file and only one loop left to go start immediately.
		dr = m_DtOutp.GetFifoLoad(FifoLoad);
		if ( !TxStarted && ( (EoF && LoopCnt==1) || FifoLoad>=MinFifoLoad ) )
		{
			// Start transmission
			dr = m_DtOutp.SetTxControl(DTAPI_TXCTRL_SEND);
			if ( dr != DTAPI_OK )
				throw Exc(c_ErrFailSetTxControl, ::DtapiResult2Str(dr));

			TxStarted = true;
		}

        // If we have reached the end-of-file check is the last data has been transmitted
		if ( EoF && TxStarted && FifoLoad<m_ExitLoad && LoopCnt==1 )
			break;	// Reached end-of-file, last loop and transmitted all data
		else if ( EoF && (LoopCnt>1 || LoopCnt==0) )
		{
			// We reached the end-of-file, but this is not the last loop

			// Loop infinitely?
			if ( LoopCnt!=0 )
				LoopCnt--;

			// Reset file position and end-of-file flag
			::fseek(m_pFile, 0, SEEK_SET);
			EoF = false;

			// Skip DTSDI file header
			if ( m_CmdLineParams.m_PlayDtSdiFile )
			{
				DtSdiFileHdr FileHdr;
				NumBytesRead = (int)fread(&FileHdr, 1, sizeof(FileHdr), m_pFile);
				// Sanity check
				if ( NumBytesRead != sizeof(FileHdr) )
					throw Exc(c_ErrInvalidFileSize);
			}
		}
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player::Play -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int Player::Play(int argc, char* argv[])
{
	int RetValue=0;
	try
	{
		try
		{
			// Parse command line options
			m_CmdLineParams.Init();
			m_CmdLineParams.ParseCommandLine(argc, argv);
		}
		catch ( Exc e ) {
			Log(e, true);
			Log("", true);
			Log("Use -? option for help", true);
			return -1;
		}
		catch ( ... ) {
			return -1;
		}
		if ( m_CmdLineParams.m_ShowHelp )
		{
			// Disable silent mode
			m_CmdLineParams.m_SilentMode = false;
			ShowHelp();
			return RetValue;
		}

		//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Print start message -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

		LogF("DtPlay player V%d.%d.%d (c) 2000-2010 DekTec Digital Video B.V.\n",
			  DTPLAY_VERSION_MAJOR, DTPLAY_VERSION_MINOR, DTPLAY_VERSION_BUGFIX);


		//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open the play file -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

		// Try to open a file for reading
		m_pFile = fopen( m_CmdLineParams.m_FileName, "rb");
		if ( m_pFile == NULL )
			throw Exc(c_ErrFailToOpenFile, m_CmdLineParams.m_FileName);

		//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Auto detect SDI format -.-.-.-.-.-.-.-.-.-.-.-.-.-.

		if ( m_CmdLineParams.m_PlayDtSdiFile )
			AutoDetectSdiFormat();

		//-.-.-.-.-.-.-.-.-.-.-.-.- Attach to the output channel -.-.-.-.-.-.-.-.-.-.-.-.-

		AttachToOutput();

		//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Final initialisation -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

		// Init output
		InitOutput();

		// Create our transfer buffer
		m_pBuf = new char [c_BufSize];

		//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Start playing -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

		// Print start info
		DisplayPlayInfo();
		// Loop file
		LoopFile();
	}
	catch( Exc e )
	{
        Log(e, true);
		RetValue = -1;
	}

	// Detach from channel and device
	m_DtOutp.SetTxControl(DTAPI_TXCTRL_IDLE);
	m_DtOutp.Detach(DTAPI_INSTANT_DETACH);
	m_DtDvc.Detach();

	// Free our buffer
	if ( m_pBuf != NULL ) {
		delete [] m_pBuf;
		m_pBuf = NULL;
	}

	// donot forget to close our file
	if ( m_pFile != NULL ) {
		::fclose(m_pFile);
		m_pFile = NULL;
	}

	return RetValue;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player::ShowHelp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Player::ShowHelp()
{
	Log( "" );
	Log( "Usage:");
	Log( "   DtPlay playfile [-l count] [-r rate] [-t type] [-n number] [-i port]\n" \
		 "          [-m mode] [-mt type] [-mf freq] [-mc coderate] [-ma annex]\n" \
		 "          [-mC constellation] [-mB bandwidth] [-mT txmode] [-mG gaurditv]\n" \
		 "          [-mH hdrmode] [-mP Pilots] [-mF fecframe] [-mI initval]\n" \
		 "          [-ml level_dbm] [-snr snr_db]\n" \
         "          [-ipa ip_address_pair] [-ipp protocol] [-ipn num_tp_per_ip]\n" \
		 "			[-mS Stuffing]\n" \
		 "          [-s] [-?]");
	Log( "");
	Log("Where:");
	Log("   playfile is the name of the file to output");
	Log( "");
	Log("Options:");
	Log("   -l  Number of times to loop the file (0=loop infinitely)");
	Log( "" );
	Log("   -r  Transport-Stream Rate in bps or sample rate in case of IQ-modulation mode");
    Log("         NOTE: set the rate to '0' to playout a file with timestamps");
	Log( "" );
	Log("   -t  Device type to use (default: any output device)");
	Log("         100, 102, 105, 107, 110, 112, 115, 116, 117, 140, 145,");
	Log("         160, 205, 245, 545, 2111, 2145 or 2160");
	Log( "" );
	Log("   -n  Device number to use (default: 1)");
	Log( "" );
	Log("   -i  Port number of the output channel to use");
	Log( "" );
	Log("   -db Doubly buffered output mode (if supported)");
	Log( "" );
	Log("   -m  Transmit Mode (default: 188)");
	Log("         188        188-byte packets in file, 188-byte packets transmitted");
	Log("         ADD16      188-byte packets in file, 204-byte packets transmitted");
	Log("                    (16 dummy bytes are automatically added to the packets)");
	Log("         130        130-byte packets in file, 130-byte packets transmitted");
	Log("         192        192-byte packets in file, 192-byte packets transmitted");
	Log("                    Note: this mode is only supported by the DTA-102");
	Log("         204        204-byte packets in file, 204-byte packets transmitted");
	Log("         MIN16      204-byte packets in file, 188-byte packets transmitted");
	Log("                    (last 16 bytes are invalidated)");
	Log("         RAW        No notion of packets, data is transmitted as is");
	Log("         SDI8B_625  8-Bit/625-line SDI in file, 8-bit SDI transmitted");
	Log("         SDI8B_525  8-Bit/525-line SDI in file, 8-bit SDI transmitted");
	Log("         DTSDI      .dtsdi file, SDI format is auto detected");
	Log( "" );
	Log("   -mt  Modulation type");
	Log("         ATSC       ATSC modulation");
	Log("         BPSK       BPSK modulation");
	Log("         CMMB       CMMB modulation");
	Log("         DTMB       DTMB modulation");
	Log("         DVBS2_8PSK DVB-S.2 8PSK modulation");
	Log("         DVBS2_QPSK DVB-S.2 QPSK modulation");
	Log("         DVBT       DVB-T/H modulation");
	Log("         ISDBT      ISDB-T modulation");
	Log("         IQ         IQ-samples");
	Log("         OFDM       DVB-T/H modulation");
	Log("         QAMn       QAM-n modulation, n=4,16,32,64,128,256");
	Log("         QPSK       DVB-S QPSK modulation");
	Log("         T2MI       T2MI modulation");
	Log( "" );
	Log("   -mf  Modulation carrier frequency in MHz");
	Log( "" );
	Log("   -ml  Output level in dBm (default: -27.5dBm)");
	Log( "" );
	Log("   -mc  Convolutional rate (default: 1/2)");
	Log("         General        : 1/2, 2/3, 3/4, 4/5, 5/6, 6/7 or 7/8");
	Log("         DVB-S2 Specific: 1/3, 2/5, 3/5, 8/9 or 9/10");
	Log("         DTMB Specific  : 0.4, 0.6 or 0.8");
	Log( "" );
	Log("   -ma  J.83 Annex (default: Annex A)");
	Log("         A          J.83 annex A (DVB-C))");
	Log("         B          J.83 annex B ('American QAM'))");
	Log("         C          J.83 annex C ('Japanese QAM'))");
	Log( "" );
	Log("   -mC  ATSC/DVB-H/DVB-T/DTMB constellation (default: QAM64)");
	Log("         QPSK       QPSK modulation");
	Log("         QAM16      QAM-16 modulation");
	Log("         QAM64      QAM-64 modulation");
	Log("         -- DTMB Specific");
	Log("         QAM4NR     QAM-4NR modulation");
	Log("         QAM4       QAM-4 modulation");
	Log("         QAM32      QAM-32 modulation");
	Log("         -- ATSC Specific");
	Log("         VSB8       VSB-8 modulation");
	Log("         VSB16      VSB-16 modulation");
	Log( "" );
	Log("   -mB  DVB-H/DVB-T/DTMB bandwidth (default: 8MHz)");
	Log("         5, 6, 7 or 8 (MHz)");
	Log( "" );
	Log("   -mT  DVB-H/DVB-T transmission mode (default: 8k)");
	Log("         2k, 4k or 8k");
	Log( "" );
	Log("   -mG  DVB-H/DVB-T guard interval (default: 1/4)");
	Log("         1/4, 1/8, 1/16 or 1/32");
	Log( "" );
	Log("   -mH  DTMB frame-header-mode (default: PN420)");
	Log("         PN420, PN595 or PN945");
	Log( "" );
	Log("   -mP  Enable Pilots in DVB-S2 (default: OFF)");
	Log("         OFF        Pilots disabled");
	Log("         ON         Pilots enabled");
	Log( "" );
	Log("   -mF  Long/short FEC frame in DVB-S2 (default: LONG)");
	Log("         SHORT      Short FECFRAME");
	Log("         LONG       Long FECFRAME");
	Log( "" );
	Log("   -mI  Gold sequence initialisation value (default: 0)");
	Log("         0 ... 262143");
	Log( "" );
	Log("   -if  Interpolation filter used in IQ mode (default: OFDM)");
	Log("         OFDM      OFDM interpolation filter");
	Log("         QAM       QAM interpolation filter");
	Log( "" );
	Log("   -snr Enable noise generation and set SNR in dB (e.g. -snr 26.0)");
	Log( "" );
	Log("   -ipa IP address/port (e.g. 192.168.0.1[:5768], port is optional)");
	Log( "" );
	Log("   -ipp IP Protocol (default: UDP)");
	Log("         UDP or RTP");
	Log( "" );
	Log("   -ipn Number of TPs per IP packet (valid range: 1-7, default: 7)");
	Log( "" );
	Log("   -ipt Time-To-Live value (for TX only)");
	Log( "" );

	Log("   -mS  Enable Stuffing (default: OFF)");
	Log("         OFF        ASI Null packet / SDI Black frame stuffing disabled");
	Log("         ON         ASI Null packet / SDI Black frame stuffing enabled");
	Log( "" );

	Log("   -s   Silent mode. No messages printed");
	Log( "" );
	Log("   -?   Display this help");
	Log("");
	Log("NOTE: The first option can only be the playfile or the help option");
	Log("");
	Log("Examples:");
	Log("   DtPlay myfile.ts -r 38000000");
	Log("   DtPlay myfile.ts -r 38000000 -t 100 -n 2 -m RAW ");
	Log("   DtPlay myfile.ts -r 38000000 -t 107 -mt QPSK -mf 1915.0 -mc 7/8 -snr 26.0");
	Log("   DtPlay myfile.dtsdi -t 2144 -i 1 -m DTSDI -l 0 -mS ON");
	LogF("DtPlay version: %d.%d.%d\n", DTPLAY_VERSION_MAJOR, DTPLAY_VERSION_MINOR,
		 DTPLAY_VERSION_BUGFIX);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Application entry point +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- main -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int main(int argc, char* argv[])
{
	int RetValue(0);
	Player ThePlayer;
	try
	{        
		RetValue = ThePlayer.Play(argc, argv);
	}
	catch(...)
	{
		RetValue = -1;
	}
	return RetValue;
}
